/********************************************************************************
*Date:			16 Octobre 2015													*
*Programme:		Personnage.h													*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe de base pour des personnages.							*
*				Permet de:														*
*					-Dessiner des sprites qui loop								*
*					-Garder en m�moire la position								*
*					-D�placer le personnage										*
*********************************************************************************/

#pragma once
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

class Personnage : sf::Drawable
{
	mutable int _noSprite;	//Num�ro du sprite auquel on est rendu. Mutable pour qu'on puisse le modifier dans sf::Draw
	sf::Vector2f _pos;		//Position sur la map
	
	sf::Texture _texture;	//Texture du personnage
	sf::Sprite	*_sprite;	//Sprites construits a partir de la texture
	int _spriteCnt;			//Nombre de sprites

public:
	//Constructeur vide, set tout a 0
	Personnage();
	//Destructeur
	virtual ~Personnage();

	virtual void clear();//D�salloue la m�moire dynamique

	//Setteurs=================================================================

	//Setteur du sprite
	void loadSprite(std::string source, std::vector<sf::IntRect> rects);

	//Setteur de la position
	virtual void setPos(sf::Vector2f pos);

	//Getteurs=================================================================
	const sf::Sprite * getSprite() const;

	virtual sf::Vector2f getPos() const;

	//Fonctions de mouvement
	virtual void move(float x, float y);
	virtual void move(sf::Vector2f deplacement);

	//Fonctions virtuelles h�rit�es de sf::drawable
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};

