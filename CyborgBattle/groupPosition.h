#ifndef GROUPPOS_H
#define GROUPPOS_H

#include <list>
#include <string>
#include <sstream>
#include "group.h"

class GroupPosition : public Group{
public:
	GroupPosition(DataGroupType type) { this->type = type; }
	inline size_t numberOfDataInGroup() const { return data.size(); }
	inline void addToGroup(std::string str){
		std::stringstream ss;
		ss << str;
		SDL_Point pt;
		ss >> pt.x >> pt.y ;
		data.push_back(pt);
	}
	inline void draw() { /* TODO */ }

private:
	std::list<SDL_Point> data;

};


#endif