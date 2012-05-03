// XWindow_DefaultTooltip.cpp: implementation of the _XWindow_DefaultTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "xwindow_warehouse.h"
#include "XWindow_MasterySkill.h"
#include "XWindow_DefaultTooltip.h"
#include "XWindow_PersonalStore.h"
#include "xwindow_repairitem.h"
#include "XWindow_Contribution.h"
#include "XWindow_TargetInfo.h"
#include "XWindow_CashItem.h"
#include "XWindow_NPCCollection.h"
#include "XWindow_NPCResource.h"
#include "XWindow_NPCResource_New.h"
#include "XWindow_Inchant.h"
#include "Xwindow_SmeltItem.h"
#include "XCharacterState.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern _XImageStatic		g_InventoryIconImage;
extern unsigned long		g_MoneyColor[6];
_XWindow_DefaultTooltip*	g_pDefaultTooltip_Window = NULL;

extern int CheckCashItem(char cType, short sID);

static int DT_ROW_INTERVAL = 16;
static int DT_LINE_INTERVAL = 5;


_XWindow_DefaultTooltip::_XWindow_DefaultTooltip()
:	m_nPriceMode(0),
	m_pClanPointImage(NULL)
{
	g_pDefaultTooltip_Window	= this;

	m_CurrentTooltipMode		= TMODE_NONE;	
	m_TooltipSize.cx			= 0;
	m_TooltipSize.cy			= 0;
	m_ExpandTooltipSize.cx		= 0;
	m_ExpandTooltipSize.cy		= 0;
	m_SetItemExpandTooltipSize.cx = 0;
	m_SetItemExpandTooltipSize.cy = 0;
	m_CurrentSlotNumber			= -1;
	m_ItemType					= -1;
	m_ItemId					= -1;
	m_CurrentItemSocketCount	= -1;
	m_CurrentItemSockekRealCount = 0;
	m_CurrentDrawPosY			= 0;
	m_DeleteSkillId				= 1;

	for( int i = 0; i < 4; i++ )
		m_TooltipEdgeImage[i] = NULL;
	for( i = 0; i < 2; i++ )
		m_TooltipIconBoxImage[i] = NULL;

	m_SkillExp					= NULL;

	m_MoneyImage				= NULL;
	m_ContributionImage			= NULL;

	m_pNPCTradeWindow			= NULL;
	m_pPCTradeWindow			= NULL;
	m_pWarehouseWindow			= NULL;
	m_pRepairItem_Window		= NULL; 
	m_pEnterBossRoom_Window		= NULL;
	m_pSocketPlugIn_Window		= NULL;	
	m_pNPCScript_Window			= NULL;
	m_pPersonalStore_Window		= NULL;
	m_pContribution_Window		= NULL;
	m_pTargetInfo_Window		= NULL;
	m_pCashItem_Window			= NULL;
	m_pInchant_Window			= NULL;
}

_XWindow_DefaultTooltip::~_XWindow_DefaultTooltip()
{
	DestroyWindow();
}

BOOL _XWindow_DefaultTooltip::Initialize(void)
{
	SetWindowMoveMode(_XWMOVE_FIXED);
	ResetTooltipInfo();	

	// Item 관련 이미지------------------------------------------------------------------------------=	
	for( int i = 0; i < 4; i++ )
	{
		SAFE_DELETE( m_TooltipEdgeImage[i] );		 
		m_AddSocketItemList[i].Reset();
	}
	for( i = 0; i < 2; i++ )
		SAFE_DELETE( m_TooltipIconBoxImage[i] );

	SAFE_DELETE( m_MoneyImage );
	SAFE_DELETE( m_ContributionImage );
	
	int resourceindex = g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName);
	
	for( i = 0; i < 4; i++ )
		m_TooltipEdgeImage[i]= new _XImageStatic;

	m_TooltipEdgeImage[0]->Create(0, 0, 8, 8, &g_MainInterfaceTextureArchive, resourceindex );
	m_TooltipEdgeImage[0]->SetClipRect(242, 127, 251, 136);
	m_TooltipEdgeImage[1]->Create(0, 0, 8, 8, &g_MainInterfaceTextureArchive, resourceindex );
	m_TooltipEdgeImage[1]->SetClipRect(242, 137, 251, 146);
	m_TooltipEdgeImage[2]->Create(0, 0, 8, 8, &g_MainInterfaceTextureArchive, resourceindex );
	m_TooltipEdgeImage[2]->SetClipRect(242, 157, 251, 166);
	m_TooltipEdgeImage[3]->Create(0, 0, 8, 8, &g_MainInterfaceTextureArchive, resourceindex );
	m_TooltipEdgeImage[3]->SetClipRect(242, 147, 251, 156);

	resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	m_TooltipIconBoxImage[0] = new _XImageStatic;
	m_TooltipIconBoxImage[0]->Create(0, 0, 31, 31, &g_MainInterfaceTextureArchive, resourceindex );
	m_TooltipIconBoxImage[0]->SetClipRect(214, 65, 246, 97);

	m_TooltipIconBoxImage[1] = new _XImageStatic;
	m_TooltipIconBoxImage[1]->Create(0, 0, 31, 31, &g_MainInterfaceTextureArchive, resourceindex );
	m_TooltipIconBoxImage[1]->SetClipRect(214, 31, 246, 63);

	resourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	m_MoneyImage = new _XImageStatic;	
	m_MoneyImage->Create( 0, 0, 15, 12, &g_MainInterfaceTextureArchive, resourceindex );
	m_MoneyImage->SetClipRect(185, 6, 200, 18);

	m_ContributionImage= new _XImageStatic;	
	m_ContributionImage->Create( 0, 0, 15, 12, &g_MainInterfaceTextureArchive, resourceindex );
	m_ContributionImage->SetClipRect(201, 5, 216, 17);

	m_pClanPointImage = new _XImageStatic;
	m_pClanPointImage->Create(0, 0, 13, 16, &g_MainInterfaceTextureArchive, resourceindex);
	m_pClanPointImage->SetClipRect(202, 3, 215, 19);

	_XSTICKGAUGE_STRUCTURE stickstruct = 
	{
		TRUE, {73, 32}, {104, 6}, g_MainInterfaceTextureArchive.FindResource("MI_Characterwin.tga"), -1, &g_MainInterfaceTextureArchive
	};
	m_SkillExp = new _XStickGauge;
	m_SkillExp->Create(stickstruct);
	m_SkillExp->SetImageClipRect(_XGAUGEIMAGE_BORDER, 0, 42, 104, 48);
	m_SkillExp->SetFactorDraw(FALSE);

	resourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	_XBTN_STRUCTURE delectskillbtn = 
	{
		TRUE, 
		{0, 0}, {60, 24},
		_XDEF_WTITLE_DT_DELECTSKILLBTN,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};	
	
	m_DeleteSkillBtn = new _XButton;
	m_DeleteSkillBtn->Create(delectskillbtn);
	m_DeleteSkillBtn->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 194, 249, 218 );
	m_DeleteSkillBtn->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  189, 167, 249, 191 );
	m_DeleteSkillBtn->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  189, 221, 249, 245 );
	m_DeleteSkillBtn->SetButtonText( _XGETINTERFACETEXT(ID_STRING_MESSENGER_MAILDEL) );
	m_DeleteSkillBtn->SetGrowTextMode( TRUE );
	m_DeleteSkillBtn->ShowWindow(FALSE);	
	InsertChildObject(m_DeleteSkillBtn);
	
	// ----------------------------------------------------------------------------------------------=
	return TRUE;
}

void _XWindow_DefaultTooltip::DestroyWindow(void)
{
	SAFE_DELETE(m_SkillExp);
	
	for( int i = 0; i < 4; i++ )
		SAFE_DELETE(m_TooltipEdgeImage[i]);
	for( i = 0; i < 2; i++ )
		SAFE_DELETE(m_TooltipIconBoxImage[i]);

	SAFE_DELETE( m_MoneyImage );
	SAFE_DELETE( m_ContributionImage );
	SAFE_DELETE(m_pClanPointImage);
}

void _XWindow_DefaultTooltip::Draw(_XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)
		return;
	
 	if( g_bShowItemMallWindow ) return;
	
	_XDrawSolidBar(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_TooltipSize.cx, m_WindowPosition.y+m_TooltipSize.cy, D3DCOLOR_ARGB(178,0,0,0) );

	switch( m_CurrentTooltipMode ) 
	{	
	case TMODE_DEFAULTITEM :	
	case TMODE_CINFOITEM :
	case TMODE_NPCTRADE_INVENITEM :	
	case TMODE_NPCTRADE_NPCITEM :
	case TMODE_WAREHOUSE_WAREHOUSEITEM :
	case TMODE_PCTRADE_TARGETITEM :
	case TMODE_PCTRADE_SELFITEM :
	case TMODE_QUEST :
	case TMODE_NPCQUEST :
	case TMODE_PERSONALSTORE_TRADEITEM :
	case TMODE_REPAIRITEM :
	case TMODE_TARGETINFO :
#ifdef _XTS_CASHITEM
	case TMODE_CASHITEM :
#endif
	case TMODE_INCHANT :
	case TMODE_LIFESKILLCOLLECTIONITEM :
	case TMODE_LIFESKILLRESOURCEITEM :
	case TMODE_NPCCOLLECTIONITEM :
	case TMODE_NPCRESOURCEITEM :
		DrawItemTooltip();
		break;
	case TMODE_GAMBLE :
		DrawGambleItemTooltip();
		break;
	case TMODE_SKILL :
	case TMODE_TRADESKILL :
		DrawSkillTooltip();
		break;	
	case TMODE_MASTERYSKILL :
		DrawMasterySkillTooltip();
		break;
	}

	_XWindow::Draw(pfocusobject);
}

void _XWindow_DefaultTooltip::DrawItemTooltip()
{
	DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_TooltipSize.cx, m_WindowPosition.y+m_TooltipSize.cy);
	
	DrawItemDefault();
	
	if( m_CurrentItemSocketCount > -1 )
	{
		_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y+m_TooltipSize.cy+2, 
			m_WindowPosition.x+m_ExpandTooltipSize.cx, m_WindowPosition.y+m_TooltipSize.cy+2+m_ExpandTooltipSize.cy, 
			D3DCOLOR_ARGB(178,0,0,0) );
		DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y+m_TooltipSize.cy+2, m_WindowPosition.x+m_ExpandTooltipSize.cx, m_WindowPosition.y+m_TooltipSize.cy+2+m_ExpandTooltipSize.cy);
		
		// socket box
		_XDrawSolidBar( m_WindowPosition.x+11, m_WindowPosition.y+m_TooltipSize.cy+12, 
			m_WindowPosition.x+m_ExpandTooltipSize.cx-11, m_WindowPosition.y+m_TooltipSize.cy+53, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+11, m_WindowPosition.y+m_TooltipSize.cy+12, 
			m_WindowPosition.x+m_ExpandTooltipSize.cx-11, m_WindowPosition.y+m_TooltipSize.cy+53, 0.0f,D3DCOLOR_ARGB(51,255,255,255) );
		
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		// socket icon
		m_CurrentDrawPosY = m_TooltipSize.cy+60;
		for( int i = 0; i < m_CurrentItemSocketCount; i++ )
		{
			if( m_AddSocketItemList[i].Get_m_sID() > 0 )
			{
				if( g_pInventory_Window )
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+18+(i*42), m_WindowPosition.y+m_TooltipSize.cy+17, &m_AddSocketItemList[i], FALSE );
				m_TooltipIconBoxImage[1]->Draw(m_WindowPosition.x + 18+(i*42), m_WindowPosition.y+m_TooltipSize.cy+17 );
				
				if( strlen(m_CurrentTooltipInfo.E_Data_Buf[i]) )
				{
					g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[i], m_TooltipSize.cx - 16, 1.0, 3 );
					int length = strlen( m_CurrentTooltipInfo.E_Data_Buf[i] ) / 29;				
					m_CurrentDrawPosY += DT_ROW_INTERVAL*(length+1);
				}
				if( strlen(m_CurrentTooltipInfo.E_Data_Buf[i+4]) )
				{
					g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[i+4], m_TooltipSize.cx - 16, 1.0, 3 );
					int length = strlen( m_CurrentTooltipInfo.E_Data_Buf[i+4] ) / 29;				
					m_CurrentDrawPosY += DT_ROW_INTERVAL*(length+1);
				}
			}
			else
			{
				m_TooltipIconBoxImage[0]->Draw(m_WindowPosition.x + 18+(i*42), m_WindowPosition.y+m_TooltipSize.cy+17 );
			}			
		}

		if( strlen(m_CurrentTooltipInfo.E_Data_Buf[8]) )		
		{
			g_XBaseFont->SetColor(0xFF59C74E);
			g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[8], m_TooltipSize.cx - 16, 1.0, 3 );
		}
		if( strlen(m_CurrentTooltipInfo.E_Data_Buf[9]) )
		{
			int length = strlen( m_CurrentTooltipInfo.E_Data_Buf[8] ) / 29;				
			m_CurrentDrawPosY += DT_ROW_INTERVAL*(length+1);
			g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[9], m_TooltipSize.cx - 16, 1.0, 3 );
		}
		
		// four corner
		m_TooltipEdgeImage[0]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_TooltipSize.cy + 3 );
		m_TooltipEdgeImage[1]->Draw( m_WindowPosition.x + m_ExpandTooltipSize.cx - 9 , m_WindowPosition.y + m_TooltipSize.cy + 3 );
		m_TooltipEdgeImage[2]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_TooltipSize.cy +3+ m_ExpandTooltipSize.cy - 10 );
		m_TooltipEdgeImage[3]->Draw( m_WindowPosition.x + m_ExpandTooltipSize.cx - 9, m_WindowPosition.y + m_TooltipSize.cy +3+ m_ExpandTooltipSize.cy - 10 );
	}

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	if( _XGameItem::GetSetItem(m_ItemType, m_ItemId) )
	{
		_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y + m_TooltipSize.cy + m_ExpandTooltipSize.cy + 2, 
						m_WindowPosition.x + m_SetItemExpandTooltipSize.cx, 
						m_WindowPosition.y + m_TooltipSize.cy + 2 + m_ExpandTooltipSize.cy + m_SetItemExpandTooltipSize.cy, 
						D3DCOLOR_ARGB(178,0,0,0) );
		DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y + m_TooltipSize.cy + m_ExpandTooltipSize.cy + 2, 
						m_WindowPosition.x+m_SetItemExpandTooltipSize.cx, 
						m_WindowPosition.y + m_TooltipSize.cy + 2 + m_ExpandTooltipSize.cy + m_SetItemExpandTooltipSize.cy);
		
		// socket box
		/*_XDrawSolidBar( m_WindowPosition.x+11, m_WindowPosition.y+m_TooltipSize.cy+12, 
			m_WindowPosition.x+m_SetItemExpandTooltipSize.cx-11, m_WindowPosition.y+m_TooltipSize.cy+53, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+11, m_WindowPosition.y+m_TooltipSize.cy+12, 
			m_WindowPosition.x+m_SetItemExpandTooltipSize.cx-11, m_WindowPosition.y+m_TooltipSize.cy+53, 0.0f,D3DCOLOR_ARGB(51,255,255,255) );*/

		// four corner
		m_TooltipEdgeImage[0]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_ExpandTooltipSize.cy + m_TooltipSize.cy + 3 ); //코너 크기
		m_TooltipEdgeImage[1]->Draw( m_WindowPosition.x + m_SetItemExpandTooltipSize.cx - 9, m_WindowPosition.y + m_TooltipSize.cy + m_ExpandTooltipSize.cy + 3 );
		m_TooltipEdgeImage[2]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_TooltipSize.cy + 3 + m_ExpandTooltipSize.cy + m_SetItemExpandTooltipSize.cy - 10 );
		m_TooltipEdgeImage[3]->Draw( m_WindowPosition.x + m_SetItemExpandTooltipSize.cx - 9, m_WindowPosition.y + m_TooltipSize.cy + 3 + m_ExpandTooltipSize.cy + m_SetItemExpandTooltipSize.cy - 10 );

		if(  _XGameItem::GetSetItem(m_ItemType, m_ItemId) > 0 )
		{
			g_pSetIteminfo->DrawToolTip( _XGameItem::GetSetItem(m_ItemType, m_ItemId),
										m_WindowPosition.x+10, m_WindowPosition.y + m_TooltipSize.cy + m_ExpandTooltipSize.cy + 2 + DT_LINE_INTERVAL*2, 
										m_CurrentTooltipInfo );
		}
	}
#endif
	
	g_XBaseFont->Flush();
}

void _XWindow_DefaultTooltip::DrawSkillTooltip()
{
	DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_TooltipSize.cx, m_WindowPosition.y+m_TooltipSize.cy);
	// four corner
	m_TooltipEdgeImage[0]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[1]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[2]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_TooltipSize.cy - 9 );
	m_TooltipEdgeImage[3]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + m_TooltipSize.cy - 9 );
	
	m_CurrentDrawPosY = 9;

	// Draw icon
	if( g_pQuickSlot_Window )
		g_pQuickSlot_Window->DrawSkillIcon( m_ItemId, m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, true );

	m_TooltipIconBoxImage[1]->Draw(m_WindowPosition.x + 7, m_WindowPosition.y+m_CurrentDrawPosY );	

	// name
	if( strlen( m_CurrentTooltipInfo.NameBuf ) > 0)
	{
		_XDrawSolidBar( m_WindowPosition.x+42, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+31, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+42, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+31, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

		m_CurrentDrawPosY += 3;
		int namepos = (m_TooltipSize.cx - 35) / 2;			
		g_XBaseFont->SetColor( 0xFFFFFDCA );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+35+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.NameBuf);
		
		FLOAT exp = 0.0f, gaugefactor = 0.0f;
		int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_ItemId, TRUE);

		int skilllevel = 0;
		if(learnedindex == _XDEF_MAX_LEARNSKILL)
		{
			skilllevel = g_pLocalUser->m_EventSkillList.m_cSkillLevel;
		}
		else
		{
			skilllevel = g_pLocalUser->m_SkillList[learnedindex].m_cSkillLevel;
		}
		
		m_CurrentDrawPosY += DT_ROW_INTERVAL-2;
		if(skilllevel >= 11)
		{
			g_XBaseFont->SetColor(0xFFFCCE47);
			exp = (FLOAT)g_SkillProperty[m_ItemId]->detailInfo[skilllevel].skillExp;
			gaugefactor = 100.0f;
		}
		else
		{
			exp = (FLOAT)g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].skillExp;

			//Author : 양희왕 //brief : exp 가 0일떄,,
			if( g_pLocalUser->m_SkillList[learnedindex].m_iSkillExp > 0 )
				gaugefactor = ((FLOAT)g_pLocalUser->m_SkillList[learnedindex].m_iSkillExp / exp) * 100;
			else
				gaugefactor = 0;
		}
		
		m_SkillExp->SetGaugeFactor(gaugefactor);
		m_SkillExp->MoveWindow(m_WindowPosition.x+73, m_WindowPosition.y+28);
		m_SkillExp->Draw();
		POINT pos = m_SkillExp->GetWindowPos();
		SIZE size = m_SkillExp->GetWindowSize();
		_XDrawRectAngle( pos.x-1, pos.y-1, pos.x + size.cx, pos.y + size.cy, 0.0f,D3DCOLOR_ARGB(255,76,76,76) );
			
		g_XSmallFont->SetColor(0xFFB4B4B4);
		if(skilllevel >= 11)
		{
			g_XSmallFont->PrintAlign(pos.x+53, m_WindowPosition.y + m_CurrentDrawPosY, 
				1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f%%", gaugefactor);
		}
		else
		{
			if(exp > 0.0f)
			{
				g_XSmallFont->PrintAlign(pos.x+53, m_WindowPosition.y + m_CurrentDrawPosY, 
					1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f%%", gaugefactor );				
			}
			else
			{
				g_XSmallFont->PrintAlign(pos.x+53, m_WindowPosition.y + m_CurrentDrawPosY, 
					1.0f, _XFONT_ALIGNTYPE_CENTER, "0.00%%" );			
			}			
		}
		g_XSmallFont->Flush();
		
		g_XBaseFont->PrintAlign(m_WindowPosition.x+184, m_WindowPosition.y + m_CurrentDrawPosY,
			1.0f, _XFONT_ALIGNTYPE_LEFT, "%d%s", skilllevel+1, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG));

		m_CurrentDrawPosY += DT_ROW_INTERVAL+2;
	}	

	// Desc
	if( strlen( m_CurrentTooltipInfo.DescBuf ) )
	{
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		length = DT_ROW_INTERVAL*length;

		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, 
						m_WindowPosition.y+m_CurrentDrawPosY+length, D3DCOLOR_ARGB(25,255,255,255) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, 
						 m_WindowPosition.y+m_CurrentDrawPosY+length, 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

		m_CurrentDrawPosY += 3;
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx - 16, 1.0, 3 );
		m_CurrentDrawPosY +=length;
	}

	// Text Draw
	m_CurrentDrawPosY += DT_LINE_INTERVAL;
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );	

	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[2] );
	}	

	if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[3], m_TooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[3], m_TooltipSize.cx-16 );
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen (m_CurrentTooltipInfo.Data_Buf[4]) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[4], m_TooltipSize.cx - 16, 1.0, 3 );
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[5]) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[5], m_TooltipSize.cx - 16, 1.0, 3 );
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[6]) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[6], m_TooltipSize.cx - 16, 1.0, 3 );
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[7], m_TooltipSize.cx - 16, 1.0, 3 );
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[8] ) )
	{
		//g_XBaseFont->SetColor( 0xFFDE0E0E );
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[8], m_TooltipSize.cx - 16, 1.0, 3 );
		
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[9] ) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[9], m_TooltipSize.cx - 16, 1.0, 3 );
	}

	if( m_CurrentTooltipMode == TMODE_TRADESKILL )
	{
		if( strlen(m_CurrentTooltipInfo.E_Data_Buf[2]))
		{
			if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() < g_SkillProperty[m_ItemId]->requirementLevel)
				g_XBaseFont->SetColor( 0xFFDE0E0E );
			else
				g_XBaseFont->SetColor( _XSC_DEFAULT );

			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[2] );
		}

		if( strlen(m_CurrentTooltipInfo.E_Data_Buf[3]))
		{
			if( m_CurrentSlotNumber )
			{
				if( g_pLocalUser->m_Money < g_SkillProperty[m_ItemId]->requirementContribution )
					g_XBaseFont->SetColor( 0xFFDE0E0E );
				else
					g_XBaseFont->SetColor( 0xFFFFFE00 );
			}
			else
			{
				if(g_pLocalUser->m_CharacterInfo.Get_iContribution() < g_SkillProperty[m_ItemId]->requirementContribution)
					g_XBaseFont->SetColor( 0xFFDE0E0E );
				else
					g_XBaseFont->SetColor( 0xFFFFFE00 );
			}		

			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[3] );
		}

		if( strlen(m_CurrentTooltipInfo.E_Data_Buf[4]))
		{
			if(g_pLocalUser->m_CharacterInfo.Get_cClassGrade() != g_SkillProperty[m_ItemId]->classGrade)
			{
				g_XBaseFont->SetColor(0xFFDE0E0E);
			}
			else
			{
				g_XBaseFont->SetColor(0xFFFFFE00);
			}

			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[4] );
		}
	}
	else
	{
		if( strlen(m_CurrentTooltipInfo.Temp_Data_Buf) )
		{			
			g_XBaseFont->SetColor( 0xFFDE0E0E );
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Temp_Data_Buf );
		}
	}

	if( strlen(m_CurrentTooltipInfo.Data_Buf[10]) )
	{
		g_XBaseFont->SetColor( 0xFFDE0E0E );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[10] );
	}

	if( strlen(m_CurrentTooltipInfo.Data_Buf[11]) )
	{
		g_XBaseFont->SetColor( 0xFFDE0E0E );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[11] );
	}

	if( strlen( m_CurrentTooltipInfo.E_Data_Buf[0] ) ) 
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y + m_CurrentDrawPosY, 
						 m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y + m_CurrentDrawPosY, 0.0f, D3DCOLOR_ARGB(255,52,52,52) );

		//Author : 양희왕 //breif : 선수 무공도 색깔을 넣자 08/07/16
		g_XBaseFont->SetColor( 0xFFDE0E0E );

		int learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[m_ItemId]->requirementSkill01);
		if(learnedskillindex >= 0)
		{
			if(g_pLocalUser->m_SkillList[learnedskillindex].m_cSkillLevel >= 7)
				g_XBaseFont->SetColor( 0xFFFFFE00 );
		}	

		m_CurrentDrawPosY += 2;		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[0] );

		if( strlen( m_CurrentTooltipInfo.E_Data_Buf[1] ) ) 
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			//Author : 양희왕 //breif : 선수 무공도 색깔을 넣자 08/07/16
			g_XBaseFont->SetColor( 0xFFDE0E0E );

			int learnedskillindex1 = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[m_ItemId]->requirementSkill02);
			if(learnedskillindex1 >= 0)
			{
				if(g_pLocalUser->m_SkillList[learnedskillindex1].m_cSkillLevel >= 7)
					g_XBaseFont->SetColor( 0xFFFFFE00 );
			}
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[1] );

			m_CurrentDrawPosY += DT_ROW_INTERVAL-2;
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y + m_CurrentDrawPosY, 
						 m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y + m_CurrentDrawPosY, 0.0f, D3DCOLOR_ARGB(255,52,52,52) );			
		}
		else
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL-2;
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y + m_CurrentDrawPosY, 
							 m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y + m_CurrentDrawPosY, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );			
		}
	}

	if( strlen(m_CurrentTooltipInfo.Data_Buf[12]) )
	{
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[12] );
	}

	if( strlen( m_CurrentTooltipInfo.LData_Buf ) ) 
	{
		if( strlen( m_CurrentTooltipInfo.E_Data_Buf[0] ) )
		{
			m_CurrentDrawPosY += DT_LINE_INTERVAL;
		}
		else
		{
			m_CurrentDrawPosY += DT_LINE_INTERVAL;
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
		}		
		
#ifdef _XTS_DELETESKILL
//#ifdef _XTS_NEWSKILLWINDOW		
		if( m_CurrentTooltipMode == TMODE_SKILL )
		{
			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY-2, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-36, D3DCOLOR_ARGB(255,0,0,0) );
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY-2, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-36, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
		}
		else
		{
			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY-2, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-7, D3DCOLOR_ARGB(255,0,0,0) );
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY-2, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-7, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
		}		
#else
		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY-2, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-7, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY-2, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-7, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
#endif
		
		m_CurrentDrawPosY += 2;
		g_XBaseFont->SetColor( 0xFFE7B329 );
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.LData_Buf, m_TooltipSize.cx - 16, 1.0, 3 );
	}

	g_XBaseFont->Flush();
}

void _XWindow_DefaultTooltip::DrawMasterySkillTooltip()
{
	DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_TooltipSize.cx, m_WindowPosition.y+m_TooltipSize.cy);
	// four corner
	m_TooltipEdgeImage[0]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[1]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[2]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_TooltipSize.cy - 9 );
	m_TooltipEdgeImage[3]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + m_TooltipSize.cy - 9 );
	
	m_CurrentDrawPosY = 9;
 
	m_TooltipIconBoxImage[1]->Draw(m_WindowPosition.x + 16, m_WindowPosition.y+16 );

	if( strlen( m_CurrentTooltipInfo.NameBuf ) )
	{
		_XDrawSolidBar( m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+46, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+46, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
	
		// Draw icon
		if( g_pMasterySkill_Window )
			g_pMasterySkill_Window->DrawMasterySkillIcon( m_ItemId, m_WindowPosition.x+17, m_WindowPosition.y+17, g_pMasterySkill_Window->GetMasterySkillProp(m_ItemId), true );
		
		m_CurrentDrawPosY += 11;
		int namepos = 44+ (m_TooltipSize.cx-44) / 2;
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.NameBuf);
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int namepos = 44+ (m_TooltipSize.cx-44) / 2;
		g_XBaseFont->SetColor( 0xFFF0CD68 );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.Data_Buf[0]);		
		m_CurrentDrawPosY += 5;
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL*2;
		g_XBaseFont->SetColor( 0xFFDE0E0E );
		int namepos = m_TooltipSize.cx / 2;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.Data_Buf[1]);
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
		if( m_CurrentDrawPosY <= (DT_LINE_INTERVAL*3+9) )
			m_CurrentDrawPosY += DT_LINE_INTERVAL*2;
		else
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFFDE0E0E );

		int namepos = m_TooltipSize.cx / 2;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.Data_Buf[2]);
	}

	if( strlen( m_CurrentTooltipInfo.LData_Buf ) )
	{
		if( m_CurrentDrawPosY <= (DT_LINE_INTERVAL*3+9) )
			m_CurrentDrawPosY += DT_LINE_INTERVAL*2;
		else
			m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		g_XBaseFont->SetColor( 0xFFF0CD68 );	
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 12, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.LData_Buf, m_TooltipSize.cx - 16, 1.0, 3 );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.LData_Buf, m_TooltipSize.cx-16 );
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);					
	}

	// box 
	if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL+DT_LINE_INTERVAL*2;
		_XDrawSolidBar( m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-9, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_TooltipSize.cy-9, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
		{
			g_XBaseFont->SetColor( 0xFFCCCCCC );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+12, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[3]);
		}

		int index = g_pLocalUser->GetLearnedMasterySkillIndex(m_ItemId);		
		if( g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() < g_SkillMasteryProperty[m_ItemId]->detailInfo[g_pLocalUser->m_MasterySkillList[index].cPoint].sReqMp)
			g_XBaseFont->SetColor( 0xFFDE0E0E );
		else
			g_XBaseFont->SetColor( 0xFF4BEF4B );

		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
			g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
		}

		g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx-11, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[4]);		
	}

	if( strlen( m_CurrentTooltipInfo.LData_Buf2 ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFF959595 );
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 12, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.LData_Buf2, m_TooltipSize.cx - 16, 1.0, 3 );
	}
	g_XBaseFont->Flush();
}

void _XWindow_DefaultTooltip::DrawGambleItemTooltip()
{
	DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_TooltipSize.cx, m_WindowPosition.y+m_TooltipSize.cy);

	// four corner
	m_TooltipEdgeImage[0]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[1]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[2]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_TooltipSize.cy - 9 );
	m_TooltipEdgeImage[3]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + m_TooltipSize.cy - 9 );

	m_CurrentDrawPosY = 9;
	// Draw icon 
	if( m_pNPCTradeWindow )
	{
		if( m_pNPCTradeWindow->GetShowStatus() )
		{
			list <_XTradeItem>::iterator iter_item = m_pNPCTradeWindow->m_TradeItemList.begin();
			advance( iter_item, m_CurrentSlotNumber );				
			_XTradeItem tradeitem = *iter_item;
			
			if( g_pInventory_Window )
			{
				if( g_pInventory_Window->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, (_XUserItem*)&tradeitem, FALSE );
				}
			}
		}
	}
		
	
	m_TooltipIconBoxImage[1]->Draw(m_WindowPosition.x + 7, m_WindowPosition.y + m_CurrentDrawPosY );		

	// name
	if( strlen( m_CurrentTooltipInfo.NameBuf ) )
	{		
		int length = strlen( m_CurrentTooltipInfo.NameBuf ) / 23;
		length = /*length2*/ + DT_ROW_INTERVAL*(length+2)-1;
		
		_XDrawSolidBar( m_WindowPosition.x+42, m_WindowPosition.y+m_CurrentDrawPosY, 
			m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+length, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+42, m_WindowPosition.y+m_CurrentDrawPosY,
			m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+length, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
		
		int namepos = (m_TooltipSize.cx - 35) / 2;
		
		m_CurrentDrawPosY += 10;
		g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 180, 180, 180 ));
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+35+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.NameBuf);
		m_CurrentDrawPosY += 22;		
	
		m_CurrentDrawPosY += DT_LINE_INTERVAL;		

		if( strlen( m_CurrentTooltipInfo.DescBuf ) )
		{
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
			length = DT_ROW_INTERVAL*length;
			
			m_CurrentDrawPosY += DT_LINE_INTERVAL;
			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, 
				m_WindowPosition.y+m_CurrentDrawPosY+length, D3DCOLOR_ARGB(25,255,255,255) );
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, 
				m_WindowPosition.y+m_CurrentDrawPosY+length, 0.0f,D3DCOLOR_ARGB(51,255,255,255) );
			
			m_CurrentDrawPosY += 3;
			g_XBaseFont->SetColor(_XSC_DEFAULT);		
			g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx - 16, 1.0, 3 );
			length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
			length = DT_ROW_INTERVAL*length;
			m_CurrentDrawPosY += length;
			m_CurrentDrawPosY += DT_LINE_INTERVAL;
		}

