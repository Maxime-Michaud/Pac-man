#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::clear()
{
	//D�salloue la m�moire des polices
	for (auto f : _fonts)
		delete f.second;
	_fonts.clear();

	//D�salloue la m�moire des textes
	for (auto t : _texts)
		delete t.second;
	_texts.clear();
}

void UI::addText(std::string& desc, std::string& text, std::string& font, sf::Vector2f& pos, int size, sf::Color color)
{
	//Empeche l'insertion d'un autre texte qui � le m�me nom
	if (_texts.find(desc) == _texts.end()) throw std::exception("Un texte � d�j� ce nom!");

	auto text = new sf::Text;
	
}