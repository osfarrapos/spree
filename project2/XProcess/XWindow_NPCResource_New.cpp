// XWindow_NPCResource_New.cpp: implementation of the _XWindow_NPCResource_New class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_NPCResource_New.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCResource_New::_XWindow_NPCResource_New() : m_bStrength(false)
{
	m_pResourceList		= NULL;
	m_pEmptyItemBorder	= NULL;
	m_pCompleteItemBorder = NULL;
	m_pExchangeBtn		= NULL;
	m_pCloseBtn			= NULL;

	m_nSelectedTab		= _TAB_LIST;
	m_nSelectedResourcePackageIndex = 0;
	m_nSelectedResourceItem = 0;

	m_bProgressExchange = FALSE;

	memset(m_tItemInfo, 0, sizeof(m_tItemInfo));
}

_XWindow_NPCResource_New::~_XWindow_NPCResource_New()
{

}

BOOL _XWindow_NPCResource_New::Initialize()
{
	int mi_main_back01_tga = g_MainInterfaceTextureArchive.FindResource("mi_main_back01.tga");
	int mi_skillwin_tga = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	int mi_chat_quick_tga = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	int mi_characterwin_tga	= g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	
	_XImageStatic* pTitle = new _XImageStatic;
	pTitle->Create(0, 0, 183, 18, &g_MainInterfaceTextureArchive, mi_main_back01_tga);
	pTitle->SetClipRect(0, 125, 183, 143);
	InsertChildObject(pTitle);

	m_BorderTile.Create(m_WindowPosition.x+1, m_WindowPosition.y+18, m_WindowPosition.x+180, m_WindowPosition.y+270);
	m_BorderTile.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTile.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));
	
	FLOAT ufactor = 179.0f/128.0f;
	FLOAT vfactor = 252.0f/128.0f; 
	
	m_BorderTile.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);

	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 17, 2, 48, &g_MainInterfaceTextureArchive, mi_main_back01_tga);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	pLeftBorder_1->SetScale(1.0f, 250.f/32.f);
	InsertChildObject(pLeftBorder_1);
	
	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(180, 17, 183, 48, &g_MainInterfaceTextureArchive, mi_main_back01_tga);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, 250.f/32.f);
	InsertChildObject(pRightBorder_1);
	
	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 267, 2, 270, &g_MainInterfaceTextureArchive, mi_main_back01_tga);
	pLeftCorner->SetClipRect(0, 227, 3, 230);
	InsertChildObject(pLeftCorner);
	
	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(180, 267, 183, 270, &g_MainInterfaceTextureArchive, mi_main_back01_tga);
	pRightCorner->SetClipRect(180, 227, 183, 230);
	InsertChildObject(pRightCorner);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 267, 179, 270, &g_MainInterfaceTextureArchive, mi_main_back01_tga);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	InsertChildObject(pBottomBorder);

	_XImageStatic* pTitleLeftBorder = new _XImageStatic;
	pTitleLeftBorder->Create(8, 22, 10, 42, &g_MainInterfaceTextureArchive, mi_skillwin_tga);
	pTitleLeftBorder->SetClipRect(4, 154, 6, 174);
	InsertChildObject(pTitleLeftBorder);

	_XImageStatic* pTitleCenterBorder = new _XImageStatic;
	pTitleCenterBorder->Create(10, 22, 173, 42, &g_MainInterfaceTextureArchive, mi_skillwin_tga);
	pTitleCenterBorder->SetClipRect(6, 154, 106, 174);
	pTitleCenterBorder->SetScale(163.f/100.f, 1.0f);
	InsertChildObject(pTitleCenterBorder);

	_XImageStatic* pTitleRightBorder = new _XImageStatic;
	pTitleRightBorder->Create(173, 22, 175, 42, &g_MainInterfaceTextureArchive, mi_skillwin_tga);
	pTitleRightBorder->SetClipRect(106, 154, 108, 174);
	InsertChildObject(pTitleRightBorder);

	// 교환 리스트
	_XLISTBOX_STRUCTURE resourcestructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 9, 48 },		// 윈도우 좌표
		{ 167, 200 },	// 윈도우 사이즈
		_XDEF_NEWRESOURCEWINDOW_RESOURCELIST,			// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		20,												// 리스트 아이템 라인 피치
		10,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 200 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		255,												// 전체 리스트 갯수
		mi_chat_quick_tga,
		mi_chat_quick_tga,
		mi_chat_quick_tga,
		mi_chat_quick_tga,
		mi_chat_quick_tga
	};
	
	m_pResourceList = new _XListBox;
	m_pResourceList->Create(resourcestructure);
	
	_XVScrollBar* pScrollBar = m_pResourceList->GetScrollBarObject();
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
//	m_ResourceList->SetAlwaysShowScrollBarMode(TRUE);
//	m_ResourceList->SetSelectItemMouseHoverCallBack( _NPCResourceListBoxItemMouseHoverCallback );
	
	POINT columnoffset1 = { 0, 0 };
	POINT columnoffset2 = { 2, 2 };
	POINT columnoffset3 = { 38, 5 };
	m_pResourceList->SetColumnOffset(0, columnoffset1);				// column offset
	m_pResourceList->SetColumnOffset(1, columnoffset2);
	m_pResourceList->SetColumnOffset(2, columnoffset3);
	
	m_pResourceList->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_pResourceList->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_pResourceList->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	
	m_pResourceList->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_pResourceList->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_pResourceList->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	
	m_pResourceList->SetColumnWidth(0, 150);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_pResourceList->SetColumnWidth(1, 150);
	m_pResourceList->SetColumnWidth(2, 134);
	
	m_pResourceList->SetTrackWidth(134);	
	m_pResourceList->ShowWindow(FALSE);
	InsertChildObject(m_pResourceList);

	m_pEmptyItemBorder = new _XImageStatic;
	m_pEmptyItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, mi_characterwin_tga);
	m_pEmptyItemBorder->SetClipRect(191, 0, 223, 32);
	
	m_pCompleteItemBorder = new _XImageStatic;
	m_pCompleteItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, mi_characterwin_tga);
	m_pCompleteItemBorder->SetClipRect(191, 33, 223, 65);

	// 버튼 (56x20)
	_XBTN_STRUCTURE btnstruct = 
	{	 TRUE, 
		{ 23, 241 }, 
		{ 56, 20 }, 
		_XDEF_NEWRESOURCEWINDOW_EXCHANGE,
		g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ),
		&g_MainInterfaceTextureArchive 
	};

	m_pExchangeBtn = new _XButton;
	m_pExchangeBtn->Create( btnstruct );
	m_pExchangeBtn->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 191, 196, 247, 216 );
	m_pExchangeBtn->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  191, 169, 247, 189 );
	m_pExchangeBtn->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  191, 223, 247, 243 );
	m_pExchangeBtn->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCRESOURCE_2265 );	// _T("교환하기")
	m_pExchangeBtn->SetButtonTextColor( 0xFFE0D7BA );
	m_pExchangeBtn->ShowWindow(FALSE);
	InsertChildObject( m_pExchangeBtn );

	btnstruct.position.x = 100;
	btnstruct.commandid = _XDEF_NEWRESOURCEWINDOW_CLOSE;

	m_pCloseBtn = new _XButton;
	m_pCloseBtn->Create( btnstruct );
	m_pCloseBtn->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 191, 196, 247, 216 );
	m_pCloseBtn->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  191, 169, 247, 189 );
	m_pCloseBtn->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  191, 223, 247, 243 );
	m_pCloseBtn->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL );	// _T("취소")
	m_pCloseBtn->SetButtonTextColor( 0xFFE0D7BA );
	m_pCloseBtn->ShowWindow(FALSE);
	InsertChildObject( m_pCloseBtn );

	return TRUE;
}

