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
	void initFlash(bool valeur, std::string texte, int frequence, int duree, int posX, int posY, int grosseur);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
	sf::VertexArray getOutline() const;

	bool getChanged() const;

	std::vector<std::vector<bool>> getBoolMap() const;

	void setColor(int r, int g, int b, int a);
private:
	sf::Color _lineColor;
	mutable bool _hasChanged = true;		//Si la map a changée depuis la derniere fois qu'on a fait getOutline
	sf::Vector2i _mapSize;
	std::vector<Ligne> _map;		//Contient toutes les lignes
	mutable sf::VertexArray _mapOutline;
	const float _width = 10;		//Largeur des lignes
	static const int _thickness = 3;
	sf::Color _mapColor;
	mutable std::vector<std::vector<bool>> _bools;
	sf::Vector2f _ghostStart;
	bool _flashActif = true;
	std::string _stringTexteFlash;		//La string du texte qui flash
	int _frequenceFlash;				//La fréquence à laquelle le flash apparaît		
	int _dureeFlash;					//La durée du texte qui flash
	int _grosseurFlash;					//Grosseur texte qui flash
	sf::Text _texteFlash;				//L'objet texte qui apparait a l'écran en flashant
	sf::Vector2f _posTexteFlash;		//La position du texte qui flash
	sf::Font _fontFlash;				//Le font de l'objet texte qui flash
	mutable sf::Clock _clockFlash;
};