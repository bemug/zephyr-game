#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SFML/Graphics.hpp>

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "World.hpp"
#include "Player.hpp"

class GameState : public State
{
	public:
		GameState(StateStack& stack, Context context);
		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		World mWorld;
		Player& mPlayer;
};

#endif
