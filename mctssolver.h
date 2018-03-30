#ifndef MCTSSOLVER_H
#define MCTSSOLVER_H

#include "board.h"

void search(const struct Board *board, struct Board *move, unsigned int iterations);

#endif
