/*----------------------------------------------------------------------
Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	platform.h
Owner:	russf@gipsysoft.com
Purpose:	Platform specific types and includes
----------------------------------------------------------------------*/
#ifndef PLATFORM_H
#define PLATFORM_H

#include "../debughlp/debughlp.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
	#pragma warning( disable : 4355 )
	#include <windows.h>
	#include <tchar.h>

	#ifndef UNDER_CE
		#include <time.h>
		#include <assert.h>
		#include <time.h>
	#endif

	#include "win32/types.h"
#endif	//	WIN32

#ifdef ELATE
	#include <elate/ave/ave.h>
	#include <time.h>
	#include "elate/types.h"
	#include <unistd.h>
	#include <time.h>
#endif	//	ELATE


template <class T>
inline T SkipWhiteSpace( T pcsz )
//
//	Skip past any whitespace
{
	while( *pcsz && isspace( *pcsz ) ) pcsz++;
	return pcsz;
}


#endif //PLATFORM_H