void _XWindow_NPCResource_New::DestroyWindow( void )
{
	SAFE_DELETE(m_pEmptyItemBorder);
	SAFE_DELETE(m_pCompleteItemBorder);

	_XWindow::DestroyWindow();
}


void _XWindow_NPCResource_New::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));

		if(m_nSelectedTab == _TAB_LIST)
		{
			_XDrawSolidBar( m_WindowPosition.x+7,  m_WindowPosition.y+45, m_WindowPosition.x+177, m_WindowPosition.y+250 , D3DCOLOR_ARGB(127,0,0,0) );
			_XDrawRectAngle( m_WindowPosition.x+6,  m_WindowPosition.y+44, m_WindowPosition.x+177, m_WindowPosition.y+250 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		}
		else
		{
			_XDrawSolidBar( m_WindowPosition.x+9,  m_WindowPosition.y+48, m_WindowPosition.x+174, m_WindowPosition.y+234 , D3DCOLOR_ARGB(127,0,0,0) );
			_XDrawRectAngle( m_WindowPosition.x+8,  m_WindowPosition.y+47, m_WindowPosition.x+174, m_WindowPosition.y+234 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

			int column = 0;
			int row = 0;
			for( int i = 0; i < 16 ; i++ )
			{
				column = i%4;
				row	= i/4;
				m_pEmptyItemBorder->Draw(m_WindowPosition.x+20+(column*37), m_WindowPosition.y+87+(36*row));					
			}

			m_pEmptyItemBorder->Draw(m_WindowPosition.x+20, m_WindowPosition.y+53);
			
 			if( m_nSelectedResourceItem != -1 && m_nSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
 			{
 				int count = 0;
 				int column = 0;
 				int row = 0;
 				_XCollectionResourceItem* currentneeditem = NULL;
 				list <_XCollectionResourceItem*>::iterator iter_needitem;
 				for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_listNeedItem.begin() ;
					iter_needitem != g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
 				{
 					currentneeditem = *iter_needitem;
 					if(currentneeditem)
 					{				
 						column = count%4;
 						row = count/4;
 						g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+20+(column*37), m_WindowPosition.y+87+(36*row), currentneeditem);

						// Drag 하면 complete 표시
						if(m_tItemInfo[count+1].bDragComplete)
						{
							m_pCompleteItemBorder->Draw(m_WindowPosition.x+20+(column*37), m_WindowPosition.y+87+(36*row));

							// 제련 수치 표시
							if(_XGI_FC_WEAPON == GetEqualItemType(currentneeditem->Get_m_cType()))
							{
								if(g_pLocalUser->m_UserItemList[m_tItemInfo[count+1].nInvenNumber].Get_m_ucStrength() > 0)
								{
									g_XSmallFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
									g_XSmallFont->SetColor(0xFFFFD901);
									g_XSmallFont->PrintAlign(m_WindowPosition.x+20+(column*37)+30, m_WindowPosition.y+87+(36*row)+32-g_XSmallFont->GetHeight(), 1.0f,
										_XFONT_ALIGNTYPE_RIGHT, "+%d", g_pLocalUser->m_UserItemList[m_tItemInfo[count+1].nInvenNumber].Get_m_ucStrength());
									g_XSmallFont->Flush();
									g_XSmallFont->DisableGrowMode();
								}
							}
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+20+(column*37),  m_WindowPosition.y+87+(36*row),
								m_WindowPosition.x+20+30+(column*37), m_WindowPosition.y+87+30+(36*row) , D3DCOLOR_ARGB(127,0,0,0) );
						}
 
 						count++;
 					}
 				}

				// 보상 아이템
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+21, m_WindowPosition.y+54, &g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_RewardItem);
				m_pCompleteItemBorder->Draw(m_WindowPosition.x+21, m_WindowPosition.y+54);

				g_XBaseFont->SetBoldMode(TRUE);
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->PrintAlign(m_WindowPosition.x+113, m_WindowPosition.y+55, 1.0f, _XFONT_ALIGNTYPE_CENTER,"[%s]",
					_XGameItem::GetItemName(g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_RewardItem.Get_m_cType(),
					g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_RewardItem.Get_m_sID()));

				if(m_tItemInfo[0].bDragComplete)
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+113, m_WindowPosition.y+70, _XFONT_ALIGNTYPE_CENTER,"(1 / 1)");
				}
				else
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+113, m_WindowPosition.y+70, _XFONT_ALIGNTYPE_CENTER,"(0 / 1)");
				}

				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode(FALSE);
 			}
		}
	}
	
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;
	
	// TODO
	if(m_nSelectedTab == _TAB_LIST)
	{
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+27, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2259));	//교환가능목록
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
	}
	else
	{
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+27, _XGETINTERFACETEXT(ID_STRING_NPCRESOURCE_2266));	//필요물품
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
	}
	
}

