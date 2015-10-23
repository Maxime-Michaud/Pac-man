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
#include "fantome.h"
#include "map.h"
#include <iostream>
#include <SFML\System.hpp>

void main()
{
	PacMan test;
	Fantome fantomeBleu;
	test.setPos(sf::Vector2f(100, 100));

	fantomeBleu.setPos(sf::Vector2f(300, 300));

	sf::Time timePerFrame;
	int frameCount = 0;

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
	bizounne.init(300, 100, 500, 100);
	map.ajouterLigne(bizounne);
	bizounne.init(300, 0, 300, 100);
	map.ajouterLigne(bizounne);

	sf::RenderWindow tstwin;
	tstwin.create(sf::VideoMode(600, 600), "Fenetre de test");

	tstwin.clear(sf::Color(200, 200, 200, 255));

	tstwin.draw(map);
	tstwin.draw(test);
	tstwin.draw(fantomeBleu);
	tstwin.display();

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
		//TODO dans un switch (trouver comment...)
		//Note, si il est dans le while event le mouvement est saccadé
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (!test.getVertical())
				test.setDirection('d');
			test.setDirectionProchaine('d');		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (test.getVertical())
				test.setDirection('s');
			test.setDirectionProchaine('s');
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (!test.getVertical())
				test.setDirection('a');
			test.setDirectionProchaine('a');
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (test.getVertical())
				test.setDirection('w');
			test.setDirectionProchaine('w');
		}

		test.move(test.getDirection(), map);
		fantomeBleu.move(fantomeBleu.getDirection(), test.getPos(), map);
		tstwin.clear(sf::Color(200, 200, 200, 255));
		tstwin.draw(map);
		tstwin.draw(test);
		tstwin.draw(fantomeBleu);
		tstwin.display();

		timePerFrame += clock.getElapsedTime();
		frameCount++;
		if (frameCount > 100) break;
		while (clock.getElapsedTime().asMilliseconds() < 16);
	}

	while (!test.hasDisappeared())
	{
		sf::Clock clock;
		tstwin.clear(sf::Color(200, 200, 200, 255));
		tstwin.draw(map);
		tstwin.draw(fantomeBleu);
		test.deathAnimation(tstwin);
		tstwin.display();
		while (clock.getElapsedTime().asMilliseconds() < 32);
	}
	std::cout << "Temps par frame: " << timePerFrame.asSeconds() / frameCount << '\n';
	std::cout << "Temps par frame: " << timePerFrame.asMilliseconds() / frameCount<<'\n';
	std::cout << "Temps par frame: " << timePerFrame.asMicroseconds() / frameCount<<'\n';
	std::cout << "Vitesse (fps):   " << frameCount / timePerFrame.asSeconds()<< '\n';
	
	system("pause");
}