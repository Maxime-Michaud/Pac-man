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
	using namespace winapi;
	//Initialisation de la fenêtre
	sf::VideoMode test(ScreenWidth, ScreenHeight);
	_window.create(test, "Pac-Man");
	_window.setPosition(_defaultWinPos);
	_window.setKeyRepeatEnabled(false);

	_loadingThread = new std::thread(&Jeu::loading, this);


	srand(time(NULL));

	//Lis la lsite des maps
	readMaps(maps);

	//Prépare l'interface graphique
	loadSounds();
	loadTexts();
	loadAnimations();
	_explosionNucleaire.openFromFile("exp.ogg");		  //Vidéo de l'exp nucléaire
	_score = 0;
	init();

	_scoreMap = 0;
	_nbBouleRouge = 4;
}

void Jeu::init()
{
	_nbBouleRougeMange = 0;
	loadMap();

	setTutorialText();

	_viewVector = sf::Vector2f(_window.getSize().x, _window.getSize().y);
	_view.setSize(_viewVector);
	_view.setCenter(sf::Vector2f(_window.getSize().x / 2, _window.getSize().y / 2));
	_window.setView(_view);

	//Initialisation de pacman
	srand(std::time(NULL));
	_targetfps = 60;

	_shake = 10;

	//Initialise les boules jaune et rouge à manger sur tout la map
	_nbBouleMange = 0;

	auto temp = _map.getBoolMap();
	_mangeable.clear();
	_posValides.clear();
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
			if (_mapsIterator != ++_maps.begin())
			{
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
							if (_mangeable[k][l] & mangeable::grosseBoule && abs(k - i) + abs(l - j) < 20)
								tropPres = true;
						}
					}
					if (!tropPres)		//Si aucune grosse boule n'est dans un rayon de 120 pixel, la grosse boule est placé
						_mangeable[i][j] = mangeable::grosseBoule;
					tropPres = false;
				}
			}
			
		}
	}
	if (_nbBouleRouge != -1)
		_nbBoulesTotal = _posValides.size() - _nbBouleRouge;
	else
		_nbBoulesTotal = _posValides.size();
	_temps = std::clock();

	if (_nbBouleRouge != -1)
	{
		for (int i = 0; i < _nbBouleRouge; i++)
		{
			sf::Vector2f temp = choisirPosRandom();
			_mangeable[temp.x / 10][temp.y / 10] = mangeable::bouleRouge;
		}
	}
	else
	{
		for (int i = 0; i < _mangeable.size(); i++)		//Vérifie tout le vecteur à la recherche de grosse boules près (12 = 120 pixel pres)
		{
			for (int j = 0; j < _mangeable[i].size(); j++)
			{
				if(_mangeable[i][j])
					_mangeable[i][j] = mangeable::bouleRouge;
			}
		}
	}
	

	//SET DES SONS, TEXTES ET VIDÉOS
	_explosionNucleaire.openFromFile("exp.ogg");		  //Vidéo de l'exp nucléaire

	if (_mapsIterator != ++_maps.begin())
		_sons.play("intro");
	else
		_sons.play("MusicIntro");


}

