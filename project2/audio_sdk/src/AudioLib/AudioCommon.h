/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __AUDIO_COMMON_H
#define __AUDIO_COMMON_H

#include <dmusicc.h>
#include <dmusici.h>
#include <dsound.h>

// Required Windows libs
//#pragma comment(lib, "msacm32")
//#pragma comment(lib, "winmm")

// Link DirectX 8 libs
//#pragma comment(lib, "dxguid")
//#pragma comment(lib, "dsound")

// Conditionally include, define, and link error functions
#if(DIRECTSOUND_VERSION==0x0900)
#include <dxerr9.h>
#define DXGetErrorString DXGetErrorString9
//#pragma comment(lib, "dxerr9")
#else
#include <dxerr8.h>
#define DXGetErrorString DXGetErrorString8
//#pragma comment(lib, "dxerr8")
#endif

// Ogg Vorbis libs
#ifdef USE_VORBIS
#ifdef _DEBUG
#pragma comment(lib, "ogg_d")
#pragma comment(lib, "vorbis_d")
#pragma comment(lib, "vorbisfile_d")
#else
#pragma comment(lib, "ogg")
#pragma comment(lib, "vorbis")
#pragma comment(lib, "vorbisfile")
#endif
#endif // USE_VORBIS

/*
// Ogg Vorbis libs
#ifdef USE_VORBIS
#ifdef _DEBUG
#pragma comment(lib, "ogg_static_d")
#pragma comment(lib, "vorbis_static_d")
#pragma comment(lib, "vorbisfile_static_d")
#else
#pragma comment(lib, "ogg_static")
#pragma comment(lib, "vorbis_static")
#pragma comment(lib, "vorbisfile_static")
#endif
#endif // USE_VORBIS
*/


#ifdef USE_WMA
#pragma comment(lib, "wmstub.lib")
#pragma comment(lib, "wmvcore.lib")
#endif // USE_WMA


#endif // __AUDIO_COMMON_H
