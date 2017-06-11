#ifndef GROUPNUMBER_H
#define GROUPNUMBER_H

#include <list>
#include <string>
#include <sstream>
#include "group.h"

class GroupNumber : public Group{
public:
	GroupNumber(DataGroupType type) { this->type = type; }
	inline size_t numberOfDataInGroup() const { return data.size(); }
	inline void addToGroup(std::string str){
		std::stringstream ss;
		ss << str;
		float r;
		ss >>r;
		data.push_back(r);
	}
	inline void draw() { /* TODO */ }
	inline float getFirst() { return data.front(); }
private:
	std::list<float> data;

};


#endif