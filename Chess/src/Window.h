#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Simple structure to hold window information
// Do not modify the fields please :(
typedef struct Window {
	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font* font;

	int width, height; // Size of the window

	int shouldClose; // 1 if the user closes the window (eg. clicking the x or Alt-F4), 0 otherwise

	int mousePosX, mousePosY; // Position of the mouse cursor
	int mouseRightButton, mouseLeftButton, mouseMiddleButton; // 1 if the button is clicked, 0 otherwise

	unsigned int keyDown; // The most recent key pressed SDL name. The name of the key always starts with SDLK_
} Window;

// Simple rectangle-shaped structure
// The angle field isn't used when drawing the rect without a texture
typedef struct Rect {
	int x, y;
	unsigned int width, height;
	float angle;
} Rect;

void initSDL();
void quitSDL();

// Initializes SDL and a creates a window object.
// Returns NULL if something goes wrong
Window* initWindow(const char* title, unsigned int width, unsigned int height, int decorations);

// Destroys the window and quits SDL
void destroyWindow(Window* window);

// Handles SDL events
void handleEvents(Window* window);

// Clears the window with the last selected draw color
void clear(Window* window);

// Presents everything drawn on the window to the screen
void presentWindow(Window* window);

// Sets the color for drawing several primitives
// The color is in RGBA format, 0-255 where 0 is no color and 255 is full color
void setDrawColor(Window* window, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

// Draws a Rect with the last selected draw color
// The angle field is ignored
void drawRect(Window* window, Rect* rect);

// Draws a circle centered at (x,y) of radius radius
void drawCircle(Window* window, int x, int y, int radius);

// Draws a line from (x1,y1) to (x2,y2)
void drawLine(Window* window, int thiccness, int x1, int y1, int x2, int y2);

// Creates a texture from the file path
// Only JPG and PNG formats are supported!
// You need to destroy the texture once you don't need it anymore
SDL_Texture* createTexture(Window* window, const char* path);

// Draws a texture in the zone defined by rect
void drawTexture(Window* window, Rect* rect, SDL_Texture* texture);

// Destroys a texture
void destroyTexture(SDL_Texture* texture);

// Draws text at (x,y)
void drawText(Window* window, SDL_Color color, const char* text, int x, int y, float sizeRatio);