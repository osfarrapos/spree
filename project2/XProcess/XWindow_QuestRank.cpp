// XWindow_QuestRank.cpp: implementation of the _XWindow_QuestRank class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_QuestRank.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_MonsterStatus.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_QuestRank::_XWindow_QuestRank()
{
	m_RankQuestID			= 0;
	m_RankQuestType1		= 0;
	m_RankQuestType2		= 0;

	m_bReceiveRankData		= FALSE;
	m_imageClanIcon				= NULL;

	memset( m_RankOrgName, 0,sizeof(TCHAR)*10*128 );
	memset( m_RankName, 0,sizeof(TCHAR)*10*128 );
	memset( m_RankLevel, 0,sizeof(TCHAR)*10*128 );
	memset( m_RankQuestData, 0,sizeof(TCHAR)*10*128 );
	memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*256 );
}

_XWindow_QuestRank::~_XWindow_QuestRank()
{

}

BOOL _XWindow_QuestRank::Initialize(void)
{
	int mainframeinterface		= g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	int npcresourceindex4		= g_MainInterfaceTextureArchive.FindResource("mi_npcquestwin_04.tga" );

	int pvpresourceindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_01.tga");
	}
	else
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pvp_01.tga");
	}
	
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

	m_BorderTitle.Create(m_WindowPosition.x+1, m_WindowPosition.y+18, m_WindowPosition.x+365, m_WindowPosition.y+268);
	m_BorderTitle.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTitle.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));

	FLOAT ufactor = 364.0f/128.0f;
	FLOAT vfactor = 250.0f/128.0f;

	m_BorderTitle.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);
	
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

	_XImageStatic* pDragonBorder_1 = new _XImageStatic;
	pDragonBorder_1->Create(5, 24, 181, 73, &g_MainInterfaceTextureArchive, npcresourceindex4);
	pDragonBorder_1->SetClipRect(4, 133, 180, 182);
	InsertChildObject(pDragonBorder_1);

	_XImageStatic* pDragonBorder_2 = new _XImageStatic;
	pDragonBorder_2->Create(357, 24, 180, 73, &g_MainInterfaceTextureArchive, npcresourceindex4);
	pDragonBorder_2->SetClipRect(4, 133, 180, 182);
	pDragonBorder_2->SetScale(-1.0f, 1.0f);
	InsertChildObject(pDragonBorder_2);

	_XImageStatic* pBorder_1 = new _XImageStatic;
	pBorder_1->Create(5, 75, 359, 75, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_1->SetClipRect(9, 193, 175, 194);
	pBorder_1->SetScale(2.138f, 1.0f);
	InsertChildObject(pBorder_1);

	_XImageStatic* pBorder_2 = new _XImageStatic;
	pBorder_2->Create(5, 256, 359, 256, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_2->SetClipRect(9, 193, 175, 194);
	pBorder_2->SetScale(2.138f, 1.0f);
	InsertChildObject(pBorder_2);

	_XImageStatic* pBorder_3 = new _XImageStatic;
	pBorder_3->Create(4, 76, 4, 255, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_3->SetClipRect(192, 64, 193, 230);
	pBorder_3->SetScale(1.0f, 1.084f);
	InsertChildObject(pBorder_3);

	_XImageStatic* pBorder_4 = new _XImageStatic;
	pBorder_4->Create(360, 76, 360, 255, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_4->SetClipRect(192, 64, 193, 230);
	pBorder_4->SetScale(1.0f, 1.084f);
	InsertChildObject(pBorder_4);
	
	_XImageStatic* pRankingBorder_1 = new _XImageStatic;
	pRankingBorder_1->Create(17, 48, 67, 66, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_1->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_1);
	
	_XImageStatic* pRankingBorder_2 = new _XImageStatic;
	pRankingBorder_2->Create(17, 78, 67, 96, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_2->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_2);
	
	_XImageStatic* pRankingBorder_3 = new _XImageStatic;
	pRankingBorder_3->Create(17, 98, 67, 116, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_3->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_3);
	
	_XImageStatic* pRankingBorder_4 = new _XImageStatic;
	pRankingBorder_4->Create(17, 118, 67, 136, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_4->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_4);
	
	_XImageStatic* pRankingBorder_5 = new _XImageStatic;
	pRankingBorder_5->Create(17, 138, 67, 156, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_5->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_5);
	
	_XImageStatic* pRankingBorder_6 = new _XImageStatic;
	pRankingBorder_6->Create(17, 158, 67, 176, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_6->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_6);
	
	_XImageStatic* pRankingBorder_7 = new _XImageStatic;
	pRankingBorder_7->Create(17, 178, 67, 196, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_7->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_7);
	
	_XImageStatic* pRankingBorder_8 = new _XImageStatic;
	pRankingBorder_8->Create(17, 198, 67, 216, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_8->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_8);
	
	_XImageStatic* pRankingBorder_9 = new _XImageStatic;
	pRankingBorder_9->Create(17, 218, 67, 236, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_9->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_9);
	
	_XImageStatic* pRankingBorder_10 = new _XImageStatic;
	pRankingBorder_10->Create(17, 238, 67, 256, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRankingBorder_10->SetClipRect(136, 69, 186, 85);
	InsertChildObject(pRankingBorder_10);
	
	
	m_imageClanIcon = new _XImageStatic;
	m_imageClanIcon->Create( 73, 49, 26, 40,&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mi_npcname2.tga" ) );	
	m_imageClanIcon->SetClipRect( 0, 0, 0, 0 );
	m_imageClanIcon->SetScale(0.6818f, 0.6818f);
	InsertChildObject( m_imageClanIcon );	

	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_QuestRank::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_QuestRank::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTitle.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow) 
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	if( m_bReceiveRankData )
	{
		if( m_SelectedQuestSubName[0] != 0 )
		{
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 
			g_XBaseFont->PutsAlign(m_WindowPosition.x+183, m_WindowPosition.y+25, _XFONT_ALIGNTYPE_CENTER, m_SelectedQuestSubName);
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);
		}
		
		if( m_RankQuestType1 != 2 )
		{
			m_imageClanIcon->SetClipRect( g_MobTargetIconPos[m_indexClan[0]].left, g_MobTargetIconPos[m_indexClan[0]].top, 
				g_MobTargetIconPos[m_indexClan[0]].right, g_MobTargetIconPos[m_indexClan[0]].bottom );
			m_imageClanIcon->SetScale(0.6818f, 0.6818f);
			m_imageClanIcon->Draw(m_WindowPosition.x+73, m_WindowPosition.y+49);
			
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
			g_XBaseFont->PutsAlign( m_WindowPosition.x+132, m_WindowPosition.y+51, _XFONT_ALIGNTYPE_CENTER, m_RankName[0] );
			g_XBaseFont->PutsAlign( m_WindowPosition.x+235, m_WindowPosition.y+51, _XFONT_ALIGNTYPE_CENTER, m_RankLevel[0] );
			g_XBaseFont->PutsAlign( m_WindowPosition.x+320, m_WindowPosition.y+51, _XFONT_ALIGNTYPE_CENTER, m_RankQuestData[0] );
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);

			g_XBaseFont->SetColor( _XSC_DEFAULT );
			
			for(int i = 1; i < 10; i++)
			{
				m_imageClanIcon->SetClipRect( g_MobTargetIconPos[m_indexClan[i]].left, g_MobTargetIconPos[m_indexClan[i]].top, 
					g_MobTargetIconPos[m_indexClan[i]].right, g_MobTargetIconPos[m_indexClan[i]].bottom );
				m_imageClanIcon->SetScale(0.6818f, 0.6818f);
				m_imageClanIcon->Draw(m_WindowPosition.x+73, m_WindowPosition.y+78+(20*(i-1)));

				g_XBaseFont->PutsAlign( m_WindowPosition.x+132, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankName[i] );
				g_XBaseFont->PutsAlign( m_WindowPosition.x+235, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankLevel[i] );
				g_XBaseFont->PutsAlign( m_WindowPosition.x+320, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankQuestData[i] );
			}
		}
		else
		{
			m_imageClanIcon->SetClipRect( g_MobTargetIconPos[m_indexClan[0]].left, g_MobTargetIconPos[m_indexClan[0]].top, 
				g_MobTargetIconPos[m_indexClan[0]].right, g_MobTargetIconPos[m_indexClan[0]].bottom );
			m_imageClanIcon->SetScale(0.6818f, 0.6818f);
			m_imageClanIcon->Draw(m_WindowPosition.x+73, m_WindowPosition.y+47);
			
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 
			g_XBaseFont->PutsAlign( m_WindowPosition.x+127, m_WindowPosition.y+51, _XFONT_ALIGNTYPE_CENTER, m_RankOrgName[0] );
			
			g_XBaseFont->SetColor( _XSC_INFORMATION );
			g_XBaseFont->PutsAlign( m_WindowPosition.x+132, m_WindowPosition.y+51, _XFONT_ALIGNTYPE_CENTER, m_RankName[0] );
			
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->PutsAlign( m_WindowPosition.x+235, m_WindowPosition.y+51, _XFONT_ALIGNTYPE_CENTER, m_RankLevel[0] );
			g_XBaseFont->PutsAlign( m_WindowPosition.x+320, m_WindowPosition.y+51, _XFONT_ALIGNTYPE_CENTER, m_RankQuestData[0] );
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);
			
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				for(int i = 1; i < 10; i++)
				{
					m_imageClanIcon->SetClipRect( g_MobTargetIconPos[m_indexClan[i]].left, g_MobTargetIconPos[m_indexClan[i]].top, 
						g_MobTargetIconPos[m_indexClan[i]].right, g_MobTargetIconPos[m_indexClan[i]].bottom );
					m_imageClanIcon->SetScale(0.6818f, 0.6818f);
					m_imageClanIcon->Draw(m_WindowPosition.x+73, m_WindowPosition.y+78+(20*(i-1)));
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 
					g_XBaseFont->PutsAlign( m_WindowPosition.x+115, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankOrgName[i] );
					
					g_XBaseFont->SetColor( _XSC_INFORMATION );
					g_XBaseFont->PutsAlign( m_WindowPosition.x+208, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankName[i] );
					
					g_XBaseFont->SetColor( _XSC_DEFAULT );
					g_XBaseFont->PutsAlign( m_WindowPosition.x+267, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankLevel[i] );
					g_XBaseFont->PutsAlign( m_WindowPosition.x+320, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankQuestData[i] );
				}
			}
			else
			{
				for(int i = 1; i < 10; i++)
				{
					m_imageClanIcon->SetClipRect( g_MobTargetIconPos[m_indexClan[i]].left, g_MobTargetIconPos[m_indexClan[i]].top, 
						g_MobTargetIconPos[m_indexClan[i]].right, g_MobTargetIconPos[m_indexClan[i]].bottom );
					m_imageClanIcon->SetScale(0.6818f, 0.6818f);
					m_imageClanIcon->Draw(m_WindowPosition.x+73, m_WindowPosition.y+78+(20*(i-1)));
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 
					g_XBaseFont->PutsAlign( m_WindowPosition.x+127, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankOrgName[i] );
					
					g_XBaseFont->SetColor( _XSC_INFORMATION );
					g_XBaseFont->PutsAlign( m_WindowPosition.x+208, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankName[i] );
					
					g_XBaseFont->SetColor( _XSC_DEFAULT );
					g_XBaseFont->PutsAlign( m_WindowPosition.x+267, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankLevel[i] );
					g_XBaseFont->PutsAlign( m_WindowPosition.x+320, m_WindowPosition.y+81+(20*(i-1)), _XFONT_ALIGNTYPE_CENTER, m_RankQuestData[i] );
				}
			}
		}
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+51,  _XGETINTERFACETEXT(ID_STRING_NEW_2879), 1.0f); //"ð¯ìé"
		g_XBaseFont->SetColor( _XSC_INFORMATION );
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+81,  _XGETINTERFACETEXT(ID_STRING_NEW_2880), 1.0f); //"ð¯ì£"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+101, _XGETINTERFACETEXT(ID_STRING_NEW_2881), 1.0f); //"ð¯ß²"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+121, _XGETINTERFACETEXT(ID_STRING_NEW_2882), 1.0f); //"ð¯ÞÌ"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+141, _XGETINTERFACETEXT(ID_STRING_NEW_2883), 1.0f); //"ð¯çé"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+161, _XGETINTERFACETEXT(ID_STRING_NEW_2884), 1.0f); //"ð¯ë»"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+181, _XGETINTERFACETEXT(ID_STRING_NEW_2885), 1.0f); //"ð¯öÒ"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+201, _XGETINTERFACETEXT(ID_STRING_NEW_2886), 1.0f); //"ð¯ø¢"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+221, _XGETINTERFACETEXT(ID_STRING_NEW_2887), 1.0f); //"ð¯Îú"
		g_XBaseFont->Puts(m_WindowPosition.x+31, m_WindowPosition.y+241, _XGETINTERFACETEXT(ID_STRING_NEW_2888), 1.0f); //"ð¯ä¨"
		
	}
	

	g_XBaseFont->Flush();

}

