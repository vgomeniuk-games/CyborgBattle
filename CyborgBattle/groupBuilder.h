#ifndef GROUPBUILDER
#define GROUPBUILDER

#include <list>
#include <string>
#include <iostream>
#include "globals.h"
#include "group.h"
#include "groupBox.h"
#include "groupPosition.h"
#include "groupNumber.h"
#include "groupString.h"
#include "dataGroupType.h"

class GroupBuilder
{
public:
	/* NOTE: This is the only one a user has to set. As it depends on if they have their data output in groups or not
	 * In groups example:
	 * hitboxes: 2
	 * 12 12 34 56
	 * 24 45 57 79
	 * Not In Groups: (DEFAULT CHOICE)
	 * hitboxes: 12 12 34 56
	 * hitboxes: 24 45 57 79
	 * If in doubt, check the fdset and see what happens with the data
	 */
	static const bool savedInGroups; //TODO - Change based on above explaination

	// Builds a Group based on dataType. default is STRING (because we may want to ignore some data in a set, but still load it)
	static Group* buildGroup(DataGroupType dataType);

	// Build a set of Groups based on dataGroupTypes list
	static void buildGroups(const std::list<DataGroupType>& groupTypes, std::list<Group*>& groups);

	// Add GroupString to groups (build on the fly)
	static Group* addGroupStringToGroup(std::string name, std::list<Group*>& groups);

	// Loads a group out to a file
	static void loadGroups(std::ifstream& file, std::list<Group*>& groups);

	// Finds a group based on name
	static Group* findGroupByName(std::string str, std::list<Group*>& groups);
};

#endif