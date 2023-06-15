#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include "UI.h"
#include "Sound.h"
#include "Network.h"
#include <stdio.h>

// Global variables (very bad)
Mix_Music* mainMenuMusic;
Mix_Music* gameMusic;
Mix_Chunk* stepSound;
Mix_Chunk* killSound;
Mix_Chunk* stalemateSound;
Mix_Chunk* funnySound;
Mix_Chunk* funnySound2;
GameState gameState = START;

IPaddress ipServer, ipClient;
TCPsocket tcpServer, tcpClient;
int multiplayerServer = 0;
int multiplayerClient = 0;

void game() {
	// INIT
	Window* window = initWindow("Chess 2", 800 + 400, 800, 1);
	Board* board = createBoard(8);
	Player* players[2];
	players[0] = initPlayers(WHITE, window);
	players[1] = initPlayers(BLACK, window);
	putInBoard(players[0], board);
	putInBoard(players[1], board);
	LastMove* last = initLastMove();
	int promo = 0;

	Piece* selectedPiece = NULL;
	int squareSize = 0;
	int leftButtonHeld = 0;
	TypeColor whoPlays = WHITE;

	SDL_Texture** textures = createTextureArray(window);

	mainMenuMusic = loadMusic("Deadly Roulette.mp3");
	gameMusic = loadMusic("Walking Along.mp3");
	stepSound = loadSound("step.mp3");
	killSound = loadSound("kill.mp3");
	stalemateSound = loadSound("impasta.mp3");
	funnySound = loadSound("funny.mp3");
	funnySound2 = loadSound("funny2.mp3");
	int enableMusic = 1;

	SidePanel panel;
	panel.width = 400;
	panel.offsetX = 800;
	panel.whoPlays = whoPlays;
	panel.playerWhite = players[0];
	panel.playerBlack = players[1];

	if (enableMusic)
		playMusic(mainMenuMusic);

	// MAIN LOOP
	while (gameState != QUIT && !window->shouldClose) {
		squareSize = min(window->width, window->height) / 8;

		handleEvents(window);
		setDrawColor(window,64, 64, 64, 255);
		clear(window);

		if (gameState == PLAYING) {
			int numPossibilities = 0;
			Cell* possibilities = getPossibilities(selectedPiece, whoPlays, board, &numPossibilities, last);
			testPossibilitiesCheck(board, whoPlays, players[0], players[1], last, selectedPiece, possibilities, numPossibilities, &promo);

			drawBoard(window, board, textures, squareSize, *last);
			drawPossibilities(window, board, possibilities, numPossibilities, squareSize, selectedPiece);
			drawSidePanel(window, &panel, textures);

			if (multiplayerServer && whoPlays == WHITE) {
				handleMouseClicking(window, board, &selectedPiece, players, possibilities, numPossibilities, squareSize, &whoPlays, last, &promo);
				panel.whoPlays = whoPlays;
				free(possibilities);
				if (window->keyDown == SDLK_p) {
					printf("Packet sent\n");
					Packet packet = { 0, 1.0f, 1 };
					SDLNet_TCP_Send(tcpClient, &packet, sizeof(Packet));
					whoPlays = BLACK;
				}
			}
			else if (multiplayerClient && whoPlays == BLACK) {
				handleMouseClicking(window, board, &selectedPiece, players, possibilities, numPossibilities, squareSize, &whoPlays, last, &promo);
				panel.whoPlays = whoPlays;
				free(possibilities);
				if (window->keyDown == SDLK_p) {
					printf("Packet sent\n");
					Packet packet = { 0, 1.0f, 1 };
					SDLNet_TCP_Send(tcpServer, &packet, sizeof(Packet));
					whoPlays = WHITE;
				}
			}
			else {
				if (whoPlays == WHITE) {
					printf("Waiting for packet...\n");
					Packet packet;
					packet.Sent = 0;
					while (!packet.Sent)
						recievePacket(&tcpServer, &packet, sizeof(Packet));
					whoPlays = BLACK;
					printf("Received packet!\n");
				}
				else if (whoPlays == BLACK) {
					printf("Waiting for packet...\n");
					Packet packet;
					packet.Sent = 0;
					while (!packet.Sent) {
						recievePacket(&tcpClient, &packet, sizeof(Packet));
					}
					whoPlays = WHITE;
					printf("Received packet!\n");
				}
			}

			if (window->keyDown == SDLK_F5)
				resetBoard(window, board, players, &whoPlays, &panel);
		}

		if (gameState == START) {
			drawStartScreen(window, textures);
			if (window->keyDown == SDLK_RETURN) {
				gameState = PLAYING;
				if (enableMusic)
					playMusic(gameMusic);
			}
			if (window->keyDown == SDLK_ESCAPE)
				gameState = QUIT;
			if (window->keyDown == SDLK_s) {
				// Open server
				if (!multiplayerServer)
					initNetworkServer(&ipServer, &tcpServer, &tcpClient);
				multiplayerServer = 1;
			}
			if (window->keyDown == SDLK_z) {
				// Open client
				if (!multiplayerClient)
					initNetworkClient(&ipClient, &tcpServer, "localhost");
				multiplayerClient = 1;
			}
		}

		if (gameState == END || gameState == STALEMATE) {
			EndScreen es;
			es.width = 800;
			es.height = 300;
			if (gameState == END)
				es.whoWon = whoPlays;
			else
				es.whoWon = -1;
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

				whoPlays = WHITE;
				
				gameState = PLAYING;

				panel.playerWhite = players[0];
				panel.playerBlack = players[1];
			}

			if (window->keyDown == SDLK_ESCAPE)
				gameState = QUIT;
		}

		presentWindow(window);
		
		// Toggle Music
		toggleMusic(window, &enableMusic);
	}

	destroyMusic(gameMusic);
	destroyMusic(mainMenuMusic);
	destroySound(stepSound);
	destroySound(killSound);
	destroySound(funnySound);
	destroySound(funnySound2);
	destroySound(stalemateSound);

	free(textures);
	freePlayer(players[0]);
	freePlayer(players[1]);
	destroyBoard(board);
	destroyWindow(window);
}

