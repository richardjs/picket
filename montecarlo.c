#include "bitboards.h"
#include "montecarlo.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define bitscan(x) __builtin_ctzl(x)
#define lbitscan(x) (63 - __builtin_clzl(x))

#define SIMS 100000

int simMoves = 0;

int winQuickSearch(const struct Board *root, enum Color testTurn, int space, int maxDepth){
	// stop at max depth, returning 0
	if(maxDepth == 0){
		return 0;
	}

	// copy board and apply cone
	struct Board board = *root;
	bitboard cone = MOVE_CONES[testTurn][space];
	board.bits[board.turn] &= cone;
	board.bits[!board.turn] &= cone;
	// if the cone filters out non-test turn pins, call it a win for the test turn
	if(board.bits[!testTurn] == 0){
		return board.turn == testTurn ? 0 : 0;
	}
	
	// generate next moves
	struct Board moves[MAX_MOVES + 1];
	int count = Board_moves(&board, moves);
	// if we have a win, return it
	if(count == -1){
		return 1;
	}
	// let the non-test turn skip turns, instead of moving
	if(board.turn != testTurn){
		moves[count] = board;
		moves[count].turn = !board.turn;
		count++;
	}

/*
	int nextSpace;
	if(testTurn == WHITE){
		nextSpace = lbitscan(board.bits[WHITE]);
	}else{
		nextSpace = bitscan(board.bits[BLACK]);
	}
*/

	// search for best result for us
	int result = -1;
	for(int i = 0; i < count; i++){
		int r = winQuickSearch(&moves[i], testTurn, space, maxDepth-1);
		// -1 indicates a loss for the next tier, so a win for us
		if(r == -1){
			return 1;
		}
		// if we have a 0, we have at least one move that's not a loss
		if(r == 0){
			result = 0;
		}
	}
	return result;
}

enum Color simulate(const struct Board *root){
	struct Board board = *root;	
	struct Board moves[MAX_MOVES];
	int count;
	while(1){
		simMoves++;
		count = Board_moves(&board, moves);
		if(count == -1){
			break;
		}

		/*
		int space = board.turn == WHITE ? lbitscan(board.bits[WHITE]) : bitscan(board.bits[BLACK]);
		if(winQuickSearch(&board, board.turn, space, 20) == 1){
			return board.turn;
		}
		*/

		board = moves[rand() % count];
	}
	return board.turn;
}

void search(const struct Board *root, struct Board *move){
	fprintf(stderr, "montecarlo search\n");

	struct timeval start;
	gettimeofday(&start, NULL);

	srand(time(NULL));

	struct Board moves[MAX_MOVES];
	int count = Board_moves(root, moves);

	if(count == -1){
		fprintf(stderr, "taking win\n");
		*move = moves[0];
		return;
	}
	
	int simsPerMove = SIMS/count;
	int wins[MAX_MOVES] = {0};

	for(int i = 0; i < count; i++){
		for(int j = 0; j < simsPerMove; j++){
			enum Color winner = simulate(&moves[i]);
			if(winner == root->turn){
				wins[i]++;
			}
		}
	}

	int mostWins = -1;
	int bestMove = 0;
	int winsSum = 0;
	int leastWins = SIMS;
	for(int i = 0; i < count; i++){
		winsSum += wins[i];

		if(wins[i] > mostWins){
			mostWins = wins[i];
			bestMove = i;
		}

		if(wins[i] < leastWins){
			leastWins = wins[i];
		}
	}

	struct timeval end;
	gettimeofday(&end, NULL);

	int duration = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;

	fprintf(stderr, "score: %f\n", 2.0*mostWins/simsPerMove - 1);
	fprintf(stderr, "mean score: %f\n", 2.0*((float)winsSum/count/simsPerMove) - 1);
	fprintf(stderr, "worst score: %f\n", 2.0*leastWins/simsPerMove - 1);
	fprintf(stderr, "time: %dms\n", duration);
	fprintf(stderr, "simulations: %d\n", SIMS);
	fprintf(stderr, "possible moves: %d\n", count);
	fprintf(stderr, "sims per move: %d\n", simsPerMove);
	fprintf(stderr, "mean sim depth: %.2f\n", (float)simMoves / (simsPerMove*count));

	*move = moves[bestMove];
}
