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
#include "dragonShout.h"
#include <map>
#include <SFML\Audio.hpp>
#include "Sons.h"

class PacMan : public Personnage
{
	mutable int _step;			//Nombre de vertex a ne pas afficher dans le dessin
	mutable int _stepIncrement;	//Nombre de vertex a ne pas afficher au prochain appel de draw()
	mutable float _deathCount;
	float _deathIncrement;

	mutable bool _invincible = false;		//Si pac-man est invincible ou non
	sf::Color _color;	//Couleur de pac-man

	//Constantes du cercle
	static const int _radius = 10;
	static const int _nbrCote = 30;
	sf::Vector2f _centre;
	mutable bool _laser = false;		//True si pacman fait un laser
	mutable bool _keepFiring = false;
	mutable sf::Clock _tempsLaser;						//Sert à calculer le temps depuis la partie du laser
	mutable sf::Clock _tempsSansLaser;
	sf::VertexArray buildPacMan() const;

	std::map<std::string, int>_powerUpNames;

	mutable bool _powerUpLaser = false;				  //Le power up du laser
	mutable bool _powerUpTimeTravel = false;		  //Le power up time traveller
	mutable bool _powerUpMindControl = false;		  //Le power up mind control
	mutable bool _powerUpMarioStar = false;			  //Le power up de l'étoile de mario
	mutable bool _powerUpDragonShout = false;		  //Le power up du dragonshout FUS ROH DAH

	//sf::Clock _clockLaser;

	mutable float _tempsPassePowerUpLaser;
	mutable float _tempsMemoireLaser = 0;
	mutable float _nbMilisecondeLaser = 0;

	//pour l'étoile de mario
	mutable float _nbMilisecondeEtoile = 0;
	sf::Clock _clockEtoile;
	int _compteurFrame = 0;
	mutable bool _stopRepeating = false;

	sf::Clock _appuyerBouton;

	//Pour le power up dragonshout
	dragonShout _dragonShout;
	int _nbDragonShout = 0;							//Le nombre de dragon shou disponible
	sf::Clock _clockDragonShout;						//Un délais de 1500 ms entre chaque dragon shout
	bool _dragonShoutActivated = false;
	bool _dragonShoutAnimation = false;
public:
	PacMan();
	~PacMan();

	//Fire et stop sont const pour pouvoir les utiliser dans sf::draw
	void fire()const;
	void stop()const;			

	bool getInvincible();
	void setInvincible(bool valeur);

	void startClockEtoile();

	bool getPowerUps(int);
	void setPowerUps(int numDuPowerUp, bool valeur);	//Set les power ups, 1= laser, 2=TimeTravel, 3=MindControl, 4=ÉtoileMario
	void setPowerUps(std::string nomDuPowerUp, bool valeur);	//Set les power ups, 1= laser, 2=TimeTravel, 3=MindControl, 4=ÉtoileMario
	 
	void changerTempsPowerUp(int numDuPowerUp, float valeur);		//Permet d'ajouter ou de supprimer du temps apparti a un power up
	void changerTempsPowerUp(std::string numDuPowerUp, float valeur);		//Permet d'ajouter ou de supprimer du temps apparti a un power up
	float getTempsEtoile();		//Rdetourne le temps restant de l'étoile
	void setCouleurRandom();	//Set la couleur de pac-man au hasard
	void setNormalStat();//reset les stats à la normal
	sf::Time getTempsLaser();	//Retourne le temps en milisecondes passé a faire le laser
	sf::Time getTempsSansLaser();	//Retourne le temps en milisecondes passé a faire le laser
	float getTempsLaserRestant()const; //Retourne le temps restant au laser.
	bool getLaser();
	void incrementeurDragonShout(int increment);
	int getNbDragonShout();
	void resetClockDragon();
	bool getDragonShoutActivated();
	float getTempsDragonShout();	//Retourne le temps écoulé depuis l'activation du dragonShout

	bool getDragonAnimation();		//Retourne la valeur qui pemret de jouer l'animation
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	void deathAnimation(sf::RenderTarget & target) const;
	bool hasDisappeared() const;

	void input(char c);

	virtual void move(char direction, Map &map);

	void respawn(sf::Vector2f pos);

	const int Width = _radius;

	void resetPowerUps();

	virtual void loadSounds();
};
