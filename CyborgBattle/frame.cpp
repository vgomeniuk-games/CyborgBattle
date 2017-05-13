#include "frame.h"
#include "drawing.h"
#include "globals.h"

void Frame::draw(SDL_Texture* spriteSheet, float x, float y) {
	/* Where to draw the frame */
	SDL_Rect dest{
		static_cast<int>(x - offSet.x),
		static_cast<int>(y- offSet.y),
		clip.w,
		clip.h
	};
	renderTexture(spriteSheet, Globals::renderer, dest, &clip);

}

