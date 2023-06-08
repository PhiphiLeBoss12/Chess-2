#include "Piece.h"

Piece *createPiece(TypePiece type, TypeColor color, int x, int y, Board *board) {
	if (!(0 <= x && x < 8 && 0 <= y && y < 8)) { //check if coord is in the table
		printf("Failed : This piece isn't in the table.");
		return NULL;
	}
	Piece* piece;
	piece = malloc(sizeof(Piece));
	piece->type = type;
	piece->color = color;
	piece->x = x;
	piece->y = y;
	piece->hasMoved = 1; //False
	piece->hasMovedOnce = 1; //False
	board->table[x][y] = piece; //put the piece in the board
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


void showCoordPiece(Piece* piece) {
	switch (piece->x) {
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
	printf("%d",piece->y+1);
}

void showPiece(Piece* piece) { 
	showTypePiece(piece->type);
	printf(": ");
	showCoordPiece(piece);
	printf("\n");
}

/* movePiece
	utilit� :	� d�placer une pi�ce sur l'�chiquier
				� manger un ennemi
	input :	� Piece* piece (pi�ce � d�placer)
			� int x, int y (nouvelle position)
			� Board* board (plateau du jeu)
			� Player* playNice (joueur � qui app la pi�ce)
			� Player* playBad (joeur adverse)
	output :	� void (rien) */
void movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad) {
	board->table[piece->x][piece->y] = NULL; //ancienne position
	if (board->table[x][y] != NULL) { //si ennemi
		int pos = getPosVideEaten(*playNice); //o� il y a un emplacement vide dans eaten
		playNice->eaten[pos] = board->table[x][y]; //ajout ennemi � la liste eaten du player
		int pos2 = searchPieceInTablePlay(*playBad, *(board->table[x][y])); //recherche la pi�ce mang� dans la liste du joueur adverse
		playBad->table[pos2] = NULL; //suppression de la pi�ce mang�
	}
	piece->x = x;
	piece->y = y;
	board->table[x][y] = piece; //pi�ce d�placer
}

/* searchPieceInTablePlay
	utilit� :	� trouver la position d'une pi�ce d'un player
	input :	� Player play (le joueur)
			� Piece piece (la pi�ce)
	output :	� la position de la pi�ce
				� -1 si non trouv� */
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
	utilit� :	� trouver une position vide dans eaten
	input :	� Player play (le joueur)
	output :	� int (la position) */
int getPosVideEaten(Player play) {
	Piece* tempo = play.eaten[0];
	int i = 0;
	while (tempo != NULL) {
		i += 1;
		tempo = play.eaten[i];
	}
	return i;
}

Case *movePosibilitiesPawn(Piece* piece, Board *board) {
	Case* tab;
	int index = 0;
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
	if (board->table[piece->x][piece->y + 1] == NULL && board->table[piece->x][piece->y + 2] == NULL && piece->hasMovedOnce != 0) { 
		casePos.x = piece->x;
		casePos.y = piece->y + 2;
		tab[index] = casePos;
		index++;
	}
	//Left move
	if (piece->x - 1 >= 0) { //Verify border
		if (board->table[piece->x - 1][piece->y + 1] == NULL || board->table[piece->x - 1][piece->y + 1]->color != piece->color) {
			casePos.x = piece->x - 1;
			casePos.y = piece->y + 1;
			tab[index] = casePos;
			index++;
		}
	}
	//Right move
	if (piece->x + 1 >= 0) { //Verify border
		if (board->table[piece->x + 1][piece->y + 1] == NULL || board->table[piece->x + 1][piece->y + 1]->color != piece->color) {
			casePos.x = piece->x + 1;
			casePos.y = piece->y + 1;
			tab[index] = casePos;
			index++;
		}
	}
	return tab;
}



