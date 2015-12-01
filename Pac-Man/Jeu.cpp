/************************************************************************************
*Date:			25 Octobre 2015														*
*Programme:		Jeu.cpp																*
*Programmeur:	Maxime Michaud-Corriveau											*
*Description:	Classe principale du jeu de Pac-man. Contient tout ce dont on 		*
*				a besoin pour pouvoir lancer une partie de pac-man facilement.		*
*************************************************************************************/
#include "Jeu.h"

Jeu::Jeu(std::string maps)
{
	srand(time(NULL));

	using namespace winapi;

	//Lis la lsite des maps
	readMaps(maps);

	//Initialisation de la fenêtre
	sf::VideoMode test(ScreenWidth, ScreenHeight);
	_window.create(test, "Pac-Man");
	_window.setPosition(_defaultWinPos);
	_window.setKeyRepeatEnabled(false);

	init();

	_laserText.setPosition(sf::Vector2f(650, 150));
	_alarmBuffer.loadFromFile("alarm.wav");				  //L'alarme quand le laser est trop utilisé
	_alarmSound.setBuffer(_alarmBuffer);
	_introBuffer.loadFromFile("intro.wav");				  //La petite musique d'intro
	_intro.setBuffer(_introBuffer);
	_explosionNucleaire.openFromFile("exp.ogg");		  //Vidéo de l'exp nucléaire
	_intro.play();
	_chompBuffer.loadFromFile("chomp.wav");				  //Son quand pac-man mange une boule
	_chomp.setBuffer(_chompBuffer);
	_fruitBuffer.loadFromFile("fruit.wav");				  //Son quand pac-man mange une boule
	_fruit.setBuffer(_fruitBuffer);
	_mortBuffer.loadFromFile("mort.wav");				  //Son quand pac-man meurt
	_mort.setBuffer(_mortBuffer);
	_continueBuffer.loadFromFile("continue.wav");		  //Son quand le joueur continue de jouer
	_continue.setBuffer(_continueBuffer);
	_ggBuffer.loadFromFile("gg.wav");
	_gg.setBuffer(_ggBuffer);
	_megaDeadBuffer.loadFromFile("deadSound0.wav");
	_megaDead.setBuffer(_megaDeadBuffer);
}

