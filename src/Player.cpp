#include <iostream>

#include "Player.hpp"
#include "Category.hpp"
#include "Category.hpp"
#include "Aircraft.hpp"

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
	const float playerSpeed = 30.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		Command moveLeft;
		moveLeft.category = Category::PlayerAircraft;
		moveLeft.action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
		commands.push(moveLeft);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		Command moveRight;
		moveRight.category = Category::PlayerAircraft;
		moveRight.action = derivedAction<Aircraft>(AircraftMover(playerSpeed, 0.f));
		commands.push(moveRight);
	}
}
