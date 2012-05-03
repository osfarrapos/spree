// XProc_Flash.cpp: implementation of the XProc_Flash class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Base
#include "stdafx.h"
#include "resource.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"

#include "XProc_Flash.h"
#include "XProc_ProcessDefine.h"
#include "XProc_ZeroLevel.h"

#include "XWindow_ChattingDefault.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _XDEF_FLASHMODE_FADE_LEVEL				10

XProc_Flash::XProc_Flash()
{
	flashWin = NULL;
	m_Singnaltime = 0;
	m_pOffScreenTexture = NULL;	
	m_bLockEndProcess = FALSE;
	m_bClickExitFlashMap = FALSE;
	m_FlashScreenAlphaLevel = 0;

// 	m_bInCurrentZone = FALSE;
// 	m_RateOfMapSizeToTerrain = 319.0f/ g_LodTerrain.m_Width;
// 	
// 	m_PrevDrawTime					= 0;
// 	m_EffectOrderSub				= FALSE;
// 	m_EffectFrame					= 0.0f;
// 		
// 	for( int i = 0; i < 4; i++ )
// 	{
// 		m_imageCurrentZoneMark[i] = NULL;
// 	}
// 	
// 	//  1	숭산		숭산(소림사)
// 	m_MaximumMapZoneList[0].left	= 653.2f;
// 	m_MaximumMapZoneList[0].right	= 653.2f + 114.5f;
// 	m_MaximumMapZoneList[0].top		= 291.7f;
// 	m_MaximumMapZoneList[0].bottom	= 291.7f + 34.0f;
// 	
// 	// 	2	천진		천진(개방)	
// 	m_MaximumMapZoneList[1].left	= 749.7f;
// 	m_MaximumMapZoneList[1].right	= 749.7f + 93.5f;
// 	m_MaximumMapZoneList[1].top		= 129.3f;
// 	m_MaximumMapZoneList[1].bottom	= 129.3f + 29.5f;
// 		
// 	// 	3	태산		태산(녹림맹)
// 	m_MaximumMapZoneList[2].left	= 777.0f;
// 	m_MaximumMapZoneList[2].right	= 777.0f + 111.5f;
// 	m_MaximumMapZoneList[2].top		= 223.1f;
// 	m_MaximumMapZoneList[2].bottom	= 223.1f + 33.2f;
// 	
// 	// 	4	자하동    	자하동(비궁)
// 	m_MaximumMapZoneList[3].left	= 526.0f;
// 	m_MaximumMapZoneList[3].right	= 526.0f + 105.0f ;
// 	m_MaximumMapZoneList[3].top		= 128.0f;
// 	m_MaximumMapZoneList[3].bottom	= 128.0f + 28.0f;
// 	
// 	// 	5	무당산		무당
// 	m_MaximumMapZoneList[4].left	= 0;
// 	m_MaximumMapZoneList[4].right	= 0;
// 	m_MaximumMapZoneList[4].top		= 0;
// 	m_MaximumMapZoneList[4].bottom	= 0;
// 	
// 	// 	6	염마산		마교	
// 	m_MaximumMapZoneList[5].left	= 0;
// 	m_MaximumMapZoneList[5].right	= 0;
// 	m_MaximumMapZoneList[5].top		= 0;
// 	m_MaximumMapZoneList[5].bottom	= 0;
// 	
// 	// 	7	정주		정주
// 	m_MaximumMapZoneList[6].left	= 666.2f;
// 	m_MaximumMapZoneList[6].right	= 666.2f + 70.8f;
// 	m_MaximumMapZoneList[6].top		= 252.7f;
// 	m_MaximumMapZoneList[6].bottom	= 252.7f + 30.0f;
// 	
// 	// 	8	석가장		석가장		
// 	m_MaximumMapZoneList[7].left	= 688.3f;
// 	m_MaximumMapZoneList[7].right	= 688.3f+ 77.6f;
// 	m_MaximumMapZoneList[7].top		= 154.9f;
// 	m_MaximumMapZoneList[7].bottom	= 154.9f+ 29.1f;
// 	
// 	// 	9	제남		제남		
// 	m_MaximumMapZoneList[8].left	= 764.0f;
// 	m_MaximumMapZoneList[8].right	= 764.0f+ 73.7f;
// 	m_MaximumMapZoneList[8].top		= 187.6f;
// 	m_MaximumMapZoneList[8].bottom	= 187.6f+ 34.0f;
// 	
// 	// 	10	대동		대동		
// 	m_MaximumMapZoneList[9].left	= 632.5f;
// 	m_MaximumMapZoneList[9].right	= 632.5f+ 75.8f;
// 	m_MaximumMapZoneList[9].top		= 107.0f;
// 	m_MaximumMapZoneList[9].bottom	= 107.0f+ 30.0f;
// 	
// 	// 	11	남창		남창	
// 	m_MaximumMapZoneList[10].left	= 764.8f;
// 	m_MaximumMapZoneList[10].right	= 764.8f+ 69.3f;
// 	m_MaximumMapZoneList[10].top	= 414.9f;
// 	m_MaximumMapZoneList[10].bottom	= 414.9f+ 34.5f;
// 	
// 	// 	12	야수맹		강서
// 	m_MaximumMapZoneList[11].left	= 0;
// 	m_MaximumMapZoneList[11].right	= 0;
// 	m_MaximumMapZoneList[11].top	= 0;
// 	m_MaximumMapZoneList[11].bottom	= 0;
// 	
// 	// 	13	항주		절강	
// 	m_MaximumMapZoneList[12].left	= 876.3f;
// 	m_MaximumMapZoneList[12].right	= 876.3f+ 66.2f;
// 	m_MaximumMapZoneList[12].top	= 359.5f;
// 	m_MaximumMapZoneList[12].bottom	= 359.5f+ 34.5f;
// 	
// 	// 	14	금전방		절강
// 	m_MaximumMapZoneList[13].left	= 0;
// 	m_MaximumMapZoneList[13].right	= 0;
// 	m_MaximumMapZoneList[13].top	= 0;
// 	m_MaximumMapZoneList[13].bottom	= 0;
// 	
// 	// 	15	교도		남만
// 	m_MaximumMapZoneList[14].left	= 0;
// 	m_MaximumMapZoneList[14].right	= 0;
// 	m_MaximumMapZoneList[14].top	= 0;
// 	m_MaximumMapZoneList[14].bottom	= 0;
// 	
// 	// 	16	복주		복건	
// 	m_MaximumMapZoneList[16].left	= 0;
// 	m_MaximumMapZoneList[16].right	= 0;
// 	m_MaximumMapZoneList[16].top	= 0;
// 	m_MaximumMapZoneList[16].bottom	= 0;
// 	
// 	// 	17	합비	    안서		
// 	m_MaximumMapZoneList[16].left	= 775.3f;
// 	m_MaximumMapZoneList[16].right	= 775.3f+ 66.2f;
// 	m_MaximumMapZoneList[16].top	= 308.0f;
// 	m_MaximumMapZoneList[16].bottom	= 308.0f+ 33.0f;
	
}

