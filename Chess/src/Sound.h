#pragma once
#include <SDL2/SDL_mixer.h>

Mix_Chunk* loadSound(const char* path);

void destroySound(Mix_Chunk* sound);

void playSound(Mix_Chunk* sound);