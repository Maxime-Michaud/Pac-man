#include "map.h"

Map::~Map()
{
}

Map::Map()
{
	_mapColor = sf::Color();
	_mapSize = sf::Vector2i(70, 70);
}

void Map::ajouterLigne(Ligne ligne)
{
	//Si la nouvelle ligne ne croise aucune autre lignes
	if (valideNouvelleLigne(ligne))
	{
		_map.push_back(ligne);
		return;
	}

	for (int i = 0; i < _map.size(); i++)
	{
		if (_map[i].traverse(ligne))
		{
			//On efface l'ancienne ligne a la position i pour la remplacer plus tard
			auto tmp = _map[i];
			std::swap(_map[i], _map[_map.size() - 1]);
			_map.resize(_map.size() - 1);

			//Si les deux lignes sont dans la même direction, on les fusionne
			if (ligne.isVertical() == tmp.isVertical())
			{
				float x1, x2, y1, y2;
				x1 = std::min(ligne.getDebut().x, tmp.getDebut().x);
				y1 = std::min(ligne.getDebut().y, tmp.getDebut().y);
				x2 = std::max(ligne.getFin().x, tmp.getFin().x);
				y2 = std::max(ligne.getFin().y, tmp.getFin().y);

				//Il peut rester des intersections avec d'autre ligness, donc on se rapelle récursivement
				ajouterLigne(Ligne(x1, y1, x2, y2));
				return; //Quitte la fonction
			}
			else
			{
				//On trouve l'intersection entre les lignes
				auto intersect = ligne.intersect(tmp);

				//Les deux lignes créées a partir de tmp n'ont aucune intersection puisque tmp n'en avait aucune en théorie
				if (tmp.getDebut() != intersect)
					_map.push_back(Ligne(tmp.getDebut(), intersect));
				if (tmp.getFin() != intersect)
					_map.push_back(Ligne(intersect, tmp.getFin()));

				//Réutilise la fonction afin d'ajouter de façon sécuritaire les deux fragments de ligne
				if (ligne.getDebut() != intersect)
					ajouterLigne(Ligne(ligne.getDebut(), intersect));
				if (ligne.getFin() != intersect)
					ajouterLigne(Ligne(intersect, ligne.getFin()));
				return; //Quitte la fonction
			}
		}
	}
	//Cette partie de la fonction ne devrais jamais etre atteinte
	assert(true);
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

	_bools = getBoolMap();
}

bool Map::valideNouvelleLigne(Ligne & l)
{
	for (auto li : _map)
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
	target.draw(getOutline(), states);
}

sf::VertexArray Map::getOutline() const
{
	sf::VertexArray lines(sf::PrimitiveType::Quads);

	sf::Color lineColor = sf::Color(0, 0, 255, 255);
	sf::Color black;

	for (int i = 1; i < _bools.size() - 1; i++)
	{
		for (int j = 1; j < _bools[i].size() - 1; j++)
		{
			//Si il s'agit d'une zone traversable
			if (!_bools[i][j])
			{
				//Mur vertical gauche
				if (_bools[i - 1][j])
				{
					//Point en haut à gauche
					if (!_bools[i - 1][j - 1]) 
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _width - _thickness), lineColor));

					//Point en bas à gauche
					if (!_bools[i - 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _width + _thickness), lineColor));


					//Point en bas à droite
					if (!_bools[i - 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width + _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width + _width + _thickness), lineColor));

					//Point en haut à droite
					if (!_bools[i - 1][j - 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width - _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width - _width - _thickness), lineColor));

				}

				//Mur vertical droit
				if (_bools[i + 1][j])
				{
					//Point en haut¸à gauche
					if (!_bools[i + 1][j - 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width - _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width - _width - _thickness), lineColor));

					//Point en bas à gauche
					if (!_bools[i + 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width + _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width + _width+ _thickness), lineColor));

					//Point en bas à droite
					if (!_bools[i + 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width+ _thickness, j*_width + _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width+ _thickness, j*_width + _width + _thickness), lineColor));

					//Point en haut¸à droite
					if (!_bools[i + 1][j - 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width + _thickness, j*_width - _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _width + _thickness, j*_width - _thickness -_width), lineColor));

				}

				//Mur horizontal haut
				if (_bools[i][j - 1])
				{
					//Point en haut à gauche 
					if (!_bools[i - 1][j - 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness, j*_width - _width - _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _width - _thickness), lineColor));

					//Point en haut à droite
					if (!_bools[i + 1][j - 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width+ _thickness, j*_width - _width - _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness + _width, j*_width - _width- _thickness), lineColor));

					//Point en bas à droite
					if (!_bools[i + 1][j - 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness, j*_width - _width + _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness + _width, j*_width - _width + _thickness), lineColor));

					//Point en bas à gauche
					if (!_bools[i - 1][j - 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness, j*_width - _width + _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _width  + _thickness), lineColor));

				}

				//Mur horizontal bas
				if (_bools[i][j + 1])
				{
					//Point en haut à gauche
					if (!_bools[i - 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness, j*_width + _width - _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _width- _thickness), lineColor));

					//Point en haut à droite
					if (!_bools[i + 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width+ _thickness, j*_width + _width- _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width + _thickness + _width, j*_width + _width- _thickness), lineColor));

					//Point en bas à droite
					if (!_bools[i + 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width+ _thickness, j*_width + _width + _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width+ _thickness + _width, j*_width + _width + _thickness), lineColor));

					//Point en bas à gauche
					if (!_bools[i - 1][j + 1])
						lines.append(sf::Vertex(sf::Vector2f(i*_width- _thickness, j*_width + _width+ _thickness), lineColor));
					else
						lines.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _width+ _thickness), lineColor));

				}
			}
		}
	}

	return lines;
}

std::vector<std::vector<bool>> Map::getBoolMap() const
{
	std::vector<std::vector<bool>> boolMap;

	boolMap.resize(_mapSize.x);

	for (int i = 0; i < _mapSize.x; i++)
	{
		boolMap[i].resize(_mapSize.y);

		for (int j = 0; j < _mapSize.y; j++)
		{
			//Valeur par défaut, qui représente un mur.
			boolMap[i][j] = true;
			//Position à laquelle on est rendu
			sf::Vector2f pos(i * _width, j*_width);

			for (auto it = _map.begin(); it != _map.end() && boolMap[i][j]; it++)
			{
				//Si la position est sur une ligne, elle ne représente pas un mur et on quitte la boucle
				if (it->isOn(pos))
					boolMap[i][j] = false;
			}
		}
	}

	return boolMap;
}