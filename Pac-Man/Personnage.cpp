/********************************************************************************
*Date:			16 Octobre 2015													*
*Programme:		Personnage.cpp													*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe de base pour des personnages.							*
*				Permet de:														*
*					-Dessiner des sprites qui loop								*
*					-Garder en mémoire la position								*
*					-Déplacer le personnage										*
*********************************************************************************/

#include "Personnage.h"


//Set tout a 0
Personnage::Personnage()
{
	_noSprite = _spriteCnt = 0;

	_pos = sf::Vector2f(0, 0);
	
	_sprite = nullptr;
}


Personnage::~Personnage()
{
	clear();
}

//désalloue la mémoire
void Personnage::clear()
{
	if (_sprite)
	{
		delete[] _sprite;
		_sprite = nullptr;
	}

	_texture = sf::Texture();	//efface la texture
}

//Charge les sprites a partir d'un fichier
void Personnage::loadSprite(std::string source, std::vector<sf::IntRect> rects)
{
	if (rects.size() < 1)
		throw std::invalid_argument("Le personnage a besoin d'avoir au moin un rectangle pour son sprite");

	_spriteCnt = rects.size();

	if (!_texture.loadFromFile(source))
		throw std::exception(std::string("Image manquante: " + source + 
										 "est introuvable sur votre ordinateur").c_str());

	//Essaie de charger la texture et de créer les sprites. En cas de problème, désalloue toute la mémoire
	try
	{
		_sprite = new sf::Sprite[_spriteCnt];

		for (int i = 0; i < _spriteCnt; i++)
			_sprite[i] = sf::Sprite(_texture, rects[i]);
	}
	catch (...)
	{
		clear();
		throw;
	}
}

//Toutes les positions sont valides en théorie. Peut être surchargée par les enfants si on veut éviter certains cas particuliés
void Personnage::setPos(sf::Vector2f pos)
{
	_pos = pos;
}

//Retourne les sprite
const sf::Sprite * Personnage::getSprite() const
{
	return _sprite;
}

//Obtiens la position du personnage
sf::Vector2f Personnage::getPos() const
{
	return _pos;
}

//Déplace le personnage de x, y
void Personnage::move(float x, float y)
{
	_pos.x += x;
	_pos.y += y;
}

//Déplace le personnage de deplacement
void Personnage::move(sf::Vector2f deplacement)
{
	_pos += deplacement;
}

//Dessine le personnage
void Personnage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//set la position du sprite
	_sprite[_noSprite].setPosition(_pos);

	//dessine le sprite et incrémente la position dans l'array de sprite
	target.draw(_sprite[_noSprite++]);

	//Si on est au dernier sprite, loop au sprite 0
	_noSprite %= _spriteCnt;
}


