#ifndef PARTICLE_NODE_H
#define PARTICLE_NODE_H

#include <SFML/Graphics.hpp>

#include "Particle.hpp"
#include "ResourceHolder.hpp"

class ParticleNode : public SceneNode
{
	public:
		ParticleNode(Particle::Type type, const TextureHolder& textures);
		void addParticle(sf::Vector2f position);
		Particle::Type getParticleType() const;
		virtual unsigned int getCategory() const;
	private:
		std::deque<Particle> Particles;
		const sf::Texture& mTexture;
		Particle::Type mType;
		mutable sf::VertexArray mVertexArray;
		mutable bool mNeedsVertexUpdate;
};

#endif
