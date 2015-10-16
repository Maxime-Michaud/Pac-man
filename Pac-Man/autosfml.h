/*                             En-tête de fichier                             *
*******************************************************************************
*Programme:			AutoSFML.h												  *
*Date:				5 juin 2015												  *
*Développeur:		Maxime Michaud-Corriveau								  *
*Description:		Inclut automatiquement tous les .libs essentiells à SFML  *
*******************************************************************************/

#pragma once

#ifdef _DEBUG		 //Debug libs
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")

#else				//Release libs
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")

#endif