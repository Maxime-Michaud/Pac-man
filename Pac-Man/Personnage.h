/********************************************************************************
*Date:			16 Octobre 2015													*
*Programme:		Personnage.h													*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe de base pour des personnages.							*
*				Permet de:														*
*					-Dessiner des sprites qui loop								*
*					-Garder en mémoire la position								*
*					-Déplacer le personnage										*
*********************************************************************************/

#pragma once
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

class Personnage : sf::Drawable
{
	mutable int _noSprite;	//Numéro du sprite auquel on est rendu. Mutable pour qu'on puisse le modifier dans sf::Draw
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
	/// <param name="spriteRects">Région de la texture a utiliser pour les sprites. Si on veut utiliser l'image entiere, il faut créer un seul sf::IntRect vide</param>
	Personnage(std::string spriteSource, std::vector<sf::IntRect> spriteRects);

	/// <summary>
	/// Désalloue la mémoire des spritees et charge une texture vide
	/// </summary>
	virtual ~Personnage();

	/// <summary>
	/// Désalloue la mémoire des spritees et charge une texture vide
	/// </summary>
	virtual void clear();

	//Setteurs=================================================================

	/// <summary>
	/// Charge des sprites pour le personnage
	/// </summary>
	/// <param name="source">Image a charger comme texture.</param>
	/// <param name="rects">Région de la texture a utiliser pour les sprites. Si on veut utiliser l'image entiere, il faut créer un seul sf::IntRect vide</param>
	void loadSprite(std::string source, std::vector<sf::IntRect> rects);

	/// <summary>
	/// Positionne le personnage a une position en particulier
	/// </summary>
	/// <param name="pos">position ou placer le personnage</param>
	virtual void setPos(sf::Vector2f pos);

	//Getteurs=================================================================

	/// <summary>
	/// Obtiens tous les sprites du personnage. Retourne nullptr si les sprites ne sont pas initialisés
	/// </summary>
	const sf::Sprite * getSprite() const;

	/// <summary>
	/// Obtiens la position du personnage
	/// </summary>
	virtual sf::Vector2f getPos() const;

	//Fonctions de mouvement
	/// <summary>
	/// Déplace le personnage
	/// </summary>
	/// <param name="x">Déplacement horizontal</param>
	/// <param name="y">Déplacement vertical</param>
	virtual void move(float x, float y);

	/// <summary>
	/// Déplace le personnage
	/// </summary>
	/// <param name="deplacement">Déplacement horizontal et vertical</param>
	virtual void move(sf::Vector2f deplacement);

	/// <summary>
	/// Dessine le personnage et se prépare a dessiner le prochain sprite
	/// </summary>
	/// <param name="target">Cible sur laquelle dessiner</param>
	/// <param name="states">États de</param>
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};

