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

#define MAX_ERROR_MESSAGES 1000

#define _countof(array) (sizeof(array)/sizeof(array[0]))

using namespace std;

std::list<std::string> Error::m_ErrorLog;
std::list<std::string>::iterator Error::m_ErrorLogItor;

extern void _XLog (TCHAR *msg, ...);

#define _XCOMMENT / ## / 

#ifdef _XDWDEBUG
extern void _XDWINPRINT( TCHAR *msg, ... );
extern void _XDWINPRINTONLY( TCHAR *msg, ... );
#else
#define _XDWINPRINT		_XLog	//_XCOMMENT
#define _XDWINPRINTONLY _XCOMMENT
#endif

//------------------------------------------------------------------------//
bool Error::Handle(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	TCHAR szBuffer[512];

	if( lpszFormat )
	{
		nBuf = _vsntprintf(szBuffer, _countof(szBuffer), lpszFormat, args);
		// was there an error? was the expanded string too long?
		assert(nBuf >= 0);
	}
	else
		szBuffer[0] = NULL;	// no string passed in.

	// store the error in the general error log (this will also print debug output)
	Log(szBuffer);

	va_end(args);

	return false;
}


//------------------------------------------------------------------------//
// Error error logging functions
void Error::Log(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	TCHAR szBuffer[512];

	if( lpszFormat )
	{
		nBuf = _vsntprintf(szBuffer, _countof(szBuffer), lpszFormat, args);
		// was there an error? was the expanded string too long?
		assert(nBuf >= 0);
	}
	else
		szBuffer[0] = NULL;	// no string passed in.

	m_ErrorLog.push_back(szBuffer);
	DebugOut(1, szBuffer);

	_XDWINPRINT( szBuffer );

	// Limit the number of lines allowed in the log (defaults to 1000)
	if(m_ErrorLog.size() > MAX_ERROR_MESSAGES)
		m_ErrorLog.pop_front();

	va_end(args);
}


//------------------------------------------------------------------------//
void Error::ClearLog()
{
	m_ErrorLog.clear();
}


//------------------------------------------------------------------------//
const char* Error::GetFirst()
{
	if(m_ErrorLog.empty())
		return NULL;
	m_ErrorLogItor = m_ErrorLog.begin();
	return (*m_ErrorLogItor).c_str();
}


//------------------------------------------------------------------------//
const char* Error::GetNext()
{
	if(++m_ErrorLogItor == m_ErrorLog.end())
		return NULL;
	return (*m_ErrorLogItor).c_str();
}

//------------------------------------------------------------------------//
const char* Error::GetLast()
{
/*
	if(m_ErrorLog.empty())
		return NULL;
	m_ErrorLogItor = m_ErrorLog.end();
	m_ErrorLogItor--;
	return (*m_ErrorLogItor).c_str();
*/
	return NULL;
}


