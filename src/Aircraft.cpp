#include <SFML/Graphics.hpp>
#include <cmath>

#include "TextNode.hpp"
#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"
#include "DataTables.hpp"
#include "Entity.hpp"
#include "Utility.hpp"

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
{
	//Center coordinates
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));
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

	Entity::updateCurrent(dt, commands);
	updateMovementPattern(dt);
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
