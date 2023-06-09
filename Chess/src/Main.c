#include <stdio.h>
#include <SDL2/SDL.h>
#include "Piece.h"
#include "Board.h"
#include "Window.h"
#include "Game.h"
#include <math.h>

int main(int argc, char* argv[])
{
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