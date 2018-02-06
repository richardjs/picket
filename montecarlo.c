#include "montecarlo.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define SIMS 100000

int simMoves = 0;

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
	for(int i = 0; i < count; i++){
		if(wins[i] > mostWins){
			mostWins = wins[i];
			bestMove = i;
		}
	}

	struct timeval end;
	gettimeofday(&end, NULL);

	int duration = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;

	fprintf(stderr, "score: %f\n", 2.0*mostWins/simsPerMove - 1);
	fprintf(stderr, "time: %dms\n", duration);
	fprintf(stderr, "simulations: %d\n", SIMS);
	fprintf(stderr, "possible moves: %d\n", count);
	fprintf(stderr, "sims per move: %d\n", simsPerMove);
	fprintf(stderr, "mean sim depth: %.2f\n", (float)simMoves / (simsPerMove*count));

	*move = moves[bestMove];
}
