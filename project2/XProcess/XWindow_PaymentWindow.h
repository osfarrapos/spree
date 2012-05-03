// XWindow_PaymentWindow.h: interface for the _XWindow_PaymentWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PAYMENTWINDOW_H__A72A422B_484A_46B6_96AE_ED017B439A10__INCLUDED_)
#define AFX_XWINDOW_PAYMENTWINDOW_H__A72A422B_484A_46B6_96AE_ED017B439A10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

// PC방 클래스와 SDS 클래스 분리
// SDS 클래스로 사용할 것

class _XWindow_PaymentWindow : public _XWindow  
{

public:

public:
	_XWindow_PaymentWindow();
	virtual ~_XWindow_PaymentWindow();

	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void MoveWindow( int X, int Y );

	void ResetTitleBar( void );
	
	void ShowWindow( BOOL show );
};

#endif // !defined(AFX_XWINDOW_PAYMENTWINDOW_H__A72A422B_484A_46B6_96AE_ED017B439A10__INCLUDED_)
