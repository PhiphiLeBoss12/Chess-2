#include "Types.h"

Player* initPlayers(TypeColor color, Window* window);

void freePlayer(Player* player);

void printPlayer(Player* player);

void putInBoard(Player* player, Board* board);

Player* createPlayerCopy(Player* player);