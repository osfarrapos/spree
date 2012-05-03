// XWindow_EventIcon.h: interface for the XWindow_EventIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EVENTICON_H__6A4494D8_FB98_4820_8B85_671BB687A16E__INCLUDED_)
#define AFX_XWINDOW_EVENTICON_H__6A4494D8_FB98_4820_8B85_671BB687A16E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XTextureAnimation;

class _XWindow_EventIcon : public _XWindow
{
public:
	_XWindow_EventIcon();
	~_XWindow_EventIcon();

	BOOL	Initialize( void );
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	void	SetTime( DWORD dwTime ); 
	void	Destroy();

	void	ShowMessageBox();

	void	SetEventType(int type)
	{
		m_nEventType = type;
	};
    const BOOL GetShowMessageBox() const { return m_bShowMessageBox; }


private :
	_XTextureAnimation* m_pPresentAni; //선물 상자
	bool m_bShowItem;
	
	int		m_nEventType;
	BOOL	m_bShowMessageBox;
};

#endif // !defined(AFX_XWINDOW_EVENTICON_H__6A4494D8_FB98_4820_8B85_671BB687A16E__INCLUDED_)
