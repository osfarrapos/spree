// XWindow_MiniTooltip.cpp: implementation of the _XWindow_MiniTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_MiniTooltip.h"
#include "XCharacterState.h"
#include "XWindow_Info.h"
#include "Xwindow_CastleInfo.h"

#include "XWindow_PK_ListBox.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern _map_NPCBuffStr				g_NPCBuffStr;

_XWindow_MiniTooltip* g_pMiniTooltip_Window = NULL;

static int MT_ROW_INTERVAL = 16;
static int MT_LINE_INTERVAL = 5;


_XWindow_MiniTooltip::_XWindow_MiniTooltip()
{
	g_pMiniTooltip_Window	= this;

	m_CurrentMiniTooltipMode	= MTMODE_NONE;	
	m_MiniTooltipSize.cx		= 0;
	m_MiniTooltipSize.cy		= 0;
	m_CurrentDrawPosY			= 0;
	m_CurrentSlotNumber			= -1;
	m_ItemType					= -1;
	m_ItemId					= -1;
	m_TimeCount					= 0;

	for( int i = 0; i < 4; i++ )
		m_RoundEdgeImage[i] = NULL;

	m_pGroupIcon	= NULL;
}

_XWindow_MiniTooltip::~_XWindow_MiniTooltip()
{

}

void _XWindow_MiniTooltip::ResetMiniTooltipInfo()
{
	memset( m_CurrentMiniTooltipInfo.NameBuf, 0, sizeof(TCHAR)*128);
	memset( m_CurrentMiniTooltipInfo.LData_Buf, 0, sizeof(TCHAR)*512);	
	
	for( int i = 0; i < 12; i++ )
	{
		memset( m_CurrentMiniTooltipInfo.Data_Buf[i], 0, sizeof(TCHAR)*128);		
	}	
}

BOOL _XWindow_MiniTooltip::Initialize(void)
{
	SetWindowMoveMode(_XWMOVE_FIXED);
	for( int i = 0; i < 4; i++ )
		SAFE_DELETE( m_RoundEdgeImage[i] );

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_chat_quick.tga");
	
	for( i = 0; i < 4; i++ )
		m_RoundEdgeImage[i]= new _XImageStatic;

	m_RoundEdgeImage[0]->Create(0, 0, 2, 2, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[0]->SetClipRect(199, 216, 201, 218);
	m_RoundEdgeImage[1]->Create(0, 0, 2, 2, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[1]->SetClipRect(202, 216, 204, 218);
	m_RoundEdgeImage[2]->Create(0, 0, 2, 2, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[2]->SetClipRect(205, 216, 207, 218);
	m_RoundEdgeImage[3]->Create(0, 0, 2, 2, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[3]->SetClipRect(208, 216, 210, 218);

	ResetMiniTooltipInfo();

	m_pGroupIcon = new _XImageStatic;
	m_pGroupIcon->Create(0, 0, 15, 15, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_party.tga"));

	// Item 관련 이미지------------------------------------------------------------------------------=		

	return TRUE;
}

void _XWindow_MiniTooltip::DestroyWindow(void)
{
	for( int i = 0; i < 4; i++ )
		SAFE_DELETE(m_RoundEdgeImage[i]);

	SAFE_DELETE(m_pGroupIcon);
}

void _XWindow_MiniTooltip::Draw(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return;
	
 	if( g_bShowItemMallWindow ) return;
	
	_XDrawSolidBar(m_WindowPosition.x+2, m_WindowPosition.y+2, m_WindowPosition.x+m_MiniTooltipSize.cx-2, m_WindowPosition.y+m_MiniTooltipSize.cy-2, D3DCOLOR_ARGB(178,0,0,0) );
	
	g_pDefaultTooltip_Window->DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_MiniTooltipSize.cx, m_WindowPosition.y+m_MiniTooltipSize.cy);

	m_RoundEdgeImage[0]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + 1 );
	m_RoundEdgeImage[1]->Draw( m_WindowPosition.x + m_MiniTooltipSize.cx - 2, m_WindowPosition.y + 1 );
	m_RoundEdgeImage[2]->Draw( m_WindowPosition.x + 1, m_WindowPosition.y + m_MiniTooltipSize.cy - 2 );
	m_RoundEdgeImage[3]->Draw( m_WindowPosition.x + m_MiniTooltipSize.cx - 2, m_WindowPosition.y + m_MiniTooltipSize.cy - 2 );
	
	switch( m_CurrentMiniTooltipMode ) 
	{
	case MTMODE_ITEM :	
		DrawMiniItem();			
		break;
	case MTMODE_SKILL :
		DrawMiniSkill();			
		break;
	case MTMODE_BUFF_EXTERNAL :
		DrawMiniBuffExternal();
		break;
	case MTMODE_BUFF_INTERNAL :
		DrawMiniBuffInternal();			
		break;
	case MTMODE_BUFF_ITEM :			
		DrawMiniBuffItem();			
		break;
	case MTMODE_BUFF_SKILL :
		DrawMiniBuffSkill();
		break;
	case MTMODE_BUFF_EFFECT :
		DrawMiniBuffEffect();
		break;
	case MTMODE_BUFF_MONSTATS_SPELL :
		DrawMonstatsSpell();
		break;
	case MTMODE_BUFF_MONSTATS_EFFECT :
		DrawMonstatsEffect();
		break;
	case MTMODE_BUFF_MONSTATS_STATE :
		DrawMonstateState();
		break;
	case MTMODE_CHARINFO:
		DrawCharInfo();
		break;
	case MTMODE_CHARINFO_ABILITY:
		DrawCharInfoAbility();
		break;
	case MTMODE_CHARINFO_ABILITYSTEP :
		DrawCharInfoAbilityStep();
		break;
	case MTMODE_CHARINFO_INTOXICATION :
		DrawCharInfoIntoxication();
		break;
	case MTMODE_CHARINFO_FAME :
		DrawCharInfoFame();
		break;
	case MTMODE_PARTYINFO :
		DrawPartyInfo();
		break;
	case MTMODE_BUFF_INTOXICATION :
		DrawMiniBuffIntoxication();
		break;
	case MTMODE_BUFF_NICKNAME :
		DrawMiniBuffNickname();
		break;
	case MTMODE_BUFF_STUDYBOOK :
		DrawMiniBuffStudyBook();
		break;
	case MTMODE_BUFF_PEMODE :
		DrawMiniBuffPeMode();
		break;
	case MTMODE_ITEM_SHOUT :
		DrawMiniItemShout();
		break;
	case MTMODE_VIETNAMPAYMENTINFO :
		DrawVietnamPaymentMethod();
		break;
	case MTMODE_PAYMENTINFO :
		DrawPaymentInfo();	//양희왕 수정
		break;
	case MTMODE_PCRPREMIUM :
		DrawPCRPremium();
		break;
	case MTMODE_PK_RECORD :
		DrawPKRecord();
		break;
	case MTMODE_PK_TRACINGSERVICES: //Author : 양희왕 
		DrawPkTracingservices();
		break;
	case MTMODE_ITEM_ACCOUNT :
		DrawAccountItemUse();
		break;
	case MTMODE_VIEWGROUP_CASTLEINFO :
		DrawViewGroupCastleInfo();
		break;
	case MTMODE_CASTLEINFO :
		DrawCastleInfo();
		break;
	case MTMODE_NOTORIOUS : //Author : 양희왕 //breif : 평판
		DrawCharInfoNotorious();
		break;
	}
}

void _XWindow_MiniTooltip::DrawMiniItem()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	g_XBaseFont->SetColor( 0xFFFEFECC );
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL+MT_LINE_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx - 16, 1.0, 3 );		
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);				
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[1]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}

	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMiniSkill()
{
	// name
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	g_XBaseFont->SetColor( 0xFFFEFECC );
	m_CurrentDrawPosY = 12;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

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

	g_XBaseFont->SetColor(0xFFB4B4B4);
	if(skilllevel >= 11)
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MiniTooltipSize.cx - 10, m_WindowPosition.y + m_CurrentDrawPosY, 
								1.0f, _XFONT_ALIGNTYPE_RIGHT, "100%%" );			
	}
	else
	{
		exp = (FLOAT)g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].skillExp;
		if(exp > 0.0f)
		{
			gaugefactor = ((FLOAT)g_pLocalUser->m_SkillList[learnedindex].m_iSkillExp / exp) * 100;
			g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MiniTooltipSize.cx - 10, m_WindowPosition.y + m_CurrentDrawPosY, 
									1.0f, _XFONT_ALIGNTYPE_RIGHT, "%3.2f%%", gaugefactor);
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MiniTooltipSize.cx - 10, m_WindowPosition.y + m_CurrentDrawPosY, 
				1.0f, _XFONT_ALIGNTYPE_RIGHT, "0.00%%" );
		}
	}	

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	m_CurrentDrawPosY += 20;
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0] ) )
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0] );

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[1] ) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[1] );		
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[2] ) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[2], m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[2], m_MiniTooltipSize.cx-16 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[3] ) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[3] );
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[4]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[4] );		
	}
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[5]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx-16 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[6] ) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[6] );
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[7] ) )
	{
		g_XBaseFont->SetColor( 0xFFDE0E0E );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[7], m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[7], m_MiniTooltipSize.cx-16 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[8] ) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[8], m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[8], m_MiniTooltipSize.cx-16 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[9] ) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFFDE0E0E );
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[9], m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[9], m_MiniTooltipSize.cx-16 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}

	if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[10]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;		
		g_XBaseFont->SetColor( 0xFFDE0E0E );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[10] );
	}

	if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[11]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;		
		g_XBaseFont->SetColor( 0xFFDE0E0E );
		g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, 
			m_CurrentMiniTooltipInfo.Data_Buf[11], m_MiniTooltipSize.cx-16, _XFONT_ALIGNTYPE_LEFT);
		int length = g_XBaseFont->GetRowCount(m_CurrentMiniTooltipInfo.Data_Buf[11], m_MiniTooltipSize.cx-16);
		m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}

	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) ) 
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		m_CurrentDrawPosY += MT_LINE_INTERVAL;

		_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+m_MiniTooltipSize.cy-8, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+m_CurrentDrawPosY, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+m_MiniTooltipSize.cy-8, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
		
		m_CurrentDrawPosY += 2;
		g_XBaseFont->SetColor( 0xFFE7B329 );
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );		
	}
	
	g_XBaseFont->Flush();	
}

void _XWindow_MiniTooltip::DrawMiniBuffExternal()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	g_XBaseFont->SetColor( 0xFFFEFECC );
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL+MT_LINE_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}	
	
	g_XBaseFont->Flush();
}
	
void _XWindow_MiniTooltip::DrawMiniBuffInternal()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	g_XBaseFont->SetColor( 0xFFFEFECC );
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL+MT_LINE_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}

	g_XBaseFont->Flush();
}
	
void _XWindow_MiniTooltip::DrawMiniBuffItem()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[3]) )
	{
		g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,255,11,0 ) );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[3], m_MiniTooltipSize.cx - 16, 1.0, 3 );		
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[3], m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);				
	}	
#ifdef _XTS_FAME
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[6] ) )
	{
		g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,255,11,0 ) );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[6], m_MiniTooltipSize.cx - 16, 1.0, 3 );		
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[6], m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}
#endif
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[4]) )
	{
		g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,255,11,0 ) );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx - 16, 1.0, 3 );		
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);				
	}
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[5]) )
	{
		g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,255,11,0 ) );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx - 16, 1.0, 3 );		
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);				
	}
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[3]) )
			m_CurrentDrawPosY += MT_LINE_INTERVAL;
		m_CurrentDrawPosY += MT_ROW_INTERVAL;

		g_XBaseFont->SetColor( 0xFFFEFECC );		
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx - 16, 1.0, 3 );		
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[1]) )
	{		
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFFFEFECC );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[2]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFFFEFECC );		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[2]);
	}
	g_XBaseFont->Flush();
}
	
void _XWindow_MiniTooltip::DrawMiniBuffSkill()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[1]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFFFE9F41 );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}

	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		
		g_XBaseFont->SetColor( 0xFFFEFECC );		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}

	g_XBaseFont->Flush();
}
	
void _XWindow_MiniTooltip::DrawMiniBuffEffect()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[1]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFFFE9F41 );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}

	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->SetColor( 0xFFFEFECC );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMonstatsSpell()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;

	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMonstatsEffect()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;
	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMonstateState()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;
	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawCharInfo()
{
	g_XBaseFont->SetColor(0xFFFFFFFF);
	m_CurrentDrawPosY = 12;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_EXTERNAL) );	// _T("외상")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_INTERNAL) );	// _T("내상")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_ATTACKDAMAGE) );	// _T("공격력")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_ATTACKRATING) );	// _T("공격성공치")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_DEPENSE) );	// _T("방어력")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_AVOID) );	// _T("회피력")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEATTACK) );	// _T("기공공격력")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEATTACKRATE) );	// _T("기공성공치")
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_2635) );	// _T("기공방어력")	
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEAVOID) );	// _T("기공회피력")

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3636) );//"음의저항력"
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3637) );//"양의저항력"
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3638) );//혼의저항력
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3648));//신의저항력
#endif

	m_CurrentDrawPosY = 12;

	int baseposition = 80;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		baseposition = 130;
	}

	g_XBaseFont->SetColor(0xFFED1B23);
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[7] );
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	g_XBaseFont->SetColor(0xFF00BFF2);
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[8] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	if(g_pLocalUser->m_P_AttackDamage.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_P_AttackDamage.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;	
	if(g_pLocalUser->m_P_AttackRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_P_AttackRate.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[1] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;	
	if(g_pLocalUser->m_P_Defence.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_P_Defence.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[2] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;	
	if(g_pLocalUser->m_P_AvoidRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_P_AvoidRate.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[3] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;	
	if(g_pLocalUser->m_M_AttackDamage.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_AttackDamage.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[4] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;	
	if(g_pLocalUser->m_M_AttackRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_AttackRate.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[5] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;	
	if(g_pLocalUser->m_M_Defence.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_Defence.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[9] );

	m_CurrentDrawPosY += MT_ROW_INTERVAL;	
	if(g_pLocalUser->m_M_AvoidRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_AvoidRate.Get_bBuff())
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC4C4C2 );
	}
	g_XBaseFont->PutsAlign(m_WindowPosition.x+baseposition, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[6] );	

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI

	// 음의 저항력
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	if(g_pLocalUser->m_ResistMinus >= 0)
	{
		g_XBaseFont->SetColor( 0xFFFC832A );
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFAA3232 );
	}
	g_XBaseFont->Print(m_WindowPosition.x+baseposition, m_WindowPosition.y+m_CurrentDrawPosY, 1.0f, "%d", g_pLocalUser->m_ResistMinus);

	// 양의 저항력
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	if(g_pLocalUser->m_ResistPlus >= 0)
	{
		g_XBaseFont->SetColor(0xFFFC832A);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFAA3232 );
	}
	g_XBaseFont->Print(m_WindowPosition.x+baseposition, m_WindowPosition.y+m_CurrentDrawPosY, 1.0f, "%d", g_pLocalUser->m_ResistPlus);

	// 혼의 저항력
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	if(g_pLocalUser->m_ResistHon >= 0)
	{
		g_XBaseFont->SetColor(0xFFFC832A);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFAA3232 );
	}
	g_XBaseFont->Print(m_WindowPosition.x+baseposition, m_WindowPosition.y+m_CurrentDrawPosY, 1.0f, "%d", g_pLocalUser->m_ResistHon);

	// 신의 저항력
	m_CurrentDrawPosY += MT_ROW_INTERVAL;
	if(g_pLocalUser->m_ResistSin >= 0)
	{
		g_XBaseFont->SetColor(0xFFFC832A);
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFAA3232 );
	}
	g_XBaseFont->Print(m_WindowPosition.x+baseposition, m_WindowPosition.y+m_CurrentDrawPosY, 1.0f, "%d", g_pLocalUser->m_ResistSin);

#endif

	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawCharInfoAbility()
{
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
	{
		g_XBaseFont->SetColor( 0xFFFFF200 );
		m_CurrentDrawPosY = 12;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) ) 
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += 20;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );		
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawCharInfoAbilityStep()
{
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
	{
		g_XBaseFont->SetColor( 0xFFFFF200 );
		m_CurrentDrawPosY = 12;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) ) 
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += 25;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );		
	}
	
	m_CurrentDrawPosY += MT_ROW_INTERVAL+8;
	for( int i = 0; i < g_pLocalUser->m_ElixirInfo.Data[m_ItemId].__Grade; i++ )
	{
		if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[i]) )
		{
			m_CurrentDrawPosY += MT_ROW_INTERVAL;
			if( g_pLocalUser->m_ElixirInfo.Data[m_ItemId].__Data[i] > 0 )
				g_XBaseFont->SetColor( 0xFFFFF200 );
			else 
				g_XBaseFont->SetColor( 0xFFFF2200 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[i]);		
		}
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawCharInfoFame()
{
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	m_CurrentDrawPosY = 12;
	
	if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+15, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}	

	if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[1]) )
	{
		g_XBaseFont->SetColor(0xFFFFF200);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+115, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_RIGHT, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}
	
	g_XBaseFont->Flush();
}
void _XWindow_MiniTooltip::DrawCharInfoIntoxication()
{
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
	{
		g_XBaseFont->SetColor( 0xFFFFF200 );
		m_CurrentDrawPosY = 12;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) ) 
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += 20;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );		
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL*length;
	}

	if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		g_XBaseFont->SetColor(0xFFFFFF00);
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx - 16, 1.0, 3 );		
	}
	
	g_XBaseFont->Flush();	
}

void _XWindow_MiniTooltip::DrawPartyInfo()
{
	//_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	//_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );
	
#ifdef _XTS_PARTYMOREINFO
	
	/* 동행창 툴팁을 2줄에서 3줄로 바꾸기 */ 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0] ) ) 
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,243,110,53) );
			m_CurrentDrawPosY = 5;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+110, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentMiniTooltipInfo.Data_Buf[0]);
		}
		
		if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			m_CurrentDrawPosY += 15;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+110, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentMiniTooltipInfo.NameBuf);
		}
	}
	else
	{
		if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0] ) ) 
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,243,110,53) );
			m_CurrentDrawPosY = 5;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+45, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentMiniTooltipInfo.Data_Buf[0]);
		}
		
		if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			m_CurrentDrawPosY = 5;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+135, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentMiniTooltipInfo.NameBuf);
		}
	}
