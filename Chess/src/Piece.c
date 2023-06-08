#include "Piece.h"

Piece *createPiece(TypePiece type, TypeColor color, int x, int y, Board *board) {
	if (!(0 <= x && x < 8 && 0 <= y && y < 8)) { //check if coord is in the table
		printf("Failed : This piece isn't in the table.");
		return NULL;
	}
	Piece* piece;
	piece = malloc(sizeof(Piece));
	piece->type = type;
	piece->color = color;
	piece->x = x;
	piece->y = y;
	piece->hasMoved = 1; //False
	piece->hasMovedOnce = 1; //False
	board->table[x][y] = piece; //put the piece in the board
	return piece;
}

void showTypePiece(TypePiece type) { //function to debug
	switch (type) {
	case PAWN:
		printf("PAWN");
		break;
	case BISHOP:
		printf("BISHOP");
		break;
	case KNIGHT:
		printf("KNIGHT");
		break;
	case ROOK:
		printf("ROOK");
		break;
	case QUEEN:
		printf("QUEEN");
		break;
	case KING:
		printf("KING");
		break;
	}
}


void showCoordPiece(Piece* piece) {
	switch (piece->x) {
	case 0:
		printf("A");
		break;
	case 1:
		printf("B");
		break;
	case 2:
		printf("C");
		break;
	case 3:
		printf("D");
		break;
	case 4:
		printf("E");
		break;
	case 5:
		printf("F");
		break;
	case 6:
		printf("G");
		break;
	case 7:
		printf("H");
		break;
	}
	printf("%d",piece->y+1);
}

void showPiece(Piece* piece) { 
	showTypePiece(piece->type);
	printf(": ");
	showCoordPiece(piece);
	printf("\n");
}

void movePiece(Piece* piece, int x, int y, Board *board) {
	board->table[x][y] = NULL;
	piece->x = x;
	piece->y = y;
	board->table[x][y] = piece;
}

Case *movePosibilitiesPawn(Piece* piece, Board *board) {
	Case* tab;
	int index = 0;
	tab = malloc(sizeof(Case) * 4);
	Case casePos;
	//To move 1 case
	if (board->table[piece->x][piece->y + 1] == NULL) { 
		casePos.x = piece->x;
		casePos.y = piece->y + 1;
		tab[index] = casePos;
		index++;
	}
	//To move 2 cases
	if (board->table[piece->x][piece->y + 1] == NULL && board->table[piece->x][piece->y + 2] == NULL && piece->hasMovedOnce != 0) { 
		casePos.x = piece->x;
		casePos.y = piece->y + 2;
		tab[index] = casePos;
		index++;
	}
	//Left move
	if (piece->x - 1 >= 0) { //Verify border
		if (board->table[piece->x - 1][piece->y + 1] == NULL || board->table[piece->x - 1][piece->y + 1]->color != piece->color) {
			casePos.x = piece->x - 1;
			casePos.y = piece->y + 1;
			tab[index] = casePos;
			index++;
		}
	}
	//Right move
	if (piece->x + 1 >= 0) { //Verify border
		if (board->table[piece->x + 1][piece->y + 1] == NULL || board->table[piece->x + 1][piece->y + 1]->color != piece->color) {
			casePos.x = piece->x + 1;
			casePos.y = piece->y + 1;
			tab[index] = casePos;
			index++;
		}
	}
	return tab;
}



