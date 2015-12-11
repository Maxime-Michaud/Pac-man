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
	for (auto f = _fonts.begin(); f != _fonts.end(); ++f)
		delete f->second;
	_fonts.clear();

	//D�salloue la m�moire des textes
	for (auto t = _texts.begin(); t != _texts.end(); ++t)
		delete t->second.first;
	_texts.clear();

	//D�salloue la m�moire des fonctions
	for (auto f = _functions.begin(); f != _functions.end(); ++f)
		delete f->second.first;
	_functions.clear();
}

void UI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto f = _functions.begin(); f != _functions.end(); ++f)
	{
		//Si la fonction est activ�e
		if (f->second.second)
		{
			//Apelle la fonction et store si elle doit etre r�activ�e dans sa valeur bool�enne associ�e
			auto func = f->second.first;
			f->second.second = (*func)();
		}
	}

	for (auto t = _texts.begin(); t != _texts.end(); ++t)
	{
		if (t->second.second != 0)
		{
			target.draw(*(t->second.first), states);
			t->second.second--;
		}
	}
}

void UI::addText(const std::string& desc, const std::string& text, const std::string& font, const sf::Vector2f& pos, int size,
			 const sf::Color & color, int frames)
{
	//Empeche l'insertion d'un autre texte qui � le m�me nom
	if (_texts.find(desc) != _texts.end()) throw std::exception("Un texte � d�j� ce nom!");

	//V�rifie si le font existe d�j�, sinon l'ajoute � _fonts
	auto fontIt = _fonts.find(font);
	if (fontIt == _fonts.end())
	{
		auto newFont = new sf::Font;
		newFont->loadFromFile(font);

		_fonts.insert({ font, newFont });	//Utilise l'URI du font comme cl�
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

void UI::changeText(const std::string & desc, const std::string & newText)
{
	auto it = _texts.find(desc);

	if (it == _texts.end())
		throw (std::exception("Le texte est introuvable"));

	it->second.first->setString(newText);
}

void UI::setFrames(const std::string & desc, int frame)
{
	auto it = _texts.find(desc);

	if (it == _texts.end())
		throw std::exception("Le texte est introuvable");

	it->second.second = frame;
}

bool UI::hasText(const std::string & desc) const
{
	return _texts.end() != _texts.find(desc);
}

void UI::addAnimation(const std::string & desc, std::function<bool()> * func)
{
	if (_functions.find(desc) != _functions.end()) throw std::exception("La fonction existe d�ja.");

	_functions.insert(std::make_pair(desc, std::make_pair(func, false)));
}

void UI::playAnimation(const std::string& desc)
{
	auto it = _functions.find(desc);
	if (it == _functions.end()) throw std::exception("La fonction n'existe pas.");

	//Set le bool associ� a la fonction de l'animation a true
	it->second.second = true;
}
