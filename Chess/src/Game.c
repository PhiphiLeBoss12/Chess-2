#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include "UI.h"
#include "Sound.h"
#include <stdio.h>

// Global variables (very bad)
Mix_Music* mainMenuMusic;
Mix_Music* gameMusic;
Mix_Chunk* stepSound;
Mix_Chunk* winSound;
Mix_Chunk* killSound;
Mix_Chunk* funnySound;
Mix_Chunk* funnySound2;
GameState gameState = START;

void game() {
	// INIT
	Window* window = initWindow("Chess 2", 800 + 400, 800);
	Board* board = createBoard(8);
	Player* players[2];
	players[0] = initPlayers(WHITE, window);
	players[1] = initPlayers(BLACK, window);
	putInBoard(players[0], board);
	putInBoard(players[1], board);
	LastMove* last = initLastMove();

	Piece* selectedPiece = NULL;
	int squareSize = 0;
	int leftButtonHeld = 0;
	TypeColor whoPlays = WHITE;

	SDL_Texture** textures = createTextureArray(window);

	mainMenuMusic = loadMusic("Deadly Roulette.mp3");
	gameMusic = loadMusic("Walking Along.mp3");
	stepSound = loadSound("step.mp3");
	winSound = loadSound("Danse Macabre.mp3");
	killSound = loadSound("kill.mp3");
	funnySound = loadSound("funny.mp3");
	funnySound2 = loadSound("funny2.mp3");

	SidePanel panel;
	panel.width = 400;
	panel.offsetX = 800;
	panel.whoPlays = whoPlays;

	playMusic(mainMenuMusic);

	// MAIN LOOP
	while (gameState != QUIT && !window->shouldClose) {
		squareSize = min(window->width, window->height) / 8;

		handleEvents(window);
		setDrawColor(window,64, 64, 64, 255);
		clear(window);

		int numPossibilities = 0;
		Cell* possibilities = getPossibilities(selectedPiece, whoPlays, board, &numPossibilities, last);

		drawBoard(window, board, textures, squareSize);
		drawPossibilities(window, board, possibilities, numPossibilities, squareSize);
		drawSidePanel(window, &panel);

		if (gameState == START) {
			drawStartScreen(window, textures);
			if (window->keyDown == SDLK_RETURN) {
				gameState = PLAYING;
				playMusic(gameMusic);
			}
			if (window->keyDown == SDLK_ESCAPE)
				gameState = QUIT;
		}

		if (gameState == END) {
			EndScreen es;
			es.width = 800;
			es.height = 600;
			es.whoWon = WHITE;
			drawEndScreen(window, &es);

			if (window->keyDown == SDLK_RETURN) {
				freePlayer(players[0]);
				freePlayer(players[1]);
				destroyBoard(board);

				players[0] = initPlayers(WHITE, window);
				players[1] = initPlayers(BLACK, window);
				board = createBoard(8);
				putInBoard(players[0], board);
				putInBoard(players[1], board);

				TypeColor whoPlays = WHITE;
				
				gameState = PLAYING;
			}

			if (window->keyDown == SDLK_ESCAPE)
				gameState = QUIT;
		}

		presentWindow(window);

		handleMouseClicking(window, board, &selectedPiece, players, possibilities, numPossibilities, squareSize, &whoPlays, last);
		panel.whoPlays = whoPlays;
	}

	destroyMusic(gameMusic);
	destroyMusic(mainMenuMusic);
	destroySound(stepSound);
	destroySound(winSound);
	destroySound(killSound);
	destroySound(funnySound);
	destroySound(funnySound2);

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

Cell* getPossibilities(Piece* selectedPiece, TypeColor whoPlays, Board* board, int* numPossibilities, LastMove* last) {
	if (!selectedPiece)
		return NULL;
	if (board->selectedX == -1 || board->selectedY == -1)
		return NULL;
	if (whoPlays != selectedPiece->color) 
		return NULL;
	if (!board->table[board->selectedX][board->selectedY])
		return NULL;

	return movePossibilitiesPiece(selectedPiece, board, numPossibilities, last);
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
				setDrawColor(window, 64, 128, 64, 200);

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

void handleMouseClicking(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays, LastMove* last) {
	static int leftButtonHeld = 0;

	if (window->mouseLeftButton && !leftButtonHeld) {
		int pieceEaten = 0;
		int x, y;
		getInputOnBoard(window, &x, &y, squareSize);

		if (x < 0 || y < 0 || x > 7 || y > 7)
			return;

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
				//Verify castling 
				if (board->table[board->selectedX][board->selectedY] != NULL) {
					if (board->table[board->selectedX][board->selectedY]->type == ROOK && board->table[board->selectedX][board->selectedY]->color == *whoPlays) {
						Piece* rook;
						rook = board->table[board->selectedX][board->selectedY];
						//Castling right
						if (board->selectedX > 4) {
							movePiece(*selectedPiece, board->selectedX - 1, board->selectedY, board, players[0], players[1], last);
							movePiece(rook, board->selectedX - 2, board->selectedY, board, players[0], players[1], last);
						}
						else { //Castling left
							movePiece(*selectedPiece, board->selectedX + 2, board->selectedY, board, players[0], players[1], last);
							movePiece(rook, board->selectedX + 3, board->selectedY, board, players[0], players[1], last);
						}
					}
					else {
						pieceEaten = movePiece(*selectedPiece, board->selectedX, board->selectedY, board, players[0], players[1], last);
					}
				}
				else {
					pieceEaten = movePiece(*selectedPiece, board->selectedX, board->selectedY, board, players[0], players[1], last);
				}

				*whoPlays = *whoPlays == WHITE ? BLACK : WHITE; // Change the color
				playSound(stepSound);
				if (pieceEaten)
					playSound(killSound);
				if (isCheck(board, *whoPlays) || isCheck(board, !(*whoPlays))) {
					playSound(funnySound);
					if (isCheckmate(board, *whoPlays, players[0], players[1])) {
						gameState = END;
						playSound(funnySound2);
					}
				}

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