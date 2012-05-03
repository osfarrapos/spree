// XWindow_DrawKeyboard.cpp: implementation of the _XWindow_DrawKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Login.h"
#include "XWindow_DrawKeyboard.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 영역 셋팅
RECT g_KeyArea[64] = 
{
	{   1,   1,  21,  21 },		// Line 1   14
	{  24,   1,  44,  21 },
	{  47,   1,  67,  21 },
	{  70,   1,  90,  21 },
	{  93,   1, 113,  21 },
	{ 116,   1, 136,  21 },
	{ 139,   1, 159,  21 },
	{ 162,   1, 182,  21 },
	{ 185,   1, 205,  21 },
	{ 208,   1, 228,  21 },
	{ 231,   1, 251,  21 },
	{ 254,   1, 274,  21 },
	{ 277,   1, 297,  21 },
	{ 300,   1, 320,  21 },	
			
	{  36,  24,  56,  44 },		// Line 2	12
	{  59,  24,  79,  44 },
	{  82,  24, 102,  44 },
	{ 105,  24, 125,  44 },
	{ 128,  24, 148,  44 },
	{ 151,  24, 171,  44 },
	{ 174,  24, 194,  44 },
	{ 197,  24, 217,  44 },
	{ 220,  24, 240,  44 },
	{ 243,  24, 263,  44 },
	{ 266,  24, 286,  44 },
	{ 289,  24, 309,  44 },
			
	{  47,  47,  67,  67 },		// Line 3	11
	{  70,  47,  90,  67 },
	{  93,  47, 113,  67 },
	{ 116,  47, 136,  67 },
	{ 139,  47, 159,  67 },
	{ 162,  47, 182,  67 },
	{ 185,  47, 205,  67 },
	{ 208,  47, 228,  67 },
	{ 231,  47, 251,  67 },
	{ 254,  47, 274,  67 },
	{ 277,  47, 297,  67 },
	
	{  59,  70,  79,  90 },		// Line 4	10
	{  82,  70, 102,  90 },
	{ 105,  70, 125,  90 },
	{ 128,  70, 148,  90 },
	{ 151,  70, 171,  90 },
	{ 174,  70, 194,  90 },
	{ 197,  70, 217,  90 },
	{ 220,  70, 240,  90 },
	{ 243,  70, 263,  90 },
	{ 266,  70, 286,  90 },
	{ 109,  93, 212,  113 },	// space	

	{ 323,   1, 343,  21 },		// backspace	index = 48
	{   1,  24,  33,  44 },		// tab
	{ 300,  47, 343,  67 },		// Enter
	{   1,  47,  44,  67 },		// capslock	

	{ 312,  24, 343,  44 },		// temp key		index = 52;
	{   1,  70,  56,  90 },		// l-shift
	{ 289,  70, 343,  90 },		// r-shift				
	{   1,  93,  29,  113 },	// l-ctrl		
	{  32,  93,  52,  113 },	// temp key
	{  55,  93,  83,  113 },	// l-alt
	{  86,  93, 106,  113 },	// temp  key	
	{ 215,  93, 235,  113 },	// temp key
	{ 238,  93, 266,  113 },	// r-alt
	{ 269,  93, 289,  113 },	// temp key
	{ 292,  93, 312,  113 },	// temp key
	{ 315,  93, 343,  113 },	// r-ctrl
};

TCHAR g_AlphaBatString[128] = "`1234567890-=\\qwertyuiop[]asdfghjkl;'zxcvbnm,./ \
~!@#$%^&*()_+|QWERTYUIOP{}ASDFGHJKL:\"ZXCVBNM<>? ";

_XWindow_DrawKeyboard::_XWindow_DrawKeyboard()
{
	m_ShiftDownValue			= 0;
	m_CurrentOverAreaIndex		= -1;
	m_OverKeyAlphaLevel			= 15;
	m_OverKeyAlphaLevelInterval = 15;
	m_ClickedMouseButton		= FALSE;
	m_TempClickedindex			= -1;
	m_LoginProcessMode			= TRUE;
}

_XWindow_DrawKeyboard::~_XWindow_DrawKeyboard()
{

}

