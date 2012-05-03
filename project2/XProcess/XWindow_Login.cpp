// XWindow_Login.cpp: implementation of the _XWindow_Login class.
//
// Created Date		: 2003.04.24 
// Author			: Sohyun, Park (kukuri)
// Desc				: Login Window
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EmperorOfDragons.h"
#include "XWindow_Login.h"
#include "XProc_LoginServer.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_DrawKeyboard.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT)
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
	#endif
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_IDEditReturn( _XIMEContainer* pIMEContainer )
{
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_IDEDITENTER, 0), (LPARAM)gHWnd);
}

void __stdcall _XIMECallback_IDEditEscape( _XIMEContainer* pIMEContainer )
{	
	XProc_LoginServer* pLoginProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver;	
	_XIMEKERNEL.SetFocus( NULL );	
	_XIMEKERNEL.ResetCombinationBuffer();	
}

void __stdcall _XIMECallback_IDEditCapsLock( _XIMEContainer* pIMEContainer )
{	
	_XWindowManager* pwindowmgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.GetWindowManagerPtr();	
	if( pwindowmgr )
	{
		_XWindow_DrawKeyboard* pDrawKeyBoard_Window = (_XWindow_DrawKeyboard*)pwindowmgr->FindWindow(_XDEF_WTITLE_DRAWKEYBOARDWINDOW);
		if( pDrawKeyBoard_Window )
		{
			if( pDrawKeyBoard_Window->GetShiftDownValue() == 48 )
				pDrawKeyBoard_Window->SetShiftDownValue(0);
			else
				pDrawKeyBoard_Window->SetShiftDownValue(48);
		}
	}	
}

void __stdcall _XIMECallback_PWEditReturn( _XIMEContainer* pIMEContainer )
{
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_PWEDITENTER, 0), (LPARAM)gHWnd);
}

void __stdcall _XIMECallback_PWEditEscape( _XIMEContainer* pIMEContainer )
{	
	XProc_LoginServer* pLoginProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver;	
	_XIMEKERNEL.SetFocus( NULL );	
	_XIMEKERNEL.ResetCombinationBuffer();	
}

void __stdcall _XIMECallback_PWEditCapsLock( _XIMEContainer* pIMEContainer )
{	
	_XWindowManager* pwindowmgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.GetWindowManagerPtr();	
	if( pwindowmgr )
	{
		_XWindow_DrawKeyboard* pDrawKeyBoard_Window = (_XWindow_DrawKeyboard*)pwindowmgr->FindWindow(_XDEF_WTITLE_DRAWKEYBOARDWINDOW);
		if( pDrawKeyBoard_Window )
		{
			if( pDrawKeyBoard_Window->GetShiftDownValue() == 48 )
				pDrawKeyBoard_Window->SetShiftDownValue(0);
			else
				pDrawKeyBoard_Window->SetShiftDownValue(48);
		}
	}	
}

void __stdcall _XCallback_SaveUserID(  POINT pos, SIZE size  )
{		
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y-16, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2482), _XSC_INFORMATION );
	g_InterfaceTooltip.Draw();
}

void __stdcall _XCallback_DrawKeyboard(  POINT pos, SIZE size  )
{		
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y-16, _XGETINTERFACETEXT(ID_STRING_NEW_2789), _XSC_INFORMATION );	// _T("스크린 키보드 On / Off")
	g_InterfaceTooltip.Draw();
}

_XWindow_Login::_XWindow_Login()
{
	m_LoginButton = NULL;
	m_ExitButton = NULL;
}

_XWindow_Login::~_XWindow_Login()
{

}

