#include "FantomeRose.h"

FantomeRose::FantomeRose()
{
	_numLigne = 0;
	_vitesse = 3;
	_vertical = true;
	_direction = 's';

	_color = sf::Color(255, 192, 203, 255);

	_pos = sf::Vector2f(300, 300);

	//variables pour le dessin
	_headOffset = sf::Vector2f(0, -_width / 1.2);
	_feetOffset = sf::Vector2f(0, -_width / 2);
	_step = 0;
	_feetWidth = (float)_width / 4;
	_eyeSize = (float)_width / 3.5;
	_pupilSize = _eyeSize / 2;
}

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

bool FantomeRose::moveTheorique(char directionTheorique, int ligneTheorique, sf::Vector2f posTheorique, Map &map)
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

//change la ligne et la pos théorique et renvois vrai si c'est changé
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

	int tempDebutX = map.getLigne(ligneParcoursTheorique).getDebut().x - posTheorique.x;
	int tempDebutY = map.getLigne(ligneParcoursTheorique).getDebut().y - posTheorique.y;
	int tempFinX = map.getLigne(ligneParcoursTheorique).getFin().x - posTheorique.x;
	int tempFinY = map.getLigne(ligneParcoursTheorique).getFin().y - posTheorique.y;

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

void FantomeRose::tentativeAmbuscade(sf::Vector2f posPacMan, int LignePacMan, Map &map)
{
	std::deque<Direction> bonChemin;
	std::vector<sf::Vector2f> pointsVisites;						//Les lignes visités  théoriquement par le fantome
	std::vector<std::deque<Direction>> tousBonsChemins;
	sf::Vector2f posTheorique = _pos;
	char directionDepart = _direction;
	char directionTheorique = _direction;
	int ligneParcoursTheorique = _numLigne;
	pointsVisites.push_back(posTheorique);
	bonChemin.push_back(Direction(ligneParcoursTheorique, directionTheorique, 0, posTheorique));
	int nombreEssais = 0;
	bool boolTest = true;
	int compteur = 0;
	while (bonChemin.size() >= 1)
	{
		if (nombreEssais < 4 && esseyerLigne(directionTheorique, ligneParcoursTheorique, pointsVisites, posTheorique, map, bonChemin.back()._direction))
		{
			pointsVisites.push_back(posTheorique);
			bonChemin.back()._nbEssaie++;
			bonChemin.push_back(Direction(ligneParcoursTheorique, directionTheorique, 0, posTheorique));
			nombreEssais = 0;
			if (ligneParcoursTheorique == LignePacMan)
			{
				std::cout << "un chemin a ete pusher " << std::endl;
				tousBonsChemins.push_back(bonChemin);
			}
		}
		else if (nombreEssais < 4)
		{
			directionTheorique = tourner(directionTheorique);
			bonChemin.back()._direction = directionTheorique;
			bonChemin.back()._nbEssaie++;
			nombreEssais++;

		}
		else
		{
			bonChemin.pop_back();
			pointsVisites.pop_back();
			if (bonChemin.empty())
			{
				std::cout << "bon chemin vide   " << std::endl;
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
			//nombreEssais++;
			//bonChemin.back()._nbEssaie++;
			bonChemin.back()._direction = directionTheorique;
		}
		compteur++;
	}
	
	int plusPetitVecteur = 1000;
	int index = 0;
	if (!tousBonsChemins.empty())
	{
		for (std::vector<std::deque<Direction>>::iterator i = tousBonsChemins.begin(); i != tousBonsChemins.end(); i++)
		{

			if ((*i).size() < plusPetitVecteur)
			{
				plusPetitVecteur = (*i).size();
				index = i - tousBonsChemins.begin();
			}
		}
		_chemin = tousBonsChemins[index];
	}
	
}
//Permet au fantome, à chaque intersection,  de décider quelle ligne il va prendre, en fonction de la position de pacMan
void FantomeRose::deciderLigne(sf::Vector2f posPacMan, Map &map)
{
	char directionArrivee = _direction;			//La direction de départ
	int tempNoLigne = map.quelleLigne(_pos, _numLigne);				//Le numéro de la ligne du fantome au départ

	//Si c'est un cul de sac, retourne sur ses pas
	switch (_direction)
	{
	case 'a':
		tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x - 1, _pos.y), tempNoLigne);
		if (tempNoLigne == _numLigne)
		{
			tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y + 1), tempNoLigne);
			if (tempNoLigne == _numLigne)
			{
				tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y - 1), tempNoLigne);
				if (tempNoLigne == _numLigne)
				{
					_direction = inverserDirection(_direction);
					return;
				}
			}

		}
		break;
	case 'd':
		tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x + 1, _pos.y), tempNoLigne);
		if (tempNoLigne == _numLigne)
		{
			tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y + 1), tempNoLigne);
			if (tempNoLigne == _numLigne)
			{
				tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y - 1), tempNoLigne);
				if (tempNoLigne == _numLigne)
				{
					_direction = inverserDirection(_direction);
					return;
				}
			}

		}
		break;
	case 's':
		tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y + 1), tempNoLigne);
		if (tempNoLigne == _numLigne)
		{
			tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x + 1, _pos.y), tempNoLigne);
			if (tempNoLigne == _numLigne)
			{
				tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x - 1, _pos.y), tempNoLigne);
				if (tempNoLigne == _numLigne)
				{
					_direction = inverserDirection(_direction);
					return;
				}
			}

		}
		break;
	case 'w':
		tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y - 1), tempNoLigne);
		if (tempNoLigne == _numLigne)
		{
			tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x + 1, _pos.y), tempNoLigne);
			if (tempNoLigne == _numLigne)
			{
				tempNoLigne = map.quelleLigne(sf::Vector2f(_pos.x - 1, _pos.y), tempNoLigne);
				if (tempNoLigne == _numLigne)
				{
					_direction = inverserDirection(_direction);
					return;
				}
			}

		}
		break;
	default:
		break;
	}

	tentativeAmbuscade(posPacMan, map.quelleLigne(posPacMan, -10), map);

}

void FantomeRose::move(char direction, sf::Vector2f posPacMan,  Map &map)
{
	if (_isDead)
	{
		fantomeDead(map, posPacMan, sf::Vector2f(1000, 900));
		return;
	}
	//Personnage::move(direction, map);

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
				setPos(temp.getDebut());
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
				setPos(temp.getFin());
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
				setPos(temp.getFin());
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
				setPos(temp.getDebut());
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
