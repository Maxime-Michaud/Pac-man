#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML\Graphics.hpp>

#pragma once
class Mire:public sf::Drawable
{
	sf::Color _couleur;
	int _size;
	sf::Vector2f _centre;


public:
	Mire(sf::Color couleur, int size);
	~Mire();

	void setCentre(const sf::Vector2f& centre);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