BOOL _XWindow_Login::Initialize(_XTextureManager& texturearchive)
{
	_XWindow::SetDrawBorder( FALSE );

	int imageindex = texturearchive.FindResource("MI_Login_00.tga");

	_XImageStatic* pLeftBorderImage = new _XImageStatic;
	pLeftBorderImage->Create(0, 0, 237, 114, &texturearchive, imageindex );
	pLeftBorderImage->SetClipRect( 0, 0, 238, 115 );
	InsertChildObject(pLeftBorderImage);

	_XImageStatic* pRightBorderImage = new _XImageStatic;
	pRightBorderImage->Create(238, 0, 268, 114, &texturearchive, imageindex );
	pRightBorderImage->SetClipRect( 0, 116, 30, 231 );
	InsertChildObject(pRightBorderImage);

	_XBTN_STRUCTURE serverlistviewbtnstruct = 
	{
		TRUE, 
		{ 17, 86 }, { 73, 24 },
		_XDEF_LS_VIEWSERVERLISTBUTTON,
		imageindex,
		imageindex,
		imageindex,
		&texturearchive
	};
	
	_XButton* pServerlistButton = new _XButton;
	pServerlistButton->Create(serverlistviewbtnstruct);
	pServerlistButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170 );
	pServerlistButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197 );
	pServerlistButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224 );
	pServerlistButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINSERVER_2179 ); 
	pServerlistButton->SetButtonTextColor( 0xFFFFFFFF );
	pServerlistButton->SetGrowTextMode( TRUE );
	InsertChildObject(pServerlistButton);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pServerlistButton->ShowWindow( FALSE );
	}


	_XBTN_STRUCTURE loginbtnstruct = 
	{
		TRUE, 
		{ 102, 86 }, { 73, 24 },
		_XDEF_LS_LOGINBUTTON,
		imageindex,
		imageindex,
		imageindex,
		&texturearchive
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		loginbtnstruct.position.x -= 40;
	}

	m_LoginButton = new _XButton;
	m_LoginButton->Create(loginbtnstruct);
	m_LoginButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170 );
	m_LoginButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197 );
	m_LoginButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224 );
	m_LoginButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINWINDOW_CONNECT );
	m_LoginButton->SetButtonTextColor( 0xFFFFFFFF );
	m_LoginButton->SetGrowTextMode( TRUE );
	InsertChildObject(m_LoginButton);

	_XBTN_STRUCTURE exitbtnstruct = 
	{
		TRUE, 
		{ 178, 86 }, { 73, 24 },
		_XDEF_LS_EXITBUTTON,
		imageindex,
		imageindex,
		imageindex,
		&texturearchive
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		exitbtnstruct.position.x -= 40;
	}

	m_ExitButton = new _XButton;
	m_ExitButton->Create(exitbtnstruct);
	m_ExitButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170  );
	m_ExitButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197  );
	m_ExitButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224  );
	m_ExitButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINWINDOW_EXIT );
	m_ExitButton->SetButtonTextColor( 0xFFFFFFFF );
	m_ExitButton->SetGrowTextMode( TRUE );
	InsertChildObject(m_ExitButton);

	m_IDEdit.ResetContainer();
	m_IDEdit.MoveWindow(m_WindowPosition.x + 122, m_WindowPosition.y + 34);
	m_IDEdit.ResizeWindow(82, 12);
	m_IDEdit.ShowWindow(FALSE);

	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_IDEdit.SetMaxLength(12);
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_IDEdit.SetMaxLength(24);
	}
	else
	{
		m_IDEdit.SetMaxLength(24);
	}

	m_IDEdit.SetReturnKeyCallback(_XIMECallback_IDEditReturn);
	m_IDEdit.SetEscapeKeyCallback(_XIMECallback_IDEditEscape);
	m_IDEdit.SetHorizScrollMode( TRUE );
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		m_IDEdit.SetCapsLockKeyCallback(_XIMECallback_IDEditCapsLock);

	m_IDEdit.SetNextIMEContainerHandle(&m_PWEdit);
	m_IDEdit.SetFocus();

	/* testing....
#ifdef _XDWDEBUG
	m_IDEdit.SetNumericEditMode( TRUE, TRUE );
	m_IDEdit.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
#endif
	*/

	m_PWEdit.ResetContainer();
	m_PWEdit.MoveWindow(m_WindowPosition.x + 122, m_WindowPosition.y + 60);
	m_PWEdit.ResizeWindow(82, 12);
	m_PWEdit.ShowWindow(FALSE);
	m_PWEdit.SetHorizScrollMode( TRUE );
	m_PWEdit.m_VKeyboardTypedCount = 0;

	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_PWEdit.SetMaxLength(12);
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		m_PWEdit.SetMaxLength(32);
	}
	else
	{
		m_PWEdit.SetMaxLength(24);
	}

	m_PWEdit.SetReturnKeyCallback(_XIMECallback_PWEditReturn);
	m_PWEdit.SetEscapeKeyCallback(_XIMECallback_PWEditEscape);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		m_PWEdit.SetCapsLockKeyCallback(_XIMECallback_PWEditCapsLock);
	m_PWEdit.SetNextIMEContainerHandle(&m_IDEdit);
	m_PWEdit.SetPasswordMode(TRUE);
	
	int imageindex2 = texturearchive.FindResource("MI_Login_01.tga");

	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { m_WindowSize.cx - 43, 33 }, 
	{ 14, 14 }, 
	_XDEF_LS_SAVELASTLOGINUSER,
	imageindex2,imageindex2,
	&texturearchive };
	
	m_SaveUserName = new _XCheckButton;
	m_SaveUserName->Create( checkbtnstruct );	
	m_SaveUserName->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 198,89, 212, 103 );
	m_SaveUserName->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  198,73, 212, 87 );
	m_SaveUserName->SetCheck( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SAVELASTUSERNAME, FALSE, TRUE ) );
	m_SaveUserName->SetButtonSwapMode( FALSE );
	m_SaveUserName->SetCallBackFunction_ToolTip( _XCallback_SaveUserID );
	InsertChildObject(m_SaveUserName);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		checkbtnstruct.position.y += 24;
		checkbtnstruct.commandid = _XDEF_LS_DRAWKEYBOARD;

		_XCheckButton* pDrawKeyboard = new _XCheckButton;
		pDrawKeyboard->Create( checkbtnstruct );	
		pDrawKeyboard->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 198,89, 212, 103 );
		pDrawKeyboard->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  198,73, 212, 87 );
		pDrawKeyboard->SetCheck( TRUE );
		pDrawKeyboard->SetButtonSwapMode( FALSE );
		pDrawKeyboard->SetCallBackFunction_ToolTip( _XCallback_DrawKeyboard );
		InsertChildObject(pDrawKeyboard);
	}
	
	SetWindowMoveMode(_XWMOVE_FIXED);
	
	return TRUE;
}

