#pragma once
#include "Types.h"
#include "Window.h"

typedef struct SidePanel {
	int width;
	int offsetX;
	TypeColor whoPlays;
} SidePanel;

typedef struct EndScreen {
	int width, height;
	TypeColor whoWon;
} EndScreen;

void drawStartScreen(Window* window);

void drawSidePanel(Window* window, SidePanel* panel);

void drawEndScreen(Window* window, EndScreen* endScreen);