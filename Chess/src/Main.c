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

    Window* window = initWindow("Chess 2", 800, 600);
    int iter = 0;
    while (!window->shouldClose) {
        setDrawColor(window, 64, 64, 64, 255);
        clear(window);

        Rect rect = { (sin(iter) + 5) * 50, (cos(iter) + 5) * 50, 64, 64, 0.0f };
        setDrawColor(window, 255, 0, 255, 255);
        drawRect(window, &rect);
        iter++;

        presentWindow(window);
        handleEvents(window);
    }
    destroyWindow(window);

    return 0;
}