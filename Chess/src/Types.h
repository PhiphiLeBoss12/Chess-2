#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum EnumPiece { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } TypePiece;
typedef enum EnumColor { WHITE, BLACK } TypeColor;
typedef enum EnumState { COOL, CHECK, CHECKMATE } TypeState; //game status

typedef struct StructPiece {
	TypePiece type;
	TypeColor color;
	int x;
	int y;
	int hasMoved; //if the piece has moved
} Piece;

typedef struct StructBoard {
	int size;
	Piece table[8][8]; //table of 64 squares
} Board;

typedef struct StructPlayer {
	Piece **table;
	TypeColor color;
	TypeState check;
} Player;


