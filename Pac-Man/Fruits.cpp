/************************************************************************************
*Date:			12 Novembre 2015													*
*Programme:		Fruits.cpp															*
*Programmeur:	Nicholas Marcil														*
*Description:	Classe des fruits. Peut dessiner 7 sortes de fruit et en place		*
*juaqu'à 3 sur la map, les prochains fruit sont dans une liste						*
*************************************************************************************/

#include "Fruits.h"

Fruits::Fruits()
{
}


Fruits::~Fruits()
{
}

//Retourne le nombre de fruit sur la map
int Fruits::getNombreFruitSurMap()
{
	return _fruitsSurMap.size();
}

void Fruits::imprimmerPosFruit()
{
	for (int i = 0; i < _fruitsSurMap.size(); i++)
	{
		std::cout << _fruitsSurMap[i]._pos.x << "," << _fruitsSurMap[i]._pos.y << "   ";
	}
}

//Dessine les 7 sortes de fruit
void Fruits::dessinerFruits(sf::RenderWindow &rw)
{
	sf::Vector2f pos;
	sf::VertexArray trucADessiner(sf::Triangles);
	for (auto p : _fruitsSurMap)
	{
		switch (p._fruit)
		{
		case 'p':			//POMME
			for (int i = 0; i <= 8; i++)	//Le cercle de la pomme
			{
				pos.x = 7 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x;
				pos.y = 6 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y;
				trucADessiner.append(sf::Vertex(pos, sf::Color::Red));

				//Repositionne le centre
				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x, p._pos.y), sf::Color(255, 200, 200, 255)));

				//Position du nouveau vertex
				pos.x = 7 * cos(2 * (float)M_PI * i / 8) + p._pos.x;
				pos.y = 6 * sin(2 * (float)M_PI * i / 8) + p._pos.y;

				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 0, 0, 255)));
			}

			//La queue
			pos = sf::Vector2f(p._pos.x, p._pos.y - 3);
			trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
			pos.x -= 2;
			pos.y -= 6;
			trucADessiner.append(sf::Vertex(pos, sf::Color(100, 0, 0, 255)));
			pos.x += 2;
			trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
			break;
		case 'o':			//ORANGE
			for (int i = 0; i <= 8; i++)	//Le cercle de l'orange
			{
				pos.x = 7 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x;
				pos.y = 6 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 127, 0, 255)));

				//Repositionne le centre
				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x, p._pos.y), sf::Color(255, 169, 0, 255)));

				//Position du nouveau vertex
				pos.x = 7 * cos(2 * (float)M_PI * i / 8) + p._pos.x;
				pos.y = 6 * sin(2 * (float)M_PI * i / 8) + p._pos.y;

				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 127, 0, 255)));
			}

			//le top noir
			for (int i = 0; i <= 8; i++)
			{
				pos.x = 1 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x;
				pos.y = 1 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y-5;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 255)));

				//Repositionne le centre
				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x, p._pos.y-5), sf::Color(255, 255, 255, 255)));

				//Position du nouveau vertex
				pos.x = 1 * cos(2 * (float)M_PI * i / 8) + p._pos.x;
				pos.y = 1 * sin(2 * (float)M_PI * i / 8) + p._pos.y-5;

				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 255)));
			}
			break;
		case 'c':			//CERISE
							//Les boules de la cerise
			for (int k = 0; k < 2; k++)
			{
				for (int i = 0; i <= 8; i++)
				{
					pos.x = 4 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x - 4 + k * 9;
					pos.y = 4 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y + 4;
					trucADessiner.append(sf::Vertex(pos, sf::Color::Red));

					//Repositionne le centre
					trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x - 4 + k * 9, p._pos.y + 4), sf::Color(128, 0, 0, 255)));

					//Position du nouveau vertex
					pos.x = 4 * cos(2 * (float)M_PI * i / 8) + p._pos.x - 4 + k * 9;
					pos.y = 4 * sin(2 * (float)M_PI * i / 8) + p._pos.y + 4;

					trucADessiner.append(sf::Vertex(pos, sf::Color::Red));
				}
			}

			//La queue de la cerise
			//Première partie de la queue
			pos = sf::Vector2f(p._pos.x - 4, p._pos.y + 4);
			trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
			pos.x += 1;
			pos.y -= 9;
			trucADessiner.append(sf::Vertex(pos, sf::Color(100, 0, 0, 255)));
			pos.x += 4;
			trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));

			//Deuxième partie de la queue
			pos = sf::Vector2f(p._pos.x + 6, p._pos.y + 4);
			trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
			pos.x -= 7;
			pos.y -= 10;
			trucADessiner.append(sf::Vertex(pos, sf::Color(100, 0, 0, 255)));
			pos.x -= 3;
			trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
			break;
		case 'b':			//BANANE
			//corps de la babane droite
			pos = sf::Vector2f(p._pos.x - 6, p._pos.y - 2);
			for (int i = 0; i < 12; i++)
			{
				pos.x += i;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 0, 255)));
				pos.y += 4;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 0, 255)));
				pos.x += 8;
				pos.y -= i;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 255)));
				//Repositionne au premier point précédent
				pos.x -= 8 + i;
				pos.y += -4 + i;
	
			}
			//corps de la babane gauche
			pos = sf::Vector2f(p._pos.x - 6, p._pos.y - 2);
			for (int i = 0; i < 4; i++)
			{
				pos.x -= i;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 0, 255)));
				pos.y += 4;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 0, 255)));
				pos.x -= 8;
				pos.y -= i;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 255, 255)));
				//Repositionne au premier point précédent
				pos.x += 8 + i;
				pos.y += -4 + i;

			}
			
			break;
		case 'f':	//FRAISE
			//Feuille de la fraise
			for (int i = 0; i < 3; i++)
			{
				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x, p._pos.y), sf::Color(0, 255, 0, 255)));
				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x - 1 + i * 2, p._pos.y - 8), sf::Color(0, 255, 0, 255)));
				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x - 3 + i * 2, p._pos.y - 6), sf::Color(0, 255, 0, 255)));
			}

			for (int j = 0; j < 4; j++)		//Le corps de la fraise
			{
				for (int i = 0; i <= 8; i++)
				{
					pos.x = (6 - j/2) * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x;
					pos.y = (3 - j / 3) * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y - 2 + j*2;
					trucADessiner.append(sf::Vertex(pos, sf::Color(255, 0, 0, 255)));

					//Repositionne le centre
					trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x, p._pos.y - 2 +  j*2), sf::Color(255, 100, 52, 255)));

					//Position du nouveau vertex
					pos.x = (6 - j / 2) * cos(2 * (float)M_PI * i / 8) + p._pos.x;
					pos.y = (3 - j / 3) * sin(2 * (float)M_PI * i / 8) + p._pos.y - 2 +  j*2;

					trucADessiner.append(sf::Vertex(pos, sf::Color(255, 0, 0, 255)));
				}
			}
			
			break;
		case 'm':			//MURE
			for (int j = 0; j < 3; j++)
			{
				for (int i = 0; i <= 8; i++)	//Les cercles du haut
				{
					pos.x = 3 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x + -5 + j * 5;
					pos.y = 3 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y - 3;
					trucADessiner.append(sf::Vertex(pos, sf::Color(128, 0, 128, 255)));

					//Repositionne le centre
					trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x + -5 + j * 5, p._pos.y - 3), sf::Color(255, 0, 255, 255)));

					//Position du nouveau vertex
					pos.x = 3 * cos(2 * (float)M_PI * i / 8) + p._pos.x + -5 + j * 5;
					pos.y = 3 * sin(2 * (float)M_PI * i / 8) + p._pos.y - 3;

					trucADessiner.append(sf::Vertex(pos, sf::Color(128, 0, 128, 255)));
				}
			}

			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i <= 8; i++)	//Les cercles du milieu
				{
					pos.x = 3 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x - 2 + j * 5;
					pos.y = 3 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y;
					trucADessiner.append(sf::Vertex(pos, sf::Color(128, 0, 128, 255)));

					//Repositionne le centre
					trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x - 2 + j * 5, p._pos.y), sf::Color(255, 0, 255, 255)));

					//Position du nouveau vertex
					pos.x = 3 * cos(2 * (float)M_PI * i / 8) + p._pos.x - 2 + j * 5;
					pos.y = 3 * sin(2 * (float)M_PI * i / 8) + p._pos.y;

					trucADessiner.append(sf::Vertex(pos, sf::Color(128, 0, 128, 255)));
				}
			}
			//dernière boule
			for (int i = 0; i <= 8; i++)	//Les cercles du milieu
			{
				pos.x = 3 * cos(2 * (float)M_PI * (i-1) / 8) + p._pos.x + 1;
				pos.y = 3 * sin(2 * (float)M_PI * (i-1) / 8) + p._pos.y + 3;
				trucADessiner.append(sf::Vertex(pos, sf::Color(128, 0, 128, 255)));
				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x + 1, p._pos.y + 3), sf::Color(255, 0, 255, 255)));
				pos.x = 3 * cos(2 * (float)M_PI * i / 8) + p._pos.x + 1;
				pos.y = 3 * sin(2 * (float)M_PI * i / 8) + p._pos.y + 3;
				trucADessiner.append(sf::Vertex(pos, sf::Color(128, 0, 128, 255)));
			}
			//queue
			trucADessiner.append(sf::Vertex(p._pos, sf::Color(139, 69, 19, 255)));
			trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x - 2, p._pos.y - 9), sf::Color(139, 69, 19, 255)));
			trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x + 2, p._pos.y - 9), sf::Color(139, 69, 19, 255)));
			break;
		case 'a':		//ANANA
			//Feuilles de l'anana
			/*pos = p._pos;
			pos.x -= 4;
			for (int i = 0; i < 5; i++)
			{
				trucADessiner.append(sf::Vertex(sf::Vector2f(pos.x + i, pos.y), sf::Color(0, 125, 0, 255)));
				trucADessiner.append(sf::Vertex(sf::Vector2f(pos.x + (i * 2), pos.y - 15), sf::Color(0, 125, 0, 255)));
				trucADessiner.append(sf::Vertex(sf::Vector2f(pos.x + 5, pos.y + i/2), sf::Color(0, 255, 0, 255)));
			}*/

			for (int i = 0; i <= 8; i++)	//tranche de lanana
			{
				pos.x = 8 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x;
				pos.y = 8 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y;
				trucADessiner.append(sf::Vertex(pos, sf::Color(255, 255, 0, 255)));

				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x, p._pos.y), sf::Color(230, 230, 0, 255)));

				//Position du nouveau vertex
				pos.x = 8 * cos(2 * (float)M_PI * i / 8) + p._pos.x;
				pos.y = 8 * sin(2 * (float)M_PI * i / 8) + p._pos.y;

				trucADessiner.append(sf::Vertex(pos, sf::Color(210, 210, 0, 255)));
			}

			for (int i = 0; i <= 8; i++)	//Le trou de l'anana
			{
				pos.x = 4 * cos(2 * (float)M_PI * (i - 1) / 8) + p._pos.x;
				pos.y = 4 * sin(2 * (float)M_PI * (i - 1) / 8) + p._pos.y;
				trucADessiner.append(sf::Vertex(pos, sf::Color(0, 0, 0, 255)));

				trucADessiner.append(sf::Vertex(sf::Vector2f(p._pos.x, p._pos.y), sf::Color(0, 0, 0, 255)));

				//Position du nouveau vertex
				pos.x = 4 * cos(2 * (float)M_PI * i / 8) + p._pos.x;
				pos.y = 4 * sin(2 * (float)M_PI * i / 8) + p._pos.y;

				trucADessiner.append(sf::Vertex(pos, sf::Color(0, 0, 0, 255)));
			}
			break;
		default:
			break;
		}
	}

	rw.draw(trucADessiner);	
	
}

