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
	srand(std::time(NULL));
	_ghostStart = _map.getLigne(3).getFin();
	_fantome.add(new FantomeRouge());
	_fantome.add(new FantomeRose());
	_fantome.add(new FantomeOrange());
	_fantome.add(new FantomeBleu());
	/*for (int i = 0; i < 100; i++)
		_fantome.add(new FantomeOrange());*/

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

	//Initialise les boules jaune à manger sur tout la map
	auto temp = _map.getBoolMap();
	_mangeable.resize(temp.size());
	for (int i = 0; i < temp.size(); i++)
	{
		_mangeable[i].resize(temp[i].size());
		for (int j = 0; j < temp[i].size(); j++)
		{
			_mangeable[i][j] = int(!temp[i][j]);
			if (_mangeable[i][j])
			{
				_posValides.push_back(sf::Vector2f(i * 10, j * 10));
			}
			//Si la boule est dans un coin, elle devient une grosse boule
			if ((i > 0 && i < temp.size() - 1 && j > 0 && j < temp[i].size() - 1) &&
				((temp[i - 1][j] == true && temp[i][j - 1] == true && temp[i][j + 1] == false && temp[i + 1][j] == false && temp[i][j] == false) ||
				(temp[i + 1][j] == true && temp[i][j + 1] == true && temp[i][j - 1] == false && temp[i - 1][j] == false && temp[i][j] == false) ||
				(temp[i - 1][j] == true && temp[i][j + 1] == true && temp[i][j - 1] == false && temp[i + 1][j] == false && temp[i][j] == false) ||
				(temp[i + 1][j] == true && temp[i][j - 1] == true && temp[i][j + 1] == false && temp[i - 1][j] == false && temp[i][j] == false)))
			{
				_mangeable[i][j] = mangeable::grosseBoule;
			}
		}
	}
	_temps = std::clock();

	_laserText = sf::Text("Laser overdredive:", _font, 40);
	_laserText.setPosition(sf::Vector2f(650, 150));
	_alarmBuffer.loadFromFile("alarm.wav");
	_alarmSound.setBuffer(_alarmBuffer);
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

					choisirDessinerFruit(sf::Vector2f((i * 10) - 5, (j * 10) - 4));
				}
				if (_mangeable[i][j] & mangeable::grosseBoule)
				{
					boule.setRadius(5);
					boule.setPosition(i * 10 - 3, j * 10 - 3 );
					_window.draw(boule);
					boule.setRadius(2);
				}
			}
		}
	}
}

void Jeu::drawLaserUi()
{
	_window.draw(_laserText);
	sf::VertexArray laserGauge(sf::Quads);
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(650, 200), sf::Color(200, 200, 200, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(850, 200), sf::Color(255, 255, 255, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(850, 220), sf::Color(255, 255, 255, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(650, 220), sf::Color(200, 200, 200, 255))));

	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(0, 0, 0, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 203), sf::Color(0, 0, 0, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 217), sf::Color(0, 0, 0, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(0, 0, 0, 255))));
	if (_pacman.getLaser())
	{
		if (_tempsEntreLaserEtStop2 < sf::milliseconds(0))
		{
			_tempsEntreLaserEtStop2 = sf::milliseconds(0);
		}
		 sf::Time tempsMs = _tempsEntreLaserEtStop = _tempsEntreLaserEtStop2 + _pacman.getTempsLaser();
		
		double ratio = tempsMs / sf::milliseconds(4000);
		if (tempsMs < sf::milliseconds(4000))
		{
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255 * ratio, 255-(255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 203), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
		}
		else
		{
			if (!_jouerSonAlarme)
			{
				_alarmSound.play();
				_jouerSonAlarme = true;
			}
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 217), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255, 0, 0, 255))));
		}
	}
	else
	{
		sf::Time tempsMs = _tempsEntreLaserEtStop2 = _tempsEntreLaserEtStop - _pacman.getTempsSansLaser();
		double ratio = tempsMs / sf::milliseconds(4000);
		if (tempsMs > sf::milliseconds(0) && tempsMs < sf::milliseconds(4000))
		{
			_alarmSound.stop();
			_jouerSonAlarme = false;
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 203), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
		}
		else if (tempsMs > sf::milliseconds(4000))
		{
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 217), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255, 0, 0, 255))));
		}
	}
	_window.draw(laserGauge);
}

void Jeu::draw(bool display)
{
	_window.clear();
	_window.draw(_map);
	drawMangeable();
	drawLaserUi();
	_window.draw(_pacman);
	_window.draw(_fantome);

	if (display)
		_window.display();
}

