#include <stdio.h>
#include <SDL2/SDL.h>
#include "Piece.h"
#include "Window.h"
#include <math.h>

int main(int argc, char* argv[])
{
    Window* window = initWindow("Chess 2", 800, 800);
    game(window);
    cleanup(window);

    return 0;
}