#include "PacMan.h"



PacMan::PacMan()
{
	step = 0;
	stepIncrement = 2;
}


PacMan::~PacMan()
{
}


void PacMan::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//Centre du cercle
	sf::Vector2f centre(300, 300), pos;
	sf::Color color(255, 255, 0, 255);

	//Définition les constantes du cercle
	const float radius = 25;
	const int nbrCote = 100;


	sf::Vertex test[nbrCote + 2];

	test[0] = sf::Vertex(centre, color);
	
	for (int i = 1; i <= nbrCote + 1; i++)
	{
		pos.x = radius * cos(2 * M_PI * (i - 1) / nbrCote) + centre.x;
		pos.y = radius * sin(2 * M_PI * (i - 1) / nbrCote) + centre.y;

		test[i] = sf::Vertex(pos, color);
	}

	for (int i = 0; i < step; i++)
		test[i + 1] = test[nbrCote + 1 - i] = centre;

	step += stepIncrement;

	if (step == 0 || step >= nbrCote / 4 - 5)
		stepIncrement *= -1;
	target.draw(test, nbrCote + 2, sf::TrianglesFan);

}