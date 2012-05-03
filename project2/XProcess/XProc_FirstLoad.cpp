// XProc_FirstLoad.cpp: implementation of the XProc_FirstLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "resource.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XProc_FirstLoad.h"
#include "EODEXTDEF_ModelData.h"
#include "XParticleIDDefine.h"
#include "LoadThread.h"

///////////////////////////////////////////////////////////////////////
// GUI 

#include "XProc_ProcessDefine.h"
#include "XWindow_CInfo.h"
#include "XWindow_QuestWindow.h"
#include "XWindow_Inventory.h"
#include "XWindow_NPCScript.h"
#include "XWindow_WorldMinimap.h"
#include "XWindow_MaximumMap.h"
#include "XWindow_QuickSlot.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_SystemMsgDefault.h"	// 2004.11.26->hotblood
#include "XWindow_Skill.h"
#include "XWindow_Party.h"
#include "XWindow_OptionMenu.h"
#include "XWindow_Option.h"
#include "XWindow_PCTrade.h"
#include "XWindow_MainMenu.h"
#include "XWindow_Messenger.h"
#include "XWindow_JinStatus.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_ZoneName.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_DeathBlow.h"
#include "XWindow_Debug.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_TradeConfirm.h"
#include "XWindow_RequestParty.h"
#include "XWindow_AdminNotice.h"
#include "XWindow_ChargeGauge.h"
#include "XWindow_WareHouse.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_MessengerTooltip.h"
#include "XWindow_NameInput.h"
#include "XWindow_NPCSkill.h"
#include "XWindow_Rebirth.h"
#include "XWindow_RepairItem.h"// 2004.06.17->oneway48 insert
#include "XWindow_Help.h"//hotblood20040825
#include "XWindow_SocketPlugIn.h"// 2004.11.3->hotblood insert
#include "XWindow_DefaultTooltip.h"
#include "XWindow_MiniTooltip.h"
#include "XWindow_MainFrame.h"
#include "XWindow_Email.h"
#include "XWindow_Contribution.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_EventNotice.h"
#include "XWindow_SocketPlugInExp.h"
#include "XWindow_AreaSelect.h"
#include "XWindow_Inchant.h"
#include "XWindow_BookOpen.h"
#include "XWindow_CashMall.h"
#include "XWindow_CashBox.h"
// GM Control windows
#include "XWindow_GMCommand.h"
#include "XWindow_GM_SendNoticeMessage.h"
#include "XWindow_GM_UserCoordination.h"
#include "XWindow_GM_MonsterManage.h"
#include "XWindow_GM_StatusControl.h"
#include "XWindow_GM_EventManager.h"

#ifdef _XUSEFMOD
#include "XFMODWrapper.h"
#else
#include "XAudioSystem.h"
using namespace Audio;
#endif

#include "SoundEffectList.h"

#include "XWindow_TrainingMed.h"
#include "XWindow_TrainingDummy.h"
#include "XWindow_TrainingSpell.h"
#include "XWindow_TrainingComplete.h"

#include "XWindow_EnterBossRoom.h"
#include "XWindow_NickName.h"
#include "Xwindow_State.h"
#include "XWindow_TrainingHelp.h"
#include "XWindow_Hedek.h"
#include "XWindow_Pocket.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_HelpBook.h"
#include "XWindow_Gamble.h"
#include "XWindow_TargetInfo.h"
#include "XWindow_PVPBroker.h"
#include "Xwindow_MatchTablePVP.h"
#include "XWindow_MatchNoticeLarge.h"
#include "XWindow_MatchNoticeSmall.h"
#include "XWindow_MatchMessage.h"
#include "XWindow_MatchPR.h"
#include "XWindow_MatchRecord.h"
#include "XWindow_CashItem.h"
#include "XWindow_NPCCollection.h"
#include "XWindow_NPCResource.h"
#include "XWindow_NPCBuff.h"
#include "XWindow_PaymentWindow.h"
#include "XWindow_QuestRank.h"
#include "Xwindow_PK.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_GuideLine.h"
#include "XWindow_HelpScreen.h"
#include "XWindow_NPCSearch.h"
#include "XWindow_WebBoard.h"
#include "XWindow_Escape.h"

#include "XSTWrapper.h"
#include "XSTWrapperCore.h"

#include "XSR_STRINGHEADER.H"

#include "XTextureController.h"
#include "XWindowObjectDefine.h"

#include "XWindow_PCRPremium.h"
#include "XWindow_AccountItemUse.h"
#include "XWindow_MoveVillage.h"
#include "XWindow_Event.h"
#include "XWindow_CastleInfo.h"
#include "XWindow_ChallengerList.h"
#include "XWindow_CastleBattleShout.h"
#include "XWindow_CastleBattleTimeChange.h"
#include "XWindow_CastleBattleRewardList.h"
#include "XWindow_CBResultNotice.h"
#include "XWindow_CastleBattlePersonalHistory.h"
#include "XWindow_ThreatenGauge.h"
#include "XWindow_Mark.h"
#include "XWindow_PKTracingRequest.h"
#include "XWindow_PK_ListBox.h" // Author : 양희왕
#include "XWindow_SmeltItem.h" //Author : 양희왕
#include "XWindow_AgeGrade.h" //Author : 양희왕
#include "XWindow_EventIcon.h" //Author : 양희왕
#include "Xwindow_WorldMap.h"
#include "XWindow_RecallItem.h" //Author : 양희왕 
#include "XWindow_CBRecord.h"
#include "XWindow_CBOrgRecord.h"
#include "XWindow_CBRewardSmall.h"
#include "XWindow_SocketDetach.h" //Author : 양희왕
#include "XWindow_BlackList.h"
#include "XWindow_Poison.h"
#include "Xwindow_Exp.h"
#include "XWindow_MixWeapon.h"
#include "XWindow_MatchEvent.h"
#include "Xwindow_RewardNick.h"
#include "XWindow_Info.h"
#include "XWindow_FreeMatchInfo.h"
#include "XWindow_TargetInfoSelect.h" //Author : 양희왕
#include "XWindow_NPCMnDInfo.h" //Author : 양희왕 //breif :사제 시스템
#include "XWindow_MnDTotalPoint.h" //Author : 양희왕 //breif :사제 시스템
#include "XWindow_MnDCurrentPoint.h" //Author : 양희왕 //breif :사제 시스템

#include "XMLPaser.h" //Author : 양희왕 
#include "XSetItemInfomation.h" //Author : 양희왕
#include "XWindow_NPCResource_New.h"

#include "XCrypto.h"
#include "../Library/CWebPage/XWebPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=__FILE__;
#endif

extern void ReplaceCharacter(LPSTR _string, TCHAR srcchar, TCHAR changechar);

extern _map_NPCBuffStr				g_NPCBuffStr;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int _XDEF_WINDOW_DEFAULTWIDTH = 183;
static int _XDEF_WINDOW_DEFAULTHEIGHT = 270;

extern CCapsulateCrypto g_PacketCrypto;

_XWindow_MainFrame*			g_pMainFrame_Window = NULL;
_XWindow_AreaSelect*		g_pBlackClanAreaSelect_Window = NULL;
_XWindow_AreaSelect*		g_pWhiteClanAreaSelect_Window = NULL;
_XWindow_AreaSelect*		g_pMultiAreaSelect_Window = NULL;
_XWindow_Mark*				g_pMark_Window = NULL;
_XWindow_WorldMinimap*		g_pMinimap_Window = NULL;

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트아이템
XSetItemInfomation*	g_pSetIteminfo = NULL;
#endif

XProc_FirstLoad::XProc_FirstLoad()
{
	setProcessInfo( 0, "First loading", __FILE__ );
	
#ifdef _ACCLAIM_IGAADSYSTEM
	m_AcclaimADMode = FALSE;
#endif

}

XProc_FirstLoad::~XProc_FirstLoad()
{

}

BOOL XProc_FirstLoad::InitializeProcess( void )
{	
	g_NormalGameExit = TRUE;
	
	//////////////////////////////////////////////////////////////////////////////////
	// Camera setting
	m_Player.SetPitchLimits(-45, 45);
	m_Player.SetRollLimits(-10, 10);	
	m_Player.SetType(camFly);
		
	_XSetDefaultPath();
	
	m_TextureArchive.DisposeTexture();

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_INTERFACE.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_INTERFACE.XP") );
#endif

	m_TextureArchive.SetTexturePackage( packagefilename );

	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{	
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex("MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
		//m_TextureArchive.GetResourceIndex("MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex("vn_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
		//m_TextureArchive.GetResourceIndex("vn_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex("tw_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
		//m_TextureArchive.GetResourceIndex("tw_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}
	/*else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		m_TextureArchive.GetResourceIndex("us_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}*/
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex("jp_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
		//m_TextureArchive.GetResourceIndex("tw_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex("rs_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
		//m_TextureArchive.GetResourceIndex("tw_MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}
	else
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex("MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
		//m_TextureArchive.GetResourceIndex("MI_FirstLoading.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}	

	m_TextureArchive.BuildTextureList();

	/*
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{	
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.m_TextureArchive.FindResource( "MI_FirstLoading.jpg" ) );
		//m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, &m_TextureArchive, m_TextureArchive.FindResource( "MI_FirstLoading.jpg" ) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.m_TextureArchive.FindResource( "vn_MI_FirstLoading.jpg" ) );
		//m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, &m_TextureArchive, m_TextureArchive.FindResource( "vn_MI_FirstLoading.jpg" ) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.m_TextureArchive.FindResource( "tw_MI_FirstLoading.jpg" ) );
		//m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, &m_TextureArchive, m_TextureArchive.FindResource( "tw_MI_FirstLoading.jpg" ) );
	}
	else
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.m_TextureArchive.FindResource( "MI_FirstLoading.jpg" ) );
		//m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, &m_TextureArchive, m_TextureArchive.FindResource( "MI_FirstLoading.jpg" ) );
	}
	*/
	
	//m_BackgroundStatic.SetClipRect( 0, 0, gnWidth, gnHeight );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_INITGAME) );
	//SetMessage( "Loading...." );

	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

#ifdef _ACCLAIM_IGAADSYSTEM
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );

#ifdef _XTESTSERVER		
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_MAININTERFACE.XP") );
#else	
	strcat( packagefilename, _T("\\Data\\Texture\\TR_MAININTERFACE.XP") );
#endif

	g_TextureMemoryPool			=	D3DPOOL_MANAGED;
	if( !g_MainInterfaceTextureArchive.LoadTexturePackage( packagefilename, _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE, TRUE ) ) 
	{
		_XFatalError( "Can't load main interface image package" );
		return FALSE;
	}	

#ifdef _XTESTSERVER	
	g_MainInterfaceTextureArchive.AttachRenderStyle( "tsInterface.xrs" );
#else
	g_MainInterfaceTextureArchive.AttachRenderStyle( "Interface.xrs" );
#endif

	if( !g_MainInterfaceTextureArchive.InitDeviceObject() )
	{		
		g_TextureMemoryPool		=	D3DPOOL_MANAGED;
		return FALSE;
	}
	g_TextureMemoryPool		=	D3DPOOL_MANAGED;
	
	m_AcclaimADMode = TRUE;

	g_IGAADWrapper.Initialize();

	g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_GAMELOADING );
	g_IGAADWrapper.ShowWindow( TRUE );
	g_IGAADWrapper.StartDisplay();
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	
	DisplayHTMLPage(g_hRubiconADWebBrowserObject, "http://www.acclaim.com/ads/9d_house_728_90.html");

	POINT pt = { 0, 0 };
	::ClientToScreen( gHWnd, &pt );

	g_RubiconADWindowPosition.x = (gnWidth>>1)-(728>>1);
	g_RubiconADWindowPosition.y = gnHeight - 140;
	g_RubiconADWindowSize.cx = 728;
	g_RubiconADWindowSize.cy = 90;

	::SetWindowPos(g_hRubiconADWindow, NULL, pt.x + (gnWidth>>1)-(728>>1), pt.y + gnHeight - 140, 728, 90, 0);
	::SetWindowPos(g_hRubiconADWebBrowserObject, NULL, 0, 0, 728, 90, 0);
				
	g_RubiconADFirstTimeView = TRUE;	

#endif

	return TRUE;
}


void XProc_FirstLoad::SetMessage( LPTSTR message )
{
	memset( m_Message, 0, sizeof(TCHAR) * 256 );

	if( !message || strlen(message) <= 0 )  return;
	
	strcpy( m_Message, message );
	
#ifdef _XRUSSIAN
	messagecoord.x = (gnWidth>>1) - 12;
#else
	messagecoord.x = (gnWidth>>1);	
#endif
	messagecoord.y = (gnHeight) - g_XBaseFont->GetHeight() - 20;
}


//////////////////////////////////////////////////////////////////////////////////
// 로딩 프로세스 종료
void XProc_FirstLoad::DestroyProcess( void )
{
	m_TextureArchive.DisposeTexture();
	m_BackgroundStatic.ReleaseObject();	
	SetEnableProcess( FALSE );

#ifdef _ACCLAIM_RUBICONADSYSTEM
	::ShowWindow(g_hRubiconADWindow, SW_HIDE);
#endif

}

//////////////////////////////////////////////////////////////////////////////////
// Initialize device defendent object
bool XProc_FirstLoad::InitDeviceObject( void )
{		
	// Set the camera view and projection matrices	
	m_Player.Set(m_Player.GetType(), 60, (float)gnWidth/(float)gnHeight, 0.001f, 10.0f);
	m_Player.UpdateViewMatrix();
	m_Player.UpdateProjMatrix();
	
	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = gnWidth;
	vp.Height = gnHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	gpDev->SetViewport(&vp);
	
	// Set matrices
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_Player.GetProjMatrix());
	
	// Wall texture just point mip mapping with linear filtering
	gpDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 16비트 모드일 경우 디더링을 켜준다.
	//if( gfmtDepth == D3DFMT_D16 ) gpDev->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	//else gpDev->SetRenderState( D3DRS_DITHERENABLE,   FALSE ); 
	
    gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    
	gpDev->SetRenderState( D3DRS_ZENABLE,        g_RS_UseZBuffer );
	gpDev->SetRenderState( D3DRS_ZFUNC,			 g_RS_ZBufferFunc );
	
	gpDev->SetRenderState( D3DRS_AMBIENT, 
		//D3DCOLOR_COLORVALUE( 0.8, 0.8, 0.8, 1.0 ) );
		//D3DCOLOR_COLORVALUE( 0.2, 0.2, 0.2, 1.0 ) );
		D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 ) );
	
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1  );
	
	gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	
	// Alpha blend state
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );		
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	// Alpha test state
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	
	D3DLIGHT9	light;
	D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0.0f, 160.0f, 0.0f );
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 0.5f;
	light.Range = 100.0f;
	gpDev->SetLight( 0, &light );
	
	gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );	
	gpDev->LightEnable( 0, TRUE );	
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Clear device defendent object
void XProc_FirstLoad::ReleaseDeviceObject( void )
{		
	
}

void XProc_FirstLoad::PauseProcess( void )
{
	
}

void XProc_FirstLoad::UnPauseProcess( void )
{
	
}

void XProc_FirstLoad::Draw( void )
{

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	Sleep( 100 );
#endif

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);	

	// Begin the scene
	gpDev->BeginScene();

	m_Player.UpdateViewMatrix();
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());
	
	//m_BackgroundStatic.Draw();
	m_BackgroundStatic.Render();

#ifdef _ACCLAIM_IGAADSYSTEM
	if( m_AcclaimADMode )
	{
		g_IGAADWrapper.Render();
	}
	else
	{
#endif

	// Draw...
	g_XBaseFont->SetAlphaBlendingEnable( TRUE );
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	
	g_XBaseFont->PrintAlign( messagecoord.x, messagecoord.y,1.0f, _XFONT_ALIGNTYPE_CENTER, m_Message );	
	g_XBaseFont->Flush();
	g_XBaseFont->SetAlphaBlendingEnable( FALSE );
	
#ifdef _XDWDEBUG 
	/*
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	
	g_XBaseFont->PrintAlign( 0, 0,1.0f, _XFONT_ALIGNTYPE_LEFT, "FPS:%d", gnFPS );
	g_XBaseFont->Flush();	
	*/
#endif
	
#ifdef _ACCLAIM_IGAADSYSTEM
	}
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->SetBoldMode(FALSE);
	g_XBaseFont->Puts( (gnWidth>>1) - 302, messagecoord.y, _T("This ad is not clickable.") );
	g_XBaseFont->Flush();
#endif

	g_MessageBox.Draw();
		
	// End the scene
	gpDev->EndScene();
			
	// Present the backbuffer contents to the display
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);		
	
	if( D3DERR_DEVICELOST == hr ) gApp->m_bDeviceLost = true;

#ifdef _ACCLAIM_RUBICONADSYSTEM
	if( g_RubiconADFirstTimeView )
	{
		BOOL bGotMsg = FALSE;
		MSG  msg;
		DWORD time = timeGetTime();
		while(1)
		{		
			bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
			
			if(bGotMsg)
			{
				TranslateMessage(&msg);			
				DispatchMessage(&msg);
			}
			if( timeGetTime() - time > 3000 )
			{
				break;
			}
		}

		::ShowWindow(g_hRubiconADWindow, SW_SHOW);
		::UpdateWindow( g_hRubiconADWindow );	
		::RedrawWindow( g_hRubiconADWindow, NULL, NULL, RDW_UPDATENOW );
		g_RubiconADFirstTimeView = FALSE;
	}
	
#endif
}

void XProc_FirstLoad::OnPaint(WPARAM wparam, LPARAM lparam)
{	
	Draw();
}

bool XProc_FirstLoad::Process( void )
{

#ifdef _ACCLAIM_IGAADSYSTEM
	if( m_AcclaimADMode )
	{
		g_IGAADWrapper.Process();

		if( !g_IGAADWrapper.GetShowStatus() )
		{
			m_AcclaimADMode = FALSE;
		}
		else
		{
			return true;
		}
	}
#endif	

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	Sleep( 100 );
#endif

#ifdef _XDEF_MODELTHREADLOADING
	//g_LoadMeshThread.SetModelArchieve( &g_ModelArchive );
	g_LoadMeshThread.SetModelArchieve( &g_ThreadModelArchive );
#endif

	_XMEMORYUSECHECKREADY
	_XMEMORYUSECHECKSTART

	TCHAR packagefilename[_MAX_PATH];
	
#ifndef _ACCLAIM_IGAADSYSTEM
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_MAININTERFACE.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_MAININTERFACE.XP") );
#endif

	g_TextureMemoryPool			=	D3DPOOL_MANAGED;
	if( !g_MainInterfaceTextureArchive.LoadTexturePackage( packagefilename, _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE, TRUE ) ) 
	{
		_XFatalError( "Can't load main interface image package" );
		return false;
	}	

#ifdef _XTESTSERVER	
	g_MainInterfaceTextureArchive.AttachRenderStyle( "tsInterface.xrs" );
#else
	g_MainInterfaceTextureArchive.AttachRenderStyle( "Interface.xrs" );
#endif

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_INITINTERFACE) );
	Draw();	
	
	if( !g_MainInterfaceTextureArchive.InitDeviceObject() )
	{		
		g_TextureMemoryPool		=	D3DPOOL_MANAGED;
		return false;
	}
	g_TextureMemoryPool		=	D3DPOOL_MANAGED;
	_XMEMORYUSECHECKEND( "Load main interface texture" );

#endif //#ifndef _ACCLAIM_IGAADSYSTEM

	// Load crypto db
	if( !g_ScriptArchive.ReOpenPackage() )
	{
			_XFatalError( "ERROR : 1706" );
			return FALSE;
	}

	FILE* pScriptFile = g_ScriptArchive.GetPackedFile( _T("lump.dat") );
		
	if( pScriptFile )
	{
		if( !g_PacketCrypto.Init(pScriptFile) )
		{
			_XFatalError( "ERROR : 1704" );
			return FALSE;
		}
	}
	else
	{
		_XFatalError( "ERROR : 1705" );
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	g_ScriptArchive.ClosePackage();
	
	Draw();

#ifdef _XUSEENCRYPTPATTERN
	extern BYTE g_EncyptPatternTable[256][32];
	for( int i = 0; i < 256; i++ ) // lump table runtime xor.
	{
		for( int j = 0; j < 32; j++ )
			g_EncyptPatternTable[i][j] ^= 0x3E;
	}
#endif

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_TERRAIN) );
	Draw();
	// 메인게임에서 미리 초기화 해야 하는 부분------------------------------

	g_LodTerrain.Initialize();

	_XMEMORYUSECHECKSTART
	g_LodTerrain.GenerateDetailLevels();
	_XMEMORYUSECHECKEND( "Terrain generate detail levels" );

	_XMEMORYUSECHECKSTART
	if( !g_LodTerrain.CreateTerrainVertex() )
	{
		_XFatalError( "Create Terrain vertices" );
		return false;
	}
	_XMEMORYUSECHECKEND( "Create default terrain vertices" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_DETAILMAP) );
	Draw();

//	if( !InitializeBGMList() ) return false;

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_AREA) );
	Draw();

	if( !InitialzeZoneTable() )
	{
		_XFatalError( "Can't intialize zone information table" );
		return false;
	}

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_USERDATABASE) );
	Draw();
	// 유저 레벨 LUT 생성
	_XLocalUser::InitializeUserDataTable();


	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_INTERFACE) );
	Draw();

	_XMEMORYUSECHECKSTART
	if( !CreateMainInterface() )
	{
		_XFatalError( "Can't create main interface" );
		return false;
	}
	_XMEMORYUSECHECKEND( "Create main interface" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_CHARACTERMODEL) );
	Draw();

	//_XMEMORYUSECHECKSTART
	if( !LoadCharacterModel() )
	{
		_XFatalError( "Can't load character model" );
		return false;
	}
	//_XMEMORYUSECHECKEND( "Load character model" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_MONSTERMODEL) );
	Draw();	

	if( !LoadMonsterModel() )
	{
		_XFatalError( "Can't load monster model" );
		return false;
	}

	_XWindow_GM_MonsterManage* pWindow = (_XWindow_GM_MonsterManage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCONTROLMONSTERWINDOW);
	if( pWindow )
	{
		pWindow->UpdateMonsterList();
	}

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_NPCMODEL) );
	Draw();	
		
	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_REALTIMEOBJECT) );
	Draw();	

	_XMEMORYUSECHECKSTART
	if( !LoadObjectModel() )
	{
		_XFatalError( "Can't load rtime object model" );
		return false;
	}
	_XMEMORYUSECHECKEND( "Load realtime object model" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_EFFECTDATA) );
	
	Draw();	

	_XMEMORYUSECHECKSTART

	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	
#ifdef _XTESTSERVER
	strcat( packagefilename, _T("tsTR_EFFECT.XP") );
#else
	strcat( packagefilename, _T("TR_EFFECT.XP") );
#endif

	if( !g_EffectTextureArchive.LoadTexturePackage( packagefilename ) )
	{
		_XFatalError( "Can't load special effect texture" );
		return false;
	}

	if( !g_EffectTextureArchive.InitDeviceObject() )
	{
		_XFatalError( "Can't Initialize special effect texture" );
		return false;
	}	
	_XMEMORYUSECHECKEND( "Initialize special effect texture" );
	

	_XMEMORYUSECHECKSTART
	g_SwordEffectController.InitEffect( &g_EffectTextureArchive );
	_XMEMORYUSECHECKEND( "Initialize sword effect" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_EFFECTSCRIPT) );
	Draw();	

	_XMEMORYUSECHECKSTART
	if( !LoadEffectScript() )
	{
		_XFatalError( "Can't Load effect script" );
		return false;
	}
	_XMEMORYUSECHECKEND( "Load effect script" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_CHATFILTERING) );
	Draw();	

	_XMEMORYUSECHECKSTART
	if( !g_WordFilteringManager.DataInit() )
	{
		_XFatalError( "Can't Load filtering DB table" );
		return false;
	}
	_XMEMORYUSECHECKEND( "Load word filtering data" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_LODOBJECT) );
	Draw();	

	_XMEMORYUSECHECKSTART
	if( !InitializeSpeedTreeWrapper() )
	{
		_XFatalError( "Can't Initialize tree object" );
		return false;
	}
	_XMEMORYUSECHECKEND( "Initialize speed tree wrapper" );

	// Quest script load
	SetMessage(_XGETINTERFACETEXT(ID_STRING_QUEST_LOADINGSCRIPT));
	Draw();

	_XMEMORYUSECHECKSTART
	if(!LoadQuestScript())
	{
		_XFatalError("Can't load quest script");
		return false;
	}
	_XMEMORYUSECHECKEND( "Load quest script" );

	_XMEMORYUSECHECKSTART
	if(!LoadSceneList())
	{
		_XFatalError("Can't load scene list");
		return false;
	}
	_XMEMORYUSECHECKEND( "Load scene list" );
	
	_XMEMORYUSECHECKSTART
	if(!LoadLifeSkillScript())
	{
		_XFatalError("Can't load lifeskill script");
		return false;
	}
	_XMEMORYUSECHECKEND( "Load lifeskill script" );

	
	_XMEMORYUSECHECKSTART
	if(!LoadMatchScript())
	{
		_XFatalError("Can't load match script");
		return false;
	}
	_XMEMORYUSECHECKEND( "Load match script" );
	
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	_XMEMORYUSECHECKSTART
	if(!g_MatchEventManager.LoadMatchEventScript())
	{
		_XFatalError("Can't load match_event(pvp_match) script");
		return false;
	}
	_XMEMORYUSECHECKEND( "Load match_event script" );
#endif

	_XMEMORYUSECHECKSTART
	if(!LoadNPCBuffScript())
	{
		_XFatalError("Can't load npc buff script");
		return false;
	}
	_XMEMORYUSECHECKEND("Load npc buff script");

#ifdef _XTS_FAME
	_XMEMORYUSECHECKSTART
		if(!LoadFameScript())
		{
			_XFatalError("Can't load fame script");
			return false;
		}
		_XMEMORYUSECHECKEND("Load fame script");
#endif

#ifdef _XDEF_CASTLEBATTLE
	_XMEMORYUSECHECKSTART
		if(!g_CBManager.Load())
		{
			g_CBManager.Release();
			_XFatalError("Can't load Manor_map_script");
			return false;
		}
		_XMEMORYUSECHECKEND("Load Manor_map_script");
#endif

#ifdef _XTS_ITEMMALLBROWSER		
		_XMEMORYUSECHECKSTART
		if( !LoadCashItemPackage() )
		{
			_XFatalError("Can't load cashitem package script");
			return false;
		}
		_XMEMORYUSECHECKEND("Load cashitem package script");
#endif

	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

#ifdef _XDWDEBUG
	if(_XCmdLineOption("xsingleplay",0) != 0)
	{		
		g_SinglePlayMode = TRUE;		
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_loading );
		return true;
	}
#endif	

#ifdef _XDWDEBUG
	if(_XCmdLineOption("xzerolevelplay",0) != 0)
	{
		g_indexScene = 0;
		((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_bLockEndProcess = TRUE;
		((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_ZEROLEVEL;
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 	
//		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_selecthero); 	
		return true;
	}	
#endif
	
#ifdef _XDWDEBUG
	if(_XCmdLineOption("xbattlelog", 0) != 0)
	{
		g_BattleLog.SetWriteLog(TRUE);
	}
#else
	if(_XCmdLineOption("xbattlelog", 0) != 0)
	{
//		if(g_LanguageType != _XLANGUAGE_TYPE_KOREAN)
		{
			g_BattleLog.SetWriteLog(TRUE);
		}
	}
#endif

	SetMessage( _XGETINTERFACETEXT(ID_STRING_FIRSTLOAD_ENDINIT) );
	Draw();	

	//hotblood zerolevel
	// 제로레벨 롹
	/*BOOL playzerolevel = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"PlayZeroLevel", 1,TRUE );	
	if( playzerolevel )
	{
		g_indexScene = 0;
		((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_bLockEndProcess = TRUE;
		((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_ZEROLEVEL;
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 	
	}
	else*/
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
	//((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_createcharacter );


	return true;
}

void XProc_FirstLoad::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( message == WM_COMMAND )
	{
		//switch( wparam )
		{

		}		
	}
}

void XProc_FirstLoad::OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		if( !g_NetworkKernel.m_bConnected )
		{
			g_NetworkKernel.ProcessUDPNetworkError();
			_XLog( "ERROR : Firstload:OnUserMessage_UDPSocket::Disconnected" );
		}
		return;
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{
	case FD_CONNECT:		
		WSAAsyncSelect(g_NetworkKernel.m_hLoginUDPSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
		g_NetworkKernel.m_bLoginConnected = TRUE;
		_XDWINPRINT( "Connected login server" );
		break;
	case FD_READ :
		g_NetworkKernel.UDPReadPacket();
		break;
	case FD_CLOSE :		
		g_NetworkKernel.ProcessNetworkError();		
		_XDWINPRINT( "SERVER MESSAGE : Login socket close message" );
		break;
	}
}

void XProc_FirstLoad::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		if( !g_NetworkKernel.m_bConnected )
		{
			g_NetworkKernel.ProcessNetworkError();			
		}
		return;
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{
	case FD_CONNECT:
		WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
		g_NetworkKernel.m_bConnected = TRUE;

		g_NetworkKernel.Login();
		
		_XDWINPRINT( "Connected server" );
		break;
	case FD_READ :
		g_NetworkKernel.ReadSocket();
		break;
	case FD_CLOSE :
		g_NetworkKernel.ProcessNetworkError();		
		_XDWINPRINT( "SERVER MESSAGE : Socket close message" );
		break;
	}
}

void XProc_FirstLoad::OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam )
{
	
}
	
bool XProc_FirstLoad::OnKeyboardPoll(_XInput *pInput)
{ 	
	return true; 
}

bool XProc_FirstLoad::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	return true;
}

bool XProc_FirstLoad::OnKeyDown(WPARAM wparam, LPARAM lparam)
{ 	
	
	return true; 
}

bool XProc_FirstLoad::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{ 					
	return true; 
}

bool XProc_FirstLoad::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
	if( ::g_CurrentFocusedObject ) return true;

	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( gHWnd, &pt );	

	switch( p_MB )
	{
	case _XmbLeft :
		if( bPushed )
		{

		}
		break;
	case _XmbRight :
		if( bPushed )
		{
			
		}
		
		break;
	case _XmbMiddle :
		if( bPushed )
		{
			
		}
		
		break;
	}

	return true; 
}	


void XProc_FirstLoad::OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( g_BgmPlayFlag )
	{		
		
	}	
}

