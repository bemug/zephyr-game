#include <iostream>

#include "ParticleNode.hpp"
#include "DataTables.hpp"
#include "CommandQueue.hpp"

namespace
{
	const std::vector<ParticleData> Table = initializeParticleData();
}

ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures)
: SceneNode()
, mParticles()
, mType(type)
, mTexture(textures.get(Textures::Particle))
, mVertexArray(sf::Quads)
, mNeedsVertexUpdate(true)
{

}

unsigned int ParticleNode::getCategory() const
{
	return Category::ParticleSystem;
}

Particle::Type ParticleNode::getParticleType() const
{
	return mType;
}

void ParticleNode::addParticle(sf::Vector2f position)
{
	Particle particle;
	particle.position = position;
	particle.color = Table[mType].color;
	particle.lifetime = Table[mType].lifetime;
	mParticles.push_back(particle);
}

void ParticleNode::addVertex(float worldX, float worldY,
		float texCoordX, float texCoordY,
		const sf::Color& color) const
{
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
	vertex.color = color;
	mVertexArray.append(vertex);
}

void ParticleNode::computeVertices() const
{
	sf::Vector2f size(mTexture.getSize());
	sf::Vector2f half = size / 2.f;
	mVertexArray.clear();

	for (const Particle& particle : mParticles)
	{
		sf::Vector2f pos = particle.position;
		sf::Color c = particle.color;
		float ratio = particle.lifetime.asSeconds()
			/ Table[mType].lifetime.asSeconds();
		c.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));
		addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, c);
		addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, c);
		addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, c);
		addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, c);
	}
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	while (!mParticles.empty()
			&& mParticles.front().lifetime <= sf::Time::Zero)
		mParticles.pop_front();
	for(Particle& particle : mParticles)
		particle.lifetime -= dt;
	mNeedsVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target,
		sf::RenderStates states) const
{
	if (mNeedsVertexUpdate)
	{
		computeVertices();
		mNeedsVertexUpdate = false;
	}
	states.texture = &mTexture;
	target.draw(mVertexArray, states);
}


