#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <string>
#include <memory>

namespace Textures
{
	enum ID { Landscape, Airplane, Missile };
}

class TextureHolder
{
	public:
		void load(Textures::ID id, const std::string& filename);
		sf::Texture& get(Textures::ID id);
		const sf::Texture& get(Textures::ID id) const;
	private:
		std::map<Textures::ID,
			std::unique_ptr<sf::Texture>> mTextureMap;
};

#endif
