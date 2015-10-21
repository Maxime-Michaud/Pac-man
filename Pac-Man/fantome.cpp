#include "fantome.h"
#include <iostream>


Fantome::Fantome()
{
	_numLigne = 1;
	_vitesse = 3;
	_vertical = true;
	_direction = 'w';
	_directionProchaine = 'a';

	_color = sf::Color::Cyan;

	_pos = sf::Vector2f(300, 300);

	//variables pour le dessin
	_headOffset = sf::Vector2f(0, -_width / 1.5);
	_feetOffset = sf::Vector2f(0, -_width / 2);
	_step = 0;
}


Fantome::~Fantome()
{
}


//Fonctions pour dessiner le fantome

//Dessine un demi cercle pour la tête du fantome
void Fantome::buildHead(sf::VertexArray & vert) const
{
	sf::Vector2f pos = _pos;

	vert.append(sf::Vertex(_pos + _headOffset, _color));
	
	for (int i = 0; i < _smoothness /2; i++)
	{
		pos.x = _width  * cos(2 * (float)M_PI * (i - (float)_smoothness / 2) / (float)_smoothness) + _pos.x;
		pos.y = _width / 1.5 * sin(2 * (float)M_PI * (i - (float)_smoothness / 2) / (float)_smoothness) + _pos.y;

		//Positionne le troisième point du triangle
		vert.append(sf::Vertex(pos + _headOffset, _color));

		//Replace un point au centre pour dessiner le prochain triangle. Simule un trangleStrip
		vert.append(sf::Vertex(_pos + _headOffset, _color));
	}

}

//Dessine le corps du fantome
void Fantome::buildBody(sf::VertexArray & vert) const
{
	//Points du premier triangle: Coté droit de la tête, centre de la tête, coté en bas a droite du corps
	vert.append(sf::Vertex(sf::Vector2f(_pos.x + _width , _pos.y +_width) + _feetOffset, _color));

	//Points du deuxième triangle: centre de la tête, coté en bas a droite du corps, coté en bas a gauche du corps
	vert.append(sf::Vertex(sf::Vector2f(_pos.x - _width, _pos.y + _width) + _feetOffset, _color));

	//Replace le un point a la tete
	vert.append(sf::Vertex(_pos + _headOffset, _color));

	//Points du troisième triangle: Coté en bas a droite du corps, Centre de la tête, Coté gauche de la tête, 
	vert.append(sf::Vertex(sf::Vector2f(_pos.x - _width, _pos.y + _headOffset.y), _color));

}

void Fantome::buildFoot(sf::VertexArray & vert, bool right, float firstX) const
{
	float feetWidth = (float)_width / 2;

	sf::Vertex Last = vert[vert.getVertexCount() - 1];

	//Pour commencer, on atteint la position a laquelle on veut commencer.
	if (Last.position.x != firstX)
		vert.append(sf::Vertex(sf::Vector2f(firstX, Last.position.y),_color));

	//Garantie que le triangle sera disjoint des autres triangles sauf par ce point
	vert.append(sf::Vertex(sf::Vector2f(firstX, _pos.y + _width + _feetOffset.y), _color));

	vert.append(sf::Vertex(sf::Vector2f(firstX + right * feetWidth, _pos.y + _width), _color));
	vert.append(sf::Vertex(sf::Vector2f(firstX + feetWidth, _pos.y + _width + _feetOffset.y), _color));

}

//Dessine les pieds du fantome
void Fantome::buildFeet(sf::VertexArray & vert) const
{
	float feetWidth = (float)_width / 2;

	bool inverse = _step / framePerStep != 0;

	for (int i = 0; i < 4; i++, inverse = !inverse)
	{
		buildFoot(vert, inverse, _pos.x - _width + feetWidth * i);
	}
	
	_step++;

	_step %= framePerStep * 2;
}

void Fantome::buildEye(sf::VertexArray & vert, sf::Vector2f eyePos, float eyeWidth) const
{
	//Atteint la position des yeux
	sf::Vertex Last = vert[vert.getVertexCount() - 1];

	vert.append(sf::Vertex(eyePos, _color));
	vert.append(sf::Vertex(eyePos, _color));


	sf::Vector2f pos;

	for (int i = 1; i <= _eyeSmooth + 1; i++)
	{
		pos.x = eyeWidth * cos(2 * (float)M_PI * (i - 1) / _eyeSmooth) + eyePos.x;
		pos.y = eyeWidth * sin(2 * (float)M_PI * (i - 1) / _eyeSmooth) + eyePos.y;

		vert.append(sf::Vertex(pos, sf::Color::White));

		//Replace le centre
		vert.append(sf::Vertex(eyePos, sf::Color::White));
	}

	//Dessine un point au centre
	vert.append(sf::Vertex(eyePos, sf::Color::White));
	vert.append(sf::Vertex(eyePos + sf::Vector2f(-1, 2), sf::Color::Black));
	vert.append(sf::Vertex(eyePos + sf::Vector2f(-1, -2), sf::Color::Black));
	vert.append(sf::Vertex(eyePos + sf::Vector2f(1, -2), sf::Color::Black));
	vert.append(sf::Vertex(eyePos + sf::Vector2f(1, 2), sf::Color::Black));
	
	//Ressors de l'oeil
	vert.append(sf::Vertex(eyePos + sf::Vector2f(1, 3), sf::Color::White));
	vert.append(sf::Vertex(eyePos + sf::Vector2f(1, 3), sf::Color::White));


}

void Fantome::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::Vector2f eyeOffset(_width / 3,0);

	//Tableau de triangles pour dessiner le fantome.
	sf::VertexArray vert(sf::TrianglesStrip);

	buildHead(vert);
	buildBody(vert);
	buildFeet(vert);

	buildEye(vert, _pos + eyeOffset + _headOffset, 5);
	buildEye(vert, _pos - eyeOffset + _headOffset, 5);

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

//Permet au fantome, à chaque intersection,  de décider quelle ligne il va prendre, en fonction de la position de pacMan
void Fantome::deciderLigne(sf::Vector2f posPacMan, Map &map)
{
	char directionArrivee = _direction;			//La direction de départ
	int tempNoLigne = _numLigne;				//Le numéro de la ligne du fantome au départ
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

	//Sinon il tente de prendre une autre ligne logique dans l'autre axe
	Personnage::changerDeLigne(_direction, map);
	if (tempNoLigne == _numLigne)
	{
		if (distanceX >= distanceY)
			_direction = basHaut;
		else
			_direction = gaucheDroite;
	}
	else
		return;
	
	//Si rien n'a fonctionné, il retourne sur ses pas
	_direction = inverserDirection(directionArrivee);	//Si rien n'a fonctionné, revient sur ses pas
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