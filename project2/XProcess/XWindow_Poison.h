// XWindow_Poison.h: interface for the _XWindow_Poison class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_POISON_H__5086BFC6_0AE6_4F55_AA69_4C509F938E11__INCLUDED_)
#define AFX_XWINDOW_POISON_H__5086BFC6_0AE6_4F55_AA69_4C509F938E11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum
{
	_MODE_NONE = 0,
	_MODE_BOSS,
	_MODE_PAEWANG
};


class _XWindow_Poison  : public _XWindow
{
private :
	_XImageStatic*		m_imgBossPoison;
	_XImageStatic*		m_imgPaewangPoison;

	int					m_Mode;


public:
	_XWindow_Poison();
	virtual ~_XWindow_Poison();

	BOOL				Initialize( void );
	void				DestroyWindow( void );

	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

    void	ShowWindow(BOOL show);

	void	SetMode(int mode)	{	m_Mode = mode;		};

};

#endif // !defined(AFX_XWINDOW_POISON_H__5086BFC6_0AE6_4F55_AA69_4C509F938E11__INCLUDED_)
