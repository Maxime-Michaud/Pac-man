/********************************************************************************
*Date:			18 octobre 2015													*
*Programme:		TimeTraveller.h													*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe virtuelle pure qui permet à quelquechose de retourner	*
*				à un état précédent, en enregistrant les différences entre les  *
*				états.															*
*																				*
*Utilisation:	La classe est inutile si elle n'est pas héritée par une classe	*
*				enfant quelconque, puisque la stack est protected et qu'aucune	*
*				méthode n'existe pour ajouter des données a la stack. La seule	*
*				fonction fait une combinaison de top et de pop: On lui passe	*
*				en parametre un element de type T, et on va ensuite popper tout	*
*				ce qui est pareil dans le top de la stack, avant de retourner	*
*				le nouveau top. Cela va ensuite permettre aux classes héritant	*
*				de TimeTraveller d'utiliser la stack pour enregistrer les		*
*				changements (ex. les déplacements) pour progressivement se		*
*				rapprocher du premier état enregistré.							*
*																				*
*				La fonction goBack doit être surchargée par les enfants afin	*
*				qu'elle accomplisse la tache voulue.							*
*********************************************************************************/

#include <stack>

#pragma once
template <typename T>
class TimeTraveller
{
protected:
	std::stack<T> _change;	//Garde tous les changements a l'état de l'objet en mémoire

public:
	
	/// <summary>
	/// Obtiens l'état précédent. Si l'état précédent est le même que l'actuel, efface le précédent et retourne celui d'avant
	/// </summary>
	/// <param name="current">État actuel</param>
	T& getPrevState(const T& current);

	/// <summary>
	/// Retourne dans le passé
	/// </summary>
	virtual void goBack() = 0;

};

template<typename T>
inline T& TimeTraveller<T>::getPrevState(const T& current)
{
	//Si la pile de changement est vide, retourne l'état actuel
	if (!_change.size())
		return current;

	//Pop tous lés états pareils a l'état actuel
	while (_change.top() == current)
	{
		_change.pop();

		if (!_change.size())
			return current;
	}

	return _change.top();
}
