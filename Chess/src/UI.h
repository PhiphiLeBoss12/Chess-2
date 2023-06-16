#pragma once
#include "Types.h"
#include "Window.h"

typedef struct SidePanel {
	int width;
	int offsetX;
	TypeColor whoPlays;
	Player* playerWhite;
	Player* playerBlack;
	int multiplayerState;
} SidePanel;

typedef struct EndScreen {
	int width, height;
	TypeColor whoWon;
} EndScreen;

void drawStartScreen(Window* window, SDL_Texture** textures);

void drawSidePanel(Window* window, SidePanel* panel, SDL_Texture** textures);

void drawEndScreen(Window* window, EndScreen* endScreen);