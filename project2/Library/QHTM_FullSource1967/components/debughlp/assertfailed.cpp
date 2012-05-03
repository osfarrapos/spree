/*----------------------------------------------------------------------
Copyright (c) 1998-present Russ Freeman. All Rights Reserved.
Web site: http://www.gipsysoft.com/

This software is provided 'as-is', without any express or implied warranty.

In no event will the author be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject
to the following restrictions: 

1) This source code may not be distributed as part of a commercial library
   without written permission from the author
2) The origin of this software must not be misrepresented; you must not claim
   that you wrote the original software. If you use this software in a product,
	 an acknowledgment in the product documentation is requested but not required.
3) Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
4) Altered source is encouraged to be submitted back to the original author so
   it can be shared with the community. Please share your changes. Don't make me
	 use GPL!
5) This notice may not be removed or altered from any source distribution.
6) If you have any confusion then ask

File:	AssertFailed.cpp
Owner:	russf@gipsysoft.com
Purpose:	assert failed function
----------------------------------------------------------------------*/
#include "stdafx.h"
//#include <WinHelper.h>
#include "DebugHlp.h"
#include <crtdbg.h> 
#include <signal.h> 

#ifndef countof
	#define countof( t )	(sizeof( (t) ) / sizeof( (t)[0] ) )
#endif	//	countof

BOOL _cdecl AssertFailed( LPCSTR pcszFilename, int nLine, LPCSTR pcszExpression )
{

	char szExeName[ MAX_PATH ];
  if( !GetModuleFileNameA( NULL, szExeName, countof( szExeName ) ) )
		strcpy( szExeName, "<No Program Name>" );

	char szMessage[ 1024 ];
  _snprintf( szMessage, countof( szMessage )
						, "Assertion Failure!"
							"\nProgram: %s"
							"\n"
							"\nFile %s"
							"\nLine %d"
							"\n"
							"\nExpression %s"
							"\n\nPress Retry to debug the application"
						, szExeName
						, pcszFilename
						, nLine
						, pcszExpression
						);

	HWND hwndParent = GetActiveWindow();
	hwndParent = GetLastActivePopup( hwndParent );
  int nCode = MessageBoxA( hwndParent, szMessage, "Debug Helper", MB_TASKMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND );

  if( nCode == IDABORT )
  {
		exit( nCode );
  }

  if( nCode == IDRETRY )
		return 1;

	return FALSE;
}