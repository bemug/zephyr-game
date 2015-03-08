#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "ResourceIdentifiers.hpp"

#include <vector>

struct AircraftData
{
	int hitpoints;
	float speed;
	Textures::ID texture;
};

std::vector<AircraftData> initializeAircraftData();

#endif
