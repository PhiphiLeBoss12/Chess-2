#include "UI.h"

void drawSidePanel(Window* window, SidePanel* panel) {
	SDL_Color color = { 255, 255, 255, 255 };
	char* text = panel->whoPlays == WHITE ? "White plays" : "Black plays";
	drawText(window, color, text, panel->offsetX, 0, 0.3f);
}