void _XWindow_Login::DestroyWindow()
{
	m_IDEdit.ShowWindow(FALSE);
	m_PWEdit.ShowWindow(FALSE);
	
	_XWindow::DestroyWindow();
}

void _XWindow_Login::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;

	/*
	if(m_WindowPosition.y > gnHeight-154)
	{
		POINT position;
		position.x = m_WindowPosition.x;
		position.y = m_WindowPosition.y - 5;
		if(position.y < gnHeight-154)
			position.y = gnHeight-154;
		
		MoveWindow(position.x, position.y);
	}	
	else
	{
		m_IDEdit.Draw();
		m_PWEdit.Draw();
	}
	*/

	m_IDEdit.Draw();
	m_PWEdit.Draw();	

	g_XBaseFont->SetColor( 0xFFFFFFFF );
	//g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFFB5B5B5 );
	
	#ifdef _XRUSSIAN
		g_XBaseFont->Puts( m_WindowPosition.x + 23, m_WindowPosition.y + 34, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_ID) );	// "계    정"
		g_XBaseFont->Puts( m_WindowPosition.x + 23, m_WindowPosition.y + 59, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_PASSWORD) );	// "비밀번호"
	#else
		g_XBaseFont->Puts( m_WindowPosition.x + 48, m_WindowPosition.y + 36, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_ID) );	// "계    정"
		g_XBaseFont->Puts( m_WindowPosition.x + 48, m_WindowPosition.y + 61, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_PASSWORD) );	// "비밀번호"
	#endif

	g_XBaseFont->Flush();
	//g_XBaseFont->DisableGrowMode();
}

BOOL _XWindow_Login::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

//	Author : 양희왕 
//게임가드 유/무 검사 5초 마다
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)

	static DWORD iCurrentCheckTime = 0;
	if( iCurrentCheckTime == 0 )
	{
		iCurrentCheckTime = g_LocalSystemTime;
	}
	
	if( (g_LocalSystemTime - iCurrentCheckTime) > 10000 )
	{
		if( g_GameGuardInstance.Check() != NPGAMEMON_SUCCESS ) 
		{
			iCurrentCheckTime = g_LocalSystemTime;
			extern LPTSTR g_nProtectModuleCheckMessage;
			_XFatalError(g_nProtectModuleCheckMessage);
			return TRUE;
		}
	}

#endif

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckIDEditBoxArea())
				{
					m_IDEdit.SetFocus();
				}
				else if(CheckPWEditBoxArea())
				{
					m_PWEdit.SetFocus();
				}
			}
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_Login::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	m_IDEdit.MoveWindow(m_WindowPosition.x + 122, m_WindowPosition.y + 34);
	m_PWEdit.MoveWindow(m_WindowPosition.x + 122, m_WindowPosition.y + 60);
}

