#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

//Forward declaration
struct Command;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<SceneNode> Ptr;
	public:
		void attachChild(Ptr child);
		Ptr detachChild(const SceneNode& node);
		void update(sf::Time dt);
		void onCommand(const Command& command, sf::Time dt);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt);
		virtual unsigned int getCategory() const;
		void updateChildren(sf::Time dt);
		sf::Transform getWorldTransform() const;
		sf::Vector2f getWorldPosition() const;
	private:
		std::vector<Ptr> mChildren;
		SceneNode* mParent;
};

#endif
