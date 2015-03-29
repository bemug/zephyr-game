#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <memory>
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>

#include "CommandQueue.hpp"
#include "Category.hpp"

//Forward declaration
struct Command;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<SceneNode> Ptr;
		typedef std::pair<SceneNode*, SceneNode*> Pair;
	public:
		SceneNode(Category::Type category = Category::None);
		void attachChild(Ptr child);
		Ptr detachChild(const SceneNode& node);
		void update(sf::Time dt, CommandQueue& commands);
		void onCommand(const Command& command, sf::Time dt);
		sf::Vector2f getWorldPosition() const;
		virtual sf::FloatRect getBoundingRect() const;
		void checkNodeCollision(SceneNode& node, std::set<Pair>&collisionPairs);
		void checkSceneCollision(SceneNode& sceneGraph,
				std::set<Pair>& collisionPairs);
		sf::Transform getWorldTransform() const;
		virtual unsigned int getCategory() const;
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time, CommandQueue&);
		virtual bool isDestroyed() const;
		void updateChildren(sf::Time dt, CommandQueue& commands);
		void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const;
	private:
		std::vector<Ptr> mChildren;
		SceneNode* mParent;
		Category::Type mDefaultCategory;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);

#endif
