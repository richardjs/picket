#include "mctssolver.h"
#include "thirdrankwin.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define FILTER_DEPTH 5
#define UCTC 3.0
#define UCTW 100.0
#define CAPTURE_PROBABILITY 1.00
#define SIM_THRESHOLD 10
#define SCA 0.0

struct Node{
	struct Board board;

	struct Node *children;
	int childrenCount;

	int visits;
	float value;
};

void Node_init(struct Node *node, const struct Board *board){
	node->board = *board;
	node->children = NULL;
	node->childrenCount = 0;
	node->visits = 0;
	node->value = 0;
}

void Node_update(struct Node *node, float result){
	node->visits++;
	node->value = (node->value*(node->visits-1) + result)/node->visits;
}

float evaluate(const struct Board *board){
	if(trwCheck(board)){
		return -INFINITY;
	}
	return Board_count_pieces(board, board->turn) - Board_count_pieces(board, !board->turn);
}

float negamax(const struct Board *root, int depth, float alpha, float beta){
	if(depth == 0 || trwCheck(root)){
		return evaluate(root);
	}

	struct Board moves[MAX_MOVES];
	int count = Board_moves(root, moves);
	if(count == -1){
		return INFINITY;
	}
	float bestScore = -INFINITY;
	for(int i = 0; i < count; i++){
		float score = -negamax(&moves[i], depth-1, -beta, -alpha);
		if(score > bestScore){
			bestScore = score;
		}
		if(score > alpha){
			alpha = score;
		}
		if(alpha >= beta){
			break;
		}
	}
	return bestScore;
}

void filterNodeChildren(struct Node *node){
	if(trwCheck(&node->board)){
		fprintf(stderr, "filter root TWR loss, aborting filtering\n");
		return;
	}

	float scores[MAX_MOVES];
	float bestScore = -INFINITY;
	for(int i = 0; i < node->childrenCount; i++){
		scores[i] = -negamax(&node->children[i].board, FILTER_DEPTH, -INFINITY, INFINITY);
		if(scores[i] > bestScore){
			bestScore = scores[i];
		}
	}
	for(int i = 0; i < node->childrenCount; i++){
		if(scores[i] < bestScore){
			for(int j = i; j < node->childrenCount-1; j++){
				node->children[j] = node->children[j+1];
				scores[j] = scores[j+1];
			}
			if(node->childrenCount == 1){
				fprintf(stderr, "stopping from filtering all moves\n");
				break;
			}
			i--;
			node->childrenCount--;
		}
	}
	fprintf(stderr, "filter score: %.2f\n", bestScore);
}

float simulate(const struct Board *root){
	struct Board board = *root;	
	struct Board moves[MAX_MOVES];
	int count;
	struct Board plainMoves[MAX_MOVES];
	int plainCount;
	struct Board captureMoves[MAX_MOVES];
	int captureCount;

	while(1){
	//	simMoves++;
		count = Board_moves(&board, moves);
		if(count == -1){
			break;
		}

		bool thirdrankwin = false;
		for(int i = 0; i < count; i++){
			if(trwCheck(&moves[i])){
				//Board_print(&moves[i]);
				//getchar();
				thirdrankwin = true;
				break;
			}
		}
		if(thirdrankwin){
			break;
		}

		Board_sortMoves(&board, moves, count, plainMoves, &plainCount, captureMoves, &captureCount);
		if(plainCount == 0 || (captureCount && (rand() % 100) < (CAPTURE_PROBABILITY * 100))){
			board = captureMoves[rand() % captureCount];
		}else{
			board = plainMoves[rand() % plainCount];
		}

		//board = moves[rand() % count];
		/*
		int fails = 0;
			while(1){
			board = moves[rand() % count];
			struct Board testMoves[MAX_MOVES];
			if(Board_moves(&board, testMoves) != -1){
				   break;
			}
			fails++;
			if(fails == 100){
				   break;
			}
		}
		*/
	}
	return root->turn == board.turn ? 1.0 : -1.0;
}