void Jeu::init()
{
	loadMap();
	//Initialisation de pacman
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
	_explosionTextureComplet.loadFromFile("explosion.png");
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			_explosionTextureRect[i][j] = sf::IntRect(i * 256, j * 256, 256, 256);
			_explosionTexture[i][j].setTexture(&_explosionTextureComplet);
			_explosionTexture[i][j].setTextureRect(_explosionTextureRect[i][j]);
		}
	}

	_ghostStart = _fantome[0]->getPos();

	//charge la police
	_font.loadFromFile("steelfish rg.ttf");
	_8bitFont.loadFromFile("8bit.ttf");
	_dragonFont.loadFromFile("dragon.otf");

	_dragonShoutLearned = sf::Text("Fus Roh Dah", _dragonFont, 40);

	_targetfps = 60;

	_shake = 10;

	//Initialise les boules jaune et rouge à manger sur tout la map
	_nbBouleRouge = 4;

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
				bool tropPres = false;		//Pour savoir si une autre boule est trop pres
				for (int k = 0; k < _mangeable.size(); k++)		//Vérifie tout le vecteur à la recherche de grosse boules près (12 = 120 pixel pres)
				{
					for (int l = 0; l < _mangeable[k].size(); l++)
					{
						if (_mangeable[k][l] & mangeable::grosseBoule && abs(k-i) + abs(l-j) < 6)
							tropPres = true;
					}
				}
				if (!tropPres)		//Si aucune grosse boule n'est dans un rayon de 120 pixel, la grosse boule est placé
					_mangeable[i][j] = mangeable::grosseBoule;
				tropPres = false;
			}
		}
	}
	_nbBoulesTotal = _posValides.size() - _nbBouleRouge;
	_temps = std::clock();

	for (int i = 0; i < _nbBouleRouge; i++)
	{
		sf::Vector2f temp = choisirPosRandom();
		_mangeable[temp.x / 10][temp.y / 10] = mangeable::bouleRouge;
	}

	//SET DES SONS, TEXTES ET VIDÉOS
	_laserText = sf::Text("Laser overdredive:", _font, 45);
	_scoreTxt = sf::Text("Score " + _score, _8bitFont, 20);

	_laserText.setPosition(sf::Vector2f(650, 150));
	_alarmBuffer.loadFromFile("alarm.wav");				  //L'alarme quand le laser est trop utilisé
	_alarmSound.setBuffer(_alarmBuffer);
	_introBuffer.loadFromFile("intro.wav");				  //La petite musique d'intro
	_intro.setBuffer(_introBuffer);
	_explosionNucleaire.openFromFile("exp.ogg");		  //Vidéo de l'exp nucléaire
	_intro.play();
	_chompBuffer.loadFromFile("chomp.wav");				  //Son quand pac-man mange une boule
	_chomp.setBuffer(_chompBuffer);
	_chomp.setVolume(50);
	_fruitBuffer.loadFromFile("fruit.wav");				  //Son quand pac-man mange une boule
	_fruit.setBuffer(_fruitBuffer);
	_mortBuffer.loadFromFile("mort.wav");				  //Son quand pac-man meurt
	_mort.setBuffer(_mortBuffer);
	_continueBuffer.loadFromFile("continue.wav");		  //Son quand le joueur continue de jouer
	_continue.setBuffer(_continueBuffer);
	_ggBuffer.loadFromFile("gg.wav");					  //Son quand le joueur fini une map
	_gg.setBuffer(_ggBuffer);
	_megaDeadBuffer.loadFromFile("deadSound0.wav");
	_megaDead.setBuffer(_megaDeadBuffer);
	_starBuffer.loadFromFile("star.wav");
	_star.setBuffer(_starBuffer);
	_plopBuffer.loadFromFile("plop.wav");
	_plop.setBuffer(_plopBuffer);
	_alahuAkbarBuffer.loadFromFile("alahuAkbar.wav");
	_alahuAkbar.setBuffer(_alahuAkbarBuffer);
	_dragonLearnBuffer.loadFromFile("dragonLearned.wav");
	_dragonLearned.setBuffer(_dragonLearnBuffer);
	//Reset les tampons des sons
	_gg.resetBuffer();
	_continue.resetBuffer();

}

void Jeu::readMaps(std::string maps)
{
	std::ifstream mapList;
	mapList.open(maps);

	while (mapList.peek() != EOF)
	{
		std::string map;
		std::getline(mapList, map);

		_maps.push_back(map);
	}

	_mapsIterator = _maps.begin();

}

Jeu::~Jeu()
{
	clear();
}

void Jeu::clear()
{
	for (auto f : _fantome)
		delete f;
}

void Jeu::drawMangeable()
{

	sf::Vector2f pos;				//Pour dessiner les triangles de la boule
	sf::Vector2f posBoule;			//La position de la boule
	sf::VertexArray boule(sf::Triangles);
	for (int i = 0; i < _mangeable.size(); i++)
	{
		for (int j = 0; j < _mangeable[i].size(); j++)
		{
			if (_mangeable[i][j])
			{
				if (_mangeable[i][j] & mangeable::boule)
				{
					posBoule = sf::Vector2f(i * 10, j * 10);
					for (int i = 0; i <= 8; i++)
					{
						//Position du premier point du triangle
						pos.x = 2 * cos(2 * (float)M_PI * (i - 1) / 8) + posBoule.x;
						pos.y = 2 * sin(2 * (float)M_PI * (i - 1) / 8) + posBoule.y;
						boule.append(sf::Vertex(pos, sf::Color::Yellow));

						//Repositionne le centre
						boule.append(sf::Vertex(posBoule, sf::Color::Yellow));

						//Position du nouveau dernier point
						pos.x = 2 * cos(2 * (float)M_PI * i / 8) + posBoule.x;
						pos.y = 2 * sin(2 * (float)M_PI * i / 8) + posBoule.y;
						boule.append(sf::Vertex(pos, sf::Color::Yellow));
					}
				}
				if (_mangeable[i][j] & mangeable::grosseBoule || _mangeable[i][j] & mangeable::bouleRouge)
				{
					sf::Color temp;
					if (_mangeable[i][j] & mangeable::grosseBoule)
						temp = sf::Color::Yellow;
					else
						temp = sf::Color::Red;
					
					posBoule = sf::Vector2f(i * 10, j * 10);
					for (int i = 0; i <= 8; i++)
					{
						//Position du premier point du triangle
						pos.x = 5 * cos(2 * (float)M_PI * (i - 1) / 8) + posBoule.x;
						pos.y = 5 * sin(2 * (float)M_PI * (i - 1) / 8) + posBoule.y;
						boule.append(sf::Vertex(pos, temp));

						//Repositionne le centre
						boule.append(sf::Vertex(posBoule, temp));

						//Position du nouveau dernier point
						pos.x = 5 * cos(2 * (float)M_PI * i / 8) + posBoule.x;
						pos.y = 5 * sin(2 * (float)M_PI * i / 8) + posBoule.y;
						boule.append(sf::Vertex(pos, temp));
					}
				}
			}
		}
	}
	_window.draw(boule);
	_fruits.dessinerFruits(_window);
}

