#include "Projectile.hpp"

#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mTargetDirection()
{
	centerOrigin(mSprite);
}

bool Projectile::isGuided() const
{
	return mType == Missile;
}

void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

void Projectile::updateCurrent(sf::Time dt,
		CommandQueue& commands)
{
	if (isGuided())
	{
		const float approachRate = 200.f;
		sf::Vector2f newVelocity = unitVector(approachRate
				* dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);
		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}
	Entity::updateCurrent(dt, commands);
}

float Projectile::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

sf::FloatRect Projectile::getBoundingRect() const
{
	sf::FloatRect ret(0.f, 0.f, 0.f, 0.f); //TODO srsly aha
	return ret;
}

unsigned int Projectile::getCategory() const
{
	if (mType == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}