BOOL XProc_FirstLoad::LoadSceneList( void )
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
	FILE* scenelistfile = NULL;

	scenelistfile = g_ScriptArchive.GetPackedFile( _T("scenelist.ini") );
	
	if( !scenelistfile )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	TCHAR buffer[256];
	
	if( feof( scenelistfile ) ) 
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	for( int i = 0; i < _XDEF_MAXFLASHCOUNT; i++)
	{		
		g_SceneList[i] = NULL;
	}
	
	int index = 0;
	do 
	{						
		if( index >= _XDEF_MAXFLASHCOUNT )
		{			
			break;
		}

		if( feof(scenelistfile) ) break;
		
		memset( buffer, 0, sizeof(TCHAR)*256 );
		if( !fgets( buffer, 255, scenelistfile ) )
		{
			break;
		}		
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		
		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			g_SceneList[index] = new TCHAR[256];
			
			buffer[ strlen(buffer)-2 ] = _T('\0');		
			
			if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
			{			
				strcpy(g_SceneList[index], buffer);
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{
				strcpy(g_SceneList[index], _T("vn_") );
				strcat(g_SceneList[index], buffer );
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
			{
				strcpy(g_SceneList[index], _T("us_") );
				strcat(g_SceneList[index], buffer );
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
			{
				strcpy(g_SceneList[index], _T("tw_") );
				strcat(g_SceneList[index], buffer );
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
			{
				strcpy(g_SceneList[index], _T("jp_") );
				strcat(g_SceneList[index], buffer );
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				strcpy(g_SceneList[index], _T("rs_") );
				strcat(g_SceneList[index], buffer );
			}
			else
			{
				strcpy(g_SceneList[index], buffer);
			}
			
			index++;
		}
		
	} while( !feof(scenelistfile) );
	
	g_ScriptArchive.ClosePackage();
	return TRUE;
}
BOOL XProc_FirstLoad::LoadLifeSkillScript( void )
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	// 명품수집 스크립트
	FILE* pCollectionFileInfo = NULL;	

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pCollectionFileInfo = g_ScriptArchive.GetPackedFile( _T("vn_Collection.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pCollectionFileInfo = g_ScriptArchive.GetPackedFile( _T("tw_Collection.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pCollectionFileInfo = g_ScriptArchive.GetPackedFile( _T("us_Collection.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		pCollectionFileInfo = g_ScriptArchive.GetPackedFile( _T("jp_Collection.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pCollectionFileInfo = g_ScriptArchive.GetPackedFile( _T("rs_Collection.txt") );
	}
	else
	{
		pCollectionFileInfo = g_ScriptArchive.GetPackedFile( _T("Collection.txt") );
	}
	
	if( !pCollectionFileInfo )
	{
		_XFatalError("Can't load collection script");
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	if(!g_CollectionManager.LoadCollectionItem(pCollectionFileInfo) )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
		
	// 자원수집 스크립트
	FILE* pResourceFileInfo = NULL;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pResourceFileInfo = g_ScriptArchive.GetPackedFile( _T("vn_Resource.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pResourceFileInfo = g_ScriptArchive.GetPackedFile( _T("tw_Resource.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pResourceFileInfo = g_ScriptArchive.GetPackedFile( _T("us_Resource.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		pResourceFileInfo = g_ScriptArchive.GetPackedFile( _T("jp_Resource.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pResourceFileInfo = g_ScriptArchive.GetPackedFile( _T("rs_Resource.txt") );
	}
	else
	{
		pResourceFileInfo = g_ScriptArchive.GetPackedFile( _T("Resource.txt") );
	}
	
	if( !pResourceFileInfo )
	{
		_XFatalError("Can't load resource script");
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	if(!g_ResourceManager.LoadResourceItem(pResourceFileInfo) )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	g_ScriptArchive.ClosePackage();

	return TRUE;
}

BOOL XProc_FirstLoad::LoadCashItemPackage( void )
{
#ifdef _XTS_ITEMMALLBROWSER
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
	FILE* fileptr = NULL;	
	fileptr = g_ScriptArchive.GetPackedFile( _T("CashItemPackage.txt") );
	
	if(!g_CashItemPackageManger.LoadCashItemPackage(fileptr) )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	g_ScriptArchive.ClosePackage();
	
#endif
	return TRUE;
}

BOOL XProc_FirstLoad::InitialzeZoneTable( void )
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* zoneinfofile = NULL;

	zoneinfofile = g_ScriptArchive.GetPackedFile( _T("Zonelist.ini") );

	if( !zoneinfofile )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	int index = 0;
	TCHAR buffer[256];
	TCHAR tempbuffer[512];

	memset( g_ZoneInfoTable, 0, sizeof(_XZone_Info)*_XDEF_MAXZONECOUNT );	

	do 
	{		
		if( index >= _XDEF_MAXZONECOUNT )
		{			
			break;
		}
		
		if( feof(zoneinfofile) ) break;

		memset( buffer, 0, sizeof(TCHAR)*256 );
		if( !fgets( buffer, 255, zoneinfofile ) )
		{
			break;
		}

		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;

		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			int zoneid = 0, zoneindex = 0;
			
			sscanf( buffer, "%d", &zoneid);	
			zoneindex = zoneid -1;

#ifdef _XDEF_NEWSTRINGMANAGER	
#ifdef _XTS_PK
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
			int count = sscanf( buffer, "%d %s %s %s %s %f %f %f %d %s %s %d %d %d %d", &g_ZoneInfoTable[zoneindex].zoneid, 
																	tempbuffer,
																	tempbuffer,
																	tempbuffer,
																	g_ZoneInfoTable[zoneindex].zonefilename,
																	&g_ZoneInfoTable[zoneindex].basepoint.x,
																	&g_ZoneInfoTable[zoneindex].basepoint.y,
																	&g_ZoneInfoTable[zoneindex].basepoint.z,
																	&g_ZoneInfoTable[zoneindex].servicezone,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename1,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename2,
																	&g_ZoneInfoTable[zoneindex].mapsize,
																	&g_ZoneInfoTable[zoneindex].clantype,
																	&g_ZoneInfoTable[zoneindex].pkclientinfo,
																	&g_ZoneInfoTable[zoneindex].zonepenalty);
			if(count <= 14)
			{
				g_ZoneInfoTable[zoneindex].zonepenalty = 0;
			}
#else
			sscanf( buffer, "%d %s %s %s %s %f %f %f %d %s %s %d %d %d", &g_ZoneInfoTable[zoneindex].zoneid, 
																	tempbuffer,
																	tempbuffer,
																	tempbuffer,
																	g_ZoneInfoTable[zoneindex].zonefilename,
																	&g_ZoneInfoTable[zoneindex].basepoint.x,
																	&g_ZoneInfoTable[zoneindex].basepoint.y,
																	&g_ZoneInfoTable[zoneindex].basepoint.z,
																	&g_ZoneInfoTable[zoneindex].servicezone,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename1,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename2,
																	&g_ZoneInfoTable[zoneindex].mapsize,
																	&g_ZoneInfoTable[zoneindex].clantype,
																	&g_ZoneInfoTable[zoneindex].pkclientinfo);
#endif

			g_ZoneInfoTable[zoneindex].zoneshortname = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ZONELIST_INI_SHORT, g_ZoneInfoTable[zoneindex].zoneid );
			g_ZoneInfoTable[zoneindex].zonename		 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ZONELIST_INI_LONG, g_ZoneInfoTable[zoneindex].zoneid );
			g_ZoneInfoTable[zoneindex].zonename2	 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ZONELIST_INI_TITLE, g_ZoneInfoTable[zoneindex].zoneid );

			g_ZoneInfoTable[zoneindex].pkserverinfo = 0;		// server에서 오는 정보로 setting 할 것

#else
			sscanf( buffer, "%d %s %s %s %s %f %f %f %d %s %s %d %d", &g_ZoneInfoTable[zoneindex].zoneid, 
																	tempbuffer,
																	tempbuffer,
																	tempbuffer,
																	g_ZoneInfoTable[zoneindex].zonefilename,
																	&g_ZoneInfoTable[zoneindex].basepoint.x,
																	&g_ZoneInfoTable[zoneindex].basepoint.y,
																	&g_ZoneInfoTable[zoneindex].basepoint.z,
																	&g_ZoneInfoTable[zoneindex].servicezone,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename1,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename2,
																	&g_ZoneInfoTable[zoneindex].mapsize,
																	&g_ZoneInfoTable[zoneindex].clantype);

			g_ZoneInfoTable[zoneindex].zoneshortname = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ZONELIST_INI_SHORT, g_ZoneInfoTable[zoneindex].zoneid );
			g_ZoneInfoTable[zoneindex].zonename		 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ZONELIST_INI_LONG, g_ZoneInfoTable[zoneindex].zoneid );
			g_ZoneInfoTable[zoneindex].zonename2	 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ZONELIST_INI_TITLE, g_ZoneInfoTable[zoneindex].zoneid );
#endif // _XTS_PK

#else
			
#ifdef _XTS_PK
			sscanf( buffer, "%d %s %s %s %s %f %f %f %d %s %s %d %d %d", &g_ZoneInfoTable[zoneindex].zoneid, 
																	g_ZoneInfoTable[zoneindex].zoneshortname,
																	g_ZoneInfoTable[zoneindex].zonename,
																	g_ZoneInfoTable[zoneindex].zonename2,
																	g_ZoneInfoTable[zoneindex].zonefilename,
																	&g_ZoneInfoTable[zoneindex].basepoint.x,
																	&g_ZoneInfoTable[zoneindex].basepoint.y,
																	&g_ZoneInfoTable[zoneindex].basepoint.z,
																	&g_ZoneInfoTable[zoneindex].servicezone,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename1,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename2,
																	&g_ZoneInfoTable[zoneindex].mapsize,
																	&g_ZoneInfoTable[zoneindex].clantype,
																	&g_ZoneInfoTable[zoneindex].pkclientinfo);

			g_ZoneInfoTable[zoneindex].pkserverinfo = 0;		// server에서 오는 정보로 setting 할 것
#else
			sscanf( buffer, "%d %s %s %s %s %f %f %f %d %s %s %d %d", &g_ZoneInfoTable[zoneindex].zoneid, 
																	g_ZoneInfoTable[zoneindex].zoneshortname,
																	g_ZoneInfoTable[zoneindex].zonename,
																	g_ZoneInfoTable[zoneindex].zonename2,
																	g_ZoneInfoTable[zoneindex].zonefilename,
																	&g_ZoneInfoTable[zoneindex].basepoint.x,
																	&g_ZoneInfoTable[zoneindex].basepoint.y,
																	&g_ZoneInfoTable[zoneindex].basepoint.z,
																	&g_ZoneInfoTable[zoneindex].servicezone,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename1,
																	g_ZoneInfoTable[zoneindex].zonebgmfilename2,
																	&g_ZoneInfoTable[zoneindex].mapsize,
																	&g_ZoneInfoTable[zoneindex].clantype);
#endif // _XTS_PK

#endif // _XDEF_NEWSTRINGMANAGER

		//	index++;
		}
		
	} while( !feof(zoneinfofile) );
	

	/*
	for( int i = 0; i < _XDEF_MAXZONECOUNT; i++ )
	{
		_XLog( "%d, %s", i, g_ZoneInfoTable[i].zonefilename );
	}
	*/
	

	// Read guard mob precaching info
	memset( g_GuardMobPrecachingInfo, 0, sizeof(_XGuardMobPrecachingInfo) * _XDEF_MAXZONECOUNT );

	FILE* guardmobinfofile = g_ScriptArchive.GetPackedFile( _T("patrol_info.txt") );

	if( !guardmobinfofile )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	int guardspawnzoneindex = -1;

	do 
	{				
		if( feof(guardmobinfofile) ) break;

		memset( buffer, 0, sizeof(TCHAR)*256 );
		if( !fgets( buffer, 255, guardmobinfofile ) )
		{
			break;
		}

		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;

		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			if( strncmp( buffer, _T("ZONE"), 4 ) == 0 ) 
			{
				TCHAR dummystring[64];
				sscanf( buffer, "%s %d",dummystring, &guardspawnzoneindex );
				guardspawnzoneindex -= 1;
			}
			else
			{
				if( guardspawnzoneindex < 0 )
				{
					_XFatalError( _T("Patrol_Info.txt : Invalid script %d"), guardspawnzoneindex );
					g_ScriptArchive.ClosePackage();
					return FALSE;
				}

				int monsterid = atoi( buffer ) - 4000;
				if( monsterid <= 0 || monsterid >= _XDEF_MODELCLASSCOUNT_MOB )
				{
					_XFatalError( _T("Patrol_Info.txt : Invalid monster ID %d"), monsterid );
					g_ScriptArchive.ClosePackage();
					return FALSE;
				}

				if( g_GuardMobPrecachingInfo[guardspawnzoneindex].count < _XDEF_GUARDMOBMAXPRECACHINGCOUNT )
				{
					g_GuardMobPrecachingInfo[guardspawnzoneindex].spawnlist[ g_GuardMobPrecachingInfo[guardspawnzoneindex].count ] = monsterid;
					g_GuardMobPrecachingInfo[guardspawnzoneindex].count++;
				}
				else 
				{
					_XLog( "Overflowed guard info..." );
				}
			}
		}
	} while( !feof(guardmobinfofile) );


	g_ScriptArchive.ClosePackage();
	return TRUE;
}


BOOL XProc_FirstLoad::LoadCharacterModel( void )
{
	int i = 0;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_CHARACTER.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_CHARACTER.XP") );
#endif

	if( !g_CharacterTextureArchive.SetTexturePackage( packagefilename ) ) return FALSE;

	if( !g_ModelArchive.ReOpenPackage() ) 
		return FALSE;

	if( !g_AniArchive.ReOpenPackage() ) 
		return FALSE;
		
	g_FemaleBipedObject.SetCheckSoundPlaying( false );
	g_MaleBipedObject.SetCheckSoundPlaying( false );

	_XMEMORYUSECHECKREADY
	
	_XMEMORYUSECHECKSTART

	if( !_XLoadCharacterBipedScripter( &g_MaleBipedObject, "MaleBiped.XMS" ) )
	{
		goto _charactermodelloaderrorprocess;
	}
	
	_XMEMORYUSECHECKEND( "Load male biped" );

	_XMEMORYUSECHECKSTART

	if( !_XLoadCharacterBipedScripter( &g_FemaleBipedObject, "FemaleBiped.XMS" ) )
	{
		goto _charactermodelloaderrorprocess;
	}
	
	_XMEMORYUSECHECKEND( "Load female biped" );

	_XMEMORYUSECHECKSTART

	if( !_XLoadModelScripter( _T("CharactorModel.XMS") ) )
	{
		goto _charactermodelloaderrorprocess;		
	}

	_XMEMORYUSECHECKEND( "Load character model data" );

	// 비무 홍보용 Flag
	for(i = 0 ; i < _XDEF_MODELCLASSCOUNT_FLAG ; i++)
	{
		SAFE_DELETE(g_FLAGMODEL[i]);
		g_FLAGMODEL[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	}
	
	g_FLAGMODEL[0]->Load(g_ModelArchive.GetPackedFile(_T("Mz_a_x_bimooa.x3d")), &g_CharacterTextureArchive, TRUE, _T("Mz_a_x_bimooa.x3d"));
	g_FLAGMODEL[1]->Load(g_ModelArchive.GetPackedFile(_T("Mz_a_x_bimoob.x3d")), &g_CharacterTextureArchive, TRUE, _T("Mz_a_x_bimoob.x3d"));
	g_FLAGMODEL[2]->Load(g_ModelArchive.GetPackedFile(_T("Mz_a_x_bimooc.x3d")), &g_CharacterTextureArchive, TRUE, _T("Mz_a_x_bimooc.x3d"));
	g_FLAGMODEL[3]->Load(g_ModelArchive.GetPackedFile(_T("Wz_a_x_bimooa.x3d")), &g_CharacterTextureArchive, TRUE, _T("Wz_a_x_bimooa.x3d"));
	g_FLAGMODEL[4]->Load(g_ModelArchive.GetPackedFile(_T("Wz_a_x_bimoob.x3d")), &g_CharacterTextureArchive, TRUE, _T("Wz_a_x_bimoob.x3d"));
	g_FLAGMODEL[5]->Load(g_ModelArchive.GetPackedFile(_T("Wz_a_x_bimooc.x3d")), &g_CharacterTextureArchive, TRUE, _T("Wz_a_x_bimooc.x3d"));

	g_Model_GroupSeparateIndex[_XGROUP_NANGIN]			= 0;
	g_Model_GroupSeparateIndex[_XGROUP_GAEBANG]			= 50;
	g_Model_GroupSeparateIndex[_XGROUP_BEEGOONG]		= 100;
	g_Model_GroupSeparateIndex[_XGROUP_SORIM]			= 150;
	g_Model_GroupSeparateIndex[_XGROUP_NOCKRIM]		    = 200;
	g_Model_GroupSeparateIndex[_XGROUP_MOODANG]			= 250;
	g_Model_GroupSeparateIndex[_XGROUP_MAKYO]			= 300;
	g_Model_GroupSeparateIndex[_XGROUP_SEGA]			= 350;
	g_Model_GroupSeparateIndex[_XGROUP_NOFIXED]			= 400;	

	switch( g_XGQProperty.characterqualitylevel ) 
	{
	default:
	case _XGQLEVEL_MAX :
		g_TextureQuality		= 1;
		break;
	case _XGQLEVEL_HIGH:
		g_TextureQuality		= 2;
		break;
	case _XGQLEVEL_MIDDLE :
		g_TextureQuality		= 4;
		break;
	case _XGQLEVEL_LOW :
		g_TextureQuality		= 8;
		break;	
	} 
	
	_XMEMORYUSECHECKSTART
	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );

#ifdef _XTESTSERVER			
	g_CharacterTextureArchive.ReadyRenderStyle(_T("tsCharacter.xrs"));	
#else
	g_CharacterTextureArchive.ReadyRenderStyle(_T("Character.xrs"));
#endif

#ifdef _XDEF_MODELTHREADLOADING
	
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_CHARACTER.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_CHARACTER.XP") );
#endif

	if( !g_CharacterTextureArchive.LoadTexturePackage( packagefilename, D3DX_DEFAULT ) )//g_TextureMipLevel_CharacterModel ) )
	{
		goto _charactermodelloaderrorprocess;
	}

#else
	
	if( !g_CharacterTextureArchive.BuildTextureList() )
	{
		goto _charactermodelloaderrorprocess;		
	}

#endif

#ifdef _XTESTSERVER			
	g_CharacterTextureArchive.AttachRenderStyle( _T("tsCharacter.xrs") );
#else
	g_CharacterTextureArchive.AttachRenderStyle( _T("Character.xrs") );
#endif
	
	
#ifdef _XDEF_MODELTHREADLOADING

#else

	if( !g_CharacterTextureArchive.InitDeviceObject() )
	{
		goto _charactermodelloaderrorprocess;		
	}
#endif

	g_LimitTextureWidth		= g_MaxTextureWidth;
	g_LimitTextureHeight	= g_MaxTextureHeight;
		
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();
	
	_XMEMORYUSECHECKEND( "Ready character texture archive" );

	//////////////////////////////////////////////////////////////////////////////////
	// Create user character common polygon for intersect check 
	_XUser::CreateIntersectionPoly();
		
	//////////////////////////////////////////////////////////////////////////////////
	// Set user character common obb info...	
	{
		D3DXMatrixIdentity( &g_BaseOBBAxisMatrix );
		g_BaseOBBAxisMatrix._11 = 1.0f;
		g_BaseOBBAxisMatrix._21 = 0.0f;
		g_BaseOBBAxisMatrix._31 = 0.0f;
		g_BaseOBBAxisMatrix._12 = 0.0f;
		g_BaseOBBAxisMatrix._22 = 1.0f;
		g_BaseOBBAxisMatrix._32 = 0.0f;
		g_BaseOBBAxisMatrix._13 = 0.0f;
		g_BaseOBBAxisMatrix._23 = 0.0f;
		g_BaseOBBAxisMatrix._33 = 1.0f;	

		g_CameraOBB.SetCenter( D3DXVECTOR3( 0.06f, 0.06f, 0.06f ) );
		g_CameraOBB.SetExtents( D3DXVECTOR3(0.12f, 0.12f, 0.12f ) );

#ifdef _XDWDEBUG
		g_CameraOBB.CreateOBBModel();
#endif
	}
	
	return TRUE;

_charactermodelloaderrorprocess :

	g_LimitTextureWidth		= g_MaxTextureWidth;
	g_LimitTextureHeight	= g_MaxTextureHeight;
	
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();
	return FALSE;

}

