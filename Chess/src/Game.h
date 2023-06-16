#pragma once
#include "Window.h"
#include "Piece.h"
#include "Player.h"
#include "UI.h"
#include "Sound.h"
#include "Network.h"

#define SQUARE_SIZE 100

typedef enum GameState { START, PLAYING, END, STALEMATE, QUIT } GameState;
typedef enum Musics { MUSIC_MENU, MUSIC_GAME } Musics;
typedef enum Sounds { SOUND_STEP, SOUND_KILL, SOUND_STALEMATE, SOUND_CHECK, SOUND_FUNNY } Sounds;

typedef struct MovePacket {
	int oldX, oldY;
	int newX, newY;
	int sent;
} MovePacket;

typedef struct Game {
	GameState gameState;

	Board* board;
	Player* players[2];
	LastMove* last;
	Piece* selectedPiece;
	TypeColor whoPlays;
	int promo;

	SDL_Texture** textures;

	Mix_Music* musics[2];
	Mix_Chunk* sounds[5];
	int enableMusic;

	IPaddress ipServer, ipClient;
	TCPsocket tcpServer, tcpClient;
	int multiplayerServer;
	int multiplayerClient;
} Game;

// Main game logic
void gameLoop();
void start(Window* window, Game* game);
void playing(Window* window, Game* game);
void end(Window* window, Game* game);

// Initializes a game object
// It needs to be destroyed afterwards
Game* initGame(Window* window);

// Destroys an existing game object
void destroyGame(Game* game);

// Allocates and returns an array containing all the textures
// You can access a texture at index `type + 6 * color`
SDL_Texture** createTextureArray(Window* window);

void doNetwork(Window* window, Game* game);
void fillGamePacketRecieve(Game* game, MovePacket* packet);

// Resets the board to the starting position
void resetBoard(Window* window, Game* game);

// Toggles music
// This doesn't need documentation
void toggleMusic(Window* window, Game* game);

// Gets the input
void getInputOnBoard(Window* window, int* boardX, int* boardY);

// Simple helper function
Cell* getPossibilities(Game* game, int* numPossibilities);

// Draws the play board on screen
// The board is always drawn at the top-left corner of the screen
// Should be called before drawing pieces
void drawBoard(Window* window, Game* game);

// Draws all move possibilities of the selected piece if a piece is selected
void drawPossibilities(Window* window, Game* game, Cell* possibilities, int numPossibilities);

// Big ahh function to react when the mouse is clicked
void handleMouseClicking(Window* window, Game* game, Cell* possibilities, int numPossibilities, int* promo);

// jsp c'est à marie ça
Window* winPromo(const char* title, Piece* pawn, TypePiece* newType);