void Jeu::readMaps(std::string maps)
{
	std::ifstream mapList;
	mapList.open(maps);

	while (mapList.peek() != EOF)
	{
		std::string map;
		int minScore;
		mapList >> map >> minScore;

		_maps.push_back(std::pair<std::string, int>(map, minScore));
	}

	_mapsIterator = _maps.begin();
	mapList.close();
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

void Jeu::loading()
{
	_loading = true;
	_stopPause = true;
	sf::RenderWindow temp;
	sf::Text loadingPleaseWait;
	sf::Font tempFont;
	tempFont.loadFromFile("steelfish rg.ttf");
	loadingPleaseWait = sf::Text("Chargement, veuillez patienter", tempFont, 45);
	loadingPleaseWait.setPosition(sf::Vector2f(20, 300));
	loadingPleaseWait.setColor(sf::Color::Black);
	loadingPleaseWait.setStyle(sf::Text::Style::Bold);
	//_window.setVisible(false);
	temp.create(sf::VideoMode(400, 400), "loading",sf::Style::None);
	temp.setPosition(sf::Vector2i(winapi::ScreenWidth / 3, winapi::ScreenHeight / 3 - 100));
	_loadingMovie.openFromFile("loading.ogv");
	temp.setFramerateLimit(60);
	_loadingMovie.fit(sf::FloatRect(0, 0, 400, 400));
	_loadingMovie.play();
	_loadingMovie.update();

	while (_loading)
	{
		temp.clear();
		_loadingMovie.update();
		temp.draw(_loadingMovie);
		temp.draw(loadingPleaseWait);
		temp.display();
		if (!_loadingMovie.getStatus())
		{
			_loadingMovie.play();
		}

	}

	sf::Event event;
	temp.pollEvent(event);
	while (temp.pollEvent(event));
	event = sf::Event();
	loadingPleaseWait.setString("Appuyer sur une touche");
	while (event.type != sf::Event::KeyReleased && event.type != sf::Event::LostFocus)
	{
		temp.pollEvent(event);
		temp.clear();
		_loadingMovie.update();
		temp.draw(_loadingMovie);
		temp.draw(loadingPleaseWait);
		temp.display();
		if (!_loadingMovie.getStatus())
		{
			_loadingMovie.play();
		}
	}

	_window.requestFocus();
	_stopPause = false;
	_loadingThread->detach();
	delete _loadingThread;

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
	if (!(_mapsIterator == ++_maps.begin())){
		if (_pacman.getPowerUps(4))
			_ui.playAnimation("etoile");
		else
			_ui.stopAnimation("etoile");
	}
}

void Jeu::drawGunUI()
{
	if (_mapsIterator != ++_maps.begin())
	{
		_ui.setFrames("gun", 1);
		_ui.changeText("gun", "Munitions: " + std::to_string(_pacman.getNbBalles()));
	}
	else
	{
		_ui.setFrames("gun", 0);
	}
}

//Dessiner le UI du dragonShout
void Jeu::drawDragonShoutUi()
{
	if (_mapsIterator != ++_maps.begin())
		_ui.playAnimation("dragonUI");
	else
	{
		if (_pacman.getNbDragonShout() != 1)
		{
			_pacman.setDragonShout(1);
		}
		_ui.stopAnimation("dragonUI");
	}
		
}

//Draw le ui du laser
void Jeu::drawLaserUi()
{
	if (_mapsIterator != ++_maps.begin())
	{
		_ui.setFrames("laser", 1);
		_ui.changeText("laser", "Laser overdrive : " + std::to_string(_pacman.getTempsLaserRestant()));
		_ui.playAnimation("laser");
	}
	else
	{
		_ui.setFrames("laser", 0);
		_ui.stopAnimation("laser");
	}
}

void Jeu::draw(bool display)
{

	_window.clear();

	if (!_pacman.getPowerUps(4))
	{
		_sons.stop("etoile");
	}

	drawEtoileUi();

	drawLaserUi();

	if (_pacman.getPowerUps(5))
		drawDragonShoutUi();

	_window.draw(_map);
	drawMangeable();

	_window.draw(_pacman);
	_window.draw(_fantome);

	for (auto f : _fantome)
	{
		if (f->getExploser())
			f->playExplosion(_window);
	}

	drawGunUI();
	_window.draw(_ui);

	//si pacman a un gun, dessine une mire
	if (_pacman.getPowerUps(3))
		_window.draw(_mire);

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
	int minScore;
	std::string mapName;
	do
	{
		_nextMap = !(_mapsIterator == _maps.end());

		if (!_nextMap)
		{
			_playing = false;
			std::cout << "Tu gagnes?";
			system("pause");
			return;

		}

		mapName = _mapsIterator->first;
		minScore = _mapsIterator->second;
		++_mapsIterator;
	} while (_scoreMap < minScore);

	_scoreMap = 1;

	if (_mapsIterator != ++_maps.begin())
		_loadingThread = new std::thread(&Jeu::loading, this);
	for (auto f : _fantome)
		delete f;

	_fantome.clear();

	_map.initFlash(false, "", 0, 0, 0, 0, 0);


	
	std::ifstream in;
	in.open(mapName);
	sf::Vector2f pos;

	std::string couleur;
	int count = 1;

	_pacman.resetPowerUps();

	if (tolower(static_cast<char>(in.peek())) == 'p')
	{
		//Skip le p
		in.get();

		//Lis la position de pacman
		auto maybePos = readNumFromStream<float, false>(in, 2, " (,)", '.');

		if (maybePos.size() == 2)
			pos = sf::Vector2f(maybePos[0], maybePos[1]);
		else
			pos = _startpos;

		_startpos = pos;
		_pacman.setPos(pos);

		//Ajoute les power-up
		while (in.peek() != '\n')
		{
			//Skip
			while (!isalpha(in.peek()) && in.peek() != EOF && in.peek() != '\n')
			{
				in.get();
			}


			if (in.peek() != EOF && in.peek() != '\n')	//On a un mot
			{
				std::string powerUp;
				int temps;
				in >> powerUp;
				in >> temps;
				_pacman.setPowerUps(powerUp, true);
				_pacman.changerTempsPowerUp(powerUp, temps);
			}
		}
	}

	while (in.peek() != 'f') in.get();

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

		//Ajoute les power up
		if (in.peek() != '\n')
		{
			//Skip
			while (!isalpha(in.peek()) && in.peek() != EOF && in.peek() != '\n')
			{
				in.get();
			}


			if (in.peek() != EOF && in.peek() != '\n')	//On a un mot
			{
				std::string powerUp;
				in >> powerUp;
				for (int i = 0; i < count; i++)
				{
					fantomes[i]->setPowerUp(powerUp, true);
				}
				
			}
		}

		for (int i = 0; i < count; i++)
		{
			fantomes[i]->setPos(pos);
			fantomes[i]->setDeathPoint(pos);
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

	while (in.peek() == '\n') in.get();
	if (in.peek() != EOF)
		std::getline(in, _mapMsg);

	std::string var;	//OUI C'EST UN NOM SIGNIFICATIF CAR JE LIS UNE VARIABLE INCONNUE! :)

	while (in.peek() != EOF)
	{
		while (in.peek() == '\n') in.get();
		if (in.peek() != EOF)
			std::getline(in, var);

		auto params = split(var);

		char type = var.c_str()[0];

		switch (type)
		{
			case 'f':
				assert(params.size() == 7);
				remplacer(params[1], '_', ' ');

				_map.initFlash(true,						//Flash
								params[1],					//Texte a afficher
								atoi(params[2].c_str()),	//Fréquence
								atoi(params[3].c_str()),	//Durée
								atoi(params[4].c_str()),	//Position (X)
								atoi(params[5].c_str()),	//Position (Y)
								atoi(params[6].c_str()));	//Grosseur du texte
				break;

			case 'c':
				assert(params.size() == 5);
				_map.setColor(atoi(params[1].c_str()),		//R
								atoi(params[2].c_str()),	//G
								atoi(params[3].c_str()),	//B
								atoi(params[4].c_str()));	//A
				break;

			case 'e':
				assert(params.size() == 2);

				for (auto f : _fantome)
					if (typeid(*f) == typeid(FantomeRose))	//Cette variable affecte uniquement les fantomes roses
						dynamic_cast<FantomeRose*>(f)->setNbEssai(atoi(params[1].c_str()));
				break;

			case 'r':
				assert(params.size() == 2);

				for (auto f : _fantome)
					if (typeid(*f) == typeid(FantomeRose))	//Cette variable affecte uniquement les fantomes roses
						dynamic_cast<FantomeRose*>(f)->setFreqRecalc(atoi(params[1].c_str()));
				break;

			case 'b':
				assert(params.size() == 2);
				_nbBouleRouge = atoi(params[1].c_str());
				break;
		default:
			in.get();
			break;
		}
	}
	_pacman.setLigne(_map.quelleLigne(_pacman.getPos(), 0));
	_pacman.setVertical(_map.getLigne(_pacman.getNumLigne()).isVertical());
}

void Jeu::donnerUnPowerUpPacman()
{
	int random = rand() % 3 + 1;
	if (random == 2)
		random = 4;
	switch (random)
	{
	case 1:
		_pacman.setPowerUps(1, true);
		_pacman.changerTempsPowerUp(1, 2000);
		break;
	case 3:
		_pacman.setPowerUps(3, true);
		_pacman.changerTempsPowerUp(3, rand() % 2 + 1);
		break;
	case 4:
		_sons.play("etoile");
		if (_pacman.getPowerUps(4))
			_pacman.startClockEtoile();

		_pacman.setPowerUps(4, true);
		_pacman.changerTempsPowerUp(4, 5000);
		break;
	case 5:
		//Affiche le texte 
		_ui.Text("dragon").setColor(sf::Color(200, 200, 200, 255));
		_ui.playAnimation("dragon");

		//jouer son dragonshou learned
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
	_loading = false;
	if (_mapsIterator != ++_maps.begin())
		pause(_mapMsg);
		
	_nextMap = false;
	sf::Event event;
	bool tempsNormal = true;
	while (_playing)
	{
		if (!tempsNormal)
		{
			_pacman.setLigne(_map.quelleLigne(_pacman.getPos(), 0));
			_pacman.setVertical(_map.getLigne(_map.quelleLigne(_pacman.getPos(), 0)).isVertical());
			
			for (auto f : _fantome)
			{
				f->setLigne(_map.quelleLigne(f->getPos(), 0));
				f->setVertical(_map.getLigne(_map.quelleLigne(f->getPos(), 0)).isVertical());
			}
		}

		tempsNormal = true;

		//Fais une pause a la fin de la boucle en attendant d'arriver a un temps voulu
		sf::Clock clock;
		//Vérifie l'entrée de l'utilisateur
		auto keys = getKeyPress();
		for (char c : keys)
		{
			//Autowin
			if (c == 'm')
			{
				_nbBouleMange = _nbBoulesTotal;
			}
			else if (c == 'p' && _mapsIterator == ++_maps.begin())
			{
				_sons.stopAll();
				_playing = false;
				_fruits.vider();
				init();
			}
			else if (c == 'f' && _dragonShoutDesactive)
			{
				c = '\0';
			}
			else if (c == 'r' && _pacman.getPowerUps(2))
			{
				tempsNormal = false;
			}

			_pacman.input(c);
		}

		shakeScreen();

		if (tempsNormal)
		{
			_pacman.move(_pacman.getDirection(), _map);

			int x = round(_pacman.getPos().x / 10);
			int y = round(_pacman.getPos().y / 10);
			//Vérifie si la case contient un élément mangeable
			if (_mangeable[x][y])
			{
				_sons.play("chomp");

				_score += 1;
				_scoreMap += 1;
				if (_mangeable[x][y] & mangeable::fruit) //Si c'est un fruit, l'enlève
				{
					_sons.play("fruit");
					_sons.stop("chomp");	//Arretele son des boules

					_score += 20;
					_scoreMap += 20;
					std::cout << "pacman mange un fruit" << std::endl;
					_fruits.retirerFruitManger(sf::Vector2f(x * 10, y * 10));
					_nbFruitMange++;
					if (_nbFruitMange % 3 == 0)
						donnerUnPowerUpPacman();

				}

				if (_mangeable[x][y] & mangeable::grosseBoule)
				{
					donnerUnPowerUpPacman();
				}

				if (_mangeable[x][y] & mangeable::boule || _mangeable[x][y] & mangeable::grosseBoule)
					_nbBouleMange++;
				else if (_mangeable[x][y] & mangeable::bouleRouge)
					_nbBouleRougeMange++;
				if (_nbBouleMange >= _nbBoulesTotal || _nbBouleRougeMange >= _nbBoulesTotal)
				{
					_sons.stopAll();
					_pacman.stopSounds();
					_sons.play("gagne");
					Sleep(5500);
					_fruits.vider();
					init();
					break;
				}
				_mangeable[x][y] = 0;

				_ui.changeText("score", "Score  " + std::to_string(_score));
			}
		}
		if (_pacman.getLaser())
		{
			sf::Vector2f end;
			switch (_pacman.getDirection())
			{
			case 'a':
				end = sf::Vector2f(50, _pacman.getPos().y);
				break;
			case 'd':
				end = sf::Vector2f(600, _pacman.getPos().y);
				break;	
			case 's':
				end = sf::Vector2f( _pacman.getPos().x, 600);
				break;
			case 'w':
				end = sf::Vector2f(_pacman.getPos().x, 50);
				break;
			default:
				break;
			}

			if (end != _pacman.getPos())
				_map.ajouterLigne(Ligne(_pacman.getPos(), end));

		}

		if (_map.getChanged())
			checkLines();
		if (tempsNormal)
		{ 
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
					if (f->getPos().x < 30 || f->getPos().x > 660 || f->getPos().y < 30 || f->getPos().y > 620)
					{
						f->setIsDead(true);
						continue;
					}

					if (_mangeable[f->getPos().x / 10][f->getPos().y / 10] & mangeable::bouleRouge)
					{
						f->setPowerUp(1, true);
						f->resetClockAlahuAkbar();
						_mangeable[f->getPos().x / 10][f->getPos().y / 10] = 0;
						_nbBouleRougeMange++;
						if (_nbBouleRougeMange >= _nbBoulesTotal)
						{
							_sons.stopAll();
							_pacman.stopSounds();
							_sons.play("gagne");
							Sleep(5500);
							_fruits.vider();
							init();
							break;
						}
					}
				}

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
						int reculX = 150 / ratioX;
						int reculY = 150 / ratioY;
						if (reculX > 350)
							reculX = 350;
						if (reculY > 350)
							reculY = 350;
						if (reculX < -350)
							reculX = -350;
						if (reculY < -350)
							reculY = -350;
						if (_pacman.getNbDragonShout() >= 2)
						{
							if (reculX < 0)
								reculX = -1000;
							else
								reculX = 1000;
							if (reculY < 0)
								reculY = -1000;
							else
								reculY = 1000;
						}
						posRecul.x = f->getPos().x - reculX;
						posRecul.y = f->getPos().y - reculY;
						f->setDragonShoutEffect(posRecul);
					}

				}
				//Tue dans un range quand il explose
				if (f->getExploser())
				{
					for (auto f2 : _fantome)
					{
						if (abs(f->getPos().x - f2->getPos().x) + abs(f->getPos().y - f2->getPos().y) < 150)
						{
							f2->setIsDead(true);
						}
					}
					if (abs(f->getPos().x - _pacman.getPos().x) + abs(f->getPos().y - _pacman.getPos().y) < 150)
					{
						if (!_pacman.getInvincible() && !_pacman.getPowerUps(4))
							killPacman();
					}
				}
			}
		}

		_dragonShoutEffect = false;
		//Pour vérifier l'effect du dragon shout une seule fois
		/*if (!_dragonShoutEffect && _pacman.getTempsDragonShout() > 5000)
			_dragonShoutEffect = true;*/
			

		//Le temps passé est vérifié
		int duration = (std::clock() - _temps) / (double)CLOCKS_PER_SEC;
		//À chaque 3 secondes, un fruit au hasard apparait
		if (duration % 3 == 0)
		{
			if (!_fermerHorloge)
			{
				sf::Vector2f randomV = choisirPosRandom();
				//ici les fruits
				if (_mapsIterator != ++_maps.begin())
				{
					if (_fruits.getNombreFruitSurMap() < 3)
					{
						_fruits.ajouterFruitListe(randomV);
						_mangeable[randomV.x / 10][randomV.y / 10] = mangeable::fruit | _mangeable[randomV.x / 10][randomV.y / 10];
					}
				}
				
				
				_fermerHorloge = true;
			}
		}
		else
			_fermerHorloge = false;

		if (_pacman.getDragonShoutActivated() && _pacman.getTempsDragonShout() < 2000 && _pacman.getNbDragonShout() >= 2)
		{
			_dragonShoutDesactive = true;
			_view.setSize(sf::Vector2f(_viewVector.x += _pacman.getTempsDragonShout() / 15, _viewVector.y += _pacman.getTempsDragonShout() / 15));
			_window.setView(_view);
		}
		else if (_pacman.getDragonAnimation() && _pacman.getTempsDragonShout() > 2500 && _pacman.getNbDragonShout() >= 2)
		{
			_view.setSize(sf::Vector2f(_viewVector.x -= _pacman.getTempsDragonShout() / 145, _viewVector.y -= _pacman.getTempsDragonShout() / 145));
			if (_viewVector.x < winapi::ScreenWidth)
				_viewVector.x = winapi::ScreenWidth;
			if (_viewVector.y < winapi::ScreenHeight)
				_viewVector.y = winapi::ScreenHeight;
			_window.setView(_view);
			if (!_megaDragonShout && _pacman.getTempsDragonShout() > 3900)
			{
				_megaDragonShout = true;
				_pacman.incrementeurDragonShout(-2);
				_dragonShoutDesactive = false;
			}

		}
		else
			_megaDragonShout = false;

		if (!tempsNormal)
		{
			_pacman.goBack(_map);

			for (auto f : _fantome)
				f->goBack(_map);
		}


		draw();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / _targetfps);
	}

	if (_nextMap)
	{
		_playing = true;
		play();
	}
}