BOOL _XWindow_NPCResource_New::Process(_XGUIObject*& pfocusobject)
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
					if(m_nSelectedTab == _TAB_ITEM)
					{
						// Drag & Drop
						if(g_pInventory_Window)
						{
							if(g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
								int index = 0;
								if(CheckItem(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(), g_pInventory_Window->m_MouseSlotItem.Get_m_sID(), g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, index))
								{
									SetItem(index, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
								}

								g_pInventory_Window->ResetMouseSlot();
							}
							else
							{
								// 툴팁 처리
								if( m_nSelectedResourceItem != -1 && m_nSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
								{
									BOOL bfinditem = FALSE;
									for(int i = 0 ; i < 17 ; ++i)
									{
										if(scrnpos->x >= m_tItemInfo[i].x && scrnpos->x <= m_tItemInfo[i].x+30
											&& scrnpos->z >= m_tItemInfo[i].y && scrnpos->z <= m_tItemInfo[i].y+30)
										{
											bfinditem = TRUE;
											break;
										}
									}

									if(bfinditem)
									{
										if(g_pDefaultTooltip_Window)
										{
											if(g_pDefaultTooltip_Window->SetTooltipMode(TMODE_NPCRESOURCEITEM, i, m_tItemInfo[i].cType, m_tItemInfo[i].sID))									
											{
												g_pDefaultTooltip_Window->ShowWindow(TRUE);
											}
										}
									}
									else
									{
										if(g_pDefaultTooltip_Window)
										{
											if(g_pDefaultTooltip_Window->GetShowStatus())
												g_pDefaultTooltip_Window->ShowWindow(FALSE);
										}
									}
								}
							}
						}
					}
				}					
				m_WindowMouseState = _XW_STATE_LEFTCLICK;				
			}
			if(mousestate->bButton[1])
			{
				if(CheckMousePosition())
				{
					if(m_nSelectedTab == _TAB_ITEM)
					{
						for(int i = 1 ; i < 17 ; ++i)
						{
							if(scrnpos->x >= m_tItemInfo[i].x && scrnpos->x <= m_tItemInfo[i].x+30
								&& scrnpos->z >= m_tItemInfo[i].y && scrnpos->z <= m_tItemInfo[i].y+30)
							{
								m_tItemInfo[i].bDragComplete = FALSE;
								m_tItemInfo[i].nInvenNumber = 0;
								break;
							}
						}
					}
				}

				m_WindowMouseState = _XW_STATE_RIGHTCLICK;
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

	if(CheckAllResourceItem())
	{
		if(!m_pExchangeBtn->GetEnableStatus())
			m_pExchangeBtn->EnableWindow(TRUE);
	}
	else
	{
		if(m_pExchangeBtn->GetEnableStatus())
			m_pExchangeBtn->EnableWindow(FALSE);
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_NPCResource_New::ShowWindow(BOOL show)
{
	if(show)
	{
		ChangeTab(_TAB_LIST);	// 새로 열릴때는 리스트부터.

		if(g_pInventory_Window)
		{
			g_pInventory_Window->SetNPCResource_New_WindowPtr(this);

			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
		
		if(g_pDefaultTooltip_Window)
			g_pDefaultTooltip_Window->SetNewNPCResourceWindowPtr(this);
	}
	else
	{
		if(g_pInventory_Window && g_pInventory_Window->m_bLockInventory)
			g_pInventory_Window->m_bLockInventory = FALSE;
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_NPCResource_New::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow(m_WindowPosition.x+1, m_WindowPosition.y+18, m_WindowPosition.x+180, m_WindowPosition.y+270);
}


void _XWindow_NPCResource_New::SetResourceList(void)
{	
	if( g_ResourceManager.m_TotalCountResourceItem == 0 )  return;
	if( g_ResourceManager.m_listResourcePackage.empty() ) return;

	m_pResourceList->DeleteAllItem();
	m_pResourceList->SetScrollPos(0);
	
	int count = 0;
	int lifeskillindex = g_MainInterfaceTextureArchive.FindResource( g_MI_LifeSkillTextureName);
	RECT rect1 = {0, 32, 150, 52}; 
	RECT rect2 = {126, 53, 158, 71};// 자원 
	_XResourcePackage* currentpackage = NULL;
	list <_XResourcePackage*>::iterator iter_package;
	for(iter_package = g_ResourceManager.m_listResourcePackage.begin() ; iter_package != g_ResourceManager.m_listResourcePackage.end() ; iter_package++)
	{
		currentpackage = *iter_package;
		if(currentpackage)
		{
			if( currentpackage->m_ResourcePackageIndex == m_nSelectedResourcePackageIndex )
			{
				int resourceindex = 0;
				list <int>::iterator iter_resourceindex;
				for(iter_resourceindex = currentpackage->m_listResourceItemIndex.begin() ; iter_resourceindex != currentpackage->m_listResourceItemIndex.end() ; iter_resourceindex++)
				{
					resourceindex = *iter_resourceindex;
					if( resourceindex != -1 && resourceindex < g_ResourceManager.m_TotalCountResourceItem )
					{
						m_pResourceList->InsertListItem(_T(""), 0, g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceIndex+1);		
						m_pResourceList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_pResourceList->SetItemAttrib(count, 0, lifeskillindex , rect1);
						m_pResourceList->SetItemAttrib(count, 1, lifeskillindex , rect2);
						
						if( g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceName[0] != 0 )
							m_pResourceList->SetItemText(count, 2, g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceName );
																								
						if(SetNeedItemComplete(resourceindex))
						{
							m_pResourceList->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 244, 255, 1));
							m_pResourceList->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 244, 255, 1));
						}
						else
						{
							m_pResourceList->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_pResourceList->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
						}
						count++;
					}
				}
				
				break;
			}
		}
	}
}

