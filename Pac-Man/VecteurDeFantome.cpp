#include "VecteurDeFantome.h"


VecteurDeFantome::VecteurDeFantome()
{
}

void VecteurDeFantome::add(Fantome* f)
{
	_fantomes.push_back(f);
}

VecteurDeFantome::~VecteurDeFantome()
{

}

void VecteurDeFantome::draw(sf::RenderTarget& r, sf::RenderStates s)const
{
	sf::VertexArray vertex(sf::Triangles);
	for (auto f : _fantomes)
	{
		auto v = f->getVertexArray();
		for (int i = 0; i < v.getVertexCount();i++)
		{
			vertex.append(v[i]);
		}
	}

	r.draw(vertex, s);
}

Fantome* VecteurDeFantome::operator[](int pos)
{
	return _fantomes[pos];
	_fantomes.begin();
}

std::vector<Fantome*, std::allocator<Fantome*>>::iterator VecteurDeFantome::begin()
{
	return _fantomes.begin();
}

std::vector<Fantome*, std::allocator<Fantome*>>::iterator VecteurDeFantome::end()
{
	return _fantomes.end();
}