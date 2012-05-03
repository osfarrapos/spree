/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	UnicodeHelpers.h
Owner:	russf@gipsysoft.com
Purpose:

	Helper functions and classes for dealing with unicode conversions

----------------------------------------------------------------------*/
#ifndef UNICODEHELPERS_H
#define UNICODEHELPERS_H

class CAnsiToUnicode
{
public:
	CAnsiToUnicode( LPCSTR pcszText, UINT uLength )
	{
		m_pszText = new WCHAR [ uLength * sizeof( TCHAR ) + 1 ];
		MultiByteToWideChar( CP_ACP, 0, pcszText, uLength, (WCHAR*)m_pszText, uLength * sizeof( TCHAR ) );
		m_pszText[ uLength ] = '\000';
	}

	~CAnsiToUnicode()
	{
		delete[] m_pszText;
	}

	LPCSTR AsAnsi() const { return (LPCSTR)m_pszText; }

	operator const LPCWSTR () const { return m_pszText; }

private:
	LPWSTR m_pszText;
};


class CUnicodeToAnsi
{
public:
	CUnicodeToAnsi( LPCWSTR pcszText, UINT uLength )
	{
		m_pszText = new char [ uLength * sizeof( TCHAR ) + 1 ];
		WideCharToMultiByte( CP_UTF8, 0, pcszText, uLength, m_pszText, uLength * sizeof( TCHAR ), NULL, NULL );
		m_pszText[ uLength ] = '\000';
	}

	~CUnicodeToAnsi()
	{
		delete[] m_pszText;
	}

	LPCWSTR AsUnicode() const { return (LPCWSTR)m_pszText; }

	operator const LPCSTR () const { return m_pszText; }

private:
	LPSTR m_pszText;
};



#endif //UNICODEHELPERS_H