#include "Types.h"

Piece* createPiece(TypePiece type, TypeColor color, int x, int y, Board* board);
void showTypePiece(TypePiece type);
void showCoordPiece(Piece* piece);
void showPiece(Piece* piece);
Case* movePosibilitiesPawn(Piece* piece, Board* board);

void movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad);
int searchPieceInTablePlay(Player play, Piece piece);
int getPosVideEaten(Player play);