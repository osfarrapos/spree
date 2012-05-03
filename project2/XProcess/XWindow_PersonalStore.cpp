// XWindow_PersonalStore.cpp: implementation of the _XWindow_PersonalStore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_TradeConfirm.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"

#include "SoundEffectList.h"
#include "XWindowObjectDefine.h"

extern unsigned long g_MoneyColor[6];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_PersonalStoreNameReturn( _XIMEContainer* pIMEContainer )
{
	TCHAR resultstring[256]; 
	memset( resultstring, 0, sizeof(TCHAR)*256 );
	strcpy( resultstring, pIMEContainer->GetText());	
#ifdef _XDEF_NEWVERSONFILETERING
	g_WordFilteringManager.Filtering( resultstring,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
	g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif
	int length = strlen(resultstring);
	if( length > 0 )
	{
		_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PERSONALSTORE);
		if( pPersonalStore_Window )
		{	
			strcpy( pPersonalStore_Window->m_PersonalStoreName, resultstring );
			if( g_pLocalUser )
				strcpy( g_pLocalUser->m_PersonalStoreName, resultstring );
			pPersonalStore_Window->m_PersonalStoreName[length] = 0;
			pIMEContainer->ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL ); 	
		}
	}
	
}

void __stdcall _XIMECallback_PersonalStorePriceReturn( _XIMEContainer* pIMEContainer )
{
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_PERSONALSTORE_INPUTOKBUTTON, 0), (LPARAM)gHWnd); 
}

_XWindow_PersonalStore::_XWindow_PersonalStore()
{
	ResetTradeInfo();
}

_XWindow_PersonalStore::~_XWindow_PersonalStore()
{

}

