#include "FantomeRouge.h"

FantomeRouge::FantomeRouge()
{
	_color = sf::Color(255, 0, 0, 255);
	_nom = "rouge";
	_pointsPrecedents.push_back(sf::Vector2f(1, 1));
}

FantomeRouge::~FantomeRouge()
{
}

//Permet au fantome, à chaque intersection,  de décider quelle ligne il va prendre, en fonction de la position de pacMan
void FantomeRouge::deciderLigne(sf::Vector2f& posPacMan, Map &map)
{
	char directionArrivee = _direction;			//La direction de départ
	int tempNoLigne = _numLigne;				//Le numéro de la ligne du fantome au départ
	char gaucheDroite;							//Contient une direction logique à prendre entre la gauche ou la droite
	char basHaut;								//Contient une direction logique à prendre entre en haut ou en bas

	int distanceX = static_cast<int>(_pos.x - posPacMan.x);		//La distance de l'axe des X entre le fantome et pac man
	//Si la distance X est plus grande que 0, le fantome est à droite et doit donc se dirifer vers la gauche
	if (distanceX >= 0)
		gaucheDroite = 'a';
	//Sinon il est à gauche de pac man et doit aller vers la droite
	else
	{
		distanceX = static_cast<int>(posPacMan.x - _pos.x);
		gaucheDroite = 'd';
	}

	int distanceY = static_cast<int>(_pos.y - posPacMan.y);		//La distance de l'axe des Y entre le fantome et pac man
	//Si la distance Y est plus grande que 0, le fantome est à droite et doit donc se dirifer vers la gauche
	if (distanceY >= 0)
		basHaut = 'w';
	else	//Sinon c'est le contraire
	{
		distanceY = static_cast<int>(posPacMan.y - _pos.y);
		basHaut = 's';
	}

	//Si la distance a parcourir en X est supérieur, va tenter de prendre un chemin dans cette direction
	if (distanceX >= distanceY)
		_direction = gaucheDroite;
	else		//Sinon prendra une direction en Y pour sa rapprocher
		_direction = basHaut;

	if (_pointsPrecedents[0] == _pos && _direction == gaucheDroite)
	{
 		if (aPritUnMauvaisChemin)
		{
			if (basHaut == 'w')
				_direction = 's';
			else
				_direction = 'w';
		}
		else
		{
			_direction = basHaut;
		}
		aPritUnMauvaisChemin = false;
	}
	else if (_pointsPrecedents[0] == _pos && _direction == basHaut)
	{
		if (aPritUnMauvaisChemin)
		{
			if (gaucheDroite == 'a')
				_direction = 'd';
			else
				_direction = 'a';
		}
		else
		{
			_direction = gaucheDroite;
		}
   		
		aPritUnMauvaisChemin = false;
	}
	//Vérifie si il peut simplement prendre la 1er direction qui lui est donné, si oui, il sort de la fonction
	switch (_direction)
	{
	case 'a':
		if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x - 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x - 1, _pos.y))))
		{
			Personnage::changerDeLigne(_direction, map);
			return;
		}
		break;
	case 'd':
		if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x + 1, _pos.y)), _numLigne)).isOn((sf::Vector2f(_pos.x + 1, _pos.y))))
		{
			Personnage::changerDeLigne(_direction, map);
			return;
		}
		break;
	case 's':		
		if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y + 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y + 1))))
		{
			Personnage::changerDeLigne(_direction, map);
			return;
		}
		break;
	case 'w':
		if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y - 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y - 1))))
		{
			Personnage::changerDeLigne(_direction, map);
			return;
		}
		break;
	default:
		break;
	}

	if (distanceX >= distanceY)
		_direction = basHaut;
	else
		_direction = gaucheDroite;
	//Sinon il tente de prendre une autre ligne logique dans l'autre axe
	Personnage::changerDeLigne(_direction, map);
	//Si rien n'a fonctionné, il retourne sur ses pas
	if (_numLigne == tempNoLigne)
	{
		aPritUnMauvaisChemin = true;
		_direction = Fantome::inverserDirection(directionArrivee);	//Si rien n'a fonctionné, revient sur ses pas
	}
}

void FantomeRouge::move(char direction, sf::Vector2f& posPacMan, Map &map)
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
			deciderLigne(posPacMan, map);
			_pointsPrecedents.push_back(_pos);
			if (_pointsPrecedents.size() > 2)
			{
				_pointsPrecedents.pop_front();
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
			if (_vertical == true)
				setPos(temp.getFin());
			deciderLigne(posPacMan, map);
			_pointsPrecedents.push_back(_pos);
			if (_pointsPrecedents.size() > 2)
			{
				_pointsPrecedents.pop_front();
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
			if (_vertical == false)
				setPos(temp.getFin());
			deciderLigne(posPacMan, map);
			_pointsPrecedents.push_back(_pos);
			if (_pointsPrecedents.size() > 2)
			{
				_pointsPrecedents.pop_front();
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
			if (_vertical == true)
				setPos(temp.getDebut());
			deciderLigne(posPacMan, map);
			_pointsPrecedents.push_back(_pos);
			if (_pointsPrecedents.size() > 2)
			{
				_pointsPrecedents.pop_front();
			}
		}

		break;
	default:
		break;
	}
}