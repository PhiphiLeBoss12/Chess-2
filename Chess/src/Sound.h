#pragma once
#include <SDL2/SDL_mixer.h>

Mix_Chunk* loadSound(const char* path);
void destroySound(Mix_Chunk* sound);
void playSound(Mix_Chunk* sound);

Mix_Music* loadMusic(const char* path);
Mix_Music* destroyMusic(Mix_Music* music);
void playMusic(Mix_Music* music);
void stopMusic(Mix_Music* music);