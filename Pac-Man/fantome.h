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
#include "Personnage.h"

class Fantome : public Personnage
{
	sf::Color _color;					//Couleur du fantome

	sf::Vector2f _headOffset;
	sf::Vector2f _feetOffset;

	//Variables pour les dessins====================================
	static const int _width = 20;

	mutable int _step;
	static const int framePerStep = 10;

	//Nombres de vertex utilisés pour dessiner les cercles. Comme il s'agit de triangle, + gros chiffre = + rond
	static const int _smoothness = 40;	//Pour la tete. Demi cercle, donc on utilise la moitié du chiffre0.
	static const int _eyeSmooth = 10;	//Pour les yeux

	void buildHead(sf::VertexArray & vert) const;
	void buildBody(sf::VertexArray & vert) const;
	void buildFoot(sf::VertexArray & vert, bool right, float firstX) const;
	void buildFeet(sf::VertexArray & vert) const;
	void buildEye(sf::VertexArray & vert, sf::Vector2f eyePos, float eyeWidth) const;


public:
	Fantome();
	~Fantome();

	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	virtual void move(char direction, sf::Vector2f posPacMan, Map &map);	//Fait bouger le fantome
	void deciderLigne(sf::Vector2f posPacMan, Map &map);					//Prend une décision de la direction à un intersection
};