#else
	if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
	{
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		m_CurrentDrawPosY = 5;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+110, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentMiniTooltipInfo.NameBuf);
	}
#endif
	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) ) 
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += 15;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+110, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, m_CurrentMiniTooltipInfo.LData_Buf);		
	}

#ifdef _XDEF_PARTY_NOTAVAILABLE

	// 거리 이탈
	if(m_ItemId != 0 || m_TimeCount != 0)
	{
		m_CurrentDrawPosY += 15;
	}

	if(strlen(m_CurrentMiniTooltipInfo.Data_Buf[1]))
	{
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 36, 147, 147));
		g_XBaseFont->Puts(m_WindowPosition.x+17, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}

	// 자리 비움
	if(strlen(m_CurrentMiniTooltipInfo.Data_Buf[2]))
	{
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 36, 147, 147));
		g_XBaseFont->Puts(m_WindowPosition.x+100, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[2]);
	}

#endif


	if( m_ItemType != 0 )//빈사 혼수 
	{
		g_XBaseFont->SetColor(_XSC_WARNING);
		m_CurrentDrawPosY += 15;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+90, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2191));	
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMiniBuffIntoxication()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;

	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMiniBuffNickname()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		g_XBaseFont->SetColor( 0xFF96C832 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}

	for( int i = 1; i < 6; i++ )
	{
		if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[i]) )
		{
			g_XBaseFont->SetColor( 0xFFFEFECC );
			m_CurrentDrawPosY += MT_ROW_INTERVAL;
			g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[i], m_MiniTooltipSize.cx - 16, 1.0, 3 );
			int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[i], m_MiniTooltipSize.cx-16 );
			if( length > 0 )
				m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
		}
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMiniBuffStudyBook()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	m_CurrentDrawPosY += MT_LINE_INTERVAL;

	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		int length = g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx - 16, 1.0, 3 );
		m_CurrentDrawPosY += MT_ROW_INTERVAL*length;
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		g_XBaseFont->SetColor( 0xFFFEFECC );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMiniBuffPeMode()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);

	g_XBaseFont->SetColor( 0xFFFEFECC );
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0] ) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx - 16, 1.0, 3 );
		int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
		if( length > 0 )
			m_CurrentDrawPosY += MT_ROW_INTERVAL*(length-1);
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[1]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawMiniItemShout()
{
	m_CurrentDrawPosY = 9;
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
	
	g_XBaseFont->SetColor( 0xFFFEFECC );
	
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0]) )
	{
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}
	
	g_XBaseFont->Flush();
}

BOOL _XWindow_MiniTooltip::Process(_XGUIObject*& pfocusobject)
{
	return TRUE;
}

void _XWindow_MiniTooltip::ShowWindow(BOOL show)
{
	if(show)
	{
		m_pMyManager->SetTopWindow(this);		
	}
	else
	{
		m_CurrentMiniTooltipMode = MTMODE_NONE;
	}	
	_XWindow::ShowWindow(show);
}

BOOL _XWindow_MiniTooltip::SetTooltipMode( _MINITOOLTIP_MODE mode, int slotnumber, int type, int id, DWORD Time, int DrawX, int DrawY )
{
	ResetMiniTooltipInfo();

	m_CurrentMiniTooltipMode = mode;
	m_CurrentSlotNumber = slotnumber;
	m_ItemType = type;
	m_ItemId = id;
	m_TimeCount = Time;

	switch( mode ) 
	{
	case MTMODE_ITEM:
		{
			m_MiniTooltipSize.cx = 183;
			m_MiniTooltipSize.cy = 12;
		}
		break;
	case MTMODE_SKILL:
		{
			m_MiniTooltipSize.cx = 258;
			m_MiniTooltipSize.cy = 34;
		}
		break;
	case MTMODE_BUFF_EXTERNAL:		
	case MTMODE_BUFF_INTERNAL:		
	case MTMODE_BUFF_ITEM:
	case MTMODE_BUFF_SKILL:
	case MTMODE_BUFF_EFFECT:
	case MTMODE_BUFF_MONSTATS_SPELL:
	case MTMODE_BUFF_MONSTATS_EFFECT:
	case MTMODE_BUFF_MONSTATS_STATE:
	case MTMODE_BUFF_INTOXICATION:
	case MTMODE_BUFF_NICKNAME:
	case MTMODE_BUFF_STUDYBOOK:	
	case MTMODE_BUFF_PEMODE:	
	case MTMODE_ITEM_SHOUT :
		{
			m_MiniTooltipSize.cx = 183;
			m_MiniTooltipSize.cy = 12;						
		}
		break;
	case MTMODE_CHARINFO:
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
				m_MiniTooltipSize.cx = 194;
				m_MiniTooltipSize.cy = 244;
#else
				m_MiniTooltipSize.cx = 194;
				m_MiniTooltipSize.cy = 180;
#endif
			}
			else
			{
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
				m_MiniTooltipSize.cx = 147;
				m_MiniTooltipSize.cy = 244;
#else
				m_MiniTooltipSize.cx = 147;
				m_MiniTooltipSize.cy = 180;
#endif
			}
		}
		break;
	case MTMODE_CHARINFO_ABILITY :
	case MTMODE_CHARINFO_INTOXICATION :
		{
			m_MiniTooltipSize.cx = 189;
			m_MiniTooltipSize.cy = 35;
		}
		break;
	case MTMODE_CHARINFO_ABILITYSTEP :
		{
			m_MiniTooltipSize.cx = 190;
			m_MiniTooltipSize.cy = 80;
		}
		break;
	case MTMODE_CHARINFO_FAME :
		{
#ifdef _XTS_FAME
			m_MiniTooltipSize.cx = 130;
			m_MiniTooltipSize.cy = 34;

			int rank = -1;
			switch(m_ItemId) 
			{
			case 0 :	// 명성
				rank = g_FameStr->GetFameIndex(g_pLocalUser->m_CharacterInfo.Get_honor());				
				break;
			case 1 :	// 악명 
				rank = g_FameStr->GetInfamyIndex(g_pLocalUser->m_CharacterInfo.Get_Notoriety());
				break;
			}
			if( rank == -1 ) 
			{
				ShowWindow(FALSE);
				return FALSE;
			}
#endif
		}
		break;
	case MTMODE_PK_RECORD:
		{
			m_MiniTooltipSize.cx = 183;
			m_MiniTooltipSize.cy = 48;
		}
		break;
	case MTMODE_PK_TRACINGSERVICES: //Author : 양희왕
		{
			m_MiniTooltipSize.cx = 191;
			m_MiniTooltipSize.cy = 142;
		}
		break;
	case MTMODE_PARTYINFO :
		{
			m_MiniTooltipSize.cx = 180;

			/* 동행창 툴팁을 2줄에서 3줄로 바꾸기 */ 
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
                m_MiniTooltipSize.cx = 220;
				if( type == 0 )
				{
#ifdef _XDEF_PARTY_NOTAVAILABLE
					if(m_ItemId != 0 || m_TimeCount != 0)
					{
						m_MiniTooltipSize.cy = 71;
					}
					else
					{
						m_MiniTooltipSize.cy = 51;
					}

#else
					m_MiniTooltipSize.cy = 51;
#endif
				}
				else
				{
#ifdef _XDEF_PARTY_NOTAVAILABLE
					if(m_ItemId != 0 || m_TimeCount != 0)
					{
						m_MiniTooltipSize.cy = 91;
					}
					else
					{
						m_MiniTooltipSize.cy = 71;
					}

#else
					m_MiniTooltipSize.cy = 71;
#endif
				}
			}
			else
			{
				if( type == 0 )
				{
#ifdef _XDEF_PARTY_NOTAVAILABLE
					if(m_ItemId != 0 || m_TimeCount != 0)
					{
						m_MiniTooltipSize.cy = 51;
					}
					else
					{
						m_MiniTooltipSize.cy = 36;
					}
#else
					m_MiniTooltipSize.cy = 36;
#endif
				}
				else
				{
#ifdef _XDEF_PARTY_NOTAVAILABLE
					if(m_ItemId != 0 || m_TimeCount != 0)
					{
						m_MiniTooltipSize.cy = 71;
					}
					else
					{
						m_MiniTooltipSize.cy = 51;
					}
#else
					m_MiniTooltipSize.cy = 51;
#endif

				}
			}
		}
		break;
	case MTMODE_ITEM_ACCOUNT :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 1 || g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 2)
				{
					m_MiniTooltipSize.cx = 300;
					m_MiniTooltipSize.cy = 62;
				}
				else if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 3)
				{
					m_MiniTooltipSize.cx = 300;
					m_MiniTooltipSize.cy = 80;
				}
				else if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 4 || g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 5 )					
				{
					m_MiniTooltipSize.cx = 300;
					
					#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
					m_MiniTooltipSize.cy = 186;
					m_MiniTooltipSize.cy += 42; // 이건.. 새로 추가된 새줄.. 기공 방어력 공격력 공격 명중률
					#else
					m_MiniTooltipSize.cy = 172 + 42;
					//m_MiniTooltipSize.cy = 172;
					#endif
				}
				else if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 6 ) //Author : 양희왕 //breif : 베트남 , 구룡천하 추가
				{
					m_MiniTooltipSize.cx = 300;
					#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
					m_MiniTooltipSize.cy = 186;
					#else
					m_MiniTooltipSize.cy = 172;
					#endif
				}
				
				#ifdef _XDWDEBUG
				m_MiniTooltipSize.cy+=10;
				#endif

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE ) //Author : 양희왕 //breif : 베트남은 좀더 길다..
				{
					m_MiniTooltipSize.cx = 320;
				}
			}
			else
			{
				if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 1 || g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 2)
				{
					m_MiniTooltipSize.cx = 193;
					m_MiniTooltipSize.cy = 62;
				}
				else if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 3)
				{
					m_MiniTooltipSize.cx = 193;
					m_MiniTooltipSize.cy = 80;
				}
				else if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 4 || g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 5
                    || g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 6)
				{
					m_MiniTooltipSize.cx = 212;
					m_MiniTooltipSize.cy = 172;
                #if defined(_XTAIWANESE) //대만은 한줄 늘어남 cx 를 바꾸면 구분선 길이도 증가해줘야한다.. 한글자 가로 약 12
                    m_MiniTooltipSize.cy += 14;
                #endif
				}

		    #ifdef _XDWDEBUG
				m_MiniTooltipSize.cy+=10;
			#endif
			}
		}
		break;
	case MTMODE_VIETNAMPAYMENTINFO :
		{
			m_MiniTooltipSize.cx = 300;

			switch( g_LocalUserBillingInformations.BillMethod )
			{
			default:
			case _VNBILLING_BILLMETHOD_FREEOFCHARGE					:
			case _VNBILLING_BILLMETHOD_FREEOFCHARGE_EVENT			:
				m_MiniTooltipSize.cy = 48;
				break;
			case _VNBILLING_BILLMETHOD_BASIC_FIXEDAMOUNTSYSTEM		:
				m_MiniTooltipSize.cy = 104;
				break;
			case _VNBILLING_BILLMETHOD_BASIC_FIXEDQUANTITYSYSTEM	:		
				m_MiniTooltipSize.cy = 104;
				break;
			case _VNBILLING_BILLMETHOD_PREMIUM_FIXEDAMOUNTSYSTEM	:		
				m_MiniTooltipSize.cy = 160;
				break;
			case _VNBILLING_BILLMETHOD_PREMIUM_FIXEDQUANTITYSYSTEM	:		
				m_MiniTooltipSize.cy = 160;
				break;
			}
		}
		break;
	case MTMODE_PAYMENTINFO :
		{
//Author : 양희왕
// 베트남 셧다운 
#ifdef _XDEF_SHUTDOWN_20061027	
			int playelapsedtime = 0;
			
			if( g_PlayLeftTime != 0 )
				playelapsedtime = g_PlayLeftTime - (g_LocalSystemTime - g_PlayTime);

	
			if( playelapsedtime >= 7200000 ) // 5시간 - 2시간까지
			{
	#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
				if(g_pJinStatus_Window)
				{
					if(g_pJinStatus_Window->GetSDSState() == 1)
					{
						m_MiniTooltipSize.cx = 300;
						m_MiniTooltipSize.cy = 58+54;
					}
					else
					{
						m_MiniTooltipSize.cx = 300;
						m_MiniTooltipSize.cy = 58;
					}
				}
	#else
				m_MiniTooltipSize.cx = 300;
				m_MiniTooltipSize.cy = 58;
	#endif
			}
			else if( playelapsedtime < 7200000 )//&& playelapsedtime > 0  )// 플레이 남은 시간이 2시간 미만인 경우 페널티 시작
			{
				m_MiniTooltipSize.cx = 300;
				m_MiniTooltipSize.cy = 58+70;
			}
#endif
//Last Updated : 07/03/14
		
		}
		break;
	case MTMODE_PCRPREMIUM :
			{
				// pc방
				m_MiniTooltipSize.cx = 192;
				m_MiniTooltipSize.cy = 100-32; //날짜 없애면서 이것도 변경했음 //Author : 양희왕
			}
			break;
	case MTMODE_VIEWGROUP_CASTLEINFO :
		{
			m_MiniTooltipSize.cx = 147;
			m_MiniTooltipSize.cy = 60;
		}
		break;
	case MTMODE_CASTLEINFO :
		{
			m_MiniTooltipSize.cx = 192;
			m_MiniTooltipSize.cy = 84;
		}
		break;
	case MTMODE_NOTORIOUS :
		{
			m_MiniTooltipSize.cx = 192;
			m_MiniTooltipSize.cy = 32;
		}
		break;
	}
	
	if( SetTooltipInfo() )
	{
		POINT position;		
		RECT slotposition;	

		SetWindowSize(m_MiniTooltipSize);
		
		switch( m_CurrentMiniTooltipMode ) 
		{
		case MTMODE_ITEM :
		case MTMODE_SKILL :
			{
				g_pQuickSlot_Window->GetSlotPosition( m_CurrentSlotNumber, slotposition );
				POINT pos = g_pQuickSlot_Window->GetWindowPos();
				SIZE size = g_pQuickSlot_Window->GetWindowSize();		
				
				position.x = slotposition.left;
				position.y = slotposition.top+48;
				
				if( slotposition.left + m_MiniTooltipSize.cx >= pos.x + size.cx )
				{
					position.x = slotposition.left - m_MiniTooltipSize.cx + 32;				
				}
				if( slotposition.top + 48 + size.cy + m_MiniTooltipSize.cy >= gnHeight )
				{
					position.y = slotposition.top -6 - m_MiniTooltipSize.cy;
				}
			}
			break;
		case MTMODE_BUFF_EXTERNAL :
		case MTMODE_BUFF_INTERNAL :		
		case MTMODE_BUFF_ITEM :
		case MTMODE_BUFF_SKILL :
		case MTMODE_BUFF_EFFECT :		
		case MTMODE_BUFF_MONSTATS_SPELL :
		case MTMODE_BUFF_MONSTATS_EFFECT : 
		case MTMODE_BUFF_MONSTATS_STATE :
		case MTMODE_BUFF_INTOXICATION :
		case MTMODE_BUFF_NICKNAME :
		case MTMODE_BUFF_STUDYBOOK :
		case MTMODE_BUFF_PEMODE:
		case MTMODE_ITEM_SHOUT :
			{
				position.x = DrawX+32;
				position.y = DrawY+32;
			}
			break;
		case MTMODE_CHARINFO :
			{
				#ifdef _ACCLAIM_RUBICONADSYSTEM			
				if( g_ViewRubiconAD )
				{
					position.x = 81;
					position.y = 72 + g_RubiconADWindowSize.cy;
				}					
				else
				{
					position.x = 81;
					position.y = 72;
				}
				#else
					position.x = 81;
					position.y = 72;
				#endif
			}
			break;
		case MTMODE_CHARINFO_ABILITY :
		case MTMODE_CHARINFO_ABILITYSTEP :
		case MTMODE_CHARINFO_INTOXICATION :
		case MTMODE_CHARINFO_FAME :		
			{
				position.x = 388;
				position.y = 388;
			}
			break;
		case MTMODE_PK_RECORD :
			{
				position.x = 388;
				position.y = 293;
			}
			break;
		case MTMODE_PK_TRACINGSERVICES: //Author :양희왕
			{
				POINT Pos;
				XWindow_PK_ListBox* pPk_TracingServicesWindow = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
				if( pPk_TracingServicesWindow )
				{
					Pos = pPk_TracingServicesWindow->GetWindowPos();

					if( Pos.x - m_MiniTooltipSize.cx - 10 > 0 )
					{
						position.x = Pos.x - m_MiniTooltipSize.cx - 10;
					}
					else
					{
						SIZE Size = pPk_TracingServicesWindow->GetWindowSize();
						position.x = Pos.x + Size.cx + 10;
					}
					position.y = Pos.y;
				}
			}
			break;
		case MTMODE_PARTYINFO :
			{
				if( g_pLocalUser->m_PartyInfo[slotnumber].isLeader )
				{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
					position.x = 179 + 36;
#else					
					position.x = 179;
#endif
					#ifdef _ACCLAIM_RUBICONADSYSTEM	
					if( g_ViewRubiconAD )
						position.y = 114 + g_RubiconADWindowSize.cy;
					else
						position.y = 114;
					#else
					position.y = 114;
					#endif
				}
				else
				{
					int count = 0;
					for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
					{
						if( !g_pLocalUser->m_PartyInfo[i].isLeader )
						{
							if( g_pLocalUser->m_PartyInfo[i].playeruniqueid != 0 )
							{
								count++;
								if( i == slotnumber )
									break;
							}
							
						}
					}
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
					position.x = 160 + 36;
#else					
					position.x = 160;
#endif
					#ifdef _ACCLAIM_RUBICONADSYSTEM	
					if( g_ViewRubiconAD )
						position.y = 170+((count-1)*36 ) + g_RubiconADWindowSize.cy;
					else
						position.y = 170+((count-1)*36 );
					#else
						position.y = 170+((count-1)*36 );
					#endif
				}
			}
			break;
		case MTMODE_ITEM_ACCOUNT :
			{
				position.x = 75 + 156;
				position.y = 73;
			}
			break;
		case MTMODE_VIETNAMPAYMENTINFO :
			{
				position.x = 75 + 156;
				position.y = 93;
			}
			break;
		case MTMODE_PAYMENTINFO :
			{
				position.x = 75 + 156;
				position.y = 73;
			}
			break;
		case MTMODE_PCRPREMIUM :
			{
				position.x = 75 + 156;
				position.y = 73;
			}
			break;
		case MTMODE_VIEWGROUP_CASTLEINFO :
			{
				position.x = 327;
				position.y = 217;
			}
			break;
		case MTMODE_CASTLEINFO :
			{
				_XWindow_Info* pInfo_Window = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
				if(pInfo_Window)
				{
					POINT pos = pInfo_Window->GetWindowPos();
					position.x = pos.x - 192;
					position.y = pos.y + 32;
				}
			}
			break;
		case MTMODE_NOTORIOUS : //Author: 양희왕 //breif : 평판 시스템
			{
				position.x = 81;
				position.y = 72;
			}
			break;
		}
		
		MoveWindow( position.x, position.y  );
	}	
	else 
		return FALSE;

	return TRUE;
}

