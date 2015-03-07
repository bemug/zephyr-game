#include "MenuState.hpp"

#include "Utility.hpp"

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mOptionIndex(0)
{
	sf::Text playOption;
	playOption.setFont(context.fonts->get(Fonts::Default));
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	mOptions.push_back(playOption);

	sf::Text quitOption;
	quitOption.setFont(context.fonts->get(Fonts::Default));
	quitOption.setString("Quit");
	centerOrigin(quitOption);
	quitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	mOptions.push_back(quitOption);

	updateOptionText();
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// Avoid press/release effect
	if (event.type != sf::Event::KeyPressed)
		 return false;

	if (event.key.code == sf::Keyboard::Up)
	{
		if (mOptionIndex > 0)
			mOptionIndex--;
		else
			mOptionIndex = mOptions.size() - 1;
		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		if (mOptionIndex < mOptions.size() - 1)
			mOptionIndex++;
		else
			mOptionIndex = 0;
		updateOptionText();
	}

	if (event.key.code == sf::Keyboard::Return)
	{
		if (mOptionIndex == Play)
		{
			requestStackPop();
			requestStackPush(States::Game);
		}
		else if (mOptionIndex == Exit)
		{
			requestStackPop();
		}
	}

	return true;
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

void MenuState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	//window.draw(mBackgroundSprite);
	for(const sf::Text& text : mOptions)
		window.draw(text);
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
		return;

	// White all texts
	for(sf::Text& text : mOptions)
		text.setColor(sf::Color::White);

	// Red the selected text
	mOptions[mOptionIndex].setColor(sf::Color::Red);
}

