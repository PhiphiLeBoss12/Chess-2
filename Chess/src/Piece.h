#include "Types.h"

Piece* createPiece(TypePiece type, TypeColor color, int x, int y, Board* board);
void showTypePiece(TypePiece type);
void showCoordPiece(Piece* piece);
void showPiece(Piece* piece);
void movePiece(Piece* piece, int x, int y, Board* board);
Case* movePosibilitiesPawn(Piece* piece, Board* board);