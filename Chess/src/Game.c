#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include "UI.h"
#include <stdio.h>

void game() {
	// INIT
	Window* window = initWindow("Chess 2", 1024, 800);
	Board* board = createBoard(8);
	Player* players[2];
	players[0] = initPlayers(WHITE, window);
	players[1] = initPlayers(BLACK, window);
	putInBoard(players[0], board);
	putInBoard(players[1], board);

	Piece* selectedPiece = NULL;
	int squareSize = 0;
	int leftButtonHeld = 0;
	TypeColor whoPlays = WHITE;

	SDL_Texture** textures = createTextureArray(window);

	SidePanel panel;
	panel.offsetX = 800;
	panel.whoPlays = whoPlays;

	// MAIN LOOP
	while (!window->shouldClose) {
		squareSize = min(window->width, window->height) / 8;

		handleEvents(window);
		setDrawColor(window,64, 64, 64, 255);
		clear(window);

		int numPossibilities = 0;
		Cell* possibilities = getPossibilities(selectedPiece, whoPlays, board, &numPossibilities);

		drawBoard(window, board, textures, squareSize);
		drawPossibilities(window, board, possibilities, numPossibilities, squareSize);
		drawSidePanel(window, &panel);

		presentWindow(window);

		handleMouseClicking(window, board, &selectedPiece, players, possibilities, numPossibilities, squareSize, &whoPlays);
		panel.whoPlays = whoPlays;
	}

	free(textures);
	freePlayer(players[0]);
	freePlayer(players[1]);
	destroyBoard(board);
	destroyWindow(window);
}

SDL_Texture** createTextureArray(Window* window) {
	SDL_Texture** textures = (SDL_Texture**)malloc(sizeof(SDL_Texture*) * 12);
	textures[0] = createTexture(window, "White_Pawn.png");
	textures[1] = createTexture(window, "White_Bishop.png");
	textures[2] = createTexture(window, "White_Knight.png");
	textures[3] = createTexture(window, "White_Rook.png");
	textures[4] = createTexture(window, "White_Queen.png");
	textures[5] = createTexture(window, "White_King.png");
	textures[6] = createTexture(window, "Black_Pawn.png");
	textures[7] = createTexture(window, "Black_Bishop.png");
	textures[8] = createTexture(window, "Black_Knight.png");
	textures[9] = createTexture(window, "Black_Rook.png");
	textures[10] = createTexture(window, "Black_Queen.png");
	textures[11] = createTexture(window, "Black_King.png");
	return textures;
}

void getInputOnBoard(Window* window, int* boardX, int* boardY, int squareSize) {
	int x = window->mousePosX;
	int y = window->mousePosY;

	*boardX = x / squareSize;
	*boardY = y / squareSize;
}

Cell* getPossibilities(Piece* selectedPiece, TypeColor whoPlays, Board* board, int* numPossibilities) {
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

void drawBoard(Window* window, Board* board, SDL_Texture** textures, int squareSize) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Rect rect;
			rect.x = j * squareSize;
			rect.y = i * squareSize;
			rect.width = squareSize;
			rect.height = squareSize;
			rect.angle = 0.0f;

			if ((i + j) % 2 == 1)
				setDrawColor(window, 200, 200, 200, 255);
			else
				setDrawColor(window, 64, 64, 64, 255);

			if (board->selectedX == j && board->selectedY == i)
				setDrawColor(window, 128, 128, 128, 128);

			drawRect(window, &rect);

			if (board->table[j][i]) {
				TypeColor color = board->table[j][i]->color;
				TypePiece type = board->table[j][i]->type;
				int index = type + 6 * color;
				drawTexture(window, &rect, textures[index]);
			}
		}
	}
}

void drawPossibilities(Window* window, Board* board, Cell* possibilities, int numPossibilities, int squareSize) {
	if (!possibilities)
		return;

	for (int i = 0; i < numPossibilities; i++) {
		int x = possibilities[i].x * squareSize + squareSize / 2;
		int y = possibilities[i].y * squareSize + squareSize / 2;
		int radius = squareSize / 2 - squareSize / 10;
		setDrawColor(window, 128, 128, 128, 128);
		drawCircle(window, x, y, radius);
	}
}

void handleMouseClicking(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays) {
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