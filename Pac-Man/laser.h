#pragma once

#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class Laser : public sf::Drawable
{
public:
	Laser(sf::Vector2f);
	~Laser();
	Laser(sf::Vector2f, char);
	void draw(sf::RenderTarget &rt, sf::RenderStates rs)const;
private:
	sf::Vector2f _pos;
	char _direction;
};