void Jeu::captureEvent()
{
	if (_window.pollEvent(event))
		switch (event.type)
		{
			case sf::Event::MouseMoved:
				_mire.setCentre(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
				break;
			case sf::Event::MouseButtonPressed:
				if (_pacman.getPowerUps(3) && !_pacman.getPaused() && !_sons.isPlaying("gun"))
				{
					_sons.play("gun");

					if (_pacman.getPos().x > event.mouseButton.x - 21 && _pacman.getPos().x < event.mouseButton.x + 21 &&
						_pacman.getPos().y > event.mouseButton.y - 21 && _pacman.getPos().y < event.mouseButton.y + 21)
						killPacman();
					else
						for (auto f : _fantome)
							if (f->getPos().x > event.mouseButton.x - 21 && f->getPos().x < event.mouseButton.x + 21 &&
								f->getPos().y > event.mouseButton.y - 21 && f->getPos().y < event.mouseButton.y + 21)
								f->setIsDead(true);

					_pacman.shoot();
				}
				break;
			// fenêtre fermée
			case sf::Event::Closed:
				_window.close();
				_playing = false;
				break;
			case sf::Event::LostFocus:
				if (!_stopPause)
				{
					pause();
				}
				event = sf::Event();
				break;
			case sf::Event::Resized:
				break;
			case sf::Event::MouseWheelMoved:
				_view.setSize(sf::Vector2f(_viewVector.x += event.mouseWheel.delta *10, _viewVector.y += event.mouseWheel.delta *10));
				_window.setView(_view);
				draw();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::F9)
					donnerUnPowerUpPacman();
				break;
			default:
				break;
		}
}
std::string Jeu::getKeyPress()
{
	std::string keys;
	captureEvent();
	//Les touches numérotées de 0 a 25 représentent l'alphabet
	for (int i = 0; i < 26; i++)
		if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i))
			keys += char(i + 'a');
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		_playing = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		pause();

	return keys;
}

