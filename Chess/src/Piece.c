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
	// piece->texture = chooseTexturePiece(piece->type, piece->color, window);
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
	char *coord;
	coord = malloc(sizeof(char*) * 2);

	char letter;
	switch (x) {
	case 0:
		letter = 'A';
		break;
	case 1:
		letter = 'B';
		break;
	case 2:
		letter = 'C';
		break;
	case 3:
		letter = 'D';
		break;
	case 4:
		letter = 'E';
		break;
	case 5:
		letter = 'F';
		break;
	case 6:
		letter = 'G';
		break;
	case 7:
		letter = 'H';
		break;
	}
	sprintf(coord, "%c%d", letter, y + 1); //concat letter and int and put in string coord
	return coord;
}


char* showCoordPiece(Piece* piece) {
	return showCoord(piece->x, piece->y);
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
	output :	1 si une pièce s'est faite mangée, 0 sinon */
int movePiece(Piece* piece, int x, int y, Board* board, Player* playNice, Player* playBad) {
	int pieceEaten = 0;
	board->table[piece->x][piece->y] = NULL; //ancienne position
	if (board->table[x][y] != NULL) { //si ennemi
		int pos = getPosVideEaten(*playNice); //o� il y a un emplacement vide dans eaten
		playNice->eaten[pos] = board->table[x][y]; //ajout ennemi � la liste eaten du player
		int pos2 = searchPieceInTablePlay(*playBad, *(board->table[x][y])); //recherche la pi�ce mang� dans la liste du joueur adverse
		playBad->table[pos2] = NULL; //suppression de la pi�ce mang�
		pieceEaten = 1;
	}
	piece->x = x;
	piece->y = y;
	
	if (piece->type == PAWN && (piece->color == WHITE && piece->y == 7) || (piece->color == BLACK && piece->y == 0))
	{
		piece->type = QUEEN;
	}
	board->table[x][y] = piece; //pi�ce d�placer
	piece->hasMovedOnce = 1;
	return pieceEaten;
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
	Cell* possibilities;
	//Calculate all the possibilities from choosen piece
	switch (piece->type) {
	case PAWN:
		possibilities = movePossibilitiesPawn(piece, board, sizeTabPossibilities);
		break;
	case BISHOP:
		possibilities = movePossibilitiesBishop(piece, board, sizeTabPossibilities);
		break;
	case KNIGHT:
		possibilities= movePossibilitiesKnight(piece, board, sizeTabPossibilities);
		break;
	case ROOK:
		possibilities= movePossibilitiesRook(piece, board, sizeTabPossibilities);
		break;
	case QUEEN:
		possibilities= movePossibilitiesQueen(piece, board, sizeTabPossibilities);
		break;
	case KING:
		possibilities= movePossibilitiesKing(piece, board, sizeTabPossibilities);
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
	return possibilities;
}

Cell *movePossibilitiesPawn(Piece* piece, Board *board, int *sizeTabPossibilities) { //Moving possibilities of a pawn
	// return all the moving possibilities of a pawn in a Cell's table without verify check

	Cell* possibilities;
	int index = 0; //position in the table possibilities
	possibilities= malloc(sizeof(Cell) * 4); //Max 4 possibilities
	Cell cell;
	int mult = 1; //Initialize to white player

	if (piece->color == BLACK) //Black player plays in negative move
		mult = -1;
	
	if (0 <= piece->y + 1 * mult && piece->y + 1*mult < SIZE) { //Verify border
		//To move 1 case
		if (board->table[piece->x][piece->y + 1*mult] == NULL) { //Empty cell
			cell.x = piece->x;
			cell.y = piece->y + 1*mult;
			possibilities[index] = cell;
			index++;

			//Can't move 2 times if it can't move once
			//To move 2 cells, only when the pawn didn't play once
			if (0 <= piece->y + 2 * mult && piece->y + 2 * mult < SIZE) { //Verify border
				if (board->table[piece->x][piece->y + 2 * mult] == NULL && piece->hasMovedOnce == 0) { //Empty cell in 2 cells in front and never played
					cell.x = piece->x;
					cell.y = piece->y + 2 * mult;
					possibilities[index] = cell;
					index++;
				}
			}
		}

		//Attacks move
		int add;
		for (int power = 1; power < 3; power++) { //Left and Right
			add = pow((-1), power); //add = 1 or -1
			if (piece->x + add * mult >= 0 && piece->x + add * mult < SIZE) { //Verify border
				if (board->table[piece->x + add * mult][piece->y + 1 * mult] != NULL) { //Not empty cell
					if (board->table[piece->x + add * mult][piece->y + 1 * mult]->color != piece->color) { //ennemy piece
						cell.x = piece->x + add * mult;
						cell.y = piece->y + 1 * mult;
						possibilities[index] = cell;
						index++;
					}
				}
			}
		}
	}
	
	*sizeTabPossibilities = index; //length min of possibilities
	return possibilities;
}

Cell* movePossibilitiesBishop(Piece* piece, Board* board, int* sizeTabPossibilities) { //Moving possibilities of a bishop
	// return all the moving possibilities of a bishop in a Cell's table without verify check

	Cell* possibilities;
	Cell cell;
	possibilities = malloc(sizeof(Cell) * 13); //Max 13 possibilities
	int index = 0; //position in the table possibilities
	int right_left, top_bottom;

	for (int power = 1; power < 3; power++) { //Left and Right
		right_left = pow((-1), power); //right_left = -1 or 1, allow to go in left and right

		for (int power2 = 1; power2 < 3; power2++) { //Top and Bottom
			top_bottom = pow((-1), power2); //top_bottom = -1 or 1, allow to go in top and bottom

			//All cell in the line (7 cells max)
			//Didn't find a way to optimize the number of cell to be checked
			for (int i = 1; i < SIZE-1; i++) { 
				//define the coords of the cell
				cell.x = piece->x + i * top_bottom;
				cell.y = piece->y + i * right_left;

				if (0 <= cell.x && cell.x < SIZE && 0 <= cell.y && cell.y < SIZE) { //Verify border
					if (board->table[cell.x][cell.y] == NULL) { //Empty cell
						possibilities[index] = cell;
						index++;
					}
					else {
						if (board->table[cell.x][cell.y]->color != piece->color) { //Ennemy piece
							possibilities[index] = cell;
							index++;
						}
						break; //We're out of the loop because there will be no more accessible pieces in front
					}
				} else {
					break; //There's no point in checking any further
				}
			}
		}
	}
	*sizeTabPossibilities = index; //length min of possibilities
	return possibilities;
}


Cell* movePossibilitiesKnight(Piece* piece, Board* board, int* sizeTabPossibilities) { //Moving possibilities of a knight
	// return all the moving possibilities of a knight in a Cell's table without verify check

	Cell* possibilities;
	Cell cell;
	possibilities = malloc(sizeof(cell) * 8); //Max 8 possibilities
	int index = 0; //position in the table possibilities
	int left_right, top_bottom;

	for (int depth = 1; depth < 3; depth++) { //multiplicate to 1 or 2 the cell 

		//allow to create a variable left_right who multiplicate depth to -1 or 1 to go left or right
		for (int power = 1; power < 3; power++) { 
			left_right = pow((-1), power);

			//allow to create a variable top_bottom who multiplicate depth to -1 or 1 to go in top or bottom uwu
			for (int power2=1; power2 < 3; power2++) { 
				top_bottom = pow((-1), power2);

				//define the coords of the cell
				cell.x = piece->x + left_right * depth; //x +- 1 or 2
				cell.y = piece->y + (2* top_bottom) / depth; // y +- 2*(1 or -1) / (1 or 2)

				if (0 <= cell.x && cell.x < SIZE && 0 <= cell.y && cell.y < SIZE) { //Verify border
					if (board->table[cell.x][cell.y] == NULL) { //Empty piece
						possibilities[index] = cell;
						index++;
					}
					else {
						if (board->table[cell.x][cell.y]->color != piece->color) { //Ennemy cell
							possibilities[index] = cell;
							index++;
						}
					}
				}	
			}
		}
	}
	*sizeTabPossibilities = index; //length min of possibilities
	return possibilities;
}

Cell *movePossibilitiesRook(Piece* piece, Board* board, int* sizeTabPossibilities) { //Moving possibilities of a rook
	// return all the moving possibilities of a rook in a Cell's table without verify check
	Cell* possibilities;
	Cell cell;
	possibilities = malloc(sizeof(Cell) * 14); //Max 14 possibilities
	int index = 0; //position in the table possibilities
	int top_bottom_left_right;

	for (int line = 1; line < 3; line++) {

		if (line == 1) //X line
			cell.y = piece->y;
		else //Y line
			cell.x = piece->x;

		//allow to create a variable top_bottom_left_right who multiplicate cell.y or cell.x to -1 or 1 to go in top or bottom or left or right
		for (int power = 1; power < 3; power++) { 
			top_bottom_left_right = pow((-1), power);

			//All cell in the line (7 cells max)
			//Didn't find a way to optimize the number of cell to be checked
			for (int i = 1; i < SIZE - 1; i++) {

				//define y of the cell
				if (line == 1) //X line
					cell.x = piece->x + i * top_bottom_left_right;
				else //Y line
					cell.y = piece->y + i * top_bottom_left_right;

				if (0 <= cell.x && cell.x < SIZE && 0 <= cell.y && cell.y < SIZE) { //Verify border
					if (board->table[cell.x][cell.y] == NULL) { //Empty cell
						possibilities[index] = cell;
						index++;
					}
					else {
						if (board->table[cell.x][cell.y]->color != piece->color) { //Ennemy piece
							possibilities[index] = cell;
							index++;
						}
						break; //We're out of the loop because there will be no more accessible pieces in front
					}
				} else {
					break; //There's no point in checking any further
				}
			}
		}
	}

	*sizeTabPossibilities = index; //length min of possibilities
	return possibilities;
}

Cell* movePossibilitiesQueen(Piece* piece, Board* board, int* sizeTabPossibilities) { //Moving possibilities of a queen
	// return all the moving possibilities of a queen in a Cell's table without verify check
	Cell* possibilities;
	possibilities = malloc(sizeof(Cell) * 27); //Max 27 possibilities
	int index = 0; //position in the table possibilities
	int i;

	//reclaim all the possibilities like it's a bishop and a rook 

	Cell* possibilitiesBishop;
	possibilitiesBishop = movePossibilitiesBishop(piece, board, sizeTabPossibilities);
	for (i = 0; i < *sizeTabPossibilities; i++) {
		possibilities[index] = possibilitiesBishop[i];
		index++;
	}

	//watch out for the castling
	Cell* possibilitiesRook;
	int* len = malloc(sizeof(int)); //sizeTabPossibilities of the rook
	possibilitiesRook = movePossibilitiesRook(piece, board, len);
	for (i = 0; i < *len; i++) {
		possibilities[index] = possibilitiesRook[i];
		index++;
	}

	*sizeTabPossibilities += *len; //length min of possibilities
	return possibilities;
}

Cell* movePossibilitiesKing(Piece* piece, Board* board, int* sizeTabPossibilities) { //Moving possibilities of a king
	// return all the moving possibilities of a king in a Cell's table without verify check
	Cell* possibilities;
	possibilities = malloc(sizeof(Cell) * 8); //Max 8 possibilities
	Cell cell;
	int index = 0; //position in the table possibilities
	int isPieceCoord;

	//We go through all the cells around
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			cell.x = piece->x + i;
			cell.y = piece->y + j;
			isPieceCoord = piece->x == cell.x && piece->y == cell.y; //Verify if the cell is piece coord
			if (0 <= cell.x && cell.x < SIZE && 0 <= cell.y && cell.y < SIZE && !isPieceCoord)  { //Verify border & !isPieceCoord
				if (board->table[cell.x][cell.y] == NULL) { //Empty cell
					possibilities[index] = cell;
					index++;
				}
				else {
					if (board->table[cell.x][cell.y]->color != piece->color) { //Ennemy piece
						possibilities[index] = cell;
						index++;
					}
				}
			}
		}
	}
	//Castling
	if (!piece->hasMovedOnce) { //Verify if king hasn't moved
		
		//The height didn't change
		if (piece->color == WHITE) {
			cell.y = 0;
		} else {
			cell.y = 7;
		}
		//Verify if there is no piece between king & if rook hasn't moved
		if (board->table[0][cell.y] != NULL && board->table[7][cell.y] != NULL) {
			//Right
			if (board->table[5][cell.y] == NULL && board->table[6][cell.y] == NULL && board->table[7][cell.y]->type == ROOK && !board->table[7][cell.y]->hasMovedOnce) {
				cell.x = 7;
				possibilities[index] = cell;
				index++;
			}
			//Left
			if (board->table[3][cell.y] == NULL && board->table[2][cell.y] == NULL && board->table[1][cell.y] == NULL && board->table[0][cell.y]->type == ROOK && !board->table[0][cell.y]->hasMovedOnce) {
				cell.x = 0;
				possibilities[index] = cell;
				index++;
			}
		}
	}

	*sizeTabPossibilities = index; //length min of possibilities
	return possibilities;
}



