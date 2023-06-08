#pragma once
#include "Types.h"

Board* createBoard(int size);
void destroyBoard(Board* board);

Board* displayBoardConsole(Board* board);