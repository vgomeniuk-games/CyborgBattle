#ifndef GROUPBOX_H
#define GROUPBOX_H

#include <list>
#include <string>
#include <sstream>
#include "group.h"

class GroupBox : public Group{
public:
	GroupBox(DataGroupType type) { this->type = type; }
	inline size_t numberOfDataInGroup() const { return data.size(); }
	inline void addToGroup(std::string str){
		// Add new group value
		std::stringstream ss;
		ss << str;
		SDL_Rect box;
		ss >> box.x >> box.y >> box.w >> box.h;
		data.push_back(box);
	}
	inline void draw(){ /* TODO */ }

private:
	std::list<SDL_Rect> data;

};


#endif