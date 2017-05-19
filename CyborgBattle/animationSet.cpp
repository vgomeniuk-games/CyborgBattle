#include <iostream>
#include <sstream>
#include "animationSet.h"
#include "cleanup.h"
#include "globals.h"
#include "assetPath.h"
#include "drawing.h"

AnimationSet::~AnimationSet() {
	cleanup(spriteSheet);
	if (whiteSpriteSheet != nullptr) {
		cleanup(whiteSpriteSheet);
	}
}

Animation* AnimationSet::getAnimation(std::string name) {
	for (Animation& animation : animations) {
		if (animation.getName() == name) {
			return &animation;
		}
	}
	return nullptr;
}

void AnimationSet::loadAnimationSet(std::string fileName, std::list<DataGroupType>& groupTypes, bool setColorKey, int transparentPixelIndex, bool createWhiteTexture) {

	std::ifstream file;
	std::string resPath = getResourcePath();
	file.open(resPath + fileName);
	if (file.good())
	{
		std::getline(file, imageName);  // Read image to read
		if (setColorKey) {
			SDL_Surface* spriteSurface = loadSurface(resPath + imageName, Globals::renderer);

			// For transparency, we will grab the [transparentPixelIndex] from the surface we just made
			SDL_Color* transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
			SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

			spriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false);

			// Create whiteTexture if required
			if (createWhiteTexture) {
				SDL_Surface* whiteSurface = loadSurface(resPath + "allwhite.png", Globals::renderer);
				surfacePaletteSwap(spriteSurface, whiteSurface);
				SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
				whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false); //create the texture whilst destroying the surface
				cleanup(whiteSurface);
			}
			else {
				whiteSpriteSheet = nullptr;
			}
			cleanup(spriteSurface);
		}
		else {
			// Simply load image
			spriteSheet = loadTexture(resPath + imageName, Globals::renderer);
		}
		// Temporary buffers
		std::string buffer;
		std::stringstream ss;

		// Get number of animations
		std::getline(file, buffer);
		buffer = Globals::clipOffDataHeader(buffer);
		ss << buffer;
		int numberOfAnimations;
		ss >> numberOfAnimations;

		// Create each animation
		for (int i = 0; i < numberOfAnimations; i++)
		{
			Animation newAnimation;
			newAnimation.loadAnimation(file, groupTypes);
			animations.push_back(newAnimation);
		}

	}
	file.close();
}