#ifdef _XDEF_PERIODITEM_20061027
		// 기간제 아이템이면 남은 시간을 표시한다.
		if(strlen(m_CurrentTooltipInfo.Data_Buf[13]))
		{
			g_XBaseFont->SetColor(_XSC_INFORMATION);
			g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
			length = DT_ROW_INTERVAL*length;
			m_CurrentDrawPosY += length;
		}
#endif

		DrawItemPrice(m_CurrentDrawPosY);
		
		g_XBaseFont->Flush();
	}
}

void _XWindow_DefaultTooltip::DrawTooltipLine( int left, int top, int right, int bottom )
{
	_XDrawLine2D( left+2, top, right-1, top, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawLine2D( left, top+2, left, bottom-1, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawLine2D( right, top+2, right, bottom-1, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawLine2D( left+2, bottom, right-1, bottom, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	
	//_XDrawRectAngle( left, top, right, bottom, 0.0f,D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( left+1, top+1, right-1, bottom-1, 0.0f,D3DCOLOR_ARGB(255,207,202,183) );
	_XDrawRectAngle( left+2, top+2, right-2, bottom-2, 0.0f,D3DCOLOR_ARGB(191,0,0,0) );
	_XDrawRectAngle( left+3, top+3, right-3, bottom-3, 0.0f,D3DCOLOR_ARGB(76,0,0,0) );
}

void _XWindow_DefaultTooltip::DrawItemDefault()
{
	// four corner
	m_TooltipEdgeImage[0]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[1]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + 1 );
	m_TooltipEdgeImage[2]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_TooltipSize.cy - 9 );
	m_TooltipEdgeImage[3]->Draw( m_WindowPosition.x + m_TooltipSize.cx - 9, m_WindowPosition.y + m_TooltipSize.cy - 9 );

	m_CurrentDrawPosY = 9;
	// Draw icon 
	switch( m_CurrentTooltipMode ) 
	{
	case TMODE_DEFAULTITEM :	
	case TMODE_NPCTRADE_INVENITEM :
	case TMODE_REPAIRITEM :
		{
			if( g_pInventory_Window )
			{
				if( g_pInventory_Window->GetShowStatus() )
					g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &g_pLocalUser->m_UserItemList[m_CurrentSlotNumber], FALSE);
			}
		}		
		break;
	case TMODE_CINFOITEM :
		{
			if( g_pInventory_Window )
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &g_pLocalUser->m_UserItemList[m_CurrentSlotNumber], FALSE);
		}
		break;
	case TMODE_NPCTRADE_NPCITEM :
		{
			if( m_pNPCTradeWindow )
			{
				if( m_pNPCTradeWindow->GetShowStatus() )
				{
					list <_XTradeItem>::iterator iter_item = m_pNPCTradeWindow->m_TradeItemList.begin();
					advance( iter_item, m_CurrentSlotNumber );				
					_XTradeItem tradeitem = *iter_item;
					
					if( g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus() )
						{
							g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, (_XUserItem*)&tradeitem, FALSE );
						}
					}
				}
			}
		}
		break;
	case TMODE_WAREHOUSE_WAREHOUSEITEM :
		{
			if( m_pWarehouseWindow )
			{
				if( m_pWarehouseWindow->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber], FALSE );
				}
			}
		}
		break;
	case TMODE_PCTRADE_TARGETITEM :
		{
			if( m_pPCTradeWindow )
			{
				if( m_pPCTradeWindow->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber], FALSE );
				}
			}
		}
		break;
	case TMODE_PCTRADE_SELFITEM :
		{
			if( m_pPCTradeWindow )
			{
				if( m_pPCTradeWindow->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber], FALSE );
				}
			}
		}
		break;	
	case TMODE_QUEST :
		{
			if( g_pQuest_Window )
			{
				if( g_pQuest_Window->GetShowStatus() )
				{				
					int	imageiconindex = -1;
					RECT rect = {0, 0, 0, 0};
					g_pInventory_Window->GetIconRect(m_ItemType, m_ItemId, imageiconindex, rect, TRUE );
					if(imageiconindex != -1)
					{
						g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, imageiconindex);
						g_InventoryIconImage.DrawWithRegion(m_WindowPosition.x+8, m_WindowPosition.y+m_CurrentDrawPosY+1, rect);					
					}
				}
			}
		}
		break;	
	case TMODE_NPCQUEST :
		{
			if( m_pNPCScript_Window )
			{
				if( m_pNPCScript_Window->GetShowStatus() )
				{
					if( g_pQuest_Window )
					{				
						int	imageiconindex = -1;
						RECT rect = {0, 0, 0, 0};
						g_pInventory_Window->GetIconRect(m_ItemType, m_ItemId, imageiconindex, rect, TRUE );
						if(imageiconindex != -1) 
						{					
							g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, imageiconindex);
							g_InventoryIconImage.DrawWithRegion(m_WindowPosition .x+8, m_WindowPosition.y+m_CurrentDrawPosY+1, rect);
						}
					}
				}
			}
		}
		break;
	case TMODE_PERSONALSTORE_TRADEITEM :
		{
			if( m_pPersonalStore_Window )
			{
				if( m_pPersonalStore_Window->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber], FALSE );
				}
			}
		}
		break;	
	case TMODE_TARGETINFO :
		{
			if( m_pTargetInfo_Window )
			{
				if( m_pTargetInfo_Window->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber], FALSE );
				}
			}
		}
		break;
#ifdef _XTS_CASHITEM
	case TMODE_CASHITEM :
		{
			if( m_pCashItem_Window )
			{
				if( m_pCashItem_Window->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, m_pCashItem_Window->GetCashItem(), FALSE );
				}
			}
		}
		break;
#endif
	case TMODE_LIFESKILLCOLLECTIONITEM :
		{		
			if( g_pLifeSkill_Window )
			{
				if( g_pLifeSkill_Window->GetShowStatus() )
				{
					if( g_pLifeSkill_Window->m_pSelectedCollectionItem )
					{
						list <_XCollectionResourceItem*>::iterator iter_needitem;
						_XCollectionResourceItem* currentneeditem = NULL;
						for(iter_needitem = g_pLifeSkill_Window->m_pSelectedCollectionItem->m_listNeedItem.begin() ; iter_needitem != g_pLifeSkill_Window->m_pSelectedCollectionItem->m_listNeedItem.end() ; iter_needitem++)
						{
							currentneeditem = *iter_needitem;
							if(currentneeditem)
							{
								if( currentneeditem->m_Index == m_CurrentSlotNumber)
								{
									g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, currentneeditem,FALSE);
									break;
								}
							}
						}
					}
				}
			}
		}
		break;
	case TMODE_LIFESKILLRESOURCEITEM :
		{
			if( g_pLifeSkill_Window )
			{
				if( g_pLifeSkill_Window->GetShowStatus() )
				{
					if( g_pLifeSkill_Window->m_indexSelectedResourceItem != -1 && g_pLifeSkill_Window->m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
					{
						if( m_CurrentSlotNumber == 0 )// 보상 아이템
						{
							g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &g_ResourceManager.m_arrayResourceItem[g_pLifeSkill_Window->m_indexSelectedResourceItem].m_RewardItem,FALSE);
						}
						else
						{
							list <_XCollectionResourceItem*>::iterator iter_needitem;
							_XCollectionResourceItem* currentneeditem = NULL;
							for(iter_needitem = g_ResourceManager.m_arrayResourceItem[g_pLifeSkill_Window->m_indexSelectedResourceItem].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[g_pLifeSkill_Window->m_indexSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
							{
								currentneeditem = *iter_needitem;
								if(currentneeditem)
								{
									if( currentneeditem->m_Index == (m_CurrentSlotNumber-1))
									{
										g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, currentneeditem,FALSE);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	case TMODE_NPCCOLLECTIONITEM :
		{
			if( m_pNPCCollection_Window )
			{
				if( m_pNPCCollection_Window->GetShowStatus() )
				{
					if( m_pNPCCollection_Window->m_pSelectedCollectionItem )
					{
						int column = m_CurrentSlotNumber%3;	// 보상 아이템 인덱스
						int row = m_CurrentSlotNumber/3;		// 보상 세트 인덱스
						_XCollectionRewardSet* currentrewardset = NULL;
						_XCollectionResourceItem* currentrewarditem = NULL;
						list <_XCollectionRewardSet*>::iterator iter_rewardset;
						for(iter_rewardset = m_pNPCCollection_Window->m_pSelectedCollectionItem->m_listRewardSet.begin() ; iter_rewardset != m_pNPCCollection_Window->m_pSelectedCollectionItem->m_listRewardSet.end() ; iter_rewardset++)
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
											g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, currentrewarditem,FALSE);
											break;
										}
									}
								}
								break;												
							}
						}
					}
				}
			}
		}
		break;
	case TMODE_NPCRESOURCEITEM :
		{
#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI
			if( m_pNPCResource_New_Window )
			{
				if( m_pNPCResource_New_Window->GetShowStatus() )
				{
					if( m_pNPCResource_New_Window->GetSelectedResourceItem() != -1 && m_pNPCResource_New_Window->GetSelectedResourceItem() < g_ResourceManager.m_TotalCountResourceItem)
					{
						if(m_CurrentSlotNumber == 0)
						{
							g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1,
								&g_ResourceManager.m_arrayResourceItem[m_pNPCResource_New_Window->GetSelectedResourceItem()].m_RewardItem, FALSE);
						}
						else
						{
							list <_XCollectionResourceItem*>::iterator iter_needitem;
							_XCollectionResourceItem* currentneeditem = NULL;
							for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_pNPCResource_New_Window->GetSelectedResourceItem()].m_listNeedItem.begin() ;
								iter_needitem != g_ResourceManager.m_arrayResourceItem[m_pNPCResource_New_Window->GetSelectedResourceItem()].m_listNeedItem.end() ; iter_needitem++)
							{
								currentneeditem = *iter_needitem;
								if(currentneeditem)
								{
									if( currentneeditem->m_Index == m_CurrentSlotNumber-1)
									{
										g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, currentneeditem,FALSE);
										break;
									}
								}
							}
						}
					}
				}
			}

#else
			if( m_pNPCResource_Window )
			{
				if( m_pNPCResource_Window->GetShowStatus() )
				{
					if( m_pNPCResource_Window->m_indexSelectedResourceItem != -1 && m_pNPCResource_Window->m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
					{
						list <_XCollectionResourceItem*>::iterator iter_needitem;
						_XCollectionResourceItem* currentneeditem = NULL;
						for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_pNPCResource_Window->m_indexSelectedResourceItem].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[m_pNPCResource_Window->m_indexSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
						{
							currentneeditem = *iter_needitem;
							if(currentneeditem)
							{
								if( currentneeditem->m_Index == m_CurrentSlotNumber)
								{
									g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, currentneeditem,FALSE);
									break;
								}
							}
						}						
					}
				}
			}
#endif
		}
		break;
	case TMODE_INCHANT :
		{
			if( m_pInchant_Window )
			{
				if( m_pInchant_Window->GetShowStatus() )
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, &m_pInchant_Window->m_InchantStuff[0], FALSE );
				}
			}

#ifdef _XDEF_SMELT_ITEM // Author : 양희왕
			XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
			if( pSmletItemWindow  && pSmletItemWindow->GetShowStatus() )
			{
				//const_cast , 함수 구조 변경 불가능
				g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+8, m_WindowPosition.y + m_CurrentDrawPosY+1, const_cast<_XUserItem*>(pSmletItemWindow->GetUserItem()), FALSE );
			}
#endif
		}
		break;
	}
	
	m_TooltipIconBoxImage[1]->Draw(m_WindowPosition.x + 7, m_WindowPosition.y + m_CurrentDrawPosY );		

	// name
	if( strlen( m_CurrentTooltipInfo.NameBuf ) )
	{
		//int length2 = strlen( m_CurrentTooltipInfo.NickBuf ) / 23; 
		//length2 = DT_ROW_INTERVAL*(length2+2)-1;
		int length = strlen( m_CurrentTooltipInfo.NameBuf ) / 23;
		length = /*length2*/ + DT_ROW_INTERVAL*(length+2)-1;
		
		_XDrawSolidBar( m_WindowPosition.x+42, m_WindowPosition.y+m_CurrentDrawPosY, 
			m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+length, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+42, m_WindowPosition.y+m_CurrentDrawPosY,
			m_WindowPosition.x+m_TooltipSize.cx-8, m_WindowPosition.y+m_CurrentDrawPosY+length, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
		
		int namepos = (m_TooltipSize.cx - 35) / 2;
		if( strlen( m_CurrentTooltipInfo.NickBuf ) )
		{
			m_CurrentDrawPosY += 2;
			g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));

			//int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NickBuf, 23 );
			//if( length == 1 )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+35+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.NickBuf);
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
			}
			//else
			//{
			//	g_XBaseFont->Puts_Separate( m_WindowPosition.x + 44, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.NickBuf, m_TooltipSize.cx - 51, 1.0, 3 );
			//	m_CurrentDrawPosY += DT_ROW_INTERVAL*length;
			//}			
		}
		if( strlen( m_CurrentTooltipInfo.NameBuf ) )
		{
			if( strlen( m_CurrentTooltipInfo.NickBuf ) )
			{
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
				if( length == 1 )	// 닉네임 있고 이름이 한줄 짜리
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+35+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.NameBuf);
					m_CurrentDrawPosY += DT_ROW_INTERVAL-2;
				}	
				else				// 닉네임 있고 이름이 두줄 짜리
				{
					g_XBaseFont->Puts_Separate( m_WindowPosition.x + 48, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx - 51, 1.0, 3 );					
					m_CurrentDrawPosY += DT_ROW_INTERVAL*length;
					m_CurrentDrawPosY -= 2;
				}				
			}
			else
			{
				m_CurrentDrawPosY += 10;
				g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
				if( length == 1 )	// 닉네임 없고 한줄 짜리
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+35+namepos, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentTooltipInfo.NameBuf);
					m_CurrentDrawPosY += 22;
				}
				else				// 닉네임 없고 두줄 짜리	
				{
					g_XBaseFont->Puts_Separate( m_WindowPosition.x + 48, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx - 51, 1.0, 3 );
					m_CurrentDrawPosY += DT_ROW_INTERVAL*length;
					m_CurrentDrawPosY += 6;
				}
			}
		}		
		
		g_XBaseFont->Flush();
	}	

	// Desc
	if( strlen( m_CurrentTooltipInfo.DescBuf ) )
	{
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		length = DT_ROW_INTERVAL*length;

		for( int i = 0; i < 5; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.S_Data_Buf[i] ) )
			{
				int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.S_Data_Buf[i], m_TooltipSize.cx-16 );
				if( length > 0 )
					length += DT_ROW_INTERVAL*templength;
			}			
		}
		
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, 
						m_WindowPosition.y+m_CurrentDrawPosY+length, D3DCOLOR_ARGB(25,255,255,255) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_TooltipSize.cx-8, 
						 m_WindowPosition.y+m_CurrentDrawPosY+length, 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

		m_CurrentDrawPosY += 3;
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx - 16, 1.0, 3 );
		length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		length = DT_ROW_INTERVAL*length;
		m_CurrentDrawPosY += length;

#ifdef _XTS_FAME
		if( _XGI_FC_WEAPON == GetEqualItemType(m_ItemType) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
			if( WeaponItem )
			{
				if( WeaponItem[m_ItemId].ucFame > 0 )
				{
					if( g_FameStr->CheckItemFame(WeaponItem[m_ItemId].ucFame) )
						g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));
					else
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
				}				
				else
					g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));
			}			
		}
		else if( _XGI_FC_CLOTHES == GetEqualItemType( m_ItemType ) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
			if( ClothesItem )
			{
				if( ClothesItem[m_ItemId].ucFame > 0 )
				{
					if( g_FameStr->CheckItemFame(ClothesItem[m_ItemId].ucFame) )
						g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));
					else
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
				}	
				else
					g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));
			}
		}
		else
			g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));
#else
		g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_ItemType, m_ItemId));
#endif
		for( i = 0; i < 5; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.S_Data_Buf[i] ) )
			{
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.S_Data_Buf[i], m_TooltipSize.cx - 16, 1.0, 3 );

				int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.S_Data_Buf[i], m_TooltipSize.cx-16 );
				m_CurrentDrawPosY += DT_ROW_INTERVAL * templength;
			}
		}
	}	

	// Text Draw
	char itemtype = GetEqualItemType(m_ItemType);
	if( itemtype != -1 )
	{	
		switch(itemtype) 
		{
		case _XGI_FC_WEAPON :
			DrawTextWeaponItem();
			break;
		case _XGI_FC_CLOTHES :
			DrawTextClothesItem();
			break;
		case _XGI_FC_BOOK:
			DrawTextBook();
			break;
		case _XGI_FC_POTION:
			DrawTextPotion();
			break;
		case _XGI_FC_CONSUMING:
			DrawTextConsuming();
			break;
		case _XGI_FC_ELIXIR:
			DrawElixir();
			break;
		case _XGI_FC_COUNTERACT:
			break;
		case _XGI_FC_RESOURCE:
			DrawTextResource();
			break;
		case _XGI_FC_QUEST:
			DrawTextQuest();
			break;
		case _XGI_FC_ACCESSORY:
			DrawTextAccessory();
			break;
		case _XGI_FC_LIFE:
			DrawTextLife();
			break;
		case _XGI_FC_MUSIC:
			break;
		case _XGI_FC_PASS:
			break;
		case _XGI_FC_SOCKET:
			DrawTextSocket();
			break;
		case _XGI_FC_BOX:
			DrawTextBox();
			break;
		case _XGI_FC_BOXKEY:
			DrawTextBoxKey();
			break;
		}
	}
}

