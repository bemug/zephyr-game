#include <iostream>

#include "Player.hpp"
#include "Category.hpp"
#include "Category.hpp"
#include "Aircraft.hpp"

struct AircraftMover
{
	AircraftMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}
	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.accelerate(velocity);
	}
	sf::Vector2f velocity;
};

Player::Player()
{
	const float playerSpeed = 200.f;
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(playerSpeed, 0.f));
	mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, playerSpeed));
	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands) {
	if (event.type == sf::Event::KeyPressed
			&& event.key.code == sf::Keyboard::P)
	{
		Command output;
		output.category = Category::PlayerAircraft;
		output.action = [] (SceneNode& s, sf::Time)
		{
			std::cout << s.getPosition().x << ","
				<< s.getPosition().y << "\n";
		};
		commands.push(output);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands) {
	for (auto pair : mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first)
				&& isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}


bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
			return true;
		default:
			return false;
	}
}
