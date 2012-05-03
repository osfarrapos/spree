// XWindow_BinkStatic.h: interface for the _XWindow_BinkStatic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_BINKSTATIC_H_
#define _XWINDOW_BINKSTATIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _ACCLAIM_IGAADSYSTEM

#include "XKernel.h"
#include "X3PCamera.h"

#include "bink.h"
#include "rad3d.h"

class _XWindow_BinkStatic : public _XWindow  
{

public:
	RAD3D			m_Rad3D;
	HBINK			m_BinkObject;
	
	int				m_maximum_texture_size;	
	HRAD3DIMAGE		m_RadImage;
	TCHAR			m_szBinkObjectFilename[_MAX_PATH];
	FLOAT			m_AlphaLevel;
	BOOL			m_PlayFlag;
	
	BOOL			m_EndOfFrame;
	
	DWORD			m_ViewPortWidth;
	DWORD			m_ViewPortHeight;

	_XImageStatic	m_DownloadMessageWindow;
	_XImageStatic	m_DownloadMessageIndicator;
	int				m_IndicatorIndex;
	BOOL			m_IndicatorDirection;

public:
	_XWindow_BinkStatic();
	virtual ~_XWindow_BinkStatic();

	BOOL			Initailize(void);
	void			DestroyWindow(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			ShowWindow(BOOL show);
	void			SetWindowSize(int width, int height);


	BOOL			BinkInitialize( LPTSTR binkobjectfilename, LPVOID binkobjectmemptr = NULL );
	void			BinkFinalize( void );
	S32				Allocate_3D_images( void );
	void			Decompress_frame( S32 copy_all, S32 never_skip );

	void			SetViewPortSize( DWORD width, DWORD height )
	{
					m_ViewPortWidth  = width;
					m_ViewPortHeight = height;
	}
};

#endif //_ACCLAIM_IGAADSYSTEM

#endif // !defined(_XWINDOW_BINKSTATIC_H_)
