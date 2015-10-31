#pragma once
#include "fantome.h"
class FantomeRose : public Fantome
{
	const int _recalculeLeCheminTousLesXFois = 1;
	int _nombreDintersectionsPassee = 0;
public:

	struct Direction
	{
		int _ligne;
		char _direction;
		int _nbEssaie = 0;
		sf::Vector2f _posi;

		Direction::Direction(int ligne, char direction, int nbEssaie, sf::Vector2f posi)
		{
			_ligne = ligne;
			_direction = direction;
			_nbEssaie = nbEssaie;
			_posi = posi;
		}
	};

	std::deque<Direction> _chemin;
	FantomeRose();
	~FantomeRose();
	bool moveTheorique(char directionTheorique, int ligneTheorique, sf::Vector2f posTheorique, Map &map);
	virtual void move(char direction, sf::Vector2f posPacMan, Map &map);	 //Fait bouger le fantome
	virtual void deciderLigne(sf::Vector2f posPacMan, Map &map);			 //Prend une décision de la direction à un intersection
	void tentativeAmbuscade(sf::Vector2f posPacMan, int LignePacMan, Map &map);
	bool esseyerLigne(char direction, int &ligneParcoursTheorique, std::vector<sf::Vector2f> &pointsVisites, sf::Vector2f &posTheorique, Map &map, char directionArrivee);
};

