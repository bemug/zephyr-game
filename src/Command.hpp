#ifndef COMMAND_H
#define COMMAND_H

#include <SFML/Graphics.hpp>

//Forward declaration
class SceneNode;

struct Command
{
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

#endif
