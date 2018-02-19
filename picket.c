#include "board.h"
#include "mctssolver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	fprintf(stderr, "Picket v.2a MCTS Solver, basic safe sims\n");

	if(argc != 2){
		fprintf(stderr, "usage: %s <board>\n", argv[0]);
		return 1;
	}
	fprintf(stderr, "input: %s\n", argv[1]);

	struct Board board;
	struct Board move;

	Board_deserialize(argv[1], &board);

	struct Board moves[MAX_MOVES];
	if(Board_moves(&board, moves) == -1){
		fprintf(stderr, "taking win\n");
		move = moves[0];
	}else{
		search(&board, &move);
	}

	char serialized[BOARD_SERIALIZED_LEN];
	Board_serialize(&move, serialized);
	fprintf(stderr, "output: %s\n", serialized);
	printf(serialized);

	return 0;
}
