#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "TextNode.hpp"
#include "ResourceHolder.hpp"
#include "CommandQueue.hpp"
#include "Projectile.hpp"

class Aircraft : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Raptor,
			Avenger,
			TypeCount,
		};

	public:
		explicit Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual unsigned int getCategory() const;
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		void updateMovementPattern(sf::Time dt);
		float getMaxSpeed();
		void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
		void createBullets(SceneNode& node, const TextureHolder& textures) const;
		void createProjectile(SceneNode& node,
				Projectile::Type type, float xOffset, float yOffset,
				const TextureHolder& textures) const;
		void fire();
		bool isAllied() const;
		void launchMissile();
		void collectMissiles(int nbMissiles);
		void increaseSpread();
		void increaseFireRate();
		sf::FloatRect getBoundingRect() const;
		virtual bool isMarkedForRemoval() const;

	private:
		Type mType;
		sf::Sprite mSprite;
		TextNode* mHealthDisplay;
		float mTravelledDistance;
		int mDirectionIndex;
		bool mIsFiring;
		sf::Time mFireCountdown;
		bool mIsLaunchingMissile;
		Command mFireCommand;
		Command mMissileCommand;
		int mFireRateLevel;
		int mSpreadLevel;
		int mMissileAmmo;
		bool mIsMarkedForRemoval;
};

#endif
