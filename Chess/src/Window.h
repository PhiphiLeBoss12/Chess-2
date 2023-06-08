#pragma once
#include <SDL2/SDL.h>

typedef struct Window {
	int shouldClose;
	SDL_Window* window;
	SDL_Renderer* renderer;
} Window;

typedef struct Rect {
	int x, y;
	unsigned int width, height;
	float angle;
} Rect;

// Initializes SDL and a creates a window object.
// Returns NULL if something goes wrong
Window* initWindow(const char* title, unsigned int width, unsigned int height);

// Destroys the window and quits SDL
void destroyWindow(Window* window);

// Handles SDL events
void handleEvents(Window* window);

// Clears the window with the last selected draw color
void clear(Window* window);

// Presents everything drawn on the window to the screen
void presentWindow(Window* window);

// Sets the color for drawing several primitives
// The color is in RGBA format, 0-255
void setDrawColor(Window* window, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

// Draws a Rect with the last selected draw color
// The angle field is ignored
void drawRect(Window* window, Rect* rect);

// Creates a texture from the file path
// You need to destroy the texture once you don't need it anymore
SDL_Texture* createTexture(Window* window, const char* path);

// Draws a texture in the zone defined by rect
void drawTexture(Window* window, Rect* rect, SDL_Texture* texture);

// Destroys a texture
void destroyTexture(SDL_Texture* texture);