#pragma once
#include "fantome.h"
class FantomeBleu : public Fantome
{
	std::deque<sf::Vector2f> _pointsPrecedents;
public:
	FantomeBleu();
	~FantomeBleu();
	virtual void move(char direction, sf::Vector2f& posPacMan, Map &map);		 //Fait bouger le fantome
	virtual void deciderLigne(sf::Vector2f& posFantomeRouge, Map &map);			 //Prend une d�cision de la direction � un intersection
};
