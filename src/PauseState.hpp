#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include <SFML/Graphics.hpp>

#include "StateStack.hpp"
#include "State.hpp"

class PauseState : public State
{
	public:
		PauseState(StateStack& stack,
				Context context);
		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Text mPausedText;
		sf::Text mInstructionText;
};

#endif
