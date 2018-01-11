#include "bitboards.h"
#include "board.h"
#include <stdio.h>

#define bitscan(x) __builtin_ctzl(x)

void Board_init(struct Board *board){
	board->bits[WHITE] = 0b1111111111111111ul;
	board->bits[BLACK] = (0b1111111111111111ul << 48);
	board->turn = WHITE;
}

int Board_moves(const struct Board *board, struct Board boards[]){
	enum Color turn = board->turn;
	bitboard pieces = board->bits[turn];
	bitboard openSpaces = ~(board->bits[WHITE] | board->bits[BLACK]);

	int count = 0;
	while(pieces > 0){
		bitboard piece = bitscan(pieces);
		pieces ^= (1ul << piece);

		bitboard moves = STRAIGHT_MOVES[turn][piece];
		moves &= openSpaces;
		while(moves > 0){
			bitboard move = bitscan(moves);
			moves ^= (1ul << move);

			struct Board *child = &boards[count++];
			child->bits[turn] = board->bits[turn];
			child->bits[turn] ^= (1ul << piece);
			child->bits[turn] |= (1ul << move);
			child->bits[!turn] = board->bits[!turn];
			child->turn = !turn;

			if(child->bits[turn] & WIN_SPACES[turn]){
				boards[0] = *child;
				return -1;
			}
		}

		moves = DIAGONAL_MOVES[turn][piece];
		moves &= ~(board->bits[turn]);
		while(moves > 0){
			bitboard move = bitscan(moves);
			moves ^= (1ul << move);

			struct Board *child = &boards[count++];
			child->bits[turn] = board->bits[turn];
			child->bits[turn] ^= (1ul << piece);
			child->bits[turn] |= (1ul << move);
			child->bits[!turn] = board->bits[!turn];
			child->bits[!turn] &= ~(1ul << move);
			child->turn = !board->turn;

			if(child->bits[turn] & WIN_SPACES[turn]){
				boards[0] = *child;
				return -1;
			}
		}
	}

	return count;
}

void Board_print(const struct Board *board){
	printf("----------------\n");
	for(int y = 7; y >= 0; y--){
		for(int x = 0; x < 8; x++){
			bitboard space = 1ul << (y*8 + x);
			char c;
			if((board->bits[WHITE] & space) && !(board->bits[BLACK] & space)){
				c = 'W';
			}else if(!(board->bits[WHITE] & space) && (board->bits[BLACK] & space)){
				c = 'B';
			}else if((board->bits[WHITE] & space) && (board->bits[BLACK] & space)){
				c = '!';
			}else{
				c = '-';
			}
			putchar(c);
			if(x != 7){
				putchar(' ');
			}
		}
		if(board->turn == WHITE && y == 0){
			printf("  T");
		}else if(board->turn == BLACK && y == 7){
			printf("  T");
		}
		putchar('\n');
	}
	printf("----------------\n");
}