void resetBoard(Window* window, Board** board, Player*** players, TypeColor* whoPlays, SidePanel* panel) {
	freePlayer(*players[0]);
	freePlayer(*players[1]);
	destroyBoard(*board);

	*players[0] = initPlayers(WHITE, window);
	*players[1] = initPlayers(BLACK, window);
	*board = createBoard(8);
	putInBoard(*players[0], board);
	putInBoard(*players[1], board);

	whoPlays = WHITE;

	gameState = PLAYING;

	panel->playerWhite = players[0];
	panel->playerBlack = players[1];
}

void toggleMusic(Window* window, int* enableMusic) {
	static int keyHeld = 0;
	if (window->keyDown == SDLK_m && !keyHeld) {
		*enableMusic = !*enableMusic;
		if (!*enableMusic)
			stopMusic();
		else
			resumeMusic();
		keyHeld = 1;
	}
	if (window->keyDown == SDLK_UNKNOWN)
		keyHeld = 0;
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

void drawBoard(Window* window, Board* board, SDL_Texture** textures, int squareSize, LastMove last) {
		
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

	if (last.piece != NULL) {
		setDrawColor(window, 245, 176, 65, 150);
		Rect rect;
		rect.x = last.piece->x * squareSize;
		rect.y = last.piece->y * squareSize;
		rect.width = squareSize;
		rect.height = squareSize;
		rect.angle = 0.0f;
		drawRect(window, &rect);
		rect.x = last.prevX * squareSize;
		rect.y = last.prevY * squareSize;
		drawRect(window, &rect);
		if (board->table[last.piece->x][last.piece->y] != NULL) {
			TypeColor color = last.piece->color;
			TypePiece type = last.piece->type;
			int index = type + 6 * color;
			rect.x = last.piece->x * squareSize;
			rect.y = last.piece->y * squareSize;
			drawTexture(window, &rect, textures[index]);
		}
	}
}

void drawPossibilities(Window* window, Board* board, Cell* possibilities, int numPossibilities, int squareSize, Piece* selectedPiece) {
	if (!possibilities)
		return;

	for (int i = 0; i < numPossibilities; i++) {
		if (possibilities[i].x == -1 && possibilities[i].y == -1)
			continue;

		int x = possibilities[i].x * squareSize + squareSize / 2;
		int y = possibilities[i].y * squareSize + squareSize / 2;
		int radius = squareSize / 2 - squareSize / 8;
		setDrawColor(window, 128, 128, 128, 128);
		if (board->table[possibilities[i].x][possibilities[i].y] && selectedPiece->color != board->table[possibilities[i].x][possibilities[i].y]->color)
			setDrawColor(window, 255, 128, 128, 128);
		if (selectedPiece->type == PAWN && (possibilities[i].x == selectedPiece->x - 1 || possibilities[i].x == selectedPiece->x + 1) && board->table[possibilities[i].x][possibilities[i].y] == NULL) {
			if (selectedPiece->color == WHITE) {
				y = (possibilities[i].y - 1) * squareSize + squareSize / 2;
			}
			else {
				y = (possibilities[i].y + 1) * squareSize + squareSize / 2;
			}
			setDrawColor(window, 255, 128, 128, 128);
			drawCircle(window, x, y, radius);
		}
		drawCircle(window, x, y, radius);
	}
}

void handleMouseClicking(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays, LastMove* last, int* promo) {
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
							movePiece(*selectedPiece, board->selectedX - 1, board->selectedY, board, players[0], players[1], last, promo);
							movePiece(rook, board->selectedX - 2, board->selectedY, board, players[0], players[1], last, promo);
						}
						else { //Castling left
							movePiece(*selectedPiece, board->selectedX + 2, board->selectedY, board, players[0], players[1], last, promo);
							movePiece(rook, board->selectedX + 3, board->selectedY, board, players[0], players[1], last, promo);
						}
					}
					else {
						pieceEaten = movePiece(*selectedPiece, board->selectedX, board->selectedY, board, players[0], players[1], last, promo);
						if (*promo == 1) {
							Window* wintest = winPromo("promotion", *selectedPiece, &(*selectedPiece)->type);
							*promo = 0;
							destroyWindow(wintest);
						}
					}
				}
				else {
					pieceEaten = movePiece(*selectedPiece, board->selectedX, board->selectedY, board, players[0], players[1], last, promo);
					if (*promo == 1) {
						Window* wintest = winPromo("promotion", *selectedPiece, &(*selectedPiece)->type);
						*promo = 0;
						destroyWindow(wintest);
					}
				}

				*whoPlays = *whoPlays == WHITE ? BLACK : WHITE; // Change the color
				playSound(stepSound);
				if (pieceEaten)
					playSound(killSound);
				if (isCheck(board, *whoPlays) || isCheck(board, !(*whoPlays))) {
					playSound(funnySound);
					if (isCheckmate(board, *whoPlays, players[0], players[1], last, promo)) {
						gameState = END;
						playSound(funnySound2);
						*whoPlays = *whoPlays == WHITE ? BLACK : WHITE; // Change the color again
					}
				}
				if (isStalemate(board, players, last, promo)) {
					gameState = STALEMATE;
					playSound(stalemateSound);
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

Window* winPromo(const char* title, Piece* pawn, TypePiece* newType) {
	Window* wintest = initWindow(title, 100*4, 100, 0);
	SDL_Texture** texturePromo = createTextureArray(wintest);
	int choose = 0;
	while (choose == 0) {
		handleEvents(wintest);
		clear(wintest);
		for (int i = 0; i < 4; i++) {
			Rect rect;
			rect.x = i * 100;
			rect.y = 0 * 100;
			rect.width = 100;
			rect.height = 100;
			rect.angle = 0.0f;
			TypeColor color = pawn->color;
			TypePiece type = i + 1;
			int index = type + 6 * color;
			if (i % 2 != 0) {
				setDrawColor(wintest, 64, 64, 64, 255);
			}
			else {
				setDrawColor(wintest, 200, 200, 200, 255);
			}
			drawRect(wintest, &rect);
			drawTexture(wintest, &rect, texturePromo[index]);
		}
		presentWindow(wintest);

		static int leftButtonHeld = 0;

		if (wintest->mouseLeftButton && !leftButtonHeld) {
			int x, y;
			getInputOnBoard(wintest, &x, &y, 100);
			*newType = x + 1; 
			choose = 1;
		}
	}
	return wintest;
}