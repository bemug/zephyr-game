#include "World.hpp"

#include "SpriteNode.hpp"
#include "CommandQueue.hpp"

World::World(sf::RenderWindow& window, FontHolder& fonts)
: mWindow(window)
, mFonts(fonts)
, mWorldView(window.getDefaultView())
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
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
}

void World::buildScene() {
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
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

	// Forward commands to the scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	sf::Vector2f velocity = mPlayerAircraft->getVelocity();
	if (velocity.x != 0.f && velocity.y != 0.f)
	{
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
	}
	mPlayerAircraft->accelerate(0.f, mScrollSpeed);

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

	spawnEnemies();
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
,y (y)
{
}
