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




//------------------------------------------------------------------------//
CreateAudioStream::CreateAudioStream(IAudioStream*& pStream)
{
	m_pStream = 0;
	if(AudioMgr()->CreateAudioStream(pStream))
		m_pStream = pStream;
}


//------------------------------------------------------------------------//
CreateAudioStream::~CreateAudioStream()
{
	_XASAFE_RELEASE(m_pStream);
}






//-----------------------------------------------------------------------------
// Name: ConvertAnsiStringToWide()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void Audio::ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, 
                                     int cchDestChar )
{
    if( wstrDestination==NULL || strSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = strlen(strSource)+1;

    MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
                         wstrDestination, cchDestChar-1 );

    wstrDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: ConvertWideStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void Audio::ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar )
{
    if( strDestination==NULL || wstrSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = wcslen(wstrSource)+1;

    WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
                         cchDestChar-1, NULL, NULL );

    strDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: ConvertGenericStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void Audio::ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( strDestination==NULL || tstrSource==NULL || cchDestChar == 0 )
        return;

#ifdef _UNICODE
    ConvertWideStringToAnsi( strDestination, tstrSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    {
        strcpy( strDestination, tstrSource );
    }
    else
    {
        strncpy( strDestination, tstrSource, cchDestChar );
        strDestination[cchDestChar-1] = '\0';
    }
#endif
}




//-----------------------------------------------------------------------------
// Name: ConvertGenericStringToWide()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void Audio::ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( wstrDestination==NULL || tstrSource==NULL || cchDestChar == 0 )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
    {
        wcscpy( wstrDestination, tstrSource );
    }
    else
    {
        wcsncpy( wstrDestination, tstrSource, cchDestChar );
        wstrDestination[cchDestChar-1] = L'\0';
    }
#else
	Audio::ConvertAnsiStringToWide( wstrDestination, tstrSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void Audio::ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || strSource==NULL || cchDestChar == 0 )
        return;
        
#ifdef _UNICODE
    ConvertAnsiStringToWide( tstrDestination, strSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    {
        strcpy( tstrDestination, strSource );
    }
    else
    {
        strncpy( tstrDestination, strSource, cchDestChar );
        tstrDestination[cchDestChar-1] = '\0';
    }
#endif
}




//-----------------------------------------------------------------------------
// Name: ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void Audio::ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || wstrSource==NULL || cchDestChar == 0 )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
    {
        wcscpy( tstrDestination, wstrSource );
    }
    else
    {
        wcsncpy( tstrDestination, wstrSource, cchDestChar );
        tstrDestination[cchDestChar-1] = L'\0';
    }
#else
	Audio::ConvertWideStringToAnsi( tstrDestination, wstrSource, cchDestChar );
#endif
}



//------------------------------------------------------------------------//
VarAdjust::VarAdjust()
{
	Clear();
}


//------------------------------------------------------------------------//
void VarAdjust::Clear()
{
	m_fStartTime = 1.0f;
	m_fEndTime = 1.0f;
	m_fStartVal = 1.0f;
	m_fEndVal = 1.0f;
	m_bFirstCheck = false;
}


//------------------------------------------------------------------------//
void VarAdjust::Init(float fStartVal, float fEndVal, float fTime)
{
	m_fStartTime = float(timeGetTime()) / 1000.0f;
	m_fEndTime = m_fStartTime + fTime;
	m_fStartVal = fStartVal;
	m_fEndVal = fEndVal;
	m_bFirstCheck = true;
}



//------------------------------------------------------------------------//
float VarAdjust::GetVar()
{
	float fPercent = Clamp<float>((((float(timeGetTime()) / 1000.0f) - m_fStartTime) / (m_fEndTime - m_fStartTime)), 0.0f, 1.0f);
	float fVolume = m_fStartVal + (fPercent * (m_fEndVal - m_fStartVal));
	return fVolume;
}


//------------------------------------------------------------------------//
bool VarAdjust::IsFinished()
{
	// This ensures that the variable will always return true at least one iteration,
	// preventing a situation where the variable will not quite reach its target
	// destination, if this is checked for before doing the calculations via the
	// GetVar() function.
	if(m_bFirstCheck)
	{
		m_bFirstCheck = false;
		return false;
	}
	return ((float(timeGetTime()) / 1000.0f) >= m_fEndTime);
}

