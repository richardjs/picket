#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	fprintf(stderr, "Picket v.1a\n");

	if(argc != 2){
		fprintf(stderr, "usage: %s <board>\n", argv[0]);
		return 1;
	}
	fprintf(stderr, "input:%s\n", argv[1]);

	srand(time(NULL));

	struct Board board;
	struct Board moves[MAX_MOVES];
	char serialized[BOARD_SERIALIZED_LEN];

	Board_deserialize(argv[1], &board);
	Board_serialize(&board, serialized);

	int count = Board_moves(&board, moves);

	Board_serialize(&moves[rand() % count], serialized);
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
