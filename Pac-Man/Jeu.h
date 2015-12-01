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
	sf::Vector2i _defaultWinPos;	//Position de la fenetre du jeu par défaut

	//Personnages
	PacMan _pacman;					//Personnage principal
	VecteurDeFantome _fantome;			//Fantomes

	//Carte
	Map _map;					//Carte du jeu
	sf::Vector2f _startpos;		//Position de depart
	sf::Vector2f _ghostStart;	//Position de depart des fantomes

	sf::Font _font;
	sf::Font _8bitFont;
	sf::Font _dragonFont;
	sf::Text _dragonShoutText;
	sf::Text _dragonShoutLearned;

	sf::Event event;
	int _targetfps;
	float _shake;
	bool _playing = true;
	std::vector<sf::Vector2f> _posValides;    //Les positions valides dans la map
	std::vector<std::vector<int>> _mangeable; //Les boules à manger sur la map
	Fruits _fruits;							  //Les fruits dans la map
	std::clock_t _temps;					  //Sert à calculer le temps passé dans le jeu
	bool _fermerHorloge = false;			  //Pour éviter de rouler plusieurs fois le script de x secondes
	sf::Text _laserText;					  //Le texte du ui du laser (Laser overdrive)
	sf::Time _tempsEntreLaserEtStop;		  //La temps passé à faire le laser
	sf::Time _tempsEntreLaserEtStop2;		  //Le temps passé à ne pas faire de laser
	int _nbFrame = 10;						  //Nombre de frame pour activer un action
	int _randColor1 = 255;					  //un 1er nombre pour une couleur random
	int _randColor2 = 255;					  //un 2e nombre pour une couleur random				  
	int _randColor3 = 255;					  //un 3e nombre pour une couleur random
	sf::Text _scoreTxt;						  //Le texte du score
	int _score = 0;							  //Le score
	int _nbBoulesTotal;						  //Le nombre de boules total dans la map
	int _nbBouleMange = 0;					  //Le nombre de boule mangé
	int _nbBouleRouge;						  //Le nombre de boule rouge
	int _nbFruitMange = 0;						  //Le nombre de fruit mangé, a chaque 3, donne un power up

	bool _dragonShoutEffect = true;				  //Pour faire la shock wave qu'une seule fois dans le dragonShout
	sf::View _view;
	sf::Vector2f _viewVector;
	sf::Texture _explosionTextureComplet;
	sf::RectangleShape _explosionTexture[6][8];
	sf::IntRect _explosionTextureRect[6][8];

	std::list<std::string> _maps;			  //Toutes les maps
	std::list<std::string>::const_iterator _mapsIterator;

	//LES SONS ET VIDÉO	
	sf::SoundBuffer _alarmBuffer;
	mutable sf::Sound _alarmSound;			  //Son de l'alarme quand le laser est trop utilisé
	mutable sf::Sound _intro;				  //Son de l'intro
	sf::SoundBuffer _introBuffer;
	mutable sf::Sound _chomp;				  //Son quand pac-man mange une boule
	sf::SoundBuffer _chompBuffer;
	mutable sf::Sound _fruit;				  //Son quand pac-man mange un fruit
	sf::SoundBuffer _fruitBuffer;
	mutable sf::Sound _mort;				  //Son quand pac-man meurt
	sf::SoundBuffer _mortBuffer;
	mutable sf::Sound _continue;			  //Son quand le joueur continue
	sf::SoundBuffer _continueBuffer;
	mutable sf::Sound _gg;					  //Son quand le joueur gagne un tableau
	sf::SoundBuffer _ggBuffer;
	sfe::Movie _explosionNucleaire;			  //Video de l'explosion nucléaire

	sf::SoundBuffer _megaDeadBuffer;		 
	sf::Sound _megaDead;					  //Son overkill de mort
	sf::SoundBuffer _starBuffer;
	sf::Sound _star;						  //Son overkill de mort
	sf::SoundBuffer _plopBuffer;
	sf::Sound _plop;						  //Son quand l'ennemie est touché par pacMan avec un étoile
	sf::SoundBuffer _alahuAkbarBuffer;
	sf::Sound _alahuAkbar;					  //Son quand un fantome touche une boule rouge
	sf::SoundBuffer _dragonLearnBuffer;
	sf::Sound _dragonLearned;				  //Son quand pac-man apprend un dragon shout

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
};