BOOL _XWindow_MiniTooltip::SetTooltipInfo()
{
	if( m_ItemId < 0 ) return FALSE;
	
	switch( m_CurrentMiniTooltipMode ) 
	{
	case MTMODE_ITEM:
		{
			switch(m_ItemType) 
			{
			case _XGI_FC_BOOK :
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_BOOK] )
						break;

					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );
				}
				break;
			case _XGI_FC_POTION :
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_POTION] )
						break;

					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );
					
					m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
					switch(g_PotionItemProperty[m_ItemId].cSecondType) 
					{
					case _XGI_SC_POTION_HERT_IN:		// 내상
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_INTERNALDECREASE), g_PotionItemProperty[m_ItemId].sApplyValue );	// "내상 %d 감소"
								break;
							case 1:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_INTERNALDECREASERATE), g_PotionItemProperty[m_ItemId].sApplyValue );	// "내상 %d%% 감소"
								break;
							}
						}				
						break;
					case _XGI_SC_POTION_HERT_OUT:		// 외상
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_EXTERNALDECREASE), g_PotionItemProperty[m_ItemId].sApplyValue );	// "외상 %d 감소"
								break;
							case 1:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_EXTERNALDECREASERATE), g_PotionItemProperty[m_ItemId].sApplyValue );	// "외상 %d%% 감소"
								break;
							}
						}				
						break;
					case _XGI_SC_POTION_FATIGUE:		// 생명회복 영약
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2042), g_PotionItemProperty[m_ItemId].sApplyValue );
								break;
							case 1:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2043), g_PotionItemProperty[m_ItemId].sApplyValue );	// "피로 %d%% 감소"
								break;
							}
						}				
						break;
					case _XGI_SC_POTION_NOTFIXED:		// 내공회복 영약
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2045), g_PotionItemProperty[m_ItemId].sApplyValue );	// "피로 %d 감소"
								break;
							case 1:
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2046), g_PotionItemProperty[m_ItemId].sApplyValue );	// "피로 %d%% 감소"
								break;
							}
						}				
						break;
					case _XGI_SC_POTION_FOOD:		// 음식
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								{
									sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2047), g_PotionItemProperty[m_ItemId].sApplyValue );	// "생명 자연회복 %d 증가"
								}
								
								break;
							case 1:
								{
									sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2048), g_PotionItemProperty[m_ItemId].sApplyValue );
								}								
								break;
							}
						}
						break;
					case _XGI_SC_POTION_EVENT:		// 신약
					case _XGI_SC_POTION_EVENT2 :
						{
							map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_PotionItemProperty[m_ItemId].sEffectId);
							if(iter_table != g_CharacterStateTable.end())
							{
								if( strlen(g_CharacterStateTable[g_PotionItemProperty[m_ItemId].sEffectId]->concept) )
								{
									sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], g_CharacterStateTable[g_PotionItemProperty[m_ItemId].sEffectId]->concept, 
										g_PotionItemProperty[m_ItemId].sApplyValue );
								}
							}				
						}
						break;
					case _XGI_SC_POTION_HWATA :
						{
							strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2051) );							
						}
						break;
					}

					int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
					if( length > 0 )
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
					
					if( g_PotionItemProperty[m_ItemId].sCooldown > 0)
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;											

#ifdef _XDEF_EXPANSION_POTION_070709_KUKURI
						if( g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_MASK  || 
							g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_EVENT ||
							g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_ANTI  ||
							g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_FOOD  ||
							g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_EVENT2 ) //Author : 양희왕 //breif : 신약 아이콘 추가
#else
						if( g_PotionItemProperty[m_ItemId].cSecondType == _XGI_SC_POTION_MASK )
#endif
						{
							if(g_PotionItemProperty[m_ItemId].sCooldown < 60)	
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_NEW_2642), g_PotionItemProperty[m_ItemId].sCooldown );	//"지속시간 %d초"
							else
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_NEW_2643), g_PotionItemProperty[m_ItemId].sCooldown/60);//"지속시간 %d분"
						}
						else
						{
							if(g_PotionItemProperty[m_ItemId].sCooldown < 60)
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2052), g_PotionItemProperty[m_ItemId].sCooldown );
							else
								sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2053), g_PotionItemProperty[m_ItemId].sCooldown/60);
						}
					}
				}
				break;
			case _XGI_FC_CONSUMING :
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_CONSUMING] )
						break;

					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );					

					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_ConsumingItemProperty[m_ItemId].usSEID);
					if(iter_table != g_CharacterStateTable.end())
					{
						if( strlen(g_CharacterStateTable[g_ConsumingItemProperty[m_ItemId].usSEID]->concept) )
						{
							sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], g_CharacterStateTable[g_ConsumingItemProperty[m_ItemId].usSEID]->concept, 
								g_ConsumingItemProperty[m_ItemId].usTime );
							
							int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						}						
					}
				}
				break;
			case _XGI_FC_ELIXIR :
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_ELIXIR] )
						break;

					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );										
				}
				break;
			case _XGI_FC_LIFE :
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_LIFE] )
						break;

					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );
				}
				break;
			}									
		}
		break;
	case MTMODE_SKILL:
		{
			if( _XSkillItem::FindSkillProperty(m_ItemId) )
			{
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_SkillProperty[m_ItemId]->skillName );
				
				strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], g_pSkillUsefulTypeName[g_SkillProperty[m_ItemId]->usefulType-1] );	// 기술
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				
				switch(g_SkillProperty[m_ItemId]->weaponCType)
				{
				case _XGI_SC_WEAPON_SHORT :
					{
						if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SWORD)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SWORD));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_KNIFE)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_KNIFE));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_CLUB)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CLUB));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_HIT)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIT));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_FIST)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FIST));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SECRET)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SECRETWEAPON));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_AXE)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_AXE));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_RING)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_RING));	// "쌍환"
					}
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_GLAIVE)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GLAIVE));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SPEAR)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SPEAR));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_STAFF)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_STAFF));
					}
					break;
				case _XGI_SC_WEAPON_SOFT :
					{
						if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_STEELWHIP)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FLAIL));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_HAMMER)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_HAMMER));	// "추술"
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_CHAIN)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CHAIN));		// "사술"
					}
					break;
				case _XGI_SC_WEAPON_HIDDEN :
					{
						if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_DART)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DART));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_DAGGER)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DAGGER));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_JAVELIN)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_PROJECTILE));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_NEEDLE)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NEEDLE));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_ARROWLAUNCHER)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALLOWLAUCHER));
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_MINE)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_BOMB));
					}
					break;
				case _XGI_SC_WEAPON_MUSICAL :
					{
						if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_RYUNG)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_RYUNG));	// "령"
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_GO)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_GO));	// "고"
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SO)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_SO));	// "소"
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_KUEM)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_KUEM));	// "금"
						else if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_JUCK)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_JUCK));	// "적"
					}
				case _XGI_SC_WEAPON_SPECIAL :
					{
						if(g_SkillProperty[m_ItemId]->weaponSType == _XGI_TC_WEAPON_SERING)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_SERING));	// "륜"
					}
				case -1 :
					{
						if(g_SkillProperty[m_ItemId]->weaponSType == 0)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALLWEAPON));
						else if(g_SkillProperty[m_ItemId]->weaponSType == -1)
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NOWEAPON));
					}
					break;
				}
				if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[1]) )
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				
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
						strcpy(ferastring,  _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_MU));	// _T("무")
					}
					break;
				}
				
				sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], "%s (%s)", g_SkillConceptProperty[g_SkillProperty[m_ItemId]->clanType].stype_name[g_SkillProperty[m_ItemId]->skillSType], ferastring );
				int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[2], m_MiniTooltipSize.cx-16 );
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
				
				int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_ItemId, TRUE);

				int skilllevel = 0;
				if(learnedindex == _XDEF_MAX_LEARNSKILL)
				{	
					skilllevel = g_pLocalUser->m_EventSkillList.m_cSelectedSkillLevel;
				}
				else
				{
					skilllevel = g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel;
				}
				
				if( _XSkillItem::GetCostForce(m_ItemId, skilllevel) > 0 )
				{
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					sprintf( m_CurrentMiniTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_FORCE), _XSkillItem::GetCostForce(m_ItemId, skilllevel)/*g_SkillProperty[m_ItemId]->detailInfo[skilllevel].costForce*/ );	// "내공 %d"
				}
				
				if( _XSkillItem::GetCasting(m_ItemId, skilllevel) > 0 )
				{
					float realcastingtime = _XSkillItem::GetCasting(m_ItemId, skilllevel) % 60000;
					if( realcastingtime == 0 )		// 분 (으)로 표시
					{
						realcastingtime = (float)_XSkillItem::GetCasting(m_ItemId, skilllevel) / 60000.0f;
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CASTINGTIMEMIN), realcastingtime );	// "시전시간 %.0f분"
					}
					else						// 초 (으)로 표시
					{
						realcastingtime = (float)_XSkillItem::GetCasting(m_ItemId, skilllevel) / 1000.0f;
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CASTINGTIMESEC), realcastingtime );	// "시전시간 %.1f초"
					}
				}	
				else
				{
					strcpy( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CASTINGTIMEDIRECT) );	// "즉시 시전"
				}
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				
				if( _XSkillItem::GetCoolDown(m_ItemId, skilllevel) > 0 )
				{
					float realcooldown = _XSkillItem::GetCoolDown(m_ItemId, skilllevel) % 60000;
					if( realcooldown == 0 )
					{
						realcooldown = (float)_XSkillItem::GetCoolDown(m_ItemId, skilllevel) / 60000.0f;
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_COOLDOWNMIN), realcooldown );	// "%.0f분 후 재시전 가능"
					}
					else
					{
						realcooldown = (float)_XSkillItem::GetCoolDown(m_ItemId, skilllevel) / 1000.0f;
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_COOLDOWNSEC), realcooldown );	// "%.1f초 후 재시전 가능"
					}
					int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx-16 );
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
				}		
				
				if( _XSkillItem::GetDistance(m_ItemId) > 0.0f )
				{
					sprintf( m_CurrentMiniTooltipInfo.Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_DISTANCE), _XSkillItem::GetDistance(m_ItemId) );	// "유효 거리 %.1f"
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				}
				
				if( g_SkillProperty[m_ItemId]->detailInfo[skilllevel].requirementItemID > 0 )
				{
					int reqtype = g_SkillProperty[m_ItemId]->detailInfo[skilllevel].reqItemCategory;
					int reqitemid = g_SkillProperty[m_ItemId]->detailInfo[skilllevel].requirementItemID;
					
					LPTSTR param1 = _XGameItem::GetItemName(reqtype, reqitemid);
					int    param2 = g_SkillProperty[m_ItemId]->detailInfo[skilllevel].itemConsumed;

					// "%s %d개 소모"
					strcpy( m_CurrentMiniTooltipInfo.Data_Buf[7] , g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_CONSUMED, param1, &param2 ) );					

					int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[7], m_MiniTooltipSize.cx-16 );
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
				}
				
				if( g_SkillProperty[m_ItemId]->requirementCondition )
				{
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SkillProperty[m_ItemId]->requirementCondition);
					if(iter_table == g_CharacterStateTable.end())
						break;
					
					int tmp = g_SkillProperty[m_ItemId]->requirementCondition;
					tmp--;
					if( tmp > 0 )
					{
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[8], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_REQCONDITION), g_CharacterConditionTable[tmp].name );	// "%s상태 시 사용가능"
						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[8], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
					}
				}			
				
				int index = g_pQuickSlot_Window->GetSelectedSet()*10 + g_pQuickSlot_Window->GetLevelButtonIndex();
				learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_pQuickSlot_Window->m_QuickSlot[index].skillid, TRUE);

				_XUserSkill* pSkill = NULL;
				if(learnedindex == _XDEF_MAX_LEARNSKILL)
				{
					pSkill = &g_pLocalUser->m_EventSkillList;
				}
				else
				{
					pSkill = g_pLocalUser->GetSkillItem(g_pQuickSlot_Window->m_QuickSlot[index].skillid);
				}

				if(pSkill)
				{
					if( pSkill->m_ucSkillState > 0)
					{
						switch(g_SkillProperty[m_ItemId]->usefulType) 
						{
						case _XSI_USEFULTYPE_PASSIVESKILL:
							break;
						case _XSI_USEFULTYPE_ACTIVESKILL:
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_TRAININGACT) );	// "수련이 필요합니다.(초식 수련장)"
							}
							break;
						case _XSI_USEFULTYPE_CHARGESKILL:
							break;
						case _XSI_USEFULTYPE_PASSIVESPELL:
							break;
						case _XSI_USEFULTYPE_CASTINGSPELL:
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_TRAININGFOR) );	// "수련이 필요합니다.(기공 수련장)"
							}
							break;
						case _XSI_USEFULTYPE_CHANNELINGSPELL:
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_TRAININGFOR) );	// "수련이 필요합니다.(기공 수련장)"
							}
							break;
						case _XSI_USEFULTYPE_FINISHSKILL:
							break;
						case _XSI_USEFULTYPE_ROUNDINGSPELL:
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[9], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_TRAININGFOR) );	// "수련이 필요합니다.(기공 수련장)"
							}
							break;
						case _XSI_USEFULTYPE_HIDDENMA:
							break;
						case _XSI_USEFULTYPE_SMASHINGSKILL:
							break;
						}

						if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[9]) )
						{
							int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[9], m_MiniTooltipSize.cx-16 );
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						}
					}
				}
				
				// 성취 단계 제한 - 2006.08.02 정식서버로 
				pSkill = NULL;
				int checkskilllevel = g_pLocalUser->GetLearnedSkillIndex(m_ItemId, TRUE);
				if(checkskilllevel == _XDEF_MAX_LEARNSKILL)
				{
					pSkill = &g_pLocalUser->m_EventSkillList;
				}
				else
				{
					pSkill = g_pLocalUser->GetSkillItem(m_ItemId);
				}

				if( pSkill )
				{
					int skilllevel = pSkill->m_cSkillLevel;
					if( skilllevel < 11 )
					{
						if( g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].requirementMission )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
							strcpy( m_CurrentMiniTooltipInfo.Data_Buf[10], _XGETINTERFACETEXT(ID_STRING_NEW_2982) );	// 
						}
						if( g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].objectID > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
						{
							int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].objectID);				// 성 ( 1 - 12 )
							int templevel = ( g_SkillProperty[m_ItemId]->detailInfo[skilllevel+1].objectID + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )					
							strcpy( m_CurrentMiniTooltipInfo.Data_Buf[11], 
									g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2983, g_LevelName[templevel], &tempstep ) ); //성취 단계 상승 조건 - %s %d성

							int length = g_XBaseFont->GetRowCount(m_CurrentMiniTooltipInfo.Data_Buf[11], m_MiniTooltipSize.cx-16);
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						}
					}

#ifdef _XDEF_NEWSTRINGMANAGER
					if(g_SkillProperty[m_ItemId]->information)
#else
					if(strlen(g_SkillProperty[m_ItemId]->information))
#endif
					{
						short tempParameter[10];				
						for( int i = 0; i < 10; i++ )
						{
							tempParameter[i] = _XSkillItem::GetParameterValue( m_ItemId, pSkill->m_cSelectedSkillLevel, g_SkillProperty[m_ItemId]->parameter[i] );
						}
						
						m_MiniTooltipSize.cy += MT_LINE_INTERVAL*2;

						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것
						int param1 = tempParameter[0]; int param2 = tempParameter[1];
						int param3 = tempParameter[2]; int param4 = tempParameter[3];
						int param5 = tempParameter[4]; int param6 = tempParameter[5];
						int param7 = tempParameter[6]; int param8 = tempParameter[7];

						strcpy( m_CurrentMiniTooltipInfo.LData_Buf, 
								g_StringDataBase.CompositeString( g_SkillProperty[m_ItemId]->information, g_SkillProperty[m_ItemId]->wordorder,
																  &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );
/*
						sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_SkillProperty[m_ItemId]->information, 
							tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
							tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9] );									
*/
						
						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
					}
				}
			}
		}
		break;
	case MTMODE_BUFF_EXTERNAL :
		{
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
			strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_EXTERNAL) );	// "외상 입음"
			
			m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
			int external = m_ItemId * 100 / g_pLocalUser->m_MaxLifePower.Get_result();
//			int external = m_ItemId * 100 / g_pLocalUser->m_CharacterInfo.Get_max_lifepower();
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CUREXTERNAL), external );	// "현재 외상 : %d%%"
		}
		break;
	case MTMODE_BUFF_INTERNAL :
		{
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
			strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_INTERNAL) );	// "내상 입음"
			
			m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
			int internal = m_ItemId * 100 / g_pLocalUser->m_MaxForcePower.Get_result();
