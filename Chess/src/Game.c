#include "Game.h"
#include "Window.h"
#include <stdio.h>


void game(Window* window) {
	while (!window->shouldClose) {
		handleEvents(window);
		setDrawColor(window, 64, 64, 64, 255);
		clear(window);
		presentWindow(window);

		int x, y;
		if (window->mouseLeftButton) {
			getInputOnBoard(window, &x, &y, 100);
			printf("%d %d\n", x, y);
		}
	}
}

void cleanup(Window* window) {
	destroyWindow(window);
}

void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize) {
	int x = window->mousePosX;
	int y = window->mousePosY;

	// We consider that the board is filling the whole screen
	*boardX = x / (int)squareSize;
	*boardY = y / (int)squareSize;
}