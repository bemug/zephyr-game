#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "ResourceIdentifiers.hpp"

#include <vector>
struct Direction
{
	Direction(float angle, float distance);
	float angle;
	float distance;
};


struct AircraftData
{
	int hitpoints;
	float speed;
	Textures::ID texture;
	std::vector<Direction> directions;
};

std::vector<AircraftData> initializeAircraftData();

#endif
