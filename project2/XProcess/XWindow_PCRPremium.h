// XWindow_PCRPremium.h: interface for the _XWindow_PCRPremium class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PCRPREMIUM_H__B5AD5D14_5602_4106_8B26_393A6CB73D63__INCLUDED_)
#define AFX_XWINDOW_PCRPREMIUM_H__B5AD5D14_5602_4106_8B26_393A6CB73D63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
@calss _XWindow_PCRPremium
@breif PC방 클래스 
@author 박소현
@author _XWindow_PaymentWindow 에서 _XWindow_PCRPremiun으로 옮김
@date 2007년 9월 19일
*/

class _XWindow_PCRPremium  : public _XWindow
{
public:
	_XWindow_PCRPremium();
	virtual ~_XWindow_PCRPremium();

	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void MoveWindow( int X, int Y );
	void ShowWindow( BOOL show );
};

#endif // !defined(AFX_XWINDOW_PCRPREMIUM_H__B5AD5D14_5602_4106_8B26_393A6CB73D63__INCLUDED_)
