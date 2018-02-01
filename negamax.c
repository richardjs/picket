#include "negamax.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

unsigned int nodesSearched;
unsigned int depthNodes[DEPTH];
unsigned long branches;

int search_recursive(const struct Board *root, int depth, struct Board *bestMove){
	struct Board moves[MAX_MOVES];
	int count = Board_moves(root, moves);

	nodesSearched++;
	depthNodes[DEPTH - depth]++;
	branches += count;

	if(depth == 0){
		// evaluation
		if(count == -1){
			return 1;
		}
		return 0;
	}

	int bestScore = INT_MIN;
	struct Board *bestM = NULL;
	for(int i = 0; i < count; i++){
		int score = -search_recursive(&moves[i], depth-1, NULL);

		if(score >= bestScore){
			bestScore = score;
			if(bestMove != NULL){
				bestM = &moves[i];
			}
		}
	}

	if(bestMove != NULL){
		*bestMove = *bestM;
	}

	return bestScore;
}

void search(const struct Board *board, struct Board *move){
	nodesSearched = 0;
	for(int i = 0; i < DEPTH; i++){
		depthNodes[i] = 0;
	}
	int bestScore = search_recursive(board, DEPTH, move);

	fprintf(stderr, "best score: %d\n", bestScore);
	fprintf(stderr, "nodes searched: %d\n", nodesSearched);
	fprintf(stderr, "avg branches: %lu\n", branches/nodesSearched);
}
