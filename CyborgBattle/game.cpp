#include <string>
#include "game.h"
#include "assetPath.h"
#include "drawing.h"
#include "globals.h"
#include "dataGroupType.h"
#include "wall.h"
#include "cleanup.h"
#include "timeController.h"

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

	// Create a hero and manage it with keyboard
	hero = std::make_unique<Hero*>(new Hero(*heroAnimations, Globals::WIDTH / 2, Globals::HEIGHT / 2));
	heroInput.setHero(*hero);
	Entity::add(*hero);

	// Build walls (each 32x32)
	int tileSize = 32;
	for (int i = 0; i < Globals::WIDTH / tileSize; ++i) {
		// Top
		Entity* wall = new Wall(*wallAnimations, i * tileSize + tileSize / 2, tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);

		// Bottom
		wall = new Wall(*wallAnimations, i * tileSize + tileSize / 2, Globals::HEIGHT - tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);
	}
	for (int i = 1; i < Globals::HEIGHT / tileSize - 1; ++i) {
		// Left
		auto wall = new Wall(*wallAnimations, tileSize / 2, i * tileSize + tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);

		// Right
		wall = new Wall(*wallAnimations, Globals::WIDTH - tileSize / 2, i * tileSize + tileSize / 2);
		walls.push_back(wall);
		Entity::add(wall);
	}
}

Game::~Game() {
	cleanup(background);
	Entity::remove(true, true);  // Note: will also remove all the walls' *s
}

void Game::update() {
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


