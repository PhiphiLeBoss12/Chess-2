#include "Board.h"
#include <string.h>

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
		for (int j = 0; j < 8; j++) {
			if (board->table[i][j])
				free(board->table[i][j]);
		}
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

Board* createBoardCopy(Board* board) {
	Board* newBoard = (Board*)malloc(sizeof(Board));

	newBoard->table = (Piece***)malloc(board->size * sizeof(Piece**));

	memcpy(newBoard->table, board->table, 8);
	for (int x = 0; x < board->size; x++) {
		newBoard->table[x] = (Piece**)malloc(board->size * sizeof(Piece*));
		memcpy(newBoard->table[x], board->table[x], 8);
		for (int y = 0; y < board->size; y++) {
			newBoard->table[x][y] = (Piece*)malloc(sizeof(Piece));
			if (board->table[x][y])
				*newBoard->table[x][y] = *(board->table[x][y]);
			else
				newBoard->table[x][y] = NULL;
		}
	}

	newBoard->selectedX = board->selectedX;
	newBoard->selectedY = board->selectedY;
	newBoard->size = board->size;

	return newBoard;
}