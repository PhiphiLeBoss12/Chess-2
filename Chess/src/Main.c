#include <stdio.h>
#include <SDL2/SDL.h>
#include "Piece.h"
#include "Window.h"
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    Board *board;
    board->size = 64;
    Piece* piece;
    piece = createPiece(PAWN, WHITE, 0,0);
    showPiece(piece);

    return 0;
}