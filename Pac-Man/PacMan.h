/********************************************************************************
*Date:			16 octobre 2015													*
*Programme:		PacMan.h														*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Personnage principal du jeu Pac-Man.							*
TODO descriptionner mieux que ça												*
*																				*
*																				*
*																				*
*********************************************************************************/
#pragma once
#define _USE_MATH_DEFINES 
#include <SFML\Graphics.hpp>
#include "TimeTraveller.h"
#include <cmath>

class PacMan:public sf::Drawable
{
	mutable int _step;			//Nombre de vertex a ne pas afficher dans le dessin
	mutable int _stepIncrement;	//Nombre de vertex a ne pas afficher au prochain appel de draw()

	sf::Color _color;	//Couleur de pac-man

	//Constantes du cercle
	static const int _radius = 50;
	static const int _nbrCote = 50;
	sf::Vector2f _centre;

public:
	PacMan();
	~PacMan();

	void move(float x, float y);
	void move(sf::Vector2f mov);

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};

