#include "laser.h"

Laser::Laser(sf::Vector2f pos)
{
	_pos = pos;
}

Laser::~Laser()
{
}

void Laser::draw(sf::RenderTarget &rt, sf::RenderStates rs)const
{
	sf::VertexArray cercle(sf::LinesStrip);
	sf::Vector2f pos;
	cercle.append(sf::Vertex(_pos, sf::Color::White));
	for (int i = 1; i <= 25 + 1; i++)
	{
		pos.x = 75 * cos(2 * (float)M_PI * i / 25) + _pos.x;   //largeur cercle
		pos.y = 25 * sin(2 * (float)M_PI * i / 25) + _pos.y;   //Hauteur cercle	

		cercle.append(sf::Vertex(sf::Vertex(_pos, sf::Color::Cyan)));
	}
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
	//debutLaser.setPosition(sf::Vector2f(_pos.x - 160, _pos.y - 40));
	milieuLaser.rotate(180);
	milieuLaser.setPosition(sf::Vector2f(_pos.x - 40, _pos.y));
	break;
	case 's':
	ligne[0] = sf::Vertex(sf::Vector2f(_pos.x, _pos.y));
	ligne[1] = sf::Vertex(sf::Vector2f(_pos.x, 600));
	//debutLaser.rotate(90);
	milieuLaser.rotate(90);
	milieuLaser.setPosition(sf::Vector2f(_pos.x, _pos.y + 40));
	//debutLaser.setPosition(sf::Vector2f(_pos.x + 40, _pos.y - 0));
	break;
	case 'd':
	ligne[0] = sf::Vertex(sf::Vector2f(_pos.x, _pos.y));
	ligne[1] = sf::Vertex(sf::Vector2f(600, _pos.y));
	//debutLaser.setPosition(sf::Vector2f(_pos.x, _pos.y - 40));
	break;
	case 'w':
	ligne[0] = sf::Vertex(sf::Vector2f(_pos.x, _pos.y));
	ligne[1] = sf::Vertex(sf::Vector2f(_pos.x, 0));
	//debutLaser.rotate(90);
	milieuLaser.rotate(-90);
	milieuLaser.setPosition(sf::Vector2f(_pos.x, _pos.y - 40));
	//debutLaser.setPosition(sf::Vector2f(_pos.x + 40, _pos.y - 160));
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
	rt.draw(verticesCourbe);

	rt.draw(cercle);
	rt.draw(milieuLaser);
	//rt.draw(debutLaser);
	rt.draw(ligne, 2, sf::Lines, rs);
}