BOOL _XWindow_QuestRank::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_QuestRank::ShowWindow(BOOL show)
{
	if(show)
	{
		_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
		if(pNPCScript_Window) 
		{
			if( pNPCScript_Window->m_MessageType == _XDEF_NPCMESSAGE_QUEST )
			{
				ShowWindow(FALSE);
				return;
			}
			else
			{
				MoveWindow( (gnWidth>>1)+101, (gnHeight>>1)-154 );
				g_NetworkKernel.SendPacket(MSG_NO_QUESTRANK_REQ, m_RankQuestID );
			}
		}
		
	}
	else
	{
		m_bReceiveRankData = FALSE;
		m_RankQuestID = -1;		

		memset( m_RankOrgName, 0,sizeof(TCHAR)*10*128 );
		memset( m_RankName, 0,sizeof(TCHAR)*10*128 );
		memset( m_RankLevel, 0,sizeof(TCHAR)*10*128 );
		memset( m_RankQuestData, 0,sizeof(TCHAR)*10*128 );
		memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*256 );
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_QuestRank::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	m_BorderTitle.MoveWindow(m_WindowPosition.x+1, m_WindowPosition.y+18);

}

void _XWindow_QuestRank::SetListItem(_QUEST_RANK_DATA* questrankdata)
{
	memset( m_RankOrgName, 0,sizeof(TCHAR)*10*128 );
	memset( m_RankName, 0,sizeof(TCHAR)*10*128 );
	memset( m_RankLevel, 0,sizeof(TCHAR)*10*128 );
	memset( m_RankQuestData, 0,sizeof(TCHAR)*10*128 );
	
	for( int i = 0; i < 10; i++ )
	{
		m_indexClan[i] = questrankdata->m_aRankRecord[i].m_ucClan;

		if( m_RankQuestType1 == 0 ) //0:°³ÀÎÄù½ºÆ® 1:µ¿ÇàÄù½ºÆ® 2:´ÜÄù½ºÆ®   //0:KillCount 1:Timer 2:ItemCount
		{
			// °³ÀÎ ÀÌ¸§À» Ç¥½ÃÇÑ´Ù.
			sprintf(m_RankName[i], "%s",questrankdata->m_aRankRecord[i].m_acName);
			
		}
		else if( m_RankQuestType1 == 1 )
		{
			// Çà¼ö ÀÌ¸§À» Ç¥½ÃÇÑ´Ù.
			sprintf(m_RankName[i], "%s",questrankdata->m_aRankRecord[i].m_acName);
			
		}
		else if( m_RankQuestType1 == 2 )
		{
			// ´Ü ÀÌ¸§À» Ç¥½ÃÇÑ´Ù.
			sprintf(m_RankOrgName[i], "%s",questrankdata->m_aRankRecord[i].m_acOrgName);
			// ´ÜÁÖ ÀÌ¸§À» Ç¥½ÃÇÑ´Ù.
			sprintf(m_RankName[i], "%s",questrankdata->m_aRankRecord[i].m_acName);
			
		}

		if( m_RankQuestType1 == 0 ) //0:°³ÀÎÄù½ºÆ® 1:µ¿ÇàÄù½ºÆ® 2:´ÜÄù½ºÆ®   //0:KillCount 1:Timer 2:ItemCount
		{
			// °³ÀÎ ¼ºÃë´Ü°è¸¦ Ç¥½ÃÇÑ´Ù. 
			DWORD levelmager = 0;
			DWORD levelminor = 0;
			
			_XLocalUser::GetUserLevel( questrankdata->m_aRankRecord[i].m_usLevelOrCount, levelmager, levelminor );
			
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
			
			sprintf(m_RankLevel[i], "%s %d%s",levelstring, levelminor, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
			
		}
		else if( m_RankQuestType1 == 1 ) //0:°³ÀÎÄù½ºÆ® 1:µ¿ÇàÄù½ºÆ® 2:´ÜÄù½ºÆ®   //0:KillCount 1:Timer 2:ItemCount
		{
			sprintf(m_RankLevel[i], _XGETINTERFACETEXT(ID_STRING_NEW_2889),questrankdata->m_aRankRecord[i].m_usLevelOrCount);//"%d¸í"
		}
		else if( m_RankQuestType1 == 2 )
		{
			sprintf(m_RankLevel[i], _XGETINTERFACETEXT(ID_STRING_NEW_2889),questrankdata->m_aRankRecord[i].m_usLevelOrCount);
		}

		// Äù½ºÆ® Ä«¿îÅÍ¸¦ Ç¥½ÃÇÑ´Ù.
		if( m_RankQuestType2 == 0 )// KillCount
		{
			sprintf(m_RankQuestData[i], _XGETINTERFACETEXT(ID_STRING_QUEST_2614),questrankdata->m_aRankRecord[i].m_usQuestData);
		}
		else if( m_RankQuestType2 == 1)//Timer
		{	
			int hour		=	questrankdata->m_aRankRecord[i].m_usQuestData/3600;
			
			int temptime	=	questrankdata->m_aRankRecord[i].m_usQuestData - hour*3600;

			int	minute		=  temptime / 60;
			int	second		=  temptime % 60;
			
			TCHAR temphour[8];
			TCHAR tempminute[8];
			TCHAR tempsecond[8];

			if( hour >= 10 )
			{
				sprintf(temphour, "%d",hour);
			}
			else
			{
				sprintf(temphour, "0%d",hour);
			}
			
			if( minute >= 10)
			{
				sprintf(tempminute, "%d",minute);
				//sprintf(m_RankQuestData[i], "00:%d:%d", minute, second );
			}
			else
			{
				sprintf(tempminute, "0%d",minute);
			}
			
			if( second >= 10)
			{
				sprintf(tempsecond, "%d",second);
			}
			else
			{
				sprintf(tempsecond, "0%d",second);
			}
			
			sprintf(m_RankQuestData[i], "%s:%s:%s", temphour, tempminute ,tempsecond );

		}
		else if( m_RankQuestType2 == 2)//ItemCount
		{
			sprintf(m_RankQuestData[i], _XGETINTERFACETEXT(ID_STRING_QUEST_2615),questrankdata->m_aRankRecord[i].m_usQuestData);
		}

	}
	
	m_bReceiveRankData = TRUE;

}

BOOL _XWindow_QuestRank::SetRankQuestID( int questid )
{
	m_RankQuestID = questid;
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( questid >= 10000 && questid < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return FALSE;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return FALSE;
	}
	
	if(!currentquest)
		return FALSE;
	
	if( currentquest->GetTempQuest() )
	{
		m_RankQuestType1 = currentquest->GetQuestRankType1();
		m_RankQuestType2 = currentquest->GetQuestRankType2();
		memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*256 );
		strcpy( m_SelectedQuestSubName, currentquest->GetQuestSubName() );
	}
	else
		return FALSE;

	return TRUE;
}
