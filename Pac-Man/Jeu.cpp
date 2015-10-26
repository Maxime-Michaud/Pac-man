/************************************************************************************
*Date:			25 Octobre 2015														*
*Programme:		Jeu.cpp																*
*Programmeur:	Maxime Michaud-Corriveau											*
*Description:	Classe principale du jeu de Pac-man. Contient tout ce dont on 		*
*				a besoin pour pouvoir lancer une partie de pac-man facilement.		*
*************************************************************************************/
#include "Jeu.h"

Jeu::Jeu()
{
	//Initialisation de la map
}

Jeu::~Jeu()
{
}

bool Jeu::verifieSiMort(PacMan &pacMan, Fantome &fantome)
{
	if (pacMan.getLaser())
	{
		switch (pacMan.getDirection())
		{
		case 'a':
			if (fantome.getPos().y == pacMan.getPos().y && fantome.getPos().x < pacMan.getPos().x)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		case 'd':
			if (fantome.getPos().y == pacMan.getPos().y && fantome.getPos().x > pacMan.getPos().x)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		case 's':
			if (fantome.getPos().x == pacMan.getPos().x && fantome.getPos().y > pacMan.getPos().y)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		case 'w':
			if (fantome.getPos().x == pacMan.getPos().x && fantome.getPos().y < pacMan.getPos().y)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		default:
			break;
		}
		return false;
	}
	return false;
}