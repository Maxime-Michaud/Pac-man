/********************************************************************************
*Date:			26 octobre 2015													*
*Programme:		windowsAPIwrapper.h												*
*Programmeur:	Maxime Michaud-Corriveau										*
*Description:	Ce fichier d'en-tête sert de wrapper pour appeller les 			*
*				fonctions de Windows.h sans avoir a l'inclure directement dans	*
*				le code source.													*
*********************************************************************************/
#include "windowsAPIwrapper.h"
#include <Windows.h>

int screenWidth()
{
	RECT desktop;
	const HWND handle = GetDesktopWindow();

	GetWindowRect(handle, &desktop);

	return desktop.right;
}

int screenHeight()
{
	RECT desktop;
	const HWND handle = GetDesktopWindow();

	GetWindowRect(handle, &desktop);

	return desktop.bottom;
}
extern const int winapi::ScreenHeight = screenHeight();
extern const int winapi::ScreenWidth = screenWidth();