XProc_Flash::~XProc_Flash()
{

}

BOOL XProc_Flash::InitializeProcess( void )
{		
	if( g_SceneList[g_indexScene] )
	{		
		if( m_MovieMode != _XMOVIE_ZEROLEVEL )
		{
			g_DrawFadeInOutFlag = TRUE;
			
			for( g_FadeInOutLevel = 0;  g_FadeInOutLevel <= 255;  g_FadeInOutLevel += 24 )
			{			
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.Draw();
			}
			
			if(g_FadeInOutLevel >= 255)
			{
				g_FadeInOutLevel = 255;			
			}
			g_DrawFadeInOutFlag = FALSE;
		}
		
		if( m_MovieMode == _XMOVIE_MAP )
		{
			if( g_indexScene == 2 )
			{
// 				m_RateOfMapSizeToTerrain = 319.0f/ g_LodTerrain.m_Width;
// 				
// 				_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
// 				if( pminimapwindow )
// 				{
// 					pminimapwindow->SetLocalUserPosition( 44, 520, m_RateOfMapSizeToTerrain, 319.0f );				
// 				}
 				
 				_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
 				if(pChat_Window)
 				{
 					pChat_Window->m_btnMaximumMapModeExit->MoveWindow( gnWidth - (1024 - 923), gnHeight - (768- 730) );
 					pChat_Window->m_btnMaximumMapModeExit->ShowWindow(TRUE);
 				}
// 				
// 				int npcpresentindex = g_MainInterfaceTextureArchive.FindResource( "MI_npc_present.tga" );
// 				for( int i = 0; i < 4; i++ )
// 				{
// 					m_imageCurrentZoneMark[i] = new _XImageStatic;
// 					m_imageCurrentZoneMark[i]->Create(0, 0, 26, 42, &g_MainInterfaceTextureArchive, npcpresentindex);
// 				}
// 				m_imageCurrentZoneMark[0]->SetClipRect(174, 2, 200, 44);
// 				m_imageCurrentZoneMark[1]->SetClipRect(204, 2, 230, 44);	
// 				m_imageCurrentZoneMark[2]->SetClipRect(204, 47, 230, 89);
// 				m_imageCurrentZoneMark[3]->SetClipRect(174, 47, 200, 89);	
			}
		}
		
#ifdef _XDEF_USENEWBACKGROUND
		m_FlashStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
#else
		m_FlashStatic.Create( 0, 0, gnWidth, gnHeight, NULL, -1 );
#endif		
		
		if( !CreateOffScreen() )
		{
			return FALSE;
		}
		
		if (flashWin == NULL)
		{
			flashWin = GetFlashControl();
			flashWin->initialize( gHWnd,gHInst,m_pOffScreenTexture );
			
//			if( g_indexScene >= 48 && g_indexScene <= 53 )
//			{
//				//튜토리얼 퀘스트에서만 퀄리티를 높인다.
//				flashWin->ControlHighQuality();
//			}				
//			else
			{
				flashWin->SetLowQuality();
			}
		}
		
		HDC pDC = GetDC( gHWnd );
		flashWin->SetDC( pDC );
		
		TCHAR resourcepath[_MAX_PATH];
		memset( resourcepath, 0, sizeof(TCHAR) * _MAX_PATH);
		_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
		
		_XPackageArchive package;
#ifdef _XTESTSERVER				
	if( !package.OpenPackage( _T("tsXSCENESCRIPT.XP") ) )
	{
		return FALSE;
	}
#else
	if( !package.OpenPackage( _T("XSCENESCRIPT.XP") ) )
	{
		return FALSE;
	}
#endif
		int resourceid = package.FindResource( g_SceneList[g_indexScene] );
		if( resourceid >= 0 )
		{
			long filesize = package.GetPackedFileSize( resourceid );
			flashWin->SetResourceFilePtr( package.GetPackedFile( resourceid ), filesize );
			flashWin->ControlOpen(g_SceneList[g_indexScene]);
			//flashWin->SetLowQuality();	
		
			if( g_indexScene >= 48 && g_indexScene <= 53 )
			{
				//튜토리얼 퀘스트에서만 퀄리티를 높인다.
				flashWin->ControlHighQuality();
			}
		}

		package.FinalizePackage();
		
		flashWin->SetDC( 0 );
		ReleaseDC( gHWnd, pDC );
		
		g_ReadyMouseCursorMode = _XMOUSETYPE_PEACE;
		((CEmperorOfDragonsApp*)gApp)->SetCursor( g_ReadyMouseCursorMode );
		
		m_FlashScreenAlphaLevel = 0;
		
		SetEnableProcess( TRUE );
	}
	else
	{
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
	}

	
	m_Initialized = TRUE;
	
	return TRUE;
}
void XProc_Flash::DestroyProcess( void )
{		
	
	if( m_MovieMode != _XMOVIE_ZEROLEVEL )
	{
		g_DrawFadeInOutFlag = TRUE;
		for( g_FadeInOutLevel = 255;  g_FadeInOutLevel >= 0;  g_FadeInOutLevel -= 24 )
		{		
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.Draw();
		}
		
		if(g_FadeInOutLevel <= 0)
		{
			g_FadeInOutLevel = 0;	
		}
 		g_DrawFadeInOutFlag = FALSE;
	}


	if(flashWin)
	{
		flashWin->FreeBuffer();
		delete flashWin;
		flashWin = 0;
	}

	SAFE_RELEASE( m_pOffScreenTexture );
	
	if( g_NetworkKernel.m_hClientSocket )
		WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
	
	SetEnableProcess( FALSE );
}
bool XProc_Flash::InitDeviceObject( void )
{
	CreateOffScreen();
	if(flashWin)
	{
		flashWin->SetOffScreenTextureObject( m_pOffScreenTexture );
	}

	return true;
}

