// XWindow_NPCCollection.cpp: implementation of the _XWindow_NPCCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XMessageWindow.h"
#include "XWindow_NPCCollection.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCCollection::_XWindow_NPCCollection()
{
	m_imageSelectedRewardItemBorder = NULL;
	m_imageRewardItemBorder			= NULL;
	m_listboxCollectionItem			= NULL;
	m_pSelectedCollectionItem		= NULL;
	m_indexSelectedRewardSet		= -1;
	m_bProcessSelectRewardSet		= false;
	m_bRewardSuccess				= false;
	
	for( int i = 0; i < 4; i++)
	{
		m_btnRewardSet[i] = NULL;
		
	}
	
	m_nOwnerNPCId	= 0;

}

_XWindow_NPCCollection::~_XWindow_NPCCollection()
{
	SAFE_DELETE(m_imageSelectedRewardItemBorder);
	SAFE_DELETE(m_imageRewardItemBorder);

}

BOOL _XWindow_NPCCollection::Initialize()
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
	_XBTN_STRUCTURE helpstruct = { TRUE, { 348, 2 }, { 12, 12 }, _XDEF_NPCCOLLECTION_HELP_BTN, charinfointerface,charinfointerface,charinfointerface, &g_MainInterfaceTextureArchive };
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

	// 교환 가능 목록 바탕 1 ( 45 X 20 )  //( 224 X 20 )
	_XImageStatic* pMainTitleLeftBorder = new _XImageStatic;
	pMainTitleLeftBorder->Create(9, 22, 54, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pMainTitleLeftBorder->SetClipRect(4, 178, 49, 199);
	InsertChildObject(pMainTitleLeftBorder);
		
	// 교환 가능 목록 바탕 2 ( 130 X 20 )  //( 224 X 20 )
	_XImageStatic* pMainTitleCenterBorder = new _XImageStatic;
	pMainTitleCenterBorder->Create(54, 22, 184, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pMainTitleCenterBorder->SetClipRect(49, 178, 59, 199);
	pMainTitleCenterBorder->SetScale(13.0f, 1.0f);
	InsertChildObject(pMainTitleCenterBorder);
		
	// 교환 가능 목록 바탕 3 ( 49 X 20 )  //( 224 X 20 )
	_XImageStatic* pMainTitleRightBorder = new _XImageStatic;
	pMainTitleRightBorder->Create(184, 22, 223, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pMainTitleRightBorder->SetClipRect(59, 178, 108, 199);
	InsertChildObject(pMainTitleRightBorder);
	
	
	// 보상 물품 목록 바탕 1 ( 45 X 20 )  //( 120 X 20 )
	_XImageStatic* pSubTitleLeftBorder = new _XImageStatic;
	pSubTitleLeftBorder->Create(238, 22, 283, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pSubTitleLeftBorder->SetClipRect(4, 178, 49, 199);
	InsertChildObject(pSubTitleLeftBorder);
	
	// 보상 물품 목록 바탕 2 ( 130 X 20 )  //( 224 X 20 )
	_XImageStatic* pSubTitleCenterBorder = new _XImageStatic;
	pSubTitleCenterBorder->Create(283, 22, 309, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pSubTitleCenterBorder->SetClipRect(49, 178, 59, 199);
	pSubTitleCenterBorder->SetScale( 2.6f, 1.0f);
	InsertChildObject(pSubTitleCenterBorder);
	
	// 보상 물품 목록 바탕 3 ( 49 X 20 )  //( 224 X 20 )
	_XImageStatic* pSubTitleRightBorder = new _XImageStatic;
	pSubTitleRightBorder->Create(309, 22, 358, 42, &g_MainInterfaceTextureArchive, skillinterface);
	pSubTitleRightBorder->SetClipRect(59, 178, 108, 199);
	InsertChildObject(pSubTitleRightBorder);
	
	
	// 수집을 표시하는 리스트 박스
	_XLISTBOX_STRUCTURE collectionstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 9, 48 },		// 윈도우 좌표
		{ 223, 211 },	// 윈도우 사이즈
		_XDEF_NPCCOLLECTION_COLLECTIONLISTBOX,			// 윈도우 command 아이디
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
	
	m_listboxCollectionItem = new _XListBox;
	m_listboxCollectionItem->Create(collectionstructure);
	
	_XVScrollBar* pScrollBar = m_listboxCollectionItem->GetScrollBarObject();
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
	m_listboxCollectionItem->SetAlwaysShowScrollBarMode(TRUE);
	
	POINT columnoffset1 = { 0, 0 };
	POINT columnoffset2 = { 2, 2 };
	POINT columnoffset3 = { 38, 5 };
	m_listboxCollectionItem->SetColumnOffset(0, columnoffset1);				// column offset
	m_listboxCollectionItem->SetColumnOffset(1, columnoffset2);
	m_listboxCollectionItem->SetColumnOffset(2, columnoffset3);
	
	m_listboxCollectionItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxCollectionItem->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxCollectionItem->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	
	m_listboxCollectionItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxCollectionItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxCollectionItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxCollectionItem->SetColumnWidth(0, 200);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxCollectionItem->SetColumnWidth(1, 200);
	m_listboxCollectionItem->SetColumnWidth(2, 200);

	m_listboxCollectionItem->SetTrackWidth(200);	
	m_listboxCollectionItem->ShowWindow(TRUE);
	InsertChildObject(m_listboxCollectionItem);
	
	// 보상 세트 버튼 
	_XBTN_STRUCTURE rewardsetbtnstruct = 
	{
		TRUE, {243, 90}, {114, 42}, _XDEF_NPCCOLLECTION_REWARDSET01_BTN,
			lifeskillindex,
			lifeskillindex,
			lifeskillindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnRewardSet[0] = new _XButton;
	m_btnRewardSet[0]->Create(rewardsetbtnstruct);
	m_btnRewardSet[0]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 73, 115, 115 );
	m_btnRewardSet[0]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 116, 115, 158 );
	m_btnRewardSet[0]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 159, 115, 201 );
	m_btnRewardSet[0]->ShowWindow(FALSE);
	InsertChildObject(m_btnRewardSet[0]);
	for(int i = 1 ; i < 4 ; i++)
	{
		rewardsetbtnstruct.position.y = 90+ (42*i);
		rewardsetbtnstruct.commandid++;
		m_btnRewardSet[i] = new _XButton;
		m_btnRewardSet[i]->Create(rewardsetbtnstruct);
		m_btnRewardSet[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 73, 115, 115 );
		m_btnRewardSet[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 116, 115, 158 );
		m_btnRewardSet[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 159, 115, 201 );
		m_btnRewardSet[i]->ShowWindow(FALSE);
		InsertChildObject(m_btnRewardSet[i]);
	}
	
	
	m_imageRewardItemBorder = new _XImageStatic;
	m_imageRewardItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwinlindex);
	m_imageRewardItemBorder->SetClipRect(191, 0, 223, 32);
	
	m_imageSelectedRewardItemBorder = new _XImageStatic;
	m_imageSelectedRewardItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwinlindex);
	m_imageSelectedRewardItemBorder->SetClipRect(191, 33, 223, 65);

	
	return TRUE;
}
void _XWindow_NPCCollection::ShowWindow(BOOL show)
{
	if(show)
	{
		SetListItem();
		if( g_pDefaultTooltip_Window )
			g_pDefaultTooltip_Window->SetNPCCollectionWindowPtr( this );
	}
	else
	{
		m_bProcessSelectRewardSet		= false;
		m_pSelectedCollectionItem = NULL;
		for( int i = 0; i < 4; i++ )
		{
			m_btnRewardSet[i]->ShowWindow(FALSE);
		}
		m_nOwnerNPCId = 0;
	}
	
	_XWindow::ShowWindow(show);
}
void _XWindow_NPCCollection::Draw(_XGUIObject*& pfocusobject)
{	
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));
		
		// 수집 리스트 바탕 ( 223 X 211 )
		_XDrawSolidBar( m_WindowPosition.x+9,  m_WindowPosition.y+48, m_WindowPosition.x+232, m_WindowPosition.y+259 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+8,  m_WindowPosition.y+47, m_WindowPosition.x+232, m_WindowPosition.y+259 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

		// 수집 리스트 바탕 ( 120 X 34 )
		_XDrawSolidBar( m_WindowPosition.x+239,  m_WindowPosition.y+48, m_WindowPosition.x+358, m_WindowPosition.y+81 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+238,  m_WindowPosition.y+47, m_WindowPosition.x+358, m_WindowPosition.y+81 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		// 수집 리스트 바탕 ( 120 X 173)
		_XDrawSolidBar( m_WindowPosition.x+239,  m_WindowPosition.y+88, m_WindowPosition.x+358, m_WindowPosition.y+259 , D3DCOLOR_ARGB(127,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+238,  m_WindowPosition.y+87, m_WindowPosition.x+358, m_WindowPosition.y+259 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

		// 보상물 아이템을 그린다.
		if( m_pSelectedCollectionItem )
		{
			if( !m_pSelectedCollectionItem->m_listRewardSet.empty() )
			{
				_XCollectionRewardSet* currentrewardset = NULL;
				_XCollectionResourceItem* currentrewarditem = NULL;
				list <_XCollectionRewardSet*>::iterator iter_rewardset;
				int column = 0;
				int row = 0;
				for(iter_rewardset = m_pSelectedCollectionItem->m_listRewardSet.begin() ; iter_rewardset != m_pSelectedCollectionItem->m_listRewardSet.end() ; iter_rewardset++)
				{
					currentrewardset = *iter_rewardset;
					if(currentrewardset)
					{	
						if( !currentrewardset->m_listRewardItem.empty() )
						{
							list <_XCollectionResourceItem*>::iterator iter_rewarditem;
							for(iter_rewarditem = currentrewardset->m_listRewardItem.begin() ; iter_rewarditem != currentrewardset->m_listRewardItem.end() ; iter_rewarditem++)
							{
								currentrewarditem = *iter_rewarditem;
								if(currentrewarditem)
								{				
									g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+249+(column*35), m_WindowPosition.y+96+(42*row), currentrewarditem);
									m_imageSelectedRewardItemBorder->Draw(m_WindowPosition.x+248+(column*35), m_WindowPosition.y+95+(42*row));					
								}
								column++;
							}
						}						
					}
					column = 0;
					row++;
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
	if( !m_bRewardSuccess )
	{
		g_XBaseFont->Puts( m_WindowPosition.x+13,  m_WindowPosition.y+53, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2258) ); 
		g_XBaseFont->Puts( m_WindowPosition.x+14,  m_WindowPosition.y+53, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2258) ); 
	}
	g_XBaseFont->Puts( m_WindowPosition.x+13,  m_WindowPosition.y+26, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2259) ); 
	g_XBaseFont->Puts( m_WindowPosition.x+243,  m_WindowPosition.y+26, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2260) ); 
	g_XBaseFont->Puts( m_WindowPosition.x+243,  m_WindowPosition.y+52, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2261) ); 
	g_XBaseFont->Puts( m_WindowPosition.x+243,  m_WindowPosition.y+66, _XGETINTERFACETEXT(ID_STRING_NPCCOLLECTION_2262) ); 
	g_XBaseFont->Flush();
	
}

