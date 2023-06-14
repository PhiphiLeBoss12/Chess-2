/*
typedef struct StructPlayer {
	Piece** table;
	TypeColor color;
	TypeState check;
} Player;
*/

#include "Player.h"
#include "Piece.h"
#include <string.h>

Player* initPlayers(TypeColor coloor, Window* window) {
	Player* player;
	player = (Player*)malloc(sizeof(Player));
	player->color = coloor;
	player->check = 0;
	player->table = (Piece**)malloc(16 * sizeof(Piece*));
	player->eaten = (Piece**)malloc(16 * sizeof(Piece*));
	for (int i = 0; i < 16; i++)
	{
		player->eaten[i] = NULL;
	}
	int pawn, pieces;
	if (coloor==WHITE)
	{
		// printf("WHITE PLAYER INIT \n");
		pawn = 1; pieces = 0;
	}
	else {
		// printf("BLACK PLAYER INIT \n");
		pawn = 6; pieces = 7;
	}
	// Init
	for (int i = 0; i < 8; i++)
	{
		player->table[i] = initPiece(PAWN, coloor, i, pawn, window);
	}
	player->table[8] = initPiece(ROOK,		coloor, 0, pieces, window);
	player->table[9] = initPiece(KNIGHT,	coloor, 1, pieces, window);
	player->table[10] = initPiece(BISHOP,	coloor, 2, pieces, window);
	player->table[11] = initPiece(QUEEN,	coloor, 3, pieces, window);
	player->table[12] = initPiece(KING,		coloor, 4, pieces, window);
	player->table[13] = initPiece(BISHOP,	coloor, 5, pieces, window);
	player->table[14] = initPiece(KNIGHT,	coloor, 6, pieces, window);
	player->table[15] = initPiece(ROOK,		coloor, 7, pieces, window);
	// printf("Player init done\n");
	return player;
}

void freePlayer(Player* player) {
	free(player->table);
	free(player->eaten);
	free(player);
}

void printPlayer(Player* player) {
	printf("\nPLAYER :\n");
	printf("-Color : %d\n", player->color);
	printf("-State : %d\n", player->check);
	printf("-Table : [");
	for (int i = 0; i < 16; i++)
	{
		printf("%d ", player->table[i]->type);
	}
	printf("]\n");
}

void putInBoard(Player* player, Board* board) {
	Piece** t = player->table;
	int x, y;
	for (int i = 0; i < 16; i++)
	{
		x = t[i]->x;
		y = t[i]->y;
		board->table[x][y] = t[i];
	}
}

Player* createPlayerCopy(Player* player) {
	Player* newPlayer = malloc(sizeof(Player));

	newPlayer->table = (Piece**)malloc(16 * sizeof(Piece*));
	for (int i = 0; i < 16; i++) {
		newPlayer->table[i] = (Piece*)malloc(sizeof(Piece));
		if (player->table[i]) {
			newPlayer->table[i]->color = player->table[i]->color;
			newPlayer->table[i]->x = player->table[i]->x;
			newPlayer->table[i]->y = player->table[i]->y;
			newPlayer->table[i]->hasMovedOnce = player->table[i]->hasMovedOnce;
			newPlayer->table[i]->type = player->table[i]->type;
		}
		else
			newPlayer->table[i] = NULL;
	}

	newPlayer->eaten = (Piece**)malloc(16 * sizeof(Piece*));
	for (int i = 0; i < 16; i++) {
		newPlayer->eaten[i] = (Piece*)malloc(sizeof(Piece));
		if (player->eaten[i])
			memcpy(newPlayer->eaten[i], player->eaten[i], sizeof(Player));
		else
			newPlayer->eaten[i] = NULL;
	}

	newPlayer->color = player->color;
	newPlayer->check = player->check;

	return newPlayer;
}