void XProc_Flash::ReleaseDeviceObject( void )
{	
	SAFE_RELEASE( m_pOffScreenTexture );
}

void XProc_Flash::Draw( void )
{	
	
}

void XProc_Flash::FlashDraw( void )
{
	if( !gApp->m_bHasFocus ) return;

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	
	// Begin the scene
	gpDev->BeginScene();

#ifdef _XDEF_USENEWBACKGROUND
	m_FlashStatic.Render( m_pOffScreenTexture );
#else
	m_FlashStatic.DrawWithRegion( m_pOffScreenTexture );	
#endif
	

#ifdef _XDWDEBUG 
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->PrintAlign( 0, 0,1.0f, _XFONT_ALIGNTYPE_LEFT, "FPS:%d", gnFPS);
	g_XBaseFont->Flush();	
#endif
				
	g_MessageBox.Draw();
	
	if((g_LocalSystemTime - m_Singnaltime) > 3000)
	{
		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		m_Singnaltime = g_LocalSystemTime;
	}		
	
	if( m_MovieMode == _XMOVIE_MAP )
	{
		_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
		if(pChat_Window)
		{
			pChat_Window->Draw();
		}

		if( m_bInCurrentZone)
		{
// 			if( !g_pLocalUser->m_bIsBossBattle )
// 			{
// 					
// 				// NPC 위치를 찍어준다.
// 				_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
// 				if( pminimapwindow )
// 				{
// 					
// 					pminimapwindow->SetLocalUserPosition( 44, 520, m_RateOfMapSizeToTerrain, 319.0f );	
// 					pminimapwindow->DrawLargeMinipMapNPCPosition( 44, 520, m_RateOfMapSizeToTerrain, 319.0f );
// 					if( g_pLocalUser->m_InParty ) pminimapwindow->DrawLargeMinipMapPartyPlayerPosition( 44, 520, m_RateOfMapSizeToTerrain, 319.0f);		
// 					
// 					pminimapwindow->m_FrustumeImage.Draw();
// 					pminimapwindow->m_MinimapArrow.RenderAlphaBlend();
// 
// 					if( pminimapwindow->m_bDrawInformationTooltip )
// 					{
// 						if( !g_pLocalUser->m_InParty )
// 						{
// 							if( pminimapwindow->m_SelectedNPCIndex != -1 )
// 							{							
// 								g_NPCNameWindow.Draw( pminimapwindow->m_TooltipTargetPosition_X + 72, pminimapwindow->m_TooltipTargetPosition_Y - 43,
// 									g_Npclist.m_pNPCObjectArray[pminimapwindow->m_SelectedNPCIndex].m_NPCRoll,
// 									g_Npclist.m_pNPCObjectArray[pminimapwindow->m_SelectedNPCIndex].m_NPCName, 
// 									FALSE, _XNPC_HAVEQUEST_NONE,  g_Npclist.m_pNPCObjectArray[pminimapwindow->m_SelectedNPCIndex].m_NPCJobClass );	
// 							}
// 							else if( pminimapwindow->m_SelectedQuestID != -1 )
// 							{
// 								TCHAR messagestring[512];
// 								memset( messagestring, 0, sizeof(TCHAR)*512 );
// 
// 								switch(pminimapwindow->m_SelectedQuestType) {
// 								case _XQUEST_TYPE_1://제룡행 
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE1), questscriptitem->GetQuestName() );
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10,
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 255, 212, 0), FALSE );								
// 									}
// 									break;
// 								case _XQUEST_TYPE_2://무림행 
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE2), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 22, 146, 252), FALSE );
// 									}
// 									break;
// 								case _XQUEST_TYPE_3://협객행
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE3), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 245, 116, 34), FALSE );		
// 									}
// 									break;
// 								case _XQUEST_TYPE_4://기연행 
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE4), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 78, 229, 48), FALSE );	
// 									}
// 									break;
// 								case _XQUEST_TYPE_5://무림소사
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE5), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y-10,
// 											messagestring, D3DCOLOR_ARGB(255, 255, 255, 255), FALSE );					
// 									}
// 									break;
// 								}						
// 								g_InterfaceTooltip.Draw();
// 							}
// 						}
// 						else
// 						{
// 							if( pminimapwindow->m_SelectedNPCIndex != -1 )
// 							{							
// 								g_NPCNameWindow.Draw( pminimapwindow->m_TooltipTargetPosition_X + 72, pminimapwindow->m_TooltipTargetPosition_Y - 43,
// 									g_Npclist.m_pNPCObjectArray[pminimapwindow->m_SelectedNPCIndex].m_NPCRoll,
// 									g_Npclist.m_pNPCObjectArray[pminimapwindow->m_SelectedNPCIndex].m_NPCName, 
// 									FALSE, _XNPC_HAVEQUEST_NONE,  g_Npclist.m_pNPCObjectArray[pminimapwindow->m_SelectedNPCIndex].m_NPCJobClass );	
// 							}
// 							else if( pminimapwindow->m_SelectedQuestID != -1 )
// 							{
// 								TCHAR messagestring[512];
// 								memset( messagestring, 0, sizeof(TCHAR)*512 );
// 								
// 								switch(pminimapwindow->m_SelectedQuestType) {
// 								case _XQUEST_TYPE_1://제룡행 
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE1), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 255, 212, 0), FALSE );								
// 									}
// 									break;
// 								case _XQUEST_TYPE_2://무림행 
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE2), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 22, 146, 252), FALSE );
// 									}
// 									break;
// 								case _XQUEST_TYPE_3://협객행
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE3), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 245, 116, 34), FALSE );		
// 									}
// 									break;
// 								case _XQUEST_TYPE_4://기연행 
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE4), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 78, 229, 48), FALSE );	
// 									}
// 									break;
// 								case _XQUEST_TYPE_5://무림소사
// 									{
// 										_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[pminimapwindow->m_SelectedQuestID];
// 										sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE5), questscriptitem->GetQuestName() );
// 										
// 										g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 											pminimapwindow->m_TooltipTargetPosition_Y -20,
// 											messagestring, D3DCOLOR_ARGB(255, 255, 255, 255), FALSE );					
// 									}
// 									break;
// 								}						
// 								g_InterfaceTooltip.Draw();
// 							}
// 							else// 파티원 이름 
// 							{
// 								TCHAR messagestring[512];
// 								memset( messagestring, 0, sizeof(TCHAR)*512 );
// 								
// 								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_PARTYMEMBER), g_pLocalUser->m_PartyInfo[pminimapwindow->m_SelectedPartyMemberIndex].playername );
// 								
// 								g_InterfaceTooltip.SetToolTipString( pminimapwindow->m_TooltipTargetPosition_X  + 10, 
// 									pminimapwindow->m_TooltipTargetPosition_Y -20,
// 									messagestring, _XSC_INFORMATION, FALSE );		
// 								g_InterfaceTooltip.Draw();			
// 							}
// 						}
// 					}//if( m_bDrawInformationTooltip )
// 				}
// 			}
		}//if( m_bInCurrentZone )
		
		if( g_CurrentZoneInfoIndex != 4 )
		{
// 			if( !g_pLocalUser->m_bIsBossBattle )
// 			{
// 				DrawCurrentZoneMark(m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].left + 2, m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].top - 28 );
// 			}
// 			else
// 			{
// 				DrawCurrentZoneMark(m_MaximumMapZoneList[g_ChannelTempZoneInfoIndex-1].left + 2, m_MaximumMapZoneList[g_ChannelTempZoneInfoIndex-1].top - 28 );
// 			}
		}
		else// 자하비궁일 경우 오른쪽...
		{
// 			if( !g_pLocalUser->m_bIsBossBattle )
// 			{
// 				DrawCurrentZoneMark(m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].right-18 , m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].top - 28 );
// 			}
// 			else
// 			{
// 				DrawCurrentZoneMark(m_MaximumMapZoneList[g_ChannelTempZoneInfoIndex-1].right-18 , m_MaximumMapZoneList[g_ChannelTempZoneInfoIndex-1].top - 28 );
// 			}
		}
		
	}
	
	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr ) gApp->m_bDeviceLost = true;
	
	
}

