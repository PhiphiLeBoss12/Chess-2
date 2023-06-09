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

Case *movePossibilitiesPiece(Piece* piece, Board* board, int* sizeTabPossibilities) {
	switch (piece->type) {
	case PAWN:
		return movePossibilitiesPawn(piece, board, sizeTabPossibilities);
		break;
	case BISHOP:
		return movePossibilitiesBishop(piece, board, sizeTabPossibilities);
		break;
	case KNIGHT:
		return movePossibilitiesKnight(piece, board, sizeTabPossibilities);
		break;
	default:
		*sizeTabPossibilities = 0;
		return NULL;
	}
}

Case *movePossibilitiesPawn(Piece* piece, Board *board, int *sizeTabPossibilities) {
	Case* tab;
	int index = 0; //index in the table
	tab = malloc(sizeof(Case) * 4);
	Case casePos;
	//To move 1 case
	if (piece->y + 1 < SIZE) {
		if (board->table[piece->x][piece->y + 1] == NULL) {
			casePos.x = piece->x;
			casePos.y = piece->y + 1;
			tab[index] = casePos;
			index++;
		}
	}
	//To move 2 cases
	if (piece->y + 2 < SIZE) {
		if (board->table[piece->x][piece->y + 1] == NULL && board->table[piece->x][piece->y + 2] == NULL && piece->hasMovedOnce == 0) {
			casePos.x = piece->x;
			casePos.y = piece->y + 2;
			tab[index] = casePos;
			index++;
		}
	}
	int add;
	for (int power = 1; power < 3; power++) { //Left and Right
		add = pow((-1), power);
		if (piece->x + add >= 0 && piece->x + add < SIZE && piece->y + 1 < SIZE) { //Verify border
			if (board->table[piece->x + add][piece->y + 1] != NULL ) {
				if (board->table[piece->x + add][piece->y + 1]->color != piece->color) {
					casePos.x = piece->x + add;
					casePos.y = piece->y + 1;
					tab[index] = casePos;
					index++;
				}
			}
		}
	}
	*sizeTabPossibilities = index;
	return tab;
}

Case* movePossibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Case* tab;
	Case casePos;
	tab = malloc(sizeof(Case) * 13);
	int index = 0;
	int i, mult, X, Y;
	for (int power = 1; power < 3; power++) { //Left and Right
		mult = pow((-1), power);
		for (i = 1; i < SIZE; i++) { //1 to 7 (top line)
			X = piece->x + i * mult;
			Y = piece->y + i * mult;
			if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
				if (board->table[X][Y] == NULL) {
					casePos.x = X;
					casePos.y = Y;
					tab[index] = casePos;
					index++;
				}
				else {
					if (board->table[X][Y]->color != piece->color) {
						casePos.x = X;
						casePos.y = Y;
						tab[index] = casePos;
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
					casePos.x = X;
					casePos.y = Y;
					tab[index] = casePos;
					index++;
				}
				else {
					if (board->table[X][Y]->color != piece->color) {
						casePos.x = X;
						casePos.y = Y;
						tab[index] = casePos;
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


Case* movePossibilitiesKnight(Piece* piece, Board* board, int* sizeTabPossibilities) {
	Case* tab;
	Case casePos;
	tab = malloc(sizeof(Case) * 8);
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
						casePos.x = X;
						casePos.y = Y;
						tab[index] = casePos;
						index++;

					}
					else {
						if (board->table[X][Y]->color != piece->color) {
							casePos.x = X;
							casePos.y = Y;
							tab[index] = casePos;
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

//Case * movePossibilitiesRook(Piece* piece, Board* board, int* sizeTabPossibilities) {
//	Case* tab;
//	Case casePos;
//	tab = malloc(sizeof(Case) * 14);
//	int index = 0;
//	int i, mult, X, Y;
//	//X line
//	for (int power = 1; power < 3; power++) { //Left and Right
//		mult = pow((-1), power);
//		for (i = 1; i < SIZE; i++) { //1 to 7 (top line)
//			Y = piece->y + i * mult;
//			if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
//				if (board->table[X][Y] == NULL) {
//					casePos.x = X;
//					casePos.y = Y;
//					tab[index] = casePos;
//					index++;
//				}
//				else {
//					if (board->table[X][Y]->color != piece->color) {
//						casePos.x = X;
//						casePos.y = Y;
//						tab[index] = casePos;
//						index++;
//					}
//					break; //We're out of the loop because there will be no more accessible pieces in front
//				}
//			}
//		}
//		for (i = -1; i > -SIZE; i--) { //-1 to -7 (bottom line)
//			X = piece->x + i * mult;
//			Y = piece->y - i * mult;
//			if (0 <= X && X < SIZE && 0 <= Y && Y < SIZE) {
//				if (board->table[X][Y] == NULL) {
//					casePos.x = X;
//					casePos.y = Y;
//					tab[index] = casePos;
//					index++;
//				}
//				else {
//					if (board->table[X][Y]->color != piece->color) {
//						casePos.x = X;
//						casePos.y = Y;
//						tab[index] = casePos;
//						index++;
//					}
//					break; //We're out of the loop because there will be no more accessible pieces in front
//				}
//			}
//		}
//	}
//	*sizeTabPossibilities = index;
//	return tab;
//}