void _XWindow_NPCResource_New::ChangeTab(int tab)
{
	if(tab == _TAB_LIST)
	{
		m_nSelectedTab = tab;

		m_pResourceList->ShowWindow(TRUE);
		SetResourceList();

		m_pExchangeBtn->ShowWindow(FALSE);
		m_pCloseBtn->ShowWindow(FALSE);

		memset(m_tItemInfo, 0, sizeof(m_tItemInfo));
	}
	else
	{
		m_nSelectedTab = tab;

		m_pResourceList->ShowWindow(FALSE);

		m_pExchangeBtn->ShowWindow(TRUE);
		m_pCloseBtn->ShowWindow(TRUE);
	}
}

void _XWindow_NPCResource_New::EnableExchangeButton(BOOL enable)
{
	m_pExchangeBtn->EnableWindow(enable);
}

void _XWindow_NPCResource_New::SetItemBasicInfo()
{
	int index = 1;
	int column = 0, row = 0;

	_XCollectionResourceItem* currentneeditem = NULL;
	list <_XCollectionResourceItem*>::iterator iter_needitem;
	for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
	{
		currentneeditem = *iter_needitem;
		if(currentneeditem)
		{
			column = (index-1)%4;
			row = (index-1)/4;

			m_tItemInfo[index].cType = currentneeditem->Get_m_cType();
			m_tItemInfo[index].sID = currentneeditem->Get_m_sID();
			m_tItemInfo[index].ucCount = currentneeditem->Get_m_ucCount();

			m_tItemInfo[index].x = m_WindowPosition.x+20+(column*37);
			m_tItemInfo[index].y = m_WindowPosition.y+87+(row*36);

			m_tItemInfo[index].bDragComplete = FALSE;
			m_tItemInfo[index].nInvenNumber = 0;

			index++;
		}
	}

	// 보상 아이템
	m_tItemInfo[0].cType = g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_RewardItem.Get_m_cType();
	m_tItemInfo[0].sID = g_ResourceManager.m_arrayResourceItem[m_nSelectedResourceItem].m_RewardItem.Get_m_sID();

	m_tItemInfo[0].x = m_WindowPosition.x+21;
	m_tItemInfo[0].y = m_WindowPosition.y+54;

	m_tItemInfo[0].bDragComplete = FALSE;
	m_tItemInfo[0].nInvenNumber = 0;

	if(SetNeedItemComplete(m_nSelectedResourceItem))
	{
		m_tItemInfo[0].bDragComplete = TRUE;
	}
}

