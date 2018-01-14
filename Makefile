CFLAGS=-std=c99 -Wall -D NEGAMAX

all: picket

picket: board.o negamax.o
