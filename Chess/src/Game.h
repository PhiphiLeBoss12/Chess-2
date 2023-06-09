#pragma once
#include "Window.h"
#include "Board.h"

// Main game loop
void game();

// Gets the input
void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize);

// Simple helper function
Case* getPossibilities(Piece* selectedPiece, TypeColor whoPlays, Board* board, int* numPossibilities);

// Draws the play board on screen
// The board is always drawn at the top-left corner of the screen
// Should be called before drawing pieces
void drawBoard(Window* window, Board* board, int squareSize);

// Draws all move possibilities of the selected piece if a piece is selected
void drawPossibilities(Window* window, Board* board, Case* possibilities, int numPossibilities, int squareSize);