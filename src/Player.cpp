#include <iostream>

#include "Player.hpp"
#include "Category.hpp"
#include "Category.hpp"
#include "Aircraft.hpp"

Player::Player()
{
	const float playerSpeed = 200.f;
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mActionBinding[MoveLeft].action =
		[=] (SceneNode& node, sf::Time dt)
		{
			node.move(-playerSpeed * dt.asSeconds(), 0.f);
		};
	mActionBinding[MoveRight].action =
		[=] (SceneNode& node, sf::Time dt)
		{
			node.move(playerSpeed * dt.asSeconds(), 0.f);
		};
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