BOOL _XWindow_NPCCollection::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;
	
	// TODO	
	if( m_pSelectedCollectionItem )
	{
		if( !m_pSelectedCollectionItem->m_listRewardSet.empty() )
		{
			if(CheckMousePosition())
			{	
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
				
				if(scrnpos->x >= m_WindowPosition.x+238 &&	scrnpos->x <= m_WindowPosition.x+358 && scrnpos->z >= m_WindowPosition.y+87 && scrnpos->z <= m_WindowPosition.y+259)
				{			
					bool bfindrewarditem = false;
					int type = -1;
					int id = 0;
					int index = -1;
					int column = 0;
					int row = 0;
					for( int i = 0; i< 12; i++ )
					{
						column = i%3;	// 보상 아이템 인덱스
						row = i/3;		// 보상 세트 인덱스
						if(scrnpos->x >= m_WindowPosition.x+248+(column*35) &&	scrnpos->x <= m_WindowPosition.x+280+(column*35) 
							&& scrnpos->z >= m_WindowPosition.y+95+(42*row) && scrnpos->z <= m_WindowPosition.y+127+(42*row))
						{
							_XCollectionRewardSet* currentrewardset = NULL;
							_XCollectionResourceItem* currentrewarditem = NULL;
							list <_XCollectionRewardSet*>::iterator iter_rewardset;
							for(iter_rewardset = m_pSelectedCollectionItem->m_listRewardSet.begin() ; iter_rewardset != m_pSelectedCollectionItem->m_listRewardSet.end() ; iter_rewardset++)
							{
								currentrewardset = *iter_rewardset;
								if(currentrewardset && (currentrewardset->m_Index == row) )
								{	
									if( !currentrewardset->m_listRewardItem.empty() )
									{
										list <_XCollectionResourceItem*>::iterator iter_rewarditem;
										for(iter_rewarditem = currentrewardset->m_listRewardItem.begin() ; iter_rewarditem != currentrewardset->m_listRewardItem.end() ; iter_rewarditem++)
										{
											currentrewarditem = *iter_rewarditem;
											if( currentrewarditem && (currentrewarditem->m_Index==column) )
											{
												type = currentrewarditem->Get_m_cType();
												id = currentrewarditem->Get_m_sID();
												index = i;
												bfindrewarditem = true;
												break;
											}
										}
									}
									break;												
								}
							}
							break;
						}	
					}
					
					if( bfindrewarditem )
					{
						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_NPCCOLLECTIONITEM, index, type, id ) )									
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

	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_NPCCollection::MoveWindow(int X, int Y)
{			
	_XWindow::MoveWindow(X, Y);

	m_BorderTile.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y+18, m_WindowPosition.x+365, m_WindowPosition.y+268 );	
}

void _XWindow_NPCCollection::SetListItem(void)
{	
	if( !m_listboxCollectionItem ) return;
	if( m_bProcessSelectRewardSet ) return;
	
	m_listboxCollectionItem->DeleteAllItem();
	
	if( !g_CollectionManager.m_listCollectionItem.empty() )
	{
		int lifeskillindex = g_MainInterfaceTextureArchive.FindResource( g_MI_LifeSkillTextureName);
		RECT rect1 = {0, 32, 204, 52}; 
		RECT rect2 = {0, 53, 32, 71};// 하품 
		_XCollectionItem* currentcollection = NULL;
		int count = 0 ;	
		list <_XCollectionItem*>::iterator iter_collection;
		for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
		{
			currentcollection = *iter_collection;
			if(currentcollection)
			{
#ifdef _XTS_COLLECTION_PACKAGE_071023_KUKURI
				if(!IsMyOwnerPackage(currentcollection->m_CollectionIndex))
					continue;
#endif
				
				if( !currentcollection->m_bComplete ) // 교환 가능한 상태의 수집 리스트만 보여준다.
				{	
					if( currentcollection->m_bEnableExchage ) // 교환 가능한 상태의 수집 리스트만 보여준다.
					{
						switch(currentcollection->m_CollectionType) {
						case 1: // 하품
							{
								rect2.left = 0;
								rect2.right = 32;
							}
							break;
						case 2: // 중품
							{
								rect2.left = 31;
								rect2.right = 31+32;
							}
							break;
						case 3: // 상픔
							{
								rect2.left = 62;
								rect2.right = 62+32;
							}
							break;
						case 4: // 명품
							{
								rect2.left = 94;
								rect2.right = 94+32;
							}
							break;
						case 5: // 보물
							{
								rect2.left = 158;
								rect2.right = 158+32;
							}
							break;
						}
						
						m_listboxCollectionItem->InsertListItem(_T(""), 0, currentcollection->m_CollectionIndex+1);		
						m_listboxCollectionItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxCollectionItem->SetItemAttrib(count, 0, lifeskillindex , rect1);
						m_listboxCollectionItem->SetItemAttrib(count, 1, lifeskillindex , rect2);
						
						if( currentcollection->m_CollectionName[0] != 0 )
							m_listboxCollectionItem->SetItemText(count, 2, currentcollection->m_CollectionName );
						
						m_listboxCollectionItem->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
						m_listboxCollectionItem->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
						currentcollection->m_indexCollectionList = count;
						count++;			
					}
				}
			}
		}
		if( count == 0 )
		{
			m_bRewardSuccess = false;
		}
		else
		{
			m_bRewardSuccess = true;
		}
	}
}

void _XWindow_NPCCollection::SetSelectedCollectionItemIndex(int index)
{
	if( !g_CollectionManager.m_listCollectionItem.empty() )
	{
		index--;//1을 뺀다.		
		int count = 0 ;	
		list <_XCollectionItem*>::iterator iter_collection;
		for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
		{
			_XCollectionItem* currentcollection = *iter_collection;
			if(currentcollection)
			{				
				if( currentcollection->m_CollectionIndex == index )
				{
					for( int i = 0; i < 4; i++ )
					{
						m_btnRewardSet[i]->ShowWindow(FALSE);
					}

					m_pSelectedCollectionItem = currentcollection;
					for(  i = 0; i < m_pSelectedCollectionItem->m_listRewardSet.size(); i++)
					{
						m_btnRewardSet[i]->ShowWindow(TRUE);
					}
					break;
				}
			}
		}
	}
	
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )						
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}
}

