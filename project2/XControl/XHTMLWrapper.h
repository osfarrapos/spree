// XHTMLWrapper.h: interface for the _XHTMLWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLWRAPPER_H__55E6FD5F_6D93_4820_B1B3_D7DF30216341__INCLUDED_)
#define AFX_XHTMLWRAPPER_H__55E6FD5F_6D93_4820_B1B3_D7DF30216341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/*
#ifdef _XVIETNAMESE
	#ifndef _UNICODE
		#define _UNICODE
			#include "qhtm.h"
		#undef _UNICODE
	#endif
#else
	#include "qhtm.h"
#endif
*/

#include "qhtm.h"

#define _XDEF_MAX_URLSTRINGLENGTH	512

class _XHTMLWrapper  
{
public:
	LPDIRECT3DTEXTURE9		m_pHTMLTexture;
	UINT					m_uBufferWidth;
	UINT					m_uMaxHeight;
	UINT					m_uViewHeight;

	TCHAR					m_LastBuildURLAdress[_XDEF_MAX_URLSTRINGLENGTH];
	int						m_LastBuildScrollPos;

	//	Dib data
	HANDLE                  m_hDIB;

public:
	_XHTMLWrapper();
	virtual ~_XHTMLWrapper();

	static BOOL InitializeQHTMLibrary( void );
	static void ReleaseQHTMLibrary( void );

	BOOL Initialize( int buffersize );
	BOOL RestoreDeviceObject( void );
	void Release( void );


	BOOL LoadHTML( LPTSTR szURLString, int imaxWidth );
	BOOL DDBToDIB( HGDIOBJ hbmp );
	BOOL UpdateTextureBuffer( int ScrollPos );

	void SetViewHeight( UINT height )
	{
		 m_uViewHeight = height;
	}
};

#endif // !defined(AFX_XHTMLWRAPPER_H__55E6FD5F_6D93_4820_B1B3_D7DF30216341__INCLUDED_)
