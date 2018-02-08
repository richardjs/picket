CFLAGS=-std=c99 -Wall -D NEGAMAX -O3

all: picket

picket: bitboards.o board.o montecarlo.o

clean:
	rm *.o
	rm picket