//			int internal = m_ItemId * 100 / g_pLocalUser->m_CharacterInfo.Get_max_forcepower();
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURINTERNAL), internal );	// "현재 내상 : %d%%"
		}
		break;
	case MTMODE_BUFF_ITEM:
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( m_ItemId > 0 && m_ItemId <= g_MaxItemCount[m_ItemType] )
			{
				char itemType = GetEqualItemType(m_ItemType);
				switch(itemType) 
				{
				case _XGI_FC_WEAPON :
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
						if( WeaponItem )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
							strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );							
							
							// 패널티 표현					
							int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(WeaponItem[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
							int templevel = ( WeaponItem[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
							int selflevel = g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel();
							int itemlevel = templevel*12+tempstep;
							if( selflevel < itemlevel )
							{
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_ATTACKDAMAGE) );	// "공격력"
							}
							
#ifdef _XTS_ITEMPENERTY
							if( WeaponItem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
							{
								int result = g_pLocalUser->m_Zen.result + g_pLocalUser->m_ElixirInfo.GetTotalBonus(1);
								if( result < WeaponItem[m_ItemId].sReqJung )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACCURATE) );
								}
								result = g_pLocalUser->m_Intelligence.result + g_pLocalUser->m_ElixirInfo.GetTotalBonus(2);
								if( result < WeaponItem[m_ItemId].sReqSim )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CRITICALRATE) );
								}
							}
							else
							{
								int result = g_pLocalUser->m_Strength.result + g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
								if( result < WeaponItem[m_ItemId].sReqJung )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACCURATE) );
								}
								result = g_pLocalUser->m_Dex.result + g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
								if( result < WeaponItem[m_ItemId].sReqSim )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CRITICALRATE) );
								}
							}
#else
							if( WeaponItem[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
							{
								if( g_pLocalUser->m_CharacterInfo.Get_zen() < WeaponItem[m_ItemId].sReqJung )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACCURATE) );
								}
								
								if( g_pLocalUser->m_CharacterInfo.Get_intelligence() < WeaponItem[m_ItemId].sReqSim )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CRITICALRATE) );
								}
							}
							else
							{
								if( g_pLocalUser->m_CharacterInfo.Get_strength() < WeaponItem[m_ItemId].sReqJung )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING1) );	// "정확도"
								}
								if( g_pLocalUser->m_CharacterInfo.Get_dexterity() < WeaponItem[m_ItemId].sReqSim )
								{
									if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
									else
										strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING2) );	// "치명타율"
								}								
							}												
#endif
#ifdef _XTS_FAME
							if( WeaponItem[m_ItemId].ucFame > 0 && !g_FameStr->CheckItemFame(WeaponItem[m_ItemId].ucFame) )
							{
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[6], _XGETINTERFACETEXT(ID_STRING_NEW_2790) );	// _T("이때 추가 효과는 적용되지 않습니다.")
								int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[6], m_MiniTooltipSize.cx-16 );
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
							}							
#endif
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
							{
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING3) );	// "의 하락이 있으며,"
								int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx-16 );
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;								
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING5));	// "내구도가 빠르게 감소합니다."
								length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx-16 );
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
								m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
							}					

							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) || strlen(m_CurrentMiniTooltipInfo.Data_Buf[6]) )
							{
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING4));	// "현재 자신의 능력을 벗어나는 무기를 쓰고 있습니다."
								int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[3], m_MiniTooltipSize.cx-16 );
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
							}								
							
							int avg = 10;
							int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
							int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;
							if(curdur > 0 && maxdur > 0)
							{
								avg = curdur*10 / maxdur;
							}
							if( avg == 0 && curdur > 0 )
								avg = 1;
							
							if( avg < 2 )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7) );	// "물품이 파괴될 수 있습니다."
							}
							else if( avg < 3 )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
							}
							
							int percentdur = 0;
							if(curdur > 0 && maxdur > 0)
							{
								percentdur = curdur * 100 / maxdur;					
								
								if( percentdur > 0 )
								{
									m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
									sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
								}
							}						
						}
					}
					break;
				case _XGI_FC_CLOTHES :
					{
						_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
						if( ClothesItem )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
							strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );							
							
							int avg = 10;
							int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_usCurDur()+99 ) / 100;
							int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].Get_m_usMaxDur()+99 ) / 100;
							if(curdur > 0 && maxdur > 0)
							{
								avg = curdur*10 / maxdur;
							}
							if( avg == 0 && curdur > 0 )
								avg = 1;
							
							if( avg < 2 )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7));	// "물품이 파괴될 수 있습니다."
							}
							else if( avg < 3 )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
							}
							
							int percentdur = 0;
							if(curdur > 0 && maxdur > 0)
							{
								percentdur = (curdur * 100) / maxdur;
								if( percentdur > 0 )
								{
									m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
									sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
								}					
							}							
						}
					}
					break;
				case _XGI_FC_POTION:
					{
						_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
						if( PotionItem )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
							strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );							
							
							if( PotionItem[m_ItemId].sApplyValue > 0 && PotionItem[m_ItemId].sCooldown > 0 )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2;
								m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
							}
							
							switch(PotionItem[m_ItemId].cSecondType)
							{
							case 0:		// 내상
								{
									switch(PotionItem[m_ItemId].cApplyType) 
									{					
									case 0:
										{
											// "내상을 %d초 동안 %d만큼 회복시켜줍니다."

											int param1 = PotionItem[m_ItemId].sCooldown;
											int param2 = PotionItem[m_ItemId].sApplyValue;
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_INTERNALEFFECT, &param1, &param2 ) );
										}
										break;
									case 1:
										{
											int param1 = PotionItem[m_ItemId].sCooldown;
											int param2 = PotionItem[m_ItemId].sApplyValue;

											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_INTERNALEFFECTRATE, &param1, &param2 ) );
										}
										break;
									}
								}				
								break;
							case 1:		// 외상
								{
									switch(PotionItem[m_ItemId].cApplyType) 
									{					
									case 0:
										{
											int param1 = PotionItem[m_ItemId].sCooldown;
											int param2 = PotionItem[m_ItemId].sApplyValue;

											// "외상을 %d초 동안 %d만큼 회복시켜줍니다."
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_EXTERNALEFFECT, &param1, &param2 ) );
										}
										break;
									case 1:
										{
											// "외상을 %d초 동안 %d%%만큼 회복시켜줍니다."
											int param1 = PotionItem[m_ItemId].sCooldown;
											int param2 = PotionItem[m_ItemId].sApplyValue;
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
												g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_EXTERNALEFFECTRATE, &param1, &param2 ) );
										}
										break;
									}
								}				
								break;
							case 2:		// 피로
								{
									switch(PotionItem[m_ItemId].cApplyType) 
									{					
									case 0:
										{
											int param1 = PotionItem[m_ItemId].sCooldown;
											int param2 = PotionItem[m_ItemId].sApplyValue;
											
											// "피로를 %d초 동안 %d만큼 회복시켜줍니다."
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_TIREDEFFECT, &param1, &param2 ) );
										}
										break;
									case 1:
										{
											int param1 = PotionItem[m_ItemId].sCooldown;
											int param2 = PotionItem[m_ItemId].sApplyValue;

											// "피로를 %d초 동안 %d%%만큼 회복시켜줍니다."
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_TIREDEFFECTRATE, &param1, &param2 ) );
										}
										break;
									}
								}				
								break;
							case 4:		// 음식
								{
									switch(PotionItem[m_ItemId].cApplyType) 
									{					
									case 0:
										{
											int param1 = PotionItem[m_ItemId].sCooldown;											
											int result = PotionItem[m_ItemId].sApplyValue * ( PotionItem[m_ItemId].sCooldown / 3 );

											// "생명을 %d초 동안 %d만큼 회복시켜줍니다."
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0],
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_LIFEEFFECT, &param1, &result ) );
										}						
										break;
									}
								}
								break;
							}
							
							if( m_TimeCount < PotionItem[m_ItemId].sCooldown * 1000 )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
								int count = ( PotionItem[m_ItemId].sCooldown * 1000 - m_TimeCount) / 60000;
								if( count > 0 )
								{
									sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_REMAINMIN), count ); // %d분 남음
								}
								else
								{
									count = ( PotionItem[m_ItemId].sCooldown * 1000 - m_TimeCount) % 60000;
									count /= 1000;
									sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_REMAINSEC), count ); // %d초 남음
								}
							}
						}					
					}
					break;
				case _XGI_FC_CONSUMING :
					{
						_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
						if( ConsumingItem )
						{
							map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(ConsumingItem[m_ItemId].usSEID);
							if(iter_table != g_CharacterStateTable.end())
							{
								if( strlen( g_CharacterStateTable[ConsumingItem[m_ItemId].usSEID]->name ) )
								{
									m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
									m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
									strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterStateTable[ConsumingItem[m_ItemId].usSEID]->name );
									
									if( m_TimeCount < ConsumingItem[m_ItemId].usTime * 1000 )
									{
										m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
										int count = ( ConsumingItem[m_ItemId].usTime * 1000 - m_TimeCount) / 60000;
										if( count > 0 )
										{
											LPTSTR param1 = g_CharacterStateTable[ConsumingItem[m_ItemId].usSEID]->name;
											// "%s 상태 %d분 남음"
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MINITOOLTIP_STATUSREMAINMIN, param1,  &count ) );
										}
										else
										{
											count = ( ConsumingItem[m_ItemId].usTime * 1000 - m_TimeCount) % 60000;
											count /= 1000;
											
											LPTSTR param1 = g_CharacterStateTable[ConsumingItem[m_ItemId].usSEID]->name;
											// "%s 상태 %d초 남음"
											strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MINITOOLTIP_STATUSREMAINSEC, param1,  &count ) );
										}
									}
								}
							}
						}															
					}
					break;
				case _XGI_FC_ELIXIR :
					{
						_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
						if( ElixirItem )
						{
							if( strlen( g_CharacterStateTable[ElixirItem[m_ItemId].usTemp[0]]->name ) )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
								strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterStateTable[ElixirItem[m_ItemId].usTemp[0]]->name );
								
								m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
								
								int value;
								if( ElixirItem[m_ItemId].third_type > 1 )
									value = ElixirItem[m_ItemId].usValue_Min / 100;
								else 
									value = ElixirItem[m_ItemId].usValue_Min;

                                //Author : 양희왕 //breif : 6/20일 구전탕 같은 영약 미만으로 변경 됨 그러므로 + 1 해준다
                                if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
                                    sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], g_CharacterStateTable[ElixirItem[m_ItemId].usTemp[0]]->concept, value + 1);
                                else
                                    sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], g_CharacterStateTable[ElixirItem[m_ItemId].usTemp[0]]->concept, value );
								
								
								int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
							}
						}						
					}
					break;
				case _XGI_FC_ACCESSORY :
					{
						_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(m_ItemType);
						if( AccessoryItem )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
							strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );
								
							m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
								
							strcpy( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_NEW_2791) );	// "자신의 칭호와 맞지 않은 소지품을 장착하고 있습니다. 추가 효과는 적용 되지 않습니다."
							int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx-16 );
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						}
					}
					break;
				}
			}			
