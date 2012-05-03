// XWindow_AgeGrade.h: interface for the XWindow_AgeGrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_AGEGRADE_H__07B0E340_7B08_4053_A009_1A4021449FDA__INCLUDED_)
#define AFX_XWINDOW_AGEGRADE_H__07B0E340_7B08_4053_A009_1A4021449FDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XWindow_AgeGrade : public _XWindow  
{
public:
	_XWindow_AgeGrade();
	~_XWindow_AgeGrade();
	static _XWindow_AgeGrade*  GetInstance();

	BOOL	Initialize( void );
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	void	SetTime( DWORD dwTime ); 
	void	Destroy();

private :
	static _XWindow_AgeGrade* m_pInstance;
	DWORD	m_dwCurrentCheckTime;
};

inline void _XWindow_AgeGrade::SetTime( DWORD dwTime )
{
	m_dwCurrentCheckTime = dwTime;
}

#endif // !defined(AFX_XWINDOW_AGEGRADE_H__07B0E340_7B08_4053_A009_1A4021449FDA__INCLUDED_)
