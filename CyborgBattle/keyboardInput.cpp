#include <map>
#include <vector>
#include <SDL_scancode.h>
#include "keyboardInput.h"

std::map<std::vector<SDL_Scancode>, int> angles{
	{ { SDL_SCANCODE_UP, SDL_SCANCODE_LEFT } , 180 },
	{ { SDL_SCANCODE_UP, SDL_SCANCODE_RIGHT } , 0 },
	{ { SDL_SCANCODE_UP } , 0 },

	{ { SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT } , 180 },
	{ { SDL_SCANCODE_DOWN, SDL_SCANCODE_RIGHT } , 0 },
	{ { SDL_SCANCODE_DOWN } , 0 },


};

void KeyboardInput::setHero(Hero* hero) { this->hero = hero; };

void KeyboardInput::update(SDL_Event* e) {
	// Press
	if (e->type == SDL_KEYDOWN) {
		switch (e->key.keysym.scancode)
		{
		case SDL_Scancode::SDL_SCANCODE_X:
			hero->dash();
			break;
		case SDL_Scancode::SDL_SCANCODE_Z:
			hero->slash();
			break;
		default:
			break;
		}
	}
	// Hold keys
	const Uint8* keyboard = SDL_GetKeyboardState(nullptr);

	// Define if can move or idle
	bool canMove = hero->getState() == HeroState::Move;
	bool canIdle = hero->getState() == HeroState::Idle;

	// Define if any key is held
	bool k_pressed = false;
	for (SDL_Scancode key : {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT}) {
		if (keyboard[key]) { k_pressed = true; break; }
	}
	float angle;
	// Stop moving if hero is unable to
	if (!(canMove || canIdle) || !k_pressed) { hero->shouldMove(false); }

	// Define move angle otherwise
	else  if (keyboard[SDL_SCANCODE_UP] && !keyboard[SDL_SCANCODE_DOWN]) {
		angle = 280.0f + (keyboard[SDL_SCANCODE_LEFT] * -45.0f) + (keyboard[SDL_SCANCODE_RIGHT] * 45.0f);
		hero->move(angle);
	}
	else if (keyboard[SDL_SCANCODE_DOWN] && !keyboard[SDL_SCANCODE_UP]) {
		angle = 90.0f + (keyboard[SDL_SCANCODE_LEFT] * 45.0f) + (keyboard[SDL_SCANCODE_RIGHT] * -45.0f);
		hero->move(angle);
	}
	else if (!(keyboard[SDL_SCANCODE_LEFT] && keyboard[SDL_SCANCODE_RIGHT])) {
		angle = keyboard[SDL_SCANCODE_LEFT] ? 180.0f : 0.0f;
		hero->move(angle);
	}
}
