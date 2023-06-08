#pragma once
#include "Window.h"

// Feel free to add more parameters in these functions
// Main game loop
void game(Window* window);
// Clean everything
void cleanup(Window* window);

// Gets the input
void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize);