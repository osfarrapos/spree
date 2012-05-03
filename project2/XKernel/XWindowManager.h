// XWindowManager.h: interface for the _XWindowManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOWMANAGER_H_
#define _XWINDOWMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XGUIControlCenter.h"

#include <map>

class _XWindow;

class _XWindowManager  
{
private:
	typedef std::multimap<DWORD, _XWindow*>		smdef_XWindow;

	_XList		m_WindowList;
	_XWindow*	m_pFocusedWindow;
	BOOL		m_CheckSelfDestroyWindow;

	BOOL			m_bStlInit;
	smdef_XWindow	m_smWindow;

public:
	_XWindowManager();
	virtual ~_XWindowManager();
	
	void		DestroyManager( void );
	void		Draw( void );
	void		Process( void );
	BOOL		ProcessWheelMessage( short zDelta );
	BOOL		OnKeyDown(WPARAM wparam, LPARAM lparam);

	_XWindow*	InsertWindow( _XWindow* xguiobject );
	void		DeleteWindow( int windowid );
	void		DeleteAllWindow( int windowid );

	_XWindow*	FindWindow( LPSTR windowtitle );
	_XWindow*	FindWindow( DWORD windowid );
	int			FindWindowIndex( LPSTR windowtitle );
	int			FindWindowIndex( DWORD windowid );
	_XWindow*	GetWindow( int index );
	int			GetWindowCount( void ){  return m_WindowList.getitemcount(); }
	void		SetTopWindow( _XWindow* ptopwindow );
	void		RebuildDockedWindow(void);
	void		SetDefaultPosition( void );
	
	BOOL		IsTopWindow(_XWindow* topwindow);
	BOOL		EscapeWindow();//2004.06.24->oneway48 insert

	void		EnableCheckSelfDestroyWindow( void ){ m_CheckSelfDestroyWindow = TRUE; }
	
	void		NotifyUIChange( void );
};

struct _XWINDOWEFFECTVERTEXTYPE
{
    FLOAT			x, y, z, rhw;      // The untransformed, 3D position for the vertex
	D3DCOLOR		diffuse;	
};

#define D3DFVF_XWINDOWEFFECTVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )
extern _XWINDOWEFFECTVERTEXTYPE g_WindowEffectVertices[8];

extern RECT g_WindowCloseRect;


#endif // !defined(AFX_XWINDOWMANAGER_H__9060AA65_42E6_4A03_84AD_822181B078CE__INCLUDED_)
