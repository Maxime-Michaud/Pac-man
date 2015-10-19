#include "PacMan.h"



PacMan::PacMan()
{
	//Variables pour l'animation
	_step = 0;
	_stepIncrement = 1;

	_color = sf::Color(255, 255, 0, 255);

	_centre = sf::Vector2f(300, 300);
}


PacMan::~PacMan()
{
}


void PacMan::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//Centre du cercle
	sf::Vector2f pos;

	//nombre de points : le centre + 1 par coté + 1 pour aller rejoindre le premier point et fermer le cercle
	sf::Vertex vertices[_nbrCote + 2];

	vertices[0] = sf::Vertex(_centre, _color);
	
	for (int i = 1; i <= _nbrCote + 1; i++)
	{
		pos.x = _radius * cos(2 * M_PI * (i - 1) / _nbrCote) + _centre.x;
		pos.y = _radius * sin(2 * M_PI * (i - 1) / _nbrCote) + _centre.y;

		vertices[i] = sf::Vertex(pos, _color);
	}


	//Fais ouvrir et fermet la bouche de pac-man
	for (int i = 0; i < _step; i++)
		vertices[i + 1] = vertices[_nbrCote + 1 - i] = _centre;

	_step += _stepIncrement;

	if (_step == 0 || _step >= _nbrCote / 4 )
		_stepIncrement *= -1;

	target.draw(vertices, _nbrCote + 2, sf::TrianglesFan);

}


void PacMan::move(float x, float y)
{
	_centre.x += x;
	_centre.y += y;
}

void PacMan::move(sf::Vector2f mov)
{
	_centre += mov;
}