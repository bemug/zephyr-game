#include "World.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

#include "SpriteNode.hpp"
#include "CommandQueue.hpp"
#include "Category.hpp"
#include "Pickup.hpp"
#include "ParticleNode.hpp"

World::World(sf::RenderWindow& window, FontHolder& fonts)
: mWindow(window)
, mFonts(fonts)
, mWorldView(window.getDefaultView())
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
, mCommandQueue()
, mEnemySpawnPoints()
, mSceneGraph()
{
	loadTextures();
	buildScene();
	mWorldView.setCenter(mSpawnPosition);

	addEnemies();
}

void World::loadTextures() {
	mTextures.load(Textures::Eagle, "media/playerShip2_blue.png");
	mTextures.load(Textures::Raptor, "media/playerShip2_red.png");
	mTextures.load(Textures::Avenger, "media/playerShip2_green.png");
	mTextures.load(Textures::Desert, "media/desert.jpg");
	mTextures.load(Textures::BlueLaser, "media/laserBlue07.png");
	mTextures.load(Textures::GreenLaser, "media/laserGreen07.png");
	mTextures.load(Textures::Missile, "media/missile.png");
	mTextures.load(Textures::Particle, "media/smoke.png");
}

void World::buildScene() {
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;
		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	sf::Texture& texture = mTextures.get(Textures::Desert);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	//Ground
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	//Plane
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures, mFonts));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->setVelocity(0.f, mScrollSpeed);
	mSceneLayers[Air]->attachChild(std::move(leader));
	//Escort
	/*
	std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, mTextures, mFonts));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(leftEscort));
	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, mTextures, mFonts));
	rightEscort->setPosition(80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(rightEscort));
	*/

	//Particles
	std::unique_ptr<ParticleNode> smokeNode(
		new ParticleNode(Particle::Smoke, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));
}

void World::draw()
{
	//Delegate to scene graph
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::update(sf::Time dt)
{
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	destroyEntitiesOutsideView();
	guideMissiles();


	// Forward commands to the scene graph
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}


	sf::Vector2f velocity = mPlayerAircraft->getVelocity();
	if (velocity.x != 0.f && velocity.y != 0.f)
	{
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
	}
	mPlayerAircraft->accelerate(0.f, mScrollSpeed);

	handleCollisions();

	mSceneGraph.removeWrecks();

	spawnEnemies();

	// Regular update step
	mSceneGraph.update(dt, mCommandQueue);

	// Correct player position
	sf::FloatRect viewBounds(
			mWorldView.getCenter() - mWorldView.getSize() / 2.f,
			mWorldView.getSize());
	const float borderDistance = 40.f;
	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x,
			viewBounds.left + borderDistance);
	position.x = std::min(position.x,
			viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y,
			viewBounds.top + borderDistance);
	position.y = std::min(position.y,
			viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);

}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() {
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;
	return bounds;	
}

void World::spawnEnemies()
{
	while (!mEnemySpawnPoints.empty()
			&& mEnemySpawnPoints.back().y
			> getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();
		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);
		mSceneLayers[Air]->attachChild(std::move(enemy));
		mEnemySpawnPoints.pop_back();
	}
}

void World::addEnemies()
{
	addEnemy(Aircraft::Raptor, 0.f, 200.f);
	addEnemy(Aircraft::Avenger, -70.f, 400.f);
	//...
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(),
		[] (SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
}

void World::addEnemy(Aircraft::Type type, float relX, float relY) {
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

World::SpawnPoint::SpawnPoint(Aircraft::Type type, float x, float y)
: type(type)
, x(x)
, y(y)
{
}

void World::guideMissiles()
{
	Command enemyCollector;
	enemyCollector.category = Category::EnemyAircraft;
	enemyCollector.action = derivedAction<Aircraft>(
		[this] (Aircraft& enemy, sf::Time)
		{
			if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
		});
	Command missileGuider;
	missileGuider.category = Category::AlliedProjectile;
	missileGuider.action = derivedAction<Projectile>(
			[this] (Projectile& missile, sf::Time)
			{
			// Ignore unguided bullets
			if (!missile.isGuided())
				return;
			float minDistance = std::numeric_limits<float>::max();
			Aircraft* closestEnemy = nullptr;
			for (Aircraft* enemy : mActiveEnemies)
			{
				float enemyDistance = distance(missile, *enemy);
				if (enemyDistance < minDistance)
				{
					closestEnemy = enemy;
					minDistance = enemyDistance;
				}
			}
			if (closestEnemy)
				missile.guideTowards(
					closestEnemy->getWorldPosition());
			});
	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);

	mActiveEnemies.clear();
}

bool matchesCategories(SceneNode::Pair& colliders,
		Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
	for(SceneNode::Pair pair : collisionPairs)
	{
		if (matchesCategories(pair,
					Category::PlayerAircraft, Category::EnemyAircraft))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& enemy = static_cast<Aircraft&>(*pair.second);
			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}
		else if (matchesCategories(pair,
					Category::PlayerAircraft, Category::Pickup))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);
			pickup.apply(player);
			pickup.destroy();
		}
		else if (matchesCategories(pair,
					Category::EnemyAircraft, Category::AlliedProjectile)
				|| matchesCategories(pair,
					Category::PlayerAircraft, Category::EnemyProjectile))
		{
			auto& aircraft = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);
			//aircraft.damage(projectile.getDamage());
			aircraft.damage(1);
			projectile.destroy();
		}
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile
		| Category::EnemyAircraft;
	command.action = derivedAction<Entity>(
			[this] (Entity& e, sf::Time)
			{
				if (!getBattlefieldBounds()
						.intersects(e.getBoundingRect()))
				{
					e.destroy();
				}
			});
	mCommandQueue.push(command);
}
