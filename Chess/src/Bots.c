#include "Bots.h"
#include <time.h>
#include <stdlib.h>

void RandomBot(Window* window, Board* board, Piece** selectedPiece, Player** players, Cell* possibilities, int numPossibilities, int squareSize, TypeColor* whoPlays) {
	srand(time(NULL));
	int randomPieceNum, randomPossibilityNum;
	Piece* randomPiece;
	Cell possibility;
	while (numPossibilities == 0) {
		randomPieceNum = rand() % 16;
		randomPiece = players[0]->table[randomPieceNum];
		possibilities = getPossibilities(randomPiece, whoPlays, board, &numPossibilities);
		if (numPossibilities != 0) {
			randomPossibilityNum = rand() % numPossibilities;
			possibility = possibilities[randomPossibilityNum];
			gayme(window, board, selectedPiece, players, possibilities, numPossibilities, squareSize, whoPlays, possibility.x, possibility.y);
		}
	}
}