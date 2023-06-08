#include "Types.h"

SDL_Texture chooseTexturePiece(TypePiece type, TypeColor color);
Piece* initPiece(TypePiece type, TypeColor color, int x, int y); //, Board* board);
void showTypePiece(TypePiece type);
void showCoord(int x, int y);
void showCoordPiece(Piece* piece);
void showPiece(Piece* piece);

Case* movePosibilitiesPawn(Piece* piece, Board* board, int* sizeTabPossibilities);
//Case* movePosibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities);
//Case* movePosibilitiesKnight(Piece* piece, Board* board);

void movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad);
int searchPieceInTablePlay(Player play, Piece piece);
int getPosVideEaten(Player play);
