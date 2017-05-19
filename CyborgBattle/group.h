#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <SDL.h>
#include "dataGroupType.h"

#ifndef GROUP_H
#define GROUP_H

// ABSTRACT CLASS
// Groups manage a frames data
class Group{
public:
	Group() {}
	Group(DataGroupType type){ this->type = type; }

	// How many bits of data
	virtual size_t numberOfDataInGroup() const = 0;

	// Add to group using string. Converts string into correct data for each group type
	virtual void addToGroup(std::string str) = 0;

	virtual void draw() = 0;
	inline DataGroupType getType() { return type; }

protected:
	DataGroupType type; // Describes its name, type and other rules

};

#endif