bool XProc_Flash::Process( void )
{	
	if( !m_bLockEndProcess )
	{		
		if( m_MovieMode == _XMOVIE_ZEROLEVEL )
		{
			if( !flashWin->IsPlaying() )		
			{
				if( g_indexScene == 0 )// 제로레벨 시작 영상-> 끝난후에 제로레벨 시작
				{
					flashWin->ControlClose();
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel );						
				}
				else// 제로레벨 끝 영상-> 끝난후에 로그인 
				{
					flashWin->ControlClose();
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver); 
				}
			}
		}
		else
		{

			if( m_MovieMode == _XMOVIE_QUEST )
			{
				if( (g_indexScene >= 16 && g_indexScene <= 19) || g_indexScene == 31 || g_indexScene == 32 || (g_indexScene >= 48 && g_indexScene <= 52))
				{
				}				
				else
				{
					if( !flashWin->IsPlaying() )		
					{
						flashWin->ControlClose();
						((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 
						((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
					}		
				}
			}
			else if( m_MovieMode != _XMOVIE_MAP && m_MovieMode != _XMOVIE_CLASSEXPLAIN)
			{
				if( !flashWin->IsPlaying() )		
				{				
					flashWin->ControlClose();
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
				}		
			}

		}
	}
	else
	{			
		if( m_MovieMode ==  _XMOVIE_ZEROLEVEL )
		{
			if( g_indexScene == 0 )
			{
				if( flashWin->GetTargetName() )
				{
					if( strcmpi(flashWin->GetTargetName(),"gaebang" )==0 )
					{								
						XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
						pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_GAEBANG);
						pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_NOCKRIM);
						pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);	
						m_bLockEndProcess = FALSE;
					}
					else if( strcmpi(flashWin->GetTargetName(),"sorim" )==0 )
					{
						XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
						pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_SORIM);
						pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_BEEGOONG);
						pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);		
						m_bLockEndProcess = FALSE;
					}
					else if( strcmpi(flashWin->GetTargetName(),"beegoong" )==0 )
					{
						
						XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
						pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_BEEGOONG);
						pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_SORIM);
						pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);	
						m_bLockEndProcess = FALSE;
					}
					else if( strcmpi(flashWin->GetTargetName(),"nockrim" )==0 )
					{
						XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
						pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_NOCKRIM);
						pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_GAEBANG);
						pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);	
						m_bLockEndProcess = FALSE;
					}
				}					
			}
		}	
		else
		{			
//			m_FlashScreenAlphaLevel += _XDEF_FLASHMODE_FADE_LEVEL;
//			m_FlashStatic.m_FColor = D3DCOLOR_ARGB( m_FlashScreenAlphaLevel, 255, 255, 255 );
//			if( m_FlashScreenAlphaLevel >= 255 )
//			{						
//				m_FlashScreenAlphaLevel = 255;
//				m_bLockEndProcess = FALSE;
//			}

		}
	}
	
	if( m_MovieMode == _XMOVIE_ZEROLEVEL )
	{
#ifdef _XUSEFMOD
		g_FMODWrapper.ProcessBGM();
		g_FMODWrapper.FMODUpdate();	
#endif
		return TRUE;//((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel.Process();
	}
	else
		return ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.Process();
}

