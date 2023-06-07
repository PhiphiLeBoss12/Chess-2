#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct WindowData {
	const char* title;
	uint32_t width, height;
} WindowData;

typedef struct Window {
	SDL_Window* window;
	SDL_Renderer* renderer;
} Window;

// Initializes SDL and a creates a window object.
// Returns NULL if something goes wrong
Window* initWindow(WindowData* data);

// Destroys the window and quits SDL
void destroyWindow(Window* window);