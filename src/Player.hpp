#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "CommandQueue.hpp"

class Player
{
	public:
		void handleEvent(const sf::Event& event, CommandQueue& commands);
		void handleRealtimeInput(CommandQueue& commands);
};

#endif