BOOL _XWindow_Login::CheckIDEditBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(scrnpos->x > (m_WindowPosition.x + 122) &&
		scrnpos->x < (m_WindowPosition.x + 122 + 116) &&
		scrnpos->z > (m_WindowPosition.y + 34) &&
		scrnpos->z < (m_WindowPosition.y + 34 + 12))
		return TRUE;

	return FALSE;
}

BOOL _XWindow_Login::CheckPWEditBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 122) &&
		scrnpos->x < (m_WindowPosition.x + 122 + 116) &&
		scrnpos->z > (m_WindowPosition.y + 60) &&
		scrnpos->z < (m_WindowPosition.y + 60 + 12))
		return TRUE;
	
	return FALSE;
}




_XWindow_ServerStatus::_XWindow_ServerStatus()
{
	m_pServerListUpdateButton = NULL;	
	m_pConnectButton = NULL;
	m_pLeftTitleBar = NULL;
	m_pCenterTitleBar = NULL;
	m_pCenter2TitleBar = NULL;
	m_pRightTitleBar = NULL;
}

_XWindow_ServerStatus::~_XWindow_ServerStatus()
{

}

void _XWindow_ServerStatus::ShowWindow( BOOL show )
{
	if( show )
	{
		m_BorderAlphaLevel = 0;
	}

	_XWindow::ShowWindow( show );
}

BOOL _XWindow_ServerStatus::Initialize(_XTextureManager& texturearchive)
{
	_XWindow::SetDrawBorder( FALSE );

	m_BorderAlphaLevel = 0;

	m_SelectBarAlphaLevelDirection = FALSE;
	m_SelectBarAlphaLevel = 0;

	int imageindex = texturearchive.FindResource("MI_Login_00.tga");

	m_pLeftTitleBar = new _XImageStatic;
	m_pLeftTitleBar->Create(0, 0, 80, 16, &texturearchive, imageindex );
	m_pLeftTitleBar->SetClipRect( 0, 0, 80, 16 );
	InsertChildObject(m_pLeftTitleBar);

	m_pCenterTitleBar = new _XImageStatic;
	m_pCenterTitleBar->Create(80, 0, m_WindowSize.cx-29-80, 16, &texturearchive, imageindex );
	m_pCenterTitleBar->SetClipRect( 80, 0, 112, 16 );
	m_pCenterTitleBar->SetScale( (m_WindowSize.cx-29-80) / 32, 1.0f );
	InsertChildObject(m_pCenterTitleBar);

	m_pCenter2TitleBar = new _XImageStatic;
	m_pCenter2TitleBar->Create(m_WindowSize.cx-47-29, 0, m_WindowSize.cx-30, 16, &texturearchive, imageindex );
	m_pCenter2TitleBar->SetClipRect( 191, 0, 238, 16 );
	InsertChildObject(m_pCenter2TitleBar);

	m_pRightTitleBar = new _XImageStatic;
	m_pRightTitleBar->Create(m_WindowSize.cx-29, 0, m_WindowSize.cx-1, 16, &texturearchive, imageindex );
	m_pRightTitleBar->SetClipRect( 0, 116, 30, 132 );
	InsertChildObject(m_pRightTitleBar);

	_XBTN_STRUCTURE refreshserverlistbtnstruct = 
	{
		TRUE, 
		{ (m_WindowSize.cx>>1) - 76, m_WindowSize.cy - 32 }, { 73, 24 },
		_XDEF_LS_SERVERREFRESHBUTTON,
		imageindex,
		imageindex,
		imageindex,
		&texturearchive
	};
	m_pServerListUpdateButton = new _XButton;
	m_pServerListUpdateButton->Create(refreshserverlistbtnstruct);
	m_pServerListUpdateButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170 );
	m_pServerListUpdateButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197 );
	m_pServerListUpdateButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224 );	
	m_pServerListUpdateButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINSERVER_2180 );
	m_pServerListUpdateButton->SetButtonTextColor( 0xFFFFFFFF );
	m_pServerListUpdateButton->SetGrowTextMode( TRUE );
	InsertChildObject(m_pServerListUpdateButton);

	_XBTN_STRUCTURE closeserverlistbtnstruct = 
	{
		TRUE, 
		{ (m_WindowSize.cx>>1) + 3, m_WindowSize.cy - 32 }, { 73, 24 },
		_XDEF_LS_SERVERLISTCLOSEBUTTON,
		imageindex,
		imageindex,
		imageindex,
		&texturearchive
	};
	m_pCloseServerlistButton = new _XButton;
	m_pCloseServerlistButton->Create(closeserverlistbtnstruct);
	m_pCloseServerlistButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170 );
	m_pCloseServerlistButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197 );
	m_pCloseServerlistButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224 );	
	m_pCloseServerlistButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_OPTION_CLOSE ); // _T("닫  기")
	m_pCloseServerlistButton->SetButtonTextColor( 0xFFFFFFFF );
	m_pCloseServerlistButton->SetGrowTextMode( TRUE );
	InsertChildObject(m_pCloseServerlistButton);
	
