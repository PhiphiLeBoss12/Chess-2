#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include <stdio.h>

void game() {
	// INIT
	Window* window = initWindow("Chess 2", 800, 800);
	Board* board = createBoard(8);
	// Player* p1 = initPlayers(WHITE, window);
	// Player* p2 = initPlayers(BLACK, window);
	// putInBoard(p1, board);
	// putInBoard(p2, board);
	/*Player *player1 = initPlayers(WHITE, window);
	Player* player2 = initPlayers(BLACK, window);
	putInBoard(player1, board);
	putInBoard(player2, board);*/


	/*Piece* piece1 = initPiece(BISHOP, WHITE, 1, 1, window);
	board->table[1][1] = piece1;

	Piece* piece2 = initPiece(PAWN, BLACK, 2, 2, window);
	piece2->hasMovedOnce = 1;
	board->table[2][2] = piece2;*/

	// MAIN LOOP
	while (!window->shouldClose) {
		handleEvents(window);
		setDrawColor(window,64, 64, 64, 255);
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

	drawPossibilities(window, board, squareSize);
}

void drawPossibilities(Window* window, Board* board, int squareSize) {
	// No case is selected
	if (board->selectedX < 0 || board->selectedY < 0)
		return;
	// No piece is in the selected case
	if (!board->table[board->selectedX][board->selectedY])
		return;

	Piece* piece = board->table[board->selectedX][board->selectedY];
	int numPossibilities;
	Case* possibilities = movePossibilitiesPiece(piece, board, &numPossibilities);
	for (int i = 0; i < numPossibilities; i++) {
		int x = possibilities[i].x * squareSize + squareSize / 2;
		int y = possibilities[i].y * squareSize + squareSize / 2;
		int radius = squareSize / 2 - squareSize / 10;
		setDrawColor(window, 128, 128, 128, 128);
		drawCircle(window, x, y, radius);
	}
}