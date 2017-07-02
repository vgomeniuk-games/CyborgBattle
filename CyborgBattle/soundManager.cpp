#include "soundManager.h"

SoundManager SoundManager::manager;

SoundManager::SoundManager () {}

SoundManager::~SoundManager() {
	for (auto& kv : SoundManager::manager.sounds) {
		Mix_FreeChunk(kv.second.sound);
	}
}

void SoundManager::load(std::string name, std::string file) {
	SoundListing listing = { name, Mix_LoadWAV(file.c_str()) };
	SoundManager::manager.sounds[name] = listing;
};

void SoundManager::play(std::string name) {
	Mix_PlayChannel(-1, SoundManager::manager.sounds[name].sound, 0);
}
