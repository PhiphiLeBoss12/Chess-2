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

Mix_Music* loadMusic(const char* path) {
	return Mix_LoadMUS(path);
}
Mix_Music* destroyMusic(Mix_Music* music) {
	Mix_FreeMusic(music);
}
void playMusic(Mix_Music* music) {
	Mix_PlayMusic(music, -1);
}
void stopMusic() {
	Mix_PauseMusic();
}
void resumeMusic() {
	Mix_ResumeMusic();
}