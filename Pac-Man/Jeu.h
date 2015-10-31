/************************************************************************************
*Date:			25 Octobre 2015														*
*Programme:		Jeu.h																*
*Programmeur:	Maxime Michaud-Corriveau											*
*Description:	Classe principale du jeu de Pac-man. Contient tout ce dont on 		*
*				a besoin pour pouvoir lancer une partie de pac-man facilement.		*
*************************************************************************************/
#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <string>
#include <vector>
#include <fstream>
#include "FantomeRouge.h"
#include "FantomeRose.h"
#include "PacMan.h"
#include "map.h"
#include "windowsAPIwrapper.h"
#include <random>		
#include "algo.h"
#include <thread>
class Jeu
{
	sf::RenderWindow _window;		//Fenetre du jeu
	sf::Vector2i _defaultWinPos;	//Position de la fenetre du jeu par d�faut

	//Personnages
	PacMan _pacman;					//Personnage principal
	std::vector<Fantome*> _fantome;			//Fantomes

	//Carte
	Map _map;					//Carte du jeu
	sf::Vector2f _startpos;		//Position de depart
	sf::Vector2f _ghostStart;	//Position de depart des fantomes


	sf::Font _font;

	int _targetfps;

	bool _playing = true;
public:
	Jeu(std::string map);
	~Jeu();

	void draw(bool display = true);

	void play();

	std::string getKeyPress();

	void pause(std::string msg = "Appuyez sur n'importe quel bouton pour continuer!");

	void shakeScreen();

	void killPacman();

	bool verifieSiMort(Fantome &fantome);

	std::thread buildMoveThread(Fantome *fantome);
};
