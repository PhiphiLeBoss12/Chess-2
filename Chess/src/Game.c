#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include <stdio.h>

void game() {
	// INIT
	Window* window = initWindow("Chess 2", 800, 800);
	Board* board = createBoard(8);

	board->table[0][0] = initPiece(PAWN, WHITE, 0, 0);

	// MAIN LOOP
	while (!window->shouldClose) {
		handleEvents(window);
		setDrawColor(window, 64, 64, 64, 255);
		clear(window);

		int squareSize = min(window->width, window->height) / 8;
		drawBoard(window, board, squareSize);

		presentWindow(window);

		int x, y;
		if (window->mouseLeftButton) {
			getInputOnBoard(window, &x, &y, squareSize);
			board->selectedX = x;
			board->selectedY = y;
		}
	}

	// CLEANUP
	cleanup(window, board);
}

void cleanup(Window* window, Board* board) {
	destroyBoard(board);
	destroyWindow(window);
}

void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize) {
	int x = window->mousePosX;
	int y = window->mousePosY;

	*boardX = x / squareSize;
	*boardY = y / squareSize;
}

void drawBoard(Window* window, Board* board, int squareSize) {
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			if (board->selectedX == j && board->selectedY == i)
				setDrawColor(window, 255, 0, 0, 255);
			else if ((i + j) % 2 == 0)
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