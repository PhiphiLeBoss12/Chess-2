#pragma once

#include "Player.h"
#include "Piece.h"
#include "Window.h"

void RandomBot(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays);