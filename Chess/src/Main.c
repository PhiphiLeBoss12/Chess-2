#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[])
{
    printf("Hello world!\n");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("a", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL, NULL);
    SDL_Quit();
    return 0;
}