#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <string>
#include <memory>
#include <cassert>
#include <SFML/Graphics.hpp>

namespace Textures
{
	enum ID { Landscape, Airplane, Missile };
}

template <typename Resource, typename Identifier>
class ResourceHolder
{
	public:
		void load(Identifier id, const std::string& filename);
		template <typename Parameter>
		void load(Identifier id, const std::string& filename, const Parameter& secondparam);
		Resource& get(Identifier id);
		const Resource& get(Identifier id) const;
	private:
		std::map<Identifier,
			std::unique_ptr<Resource>> mResourceMap;
};

#include "ResourceHolder.inl"
#endif
