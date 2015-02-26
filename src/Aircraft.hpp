#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "ResourceHolder.hpp"

class Aircraft : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Raptor,
		};

	public:
		explicit Aircraft(Type type, const TextureHolder& textures);  //Explicit means the "type" paramter has to be a proper type (not implicitly converted)
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual unsigned int Aircraft::getCategory() const;

	private:
		Type mType;
		sf::Sprite mSprite;
};

#endif
