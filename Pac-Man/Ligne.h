/********************************************************************************
*Date:			16 octobre														*
*Programme:		Ligne.h															*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe qui permet de dessiner des lignes a partir de			*
*				coordonn�es. Les lignes peuvent �tre de haut en bas ou de 		*
*				gauche � droite, mais pas invers�es, en diagonales ni un point.	*
*																				*
*				Des v�rifications sont faites par la classe afin de garantir 	*
*				que les deux conditions sont respect�es en tout temps. par		*
*				exemple, si on tente de modifier le point de d�part et que l'on *
*				d�passe l'ancien point d'arriv�e, les deux points seront		*
*				invers�s apr�s la modification.									*
*																				*
*********************************************************************************/

#include <SFML\Graphics.hpp>
#include <algorithm>

#pragma once
class Ligne: public sf::Drawable
{
private:
	sf::Vector2f	_p1,		//Position en haut ou a gauche de la ligne 
					_p2;		//Position en bas ou a droite de la ligne
public:
	//TODO Coder les setteurs

	//Constructeurs
	/// <summary>
	/// Initialise la ligne � un point en dehors de l'�cran
	/// </summary>
	Ligne();			

	/// <summary>
	/// Construit et initialise la ligne 
	/// </summary>
	/// <param name="x1">Position horizontale du d�but de la ligne</param>
	/// <param name="y1">Position verticale du d�but de la ligne</param>
	/// <param name="x2">Position horizontale de la fin de la ligne</param>
	/// <param name="y2">Position verticale de la fin de la ligne</param>
	Ligne(float x1, float y1, float x2, float y2);	//construit la ligne a partir des coordon�es

	//Destructeur
	~Ligne();

	//Initialiseur

	/// <summary>
	/// Initialise la ligne aux valeurs sp�cifi�es
	/// </summary>
	/// <param name="x1">Position horizontale du d�but de la ligne</param>
	/// <param name="y1">Position verticale du d�but de la ligne</param>
	/// <param name="x2">Position horizontale de la fin de la ligne</param>
	/// <param name="y2">Position verticale de la fin de la ligne</param>
	void init(float x1, float y1, float x2, float y2);

	//Setteurs=====================================================================================

	/// <summary>
	/// Set le point de d�but de la ligne. Si les coordonn�es corresponde a un point apr�s la fin de la ligne,
	/// set la fin de la ligne au lieu du d�but. 
	/// </summary>
	/// <param name="p">La position de la coordonn�e sur la ligne. Correspond aux X sur une ligne horizontale et
	/// aux Y sur une ligne verticale </param>
	void setDebut(float p);

	/// <summary>
	/// Set le point de d�but de la ligne. Si les coordonn�es corresponde a un point apr�s la fin de la ligne,
	/// set la fin de la ligne au lieu du d�but. 
	/// </summary>
	/// <param name="x">position horizontale du point</param>
	/// <param name="y">position verticale du point</param>
	void setDebut(float x, float y);

	/// <summary>
	/// Set le point de fin de la ligne. Si les coordonn�es corresponde a un point avant le d�but de la ligne,
	/// set le d�but de la ligne au lieu de la fin. 
	/// </summary>
	/// <param name="p">La position de la coordonn�e sur la ligne. Correspond aux X sur une ligne horizontale et
	/// aux Y sur une ligne verticale </param>
	void setFin(float p);

	/// <summary>
	/// Set le point de fin de la ligne. Si les coordonn�es corresponde a un point avant le d�but de la ligne,
	/// set le d�but de la ligne au lieu de la fin.
	/// </summary>
	/// <param name="x">position horizontale du point</param>
	/// <param name="y">position verticale du point</param>
	void setFin(float x, float y);

	//Getteurs=====================================================================================
	/// <summary>
	/// Retourne la position du d�but de la ligne
	/// </summary>
	sf::Vector2f getDebut() const;

	/// <summary>
	/// Retourne la position de la fin de la ligne
	/// </summary>
	sf::Vector2f getFin() const;

	/// <summary>
	/// Retourne les vertex pour dessiner la ligne. 
	/// </summary>
	sf::VertexArray getVertex() const;

	/// <summary>
	/// V�rifie si une position est directement sur la ligne.
	/// </summary>
	/// <param name="pos">Position a v�rifier</param>
	bool isOn(sf::Vector2f pos) const;

	/// <summary>
	/// Dessine la ligne. 
	/// A remplacer par getVertex si on veut dessiner plusieurs lignes � la fois, pour des raisons de performance
	/// </summary>
	/// <param name="target">cible sur laquelle dessiner</param>
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

