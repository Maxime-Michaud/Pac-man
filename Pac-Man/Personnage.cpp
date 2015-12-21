/********************************************************************************
*Date:			16 Octobre 2015													*
*Programme:		Personnage.cpp													*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe de base pour des personnages.							*
*				Permet de:														*
*					-Dessiner des sprites qui loop								*
*					-Garder en mémoire la position								*
*					-Déplacer le personnage										*
*********************************************************************************/

#include "Personnage.h"

//Set tout a 0
Personnage::Personnage()
{
	_pos = sf::Vector2f(0, 0);
}

//Toutes les positions sont valides en théorie. Peut être surchargée par les enfants si on veut éviter certains cas particuliés
void Personnage::setPos(sf::Vector2f pos)
{
	_pos = pos;
}

//Set la position, mais vérifie si il se téléporte, ne doit pas se téléporter
void Personnage::setPosSecuritaire(sf::Vector2f pos)
{
	int x = _pos.x - pos.x;
	int y = _pos.y - pos.y;
	if (x > _vitesse + 1)
		pos.x = _pos.x - _vitesse;
	else if (x < -_vitesse - 1)
		pos.x = _pos.x + _vitesse;
	if (y > _vitesse + 1)
		pos.y = _pos.y - _vitesse;
	else if (y < -_vitesse - 1)
		pos.y = _pos.y + _vitesse;
	_pos = pos;
}

void Personnage::setLigne(unsigned int ligne)
{
	_numLigne = ligne;
}

void Personnage::setNumLigne(int numLigne)
{
	_numLigne = numLigne;
}

int Personnage::getNumLigne()
{
	return _numLigne;
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
	if (d != _direction)
		_change.push(_pos);

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

void Personnage::setVertical(bool v)
{
	_vertical = v;
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
bool Personnage::changerDeLigne(char direction, Map &map)
{
	int tempNoLigne = _numLigne;	//la ligne d'origine
	switch (direction)
	{
	case 'a':
	{
		//regarde si le pixel dans cette direction est sur une ligne et affecte cette ligne au No de ligne du personnage
		_numLigne = map.quelleLigne(sf::Vector2f(_pos.x - 1, _pos.y), _numLigne);
		if (_numLigne != tempNoLigne)	//Si il y a bien eut un changement de ligne
		{								//place le personnage dans cette ligne au début(ou à la fin) +(ou -) sa vitesse
			_direction = 'a';
			_vertical = false;
			_change.push(_pos);
			setPosSecuritaire(sf::Vector2f(map.getLigne(_numLigne).getFin().x - _vitesse, map.getLigne(_numLigne).getFin().y));
			return true;
		}
		break;
	}
	//Les autre cas suivent la même logique sauf que dans le cas de d par exemple, il se servira du début de la ligne
	//au lieu de la fin car une ligne est TOUJOURS dessinée de gauche à droite.
	case 'd':
	{
		_numLigne = map.quelleLigne(sf::Vector2f(_pos.x + 1, _pos.y), _numLigne);
		if (_numLigne != tempNoLigne)
		{
			_vertical = false;
			_direction = 'd';
			_change.push(_pos);
			setPosSecuritaire(sf::Vector2f(map.getLigne(_numLigne).getDebut().x + _vitesse, map.getLigne(_numLigne).getDebut().y));
			return true;
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
			_change.push(_pos);
			setPosSecuritaire(sf::Vector2f(map.getLigne(_numLigne).getDebut().x, map.getLigne(_numLigne).getDebut().y + _vitesse));
			return true;
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
			_change.push(_pos);
			setPosSecuritaire(sf::Vector2f(map.getLigne(_numLigne).getFin().x, map.getLigne(_numLigne).getFin().y - _vitesse));
			return true;
		}
		break;
	}
	default:
		break;
	}
	return false;
}

//Déplace le personnage de x, y
void Personnage::move(char direction, Map &map)
{
	Ligne temp = map.getLigne(_numLigne);	//La ligne de départ
	sf::Vector2f vtemp(_pos);
	switch (direction)
	{
	case 'a':
		vtemp.x -= _vitesse;
		if (temp.isOn(vtemp))				//Si le personnage est sur la ligne après son mouvement, avance
		{
			_direction = 'a';
			_pos.x -= _vitesse;
		}
		else								//Sinon regarde pour changer de ligne si l'axe correspond avec la direction
		{
			if (_vertical == false)
				setPosSecuritaire(temp.getDebut());
			char tempDirection = _direction;
			if (!changerDeLigne(_directionProchaine, map))
				changerDeLigne(tempDirection, map);
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
				setPosSecuritaire(temp.getFin());
			char tempDirection = _direction;
			if (!changerDeLigne(_directionProchaine, map))
				changerDeLigne(tempDirection, map);
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
				setPosSecuritaire(temp.getFin());
			char tempDirection = _direction;
			if (!changerDeLigne(_directionProchaine, map))
				changerDeLigne(tempDirection, map);
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
				setPosSecuritaire(temp.getDebut());
			char tempDirection = _direction;
			if (!changerDeLigne(_directionProchaine, map))
				changerDeLigne(tempDirection, map);
		}

		break;
	default:
		break;
	}
}

Personnage::~Personnage() {}

void Personnage::stopSounds()
{
	_sons.stopAll();
}

void Personnage::pause()
{
	_paused = true;
}

void Personnage::unpause()
{
	_paused = false;
}

void Personnage::goBack(Map map)
{
	sf::Vector2f destination = getPrevState(_pos);

	auto deplacement = destination - _pos;

	int vitesse = std::min(_vitesse,
					std::max<int>(std::abs(deplacement.x), std::abs(deplacement.y)));

	if (deplacement.x > 0)
		_direction = 'd';
	else if (deplacement.x < 0)
		_direction = 'a';
	else if (deplacement.y > 0)
		_direction = 'w';
	else
		_direction = 's';

	switch (_direction)
	{
	case 'd':
		_pos.x += vitesse;
		break;
	case 'a':
		_pos.x -= vitesse;
		break;
	case 's':
		_pos.y -= vitesse;
		break;
	case 'w':
		_pos.y += vitesse;
		break;
	default:
		break;
	}

	if (_direction == 'a')
	{
		_direction = 'd';
	}
	else if (_direction == 'd')
	{
		_direction = 'a';
	}

}
