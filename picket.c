#include "board.h"
#include "negamax.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	fprintf(stderr, "Picket v.1a\n");

	if(argc != 2){
		fprintf(stderr, "usage: %s <board>\n", argv[0]);
		return 1;
	}
	fprintf(stderr, "input:%s\n", argv[1]);

	struct Board board;
	struct Board move;

	Board_deserialize(argv[1], &board);

	search(&board, &move);

	char serialized[BOARD_SERIALIZED_LEN];
	Board_serialize(&move, serialized);
	fprintf(stderr, "move: %s\n", serialized);
	printf(serialized);

	/*
	board = moves[0];
	count = Board_moves(&board, moves);
	for(int i = 0; i < count; i++){
		Board_print(&moves[i]);
		Board_serialize(&moves[i], serialized);
		printf("%s\n", serialized);
	}
	*/

	return 0;
}
