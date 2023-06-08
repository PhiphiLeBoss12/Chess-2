#include "Piece.h"

SDL_Texture *chooseTexturePiece(TypePiece type, TypeColor color, Window *window) {
	char* path;
	if (color == WHITE)
		path = "White_";
	else
		path = "Black_";

	switch (type) {
	case PAWN:
		strcat(path, "Pawn");
		break;
	case BISHOP:
		strcat(path, "Bishop");
		break;
	case KNIGHT:
		strcat(path, "Knight");
		break;
	case ROOK:
		strcat(path, "Rook");
		break;
	case QUEEN:
		strcat(path, "Queen");
		break;
	case KING:
		strcat(path, "King");
		break;
	}
	printf("%s", path);
	return createTexture(window, path);
}

Piece *initPiece(TypePiece type, TypeColor color, int x, int y, Window *window) {//, Board *board) {
	if (!(0 <= x && x < SIZE && 0 <= y && y < SIZE)) { //check if coord is in the table
		printf("Failed : This piece isn't in the table.");
		__debugbreak();
		return NULL;
	}
	Piece* piece;
	piece = malloc(sizeof(Piece));
	piece->type = type;
	piece->color = color;
	piece->x = x;
	piece->y = y;
	piece->hasMoved = 0; //False
	piece->hasMovedOnce = 0; //False
	piece->texture = chooseTexturePiece(type, color, window);
	//board->table[x][y] = piece; //put the piece in the board
	return piece;
}

void showTypePiece(TypePiece type) { //function to debug
	switch (type) {
	case PAWN:
		printf("PAWN");
		break;
	case BISHOP:
		printf("BISHOP");
		break;
	case KNIGHT:
		printf("KNIGHT");
		break;
	case ROOK:
		printf("ROOK");
		break;
	case QUEEN:
		printf("QUEEN");
		break;
	case KING:
		printf("KING");
		break;
	}
}
void showCoord(int x, int y) {
	switch (x) {
	case 0:
		printf("A");
		break;
	case 1:
		printf("B");
		break;
	case 2:
		printf("C");
		break;
	case 3:
		printf("D");
		break;
	case 4:
		printf("E");
		break;
	case 5:
		printf("F");
		break;
	case 6:
		printf("G");
		break;
	case 7:
		printf("H");
		break;
	}
	printf("%d", y + 1);
}


void showCoordPiece(Piece* piece) {
	showCoord(piece->x, piece->y);
}

void showPiece(Piece* piece) { 
	showTypePiece(piece->type);
	printf(": ");
	showCoordPiece(piece);
	printf("\n");
}

/* movePiece
	utilité :	¤ déplacer une pièce sur l'échiquier
				¤ manger un ennemi
	input :	¤ Piece* piece (pièce à déplacer)
			¤ int x, int y (nouvelle position)
			¤ Board* board (plateau du jeu)
			¤ Player* playNice (joueur à qui app la pièce)
			¤ Player* playBad (joeur adverse)
	output :	¤ void (rien) */
void movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad) {
	board->table[piece->x][piece->y] = NULL; //ancienne position
	if (board->table[x][y] != NULL) { //si ennemi
		int pos = getPosVideEaten(*playNice); //où il y a un emplacement vide dans eaten
		playNice->eaten[pos] = board->table[x][y]; //ajout ennemi à la liste eaten du player
		int pos2 = searchPieceInTablePlay(*playBad, *(board->table[x][y])); //recherche la pièce mangé dans la liste du joueur adverse
		playBad->table[pos2] = NULL; //suppression de la pièce mangé
	}
	piece->x = x;
	piece->y = y;
	board->table[x][y] = piece; //pièce déplacer
}

/* searchPieceInTablePlay
	utilité :	¤ trouver la position d'une pièce d'un player
	input :	¤ Player play (le joueur)
			¤ Piece piece (la pièce)
	output :	¤ la position de la pièce
				¤ -1 si non trouvé */
int searchPieceInTablePlay(Player play, Piece piece) {
	int pos = -1;
	for (int i = 0; i < 16; i++) {
		if ((play.table[i]->x == piece.x) && (play.table[i]->y == piece.y)) {
			pos = i;
		}
	}
	return pos;
}

/* getPosVideEaten
	utilité :	¤ trouver une position vide dans eaten
	input :	¤ Player play (le joueur)
	output :	¤ int (la position) */
int getPosVideEaten(Player play) {
	Piece* tempo = play.eaten[0];
	int i = 0;
	while (tempo != NULL) {
		i += 1;
		tempo = play.eaten[i];
	}
	return i;
}

Case *movePosibilitiesPawn(Piece* piece, Board *board, int *sizeTabPossibilities) {
	Case* tab;
	int index = 0; //index in the table
	tab = malloc(sizeof(Case) * 4);
	Case casePos;
	//To move 1 case
	if (board->table[piece->x][piece->y + 1] == NULL) { 
		casePos.x = piece->x;
		casePos.y = piece->y + 1;
		tab[index] = casePos;
		index++;
	}
	//To move 2 cases
	if (board->table[piece->x][piece->y + 1] == NULL && board->table[piece->x][piece->y + 2] == NULL && piece->hasMovedOnce == 0) { 
		casePos.x = piece->x;
		casePos.y = piece->y + 2;
		tab[index] = casePos;
		index++;
	}
	//Left move
	if (piece->x - 1 >= 0) { //Verify border
		if (board->table[piece->x - 1][piece->y + 1] != NULL && board->table[piece->x - 1][piece->y + 1]->color != piece->color) {
			casePos.x = piece->x - 1;
			casePos.y = piece->y + 1;
			tab[index] = casePos;
			index++;
		}
	}
	//Right move
	if (piece->x + 1 <= SIZE) { //Verify border
		if (board->table[piece->x + 1][piece->y + 1] != NULL && board->table[piece->x + 1][piece->y + 1]->color != piece->color) {
			casePos.x = piece->x + 1;
			casePos.y = piece->y + 1;
			tab[index] = casePos;
			index++;
		}
	}
	*sizeTabPossibilities = index;
	return tab;
}
/*
Case* movePosibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Case* tab;
	Case casePos;
	tab = malloc(sizeof(Case) * 13);
	int index = 0;
	int i, mult, X, Y;
	//Right diagonal
	for (int power = 1; power < 3; power++) { //Left and Right
		mult = pow((-1), power);
		for (i = 1; i < SIZE; i++) { //1 to 7 (top line)
			X = piece->x + i * mult;
			Y = piece->y + i * mult;
			if (board->table[X][Y] == NULL || board->table[X][Y]->color != piece->color) {
				casePos.x = X;
				casePos.y = Y;
				tab[index] = casePos;
				index++;
				if (board->table[X][Y]->color != piece->color) {
					break; //We're out of the loop because there will be no more accessible pieces in front
				}
			}
		}
		for (i = -1; i > -SIZE; i--) { //-1 to -7 (bottom line)
			X = piece->x + i * mult;
			Y = piece->y + i * mult;
			if (board->table[X][Y] == NULL || board->table[X][Y]->color != piece->color) {
				casePos.x = X;
				casePos.y = Y;
				tab[index] = casePos;
				index++;
				if (board->table[X][Y]->color != piece->color) {
					break; //We're out of the loop because there will be no more accessible pieces in front
				}
			}
		}
	}
	*sizeTabPossibilities = index;
	return tab;
*/
/*
Case* movePosibilitiesKnight(Piece* piece, Board* board) {
	Case* tab;
	Case casePos;
	tab = malloc(sizeof(Case) * 8);
	int index = 0;
}*/