BOOL XProc_FirstLoad::LoadMonsterModel( void )
{
	FILE* fileptr = NULL;
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH );
	
	if( !g_ModelArchive.ReOpenPackage() ) 
		return FALSE;

	if( !g_AniArchive.ReOpenPackage() ) 
		return FALSE;

	_XMEMORYUSECHECKREADY
	_XMEMORYUSECHECKSTART
	
	if( !_XLoadMOBModelScripter( _T("MobModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	

	/*
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{
		if( !_XLoadMOBModelScripter( _T("MobModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_CHINA )
	{
		if( !_XLoadMOBModelScripter( _T("MobModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	
	}
	else 
	{
		if( !_XLoadMOBModelScripter( _T("MobModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	
	}
	*/

	if( !_XLoadNPCModelScripter( _T("NPCModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	

	/*
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{
		if( !_XLoadNPCModelScripter( _T("NPCModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_CHINA )
	{
		if( !_XLoadNPCModelScripter( _T("NPCModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	
	}
	else
	{
		if( !_XLoadNPCModelScripter( _T("NPCModel.XMS") ) ){ 	goto _mobmodelloaderrorprocess;	}	
	}
	*/

	switch( g_XGQProperty.monsterqualitylevel ) 
	{
	default:
	case _XGQLEVEL_MAX :
		g_TextureQuality		= 1;
		break;
	case _XGQLEVEL_HIGH:
		g_TextureQuality		= 2;
		break;
	case _XGQLEVEL_MIDDLE :
		g_TextureQuality		= 4;
		break;
	case _XGQLEVEL_LOW :
		g_TextureQuality		= 8;
		break;	
	} 	

	_XMEMORYUSECHECKEND( "Loaded monster model descriptor" );
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );

#ifdef _XTESTSERVER	
	g_MobTextureArchive.ReadyRenderStyle( _T("tsMonster.xrs") );	
#else
	g_MobTextureArchive.ReadyRenderStyle( _T("Monster.xrs") );
#endif

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );

#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_MONSTER.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_MONSTER.XP") );
#endif

	if( !g_MobTextureArchive.LoadTexturePackage( packagefilename, g_TextureMipLevel_CharacterModel ) )
	{
		goto _mobmodelloaderrorprocess;
	}
	
#ifdef _XTESTSERVER	
	g_MobTextureArchive.AttachRenderStyle( _T("tsMonster.xrs") );
#else
	g_MobTextureArchive.AttachRenderStyle( _T("Monster.xrs") );
#endif		

	g_TextureQuality		= 1;
	
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();

	if(!LoadMonsterTable())
		return FALSE;
	
	return TRUE;

	// load monster table - from server
/*	_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
	fileptr = fopen("mob_table_client.xbmo", "rb");
	if(fileptr)
	{
		_XMEMORYUSECHECKSTART

		// header
		TCHAR		fileheadername[_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1];
		short		version[2];
		SYSTEMTIME	updatetime;
		
		memset(fileheadername, 0, sizeof(TCHAR)*(_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1));
		if(fread(fileheadername, sizeof(TCHAR), _XDEF_SKILLTABLE_HEADERNAMELENGTH, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		if(fread(&version[0], sizeof(short), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		if(fread(&version[1], sizeof(short), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		if(fread(&updatetime, sizeof(SYSTEMTIME), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		
		unsigned short	id_type;
		short			level;
		char			clan;
		char			type;
		char			mission;
		unsigned char	length;
		char			name[256];
		short			size;
		// body
		for(int i = 0 ; ; i++)
		{
			if(fread(&id_type, sizeof(unsigned short), 1, fileptr) < 1)
			{
				fclose(fileptr);
				return TRUE;
			}
			
			if(id_type == 0xffff)
				break;
			
			if(fread(&level, sizeof(short), 1, fileptr) < 1)
			{
				fclose(fileptr);
				return TRUE;
			}
			if(fread(&clan, sizeof(char), 1, fileptr) < 1)
			{
				fclose(fileptr);
				return TRUE;
			}
			if(fread(&type, sizeof(char), 1, fileptr) < 1)
			{
				fclose(fileptr);
				return TRUE;
			}
			if(fread(&mission, sizeof(char), 1, fileptr) < 1)
			{
				fclose(fileptr);
				return TRUE;
			}
			if(fread(&length, sizeof(unsigned char), 1, fileptr) < 1)
			{
				fclose(fileptr);
				return TRUE;
			}
			if(length > 0)
			{
				if(fread(name, sizeof(char)*length, 1, fileptr) < 1)
				{
					fclose(fileptr);
					return TRUE;
				}
			}
			if(fread(&size, sizeof(short), 1, fileptr) < 1)
			{
				fclose(fileptr);
				return TRUE;
			}
			
			g_MobAttribTable[id_type-4000].level = level;
		}
		
		fclose(fileptr);

		_XMEMORYUSECHECKEND( "Loaded monster attrib" );

		return TRUE;
	}*/

_mobmodelloaderrorprocess :

	g_TextureQuality		= 1;
	
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();

	return FALSE;
}

BOOL XProc_FirstLoad::LoadDummyObjectModel(void)
{
	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_ENVIRONMENT.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_ENVIRONMENT.XP") );
#endif

	if( !g_DummyObjectTextureArchive.SetTexturePackage( packagefilename ) ) 
		return FALSE;

	// 개방 더미
	SAFE_DELETE(g_DUMMYMODEL[0]);
	g_DUMMYMODEL[0] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_DUMMYMODEL[0]->Load(g_ModelArchive.GetPackedFile(_T("obj_soor_gb.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_soor_gb.x3d"));
	g_DUMMYBIPEDMODEL[0].Load(g_ModelArchive.GetPackedFile(_T("obj_soor_gb_B.x3d")), _T("obj_soor_gb_B.x3d"));
	
	g_DUMMYBIPEDMODEL[0].ReserveAnimationStack(1);
	if(!g_DUMMYBIPEDMODEL[0].LoadKey(g_AniArchive.GetPackedFile(_T("obj_soor_gb.xa")), _T("obj_soor_gb.xa")))
	{
		return FALSE;
	}
	
	// 비궁 더미
	SAFE_DELETE(g_DUMMYMODEL[1]);
	g_DUMMYMODEL[1] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_DUMMYMODEL[1]->Load(g_ModelArchive.GetPackedFile(_T("obj_soor_bg.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_soor_bg.x3d"));
	g_DUMMYBIPEDMODEL[1].Load(g_ModelArchive.GetPackedFile(_T("obj_soor_bg_B.x3d")), _T("obj_soor_bg_B.x3d"));
	
	g_DUMMYBIPEDMODEL[1].ReserveAnimationStack(1);
	if(!g_DUMMYBIPEDMODEL[1].LoadKey(g_AniArchive.GetPackedFile(_T("obj_soor_bg.xa")), _T("obj_soor_bg.xa")))
	{
		return FALSE;
	}

	// 소림 더미
	SAFE_DELETE(g_DUMMYMODEL[2]);
	g_DUMMYMODEL[2] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_DUMMYMODEL[2]->Load(g_ModelArchive.GetPackedFile(_T("obj_sooryun_d_01.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_sooryun_d_01.x3d"));
	g_DUMMYBIPEDMODEL[2].Load(g_ModelArchive.GetPackedFile(_T("obj_sooryun_d_01_B.x3d")), _T("obj_sooryun_d_01_B.x3d"));

	g_DUMMYBIPEDMODEL[2].ReserveAnimationStack(1);
	if(!g_DUMMYBIPEDMODEL[2].LoadKey(g_AniArchive.GetPackedFile(_T("obj_sooryun_d_01.xa")), _T("obj_sooryun_d_01.xa")))
	{
		return FALSE;
	}
	
	// 녹림 더미
	SAFE_DELETE(g_DUMMYMODEL[3]);
	g_DUMMYMODEL[3] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_DUMMYMODEL[3]->Load(g_ModelArchive.GetPackedFile(_T("obj_soor_lr.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_soor_lr.x3d"));
	g_DUMMYBIPEDMODEL[3].Load(g_ModelArchive.GetPackedFile(_T("obj_soor_lr_B.x3d")), _T("obj_soor_lr_B.x3d"));
	
	g_DUMMYBIPEDMODEL[3].ReserveAnimationStack(1);
	if(!g_DUMMYBIPEDMODEL[3].LoadKey(g_AniArchive.GetPackedFile(_T("obj_soor_lr.xa")), _T("obj_soor_lr.xa")))
	{
		return FALSE;
	}
	
	// 무당 더미
	SAFE_DELETE(g_DUMMYMODEL[4]);
	g_DUMMYMODEL[4] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_DUMMYMODEL[4]->Load(g_ModelArchive.GetPackedFile(_T("obj_soor_gb.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_soor_gb.x3d"));
	g_DUMMYBIPEDMODEL[4].Load(g_ModelArchive.GetPackedFile(_T("obj_soor_gb_B.x3d")), _T("obj_soor_gb_B.x3d"));
	
	g_DUMMYBIPEDMODEL[4].ReserveAnimationStack(1);
	if(!g_DUMMYBIPEDMODEL[4].LoadKey(g_AniArchive.GetPackedFile(_T("obj_soor_gb.xa")), _T("obj_soor_gb.xa")))
	{
		return FALSE;
	}

	// 마교 더미
	SAFE_DELETE(g_DUMMYMODEL[5]);
	g_DUMMYMODEL[5] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_DUMMYMODEL[5]->Load(g_ModelArchive.GetPackedFile(_T("obj_soor_bg.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_soor_bg.x3d"));
	g_DUMMYBIPEDMODEL[5].Load(g_ModelArchive.GetPackedFile(_T("obj_soor_bg_B.x3d")), _T("obj_soor_bg_B.x3d"));
	
	g_DUMMYBIPEDMODEL[5].ReserveAnimationStack(1);
	if(!g_DUMMYBIPEDMODEL[5].LoadKey(g_AniArchive.GetPackedFile(_T("obj_soor_bg.xa")), _T("obj_soor_bg.xa")))
	{
		return FALSE;
	}

	// 개방 기공 수련용 방석
	SAFE_DELETE(g_SEATMODEL[0]);
	g_SEATMODEL[0] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_SEATMODEL[0]->Load(g_ModelArchive.GetPackedFile(_T("obj_gigong.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_gigong.x3d"));

	// 비궁 기공 수련용 방석
	SAFE_DELETE(g_SEATMODEL[1]);
	g_SEATMODEL[1] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_SEATMODEL[1]->Load(g_ModelArchive.GetPackedFile(_T("obj_gigong_bi.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_gigong_bi.x3d"));

	// 소림 기공 수련용 방석
	SAFE_DELETE(g_SEATMODEL[2]);
	g_SEATMODEL[2] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_SEATMODEL[2]->Load(g_ModelArchive.GetPackedFile(_T("obj_gigong.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_gigong.x3d"));

	// 녹림 기공 수련용 방석
	SAFE_DELETE(g_SEATMODEL[3]);
	g_SEATMODEL[3] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_SEATMODEL[3]->Load(g_ModelArchive.GetPackedFile(_T("obj_gigong.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_gigong.x3d"));

	// 무당 기공 수련용 방석
	SAFE_DELETE(g_SEATMODEL[4]);
	g_SEATMODEL[4] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_SEATMODEL[4]->Load(g_ModelArchive.GetPackedFile(_T("obj_gigong.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_gigong.x3d"));

	// 마교 기공 수련용 방석
	SAFE_DELETE(g_SEATMODEL[5]);
	g_SEATMODEL[5] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	g_SEATMODEL[5]->Load(g_ModelArchive.GetPackedFile(_T("obj_gigong.x3d")), &g_DummyObjectTextureArchive, TRUE, _T("obj_gigong.x3d"));
	
	switch( g_XGQProperty.worldobjectqualitylevel ) 
	{
	default:
	case _XGQLEVEL_MAX :
		g_TextureQuality		= 1;
		break;
	case _XGQLEVEL_HIGH:
		g_TextureQuality		= 2;
		break;
	case _XGQLEVEL_MIDDLE :
		g_TextureQuality		= 4;
		break;
	case _XGQLEVEL_LOW :
		g_TextureQuality		= 8;
		break;	
	} 
		
#ifdef _XTESTSERVER
	if( !g_DummyObjectTextureArchive.ReadyRenderStyle( _T("tsObject.xrs") ) ) return FALSE;
	if( !g_DummyObjectTextureArchive.BuildTextureList() ) return FALSE;
	if( !g_DummyObjectTextureArchive.AttachRenderStyle( _T("tsObject.xrs") ) ) return FALSE;	
#else
	if( !g_DummyObjectTextureArchive.ReadyRenderStyle( _T("Object.xrs") ) ) return FALSE;
	if( !g_DummyObjectTextureArchive.BuildTextureList() ) return FALSE;
	if( !g_DummyObjectTextureArchive.AttachRenderStyle( _T("Object.xrs") ) ) return FALSE;	
#endif
			
	g_TextureQuality		= 1;
	
	return TRUE;
}

BOOL XProc_FirstLoad::LoadObjectModel( void )
{
	_XSetDefaultPath( _XDEF_DEFAULTPATH );	

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_ENVIRONMENT.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_ENVIRONMENT.XP") );
#endif

	if( !g_RTimeObjectTextureArchive.SetTexturePackage( packagefilename ) ) return FALSE;
	
	if( !g_ModelArchive.ReOpenPackage() ) 
		return FALSE;

	if( !g_AniArchive.ReOpenPackage() ) 
		return FALSE;
	
	if(!LoadDummyObjectModel())
	{
		_XFatalError("Can't load dummy model");
		return false;
	}
			
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();

	return TRUE;
}

BOOL XProc_FirstLoad::LoadEffectScript( void )
{
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

#ifdef _XTESTSERVER		
	g_CharacterEffectGroupManager.Load( "tsVisualEffect.xes" );
#else
	g_CharacterEffectGroupManager.Load( "VisualEffect.xes" );
#endif
	
	// Damage value 관련 Effect의 최대 사이즈를 제한한다.
	D3DXVECTOR3 size = D3DXVECTOR3(5.0f,5.0f,5.0f);
	_XParticleGroupItem* pEffect = g_CharacterEffectGroupManager.FindTemplate( _XDEF_INTERFACEID_damage );
	if( pEffect ) pEffect->SetParticleMaxSize( size );
	pEffect = g_CharacterEffectGroupManager.FindTemplate( _XDEF_INTERFACEID_cdamage );
	if( pEffect ) pEffect->SetParticleMaxSize( size );

	_XRestorePrevPath();

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

#ifdef _XTESTSERVER		
	g_WorldEffectGroupManager.Load( _T("tsEnvEffect.xes") );
#else
	g_WorldEffectGroupManager.Load( _T("EnvEffect.xes") );
#endif

	_XRestorePrevPath();
	
	g_WorldEffectInstanceManager.SetTemplateManager( &g_WorldEffectGroupManager );
	
	return TRUE;
}


BOOL XProc_FirstLoad::CreateMainInterface( void )
{
	//XML 파일 로드 //Author : 양희왕 //breif: 08/01/28
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	int resid = g_ScriptArchive.FindResource(_T("GameInfo.xml"));	
	FILE* pFile = g_ScriptArchive.GetPackedFile(_T("GameInfo.xml"));

	XMLPaser XmlPaser;

	if( resid >= 0 && pFile )
	{
		int filesize = g_ScriptArchive.GetPackedFileSize(resid) - 1;
		
		LPTSTR pBuffer = NULL;
		pBuffer = new TCHAR[ filesize+1 ];

		if( pBuffer )
		{
			memset( pBuffer, 0, sizeof(TCHAR) * (filesize + 1) );
			fread(  pBuffer, filesize, 1, pFile );
			
			if( E_FAIL == XmlPaser.ND_hrOpenMemory(pBuffer) )
			{
				_XFatalError( "Can't read GameInfo.xml file" );
				
				delete[] pBuffer;
				g_ScriptArchive.ClosePackage();

				return FALSE;
			}		
			delete[] pBuffer;
		}
		else
		{
			g_ScriptArchive.ClosePackage();
			
			_XFatalError( "Can't allocate xml buffer" );			
			return FALSE;
		}
	}
	else
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	g_ScriptArchive.ClosePackage();
	//XML 파일 로드 끝

#ifdef _XDEF_SETITEM_080118_MAGVIPER 	//Author : 양희왕 //breif : 세트아이템 정보 불러오기
	g_pSetIteminfo = new XSetItemInfomation();
	g_pSetIteminfo->LoadXMLTable( XmlPaser, _T("Set_Item"));
#endif

	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	// Initialize interface object========================================================
	g_MessageBox.Create();

	_XTOOLTIP_STRUCTURE tooltipstructure = {
		{ 200, 200 }, { 32, 32 }, { 2, 2 }, 
			-1,
			&g_MainInterfaceTextureArchive
	};
	g_InterfaceTooltip.Create( tooltipstructure );
	g_WorldObjectTooltip.Create( tooltipstructure );
	g_FameTooltip.Create( tooltipstructure );
	
	_XTOOLTIP_STRUCTURE operatortooltipstructure = 
	{
		{ 200, 200 }, { 146, 18 }, { 73, 4 },
		g_MainInterfaceTextureArchive.FindResource( "MI_GMWindow.tga" ),
		&g_MainInterfaceTextureArchive
	};	
	g_OperaterTooltip.Create( operatortooltipstructure );
	g_OperaterTooltip.SetClipRect( 0, 81, 146, 99 );
	g_OperaterTooltip.SetTextBold(TRUE);

	_XTOOLTIP_STRUCTURE itemtooltipstructure = {
		{ 0, 0 }, { 32, 32 }, { 8, 10 }, 
			g_MainInterfaceTextureArchive.FindResource( "ItemTT_middle.tga" ),
			&g_MainInterfaceTextureArchive
	};
	g_ItemTooltip.Create( itemtooltipstructure );

	// Create IME list window=======================================================
	_XWINDOW_STRUCTURE imewindowstruct = 
	{
		TRUE, { 200, 200 }, { 128, 168 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 220, 64, 64, 64 ),
			_XDEF_WTITLE_IMEWINDOW
	};
	g_IMEWindow.Create(imewindowstruct);
	g_IMEWindow.Initialize();
	g_IMEWindow.ShowWindow(FALSE);
	g_IMEWindow.EnableWindowEffect(FALSE);
	g_IMEWindow.SetDocking(FALSE);


	// Create main menu window======================================================
	_XWindow_MainMenu* pMainMenu_Window = new _XWindow_MainMenu;
	_XWINDOW_STRUCTURE mainmenuwindowstruct = 
	{
		TRUE, { gnWidth-182, gnHeight-93 }, { 182, 93 }, 0, 
			g_MainInterfaceTextureArchive.FindResource( "MI_main_button.tga" ),
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 255, 255, 255, 255 ),
			_XDEF_WTITLE_MAINMENUWINDOW  
	};
	pMainMenu_Window->Create( mainmenuwindowstruct );
	pMainMenu_Window->Initialize();	
	pMainMenu_Window->SetDocking( TRUE );
	pMainMenu_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pMainMenu_Window );
	

	// Create jin status window=====================================================
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	_XWindow_JinStatus* pJin_Window = new _XWindow_JinStatus;
	_XWINDOW_STRUCTURE jinwindowstruct = 
	{
		TRUE, { 0, 0 }, { 256, 72 }, 0,
			g_MainInterfaceTextureArchive.FindResource( "mi_char_01.tga" ),
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_JINSTATUSWINDOW
	};
	pJin_Window->Create( jinwindowstruct );
	pJin_Window->Initialize();	
	pJin_Window->SetDocking( TRUE );
	pJin_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pJin_Window );
#else
	_XWindow_JinStatus* pJin_Window = new _XWindow_JinStatus;
	_XWINDOW_STRUCTURE jinwindowstruct = 
	{
		TRUE, { 0, 0 }, { 229, 82 }, 0,
			g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_JINSTATUSWINDOW
	};
	pJin_Window->Create( jinwindowstruct );
	pJin_Window->Initialize();	
	pJin_Window->SetDocking( TRUE );
	pJin_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pJin_Window );
#endif

	// Create monster status window=================================================
	_XWindow_MonsterStatus* pMonsterStatus_Window = new _XWindow_MonsterStatus;
	_XWINDOW_STRUCTURE monsterstatuswindowstruct = 
	{
		TRUE, { (gnWidth>>1)-90, 22 }, { 180, 49 }, 0,
			g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 255, 255, 255, 255 ),
			_XDEF_WTITLE_MONSTERSTATUSWINDOW
	};
	pMonsterStatus_Window->Create( monsterstatuswindowstruct );
	pMonsterStatus_Window->Initialize();	
	pMonsterStatus_Window->SetDocking( TRUE );
	g_MainWindowManager.InsertWindow( pMonsterStatus_Window );	

	// Create monster status window=================================================
	_XWindow_TargetInfo* pTargetInfo_Window = new _XWindow_TargetInfo;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		_XWINDOW_STRUCTURE targetinfowindowstruct = 
		{
#ifdef _ACCLAIM_RUBICONADSYSTEM	
			TRUE, { (gnWidth>>1)-90, 75 + g_RubiconADWindowSize.cy }, { 180, 344 }, 0,
#else
			TRUE, { (gnWidth>>1)-90, 75 }, { 180, 344 }, 0,
#endif
				-1,
				&g_MainInterfaceTextureArchive,
				D3DCOLOR_ARGB( 0, 0, 0, 0 ),
				_XDEF_WTITLE_TARGETINFOWINDOW
		};
		pTargetInfo_Window->Create( targetinfowindowstruct );

	}
	else
	{
		_XWINDOW_STRUCTURE targetinfowindowstruct = 
		{
			TRUE, { (gnWidth>>1)-90, 75 }, { 180, 244 }, 0,
				-1,
				&g_MainInterfaceTextureArchive,
				D3DCOLOR_ARGB( 0, 0, 0, 0 ),
				_XDEF_WTITLE_TARGETINFOWINDOW
		};
		pTargetInfo_Window->Create( targetinfowindowstruct );
	}
	pTargetInfo_Window->Initialize();	
	pTargetInfo_Window->SetDocking( TRUE );
	pTargetInfo_Window->ShowWindow(FALSE);
	pTargetInfo_Window->SetEscapeWindow(TRUE);
	g_MainWindowManager.InsertWindow( pTargetInfo_Window );

	// Create minimap window========================================================
	g_pMinimap_Window = new _XWindow_WorldMinimap;
	_XWINDOW_STRUCTURE MMwindowstruct = 
	{
		TRUE, { gnWidth-154, 0 }, { 154, 210 }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 255, 255, 255, 255 ),
			_XDEF_WTITLE_MINIMAPWINDOW
	};
	g_pMinimap_Window->Create( MMwindowstruct );
	g_pMinimap_Window->Initialize();	
	g_pMinimap_Window->SetDocking( TRUE );
	g_pMinimap_Window->EnableWindowEffect( !TRUE );	
	g_pMinimap_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( g_pMinimap_Window );

	// Create maximum minimap window================================================
	_XWindow_MaximumMap * pMaxM_Window = new _XWindow_MaximumMap;
	_XWINDOW_STRUCTURE MaxMinimapwindowstruct = 
	{
		TRUE, { (gnWidth>>1)+232, (gnHeight>>1)-230 }, { 135, 362 }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 255, 255, 255, 255 ),
			_XDEF_WTITLE_MAXMINIMAPWINDOW
	};
	pMaxM_Window->Create( MaxMinimapwindowstruct );
	pMaxM_Window->Initialize();		
	pMaxM_Window->SetDocking( FALSE );
	pMaxM_Window->ShowWindow( FALSE );
	pMaxM_Window->EnableWindowEffect( !TRUE );
	pMaxM_Window->SetEscapeWindow(TRUE);
	pMaxM_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pMaxM_Window );

	// Create QuickSlot window======================================================
	_XWindow_QuickSlot* pQS_Window = new _XWindow_QuickSlot;
	_XWINDOW_STRUCTURE QuickWindowStruct =
	{
		TRUE, {gnWidth-183-459, gnHeight-48}, {460, 48}, 0,
			-1,
			NULL,
			D3DCOLOR_ARGB(8, 0, 0, 0),
			_XDEF_WTITLE_QUICKSLOT
	};
	pQS_Window->Create(QuickWindowStruct);
	pQS_Window->Initialize();
	pQS_Window->SetDocking(TRUE);
	pQS_Window->EnableWindowEffect( !TRUE );	
	g_MainWindowManager.InsertWindow( pQS_Window );
	
	// Create default chat window===================================================
	_XWindow_ChattingDefault* pChatDefault_Window = new _XWindow_ChattingDefault;
	_XWINDOW_STRUCTURE ChatDefaultwindowstruct = 
	{
		TRUE, { 0, gnHeight-195 }, { 374, 214 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 64, 0, 0, 0 ),
			_XDEF_WTITLE_CHATTINGWINDOWDEFAULT
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		ChatDefaultwindowstruct.position.y = gnHeight-181;
		ChatDefaultwindowstruct.windowsize.cy = 200;
	}

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	ChatDefaultwindowstruct.position.y -= 47;
#endif

	pChatDefault_Window->Create( ChatDefaultwindowstruct );
	pChatDefault_Window->Initialize();		
	pChatDefault_Window->SetDocking( FALSE );
	pChatDefault_Window->SetEdgeDocking( FALSE );
	pChatDefault_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pChatDefault_Window->SetEscapeWindow(FALSE);
	g_MainWindowManager.InsertWindow( pChatDefault_Window );
	pChatDefault_Window->ShowWindow( TRUE );

#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 아이콘
	_XWindow_EventIcon* pEventIconWindow = new _XWindow_EventIcon();

	_XWINDOW_STRUCTURE  EventIconWindow = 
	{
		TRUE,
		{ 0, 0}, 
		{ 46, 46}, 
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
		_XDEF_WTITLE_EVENT_RENEWAL_WINDOW
	};

	pEventIconWindow->Create( EventIconWindow );
	pEventIconWindow->Initialize();
	pEventIconWindow->ShowWindow( FALSE );
	pEventIconWindow->EnableWindowEffect( FALSE );
	pEventIconWindow->SetDocking( TRUE, 1 );
	pEventIconWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pEventIconWindow );	
#endif

	// Create default System Message window ========================================
	_XWindow_SystemMsgDefault* pSystemMsg_Window = new _XWindow_SystemMsgDefault;
	_XWINDOW_STRUCTURE SystemMsgwindowstruct = 
	{
		TRUE, { gnWidth - 374, gnHeight-195}, { 374, 96 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ),
			_XDEF_WTITLE_SYSTEMMSGDEFAULT
	};
	pSystemMsg_Window->Create( SystemMsgwindowstruct );
	pSystemMsg_Window->Initialize();
	pSystemMsg_Window->SetDocking( FALSE );
	pSystemMsg_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pSystemMsg_Window );
	pSystemMsg_Window->ShowWindow( TRUE );
			
	// Create character NPCskill window=================================================
	_XWindow_NPCSkill* pNPCSkill_Window = new _XWindow_NPCSkill;
	_XWINDOW_STRUCTURE npcskillwindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 356, 266 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
			_XDEF_WTITLE_NPCSKILL
	};
	pNPCSkill_Window->Create( npcskillwindowstruct );
	pNPCSkill_Window->Initialize();
	pNPCSkill_Window->ShowWindow( !TRUE );
	pNPCSkill_Window->EnableWindowEffect( !TRUE );
	pNPCSkill_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pNPCSkill_Window );
	
	// Create FreeMatchInfo window =====================================================
	_XWindow_FreeMatchInfo* pFMInfo_Window = new _XWindow_FreeMatchInfo;
	_XWINDOW_STRUCTURE fmwindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 356, 266 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
			_XDEF_WTITLE_FREEMATCHINFO
	};
	pFMInfo_Window->Create( fmwindowstruct );
	pFMInfo_Window->Initialize();
	pFMInfo_Window->ShowWindow( !TRUE );
	pFMInfo_Window->EnableWindowEffect( !TRUE );
	pFMInfo_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pFMInfo_Window );
	
	// Create CastleInfo window =====================================================
	_XWindow_CastleInfo* pCastleInfo_Window = new _XWindow_CastleInfo;
	_XWINDOW_STRUCTURE castleinfowindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 356, 266 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
			_XDEF_WTITLE_CASTLEINFO
	};
	pCastleInfo_Window->Create( castleinfowindowstruct );
	pCastleInfo_Window->Initialize();
	pCastleInfo_Window->ShowWindow( !TRUE );
	pCastleInfo_Window->EnableWindowEffect( !TRUE );
	pCastleInfo_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pCastleInfo_Window );

	// Create ChallengerList window ==================================================
	_XWindow_ChallengerList* pChallengerList_Window = new _XWindow_ChallengerList;
	_XWINDOW_STRUCTURE challengerlistwindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 356, 266 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
			_XDEF_WTITLE_CHALLENGERLIST
	};
	pChallengerList_Window->Create( challengerlistwindowstruct );
	pChallengerList_Window->Initialize();
	pChallengerList_Window->ShowWindow( !TRUE );
	pChallengerList_Window->EnableWindowEffect( !TRUE );
	pChallengerList_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pChallengerList_Window );

	// Create CBResultNotice window ==================================================
	_XWindow_CBResultNotice* pCBResultNotice_Window = new _XWindow_CBResultNotice;
	_XWINDOW_STRUCTURE cbresultnoticestructure = 
	{
		TRUE, { (gnWidth>>1)-293, (gnHeight>>1)-257 }, { 586, 257 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
			_XDEF_WTITLE_CBRESULTNOTICE
	};
	pCBResultNotice_Window->Create( cbresultnoticestructure );
	pCBResultNotice_Window->Initialize();
	pCBResultNotice_Window->ShowWindow( !TRUE );
	pCBResultNotice_Window->EnableWindowEffect( !TRUE );
	pCBResultNotice_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pCBResultNotice_Window );
	
	// Create main frame window======================================================
	g_pMainFrame_Window = new _XWindow_MainFrame;

	FLOAT yfactor = (FLOAT)gnHeight / 768.0f;
	int halfsize = 442>>1;

	_XWINDOW_STRUCTURE mainframewindowstruct = 
	{
		TRUE, { 36, (int)((114.0f+halfsize)*yfactor) - halfsize }, { 352, 442 }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 64, 0, 0, 0 ),
			_XDEF_WTITLE_MAINFRAME  
	};
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		mainframewindowstruct.windowsize.cy = 453;
	}

	g_pMainFrame_Window->Create( mainframewindowstruct );
	g_pMainFrame_Window->Initialize();	
	g_pMainFrame_Window->SetDocking( TRUE, 1 );
	g_pMainFrame_Window->SetDrawBorder(FALSE);
	g_pMainFrame_Window->ShowWindow( FALSE );
	g_pMainFrame_Window->SetEscapeWindow( TRUE );
	g_pMainFrame_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( g_pMainFrame_Window );
	
	// Create messengerwindow=======================================================
	_XWindow_Messenger* pMessenger_Window = new _XWindow_Messenger;
	_XWINDOW_STRUCTURE messengerwindowstruct = 
	{
		TRUE, 
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 친구소환
		{ gnWidth - (1024-841), gnHeight - 355 }, 
		{ 183, 283 }, 
#else
		{ gnWidth - (1024-841), gnHeight - 329 }, 
		{ 183, 281 }, 
#endif
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 200, 0, 0, 0 ),
		_XDEF_WTITLE_MESSENGERWINDOW
	};
	pMessenger_Window->Create( messengerwindowstruct );
	pMessenger_Window->Initialize();	
	pMessenger_Window->SetDocking( TRUE, 1 );
	pMessenger_Window->EnableWindowEffect( !TRUE );	
	pMessenger_Window->SetEscapeWindow(TRUE);
	pMessenger_Window->ShowWindow( FALSE );
	pMessenger_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pMessenger_Window );	
	pSystemMsg_Window->SetMessengerWindowPtr( pMessenger_Window );	// 2005.02.25->hotblood

	// Create emailwindow=======================================================
	_XWindow_Email* pEmail_Window = new _XWindow_Email;
	_XWINDOW_STRUCTURE emailwindowstruct = 
	{
		TRUE, { gnWidth - (1024-841), gnHeight - 336 }, { 183, 288 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 200, 0, 0, 0 ),
			_XDEF_WTITLE_EMAILWINDOW
	};
	pEmail_Window->Create( emailwindowstruct );
	pEmail_Window->Initialize();	
	pEmail_Window->SetDocking( TRUE );
	pEmail_Window->EnableWindowEffect( !TRUE );	
	pEmail_Window->SetEscapeWindow(TRUE);
	pEmail_Window->ShowWindow( FALSE );
	pEmail_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pEmail_Window );
	pSystemMsg_Window->SetEmailWindowPtr( pEmail_Window );	// 2005.02.25->hotblood
	
	// Create inventory window==========================================================
	_XWindow_Inventory* pInventory_Window = new _XWindow_Inventory;
	_XWINDOW_STRUCTURE inventorywindowstruct =     
	{
		TRUE, { (gnWidth>>1), (gnHeight>>1)-205 }, { 183, 270 }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_INVENTORY
	};
	pInventory_Window->Create( inventorywindowstruct );
	pInventory_Window->Initialize();
	pInventory_Window->ShowWindow( FALSE );	
	pInventory_Window->EnableWindowEffect( !TRUE );
	pInventory_Window->SetDocking( TRUE, 1 );	
	pInventory_Window->SetEscapeWindow(TRUE);//2004.06.24->oneway48 insert
	pInventory_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pInventory_Window );	

	// Create Party window==============================================================
	_XWindow_Party* pParty_Window = new _XWindow_Party;
	_XWINDOW_STRUCTURE partywindowstruct = 
	{
		TRUE, { gnWidth-204, (gnHeight>>1)-205 }, { 205, 327 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_PARTY
	};
	pParty_Window->Create( partywindowstruct );
	pParty_Window->Initialize();
	pParty_Window->ShowWindow( FALSE );
	pParty_Window->EnableWindowEffect( !TRUE );
	pParty_Window->SetDocking( TRUE, 1 );
	pParty_Window->SetEscapeWindow(TRUE);//2004.06.24->oneway48 insert
	g_MainWindowManager.InsertWindow( pParty_Window );		

	// Create mini party info bar  ======================================================
	_XWINDOW_STRUCTURE partymasterminiwindowstruct = 
	{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
		#ifdef _ACCLAIM_RUBICONADSYSTEM	
		TRUE, { 50, 114+g_RubiconADWindowSize.cy  }, { 165, 56 }, 0, 
		#else
		TRUE, { 50, 114  }, { 165, 56 }, 0, 
		#endif
#else		
		#ifdef _ACCLAIM_RUBICONADSYSTEM	
		TRUE, { 14, 114+g_RubiconADWindowSize.cy  }, { 165, 56 }, 0, 
		#else
		TRUE, { 14, 114  }, { 165, 56 }, 0, 
		#endif
#endif
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ),
			_XDEF_WTITLE_MINIPARTY_MEMBER1
	};
	_XWindow_PartyMasterMiniBar* pPartyMasterMinibar_Window = new _XWindow_PartyMasterMiniBar;
	pPartyMasterMinibar_Window->m_CloseButtoncommandid = _XDEF_MINIPARTYWINDOW_CLOSEBUTTON1;		
	pPartyMasterMinibar_Window->Create( partymasterminiwindowstruct );
	pPartyMasterMinibar_Window->SetWindowID( _XDEF_WTITLE_MINIPARTY_MEMBER1 );
	pPartyMasterMinibar_Window->Initialize();	
	pPartyMasterMinibar_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pPartyMasterMinibar_Window->SetDocking( TRUE, 2 );
	pPartyMasterMinibar_Window->ShowWindow( FALSE );
	g_MainWindowManager.InsertWindow( pPartyMasterMinibar_Window );	
	
	// Create mini party info bar  ======================================================
	_XWINDOW_STRUCTURE partyminiwindowstruct = 
	{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
		#ifdef _ACCLAIM_RUBICONADSYSTEM
		TRUE, { 50, 170+g_RubiconADWindowSize.cy }, { 146, 36 }, 0, 
		#else
		TRUE, { 50, 170 }, { 146, 36 }, 0, 
		#endif
#else
		#ifdef _ACCLAIM_RUBICONADSYSTEM			
		TRUE, { 14, 170+g_RubiconADWindowSize.cy }, { 146, 36 }, 0, 
		#else
		TRUE, { 14, 170 }, { 146, 36 }, 0, 
		#endif
#endif
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ),
			_XDEF_WTITLE_MINIPARTY_MEMBER1
	};

	for( int i = 1; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
	{				
		_XWindow_PartyMemberMiniBar* pPartyMemberMinibar_Window = new _XWindow_PartyMemberMiniBar;
		pPartyMemberMinibar_Window->m_CloseButtoncommandid = _XDEF_MINIPARTYWINDOW_CLOSEBUTTON1+i;	
		pPartyMemberMinibar_Window->m_EntrustButtoncommandid = _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON1-1+i;		
		pPartyMemberMinibar_Window->Create( partyminiwindowstruct );
		pPartyMemberMinibar_Window->SetWindowID( _XDEF_WTITLE_MINIPARTY_MEMBER1 + i );
		pPartyMemberMinibar_Window->Initialize();	
		pPartyMemberMinibar_Window->SetWindowMoveMode( _XWMOVE_FIXED );
		pPartyMemberMinibar_Window->SetDocking( TRUE, 2 );

	/*#ifdef _ACCLAIM_RUBICONADSYSTEM
		pPartyMemberMinibar_Window->MoveWindow( 50, 170 + g_RubiconADWindowSize.cy + partyminiwindowstruct.position.y);
	#else
		pPartyMemberMinibar_Window->MoveWindow( 50, 170 + partyminiwindowstruct.position.y);
	#endif*/
		pPartyMemberMinibar_Window->ShowWindow( FALSE );
		g_MainWindowManager.InsertWindow( pPartyMemberMinibar_Window );	
	}
				
	
	// Create Option menu window=========================================================
	_XWindow_OptionMenu* pOptionMenu_Window = new _XWindow_OptionMenu;
	_XWINDOW_STRUCTURE optionmenuwindowstruct = 
	{
		TRUE, { (gnWidth>>1)-(183>>1), (gnHeight>>1)-(270>>1) }, { 183, 270 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_OPTIONMENU
	};
	pOptionMenu_Window->Create( optionmenuwindowstruct );
	pOptionMenu_Window->Initialize();
	pOptionMenu_Window->ShowWindow( FALSE );
	pOptionMenu_Window->EnableWindowEffect( FALSE );
	pOptionMenu_Window->SetDocking( TRUE, 1 );	
	pOptionMenu_Window->SetEscapeWindow(TRUE);
	pOptionMenu_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pOptionMenu_Window );


	// Create Option window==============================================================	
	_XWindow_Option* pOption_Window = new _XWindow_Option;
	_XWINDOW_STRUCTURE optionwindowstruct = 
	{
		TRUE, 

#ifdef _XJAPANESE
		{ (gnWidth>>1)-(282>>1), (gnHeight>>1)-((392+32)>>1) }, { 282, 392+8 }, 
#else
		{ (gnWidth>>1)-(282>>1), (gnHeight>>1)-(392>>1) }, { 282, 392 }, 
#endif
			0,-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_OPTION
	};
	pOption_Window->Create( optionwindowstruct );
	pOption_Window->Initialize();
	pOption_Window->ShowWindow( FALSE );
	pOption_Window->EnableWindowEffect( !TRUE );
	pOption_Window->SetDocking( TRUE, 1 );	
	pOption_Window->SetEscapeWindow(TRUE);
	pOption_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pOption_Window );


	// Initialize NPC Name window========================================================
	g_NPCNameWindow.Initialize();
	

	// Create NPC Script window==========================================================
	_XWindow_NPCScript* pNPCScript_Window = new _XWindow_NPCScript;
	_XWINDOW_STRUCTURE npcscriptwindowstruct = 
	{
		TRUE, { gnWidth - (1024 - 581), (gnHeight>>1) - 157 - 82 }, { 425, 410 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ),
			_XDEF_WTITLE_NPCSCRIPT
	};
	pNPCScript_Window->Create( npcscriptwindowstruct );	
	pNPCScript_Window->Initialize();
	pNPCScript_Window->ShowWindow( !TRUE );		
	pNPCScript_Window->EnableWindowEffect( !TRUE );	
	pNPCScript_Window->SetDocking( TRUE, 1 );
	pNPCScript_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pNPCScript_Window->SetEnableTopWindow( FALSE );
	g_MainWindowManager.InsertWindow( pNPCScript_Window );
	
	// Create Warehouse window==========================================================
	_XWindow_Warehouse* pWarehouse_Window = new _XWindow_Warehouse;
	_XWINDOW_STRUCTURE warehousewindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { _XDEF_WINDOW_DEFAULTWIDTH, _XDEF_WINDOW_DEFAULTHEIGHT }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
			_XDEF_WTITLE_WAREHOUSE
	};
	pWarehouse_Window->Create( warehousewindowstruct );
	pWarehouse_Window->Initialize();
	pWarehouse_Window->ShowWindow( !TRUE );		
	pWarehouse_Window->EnableWindowEffect( FALSE );	
	pWarehouse_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pWarehouse_Window );	
	
	// Create Add SaveItem window==========================================================
	_XWindow_AddSaveItem* pAddSaveItem_Window = new _XWindow_AddSaveItem;
	_XWINDOW_STRUCTURE addsaveitemwindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { _XDEF_WINDOW_DEFAULTWIDTH, _XDEF_WINDOW_DEFAULTHEIGHT }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
			_XDEF_WTITLE_ADDSAVEITEM
	};
	pAddSaveItem_Window->Create( addsaveitemwindowstruct );
	pAddSaveItem_Window->Initialize();
	pAddSaveItem_Window->ShowWindow( FALSE );		
	pAddSaveItem_Window->EnableWindowEffect( FALSE );	
	pAddSaveItem_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pAddSaveItem_Window );

	// Create PC trade window============================================================
	_XWindow_PCTrade* pPCTrade_Window = new _XWindow_PCTrade;
	_XWINDOW_STRUCTURE pctradewindowstruct = 
	{
#ifdef _ACCLAIM_RUBICONADSYSTEM			
		TRUE, { (gnWidth>>1)-274, (gnHeight>>1)-270 + g_RubiconADWindowSize.cy }, { _XDEF_WINDOW_DEFAULTWIDTH, _XDEF_WINDOW_DEFAULTHEIGHT }, 0,
#else
		TRUE, { (gnWidth>>1)-274, (gnHeight>>1)-270 }, { _XDEF_WINDOW_DEFAULTWIDTH, _XDEF_WINDOW_DEFAULTHEIGHT }, 0,
#endif	
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 255, 0, 0, 0 ),
		_XDEF_WTITLE_PCTRADE
	};
	pPCTrade_Window->Create( pctradewindowstruct );
	pPCTrade_Window->Initialize();
	pPCTrade_Window->ShowWindow( !TRUE );
	pPCTrade_Window->EnableWindowEffect( FALSE );
	pPCTrade_Window->SetDocking( TRUE, 1 );
	pPCTrade_Window->SetEscapeWindow(TRUE);//2004.06.24->oneway48 insert
	pPCTrade_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pPCTrade_Window );	
	
	// Create Personal Store window============================================================
	_XWindow_PersonalStore* pPersonalStore_Window = new _XWindow_PersonalStore;
	_XWINDOW_STRUCTURE personalstorewindowstruct = 
	{
#ifdef _ACCLAIM_RUBICONADSYSTEM			
		TRUE, { (gnWidth>>1)-288, 74 + g_RubiconADWindowSize.cy }, { 288, _XDEF_WINDOW_DEFAULTHEIGHT }, 0,
#else
		TRUE, { (gnWidth>>1)-288, 74 }, { 288, _XDEF_WINDOW_DEFAULTHEIGHT }, 0,
#endif
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 64, 0, 0, 0 ),
		_XDEF_WTITLE_PERSONALSTORE
	};
	pPersonalStore_Window->Create( personalstorewindowstruct );
	pPersonalStore_Window->Initialize();
	pPersonalStore_Window->ShowWindow( !TRUE );
	pPersonalStore_Window->EnableWindowEffect( FALSE );
	pPersonalStore_Window->SetDocking( TRUE, 1 );
	pPersonalStore_Window->SetEscapeWindow(TRUE);
	pPersonalStore_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pPersonalStore_Window );	
	
	// Create Trade Window ====================================================
	_XWindow_NPCTrade* pNPCTrade_Window = new _XWindow_NPCTrade;
	_XWINDOW_STRUCTURE npctradewindowstruct = 
	{
		TRUE, {409, (gnHeight>>1)-205}, { _XDEF_WINDOW_DEFAULTWIDTH, _XDEF_WINDOW_DEFAULTHEIGHT }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_NPCTRADE
	};
	pNPCTrade_Window->Create(npctradewindowstruct);
	pNPCTrade_Window->Initailize();
	pNPCTrade_Window->ShowWindow(FALSE);
	pNPCTrade_Window->EnableWindowEffect(!TRUE);
	pNPCTrade_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pNPCTrade_Window);
		
	// Create Trade confirm window =============================================
	_XWindow_TradeConfirm* pTradeConfirm_Window = new _XWindow_TradeConfirm;
	_XWINDOW_STRUCTURE tradeconfirmstruct = 
	{
		TRUE, {(gnWidth/2)-102, (gnHeight/2)-44}, {208, 88}, 0, 
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_TRADECONFIRM
	};
	pTradeConfirm_Window->Create(tradeconfirmstruct);
	pTradeConfirm_Window->Initialize();
	pTradeConfirm_Window->ShowWindow(FALSE);
	pTradeConfirm_Window->EnableWindowEffect(!TRUE);
	pTradeConfirm_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pTradeConfirm_Window);
	
	// Create Request Party window ==============================================
	_XWindow_RequestParty* pRequestParty_Window = new _XWindow_RequestParty;
	
	_XWINDOW_STRUCTURE requestpartystruct = 
	{
		TRUE, {0, 0}, {96, 134}, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(130, 0, 0, 0),
			_XDEF_WTITLE_REQUESTPARTY
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		requestpartystruct.windowsize.cx = 132;		
	}
	
#ifdef _XTS_ALIANCE_20061018
	requestpartystruct.windowsize.cy = 152;	
#endif
	
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	requestpartystruct.windowsize.cy += 20;	
#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	requestpartystruct.windowsize.cy += 20;	
#endif

	pRequestParty_Window->Create(requestpartystruct);
	pRequestParty_Window->Initialize();
	pRequestParty_Window->ShowWindow(!TRUE);
	pRequestParty_Window->EnableWindowEffect(!TRUE);
	pRequestParty_Window->SetDocking(TRUE, 1);
	pRequestParty_Window->SetEscapeWindow(TRUE);//2004.06.24->oneway48 insert
	g_MainWindowManager.InsertWindow(pRequestParty_Window);

	// Create Rebirth window ==============================================
	_XWindow_Rebirth* pRebirth_Window = new _XWindow_Rebirth;
	_XWINDOW_STRUCTURE rebirthstruct = 
	{
		TRUE, {(gnWidth>>1)-106, (gnHeight>>1)+188}, {212, 144}, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_REBIRTH
	};
	pRebirth_Window->Create(rebirthstruct);
	pRebirth_Window->Initialize();
	pRebirth_Window->ShowWindow(!TRUE);
	pRebirth_Window->EnableWindowEffect(!TRUE);
	pRebirth_Window->SetDocking(TRUE, 1);

#ifdef _ACCLAIM_IGAADSYSTEM // 광고화면을 위해 화면중앙으로 위치
	//pRebirth_Window->MoveWindow( (gnWidth>>1)-106, (gnHeight>>1)-70 );
