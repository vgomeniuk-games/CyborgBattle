#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <SDL.h>
#include "hero.h"

class KeyboardInput {
public:
	KeyboardInput();
	void update(SDL_Event* e);

private:
	Hero* hero;

};

#endif // !KEYBOARDINPUT_H
