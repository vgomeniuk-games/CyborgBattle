#include "groupBuilder.h"

const bool GroupBuilder::savedInGroups = false;//TODO change this if fdset has saveInGroups selected

Group* GroupBuilder::buildGroup(DataGroupType dataType)
{
	Group* group = nullptr;

	switch (dataType.getGroupType())
	{
	case DataType::Number:
		group = new GroupNumber(dataType);
		break;
	case DataType::Position:
		group = new GroupPosition(dataType);
		break;
	case DataType::Box:
		group = new GroupBox(dataType);
		break;
	case DataType::String:
		group = new GroupString(dataType);
		break;
	}
	return group;
}

void GroupBuilder::buildGroups(const std::list<DataGroupType>& groupTypes, std::list<Group*>& groups){
	for (DataGroupType dgt : groupTypes) {
		Group *group = buildGroup(dgt);
		groups.push_back(group);
	}
}

Group* GroupBuilder::addGroupStringToGroup(std::string name, std::list<Group*>& groups){
	DataGroupType dgt(DataType::String, name, false);
	Group *group = new GroupString(dgt);
	groups.push_back(group); //ok, we just added some random group on the fly for this frame only. cool, I hope
	return group;
}
void GroupBuilder::loadGroups(std::ifstream& file, std::list<Group*>& groups) {
	// Read each group in file:
	while (!file.eof()) {

		// First, get the next line out of the file
		std::streamoff positionBeforeRead = file.tellg();
		std::string line;
		std::getline(file, line);
		if (line.empty() || line == "") {
			break; //must actually be at the end of the file
		}
		if (savedInGroups) {
			// Find the semi colon
			size_t pos = line.find(":", 0);
			if (pos == -1) {
				/* Must actually be reading something else, like the next animation or something. TODO how to deal with this?
				 * Somehow backtrack to the start of this line, so the rest of the reading can happen for other animations */
				file.seekg(positionBeforeRead);
				break;
			}
			// Get group
			Group *group = findGroupByName(line.substr(0, pos), groups);
			if (group == nullptr) {
				// Can't find the group, but try not to lose the data
				group = addGroupStringToGroup(line.substr(0, pos), groups);
			}
			// Get the rest of the string and work out how many of these we have to build
			std::string numStr = line.substr(pos + 1, line.length() - pos + 2);
			std::stringstream ss;
			ss << numStr << std::endl;
			int num;
			ss >> num;

			// Finally can read in all the data
			for (int i = 0; i < num; i++) {
				if (!file.good()) {
					break;
				}
				std::getline(file, line);
				group->addToGroup(line);
			}
				 
		}
		else {
			// Dealing with 1 item at a time, regardless of in a group or not
			// Find the semi colon
			size_t pos = line.find(":", 0);
			if (pos == -1) {
				/* Must actually be reading something else, like the next animation or something. TODO how to deal with this?
				* Somehow backtrack to the start of this line, so the rest of the reading can happen for other animations */
				file.seekg(positionBeforeRead);
				break;
			}
			// Get group
			std::cout << "sub: " << line.substr(0, pos) << std::endl;
			Group *group = findGroupByName(line.substr(0, pos), groups);
			if (group == nullptr) {
				// Can't find the group, but try not to lose the data
				group = addGroupStringToGroup(line.substr(0, pos), groups);
			}
			// Clean up string so we can add data to the group
			// TODO
			// line = Globals::clipOffDataHeader(line);//get rid of the 'groupName: '
			// group->addToGroup(line);
		}
	}

}

Group* GroupBuilder::findGroupByName(std::string str, std::list<Group*> &groups) {
	for (Group* group : groups) {
		if (group->getType().getGroupName() == str) {
			return group;
		}
	}
	return nullptr;
}