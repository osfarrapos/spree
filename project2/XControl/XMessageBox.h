// XMessageBox.h: interface for the _XMessageBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMESSAGEBOX_H_
#define _XMESSAGEBOX_H_

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

#define _XDEF_MAXMESSAGEBUFFERSIZE		512
#define _XDEF_MAXTITLEBUFFERSIZE		64

#define _XDEF_DLGBTNTYPE_NOTICE			  0
#define _XDEF_DLGBTNTYPE_OK				  1
#define _XDEF_DLGBTNTYPE_YESNO			  2
#define _XDEF_DLGBTNTYPE_YESNOCANCEL	  3
#define _XDEF_DLGBTNTYPE_CANCEL			  4
#define _XDEF_DLGBTNTYPE_INPUTSTRING	  5
#define _XDEF_DLGBTNTYPE_SOCIALSENUMBER	  6


#define _XDEF_MESSAGEDLGBUTTON_OK	    999
#define _XDEF_MESSAGEDLGBUTTON_CANCEL   998
#define _XDEF_MESSAGEDLGBUTTON_CLOSE    997

class _XMessageBox 
{
public:
	POINT				m_WindowPosition;
	SIZE				m_WindowSize;
	BOOL				m_ShowWindow;

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

	_XStatic            m_BorderWindow;
	_XImageStatic		m_LeftBorderStatic;
	_XImageStatic		m_RightBorderStatic;
	_XButton			m_OkBtn;
	_XButton			m_CancelBtn;
	_XButton			m_CloseBtn;

	_XGUIObject*		m_FocusedObject;
	D3DCOLOR			m_MessageColor;
	
	BOOL				m_Dragging;
	POINT				m_DragStartPos;

	BOOL				m_IMEPasswordMode;
	_XIMEContainer		m_IMEControl;

	BOOL				m_TimerMode;
	BOOL				m_SendCancelMessage;
	LONG				m_Timer;	
	LONG				m_ViewTime;

	DWORD				m_TextSplitWidth;

public:
	_XMessageBox();
	virtual ~_XMessageBox();

	void Create( void );
	void DestroyWindow( void );
	void Draw( void );
	int  Process( void );

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

	void ShowWindow( BOOL show );
	
	inline BOOL GetShowStatus( void ){ return m_ShowWindow;	}
	void SetMessageColor( D3DCOLOR color = _XSC_DEFAULT ){ m_MessageColor = color; }
	void SetBorderAlphaLevel( DWORD alphalevel = 255 )
	{ 		
		m_BorderWindow.SetFaceColor( D3DCOLOR_ARGB( alphalevel, 0, 0, 0) );		
	}

	bool OnKeyDown(WPARAM wparam, LPARAM lparam);

	void MoveWindow( int X, int Y );
	BOOL CheckMousePosition( void );
};

#endif // !defined(_XMESSAGEBOX_H_)
