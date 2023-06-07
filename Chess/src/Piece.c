#include "Piece.h"

Piece *createPiece(TypePiece type, TypeColor color, int x, int y) {
	Piece* piece;
	piece = malloc(sizeof(Piece));
	piece->type = type;
	piece->color = color;
	piece->x = x;
	piece->y = y;
	piece->hasMoved = 1; //False
	return piece;
}

void showTypePiece(TypePiece type) {
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

void showPiece(Piece* piece) {
	showTypePiece(piece->type);
	printf(": %d,%d\n", piece->x, piece->y);
}