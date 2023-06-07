#pragma once
#include <stdint.h>

enum TypePiece {PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING};
enum TypeColor {WHITE, BLACK};
enum TypeState {COOL, CHECK, CHECKMATE}; //statue de la partie

typedef struct Piece {
	TypePiece type;
	TypeColor color;
	int x;
	int y;
	int moved; //A bougé
};


