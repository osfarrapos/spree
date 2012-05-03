/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __DIAGNOSTICS_H
#define __DIAGNOSTICS_H

namespace Audio
{

#define MAX_DEBUG_LINE_LEN 256

inline void OutputStringFn(const char* pszString)
{  
#ifdef _WINDOWS_
	OutputDebugString(pszString);
#endif
}

#ifdef _XDWDEBUG
// The diagnostics flag is by default tied to the _DEBUG flag, but can
// be left on for use in release builds if desired.
#define DIAGNOSTICS
#endif

#ifdef DIAGNOSTICS

class DIAG
{
public: 

	static void SetDiagDebugLevel(int iLevel)	
	{  m_iDebugLevel = iLevel;  }
	static void DiagDebugOut(int iLevel, const char* lpszFormat, ...);
	static void DiagFN(const char* lpszFormat, ...);
	
private:
	static int m_iDebugLevel;
};

#define DebugOut				DIAG::DiagDebugOut
#define FN						DIAG::DiagFN
#define SetDebugLevel			DIAG::SetDiagDebugLevel

#else // DIAGNOSTICS
	#define DebugOut			((void)0)
	#define FN					((void)0)
	#define SetDebugLevel		((void)0)

#endif // DIAGNOSTICS

}; // namespace Audio


#endif // __DIAGNOSTICS_H