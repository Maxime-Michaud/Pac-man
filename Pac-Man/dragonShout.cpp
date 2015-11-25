#include "dragonShout.h"



dragonShout::dragonShout()
{
}


dragonShout::~dragonShout()
{
}


void dragonShout::draw(sf::RenderTarget & rt, sf::RenderStates rs) const
{
	sf::Vector2f pos;
	sf::VertexArray trucADessiner(sf::Triangles);
	float ratio = _clockDragonShout.getElapsedTime() / sf::milliseconds(3000);
	if (ratio > 1)
		ratio = 1;
	for (int i = 1; i <= 30; i++)	//Le premier cercle
	{
		pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds()) * cos(2 * (float)M_PI * (i - 1) / 30) + _pos.x;
		pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds()) * sin(2 * (float)M_PI * (i - 1) / 30) + _pos.y;
		trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 30 - (ratio * 30))));

		//Repositionne le centre
		trucADessiner.append(sf::Vertex(sf::Vector2f(_pos.x, _pos.y), sf::Color(255, 255, 255, 30 - (ratio * 30))));

		//Position du nouveau vertex
		pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds()) * cos(2 * (float)M_PI * i / 30) + _pos.x;
		pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds()) * sin(2 * (float)M_PI * i / 30) + _pos.y;

		trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 30 - (ratio * 30))));
	}

	for (int i = 1; i <= 30; i++)	//Le 2e cercle
	{
		pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds() / 3) * cos(2 * (float)M_PI * (i - 1) / 30) + _pos.x;
		pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds() / 3) * sin(2 * (float)M_PI * (i - 1) / 30) + _pos.y;
		trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 255 - ratio * 255)));

		//Repositionne le centre
		trucADessiner.append(sf::Vertex(sf::Vector2f(_pos.x, _pos.y), sf::Color(255, 255, 255, 255 - ratio * 255)));

		//Position du nouveau vertex
		pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds() / 3) * cos(2 * (float)M_PI * i / 30) + _pos.x;
		pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds() / 3) * sin(2 * (float)M_PI * i / 30) + _pos.y;

		trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 255 - ratio * 255)));
	}

	for (int i = 1; i <= 30; i++)	//Le 3e cercle super rapide
	{
		pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds() * 2) * cos(2 * (float)M_PI * (i - 1) / 30) + _pos.x;
		pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds() * 2) * sin(2 * (float)M_PI * (i - 1) / 30) + _pos.y;
		trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 20 - ratio * 20)));

		//Repositionne le centre
		trucADessiner.append(sf::Vertex(sf::Vector2f(_pos.x, _pos.y), sf::Color(255, 255, 255, 20 - ratio * 20)));

		//Position du nouveau vertex
		pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds() * 2) * cos(2 * (float)M_PI * i / 30) + _pos.x;
		pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds() * 2) * sin(2 * (float)M_PI * i / 30) + _pos.y;

		trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 20 - ratio * 20)));
	}
	rt.draw(trucADessiner, rs);
}

void dragonShout::resetClockDragonShout()
{
	_clockDragonShout.restart();
}

void dragonShout::setPos(sf::Vector2f pos)
{
	_pos = pos;
}

//faire un soleil:
//sf::Vector2f pos;
//sf::VertexArray trucADessiner(sf::Triangles);
//
//for (int i = 0; i <= 30; i++)	//Le premier cercle
//{
//	pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds() / 5) * cos(2 * (float)M_PI * (i - 1) / 30) + _pos.x;
//	pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds() / 5) * sin(2 * (float)M_PI * (i - 1) / 30) + _pos.y;
//	trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 30)));
//
//	//Repositionne le centre
//	trucADessiner.append(sf::Vertex(sf::Vector2f(_pos.x, _pos.y), sf::Color(255, 255, 255, 30)));
//
//	//Position du nouveau vertex
//	pos.x = (_clockDragonShout.getElapsedTime().asMilliseconds() / 10) * cos(2 * (float)M_PI * i / 30) + _pos.x;
//	pos.y = (_clockDragonShout.getElapsedTime().asMilliseconds() / 10) * sin(2 * (float)M_PI * i / 30) + _pos.y;
//
//	trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 30)));
//}
//rt.draw(trucADessiner, rs);
