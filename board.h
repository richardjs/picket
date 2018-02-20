#ifndef BOARD_H
#define BOARD_H

#include "bitboards.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_MOVES 48
#define BOARD_SERIALIZED_LEN 66

typedef uint64_t bitboard;

enum Color{WHITE=0, BLACK=1};

struct Board{
	bitboard bits[2];
	enum Color turn;
};

void Board_init(struct Board *board);
int Board_moves(const struct Board *board, struct Board boards[]);

void Board_serialize(const struct Board *board, char string[]);
void Board_deserialize(const char string[], struct Board *board);
void Board_print(const struct Board *board);

int Board_count_pieces(const struct Board *board, enum Color color);
bool Board_is_capture(const struct Board *first, const struct Board *second);

#endif
