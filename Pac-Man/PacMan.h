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
#include <cmath>

class PacMan:public sf::Drawable
{
	mutable int step;
	mutable int stepIncrement;

public:
	PacMan();
	~PacMan();

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};

