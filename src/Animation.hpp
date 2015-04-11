#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation : public sf::Drawable, public sf::Transformable
{
	public:
		Animation();
		Animation(const sf::Texture& texture);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void update(sf::Time dt);
		void setNumFrames(int nb);
		void setDuration(sf::Time duration);
		void setFrameSize(sf::Vector2i v);
		bool isFinished() const;
		sf::FloatRect getLocalBounds() const;
	private:
		sf::Sprite mSprite;
		sf::Vector2i mFrameSize;
		std::size_t mNumFrames;
		std::size_t mCurrentFrame;
		sf::Time mDuration;
		sf::Time mElapsedTime;
		bool mRepeat;
		sf::Vector2i getFrameSize() const;
};

#endif
