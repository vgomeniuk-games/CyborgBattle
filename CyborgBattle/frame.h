#ifndef FRAME_H
#define FRAME_H

#include <list>
#include <SDL.h>


class Frame {
	int frameNumber;  // Frame index
	SDL_Rect clip;  // Region on spritesheet
	float duration;  // How long frame runs
	SDL_Point offSet;  // Pivot for frame allignment
	std::list<void*> frameData;  // Groups of additional data

	void draw(SDL_Texture* spriteSheet, float x, float y);
	// TODO loadframe
};

#endif // !FRAME_H
