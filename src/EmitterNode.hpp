#ifndef EMITTER_NODE_H
#define EMITTER_NODE_H

#include "Particle.hpp"
#include "ParticleNode.hpp"

class EmitterNode : public SceneNode
{
	public:
		explicit EmitterNode(Particle::Type type);
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

	private:
		sf::Time mAccumulatedTime;
		Particle::Type mType;
		ParticleNode* mParticleSystem;
		void emitParticles(sf::Time dt);
};

#endif
