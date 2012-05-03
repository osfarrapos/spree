// XWindow_Login.h: interface for the _XWindow_Login class.
//
// Created Date		: 2003.04.24 
// Author			: Sohyun, Park (kukuri)
// Desc				: Login Window
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUXWINDOW_LOGIN_H__B8E7C427_1539_4B3E_AE0E_BF41DA510DA7__INCLUDED_)
#define AFX_KUXWINDOW_LOGIN_H__B8E7C427_1539_4B3E_AE0E_BF41DA510DA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "XIME.h"

class _XWindow_Login : public _XWindow  
{
public :
	_XButton*			m_LoginButton;
	_XButton*			m_ExitButton;
	
	_XIMEContainer		m_IDEdit;
	_XIMEContainer		m_PWEdit;
	
	_XCheckButton*		m_SaveUserName;
	
public:
	_XWindow_Login();
	virtual ~_XWindow_Login();
	
	BOOL				Initialize(_XTextureManager& texturearchive);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				MoveWindow(int X, int Y);

	BOOL				CheckIDEditBoxArea(void);
	BOOL				CheckPWEditBoxArea(void);
};


class _XWindow_ServerStatus : public _XWindow  
{
public :
	
	BOOL				m_SelectBarAlphaLevelDirection;
	int					m_SelectBarAlphaLevel;		

	_XButton*			m_pServerListUpdateButton;
	_XButton*			m_pConnectButton;
	_XButton*			m_pCloseServerlistButton;
	_XImageStatic*		m_pLeftTitleBar;
	_XImageStatic*		m_pCenterTitleBar;
	_XImageStatic*		m_pCenter2TitleBar;
	_XImageStatic*		m_pRightTitleBar;

	int					m_BorderAlphaLevel;
	
#ifdef _XNEXON_WEBLOGIN	
	_XButton			m_ExitButton;
#endif

public:
	_XWindow_ServerStatus();
	virtual ~_XWindow_ServerStatus();
	
	BOOL				Initialize(_XTextureManager& texturearchive);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				MoveWindow(int X, int Y);

	void				ShowWindow( BOOL show );
};

#endif // !defined(AFX_KUXWINDOW_LOGIN_H__B8E7C427_1539_4B3E_AE0E_BF41DA510DA7__INCLUDED_)
