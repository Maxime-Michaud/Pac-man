#pragma once
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class dragonShout : public sf::Drawable
{
	sf::Clock _clockDragonShout;			  //Pour l'effet du dragonshout
	sf::Vector2f _pos;
public:
	dragonShout();
	~dragonShout();
	void draw(sf::RenderTarget &rt, sf::RenderStates rs)const;
	void resetClockDragonShout();
	void setPos(sf::Vector2f pos);
};