#if defined(_ACCLAIM_VERSION) || defined(_XNEXON_WEBLOGIN)
	_XBTN_STRUCTURE connectbtnstruct = 
	{
		TRUE, 
		{ (m_WindowSize.cx>>1) + 3, m_WindowSize.cy - 32 }, { 73, 24 },
		_XDEF_LS_LOGINBUTTON,
		imageindex,
		imageindex,
		imageindex,
		&texturearchive
	};
	m_pConnectButton = new _XButton;
	m_pConnectButton->Create(connectbtnstruct);
	m_pConnectButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170 );
	m_pConnectButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197 );
	m_pConnectButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224 );	
	m_pConnectButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINWINDOW_CONNECT ); // _T("접  속") 
	m_pConnectButton->SetButtonTextColor( 0xFFFFFFFF );
	m_pConnectButton->SetGrowTextMode( TRUE );
	InsertChildObject(m_pConnectButton);

	m_pCloseServerlistButton->ShowWindow( FALSE );

#ifdef _XNEXON_WEBLOGIN
	
	connectbtnstruct.commandid = _XDEF_LS_EXITBUTTON;
	connectbtnstruct.position.x = gnWidth - 80;
	connectbtnstruct.position.y = gnHeight - 28;
	m_ExitButton.Create(connectbtnstruct);
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224 );	
	m_ExitButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINWINDOW_EXIT );
	m_ExitButton.SetButtonTextColor( 0xFFFFFFFF );
	m_ExitButton.SetGrowTextMode( TRUE );
		
	int parampos = _XCmdLineOption("passport=",0,8);
	if( parampos != 0 )
	{
		m_ExitButton.ShowWindow( TRUE );
	}
	else
	{
		m_ExitButton.ShowWindow( FALSE );
	}

#endif

#endif
	
	SetWindowMoveMode(_XWMOVE_FIXED);
	
	return TRUE;
}

void _XWindow_ServerStatus::DestroyWindow()
{
	_XWindow::DestroyWindow();
	m_pServerListUpdateButton = NULL;	
	m_pLeftTitleBar = NULL;
	m_pCenterTitleBar = NULL;
	m_pRightTitleBar = NULL;
}

extern void _XDrawRoundRectAngle( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT baseheight, D3DCOLOR color );

void _XWindow_ServerStatus::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		if( m_BorderAlphaLevel < 255 )
		{
			m_BorderAlphaLevel += (int)(g_fElapsedFrameTime * 0.8f);
			if( m_BorderAlphaLevel > 255 )
				m_BorderAlphaLevel = 255;
		}

		if( m_BorderAlphaLevel > 0x7F )
		{
			_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y+16, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0x7F000000 ); 
		}
		else
		{
			_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y+16, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, D3DCOLOR_ARGB(m_BorderAlphaLevel,0,0,0) ); 
		}

		_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+15, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f, D3DCOLOR_ARGB(m_BorderAlphaLevel,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+15, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, D3DCOLOR_ARGB(m_BorderAlphaLevel,175,170,172) );
		_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+15, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(m_BorderAlphaLevel,0,0,0) );
		
		if( m_BorderAlphaLevel > 0x5F )
		{
			_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+19, m_WindowPosition.x+m_WindowSize.cx-5, m_WindowPosition.y+m_WindowSize.cy-40, 0x5F000000 ); 
		}
		else
		{
			_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+19, m_WindowPosition.x+m_WindowSize.cx-5, m_WindowPosition.y+m_WindowSize.cy-40, D3DCOLOR_ARGB(m_BorderAlphaLevel,0,0,0) ); 
		}		
	
		if( m_pLeftTitleBar )
		{
			m_pLeftTitleBar->m_FColor = D3DCOLOR_ARGB(m_BorderAlphaLevel,0xFF,0xFF,0xFF);
			m_pLeftTitleBar->Draw(pfocusobject);
		}

		if( m_pCenterTitleBar )
		{
			m_pCenterTitleBar->m_FColor = D3DCOLOR_ARGB(m_BorderAlphaLevel,0xFF,0xFF,0xFF);
			m_pCenterTitleBar->Draw(pfocusobject);
		}

		if( m_pCenter2TitleBar )
		{
			m_pCenter2TitleBar->m_FColor = D3DCOLOR_ARGB(m_BorderAlphaLevel,0xFF,0xFF,0xFF);
			m_pCenter2TitleBar->Draw(pfocusobject);
		}

		if( m_pRightTitleBar )
		{
			m_pRightTitleBar->m_FColor = D3DCOLOR_ARGB(m_BorderAlphaLevel,0xFF,0xFF,0xFF);
			m_pRightTitleBar->Draw(pfocusobject);
		}
	}

	if( m_BorderAlphaLevel < 255 ) return;

	_XWindow::Draw(pfocusobject);
	
