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
#include "laser.h"
#include <SFML\Audio.hpp>

class PacMan : public Personnage
{
	mutable int _step;			//Nombre de vertex a ne pas afficher dans le dessin
	mutable int _stepIncrement;	//Nombre de vertex a ne pas afficher au prochain appel de draw()
	mutable float _deathCount;
	float _deathIncrement;

	sf::Color _color;	//Couleur de pac-man

	//Constantes du cercle
	static const int _radius = 30;
	static const int _nbrCote = 30;
	sf::Vector2f _centre;
	mutable bool _laser = false;		//True si pacman fait un laser
	mutable bool _keepFiring = false;
	sf::SoundBuffer _laserSB;
	mutable sf::Sound _laserSound;

	sf::VertexArray buildPacMan() const;
public:
	PacMan();
	~PacMan();

	//Fire et stop sont const pour pouvoir les utiliser dans sf::draw
	void fire() const;
	void stop() const;

	bool getLaser();
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	void deathAnimation(sf::RenderTarget & target) const;
	bool hasDisappeared() const;

	void input(char c);

	virtual void move(char direction, Map &map);
};
