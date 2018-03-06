CFLAGS=-std=c99 -Wall -lm -O3

all: picket

picket: bitboards.o board.o mctssolver.o thirdrankwin.o

thirdrankwingen: bitboards.o board.o

clean:
	rm *.o
	rm picket
