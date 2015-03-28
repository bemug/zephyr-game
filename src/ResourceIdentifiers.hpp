#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H

#include <SFML/Graphics.hpp>

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
}

namespace Textures
{
	enum ID
	{
		Eagle,
		Raptor,
		Avenger,
		GreenLaser,
		BlueLaser,
		Missile,
		Desert,
	};
}

namespace Fonts
{
	enum ID
	{
		Default,
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif // RESOURCE_IDENTIFIERS_H
