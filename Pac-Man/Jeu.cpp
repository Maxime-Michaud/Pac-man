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
	_window.setKeyRepeatEnabled(false);

	//Initialisation de la map
	std::ifstream in;
	in.open(map);
	_map.lireMap(in);

	//Initialisation des personnages
	_startpos = _map.getLigne(0).getDebut();
	_pacman.setPos(_startpos);

	_ghostStart = _map.getLigne(3).getFin();
	_fantome.push_back(new FantomeRouge());
	_fantome.push_back(new FantomeRose());
	_fantome.push_back(new FantomeOrange());
	_fantome.push_back(new FantomeBleu());

	for (auto f : _fantome)
	{
		f->setPos(_ghostStart);
		f->setLigne(_map.quelleLigne(_ghostStart, 0));
		f->setVertical(_map.getLigne(_map.quelleLigne(_ghostStart, 0)).isVertical());
	}

	//charge la police
	_font.loadFromFile("steelfish rg.ttf");

	_targetfps = 60;

	_shake = 10;

	auto temp = _map.getBoolMap();
	_mangeable.resize(temp.size());
	for (int i = 0; i < temp.size(); i++)
	{
		_mangeable[i].resize(temp[i].size());
		for (int j = 0; j < temp[i].size(); j++)
		{
			_mangeable[i][j] = int(!temp[i][j]);
		}
	}
}

Jeu::~Jeu()
{
	for (auto f : _fantome)
		delete f;
}

void Jeu::drawMangeable()
{
	sf::CircleShape boule;
	boule.setRadius(2);
	boule.setFillColor(sf::Color::Yellow);
	for (int i = 0; i < _mangeable.size(); i++)
	{
		for (int j = 0; j < _mangeable[i].size(); j++)
		{
			if (_mangeable[i][j])
			{
				if (_mangeable[i][j] & mangeable::boule)
				{
					boule.setPosition(i * 10, j * 10);
					_window.draw(boule);
				}
				if (_mangeable[i][j] & mangeable::fruit)
				{
					//draw fruit
				}
				if (_mangeable[i][j] & mangeable::grosseBoule)
				{
					//draw grosseBoule rand 4 power up
				}
			}
		}
	}
}
void Jeu::draw(bool display)
{
	_window.clear();
	_window.draw(_map);
	drawMangeable();

	_window.draw(_pacman);
	for (auto f : _fantome)
		_window.draw(*f);

	if (display)
		_window.display();
}

void Jeu::play()
{
	pause("Appuyez sur espace pour commencer!");

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
		if (_mangeable[round(_pacman.getPos().x / 10)][round(_pacman.getPos().y / 10)])
		{
			_mangeable[round(_pacman.getPos().x / 10)][round(_pacman.getPos().y / 10)] = false;
		}

		std::vector<std::thread> threads;

		for (auto f : _fantome)
		{
			threads.push_back(buildMoveThread(f));
			verifieSiMort(*f);
		}

		draw();

		for (int i = threads.size() - 1; i >= 0; i--)
			threads[i].join();

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

	//Dessine un background pour le message
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(0, 0, 0, 100));	//noir semi-transparent
	rect.setSize(sf::Vector2f(float(_window.getSize().x), 75));
	_window.draw(rect);

	sf::Event event;

	_window.pollEvent(event);

	//Dessine le message
	sf::Text pauseMsg(msg, _font, 60);
	_window.draw(pauseMsg);

	//Affiche la fenêtre	//L'event queue a presque assurément déjà un key press qui a causé l'entrée dans Jeu::pause, donc on pop cet éènement avant la boucle de pause
	_window.display();
	bool loop = true;
	while (loop)
	{
		_window.pollEvent(event);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			//Attend qu'on relache avant de dépauser
			while (loop)
			{
				_window.pollEvent(event);
				if (event.type == sf::Event::KeyReleased)
					loop = false;
			}
		}
	}
}