void Jeu::drawEtoileUi()
{
	// crée une forme vide
	sf::ConvexShape convex;

	// définit le nombre de points (5)
	convex.setPointCount(10);

	// définit les points
	convex.setPoint(0, sf::Vector2f(715, 235));
	convex.setPoint(1, sf::Vector2f(738, 275));
	convex.setPoint(2, sf::Vector2f(785, 285));
	convex.setPoint(3, sf::Vector2f(752, 317));
	convex.setPoint(4, sf::Vector2f(758, 365));
	convex.setPoint(5, sf::Vector2f(715, 345));
	convex.setPoint(6, sf::Vector2f(675, 365));
	convex.setPoint(7, sf::Vector2f(681, 317));
	convex.setPoint(8, sf::Vector2f(648, 285));
	convex.setPoint(9, sf::Vector2f(695, 275));

	if (_nbFrame < 1)
	{
		_randColor1 = rand() % 100;
		_randColor2 = rand() % 100;
		_randColor3 = rand() % 156;
		_nbFrame = 10;
	}
	else
	{
		_nbFrame--;
	}


	convex.setFillColor(sf::Color(_randColor1 + 115, _randColor2 + 115, _randColor3 + 100, (_pacman.getTempsEtoile() / 5000) * 255));

	_window.draw(convex);
}

//Dessiner le UI du dragonShout
void Jeu::drawDragonShoutUi()
{

	if (_pacman.getTempsDragonShout() < 2000)
	{
		float ratio = _pacman.getTempsDragonShout() / 2000;
		_dragonShoutLearned.setPosition(sf::Vector2f(200, 10));
		_dragonShoutLearned.setColor(sf::Color(200, 200, 200, 255 - (ratio * 255)));
		_window.draw(_dragonShoutLearned);
	}
	std::string temp = "Dragon shout avaible: " + std::to_string(_pacman.getNbDragonShout());
	_dragonShoutText = sf::Text(temp, _font, 30);
	_dragonShoutText.setPosition(sf::Vector2f(610, 310));
	_window.draw(_dragonShoutText);

}

