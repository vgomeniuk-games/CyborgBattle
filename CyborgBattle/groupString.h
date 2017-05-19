#ifndef GROUPSTRING_H
#define GROUPSTRING_H

#include <list>
#include <string>
#include <sstream>
#include "group.h"

class GroupString : public Group{
public:
	GroupString(DataGroupType type) { this->type = type; }
	inline size_t numberOfDataInGroup() const { return data.size(); }
	inline void addToGroup(std::string str){
		// Get rid of a whitespace at the front if exists
		if (str[0] == ' '){
			str.erase(0, 1);
		}
		data.push_back(str);
	}
	inline void draw() { /* TODO */ }

private:
	std::list<std::string> data;

};


#endif