void _XWindow_PersonalStore::ResetTradeInfo(void)
{
	m_ServerPersonalStoreID			= -1;
	m_OwnerUniqueID					= -1;		
	m_TotalCountTradeItem			= 0;
	m_PrevPacketSendedTime			= 0;
	m_Price							= 0;

	for( int i = 0; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER; i++ )
	{
		m_TradeItem[i].ResetTradeItem();
		m_TradeItemPrice[i] = 0;
	}

	m_LastDraggingSlot = -1;
	m_SelectedSlotNumber = -1;
	
	memset(m_OwnerName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
	memset(m_PersonalStoreName, 0, sizeof(TCHAR)*128);
	memset(m_strPrice, 0, sizeof(TCHAR)*64);
			
}

BOOL _XWindow_PersonalStore::Initialize( void )
{
	int mainframeinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int charinfointerface			= g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int npcpresentinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_npc_present.tga" );
	int tradeinterface				= g_MainInterfaceTextureArchive.FindResource( "MI_Trade.tga" );
	int npciconinterface			= g_MainInterfaceTextureArchive.FindResource( "mi_npcname.tga" );
	int characterwininterface		= g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int messengerresourceindex1		= g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	
	// 개인 거래창 타이틀 바 왼쪽 생성 ( 77 X 18 )
	_XImageStatic* pTitleBar_Left = new _XImageStatic;
	pTitleBar_Left->Create( 0, 0, 77, 18, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pTitleBar_Left->SetClipRect( 0, 125, 77, 143 );
	InsertChildObject( pTitleBar_Left );
	
	// 개인 거래창 타이틀 바 중간 생성 ( 206 X 18 )
	_XImageStatic* pTitleBar_Middle = new _XImageStatic;
	pTitleBar_Middle->Create( 77, 0, 283, 18, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pTitleBar_Middle->SetClipRect( 77, 125, 180, 143 );
	pTitleBar_Middle->SetScale(2.0f, 1.0f);
	InsertChildObject( pTitleBar_Middle );
	
	// 개인 거래창 타이틀 바 오른쪽 생성 ( 6 X 18 )
	_XImageStatic* pTitleBar_Right = new _XImageStatic;
	pTitleBar_Right->Create( 282, 0, 288, 18, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pTitleBar_Right->SetClipRect( 177, 125, 183, 143 );
	InsertChildObject( pTitleBar_Right );
	
	// 닫기 버튼 
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 271, 2 }, { 12, 12 }, _XDEF_PERSONALSTORE_CLOSEBUTTON,
		charinfointerface,charinfointerface,charinfointerface, &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
	
	// 도움 버튼 
	_XBTN_STRUCTURE helpstruct = { TRUE, { 258, 2 }, { 12, 12 }, _XDEF_PERSONALSTORE_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, &g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	
	// 개인 노점 아이콘 ( 40 X 40 )
	_XImageStatic* pStoreIcon = new _XImageStatic;
	pStoreIcon->Create( 8, 22, 48, 62, &g_MainInterfaceTextureArchive, npciconinterface );
	pStoreIcon->SetClipRect(  52, 4, 92, 44 );
	InsertChildObject( pStoreIcon );
	
	// 개인 거래창 모델 View 바탕 생성 ( 168 X 147 )
	_XImageStatic* pModelBorder = new _XImageStatic;
	pModelBorder->Create( 6, 65, 174, 212, &g_MainInterfaceTextureArchive, npcpresentinterface );
	pModelBorder->SetClipRect( 0, 0, 168, 147 );
	InsertChildObject( pModelBorder );
	
	// 개인 거래창 물건 리스트 바탕1 생성 ( 100 X 66 )
	_XImageStatic* pTradeListBorder1 = new _XImageStatic;
	pTradeListBorder1->Create( 179, 109, 279, 175, &g_MainInterfaceTextureArchive, tradeinterface );
	pTradeListBorder1->SetClipRect( 9, 26, 109, 92 );
	InsertChildObject( pTradeListBorder1 );
	
	// 개인 거래창 물건 리스트 바탕2 생성 ( 100 X 34 )
	_XImageStatic* pTradeListBorder2 = new _XImageStatic;
	pTradeListBorder2->Create( 179, 175, 279, 209, &g_MainInterfaceTextureArchive, tradeinterface );
	pTradeListBorder2->SetClipRect( 9, 26, 109, 60 );
	InsertChildObject( pTradeListBorder2 );

	// 개인 거래창 판매 가격 바탕 생성 ( 162 X 22 )
	_XImageStatic* pCostBorder = new _XImageStatic;
	pCostBorder->Create( 10, 214, 172, 236, &g_MainInterfaceTextureArchive, characterwininterface );
	pCostBorder->SetClipRect( 0, 0, 162, 22 );
	InsertChildObject( pCostBorder );
	
	// 노점열기 버튼 ( 60 X 24 )
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {91, 240}, {60, 24}, _XDEF_PERSONALSTORE_OPENBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnOpen = new _XButton;
	m_btnOpen->Create(btnstruct);
	m_btnOpen->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnOpen->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnOpen->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_BOOTH_OPEN);		// 노점열기
	m_btnOpen->ShowWindow(TRUE);
	InsertChildObject(m_btnOpen);
	
	// 노점취소 버튼 ( 60 X 24 )
	btnstruct.commandid = _XDEF_PERSONALSTORE_CANCLEBUTTON;
	btnstruct.position.x = 154;
	
	m_btnCancle = new _XButton;
	m_btnCancle->Create(btnstruct);
	m_btnCancle->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnCancle->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnCancle->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_btnCancle->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CLOSE);	// 닫기
	m_btnCancle->ShowWindow(TRUE);
	InsertChildObject(m_btnCancle);
	
	// 입력완료 버튼 ( 16 X 16 )	
	_XBTN_STRUCTURE oKbtnstruct = 
	{
		TRUE, {153, 217}, {16, 16}, _XDEF_PERSONALSTORE_INPUTOKBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	m_btnInputOK = new _XButton;
	m_btnInputOK->Create(oKbtnstruct);
	m_btnInputOK->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 33, 137, 49, 153);
	m_btnInputOK->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  49, 137, 65, 153);
	m_btnInputOK->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  65, 137, 81, 153);
	m_btnInputOK->ShowWindow(FALSE);
	InsertChildObject(m_btnInputOK);
	
	
	m_StoreNameIMEControl.ResetContainer();
	m_StoreNameIMEControl.MoveWindow(m_WindowPosition.x + 59, m_WindowPosition.y + 28);
	m_StoreNameIMEControl.ResizeWindow(223, 15);
	m_StoreNameIMEControl.ShowWindow(FALSE);
	m_StoreNameIMEControl.SetMaxLength(36);	
	m_StoreNameIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_StoreNameIMEControl.SetReturnKeyCallback( _XIMECallback_PersonalStoreNameReturn );
	
	m_TradeItemPriceIMEControl.ResetContainer();
	m_TradeItemPriceIMEControl.MoveWindow(m_WindowPosition.x + 82, m_WindowPosition.y + 220);
	m_TradeItemPriceIMEControl.ResizeWindow(76, 12);
	m_TradeItemPriceIMEControl.ShowWindow(FALSE);
	m_TradeItemPriceIMEControl.SetMaxLength(10);	// 10억단위까지 표시
	m_TradeItemPriceIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_TradeItemPriceIMEControl.SetNumericEditMode( TRUE, TRUE );
	m_TradeItemPriceIMEControl.SetReturnKeyCallback( _XIMECallback_PersonalStorePriceReturn );
	
	// 개인 거래창 왼쪽 라인1 생성 ( 3 X 51 )
	_XImageStatic* pLeftBar1 = new _XImageStatic;
	pLeftBar1->Create( 0, 18, 3, 69, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pLeftBar1->SetClipRect( 0, 143, 3, 194 );
	InsertChildObject( pLeftBar1 );

	// 개인 거래창 왼쪽 라인2 생성 ( 3 X 166 )
	_XImageStatic* pLeftBar2 = new _XImageStatic;
	pLeftBar2->Create( 0, 69, 3, 235, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pLeftBar2->SetClipRect( 184, 64, 187, 230 );
	InsertChildObject( pLeftBar2 );
	
	// 개인 거래창 왼쪽 라인3 생성 ( 3 X 35 )
	_XImageStatic* pLeftBar3 = new _XImageStatic;
	pLeftBar3->Create( 0, 235, 3, 270 , &g_MainInterfaceTextureArchive, mainframeinterface );	
	pLeftBar3->SetClipRect( 0, 195, 3, 230 );
	InsertChildObject( pLeftBar3 );
		
	// 개인 거래창 오른쪽 라인1 생성 ( 3 X 51 )
	_XImageStatic* pRightBar1 = new _XImageStatic;
	pRightBar1->Create( 285, 18, 288, 69, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pRightBar1->SetClipRect( 0, 143, 3, 194 );
	InsertChildObject( pRightBar1 );
	
	// 개인 거래창 오른쪽 라인2 생성 ( 3 X 166 )
	_XImageStatic* pRightBar2 = new _XImageStatic;
	pRightBar2->Create( 285, 69, 288, 235, &g_MainInterfaceTextureArchive, mainframeinterface );	
	pRightBar2->SetClipRect( 184, 64, 187, 230 );
	InsertChildObject( pRightBar2 );
	
	// 개인 거래창 오른쪽 라인3 생성 ( 3 X 35 )
	_XImageStatic* pRightBar3 = new _XImageStatic;
	pRightBar3->Create( 285, 235, 288, 270 , &g_MainInterfaceTextureArchive, mainframeinterface );	
	pRightBar3->SetClipRect( 180, 195, 183, 230 );
	InsertChildObject( pRightBar3 );
	
	// 개인 거래창 아래쪽 라인 생성 ( 177 X 3 )
	_XImageStatic* pDownLine = new _XImageStatic;
	pDownLine->Create( 3, 267, 285, 270 , &g_MainInterfaceTextureArchive, mainframeinterface );	
	pDownLine->SetClipRect( 3, 227, 180, 230 );
	pDownLine->SetScale(1.5932f, 1.0f);
	InsertChildObject( pDownLine );
		

	// 거래창 바탕 타일 생성
	m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 286, m_WindowPosition.y + 267);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	FLOAT ufactor = 285.0f/128.0f;
	FLOAT vfactor = 252.0f/128.0f;
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );
	
	for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
	{
		m_TradeItemSlotPosition[i].x = 179 + ((i%3)*33);
		m_TradeItemSlotPosition[i].y = 109 + ((i/3)*33);
	}
	
	// Model View Camera Setting =========================================
	m_ModelViewCamera.Set( 45, 110.0f/90.0f, 0.01f, 180.0f);
	m_ModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_ModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_ModelViewCamera.SetDistance( 1.0f );	
	m_ModelViewCamera.UpdateViewMatrix();
	m_ModelViewCamera.UpdateProjMatrix();
	
	// ViewPortReposition - for Model View
	m_ModelViewPort.X = (DWORD)(m_WindowPosition.x + 37);
	m_ModelViewPort.Y = (DWORD)(m_WindowPosition.y + 95);
	m_ModelViewPort.Width  = 110;
	m_ModelViewPort.Height = 90;
	m_ModelViewPort.MinZ = 0.0f;
	m_ModelViewPort.MaxZ = 1.0f;
	
	m_RotateAngle = 0.0f;
	
	
	return TRUE;
}

void _XWindow_PersonalStore::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

BOOL _XWindow_PersonalStore::CheckPersonalStoreNameIMEBoxArea(void)
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if(scrnpos->x > (m_WindowPosition.x+53) &&
		scrnpos->x < (m_WindowPosition.x + 279 ) &&
		scrnpos->z > (m_WindowPosition.y + 23) &&
		scrnpos->z < (m_WindowPosition.y + 42))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_PersonalStore::CheckTradeItemPriceIMEBoxArea(void)
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 12) &&
		scrnpos->z > (m_WindowPosition.y + 216) &&
		scrnpos->z < (m_WindowPosition.y + 234))
	{
		if( m_TradeItemPriceIMEControl.GetFocus() )
		{
			if(  scrnpos->x < (m_WindowPosition.x + 152) )
				return TRUE;
		}
		else
		{
			if(  scrnpos->x < (m_WindowPosition.x + 170) )
				return TRUE;
		}
	}
	
	return FALSE;
}


