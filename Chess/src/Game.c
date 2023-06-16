#include "Game.h"
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include "UI.h"
#include "Sound.h"
#include "Network.h"
#include "Board.h"
#include <stdio.h>

void gameLoop() {
	// INIT
	Window* window = initWindow("Chess 2", 800 + 400, 800, 1);
	Game* game = initGame(window);

	if (game->enableMusic)
		playMusic(game->musics[MUSIC_MENU]);

	// MAIN LOOP
	while (game->gameState != QUIT && !window->shouldClose) {
		handleEvents(window);
		setDrawColor(window,64, 64, 64, 255);
		clear(window);
		
		if (game->gameState == START)
			start(window, game);

		if (game->gameState == PLAYING)
			playing(window, game);

		if (game->gameState == END || game->gameState == STALEMATE)
			end(window, game);

		presentWindow(window);

		// Toggle Music
		toggleMusic(window, game);
	}

	destroyGame(game);
	destroyWindow(window);
}

void start(Window* window, Game* game) {
	drawStartScreen(window, game->textures);
	if (window->keyDown == SDLK_RETURN) {
		game->gameState = PLAYING;
		if (game->enableMusic)
			playMusic(game->musics[MUSIC_GAME]);
	}
	if (window->keyDown == SDLK_ESCAPE)
		game->gameState = QUIT;
	if (window->keyDown == SDLK_s) {
		// Open server
		if (!game->multiplayerServer)
			initNetworkServer(&game->ipServer, &game->tcpServer, &game->tcpClient);
		game->multiplayerServer = 1;
	}
	if (window->keyDown == SDLK_z) {
		// Open client
		if (!game->multiplayerClient)
			initNetworkClient(&game->ipClient, &game->tcpServer);
		game->multiplayerClient = 1;
	}
}

void playing(Window* window, Game* game) {
	SidePanel panel;
	panel.width = 400;
	panel.offsetX = 800;
	panel.whoPlays = game->whoPlays;
	panel.playerWhite = game->players[0];
	panel.playerBlack = game->players[1];

	int numPossibilities = 0;
	Cell* possibilities = getPossibilities(game, &numPossibilities);
	testPossibilitiesCheck(game->board, game->whoPlays, game->players[0], game->players[1], game->last, game->selectedPiece, possibilities, numPossibilities, &game->promo);

	if (game->multiplayerServer || game->multiplayerClient) {
		clear(window);

		drawBoard(window, game);
		drawPossibilities(window, game, possibilities, numPossibilities);

		panel.whoPlays = game->whoPlays;
		drawSidePanel(window, &panel, game->textures);

		presentWindow(window);

		doNetwork(window, game);
	}

	drawBoard(window, game);
	drawPossibilities(window, game, possibilities, numPossibilities);

	panel.whoPlays = game->whoPlays;
	drawSidePanel(window, &panel, game->textures);

	handleMouseClicking(window, game, possibilities, numPossibilities, &game->promo);

	if (window->keyDown == SDLK_F5)
		resetBoard(window, game);
}

void end(Window* window, Game* game) {
	EndScreen es;
	es.width = 800;
	es.height = 300;
	if (game->gameState == END)
		es.whoWon = game->whoPlays;
	else
		es.whoWon = -1;
	drawEndScreen(window, &es);

	if (window->keyDown == SDLK_RETURN)
		resetBoard(window, game);

	if (window->keyDown == SDLK_ESCAPE)
		game->gameState = QUIT;
}

Game* initGame(Window* window) {
	Game* game = malloc(sizeof(Game));

	game->gameState = START;

	game->board = createBoard(8);
	game->players[0] = initPlayers(WHITE);
	game->players[1] = initPlayers(BLACK);
	game->last = initLastMove();
	game->selectedPiece = NULL;
	game->whoPlays = WHITE;

	putInBoard(game->players[0], game->board);
	putInBoard(game->players[1], game->board);

	game->textures = createTextureArray(window);

	game->musics[MUSIC_MENU] = loadMusic("Deadly Roulette.mp3");
	game->musics[MUSIC_GAME] = loadMusic("Walking Along.mp3");
	game->sounds[SOUND_STEP] = loadSound("step.mp3");
	game->sounds[SOUND_KILL] = loadSound("kill.mp3");
	game->sounds[SOUND_STALEMATE] = loadSound("impasta.mp3");
	game->sounds[SOUND_CHECK] = loadSound("funny.mp3");
	game->sounds[SOUND_FUNNY] = loadSound("funny2.mp3");
	game->enableMusic = 0;

	game->multiplayerServer = 0;
	game->multiplayerClient = 0;

	return game;
}

void destroyGame(Game* game) {
	destroyBoard(game->board);
	freePlayer(game->players[0]);
	freePlayer(game->players[1]);
	free(game->last);

	for (int i = 0; i < 12; i++)
		destroyTexture(game->textures[i]);

	for (int i = 0; i < 2; i++)
		destroyMusic(game->musics[i]);
	for (int i = 0; i < 5; i++)
		destroySound(game->sounds[i]);

	if (game->multiplayerServer || game->multiplayerClient) {
		SDLNet_TCP_Close(game->tcpServer);
	}
	if (game->multiplayerServer)
		SDLNet_TCP_Close(game->tcpClient);
}