#endif

	g_MainWindowManager.InsertWindow(pRebirth_Window);

	// 2004.06.17->oneway48 insert
	// Create RepairItem window ==============================================
	_XWindow_RepairItem* pRepairItem_Window = new _XWindow_RepairItem;
	_XWINDOW_STRUCTURE repairstruct = 
	{
		TRUE, {409, (gnHeight>>1)-205}, { _XDEF_WINDOW_DEFAULTWIDTH, _XDEF_WINDOW_DEFAULTHEIGHT }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_REPAIRITEM
	};
	pRepairItem_Window->Create(repairstruct);
	pRepairItem_Window->Initialize();
	pRepairItem_Window->ShowWindow(FALSE);
	pRepairItem_Window->EnableWindowEffect(!TRUE);
	pRepairItem_Window->SetDocking(TRUE, 1);
	pRepairItem_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pRepairItem_Window->SetEnableTopWindow( FALSE );
	g_MainWindowManager.InsertWindow(pRepairItem_Window);

	// Create Zone name window ==================================================
	_XWindow_ZoneName* pZoneName_Window = new _XWindow_ZoneName;
	
#ifdef _XDEF_EXPAND_ZONENAME
	_XWINDOW_STRUCTURE zonenamewindowstruct = 
	{
		TRUE, {(gnWidth/2)-182, 96}, {364, 45}, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_ZONENAMEWINDOW
	};
#else
	_XWINDOW_STRUCTURE zonenamewindowstruct = 
	{
		TRUE, {(gnWidth/2)-80, 96}, {161, 45}, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_ZONENAMEWINDOW
	};
#endif
	pZoneName_Window->Create(zonenamewindowstruct);
	pZoneName_Window->Initialize();
	pZoneName_Window->ShowWindow(FALSE);
	pZoneName_Window->EnableWindowEffect(FALSE);
	pZoneName_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pZoneName_Window);

	// Create Death blow window ==================================================
	_XWindow_DeathBlow* pDeathBlow_Window = new _XWindow_DeathBlow;
	_XWINDOW_STRUCTURE deathblowindowstruct = 
	{
		TRUE, { (gnWidth>>1)-26,56}, {52, 17}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_DEATHBLOWWINDOW
	};
	pDeathBlow_Window->Create(deathblowindowstruct);
	pDeathBlow_Window->Initialize();	
	pDeathBlow_Window->ShowWindow(FALSE);
	pDeathBlow_Window->EnableWindowEffect(FALSE);
	pDeathBlow_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pDeathBlow_Window);
		
	// Create admin notice window================================================
	_XWindow_AdminNotice* pAdminNotice_Window = new _XWindow_AdminNotice;
	_XWINDOW_STRUCTURE adminnoticewindowstruct = 
	{
		//TRUE, { (gnWidth>>1)-157, (gnHeight>>1) - 120 }, { 270, 130 }, 0,
		TRUE, { (gnWidth>>1)-155, 100 }, { 310, 136 }, 0,
			-1, 
			NULL,
			D3DCOLOR_ARGB( 96, 0, 0, 0 ),
			_XDEF_WTITLE_NOTICEWINDOW
	};
	pAdminNotice_Window->Create( adminnoticewindowstruct );
	pAdminNotice_Window->Initialize();		
	pAdminNotice_Window->SetDocking( FALSE );
	pAdminNotice_Window->EnableWindowEffect( FALSE );
	pAdminNotice_Window->ShowWindow( FALSE );
	pAdminNotice_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pAdminNotice_Window );

	// Create Charge gauge window ================================================
	_XWindow_ChargeGauge* pChargeGauge_Window = new _XWindow_ChargeGauge;
	_XWINDOW_STRUCTURE chargegaugestruct = 
	{
		TRUE, {200, 200}, {203, 27}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_CHARGEGAUGE
	};
	pChargeGauge_Window->Create(chargegaugestruct);
	pChargeGauge_Window->Initailize();
	pChargeGauge_Window->ShowWindow(FALSE);
	pChargeGauge_Window->SetDocking(FALSE, 0);
	g_MainWindowManager.InsertWindow(pChargeGauge_Window);


	// Create MessengerTooltip window ==============================================
	_XWindow_MessengerTooltip* pMessengerTooltip_Window = new _XWindow_MessengerTooltip;
	_XWINDOW_STRUCTURE messengertooltipstruct = 
	{
		TRUE, {(gnWidth/2) - 200, (gnHeight/2) - 200}, {122, 95}, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(255, 0, 0, 0),
			_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW
	};
	pMessengerTooltip_Window->Create(messengertooltipstruct);
	pMessengerTooltip_Window->Initialize();
	pMessengerTooltip_Window->ShowWindow(!TRUE);
	pMessengerTooltip_Window->EnableWindowEffect(!TRUE);
	pMessengerTooltip_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pMessengerTooltip_Window);
	
	// Create NameInput window =============================================
	_XWindow_NameInput* pNameInput_Window = new _XWindow_NameInput;
	_XWINDOW_STRUCTURE nameinputstruct = 
	{
		TRUE, {(gnWidth/2)-102, (gnHeight/2)-161}, {286, 65}, 0, 
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_NAMEINPUTWINDOW
	};
	pNameInput_Window->Create(nameinputstruct);
	pNameInput_Window->Initialize();
	pNameInput_Window->ShowWindow(FALSE);
	pNameInput_Window->EnableWindowEffect(!TRUE);
	pNameInput_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pNameInput_Window);
	
	// Create NEW Training Meditation window ====================================
	_XWindow_TrainingMed* pTMeditation_Window = new _XWindow_TrainingMed;
	_XWINDOW_STRUCTURE tmedistruct =
	{
		TRUE, {0, 107}, {gnWidth, 300}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(204, 0, 0, 0),
		_XDEF_WTITLE_TRAININGMEDITATION
	};
	pTMeditation_Window->Create(tmedistruct);
	pTMeditation_Window->Initialize();
	pTMeditation_Window->ShowWindow(FALSE);
	pTMeditation_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pTMeditation_Window);

	// Create Training Dummy window ===========================================
	_XWindow_TrainingDummy* pTDummy_Window = new _XWindow_TrainingDummy;
	_XWINDOW_STRUCTURE tdummystruct =
	{
		TRUE, {0, 0}, {420, 118}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_TRAININGDUMMY
	};
	pTDummy_Window->Create(tdummystruct);
	pTDummy_Window->Initialize();
	pTDummy_Window->ShowWindow(FALSE);
	pTDummy_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pTDummy_Window);

	// Create Training Spell window ===========================================
	_XWindow_TrainingSpell* pTSpell_Window = new _XWindow_TrainingSpell;
	_XWINDOW_STRUCTURE tspellstruct =
	{
		TRUE, {0, 0}, {420, 118}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_TRAININGSPELL
	};
	pTSpell_Window->Create(tspellstruct);
	pTSpell_Window->Initialize();
	pTSpell_Window->ShowWindow(FALSE);
	pTSpell_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pTSpell_Window);

	// Create Training Complete window ===========================================
	_XWindow_TrainingComplete* pTComplete_Window = new _XWindow_TrainingComplete;
	_XWINDOW_STRUCTURE tcompletestruct =
	{
		TRUE, {0, 0}, {196, 52}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_TRAININGCOMPLETE
	};
	pTComplete_Window->Create(tcompletestruct);
	pTComplete_Window->Initialize();
	pTComplete_Window->ShowWindow(FALSE);
	pTComplete_Window->SetDocking(FALSE);
	g_MainWindowManager.InsertWindow(pTComplete_Window);

	// Create Help window ===========================================
	_XWindow_Help* pHelp_Window = new _XWindow_Help;
	_XWINDOW_STRUCTURE helpstruct = 
	{
		TRUE, {0, 0}, {gnWidth, gnHeight}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(200, 0, 0, 0),
			_XDEF_WTITLE_HELP
	};
	pHelp_Window->Create(helpstruct);
	pHelp_Window->Initialize();
	pHelp_Window->ShowWindow(FALSE);
	pHelp_Window->SetDocking(FALSE);
	pHelp_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pHelp_Window);
	
	// Create EnterBossRoom window ===========================================

	int ebwinxpos = (int)((FLOAT)(gnWidth-1) *( (368.0f+138.0f) / 1023.0f) - 138.0f);
	int ebwinypos = (int)((FLOAT)(gnHeight-1)*( (200.0f+138.0f) /  767.0f) - 138.0f);

	int ebwinxsize = (int)((FLOAT)gnWidth*( 277.0f  /  1024.0f));
	int ebwinysize = (int)((FLOAT)gnHeight*( 277.0f /  768.0f));

	_XWindow_EnterBossRoom* pEnterBossRoom_Window = new _XWindow_EnterBossRoom;
	_XWINDOW_STRUCTURE enterbossroomstruct = 
	{			
		TRUE, { ebwinxpos, ebwinypos }, { ebwinxsize, ebwinysize }, 0, 
			-1,
			//g_MainInterfaceTextureArchive.FindResource( "enterbossroom_base.tga" ),
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ),			
			_XDEF_WTITLE_ENTERBOSSROOM  
	};
	pEnterBossRoom_Window->Create(enterbossroomstruct);
	pEnterBossRoom_Window->Initialize();
	pEnterBossRoom_Window->ShowWindow(FALSE);
	pEnterBossRoom_Window->SetDocking(FALSE);
	pEnterBossRoom_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	pEnterBossRoom_Window->SetEscapeWindow(TRUE);
	g_MainWindowManager.InsertWindow(pEnterBossRoom_Window);
	
	// Create NickName window ===========================================
	_XWindow_NickName* pNickName_Window = new _XWindow_NickName;
	_XWINDOW_STRUCTURE nicknamestruct = 
	{			
		TRUE, { 205, (gnHeight>>1)-205 }, { 306, 342 }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ),
			_XDEF_WTITLE_NICKNAME  
	};
	pNickName_Window->Create(nicknamestruct);
	pNickName_Window->Initialize();
	pNickName_Window->ShowWindow(FALSE);
	pNickName_Window->SetDocking(TRUE, 1);
	pNickName_Window->SetEscapeWindow(TRUE);
	g_MainWindowManager.InsertWindow(pNickName_Window);	

	// Create Socket Plug-in Window =============================================
	_XWindow_SocketPlugIn* pSocketPlugIn_Window = new _XWindow_SocketPlugIn;
	_XWINDOW_STRUCTURE socketstruct =
	{
		TRUE, { (gnWidth>>1)-274, (gnHeight>>1)-270 }, { 183, 270 }, 0,
		g_MainInterfaceTextureArchive.FindResource( "socket_Window.tga" ),
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_SOCKETPLUGIN
	};
	pSocketPlugIn_Window->Create(socketstruct);
	pSocketPlugIn_Window->Initialize();
	pSocketPlugIn_Window->ShowWindow(FALSE);
	pSocketPlugIn_Window->SetDocking(TRUE, 1);
	pSocketPlugIn_Window->SetPlaySoundEfect( TRUE );

	g_MainWindowManager.InsertWindow(pSocketPlugIn_Window);	
	
	// Create Socket Plug-in Window =============================================
	_XWindow_SocketPlugInExp* pSocketPlugInExp_Window = new _XWindow_SocketPlugInExp;
	_XWINDOW_STRUCTURE socketexpstruct = 
	{
		TRUE, {409, (gnHeight>>1)-205}, { _XDEF_WINDOW_DEFAULTWIDTH, _XDEF_WINDOW_DEFAULTHEIGHT }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_SOCKETPLUGINEXP
	};
	pSocketPlugInExp_Window->Create(socketexpstruct);
	pSocketPlugInExp_Window->Initialize();
	pSocketPlugInExp_Window->ShowWindow(FALSE);
	pSocketPlugInExp_Window->EnableWindowEffect(!TRUE);
	pSocketPlugInExp_Window->SetDocking(TRUE, 1);
	
	g_MainWindowManager.InsertWindow(pSocketPlugInExp_Window);	
	
	// Create Default Tooltip Window =============================================
	_XWindow_DefaultTooltip* pDefaultTooltip_Window = new _XWindow_DefaultTooltip;
	_XWINDOW_STRUCTURE defaulttooltipstruct =
	{
		TRUE, { 0, 0 }, { 0, 0 }, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_DEFAULTTOOLTIP
	};
	pDefaultTooltip_Window->Create(defaulttooltipstruct);
	pDefaultTooltip_Window->Initialize();
	pDefaultTooltip_Window->ShowWindow(FALSE);	
	pDefaultTooltip_Window->EnableWindowEffect(FALSE);
	pDefaultTooltip_Window->SetDocking(FALSE, 0);

	g_MainWindowManager.InsertWindow(pDefaultTooltip_Window);
	
	// Create MiniTooltip Window ===================================================
	_XWindow_MiniTooltip* pMiniTooltip_Window = new _XWindow_MiniTooltip;
	_XWINDOW_STRUCTURE minitooltipstruct =
	{
		TRUE, { 0, 0 }, { 0, 0 }, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_DEFAULTTOOLTIP
	};
	pMiniTooltip_Window->Create(minitooltipstruct);
	pMiniTooltip_Window->Initialize();
	pMiniTooltip_Window->ShowWindow(FALSE);	
	pMiniTooltip_Window->EnableWindowEffect(FALSE);
	pMiniTooltip_Window->SetDocking(FALSE, 0);

	g_MainWindowManager.InsertWindow(pMiniTooltip_Window);

	// Create State window =========================================================
	_XWindow_State* pState_Window = new _XWindow_State;
	_XWINDOW_STRUCTURE statestruct = 
	{
		TRUE, {233, 10}, {5, 30}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_STATEWINDOW
	};
	
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	statestruct.position.x = 260;
#endif

	pState_Window->Create(statestruct);
	pState_Window->Initailize();
//	pState_Window->ShowWindow(FALSE);
	pState_Window->EnableWindowEffect(FALSE);
	pState_Window->SetDocking(FALSE, 0);

	g_MainWindowManager.InsertWindow(pState_Window);

	// Create Contribution window =========================================================
	_XWindow_Contribution* pContribution_Window = new _XWindow_Contribution;
	_XWINDOW_STRUCTURE contributionstruct = 
	{
		TRUE, { (gnWidth>>1), (gnHeight>>1)-205 }, { 183, 270 }, 0, 
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ),
		_XDEF_WTITLE_CONTRIBUTIONWINDOW
	};

	pContribution_Window->Create(contributionstruct);
	pContribution_Window->Initailize();
	pContribution_Window->ShowWindow(FALSE);
	pContribution_Window->EnableWindowEffect(!TRUE);
	pContribution_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pContribution_Window);	

	// Create Training Help window =========================================================
	_XWindow_TrainingHelp* pTrainingHelp_Window = new _XWindow_TrainingHelp;
	_XWINDOW_STRUCTURE traininghelpstruct = 
	{
		TRUE, {(gnWidth>>1)-212, (gnHeight>>1)-111}, {425, 408}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_TRAININGHELP
	};
	pTrainingHelp_Window->Create(traininghelpstruct);
	pTrainingHelp_Window->Initialize();
	pTrainingHelp_Window->ShowWindow(!TRUE);
	pTrainingHelp_Window->EnableWindowEffect(FALSE);
	pTrainingHelp_Window->SetDocking(FALSE, 0);
	g_MainWindowManager.InsertWindow(pTrainingHelp_Window);
	
	// Create Hedek window ==================================================================
	_XWindow_Hedek* pHedek_Window = new _XWindow_Hedek;
	_XWINDOW_STRUCTURE hedekstruct = 
	{
		TRUE, {(gnWidth>>1)-127, (gnHeight>>1)-131}, {256, 262}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_HEDEK
	};
	pHedek_Window->Create(hedekstruct);
	pHedek_Window->Initialize();
	pHedek_Window->ShowWindow(!TRUE);
	pHedek_Window->EnableWindowEffect(FALSE);
	pHedek_Window->SetDocking(FALSE, 0);
	pHedek_Window->SetEscapeWindow(TRUE);
	pHedek_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pHedek_Window);

	// Create Help book window ===============================================================
	_XWindow_HelpBook* pHelpBook_Window = new _XWindow_HelpBook;
	_XWINDOW_STRUCTURE helpbookstruct = 
	{
		TRUE, {(gnWidth>>1)-(450>>1), (gnHeight>>1)-(324>>1)}, {450, 316}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_HELPBOOK
	};
	pHelpBook_Window->Create(helpbookstruct);
	pHelpBook_Window->Initialize();
	pHelpBook_Window->ShowWindow(!TRUE);
	pHelpBook_Window->EnableWindowEffect(FALSE);
	pHelpBook_Window->SetDocking(FALSE, 0);
	pHelpBook_Window->SetEscapeWindow(TRUE);
	pHelpBook_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pHelpBook_Window);
	
	// Create Pocket window ==================================================================
	_XWindow_Pocket* pPocket_Window = new _XWindow_Pocket;
	_XWINDOW_STRUCTURE pocketstruct = 
	{
		TRUE, {(gnWidth>>1)-127, (gnHeight>>1)-131}, {167, 146}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_POCKET
	};
	pPocket_Window->Create(pocketstruct);
	pPocket_Window->Initialize();
	pPocket_Window->ShowWindow(!TRUE);
	pPocket_Window->EnableWindowEffect(FALSE);
	pPocket_Window->SetDocking(FALSE, 0);
	g_MainWindowManager.InsertWindow(pPocket_Window);
	
	// Create Tutorial window ==================================================================
	_XWindow_TutorialIcon* pTutorialIcon_Window = new _XWindow_TutorialIcon;
	_XWINDOW_STRUCTURE tutorialiconstruct = 
	{
		TRUE, {0, 0}, {0, 0}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(150, 255, 255, 0),
		_XDEF_WTITLE_TUTORIALICON
	};
	pTutorialIcon_Window->Create(tutorialiconstruct);
	pTutorialIcon_Window->Initialize();
	pTutorialIcon_Window->ShowWindow(!TRUE);
	pTutorialIcon_Window->EnableWindowEffect(FALSE);
	pTutorialIcon_Window->SetDocking(FALSE, 0);
	g_MainWindowManager.InsertWindow(pTutorialIcon_Window);
	pTutorialIcon_Window->SetChattingDefaultWindowPtr( pChatDefault_Window );

	// EventNotice window =========================================================================
	_XWindow_EventNotice* pEventNotice_Window = new _XWindow_EventNotice;
	_XWINDOW_STRUCTURE eventnoticestruct = 
	{
		TRUE, {12, 115}, {0, 0}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(255, 0, 0, 0),
		_XDEF_WTITLE_EVENTNOTICEWINDOW
	};
	pEventNotice_Window->Create(eventnoticestruct);
	pEventNotice_Window->Initialize();
	pEventNotice_Window->ShowWindow(FALSE);
	pEventNotice_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pEventNotice_Window->EnableWindowEffect(FALSE);
	pEventNotice_Window->SetDocking(FALSE, 0);
	g_MainWindowManager.InsertWindow(pEventNotice_Window);

	// GM Command window ==========================================================================
	_XWindow_GMCommand* pGMCommandWindow = new _XWindow_GMCommand;
	_XWINDOW_STRUCTURE gmwindowstruct= 
	{
		TRUE, { gnWidth - (1024-980), 211}, {154, 32 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(150, 255, 255, 0),
			_XDEF_WTITLE_GMWINDOW
	};
	pGMCommandWindow->Create(gmwindowstruct);
	pGMCommandWindow->Initialize();
	pGMCommandWindow->ShowWindow(FALSE);
	pGMCommandWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	pGMCommandWindow->EnableWindowEffect(FALSE);
	pGMCommandWindow->SetDocking(FALSE, 0);
	pGMCommandWindow->SetEdgeDocking(FALSE);
	pGMCommandWindow->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pGMCommandWindow );

	// Create GM send notice message window ========================================================
	_XWindow_GM_SendNoticeMessage* pGMSendNoticeMessageWindow= new _XWindow_GM_SendNoticeMessage;
	_XWINDOW_STRUCTURE gmsendnoticemessagewindowstruct = 
	{
		TRUE, {100, 100}, {450, 200}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW
	};
	pGMSendNoticeMessageWindow->Create(gmsendnoticemessagewindowstruct);
	pGMSendNoticeMessageWindow->Initialize();
	pGMSendNoticeMessageWindow->ShowWindow(FALSE);
	pGMSendNoticeMessageWindow->EnableWindowEffect(FALSE);
	pGMSendNoticeMessageWindow->SetDocking(FALSE, 0);
	pGMSendNoticeMessageWindow->SetEscapeWindow( TRUE );
	pGMSendNoticeMessageWindow->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pGMSendNoticeMessageWindow);

	// Create GM Coordinate control window ==========================================================
	_XWindow_GM_UserCoordination* pGMCoordinateControlWindow = new _XWindow_GM_UserCoordination;
	_XWINDOW_STRUCTURE gmcoordinatecontrolwindowstruct = 
	{
		TRUE, {125, 125}, {450, 84}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW
	};
	pGMCoordinateControlWindow->Create(gmcoordinatecontrolwindowstruct);
	pGMCoordinateControlWindow->Initialize();
	pGMCoordinateControlWindow->ShowWindow(FALSE);
	pGMCoordinateControlWindow->EnableWindowEffect(FALSE);
	pGMCoordinateControlWindow->SetDocking(FALSE, 0);
	pGMCoordinateControlWindow->SetEscapeWindow( TRUE );
	pGMCoordinateControlWindow->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pGMCoordinateControlWindow);
	
	_XWindow_GM_MonsterManage* pGMMonsterManageWindow = new _XWindow_GM_MonsterManage;
	_XWINDOW_STRUCTURE gmmonstermanagewindowstruct = 
	{
		TRUE, {150, 150}, {450, 160}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_GMCONTROLMONSTERWINDOW
	};
	pGMMonsterManageWindow->Create(gmmonstermanagewindowstruct);
	pGMMonsterManageWindow->Initialize();
	pGMMonsterManageWindow->ShowWindow(FALSE);
	pGMMonsterManageWindow->EnableWindowEffect(FALSE);
	pGMMonsterManageWindow->SetDocking(FALSE, 0);
	pGMMonsterManageWindow->SetEscapeWindow( FALSE );
	pGMMonsterManageWindow->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pGMMonsterManageWindow);

	_XWindow_GM_StatusControl* pGMStatusWindow = new _XWindow_GM_StatusControl;
	_XWINDOW_STRUCTURE gmstatuswindowstruct = 
	{
		TRUE, {175, 175}, {450, 104}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_GMCHARACTERSETTINGWINDOW
	};
	pGMStatusWindow->Create(gmstatuswindowstruct);
	pGMStatusWindow->Initialize();
	pGMStatusWindow->ShowWindow(FALSE);
	pGMStatusWindow->EnableWindowEffect(FALSE);
	pGMStatusWindow->SetDocking(FALSE, 0);
	pGMStatusWindow->SetEscapeWindow( TRUE );
	pGMStatusWindow->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pGMStatusWindow);

	// Create GM Event manager window ==========================================================
	_XWindow_GM_EventManager* pGMEventManagerWindow = new _XWindow_GM_EventManager;
	_XWINDOW_STRUCTURE gmeventmanagerwindowstruct = 
	{
		TRUE, {190, 190}, {450, 180}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_GMEVENTCONTROLWINDOW
	};
	pGMEventManagerWindow->Create(gmeventmanagerwindowstruct);
	pGMEventManagerWindow->Initialize();
	pGMEventManagerWindow->ShowWindow(FALSE);
	pGMEventManagerWindow->EnableWindowEffect(FALSE);
	pGMEventManagerWindow->SetDocking(FALSE, 0);
	pGMEventManagerWindow->SetEscapeWindow( TRUE );
	pGMEventManagerWindow->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pGMEventManagerWindow);
	
	// Create Gamble Window =============================================
	_XWindow_Gamble* pGamble_Window = new _XWindow_Gamble;
	_XWINDOW_STRUCTURE gamblestruct =
	{
		TRUE, { (gnWidth>>1)-274, (gnHeight>>1)-270 }, { 183, 270 }, 0,
		g_MainInterfaceTextureArchive.FindResource( "socket_Window.tga" ),
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_GAMBLE
	};
	pGamble_Window->Create(gamblestruct);
	pGamble_Window->Initialize();
	pGamble_Window->ShowWindow(FALSE);
	pGamble_Window->SetDocking(TRUE, 1);
	pGamble_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow(pGamble_Window);	

	// Create character PVP Broker window=================================================
	_XWindow_PVPBroker* pPVPBroker_Window = new _XWindow_PVPBroker;
	_XWINDOW_STRUCTURE pvpbrokerwindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 366, 270 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 64, 0, 0, 0 ), 
			_XDEF_WTITLE_PVPBROKER
	};
	pPVPBroker_Window->Create( pvpbrokerwindowstruct );
	pPVPBroker_Window->Initialize();
	pPVPBroker_Window->ShowWindow( !TRUE );
	pPVPBroker_Window->EnableWindowEffect( !TRUE );
	pPVPBroker_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pPVPBroker_Window );

	// Create character MatchTable PVP window=================================================
	_XWindow_MatchTablePVP* pMatchTablePVP_Window = new _XWindow_MatchTablePVP;
	_XWINDOW_STRUCTURE matchtablepvpwindowstruct = 
	{

#ifdef _ACCLAIM_RUBICONADSYSTEM	
		TRUE, { (gnWidth>>1)-124, (gnHeight>>1)-270 + g_RubiconADWindowSize.cy }, { 306, 497 }, 0,
#else
		TRUE, { (gnWidth>>1)-124, (gnHeight>>1)-270 }, { 306, 497 }, 0,
#endif
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 64, 0, 0, 0 ), 
			_XDEF_WTITLE_MATCHTABLEPVP
	};
	pMatchTablePVP_Window->Create(matchtablepvpwindowstruct);
	pMatchTablePVP_Window->Initialize();
	pMatchTablePVP_Window->ShowWindow(!TRUE);
	pMatchTablePVP_Window->EnableWindowEffect(FALSE);
	pMatchTablePVP_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pMatchTablePVP_Window);
	
	// Create character MatchNoticeLarge window =================================================
	_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = new _XWindow_MatchNoticeLarge;
	_XWINDOW_STRUCTURE matchnoticelargestruct =
	{
#ifdef _ACCLAIM_RUBICONADSYSTEM	
		TRUE, {(gnWidth>>1) -264, (gnHeight>>1)-278+g_RubiconADWindowSize.cy}, {525, 76}, 0,
#else
		TRUE, {(gnWidth>>1) -264, (gnHeight>>1)-278}, {525, 76}, 0,
#endif
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(64, 0, 0, 0),
		_XDEF_WTITLE_MATCHNOTICELARGE
	};
	pMatchNoticeLarge_Window->Create(matchnoticelargestruct);
	pMatchNoticeLarge_Window->Initialize();
	pMatchNoticeLarge_Window->ShowWindow(!TRUE);
	pMatchNoticeLarge_Window->EnableWindowEffect(FALSE);
	pMatchNoticeLarge_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pMatchNoticeLarge_Window);

	// Create character MatchNoticeLarge window =================================================
	_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = new _XWindow_MatchNoticeSmall;
	_XWINDOW_STRUCTURE matchnoticesmallstruct =
	{
		TRUE, {(gnWidth>>1)+115, (gnHeight>>1)-369}, {222, 59}, 0,
		-1, 
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(64, 0, 0, 0),
		_XDEF_WTITLE_MATCHNOTICESMALL
	};
	pMatchNoticeSmall_Window->Create(matchnoticesmallstruct);
	pMatchNoticeSmall_Window->Initialize();
	pMatchNoticeSmall_Window->ShowWindow(!TRUE);
	pMatchNoticeSmall_Window->EnableWindowEffect(FALSE);
	pMatchNoticeSmall_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pMatchNoticeSmall_Window);
	
	// Create character MatchMessage window =================================================
	_XWindow_MatchMessage* pMatchMessage_Window = new _XWindow_MatchMessage;
	_XWINDOW_STRUCTURE matchmessagestruct = 
	{
#ifdef _ACCLAIM_RUBICONADSYSTEM			
		TRUE, {0, 0+g_RubiconADWindowSize.cy}, {1, 1}, 0,
#else
		TRUE, {0, 0}, {1, 1}, 0,
#endif
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_MATCHMESSAGE
	};
	pMatchMessage_Window->Create(matchmessagestruct);
	pMatchMessage_Window->Initialize();
	pMatchMessage_Window->ShowWindow(TRUE);
	pMatchMessage_Window->EnableWindowEffect(FALSE);
	pMatchMessage_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pMatchMessage_Window);

	// Create character MatchRecord window =================================================
	_XWindow_MatchRecord* pMatchRecord_Window = new _XWindow_MatchRecord;
	_XWINDOW_STRUCTURE matchrecordstruct =
	{
		TRUE, { (gnWidth>>1)-124, (gnHeight>>1)-270 }, {306, 361}, 0, 
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(64, 0, 0, 0),
		_XDEF_WTITLE_MATCHRECORD
	};
	pMatchRecord_Window->Create(matchrecordstruct);
	pMatchRecord_Window->Initialize();
	pMatchRecord_Window->ShowWindow(!TRUE);
	pMatchRecord_Window->EnableWindowEffect(FALSE);
	pMatchRecord_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pMatchRecord_Window);
	
	// Create Match PR window =================================================
	_XWindow_MatchPR* pMatchPR_Window= new _XWindow_MatchPR;
	_XWINDOW_STRUCTURE matchprstruct =
	{
		TRUE,{ 388, 114 }, { 288, 81 } , 0,
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(64, 0, 0, 0),
			_XDEF_WTITLE_MATCHPR			
	};
	pMatchPR_Window->Create(matchprstruct);
	pMatchPR_Window->Initialize();
	pMatchPR_Window->ShowWindow(!TRUE);
	pMatchPR_Window->EnableWindowEffect(FALSE);
	pMatchPR_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pMatchPR_Window);
	
	// Create Match PR window =================================================
	_XWindow_NPCBuff* pNPCBuff_Window = new _XWindow_NPCBuff;
	_XWINDOW_STRUCTURE npcbuffstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 366, 270 }, 0,
		-1, 
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(64, 0, 0, 0),
		_XDEF_WTITLE_NPCBUFF
	};
	pNPCBuff_Window->Create(npcbuffstruct);
	pNPCBuff_Window->Initialize();
	pNPCBuff_Window->ShowWindow(!TRUE);
	pNPCBuff_Window->EnableWindowEffect(FALSE);
	pNPCBuff_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pNPCBuff_Window);
	
	_XWindow_AreaSelect* pAreaSelectWindow_White = new _XWindow_AreaSelect;
	_XWINDOW_STRUCTURE areaselectstruct_white =
	{
		TRUE,{ (gnWidth>>1)-135, 205 }, { 310, 104 /*140*/ } , 0,
		-1, 
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(32, 0, 0, 0),
		_XDEF_WTITLE_AREASELECTWINDOW_WHITE
	};
	pAreaSelectWindow_White->Create(areaselectstruct_white);
	pAreaSelectWindow_White->Initialize();
	pAreaSelectWindow_White->SetDocking( FALSE );	
	pAreaSelectWindow_White->ShowWindow(!TRUE);
	pAreaSelectWindow_White->EnableWindowEffect(FALSE);	
	pAreaSelectWindow_White->SetWhiteClan();
	g_pWhiteClanAreaSelect_Window = pAreaSelectWindow_White;

	g_MainWindowManager.InsertWindow(pAreaSelectWindow_White);

	_XWindow_AreaSelect* pAreaSelectWindow_Black = new _XWindow_AreaSelect;
	_XWINDOW_STRUCTURE areaselectstruct_black =
	{
		TRUE,{ (gnWidth>>1)-135, 205 }, { 310, 104 /*140*/ } , 0,
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(32, 0, 0, 0),
			_XDEF_WTITLE_AREASELECTWINDOW_BLACK
	};
	pAreaSelectWindow_Black->Create(areaselectstruct_black);
	pAreaSelectWindow_Black->Initialize();
	pAreaSelectWindow_Black->SetDocking( FALSE );	
	pAreaSelectWindow_Black->ShowWindow(!TRUE);
	pAreaSelectWindow_Black->EnableWindowEffect(FALSE);	
	pAreaSelectWindow_Black->SetBlackClan();
	g_pBlackClanAreaSelect_Window = pAreaSelectWindow_Black;
	g_MainWindowManager.InsertWindow(pAreaSelectWindow_Black);
	
	_XWindow_AreaSelect* pAreaSelectWindow_Multi = new _XWindow_AreaSelect;
	_XWINDOW_STRUCTURE areaselectstruct_multi =
	{
		TRUE,{ (gnWidth>>1)-135, 205 }, { 310, 104 /*140*/ } , 0,
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(32, 0, 0, 0),
			_XDEF_WTITLE_AREASELECTWINDOW_MULTI
	};
	pAreaSelectWindow_Multi->Create(areaselectstruct_multi);
	pAreaSelectWindow_Multi->Initialize();
	pAreaSelectWindow_Multi->SetDocking( FALSE );	
	pAreaSelectWindow_Multi->ShowWindow(!TRUE);
	pAreaSelectWindow_Multi->EnableWindowEffect(FALSE);	
	g_pMultiAreaSelect_Window = pAreaSelectWindow_Multi;
	g_MainWindowManager.InsertWindow(pAreaSelectWindow_Multi);
	
	// Create cashItem window =================================================
	_XWindow_CashItem* pCashItem_Window = new _XWindow_CashItem;
	_XWINDOW_STRUCTURE cashitemstruct =
	{
		TRUE, { (gnWidth>>1)-213, (gnHeight>>1)-270 }, {425, 410}, 0, 
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_CASHITEM
	};
	pCashItem_Window->Create(cashitemstruct);
	pCashItem_Window->Initialize();
	pCashItem_Window->ShowWindow(FALSE);
	pCashItem_Window->EnableWindowEffect(FALSE);
	pCashItem_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pCashItem_Window);
	
	// Create NPC Collection window =================================================
	_XWindow_NPCCollection* pNPCCollection_Window= new _XWindow_NPCCollection;
	_XWINDOW_STRUCTURE npccollectionstruct =
	{
		TRUE,{ 609, 222 }, { 368, 268 } , 0,
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_NPCCOLLECTION			
	};
	pNPCCollection_Window->Create(npccollectionstruct);
	pNPCCollection_Window->Initialize();
	pNPCCollection_Window->ShowWindow(!TRUE);
	pNPCCollection_Window->EnableWindowEffect(FALSE);
	pNPCCollection_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow(pNPCCollection_Window);
	
	// Create NPC Resource window =================================================
	_XWindow_NPCResource* pNPCResource_Window= new _XWindow_NPCResource;
	_XWINDOW_STRUCTURE npcresourcestruct =
	{
		TRUE,{ 609, 222 }, { 368, 268 } , 0,
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_NPCRESOURCE			
	};
	pNPCResource_Window->Create(npcresourcestruct);
	pNPCResource_Window->Initialize();
	pNPCResource_Window->ShowWindow(!TRUE);
	pNPCResource_Window->EnableWindowEffect(FALSE);
	pNPCResource_Window->SetWindowMoveMode( _XWMOVE_FIXED );	
	g_MainWindowManager.InsertWindow(pNPCResource_Window);
	
	// Create NEW NPC resource window =========================================
	_XWindow_NPCResource_New* pNPCResource_New_Window= new _XWindow_NPCResource_New;
	_XWINDOW_STRUCTURE newresourcewindow =
	{
		TRUE,{ 602, 222 }, { 182, 270 } , 0,
			-1, 
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_NEW_RESOURCEWINDOW			
	};
	pNPCResource_New_Window->Create(newresourcewindow);
	pNPCResource_New_Window->Initialize();
	pNPCResource_New_Window->ShowWindow(!TRUE);
	pNPCResource_New_Window->EnableWindowEffect(FALSE);
	pNPCResource_New_Window->SetWindowMoveMode( _XWMOVE_FIXED );	
	g_MainWindowManager.InsertWindow(pNPCResource_New_Window);
	
	// Create Weapon Grade Window =============================================
	_XWindow_Inchant* pInchant_Window = new _XWindow_Inchant;
	_XWINDOW_STRUCTURE upgradestruct =
	{
		TRUE, { (gnWidth>>1)-274, (gnHeight>>1)-270 }, { 183, 270 }, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_INCHANT
	};
	pInchant_Window->Create(upgradestruct);
	pInchant_Window->Initialize();
	pInchant_Window->ShowWindow(FALSE);
	pInchant_Window->SetDocking(TRUE, 1);
	pInchant_Window->SetPlaySoundEfect( TRUE );
	pInchant_Window->SetEnableTopWindow( FALSE );

	g_MainWindowManager.InsertWindow(pInchant_Window);	
	

	// Payment method window =============================================
	_XWindow_PaymentWindow* pPaymentMethod_Window = new _XWindow_PaymentWindow;
	_XWINDOW_STRUCTURE paymentmethodstruct =
	{
		TRUE, { 82, 73 }, { 146, 20 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 255, 0, 0, 0 ), 
			_XDEF_WTITLE_PAYMENTMETHODWINDOW
	};
	pPaymentMethod_Window->Create(paymentmethodstruct);
	pPaymentMethod_Window->Initialize();
	pPaymentMethod_Window->ShowWindow(FALSE);
	pPaymentMethod_Window->SetDocking(FALSE, 0);
	pPaymentMethod_Window->SetPlaySoundEfect( TRUE );
	pPaymentMethod_Window->SetUseBorderImage( FALSE );
	pPaymentMethod_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	
	g_MainWindowManager.InsertWindow(pPaymentMethod_Window);
	
	// 경험치/숙련도/구룡기행 window =============================================
	_XWindow_AccountItemUse* pAccoutnItenUse_Window = new _XWindow_AccountItemUse;
	_XWINDOW_STRUCTURE accountitemusestruct =
	{
		TRUE, 
		{ 82, 73 }, 
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI //Author :양희왕// breif : 구룡창 사이즈 변경 
		{ 173, 20 }, 
#else
		{ 146, 20 }, 
#endif
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 255, 0, 0, 0 ), 
		_XDEF_WTITLE_ACCOUNTITEMUSE
	};
	pAccoutnItenUse_Window->Create(accountitemusestruct);
	pAccoutnItenUse_Window->Initialize();
	pAccoutnItenUse_Window->ShowWindow(FALSE);
	pAccoutnItenUse_Window->SetDocking(FALSE, 0);
	pAccoutnItenUse_Window->SetPlaySoundEfect( TRUE );
	pAccoutnItenUse_Window->SetUseBorderImage( FALSE );
	pAccoutnItenUse_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	
	g_MainWindowManager.InsertWindow(pAccoutnItenUse_Window);	
	
	// PC방 표시 window =============================================
	_XWindow_PCRPremium* pPCRPremiun_Window = new _XWindow_PCRPremium;
	_XWINDOW_STRUCTURE pcrstruct =
	{
		TRUE, { 82, 73 }, { 146, 20 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 255, 0, 0, 0 ), 
			_XDEF_WTITLE_PCRPREMIUN_WINDOW
	};
	pPCRPremiun_Window->Create(pcrstruct);
	pPCRPremiun_Window->Initialize();
	pPCRPremiun_Window->ShowWindow(FALSE);
	pPCRPremiun_Window->SetDocking(FALSE, 0);
	pPCRPremiun_Window->SetPlaySoundEfect( TRUE );
	pPCRPremiun_Window->SetUseBorderImage( FALSE );
	pPCRPremiun_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	
	g_MainWindowManager.InsertWindow(pPCRPremiun_Window);
	
	// Create bookopen window =================================================
	_XWindow_BookOpen* pBookOpen_Window = new _XWindow_BookOpen;
	_XWINDOW_STRUCTURE bookopenstruct =
	{
		TRUE, { (gnWidth>>1)-213, (gnHeight>>1)-270 }, {425, 410}, 0, 
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_BOOKOPENWINDOW
	};
	pBookOpen_Window->Create(bookopenstruct);
	pBookOpen_Window->Initialize();
	pBookOpen_Window->ShowWindow(FALSE);
	pBookOpen_Window->EnableWindowEffect(FALSE);
	pBookOpen_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow(pBookOpen_Window);	
	
	// Create character Quest Rank Window=================================================
	_XWindow_QuestRank* pQuestRank_Window = new _XWindow_QuestRank;
	_XWINDOW_STRUCTURE questrankwindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 366, 270 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 64, 0, 0, 0 ), 
			_XDEF_WTITLE_QUESTRANKWINDOW
	};
	pQuestRank_Window->Create( questrankwindowstruct );
	pQuestRank_Window->Initialize();
	pQuestRank_Window->ShowWindow( !TRUE );
	pQuestRank_Window->EnableWindowEffect( !TRUE );
	pQuestRank_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pQuestRank_Window );
	
	// Create PK Window========================================================

	int pkuiresourcindex = -1;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pk_01.tga");
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pk_01.tga");	
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pk_01.tga");	
	else
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("mi_pk_01.tga");	

	_XWindow_PK* pPK_Window = new _XWindow_PK;
	_XWINDOW_STRUCTURE pkstruct = 
	{
#ifdef _ACCLAIM_RUBICONADSYSTEM			
		TRUE, {(gnWidth>>1)-89, (gnHeight>>1)-288 + g_RubiconADWindowSize.cy}, {178, 42}, 0,
#else
		TRUE, {(gnWidth>>1)-89, (gnHeight>>1)-288}, {178, 42}, 0,
#endif
		pkuiresourcindex,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_PKWINDOW
	};
	pPK_Window->Create(pkstruct);
	pPK_Window->Initialize();
	pPK_Window->ShowWindow(!TRUE);
	pPK_Window->EnableWindowEffect(FALSE);
	g_MainWindowManager.InsertWindow(pPK_Window);
	
