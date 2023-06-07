#include <Window.h>

Window* initWindow(WindowData* data) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
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
	SDL_Quit();
}