#include "Piece.h"
#include <string.h>

SDL_Texture* chooseTexturePiece(TypePiece type, TypeColor color, Window *window) {
	char path[100];
	if (color == WHITE)
		strcpy(path, "White_");
	else
		strcpy(path, "Black_");

	switch (type) {
	case PAWN:
		strcat(path, "Pawn.png");
		break;
	case BISHOP:
		strcat(path, "Bishop.png");
		break;
	case KNIGHT:
		strcat(path, "Knight.png");
		break;
	case ROOK:
		strcat(path, "Rook.png");
		break;
	case QUEEN:
		strcat(path, "Queen.png");
		break;
	case KING:
		strcat(path, "King.png");
		break;
	}
	return createTexture(window, path);
}

Piece *initPiece(TypePiece type, TypeColor color, int x, int y, Window* window) {
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
	piece->texture = chooseTexturePiece(piece->type, piece->color, window);
	return piece;
}

void destroyPiece(Piece* piece) {
	destroyTexture(piece->texture);
	free(piece);
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
char *showCoord(int x, int y) {
	char* coord;
	coord = malloc(sizeof(char*) * 2);
	switch (x) {
	case 0:
		coord = "A";
		break;
	case 1:
		coord = "B";
		break;
	case 2:
		coord = "C";
		break;
	case 3:
		coord = "D";
		break;
	case 4:
		coord = "E";
		break;
	case 5:
		coord = "F";
		break;
	case 6:
		coord = "G";
		break;
	case 7:
		coord = "H";
		break;
	}
	char* charNumber;
	sprintf(charNumber, "%d", y + 1);
	strcat(coord, charNumber);
	return coord;
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
	
	if (piece->type == PAWN && (piece->color == WHITE && piece->y == 7) || (piece->color == BLACK && piece->y == 0))
	{
		piece->type = QUEEN;
	}
	board->table[x][y] = piece; //pi�ce d�placer
	piece->hasMovedOnce = 1;
}

void affTabPlayer(Player play) {
	for (int i = 0; i < 16; i++) {
		if (play.table[i] != NULL) {
			switch (play.table[i]->type) {
				case 0:
					printf("P ");
					break;
				case 1:
					printf("F ");
					break;
				case 2:
					printf("C ");
					break;
				case 3:
					printf("T ");
					break;
				case 4:
					printf("Q ");
					break;
				case 5:
					printf("R ");
					break;
			}
		}
		else {
			printf("_ ");
		}
	}
}

void affEatenPlayer(Player play) {
	for (int i = 0; i < 16; i++) {
		if (play.eaten[i] != NULL) {
			switch (play.eaten[i]->type) {
			case 0:
				printf("P ");
				break;
			case 1:
				printf("F ");
				break;
			case 2:
				printf("C ");
				break;
			case 3:
				printf("T ");
				break;
			case 4:
				printf("Q ");
				break;
			case 5:
				printf("R ");
				break;
			}
		}
		else {
			printf("_ ");
		}
	}
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
		if (play.table[i] != NULL) {
			if ((play.table[i]->x == piece.x) && (play.table[i]->y == piece.y)) {
				pos = i;
			}
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

Cell *movePossibilitiesPiece(Piece* piece, Board* board, int* sizeTabPossibilities, Player* playNice, Player* playBad) {
	Cell* tab;
	switch (piece->type) {
	case PAWN:
		tab = movePossibilitiesPawn(piece, board, sizeTabPossibilities);
		break;
	case BISHOP:
		tab = movePossibilitiesBishop(piece, board, sizeTabPossibilities);
		break;
	case KNIGHT:
		tab = movePossibilitiesKnight(piece, board, sizeTabPossibilities);
		break;
	case ROOK:
		tab = movePossibilitiesRook(piece, board, sizeTabPossibilities);
		break;
	case QUEEN:
		tab = movePossibilitiesQueen(piece, board, sizeTabPossibilities);
		break;
	case KING:
		tab = movePossibilitiesKing(piece, board, sizeTabPossibilities);
		break;
	default:
		*sizeTabPossibilities = 0;
		return NULL;
	}
	//Verify check
	/*
	Case* newTab;
	int index = 0;
	Board tempBoard = *board;
	for (int i = 0; i < *sizeTabPossibilities; i++) {
		movePiece(piece, tab[i].x, tab[i].y, &tempBoard, playNice, playBad);
		if (!isCheck(tempBoard, piece->color)) {
			newTab[index] = tab[i];
			index++;
		}
	}
	return newTab;*/
	return tab;
}

Cell *movePossibilitiesPawn(Piece* piece, Board *board, int *sizeTabPossibilities) {
	Cell* tab;
	int index = 0; //index in the table
	tab = malloc(sizeof(Cell) * 4);
	Cell cell;
	int mult = 1;
	if (piece->color == BLACK)
		mult = -1;
	//To move 1 case
	if (piece->y + 1*mult < SIZE) {
		if (board->table[piece->x][piece->y + 1*mult] == NULL) {
			cell.x = piece->x;
			cell.y = piece->y + 1*mult;
			tab[index] = cell;
			index++;
		}
	}
	//To move 2 cells
	if (piece->y + 2*mult < SIZE) {
		if (board->table[piece->x][piece->y + 1*mult] == NULL && board->table[piece->x][piece->y + 2*mult] == NULL && piece->hasMovedOnce == 0) {
			cell.x = piece->x;
			cell.y = piece->y + 2*mult;
			tab[index] = cell;
			index++;
		}
	}
	int add;
	for (int power = 1; power < 3; power++) { //Left and Right
		add = pow((-1), power);
		if (piece->x + add*mult >= 0 && piece->x + add*mult < SIZE && piece->y + 1*mult < SIZE) { //Verify border
			if (board->table[piece->x + add*mult][piece->y + 1*mult] != NULL ) {
				if (board->table[piece->x + add * mult][piece->y + 1 * mult]->color != piece->color) {
					cell.x = piece->x + add * mult;
					cell.y = piece->y + 1 * mult;
					tab[index] = cell;
					index++;
				}
			}
		}
	}
	*sizeTabPossibilities = index;
	return tab;
}

Cell* movePossibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Cell* tab;
	Cell cell;
	tab = malloc(sizeof(Cell) * 13);
	int index = 0;
	int i, mult, X, Y;
	for (int power = 1; power < 3; power++) { //Left and Right
		mult = pow((-1), power);
		for (i = 1; i < SIZE; i++) { //1 to 7 (top line)
			X = piece->x + i * mult;
			Y = piece->y + i * mult;
			if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
				if (board->table[X][Y] == NULL) {
					cell.x = X;
					cell.y = Y;
					tab[index] = cell;
					index++;
				}
				else {
					if (board->table[X][Y]->color != piece->color) {
						cell.x = X;
						cell.y = Y;
						tab[index] = cell;
						index++;
					}
					break; //We're out of the loop because there will be no more accessible pieces in front
				}
			}
		}
		for (i = -1; i > -SIZE; i--) { //-1 to -7 (bottom line)
			X = piece->x + i * mult;
			Y = piece->y - i * mult;
			if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
				if (board->table[X][Y] == NULL) {
					cell.x = X;
					cell.y = Y;
					tab[index] = cell;
					index++;
				}
				else {
					if (board->table[X][Y]->color != piece->color) {
						cell.x = X;
						cell.y = Y;
						tab[index] = cell;
						index++;
					}
					break; //We're out of the loop because there will be no more accessible pieces in front
				}
			}
		}
	}
	*sizeTabPossibilities = index;
	return tab;
}