#ifdef _XTS_BLOODPOINT
	// Create PK Window========================================================
	_XWindow_BloodPoint* pBloodPoint_Window = new _XWindow_BloodPoint;
	_XWINDOW_STRUCTURE blpstruct = 
	{
		TRUE, {gnWidth - 72, gnHeight - 217}, {58, 19}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_BLOODPOINTWINDOW
	};
	pBloodPoint_Window->Create(blpstruct);
	pBloodPoint_Window->Initialize();
	pBloodPoint_Window->ShowWindow(TRUE);
	pBloodPoint_Window->EnableWindowEffect(FALSE);
	g_MainWindowManager.InsertWindow(pBloodPoint_Window);
#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = new _XWindow_MnDCurrentPoint;
	_XWINDOW_STRUCTURE mndCurrentPointstruct = 
	{
		TRUE, 
		{pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19}, 
		{58, 19}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_MNDCURRENTPOINTWINDOW
	};
	pMnDCurrentPoint_Window->Create(mndCurrentPointstruct);
	pMnDCurrentPoint_Window->Initialize();
	pMnDCurrentPoint_Window->ShowWindow(TRUE);
	pMnDCurrentPoint_Window->EnableWindowEffect(FALSE);
	pMnDCurrentPoint_Window->SetNumberProcess();
	g_MainWindowManager.InsertWindow(pMnDCurrentPoint_Window);

	_XWindow_MnDTotalPoint* pMnDToTalPoint_Window = new _XWindow_MnDTotalPoint;
	_XWINDOW_STRUCTURE mndTotalPointstruct = 
	{
		TRUE, 
		{pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19}, 
		{58, 19}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_MNDTOTALPOINTWINDOW
	};
	pMnDToTalPoint_Window->Create(mndTotalPointstruct);
	pMnDToTalPoint_Window->Initialize();
	pMnDToTalPoint_Window->ShowWindow(TRUE);
	pMnDToTalPoint_Window->EnableWindowEffect(FALSE);
	pMnDToTalPoint_Window->SetNumberProcess();
	g_MainWindowManager.InsertWindow(pMnDToTalPoint_Window);
#endif
	// Create GuideLine Window========================================================
	_XWindow_GuideLine* pGuideLine_Window = new _XWindow_GuideLine;
	_XWINDOW_STRUCTURE glstruct = 
	{
		TRUE, {(gnWidth>>1)-181, (gnHeight>>1)-300}, {361, 136}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_GUIDELINEWINDOW
	};
	pGuideLine_Window->Create(glstruct);
	pGuideLine_Window->Initialize();
	pGuideLine_Window->ShowWindow(FALSE);
	pGuideLine_Window->EnableWindowEffect(FALSE);
	g_MainWindowManager.InsertWindow(pGuideLine_Window);	
	
#ifdef _XDWDEBUG
	// ====================================================
	// Create xdebug window
	_XWindow_Debug* pDebugInfo_Window = new _XWindow_Debug;
	_XWINDOW_STRUCTURE xdebugwindowstruct = 
	{
		TRUE, { 0, 0 }, { 314, 100 }, 0,
		-1, 
		NULL,
		D3DCOLOR_ARGB( 96, 0, 0, 0 ),
		_XDEF_WTITLE_XDEBUGWINDOW
	};
	pDebugInfo_Window->Create( xdebugwindowstruct );
	pDebugInfo_Window->Initialize();		
	pDebugInfo_Window->SetDocking( FALSE );	
	pDebugInfo_Window->ShowWindow( FALSE );
	pDebugInfo_Window->EnableWindowEffect(!TRUE);	
	pDebugInfo_Window->SetEscapeWindow( TRUE );
	g_MainWindowManager.InsertWindow( pDebugInfo_Window );
#endif
	
	// NPC Search window=================================================
	_XWindow_NPCSearch* pNPCSearch_Window = new _XWindow_NPCSearch;
	_XWINDOW_STRUCTURE npcsearchwindowstruct = 
	{
		TRUE, { gnWidth-154, 0 }, { 112, 210 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_NPCSEARCHWINDOW
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		npcsearchwindowstruct.position.x = gnWidth-154-42;
		npcsearchwindowstruct.windowsize.cx = 154;
	}

	pNPCSearch_Window->Create( npcsearchwindowstruct );
	pNPCSearch_Window->Initialize();
	pNPCSearch_Window->ShowWindow( !TRUE );
	pNPCSearch_Window->EnableWindowEffect( !TRUE );
	pNPCSearch_Window->SetDocking( FALSE );
	pNPCSearch_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pNPCSearch_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pNPCSearch_Window );
	
#ifdef _XTS_ITEMMALLBROWSER	
	// Cash Mall window=================================================
	_XWindow_CashMall* pCashMall_Window = new _XWindow_CashMall;
	_XWINDOW_STRUCTURE cashmallwindowstruct = 
	{
		//TRUE, { 87, 36 }, { 820, 700 }, 0,
		//TRUE, { (gnWidth>>1)-410, (gnHeight>>1)-350 }, { 820, 700 }, 0,
		TRUE, { 0, 0 }, { gnWidth, gnHeight }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CASHMALL
	};
	pCashMall_Window->Create( cashmallwindowstruct );
	pCashMall_Window->Initialize();
	pCashMall_Window->ShowWindow( !TRUE );
	pCashMall_Window->EnableWindowEffect( !TRUE );
	pCashMall_Window->SetDocking( FALSE );
	pCashMall_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pCashMall_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCashMall_Window );

	g_pCashMall_Window = pCashMall_Window;

	// Cash Box window=================================================
	_XWindow_CashBox* pCashBox_Window = new _XWindow_CashBox;
	_XWINDOW_STRUCTURE cashboxwindowstruct = 
	{
		TRUE, { 400, 150 }, { 327, 442+32 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CASHBOX
	};
	pCashBox_Window->Create( cashboxwindowstruct );
	pCashBox_Window->Initialize();
	pCashBox_Window->ShowWindow( !TRUE );
	pCashBox_Window->EnableWindowEffect( !TRUE );
	pCashBox_Window->SetDocking( FALSE );
	pCashBox_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCashBox_Window );
#endif

#ifdef _XTS_MOVEVILLAGE
	//  Move Village window =============================================
	_XWindow_MoveVillage* pMoveVillage_Window = new _XWindow_MoveVillage;
	_XWINDOW_STRUCTURE movevillagestruct = 
	{
		TRUE, {(gnWidth)-415, (gnHeight>>1)-157}, {375, 280}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(127, 0, 0, 0),
		_XDEF_WTITLE_MOVEVILLAGE
	};
	
#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	movevillagestruct.position.x = (gnWidth>>1) - 272;
	movevillagestruct.position.y = (gnHeight>>1) - 140;

	movevillagestruct.windowsize.cx = 545;
#endif

	pMoveVillage_Window->Create(movevillagestruct);
	pMoveVillage_Window->Initialize();
	pMoveVillage_Window->ShowWindow(!TRUE);
	pMoveVillage_Window->EnableWindowEffect(!TRUE);
	pMoveVillage_Window->SetDocking(TRUE);
	pMoveVillage_Window->SetPlaySoundEfect(TRUE);
	g_MainWindowManager.InsertWindow(pMoveVillage_Window);
#endif
	
	// Create Event window ===========================================
	_XWindow_Event* pEvent_Window = new _XWindow_Event;
	_XWINDOW_STRUCTURE eventstruct = 
	{			
		TRUE, { 205, (gnHeight>>1)-205 }, { 306, 342 }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ),
			_XDEF_WTITLE_EVENT  
	};
	pEvent_Window->Create(eventstruct);
	pEvent_Window->Initialize();
	pEvent_Window->ShowWindow(FALSE);
	pEvent_Window->SetDocking(TRUE, 1);
	pEvent_Window->SetEscapeWindow(TRUE);
	g_MainWindowManager.InsertWindow(pEvent_Window);	
	
#ifdef _XDEF_WEBBOARD_20061211	
	// Cash Mall window=================================================
	_XWindow_WebBoard* pWebBoard_Window = new _XWindow_WebBoard;
	_XWINDOW_STRUCTURE webboardwindowstruct = 
	{
		TRUE, { 0, 0 }, { gnWidth, gnHeight }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_WEBBOARD
	};
	pWebBoard_Window->Create( webboardwindowstruct );
	pWebBoard_Window->Initialize();
	pWebBoard_Window->ShowWindow( !TRUE );
	pWebBoard_Window->EnableWindowEffect( !TRUE );
	pWebBoard_Window->SetDocking( FALSE );
	pWebBoard_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	pWebBoard_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pWebBoard_Window );	
#endif
	
	//장원전 사자후 관련
	// CastleBattle Shout window=================================================
	_XWindow_CastleBattleShout* pCastleBattleShout_Window = new _XWindow_CastleBattleShout;
	_XWINDOW_STRUCTURE shoutwindowstruct = 
	{
		TRUE, { (gnWidth>>1)-185, (gnHeight>>1)-55 }, { 390, 110 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CASTLEBATTLE_SHOUT
	};
	pCastleBattleShout_Window->Create( shoutwindowstruct );
	pCastleBattleShout_Window->Initialize();
	pCastleBattleShout_Window->ShowWindow( !TRUE );
	pCastleBattleShout_Window->EnableWindowEffect( !TRUE );
	pCastleBattleShout_Window->SetDocking( FALSE );
	pCastleBattleShout_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCastleBattleShout_Window );	
	
	// 장원전 시간변경 관련
	// CastleBattle Shout window=================================================
	_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = new _XWindow_CastleBattleTimeChange;
	_XWINDOW_STRUCTURE timechangewindowstruct = 
	{
		TRUE, { (gnWidth>>1)-91, (gnHeight>>1)-83 }, { 183, 167 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE
	};
	pCastleBattleTimechange_Window->Create( timechangewindowstruct );
	pCastleBattleTimechange_Window->Initialize();
	pCastleBattleTimechange_Window->ShowWindow( !TRUE );
	pCastleBattleTimechange_Window->EnableWindowEffect( !TRUE );
	pCastleBattleTimechange_Window->SetDocking( FALSE );
	pCastleBattleTimechange_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCastleBattleTimechange_Window );	
	
	// 장원전 - 협박 진행 gauge
	// Create Threaten gauge window ================================================
	_XWindow_ThreatenGauge* pThreatenGauge_Window = new _XWindow_ThreatenGauge;
	_XWINDOW_STRUCTURE threatengaugestruct = 
	{
		TRUE, {200, 200}, {203, 27}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_THREATENGAUGE
	};
	pThreatenGauge_Window->Create(threatengaugestruct);
	pThreatenGauge_Window->Initialize();
	pThreatenGauge_Window->ShowWindow(FALSE);
	pThreatenGauge_Window->SetDocking(FALSE, 0);
	g_MainWindowManager.InsertWindow(pThreatenGauge_Window);
	
	// 장원전 피아 식별 마크
	// Create Mark window ========================================================
	g_pMark_Window = new _XWindow_Mark;
	_XWINDOW_STRUCTURE markstruct = 
	{
		TRUE, {0, 0}, {32, 45}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(255, 0, 0, 0),
		_XDEF_WTITLE_MARK
	};
	g_pMark_Window->Create(markstruct);
	g_pMark_Window->Initialize();
	g_pMark_Window->ShowWindow(FALSE);
	g_pMark_Window->SetDocking(FALSE, 0);
	g_MainWindowManager.InsertWindow(g_pMark_Window);
	
	// 장원전 포상 내역 : 특별 보상
	// CastleBattle Reward window=================================================
	_XWindow_CastleBattleRewardList* pCastleBattleRewardList_Window = new _XWindow_CastleBattleRewardList;
	_XWINDOW_STRUCTURE rewardlistwindowstruct = 
	{
		TRUE, { (gnWidth>>1)-151, (gnHeight>>1)-200 }, { 375, 401 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CASTLEBATTLE_REWARDLIST
	};
	pCastleBattleRewardList_Window->Create( rewardlistwindowstruct );
	pCastleBattleRewardList_Window->Initialize();
	pCastleBattleRewardList_Window->ShowWindow( !TRUE );
	pCastleBattleRewardList_Window->EnableWindowEffect( !TRUE );
	pCastleBattleRewardList_Window->SetDocking( FALSE );
	pCastleBattleRewardList_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCastleBattleRewardList_Window );
	
	// 장원전 포상 내역 : 개인 보상
	// CBRewardSmall ==============================================================
	_XWindow_CBRewardSmall* pCBRewardSmall_Window = new _XWindow_CBRewardSmall;
	_XWINDOW_STRUCTURE rewardsmallstruct = 
	{
		TRUE, { (gnWidth>>1)-151, (gnHeight>>1)-94 }, { 347, 189 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CBREWARDSMALL
	};
	pCBRewardSmall_Window->Create( rewardsmallstruct );
	pCBRewardSmall_Window->Initialize();
	pCBRewardSmall_Window->ShowWindow( !TRUE );
	pCBRewardSmall_Window->EnableWindowEffect( !TRUE );
	pCBRewardSmall_Window->SetDocking( FALSE );
	pCBRewardSmall_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCBRewardSmall_Window );
	
	// 장원전 시간표시: 장원혈
	// CastleBattle Personal History window============================================
	_XWindow_CastleBattlePersonalHistory* pCastleBattlePersonalHistory = new _XWindow_CastleBattlePersonalHistory;
	_XWINDOW_STRUCTURE personalhistorywindowstruct = 
	{
		TRUE, { gnWidth-341, 0 }, { 187, 144 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY
	};	

#ifdef _XDEF_CASTLEBATTLE_RECORD
	personalhistorywindowstruct.windowsize.cy = 171;
#endif

	pCastleBattlePersonalHistory->Create( personalhistorywindowstruct );
	pCastleBattlePersonalHistory->Initialize();
	pCastleBattlePersonalHistory->ShowWindow( !TRUE );
	pCastleBattlePersonalHistory->EnableWindowEffect( !TRUE );
	pCastleBattlePersonalHistory->SetDocking( TRUE );
	pCastleBattlePersonalHistory->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCastleBattlePersonalHistory );
	
	// 장원전 중 개인 전적 
	// CastleBattle Record window======================================================
	_XWindow_CBRecord* pCBRecord_window = new _XWindow_CBRecord;
	_XWINDOW_STRUCTURE cbrecordstruct = 
	{
		TRUE, { gnWidth-341, 172 }, { 187, 185 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CBRECORD
	};	

	pCBRecord_window->Create( cbrecordstruct );
	pCBRecord_window->Initialize();
	pCBRecord_window->ShowWindow( !TRUE );
	pCBRecord_window->EnableWindowEffect( !TRUE );
	pCBRecord_window->SetDocking( TRUE );
	pCBRecord_window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCBRecord_window );

	// 장원전 후 전체 전적 
	// CastleBattle Org Record window===================================================
	_XWindow_CBOrgRecord* pCBOrgRecord_window = new _XWindow_CBOrgRecord;
	_XWINDOW_STRUCTURE cborgrecordstruct = 
	{
		TRUE, { (gnWidth>>1)-352, (gnHeight>>1)-219 }, { 704, 438 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_CBORGRECORD
	};	

	pCBOrgRecord_window->Create( cborgrecordstruct );
	pCBOrgRecord_window->Initialize();
	pCBOrgRecord_window->ShowWindow( !TRUE );
	pCBOrgRecord_window->EnableWindowEffect( !TRUE );
	pCBOrgRecord_window->SetDocking( TRUE );
	pCBOrgRecord_window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pCBOrgRecord_window );
		
#ifdef _XDEF_ESCAPE_20070205	
	// 탈출 관련
	// Escape window=================================================
	_XWindow_Escape* pEscape_Window = new _XWindow_Escape;
	_XWINDOW_STRUCTURE escapewindowstruct = 
	{
		TRUE, { (gnWidth>>1)-91, (gnHeight>>1)-72 }, { 183, 144 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ), 
			_XDEF_WTITLE_ESCAPE
	};
	pEscape_Window->Create( escapewindowstruct );
	pEscape_Window->Initialize();
	pEscape_Window->ShowWindow( !TRUE );
	pEscape_Window->EnableWindowEffect( !TRUE );
	pEscape_Window->SetDocking( FALSE );
	pEscape_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pEscape_Window);	
#endif

// Author : 양희왕
// breif : pk 위치추적 리스트
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPk_TracingServicesWindow = new XWindow_PK_ListBox;
	_XWINDOW_STRUCTURE  pktracingservices = 
	{
		TRUE,
		{ gnWidth-154, 220 }, // 클라이언트에서 위치
		{ 154, 210 }, // 크기
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 255, 0, 0, 0 ), 
		_XDEF_WTITLE_PK_TRACINGSERVICES
	};

	pPk_TracingServicesWindow->Create( pktracingservices );
	pPk_TracingServicesWindow->Initialize();
	pPk_TracingServicesWindow->ShowWindow( FALSE );
	pPk_TracingServicesWindow->EnableWindowEffect( FALSE );
	pPk_TracingServicesWindow->SetEscapeWindow(TRUE);
	g_MainWindowManager.InsertWindow( pPk_TracingServicesWindow );	
#endif
//Last Updated : 07/04/03
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK범위치추적, 레벨차 PK제한	
	// Escape window=================================================
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = new _XWindow_PKTracingRequest;
	_XWINDOW_STRUCTURE pktracingrequeststruct = 
	{
		TRUE, {(gnWidth>>1)-106, (gnHeight>>1)+188}, {212, 116}, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_PKTRACINGREQUEST
	};
	pPKTracingRequest_Window->Create(pktracingrequeststruct);
	pPKTracingRequest_Window->Initialize();
	pPKTracingRequest_Window->ShowWindow(!TRUE);
	pPKTracingRequest_Window->EnableWindowEffect(!TRUE);
	pPKTracingRequest_Window->SetDocking(TRUE, 1);
	g_MainWindowManager.InsertWindow( pPKTracingRequest_Window);	
#endif

//Author 양희왕 // breif : 용련, 용루 
#ifdef _XDEF_SMELT_ITEM
	XWindow_SmeltItem* pSmletItemWindow = new XWindow_SmeltItem;
	_XWINDOW_STRUCTURE  smletItem = 
	{
		TRUE,
		{ (gnWidth>>1), (gnHeight>>1)-205 }, 
		{ 183, 270 }, 
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_SMELT_ITEM_WINDOW
	};

	pSmletItemWindow->Create( smletItem );
	pSmletItemWindow->Initialize();
	pSmletItemWindow->ShowWindow( FALSE );
	pSmletItemWindow->EnableWindowEffect( FALSE );
	pSmletItemWindow->SetDocking( TRUE, 1 );
	pSmletItemWindow->SetEscapeWindow(TRUE);
	//pSmletItemWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pSmletItemWindow );	
#endif

//Author 양희왕 // breif : 이용등급
#ifdef _XDEF_AGEGRADE_070607_MAGVIPER
	_XWindow_AgeGrade* pAgeGradeWindow = _XWindow_AgeGrade::GetInstance();
	_XWINDOW_STRUCTURE  AgeGrade = 
	{
		TRUE,
		{ (gnWidth - 402), 0}, 
		{ 402, 167 }, 
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
		_XDEF_WTITLE_AGEGRADE_WINDOW
	};

	pAgeGradeWindow->Create( AgeGrade );
	pAgeGradeWindow->Initialize();
	pAgeGradeWindow->ShowWindow( FALSE );
	pAgeGradeWindow->EnableWindowEffect( FALSE );
	pAgeGradeWindow->SetDocking( TRUE, 1 );
	pAgeGradeWindow->SetEnableTopWindow( TRUE );
	//pAgeGradeWindow->SetEscapeWindow(TRUE);
	pAgeGradeWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	//g_MainWindowManager.InsertWindow( pAgeGradeWindow );	
#endif

#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 친구소환
	_XWindow_RecallItem* pRecallItemWindow = new _XWindow_RecallItem;
	_XWINDOW_STRUCTURE Recall = 
	{
		TRUE, 
		{ 0, 0 },
		//{ gnWidth - (1024-841), gnHeight - 329 }, 
		{ 183, 269 }, 
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(200, 0, 0, 0),
		_XDEF_WTITLE_RECALLITEM_WINDOW
	};

	pRecallItemWindow->Create( Recall );
	pRecallItemWindow->Initialize();
	pRecallItemWindow->ShowWindow( FALSE );
	pRecallItemWindow->EnableWindowEffect( FALSE );
	pRecallItemWindow->SetEscapeWindow(TRUE);
	pRecallItemWindow->SetDocking( TRUE, 1 );
	//pRecallItemWindow->SetEnableTopWindow( TRUE );
	pRecallItemWindow->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pRecallItemWindow );	
#endif

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	// Create blacklistwindow=======================================================
	_XWindow_BlackList* pBlackList_Window = new _XWindow_BlackList;
	_XWINDOW_STRUCTURE blacklistwindowstruct = 
	{
		TRUE, { (gnWidth>>1)-(183>>1), (gnHeight>>1)-(270>>1) }, { 183, 270 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_WTITLE_BLACKLISTWINDOW
	};
	pBlackList_Window->Create( blacklistwindowstruct );
	pBlackList_Window->Initialize();	
	pBlackList_Window->SetDocking( TRUE );
	pBlackList_Window->EnableWindowEffect( !TRUE );	
	pBlackList_Window->SetEscapeWindow(TRUE);
	pBlackList_Window->ShowWindow( FALSE );
	pBlackList_Window->SetPlaySoundEfect( TRUE );
	g_MainWindowManager.InsertWindow( pBlackList_Window );	
#endif

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 //breif : 소켓분리
	_XWindow_SocketDetach* pSocketDetach_Window = new _XWindow_SocketDetach;
	_XWINDOW_STRUCTURE socketdetachstruct =
	{
		TRUE, { (gnWidth>>1)-274, (gnHeight>>1)-270 }, { 183, 270 }, 0,
		g_MainInterfaceTextureArchive.FindResource( "socket_Window.tga" ),
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_SOCKETDETACH_WINDOW
	};
	pSocketDetach_Window->Create(socketdetachstruct);
	pSocketDetach_Window->Initialize();
	pSocketDetach_Window->ShowWindow(FALSE);
	pSocketDetach_Window->SetDocking(TRUE, 1);
	pSocketDetach_Window->SetPlaySoundEfect( TRUE );

	g_MainWindowManager.InsertWindow(pSocketDetach_Window);	
#endif
	
//Last Updated : 07/04/16

	// 보스전, 패왕동 중독 아이콘 창===============================================
	_XWindow_Poison* pPoison_Window = new _XWindow_Poison;
	_XWINDOW_STRUCTURE poisonstructure = 
	{
		TRUE, {gnWidth - 155, 214}, {155, 43}, 0,
			-1, 
			&g_MainInterfaceTextureArchive, 
			D3DCOLOR_ARGB(127, 0, 0, 0),
			_XDEF_WTITLE_POISON_WINDOW
	};
	pPoison_Window->Create(poisonstructure);
	pPoison_Window->Initialize();
	pPoison_Window->ShowWindow(!FALSE);
	pPoison_Window->EnableWindowEffect( FALSE );
//	pPoison_Window->SetDocking( TRUE, 1 );
	pPoison_Window->SetEnableTopWindow( TRUE );
	pPoison_Window->SetWindowMoveMode( _XWMOVE_FIXED );

	g_MainWindowManager.InsertWindow(pPoison_Window);

	// 무기 조합 창================================================================
	_XWindow_MixWeapon* pMixWeapon_Window = new _XWindow_MixWeapon;
	_XWINDOW_STRUCTURE  mixweapon = 
	{
		TRUE,
		{ (gnWidth>>1), (gnHeight>>1)-205 }, 
		{ 183, 270 }, 
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ), 
		_XDEF_WTITLE_MIXWEAPON_WINDOW
	};

	pMixWeapon_Window->Create( mixweapon );
	pMixWeapon_Window->Initialize();
	pMixWeapon_Window->ShowWindow( !TRUE );
	pMixWeapon_Window->EnableWindowEffect( FALSE );
	pMixWeapon_Window->SetDocking( TRUE, 1 );
	pMixWeapon_Window->SetEscapeWindow(TRUE);
	g_MainWindowManager.InsertWindow( pMixWeapon_Window );

	// 경험치 윈도우
	// Exp window  ====================================================================
	_XWindow_Exp* pExp_window = new _XWindow_Exp;
	_XWINDOW_STRUCTURE expstruct = 
	{
		TRUE, { 0, gnHeight-47 }, { 383, 47 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
			_XDEF_WTITLE_EXP_WINDOW
	};	

	pExp_window->Create( expstruct );
	pExp_window->Initialize();
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	pExp_window->ShowWindow( TRUE );
#else
	pExp_window->ShowWindow( FALSE );
#endif
	pExp_window->EnableWindowEffect( !TRUE );
	pExp_window->SetDocking( TRUE );
	pExp_window->SetPlaySoundEfect( TRUE );
	pExp_window->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pExp_window );
	
	_XWindow_MatchEvent* pMatchEvent_Window = new _XWindow_MatchEvent;
	_XWINDOW_STRUCTURE fmstruct = 
	{
		TRUE, {(gnWidth>>1)-180, (gnHeight>>1)-311}, {355, 119},
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_MATCHEVENT
	};
	pMatchEvent_Window->Create(fmstruct);
	pMatchEvent_Window->Initialize();
	pMatchEvent_Window->ShowWindow(FALSE);
	pMatchEvent_Window->EnableWindowEffect(!TRUE);
	pMatchEvent_Window->SetDocking(TRUE);
	pMatchEvent_Window->SetPlaySoundEfect(TRUE);
	pMatchEvent_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	g_MainWindowManager.InsertWindow(pMatchEvent_Window);
	
	_XWindow_RewardNick* pRewardNick_Window = new _XWindow_RewardNick;
	_XWINDOW_STRUCTURE rnstruct = 
	{
		TRUE, {0, 0}, {226, 81},
		0,
		-1, 
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_REWARDNICK
	};
	pRewardNick_Window->Create(rnstruct);
	pRewardNick_Window->Initialize();
	pRewardNick_Window->ShowWindow(FALSE);
	pRewardNick_Window->EnableWindowEffect(!TRUE);
	pRewardNick_Window->SetDocking(TRUE);
	pRewardNick_Window->SetPlaySoundEfect(TRUE);
	pRewardNick_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	g_MainWindowManager.InsertWindow(pRewardNick_Window);
	
	_XWindow_Info* pInfo_Window = new _XWindow_Info;
	_XWINDOW_STRUCTURE infostruct =
	{
		TRUE, {gnWidth-204, 10}, {35, 30},
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_INFOWINDOW
	};
	pInfo_Window->Create(infostruct);
	pInfo_Window->Initialize();
	pInfo_Window->ShowWindow(FALSE);
	pInfo_Window->EnableWindowEffect(!TRUE);
	pInfo_Window->SetDocking(FALSE);
	pInfo_Window->SetPlaySoundEfect(TRUE);
	pInfo_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	g_MainWindowManager.InsertWindow(pInfo_Window);

