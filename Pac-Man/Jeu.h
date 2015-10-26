/************************************************************************************
*Date:			25 Octobre 2015														*
*Programme:		Jeu.h																*
*Programmeur:	Maxime Michaud-Corriveau											*
*Description:	Classe principale du jeu de Pac-man. Contient tout ce dont on 		*
*				a besoin pour pouvoir lancer une partie de pac-man facilement.		*
*************************************************************************************/
#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "fantome.h"
#include "PacMan.h"
#include "map.h"

class Jeu
{
	sf::RenderWindow _window;	//Fenetre du jeu

	//Personnages
	PacMan _pacman;				//Personnage principal
	Fantome _fantome[4];		//Fantomes

	//Carte
	Map _map;					//Carte du jeu

public:
	Jeu();
	~Jeu();
};
