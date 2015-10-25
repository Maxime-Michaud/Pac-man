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
#include "TimeTraveller.h"
#include "Personnage.h"

class Fantome : public Personnage
{
	//Variables pour les dessins====================================
	sf::Color _color;					//Couleur du fantome

	sf::Vector2f _headOffset;			//Décalage de la tête par rapport au centre
	sf::Vector2f _feetOffset;			//Décalage des pieds par rapport au centre

	static const int _width = 25;		//Largeur du fantome

	mutable int _step;					//Étape de l'animation a laquelle on est rendu
	static const int framePerStep = 8;	//Nombre de frame dessinés dans une étape

	//Nombres de vertex utilisés pour dessiner les cercles. Comme il s'agit de triangle, + gros chiffre = + rond
	static const int _smoothness = 25;	//Pour la tete. Demi cercle, donc on utilise la moitié du chiffre0.
	static const int _eyeSmooth = 8;	//Pour les yeux
	static const int _pupilSmooth = 8;	//Pour les pupilles

	float _eyeSize;						//Grosseur des yeux
	float _pupilSize;					//grosseur des pupilles
	float _feetWidth;					//Largeur des pieds

	//Fin des variables pour les dessins============================

	//Fonctions pour construire le fantome
	void buildHead(sf::VertexArray & vert) const;
	void buildBody(sf::VertexArray & vert) const;
	void buildFoot(sf::VertexArray & vert, bool right, float firstX) const;
	void buildFeet(sf::VertexArray & vert) const;
	void buildEye(sf::VertexArray & vert, sf::Vector2f eyePos) const;
	
	bool _isDead = false;	//Si le fantome est mort ou pas

public:
	Fantome();
	~Fantome();

	void setIsDead(bool isDead);
	void deadAnimation(Map & map, sf::Vector2f pacManPos);													 //L'animation et la placement du fantome quand il est mort
	bool verifieSiMort(sf::Vector2f coordPacMan, char pacManDirection, bool laser);//Vérifie le fantome est dans une situation lui causant la mort
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	virtual void move(char direction, sf::Vector2f posPacMan, Map &map);	 //Fait bouger le fantome
	void deciderLigne(sf::Vector2f posPacMan, Map &map);					 //Prend une décision de la direction à un intersection
};

