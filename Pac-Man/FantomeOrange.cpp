#include "FantomeOrange.h"

FantomeOrange::FantomeOrange()
{
	_color = sf::Color(255, 127, 0, 255);
	_nom = "orange";
}

//Permet au fantome, à chaque intersection,  de décider quelle ligne il va prendre, en fonction de la position de pacMan
void FantomeOrange::deciderLigne(sf::Vector2f posPacMan, Map &map)
{
	char directionFantome = _direction;							//Contient une direction logique à prendre entre la gauche ou la droit

	if (_nbIntersectionPasse >= _nombreIntersectionAvantChangerDirection)
	{
		_nbIntersectionPasse = 0;
		int randDirection = rand() % 4 + 1;
		switch (randDirection)
		{
		case 1:
			directionFantome = 'a';
			break;
		case 2:
			directionFantome = 's';
			break;
		case 3:
			directionFantome = 'd';
			break;
		case 4:
			directionFantome = 'w';
			break;
		default:
			break;
		}
	}
	else

		//Vérifie si il peut prendre la 1er direction qui lui est donné, si oui, il sort de la fonction, sinon essais une direction proche de 90 degrés
		switch (directionFantome)
		{
		case 'a':
			_nbIntersectionPasse++;
			if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x - 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x - 1, _pos.y))))
			{
				Personnage::changerDeLigne(_direction, map);
				aPritUnMauvaisChemin = false;
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y - 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y - 1))))
			{
				Personnage::changerDeLigne('w', map);
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y + 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y + 1))))
			{
				Personnage::changerDeLigne('s', map);
				return;
			}
			else
			{
				_direction = inverserDirection(_direction);
				aPritUnMauvaisChemin = true;
				return;
			}
			break;
		case 'd':
			_nbIntersectionPasse++;
			if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x + 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x + 1, _pos.y))))
			{
				Personnage::changerDeLigne(_direction, map);
				aPritUnMauvaisChemin = false;
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y - 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y - 1))))
			{
				Personnage::changerDeLigne('w', map);
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y + 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y + 1))))
			{
				Personnage::changerDeLigne('s', map);
				return;
			}
			else
			{
				_direction = inverserDirection(_direction);
				aPritUnMauvaisChemin = true;
				return;
			}
			break;
		case 's':
			_nbIntersectionPasse++;
			if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y + 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y + 1))))
			{
				Personnage::changerDeLigne(_direction, map);
				aPritUnMauvaisChemin = false;
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x - 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x - 1, _pos.y))))
			{
				Personnage::changerDeLigne('a', map);
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x + 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x + 1, _pos.y))))
			{
				Personnage::changerDeLigne('d', map);
				return;
			}
			else
			{
				_direction = inverserDirection(_direction);
				aPritUnMauvaisChemin = true;
				return;
			}
			break;
		case 'w':
			_nbIntersectionPasse++;
			if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y - 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y - 1))))
			{
				Personnage::changerDeLigne(_direction, map);
				aPritUnMauvaisChemin = false;
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x - 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x - 1, _pos.y))))
			{
				Personnage::changerDeLigne('a', map);
				return;
			}
			else if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x + 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x + 1, _pos.y))))
			{
				Personnage::changerDeLigne('d', map);
				return;
			}
			else
			{
				_direction = inverserDirection(_direction);
				aPritUnMauvaisChemin = true;
				return;
			}
			break;
		default:
			break;
		}
}

void FantomeOrange::move(char direction, sf::Vector2f posPacMan, Map &map)
{
	if (_isDead)
	{
		fantomeDead(map, sf::Vector2f(600, 600));
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
			if (_vertical == false)
				setPos(temp.getDebut());
			deciderLigne(sf::Vector2f(0, 0), map);
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
			if (_vertical == true)
				setPos(temp.getFin());
			deciderLigne(sf::Vector2f(0, 0), map);
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
			if (_vertical == false)
				setPos(temp.getFin());
			deciderLigne(sf::Vector2f(0, 0), map);
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
			if (_vertical == true)
				setPos(temp.getDebut());
			deciderLigne(sf::Vector2f(0, 0), map);
		}

		break;
	default:
		break;
	}
}

FantomeOrange::~FantomeOrange()
{
}