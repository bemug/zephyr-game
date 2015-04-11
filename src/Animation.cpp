#include "Animation.hpp"

Animation::Animation()
	: mSprite()
	, mFrameSize()
	, mNumFrames(0)
	, mCurrentFrame(0)
	, mDuration(sf::Time::Zero)
	, mElapsedTime(sf::Time::Zero)
	, mRepeat(false)
{
}

Animation::Animation(const sf::Texture& texture)
	: mSprite(texture)
	, mFrameSize()
	, mNumFrames(0)
	, mCurrentFrame(0)
	, mDuration(sf::Time::Zero)
	, mElapsedTime(sf::Time::Zero)
	, mRepeat(false)
{
}

void Animation::update(sf::Time dt)
{
	sf::Time timePerFrame = mDuration /
		static_cast<float>(mNumFrames);
	mElapsedTime += dt;
	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	sf::IntRect textureRect = mSprite.getTextureRect();
	if (mCurrentFrame == 0)
		textureRect = sf::IntRect(0, 0, mFrameSize.x,
				mFrameSize.y);

	while (mElapsedTime >= timePerFrame && (mCurrentFrame <=
				mNumFrames || mRepeat))
	{
		textureRect.left += textureRect.width;
		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}
		mElapsedTime -= timePerFrame;
		if (mRepeat)
		{
			mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;
			if (mCurrentFrame == 0)
				textureRect = sf::IntRect(0, 0, mFrameSize.x,
						mFrameSize.y);
		}
		else
		{
			mCurrentFrame++;
		}
	}
	mSprite.setTextureRect(textureRect);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

void Animation::setNumFrames(int nb)
{
	mNumFrames = nb;
}

void Animation::setDuration(sf::Time duration)
{
	mDuration = duration;
}

void Animation::setFrameSize(sf::Vector2i v)
{
	mFrameSize = v;
}

bool Animation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::Vector2i Animation::getFrameSize() const
{
	return mFrameSize;
}