void _XWindow_PersonalStore::ShowWindow(BOOL show)
{
	if( show )
	{
		if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE 
			|| g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN 
			|| g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY )
		{
			if(g_pInventory_Window)
			{
				if(!g_pInventory_Window->m_bLockInventory)
					g_pInventory_Window->m_bLockInventory = TRUE;
			}
		}
	}
	else
	{
		if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_MODIFY )
		{
			m_btnOpen->EnableWindow(TRUE);
			m_btnCancle->EnableWindow(TRUE);
			g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_list_req,m_OwnerUniqueID , 1);
		
			g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;			
			ResetTradeInfo();
			
			// 노점이 끝나면 Inventory lock 해제
			if(g_pInventory_Window)
			{
				if(g_pInventory_Window->m_bLockInventory)
					g_pInventory_Window->m_bLockInventory = FALSE;
			}					
		}
		else
		{
			m_StoreNameIMEControl.ClearBuffer();
			m_TradeItemPriceIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL ); 	
		}
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_PersonalStore::Draw( _XGUIObject*& pfocusobject )
{	
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
		
		//_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+6, m_WindowPosition.x+194, m_WindowPosition.y+22 , D3DCOLOR_ARGB(127,0,0,0) );	
		//_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+5, m_WindowPosition.x+194, m_WindowPosition.y+22 , 0.0f, _XSC_DEFAULT );		
		
		// 개인 거래창 물건 리스트 Edge 위쪽
		_XDrawLine2D( m_WindowPosition.x+179, m_WindowPosition.y+107, m_WindowPosition.x+279, m_WindowPosition.y+107 , D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawLine2D( m_WindowPosition.x+179, m_WindowPosition.y+108, m_WindowPosition.x+279, m_WindowPosition.y+108 , D3DCOLOR_ARGB(255,147,128,86), D3DCOLOR_ARGB(255,147,128,86) );
		
		// 개인 거래창 물건 리스트 Edge 왼쪽
		_XDrawLine2D( m_WindowPosition.x+177, m_WindowPosition.y+109, m_WindowPosition.x+177, m_WindowPosition.y+209 , D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawLine2D( m_WindowPosition.x+178, m_WindowPosition.y+109, m_WindowPosition.x+178, m_WindowPosition.y+209 , D3DCOLOR_ARGB(255,147,128,86), D3DCOLOR_ARGB(255,147,128,86) );
		
		// 개인 거래창 물건 리스트 Edge 오른쪽
		_XDrawLine2D( m_WindowPosition.x+279, m_WindowPosition.y+109, m_WindowPosition.x+279, m_WindowPosition.y+209 , D3DCOLOR_ARGB(255,147,128,86), D3DCOLOR_ARGB(255,147,128,86) );
		_XDrawLine2D( m_WindowPosition.x+280, m_WindowPosition.y+109, m_WindowPosition.x+280, m_WindowPosition.y+209 , D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
		
		// 개인 거래창 물건 리스트 Edge 아래쪽
		_XDrawLine2D( m_WindowPosition.x+179, m_WindowPosition.y+209, m_WindowPosition.x+279, m_WindowPosition.y+209 , D3DCOLOR_ARGB(255,147,128,86), D3DCOLOR_ARGB(255,147,128,86) );
		_XDrawLine2D( m_WindowPosition.x+179, m_WindowPosition.y+210, m_WindowPosition.x+279, m_WindowPosition.y+210 , D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );		
		
		// 노점 이름 표시 ( 226 X 19)
		_XDrawSolidBar( m_WindowPosition.x+53,  m_WindowPosition.y+23, m_WindowPosition.x+279, m_WindowPosition.y+42 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+52,  m_WindowPosition.y+22, m_WindowPosition.x+279, m_WindowPosition.y+42 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
		
		// 노점 상태 표시 ( 102 X 36)
		_XDrawSolidBar( m_WindowPosition.x+178,  m_WindowPosition.y+68, m_WindowPosition.x+280, m_WindowPosition.y+104 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+177,  m_WindowPosition.y+67, m_WindowPosition.x+280, m_WindowPosition.y+104 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
		
		// 판매가격 바탕 ( 158 X 18)
		_XDrawSolidBar( m_WindowPosition.x+12,  m_WindowPosition.y+216, m_WindowPosition.x+170, m_WindowPosition.y+234 , D3DCOLOR_ARGB(255,0,0,0) );

		// 노점 버튼 바탕 ( 276 X 24)
		_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+240, m_WindowPosition.x+282, m_WindowPosition.y+264 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5,  m_WindowPosition.y+239, m_WindowPosition.x+282, m_WindowPosition.y+264 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
		
	}
	_XWindow::Draw(pfocusobject);
	if(!this->m_ShowWindow)	return;
	
	for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i ++)
	{
		// Trade Icon draw
		if( m_TradeItem[i].Get_m_sID() > 0 )
			g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_TradeItemSlotPosition[i].x+2, m_WindowPosition.y+m_TradeItemSlotPosition[i].y+2, &m_TradeItem[i]);		
	}
	
	if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY)
	{
		// 노점 이름 표시 
		if( m_PersonalStoreName[0] != 0 )
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 210, 210, 210 ) );
			g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+28, m_PersonalStoreName);
		}
		else
		{
			if( !m_StoreNameIMEControl.GetFocus() )
			{
				g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 210, 210, 210 ) );
				g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_BOOTH_INPUTNAME));
			}
		}
		TCHAR messagestring[64];
		memset( messagestring, 0, sizeof(TCHAR)*64 );	
		sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_NAME), g_pLocalUser->m_CharacterInfo.charactername );
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 222, 191, 48 ) );
		g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+47, messagestring );
		
		// 노점 상태 표시 
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 238, 185, 22 ) );
		g_XBaseFont->Puts( m_WindowPosition.x+184,  m_WindowPosition.y+73, _XGETINTERFACETEXT(ID_STRING_BOOTH_REFRESH) );
		g_XBaseFont->Puts( m_WindowPosition.x+184,  m_WindowPosition.y+89, _XGETINTERFACETEXT(ID_STRING_BOOTH_REFRESH1) );

		// 물품 판매 가격 표시 
		if( m_SelectedSlotNumber > -1 )
		{
			if( m_TradeItemPrice[m_SelectedSlotNumber] == 0 )
			{
				g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 244, 221, 158 ) );
				g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+220, _XGETINTERFACETEXT(ID_STRING_BOOTH_PRICE) );
