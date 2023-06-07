#include <Window.h>
#include <SDL2/SDL_image.h>

Window* initWindow(WindowData* data) {
	int flags = IMG_INIT_AVIF | IMG_INIT_JPG | IMG_INIT_JXL | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return NULL;
	if (IMG_Init(flags) != flags)
		return NULL;

	Window* window = (Window*)malloc(sizeof(Window));

	window->window = SDL_CreateWindow(data->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data->width, data->height, SDL_WINDOW_SHOWN);
	if (!window->window)
		return NULL;

	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
	if (!window->renderer)
		return NULL;

	return window;
}

void destroyWindow(Window* window) {
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
	free(window);

	IMG_Quit();
	SDL_Quit();
}