/*
CHECK VERIFICATION
*/
Cell getKingPosition(Board* board, TypeColor color) {
	Cell king;
	for (int x = 0; x < board->size; x++) {
		for (int y = 0; y < board->size; y++) {
			if (board->table[x][y] != NULL) {
				if (board->table[x][y]->type == KING && board->table[x][y]->color == color) {
					king.x = x;
					king.y = y;
					return king;
				}
			}
		}
	}
	printf("Error getKingPosition : Your king might be in another castle (he's not in the board)\n");
}

int knightIsMenacing(Board* board, TypeColor color, Cell king) {
	int posThreatx, posThreaty;
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			if (x != 0 && y != 0) {
				// Computing the position of a potential knight
				posThreatx = king.x + x;
				posThreaty = king.y + y;

				// Checking if the position exists in the grid
				if (posThreatx >= 0 && posThreatx < 8 && posThreaty >= 0 && posThreaty < 8) {

					// checking if a knight actually is in the grid
					Piece* piece = board->table[posThreatx][posThreaty];
					if (piece != NULL && piece->type == KNIGHT && piece->color != color) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int verifyPieceLine(Piece* piece, int startingPos, int i, Cell king, TypeColor color) {
	if (piece != NULL) {

		if (i == startingPos && piece->type == KING && piece->color != color)
			return 1;

		else if ((piece->type == ROOK || piece->type == QUEEN) && piece->color != color)
			return 1;

		else return 2;
	}
	else return 0;
}

int rookOrQueenOrKingAreMenacing(Board* board, TypeColor color, Cell king) {
	Piece* piece;

	int i, status;
	for (i = king.y + 1; i < 8; i++) {
		piece = board->table[king.x][i];
		status = verifyPieceLine(piece, king.y + 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	for (i = king.y - 1; i >= 0; i--) {
		piece = board->table[king.x][i];
		status = verifyPieceLine(piece, king.y - 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	for (i = king.x + 1; i < 8; i++) {
		piece = board->table[i][king.y];
		status = verifyPieceLine(piece, king.x + 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	for (i = king.x - 1; i >= 0; i--) {
		piece = board->table[i][king.y];
		status = verifyPieceLine(piece, king.x - 1, i, king, color);

		if (status == 1)
			return 1;
		else if (status == 2)
			break;
	}

	return 0;
}

int bishopOrQueenOrKingAreMenacing(Board* board, TypeColor color, Cell king) {
	// return all the moving possibilities of a bishop in a Cell's table without verify check
	Cell cell;
	int right_left, top_bottom;

	for (int power = 1; power < 3; power++) { //Left and Right
		right_left = pow((-1), power); //right_left = -1 or 1, allow to go in left and right

		for (int power2 = 1; power2 < 3; power2++) { //Top and Bottom
			top_bottom = pow((-1), power2); //top_bottom = -1 or 1, allow to go in top and bottom

			//All cell in the line (7 cells max)
			//Didn't find a way to optimize the number of cell to be checked
			for (int i = 1; i < SIZE - 1; i++) {
				//define the coords of the cell
				cell.x = king.x + i * top_bottom;
				cell.y = king.y + i * right_left;

				if (0 <= cell.x && cell.x < SIZE && 0 <= cell.y && cell.y < SIZE) { //Verify border
					Piece* piece = board->table[cell.x][cell.y];

					if (piece != NULL) { // Non-empty cell
						if ((piece->type == BISHOP || piece->type == QUEEN) && piece->color != color) //Ennemy piece
							return 1;

						else if (piece->type == KING && piece->color != color && i == 1)
							return 1;

						break;
					}
				}
				else {
					break; //There's no point in checking any further
				}
			}
		}
	}

	return 0;
}

int pawnMenacing(Board* board, TypeColor color, Cell king) {
	// return all the moving possibilities of a pawn in a Cell's table without verify check

	int index = 0; //position in the table possibilities
	Cell cell;
	int mult = 1; //Initialize to white player

	if (color == BLACK) //Black player plays in negative move
		mult = -1;

	if (0 <= king.y + 1 * mult && king.y + 1 * mult < SIZE) { //Verify border
		//Attacks move
		int add;
		for (int power = 1; power < 3; power++) { //Left and Right
			add = pow((-1), power); //add = 1 or -1
			
			if (king.x + add * mult >= 0 && king.x + add * mult < SIZE) { //Verify border
				Piece* piece = board->table[king.x + add * mult][king.y + 1 * mult];

				if (piece != NULL) { //Not empty cell
					if (piece->color != color) { //ennemy piece
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

int isCheck(Board* board, TypeColor color) {
	Cell king = getKingPosition(board, color);

	return knightIsMenacing(board, color, king) || 
		rookOrQueenOrKingAreMenacing(board, color, king) || 
		bishopOrQueenOrKingAreMenacing(board, color, king) || 
		pawnMenacing(board, color, king);
}

