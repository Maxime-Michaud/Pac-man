#pragma once
#include "Ligne.h"
#include "saisieSecurisee.h"
#include <vector>
#include <SFML\Graphics.hpp>

class Map :public sf::Drawable
{
public:
	Map();
	~Map();
	/// <summary>
	/// Renvois la ligne demandé
	/// </summary>
	/// <param name="v">Une position en x et y à laquelle on veut la ligne</param>
	Ligne getLigne(int noLigne);
	void ajouterLigne(Ligne ligne);
	void lireMap(std::istream &map);
	bool valideNouvelleLigne(Ligne &l);
	int quelleLigne(sf::Vector2f ligne, int numLigne);			//retourne le nu de la ligne selon la position

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
private:
	std::vector<Ligne> _map;		//Contient toutes les lignes
	const float _width = 30;		//Largeur des lignes
	sf::Color _mapColor;
	sf::Vector2f _ghostStart;
};