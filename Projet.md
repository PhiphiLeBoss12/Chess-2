## CODE :
- creer plateau
- creer pieces
- placer les pieces dans le plateau
- initialiser les joueurs
- Jeu tant pas mat:
	- affichage
	- Repeter jusqu'a selection possibilité
		- input d'une piece
	 	- calcul possibilités + affichage
	- deplace la piece (si detruit piece adverse ou non)
	- verification du plateau (echec ou non)
- affichage du gagnant + menu
- free

## STRUCTURES:
- Board:
	- tableau des pieces (NULL si pas de pieces, *Piece[][])
- Player:
	- tableau de ses pieces (*Piece[])
	- sa couleur (enum)
	- en echec (enum : COOL | CHECK | CHECKMATE)
- Piece:
	- type (enum)
	- coord (int, int)
	- sa couleur (enum)
	- a bougé (bool)

## REPARTITION:
- affichage : Toto
- struture : 
- fonctions de deplacements :
- calcul des possibilités :
- verif plateau pour un joueur :

## FICHIERS:
- main.c
- types : def des structures
- piece : fonctions des pieces
- player : fonctions des players
- tableau : fonctions du tableau
- window : affichage SDL
- game : fonctions générales
	
