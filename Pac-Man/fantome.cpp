#include "fantome.h"



Fantome::Fantome()
{
	//Variables pour l'animation
	_step = 0;
	_stepIncrement = 1;
	_numLigne = 1;
	_vitesse = 3;
	_vertical = true;
	_direction = 'w';
	_directionProchaine = 'a';

	_color = sf::Color::Cyan;

	_pos = sf::Vector2f(300, 300);
}


Fantome::~Fantome()
{
}


void Fantome::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//Centre du fantome
	sf::Vector2f pos;
	sf::RectangleShape body(sf::Vector2f(20, 20));
	body.setOrigin(sf::Vector2f(10, 10));
	body.setFillColor(_color);
	body.setPosition(_pos);
	target.draw(body);
}

char inverserDirection(char direction)
{
	switch (direction)
	{
	case 'a':
		return 'd';
		break;
	case 's':
		return 'w';
		break;
	case 'd':
		return 'a';
		break;
	case 'w':
		return 's';
		break;
	default:
		return direction;
		break;
	}
}

void Fantome::deciderLigne(sf::Vector2f posPacMan, Map &map)
{
	char directionArrivee = _direction;
	int tempNoLigne = _numLigne;
	char gaucheDroite;
	char basHaut;
	int distanceX = _pos.x - posPacMan.x;
	if (distanceX >= 0)
		gaucheDroite = 'a';
	else
	{
		distanceX = posPacMan.x - _pos.x;
		gaucheDroite = 'd';
	}

	int distanceY = _pos.y - posPacMan.y;
	if (distanceY >= 0)
		basHaut = 'w';
	else
	{
		distanceY = posPacMan.y - _pos.y;
		basHaut = 's';
	}
	
	if (distanceX >= distanceY)
		_direction = gaucheDroite;
	else
		_direction = basHaut;
	switch (_direction)
	{
	case 'a':
		if (map.getLigne(_numLigne).isOn(sf::Vector2f(_pos.x - 1, _pos.y)))
			return;
		break;
	case 'd':
		if (map.getLigne(_numLigne).isOn(sf::Vector2f(_pos.x + 1, _pos.y)))
			return;
		break;
	case 's':
		if (map.getLigne(_numLigne).isOn(sf::Vector2f(_pos.x, _pos.y + 1)))
			return;
		break;
	case 'w':
		if (map.getLigne(_numLigne).isOn(sf::Vector2f(_pos.x, _pos.y - 1)))
			return;
		break;
	default:
		break;
	}

	Personnage::changerDeLigne(_direction, map);
	if (tempNoLigne == _numLigne)
	{
		if (distanceX > distanceY)
			_direction = basHaut;
		else
			_direction = gaucheDroite;
	}
	else
		return;
	Personnage::changerDeLigne(_direction, map);
	if (tempNoLigne == _numLigne)
		_direction = inverserDirection(directionArrivee);
	else 
		return;
}

void Fantome::move(char direction, sf::Vector2f posPacMan, Map &map)
{

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
		}

		break;
	default:
		break;
	}
}