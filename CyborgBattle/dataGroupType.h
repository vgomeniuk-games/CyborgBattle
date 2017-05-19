#ifndef DATAGROUPTYPE_H
#define DATAGROUPTYPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


/* Possible data types */
enum DataType { String, Position, Box, Number };

// Frame data is all about the data. These types help to describe what types of data are in a group
class DataGroupType{
public:
	DataGroupType() {};
	DataGroupType(DataType type, std::string name, bool single) : dataType(type), groupName(name), singleItem(single) {};

	// Saving and loading these for RuleSets(RS)
	static void saveRSDataGroupType(std::ofstream& file, DataGroupType dataGroupType);
	static DataGroupType loadRSDataGroupType(std::ifstream& file);
	inline DataType getGroupType() { return dataType; };
	inline std::string getGroupName() { return groupName; };
	inline bool isSingleItem() { return singleItem; }
private:
	std::string groupName; // e.g hitBoxes
	DataType dataType; // What do we expect to see here e.g string, position, rectangle region, rotation, et
	bool singleItem; // Can this datagroup only have 1 item inside (usually false)

};

#endif