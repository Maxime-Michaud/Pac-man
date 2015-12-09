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
	Ligne getLigne(unsigned int noLigne);
	void ajouterLigne(Ligne ligne);
	void lireMap(std::istream &map);
	bool valideNouvelleLigne(Ligne &l);
	int quelleLigne(sf::Vector2f ligne, int numLigne);			//retourne le nu de la ligne selon la position

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
	sf::VertexArray getOutline() const;

	bool getChanged() const;

	std::vector<std::vector<bool>> getBoolMap() const;
private:

	mutable bool _hasChanged = true;		//Si la map a changée depuis la derniere fois qu'on a fait getOutline
	sf::Vector2i _mapSize;
	std::vector<Ligne> _map;		//Contient toutes les lignes
	mutable sf::VertexArray _mapOutline;
	const float _width = 10;		//Largeur des lignes
	static const int _thickness = 3;
	sf::Color _mapColor;
	mutable std::vector<std::vector<bool>> _bools;
	sf::Vector2f _ghostStart;
};