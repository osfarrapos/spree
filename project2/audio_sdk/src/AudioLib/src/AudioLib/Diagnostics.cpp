/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Audio.h"

using namespace Audio;

#ifdef DIAGNOSTICS
int DIAG::m_iDebugLevel = 1;
#endif

#define _XCOMMENT / ## / 

#ifdef _XDWDEBUG
extern void _XDWINPRINT( TCHAR *msg, ... );
extern void _XDWINPRINTONLY( TCHAR *msg, ... );
#else
#define _XDWINPRINT		_XCOMMENT
#define _XDWINPRINTONLY _XCOMMENT
#endif

#ifdef DIAGNOSTICS

void DIAG::DiagDebugOut(int iLevel, const char* lpszFormat, ...) 
{ 
	va_list args; 
	va_start(args, lpszFormat); 
	char szBuffer[MAX_DEBUG_LINE_LEN]; 
	int nBuf = _vsnprintf(szBuffer, MAX_DEBUG_LINE_LEN, lpszFormat, args); 
	assert(nBuf >= 0); 
	if(m_iDebugLevel >= iLevel) 
	{ 
		szBuffer[nBuf] = '\n';
		szBuffer[nBuf + 1] = 0;
		OutputStringFn(szBuffer); 
		_XDWINPRINT( szBuffer );
	} 
	va_end(args); 
}
void DIAG::DiagFN(const char* lpszFormat, ...) 
{ 
	va_list args; 
	va_start(args, lpszFormat); 
	char szBuffer[MAX_DEBUG_LINE_LEN]; 
	int nBuf = _vsnprintf(szBuffer, MAX_DEBUG_LINE_LEN, lpszFormat, args); 
	assert(nBuf >= 0); 
	if(m_iDebugLevel >= 5) 
	{ 
		szBuffer[nBuf] = '\n';
		szBuffer[nBuf + 1] = 0;
		OutputStringFn(szBuffer); 
		_XDWINPRINT( szBuffer );
	} 
	va_end(args); 
}

#endif