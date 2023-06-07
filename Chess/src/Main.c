#include <stdio.h>
#include <SDL2/SDL.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    printf("Hello world!\n");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Chess 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}