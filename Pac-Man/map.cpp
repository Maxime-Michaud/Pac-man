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