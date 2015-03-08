#include <SFML/Graphics.hpp>

#include "TextNode.hpp"
#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"
#include "DataTables.hpp"

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
	}
};

Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts):
	mType(type), mSprite(textures.get(toTextureID(type)))
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
	mHealthDisplay->setString("HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());
}