BOOL _XWindow_DrawKeyboard::Initialize( _XTextureManager& texturearchive )
{
	_XWindow::SetDrawBorder( FALSE );
	
	int imageindex = texturearchive.FindResource("MI_keyboard.tga");

	_XImageStatic* pLeftBorder = new _XImageStatic;
	pLeftBorder->Create( 0, 0, 256, 215, &texturearchive, imageindex );
	pLeftBorder->SetClipRect( 0, 0, 256, 114 );
	InsertChildObject( pLeftBorder );

	_XImageStatic* pRightBorder = new _XImageStatic;
	pRightBorder->Create( 256, 0, 344, 215, &texturearchive, imageindex );
	pRightBorder->SetClipRect( 0, 115, 88, 229 );
	InsertChildObject( pRightBorder );

	return TRUE;
}

void _XWindow_DrawKeyboard::DestroyWindow( void )
{

}

void _XWindow_DrawKeyboard::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw( pfocusobject );

	if( this->m_ShowWindow )
	{
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF2E2E2E);

		TCHAR ch[2];		
		ch[1] = NULL;
		for( int i = 0; i < 48; i++ )
		{		
			ch[0] = g_AlphaBatString[m_ShiftDownValue+i];
			
			g_XBaseFont->PutsAlign( m_WindowPosition.x+g_KeyArea[i].left+10, m_WindowPosition.y+g_KeyArea[i].top+5, 
				_XFONT_ALIGNTYPE_CENTER, ch );
		}				
		g_XBaseFont->Flush();		
		g_XBaseFont->DisableGrowMode();

		if( m_CurrentOverAreaIndex > -1 )		// 오버된 키 테두리 그리기 -> 베트남 Mr. Quang이 빼달라고 했음...
		{		
			/*
			if( m_OverKeyAlphaLevel > 240 || m_OverKeyAlphaLevel < 15 )
				m_OverKeyAlphaLevelInterval *= -1;
			m_OverKeyAlphaLevel += m_OverKeyAlphaLevelInterval;

			D3DCOLOR color = ((DWORD)m_OverKeyAlphaLevel << 24);
			if( m_CurrentOverAreaIndex < 52 )
				color |= 0x4CF61E;
			else
				color |= 0xC93C26;

			_XDrawRectAngle( m_WindowPosition.x+g_KeyArea[m_CurrentOverAreaIndex].left-2, 
							 m_WindowPosition.y+g_KeyArea[m_CurrentOverAreaIndex].top-2,
							 m_WindowPosition.x+g_KeyArea[m_CurrentOverAreaIndex].right+1, 
							 m_WindowPosition.y+g_KeyArea[m_CurrentOverAreaIndex].bottom+1, 0.0f, color );
			*/
		}
	}
}