void Jeu::pause(std::string msg)
{
	//Dessine un background pour le message
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(0, 0, 0, 225));	//noir semi-transparent
	rect.setSize(sf::Vector2f(float(_window.getSize().x), 75));
	_window.draw(rect);

	//Affiche le message
	_ui.changeText("pause", msg);
	_ui.setFrames("pause", -1);
	_ui.setFrames("score", 0);
	
	//Pause les personnages
	_pacman.pause();

	for (auto f : _fantome)
		f->pause();

	//Dessine le jeu
	draw();

	bool loop = true;
	while (loop)
	{
	 	captureEvent();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			_window.close();
			_playing = false;
			loop = false;
		}
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

	//Efface le texte de pause
	_ui.setFrames("pause", 0);
	_ui.setFrames("score", -1);
	//Dépause les personnages
	_pacman.unpause();

	for (auto f : _fantome)
		f->unpause();

	//Sinon les fantomes explosent en sortant de pause, la au moins ils sont encore dangereux
	for (auto &f : _fantome)
		f->resetClockAlahuAkbar();
}

void Jeu::shakeScreen()
{
	if (_pacman.getLaser() || (_pacman.getDragonAnimation() && _pacman.getTempsDragonShout() > 800 && _pacman.getNbDragonShout() >= 2))
	{
		if (_shake < 1) _shake = 1;
		_shake+= 0.7;

		//Cruauté envers les joueurs supplémentaire
		if (_pacman.getLaser() && (_pacman.getDragonAnimation() && _pacman.getTempsDragonShout() > 800 && _pacman.getNbDragonShout() >= 2))
			_shake *= 1.03;

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
	if (!_sons.isPlaying("mort"))
		_sons.play("mort");
	while (!_pacman.hasDisappeared())
	{
		sf::Clock clock;
		_window.clear();

		_window.draw(_map);
		drawMangeable();

		for (auto f : _fantome)
			_window.draw(*f);

		_pacman.deathAnimation(_window);
		_window.draw(_ui);

		for (auto f : _fantome)
		{
			if (f->getExploser())
				f->playExplosion(_window);
		}
		_window.display();

		while (clock.getElapsedTime().asMilliseconds() < 1000 / _targetfps);
	}

	_ui.setFrames("continuer", -1);
	_window.draw(_ui);
	_window.display();

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::O) && !sf::Keyboard::isKeyPressed(sf::Keyboard::N) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		//pour capturer le resize et le close
		captureEvent();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		_playing = false;

	else
	{
		_sons.play("continue");

		_pacman.respawn(_startpos);
		_pacman.setLigne(_map.quelleLigne(_startpos, 0));
		_pacman.setVertical(_map.getLigne(_map.quelleLigne(_startpos, 0)).isVertical());

		for (auto f : _fantome)
		{
			f->setPos(f->getDeathPoint());
 			f->setLigne(_map.quelleLigne(f->getDeathPoint(), 0));
			f->setVertical(_map.getLigne(_map.quelleLigne(f->getDeathPoint(), 0)).isVertical());
		}
	}

	_ui.setFrames("continuer", 0);
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
				_scoreMap += 30;
				_ui.changeText("score", "Score  " + std::to_string(_score));
				return true;
			}
			break;
		case 'd':
			if (fantome.getPos().y == _pacman.getPos().y && fantome.getPos().x > _pacman.getPos().x)
			{
				fantome.setIsDead(true);
				_score += 30;
				_scoreMap += 30;
				_ui.changeText("score", "Score  " + std::to_string(_score));
				return true;
			}
			break;
		case 's':
			if (fantome.getPos().x == _pacman.getPos().x && fantome.getPos().y > _pacman.getPos().y)
			{
				fantome.setIsDead(true);
				_score += 30;
				_scoreMap += 30;
				_ui.changeText("score", "Score  " + std::to_string(_score));
				return true;
			}
			break;
		case 'w':
			if (fantome.getPos().x == _pacman.getPos().x && fantome.getPos().y < _pacman.getPos().y)
			{
				fantome.setIsDead(true);
				_score += 30;
				_scoreMap += 30;
				_ui.changeText("score", "Score  " + std::to_string(_score));
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
				_sons.play("plop");
				_score += 30;
				_scoreMap += 30;
				_ui.changeText("score", "Score  " + std::to_string(_score));
				fantome.setIsDead(true);
			}
			else if (!_pacman.getInvincible())
			{
				_sons.play("mort");

				_score -= (_score / 2 > 100 ? _score / 2:100);
				_ui.changeText("score", "Score  " + std::to_string(_score));
				killPacman();
			}
			
		}
			

	}
	return false;
}

