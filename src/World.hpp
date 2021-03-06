#ifndef WORLD_H
#define WORLD_H

#include<SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "ResourceHolder.hpp"
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "CommandQueue.hpp"
#include "BloomEffect.hpp"

class World : private sf::NonCopyable
{
	public:
		explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts);
		void update(sf::Time dt);
		void draw();
		CommandQueue& getCommandQueue();
		void guideMissiles();

		struct SpawnPoint
		{
			SpawnPoint(Aircraft::Type type, float x, float y);
			Aircraft::Type type;
			float x;
			float y;
		};

	private:
		void loadTextures();
		void buildScene();

	private:
		enum Layer
		{
			Background,
			LowerAir,
			Air,
			LayerCount //hack
		};

	private:
		sf::RenderTarget& mTarget;
		sf::View mWorldView;
		TextureHolder mTextures;
		FontHolder& mFonts;
		SceneNode mSceneGraph;
		std::array<SceneNode*, LayerCount> mSceneLayers;
		sf::FloatRect mWorldBounds;
		sf::Vector2f mSpawnPosition;
		float mScrollSpeed;
		Aircraft* mPlayerAircraft;
		CommandQueue mCommandQueue;
		std::vector<SpawnPoint> mEnemySpawnPoints;
		sf::FloatRect getViewBounds() const;
		std::vector<Aircraft*> mActiveEnemies;
		sf::RenderTexture mSceneTexture;
		BloomEffect mBloomEffect;
		
		sf::FloatRect getBattlefieldBounds();
		void spawnEnemies();
		void addEnemies();
		void addEnemy(Aircraft::Type type, float x, float y);
		void handleCollisions();
		void destroyEntitiesOutsideView();
		void addRandomEnnemy();
};

bool matchesCategories(SceneNode::Pair& colliders,
		Category::Type type1, Category::Type type2);

#endif
