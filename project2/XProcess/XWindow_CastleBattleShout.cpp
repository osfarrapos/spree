// XWindow_CastleBattleShout.cpp: implementation of the _XWindow_CastleBattleShout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_CastleBattleShout.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_IMECastleBattleShoutReturn( _XIMEContainer* pIMEContainer )
{
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CASTLEBATTLE_SHOUT_BTN_SHOUT, 0), (LPARAM)gHWnd); 
}

_XWindow_CastleBattleShout::_XWindow_CastleBattleShout()
{
	m_nTotalCount = 0; //사자후를 사용할 수 있는 최대개수
	m_nCurrentCount = 0; // 사용한 사자후 개수

}

_XWindow_CastleBattleShout::~_XWindow_CastleBattleShout()
{

}

BOOL _XWindow_CastleBattleShout::Initialize(void)
{
	int mainframeresourceindex	= g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	int charresourceindex		= g_MainInterfaceTextureArchive.FindResource("MI_Characterwin.tga");
	int charinforesourceindex	= g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	
	// 총 크기  390 X 110
	
	// 타이틀 바1 ( 80 X 18 )
	_XImageStatic* pTitleBar1 = new _XImageStatic;
	pTitleBar1->Create(0, 0, 80, 18, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pTitleBar1->SetClipRect(0, 125, 80, 143);	
	InsertChildObject( pTitleBar1);	
	
	// 타이틀 바2 ( 100 X 18 ) : 305
	_XImageStatic* pTitleBar2 = new _XImageStatic;
	pTitleBar2->Create(80, 0, 80+305, 18, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pTitleBar2->SetClipRect(80, 125, 180, 143);	
	pTitleBar2->SetScale( 3.05f, 1.0f );
	InsertChildObject( pTitleBar2);	
	
	// 타이틀 바3 ( 5 X 18 )
	_XImageStatic* pTitleBar3 = new _XImageStatic;
	pTitleBar3->Create(80+305, 0, 390, 18, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pTitleBar3->SetClipRect(178, 125, 183, 143);	
	InsertChildObject( pTitleBar3 );	
	
	// 왼쪽 라인 ( 3 X 89 )
	_XImageStatic* pLeftBorder = new _XImageStatic;
	pLeftBorder->Create(0, 18, 3, 18+89, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pLeftBorder->SetClipRect(0, 195, 3, 227);
	pLeftBorder->SetScale(1.0f, 2.78125f);
	InsertChildObject(pLeftBorder);
	
	// 오른쪽 라인 ( 3 X 89 )
	_XImageStatic* pRightBorder = new _XImageStatic;
	pRightBorder->Create(387, 18, 390, 18+89, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pRightBorder->SetClipRect(0, 195, 3, 227);
	pRightBorder->SetScale(1.0f, 2.78125f);
	InsertChildObject(pRightBorder);
	
	// 바닥 라인 1 ( 3 X 3 )
	_XImageStatic* pBottomLine1 = new _XImageStatic;
	pBottomLine1->Create(0, 18+89, 3, 110, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pBottomLine1->SetClipRect(0, 227, 3, 230);
	InsertChildObject(pBottomLine1);
	
	// 바닥 라인 2 ( 384 X 3 )
	_XImageStatic* pBottomLine2 = new _XImageStatic;
	pBottomLine2->Create(3, 18+89, 387, 110, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pBottomLine2->SetClipRect(3, 227, 180, 230);
	pBottomLine2->SetScale(2.1694915254f, 1.0f);
	InsertChildObject(pBottomLine2);
	
	// 바닥 라인 3 ( 3 X 3 )
	_XImageStatic* pBottomLine3 = new _XImageStatic;
	pBottomLine3->Create(387, 18+89, 390, 110, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pBottomLine3->SetClipRect(180, 227, 183, 230);
	InsertChildObject(pBottomLine3);
	
	
	// IME 라인 1 ( 2 X 22 )
	_XImageStatic* pIMELine1 = new _XImageStatic;
	pIMELine1->Create(12, 48, 14, 70, &g_MainInterfaceTextureArchive, charresourceindex);
	pIMELine1->SetClipRect(0, 0, 2, 22);
	InsertChildObject(pIMELine1);
		
	// IME 라인 2 ( 162 X 22 )
	_XImageStatic* pIMELine2 = new _XImageStatic;
	pIMELine2->Create(14, 48, 378, 70, &g_MainInterfaceTextureArchive, charresourceindex);
	pIMELine2->SetClipRect(2, 0, 160, 22);
	pIMELine2->SetScale(2.303797468f, 1.0f);
	InsertChildObject(pIMELine2);
		
	// IME 라인 3 ( 162 X 22 )
	_XImageStatic* pIMELine3 = new _XImageStatic;
	pIMELine3->Create(378, 48, 380, 70, &g_MainInterfaceTextureArchive, charresourceindex);
	pIMELine3->SetClipRect(160, 0, 162, 22);
	InsertChildObject(pIMELine3);
	

	// 장원전 사자후 닫기 버튼  ( 12 X 12 )
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 373, 2 }, { 12, 12 }, _XDEF_CASTLEBATTLE_SHOUT_BTN_CLOSE,
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 	&g_MainInterfaceTextureArchive };
	_XButton* pBtnClose = new _XButton;
	pBtnClose->Create( closebtnstruct );	
	
	pBtnClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pBtnClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pBtnClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pBtnClose );
	
	// 외치기 버튼 ( 106 X 22 )
	_XBTN_STRUCTURE shoutbtnstruct = { TRUE, { 142, 82 }, { 106, 22 }, _XDEF_CASTLEBATTLE_SHOUT_BTN_SHOUT,
		charresourceindex,charresourceindex,charresourceindex, &g_MainInterfaceTextureArchive };
	m_pBtnShout	= new _XButton;
	m_pBtnShout->Create( shoutbtnstruct );
	m_pBtnShout->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 4, 107, 110, 129 );
	m_pBtnShout->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  4, 107, 110, 129 );
	m_pBtnShout->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  4, 133, 110, 155 );
	m_pBtnShout->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3249);		//_T("외 치 기")
	m_pBtnShout->SetButtonTextColor( 0xFFE0D7BA );
	InsertChildObject( m_pBtnShout );	

	// 장원전 사자후 바탕 타일 생성
	m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 390, m_WindowPosition.y + 110);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	FLOAT ufactor = 390.0f/128.0f;
	FLOAT vfactor = 92.0f/128.0f;
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );
	
	// IME 생성 ( 364 X 18 )
	m_IMECastleBattleShout.ResetContainer();
	m_IMECastleBattleShout.MoveWindow(m_WindowPosition.x + 15, m_WindowPosition.y + 53);
	m_IMECastleBattleShout.ResizeWindow(364-12-12, 18);
	m_IMECastleBattleShout.ShowWindow(FALSE);
	m_IMECastleBattleShout.SetMaxLength(60);	
	m_IMECastleBattleShout.SetAlignType( _XFONT_ALIGNTYPE_LEFT );

	m_IMECastleBattleShout.SetReturnKeyCallback( _XIMECallback_IMECastleBattleShoutReturn );

	
	return TRUE;
}

