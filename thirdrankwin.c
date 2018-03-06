#include "board.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const long BUFFER_SIZE = (1 << 24) * 2 * sizeof(bool);

bool *data;

void trwInit(){
	data = malloc(BUFFER_SIZE);

	FILE *f = fopen("thirdrankwin.dat", "r");
	fread(data, BUFFER_SIZE, 1, f);
	fclose(f);
}

void trwDestroy(){
	free(data);
}

bool trwCheck(const struct Board *board){
	if(board->turn == WHITE){
		bitboard attackPos = (board->bits[WHITE] >> 40ul);
		bitboard defensePos = (board->bits[BLACK] >> 48ul);
		return data[attackPos + (defensePos << 8ul)];
	}else{
		bitboard attackPos = ((board->bits[BLACK] >> 16ul) & 0b11111111);
		bitboard defensePos = (board->bits[WHITE] & 0b1111111111111111);
		return data[(1 << 24) + attackPos + (defensePos << 8ul)];
	}
}
