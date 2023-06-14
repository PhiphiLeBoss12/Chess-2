#pragma once
#include "Types.h"
#include "Window.h"
#include "Board.h"

typedef enum GameState { START, PLAYING, END, STALEMATE, QUIT } GameState;

// Main game loop
void game();

void toggleMusic(Window* window, int* enableMusic);

SDL_Texture** createTextureArray(Window* window);

// Gets the input
void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize);

// Simple helper function
Cell* getPossibilities(Piece* selectedPiece, TypeColor whoPlays, Board* board, int* numPossibilities, LastMove* last);

// Draws the play board on screen
// The board is always drawn at the top-left corner of the screen
// Should be called before drawing pieces
void drawBoard(Window* window, Board* board, SDL_Texture** textures, int squareSize, LastMove last);

// Draws all move possibilities of the selected piece if a piece is selected
void drawPossibilities(Window* window, Board* board, Cell* possibilities, int numPossibilities, int squareSize, Piece* selectedPiece);

// Big ahh function to react when the mouse is clicked
void handleMouseClicking(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays, LastMove* last);