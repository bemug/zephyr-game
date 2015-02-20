#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Game
{
	public:
		Game();
		void run();
	private:
		void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
		void processEvents();
		void update(sf::Time deltaTime);
		void render();
	private:
		sf::RenderWindow mWindow;
		sf::Texture mTexture;
		sf::Sprite mPlayer;
		bool mIsMovingUp, mIsMovingDown, mIsMovingLeft, mIsMovingRight;
};

#endif
