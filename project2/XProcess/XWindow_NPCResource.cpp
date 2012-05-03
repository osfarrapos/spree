// XWindow_NPCResource.cpp: implementation of the _XWindow_NPCResource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_NPCResource.h"
#include "XSR_STRINGHEADER.H"

BOOL __stdcall _NPCResourceListBoxItemMouseHoverCallback( int param1, int param2, int XPos, int YPos )
{
	if( param2 == -1 )
	{
		
	}
	else
	{
		_XWindow_NPCResource* pNPCResource_Window = (_XWindow_NPCResource*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCRESOURCE);

		if(pNPCResource_Window)
		{
			LPTSTR szitemtext = pNPCResource_Window->m_listboxResourceItem->GetItemText( param2, 2 );
			if( szitemtext )
			{
				int strwidth = g_XBaseFont->GetWidth( szitemtext );
				
				if( strwidth >= 134 )
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					_XDrawSolidBar( XPos+36, YPos+1, XPos+36+strwidth+5, YPos+18 , D3DCOLOR_ARGB(255,44,44,44) );
					_XDrawRectAngle( XPos+36, YPos+1, XPos+36+strwidth+5, YPos+18 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->Puts( XPos+38, YPos+5, szitemtext );
					
					if( param2 == pNPCResource_Window->m_listboxResourceItem->GetSelectedItem() )
					{
						g_XBaseFont->Puts( XPos+39, YPos+5, szitemtext );
					}
					
					g_XBaseFont->Flush();
				}
			}
		}
	}
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCResource::_XWindow_NPCResource() : m_bStrength(false)
{
	m_indexSelectedPackage			= -1;
	m_indexSelectedResourceItem		= -1;
	m_listboxResourceItem			= NULL;
	m_imageNeeItemBorder			= NULL;
	m_imageCompleteNeeItemBorder	= NULL;
	m_bProcessExchange				= false;


}

_XWindow_NPCResource::~_XWindow_NPCResource()
{
	SAFE_DELETE(m_imageNeeItemBorder);
	SAFE_DELETE(m_imageCompleteNeeItemBorder);

}

BOOL _XWindow_NPCResource::Initialize()
{

	int mainframeinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga");
	int skillinterface				= g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	int chattingbuttonindex			= g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	int lifeskillindex				= g_MainInterfaceTextureArchive.FindResource( g_MI_LifeSkillTextureName);
	int charinfointerface			= g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int characterwinlindex			= g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create(0, 0, 180, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleLeftBar->SetClipRect(0, 125, 180, 143);
	InsertChildObject(pTitleLeftBar);
	
	_XImageStatic* pTitleCenterBar = new _XImageStatic;
	pTitleCenterBar->Create(180, 0, 282, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleCenterBar->SetClipRect(77, 125, 180, 143);
	InsertChildObject(pTitleCenterBar);
	
	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create(282, 0, 365, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleRightBar->SetClipRect(99, 125, 183, 143);
	InsertChildObject(pTitleRightBar);
	
	// 도움 버튼 
	_XBTN_STRUCTURE helpstruct = { TRUE, { 348, 2 }, { 12, 12 }, _XDEF_NPCRESOURCE_HELP_BTN, charinfointerface,charinfointerface,charinfointerface, &g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );		
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	
	m_BorderTile.Create(m_WindowPosition.x+1, m_WindowPosition.y+18, m_WindowPosition.x+365, m_WindowPosition.y+268);
	m_BorderTile.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTile.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));
	
	FLOAT ufactor = 364.0f/128.0f;
	FLOAT vfactor = 250.0f/128.0f;
	
	m_BorderTile.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);
	
	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 17, 2, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	pLeftBorder_1->SetScale(1.0f, 7.2f);
	InsertChildObject(pLeftBorder_1);
	
	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(363, 17, 365, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, 7.2f);
	InsertChildObject(pRightBorder_1);
	
	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 245, 2, 269, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftCorner->SetClipRect(0, 205, 3, 230);
	InsertChildObject(pLeftCorner);
	
	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(363, 245, 365, 269, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightCorner->SetClipRect(180, 205, 183, 230);
	InsertChildObject(pRightCorner);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 267, 179, 269, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	pBottomBorder->SetScale(2.033f, 1.0f);
	InsertChildObject(pBottomBorder);

	// 교환 가능 목록 바탕 1 ( 45 X 20 )  //( 188 X 20 )
	_XImageStatic* pMainTitleLeftBorder = new _XImageStatic;
	pMainTitleLeftBorder->Create(9, 22, 54, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pMainTitleLeftBorder->SetClipRect(4, 178, 49, 199);
	InsertChildObject(pMainTitleLeftBorder);
		
	// 교환 가능 목록 바탕 2 ( 94 X 20 )  //( 188 X 20 )
	_XImageStatic* pMainTitleCenterBorder = new _XImageStatic;
	pMainTitleCenterBorder->Create(54, 22, 148, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pMainTitleCenterBorder->SetClipRect(49, 178, 59, 199);
	pMainTitleCenterBorder->SetScale( 9.4f, 1.0f);
	InsertChildObject(pMainTitleCenterBorder);
		
	// 교환 가능 목록 바탕 3 ( 49 X 20 )  //( 188 X 20 )
	_XImageStatic* pMainTitleRightBorder = new _XImageStatic;
	pMainTitleRightBorder->Create(148, 22, 197, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pMainTitleRightBorder->SetClipRect(59, 178, 108, 199);
	InsertChildObject(pMainTitleRightBorder);
	
	
	// 필요 물품 목록 바탕 1 ( 45 X 20 )  //( 154 X 20 )
	_XImageStatic* pSubTitleLeftBorder = new _XImageStatic;
	pSubTitleLeftBorder->Create(205, 22, 250, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pSubTitleLeftBorder->SetClipRect(4, 178, 49, 199);
	InsertChildObject(pSubTitleLeftBorder);
	
	// 보상 물품 목록 바탕 2 ( 60 X 20 )  //( 154 X 20 )
	_XImageStatic* pSubTitleCenterBorder = new _XImageStatic;
	pSubTitleCenterBorder->Create(250, 22, 310, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pSubTitleCenterBorder->SetClipRect(49, 178, 59, 199);
	pSubTitleCenterBorder->SetScale( 6.0f, 1.0f);
	InsertChildObject(pSubTitleCenterBorder);
	
	// 보상 물품 목록 바탕 3 ( 49 X 20 )  //( 154 X 20 )
	_XImageStatic* pSubTitleRightBorder = new _XImageStatic;
	pSubTitleRightBorder->Create(310, 22, 359, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pSubTitleRightBorder->SetClipRect(59, 178, 108, 199);
	InsertChildObject(pSubTitleRightBorder);
	
	
	// 수집을 표시하는 리스트 박스
	_XLISTBOX_STRUCTURE resourcestructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 9, 48 },		// 윈도우 좌표
		{ 190, 211 },	// 윈도우 사이즈
		_XDEF_NPCRESOURCE_RESOURCELISTBOX,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		20,												// 리스트 아이템 라인 피치
		10,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 211 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		255,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	
	m_listboxResourceItem = new _XListBox;
	m_listboxResourceItem->Create(resourcestructure);
	
	_XVScrollBar* pScrollBar = m_listboxResourceItem->GetScrollBarObject();
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
	m_listboxResourceItem->SetAlwaysShowScrollBarMode(TRUE);
	m_listboxResourceItem->SetSelectItemMouseHoverCallBack( _NPCResourceListBoxItemMouseHoverCallback );
	
	POINT columnoffset1 = { 0, 0 };
	POINT columnoffset2 = { 2, 2 };
	POINT columnoffset3 = { 38, 5 };
	m_listboxResourceItem->SetColumnOffset(0, columnoffset1);				// column offset
	m_listboxResourceItem->SetColumnOffset(1, columnoffset2);
	m_listboxResourceItem->SetColumnOffset(2, columnoffset3);
	
	m_listboxResourceItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxResourceItem->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxResourceItem->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	
	m_listboxResourceItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxResourceItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxResourceItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxResourceItem->SetColumnWidth(0, 150);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxResourceItem->SetColumnWidth(1, 150);
	m_listboxResourceItem->SetColumnWidth(2, 134);
	
	m_listboxResourceItem->SetTrackWidth(134);	
	m_listboxResourceItem->ShowWindow(TRUE);
	InsertChildObject(m_listboxResourceItem);
	
	
	// 교환하기 버튼 
	_XBTN_STRUCTURE exchangebtnstruct = 
	{
		TRUE, {230, 236}, {111, 26}, _XDEF_NPCRESOURCE_EXCHANGE_BTN,
			characterwinlindex,
			characterwinlindex,
			characterwinlindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnExchange = new _XButton;
	m_btnExchange->Create(exchangebtnstruct);
	m_btnExchange->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 105, 112, 131 );
	m_btnExchange->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 105, 112, 131 );
	m_btnExchange->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 131, 112, 157 );
	m_btnExchange->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCRESOURCE_2265 );
	m_btnExchange->SetGrowTextMode( TRUE, 0xFF000000 );
	m_btnExchange->SetButtonTextColor( D3DCOLOR_ARGB(255, 250, 195, 34) );
	m_btnExchange->ShowWindow(TRUE);
	InsertChildObject(m_btnExchange);


	m_imageNeeItemBorder = new _XImageStatic;
	m_imageNeeItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwinlindex);
	m_imageNeeItemBorder->SetClipRect(191, 0, 223, 32);
	
	m_imageCompleteNeeItemBorder = new _XImageStatic;
	m_imageCompleteNeeItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwinlindex);
	m_imageCompleteNeeItemBorder->SetClipRect(191, 33, 223, 65);
	return TRUE;
}
void _XWindow_NPCResource::ShowWindow(BOOL show)
{
	if(show)
	{
		SetListItem();
		if( g_pDefaultTooltip_Window )
			g_pDefaultTooltip_Window->SetNPCResourceWindowPtr( this );
	}
	else
	{
		m_bProcessExchange = false;
		m_bStrength = false;
		m_indexSelectedPackage		= -1;
		m_indexSelectedResourceItem = -1;
	}
	
	_XWindow::ShowWindow(show);
}
void _XWindow_NPCResource::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));

		// 자원 리스트 바탕 ( 190 X 211 )
		_XDrawSolidBar( m_WindowPosition.x+9,  m_WindowPosition.y+48, m_WindowPosition.x+199, m_WindowPosition.y+259 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+8,  m_WindowPosition.y+47, m_WindowPosition.x+199, m_WindowPosition.y+259 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		// 자원 리스트 바탕 ( 154 X 184 )
		_XDrawSolidBar( m_WindowPosition.x+206,  m_WindowPosition.y+48, m_WindowPosition.x+360, m_WindowPosition.y+232 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+205,  m_WindowPosition.y+47, m_WindowPosition.x+360, m_WindowPosition.y+232 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		int column = 0;
		int row = 0;
		for( int i = 0; i < 20 ; i++ )
		{
			column = i%4;
			row	= i/4;
			m_imageNeeItemBorder->Draw(m_WindowPosition.x+211+(column*37), m_WindowPosition.y+52+(36*row));					
		}
		
		if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
		{
			int count = 0;
			int column = 0;
			int row = 0;
			_XCollectionResourceItem* currentneeditem = NULL;
			list <_XCollectionResourceItem*>::iterator iter_needitem;
			for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
			{
				currentneeditem = *iter_needitem;
				if(currentneeditem)
				{				
					column = count%4;
					row = count/4;
					g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+212+(column*37), m_WindowPosition.y+53+(36*row), currentneeditem);
					if( currentneeditem->m_bComplete ) 
					{
						m_imageCompleteNeeItemBorder->Draw(m_WindowPosition.x+211+(column*37), m_WindowPosition.y+52+(36*row));	
					}
					else
					{
						_XDrawSolidBar( m_WindowPosition.x+212+(column*37),  m_WindowPosition.y+53+(36*row),
							m_WindowPosition.x+242+(column*37), m_WindowPosition.y+83+(36*row) , D3DCOLOR_ARGB(127,0,0,0) );
					}

					count++;
				}
			}
		}
	}
	
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;
	
	// TODO

	
	
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( m_WindowPosition.x+13,  m_WindowPosition.y+26, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2259) ); 
	g_XBaseFont->Puts( m_WindowPosition.x+212,  m_WindowPosition.y+26, _XGETINTERFACETEXT(ID_STRING_NPCRESOURCE_2266) );
	g_XBaseFont->Flush();
	
}

