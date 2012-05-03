// XWindow_CBResultNotice.h: interface for the _XWindow_CBResultNotice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CBRESULTNOTICE_H__56BCDA3A_6724_48D5_B3A7_461904B0C2D3__INCLUDED_)
#define AFX_XWINDOW_CBRESULTNOTICE_H__56BCDA3A_6724_48D5_B3A7_461904B0C2D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CBResultNotice  : public _XWindow
{
private :
	int				m_nCastleNameTextId;
	TCHAR			m_strOrgName[256];
	
	BOOL			m_bMonsterWin;
	
public:
	_XWindow_CBResultNotice();
	virtual ~_XWindow_CBResultNotice();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void			ShowWindow(BOOL show);

	void			SetCastleName(int index);
	void			SetOrgName(TCHAR* orgname, BOOL bMonsterWin, int orgtype);

};

#endif // !defined(AFX_XWINDOW_CBRESULTNOTICE_H__56BCDA3A_6724_48D5_B3A7_461904B0C2D3__INCLUDED_)
