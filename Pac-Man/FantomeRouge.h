#pragma once
#include "fantome.h"
class FantomeRouge : public Fantome
{
public:
	FantomeRouge();
	~FantomeRouge();
	virtual void move(char direction, sf::Vector2f& posPacMan, Map &map);	 //Fait bouger le fantome
	virtual void deciderLigne(sf::Vector2f& posPacMan, Map &map);			 //Prend une décision de la direction à un intersection
};
