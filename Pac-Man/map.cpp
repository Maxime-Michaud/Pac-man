#include "map.h"

void Map::ajouterLigne(Ligne ligne)
{
	_map.push_back(ligne);
}

Ligne Map::getLigne(int noLigne)
{
	return _map[noLigne];
}

Map::~Map()
{

}

Map::Map()
{
	_mapColor = sf::Color();
}

int Map::quelleLigne(sf::Vector2f ligne, int noLigne)
{
	for (int i = 0; i < _map.size(); i++)
	{
		if (_map[i].isOn(ligne) == true)
		{
			return i;
		}
	}
	return noLigne;
}

void Map::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::VertexArray quads(sf::Quads, 2 * _map.size());

	sf::Vector2f offset(-_width, -_width);
	//Pour chaque ligne, crée un rectangle et l'ajoute a quads
	for (auto l : _map)
	{
		//Haut-Gauche
		quads.append(sf::Vertex(l.getDebut() + offset, _mapColor));
		
		//Haut-Droit
		quads.append(sf::Vertex(sf::Vector2f(l.getFin().x - offset.x, l.getDebut().y + offset.y),_mapColor));

		//Bas-Droit
		quads.append(sf::Vertex(l.getFin() - offset, _mapColor));

		//Bas-Gauche
		quads.append(sf::Vertex(sf::Vector2f(l.getDebut().x + offset.x, l.getFin().y - offset.y), _mapColor));
	}

	target.draw(quads);
}

