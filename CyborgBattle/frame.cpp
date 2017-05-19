#include <sstream>
#include <string>
#include "frame.h"
#include "drawing.h"
#include "globals.h"
#include "groupBuilder.h"

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

void Frame::loadFrame(std::ifstream& file, std::list<DataGroupType>& groupTypes) {
	//Ok, we dragged these groupTypes all the way here, so we could construct groups on our frame to hold the data in
	GroupBuilder::buildGroups(groupTypes, frameData);

	// Helper buffers
	std::string buffer;
	std::stringstream ss;

	// frame & clip
	std::getline(file, buffer);
	std::getline(file, buffer);
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h;
	ss.clear();

	// offset/pivot
	std::getline(file, buffer);
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offSet.x >> offSet.y;
	ss.clear();

	// duration
	std::getline(file, buffer);
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;
	ss.clear();

	// index
	std::getline(file, buffer);
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	GroupBuilder::loadGroups(file, frameData);
}

