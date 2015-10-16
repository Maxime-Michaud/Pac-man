#include "Ligne.h"


//Ne construit rien
Ligne::Ligne()
{
	//Si on dessine la ligne dans cet état, un point sera dessiner en dehors de l'écran
	_p1 = _p2 = sf::Vector2f(-1, -1);	
}

Ligne::Ligne(float x1, float y1, float x2, float y2)
{
	//Une des deux paires doit être différente
	if ((x1 == x2) ==  (y1 == y2))
		throw std::invalid_argument("Les lignes ne peuvent être diagonales ni des points");

	//Les paires n'ont pas besoin d'être matchées puisque X1 == X2 ou Y1 == Y2
	_p1 = sf::Vector2f(std::min(x1, x2), std::min(y1, y2));
	_p2 = sf::Vector2f(std::max(x1, x2), std::max(y1, y2));
}


Ligne::~Ligne()
{
}

void Ligne::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	auto p1 = sf::Vertex(_p1),
		 p2 = sf::Vertex(_p2);
}
