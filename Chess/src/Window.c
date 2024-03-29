#include <Window.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Global variable (very bad)
unsigned int NumWindows = 0;

void initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Failed to initialize SDL! SDL error: %s\n", SDL_GetError());
		return NULL;
	}
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
		printf("Failed to initialize SDL_image! IMG error: %s\n", IMG_GetError());
		return NULL;
	}
	if (TTF_Init() < 0) {
		printf("Failed to initialize SDL_ttf! TTF error: %s\n", TTF_GetError());
		return NULL;
	}
	if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS) < 0) {
		printf("Failed to initialize SDL_mixer! MIX error: %s\n", Mix_GetError());
		return NULL;
	}
	Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 2048);
}

void quitSDL() {
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

Window* initWindow(const char* title, unsigned int width, unsigned int height, int decorations) {
	if (NumWindows == 0)
		initSDL();

	Window* window = (Window*)malloc(sizeof(Window));

	unsigned int windowFlags = 0;
	if (!decorations)
		windowFlags = SDL_WINDOW_BORDERLESS;
	window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
	if (!window->window) {
		printf("Failed to create window! SDL error: %s\n", SDL_GetError());
		return NULL;
	}

	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
	if (!window->renderer) {
		printf("Failed to create window! SDL error: %s\n", SDL_GetError());
		return NULL;
	}
	SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);

	window->font = TTF_OpenFont("OpenSans-Regular.ttf", 128);

	window->width = width;
	window->height = height;

	window->shouldClose = 0;

	window->mousePosX = 0;
	window->mousePosY = 0;
	window->mouseRightButton = 0;
	window->mouseLeftButton = 0;
	window->mouseMiddleButton = 0;

	window->keyDown = SDLK_UNKNOWN;

	NumWindows++;

	return window;
}

void destroyWindow(Window* window) {
	TTF_CloseFont(window->font);
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
	free(window);

	NumWindows--;

	if (NumWindows == 0)
		quitSDL();
}

void handleEvents(Window* window) {
	unsigned int windowId = SDL_GetWindowID(window->window);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT:
			if (event.window.windowID != windowId)
				break;

			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				window->shouldClose = 1;
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				window->width = event.window.data1;
				window->height = event.window.data2;
			}
			break;

		case SDL_MOUSEMOTION:
			if (event.motion.windowID != windowId)
				break;

			window->mousePosX = event.motion.x;
			window->mousePosY = window->height - event.motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.windowID != windowId)
				break;

			switch (event.button.button) {
			case SDL_BUTTON_LEFT: window->mouseLeftButton = 1; break;
			case SDL_BUTTON_RIGHT: window->mouseRightButton = 1; break;
			case SDL_BUTTON_MIDDLE: window->mouseMiddleButton = 1; break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.windowID != windowId)
				break;

			switch (event.button.button) {
			case SDL_BUTTON_LEFT: window->mouseLeftButton = 0; break;
			case SDL_BUTTON_RIGHT: window->mouseRightButton = 0; break;
			case SDL_BUTTON_MIDDLE: window->mouseMiddleButton = 0; break;
			}
			break;

		case SDL_KEYDOWN:
			if (event.key.windowID != windowId)
				break;

			window->keyDown = event.key.keysym.sym;
			break;
		case SDL_KEYUP:
			if (event.key.windowID != windowId)
				break;

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
	// We want to set the origin of the window at the bottom-left
	SDL_Rect sdlRect = { rect->x,  window->height - rect->y - rect->height, rect->width, rect->height };
	if (SDL_RenderFillRect(window->renderer, &sdlRect) < 0)
		printf("Failed to fill rect! SDL error: %s\n", SDL_GetError());
}

void drawCircle(Window* window, int x, int y, int radius) {
	// Shamelessly stolen from https://stackoverflow.com/questions/65723827/sdl2-function-to-draw-a-filled-circle
	for (int w = 0; w < radius * 2; w++) {
		for (int h = 0; h < radius * 2; h++) {
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(window->renderer, x + dx, window->height - y - dy);
			}
		}
	}
}

void drawLine(Window* window, int thiccness, int x1, int y1, int x2, int y2) {
	for (int i = 0; i < thiccness; i++)
		SDL_RenderDrawLine(window->renderer, x1 + (i - thiccness / 2), y1, x2 + (i - thiccness / 2), y2);
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
	SDL_FreeSurface(surface);
	return texture;
}

void drawTexture(Window* window, Rect* rect, SDL_Texture* texture) {
	// We want to set the origin of the window at the bottom-left
	SDL_Rect sdlRect = { rect->x, window->height - rect->y - rect->height, rect->width, rect->height };
	if (SDL_RenderCopyEx(window->renderer, texture, NULL, &sdlRect, rect->angle, NULL, SDL_FLIP_NONE) < 0)
		printf("Failed to copy texture! SDL error: %s\n", SDL_GetError());
}

void destroyTexture(SDL_Texture* texture) {
	SDL_DestroyTexture(texture);
}

void drawText(Window* window, SDL_Color color, const char* text, int x, int y, float sizeRatio) {
	SDL_Surface* surface = TTF_RenderText_Solid(window->font, text, color);
	if (!surface) {
		printf("Failed to create surface! TTF error: %s\n", TTF_GetError());
		return;
	}

	Rect rect;
	rect.x = x;
	rect.y = y;
	rect.width = (float)surface->w * sizeRatio;
	rect.height = (float)surface->h * sizeRatio;
	rect.angle = 0.0f;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->renderer, surface);
	drawTexture(window, &rect, texture);
	
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}