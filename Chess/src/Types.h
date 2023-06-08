#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Window.h"

#define SIZE 8

typedef enum EnumPiece { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } TypePiece;
typedef enum EnumColor { WHITE, BLACK } TypeColor;
typedef enum EnumState { COOL, CHECK, CHECKMATE } TypeState; //game status

static const char PiecesNames[6] = { 'P', 'B', 'N', 'R', 'Q', 'K' };

typedef struct StructPiece {
	TypePiece type;
	TypeColor color;
	int x;
	int y;
	int hasMoved; //if the piece has moved in this turn
	int hasMovedOnce; //if the piece has moved one time
	SDL_Texture* texture; 
} Piece;

typedef struct StructBoard {
	int size;
	Piece * **table; //table of 64 squares
	int selectedX, selectedY;
} Board;

typedef struct StructPlayer {
	Piece * *table; //tableau pi�ce joueur
	TypeColor color;
	TypeState check;
	Piece * *eaten; //tableau pi�ce adverse mang�
} Player;

typedef struct StructCase {
	int x;
	int y;
} Case;


