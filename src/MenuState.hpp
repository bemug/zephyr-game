#ifndef MENU_STATE_H
#define MENU_STATE_H

#include <SFML/Graphics.hpp>

#include "StateStack.hpp"
#include "State.hpp"

class MenuState : public State
{
	public:
		MenuState(StateStack& stack,
				Context context);
		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		void updateOptionText();

		enum OptionNames
		{
			Play,
			Exit,
		};
		std::vector<sf::Text> mOptions;
		//sf::Sprite mBackgroundSprite;
		std::size_t mOptionIndex;
};

#endif
