#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <string>
#include <map>
#include <SDL_mixer.h>

struct SoundListing {
	std::string name;
	Mix_Chunk* sound;
};


class SoundManager {
public:
	static void load(std::string name, std::string file);
	static void play(std::string name);

private:
	SoundManager();
	~SoundManager();

	static SoundManager manager;
	std::map<std::string, SoundListing> sounds;

};

#endif // !SOUNDMANAGER_H

