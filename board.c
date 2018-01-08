#include "board.h"

void Board_init(struct Board *board){
	board->bits[0] = 0b11111111ul;
	board->bits[1] = (0b11111111ul << 56);
	board->turn = WHITE;
}

void Board_moves(const struct Board *board, struct Board boards[]){
	
}