Cell* movePossibilitiesKnight(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Cell* tab;
	Cell cell;
	tab = malloc(sizeof(cell) * 8);
	int index = 0;
	int add, X, Y, mult2;
	for (int mult = 1; mult < 3; mult++) {
		for (int power = 1; power < 3; power++) {
			for (int power2=1; power2 < 3; power2++) {
				add = pow((-1), power);
				mult2 = pow((-1), power2);
				X = piece->x + add * mult;
				Y = piece->y + (2*mult2) / mult;
				if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
					if (board->table[X][Y] == NULL) {
						cell.x = X;
						cell.y = Y;
						tab[index] = cell;
						index++;

					}
					else {
						if (board->table[X][Y]->color != piece->color) {
							cell.x = X;
							cell.y = Y;
							tab[index] = cell;
							index++;
						}
					}
				}	
			}
		}
	}
	*sizeTabPossibilities = index;
	return tab;
}

Cell * movePossibilitiesRook(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Cell* tab;
	Cell cell;
	tab = malloc(sizeof(Cell) * 14);
	int index = 0;
	int i, mult, X, Y;
	//X line
	X = piece->x;
	for (i = 1; i < SIZE; i++) { //1 to 7 (top line)
		Y = piece->y + i;
		if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
			if (board->table[X][Y] == NULL) {
				cell.x = X;
				cell.y = Y;
				tab[index] = cell;
				index++;
			}
			else {
				if (board->table[X][Y]->color != piece->color) {
					cell.x = X;
					cell.y = Y;
					tab[index] = cell;
					index++;
				}
				break; //We're out of the loop because there will be no more accessible pieces in front
			}
		}
	}
	for (i = -1; i > -SIZE; i--) { //-1 to -7 (bottom line)
		Y = piece->y + i;
		if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
			if (board->table[X][Y] == NULL) {
				cell.x = X;
				cell.y = Y;
				tab[index] = cell;
				index++;
			}
			else {
				if (board->table[X][Y]->color != piece->color) {
					cell.x = X;
					cell.y = Y;
					tab[index] = cell;
					index++;
				}
				break; //We're out of the loop because there will be no more accessible pieces in front
			}
		}
	}
	Y = piece->y;
	for (i = 1; i < SIZE; i++) { //1 to 7 (top line)
		X = piece->x + i;
		if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
			if (board->table[X][Y] == NULL) {
				cell.x = X;
				cell.y = Y;
				tab[index] = cell;
				index++;
			}
			else {
				if (board->table[X][Y]->color != piece->color) {
					cell.x = X;
					cell.y = Y;
					tab[index] = cell;
					index++;
				}
				break; //We're out of the loop because there will be no more accessible pieces in front
			}
		}
	}
	for (i = -1; i > -SIZE; i--) { //-1 to -7 (bottom line)
		X = piece->x + i;
		if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
			if (board->table[X][Y] == NULL) {
				cell.x = X;
				cell.y = Y;
				tab[index] = cell;
				index++;
			}
			else {
				if (board->table[X][Y]->color != piece->color) {
					cell.x = X;
					cell.y = Y;
					tab[index] = cell;
					index++;
				}
				break; //We're out of the loop because there will be no more accessible pieces in front
			}
		}
	}
	*sizeTabPossibilities = index;
	return tab;
}