BOOL _XWindow_DrawKeyboard::Process(_XGUIObject*& pfocusobject)
{
	if( !this->m_ShowWindow ) return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if( mousestate )
	{
		if(CheckMousePosition())
		{
			for( int i = 0; i < 64; i++ )
			{
				if( scrnpos->x > m_WindowPosition.x+g_KeyArea[i].left && scrnpos->x < m_WindowPosition.x+g_KeyArea[i].right &&
					scrnpos->z > m_WindowPosition.y+g_KeyArea[i].top && scrnpos->z < m_WindowPosition.y+g_KeyArea[i].bottom )
				{
					m_CurrentOverAreaIndex = i;
				}
			}

			if( !m_ClickedMouseButton && mousestate->bButton[0] )
			{
				if( m_CurrentOverAreaIndex > -1 )
				{
					m_ClickedMouseButton = TRUE;
					m_TempClickedindex = m_CurrentOverAreaIndex;
				}
				else
				{
					m_ClickedMouseButton = FALSE;
					m_TempClickedindex = -1;
				}
			}

			if( m_ClickedMouseButton )	// 누른 상태에서 
			{
				if( !mousestate->bButton[0] )	// 버튼 업 됐을 때
				{
					if( m_CurrentOverAreaIndex == m_TempClickedindex )
					{
						KeyProcess();
					}
					else
					{
						m_ClickedMouseButton = FALSE;
						m_TempClickedindex = -1;
					}
				}
			}
		}
		else
			m_CurrentOverAreaIndex = -1;
	}
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

void _XWindow_DrawKeyboard::KeyProcess(void)
{
	if( m_LoginProcessMode )
	{
		_XWindow_Login* pLogin_Window = 
			(_XWindow_Login *)((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_WindowManager.FindWindow(_XDEF_WTITLE_LOGIN);
		if( pLogin_Window == NULL )	return;

		_XIMEContainer* tempIME = NULL;
		if( pLogin_Window->m_IDEdit.IsFocused() )
			tempIME = &pLogin_Window->m_IDEdit;	
		else if( pLogin_Window->m_PWEdit.IsFocused() )
			tempIME = &pLogin_Window->m_PWEdit;	

		if( tempIME == NULL )	return;

		TCHAR ch[2];		
		ch[1] = NULL;
		
		switch(m_CurrentOverAreaIndex) 
		{
		case 48 :		// backspace key
			{
				tempIME->ProcessBackSpaceKey();			

				if( pLogin_Window->m_PWEdit.IsFocused() )
				{
					if( strlen( pLogin_Window->m_PWEdit.GetText() ) <= 0 )
					{
						pLogin_Window->m_PWEdit.m_VKeyboardTypedCount++;
					}
				}
			}
			break;
		case 49 :		// tab key
			{
				tempIME->ProcessTabKey();
			}
			break;
		case 50 :		// enter key
			{
				if( pLogin_Window->m_IDEdit.IsFocused() )
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_IDEDITENTER, 0), (LPARAM)gHWnd);				
				else if( pLogin_Window->m_PWEdit.IsFocused() )
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_PWEDITENTER, 0), (LPARAM)gHWnd);
			}
			break;
		case 51 :		// capslock	
			{
				m_ShiftDownValue = (m_ShiftDownValue == 48 ) ? 0 : 48;
			}		
			break;
		default:		// char key
			{
				if( m_CurrentOverAreaIndex < 48 )
				{
					if( gpInput->CheckKeyPress(DIK_LSHIFT) || gpInput->CheckKeyPress(DIK_RSHIFT) )
						ch[0] = g_AlphaBatString[48+m_CurrentOverAreaIndex];
					else
						ch[0] = g_AlphaBatString[m_ShiftDownValue+m_CurrentOverAreaIndex];
					tempIME->AddString( ch, strlen(ch) );

					if( pLogin_Window->m_PWEdit.IsFocused() )
					{
						pLogin_Window->m_PWEdit.m_VKeyboardTypedCount++;
					}
				}
			}
			break;
		}
		m_TempClickedindex = -1;
	}
	else
	{
		TCHAR ch[2];		
		ch[1] = NULL;
		
		switch(m_CurrentOverAreaIndex) 
		{
		case 48 :		// backspace key
			{
				g_MessageBox.m_IMEControl.ProcessBackSpaceKey();			
				
				if( strlen( g_MessageBox.m_IMEControl.GetText() ) <= 0 )
				{
					g_MessageBox.m_IMEControl.m_VKeyboardTypedCount++;
				}
			}
			break;
		case 49 :		// tab key
			{
				g_MessageBox.m_IMEControl.ProcessTabKey();
			}
			break;
		case 50 :		// enter key
			{
				if( g_MessageBox.m_IMEControl.IsFocused() )
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_PWEDITENTER, 0), (LPARAM)gHWnd);
			}
			break;
		case 51 :		// capslock	
			{
				m_ShiftDownValue = (m_ShiftDownValue == 48 ) ? 0 : 48;
			}		
			break;
		default:		// char key
			{
				if( m_CurrentOverAreaIndex < 48 )
				{
					if( gpInput->CheckKeyPress(DIK_LSHIFT) || gpInput->CheckKeyPress(DIK_RSHIFT) )
						ch[0] = g_AlphaBatString[48+m_CurrentOverAreaIndex];
					else
						ch[0] = g_AlphaBatString[m_ShiftDownValue+m_CurrentOverAreaIndex];
					g_MessageBox.m_IMEControl.AddString( ch, strlen(ch) );
					
					if( g_MessageBox.m_IMEControl.IsFocused() )
					{
						g_MessageBox.m_IMEControl.m_VKeyboardTypedCount++;
					}
				}
			}
			break;
		}
		m_TempClickedindex = -1;
	}
}