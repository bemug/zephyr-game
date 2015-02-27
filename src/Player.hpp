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
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			ActionCount
		};

	public:
		Player();
		void handleEvent(const sf::Event& event, CommandQueue& commands);
		void handleRealtimeInput(CommandQueue& commands);

		void assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key getAssignedKey(Action action) const;

	private:
		void initializeActions();
		static bool isRealtimeAction(Action action);
		std::map<sf::Keyboard::Key, Action> mKeyBinding;
		std::map<Action, Command> mActionBinding;
};

#endif
