#pragma once
#include "fantome.h"
class FantomeRose : public Fantome
{
	const int _recalculeLeCheminTousLesXFois = 4;
	int _nbEloignementPourAiRose = 3;			//le nombre de fois que le rose peut s'eloignement de pacman pour trouver un chemin, dans un labyrinthw, monter cette valeur.
	int _nombreDintersectionsPassee = 0;
	sf::Clock _clockFantomeRose;
	bool _bougerCommeRouge = false;				//Bouge comme le rouge quand le chemin est trop demandant
public:

	struct Direction
	{
		int _ligne;
		char _direction;
		int _nbEssaie = 0;
		sf::Vector2f _posi;
		int _nbEloignement = 0;

		Direction::Direction(int ligne, char direction, int nbEssaie, sf::Vector2f posi, int nbEloignement)
		{
			_ligne = ligne;
			_direction = direction;
			_nbEssaie = nbEssaie;
			_posi = posi;
			_nbEloignement = nbEloignement;
		}
	};

	std::deque<Direction> _chemin;
	FantomeRose();
	~FantomeRose();
	bool moveTheorique(char directionTheorique, int ligneTheorique, sf::Vector2f& posTheorique, Map &map);
	virtual void move(char direction, sf::Vector2f& posPacMan, Map &map);	 //Fait bouger le fantome
	void bougerCommeRouge(sf::Vector2f& posPacMan, Map &map);				 //Bouge comme le rouge quand trop de chemin est tenté
	virtual void deciderLigne(sf::Vector2f& posPacMan, Map &map);			 //Prend une décision de la direction à un intersection
	void tentativeAmbuscade(int LignePacMan, Map &map, sf::Vector2f &posPacMan);
	bool esseyerLigne(char direction, int &ligneParcoursTheorique, std::vector<sf::Vector2f> &pointsVisites, sf::Vector2f &posTheorique, Map &map, char directionArrivee);
};
