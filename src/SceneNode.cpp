#include <iostream>
#include <cassert>
#include <algorithm>
#include <set>

#include "SceneNode.hpp"
#include "Category.hpp"
#include "Command.hpp"
#include "Utility.hpp"

SceneNode::SceneNode(Category::Type category)
: mChildren()
, mParent(nullptr)
, mDefaultCategory(category)
{
}

void SceneNode::attachChild(SceneNode::Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(),
			[&] (Ptr& p) -> bool { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::draw(sf::RenderTarget& target,
		sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);

	for (const Ptr& child : mChildren)
	{
		child->draw(target, states);
	}
	drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (Ptr& child : mChildren)
		child->update(dt, commands);
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	for (const SceneNode* node = this; node != nullptr; node = node->mParent) {
		transform = node->getTransform() * transform;
	}
	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

unsigned int SceneNode::getCategory() const
{
	return mDefaultCategory;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	if (command.category & getCategory()) {
		command.action(*this, dt);
	}
	for (Ptr& child : mChildren)
		child->onCommand(command, dt);
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>&
		collisionPairs)
{
	if (this != &node && collision(*this, node)
			&& !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));
	for (Ptr& child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph,
		std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);
	for (Ptr& child : sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

bool SceneNode::isDestroyed() const
{
	// By default, scene node needn't be removed
	return false;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
	sf::FloatRect rect = getBoundingRect();
	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	target.draw(shape);
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

void SceneNode::removeWrecks()
{
	auto wreckfieldBegin = std::remove_if(mChildren.begin(),
			mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());
	std::for_each(mChildren.begin(), mChildren.end(),
			std::mem_fn(&SceneNode::removeWrecks));
}
