#include <Window.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

Window* initWindow(WindowData* data) {
	int flags = IMG_INIT_AVIF | IMG_INIT_JPG | IMG_INIT_JXL | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL! SDL error: %s\n", SDL_GetError());
		return NULL;
	}
	if (IMG_Init(flags) != flags) {
		printf("Failed to initialize SDL_image! IMG error: %s\n", IMG_GetError());
		return NULL;
	}

	Window* window = (Window*)malloc(sizeof(Window));

	window->window = SDL_CreateWindow(data->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data->width, data->height, SDL_WINDOW_SHOWN);
	if (!window->window) {
		printf("Failed to create window! SDL error: %s\n", SDL_GetError());
		return NULL;
	}

	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
	if (!window->renderer) {
		printf("Failed to create window! SDL error: %s\n", SDL_GetError());
		return NULL;
	}

	return window;
}

void destroyWindow(Window* window) {
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
	free(window);

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* createTexture(Window* window, const char* path) {
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		printf("Failed to create surface from file! IMG error: %s\n", IMG_GetError());
		return NULL;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->renderer, surface);
	if (!texture) {
		printf("Failed to create texture from surface! SDL error: %s\n", SDL_GetError());
		return NULL;
	}
}

void destroyTexture(SDL_Texture* texture) {
	SDL_DestroyTexture(texture);
}