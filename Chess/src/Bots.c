#include "Bots.h"
#include <stdlib.h>
#include <time.h>

void RandomBot(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays) {
	srand(time(NULL));
	Cell possibility;
	int randomPieceNum;
	int randomPossibilitiesNum;
	int i;
	//gayme(window, board, selectedPiece, players, possibilities, numPossibilities, squareSize, &whoPlays, 0, 0);
	for (i = 0; i < 100 && numPossibilities == 0; i++) {
		randomPieceNum = rand() % 16;
		Piece* randomPiece = players[1]->table[randomPieceNum];
		possibilities = getPossibilities(randomPiece, whoPlays, board, &numPossibilities);
		if (possibilities != NULL) {
			randomPossibilitiesNum = rand() % numPossibilities;
			possibility = possibilities[randomPossibilitiesNum];
			gayme(window, board, randomPiece, players, possibilities, numPossibilities, squareSize, &whoPlays, possibility.x, possibility.y);
		}
	}
	if (i >= 100) {
		__debugbreak();
	}
	printf("Sortie de fonction\n");
}