﻿#include "UI.h"
#include <math.h>

void drawStartScreen(Window* window, SDL_Texture** textures) {
	static double iter = 0;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 15; j++) {
			Rect rect;
			rect.x = j * 100;
			rect.y = i * 100;
			rect.width = 100;
			rect.height = 100;
			rect.angle = 0.0f;

			if ((i + j) % 2 == 1)
				setDrawColor(window, 80, 80, 80, 255);
			else
				setDrawColor(window, 15, 15, 15, 255);

			drawRect(window, &rect);
		}
	}

	Rect texRect = { 800, 300, 256, 256, -iter / 5 };
	drawTexture(window, &texRect, textures[4]);
	Rect texRect1 = { 600, 600, 128, 128, iter };
	drawTexture(window, &texRect1, textures[9]);
	Rect texRect2 = { 100, 300, 128, 128, iter / 6 };
	drawTexture(window, &texRect2, textures[5]);
	Rect texRect3 = { cos(iter / 50) * 40 + 500, sin(iter / 50) * 40 + 176, 64, 64, iter / 2 };
	drawTexture(window, &texRect3, textures[10]);
	Rect texRect4 = { sin(iter / 70) * 50 + 900, cos(iter / 70) * 50 + 100, 128, 128, iter };
	drawTexture(window, &texRect4, textures[1]);

	SDL_Color color = { 220, 220, 220, 255 };
	drawText(window, color, "chess 2", 20, 70, 1.0f);
	drawText(window, color, "the long awaited sequel", 200, sin(iter / 100) * 10 + 55, 0.2f);

	drawText(window, color, "press return to play", 30, 700, 0.3f);
	drawText(window, color, "press escape to quit", 30, 650, 0.3f);
	drawText(window, color, "press c to see the credits", 30, 600, 0.3f);
	drawText(window, color, "press m to toggle music", 30, 550, 0.3f);

	if (window->keyDown == SDLK_c) {
		drawText(window, color, "supervisor: éric andrès", 450, 550 - 20, 0.2f);
		drawText(window, color, "development: julien", 450, 525 - 20, 0.2f);
		drawText(window, color, "development: lilian", 450, 500 - 20, 0.2f);
		drawText(window, color, "development: marie", 450, 475 - 20, 0.2f);
		drawText(window, color, "development: philéas", 450, 450 - 20, 0.2f);
		drawText(window, color, "development: tom", 450, 425 - 20, 0.2f);
		drawText(window, color, "graphics design: wikipedia", 450, 400 - 20, 0.2f);
		drawText(window, color, "original idea: Hán Xin", 450, 375 - 20, 0.2f);
		drawText(window, color, "\"Deadly Roulette\" \"Walking Along\" Kevin MacLeod (incompetech.com)", 450, 350 - 20, 0.12f);
		drawText(window, color, "Licensed under Creative Commons : By Attribution 4.0 License", 450, 335 - 20, 0.12f);
		drawText(window, color, "http ://creativecommons.org/licenses/by/4.0/", 450, 320 - 20, 0.12f);
	}

	iter++;
}

void drawSidePanel(Window* window, SidePanel* panel, SDL_Texture** textures) {
	setDrawColor(window, 50, 50, 50, 255);
	Rect rect = { panel->offsetX, 0, panel->width, window->height, 0.0f };
	drawRect(window, &rect);

	setDrawColor(window, 220, 220, 220, 255);
	drawLine(window, 3, panel->offsetX, 0, panel->offsetX, window->height);

	SDL_Color color = { 220, 220, 220, 255 };
	char* text = panel->whoPlays == WHITE ? "white plays" : "black plays";
	int textOffset = panel->offsetX + 16; // TODO: better offset calculation
	drawText(window, color, text, textOffset, window->height / 2, 0.3f);

	for (int i = 0; i < 16; i++) {
		if (!panel->playerWhite->eaten[i])
			continue;
		TypeColor color = panel->playerWhite->eaten[i]->color;
		TypePiece type = panel->playerWhite->eaten[i]->type;
		int index = type + 6 * color;
		Rect rect = { panel->offsetX + (64 * (i % 6)), 50 + (64 * (i / 6)), 64, 64, 0.0f};
		drawTexture(window, &rect, textures[type + 6 * color]);
	}

	for (int i = 0; i < 16; i++) {
		if (!panel->playerBlack->eaten[i])
			continue;
		TypeColor color = panel->playerBlack->eaten[i]->color;
		TypePiece type = panel->playerBlack->eaten[i]->type;
		int index = type + 6 * color;
		Rect rect = { panel->offsetX + (64 * (i % 6)), window->height - 100 - (64 * (i / 6)), 64, 64, 0.0f };
		drawTexture(window, &rect, textures[type + 6 * color]);
	}
}

void drawEndScreen(Window* window, EndScreen* endScreen) {
	int endScreenOriginX = window->width / 2 - endScreen->width / 2;
	int endScreenOriginY = window->height / 2 - endScreen->height / 2;

	Rect rect = { endScreenOriginX, endScreenOriginY, endScreen->width, endScreen->height, 0.0f };
	setDrawColor(window, 25, 25, 25, 255);
	drawRect(window, &rect);

	char* whoWonText;
	if (endScreen->whoWon == WHITE)
		whoWonText = "white won";
	if (endScreen->whoWon == BLACK)
		whoWonText = "black won";
	if (endScreen->whoWon == -1)
		whoWonText = "stalemate";
	SDL_Color color = { 220, 220, 220, 255 };
	drawText(window, color, whoWonText, endScreenOriginX + endScreen->width / 2 - 175, endScreenOriginY + endScreen->height - 100, 0.3f);
	drawText(window, color, "press return to play again", endScreenOriginX + 20, endScreenOriginY + endScreen->height - 200, 0.2f);
	drawText(window, color, "press escape to quit", endScreenOriginX + 20, endScreenOriginY + endScreen->height - 250, 0.2f);
}