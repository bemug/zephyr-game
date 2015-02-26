#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "ResourceHolder.hpp"
#include "World.hpp"
#include "Player.hpp"

class Game
{
	public:
		Game();
		void run();
	private:
		void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
		void displayFps();
		void processInput();
		void update(sf::Time deltaTime);
		void updateFps(sf::Time elapsedTime);
		void render();
	private:
		sf::RenderWindow mWindow;
		World mWorld;
		Player mPlayer;

		float mFps;
		sf::Text mFpsText;
		sf::Font mFpsFont;
		bool mIsMovingUp, mIsMovingDown, mIsMovingLeft, mIsMovingRight;
};

#endif
