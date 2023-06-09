#include "Types.h"

SDL_Texture* chooseTexturePiece(TypePiece type, TypeColor color, Window* window);
Piece* initPiece(TypePiece type, TypeColor color, int x, int y, Window* window);
void destroyPiece(Piece* piece);

void showTypePiece(TypePiece type);
void showCoord(int x, int y);
void showCoordPiece(Piece* piece);
void showPiece(Piece* piece);

Cell *movePossibilitiesPiece(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesPawn(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesKnight(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesRook(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesQueen(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesKing(Piece* piece, Board* board, int* sizeTabPossibilities);


void movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad);
int searchPieceInTablePlay(Player play, Piece piece);
int getPosVideEaten(Player play);
