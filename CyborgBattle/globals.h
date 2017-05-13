#ifndef GLOBALS_H
#define GLOBALS_H
#include <SDL.h>

class Globals {
public:
	static const int WIDTH;
	static const int HEIGHT;
	static const int SCALE;
	static const float PI;
	static const bool DEBUG;
	static SDL_Renderer* renderer;
};

#endif // !GLOBALS_H
