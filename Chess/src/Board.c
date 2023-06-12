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

Cell getKingPosition(Board* board, TypeColor color) {
	Cell king;
	for (int x = 0; x < board->size; x++) {
		for (int y = 0; y < board->size; y++) {
			if (board->table[x][y] != NULL) {
				if (board->table[x][y]->type == KING && board->table[x][y]->color == color) {
					king.x = x;
					king.y = y;
					return king;
				}
			}
		}
	}
	printf("Error getKingPosition : Your king might be in another castle (he's not in the board)\n");
}


int knightIsMenacing(Board* board, TypeColor color, Cell king) {
	int posThreatx, posThreaty;
	for (int x = -2 ; x <= 2 ; x++) {
		for (int y = -2; y <= 2; y++) {
			if (x != 0 && y != 0) {
				// Computing the position of a potential knight
				posThreatx = king.x + x;
				posThreaty = king.y + y;

				// Checking if the position exists in the grid
				if (posThreatx >= 0 && posThreatx < 8 && posThreaty >= 0 && posThreaty < 8){

					// checking if a knight actually is in the grid
					Piece *piece = board->table[posThreatx][posThreaty];
					if (piece != NULL && piece->type == KNIGHT && piece->color != color) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}


int verifyPieceLine(Piece *piece, int startingPos, int i, Cell king, TypeColor color) {
	if (piece != NULL) {

		if (i == king.y + 1 && piece->type == KING && piece->color != color)
			return 1;

		else if ((piece->type == ROOK || piece->type == QUEEN) && piece->color != color)
			return 1;

		else return 2;
	}
	else return 0;
}

int rookOrQueenIsMenacing(Board* board, TypeColor color, Cell king) {
	Piece *piece;

	int i, status;
	for (i = king.y + 1; i < 8; i++) {
		piece = board->table[king.x][i];
		status = verifyPieceLine(piece, king.y + 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	for (i = king.y - 1; i >=0; i--) {
		piece = board->table[king.x][i];
		status = verifyPieceLine(piece, king.y - 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	for (i = king.x + 1; i < 8; i++) {
		piece = board->table[i][king.y];
		status = verifyPieceLine(piece, king.x + 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	for (i = king.x - 1; i >= 0; i--) {
		piece = board->table[i][king.y];
		status = verifyPieceLine(piece, king.x - 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	return 0;
}

int bishopOrQueenIsMenacing(Board* board, TypeColor color, Cell king) {
}

int isCheck(Board* board, TypeColor color) {
	Cell king = getKingPosition(board, color);

	printf("%d", knightIsMenacing(board, color, king) || rookOrQueenIsMenacing(board, color, king));
	
	return 0;
}