bool _XWindow_NPCCollection::CheckInventorySpace(int rewardsetindex)
{
	if( !m_pSelectedCollectionItem ) return false;
	if( m_pSelectedCollectionItem->m_listRewardSet.empty() ) return false;

	int		emptyslotcount = 0;
	int		needinvencount = 0;

	_XCollectionRewardSet* currentrewardset = NULL;
	_XCollectionResourceItem* currentrewarditem = NULL;
	list <_XCollectionRewardSet*>::iterator iter_rewardset;
	for(iter_rewardset = m_pSelectedCollectionItem->m_listRewardSet.begin() ; iter_rewardset != m_pSelectedCollectionItem->m_listRewardSet.end() ; iter_rewardset++)
	{
		currentrewardset = *iter_rewardset;
		if(currentrewardset)
		{				
			if( currentrewardset->m_Index == rewardsetindex )
			{
				needinvencount = currentrewardset->m_listRewardItem.size();
				break;
			}						
		}
	}

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

BOOL _XWindow_NPCCollection::IsMyOwnerPackage(int collectionid)
{
#ifdef _XTS_COLLECTION_PACKAGE_071023_KUKURI

	vector<int>::iterator iter_list;
	for(iter_list = g_CollectionManager.m_mapCollectionPackage[m_nOwnerNPCId]->vCollectionId.begin() ; 
	iter_list != g_CollectionManager.m_mapCollectionPackage[m_nOwnerNPCId]->vCollectionId.end() ; ++iter_list)
	{
		int id = *iter_list;
		if(collectionid == id)
		{
			return TRUE;
		}
	}

	return FALSE;

#else

	return TRUE;

#endif
}