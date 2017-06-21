#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <SDL.h>
#include "hero.h"

class KeyboardInput {
public:
	void update(SDL_Event* e);
	void setHero(Hero* hero);
private:
	Hero* hero;

};

#endif // !KEYBOARDINPUT_H