void _XWindow_CastleBattleShout::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_CastleBattleShout::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
		
		_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+24, m_WindowPosition.x+383, m_WindowPosition.y+75 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+24, m_WindowPosition.x+383, m_WindowPosition.y+75 , 0.0f, 0xFF000000 );
	}
	
	_XWindow::Draw(pfocusobject);
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XBaseFont->PrintAlign( m_WindowPosition.x+195, m_WindowPosition.y+3, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3250));	//_T("장원전 사자후")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 224, 150, 45));
	g_XBaseFont->PrintAlign( m_WindowPosition.x+11, m_WindowPosition.y+30, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3250));	//_T("장원전 사자후")
	
	g_XBaseFont->SetColor( _XSC_DEFAULT );	
	g_XBaseFont->PrintAlign( m_WindowPosition.x+378, m_WindowPosition.y+30, 1.0f, _XFONT_ALIGNTYPE_RIGHT,
		_XGETINTERFACETEXT(ID_STRING_NEW_3251),m_nCurrentCount, g_CBManager.GetShoutMaxCount() );	//%d/%d 회
	
	g_XBaseFont->Flush();

	m_IMECastleBattleShout.Draw();
}

BOOL _XWindow_CastleBattleShout::Process(_XGUIObject*& pfocusobject)
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
				if( m_IMECastleBattleShout.CheckMousePosition() ) 
				{
					m_IMECastleBattleShout.SetFocus();
				}
				else
				{
					
//					m_IMECastleBattleShout.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL );
				}
			}
		}
	}
	

	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_CastleBattleShout::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 390, m_WindowPosition.y + 110 );
	m_IMECastleBattleShout.MoveWindow(m_WindowPosition.x + 15, m_WindowPosition.y + 53);
}

void _XWindow_CastleBattleShout::ShowWindow(BOOL show)
{	
	if( show )
	{
		m_IMECastleBattleShout.ShowWindow(TRUE);
		m_nCurrentCount = 0;
	}
	else
	{
		m_IMECastleBattleShout.ClearBuffer();
		_XIMEKERNEL.SetFocus( NULL );
		m_IMECastleBattleShout.ShowWindow(FALSE);
	}
	_XWindow::ShowWindow(show);
}