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
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h"
#include "Sons.h"
#include "TimeTraveller.h"

class Personnage : public sf::Drawable, public TimeTraveller <sf::Vector2f>
{
protected:
	sf::Vector2f _pos;		//Position sur la map
	mutable int _vitesse;
	int _numLigne;
	bool _vertical;	//Si le personnage est sur une ligne vertical ou non
	char _direction;
	char _directionProchaine; //La direction dans laquelle le personnage veut se diriger

	mutable Sons _sons;

	bool _paused = false;
public:
	/// <summary>
	/// Set toute les variables a 0
	/// </summary>
	Personnage();

	/// <summary>
	/// D�salloue la m�moire des spritees et charge une texture vide
	/// </summary>
	virtual ~Personnage();

	//Setteurs=================================================================

	/// <summary>
	/// Positionne le personnage a une position en particulier
	/// </summary>
	/// <param name="pos">position ou placer le personnage</param>
	virtual void setPos(sf::Vector2f pos);

	//Set la position, mais v�rifie si il se t�l�porte, ne doit pas se t�l�porter
	void setPosSecuritaire(sf::Vector2f pos);

	virtual void setLigne(unsigned int ligne);
	/// <summary>
	/// Set le num�ro de la ligne du personnage
	/// </summary>
	/// <param name="numLigne">Ligne ou placer le personnage</param>
	virtual void setNumLigne(int numLigne);
	//Getteurs=================================================================

	int getNumLigne();
	/// <summary>
	/// Obtiens tous les sprites du personnage. Retourne nullptr si les sprites ne sont pas initialis�s
	/// </summary>
	const sf::VertexArray getSfVertexArray()const;

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
	virtual void move(char direction, Map &map);
	bool changerDeLigne(char direction, Map &map);

	char getDirection();
	void setDirection(char d);
	bool getVertical();
	void setVertical(bool v);
	void setDirectionProchaine(char d);
	char getDirectionProchaine();

	virtual void loadSounds() = 0;
	void stopSounds();

	void pause();
	void unpause();
};
