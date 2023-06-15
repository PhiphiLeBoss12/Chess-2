#include "Types.h"

Piece* initPiece(TypePiece type, TypeColor color, int x, int y, Window* window);
void destroyPiece(Piece* piece);

void showTypePiece(TypePiece type);
char* showCoord(int x, int y);
char* showCoordPiece(Piece* piece);
void showPiece(Piece* piece);

Cell* movePossibilitiesPiece(Piece* piece, Board* board, int* sizeTabPossibilities, LastMove* last);
Cell* movePossibilitiesPawn(Piece* piece, Board* board, int* sizeTabPossibilities, LastMove* last);
Cell* movePossibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesKnight(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesRook(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesQueen(Piece* piece, Board* board, int* sizeTabPossibilities);
Cell* movePossibilitiesKing(Piece* piece, Board* board, int* sizeTabPossibilities);


int movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad, LastMove* last, int* promo);
int searchPieceInTablePlay(Player play, Piece piece);
int getPosVideEaten(Player play);
void affTabPlayer(Player play);
void affEatenPlayer(Player play);
LastMove* initLastMove();
void affLastCoup(LastMove last);

int isCheck(Board* board, TypeColor color);
Board* simulateMove(Board* board, Piece* piece, Cell possibility, Player* playNice, Player* playBad, LastMove* last, int* promo);
int isCheckmate(Board* board, TypeColor color, Player* playNice, Player* playBad, LastMove* last, int* promo);
void testPossibilitiesCheck(Board* board, TypeColor color, Player* playNice, Player* playBad, LastMove* last, Piece* piece, Cell* possibilities, int numPossibilities, int* promo);

int isStalemate(Board* board, Player** players, LastMove* last, int* promo);