//Draw le ui du laser
void Jeu::drawLaserUi()
{
	std::string testStr = "Laser overdrive : " + std::to_string(_pacman.getTempsLaserRestant());
	_laserText = sf::Text(testStr, _font, 45);
	_laserText.setPosition(sf::Vector2f(650, 150));
	_window.draw(_laserText);
	sf::VertexArray laserGauge(sf::Quads);
	//Draw la boite qui contient la jauge
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(650, 200), sf::Color(200, 200, 200, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(850, 200), sf::Color(255, 255, 255, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(850, 220), sf::Color(255, 255, 255, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(650, 220), sf::Color(200, 200, 200, 255))));

	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(0, 0, 0, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 203), sf::Color(0, 0, 0, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 217), sf::Color(0, 0, 0, 255))));
	laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(0, 0, 0, 255))));
	if (_pacman.getLaser())	//Si le laser est activer
	{
		if (_tempsEntreLaserEtStop2 < sf::milliseconds(0))
		{
			_tempsEntreLaserEtStop2 = sf::milliseconds(0);
		}
		 sf::Time tempsMs = _tempsEntreLaserEtStop = _tempsEntreLaserEtStop2 + _pacman.getTempsLaser();
		
		double ratio = tempsMs / sf::milliseconds(2000);		//Le ration de sonconde passé /4
		if (tempsMs < sf::milliseconds(2000))					//Dessine la barre verte qui monte
		{
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255 * ratio, 255-(255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 203), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
		}
		else		//Sinon le laser est en over load et dessine la barre rouge seulement et joue le son d'alerte
		{
			int test = (std::clock() - _temps) / (double)CLOCKS_PER_SEC * 1000;
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, 0), sf::Color(255, 0, 0, test % 200))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(_window.getSize().x, 0), sf::Color(255, 0, 0, test % 200))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(_window.getSize().x, _window.getSize().y), sf::Color(255, 0, 0, test % 200))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, _window.getSize().y), sf::Color(255, 0, 0, test % 200))));
			if (!_alarmSound.getStatus())
			{
				_alarmSound.play();
			}
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 217), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255, 0, 0, 255))));
			if (tempsMs > sf::milliseconds(5000))	//Si le laser atteint le point de non retour de 8 secondes, tout explose
			{
				_window.setFramerateLimit(60);
				_explosionNucleaire.fit(sf::FloatRect(0, 0, _window.getSize().x, _window.getSize().y));
				_explosionNucleaire.play();
				_explosionNucleaire.update();
				_window.setPosition(sf::Vector2i(0, 0));
				_alarmSound.stop();
				_megaDead.play();
				while (_explosionNucleaire.getStatus())		//Joue le vidéo de l'explosion nucléaire et ferme le jeu
				{
					_window.clear();
					_explosionNucleaire.update();
					_window.draw(_explosionNucleaire);
					_window.display();
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						break;
					}
				}
#if !defined(_DEBUG)
				//system("Shutdown -h");
#endif
				_playing = false;
			}
		}
	}
	else	//Si le laser n'est pas activé
	{
		sf::Time tempsMs = _tempsEntreLaserEtStop2 = _tempsEntreLaserEtStop - _pacman.getTempsSansLaser();
		double ratio = tempsMs / sf::milliseconds(2000);
		if (tempsMs > sf::milliseconds(0) && tempsMs < sf::milliseconds(2000))	//Si il est entre 0 et 4 seconde, dessine la barre qui monte ou descend
		{
			_alarmSound.stop();
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 203), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653 + (ratio * 194), 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255 * ratio, 255 - (255 * ratio), 0, 255))));
		}
		else if (tempsMs > sf::milliseconds(2000))			//Sinon si il est a plus de 4 seconde, dessine la barre en rouge
		{
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 203), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 217), sf::Color(255, 0, 0, 255))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255, 0, 0, 255))));

			int test = (std::clock() - _temps) / (double)CLOCKS_PER_SEC * 1000;
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, 0), sf::Color(255, 0, 0, test % 200))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(_window.getSize().x, 0), sf::Color(255, 0, 0, test % 200))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(_window.getSize().x, _window.getSize().y), sf::Color(255, 0, 0, test % 200))));
			laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, _window.getSize().y), sf::Color(255, 0, 0, test % 200))));
		}

	}

	_window.draw(_laserText);
	_window.draw(laserGauge);
}

void Jeu::draw(bool display)
{

	_window.clear();
	if (!_pacman.getPowerUps(4))
	{
		_star.stop();
	}
	else
	{
		drawEtoileUi();
	}
	_window.draw(_map);
	drawMangeable();
	if (_pacman.getPowerUps(1))
		drawLaserUi();
	if (_pacman.getPowerUps(5))
		drawDragonShoutUi();
	_window.draw(_pacman);
	_window.draw(_fantome);
	_window.draw(_scoreTxt);
	if (display)
		_window.display();
}

sf::Vector2f Jeu::choisirPosRandom()
{
	int random;
	do		//Tant que la position choisi au hasard est a 30 pixel en x et y de pacman et ne contient pas de fruit ou de grosses boules
	{
		random = rand() % _posValides.size();
	} while (!(_mangeable[_posValides.at(random).x / 10][_posValides.at(random).y / 10] & mangeable::fruit) &&
		!(_mangeable[_posValides.at(random).x / 10][_posValides.at(random).y / 10] & mangeable::grosseBoule) && 
		abs(_posValides.at(random).x - _pacman.getPos().x) < 30
		&& abs(_posValides.at(random).y - _pacman.getPos().y) < 30);
	std::cout << "La pos random choisit est: " << _posValides.at(random).x << "," << _posValides.at(random).y << std::endl;
	return _posValides.at(random);
}