#else			
			switch(m_ItemType) 
			{
			case _XGI_FC_WEAPON:
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_WEAPON] )
						break;

					if( strlen(g_WeaponItemProperty[m_ItemId].cName) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_WeaponItemProperty[m_ItemId].cName );
					}

					// 패널티 표현					
					int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(g_WeaponItemProperty[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
					int templevel = ( g_WeaponItemProperty[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
					int selflevel = g_pLocalUser->m_CharacterInfo.level*12+g_pLocalUser->m_CharacterInfo.sublevel;
					int itemlevel = templevel*12+tempstep;
					if( selflevel < itemlevel )
					{
						strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_ATTACKDAMAGE) );	// "공격력"
					}
					
					if( g_WeaponItemProperty[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
					{
						if( g_pLocalUser->m_CharacterInfo.zen < g_WeaponItemProperty[m_ItemId].sReqJung )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING1) );	// "정확도"
						}
						if( g_pLocalUser->m_CharacterInfo.intelligence < g_WeaponItemProperty[m_ItemId].sReqSim )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING2) );	// "치명타율"
						}						
					}
					else
					{
						if( g_pLocalUser->m_CharacterInfo.strength < g_WeaponItemProperty[m_ItemId].sReqJung )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING1) );	// "정확도"
						}
						if( g_pLocalUser->m_CharacterInfo.dexterity < g_WeaponItemProperty[m_ItemId].sReqSim )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING2) );	// "치명타율"
						}								
					}
					if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
					{
						strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING3) );	// "의 하락이 있으며,"
						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING4));	// "현재 자신의 능력을 벗어나는 무기를 쓰고 있습니다."
						length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[3], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING5));	// "내구도가 빠르게 감소합니다."
						length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
					}					

					int avg = 10;
					int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurDur+99 ) / 100;
					int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usMaxDur+99 ) / 100;
					if(curdur > 0 && maxdur > 0)
					{
						avg = curdur*10 / maxdur;
					}
					if( avg == 0 && curdur > 0 )
						avg = 1;

					if( avg < 2 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7) );	// "물품이 파괴될 수 있습니다."
					}
					else if( avg < 3 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
					}

					int percentdur = 0;
					if(curdur > 0 && maxdur > 0)
					{
						percentdur = curdur * 100 / maxdur;					
					
						if( percentdur > 0 )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
							sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
						}
					}
				}
				break;
			case _XGI_FC_CLOTHES:
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_CLOTHES] )
						break;

					if( strlen(g_ClothesItemProperty[m_ItemId].cName) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_ClothesItemProperty[m_ItemId].cName );
					}
					
					int avg = 10;
					int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurDur+99 ) / 100;
					int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usMaxDur+99 ) / 100;
					if(curdur > 0 && maxdur > 0)
					{
						avg = curdur*10 / maxdur;
					}
					if( avg == 0 && curdur > 0 )
						avg = 1;
					
					if( avg < 2 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7));	// "물품이 파괴될 수 있습니다."
					}
					else if( avg < 3 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
					}
					
					int percentdur = 0;
					if(curdur > 0 && maxdur > 0)
					{
						percentdur = (curdur * 100) / maxdur;
						if( percentdur > 0 )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
							sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
						}					
					}
				}
				break;
			case _XGI_FC_WEAPON2:
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_WEAPON2] )
						break;

					if( strlen(g_WeaponItemProperty2[m_ItemId].cName) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_WeaponItemProperty2[m_ItemId].cName );
					}

					// 패널티 표현					
					int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(g_WeaponItemProperty2[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
					int templevel = ( g_WeaponItemProperty2[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
					int selflevel = g_pLocalUser->m_CharacterInfo.level*12+g_pLocalUser->m_CharacterInfo.sublevel;
					int itemlevel = templevel*12+tempstep;
					if( selflevel < itemlevel )
					{
						strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_ATTACKDAMAGE) );	// "공격력"
					}
					if( g_WeaponItemProperty2[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
					{
						if( g_pLocalUser->m_CharacterInfo.zen < g_WeaponItemProperty2[m_ItemId].sReqJung )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING1) );	// "정확도"
						}
						if( g_pLocalUser->m_CharacterInfo.intelligence < g_WeaponItemProperty2[m_ItemId].sReqSim )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING2) );	// "치명타율"
						}						
					}
					else
					{
						if( g_pLocalUser->m_CharacterInfo.strength < g_WeaponItemProperty2[m_ItemId].sReqJung )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING1) );	// "정확도"
						}
						if( g_pLocalUser->m_CharacterInfo.dexterity < g_WeaponItemProperty2[m_ItemId].sReqSim )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );	
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING2) );	// "치명타율"
						}								
					}
					if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
					{
						strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING3) );	// "의 하락이 있으며,"
						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING4));	// "현재 자신의 능력을 벗어나는 무기를 쓰고 있습니다."
						length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[3], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING5));	// "내구도가 빠르게 감소합니다."
						length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						 m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
					}					

					int avg = 10;
					int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurDur+99 ) / 100;
					int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usMaxDur+99 ) / 100;
					if(curdur > 0 && maxdur > 0)
					{
						avg = curdur*10 / maxdur;
					}
					if( avg == 0 && curdur > 0 )
						avg = 1;

					if( avg < 2 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7) );	// "물품이 파괴될 수 있습니다."
					}
					else if( avg < 3 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
					}

					int percentdur = 0;
					if(curdur > 0 && maxdur > 0)
					{
						 percentdur = curdur * 100 / maxdur;
						if( percentdur > 0 )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
							sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
						}
					}					
				}
				break;
			case _XGI_FC_CLOTHES2:
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_CLOTHES2] )
						break;

					if( strlen(g_ClothesItemProperty2[m_ItemId].cName) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_ClothesItemProperty2[m_ItemId].cName );
					}
					
					int avg = 10;
					int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurDur+99 ) / 100;
					int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usMaxDur+99 ) / 100;
					if(curdur > 0 && maxdur > 0)
					{
						avg = curdur*10 / maxdur;
					}
					if( avg == 0 && curdur > 0 )
						avg = 1;
					
					if( avg < 2 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7));	// "물품이 파괴될 수 있습니다."
					}
					else if( avg < 3 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
					}
					
					int percentdur  = 0;
					if(curdur > 0 && maxdur > 0)
					{
						 percentdur = curdur * 100 / maxdur;
						if( percentdur > 0 )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
							sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
						}
					}					
				}
				break;		
			case _XGI_FC_WEAPON3:
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_WEAPON3] )
						break;

					if( strlen(g_WeaponItemProperty3[m_ItemId].cName) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_WeaponItemProperty3[m_ItemId].cName );
					}

					// 패널티 표현					
					int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(g_WeaponItemProperty3[m_ItemId].sReqLevel);				// 성 ( 1 - 12 )
					int templevel = ( g_WeaponItemProperty3[m_ItemId].sReqLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
					int selflevel = g_pLocalUser->m_CharacterInfo.level*12+g_pLocalUser->m_CharacterInfo.sublevel;
					int itemlevel = templevel*12+tempstep;
					if( selflevel < itemlevel )
					{
						strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_ATTACKDAMAGE) );	// "공격력"
					}
					if( g_WeaponItemProperty3[m_ItemId].cSecondType == _XGI_SC_WEAPON_SPECIAL )
					{
						if( g_pLocalUser->m_CharacterInfo.zen < g_WeaponItemProperty3[m_ItemId].sReqJung )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING1) );	// "정확도"
						}
						if( g_pLocalUser->m_CharacterInfo.intelligence < g_WeaponItemProperty3[m_ItemId].sReqSim )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING2) );	// "치명타율"
						}						
					}
					else
					{					
						if( g_pLocalUser->m_CharacterInfo.strength < g_WeaponItemProperty3[m_ItemId].sReqJung )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2192) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING1) );	// "정확도"
						}
						if( g_pLocalUser->m_CharacterInfo.dexterity < g_WeaponItemProperty3[m_ItemId].sReqSim )
						{
							if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2193) );
							else
								strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING2) );	// "치명타율"
						}								
					}
					if( strlen(m_CurrentMiniTooltipInfo.Data_Buf[4]) )
					{
						strcat( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING3) );	// "의 하락이 있으며,"
						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[3], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING4));	// "현재 자신의 능력을 벗어나는 무기를 쓰고 있습니다."
						length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[3], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[5], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING5));	// "내구도가 빠르게 감소합니다."
						length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[5], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						 m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
					}					

					int avg = 10;
					int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurDur+99 ) / 100;
					int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usMaxDur+99 ) / 100;
					if(curdur > 0 && maxdur > 0)
					{
						avg = curdur*10 / maxdur;
					}
					if( avg == 0 && curdur > 0 )
						avg = 1;

					if( avg < 2 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7) );	// "물품이 파괴될 수 있습니다."
					}
					else if( avg < 3 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
					}

					int percentdur = 0;
					if(curdur > 0 && maxdur > 0)
					{
						 percentdur = curdur * 100 / maxdur;
						if( percentdur > 0 )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
							sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
						}
					}					
				}
				break;
			case _XGI_FC_CLOTHES3:
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_CLOTHES3] )
						break;

					if( strlen(g_ClothesItemProperty3[m_ItemId].cName) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_ClothesItemProperty3[m_ItemId].cName );
					}
					
					int avg = 10;
					int curdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usCurDur+99 ) / 100;
					int maxdur = ( g_pLocalUser->m_UserItemList[m_CurrentSlotNumber].m_usMaxDur+99 ) / 100;
					if(curdur > 0 && maxdur > 0)
					{
						avg = curdur*10 / maxdur;
					}
					if( avg == 0 && curdur > 0 )
						avg = 1;
					
					if( avg < 2 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2 + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING6) );	// "수리가 시급합니다."
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING7));	// "물품이 파괴될 수 있습니다."
					}
					else if( avg < 3 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_WARNING8) );	// "수리가 필요합니다."
					}
					
					int percentdur  = 0;
					if(curdur > 0 && maxdur > 0)
					{
						 percentdur = curdur * 100 / maxdur;
						if( percentdur > 0 )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL + MT_LINE_INTERVAL;
							sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_CURRENTDUR), percentdur );	// "현재 내구도 : %d%%"
						}
					}					
				}
				break;
			case _XGI_FC_POTION:
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_POTION] )
						break;

					if( strlen(g_PotionItemProperty[m_ItemId].cName) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_PotionItemProperty[m_ItemId].cName );
					}
					
					if( g_PotionItemProperty[m_ItemId].sApplyValue > 0 && g_PotionItemProperty[m_ItemId].sCooldown > 0 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*2;
						m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
					}
					
					switch(g_PotionItemProperty[m_ItemId].cSecondType)
					{
					case 0:		// 내상
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								{
									int param1 = g_PotionItemProperty[m_ItemId].sCooldown;
									int param2 = g_PotionItemProperty[m_ItemId].sApplyValue;
									
									// "내상을 %d초 동안 %d만큼 회복시켜줍니다."
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0],
										g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_INTERNALEFFECT, &param1, &param2 ) );
								}
								break;
							case 1:
								{
									int param1 = g_PotionItemProperty[m_ItemId].sCooldown;
									int param2 = g_PotionItemProperty[m_ItemId].sApplyValue;

									// "내상을 %d초 동안 %d%%만큼 회복시켜줍니다."
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0],
											g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_INTERNALEFFECTRATE, &param1, &param2 ) );
								}
								break;
							}
						}				
						break;
					case 1:		// 외상
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								{
									int param1 = g_PotionItemProperty[m_ItemId].sCooldown;
									int param2 = g_PotionItemProperty[m_ItemId].sApplyValue;
									
									// "외상을 %d초 동안 %d만큼 회복시켜줍니다."
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
										g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_EXTERNALEFFECT, &param1, &param2 ) );
								}
								break;
							case 1:
								{
									int param1 = g_PotionItemProperty[m_ItemId].sCooldown;
									int param2 = g_PotionItemProperty[m_ItemId].sApplyValue;
									
									// "외상을 %d초 동안 %d%%만큼 회복시켜줍니다."
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
										g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_EXTERNALEFFECTRATE, &param1, &param2 ) );

								}
								break;
							}
						}				
						break;
					case 2:		// 피로
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								{
									int param1 = g_PotionItemProperty[m_ItemId].sCooldown;
									int param2 = g_PotionItemProperty[m_ItemId].sApplyValue;
									// "피로를 %d초 동안 %d만큼 회복시켜줍니다."
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
											g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_TIREDEFFECT, &param1, &param2 ) );
								}
								break;
							case 1:
								{
									int param1 = g_PotionItemProperty[m_ItemId].sCooldown;
									int param2 = g_PotionItemProperty[m_ItemId].sApplyValue;
									// "피로를 %d초 동안 %d%%만큼 회복시켜줍니다."
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0] ,
										g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_TIREDEFFECTRATE, &param1, &param2 ) );
								}
								break;
							}
						}				
						break;
					case 4:		// 음식
						{
							switch(g_PotionItemProperty[m_ItemId].cApplyType) 
							{					
							case 0:
								{
									int param1 = g_PotionItemProperty[m_ItemId].sCooldown;
									int result = g_PotionItemProperty[m_ItemId].sApplyValue * ( g_PotionItemProperty[m_ItemId].sCooldown / 3 );

									// "생명을 %d초 동안 %d만큼 회복시켜줍니다."
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0],
											g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_LIFEEFFECT, &param1, &result ) );
								}						
								break;
							}
						}
						break;
					}
					
					if( m_TimeCount < g_PotionItemProperty[m_ItemId].sCooldown * 1000 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						int count = ( g_PotionItemProperty[m_ItemId].sCooldown * 1000 - m_TimeCount) / 60000;
						if( count > 0 )
						{
							sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_REMAINMIN), count ); // %d분 남음
						}
						else
						{
							count = ( g_PotionItemProperty[m_ItemId].sCooldown * 1000 - m_TimeCount) % 60000;
							count /= 1000;
							sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_REMAINSEC), count ); // %d초 남음
						}
					}
				}
				break;
			case _XGI_FC_CONSUMING :
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_CONSUMING] )
						break;

					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_ConsumingItemProperty[m_ItemId].usSEID);
					if(iter_table != g_CharacterStateTable.end())
					{
						if( strlen( g_CharacterStateTable[g_ConsumingItemProperty[m_ItemId].usSEID]->name ) )
						{
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
							m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
							strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterStateTable[g_ConsumingItemProperty[m_ItemId].usSEID]->name );

							if( m_TimeCount < g_ConsumingItemProperty[m_ItemId].usTime * 1000 )
							{
								m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
								int count = ( g_ConsumingItemProperty[m_ItemId].usTime * 1000 - m_TimeCount) / 60000;
								if( count > 0 )
								{
									LPTSTR param1 = g_CharacterStateTable[g_ConsumingItemProperty[m_ItemId].usSEID]->name;
									// "%s 상태 %d분 남음"
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], 
											g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MINITOOLTIP_STATUSREMAINMIN, param1,  &count ));
								}
								else
								{
									count = ( g_ConsumingItemProperty[m_ItemId].usTime * 1000 - m_TimeCount) % 60000;
									count /= 1000;

									// "%s 상태 %d초 남음"
									LPTSTR param1 = g_CharacterStateTable[g_ConsumingItemProperty[m_ItemId].usSEID]->name;
									strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], 
										g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MINITOOLTIP_STATUSREMAINSEC, param1,  &count ));
								}
							}
						}
					}									
				}
				break;
			case _XGI_FC_ELIXIR :
				{
					if( m_ItemId <= 0 || m_ItemId > g_MaxItemCount[_XGI_FC_ELIXIR] )
						break;
					
					if( strlen( g_CharacterStateTable[g_ElixirItemProperty[m_ItemId].usTemp[0]]->name ) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterStateTable[g_ElixirItemProperty[m_ItemId].usTemp[0]]->name );

						m_MiniTooltipSize.cy += MT_LINE_INTERVAL;

						int value;
						if( g_ElixirItemProperty[m_ItemId].third_type > 1 )
							value = g_ElixirItemProperty[m_ItemId].usValue_Min / 100;
						else 
							value = g_ElixirItemProperty[m_ItemId].usValue_Min;
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], g_CharacterStateTable[g_ElixirItemProperty[m_ItemId].usTemp[0]]->concept, value );

						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
					}
				}
				break;
#ifdef _XTS_FAME				
			case _XGI_FC_ACCESSORY :
				{
					if( strlen( g_AccessoryItemProperty[m_ItemId].cName ) )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_AccessoryItemProperty[m_ItemId].cName );
						
						m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
						
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[4], _XGETINTERFACETEXT(ID_STRING_NEW_2791) );	// "자신의 칭호와 맞지 않은 소지품을 장착하고 있습니다. 추가 효과는 적용 되지 않습니다."							
						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[4], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
					}
				}
				break;
#endif
			}						
#endif
		}
		break;
	case MTMODE_BUFF_SKILL:
		{
			if( _XSkillItem::FindSkillProperty(m_ItemId) )
			{
				if( strlen( g_SkillProperty[m_ItemId]->skillName ) )
				{
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_SkillProperty[m_ItemId]->skillName );
				}
				
				if( strlen( g_SkillProperty[m_ItemId]->information ) )
				{
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_ItemId);
					short tempParameter[10];				
					for( int i = 0; i < 10; i++ )
					{
						tempParameter[i] = _XSkillItem::GetParameterValue( m_ItemId, m_CurrentSlotNumber, g_SkillProperty[m_ItemId]->parameter[i] );
					}

					m_MiniTooltipSize.cy += MT_LINE_INTERVAL;

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것

					int param1 = tempParameter[0]; int param2 = tempParameter[1];
					int param3 = tempParameter[2]; int param4 = tempParameter[3];
					int param5 = tempParameter[4]; int param6 = tempParameter[5];
					int param7 = tempParameter[6]; int param8 = tempParameter[7];
					
//					_XDWINPRINT("information : %s", g_SkillProperty[m_ItemId]->information);

					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, 
							g_StringDataBase.CompositeString( g_SkillProperty[m_ItemId]->information, g_SkillProperty[m_ItemId]->wordorder,
															  &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 ) );
/*
					sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_SkillProperty[m_ItemId]->information, 
						tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
						tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9] );				
*/					
					int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
				}
				
				if( m_TimeCount > 0 )
				{
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					
					int count = m_TimeCount / 60000;
					if( count > 0 )
					{
						sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2194), count );
					}
					else
					{
						count = m_TimeCount % 60000;
						count /= 1000;
						sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2195), count );
					}
				}

				int findindex = -1;
				_XNPCBUFFSTR* pNPCBuff = NULL;
				_map_NPCBuffStr::iterator iter_buff;
				for(iter_buff = g_NPCBuffStr.begin() ; iter_buff != g_NPCBuffStr.end() ; ++iter_buff)
				{
					pNPCBuff = iter_buff->second;
					if(pNPCBuff)
					{
						if(m_ItemId == pNPCBuff->nSkillindex)
						{
							findindex = iter_buff->first;
							break;
						}
					}
				}

//				for( int i = 0; i < g_NPCBuffCount; i++ )
//				{
//					if( m_ItemId == g_NPCBuffStr[i].nSkillindex )
//					{
//						findindex = i;
//						break;
//					}
//				}
				if(findindex > 0)
				{
					if( pNPCBuff )
					{
						switch(pNPCBuff->type) 
						{
						case 4 :
							strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2196) );
							break;
						case 5 :
							strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2197) );
							break;
						}

						if( pNPCBuff->type > 0 )
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					}
				}
			}
		}
		break;
	case MTMODE_BUFF_EFFECT:
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(m_ItemId);
			if(iter_table == g_CharacterStateTable.end())
				break;

			if( strlen( g_CharacterStateTable[m_ItemId]->name ) )
			{
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterStateTable[m_ItemId]->name );
			}
			
			int findindex = -1;
			_XNPCBUFFSTR* pNPCBuff = NULL;
			_map_NPCBuffStr::iterator iter_buff;
			for(iter_buff = g_NPCBuffStr.begin() ; iter_buff != g_NPCBuffStr.end() ; ++iter_buff)
			{
				pNPCBuff = iter_buff->second;
				if(pNPCBuff)
				{
					if(m_ItemId == pNPCBuff->nSkillindex)
					{
						findindex = iter_buff->first;
						break;
					}
				}
			}
