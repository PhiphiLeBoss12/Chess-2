#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include <stdio.h>

void game() {
	// INIT
	Window* window = initWindow("Chess 2", 800, 800);
	Board* board = createBoard(8);
	Player* p1 = initPlayers(WHITE, window);
	Player* p2 = initPlayers(BLACK, window);
	putInBoard(p1, board);
	putInBoard(p2, board);

	showCoordPiece(p1->table[0]);

	Piece* selectedPiece = NULL;
	int squareSize = 0;
	int leftButtonHeld = 0;
	TypeColor whoPlays = WHITE;

	// MAIN LOOP
	while (!window->shouldClose) {
		handleEvents(window);
		setDrawColor(window,64, 64, 64, 255);
		clear(window);

		squareSize = min(window->width, window->height) / 8;

		int numPossibilities = 0;
		Cell* possibilities = getPossibilities(selectedPiece, whoPlays, board, &numPossibilities);

		drawBoard(window, board, squareSize);
		drawPossibilities(window, board, possibilities, numPossibilities, squareSize);

		presentWindow(window);

		if (window->mouseLeftButton && !leftButtonHeld) {
			int x, y;
			getInputOnBoard(window, &x, &y, squareSize);
			board->selectedX = x;
			board->selectedY = y;
			
			for (int i = 0; i < numPossibilities; i++) {
				if (board->selectedX == possibilities[i].x && board->selectedY == possibilities[i].y && whoPlays == selectedPiece->color) {
					movePiece(selectedPiece, board->selectedX, board->selectedY, board, p1, p2);
					whoPlays = whoPlays == WHITE ? BLACK : WHITE; // Change the color
					// Unselect the square
					board->selectedX = -1;
					board->selectedY = -1;
				}
			}

			if (board->selectedX != -1 && board->selectedY != -1 && board->table[board->selectedX][board->selectedY])
				selectedPiece = board->table[board->selectedX][board->selectedY];

			leftButtonHeld = 1;
		}
		if (!window->mouseLeftButton)
			leftButtonHeld = 0;

		// Free the possibilities
		if (possibilities)
			free(possibilities);
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

Cell* getPossibilities(Piece* selectedPiece, TypeColor whoPlays, Board* board, int* numPossibilities) {
	if (board->selectedX != -1 && board->selectedY != -1 && whoPlays == selectedPiece->color) {
		if (board->table[board->selectedX][board->selectedY]) {
			return movePossibilitiesPiece(selectedPiece, board, numPossibilities);
		}
	}
	return NULL;
}

void drawBoard(Window* window, Board* board, int squareSize) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Rect rect;
			rect.x = j * squareSize;
			rect.y = i * squareSize;
			rect.width = squareSize;
			rect.height = squareSize;
			rect.angle = 0.0f;

			if ((i + j) % 2 == 0)
				setDrawColor(window, 200, 200, 200, 255);
			else
				setDrawColor(window, 64, 64, 64, 255);

			drawRect(window, &rect);

			if (board->selectedX == j && board->selectedY == i) {
				setDrawColor(window, 128, 128, 128, 128);
				int x = rect.x + rect.width / 2;
				int y = rect.y + rect.height / 2;
				int radius = rect.width / 2 - rect.width / 10;
				drawCircle(window, x, y, radius);
			}

			if (board->table[j][i])
				drawTexture(window, &rect, board->table[j][i]->texture);
		}
	}
}

void drawPossibilities(Window* window, Board* board, Cell* possibilities, int numPossibilities, int squareSize) {
	if (!possibilities)
		return NULL;

	for (int i = 0; i < numPossibilities; i++) {
		int x = possibilities[i].x * squareSize + squareSize / 2;
		int y = possibilities[i].y * squareSize + squareSize / 2;
		int radius = squareSize / 2 - squareSize / 10;
		setDrawColor(window, 128, 128, 128, 128);
		drawCircle(window, x, y, radius);
	}
}