void doNetwork(Window* window, Game* game) {
	if (game->multiplayerClient && game->whoPlays == WHITE) {
		printf("Waiting for packet...\n");
		MovePacket packet;
		packet.sent = 0;
		while (!packet.sent) {
			handleEvents(window);
			if (recievePacket(&game->tcpServer, &packet, sizeof(MovePacket)) <= 0)
				break;
		}
		fillGamePacketRecieve(game, &packet);
		game->whoPlays = BLACK;
		printf("Received packet!\n");
	}
	else if (game->multiplayerServer && game->whoPlays == BLACK) {
		printf("Waiting for packet...\n");
		MovePacket packet;
		packet.sent = 0;
		while (!packet.sent) {
			handleEvents(window);
			if (recievePacket(&game->tcpClient, &packet, sizeof(MovePacket)) <= 0)
				break;
		}
		fillGamePacketRecieve(game, &packet);
		game->whoPlays = WHITE;
		printf("Received packet!\n");
	}
}

void fillGamePacketRecieve(Game* game, MovePacket* packet) {
	movePiece(game->board->table[packet->oldX][packet->oldY], packet->newX, packet->newY, game->board, game->players[0], game->players[1], game->last, &game->promo);
}

void resetBoard(Window* window, Game* game) {
	destroyGame(game);
	game = initGame(window);
}

void toggleMusic(Window* window, Game* game) {
	static int keyHeld = 0;
	if (window->keyDown == SDLK_m && !keyHeld) {
		game->enableMusic = !game->enableMusic;
		if (!game->enableMusic)
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

void getInputOnBoard(Window* window, int* boardX, int* boardY) {
	int x = window->mousePosX;
	int y = window->mousePosY;

	*boardX = x / SQUARE_SIZE;
	*boardY = y / SQUARE_SIZE;
}

Cell* getPossibilities(Game* game, int* numPossibilities) {
	if (!game->selectedPiece)
		return NULL;
	if (game->board->selectedX == -1 || game->board->selectedY == -1)
		return NULL;
	if (game->whoPlays != game->selectedPiece->color) 
		return NULL;
	if (!game->board->table[game->board->selectedX][game->board->selectedY])
		return NULL;

	return movePossibilitiesPiece(game->selectedPiece, game->board, numPossibilities, game->last);
}

void drawBoard(Window* window, Game* game) {
		
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Rect rect;
			rect.x = j * SQUARE_SIZE;
			rect.y = i * SQUARE_SIZE;
			rect.width = SQUARE_SIZE;
			rect.height = SQUARE_SIZE;
			rect.angle = 0.0f;

			if ((i + j) % 2 == 1)
				setDrawColor(window, 200, 200, 200, 255);
			else
				setDrawColor(window, 64, 64, 64, 255);

			if (game->board->selectedX == j && game->board->selectedY == i)
				setDrawColor(window, 64, 128, 64, 200);

			drawRect(window, &rect);

			if (game->board->table[j][i]) {
				TypeColor color = game->board->table[j][i]->color;
				TypePiece type = game->board->table[j][i]->type;
				int index = type + 6 * color;
				drawTexture(window, &rect, game->textures[index]);
			}
		}
	}

	if (game->last->piece != NULL) {
		setDrawColor(window, 245, 176, 65, 150);
		Rect rect;
		rect.x = game->last->piece->x * SQUARE_SIZE;
		rect.y = game->last->piece->y * SQUARE_SIZE;
		rect.width = SQUARE_SIZE;
		rect.height = SQUARE_SIZE;
		rect.angle = 0.0f;
		drawRect(window, &rect);
		rect.x = game->last->prevX * SQUARE_SIZE;
		rect.y = game->last->prevY * SQUARE_SIZE;
		drawRect(window, &rect);
		if (game->board->table[game->last->piece->x][game->last->piece->y] != NULL) {
			TypeColor color = game->last->piece->color;
			TypePiece type = game->last->piece->type;
			int index = type + 6 * color;
			rect.x = game->last->piece->x * SQUARE_SIZE;
			rect.y = game->last->piece->y * SQUARE_SIZE;
			drawTexture(window, &rect, game->textures[index]);
		}
	}
}