//				if( !m_TradeItemPriceIMEControl.GetFocus() )
//				{
//					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 244, 221, 158 ) );
//					g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+220, _T("가격을 입력해 주십시오.") );
//				}
//				else
//				{
//					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 244, 221, 158 ) );
//					g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+220, _T("판매가격 :") );
//				}

			}
			else
			{
				TCHAR messagestring[64];
				memset( messagestring, 0, sizeof(TCHAR)*64 );
				
				TCHAR moneyStr[128];
				memset( moneyStr, 0, sizeof(moneyStr));
				sprintf(moneyStr, "%u", m_TradeItemPrice[m_SelectedSlotNumber]);
//				itoa(m_TradeItemPrice[m_SelectedSlotNumber], moneyStr, 10 );
				int colorindex = 0;
				
				if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
				{
					g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
					sprintf( messagestring, "%s %s", _XGETINTERFACETEXT(ID_STRING_BOOTH_PRICE), moneyStr );					
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 244, 221, 158 ) );
					sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_PRICE1), m_TradeItemPrice[m_SelectedSlotNumber] );					
				}
				
				g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+220, messagestring );
			}
		}
		
		g_XBaseFont->Flush();

		m_StoreNameIMEControl.Draw();
		m_TradeItemPriceIMEControl.Draw();	
	}
	else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
	{
		// 노점 이름 표시 
		if( m_PersonalStoreName[0] != 0 )
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 210, 210, 210 ) );
			g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+28, m_PersonalStoreName);
		}
		TCHAR messagestring[64];
		memset( messagestring, 0, sizeof(TCHAR)*64 );	
		sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_NAME), g_pLocalUser->m_CharacterInfo.charactername );
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 222, 191, 48 ) );
		g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+47, messagestring );
		
		// 노점 상태 표시 
		memset( messagestring, 0, sizeof(TCHAR)*64 );	
		sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_SELLITEM), m_TotalCountTradeItem );
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 173, 169, 127 ) );
		g_XBaseFont->Puts( m_WindowPosition.x+192,  m_WindowPosition.y+73, messagestring );
		g_XBaseFont->Puts( m_WindowPosition.x+192,  m_WindowPosition.y+89, _XGETINTERFACETEXT(ID_STRING_BOOTH_SELLITEM1) );
		
		// 물품 판매 가격 표시 
		if( m_SelectedSlotNumber > -1 )
		{
			if( m_TradeItemPrice[m_SelectedSlotNumber] != 0 )
			{
				TCHAR messagestring[64];
				memset( messagestring, 0, sizeof(TCHAR)*64 );
				TCHAR moneyStr[128];
				memset( moneyStr, 0, sizeof(moneyStr));
				sprintf(moneyStr, "%u", m_TradeItemPrice[m_SelectedSlotNumber]);
//				itoa(m_TradeItemPrice[m_SelectedSlotNumber], moneyStr, 10 );
				int colorindex = 0;
				
				if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
				{
					g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
					sprintf( messagestring, "%s %s", _XGETINTERFACETEXT(ID_STRING_BOOTH_PRICE), moneyStr );					
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 244, 221, 158 ) );
					sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_PRICE1), m_TradeItemPrice[m_SelectedSlotNumber] );				
				}
				
				g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+220, messagestring );
			}
		}
		g_XBaseFont->Flush();
		
	}	
	else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST )
	{
		// 노점 이름 표시 
		if( m_PersonalStoreName[0] != 0 )
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 210, 210, 210 ) );
			g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+28, m_PersonalStoreName);
		}

		if( m_OwnerName[0] != 0 )
		{
			TCHAR messagestring[64];
			memset( messagestring, 0, sizeof(TCHAR)*64 );	
			sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_NAME), m_OwnerName );
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 222, 191, 48 ) );
			g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+47, messagestring );
		}
		
		// 노점 상태 표시 
		TCHAR messagestring[64];
		memset( messagestring, 0, sizeof(TCHAR)*64 );	
		sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_REGISTER), m_TotalCountTradeItem );
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 173, 169, 127 ) );
		g_XBaseFont->Puts( m_WindowPosition.x+192,  m_WindowPosition.y+73, messagestring );
		g_XBaseFont->Puts( m_WindowPosition.x+184,  m_WindowPosition.y+89, _XGETINTERFACETEXT(ID_STRING_BOOTH_REGISTER1) );
		
		// 물품 판매 가격 표시 
		if( m_SelectedSlotNumber > -1 )
		{
			if( m_TradeItemPrice[m_SelectedSlotNumber] != 0 )
			{
				TCHAR messagestring[64];
				memset( messagestring, 0, sizeof(TCHAR)*64 );
				TCHAR moneyStr[128];
				memset( moneyStr, 0, sizeof(moneyStr));
				sprintf(moneyStr, "%u", m_TradeItemPrice[m_SelectedSlotNumber]);
//				itoa(m_TradeItemPrice[m_SelectedSlotNumber], moneyStr, 10 );
				int colorindex = 0;
				
				if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
				{
					g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
					sprintf( messagestring, "%s %s", _XGETINTERFACETEXT(ID_STRING_BOOTH_PRICE), moneyStr );					
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 244, 221, 158 ) );
					sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_PRICE1), m_TradeItemPrice[m_SelectedSlotNumber] );				
				}
				
				g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+220, messagestring );
			}
		}
		g_XBaseFont->Flush();
	}
	else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_MODIFY ) // 오너가 수정 중일때 클라이언트가 보는 화면 
	{
		// 노점 이름 표시 
		if( m_PersonalStoreName[0] != 0 )
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 210, 210, 210 ) );
			g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+28, m_PersonalStoreName);
		}
		
		if( m_OwnerName[0] != 0 )
		{
			TCHAR messagestring[64];
			memset( messagestring, 0, sizeof(TCHAR)*64 );	
			sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_BOOTH_NAME), m_OwnerName );
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 222, 191, 48 ) );
			g_XBaseFont->Puts( m_WindowPosition.x+59,  m_WindowPosition.y+47, messagestring );
		}
		
		// 노점 상태 표시 
		// 노점 상태 표시 
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 238, 185, 22 ) );
		g_XBaseFont->Puts( m_WindowPosition.x+184,  m_WindowPosition.y+73, _XGETINTERFACETEXT(ID_STRING_BOOTH_REFRESH) );
		g_XBaseFont->Puts( m_WindowPosition.x+184,  m_WindowPosition.y+89, _XGETINTERFACETEXT(ID_STRING_BOOTH_REFRESH1) );
			
		g_XBaseFont->Flush();
	}
	
	if( m_SelectedSlotNumber > -1 )
	{
		_XDrawRectAngle(m_WindowPosition.x+m_TradeItemSlotPosition[m_SelectedSlotNumber].x,
			m_WindowPosition.y+m_TradeItemSlotPosition[m_SelectedSlotNumber].y,
			m_WindowPosition.x+m_TradeItemSlotPosition[m_SelectedSlotNumber].x+31,
			m_WindowPosition.y+m_TradeItemSlotPosition[m_SelectedSlotNumber].y+31, 1.0f, _XSC_ITEM_SELECT);	
		
		RenderTradeItem3D();
		g_pDefaultTooltip_Window->Draw();
	}
}