BOOL _XWindow_NPCResource::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;
	
	// TODO
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{										
				if(CheckMousePosition())
				{		
					if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
					{
						int type = -1;
						int id = 0;
						int index = -1;
						int column = 0;
						int row = 0;
						bool bfindneeditem = false;
						for( int i = 0; i< 20; i++ )
						{
							column = i%4;	// 보상 아이템 인덱스
							row = i/4;		// 보상 세트 인덱스
							if(scrnpos->x >= m_WindowPosition.x+212+(column*37) &&	scrnpos->x <= m_WindowPosition.x+244+(column*37) 
								&& scrnpos->z >= m_WindowPosition.y+53+(36*row) && scrnpos->z <= m_WindowPosition.y+85+(36*row))
							{
								_XCollectionResourceItem* currentneeditem = NULL;
								list <_XCollectionResourceItem*>::iterator iter_needitem;
								for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
								{
									currentneeditem = *iter_needitem;
									if(currentneeditem)
									{	
										if( currentneeditem->m_Index == i)
										{
											type = currentneeditem->Get_m_cType();
											id = currentneeditem->Get_m_sID();
											index = i;
											bfindneeditem = true;
											break;
										}
									}
								}
								break;
								
							}	
						}
						if( bfindneeditem )
						{
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_NPCRESOURCEITEM, index, type, id ) )									
								{
									g_pDefaultTooltip_Window->ShowWindow( TRUE );
								}
							}
						}
						else
						{
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
				}					
				m_WindowMouseState = _XW_STATE_LEFTCLICK;				
			}
		}

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				m_WindowMouseState = _XW_STATE_NONE;
			}
		}
		if(!mousestate->bButton[0])
		{
			if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
			{
				m_WindowMouseState = _XW_STATE_NONE;
			}
		}
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_NPCResource::MoveWindow(int X, int Y)
{			
	_XWindow::MoveWindow(X, Y);
	
	m_BorderTile.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y+18, m_WindowPosition.x+365, m_WindowPosition.y+268 );	
}
void _XWindow_NPCResource::SetListItem(void)
{	
	if( !m_listboxResourceItem ) return;
	if( m_indexSelectedPackage == -1 ) return;
	if( g_ResourceManager.m_TotalCountResourceItem == 0 )  return;
	if( g_ResourceManager.m_listResourcePackage.empty() ) return;
	if( m_bProcessExchange ) return;

	m_listboxResourceItem->DeleteAllItem();	
	
	int count = 0;
	int lifeskillindex = g_MainInterfaceTextureArchive.FindResource( g_MI_LifeSkillTextureName);
	RECT rect1 = {0, 32, 177, 52}; 
	RECT rect2 = {126, 53, 158, 71};// 자원 
	_XResourcePackage* currentpackage = NULL;
	list <_XResourcePackage*>::iterator iter_package;
	for(iter_package = g_ResourceManager.m_listResourcePackage.begin() ; iter_package != g_ResourceManager.m_listResourcePackage.end() ; iter_package++)
	{
		currentpackage = *iter_package;
		if(currentpackage)
		{
			if( currentpackage->m_ResourcePackageIndex == m_indexSelectedPackage )
			{
				int resourceindex = 0;
				list <int>::iterator iter_resourceindex;
				for(iter_resourceindex = currentpackage->m_listResourceItemIndex.begin() ; iter_resourceindex != currentpackage->m_listResourceItemIndex.end() ; iter_resourceindex++)
				{
					resourceindex = *iter_resourceindex;
					if( resourceindex != -1 && resourceindex < g_ResourceManager.m_TotalCountResourceItem )
					{			
						m_listboxResourceItem->InsertListItem(_T(""), 0, g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceIndex+1);		
						m_listboxResourceItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxResourceItem->SetItemAttrib(count, 0, lifeskillindex , rect1);
						m_listboxResourceItem->SetItemAttrib(count, 1, lifeskillindex , rect2);
						
						if( g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceName[0] != 0 )
							m_listboxResourceItem->SetItemText(count, 2, g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceName );
																								
						m_listboxResourceItem->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
						m_listboxResourceItem->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 255, 255, 255));						
						count++;			
					}
				}
				
				break;
			}
		}
	}
}
void _XWindow_NPCResource::SetSelectedResourceItem(int resourceid)
{
	m_indexSelectedResourceItem = resourceid;
	int currentcompleteneeditem = 0;
	m_bStrength = false;
	// 자원 체크하기 	
	if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
	{
		int needitemcount = 0;
		_XCollectionResourceItem* currentneeditem = NULL;
		list <_XCollectionResourceItem*>::iterator iter_needitem;
		for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
		{
			currentneeditem = *iter_needitem;
			if(currentneeditem)
			{				
				needitemcount = 0;
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == currentneeditem->Get_m_cType() && g_pLocalUser->m_UserItemList[i].Get_m_sID() == currentneeditem->Get_m_sID())
						needitemcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();

					if(m_bStrength)
						continue;
					
					//Author : 양희왕 //breif : 제련된 무기에 대한 질의
					switch( currentneeditem->Get_m_cType() )
					{
					case _XGI_FC_WEAPON :
					case _XGI_FC_WEAPON2 :
					case _XGI_FC_WEAPON3 :
						{
							if( needitemcount == 1 && g_pLocalUser->m_UserItemList[i].Get_m_sID() == currentneeditem->Get_m_sID() &&
								( g_pLocalUser->m_UserItemList[i].Get_m_ucStrength() > 0  ||
								  g_pLocalUser->m_UserItemList[i].m_usCurProtect != 0 ||
								  g_pLocalUser->m_UserItemList[i].m_usCurMaxProtect != _XGameItem::GetNature(currentneeditem->Get_m_cType(), currentneeditem->Get_m_sID()) ||
								  g_pLocalUser->m_UserItemList[i].m_usUsedProtect != 0 ))
							{
								m_bStrength = true;
							}
							else if( needitemcount == 1 )
								m_bStrength = false;
						}
						break;
					}
					
				}
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장 //빈슬롯 없으면 캐시 행낭 검색
				for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == currentneeditem->Get_m_cType() && g_pLocalUser->m_UserItemList[i].Get_m_sID() == currentneeditem->Get_m_sID())
						needitemcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();

					if(m_bStrength)
						continue;
					
					//Author : 양희왕 //breif : 제련된 무기에 대한 질의
					switch( currentneeditem->Get_m_cType() )
					{
					case _XGI_FC_WEAPON :
					case _XGI_FC_WEAPON2 :
					case _XGI_FC_WEAPON3 :
						{
							if( needitemcount == 1 && g_pLocalUser->m_UserItemList[i].Get_m_sID() == currentneeditem->Get_m_sID() &&
								( g_pLocalUser->m_UserItemList[i].Get_m_ucStrength() > 0  ||
								  g_pLocalUser->m_UserItemList[i].m_usCurProtect != 0 ||
								  g_pLocalUser->m_UserItemList[i].m_usCurMaxProtect != _XGameItem::GetNature(currentneeditem->Get_m_cType(), currentneeditem->Get_m_sID()) ||
								  g_pLocalUser->m_UserItemList[i].m_usUsedProtect != 0 ))
							{
								m_bStrength = true;
							}
							else if( needitemcount == 1 )
								m_bStrength = false;
						}
						break;
					}									
				}
			#endif
				if( currentneeditem->Get_m_ucCount() <= needitemcount )
				{
					currentneeditem->m_bComplete = true;
					currentcompleteneeditem++;
				}
				else
				{
					currentneeditem->m_bComplete = false;
				}
			}
		}
	}
	if( currentcompleteneeditem < g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.size())
	{
		m_btnExchange->EnableWindow(FALSE);
	}
	else
	{
		m_btnExchange->EnableWindow(TRUE);
	}
}

bool _XWindow_NPCResource::CheckInventorySpace(void)
{	
	int		emptyslotcount = 0;
	int		needinvencount = 1; // 자원의 보상 공간은 한칸만 있으면 된다.
	
	for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
		{
			emptyslotcount++;
			if(emptyslotcount == needinvencount)
			{
				return true;
			}
		}
	}

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장 //빈슬롯 없으면 캐시 행낭 검색
	for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
		{
			emptyslotcount++;
			if(emptyslotcount == needinvencount)
			{
				return true;
			}
		}										
	}
#endif
	
	return false;
}
