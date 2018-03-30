#include "board.h"
#include "mctssolver.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_ITERATIONS 100000

int main(int argc, char *argv[]){
	fprintf(stderr, "Picket v1 (%s %s)\n", __DATE__, __TIME__);

	int opt;
	unsigned int iterations = DEFAULT_ITERATIONS;
	while((opt = getopt(argc, argv, "i:")) != -1 ){
		switch(opt){
			case 'i':
				iterations = atoi(optarg);		
				break;
			default:
				fprintf(stderr, "usage: %s [-i iterations] <board>\n", argv[0]);
				return 1;
		}
	}

	if(argc == optind || iterations < 1){
		fprintf(stderr, "usage: %s [-i iterations] <board>\n", argv[0]);
		return 1;
	}
	fprintf(stderr, "input: %s\n", argv[optind]);

	struct Board board;
	struct Board move;

	Board_deserialize(argv[optind], &board);
	Board_print(&board);

	struct Board moves[MAX_MOVES];
	if(Board_moves(&board, moves) == -1){
		fprintf(stderr, "taking win\n");
		move = moves[0];
	}else{
		search(&board, &move, iterations);
	}

	char serialized[BOARD_SERIALIZED_LEN];
	Board_serialize(&move, serialized);
	fprintf(stderr, "output: %s\n", serialized);
	Board_print(&move);
	printf(serialized);

	return 0;
}
