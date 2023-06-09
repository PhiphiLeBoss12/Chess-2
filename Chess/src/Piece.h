#include "Types.h"

SDL_Texture* chooseTexturePiece(TypePiece type, TypeColor color, Window* window);
Piece* initPiece(TypePiece type, TypeColor color, int x, int y, Window* window); //, Board* board);
void destroyPiece(Piece* piece);

void showTypePiece(TypePiece type);
void showCoord(int x, int y);
void showCoordPiece(Piece* piece);
void showPiece(Piece* piece);

Case *movePossibilitiesPiece(Piece* piece, Board* board, int* sizeTabPossibilities);
Case* movePossibilitiesPawn(Piece* piece, Board* board, int* sizeTabPossibilities);
Case* movePossibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities);
Case* movePossibilitiesKnight(Piece* piece, Board* board, int* sizeTabPossibilities);
Case* movePossibilitiesRook(Piece* piece, Board* board, int* sizeTabPossibilities);
Case* movePossibilitiesQueen(Piece* piece, Board* board, int* sizeTabPossibilities);
Case* movePossibilitiesKing(Piece* piece, Board* board, int* sizeTabPossibilities);


void movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad);
int searchPieceInTablePlay(Player play, Piece piece);
int getPosVideEaten(Player play);
void affTabPlayer(Player play);
void affEatenPlayer(Player play);
