#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	fprintf(stderr, "Picket v.1a random\n");

	if(argc != 2){
		fprintf(stderr, "usage: %s <board>\n", argv[0]);
		return 1;
	}
	fprintf(stderr, "input:%s\n", argv[1]);

	struct Board board;
	struct Board move;

	Board_deserialize(argv[1], &board);

	struct Board moves[MAX_MOVES];
	int count = Board_moves(&board, moves);
	if(count == -1){
		fprintf(stderr, "taking win\n");
		move = moves[0];
	}else{
		srand(time(NULL));
		move = moves[rand() % count];
	}

	char serialized[BOARD_SERIALIZED_LEN];
	Board_serialize(&move, serialized);
	fprintf(stderr, "move: %s\n", serialized);
	printf(serialized);

	return 0;
}
