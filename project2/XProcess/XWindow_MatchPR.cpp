// XWindow_MatchPR.cpp: implementation of the _XWindow_MatchPR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_MatchPR.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchPR::_XWindow_MatchPR()
{
	m_btnOpen			= NULL;
	memset(m_stringPVPPR, 0, sizeof(TCHAR)*81);

}

_XWindow_MatchPR::~_XWindow_MatchPR()
{

}

BOOL _XWindow_MatchPR::Initialize()
{
	int mainframeinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int charinfointerface			= g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int messengerresourceindex1		= g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int	skillwindowindex			= g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga");

	int	pvpindex2					= -1;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "vn_MI_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "us_MI_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "tw_MI_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "rs_MI_pvp_02.tga");
	}
	else
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "MI_pvp_02.tga");
	}

	
	// 비무 홍보창 타이틀 바 왼쪽 생성 ( 77 X 18 )
	_XImageStatic* pTitleBar_Left = new _XImageStatic;
	pTitleBar_Left->Create( 0, 0, 77, 18, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pTitleBar_Left->SetClipRect( 0, 125, 77, 143 );
	InsertChildObject( pTitleBar_Left );
	
	// 비무 홍보창 타이틀 바 중간 생성 ( 206 X 18 )
	_XImageStatic* pTitleBar_Middle = new _XImageStatic;
	pTitleBar_Middle->Create( 77, 0, 283, 18, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pTitleBar_Middle->SetClipRect( 77, 125, 180, 143 );
	pTitleBar_Middle->SetScale(2.0f, 1.0f);
	InsertChildObject( pTitleBar_Middle );
	
	// 비무 홍보창 타이틀 바 오른쪽 생성 ( 6 X 18 )
	_XImageStatic* pTitleBar_Right = new _XImageStatic;
	pTitleBar_Right->Create( 282, 0, 288, 18, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pTitleBar_Right->SetClipRect( 177, 125, 183, 143 );
	InsertChildObject( pTitleBar_Right );
	
	// 개인 거래창 왼쪽 라인1 생성 ( 3 X 51 )
	_XImageStatic* pLeftBar1 = new _XImageStatic;
	pLeftBar1->Create( 0, 18, 3, 69, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pLeftBar1->SetClipRect( 0, 143, 3, 194 );
	InsertChildObject( pLeftBar1 );
	
	// 개인 거래창 왼쪽 라인2 생성 ( 3 X 13 )
	_XImageStatic* pLeftBar2 = new _XImageStatic;
	pLeftBar2->Create( 0, 69, 3, 82 , &g_MainInterfaceTextureArchive, mainframeinterface );	
	pLeftBar2->SetClipRect( 0, 216, 3, 230 );
	InsertChildObject( pLeftBar2 );
	
	// 개인 거래창 오른쪽 라인1 생성 ( 3 X 51 )
	_XImageStatic* pRightBar1 = new _XImageStatic;
	pRightBar1->Create( 285, 18, 288, 69, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pRightBar1->SetClipRect( 0, 143, 3, 194 );
	InsertChildObject( pRightBar1 );
	
	// 개인 거래창 오른쪽 라인2 생성 ( 3 X 13 )
	_XImageStatic* pRightBar2 = new _XImageStatic;
	pRightBar2->Create( 285, 69, 288, 82 , &g_MainInterfaceTextureArchive, mainframeinterface );	
	pRightBar2->SetClipRect( 180, 216, 183, 230 );
	InsertChildObject( pRightBar2 );
	
	// 개인 거래창 아래쪽 라인 생성 ( 177 X 3 )
	_XImageStatic* pDownLine = new _XImageStatic;
	pDownLine->Create( 3, 80, 285, 82 , &g_MainInterfaceTextureArchive, mainframeinterface );	
	pDownLine->SetClipRect( 3, 227, 180, 230 );
	pDownLine->SetScale(1.5932f, 1.0f);
	InsertChildObject( pDownLine );
	
	// 비무 마크 ( 30 X 30 )
	_XImageStatic* pPVPMark = new _XImageStatic;
	pPVPMark->Create( 6, 21, 36, 51, &g_MainInterfaceTextureArchive, pvpindex2 );	
	pPVPMark->SetClipRect( 225, 33, 255, 63 );
	InsertChildObject( pPVPMark );
	
	// X 버튼 (닫기 버튼)
	_XBTN_STRUCTURE xbtnstruct = { TRUE, { 271, 2 }, { 12, 12 }, _XDEF_MATCHPR_CLOSEBUTTON,	charinfointerface,charinfointerface,charinfointerface, &g_MainInterfaceTextureArchive };
	_XButton* pXbutton = new _XButton;
	pXbutton->Create( xbtnstruct );		
	pXbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pXbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pXbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pXbutton );
	
	// 도움 버튼 
	_XBTN_STRUCTURE helpstruct = { TRUE, { 258, 2 }, { 12, 12 }, _XDEF_MATCHPR_HELPBUTTON, charinfointerface,charinfointerface,charinfointerface, &g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );		
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
		
	// 비무홍보 열기 버튼
	_XBTN_STRUCTURE openbtnstruct = {TRUE, {57, 53}, {108, 24}, _XDEF_MATCHPR_OPENBUTTON, skillwindowindex, skillwindowindex, skillwindowindex, &g_MainInterfaceTextureArchive};
	m_btnOpen = new _XButton;
	m_btnOpen->Create(openbtnstruct);
	m_btnOpen->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 176);
	m_btnOpen->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
	m_btnOpen->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	m_btnOpen->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1937);
	m_btnOpen->EnableWindow(TRUE);
	InsertChildObject(m_btnOpen);
	
	// 닫기 버튼 ( 60 X 24 )
	_XBTN_STRUCTURE closebtnstruct = {TRUE, {181, 53}, {60, 24}, _XDEF_MATCHPR_CLOSEBUTTON, messengerresourceindex1, messengerresourceindex1, messengerresourceindex1, &g_MainInterfaceTextureArchive	};
	_XButton* pbtnClose = new _XButton;
	pbtnClose->Create(closebtnstruct);
	pbtnClose->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	pbtnClose->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	pbtnClose->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	pbtnClose->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_CLOSE);
	pbtnClose->ShowWindow(TRUE);
	InsertChildObject(pbtnClose);
	
	// 거래창 바탕 타일 생성
	m_BorderTile.Create( m_WindowPosition.x, m_WindowPosition.y + 18, m_WindowPosition.x + 285, m_WindowPosition.y + 81);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	FLOAT ufactor = 285.0f/128.0f;
	FLOAT vfactor = 62.0f/128.0f;
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );

	
	m_PVPIMEControl.ResetContainer();
	m_PVPIMEControl.MoveWindow(m_WindowPosition.x + 41, m_WindowPosition.y + 28);
	m_PVPIMEControl.ResizeWindow(223, 15);
	m_PVPIMEControl.SetMaxLength(80);	
	m_PVPIMEControl.ShowWindow(FALSE);
	m_PVPIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );

	
	return TRUE;
}
void _XWindow_MatchPR::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));	
		
		// 노점 이름 표시 ( 226 X 19)
		_XDrawSolidBar( m_WindowPosition.x+41,  m_WindowPosition.y+23, m_WindowPosition.x+281, m_WindowPosition.y+42 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+40,  m_WindowPosition.y+22, m_WindowPosition.x+281, m_WindowPosition.y+42 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );		
	}
	
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;
	
	// TODO
	
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFFFFFFF);
	g_XBaseFont->Puts( m_WindowPosition.x+124,  m_WindowPosition.y+4, _XGETINTERFACETEXT(ID_STRING_MATCH_2189) );
	g_XBaseFont->Puts( m_WindowPosition.x+125,  m_WindowPosition.y+4, _XGETINTERFACETEXT(ID_STRING_MATCH_2189) );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	if( !m_PVPIMEControl.GetFocus() )
	{
		if( m_stringPVPPR[0] != 0 )
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 210, 210, 210 ) );
			g_XBaseFont->Puts( m_WindowPosition.x+42,  m_WindowPosition.y+28, m_stringPVPPR);
			g_XBaseFont->Flush();
		}
	}
	else
	{
		m_PVPIMEControl.Draw();	
	}
}

