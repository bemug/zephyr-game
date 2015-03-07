#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "Game.hpp"
#include "ResourceHolder.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "State.hpp"

//All states
#include "GameState.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "Zephyr")
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingLeft(false)
	, mIsMovingRight(false)
	, mFps(-1)
	, mFpsText()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer)) //TODO font holder
{
	 mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::Default, "media/UbuntuMono-R.ttf");

	mFpsText.setString("");
	mFpsText.setFont(mFonts.get(Fonts::Default));
	mFpsText.setPosition(2,0);
	mFpsText.setCharacterSize(12); // in pixels, not points!
	mFpsText.setColor(sf::Color::White);

	//Start with the title screen
	registerStates();
	mStateStack.pushState(States::Title);
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

			//Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
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
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Game::update(sf::Time deltaTime)
{
	mStateStack.update(deltaTime);
}

void Game::displayFps() {
	std::ostringstream oss;
	oss << "fps: " << mFps;
	mFpsText.setString(oss.str());
}

void Game::render()
{
	mWindow.clear();
	mStateStack.draw();
	mWindow.setView(mWindow.getDefaultView());
	displayFps();
	mWindow.display();
}

void Game::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	//mStateStack.registerState<PauseState>(States::Pause);
}
