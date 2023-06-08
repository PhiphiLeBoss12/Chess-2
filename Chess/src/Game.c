#include "Game.h"
#include "Window.h"
#include <stdio.h>


void game(Window* window) {
	while (!window->shouldClose) {
		handleEvents(window);
		setDrawColor(window, 64, 64, 64, 255);
		clear(window);

		drawBoard(window, window->width > window->height ? window->height / 8 : window->width / 8);

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

void drawBoard(Window* window, int squareSize) {
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0)
				setDrawColor(window, 200, 200, 200, 255);
			else
				setDrawColor(window, 64, 64, 64, 255);

			Rect rect;
			rect.x = j * squareSize;
			rect.y = i * squareSize;
			rect.width = squareSize;
			rect.height = squareSize;

			drawRect(window, &rect);
		}
	}
}