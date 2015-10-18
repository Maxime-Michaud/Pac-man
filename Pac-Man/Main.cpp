/********************************************************************************
*Date:			16 Octobre 2015													*
*Programme:		Main.cpp														*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Jeu de Pacman													*
TODO ajouter une description				
*																				*
*																				*
*																				*
*********************************************************************************/

#include "autosfml.h"	//Définis des macros pour inclure les .lib de SFML, like a paresseux
#include "Ligne.h"	
#include "PacMan.h"
#include <iostream>
#include <SFML\System\Clock.hpp>
 
void main()
{
	PacMan test;
	sf::RenderWindow tstwin;
	tstwin.create(sf::VideoMode(600, 600), "Fenetre de test");

	while (true)
	{
		sf::Clock clock;
		tstwin.clear();
		tstwin.draw(test);
		tstwin.display();
		while (clock.getElapsedTime().asMilliseconds() < 15);
	}

	system("pause");
}