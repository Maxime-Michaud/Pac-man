#pragma once
/*
Auteur:			Maxime Michaud-Corriveau
Programme:		UI.h
Description:	Cette classe regroupe tous les éléments d'interface utilisateur d'un projet
				afin de les dessiner toutes d'un coup
*/

#include <SFML\Graphics.hpp>
#include <map>

class UI:sf::Drawable
{
	std::map<std::string, sf::Font*> _fonts;
	std::map<std::string, sf::Text*> _texts;

public:
	UI();
	~UI();

	//Efface tout l'interface
	void clear();	

	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	void addText(std::string& desc, 			//Paramètres obligatoires
				 std::string& text, 
				 std::string& font,
				 sf::Vector2f& pos,
				 int size = 30,					//Paramètres optionnels
				 sf::Color color = sf::Color::White);

};

