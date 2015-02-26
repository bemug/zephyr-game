#ifndef COMMAND_H
#define COMMAND_H

struct Command
{
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

#endif
