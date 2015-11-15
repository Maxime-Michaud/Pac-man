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
	_deathIncrement = .4f;

	_color = sf::Color(255, 255, 0, 255);

	_pos = sf::Vector2f(300, 300);
	_laserSB.loadFromFile("BWAAAAH.wav");
	_laserSound.setBuffer(_laserSB);
}

PacMan::~PacMan()
{
}

float PacMan::getTempsLaserRestant()const
{
	return _nbMilisecondeLaser - _tempsPassePowerUpLaser;
}

float PacMan::getTempsEtoile()
{
	return _nbMilisecondeEtoile - _clockEtoile.getElapsedTime().asMilliseconds();
}

void PacMan::fire()const
{

	if (!_laser)
	{
		_tempsLaser.restart();
		_laser = true;
		_laserSound.play();
	}
	_tempsPassePowerUpLaser = _tempsLaser.getElapsedTime().asMilliseconds() + _tempsMemoireLaser;
	std::cout << _tempsLaser.getElapsedTime().asMilliseconds() << std::endl;
	if (_tempsLaser.getElapsedTime().asMilliseconds() >= _nbMilisecondeLaser - _tempsMemoireLaser)
	{
		std::cout << "arrêt du laser par le temps ecouler" << std::endl;
		_powerUpLaser = false;
		_nbMilisecondeLaser = 0;
		_tempsMemoireLaser = 0;
		_tempsPassePowerUpLaser = 0;

	}
}

void PacMan::stop()const
{
	if (_laser)
	{
		//_tempsPassePowerUpLaser += _tempsLaser.getElapsedTime().asMilliseconds();
		_laser = false;
		_laserSound.stop();
		_tempsMemoireLaser += _tempsPassePowerUpLaser;
		_tempsSansLaser.restart();

	}
}

bool PacMan::getPowerUps(int powerUp)
{
	assert(powerUp > 0 && powerUp < 5);
	switch (powerUp)
	{
	case 1:
		return _powerUpLaser;
		break;
	case 2:
		return _powerUpTimeTravel;
		break;
	case 3:
		return _powerUpMindControl;
		break;
	case 4:
		return _powerUpMarioStar;
		break;
	default:
		break;
	}
	return false;
}

//Set les power ups, 1= laser, 2=TimeTravel, 3=MindControl, 4=ÉtoileMario
void PacMan::setPowerUps(int numDuPowerUp, bool valeur)
{
	assert(numDuPowerUp > 0 && numDuPowerUp < 5);
	switch (numDuPowerUp)
	{
	case 1:
		_powerUpLaser = valeur;
		break;
	case 2:
		_powerUpTimeTravel = valeur;
		break;
	case 3:
		_powerUpMindControl = valeur;
		break;
	case 4:
		_powerUpMarioStar = valeur;
		break;
	default:
		break;
	}
}

sf::Time PacMan::getTempsLaser()
{
	return _tempsLaser.getElapsedTime();
}

//Retourne le temps passé sans faire le laser
sf::Time PacMan::getTempsSansLaser()
{
	return _tempsSansLaser.getElapsedTime();
}

bool PacMan::getLaser()
{
	return _laser;
}

void PacMan::startClockEtoile()
{
	_clockEtoile.restart();
}

//Permet d'ajouter ou de supprimer du temps apparti a un power up
void PacMan::changerTempsPowerUp(int numDuPowerUp, float valeur)
{
	assert(numDuPowerUp > 0 && numDuPowerUp < 5);
	switch (numDuPowerUp)
	{
	case 1:
		_nbMilisecondeLaser += valeur;
		break;
	case 2:
		//_powerUpTimeTravel = valeur;
		break;
	case 3:
		//_powerUpMindControl = valeur;
		break;
	case 4:
		_clockEtoile.restart();
		_nbMilisecondeEtoile = valeur;
		break;
	default:
		break;
	}
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
		vertices[i + 1] = vertices[_nbrCote + 1 - i] = sf::Vertex(_pos, _color);

	return vertices;
}

void PacMan::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	_step += _stepIncrement;
	if (_step < 0 || _step >= _nbrCote / 4)
		_stepIncrement *= -1;

	auto vertices = buildPacMan();

	if (_keepFiring)
	{
		_keepFiring = false;
		fire();
	}
	else
	{
		stop();
	}

	if (_laser)
	{
		Laser laser(_pos, _direction);
		laser.draw(target, states);
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
		vertices[i + _step + 1] = sf::Vertex(_pos, _color);

	target.draw(vertices);
}

bool PacMan::hasDisappeared() const
{
	return (_step + _deathCount) >= _nbrCote;
}

void PacMan::input(char c)
{
	if (c == 'l' && _powerUpLaser)
	{
		_keepFiring = true;
	}
		

	setDirection(c);
}

void PacMan::setCouleurRandom()
{
	if (_compteurFrame >=10)
	{
		int rand1 = rand() % 156;
		int rand2 = rand() % 156;
		int rand3 = rand() % 156;
		_color = sf::Color(rand1 + 100, rand2 + 100, rand3 + 100);
		_compteurFrame = 0;
	}
	else
	{
		_compteurFrame++;
	}
	
}

void PacMan::setNormalStat()
{
	_vitesse = 3;
	_powerUpMarioStar = false;
	_nbMilisecondeEtoile = 0;
	_color = sf::Color::Yellow;
}

void PacMan::move(char direction, Map &map)
{
	if (_powerUpMarioStar && _clockEtoile.getElapsedTime().asMilliseconds() < _nbMilisecondeEtoile)
	{
		_stopRepeating = false;
		_vitesse = 5;
		setCouleurRandom();
	}
	else
	{
		if (!_stopRepeating)
			setNormalStat();
	}
	Personnage::move(direction, map);
}

void PacMan::respawn(sf::Vector2f pos)
{
	_pos = pos;
	_step = 0;
	_deathCount = 0;
}