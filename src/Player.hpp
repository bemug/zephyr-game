#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "CommandQueue.hpp"
#include "Aircraft.hpp"

struct AircraftMover
{
	AircraftMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}
	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.setVelocity(aircraft.getVelocity() + velocity);
	}
	sf::Vector2f velocity;
};

class Player
{
	public:
		void handleEvent(const sf::Event& event, CommandQueue& commands);
		void handleRealtimeInput(CommandQueue& commands);
};

#endif