sf::Vector2f Jeu::choisirPosRandom()
{
	int random = rand() % _posValides.size();
	return _posValides.at(random);
}

void Jeu::choisirDessinerFruit(sf::Vector2f &posFruit)
{
	sf::Vector2f pos;
	sf::VertexArray trucADessiner(sf::Triangles);
	switch (_fruits.front()[0])
	{
	case 'p':
		break;
	case 'o':
		break;
	case 'c':			//CERISE
		//Les boules de la cerise
		for (int k = 0; k < 2; k++)
		{
			for (int i = 0; i <= 8; i++)
			{
				pos.x = 4 * cos(2 * (float)M_PI * (i - 1) / 8) + posFruit.x + k * 9;
				pos.y = 4 * sin(2 * (float)M_PI * (i - 1) / 8) + posFruit.y + 4;
				trucADessiner.append(sf::Vertex(pos, sf::Color::Red));

				//Repositionne le centre
				trucADessiner.append(sf::Vertex(sf::Vector2f(posFruit.x + k * 9, posFruit.y + 4), sf::Color(128, 0, 0, 255)));

				//Position du nouveau vertex
				pos.x = 4 * cos(2 * (float)M_PI * i / 8) + posFruit.x + k * 9;
				pos.y = 4 * sin(2 * (float)M_PI * i / 8) + posFruit.y + 4;

				trucADessiner.append(sf::Vertex(pos, sf::Color::Red));
			}
		}

		//La queue de la cerise
		//Première partie de la queue
		pos = sf::Vector2f(posFruit.x, posFruit.y + 4);
		trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
		pos.x += 6;
		pos.y -= 10;
		trucADessiner.append(sf::Vertex(pos, sf::Color(100, 0, 0, 255)));
		pos.x += 3;
		trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));

		//Deuxième partie de la queue
		pos = sf::Vector2f(posFruit.x + 10, posFruit.y + 4);
		trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
		pos.x -= 6;
		pos.y -= 10;
		trucADessiner.append(sf::Vertex(pos, sf::Color(100, 0, 0, 255)));
		pos.x -= 3;
		trucADessiner.append(sf::Vertex(pos, sf::Color(139, 69, 19, 255)));
		break;
	case 'b':
		break;
	case 'f':
		break;
	case 'm':
		break;
	case 'a':
		break;
	default:
		break;
	}

	_window.draw(trucADessiner);
}

void Jeu::ajouterFruitListe()
{
	//int random = rand() % 7 + 1;
	int random = 3;
	switch (random)
	{
	case 1:
		_fruits.push_back("pomme");
		break;
	case 2:
		_fruits.push_back("orange");
		break;
	case 3:
		_fruits.push_back("cerise");
		break;
	case 4:
		_fruits.push_back("banane");
		break;
	case 5:
		_fruits.push_back("fraise");
		break;
	case 6:
		_fruits.push_back("mure");
		break;
	case 8:
		_fruits.push_back("anana");
		break;
	default:
		break;
	}
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
			if (_mangeable[round(_pacman.getPos().x / 10)][round(_pacman.getPos().y / 10)] & mangeable::fruit)
			{
				_nombreFruit--;
			}
			_mangeable[round(_pacman.getPos().x / 10)][round(_pacman.getPos().y / 10)] = 0;
		}

		for (auto f : _fantome)
		{
			if (f->getNom() != "bleu")
				f->move(f->getDirection(), _pacman.getPos(), _map);
			else
				f->move(f->getDirection(), _fantome[0]->getPos(), _map);
			verifieSiMort(*f);
		}

		//Le temps passé est vérifié
		int duratio = (std::clock() - _temps) / (double)CLOCKS_PER_SEC;
		//À chauque 20 secondes, un fruit au hasard apparait
		if (duratio % 3 == 0)
		{
			if (!_fermerHorloge)
			{
				ajouterFruitListe();
				if (_nombreFruit <=2)
				{
					_nombreFruit++;
					sf::Vector2f randomV = choisirPosRandom();
					_mangeable[randomV.x / 10][randomV.y / 10] += mangeable::fruit;
				}
				_fermerHorloge = true;
			}
		}
		else
			_fermerHorloge = false;

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
		_shake+= 0.4;

		sf::Vector2i windowPos;
		windowPos.x = static_cast<int>(_defaultWinPos.x + rand() % static_cast<int>(_shake) - _shake/ 2);
		windowPos.y = static_cast<int>(_defaultWinPos.y + rand() % static_cast<int>(_shake) - _shake / 2);
		_window.setPosition(windowPos);
	}
	else
	{
		_shake -= 1;

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