//Charge tous les sons à partir de leurs fichier
void Jeu::loadSounds()
{
	_sons.add("intro", "intro.wav");
	_sons.add("alarme", "alarm.wav");
	_sons.add("chomp", "chomp.wav");
	_sons.add("fruit", "fruit.wav");
	_sons.add("mort", "mort.wav");
	_sons.add("continue", "continue.wav");
	_sons.add("gagne", "gg.wav");
	_sons.add("nuke", "deadSound0.wav");
	_sons.add("plop", "plop.wav");
	_sons.add("MusicIntro", "musicIntro.wav");
	_sons.add("gun", "shotgun.wav");
}

void Jeu::checkLines()
{
	_pacman.setLigne(_map.quelleLigne(_pacman.getPos(), 0));
	_pacman.setVertical(_map.getLigne(_pacman.getNumLigne()).isVertical());

	for (auto &f : _fantome)
	{
		f->setLigne(_map.quelleLigne(f->getPos(), 0));
		f->setVertical(_map.getLigne(f->getNumLigne()).isVertical());

	}
}

void Jeu::loadAnimations()
{
	_ui.addAnimation("dragon", new std::function < bool() >{ 
		[&, this](){
		float alpha = _ui.Text("dragon").getColor().a - 1;
		alpha = alpha > 0 ? alpha : 0;
		_ui.Text("dragon").setColor(sf::Color(200, 200, 200, alpha));
		return alpha != 0;
	} 
	});

	_ui.addAnimation("laser", new std::function < bool() >{
		[&, this]() {
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
				_sons.play("alarme");
		
				int test = (std::clock() - _temps) / (double)CLOCKS_PER_SEC * 1000;
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, 0), sf::Color(255, 0, 0, test % 200))));
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(_window.getSize().x, 0), sf::Color(255, 0, 0, test % 200))));
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(_window.getSize().x, _window.getSize().y), sf::Color(255, 0, 0, test % 200))));
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(0, _window.getSize().y), sf::Color(255, 0, 0, test % 200))));
		
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 203), sf::Color(255, 0, 0, 255))));
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 203), sf::Color(255, 0, 0, 255))));
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(847, 217), sf::Color(255, 0, 0, 255))));
				laserGauge.append(sf::Vertex(sf::Vertex(sf::Vector2f(653, 217), sf::Color(255, 0, 0, 255))));
				if (tempsMs > sf::milliseconds(5000))	//Si le laser atteint le point de non retour de 8 secondes, tout explose
				{
					_sons.stopAll();
					_sons.play("nuke");
		
					_window.setFramerateLimit(60);
					_explosionNucleaire.fit(sf::FloatRect(0, 0, _window.getSize().x, _window.getSize().y));
					_explosionNucleaire.play();
					_explosionNucleaire.update();
					_window.setPosition(sf::Vector2i(0, 0));
		
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
				_sons.stop("alarme");
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
		
		_window.draw(laserGauge);

		//On veut continuer l'animation
		return true;
	} });

	_ui.addAnimation("dragonUI", new std::function<bool()>{
		[&, this]() {
		if (_pacman.getTempsDragonShout() < 2000)
		{
			_ui.playAnimation("dragon");
		}

		//Écris le nombre de dragon shouts disponible
		if (!_ui.hasText("shouts"))
			_ui.addText("shouts", "Dragon shout available: " + std::to_string(_pacman.getNbDragonShout()), "steelfish rg.ttf", sf::Vector2f(650, 310));
		else
			_ui.changeText("shouts", "Dragon shout available: " + std::to_string(_pacman.getNbDragonShout()));

		//On veut continuer l'animation
		return true;
	} });

	_ui.addAnimation("etoile", new std::function<bool()>{
		[&, this]() {// crée une forme vide
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
	return _pacman.getTempsEtoile() < 5000;
	}
	});
}

