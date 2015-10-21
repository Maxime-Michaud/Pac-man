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
#include "Personnage.h"

class PacMan: public Personnage
{
	mutable int _step;			//Nombre de vertex a ne pas afficher dans le dessin
	mutable int _stepIncrement;	//Nombre de vertex a ne pas afficher au prochain appel de draw()

	sf::Color _color;	//Couleur de pac-man

	//Constantes du cercle
	static const int _radius = 30;
	static const int _nbrCote = 30;
	sf::Vector2f _centre;

public:
	PacMan();
	~PacMan();

	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	virtual void move(char direction, Map &map);
};