void _XWindow_DefaultTooltip::DrawDurability()
{
	int avg= 10;
	int curdur = 0;
	int maxdur = 0;
	g_XSmallFont->SetColor( _XSC_DEFAULT );
	
	int durstarticon = 50;
	int durstartnumber = 90;

	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		durstarticon = 70;
		durstartnumber = 110;
	}

	switch( m_CurrentTooltipMode ) 
	{
	case TMODE_DEFAULTITEM :
	case TMODE_CINFOITEM :
	case TMODE_NPCTRADE_INVENITEM :
	case TMODE_REPAIRITEM :
		{
			curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
			maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;

			if(curdur > 0 && maxdur > 0)
			{
				avg = curdur*10 / maxdur;
			}
			if( avg == 0 && curdur > 0 )
				avg = 1;
			sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", curdur, maxdur );
			g_XSmallFont->PutsAlign(m_WindowPosition.x+durstartnumber, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Dur_Data_Buf );
		}
		break;
	case TMODE_NPCTRADE_NPCITEM :	
		{
			avg = 10;
			g_XSmallFont->PutsAlign(m_WindowPosition.x+durstartnumber, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Dur_Data_Buf );
		}
		break;
	case TMODE_WAREHOUSE_WAREHOUSEITEM :
		{
			curdur = ( g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
			maxdur = ( g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;

			if(curdur > 0 && maxdur > 0)
			{
				avg = curdur*10 / maxdur;
			}
			if( avg == 0 && curdur > 0 )
				avg = 1;

			sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", curdur, maxdur );
			g_XSmallFont->PutsAlign(m_WindowPosition.x+durstartnumber, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Dur_Data_Buf );
		}
		break;
	case TMODE_PCTRADE_TARGETITEM :
		{
			if( m_pPCTradeWindow )
			{
				curdur = ( m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
				maxdur = ( m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;				
			}

			if(curdur > 0 && maxdur > 0)
			{
				avg = curdur*10 / maxdur;
			}
			if( avg == 0 && curdur > 0 )
				avg = 1;
			sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", curdur, maxdur );
			g_XSmallFont->PutsAlign(m_WindowPosition.x+durstartnumber, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Dur_Data_Buf );
		}
		break;
	case TMODE_PCTRADE_SELFITEM :
		{
			if( m_pPCTradeWindow )
			{
				curdur = ( m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
				maxdur = ( m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;				
			}

			if(curdur > 0 && maxdur > 0)
			{
				avg = curdur*10 / maxdur;
			}
			if( avg == 0 && curdur > 0 )
				avg = 1;
			sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", curdur, maxdur );
			g_XSmallFont->PutsAlign(m_WindowPosition.x+durstartnumber, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Dur_Data_Buf );
		}
		break;
	case TMODE_PERSONALSTORE_TRADEITEM :
		{
			if( m_pPersonalStore_Window )
			{
				curdur = ( m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
				maxdur = ( m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;				
			}
			
			if(curdur > 0 && maxdur > 0)
			{
				avg = curdur*10 / maxdur;
			}
			if( avg == 0 && curdur > 0 )
				avg = 1;
			sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", curdur, maxdur );
			g_XSmallFont->PutsAlign(m_WindowPosition.x+durstartnumber, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Dur_Data_Buf );
		}
		break;
	case TMODE_TARGETINFO :
		{
			if( m_pTargetInfo_Window )
			{
				curdur = ( m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
				maxdur = ( m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;				
			}
			
			if(curdur > 0 && maxdur > 0)
			{
				avg = curdur*10 / maxdur;
			}
			if( avg == 0 && curdur > 0 )
				avg = 1;
			sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", curdur, maxdur );
			g_XSmallFont->PutsAlign(m_WindowPosition.x+durstartnumber, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Dur_Data_Buf );
		}
		break;
	}

	if( avg < 2 )
	{
		for( int i = 0; i < avg; i++ )
		{
			_XDrawSolidBar( m_WindowPosition.x+durstarticon+(i*3), m_WindowPosition.y + m_CurrentDrawPosY,
				m_WindowPosition.x+durstarticon+2+(i*3), m_WindowPosition.y + m_CurrentDrawPosY+11, D3DCOLOR_ARGB(255,234,32,23));				
		}			
	}
	else if( avg < 5 )
	{
		for( int i = 0; i < avg; i++ )
		{
			_XDrawSolidBar( m_WindowPosition.x+durstarticon+(i*3), m_WindowPosition.y + m_CurrentDrawPosY,
				m_WindowPosition.x+durstarticon+2+(i*3), m_WindowPosition.y + m_CurrentDrawPosY+11, D3DCOLOR_ARGB(255,246,223,68));				
		}						
	}
	else
	{
		if( avg > 10 )
			avg = 10;
		for( int i = 0; i < avg; i++ )
		{
			_XDrawSolidBar( m_WindowPosition.x+durstarticon+(i*3), m_WindowPosition.y + m_CurrentDrawPosY,
				m_WindowPosition.x+durstarticon+2+(i*3), m_WindowPosition.y + m_CurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
		}
	}

	for( int i = avg; i < 10; i++)
	{
		_XDrawSolidBar( m_WindowPosition.x+durstarticon+(i*3), m_WindowPosition.y + m_CurrentDrawPosY,
				m_WindowPosition.x+durstarticon+2+(i*3), m_WindowPosition.y + m_CurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,0,0));
	}

	g_XSmallFont->Flush();
}

void _XWindow_DefaultTooltip::DrawItemPrice( int nDrawY )
{
    if( _XGameItem::CheckNPCTrade(m_ItemType, m_ItemId) ) //Author : 양희왕 //breif : 팔수없는 물건은 가격이 나오면 안됨
        return;

	if(m_nPriceMode == 0)
		m_MoneyImage->Draw( m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY );
	else
		m_pClanPointImage->Draw(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY);
	
	unsigned int tempMoney=0;
	unsigned int tax_money = 0;
	unsigned int org_money = 0;
	
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM )
	{
		if( m_pNPCTradeWindow )
			tempMoney = m_pNPCTradeWindow->PriceCalc(m_CurrentSlotNumber);

		// 지역 세율 계산 - 팔 때
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE

		if(m_nPriceMode == 0)
		{
			// 임시 코드
			FLOAT correction = 0.4f;
			int tvalue = tempMoney % 10;
			if(tvalue == 6)
			{
				correction = 0.5f;
			}

			org_money = tempMoney;
			tax_money = static_cast<u_int>( ((float)tempMoney*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== 세금으로 걷는 돈.
			tempMoney -= tax_money;
		}

#endif

	}
	else if( m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		tempMoney = _XGameItem::GetItemPrice( m_ItemType, m_ItemId);

		// 지역 세율 계산 - 살 때
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE

		if(m_nPriceMode == 0)
		{
			// 임시코드
			FLOAT correction = 0.4f;
			int tvalue = tempMoney % 10;
			if(tvalue == 6)
			{
				correction = 0.5f;
			}

			org_money = tempMoney;
			tax_money = static_cast<u_int>( ((float)tempMoney*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== 세금으로 걷는 돈.
			tempMoney += tax_money;
		}

#endif

	}
	else if( m_CurrentTooltipMode == TMODE_GAMBLE )
	{
		if( m_pNPCTradeWindow )
			tempMoney = m_pNPCTradeWindow->m_GambleItemInfo[m_CurrentSlotNumber].Price;

		org_money = tempMoney;
	}
	
	TCHAR moneyStr[128];
	memset( moneyStr, 0, sizeof(moneyStr));
	sprintf(moneyStr, "%u", tempMoney);
	int colorindex = 0;

	if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
	{
		g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
		g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+nDrawY+1, moneyStr );
		
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
		if(tax_money > 0)
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->PrintAlign(m_WindowPosition.x+189, m_WindowPosition.y+nDrawY+1, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_3401));	//"거래세 적용"
		}
#endif
		g_XBaseFont->Flush();

#ifdef _XDWDEBUG
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
		g_XBaseFont->Print(m_WindowPosition.x+30, m_WindowPosition.y+m_CurrentDrawPosY+18+1, 1.0f, "(org:%d+tax:%d)", org_money, tax_money );
#endif
#endif
	}
	else
	{
		TCHAR tempstr[128];
		sprintf( tempstr, "%d", tempMoney );		
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->Puts( m_WindowPosition.x+30, m_WindowPosition.y + nDrawY+1, tempstr );

#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
		if(tax_money > 0)
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->PrintAlign(m_WindowPosition.x+189, m_WindowPosition.y+nDrawY+1, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_3401));	//"거래세 적용"
		}
#endif
		g_XBaseFont->Flush();

#ifdef _XDWDEBUG
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
		g_XBaseFont->Print(m_WindowPosition.x+30, m_WindowPosition.y+m_CurrentDrawPosY+18+1, 1.0f, "(org:%d+tax:%d)", org_money, tax_money );
#endif
#endif
	}
}

// 문파 기여도
void _XWindow_DefaultTooltip::DrawItemClanPoint( int nDrawY )
{
    if( _XGameItem::CheckNPCTrade(m_ItemType, m_ItemId) ) //Author : 양희왕 //breif : 팔수없는 물건은 가격이 나오면 안됨
        return;

	m_ContributionImage->Draw( m_WindowPosition.x+10, m_WindowPosition.y + nDrawY );
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	
	unsigned int tempContibution = 0;
	
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			tempContibution = m_pContribution_Window->CalcContribution(m_CurrentSlotNumber);
		}
	}
	
	TCHAR tempstr[128];
	sprintf( tempstr, "%d", tempContibution );
	g_XBaseFont->Puts( m_WindowPosition.x+30, m_WindowPosition.y + nDrawY, tempstr );
}

BOOL _XWindow_DefaultTooltip::Process(_XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)		return FALSE;	
	if(!this->m_ShowWindow)		return FALSE;	

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		// 무공 전수창 때문에 다시 바꿔놓았음 - kukuri 2005.01.30
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
#ifdef _XTS_DELETESKILL
//#ifdef _XTS_NEWSKILLWINDOW
				if( !m_DeleteSkillBtn->CheckMousePosition() )
#endif
				{
					if(!CheckMousePosition())
						this->ShowWindow(FALSE);
					
					if( this->m_ShowWindow )
						this->ShowWindow(FALSE);
				}				
			}
		}
	}
	if( m_CurrentTooltipMode != TMODE_TRADESKILL )
		g_MainWindowManager.SetTopWindow(this);
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_DefaultTooltip::ShowWindow(BOOL show)
{
	if(show)
	{
		m_pMyManager->SetTopWindow(this);		
	}
	else
	{
		m_nPriceMode = 0;
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_DefaultTooltip::MoveWindow(int X, int Y)
{
#ifdef _ACCLAIM_RUBICONADSYSTEM			
	if( g_ViewRubiconAD )
	{
		if( Y < g_RubiconADWindowSize.cy ) //툴팁이 광고 위로 올라가면
			Y = g_RubiconADWindowSize.cy + 2; //광고줄에 맞춰준다
	}					
#endif

	_XWindow::MoveWindow(X, Y);
}

BOOL _XWindow_DefaultTooltip::SetTooltipMode( _TOOLTIP_MODE mode, int slotnumber, int type, int id, int nPriceMode )
{
	if( mode == TMODE_QUEST || mode == TMODE_NPCQUEST  )
	{
		if( slotnumber == -1 )
			return FALSE;
	}
	else if ( mode == TMODE_LIFESKILLCOLLECTIONITEM || mode == TMODE_LIFESKILLRESOURCEITEM  || mode == TMODE_NPCCOLLECTIONITEM  || mode == TMODE_NPCRESOURCEITEM )
	{
		if( slotnumber == -1 )
			return FALSE;
	}
	else if( id == 0 || slotnumber == -1 )
		return FALSE;

	m_nPriceMode = nPriceMode;

	ResetTooltipInfo();
#ifdef _XTS_DELETESKILL
//#ifdef _XTS_NEWSKILLWINDOW
	m_DeleteSkillBtn->ShowWindow(FALSE);
#endif

	if( mode < TMODE_SKILL )	// Item Tooltip Size
	{
		// 최소 사이즈 설정 ---------=
		m_TooltipSize.cx = 194;
		m_TooltipSize.cy = 16;
		m_ExpandTooltipSize.cx = 0;
		m_ExpandTooltipSize.cy = 0;
		// --------------------------=
	}
	else	// Skill Tooltip Size
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
			g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			m_TooltipSize.cx = 300;
		else
			m_TooltipSize.cx = 236;
		m_TooltipSize.cy = 16;	
	}
	m_SetItemExpandTooltipSize.cx = 0; //Author :양희왕 //breif : 세트아이템 사이즈
	m_SetItemExpandTooltipSize.cy = 0;

	m_CurrentTooltipMode = mode;
	m_CurrentSlotNumber = slotnumber;
	m_ItemType = type;
	m_ItemId = id;	

	switch( m_CurrentTooltipMode ) 
	{
	case TMODE_DEFAULTITEM :
	case TMODE_CINFOITEM :
	case TMODE_NPCTRADE_INVENITEM :
	case TMODE_NPCTRADE_NPCITEM :		
	case TMODE_WAREHOUSE_WAREHOUSEITEM :		
	case TMODE_PCTRADE_TARGETITEM :		
	case TMODE_PCTRADE_SELFITEM :
	case TMODE_QUEST :
	case TMODE_NPCQUEST :		
	case TMODE_PERSONALSTORE_TRADEITEM :		
	case TMODE_REPAIRITEM :
	case TMODE_GAMBLE :
	case TMODE_TARGETINFO :
#ifdef _XTS_CASHITEM
	case TMODE_CASHITEM :
#endif
	case TMODE_INCHANT :
	case TMODE_LIFESKILLCOLLECTIONITEM :
	case TMODE_LIFESKILLRESOURCEITEM :
	case TMODE_NPCCOLLECTIONITEM :
	case TMODE_NPCRESOURCEITEM :
		{			
			if( SetTooltipInfo() )
			{
				// Money 공간
				if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM ||
					m_CurrentTooltipMode == TMODE_GAMBLE )
				{
					if( m_ItemType != _XGI_FC_QUEST && !_XGameItem::CheckNPCTrade(m_ItemType, m_ItemId) ) //Author : 양희왕 //breif : 팔수없는 물건은 가격이 나오면 안됨
						m_TooltipSize.cy += DT_ROW_INTERVAL;
				}
				
				// 기여도 공간
				if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
				{
					if( m_pContribution_Window )
					{
						if( m_pContribution_Window->GetShowStatus() && m_ItemType != _XGI_FC_QUEST &&
                            !_XGameItem::CheckNPCTrade(m_ItemType, m_ItemId) ) //Author : 양희왕 //breif : 팔수없는 물건은 가격이 나오면 안됨
							m_TooltipSize.cy += DT_ROW_INTERVAL;
					}
				}
				
				// Expand TooltipSize ----------------------------------=			
				if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_CLOTHES ||
					m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_CLOTHES2 || 
					m_ItemType == _XGI_FC_WEAPON3 || m_ItemType == _XGI_FC_CLOTHES3 )
				{
					if( m_CurrentTooltipMode != TMODE_NPCTRADE_NPCITEM )
					{
						m_ExpandTooltipSize.cx = 194;
						m_ExpandTooltipSize.cy = 62;

						int socketindex;
						switch( m_CurrentTooltipMode ) 
						{
						case TMODE_DEFAULTITEM :
						case TMODE_CINFOITEM :
						case TMODE_NPCTRADE_INVENITEM :							
						case TMODE_QUEST :
						case TMODE_NPCQUEST :
						case TMODE_REPAIRITEM :
						case TMODE_LIFESKILLCOLLECTIONITEM :
						case TMODE_LIFESKILLRESOURCEITEM :
						case TMODE_NPCCOLLECTIONITEM :
						case TMODE_NPCRESOURCEITEM :
							{
								if( m_CurrentTooltipMode == TMODE_QUEST || m_CurrentTooltipMode == TMODE_NPCQUEST )
									m_CurrentItemSocketCount = -1;
								else if( m_CurrentTooltipMode == TMODE_LIFESKILLCOLLECTIONITEM || m_CurrentTooltipMode == TMODE_LIFESKILLRESOURCEITEM 
									|| m_CurrentTooltipMode == TMODE_NPCCOLLECTIONITEM || m_CurrentTooltipMode == TMODE_NPCRESOURCEITEM ) 
									m_CurrentItemSocketCount = -1;
								else
									m_CurrentItemSocketCount = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucSocketCount();

								if( m_CurrentItemSocketCount > 0 )
								{
									m_CurrentItemSockekRealCount = 0;								
									for( int i = 0; i < 4; i ++ )
									{
										socketindex = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_sSocket(i);
										if(  socketindex > 0 )
										{
											m_AddSocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
											m_AddSocketItemList[i].Set_m_cSecond(0);
											m_AddSocketItemList[i].Set_m_sID(socketindex);
											m_AddSocketItemList[i].m_cIdentify			= 0;
											m_AddSocketItemList[i].Set_m_cThird(0);
											m_AddSocketItemList[i].Set_m_ucCount(0);									
											m_AddSocketItemList[i].m_cReserve			= 0;
											m_AddSocketItemList[i].Set_m_usCurDur(0);
											m_AddSocketItemList[i].Set_m_usMaxDur(0);									
											m_AddSocketItemList[i].Set_m_sSocket(0,0);
											m_AddSocketItemList[i].Set_m_sSocket(1,0);
											m_AddSocketItemList[i].Set_m_sSocket(2,0);
											m_AddSocketItemList[i].Set_m_sSocket(3,0);
											m_AddSocketItemList[i].Set_m_ucSocketCount(0);
											m_AddSocketItemList[i].m_bDurabilityDecrease = FALSE;	
											m_AddSocketItemList[i].m_bQuestReward		= FALSE;

											if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_WEAPON3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID1);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID1]->concept, 
														g_SocketItemProperty[socketindex].sValue1 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}

												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID2);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID2]->concept, 
														g_SocketItemProperty[socketindex].sValue2 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}
											else if( m_ItemType == _XGI_FC_CLOTHES || m_ItemType == _XGI_FC_CLOTHES2 || m_ItemType == _XGI_FC_CLOTHES3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID3);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID3]->concept, 
														g_SocketItemProperty[socketindex].sValue3 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}

												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID4);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID4]->concept, 
														g_SocketItemProperty[socketindex].sValue4 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}											
											m_CurrentItemSockekRealCount++;
										}
										else
										{
											m_AddSocketItemList[i].Reset();
										}
									}
								}
								else
								{
									m_CurrentItemSocketCount = -1;
								}
							}		
							break;					
						case TMODE_WAREHOUSE_WAREHOUSEITEM :
							{
								m_CurrentItemSocketCount = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucSocketCount();
								if( m_CurrentItemSocketCount > 0 )
								{
									m_CurrentItemSockekRealCount = 0;
									for( int i = 0; i < 4; i ++ )
									{
										socketindex = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_sSocket(i);
										if(  socketindex > 0 )
										{
											m_AddSocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
											m_AddSocketItemList[i].Set_m_cSecond(0);
											m_AddSocketItemList[i].Set_m_sID(socketindex);
											m_AddSocketItemList[i].m_cIdentify			= 0;
											m_AddSocketItemList[i].Set_m_cThird(0);
											m_AddSocketItemList[i].Set_m_ucCount(0);									
											m_AddSocketItemList[i].m_cReserve			= 0;
											m_AddSocketItemList[i].Set_m_usCurDur(0);
											m_AddSocketItemList[i].Set_m_usMaxDur(0);									
											m_AddSocketItemList[i].Set_m_sSocket(0,0);
											m_AddSocketItemList[i].Set_m_sSocket(1,0);
											m_AddSocketItemList[i].Set_m_sSocket(2,0);
											m_AddSocketItemList[i].Set_m_sSocket(3,0);
											m_AddSocketItemList[i].Set_m_ucSocketCount(0);									
											m_AddSocketItemList[i].m_bDurabilityDecrease = FALSE;	
											m_AddSocketItemList[i].m_bQuestReward		= FALSE;
	
											if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_WEAPON3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID1);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID1]->concept, 
														g_SocketItemProperty[socketindex].sValue1 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}

												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID2);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID2]->concept, 
														g_SocketItemProperty[socketindex].sValue2 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}
											else if( m_ItemType == _XGI_FC_CLOTHES || m_ItemType == _XGI_FC_CLOTHES2 || m_ItemType == _XGI_FC_CLOTHES3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID3);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID3]->concept, 
														g_SocketItemProperty[socketindex].sValue3 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID4);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID4]->concept, 
														g_SocketItemProperty[socketindex].sValue4 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}

											m_CurrentItemSockekRealCount++;
										}
										else
										{
											m_AddSocketItemList[i].Reset();
										}
									}
								}
								else
								{
									m_CurrentItemSocketCount = -1;							
								}
							}
							break;
						case TMODE_PCTRADE_TARGETITEM :
							{
								m_CurrentItemSocketCount = m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucSocketCount();
								if( m_CurrentItemSocketCount > 0 )
								{
									m_CurrentItemSockekRealCount = 0;
									for( int i = 0; i < 4; i++ )
									{
										socketindex = m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_sSocket(i);
										if(  socketindex > 0 )
										{
											m_AddSocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
											m_AddSocketItemList[i].Set_m_cSecond(0);
											m_AddSocketItemList[i].Set_m_sID(socketindex);
											m_AddSocketItemList[i].m_cIdentify			= 0;
											m_AddSocketItemList[i].Set_m_cThird(0);
											m_AddSocketItemList[i].Set_m_ucCount(0);									
											m_AddSocketItemList[i].m_cReserve			= 0;
											m_AddSocketItemList[i].Set_m_usCurDur(0);
											m_AddSocketItemList[i].Set_m_usMaxDur(0);									
											m_AddSocketItemList[i].Set_m_sSocket(0,0);
											m_AddSocketItemList[i].Set_m_sSocket(1,0);
											m_AddSocketItemList[i].Set_m_sSocket(2,0);
											m_AddSocketItemList[i].Set_m_sSocket(3,0);
											m_AddSocketItemList[i].Set_m_ucSocketCount(0);									
											m_AddSocketItemList[i].m_bDurabilityDecrease = FALSE;	
											m_AddSocketItemList[i].m_bQuestReward		= FALSE;

											if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_WEAPON3 )
											{
												// id1, id2 는 무기쪽 
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID1);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID1]->concept, 
														g_SocketItemProperty[socketindex].sValue1 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID2);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID2]->concept, 
														g_SocketItemProperty[socketindex].sValue2 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}
											else if( m_ItemType == _XGI_FC_CLOTHES || m_ItemType == _XGI_FC_CLOTHES2 || m_ItemType == _XGI_FC_CLOTHES3 )
											{
												// id3, id4 는 의상쪽
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID3);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID3]->concept, 
														g_SocketItemProperty[socketindex].sValue3 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID4);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID4]->concept, 
														g_SocketItemProperty[socketindex].sValue4 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}

											m_CurrentItemSockekRealCount++;
										}
										else
										{
											m_AddSocketItemList[i].Reset();
										}
									}
								}
								else
								{
									m_CurrentItemSocketCount = -1;
								}						
							}
							break;
						case TMODE_PCTRADE_SELFITEM :
							{
								m_CurrentItemSocketCount = m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucSocketCount();
								if( m_CurrentItemSocketCount > 0 )
								{
									m_CurrentItemSockekRealCount = 0;
									for( int i = 0; i < 4; i++ )
									{
										socketindex = m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_sSocket(i);
										if(  socketindex > 0 )
										{
											m_AddSocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
											m_AddSocketItemList[i].Set_m_cSecond(0);
											m_AddSocketItemList[i].Set_m_sID(socketindex);
											m_AddSocketItemList[i].m_cIdentify			= 0;
											m_AddSocketItemList[i].Set_m_cThird(0);
											m_AddSocketItemList[i].Set_m_ucCount(0);
											m_AddSocketItemList[i].m_cReserve			= 0;
											m_AddSocketItemList[i].Set_m_usCurDur(0);
											m_AddSocketItemList[i].Set_m_usMaxDur(0);									
											m_AddSocketItemList[i].Set_m_sSocket(0,0);
											m_AddSocketItemList[i].Set_m_sSocket(1,0);
											m_AddSocketItemList[i].Set_m_sSocket(2,0);
											m_AddSocketItemList[i].Set_m_sSocket(3,0);
											m_AddSocketItemList[i].Set_m_ucSocketCount(0);									
											m_AddSocketItemList[i].m_bDurabilityDecrease = FALSE;	
											m_AddSocketItemList[i].m_bQuestReward		= FALSE;

											if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_WEAPON3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID1);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID1]->concept, 
														g_SocketItemProperty[socketindex].sValue1 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID2);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID2]->concept, 
														g_SocketItemProperty[socketindex].sValue2 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}
											else if( m_ItemType == _XGI_FC_CLOTHES || m_ItemType == _XGI_FC_CLOTHES2 || m_ItemType == _XGI_FC_CLOTHES3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID3);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID3]->concept, 
														g_SocketItemProperty[socketindex].sValue3 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID4);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID4]->concept, 
														g_SocketItemProperty[socketindex].sValue4 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}

											m_CurrentItemSockekRealCount++;
										}
										else
										{
											m_AddSocketItemList[i].Reset();
										}
									}							
								}
								else
								{
									m_CurrentItemSocketCount = -1;
								}						
							}
							break;
						case TMODE_PERSONALSTORE_TRADEITEM :
							{
								m_CurrentItemSocketCount = m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucSocketCount();
								if( m_CurrentItemSocketCount > 0 )
								{
									m_CurrentItemSockekRealCount = 0;
									for( int i = 0; i < 4; i++ )
									{
										socketindex = m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_sSocket(i);
										if(  socketindex > 0 )
										{
											m_AddSocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
											m_AddSocketItemList[i].Set_m_cSecond(0);
											m_AddSocketItemList[i].Set_m_sID(socketindex);
											m_AddSocketItemList[i].m_cIdentify			= 0;
											m_AddSocketItemList[i].Set_m_cThird(0);
											m_AddSocketItemList[i].Set_m_ucCount(0);
											m_AddSocketItemList[i].m_cReserve			= 0;
											
											m_AddSocketItemList[i].Set_m_usCurDur(0);
											m_AddSocketItemList[i].Set_m_usMaxDur(0);									
											m_AddSocketItemList[i].Set_m_sSocket(0,0);
											m_AddSocketItemList[i].Set_m_sSocket(1,0);
											m_AddSocketItemList[i].Set_m_sSocket(2,0);
											m_AddSocketItemList[i].Set_m_sSocket(3,0);
											m_AddSocketItemList[i].Set_m_ucSocketCount(0);
											
											m_AddSocketItemList[i].m_bDurabilityDecrease = FALSE;	
											m_AddSocketItemList[i].m_bQuestReward		= FALSE;

											if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_WEAPON3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID1);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb1 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID1]->concept, 
														g_SocketItemProperty[socketindex].sValue1 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID2);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb2 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID2]->concept, 
														g_SocketItemProperty[socketindex].sValue2 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}
											else if( m_ItemType == _XGI_FC_CLOTHES || m_ItemType == _XGI_FC_CLOTHES2 || m_ItemType == _XGI_FC_CLOTHES3 )
											{
												map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID3);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb3 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID3]->concept, 
														g_SocketItemProperty[socketindex].sValue3 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
												iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID4);
												if(iter_table != g_CharacterStateTable.end())
												{
													if( g_SocketItemProperty[socketindex].sAP_Per4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}						
													if( g_SocketItemProperty[socketindex].cProb4 )
													{
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													
													sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID4]->concept, 
														g_SocketItemProperty[socketindex].sValue4 );
													strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
												}
											}

											m_CurrentItemSockekRealCount++;
										}
										else
										{
											m_AddSocketItemList[i].Reset();
										}
									}							
								}
								else
								{
									m_CurrentItemSocketCount = -1;
								}						
							}
							break;
						case TMODE_TARGETINFO :
							{
								if( m_pTargetInfo_Window )
								{
									m_CurrentItemSocketCount = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucSocketCount();
									if( m_CurrentItemSocketCount > 0 )
									{
										m_CurrentItemSockekRealCount = 0;
										for( int i = 0; i < 4; i++ )
										{
											socketindex = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_sSocket(i);
											if(  socketindex > 0 )
											{
												m_AddSocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
												m_AddSocketItemList[i].Set_m_cSecond(0);
												m_AddSocketItemList[i].Set_m_sID(socketindex);
												m_AddSocketItemList[i].m_cIdentify			= 0;
												m_AddSocketItemList[i].Set_m_cThird(0);
												m_AddSocketItemList[i].Set_m_ucCount(0);
												m_AddSocketItemList[i].m_cReserve			= 0;
												m_AddSocketItemList[i].Set_m_usCurDur(0);
												m_AddSocketItemList[i].Set_m_usMaxDur(0);									
												m_AddSocketItemList[i].Set_m_sSocket(0,0);
												m_AddSocketItemList[i].Set_m_sSocket(1,0);
												m_AddSocketItemList[i].Set_m_sSocket(2,0);
												m_AddSocketItemList[i].Set_m_sSocket(3,0);
												m_AddSocketItemList[i].Set_m_ucSocketCount(0);												
												m_AddSocketItemList[i].m_bDurabilityDecrease = FALSE;	
												m_AddSocketItemList[i].m_bQuestReward		= FALSE;

												if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_WEAPON3 )
												{
													map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID1);
													if(iter_table != g_CharacterStateTable.end())
													{
														if( g_SocketItemProperty[socketindex].sAP_Per1 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per1 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
														}						
														if( g_SocketItemProperty[socketindex].cProb1 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb1 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
														}
														
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID1]->concept, 
															g_SocketItemProperty[socketindex].sValue1 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID2);
													if(iter_table != g_CharacterStateTable.end())
													{
														if( g_SocketItemProperty[socketindex].sAP_Per2 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per2 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
														}						
														if( g_SocketItemProperty[socketindex].cProb2 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb2 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
														}
														
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID2]->concept, 
															g_SocketItemProperty[socketindex].sValue2 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
												}
												else if( m_ItemType == _XGI_FC_CLOTHES || m_ItemType == _XGI_FC_CLOTHES2 || m_ItemType == _XGI_FC_CLOTHES3 )
												{
													map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID3);
													if(iter_table != g_CharacterStateTable.end())
													{
														if( g_SocketItemProperty[socketindex].sAP_Per3 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per3 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
														}						
														if( g_SocketItemProperty[socketindex].cProb3 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb3 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
														}
														
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID3]->concept, 
															g_SocketItemProperty[socketindex].sValue3 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
													iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketindex].sSkill_Effect_ID4);
													if(iter_table != g_CharacterStateTable.end())
													{
														if( g_SocketItemProperty[socketindex].sAP_Per4 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[socketindex].sAP_Per4 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
														}						
														if( g_SocketItemProperty[socketindex].cProb4 )
														{
															sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[socketindex].cProb4 );
															strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
														}
														
														sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[socketindex].sSkill_Effect_ID4]->concept, 
															g_SocketItemProperty[socketindex].sValue4 );
														strcat( m_CurrentTooltipInfo.E_Data_Buf[i+4], m_CurrentTooltipInfo.Temp_Data_Buf );
													}
												}

												m_CurrentItemSockekRealCount++;
											}
											else
											{
												m_AddSocketItemList[i].Reset();
											}
										}							
									}
									else
									{
										m_CurrentItemSocketCount = -1;
									}								
								}								
							}
							break;
						default:
							m_CurrentItemSocketCount = -1;						
							socketindex = -1;
							break;
						}
						
						if( m_CurrentItemSocketCount < 0 )
						{
							m_ExpandTooltipSize.cx = 0;
							m_ExpandTooltipSize.cy = 0;
						}
						else
						{
							m_ExpandTooltipSize.cx = 194;
							int addeffectindex = _XGameItem::GetAddEffectFromSocket(m_AddSocketItemList[0].Get_m_sID(), m_AddSocketItemList[1].Get_m_sID(), m_AddSocketItemList[2].Get_m_sID(), m_AddSocketItemList[3].Get_m_sID());

							if( addeffectindex )
							{
								if( m_ItemType == _XGI_FC_WEAPON || m_ItemType == _XGI_FC_WEAPON2 || m_ItemType == _XGI_FC_WEAPON3 )
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addeffectindex].sSkill_Effect_ID1);
									if(iter_table != g_CharacterStateTable.end())
									{
										if( g_SocketItemProperty[addeffectindex].sAP_Per1 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[addeffectindex].sAP_Per1 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[8], m_CurrentTooltipInfo.Temp_Data_Buf );
										}						
										if( g_SocketItemProperty[addeffectindex].cProb1 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[addeffectindex].cProb1 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[8], m_CurrentTooltipInfo.Temp_Data_Buf );
										}
										
										sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[addeffectindex].sSkill_Effect_ID1]->concept, 
											g_SocketItemProperty[addeffectindex].sValue1 );
										strcat( m_CurrentTooltipInfo.E_Data_Buf[8], m_CurrentTooltipInfo.Temp_Data_Buf );
									}

									iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addeffectindex].sSkill_Effect_ID2);
									if(iter_table != g_CharacterStateTable.end())
									{
										if( g_SocketItemProperty[addeffectindex].sAP_Per2 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[addeffectindex].sAP_Per2 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[9], m_CurrentTooltipInfo.Temp_Data_Buf );
										}						
										if( g_SocketItemProperty[addeffectindex].cProb2 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[addeffectindex].cProb2 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[9], m_CurrentTooltipInfo.Temp_Data_Buf );
										}
										
										sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[addeffectindex].sSkill_Effect_ID2]->concept, 
											g_SocketItemProperty[addeffectindex].sValue2 );
										strcat( m_CurrentTooltipInfo.E_Data_Buf[9], m_CurrentTooltipInfo.Temp_Data_Buf );
									}
								}
								else if( m_ItemType == _XGI_FC_CLOTHES || m_ItemType == _XGI_FC_CLOTHES2 || m_ItemType == _XGI_FC_CLOTHES3 )
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addeffectindex].sSkill_Effect_ID3);
									if(iter_table != g_CharacterStateTable.end())
									{
										if( g_SocketItemProperty[addeffectindex].sAP_Per3 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[addeffectindex].sAP_Per3 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[8], m_CurrentTooltipInfo.Temp_Data_Buf );
										}						
										if( g_SocketItemProperty[addeffectindex].cProb3 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[addeffectindex].cProb3 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[8], m_CurrentTooltipInfo.Temp_Data_Buf );
										}
										
										sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[addeffectindex].sSkill_Effect_ID3]->concept, 
											g_SocketItemProperty[addeffectindex].sValue3 );
										strcat( m_CurrentTooltipInfo.E_Data_Buf[8], m_CurrentTooltipInfo.Temp_Data_Buf );
									}

									iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addeffectindex].sSkill_Effect_ID4);
									if(iter_table != g_CharacterStateTable.end())
									{
										if( g_SocketItemProperty[addeffectindex].sAP_Per4 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[addeffectindex].sAP_Per4 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[9], m_CurrentTooltipInfo.Temp_Data_Buf );
										}						
										if( g_SocketItemProperty[addeffectindex].cProb4 )
										{
											sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[addeffectindex].cProb4 );
											strcat( m_CurrentTooltipInfo.E_Data_Buf[9], m_CurrentTooltipInfo.Temp_Data_Buf );
										}
										
										sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[addeffectindex].sSkill_Effect_ID4]->concept, 
											g_SocketItemProperty[addeffectindex].sValue3 );
										strcat( m_CurrentTooltipInfo.E_Data_Buf[9], m_CurrentTooltipInfo.Temp_Data_Buf );
									}
								}
							}

							for( int i = 0; i < 10; i++ )
							{
								if( strlen( m_CurrentTooltipInfo.E_Data_Buf[i] ) )
								{
									int length = strlen( m_CurrentTooltipInfo.E_Data_Buf[i] ) / 29;
									m_ExpandTooltipSize.cy += DT_ROW_INTERVAL*(length+1);
								}							
							}
						}
					}	
				}
				// -----------------------------------------------------=				
				POINT position;
				SIZE tooltipsize;
				RECT slotposition;
				tooltipsize.cx = m_TooltipSize.cx;
				tooltipsize.cy = m_TooltipSize.cy + m_ExpandTooltipSize.cy + m_SetItemExpandTooltipSize.cy; //Author : 양희왕 breif : 세트 아이템
				SetWindowSize(m_TooltipSize);

				switch( m_CurrentTooltipMode ) 
				{
				case TMODE_DEFAULTITEM :
				case TMODE_NPCTRADE_INVENITEM :
					{
						g_pInventory_Window->SearchSlotRegion( &slotposition );
					}		
					break;
				case TMODE_CINFOITEM :
					{
						g_pCInfo_Window->SearchSlotRegion( & slotposition );
					}
					break;
				case TMODE_NPCTRADE_NPCITEM :
				case TMODE_GAMBLE :
					{
						if( m_pNPCTradeWindow )
						{
							if( m_pNPCTradeWindow->GetShowStatus() )
							{
								POINT pos = m_pNPCTradeWindow->GetWindowPos();
								int drawslotnumber = m_CurrentSlotNumber % _XDEF_MAX_NPCTRADESLOT;
								slotposition.left = pos.x + m_pNPCTradeWindow->m_SlotPosition[drawslotnumber].x;
								slotposition.top = pos.y + m_pNPCTradeWindow->m_SlotPosition[drawslotnumber].y;
							}						
						}
					}
					break;
				case TMODE_WAREHOUSE_WAREHOUSEITEM :
					{
						if( m_pWarehouseWindow )
						{
							if( m_pWarehouseWindow->GetShowStatus() )						
							{
								POINT pos = m_pWarehouseWindow->GetWindowPos();
								int drawslotnumber = m_CurrentSlotNumber % _XDEF_WAREHOUSE_MAX_SENDCOUNT;
								slotposition.left = pos.x + m_pWarehouseWindow->m_SlotPosition[drawslotnumber].x;
								slotposition.top = pos.y + m_pWarehouseWindow->m_SlotPosition[drawslotnumber].y;
							}
						}
					}
					break;
				case TMODE_PCTRADE_TARGETITEM :
					{
						if( m_pPCTradeWindow )
						{
							if( m_pPCTradeWindow->GetShowStatus() )
							{
								POINT pos = m_pPCTradeWindow->GetWindowPos();
								slotposition.left = pos.x + m_pPCTradeWindow->m_TargetSlotPosition[m_CurrentSlotNumber].x;
								slotposition.top = pos.y + m_pPCTradeWindow->m_TargetSlotPosition[m_CurrentSlotNumber].y;
							}
						}
					}
					break;
				case TMODE_PCTRADE_SELFITEM :
					{
						if( m_pPCTradeWindow) 
						{
							if( m_pPCTradeWindow->GetShowStatus() )
							{
								POINT pos = m_pPCTradeWindow->GetWindowPos();
								slotposition.left = pos.x + m_pPCTradeWindow->m_SelfSlotPosition[m_CurrentSlotNumber].x;
								slotposition.top = pos.y + m_pPCTradeWindow->m_SelfSlotPosition[m_CurrentSlotNumber].y;
							}
						}
					}
					break;
				case TMODE_QUEST :
					{
						if( g_pQuest_Window->GetShowStatus() )
						{					
							m_ItemType = type;
							m_ItemId = id;
							POINT pos = g_pQuest_Window->m_listboxQuestReward->GetWindowPos();							
							slotposition.left = pos.x +(m_CurrentSlotNumber*34) + 2;
							slotposition.top = pos.y ;
						}						
					}
					break;
				case TMODE_NPCQUEST :
					{
						if( m_pNPCScript_Window )
						{
							if( m_pNPCScript_Window->GetShowStatus() )
							{
								m_ItemType = type;
								m_ItemId = id;
								POINT pos = m_pNPCScript_Window->m_listboxQuestReward->GetWindowPos();							
								slotposition.left = pos.x +(m_CurrentSlotNumber*36) + 2;
								slotposition.top = pos.y;
							}
						}
					}		
					break;
				case TMODE_PERSONALSTORE_TRADEITEM :
					{
						if( m_pPersonalStore_Window) 
						{
							if( m_pPersonalStore_Window->GetShowStatus() )
							{
								POINT pos = m_pPersonalStore_Window->GetWindowPos();
								slotposition.left = pos.x + m_pPersonalStore_Window->m_TradeItemSlotPosition[m_CurrentSlotNumber].x;
								slotposition.top = pos.y + m_pPersonalStore_Window->m_TradeItemSlotPosition[m_CurrentSlotNumber].y;
							}
						}
					}
					break;
				case TMODE_REPAIRITEM :
					{
						if( m_pRepairItem_Window )
						{
							if( m_pRepairItem_Window->GetShowStatus() )
							{
								int i = 0;
								list<int>::iterator k = m_pRepairItem_Window->m_SelectedSlotNoList.begin();
								for( ; k!= m_pRepairItem_Window->m_SelectedSlotNoList.end(); k++ )
								{
									if( *k == m_CurrentSlotNumber )
									{
										POINT pos = m_pRepairItem_Window->GetWindowPos();
										slotposition.left = pos.x + m_pRepairItem_Window->m_RepairSlotPosition[i].x;
										slotposition.top = pos.y + m_pRepairItem_Window->m_RepairSlotPosition[i].y;
										break;
									}
									i++;
								}								
							}
						}
					}
					break;
				case TMODE_TARGETINFO :
					{
						if( m_pTargetInfo_Window )
						{
							if( m_pTargetInfo_Window->GetShowStatus() )
							{
								m_pTargetInfo_Window->SearchSlotRegion( &slotposition );
							}								
						}						
					}
					break;
#ifdef _XTS_CASHITEM
				case TMODE_CASHITEM :
					{
						if( m_pCashItem_Window )
						{
							if( m_pCashItem_Window->GetShowStatus() )
							{
								m_pCashItem_Window->SearchSlotRegion( &slotposition );
							}
						}
					}
					break;
#endif
				case TMODE_LIFESKILLCOLLECTIONITEM :
				case TMODE_LIFESKILLRESOURCEITEM :
					{
						if( g_pLifeSkill_Window )
						{
							if( g_pLifeSkill_Window->GetShowStatus() )
							{
								g_pLifeSkill_Window->SearchSlotRegion( &slotposition );
							}
						}
					}
					break;
				case TMODE_NPCCOLLECTIONITEM :
					{
						if( m_pNPCCollection_Window )
						{
							if( m_pNPCCollection_Window->GetShowStatus() )
							{
								POINT pos = m_pNPCCollection_Window->GetWindowPos();
								int column = slotnumber%3;
								int row = slotnumber/3;
																	
								slotposition.left = pos.x + 248+(column*35);
								slotposition.top = pos.y + 95+(42*row);
							}
						}
					}
					break;
				case TMODE_NPCRESOURCEITEM :
					{
#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI
						if(m_pNPCResource_New_Window)
						{
							if(m_pNPCResource_New_Window->GetShowStatus())
							{
								POINT pos = m_pNPCResource_New_Window->GetWindowPos();
								int column = 0, row = 0;

								if(slotnumber > 0)
								{
									column = (slotnumber-1)%4;
									row = (slotnumber-1)/4;
									slotposition.left = pos.x + 20+(column*37);
									slotposition.top = pos.y + 87+(36*row);
								}
								else
								{
									slotposition.left = pos.x + 21;
									slotposition.top = pos.y + 54;
								}
							}
						}

#else
						if( m_pNPCResource_Window )
						{
							if( m_pNPCResource_Window->GetShowStatus() )
							{
								POINT pos = m_pNPCResource_Window->GetWindowPos();
								int column = slotnumber%4;
								int row = slotnumber/4;

								slotposition.left = pos.x + 212+(column*37);
								slotposition.top = pos.y + 53+(36*row);
							}
						}
#endif
					}
					break;
				case TMODE_INCHANT :
					{
						if( m_pInchant_Window )
						{
							if( m_pInchant_Window->GetShowStatus() )
							{
								POINT pos = m_pInchant_Window->GetWindowPos();

								slotposition.left = pos.x + 45;
								slotposition.top = pos.y + 157;
							}
						}

#ifdef _XDEF_SMELT_ITEM // Author : 양희왕
						XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
						if( pSmletItemWindow && pSmletItemWindow->GetShowStatus() )
						{
							POINT pos = pSmletItemWindow->GetWindowPos();

							slotposition.left = pos.x + 45;
							slotposition.top = pos.y + 157;
						}
#endif
					}
					break;
				}				
				
				position.x = slotposition.left + 22;
				position.y = slotposition.top + 22;
				
				if( slotposition.left + 22 + tooltipsize.cx >= gnWidth )
				{
					position.x = slotposition.left + 10 - tooltipsize.cx;
					if( position.x < 0 ) position.x = 0;
				}
				if( slotposition.top + 22 + tooltipsize.cy + 98 >= gnHeight )
				{
					position.y = slotposition.top + 10 - tooltipsize.cy;
					if( position.y < 0 ) position.y = 0;
				}
				
				MoveWindow( position.x, position.y  );
			}
			else
			{
				return FALSE;			
			}
		}			
		break;	
	case TMODE_SKILL :
		{
			if( SetTooltipInfo() )
			{
				if( g_pSkill_Window )
				{
					if( g_pSkill_Window->GetShowStatus() )
					{
						_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);					
						if( pMainFrame_Window )
						{
							if(  pMainFrame_Window->GetShowStatus() )
							{
								POINT pos = pMainFrame_Window->GetWindowPos();
								SIZE size = pMainFrame_Window->GetWindowSize();
								MoveWindow( pos.x + size.cx, pos.y );
#ifdef _XTS_DELETESKILL
//#ifdef _XTS_NEWSKILLWINDOW
								m_DeleteSkillBtn->ShowWindow(TRUE);
								m_DeleteSkillBtn->MoveWindow(m_WindowPosition.x+m_TooltipSize.cx-70, m_WindowPosition.y+m_TooltipSize.cy-30 );
#endif
							}					
						}
					}
				}
			}
			else
				return FALSE;
		}
		break;	
	case TMODE_TRADESKILL :
		{
			if( SetTooltipInfo() )
			{
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);

				if( pNPCScript_Window )
				{
					if( pNPCScript_Window->GetShowStatus() )
					{
						POINT pos = pNPCScript_Window->GetWindowPos();
						MoveWindow( pos.x - m_TooltipSize.cx, pos.y-48);
					}				
				}
			}
			else return FALSE;
		}
		break;
	case TMODE_MASTERYSKILL :
		{
			if(!_XSkillItem::FindMasterySkillProperty(m_ItemId))
				break;		// error

			if( SetTooltipInfo() )
			{
				if( g_pMasterySkill_Window )
				{
					if( g_pMasterySkill_Window->GetShowStatus() )
					{
						_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);					
						if( pMainFrame_Window )
						{
							if( pMainFrame_Window->GetShowStatus() )
							{
								POINT pos = pMainFrame_Window->GetWindowPos();
								SIZE size = pMainFrame_Window->GetWindowSize();
								MoveWindow( pos.x + size.cx, pos.y );
							}
						}
					}
				}
			}
		}
		break;
	}

	this->SetWindowSize(m_TooltipSize.cx, m_TooltipSize.cy);
	
	return TRUE;	
}
BOOL _XWindow_DefaultTooltip::SetTooltipInfo()
{
	if( m_CurrentTooltipMode < TMODE_SKILL )		// Item Tooltip
	{
		if( m_CurrentTooltipMode == TMODE_GAMBLE )
		{
			if( m_ItemId > 0 )
			{
				m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
				TCHAR TempStr[128];
				memset( TempStr, 0, sizeof(TCHAR)* 128 );

#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_ItemType) )
				{
					strcpy( m_CurrentTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );
				}
				else if( _XGI_FC_CLOTHES == GetEqualItemType(m_ItemType) )
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
					if( ClothesItem )
					{
						if( ClothesItem[m_ItemId].cSexType )
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2012), 
								g_pClothesItemClanName[ClothesItem[m_ItemId].cClan], 
								g_pClothesItemSecondName[ClothesItem[m_ItemId].cSecondType] );
						}
						else
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2013), 
								g_pClothesItemClanName[ClothesItem[m_ItemId].cClan], 
								g_pClothesItemSecondName[ClothesItem[m_ItemId].cSecondType] );
						}
					}
				}
