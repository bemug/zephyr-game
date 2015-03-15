#ifndef _UTILITY_H
#define _UTILITY_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
float toRadian(float degree);
sf::Vector2f unitVector(sf::Vector2f vector);
float length(sf::Vector2f vector);
float toDegree(float radian);

#endif
