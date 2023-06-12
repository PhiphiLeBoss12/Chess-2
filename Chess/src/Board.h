#pragma once
#include "Types.h"

Board* createBoard(int size);
void destroyBoard(Board* board);

void displayBoardConsole(Board* board);

Cell getKingPosition(Board* board, TypeColor color);
int knightIsMenacing(Board* board, TypeColor color, Cell king);
int isCheck(Board* board, TypeColor color);