#else
				switch(m_ItemType) 
				{
				case _XGI_FC_WEAPON :	
				case _XGI_FC_WEAPON2 :
				case _XGI_FC_WEAPON3 :
					{
						strcpy( m_CurrentTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );
					}					
					break;
				case _XGI_FC_CLOTHES :					
					{
						if( g_ClothesItemProperty[m_ItemId].cSexType )
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2012), 
								g_pClothesItemClanName[g_ClothesItemProperty[m_ItemId].cClan], 
								g_pClothesItemSecondName[g_ClothesItemProperty[m_ItemId].cSecondType] );
						}
						else
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2013), 
								g_pClothesItemClanName[g_ClothesItemProperty[m_ItemId].cClan], 
								g_pClothesItemSecondName[g_ClothesItemProperty[m_ItemId].cSecondType] );
						}
					}
					break;
				case _XGI_FC_CLOTHES2 :
					{
						if( g_ClothesItemProperty[m_ItemId].cSexType )
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2014), 
								g_pClothesItemClanName[g_ClothesItemProperty2[m_ItemId].cClan], 
								g_pClothesItemSecondName[g_ClothesItemProperty2[m_ItemId].cSecondType] );
						}
						else
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2015), 
								g_pClothesItemClanName[g_ClothesItemProperty2[m_ItemId].cClan], 
								g_pClothesItemSecondName[g_ClothesItemProperty2[m_ItemId].cSecondType] );
						}
					}
					break;
				case _XGI_FC_CLOTHES3 :
					{
						if( g_ClothesItemProperty[m_ItemId].cSexType )
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2014), 
								g_pClothesItemClanName[g_ClothesItemProperty3[m_ItemId].cClan], 
								g_pClothesItemSecondName[g_ClothesItemProperty3[m_ItemId].cSecondType] );
						}
						else
						{
							sprintf( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2015), 
								g_pClothesItemClanName[g_ClothesItemProperty3[m_ItemId].cClan], 
								g_pClothesItemSecondName[g_ClothesItemProperty3[m_ItemId].cSecondType] );
						}
					}
					break;
				}	
#endif
				
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;			
				
				strcpy( m_CurrentTooltipInfo.DescBuf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2016) );
				
				m_TooltipSize.cy += DT_LINE_INTERVAL*3;
				
				length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
				
#ifdef _XDEF_PERIODITEM_20061027
				int cashcheck = 0;
				int leftday = 0;

				if( _XGI_FC_WEAPON == GetEqualItemType(m_ItemType) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
					cashcheck = WeaponItem[m_ItemId].cCashCheck;
					leftday = WeaponItem[m_ItemId].usTimeValue;
				}
				else if( _XGI_FC_CLOTHES == GetEqualItemType(m_ItemType) )
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
					cashcheck = ClothesItem[m_ItemId].cCashCheck;
					leftday = ClothesItem[m_ItemId].usTimeValue;
				}
				// 기간제 아이템
				if(cashcheck == 2 || cashcheck == 3)
				{
					// 기간제 아이템이면 남은 시간 표시
					m_TooltipSize.cy += DT_ROW_INTERVAL;

					sprintf(m_CurrentTooltipInfo.Data_Buf[13], _XGETINTERFACETEXT(ID_STRING_NEW_3668), leftday); //구매 후 %d일간 사용 가능
					
					int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
					if( length > 1 )
						m_TooltipSize.cy += DT_ROW_INTERVAL*(length-1);
				}
#endif
			}
		}
		else
		{			
			char itemtype = GetEqualItemType(m_ItemType);
			if( itemtype != -1 )
			{
				switch(itemtype) 
				{
				case _XGI_FC_WEAPON :	
					SetInfoWeaponItem();				
					break;
				case _XGI_FC_CLOTHES :
					SetInfoClothesItem();				
					break;			
				case _XGI_FC_BOOK :
					SetInfoBook();				
					break;
				case _XGI_FC_POTION :
					SetInfoPotion();				
					break;
				case _XGI_FC_CONSUMING :
					SetInfoConsuming();
					break;
				case _XGI_FC_ELIXIR :
					SetElixir();
					break;
				case _XGI_FC_RESOURCE :
					SetInfoResource();
					break;
				case  _XGI_FC_QUEST :
					SetInfoQuest();				
					break;
				case _XGI_FC_ACCESSORY :
					SetInfoAccessory();				
					break;
				case _XGI_FC_LIFE :
					SetInfoLife();				
					break;	
				case _XGI_FC_MONEY :
					{
						m_TooltipSize.cy += DT_ROW_INTERVAL*2;
						strcpy( m_CurrentTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_NETWORK_MONEY) );
					}
					break;
				case _XGI_FC_SOCKET :
					SetInfoSocket();							
					break;
				case _XGI_FC_BOX :
					SetInfoBox();
					break;
				case _XGI_FC_BOXKEY :
					SetInfoBoxKey();
					break;
				}
			}
		}		
	}
	else if( m_CurrentTooltipMode < TMODE_MASTERYSKILL ) 		// Skill, TradeSkill Tooltip
	{//Author : 양희왕 //breif : 무공창에 툴팁이 안나온다. 뭔가 이상함...
		_XUserSkill* pSkill = NULL;
		int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_ItemId, TRUE);

		if(learnedindex == _XDEF_MAX_LEARNSKILL)
		{
			pSkill = &g_pLocalUser->m_EventSkillList;
		}
		else
		{
			pSkill = g_pLocalUser->GetSkillItem(m_ItemId); //Author : 양희왕 //breif : 무공전수 시에 NULL 이 리턴됨
		}

		if( strlen( g_SkillProperty[m_ItemId]->skillName ) )	// 이름
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL*2;

#ifdef _XJAPANESE
			if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
			{
				if( g_SkillProperty[m_ItemId]->skillHiraganaName && strlen( g_SkillProperty[m_ItemId]->skillHiraganaName ) )
				{
					sprintf( m_CurrentTooltipInfo.NameBuf, _T("%s (%s)"), g_SkillProperty[m_ItemId]->skillName, g_SkillProperty[m_ItemId]->skillHiraganaName );
				}
				else
				{
					strcpy( m_CurrentTooltipInfo.NameBuf, g_SkillProperty[m_ItemId]->skillName );
				}
			}
			else
#endif
			{
				strcpy( m_CurrentTooltipInfo.NameBuf, g_SkillProperty[m_ItemId]->skillName );
			}
		}

		if( strlen( g_SkillProperty[m_ItemId]->concept ) )	// 설명
		{
			strcpy( m_CurrentTooltipInfo.DescBuf, g_SkillProperty[m_ItemId]->concept );
			
			m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}

		m_TooltipSize.cy += DT_ROW_INTERVAL;
		switch(g_SkillProperty[m_ItemId]->clanType)		// 문파
		{
		case _XGROUP_NANGIN :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00));
			break;
		case _XGROUP_GAEBANG :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01));
			break;
		case _XGROUP_BEEGOONG :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02));
			break;
		case _XGROUP_SORIM :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03));
			break;
		case _XGROUP_NOCKRIM :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04));
			break;
		case _XGROUP_MOODANG :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05));
			break;
		case _XGROUP_MAKYO :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06));
			break;
		case _XGROUP_SEGA :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07));
			break;
		case _XGROUP_NOFIXED :
			strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08));
			break;
		}
		
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		strcpy( m_CurrentTooltipInfo.Data_Buf[1], g_pSkillUsefulTypeName[g_SkillProperty[m_ItemId]->usefulType-1] );	// 기술

		m_TooltipSize.cy += DT_ROW_INTERVAL;
		switch(g_SkillProperty[m_ItemId]->weaponCType)
		{
		case _XGI_SC_WEAPON_SHORT :
			{
				if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SWORD)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SWORD));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_KNIFE)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_KNIFE));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_CLUB)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CLUB));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_HIT)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIT));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_FIST)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FIST));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SECRET)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SECRETWEAPON));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_AXE)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_AXE));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_RING)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_RING));
			}
			break;
		case _XGI_SC_WEAPON_LONG :
			{
				if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_GLAIVE)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GLAIVE));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SPEAR)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SPEAR));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_STAFF)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_STAFF));
			}
			break;
		case _XGI_SC_WEAPON_SOFT :
			{
				if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_STEELWHIP)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FLAIL));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_HAMMER)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HAMMER));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_CHAIN)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CHAIN));
			}
			break;
		case _XGI_SC_WEAPON_HIDDEN :
			{
				if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_DART)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DART));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_DAGGER)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DAGGER));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_JAVELIN)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_PROJECTILE));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_NEEDLE)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NEEDLE));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_ARROWLAUNCHER)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALLOWLAUCHER));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_MINE)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_BOMB));
			}
			break;
		case _XGI_SC_WEAPON_MUSICAL :
			{
				if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_RYUNG)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_RYUNG));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_GO)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GO));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SO)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SO));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_KUEM)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_KUEM));
				else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_JUCK)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_JUCK));
			}
		case _XGI_SC_WEAPON_SPECIAL :
			{
				if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SERING)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SERING));
			}
		case -1 :
			{
				if(g_SkillProperty[m_ItemId]->weaponSType == 0)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALLWEAPON));
				else if(g_SkillProperty[m_ItemId]->weaponSType == -1)
					strcpy(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NOWEAPON));
			}
			break;
		}
		
		TCHAR		ferastring[128];
		memset(ferastring, 0, sizeof(TCHAR) * 128);
		switch(g_SkillProperty[m_ItemId]->FERA)
		{
		case _XSI_FERA_MU :
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MU));
			}
			break;
		case _XSI_FERA_MOK :
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MOK));
			}
			break;
		case _XSI_FERA_HWA :
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HWA));
			}
			break;
		case _XSI_FERA_TO :
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TO));
			}
			break;
		case _XSI_FERA_KUM :
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_KUM));
			}
			break;
		case _XSI_FERA_SU :
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SU));
			}
			break;
		case _XSI_FERA_JI :
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_JI));
			}
			break;
		default:
			{
				strcpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MU));
			}
			break;
		}

		sprintf( m_CurrentTooltipInfo.Data_Buf[3], "%s (%s)", g_SkillConceptProperty[g_SkillProperty[m_ItemId]->clanType].stype_name[g_SkillProperty[m_ItemId]->skillSType], ferastring );
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[3], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;

        //Author : 양희왕 //breif : 무공 전수 창에서 툴팁이 안나옴
        int skilllevel = 0;
        if( pSkill )
            skilllevel = pSkill->m_cSkillLevel;
		
		if( _XSkillItem::GetCostForce(m_ItemId, skilllevel) )
		{
			sprintf( m_CurrentTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_FORCE), _XSkillItem::GetCostForce(m_ItemId, skilllevel)/*g_SkillProperty[m_ItemId]->detailInfo[skilllevel].costForce*/ );
			m_TooltipSize.cy += DT_ROW_INTERVAL;
		}
		
		if( _XSkillItem::GetCasting(m_ItemId, skilllevel) > 0 )
		{
			float realcastingtime = _XSkillItem::GetCasting(m_ItemId, skilllevel) % 60000;
			if( realcastingtime == 0 )		// 분 (으)로 표시
			{
				realcastingtime = (float)_XSkillItem::GetCasting(m_ItemId, skilllevel) / 60000.0f;
				sprintf( m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CASTINGTIMEMIN), (int)realcastingtime );
			}
			else						// 초 (으)로 표시
			{
				realcastingtime = (float)_XSkillItem::GetCasting(m_ItemId, skilllevel) / 1000.0f;
				sprintf( m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CASTINGTIMESEC), skilllevel );
			}
		}	
		else
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CASTINGTIMEDIRECT) );
		}


		if( strlen(m_CurrentTooltipInfo.Data_Buf[5]) )
		{
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[5], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}		
		
		if( _XSkillItem::GetCoolDown(m_ItemId, skilllevel) > 0 )
		{
			float realcooldown = _XSkillItem::GetCoolDown(m_ItemId, skilllevel) % 60000;
			if( realcooldown == 0 )
			{
				realcooldown = (float)_XSkillItem::GetCoolDown(m_ItemId, skilllevel) / 60000.0f;
				sprintf( m_CurrentTooltipInfo.Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_COOLDOWNMIN), (int)realcooldown );
			}
			else
			{
				realcooldown = (float)_XSkillItem::GetCoolDown(m_ItemId, skilllevel) / 1000.0f;
				sprintf( m_CurrentTooltipInfo.Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_COOLDOWNSEC), realcooldown );				
			}
		}
		if( strlen(m_CurrentTooltipInfo.Data_Buf[6]) )
		{
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[6], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		
		if( _XSkillItem::GetDistance(m_ItemId) > 0.0f )
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;
			sprintf( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_DISTANCE), _XSkillItem::GetDistance(m_ItemId) );
		}

		if( g_SkillProperty[m_ItemId]->detailInfo[skilllevel].requirementItemID > 0 )
		{
			int reqtype = g_SkillProperty[m_ItemId]->detailInfo[skilllevel].reqItemCategory;
			int reqitemid = g_SkillProperty[m_ItemId]->detailInfo[skilllevel].requirementItemID;			


			LPTSTR param1 = _XGameItem::GetItemName(reqtype, reqitemid);
			int    param2 = g_SkillProperty[m_ItemId]->detailInfo[skilllevel].itemConsumed;
			
			strcpy( m_CurrentTooltipInfo.Data_Buf[8], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_CONSUMED, param1, &param2 ) );
			
			int _length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[8], m_TooltipSize.cx-16 );
			
			m_TooltipSize.cy += DT_ROW_INTERVAL*_length;
		}		
		
		if( g_SkillProperty[m_ItemId]->requirementCondition )
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SkillProperty[m_ItemId]->requirementCondition);
			if(iter_table != g_CharacterStateTable.end())
			{
				int tmp = g_SkillProperty[m_ItemId]->requirementCondition;
				tmp--;
				if( tmp > 0 )
				{
					sprintf( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQCONDITION), g_CharacterConditionTable[tmp].name );
					int _length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[9], m_TooltipSize.cx-16 );
					m_TooltipSize.cy += DT_ROW_INTERVAL*_length;
				}
			}
		}
		
		if( m_CurrentTooltipMode == TMODE_TRADESKILL )
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;
			m_TooltipSize.cy += DT_ROW_INTERVAL;
			int level = g_UserLevelTable[g_SkillProperty[m_ItemId]->requirementLevel].level;
			int sublevel = g_UserLevelTable[g_SkillProperty[m_ItemId]->requirementLevel].sublevel;
			
			strcpy( m_CurrentTooltipInfo.E_Data_Buf[2], 
					g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[level], &sublevel ) );
			
			if( m_CurrentSlotNumber )
			{
				sprintf(m_CurrentTooltipInfo.E_Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQMONEY), g_SkillProperty[m_ItemId]->requirementContribution );
			}
			else
			{
				sprintf(m_CurrentTooltipInfo.E_Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQCONTRIBUTION), g_SkillProperty[m_ItemId]->requirementContribution);
			}
			
			if(g_SkillProperty[m_ItemId]->classGrade == 2)
			{
				sprintf(m_CurrentTooltipInfo.E_Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_NEW_3797));//_T("2차 전직용")
				m_TooltipSize.cy += DT_ROW_INTERVAL;
			}
		}
		else
		{
            //Author : 양희왕 //breif : 무공전수 창에서 툴팁이 안나옴
            int SkillState = 0;
            if( pSkill )
                SkillState = pSkill->m_ucSkillState;
			
			if( SkillState > 0)
			{
				switch(g_SkillProperty[m_ItemId]->usefulType) 
				{
				case _XSI_USEFULTYPE_PASSIVESKILL:
					break;
				case _XSI_USEFULTYPE_ACTIVESKILL:
					{
						m_TooltipSize.cy += DT_ROW_INTERVAL;
						strcpy( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TRAININGACT) );
					}
					break;
				case _XSI_USEFULTYPE_CHARGESKILL:
					break;
				case _XSI_USEFULTYPE_PASSIVESPELL:
					break;
				case _XSI_USEFULTYPE_CASTINGSPELL:
					{
						m_TooltipSize.cy += DT_ROW_INTERVAL;
						strcpy( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TRAININGFOR) );
					}
					break;
				case _XSI_USEFULTYPE_CHANNELINGSPELL:
					{
						if( g_SkillProperty[m_ItemId]->skillSType != _XSI_STYPE_03)
						{
							m_TooltipSize.cy += DT_ROW_INTERVAL;
							strcpy( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TRAININGFOR) );
						}
					}
					break;
				case _XSI_USEFULTYPE_FINISHSKILL:
					break;
				case _XSI_USEFULTYPE_ROUNDINGSPELL:
					{
						m_TooltipSize.cy += DT_ROW_INTERVAL;
						strcpy( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TRAININGFOR) );
					}
					break;
				case _XSI_USEFULTYPE_HIDDENMA:
					break;
				case _XSI_USEFULTYPE_SMASHINGSKILL:
					break;
				}
			}
		}

		if( g_SkillProperty[m_ItemId]->requirementSkill01 )
		{
			int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[m_ItemId]->requirementSkill01);
			if( tempskilllevel > -1 )
			{
				if( skilllevel < 7 )
				{
					sprintf( m_CurrentTooltipInfo.E_Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQNEEDSKILL), g_SkillProperty[g_SkillProperty[m_ItemId]->requirementSkill01]->skillName );
				}							
			}
			else
			{
				sprintf( m_CurrentTooltipInfo.E_Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQNEEDSKILL), g_SkillProperty[g_SkillProperty[m_ItemId]->requirementSkill01]->skillName );
			}

			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[0], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;

			if( g_SkillProperty[m_ItemId]->requirementSkill02 )
			{
				int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[m_ItemId]->requirementSkill02);
				if( tempskilllevel > -1 )
				{
					int curskillindex = g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel;
					
					if( curskillindex < 7 )
					{
						sprintf( m_CurrentTooltipInfo.E_Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQNEEDSKILL), g_SkillProperty[g_SkillProperty[m_ItemId]->requirementSkill02]->skillName );						
					}								
				}
				else
				{
					sprintf( m_CurrentTooltipInfo.E_Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQNEEDSKILL), g_SkillProperty[g_SkillProperty[m_ItemId]->requirementSkill02]->skillName );
				}
				int length2 = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length2;
			}
		}

		// 성취 단계 제한 - 2006.08.02 정식서버로 
		if( skilllevel < 11 )
		{
			if( g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].requirementMission )
			{
				m_TooltipSize.cy += DT_ROW_INTERVAL;
				strcpy( m_CurrentTooltipInfo.Data_Buf[10], _XGETINTERFACETEXT(ID_STRING_NEW_2982) );	//성취 단계 상승 조건 - 무공서 필요
			}
			if( g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].objectID > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
			{
				m_TooltipSize.cy += DT_ROW_INTERVAL;
				int tempstep = GetInnerLevel(g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].objectID);				// 성 ( 1 - 12 )
				int templevel = ( g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].objectID + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )	
				strcpy( m_CurrentTooltipInfo.Data_Buf[11], 
					g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2983, g_LevelName[templevel], &tempstep ) ); //성취 단계 상승 조건 - %s %d성
			}
		}
		
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
        //Author : 양희왕 //breif : 무공 전수 창에 툴팁이 안나옴
		if(pSkill && pSkill->m_cCurMission == 1)	//event 무공이면 끝나는 날짜 명시
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;

			int param1 = 0, param2 = 0, param3 = 0, param4 = 0, param5 = 0;

			param1 = g_pLocalUser->m_EventSkillEndTime.dwYear + 2000;
			param2 = g_pLocalUser->m_EventSkillEndTime.dwMonth;
			param3 = g_pLocalUser->m_EventSkillEndTime.dwDay;
			param4 = g_pLocalUser->m_EventSkillEndTime.dwHour;
			param5 = g_pLocalUser->m_EventSkillEndTime.dwMinute;

			sprintf(m_CurrentTooltipInfo.Data_Buf[12], 
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
					&param1, &param2, &param3, &param4,	&param5 ));
		}
#endif

#ifdef _XDEF_NEWSTRINGMANAGER		
		if( g_SkillProperty[m_ItemId]->information )
#endif
		{
			if( strlen( g_SkillProperty[m_ItemId]->information ) )
			{
				m_TooltipSize.cy += DT_LINE_INTERVAL*2;

				int	tempParameter[10];
				for( int i = 0; i < 10; i++ )
				{
					tempParameter[i] = _XSkillItem::GetParameterValue( m_ItemId, skilllevel, g_SkillProperty[m_ItemId]->parameter[i] );
				}
				
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 10개까지 사용하려면 _XStringDataBase의 Method를 수정할 것
				int param1 = tempParameter[0]; int param2 = tempParameter[1];
				int param3 = tempParameter[2]; int param4 = tempParameter[3];
				int param5 = tempParameter[4]; int param6 = tempParameter[5];
				int param7 = tempParameter[6]; int param8 = tempParameter[7];
				
				strcpy( m_CurrentTooltipInfo.LData_Buf, g_StringDataBase.CompositeString( g_SkillProperty[m_ItemId]->information, g_SkillProperty[m_ItemId]->wordorder,
																						  &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );

				/*
				sprintf( m_CurrentTooltipInfo.LData_Buf, g_SkillProperty[m_ItemId]->information, 
					tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
					tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9] );
				*/
				
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.LData_Buf, m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}		
		}
#ifdef _XTS_DELETESKILL
//#ifdef _XTS_NEWSKILLWINDOW
		if( m_CurrentTooltipMode == TMODE_SKILL )	// 스킬 삭제 버튼 추가
		{
			m_TooltipSize.cy += 30;			
		}
#endif
	}
	else		// MasterySkill Tooltip
	{
		if( g_pMasterySkill_Window )
		{
#ifdef _XDEF_NEWSTRINGMANAGER		
			if( g_SkillMasteryProperty[m_ItemId]->cName )
#endif
			{
				if( strlen( g_SkillMasteryProperty[m_ItemId]->cName ) )		// 이름
				{
					m_TooltipSize.cy += DT_ROW_INTERVAL;
					strcpy( m_CurrentTooltipInfo.NameBuf, g_SkillMasteryProperty[m_ItemId]->cName );
				}
			}

			int index = g_pLocalUser->GetLearnedMasterySkillIndex(m_ItemId);

			if( index != -1 )
			{
				if( g_pLocalUser->m_MasterySkillList[index].cPoint <= g_SkillMasteryProperty[m_ItemId]->cMaxMP )
				{
					if( g_SkillMasteryProperty[m_ItemId]->cMaxMP )
					{
						m_TooltipSize.cy += DT_ROW_INTERVAL*2;
						sprintf( m_CurrentTooltipInfo.Data_Buf[0], "(%d/%d)%s", g_pLocalUser->m_MasterySkillList[index].cPoint, 
																				g_SkillMasteryProperty[m_ItemId]->cMaxMP, 
																				_XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG) );
					}
					
					m_TooltipSize.cy += DT_LINE_INTERVAL*4;

					if( strlen(g_SkillMasteryProperty[m_ItemId]->information) )
					{
						int tempPoint = g_pLocalUser->m_MasterySkillList[index].cPoint-1;
						if( tempPoint >= 0 )
						{
							if(g_SkillMasteryProperty[m_ItemId]->detailInfo[tempPoint].effect)
							{
								short tempParameter[14];				
								for( int i = 0; i < 14; i++ )
								{
									tempParameter[i] = _XSkillItem::GetMasteryParameterValue( m_ItemId, tempPoint, g_SkillMasteryProperty[m_ItemId]->parameter[i] );
								}

								//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것

								int param1 = tempParameter[0]; int param2 = tempParameter[1];
								int param3 = tempParameter[2]; int param4 = tempParameter[3];
								int param5 = tempParameter[4]; int param6 = tempParameter[5];
								int param7 = tempParameter[6]; int param8 = tempParameter[7];
								
								strcpy( m_CurrentTooltipInfo.LData_Buf, g_StringDataBase.CompositeString( g_SkillMasteryProperty[m_ItemId]->information, g_SkillMasteryProperty[m_ItemId]->wordorder,
																										  &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );


								/*
								sprintf( m_CurrentTooltipInfo.LData_Buf, g_SkillMasteryProperty[m_ItemId]->information, 
										tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
										tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9],
										tempParameter[10], tempParameter[11], tempParameter[12], tempParameter[13] );
								*/
							}
							else
							{
								if(tempPoint > 0)
								{
									if(g_SkillMasteryProperty[m_ItemId]->detailInfo[tempPoint-1].effect)
									{
										short tempParameter[14];				
										for( int i = 0; i < 14; i++ )
										{
											tempParameter[i] = _XSkillItem::GetMasteryParameterValue( m_ItemId, tempPoint, g_SkillMasteryProperty[m_ItemId]->parameter[i] );
										}
										
										//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것								
										int param1 = tempParameter[0]; int param2 = tempParameter[1];
										int param3 = tempParameter[2]; int param4 = tempParameter[3];
										int param5 = tempParameter[4]; int param6 = tempParameter[5];
										int param7 = tempParameter[6]; int param8 = tempParameter[7];
										
										strcpy( m_CurrentTooltipInfo.LData_Buf, g_StringDataBase.CompositeString( g_SkillMasteryProperty[m_ItemId]->information, g_SkillMasteryProperty[m_ItemId]->wordorder,
																												  &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );

										/*
										sprintf( m_CurrentTooltipInfo.LData_Buf, g_SkillMasteryProperty[m_ItemId]->information, 
											tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
											tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9],
											tempParameter[10], tempParameter[11], tempParameter[12], tempParameter[13] );
										*/
									}
									else
									{
										sprintf(m_CurrentTooltipInfo.LData_Buf, g_SkillMasteryProperty[m_ItemId]->information);
									}
								}
								else
								{
									sprintf(m_CurrentTooltipInfo.LData_Buf, g_SkillMasteryProperty[m_ItemId]->information);
									//sprintf( m_CurrentTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2018));
								}
							}
							// ================================
							
							int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.LData_Buf, m_TooltipSize.cx-16 );
							m_TooltipSize.cy += DT_ROW_INTERVAL*length;
						}
					}
					// 다음단계 ------------------------------------------------------------------------------------------------------=
					if( g_pLocalUser->m_MasterySkillList[index].cPoint < g_SkillMasteryProperty[m_ItemId]->cMaxMP )
					{
						// 필요 오의
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
							g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
							m_TooltipSize.cy += DT_ROW_INTERVAL*2;
						else
							m_TooltipSize.cy += DT_ROW_INTERVAL;

						m_TooltipSize.cy += DT_LINE_INTERVAL*2;
						sprintf( m_CurrentTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2017), g_pLocalUser->m_MasterySkillList[index].cPoint+1, g_SkillMasteryProperty[m_ItemId]->cMaxMP );
						sprintf( m_CurrentTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NEEDLEVEL), g_SkillMasteryProperty[m_ItemId]->detailInfo[g_pLocalUser->m_MasterySkillList[index].cPoint].sReqMp );

						if( strlen(g_SkillMasteryProperty[m_ItemId]->information) )
						{
							int tempPoint = g_pLocalUser->m_MasterySkillList[index].cPoint;
							if( tempPoint >= 0 )
							{
								if(g_SkillMasteryProperty[m_ItemId]->detailInfo[tempPoint].effect)
								{
									short tempParameter[14];				
									for( int i = 0; i < 14; i++ )
									{
										tempParameter[i] = _XSkillItem::GetMasteryParameterValue( m_ItemId, tempPoint, g_SkillMasteryProperty[m_ItemId]->parameter[i] );
									}

									//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것

									int param1 = tempParameter[0]; int param2 = tempParameter[1];
									int param3 = tempParameter[2]; int param4 = tempParameter[3];
									int param5 = tempParameter[4]; int param6 = tempParameter[5];
									int param7 = tempParameter[6]; int param8 = tempParameter[7];
									
									strcpy( m_CurrentTooltipInfo.LData_Buf2, 
											g_StringDataBase.CompositeString( g_SkillMasteryProperty[m_ItemId]->information, g_SkillMasteryProperty[m_ItemId]->wordorder,
																			  &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );
									
										/*
									sprintf( m_CurrentTooltipInfo.LData_Buf2, g_SkillMasteryProperty[m_ItemId]->information, 
										tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
										tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9],
										tempParameter[10], tempParameter[11], tempParameter[12], tempParameter[13] );
										*/
								}
								else
								{
									if(tempPoint > 0)
									{
										if(g_SkillMasteryProperty[m_ItemId]->detailInfo[tempPoint-1].effect)
										{
											short tempParameter[14];				
											for( int i = 0; i < 14; i++ )
											{
												tempParameter[i] = _XSkillItem::GetMasteryParameterValue( m_ItemId, tempPoint, g_SkillMasteryProperty[m_ItemId]->parameter[i] );
											}
											
											//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
											// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것
											int param1 = tempParameter[0]; int param2 = tempParameter[1];
											int param3 = tempParameter[2]; int param4 = tempParameter[3];
											int param5 = tempParameter[4]; int param6 = tempParameter[5];
											int param7 = tempParameter[6]; int param8 = tempParameter[7];
											
											strcpy( m_CurrentTooltipInfo.LData_Buf2, 
													g_StringDataBase.CompositeString( g_SkillMasteryProperty[m_ItemId]->information, g_SkillMasteryProperty[m_ItemId]->wordorder,
																					  &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );

											/*
											sprintf( m_CurrentTooltipInfo.LData_Buf2, g_SkillMasteryProperty[m_ItemId]->information, 
												tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
												tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9],
												tempParameter[10], tempParameter[11], tempParameter[12], tempParameter[13] );
											*/
										}
										else
										{
											sprintf( m_CurrentTooltipInfo.LData_Buf2, g_SkillMasteryProperty[m_ItemId]->information);
										}
									}
									else
									{
										sprintf( m_CurrentTooltipInfo.LData_Buf2, g_SkillMasteryProperty[m_ItemId]->information);
									}
								}
								int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.LData_Buf2, m_TooltipSize.cx-16 );
								m_TooltipSize.cy += DT_ROW_INTERVAL*length;
							}
						}
					}					
				}
				/*else
				{
					sprintf( m_CurrentTooltipInfo.Data_Buf[0], "(%d/%d)成", g_pLocalUser->m_MasterySkillList[index].cPoint, g_SkillMasteryProperty[m_ItemId]->cMaxMP );
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], "필요 오의 %d", g_SkillMasteryProperty[m_ItemId]->sReqMp[g_pLocalUser->m_MasterySkillList[index].cPoint] );
					int needid = g_SkillMasteryProperty[m_ItemId]->sNeedMasteryID;
					if( needid > 0 )
					{
						if( g_pLocalUser->m_MasterySkillList[index].cPoint < g_SkillMasteryProperty[needid]->cMaxMP )
						{
							m_TooltipSize.cy += DT_ROW_INTERVAL;	// 선행 스킬
							sprintf( m_CurrentTooltipInfo.Data_Buf[2], "%s %d성 필요", g_SkillMasteryProperty[needid]->cName, 
								g_SkillMasteryProperty[needid]->cMaxMP );
						}
					}
					
					if( strlen(g_SkillMasteryProperty[m_ItemId]->information) )
					{
						int tempPoint = g_pLocalUser->m_MasterySkillList[index].cPoint+m_CurrentSlotNumber-1;
						if( tempPoint >= 0 )
						{
							if(g_SkillMasteryProperty[m_ItemId]->effect[tempPoint])
							{
								short tempParameter[14];				
								for( int i = 0; i < 14; i++ )
								{
									tempParameter[i] = _XSkillItem::GetMasteryParameterValue( m_ItemId, tempPoint, g_SkillMasteryProperty[m_ItemId]->parameter[i] );
								}

							    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것
								strcpy( m_CurrentTooltipInfo.LData_Buf, 
										g_StringDataBase.CompositeString( g_SkillProperty[m_ItemId]->information, g_SkillProperty[m_ItemId]->wordorder,
																			&tempParameter[0], &tempParameter[1], &tempParameter[2], &tempParameter[3], 
																			&tempParameter[4], &tempParameter[5], &tempParameter[6], &tempParameter[7] ) );


								//sprintf( m_CurrentTooltipInfo.LData_Buf, g_SkillMasteryProperty[m_ItemId]->information, 
								//		tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
								//		tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9],
								//		tempParameter[10], tempParameter[11], tempParameter[12], tempParameter[13] );


								int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.LData_Buf, 36 );
								m_TooltipSize.cy += DT_ROW_INTERVAL*length;
							}
						}
					}
				}*/
			}
			else	// Not Learned MasterySkill
			{
				if( g_SkillMasteryProperty[m_ItemId]->cMaxMP )
				{
					m_TooltipSize.cy += DT_ROW_INTERVAL*2;
					sprintf( m_CurrentTooltipInfo.Data_Buf[0], "(0/%d)%s", g_SkillMasteryProperty[m_ItemId]->cMaxMP,_XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG) );
				}

				m_TooltipSize.cy += DT_LINE_INTERVAL*4;
				
				int needid = g_SkillMasteryProperty[m_ItemId]->sNeedMasteryID;
				if( needid > 0 )
				{
					BOOL bMsg = TRUE;
					int param2 = g_SkillMasteryProperty[needid]->cMaxMP;

					int index = g_pLocalUser->GetLearnedMasterySkillIndex(needid);
					if(index > -1)
					{
						// 선행 오의 배웠음 && 선행 오의 point 충족
						if(g_pLocalUser->m_MasterySkillList[index].cPoint == param2)
						{
							bMsg = FALSE;
						}
					}

					if(bMsg)
					{
						m_TooltipSize.cy += DT_ROW_INTERVAL;	// 선행 스킬

						int param2 = g_SkillMasteryProperty[needid]->cMaxMP;
						strcpy( m_CurrentTooltipInfo.Data_Buf[1], 
								g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_NEEDMP, g_SkillMasteryProperty[needid]->cName, &param2 ) );
					}
				}
				
				if(g_SkillMasteryProperty[m_ItemId]->sNeedPageTotalPoint > 0)
				{
					if(g_pMasterySkill_Window->m_TotalPoint < g_SkillMasteryProperty[m_ItemId]->sNeedPageTotalPoint)
					{
						m_TooltipSize.cy += DT_ROW_INTERVAL;
						
#ifdef _XDEF_MASTERY_TOTALPOINT_080313_KUKURI

						int	param1 = g_SkillMasteryProperty[m_ItemId]->sNeedPageTotalPoint;
						strcpy(m_CurrentTooltipInfo.Data_Buf[2], 
							g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2020, 
							_XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALL), &param1));	//전체 오의합계 %d 요구 