//			for( int i = 0; i < g_NPCBuffCount; i++ )
//			{
//				if( m_ItemId == g_NPCBuffStr[i].bufindex )
//				{
//					findindex = i;
//					break;
//				}
//			}

			//Author : 양희왕 
			//breif : 수정
			if( findindex != -1 )
			{
				if( pNPCBuff )
				{
					switch(pNPCBuff->type) 
					{
					case 4 :
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2196) );
						break;
					case 5 :
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2197) );
						break;
					/*case 3:
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2197) );
						break;
					case 2 :
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2198) );
						break;
					case 1 :
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2199) );
						break;*/
					}
					
					if( pNPCBuff->type > 0 )
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				}					
			}// Last Updated : 07/03/28

			if( strlen( g_CharacterStateTable[m_ItemId]->concept ) )
			{
				int tempcount = 0;
				for( int i = 0; i < strlen( g_CharacterStateTable[m_ItemId]->concept ); i++ )
				{
					if( g_CharacterStateTable[m_ItemId]->concept[i] == '%' )
						tempcount++;
				}
				/*if( tempcount == 3 )
				{
				sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept, g_CharacterStateTable[m_ItemId]->prob );
			}*/
				if( tempcount == 1 || tempcount == 3 )
				{
					int index = -1;
					if( m_CurrentSlotNumber == 1)	// extra
					{
						index = g_pLocalUser->m_StateList.FindState(en_from_extra, m_ItemId, 0, m_ItemType );
					}
					else	// effect
					{
						index = g_pLocalUser->m_StateList.FindState(en_from_effect, m_ItemId, 0 );
					}						 
					
					if( index >= 0 )
					{
						map <int, _XCharacterState*>::iterator iter_table = g_pLocalUser->m_StateList.m_mapCharacterStateList.find(index);
						if(iter_table != g_pLocalUser->m_StateList.m_mapCharacterStateList.end())
						{
							_XCharacterState* pState = iter_table->second;
							if( pState )
							{
								sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept, pState->m_sValue );
							}
						}
					}					
				}
				else if( tempcount == 2 )
				{
					sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept );
				}
				else
				{
					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept );	
				}				
				
				int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
			}
			
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
			int count = m_TimeCount / 60000;
			if( count > 0 )
			{
				sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2194), count );
			}
			else
			{
				count = m_TimeCount % 60000;
				count /= 1000;
				sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2195), count );
			}
		}
		break;	
	case MTMODE_BUFF_MONSTATS_SPELL:
		{
			if( _XSkillItem::FindSkillProperty(m_ItemId) )
			{							
				if( strlen( g_SkillProperty[m_ItemId]->skillName ) )
				{
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_SkillProperty[m_ItemId]->skillName );
				}
			}
			/*
			if( strlen( g_SkillProperty[m_ItemId]->information ) )
			{
				int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_ItemId);
				short tempParameter[10];				
				for( int i = 0; i < 10; i++ )
				{
					tempParameter[i] = _XSkillItem::GetParameterValue( m_ItemId, g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel, g_SkillProperty[m_ItemId]->parameter[i] );
				}
				
				m_MiniTooltipSize.cy += MT_LINE_INTERVAL;

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Localize part : 파라미터가 현재는 최대 8개까지만 수용할 수 있음. 14개까지 사용하려면 _XStringDataBase의 Method를 수정할 것
				strcpy( m_CurrentMiniTooltipInfo.LData_Buf, 
						g_StringDataBase.CompositeString( g_SkillProperty[m_ItemId]->information, g_SkillProperty[m_ItemId]->wordorder,
														&tempParameter[0], &tempParameter[1], &tempParameter[2], &tempParameter[3], 
														&tempParameter[4], &tempParameter[5], &tempParameter[6], &tempParameter[7] ) );

				//sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_SkillProperty[m_ItemId]->information, 
				//	tempParameter[0], tempParameter[1], tempParameter[2], tempParameter[3], tempParameter[4], 
				//	tempParameter[5], tempParameter[6], tempParameter[7], tempParameter[8], tempParameter[9] );				
				
				int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, 27 );
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
			}	*/				
		}
		break;
	case MTMODE_BUFF_MONSTATS_EFFECT:
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(m_ItemId);
			if(iter_table == g_CharacterStateTable.end())
				break;

			if( strlen( g_CharacterStateTable[m_ItemId]->name ) )
			{
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterStateTable[m_ItemId]->name );
			}	/*		
			
			if( strlen( g_CharacterStateTable[m_ItemId]->concept ) )
			{
				int tempcount = 0;
				for( int i = 0; i < strlen( g_CharacterStateTable[m_ItemId]->concept ); i++ )
				{
					if( g_CharacterStateTable[m_ItemId]->concept[i] == '%' )
						tempcount++;
				}
				if( tempcount == 2 )
				{
					sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept, g_CharacterStateTable[m_ItemId]->prob );
				}
				else if( tempcount == 1 )
				{
					sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept, g_CharacterStateTable[m_ItemId]->value );
				}
				else
				{
					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept );	
				}				
				
				int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, 27 );
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
			}*/
		}
		break;
	case MTMODE_BUFF_MONSTATS_STATE :
		{
			if( strlen( g_CharacterConditionTable[m_ItemId].name ) )
			{
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterConditionTable[m_ItemId].name );
			}			
			
			/*if( strlen( g_CharacterConditionTable[m_ItemId].concept ) )
			{
				strcpy( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterConditionTable[m_ItemId].concept );
				int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, 27 );
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
			}*/			
		}
		break;
	case MTMODE_CHARINFO:
		{
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], "%d~%d", g_pLocalUser->m_P_MinAttackDamage, g_pLocalUser->m_P_MaxAttackDamage );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[1], "%d", g_pLocalUser->m_P_AttackRate.Get_result() );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[2], "%d", g_pLocalUser->m_P_Defence.Get_result() );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[3], "%d", g_pLocalUser->m_P_AvoidRate.Get_result() );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[4], "%d~%d", g_pLocalUser->m_M_MinAttackDamage, g_pLocalUser->m_M_MaxAttackDamage );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[5], "%d", g_pLocalUser->m_M_AttackRate.Get_result() );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[6], "%d", g_pLocalUser->m_M_AvoidRate.Get_result() );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[7], "%d", g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[8], "%d", g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() );
			sprintf( m_CurrentMiniTooltipInfo.Data_Buf[9], "%d", g_pLocalUser->m_M_Defence.Get_result() );
		}
		break;
	case MTMODE_CHARINFO_ABILITY :
		{
			switch(m_ItemId) 
			{
			case 0:		// 근력
				{
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_CINFO_STR)); 
					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2200) );
				}
				break;
			case 1:		// 진기
				{
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_CINFO_INNERENERGY));
					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2201) );
				}
				break;
			case 2:		// 지혜
				{
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_CINFO_HEART));
					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2202) );
				}
				break;
			case 3:		// 건강
				{
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTHENERGY));
					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2203) );
				}
				break;
			case 4:		// 민첩
				{
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_CINFO_DEX));
					strcpy( m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2204) );
				}
				break;
			}

			int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;			
		}
		break;
	case MTMODE_CHARINFO_ABILITYSTEP :
		{
			int MaxValue[10] = { 2, 3, 4, 5, 6, 6, 6, 6, 7, 10 };	// 단계별 최대치 : !! 테이블의 수치가 변하면 바꺼줘야 함

			switch(m_ItemId) 
			{
			case 0:		// 근력
				{
					sprintf( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2205), g_pLocalUser->m_ElixirInfo.Data[0].__Grade );
					for( int i = 0; i < g_pLocalUser->m_ElixirInfo.Data[0].__Grade; i++ )
					{
						if( g_pLocalUser->m_ElixirInfo.Data[0].__Data[i] > 0 )
						{
							if( g_pLocalUser->m_ElixirInfo.Data[0].__Data[i] >= MaxValue[i] )
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[0].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2206, &param1, &param2 ) );								
							}
							else
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[0].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2207, &param1, &param2 ) );
							}
						}
						else
						{
							int param1 = i+1;
							int param2 = g_pLocalUser->m_ElixirInfo.Data[0].__Data[i];							
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2612, &param1, &param2 ) );
						}

						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					}
				}
				break;
			case 1:		// 진기
				{
					sprintf( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2208), g_pLocalUser->m_ElixirInfo.Data[1].__Grade );
					for( int i = 0; i < g_pLocalUser->m_ElixirInfo.Data[1].__Grade; i++ )
					{
						if( g_pLocalUser->m_ElixirInfo.Data[1].__Data[i] > 0 )
						{
							if( g_pLocalUser->m_ElixirInfo.Data[1].__Data[i] >= MaxValue[i] )
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[1].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2206, &param1, &param2 ) );
							}
							else
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[1].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2207, &param1, &param2 ) );
							}
						}
						else
						{
							int param1 = i+1;
							int param2 = g_pLocalUser->m_ElixirInfo.Data[1].__Data[i];
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2612, &param1, &param2 ) );							
						}
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					}
				}
				break;
			case 2:		// 지혜
				{
					sprintf( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2209), g_pLocalUser->m_ElixirInfo.Data[2].__Grade );
					for( int i = 0; i < g_pLocalUser->m_ElixirInfo.Data[2].__Grade; i++ )
					{
						if( g_pLocalUser->m_ElixirInfo.Data[2].__Data[i] > 0 )
						{
							if( g_pLocalUser->m_ElixirInfo.Data[2].__Data[i] >= MaxValue[i] )
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[2].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2206, &param1, &param2 ) );
							}
							else
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[2].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2207, &param1, &param2 ) );
							}
						}
						else
						{
							int param1 = i+1;
							int param2 = g_pLocalUser->m_ElixirInfo.Data[2].__Data[i];
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2612, &param1, &param2 ) );
						}
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					}
				}
				break;
			case 3:		// 건강
				{
					sprintf( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2210), g_pLocalUser->m_ElixirInfo.Data[3].__Grade );
					for( int i = 0; i < g_pLocalUser->m_ElixirInfo.Data[3].__Grade; i++ )
					{
						if( g_pLocalUser->m_ElixirInfo.Data[3].__Data[i] > 0 )
						{
							if( g_pLocalUser->m_ElixirInfo.Data[3].__Data[i] >= MaxValue[i] )
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[3].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2206, &param1, &param2 ) );
							}
							else
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[3].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2207, &param1, &param2 ) );
							}
						}
						else
						{
							int param1 = i+1;
							int param2 = g_pLocalUser->m_ElixirInfo.Data[3].__Data[i];
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2612, &param1, &param2 ) );
						}
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					}
				}
				break;
			case 4:		// 민첩
				{
					sprintf( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2211), g_pLocalUser->m_ElixirInfo.Data[4].__Grade );
					for( int i = 0; i < g_pLocalUser->m_ElixirInfo.Data[4].__Grade; i++ )
					{
						if( g_pLocalUser->m_ElixirInfo.Data[4].__Data[i] > 0 )
						{
							if( g_pLocalUser->m_ElixirInfo.Data[4].__Data[i] >= MaxValue[i] )
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[4].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2206, &param1, &param2 ) );
							}
							else
							{
								int		param1 = i+1;
								int     param2 = g_pLocalUser->m_ElixirInfo.Data[4].__Data[i];
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2207, &param1, &param2 ) );
							}
						}
						else 
						{
							int param1 = i+1;
							int param2 = g_pLocalUser->m_ElixirInfo.Data[4].__Data[i];
							strcpy(m_CurrentMiniTooltipInfo.Data_Buf[i], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2612, &param1, &param2 ) );
						}
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					}
				}
				break;
			}

			strcpy( m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2212) );
		}
		break;
	case MTMODE_CHARINFO_INTOXICATION :
		{
			strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2213) );
			strcpy( m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2214) );

			int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;	

			if( g_pLocalUser->m_ElixirInfo._sIntoxication < 101 )
			{
				strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2215));
			}
			else if( g_pLocalUser->m_ElixirInfo._sIntoxication < 201 )
			{
				strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2216));
			}
			else
			{
				strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2217));
			}

			length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
		}
		break;
	case MTMODE_CHARINFO_FAME :
		{
#ifdef _XTS_FAME
			int rank = -1;
			switch(m_ItemId) 
			{
			case 0 :	// 명성
				{
					rank = g_FameStr->GetFameIndex(g_pLocalUser->m_CharacterInfo.Get_honor());
					if( rank != -1 )
					{
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], "%s %d%s", 
								 _XGETINTERFACETEXT(ID_STRING_NETWORK_REPUTATION), rank+1, 
								 _XGETINTERFACETEXT(ID_STRING_QUEST_STEP) );
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], g_FameStr[rank].m_FameName );
					}
				}
				break;
			case 1 :	// 악명 
				{
					rank = g_FameStr->GetInfamyIndex(g_pLocalUser->m_CharacterInfo.Get_Notoriety());
					if( rank != -1 )
					{
						sprintf( m_CurrentMiniTooltipInfo.Data_Buf[0], "%s %d%s", 
								 _XGETINTERFACETEXT(ID_STRING_NEW_2636), rank+1, //_T("악명")
								 _XGETINTERFACETEXT(ID_STRING_QUEST_STEP) );								 
						strcpy( m_CurrentMiniTooltipInfo.Data_Buf[1], g_FameStr[rank].m_InfamyName );
					}
				}
				break;
			}
#endif
		}
		break;
	case MTMODE_PARTYINFO :
		{			
			DWORD levelmager = 0;
			DWORD levelminor = 0;
			
			_XLocalUser::GetUserLevel( g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].innerlevel, levelmager, levelminor );
			
			TCHAR levelstring[128];
			
			switch(levelmager)
			{
			default :
			case 0 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR));
				break;
			case 1 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_01));
				break;
			case 2 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_02));
				break;
			case 3 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_03));
				break;
			case 4 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_04));
				break;
			case 5 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_05));
				break;
			case 6 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_06));
				break;
			case 7 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_07));
				break;
			case 8 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_08));
				break;
			case 9 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_09));
				break;
			case 10 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_10));
				break;
			case 11 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_11));
				break;
			case 12 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_12));
				break;
			case 13 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_13));
				break;
			case 14 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_14));
				break;
			case 15 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_15));
				break;
			case 16 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_16));
				break;
			case 17 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_17));
				break;
			case 18 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_18));
				break;
			case 19 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_19));
				break;
			case 20 :
				strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_20));
				break;
			}
#ifdef _XTS_PARTYMOREINFO
			TCHAR clanandclassname[256];
        #ifdef _XDEF_PARTY_NOTAVAILABLE //Author : 양희왕 //breif : 2차 직책표시
            sprintf(clanandclassname, "%s %s", g_JoinGroupName[ g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].clan ], 
				g_CharacterRollNameTable[g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].clan][g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].clanclass][g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].nClassGrade]);
        #else  
			sprintf(clanandclassname, "%s %s", g_JoinGroupName[ g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].clan ], 
				g_CharacterRollNameTable[g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].clan][g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].clanclass][1]);
        #endif
			
			strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], clanandclassname);
			
			TCHAR buffer[128];
			sprintf(buffer, "%s %d%s",levelstring, levelminor, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
#else
			TCHAR buffer[128];
			sprintf(buffer, "%s %d%s", levelstring, levelminor, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
			
#endif
			TCHAR zonename[128];
			sprintf(zonename, _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_2424), g_ZoneInfoTable[g_pLocalUser->m_PartyInfo[m_CurrentSlotNumber].serverno - 1].zoneshortname );	

			strcpy( m_CurrentMiniTooltipInfo.NameBuf, buffer);
			strcpy( m_CurrentMiniTooltipInfo.LData_Buf, zonename );

#ifdef _XDEF_PARTY_NOTAVAILABLE
			if(m_TimeCount != 0)
				strcpy(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_NEW_3392));	//"거리 이탈"
			if(m_ItemId != 0)
				strcpy(m_CurrentMiniTooltipInfo.Data_Buf[2], _XGETINTERFACETEXT(ID_STRING_NEW_3393));	//"자리 비움"
