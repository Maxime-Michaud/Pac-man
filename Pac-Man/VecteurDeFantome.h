#include<vector>
#include "fantome.h"
#pragma once
class VecteurDeFantome : public sf::Drawable
{
	std::vector<Fantome*> _fantomes;
public:
	VecteurDeFantome();
	~VecteurDeFantome();
	void add(Fantome*);
	void draw(sf::RenderTarget&, sf::RenderStates)const;
	Fantome* operator[](int pos);
	std::vector<Fantome*, std::allocator<Fantome*>>::iterator begin();
	std::vector<Fantome*, std::allocator<Fantome*>>::iterator end();
};

