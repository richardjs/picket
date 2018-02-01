#ifndef NEGAMAX_H
#define NEGAMAX_H

#include "board.h"

#define DEPTH 5

void search(const struct Board *board, struct Board *move);

#endif
