#include "fantome.h"
#include <iostream>

Fantome::Fantome()
{
	_numLigne = 0;
	_vitesse = 3;
	_vertical = true;
	_direction = 's';

	_color = sf::Color::Cyan;

	_pos = sf::Vector2f(300, 300);

	//variables pour le dessin
	_headOffset = sf::Vector2f(0, -_width / 1.2);
	_feetOffset = sf::Vector2f(0, -_width / 2);
	_step = 0;
	_feetWidth = (float)_width / 4;
	_eyeSize = (float)_width / 3.5;
	_pupilSize = _eyeSize / 2;
}

Fantome::~Fantome()
{
}

void Fantome::setIsDead(bool isDead)
{
	_isDead = isDead;
}

//Fonctions pour dessiner le fantome

//Dessine un demi cercle pour la tête du fantome
void Fantome::buildHead(sf::VertexArray & vert) const
{
	sf::Vector2f pos;

	//On simule un triangleFan a l'aide de plusieurs triangles
	for (int i = 0; i < _smoothness / 2 + _smoothness % 2; i++)
	{
		//Premier point du triangle
		pos.x = _width  * cos(2 * (float)M_PI * (i - (float)_smoothness / 2) / (float)_smoothness) + _pos.x;
		pos.y = _width / 1.5 * sin(2 * (float)M_PI * (i - (float)_smoothness / 2) / (float)_smoothness) + _pos.y;
		vert.append(sf::Vertex(pos + _headOffset, _color));

		//Second point du triangle
		pos.x = _width  * cos(2 * (float)M_PI * (i + 1 - (float)_smoothness / 2) / (float)_smoothness) + _pos.x;
		pos.y = _width / 1.5 * sin(2 * (float)M_PI * (i + 1 - (float)_smoothness / 2) / (float)_smoothness) + _pos.y;
		vert.append(sf::Vertex(pos + _headOffset, _color));

		//Place le poind central du triangle
		vert.append(sf::Vertex(_pos + _headOffset, _color));
	}
}

//Dessine le corps du fantome
void Fantome::buildBody(sf::VertexArray & vert) const
{
	/*Triangle 1:
		*****
		-****
		--***
		---**
		----*
	   Triangle 2
		*----
		**---
		***--
		****-
		*****
		*/

		//Triangle 1
		//Coin haut droit
	vert.append(sf::Vertex(sf::Vector2f(_pos.x + _width, _pos.y + _headOffset.y), _color));

	//Coin bas droit
	vert.append(sf::Vertex(sf::Vector2f(_pos.x + _width, _pos.y + _feetOffset.y + _width), _color));

	//Coin haut gauche
	vert.append(sf::Vertex(sf::Vector2f(_pos.x - _width, _pos.y + _headOffset.y), _color));

	//Triangle 2
	//Coin bas droit
	vert.append(sf::Vertex(sf::Vector2f(_pos.x + _width, _pos.y + _feetOffset.y + _width), _color));

	//Coin haut gauche
	vert.append(sf::Vertex(sf::Vector2f(_pos.x - _width, _pos.y + _headOffset.y), _color));

	//Coin bas gauche
	vert.append(sf::Vertex(sf::Vector2f(_pos.x - _width, _pos.y + _feetOffset.y + _width), _color));
}

void Fantome::buildFoot(sf::VertexArray & vert, bool right, float firstX) const
{
	sf::Vertex Last = vert[vert.getVertexCount() - 1];

	//Dessine le premier point
	vert.append(sf::Vertex(sf::Vector2f(firstX, _pos.y + _feetOffset.y + _width), _color));

	//Dessine le second point
	vert.append(sf::Vertex(sf::Vector2f(firstX + _feetWidth, _pos.y + _feetOffset.y + _width), _color));

	//Dessine le troisième point
	vert.append(sf::Vertex(sf::Vector2f(firstX + _feetWidth * right, _pos.y + _width), _color));
}

//Dessine les pieds du fantome
void Fantome::buildFeet(sf::VertexArray & vert) const
{
	bool inverse = _step / framePerStep != 0;

	for (int i = 0; i < 2 * _width / (int)_feetWidth; i++, inverse = !inverse)
	{
		buildFoot(vert, inverse, _pos.x - _width + _feetWidth * i);
	}

	_step++;

	_step %= framePerStep * 2;
}

