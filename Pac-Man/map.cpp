#include "map.h"

Map::~Map()
{
}

Map::Map()
{
	_mapColor = sf::Color();
	_mapSize = sf::Vector2i(70, 70);

	/*TODO a effacer apres, un test*/
	_stringTexteFlash = "Appuyer sur P pour commencer =D!";
	_posTexteFlash.x = 50;
	_posTexteFlash.y = 400;
	_fontFlash.loadFromFile("steelfish rg.ttf");
	_frequenceFlash = 1500;
	_dureeFlash = 1000;
	_texteFlash = sf::Text(_stringTexteFlash, _fontFlash, 80);
	_texteFlash.setPosition(_posTexteFlash);
	_flashActif = true;
	_lineColor = sf::Color(0, 0, 255, 255);
}

void Map::initFlash(bool actif, std::string texte, int frequence, int duree, int posX, int posY, int grosseur)
{
	_flashActif = actif;
	_stringTexteFlash = texte;
	_posTexteFlash.x = posX;
	_posTexteFlash.y = posY;
	_fontFlash.loadFromFile("steelfish rg.ttf");
	_frequenceFlash = frequence;
	_dureeFlash = duree;
	_grosseurFlash = grosseur;
	_texteFlash = sf::Text(_stringTexteFlash, _fontFlash, _grosseurFlash);
	_texteFlash.setPosition(_posTexteFlash);
}

void Map::ajouterLigne(Ligne ligne)
{
	//Si la nouvelle ligne ne croise aucune autre lignes
	if (valideNouvelleLigne(ligne))
	{
		_map.push_back(ligne);
		_hasChanged = true;
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

			//Si les deux lignes sont dans la m�me direction, on les fusionne
			if (ligne.isVertical() == tmp.isVertical())
			{
				float x1, x2, y1, y2;
				x1 = std::min(ligne.getDebut().x, tmp.getDebut().x);
				y1 = std::min(ligne.getDebut().y, tmp.getDebut().y);
				x2 = std::max(ligne.getFin().x, tmp.getFin().x);
				y2 = std::max(ligne.getFin().y, tmp.getFin().y);

				//Il peut rester des intersections avec d'autre ligness, donc on se rapelle r�cursivement
				ajouterLigne(Ligne(x1, y1, x2, y2));
				return; //Quitte la fonction
			}
			else
			{
				//On trouve l'intersection entre les lignes
				auto intersect = ligne.intersect(tmp);

				//Les deux lignes cr��es a partir de tmp n'ont aucune intersection puisque tmp n'en avait aucune en th�orie
				if (tmp.getDebut() != intersect)
					_map.push_back(Ligne(tmp.getDebut(), intersect));
				if (tmp.getFin() != intersect)
					_map.push_back(Ligne(intersect, tmp.getFin()));

				//R�utilise la fonction afin d'ajouter de fa�on s�curitaire les deux fragments de ligne
				if (ligne.getDebut() != intersect)
					ajouterLigne(Ligne(ligne.getDebut(), intersect));
				if (ligne.getFin() != intersect)
					ajouterLigne(Ligne(intersect, ligne.getFin()));
				return; //Quitte la fonction
			}
		}
	}
	//Cette partie de la fonction ne devrais jamais etre atteinte
	assert(false);
}

void Map::lireMap(std::istream & map)
{
	if (!_map.empty())
		_map.clear();

	auto linePos = readNumFromStream<float, false>(map, 0, "(, )\n", '.');

	if (linePos.size() % 4)
		throw std::invalid_argument("Le format de la map est invalide!");

	for (int i = 0; i < linePos.size(); i += 4)
	{
		ajouterLigne(Ligne(linePos[i], linePos[i + 1], linePos[i + 2], linePos[i + 3]));
	}

	_bools = getBoolMap();

	_mapOutline = getOutline();
}

bool Map::valideNouvelleLigne(Ligne & l)
{
	for (auto li : _map)
		if (li.traverse(l))
			return false;

	return true;
}

