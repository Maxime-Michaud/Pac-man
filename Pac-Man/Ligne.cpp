/********************************************************************************
*Date:			16 octobre														*
*Programme:		Ligne.cpp														*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe qui permet de dessiner des lignes a partir de			*
*				coordonnées. Les lignes peuvent être de haut en bas ou de 		*
*				gauche à droite, mais pas inversées, en diagonales ni un point.	*
*																				*
*				Des vérifications sont faites par la classe afin de garantir 	*
*				que les deux conditions sont respectées en tout temps. par		*
*				exemple, si on tente de modifier le point de départ et que l'on *
*				dépasse l'ancien point d'arrivée, les deux points seront		*
*				inversés après la modification.									*
*																				*
*********************************************************************************/

#include "Ligne.h"

//Ne construit rien
Ligne::Ligne()
{
	//Si on dessine la ligne dans cet état, un point sera dessiner en dehors de l'écran
	_p1 = _p2 = sf::Vector2f(-1, -1);
}

//Initialise la ligne
Ligne::Ligne(float x1, float y1, float x2, float y2)
{
	init(x1, y1, x2, y2);
}

void Ligne::init(float x1, float y1, float x2, float y2)
{
	//Une des deux paires doit être différente
	if ((x1 == x2) == (y1 == y2))
		throw std::invalid_argument("Les lignes ne peuvent être diagonales ni des points");

	//Les paires n'ont pas besoin d'être matchées puisque X1 == X2 ou Y1 == Y2
	_p1 = sf::Vector2f(std::min(x1, x2), std::min(y1, y2));
	_p2 = sf::Vector2f(std::max(x1, x2), std::max(y1, y2));
}

Ligne::Ligne(sf::Vector2f debut, sf::Vector2f fin)
{
	init(debut.x, debut.y, fin.x, fin.y);
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

sf::VertexArray Ligne::getVertex() const
{
	auto val = sf::VertexArray(sf::Lines, 2);
	val.append(sf::Vertex(_p1));
	val.append(sf::Vertex(_p2));

	return val;
}

bool Ligne::isOn(sf::Vector2f pos) const
{
	return pos.x >= _p1.x && pos.x <= _p2.x
		&& pos.y >= _p1.y && pos.y <= _p2.y;
}

bool Ligne::isVertical() const
{
	return _p1.x == _p2.x;
}

bool Ligne::traverse(const Ligne & l) const
{
	if (l == *this) return false;	//Une ligne ne se traverse pas elle meme

	if (isVertical() != l.isVertical()		//Les deux ne sont pas dans la meme direction
		&& (_p1 == l._p1 || _p2 == l._p1 || _p1 == l._p2 || _p2 == l._p2)) //Les lignes partagent un coin
		return false;	//Les lignes qui partagent un coin ne s'intersectent pas

	if (isVertical())
		//Les deux sont verticales
		if (l.isVertical())
			return	_p1.x == l._p1.x &&
			!(_p2.y <= l._p1.y ||
				l._p2.y <= _p1.y);
		else //ligne implicite verticale, explicite horizontale
			return l._p1.x <= _p1.x && l._p2.x >= _p1.x		//X de la ligne verticale sur la ligne horizontale
			&& _p1.y <= l._p1.y && _p2.y >= l._p1.y;	//y de la ligne horizontale sur la ligne verticale
	else
		//ligne implicite horizontale, explicite verticale
		if (l.isVertical())
			return _p1.x <= l._p1.x && _p2.x >= l._p1.x		//X de la ligne verticale sur la ligne horizontale
			&& l._p1.y <= _p1.y && l._p2.y >= _p1.y;	//y de la ligne horizontale sur la ligne verticale
		else //Les deux sont horizontales
			return	_p1.y == l._p1.y &&
			!(_p1.x <= l._p1.x ||
				l._p2.x <= _p1.x);
}

sf::Vector2f Ligne::intersect(const Ligne & l) const
{
	return sf::Vector2f(std::max(_p1.x, l._p1.x), std::max(_p1.y, l._p1.y));
}

void Ligne::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::Vertex ligne[] = { sf::Vertex(_p1), sf::Vertex(_p2) };

	target.draw(ligne, 2, sf::Lines, states);
}

bool Ligne::operator==(const Ligne & l) const
{
	return _p1 == l._p1 && _p2 == l._p2;
}