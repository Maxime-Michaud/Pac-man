#include "FantomeRose.h"

FantomeRose::FantomeRose()
{
	_color = sf::Color(255, 192, 203, 255);
	_nom = "rose";
}

//Donne la prohcaine direction vers la DROITE
char tourner(char direction)
{
	switch (direction)
	{
	case 'a':
		direction = 'w';
		break;
	case 'd':
		direction = 's';
		break;
	case 's':
		direction = 'a';
		break;
	case 'w':
		direction = 'd';
		break;
	default:
		break;
	}
	return direction;
}

//Permet au fantôme rose de se déplacer théoriquement sans changer sa position actuelle
bool FantomeRose::moveTheorique(char directionTheorique, int ligneTheorique, sf::Vector2f& posTheorique, Map &map)
{
	Ligne temp = map.getLigne(ligneTheorique);
	sf::Vector2f vtemp(posTheorique);
	switch (directionTheorique)
	{
	case 'a':
		vtemp.x -= _vitesse;
		if (temp.isOn(vtemp))
		{
			posTheorique.x -= _vitesse;
			return true;
		}
		break;

	case 's':
		vtemp.y += _vitesse;
		if (temp.isOn(vtemp))
		{
			posTheorique.y += _vitesse;
			return true;
		}
		break;

	case 'd':
		vtemp.x += _vitesse;
		if (temp.isOn(vtemp))
		{
			posTheorique.x += _vitesse;
			return true;
		}
		break;

	case 'w':
		vtemp.y -= _vitesse;
		if (temp.isOn(vtemp))
		{
			posTheorique.y -= _vitesse;
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

//Essais de changer de ligne et de pos théorique et renvois vrai si c'est changé
//Change la ligne théorique, les points visitées et la pos théorique si il effectue un changement de ligne
bool FantomeRose::esseyerLigne(char direction, int &ligneParcoursTheorique, std::vector<sf::Vector2f> &pointsVisites, sf::Vector2f &posTheorique, Map &map, char directionArrivee)
{
	_vertical = map.getLigne(ligneParcoursTheorique).isVertical();
	int temp = ligneParcoursTheorique;
	sf::Vector2f posTemp = posTheorique;
	if (direction == inverserDirection(directionArrivee))
	{
		return false;
	}

	switch (direction)
	{
	case 'a':
		ligneParcoursTheorique = map.quelleLigne(sf::Vector2f(posTheorique.x - 1, posTheorique.y), ligneParcoursTheorique);
		break;
	case 'd':
		ligneParcoursTheorique = map.quelleLigne(sf::Vector2f(posTheorique.x + 1, posTheorique.y), ligneParcoursTheorique);
		break;
	case 's':
		ligneParcoursTheorique = map.quelleLigne(sf::Vector2f(posTheorique.x, posTheorique.y + 1), ligneParcoursTheorique);
		break;
	case 'w':
		ligneParcoursTheorique = map.quelleLigne(sf::Vector2f(posTheorique.x, posTheorique.y - 1), ligneParcoursTheorique);
		break;
	default:
		break;
	}

	auto tempDebutX = map.getLigne(ligneParcoursTheorique).getDebut().x - posTheorique.x;
	auto tempDebutY = map.getLigne(ligneParcoursTheorique).getDebut().y - posTheorique.y;
	auto tempFinX = map.getLigne(ligneParcoursTheorique).getFin().x - posTheorique.x;
	auto tempFinY = map.getLigne(ligneParcoursTheorique).getFin().y - posTheorique.y;

	if (ligneParcoursTheorique != temp)
	{
		//prend le point de la ligne le plus éloigné et se set a cette position
		if (abs(tempDebutX + tempDebutY) > abs(tempFinX + tempFinY))
			posTheorique = map.getLigne(ligneParcoursTheorique).getDebut();
		else
			posTheorique = map.getLigne(ligneParcoursTheorique).getFin();
		for (std::vector<sf::Vector2f>::iterator i = pointsVisites.begin(); i != pointsVisites.end(); i++)
		{
			if (posTheorique == (*i))
			{
				posTheorique = posTemp;
				ligneParcoursTheorique = temp;
				return false;
			}
		}
		return true;
	}

	//Si la ligne est la même, vérifie si il peut aller en ligne droite
	if (moveTheorique(direction, ligneParcoursTheorique, posTheorique, map))
	{
		//prend le point de la ligne le plus éloigné et se set a cette position
		if (abs(tempDebutX + tempDebutY) > abs(tempFinX + tempFinY))
			posTheorique = map.getLigne(ligneParcoursTheorique).getDebut();
		else
			posTheorique = map.getLigne(ligneParcoursTheorique).getFin();
		for (std::vector<sf::Vector2f>::iterator i = pointsVisites.begin(); i != pointsVisites.end(); i++)
		{
			if (posTheorique == (*i))
			{
				posTheorique = posTemp;
				ligneParcoursTheorique = temp;
				return false;
			}
		}
		return true;
	}
	else
		return false;
}

void FantomeRose::setNbEssai(int n)
{
	_nbEloignementPourAiRose = n;
}

void FantomeRose::setFreqRecalc(int n)
{
	_recalculeLeCheminTousLesXFois = n;
}

void FantomeRose::tentativeAmbuscade(int LignePacMan, Map &map, sf::Vector2f &posPacMan)
{
	std::deque<Direction> bonChemin;					//Contient les directions pour le bon chemin vers pacman
	std::vector<sf::Vector2f> pointsVisites;			//Les intersections visités  théoriquement par le fantome
	std::vector<std::deque<Direction>> tousBonsChemins;	//Tous les bons chemin obtenus dans le calcul	
	sf::Vector2f posTheorique = _pos;					//La position théorique du fantome
	char directionTheorique = _direction;				//La direction théorique du fantome
	int ligneParcoursTheorique = _numLigne;				//La ligne sur laquel le fantome se trouve théoriquement
	int distanceXYAvant = 0;	//Distance entre le fantome et pacman avant un déplacement théorique
	int distanceXYApres = 0;	//Distance entre le fantome et pacman apres un déplacement théorique
	pointsVisites.push_back(posTheorique);	//Pousse l'intersection du début et sa direction
	bonChemin.push_back(Direction(ligneParcoursTheorique, directionTheorique, 0, posTheorique, 0));
	int nombreEssais = 0;
	int compteurCheminCalculés = 0;
	int nbBoucle = 0;
	while (bonChemin.size() >= 1)
	{
		nbBoucle ++;
		/*compteurCheminCalculés++;
		if (tousBonsChemins.empty() &&compteurCheminCalculés > 70)
		{
			_clockFantomeRose.restart();
			_bougerCommeRouge = true;
			bougerCommeRouge(posPacMan, map);
			break;
		}*/
		//Calcul la distance avant le déplacement théorique pour pouvoir comparer avec après
		distanceXYAvant = abs((posTheorique.x - posPacMan.x)) + abs(posTheorique.y - posPacMan.y);
		//Si le déplacement rencontre les normes d'un bon déplacement (ne s'éloigne pas plus de 2 fois, est sur une ligne valide et cette direction n'a pas été tenter encore)
		if (nombreEssais < 4 && esseyerLigne(directionTheorique, ligneParcoursTheorique, pointsVisites, posTheorique, map, bonChemin.back()._direction) && bonChemin.back()._nbEloignement <= _nbEloignementPourAiRose)
		{	
			distanceXYApres = abs((posTheorique.x - posPacMan.x)) + abs(posTheorique.y - posPacMan.y);
			pointsVisites.push_back(posTheorique);
			bonChemin.back()._nbEssaie++;
			//Si le fantome s'éloigne de pacman avec cette direction, augmente le compteur d'éloignement
			if (distanceXYApres > distanceXYAvant)
				bonChemin.push_back(Direction(ligneParcoursTheorique, directionTheorique, 0, posTheorique, 1 + bonChemin.back()._nbEloignement));
			//Sinon push la direction normalement
			else
				bonChemin.push_back(Direction(ligneParcoursTheorique, directionTheorique, 0, posTheorique, 0));
			nombreEssais = 0;
			//Si le fantome se trouve sur la même ligne que pac-man, enregistre ce chemin
			if (ligneParcoursTheorique == LignePacMan || nbBoucle > 2000)
			{
				if (nbBoucle > 2000)
				{
					std::cout << "fantome rose probleme, boucle + 2000";
				}
				//tousBonsChemins.push_back(bonChemin);
				_chemin = bonChemin;
				break;
			}
		}
		//Sinon si les essais sont moins que 4, tourne vers la droite
		else if (nombreEssais < 4)
		{
			directionTheorique = tourner(directionTheorique);
			bonChemin.back()._direction = directionTheorique;
			bonChemin.back()._nbEssaie++;
			nombreEssais++;
		}
		//Sinon, revient en arrière le chemin n'était pas bon
		else
		{
			bonChemin.pop_back();
			pointsVisites.pop_back();
			if (bonChemin.empty())
			{
				if (tousBonsChemins.empty())
				{
					_direction = inverserDirection(_direction);
				}
				break;
			}
			nombreEssais = bonChemin.back()._nbEssaie;

			directionTheorique = bonChemin.back()._direction;
			directionTheorique = tourner(directionTheorique);
			ligneParcoursTheorique = bonChemin.back()._ligne;
			posTheorique = bonChemin.back()._posi;
			bonChemin.back()._direction = directionTheorique;
		}
	}

	//Détemrine le chemin le plus court si on voulait plus d'un chemin
	/*if (!_bougerCommeRouge)
	{
		int plusPetitVecteur = 1000;
		int index = 0;
		if (!tousBonsChemins.empty())
		{
			for (std::vector<std::deque<Direction>>::iterator i = tousBonsChemins.begin(); i != tousBonsChemins.end(); i++)
			{
				if ((*i).size() < plusPetitVecteur)
				{
					plusPetitVecteur = static_cast<int>((*i).size());
					index = static_cast<int>(i - tousBonsChemins.begin());
				}
			}
			_chemin = tousBonsChemins[index];
		}
	}*/
	
}
//Permet au fantome, à chaque intersection,  de décider quelle ligne il va prendre, en fonction de la position de pacMan
void FantomeRose::deciderLigne(sf::Vector2f& posPacMan, Map &map)
{
	if (_bougerCommeRouge && _clockFantomeRose.getElapsedTime() < sf::milliseconds(500))
	{
		//bougerCommeRouge(posPacMan, map);
	}
	else
	{
		_bougerCommeRouge = false;
		tentativeAmbuscade(map.quelleLigne(posPacMan, -10), map, posPacMan);
	}
	
}

void FantomeRose::move(char direction, sf::Vector2f& posPacMan, Map &map)
{
	Fantome::move(direction, posPacMan, map);

	if (_isDead)
	{
		fantomeDead(map, sf::Vector2f(600, 600));
		return;
	}

	if (_toucherParDragonshout)
	{
		fantomeDragonShouter(_posRecul, map, sf::Vector2f(600, 600));
		return;
	}

	Ligne temp = map.getLigne(_numLigne);
	sf::Vector2f vtemp(_pos);
	switch (direction)
	{
	case 'a':
		vtemp.x -= _vitesse;
		if (temp.isOn(vtemp))
		{
			_pos.x -= _vitesse;
		}
		else
		{
			//if (_vertical == false)
			setPosSecuritaire(temp.getDebut());
			if (!_chemin.empty() && _nombreDintersectionsPassee <= _recalculeLeCheminTousLesXFois)
			{
				_direction = _chemin.front()._direction;
				changerDeLigne(_direction, map);
				_chemin.pop_front();
				_nombreDintersectionsPassee++;
			}
			else
			{
				deciderLigne(posPacMan, map);
				_nombreDintersectionsPassee = 0;
			}
		}
		break;

	case 's':
		vtemp.y += _vitesse;
		if (temp.isOn(vtemp))
		{
			_pos.y += _vitesse;
		}
		else
		{
			//if (_vertical == true)
			setPosSecuritaire(temp.getFin());
			if (!_chemin.empty() && _nombreDintersectionsPassee <= _recalculeLeCheminTousLesXFois)
			{
				_direction = _chemin.front()._direction;
				changerDeLigne(_direction, map);
				_chemin.pop_front();
				_nombreDintersectionsPassee++;
			}
			else
			{
				deciderLigne(posPacMan, map);
				_nombreDintersectionsPassee = 0;
			}
		}
		break;

	case 'd':
		vtemp.x += _vitesse;
		if (temp.isOn(vtemp))
		{
			_pos.x += _vitesse;
		}
		else
		{
			//if (_vertical == false)
			setPosSecuritaire(temp.getFin());
			if (!_chemin.empty() && _nombreDintersectionsPassee <= _recalculeLeCheminTousLesXFois)
			{
				_direction = _chemin.front()._direction;
				changerDeLigne(_direction, map);
				_chemin.pop_front();
				_nombreDintersectionsPassee++;
			}
			else
			{
				deciderLigne(posPacMan, map);
				_nombreDintersectionsPassee = 0;
			}
		}
		break;

	case 'w':
		vtemp.y -= _vitesse;
		if (temp.isOn(vtemp))
		{
			_pos.y -= _vitesse;
		}
		else
		{
			//if (_vertical == true)
			setPosSecuritaire(temp.getDebut());
			if (!_chemin.empty() && _nombreDintersectionsPassee <= _recalculeLeCheminTousLesXFois)
			{
				_direction = _chemin.front()._direction;
				changerDeLigne(_direction, map);
				_chemin.pop_front();
				_nombreDintersectionsPassee++;
			}
			else
			{
				deciderLigne(posPacMan, map);
				_nombreDintersectionsPassee = 0;
			}
		}

		break;
	default:
		break;
	}
}

FantomeRose::~FantomeRose()
{
}