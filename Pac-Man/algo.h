/********************************************************************************
*Date:			27 octobre 2015													*
*Programme:		algo.h															*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Librairie contenant des fonctions d'usage général ne faisant	*
*				pas partie de la librairie standard.	    					*
*********************************************************************************/
#pragma once

template <typename T>
inline bool isBetween(T val, T lowerBound, T upperBound)
{
	return (val >= lowerBound && val <= upperBound);
}
