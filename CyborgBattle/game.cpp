#include <string>
#include "game.h"
#include "assetPath.h"
#include "drawing.h"
#include "globals.h"
#include "dataGroupType.h"
#include "wall.h"
#include "cleanup.h"
#include "timeController.h"
#include "randomNumber.h"

Game::Game() {
	std::string resPath = getResourcePath("assets");
	background = loadTexture(resPath + "map.png", Globals::renderer);

	// Hold list of data group types
	std::list<DataGroupType> d_types = {
		DataGroupType(DataType::Box, "collisionBox", false),
		DataGroupType(DataType::Box, "hitBox", false),
		DataGroupType(DataType::Number, "damage", false)
	};

	// Load animations
	heroAnimations = std::make_unique<AnimationSet*>(new AnimationSet());
	(*heroAnimations)->loadAnimationSet("cyborg.fdset", d_types, true, 0, true);
	wallAnimations = std::make_unique<AnimationSet*>(new AnimationSet());
	(*wallAnimations)->loadAnimationSet("wall.fdset", d_types);
	globAnimations = std::make_unique<AnimationSet*>(new AnimationSet());
	(*globAnimations)->loadAnimationSet("glob.fdset", d_types, true, 0, true);
	

	// Create a hero and manage it with keyboard
	hero = std::make_unique<Hero*>(new Hero(
		/*animations=*/ *heroAnimations,
		/*x=*/ Globals::WIDTH / 2,
		/*y=*/ Globals::HEIGHT / 2));
	heroInput.setHero(*hero);
	Entity::add(*hero);

	// Build walls (each 32x32)
	int tileSize = 32;
	for (int i = 0; i < Globals::WIDTH / tileSize; ++i) {
		// Top
		Entity* wall = new Wall(
			/*animations=*/ *wallAnimations,
			/*x=*/ i * tileSize + tileSize / 2,
			/*y=*/ tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);

		// Bottom
		wall = new Wall(
			/*animations=*/ *wallAnimations,
			/*x=*/ i * tileSize + tileSize / 2,
			/*y=*/ Globals::HEIGHT - tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);
	}
	for (int i = 1; i < Globals::HEIGHT / tileSize - 1; ++i) {
		// Left
		auto wall = new Wall(
			/*animations=*/ *wallAnimations,
			/*x=*/ tileSize / 2,
			/*y=*/ i * tileSize + tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);

		// Right
		wall = new Wall(
			/*animations=*/ *wallAnimations,
			/*x=*/ Globals::WIDTH - tileSize / 2,
			/*y=*/ i * tileSize + tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);
	}
}

Game::~Game() {
	cleanup(background);
	Entity::remove(true, true);  // Note: will also remove all the walls' / enemies' *s
}

void Game::update() {
	// TODO Spawn enemies
	int enemiesQty = 0;
	int enemiesMaxQty = 2;
	float enemySpawnTimer = 1.0f;

	bool quit = false;

	SDL_Event e;
	// Setup TimeController bewfore the game starts
	TimeController::controller.reset();

	// Main loop
	while (!quit) {
		TimeController::controller.updateTime();
		Entity::remove();  // Remove inactive events
		while (SDL_PollEvent(&e)) {
			// Exit
			if (e.type == SDL_QUIT) { quit = true; }

			// Keydown
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					(*hero)->revive();
					break;

				default:
					break;
				}
			}
			heroInput.update(&e);
		}
		// Update all the entities in the game
		Entity::updateAll();

		// Spawn enemies
		if ((*hero)->getHP() > 0) {
			if (enemiesQty == enemiesMaxQty) {
				enemiesMaxQty = enemiesMaxQty * 2;
				enemiesQty = 0;
				enemySpawnTimer = 4;
			}
			enemySpawnTimer -= TimeController::controller.getDeltaTime();
			if (enemySpawnTimer <= 0 && enemiesQty < enemiesMaxQty && enemies.size() < 1) {
				Glob* enemy = new Glob(
					/*animations=*/ *globAnimations,
					// Random position within accessible area
					/*x=*/ getRandomNumber(Globals::WIDTH - 96) + 64,
					/*y=*/ getRandomNumber(Globals::HEIGHT - 96) + 64,
					/*invincible=*/ 0.1
				);
				enemies.push_back(enemy);
				Entity::add(enemy);
			}
		}

		// Draw the game
		draw();
	}
}

void Game::draw() {
	// Clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	// Sort entities based on Y to properly display depth
	Entity::sort();

	// Render background & all the entities
	renderTexture(background, Globals::renderer, 0, 0);
	Entity::drawAll();

	// Display rendered image
	SDL_RenderPresent(Globals::renderer);
}