BOOL _XWindow_NPCResource_New::CheckAllResourceItem()
{
	for(int i = 1 ; i < 17 ; ++i)
	{
		if(m_tItemInfo[i].cType >= 0 && m_tItemInfo[i].sID > 0)
		{
			if(m_tItemInfo[i].bDragComplete == FALSE)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL _XWindow_NPCResource_New::CheckItem(int cType, int sID, int invennumber, int& index)
{
	for(int i = 1 ; i < 17 ; ++i)
	{
		if(m_tItemInfo[i].cType == cType && m_tItemInfo[i].sID == sID)
		{
			if(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucCount() >= m_tItemInfo[i].ucCount)
			{
				//Author : 양희왕 //breif : 제련된 무기에 대한 질의
				switch(cType)
				{
				case _XGI_FC_WEAPON :
				case _XGI_FC_WEAPON2 :
				case _XGI_FC_WEAPON3 :
					{
						if(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucStrength() > 0 
							|| g_pLocalUser->m_UserItemList[invennumber].m_usCurProtect != 0 
							|| g_pLocalUser->m_UserItemList[invennumber].m_usCurMaxProtect != _XGameItem::GetNature(cType, sID) 
							|| g_pLocalUser->m_UserItemList[invennumber].m_usUsedProtect != 0)
						{
							m_bStrength = true;
						}
					}
					break;
				}

				index = i;
				return TRUE;
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LIFESKILL_2175), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				return FALSE;
			}
		}
	}

	return FALSE;
}

void _XWindow_NPCResource_New::SetItem(int index, int invennumber)
{
	m_tItemInfo[index].bDragComplete = TRUE;
	m_tItemInfo[index].nInvenNumber = invennumber;
}

BOOL _XWindow_NPCResource_New::CheckInventorySpace()
{	
	int		emptyslotcount = 0;
	int		needinvencount = 1; // 자원의 보상 공간은 한칸만 있으면 된다.
	
	for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
		{
			emptyslotcount++;
			if(emptyslotcount == needinvencount)
			{
				return TRUE;
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
				return TRUE;
			}
		}											
	}
#endif
	
	return FALSE;
}

void _XWindow_NPCResource_New::GetItemSlotIndex(int* itemslotindex)
{
	for(int i = 1 ; i < 17 ; ++i)
	{
		if(m_tItemInfo[i].cType >= 0 && m_tItemInfo[i].sID > 0)
		{
			itemslotindex[i-1] = m_tItemInfo[i].nInvenNumber;
		}
	}
}

BOOL _XWindow_NPCResource_New::SetNeedItemComplete(int index)
{
	bool allcomplete = true;

	_XCollectionResourceItem* currentneeditem = NULL;
	list <_XCollectionResourceItem*>::iterator iter_needitem;
	for(iter_needitem = g_ResourceManager.m_arrayResourceItem[index].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[index].m_listNeedItem.end() ; iter_needitem++)
	{
		currentneeditem = *iter_needitem;
		if(currentneeditem)
		{
			// 교환 가능 체크
			int itemtotalcount = 0;
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == currentneeditem->Get_m_cType() 
					&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == currentneeditem->Get_m_sID())
					itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();
			}

		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == currentneeditem->Get_m_cType() 
					&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == currentneeditem->Get_m_sID())
					itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();												
			}
		#endif
			if(itemtotalcount >= currentneeditem->Get_m_ucCount())
			{
				currentneeditem->m_bComplete = true;
			}
			else
			{
				currentneeditem->m_bComplete = false;
			}
		}

		allcomplete &= currentneeditem->m_bComplete;
	}

	return allcomplete;
}