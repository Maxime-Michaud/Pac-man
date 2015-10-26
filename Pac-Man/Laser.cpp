#include "laser.h"
#include <random>

Laser::Laser(sf::Vector2f pos, char direction)
{
	_pos = pos;
	_direction = direction;
}

Laser::~Laser()
{
}

void Laser::draw(sf::RenderTarget &rt, sf::RenderStates rs)const
{
	sf::VertexArray cercle(sf::LinesStrip);
	sf::Vector2f offset;
	int nbCote = 25;
	sf::Vector2f arrondir(1, 3);
	int hauteurLaser = 25;
	switch (_direction)
	{
	case 'd':
		arrondir = sf::Vector2f(3, 1);
		offset = sf::Vector2f(4 * 25 / 2, 0);
		break;
	case 'a':
		arrondir = sf::Vector2f(3, 1);
		offset = sf::Vector2f(4 * -25 / 2, 0);
		break;
	case 'w':
		arrondir = sf::Vector2f(1, 3);
		offset = sf::Vector2f(0, 4 * -25 / 2);
		break;
	case 's':
		arrondir = sf::Vector2f(1, 3);
		offset = sf::Vector2f(0, 4 * 25 / 2);
		break;
	}
	sf::VertexArray debutLaser(sf::TrianglesStrip);
	sf::Vector2f pos;

	debutLaser.append(sf::Vertex(_pos, sf::Color::White));
	for (int i = 1; i <= nbCote + 1; i++)
	{
		pos.x = hauteurLaser * arrondir.x * cos(2 * (float)M_PI * (i) / nbCote) + _pos.x + offset.x;   //largeur cercle
		pos.y = hauteurLaser * arrondir.y * sin(2 * (float)M_PI * (i) / nbCote) + _pos.y + offset.y;   //Hauteur cercle
		debutLaser.append(sf::Vertex(_pos, sf::Color(0, 255, 255, 255)));
		debutLaser.append(sf::Vertex(sf::Vertex(pos, sf::Color(i * 25, 255, 255, 255))));
	}

	sf::VertexArray milieuLaser(sf::Quads);
	switch (_direction)
	{
	case 'd':
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x - 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2 + 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x - 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x - 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x - 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2 + 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2 + 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2 + 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x - 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(rt.getSize().x - 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2 + 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * 2 + 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		break;
	case 'a':
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0 + 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2 - 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0 + 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2 - 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0 + 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0 + 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2 - 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2, _pos.y + hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2, _pos.y - hauteurLaser), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2 - 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0 + 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(0 + 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2 - 30, _pos.y + 10), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser * -2 - 30, _pos.y - 10), sf::Color(255, 255, 255, 255))));

		break;
	case 's':
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, _pos.y + hauteurLaser * 2), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, rt.getSize().y), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, rt.getSize().y - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, _pos.y + hauteurLaser * 2 + 30), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, rt.getSize().y - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, rt.getSize().y), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, rt.getSize().y), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, rt.getSize().y - 30), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, rt.getSize().y), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, rt.getSize().y - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, _pos.y + hauteurLaser * 2 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, _pos.y + hauteurLaser * 2), sf::Color(0, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, _pos.y + hauteurLaser * 2 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, _pos.y + hauteurLaser * 2), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, _pos.y + hauteurLaser * 2), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, _pos.y + hauteurLaser * 2 + 30), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, rt.getSize().y - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, rt.getSize().y - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, _pos.y + hauteurLaser * 2 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, _pos.y + hauteurLaser * 2 + 30), sf::Color(255, 255, 255, 255))));
		break;
	case 'w':
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, _pos.y + hauteurLaser * -2), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, 0), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, 0 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, _pos.y + hauteurLaser * -2 - 30), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, 0 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, 0), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, 0), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, 0 + 30), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, 0), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, 0 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, _pos.y + hauteurLaser * -2 - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, _pos.y + hauteurLaser * -2), sf::Color(0, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, _pos.y + hauteurLaser * -2 - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + hauteurLaser, _pos.y + hauteurLaser * -2), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - hauteurLaser, _pos.y + hauteurLaser * -2), sf::Color(0, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, _pos.y + hauteurLaser * -2 - 30), sf::Color(255, 255, 255, 255))));

		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, 0 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, 0 + 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x + 10, _pos.y + hauteurLaser * -2 - 30), sf::Color(255, 255, 255, 255))));
		milieuLaser.append(sf::Vertex(sf::Vertex(sf::Vector2f(_pos.x - 10, _pos.y + hauteurLaser * -2 - 30), sf::Color(255, 255, 255, 255))));
		break;
	}

	rt.draw(debutLaser);
	rt.draw(milieuLaser);

	for (int i = 0; i < 6; i++)
	{
		sf::VertexArray ligneDansLaser(sf::LinesStrip);
		sf::Color couleurRandom(0, rand() % 255, 255, 100 + rand() % 150);
		sf::Color couleurRandom2(255, 0 + rand() % 100, 0 + rand() % 100, 100 + rand() % 150);
		for (int j = 0; j < 50; j++)
		{
			sf::Vector2f pos, pos2;
			if (_direction == 'd')
			{
				pos = sf::Vector2f(_pos.x + 15 * j, _pos.y + sin(j + rand() % 10) * float(rand() % 15));
				pos2 = sf::Vector2f(_pos.x + 4 * j, _pos.y + sin(j + rand() % 50) * float(rand() % 10));
			}

			else if (_direction == 'a')
			{
				pos = sf::Vector2f(_pos.x - 15 * j, _pos.y + sin(j + rand() % 10) * float(rand() % 15));
				pos2 = sf::Vector2f(_pos.x - 4 * j, _pos.y + sin(j + rand() % 50) * float(rand() % 10));
			}

			else if (_direction == 's')
			{
				pos = sf::Vector2f(_pos.x + sin(j + rand() % 10) * float(rand() % 15), _pos.y + 15 * j);
				pos2 = sf::Vector2f(_pos.x + sin(j + rand() % 50) * float(rand() % 10), _pos.y + 4 * j);
			}

			else if (_direction == 'w')
			{
				pos = sf::Vector2f(_pos.x + sin(j + rand() % 10) * float(rand() % 15), _pos.y - 15 * j);
				pos2 = sf::Vector2f(_pos.x + sin(j + rand() % 50) * float(rand() % 10), _pos.y - 4 * j);
			}

			ligneDansLaser.append(sf::Vertex(pos2, couleurRandom));
			ligneDansLaser.append(sf::Vertex(pos, couleurRandom2));
		}
		rt.draw(ligneDansLaser);
	}
}