void XProc_Flash::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{	
	
	if( message == WM_COMMAND )
	{
		// Messenger Dialog window messge handling-----------------------------------------
		if( wparam >= 60000 ) //메신져 인스턴스 대화창에 대한 처리 
		{
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.MessengerDialogWindowMessageHandling(wparam, lparam);
			return;
		}
		
		switch( wparam )
		{
		// Chatting window message handling-----------------------------------------
		case _XDEF_CHATDEFAULT_FUNCTIONBUTTON		:
		case _XDEF_CHATDEFAULT_FUNCTION_EMOTIONBUTTON:
		case _XDEF_CHATDEFAULT_FUNCTION_ALLBUTTON	:
		case _XDEF_CHATDEFAULT_FUNCTION_WHISPERBUTTON:
		case _XDEF_CHATDEFAULT_FUNCTION_PARTYBUTTON	:
		case _XDEF_CHATDEFAULT_FUNCTION_SUBCLANBUTTON:
		case _XDEF_CHATDEFAULT_FUNCTION_CLANBUTTON	:
		case _XDEF_CHATDEFAULT_RESIZEBUTTON			:
		case _XDEF_CHATDEFAULT_WHISPERTARGETBUTTON1 :
		case _XDEF_CHATDEFAULT_WHISPERTARGETBUTTON2 :
		case _XDEF_CHATDEFAULT_WHISPERTARGETBUTTON3 :
		case _XDEF_CHATDEFAULT_DOCKINGSYSTEMMESSAGE :
		case _XDEF_SYSTEMMSGDEFAULT_CLOSE			:
		case _XDEF_CHATDEFAULT_EMOTIONLISTBOX		:
		/*case _XDEF_CHATDEFAULT_EMOTION1	 :			
		case _XDEF_CHATDEFAULT_EMOTION2	 :
		case _XDEF_CHATDEFAULT_EMOTION3	 :
		case _XDEF_CHATDEFAULT_EMOTION4	 :
		case _XDEF_CHATDEFAULT_EMOTION5	 :
		case _XDEF_CHATDEFAULT_EMOTION6	 :
		case _XDEF_CHATDEFAULT_EMOTION7	 :
		case _XDEF_CHATDEFAULT_EMOTION8	 :
		case _XDEF_CHATDEFAULT_EMOTION9	 :
		case _XDEF_CHATDEFAULT_EMOTION10 :*/
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.ChatWindowMessageHandling( wparam, lparam );			
			break;
		
		// Minimap window message handling------------------------------------------
		case _XDEF_MAXMINIMAP_CLOSEBUTTON			:
			{
				if( m_MovieMode == _XMOVIE_MAP )
				{
					if( g_indexScene == 2 )// 제로레벨 시작 영상-> 끝난후에 제로레벨 시작
					{
						_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
						if(pChat_Window)
						{
							pChat_Window->m_btnMaximumMapModeExit->ShowWindow(FALSE);
						}
// 						_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
// 						if( pminimapwindow )
// 						{
// 							pminimapwindow->SetLocalUserPosition();
// 						}
// 
// 						m_bInCurrentZone = FALSE;
						
						flashWin->ControlClose();
						((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 					
						((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
					}
				}
			}			
			break;
		
		default:
			break;
		}
	}	

}
void XProc_Flash::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{	
	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.OnUserMessage_Socket( message, wparam, lparam);
}

void XProc_Flash::OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{	
	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.OnUserMessage_MessengerSocket( message, wparam, lparam);
}

bool XProc_Flash::OnKeyboardPoll(_XInput *pInput )
{	
	return TRUE;
}
bool XProc_Flash::OnKeyDown(WPARAM wparam, LPARAM lparam )
{	
	
	DWORD scancode = (HIWORD( lparam ) & 0xFF);
	switch( scancode )
	{
		
	case DIK_RETURN :
		{			
			if( m_MovieMode == _XMOVIE_MAP )
			{
				_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

				if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ChatEditContainer.GetShowStatus() || !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ChatEditContainer.GetFocus() )
				{
					_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA					
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ChatEditContainer.ClearBuffer();						
#endif
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ChatEditContainer.ShowWindow( TRUE );
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ChatEditContainer.SetFocus();
					
					pChatDefault_Window->m_btnMaximumMapModeExit->MoveWindow( gnWidth - (1024 - 923), gnHeight- (768- 730) );	
					pChatDefault_Window->m_MovingChatWindow = TRUE;
					pChatDefault_Window->SetDrawBorder(TRUE);
				}
				else
				{				
					_XIMEKERNEL.SetFocus( NULL );	
					pChatDefault_Window->m_btnMaximumMapModeExit->MoveWindow( gnWidth - (1024 - 923), gnHeight - (768- 730) );
				}
			}
		}
		return true;	
	
	case DIK_ESCAPE :
		{					
			if( m_MovieMode == _XMOVIE_MAP )
			{
				if( g_indexScene == 2 )
				{						
					_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
					if(pChat_Window)
					{
						pChat_Window->m_btnMaximumMapModeExit->ShowWindow(FALSE);
					}
// 					_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
// 					if( pminimapwindow )
// 					{
// 						pminimapwindow->SetLocalUserPosition();
// 					}
// 
// 					m_bInCurrentZone = FALSE;

					flashWin->ControlClose();
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 					
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
				}
			}
			else if( m_MovieMode == _XMOVIE_CLASSEXPLAIN )
			{
				if( (g_indexScene >= 16 && g_indexScene <= 19) || g_indexScene == 31 || g_indexScene == 32 )
				{
					flashWin->ControlClose();
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
				}
			}
			else if( m_MovieMode == _XMOVIE_QUEST )
			{
				flashWin->ControlClose();
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
			}
			else if( m_MovieMode == _XMOVIE_BOSS )
			{
				flashWin->ControlClose();
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
			}
#ifdef _XDWDEBUG 
			else if( m_MovieMode == _XMOVIE_NONE )
			{
				flashWin->ControlClose();
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
			}
#endif
		}		
		return true;	
	case DIK_N :
		{			
			if( m_MovieMode == _XMOVIE_MAP )
			{
				if( g_indexScene == 2 )
				{						
					_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
					if(pChat_Window)
					{
						pChat_Window->m_btnMaximumMapModeExit->ShowWindow(FALSE);
					}
// 					_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
// 					if( pminimapwindow )
// 					{
// 						pminimapwindow->SetLocalUserPosition();
// 					}
// 					
// 					m_bInCurrentZone = FALSE;
					
					flashWin->ControlClose();
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 					
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
				}
			}
		}
		return true;
	}
	return true;
}
bool XProc_Flash::OnKeyUp(WPARAM wparam, LPARAM lparam )
{	
	return TRUE;
}
bool XProc_Flash::OnMousePoll(MouseState *pState, ScrnPos *pPos )
{	
	return TRUE;
}

void XProc_Flash::OnMouseMove( WPARAM wparam, LPARAM lparam )
{	
	int fwKeys = wparam;        // key flags 
	int xPos = LOWORD(lparam);  // horizontal position of cursor 
	int yPos = HIWORD(lparam);  // vertical position of cursor 
	
	flashWin->MouseMove( xPos, yPos, fwKeys & MK_LBUTTON );		
	FlashDraw();

}

bool XProc_Flash::OnMouseButton(_XMouseButton p_MB, bool bPushed )
{		
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	switch(p_MB) {
	case _XmbLeft:
		{
			if( bPushed )//Left Button Down
			{				
				SetCapture( gHWnd );				
				flashWin->MouseDown( scrnpos->x, scrnpos->z );					
				FlashDraw();
				if( m_MovieMode == _XMOVIE_MAP )
				{
					if( g_indexScene == 2 )
					{
						_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
						
						
						if( pChatDefault_Window->m_btnMaximumMapModeExit->CheckMousePosition() )
						{
							//pChatDefault_Window->m_btnMaximumMapModeExit->ShowWindow(FALSE);

// 							_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
// 							if( pminimapwindow )
// 							{
// 								pminimapwindow->SetLocalUserPosition();
// 							}
// 							
// 							m_bInCurrentZone = FALSE;
							
							m_bClickExitFlashMap = TRUE;
							//flashWin->ControlClose();
							//((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 							
						}
// 						else
// 						{
// 							m_bInCurrentZone = FALSE;							
// 							if( scrnpos->z > m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].top )
// 							{
// 								if( scrnpos->z < m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].bottom )
// 								{
// 									if( scrnpos->x > m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].left  )
// 									{
// 										if( scrnpos->x < m_MaximumMapZoneList[g_CurrentZoneInfoIndex-1].right )
// 										{
// 											m_bInCurrentZone = TRUE;
// 										}
// 									}
// 								}
// 							}
// 							
// 						}						
					}					
				}
				else if( m_MovieMode == _XMOVIE_CLASSEXPLAIN || m_MovieMode == _XMOVIE_QUEST)
				{
					if( (g_indexScene >= 16 && g_indexScene <= 19) || g_indexScene == 31 || g_indexScene == 32 )
					{					
						_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
						
						
						if( pChatDefault_Window->m_btnMaximumMapModeExit->CheckMousePosition() )
						//Author : 양희왕 //breif : 닫기 버튼 위치가 틀리다.. 해상도 때문인가?? 
//						if( scrnpos->z >= 700 && scrnpos->z <= 720 &&
//							scrnpos->x >= 920 && scrnpos->x <= 995 )
						{
							flashWin->ControlClose();
							((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
							((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
						}
						/*if( scrnpos->z > 730 )
						{
							if( scrnpos->z < 759 )
							{
								if( scrnpos->x > 922 )
								{
									if( scrnpos->x < 1006 )
									{
										flashWin->ControlClose();
										((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
										((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
									}
								}
							}
						}*/
					}
					else if( (g_indexScene >= 48 && g_indexScene < 53) )// 튜토리얼 퀘스트 설명 플래쉬
					{		
						RECT rc; 
						GetClientRect( gHWnd, &rc);	
						int Height = rc.bottom-rc.top;
						int Width = rc.right-rc.left;
						if( scrnpos->z > Height-200)//(Height>>1)+184 )//568 )
						{
							if( scrnpos->z < Height-200+22)//(Height>>1)+206 )//590 )
							{
								if( scrnpos->x > (Width>>1)-34 )//478 )
								{
									if( scrnpos->x < (Width>>1)+32 )//554 )
									{
										flashWin->ControlClose();
										((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
										((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
									}
								}
							}
						}
					}
					else if( (g_indexScene == 53) )// 튜토리얼 퀘스트 설명 플래쉬
					{	
						RECT rc; 
						GetClientRect( gHWnd, &rc);	
						int Height = rc.bottom-rc.top;
						int Width = rc.right-rc.left;

						if( scrnpos->z > Height-35)//733 )
						{
							if( scrnpos->z < Height-15)//753 )
							{
								if( scrnpos->x > (Width>>1)-36)//476 )
								{
									if( scrnpos->x < (Width>>1)+38)//550 )
									{
										flashWin->ControlClose();
										((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 	
										((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
									}
								}
							}
						}
						
					}

				}
				//if( m_MovieMode == _XMOVIE_MAP )	
				//				//제로레벨일때 버튼이 클릭되면 
				//				if( m_MovieMode == _XMOVIE_ZEROLEVEL )
				//				{
				//					if(  scrnpos->z > 664 && scrnpos->z < 664+58 )
				//					{
				//						if( scrnpos->x > 633 && scrnpos->x < 633+295 )
				//						{
				//							if( flashWin->GetTargetName() )
				//							{
				//								if( strcmpi(flashWin->GetTargetName(),"gaebang" )==0 )
				//								{								
				//									XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
				//									pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_GAEBANG);
				//									pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_NOCKRIM);
				//									pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);	
				//									m_bLockEndProcess = FALSE;
				//								}
				//								else if( strcmpi(flashWin->GetTargetName(),"sorim" )==0 )
				//								{
				//									XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
				//									pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_SORIM);
				//									pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_BEEGOONG);
				//									pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);		
				//									m_bLockEndProcess = FALSE;
				//								}
				//								else if( strcmpi(flashWin->GetTargetName(),"beegoong" )==0 )
				//								{
				//									
				//									XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
				//									pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_BEEGOONG);
				//									pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_SORIM);
				//									pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);	
				//									m_bLockEndProcess = FALSE;
				//								}
				//								else if( strcmpi(flashWin->GetTargetName(),"nockrim" )==0 )
				//								{
				//									XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
				//									pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_NOCKRIM);
				//									pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_GAEBANG);
				//									pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);	
				//									m_bLockEndProcess = FALSE;
				//								}
				//							}								
				//						}
				//					}					
				//				}
			}
			else//Left Button Up
			{
				ReleaseCapture();								
				flashWin->MouseUp( scrnpos->x, scrnpos->z );
				FlashDraw();

				if( m_bClickExitFlashMap )
				{
					if( m_MovieMode == _XMOVIE_MAP )
					{
						if( g_indexScene == 2 )
						{
							_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
														
							if( pChatDefault_Window->m_btnMaximumMapModeExit->CheckMousePosition() )
							{
								pChatDefault_Window->m_btnMaximumMapModeExit->ShowWindow(FALSE);
								m_bClickExitFlashMap = FALSE;

								flashWin->ControlClose();
								((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.InitDeviceObject();
							}
						}
					}
				}//if( m_bClickExitFlashMap )
			}
		}
		break;
	}
	return TRUE;
}

void XProc_Flash::OnTimer(WPARAM wparam, LPARAM lparam)
{
	int wmId;
	wmId = wparam;             // timer identifier 
	if ( wmId == TIMER_PLAY )
		flashWin->PlayTimerFire();
	else if ( wmId == TIMER_CURSOR )
		flashWin->CursorTimerFire();
	
	FlashDraw();

}

void XProc_Flash::OnPaint(WPARAM wparam, LPARAM lparam)
{
	if (flashWin == NULL)
	{
		CreateOffScreen();
		flashWin = GetFlashControl();
		flashWin->initialize( gHWnd,gHInst,m_pOffScreenTexture );
		//flashWin->SetLowQuality();
		
		//flashWin->ControlHighQuality();
		return;
	}
	
	RECT rc; 
	GetClientRect( gHWnd, &rc);	
	flashWin->Repaint( rc);
	
	FlashDraw();
}

BOOL XProc_Flash::CreateOffScreen( void )
{
	if( m_pOffScreenTexture ) return TRUE;

#ifdef _XDEF_USENEWBACKGROUND
	//if( FAILED( gpDev->CreateTexture( 1024, 768, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pOffScreenTexture, NULL ) ) )
	if( FAILED( gpDev->CreateTexture( gnWidth, gnHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
#else
	SIZE size = m_FlashStatic.GetWindowSize();

	if( FAILED( gpDev->CreateTexture( size.cx, size.cy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
#endif

	return TRUE;
}

void XProc_Flash::DrawCurrentZoneMark(int x, int y)
{
// 	if(m_PrevDrawTime == 0)
// 		m_PrevDrawTime = g_LocalSystemTime;
// 	
// 	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
// 	
// 	if(!m_EffectOrderSub)
// 		m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
// 	else 
// 		m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
// 	
// 	if(m_EffectFrame > 5.0f)
// 	{
// 		m_EffectOrderSub = true;
// 		m_EffectFrame = 4.0f;
// 	}
// 	else if(m_EffectFrame <= 0.0f)
// 	{
// 		m_EffectOrderSub = false;
// 		m_EffectFrame = 0.0f;
// 	}
// 	
// 	
// 	int selectedframe = (int)m_EffectFrame;
// 	
// 	if(selectedframe > 3)
// 		selectedframe = 3;
// 	else if(selectedframe < 0)
// 		selectedframe = 0;
// 		
// 	m_imageCurrentZoneMark[selectedframe]->Draw( x, y );
// 	m_PrevDrawTime = g_LocalSystemTime;
}