void Jeu::loadMap()
{
	if (_mapsIterator == _maps.end())
	{
		_playing = false;
		std::cout << "Tu gagnes? je sais c'Est plate comme message de fin";
		system("pause");
	}
	
	std::string mapName = *_mapsIterator;
	++_mapsIterator;
	
	std::ifstream in;
	in.open(mapName);

	std::string couleur;
	int count = 1;
	while (tolower(static_cast<char>(in.peek())) == 'f')
	{
		in.get();	//Get le f

		//Lis le type de fantome
		in >> couleur;

		//Lis le nombre a placer, 1 si le nombre est absent 
		in >> count;
		if (in.bad())
		{
			in.clear();
			count = 1;
		}

		//Lis la position du fantome
		auto maybePos = readNumFromStream<float, false>(in, 2, " (,)", '.');
		sf::Vector2f pos;

		if (maybePos.size() == 2)
			pos = sf::Vector2f(maybePos[0], maybePos[1]);
		else
			pos = _startpos;


		//Fantomes a ajouter
		Fantome** fantomes = new Fantome*[count];


		if (couleur == "rouge")
			for (int i = 0; i < count; i++)
				i[fantomes] = new FantomeRouge;
		else if (couleur == "orange")
			for (int i = 0; i < count; i++)
				i[fantomes] = new FantomeOrange;
		else if (couleur == "rose")
			for (int i = 0; i < count; i++)
				i[fantomes] = new FantomeRose;
		else if (couleur == "bleu")
			for (int i = 0; i < count; i++)
				i[fantomes] = new FantomeBleu;

		for (int i = 0; i < count; i++)
		{
			fantomes[i]->setPos(pos);
			_fantome.add(fantomes[i]);
		}

		//On ne delete pas les pointeurs car ils sont ajoutés au vecteur de fantomes
		delete[] fantomes;
		in.ignore(99999,'\n');
	}

	_map.lireMap(in);

	for (auto &f : _fantome)
	{
		f->setLigne(_map.quelleLigne(f->getPos(), 0));
		f->setVertical(_map.getLigne(f->getNumLigne()).isVertical());
	}
}

void Jeu::donnerUnPowerUpPacman()
{
	int random = rand() % 3 + 1;
	if (random == 2)
		random = 4;
	else if (random == 3)
		random = 5;
	switch (random)
	{
	case 1:
		_pacman.setPowerUps(1, true);
		_pacman.changerTempsPowerUp(1, 2000);
		break;
	case 4:
		if (_pacman.getPowerUps(4))
			_pacman.startClockEtoile();
		_star.play();
		_pacman.setPowerUps(4, true);
		_pacman.changerTempsPowerUp(4, 5000);
		break;
	case 5:
		//jouer son dragonshou learned
		_dragonLearned.play();
		_pacman.setPowerUps(5, true);
		_dragonShoutEffect = true;
		_pacman.resetClockDragon();
		_pacman.incrementeurDragonShout(1);
		break;
	default:
		break;
	}
}

