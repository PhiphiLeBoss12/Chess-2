﻿#include "UI.h"
#include <math.h>

void drawStartScreen(Window* window, SDL_Texture** textures) {
	static double iter = 0;

	Rect rect = { 0, 0, window->width, window->height, 0.0f };
	setDrawColor(window, 64, 64, 64, 255);
	drawRect(window, &rect);

	SDL_Color color = { 220, 220, 220, 255 };
	drawText(window, color, "chess 2", 20, 70, 1.0f);
	drawText(window, color, "the long awaited sequel", 200, sin(iter / 100) * 10 + 55, 0.2f);

	drawText(window, color, "press return to play", 30, 700, 0.3f);
	drawText(window, color, "press escape to quit :(", 30, 650, 0.3f);

	Rect texRect = { 800, 300, 256, 256, -iter / 5 };
	drawTexture(window, &texRect, textures[4]);
	Rect texRect1 = { 600, 600, 128, 128, iter };
	drawTexture(window, &texRect1, textures[9]);
	Rect texRect2 = { 100, 300, 128, 128, iter / 6 };
	drawTexture(window, &texRect2, textures[5]);
	Rect texRect3 = { cos(iter / 50) * 40 + 500, sin(iter / 50) * 40 + 176, 64, 64, -iter};
	drawTexture(window, &texRect3, textures[10]);
	Rect texRect4 = { sin(iter / 70) * 50 + 900, cos(iter / 70) * 50 + 100, 128, 128, iter};
	drawTexture(window, &texRect4, textures[1]);

	iter++;
}

void drawSidePanel(Window* window, SidePanel* panel) {
	setDrawColor(window, 20, 20, 20, 255);
	Rect rect = { panel->offsetX, 0, panel->width, window->height, 0.0f };
	drawRect(window, &rect);

	setDrawColor(window, 220, 220, 220, 255);
	drawLine(window, 3, panel->offsetX, 0, panel->offsetX, window->height);

	SDL_Color color = { 220, 220, 220, 255 };
	char* text = panel->whoPlays == WHITE ? "white plays" : "black plays";
	int textOffset = panel->offsetX + 16; // TODO: better offset calculation
	drawText(window, color, text, textOffset, window->height - 60, 0.3f);
}

void drawEndScreen(Window* window, EndScreen* endScreen) {
	int endScreenOriginX = window->width / 2 - endScreen->width / 2;
	int endScreenOriginY = window->height / 2 - endScreen->height / 2;

	Rect rect = { endScreenOriginX, endScreenOriginY, endScreen->width, endScreen->height, 0.0f };
	setDrawColor(window, 25, 25, 25, 255);
	drawRect(window, &rect);

	const char* whoWonText = endScreen->whoWon == WHITE ? "white won w(°o°)w" : "black won w(°o°)w";
	SDL_Color color = { 220, 220, 220, 255 };
	drawText(window, color, whoWonText, endScreenOriginX + endScreen->width / 2 - 175, endScreenOriginY + endScreen->height - 100, 0.3f);
	drawText(window, color, "press return to play again (good ending) (uwu)", endScreenOriginX + 20, endScreenOriginY + endScreen->height - 200, 0.2f);
	drawText(window, color, "press escape to quit (nooooo dont kill me) ~(> <)~", endScreenOriginX + 20, endScreenOriginY + endScreen->height - 300, 0.2f);
}