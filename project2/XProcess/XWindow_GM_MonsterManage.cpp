// XWindow_GM_MonsterManage.cpp: implementation of the XWindow_GM_MonsterManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_GM_MonsterManage.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_GM_MonsterManage::_XWindow_GM_MonsterManage()
{

}

_XWindow_GM_MonsterManage::~_XWindow_GM_MonsterManage()
{

}


BOOL _XWindow_GM_MonsterManage::Initialize( void )
{
	// Title---------------------------------------------------------------------------------------=
	int titleimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 177, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleLeftBar->SetClipRect( 0, 0, 177, 24 );
	InsertChildObject( pTitleLeftBar );
	
	_XImageStatic* pTitleCentertBar = new _XImageStatic;
	pTitleCentertBar->Create( 176, 0, 177+175, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleCentertBar->SetClipRect( 79, 0, 177, 24 );
	InsertChildObject( pTitleCentertBar );
	
	_XImageStatic* pTitleRightBar = new _XImageStatic; 
	pTitleRightBar->Create( 451, 0, 451+177, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleRightBar->SetClipRect( 0, 0, 177, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );


	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 430, 5 }, { 12, 12 }, _XDEF_GMCONTROLMONSTERWINDOWCLOSEBTN,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
			
	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );

	_XBTN_STRUCTURE menubtnstruct = { TRUE, { 10, 51 }, { 111, 26 },
		_XDEF_GMMONSTERSPAWNBUTTON,
		characterwindowimageindex,characterwindowimageindex,characterwindowimageindex, 
		&g_MainInterfaceTextureArchive };
		
	_XButton* pMenubutton = new _XButton;
	pMenubutton->Create( menubtnstruct );
	pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 105, 112, 131 );
	pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 105, 112, 131 );
	pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 131, 112, 157 );		
	pMenubutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_GMCOMMAND_2479);
	pMenubutton->SetGrowTextMode( TRUE );
	pMenubutton->SetButtonTextColor( D3DCOLOR_ARGB(255,180,180,180) );
	InsertChildObject( pMenubutton );

	m_SpawnCountIMEControl.ResetContainer();
	m_SpawnCountIMEControl.ResizeWindow(40, 12);
	m_SpawnCountIMEControl.MoveWindow( m_WindowPosition.x + 80 , m_WindowPosition.y + 34 );	
	m_SpawnCountIMEControl.ShowWindow(TRUE);
	m_SpawnCountIMEControl.ShowWindowRectAngle( TRUE );
	m_SpawnCountIMEControl.SetMaxLength(4);
	m_SpawnCountIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_SpawnCountIMEControl.SetHorizScrollMode( FALSE );		
	//m_SpawnCountIMEControl.SetTabKeyCallback( _XIMECallback_GMCoordControlTabKey );	
	//m_SpawnCountIMEControl.SetEscapeKeyCallback( _XIMECallback_GMCoordControlEscapeKey );

	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_chat_quick.tga");
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								// 윈도우 활성화
		{ 170, 34 },							// 윈도우 좌표
		{ m_WindowSize.cx- 182, m_WindowSize.cy - 48 },						// 윈도우 사이즈
		_XDEF_GMMONSTERSPAWN_MOBLISTBOX,	// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		14,									// 리스트 아이템 라인 피치
		8,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,			// 스크롤바의 위치
		
		// Scroll bar property
		{12, m_WindowSize.cy - 52},
		12,
		22,
		150,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex
	};
	
	m_pMonsterListBox = new _XListBox;
	m_pMonsterListBox->Create(listboxstructure);
	
	POINT coloffset = { 2, 2 };
	m_pMonsterListBox->SetColumnOffset(0, coloffset);
	m_pMonsterListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pMonsterListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_pMonsterListBox->SetColumnWidth(0, 110 );
	m_pMonsterListBox->SetTrackHeight(14);
	m_pMonsterListBox->SetTrackBarColor( D3DCOLOR_ARGB(64,128,128,128) );
	m_pMonsterListBox->EnableSelectItem(TRUE);
	m_pMonsterListBox->SetAlwaysShowScrollBarMode( FALSE );
	m_pMonsterListBox->SetDrawBorder( TRUE );
	
	_XVScrollBar* pScrollBar = m_pMonsterListBox->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	InsertChildObject(m_pMonsterListBox);	

	return TRUE;
}

void _XWindow_GM_MonsterManage::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_GM_MonsterManage::Draw( _XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// Draw Border, Line ---------------------------------------------------------------------=	
	_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, D3DCOLOR_ARGB(128, 0, 0, 0) );
	
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+24, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f,0xFF9F9A84 );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+25, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f,0xFF040000 );
	_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f,0xFF1A211A );

	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+28, m_WindowPosition.x+m_WindowSize.cx-9, m_WindowPosition.y+m_WindowSize.cy-6, 0.0f,0xFF000000 );

	_XDrawRectAngle( m_pMonsterListBox->GetWindowPos().x-1, m_pMonsterListBox->GetWindowPos().y  -1, 
					 m_pMonsterListBox->GetWindowPos().x + m_pMonsterListBox->GetWindowSize().cx +1, 
					 m_pMonsterListBox->GetWindowPos().y + m_pMonsterListBox->GetWindowSize().cy +5, 0.0f,0xFF000000 );
	
	_XWindow::Draw(pfocusobject);
		
	// Title Name
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_MONSTER) );	
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+34 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2072) );
	g_XBaseFont->Flush();

	m_SpawnCountIMEControl.Draw();	
}

BOOL _XWindow_GM_MonsterManage::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckMousePosition() )
				{
					if( m_SpawnCountIMEControl.CheckMousePosition() )
						m_SpawnCountIMEControl.SetFocus();
				}
			}
		}
	}
		
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	

void _XWindow_GM_MonsterManage::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );

	m_SpawnCountIMEControl.MoveWindow(m_WindowPosition.x + 80 , m_WindowPosition.y + 34);
}

void _XWindow_GM_MonsterManage::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow(show);

	if( show )
	{
		m_SpawnCountIMEControl.SetFocus();
		m_pMyManager->SetTopWindow( this );
	}
	else
	{
		_XIMEKERNEL.SetFocus( NULL );
	}
}


void _XWindow_GM_MonsterManage::UpdateMonsterList( void )

{
	m_pMonsterListBox->DeleteAllItem();

	for( int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
	{	
#ifdef _XDEF_NEWSTRINGMANAGER
		if( g_MonsterName[ i ] )		
#endif
		{
			if( strlen( g_MonsterName[ i ] ) > 0 )
			{
				m_pMonsterListBox->InsertListItem( g_MonsterName[ i ], _XSC_DEFAULT, i, NULL );
				m_pMonsterListBox->SetItemAttribHighlight( m_pMonsterListBox->GetItemCount()-1, 0, _XSC_DEFAULT_HIGHLIGHT );
			}		
		}
	}

	m_pMonsterListBox->SelectItem(0);

	m_SelectedMonsterType = 0;
}