#endif
		}
		break;
	case MTMODE_BUFF_INTOXICATION :
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(m_ItemId);
			if(iter_table == g_CharacterStateTable.end())
				break;
			
			if( m_ItemId == 2003 )	// 중독 예외처리
			{
				if( strlen( g_CharacterStateTable[m_ItemId]->name ) )
				{
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_CharacterStateTable[m_ItemId]->name );

					m_MiniTooltipSize.cy += MT_LINE_INTERVAL;

					sprintf( m_CurrentMiniTooltipInfo.LData_Buf, g_CharacterStateTable[m_ItemId]->concept );

					int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
				}
			}			
		}
		break;
	case MTMODE_BUFF_NICKNAME:
		{
			if( strlen(g_NickNameInfoTable[m_ItemId].cNickName) )
			{
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, g_NickNameInfoTable[m_ItemId].cNickName );
			}

			m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
			strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2218) );
			
			for( int i = 0; i < 5; i++ )
			{
				if( g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectIndex > 0 )
				{
					if( strlen( g_CharacterStateTable[g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectIndex]->concept ) )
					{
						int tempcount = 0;
						for( int j = 0; j < strlen( g_CharacterStateTable[g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectIndex]->concept ); j++ )
						{
							if( g_CharacterStateTable[g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectIndex]->concept[j] == '%' )
								tempcount++;
						}
						
						switch(tempcount) 
						{
						case 3:
							{
								int value = g_NickNameInfoTable[m_ItemId]._Effect[i].cEffectProbability;
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[i+1], g_CharacterStateTable[g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectIndex]->concept, value );
							}
							break;
						case 1:
							{
								int value = g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectValue;
								sprintf( m_CurrentMiniTooltipInfo.Data_Buf[i+1], g_CharacterStateTable[g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectIndex]->concept, value );
							}
							break;
						default:
							{
								strcpy( m_CurrentMiniTooltipInfo.Data_Buf[i+1], g_CharacterStateTable[g_NickNameInfoTable[m_ItemId]._Effect[i].usEffectIndex]->concept );	
							}
							break;
						}
						
						int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[i+1], m_MiniTooltipSize.cx-16 );
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
					}
				}				
			}
		}
		break;
	case MTMODE_BUFF_STUDYBOOK:
		{
			if( _XGameItem::GetItemName(m_ItemType, m_ItemId) )
			{
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );

				if( g_BookItemProperty[m_ItemId].cSecondType == 1 )	// 무공서
				{
					switch(g_BookItemProperty[m_ItemId].cThirdType) 
					{
					case 0 :	// 지정 무공서
						{
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
							
							strcpy( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							memset(tempBuf, 0, sizeof(TCHAR)*128);
							sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2031),g_SkillProperty[g_BookItemProperty[m_ItemId].usSkillID]->skillName );
							strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							memset(tempBuf, 0, sizeof(TCHAR)*128);
							sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
							strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						}
						break;
					case 1 :	// 문파 무공서
						{
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
							
							strcpy( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							memset(tempBuf, 0, sizeof(TCHAR)*128);
							if( g_BookItemProperty[m_ItemId].ucClan != 20 )
							{
								sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2034), g_JoinGroupName[g_BookItemProperty[m_ItemId].ucClan] );
							}
							else// 20이면 문파에 상관없이 공통으로 사용함.
							{
								sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2034), _XGETINTERFACETEXT(ID_STRING_NEW_3352) );
							}
							strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							memset(tempBuf, 0, sizeof(TCHAR)*128);
							sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
							strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						}
						break;
					case 2 :	// 역할 무공서
						{
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
							
							strcpy( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							if( g_BookItemProperty[m_ItemId].ucClan > 0 )
							{
								memset(tempBuf, 0, sizeof(TCHAR)*128);

								LPTSTR param1 = g_pClothesItemClanName[g_BookItemProperty[m_ItemId].ucClan];
                            #ifdef _XDEF_ITEM_V25 //Author : 양희왕 //breif : 2차전직도 추가 해야 될듯하다
                                LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][g_BookItemProperty[m_ItemId].ucCharacGrade];
                            #else
								LPTSTR param2 = g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][1];
                            #endif

								strcpy( tempBuf, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2219, param1, param2 ) );
								
								strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							}
							else if( g_BookItemProperty[m_ItemId].ucClan == 0 )
							{
								memset(tempBuf, 0, sizeof(TCHAR)*128);
                            #ifdef _XDEF_ITEM_V25 //Author : 양희왕 //breif : 2차전직도 추가 해야 될듯하다
                                sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2220), g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][g_BookItemProperty[m_ItemId].ucCharacGrade] );
                            #else
								sprintf(tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2220), g_CharacterRollNameTable[g_BookItemProperty[m_ItemId].ucClan][g_BookItemProperty[m_ItemId].ucClass][0] );
                            #endif
								strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							}				
							
							memset(tempBuf, 0, sizeof(TCHAR)*128);
							sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
							strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;				
						}
						break;
					case 3 :	// 계열 무공서
						{
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
							
							strcpy( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							memset(tempBuf, 0, sizeof(TCHAR)*128);

							LPTSTR param1 = g_pClothesItemClanName[g_BookItemProperty[m_ItemId].ucClan];
							LPTSTR param2 = g_SkillConceptProperty[g_BookItemProperty[m_ItemId].ucClan].stype_name[g_SkillProperty[g_BookItemProperty[m_ItemId].ucSkillClass]->skillSType];
							
							strcpy( tempBuf, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2040, param1, param2 ) );
							
							strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							memset(tempBuf, 0, sizeof(TCHAR)*128);
							sprintf( tempBuf, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2032), g_BookItemProperty[m_ItemId].ucValue );
							strcat( m_CurrentMiniTooltipInfo.LData_Buf, tempBuf );
							
							int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.LData_Buf, m_MiniTooltipSize.cx-16 );
							m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;
						}
						break;
					}
					if( m_TimeCount > 0 )
					{
						m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
						int count = 0;
						if( m_TimeCount > 3600 )	// 시간
						{
							count = m_TimeCount / 3600;
							int count2 = (m_TimeCount-3600*count) / 60;

							// %d시간 %분남음
							strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2221, &count, &count2 ) );
						}
						else
						{						
							if( m_TimeCount > 60 )
							{
								count = m_TimeCount / 60;							
								sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_REMAINMIN), count ); // %d분 남음
							}
							else
							{
								count = m_TimeCount;
								sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_MINITOOLTIP_REMAINSEC), count ); // %d초 남음
							}
						}
					}
				}
			}						
		}
		break;
	case MTMODE_BUFF_PEMODE :
		{
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
			strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2650) );		
	
			strcpy( m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_NEW_2651) );

			int length = g_XBaseFont->GetRowCount( m_CurrentMiniTooltipInfo.Data_Buf[0], m_MiniTooltipSize.cx-16 );
			m_MiniTooltipSize.cy += MT_ROW_INTERVAL*length;

			if( m_TimeCount > 0 )
			{
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				
				int count = m_TimeCount / 60000;
				if( count > 0 )
				{
					sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2194), count );
				}
				else
				{
					count = m_TimeCount % 60000;
					count /= 1000;
					sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2195), count );
				}			
			}
		}
		break;
	case MTMODE_ITEM_SHOUT :
		{
			if( _XGameItem::GetItemName(m_ItemType, m_ItemId) )
			{
				m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );

				if( m_TimeCount > 0 )
				{
					m_MiniTooltipSize.cy += MT_ROW_INTERVAL;
					
					int count = m_TimeCount / 60000;
					if( count > 0 )
					{
						sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2194), count );
					}
					else
					{
						count = m_TimeCount % 60000;
						count /= 1000;
						sprintf(m_CurrentMiniTooltipInfo.Data_Buf[0], _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2195), count );
					}
				}
			}
		}
		break;
	case MTMODE_PK_RECORD :
		{
			if( g_pPKRecord_Window )
			{
				if( m_ItemType == 0 )	// 승전
				{								
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2652) );	

					int param1 = g_pPKRecord_Window->m_WinRecord[m_ItemId].year+2006;
					int param2 = g_pPKRecord_Window->m_WinRecord[m_ItemId].month;
					int param3 = g_pPKRecord_Window->m_WinRecord[m_ItemId].day;
					int param4 = g_pPKRecord_Window->m_WinRecord[m_ItemId].hour;
					int param5 = g_pPKRecord_Window->m_WinRecord[m_ItemId].minute;

					strcpy(m_CurrentMiniTooltipInfo.Data_Buf[0], 
						   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2654, &param1, &param2, &param3, &param4, &param5 ) );
				}
				else	// 패전
				{
					strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2653) );	
					
					int param1 = g_pPKRecord_Window->m_LoseRecord[m_ItemId].year+2006;
					int param2 = g_pPKRecord_Window->m_LoseRecord[m_ItemId].month;
					int param3 = g_pPKRecord_Window->m_LoseRecord[m_ItemId].day;
					int param4 = g_pPKRecord_Window->m_LoseRecord[m_ItemId].hour;
					int param5 = g_pPKRecord_Window->m_LoseRecord[m_ItemId].minute;
					
					strcpy(m_CurrentMiniTooltipInfo.Data_Buf[0], 
						   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2654, &param1, &param2, &param3, &param4, &param5 ) );
				}
			}			
		}
		break;
	case MTMODE_PK_TRACINGSERVICES:
		{

		}
		break;
	case MTMODE_VIETNAMPAYMENTINFO :
		{

		}
		break;
	case MTMODE_PAYMENTINFO :
	case MTMODE_PCRPREMIUM :
		{	

		}
		break;
	case MTMODE_ITEM_ACCOUNT :
		{
			if(m_ItemId > 0)
			{
				strcpy( m_CurrentMiniTooltipInfo.NameBuf, _XGameItem::GetItemName(m_ItemType, m_ItemId) );
			}
		}
		break;
	case MTMODE_VIEWGROUP_CASTLEINFO :
		{
			TCHAR tempbuf[128];
			memset(tempbuf, 0, sizeof(tempbuf));

			strcpy(m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_NEW_3229));		//장원
			sprintf(tempbuf, _XGETINTERFACETEXT(ID_STRING_NEW_3206), m_ItemId+1);		// %d 단계
			strcat(m_CurrentMiniTooltipInfo.NameBuf, " ");
			strcat(m_CurrentMiniTooltipInfo.NameBuf, tempbuf);

			memset(tempbuf, 0, sizeof(tempbuf));
			sprintf(tempbuf, ":%.1f%%", g_CBManager.GetDefaultTax(m_ItemType, m_ItemId));
			sprintf(m_CurrentMiniTooltipInfo.LData_Buf, _XGETINTERFACETEXT(ID_STRING_NEW_3266));	// 세율
			strcat(m_CurrentMiniTooltipInfo.LData_Buf, tempbuf);

			strcpy(m_CurrentMiniTooltipInfo.Data_Buf[0], "+");

			memset(tempbuf, 0, sizeof(tempbuf));
			sprintf(tempbuf, "%.1f%%", g_CBManager.GetAdditiveTax(m_ItemType, m_ItemId));
			sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], _XGETINTERFACETEXT(ID_STRING_NEW_3267));			//추가
			strcat(m_CurrentMiniTooltipInfo.Data_Buf[1], tempbuf);
		}
		break;
	case MTMODE_CASTLEINFO :
		{
			TCHAR tempbuf[256];

			sprintf(m_CurrentMiniTooltipInfo.NameBuf, _XGETINTERFACETEXT(ID_STRING_NEW_3234));//장원 정보

			_XCastleInfo* pCastleInfo = g_CBManager.GetCastleInfo(g_CurrentZoneInfoIndex);
			if(pCastleInfo)
			{
				TCHAR namebuf[256];
				memset(namebuf, 0, sizeof(namebuf));

				if(pCastleInfo->m_tOwner.nOrIndex == 0)
				{
					sprintf(namebuf, _XGETINTERFACETEXT(g_CBManager.GetMobMasterNameIndex(g_CurrentZoneInfoIndex)));
				}
				else
				{
					if(pCastleInfo->m_tOwner.nType == 0)
						sprintf(namebuf, _XGETINTERFACETEXT(ID_STRING_NEW_3634), pCastleInfo->m_tOwner.szOrgName);//%s단
					else if(pCastleInfo->m_tOwner.nType == 1)
						sprintf(namebuf, _XGETINTERFACETEXT(ID_STRING_NEW_3125), pCastleInfo->m_tOwner.szOrgName);//%s회
				}

				m_ItemId = pCastleInfo->m_tOwner.nGroupOfMaster;

				memset(tempbuf, 0, sizeof(tempbuf));
				sprintf(tempbuf, "%s :    %s", _XGETINTERFACETEXT(ID_STRING_NEW_3200), namebuf);	// 소유자
				strncpy(m_CurrentMiniTooltipInfo.Data_Buf[0], tempbuf, sizeof(m_CurrentMiniTooltipInfo.Data_Buf[0]));

				// 소유일
				memset(tempbuf, 0, sizeof(tempbuf));
				int year = pCastleInfo->m_tPropertyDate.dwYear + 2000;
				int month = pCastleInfo->m_tPropertyDate.dwMonth;
				int day = pCastleInfo->m_tPropertyDate.dwDay;

				sprintf(tempbuf, g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2102,
					&year, &month, &day));
				sprintf(m_CurrentMiniTooltipInfo.LData_Buf, "%s : %s", _XGETINTERFACETEXT(ID_STRING_NEW_3201), tempbuf);

				// 단계
				memset(tempbuf, 0, sizeof(tempbuf));
				sprintf(tempbuf, _XGETINTERFACETEXT(ID_STRING_NEW_3206), pCastleInfo->m_nStep+1);
				sprintf(m_CurrentMiniTooltipInfo.Data_Buf[1], "%s : %s", _XGETINTERFACETEXT(ID_STRING_NEW_3202), tempbuf);

				// 경험치 안내문
				if(pCastleInfo->m_tOwner.nOrIndex != 0)
				{
					memset(tempbuf, 0, sizeof(tempbuf));
					if(pCastleInfo->m_tOwner.nType == 0)
					{
						sprintf(tempbuf, _XGETINTERFACETEXT(ID_STRING_NEW_3771), pCastleInfo->m_tOwner.szOrgName);//%s단 세력은 경험치 증가 혜택을 받을 수 있습니다.
					}
					else
					{
						sprintf(tempbuf, _XGETINTERFACETEXT(ID_STRING_NEW_3772), pCastleInfo->m_tOwner.szOrgName);//%s회 세력은 경험치 증가 혜택을 받을 수 있습니다.
					}

					strncpy(m_CurrentMiniTooltipInfo.Data_Buf[2], tempbuf, sizeof(m_CurrentMiniTooltipInfo.Data_Buf[2]));

					int length = g_XBaseFont->GetRowCount(m_CurrentMiniTooltipInfo.Data_Buf[2], m_MiniTooltipSize.cx-16);
					if(length > 0)
					{
						m_MiniTooltipSize.cy += (MT_ROW_INTERVAL*length);
						m_MiniTooltipSize.cy += MT_LINE_INTERVAL;
						SetWindowSize(m_MiniTooltipSize);
					}
				}
			}
		}
		break;
	case MTMODE_NOTORIOUS ://Author : 양희왕 //breif: 평판 시스템
		{
			if( g_pLocalUser->GetNotorious() >= 0 && g_pLocalUser->GetNotorious() <= 4)
			{
			}
			else if( g_pLocalUser->GetNotorious() >= 5 && g_pLocalUser->GetNotorious() <= 9)
			{
			}
			else if( g_pLocalUser->GetNotorious() >= 10 && g_pLocalUser->GetNotorious() <= 14)
			{
			}
			else if( g_pLocalUser->GetNotorious() >= 15 && g_pLocalUser->GetNotorious() <= 19)
			{
			}
			else if( g_pLocalUser->GetNotorious() >= 20 && g_pLocalUser->GetNotorious() <= 24)
			{
			}
			else if( g_pLocalUser->GetNotorious() >= 25 && g_pLocalUser->GetNotorious() <= 30)
			{
			}
		}
		break;
	default:
		return FALSE;		
	}

	return TRUE;
}

void _XWindow_MiniTooltip::DrawVietnamPaymentMethod()
{
	m_CurrentDrawPosY = 6;

	switch( g_LocalUserBillingInformations.BillMethod )
	{
	default:
	case _VNBILLING_BILLMETHOD_FREEOFCHARGE					:
	case _VNBILLING_BILLMETHOD_FREEOFCHARGE_EVENT			:
		{
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2222) );
			
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			m_CurrentDrawPosY += 20;
			
			int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
			int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
			
			if( remainhour > 0 )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
									   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
			}
			else if( remainminute > 0 )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
			}
			else
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
			}
		}		
		break;
	case _VNBILLING_BILLMETHOD_BASIC_FIXEDAMOUNTSYSTEM		:
		{
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2227) );
			
			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2230) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2231) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
			
			m_CurrentDrawPosY += 20;

			if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
			{
				int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
				int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
				
				if( remainhour > 0 )
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
										   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
				}
				else if( remainminute > 0 )
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
				}
				else
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
				}
			}
			else
			{	
				int param1 = g_LocalUserBillingInformations.BillExpireYear;
				int param2 = g_LocalUserBillingInformations.BillExpireMonth;
				int param3 = g_LocalUserBillingInformations.BillExpireDay;
				
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
									   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );
				
			}
		}		
		break;
	case _VNBILLING_BILLMETHOD_BASIC_FIXEDQUANTITYSYSTEM	:		
		{
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2227) );
			
			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2230) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2231) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
			
			m_CurrentDrawPosY += 20;

			if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
			{
				int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
				int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
				
				if( remainhour > 0 )
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
										   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
				}
				else if( remainminute > 0 )
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
				}
				else
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
				}
			}
			else
			{	
				int param1 = g_LocalUserBillingInformations.BillExpireYear;
				int param2 = g_LocalUserBillingInformations.BillExpireMonth;
				int param3 = g_LocalUserBillingInformations.BillExpireDay;
				
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
									   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );
				
			}

		}
		break;
	case _VNBILLING_BILLMETHOD_PREMIUM_FIXEDAMOUNTSYSTEM	:		
		{
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2234) );
			
			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2235) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2237) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2238) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2239) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2240) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
			
			m_CurrentDrawPosY += 20;
			
			if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
			{
				int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
				int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

				if( remainhour > 0 )
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
										   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
				}
				else if( remainminute > 0 )
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
				}
				else
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
				}
			}
			else
			{	
				int param1 = g_LocalUserBillingInformations.BillExpireYear;
				int param2 = g_LocalUserBillingInformations.BillExpireMonth;
				int param3 = g_LocalUserBillingInformations.BillExpireDay;
				
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
									   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );		
			}
		}
		break;
	case _VNBILLING_BILLMETHOD_PREMIUM_FIXEDQUANTITYSYSTEM	:		
		{
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2234) );
			
			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2235) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2237) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2238) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2239) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2240) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
			
			m_CurrentDrawPosY += 20;
			
			if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
			{
				int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
				int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

				if( remainhour > 0 )
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
										   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
				}
				else if( remainminute > 0 )
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
				}
				else
				{
					g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
				}
			}
			else
			{	
				int param1 = g_LocalUserBillingInformations.BillExpireYear;
				int param2 = g_LocalUserBillingInformations.BillExpireMonth;
				int param3 = g_LocalUserBillingInformations.BillExpireDay;
				
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
									   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );		
			}
		}
		break;
	}

	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawPaymentInfo()
{

#ifdef _XDEF_SHUTDOWN_20061027	// 베트남 셧다운 

	//Author : 양희왕
	m_CurrentDrawPosY = 6;

	int playelapsedtime = 0;
	
	if( g_PlayLeftTime > 0 )
		playelapsedtime = g_PlayLeftTime - (g_LocalSystemTime - g_PlayTime);

	int	state = 0;
 #ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
	if(g_pJinStatus_Window)
	{
		state = g_pJinStatus_Window->GetSDSState();
	}
 #endif
	
	if(state == 0 || state == 2)
	{
		if( playelapsedtime >= 7200000 ) // 5시간 미만인 경우
		{		
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3150) );
			
			int remainhour = (playelapsedtime / 3600000);
			int remainminute = (playelapsedtime % 3600000) / 60000;
			
			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			if( remainhour > 0 )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
					g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
			}
			else if( remainminute > 0 )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT,
					_XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
			}
			else
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT,
					_XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), playelapsedtime / 1000 );
			}		
			m_CurrentDrawPosY += 20;

			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3151) );

		}
		else if( playelapsedtime < 7200000 && playelapsedtime > 0  )// 2시간 미만인 경우
		{
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3150) );
			
			int remainhour = (playelapsedtime / 3600000);
			int remainminute = (playelapsedtime % 3600000) / 60000;
			
			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			if( remainhour > 0 )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
					g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
			}
			else if( remainminute > 0 )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT,
					_XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
			}
			else
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT,
					_XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), playelapsedtime / 1000 );
			}		
			m_CurrentDrawPosY += 20;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3152) );
			
			m_CurrentDrawPosY += 20;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT,_XGETINTERFACETEXT(ID_STRING_NEW_3153) );
			
			m_CurrentDrawPosY += 20;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3154) );
			m_CurrentDrawPosY += 14;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3155) );
		}
		else if( playelapsedtime <= 0 )// 0시간 미만인 경우
		{
			g_XBaseFont->SetColor( 0xFFFFA243 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3150) );
			
			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );
			playelapsedtime = 0;
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT,
				_XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), playelapsedtime );
		
			m_CurrentDrawPosY += 20;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3152) );
			
			m_CurrentDrawPosY += 20;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3153) );
			
			m_CurrentDrawPosY += 20;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3156) );
			m_CurrentDrawPosY += 14;		
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3157) );

		}
	}
	else if(state == 1)
	{
		g_XBaseFont->SetColor( 0xFFFFA243 );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3150) );
		
/*		m_CurrentDrawPosY += 16;
		playelapsedtime = 0;
		g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT,
			_XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), playelapsedtime );
*/	
		g_XBaseFont->SetColor( 0xFFFFFFAF );
		m_CurrentDrawPosY += 20;		
		g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3152) );
		
		m_CurrentDrawPosY += 20;		
		g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3153) );
		
		m_CurrentDrawPosY += 20;		
		g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3156) );
		m_CurrentDrawPosY += 14;		
		g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3157) );
	}

	g_XBaseFont->Flush();

#endif

}

void _XWindow_MiniTooltip::DrawPCRPremium()
{
	m_CurrentDrawPosY = 6;

	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3343) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3344) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3345) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3347) );

	//Author : 양희왕 breif: PC 방 날짜 제거  date: 07/09/05

	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawPKRecord()
{
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
	{
		g_XBaseFont->SetColor( 0xFFFFF200 );
		m_CurrentDrawPosY = 12;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[0] ) ) 
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY += 20;
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.Data_Buf[0] );
	}
	
	g_XBaseFont->Flush();	
}

//Author : 양희왕
void _XWindow_MiniTooltip::DrawPkTracingservices()
{
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPk_TracingServicesWindow = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	_XWindow_WorldMinimap* pMiniMap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
	
	if( !pPk_TracingServicesWindow && !pMiniMap_Window )
		return;

	//칠해지는 바
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, 
					m_WindowPosition.x + 6 + 177, m_WindowPosition.y + 6 + 21, 
					D3DCOLOR_ARGB(255,4,4,4) );
	//사각형 라인 그리기
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, 
					m_WindowPosition.x + 6 + 177, m_WindowPosition.y + 6 + 21, 
					0.0f, D3DCOLOR_ARGB(255, 51, 51, 51) );

	_XDrawSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 32, 
					m_WindowPosition.x + 6 + 177, m_WindowPosition.y + 32 + 18, 
					D3DCOLOR_ARGB(255,41,41,41) );
	_XDrawRectAngle( m_WindowPosition.x + 6, m_WindowPosition.y + 6, 
					m_WindowPosition.x + 6 + 177, m_WindowPosition.y + 32 + 18, 
					0.0f, D3DCOLOR_ARGB(255, 85, 85, 85) );

	_XDrawSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 116, 
					m_WindowPosition.x + 6 + 177, m_WindowPosition.y + 116 + 18, 
					D3DCOLOR_ARGB(255,4,4,4) );
	_XDrawRectAngle( m_WindowPosition.x + 6, m_WindowPosition.y + 116, 
					m_WindowPosition.x + 6 + 177, m_WindowPosition.y + 116 + 18, 
					0.0f, D3DCOLOR_ARGB(255, 51, 51, 51) );
	
	DWORD dwTempHourTime = 0;
	DWORD dwTempMinTime = 0;
	DWORD dwTempSecTime = 0;
	bool bSearch = false;
	list<_XPKTRACING_POSITION_INFO*>::iterator iter = pMiniMap_Window->m_listPKTracingPosition.begin();

	m_CurrentDrawPosY = 12;

	//위치보고 메시지
	g_XBaseFont->SetColor( 0xFF66CC66 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x + 12, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3458));// _T("위치 보고")

	
	//케릭터 추적 중
	m_CurrentDrawPosY += 24;

	LPCTSTR pString = pPk_TracingServicesWindow->GetListBox().GetItemText( m_ItemId-1, 1 ); // 케릭터 이름
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));

	if(pString)
		sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3459), pString );	//_T("%s 추적 중...")
	else
		sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3460));	// _T("추적 중...")
	
	g_XBaseFont->SetColor( 0xFFC0C0C0 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x + 12, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, messagestring );
	
	for(; iter != pMiniMap_Window->m_listPKTracingPosition.end(); ++iter )
	{
		if( (*iter)->ucPKType == 2 || (*iter)->ucPKType == 3)
		{
			if( !stricmp( (*iter)->szCharacName, pString ) )
			{
				bSearch = true;
				m_CurrentDrawPosY += 20;
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3461), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zoneshortname);	//_T("%s 지역에서 흔적 발견.")
				g_XBaseFont->PutsAlign(m_WindowPosition.x + 12, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, messagestring );
				
				m_CurrentDrawPosY += 14;
				memset(messagestring, 0, sizeof(messagestring));
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3462));	//_T("상세 위치 정보가 지도에표시 됩니다.")
				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x + 12, m_WindowPosition.y + m_CurrentDrawPosY, 
												messagestring, 142, _XFONT_ALIGNTYPE_LEFT, 1.0f, 2);
			}
		}
	}

	if( !bSearch )
	{
		m_CurrentDrawPosY += 20;
		sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3463));	//_T("행방을 알 수 없음.")
		g_XBaseFont->PutsAlign(m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, messagestring );
	}
	
	pString = pPk_TracingServicesWindow->GetListBox().GetItemText( m_ItemId-1, 2 ); //시간

	if(pString)
	{
		memset(messagestring, 0, sizeof(messagestring));
		strcpy( messagestring, pString );
		g_XBaseFont->SetColor( 0xFFFFCC66 );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 12, m_WindowPosition.y + ( m_MiniTooltipSize.cy - 22 ), _XFONT_ALIGNTYPE_LEFT, messagestring );
	}
	
	g_XBaseFont->Flush();
