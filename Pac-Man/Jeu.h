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
#include <list>
#include "VecteurDeFantome.h"
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

	int _targetfps;
	float _shake;
	bool _playing = true;
	std::vector<sf::Vector2f> _posValides;    //Les positions valides dans la map
	std::vector<std::vector<int>> _mangeable; //Les boules � manger sur la map
	std::list<std::string> _fruits;			  //Une liste de tous les fruits possibles
	std::clock_t _temps;					  //Sert � calculer le temps pass� dans le jeu
	bool _fermerHorloge = false;			  //Pour �viter de rouler plusieurs fois le script de x secondes
	int _nombreFruit = 0;					  //le nombre de fruit pr�sent dans la map
public:
	Jeu(std::string map);
	~Jeu();

	void draw(bool display = true);

	void drawMangeable();

	void play();

	std::string getKeyPress();

	void pause(std::string msg = "Appuyez sur n'importe quel bouton pour continuer!");

	void shakeScreen();

	void killPacman();

	bool verifieSiMort(Fantome &fantome);
	
	//Choisi un position random pour le fruit
	sf::Vector2f choisirPosRandom();

	//Choisit un fruit dans la liste et le dessine
	void choisirDessinerFruit(sf::Vector2f &pos);

	//Ajoute un fruit random dans la liste
	void ajouterFruitListe();

};