#ifdef _XNEXON_WEBLOGIN
	m_ExitButton.Draw( pfocusobject );
#endif

	if(!this->m_ShowWindow)
		return;

	g_XBaseFont->SetColor( 0xFFFFCC43 );	
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2179) );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	if( !m_SelectBarAlphaLevelDirection ) // forward
	{
		m_SelectBarAlphaLevel += (int)(g_fElapsedFrameTime * 0.4f);

		if( m_SelectBarAlphaLevel > 255 )
		{
			m_SelectBarAlphaLevel = 255;
			m_SelectBarAlphaLevelDirection = TRUE;
		}
	}
	else // backward
	{
		m_SelectBarAlphaLevel -= (int)(g_fElapsedFrameTime * 0.4f);

		if( m_SelectBarAlphaLevel < 0 )
		{
			m_SelectBarAlphaLevel = 0;
			m_SelectBarAlphaLevelDirection = FALSE;
		}
	}
	
	int barrect			= 210;
	int statuspos		= 98;
	int statusbarpos	= 144;
	FLOAT guagesize		= 62.0f;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		barrect			= 280;
		statuspos		= 134;
		statusbarpos	= 216;
		guagesize		= 62.0f;
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		barrect			= 280;
		statuspos		= 155;
		statusbarpos	= 216;
		guagesize		= 62.0f;
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		barrect			= 280;
		statuspos		= 84;
		statusbarpos	= 216;
		guagesize		= 62.0f;
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
	{
		barrect			= 225;
		statuspos		= 113;
		statusbarpos	= 159;
		guagesize		= 62.0f;
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN)
	{
		barrect			= 280;
		statuspos		= 118;
		statusbarpos	= 216;
		guagesize		= 62.0f;
	}

	XProc_LoginServer* pLoginProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver;	

	#ifdef _X_US_EXPANDSERVERLIST
	//if( m_BorderAlphaLevel > 0x7F )
	{
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+93, m_WindowPosition.x + m_WindowSize.cx - 2, m_WindowPosition.y+93, 0x7F808080, 0x7F808080 ); 
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+94, m_WindowPosition.x + m_WindowSize.cx - 2, m_WindowPosition.y+94, 0x7F808080, 0x7F808080 );
	}

	TCHAR servergroupname[2][32] = 
	{
		_T("USA"),
		_T("EUROPE"),
	};

	g_XBaseFont->SetBoldMode( TRUE );
	g_XBaseFont->SetColor( 0xFFFBFB00 );
	g_XBaseFont->Puts( m_WindowPosition.x + 12, m_WindowPosition.y + 26 , _T("[ Region ]"), 1.0f );
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode( FALSE );

	for( int sgi = 0; sgi < 2; sgi++ )
	{
		g_XBaseFont->SetColor( 0xFF898989 );
		
		if( pLoginProcess->m_LastSelectedRegion_Server == sgi )
		{
			_XDrawRoundRectAngle( m_WindowPosition.x + 8, m_WindowPosition.y + 42 + (sgi * 20), m_WindowPosition.x + barrect - 3, m_WindowPosition.y + 61 + (sgi * 20), 0.0f, 0xFFB0A180 );
			_XDrawSolidBar(  m_WindowPosition.x + 9, m_WindowPosition.y + 42 + (sgi * 20) + 1, m_WindowPosition.x + barrect - 3 - 1, m_WindowPosition.y + 61 + (sgi * 20) - 1, 0x7F2f2f2f ); 

			g_XBaseFont->SetColor( 0xFFFFFFFF );
			g_XBaseFont->SetBoldMode(TRUE);
		}
		else
		{
			g_XBaseFont->SetBoldMode(FALSE);
		}

		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y + 46 + (sgi * 20), servergroupname[sgi], 1.0f );		
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		if( gpInput->CheckMousePos( m_WindowPosition.x + 9, m_WindowPosition.y + 42 + (sgi * 20), m_WindowPosition.x + barrect - 3, m_WindowPosition.y + 61 + (sgi * 20) ) )
		{
			if( gpInput->GetMouseState()->bButton[0] )
			{
				int previndex = pLoginProcess->m_LastSelectedRegion_Server;

				pLoginProcess->m_LastSelectedRegion_Server = sgi;				
				g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSERVERGROUP, sgi );
				
				if( previndex != pLoginProcess->m_LastSelectedRegion_Server )
				{
					m_pServerListUpdateButton->EnableWindow( FALSE );
					pLoginProcess->GetServerList( TRUE );
				}
			}
			else
			{
				_XDrawRoundRectAngle( m_WindowPosition.x + 9, m_WindowPosition.y + 42 + (sgi * 20), m_WindowPosition.x + barrect - 3, m_WindowPosition.y + 61 + (sgi * 20),
									  0.0f, D3DCOLOR_ARGB( m_SelectBarAlphaLevel, 0xBF, 0xAF, 0x8F ) );
			}
		}
	}

	#endif
	
	for( int i = 0; i < pLoginProcess->m_MaxServerCount; i++ )
	{
		if( i == _COUNT_MAX_GROUP_ ) break;

#ifdef _X_US_EXPANDSERVERLIST
		int ycoord = m_WindowPosition.y + 106 + i * 18;
#else
		int ycoord = m_WindowPosition.y + 26 + i * 18;
#endif
		
		_XDrawRectAngle( m_WindowPosition.x + statusbarpos - 1, ycoord + 2, m_WindowPosition.x + barrect - 4, ycoord + 8, 0.0f, 0xFFA1A1A1 );

		if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor < 0 )
		{
			g_XBaseFont->SetColor( 0xFF898989 );	
		}
		else if( pLoginProcess->m_LastSelectedServer == i )
		{
			_XDrawRoundRectAngle( m_WindowPosition.x + 8, ycoord - 3, m_WindowPosition.x + barrect, ycoord + 13, 0.0f, 0xFFB0A180 );
			_XDrawSolidBar(  m_WindowPosition.x + 9, ycoord - 2, m_WindowPosition.x + barrect - 1, ycoord + 12, 0x7F2f2f2f ); 
			g_XBaseFont->SetColor( 0xFFFFFFFF );				
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFD2D2D2 );	
		}
			