BOOL _XWindow_MatchPR::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;
	
	// TODO
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			// 왼쪽 버튼 클릭 시 처리 
			if(mousestate->bButton[0])
			{
				// left down
				if(CheckMousePosition())
				{
					if( CheckPVPIMEBoxArea() )
					{						
						//memset(m_stringPVPPR, 0, sizeof(TCHAR)*81);
						m_PVPIMEControl.ClearBuffer();
						_XIMEKERNEL.ResetCombinationBuffer();
						m_PVPIMEControl.AddString( m_stringPVPPR, strlen(m_stringPVPPR) );
						m_PVPIMEControl.SetFocus();
					}
				}
			}
		}
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}
void _XWindow_MatchPR::MoveWindow(int X, int Y)
{	
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow( m_WindowPosition.x , m_WindowPosition.y + 18, m_WindowPosition.x + 285, m_WindowPosition.y + 81 );	
	m_PVPIMEControl.MoveWindow(m_WindowPosition.x + 42, m_WindowPosition.y + 28);
}

void _XWindow_MatchPR::ShowWindow(BOOL show)
{
	if( show )
	{
		m_PVPIMEControl.ShowWindow(TRUE);
		m_PVPIMEControl.SetFocus();
	}
	else
	{
		m_PVPIMEControl.ClearBuffer();
		_XIMEKERNEL.SetFocus( NULL ); 	
		m_PVPIMEControl.ShowWindow(FALSE);
	}
	_XWindow::ShowWindow(show);
}
BOOL _XWindow_MatchPR::CheckPVPIMEBoxArea(void)
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if(scrnpos->x > (m_WindowPosition.x+41) &&
		scrnpos->x < (m_WindowPosition.x + 279 ) &&
		scrnpos->z > (m_WindowPosition.y + 23) &&
		scrnpos->z < (m_WindowPosition.y + 42))
		return TRUE;
	
	return FALSE;
}