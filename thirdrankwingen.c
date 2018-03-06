#include "board.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const long BUFFER_SIZE = (1 << 24) * 2 * sizeof(bool);
const bitboard MASK = ((0b111111111111111111111111ul) << 40ul) + (0b111111111111111111111111ul);

int wins = 0;
int notwins = 0;

bool search(const struct Board *root, enum Color attacker){
	struct Board moves[MAX_MOVES];
	int count = Board_moves(root, moves);

	if(count ==  -1){
		if(root->turn == attacker){
			return true;
		}
		return false;
	}

	if(root->turn == attacker){
		for(int i = 0; i < count; i++){
			struct Board move = moves[i];
			bool result = search(&move, attacker);
			if(result){
				return true;
			}
			return false;
		}
	}else{
		for(int i = 0; i < count; i++){
			struct Board move = moves[i];
			//move.bits[!attacker] &= MASK;
			bool result = search(&move, attacker);
			if(!result){
				return false;
			}
		}
		return true;
	}
	return false;
}

int main(){
	puts("generating...");
	srand(1);
	bool *buffer = malloc(BUFFER_SIZE);
	bool result;
	for(bitboard attackPos = 0; attackPos < (1 << 8); attackPos++){
		for(bitboard defensePos = 0; defensePos < (1 << 16); defensePos++){
			struct Board board;
			Board_init(&board);
			board.bits[WHITE] = (attackPos << 40ul);
			board.bits[BLACK] = (defensePos << 48ul);
			board.turn = BLACK;
			result = search(&board, WHITE);
			buffer[attackPos + (defensePos << 8ul)] = result;
			if(result){
				/*
				if(rand() % 1000 == 0){
					Board_print(&board);
					getchar();
				}
				*/
				wins++;
			}else{
				notwins++;
			}

			Board_init(&board);
			board.bits[WHITE] = (defensePos << 0ul);
			board.bits[BLACK] = (attackPos << 16ul);
			board.turn = WHITE;
			result =  search(&board, BLACK);
			buffer[(1 << 24) + attackPos + (defensePos << 8ul)] = result;
			if(result){
				wins++;
			}else{
				notwins++;
			}
		}
		putchar('.');
		fflush(stdout);
	}

	puts("\nwriting...");
	FILE *f = fopen("thirdrankwin.dat", "w");
	fwrite(buffer, BUFFER_SIZE, 1, f);
	fclose(f);

	free(buffer);

	printf("wins: %d\nnot wins: %d\n", wins, notwins);

	return 0;
}
