#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "TextNode.hpp"
#include "ResourceHolder.hpp"
#include "CommandQueue.hpp"

class Aircraft : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Raptor,
			TypeCount,
		};

	public:
		explicit Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual unsigned int getCategory() const;
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

	private:
		Type mType;
		sf::Sprite mSprite;
		TextNode* mHealthDisplay;
};

#endif