#else

						if(g_pMasterySkill_Window->m_SelectedTab == 0)
						{
							sprintf(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2019), g_SkillMasteryProperty[m_ItemId]->sNeedPageTotalPoint);
						}
						else
						{
	#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI
							int		param1 = g_pMasterySkill_Window->GetMasterySkillNameId();
							int		param2 = g_SkillMasteryProperty[m_ItemId]->sNeedPageTotalPoint;

							strcpy( m_CurrentTooltipInfo.Data_Buf[2], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2020, 
									g_StringDataBase.GetTextString(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, param1), &param2 ) );

	#else
							LPTSTR  param1 = g_pMasterySkill_Window->GetMasterySkillTypeName(g_pLocalUser->m_CharacterInfo.Get_cClass(), g_pMasterySkill_Window->m_SelectedTab-1);
							int		param2 = g_SkillMasteryProperty[m_ItemId]->sNeedPageTotalPoint;

							strcpy( m_CurrentTooltipInfo.Data_Buf[2], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2020, 
																	 param1, &param2 ) );
	#endif
						}
#endif
					}
				}

				if(g_SkillMasteryProperty[m_ItemId]->information)
				{
					if(g_SkillMasteryProperty[m_ItemId]->detailInfo[0].effect)
					{
						short tempParameter[14];				
						for( int i = 0; i < 14; i++ )
						{
							tempParameter[i] = _XSkillItem::GetMasteryParameterValue( m_ItemId, 0, g_SkillMasteryProperty[m_ItemId]->parameter[i] );
						}

						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것

						int param1 = tempParameter[0]; int param2 = tempParameter[1];
						int param3 = tempParameter[2]; int param4 = tempParameter[3];
						int param5 = tempParameter[4]; int param6 = tempParameter[5];
						int param7 = tempParameter[6]; int param8 = tempParameter[7];
						
						strcpy( m_CurrentTooltipInfo.LData_Buf, 
								g_StringDataBase.CompositeString( g_SkillMasteryProperty[m_ItemId]->information, g_SkillMasteryProperty[m_ItemId]->wordorder,
																	&param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );
						
/*
						sprintf( m_CurrentTooltipInfo.LData_Buf, g_SkillMasteryProperty[m_ItemId]->information, 
							tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
							tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9],
							tempParameter[10], tempParameter[11], tempParameter[12], tempParameter[13] );
*/
						
						int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.LData_Buf, m_TooltipSize.cx-16 );
						m_TooltipSize.cy += DT_ROW_INTERVAL*length;						
					}
					else
					{
						sprintf( m_CurrentTooltipInfo.LData_Buf, g_SkillMasteryProperty[m_ItemId]->information);

						int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.LData_Buf, m_TooltipSize.cx-16 );
						m_TooltipSize.cy += DT_ROW_INTERVAL*length;
					}
				}
				if( g_SkillMasteryProperty[m_ItemId]->detailInfo[0].sReqMp )
				{
					m_TooltipSize.cy += DT_ROW_INTERVAL;
					m_TooltipSize.cy += DT_LINE_INTERVAL*2;
					sprintf( m_CurrentTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NEEDLEVEL), g_SkillMasteryProperty[m_ItemId]->detailInfo[0].sReqMp );
				}
			}
		}		
	}
	
	return TRUE;
}

void _XWindow_DefaultTooltip::ResetTooltipInfo()
{
	memset( &m_CurrentTooltipInfo, 0, sizeof(m_CurrentTooltipInfo)); //Author : 양희왕 //breif : 변경
	/*memset( m_CurrentTooltipInfo.NickBuf, 0, sizeof(TCHAR)*256);
	memset( m_CurrentTooltipInfo.NameBuf, 0, sizeof(TCHAR)*256);
	memset( m_CurrentTooltipInfo.Temp_Data_Buf, 0, sizeof(TCHAR)*256);
	memset( m_CurrentTooltipInfo.DescBuf, 0, sizeof(TCHAR)*256);	
	memset( m_CurrentTooltipInfo.LData_Buf, 0, sizeof(TCHAR)*512);
	memset( m_CurrentTooltipInfo.LData_Buf2, 0, sizeof(TCHAR)*512);

	memset( m_CurrentTooltipInfo.Dur_Data_Buf, 0, sizeof(TCHAR)*256);
	
	for( int i = 0; i < 16; i++ )
		memset( m_CurrentTooltipInfo.Data_Buf[i], 0, sizeof(TCHAR)*256);

	for( i = 0; i < 5; i++ )
	{
		memset( m_CurrentTooltipInfo.S_Data_Buf[i], 0, sizeof(TCHAR)*256);
	}
	for( i = 0; i < 10; i++ )
	{
		memset( m_CurrentTooltipInfo.E_Data_Buf[i], 0, sizeof(TCHAR)*256);		
	}

	m_CurrentTooltipInfo.Data_Value1 = 0;
	m_CurrentTooltipInfo.Data_Value2 = 0;
	m_CurrentTooltipInfo.Data_Value3 = 0;
	m_CurrentTooltipInfo.Data_Value4 = 0;*/

	m_CurrentItemSocketCount = -1;
}

int	_XWindow_DefaultTooltip::GetInnerLevel(int innerlevel)
{
	int templevel = innerlevel;
	if( templevel != 0 )
	{
		templevel = templevel % 12;
		templevel = (templevel != 0) ? templevel : 12;
	}
	return templevel;
}

void _XWindow_DefaultTooltip::SetInfoWeaponItem()
{
	if(m_ItemId <= 0)
		return;

	_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
	if( tempWeaponItem )
	{
		if( tempWeaponItem[m_ItemId].cNick )	// 별명
		{
			strcpy( m_CurrentTooltipInfo.NickBuf, tempWeaponItem[m_ItemId].cNick );					
		}
		
		if( tempWeaponItem[m_ItemId].cName )	// 이름
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
#ifdef _XTS_ITEM_V20
			if( _XGameItem::GetItemNameAddtoString(m_ItemType, m_ItemId) )
			{
				sprintf( m_CurrentTooltipInfo.NameBuf, "%s %s", _XGameItem::GetItemNameAddtoString(m_ItemType, m_ItemId), tempWeaponItem[m_ItemId].cName );
			}
			else
			{
				strcpy( m_CurrentTooltipInfo.NameBuf, tempWeaponItem[m_ItemId].cName );
			}		
#else
			strcpy( m_CurrentTooltipInfo.NameBuf, tempWeaponItem[m_ItemId].cName );
#endif			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		if( tempWeaponItem[m_ItemId].cItemInfo )	// 설명
		{
			strcpy( m_CurrentTooltipInfo.DescBuf, tempWeaponItem[m_ItemId].cItemInfo );
			
			m_TooltipSize.cy += DT_LINE_INTERVAL*2;
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		
		m_TooltipSize.cy += DT_ROW_INTERVAL;				
		switch(tempWeaponItem[m_ItemId].cSecondType)		// 분류
		{			
		case _XGI_SC_WEAPON_SHORT :
			{
				switch(tempWeaponItem[m_ItemId].cThirdType)
				{
				case _XGI_TC_WEAPON_SWORD		 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SWORD));	break;
				case _XGI_TC_WEAPON_KNIFE		 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_KNIFE));	break;			
				case _XGI_TC_WEAPON_CLUB		 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_CLUB));	break;		
				case _XGI_TC_WEAPON_HIT			 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_HIT));	break;
				case _XGI_TC_WEAPON_FIST		 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_FIST));	break;
				case _XGI_TC_WEAPON_AXE			 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_AXE));	break;
				case _XGI_TC_WEAPON_SECRET		 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECRETWEAPON)); break;
				case _XGI_TC_WEAPON_RING		 : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_RING)); break;
				}
			}				
			break;
		case _XGI_SC_WEAPON_LONG :
			{
				switch(tempWeaponItem[m_ItemId].cThirdType)
				{
				case _XGI_TC_WEAPON_GLAIVE		: strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_GLAIVE));  break;
				case _XGI_TC_WEAPON_SPEAR		: strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SPEAR));	break;
				case _XGI_TC_WEAPON_STAFF		: strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_STAFF));	break;
				}
			}
			break;
		case _XGI_SC_WEAPON_SPECIAL :
			{
				switch(tempWeaponItem[m_ItemId].cThirdType) 
				{
				case _XGI_TC_WEAPON_SERING : strcpy(m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2021));  break;
				}
			}
			break;
		}
		int tempstrength = GetInchantInfo();
		if( tempstrength > 0 )
			sprintf( m_CurrentTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2022), tempstrength );
		
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		if( tempWeaponItem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
		{
			if( tempstrength > 0 )
			{
				int param1 = tempWeaponItem[m_ItemId].sMinPower;
				int param2 = tempWeaponItem[m_ItemId].sMaxPower; 
				int param3 = _XGameItem::GetInchantPlusPoint( tempWeaponItem[m_ItemId].cSecondType, tempWeaponItem[m_ItemId].sMaxPower, tempstrength );

				strcpy( m_CurrentTooltipInfo.Data_Buf[1], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2023, &param1, &param2, &param3 ) );
			}
			else
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2387), tempWeaponItem[m_ItemId].sMinPower, tempWeaponItem[m_ItemId].sMaxPower );			
			}
		}	
		else
		{
			if( tempstrength > 0 )
			{
				int param1 = tempWeaponItem[m_ItemId].sMinPower;
				int param2 = tempWeaponItem[m_ItemId].sMaxPower; 
				int param3 = _XGameItem::GetInchantPlusPoint( tempWeaponItem[m_ItemId].cSecondType, tempWeaponItem[m_ItemId].sMaxPower, tempstrength );
				
				strcpy( m_CurrentTooltipInfo.Data_Buf[1], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2024, 
																							&param1, &param2, &param3 ) );
			}
			else
			{
				int param1 = tempWeaponItem[m_ItemId].sMinPower;
				int param2 = tempWeaponItem[m_ItemId].sMaxPower;

				strcpy( m_CurrentTooltipInfo.Data_Buf[1], 
					g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_POWER, &param1, &param2 ) );
			}
			
			sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_BALANCE), tempWeaponItem[m_ItemId].cBalance ); // 균형 %d
			m_TooltipSize.cy += DT_ROW_INTERVAL;
			sprintf( m_CurrentTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2388), tempWeaponItem[m_ItemId].cAttackRate );	
		}
		
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		strcpy( m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DURABILITY) );
		sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", (tempWeaponItem[m_ItemId].usDurability+99)/100, (tempWeaponItem[m_ItemId].usDurability+99)/100 );

#ifdef _XDEF_SMELT_ITEM	//Author : 양희왕
		if( _XGameItem::GetNature( tempWeaponItem[m_ItemId].cType, tempWeaponItem[m_ItemId].sID) > 0 ) //Author : 양희왕 breif : 강도가 0 이면 출력하지 않는다. //Date : 08/03/04
		{
			if( !(tempWeaponItem[m_ItemId].cType == 0 && ( tempWeaponItem[m_ItemId].sID == 3088 || tempWeaponItem[m_ItemId].sID == 3089 )) ) //제련할 무기가 아닐때
			{
				m_TooltipSize.cy += DT_ROW_INTERVAL;
				strcpy( m_CurrentTooltipInfo.Data_Buf[11], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_STRENGTH) );
				sprintf( m_CurrentTooltipInfo.Strength_Data_Buf, "( %d / %d )", GetSmeltItem_ScaleInfo(), GetSmeltItem_MaxScaleInfo() );

				m_TooltipSize.cy += DT_ROW_INTERVAL;
				if( GetSmeltItem_TearInfo() == 0 )
					strcpy( m_CurrentTooltipInfo.Data_Buf[12], _XGETINTERFACETEXT(ID_STRING_NEW_3451) ); // 강도증가 가능
				else
					strcpy( m_CurrentTooltipInfo.Data_Buf[12], _XGETINTERFACETEXT(ID_STRING_NEW_3452) ); // 강도증가 불가
			}
		}
#endif //Last Updated : 07/04/25

//Author : 양희왕 //breif : 수집 가능 메시지 추가
#ifdef _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER
		int nMixWeaponRes = _XGameItem::GetTempField5( m_ItemType, m_ItemId ); // 메시지 처리 방법을 얻어온다

		switch( nMixWeaponRes )
		{
		case 0 :
			{
				strcpy( m_CurrentTooltipInfo.Data_Buf[14], _XGETINTERFACETEXT(ID_STRING_NEW_3813));//수집 불가
				strcpy( m_CurrentTooltipInfo.Data_Buf[15], _XGETINTERFACETEXT(ID_STRING_NEW_3783) ); // 조합 불가
			}
			break;
		case 1 :
			{
				strcpy( m_CurrentTooltipInfo.Data_Buf[14], _XGETINTERFACETEXT(ID_STRING_NEW_3781) ); // 수집 가능
				strcpy( m_CurrentTooltipInfo.Data_Buf[15], _XGETINTERFACETEXT(ID_STRING_NEW_3782) ); // 조합 가능
			}
			break;
		case 2 :
			{
				strcpy( m_CurrentTooltipInfo.Data_Buf[14], _XGETINTERFACETEXT(ID_STRING_NEW_3813));//수집 불가
				strcpy( m_CurrentTooltipInfo.Data_Buf[15], _XGETINTERFACETEXT(ID_STRING_NEW_3783) ); // 조합 불가
			}
			break;
		case 3 :
			{
				strcpy( m_CurrentTooltipInfo.Data_Buf[14], _XGETINTERFACETEXT(ID_STRING_NEW_3813));//수집 불가
				strcpy( m_CurrentTooltipInfo.Data_Buf[15], _XGETINTERFACETEXT(ID_STRING_NEW_3782) ); // 조합 가능
			}
			break;
		case 4 :
			{
				strcpy( m_CurrentTooltipInfo.Data_Buf[14], _XGETINTERFACETEXT(ID_STRING_NEW_3781) ); // 수집 가능
				strcpy( m_CurrentTooltipInfo.Data_Buf[15], _XGETINTERFACETEXT(ID_STRING_NEW_3783) ); // 조합 불가
			}
			break;
		}
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		m_TooltipSize.cy += DT_ROW_INTERVAL;

		//Author : 양희왕 //breif : 제련된거 없앤다 Date : 08/07/31 패치
	#ifdef _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731
	#else
		if( tempstrength > 0 )
		{
			memset( m_CurrentTooltipInfo.Data_Buf[15], 0, sizeof(m_CurrentTooltipInfo.Data_Buf[15]));
			strcpy( m_CurrentTooltipInfo.Data_Buf[15], _XGETINTERFACETEXT(ID_STRING_NEW_3783) ); // 조합 불가
		}
	#endif
#endif
		
		int tempstep = GetInnerLevel(tempWeaponItem[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( tempWeaponItem[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		if( tempWeaponItem[m_ItemId].sReqLevel )
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[6], 
				g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[templevel], &tempstep ) );			

			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[6], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		if( tempWeaponItem[m_ItemId].sReqJung )	// 근력
		{
			if( tempWeaponItem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			{
#ifdef _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI
				sprintf( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQINT), tempWeaponItem[m_ItemId].sReqJung );
#else
				sprintf( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQFFORCE), tempWeaponItem[m_ItemId].sReqJung );
#endif
			}
			else
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQJUNG), tempWeaponItem[m_ItemId].sReqJung );
			}
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[7], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		if( tempWeaponItem[m_ItemId].sReqSim )	// 민첩
		{
			if( tempWeaponItem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			{
#ifdef _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI
				sprintf( m_CurrentTooltipInfo.Data_Buf[8], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQSIM), tempWeaponItem[m_ItemId].sReqSim );
#else
				sprintf( m_CurrentTooltipInfo.Data_Buf[8], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQINT), tempWeaponItem[m_ItemId].sReqSim );
#endif
			}
			else
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[8], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQSIM), tempWeaponItem[m_ItemId].sReqSim );
			}
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[8], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		
		// 패널티 표현
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		if( selflevel < itemlevel )
		{
			strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_ATTACKDAMAGE) );
		}
		if( tempWeaponItem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
		{
#ifdef _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI
			if( g_pLocalUser->m_CharacterInfo.Get_intelligence() < tempWeaponItem[m_ItemId].sReqJung )
#else
			if( g_pLocalUser->m_CharacterInfo.Get_zen() < tempWeaponItem[m_ItemId].sReqJung )
#endif
			{
				if( strlen(m_CurrentTooltipInfo.Data_Buf[9]) )
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
				else
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACCURATE) );
			}
			
#ifdef _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI
			if( g_pLocalUser->m_CharacterInfo.Get_dexterity() < tempWeaponItem[m_ItemId].sReqSim )
#else
			if( g_pLocalUser->m_CharacterInfo.Get_intelligence() < tempWeaponItem[m_ItemId].sReqSim )
#endif
			{
				if( strlen(m_CurrentTooltipInfo.Data_Buf[9]) )
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
				else
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CRITICALRATE) );
			}
		}
		else
		{
			if( g_pLocalUser->m_CharacterInfo.Get_strength() < tempWeaponItem[m_ItemId].sReqJung )
			{
				if( strlen(m_CurrentTooltipInfo.Data_Buf[9]) )
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
				else
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACCURATE) );
			}
			
			if( g_pLocalUser->m_CharacterInfo.Get_dexterity() < tempWeaponItem[m_ItemId].sReqSim )
			{
				if( strlen(m_CurrentTooltipInfo.Data_Buf[9]) )
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
				else
					strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CRITICALRATE) );
			}
		}	
		if( strlen(m_CurrentTooltipInfo.Data_Buf[9]) )
		{
			strcat( m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DECREASE) );
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[9], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		
#ifdef _XTS_FAME
		if( tempWeaponItem[m_ItemId].ucFame > 0 )	// 명성 or 악명	
		{
			if( tempWeaponItem[m_ItemId].ucFame > 100 )	// 악명
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[10], _XGETINTERFACETEXT(ID_STRING_NEW_2638), g_FameStr[tempWeaponItem[m_ItemId].ucFame-101].m_InfamyName );
			}
			else	// 명성
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[10], _XGETINTERFACETEXT(ID_STRING_NEW_2639), g_FameStr[tempWeaponItem[m_ItemId].ucFame-1].m_FameName );
			}
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[10], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
#endif
		// 스페셜 어빌리티	
		for( int i = 0; i < 5; i++ )
		{
			memset( m_CurrentTooltipInfo.Temp_Data_Buf, 0, sizeof(TCHAR)*128);
			
			if( tempWeaponItem[m_ItemId].ucAdd_Prob[i] )
			{
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), tempWeaponItem[m_ItemId].ucAdd_Prob[i] );
				strcat( m_CurrentTooltipInfo.S_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
			}
			
			if( tempWeaponItem[m_ItemId].usAdd_SEID[i] )
			{
				LPTSTR pStr = NULL;
				pStr = g_CharacterStateTable[tempWeaponItem[m_ItemId].usAdd_SEID[i]]->concept;
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[tempWeaponItem[m_ItemId].usAdd_SEID[i]]->concept, 
					tempWeaponItem[m_ItemId].usAdd_Value[i] );
			}
			
			if( strlen(m_CurrentTooltipInfo.Temp_Data_Buf) )
			{
				strcat( m_CurrentTooltipInfo.S_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
				
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.S_Data_Buf[i], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}		
		}

#ifdef _XDEF_PERIODITEM_20061027
		// 무기 기간제 아이템
		if(tempWeaponItem[m_ItemId].cCashCheck == 2 || tempWeaponItem[m_ItemId].cCashCheck == 3)
		{
			// 기간제 아이템이면 남은 시간 표시
			m_TooltipSize.cy += DT_ROW_INTERVAL;

			int    param1, param2, param3, param4, param5;

			switch( m_CurrentTooltipMode )
			{
			case TMODE_TARGETINFO :
				{
					param1 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) );
				}
				break;
			case TMODE_WAREHOUSE_WAREHOUSEITEM :
				{
					param1 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) );
				}
				break;
			case TMODE_PCTRADE_SELFITEM :
				{
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
					if(pPCTrade_Window)
					{
						param1 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
						param2 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
						param3 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucDay();
						param4 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucHour();
						param5 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

						sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) );
					}
				}
				break;
			case TMODE_PCTRADE_TARGETITEM :
				{
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
					if(pPCTrade_Window)
					{
						param1 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
						param2 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
						param3 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucDay();
						param4 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucHour();
						param5 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

						sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) );
					}
				}
				break;
			case TMODE_PERSONALSTORE_TRADEITEM :
				{
					_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PERSONALSTORE);
					if(pPersonalStore_Window)
					{
						param1 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
						param2 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
						param3 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucDay();
						param4 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucHour();
						param5 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

						sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) );
					}
				}
				break;
			case TMODE_NPCCOLLECTIONITEM :
				{
					int leftday = tempWeaponItem[m_ItemId].usTimeValue;
					sprintf(m_CurrentTooltipInfo.Data_Buf[13], _XGETINTERFACETEXT(ID_STRING_NEW_3814), leftday); //교환 후 %d일간 사용 가능

				}
				break;
			default :
				{
					param1 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) );
				}
				break;
			}
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
			if( length > 1 )
				m_TooltipSize.cy += DT_ROW_INTERVAL*(length-1);
		}
#endif

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
		int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
		if( nSetGroup > 0 )
		{
			int nSetItemAccout = g_pSetIteminfo->SearchGroupItem( nSetGroup ); // 몇개 입고 있는가..
			int nSetItemSkillCount = g_pSetIteminfo->SetGroupSkillString( nSetGroup, nSetItemAccout, m_TooltipSize, m_CurrentTooltipInfo ); //스킬 스트링 저장
			int nSetItemCount = g_pSetIteminfo->SetGroupItemString( nSetGroup, m_CurrentTooltipInfo ); //세트아이템 정보 저장

			if( nSetItemSkillCount > 0 ) //사이즈 늘리기
			{
				m_TooltipSize.cy += DT_LINE_INTERVAL*2;
				m_TooltipSize.cy += DT_ROW_INTERVAL*nSetItemSkillCount;
			}
			if( nSetItemCount > 0 )
			{
				m_SetItemExpandTooltipSize.cx = 194;
				m_SetItemExpandTooltipSize.cy = 16;
				m_SetItemExpandTooltipSize.cy += DT_ROW_INTERVAL*nSetItemCount;
			}
		}
#endif

	}
}

void _XWindow_DefaultTooltip::SetInfoClothesItem()
{
	if(m_ItemId <= 0)
		return;

	_XGI_ClothesItem_Property* tempClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
	int nTempDataBuf = 0;
	
	if( tempClothesItem )
	{
		if( tempClothesItem[m_ItemId].cNick )	// 별명
			strcpy( m_CurrentTooltipInfo.NickBuf, tempClothesItem[m_ItemId].cNick );
		
		if( tempClothesItem[m_ItemId].cName )	// 이름
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
#ifdef _XTS_ITEM_V20
			if( _XGameItem::GetItemNameAddtoString(m_ItemType, m_ItemId) )
			{
				sprintf( m_CurrentTooltipInfo.NameBuf, "%s %s", _XGameItem::GetItemNameAddtoString(m_ItemType, m_ItemId), tempClothesItem[m_ItemId].cName);
			}
			else
			{
				strcpy( m_CurrentTooltipInfo.NameBuf, tempClothesItem[m_ItemId].cName );
			}		
#else
			strcpy( m_CurrentTooltipInfo.NameBuf, tempClothesItem[m_ItemId].cName );
#endif
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		if( strlen( tempClothesItem[m_ItemId].cItemInfo ) )	// 설명
		{
			strcpy( m_CurrentTooltipInfo.DescBuf, tempClothesItem[m_ItemId].cItemInfo );
			
			m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 문파, 부위, 성별
		if( tempClothesItem[m_ItemId].cSexType )
		{
			if( tempClothesItem[m_ItemId].cSecondType == _XGI_SC_CLOTHES_MASK )
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[0], "%s %s", g_pClothesItemClanName[tempClothesItem[m_ItemId].cClan], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2389) );
			}
			else
				sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2014), 
				g_pClothesItemClanName[tempClothesItem[m_ItemId].cClan], 
				g_pClothesItemSecondName[tempClothesItem[m_ItemId].cSecondType] );
		}
		else
		{
			if( tempClothesItem[m_ItemId].cSecondType == _XGI_SC_CLOTHES_MASK )
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[0], "%s %s", g_pClothesItemClanName[tempClothesItem[m_ItemId].cClan], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2390) );
			}
			else
				sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_2015), 
				g_pClothesItemClanName[tempClothesItem[m_ItemId].cClan], 
				g_pClothesItemSecondName[tempClothesItem[m_ItemId].cSecondType] );
		}

		int length = g_XBaseFont->GetRowCount(m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16);
		m_TooltipSize.cy += DT_ROW_INTERVAL*(length-1);
		
		switch( tempClothesItem[m_ItemId].cSecondType )
		{
		case _XGI_SC_CLOTHES_UPPER :			// 상의 = 0
		case _XGI_SC_CLOTHES_LOWER :			// 하의 = 1
			{
				if( tempClothesItem[m_ItemId].cPocket )
				{
					m_TooltipSize.cy += DT_ROW_INTERVAL;		// 기능
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_POCKET), tempClothesItem[m_ItemId].cPocket );
				}
			}
			break;
		case _XGI_SC_CLOTHES_GLOVES :			// 아대(장갑) = 2
			{
				if( tempClothesItem[m_ItemId].cAttackRate )
				{
					m_TooltipSize.cy += DT_ROW_INTERVAL;		// 기능
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ATTACKRATEADD), tempClothesItem[m_ItemId].cAttackRate );
				}
			}
			break;
		case _XGI_SC_CLOTHES_SHOES :			// 신발 = 3
			{
				
			}
			break;
		case _XGI_SC_CLOTHES_CAP :			// 모자 = 4
			{
				if( tempClothesItem[m_ItemId].cEvasionRate )
				{
					m_TooltipSize.cy += DT_ROW_INTERVAL;		// 기능
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2391), tempClothesItem[m_ItemId].cEvasionRate );
				}
			}
			break;
		}				
		
		if( tempClothesItem[m_ItemId].cSecondType != _XGI_SC_CLOTHES_BACKPACK &&
			tempClothesItem[m_ItemId].cSecondType != _XGI_SC_CLOTHES_MASK )
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;	
			strcpy( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DURABILITY) );
			sprintf( m_CurrentTooltipInfo.Dur_Data_Buf, "( %d / %d )", (tempClothesItem[m_ItemId].usDurability+99)/100, (tempClothesItem[m_ItemId].usDurability+99)/100 );
			if( tempClothesItem[m_ItemId].sDefence > 0 )
				sprintf( m_CurrentTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2392), tempClothesItem[m_ItemId].sDefence );
		}
		
		if( tempClothesItem[m_ItemId].sReqLevel )
		{
			int tempstep = GetInnerLevel(tempClothesItem[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
			int templevel = ( tempClothesItem[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )				

			strcpy( m_CurrentTooltipInfo.Data_Buf[4], 
				g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[templevel], &tempstep ) );
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[4], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		if( tempClothesItem[m_ItemId].sReqJung )	// 근력
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;
			sprintf( m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQJUNG), tempClothesItem[m_ItemId].sReqJung );
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[5], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		if( tempClothesItem[m_ItemId].sReqSim )	// 민첩
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;
			sprintf( m_CurrentTooltipInfo.Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQSIM), tempClothesItem[m_ItemId].sReqSim );				
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[6], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		
		// 패널티 표현
		if( g_pLocalUser->m_CharacterInfo.Get_strength() < tempClothesItem[m_ItemId].sReqJung )
		{
			strcat( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_AVOIDABILLITY) );
		}
		
		if( g_pLocalUser->m_CharacterInfo.Get_dexterity() < tempClothesItem[m_ItemId].sReqSim )
		{
			if( strlen(m_CurrentTooltipInfo.Data_Buf[7]) )
				strcat( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2393) );
			else
				strcat( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DURABILITY) );
		}
		if( strlen(m_CurrentTooltipInfo.Data_Buf[7]) )
		{
			strcat( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DECREASE) );
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[7], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
		
