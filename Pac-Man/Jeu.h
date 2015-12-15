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
#include "FantomeOrange.h"
#include "FantomeBleu.h"
#include "PacMan.h"
#include "map.h"
#include "windowsAPIwrapper.h"
#include <random>
#include "algo.h"
#include <ctime>
#include "VecteurDeFantome.h"
#include <sfeMovie\Movie.hpp>
#include "Fruits.h"
#include <windows.h>
#include <list>
#include <iostream>
#include "Sons.h"
#include "UI.h"

enum mangeable
{
	boule = 1,
	fruit = 2,
	grosseBoule = 4,
	bouleRouge = 8
};
class Jeu
{
	sf::RenderWindow _window;		//Fenetre du jeu
	sf::Vector2i _defaultWinPos;	//Position de la fenetre du jeu par d�faut

	//Personnages
	PacMan _pacman;					//Personnage principal
	VecteurDeFantome _fantome;			//Fantomes

	//Carte
	Map _map;					//Carte du jeu
	sf::Vector2f _startpos;		//Position de depart

	sf::Text _dragonShoutLearned;

	sf::Event event;
	int _targetfps;
	float _shake;
	bool _playing = true;
	std::vector<sf::Vector2f> _posValides;    //Les positions valides dans la map
	std::vector<std::vector<int>> _mangeable; //Les boules � manger sur la map
	Fruits _fruits;							  //Les fruits dans la map
	std::clock_t _temps;					  //Sert � calculer le temps pass� dans le jeu
	bool _fermerHorloge = false;			  //Pour �viter de rouler plusieurs fois le script de x secondes
	sf::Time _tempsEntreLaserEtStop;		  //La temps pass� � faire le laser
	sf::Time _tempsEntreLaserEtStop2;		  //Le temps pass� � ne pas faire de laser
	int _nbFrame = 10;						  //Nombre de frame pour activer un action
	int _randColor1 = 255;					  //un 1er nombre pour une couleur random
	int _randColor2 = 255;					  //un 2e nombre pour une couleur random				  
	int _randColor3 = 255;					  //un 3e nombre pour une couleur random
	int _score = 0;							  //Le score
	int _nbBoulesTotal;						  //Le nombre de boules total dans la map
	int _nbBouleMange = 0;					  //Le nombre de boule mang�
	int _nbBouleRouge;						  //Le nombre de boule rouge
	int _nbFruitMange = 0;					  //Le nombre de fruit mang�, a chaque 3, donne un power up
	bool _megaDragonShout = false;			  //Un bool pour g�rer le dragonshout et ne pas incr�menter plus d'une fois le nb de dragonshout
	bool _dragonShoutDesactive = false;		  //D�sactive le dragon shout pendant le MEGA dragon shout
	bool _dragonShoutEffect = true;			  //Pour faire la shock wave qu'une seule fois dans le dragonShout
	sf::View _view;
	sf::Vector2f _viewVector;
	sf::Texture _explosionTextureComplet;
	sf::RectangleShape _explosionTexture[6][8];
	sf::IntRect _explosionTextureRect[6][8];
	int _vitesseExplosion = 0;				  //Dessiner l'explosion tous les 5 frames

	std::list<std::pair<std::string, int>> _maps;			  //Toutes les maps
	std::list<std::pair<std::string, int>>::const_iterator _mapsIterator;

	//LES SONS ET VID�O	
	Sons _sons;								//Conteneur pour les sons

	UI _ui;									//Conteneur pour le UI

	sfe::Movie _explosionNucleaire;			  //Video de l'explosion nucl�aire

	bool _nextMap;

public:
	Jeu(std::string map);
	~Jeu();

	void init();

	void draw(bool display = true);
	void donnerUnPowerUpPacman();
	void drawMangeable();

	void drawLaserUi();
	void drawDragonShoutUi();
	void drawEtoileUi();
	void play();

	void captureEvent();
	std::string getKeyPress();

	void pause(std::string msg = "Appuyez sur n'importe quel bouton pour continuer!");

	void shakeScreen();

	void killPacman();

	void clear();

	void readMaps(std::string maps);

	bool verifieSiMort(Fantome &fantome);
	
	//Choisi un position random pour le fruit
	sf::Vector2f choisirPosRandom();

	void loadMap();

	//Charge tous les sons dans l'objet de son
	void loadSounds();

	//Charge les explications
	void setTutorialText();

	//V�rifie que tout le monde est sur la bonne ligne
	void checkLines();

	//Charge toutes les animations dans la classe UI. C'est un peu l'�quivalent de balayer la poussi�re sous le tapis: au moins, �a PARAIS plus propre
	void loadAnimations();

	void loadTexts();
};
