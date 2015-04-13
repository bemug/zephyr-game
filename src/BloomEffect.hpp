#ifndef BLOOM_EFFECT_H
#define BLOOM_EFFECT_H

#include "SFML/Graphics.hpp"

#include "PostEffect.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

class BloomEffect : public PostEffect
{
	private:
		typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

	public:
		BloomEffect();
		virtual void apply(const sf::RenderTexture& input,
				sf::RenderTarget& output);
	private:
		void prepareTextures(sf::Vector2u size);
		void filterBright(const sf::RenderTexture& input,
				sf::RenderTexture& output);
		void blurMultipass(RenderTextureArray& renderTextures);
		void blur(const sf::RenderTexture& input,
				sf::RenderTexture& output, sf::Vector2f offsetFactor);
		void downsample(const sf::RenderTexture& input,
				sf::RenderTexture& output);
		void add(const sf::RenderTexture& source, const
				sf::RenderTexture& bloom, sf::RenderTarget& output);

		ShaderHolder mShaders;
		sf::RenderTexture mBrightnessTexture;
		RenderTextureArray mFirstPassTextures;
		RenderTextureArray mSecondPassTextures;
};

#endif
