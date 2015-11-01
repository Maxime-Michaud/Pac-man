/* En-tête du programme
=======================
Programme:        saisieSecurisee.h
Auteur:           Maxime Michaud-Corriveau
Date création:    16/02/2015
Description:      L'objectif de ce fichier d'en-tête
				  est de regrouper les fonctions
				  de saisie, de validation ainsie que
				  les fonctions d'ouverture fichier et de
				  test de fichiers vides.*/
				  /*Directives au pré-processeur
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

//Affiche un message et permet à l'utilisateur d'entrer un int
int saisirInt(std::string message);
//surcharge pour éviter le message
int saisirInt();

//valide qu'un int saisi par saisirInt est positif
int saisirIntPositif(std::string message);

//Affiche un message variable et permet a l'utilisateur d'entré un float qui est retourné
float saisirFloat(std::string message);

//Valide que les floats de saisirFloat sont positifs et non nuls
float saisirFloatPositif(std::string message);

//Permet de valider une entrée selon des critères choisis lors de la programmation
char saisirCharValide(std::string message, char critere1, char critere2);

//Permet de saisir une string qui n'est pas vide
std::string saisirStringNonVide(std::string message);

//Demande a l'utilisateur s'il veut effectuer un autre calcul
bool questionOuiNon(std::string message);

//vide le buffer et fait un ignore d’un buffer vide, donc fait une pause
void pause();

//Fais une pause jusqu'a ce que l'utilisateur appuie sur enter
void enter();

//Ouvre un fichier dans un flux reçu en entrée. Retourne une valeur booléene qui
//correspond à la présence ou l'absence du fichier
bool ouvrirFichier(std::ifstream & stream, std::string filepath, std::string messageAbsent);
//Surcharge sans message d'absence
bool ouvrirFichier(std::ifstream & stream, std::string filepath);

//Vérifie si le flux reçu en entrée est vide ou non et retourne une valeur booléene selon le cas.
bool siFichierVide(std::ifstream & stream, std::string messageVide);

//Trouve la bonne fonction de conversion de string a numérique. Utilise les plus gros types possibles
template <typename T>
T strtonum(const char * num)
{
#if defined(_MSC_VER) && _MSC_VER >= 1900
	static_assert(std::is_arithmetic<T>(),
		"Assurez vous que le type de atoNum<T> est numérique (intégral ou a virgule flottante)!");
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
/*Lis des nombres dans un flux d'entrée. Permet d'utiliser un ou plusieurs séparateurs différents.
	Les séparateurs doivent être un tableaux de char terminé par \0.
	Permet aussi de spécifier le séparateur décimal pour les nombres non intégraux.*/
std::vector<T> readNumFromStream(std::istream & in, int count = 1, const char * separateur = " ,", char separateurDecimal = '.')
{
	//Gestion d'erreurs et de cas limites
#if defined(_MSC_VER) && _MSC_VER >= 1900	//Les versions précédentes de VS ne supportent pas constexpr
	static_assert(std::is_arithmetic<T>(),
		"Assurez vous que le type de readNumFromStream<T> est numérique (intégral ou a virgule flottante)!");
#else
	assert(std::is_arithmetic<T>());
#endif

	//Pour les nombres a virgules flottante, le séparateur décimal ne doit pas faire partie des séparateurs.
	std::string tmp;	//Doit etre déclaré en dehors du if
	if (std::is_floating_point<T>())
	{
		for (int i = 0; i < strlen(separateur); i++)
			assert(separateur[i] != separateurDecimal);
	}
	//Pour les entiers, ajoute le separateur décimal aux autres séparateurs
	else
	{
		tmp = separateur;
		tmp += separateurDecimal;
		separateur = tmp.c_str();
	}

	std::vector<T> ret;		//Vecteur retourné
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
			//Nombre négatif unsigned = pas bien.
			if (can_throw && std::is_unsigned<T>())
				throw std::invalid_argument("Un nombre unsigned ne peux commencer par '-'!");

			num += in.get();
		}

		//Copie la partie intégrale dans le nombre
		while (in.peek() >= '0' && in.peek() <= '9')
			num += in.get();

		//Pour les nombres a virgule flottante
		if (std::is_floating_point<T>() && in.peek() == separateurDecimal)
		{
			//Obtiens la partie du nombre apres la virgule, si nécéssaire
			num += '.';
			in.get();

			while (in.peek() >= '0' && in.peek() <= '9')
				num += in.get();
		}

		//Si aucun nombre n'a été lu, quitte la boucle
		if (num == "")
			break;

		ret.push_back(strtonum<T>(num.c_str()));
	}

	if (can_throw && ret.size() != count)
		throw std::invalid_argument("Le fichier ne contient pas le bon nombre de nombre ou est mal formaté!");

	return ret;
}