void Jeu::shakeScreen()
{
	if (_pacman.getLaser())
	{
		if (_shake < 1) _shake = 1;
		_shake++;

		sf::Vector2i windowPos;
		windowPos.x = static_cast<int>(_defaultWinPos.x + rand() % static_cast<int>(_shake) - _shake/ 2);
		windowPos.y = static_cast<int>(_defaultWinPos.y + rand() % static_cast<int>(_shake) - _shake / 2);
		_window.setPosition(windowPos);
	}
	else
	{
		_shake -= .5;

		_window.setPosition(_defaultWinPos);
	}
}

void Jeu::killPacman()
{
	while (!_pacman.hasDisappeared())
	{
		sf::Clock clock;
		_window.clear();

		_window.draw(_map);
		drawMangeable();

		for (auto f : _fantome)
			_window.draw(*f);

		_pacman.deathAnimation(_window);

		_window.display();

		while (clock.getElapsedTime().asMilliseconds() < 1000 / _targetfps);
	}

	sf::Text msg("Voulez-vous continuer? (O/n)", _font, 60);
	_window.draw(msg);
	_window.display();

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::O) && !sf::Keyboard::isKeyPressed(sf::Keyboard::N));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		_playing = false;

	else
	{
		_pacman.respawn(_startpos);
		_pacman.setLigne(_map.quelleLigne(_startpos, 0));
		_pacman.setVertical(_map.getLigne(_map.quelleLigne(_startpos, 0)).isVertical());

		for (auto f : _fantome)
		{
			f->setPos(_ghostStart);
			f->setLigne(_map.quelleLigne(_ghostStart, 0));
			f->setVertical(_map.getLigne(_map.quelleLigne(_ghostStart, 0)).isVertical());
		}
	}
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
	else
	{
		if ((isBetween(_pacman.getPos().x + 5 - _pacman.Width, fantome.getPos().x - fantome.Width, fantome.getPos().x + fantome.Width) || //Coté gauche de pacman dans le fantome
			isBetween(_pacman.getPos().x - 5 + _pacman.Width, fantome.getPos().x - fantome.Width, fantome.getPos().x + fantome.Width)) && //Coté droit de pacman dans le fantome
			(isBetween(_pacman.getPos().y + 5 - _pacman.Width, fantome.getPos().y - fantome.Width, fantome.getPos().y + fantome.Width) || //Coté haut de pacman dans le fantome
				isBetween(_pacman.getPos().y - 5 + _pacman.Width, fantome.getPos().y - fantome.Width, fantome.getPos().y + fantome.Width)) && //Coté bas de pacman dans le fantome
			!fantome.isDead())	//Le fantome mort ne peut pas tuer pac-man
			killPacman();
	}
	return false;
}

std::thread Jeu::buildMoveThread(Fantome * fantome)
{
	if (typeid(*fantome).name() == typeid(FantomeRose).name())
	{
		auto moveFunc = &FantomeRose::move;
		return std::thread(moveFunc, (FantomeRose*)fantome, fantome->getDirection(), _pacman.getPos(), _map);
	}
	else if (typeid(*fantome).name() == typeid(FantomeRouge).name())
	{
		auto moveFunc = &FantomeRouge::move;
		return std::thread(moveFunc, (FantomeRouge*)fantome, fantome->getDirection(), _pacman.getPos(), _map);
	}
	else if (typeid(*fantome).name() == typeid(FantomeOrange).name())
	{
		auto moveFunc = &FantomeOrange::move;
		return std::thread(moveFunc, (FantomeOrange*)fantome, fantome->getDirection(), _pacman.getPos(), _map);
	}
	else if (typeid(*fantome).name() == typeid(FantomeBleu).name())
	{
		auto moveFunc = &FantomeBleu::move;
		return std::thread(moveFunc, (FantomeBleu*)fantome, fantome->getDirection(), _fantome[0]->getPos(), _map);
	}
	throw std::exception("Le type de fantome est inconnu");
}
