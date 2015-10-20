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
#include "map.h"
#include <iostream>
#include <SFML\System.hpp>

void main()
{
	PacMan test;
	test.setPos(sf::Vector2f(100, 100));
	Map map;
	Ligne bizounne;
	bizounne.init(100, 100, 300, 100);
	map.ajouterLigne(bizounne);
	bizounne.init(300, 100, 300, 300);
	map.ajouterLigne(bizounne);
	bizounne.init(100, 300, 300, 300);
	map.ajouterLigne(bizounne);
	bizounne.init(100, 100, 100, 300);
	map.ajouterLigne(bizounne);

	sf::Vertex lignes[] =
	{
		sf::Vertex(sf::Vector2f(100, 100)),
		sf::Vertex(sf::Vector2f(300, 100)),

		sf::Vertex(sf::Vector2f(300, 100)),
		sf::Vertex(sf::Vector2f(300, 300)),

		sf::Vertex(sf::Vector2f(100, 300)),
		sf::Vertex(sf::Vector2f(300, 300)),

		sf::Vertex(sf::Vector2f(100, 100)),
		sf::Vertex(sf::Vector2f(100, 300))
	};

	sf::RenderWindow tstwin;
	tstwin.create(sf::VideoMode(600, 600), "Fenetre de test");

	sf::Event event;
	while (tstwin.isOpen())
	{
		tstwin.pollEvent(event);
		if (event.type == sf::Event::KeyPressed)
			break;
	}

	while (tstwin.isOpen())
	{
		sf::Clock clock;
		while (tstwin.pollEvent(event))
		{
			
		}
		//TODO dans un switch (trouver comment...)
		//Note, si il est dans le while event le mouvement est saccadé
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (!test.getVertical())
				test.setDirection('d');
			else
				test.setDirectionProchaine('d');
			//test.move('d', map);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (test.getVertical())
				test.setDirection('s');
			else
				test.setDirectionProchaine('s');
			//test.move('s', map);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (!test.getVertical())
				test.setDirection('a');
			else
				test.setDirectionProchaine('a');
			//test.move('a', map);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (test.getVertical())
				test.setDirection('w');
			else
				test.setDirectionProchaine('w');
			//test.move('w', map);
		}

		test.move(test.getDirection(), map);
		tstwin.clear(sf::Color(200, 200, 200, 255));
		tstwin.draw(map);
		tstwin.draw(test);
		tstwin.display();
		while (clock.getElapsedTime().asMilliseconds() < 20);
	
}


	/*for (int i = 0; i < 200; i++)
	{
		sf::Clock clock;
		tstwin.clear();
		tstwin.draw(lignes, 8, sf::Lines);
		tstwin.draw(test);
		tstwin.display();
		
		while (clock.getElapsedTime().asMilliseconds() < 20);
	}*/
	system("pause");
}