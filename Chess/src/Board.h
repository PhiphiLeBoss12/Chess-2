#pragma once
#include "Types.h"

Board* createBoard(int size);
void destroyBoard(Board* board);

void displayBoardConsole(Board* board);

Board* createBoardCopy(Board* board);