void drawPossibilities(Window* window, Game* game, Cell* possibilities, int numPossibilities) {
	if (!possibilities)
		return;

	for (int i = 0; i < numPossibilities; i++) {
		if (possibilities[i].x == -1 && possibilities[i].y == -1)
			continue;

		int x = possibilities[i].x * SQUARE_SIZE + SQUARE_SIZE / 2;
		int y = possibilities[i].y * SQUARE_SIZE + SQUARE_SIZE / 2;
		int radius = SQUARE_SIZE / 2 - SQUARE_SIZE / 8;
		setDrawColor(window, 128, 128, 128, 128);
		if (game->board->table[possibilities[i].x][possibilities[i].y] && game->selectedPiece->color != game->board->table[possibilities[i].x][possibilities[i].y]->color)
			setDrawColor(window, 255, 128, 128, 128);
		if (game->selectedPiece->type == PAWN && (possibilities[i].x == game->selectedPiece->x - 1 || possibilities[i].x == game->selectedPiece->x + 1) && game->board->table[possibilities[i].x][possibilities[i].y] == NULL) {
			if (game->selectedPiece->color == WHITE) {
				y = (possibilities[i].y - 1) * SQUARE_SIZE + SQUARE_SIZE / 2;
			}
			else {
				y = (possibilities[i].y + 1) * SQUARE_SIZE + SQUARE_SIZE / 2;
			}
			setDrawColor(window, 255, 128, 128, 128);
			drawCircle(window, x, y, radius);
		}
		drawCircle(window, x, y, radius);
	}
}

void handleMouseClicking(Window* window, Game* game, Cell* possibilities, int numPossibilities, int* promo) {
	static int leftButtonHeld = 0;

	Board* board = game->board;
	Player* players[2] = { game->players[0], game->players[1] }; // May cause some problems
	LastMove* last = game->last;
	TypeColor* whoPlays = &game->whoPlays;

	if (window->mouseLeftButton && !leftButtonHeld) {
		int pieceEaten = 0;
		int x, y;
		getInputOnBoard(window, &x, &y);

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

			if (board->selectedX == possibilities[i].x && board->selectedY == possibilities[i].y && *whoPlays == game->selectedPiece->color) {
				int pieceOldX = game->selectedPiece->x;
				int pieceOldY = game->selectedPiece->y;

				//Verify castling 
				if (board->table[board->selectedX][board->selectedY] != NULL) {
					if (board->table[board->selectedX][board->selectedY]->type == ROOK && board->table[board->selectedX][board->selectedY]->color == *whoPlays) {
						Piece* rook;
						rook = board->table[board->selectedX][board->selectedY];
						//Castling right
						if (board->selectedX > 4) {
							movePiece(game->selectedPiece, board->selectedX - 1, board->selectedY, board, players[0], players[1], last, promo);
							movePiece(rook, board->selectedX - 2, board->selectedY, board, players[0], players[1], last, promo);
						}
						else { //Castling left
							movePiece(game->selectedPiece, board->selectedX + 2, board->selectedY, board, players[0], players[1], last, promo);
							movePiece(rook, board->selectedX + 3, board->selectedY, board, players[0], players[1], last, promo);
						}
					}
					else {
						pieceEaten = movePiece(game->selectedPiece, board->selectedX, board->selectedY, board, players[0], players[1], last, promo);
						if (*promo == 1) {
							Window* wintest = winPromo("promotion", game->selectedPiece, &game->selectedPiece->type);
							*promo = 0;
							destroyWindow(wintest);
						}
					}
				}
				else {
					pieceEaten = movePiece(game->selectedPiece, board->selectedX, board->selectedY, board, players[0], players[1], last, promo);
					if (*promo == 1) {
						Window* wintest = winPromo("promotion", game->selectedPiece, &game->selectedPiece->type);
						*promo = 0;
						destroyWindow(wintest);
					}
				}

				*whoPlays = *whoPlays == WHITE ? BLACK : WHITE; // Change the color
				playSound(game->sounds[SOUND_STEP]);
				if (pieceEaten)
					playSound(game->sounds[SOUND_KILL]);
				if (isCheck(board, *whoPlays) || isCheck(board, !(*whoPlays))) {
					playSound(game->sounds[SOUND_CHECK]);
					if (isCheckmate(board, *whoPlays, players[0], players[1], last, promo)) {
						game->gameState = END;
						playSound(game->sounds[SOUND_FUNNY]);
						*whoPlays = *whoPlays == WHITE ? BLACK : WHITE; // Change the color again
					}
				}
				if (isStalemate(board, players, last, promo)) {
					game->gameState = STALEMATE;
					playSound(game->sounds[SOUND_STALEMATE]);
				}

				// Network stuff
				if (game->multiplayerServer) {
					MovePacket packet;
					packet.oldX = pieceOldX;
					packet.oldY = pieceOldY;
					packet.newX = board->selectedX;
					packet.newY = board->selectedY;
					SDLNet_TCP_Send(game->tcpClient, &packet, sizeof(MovePacket));
					printf("Packet sent\n");
				}
				else if (game->multiplayerClient) {
					MovePacket packet;
					packet.oldX = pieceOldX;
					packet.oldY = pieceOldY;
					packet.newX = board->selectedX;
					packet.newY = board->selectedY;
					SDLNet_TCP_Send(game->tcpServer, &packet, sizeof(MovePacket));
					printf("Packet sent\n");
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
			game->selectedPiece = board->table[board->selectedX][board->selectedY];

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
			getInputOnBoard(wintest, &x, &y);
			*newType = x + 1; 
			choose = 1;
		}
	}
	return wintest;
}