void Jeu::play()
{
	pause("Appuyez sur espace pour commencer!");
	sf::Event event;
	while (_playing)
	{
		//Fais une pause a la fin de la boucle en attendant d'arriver a un temps voulu
		sf::Clock clock;
		_window.pollEvent(event);
		switch (event.type)
		{
			// fenêtre fermée
		case sf::Event::Closed:
			//_window.close();
			break;
		case sf::Event::LostFocus:
			pause();
			event = sf::Event();
			break;
		case sf::Event::Resized:
			break;
		default:
			break;
		}
		//Vérifie l'entrée de l'utilisateur
		auto keys = getKeyPress();
		for (char c : keys)
		{
			_pacman.input(c);
			/*if (c == 'm')
			{
				std::cout << "Pos pac-man: " << _pacman.getPos().x << "," << _pacman.getPos().y << std::endl;
				_fruits.imprimmerPosFruit();
				std::cout << std::endl << x << "," << y << std::endl;
			}*/	
		}
		shakeScreen();
		_pacman.move(_pacman.getDirection(), _map);

		int x = round(_pacman.getPos().x / 10);
		int y = round(_pacman.getPos().y / 10);
		//Vérifie si la case contient un élément mangeable
		if (_mangeable[x][y])
		{
			_score += 1;
			if (_mangeable[x][y] & mangeable::boule || _mangeable[x][y] & mangeable::grosseBoule)
			{
				_nbBouleMange += 1;
				if (_nbBouleMange >= _nbBoulesTotal)
				{
					_star.stop();
					_gg.play();
					Sleep(5500);	//Attends que le son de victoire joue
					init();
				}
				if (_mangeable[x][y] & mangeable::grosseBoule)
				{
					int random = rand() % 2 + 1;
					if (random == 2)
						random = 4;
					switch (random)
					{
					case 1:
						_pacman.setPowerUps(1, true);
						_pacman.changerTempsPowerUp(1, 2000);
						break;
					case 4:
						if (_pacman.getPowerUps(4))
							_pacman.startClockEtoile();
						_star.play();
						_pacman.setPowerUps(4, true);
						_pacman.changerTempsPowerUp(4, 5000);
						break;
					default:
						break;
					}
				}
			}
			if (_mangeable[x][y] & mangeable::fruit) //Si c'est un fruit, l'enlève
			{
				_score += 20;
				std::cout << "pacman mange un fruit" << std::endl;
				_fruits.retirerFruitManger(sf::Vector2f(x * 10, y * 10));
				_nbFruitMange++;
				if (_nbFruitMange % 3 == 0)
					donnerUnPowerUpPacman();
				_fruit.play();
			}
			else if (!_chomp.getStatus())
			{
				_chomp.play();
			}

			if (_mangeable[x][y] & mangeable::grosseBoule)
			{
				donnerUnPowerUpPacman();
			}
			if (_mangeable[x][y] & mangeable::boule || _mangeable[x][y] & mangeable::grosseBoule)
				_nbBouleMange++;

			if (_nbBouleMange >= _nbBoulesTotal)
			{
				_star.stop();
				_pacman.setSonDragonShout(false);
				_gg.play();
				Sleep(5500);
				_playing = false;
				break;
			}
			_mangeable[x][y] = 0;
			std::string temp = "Score  " + std::to_string(_score);
			_scoreTxt = sf::Text(temp, _8bitFont, 20);		
		}

		//Fait arrêter le son de l'alarme
		if (!_pacman.getPowerUps(1))
			_alarmSound.stop();

		//Fait les mouvements des fantomes
		for (auto f : _fantome)
		{
			if (f->getNom() != "bleu")
				f->move(f->getDirection(), _pacman.getPos(), _map);
			else
				f->move(f->getDirection(), _fantome[0]->getPos(), _map);
			verifieSiMort(*f);
			if (!f->getToucherParDragonshout())
			{
				if (_mangeable[f->getPos().x / 10][f->getPos().y / 10] & mangeable::bouleRouge)
				{
					_alahuAkbar.play();
					f->setPowerUp(1, true);
					f->resetClockAlahuAkbar();
					_mangeable[f->getPos().x / 10][f->getPos().y / 10] = 0;
				}
			}
			
			//TODO faire l'animation de l'explosion
			//if (f->getExplosionStatus())
				//_explosionTexture.update();
			if (_pacman.getDragonShoutActivated() && !f->getToucherParDragonshout() && _pacman.getTempsDragonShout() > 700)
			{
				//Si le fantome est en range du dragonShout
				if (abs(f->getPos().x - _pacman.getPos().x) < 250 && abs(f->getPos().y - _pacman.getPos().y) < 250)
				{
					sf::Vector2f posRecul;
					float x = (_pacman.getPos().x - f->getPos().x);
					float y = (_pacman.getPos().y - f->getPos().y);
					float ratioX = x / 250;
					float ratioY = y / 250;
					float reculX = 150 / ratioX;
					float reculY = 150 / ratioY;
					if (reculX > 350)
						reculX = 350;
					if (reculY > 350)
						reculY = 350;
					posRecul.x = f->getPos().x - reculX;
					posRecul.y = f->getPos().y - reculY;
					f->setDragonShoutEffect(posRecul);
				}
				
			}
				
		}
		_dragonShoutEffect = false;
		//Pour vérifier l'effect du dragon shout une seule fois
		/*if (!_dragonShoutEffect && _pacman.getTempsDragonShout() > 5000)
			_dragonShoutEffect = true;*/
			

		//Le temps passé est vérifié
		int duration = (std::clock() - _temps) / (double)CLOCKS_PER_SEC;
		//À chauque 3 secondes, un fruit au hasard apparait
		if (duration % 3 == 0)
		{
			if (!_fermerHorloge)
			{
				sf::Vector2f randomV = choisirPosRandom();
				//ici les fruits
				if (_fruits.getNombreFruitSurMap() < 3)
				{
					_fruits.ajouterFruitListe(randomV);
					_mangeable[randomV.x / 10][randomV.y / 10] = mangeable::fruit | _mangeable[randomV.x / 10][randomV.y / 10];
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
		_shake -= 0.4;

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

	//RAJOUTÉ POUR ÉVITÉ DE PLANTER DANS CETTE PARTIE QUAND RESIZE
	sf::Event event;
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::O) && !sf::Keyboard::isKeyPressed(sf::Keyboard::N))
	{
		_window.pollEvent(event);
		switch (event.type)
		{
			// fenêtre fermée
		case sf::Event::Closed:
			//_window.close();
			break;
		case sf::Event::LostFocus:
			break;
		case sf::Event::Resized:
			break;
		default:
			break;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		_playing = false;

	else
	{
		_continue.play();
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
				_score += 30;
				std::string temp = "Score  " + std::to_string(_score);
				_scoreTxt = sf::Text(temp, _8bitFont, 20);
				return true;
			}
			break;
		case 'd':
			if (fantome.getPos().y == _pacman.getPos().y && fantome.getPos().x > _pacman.getPos().x)
			{
				fantome.setIsDead(true);
				_score += 30;
				std::string temp = "Score  " + std::to_string(_score);
				_scoreTxt = sf::Text(temp, _8bitFont, 20);
				return true;
			}
			break;
		case 's':
			if (fantome.getPos().x == _pacman.getPos().x && fantome.getPos().y > _pacman.getPos().y)
			{
				fantome.setIsDead(true);
				_score += 30;
				std::string temp = "Score  " + std::to_string(_score);
				_scoreTxt = sf::Text(temp, _8bitFont, 20);
				return true;
			}
			break;
		case 'w':
			if (fantome.getPos().x == _pacman.getPos().x && fantome.getPos().y < _pacman.getPos().y)
			{
				fantome.setIsDead(true);
				_score += 30;
				std::string temp = "Score  " + std::to_string(_score);
				_scoreTxt = sf::Text(temp, _8bitFont, 20);
				return true;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		if ( (isBetween(_pacman.getPos().x + 5 - _pacman.Width, fantome.getPos().x - fantome.Width, fantome.getPos().x + fantome.Width) || //Coté gauche de pacman dans le fantome
			isBetween(_pacman.getPos().x - 5 + _pacman.Width, fantome.getPos().x - fantome.Width, fantome.getPos().x + fantome.Width)) && //Coté droit de pacman dans le fantome
			(isBetween(_pacman.getPos().y + 5 - _pacman.Width, fantome.getPos().y - fantome.Width, fantome.getPos().y + fantome.Width) || //Coté haut de pacman dans le fantome
				isBetween(_pacman.getPos().y - 5 + _pacman.Width, fantome.getPos().y - fantome.Width, fantome.getPos().y + fantome.Width)) && //Coté bas de pacman dans le fantome
			!fantome.isDead())	//Le fantome mort ne peut pas tuer pac-man
		{
			if (_pacman.getPowerUps(4))		//Si pac a une étoile de mario il tue le fantome à la place
			{
				_plop.play();
				_score += 30;
				std::string temp = "Score  " + std::to_string(_score);
				_scoreTxt = sf::Text(temp, _8bitFont, 20);
				fantome.setIsDead(true);
			}
			else if (!_pacman.getInvincible())
			{
				_score -= 100;
				std::string temp = "Score  " + std::to_string(_score);
				_scoreTxt = sf::Text(temp, _8bitFont, 20);
				_mort.play();
				killPacman();
			}
			
		}
			

	}
	return false;
}