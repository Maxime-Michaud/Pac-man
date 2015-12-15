#include "Mire.h"

Mire::Mire(sf::Color couleur, int size)
{
	_couleur = couleur;
	_size = size;
}

Mire::~Mire()
{
}

void Mire::setCentre(const sf::Vector2f & centre)
{
	_centre = centre;
}

void Mire::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::VertexArray quads(sf::Quads);

	sf::Vector2f p1;
	sf::Vector2f p2;
	sf::Vector2f p3;
	sf::Vector2f p4;

	int nbCote = std::max(_size * 5, 10);	//10 est le minimum pour que ca soit acceptable
	sf::Vertex vertex;
	float rayon = _size * 7;
	float largeur = rayon/5;
	vertex.color = _couleur;

	//Dessine le cercle
	for (int i = 0; i < nbCote; ++i)
	{

		p1.x = rayon * cos(2 * (float)M_PI * i / nbCote) + _centre.x;
		p1.y = rayon * sin(2 * (float)M_PI * i / nbCote) + _centre.y;

		p2.x = rayon * cos(2 * (float)M_PI * (i + 1) / nbCote) + _centre.x;
		p2.y = rayon * sin(2 * (float)M_PI * (i + 1) / nbCote) + _centre.y;

		p3.x = (rayon - largeur) * cos(2 * (float)M_PI * (i + 1) / nbCote) + _centre.x;
		p3.y = (rayon - largeur)  * sin(2 * (float)M_PI * (i + 1) / nbCote) + _centre.y;

		p4.x = (rayon - largeur)  * cos(2 * (float)M_PI * i / nbCote) + _centre.x;
		p4.y = (rayon - largeur)  * sin(2 * (float)M_PI * i / nbCote) + _centre.y;

		vertex.position = p1;
		quads.append(vertex);

		vertex.position = p2;
		quads.append(vertex);

		vertex.position = p3;
		quads.append(vertex);

		vertex.position = p4;
		quads.append(vertex);
	}

	//Dessine les rectangles autours
	for (int i = 0; i < 4; ++i)
	{
		int longueur = _size * 5, largeur = _size * 2;
		//Offsets: 1 : pour les points plus a l'extérieur; 2: pour la largeur; 3: recentre les rectangles
		sf::Vector2f offset1, offset2, offset3;

		switch (i)
		{
		case 0:	//Droite
			offset1.x = longueur;
			offset1.y = 0;
			offset2.x = 0;
			offset2.y = largeur;
			offset3.x = -1;
			offset3.y = 0;
			break;
		case 1:	//Bas
			offset1.x = 0;
			offset1.y = longueur;
			offset2.x = largeur;
			offset2.y = 0;
			offset3.x = 0;
			offset3.y = -1;
			break;
		case 2:	//Gauche
			offset1.x = -longueur;
			offset1.y = 0;
			offset2.x = 0;
			offset2.y = largeur;
			offset3.x = 1;
			offset3.y = 0;
			break;
		case 3:	//Haut
			offset1.x = 0;
			offset1.y = -longueur;
			offset2.x = largeur;
			offset2.y = 0;
			offset3.x = 0;
			offset3.y = 1;
			break;
		}

		p1.x = rayon * cos(2 * (float)M_PI * i / 4) + _centre.x + offset2.x + offset3.x;
		p1.y = rayon * sin(2 * (float)M_PI * i / 4) + _centre.y + offset2.y + offset3.y;

		p2.x = rayon * cos(2 * (float)M_PI * i / 4) + _centre.x - offset2.x + offset3.x;
		p2.y = rayon * sin(2 * (float)M_PI * i / 4) + _centre.y - offset2.y + offset3.y;

		p3.x = rayon * cos(2 * (float)M_PI * i / 4) + _centre.x - offset2.x + offset1.x + offset3.x;
		p3.y = rayon * sin(2 * (float)M_PI * i / 4) + _centre.y - offset2.y + offset1.y + offset3.y;

		p4.x = rayon * cos(2 * (float)M_PI * i / 4) + _centre.x + offset2.x + offset1.x + offset3.x;
		p4.y = rayon * sin(2 * (float)M_PI * i / 4) + _centre.y + offset2.y + offset1.y + offset3.y;

		vertex.position = p1;
		quads.append(vertex);

		vertex.position = p2;
		quads.append(vertex);

		vertex.position = p3;
		quads.append(vertex);

		vertex.position = p4;
		quads.append(vertex);
	}

	target.draw(quads);
}
