#include "TitleState.hpp"

#include "Utility.hpp"

TitleState::TitleState(StateStack& stack, Context context) 
: State(stack, context)
, mShowText(true)
{

	mText.setFont(context.fonts->get(Fonts::Default));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	mText.setPosition(context.window->getView().getSize() / 2.f);
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Menu);
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
	sf::RenderWindow& window = *getContext().window;
	//window.draw(mBackgroundSprite);
	if (mShowText)
		window.draw(mText);
}
