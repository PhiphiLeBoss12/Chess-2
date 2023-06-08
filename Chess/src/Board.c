#include "Types.h"

Board* createBoard(int sizeX, int sizeY) {
	Board *board;
	board->size = sizeX*sizeY;
	for (int i = 0; i < sizeX; i++) {
		board->table[i] = malloc(sizeof(Piece*) * sizeY);
		for (int j = 0; j < sizeY; j++) {
			board->table[i][j] = NULL;
		}
	}
	return board;
}