#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <string>
#include "frame.h"
#include "dataGroupType.h"

class Animation {
public:
	Animation(std::string name = "") : name(name) {};
	int getNextFrameNumber(int frameNumber);
	Frame* getNextFrame(Frame* frame);
	size_t getEndFrameNumber();
	Frame* getFrame(int frameNumber);
	void loadAnimation(std::ifstream& file, std::list<DataGroupType>& groupTypes);
	std::string& getName();
private:
	std::string name;
	std::list<Frame> frames;

};

#endif // !ANIMATION_H
