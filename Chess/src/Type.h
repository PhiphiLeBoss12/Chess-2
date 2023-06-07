#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum EnumPiece{PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING} TypePiece;
typedef enum EnumColor {WHITE, BLACK} TypeColor;
typedef enum EnumState {COOL, CHECK, CHECKMATE} TypeState; //statut de la partie

typedef struct StructPiece {
	TypePiece type;
	TypeColor color;
	int x;
	int y;
	int hasMoved; //A bougé
} Piece;

typedef struct StructBoard {
	int size;
	Piece table[8][8]; //Tableau de 64
} Board;

typedef struct StructPlayer {
	Piece **table;
	TypeColor color;
	TypeState check;
} Player;


