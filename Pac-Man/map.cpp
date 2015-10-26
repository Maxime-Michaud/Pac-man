#include "map.h"

Map::~Map()
{

}

Map::Map()
{
	_mapColor = sf::Color();
}

void Map::ajouterLigne(Ligne ligne)
{
	//Si la nouvelle ligne ne croise aucune autre lignes
//	if (valideNouvelleLigne(ligne))
	{
		_map.push_back(ligne);
		return;
	}

	for (int i = 0; i < _map.size; i++)
	{
		if (_map[i].traverse(ligne))
		{
			//On efface l'ancienne ligne a la position i pour la remplacer par deux petites lignes
			auto tmp = _map[i];
			std::swap(_map[i], *_map.end());
		}
	}

}

void Map::lireMap(std::istream & map)
{
	auto linePos = readNumFromStream<float, false>(map, 0, "(, )\n", '.');

	if (linePos.size() % 4)
		throw std::invalid_argument("Le format de la map est invalide!");

	for (int i = 0; i < linePos.size(); i += 4)
	{
		ajouterLigne(Ligne(linePos[i], linePos[i + 1], linePos[i + 2], linePos[i + 3]));
	}
}

bool Map::valideNouvelleLigne(Ligne & l)
{
	for (auto li:_map)
		if (li.traverse(l))
			return false;

	return true;
}

Ligne Map::getLigne(int noLigne)
{
	if (noLigne >= _map.size()) throw std::invalid_argument("Le numero de ligne est trop grand");
	return _map[noLigne];
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

