#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include <stdio.h>

void game() {
	// INIT
	Window* window = initWindow("Chess 2", 800, 800);
	Board* board = createBoard(8);
	Player* players[2];
	players[0] = initPlayers(WHITE, window);
	players[1] = initPlayers(BLACK, window);
	putInBoard(players[0], board);
	putInBoard(players[1], board);

	showCoordPiece(p1->table[0]);

	Piece* selectedPiece = NULL;
	int squareSize = 0;
	int leftButtonHeld = 0;
	TypeColor whoPlays = WHITE;

	// MAIN LOOP
	while (!window->shouldClose) {
		squareSize = min(window->width, window->height) / 8;

		handleEvents(window);
		setDrawColor(window,64, 64, 64, 255);
		clear(window);

		int numPossibilities = 0;
		Cell* possibilities = getPossibilities(selectedPiece, whoPlays, board, &numPossibilities);

		drawBoard(window, board, squareSize);
		drawPossibilities(window, board, possibilities, numPossibilities, squareSize);

		presentWindow(window);

		handleMouseClicking(window, board, &selectedPiece, players, possibilities, numPossibilities, squareSize, &whoPlays);
	}

	freePlayer(players[0]);
	freePlayer(players[1]);
	destroyBoard(board);
	destroyWindow(window);
}

void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize) {
	int x = window->mousePosX;
	int y = window->mousePosY;

	*boardX = x / squareSize;
	*boardY = y / squareSize;
}

Case* getPossibilities(Piece* selectedPiece, TypeColor whoPlays, Board* board, int* numPossibilities) {
	if (!selectedPiece)
		return NULL;
	if (board->selectedX == -1 || board->selectedY == -1)
		return NULL;
	if (whoPlays != selectedPiece->color) 
		return NULL;
	if (!board->table[board->selectedX][board->selectedY])
		return NULL;

	return movePossibilitiesPiece(selectedPiece, board, numPossibilities);
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

void handleMouseClicking(Window* window, Board* board, Piece** selectedPiece, Player** players, Case* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays) {
	static int leftButtonHeld = 0;

	if (window->mouseLeftButton && !leftButtonHeld) {
		int x, y;
		getInputOnBoard(window, &x, &y, squareSize);

		if (board->table[x][y]) {
			board->selectedX = x;
			board->selectedY = y;
		}

		for (int i = 0; i < numPossibilities; i++) {
			if (x == possibilities[i].x && y == possibilities[i].y) {
				board->selectedX = x;
				board->selectedY = y;
			}

			if (board->selectedX == possibilities[i].x && board->selectedY == possibilities[i].y && *whoPlays == (*selectedPiece)->color) {
				movePiece(*selectedPiece, board->selectedX, board->selectedY, board, players[0], players[1]);
				*whoPlays = *whoPlays == WHITE ? BLACK : WHITE; // Change the color
				// Unselect the square
				board->selectedX = -1;
				board->selectedY = -1;
				Player* tempo = players[0];
				players[0] = players[1];
				players[1] = tempo;
			}
		}

		if (board->selectedX != -1 && board->selectedY != -1 && board->table[board->selectedX][board->selectedY])
			*selectedPiece = board->table[board->selectedX][board->selectedY];

		leftButtonHeld = 1;
	}
	if (!window->mouseLeftButton)
		leftButtonHeld = 0;
}