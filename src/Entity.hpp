#ifndef ENTITY_H
#define ENTITY_H

#include "SceneNode.hpp"

class Entity : public SceneNode
{
	public:
		explicit Entity(); //Backward compatibility (remove me)
		explicit Entity(int hitpoints);
		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);
		sf::Vector2f getVelocity() const;
		void accelerate(sf::Vector2f velocity);
		void accelerate(float vx, float vy);

		void repair(int points);
		void damage(int points);
		void destroy();

		int getHitpoints() const;
		bool isDestroyed() const;

	private:
		virtual void updateCurrent(sf::Time dt);
	private:
		sf::Vector2f mVelocity;
		int hitpoints;
};

#endif
