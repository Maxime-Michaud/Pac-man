/********************************************************************************
*Date:			16 Octobre 2015													*
*Programme:		Main.cpp														*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Jeu de Pacman													*
*TODO ajouter une description													*
*																				*
*																				*
*																				*
*********************************************************************************/
#include "autosfml.h"	//Définis des macros pour inclure les .lib de SFML, like a paresseux
#include "Jeu.h"

void main(int argc, char* argv[])
{
	std::string map;
	if (argc < 2)
		map = "maps.txt";
	else
		map = argv[1];

	Jeu jeu(map);
	jeu.play();
	
}