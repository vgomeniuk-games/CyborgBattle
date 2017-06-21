#ifndef GAME_H
#define GAME_H

#include <memory>
#include <list>
#include <SDL.h>
#include "entity.h"
#include "hero.h"
#include "animationSet.h"
#include "keyboardInput.h"

class Game {
public:
	Game();
	~Game();
	void update();
	void draw();

private:
	SDL_Texture* background;

	std::unique_ptr<Hero*> hero;
	std::unique_ptr<AnimationSet*> heroAnimations;
	KeyboardInput heroInput;

	std::list<Entity*> walls;  // Deleting handled by Entity::remove
	std::unique_ptr<AnimationSet*> wallAnimations;

};

#endif // !GAME_H
