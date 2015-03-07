#include "PauseState.hpp"

#include "Utility.hpp"

PauseState::PauseState(StateStack& stack, Context context) 
: State(stack, context)
{
	mPausedText.setFont(context.fonts->get(Fonts::Default));
	mPausedText.setString("Pause");
	centerOrigin(mPausedText);
	mPausedText.setPosition(context.window->getView().getSize() / 2.f);

	mInstructionText.setFont(context.fonts->get(Fonts::Default));
	mInstructionText.setString("Press backspace to go to main menu");
	mInstructionText.setCharacterSize(12);
	centerOrigin(mInstructionText);
	mInstructionText.setPosition(context.window->getView().getSize() / 2.f + sf::Vector2f(0.f, 30.f));
;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	// Avoid press/release effect
	if (event.type != sf::Event::KeyPressed)
		 return false;

	if (event.key.code == sf::Keyboard::Escape)
	{
		//Go back to game
		requestStackPop();
	}
	else if (event.key.code == sf::Keyboard::BackSpace)
	{
		//Go to main menu
		requestStateClear();
		requestStackPush(States::Menu);
	}

	return false;
}

bool PauseState::update(sf::Time dt)
{
	//We're paused, nothing to do
}

void PauseState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(sf::Vector2f(window.getSize()));
	window.draw(backgroundShape);

	window.draw(mPausedText);
	window.draw(mInstructionText);
}