Ligne Map::getLigne(unsigned int noLigne)
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
	if (_hasChanged)
	{
		_bools = getBoolMap();
		_mapOutline = getOutline();
		_hasChanged = false;
	}
	target.draw(_mapOutline, states);

	if (_flashActif)
	{
		if (_clockFlash.getElapsedTime().asMilliseconds() < _dureeFlash)
			target.draw(_texteFlash);
		else if (_clockFlash.getElapsedTime().asMilliseconds() > _frequenceFlash)
			_clockFlash.restart();
	}
}

sf::VertexArray Map::getOutline() const
{
	sf::VertexArray walls(sf::Quads);

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
					//Point en haut � gauche
					if (!_bools[i - 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _width - _thickness), _lineColor));

					//Point en bas � gauche
					if (!_bools[i - 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _width + _thickness), _lineColor));

					//Point en bas � droite
					if (!_bools[i - 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width + _width + _thickness), _lineColor));

					//Point en haut � droite
					if (!_bools[i - 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness - _width, j*_width - _width - _thickness), _lineColor));
				}

				//Mur vertical droit
				if (_bools[i + 1][j])
				{
					//Point en haut�� gauche
					if (!_bools[i + 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width - _width - _thickness), _lineColor));

					//Point en bas � gauche
					if (!_bools[i + 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width - _thickness, j*_width + _width + _thickness), _lineColor));

					//Point en bas � droite
					if (!_bools[i + 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width + _thickness, j*_width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width + _thickness, j*_width + _width + _thickness), _lineColor));

					//Point en haut�� droite
					if (!_bools[i + 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width + _thickness, j*_width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _width + _thickness, j*_width - _thickness - _width), _lineColor));
				}

				//Mur horizontal haut
				if (_bools[i][j - 1])
				{
					//Point en haut � gauche
					if (!_bools[i - 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness, j*_width - _width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _width - _thickness), _lineColor));

					//Point en haut � droite
					if (!_bools[i + 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness, j*_width - _width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness + _width, j*_width - _width - _thickness), _lineColor));

					//Point en bas � droite
					if (!_bools[i + 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness, j*_width - _width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness + _width, j*_width - _width + _thickness), _lineColor));

					//Point en bas � gauche
					if (!_bools[i - 1][j - 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness, j*_width - _width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width - _width + _thickness), _lineColor));
				}

				//Mur horizontal bas
				if (_bools[i][j + 1])
				{
					//Point en haut � gauche
					if (!_bools[i - 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness, j*_width + _width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _width - _thickness), _lineColor));

					//Point en haut � droite
					if (!_bools[i + 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness, j*_width + _width - _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness + _width, j*_width + _width - _thickness), _lineColor));

					//Point en bas � droite
					if (!_bools[i + 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness, j*_width + _width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width + _thickness + _width, j*_width + _width + _thickness), _lineColor));

					//Point en bas � gauche
					if (!_bools[i - 1][j + 1])
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness, j*_width + _width + _thickness), _lineColor));
					else
						walls.append(sf::Vertex(sf::Vector2f(i*_width - _thickness - _width, j*_width + _width + _thickness), _lineColor));
				}
			}
		}
	}

	return walls;
}

bool Map::getChanged() const
{
	return _hasChanged;
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
			//Valeur par d�faut, qui repr�sente un mur.
			boolMap[i][j] = true;

			//Position � laquelle on est rendu
			sf::Vector2f pos(i * _width, j*_width);

			for (auto it = _map.begin(); it != _map.end() && boolMap[i][j]; it++)
			{
				//Si la position est sur une ligne, elle ne repr�sente pas un mur et on quitte la boucle
				if (it->isOn(pos))
					boolMap[i][j] = false;
			}
		}
	}

	return boolMap;
}

void Map::setColor(int r, int g, int b, int a)
{
	_lineColor = sf::Color(r, g, b, a);
}