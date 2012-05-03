// XWindow_BookOpen.h: interface for the _XWindow_BookOpen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_BOOKOPEN_H__35DDEF64_84F5_4C60_BE96_FFA4D600835A__INCLUDED_)
#define AFX_XWINDOW_BOOKOPEN_H__35DDEF64_84F5_4C60_BE96_FFA4D600835A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 일반 책 읽기 창
 * ItemType이 Book 이고 SecondType이 _XGI_SC_BOOK_SCROLL, _XGI_SC_BOOK_STUFF, _XGI_SC_BOOK_PAPER
 * 인 아이템은 읽을 수 있는 책으로 분류 그냥 보는 기능만 있음 
 *
 * SetBookOpenMode(int fileid, char mode )함수에서 mode와 fileid 값으로 해당 html 을 찾아서 보여준다.
 * 해당 html들은 book폴더 안에 있음.
*/

class _XWindow_BookOpen : public _XWindow  
{
private:
	_XImageStatic*	m_pMainBorder;
	_XImageStatic*	m_pTitleBorder;
	TCHAR			m_TitleName[64];

public:
	_XWindow_BookOpen();
	virtual ~_XWindow_BookOpen();
	
	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusedobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	ProcessWheelMessage( short zDelta );

	void	MoveWindow( int X, int Y );
	BOOL	SetBookOpenMode(int fileid, char mode );
	void	SetTitleName(short sID);

public:
	_XVScrollBar		m_HTMLListScrollBar;
	_XGUIObject*		m_pScrbarFocusedObject;

	_XImageStatic		m_HTMLViewStatic;
	int					m_HTMLListCount;	

};

#endif // !defined(AFX_XWINDOW_BOOKOPEN_H__35DDEF64_84F5_4C60_BE96_FFA4D600835A__INCLUDED_)
