#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "TextNode.hpp"
#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"
#include "DataTables.hpp"
#include "Entity.hpp"
#include "Utility.hpp"
#include "Projectile.hpp"

namespace
{
	const std::vector<AircraftData> Table = initializeAircraftData();
}

//Intern function for clarity puproses
Textures::ID toTextureID(Aircraft::Type type)
{
	switch (type)
	{
		case Aircraft::Eagle:
			return Textures::Eagle;
		case Aircraft::Raptor:
			return Textures::Raptor;
		case Aircraft::Avenger:
			return Textures::Avenger;
	}
};

Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
: mType(type)
, mSprite(textures.get(toTextureID(type)))
, Entity(Table[type].hitpoints)
, mDirectionIndex(0)
, mFireRateLevel(1)
, mSpreadLevel(1)
, mMissileAmmo(2)
, mIsFiring(false)
, mIsLaunchingMissile(false)
, mFireCountdown(sf::Time::Zero)
, mTravelledDistance(0.f)
, mFireCommand()
, mMissileCommand()
, mHealthDisplay(nullptr)
{
	//Center coordinates
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	mFireCommand.category = Category::SceneAirLayer;
	mFireCommand.action =
		[this, &textures] (SceneNode& node, sf::Time)
		{
			createBullets(node, textures);
		};
	mMissileCommand.category = Category::SceneAirLayer;
	mMissileCommand.action =
		[this, &textures] (SceneNode& node, sf::Time)
		{
			createProjectile(node, Projectile::Missile, 0.f, 0.5f,
					textures);
		};
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Aircraft::getCategory() const
{
	switch (mType)
	{
		case Eagle:
			return Category::PlayerAircraft;
		default:
			return Category::EnemyAircraft;
	}
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{

	checkProjectileLaunch(dt, commands);
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

	mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		float distanceToTravel = directions[mDirectionIndex].distance;
		if (mTravelledDistance > distanceToTravel)
		{
			//FIXME Can fly further than should have
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);
		setVelocity(vx, vy);
		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

float Aircraft::getMaxSpeed() {
	return Table[mType].speed;
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (!isAllied())
		fire();

	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
	}
	if (mIsLaunchingMissile)
	{
		commands.push(mMissileCommand);
		mIsLaunchingMissile = false;
	}
}

void Aircraft::createBullets(SceneNode& node, const TextureHolder&
		textures) const
{
	Projectile::Type type = isAllied()
		? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch (mSpreadLevel)
	{
		case 1:
			createProjectile(node, type, 0.0f, 0.5f, textures);
			break;
		case 2:
			createProjectile(node, type, -0.33f, 0.33f, textures);
			createProjectile(node, type, +0.33f, 0.33f, textures);
			break;
		case 3:
			createProjectile(node, type, -0.5f, 0.33f, textures);
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.33f, textures);
			break;
	}
}

void Aircraft::createProjectile(SceneNode& node,
		Projectile::Type type, float xOffset, float yOffset,
		const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(
			new Projectile(type, textures));
	sf::Vector2f offset(
			xOffset * mSprite.getGlobalBounds().width,
			yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());
	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

void Aircraft::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (Table[mType].fireInterval != sf::Time::Zero) {
		mIsFiring = true;
	}
}

bool Aircraft::isAllied() const
{
	return mType == Eagle;
}

void Aircraft::launchMissile()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchingMissile = true;
		--mMissileAmmo;
	}
}

void Aircraft::collectMissiles(int nbMissiles)
{
	mMissileAmmo += nbMissiles;
}

void Aircraft::increaseSpread()
{
	mSpreadLevel++;
}

void Aircraft::increaseFireRate()
{
	mFireRateLevel++;
}

sf::FloatRect Aircraft::getBoundingRect() const
{
	return getWorldTransform()
		.transformRect(mSprite.getGlobalBounds());
}
