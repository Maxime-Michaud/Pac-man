#include "Sons.h"

#ifdef _DEBUG
#include <iostream>
#endif

//Constructeur par défaut
Sons::Sons()
{
}

//Désalloue la mémoire des sons
Sons::~Sons()
{
	for (auto sb : _buffers)
		delete sb;
}

bool Sons::play(const std::string & nom)
{
	bool isPlayed = false;

	//Vérifie si le son est trouvable
	auto s = _sons.find(nom);
	if (s != _sons.end())
	{
		//Joue le son s'il ne joue pas déjà
		if (s->second.getStatus() != sf::Sound::SoundSource::Status::Playing)
		{
			s->second.play();
			isPlayed = true;
		}
	}
#ifdef _DEBUG
	else
	{
		std::cout << "Le son " + nom + "n'existe pas.";
	}
#endif

	return isPlayed;
}

bool Sons::stop(const std::string & nom)
{
	bool isStopped = false;

	//Vérifie si le son est trouvable
	auto s = _sons.find(nom);
	if (s != _sons.end())
	{
		//Joue le son s'il ne joue pas déjà
		if (s->second.getStatus() == sf::Sound::SoundSource::Status::Playing)
		{
			s->second.stop();
			isStopped = true;
		}
	}

	return isStopped;
}

void Sons::stopAll()
{
	for (auto s : _sons)
		s.second.stop();
}

bool Sons::add(const std::string & nom, const std::string & source)
{
	auto buffer = new sf::SoundBuffer;
	buffer->loadFromFile(source);

	sf::Sound son(*buffer);

	_sons.emplace(std::piecewise_construct, 
					std::forward_as_tuple(nom),
					std::forward_as_tuple(*buffer));

	return true;
}

void Sons::replay(const std::string & nom)
{
	auto s = _sons.find(nom);
	if (s != _sons.end())
		s->second.play();
#ifdef _DEBUG
	else
		std::cout << "Le son " + nom + "n'existe pas.";
#endif
}

bool Sons::isPlaying(const std::string & nom) const
{
	bool isPlaying = false;
	auto s = _sons.find(nom);

	if (s != _sons.end())
		isPlaying = s->second.getStatus() == sf::Sound::Playing;

	return isPlaying;
}