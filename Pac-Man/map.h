#pragma once
#include "Ligne.h"
#include <list>
#include <SFML\Graphics.hpp>

class Map :public sf::Drawable
{
public:
	Map();
	~Map();
	/// <summary>
	/// Renvois la ligne demand�
	/// </summary>
	/// <param name="v">Une position en x et y � laquelle on veut la ligne</param>
	Ligne getLigne(int noLigne);
	void ajouterLigne(Ligne ligne);
	void lireMap(std::istream &nomFichier);
	bool valideNouvelleLigne(Ligne &l);
	int quelleLigne(sf::Vector2f ligne, int numLigne);			//retourne le nu de la ligne selon la position

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;

private:
	std::vector<Ligne> _map;		//Contient toutes les lignes
	const float _width = 30;		//LArgeur des lignes
	sf::Color _mapColor;
};