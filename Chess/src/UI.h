#pragma once
#include "Types.h"
#include "Window.h"

typedef struct SidePanel {
	int offsetX;
	TypeColor whoPlays;
} SidePanel;

void drawSidePanel(Window* window, SidePanel* panel);