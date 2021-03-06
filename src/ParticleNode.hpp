#ifndef PARTICLE_NODE_H
#define PARTICLE_NODE_H

#include <SFML/Graphics.hpp>

#include "Particle.hpp"
#include "ResourceHolder.hpp"
#include "SceneNode.hpp"

class ParticleNode : public SceneNode
{
	public:
		ParticleNode(Particle::Type type, const TextureHolder& textures);
		void addParticle(sf::Vector2f position);
		Particle::Type getParticleType() const;
		virtual unsigned int getCategory() const;
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void drawCurrent(sf::RenderTarget& target, 
				sf::RenderStates states) const;
	private:
		std::deque<Particle> mParticles;
		const sf::Texture& mTexture;
		Particle::Type mType;
		mutable sf::VertexArray mVertexArray;
		mutable bool mNeedsVertexUpdate;
		void computeVertices() const;
		void addVertex(float worldX, float worldY,
			float texCoordX, float texCoordY,
			const sf::Color& color) const;
};

#endif
