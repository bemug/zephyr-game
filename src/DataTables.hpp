#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "ResourceIdentifiers.hpp"
#include "Aircraft.hpp"

#include <vector>
#include <functional>

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
	sf::Time fireInterval;
};

struct ProjectileData
{
	Textures::ID texture;
	int speed;
	int damage;
};

struct PickupData
{
	Textures::ID texture;
	std::function<void(Aircraft&)> action;
};

std::vector<AircraftData> initializeAircraftData();
std::vector<ProjectileData> initializeProjectileData();

#endif
