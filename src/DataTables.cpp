#include <iostream>

#include "DataTables.hpp"

#include "Aircraft.hpp"
#include "Pickup.hpp"

#include <vector>

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<AircraftData> initializeAircraftData()
{
	std::vector<AircraftData> data(Aircraft::TypeCount);

	data[Aircraft::Eagle].hitpoints = 100;
	data[Aircraft::Eagle].speed = 200.f;
	data[Aircraft::Eagle].texture = Textures::Eagle;
	data[Aircraft::Eagle].fireInterval = sf::seconds(1);

	data[Aircraft::Raptor].hitpoints = 20;
	data[Aircraft::Raptor].speed = 80.f;
	data[Aircraft::Raptor].texture = Textures::Raptor;
	data[Aircraft::Raptor].fireInterval = sf::seconds(0);

	data[Aircraft::Raptor].directions.push_back(Direction( 45, 80));
	data[Aircraft::Raptor].directions.push_back(Direction(-45, 160));
	data[Aircraft::Raptor].directions.push_back(Direction( 45, 80));

	data[Aircraft::Avenger].hitpoints = 20;
	data[Aircraft::Avenger].speed = 120.f;
	data[Aircraft::Avenger].texture = Textures::Avenger;
	data[Aircraft::Avenger].fireInterval = sf::seconds(2);

	data[Aircraft::Avenger].directions.push_back(Direction(+45, 50));
	data[Aircraft::Avenger].directions.push_back(Direction( 0, 50));
	data[Aircraft::Avenger].directions.push_back(Direction(-45, 100));
	data[Aircraft::Avenger].directions.push_back(Direction( 0, 50));
	data[Aircraft::Avenger].directions.push_back(Direction(+45, 50));

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{

	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::BlueLaser;

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::GreenLaser;

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Textures::Missile;

	return data;
}

Direction::Direction(float angle, float distance)
: angle(angle)
, distance(distance)
{
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	data[Pickup::HealthRefill].texture = Textures::HealthRefill;
	data[Pickup::HealthRefill].action
		= std::bind(&Aircraft::repair, _1, 25);
	data[Pickup::MissileRefill].texture = Textures::MissileRefill;
	data[Pickup::MissileRefill].action
		= std::bind(&Aircraft::collectMissiles, _1, 3);
	data[Pickup::FireSpread].texture = Textures::FireSpread;
	data[Pickup::FireSpread].action
		= std::bind(&Aircraft::increaseSpread, _1);
	data[Pickup::FireRate].texture = Textures::FireRate;
	data[Pickup::FireRate].action
		= std::bind(&Aircraft::increaseFireRate, _1);
	return data;
}

std::vector<ParticleData> initializeParticleData() {
	std::cout << "TODO initialize particle data" << std::endl;
}
