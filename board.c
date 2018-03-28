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

			if((child->bits[turn] & WIN_SPACES[turn]) || child->bits[!turn] == 0){
				boards[0] = *child;
				return -1;
			}
		}
	}

	return count;
}

void Board_serialize(const struct Board *board, char string[]){
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			bitboard space = 1ul << (y*8 + x);
			char c;
			if((board->bits[WHITE] & space) && !(board->bits[BLACK] & space)){
				c = '1';
			}else if(!(board->bits[WHITE] & space) && (board->bits[BLACK] & space)){
				c = '2';
			}else if((board->bits[WHITE] & space) && (board->bits[BLACK] & space)){
				c = '!';
			}else{
				c = '0';
			}
			string[y*8 + x] = c;
		}
	}
	string[64] = board->turn ? '2' : '1';
	string[65] = '\0';
}

void Board_deserialize(const char string[], struct Board *board){
	board->bits[WHITE] = 0ul;
	board->bits[BLACK] = 0ul;
	
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			switch(string[y*8 + x]){
				case '1':
					board->bits[WHITE] |= (1ul << (y*8 + x));
					break;
				case '2':
					board->bits[BLACK] |= (1ul << (y*8 + x));
					break;
				default:
					break;
			}
		}
	}
	board->turn = string[64] == '1' ? WHITE : BLACK;
}

int Board_count_pieces(const struct Board *board, enum Color color){
	int count = 0;
	bitboard pieces = board->bits[color];
	while(pieces){
		count++;
		pieces ^= (1ul << bitscan(pieces));
	}
	return count;
}

bool Board_is_capture(const struct Board *first, const struct Board *second){
	return Board_count_pieces(first, second->turn) > Board_count_pieces(second, second->turn);
}

void Board_sortMoves(const struct Board* board,
		const struct Board moves[], const int moveCount,
		struct Board plainMoves[], int *plainCount,
		struct Board captureMoves[], int *captureCount){
	*plainCount = 0;
	*captureCount = 0;
	for(int i = 0; i < moveCount; i++){
		if(Board_is_capture(board, &moves[i])){
			captureMoves[(*captureCount)++] = moves[i];
		}else{
			plainMoves[(*plainCount)++] = moves[i];
		}
	}
}

void Board_print(const struct Board *board){
	fprintf(stderr, "----------------\n");
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
			putc(c, stderr);
			if(x != 7){
				putc(' ', stderr);
			}
		}
		if(board->turn == WHITE && y == 0){
			fprintf(stderr, "  T");
		}else if(board->turn == BLACK && y == 7){
			fprintf(stderr, "  T");
		}
		putc('\n', stderr);
	}
	fprintf(stderr, "----------------\n");
}
