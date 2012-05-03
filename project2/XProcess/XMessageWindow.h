// XMessageWindow.h: interface for the _XMessageWindow class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMESSAGEWINDOW_H_
#define _XMESSAGEWINDOW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGUIControlCenter.h"
#include "XButton.h"
#include "XStatic.h"
#include "XImageStatic.h"
#include "XTextureManager.h"
#include "XIME.h"
#include "XWindow.h"
#include "XMessageBox.h"

class _XMessageWindow  : public _XWindow
{
public:
	int					m_OkCmdID;
	int					m_CancelCmdID;
	int					m_CloseCmdID;
	int					m_ButtonMode;
	int					m_MessageBoxCancelBtnView;	
	
	TCHAR				m_Message[_XDEF_MAXMESSAGEBUFFERSIZE];	
	POINT				m_MessagePos;
	TCHAR				m_TitleString[_XDEF_MAXTITLEBUFFERSIZE];	
	POINT				m_TitlePos;
	
	_XList				m_MessageList;
	
	_XImageStatic		m_LeftBorderStatic;
	_XImageStatic		m_RightBorderStatic;
	_XButton			m_OkBtn;
	_XButton			m_CancelBtn;
	_XButton			m_CloseBtn;
	
	D3DCOLOR			m_MessageColor;
	
	BOOL				m_IMEPasswordMode;
	_XIMEContainer		m_IMEControl;
	
	BOOL				m_TimerMode;
	BOOL				m_SendCancelMessage;
	LONG				m_Timer;
	LONG				m_ViewTime;

public:
	_XMessageWindow();
	virtual ~_XMessageWindow();

	static _XMessageWindow*	CreateMessageBox( _XWindowManager* pWindowManager, DWORD windowid, BOOL bCash = FALSE );
	
	void Create( _XWindowManager* pWindowManager, DWORD windowid, BOOL bCash = FALSE );
	void DestroyWindow( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	int  Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void ShowWindow( BOOL show );
	BOOL OnKeyDown(WPARAM wparam, LPARAM lparam);	
	void MoveWindow( int X, int Y );
	
	void SetMessage( int btntype, int resourceid, BOOL showwindow = TRUE, 
		DWORD okcmdid = 0, DWORD cancelcmdid = 0, DWORD closecmdid = 0 );
	void SetMessage( int btntype, LPSTR title, LPSTR message, BOOL showwindow = TRUE, 
		DWORD okcmdid = 0, DWORD cancelcmdid = 0, DWORD closecmdid = 0 );
	void SetMessage( int resourceid );
	void SetMessage( TCHAR* argv, ... );
	
	void StartTimer( int counttime, BOOL sendcancelmessage = FALSE );
	
	void SetPasswordMode( BOOL passwordmode ){ 	m_IMEPasswordMode = passwordmode; }
	
	void SetOkButtonCmdID( DWORD id ){  m_OkCmdID = id; }
	void SetCancelButtonCmdID( DWORD id ){  m_CancelCmdID = id; }
	void SetCloseButtonCmdID( DWORD id ){  m_CloseCmdID = id; }
	
	void SetMessageColor( D3DCOLOR color = _XSC_DEFAULT ){ m_MessageColor = color; }
	void SetBorderAlphaLevel( DWORD alphalevel = 255 )
	{ 		
		m_solidborderstatic.SetFaceColor( D3DCOLOR_ARGB( alphalevel, 0, 0, 0) );
	}
};

#endif // !defined(_XMESSAGEWINDOW_H_)
