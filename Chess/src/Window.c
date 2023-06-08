#include <Window.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

Window* initWindow(const char* title, unsigned int width, unsigned int height) {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL! SDL error: %s\n", SDL_GetError());
		return NULL;
	}
	if (IMG_Init(flags) != flags) {
		printf("Failed to initialize SDL_image! IMG error: %s\n", IMG_GetError());
		return NULL;
	}

	Window* window = (Window*)malloc(sizeof(Window));

	window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!window->window) {
		printf("Failed to create window! SDL error: %s\n", SDL_GetError());
		return NULL;
	}

	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
	if (!window->renderer) {
		printf("Failed to create window! SDL error: %s\n", SDL_GetError());
		return NULL;
	}

	window->shouldClose = 0;
	window->mousePosX = 0;
	window->mousePosY = 0;
	window->mouseRightButton = 0;
	window->mouseLeftButton = 0;
	window->mouseMiddleButton = 0;

	return window;
}

void destroyWindow(Window* window) {
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
	free(window);

	IMG_Quit();
	SDL_Quit();
}

void handleEvents(Window* window) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				window->shouldClose = 1;
			break;

		case SDL_MOUSEMOTION:
			window->mousePosX = event.motion.x;
			window->mousePosY = event.motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT: window->mouseLeftButton = 1; break;
			case SDL_BUTTON_RIGHT: window->mouseRightButton = 1; break;
			case SDL_BUTTON_MIDDLE: window->mouseMiddleButton = 1; break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT: window->mouseLeftButton = 0; break;
			case SDL_BUTTON_RIGHT: window->mouseRightButton = 0; break;
			case SDL_BUTTON_MIDDLE: window->mouseMiddleButton = 0; break;
			}
			break;

		case SDL_KEYDOWN:
			window->keyDown = event.key.keysym.sym;
			break;
		case SDL_KEYUP:
			window->keyDown = SDLK_UNKNOWN;
			break;
		}
	}
}

void clear(Window* window) {
	SDL_RenderClear(window->renderer);
}

void presentWindow(Window* window) {
	SDL_RenderPresent(window->renderer);
}

void setDrawColor(Window* window, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	SDL_SetRenderDrawColor(window->renderer, r, g, b, a);
}

void drawRect(Window* window, Rect* rect) {
	SDL_Rect sdlRect = { rect->x, rect->y, rect->width, rect->height };
	SDL_RenderFillRect(window->renderer, &sdlRect);
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
	return texture;
}

void drawTexture(Window* window, Rect* rect, SDL_Texture* texture) {
	SDL_Rect sdlRect = { rect->x, rect->y, rect->width, rect->height };
	if (SDL_RenderCopyEx(window->renderer, texture, NULL, &sdlRect, rect->angle, NULL, SDL_FLIP_NONE) < 0)
		printf("Failed to copy texture! SDL error: %s\n", SDL_GetError());
}

void destroyTexture(SDL_Texture* texture) {
	SDL_DestroyTexture(texture);
}