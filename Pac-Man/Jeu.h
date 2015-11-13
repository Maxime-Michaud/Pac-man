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

enum mangeable
{
	boule = 1,
	fruit = 2,
	grosseBoule = 4
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
	sf::Vector2f _ghostStart;	//Position de depart des fantomes

	sf::Font _font;
	sf::Font _8bitFont;

	int _targetfps;
	float _shake;
	bool _playing = true;
	std::vector<sf::Vector2f> _posValides;    //Les positions valides dans la map
	std::vector<std::vector<int>> _mangeable; //Les boules � manger sur la map
	Fruits _fruits;							  //Les fruits dans la map
	std::clock_t _temps;					  //Sert � calculer le temps pass� dans le jeu
	bool _fermerHorloge = false;			  //Pour �viter de rouler plusieurs fois le script de x secondes
	sf::Text _laserText;					  //Le texte du ui du laser (Laser overdrive)
	sf::Time _tempsEntreLaserEtStop;		  //La temps pass� � faire le laser
	sf::Time _tempsEntreLaserEtStop2;		  //Le temps pass� � ne pas faire de laser
	sf::Text _scoreTxt;
	int _score = 0;
	int _nbBoulesTotal;						  //Le nombre de boules total dans la map
	int _nbBouleMange = 0;					  //Le nombre de boule mang�

	//LES SONS ET VID�O	
	sf::SoundBuffer _alarmBuffer;
	mutable sf::Sound _alarmSound;			  //Son de l'alarme quand le laser est trop utilis�
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
	sfe::Movie _explosionNucleaire;			  //Video de l'explosion nucl�aire
public:
	Jeu(std::string map);
	~Jeu();

	void draw(bool display = true);

	void drawMangeable();

	void drawLaserUi();
	void play();

	std::string getKeyPress();

	void pause(std::string msg = "Appuyez sur n'importe quel bouton pour continuer!");

	void shakeScreen();

	void killPacman();

	bool verifieSiMort(Fantome &fantome);
	
	//Choisi un position random pour le fruit
	sf::Vector2f choisirPosRandom();

};
