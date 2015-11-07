#pragma once
#include "fantome.h"
class FantomeOrange : public Fantome
{
	const int _nombreIntersectionAvantChangerDirection = 2;
	int _nbIntersectionPasse = 0;
public:
	FantomeOrange();
	~FantomeOrange();
	virtual void move(char direction, sf::Vector2f posPacMan, Map &map);	 //Fait bouger le fantome
	virtual void deciderLigne(sf::Vector2f posPacMan, Map &map);			 //Prend une décision de la direction à un intersection
};
