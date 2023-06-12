#include "Bots.h"
#include <stdlib.h>
#include <time.h>

void RandomBot(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays) {
	srand(time(NULL));
	Cell possibility;
	int i, randomPieceNum, randomPossibilitiesNum;

	for (i = 0; i < 100 && numPossibilities == 0; i++) {
		randomPieceNum = rand() % 16;
		Piece* randomPiece = players[0]->table[randomPieceNum];
		*selectedPiece = randomPiece;
		printf("WhoPlays :");
		printf(whoPlays == WHITE ? "White\n" : "Black\n");
		//printf(whoPlays != selectedPiece->color ? "True\n" : "False\n");
		printf(randomPiece->color == WHITE ? "WHITE\n" : "BLACK\n");
		//showPiece(randomPiece);
		//printf(randomPiece->color == WHITE ? "WHite\n" : "Black\n");
		board->selectedX = randomPiece->x;
		board->selectedY = randomPiece->y;
		possibilities = getPossibilities(*selectedPiece, whoPlays, board, &numPossibilities);
		//printf("%d\n", numPossibilities);
		if (possibilities != NULL) {
			randomPossibilitiesNum = rand() % numPossibilities;
			possibility = possibilities[randomPossibilitiesNum];
			gayme(window, board, *selectedPiece, players, possibilities, numPossibilities, squareSize, &whoPlays, possibility.x, possibility.y);
		}
	}
	if (i >= 100) {
		__debugbreak();
	}
	printf("Sortie de fonction\n");
}