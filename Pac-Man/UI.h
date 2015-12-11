#pragma once
/*
Auteur:			Maxime Michaud-Corriveau
Programme:		UI.h
Description:	Cette classe regroupe tous les �l�ments d'interface utilisateur d'un projet
				afin de les dessiner toutes d'un coup
*/

#include <SFML\Graphics.hpp>
#include <map>
#include <tuple>
#include <vector>
#include <functional>

class UI:public sf::Drawable
{
	std::map<std::string, sf::Font*> _fonts;
	mutable std::map<std::string, std::pair<sf::Text*, int>> _texts;
	mutable std::map<std::string, std::pair<std::function<bool()>*, bool>> _functions;

public:
	UI();
	~UI();

	//Efface tout l'interface
	void clear();	

	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	/// <summary>
	/// Ajoute un texte a l'interface
	/// </summary>
	/// <param name="desc">Nom du texte. utilis�e pour pouvoir le modifier plus tard</param>
	/// <param name="text">Texte � �crire</param>
	/// <param name="font">URI pour la police de charact�re</param>
	/// <param name="pos">Position du texte</param>
	/// <param name="size">Grosseur du texte. 30 est pris par d�faut</param>
	/// <param name="color">Couleur du texte. La couleur par d�faut est blanc</param>
	/// <param name="frames">Nombre de frame que le texte restera afficher. Un nombre n�gatif sera afficher tout le temps.</param>
	void addText(const std::string& desc, 			//Param�tres obligatoires
				 const std::string& text, 
				 const std::string& font,
				 const sf::Vector2f& pos,
				 int size = 30,					//Param�tres optionnels
				 const sf::Color &color = sf::Color::White,
				 int frames = -1);

	/// <summary>
	/// Retourne une r�f�rence vers un texte en m�moire si il existe. Permet de modifier un texte d�ja cr��
	/// </summary>
	/// <param name="desc">Nom du texte</param>
	sf::Text& Text(const std::string& desc);

	void changeText(const std::string& desc, const std::string & newText);

	void setFrames(const std::string& desc, int frame);

	bool hasText(const std::string& desc) const;

	void addAnimation(const std::string& desc, std::function<bool()> * func);

	void playAnimation(const std::string& desc);
};


