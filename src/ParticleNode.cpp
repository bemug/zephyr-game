#include "ParticleNode.hpp"

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

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue&)
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