#endif
}
//Last Updated : 07/04/03

void _XWindow_MiniTooltip::DrawAccountItemUse()
{
	m_CurrentDrawPosY = 6;

	g_XBaseFont->SetColor( 0xFFFFA243 );
	
	if(strlen(m_CurrentMiniTooltipInfo.NameBuf))
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
		m_CurrentDrawPosY += 16;
	}

	switch(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi)
	{
	case 1 :
		{
			// 경험치 20% 증가
//			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _T("[유료 쿠폰 효과]"));

//			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2235) );
		
			m_CurrentDrawPosY += 20;
			
			int param1 = g_pLocalUser->m_etYear+2000;
			int param2 = g_pLocalUser->m_etMonth;
			int param3 = g_pLocalUser->m_etDay;
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );
			
			#ifdef _XDWDEBUG
			m_CurrentDrawPosY += 16;
			g_XBaseFont->Print(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, "%dH %dM %dS", g_pLocalUser->m_etHour, g_pLocalUser->m_etMinute, g_pLocalUser->m_etSecond);
			#endif
		}
		break;
	case 2 :
		{
			// 숙련도 50% 증가
//			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _T("[유료 쿠폰 효과]"));

//			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
			
			m_CurrentDrawPosY += 20;
			
			int param1 = g_pLocalUser->m_etYear+2000;
			int param2 = g_pLocalUser->m_etMonth;
			int param3 = g_pLocalUser->m_etDay;
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );

			#ifdef _XDWDEBUG
			m_CurrentDrawPosY += 16;
			g_XBaseFont->Print(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, "%dH %dM %dS", g_pLocalUser->m_etHour, g_pLocalUser->m_etMinute, g_pLocalUser->m_etSecond);
			#endif
		}
		break;
	case 3 :
		{
			// 경험치 20% + 숙련도 50% 증가
//			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _T("[유료 쿠폰 효과]"));

//			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2235) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
						
			m_CurrentDrawPosY += 20;
			
			int param1 = g_pLocalUser->m_etYear+2000;
			int param2 = g_pLocalUser->m_etMonth;
			int param3 = g_pLocalUser->m_etDay;
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );

			#ifdef _XDWDEBUG
			m_CurrentDrawPosY += 16;
			g_XBaseFont->Print(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, "%dH %dM %dS", g_pLocalUser->m_etHour, g_pLocalUser->m_etMinute, g_pLocalUser->m_etSecond);
			#endif
		}
		break;
	case 4 :
		{
			// 구룡 기행
//			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _T("[구룡 천하]"));

//			m_CurrentDrawPosY += 16;
			g_XBaseFont->SetColor( 0xFFFFFFAF );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2235) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2237) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
            m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
        #if defined(_XTAIWANESE)
            TCHAR SectionLineString[128];
            memset( SectionLineString, 0, sizeof( SectionLineString ));

            for( int i = 0; i < 34; i++ )
            {
                SectionLineString[i] = -94;
                i += 1;
                SectionLineString[i] = 119;
            }
            m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x + 4, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, SectionLineString);//_T("─────────────────") );
        #endif
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2239) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2240) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2238) );

			//breif : 베트남도 추가 Date : 08/03/11
			//Author : 양희왕, 미국 구룡천하 내용 추가
			#if defined(_XVIETNAMESE) || defined(_XENGLISH)	|| defined(_XRUSSIAN)
			//if( m_ItemId == 1053 ) // 90일권
			//7일, 30일
			TCHAR tempString[128];
			memset( tempString, 0, sizeof( tempString ));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3786), 3 ); //  - %d%% Critical Damage	// 미국만 사용.

			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

			memset( tempString, 0, sizeof( tempString ));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3787), 5 ); //  - %d%% Attack Rating	// 미국만 사용.

			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

			memset( tempString, 0, sizeof( tempString ));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3788), 5 ); //  - %d%% Chi Kung Defense // 미국만 사용.

			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
			#endif
			
			#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3677) );
			#endif
			
			m_CurrentDrawPosY += 20;

			int param1 = g_pLocalUser->m_etYear+2000;
			int param2 = g_pLocalUser->m_etMonth;
			int param3 = g_pLocalUser->m_etDay;
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );

			#ifdef _XDWDEBUG
			m_CurrentDrawPosY += 16;
			g_XBaseFont->Print(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, "%dH %dM %dS", g_pLocalUser->m_etHour, g_pLocalUser->m_etMinute, g_pLocalUser->m_etSecond);
			#endif
		}
		break;
	case 5 :
		{
			// 구룡천하(가칭) - 베트남용
			g_XBaseFont->SetColor( 0xFFFFFFAF );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3406) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3407) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3408) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3409) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3410) );
        #if defined(_XTAIWANESE)
            TCHAR SectionLineString[128];
            memset( SectionLineString, 0, sizeof( SectionLineString ));

            for( int i = 0; i < 34; i++ )
            {
                SectionLineString[i] = -94;
                i += 1;
                SectionLineString[i] = 119;
            }
            m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x + 4, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, SectionLineString);//_T("─────────────────") );
        #endif
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3411) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3412) );
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3413) );

			//breif : 베트남도 추가 Date : 08/03/11
			//Author : 양희왕, 미국 구룡천하 내용 추가
			#if defined(_XVIETNAMESE) || defined(_XENGLISH)	|| defined(_XRUSSIAN)
			// 90일권
			TCHAR tempString[128];
			memset( tempString, 0, sizeof( tempString ));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3786), 5 ); //  - %d%% Critical Damage	// 미국만 사용.

			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

			memset( tempString, 0, sizeof( tempString ));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3787), 10 ); //  - %d%% Attack Rating	// 미국만 사용.

			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

			memset( tempString, 0, sizeof( tempString ));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3788), 10 ); //  - %d%% Chi Kung Defense // 미국만 사용.

			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
			#endif

			#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3677) );
			#endif
			
			m_CurrentDrawPosY += 20;

			int param1 = g_pLocalUser->m_etYear+2000;
			int param2 = g_pLocalUser->m_etMonth;
			int param3 = g_pLocalUser->m_etDay;
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );
		}
		break;
	case 6 : //Author : 양희왕 //breif : 구룡천하 심기 6 추가
		{
			g_XBaseFont->SetColor( 0xFFFFFFAF );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2235) ); //ㆍ경험치 20% 증가
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) ); //ㆍ숙련도 50% 증가
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2237) ); //ㆍ개인 프리미엄 전용 점혈술 제공
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) ); //ㆍ별호 변경 대기 시간 없음
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) ); //ㆍ부활시 경험치 하락 감소
        #if defined(_XTAIWANESE)
            TCHAR SectionLineString[128];
            memset( SectionLineString, 0, sizeof( SectionLineString ));

            for( int i = 0; i < 34; i++ )
            {
                SectionLineString[i] = -94;
                i += 1;
                SectionLineString[i] = 119;
            }
            m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x + 4, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, SectionLineString);//_T("─────────────────") );
        #endif
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2239) ); //  - 제자리 부활 : 15%->5%
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2240) ); //  - 여점 부활 : 5%->2%

			TCHAR tempString[256];
			memset( tempString, 0, sizeof(tempString));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3808), 5 ); //- 오기 %d 증가
			
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString); 
			
        #ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
			m_CurrentDrawPosY += 14;
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3677) ); //SDS 타임 설정 기능 제공
        #endif
			
			m_CurrentDrawPosY += 20;

			int param1 = g_pLocalUser->m_etYear+2000;
			int param2 = g_pLocalUser->m_etMonth;
			int param3 = g_pLocalUser->m_etDay;
			
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );

        #ifdef _XDWDEBUG
			m_CurrentDrawPosY += 16;
			g_XBaseFont->Print(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, "%dH %dM %dS", g_pLocalUser->m_etHour, g_pLocalUser->m_etMinute, g_pLocalUser->m_etSecond);
        #endif
		}
		break;
	}

	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawPaymentMethod_NormalUser( void )
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2222) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2223) );
	m_CurrentDrawPosY += 20;
	
	int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
	int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
	
	if( remainhour > 0 )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
	}
	else if( remainminute > 0 )
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
	}
	else
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
	}
}

void _XWindow_MiniTooltip::DrawPaymentMethod_Premium1User( void )
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2227) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2228) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2230) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2231) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
	
	m_CurrentDrawPosY += 20;

	if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
	{
		int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
		int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
		
		if( remainhour > 0 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
		}
		else if( remainminute > 0 )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
		}
	}
	else
	{	
		int param1 = g_LocalUserBillingInformations.BillExpireYear;
		int param2 = g_LocalUserBillingInformations.BillExpireMonth;
		int param3 = g_LocalUserBillingInformations.BillExpireDay;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3 ) );
		
	}
}

void _XWindow_MiniTooltip::DrawPaymentMethod_Premium2User( void )
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2234) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2228) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2235) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2237) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2238) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2239) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2240) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
	
	m_CurrentDrawPosY += 20;
	
	if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
	{
		int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
		int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

		if( remainhour > 0 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
		}
		else if( remainminute > 0 )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
		}
	}
	else
	{	
		int param1 = g_LocalUserBillingInformations.BillExpireYear;
		int param2 = g_LocalUserBillingInformations.BillExpireMonth;
		int param3 = g_LocalUserBillingInformations.BillExpireDay;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );		
	}
}

void _XWindow_MiniTooltip::DrawPaymentMethod_NormalPCRoom( void )
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2199) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2241) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2242) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2243) );
}

void _XWindow_MiniTooltip::DrawPaymentMethod_PremiumPCRoom( void )
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2198) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2228) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2244) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2245) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2246) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2247) );

	m_CurrentDrawPosY += 20;
	
	if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
	{
		int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
		int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

		if( remainhour > 0 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
		}
		else if( remainminute > 0 )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
		}
	}
	else
	{	
		int param1 = g_LocalUserBillingInformations.PCRoomBillExpireYear;
		int param2 = g_LocalUserBillingInformations.PCRoomBillExpireMonth;
		int param3 = g_LocalUserBillingInformations.PCRoomBillExpireDay;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );
	}
}

void _XWindow_MiniTooltip::DrawPaymentMethod_PremiumUser1AndPCRoom( void )
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2248) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2228) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2244) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2245) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2246) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2230) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2231) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2247) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );

	m_CurrentDrawPosY += 20;
	
	if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
	{
		int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
		int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

		if( remainhour > 0 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
		}
		else if( remainminute > 0 )
		{	
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
		}
	}
	else
	{	
		int param1 = g_LocalUserBillingInformations.BillExpireYear;
		int param2 = g_LocalUserBillingInformations.BillExpireMonth;
		int param3 = g_LocalUserBillingInformations.BillExpireDay;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );
		
	}
}

void _XWindow_MiniTooltip::DrawPaymentMethod_PremiumUser2AndPCRoom( void )
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2250) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2228) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2244) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2237) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2238) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2245) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2246) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2239) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2240) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2247) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
	
	m_CurrentDrawPosY += 20;
	
	if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
	{
		int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
		int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

		if( remainhour > 0 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
		}
		else if( remainminute > 0 )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
		}
	}
	else
	{	
		int param1 = g_LocalUserBillingInformations.BillExpireYear;
		int param2 = g_LocalUserBillingInformations.BillExpireMonth;
		int param3 = g_LocalUserBillingInformations.BillExpireDay;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );		
	}
}

void _XWindow_MiniTooltip::DrawPaymentMethod_PremiumUser1NormalPCRoom()
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2251) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2228) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2242) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2230) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2231) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2243) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
	
	m_CurrentDrawPosY += 20;
	
	if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
	{
		int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
		int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

		if( remainhour > 0 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
		}
		else if( remainminute > 0 )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
		}
	}
	else
	{	
		int param1 = g_LocalUserBillingInformations.BillExpireYear;
		int param2 = g_LocalUserBillingInformations.BillExpireMonth;
		int param3 = g_LocalUserBillingInformations.BillExpireDay;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );
	}
}

void _XWindow_MiniTooltip::DrawPaymentMethod_PremiumUser2NormalPCRoom()
{
	g_XBaseFont->SetColor( 0xFFFFA243 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2253) );
	
	m_CurrentDrawPosY += 16;
	g_XBaseFont->SetColor( 0xFFFFFFAF );		
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2228) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2244) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2236) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2237) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2238) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2229) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2239) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2240) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2243) );
	m_CurrentDrawPosY += 14;
	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2232) );
	
	m_CurrentDrawPosY += 20;
	
	if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
	{
		int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
		int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

		if( remainhour > 0 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2224, &remainhour, &remainminute ) );
		}
		else if( remainminute > 0 )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), remainminute );
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), g_LocalUserBillingInformations.BillRemain / 1000 );
		}			
	}
	else
	{	
		int param1 = g_LocalUserBillingInformations.BillExpireYear;
		int param2 = g_LocalUserBillingInformations.BillExpireMonth;
		int param3 = g_LocalUserBillingInformations.BillExpireDay;
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_DEFAULTTOOLTIP_2233, &param1, &param2, &param3) );
	}
}

void _XWindow_MiniTooltip::DrawViewGroupCastleInfo()
{
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	if( strlen( m_CurrentMiniTooltipInfo.NameBuf ) ) 
	{
		g_XBaseFont->SetColor( D3DCOLOR_ARGB(255, 232, 179, 41) );
		m_CurrentDrawPosY = 12;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
	}
	
	if( strlen( m_CurrentMiniTooltipInfo.LData_Buf ) ) 
	{
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		m_CurrentDrawPosY += 25;
		g_XBaseFont->Puts(m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf);		
	}

	if( strlen( m_CurrentMiniTooltipInfo.Data_Buf[1] ) ) 
	{
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 243, 0));
		g_XBaseFont->Puts(m_WindowPosition.x + 75, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[0]);
		g_XBaseFont->Puts(m_WindowPosition.x + 85, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}
	
	g_XBaseFont->Flush();
}

void _XWindow_MiniTooltip::DrawCastleInfo()
{
	_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+m_MiniTooltipSize.cx-6, m_WindowPosition.y+28, 0.0f,D3DCOLOR_ARGB(255,52,52,52) );

	if(strlen(m_CurrentMiniTooltipInfo.NameBuf)) 
	{
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		m_CurrentDrawPosY = 12;
		g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y + m_CurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, m_CurrentMiniTooltipInfo.NameBuf);
		g_XBaseFont->Flush();
	}

	g_XBaseFont->SetBoldMode(FALSE);
	m_CurrentDrawPosY +=MT_LINE_INTERVAL;

	if(strlen(m_CurrentMiniTooltipInfo.Data_Buf[0])) 
	{
		g_XBaseFont->SetColor(0xFFFEFECC);
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts(m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[0]);
	}
	
	if(strlen(m_CurrentMiniTooltipInfo.LData_Buf)) 
	{
		g_XBaseFont->SetColor(0xFFFEFECC);
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts(m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.LData_Buf);		
	}

	if(strlen(m_CurrentMiniTooltipInfo.Data_Buf[1])) 
	{
		g_XBaseFont->SetColor(0xFFFEFECC);
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts(m_WindowPosition.x + 10, m_WindowPosition.y + m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[1]);
	}

	m_CurrentDrawPosY +=MT_LINE_INTERVAL;

	if(strlen(m_CurrentMiniTooltipInfo.Data_Buf[2]))
	{
		g_XBaseFont->SetColor(0xFFFEFECC);
		m_CurrentDrawPosY += MT_ROW_INTERVAL;
		g_XBaseFont->Puts_Separate(m_WindowPosition.x+10, m_WindowPosition.y+m_CurrentDrawPosY, m_CurrentMiniTooltipInfo.Data_Buf[2], 
			m_MiniTooltipSize.cx-16, 1.0, 3);
	}
	
	g_XBaseFont->Flush();

	RECT rect;
	_XWindow_CastleInfo* pCaslteInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
	if(pCaslteInfo_Window)
	{	
		pCaslteInfo_Window->GetIconRect(m_ItemId, rect);
		m_pGroupIcon->SetClipRect(rect);
		m_pGroupIcon->Draw(m_WindowPosition.x+65, m_WindowPosition.y+31);
	}
}

void _XWindow_MiniTooltip::DrawCharInfoNotorious()
{
}