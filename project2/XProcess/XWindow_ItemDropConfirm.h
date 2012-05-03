// XWindow_ItemDropConfirm.h: interface for the _XWindow_TradeConfirm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ITEMDROPCONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_)
#define AFX_XWINDOW_ITEMDROPCONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "XButton.h"
#include "XIME.h"
#include "XSR_STRINGHEADER.H"

typedef enum _XDrop_Mode
{
	_XDROP_NONE = 0,
		_XDROP_ITEM1,
		_XDROP_MONEY1,
		_XDROP_SELECTION
}_XDROP_MODE;

class _XWindow_ItemDropConfirm : public _XWindow  
{
public :	
	_XButton*		m_OKButton;
	_XButton*		m_CancelButton;
	_XButton*		m_AllButton;// 2004.05.17->oneway48 insert
	
	_XIMEContainer	m_IMEControl;
	
	int				m_DropCount;
	
	char			m_SelectedCategory1;
	short			m_SelectedItemID;
	
	_XDROP_MODE		m_DropMode;
	
	TCHAR			m_CountString[64];	

	int				m_SelectionQuestID;	
public:
	_XWindow_ItemDropConfirm();
	virtual ~_XWindow_ItemDropConfirm();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);	
	
	BOOL			CheckIMEBoxArea(void);
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);	
	void			SetMode( _XDROP_MODE mode );
	void			SetMessage( LPSTR messagestring );
};

#endif // !defined(AFX_XWINDOW_ITEMDROPCONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_)
