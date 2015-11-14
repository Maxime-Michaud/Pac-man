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
	static const int _radius = 10;
	static const int _nbrCote = 30;
	sf::Vector2f _centre;
	mutable bool _laser = false;		//True si pacman fait un laser
	mutable bool _keepFiring = false;
	sf::SoundBuffer _laserSB;
	mutable sf::Sound _laserSound;
	mutable sf::Clock _tempsLaser;						//Sert à calculer le temps depuis la partie du laser
	mutable sf::Clock _tempsSansLaser;
	sf::VertexArray buildPacMan() const;

	mutable bool _powerUpLaser = false;				  //Le power up du laser
	mutable bool _powerUpTimeTravel = false;		  //Le power up time traveller
	mutable bool _powerUpMindControl = false;		  //Le power up mind control
	mutable bool _powerUpMarioStar = false;			  //Le power up de l'étoile de mario

	//sf::Clock _clockLaser;

	mutable float _tempsPassePowerUpLaser;
	mutable float _tempsMemoireLaser = 0;
	mutable float _nbMilisecondeLaser = 0;

public:
	PacMan();
	~PacMan();

	//Fire et stop sont const pour pouvoir les utiliser dans sf::draw
	void fire()const;
	void stop()const;			

	bool getPowerUps(int);
	void setPowerUps(int numDuPowerUp, bool valeur);	//Set les power ups, 1= laser, 2=TimeTravel, 3=MindControl, 4=ÉtoileMario
	void changerTempsPowerUp(int numDuPowerUp, float valeur);		//Permet d'ajouter ou de supprimer du temps apparti a un power up
	sf::Time getTempsLaser();	//Retourne le temps en milisecondes passé a faire le laser
	sf::Time getTempsSansLaser();	//Retourne le temps en milisecondes passé a faire le laser
	float getTempsLaserRestant()const; //Retourne le temps restant au laser.
	bool getLaser();
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	void deathAnimation(sf::RenderTarget & target) const;
	bool hasDisappeared() const;

	void input(char c);

	virtual void move(char direction, Map &map);

	void respawn(sf::Vector2f pos);

	const int Width = _radius;
};
