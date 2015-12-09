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
	for (auto f = _fonts.begin(); f != _fonts.end(); ++f)
		delete f->second;
	_fonts.clear();

	//Désalloue la mémoire des textes
	for (auto t = _texts.begin(); t != _texts.end(); ++t)
		delete t->second.first;
	_texts.clear();
}

void UI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto t = _texts.begin(); t != _texts.end(); ++t)
	{
		if (t->second.second != 0)
		{
			target.draw(*(t->second.first), states);
			t->second.second--;
		}
	}
}

void UI::addText(std::string& desc, std::string& text, std::string& font, sf::Vector2f& pos, int size, sf::Color color, int frames)
{
	//Empeche l'insertion d'un autre texte qui à le même nom
	if (_texts.find(desc) != _texts.end()) throw std::exception("Un texte à déjà ce nom!");

	//Vérifie si le font existe déjà, sinon l'ajoute à _fonts
	auto fontIt = _fonts.find(font);
	if (fontIt == _fonts.end())
	{
		auto newFont = new sf::Font;
		newFont->loadFromFile(font);

		_fonts.insert({ font, newFont });	//Utilise l'URI du font comme clé
		fontIt = _fonts.find(font);
	}

	auto newText = new sf::Text;

	newText->setString(text);
	newText->setFont(*(fontIt->second));
	newText->setPosition(pos);
	newText->setCharacterSize(size);
	newText->setColor(color);

	_texts.insert({ desc,std::make_pair( newText,frames) });
}

sf::Text & UI::Text(const std::string& desc)
{
	auto it = _texts.find(desc);

	if (it == _texts.end())
		throw (std::exception("Le texte est introuvable"));

	return *(it->second.first);
}

void UI::changeText(std::string & desc, std::string newText)
{
	auto it = _texts.find(desc);

	if (it == _texts.end())
		throw (std::exception("Le texte est introuvable"));

	it->second.first->setString(newText);
}