#ifdef _XDEF_NEW_WORLDMAP_070528_MAGVIPER //Author : 양희왕 //breif : 월드맵 윈도우
	XWindow_WorldMap* pWorldMapWindow = new XWindow_WorldMap;

	RECT WorldMapRect;
	int nWorldMapX = 0, nWorldMapY = 0;
	GetWindowRect( gHWnd, &WorldMapRect );
	
	if( WorldMapRect.right - WorldMapRect.left < 0 ||
		WorldMapRect.bottom - WorldMapRect.top < 0 )
	{
		nWorldMapX = 0;
		nWorldMapY = 0;
	}
	else
	{
		nWorldMapX = ((WorldMapRect.right - WorldMapRect.left) - 1024)/2;
		nWorldMapY = ((WorldMapRect.bottom - WorldMapRect.top) - 768)/2;
	}

	pWorldMapWindow->FirstPostion(nWorldMapX, nWorldMapY );
	pWorldMapWindow->SetDrawPostion( nWorldMapX, nWorldMapY );
	
	_XWINDOW_STRUCTURE  WorldMap = 
	{
		TRUE,
		{nWorldMapX, nWorldMapY} ,
		{ 1024, 768 }, 
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
		_XDEF_WTITLE_WORLDMAP
	};

	pWorldMapWindow->Create( WorldMap );
	pWorldMapWindow->Initialize(XmlPaser);
	pWorldMapWindow->ShowWindow( FALSE );
	pWorldMapWindow->EnableWindowEffect( FALSE );
	pWorldMapWindow->SetEscapeWindow(TRUE);
	pWorldMapWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	g_MainWindowManager.InsertWindow( pWorldMapWindow );	
	
	g_pWorldMap_Window			= pWorldMapWindow;
#endif 

	_XWindow_TargetInfoSelect* pTargetInfoSelect = new _XWindow_TargetInfoSelect();
	_XWINDOW_STRUCTURE  TargetInfoSelect = 
	{
		TRUE,
		{ 0, 0}, 
		{ 93, 77}, 
		0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
		_XDEF_WTITLE_TARGETINFOSELECTWINNDOW
	};

	pTargetInfoSelect->Create( TargetInfoSelect );
	pTargetInfoSelect->Initialize();
	pTargetInfoSelect->ShowWindow( FALSE );
	pTargetInfoSelect->EnableWindowEffect( FALSE );
	pTargetInfoSelect->SetEnableTopWindow( TRUE );
	pTargetInfoSelect->SetEscapeWindow(TRUE);
	g_MainWindowManager.InsertWindow( pTargetInfoSelect );

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XWindow_NPCMnDInfo* pNPCMnDInfo_Window = new _XWindow_NPCMnDInfo;
	_XWINDOW_STRUCTURE npcmndinfowindowstruct = 
	{
		TRUE, { 410, (gnHeight>>1)-205 }, { 366, 270 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 64, 0, 0, 0 ), 
			_XDEF_WTITLE_NPCMNDWINDOW
	};
	pNPCMnDInfo_Window->Create( npcmndinfowindowstruct );
	pNPCMnDInfo_Window->Initialize();
	pNPCMnDInfo_Window->ShowWindow( FALSE );
	pNPCMnDInfo_Window->EnableWindowEffect( FALSE );
	pNPCMnDInfo_Window->SetDocking( TRUE, 1 );
	g_MainWindowManager.InsertWindow( pNPCMnDInfo_Window );
#endif

	// 항상 제일 아래 쪽에 있어야 한다. 그림이 화면위에 덮어야 하기 때문이다.
// Create HelpScreen Window=======================================================
	_XWindow_HelpScreen* pHelpScreen_Window = new _XWindow_HelpScreen;
	_XWINDOW_STRUCTURE hsstruct = 
	{
		TRUE, {0, 0}, {10, 10}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_HELPSCREENWINDOW
	};
	pHelpScreen_Window->Create(hsstruct);
	pHelpScreen_Window->Initialize();
	pHelpScreen_Window->ShowWindow(FALSE);
	pHelpScreen_Window->EnableWindowEffect(FALSE);
	g_MainWindowManager.InsertWindow(pHelpScreen_Window);
	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_STATUS, _XGETINTERFACETEXT(ID_STRING_NEW_2669), _XGETINTERFACETEXT(ID_STRING_NEW_2678) );
	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MINIMAP, _XGETINTERFACETEXT(ID_STRING_NEW_2670), _XGETINTERFACETEXT(ID_STRING_NEW_2679) );
	
#ifdef _XDEF_KEYCHANGE_20070115
	if( g_bGameOptionKeyTypeASelected )
#endif
	{
		pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_QUICKSLOT, _XGETINTERFACETEXT(ID_STRING_NEW_2671), _XGETINTERFACETEXT(ID_STRING_NEW_2684) );	
		pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MESSENGER, _XGETINTERFACETEXT(ID_STRING_NEW_2673), _XGETINTERFACETEXT(ID_STRING_NEW_2686) );
		pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MAIL, _XGETINTERFACETEXT(ID_STRING_NEW_2674), _XGETINTERFACETEXT(ID_STRING_NEW_2687) );

	}
#ifdef _XDEF_KEYCHANGE_20070115
	else
	{
		pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_QUICKSLOT, _XGETINTERFACETEXT(ID_STRING_NEW_3239), _XGETINTERFACETEXT(ID_STRING_NEW_2684) );	
		pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MESSENGER, _XGETINTERFACETEXT(ID_STRING_NEW_3240), _XGETINTERFACETEXT(ID_STRING_NEW_2686) );
		pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MAIL, _XGETINTERFACETEXT(ID_STRING_NEW_3241), _XGETINTERFACETEXT(ID_STRING_NEW_2687) );
	}
#endif

	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MEDETATIOH, _XGETINTERFACETEXT(ID_STRING_NEW_2672), _XGETINTERFACETEXT(ID_STRING_NEW_2685) );
	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_CINFO, _XGETINTERFACETEXT(ID_STRING_NEW_2675), _XGETINTERFACETEXT(ID_STRING_NEW_2688) );
	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_INVEN, _XGETINTERFACETEXT(ID_STRING_NEW_2676), _XGETINTERFACETEXT(ID_STRING_NEW_2689) );
	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_OPTION, _XGETINTERFACETEXT(ID_STRING_NEW_2677), _XGETINTERFACETEXT(ID_STRING_NEW_2690) );
#ifdef _XTS_ITEMMALLBROWSER	// 아이템몰
	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_ITEMMALL, _XGETINTERFACETEXT(ID_STRING_NEW_2977), _XGETINTERFACETEXT(ID_STRING_NEW_2979) );
	pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_ITEMBOX, _XGETINTERFACETEXT(ID_STRING_NEW_2978), _XGETINTERFACETEXT(ID_STRING_NEW_2980) );
#endif

	//2004.06.18->oneway48 insert	
	int gaugeimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_party.tga" );
				
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 600, 350 }, { 91, 5 },	
			gaugeimageindex, -1, &g_MainInterfaceTextureArchive
	};
	
	g_pLocalUser->m_MiniHpBorder.Create( gaugestruct );
	g_pLocalUser->m_MiniHpBorder.SetImageClipRect( _XGAUGEIMAGE_BORDER, 157, 70, 248, 75 );
	g_pLocalUser->m_MiniHpBorder.SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 157, 70, 248, 75 );
	g_pLocalUser->m_MiniHpBorder.SetBackGroundDraw(FALSE);
	g_pLocalUser->m_MiniHpBorder.SetViewDelayGauge( TRUE, 20 );
	g_pLocalUser->m_MiniHpBorder.SetFactorDraw(FALSE);
	
	g_pLocalUser->m_MonsterMiniHpBorder.Create( gaugestruct );
	g_pLocalUser->m_MonsterMiniHpBorder.SetImageClipRect( _XGAUGEIMAGE_BORDER, 157, 63, 248, 68 );
	g_pLocalUser->m_MonsterMiniHpBorder.SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 157, 63, 248, 68 );
	g_pLocalUser->m_MonsterMiniHpBorder.SetBackGroundDraw(FALSE);
	g_pLocalUser->m_MonsterMiniHpBorder.SetViewDelayGauge( TRUE, 20 );
	g_pLocalUser->m_MonsterMiniHpBorder.SetFactorDraw(FALSE);
	
	g_pLocalUser->m_imageMiniHpBackBorder.Create( 0, 0, 108, 9,
			&g_MainInterfaceTextureArchive, gaugeimageindex );	
	g_pLocalUser->m_imageMiniHpBackBorder.SetClipRect( 148, 77, 256, 86  );
	g_pLocalUser->m_imageMiniHpBackBorder.ShowWindow(TRUE);



	// 윈도우들간의 도킹 상태를 체크
	g_MainWindowManager.RebuildDockedWindow();	

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
	pPartyMasterMinibar_Window->MoveWindow( 50, 114);
#else
	pPartyMasterMinibar_Window->MoveWindow( 14, 114 );
#endif

	int ypos = 0;
	for( i = 1; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
	{				
		_XWindow_PartyMemberMiniBar* pPartyMemberMinibar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1 + i);

	#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
		pPartyMemberMinibar_Window->MoveWindow( 50, 170 + ypos); 
	#else
		pPartyMasterMinibar_Window->MoveWindow( 14, 170 + ypos);
	#endif

		ypos += 36;
	}
	
	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.LoadInformationString();

	return TRUE;
}

bool XProc_FirstLoad::Initialize3DSound( void )
{
	// Read registry setting  sound section 
	g_BgmPlayFlag				= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, TRUE, TRUE );
	g_InterfaceSoundEffectFlag	= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_INTERFACESOUNDENABLE, TRUE, TRUE );
	g_SoundEffectFlag			= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDEFFECTENABLE, TRUE, TRUE );
	g_3DSoundEffectFlag			= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_3DSOUNDENABLE,  TRUE, TRUE );


#ifdef _XUSEFMOD
	if( !g_FMODWrapper.Initialize() ) return false;

	_XMEMORYUSECHECKREADY
	_XMEMORYUSECHECKSTART
	if( !PreLoadSoundEffect() ) return false;
	_XMEMORYUSECHECKEND( "Load sound asset" );
#else
	_XMEMORYUSECHECKREADY
	_XMEMORYUSECHECKSTART
	if( !g_XAudioSystem.Initialize() )
	{
		_XFatalError("Couldn't setup _XAudioSystem");
		return false;
	}	
	_XMEMORYUSECHECKEND( "Initialize 3D Sound" );
				
	_XMEMORYUSECHECKSTART
	if( !PreLoadSoundEffect() ) return false;
	_XMEMORYUSECHECKEND( "Load sound asset" );

	AudioMgr()->SetMusicVolume(1.0f);
	AudioMgr()->SetSoundVolume(1.0f);
	
	g_XAudioSystem.m_pListener->SetDistanceFactor( 0.1f );	
	g_XAudioSystem.m_pListener->CommitDeferredSettings();
#endif
	
	return true;
}

void XProc_FirstLoad::Relase3DSound( void )
{	
#ifdef _XUSEFMOD
	g_FMODWrapper.Release();
#else
	ReleaseSoundEffect();	
	g_XAudioSystem.Release();
#endif
}

