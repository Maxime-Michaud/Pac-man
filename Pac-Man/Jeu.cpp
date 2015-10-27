/************************************************************************************
*Date:			25 Octobre 2015														*
*Programme:		Jeu.cpp																*
*Programmeur:	Maxime Michaud-Corriveau											*
*Description:	Classe principale du jeu de Pac-man. Contient tout ce dont on 		*
*				a besoin pour pouvoir lancer une partie de pac-man facilement.		*
*************************************************************************************/
#include "Jeu.h"

Jeu::Jeu(std::string map)
{
	using namespace winapi;
	//Initialisation de la fenêtre
	_window.create(sf::VideoMode(ScreenWidth, ScreenHeight), "Pac-Man");
	_window.setPosition(_defaultWinPos);

	//Initialisation de la map
	std::ifstream in;
	in.open(map);
	_map.lireMap(in);
	
	//Initialisation des personnages
	_pacman.setPos(_map.getLigne(0).getDebut());
	_fantome.push_back(new FantomeRouge());
	
	//charge la police
	_font.loadFromFile("steelfish rg.ttf");

	_targetfps = 60;
}

Jeu::~Jeu()
{
	for (int i = _fantome.size() - 1; i >= 0; i--)
	{
		delete _fantome[i];
		_fantome.pop_back();
	}
}

void Jeu::draw(bool display)
{	//Dessins
	_window.clear(sf::Color(200, 200, 200, 255));
	_window.draw(_map);

	_window.draw(_pacman);
	for (auto f : _fantome)
		_window.draw(*f);

	if (display)
		_window.display();
}

void Jeu::play()
{
	//pause("Appuyez sur une touche pour commencer!");


	while (_playing)
	{
		//Fais une pause a la fin de la boucle en attendant d'arriver a un temps voulu
		sf::Clock clock;
		
		//Vérifie l'entrée de l'utilisateur
		auto keys = getKeyPress();

		for (char c : keys)
			_pacman.input(c);

		shakeScreen();

		_pacman.move(_pacman.getDirection(), _map);

		for (auto &f : _fantome)
		{
			f->move(f->getDirection(), _pacman.getPos(), _map);
			verifieSiMort(*f);
		}

		draw();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / _targetfps);
	}
}

std::string Jeu::getKeyPress()
{
	std::string keys;

	//Les touches numérotées de 0 a 25 représentent l'alphabet
	for (int i = 0; i < 26; i++)
		if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i))
			keys += char(i + 'a');

	_playing = !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		pause();
	
	return keys;
}

void Jeu::pause(std::string msg)
{
	//Dessine le jeu
	draw(false);
	
	//Dessine le message
	sf::Text pauseMsg(msg, _font, 60);
	_window.draw(pauseMsg);

	//Affiche la fenêtre
	_window.display();


	sf::Event event;
	while (true)
	{
		_window.pollEvent(event);
		if (event.type == sf::Event::KeyPressed)
			break;
	} 
}

void Jeu::shakeScreen()
{
	if (_pacman.getLaser())
	{
		sf::Vector2i windowPos;
		windowPos.x = _defaultWinPos.x +_window.getPosition().x / 2 + rand() % 31 - 15;
		windowPos.y = _defaultWinPos.y +_window.getPosition().y / 2 + rand() % 31 - 15;
		_window.setPosition(windowPos);
	}
	else
		_window.setPosition(_defaultWinPos);

}

bool Jeu::verifieSiMort(Fantome &fantome)
{
	if (_pacman.getLaser())
	{
		switch (_pacman.getDirection())
		{
		case 'a':
			if (fantome.getPos().y == _pacman.getPos().y && fantome.getPos().x < _pacman.getPos().x)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		case 'd':
			if (fantome.getPos().y == _pacman.getPos().y && fantome.getPos().x > _pacman.getPos().x)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		case 's':
			if (fantome.getPos().x == _pacman.getPos().x && fantome.getPos().y > _pacman.getPos().y)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		case 'w':
			if (fantome.getPos().x == _pacman.getPos().x && fantome.getPos().y < _pacman.getPos().y)
			{
				fantome.setIsDead(true);
				return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}