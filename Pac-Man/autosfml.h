/*                             En-tête de fichier                             *
*******************************************************************************
*Programme:			AutoSFML.h												  *
*Date:				5 juin 2015												  *
*Développeur:		Maxime Michaud-Corriveau								  *
*Description:		Inclut automatiquement tous les .libs essentiells à SFML  *
*******************************************************************************/

#pragma once

#ifdef _DEBUG //Debug libs
#ifdef SFML_STATIC		 
#pragma comment(lib, "sfml-audio-s-d.lib")
#pragma comment(lib, "sfml-graphics-s-d.lib")
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-network-s-d.lib")
#pragma comment(lib, "sfml-system-s-d.lib")
#pragma comment(lib, "sfml-window-s-d.lib")
#pragma comment(lib, "sfeMovie-d.lib")
#pragma comment(lib, "flac.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ogg.lib")
#pragma comment(lib, "openal32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "vorbis.lib")
#pragma comment(lib, "vorbisenc.lib")
#pragma comment(lib, "vorbisfile.lib")

#else
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfeMovie-d.lib")
#endif


#elif defined(SFML_STATIC) //Release static
#pragma comment(lib, "sfml-audio-s.lib")
#pragma comment(lib, "sfml-graphics-s.lib")
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-network-s.lib")
#pragma comment(lib, "sfml-system-s.lib")
#pragma comment(lib, "sfml-window-s.lib")
#pragma comment(lib, "flac.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ogg.lib")
#pragma comment(lib, "openal32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "vorbis.lib")
#pragma comment(lib, "vorbisenc.lib")
#pragma comment(lib, "vorbisfile.lib")




#else				//Release libs
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfeMovie.lib")
#endif