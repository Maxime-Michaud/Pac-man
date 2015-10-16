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


void main()
{
	sf::RenderWindow tstWin;
	tstWin.create(sf::VideoMode(800, 600), "Fenetre de test");
	Ligne Test[] = { Ligne(5,5,5,100), Ligne(100,5,5,5), Ligne(10, 10, 10, 950) };

	for (int i = 0; i < 3 ; i++)
		tstWin.draw(Test[i]);

	tstWin.display();
	system("pause");
}