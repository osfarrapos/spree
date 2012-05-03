// XWindow_HelpScreen.h: interface for the _XWindow_HelpScreen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_HELPSCREEN_H__002EA2F2_43B4_49F4_B234_AD4AFC7F61A9__INCLUDED_)
#define AFX_XWINDOW_HELPSCREEN_H__002EA2F2_43B4_49F4_B234_AD4AFC7F61A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#ifdef _XTS_ITEMMALLBROWSER	// 아이템몰
#define _XDEF_TOTALHELPSCREENBOX				11
#else
#define _XDEF_TOTALHELPSCREENBOX				9
#endif
typedef enum _XHS_HelpDescriptionBox
{
	_XHS_HELPBOX_STATUS = 0,		// 상태창 = 0
		_XHS_HELPBOX_MINIMAP,		// 미니맵 = 1
		_XHS_HELPBOX_QUICKSLOT,		// 단축창 = 2
		_XHS_HELPBOX_MEDETATIOH,	// 운기요상 = 3
		_XHS_HELPBOX_MESSENGER,		// 메신져 - 4
		_XHS_HELPBOX_MAIL,			// 전서구 = 5
		_XHS_HELPBOX_CINFO,			// 인물창 = 6
		_XHS_HELPBOX_INVEN,			// 행낭 = 7
		_XHS_HELPBOX_OPTION,		// 옵션 = 8
#ifdef _XTS_ITEMMALLBROWSER	// 아이템몰
		_XHS_HELPBOX_ITEMMALL,		// 아이템 상점 = 9
		_XHS_HELPBOX_ITEMBOX		// 아이템 보관함 = 10
#endif
};

class _XWindow_HelpScreen : public _XWindow  
{	
public:	
	_XImageStatic*			m_imageLeftTopEdge;
	_XImageStatic*			m_imageLeftDownEdge;
	_XImageStatic*			m_imageRightTopEdge;
	_XImageStatic*			m_imageRightDownEdge;
	
	_XImageStatic*			m_imageTitleLeftBar;
	_XImageStatic*			m_imageTitleCenterBar;
	_XImageStatic*			m_imageTitleRightBar;
	
	_XImageStatic*			m_imageArrow1;
	_XImageStatic*			m_imageArrow2;
	_XImageStatic*			m_imageArrow3;
	_XImageStatic*			m_imageArrow4;
	_XImageStatic*			m_imageArrow5;
	_XImageStatic*			m_imageArrowBody1;
	
	_XImageStatic*			m_imageMiniMap1;
	_XImageStatic*			m_imageMiniMap2;
	_XImageStatic*			m_imageMiniMap3;
	_XImageStatic*			m_imageMiniMap4;
	_XImageStatic*			m_imageMiniMap5;
			
	// 상태창,미니맵,단축창,운기요상,메신져,전서구,인물창,행낭,옵션
	LPTSTR					m_stringBoxTitle[_XDEF_TOTALHELPSCREENBOX]; 
	LPTSTR					m_stringBoxDescription[_XDEF_TOTALHELPSCREENBOX]; 

	int						m_nHelpDescriptionBoxHeight[_XDEF_TOTALHELPSCREENBOX]; //설명에 따라 높이가 달라진다.

public:
	_XWindow_HelpScreen();
	virtual ~_XWindow_HelpScreen();
	
	BOOL	Initialize(void);	
	void	Draw(_XGUIObject*& pfocusedobject = g_CurrentFocusedObject);

	void	DrawHelpDescriptionBox( int left, int top, int right, int bottom );
	void	DrawAllBox( void );
	void	SetHelpDescriptionBox( int index, LPTSTR title, LPTSTR contents );
};

#endif // !defined(AFX_XWINDOW_HELPSCREEN_H__002EA2F2_43B4_49F4_B234_AD4AFC7F61A9__INCLUDED_)
