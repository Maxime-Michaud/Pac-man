/********************************************************************************
*Date:			16 octobre 2015													*
*Programme:		PacMan.h														*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Fantome du jeu de Pac-Man.										*
*				Il s'agit d'un personnage qui se déplace selon les mêmes règles	*
*				que pac-man, mais qui est controlé par l'ordinateur. Si il		*
*				touche pac-man, le joueur perd.									*
*********************************************************************************/
#pragma once
#define _USE_MATH_DEFINES
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "TimeTraveller.h"
#include "Personnage.h"
#include "map.h"
#include <deque>

class Fantome : public Personnage
{
protected:
	//Variables pour les dessins====================================
	std::string _nom;
	sf::Color _color;					//Couleur du fantome

	sf::Vector2f _headOffset;			//Décalage de la tête par rapport au centre
	sf::Vector2f _feetOffset;			//Décalage des pieds par rapport au centre

	static const int _width = 9;		//Largeur du fantome

	mutable int _step;					//Étape de l'animation a laquelle on est rendu
	static const int framePerStep = 8;	//Nombre de frame dessinés dans une étape

	//Nombres de vertex utilisés pour dessiner les cercles. Comme il s'agit de triangle, + gros chiffre = + rond
	static const int _smoothness = 25;	//Pour la tete. Demi cercle, donc on utilise la moitié du chiffre0.
	static const int _eyeSmooth = 8;	//Pour les yeux
	static const int _pupilSmooth = 8;	//Pour les pupilles

	double _eyeSize;						//Grosseur des yeux
	double _pupilSize;					//grosseur des pupilles
	float _feetWidth;					//Largeur des pieds

	//Fin des variables pour les dessins============================

	//Fonctions pour construire le fantome
	void buildHead(sf::VertexArray & vert) const;
	void buildBody(sf::VertexArray & vert) const;
	void buildFoot(sf::VertexArray & vert, bool right, float firstX) const;
	void buildFeet(sf::VertexArray & vert) const;
	void buildEye(sf::VertexArray & vert, sf::Vector2f eyePos) const;

	bool _isDead = false;				//Si le fantome est mort ou pas
	bool aPritUnMauvaisChemin = false;	//Si le fantome a prit un mauvais chemin
	bool _alahuAkbar = false;
	sf::Clock _clockAlahhuAkbar;

	sf::SoundBuffer _explosionBuffer;
	sf::Sound _explosion;				//Le son d'explosion


public:
	Fantome();
	~Fantome();

	const int Width = _width;

	void resetClockAlahuAkbar();
	void setPowerUp(int nbPowerUp, bool valeur);
	std::string getNom();	//Renvois le nom du fantome(nommer par sa couleur)
	char inverserDirection(char direction);
	void setIsDead(bool isDead);
	void fantomeDead(Map & map, sf::Vector2f window);													 //L'animation et la placement du fantome quand il est mort
	bool isDead() const;	//Obtiens si le fantome est mort ou vivant
	sf::VertexArray getVertexArray()const;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	virtual void move(char direction, sf::Vector2f& posPacMan, Map &map) = 0;	 //Fait bouger le fantome
	virtual void deciderLigne(sf::Vector2f posPacMan, Map &map);					 //Prend une décision de la direction à un intersection
};
