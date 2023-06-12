#pragma once
#include "Types.h"

Board* createBoard(int size);
void destroyBoard(Board* board);

void displayBoardConsole(Board* board);

Case getKingPosition(Board* board, TypeColor color);
int knightIsMenacing(Board* board, TypeColor color, Case king);
int isCheck(Board* board, TypeColor color);