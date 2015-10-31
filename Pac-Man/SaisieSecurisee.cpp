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
#include "saisieSecurisee.h"

				  //vide le buffer et remet le flux valide
void viderBuffer(std::istream & entree)
{
	entree.clear();
	entree.seekg(0, std::ios::end);

	if (!entree.fail())	//Le flux est valide, donc le buffer est non vide
		entree.ignore(std::numeric_limits<std::streamsize>::max());
	else		//Le flux est invalide, donc le buffer est vide
		entree.clear();
	// Le flux est dans un état invalide donc on le remet en état valide
}

//Affiche un message et permet à l'utilisateur d'entrer un int
int saisirInt(std::string message)
{
	int valeur;		//Valeur entrée par l'utilisateur
	std::cout << message;
	std::cin >> valeur;

	while (std::cin.fail())
	{
		std::cout << "La valeur entree est invalide. Veuillez entrer un int valide\n";
		viderBuffer(std::cin);
		std::cout << message;
		std::cin >> valeur;
	}

	return valeur;
}
//surcharge pour éviter le message
int saisirInt()
{
	int valeur;		//Valeur entrée par l'utilisateur
	std::cin >> valeur;

	while (std::cin.fail())
	{
		std::cout << "Vous devez entrer un int valide";
		viderBuffer(std::cin);
		std::cin >> valeur;
	}

	return valeur;
}

//valide qu'un int saisi par saisirInt est positif
int saisirIntPositif(std::string message)
{
	int valeur = saisirInt(message);

	while (valeur <= 0)
	{
		std::cout << "L'entier saisi doit être positif";
		valeur = saisirInt(message);
	}

	return valeur;
}

//Affiche un message variable et permet a l'utilisateur d'entré un float qui est retourné
float saisirFloat(std::string message)
{
	float valeur;		//Valeur entrée par l'utilisateur
	std::cout << message;
	std::cin >> valeur;

	while (std::cin.fail())
	{
		std::cout << "La valeur entree est invalide. Veuillez entrer un float valide\n";
		viderBuffer(std::cin);
		std::cout << message;
		std::cin >> valeur;
	}

	return valeur;
}

//Valide que les floats de saisirFloat sont positifs et non nuls
float saisirFloatPositif(std::string message)
{
	float valeur;		//valeur entrée par l'utilisateur
	valeur = saisirFloat(message);

	while (valeur <= 0)
	{
		std::cout << "Vous devez entrer une valeur positive et non-nulle\n";
		valeur = saisirFloat(message);
	}

	return valeur;
}

//Permet de valider une entrée selon des critères choisis lors de la programmation
char saisirCharValide(std::string message, char critere1, char critere2)
{
	char charSaisi;	//Charactere saisi par l'utilisateur

	critere1 = tolower(critere1);
	critere2 = tolower(critere2);

	std::cout << '\n' << message << '(' << critere1 << "/" << critere2 << "): ";
	charSaisi = tolower(std::cin.get());

	//Tant que le charactere est invalide
	while (charSaisi != critere1 && charSaisi != critere2) {
		//Message d'erreur
		std::cout << "Sexe invalide, vous devez entrer f ou g\n";
		std::cout << '\n' << message << critere1 << "/" << critere2 << "): ";
		viderBuffer(std::cin);

		charSaisi = std::cin.get();
		//met le charactere en majuscule pour avoir moins de vérifications
		charSaisi = tolower(charSaisi);
	}

	return charSaisi;
}

//Permet de saisir une string qui n'est pas vide
std::string saisirStringNonVide(std::string message)
{
	std::string stringSaisi;

	std::cout << message;

	viderBuffer(std::cin); //Vide le buffer pour s'assurer
	getline(std::cin, stringSaisi);

	while (stringSaisi == "")
	{
		std::cout << "La string saisie ne peux être vide.\n"
			<< message;
		getline(std::cin, stringSaisi);
	}

	return stringSaisi;
}

//Demande a l'utilisateur s'il veut effectuer un autre calcul
bool questionOuiNon(std::string message)
{
	char reponse;

	std::cout << message << " (o/n)";
	std::cin >> reponse;
	reponse = toupper(reponse);

	while (!(reponse == 'O' || reponse == 'N')) //Redemande si la réponse est invalide
	{
		viderBuffer(std::cin);
		system("cls");

		std::cout << "Veuillez répondre par o (oui) ou n (non)\n"
			<< message << " (o/n)";
	}

	viderBuffer(std::cin);

	return (reponse == 'O'); //Retourne la valeur booléene associée à la réponse
}

//vide le buffer et fait un ignore d’un buffer vide, donc fait une pause
void pause()
{
	viderBuffer(std::cin);
	std::cin.ignore();
}

//Fais une pause jusqu'a ce que l'utilisateur appuie sur enter
void enter()
{
	std::string attente;			//String qui reçois le getline
	getline(std::cin, attente);
}

//Ouvre un fichier dans un flux reçu en entrée. Retourne une valeur booléene qui
//correspond à la présence ou l'absence du fichier
bool ouvrirFichier(std::ifstream & stream, std::string filepath, std::string messageAbsent)
{
	bool present = true; //Faux si le fichier est absent

	stream.open(filepath);

	if (!stream.is_open())
	{
		std::cout << messageAbsent;
		present = false;
	}

	return present;
}
//Surcharge sans message d'absence
bool ouvrirFichier(std::ifstream & stream, std::string filepath)
{
	bool present = true; //Faux si le fichier est absent

	stream.open(filepath);

	if (!stream.is_open())
		present = false;

	return present;
}
//Vérifie si le flux reçu en entrée est vide ou non et retourne une valeur booléene selon le cas.
bool siFichierVide(std::ifstream & stream, std::string messageVide)
{
	bool vide = false; //vrai si le fichier est vide

	if (stream.peek() == EOF)
	{
		std::cout << messageVide;
		vide = true;
	}

	return vide;
}