float solver(struct Node *root){
	if(root->children == NULL){
		struct Board moves[MAX_MOVES];
		root->childrenCount = Board_moves(&root->board, moves);
		if(root->childrenCount == -1){
			Node_update(root, INFINITY);
			return INFINITY;
		}
		root->children = malloc(sizeof(struct Node) * root->childrenCount);
		for(int i = 0; i < root->childrenCount; i++){
			Node_init(&root->children[i], &moves[i]);
		}
	}

	// select best child
	struct Node *selected = NULL;
	float bestUCT = -INFINITY;
	for(int i = 0; i < root->childrenCount; i++){
		struct Node *child = &root->children[i];

		if(child->visits == 0){
			selected = child;
			break;
		}
		
		float transitionProbability = Board_is_capture(&(root->board), &(child->board)) ? CAPTURE_PROBABILITY : 1 - CAPTURE_PROBABILITY;
		float uct = -child->value + sqrtf(UCTC*logf(root->visits) / child->visits) + (UCTW*transitionProbability / (child->visits+1));
		if(uct >= bestUCT){
			selected = child;
			bestUCT = uct;
		}
	}

	// r is from the perspective of *this* node
	float r;
	if(selected->value == INFINITY || selected->value == -INFINITY){
		r = -selected->value;
	}else{
		if(selected->visits < SIM_THRESHOLD){
			r = -simulate(&selected->board);
			Node_update(selected, -r);
		}else{
			r = -solver(selected);
		}
	}

	if(r == -INFINITY){
		for(int i = 0; i < root->childrenCount; i++){
			if(root->children[i].value != INFINITY){
				r = -1;
				break;
			}
		}
	}

	Node_update(root, r);
	return r;
}

void search(const struct Board *board, struct Board *move, unsigned int iterations){
	fprintf(stderr, "MCTS solver search\n");
	fprintf(stderr, "loading thirdrankwin.dat...");
	trwInit();
	fprintf(stderr, " done!\n");

	struct timeval start;
	gettimeofday(&start, NULL);

	srand(time(NULL));
	//srand(6);

	struct Node root;
	Node_init(&root, board);
	struct Board moves[MAX_MOVES];
	root.childrenCount = Board_moves(&root.board, moves);
	root.children = malloc(sizeof(struct Node) * root.childrenCount);
	for(int i = 0; i < root.childrenCount; i++){
		Node_init(&root.children[i], &moves[i]);
	}

	for(int i = 0; i < iterations; i++){
		solver(&root);
	}

	bool infFound = false;
	for(int i = 0; i < root.childrenCount; i++){
		if(root.children[i].value == -INFINITY){
			infFound = true;
			fprintf(stderr, "inf found; no more filtering\n");
			break;
		}
	}
	if(!infFound){
		fprintf(stderr, "filtering %d moves with depth %d...\n", root.childrenCount, FILTER_DEPTH);
		filterNodeChildren(&root);
		fprintf(stderr, "filtered down to %d moves\n", root.childrenCount);
	}

	float bestScore = -INFINITY;
	struct Node *bestChild = NULL;
	for(int i = 0; i < root.childrenCount; i++){
		//fprintf(stderr, "child %d\n", i);
		struct Node child = root.children[i];
		float score = -child.value + (SCA/sqrtf(child.visits));
		//fprintf(stderr, "visits %d\n", child.visits);
		//fprintf(stderr, "value %f\n", child.value);
		//fprintf(stderr, "score %f\n", score);

		if(score >= bestScore){
			//fprintf(stderr, "new best %d\n", i);
			bestScore = score;
			bestChild = &root.children[i];
		}
		//fprintf(stderr, "-----\n");
	}

	*move = bestChild->board;

	struct timeval end;
	gettimeofday(&end, NULL);
	int duration = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;

	// analyze tree for technical output
	int maxDepth = 0;
	struct Node stack[100];
	int stackDepth[100];
	int sp = 0;
	stack[sp] = root;
	stackDepth[sp++] = 0;
	while(sp > 0) {
		struct Node *current = &(stack[--sp]);
		int currentDepth = stackDepth[sp];
		if(currentDepth > maxDepth){
			maxDepth = currentDepth;
		}
		for(int i = 0; i < current->childrenCount; i++){
			stack[sp] = current->children[i];
			stackDepth[sp++] = currentDepth++;
		}
	}

	fprintf(stderr, "score: %f\n", bestScore);
	//fprintf(stderr, "value: %f\n", -bestChild->value);
	fprintf(stderr, "visits: %d\n", bestChild->visits);
	fprintf(stderr, "time: %dms\n", duration);
	fprintf(stderr, "iterations: %d\n", iterations);
	fprintf(stderr, "max tree depth: %d\n", maxDepth);
	fprintf(stderr, "UCTC: %.2f\n", UCTC);
	fprintf(stderr, "UCTW: %.2f\n", UCTW);
	fprintf(stderr, "capture bias: %.2f\n", CAPTURE_PROBABILITY);

	// TODO clean up root
}