#ifdef _XTS_FAME
		if( tempClothesItem[m_ItemId].ucFame > 0 )	// 명성 or 악명	
		{
			if( tempClothesItem[m_ItemId].ucFame > 100 )	// 악명
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[8], _XGETINTERFACETEXT(ID_STRING_NEW_2638), g_FameStr[tempClothesItem[m_ItemId].ucFame-101].m_InfamyName );	// "요구악명 %s 이상"
			}
			else	// 명성
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[8], _XGETINTERFACETEXT(ID_STRING_NEW_2639), g_FameStr[tempClothesItem[m_ItemId].ucFame-1].m_FameName ); //"요구명성 %s 이상"
			}
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[8], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
#endif

		// 스페셜 어빌리티	
		for( int i = 0; i < 5; i++ )
		{
			memset( m_CurrentTooltipInfo.Temp_Data_Buf, 0, sizeof(TCHAR)*128);
			
			if( tempClothesItem[m_ItemId].ucAdd_Prob[i] )
			{
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), tempClothesItem[m_ItemId].ucAdd_Prob[i] );
				strcat( m_CurrentTooltipInfo.S_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
			}
			
			if( tempClothesItem[m_ItemId].usAdd_SEID[i] )
			{
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[tempClothesItem[m_ItemId].usAdd_SEID[i]]->concept, 
					tempClothesItem[m_ItemId].usAdd_Value[i] );
			}		
			
			if( strlen(m_CurrentTooltipInfo.Temp_Data_Buf) )
			{
				strcat( m_CurrentTooltipInfo.S_Data_Buf[i], m_CurrentTooltipInfo.Temp_Data_Buf );
				
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.S_Data_Buf[i], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}		
		}

#ifdef _XDEF_ITEM_V25
		if(tempClothesItem[m_ItemId].ucCharacGrade == 2)
		{
			sprintf(m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_NEW_3797));	//2차 전직용
			int length = g_XBaseFont->GetRowCount(m_CurrentTooltipInfo.Data_Buf[9], m_TooltipSize.cx-16);
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
#endif
		
#ifdef _XDEF_PERIODITEM_20061027
		// 기간제 아이템이면 남은 시간을 표시한다.
		if( tempClothesItem[m_ItemId].cCashCheck == 2 || tempClothesItem[m_ItemId].cCashCheck == 3)
		{
			m_TooltipSize.cy += DT_ROW_INTERVAL;

			int param1, param2, param3, param4, param5;

			switch(m_CurrentTooltipMode)
			{
			case TMODE_TARGETINFO :
				{
					param1 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
				}
				break;
			case TMODE_WAREHOUSE_WAREHOUSEITEM :
				{
					param1 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
				}
				break;
			case TMODE_PCTRADE_SELFITEM :
				{
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
					if(pPCTrade_Window)
					{
						param1 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
						param2 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
						param3 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucDay();
						param4 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucHour();
						param5 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

						sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
					}
				}
				break;
			case TMODE_PCTRADE_TARGETITEM :
				{
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
					if(pPCTrade_Window)
					{
						param1 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
						param2 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
						param3 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucDay();
						param4 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucHour();
						param5 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

						sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
					}
				}
				break;
			case TMODE_PERSONALSTORE_TRADEITEM :
				{
					_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PERSONALSTORE);
					if(pPersonalStore_Window)
					{
						param1 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
						param2 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
						param3 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucDay();
						param4 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucHour();
						param5 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

						sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
					}
				}
				break;
			case TMODE_NPCCOLLECTIONITEM :
				{
					int leftday = tempClothesItem[m_ItemId].usTimeValue;
					sprintf(m_CurrentTooltipInfo.Data_Buf[13], _XGETINTERFACETEXT(ID_STRING_NEW_3814), leftday); //교환 후 %d일간 사용 가능

				}
				break;
			default :
				{
					param1 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
							g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
				}
				break;
			}
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
			if( length > 1 )
				m_TooltipSize.cy += DT_ROW_INTERVAL*(length-1);
		}
#endif

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
		int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
		if( nSetGroup > 0 )
		{
			int nSetItemAccout = g_pSetIteminfo->SearchGroupItem( nSetGroup ); // 몇개 입고 있는가..
			int nSetItemSkillCount = g_pSetIteminfo->SetGroupSkillString( nSetGroup, nSetItemAccout, m_TooltipSize, m_CurrentTooltipInfo ); //스킬 스트링 저장
			int nSetItemCount = g_pSetIteminfo->SetGroupItemString( nSetGroup, m_CurrentTooltipInfo ); //세트아이템 정보 저장

			if( nSetItemSkillCount > 0 ) //사이즈 늘리기
			{
				m_TooltipSize.cy += DT_LINE_INTERVAL*2;
				m_TooltipSize.cy += DT_ROW_INTERVAL*nSetItemSkillCount;
			}
			if( nSetItemCount > 0 )
			{
				m_SetItemExpandTooltipSize.cx = 194;
				m_SetItemExpandTooltipSize.cy = 16;
				m_SetItemExpandTooltipSize.cy += DT_ROW_INTERVAL*nSetItemCount;
			}
		}
#endif
	}
}


void _XWindow_DefaultTooltip::SetInfoBook()
{
	if( strlen( g_BookItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_BookItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;					
	}
	if( strlen( g_BookItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_BookItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_SKILL2 && g_BookItemProperty[m_ItemId].cThirdType == _XGI_TC_BOOK_GROUP )
	{
		if( g_BookItemProperty[m_ItemId].usReqSkill1 > 0 )		// 선수무공1 비교
		{
			int param2 = g_BookItemProperty[m_ItemId].cReqSkill1Step;
			strcpy( m_CurrentTooltipInfo.E_Data_Buf[3], 
					g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_2821, //"선수무공 %s %d성 완료"
													  g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill1]->skillName, &param2) );			
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[3], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			
			if( g_BookItemProperty[m_ItemId].usReqSkill2 > 0 )		// 선수무공2 비교
			{
				param2 = g_BookItemProperty[m_ItemId].cReqSkill2Step;
				strcpy( m_CurrentTooltipInfo.E_Data_Buf[4], 
					g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_2821, //"선수무공 %s %d성 완료"
													  g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill2]->skillName, &param2) );
				
				
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[4], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
				
				if( g_BookItemProperty[m_ItemId].usReqSkill3 > 0 )		// 선수무공3 비교
				{
					param2 = g_BookItemProperty[m_ItemId].cReqSkill3Step;
					strcpy( m_CurrentTooltipInfo.E_Data_Buf[5], 
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_2821, //"선수무공 %s %d성 완료"
						g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill3]->skillName, &param2) );
					
					int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[5], m_TooltipSize.cx-16 );
					m_TooltipSize.cy += DT_ROW_INTERVAL*length;
				}
			}
		}
	}
	else
	{
		if( g_BookItemProperty[m_ItemId].usReqSkill1 > 0 )		// 선수무공1 비교
		{
			LPTSTR param1 = g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill1]->skillName;
			int    param2 = g_BookItemProperty[m_ItemId].cReqSkill1Step;

			strcpy( m_CurrentTooltipInfo.E_Data_Buf[3],  g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2394, param1, &param2 ) );
			
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[3], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			
			if( g_BookItemProperty[m_ItemId].usReqSkill2 > 0 )		// 선수무공2 비교
			{
				LPTSTR param1 = g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill2]->skillName;
				int    param2 = g_BookItemProperty[m_ItemId].cReqSkill2Step;
				
				strcpy( m_CurrentTooltipInfo.E_Data_Buf[4],  g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2394, param1, &param2 ) );
				
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[4], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
				
				if( g_BookItemProperty[m_ItemId].usReqSkill3 > 0 )		// 선수무공3 비교
				{
					LPTSTR param1 = g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill3]->skillName;
					int    param2 = g_BookItemProperty[m_ItemId].cReqSkill3Step;
					
					strcpy( m_CurrentTooltipInfo.E_Data_Buf[5],  g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2394, param1, &param2 ) );
					
					int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[5], m_TooltipSize.cx-16 );
					m_TooltipSize.cy += DT_ROW_INTERVAL*length;
				}
			}
		}
	}	
	
#ifdef _XTS_SCROLLBOOK
	if( g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_SCROLL ||
		g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_STUFF ||
		g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_PAPER )
	{
		strcpy( m_CurrentTooltipInfo.E_Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2027) );
		m_TooltipSize.cy += DT_ROW_INTERVAL;

		LPTSTR param1 = g_pClothesItemClanName[g_BookItemProperty[m_ItemId].ucClan];
    #ifdef _XDEF_ITEM_V25 //Author : 양희왕 //breif : 2차전직도 추가 해야 될듯하다
        LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][g_BookItemProperty[m_ItemId].ucCharacGrade];
    #else
		LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][1];
    #endif
		strcpy( m_CurrentTooltipInfo.E_Data_Buf[7], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2028, param1, param2 ) );
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[7], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}	
#endif

	if( g_BookItemProperty[m_ItemId].usReqLevel )
	{
		int tempstep = GetInnerLevel(g_BookItemProperty[m_ItemId].usReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_BookItemProperty[m_ItemId].usReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )				
		
		strcpy( m_CurrentTooltipInfo.Data_Buf[0],
			    g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[templevel], &tempstep ) );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( g_BookItemProperty[m_ItemId].usReq_STR )	// 근력
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQJUNG), g_BookItemProperty[m_ItemId].usReq_STR );
	}
	if( g_BookItemProperty[m_ItemId].usReq_Force )	// 진기
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQFFORCE), g_BookItemProperty[m_ItemId].usReq_Force );				
	}
	if( g_BookItemProperty[m_ItemId].usReq_Intell )	// 지혜
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQINT), g_BookItemProperty[m_ItemId].usReq_Intell );				
	}
	if( g_BookItemProperty[m_ItemId].usReq_Const )	// 건강
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQCONST), g_BookItemProperty[m_ItemId].usReq_Const );				
	}
	if( g_BookItemProperty[m_ItemId].usReq_Dex )	// 민첩
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQSIM), g_BookItemProperty[m_ItemId].usReq_Dex );				
	}
	if( g_BookItemProperty[m_ItemId].usLife )	// 생명력
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQLIFE), g_BookItemProperty[m_ItemId].usLife );				
	}
	if( g_BookItemProperty[m_ItemId].usForce )	// 내공
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQFORCE), g_BookItemProperty[m_ItemId].usForce );				
	}
	if( g_BookItemProperty[m_ItemId].usConcent )	// 의지
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.Data_Buf[8], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQCONCENT), g_BookItemProperty[m_ItemId].usConcent );				
	}

#ifdef _XDEF_ITEM_V25
	if(g_BookItemProperty[m_ItemId].ucCharacGrade)	// 직책 등급
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf(m_CurrentTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_NEW_3797));	//_T("2차 전직용")
	}
#endif

	if( g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_STUDY )	// 무공서
	{
		//m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf( m_CurrentTooltipInfo.E_Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2025), g_BookItemProperty[m_ItemId].cSkillStep );	
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[2], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * templength-1;
		m_TooltipSize.cy += templength;

		switch(g_BookItemProperty[m_ItemId].cThirdType) 
		{
		case _XGI_TC_BOOK_APPOINTMENT :	// 지정 무공서
			{
				//m_TooltipSize.cy += DT_ROW_INTERVAL;
				strcpy( m_CurrentTooltipInfo.E_Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2026) );			

				int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[0], m_TooltipSize.cx-16 );
				templength += DT_ROW_INTERVAL * templength-1;
				m_TooltipSize.cy += templength;
				
				TCHAR tempBuf[128];
				memset(tempBuf, 0, sizeof(TCHAR)*128);

				int temp_time = g_BookItemProperty[m_ItemId].uiApplyTime / 60; // 분으로
				if( temp_time >= 60 )
				{
					temp_time /= 60;
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2029), temp_time );					
				}
				else
				{
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2030), temp_time );
				}

				strcpy( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				memset(tempBuf, 0, sizeof(TCHAR)*128);
				sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2031),g_SkillProperty[g_BookItemProperty[m_ItemId].usSkillID]->skillName );
				strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				memset(tempBuf, 0, sizeof(TCHAR)*128);
				sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
				strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}
			break;
		case _XGI_TC_BOOK_GROUP :	// 문파 무공서
			{
				//m_TooltipSize.cy += DT_ROW_INTERVAL;
				if( g_BookItemProperty[m_ItemId].ucClan != 20 )
				{
					sprintf( m_CurrentTooltipInfo.E_Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2033), g_JoinGroupName[g_BookItemProperty[m_ItemId].ucClan] );
				}
				else // 20이면 문파에 상관없이 공통으로 사용함.
				{
					sprintf( m_CurrentTooltipInfo.E_Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2033), _XGETINTERFACETEXT(ID_STRING_NEW_3352));
				}

				int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[0], m_TooltipSize.cx-16 );
				templength += DT_ROW_INTERVAL * templength-1;
				m_TooltipSize.cy += templength;

				TCHAR tempBuf[128];
				memset(tempBuf, 0, sizeof(TCHAR)*128);

				int temp_time = g_BookItemProperty[m_ItemId].uiApplyTime / 60; // 분으로
				if( temp_time >= 60 )
				{
					temp_time /= 60;
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2029), temp_time );					
				}
				else
				{
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2030), temp_time );
				}

				strcpy( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				memset(tempBuf, 0, sizeof(TCHAR)*128);
				if( g_BookItemProperty[m_ItemId].ucClan != 20 )
				{
					sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2034), g_JoinGroupName[g_BookItemProperty[m_ItemId].ucClan] );
				}
				else// 20이면 문파에 상관없이 공통으로 사용함.
				{
					sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2034), _XGETINTERFACETEXT(ID_STRING_NEW_3352) );
				}
				strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				memset(tempBuf, 0, sizeof(TCHAR)*128);
				sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
				strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}
			break;
		case _XGI_TC_BOOK_DUTIES :	// 역할 무공서
			{
				//m_TooltipSize.cy += DT_ROW_INTERVAL;
				if( g_BookItemProperty[m_ItemId].ucClan > 0 )
				{
					LPTSTR param1 = g_pClothesItemClanName[g_BookItemProperty[m_ItemId].ucClan];
                #ifdef _XDEF_ITEM_V25 //Author : 양희왕 //breif : 2차전직도 추가 해야 될듯하다
					LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][g_BookItemProperty[m_ItemId].ucCharacGrade];
                #else
                    LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][1];
                #endif

					strcpy( m_CurrentTooltipInfo.E_Data_Buf[0], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2035, param1, param2 ) );
				}
				else if( g_BookItemProperty[m_ItemId].ucClan == 0 )
				{
					sprintf( m_CurrentTooltipInfo.E_Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2036), 					
					g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][0] );
				}				

				int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[0], m_TooltipSize.cx-16 );
				templength += DT_ROW_INTERVAL * templength-1;
				m_TooltipSize.cy += templength;

				TCHAR tempBuf[128];
				memset(tempBuf, 0, sizeof(TCHAR)*128);

				int temp_time = g_BookItemProperty[m_ItemId].uiApplyTime / 60; // 분으로
				if( temp_time >= 60 )
				{
					temp_time /= 60;
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2029), temp_time );					
				}
				else
				{
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2030), temp_time );
				}

				strcpy( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				if( g_BookItemProperty[m_ItemId].ucClan > 0 )
				{
					memset(tempBuf, 0, sizeof(TCHAR)*128);

					LPTSTR param1 = g_pClothesItemClanName[g_BookItemProperty[m_ItemId].ucClan];
                #ifdef _XDEF_ITEM_V25 //Author : 양희왕 //breif : 2차전직도 추가 해야 될듯하다
                    LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][g_BookItemProperty[m_ItemId].ucCharacGrade];
                #else
					LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][1];
                #endif
					
					strcpy( tempBuf, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2037, param1, param2 ) );
					strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );
				}
				else if( g_BookItemProperty[m_ItemId].ucClan == 0 )
				{
					memset(tempBuf, 0, sizeof(TCHAR)*128);
					sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2038), g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][0] );
					strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );
				}				

				memset(tempBuf, 0, sizeof(TCHAR)*128);
				sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
				strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;				
			}
			break;
		case _XGI_TC_BOOK_SERIES :	// 계열 무공서
			{
				//m_TooltipSize.cy += DT_ROW_INTERVAL;

				LPTSTR param1 = g_pClothesItemClanName[g_BookItemProperty[m_ItemId].ucClan];
				LPTSTR param2 = g_SkillConceptProperty[g_BookItemProperty[m_ItemId].ucClan].stype_name[g_BookItemProperty[m_ItemId].ucSkillClass];

				strcpy( m_CurrentTooltipInfo.E_Data_Buf[0], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2039, param1, param2 ) );

				int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[0], m_TooltipSize.cx-16 );
				templength += DT_ROW_INTERVAL * templength-1;
				m_TooltipSize.cy += templength;

				TCHAR tempBuf[128];
				memset(tempBuf, 0, sizeof(TCHAR)*128);

				int temp_time = g_BookItemProperty[m_ItemId].uiApplyTime / 60; // 분으로
				if( temp_time >= 60 )
				{
					temp_time /= 60;
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2029), temp_time );					
				}
				else
				{
					sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2030), temp_time );
				}

				strcpy( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				memset(tempBuf, 0, sizeof(TCHAR)*128);

				param1 = g_pClothesItemClanName[g_BookItemProperty[m_ItemId].ucClan];
				param2 = g_SkillConceptProperty[g_BookItemProperty[m_ItemId].ucClan].stype_name[g_SkillProperty[g_BookItemProperty[m_ItemId].ucSkillClass]->skillSType];

				strcpy( tempBuf, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2040, param1, param2 ) );

				strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				memset(tempBuf, 0, sizeof(TCHAR)*128);
				sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
				strcat( m_CurrentTooltipInfo.E_Data_Buf[1], tempBuf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}
			break;
		}
	}
}

void _XWindow_DefaultTooltip::SetInfoPotion()
{
	if( strlen( g_PotionItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_PotionItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_PotionItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strncpy( m_CurrentTooltipInfo.DescBuf, g_PotionItemProperty[m_ItemId].cItemInfo, sizeof(m_CurrentTooltipInfo.DescBuf) );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	
	m_TooltipSize.cy += DT_ROW_INTERVAL;	// 종류
	switch(g_PotionItemProperty[m_ItemId].cSecondType) 
	{
	case _XGI_SC_POTION_HERT_IN:		// 내상
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_INTERNALPOTION) );
			if( g_PotionItemProperty[m_ItemId].sApplyValue > 0 )
			{
				switch(g_PotionItemProperty[m_ItemId].cApplyType) 
				{					
				case 0:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_INTERNALDECREASE), 
							 g_PotionItemProperty[m_ItemId].sApplyValue, g_PotionItemProperty[m_ItemId].sCooldown );
					break;
				case 1:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_INTERNALDECREASERATE), 
							 g_PotionItemProperty[m_ItemId].sApplyValue, g_PotionItemProperty[m_ItemId].sCooldown );
					break;
				}
			}				
		}				
		break;
	case _XGI_SC_POTION_HERT_OUT:		// 외상
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_EXTERNALPOTION) );
			if( g_PotionItemProperty[m_ItemId].sApplyValue > 0 )
			{
				switch(g_PotionItemProperty[m_ItemId].cApplyType) 
				{					
				case 0:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_EXTERNALDECREASE), 
							 g_PotionItemProperty[m_ItemId].sApplyValue, g_PotionItemProperty[m_ItemId].sCooldown );
					break;
				case 1:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_EXTERNALDECREASERATE), 
							 g_PotionItemProperty[m_ItemId].sApplyValue, g_PotionItemProperty[m_ItemId].sCooldown );
					break;
				}
			}
		}				
		break;
	case _XGI_SC_POTION_FATIGUE:		// 생명 회복 영약
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2041) );
			if( g_PotionItemProperty[m_ItemId].sApplyValue > 0 )
			{
				switch(g_PotionItemProperty[m_ItemId].cApplyType) 
				{					
				case 0:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2042), g_PotionItemProperty[m_ItemId].sApplyValue );
					break;
				case 1:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2043), g_PotionItemProperty[m_ItemId].sApplyValue );
					break;
				}
			}
		}				
		break;
	case _XGI_SC_POTION_NOTFIXED:		// 내공 회복 영약
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2044) );
			if( g_PotionItemProperty[m_ItemId].sApplyValue > 0 )
			{
				switch(g_PotionItemProperty[m_ItemId].cApplyType) 
				{					
				case 0:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2045), g_PotionItemProperty[m_ItemId].sApplyValue );
					break;
				case 1:
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2046), g_PotionItemProperty[m_ItemId].sApplyValue );
					break;
				}
			}
		}				
		break;
	case _XGI_SC_POTION_FOOD:		// 음식
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FOOD) );
			if( g_PotionItemProperty[m_ItemId].sApplyValue > 0 )
			{
				switch(g_PotionItemProperty[m_ItemId].cApplyType) 
				{					
				case 0:
					{
						sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2047), g_PotionItemProperty[m_ItemId].sApplyValue );
					}								
					break;
				case 1:
					{						
						sprintf( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2048), g_PotionItemProperty[m_ItemId].sApplyValue );
					}								
					break;
				}
			}
		}
		break;
	case _XGI_SC_POTION_EVENT:		// 신약
	case _XGI_SC_POTION_EVENT2 :
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2049) );
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_PotionItemProperty[m_ItemId].sEffectId);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( strlen(g_CharacterStateTable[g_PotionItemProperty[m_ItemId].sEffectId]->concept) )
				{
					sprintf( m_CurrentTooltipInfo.Data_Buf[1], g_CharacterStateTable[g_PotionItemProperty[m_ItemId].sEffectId]->concept, 
							g_PotionItemProperty[m_ItemId].sApplyValue );
				}
			}				
		}
		break;
	case _XGI_SC_POTION_HWATA :
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2050) );
			strcpy( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2051) );
		}
		break;
	case _XGI_SC_POTION_MASK :
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_NEW_2640) );
			strcpy( m_CurrentTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_NEW_2641) );
		}
		break;
	case _XGI_SC_POTION_ANTI :
		{
			//strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_NEW_2779) );	// _T("해독제")
		}
		break;
	}

	if( strlen(m_CurrentTooltipInfo.Data_Buf[1]) )
	{
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
		if( length > 0 )
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	
	if( g_PotionItemProperty[m_ItemId].sCooldown > 0 )
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		
#ifdef _XDEF_EXPANSION_POTION_070709_KUKURI
		if( g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_MASK  || 
			g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_EVENT ||
			g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_ANTI  ||
			g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_FOOD  ||
			g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_EVENT2 ) //Author : 양희왕 //breif : 신약 아이콘 모잘라서 추가
#else
		if( g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_MASK )
#endif
		{
			if(g_PotionItemProperty[m_ItemId].sCooldown < 60)	
				sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_2642), g_PotionItemProperty[m_ItemId].sCooldown );
			else
				sprintf(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_2643), g_PotionItemProperty[m_ItemId].sCooldown/60);
		}
		else
		{
			if(g_PotionItemProperty[m_ItemId].sCooldown < 60)
				sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2052), g_PotionItemProperty[m_ItemId].sCooldown );
			else
				sprintf(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2053), g_PotionItemProperty[m_ItemId].sCooldown/60);
		}		
	}				
	
	if( g_PotionItemProperty[m_ItemId].sReqLevel )
	{
		int tempstep = GetInnerLevel(g_PotionItemProperty[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_PotionItemProperty[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )				

		strcpy( m_CurrentTooltipInfo.Data_Buf[3],
			g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[templevel], &tempstep ) );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[3], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	//포션류는 심기 정기( 근력, 민첩) 값을 쓰지 않는다.
// 	if( g_PotionItemProperty[m_ItemId].sJungGi )	// 근력
// 	{
// 		m_TooltipSize.cy += DT_ROW_INTERVAL;
// 		sprintf( m_CurrentTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQJUNG), g_PotionItemProperty[m_ItemId].sJungGi );
// 	}
// 	if( g_PotionItemProperty[m_ItemId].sSimgi )	// 민첩
// 	{
// 		m_TooltipSize.cy += DT_ROW_INTERVAL;
// 		sprintf( m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQSIM), g_PotionItemProperty[m_ItemId].sSimgi );				
// 	}
}

void _XWindow_DefaultTooltip::SetInfoConsuming()
{
	if( strlen( g_ConsumingItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_ConsumingItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}

	if( strlen( g_ConsumingItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_ConsumingItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}

	m_TooltipSize.cy += DT_ROW_INTERVAL;
	sprintf( m_CurrentTooltipInfo.Data_Buf[0], "%s ", g_pClothesItemClanName[g_ConsumingItemProperty[m_ItemId].cClan] ); 
	strcat( m_CurrentTooltipInfo.Data_Buf[0], (g_ConsumingItemProperty[m_ItemId].cSecondType == 0 ) ? _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ASSICHARGE) : _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ASSIMINITERYARTS) );

	int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16 );
	m_TooltipSize.cy += DT_ROW_INTERVAL*(length-1);
	
	if( g_ConsumingItemProperty[m_ItemId].usSEID > 0 )
	{
		sprintf( m_CurrentTooltipInfo.Data_Buf[1], g_CharacterStateTable[g_ConsumingItemProperty[m_ItemId].usSEID]->concept, 
				g_ConsumingItemProperty[m_ItemId].usTime );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
}

void _XWindow_DefaultTooltip::SetElixir()
{
	if( strlen( g_ElixirItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_ElixirItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_ElixirItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_ElixirItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
		
	switch(g_ElixirItemProperty[m_ItemId].cSecondType) 
	{
	case _XGI_SC_ELIXIR_SUPPORT :
		{
			switch(g_ElixirItemProperty[m_ItemId].third_type ) 
			{
			case 1 :
				strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2054));
				break;
			case 2 :
				strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2055) );
				break;
			case 3 :
				strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2056) );
				break;
			}
			if( g_ElixirItemProperty[m_ItemId].usTemp[0] > 0)
			{
				int value;
				if( g_ElixirItemProperty[m_ItemId].third_type > 1 )
					value = g_ElixirItemProperty[m_ItemId].usValue_Min / 100;
				else 
					value = g_ElixirItemProperty[m_ItemId].usValue_Min;

                //Author : 양희왕 //breif : 6/20일 구전탕 같은 영약 미만으로 변경 됨 그러므로 + 1 해준다
                if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
                    sprintf( m_CurrentTooltipInfo.Data_Buf[1], g_CharacterStateTable[g_ElixirItemProperty[m_ItemId].usTemp[0]]->concept, value + 1 );
                else
                    sprintf( m_CurrentTooltipInfo.Data_Buf[1], g_CharacterStateTable[g_ElixirItemProperty[m_ItemId].usTemp[0]]->concept, value);

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}			
		}
		break;
	case _XGI_SC_ELIXIR_STRENGTH :
		sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2057), g_ElixirItemProperty[m_ItemId].ucStep, g_ElixirItemProperty[m_ItemId].usValue_Max );
		break;
	case _XGI_SC_ELIXIR_INNER :
		sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2058), g_ElixirItemProperty[m_ItemId].ucStep, g_ElixirItemProperty[m_ItemId].usValue_Max );
		break;
	case _XGI_SC_ELIXIR_HEART :
		sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2059), g_ElixirItemProperty[m_ItemId].ucStep, g_ElixirItemProperty[m_ItemId].usValue_Max );
		break;
	case _XGI_SC_ELIXIR_HEALTH :
		sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2060), g_ElixirItemProperty[m_ItemId].ucStep, g_ElixirItemProperty[m_ItemId].usValue_Max );
		break;
	case _XGI_SC_ELIXIR_DEX :
		sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2061), g_ElixirItemProperty[m_ItemId].ucStep, g_ElixirItemProperty[m_ItemId].usValue_Max );
		break;
	}

	if( strlen(m_CurrentTooltipInfo.Data_Buf[0]) )
	{
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;	// 종류
	}

	if( g_ElixirItemProperty[m_ItemId].ucReq_Level )
	{
		int tempstep = GetInnerLevel(g_ElixirItemProperty[m_ItemId].ucReq_Level);				// 성 ( 1 - 12 )
		int templevel = ( g_ElixirItemProperty[m_ItemId].ucReq_Level + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )				
		
		strcpy( m_CurrentTooltipInfo.Data_Buf[2],
			g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[templevel], &tempstep ) );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}	
}

void _XWindow_DefaultTooltip::SetInfoResource()
{
	if( strlen( g_ResourceItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_ResourceItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_ResourceItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_ResourceItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
}

void _XWindow_DefaultTooltip::SetInfoQuest()
{
	if( strlen( g_QuestItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_QuestItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_QuestItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_QuestItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}				
	
	m_TooltipSize.cy += DT_ROW_INTERVAL;
	strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_QUESTITEM));
}

void _XWindow_DefaultTooltip::SetInfoAccessory()
{
	if( strlen( g_AccessoryItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_AccessoryItemProperty[m_ItemId].cName );
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_AccessoryItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_AccessoryItemProperty[m_ItemId].cItemInfo );
		
		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	
	if( g_AccessoryItemProperty[m_ItemId].cSecondType == _XGI_SC_ACCESSORY_RING )
	{
		if( g_AccessoryItemProperty[m_ItemId].cClan == 0 )
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACCESSORYITEM)); // 소지품
		}
		else if( g_AccessoryItemProperty[m_ItemId].cClan == 11 )
		{
			// 보류 
		}
		else if( g_AccessoryItemProperty[m_ItemId].cClan == 12 )
		{
			
		}
		else if( g_AccessoryItemProperty[m_ItemId].cClan == 20 )
		{
			strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2062) );
		}
		else
		{
			sprintf( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2063), g_pClothesItemClanName[g_AccessoryItemProperty[m_ItemId].cClan] );
		}
	}
	else if( g_AccessoryItemProperty[m_ItemId].cSecondType == _XGI_SC_ACCESSORY_CHARM )
	{
		strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACCESSORYITEM) );
	}
#ifdef _XDEF_PERIODITEM_20061027
	else if( g_AccessoryItemProperty[m_ItemId].cSecondType == _XGI_SC_ACCESSORY_ARMOR_MULTI 
		|| g_AccessoryItemProperty[m_ItemId].cSecondType == _XGI_SC_ACCESSORY_ARMOR )
	{
		strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_NEW_3158)); // 유료화 소지품
	}
#endif

	if( strlen(m_CurrentTooltipInfo.Data_Buf[0]) )
		m_TooltipSize.cy += DT_ROW_INTERVAL;

	if( g_AccessoryItemProperty[m_ItemId].sReqLevel )
	{
		int tempstep = GetInnerLevel(g_AccessoryItemProperty[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_AccessoryItemProperty[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )				

		strcpy( m_CurrentTooltipInfo.Data_Buf[1],
				g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[templevel], &tempstep ) );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	
#ifdef _XTS_FAME
	if( g_AccessoryItemProperty[m_ItemId].ucFame > 0 )	// 명성 or 악명	
	{
		if( g_AccessoryItemProperty[m_ItemId].ucFame > 100 )	// 악명
		{
#ifdef _XTS_ITEM_V22
			if(g_AccessoryItemProperty[m_ItemId].ucFame == 199)
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_3136));
			}
			else
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_2638), g_FameStr[g_AccessoryItemProperty[m_ItemId].ucFame-101].m_InfamyName );
			}
#else
			sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_2638), g_FameStr[g_AccessoryItemProperty[m_ItemId].ucFame-101].m_InfamyName );
#endif
		}
		else	// 명성
		{
#ifdef _XTS_ITEM_V22
			if(g_AccessoryItemProperty[m_ItemId].ucFame == 99)
			{
				sprintf(m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_3135));
			}
			else
			{
				sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_2639), g_FameStr[g_AccessoryItemProperty[m_ItemId].ucFame-1].m_FameName );
			}
