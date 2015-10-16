#include <SFML\Graphics.hpp>
#include <algorithm>

#pragma once
class Ligne: sf::Drawable
{
private:
	sf::Vector2f	_p1,		//Position en haut ou a gauche de la ligne 
					_p2;		//Position en bas ou a droite de la ligne

public:
	Ligne();					//Ne construit rien
	Ligne(float x1, float y1, float x2, float y2);	//construit la ligne a partir des coordonées

	~Ligne();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

