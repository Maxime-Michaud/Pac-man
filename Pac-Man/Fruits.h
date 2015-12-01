/************************************************************************************
*Date:			12 Novembre 2015													*
*Programme:		Fruits.h															*
*Programmeur:	Nicholas Marcil														*
*Description:	Classe des fruits. Peut dessiner 7 sortes de fruit et en place		*
*juaqu'à 3 sur la map, les prochains fruit sont dans une liste						*
*************************************************************************************/

#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>

//Une position de fruit(un vecteur 2f et un char pour le type de fruit
struct positionFruit
{
	positionFruit(sf::Vector2f pos, char c)
	{
		_pos = pos;
		_fruit = c;
	}
	sf::Vector2f _pos;
	char _fruit;
};

class Fruits
{
	std::vector<positionFruit> _fruitsSurMap;			//Les fruits sur la map
	std::list<positionFruit> _fruitsListe;				//Une liste de tous les fruits en attente
public:
	Fruits();
	~Fruits();
	int getNombreFruitSurMap();							//Retourne le nombre de fruit présent sur la map
	bool retirerFruitManger(sf::Vector2f&);				//Retire un fruit mangé
	void dessinerFruits(sf::RenderWindow &rw);			//Dessine un fruit
	void imprimmerPosFruit();
	//Ajoute un fruit aux liste
	void ajouterFruitListe(sf::Vector2f &pos);			
};