//Retirer le fruit mangé
bool Fruits::retirerFruitManger(sf::Vector2f &pos)
{
	for (std::vector<positionFruit>::iterator it = _fruitsSurMap.begin(); it != _fruitsSurMap.end(); it++)
	{
		if (pos.x == it->_pos.x && pos.y == it->_pos.y)
		{
			_fruitsSurMap.erase(it);
			return true;
		}
	}
	std::cout << "gros bug dans retirer fruit!!" << std::endl;
	return false;
}

//ajoute un fruit a la liste et à la map si il y a moins de 3 fruits
void Fruits::ajouterFruitListe(sf::Vector2f &pos)
{
	if (_fruitsSurMap.size() < 3)
	{
		int random = rand() % 7 + 1;
		switch (random)
		{
		case 1:		//Pomme
			_fruitsSurMap.push_back(positionFruit(pos, 'p'));
			break;
		case 2:		//Orange
			_fruitsSurMap.push_back(positionFruit(pos, 'o'));
			break;
		case 3:		//Cerise
			_fruitsSurMap.push_back(positionFruit(pos, 'c'));
			break;
		case 4:		//Banane
			_fruitsSurMap.push_back(positionFruit(pos, 'b'));
			break;
		case 5:		//Fraise
			_fruitsSurMap.push_back(positionFruit(pos, 'f'));
			break;
		case 6:		//Mure
			_fruitsSurMap.push_back(positionFruit(pos, 'm'));
			break;
		case 7:		//Anana
			_fruitsSurMap.push_back(positionFruit(pos, 'a'));
			break;
		default:
			break;
		}
	}
	
}