/********************************************************************************
*Date:			16 Octobre 2015													*
*Programme:		Personnage.cpp													*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe de base pour des personnages.							*
*				Permet de:														*
*					-Dessiner des sprites qui loop								*
*					-Garder en m�moire la position								*
*					-D�placer le personnage										*
*********************************************************************************/

#include "Personnage.h"

//Set tout a 0
Personnage::Personnage()
{
	_pos = sf::Vector2f(0, 0);
}

//Toutes les positions sont valides en th�orie. Peut �tre surcharg�e par les enfants si on veut �viter certains cas particuli�s
void Personnage::setPos(sf::Vector2f pos)
{
	_pos = pos;
}

//Obtiens la position du personnage
sf::Vector2f Personnage::getPos() const
{
	return _pos;
}

char Personnage::getDirection()
{
	return _direction;
}

void Personnage::setDirection(char d)
{
	switch (d)
	{
	case 'w': case 's':
		if (_vertical)
			_direction = d;
		break;
	case 'a': case 'd':
		if (!_vertical)
			_direction = d;
		break;
	//Quitte la fonction si la direction n'est pas w a s d
	default:
		return;
	}

	_directionProchaine = d;
}

bool Personnage::getVertical()
{
	return _vertical;
}

void Personnage::setDirectionProchaine(char c)
{
	_directionProchaine = c;
}

char Personnage::getDirectionProchaine()
{
	return _directionProchaine;
}

//Valide et effectue un changement de ligne
void Personnage::changerDeLigne(char direction, Map &map)
{
	int tempNoLigne = _numLigne;	//la ligne d'origine
	switch (direction)
	{
	case 'a':
	{
		//regarde si le pixel dans cette direction est sur une ligne et affecte cette ligne au No de ligne du personnage
		_numLigne = map.quelleLigne(sf::Vector2f(_pos.x - 1, _pos.y), _numLigne);
		if (_numLigne != tempNoLigne)	//Si il y a bien eut un changement de ligne
		{								//place le personnage dans cette ligne au d�but(ou � la fin) +(ou -) sa vitesse
			_direction = 'a';
			_vertical = false;
			setPos(sf::Vector2f(map.getLigne(_numLigne).getFin().x - _vitesse, map.getLigne(_numLigne).getFin().y));
		}
		break;
	}
	//Les autre cas suivent la m�me logique sauf que dans le cas de d par exemple, il se servira du d�but de la ligne
	//au lieu de la fin car une ligne est TOUJOURS dessin�e de gauche � droite.
	case 'd':
	{
		_numLigne = map.quelleLigne(sf::Vector2f(_pos.x + 1, _pos.y), _numLigne);
		if (_numLigne != tempNoLigne)
		{
			_vertical = false;
			_direction = 'd';
			setPos(sf::Vector2f(map.getLigne(_numLigne).getDebut().x + _vitesse, map.getLigne(_numLigne).getDebut().y));
		}

		break;
	}
	case 's':
	{
		_numLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y + 1), _numLigne);
		if (_numLigne != tempNoLigne)
		{
			_vertical = true;
			_direction = 's';
			setPos(sf::Vector2f(map.getLigne(_numLigne).getDebut().x, map.getLigne(_numLigne).getDebut().y + _vitesse));
		}
		break;
	}
	case 'w':
	{
		_numLigne = map.quelleLigne(sf::Vector2f(_pos.x, _pos.y - 1), _numLigne);
		if (_numLigne != tempNoLigne)
		{
			_vertical = true;
			_direction = 'w';
			setPos(sf::Vector2f(map.getLigne(_numLigne).getFin().x, map.getLigne(_numLigne).getFin().y - _vitesse));
		}
		break;
	}
	default:
		break;
	}
}

//D�place le personnage de x, y
void Personnage::move(char direction, Map &map)
{
	Ligne temp = map.getLigne(_numLigne);	//La ligne de d�part
	sf::Vector2f vtemp(_pos);
	switch (direction)
	{
	case 'a':
		vtemp.x -= _vitesse;
		if (temp.isOn(vtemp))				//Si le personnage est sur la ligne apr�s son mouvement, avance
		{
			_direction = 'a';
			_pos.x -= _vitesse;
		}
		else								//Sinon regarde pour changer de ligne si l'axe correspond avec la direction
		{
			if (_vertical == false)
				setPos(temp.getDebut());
			changerDeLigne(_directionProchaine, map);
		}
		break;

	case 's':
		vtemp.y += _vitesse;
		if (temp.isOn(vtemp))
		{
			_direction = 's';
			_pos.y += _vitesse;
		}
		else
		{
			if (_vertical == true)
				setPos(temp.getFin());
			changerDeLigne(_directionProchaine, map);
		}
		break;

	case 'd':
		vtemp.x += _vitesse;
		if (temp.isOn(vtemp))
		{
			_direction = 'd';
			_pos.x += _vitesse;
		}
		else
		{
			if (_vertical == false)
				setPos(temp.getFin());
			changerDeLigne(_directionProchaine, map);
		}
		break;

	case 'w':
		vtemp.y -= _vitesse;
		if (temp.isOn(vtemp))
		{
			_direction = 'w';
			_pos.y -= _vitesse;
		}
		else
		{
			if (_vertical == true)
				setPos(temp.getDebut());
			changerDeLigne(_directionProchaine, map);
		}

		break;
	default:
		break;
	}
}

Personnage::~Personnage() {}