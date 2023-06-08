#pragma once
#include "Window.h"

// Feel free to add more parameters in these functions
// Main game loop
void game(Window* window);
// Clean everything
void cleanup(Window* window);

// Gets the input
void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize);

// Draws the play board on screen
// The board is always drawn at the top-right corner of the screen
// Should be called before drawing pieces
void drawBoard(Window* window, int squareSize);