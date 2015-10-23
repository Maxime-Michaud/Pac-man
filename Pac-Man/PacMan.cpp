#include "PacMan.h"



PacMan::PacMan()
{
	//Variables pour l'animation
	_step = 0;
	_stepIncrement = 1;
	_numLigne = 0;
	_vitesse = 3;
	_vertical = false;
	_direction = 'd';
	_directionProchaine = 'd';
	_deathCount = 0;
	_deathIncrement = .4;

	_color = sf::Color(255, 255, 0, 255);

	_pos = sf::Vector2f(300, 300);
}


PacMan::~PacMan()
{
}

void PacMan::setLaser(bool b)
{
	_laser = b;
}

//faire une ligne courbé, source : https://github.com/SFML/SFML/wiki/Source%3A-cubic-bezier-curve#code
std::vector<sf::Vector2f> CalcCubicBezier(const sf::Vector2f &start, const sf::Vector2f &end,
		const sf::Vector2f &startControl,
		const sf::Vector2f &endControl,
		const size_t numSegments)
{
	std::vector<sf::Vector2f> ret;
	if (!numSegments) // Any points at all?
		return ret;

	ret.push_back(start); // First point is fixed
	float p = 1.f / numSegments;
	float q = p;
	for (size_t i = 1; i < numSegments; i++, p += q) // Generate all between
		ret.push_back(p * p * p * (end + 3.f * (startControl - endControl) - start) +
		3.f * p * p * (start - 2.f * startControl + endControl) +
		3.f * p * (startControl - start) + start);
	ret.push_back(end); // Last point is fixed
	return ret;
}

sf::VertexArray PacMan::buildPacMan() const
{
	//possition du prochain vertex
	sf::Vector2f pos;

	//nombre de points : le centre + 1 par coté + 1 pour aller rejoindre le premier point et fermer le cercle
	sf::VertexArray vertices(sf::TrianglesFan);

	vertices.append(sf::Vertex(_pos, _color));

	for (int i = 1; i <= _nbrCote + 1; i++)
	{

		float offset = -1;

		switch (_direction)
		{
		case 'd':
			offset = -1;
			break;
		case 'a':
			offset = (float)_nbrCote / 2 - 1;
			break;
		case 'w':
			offset = 3 * (float)_nbrCote / 4 - 1;
			break;
		case 's':
			offset = (float)_nbrCote / 4 - 1;
			break;
		}


		pos.x = _radius * cos(2 * (float)M_PI * (i + offset) / _nbrCote) + _pos.x;
		pos.y = _radius * sin(2 * (float)M_PI * (i + offset) / _nbrCote) + _pos.y;

		vertices.append(sf::Vertex(sf::Vertex(pos, _color)));
	}



	//Fais ouvrir et fermer la bouche de pac-man
	for (int i = 0; i < _step; i++)
		vertices[i + 1] = vertices[_nbrCote + 1 - i] = _pos;

	if (_step == 0 || _step >= _nbrCote / 4)
		_stepIncrement *= -1;

	return vertices;
}

void PacMan::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	_step += _stepIncrement;
	auto vertices = buildPacMan();

	if (_laser)
	{
		sf::CircleShape debutLaser(40);
		debutLaser.setFillColor(sf::Color::White);
		debutLaser.setScale(2, 1);
		sf::Vertex ligne[2];
		sf::RectangleShape milieuLaser(sf::Vector2f(600, 70));
		milieuLaser.setFillColor(sf::Color::White);
		milieuLaser.setOrigin(0, 35);
		milieuLaser.setPosition(sf::Vector2f(_pos.x + 40, _pos.y));
		switch (_direction)
		{
		case 'a':
			ligne[0] = sf::Vertex(sf::Vector2f(_pos.x, _pos.y));
			ligne[1] = sf::Vertex(sf::Vector2f(0, _pos.y));
			debutLaser.setPosition(sf::Vector2f(_pos.x - 160, _pos.y - 40));
			milieuLaser.rotate(180);
			milieuLaser.setPosition(sf::Vector2f(_pos.x - 40, _pos.y));
			break;
		case 's':
			ligne[0] = sf::Vertex(sf::Vector2f(_pos.x, _pos.y));
			ligne[1] = sf::Vertex(sf::Vector2f(_pos.x, 600));
			debutLaser.rotate(90);
			milieuLaser.rotate(90);
			milieuLaser.setPosition(sf::Vector2f(_pos.x, _pos.y + 40));
			debutLaser.setPosition(sf::Vector2f(_pos.x + 40, _pos.y - 0));
			break;
		case 'd':
			ligne[0] = sf::Vertex(sf::Vector2f(_pos.x, _pos.y));
			ligne[1] = sf::Vertex(sf::Vector2f(600, _pos.y));
			debutLaser.setPosition(sf::Vector2f(_pos.x, _pos.y - 40));
			break;
		case 'w':
			ligne[0] = sf::Vertex(sf::Vector2f(_pos.x, _pos.y));
			ligne[1] = sf::Vertex(sf::Vector2f(_pos.x, 0));
			debutLaser.rotate(90);
			milieuLaser.rotate(-90);
			milieuLaser.setPosition(sf::Vector2f(_pos.x, _pos.y - 40));
			debutLaser.setPosition(sf::Vector2f(_pos.x + 40, _pos.y - 160));
			break;
		default:
			break;
		}



		// Create a vertex array for drawing; a line strip is perfect for this
		sf::VertexArray verticesCourbe(sf::LinesStrip, 0);
		// Calculate the points on the curve (10 segments)
		std::vector<sf::Vector2f> points;
		for (int i = -20; i < 20; i++)
		{
			points.push_back(sf::Vector2f(_pos.x + abs(i) * 20, _pos.y + i));
		}

		// Append the points as vertices to the vertex array
		for (std::vector<sf::Vector2f>::const_iterator a = points.begin(); a != points.end(); ++a)
			verticesCourbe.append(sf::Vertex(*a, sf::Color::White));
		// Draw the vertex array
		target.draw(verticesCourbe);


		target.draw(milieuLaser);
		target.draw(debutLaser);
		target.draw(ligne, 2, sf::Lines, states);
	}
	target.draw(vertices);
}

void PacMan::deathAnimation(sf::RenderTarget & target) const
{
	auto vertices = buildPacMan();
	_deathCount += _deathIncrement;

	for (int i = 0; i < _deathCount && 
					i + _step + 1 < vertices.getVertexCount();
					i++)
		vertices[i + _step + 1] = _pos;

	target.draw(vertices);
}

bool PacMan::hasDisappeared() const
{
	return (_step + _deathCount) >= _nbrCote;
}
	
void PacMan::move(char direction, Map &map)
{
	Personnage::move(direction, map);
}
