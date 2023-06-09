#include <stdio.h>
#include <SDL2/SDL.h>
#include "Piece.h"
#include "Board.h"
#include "Window.h"
#include "Game.h"
#include <math.h>

/* ------------------- */
/*   ZONE TEST MARIE   */
/* ------------------- */

/* affBoardTest
	utilité :	¤ visulaiser mon tableau pendant mes tests
	input :	¤ Board board (le plateau)
	output :	¤ void (rien) */
void affBoardTest(Board board) {
	printf("\nTableau :\n");
	for (int i = 0; i < 8; i++) {
		printf("\t");
		for (int j = 0; j < 8; j++) {
			if (board.table[i][j] != NULL) {
				switch (board.table[i][j]->type) {
				case 0:
					printf("P ");
					break;
				case 1:
					printf("F ");
					break;
				case 2:
					printf("C ");
					break;
				case 3:
					printf("T ");
					break;
				case 4:
					printf("Q ");
					break;
				case 5:
					printf("K ");
					break;
				}
			}
			else {
				printf("X ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

/* affTabTest
	utilité :	¤ visulaiser mon tableau pendant mes tests
	input :	¤ Board board (le plateau)
	output :	¤ void (rien) */
void affTabTest(Piece* tab[]) {
	for (int i = 0; i < 8; i++) {
		if (tab[i]->type != NULL) {
			switch (tab[i]->type) {
				case 0:
					printf("P ");
					break;
				case 1:
					printf("F ");
					break;
				case 2:
					printf("C ");
					break;
				case 3:
					printf("T ");
					break;
				case 4:
					printf("Q ");
					break;
				case 5:
					printf("K ");
					break;
			}
		}
		else {
			printf("X ");
		};
	}
	printf("\n");
}

/* testMainMarie
	utilité :	¤ tester mes fonctions bruh
	input :	¤ void (rien bruh)
	output :	¤ void (rien bruh) */
void testMainMarie() {
	//init board
	Board* board = createBoard(64);
	//init pièce
	Piece* piece;
	piece = initPiece(PAWN, WHITE, 1, 0, board);
	Piece* piece2;
	piece2 = initPiece(PAWN, BLACK, 2, 0, board);
	//init player
	Player* playBlanc;
	Player* playNoir;
	playBlanc->color = WHITE;
	playNoir->color = BLACK;
	playBlanc->check = COOL;
	playNoir->check = COOL;
	Piece* tabB[16];
	tabB[0] = piece;
	Piece* eatB[16];
	Piece* tabN[16];
	tabN[3] = piece2;
	Piece* eatN[16];
	playBlanc->table = tabB;
	playBlanc->eaten = eatB;
	playBlanc->table = tabN;
	playBlanc->eaten = eatN;

	//affichage de base
	printf("Pion blanc :\n");
	affTabTest(playBlanc->table);
	printf("Pion noir :\n");
	affTabTest(playNoir->table);
}

/* ------------------- */
/*        MAIN         */
/* ------------------- */

int main(int argc, char* argv[])
{

	//testMainMarie();

    Board* board;
    board = malloc(sizeof(Board));
    Piece* tab[8][8];
    board->size = 64;
    board->table = tab;
    for (int i = 0; i < 8; i++) {
        board->table[i] = malloc(sizeof(Piece*)*8);
        for (int j = 0; j < 8; j++) {
            board->table[i][j] = NULL;
        }
    }

    Piece* piece;
    piece = initPiece(BISHOP, WHITE, 1, 0, board);
    showPiece(piece);

    //Piece* pieceB;
    //pieceB = initPiece(PAWN, WHITE, 0, 1, board);

    Case* tabCase;
    int* lenTab = malloc(sizeof(int));
    tabCase = movePossibilitiesPawn(piece, board, lenTab);
    for (int i = 0; i < *lenTab; i++) {
        showCoord(tabCase[i].x, tabCase[i].y);
        printf("\n");
    }
    return 0;
}