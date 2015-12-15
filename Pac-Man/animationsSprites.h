#pragma once
#include <SFML/Graphics.hpp>
#include "Sons.h"

class Explosion
{
public:
	Explosion();
	~Explosion();
	void drawExplosion(sf::RenderTarget &rt);
	void incrementerAnimationExplosion();
	void setPos(sf::Vector2f);
	void playSonExplosion();
private:
	Sons _sonsExplosion;
	sf::Texture _explosionTextureComplet;
	sf::RectangleShape _explosionTexture[6][8];
	sf::IntRect _explosionTextureRect[6][8];
	int _vitesseExplosion = 0;				//Dessiner l'explosion tous les 5 frames
	sf::Vector2f _posExplosion;
	int _explosionAnimation = 0;            //Les frames d'animation de l'explosion
	bool _flagExplosion = false;			//Pour setter la position de l'explosuion qu'une seule fois
};