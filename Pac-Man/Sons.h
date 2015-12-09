#pragma once
/*
Auteur:			Maxime Michaud-Corriveau
Programme:		Sons.h
Description:	Cette classe regroupe tous les sons d'un projet sous SFML et 
				les jouent selon des conditions déterminées lors de 
				l'ajout du son
*/
#include <SFML\Audio.hpp>
#include <map>
#include <vector>

class Sons
{
	std::vector<sf::SoundBuffer*> _buffers;	//Store les buffers des sons pour les garder
											//jusqu'a la fin de l'exécution
	std::map<std::string, sf::Sound> _sons;
public:
	Sons();			//Ne fais rien
	~Sons();		//Désalloue la mémoire des soundbuffers

	//Joue le son
	bool play(const std::string & nom);
	//Stop le son
	bool stop(const std::string & nom);
	//Stop tous les sons
	void stopAll();
	//Ajoute un son
	bool add(const std::string & nom, const std::string & source);
	//Joue un son. Si il joue déjà, le recommence
	void replay(const std::string & nom);

	bool isPlaying(const std::string & nom) const;
};

