#include "board.h"
#include <stdio.h>

int main(){
	puts("Picket v.1a ready");
	struct Board board;
	struct Board moves[MAX_MOVES];
	Board_init(&board);
	int count = Board_moves(&board, moves);

	board = moves[0];
	count = Board_moves(&board, moves);
	for(int i = 0; i < count; i++){
		Board_print(&moves[i]);
	}

	return 0;
}
