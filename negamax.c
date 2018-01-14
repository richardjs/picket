#include "negamax.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int search_recursive(const struct Board *root, int depth){
	struct Board moves[MAX_MOVES];
	int count = Board_moves(root, moves);

	if(depth == 0){
		if(count == -1){
			return 1;
		}
		return 0;
	}

	int bestScore = INT_MIN;
	for(int i = 0; i < count; i++){
		int score = -search_recursive(&moves[i], depth-1);
		if(score > bestScore){
			bestScore = score;
		}
	}

	return bestScore;
}

void search(const struct Board *board, struct Board *move){
	srand(time(NULL));
	
	struct Board moves[MAX_MOVES];
	int count = Board_moves(board, moves);

	int bestScore = INT_MIN;
	struct Board *bestMove = &moves[0];
	for(int i = 0; i < count; i++){
		int score = -search_recursive(&moves[i], DEPTH);
		if(score > bestScore){
			bestScore = score;
			bestMove = &moves[i];
		}
	}
	fprintf(stderr, "best score: %d\n", bestScore);

	*move = *bestMove;
}
