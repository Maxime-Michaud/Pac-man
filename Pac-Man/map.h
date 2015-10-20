#pragma once
#include "Ligne.h"
#include <list>

class Map
{
public:
	Map();
	~Map();
	void draw (sf::RenderTarget &rt, sf::RenderStates rs)const;
	/// <summary>
	/// Renvois la ligne demandé
	/// </summary>
	/// <param name="v">Une position en x et y à laquelle on veut la ligne</param>
	Ligne getLigne(int noLigne);
	void ajouterLigne(Ligne ligne);
	void lireMap(std::istream &nomFichier);
	bool valideNouvelleLigne(Ligne &l);
	int quelleLigne(sf::Vector2f ligne, int numLigne);			//retourne le nu de la ligne selon la position

private:
	std::vector<Ligne> _map;		//Contient toutes les lignes 

};