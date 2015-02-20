#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.h"

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "Zephyr")
	  , mPlayer()
{
	mPlayer.setRadius(40.f);
	mPlayer.setPosition(100.f, 100.f);
	mPlayer.setFillColor(sf::Color::Cyan);
}

void Game::run()
{
	while (mWindow.isOpen())
	{
		processEvents();
		update();
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Game::update()
{
	//Nothing for now!
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.display();
}
