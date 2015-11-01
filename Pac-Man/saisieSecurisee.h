/* En-t�te du programme
=======================
Programme:        saisieSecurisee.h
Auteur:           Maxime Michaud-Corriveau
Date cr�ation:    16/02/2015
Description:      L'objectif de ce fichier d'en-t�te
				  est de regrouper les fonctions
				  de saisie, de validation ainsie que
				  les fonctions d'ouverture fichier et de
				  test de fichiers vides.*/
				  /*Directives au pr�-processeur
				  ==============================*/
#include <string>
#include <iostream>
#include <fstream>
#include <type_traits>
#include <vector>
#include <cassert>

#pragma once

				  //vide le buffer et remet le flux valide
void viderBuffer(std::istream & entree);

//Affiche un message et permet � l'utilisateur d'entrer un int
int saisirInt(std::string message);
//surcharge pour �viter le message
int saisirInt();

//valide qu'un int saisi par saisirInt est positif
int saisirIntPositif(std::string message);

//Affiche un message variable et permet a l'utilisateur d'entr� un float qui est retourn�
float saisirFloat(std::string message);

//Valide que les floats de saisirFloat sont positifs et non nuls
float saisirFloatPositif(std::string message);

//Permet de valider une entr�e selon des crit�res choisis lors de la programmation
char saisirCharValide(std::string message, char critere1, char critere2);

//Permet de saisir une string qui n'est pas vide
std::string saisirStringNonVide(std::string message);

//Demande a l'utilisateur s'il veut effectuer un autre calcul
bool questionOuiNon(std::string message);

//vide le buffer et fait un ignore d�un buffer vide, donc fait une pause
void pause();

//Fais une pause jusqu'a ce que l'utilisateur appuie sur enter
void enter();

//Ouvre un fichier dans un flux re�u en entr�e. Retourne une valeur bool�ene qui
//correspond � la pr�sence ou l'absence du fichier
bool ouvrirFichier(std::ifstream & stream, std::string filepath, std::string messageAbsent);
//Surcharge sans message d'absence
bool ouvrirFichier(std::ifstream & stream, std::string filepath);

//V�rifie si le flux re�u en entr�e est vide ou non et retourne une valeur bool�ene selon le cas.
bool siFichierVide(std::ifstream & stream, std::string messageVide);

//Trouve la bonne fonction de conversion de string a num�rique. Utilise les plus gros types possibles
template <typename T>
T strtonum(const char * num)
{
#if defined(_MSC_VER) && _MSC_VER >= 1900
	static_assert(std::is_arithmetic<T>(),
		"Assurez vous que le type de atoNum<T> est num�rique (int�gral ou a virgule flottante)!");
#else
	assert(std::is_arithmetic<T>());
#endif
	//Pour les floats
	if (std::is_floating_point<T>())
		return static_cast<T>(strtold(num, nullptr));
	//Pour les ints unsigned
	if (std::is_unsigned<T>())
		return static_cast<T>(strtoull(num, nullptr, 10));
	//Il ne reste que les ints:
	return static_cast<T>(strtoll(num, nullptr, 10));
}

template <typename T, bool can_throw = true>
/*Lis des nombres dans un flux d'entr�e. Permet d'utiliser un ou plusieurs s�parateurs diff�rents.
	Les s�parateurs doivent �tre un tableaux de char termin� par \0.
	Permet aussi de sp�cifier le s�parateur d�cimal pour les nombres non int�graux.*/
std::vector<T> readNumFromStream(std::istream & in, int count = 1, const char * separateur = " ,", char separateurDecimal = '.')
{
	//Gestion d'erreurs et de cas limites
#if defined(_MSC_VER) && _MSC_VER >= 1900	//Les versions pr�c�dentes de VS ne supportent pas constexpr
	static_assert(std::is_arithmetic<T>(),
		"Assurez vous que le type de readNumFromStream<T> est num�rique (int�gral ou a virgule flottante)!");
#else
	assert(std::is_arithmetic<T>());
#endif

	//Pour les nombres a virgules flottante, le s�parateur d�cimal ne doit pas faire partie des s�parateurs.
	std::string tmp;	//Doit etre d�clar� en dehors du if
	if (std::is_floating_point<T>())
	{
		for (int i = 0; i < strlen(separateur); i++)
			assert(separateur[i] != separateurDecimal);
	}
	//Pour les entiers, ajoute le separateur d�cimal aux autres s�parateurs
	else
	{
		tmp = separateur;
		tmp += separateurDecimal;
		separateur = tmp.c_str();
	}

	std::vector<T> ret;		//Vecteur retourn�
	//Ajoute les nombres au vecteur
	for (int c = 0; c < count || count == 0; c++)
	{
		std::string num;

		//Atteint le nombre
		for (int i = 0; i < strlen(separateur); i++)
		{
			if (in.peek() == separateur[i])
			{
				in.get();
				//Reset la loop
				i = -1;
			}
		}

		//Si le nombre commence par -, s'assure qu'il n'est pas unsigned
		if (in.peek() == '-')
		{
			//Nombre n�gatif unsigned = pas bien.
			if (can_throw && std::is_unsigned<T>())
				throw std::invalid_argument("Un nombre unsigned ne peux commencer par '-'!");

			num += in.get();
		}

		//Copie la partie int�grale dans le nombre
		while (in.peek() >= '0' && in.peek() <= '9')
			num += in.get();

		//Pour les nombres a virgule flottante
		if (std::is_floating_point<T>() && in.peek() == separateurDecimal)
		{
			//Obtiens la partie du nombre apres la virgule, si n�c�ssaire
			num += '.';
			in.get();

			while (in.peek() >= '0' && in.peek() <= '9')
				num += in.get();
		}

		//Si aucun nombre n'a �t� lu, quitte la boucle
		if (num == "")
			break;

		ret.push_back(strtonum<T>(num.c_str()));
	}

	if (can_throw && ret.size() != count)
		throw std::invalid_argument("Le fichier ne contient pas le bon nombre de nombre ou est mal format�!");

	return ret;
}