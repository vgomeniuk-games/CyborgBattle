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
#include "soundManager.h"


Game::Game() {
	// Load textures
	std::string resPath = getResourcePath("assets");
	background = loadTexture(resPath + "map.png", Globals::renderer);
	splash = loadTexture(resPath + "cyborgtitle.png", Globals::renderer);
	overlay = loadTexture(resPath + "overlay.png", Globals::renderer);

	// Load music & sounds
	std::string soundsPath = getResourcePath("assets", "sounds");
	// Song by Ryan Beveridge: https://soundcloud.com/ryan-beveridge/fatal-theory-1
	soundtrack = Mix_LoadMUS((soundsPath + "FatalTheory.wav").c_str());
	if (soundtrack != nullptr) {
		Mix_PlayMusic(soundtrack, -1);
	}
	for (auto& sound : { "heroHit", "enemyHit", "swing", "dash", "enemyDeath" }) {
		SoundManager::load(sound, soundsPath + sound + ".wav");
	}

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
	// Stop playing music and clean
	Mix_PauseMusic();
	Mix_FreeMusic(soundtrack);

	// Clean all the textures loaded
	for (auto texture : { background, splash, overlay, controlUI, hpUI, scoreUI }) { cleanup(texture); }
	Entity::remove(true, true);  // Note: will also remove all the walls' / enemies' *s
}

void Game::restart(bool splash) {
	// Reset helper variables
	overlayTimer = 2.0f;
	isSplash = splash;
	enemiesQty = 0;
	enemiesMaxQty = 2;
	enemySpawnTimer = 1.0f;

	// Remove all the enemies and clean killed score
	Glob::reset();
	for (auto enemy : enemies) { enemy->deactivate(); }
	enemies.clear();
}

void Game::update() {
	restart(true);

	SDL_Event e;
	// Setup TimeController bewfore the game starts
	TimeController::controller.reset();

	// Main loop
	bool quit = false;
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
					if (isSplash) { isSplash = false; }
					if (overlayTimer <= 0 && (*hero)->getHP() < 1) {
						restart(false);
						(*hero)->revive();
					}

					// TODO DEBUG
					restart(false);
					(*hero)->revive();
					break;
				default:
					break;
				}
			}
			heroInput.update(&e);
		}
		// Tick overlayTimer
		if ((*hero)->getHP() < 1 && overlayTimer > 0) {
			overlayTimer -= TimeController::controller.getDeltaTime();
		}

		// Update all the entities in the game
		Entity::updateAll();

		// Spawn enemies
		if ((*hero)->getHP() > 0 && !isSplash) {
			if (enemiesQty == enemiesMaxQty) {
				enemiesMaxQty = enemiesMaxQty * 2;
				enemiesQty = 0;
				enemySpawnTimer = 4;
			}
			enemySpawnTimer -= TimeController::controller.getDeltaTime();
			if (enemySpawnTimer <= 0 && enemiesQty < enemiesMaxQty && enemies.size() < 10) {
				Glob* enemy = new Glob(
					/*animations=*/ *globAnimations,
					// Random position within accessible area
					/*x=*/ getRandomNumber(Globals::WIDTH - 96) + 64,
					/*y=*/ getRandomNumber(Globals::HEIGHT - 96) + 64,
					/*invincible=*/ 0.1f
				);
				++enemiesQty;
				enemies.push_back(enemy);
				Entity::add(enemy);
			}
		}
		// Draw the game
		draw();
	}
}

void Game::drawUI(SDL_Texture* ui, std::initializer_list<std::string> text, int size, int posX, int posY, SDL_Color color) {
	// Generate text
	std::stringstream ss;
	for (auto& str : text) { ss << str; }

	// Draw ui and clean at the end
	std::string path = getResourcePath("assets", "fonts");
	if (ui == nullptr) {
		ui = renderText(ss.str(), path + "vermin_vibes_1989.ttf", color, size, Globals::renderer);
	}
	renderTexture(ui, Globals::renderer, posX, posY);
}

void Game::draw() {
	// Clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	if (isSplash) {
		renderTexture(splash, Globals::renderer, 0, 0);
		drawUI(controlUI, { "Control: ", "Z - dash / X - slash / Up, Left, Down, Right - move" }, 20, 50, Globals::HEIGHT - 20);
	}
	else {
		// Render background & all the entities
		renderTexture(background, Globals::renderer, 0, 0);
		Entity::sort();  // Sort entities based on Y to properly display depth
		Entity::drawAll();
		drawUI(hpUI, { "HP: ", std::to_string((*hero)->getHP()), " / ", std::to_string((*hero)->getMaxHP())}, 30, 20, 20);

		// Show overlay if dead
		if (overlayTimer <= 0 && (*hero)->getHP() < 1) {
			renderTexture(overlay, Globals::renderer, 0, 0);
			drawUI(scoreUI, { "Enemies dispatched: ", std::to_string(Glob::score()) }, 30, 20, 50);
		}
	}
	// Display rendered image
	SDL_RenderPresent(Globals::renderer);
}


