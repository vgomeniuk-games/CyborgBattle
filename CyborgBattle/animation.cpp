#include <sstream>
#include "animation.h"
#include "globals.h"

int Animation::getNextFrameNumber(int frameNumber) {
	if (frameNumber + 1 < frames.size()) {
		return frameNumber + 1;
	}
	return 0;
}

Frame* Animation::getNextFrame(Frame* frame) {
	return getFrame(getNextFrameNumber(frame->getFrameNumber()));
}

size_t Animation::getEndFrameNumber() {
	return frames.size() - 1;
}

Frame* Animation::getFrame(int frameNumber) {
	if (frames.size() == 0) {
		return nullptr;
	}
	std::list<Frame>::iterator it = frames.begin();
	while (frameNumber)
	{
		++it;
		--frameNumber;
	}
	return &(*it);
}

std::string& Animation::getName() {
	return name;
}

void Animation::loadAnimation(std::ifstream& file, std::list<DataGroupType>& groupTypes) {
	// Temporary buffers
	std::string buffer;
	std::stringstream ss;

	// Load animation name
	std::getline(file, name);

	// Get number of frames
	std::getline(file, buffer);
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	int numberOfFrames;
	ss >> numberOfFrames;

	// Upload all frames for current animation
	for (int i = 0; i < numberOfFrames; ++i) {
		Frame newFrame;
		newFrame.loadFrame(file, groupTypes);
		frames.push_back(newFrame);
	}
}