#else
			sprintf( m_CurrentTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_2639), g_FameStr[g_AccessoryItemProperty[m_ItemId].ucFame-1].m_FameName );
#endif
		}
		
		if( strlen(m_CurrentTooltipInfo.Data_Buf[2]) )
		{
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
		}
	}
#endif
	
#ifdef _XTS_ITEM_V22
	// 근력
	if(g_AccessoryItemProperty[m_ItemId].sReqStr)
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf(m_CurrentTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQJUNG), g_AccessoryItemProperty[m_ItemId].sReqStr);
	}
	// 건강
	if(g_AccessoryItemProperty[m_ItemId].sReqHealth)
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf(m_CurrentTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQCONST), g_AccessoryItemProperty[m_ItemId].sReqHealth);
	}
	// 진기
	if(g_AccessoryItemProperty[m_ItemId].sReqJingi)
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf(m_CurrentTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQFFORCE), g_AccessoryItemProperty[m_ItemId].sReqJingi);
	}
	// 지혜
	if(g_AccessoryItemProperty[m_ItemId].sReqInt)
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf(m_CurrentTooltipInfo.Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQINT), g_AccessoryItemProperty[m_ItemId].sReqInt);
	}
	// 민첩
	if(g_AccessoryItemProperty[m_ItemId].sReqDex)
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;
		sprintf(m_CurrentTooltipInfo.Data_Buf[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQSIM), g_AccessoryItemProperty[m_ItemId].sReqDex);
	}

	int databufindex = 8;
#else
	int databufindex = 3; 
#endif
	for( int i = 0; i < 5; i++ )
	{
		if( g_AccessoryItemProperty[m_ItemId].usAP_Per[i] )
		{
			sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_AccessoryItemProperty[m_ItemId].usAP_Per[i] );
			strcat( m_CurrentTooltipInfo.Data_Buf[databufindex], m_CurrentTooltipInfo.Temp_Data_Buf );
		}						
		if( g_AccessoryItemProperty[m_ItemId].ucAdd_Prob[i] )
		{
			sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_AccessoryItemProperty[m_ItemId].ucAdd_Prob[i] );
			strcat( m_CurrentTooltipInfo.Data_Buf[databufindex], m_CurrentTooltipInfo.Temp_Data_Buf );
		}
		
		if( g_AccessoryItemProperty[m_ItemId].usAdd_SEID[i] )
		{
			map <int, _XCharacterStateTable*>::iterator iter_cond = g_CharacterStateTable.find(g_AccessoryItemProperty[m_ItemId].usAdd_SEID[i]);
			if(iter_cond != g_CharacterStateTable.end())
			{
				if(g_CharacterStateTable[g_AccessoryItemProperty[m_ItemId].usAdd_SEID[i]]->concept)
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_AccessoryItemProperty[m_ItemId].usAdd_SEID[i]]->concept, 
						g_AccessoryItemProperty[m_ItemId].usAdd_Value[i] );
					strcat( m_CurrentTooltipInfo.Data_Buf[databufindex], m_CurrentTooltipInfo.Temp_Data_Buf );			
				}
			}
		}
		
		if( strlen(m_CurrentTooltipInfo.Data_Buf[databufindex]) )
		{
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[databufindex], m_TooltipSize.cx-16 );
			m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			databufindex++;
		}				
	}		
	
#ifdef _XDEF_PERIODITEM_20061027
	// 기간제 아이템이면 남은 시간을 표시한다.
	if( g_AccessoryItemProperty[m_ItemId].cCashCheck == 2 || g_AccessoryItemProperty[m_ItemId].cCashCheck == 3)
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;

		int param1, param2, param3, param4, param5;

		switch(m_CurrentTooltipMode)
		{
		case TMODE_TARGETINFO :
			{
				param1 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucYear()+1900;
				param2 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucMonth();
				param3 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucDay();
				param4 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucHour();
				param5 = m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucMinute();

				sprintf(m_CurrentTooltipInfo.Data_Buf[13],
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
			}
			break;
		case TMODE_WAREHOUSE_WAREHOUSEITEM :
			{
				param1 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucYear()+1900;
				param2 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucMonth();
				param3 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucDay();
				param4 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucHour();
				param5 = g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucMinute();

				sprintf(m_CurrentTooltipInfo.Data_Buf[13],
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
			}
			break;
		case TMODE_PCTRADE_SELFITEM :
				{
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
					if(pPCTrade_Window)
					{
						param1 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
						param2 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
						param3 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucDay();
						param4 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucHour();
						param5 = pPCTrade_Window->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

						sprintf(m_CurrentTooltipInfo.Data_Buf[13],
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
					}
				}
				break;
		case TMODE_PCTRADE_TARGETITEM :
			{
				_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
				if(pPCTrade_Window)
				{
					param1 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = pPCTrade_Window->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
				}
			}
			break;
		case TMODE_PERSONALSTORE_TRADEITEM :
			{
				_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PERSONALSTORE);
				if(pPersonalStore_Window)
				{
					param1 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucYear()+1900;
					param2 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucMonth();
					param3 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucDay();
					param4 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucHour();
					param5 = pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucMinute();

					sprintf(m_CurrentTooltipInfo.Data_Buf[13],
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
				}
			}
			break;
		case TMODE_NPCCOLLECTIONITEM :
			{
				int leftday = g_AccessoryItemProperty[m_ItemId].usTimeValue;
				sprintf(m_CurrentTooltipInfo.Data_Buf[13], _XGETINTERFACETEXT(ID_STRING_NEW_3814), leftday); //교환 후 %d일간 사용 가능

			}
			break;
		default :
			{
				param1 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucYear()+1900;
				param2 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucMonth();
				param3 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucDay();
				param4 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucHour();
				param5 = g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucMinute();

				sprintf(m_CurrentTooltipInfo.Data_Buf[13],
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
							&param1, &param2, &param3, &param4,	&param5 ) ); 
			}
			break;
		}

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
		if(length > 1)
			m_TooltipSize.cy += DT_ROW_INTERVAL*(length-1);		
	}
#endif
	
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
	if( nSetGroup > 0 )
	{
		int nSetItemAccout = g_pSetIteminfo->SearchGroupItem( nSetGroup ); // 몇개 입고 있는가..
		int nSetItemSkillCount = g_pSetIteminfo->SetGroupSkillString( nSetGroup, nSetItemAccout, m_TooltipSize, m_CurrentTooltipInfo ); //스킬 스트링 저장
		int nSetItemCount = g_pSetIteminfo->SetGroupItemString( nSetGroup, m_CurrentTooltipInfo ); //세트아이템 정보 저장

		if( nSetItemSkillCount > 0 ) //사이즈 늘리기
		{
			m_TooltipSize.cy += DT_LINE_INTERVAL*2;
			m_TooltipSize.cy += DT_ROW_INTERVAL*nSetItemSkillCount;
		}
		if( nSetItemCount > 0 )
		{
			m_SetItemExpandTooltipSize.cx = 194;
			m_SetItemExpandTooltipSize.cy = 16;
			m_SetItemExpandTooltipSize.cy += DT_ROW_INTERVAL*nSetItemCount;
		}
	}
#endif
}

void _XWindow_DefaultTooltip::SetInfoLife()
{
	if( strlen( g_LifeItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_LifeItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_LifeItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_LifeItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
}

void _XWindow_DefaultTooltip::SetInfoSocket()
{
	if( strlen( g_SocketItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_SocketItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_SocketItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_SocketItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}

	switch(g_SocketItemProperty[m_ItemId].cSecondType) 
	{
	case 0:		// 무기
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID1);
			if(iter_table != g_CharacterStateTable.end())
			{
				m_TooltipSize.cy += DT_ROW_INTERVAL;
				strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2064));

				if( g_SocketItemProperty[m_ItemId].sAP_Per1 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per1 );
					strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb1 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb1 );
					strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );
				}

				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID1]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue1 );
				strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}

			iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID2);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_SocketItemProperty[m_ItemId].sAP_Per2 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per2 );
					strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb1 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb2 );
					strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );
				}

				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID2]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue2 );
				strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}
		}
		break;
	case 1:		// 의상
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID3);
			if(iter_table != g_CharacterStateTable.end())
			{
				m_TooltipSize.cy += DT_ROW_INTERVAL;
				strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2065));

				if( g_SocketItemProperty[m_ItemId].sAP_Per3 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per3 );
					strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb3 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb3 );
					strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );
				}

				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID3]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue3 );
				strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}

			iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID4);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_SocketItemProperty[m_ItemId].sAP_Per4 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per4 );
					strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb3 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb4 );
					strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );
				}

				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID4]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue4 );
				strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}
		}
		break;
	case 2:		// 공용
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID1);
			if(iter_table != g_CharacterStateTable.end())
			{
				m_TooltipSize.cy += DT_ROW_INTERVAL;
				strcpy( m_CurrentTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DECOITEM)); // 무기 의상 공통장식물
				
				if( g_SocketItemProperty[m_ItemId].sAP_Per1 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per1 );
					strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb1 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb1 );
					strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );
				}
				
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID1]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue1 );
				strcat( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2066));
				strcat( m_CurrentTooltipInfo.Data_Buf[1], m_CurrentTooltipInfo.Temp_Data_Buf );


				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}

			//---------
			iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID2);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_SocketItemProperty[m_ItemId].sAP_Per2 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per2 );
					strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb2 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb2 );
					strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID2]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue2 );
				strcat( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2066) );
				strcat( m_CurrentTooltipInfo.Data_Buf[2], m_CurrentTooltipInfo.Temp_Data_Buf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}

			//---------
			iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID3);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_SocketItemProperty[m_ItemId].sAP_Per3 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per3 );
					strcat( m_CurrentTooltipInfo.Data_Buf[3], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb3 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb3 );
					strcat( m_CurrentTooltipInfo.Data_Buf[3], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID3]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue3 );
				strcat( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2067));
				strcat( m_CurrentTooltipInfo.Data_Buf[3], m_CurrentTooltipInfo.Temp_Data_Buf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[3], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}

			//---------
			iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_ItemId].sSkill_Effect_ID4);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_SocketItemProperty[m_ItemId].sAP_Per4 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_ItemId].sAP_Per4 );
					strcat( m_CurrentTooltipInfo.Data_Buf[4], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				if( g_SocketItemProperty[m_ItemId].cProb4 )
				{
					sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_ItemId].cProb4 );
					strcat( m_CurrentTooltipInfo.Data_Buf[4], m_CurrentTooltipInfo.Temp_Data_Buf );
				}						
				sprintf( m_CurrentTooltipInfo.Temp_Data_Buf, g_CharacterStateTable[g_SocketItemProperty[m_ItemId].sSkill_Effect_ID4]->concept, 
						 g_SocketItemProperty[m_ItemId].sValue4 );
				strcat( m_CurrentTooltipInfo.Temp_Data_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2067));
				strcat( m_CurrentTooltipInfo.Data_Buf[4], m_CurrentTooltipInfo.Temp_Data_Buf );

				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[4], m_TooltipSize.cx-16 );
				m_TooltipSize.cy += DT_ROW_INTERVAL*length;
			}
		}
		break;			
	}
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 //breif : 치장의상 소켓 장착 방식 변경
	//레벨
	if( g_SocketItemProperty[m_ItemId].sTempField5 )
	{
		int tempstep = GetInnerLevel(g_SocketItemProperty[m_ItemId].sTempField5);				// 성 ( 1 - 12 )
		int templevel = ( g_SocketItemProperty[m_ItemId].sTempField5 + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )

		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();

		strcpy( m_CurrentTooltipInfo.Data_Buf[5], 
		g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_REQLEVEL, g_LevelName[templevel], &tempstep ) );			

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[5], m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
#endif

}

void _XWindow_DefaultTooltip::SetInfoBox()
{
	if( strlen( g_BoxItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_BoxItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_BoxItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_BoxItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
}

void _XWindow_DefaultTooltip::SetInfoBoxKey()
{
	if( strlen( g_BoxKeyItemProperty[m_ItemId].cName ) )	// 이름
	{
		m_TooltipSize.cy += DT_ROW_INTERVAL;		// 별명 +1
		strcpy( m_CurrentTooltipInfo.NameBuf, g_BoxKeyItemProperty[m_ItemId].cName );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.NameBuf, m_TooltipSize.cx-51 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
	if( strlen( g_BoxKeyItemProperty[m_ItemId].cItemInfo ) )	// 설명
	{
		strcpy( m_CurrentTooltipInfo.DescBuf, g_BoxKeyItemProperty[m_ItemId].cItemInfo );

		m_TooltipSize.cy += DT_LINE_INTERVAL*2;					
		
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.DescBuf, m_TooltipSize.cx-16 );
		m_TooltipSize.cy += DT_ROW_INTERVAL*length;
	}
}

void _XWindow_DefaultTooltip::DrawTextWeaponItem()
{
	_XGI_WeaponItem_Property* tempWeaponitem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
	if( tempWeaponitem )
	{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
		int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
		int nSetlength = g_pSetIteminfo->GetSkillStringCount();

		if( nSetlength > 0 && nSetGroup > 0)
		{
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
							m_WindowPosition.x+m_TooltipSize.cx-8, 
							m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL), 
							D3DCOLOR_ARGB(25,255,255,255) );
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
							 m_WindowPosition.x+m_TooltipSize.cx-8, 
							 m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL),
							 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

			m_CurrentDrawPosY += 4;
            g_pSetIteminfo->DrawSkillToolTip( nSetGroup, m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_TooltipSize, m_CurrentDrawPosY, m_CurrentTooltipInfo );

            /*g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));
			for( int i = 0; i < nSetlength; i++ )
			{
				if( strlen( m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] ) )
                {
					g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] );
					m_CurrentDrawPosY += DT_ROW_INTERVAL;
				}
			}*/
		}
#endif
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
		{
			m_CurrentDrawPosY += DT_LINE_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
			if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
			{
				g_XBaseFont->SetColor(0xFFFFD901);			
				g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[4] );
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			}
		}
		
		if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );
			if( g_LanguageType != _XLANGUAGE_TYPE_VIETNAMESE )
			{	
				if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
				}
			}	
		}	
		if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[3] );		
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{	
				if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
				}
			}	
		}
		
		if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL; 
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
			DrawDurability();	
		}
		
#ifdef _XDEF_SMELT_ITEM	//Author : 양희왕
		if( strlen( m_CurrentTooltipInfo.Data_Buf[11] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->SetColor( 0xFF00F064 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[11] );
			
			int nSmallFontDraw = 0;

			for( int i = 0; i < GetSmeltItem_MaxScaleInfo(); i++)
			{
				if( GetSmeltItem_ScaleInfo() > i )
				{
					_XDrawSolidBar( m_WindowPosition.x+ 60 +(i*3), m_WindowPosition.y + m_CurrentDrawPosY,
						m_WindowPosition.x+60+2+(i*3), m_WindowPosition.y + m_CurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
				}
				else
				{
					_XDrawSolidBar( m_WindowPosition.x+ 60 +(i*3), m_WindowPosition.y + m_CurrentDrawPosY,
						m_WindowPosition.x+60+2+(i*3), m_WindowPosition.y + m_CurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
				}
				nSmallFontDraw = (i*3); // 어차피 가장 마지막 i 의 값이 들어감
			}
			sprintf( m_CurrentTooltipInfo.Strength_Data_Buf, "( %d / %d )", GetSmeltItem_ScaleInfo(), GetSmeltItem_MaxScaleInfo() );

			g_XSmallFont->SetColor( _XSC_DEFAULT );
			g_XSmallFont->PutsAlign(m_WindowPosition.x + 65 + nSmallFontDraw, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Strength_Data_Buf );
			g_XSmallFont->Flush();
		}

		if( strlen( m_CurrentTooltipInfo.Data_Buf[12] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			if( GetSmeltItem_TearInfo() == 0 )
			{
				g_XBaseFont->SetColor( 0xFF00F064 );
			}
			else
			{
				g_XBaseFont->SetColor( 0xFFFA4141 );
			}
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[12] );
		}
#endif //Last Updated : 07/04/25


//Author : 양희왕 //breif : 수집 조합 가능 메시지
#ifdef _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER
		int nMixWeaponRes = _XGameItem::GetTempField5( m_ItemType, m_ItemId ); // 메시지 처리 방법을 얻어온다
		if( strlen( m_CurrentTooltipInfo.Data_Buf[14] ) )
		{
			if(nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 3)
				g_XBaseFont->SetColor( 0xFFFA4141 );
			else
				g_XBaseFont->SetColor( 0xFF00F064 );

			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[14] );
		}
		
		if( strlen( m_CurrentTooltipInfo.Data_Buf[15]) )
		{
		//Author : 양희왕 //breif : 제련된거 없앤다 Date : 08/07/31 패치
		#ifdef _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731
			if(nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 4)
		#else
			if( GetInchantInfo() || nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 4)
		#endif
				g_XBaseFont->SetColor( 0xFFFA4141 );
			else
				g_XBaseFont->SetColor( 0xFF00F064 );

			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[15] );
		}
#endif

		
		if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			int tempstep = GetInnerLevel(tempWeaponitem[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
			int templevel = ( tempWeaponitem[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
			
			int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
			int itemlevel = templevel*12+tempstep;
			
			if( selflevel >= itemlevel )
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			}
			else 
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
			g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[6], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

			int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[6], m_TooltipSize.cx-16 );
			templength += DT_ROW_INTERVAL * (templength-1);
			m_CurrentDrawPosY += templength;
		}
		if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			int result = 0;
			if( tempWeaponitem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			{
#ifdef _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI
				result = g_pLocalUser->m_CharacterInfo.Get_intelligence();
#else
				result = g_pLocalUser->m_CharacterInfo.Get_zen();
#endif
			}
			else
			{
				result = g_pLocalUser->m_CharacterInfo.Get_strength();
			}
			if( result >= tempWeaponitem[m_ItemId].sReqJung )
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
		}
		if( strlen( m_CurrentTooltipInfo.Data_Buf[8] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			int result = 0;
			if( tempWeaponitem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			{
#ifdef _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI
				result = g_pLocalUser->m_CharacterInfo.Get_dexterity();
#else
				result = g_pLocalUser->m_CharacterInfo.Get_intelligence();
#endif
			}
			else
			{
				result = g_pLocalUser->m_CharacterInfo.Get_dexterity();
			}
			if( result >= tempWeaponitem[m_ItemId].sReqSim )
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[8] );
		}
		if( strlen( m_CurrentTooltipInfo.Data_Buf[9] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[9] );
		}
		
#ifdef _XTS_FAME
		if( strlen( m_CurrentTooltipInfo.Data_Buf[10] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			if( g_FameStr->CheckItemFame(tempWeaponitem[m_ItemId].ucFame) )
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));

			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[10] );
		}
#endif
		if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
		{
			DrawItemPrice(m_CurrentDrawPosY += DT_ROW_INTERVAL);
		}
		
		if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
		{
			if( m_pContribution_Window )
			{
				if( m_pContribution_Window->GetShowStatus() )
				{
					DrawItemClanPoint(m_CurrentDrawPosY += DT_ROW_INTERVAL);
				}			
			}
		}
#ifdef _XDEF_PERIODITEM_20061027
		if(tempWeaponitem[m_ItemId].cCashCheck == 2 || tempWeaponitem[m_ItemId].cCashCheck == 3)
		{
			// 기간제 아이템이면 남은 시간 표시
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			g_XBaseFont->SetColor(_XSC_INFORMATION);
			g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
			
			if(templength>1)
				m_CurrentDrawPosY += DT_ROW_INTERVAL * (templength-1);
		}
#endif
	}
}

void _XWindow_DefaultTooltip::DrawTextClothesItem()
{
	_XGI_ClothesItem_Property* tempClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
	if( tempClothesItem )
	{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
		int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
		int nSetlength = g_pSetIteminfo->GetSkillStringCount();

		if( nSetlength > 0 && nSetGroup > 0)
		{
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
							m_WindowPosition.x+m_TooltipSize.cx-8, 
							m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL), 
							D3DCOLOR_ARGB(25,255,255,255) );
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
							 m_WindowPosition.x+m_TooltipSize.cx-8, 
							 m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL),
							 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

			m_CurrentDrawPosY += 4;
            g_pSetIteminfo->DrawSkillToolTip( nSetGroup, m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_TooltipSize, m_CurrentDrawPosY, m_CurrentTooltipInfo );

            /*g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));
			for( int i = 0; i < nSetlength; i++ )
			{
				if( strlen( m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] ) )
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] );
					m_CurrentDrawPosY += DT_ROW_INTERVAL;
				}
			}*/
		}
#endif

		g_XBaseFont->SetColor(_XSC_DEFAULT);
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
		{
			m_CurrentDrawPosY += DT_LINE_INTERVAL;
			//if( _XGameItem::CheckClan( m_ItemType, m_ItemId ) &&
			if( ( tempClothesItem[m_ItemId].cClan == _XGROUP_NANGIN || g_pLocalUser->m_CharacterInfo.groupindex == tempClothesItem[m_ItemId].cClan ) &&
				g_pLocalUser->m_CharacterInfo.gender == tempClothesItem[m_ItemId].cSexType )
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			}
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));

//			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );

			g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[0], 
				m_TooltipSize.cx-16, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);
			int length = g_XBaseFont->GetRowCount(m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16);
			if(length>1)
				m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
		}
		
		if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );		
		}
		
		if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[2] );
			DrawDurability();
			if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
				g_XBaseFont->PutsAlign( m_WindowPosition.x+m_TooltipSize.cx - 10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[3] );		
		}		
		
		if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			int tempstep = GetInnerLevel(tempClothesItem[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
			int templevel = ( tempClothesItem[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
			
			int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
			int itemlevel = templevel*12+tempstep;
			
			if( selflevel >= itemlevel )			
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
			g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[4], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[4], m_TooltipSize.cx-16 );
			if(templength>1)
				m_CurrentDrawPosY += DT_ROW_INTERVAL * (templength-1);
		}
		if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			int result = g_pLocalUser->m_Strength.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
			if( result >= tempClothesItem[m_ItemId].sReqJung )
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
		}
		if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			int result = g_pLocalUser->m_Dex.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
			if( result >= tempClothesItem[m_ItemId].sReqSim )
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
		}
		if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
		}
#ifdef _XTS_FAME
		if( strlen( m_CurrentTooltipInfo.Data_Buf[8] ) )
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			if( g_FameStr->CheckItemFame(tempClothesItem[m_ItemId].ucFame) )
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));

			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[8] );
		}
#endif

#ifdef _XDEF_ITEM_V25
		if(strlen(m_CurrentTooltipInfo.Data_Buf[9]))
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			if(g_pLocalUser->m_CharacterInfo.Get_cClassGrade() == 2)
				g_XBaseFont->SetColor(_XSC_DEFAULT);
			else
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));

			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[9]);
		}
#endif
		
#ifdef _XDEF_PERIODITEM_20061027
		// 기간제 아이템이면 남은 시간을 표시한다.
		if( tempClothesItem[m_ItemId].cCashCheck == 2 || tempClothesItem[m_ItemId].cCashCheck == 3)
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			g_XBaseFont->SetColor(_XSC_INFORMATION);
			g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
			
			if(templength>1)
				m_CurrentDrawPosY += DT_ROW_INTERVAL * (templength-1);
		}
#endif

		if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
		{
			DrawItemPrice(m_CurrentDrawPosY += DT_ROW_INTERVAL);
                
		}
		
		if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
		{
			if( m_pContribution_Window )
			{
				if( m_pContribution_Window->GetShowStatus() )
				{
					DrawItemClanPoint(m_CurrentDrawPosY += DT_ROW_INTERVAL);
				}			
			}
		}
	}
}


/*
void _XWindow_DefaultTooltip::DrawTextWeapon()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
		if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
		{
			g_XBaseFont->SetColor(0xFFFFD901);			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[4] );
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );
		
		if( g_LanguageType != _XLANGUAGE_TYPE_VIETNAMESE )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
			}
		}
	}	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[3] );		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
			}
		}
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
		DrawDurability();	
	}	
#ifdef _XDEF_SMELT_ITEM	//Author : 양희왕
		
#endif //Last Updated : 07/04/25
	
	int cashcheck = CheckCashItem(_XGI_FC_WEAPON, m_ItemId);
	//Author : 양희왕 //breif : 수집 조합 가능 메시지
#ifdef _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER
	int nMixWeaponRes = _XGameItem::GetTempField5( m_ItemType, m_ItemId ); // 메시지 처리 방법을 얻어온다
	if( strlen( m_CurrentTooltipInfo.Data_Buf[14] ) )
	{
		if(nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 3)
			g_XBaseFont->SetColor( 0xFFFA4141 );
		else
			g_XBaseFont->SetColor( 0xFF00F064 );

		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[14] );
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[15]) )
	{
		if( GetInchantInfo() || nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 4)
			g_XBaseFont->SetColor( 0xFFFA4141 );
		else
			g_XBaseFont->SetColor( 0xFF00F064 );

		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[15] );
	}
#endif
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_WeaponItemProperty[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_WeaponItemProperty[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )

		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;

		if( selflevel >= itemlevel )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
		else 
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = 0;
		if( g_WeaponItemProperty[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			result = g_pLocalUser->m_CharacterInfo.Get_zen();
		else
			result = g_pLocalUser->m_CharacterInfo.Get_strength();	

		if( result >= g_WeaponItemProperty[m_ItemId].sReqJung )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));

		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[8] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = 0;
		if( g_WeaponItemProperty[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			result = g_pLocalUser->m_CharacterInfo.Get_intelligence();
		else
			result = g_pLocalUser->m_CharacterInfo.Get_dexterity();
		if( result >= g_WeaponItemProperty[m_ItemId].sReqSim )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[8] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[9] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[9] );
	}

#ifdef _XDEF_PERIODITEM_20061027
	if(cashcheck == 2 || cashcheck == 3)
	{
		// 기간제 아이템이면 남은 시간 표시
		m_CurrentDrawPosY += DT_ROW_INTERVAL;

		g_XBaseFont->SetColor(_XSC_INFORMATION);
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
		
		if(templength>1)
			m_CurrentDrawPosY += DT_ROW_INTERVAL * (templength-1);
	}
#endif

	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		DrawItemPrice();
	}

	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
				DrawItemClanPoint();
			}			
		}
	}		
}
*/

/*
void _XWindow_DefaultTooltip::DrawTextWeapon2()
{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
	int nSetlength = g_pSetIteminfo->GetSkillStringCount();

	if( nSetlength > 0 && nSetGroup > 0)
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;

		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						m_WindowPosition.x+m_TooltipSize.cx-8, 
						m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL), 
						D3DCOLOR_ARGB(25,255,255,255) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						 m_WindowPosition.x+m_TooltipSize.cx-8, 
						 m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL),
						 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));
		m_CurrentDrawPosY += 4;
		for( int i = 0; i < nSetlength; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] );
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
			}
		}
	}
#endif

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
		if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
		{
			g_XBaseFont->SetColor(0xFFFFD901);
			g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[4] );
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );
		
		if( g_LanguageType != _XLANGUAGE_TYPE_VIETNAMESE )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
			}
		}
	}	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[3] );
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
			}
		}		
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
		DrawDurability();	
	}	

		//Author : 양희왕 //breif : 수집 조합 가능 메시지
	int cashcheck = CheckCashItem(_XGI_FC_WEAPON2, m_ItemId);
#ifdef _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER
	int nMixWeaponRes = _XGameItem::GetTempField5( m_ItemType, m_ItemId ); // 메시지 처리 방법을 얻어온다
	if( strlen( m_CurrentTooltipInfo.Data_Buf[14] ) )
	{
		if(nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 3)
			g_XBaseFont->SetColor( 0xFFFA4141 );
		else
			g_XBaseFont->SetColor( 0xFF00F064 );

		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[14] );
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[15]) )
	{
		if( GetInchantInfo() || nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 4)
			g_XBaseFont->SetColor( 0xFFFA4141 );
		else
			g_XBaseFont->SetColor( 0xFF00F064 );

		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[15] );
	}
#endif
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_WeaponItemProperty2[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_WeaponItemProperty2[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )

		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;

		if( selflevel >= itemlevel )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
		else 
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = 0;
		if( g_WeaponItemProperty2[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			result = g_pLocalUser->m_CharacterInfo.Get_zen();
		else
			result = g_pLocalUser->m_CharacterInfo.Get_strength();

		if( result >= g_WeaponItemProperty2[m_ItemId].sReqJung )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));

		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[8] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = 0;
		if( g_WeaponItemProperty2[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			result = g_pLocalUser->m_CharacterInfo.Get_intelligence();
		else
			result = g_pLocalUser->m_CharacterInfo.Get_dexterity();

		if( result >= g_WeaponItemProperty2[m_ItemId].sReqSim )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[8] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[9] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[9] );
	}

#ifdef _XDEF_PERIODITEM_20061027
	if(cashcheck == 2 || cashcheck == 3)
	{
		// 기간제 아이템이면 남은 시간 표시
		m_CurrentDrawPosY += DT_ROW_INTERVAL;

		g_XBaseFont->SetColor(_XSC_INFORMATION);
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
		
		if(templength>1)
			m_CurrentDrawPosY += DT_ROW_INTERVAL * (templength-1);
	}
#endif


	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		DrawItemPrice();
	}

	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
				DrawItemClanPoint();
			}			
		}
	}
}
*/
/*
void _XWindow_DefaultTooltip::DrawTextWeapon3()
{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
	int nSetlength = g_pSetIteminfo->GetSkillStringCount();

	if( nSetlength > 0 && nSetGroup > 0)
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;

		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						m_WindowPosition.x+m_TooltipSize.cx-8, 
						m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL), 
						D3DCOLOR_ARGB(25,255,255,255) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						 m_WindowPosition.x+m_TooltipSize.cx-8, 
						 m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL),
						 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));
		m_CurrentDrawPosY += 4;
		for( int i = 0; i < nSetlength; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] );
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
			}
		}
	}
#endif

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
		if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
		{
			g_XBaseFont->SetColor(0xFFFFD901);
			g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[4] );
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );
		if( g_LanguageType != _XLANGUAGE_TYPE_VIETNAMESE )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
			}
		}		
	}	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[3] );		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+m_TooltipSize.cx - 8, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[2] );
			}
		}		
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
		DrawDurability();	
	}	

		//Author : 양희왕 //breif : 수집 조합 가능 메시지
	int cashcheck = CheckCashItem(_XGI_FC_WEAPON3, m_ItemId);
#ifdef _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER
	int nMixWeaponRes = _XGameItem::GetTempField5( m_ItemType, m_ItemId ); // 메시지 처리 방법을 얻어온다
	if( strlen( m_CurrentTooltipInfo.Data_Buf[14] ) )
	{
		if(nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 3)
			g_XBaseFont->SetColor( 0xFFFA4141 );
		else
			g_XBaseFont->SetColor( 0xFF00F064 );

		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[14] );
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[15]) )
	{
		if( GetInchantInfo() || nMixWeaponRes == 0 || nMixWeaponRes == 2 || nMixWeaponRes == 4)
			g_XBaseFont->SetColor( 0xFFFA4141 );
		else
			g_XBaseFont->SetColor( 0xFF00F064 );

		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[15] );
	}
#endif
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_WeaponItemProperty3[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_WeaponItemProperty3[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )

		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;

		if( selflevel >= itemlevel )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
		else 
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = 0;
		if( g_WeaponItemProperty3[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			result = g_pLocalUser->m_CharacterInfo.Get_zen();
		else
			result = g_pLocalUser->m_CharacterInfo.Get_strength();

		if( result >= g_WeaponItemProperty3[m_ItemId].sReqJung )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));

		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[8] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = 0;
		if( g_WeaponItemProperty3[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
			result = g_pLocalUser->m_CharacterInfo.Get_intelligence();
		else
			result = g_pLocalUser->m_CharacterInfo.Get_dexterity();

		if( result >= g_WeaponItemProperty3[m_ItemId].sReqSim )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[8] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[9] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[9] );
	}

#ifdef _XDEF_PERIODITEM_20061027
	if(cashcheck == 2 || cashcheck == 3)
	{
		// 기간제 아이템이면 남은 시간 표시
		m_CurrentDrawPosY += DT_ROW_INTERVAL;

		g_XBaseFont->SetColor(_XSC_INFORMATION);
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
		
		if(templength>1)
			m_CurrentDrawPosY += DT_ROW_INTERVAL * (templength-1);
	}
#endif

	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		DrawItemPrice();
	}

	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
				DrawItemClanPoint();
			}			
		}
	}
}
*/
// 사용안함 - 주의!!!
/*
void _XWindow_DefaultTooltip::DrawTextClothes()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		//if( _XGameItem::CheckClan( m_ItemType, m_ItemId ) &&
		if( ( g_ClothesItemProperty[m_ItemId].cClan == _XGROUP_NANGIN || g_pLocalUser->m_CharacterInfo.groupindex == g_ClothesItemProperty[m_ItemId].cClan ) &&
			g_pLocalUser->m_CharacterInfo.gender == g_ClothesItemProperty[m_ItemId].cSexType )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));

//		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );

		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[0], 
			m_TooltipSize.cx-16, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);
		int length = g_XBaseFont->GetRowCount(m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16);
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );		
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[2] );
		DrawDurability();
		if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
			g_XBaseFont->PutsAlign( m_WindowPosition.x+m_TooltipSize.cx - 10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[3] );		
	}		
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_ClothesItemProperty[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_ClothesItemProperty[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		
		if( selflevel >= itemlevel )			
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[4] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = g_pLocalUser->m_Strength.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
		if( result >= g_ClothesItemProperty[m_ItemId].sReqJung )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = g_pLocalUser->m_Dex.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
		if( result >= g_ClothesItemProperty[m_ItemId].sReqSim )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
	}
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		DrawItemPrice();			
	}

	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
				DrawItemClanPoint();
			}			
		}
	}
}
*/

