/********************************************************************************
*Date:			16 octobre														*
*Programme:		Ligne.cpp														*
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

#include "Ligne.h"


//Ne construit rien
Ligne::Ligne()
{
	//Si on dessine la ligne dans cet �tat, un point sera dessiner en dehors de l'�cran
	_p1 = _p2 = sf::Vector2f(-1, -1);	
}

//Initialise la ligne
Ligne::Ligne(float x1, float y1, float x2, float y2)
{
	init(x1, y1, x2, y2);
}

void Ligne::init(float x1, float y1, float x2, float y2)
{
	//Une des deux paires doit �tre diff�rente
	if ((x1 == x2) == (y1 == y2))
		throw std::invalid_argument("Les lignes ne peuvent �tre diagonales ni des points");

	//Les paires n'ont pas besoin d'�tre match�es puisque X1 == X2 ou Y1 == Y2
	_p1 = sf::Vector2f(std::min(x1, x2), std::min(y1, y2));
	_p2 = sf::Vector2f(std::max(x1, x2), std::max(y1, y2));
}

Ligne::~Ligne()
{
}

sf::Vector2f Ligne::getDebut() const
{
	return _p1;
}

sf::Vector2f Ligne::getFin() const
{
	return _p2;
}

bool Ligne::isOn(sf::Vector2f pos) const
{
	return pos.x >= _p1.x && pos.x <= _p2.x
		&& pos.y >= _p1.y && pos.y <= _p2.y;
}

void Ligne::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::Vertex ligne[] = { sf::Vertex(_p1), sf::Vertex(_p2) };

	target.draw(ligne, 2, sf::Lines, states);
}