void Jeu::loadTexts()
{

	_ui.addText("laser", "Laser overdrive: ", "steelfish rg.ttf", sf::Vector2f(650, 150), 45);

	//Affiche le nombre de balles disponibles
	_ui.addText("gun", "Munitions: 0", "steelfish rg.ttf", sf::Vector2f(650, 350));

	//Construit le texte pour le menu de pause, mais ne l'affiche pas (frames = 0)
	_ui.addText("pause", "Appuyez sur espace pour continuer", "steelfish rg.ttf", sf::Vector2f(), 60, sf::Color::White, 0);
	//Construit le texte pour continuer sans l'afficher
	_ui.addText("continuer", "Voulez-vous continuer? (O/n)", "steelfish rg.ttf", sf::Vector2f(), 60, sf::Color::White, 0);
	//Ajoute le texte du score
	_ui.addText("score", "Score  ", "8bit.ttf", sf::Vector2f());
	//Prépare les runes pour quand on apprend un dragon shout. Il disparait en devenant transparent, pas en n'étant pas dessiné
	_ui.addText("dragon", "Fus Roh Dah", "dragon.otf", sf::Vector2f(200, 10), 30, sf::Color(200, 200, 200, 0), -1);


	//Ajoute les textes pour le premier niveau qui expliquent les controles
	_ui.addText("tutW", "W", "keyboard.otf", sf::Vector2f(815, 45), 75);
	_ui.Text("tutW").setStyle(sf::Text::Bold);

	_ui.addText("tutASD", "ASD", "keyboard.otf", sf::Vector2f(750, 110), 75);
	_ui.Text("tutASD").setStyle(sf::Text::Bold);

	_ui.addText("tutMove", "Déplacements", "steelfish rg.ttf", sf::Vector2f(975, 90), 60);

	_ui.addText("tutL", "L", "keyboard.otf",sf::Vector2f(750, 200),75);
	_ui.Text("tutL").setStyle(sf::Text::Bold);

	_ui.addText("tutLaser", "Laser", "steelfish rg.ttf", sf::Vector2f(825, 210), 60);

	_ui.addText("tutF", "F", "keyboard.otf", sf::Vector2f(750, 280), 75);
	_ui.Text("tutF").setStyle(sf::Text::Bold);

	_ui.addText("tutShout", "Fus Roh Dah", "steelfish rg.ttf", sf::Vector2f(825, 290), 60);
	_ui.addText("tutMegaShout", "3 X Fus Roh Dah = surprise", "steelfish rg.ttf", sf::Vector2f(700, 350), 60);

	_ui.addText("tutGun", "Si le curseur change, essayez de cliquer!", "steelfish rg.ttf", sf::Vector2f(700, 425), 60);

	_ui.addText("tutPause", "Espace pour faire pause", "steelfish rg.ttf", sf::Vector2f(700, 500), 60);
	_ui.addText("tutFruit", "3 Fruits = 1 power up", "steelfish rg.ttf", sf::Vector2f(700, 575), 60);
}

//Cache les texte du tutorial si nécéssaire
void Jeu::setTutorialText()
{
	if (_mapsIterator != ++_maps.begin())
	{
		_ui.setFrames("tutW", 0);
		_ui.setFrames("tutASD", 0);
		_ui.setFrames("tutMove", 0);
		_ui.setFrames("tutL", 0);
		_ui.setFrames("tutLaser", 0);
		_ui.setFrames("tutF", 0);
		_ui.setFrames("tutShout", 0);
		_ui.setFrames("tutMegaShout", 0);
		_ui.setFrames("tutGun", 0);
		_ui.setFrames("tutPause", 0);
		_ui.setFrames("tutFruit", 0);
	}
}