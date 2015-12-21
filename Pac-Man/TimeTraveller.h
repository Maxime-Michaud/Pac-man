/********************************************************************************
*Date:			18 octobre 2015													*
*Programme:		TimeTraveller.h													*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Classe virtuelle pure qui permet � quelquechose de retourner	*
*				� un �tat pr�c�dent, en enregistrant les diff�rences entre les  *
*				�tats.															*
*																				*
*Utilisation:	La classe est inutile si elle n'est pas h�rit�e par une classe	*
*				enfant quelconque, puisque la stack est protected et qu'aucune	*
*				m�thode n'existe pour ajouter des donn�es a la stack. La seule	*
*				fonction fait une combinaison de top et de pop: On lui passe	*
*				en parametre un element de type T, et on va ensuite popper tout	*
*				ce qui est pareil dans le top de la stack, avant de retourner	*
*				le nouveau top. Cela va ensuite permettre aux classes h�ritant	*
*				de TimeTraveller d'utiliser la stack pour enregistrer les		*
*				changements (ex. les d�placements) pour progressivement se		*
*				rapprocher du premier �tat enregistr�.							*
*																				*
*				La fonction goBack doit �tre surcharg�e par les enfants afin	*
*				qu'elle accomplisse la tache voulue.							*
*********************************************************************************/

#include <stack>

#pragma once
template <typename T, typename U>
class TimeTraveller
{
protected:
	std::stack<T> _change;	//Garde tous les changements a l'�tat de l'objet en m�moire

public:

	/// <summary>
	/// Obtiens l'�tat pr�c�dent. Si l'�tat pr�c�dent est le m�me que l'actuel, efface le pr�c�dent et retourne celui d'avant
	/// </summary>
	/// <param name="current">
	/// �tat actuel. N'est pas modifi� par la fonction, 
	///	mais puisque current peut etre retourn�, il n'est pas const.
	/// </param>
	T& getPrevState(T& current);

	/// <summary>
	/// Retourne dans le pass�
	/// </summary>
	virtual void goBack(U params);
};

template<typename T, typename U>
T& TimeTraveller<T, U>::getPrevState(T& current)
{
	//Si la pile de changement est vide, retourne l'�tat actuel
	if (!_change.size())
		return current;

	//Pop tous les �tats pareils a l'�tat actuel
	while (_change.top() == current)
	{
		_change.pop();

		if (!_change.size())
			return current;
	}

	return _change.top();
}

template<typename T, typename U>
void TimeTraveller<T, U>::goBack(U params)
{
	throw std::bad_exception("Fonction non impl�ment�e");
}