BOOL _XWindow_PersonalStore::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;	

	if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY)
	{
		if( !ProcessPreparingPersonalStore(pfocusobject) ) return FALSE;
	}
	else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
	{
		if( !ProcessOpeningPersonalStore(pfocusobject) ) return FALSE;
	}	
	else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST )
	{
		if( !ProcessInterestingPersonalStore(pfocusobject) ) return FALSE;
	}
	else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_MODIFY ) // 오너가 수정 중일때 클라이언트가 보는 화면 
	{
		if( !ProcessModifingPersonalStore(pfocusobject) ) return FALSE;
	}
	
	if( m_TradeItemPriceIMEControl.GetFocus() )
	{
		strcpy(m_strPrice, m_TradeItemPriceIMEControl.GetText());
		//m_Price = atoi(m_strPrice);
		sscanf( m_strPrice, _T("%u"), &m_Price );
	}
	
	if(!_XWindow::Process(pfocusobject)) return FALSE;

	return TRUE;
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XWindow_PersonalStore::RenderTradeItem3D( void )
{
	int type = GetEqualItemType( m_TradeItem[m_SelectedSlotNumber].Get_m_cType() );
	int id = m_TradeItem[m_SelectedSlotNumber].Get_m_sID();
	int secondtype = m_TradeItem[m_SelectedSlotNumber].Get_m_cSecond();
	int thirdtype = 0;
	int modelindex = 0;
	
	if( id <= 0 ) return;

	switch(type) 
	{
	case _XGI_FC_WEAPON:
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_TradeItem[m_SelectedSlotNumber].Get_m_cType());
			if( WeaponItem )
			{
				modelindex = WeaponItem[id].sModelIndex;												
				thirdtype = WeaponItem[id].cThirdType;
			}			
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_TradeItem[m_SelectedSlotNumber].Get_m_cType());
			if( ClothesItem )
			{
				modelindex = ClothesItem[id].sModelIndex;
				if( ClothesItem[id].cSexType ) 
					modelindex+= g_Model_GenderSeparateIndex;
			}
		}		
		break;
	case _XGI_FC_BOOK :
		{
			modelindex = g_BookItemProperty[id].sModelIndex;						
		}
		break;
	case _XGI_FC_POTION:
		{
			modelindex = g_PotionItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			modelindex = g_ResourceItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			modelindex = 0;
		}
		break;
	case _XGI_FC_LIFE :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_MONEY :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			modelindex = g_ElixirItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_SOCKET :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_BOX :
		{
			modelindex = g_BoxItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			modelindex = g_BoxKeyItemProperty[id].sModelIndex;
		}
		break;
	}

	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);

	m_RotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_RotateAngle));


	if( type == _XGI_FC_WEAPON )
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(45.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	

	// Initialize Matrix
	D3DXMatrixIdentity(&m_RHWMatrix);	

	// Camera/ViewPort Setting
	m_ModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_ModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_ModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_ModelViewPort);

	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);

	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	

	
	
	FLOAT scalefactor;
	switch(type) 
	{
	case _XGI_FC_WEAPON:
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
						   -g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
						   -g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &matRot);
			
			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			switch( secondtype ) 
			{
			case _XGI_SC_CLOTHES_UPPER:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_UBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_UBodyType[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_UBodyType[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_UBodyType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_LOWER:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_LBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LBodyType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_LBodyType[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LBodyType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_GLOVES:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_LHandType[modelindex]->OBB_Center.x,
						-g_UserModel_LHandType[modelindex]->OBB_Center.y,
						-g_UserModel_LHandType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LHandType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = 0.0f;
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LHandType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_SHOES:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_LFootType[modelindex]->OBB_Center.x,
						-g_UserModel_LFootType[modelindex]->OBB_Center.y,
						-g_UserModel_LFootType[modelindex]->OBB_Center.z	);
					scalefactor = 0.28f / g_UserModel_LFootType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.x;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LFootType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);					
				}
				break;
			case _XGI_SC_CLOTHES_CAP:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_Cap[modelindex]->OBB_Center.x,
						-g_UserModel_Cap[modelindex]->OBB_Center.y,
						-g_UserModel_Cap[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Cap[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_Cap[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_Cap[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;			
			case _XGI_SC_CLOTHES_BACKPACK:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_BackPack[modelindex]->OBB_Center.x,
						-g_UserModel_BackPack[modelindex]->OBB_Center.y,
						-g_UserModel_BackPack[modelindex]->OBB_Center.z	);
					scalefactor = 0.4f / g_UserModel_BackPack[modelindex]->m_ObjectRadius; 
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_BackPack[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_BackPack[modelindex]->RenderInterfaceItem( &m_RHWMatrix );
				}
				break;	
			case _XGI_SC_CLOTHES_MASK :
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_Mask[modelindex]->OBB_Center.x,
						-g_UserModel_Mask[modelindex]->OBB_Center.y,
						-g_UserModel_Mask[modelindex]->OBB_Center.z	);
					scalefactor = 0.4f / g_UserModel_Mask[modelindex]->m_ObjectRadius;
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_Mask[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_Mask[modelindex]->RenderInterfaceItem( &m_RHWMatrix );
				}
				break;
			}			
		}		
		break;
	case _XGI_FC_BOOK :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_MartialBook[modelindex]->OBB_Center.x,
						   -g_ItemModel_MartialBook[modelindex]->OBB_Center.y,
						   -g_ItemModel_MartialBook[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_MartialBook[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_MartialBook[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_MartialBook[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_POTION:
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_Potion[modelindex]->OBB_Center.x,
						   -g_ItemModel_Potion[modelindex]->OBB_Center.y,
						   -g_ItemModel_Potion[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Potion[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_Potion[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_Potion[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_ETC[modelindex]->OBB_Center.x,
						   -g_ItemModel_ETC[modelindex]->OBB_Center.y,
						   -g_ItemModel_ETC[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_ETC[modelindex]->m_ObjectRadius;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_ETC[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_ETC[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.x,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.y,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;

			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;	
	case _XGI_FC_MONEY :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.x,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.y,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.x,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.y,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_Elixir[modelindex]->OBB_Center.x,
						   -g_ItemModel_Elixir[modelindex]->OBB_Center.y,
						   -g_ItemModel_Elixir[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Elixir[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_Elixir[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_Elixir[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_LIFE :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.x,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.y,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_SOCKET :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.x,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.y,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_BOX :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.x,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.y,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.x,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.y,
						   -g_ItemModel_JewelBox[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	}	

	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}
#else
void _XWindow_PersonalStore::RenderTradeItem3D( void )
{
	int type = m_TradeItem[m_SelectedSlotNumber].m_cType;
	int id = m_TradeItem[m_SelectedSlotNumber].m_sID;
	int secondtype = m_TradeItem[m_SelectedSlotNumber].m_cSecond;
	int thirdtype = 0;
	int modelindex = 0;
	
	switch(type) 
	{
	case _XGI_FC_WEAPON:
		{
			modelindex = g_WeaponItemProperty[id].sModelIndex;												
			thirdtype = g_WeaponItemProperty[id].cThirdType;
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			modelindex = g_ClothesItemProperty[id].sModelIndex;
			if( g_ClothesItemProperty[id].cSexType ) 
				modelindex+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			modelindex = g_WeaponItemProperty2[id].sModelIndex;												
			thirdtype = g_WeaponItemProperty2[id].cThirdType;
		}		
		break;
	case _XGI_FC_CLOTHES2:
		{
			modelindex = g_ClothesItemProperty2[id].sModelIndex;
			if( g_ClothesItemProperty2[id].cSexType ) 
				modelindex+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_BOOK :
		{
			modelindex = g_BookItemProperty[id].sModelIndex;						
		}
		break;
	case _XGI_FC_POTION:
		{
			modelindex = g_PotionItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			modelindex = g_ResourceItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			modelindex = 0;
		}
		break;
	case _XGI_FC_LIFE :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_MONEY :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			modelindex = g_ElixirItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_SOCKET :
		{
			modelindex = 2;
		}
		break;
	case _XGI_FC_BOX :
		{
			modelindex = g_BoxItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			modelindex = g_BoxKeyItemProperty[id].sModelIndex;
		}
		break;
	case _XGI_FC_WEAPON3:
		{
			modelindex = g_WeaponItemProperty3[id].sModelIndex;												
			thirdtype = g_WeaponItemProperty3[id].cThirdType;
		}		
		break;
	case _XGI_FC_CLOTHES3:
		{
			modelindex = g_ClothesItemProperty3[id].sModelIndex;
			if( g_ClothesItemProperty3[id].cSexType ) 
				modelindex+= g_Model_GenderSeparateIndex;
		}		
		break;
	}

	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);

	m_RotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_RotateAngle));


	if( type == _XGI_FC_WEAPON || type == _XGI_FC_WEAPON2 || type == _XGI_FC_WEAPON3 )
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(45.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	

	// Initialize Matrix
	D3DXMatrixIdentity(&m_RHWMatrix);
	D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
						   -g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
						   -g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);

	// Camera/ViewPort Setting
	m_ModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_ModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_ModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_ModelViewPort);

	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);

	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	

	
	
	FLOAT scalefactor;
	switch(type) 
	{
	case _XGI_FC_WEAPON:
	case _XGI_FC_WEAPON2:
	case _XGI_FC_WEAPON3:
		{
			scalefactor = 0.5f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &matRot);

			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}		
		break;
	case _XGI_FC_CLOTHES:
	case _XGI_FC_CLOTHES2:
	case _XGI_FC_CLOTHES3:
		{
			switch( secondtype ) 
			{
			case _XGI_SC_CLOTHES_UPPER:
				{
					scalefactor = 0.5f / g_UserModel_UBodyType[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_UBodyType[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_UBodyType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_LOWER:
				{
					scalefactor = 0.5f / g_UserModel_LBodyType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_LBodyType[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LBodyType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_GLOVES:
				{
					scalefactor = 0.5f / g_UserModel_LHandType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = 0.0f;
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LHandType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_SHOES:
				{
					scalefactor = 0.28f / g_UserModel_LFootType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.x;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LFootType[modelindex]->RenderInterfaceItem(&m_RHWMatrix);					
				}
				break;
			case _XGI_SC_CLOTHES_CAP:
				{
					scalefactor = 0.5f / g_UserModel_Cap[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_Cap[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_Cap[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;			
			case _XGI_SC_CLOTHES_BACKPACK:
				{
					scalefactor = 0.4f / g_UserModel_BackPack[modelindex]->m_ObjectRadius; 
										
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_BackPack[modelindex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_BackPack[modelindex]->RenderInterfaceItem( &m_RHWMatrix );
				}
				break;	
			case _XGI_SC_CLOTHES_MASK :
				{

				}
				break;
			}			
		}		
		break;
	case _XGI_FC_BOOK :
		{
			scalefactor = 0.5f / g_ItemModel_MartialBook[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_MartialBook[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_MartialBook[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_POTION:
		{
			scalefactor = 0.5f / g_ItemModel_Potion[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_Potion[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_Potion[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			scalefactor = 0.5f / g_ItemModel_ETC[modelindex]->m_ObjectRadius;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_ETC[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_ETC[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;

			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;	
	case _XGI_FC_MONEY :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			scalefactor = 0.5f / g_ItemModel_Elixir[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_Elixir[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_Elixir[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_LIFE :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_SOCKET :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_BOX :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	}	

	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}
#endif

BOOL _XWindow_PersonalStore::ProcessPreparingPersonalStore(_XGUIObject*& pfocusobject)
{	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[1])
			{
				// right down
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK;
			}
			
			// 왼쪽 버튼 클릭 시 처리 
			if(mousestate->bButton[0])
			{
				// left down
				if(CheckMousePosition())
				{
					if( CheckPersonalStoreNameIMEBoxArea() )
					{						
						if( m_Price > 0 )
						{
							m_TradeItemPrice[m_SelectedSlotNumber] = m_Price;
							m_Price = 0;
						}

						m_StoreNameIMEControl.ClearBuffer();
						m_TradeItemPriceIMEControl.ClearBuffer();
						m_btnInputOK->ShowWindow(FALSE);

						memset(m_PersonalStoreName, 0, sizeof(TCHAR)*128);
						m_StoreNameIMEControl.SetFocus();
						
						g_pInventory_Window->m_bDrawSelectedItem = FALSE;						
						if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
					else if( CheckTradeItemPriceIMEBoxArea() )
					{						
						TCHAR resultstring[256]; 
						memset( resultstring, 0, sizeof(TCHAR)*256 );
						strcpy( resultstring, m_StoreNameIMEControl.GetText());
#ifdef _XDEF_NEWVERSONFILETERING
						g_WordFilteringManager.Filtering( resultstring,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
						g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif
						int length = strlen(resultstring);
						if( length > 0 )
						{
							strcpy( m_PersonalStoreName, resultstring );
							if( g_pLocalUser )
								strcpy( g_pLocalUser->m_PersonalStoreName, resultstring );
							m_PersonalStoreName[length] = 0;
							m_StoreNameIMEControl.ClearBuffer();
							_XIMEKERNEL.SetFocus( NULL ); 	
						}

						if( m_SelectedSlotNumber > -1 )
						{
							m_StoreNameIMEControl.ClearBuffer();
							m_TradeItemPriceIMEControl.ClearBuffer();

							m_TradeItemPrice[m_SelectedSlotNumber] = 0;
							m_TradeItemPriceIMEControl.SetFocus();
							m_btnInputOK->ShowWindow(TRUE);
							
							g_pInventory_Window->m_bDrawSelectedItem = FALSE;						
							if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
					else
					{
						if(g_pInventory_Window)
						{
							if(g_pInventory_Window->GetShowStatus())
							{
								if(g_pInventory_Window->m_bHaveMouseSlot)
								{
									m_LastDraggingSlot = SearchEmptySlotIndex();
									if( m_LastDraggingSlot > -1)
									{
										if(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)	//장착하고 있는 것은 불가능
										{
											if( _XGameItem::CheckUserTrade( g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType(), 
																			g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_sID() ) )
											{												
												_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
												pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2123), TRUE);
												g_pInventory_Window->m_bHaveMouseSlot = FALSE;
												g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
												return FALSE;
											}
											
											if(g_pInventory_Window->m_MouseSlotItem.IsStackable())
											{
												// stackable 처리
												_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
												if(pTradeConfirm_Window)
												{
													pTradeConfirm_Window->ShowWindow(TRUE);			
													
													pTradeConfirm_Window->m_SelectedCategory1 = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
													pTradeConfirm_Window->m_SelectedItemID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
													m_TradeItem[m_LastDraggingSlot].m_InventoryNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
													pTradeConfirm_Window->m_PersonalStoreSlotIndex = m_LastDraggingSlot;
													pTradeConfirm_Window->m_TradeMode = _XPERSONALSTORE_TOTRADE_ITEM;
													pTradeConfirm_Window->MoveWindow((gnWidth>>1)-102, 344);
												}
											}
											else
											{
												CopyItemToSlot(m_LastDraggingSlot, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
												m_TradeItem[m_LastDraggingSlot].m_InventoryNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
												m_SelectedSlotNumber = m_LastDraggingSlot;
												g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Reset();
												g_pInventory_Window->m_bHaveMouseSlot = FALSE;
												g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
												
												m_TradeItemPrice[m_SelectedSlotNumber] = 0;
												m_TradeItemPriceIMEControl.SetFocus();
												m_btnInputOK->ShowWindow(TRUE);
											}
										} // if(g_pInventory_Window->m_MouseSlotItem.inventorynumber >= 16)
									}
								} // if(g_pInventory_Window->m_bHaveMouseSlot)
								else // 개인노점 창에서 그냥 클릭 했을 경우 
								{
									int selectedindex = SearchFullSlotIndex();
									if( selectedindex > -1 )
									{
										// 노점 이름을 입력하고 있을 때 자동 기입됨 
										if( m_StoreNameIMEControl.GetFocus() )
										{
											TCHAR resultstring[256]; 
											memset( resultstring, 0, sizeof(TCHAR)*256 );
											strcpy( resultstring, m_StoreNameIMEControl.GetText());
#ifdef _XDEF_NEWVERSONFILETERING
											g_WordFilteringManager.Filtering( resultstring,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
											g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif
											int length = strlen(resultstring);
											if( length > 0 )
											{
												strcpy( m_PersonalStoreName, resultstring );
												if( g_pLocalUser )
													strcpy( g_pLocalUser->m_PersonalStoreName, resultstring );
												m_PersonalStoreName[length] = 0;
												m_StoreNameIMEControl.ClearBuffer();
												m_TradeItemPriceIMEControl.ClearBuffer();
												_XIMEKERNEL.SetFocus( NULL ); 	
											}
										}

										if( m_SelectedSlotNumber != selectedindex )
										{
											if( m_TradeItemPriceIMEControl.GetFocus() )
											{
												if( m_Price > 0 )
												{
													m_TradeItemPrice[m_SelectedSlotNumber] = m_Price;
													m_Price = 0;
												}
												m_StoreNameIMEControl.ClearBuffer();
												m_TradeItemPriceIMEControl.ClearBuffer();
												_XIMEKERNEL.SetFocus( NULL ); 	
											}

											m_SelectedSlotNumber = selectedindex;
											if( m_TradeItemPrice[m_SelectedSlotNumber] == 0)
											{
												m_TradeItemPriceIMEControl.ClearBuffer();
												m_TradeItemPriceIMEControl.SetFocus();
												m_btnInputOK->ShowWindow(TRUE);
											}
											else
											{
												if( m_TradeItemPriceIMEControl.GetFocus() )
												{
													m_TradeItemPriceIMEControl.ClearBuffer();
													_XIMEKERNEL.SetFocus( NULL ); 	
												}	
												m_btnInputOK->ShowWindow(FALSE);
											}
											
										}
										
										if( g_pDefaultTooltip_Window )
										{
											char cType = m_TradeItem[m_SelectedSlotNumber].Get_m_cType();
											short sID = m_TradeItem[m_SelectedSlotNumber].Get_m_sID();	

											g_pDefaultTooltip_Window->SetPersonalStoreWindowPtr(this );
											
											if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_PERSONALSTORE_TRADEITEM, m_SelectedSlotNumber, cType, sID ) )
											{
												g_pDefaultTooltip_Window->ShowWindow( TRUE );
												g_MainWindowManager.SetTopWindow( g_pDefaultTooltip_Window );
											}
										}							
									}
									else
									{
										if( m_StoreNameIMEControl.GetFocus() )
										{
											TCHAR resultstring[256]; 
											memset( resultstring, 0, sizeof(TCHAR)*256 );
											strcpy( resultstring, m_StoreNameIMEControl.GetText());
#ifdef _XDEF_NEWVERSONFILETERING
											g_WordFilteringManager.Filtering( resultstring,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
											g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif
											int length = strlen(resultstring);
											if( length > 0 )
											{
												strcpy( m_PersonalStoreName, resultstring );
												if( g_pLocalUser )
													strcpy( g_pLocalUser->m_PersonalStoreName, resultstring );
												m_PersonalStoreName[length] = 0;
												m_StoreNameIMEControl.ClearBuffer();
												m_TradeItemPriceIMEControl.ClearBuffer();
												_XIMEKERNEL.SetFocus( NULL ); 	
											}
										}
										else if( m_TradeItemPriceIMEControl.GetFocus() )
										{
											if( m_Price > 0 )
											{
												m_TradeItemPrice[m_SelectedSlotNumber] = m_Price;
												m_Price = 0;
											}
											m_StoreNameIMEControl.ClearBuffer();
											m_TradeItemPriceIMEControl.ClearBuffer();
											_XIMEKERNEL.SetFocus( NULL ); 	
											m_btnInputOK->ShowWindow(FALSE);
										}
										
										g_pInventory_Window->m_bDrawSelectedItem = FALSE;						
										if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
											g_pDefaultTooltip_Window->ShowWindow( FALSE );
									}
								}
								
							}
						} // if(g_pInventory_Window)
					}

				
				}
				else
				{					
					if( g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus() && !g_pInventory_Window->CheckMousePosition() )
						{
							m_LastDraggingSlot = -1;
						}	
					}
				}
			}

			// 오른쪽 버튼 처리
			if(!mousestate->bButton[1])
			{
				if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
				{
					m_LastDraggingSlot = SearchFullSlotIndex();
					if( m_LastDraggingSlot > -1 )
					{	
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);	
						int inventoryindex = m_TradeItem[m_LastDraggingSlot].m_InventoryNumber;
						if( m_TradeItem[m_LastDraggingSlot].IsStackable() )
						{
							if( g_pLocalUser->m_UserItemList[ inventoryindex ].Get_m_ucCount() == 0)
							{
								g_pLocalUser->m_UserItemList[ inventoryindex ].Copy( m_TradeItem[m_LastDraggingSlot] );
							}
							else
							{
								g_pLocalUser->m_UserItemList[ inventoryindex ].Set_m_ucCount(g_pLocalUser->m_UserItemList[ inventoryindex ].Get_m_ucCount() + m_TradeItem[m_LastDraggingSlot].Get_m_ucCount());
							}
						}
						else
						{
							g_pLocalUser->m_UserItemList[ inventoryindex ].Copy( m_TradeItem[m_LastDraggingSlot] );
						}
						m_TradeItem[m_LastDraggingSlot].ResetTradeItem();
						m_TradeItemPriceIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL ); 
						m_btnInputOK->ShowWindow(FALSE);

						if( m_SelectedSlotNumber == m_LastDraggingSlot )
							m_SelectedSlotNumber = -1;
					}
					m_WindowMouseState = _XW_STATE_NONE;
					m_LastDraggingSlot = -1;
					
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )
							g_pDefaultTooltip_Window->ShowWindow(FALSE);
					}

				} // if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			}
		}
	}
	return TRUE;
}

BOOL _XWindow_PersonalStore::ProcessOpeningPersonalStore(_XGUIObject*& pfocusobject)
{	
	
	// 개인 노점을 열었을때도 일정한 간격으로 시그널을 보냄 
	if((g_LocalSystemTime - m_PrevPacketSendedTime) > 3000)
	{		
		g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_broad_req_cast,m_ServerPersonalStoreID);
		m_PrevPacketSendedTime  = g_LocalSystemTime;
	}
	

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
				int selectedindex = SearchFullSlotIndex();
				if( selectedindex > -1 )
				{
					if( m_SelectedSlotNumber != selectedindex )
					{
						
						m_SelectedSlotNumber = selectedindex;
						if( m_TradeItemPrice[m_SelectedSlotNumber] == 0)
						{
							m_TradeItemPriceIMEControl.SetFocus();
							m_btnInputOK->ShowWindow(TRUE);
						}
						else
						{
							if( m_TradeItemPriceIMEControl.GetFocus() )
							{
								m_TradeItemPriceIMEControl.ClearBuffer();
								_XIMEKERNEL.SetFocus( NULL ); 	
								m_btnInputOK->ShowWindow(FALSE);
							}	
						}
						
					}
					char cType = m_TradeItem[m_SelectedSlotNumber].Get_m_cType();
					short sID = m_TradeItem[m_SelectedSlotNumber].Get_m_sID();	
					
					if( g_pDefaultTooltip_Window )
					{
						g_pDefaultTooltip_Window->SetPersonalStoreWindowPtr(this );
						
						if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_PERSONALSTORE_TRADEITEM, m_SelectedSlotNumber, cType, sID ) )
						{
							g_pDefaultTooltip_Window->ShowWindow( TRUE );
							g_MainWindowManager.SetTopWindow( g_pDefaultTooltip_Window );
						}
					}							
				}
				else
				{
					g_pInventory_Window->m_bDrawSelectedItem = FALSE;						
					if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
				
			}
		}
	}
	return TRUE;
}

BOOL _XWindow_PersonalStore::ProcessInterestingPersonalStore(_XGUIObject*& pfocusobject)
{	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[1])
			{
				// right down
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK;
			}
			
			// 왼쪽 버튼 클릭 시 처리 
			if(mousestate->bButton[0])
			{
				// left down
				int selectedindex = SearchFullSlotIndex();
				if( selectedindex > -1 )
				{
					if( m_SelectedSlotNumber != selectedindex )
					{						
						m_SelectedSlotNumber = selectedindex;						
					}
					char cType = m_TradeItem[m_SelectedSlotNumber].Get_m_cType();
					short sID = m_TradeItem[m_SelectedSlotNumber].Get_m_sID();	
					
					if( g_pDefaultTooltip_Window )
					{
						g_pDefaultTooltip_Window->SetPersonalStoreWindowPtr(this );
						
						if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_PERSONALSTORE_TRADEITEM, m_SelectedSlotNumber, cType, sID ) )
						{
							g_pDefaultTooltip_Window->ShowWindow( TRUE );
							g_MainWindowManager.SetTopWindow( g_pDefaultTooltip_Window );
						}
					}							
				}
				else
				{
					g_pInventory_Window->m_bDrawSelectedItem = FALSE;						
					if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
			
			// 오른쪽 버튼 처리
			if(!mousestate->bButton[1])
			{
				if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
				{
					int selectedindex = SearchFullSlotIndex();
					if( m_LastDraggingSlot > -1 )
					{	
						m_SelectedSlotNumber = selectedindex;
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_PERSONALSTORE_OPENBUTTON, 0), (LPARAM)gHWnd);
					}					
					m_WindowMouseState = _XW_STATE_NONE;					
				} // if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			}
		}
	}
	return TRUE;
}

BOOL _XWindow_PersonalStore::ProcessModifingPersonalStore(_XGUIObject*& pfocusobject)
{	
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
				int selectedindex = SearchFullSlotIndex();
				if( selectedindex > -1 )
				{
					if( m_SelectedSlotNumber != selectedindex )
					{
						
						m_SelectedSlotNumber = selectedindex;
						if( m_TradeItemPrice[m_SelectedSlotNumber] == 0)
						{
							m_TradeItemPriceIMEControl.SetFocus();
							m_btnInputOK->ShowWindow(TRUE);
						}
						else
						{
							if( m_TradeItemPriceIMEControl.GetFocus() )
							{
								m_TradeItemPriceIMEControl.ClearBuffer();
								_XIMEKERNEL.SetFocus( NULL ); 	
								m_btnInputOK->ShowWindow(FALSE);
							}	
						}
						
					}
					char cType = m_TradeItem[m_SelectedSlotNumber].Get_m_cType();
					short sID = m_TradeItem[m_SelectedSlotNumber].Get_m_sID();	
					
					if( g_pDefaultTooltip_Window )
					{
						g_pDefaultTooltip_Window->SetPersonalStoreWindowPtr(this );
						
						if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_PERSONALSTORE_TRADEITEM, m_SelectedSlotNumber, cType, sID ) )
						{
							g_pDefaultTooltip_Window->ShowWindow( TRUE );
							g_MainWindowManager.SetTopWindow( g_pDefaultTooltip_Window );
						}
					}							
				}
				else
				{
					g_pInventory_Window->m_bDrawSelectedItem = FALSE;						
					if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}			
		}
	}
	return TRUE;
}

void _XWindow_PersonalStore::MoveWindow(int X, int Y)
{	
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 286, m_WindowPosition.y + 267 );
	m_StoreNameIMEControl.MoveWindow(m_WindowPosition.x + 59, m_WindowPosition.y + 28);
	m_TradeItemPriceIMEControl.MoveWindow(m_WindowPosition.x + 84, m_WindowPosition.y + 220);
	m_ModelViewPort.X = (DWORD)(m_WindowPosition.x + 37);
	m_ModelViewPort.Y = (DWORD)(m_WindowPosition.y + 95);
}

int _XWindow_PersonalStore::SearchEmptySlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x+m_TradeItemSlotPosition[i].x && scrnpos->x < m_WindowPosition.x+m_TradeItemSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_WindowPosition.y+m_TradeItemSlotPosition[i].y && scrnpos->z < m_WindowPosition.y+m_TradeItemSlotPosition[i].y+30)
			{
				if(m_TradeItem[i].Get_m_sID() == 0)
				{
					return i;
				}
			}
		}		
	}
	return -1;
}

int _XWindow_PersonalStore::SearchFullSlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x+m_TradeItemSlotPosition[i].x && scrnpos->x < m_WindowPosition.x+m_TradeItemSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_WindowPosition.y+m_TradeItemSlotPosition[i].y && scrnpos->z < m_WindowPosition.y+m_TradeItemSlotPosition[i].y+30)
			{
				if(m_TradeItem[i].Get_m_sID() != 0)
				{
					return i;
				}
			}
		}		
	}
	return -1;
}

int _XWindow_PersonalStore::GetNextEmptySlot(void)
{
	for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
	{
		if(m_TradeItem[i].Get_m_sID() <= 0)
			return i;
	}
	return -1;
}

void _XWindow_PersonalStore::CopyItemToSlot(int slotnumber, int invennumber)
{
	m_TradeItem[slotnumber].Set_m_cType(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType());
	m_TradeItem[slotnumber].Set_m_cSecond(g_pLocalUser->m_UserItemList[invennumber].Get_m_cSecond());
	m_TradeItem[slotnumber].Set_m_sID(g_pLocalUser->m_UserItemList[invennumber].Get_m_sID());
	m_TradeItem[slotnumber].m_cIdentify = g_pLocalUser->m_UserItemList[invennumber].m_cIdentify;
	
	m_TradeItem[slotnumber].Set_m_cThird( g_pLocalUser->m_UserItemList[invennumber].Get_m_cThird());
	m_TradeItem[slotnumber].Set_m_ucCount(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucCount());
	
	m_TradeItem[slotnumber].Set_m_usCurDur(g_pLocalUser->m_UserItemList[invennumber].Get_m_usCurDur());
	m_TradeItem[slotnumber].Set_m_usMaxDur(g_pLocalUser->m_UserItemList[invennumber].Get_m_usMaxDur());

	for( int i = 0; i < 4; i++ )
		m_TradeItem[slotnumber].Set_m_sSocket( i, g_pLocalUser->m_UserItemList[invennumber].Get_m_sSocket(i) );

	m_TradeItem[slotnumber].Set_m_ucStrength( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucStrength() );
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
	m_TradeItem[slotnumber].m_usCurProtect = g_pLocalUser->m_UserItemList[invennumber].m_usCurProtect;
	m_TradeItem[slotnumber].m_usCurMaxProtect = g_pLocalUser->m_UserItemList[invennumber].m_usCurMaxProtect;
	m_TradeItem[slotnumber].m_usUsedProtect = g_pLocalUser->m_UserItemList[invennumber].m_usUsedProtect;
	
	m_TradeItem[slotnumber].Set_m_ucWeaponStrength( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucWeaponStrength() );
	m_TradeItem[slotnumber].Set_m_ucSharpness( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucSharpness() );
#else
	m_TradeItem[slotnumber].Set_m_ucSharpness( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucSharpness() );
#endif
	m_TradeItem[slotnumber].Set_m_ucSocketCount( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucSocketCount() );
}
