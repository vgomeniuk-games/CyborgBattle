#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <list>
#include <SDL.h>
#include "group.h"
#include "dataGroupType.h"


class Frame {
public:
	void draw(SDL_Texture* spriteSheet, float x, float y);
	void loadFrame(std::ifstream& file, std::list<DataGroupType>& groupTypes);
	int getFrameNumber() { return frameNumber; };
	std::list<Group*> getFrameData();
	inline SDL_Point getOffset() { return offSet; }\

private:
	int frameNumber;  // Frame index
	SDL_Rect clip;  // Region on spritesheet
	float duration;  // How long frame runs
	SDL_Point offSet;  // Pivot for frame allignment
	std::list<Group*> frameData;  // Groups of additional data

};

#endif // !FRAME_H
