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
	/// <summary>
	/// Set toute les variables a 0
	/// </summary>
	Personnage();

	/// <summary>
	/// Construit le personnage et l'initialise avec des sprites
	/// </summary>
	/// <param name="spriteSource">Image a charger comme texture.</param>
	/// <param name="spriteRects">R�gion de la texture a utiliser pour les sprites. Si on veut utiliser l'image entiere, il faut cr�er un seul sf::IntRect vide</param>
	Personnage(std::string spriteSource, std::vector<sf::IntRect> spriteRects);

	/// <summary>
	/// D�salloue la m�moire des spritees et charge une texture vide
	/// </summary>
	virtual ~Personnage();

	/// <summary>
	/// D�salloue la m�moire des spritees et charge une texture vide
	/// </summary>
	virtual void clear();

	//Setteurs=================================================================

	/// <summary>
	/// Charge des sprites pour le personnage
	/// </summary>
	/// <param name="source">Image a charger comme texture.</param>
	/// <param name="rects">R�gion de la texture a utiliser pour les sprites. Si on veut utiliser l'image entiere, il faut cr�er un seul sf::IntRect vide</param>
	void loadSprite(std::string source, std::vector<sf::IntRect> rects);

	/// <summary>
	/// Positionne le personnage a une position en particulier
	/// </summary>
	/// <param name="pos">position ou placer le personnage</param>
	virtual void setPos(sf::Vector2f pos);

	//Getteurs=================================================================

	/// <summary>
	/// Obtiens tous les sprites du personnage. Retourne nullptr si les sprites ne sont pas initialis�s
	/// </summary>
	const sf::Sprite * getSprite() const;

	/// <summary>
	/// Obtiens la position du personnage
	/// </summary>
	virtual sf::Vector2f getPos() const;

	//Fonctions de mouvement
	/// <summary>
	/// D�place le personnage
	/// </summary>
	/// <param name="x">D�placement horizontal</param>
	/// <param name="y">D�placement vertical</param>
	virtual void move(float x, float y);

	/// <summary>
	/// D�place le personnage
	/// </summary>
	/// <param name="deplacement">D�placement horizontal et vertical</param>
	virtual void move(sf::Vector2f deplacement);

	/// <summary>
	/// Dessine le personnage et se pr�pare a dessiner le prochain sprite
	/// </summary>
	/// <param name="target">Cible sur laquelle dessiner</param>
	/// <param name="states">�tats de</param>
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};