#ifdef _XUSEFMOD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FMOD Sound system
BOOL XProc_FirstLoad::PreLoadSoundEffect( void )
{		
	_XLog( "Load sound object" );
	
	ID_SR_INTERFACE_DROP_METAL_003_WAV				= g_FMODWrapper.InsertSound( _T("DROP_METAL_003.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_METAL_002_WAV				= g_FMODWrapper.InsertSound( _T("DROP_METAL_002.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_METAL_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_METAL_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_LIGHT_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_LIGHT_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_HOOP_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_HOOP_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_GLASS_DROP_001_WAV			= g_FMODWrapper.InsertSound( _T("DROP_GLASS_DROP_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_CLOTH_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_CLOTH_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_CHOP_002_WAV				= g_FMODWrapper.InsertSound( _T("DROP_CHOP_002.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_CHOP_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_CHOP_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_COOLDOWN_WAV					= g_FMODWrapper.InsertSound( _T("COOLDOWN.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_CLOTHESDROP01_WAV				= g_FMODWrapper.InsertSound( _T("CLOTHESDROP01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_CHOICEICON01_WAV				= g_FMODWrapper.InsertSound( _T("CHOICEICON01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_CHARACTER_START_WAV				= g_FMODWrapper.InsertSound( _T("CHARACTER_START.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_BURNANGER01_WAV					= g_FMODWrapper.InsertSound( _T("BURNANGER01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_BIGBUTTONCLICK01_WAV			= g_FMODWrapper.InsertSound( _T("BIGBUTTONCLICK01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_ANGERFIRE01_WAV					= g_FMODWrapper.InsertSound( _T("ANGERFIRE01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_WOODDROP01_WAV					= g_FMODWrapper.InsertSound( _T("WOODDROP01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_WINDOWOPEN01_WAV				= g_FMODWrapper.InsertSound( _T("WINDOWOPEN01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_WINDOWCLOSE01_WAV				= g_FMODWrapper.InsertSound( _T("WINDOWCLOSE01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_WARNING01_WAV					= g_FMODWrapper.InsertSound( _T("WARNING01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_USE_FIRE_WAV					= g_FMODWrapper.InsertSound( _T("USE_FIRE.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_USE_EAT_004_WAV					= g_FMODWrapper.InsertSound( _T("USE_EAT_004.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_USE_EAT_003_WAV					= g_FMODWrapper.InsertSound( _T("USE_EAT_003.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_USE_EAT_002_WAV					= g_FMODWrapper.InsertSound( _T("USE_EAT_002.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_USE_EAT_001_WAV					= g_FMODWrapper.InsertSound( _T("USE_EAT_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_USE_BOOK_001_WAV				= g_FMODWrapper.InsertSound( _T("USE_BOOK_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_TUTO_01_WAV						= g_FMODWrapper.InsertSound( _T("TUTO_01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_TURN_POINT00_WAV				= g_FMODWrapper.InsertSound( _T("TURN_POINT00.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_TARGETING00_WAV					= g_FMODWrapper.InsertSound( _T("TARGETING00.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_STARTGAME_WAV					= g_FMODWrapper.InsertSound( _T("STARTGAME.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_SMALLBUTTONCLICK_WAV			= g_FMODWrapper.InsertSound( _T("SMALLBUTTONCLICK.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_SLIDEBUTTON01_WAV				= g_FMODWrapper.InsertSound( _T("SLIDEBUTTON01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_SKILLBOOKDROP01_WAV				= g_FMODWrapper.InsertSound( _T("SKILLBOOKDROP01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_SKILL_DOWN_WAV					= g_FMODWrapper.InsertSound( _T("SKILL_DOWN.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_SKILL_CLICK_WAV					= g_FMODWrapper.InsertSound( _T("SKILL_CLICK.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_REBORN_WAV						= g_FMODWrapper.InsertSound( _T("REBORN.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_REBIRTH01_WAV					= g_FMODWrapper.InsertSound( _T("REBIRTH01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_QUICKBUTTONCLICK01_WAV			= g_FMODWrapper.InsertSound( _T("QUICKBUTTONCLICK01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_QUESTSUCCESS01_WAV				= g_FMODWrapper.InsertSound( _T("QUESTSUCCESS01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_QUESTFIND01_WAV					= g_FMODWrapper.InsertSound( _T("QUESTFIND01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_QUESTFAIL01_WAV					= g_FMODWrapper.InsertSound( _T("QUESTFAIL01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_PICK_ITEM_001_WAV				= g_FMODWrapper.InsertSound( _T("PICK_ITEM_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_NORMALSTART01_WAV				= g_FMODWrapper.InsertSound( _T("NORMALSTART01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_MONEY_01_WAV					= g_FMODWrapper.InsertSound( _T("MONEY_01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_LEVELUP01_WAV					= g_FMODWrapper.InsertSound( _T("LEVELUP01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_LEVELUP_INTRO_WAV				= g_FMODWrapper.InsertSound( _T("LEVELUP_INTRO.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_LEVELFAIL_WAV					= g_FMODWrapper.InsertSound( _T("LEVELFAIL.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_LEARNSKILL01_WAV				= g_FMODWrapper.InsertSound( _T("LEARNSKILL01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_ERROR01_WAV						= g_FMODWrapper.InsertSound( _T("ERROR01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROPICON01_WAV					= g_FMODWrapper.InsertSound( _T("DROPICON01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_WOOD_002_WAV				= g_FMODWrapper.InsertSound( _T("DROP_WOOD_002.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_WOOD_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_WOOD_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_WHEEP_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_WHEEP_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_SHINE_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_SHINE_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_DROP_PAPER_001_WAV				= g_FMODWrapper.InsertSound( _T("DROP_PAPER_001.WAV"), FSOUND_HW2D );
	
	ID_SR_INTERFACE_TRA_COMBO01_WAV					= g_FMODWrapper.InsertSound( _T("TRA_COMBO01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_MISS_01_WAV						= g_FMODWrapper.InsertSound( _T("MISS_01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_KIPOINT_01_WAV					= g_FMODWrapper.InsertSound( _T("KIPOINT_01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_BUTTON_WAV						= g_FMODWrapper.InsertSound( _T("BUTTON.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_BLUE_01_WAV						= g_FMODWrapper.InsertSound( _T("BLUE_01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_WHITE_01_WAV					= g_FMODWrapper.InsertSound( _T("WHITE_01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_BLACK_01_WAV					= g_FMODWrapper.InsertSound( _T("BLACK_01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_TRA_UP01_WAV					= g_FMODWrapper.InsertSound( _T("TRA_UP01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_TRA_OK01_WAV					= g_FMODWrapper.InsertSound( _T("TRA_OK01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_TRA_MISS01_WAV					= g_FMODWrapper.InsertSound( _T("TRA_MISS01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_QUICK_USE_WAV					= g_FMODWrapper.InsertSound( _T("QUICK_USE.WAV"), FSOUND_HW2D );

	ID_SR_INTERFACE_BOSSBUTTON00_WAV				= g_FMODWrapper.InsertSound( _T("BOSSINTROBUTTON00.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_BOSSBUTTON01_WAV				= g_FMODWrapper.InsertSound( _T("BOSSINTROBUTTON01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_EARTHQUAKE_WAV					= g_FMODWrapper.InsertSound( _T("QUAKE01.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_EARTHQUAKE01_WAV				= g_FMODWrapper.InsertSound( _T("QUAKE00.WAV"), FSOUND_HW2D );
	
	ID_SR_INTERFACE_USE_ELIXIR01_WAV				= g_FMODWrapper.InsertSound( _T("Use_Elx_001.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_ELIXIR_RUN_WAV					= g_FMODWrapper.InsertSound( _T("Elixir_run.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_ELIXIR_YES_WAV					= g_FMODWrapper.InsertSound( _T("Elixir_yes.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_ELIXIR_NO_WAV					= g_FMODWrapper.InsertSound( _T("Elixir_no.WAV"), FSOUND_HW2D );
	ID_SR_INTERFACE_OPEN_BOX_WAV					= g_FMODWrapper.InsertSound( _T("open_box_001.WAV"), FSOUND_HW2D );

	ID_SR_INTERFACE_INCHANT_01_WAV					= g_FMODWrapper.InsertSound( _T("INCHANT_BAR_01.wav"), FSOUND_HW2D );
	ID_SR_INTERFACE_INCHANT_02_WAV					= g_FMODWrapper.InsertSound( _T("INCHANT_BAR_02.wav"), FSOUND_HW2D );
	ID_SR_INTERFACE_INCHANT_03_WAV					= g_FMODWrapper.InsertSound( _T("INCHANT_BAR_03.wav"), FSOUND_HW2D );
	ID_SR_INTERFACE_MASK_ON							= g_FMODWrapper.InsertSound( _T("mask_on.wav"), FSOUND_HW2D );
	ID_SR_INTERFACE_MASK_OFF						= g_FMODWrapper.InsertSound( _T("mask_off.wav"), FSOUND_HW2D );
#ifdef _XDEF_CHRISTMASEVENT
	ID_SR_INTERFACE_CHRISTMAS_WAV					= g_FMODWrapper.InsertSound( _T("xmas.wav"), FSOUND_HW2D );
#endif

#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
	ID_SR_INTERFACE_RECALL_WAV						= g_FMODWrapper.InsertSound( _T("warp_call.wav"), FSOUND_HW2D );
	ID_SR_INTERFACE_RECALLEFFECT_WAV				= g_FMODWrapper.InsertSound( _T("warp_beam.wav"), FSOUND_HW2D );
#endif

	g_PVPBattleBGMStartIndex						= g_FMODWrapper.InsertSound( _T("DUEL_BGM_START_00.wav"), FSOUND_HW2D );
	g_PVPBattleBGMEndIndex							= g_FMODWrapper.InsertSound( _T("DUEL_BGM_ENDING_00.wav"), FSOUND_HW2D );
	g_PVPBattleFireworkSoundIndex					= g_FMODWrapper.InsertSound( _T("DUEL_SE_WIN_01.wav"), FSOUND_HW2D );
	
	g_BloodInSoundIndex								= g_FMODWrapper.InsertSound( _T("BLOODPOINT_IN.wav"), FSOUND_HW2D );
	g_BloodOutSoundIndex							= g_FMODWrapper.InsertSound( _T("BLOODPOINT_OUT.wav"), FSOUND_HW2D );

	g_FMODWrapper.InsertSound( _T("DUEL_BGM_MAIN_00.wav"), FSOUND_HW2D /*| FSOUND_LOOP_NORMAL*/ );

	g_ClickSoundIndex		=	ID_SR_INTERFACE_SMALLBUTTONCLICK_WAV;
	g_WindowOpenSoundIndex	=	ID_SR_INTERFACE_WINDOWOPEN01_WAV;
	g_WindowCloseSoundIndex	=	ID_SR_INTERFACE_WINDOWCLOSE01_WAV;



	/*
	ID_SR_INTERFACE_INTERFACE_DROPICON01_WAV			= g_FMODWrapper.InsertSound( "DROPICON01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_LEARNSKILL01_WAV			= g_FMODWrapper.InsertSound( "LEARNSKILL01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_PICK_ITEM_001_WAV			= g_FMODWrapper.InsertSound( "PICK_ITEM_001.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_LEVELUP_INTRO_WAV			= g_FMODWrapper.InsertSound( "LEVELUP_INTRO.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_HOOP_001_WAV			= g_FMODWrapper.InsertSound( "DROP_HOOP_001.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_SKILLBOOKDROP01_WAV		= g_FMODWrapper.InsertSound( "SKILLBOOKDROP01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_SLIDEBUTTON01_WAV			= g_FMODWrapper.InsertSound( "SLIDEBUTTON01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_SMALLBUTTONCLICK_WAV		= g_FMODWrapper.InsertSound( "SMALLBUTTONCLICK.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_TARGETING00_WAV			= g_FMODWrapper.InsertSound( "TARGETING00.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_WINDOWCLOSE01_WAV			= g_FMODWrapper.InsertSound( "WINDOWCLOSE01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_CLOTH_001_WAV		= g_FMODWrapper.InsertSound( "DROP_CLOTH_001.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_WOOD_001_WAV			= g_FMODWrapper.InsertSound( "DROP_WOOD_001.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_PICKITEM01_WAV			= g_FMODWrapper.InsertSound( "PICKITEM01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_ERROR01_WAV				= g_FMODWrapper.InsertSound( "ERROR01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_PARAMETERUP01_WAV			= g_FMODWrapper.InsertSound( "PARAMETERUP01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_BURNANGER01_WAV			= g_FMODWrapper.InsertSound( "BURNANGER01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_METAL_002_WAV		= g_FMODWrapper.InsertSound( "DROP_METAL_002.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_WOOD_002_WAV			= g_FMODWrapper.InsertSound( "DROP_WOOD_002.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_SHINE_001_WAV		= g_FMODWrapper.InsertSound( "DROP_SHINE_001.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_003_WAV			= g_FMODWrapper.InsertSound( "USE_EAT_003.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_001_WAV			= g_FMODWrapper.InsertSound( "USE_EAT_001.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_LIKE_DEATH_WAV			= g_FMODWrapper.InsertSound( "LIKE_DEATH.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_CHOP_001_WAV			= g_FMODWrapper.InsertSound( "DROP_CHOP_001.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_ANGERFIRE01_WAV			= g_FMODWrapper.InsertSound( "ANGERFIRE01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_002_WAV			= g_FMODWrapper.InsertSound( "USE_EAT_002.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_LEVELFAIL_WAV				= g_FMODWrapper.InsertSound( "LEVELFAIL.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_QUESTSUCCESS02_WAV		= g_FMODWrapper.InsertSound( "QUESTSUCCESS02.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_QUESTFIND02_WAV			= g_FMODWrapper.InsertSound( "QUESTFIND02.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_NORMALSTART01_WAV			= g_FMODWrapper.InsertSound( "NORMALSTART01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_LEVELUP01_WAV				= g_FMODWrapper.InsertSound( "LEVELUP01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_WINDOWOPEN01_WAV			= g_FMODWrapper.InsertSound( "WINDOWOPEN01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_WARNING01_WAV				= g_FMODWrapper.InsertSound( "WARNING01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_QUICKBUTTONCLICK01_WAV	= g_FMODWrapper.InsertSound( "QUICKBUTTONCLICK01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_CLOTHESDROP01_WAV			= g_FMODWrapper.InsertSound( "CLOTHESDROP01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_WOODDROP01_WAV			= g_FMODWrapper.InsertSound( "WOODDROP01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_QUESTFAIL01_WAV			= g_FMODWrapper.InsertSound( "QUESTFAIL01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_QUESTSUCCESS01_WAV		= g_FMODWrapper.InsertSound( "QUESTSUCCESS01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_QUESTFIND01_WAV			= g_FMODWrapper.InsertSound( "QUESTFIND01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_METAL_001_WAV		= g_FMODWrapper.InsertSound( "DROP_METAL_001.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_CHOP_002_WAV			= g_FMODWrapper.InsertSound( "DROP_CHOP_002.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_004_WAV			= g_FMODWrapper.InsertSound( "USE_EAT_004.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_BIGBUTTONCLICK01_WAV		= g_FMODWrapper.InsertSound( "BIGBUTTONCLICK01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_CHOICEICON01_WAV			= g_FMODWrapper.InsertSound( "CHOICEICON01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_LIGHT_001_WAV		= g_FMODWrapper.InsertSound( "DROP_LIGHT_001.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_GLASS_DROP_001_WAV	= g_FMODWrapper.InsertSound( "DROP_GLASS_DROP_001.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_DROP_WHEEP_001_WAV		= g_FMODWrapper.InsertSound( "DROP_WHEEP_001.WAV" , FSOUND_HW2D );

	ID_SR_INTERFACE_INTERFACE_END_WAV					= g_FMODWrapper.InsertSound( "END.WAV" , FSOUND_HW2D );   
	ID_SR_INTERFACE_INTERFACE_START_WAV					= g_FMODWrapper.InsertSound( "START.WAV" , FSOUND_HW2D ); 
	ID_SR_INTERFACE_INTERFACE_EVENT_WAV					= g_FMODWrapper.InsertSound( "EVENT.WAV" , FSOUND_HW2D ); 
	ID_SR_INTERFACE_INTERFACE_UP_WAV					= g_FMODWrapper.InsertSound( "UP.WAV" , FSOUND_HW2D );    
	ID_SR_INTERFACE_INTERFACE_CLOSE_WAV					= g_FMODWrapper.InsertSound( "CLOSE.WAV" , FSOUND_HW2D ); 
	ID_SR_INTERFACE_INTERFACE_GRANT1_WAV				= g_FMODWrapper.InsertSound( "GRANT1.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_GRANT2_WAV				= g_FMODWrapper.InsertSound( "GRANT2.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_READY_WAV					= g_FMODWrapper.InsertSound( "READY.WAV" , FSOUND_HW2D ); 
	ID_SR_INTERFACE_INTERFACE_NO_WAV					= g_FMODWrapper.InsertSound( "NO.WAV" , FSOUND_HW2D );    
	ID_SR_INTERFACE_INTERFACE_YES1_WAV					= g_FMODWrapper.InsertSound( "YES1.WAV" , FSOUND_HW2D );  
	ID_SR_INTERFACE_INTERFACE_YES2_WAV					= g_FMODWrapper.InsertSound( "YES2.WAV" , FSOUND_HW2D ); 	
	ID_SR_INTERFACE_INTERFACE_BUTTON_WAV				= g_FMODWrapper.InsertSound( "BUTTON.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_OPEN_WAV					= g_FMODWrapper.InsertSound( "OPEN.WAV" , FSOUND_HW2D );  

	ID_SR_INTERFACE_INTERFACE_BLUE_01_WAV				= g_FMODWrapper.InsertSound( "BLUE_01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_MISS_01_WAV				= g_FMODWrapper.InsertSound( "MISS_01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_BLACK_01_WAV				= g_FMODWrapper.InsertSound( "BLACK_01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_WHITE_01_WAV				= g_FMODWrapper.InsertSound( "WHITE_01.WAV" , FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_KIPOINT_01_WAV			= g_FMODWrapper.InsertSound( "KIPOINT_01.WAV" , FSOUND_HW2D );
	
	ID_SR_INTERFACE_INTERFACE_SKILL_CLICK_WAV			= g_FMODWrapper.InsertSound( "SKILL_CLICK.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_SKILL_DOWN_WAV			= g_FMODWrapper.InsertSound( "SKILL_DOWN.WAV", FSOUND_HW2D );

	ID_SR_INTERFACE_INTERFACE_TRA_COMBO_01_WAV			= g_FMODWrapper.InsertSound( "TRA_combo01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_TRA_MISS_01_WAV			= g_FMODWrapper.InsertSound( "TRA_miss01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_TRA_OK_01_WAV				= g_FMODWrapper.InsertSound( "TRA_ok01.WAV", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_TRA_UP_01_WAV				= g_FMODWrapper.InsertSound( "TRA_up01.WAV", FSOUND_HW2D );
	
	ID_SR_INTERFACE_INTERFACE_STARTGAME_WAV				= g_FMODWrapper.InsertSound( "StartGame.wav", FSOUND_HW2D );
	ID_SR_INTERFACE_INTERFACE_COOLDOWN_WAV				= g_FMODWrapper.InsertSound( "COOLDOWN.WAV", FSOUND_HW2D );
	*/

	// Load item use sound list
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* pItemUseSoundScript = NULL;

	pItemUseSoundScript = g_ScriptArchive.GetPackedFile( _T("ItemSoundList.ini") );

	if( !pItemUseSoundScript ) return FALSE;

	BOOL bReadCount = FALSE;
	
	TCHAR buffer[128];
	TCHAR soundnamebuffer[128];
	memset( buffer, 0, sizeof(TCHAR) * 128 );
/*	if( !fgets( buffer, 127, pItemUseSoundScript ) )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	g_ItemUseSoundIndexTableSize = atoi( buffer ) + 1;
	if( g_ItemUseSoundIndexTableSize > 0 )
	{
		SAFE_DELETE_ARRAY( g_ItemUseSoundIndexTable );
		g_ItemUseSoundIndexTable = new int[g_ItemUseSoundIndexTableSize];
	}

	memset( g_ItemUseSoundIndexTable, 0, sizeof(int) * g_ItemUseSoundIndexTableSize );
*/
	do 
	{
		if( feof(pItemUseSoundScript) ) break;
		
		memset( buffer, 0, sizeof(TCHAR)*128);
		if( !fgets( buffer, 127, pItemUseSoundScript ) )
		{
			break;
		}
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		
		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			if(!bReadCount)
			{
				g_ItemUseSoundIndexTableSize = atoi( buffer ) + 1;
				if( g_ItemUseSoundIndexTableSize > 0 )
				{
					SAFE_DELETE_ARRAY( g_ItemUseSoundIndexTable );
					g_ItemUseSoundIndexTable = new int[g_ItemUseSoundIndexTableSize];
				}

				memset( g_ItemUseSoundIndexTable, 0, sizeof(int) * g_ItemUseSoundIndexTableSize );
				bReadCount = TRUE;
			}
			else
			{
				int insertpos = 0;
				sscanf( buffer, "%d %s", &insertpos, soundnamebuffer );
				
				if( insertpos < g_ItemUseSoundIndexTableSize )
				{
					g_ItemUseSoundIndexTable[insertpos] = g_FMODWrapper.InsertSound( soundnamebuffer, FSOUND_HW2D );
					
					if( g_ItemUseSoundIndexTable[insertpos] < 0 )
					{
						_XFatalError( "Can't found interface sound file [%s]", soundnamebuffer );
						break;
					}
				}
				else
				{
					_XFatalError( "Interface sound : overflowed buffer size [ip:%d/max:%d]", insertpos, g_ItemUseSoundIndexTableSize );
				}
			}
		}
		
	} while( !feof(pItemUseSoundScript) );
		
	// monster transform
	ID_SR_INTERFACE_TURN_POINT00_WAV			= g_FMODWrapper.InsertSound( "TURN_POINT00.WAV" , FSOUND_HW3D );

	// Load 3D effect sound --------------------------------------------------------------------------------------------------------------------				

	FILE* pWeaponSoundScript = NULL;

	pWeaponSoundScript = g_ScriptArchive.GetPackedFile( _T("WeaponSound.ini") );

	if( !pWeaponSoundScript ) return FALSE;
		
	memset( buffer, 0, sizeof(TCHAR) * 128 );
	if( !fgets( buffer, 127, pWeaponSoundScript ) )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	g_WeaponSoundCount = atoi( buffer ) + 1;
	if( g_WeaponSoundCount > 0 )
	{
		SAFE_DELETE_ARRAY( g_WeaponSoundIndexList );
		g_WeaponSoundIndexList = new int[g_WeaponSoundCount];
		memset( g_WeaponSoundIndexList, 0, sizeof(int) * g_WeaponSoundCount );
	}
	
	int weaponsoundcounter = 1;
	
	do 
	{
		if( feof(pWeaponSoundScript) ) break;
		
		memset( buffer, 0, sizeof(TCHAR)*128);
		if( !fgets( buffer, 127, pWeaponSoundScript ) )
		{
			break;
		}
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		
		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			sscanf( buffer, "%s", soundnamebuffer );

			if( weaponsoundcounter < g_WeaponSoundCount )
			{
				g_WeaponSoundIndexList[weaponsoundcounter] = g_FMODWrapper.InsertSound( soundnamebuffer, FSOUND_HW3D );
				
				if( g_WeaponSoundIndexList[weaponsoundcounter] < 0 )
				{
					_XFatalError( "Can't found Weapon sound file [%s]", soundnamebuffer );
					break;
				}
			}
			else
			{
				_XFatalError( "Weapon sound : overflowed  buffer size [ip:%d/max:%d]", weaponsoundcounter, g_WeaponSoundCount );
				break;
			}
		}
		
		weaponsoundcounter++;
		
	} while( !feof(pWeaponSoundScript) );

	FILE* pMobWeaponSoundScript = NULL;

	pMobWeaponSoundScript = g_ScriptArchive.GetPackedFile( _T("MobAttackSound.ini") );
	
	if( !pMobWeaponSoundScript ) return FALSE;
	
	memset( buffer, 0, sizeof(TCHAR) * 128 );
	if( !fgets( buffer, 127, pMobWeaponSoundScript ) )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	g_MobWeaponSoundCount = atoi( buffer ) + 1;
	if( g_MobWeaponSoundCount > 0 )
	{
		SAFE_DELETE_ARRAY( g_MobWeaponSoundIndexList );
		g_MobWeaponSoundIndexList = new int[g_MobWeaponSoundCount];
		memset( g_MobWeaponSoundIndexList, 0, sizeof(int) * g_MobWeaponSoundCount );
	}
	
	weaponsoundcounter = 1;
	
	do 
	{
		if( feof(pMobWeaponSoundScript) ) break;
		
		memset( buffer, 0, sizeof(TCHAR)*128);
		if( !fgets( buffer, 127, pMobWeaponSoundScript ) )
		{
			break;
		}
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		
		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			sscanf( buffer, "%s", soundnamebuffer );
			
			if( weaponsoundcounter < g_MobWeaponSoundCount )
			{
				g_MobWeaponSoundIndexList[weaponsoundcounter] = g_FMODWrapper.InsertSound( soundnamebuffer, FSOUND_HW3D );
				
				if( g_MobWeaponSoundIndexList[weaponsoundcounter] < 0 )
				{
					_XFatalError( "Can't found Mob weapon sound file [%s]", soundnamebuffer );
					break;
				}
			}
			else
			{
				_XFatalError( "Mob weapon sound : overflowed buffer size [ip:%d/max:%d]", weaponsoundcounter, g_MobWeaponSoundCount );
				break;
			}
		}
		
		weaponsoundcounter++;
		
	} while( !feof(pMobWeaponSoundScript) );

	g_ScriptArchive.ClosePackage();

	g_DefenceEffectSoundIndex[0][0] = g_FMODWrapper.InsertSound( "MCh01_Shout01.wav", FSOUND_HW3D );	// 남자 기합
	g_DefenceEffectSoundIndex[0][1] = g_FMODWrapper.InsertSound( "MCh01_Shout02.wav", FSOUND_HW3D );
	g_DefenceEffectSoundIndex[0][2] = g_FMODWrapper.InsertSound( "MCh01_Shout03.wav", FSOUND_HW3D );

	g_DefenceEffectSoundIndex[1][0] = g_FMODWrapper.InsertSound( "WCh03_Shout01.wav", FSOUND_HW3D );  // 여자 기합
	g_DefenceEffectSoundIndex[1][1] = g_FMODWrapper.InsertSound( "WCh03_Shout02.wav", FSOUND_HW3D );
	g_DefenceEffectSoundIndex[1][2] = g_FMODWrapper.InsertSound( "WCh03_Shout03.wav", FSOUND_HW3D );

	g_BlowEffectSoundIndex[0][0] = g_FMODWrapper.InsertSound( "MCh01_Scream01.wav", FSOUND_HW3D );	// 남자 맞기 
	g_BlowEffectSoundIndex[0][1] = g_FMODWrapper.InsertSound( "MCh01_Scream02.wav", FSOUND_HW3D );
	g_BlowEffectSoundIndex[0][2] = g_FMODWrapper.InsertSound( "MCh01_Scream03.wav", FSOUND_HW3D );

	g_BlowEffectSoundIndex[1][0] = g_FMODWrapper.InsertSound( "WCh03_Scream01.wav", FSOUND_HW3D ); // 여자 맞기 
	g_BlowEffectSoundIndex[1][1] = g_FMODWrapper.InsertSound( "WCh03_Scream02.wav", FSOUND_HW3D );
	g_BlowEffectSoundIndex[1][2] = g_FMODWrapper.InsertSound( "WCh03_Scream03.wav", FSOUND_HW3D );
	

	g_DieEffectSoundIndex[0][0] = g_FMODWrapper.InsertSound( "MCh01_Die01.wav", FSOUND_HW3D ); // 남자 죽기
	g_DieEffectSoundIndex[0][1] = g_FMODWrapper.InsertSound( "MCh01_Die02.wav", FSOUND_HW3D );	

	g_DieEffectSoundIndex[1][0] = g_FMODWrapper.InsertSound( "WCh03_Die01.wav", FSOUND_HW3D ); // 여자 죽기
	g_DieEffectSoundIndex[1][1] = g_FMODWrapper.InsertSound( "WCh03_Die02.wav", FSOUND_HW3D );

	// Critical sound
	g_CriticalEffectSoundIndex[0] = g_FMODWrapper.InsertSound( "cri_fist.wav", FSOUND_HW3D );
	g_CriticalEffectSoundIndex[1] = g_FMODWrapper.InsertSound( "cri_Metal_Heavy.wav", FSOUND_HW3D );
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XAudioSystem

#else	

BOOL XProc_FirstLoad::PreLoadSoundEffect( void )
{		
	_XLog( "Load sound object" );
		
	_XSetDefaultPath(_XDEF_DEFAULTPATH);

	SoundInit init2dsound;
	init2dsound.m_bLooping = false;
	init2dsound.m_bStreaming = false;
	init2dsound.m_bMusic = false;	
	init2dsound.m_pPackage = &g_InterfaceSoundArchive;
	
	// Load interface sound ---------------------------------------------------

	if( !g_InterfaceSoundArchive.Open( _XDEF_INTERFACESOUNDARCHIVENAME ) )
	{
		return FALSE;
	}

	g_MaxInterfaceSoundEffectCount = g_InterfaceSoundArchive.GetResourceCount();

	SAFE_DELETE_ARRAY( g_pInterfaceSound );

	g_pInterfaceSound = new ISound*[g_MaxInterfaceSoundEffectCount];
		
	for( int i = 0; i < g_MaxInterfaceSoundEffectCount; i++ )
	{	
		init2dsound.m_sFileName = g_InterfaceSoundArchive.GetResourceName( i );
				
		if(!AudioMgr()->CreateSound(g_pInterfaceSound[i])) return false;
		if(!g_pInterfaceSound[i]->Init(init2dsound))
		{
			_XFatalError("Error initializing audio file");
			return false;
		}
	}


	ID_SR_INTERFACE_INTERFACE_DROPICON01_WAV			= g_InterfaceSoundArchive.FindResource( "DROPICON01.WAV" );
	ID_SR_INTERFACE_INTERFACE_LEARNSKILL01_WAV			= g_InterfaceSoundArchive.FindResource( "LEARNSKILL01.WAV" );
	ID_SR_INTERFACE_INTERFACE_PICK_ITEM_001_WAV			= g_InterfaceSoundArchive.FindResource( "PICK_ITEM_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_LEVELUP_INTRO_WAV			= g_InterfaceSoundArchive.FindResource( "LEVELUP_INTRO.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_HOOP_001_WAV			= g_InterfaceSoundArchive.FindResource( "DROP_HOOP_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_SKILLBOOKDROP01_WAV		= g_InterfaceSoundArchive.FindResource( "SKILLBOOKDROP01.WAV" );
	ID_SR_INTERFACE_INTERFACE_SLIDEBUTTON01_WAV			= g_InterfaceSoundArchive.FindResource( "SLIDEBUTTON01.WAV" );
	ID_SR_INTERFACE_INTERFACE_SMALLBUTTONCLICK_WAV		= g_InterfaceSoundArchive.FindResource( "SMALLBUTTONCLICK.WAV" );
	ID_SR_INTERFACE_INTERFACE_TARGETING00_WAV			= g_InterfaceSoundArchive.FindResource( "TARGETING00.WAV" );
	ID_SR_INTERFACE_INTERFACE_WINDOWCLOSE01_WAV			= g_InterfaceSoundArchive.FindResource( "WINDOWCLOSE01.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_CLOTH_001_WAV		= g_InterfaceSoundArchive.FindResource( "DROP_CLOTH_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_WOOD_001_WAV			= g_InterfaceSoundArchive.FindResource( "DROP_WOOD_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_PICKITEM01_WAV			= g_InterfaceSoundArchive.FindResource( "PICKITEM01.WAV" );
	ID_SR_INTERFACE_INTERFACE_ERROR01_WAV				= g_InterfaceSoundArchive.FindResource( "ERROR01.WAV" );
	ID_SR_INTERFACE_INTERFACE_PARAMETERUP01_WAV			= g_InterfaceSoundArchive.FindResource( "PARAMETERUP01.WAV" );
	ID_SR_INTERFACE_INTERFACE_BURNANGER01_WAV			= g_InterfaceSoundArchive.FindResource( "BURNANGER01.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_METAL_002_WAV		= g_InterfaceSoundArchive.FindResource( "DROP_METAL_002.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_WOOD_002_WAV			= g_InterfaceSoundArchive.FindResource( "DROP_WOOD_002.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_SHINE_001_WAV		= g_InterfaceSoundArchive.FindResource( "DROP_SHINE_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_003_WAV			= g_InterfaceSoundArchive.FindResource( "USE_EAT_003.WAV" );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_001_WAV			= g_InterfaceSoundArchive.FindResource( "USE_EAT_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_LIKE_DEATH_WAV			= g_InterfaceSoundArchive.FindResource( "LIKE_DEATH.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_CHOP_001_WAV			= g_InterfaceSoundArchive.FindResource( "DROP_CHOP_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_ANGERFIRE01_WAV			= g_InterfaceSoundArchive.FindResource( "ANGERFIRE01.WAV" );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_002_WAV			= g_InterfaceSoundArchive.FindResource( "USE_EAT_002.WAV" );
	ID_SR_INTERFACE_INTERFACE_LEVELFAIL_WAV				= g_InterfaceSoundArchive.FindResource( "LEVELFAIL.WAV" );
	ID_SR_INTERFACE_INTERFACE_QUESTSUCCESS02_WAV		= g_InterfaceSoundArchive.FindResource( "QUESTSUCCESS02.WAV" );
	ID_SR_INTERFACE_INTERFACE_QUESTFIND02_WAV			= g_InterfaceSoundArchive.FindResource( "QUESTFIND02.WAV" );
	ID_SR_INTERFACE_INTERFACE_NORMALSTART01_WAV			= g_InterfaceSoundArchive.FindResource( "NORMALSTART01.WAV" );
	ID_SR_INTERFACE_INTERFACE_LEVELUP01_WAV				= g_InterfaceSoundArchive.FindResource( "LEVELUP01.WAV" );
	ID_SR_INTERFACE_INTERFACE_WINDOWOPEN01_WAV			= g_InterfaceSoundArchive.FindResource( "WINDOWOPEN01.WAV" );
	ID_SR_INTERFACE_INTERFACE_WARNING01_WAV				= g_InterfaceSoundArchive.FindResource( "WARNING01.WAV" );
	ID_SR_INTERFACE_INTERFACE_QUICKBUTTONCLICK01_WAV	= g_InterfaceSoundArchive.FindResource( "QUICKBUTTONCLICK01.WAV" );
	ID_SR_INTERFACE_INTERFACE_CLOTHESDROP01_WAV			= g_InterfaceSoundArchive.FindResource( "CLOTHESDROP01.WAV" );
	ID_SR_INTERFACE_INTERFACE_WOODDROP01_WAV			= g_InterfaceSoundArchive.FindResource( "WOODDROP01.WAV" );
	ID_SR_INTERFACE_INTERFACE_QUESTFAIL01_WAV			= g_InterfaceSoundArchive.FindResource( "QUESTFAIL01.WAV" );
	ID_SR_INTERFACE_INTERFACE_QUESTSUCCESS01_WAV		= g_InterfaceSoundArchive.FindResource( "QUESTSUCCESS01.WAV" );
	ID_SR_INTERFACE_INTERFACE_QUESTFIND01_WAV			= g_InterfaceSoundArchive.FindResource( "QUESTFIND01.WAV" );
	ID_SR_INTERFACE_INTERFACE_TURN_POINT00_WAV			= g_InterfaceSoundArchive.FindResource( "TURN_POINT00.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_METAL_001_WAV		= g_InterfaceSoundArchive.FindResource( "DROP_METAL_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_CHOP_002_WAV			= g_InterfaceSoundArchive.FindResource( "DROP_CHOP_002.WAV" );
	ID_SR_INTERFACE_INTERFACE_USE_EAT_004_WAV			= g_InterfaceSoundArchive.FindResource( "USE_EAT_004.WAV" );
	ID_SR_INTERFACE_INTERFACE_BIGBUTTONCLICK01_WAV		= g_InterfaceSoundArchive.FindResource( "BIGBUTTONCLICK01.WAV" );
	ID_SR_INTERFACE_INTERFACE_CHOICEICON01_WAV			= g_InterfaceSoundArchive.FindResource( "CHOICEICON01.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_LIGHT_001_WAV		= g_InterfaceSoundArchive.FindResource( "DROP_LIGHT_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_GLASS_DROP_001_WAV	= g_InterfaceSoundArchive.FindResource( "DROP_GLASS_DROP_001.WAV" );
	ID_SR_INTERFACE_INTERFACE_DROP_WHEEP_001_WAV		= g_InterfaceSoundArchive.FindResource( "DROP_WHEEP_001.WAV" );

	ID_SR_INTERFACE_INTERFACE_END_WAV					= g_InterfaceSoundArchive.FindResource( "END.WAV" );   
	ID_SR_INTERFACE_INTERFACE_START_WAV					= g_InterfaceSoundArchive.FindResource( "START.WAV" ); 
	ID_SR_INTERFACE_INTERFACE_EVENT_WAV					= g_InterfaceSoundArchive.FindResource( "EVENT.WAV" ); 
	ID_SR_INTERFACE_INTERFACE_UP_WAV					= g_InterfaceSoundArchive.FindResource( "UP.WAV" );    
	ID_SR_INTERFACE_INTERFACE_CLOSE_WAV					= g_InterfaceSoundArchive.FindResource( "CLOSE.WAV" ); 
	ID_SR_INTERFACE_INTERFACE_GRANT1_WAV				= g_InterfaceSoundArchive.FindResource( "GRANT1.WAV" );
	ID_SR_INTERFACE_INTERFACE_GRANT2_WAV				= g_InterfaceSoundArchive.FindResource( "GRANT2.WAV" );
	ID_SR_INTERFACE_INTERFACE_READY_WAV					= g_InterfaceSoundArchive.FindResource( "READY.WAV" ); 
	ID_SR_INTERFACE_INTERFACE_NO_WAV					= g_InterfaceSoundArchive.FindResource( "NO.WAV" );    
	ID_SR_INTERFACE_INTERFACE_YES1_WAV					= g_InterfaceSoundArchive.FindResource( "YES1.WAV" );  
	ID_SR_INTERFACE_INTERFACE_YES2_WAV					= g_InterfaceSoundArchive.FindResource( "YES2.WAV" ); 	
	ID_SR_INTERFACE_INTERFACE_BUTTON_WAV				= g_InterfaceSoundArchive.FindResource( "BUTTON.WAV" );
	ID_SR_INTERFACE_INTERFACE_OPEN_WAV					= g_InterfaceSoundArchive.FindResource( "OPEN.WAV" );  

	ID_SR_INTERFACE_INTERFACE_BLUE_01_WAV				= g_InterfaceSoundArchive.FindResource( "BLUE_01.WAV" );
	ID_SR_INTERFACE_INTERFACE_MISS_01_WAV				= g_InterfaceSoundArchive.FindResource( "MISS_01.WAV" );
	ID_SR_INTERFACE_INTERFACE_BLACK_01_WAV				= g_InterfaceSoundArchive.FindResource( "BLACK_01.WAV" );
	ID_SR_INTERFACE_INTERFACE_WHITE_01_WAV				= g_InterfaceSoundArchive.FindResource( "WHITE_01.WAV" );
	ID_SR_INTERFACE_INTERFACE_KIPOINT_01_WAV			= g_InterfaceSoundArchive.FindResource( "KIPOINT_01.WAV" );

	ID_SR_INTERFACE_INTERFACE_SKILL_CLICK_WAV			= g_InterfaceSoundArchive.FindResource( "SKILL_CLICK.wav" );
	ID_SR_INTERFACE_INTERFACE_SKILL_DOWN_WAV			= g_InterfaceSoundArchive.FindResource( "SKILL_DOWN.wav" );
	ID_SR_INTERFACE_INTERFACE_TRA_COMBO_01_WAV		    = g_InterfaceSoundArchive.FindResource( "TRA_COMBO01.WAV" );
	ID_SR_INTERFACE_INTERFACE_TRA_MISS_01_WAV			= g_InterfaceSoundArchive.FindResource( "TRA_MISS01.WAV" );
	ID_SR_INTERFACE_INTERFACE_TRA_UP_01_WAV				= g_InterfaceSoundArchive.FindResource( "TRA_up01.wav" );
	ID_SR_INTERFACE_INTERFACE_TRA_OK_01_WAV				= g_InterfaceSoundArchive.FindResource( "TRA_ok01.wav" );
	
	ID_SR_INTERFACE_INTERFACE_STARTGAME_WAV				= g_InterfaceSoundArchive.FindResource( "StartGame.wav" );

	// Load item use sound list
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
	FILE* pItemUseSoundScript = g_ScriptArchive.GetPackedFile( _T("ItemSoundList.ini") );
	if( !pItemUseSoundScript ) return FALSE;

	int index = 0;
	TCHAR buffer[128];
	TCHAR soundnamebuffer[128];
	memset( buffer, 0, sizeof(TCHAR) * 128 );
	if( !fgets( buffer, 127, pItemUseSoundScript ) )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	g_ItemUseSoundIndexTableSize = atoi( buffer ) + 1;
	if( g_ItemUseSoundIndexTableSize > 0 )
	{
		SAFE_DELETE_ARRAY( g_ItemUseSoundIndexTable );
		g_ItemUseSoundIndexTable = new int[g_ItemUseSoundIndexTableSize];
	}

	do 
	{		
		if( index >= g_ItemUseSoundIndexTableSize )
		{			
			break;
		}
		
		if( feof(pItemUseSoundScript) ) break;
		
		memset( buffer, 0, sizeof(TCHAR)*128);
		if( !fgets( buffer, 127, pItemUseSoundScript ) )
		{
			break;
		}
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		
		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			int insertpos = 0;
			sscanf( buffer, "%d %s", &insertpos, soundnamebuffer );
			
			if( insertpos < g_ItemUseSoundIndexTableSize )
			{
				g_ItemUseSoundIndexTable[insertpos] = g_InterfaceSoundArchive.FindResource( soundnamebuffer );

				if( g_ItemUseSoundIndexTable[insertpos] < 0 )
				{
					_XFatalError( "Can't found interface sound file [%s]", soundnamebuffer );
					break;
				}
			}
			
			index++;
		}
		
	} while( !feof(pItemUseSoundScript) );
	
	g_ScriptArchive.ClosePackage();
	
	g_InterfaceSoundArchive.Close( FALSE );


	g_ClickSoundIndex = ID_SR_INTERFACE_INTERFACE_SMALLBUTTONCLICK_WAV;


	// Load effect sound ------------------------------------------------------
	_XSetDefaultPath(_XDEF_DEFAULTPATH);
	if( !g_SoundEffectArchive.Open( _XDEF_SOUNDEFFECTARCHIVENAME ) )
	{
		return FALSE;
	}

	g_MaxSoundEffectCount = g_SoundEffectArchive.GetResourceCount();

	Sound3DInit init3dsound;
	init3dsound.m_bLooping = false;
	init3dsound.m_bStreaming = false;
	init3dsound.m_bMusic = false;	
	init3dsound.m_pPackage = &g_SoundEffectArchive;

	// Use 3D Sound system
	for( i = 0; i < g_MaxSoundEffectCount; i++ )
	{
		init3dsound.m_sFileName = g_SoundEffectArchive.GetResourceName( i );

		if(!AudioMgr()->CreateSound3D(g_pSoundEffect[i])) return false;
		if(!g_pSoundEffect[i]->Init(init3dsound))
		{
			_XFatalError("Error initializing 3d audio file");
			return false;
		}		

		g_pSoundEffect[i]->SetVolume( 1.0f );
	}	

	g_DefenceEffectSoundIndex[0][0] = g_SoundEffectArchive.FindResource( "MCh01_Shout01.wav" );	// 남자 기합
	g_DefenceEffectSoundIndex[0][1] = g_SoundEffectArchive.FindResource( "MCh01_Shout02.wav" );
	g_DefenceEffectSoundIndex[0][2] = g_SoundEffectArchive.FindResource( "MCh01_Shout03.wav" );

	g_DefenceEffectSoundIndex[1][0] = g_SoundEffectArchive.FindResource( "WCh03_Shout01.wav" );  // 여자 기합
	g_DefenceEffectSoundIndex[1][1] = g_SoundEffectArchive.FindResource( "WCh03_Shout02.wav" );
	g_DefenceEffectSoundIndex[1][2] = g_SoundEffectArchive.FindResource( "WCh03_Shout03.wav" );

	g_BlowEffectSoundIndex[0][0] = g_SoundEffectArchive.FindResource( "MCh01_Scream01.wav" );	// 남자 맞기 
	g_BlowEffectSoundIndex[0][1] = g_SoundEffectArchive.FindResource( "MCh01_Scream02.wav" );
	g_BlowEffectSoundIndex[0][2] = g_SoundEffectArchive.FindResource( "MCh01_Scream03.wav" );

	g_BlowEffectSoundIndex[1][0] = g_SoundEffectArchive.FindResource( "WCh03_Scream01.wav" ); // 여자 맞기 
	g_BlowEffectSoundIndex[1][1] = g_SoundEffectArchive.FindResource( "WCh03_Scream02.wav" );
	g_BlowEffectSoundIndex[1][2] = g_SoundEffectArchive.FindResource( "WCh03_Scream03.wav" );
	

	g_DieEffectSoundIndex[0][0] = g_SoundEffectArchive.FindResource( "MCh01_Die01.wav" ); // 남자 죽기
	g_DieEffectSoundIndex[0][1] = g_SoundEffectArchive.FindResource( "MCh01_Die02.wav" );	

	g_DieEffectSoundIndex[1][0] = g_SoundEffectArchive.FindResource( "WCh03_Die01.wav" ); // 여자 죽기
	g_DieEffectSoundIndex[1][1] = g_SoundEffectArchive.FindResource( "WCh03_Die02.wav" );

	/*
	g_HitEffectSoundIndex[0] = g_SoundEffectArchive.FindResource( "BAREKNUCKLEFLESH01.WAV" );
	g_HitEffectSoundIndex[1] = g_SoundEffectArchive.FindResource( "BAREKNUCKLEFLESH02.WAV" );
	g_HitEffectSoundIndex[2] = g_SoundEffectArchive.FindResource( "BAREKNUCKLEMETAL02.WAV" );
	g_HitEffectSoundIndex[3] = g_SoundEffectArchive.FindResource( "BAREKNUCKLESTONE01.WAV" );
	g_HitEffectSoundIndex[4] = g_SoundEffectArchive.FindResource( "BAREKNUCKLEWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[0][0] = 0;
	g_HitEffectSoundCategoryLUT[0][1] = 4;


	g_HitEffectSoundIndex[5] = g_SoundEffectArchive.FindResource( "FISTBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[6] = g_SoundEffectArchive.FindResource( "FISTBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[7] = g_SoundEffectArchive.FindResource( "FISTBASHFLASH03.WAV" );
	g_HitEffectSoundIndex[8] = g_SoundEffectArchive.FindResource( "FISTBASHWOOD01.WAV" );
	g_HitEffectSoundCategoryLUT[1][0] = 5;
	g_HitEffectSoundCategoryLUT[1][1] = 8;


	g_HitEffectSoundIndex[9] =  g_SoundEffectArchive.FindResource( "WOODLIGHTBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[10] = g_SoundEffectArchive.FindResource( "WOODLIGHTBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[11] = g_SoundEffectArchive.FindResource( "WOODLIGHTBASHFLASH03.WAV" );
	g_HitEffectSoundIndex[12] = g_SoundEffectArchive.FindResource( "WOODLIGHTBASHWOOD01.WAV" );
	g_HitEffectSoundIndex[13] = g_SoundEffectArchive.FindResource( "WOODLIGHTBASHWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[2][0] = 9;
	g_HitEffectSoundCategoryLUT[2][1] = 13;

	g_HitEffectSoundIndex[14] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHFLESH01.WAV" );
	g_HitEffectSoundIndex[15] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHFLESH02.WAV" );
	g_HitEffectSoundIndex[16] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHFLESH03.WAV" );
	g_HitEffectSoundIndex[17] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHWOOD01.WAV" );
	g_HitEffectSoundIndex[18] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[3][0] = 14;
	g_HitEffectSoundCategoryLUT[3][1] = 18;
	

	g_HitEffectSoundIndex[19] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[20] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[21] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHFLASH03.WAV" );
	g_HitEffectSoundIndex[22] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHSTONE01.WAV" );
	g_HitEffectSoundIndex[23] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHSTONE02.WAV" );
	g_HitEffectSoundCategoryLUT[4][0] = 19;
	g_HitEffectSoundCategoryLUT[4][1] = 23;

	g_HitEffectSoundIndex[24] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICEFLASH01.WAV" );
	g_HitEffectSoundIndex[25] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICEFLASH03.WAV" );
	g_HitEffectSoundIndex[26] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICEMETAL01.WAV" );
	g_HitEffectSoundIndex[27] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICEMETAL02.WAV" );
	g_HitEffectSoundIndex[28] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICESTONE01.WAV" );
	g_HitEffectSoundIndex[29] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICESTONE02.WAV" );
	g_HitEffectSoundCategoryLUT[5][0] = 24;
	g_HitEffectSoundCategoryLUT[5][1] = 29;

	g_HitEffectSoundIndex[30] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABFLASH01.WAV" );
	g_HitEffectSoundIndex[31] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABFLASH02.WAV" );
	g_HitEffectSoundIndex[32] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABFLASH03.WAV" );
	g_HitEffectSoundIndex[33] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABSTONE01.WAV" );
	g_HitEffectSoundIndex[34] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABWOOD01.WAV" );
	g_HitEffectSoundIndex[35] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[6][0] = 30;
	g_HitEffectSoundCategoryLUT[6][1] = 35;

	g_HitEffectSoundIndex[36] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[37] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[38] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHFLASH03.WAV" );
	g_HitEffectSoundIndex[39] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHMETAL01.WAV" );
	g_HitEffectSoundIndex[40] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHMETAL02.WAV" );
	g_HitEffectSoundCategoryLUT[7][0] = 36;
	g_HitEffectSoundCategoryLUT[7][1] = 40;

	g_HitEffectSoundIndex[41] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEFLASH01.WAV" );
	g_HitEffectSoundIndex[42] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEFLASH02.WAV" );
	g_HitEffectSoundIndex[43] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEFLASH03.WAV" );
	g_HitEffectSoundIndex[44] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEMETAL01.WAV" );
	g_HitEffectSoundIndex[45] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[8][0] = 41;
	g_HitEffectSoundCategoryLUT[8][1] = 45;

	g_HitEffectSoundIndex[46] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABFLASH01.WAV" );
	g_HitEffectSoundIndex[47] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABFLASH02.WAV" );
	g_HitEffectSoundIndex[48] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABFLASH03.WAV" );
	g_HitEffectSoundIndex[49] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABWOOD01.WAV" );
	g_HitEffectSoundIndex[50] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[9][0] = 46;
	g_HitEffectSoundCategoryLUT[9][1] = 50;

	g_HitEffectSoundIndex[51] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[52] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[53] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHFLASH03.WAV" );
	g_HitEffectSoundIndex[54] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHSTONE01.WAV" );
	g_HitEffectSoundIndex[55] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHSTONE02.WAV" );
	g_HitEffectSoundCategoryLUT[10][0] = 51;
	g_HitEffectSoundCategoryLUT[10][1] = 55;

	g_HitEffectSoundIndex[56] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEFLASH01.WAV" );
	g_HitEffectSoundIndex[57] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEFLASH02.WAV" );
	g_HitEffectSoundIndex[58] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEFLASH03.WAV" );
	g_HitEffectSoundIndex[59] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEWOOD01.WAV" );
	g_HitEffectSoundIndex[60] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[11][0] = 56;
	g_HitEffectSoundCategoryLUT[11][1] = 60;

	g_HitEffectSoundIndex[61] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABFLASH01.WAV" );
	g_HitEffectSoundIndex[62] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABFLASH02.WAV" );
	g_HitEffectSoundIndex[63] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABFLASH03.WAV" );
	g_HitEffectSoundIndex[64] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABSTONE01.WAV" );
	g_HitEffectSoundIndex[65] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABSTONE02.WAV" );	
	g_HitEffectSoundCategoryLUT[12][0] = 61;
	g_HitEffectSoundCategoryLUT[12][1] = 65;
	*/
	
	g_HitEffectSoundIndex[0] = g_SoundEffectArchive.FindResource( "BAREKNUCKLEFLESH01.WAV" );	
	g_HitEffectSoundIndex[1] = g_SoundEffectArchive.FindResource( "BAREKNUCKLEMETAL02.WAV" );
	g_HitEffectSoundIndex[2] = g_SoundEffectArchive.FindResource( "BAREKNUCKLEWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[0][0] = 0;
	g_HitEffectSoundCategoryLUT[0][1] = 2;
	g_CriticalEffectSoundIndex[0] = g_SoundEffectArchive.FindResource( "cri_fist.wav" );


	g_HitEffectSoundIndex[3] = g_SoundEffectArchive.FindResource( "FISTBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[4] = g_SoundEffectArchive.FindResource( "FISTBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[5] = g_SoundEffectArchive.FindResource( "FISTBASHFLASH03.WAV" );
	g_HitEffectSoundCategoryLUT[1][0] = 3;
	g_HitEffectSoundCategoryLUT[1][1] = 5;
	g_CriticalEffectSoundIndex[1] = g_SoundEffectArchive.FindResource( "cri_fist.wav" );


	g_HitEffectSoundIndex[6] =  g_SoundEffectArchive.FindResource( "WOODLIGHTBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[7] = g_SoundEffectArchive.FindResource( "WOODLIGHTBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[8] = g_SoundEffectArchive.FindResource( "WOODLIGHTBASHFLASH03.WAV" );
	g_HitEffectSoundCategoryLUT[2][0] = 6;
	g_HitEffectSoundCategoryLUT[2][1] = 8;
	g_CriticalEffectSoundIndex[2] = g_SoundEffectArchive.FindResource( "cri_WoodLight.wav" );

	g_HitEffectSoundIndex[9] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHFLESH01.WAV" );
	g_HitEffectSoundIndex[10] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHFLESH02.WAV" );
	g_HitEffectSoundIndex[11] = g_SoundEffectArchive.FindResource( "WOODHEAVYBASHWOOD02.WAV" );
	g_HitEffectSoundCategoryLUT[3][0] = 9;
	g_HitEffectSoundCategoryLUT[3][1] = 11;
	g_CriticalEffectSoundIndex[3] = g_SoundEffectArchive.FindResource( "cri_WoodHeavy.wav" );
	

	g_HitEffectSoundIndex[12] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[13] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[14] = g_SoundEffectArchive.FindResource( "METALLIGHTBASHFLASH03.WAV" );
	g_HitEffectSoundCategoryLUT[4][0] = 12;
	g_HitEffectSoundCategoryLUT[4][1] = 14;
	g_CriticalEffectSoundIndex[4] = g_SoundEffectArchive.FindResource( "cri_MetalLight.wav" );

	g_HitEffectSoundIndex[15] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICEFLASH01.WAV" );
	g_HitEffectSoundIndex[16] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICEFLASH03.WAV" );
	g_HitEffectSoundIndex[17] = g_SoundEffectArchive.FindResource( "METALLIGHTSLICEMETAL01.WAV" );
	g_HitEffectSoundCategoryLUT[5][0] = 15;
	g_HitEffectSoundCategoryLUT[5][1] = 17;
	g_CriticalEffectSoundIndex[5] = g_SoundEffectArchive.FindResource( "cri_MetalLight.wav" );

	g_HitEffectSoundIndex[18] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABFLASH01.WAV" );	
	g_HitEffectSoundIndex[19] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABFLASH03.WAV" );
	g_HitEffectSoundIndex[20] = g_SoundEffectArchive.FindResource( "METALLIGHTSTABSTONE01.WAV" );
	g_HitEffectSoundCategoryLUT[6][0] = 18;
	g_HitEffectSoundCategoryLUT[6][1] = 20;
	g_CriticalEffectSoundIndex[6] = g_SoundEffectArchive.FindResource( "cri_MetalLight.wav" );

	g_HitEffectSoundIndex[21] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[22] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[23] = g_SoundEffectArchive.FindResource( "METALMEDIUMBASHFLASH03.WAV" );
	g_HitEffectSoundCategoryLUT[7][0] = 21;
	g_HitEffectSoundCategoryLUT[7][1] = 23;
	g_CriticalEffectSoundIndex[7] = g_SoundEffectArchive.FindResource( "cri_MetalMedium.wav" );

	g_HitEffectSoundIndex[24] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEFLASH01.WAV" );
	g_HitEffectSoundIndex[25] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEFLASH02.WAV" );
	g_HitEffectSoundIndex[26] = g_SoundEffectArchive.FindResource( "METALMEDIUMSLICEFLASH03.WAV" );
	g_HitEffectSoundCategoryLUT[8][0] = 24;
	g_HitEffectSoundCategoryLUT[8][1] = 26;
	g_CriticalEffectSoundIndex[8] = g_SoundEffectArchive.FindResource( "cri_MetalMedium.wav" );

	g_HitEffectSoundIndex[27] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABFLASH01.WAV" );
	g_HitEffectSoundIndex[28] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABFLASH02.WAV" );	
	g_HitEffectSoundIndex[29] = g_SoundEffectArchive.FindResource( "METALMEDIUMSTABWOOD01.WAV" );
	g_HitEffectSoundCategoryLUT[9][0] = 27;
	g_HitEffectSoundCategoryLUT[9][1] = 29;
	g_CriticalEffectSoundIndex[9] = g_SoundEffectArchive.FindResource( "cri_MetalMedium.wav" );

	g_HitEffectSoundIndex[30] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHFLASH01.WAV" );
	g_HitEffectSoundIndex[31] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHFLASH02.WAV" );
	g_HitEffectSoundIndex[32] = g_SoundEffectArchive.FindResource( "METALHEAVYBASHSTONE02.WAV" );
	g_HitEffectSoundCategoryLUT[10][0] = 30;
	g_HitEffectSoundCategoryLUT[10][1] = 32;
	g_CriticalEffectSoundIndex[10] = g_SoundEffectArchive.FindResource( "cri_MetalHeavy.wav" );

	g_HitEffectSoundIndex[33] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEFLASH01.WAV" );
	g_HitEffectSoundIndex[34] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEFLASH02.WAV" );
	g_HitEffectSoundIndex[35] = g_SoundEffectArchive.FindResource( "METALHEAVYSLICEFLASH03.WAV" );
	g_HitEffectSoundCategoryLUT[11][0] = 33;
	g_HitEffectSoundCategoryLUT[11][1] = 35;
	g_CriticalEffectSoundIndex[11] = g_SoundEffectArchive.FindResource( "cri_MetalHeavy.wav" );

	g_HitEffectSoundIndex[36] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABFLASH01.WAV" );	
	g_HitEffectSoundIndex[37] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABFLASH03.WAV" );
	g_HitEffectSoundIndex[38] = g_SoundEffectArchive.FindResource( "METALHEAVYSTABSTONE02.WAV" );	
	g_HitEffectSoundCategoryLUT[12][0] = 36;
	g_HitEffectSoundCategoryLUT[12][1] = 38;
	g_CriticalEffectSoundIndex[12] = g_SoundEffectArchive.FindResource( "cri_MetalHeavy.wav" );
	
	g_SoundEffectArchive.Close( FALSE );        	

	// Load npc speech archive ------------------------------------------------------
	if( !g_NPCSoundArchive.Open( _XDEF_NPCSPEECHARCHIVENAME ) )
	{
		return FALSE;
	}
	g_NPCSoundArchive.Close( FALSE );
		
	return TRUE;
}
#endif

void XProc_FirstLoad::ReleaseSoundEffect( void )
{	
	_XLog( "Release sound object" );

#ifndef _XUSEFMOD
	g_SoundEffectArchive.Close( TRUE );
	g_InterfaceSoundArchive.Close( TRUE );
	g_EnvSoundEffectArchive.Close( TRUE );
	g_NPCSoundArchive.Close( TRUE );
#endif

}


BOOL XProc_FirstLoad::InitializeBGMList(void)
{
	int bgmcount;
	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* pScripterFile = NULL;

	pScripterFile = g_ScriptArchive.GetPackedFile( _T("musiclist.ini") );
	
	if( !pScripterFile )
	{
		_XFatalError("Can't open bgm list file.");
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	TCHAR buffer[256];
	memset( buffer, 0, sizeof(TCHAR) * 256 );

	if( fgets(buffer, 255, pScripterFile) == 0 )
	{
		_XFatalError("Can't read bgm list count.");
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	bgmcount = atoi( buffer );	
	
#ifdef _XDWDEBUG
	_XDWINPRINT("=========== BGM LIST ==============");
	_XDWINPRINT("bgmcount : %d", bgmcount);
#endif
	
	for(int i = 0 ; i < bgmcount ; i++)
	{
		if( fgets(g_BgmList[i], sizeof(TCHAR)*_XDEF_MAXBGMCHAR, pScripterFile ) == 0 )
		{
			_XFatalError("Can't read bgm list.");
			g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		
#ifdef _XDWDEBUG
		_XDWINPRINT("name : %s", g_BgmList[i]);
#endif
	}
		
	g_ScriptArchive.ClosePackage();

	return TRUE;
}



BOOL XProc_FirstLoad::InitializeSpeedTreeWrapper( void )
{
	//CSpeedTreeRT::Authorize( _T("RT-0270-987251a99c6e") );
	CSpeedTreeRT::Authorize( _T("RT-0405-cd8551b19ff8") );
	//CSpeedTreeRT::Authorize( _T("RT-0567-cda40724f8f8") );

	return TRUE;
}

BOOL XProc_FirstLoad::LoadQuestScript(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		_XDeleteFile(_T("questscript.xqs"));
	}
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );

	FILE* pScriptFile = NULL;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	g_LoadQuestScriptText_LastIndex = 0;
	SAFE_DELETE_ARRAY( g_LoadQuestScriptText_IndexTable );

	pScriptFile = g_ScriptArchive.GetPackedFile( _T("QuestTextIndex.xti") );

	int count = 0;
	fread( &count, sizeof(int), 1, pScriptFile );
	
	if( count <= 0 )
	{
		_XFatalError( _T("Invalid text index table size") );
		g_ScriptArchive.ClosePackage();
		return FALSE;			
	}	

	g_LoadQuestScriptText_IndexTable = new int[count];
	if( fread( g_LoadQuestScriptText_IndexTable, sizeof(int) * count, 1, pScriptFile ) < 1 )
	{
		_XFatalError( _T("Could not read text index table") );
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
#endif
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		pScriptFile = fopen( _T("questscript.xqs"), "rb" );

		if( !pScriptFile )
		{
			bLoadFromFile = FALSE;

			pScriptFile = g_ScriptArchive.GetPackedFile( _T("questscript.xqs") );
		}
	}
	else
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("questscript.xqs") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		pScriptFile = fopen( _T("questscript.xqs"), "rb" );

		if( !pScriptFile )
		{
			bLoadFromFile = FALSE;
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("questscript.xqs") );
		}
	}
	else
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("questscript.xqs") );
	}
#endif

	if( !pScriptFile )
	{
		_XFatalError("Can't load quest script");
		if( !bLoadFromFile )
		{
			g_ScriptArchive.ClosePackage();
		}
		return FALSE;
	}
	
	if(!g_QuestScriptManager.LoadQuestScript(pScriptFile))
	{
		if( bLoadFromFile )
		{	
			fclose( pScriptFile );
		}
		else
		{
			g_ScriptArchive.ClosePackage();
		}
		return FALSE;
	}
	
	if( bLoadFromFile )
	{	
		fclose( pScriptFile );
	}
	else
	{
		g_ScriptArchive.ClosePackage();
	}

	return TRUE;
}

BOOL XProc_FirstLoad::LoadMatchScript()
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("DuelRankRef.txt"));
	}		
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("DuelRankRef.txt"), "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( _T("DuelRankRef.txt") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("DuelRankRef.txt") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("DuelRankRef.txt"), "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("DuelRankRef.txt") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("DuelRankRef.txt") );
	}
#endif
	
	if( !fileptr )
	{
		_XFatalError("Can't load DuelRankRef");
		if( !bLoadFromFile )
			g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	TCHAR buffer[256];
	int index = 0;
	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 256, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			sscanf(buffer, "%d", &index);			
#ifdef _XDEF_NEWSTRINGMANAGER
			TCHAR	_temptring[128];
			sscanf(buffer, "%d %s", &index, _temptring);
			g_MatchGradeName[index] = g_StringDataBase.GetTextString( _XSTRINGDB_SN_DUELFIGHTMONEYREF_TXT, index );
#else			
			sscanf(buffer, "%d %s", &index, g_MatchGradeName[index]);
#endif
		}

	} while(!feof(fileptr));

	
#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("DuelFightMoneyRef.txt"));
	}		
#endif
	
	fileptr = NULL;
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("DuelFightMoneyRef.txt"), "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( _T("DuelFightMoneyRef.txt") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("DuelFightMoneyRef.txt") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("DuelFightMoneyRef.txt"), "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("DuelFightMoneyRef.txt") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("DuelFightMoneyRef.txt") );
	}
#endif
	
	if( !fileptr )
	{
		_XFatalError("Can't load DuelFightMoneyRef");
		if( !bLoadFromFile )
			g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	memset(buffer, 0, sizeof(buffer));
	index = 0;
	TCHAR matchname[128];

	do 
	{
		if(feof(fileptr))
			break;
		
		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 256, fileptr))
			break;
		
		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			sscanf(buffer, "%d", &index);
			sscanf(buffer, "%d %s %d", &index, matchname, &g_MatchFightMoney[index-1]);
			
			// Localize modify
			// matchname = g_StringDataBase.GetTextString( _XSTRINGDB_SN_DUELFIGHTMONEYREF_TXT, index );
		}
		
	} while(!feof(fileptr));

	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();

	return TRUE;
}

BOOL XProc_FirstLoad::LoadNPCBuffScript(void)
{
	// load npc buff script
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("BuffNPCRef.txt"));
	}		
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;


	TCHAR scriptfilename[32];

	memset( scriptfilename, 0, sizeof(TCHAR) * 32 );
	strcpy( scriptfilename, _T("BuffNPCRef.txt") );
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( scriptfilename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( scriptfilename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
	}
#endif
	
	if( !fileptr )
	{
		_XFatalError("Can't load BuffNPCRef");
		if( !bLoadFromFile )
			g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	TCHAR buffer[512];
	TCHAR tempbuffer1[512];
	TCHAR tempbuffer2[512];
	TCHAR contryname[32];
	int nTotalCount = 0, index = 0;
	int	nReadOrder = 0, nReadCount = 0;

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 512, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(nReadOrder == 0)
			{
				sscanf(buffer, "%d", &nTotalCount);
//				g_NPCBuffStr = new _XNPCBUFFSTR[nTotalCount];
//				memset(g_NPCBuffStr, 0, sizeof(_XNPCBUFFSTR)*nTotalCount);
				g_NPCBuffCount = nTotalCount;
				nReadOrder = 1;
			}
			else if(nReadOrder == 1)
			{
				_XNPCBUFFSTR* pNPCBuffStr = new _XNPCBUFFSTR;

				sscanf(buffer, "%d", &index);

				sscanf(buffer, "%d %s %s %d %d %d %d %d %d", 
					&index, tempbuffer1, tempbuffer2, 
					&pNPCBuffStr->type,
					&pNPCBuffStr->price,
					&pNPCBuffStr->bufindex,
					&pNPCBuffStr->bufvalue,
					&pNPCBuffStr->time,
					&pNPCBuffStr->nSkillindex);

//					&g_NPCBuffStr[index-1].type, 
//					&g_NPCBuffStr[index-1].price, 
//					&g_NPCBuffStr[index-1].bufindex, 
//					&g_NPCBuffStr[index-1].bufvalue, 
//					&g_NPCBuffStr[index-1].time,
//					&g_NPCBuffStr[index-1].nSkillindex );

#ifdef _XDEF_NEWSTRINGMANAGER
//				g_NPCBuffStr[index-1].name = g_StringDataBase.GetTextString( _XSTRINGDB_SN_BUFFNPCREF_TXT_NAME, index );
//				g_NPCBuffStr[index-1].info = g_StringDataBase.GetTextString( _XSTRINGDB_SN_BUFFNPCREF_TXT_INFO, index );
				if(index > 100)
				{
					pNPCBuffStr->name = g_StringDataBase.GetTextString( _XSTRINGDB_SN_BUFFNPCREF_TXT_NAME, index-100 );
					pNPCBuffStr->info = g_StringDataBase.GetTextString( _XSTRINGDB_SN_BUFFNPCREF_TXT_INFO, index-100 );
				}
				else
				{
					pNPCBuffStr->name = g_StringDataBase.GetTextString( _XSTRINGDB_SN_BUFFNPCREF_TXT_NAME, index );
					pNPCBuffStr->info = g_StringDataBase.GetTextString( _XSTRINGDB_SN_BUFFNPCREF_TXT_INFO, index );
				}
#else
//				strcpy(g_NPCBuffStr[index-1].name, tempbuffer1);
//				strcpy(g_NPCBuffStr[index-1].info, tempbuffer2);
				strcpy(pNPCBuffStr->name, tempbuffer1);
				strcpy(pNPCBuffStr->info, tempbuffer2);
#endif

				g_NPCBuffStr[index] = pNPCBuffStr;

				nReadCount++;
				if(nReadCount == nTotalCount)
				{
					nReadOrder = 2;
				}
			}
			else if(nReadOrder == 2)
			{
				sscanf(buffer, "%s", contryname);

				if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
				{
					if(strcmp(contryname, "KR") == 0)
					{
						int count = sscanf(buffer, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
											contryname, &g_NPCBuffList.nList[0], &g_NPCBuffList.nList[1], &g_NPCBuffList.nList[2],
											&g_NPCBuffList.nList[3], &g_NPCBuffList.nList[4], &g_NPCBuffList.nList[5],
											&g_NPCBuffList.nList[6], &g_NPCBuffList.nList[7], &g_NPCBuffList.nList[8],&g_NPCBuffList.nList[9],
											&g_NPCBuffList.nList[10], &g_NPCBuffList.nList[11], &g_NPCBuffList.nList[12],
											&g_NPCBuffList.nList[13],&g_NPCBuffList.nList[14]);

						for(int i = 0 ; i < 15 ; ++i)
						{
							if(i >= count)
								g_NPCBuffList.nList[i] = -1;
						}
					}
				}
				else if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
				{
					if(strcmp(contryname, "VN") == 0)
					{//Author : 양희왕, breif 수정   & 붙임
						int count = sscanf(buffer, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
											contryname, &g_NPCBuffList.nList[0], &g_NPCBuffList.nList[1], &g_NPCBuffList.nList[2],
											&g_NPCBuffList.nList[3], &g_NPCBuffList.nList[4], &g_NPCBuffList.nList[5],
											&g_NPCBuffList.nList[6], &g_NPCBuffList.nList[7], &g_NPCBuffList.nList[8],&g_NPCBuffList.nList[9],
											&g_NPCBuffList.nList[10], &g_NPCBuffList.nList[11], &g_NPCBuffList.nList[12],
											&g_NPCBuffList.nList[13],&g_NPCBuffList.nList[14]);
						for(int i = 0 ; i < 15 ; ++i)
						{
							if(i >= count)
								g_NPCBuffList.nList[i] = -1;
						}
					}
				}
				else if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				{
					if(strcmp(contryname, "US") == 0)
					{
						int count = sscanf(buffer, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
											contryname, &g_NPCBuffList.nList[0], &g_NPCBuffList.nList[1], &g_NPCBuffList.nList[2],
											&g_NPCBuffList.nList[3], &g_NPCBuffList.nList[4], &g_NPCBuffList.nList[5],
											&g_NPCBuffList.nList[6], &g_NPCBuffList.nList[7], &g_NPCBuffList.nList[8],&g_NPCBuffList.nList[9],
											&g_NPCBuffList.nList[10], &g_NPCBuffList.nList[11], &g_NPCBuffList.nList[12],
											&g_NPCBuffList.nList[13],&g_NPCBuffList.nList[14]);
						for(int i = 0 ; i < 15 ; ++i)
						{
							if(i >= count)
								g_NPCBuffList.nList[i] = -1;
						}
					}
				}
				else if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
				{
					if(strcmp(contryname, "TW") == 0)
					{
						int count = sscanf(buffer, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
											contryname, &g_NPCBuffList.nList[0], &g_NPCBuffList.nList[1], &g_NPCBuffList.nList[2],
											&g_NPCBuffList.nList[3], &g_NPCBuffList.nList[4], &g_NPCBuffList.nList[5],
											&g_NPCBuffList.nList[6], &g_NPCBuffList.nList[7], &g_NPCBuffList.nList[8],&g_NPCBuffList.nList[9],
											&g_NPCBuffList.nList[10], &g_NPCBuffList.nList[11], &g_NPCBuffList.nList[12],
											&g_NPCBuffList.nList[13],&g_NPCBuffList.nList[14]);
						for(int i = 0 ; i < 15 ; ++i)
						{
							if(i >= count)
								g_NPCBuffList.nList[i] = -1;
						}
					}
				}
				else if(g_LanguageType == _XLANGUAGE_TYPE_JAPANESE)
				{
					if(strcmp(contryname, "JP") == 0)
					{
						int count = sscanf(buffer, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
											contryname, &g_NPCBuffList.nList[0], &g_NPCBuffList.nList[1], &g_NPCBuffList.nList[2],
											&g_NPCBuffList.nList[3], &g_NPCBuffList.nList[4], &g_NPCBuffList.nList[5],
											&g_NPCBuffList.nList[6], &g_NPCBuffList.nList[7], &g_NPCBuffList.nList[8],&g_NPCBuffList.nList[9],
											&g_NPCBuffList.nList[10], &g_NPCBuffList.nList[11], &g_NPCBuffList.nList[12],
											&g_NPCBuffList.nList[13],&g_NPCBuffList.nList[14]);
						for(int i = 0 ; i < 15 ; ++i)
						{
							if(i >= count)
								g_NPCBuffList.nList[i] = -1;
						}
					}
				}
				else if(g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN)
				{
					if(strcmp(contryname, "RS") == 0)
					{
						int count = sscanf(buffer, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
											contryname, &g_NPCBuffList.nList[0], &g_NPCBuffList.nList[1], &g_NPCBuffList.nList[2],
											&g_NPCBuffList.nList[3], &g_NPCBuffList.nList[4], &g_NPCBuffList.nList[5],
											&g_NPCBuffList.nList[6], &g_NPCBuffList.nList[7], &g_NPCBuffList.nList[8],&g_NPCBuffList.nList[9],
											&g_NPCBuffList.nList[10], &g_NPCBuffList.nList[11], &g_NPCBuffList.nList[12],
											&g_NPCBuffList.nList[13],&g_NPCBuffList.nList[14]);
						for(int i = 0 ; i < 15 ; ++i)
						{
							if(i >= count)
								g_NPCBuffList.nList[i] = -1;
						}
					}
				}
			}
		}

	} while(!feof(fileptr));

	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();
	
	return TRUE;
}

BOOL XProc_FirstLoad::LoadFameScript(void)
{
	g_FameStrLength = 0;

	// load fame script
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
	TCHAR filename[64];
	memset( filename, 0, sizeof(TCHAR) * 64 );

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( filename, _T("Fame_Infamy_Ref.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( filename, _T("TW_Fame_Infamy_Ref.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( filename, _T("US_Fame_Infamy_Ref.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( filename, _T("JP_Fame_Infamy_Ref.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( filename, _T("RS_Fame_Infamy_Ref.txt") );
	}
	else
	{
		strcpy( filename, _T("Fame_Infamy_Ref.txt") );
	}

#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(filename);
	}		
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( filename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( filename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( filename );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( filename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( filename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( filename );
	}
#endif
	
	if( !fileptr )
	{
		_XFatalError("Can't load Fame_Infamy_Ref");
		if( !bLoadFromFile )
			g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	TCHAR buffer[256];
	int count = 0, index = 0;
	BOOL bReadCount = FALSE;
	
	do 
	{
		if(feof(fileptr))
			break;
		
		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 256, fileptr))
			break;
		
		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(!bReadCount)
			{
				sscanf(buffer, "%d", &count);
				g_FameStr = new _XFame[count];
				bReadCount = TRUE;

				g_FameStrLength = count;
			}
			else
			{
				sscanf(buffer, "%d", &index);
				sscanf(buffer, "%d %s %d %s %d", 
						&g_FameStr[index-1].m_Index, g_FameStr[index-1].m_FameName, &g_FameStr[index-1].m_FameRange, 
						g_FameStr[index-1].m_InfamyName, &g_FameStr[index-1].m_InfamyRange);

				int _length = strlen(g_FameStr[index-1].m_FameName);
				for( int i = 0; i < _length; i++ )
				{
					if( g_FameStr[index-1].m_FameName[i] == '_' )
						g_FameStr[index-1].m_FameName[i] = ' ';
				}
				_length = strlen(g_FameStr[index-1].m_InfamyName);
				for( i = 0; i < _length; i++ )
				{
					if( g_FameStr[index-1].m_InfamyName[i] == '_' )
						g_FameStr[index-1].m_InfamyName[i] = ' ';
				}
			}
		}
		
	} while(!feof(fileptr));

	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();
	
	return TRUE;
}

BOOL XProc_FirstLoad::LoadMonsterTable(void)
{
	// load monster table - from server
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("monster_ref.bmo"));
	}		
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );

	FILE* fileptr = NULL;
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("monster_ref.bmo"), "rb" );
			
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( _T("monster_ref.bmo") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("monster_ref.bmo") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("monster_ref.bmo"), "rb" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("monster_ref.bmo") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("monster_ref.bmo") );
	}
#endif

	if( !fileptr )
	{
		_XFatalError("Can't load monster reference table");
		if( !bLoadFromFile )
			g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	_XMEMORYUSECHECKREADY
	_XMEMORYUSECHECKSTART

	// header
	TCHAR		fileheadername[_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1];
	short		version[2];
	SYSTEMTIME	updatetime;
	TCHAR		temp_name[128];
	
	memset(fileheadername, 0, sizeof(TCHAR)*(_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1));
	if(fread(fileheadername, sizeof(TCHAR), _XDEF_SKILLTABLE_HEADERNAMELENGTH, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version[0], sizeof(short), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version[1], sizeof(short), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&updatetime, sizeof(SYSTEMTIME), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(temp_name, sizeof(TCHAR)*16, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(temp_name, sizeof(TCHAR)*20, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
#define _COUNT_MONSTER_TYPE_		10000

	unsigned short	usTemp = 0;
	unsigned char	ucTemp = 0;
	unsigned int	unTemp = 0;
	short			sTemp = 0;
	char			cTemp = 0;
	int				nTemp = 0;
	float			fTemp = 0.0f;

	unsigned char	length = 0;
	unsigned short	type_id = 0;
	char			name[256];

	while(true)
	{
		memset(name, 0, sizeof(char)*256);

		if(fread(&type_id, sizeof(unsigned short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		if(type_id <= 4000 || type_id >= 4000 + _COUNT_MONSTER_TYPE_)
			break;

		// level
		if(fread(&g_MobAttribTable[type_id-4000].level, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// party
		if(fread(&g_MobAttribTable[type_id-4000].party, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		
#ifdef _XTS_MONSTERREF_13
		// script type
		if(fread(&g_MobAttribTable[type_id-4000].scripttype, sizeof(unsigned short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#else
		// script type
		if(fread(&g_MobAttribTable[type_id-4000].scripttype, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#endif
		// mission
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// rank
		if(fread(&g_MobAttribTable[type_id-4000].rank, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// length
		if(fread(&length, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		if(length > 0)
		{
			if(fread(name, sizeof(char)*length, 1, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
		}

		// size
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// advent time
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// time_result
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// hp
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// sp
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// stamina
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// upgrade_exp
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// upgrade_count
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// walk_speed
/*			if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// run_speed
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
			return FALSE;
		}*/
		// walk_speed
		if(fread(&g_MobAttribTable[type_id-4000].movespeed_walk, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// run_speed
		if(fread(&g_MobAttribTable[type_id-4000].movespeed_run, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// sp_speed
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// walk_length
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// run_length
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// sp_length
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// activity
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// moon_resist
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		g_MobAttribTable[type_id-4000].resistMinus = sTemp;
		// sun_resist
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		g_MobAttribTable[type_id-4000].resistPlus = sTemp;
		// spirit_resist
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		g_MobAttribTable[type_id-4000].resistHon = sTemp;
		// body_resist
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		g_MobAttribTable[type_id-4000].resistSin = sTemp;
		// m_attribute
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// f_attribute
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// agility
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// agility1 - 기공
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if(bLoadFromFile)	fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// defence_power
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// Skip Spell defence.. (ver 0.7 <kerygma 0630>)
		fseek( fileptr, sizeof(short), SEEK_CUR );

		// attack class
		if(fread(&ucTemp, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// attack type
		if(fread(&ucTemp, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// m_attack power min
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// 클라이언트 몬스터에서 공격력으로 사용한다.
		// m_attack power max
		if(fread(&g_MobAttribTable[type_id-4000].attackpowermax, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_attack success
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_attack success1
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_attack delay
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_attack angle
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_attack range
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_attack area
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_critical count
		if(fread(&cTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// m_critical effect
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// r_attack power min
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_attack power max
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_attack success
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_attack success1
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_attack delay
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_attack angle
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_attack range
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_attack area
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_critical count
		if(fread(&cTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// r_critical effect
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		
#ifdef _XDEF_MONSTERREF_15
		// ucAttribute
		if(fread(&ucTemp, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		g_MobAttribTable[type_id-4000].attribute = ucTemp;

		// iAttributeAttack
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#endif

		// special1_condition
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_count
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_Spell id
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_spell level
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_sp
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_delay
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_angle
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_range
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special1_area
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_condition
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_count
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_spell id
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_spell level
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_sp
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_delay
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_angle
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_range
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special2_area
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill title
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill 1 id
		if(fread(&g_MobAttribTable[type_id-4000].skill1_id, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill 1 condition
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 count
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 target
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 spell id
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 spell level
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 SP
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 delay
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 angle
		if(fread(&ucTemp, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 range
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill1 area
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill 2 id
		if(fread(&g_MobAttribTable[type_id-4000].skill2_id, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill 2 condition
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 count
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 target
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 spell id
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 spell level
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 SP
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 delay
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 angle
		if(fread(&ucTemp, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 range
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill2 area
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// model change
		if(fread(&usTemp, sizeof(unsigned short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// change prob
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// hp regain
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// sp regain
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// stamina regain
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// normal scope
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// chase scope
		if(fread(&fTemp, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// ai target
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// ai item
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// ai attack
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// ai family
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// ai escape
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// group attack
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// ai battle
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// ai siege
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// temp1
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// temp2
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// temp3
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// exp level
		if(fread(&g_MobAttribTable[type_id-4000].explevel, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return exp
		if(fread(&unTemp, sizeof(unsigned int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return skill exp
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// honor level
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return honor
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return fame1
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return fame2
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return fame3
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return gold level
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return gold min
		if(fread(&unTemp, sizeof(unsigned int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return gold max
		if(fread(&unTemp, sizeof(unsigned int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// return gold prob
		if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#ifdef _XDEF_MONSTERREF_14
		for(int i = 0 ; i < 30 ; i++)
#else
		for(int i = 0 ; i < 12 ; i++)
#endif
		{
			// drop level
			if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
			g_MobAttribTable[type_id-4000].droplevel[i] = cTemp;
			// item_type
			if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
			// item merge
			if(fread(&ucTemp, sizeof(unsigned char), 1, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
			// item rank
			if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
			// item rate
			if(fread(&nTemp, sizeof(int), 1, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
		}
		// attack prob
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// key type
		if(fread(&g_MobAttribTable[type_id-4000].key_type, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// key id
		if(fread(&g_MobAttribTable[type_id-4000].key_id, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#ifdef _XTS_MONSTERREF_12
		// respawn time min
		if(fread(&unTemp, sizeof(unsigned int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// respawn time max
		if(fread(&unTemp, sizeof(unsigned int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#else
		// respawn time
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#endif
		// monster count
		if(fread(&cTemp, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
	}

	_XMEMORYUSECHECKEND( "Loaded monster attrib" );

	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();
	return TRUE;
}