#ifndef ANIMATIONSET_H
#define ANIMATIONSET_H

#include <list>
#include <string>
#include <SDL.h>
#include "animation.h"
#include "dataGroupType.h"

class AnimationSet {
public:
	AnimationSet() {};
	~AnimationSet();
	Animation* getAnimation(std::string name);

	/* loads a fdset file, also takes a list of what types of data we expect to loading.
	 * Also, if we're working with an 8bit image and we know in that 8bit image what coloured pixel in the palette 
	 * we would prefer to be transparent, then we can say yes to 'colorKeying'(make a colour in the palette actually equal full transparent)
	 * and then just use the index of that transparentPixel (e.g, if its the third colour in the palette, then put in 2 as index starts at 0)
	 * if you need an alternative white version of the sprite sheet, then make this last option true (maybe move this out of this class? not sure)
	*/
	void loadAnimationSet(std::string fileName, std::list<DataGroupType>& groupTypes, bool setColorKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
	SDL_Texture* getSpriteSheet();
private:
	std::string imageName;
	SDL_Texture* spriteSheet = nullptr;
	SDL_Texture* whiteSpriteSheet = nullptr;  // Used to show damage
	std::list<Animation> animations;

};

#endif // !ANIMATIONSET_H