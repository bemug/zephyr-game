#include <iostream>

#include "Entity.hpp"

Entity::Entity()
: hitpoints(1)
{
}

Entity::Entity(int hitpoints)
: hitpoints(hitpoints)
{
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	move(mVelocity * dt.asSeconds()); //short for setPosition(getPosition() + offset)
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}

void Entity::repair(int points)
{
	hitpoints += points;
}

void Entity::damage(int points)
{
	hitpoints -= points;
}

void Entity::destroy()
{
	hitpoints = 0;
}

int Entity::getHitpoints() const {
	return hitpoints;
}
