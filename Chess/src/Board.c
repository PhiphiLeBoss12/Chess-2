#include "Board.h"

/*
* Returns a board form its size. The board will be a square.
* size : the length of the square
*/
Board* createBoard(int size) {
	Board* board = (Board*)malloc(sizeof(Board));

	board->table = (Piece***)malloc(size * sizeof(Piece**));
	for (int y = 0; y < size; y++) {
		board->table[y] = (Piece**)malloc(size * sizeof(Piece*));
		for (int x = 0; x < size; x++) {
			board->table[x][y] = NULL;
		}
	}

	board->selectedX = -1;
	board->selectedY = -1;

	return board;
}


Board* displayBoardConsole(Board* board) {
	for (int x = 0; x < board->size; x++) {
		for (int y = 0; y < board->size; y++) {
			if (board->table[x][y] != NULL) {
				printf("$c ", PiecesNames[(board->table[x][y])->type]);
			}
			else {
				printf("X");
			}
			printf("\n");
		}
	}
}
