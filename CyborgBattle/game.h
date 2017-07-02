#ifndef GAME_H
#define GAME_H

#include <memory>
#include <list>
#include <SDL.h>
#include "entity.h"
#include "hero.h"
#include "glob.h"
#include "animationSet.h"
#include "keyboardInput.h"

class Game {
public:
	Game();
	~Game();
	void restart(bool splash);
	void update();
	void draw();
	
private:
	// Additional textures
	SDL_Texture* background{ nullptr };
	SDL_Texture* splash{ nullptr };
	SDL_Texture* overlay{ nullptr };
	SDL_Texture* score{ nullptr };

	std::unique_ptr<Hero*> hero;
	std::unique_ptr<AnimationSet*> heroAnimations;
	KeyboardInput heroInput;

	std::list<Entity*> enemies;
	std::unique_ptr<AnimationSet*> globAnimations;


	std::list<Entity*> walls;  // Deleting handled by Entity::remove
	std::unique_ptr<AnimationSet*> wallAnimations;

	// Helper variables
	float overlayTimer;
	bool isSplash;
	int enemiesQty;
	int enemiesMaxQty;
	float enemySpawnTimer;
};

#endif // !GAME_H
