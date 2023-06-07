#pragma once
#include <SDL2/SDL.h>

typedef struct WindowData {
	const char* title;
	int width, height;
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