Cell* movePossibilitiesQueen(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Cell* tab;
	tab = malloc(sizeof(Cell) * 27);
	int index = 0;
	int* len = malloc(sizeof(int));
	int i;

	Cell* tabBishop;
	tabBishop = movePossibilitiesBishop(piece, board, sizeTabPossibilities);
	for (i = 0; i < *sizeTabPossibilities; i++) {
		tab[index] = tabBishop[i];
		index++;
	}

	Cell* tabRook;
	tabRook = movePossibilitiesRook(piece, board, len);
	for (i = 0; i < *len; i++) {
		tab[index] = tabRook[i];
		index++;
	}

	*sizeTabPossibilities += *len;
	return tab;
}

Cell* movePossibilitiesKing(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Cell* tab;
	tab = malloc(sizeof(Cell) * 27);
	Cell cell;
	int index = 0;
	int X, Y;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			X = piece->x + i;
			Y = piece->y + j;
			if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE && (piece->x != X || piece->y != Y)) {
				if (board->table[X][Y] == NULL) {
					cell.x = X;
					cell.y = Y;
					tab[index] = cell;
					index++;
				}
				else {
					if (board->table[X][Y]->color != piece->color) {
						cell.x = X;
						cell.y = Y;
						tab[index] = cell;
						index++;
					}
				}
			}
		}
	}
	*sizeTabPossibilities = index;
	return tab;
}