#ifdef _XTESTSERVER			
		if( _XCmdLineOption("xinnertestserverlogin",0) == 0 )
		{
			g_XBaseFont->Puts( m_WindowPosition.x + 12, ycoord, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2181), 1.0f );
		}
		else
		{
			g_XBaseFont->Puts( m_WindowPosition.x + 12, ycoord, g_szServerNameString[pLoginProcess->m_NextServerTrafficFactor[i].serverindex], 1.0f );
		}
#else
		g_XBaseFont->Puts( m_WindowPosition.x + 12, ycoord, g_szServerNameString[pLoginProcess->m_NextServerTrafficFactor[i].serverindex], 1.0f );
#endif


		if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor < 0 )
		{
			if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_FULL )
			{
				g_XBaseFont->SetColor( 0xFFB90000 );	
				g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2182), 1.0f );
							
				_XDrawSolidBar( m_WindowPosition.x + statusbarpos, ycoord + 3, m_WindowPosition.x + statusbarpos + guagesize, ycoord + 8,  0xFFFF3000 );
			}
			else if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_STOP )
			{
				g_XBaseFont->SetColor( 0xFF898989 );	
				g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2183), 1.0f );
			}
			else // _FOREST_STATE_OFF
			{
				g_XBaseFont->SetColor( 0xFF898989 );	
				g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2184), 1.0f );
			}

			int factor = 0;//(int)(guagesize.0f * (FLOAT)pLoginProcess->m_ServerTrafficFactor[i].trafficfactor / 100.0f);

			_XDrawSolidBar( m_WindowPosition.x + statusbarpos, ycoord + 3, m_WindowPosition.x + statusbarpos + factor, ycoord + 8,  0xFF898989 );
		}
		else if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor < 25  )
		{
			g_XBaseFont->SetColor( 0xFF00FF00 );	
			g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2185), 1.0f );

			int factor = (int)(guagesize * (FLOAT)pLoginProcess->m_ServerTrafficFactor[i].trafficfactor / 100.0f);

			_XDrawSolidBar( m_WindowPosition.x + statusbarpos, ycoord + 3, m_WindowPosition.x + statusbarpos + factor, ycoord + 8,  0xFF00FF00 );
		}
		else if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor < 50  )
		{
			g_XBaseFont->SetColor( 0xFFD2D2D2 );	
			g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_NORMAL), 1.0f );

			int factor = (int)(guagesize * (FLOAT)pLoginProcess->m_ServerTrafficFactor[i].trafficfactor / 100.0f);

			_XDrawSolidBar( m_WindowPosition.x + statusbarpos, ycoord + 3, m_WindowPosition.x + statusbarpos + factor, ycoord + 8,  0xFFD2D2D2 );
		}
		else if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor < 75  )
		{
			g_XBaseFont->SetColor( 0xFFD2D2D2 );	
			g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2186), 1.0f );
			
			int factor = (int)(guagesize * (FLOAT)pLoginProcess->m_ServerTrafficFactor[i].trafficfactor / 100.0f);
			
			_XDrawSolidBar( m_WindowPosition.x + statusbarpos, ycoord + 3, m_WindowPosition.x + statusbarpos + factor, ycoord + 8,  0xFFD2D2D2 );
		}
		else if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor <= 100  )
		{
			g_XBaseFont->SetColor( 0xFFFF3000 );	
			g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2187), 1.0f );

			int factor = (int)(guagesize * (FLOAT)pLoginProcess->m_ServerTrafficFactor[i].trafficfactor / 100.0f);

			_XDrawSolidBar( m_WindowPosition.x + statusbarpos, ycoord + 3, m_WindowPosition.x + statusbarpos + factor, ycoord + 8,  0xFFFF3000 );
		}
		/*
		else if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor < 100 )
		{
			g_XBaseFont->SetColor( 0xFFB90000 );	
			g_XBaseFont->Puts( m_WindowPosition.x + statuspos, ycoord, _T("포화"), 1.0f );

			int factor = (int)(guagesize * (FLOAT)pLoginProcess->m_ServerTrafficFactor[i].trafficfactor / 100.0f);

			_XDrawSolidBar( m_WindowPosition.x + statusbarpos, ycoord + 3, m_WindowPosition.x + statusbarpos + factor, ycoord + 8,  0xFFB90000 );
		}
		*/
		
		g_XBaseFont->Flush();

		if( g_InnerUser )
		{
			if( gpInput->CheckMousePos( m_WindowPosition.x + 9, ycoord - 2, m_WindowPosition.x + barrect - 3, ycoord + 12 ) )
			{
				if( gpInput->GetMouseState()->bButton[0] )
				{
					pLoginProcess->m_LastSelectedServer = i;
					g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, pLoginProcess->m_NextServerTrafficFactor[i].serverindex );
				}
				else
				{
					_XDrawRoundRectAngle( m_WindowPosition.x + 8, ycoord - 3, m_WindowPosition.x + barrect, ycoord + 13, 0.0f, D3DCOLOR_ARGB( m_SelectBarAlphaLevel, 0xBF, 0xAF, 0x8F ) );
				}
			}
		}
		else
		{		
			if( gpInput->CheckMousePos( m_WindowPosition.x + 9, ycoord - 2, m_WindowPosition.x + barrect - 3, ycoord + 12 ) )
			{
				if( gpInput->GetMouseState()->bButton[0] )
				{
					if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor >= 0 )
					{
						pLoginProcess->m_LastSelectedServer = i;
						g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, pLoginProcess->m_NextServerTrafficFactor[i].serverindex );
					}
				}
				else
				{
					if( pLoginProcess->m_NextServerTrafficFactor[i].trafficfactor >= 0 )
					{
						_XDrawRoundRectAngle( m_WindowPosition.x + 8, ycoord - 3, m_WindowPosition.x + barrect, ycoord + 13, 0.0f, D3DCOLOR_ARGB( m_SelectBarAlphaLevel, 0xBF, 0xAF, 0x8F ) );
					}
				}
			}
		}
	}
}

BOOL _XWindow_ServerStatus::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;

#ifdef _XNEXON_WEBLOGIN
	if(!m_ExitButton.Process( pfocusobject ))
		return FALSE;
#endif

	return TRUE;
}

void _XWindow_ServerStatus::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}
