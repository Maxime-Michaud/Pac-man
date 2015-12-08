#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::clear()
{
	//Désalloue la mémoire des polices
	for (auto f : _fonts)
		delete f.second;
	_fonts.clear();

	//Désalloue la mémoire des textes
	for (auto t : _texts)
		delete t.second;
	_texts.clear();
}

void UI::addText(std::string& desc, std::string& text, std::string& font, sf::Vector2f& pos, int size, sf::Color color)
{
	//Empeche l'insertion d'un autre texte qui à le même nom
	if (_texts.find(desc) == _texts.end()) throw std::exception("Un texte à déjà ce nom!");

	auto text = new sf::Text;
	
}