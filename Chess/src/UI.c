#include "UI.h"

void drawSidePanel(Window* window, SidePanel* panel) {
	setDrawColor(window, 20, 20, 20, 255);
	Rect rect = { panel->offsetX, 0, panel->width, window->height, 0.0f };
	drawRect(window, &rect);

	setDrawColor(window, 220, 220, 220, 255);
	drawLine(window, 3, panel->offsetX, 0, panel->offsetX, window->height);

	SDL_Color color = { 220, 220, 220, 255 };
	char* text = panel->whoPlays == WHITE ? "White plays" : "Black plays";
	int textOffset = panel->offsetX + 16; // TODO: better offset calculation
	drawText(window, color, text, textOffset, window->height - 60, 0.3f);
}

void drawEndScreen(Window* window, EndScreen* endScreen) {
	Rect rect = { window->width / 2 - endScreen->width / 2, window->height / 2 - endScreen->height / 2, endScreen->width, endScreen->height, 0.0f };
	setDrawColor(window, 20, 20, 20, 255);
	drawRect(window, &rect);
}