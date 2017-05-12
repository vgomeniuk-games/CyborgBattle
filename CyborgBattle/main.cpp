#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"


static int WIDTH = 640;
static int HEIGHT = 352;


int main(int argc, char **argv)
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error initialising SDL" << std::endl;
		return 1;
	}

	// Setup window
	SDL_Window* window = SDL_CreateWindow("Cyborg Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_SHOWN /* SDL_WINDOW_FULLSCREEN */
	);
	if (window == nullptr) {
		SDL_Quit();
		std::cout << "Window error" << std::endl;
		return 1;
	}

	// Setup renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		cleanup(window);
		SDL_Quit();
		std::cout << "Renderer error" << std::endl;
		return 1;
	}

	// Renderer size before scaling to the screen size dimensions if created window
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	// Initialise SDL_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		std::cout << "SDL_image not initialised" << std::endl;
		return 1;
	}
	
	// Initialise text to font
	if (TTF_Init() != 0) {
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		std::cout << "SDL_ttf not initialised" << std::endl;
		return 1;
	}

	// Initialise SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		std::cout << "SDL_mixer not initialised" << std::endl;
		return 1;
	}

	// Load texture to draw
	std::string resPath = getResourcePath("assets");
	SDL_Texture* texture = loadTexture(resPath + "map.png", renderer);

	// Run game for 5000 ticks (5000ms)
	while (SDL_GetTicks() < 5000) {
		SDL_RenderClear(renderer);
		renderTexture(texture, renderer, 0, 0);
		SDL_RenderPresent(renderer);
	}
	cleanup(renderer);
	cleanup(window);
	cleanup(texture);
	SDL_Quit();
	return 0;
}
