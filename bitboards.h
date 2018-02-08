#ifndef BITBOARDS_H
#define BITBOARDS_H

#include <stdint.h>

typedef uint64_t bitboard;

extern const bitboard STRAIGHT_MOVES[2][64];
extern const bitboard DIAGONAL_MOVES[2][64];
extern const bitboard WIN_SPACES[2];
extern const bitboard MOVE_CONES[2][64];

#endif
