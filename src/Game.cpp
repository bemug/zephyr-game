#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "Game.h"

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "Zephyr")
	, mPlayer()
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingLeft(false)
	, mIsMovingRight(false)
	, mFps(-1)
	, mFpsText()
	, mFpsFont()
{
	if (!mTexture.loadFromFile("media/ship.gif"))
	{
		// Handle loading error
	}
	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(100.f, 100.f);
	mFpsText.setString("");
	if (!mFpsFont.loadFromFile("media/UbuntuMono-R.ttf")) {
		//Error handling
		std::cout << "Error loading font" << std::endl;
	}
	mFpsText.setFont(mFpsFont);
	mFpsText.setPosition(2,0);
	mFpsText.setCharacterSize(12); // in pixels, not points!
	mFpsText.setColor(sf::Color::White);
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Z)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Q)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}

void Game::run()
{
	sf::Clock clock;
	sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}
		updateFps(elapsedTime);
		render();
	}
}

void Game::updateFps(sf::Time elapsedTime)
{
	mFps = 1/elapsedTime.asSeconds();
}


void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				handlePlayerInput(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				handlePlayerInput(event.key.code, false);
				break;
			case sf::Event::Closed:
				mWindow.close();
				break;
		}
	}
}

void Game::update(sf::Time deltaTime)
{
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp)
		movement.y -= 1.f;
	if (mIsMovingDown)
		movement.y += 1.f;
	if (mIsMovingLeft)
		movement.x -= 1.f;
	if (mIsMovingRight)
		movement.x += 1.f;
	mPlayer.move(movement * (float)deltaTime.asMilliseconds());
}

void Game::displayFps() {
	std::ostringstream oss;
	oss << "fps: " << mFps;
	mFpsText.setString(oss.str());
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	displayFps();
	mWindow.draw(mFpsText);
	mWindow.display();
}
