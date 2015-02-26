#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "Game.hpp"
#include "ResourceHolder.hpp"
#include "Player.hpp"

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "Zephyr")
	, mWorld(mWindow)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingLeft(false)
	, mIsMovingRight(false)
	, mFps(-1)
	, mFpsText()
{
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
			processInput();
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


void Game::processInput()
{
	CommandQueue& commands = mWorld.getCommandQueue();

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mPlayer.handleEvent(event, commands);
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}

	mPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time deltaTime)
{
	/*
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
	*/
	mWorld.update(deltaTime);
}

void Game::displayFps() {
	std::ostringstream oss;
	oss << "fps: " << mFps;
	mFpsText.setString(oss.str());
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();
	mWindow.setView(mWindow.getDefaultView());
	displayFps();
	mWindow.display();
}
