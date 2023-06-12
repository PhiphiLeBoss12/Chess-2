#include "Sound.h"

Mix_Chunk* loadSound(const char* path) {
	return Mix_LoadWAV(path);
}

void destroySound(Mix_Chunk* sound) {
	Mix_FreeChunk(sound);
}

void playSound(Mix_Chunk* sound) {
	Mix_PlayChannel(-1, sound, 0);
}