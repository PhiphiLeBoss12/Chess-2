#include "Board.h"

/*
* Returns a board form its size. The board will be a square.
* size : the length of the square
*/
Board* createBoard(int size) {
	Board* board = (Board*)malloc(sizeof(Board));

	board->table = (Piece***)malloc(size * sizeof(Piece**));
	for (int x = 0; x < size; x++) {
		board->table[x] = (Piece**)malloc(size * sizeof(Piece*));
		for (int y = 0; y < size; y++) {
			board->table[x][y] = NULL;
		}
	}

	board->size = size;
	board->selectedX = -1;
	board->selectedY = -1;

	return board;
}

void destroyBoard(Board* board) {
	for (int i = 0; i < 8; i++) {
		free(board->table[i]);
	}
	free(board->table);
	free(board);
}

void displayBoardConsole(Board* board) {
	for (int y = 0; y < board->size; y++) {
		for (int x = 0; x < board->size; x++) {
			if (board->table[x][y] != NULL) {
				printf("%c ", PiecesNames[(board->table[x][y])->type]);
			}
			else {
				printf("X ");
			}
		}
		printf("\n");
	}
}