/*
void _XWindow_DefaultTooltip::DrawTextClothes2()
{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
	int nSetlength = g_pSetIteminfo->GetSkillStringCount();

	if( nSetlength > 0 && nSetGroup > 0)
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;

		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						m_WindowPosition.x+m_TooltipSize.cx-8, 
						m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL), 
						D3DCOLOR_ARGB(25,255,255,255) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						 m_WindowPosition.x+m_TooltipSize.cx-8, 
						 m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL),
						 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));
		m_CurrentDrawPosY += 4;
		for( int i = 0; i < nSetlength; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] );
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
			}
		}
	}
#endif

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
//		if( _XGameItem::CheckClan( m_ItemType, m_ItemId ) &&
		if( ( g_ClothesItemProperty2[m_ItemId].cClan == _XGROUP_NANGIN || g_pLocalUser->m_CharacterInfo.groupindex == g_ClothesItemProperty2[m_ItemId].cClan ) &&
			g_pLocalUser->m_CharacterInfo.gender == g_ClothesItemProperty2[m_ItemId].cSexType )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));

//		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );

		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[0], 
			m_TooltipSize.cx-16, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);
		int length = g_XBaseFont->GetRowCount(m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16);
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );		
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[2] );
		DrawDurability();
		if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
			g_XBaseFont->PutsAlign( m_WindowPosition.x+m_TooltipSize.cx - 10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[3] );		
	}		
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_ClothesItemProperty2[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_ClothesItemProperty2[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		
		if( selflevel >= itemlevel )			
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[4] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = g_pLocalUser->m_Strength.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
		if( result >= g_ClothesItemProperty2[m_ItemId].sReqJung )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = g_pLocalUser->m_Dex.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
		if( result >= g_ClothesItemProperty2[m_ItemId].sReqSim )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
	}

	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		DrawItemPrice();			
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
				DrawItemClanPoint();
			}			
		}
	}
}
*/

/*
void _XWindow_DefaultTooltip::DrawTextClothes3()
{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
	int nSetlength = g_pSetIteminfo->GetSkillStringCount();

	if( nSetlength > 0 && nSetGroup > 0)
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;

		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						m_WindowPosition.x+m_TooltipSize.cx-8, 
						m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL), 
						D3DCOLOR_ARGB(25,255,255,255) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						 m_WindowPosition.x+m_TooltipSize.cx-8, 
						 m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL),
						 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));
		m_CurrentDrawPosY += 4;
		for( int i = 0; i < nSetlength; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] );
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
			}
		}
	}
#endif

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
//		if( _XGameItem::CheckClan( m_ItemType, m_ItemId ) &&
		if( ( g_ClothesItemProperty3[m_ItemId].cClan == _XGROUP_NANGIN || g_pLocalUser->m_CharacterInfo.groupindex == g_ClothesItemProperty3[m_ItemId].cClan ) &&
			g_pLocalUser->m_CharacterInfo.gender == g_ClothesItemProperty3[m_ItemId].cSexType )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));

//		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );

		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[0], 
			m_TooltipSize.cx-16, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);
		int length = g_XBaseFont->GetRowCount(m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16);
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );		
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[2] );
		DrawDurability();
		if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
			g_XBaseFont->PutsAlign( m_WindowPosition.x+m_TooltipSize.cx - 10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentTooltipInfo.Data_Buf[3] );		
	}		
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_ClothesItemProperty3[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_ClothesItemProperty3[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		
		if( selflevel >= itemlevel )			
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[4] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = g_pLocalUser->m_Strength.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
		if( result >= g_ClothesItemProperty3[m_ItemId].sReqJung )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int result = g_pLocalUser->m_Dex.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
		if( result >= g_ClothesItemProperty3[m_ItemId].sReqSim )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
	}

	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
		DrawItemPrice();			
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
				DrawItemClanPoint();
			}			
		}
	}
}
*/

void _XWindow_DefaultTooltip::DrawTextBook()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	m_CurrentDrawPosY += DT_LINE_INTERVAL;

	if( strlen( m_CurrentTooltipInfo.E_Data_Buf[0] ) )
	{
		g_XBaseFont->SetColor(0xFFFFFE00);
		
		int rowcount = g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[0], m_TooltipSize.cx - 16, 1.0, 3 );
		//g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[0] );
		rowcount += DT_ROW_INTERVAL * rowcount-1;		
		m_CurrentDrawPosY += rowcount;
	}

	if( strlen( m_CurrentTooltipInfo.E_Data_Buf[2] ) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		int rowcount = g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[2], m_TooltipSize.cx - 16, 1.0, 3 );
		//g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[2] );
		rowcount += DT_ROW_INTERVAL * rowcount-1;		
		m_CurrentDrawPosY += rowcount;
	}

	if( strlen( m_CurrentTooltipInfo.E_Data_Buf[1] ) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[1], m_TooltipSize.cx - 16, 1.0, 3 );
		
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[1], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * templength-1;
		m_CurrentDrawPosY += templength;
	}

	if( strlen( m_CurrentTooltipInfo.E_Data_Buf[3] ) )
	{
		int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[m_ItemId].usReqSkill1);
		if( tempskilllevel < 0 )
		{
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));			
		}
		else
		{
			if( g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_SKILL2 )
			{
				if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[m_ItemId].cReqSkill1Step-1 )
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
				else
				{
					if( g_BookItemProperty[m_ItemId].cThirdType == _XGI_TC_BOOK_GROUP )
					{
						int curskillindex = g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel;
						FLOAT exp = (FLOAT)g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill1]->detailInfo[curskillindex+1].skillExp;
						
						if( ((FLOAT)g_pLocalUser->m_SkillList[tempskilllevel].m_iSkillExp / exp) < 1.0f )
							g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
						else
							g_XBaseFont->SetColor(_XSC_DEFAULT);
					}
				}			
			}
			else
			{
				if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel < g_BookItemProperty[m_ItemId].cReqSkill1Step-1 )
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));			
				else
					g_XBaseFont->SetColor(_XSC_DEFAULT);
			}
		}

		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[3], m_TooltipSize.cx - 16, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[3], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * templength-1;
		m_CurrentDrawPosY += templength;

		if( strlen( m_CurrentTooltipInfo.E_Data_Buf[4] ) )
		{
			int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[m_ItemId].usReqSkill2);
			if( tempskilllevel < 0 )
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));			
			}
			else
			{
				if( g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_SKILL2 )
				{
					if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[m_ItemId].cReqSkill2Step-1 )
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
					else
					{
						if( g_BookItemProperty[m_ItemId].cThirdType == _XGI_TC_BOOK_GROUP )
						{
							int curskillindex = g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel;
							FLOAT exp = (FLOAT)g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill2]->detailInfo[curskillindex+1].skillExp;
							
							if( ((FLOAT)g_pLocalUser->m_SkillList[tempskilllevel].m_iSkillExp / exp) < 1.0f )
								g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
							else
								g_XBaseFont->SetColor(_XSC_DEFAULT);
						}
					}			
				}
				else
				{
					if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel < g_BookItemProperty[m_ItemId].cReqSkill2Step-1 )
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));			
					else
						g_XBaseFont->SetColor(_XSC_DEFAULT);
				}
			}
			
			g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[4], m_TooltipSize.cx - 16, 1.0, 3 );
			int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[4], m_TooltipSize.cx-16 );
			templength += DT_ROW_INTERVAL * templength-1;
			m_CurrentDrawPosY += templength;

			if( strlen( m_CurrentTooltipInfo.E_Data_Buf[5] ) )
			{
				int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[m_ItemId].usReqSkill3);
				if( tempskilllevel < 0 )
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));			
				}
				else
				{
					if( g_BookItemProperty[m_ItemId].cSecondType == _XGI_SC_BOOK_SKILL2 )
					{
						if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[m_ItemId].cReqSkill3Step-1 )
							g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
						else
						{
							if( g_BookItemProperty[m_ItemId].cThirdType == _XGI_TC_BOOK_GROUP )
							{
								int curskillindex = g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel;
								FLOAT exp = (FLOAT)g_SkillProperty[g_BookItemProperty[m_ItemId].usReqSkill3]->detailInfo[curskillindex+1].skillExp;
								
								if( ((FLOAT)g_pLocalUser->m_SkillList[tempskilllevel].m_iSkillExp / exp) < 1.0f )
									g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
								else
									g_XBaseFont->SetColor(_XSC_DEFAULT);
							}
						}			
					}
					else
					{
						if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel < g_BookItemProperty[m_ItemId].cReqSkill3Step-1 )
							g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));			
						else
							g_XBaseFont->SetColor(_XSC_DEFAULT);
					}
				}
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[5], m_TooltipSize.cx - 16, 1.0, 3 );
				int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[5], m_TooltipSize.cx-16 );
				templength += DT_ROW_INTERVAL * templength-1;
				m_CurrentDrawPosY += templength;
			}
		}
	}

#ifdef _XTS_SCROLLBOOK
	if( strlen(m_CurrentTooltipInfo.E_Data_Buf[6]) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[6] );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+11, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.E_Data_Buf[6] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
	if( strlen(m_CurrentTooltipInfo.E_Data_Buf[7]) )
	{
		if( g_BookItemProperty[m_ItemId].ucClan == g_pLocalUser->m_CharacterInfo.groupindex && g_pLocalUser->m_CharacterInfo.cClass == g_BookItemProperty[m_ItemId].ucClass )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));

		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.E_Data_Buf[7], m_TooltipSize.cx - 16, 1.0, 3 );		
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.E_Data_Buf[7], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * templength-1;
		m_CurrentDrawPosY += templength;
		
	}	
#endif

	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{			
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		int tempstep = GetInnerLevel(g_BookItemProperty[m_ItemId].usReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_BookItemProperty[m_ItemId].usReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		
		if( selflevel >= itemlevel )		
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * templength;
		m_CurrentDrawPosY += templength;
	}	

	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )	// 근력
	{
		if( g_pLocalUser->m_CharacterInfo.Get_strength() >= g_BookItemProperty[m_ItemId].usReq_STR )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[1] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )	// 진기
	{
		if( g_pLocalUser->m_CharacterInfo.Get_zen() >= g_BookItemProperty[m_ItemId].usReq_Force )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[2] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )	// 지혜
	{
		if( g_pLocalUser->m_CharacterInfo.Get_intelligence() >= g_BookItemProperty[m_ItemId].usReq_Intell )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[3] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )	// 건강
	{
		if( g_pLocalUser->m_CharacterInfo.Get_constitution() >= g_BookItemProperty[m_ItemId].usReq_Const )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[4] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )	// 민첩
	{
		if( g_pLocalUser->m_CharacterInfo.Get_dexterity() >= g_BookItemProperty[m_ItemId].usReq_Dex )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )	// 생명력
	{
		if( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() >= g_BookItemProperty[m_ItemId].usLife )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )	// 내공
	{
		if( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() >= g_BookItemProperty[m_ItemId].usForce )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
	if( strlen( m_CurrentTooltipInfo.Data_Buf[8] ) )	// 의지
	{
		if( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() >= g_BookItemProperty[m_ItemId].usConcent )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[8] );
		m_CurrentDrawPosY += DT_ROW_INTERVAL;		
	}
#ifdef _XDEF_ITEM_V25
	if(strlen(m_CurrentTooltipInfo.Data_Buf[9]))	// 직책 등급
	{
		if(g_pLocalUser->m_CharacterInfo.Get_cClassGrade() == 2)
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));

		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[9]);
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
	}
#endif
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		/*bool flag = false;
		for( int i = 0; i < 9; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[i] ) )
			{
				flag = true;
				break;
			}					
		}
		if( flag )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;*/
		
		DrawItemPrice(m_CurrentDrawPosY);			
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				//m_CurrentDrawPosY += DT_ROW_INTERVAL;
				DrawItemClanPoint(m_CurrentDrawPosY);
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawTextPotion()
{
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		g_XBaseFont->SetColor(0xFFFFFE00);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
	}

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx - 16, 1.0, 3 );
		
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * (templength-1);
		m_CurrentDrawPosY += templength;
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[2] );
	} 	
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_PotionItemProperty[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_PotionItemProperty[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		
		if( selflevel >= itemlevel )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[3], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[3], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * (templength-1);
		m_CurrentDrawPosY += templength;
	}
	//포션류는 심기 정기( 근력, 민첩) 값을 쓰지 않는다.
// 	if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
// 	{
// 		m_CurrentDrawPosY += DT_ROW_INTERVAL;
// 		if( g_pLocalUser->m_CharacterInfo.strength >= g_PotionItemProperty[m_ItemId].sJungGi )
// 			g_XBaseFont->SetColor(_XSC_DEFAULT);
// 		else
// 			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
// 		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[4] );
// 	}
// 	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
// 	{
// 		m_CurrentDrawPosY += DT_ROW_INTERVAL;
// 		if( g_pLocalUser->m_CharacterInfo.dexterity >= g_PotionItemProperty[m_ItemId].sSimgi )
// 			g_XBaseFont->SetColor(_XSC_DEFAULT);
// 		else
// 			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
// 		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
// 	}	
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
        DrawItemPrice(m_CurrentDrawPosY += DT_ROW_INTERVAL);
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
               DrawItemClanPoint(m_CurrentDrawPosY += DT_ROW_INTERVAL);
                   
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawTextConsuming()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen(m_CurrentTooltipInfo.Data_Buf[0]) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		if( g_ConsumingItemProperty[m_ItemId].cClan != 0 && g_ConsumingItemProperty[m_ItemId].cClan != g_pLocalUser->m_CharacterInfo.groupindex )
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		else
			g_XBaseFont->SetColor(0xFFFFFE00);
//		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
	
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[0],
			 m_TooltipSize.cx-16, _XFONT_ALIGNTYPE_LEFT,  1.0f, 3);
		int length = g_XBaseFont->GetRowCount(m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16);
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
	}
	if( strlen(m_CurrentTooltipInfo.Data_Buf[1]) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx - 16, 1.0, 3 );			
	}
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		int length = 1;
		if(strlen(m_CurrentTooltipInfo.Data_Buf[1]))
		{
			length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
		}
		
		DrawItemPrice(m_CurrentDrawPosY += DT_ROW_INTERVAL*length);
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				DrawItemClanPoint(m_CurrentDrawPosY += DT_ROW_INTERVAL);
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawElixir()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		g_XBaseFont->SetColor(0xFFFFFE00);
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[0], m_TooltipSize.cx-16 );
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
		m_CurrentDrawPosY += DT_ROW_INTERVAL*(length-1);
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
		m_CurrentDrawPosY += DT_ROW_INTERVAL;
		int tempstep = GetInnerLevel(g_ElixirItemProperty[m_ItemId].ucReq_Level);				// 성 ( 1 - 12 )
		int templevel = ( g_ElixirItemProperty[m_ItemId].ucReq_Level + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		
		if( selflevel >= itemlevel )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * (templength-1);
		m_CurrentDrawPosY += templength;
	}

	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		DrawItemPrice(m_CurrentDrawPosY += DT_ROW_INTERVAL);
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				DrawItemClanPoint(m_CurrentDrawPosY += DT_ROW_INTERVAL);
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawTextResource()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		DrawItemPrice(m_CurrentDrawPosY += DT_LINE_INTERVAL);
            
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				DrawItemClanPoint(m_CurrentDrawPosY += DT_LINE_INTERVAL);
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawTextQuest()
{
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		g_XBaseFont->SetColor(0xFFFFFE00);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
	}
	g_XBaseFont->SetColor(_XSC_DEFAULT);
}

void _XWindow_DefaultTooltip::DrawTextAccessory()
{

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	int nSetGroup = _XGameItem::GetSetItem(m_ItemType, m_ItemId);
	int nSetlength = g_pSetIteminfo->GetSkillStringCount();
    
	if( nSetlength > 0 && nSetGroup > 0)
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;

		_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						m_WindowPosition.x+m_TooltipSize.cx-8, 
						m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL), 
						D3DCOLOR_ARGB(25,255,255,255) );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+m_CurrentDrawPosY, 
						 m_WindowPosition.x+m_TooltipSize.cx-8, 
						 m_WindowPosition.y+m_CurrentDrawPosY+(nSetlength*DT_ROW_INTERVAL),
						 0.0f,D3DCOLOR_ARGB(51,255,255,255) );

		m_CurrentDrawPosY += 4;
        g_pSetIteminfo->DrawSkillToolTip( nSetGroup, m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_TooltipSize, m_CurrentDrawPosY, m_CurrentTooltipInfo );

        /*g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));
		for( int i = 0; i < nSetlength; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] ) )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.SetItemSkill_Data_Buf[i] );
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
			}
		}*/
	}
#endif

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		if( g_AccessoryItemProperty[m_ItemId].cClan != 0 && g_AccessoryItemProperty[m_ItemId].cClan != g_pLocalUser->m_CharacterInfo.groupindex &&
			g_AccessoryItemProperty[m_ItemId].cClan != 20 )
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		else
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[1] ) )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;
		
		int tempstep = GetInnerLevel(g_AccessoryItemProperty[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
		int templevel = ( g_AccessoryItemProperty[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
		
		int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
		int itemlevel = templevel*12+tempstep;
		
		if( selflevel >= itemlevel )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[1], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * (templength-1);
		m_CurrentDrawPosY += templength;
	}

	if( strlen( m_CurrentTooltipInfo.Data_Buf[2] ) )
	{
#ifdef _XTS_FAME
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

		if( g_FameStr->CheckItemFame(g_AccessoryItemProperty[m_ItemId].ucFame) )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,11,0));
		
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[2], m_TooltipSize.cx-16 );
		templength += DT_ROW_INTERVAL * (templength-1);
		m_CurrentDrawPosY += templength;
#endif
	}

	int databufindex = 3;

#ifdef _XTS_ITEM_V22
	// 근력
	if( strlen( m_CurrentTooltipInfo.Data_Buf[3] ) )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

		if( g_pLocalUser->m_CharacterInfo.Get_strength() >= g_AccessoryItemProperty[m_ItemId].sReqStr )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[3] );
	}
	// 건강
	if( strlen( m_CurrentTooltipInfo.Data_Buf[4] ) )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

		if( g_pLocalUser->m_CharacterInfo.Get_constitution() >= g_AccessoryItemProperty[m_ItemId].sReqHealth )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[4] );
	}
	// 진기
	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

		if( g_pLocalUser->m_CharacterInfo.Get_zen() >= g_AccessoryItemProperty[m_ItemId].sReqJingi )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[5] );
	}
	// 지혜
	if( strlen( m_CurrentTooltipInfo.Data_Buf[6] ) )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

		if( g_pLocalUser->m_CharacterInfo.Get_intelligence() >= g_AccessoryItemProperty[m_ItemId].sReqInt )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[6] );
	}
	// 민첩
	if( strlen( m_CurrentTooltipInfo.Data_Buf[7] ) )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

		if( g_pLocalUser->m_CharacterInfo.Get_dexterity() >= g_AccessoryItemProperty[m_ItemId].sReqDex )
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		else
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
			
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[7] );
	}

	databufindex = 8;
#endif
	if( strlen( m_CurrentTooltipInfo.Data_Buf[databufindex] ) )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
			m_CurrentDrawPosY += DT_ROW_INTERVAL;			
		else
			m_CurrentDrawPosY += DT_LINE_INTERVAL;

		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[databufindex], m_TooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[databufindex], m_TooltipSize.cx-16 );
		if( length > 0 )
			length--;
		m_CurrentDrawPosY += DT_ROW_INTERVAL*length;

		for( int i = databufindex+1; i < databufindex+5; i++ )
		{
			if( strlen( m_CurrentTooltipInfo.Data_Buf[i] ) )
			{
				m_CurrentDrawPosY += DT_ROW_INTERVAL;
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[i], m_TooltipSize.cx - 16, 1.0, 3 );
				int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[i], m_TooltipSize.cx-16 );
				if( length > 0 )
					length--;

				m_CurrentDrawPosY += DT_ROW_INTERVAL*length;
			}
		}
#ifdef _XDEF_PERIODITEM_20061027
		// 기간제 아이템이면 남은 시간을 표시한다.
		if( g_AccessoryItemProperty[m_ItemId].cCashCheck == 2 || g_AccessoryItemProperty[m_ItemId].cCashCheck == 3)
		{
			m_CurrentDrawPosY += DT_ROW_INTERVAL;

			g_XBaseFont->SetColor(_XSC_INFORMATION);
			g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			int templength = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[13], m_TooltipSize.cx-16 );
			if( templength > 1)
				m_CurrentDrawPosY += DT_ROW_INTERVAL * (templength-1);
		}
#endif
	}

	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
        if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
    		DrawItemPrice(m_CurrentDrawPosY += DT_ROW_INTERVAL);			
		else
			DrawItemPrice(m_CurrentDrawPosY += DT_LINE_INTERVAL);		
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
                if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) ) 
					DrawItemClanPoint(m_CurrentDrawPosY += DT_ROW_INTERVAL);
				else
					DrawItemClanPoint(m_CurrentDrawPosY += DT_LINE_INTERVAL);
			}			
		}
	}
	
}

void _XWindow_DefaultTooltip::DrawTextLife()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
        DrawItemPrice(m_CurrentDrawPosY += DT_LINE_INTERVAL);		
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				DrawItemClanPoint(m_CurrentDrawPosY += DT_LINE_INTERVAL);
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawTextSocket()
{
	if( strlen( m_CurrentTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += DT_LINE_INTERVAL;
		g_XBaseFont->SetColor(0xFFFFFE00);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentTooltipInfo.Data_Buf[0] );
	}
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	
	m_CurrentDrawPosY += DT_ROW_INTERVAL;
	for( int i = 1; i < 5; i++ )
	{
		if( strlen( m_CurrentTooltipInfo.Data_Buf[i] ) )
		{				
			g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[i], m_TooltipSize.cx - 16, 1.0, 3 );
			int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[i], m_TooltipSize.cx-16 );
			m_CurrentDrawPosY += DT_ROW_INTERVAL*length;
		}			
	}

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 //breif : 치장의상 소켓 장착 방식 변경
	if( strlen( m_CurrentTooltipInfo.Data_Buf[5] ) )
	{
		//본인레벨 >= 소켓레벨
		if( (g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel()) >= g_SocketItemProperty[m_ItemId].sTempField5 )
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
		}
		else 
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,220,156,156));
		
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentTooltipInfo.Data_Buf[5], m_TooltipSize.cx - 16, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

		int length = g_XBaseFont->GetRowCount( m_CurrentTooltipInfo.Data_Buf[5], m_TooltipSize.cx-16 );
		m_CurrentDrawPosY += DT_ROW_INTERVAL * length;
	}
	g_XBaseFont->SetColor(_XSC_DEFAULT);
#endif
	
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		DrawItemPrice(m_CurrentDrawPosY);			
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				DrawItemClanPoint(m_CurrentDrawPosY);
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawTextBox()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		DrawItemPrice(m_CurrentDrawPosY += DT_LINE_INTERVAL);
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				DrawItemClanPoint(m_CurrentDrawPosY += DT_LINE_INTERVAL);
			}			
		}
	}
}

void _XWindow_DefaultTooltip::DrawTextBoxKey()
{
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	if( m_CurrentTooltipMode == TMODE_NPCTRADE_INVENITEM || m_CurrentTooltipMode == TMODE_NPCTRADE_NPCITEM )
	{
		DrawItemPrice(m_CurrentDrawPosY += DT_LINE_INTERVAL);
	}
	if( m_CurrentTooltipMode == TMODE_DEFAULTITEM )
	{
		if( m_pContribution_Window )
		{
			if( m_pContribution_Window->GetShowStatus() )
			{
				DrawItemClanPoint(m_CurrentDrawPosY += DT_LINE_INTERVAL);
			}			
		}
	}
}

unsigned char _XWindow_DefaultTooltip::GetInchantInfo(void)
{
	switch( m_CurrentTooltipMode ) 
	{
	case TMODE_DEFAULTITEM :
	case TMODE_CINFOITEM :
	case TMODE_NPCTRADE_INVENITEM :
	case TMODE_REPAIRITEM :
			return g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_ucStrength();
	case TMODE_WAREHOUSE_WAREHOUSEITEM :
		return g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].Get_m_ucStrength();			
	case TMODE_PCTRADE_TARGETITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].Get_m_ucStrength();				
			}
		}
		break;
	case TMODE_PCTRADE_SELFITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].Get_m_ucStrength();
			}
		}
		break;
	case TMODE_PERSONALSTORE_TRADEITEM :
		{
			if( m_pPersonalStore_Window )
			{
				return m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].Get_m_ucStrength();
			}
		}
		break;
	case TMODE_TARGETINFO :
		{
			if( m_pTargetInfo_Window )
			{
				return m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].Get_m_ucStrength();
			}
		}
		break;
	}
	return 0;
}

unsigned short _XWindow_DefaultTooltip::GetSmeltItem_ScaleInfo()
{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 용린값을 얻어온다
	_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ItemType);

	switch( m_CurrentTooltipMode ) 
	{
	case TMODE_NPCTRADE_INVENITEM :
		{
				return g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurProtect;
		}
		break;
	case TMODE_LIFESKILLCOLLECTIONITEM :
	case TMODE_NPCRESOURCEITEM :
	case TMODE_LIFESKILLRESOURCEITEM :
	case TMODE_NPCTRADE_NPCITEM :
	case TMODE_NPCCOLLECTIONITEM :
		return 0;
		break;
	case TMODE_DEFAULTITEM :
	case TMODE_CINFOITEM :
	case TMODE_REPAIRITEM :
	case TMODE_INCHANT :
	case TMODE_QUEST :
	case TMODE_NPCQUEST :
	case TMODE_GAMBLE :
	case TMODE_CASHITEM :
		return g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurProtect;
		break;
	case TMODE_WAREHOUSE_WAREHOUSEITEM :
		return g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].m_usCurProtect;			
		break;
	case TMODE_PCTRADE_TARGETITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].m_usCurProtect;				
			}
		}
		break;
	case TMODE_PCTRADE_SELFITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].m_usCurProtect;
			}
		}
		break;
	case TMODE_PERSONALSTORE_TRADEITEM :
		{
			if( m_pPersonalStore_Window )
			{
				return m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].m_usCurProtect;
			}
		}
		break;
	case TMODE_TARGETINFO :
		{
			if( m_pTargetInfo_Window )
			{
				return m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].m_usCurProtect;
			}
		}
		break;
	}
#endif

	return 0;
}
unsigned short _XWindow_DefaultTooltip::GetSmeltItem_MaxScaleInfo()
{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 용린 최대값을 얻어온다
	_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ItemType);

	switch( m_CurrentTooltipMode ) 
	{
	case TMODE_NPCTRADE_INVENITEM :
		{
			return g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurMaxProtect;
		}
		break;
	case TMODE_LIFESKILLCOLLECTIONITEM :
	case TMODE_NPCRESOURCEITEM :
	case TMODE_LIFESKILLRESOURCEITEM :
	case TMODE_NPCTRADE_NPCITEM :
	case TMODE_NPCCOLLECTIONITEM :
		return _XGameItem::GetNature( tempWeaponItem[m_ItemId].cType, tempWeaponItem[m_ItemId].sID);
		break;
	case TMODE_DEFAULTITEM :
	case TMODE_CINFOITEM :
	case TMODE_REPAIRITEM :
	case TMODE_INCHANT :
	case TMODE_QUEST :
	case TMODE_NPCQUEST :
	case TMODE_GAMBLE :
	case TMODE_CASHITEM :
		return g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurMaxProtect;
		break;
	case TMODE_WAREHOUSE_WAREHOUSEITEM :
		return g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].m_usCurMaxProtect;			
		break;
	case TMODE_PCTRADE_TARGETITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].m_usCurMaxProtect;				
			}
		}
		break;
	case TMODE_PCTRADE_SELFITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].m_usCurMaxProtect;
			}
		}
		break;
	case TMODE_PERSONALSTORE_TRADEITEM :
		{
			if( m_pPersonalStore_Window )
			{
				return m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].m_usCurMaxProtect;
			}
		}
		break;
	case TMODE_TARGETINFO :
		{
			if( m_pTargetInfo_Window )
			{
				return m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].m_usCurMaxProtect;
			}
		}
		break;
	}
#endif
	return 0;
}

unsigned short _XWindow_DefaultTooltip::GetSmeltItem_TearInfo()
{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 용루값을 얻어온다
	switch( m_CurrentTooltipMode ) 
	{
	case TMODE_NPCTRADE_INVENITEM :
		{
			return g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usUsedProtect;
		}
		break;
	case TMODE_NPCTRADE_NPCITEM :
	case TMODE_DEFAULTITEM :
	case TMODE_CINFOITEM :
	case TMODE_REPAIRITEM :
	case TMODE_INCHANT :
	case TMODE_QUEST :
	case TMODE_NPCQUEST :
	case TMODE_GAMBLE :
	case TMODE_CASHITEM :
	case TMODE_LIFESKILLCOLLECTIONITEM :
	case TMODE_LIFESKILLRESOURCEITEM :
	case TMODE_NPCCOLLECTIONITEM :
	case TMODE_NPCRESOURCEITEM :
		return g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usUsedProtect;
		break;
	case TMODE_WAREHOUSE_WAREHOUSEITEM :
		return g_pLocalUser->m_WarehouseItemList[m_CurrentSlotNumber].m_usUsedProtect;		
		break;
	case TMODE_PCTRADE_TARGETITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_TargetTradeItem[m_CurrentSlotNumber].m_usUsedProtect;
			}
		}
		break;
	case TMODE_PCTRADE_SELFITEM :
		{
			if( m_pPCTradeWindow )
			{
				return m_pPCTradeWindow->m_SelfTradeItem[m_CurrentSlotNumber].m_usUsedProtect;
			}
		}
		break;
	case TMODE_PERSONALSTORE_TRADEITEM :
		{
			if( m_pPersonalStore_Window )
			{
				return m_pPersonalStore_Window->m_TradeItem[m_CurrentSlotNumber].m_usUsedProtect;
			}
		}
		break;
	case TMODE_TARGETINFO :
		{
			if( m_pTargetInfo_Window )
			{
				return m_pTargetInfo_Window->m_SelectedTargetInfo.getOnItems[m_CurrentSlotNumber].m_usUsedProtect;
			}
		}
		break;
	}
#endif

	return 0;
}