void Fantome::buildEye(sf::VertexArray & vert, sf::Vector2f eyePos) const
{
	sf::Vector2f pos;

	//Fait le blanc des yeux

	for (int i = 0; i <= _eyeSmooth; i++)	//Simule un triangleFan
	{
		//Replace le dernier vertex
		pos.x = _eyeSize * cos(2 * (float)M_PI * (i - 1) / _eyeSmooth) + eyePos.x;
		pos.y = _eyeSize * sin(2 * (float)M_PI * (i - 1) / _eyeSmooth) + eyePos.y;
		vert.append(sf::Vertex(pos, sf::Color::White));

		//Repositionne le centre
		vert.append(sf::Vertex(eyePos, sf::Color::White));

		//Position du nouveau vertex
		pos.x = _eyeSize * cos(2 * (float)M_PI * i / _eyeSmooth) + eyePos.x;
		pos.y = _eyeSize * sin(2 * (float)M_PI * i / _eyeSmooth) + eyePos.y;

		vert.append(sf::Vertex(pos, sf::Color::White));
	}

	//Fais le noir des yeux
	sf::Vector2f pupilOffset;
	switch (_direction)
	{
	case 'a':
		pupilOffset = sf::Vector2f(-(_eyeSize - _pupilSize), 0);
		break;
	case 's':
		pupilOffset = sf::Vector2f(0, _eyeSize - _pupilSize);
		break;
	case 'w':
		pupilOffset = sf::Vector2f(0, -(_eyeSize - _pupilSize));
		break;
	case 'd':
		pupilOffset = sf::Vector2f(_eyeSize - _pupilSize, 0);
		break;
	}

	for (int i = 0; i <= _pupilSmooth; i++)	//Simule un triangleFan
	{
		//Replace le dernier vertex
		pos.x = _pupilSize * cos(2 * (float)M_PI * (i - 1) / _pupilSmooth) + eyePos.x + pupilOffset.x;
		pos.y = _pupilSize * sin(2 * (float)M_PI * (i - 1) / _pupilSmooth) + eyePos.y + pupilOffset.y;
		vert.append(sf::Vertex(pos, sf::Color::Black));

		//Repositionne le centre
		vert.append(sf::Vertex(eyePos + pupilOffset, sf::Color::Black));

		//Position du nouveau vertex
		pos.x = _pupilSize * cos(2 * (float)M_PI * i / _pupilSmooth) + eyePos.x + pupilOffset.x;
		pos.y = _pupilSize * sin(2 * (float)M_PI * i / _pupilSmooth) + eyePos.y + pupilOffset.y;

		vert.append(sf::Vertex(pos, sf::Color::Black));
	}
}

void Fantome::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::Vector2f eyeOffset(_width / 3, 0);

	//Tableau de triangles pour dessiner le fantome.
	sf::VertexArray vert(sf::Triangles);

	(_isDead != true) ? buildHead(vert) : NULL;
	(_isDead != true) ? buildBody(vert) : NULL;
	(_isDead != true) ? buildFeet(vert) : NULL;

	buildEye(vert, _pos + eyeOffset + _headOffset);
	buildEye(vert, _pos - eyeOffset + _headOffset);

	target.draw(vert);
}

//Inverse la direction donnée
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

//L'animation et la placement du fantome quand il est mort
void Fantome::deadAnimation(Map &map, sf::Vector2f pacManPos)
{
	if (_pos != sf::Vector2f(300, 300))
	{
		_direction = 's';
		(_pos.x > 300) ? _pos.x -= 1 : (_pos.x < 300) ? _pos.x += 1 : NULL;
		(_pos.y > 300) ? _pos.y -= 1 : (_pos.y < 300) ? _pos.y += 1 : NULL;
	}
	else
	{
		_numLigne = 2;
		_vertical = true;
		_isDead = false;
	}
}

//Permet au fantome, à chaque intersection,  de décider quelle ligne il va prendre, en fonction de la position de pacMan
void Fantome::deciderLigne(sf::Vector2f posPacMan, Map &map)
{
	char directionArrivee = _direction;			//La direction de départ
	int tempNoLigne = map.quelleLigne(_pos, _numLigne);				//Le numéro de la ligne du fantome au départ
	char gaucheDroite;							//Contient une direction logique à prendre entre la gauche ou la droite
	char basHaut;								//Contient une direction logique à prendre entre en haut ou en bas

	int distanceX = _pos.x - posPacMan.x;		//La distance de l'axe des X entre le fantome et pac man
	//Si la distance X est plus grande que 0, le fantome est à droite et doit donc se dirifer vers la gauche
	if (distanceX >= 0)
		gaucheDroite = 'a';
	//Sinon il est à gauche de pac man et doit aller vers la droite
	else
	{
		distanceX = posPacMan.x - _pos.x;
		gaucheDroite = 'd';
	}

	int distanceY = _pos.y - posPacMan.y;		//La distance de l'axe des Y entre le fantome et pac man
	//Si la distance Y est plus grande que 0, le fantome est à droite et doit donc se dirifer vers la gauche
	if (distanceY >= 0)
		basHaut = 'w';
	else	//Sinon c'est le contraire
	{
		distanceY = posPacMan.y - _pos.y;
		basHaut = 's';
	}

	//Si la distance a parcourir en X est supérieur, va tenter de prendre un chemin dans cette direction
	if (distanceX >= distanceY)
		_direction = gaucheDroite;
	else		//Sinon prendra une direction en Y pour sa rapprocher
		_direction = basHaut;

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
	case 's':		if (map.getLigne(map.quelleLigne((sf::Vector2f(_pos.x, _pos.y + 1)), _numLigne)).isOn((sf::Vector2f(_pos.x, _pos.y + 1))))
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
		_direction = inverserDirection(directionArrivee);	//Si rien n'a fonctionné, revient sur ses pas
	}
}

void Fantome::move(char direction, sf::Vector2f posPacMan, Map &map)
{
	if (_isDead)
	{
		deadAnimation(map, posPacMan);
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