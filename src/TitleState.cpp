#include "TitleState.hpp"

TitleState::TitleState(StateStack& stack, Context context) 
	: State(stack, context)
{
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Game);
	}
	return true;
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;
	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}
	return true;
}

void TitleState::draw() {

}
