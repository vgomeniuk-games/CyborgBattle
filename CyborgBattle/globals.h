#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>
#include <SDL.h>

class Globals {
public:
	static const int WIDTH;
	static const int HEIGHT;
	static const int SCALE;
	static const float PI;
	static const bool DEBUG;
	static SDL_Renderer* renderer;

	// Clips off header, e.g. "collisionBox: 19 13 16 27" turns into "19 13 16 27"
	static std::string clipOffDataHeader(std::string data);
};

#endif // !GLOBALS_H
