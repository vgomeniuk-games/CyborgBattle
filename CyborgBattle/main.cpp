#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "cleanup.h"
#include "assetPath.h"
#include "drawing.h"
#include "globals.h"
#include "game.h"





int main(int argc, char **argv)
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error initialising SDL" << std::endl;
		return 1;
	}

	// Setup window
	SDL_Window* window = SDL_CreateWindow("Cyborg Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Globals::WIDTH * Globals::SCALE, Globals::HEIGHT * Globals::SCALE, SDL_WINDOW_SHOWN /* SDL_WINDOW_FULLSCREEN */
	);
	if (window == nullptr) {
		SDL_Quit();
		std::cout << "Window error" << std::endl;
		return 1;
	}

	// Setup renderer
	Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globals::renderer == nullptr) {
		cleanup(window);
		SDL_Quit();
		std::cout << "Renderer error" << std::endl;
		return 1;
	}

	// Renderer size before scaling to the screen size dimensions if created window
	SDL_RenderSetLogicalSize(Globals::renderer, Globals::WIDTH, Globals::HEIGHT);

	// Initialise SDL_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		cleanup(Globals::renderer);
		cleanup(window);
		SDL_Quit();
		std::cout << "SDL_image not initialised" << std::endl;
		return 1;
	}
	
	// Initialise text to font
	if (TTF_Init() != 0) {
		cleanup(Globals::renderer);
		cleanup(window);
		SDL_Quit();
		std::cout << "SDL_ttf not initialised" << std::endl;
		return 1;
	}

	// Initialise SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		cleanup(Globals::renderer);
		cleanup(window);
		SDL_Quit();
		std::cout << "SDL_mixer not initialised" << std::endl;
		return 1;
	}

	// Instantiate game object and update it constantly
	Game game;
	game.update();

	// Once game is over clean everything up and quit
	cleanup(Globals::renderer);
	cleanup(window);
	SDL_Quit();
	return 0;
}
