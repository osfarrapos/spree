// XMString.h: interface for the _XMString class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMSTRING_H_
#define _XMSTRING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"
#include "XTextureManager.h"

#define _XDEF_MULTISTRINGARRAYCOUNT		7

class _XMString : public _XItem  
{
protected:
	_XTextureManager*		m_pIconImageArchive;
	int						m_CommandID;
	LPTSTR					m_stringarray[ _XDEF_MULTISTRINGARRAYCOUNT ];
	DWORD					m_attribute[ _XDEF_MULTISTRINGARRAYCOUNT ];
	
	RECT					m_drawrect[ _XDEF_MULTISTRINGARRAYCOUNT ];
	DWORD					m_attributeHighlight[_XDEF_MULTISTRINGARRAYCOUNT];
	BOOL					m_bBoldString;
	
	BOOL					m_bSepaateString;	// 자동 줄바꿈 기능 여부 판단
	int						m_StringWidth;		// 너비이상이면 줄바꿈.

public:

public:
	_XMString();
	_XMString( LPTSTR str, DWORD attrib = 0, int commandid=0 );
	_XMString( wchar_t* str, DWORD attrib = 0, int commandid=0 );
	virtual ~_XMString();

	void	Reset( void );
	void	ResetString( int index );
	
	void	SetCommandID( int commandid ){ m_CommandID = commandid; }
	int		GetCommandID( void ){ return m_CommandID; }

	BOOL	SetString( LPTSTR str, int index = 0 );
	BOOL	SetString( wchar_t* str, int index = 0 );
	LPTSTR	GetString( int index = 0 );

	BOOL	SetAttrib( DWORD attrib, int index = 0 );
	DWORD	GetAttrib( int index = 0 );

	BOOL	SetAttrib(DWORD attrib, int index, RECT rect);
	RECT	GetRect(int index)
	{
		return m_drawrect[index];
	}

	BOOL	SetAttribHighlight(DWORD attrib, int index = 0);
	DWORD	GetAttribHighlight(int index = 0);

	void	SetIconArchive( _XTextureManager* pIconArchive )
	{
			m_pIconImageArchive = pIconArchive;
	}
	_XTextureManager* GetIconArchive( void )
	{
		return m_pIconImageArchive;
	}
	
	void	SetBoldString( BOOL boldstring){ m_bBoldString = boldstring; }
	BOOL	GetBoldString( void ){ return m_bBoldString; }
	
	void	SetSeparateString( BOOL separatestring){ m_bSepaateString = separatestring; }
	BOOL	GetSeparateString( void ){ return m_bSepaateString; }
	void	SetStringWidth( int width){ m_StringWidth = width; }
	int		GetStringWidth( void ){ return m_StringWidth; }
};

#endif // !defined(_XMSTRING_H_)
