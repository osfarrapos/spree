// XProc_MainGame.cpp: implementation of the XProc_MainGame class.
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"

#ifdef _XGMCLIENT
#pragma message( "GM Client version" )
#else
#pragma message( "Client version" )
#endif

//Author : 양희왕	
#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT)
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
	#endif
#endif
//Last updated: 07/01/22

#include <mswsock.h> // 소켓 옵션 검사

#ifdef _XVIETNAMESE
	#include <psapi.h>
	#pragma comment(lib, "Psapi.Lib" )
#endif

#include "XProc_MainGame.h"
#include "XMeshContainer.h"

#include "XWindow_QuickSlot.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_Option.h"
#include "XWindow_OptionSound.h"
#include "XWindow_NPCScript.h"
#include "XWindow_PCTrade.h"
#include "XWindow_Debug.h"
#include "XWindow_ZoneName.h"

#include "XWindow_Party.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_Skill.h"
#include "XWindow_RequestParty.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_TradeConfirm.h"
#include "XWindow_AdminNotice.h"
#include "XWindow_ChargeGauge.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_Warehouse.h"
#include "XWindow_MaximumMap.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_SystemMsgDefault.h"
#include "XWindow_MainMenu.h"
#include "XWindow_Messenger.h"
#include "XWindow_MessengerDialog.h"
#include "XWindow_MessengerTooltip.h"
#include "XWindow_Email.h"
#include "XWindow_JinStatus.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_NPCSkill.h"
#include "XWindow_Rebirth.h"
#include "XWindow_RepairItem.h"// 2004.06.17->oneway48 insert
#include "XWindow_TradeConfirm.h"
#include "XWindow_NameInput.h"
#include "XWindow_TrainingMed.h"
#include "XWindow_TrainingDummy.h"
#include "XWindow_TrainingSpell.h"
#include "Xwindow_TrainingHelp.h"
#include "XMessageWindow.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_NickName.h"
#include "XWindow_SocketPlugIn.h"//2004.11.5->hotblood
#include "XWindow_Contribution.h"
#include "XWindow_Hedek.h"
#include "XWindow_Pocket.h"
#include "XWindow_Tutorial.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_HelpBook.h"	// 추후 삭제
#include "XWindowObjectDefine.h"
#include "XWindow_MainFrame.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_QuestMail.h"
#include "XWindow_Gamble.h"
#include "XWindow_EventNotice.h"
#include "XWindow_TargetInfo.h"
#include "Xwindow_MatchTablePVP.h"
#include "XWindow_MatchNoticeSmall.h"
#include "Xwindow_PVPBroker.h"
#include "XWindow_SocketPlugInExp.h"
#include "XWindow_AreaSelect.h"
#include "XWindow_NPCCollection.h"
#include "XWindow_NPCResource.h"
#include "XWindow_Inchant.h"
#include "XWindow_PaymentWindow.h"
#include "XWindow_NPCBuff.h"
#include "XWindow_BookOpen.h"
#include "XWindow_QuestRank.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_GuideLine.h"
#include "XWindow_HelpScreen.h"
#include "XWindow_NPCSearch.h"
#include "XWindow_AccountItemUse.h"
#include "XWindow_PCRPremium.h"

#include "XWindow_MoveVillage.h"
#include "XWindow_Event.h"
#include "XWindow_Escape.h"
#include "XWindow_ThreatenGauge.h"
#include "Xwindow_CBRecord.h"
#include "XWindow_Poison.h"
#include "XWindow_MixWeapon.h"
#include "XWindow_MatchEvent.h"
#include "XWindow_PK.h"
#include "XWindow_Info.h"
#include "XWindow_FreeMatchInfo.h"

#include "XWindow_MatchMessage.h"
#include "XWindow_MatchNoticeLarge.h"

// 나중에 삭제 할 것
#include "XWindow_CastleInfo.h"
#include "XWindow_ChallengerList.h"
#include "XWindow_CastleBattleRewardList.h"
#include "XWindow_CastleBattlePersonalHistory.h"
#include "XWindow_CastleBattleTimeChange.h"
#include "XWindow_PKTracingRequest.h"
#include "XWindow_BlackList.h"
#include "XWindow_NPCResource_New.h"

#ifdef _XTS_ITEMMALLBROWSER	
#include "XWindow_CashMall.h"	
#include "XWindow_CashBox.h"
#endif

#ifdef _XDEF_WEBBOARD_20061211
#include "XWindow_WebBoard.h"	
#endif

#include "XWindow_PK_ListBox.h" //Author : 양희왕
#include "XWindow_SmeltItem.h" //Author : 양희왕
#include "XWindow_AgeGrade.h" //Author : 양희왕
#include "XWindow_WorldMap.h" //Author : 양희왕
#include "XWindow_RecallItem.h" //Author : 양희왕
#include "XWindow_SocketDetach.h" //Author : 양희왕
#include "XWindow_EventIcon.h" //Author : 양희왕
#include "XWindow_NPCMnDInfo.h" //Author : 양희왕 //breif :사제 시스템
#include "XWindow_MnDTotalPoint.h" //Author : 양희왕 //breif :사제 시스템
#include "XWindow_MnDCurrentPoint.h" //Author : 양희왕 //breif :사제 시스템

// GM Control windows
#include "XWindow_GMCommand.h"
#include "XWindow_GM_SendNoticeMessage.h"


#include "XParticleList.h"
#include "XParticleIDDefine.h"

#include "SoundEffectList.h"
#include "XNPCScript.h"

#include "XSTWrapper.h"
#include "XSTWrapperCore.h"

#include "XDummy.h"
#include "XSeat.h"

#include "XSR_STRINGHEADER.H"

#include "LoadThread.h"

#include "XTextureController.h"

// by mahwang
#include "XLifeEventManager.h"

// win32 web browser api
#include "./Library/CWebPage/XWebPage.h"

#include "XDecalObject.h"
extern _XDecalObject g_TerrainDecalObject;
extern CFlashControlInterface*	g_pTitleFlashObject; //지워

extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

#ifdef  _XDEF_PERFORMANCE_CHECK				
LARGE_INTEGER frequency, count1, count2;
#endif

extern _XTimeCounter	g_SystemTimeCounter;

#define _XDEF_DBGVIEWXOFFSET		500
#define _XDEF_DBGVIEWYOFFSET		4	

#define _XDEF_DETECTSPEEDHACK_MAXDETECTEDCOUNT		15
#define _XDEF_DETECTSPEEDHACK_REPEATTIME			10000

static FLOAT _XDEF_NPCSCRIPT_DRAWPOSX = -415.0f;
static FLOAT _XDEF_NPCSCRIPT_DRAWPOSY = 157.0f;

BOOL  g_SpeedHackDetected			 = FALSE;
FLOAT g_SpeedHackCheck_PrevCheckTime = 0.0f;

DWORD g_SpeedHackCheck_timeGetTimePrevTime = 0;
DWORD g_SpeedHackCheck_SystemPrevTime = 0;
DWORD g_SpeedHackCheck_DetectedCounter = 0;
DWORD g_SpeedHackCheck_DetectFrameTime = 0;

FLOAT g_FreeMoveModeSpeed = 0.6f;

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	DWORD g_GGServerAuthReadyTime = 0;	
#endif


extern void _XInsertSystemMessage( int messagemode, TCHAR* args, ... );

void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo  )
{
	
	int strlength = strlen(message);
	if( strlength > 0 )
	{				
		_XStringItem* systemmessage = new _XStringItem( message, messageinfo , g_LocalSystemTime );
		if( systemmessage )
		{				
			if( g_CenterSystemMessageList.getitemcount() >= 3 )
			{
				g_CenterSystemMessageList.deleteItem( 0 );
			}
			
			if( !g_CenterSystemMessageList.listEmpty() )
			{
				g_CenterSystemMessageList.resetList();
				_XStringItem* currentItem = (_XStringItem*)g_CenterSystemMessageList.currentItem();
				currentItem = (_XStringItem*)currentItem->getleft();							
				if( currentItem )
				{
					if( strcmpi( currentItem->Get_String(), message ) == 0 )
						currentItem->Set_Attribute2( g_LocalSystemTime );
					else
					{
						g_CenterSystemMessageList.resetList();
						g_CenterSystemMessageList.insertItem( systemmessage );
					}
				}
			}
			else
			{
				g_CenterSystemMessageList.resetList();
				g_CenterSystemMessageList.insertItem( systemmessage );
			}
		}
	}	
}

#include < tlhelp32.h>

BOOL _XGetProcessModule(DWORD dwPID, LPCTSTR sProcessName)
{
	HANDLE        hModuleSnap = NULL; 
    MODULEENTRY32 me32        = {0}; 
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 

    if (hModuleSnap == (HANDLE)-1) 
        return FALSE; 

    me32.dwSize = sizeof(MODULEENTRY32); 
    
    if(Module32First(hModuleSnap, &me32)) 
    { 
       do 
       { 
		  if( strcmpi( me32.szModule, sProcessName ) == 0 )
          { 
			  //_XLog( "%s : %s", _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2594), me32.szExePath );

             CloseHandle (hModuleSnap); 
             return TRUE;
          } 
       } 
       while(Module32Next(hModuleSnap, &me32)); 
    } 
    CloseHandle (hModuleSnap); 
    return FALSE;
}

BOOL _XProcessKill(LPCTSTR strProcessName, LPCTSTR strWindowTitle, BOOL& found )
{
    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == (HANDLE)-1) 
        return FALSE;

    pe32.dwSize = sizeof(PROCESSENTRY32); 
    
    if (Process32First(hProcessSnap, &pe32)) 
    { 
        BOOL          bCurrent = FALSE; 
        MODULEENTRY32 me32       = {0}; 

        do 
        { 
           bCurrent = _XGetProcessModule(pe32.th32ProcessID,strProcessName);
           if(bCurrent) 
           { 
			  found = TRUE;
			  /*
			  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); 

              if(hProcess)
              {
                 if(TerminateProcess(hProcess, 0))
                 {
                    unsigned long nCode;
                    GetExitCodeProcess(hProcess, &nCode);
                 }
                 CloseHandle(hProcess); 
              }
			  */
           } 
        } 
        while (Process32Next(hProcessSnap, &pe32));
    } 
    CloseHandle (hProcessSnap); 
    return TRUE;
}

BOOL  g_szFoundKillWindow = FALSE;
TCHAR g_szFoundKillWindowPath[512];

BOOL CALLBACK _XKillWindowsProc(HWND hWnd, LPARAM lParam) 
{
	if( strlen((LPCTSTR)lParam) <= 0 )
		return FALSE; // end enumerate

	if ( !IsWindowVisible ( hWnd ) ) 
		return TRUE;		// Not a window

	TCHAR Window_Caption [ 256 ];
	
	if ( GetWindowText ( hWnd, Window_Caption, 256 ) <= 0 ) 
	{
		return TRUE; // No window title
	}

	strlwr( Window_Caption );

	if( strstr( Window_Caption, (LPCTSTR)lParam) != NULL )
	{
		g_szFoundKillWindow = TRUE;

		DWORD dwThreadId, dwProcessId;
		HINSTANCE hInstance;
		HANDLE hProcess;

		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);	
		//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);

		if(hProcess)
		{
#ifdef _XVIETNAMESE			
			if( GetModuleFileNameEx( hProcess, NULL, g_szFoundKillWindowPath, 511 ) == 0 )
			{
				
			}
#endif
			//_XLog( "%s : %s", _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2594), (LPCTSTR)lParam );

			/*
			if(TerminateProcess(hProcess, 0))
			{
				unsigned long nCode;
				GetExitCodeProcess(hProcess, &nCode);
			}
			CloseHandle(hProcess); 
			*/

			return FALSE; // end enumerate
		 }
	}	

	return TRUE;
}

BOOL _XKillWindow( LPCTSTR szWindowTitle )
{
	g_szFoundKillWindow = FALSE;
	memset( g_szFoundKillWindowPath, 0, sizeof(TCHAR) * 512 );
	TCHAR lwrtitlename[64];
	memset( lwrtitlename, 0, sizeof(TCHAR) * 64 );
	strcpy( lwrtitlename, szWindowTitle );
	strlwr( lwrtitlename );
	EnumWindows( (WNDENUMPROC)_XKillWindowsProc, (LPARAM)lwrtitlename );
	return g_szFoundKillWindow;
}

#ifdef _XDWDEBUG 

BOOL g_AutoAttackMode = FALSE;
extern BOOL CheckInnerTester( void );

BOOL _xinnertester = CheckInnerTester();

void _Process_AutoAttack( void )
{
	if( !g_AutoAttackMode ) return;
	
#ifndef _XTESTSERVER
	if( ((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_LastSelectedServer < 10 )
		return;
#endif
	
	static int _autoattacktimer = 0;
	
	if( _autoattacktimer == 0 )
	{
		_autoattacktimer = g_LocalSystemTime;
	}
	
	if( g_LocalSystemTime - _autoattacktimer > 500 )
	{
		_autoattacktimer = g_LocalSystemTime;
	}
	else
	{
		g_pLocalUser->m_bGrabItem = g_GameItemlist.GrabWorldItem();
		return;
	}
	//if( !_xinnertester )  return;
	
	if( g_pLocalUser->GetMotionClass() == _XACTION_ATTACK || g_pLocalUser->GetMotionClass() == _XACTION_MOVE )
	{
		return;
	}
	
	_XMob* pMob = NULL;
	_XMob* pNearDistMob = NULL;
	FLOAT  NearDistance = 9999.0f;
	
	for (smdef_XMob::iterator it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ it)
	{
		pMob = (*it).second;
		FLOAT dx = (pMob->m_Position.x - g_pLocalUser->m_Position.x);
		FLOAT dz = (pMob->m_Position.z - g_pLocalUser->m_Position.z);
		FLOAT _dist = _XDistance( dx , dz );
		if( _dist < NearDistance )
		{
			NearDistance = _dist;
			pNearDistMob = pMob;
		}		
	}
	
	if( !pNearDistMob )
	{
		return;
	}
	
	g_pLocalUser->SetViewTarget( (_XItem*)pNearDistMob, _XPICKINGTARGET_MOB );

	g_pLocalUser->SetViewTargetToAttackTarget();
}

#endif

DWORD CheckUseSpeedHack( void )
{	
	DWORD cursystemtick, postcursystemtick;
	SYSTEMTIME systemtime;
	
	cursystemtick = timeGetTime();
	GetSystemTime(&systemtime);	
	postcursystemtick = timeGetTime();
	cursystemtick += ((postcursystemtick - cursystemtick) >> 1);

	DWORD cursystemtime = (systemtime.wSecond * 1000) + systemtime.wMilliseconds;

	if( g_SpeedHackCheck_SystemPrevTime == 0.0f )
	{
		g_SpeedHackCheck_SystemPrevTime = cursystemtime;
		g_SpeedHackCheck_timeGetTimePrevTime = cursystemtick;
		g_SpeedHackCheck_DetectedCounter = 0;
		return g_SpeedHackCheck_DetectedCounter;
	}
	else
	{
		if( cursystemtime > g_SpeedHackCheck_SystemPrevTime )
		{
			DWORD systemtimeelapsedtime = cursystemtime - g_SpeedHackCheck_SystemPrevTime;
			DWORD systemtickelapedtime =  cursystemtick - g_SpeedHackCheck_timeGetTimePrevTime;

			_XDWINPRINT( "SH Detector [ Local %d / Tick %d / Gab %d / Detect %d ]", systemtimeelapsedtime, systemtickelapedtime, abs( systemtickelapedtime - systemtimeelapsedtime), g_SpeedHackCheck_DetectedCounter );

			if( abs( systemtickelapedtime - systemtimeelapsedtime) > 15 )
			{
				g_SpeedHackCheck_DetectedCounter++;
			}			

			g_SpeedHackCheck_SystemPrevTime = cursystemtime;
			g_SpeedHackCheck_timeGetTimePrevTime = cursystemtick;
		}
		else
		{
			g_SpeedHackCheck_SystemPrevTime = cursystemtime;
			g_SpeedHackCheck_timeGetTimePrevTime = cursystemtick;
		}
	}

	return g_SpeedHackCheck_DetectedCounter;
}

XProc_MainGame::XProc_MainGame()
{
	setProcessInfo( 0, "EOD Main game", __FILE__ );

#ifdef _XDWDEBUG 
	m_ViewRuntimeDebugData = FALSE;	
	m_ViewFPS = FALSE;
	m_ViewShadowTexture = FALSE;
	m_pLightConeMesh = NULL;
	m_LightConeTextureIndex = 0;
	m_ViewLightConeModel = FALSE;
	m_ViewPathObject = FALSE;
	m_ViewArrowModel = FALSE;
	m_ViewMouseRay = FALSE;
#endif
	m_MoveServer = FALSE;
	m_MovingGameServer = FALSE;
	m_WarpStartTime = 0;
	m_LockMoveProcess = FALSE;
	m_NextWarpZoneIndex = -2;
	m_FirstEntered = TRUE;	
	m_ToolTipEffectRepeater		= 0;
	m_ToolTipEffectRepeaterFlag = FALSE;

	m_pNPCScriptWindowPtr			= NULL;
	m_pNPCTradeWindowPtr			= NULL;
	m_pWarehouseWindowPtr			= NULL;
	m_pPCTradeWindowPtr				= NULL;
	m_pNPCSkillWindowPtr			= NULL;
	m_pSocketPlugInWindowPtr		= NULL;
	m_pMainFrameWindowPtr			= NULL;
	m_pContributionWindowPtr		= NULL;
	m_pRepairItemWindowPtr			= NULL;
	m_pPocketItemWindowPtr			= NULL;
	m_pHedekWindowPtr				= NULL;		
	m_pPersonalStoreWindowPtr		= NULL;
	m_pGambleBoxWindowPtr			= NULL;
	m_pSocketPlugInExWindowPtr		= NULL;
	m_pInchantWindowPtr				= NULL;
	m_pPVPBrokerWindowPtr			= NULL;
	m_pAddSaveItemWindowPtr			= NULL;
	m_pNPCCollectionWindowPtr		= NULL;
	m_pNPCResourceWindowPtr			= NULL;
	m_pNPCResource_New_windowptr	= NULL;
	m_pNPCBuffWindowPtr				= NULL;
	m_pMoveVillageWindowPtr			= NULL;

	m_pInfoHelperString = new _XStringItem( _T(""), 0 );

	m_bNPCDialogWideView = FALSE;
	m_NPCDialogScreenEffectFlag = TRUE;
	m_NPCDialogScreenAlphaLevel = 0;
	m_bNPCFadeInOutFlag = FALSE;
	m_TempNPCObject = NULL;
	m_bAutoNPCScriptPopUp = FALSE;
	m_bNPCDialogMode = FALSE;
	m_NPCWideViewExitTimer = 0; //2004.07.05->oneway48 insert

	
	m_bFunctionalObjectWideView			= FALSE;
	m_bAutoFunctionalObjectScriptPopUp	= FALSE;
	m_bFunctionalObjectFadeInOut		= FALSE;
	m_bExitFunctionalObjectWideView		= FALSE;
	m_TempFunctionalObject				= NULL;

	m_MessengerConnectTimer = 0;
	m_MessengerLoginTimer = 0;

	m_PCRoomConnectNoticeTimer = 0;
	
	m_fTempTargetDistance = 0;
	m_fTempFov  = 0;
	m_fTempPitch = 0;
	m_fTempYaw = 0;

	m_bNPCTalkLog = FALSE;
	m_bMoveNPCWindow = FALSE;
//	m_bMiniGameStart = FALSE;//hotblood->2004.07.27
//	m_bMiniGameDraw = FALSE;// hotblood->2004.08.13
	
	m_CurrentEnvironmentTime = 0;
	
	m_imagePoisonFogIndex	= -1;

	m_ExperiencePCRoomConnect = FALSE;

#ifdef _ACCLAIM_IGAADSYSTEM
	m_AcclaimADMode_MainGame	= _XACCLAIMAD_MAINGAME_NONE;
	m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_NONE;
	m_ADScreeenDimmedLevel		= 0;
	m_DeathAD_Gold				= 0;
	m_DeathAD_ItemCnt			= 0;
	m_pDeathAD_ADIteminfo		= NULL;

	m_UseNPCAD					= FALSE;
#endif

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	m_GameGuardAuthenticationCount = 0;
	m_CompleteGameGuardAuth = FALSE;
	m_CancleGameGuardAuth = FALSE;
#endif

}

BOOL XProc_MainGame::LoadInformationString( void )
{
	if( (g_LanguageType != _XLANGUAGE_TYPE_VIETNAMESE) && (g_LanguageType != _XLANGUAGE_TYPE_KOREAN) ) return TRUE;
	m_InformationStringList.disposeList();

	_XStringItem* pStringItem;
	TCHAR buffer[1024];
	
	FILE* infofp = NULL;
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

#ifdef _XTESTSERVER		
	infofp = fopen( "tsHelper.xsc", "rt" );
#else
	infofp = fopen( "Helper.xsc", "rt" );
#endif

	if( !infofp ) return FALSE;
	
	while( !feof(infofp) )
	{
		memset( buffer, 0, 1024 );
		fgets( buffer, sizeof(TCHAR)*1023, infofp );
		if( strlen(buffer) > 0 )
		{
			pStringItem = new _XStringItem( buffer, 0 );

			m_InformationStringList.resetList();
			m_InformationStringList.insertItem( pStringItem );		
		}
	}

	fclose( infofp );

	return TRUE;
}

XProc_MainGame::~XProc_MainGame()
{
	SAFE_DELETE( m_pInfoHelperString );
	m_InformationStringList.disposeList();

#ifdef _ACCLAIM_IGAADSYSTEM	
	SAFE_DELETE_ARRAY( m_pDeathAD_ADIteminfo );
#endif
}

void ThreadEnable(void)
{

#ifdef _XDEF_MODELTHREADLOADING
	if( !g_LoadTextureThread.m_bEnable )
	{
		g_LoadTextureThread.CreateThread();
	}

	g_LoadTextureThread.m_bEnable = TRUE;
	g_WorldObjectTextureArchive.m_bUseThread = TRUE;
	g_MobTextureArchive.m_bUseThread = TRUE;
	g_CharacterTextureArchive.m_bUseThread = TRUE;
	
	g_CharacterEffectGroupManager.m_TextureManager.m_bUseThread = TRUE;
	g_WorldEffectGroupManager.m_TextureManager.m_bUseThread = TRUE;
	

	if( !g_LoadMeshThread.m_bEnable )
	{
		g_LoadMeshThread.CreateThread();
	}

	g_LoadMeshThread.m_bEnable = TRUE;
#else

	if( !g_LoadTextureThread.m_bEnable )
	{
		g_LoadTextureThread.CreateThread();
	}

	g_LoadTextureThread.m_bEnable = TRUE;	
	g_WorldObjectTextureArchive.m_bUseThread = TRUE;
	g_MobTextureArchive.m_bUseThread = TRUE;

#endif

	_XRemoveTimer(ThreadEnable);
}

BOOL XProc_MainGame::InitializeProcess( void )
{		
	_XWindow::m_GlobalPlaySoundEffect = FALSE;

#ifdef _XDEF_MODELTHREADLOADING
	_XAddTimer(ThreadEnable, 1000, TIMER_NORMAL);
#else
	_XAddTimer(ThreadEnable, 1000, TIMER_NORMAL);
	//_XAddTimer(ThreadEnable, 20000, TIMER_NORMAL);
#endif

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	m_GameGuardAuthenticationCount = 0;
	m_CompleteGameGuardAuth = FALSE;
	m_CancleGameGuardAuth = FALSE;	
	g_GGServerAuthReadyTime = 0;
#endif

#ifdef _ACCLAIM_IGAADSYSTEM
	m_AcclaimADMode_MainGame	= _XACCLAIMAD_MAINGAME_NONE;
	m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_NONE;	
	m_DeathADRecievedResult		= FALSE;
	m_DeathADResultViewCounter	= 0;
	m_DeathADRecievedResultAutoCancelCounter = 0;
	m_DeathStaticBackgroundAlphaLevel = 0;
#endif

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.SetMADLocation( _T("In-game world") );
#endif

	_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
	if( optionmenuwindow )
	{				
		optionmenuwindow->ShowWindow( FALSE );
	}
	
	m_LastGraphUpdateTime = 0;
	m_GraphLogListInsertPoint = 0;
	for(int i = 0; i < 7; i++ )
	{
		memset( m_GraphLogList[i], 0, sizeof(DWORD) * 600 );
	}

#ifdef _XDWDEBUG 
	g_NormalGameExit = TRUE;	

	memset( g_UserTraceTargetName, 0, sizeof(TCHAR) * 13 );
	memset( g_PacketFilteringTargetName, 0, sizeof(TCHAR) * 13 );
	g_EnableUserPacketFiltering = FALSE;
	g_UserTracing = FALSE;
#else
	g_NormalGameExit = FALSE;
#endif
	
	g_MessageBox.SetBorderAlphaLevel( 96 );

	// Character move factor===================================================
	m_fMoveFactor			= 0.5f;

	g_vMousePosition		= D3DXVECTOR3( 0.f,0.f,0.f );	
	g_vPickInfinityRayDir   = g_vPickRayDir	= g_vPickRayOrig = g_vMousePosition;
	m_DuplicateCoordCount	= 0;
		
	m_LastMeshHitrateCheckTime = m_LastTextureHitrateCheckTime1 = m_LastTextureHitrateCheckTime2 = g_LocalSystemTime;
	m_LastPickedItemTime = g_fLocalSystemTime;

	// Set user informations===================================================

	m_PrevCheckedWarpingZoneTimer = 0;
	m_CurrentWarpZoneIndex = -1;
	g_CurrentHoveredWarpzoneIndex = -1;

/*
	if( g_CurrentZoneInfoIndex == 17 ) // 합비 비무 존에 들어있는 경우 플레이어 스폰 영역으로 이동 : 예외 처리
	{
		_XOBBData* tempOBBData = NULL;
		g_pLocalUser->m_NextRespawnPosition.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_NextRespawnPosition, tempOBBData );
		if( g_AreaManager.CheckInsideArea( g_pLocalUser->m_NextRespawnPosition, // 유저의 최종 위치가 비무 영역 안에 위치하면 
									       _XDEF_AREACATEGORY_PVPAREA ) )
		{
			// 안전지역(플레이어 스폰 영역)으로 이동.
			if( !g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_PLAYERSPAWN].AreaList.empty() )
			{
				_XArea* currentarea = NULL;
				list <_XArea *>::iterator iter = g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_PLAYERSPAWN].AreaList.begin();
				
				if( *iter )
				{
					currentarea = *iter;
					
					g_pLocalUser->m_NextRespawnPosition.x = currentarea->m_Point[0].x + ((currentarea->m_Point[2].x - currentarea->m_Point[0].x) / 2.0f);
					g_pLocalUser->m_NextRespawnPosition.z = currentarea->m_Point[0].z + ((currentarea->m_Point[2].z - currentarea->m_Point[0].z) / 2.0f);
					
				}
			}

			_XLog( "PVP Zone exception : moved player spawn area" );
		}
	}
*/		

	// 가상 공간 들어가기에서 다른지역 다른위치에 들어갈 떄는 들어간 후에 맵 크기를 알 수 있기 떄문에
	// 들어오고 난 후에 실제 맵에 적용되는 좌표로 바꾸어주는 작업을 한다.
	if( g_pLocalUser->m_bVirtualMode )
	{
		if(g_QuestScriptManager.m_VirtualMode == _XQUEST_VIRTUALMODE_ENTER_OTHERZONE) 
		{
			g_pLocalUser->m_NextRespawnPosition.x  = g_pLocalUser->m_NextRespawnPosition.x - g_LodTerrain.m_HalfWorldSize;
			g_pLocalUser->m_NextRespawnPosition.z  = g_pLocalUser->m_NextRespawnPosition.z - g_LodTerrain.m_HalfWorldSize;
		}
	}
	
	int	xcoord = _XFC_FloatToIntRet( ( g_pLocalUser->m_NextRespawnPosition.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	int zcoord = _XFC_FloatToIntRet( ( g_pLocalUser->m_NextRespawnPosition.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );	
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	
	if( g_LodTerrain.GetPathFindAreaNeighborInfo( zcoord, xcoord ) >= 8 ) // 갈수 없는 영역이면 마을로 강제이동...
	{
		if( !g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_PLAYERSPAWN].AreaList.empty() )
		{
			_XArea* currentarea = NULL;
			list <_XArea *>::iterator iter = g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_PLAYERSPAWN].AreaList.begin();
			
			if( *iter )
			{
				currentarea = *iter;
				
				g_pLocalUser->m_NextRespawnPosition.x = currentarea->m_Point[0].x + ((currentarea->m_Point[2].x - currentarea->m_Point[0].x) / 2.0f);
				g_pLocalUser->m_NextRespawnPosition.z = currentarea->m_Point[0].z + ((currentarea->m_Point[2].z - currentarea->m_Point[0].z) / 2.0f);				
			}

			_XLog( "Player position exception : moved player spawn area" );
		}
	}

	g_pLocalUser->m_TargetPosition = g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position = g_pLocalUser->m_NextRespawnPosition;
	
	_XOBBData* OBBData = NULL;
	// get terrain normal
	g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position, &g_pLocalUser->m_CurrentPositionTerrainNormal );
	// Get charater height
	g_pLocalUser->m_PrevPosition.y = g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, OBBData );	
	g_pLocalUser->m_pGoundCollideOBB = OBBData;
 
	g_pLocalUser->ResetAnimationState();
	g_pLocalUser->ResetProcessTime();
	g_pLocalUser->ResetZoneData();
	g_pLocalUser->MoveStartPosition();
	
	// 유저 튜토리얼 셋팅
	LoadTutorialInfo();

	// 이벤트 공지 설정 On/Off	
#ifdef _XDEF_SHOWEVENTNOTICE
	LoadEventNoticeInfo();
#endif

#ifndef _XDEF_HELPSYSTEMSERVERSAVE
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_051 )
		{
			if( g_pLocalUser->m_CharacterInfo.jinlevel == 0 && g_pLocalUser->m_CharacterInfo.innerlevel == 0)
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_051);
				g_TutorialInfo->_XTutorial_051 = TRUE;
			}			
		}
	}
#endif

	// Set the camera default data ============================================
	g_LodTerrain.m_3PCamera.Set( g_DefaultCameraFov, (float)gnWidth/(float)gnHeight, 
						      g_EnvironmentManager.m_ZValueNear_ViewPlane, 
						      g_EnvironmentManager.m_ZValueFar_ViewPlane);

	// Initialize third person view camera=====================================
	g_LodTerrain.m_3PCamera.SetAdditionalHeightMinDistance( 0.2f ); // 카메라 높이 재조정관련 파라미터를 켜줌.
	g_LodTerrain.m_3PCamera.SetPitchLimits( -85.0f, 80.0f);
	g_LodTerrain.m_3PCamera.SetRollLimits(-0, 0);
	g_LodTerrain.m_3PCamera.Set( 0, 0, 0 );
	g_LodTerrain.m_3PCamera.SetDistance( 5.0f );
	
#ifdef _XDWDEBUG
	g_LodTerrain.m_3PCamera.SetDistanceLimits(0.8f, 150.f);
#else
	
#ifdef _XMANAGER
	g_LodTerrain.m_3PCamera.SetDistanceLimits(0.8f, 20.f);
#else
	g_LodTerrain.m_3PCamera.SetDistanceLimits(0.8f, 10.f);	
#endif
	
#endif
		
	int registrycameradistance = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"_xkern_maxcameradistance", (int)g_LodTerrain.m_3PCamera.m_MaxDistance, FALSE );
	if( registrycameradistance > 150 )
		registrycameradistance = 150;

	g_LodTerrain.m_3PCamera.m_MaxDistance = registrycameradistance;

	g_LodTerrain.m_3PCamera.mp_fYaw   = 0.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
	
	D3DXVECTOR3 CameraPos;	
	// 플레이어는 처음에 -z 방향으로 보고 있기 때문에, 플레이어의 뒤통수에 카메라를 놓으려면 + 값을 더해주어야 한다.
	CameraPos.x = g_LodTerrain.m_3PCamera.m_TargetPosition.x;
	CameraPos.y = g_LodTerrain.m_3PCamera.m_TargetPosition.y + 1;
	CameraPos.z = g_LodTerrain.m_3PCamera.m_TargetPosition.z + 1;	
	g_LodTerrain.m_3PCamera.SetCameraPosition(CameraPos);

	g_LodTerrain.m_3PCamera.SetTargetPosition( 
		D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z ) );
	
	// reset camera rotate & zoom velocity...
	g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

	g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
	g_LodTerrain.RebuildLevel(FALSE);
	
	BOOL enablefpv = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEFIRSTPERSONVIEW, TRUE, TRUE );
	g_LodTerrain.m_3PCamera.SetEnableFirstPersonView( enablefpv );

	BOOL usequaterview = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEQUATERVIEW, TRUE, TRUE );
	g_LodTerrain.m_3PCamera.SetQuaterViewMode( usequaterview );

	if( g_LodTerrain.m_3PCamera.GetEnableFirstPersonView() )
	{
		g_LodTerrain.m_3PCamera.SetDistanceLimits( 0.8f, g_LodTerrain.m_3PCamera.m_MaxDistance );
	}
	else
	{
		g_LodTerrain.m_3PCamera.SetDistanceLimits( 1.85f, g_LodTerrain.m_3PCamera.m_MaxDistance );
		
		if( g_LodTerrain.m_3PCamera.m_TargetDistance < 1.85f )
			g_LodTerrain.m_3PCamera.SetDistance( 1.85f );
	}

	// Clear global effect manager=============================================
	g_WorldEffectInstanceManager.Dispose();
	g_PostProcessWorldEffectInstanceManager.Dispose();
		
	// Initialize interface data===============================================
	_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );	
	BOOL viewchatdefaultwindow = FALSE;
	POINT chatimepos;
	if( defaultchatwindow )
	{		
		POINT pos = defaultchatwindow->GetWindowPos();
		SIZE size = defaultchatwindow->GetWindowSize();
		chatimepos.x = pos.x + 96;
		chatimepos.y = pos.y + size.cy - 17;
	}
	
	m_ChatEditContainer.ResetContainer();
	m_ChatEditContainer.MoveWindow( chatimepos.x, chatimepos.y );
	m_ChatEditContainer.ResizeWindow( 260, 12 );
	m_ChatEditContainer.ShowWindow(FALSE);
	m_ChatEditContainer.SetMaxLength(80);
	m_ChatEditContainer.SetReturnKeyCallback( _XIMECallback_ChatReturn );	
	m_ChatEditContainer.SetEscapeKeyCallback( _XIMECallback_ChatEscape );
	m_ChatEditContainer.SetUpArrowKeyCallback( _XIMECallback_ChatUpArrow );
	m_ChatEditContainer.SetDownArrowKeyCallback( _XIMECallback_ChatDownArrow );
	m_ChatEditContainer.SetLeftArrowKeyCallback( _XIMECallback_ChatLeftArrow );
	m_ChatEditContainer.SetRightArrowKeyCallback( _XIMECallback_ChatRightArrow );
	m_ChatEditContainer.SetTabKeyCallback( _XIMECallback_ChatTab );
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	extern BOOL __stdcall _XIMECallback_DataChanged( _XIMEContainer* pIMEContainer, DWORD lastkey );	
	m_ChatEditContainer.SetDataChangeCallBack( _XIMECallback_DataChanged );
#endif
	m_ChatEditContainer.SetHorizScrollMode( TRUE );

	memset( m_LastSendedChatMessage, 0, sizeof(m_LastSendedChatMessage) );
	
	memset( m_LastMsg, 0, sizeof(m_LastMsg) );
	m_OverlappingMsgCount		= 0;
	m_LastSendChatMessageTime = 0;

	if( defaultchatwindow )
	{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		defaultchatwindow->m_ChatListScrollBar->SetTotalLineCount( defaultchatwindow->RecalcChattingListLength() );
#else				
		defaultchatwindow->m_ChatListScrollBar->SetTotalLineCount( g_Chatlist.getitemcount() );
#endif
		defaultchatwindow->m_ChatListScrollBar->UpdateData();
		defaultchatwindow->m_ChatListScrollBar->SetLineScrollPos( 999999 );
	}

	m_NoDisplayZoneInfo			=   FALSE;
	m_NextWarpZoneIndex_COMMONGATE	=	0;
	
	m_GameExitSignalTime		=	0;
	m_GameExitFlag				=	FALSE;
	m_bGotoLoginProcess			=	FALSE;
	m_ProgramExitFlag			=   FALSE;
	
	m_ProcessStopTime			=	0;
	m_ProcessStopFlag			=   FALSE;

	// User handler============================================================
	m_EnableAttack				=	FALSE;
	m_ProcessLocalUserDieFlag	=	TRUE;
	m_UserDieScreenEffectFlag	=   FALSE;
	m_UserDieScreenAlphaLevel	=	255;
	
	g_ScriptStarted				=	FALSE;
	g_bFunctionalObjectScriptStarted = FALSE;

	m_RotateAccelerate			=   FALSE;
	m_CameraRotateSpeed			=	0.0f;
	m_RotateYaw					=	0;
	m_RotatePitch				=	0;

	g_LMenuButtonClicked		=   FALSE;

	m_LastWeaponChangeTime		=   0;

#ifdef _XDEF_USESCREEENMOTIONBLUR 
	g_ScrrenMotionBlurMode		=	FALSE;
#endif

	InitDeviceObject();

	// Set Timer...	
	g_GameTimer.Init_All();

	m_LastKeyDownTime = 0;
		
	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );
	
	g_MessageBox.ShowWindow( FALSE );
	SetFocus( gHWnd );
	_XIMEKERNEL.SetFocus( NULL );

#ifdef _XDWDEBUG
	if( g_SinglePlayMode )
	{
		g_pLocalUser->m_CharacterCount = 1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.hair		=	0;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.face		=	1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.upperbody	=	1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.lowerbody	=	1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.lhand		=	1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.rhand		=	1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.lfoot		=	1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.rfoot		=	1;
		g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1		=	14;

		g_pLocalUser->SelectCharacter(0);

		g_pLocalUser->m_CharacterInfo.Set_max_lifepower( 1000 );
		g_pLocalUser->m_CharacterInfo.Set_max_forcepower( 1000 );
		g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower( 1000 );		

		g_pLocalUser->m_CharacterInfo.Set_current_lifepower(1000);
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower(1000);
		g_pLocalUser->m_CharacterInfo.Set_current_concentrationpower(1000);
		
		g_pLocalUser->m_PrevPosition.x = g_pLocalUser->m_Position.x = 326-256;
		g_pLocalUser->m_PrevPosition.z = g_pLocalUser->m_Position.z = 420-256;
		
		g_pLocalUser->UpdatePosition();
		g_pLocalUser->m_PrevPosition.y = g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
		g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
															   g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
															   g_pLocalUser->m_Position.z ) );
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.RebuildLevel(FALSE);
		
		_XUser::CreateIntersectionPoly();

		/*
		int aaa = 0;
		for( int i = 1; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
		{
			if( g_MOBMODEL[i]->GetNodeCnt() > 0 )
			{
				_XMob* pMob = new _XMob;
				
				pMob->m_MobID = i;
				
				pMob->m_Position.x = aaa * 2 ;// - (FLOAT)_XRand( 200l );
				pMob->m_Position.y = 0.0f;
				pMob->m_Position.z = 110.0f;// - (FLOAT)_XRand( 200l );
				
				pMob->m_EndPosition.x = aaa * 2 ;// - (FLOAT)_XRand( 200l );
				pMob->m_EndPosition.y = 0.0f;
				pMob->m_EndPosition.z = 60.0f;// - (FLOAT)_XRand( 200l );
				
				pMob->m_HealthPercentage = 100;
				
				// Find mob kind...				
				pMob->m_MobType = i;
				
				
				pMob->m_ModelDescriptor.ReserveModelStack( 1 );
				pMob->m_ModelDescriptor.LinkBipedObject( &g_MOBBIPEDMODEL[i] );
				pMob->m_ModelDescriptor.SetMeshModel( 0, g_MOBMODEL[i] );
				pMob->SetAnimation( _XMOBACTION_MOVE );
				pMob->m_ModelDescriptor.PlayAnimation();
				
				pMob->m_fMoveSpeed = g_MobAttribTable[ i ].movespeed_walk;
				
				// 이동 방향 노멀벡터를 생성
				pMob->m_MoveVector	 = pMob->m_EndPosition - pMob->m_Position;
				D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
				
				pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				pMob->m_PrevTime = g_LocalSystemTime;
				
				g_Moblist.AddMob( pMob );
				
				aaa++;
			}
		}

		_XGameItem* newItem;
		D3DXVECTOR3 position;
		FLOAT posv = 0;
		for( i = 1; i < _XDEF_ITEMMODELTYPECOUNT_WEAPON; i++ )
		{
			if( g_ItemModel_Weapon[0][i]->GetNodeCnt() )
			{
				float temp , dx, dy, dz;
				
				temp = g_ItemModel_Weapon[0][i]->OBB_Extent[1] ;
				g_ItemModel_Weapon[0][i]->OBB_Extent[1] = g_ItemModel_Weapon[0][i]->OBB_Extent[2];
				g_ItemModel_Weapon[0][i]->OBB_Extent[2] = temp;
				
				dx = g_ItemModel_Weapon[0][i]->OBB_Extent[0] / 2.0f;
				dz = g_ItemModel_Weapon[0][i]->OBB_Extent[2] / 2.0f;
				dy = g_ItemModel_Weapon[0][i]->OBB_Extent[1] ;
				g_ItemModel_Weapon[0][i]->m_ObjectRadius = _XFC_sqrt( dx*dx + dy*dy + dz*dz );
				
				newItem = new _XGameItem;
				
				position.x = 50.0f + posv;			//(FLOAT)_XRand( 50L ) - 25.0f;
				position.z = 100.0f;				//(FLOAT)_XRand( 50L ) - 25.0f;
				
				position.y = g_LodTerrain.GetTerrainHeight( position ) + 0.05f;
				newItem->Create( 0, position, 0.0f, g_ItemModel_Weapon[0][i] );
				
				newItem->m_cType = _XGI_FC_WEAPON;
				newItem->m_sID = i;
				newItem->m_ucCount = 10;
				
				CsuX3D_Box	obbbox;
				obbbox.Create( newItem->m_pMeshContainer->OBB_Center, newItem->m_pMeshContainer->OBB_Axis, newItem->m_pMeshContainer->OBB_Extent );
				newItem->m_pMeshContainer->m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
				
				g_GameItemlist.insertItem( newItem );
				
				posv+=0.5f;
			}
		}
		*/
		

		// Test...
		int cnt = 0;				
		int itemid1[] = { 1,4, 7,16,31,49,70,91,246,249,252,261,276,294,315,336 };	// 상의 
		int itemid2[] = { 516,519, 522,528,540,552,567,582, 704,707, 710, 716, 728, 740, 755, 770  };//하의
		int weaponitemid[] = { 3001,3010,3019,3034,3067,3100,3133,3166,3211,3262,3319,3376,3439,3508,3577,3640,3709,3790,3871,3946,4033,4102,4159,4222,4297 };// 타격
		//int itemid1[] = { 2449, 2458, 2473, 2491, 2512, 2533,2557,2578, 2694, 2703,2719,2736,2757,2778,2802,2823  };//개방 상의
		
		for( int iter = _XINVENTORY_SLOTTYPE_POCKETSTART; iter < _XINVENTORY_SLOTTYPE_POCKETSTART+16; iter++ )
		{
			g_pLocalUser->m_UserItemList[iter].Set_m_cType(_XGI_FC_CLOTHES);
			g_pLocalUser->m_UserItemList[iter].Set_m_sID( itemid1[cnt++] );
			g_pLocalUser->m_UserItemList[iter].Set_m_ucCount(1);
		}

		cnt = 0;
		for( iter = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; iter < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+14; iter++ )
		{
			g_pLocalUser->m_UserItemList[iter].Set_m_cType(_XGI_FC_CLOTHES);
			g_pLocalUser->m_UserItemList[iter].Set_m_sID( itemid2[cnt++] );
			g_pLocalUser->m_UserItemList[iter].Set_m_ucCount(1);
		}

		cnt = 0;
		for( iter = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+14; iter < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+20; iter++ )
		{
			g_pLocalUser->m_UserItemList[iter].Set_m_cType(_XGI_FC_WEAPON);
			g_pLocalUser->m_UserItemList[iter].Set_m_sID( weaponitemid[cnt++] );
			g_pLocalUser->m_UserItemList[iter].Set_m_ucCount (1);
		}
		 
		/*
		int skillindex = 0;
		for(  int i = 1 ; i < _XDEF_MAXSKILLCOUNT+1 ; i++)
		{
			// 임시로 개방 무공 setting
			if(g_SkillProperty[i]->clanType == _XGROUP_GAEBANG)
			{
				g_pLocalUser->m_SkillList[skillindex].m_sSkillID = i;
				g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel = 11;
				skillindex++;
			}
		}
				
		_XWindow_Skill* pSkill_Window = (_XWindow_Skill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SKILL);
		if(pSkill_Window)
		{
			pSkill_Window->SetListItem();
		}
		*/
	}
	
	// Light position indicator	
	if (FAILED( D3DXCreateCylinder(gpDev, 0.0f, 0.15f, 0.30f, 10, 10, &m_pLightConeMesh, NULL) ) )
        return false;	
#endif

	m_SendedRequestRestart = FALSE;
	m_FirstEntered = FALSE; // 페이드 아웃 쪽에서 처리	

	g_TerrainTextureArchive.ResetTextureCacheTime();	
	g_WorldObjectTextureArchive.ResetTextureCacheTime();
	g_EffectTextureArchive.ResetTextureCacheTime();	
	g_CharacterTextureArchive.ResetTextureCacheTime();
	g_MobTextureArchive.ResetTextureCacheTime();
//	g_MobTextureArchive.m_bTextureCache = FALSE;
	
	g_pLocalUser->m_ModelDescriptor.InitEffectManager(1);
	g_pLocalUser->m_ModelDescriptor.m_EnableEffectProcess = TRUE;
	g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
	
	g_WarpProcessFadeInOutFlag	= FALSE;
	g_DrawFadeInOutFlag			= FALSE;	
	m_WarpStartTime				= 0;

	D3DCOLOR fadecolor = D3DCOLOR_ARGB(255, 0, 0, 0);
	m_TopScreenVertex[0]=_XTLVERTEX(0.0f, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);
	m_TopScreenVertex[1]=_XTLVERTEX((FLOAT)gnWidth, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);
	m_TopScreenVertex[2]=_XTLVERTEX(0.0f, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);
	m_TopScreenVertex[3]=_XTLVERTEX((FLOAT)gnWidth, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);
	
	m_BottomScreenVertex[0]=_XTLVERTEX(0.0f, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);
	m_BottomScreenVertex[1]=_XTLVERTEX((FLOAT)gnWidth, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);
	m_BottomScreenVertex[2]=_XTLVERTEX(0.0f, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);
	m_BottomScreenVertex[3]=_XTLVERTEX((FLOAT)gnWidth, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, fadecolor, 0, 0.0f, 0.0f);

	m_GridTopScreenVertex[0]=_XTLVERTEX(0.0f, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(255, 0, 0, 0), 0, 0.0f, 0.0f);
	m_GridTopScreenVertex[1]=_XTLVERTEX((FLOAT)gnWidth, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(255, 0, 0, 0), 0, 0.0f, 0.0f);
	m_GridTopScreenVertex[2]=_XTLVERTEX(0.0f, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(0, 0, 0, 0), 0, 0.0f, 0.0f);
	m_GridTopScreenVertex[3]=_XTLVERTEX((FLOAT)gnWidth, 0.0f, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(0, 0, 0, 0), 0, 0.0f, 0.0f);
	
	m_GridBottomScreenVertex[0]=_XTLVERTEX(0.0f, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(0, 0, 0, 0), 0, 0.0f, 0.0f);
	m_GridBottomScreenVertex[1]=_XTLVERTEX((FLOAT)gnWidth, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(0, 0, 0, 0), 0, 0.0f, 0.0f);
	m_GridBottomScreenVertex[2]=_XTLVERTEX(0.0f, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(255, 0, 0, 0), 0, 0.0f, 0.0f);
	m_GridBottomScreenVertex[3]=_XTLVERTEX((FLOAT)gnWidth, (FLOAT)gnHeight, 0.0f, _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(255, 0, 0, 0), 0, 0.0f, 0.0f);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Interface window and skill window precacheing	
	m_pNPCScriptWindowPtr		= (_XWindow_NPCScript *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
	m_pNPCTradeWindowPtr		= (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
	m_pWarehouseWindowPtr		= (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	m_pPCTradeWindowPtr			= (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	m_pNPCSkillWindowPtr		= (_XWindow_NPCSkill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSKILL);
	m_pSocketPlugInWindowPtr	= (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
	m_pContributionWindowPtr	= (_XWindow_Contribution*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CONTRIBUTIONWINDOW );
	m_pRepairItemWindowPtr		= (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM);
	m_pMainFrameWindowPtr		= (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
	m_pPocketItemWindowPtr		= (_XWindow_Pocket*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POCKET);
	m_pHedekWindowPtr			= (_XWindow_Hedek*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_HEDEK );
	m_pPersonalStoreWindowPtr	= (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
	m_pGambleBoxWindowPtr		= (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
	m_pSocketPlugInExWindowPtr	= (_XWindow_SocketPlugInExp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINEXP);
	m_pInchantWindowPtr			= (_XWindow_Inchant*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANT);	
	m_pPVPBrokerWindowPtr		= (_XWindow_PVPBroker*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PVPBROKER);
	m_pAddSaveItemWindowPtr		= (_XWindow_AddSaveItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ADDSAVEITEM);
	m_pNPCCollectionWindowPtr	= (_XWindow_NPCCollection*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCCOLLECTION);
	m_pNPCResourceWindowPtr		= (_XWindow_NPCResource*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCRESOURCE);
	m_pNPCResource_New_windowptr = (_XWindow_NPCResource_New*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NEW_RESOURCEWINDOW);
	m_pNPCBuffWindowPtr			= (_XWindow_NPCBuff*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCBUFF);
		
#ifdef _XTS_MOVEVILLAGE
	m_pMoveVillageWindowPtr		= (_XWindow_MoveVillage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MOVEVILLAGE);
#endif
	
	
	// 2004.06.01->oneway48 insert
	if( g_pInventory_Window )
	{
		if( g_pInventory_Window->GetShowStatus() )
			g_pInventory_Window->ShowWindow(FALSE);
	}
	// insert end

	// ------------=
	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pLastPickedOBB );
	if( pLastPickedOBB )
	{
		g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = height;
		g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;		
	}
	
	g_pLocalUser->UpdatePosition();
	
// pk범 위치 추적 window 띄우기
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK범위치추적, 레벨차 PK제한	
	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if( pRebirth_Window->m_bEnablePKTracing )
	{			
		_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
		if(pPKTracingRequest_Window)
		{
			if( pPKTracingRequest_Window->m_cTargetName[0] != 0 )
				pPKTracingRequest_Window->ShowWindow(TRUE);
			
			pRebirth_Window->m_bEnablePKTracing = FALSE;
		}
	}
#endif
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// 지형 관련 Parameter
	g_LodTerrain.m_3PCamera.SetCameraShakeMode( FALSE );

	//////////////////////////////////////////////////////////////////////////////////////////
	// Messenger 관련 Parameter
	m_MessengerConnectTimer = 0;	
	m_MessengerLoginTimer = 0;

	//////////////////////////////////////////////////////////////////////////////////////////
	// NPC 관련 Parameter
	m_bNPCDialogWideView = FALSE;		// 대화모드로 전환되는 시점 - Fade In되기 시작할 때부터 
	m_NPCDialogScreenEffectFlag = TRUE;
	m_NPCDialogScreenAlphaLevel = 0;
	m_bNPCFadeInOutFlag = FALSE;
	m_TempNPCObject = NULL;
	m_bAutoNPCScriptPopUp = FALSE;
	m_bNPCDialogMode = FALSE;			// 대화모드로 들어간 시점 - Fade In,Out된 후 와이드 화면일 때부터
	g_LastNPCScriptOwner = NULL;
	
	if( m_pWarehouseWindowPtr )
		m_pWarehouseWindowPtr->m_bWarehouseFirstLoad = FALSE; // 서버간 이동후에 창고정보를 다시 받아오도록 한다.

	m_NPCWideViewExitTimer = 0; //2004.07.05->oneway48 insert
	
	m_NPCWideViewMove = 0;
	m_NPCWindowMoveFast = 0;
	m_NPCWindowMoveSlow = 0;
	
	m_bNPCTalkLog = FALSE;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONAL OBJECT 관련 Parameter
	g_LastFunctionalObject = NULL;
	
	m_bFunctionalObjectWideView			= FALSE;
	m_bAutoFunctionalObjectScriptPopUp	= FALSE;
	m_bFunctionalObjectFadeInOut		= FALSE;
	m_bExitFunctionalObjectWideView		= FALSE;
	m_TempFunctionalObject				= NULL;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 도움말 시스템 초기화
	m_CurrentStringWidth	= 0;
	m_InfoHelperScrollTimer = 0;
	m_InfoHelperInsertTimer = 0;
	m_InfoHelperStartTimer  = 0;
	m_LastInsertedInformationStringIndex = 0;
	m_CurrenStringScrollEffectProgress = 0.0f;
	m_CurrenStringScrollEffectDirection = 2;	// 0이면 enter 1이면 fade 2 면 고정 
	m_CurrenStringScrollEffectPosition = 0;
	if( m_pInfoHelperString )
		m_pInfoHelperString->Set_String( _T("") );
	m_InfoHelperSystemView  = TRUE;

	g_SpeedHackCheck_PrevCheckTime = g_fLocalSystemTime;

	m_pPCTradeWindowPtr->ResetTradeInfo(); // 2004.05.25->oneway48 insert
	m_pPersonalStoreWindowPtr->ResetTradeInfo();

	InitializeTraining();
	InitializeInterface();

	// 캡춰 파일 폴더를 현재 캐릭터에
	_XSetCaptureFileSubFolder( g_pLocalUser->m_CharacterInfo.charactername );

	m_CurrentBGMType=   0;
	m_ChangingBGM	=	0;
	m_SpecialBGMPlayMode = FALSE;

#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
	m_PreviousBGMPlayFlag = g_BgmPlayFlag;
	m_SpecialBGMLoopPlayFlag = FALSE;
	m_SpecialBGMPlayTimer = 0;
#endif

	memset( m_SpecialBGMFileName, 0, sizeof(TCHAR) * 64 );
	g_BgmPlayFlag	= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, TRUE, TRUE );
	
#ifdef _XUSEFMOD
	if( g_BgmPlayFlag )
	{
		g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename1 );		
		m_BGMPlayTimer	=	g_LocalSystemTime;
	}
	else
	{
		//g_FMODWrapper.StopBGM();
		g_FMODWrapper.PlayBGM( _T("null") );
		m_BGMPlayTimer	=	g_LocalSystemTime;
	}
#else
	m_BGMPlayTimer	=	g_LocalSystemTime + 170000;

	if( g_pBGMSoundObject[1] ) 
	{
		if( !g_pBGMSoundObject[1]->IsPlaying() )
		{
			g_pBGMSoundObject[1]->Stop();
		}
		g_pBGMSoundObject[1]->Destroy();
		g_pBGMSoundObject[1] = NULL;
	}
	
	SoundInit initsound;
	initsound.m_bLooping = false;
	initsound.m_bStreaming = false;
	initsound.m_bMusic = true;
	
	initsound.m_sFileName = _T( gModulePath );
	initsound.m_sFileName += _T("\\Data\\Sound\\BGM\\");
	initsound.m_sFileName += _T(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename1);
							
	if(!AudioMgr()->CreateSound(g_pBGMSoundObject[1])) return false;
	if(!g_pBGMSoundObject[1]->Init(initsound))
	{
		_XFatalError("Error initializing Second BGM file [%s]", initsound.m_sFileName );
		return FALSE;
	}

	if( g_pBGMSoundObject[2] ) 
	{
		if( !g_pBGMSoundObject[2]->IsPlaying() )
		{
			g_pBGMSoundObject[2]->Stop();
		}
		g_pBGMSoundObject[2]->Destroy();
		g_pBGMSoundObject[2] = NULL;
	}

	initsound.m_sFileName = _T( gModulePath );
	initsound.m_sFileName += _T("\\Data\\Sound\\BGM\\");
	initsound.m_sFileName += _T(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename2);
							
	if(!AudioMgr()->CreateSound(g_pBGMSoundObject[2])) return false;
	if(!g_pBGMSoundObject[2]->Init(initsound))
	{
		_XFatalError("Error initializing Second BGM file [%s]", initsound.m_sFileName );
		return FALSE;
	}
#endif

//	g_LifeEventManager.LoadLifeEventItem();

	// Set environment....
	g_LodTerrain.m_nCurEnvironmentType = 999;
	g_EnvironmentManager.SetChangeEnvironmentTime( m_CurrentEnvironmentTime );
	
	// reset minimap info...
	if( g_pMinimap_Window )
	{
		g_pMinimap_Window->ShowWindow(TRUE);
		g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );
		g_pMinimap_Window->SetLocalUserPosition();
		g_pMinimap_Window->SetCheckLocalUserPosition(g_pLocalUser->m_Position.x,g_pLocalUser->m_Position.z);		
		g_pMinimap_Window->RebuildDrawNPCList( g_pLocalUser->m_Position.x,g_pLocalUser->m_Position.z );
		g_pMinimap_Window->SetMode( g_pMinimap_Window->GetMode() );
		g_pMinimap_Window->RebuildCurrentTimeIcon();		
	}
	
	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
	// 보류
	/*
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_029 )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_029);
			g_TutorialInfo->_XTutorial_029 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
		}
	}*/
	// ----------------------------------------------------------------------------------------------------------------------------=
	
	m_imagePoisonFogIndex = g_MainInterfaceTextureArchive.FindResource("PoisonFog.tga");
//	m_imagePoisonFog = new _XImageStatic;
//	m_imagePoisonFog->Create(0, 0, gnWidth, gnHeight, &g_MainInterfaceTextureArchive, resourceindex );
//	m_imagePoisonFog->SetScale( 4.0f, 3.0f );

	// 숫자 - 혈수/연타 표시
	int resourceindex_2 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_02.tga");
	}
	else 
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("MI_unki_02.tga");
	}	

	for( int _iter = 0 ; _iter < 12 ; _iter++)
	{
		m_NumberSmallImage[_iter].Create(0, 0, 11, 17, &g_MainInterfaceTextureArchive, resourceindex_2);
	}
	m_NumberSmallImage[0].SetClipRect(124, 226, 124+11, 226+17);
	m_NumberSmallImage[1].SetClipRect(2, 226, 2+11, 226+17);
	m_NumberSmallImage[2].SetClipRect(13, 226, 13+11, 226+17);
	m_NumberSmallImage[3].SetClipRect(27, 226, 27+11, 226+17);
	m_NumberSmallImage[4].SetClipRect(41, 226, 41+11, 226+17);
	m_NumberSmallImage[5].SetClipRect(55, 226, 55+11, 226+17);
	m_NumberSmallImage[6].SetClipRect(69, 226, 69+11, 226+17);
	m_NumberSmallImage[7].SetClipRect(83, 226, 83+11, 226+17);
	m_NumberSmallImage[8].SetClipRect(96, 226, 96+11, 226+17);
	m_NumberSmallImage[9].SetClipRect(110, 226, 110+11, 226+17);
	m_NumberSmallImage[10].SetClipRect(136, 226, 136+11, 226+17); // '/'
	m_NumberSmallImage[11].SetClipRect(147, 226, 147+11, 226+17); // ':'

	//g_ScreenMotionBlurEffectManager.Enable( TRUE );
	//g_ScreenMotionBlurEffectManager.SetBlurAlpha(0.7f);
	
	// 메신져 접속
	if( !g_NetworkKernel.m_bMessengerServerConnected   )
	{
		
		_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
		pMessenger_Window->m_CurrentUserStatus = 0;
		
		_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
		if(pMainMenu_Window)
		{
			pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
		}
		pMessenger_Window->ReleaseMessengerList();
		
		g_NetworkKernel.InitializeMessengerSocket();
		g_NetworkKernel.m_ConnectTryCount++;
		m_MessengerConnectTimer = 0;
	}
	else
	{			
		WSAAsyncSelect(g_NetworkKernel.m_hMessengerSocket, gHWnd, WM_USER_MESSENGERSOCKET, FD_READ | FD_CLOSE );
	}
	
	if(g_pLocalUser->m_CharacterInfo.Get_gm_level() >= 2)
	{
		// GM - 자동으로 투명 기능 켜져있음
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2462), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
	}

	_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = (_XWindow_MatchNoticeSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICESMALL);
	if(pMatchNoticeSmall_Window)
	{
		if(pMatchNoticeSmall_Window->GetShowStatus())
			pMatchNoticeSmall_Window->ShowWindow(FALSE);
	}
	
	_XWindow::m_GlobalPlaySoundEffect = TRUE;
	
	m_Initialized = TRUE;
	m_PCRoomConnectNoticeTimer = g_LocalSystemTime;

	_XWindow* pMessageBox = NULL;
	do 
	{
		pMessageBox = g_MainWindowManager.FindWindow(_XDEF_WTITLE_BILLINGTIMOUTMESSAGEWINDOW);
		if( !pMessageBox )
		{
			break;			
		}

		g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_BILLINGTIMOUTMESSAGEWINDOW );		
	} while(1);

	if( !g_pLocalUser->m_bIsBossBattle )//보스전이 아니면 서버이동 
	{
		if( !g_pLocalUser->m_bBossBattleEndProcess ) // 보스전이 끝나고 나올때도 이 세팅은 하지 않는다.
		{	
			if( !g_pLocalUser->m_bVirtualMode )	// 가상의 공간에서 진행하는 모드가 아닐때 
			{
				m_MoveServer = FALSE; // 이플래그를 꺼서 패킷처리가 가능케 한다.
				m_MovingGameServer = FALSE;	// 이동 완료...
				m_LockMoveProcess = FALSE;	
				g_LodTerrain.m_bInvisiblePortalMouseView = FALSE;
				g_ClickedWarpZoneIndex = -1;
				g_ViewPoisonFog = FALSE;

				// 지역 세율 초기화
				g_nMapTaxRate			= 0;		// 보스전이나 가상 공간에서 나올 경우 초기화 하지 않음
				
				g_pLocalUser->ResetSkillAnimation();
				
				// 영웅단 초기화
				if( g_pGroup_Window )
				{
					g_pGroup_Window->ResetData();
					if( g_pInventory_Window->GetShowStatus() )
						g_pInventory_Window->ShowWindow(TRUE);
				}
				
				g_NetworkKernel.SendPacket( MSG_NO_CHARAC_GAMESTARTSIGNAL );			
				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 1 );// 로그인이라는 상태를 알려준다.
				
				if(g_pLocalUser->m_InParty)
				{
					// 2004.06.01->oneway48 modify : 초기화하고 다시 받음
					for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) // 2004.06.01->oneway48 modify : 15->9
					{
						memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
						g_pLocalUser->m_PartyInfo[i].serverno = g_pLocalUser->m_PartyInfo[i].posX = g_pLocalUser->m_PartyInfo[i].posZ = 0;
						g_pLocalUser->m_PartyInfo[i].innerlevel = g_pLocalUser->m_PartyInfo[i].vitalpercent = g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
						g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
					}
				}

#ifdef _XDEF_SHUTDOWN_20061027
				_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
				if( pPaymentMethod_Window )
					pPaymentMethod_Window->ShowWindow(TRUE);
#endif

				_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
				if( !pPCRPremium_Window->GetShowStatus() ) 
				{
					if( g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREMIUM ) || g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREPAID ) )
						pPCRPremium_Window->ShowWindow( TRUE );
				}
				
				if( g_CurrentZoneInfoIndex == 21 )// 패왕릉에서는 독무 아이콘이 표시된다.
				{				
					_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
					if(pPoison_Window)
					{
						pPoison_Window->SetMode(_MODE_PAEWANG);
						pPoison_Window->ShowWindow(TRUE);
					}
				}
				else
				{
					_XWindow_Poison* pPoison_window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
					if(pPoison_window)
					{
						pPoison_window->SetMode(_MODE_NONE);
						pPoison_window->ShowWindow(FALSE);
					}
				}
				
				
				if(g_CurrentZoneInfoIndex == 17)	//합비
				{
					if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= 25)	// 옥동 1성 이상
					{
						_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
						if(pRequestParty_Window)
						{
							pRequestParty_Window->m_RequestPVPButton->EnableWindow(TRUE);
						}
						
						if(!g_DenyMatch)
						{
							if( g_LanguageType != _XLANGUAGE_TYPE_JAPANESE ) //Author : 양희왕 //breif : 비무목록 막기 일본만 막음
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2461), TRUE, _XDEF_MATCH_REGISTRATION, 0);
								pMessageBox->StartTimer(30000, TRUE);
							}
						}
						if( g_pLifeSkill_Window )
							g_pLifeSkill_Window->m_btnPVPPR->EnableWindow(TRUE);
					}
				}
				else
				{
					_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
					if(pRequestParty_Window)
					{
						pRequestParty_Window->m_RequestPVPButton->EnableWindow(FALSE);
					}
					if( g_pLifeSkill_Window )
						g_pLifeSkill_Window->m_btnPVPPR->EnableWindow(FALSE);
					
				}
			}
			else // 가상공간 모드 일때 
			{
				switch(g_QuestScriptManager.m_VirtualMode ) {
				case _XQUEST_VIRTUALMODE_ENTER_OTHERZONE:
					{
						//가상모드로 진입
						SetEnterVirtualSpace();
					}
					break;
				case _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONE:
				case _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONEOTHERPOSITION:
				case _XQUEST_VIRTUALMODE_LEAVE_OTHERZONEOTHERPOSITION:
					{
						//가상모드에서 탈출 
						SetLeaveVirtualSpace();
					}
					break;
				}
			}
		}
		else
		{
			// 인스턴스 던전인 생사결에서 나와서 필요한 것들을 새로 설정해 주어야 함.
			SetExitBossBattleRoom();

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD) // 보스전이 끝난 후에도 서버에서 인증패킷이 안오므로 서버 인증 안함.
			m_GameGuardAuthenticationCount = 2;
			m_CompleteGameGuardAuth = TRUE;
			m_CancleGameGuardAuth = FALSE;
#endif
		}
	}
	else // 보스전이면 인증키 전송
	{
		// 인스턴스 던전(생사결)에 들어오면 설정해야 함.( 일반 게임 진입과는 다름. )
		SetEnterBossBattleRoom();		
	} 

	_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
	if(pEnterBossRoom_Window)
	{
		pEnterBossRoom_Window->ShowWindow(FALSE);
		pEnterBossRoom_Window->ResetNeedItemInfo();
	}
	
	// 해상도가 변경되었으며 UI를 재배치한다.
	BOOL changedscreenmode = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_CHANGEDSCREENMODE, FALSE, FALSE );

	if( changedscreenmode )
	{
		_XLog( _T("Resolution changed") );
		g_MainWindowManager.SetDefaultPosition();		
		g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHANGEDSCREENMODE, FALSE );
	}

	if( g_pInventory_Window )
	{
		g_pInventory_Window->ShowWindow( TRUE );
		g_pInventory_Window->ShowWindow( FALSE );
	}
	
	// map에 들어갈 때 들어가는 시간 기록
	m_dwMapEnteringTime = g_LocalSystemTime;

#ifdef _XTS_ITEMMALLBROWSER
	if( g_bItemMallLogOff )
	{
		g_bItemMallLogOff = FALSE;
#ifdef _XKOREAN
		// 아이템 몰 로그 오프 시킨다..
		if( !g_bTestWebPageAddress )
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com/logoff.asp");
		else
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://panda.nexon.com/logoff.asp");
#endif
		
#ifdef _XENGLISH 
	//	DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com/logoff.asp");	
#endif
		
#ifdef _XVIETNAMESE
	//DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://www.cuulong.com.vn/ktc/logout.aspx");	
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulongid.zing.vn/ktc/logout.aspx");	
#endif
	
#ifdef _XJAPANESE
	//DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninetest.gameleon.jp/item_mall/item_logout.asp");
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://nine.gameleon.jp/item_mall/item_logout.asp");
#endif	

#ifdef _XTAIWANESE
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://member.pfamily.com.tw/9ds_shopping_mall/logout.php");
#endif

	}
#endif	


//	Author : 양희왕
#ifdef _XDEF_NOTICE_STRING_PROCESS
	m_pXNoticeString = _XNoticeProcess::GetInstance();
	m_pXNoticeString->SetNoticeProfile( _T("Notice.txt") );
#endif
//	Last updated: 07/01/10


	_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
	_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);

	if(pPCRPremium_Window)
	{
		// pc방일 경우 window 보여주기
		if( g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREMIUM ) || g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREPAID ) )
		{
			pPCRPremium_Window->ShowWindow(TRUE);
		}
	}

#ifdef _XDEF_SHUTDOWN_20061027
	//구룡천하 유저일 경우 베트남셧다운 위치를 변경
	if( g_pLocalUser->m_AccountItemsID > 0 )
	{
		if( pPaymentMethod_Window)
			pPaymentMethod_Window->MoveWindow( 82, 93 );

		if(pPCRPremium_Window)
			pPCRPremium_Window->MoveWindow(82, 113);
	}
	else
	{
		if( pPaymentMethod_Window)
			pPaymentMethod_Window->MoveWindow( 82, 73 );

		if(pPCRPremium_Window)
			pPCRPremium_Window->MoveWindow(82, 93);
	}
#else

	if( g_pLocalUser->m_AccountItemsID > 0 )
	{
		if(pPCRPremium_Window)
			pPCRPremium_Window->MoveWindow(82, 93);
	}
	else
	{
		if(pPCRPremium_Window)
			pPCRPremium_Window->MoveWindow(82, 73);
	}

#endif
	
	
#ifdef _XDEF_ZONECHATTING_20070108		
	if( g_bCommunityOptionDisableZoneChatting )
	{
		defaultchatwindow->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(FALSE);	//지역대화 disable
	}
	else
	{
		defaultchatwindow->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);	//지역대화 enable
	}
#endif

#ifdef _XDEF_CASTLEBATTLE
	// 장원전 초기화
	g_CBManager.ReleaseCastleInfo();
	g_CBManager.Reset();
	g_pLocalUser->ResetCastleBattleInfo();

	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		if(pCBPersonalHistory_Window->GetShowStatus())
			pCBPersonalHistory_Window->ShowWindow(FALSE);
	}
	
	_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
	if(pThreatenGauge_Window)
	{
		if(pThreatenGauge_Window->GetShowStatus())
			pThreatenGauge_Window->ShowWindow(FALSE);
	}

	#ifdef _XDEF_CASTLEBATTLE_RECORD
		// 장원 전적
		_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
		if(pCBRecord_Window)
		{
			// 이 전 data clear
			pCBRecord_Window->DeleteAll();

			if(pCBRecord_Window->GetShowStatus())
				pCBRecord_Window->ShowWindow(FALSE);
		}
	#endif
		
	#ifdef _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI
		_XWindow_Info* pInfo_Window = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
		if(pInfo_Window && pInfo_Window->GetShowStatus())
		{
			pInfo_Window->ShowWindow(FALSE);
		}
	#endif

#endif

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		// 비무 대회 정보 초기화
		g_pLocalUser->m_bEnterMatchEvent = FALSE;
		g_pLocalUser->m_nKillCount = g_pLocalUser->m_nDeadCount = 0;
		
		if(g_pRewardNick_Window)
		{
			if(g_pRewardNick_Window->GetShowStatus())
				g_pRewardNick_Window->ShowWindow(FALSE);
		}

		_XWindow_MatchEvent* pMatchEvent_Window = (_XWindow_MatchEvent*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHEVENT);
		if(pMatchEvent_Window && pMatchEvent_Window->GetShowStatus())
		{
			pMatchEvent_Window->ShowWindow(FALSE);
		}

		_XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
		if(pPK_Window && pPK_Window->GetShowStatus())
		{
			pPK_Window->ShowWindow(FALSE);
		}
#endif
	
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	_XWindow_BlackList* pBlackList_Window = (_XWindow_BlackList*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_BLACKLISTWINDOW );
	pBlackList_Window->LoadList( g_pLocalUser->m_UserName );

	if( defaultchatwindow->m_pChatFilterButton[3] )
	{

	#ifdef _XTS_ALIANCE_20061018
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 || g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
			{
				defaultchatwindow->m_pChatFilterButton[3]->EnableWindow( TRUE );
			}
			else
			{
				defaultchatwindow->m_pChatFilterButton[3]->EnableWindow( FALSE );
			}
	#endif
	}	

	if( defaultchatwindow->m_pChatFilterButton[2] )
	{
		defaultchatwindow->m_pChatFilterButton[2]->EnableWindow( g_pLocalUser->m_InParty ? TRUE : FALSE );
	}	

#endif

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->DeleteList();
	}
#endif
	
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	// 채팅창 위치 변경
	if( defaultchatwindow )
	{
		defaultchatwindow->SetPositionDie(FALSE);
	}
#endif
	
	for( int windowid = 60051; windowid < 60056; windowid++ )
	{
		_XWindow_QuestMail* pQuestMail_Window = (_XWindow_QuestMail*)g_MainWindowManager.FindWindow( windowid );
		if( pQuestMail_Window )
		{
			g_MainWindowManager.DeleteWindow( windowid );
		}
	}

	m_dwLastInputTime = 0;
	
#ifdef _ACCLAIM_RUBICONADSYSTEM	
	
	g_RubiconADRefreshTimer = 0;
	g_RubiconADFirstTimeView = TRUE;
	
	if( (g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0) || 
		g_RubiconADFreeLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
	{
		g_ViewRubiconAD = FALSE;
	}
	else
	{
		g_ViewRubiconAD = TRUE;
	}

	DisplayHTMLPage(g_hRubiconADWebBrowserObject, "http://www.acclaim.com/ads/9d_rubicon_728_90.html");

	POINT pt = { 0, 0 };
	::ClientToScreen( gHWnd, &pt );

	g_RubiconADWindowPosition.x = (gnWidth>>1)-(728>>1);
	g_RubiconADWindowPosition.y = 0;
	g_RubiconADWindowSize.cx = 728;
	g_RubiconADWindowSize.cy = 90;

	::SetWindowPos(g_hRubiconADWindow, NULL, pt.x + g_RubiconADWindowPosition.x, pt.y + g_RubiconADWindowPosition.y, g_RubiconADWindowSize.cx, g_RubiconADWindowSize.cy, 0);
	::SetWindowPos(g_hRubiconADWebBrowserObject, NULL, 0, 0, g_RubiconADWindowSize.cx, g_RubiconADWindowSize.cy, 0);
	//::ShowWindow(g_hRubiconADWindow, SW_SHOW);
	//::UpdateWindow( g_hRubiconADWindow );	
	//::RedrawWindow( g_hRubiconADWindow, NULL, NULL, RDW_UPDATENOW );		
	
	SetUIforRubiconADSystem();
	
#endif
	
	return TRUE;
}

#ifdef _ACCLAIM_RUBICONADSYSTEM	
void XProc_MainGame::SetUIforRubiconADSystem( void )
{
	int height = 0;
	if( g_ViewRubiconAD )
	{
		height = g_RubiconADWindowSize.cy;		
	}
	
	if(g_pMinimap_Window)
	{
		if(height)
		{
			g_pMinimap_Window->MoveWindow( gnWidth-154, height );
		}
		else
		{
			g_pMinimap_Window->MoveWindow( gnWidth-154, 0 );
		}
	}

	if( g_pJinStatus_Window )
	{
		if(height)
		{
			g_pJinStatus_Window->MoveWindow( 0, height );
		}
		else
		{
			g_pJinStatus_Window->MoveWindow( 0, 0 );
		}
	}

	if( g_pState_Window )
	{
		if(height)
		{
			g_pState_Window->MoveWindow( 233, 10 + height );
		}
		else
		{
			g_pState_Window->MoveWindow( 233, 10 );
		}
	}

	if( g_pMainFrame_Window )
	{
		if(height)
		{
			FLOAT yfactor = (FLOAT)gnHeight / 768.0f;
			int halfsize = 442>>1;
			g_pMainFrame_Window->MoveWindow( 36, (int)((114.0f+halfsize)*yfactor) - halfsize + height );
		}
		else
		{
			FLOAT yfactor = (FLOAT)gnHeight / 768.0f;
			int halfsize = 442>>1;
			g_pMainFrame_Window->MoveWindow( 36, (int)((114.0f+halfsize)*yfactor) - halfsize );
		}
	}

	_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
	if(pMonsterStatus_Window)
	{
		if(height)
		{
			pMonsterStatus_Window->MoveWindow( (gnWidth>>1)-90, 22 + height );
		}
		else
		{
			pMonsterStatus_Window->MoveWindow( (gnWidth>>1)-90, 22 );
		}
	}

	/*
	_XWindow_ZoneName* pZoneName_Window = (_XWindow_ZoneName *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ZONENAMEWINDOW);
	if(pZoneName_Window)
	{
		if(height)
		{
			pZoneName_Window->MoveWindow( (gnWidth/2)-182, 96 + height );
		}
		else
		{
			pZoneName_Window->MoveWindow( (gnWidth/2)-182, 96 );
		}
	}
	*/

#ifdef _XGMCLIENT
	_XWindow_GMCommand* pGMCommandWindow = (_XWindow_GMCommand*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMWINDOW);	
	if( pGMCommandWindow )
	{
		if(height)
		{
			pGMCommandWindow->MoveWindow( gnWidth - (1024-980), 211 + height );
		}
		else
		{
			pGMCommandWindow->MoveWindow( gnWidth - (1024-980), 211 );
		}
	}
#endif

	_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
	if(pMatchTablePVP_Window)
	{
		if(height)
		{
			pMatchTablePVP_Window->MoveWindow( (gnWidth>>1)-124, (gnHeight>>1)-270 + height );
		}
		else
		{
			pMatchTablePVP_Window->MoveWindow( (gnWidth>>1)-124, (gnHeight>>1)-270 );
		}
	}

	_XWindow_TargetInfo* pTargetInfoWindow = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
	if(pTargetInfoWindow)
	{
		if(height)
		{
			pTargetInfoWindow->MoveWindow( (gnWidth>>1)-90, 75 + height );
		}
		else
		{
			pTargetInfoWindow->MoveWindow( (gnWidth>>1)-90, 75 );
		}
	}

	int ypos = height;
	for( int i=0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
	{
		DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + i;
		if( g_pLocalUser->m_PartyInfo[i].isLeader )
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
			if(pMiniBar_Window)
			{
				if( pMiniBar_Window->GetShowStatus() )
				{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
					pMiniBar_Window->MoveWindow( 50, 114+height );
#else				
					pMiniBar_Window->MoveWindow( 14, 114+height );
#endif
				}
			}
		}
		else
		{
			_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar *)g_MainWindowManager.FindWindow(windowid);
			if(pMiniBar_Window)
			{
				if( pMiniBar_Window->GetShowStatus() )
				{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI							
					pMiniBar_Window->MoveWindow( 50, 170 + ypos);
#else
					pMiniBar_Window->MoveWindow( 14, 170 + ypos );
#endif
					ypos+=36;
				}
			}
		}
	}

    _XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
	if(pPK_Window )
	{
		if(height)
			pPK_Window->MoveWindow( (gnWidth>>1)-89, (gnHeight>>1)-288 + height );		
		else
			pPK_Window->MoveWindow( (gnWidth>>1)-89, (gnHeight>>1)-288 );
	}

	_XWindow_PCTrade* pPCTradeWindow = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	if(pPCTradeWindow)
	{
		if(height)
			pPCTradeWindow->MoveWindow((gnWidth>>1)-274, (gnHeight>>1)-270 + height);
		else
			pPCTradeWindow->MoveWindow((gnWidth>>1)-274, (gnHeight>>1)-270);
	}

	_XWindow_PersonalStore* pPersonalStoreWindowPtr	= (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
	if(pPersonalStoreWindowPtr)
	{
		if(height)
			pPersonalStoreWindowPtr->MoveWindow((gnWidth>>1)-288, 74 + height);
		else
			pPersonalStoreWindowPtr->MoveWindow((gnWidth>>1)-288, 74);
	}

	_XWindow_MatchMessage* pMatchMessage_Window = (_XWindow_MatchMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHMESSAGE);
	if(pMatchMessage_Window)
	{
		if(height)
			pMatchMessage_Window->MoveWindow(0, 0 + height);
		else
			pMatchMessage_Window->MoveWindow(0, 0);
	}

	_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
	if(pMatchNoticeLarge_Window)
	{
		if(height)
			pMatchNoticeLarge_Window->MoveWindow((gnWidth>>1) -264, (gnHeight>>1)-278 + height);
		else
			pMatchNoticeLarge_Window->MoveWindow((gnWidth>>1) -264, (gnHeight>>1)-278);
	}

	_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = (_XWindow_MatchNoticeSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICESMALL);
	
	if(pMatchNoticeSmall_Window)
	{
		if(height)
			pMatchNoticeSmall_Window->MoveWindow((gnWidth>>1)+115, (gnHeight>>1)-369 + height);
		else
			pMatchNoticeSmall_Window->MoveWindow((gnWidth>>1)+115, (gnHeight>>1)-369);		
	}

	if( g_ViewRubiconAD )
	{
		#ifdef _XTS_ITEMMALLBROWSER
			if( g_bShowItemMallWindow )
			{
				::ShowWindow(g_hRubiconADWindow, SW_HIDE);
			}
			else
			{
				::ShowWindow(g_hRubiconADWindow, SW_SHOW);
			}
		#else
			::ShowWindow(g_hRubiconADWindow, SW_SHOW);
		#endif
	}
	else
	{
		::ShowWindow(g_hRubiconADWindow, SW_HIDE);
	}

}
#endif

void XProc_MainGame::ProcessPCRoomConnectNotice( void )
{
	if( (g_LocalSystemTime - m_PCRoomConnectNoticeTimer) > 120000 )
	{
		m_PCRoomConnectNoticeTimer = g_LocalSystemTime;
		
		if( g_Chatlist.getitemcount() > 0 )
		{
			g_Chatlist.resetList();
			if( g_Chatlist.getAnchor() )
			{
				_XStringItem* pMessageString = (_XStringItem*)g_Chatlist.getAnchor()->getleft();

				if( pMessageString )
				{
					if( strcmp( pMessageString->Get_String(), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2442) ) == 0 )
					{
						return;
					}
				}
			}
		}		

		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2443), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, FALSE );
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2444), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, FALSE );
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2445), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, FALSE );
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2442), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, FALSE );
	}
}

void XProc_MainGame::ProcessBgmTimer( void )
{
#ifdef _XJAPANESE
	if( g_LocalSystemTime - m_BGMPlayTimer > 120000 ) 
#else
	if( g_LocalSystemTime - m_BGMPlayTimer > 180000 ) 
#endif
	{
		m_BGMPlayTimer = g_LocalSystemTime;
		m_ChangingBGM = 1;
	}
}

#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
void XProc_MainGame::SetSpecialBGMPlay( LPTSTR filename, BOOL loop )
#else
void XProc_MainGame::SetSpecialBGMPlay( LPTSTR filename )
#endif
{
	g_BgmPlayFlag = FALSE;

#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
	m_PreviousBGMPlayFlag = g_BgmPlayFlag;
	m_SpecialBGMLoopPlayFlag = loop;
#endif

	_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
	if( optionwindow )
	{	
		_XCheckButton* pButton = ((_XWindow_OptionSound*)optionwindow->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pUseBGMButton;
		
		if( pButton )
		{			
			pButton->SetCheck( FALSE );
			
			g_BgmPlayFlag = pButton->GetCheck();
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, g_BgmPlayFlag );
		}
	}
	
	if( strlen( filename ) <= 0 )
	{
		m_SpecialBGMPlayMode = FALSE;
	}
	else
	{
		if( (strcmp(filename, m_SpecialBGMFileName) != 0) || !m_SpecialBGMPlayMode )
		{
			m_SpecialBGMPlayMode = TRUE;
			memset( m_SpecialBGMFileName, 0, sizeof(TCHAR) * 64 );
			strcpy( m_SpecialBGMFileName, filename );	
			g_FMODWrapper.PlayBGM( m_SpecialBGMFileName, FALSE );
			
#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
			m_SpecialBGMPlayTimer = g_LocalSystemTime;
#endif

			//int lengthms = FSOUND_Stream_GetLengthMs( g_FMODWrapper.m_pBGMStreamObject );
			//g_FMODWrapper.SetBGMLoopPoints( 0, lengthms );
			//FSOUND_Stream_SetLoopCount( g_FMODWrapper.m_pBGMStreamObject, 100 );
		}
	}
}

void XProc_MainGame::StopSpecialBGMPlay( void )
{
	g_FMODWrapper.StopBGM();
	m_SpecialBGMPlayMode = FALSE;	
	memset( m_SpecialBGMFileName, 0, sizeof(TCHAR) * 64 );

	m_BGMPlayTimer = g_LocalSystemTime;
}

void XProc_MainGame::ProcessChangeBgm( void )
{
	if( m_SpecialBGMPlayMode )
	{
		if( !g_FMODWrapper.IsBGMPlaying() )
		{
#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
			if( m_SpecialBGMLoopPlayFlag )
			{
#ifdef _XJAPANESE
				if( g_LocalSystemTime - m_SpecialBGMPlayTimer > 120000 ) 
#else
				if( g_LocalSystemTime - m_SpecialBGMPlayTimer > 180000 ) 
#endif
				{
					TCHAR szSpecialBGMFilename[64];
					memset( szSpecialBGMFilename, 0, sizeof(TCHAR) * 64 );
					strcpy( szSpecialBGMFilename, m_SpecialBGMFileName );

					StopSpecialBGMPlay();
					SetSpecialBGMPlay( szSpecialBGMFilename, m_SpecialBGMLoopPlayFlag );

					m_SpecialBGMPlayTimer = g_LocalSystemTime;
				}
			}
			else
#endif
			{
				//if( strlen( m_SpecialBGMFileName ) <= 0 )
				{
					m_SpecialBGMPlayMode = FALSE;
					memset( m_SpecialBGMFileName, 0, sizeof(TCHAR) * 64 );

#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
					_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
					if( optionwindow )
					{	
						_XCheckButton* pButton = ((_XWindow_OptionSound*)optionwindow->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pUseBGMButton;
						
						if( pButton )
						{			
							pButton->SetCheck( m_PreviousBGMPlayFlag );
							g_BgmPlayFlag = m_PreviousBGMPlayFlag;
							g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, g_BgmPlayFlag );
						}
					}
#endif
				}	
				//else
				{
					/*
					if( !g_FMODWrapper.m_ChangeBGM )
					{
						g_FMODWrapper.PlayBGM( m_SpecialBGMFileName );
					}
					*/
				}
			}
		}
		return;
	}

	if( !g_BgmPlayFlag ) return;
	
#ifdef _XDEF_CASTLEBATTLE
	if(g_pLocalUser->m_bCastleBattle)
	{
		// 장원전에 참여중일 때는 일반 BGM play 하지 않음
		return;
	}
#endif

#ifdef _XUSEFMOD

	if( m_ChangingBGM == 0 ) return;
	
	if( m_CurrentBGMType == 0 )
	{
		g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename2 );
		m_CurrentBGMType = 1;
		m_ChangingBGM = 0;

#ifdef _XDWDEBUG
		_XInsertSystemMessage( 0, "Change BGM 1->2 : %s", g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename2 );
#endif

	}
	else if( m_CurrentBGMType == 1 )
	{
		g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename1 );

		m_CurrentBGMType = 0;
		m_ChangingBGM = 0;

#ifdef _XDWDEBUG
		_XInsertSystemMessage( 0, "Change BGM 2->1 : %s", g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename1 );
#endif

	}

#else	
	if( g_pBGMSoundObject[0] )
	{
		if( g_pBGMSoundObject[0]->IsPlaying() )
		{
			FLOAT fVolume;
			g_pBGMSoundObject[0]->GetVolume( fVolume );
			fVolume -= 0.01f;
			if( fVolume < 0.0f ) fVolume = 0.0f;
			
			g_pBGMSoundObject[0]->SetVolume( fVolume );
			
			if( fVolume <= 0.01f ) // 소리가 완전히 작아지면
			{
				g_pBGMSoundObject[0]->Stop();
				g_pBGMSoundObject[0]->Destroy();
				g_pBGMSoundObject[0] = NULL;
			}
		}
		else
		{
			g_pBGMSoundObject[0]->Destroy();
			g_pBGMSoundObject[0] = NULL;
		}
	}

	if( m_ChangingBGM == 0 ) return;
		
	if( m_ChangingBGM == 1 ) // To end
	{
		if( m_CurrentBGMType == 0 )
		{
			if( g_pBGMSoundObject[2]->IsPlaying() )
			{
				FLOAT fVolume;
				g_pBGMSoundObject[2]->GetVolume( fVolume );
				fVolume -= 0.01f;
				if( fVolume < 0.0f ) fVolume = 0.0f;
				
				g_pBGMSoundObject[2]->SetVolume( fVolume );
				
				if( fVolume <= 0.01f ) // 소리가 완전히 작아지면
				{
					g_pBGMSoundObject[2]->Stop();
					
					m_ChangingBGM = 2;
					g_pBGMSoundObject[2]->SetVolume(0.01f);
					g_pBGMSoundObject[2]->Play();
				}
			}
			else
			{
				m_ChangingBGM = 2;
				g_pBGMSoundObject[1]->SetVolume(0.01f);
				g_pBGMSoundObject[1]->Play();
			}
		}
		else
		{
			if( g_pBGMSoundObject[1]->IsPlaying() )
			{
				FLOAT fVolume;
				g_pBGMSoundObject[1]->GetVolume( fVolume );
				fVolume -= 0.01f;
				if( fVolume < 0.0f ) fVolume = 0.0f;
				
				g_pBGMSoundObject[1]->SetVolume( fVolume );
				
				if( fVolume <= 0.01f ) // 소리가 완전히 작아지면
				{
					g_pBGMSoundObject[1]->Stop();
					
					m_ChangingBGM = 2;
					g_pBGMSoundObject[2]->SetVolume(0.01f);
					g_pBGMSoundObject[2]->Play();
				}
			}
			else
			{
				m_ChangingBGM = 2;
				g_pBGMSoundObject[2]->SetVolume(0.01f);
				g_pBGMSoundObject[2]->Play();
			}
		}
	}
	else if( m_ChangingBGM == 2 ) // To start
	{
		if( m_CurrentBGMType == 1 )
		{
			if( g_pBGMSoundObject[2]->IsPlaying() )
			{
				FLOAT fVolume;
				g_pBGMSoundObject[2]->GetVolume( fVolume );
				fVolume += 0.01f;
				if( fVolume > g_pBGMSoundMaxVolume ) fVolume = g_pBGMSoundMaxVolume;
				
				if( fVolume < g_pBGMSoundMaxVolume )
				{
					g_pBGMSoundObject[2]->SetVolume( fVolume );
				}
				else // 소리가 최대로 커지면 이프로세스는 끝이다.
				{
					m_ChangingBGM = 0;
				}
			}
		}		
		else // if( m_CurrentBGMType == 0 )
		{
			if( g_pBGMSoundObject[1]->IsPlaying() )
			{
				FLOAT fVolume;
				g_pBGMSoundObject[1]->GetVolume( fVolume );
				fVolume += 0.01f;
				if( fVolume > g_pBGMSoundMaxVolume ) fVolume = g_pBGMSoundMaxVolume;
				
				if( fVolume < g_pBGMSoundMaxVolume )
				{
					g_pBGMSoundObject[1]->SetVolume( fVolume );
				}
				else // 소리가 최대로 커지면 이프로세스는 끝이다.
				{
					m_ChangingBGM = 0;
				}
			}
		}
	}
#endif
}



void XProc_MainGame::SendSignal_ProcessStop( void )
{
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;

	if( eodapp->m_ProcessManager.GetCurrentProcess() == (_XProcess*)this  )
	{
		m_GameExitFlag = FALSE;
		m_ProcessStopFlag = TRUE;		
	}
}

void XProc_MainGame::ProcessLocalUserDie(void)
{
	D3DCOLOR color = ((DWORD)m_UserDieScreenAlphaLevel << 24);
	g_ScreenBurnOut[0].color = color;
	g_ScreenBurnOut[1].color = color;
	g_ScreenBurnOut[2].color = color;
	g_ScreenBurnOut[3].color = color;
	
	// Fade in
	if(m_UserDieScreenEffectFlag)
	{
		m_UserDieScreenAlphaLevel += _XDEF_SCREENFADE_LEVEL;
		
		if(m_UserDieScreenAlphaLevel >= 255)
		{
			m_UserDieScreenAlphaLevel = 255;
			m_UserDieScreenEffectFlag = FALSE;
		}
	}
	else
	{
		m_UserDieScreenAlphaLevel -= _XDEF_SCREENFADE_LEVEL;
		if(m_UserDieScreenAlphaLevel <= 0)
		{
			m_UserDieScreenAlphaLevel = 0;
			m_UserDieScreenEffectFlag = TRUE;
			m_ProcessLocalUserDieFlag = FALSE;
			
			if( !m_NoDisplayZoneInfo )
			{			
				_XWindow_ZoneName* pZoneName_Window = (_XWindow_ZoneName *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ZONENAMEWINDOW);
				if(pZoneName_Window)
				{
					pZoneName_Window->SetWorldNameDisplayMode( TRUE );
#ifdef _XJAPANESE	
					pZoneName_Window->SetZoneName( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename2, NULL );
#else
					pZoneName_Window->SetZoneName( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename2 );
#endif
					pZoneName_Window->StartAnimation( _XZoneWindowEffectClass_Open );
				}
				m_NoDisplayZoneInfo = FALSE;
			}
			
			return;
		}
	}
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );
	
	gpDev->SetTexture(0,NULL);
	
	gpDev->SetFVF( D3DFVF_XTLVERTEX );
	gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );

	//gpDev->SetRenderState( D3DRS_FOGENABLE, g_LodTerrain.m_Fog );
}

void XProc_MainGame::Process_ProcessStop( void )
{
	if( m_ProcessStopTime == 0 )
	{
		m_ProcessStopTime = g_LocalSystemTime;
	}
	else
	{
		DWORD progresstime = (g_LocalSystemTime - m_ProcessStopTime);
		if( progresstime < _XDEF_PROCESSTOP_DELAYTIME )
		{	
			DWORD seconds = (( _XDEF_PROCESSTOP_DELAYTIME - progresstime ) / 1000);

			if( seconds == 0 )
			{
//				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_MAIGAME_EXITGAME, TRUE );
//				pMessageBox->SetMessageColor( D3DCOLOR_ARGB( 255, 255, 255, 0 ) );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_MAIGAME_EXITGAME, TRUE );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 255, 0 ) );
			}
			else
			{
//				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_OPTION_EXITGAME, " ", TRUE );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_OPTION_EXITGAME, " ", TRUE );
				
//				TCHAR messagestring[128];
//				LoadString( gHInst, ID_STRING_MAIGAME_COUNTEXIT, messagestring, 127 );
//				pMessageBox->SetMessage( messagestring, seconds );
//				pMessageBox->SetMessageColor(D3DCOLOR_ARGB( 255, 255, 64, 64));
				g_MessageBox.SetMessage( _XGETINTERFACETEXT(ID_STRING_MAIGAME_COUNTEXIT), seconds );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 64, 64 ) );
			}
		}
		// Send stop process signal...
		else 
		{
			//PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GAMECLOSE, 0), (LPARAM)gHWnd);
			/*g_NormalGameExit = TRUE;
			PostMessage(gHWnd, WM_CLOSE, 0, 0 );*/

			// Save quickslot informations...
			{
				_XQuickSlotInfoList	quickslotinfolist;
				quickslotinfolist.LoadQuickSlotInfo();
				quickslotinfolist.UpdateInfoList();
				quickslotinfolist.SaveQuickSlotInfo();
			}

			ResetData();//2004.06.19->oneway48 insert

			g_NetworkKernel.DisconnectServer();			
			g_NetworkKernel.InitializeNetwork();

			g_QuestScriptManager.ReleasePrivateData();

			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
			
			g_MessageBox.ShowWindow( FALSE );
			g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_MESSAGEWINDOW);

			m_ProcessStopTime = 0;
		}
	}
}



void XProc_MainGame::Process_GameExit( void )
{
	if( m_GameExitSignalTime == 0 )
	{				
		m_GameExitSignalTime = g_LocalSystemTime;
	}
	else
	{
		DWORD progresstime = (g_LocalSystemTime - m_GameExitSignalTime);
		TCHAR  message[256];
		DWORD seconds = (( _XDEF_GAMEEXIT_DELAYTIME - progresstime ) / 1000);		
		
		static DWORD g_LastPrintedSecond = 0;

		if( progresstime < _XDEF_GAMEEXIT_DELAYTIME )
		{	
			if( g_LastPrintedSecond != seconds )
			{
				TCHAR message[256];
				TCHAR messagestring[128];
				strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAIGAME_COUNTEXIT), sizeof(messagestring));

				_snprintf( message, sizeof(message), messagestring, seconds+1 ); //"%d초 후에 게임을 종료하겠습니다."
				g_NetworkKernel.InsertChatString( message ,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

				g_LastPrintedSecond = seconds;
			}
		}
		// Send end process  signal...
		else 
		{	
			if( m_ProgramExitFlag )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_MAIGAME_WARNEXITGAME, TRUE );
				//Draw();
				
				_XQuickSlotInfoList	quickslotinfolist;
				quickslotinfolist.LoadQuickSlotInfo();
				quickslotinfolist.UpdateInfoList();
				quickslotinfolist.SaveQuickSlotInfo();

				g_NormalGameExit = TRUE;
				PostMessage( gHWnd, WM_CLOSE, NULL, NULL );
			}
			else
			{			
				// "게임을 종료합니다."
	//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
	//			pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_MAIGAME_WARNEXITGAME, TRUE, _XDEF_GOTOLOGINSERVER, _XDEF_GOTOLOGINSERVER, _XDEF_GOTOLOGINSERVER );
	//			pMessageBox->StartTimer(1500, TRUE);
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_MAIGAME_WARNEXITGAME, TRUE, _XDEF_GOTOLOGINSERVER, _XDEF_GOTOLOGINSERVER, _XDEF_GOTOLOGINSERVER );
				g_MessageBox.StartTimer( 1500, TRUE );
				Draw();
				
				TCHAR messagestring[128];
				memset(messagestring, 0, sizeof(messagestring));
				strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAIGAME_WARNEXITGAME), sizeof(messagestring));
				_snprintf( message, sizeof(message), messagestring, sizeof(message) );
				g_NetworkKernel.InsertChatString( message ,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				
				m_GameExitSignalTime = 0;
				m_GameExitFlag = FALSE;
				g_LastPrintedSecond = 0;

				m_bGotoLoginProcess = TRUE;//2004.06.29->oneway48 insert
				
				ResetData();//2004.06.19->oneway48 insert
			}
		}
	}
}

void XProc_MainGame::OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{	
}

void XProc_MainGame::OnWindowSizeChangeNotify( void )
{
	g_MainWindowManager.SetDefaultPosition();
	
	int ypos = 0;

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	if( g_ViewRubiconAD)
		ypos = g_RubiconADWindowSize.cy;
#endif

	for( int i=0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
	{
		DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + i;
		if( g_pLocalUser->m_PartyInfo[i].isLeader )
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
			if(pMiniBar_Window)
			{
				if( pMiniBar_Window->GetShowStatus() )
				{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI			
				#ifdef _ACCLAIM_RUBICONADSYSTEM	
					pMiniBar_Window->MoveWindow( 50, 114+((g_ViewRubiconAD)?g_RubiconADWindowSize.cy:0) );
				#else
					pMiniBar_Window->MoveWindow( 50, 114 );
				#endif
#else					
				#ifdef _ACCLAIM_RUBICONADSYSTEM	
					pMiniBar_Window->MoveWindow( 14, 114+((g_ViewRubiconAD)?g_RubiconADWindowSize.cy:0) );
				#else
					pMiniBar_Window->MoveWindow( 14, 114 );
				#endif
#endif
				}
			}
		}
		else
		{
			_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar *)g_MainWindowManager.FindWindow(windowid);
			if(pMiniBar_Window)
			{
				if( pMiniBar_Window->GetShowStatus() )
				{
				#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI		
					pMiniBar_Window->MoveWindow( 50, 170 + ypos );
				#else		
					pMiniBar_Window->MoveWindow( 14, 170 + ypos );
				#endif
					ypos+=36;
				}
			}
		}
	}
	
	g_MessageBox.MoveWindow((gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1), (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1) - 80 );
}

void XProc_MainGame::DestroyProcess( void )
{
	// by mahwang
//	g_LifeEventManager.DeleteData();

	_XWindow::m_GlobalPlaySoundEffect = FALSE;

	g_TerrainDecalObject.InvalidateDeviceObject();

	g_WorldObjectTextureArchive.m_bUseThread = FALSE;
	g_MobTextureArchive.m_bUseThread = FALSE;

#ifdef _XDEF_MODELTHREADLOADING
	
	g_LoadTextureThread.DeleteThread();
	g_LoadTextureThread.m_bEnable = FALSE;
	g_LoadMeshThread.DeleteThread();
	g_LoadMeshThread.m_bEnable = FALSE;

	g_CharacterTextureArchive.m_bUseThread = TRUE;

	g_CharacterEffectGroupManager.m_TextureManager.m_bUseThread = TRUE;
	g_WorldEffectGroupManager.m_TextureManager.m_bUseThread = TRUE;

#else

	g_LoadTextureThread.DeleteThread();
	g_LoadTextureThread.m_bEnable = FALSE;

#endif

	m_FirstEntered = !TRUE;

	// 카메라 높이 재조정관련 파라미터를 꺼줌.
	g_LodTerrain.m_3PCamera.SetAdditionalHeightMinDistance( 0.0f );

	// 이젠 reference count 를 통해서 지우기 때문에 
	// 여기서 한꺼번에 지울 필요가 없다.
	g_MobTextureArchive.ReleaseTextureData();
	g_CharacterTextureArchive.ReleaseTextureData();	
	g_EffectTextureArchive.ReleaseTextureData();

	g_TerrainTextureArchive.DisposeTexture();
	g_WorldObjectTextureArchive.DisposeTexture();
	g_LodTerrain.ReleaseTerrainTileInfo();
	
	g_XST_Wrapper.Release();
	//g_XST_Wrapper.CleanUpInstance();
	
#ifdef _XDWDEBUG
	m_ArrowModel.DestroyModel();
	g_AxisModel.DestroyModel();
	SAFE_RELEASE( m_pLightConeMesh );
#endif

#ifdef _XUSEFMOD
	
	g_FMODWrapper.StopAllSound();
	g_FMODWrapper.FL_StopAllSound();
	g_FMODWrapper.FL_ReleaseFlexibleList();

#else		
	if( g_pBGMSoundObject[1] )
	{
		if( g_pBGMSoundObject[1]->IsPlaying() )
		{
			g_pBGMSoundObject[1]->Stop();			
		}

		g_pBGMSoundObject[1]->Destroy();
		g_pBGMSoundObject[1] = NULL;
	}

	if( g_pBGMSoundObject[2] )
	{
		if( g_pBGMSoundObject[2]->IsPlaying() )
		{
			g_pBGMSoundObject[2]->Stop();			
		}
		
		g_pBGMSoundObject[2]->Destroy();
		g_pBGMSoundObject[2] = NULL;
	}
#endif
		
	m_ChatEditContainer.ShowWindow( FALSE );
				
	g_GameTimer.Init_All();

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

#else	
	g_Chatlist.disposeList();
	g_SystemMsgList.disposeList();	// 2004.11.26->hotblood 
#endif
	
	_XWindow_ChattingDefault* pChattingDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
	if( pChattingDefault_Window )
		pChattingDefault_Window->ResetChattingDefault();
	
	_XWindow_AdminNotice* pAdminNotice_Window = (_XWindow_AdminNotice*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NOTICEWINDOW );
	if( pAdminNotice_Window )
	{
		pAdminNotice_Window->m_NoticeMessageList.disposeList();	
		
		if( pAdminNotice_Window->m_NoticeListScrollBar )
		{
			pAdminNotice_Window->m_NoticeListScrollBar->SetTotalLineCount( pAdminNotice_Window->m_NoticeMessageList.getitemcount() );									
			pAdminNotice_Window->m_NoticeListScrollBar->UpdateData();			
		}				
		
		pAdminNotice_Window->ShowWindow( FALSE );
	}
	
	if( !g_pLocalUser->m_bIsBossBattle && !g_pLocalUser->m_bBossBattleEndProcess)//보스전이면 상태효과를 다시 받지 않음.->유지
	{
		g_pLocalUser->ResetLocalUserTempData();
	}
	else
	{
		g_pLocalUser->ResetAttackTarget();
		g_pLocalUser->ResetViewTarget();
	}
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	if( g_pMinimap_Window )
	{
		_XPKTRACING_POSITION_INFO* pktracing = NULL;
		list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pMinimap_Window->m_listPKTracingPosition.begin() ; iter_pktracing != g_pMinimap_Window->m_listPKTracingPosition.end() ; ++iter_pktracing)
		{
			pktracing = *iter_pktracing;
			SAFE_DELETE( pktracing );
		}
		g_pMinimap_Window->m_listPKTracingPosition.clear();
		g_pMinimap_Window->m_bDrawPKWarning = FALSE;
	}
#endif

	//Author : 양희왕 //breif : 비무 신청 창이 켜있으면 취소하고 끝낸다
	_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
	if( pMatchTablePVP_Window && pMatchTablePVP_Window->GetShowStatus() )
	{
		pMatchTablePVP_Window->ShowWindow(FALSE);
		g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_cancel_all);
	}

	
	g_Moblist.disposeList();
	g_Npclist.disposeList();
	g_Userlist.disposeList();
	g_GameItemlist.disposeList();

	// 유저 튜토리얼 저장
	SaveTutorialInfo();	

	g_WorldEffectInstanceManager.Dispose();
	g_PostProcessWorldEffectInstanceManager.Dispose();
		
	//g_QuestScriptManager.Release();
	memset(&g_pLocalUser->m_CharacterLog, 0, sizeof(g_pLocalUser->m_CharacterLog));
	_XDWINPRINT("[MAINGAME/DestroyProcess] All log clear");	
	
	g_LastNPCScriptOwner = NULL;

	if(m_pNPCScriptWindowPtr)
	{
		if(m_pNPCScriptWindowPtr->GetShowWindow())
			m_pNPCScriptWindowPtr->ShowWindow(FALSE);
	}

	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) // 2004.06.01->oneway48 modify : 15->9
	{
		DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + i;
		if( g_pLocalUser->m_PartyInfo[i].isLeader )
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
			if(pMiniBar_Window)
			{
				if(pMiniBar_Window->GetShowStatus())
					pMiniBar_Window->ShowWindow(FALSE);
			}
		}
		else
		{
			_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
			if(pMiniBar_Window)
			{
				if(pMiniBar_Window->GetShowStatus())
					pMiniBar_Window->ShowWindow(FALSE);
			}
		}
	}

	if(g_pInventory_Window)
	{
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}
	if(m_pWarehouseWindowPtr)
	{
		if(m_pWarehouseWindowPtr->GetShowStatus())
			m_pWarehouseWindowPtr->ShowWindow(FALSE);
		m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}
	if(m_pNPCTradeWindowPtr)
	{
		if(m_pNPCTradeWindowPtr->GetShowStatus())
			m_pNPCTradeWindowPtr->ShowWindow(FALSE);
		m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}
	if(m_pPCTradeWindowPtr)
	{
		if(m_pPCTradeWindowPtr->GetShowStatus())
			m_pPCTradeWindowPtr->ShowWindow(FALSE);
		m_pPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}

	if( m_pSocketPlugInWindowPtr )
	{
		if( m_pSocketPlugInWindowPtr->GetShowStatus() )
			m_pSocketPlugInWindowPtr->ShowWindow(FALSE);
		m_pSocketPlugInWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pSocketPlugInWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}
	if(g_pQuickSlot_Window)
	{
		g_pQuickSlot_Window->SetDrawSelectedIcon(false);
		g_pQuickSlot_Window->ResetMouseSlot();
	}

	if( m_pContributionWindowPtr )
	{
		if( m_pContributionWindowPtr->GetShowStatus() )
			m_pContributionWindowPtr->ShowWindow(FALSE);
	}

	if( m_pRepairItemWindowPtr )
	{
		if( m_pRepairItemWindowPtr->GetShowStatus() )
			m_pRepairItemWindowPtr->ShowWindow(FALSE);
		m_pRepairItemWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pRepairItemWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}

	if( m_pPocketItemWindowPtr )
	{
		if( m_pPocketItemWindowPtr->GetShowStatus() )
			m_pPocketItemWindowPtr->ShowWindow(FALSE);
		m_pPocketItemWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pPocketItemWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}
	if( m_pHedekWindowPtr )
	{
		if( m_pHedekWindowPtr->GetShowStatus() )
			m_pHedekWindowPtr->ShowWindow( FALSE );
	}
	
	if( m_pPersonalStoreWindowPtr )
	{
		if( m_pPersonalStoreWindowPtr->GetShowStatus() )
			m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
	}
	if( m_pGambleBoxWindowPtr )
	{
		if( m_pGambleBoxWindowPtr->GetShowStatus() )
			m_pGambleBoxWindowPtr->ShowWindow(FALSE);
		m_pGambleBoxWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pGambleBoxWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}
	if( m_pSocketPlugInExWindowPtr )
	{
		if( m_pSocketPlugInExWindowPtr->GetShowStatus() )
			m_pSocketPlugInExWindowPtr->ShowWindow(FALSE);
		m_pSocketPlugInExWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pSocketPlugInExWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}
	if( m_pInchantWindowPtr )
	{
		if( m_pInchantWindowPtr->GetShowStatus() )
			m_pInchantWindowPtr->ShowWindow(FALSE);
	}		
	
	if( m_pPVPBrokerWindowPtr )
	{
		if( m_pPVPBrokerWindowPtr->GetShowStatus() )
			m_pPVPBrokerWindowPtr->ShowWindow(FALSE);
	}		
	
	if( m_pAddSaveItemWindowPtr )
	{
		if( m_pAddSaveItemWindowPtr->GetShowStatus() )
			m_pAddSaveItemWindowPtr->ShowWindow(FALSE);
	}		
	
	if( m_pNPCCollectionWindowPtr )
	{
		if( m_pNPCCollectionWindowPtr->GetShowStatus() )
			m_pNPCCollectionWindowPtr->ShowWindow(FALSE);
	}		
	
	if( m_pNPCResourceWindowPtr )
	{
		if( m_pNPCResourceWindowPtr->GetShowStatus() )
			m_pNPCResourceWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pNPCResource_New_windowptr)
	{
		if(m_pNPCResource_New_windowptr->GetShowStatus())
			m_pNPCResource_New_windowptr->ShowWindow(FALSE);
	}

	if( m_pNPCBuffWindowPtr )
	{
		if( m_pNPCBuffWindowPtr->GetShowStatus() )
			m_pNPCBuffWindowPtr->ShowWindow(FALSE);
	}
#ifdef _XTS_MOVEVILLAGE
	if(m_pMoveVillageWindowPtr)
	{
		if(m_pMoveVillageWindowPtr->GetShowStatus())
			m_pMoveVillageWindowPtr->ShowWindow(FALSE);
	}
#endif	

	_XWindow_Party* pParty_Window = (_XWindow_Party*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
	if(pParty_Window)
	{
		pParty_Window->SetPartyList();
	}	

	_XWindow_TutorialIcon* pTutorialIcon_Window = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_TUTORIALICON );
	if( pTutorialIcon_Window )
	{
		pTutorialIcon_Window->ResetTutorialData();
	}

	if( g_pMainFrame_Window && g_pMainFrame_Window->GetShowStatus() )
	{
		g_pMainFrame_Window->ShowWindow(FALSE);
	}
	
	// delete object
	g_DummyList.disposeList();
	g_SeatList.disposeList();

	_XSetCaptureFileSubFolder( NULL );
	((CEmperorOfDragonsApp*)gApp)->SetCursor( _XMOUSETYPE_PEACE );

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.DisableMADLocation( _T("In-game world") );
#endif

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	m_CancleGameGuardAuth = TRUE;
#endif

	_XWindow_EventNotice* pEventNotice_Window = (_XWindow_EventNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTNOTICEWINDOW);
	if( pEventNotice_Window )
		pEventNotice_Window->ShowWindow(FALSE);

#ifdef _XDEF_WEBBOARD_20061211
	_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
	if( pWebBoard_Window  )
	{
		pWebBoard_Window->ShowWindow(FALSE);
		ShowWindow(g_hWebBoardWindow, SW_HIDE );
		g_bShowItemMallWindow = FALSE;					
	}
#endif
	
#ifdef _XTS_ITEMMALLBROWSER	
	_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
	if( pCashMall_Window )
	{
		pCashMall_Window->ShowWindow( FALSE );
		ShowWindow(g_hItemMallWindow, SW_HIDE);
		g_bShowItemMallWindow = FALSE;
	}		
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM
	::ShowWindow(g_hRubiconADWindow, SW_HIDE);
#endif

	SetEnableProcess( FALSE );

	_XLog( "End of main process" );
}

bool XProc_MainGame::InitDeviceObject( void )
{		
	// Initialize renderer state machine..
	InitializeRenderState();
			
	// Setup viewport
	g_MainViewPort.X = 0;
	g_MainViewPort.Y = 0;
	g_MainViewPort.Width = gnWidth;
	g_MainViewPort.Height = gnHeight;
	g_MainViewPort.MinZ = 0.0f;
	g_MainViewPort.MaxZ = 1.0f;
	gpDev->SetViewport(&g_MainViewPort);

	// Set camera matrices
	gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());	
					
	// Initialize light object=================================================
	InitializeLight();

	g_EnvironmentManager.SetEnvironmentFog();
	//g_EnvironmentManager.SetChangeEnvironmentTime( m_CurrentEnvironmentTime );

	_XWindow_GMCommand* pGMCommandWindow = (_XWindow_GMCommand*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMWINDOW);	
	if( pGMCommandWindow )
	{
		_XWindow::m_GlobalPlaySoundEffect = FALSE;
		pGMCommandWindow->MoveWindow( 980, 211 );
		pGMCommandWindow->SetWindowAnimationMode( _XGMWINDOW_ANIMATIONMODE_CLOSE );
		_XWindow::m_GlobalPlaySoundEffect = TRUE;
	}

	return true;
}

void XProc_MainGame::ReleaseDeviceObject( void )
{
#ifdef _XDWDEBUG		
	SAFE_RELEASE( m_pLightConeMesh );
#endif
}

void XProc_MainGame::PauseProcess( void )
{

}

void XProc_MainGame::UnPauseProcess( void )
{
	
}

void XProc_MainGame::Draw( void )
{
#ifdef _XGMCLIENT	
if( g_pLocalUser->m_CharacterInfo.Get_gm_level() == 0 )
{
#endif

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD) && !defined(_XJAPANESE) // 일본은 gameguard basic license라서 서버 인증을 사용 안함.
	if( !g_pLocalUser->m_bIsBossBattle && 		
		( g_QuestScriptManager.m_indexVirtualZone != 22 ) && 
		!m_CompleteGameGuardAuth && !m_CancleGameGuardAuth ) // 첫번째 서버인증이 안되있을 경우 Draw를 하지 않는다.
	{
		//_XLog("Skipped Draw");
		Sleep( 1 );
		return;
	}
#endif

#ifdef _XGMCLIENT
}
#endif

	//if (g_EnvironmentManager.Process(g_LodTerrain.m_3PCamera.m_CameraPosition, g_fElapsedFrameMilisecondTime, g_LocalSystemTime) == TRUE)
	if( g_FreeMoveMode )
	{
		if (g_EnvironmentManager.Process(g_LodTerrain.m_3PCamera.m_CameraPosition, g_pLocalUser->m_Position, g_fElapsedFrameMilisecondTime, g_LocalSystemTime) == TRUE)
		{
			if( g_pMinimap_Window )
			{
				g_pMinimap_Window->RebuildCurrentTimeIcon();
			}
		}
	}
	else
	{
		if (g_EnvironmentManager.Process(g_pLocalUser->m_Position, g_pLocalUser->m_Position, g_fElapsedFrameMilisecondTime, g_LocalSystemTime) == TRUE)
		{
			if( g_pMinimap_Window )
			{
				g_pMinimap_Window->RebuildCurrentTimeIcon();
			}
		}
	}
	
	if( CWater::m_WaterRenderingMode != _XWATER_RM_NORMAL ) 
	{
		g_LodTerrain.m_pWaterObjectManager->PreDrawEnvironment();
	}

	if( g_LodTerrain.m_3PCamera.m_ImpactValue != 0.0f )
	{
		g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() + g_LodTerrain.m_3PCamera.m_ImpactValue );
		
		// reset camera rotate & zoom velocity...
		g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
		g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		
		// Set Third person proj.. matrix
		gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());
		// Set Third person view matrix
		gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );

		g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - g_LodTerrain.m_3PCamera.m_ImpactValue );

		// reset camera rotate & zoom velocity...
		g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
		g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();

		g_LodTerrain.m_3PCamera.m_ImpactValue = 0.0f;
	}
	else
	{
		// Set Third person proj.. matrix
		gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());
		// Set Third person view matrix
		gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
	}
	
	g_InterfaceTooltip.ShowWindow( FALSE );

	

	g_CharacterEffectGroupManager.Update( g_fElapsedFrameMilisecondTime, g_fLocalSystemTime );

// Begin the scene
	gpDev->BeginScene();

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_EnvironmentManager.m_FogColor, 1.0f, 0);

	g_PostProcessManager.BeginPostProcess();
	
#ifdef _XTS_MOTIONBLURTEST
	g_ScreenMotionBlurEffectManager.BeginBlur();
#endif
	
	// Last picking object info.. data intialize...
	g_LastPickedObjectType = _XPICKINGTARGET_NONE;
	g_LastPickedObjectDistance = 9999.0f;
	g_LastPickedObject = NULL;

	// Render Terrain meshes..

//	if( !m_bMiniGameDraw )
	{
		g_EnvironmentManager.DrawSkyBox();
		g_LodTerrain.DrawTerrain();
	}

	// Render world water object...	
	if(g_LodTerrain.m_pWaterObjectManager)
	{		
		g_LodTerrain.m_pWaterObjectManager->Draw();
	}

	//////////////////////////////////////////////////////////////
	// Render world object...
	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_ObjectAmbientColor );
	gpDev->SetRenderState( D3DRS_FOGENABLE, TRUE );

#ifdef _XDWDEBUG	
	if( g_LodTerrain.viewobject )
	{
#endif
		
#ifdef _XDWDEBUG	
	extern BOOL g_DrawAboveWaterObject;
	if(g_DrawAboveWaterObject)
	{
		g_LodTerrain.RenderWaterAboveObject();
	}
	else
#endif
	{
		g_LodTerrain.RenderObject();

		//gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
		//g_LodTerrain.m_MeshObjectManager.RenderBakemapObject();

		g_LodTerrain.RenderAniObject(g_fElapsedFrameTime);
	}

	g_XST_Wrapper.Process();
	g_XST_Wrapper.DrawRenderStack();

	// Render World effect...
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	
	g_WorldEffectInstanceManager.Update( NULL, NULL, NULL, NULL );
	if( !m_bNPCDialogMode && !m_bNPCDialogWideView && !m_bFunctionalObjectWideView)// 2004.06.01->oneway48 insert : NPC대화모드일때 아이템이 떨어진 것을 그리지 않는다.
	{
		g_WorldEffectInstanceManager.Render();
	}
	
	g_LodTerrain.m_EffectObjectManager.Update(NULL, NULL, NULL, NULL);
	if( !m_bNPCDialogMode && !m_bNPCDialogWideView && !m_bFunctionalObjectWideView )// 2004.06.01->oneway48 insert : NPC대화모드일때 아이템이 떨어진 것을 그리지 않는다.
	{
		g_LodTerrain.m_EffectObjectManager.RenderWorldEffect();
	}

	/*
	gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
	g_TerrainDecalObject.Render( &g_CharacterEffectGroupManager.m_RenderStyleManager,
								 &g_CharacterEffectGroupManager.m_TextureManager, 295 );//83 );
	*/

#ifdef _XDWDEBUG	
	}
#endif
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	// Render monster
//	if( !m_bMiniGameDraw )
	{
		// Render dummy
		g_DummyList.Render();

		// Render seat 
		g_SeatList.Render();
		
		if(!m_bNPCDialogMode && !m_bFunctionalObjectWideView ) 
			g_Moblist.Render();				

		// Render npc
		g_Npclist.Render();
				
		// by mahwang
		//g_pLocalUser->ProcessCollectEvent();
	}

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	// Render user
	if(!m_bNPCDialogMode && !m_bFunctionalObjectWideView/*&& !m_bMiniGameDraw*/ )	// 대화모드일 때는 PC를 Rendering하지 않는다.
	{
		/*
		if( g_pLocalUser->GetMotionClass() == _XACTION_ATTACK &&  
			g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL )
		{
			g_ObjectBlurProcessManager.ProcessMotionBlur();
		}
		*/

		g_Userlist.Render();
	
		// Process radial blur effect
		/*
		if( g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY )
		{
			g_ObjectBlurProcessManager.SetBlurStep( 30 );
			g_ObjectBlurProcessManager.SetUVDelta( 0.019f );
			g_ObjectBlurProcessManager.SetBlendColor( 0xFFFF0000 );
			g_ObjectBlurProcessManager.ProcessRadialBlur();
		}
		else*/
		{
			if( g_pLocalUser->m_UseSpeedSkill || g_ObjectBlurProcessManager.m_iNumRadialBlurSteps > 1 )
			{
				if( !g_pLocalUser->m_UseSpeedSkill )
				{
					if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps > 3 )
					{
						g_ObjectBlurProcessManager.SetBlurStep( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps - 2 );
					}
					else
					{
						g_ObjectBlurProcessManager.SetBlurStep( 1 );
					}
				}

				g_ObjectBlurProcessManager.SetBlendColor( 0xFFFFFFFF );
				g_ObjectBlurProcessManager.SetUVDelta( 0.016f );
				
				g_ObjectBlurProcessManager.ProcessRadialBlur();
			}
		}
	}

	// Render world item list	
	if( !m_bNPCDialogMode && !m_bNPCDialogWideView && !m_bFunctionalObjectWideView/* && !m_bMiniGameDraw*/ )// 2004.06.01->oneway48 insert (hotblood insert->2004.07.27): NPC대화모드일때 아이템이 떨어진 것을 그리지 않는다.
		g_GameItemlist.Render();

	// Render realtime object...
	if( g_bUseRealtimeGenerateObject ) 
		g_LodTerrain.RenderRealtimeObject();

	g_PostProcessWorldEffectInstanceManager.Update( NULL, NULL, NULL, NULL );
	g_PostProcessWorldEffectInstanceManager.Render();

	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
						
	// Render battle effect

	g_RenderAlphaBlendFlag = FALSE;
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

#ifdef _XDWDEBUG		
	
	if( g_ViewNormalVector ) // 캐릭터의 노말벡터
	{	
		AXIS_VERTEX	line[2];
		// character specular light	
		line[0].v = g_pLocalUser->m_Position;	
		line[1].v = g_pLocalUser->m_Position + g_pLocalUser->m_CurrentPositionTerrainNormal * 1.0f;
		line[0].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );
		line[1].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );		
		
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		gpDev->SetTexture(0, NULL); 
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);		
		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	}

	if( g_LodTerrain.m_ViewOBBObject )
	{
		D3DXMATRIX	cameramat;
		D3DXMatrixTranslation( &cameramat, g_LodTerrain.m_3PCamera.m_CameraPosition.x,
										   g_LodTerrain.m_3PCamera.m_CameraPosition.y,
										   g_LodTerrain.m_3PCamera.m_CameraPosition.z );
		g_CameraOBB.m_OBBModel.Render( cameramat, g_CameraOBB.Axis() );
		
//		_XArea* pArea = g_AreaManager.GetArea( 0, 66 );
//
//		AXIS_VERTEX	line1[2];
//		AXIS_VERTEX	line2[2];
//		AXIS_VERTEX	line3[2];
//		AXIS_VERTEX	line4[2];
//		
//								
//		line1[0].v.x = pArea->m_Point[0].x;
//		line1[0].v.z = pArea->m_Point[0].z;
//		
//		line1[1].v.x = pArea->m_Point[1].x;
//		line1[1].v.z = pArea->m_Point[1].z;				
//		
//		line1[0].v.y = g_pLocalUser->m_Position.y;	
//		line1[1].v.y = g_pLocalUser->m_Position.y;
//		
//		line1[0].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );
//		line1[1].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );		
//		
//		line2[0].v.x = pArea->m_Point[1].x;
//		line2[0].v.z = pArea->m_Point[1].z;
//		
//		line2[1].v.x = pArea->m_Point[2].x;
//		line2[1].v.z = pArea->m_Point[2].z;				
//		
//		line2[0].v.y = g_pLocalUser->m_Position.y;	
//		line2[1].v.y = g_pLocalUser->m_Position.y;
//		
//		line2[0].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );
//		line2[1].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );		
//		
//		line3[0].v.x = pArea->m_Point[2].x;
//		line3[0].v.z = pArea->m_Point[2].z;
//		
//		line3[1].v.x = pArea->m_Point[3].x;
//		line3[1].v.z = pArea->m_Point[3].z;				
//		
//		line3[0].v.y = g_pLocalUser->m_Position.y;	
//		line3[1].v.y = g_pLocalUser->m_Position.y;
//		
//		line3[0].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );
//		line3[1].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );		
//		
//		line4[0].v.x = pArea->m_Point[3].x;
//		line4[0].v.z = pArea->m_Point[3].z;
//		
//		line4[1].v.x = pArea->m_Point[0].x;
//		line4[1].v.z = pArea->m_Point[0].z;				
//		
//		line4[0].v.y = g_pLocalUser->m_Position.y;	
//		line4[1].v.y = g_pLocalUser->m_Position.y;
//		
//		line4[0].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );
//		line4[1].color = D3DCOLOR_ARGB( 255, 255, 0, 0 );		
//		
//
//		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
//		gpDev->SetTexture(0, NULL); 
//		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
//		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
//		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
//		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line1, sizeof(AXIS_VERTEX));
//		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line2, sizeof(AXIS_VERTEX));
//		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line3, sizeof(AXIS_VERTEX));
//		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line4, sizeof(AXIS_VERTEX));
//		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);		
//		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	}

	if( g_ViewPathFindInfo ) g_pLocalUser->DrawMovePath();

#endif
	
	// Render lens flare ( flare only )
	if( g_EnvironmentManager.m_Environment == _XE_DAY )
	{
		if(g_EnvironmentManager.m_ViewLensFlare && g_EnvironmentManager.m_SkyBoxHeightOffset < 1000.0f ) g_EnvironmentManager.m_LensFlare.Render();
	}

#ifdef _XTS_MOTIONBLURTEST
	g_ScreenMotionBlurEffectManager.EndBlur();
#endif

	g_PostProcessManager.EndPostProcess();

	if(!m_bNPCDialogMode && !m_bNPCDialogWideView && !m_bFunctionalObjectWideView/* && !m_bMiniGameDraw*/ )	//2004.06.19->oneway48 close3-1 hotblood->2004.07.27
		g_Userlist.RenderUserEffectAndToolTip();
			
	// Display last picked object information tooltip...
	DrawPickedObjectInfoTooltip();	

#ifdef _XDWDEBUG	

	//gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );

	if( m_ViewPathObject ) 
	{
		//gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		g_LodTerrain.m_PathObjectManager->Draw();
	}
			
	if( m_ViewLightConeModel )
	{		
		//gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		AXIS_VERTEX	line[2];
		// character specular light			
		line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
		line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
			
		D3DXVECTOR3 vecFrom;
		D3DXVECTOR3 vecAt;
		D3DXMATRIX matWorld;
		D3DXVECTOR3 vecUp( 0, 1, 0);
		D3DXMATRIX matWorldInv;
		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixLookAtLH( &matWorldInv, &vecFrom, &vecAt, &vecUp);
		D3DXMatrixInverse( &matWorld, NULL, &matWorldInv);
				
		g_TerrainTextureArchive.SetTexture( m_LightConeTextureIndex );	
		gpDev->SetTransform( D3DTS_WORLD, &matWorld );
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pLightConeMesh->DrawSubset(0);

		gpDev->SetTexture(0, NULL); 

		// sun & moon light			
		D3DXVECTOR3 lightPos = g_EnvironmentManager.m_ObjectLight.Position;
		vecFrom = (lightPos - g_pLocalUser->m_Position);
		D3DXVec3Normalize( &vecFrom, &vecFrom );
		vecFrom *= 20.0f;
		vecFrom = g_pLocalUser->m_Position + vecFrom;		
			
		vecAt	= D3DXVECTOR3 (   vecFrom.x + g_pLocalUser->m_Position.x - lightPos.x, 
								  vecFrom.y + g_pLocalUser->m_Position.y - lightPos.y,
								  vecFrom.z + g_pLocalUser->m_Position.z - lightPos.z );

		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixLookAtLH( &matWorldInv, &vecFrom, &vecAt, &vecUp);
		D3DXMatrixInverse( &matWorld, NULL, &matWorldInv);	
		gpDev->SetTransform( D3DTS_WORLD, &matWorld );
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pLightConeMesh->DrawSubset(0);

		line[0].v = g_pLocalUser->m_Position;	
		line[1].v = vecFrom;	
		line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 64 );
		line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 64 );
		
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);

		// Real lighting position...
		D3DXVECTOR3 vBlockerCenter; //유저의 위치값 필요 
		vBlockerCenter.x	= g_pLocalUser->m_Position.x;
		vBlockerCenter.y	= g_pLocalUser->m_Position.y+0.75f;
		vBlockerCenter.z	= g_pLocalUser->m_Position.z;	

		D3DXVECTOR3		vOrig	= g_EnvironmentManager.m_ObjectLight.Position;
		D3DXVECTOR3		vEnd	= vBlockerCenter;
		
		D3DXVECTOR3		vLightPo;
		vLightPo	= vOrig + 0.98f*( vEnd - vOrig );
		vLightPo.y	= vBlockerCenter.y + 5.0f;
				
		vecFrom = (vLightPo - g_pLocalUser->m_Position);
		D3DXVec3Normalize( &vecFrom, &vecFrom );
		vecFrom *= 20.0f;
		vecFrom = g_pLocalUser->m_Position + vecFrom;
		vecAt	= D3DXVECTOR3 (   vecFrom.x + vBlockerCenter.x - vLightPo.x, 
								  vecFrom.y + vBlockerCenter.y - vLightPo.y,
								  vecFrom.z + vBlockerCenter.z - vLightPo.z );

		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixLookAtLH( &matWorldInv, &vecFrom, &vecAt, &vecUp);
		D3DXMatrixInverse( &matWorld, NULL, &matWorldInv);	
		gpDev->SetTransform( D3DTS_WORLD, &matWorld );
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pLightConeMesh->DrawSubset(0);

		line[0].v = g_pLocalUser->m_Position;	
		line[1].v = vecFrom;	
		line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 64 );
		line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 64 );

		gpDev->SetTexture(0, NULL); 
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	}

	// Move point
	if( m_ViewArrowModel )
	{	
		gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );
		gpDev->SetRenderState( D3DRS_ZWRITEENABLE,true); 
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		AXIS_VERTEX	line[2];
		
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		D3DXMATRIX	m_MoveTargetMatrix;
		D3DXMatrixTranslation( &m_MoveTargetMatrix, g_pLocalUser->m_TargetPosition.x,					
													g_pLocalUser->m_TargetPosition.y,
													g_pLocalUser->m_TargetPosition.z );
		
		m_ArrowModel.Render( m_MoveTargetMatrix );

		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		line[0].v = g_pLocalUser->m_Position;
		line[0].color = D3DCOLOR_ARGB( 96, 64, 64, 255 );
		line[1].v = g_pLocalUser->m_TargetPosition;
		line[1].color = D3DCOLOR_ARGB( 96, 64, 64, 255 );

		line[0].v.y +=_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA / 2.0f + 0.3f;		
		line[1].v.y +=0.27f;
		
		gpDev->SetTexture(0, NULL);
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	}

	AXIS_VERTEX	line[2];

	if( m_ViewMouseRay )
	{
		POINT mousept = { gpInput->GetMouseVirScreenPos()->x, gpInput->GetMouseVirScreenPos()->z };
		GetVectorFromMousePos( mousept, g_vPickRayDir, g_vPickRayOrig );

		gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );
		gpDev->SetRenderState( D3DRS_ZWRITEENABLE,true); 
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE );
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		gpDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		
		D3DXMATRIX	mat;
		D3DXMatrixIdentity( &mat );
		gpDev->SetTransform( D3DTS_WORLD, &mat );
		line[0].v = g_vPickRayOrig;
		line[1].v = g_vPickRayOrig + g_vPickInfinityRayDir;

		/*line[0].v = g_pLocalUser->m_Position - D3DXVECTOR3( 0, 0, 1.0f ) * 30.0f;
		line[0].v.y += 0.5f;
		line[1].v = g_pLocalUser->m_Position + D3DXVECTOR3( 0, 0, 1.0f ) * 30.0f;
		line[1].v.y = line[0].v.y;*/

		line[0].color = D3DCOLOR_ARGB( 255, 255, 0, 255 );
		line[1].color = D3DCOLOR_ARGB( 255, 0, 255, 255 );

		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX ); 
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		
		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	}	

#endif
	
#ifdef _XDWDEBUG
	if( g_LodTerrain.m_ObjectQuadTree.m_ViewFrustum )
	{	
		D3DXMATRIX mat;
		D3DXMatrixIdentity( &mat );	
		gpDev->SetTransform( D3DTS_WORLD, &mat );
		
		AXIS_VERTEX verts[6];
		
		verts[0].color = 0x77DDDD00;
		verts[1].color = 0x77DDDD00;
		verts[2].color = 0x77DDDD00;
		verts[3].color = 0x77DDDD00;
		verts[4].color = 0x77DDDD00;
		verts[5].color = 0x77DDDD00;
		
		
		verts[0].v = D3DXVECTOR3( g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.x, g_LodTerrain.m_ObjectQuadTree.m_FrustumHeight, g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.z);
		
		verts[1].v = D3DXVECTOR3(g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.x + -g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nz[0] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane, 
			g_LodTerrain.m_ObjectQuadTree.m_FrustumHeight, 
			g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.z + g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nx[0] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane );
		
		verts[2].v = D3DXVECTOR3(g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.x, g_LodTerrain.m_ObjectQuadTree.m_FrustumHeight, g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.z);
		verts[3].v = D3DXVECTOR3(g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.x + g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nz[1] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane, 
			g_LodTerrain.m_ObjectQuadTree.m_FrustumHeight, 
			g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.z + -g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nx[1] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane );
		
		verts[4].v = D3DXVECTOR3(g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.x + -g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nz[0] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane, 
			g_LodTerrain.m_ObjectQuadTree.m_FrustumHeight, 
			g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.z + g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nx[0] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane );
		verts[5].v = D3DXVECTOR3(g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.x + g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nz[1] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane, 
			g_LodTerrain.m_ObjectQuadTree.m_FrustumHeight, 
			g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.z + -g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nx[1] * g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane );
		
		
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		gpDev->SetTexture(0, NULL); 	
		gpDev->SetTexture(1, NULL); 	
		gpDev->SetTexture(2, NULL); 	
		gpDev->SetTexture(3, NULL); 	
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
		gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE );
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		gpDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		gpDev->DrawPrimitiveUP(D3DPT_LINELIST, 3, verts, sizeof(AXIS_VERTEX));
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	}

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(m_ViewRuntimeDebugData) DrawDebugState();

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	if( m_ViewFPS && !g_LodTerrain.viewterrainrenderingtime )
	{
		g_XBaseFont->Print( 0, gnHeight>>2, 1.0f, "FPS: %d", gnFPS );
		g_XBaseFont->Flush();
	}
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
#endif


/*
#ifdef _XGMCLIENT
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 274+_XDEF_DBGVIEWYOFFSET, 1.0f, "FPS : %d, ElapsedT : %f", gnFPS, g_fElapsedFrameTime );
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
#endif
*/

/*
	if( g_ViewLocalUserBillingInfo )
	{
#define _XDEF_BILLINGINFOXOFFSET	820
#define _XDEF_BILLINGINFOYOFFSET	460
		
		g_XBaseFont->SetColor( 0xFFFFFFFF );
				
		switch( g_LocalUserBillingInformations.BillingStatusWho ) 
		{
		case _XB_BillingStatusWho_Home :
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET, 1.0f, "접속자 상태 : 개인 사용자 [H]" );
			break;
		case _XB_BillingStatusWho_PC	:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET, 1.0f, "접속자 상태 : PC방 사용자 [P]" );
			break;
		case _XB_BillingStatusWho_ISP	:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET, 1.0f, "접속자 상태 : ISP 사용자  [S]" );
			break;
		case _XB_BillingStatusWho_Nothing	:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET, 1.0f, "접속자 상태 : 미인증 사용자 [0]" );
			break;
		}

		switch( g_LocalUserBillingInformations.BillingStatusWhere ) 
		{
		case _XB_BillingStatusWhere_Home	:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 16, 1.0f, "접속   장소 : 집 [H]" );
			break;
		case _XB_BillingStatusWhere_PCRoom	:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 16, 1.0f, "접속   장소 : PC방 [P]" );
			break;
		case _XB_BillingStatusWhere_ISP		:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 16, 1.0f, "접속   장소 : ISP [S]" );
			break;
		}
		
		switch( g_LocalUserBillingInformations.BillingETCData )
		{
		case 0 :
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 32, 1.0f, "ETC : 0" );
			break;
		default	:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 32, 1.0f, "ETC : %d", g_LocalUserBillingInformations.BillingETCData );
			break;
		}

		switch( g_LocalUserBillingInformations.BillPayType )
		{
		case _XB_BillingPaymentType_DO :
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 48, 1.0f, "지불   방식 : 직불 [DO]" );
			break;
		case _XB_BillingPaymentType_FO	:
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 48, 1.0f, "지불   방식 : 무료 [FO]" );
			break;
		}
		
		switch( g_LocalUserBillingInformations.BillMethod ) 
		{
		case _XB_BillingMethod_Free    :
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 64, 1.0f, "상품   종류 : 무료 상품 [N]" );
			break;
		case _XB_BillingMethod_Basic   :
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 64, 1.0f, "상품   종류 : 기본 상품 [F]" );
			break;
		case _XB_BillingMethod_Premium :
			g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 64, 1.0f, "상품   종류 : 프리미엄 상품 [P]" );
			break;
		}
		
		g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 80, 1.0f, "과금 종료일 : %d/%d/%d", 
							g_LocalUserBillingInformations.BillExpireYear, 
							g_LocalUserBillingInformations.BillExpireMonth,
							g_LocalUserBillingInformations.BillExpireDay );


		g_XBaseFont->Print( _XDEF_BILLINGINFOXOFFSET, _XDEF_BILLINGINFOYOFFSET + 96, 1.0f, "남은 시간 : %d", 
							g_LocalUserBillingInformations.BillRemain );	
		g_XBaseFont->Flush();
		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	}	
*/

#define _XDEF_USEAREAFUNCTION 
	
	// 합비쪽 공통 수구 적용 플래그
#ifdef _XDEF_USEAREAFUNCTION 

	if( !g_DrawFadeInOutFlag && g_ClickedWarpZoneIndex > -1 )
	{
		_XArea* pCurrentFocusedArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_MAPWARP, g_ClickedWarpZoneIndex );
		
		if( pCurrentFocusedArea )
		{
			if( !(m_bNPCDialogMode || m_bNPCDialogWideView || m_MoveServer || m_LockMoveProcess ) )
			{
				if( !g_WarpProcessFadeInOutFlag )
				{	
					if( g_pLocalUser->m_bIsBossBattle ) // 보스맵에서는 워프 영역이 탈출 지역 밖에 없다.
					{
						if( g_pLocalUser->m_bExitBossBattleRoom )
						{
							if( g_AreaManager.CheckInArea( g_pLocalUser->m_Position , pCurrentFocusedArea) )
							{
								g_WarpProcessFadeInOutFlag = TRUE;
								g_FadeInOutLevel = 0;
								m_WarpStartTime = g_LocalSystemTime; 
								//생사결일때는..다음 존인덱스를 세팅하면 안됨.
								
								g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_LEAVE_REQ_C_S, 0 );//탈출 
								m_LockMoveProcess = TRUE;
							}							
						}
					}				
					else
					{
						if( g_AreaManager.CheckInArea( g_pLocalUser->m_Position , pCurrentFocusedArea) )
						{
							BOOL clancheck = TRUE;
							int  clantype[_XGROUP_MAXGROUPCOUNT] = 
							{
								0,1,-1,1,-1,1,-1,0,0
							};
							
							if( pCurrentFocusedArea->m_SpecialFunction == 0 ) // 일반 수구 
							{
								if( g_CurrentZoneInfoIndex == 17 /*합비*/ ) // 합비같은 공용 지역일 경우 흑/백도 체크 
								{
									if(g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].clantype != 0)
									{
										if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != 0 ) // 낭인이 아닐 경우 
										{
											if( g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].clantype != // 넘어갈 존의 클랜타입 검사
												clantype[g_pLocalUser->m_CharacterInfo.groupindex] ) // 현재 유저의 클랜타입.
											{   // 흑도는 백도로 백도는 흑도로 못넘어감.
												if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] == -1 )
												{
													g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2446), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
													clancheck = FALSE;
													g_ClickedWarpZoneIndex = -1;
												}
												else
												{
													g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2447), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
													clancheck = FALSE;
													g_ClickedWarpZoneIndex = -1;
												}
											}
										}
									}
								}								
							}
							else if( pCurrentFocusedArea->m_SpecialFunction == 1 ) // 흑도 수구 
							{
								if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != 0 ) // 낭인이 아닐 경우 
								{
									if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != -1 )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2447), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
										clancheck = FALSE;
										g_ClickedWarpZoneIndex = -1;
									}
									else
									{
										if(g_pBlackClanAreaSelect_Window)
										{
											g_pBlackClanAreaSelect_Window->m_pFocusedArea = pCurrentFocusedArea;
											if( !g_pBlackClanAreaSelect_Window->GetShowStatus() )
											{
												g_pBlackClanAreaSelect_Window->ShowWindow( TRUE );
												g_pBlackClanAreaSelect_Window->SelectClan( g_pLocalUser->m_CharacterInfo.groupindex );

												if( g_pBlackClanAreaSelect_Window->m_pListBox )
												{
													m_NextWarpZoneIndex_COMMONGATE = g_pBlackClanAreaSelect_Window->m_pListBox->GetItemCommandID( 
																					   g_pBlackClanAreaSelect_Window->m_pListBox->GetSelectedItem() ) - 1; 
												}
											}
										}									
										
										g_ClickedWarpZoneIndex = -1;
										clancheck = FALSE;
									}
								}
								else // 낭인일 경우 선택리스트 표시
								{
									if(g_pBlackClanAreaSelect_Window)
									{
										g_pBlackClanAreaSelect_Window->m_pFocusedArea = pCurrentFocusedArea;
										
										if( !g_pBlackClanAreaSelect_Window->GetShowStatus() )
										{
											g_pBlackClanAreaSelect_Window->ShowWindow( TRUE );
											g_pBlackClanAreaSelect_Window->SelectClan( 0 );

											if( g_pBlackClanAreaSelect_Window->m_pListBox )
											{
												m_NextWarpZoneIndex_COMMONGATE = g_pBlackClanAreaSelect_Window->m_pListBox->GetItemCommandID( 
													g_pBlackClanAreaSelect_Window->m_pListBox->GetSelectedItem() ) - 1; 
											}
										}
									}									

									g_ClickedWarpZoneIndex = -1;
									clancheck = FALSE;
								}
							}
							else if( pCurrentFocusedArea->m_SpecialFunction == 2 ) // 백도 수구 
							{
								if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != 0 ) // 낭인이 아닐 경우 
								{
									if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != 1 )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2446), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
										clancheck = FALSE;
										g_ClickedWarpZoneIndex = -1;										
									}
									else
									{
										if(g_pWhiteClanAreaSelect_Window)
										{
											if( !g_pWhiteClanAreaSelect_Window->GetShowStatus() )
											{
												g_pWhiteClanAreaSelect_Window->m_pFocusedArea = pCurrentFocusedArea;
												g_pWhiteClanAreaSelect_Window->ShowWindow( TRUE );
												g_pWhiteClanAreaSelect_Window->SelectClan( g_pLocalUser->m_CharacterInfo.groupindex );

												if( g_pWhiteClanAreaSelect_Window->m_pListBox )
												{
													m_NextWarpZoneIndex_COMMONGATE = g_pWhiteClanAreaSelect_Window->m_pListBox->GetItemCommandID( 
														g_pWhiteClanAreaSelect_Window->m_pListBox->GetSelectedItem() ) - 1; 
												}
											}
										}																		
										
										g_ClickedWarpZoneIndex = -1;
										clancheck = FALSE;
									}
								}
								else // 낭인일 경우 선택리스트 표시
								{
									if(g_pWhiteClanAreaSelect_Window)
									{
										g_pWhiteClanAreaSelect_Window->m_pFocusedArea = pCurrentFocusedArea;
										
										if( !g_pWhiteClanAreaSelect_Window->GetShowStatus() )
										{
											g_pWhiteClanAreaSelect_Window->ShowWindow( TRUE );
											g_pWhiteClanAreaSelect_Window->SelectClan( 0 );

											if( g_pWhiteClanAreaSelect_Window->m_pListBox )
											{
												m_NextWarpZoneIndex_COMMONGATE = g_pWhiteClanAreaSelect_Window->m_pListBox->GetItemCommandID( 
													g_pWhiteClanAreaSelect_Window->m_pListBox->GetSelectedItem() ) - 1; 
											}
										}
									}																		

									g_ClickedWarpZoneIndex = -1;
									clancheck = FALSE;
								}																
							}
							else if( pCurrentFocusedArea->m_SpecialFunction == 4 ) // 흑도 지정영역 이동 수구 
							{
								if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != 0 ) // 낭인이 아닐 경우 
								{
									if( g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].clantype == 0 )
									{
										if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != -1 )
										{
											g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2447), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
											clancheck = FALSE;
											g_ClickedWarpZoneIndex = -1;
										}

									}
									else if( g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].clantype != // 넘어갈 존의 클랜타입 검사
										clantype[g_pLocalUser->m_CharacterInfo.groupindex] ) // 현재 유저의 클랜타입.
									{   // 백도는 흑도로 못넘어감.
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2447), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
										clancheck = FALSE;
										g_ClickedWarpZoneIndex = -1;
									}
								}
							}
							else if( pCurrentFocusedArea->m_SpecialFunction == 5 ) // 백도 지정영역 이동 수구 
							{
								
								if( g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].clantype == 0 )
								{
									if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] == -1 )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2446), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
										clancheck = FALSE;
										g_ClickedWarpZoneIndex = -1;
									}
									
								}
								else if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != 0 ) // 낭인이 아닐 경우 
								{
									if( g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].clantype != // 넘어갈 존의 클랜타입 검사
										clantype[g_pLocalUser->m_CharacterInfo.groupindex] ) // 현재 유저의 클랜타입.
									{   // 백도는 흑도로 못넘어감.
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2446), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
										clancheck = FALSE;
										g_ClickedWarpZoneIndex = -1;
									}
								}								
							}
							else if( pCurrentFocusedArea->m_SpecialFunction == 6 )//다중맵이동 수구일때
							{
								if( !g_pMultiAreaSelect_Window->GetShowStatus() )
								{
									int listcount = 0;

									
									if( pCurrentFocusedArea->m_MultiTargetWarpzone1 > 0 && pCurrentFocusedArea->m_WarpZoneName1[0] != 0)
									{
										bool vaild = false;
										switch(pCurrentFocusedArea->m_MultiWarpzonePassage1) {
										case 0://전부통과
											{
												vaild = true;
											}
											break;
										case 1://흑도통과
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
												{
													vaild = true;
												}
											}
											break;
										case 2://백도통과
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
												{
													vaild = true;
												}
											}
											break;
										case 3://소림
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM )
												{
													vaild = true;
												}
											}
											break;
										case 4://개방
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG )
												{
													vaild = true;
												}
											}
											break;
										case 5://녹림
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM )
												{
													vaild = true;
												}
											}
											break;
										case 6://비궁
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG )
												{
													vaild = true;
												}
											}
											break;
										case 7://무당
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG )
												{
													vaild = true;
												}
											}
											break;
										case 8://마교
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
												{
													vaild = true;
												}
											}
											break;
										}

										if( vaild )
										{
											g_pMultiAreaSelect_Window->InsertAreaList( pCurrentFocusedArea->m_MultiTargetZone1, pCurrentFocusedArea->m_WarpZoneName1 );
											++listcount;
										}
									}
									if( pCurrentFocusedArea->m_MultiTargetWarpzone2 > 0 && pCurrentFocusedArea->m_WarpZoneName2[0] != 0)
									{
										bool vaild = false;
										switch(pCurrentFocusedArea->m_MultiWarpzonePassage2) {
										case 0://전부통과
											{
												vaild = true;
											}
											break;
										case 1://흑도통과
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
												{
													vaild = true;
												}
											}
											break;
										case 2://백도통과
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
												{
													vaild = true;
												}
											}
											break;
										case 3://소림
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM )
												{
													vaild = true;
												}
											}
											break;
										case 4://개방
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG )
												{
													vaild = true;
												}
											}
											break;
										case 5://녹림
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM )
												{
													vaild = true;
												}
											}
											break;
										case 6://비궁
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG )
												{
													vaild = true;
												}
											}
											break;
										case 7://무당
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG )
												{
													vaild = true;
												}
											}
											break;
										case 8://마교
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
												{
													vaild = true;
												}
											}
											break;
										}
										
										if( vaild )
										{
											g_pMultiAreaSelect_Window->InsertAreaList( pCurrentFocusedArea->m_MultiTargetZone2, pCurrentFocusedArea->m_WarpZoneName2 );
											++listcount;
										}
									}
									if( pCurrentFocusedArea->m_MultiTargetWarpzone3 > 0 && pCurrentFocusedArea->m_WarpZoneName3[0] != 0)
									{
										bool vaild = false;
										switch(pCurrentFocusedArea->m_MultiWarpzonePassage3) {
										case 0://전부통과
											{
												vaild = true;
											}
											break;
										case 1://흑도통과
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
												{
													vaild = true;
												}
											}
											break;
										case 2://백도통과
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
												{
													vaild = true;
												}
											}
											break;
										case 3://소림
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM )
												{
													vaild = true;
												}
											}
											break;
										case 4://개방
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG )
												{
													vaild = true;
												}
											}
											break;
										case 5://녹림
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM )
												{
													vaild = true;
												}
											}
											break;
										case 6://비궁
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG )
												{
													vaild = true;
												}
											}
											break;
										case 7://무당
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG )
												{
													vaild = true;
												}
											}
											break;
										case 8://마교
											{
												if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN ||
													g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
												{
													vaild = true;
												}
											}
											break;
										}
										
										if( vaild )
										{
											g_pMultiAreaSelect_Window->InsertAreaList( pCurrentFocusedArea->m_MultiTargetZone3, pCurrentFocusedArea->m_WarpZoneName3 );
											++listcount;
										}
									}
									
									if( listcount != 0 )
									{
										if(g_pMultiAreaSelect_Window)
										{
											g_pMultiAreaSelect_Window->m_pFocusedArea = pCurrentFocusedArea;
											
											g_pMultiAreaSelect_Window->ShowWindow( TRUE );
											//g_pMultiAreaSelect_Window->SelectClan( g_pLocalUser->m_CharacterInfo.groupindex );
											
											if( g_pMultiAreaSelect_Window->m_pListBox )
											{
												m_NextWarpZoneIndex_COMMONGATE = g_pMultiAreaSelect_Window->m_pListBox->GetItemCommandID( 
													g_pMultiAreaSelect_Window->m_pListBox->GetSelectedItem() ) - 1; 
											}
										}						
									}
									else // 이동리스트가 하나도 없을때 
									{	
										if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] == -1 )
										{
											g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2446), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );										
										}
										else if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] == 1 )
										{
											g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2447), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );											
										}		
									}

									g_ClickedWarpZoneIndex = -1;
									clancheck = FALSE;
									
								}
								else
								{
									g_ClickedWarpZoneIndex = -1;
									clancheck = FALSE;
								}
							}
															
							if( clancheck )
							{
#ifdef _XDEF_MOVE_PKSERVER_SELECT
								if(g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].pkclientinfo)
								{
									m_strMoveZoneInfo.currentwarpzone = pCurrentFocusedArea->m_Num;
									m_strMoveZoneInfo.targetzone = pCurrentFocusedArea->m_TargetZone+1;
									m_strMoveZoneInfo.nextwarpzone = pCurrentFocusedArea->m_TargetWarpzone;

									TCHAR tstr[256];
									_snprintf(tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_NEW_3198), g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].zonename);
									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tstr, TRUE, _XDEF_MG_MOVEPKSERVER_YES, _XDEF_MG_MOVEPKSERVER_NO);

									m_LockMoveProcess = TRUE;
								}
								else
								{

#endif
								g_WarpProcessFadeInOutFlag = TRUE;
								g_FadeInOutLevel = 0;
								
								m_PrevCheckedWarpingZoneTimer = g_LocalSystemTime;
								m_CurrentWarpZoneIndex = pCurrentFocusedArea->m_Num; //i;		 // 현재 서있는 곳의 포탈 인덱스 서버 전송용
								m_PrevZoneInfoIndex	   = g_CurrentZoneInfoIndex;
								g_CurrentZoneInfoIndex = pCurrentFocusedArea->m_TargetZone+1;    // 현재존을 다음존인덱스로 세팅한다. 실패일경우 되돌림.
								m_NextWarpZoneIndex	   = pCurrentFocusedArea->m_TargetWarpzone;	 // 다음 존의 리스폰 리젼 인덱스
								
								m_WarpStartTime = g_LocalSystemTime; 
								
								TCHAR tstr[256];
								_snprintf( tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename );
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );
								
								
								g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 4 );// 서버이동 중이라는 상태를 알려준다.
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MG_WARPINGNEXTZONE, 0), 0 );
								m_LockMoveProcess = TRUE;
#ifdef _XDEF_MOVE_PKSERVER_SELECT
								}
#endif
							}
						}
					}
				}
			}
		}		
	}

#else
	// 포탈에서 워프될때 워프존 안에 들어 오면 화면을 페이드 시키고 서버로 이동시그널 전송.
	if( !g_DrawFadeInOutFlag && g_ClickedWarpZoneIndex > -1 )
	{
		_XArea* pCurrentFocusedArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_MAPWARP, g_ClickedWarpZoneIndex );
		
		if( pCurrentFocusedArea )
		{
			if( !(m_bNPCDialogMode || m_bNPCDialogWideView || m_MoveServer || m_LockMoveProcess ) )
			{
				if( !g_WarpProcessFadeInOutFlag )
				{	
					if( g_pLocalUser->m_bIsBossBattle ) // 보스맵에서는 워프 영역이 탈출 지역 밖에 없다.
					{
						if( g_pLocalUser->m_bExitBossBattleRoom )
						{
							if( g_AreaManager.CheckInArea( g_pLocalUser->m_Position , pCurrentFocusedArea) )
							{
								g_WarpProcessFadeInOutFlag = TRUE;
								g_FadeInOutLevel = 0;
								m_WarpStartTime = g_LocalSystemTime; 
								//생사결일때는..다음 존인덱스를 세팅하면 안됨.
								
								g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_LEAVE_REQ_C_S, 0 );//탈출 
								m_LockMoveProcess = TRUE;
							}							
						}
					}				
					else
					{
						if( g_AreaManager.CheckInArea( g_pLocalUser->m_Position , pCurrentFocusedArea) )
						{
							BOOL clancheck = TRUE;
							if( g_CurrentZoneInfoIndex == 17 /*합비*/ ) // 합비같은 공용 지역일 경우 흑/백도 체크 
							{
								int clantype[_XGROUP_MAXGROUPCOUNT] = 
								{
									0,1,-1,1,-1,1,-1,0,0
								};

								if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] != 0 ) // 낭인이 아닐 경우 
								{
									if( g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].clantype != // 넘어갈 존의 클랜타입 검사
										clantype[g_pLocalUser->m_CharacterInfo.groupindex] ) // 현재 유저의 클랜타입.
									{   // 흑도는 백도로 백도는 흑도로 못넘어감.
										if( clantype[g_pLocalUser->m_CharacterInfo.groupindex] == -1 )
										{
											g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2446), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
											clancheck = FALSE;
											g_ClickedWarpZoneIndex = -1;
										}
										else
										{
											g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2447), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
											clancheck = FALSE;
											g_ClickedWarpZoneIndex = -1;
										}
									}
								}
							}

							if( clancheck )
							{
#ifdef _XDEF_MOVE_PKSERVER_SELECT
								if(g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].pkclientinfo)
								{
									m_strMoveZoneInfo.currentwarpzone = pCurrentFocusedArea->m_Num;
									m_strMoveZoneInfo.targetzone = pCurrentFocusedArea->m_TargetZone+1;
									m_strMoveZoneInfo.nextwarpzone = pCurrentFocusedArea->m_TargetWarpzone;

									TCHAR tstr[256];
									_snprintf(tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_NEW_3198), g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].zonename);
									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tstr, TRUE, _XDEF_MG_MOVEPKSERVER_YES, _XDEF_MG_MOVEPKSERVER_NO);

									m_LockMoveProcess = TRUE;
								}
								else
								{

#endif
								g_WarpProcessFadeInOutFlag = TRUE;
								g_FadeInOutLevel = 0;
								
								m_PrevCheckedWarpingZoneTimer = g_LocalSystemTime;
								m_CurrentWarpZoneIndex = pCurrentFocusedArea->m_Num; //i;		 // 현재 서있는 곳의 포탈 인덱스 서버 전송용
								m_PrevZoneInfoIndex	   = g_CurrentZoneInfoIndex;
								g_CurrentZoneInfoIndex = pCurrentFocusedArea->m_TargetZone+1;    // 현재존을 다음존인덱스로 세팅한다. 실패일경우 되돌림.
								m_NextWarpZoneIndex	   = pCurrentFocusedArea->m_TargetWarpzone;	 // 다음 존의 리스폰 리젼 인덱스

								m_WarpStartTime = g_LocalSystemTime; 

								TCHAR tstr[256];
								_snprintf( tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename );
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );

								
								g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 4 );// 서버이동 중이라는 상태를 알려준다.
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MG_WARPINGNEXTZONE, 0), 0 );
								m_LockMoveProcess = TRUE;
#ifdef _XDEF_MOVE_PKSERVER_SELECT
								}
#endif
							}
						}
					}
				}
			}
		}		
	}

#endif
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	
	if(g_pLocalUser->GetMotionClass() == _XACTION_DYING)
	{
		// 빈사 상태 처리
		DrawDyingState();

#ifdef _ACCLAIM_IGAADSYSTEM
		if( g_IGAADWrapper.GetShowStatus() )
		{
			g_IGAADWrapper.ShowWindow( FALSE );
		}
#endif
	}
	else if(g_pLocalUser->GetMotionClass() == _XACTION_DIE)
	{
		// 혼수 상태 처리
		DrawComaState();
	}
	else
	{
		// 일반 상태 처리
		if( m_ProcessLocalUserDieFlag ) 
		{
			ProcessLocalUserDie();
		}
		else if(g_DrawFadeInOutFlag)
		{
			DrawFadeInOut();
		}
		
		if( m_bNPCDialogWideView && m_Initialized )
		{
#ifdef _XDEF_USESCREEENMOTIONBLUR 
			g_ScrrenMotionBlurMode = FALSE; // 2004.06.01->oneway48 insert
#endif
			
			DrawNPCDialogWideView();
		}
		else if(m_bNPCFadeInOutFlag)
		{
			DrawNPCFadeInOut();
		}

		if( m_bFunctionalObjectWideView )
		{
			DrawFunctionalObjectWideView();
		}
		else if(m_bFunctionalObjectFadeInOut)
		{
			DrawFunctionalObjectFadeInOut();
		}

		
		if( g_ViewPoisonFog )
		{
			if( g_DeleteStartViewPoisonFog )
			{
				for( g_FadeInOutLevel = 255;  g_FadeInOutLevel >= 0;  g_FadeInOutLevel -= 10 )
				{
					DWORD color = 0xFFFFFF;
					D3DCOLOR color1;

					if(g_CurrentZoneInfoIndex == 54)
						color1 = (((DWORD)((g_FadeInOutLevel>>1) << 24))| color);
					else
						color1 = (((DWORD)(g_FadeInOutLevel << 24))| color);

					g_ScreenBurnOut[0].color = color1;
					g_ScreenBurnOut[1].color = color1;
					g_ScreenBurnOut[2].color = color1;
					g_ScreenBurnOut[3].color = color1;
					

				}
				
				if(g_FadeInOutLevel <= 0)
				{
					g_FadeInOutLevel = 0;	
					g_DeleteStartViewPoisonFog = FALSE;
					g_ViewPoisonFog = FALSE;
				}			
			}
			else
			{
				if(g_CurrentZoneInfoIndex == 54)
				{
					g_ScreenBurnOut[0].color = 0x7FFFFFFF;
					g_ScreenBurnOut[1].color = 0x7FFFFFFF;
					g_ScreenBurnOut[2].color = 0x7FFFFFFF;
					g_ScreenBurnOut[3].color = 0x7FFFFFFF;
				}
				else
				{
					g_ScreenBurnOut[0].color = 0xFFFFFFFF;
					g_ScreenBurnOut[1].color = 0xFFFFFFFF;
					g_ScreenBurnOut[2].color = 0xFFFFFFFF;
					g_ScreenBurnOut[3].color = 0xFFFFFFFF;
				}
			}
	

			if( g_ScreenBurnOut[0].tu >= 1.0f )
			{
				g_ScreenBurnOut[0].tu = 0.0f;
				g_ScreenBurnOut[1].tu = 1.0f;
				g_ScreenBurnOut[2].tu = 0.0f;
				g_ScreenBurnOut[3].tu = 1.0f;
			}			
			else
			{
				g_ScreenBurnOut[0].tu += g_fElapsedFrameMilisecondTime*0.05f;				
				g_ScreenBurnOut[1].tu = (g_ScreenBurnOut[0].tu + 1.0f);
				g_ScreenBurnOut[2].tu = g_ScreenBurnOut[0].tu;
				g_ScreenBurnOut[3].tu = (g_ScreenBurnOut[0].tu + 1.0f);
			}
			
			gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 14 );
			

			gpDev->SetTexture( 0, g_MainInterfaceTextureArchive.GetTexture(m_imagePoisonFogIndex) );
			
			gpDev->SetFVF(D3DFVF_XTLVERTEX);
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX));
			
			gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
			gpDev->SetRenderState(D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog);
			gpDev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		}
		else
		{			
			if(g_FadeInOutLevel <= 0)
			{
				g_FadeInOutLevel = 0;	
			}
			else
			{
			}
		}
		
		if(g_QuestScriptManager.GetQuestWideView())
		{
			if(m_pNPCScriptWindowPtr)
				m_pNPCScriptWindowPtr->Draw();
		}
		else
		{
			DrawInterface();

#ifdef _ACCLAIM_IGAADSYSTEM	
			if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_AREACHANGE )
			{
				g_IGAADWrapper.Render();
			}
			else if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_DEATH && 
					 m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_STATICAD )
			{
				D3DCOLOR color = ((DWORD) m_DeathStaticBackgroundAlphaLevel << 24);
				g_ScreenBurnOut[0].color = color;
				g_ScreenBurnOut[1].color = color;
				g_ScreenBurnOut[2].color = color;
				g_ScreenBurnOut[3].color = color;
				
				gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
				gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
				gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
				gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);				
				gpDev->SetTexture(0, NULL);				
				gpDev->SetFVF(D3DFVF_XTLVERTEX);
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX));				
				gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

				g_IGAADWrapper.Render();
			}
			else if( m_DeathStaticBackgroundAlphaLevel > 0 )
			{
				m_DeathStaticBackgroundAlphaLevel -= 5;

				if( m_DeathStaticBackgroundAlphaLevel < 0 )
					m_DeathStaticBackgroundAlphaLevel = 0;

				D3DCOLOR color = ((DWORD) m_DeathStaticBackgroundAlphaLevel << 24);
				g_ScreenBurnOut[0].color = color;
				g_ScreenBurnOut[1].color = color;
				g_ScreenBurnOut[2].color = color;
				g_ScreenBurnOut[3].color = color;
				
				gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
				gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
				gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
				gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);				
				gpDev->SetTexture(0, NULL);				
				gpDev->SetFVF(D3DFVF_XTLVERTEX);
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX));				
				gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			}
#endif
		}

#ifdef _ACCLAIM_IGAADSYSTEM		
		if( m_UseNPCAD )
		{
			if( m_bNPCDialogWideView ) //m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NPC )
			{
				D3DCOLOR color = ((DWORD) m_ADScreeenDimmedLevel << 24);
				g_ScreenBurnOut[0].color = color;
				g_ScreenBurnOut[1].color = color;
				g_ScreenBurnOut[2].color = color;
				g_ScreenBurnOut[3].color = color;
				
				gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
				gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
				gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
				gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);				
				gpDev->SetTexture(0, NULL);				
				gpDev->SetFVF(D3DFVF_XTLVERTEX);
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX));				
				gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
							
				if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NPC )
				{
					g_IGAADWrapper.Render();

					if( !g_IGAADWrapper.GetShowStatus() )
					{
						if( m_ADScreeenDimmedLevel > 0 )
						{
							m_ADScreeenDimmedLevel -= 3;

							if( m_ADScreeenDimmedLevel < 0 )
								m_ADScreeenDimmedLevel = 0;
						}
						else
						{
							m_ADScreeenDimmedLevel = 0;
							m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
						}
					}
				}
			}	
		}
#endif
		
		if( g_WarpProcessFadeInOutFlag )
		{
			g_FadeInOutLevel += 24;
			if(g_FadeInOutLevel >= 255)
			{
				g_FadeInOutLevel = 255;			
			}
			
			DrawFadeInOut();
		}
		
#ifdef _XMANAGER
		if( gpInput->CheckKeyPress(DIK_LCONTROL) )
		{
			//if( _XFullscreen )
			{	
				g_XBaseFont->SetColor( _XSC_INFORMATION );
				g_XBaseFont->Puts( gnWidth-200, gnHeight-14, "Administrator mode" );			

				TCHAR envstring[4][16] =
				{
					"DAY",
					"NIGHTFALL",
					"NIGHT",
					"DAWN"
				};

				g_XBaseFont->Print( 0, 0, 1.0f, "Env time : %s/%u [FPS:%d]", 
																   envstring[ g_LodTerrain.m_Environment ],
																   g_LodTerrain.m_EnvironmentTime, gnFPS );

				g_XBaseFont->Flush();
			}
		}
#else
#ifdef _XCHECKMEMORYUSAGE
		if( gpInput->CheckKeyPress(DIK_LCONTROL) )
		{
			g_XBaseFont->PrintAlign( 0, 0, 1.0f, _XFONT_ALIGNTYPE_LEFT , "FPS: %d, Memory Usage %d", gnFPS, GetMemoryUsageInfo() );
			g_XBaseFont->Flush();
		}
#endif
#endif
		
		if( !m_bNPCDialogMode && !m_bNPCDialogWideView && !m_bFunctionalObjectWideView)// 대화모드일때는 수구 이동을 막는다.
		{
			if( g_CurrentHoveredWarpzoneIndex > -1 && !g_LodTerrain.m_bInvisiblePortalMouseView )
			{
				if( !g_WarpProcessFadeInOutFlag )
				{
					if( !g_pBlackClanAreaSelect_Window->GetShowStatus() && // 통합수구 GUI가 안보일때만 수구 체크
						!g_pWhiteClanAreaSelect_Window->GetShowStatus()  )
					{
						_XArea* pCurrentFocusedArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_MAPWARP, g_CurrentHoveredWarpzoneIndex );
						ScrnPos* pScrnpos = gpInput->GetMouseVirScreenPos();

						if( pCurrentFocusedArea )
						{
							if( !g_pLocalUser->m_bIsBossBattle )
							{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
								if( pCurrentFocusedArea->m_SpecialFunction == 1 ) // 흑도 공통수구
								{
									g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, _XGETINTERFACETEXT( ID_STRING_NEW_2725 ), _XSC_INFORMATION, FALSE ); 
								}
								else if( pCurrentFocusedArea->m_SpecialFunction == 4 ) // 흑도 공통수구 타겟 지정
								{
									TCHAR szTootipTextBuffer[128];
									_snprintf( szTootipTextBuffer, sizeof(szTootipTextBuffer), _T("%s-%s"), _XGETINTERFACETEXT( ID_STRING_NEW_2725 ), pCurrentFocusedArea->m_Name );
									g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, szTootipTextBuffer, _XSC_INFORMATION, FALSE ); 
								}
								else if( pCurrentFocusedArea->m_SpecialFunction == 2 ) // 백도 공통수구  										 
								{
									g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, _XGETINTERFACETEXT( ID_STRING_NEW_2726 ), _XSC_INFORMATION, FALSE );	 
								}
								else if( pCurrentFocusedArea->m_SpecialFunction == 5 ) // 백도 공통수구 타겟 지정
								{
									TCHAR szTootipTextBuffer[128];
									_snprintf( szTootipTextBuffer, sizeof(szTootipTextBuffer), _T("%s-%s"), _XGETINTERFACETEXT( ID_STRING_NEW_2726 ), pCurrentFocusedArea->m_Name );
									g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, szTootipTextBuffer, _XSC_INFORMATION, FALSE );
								}
#else
								if( pCurrentFocusedArea->m_SpecialFunction == 1 || 									
									pCurrentFocusedArea->m_SpecialFunction == 4 ) // 흑도 공통수구 
								{
									g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, _XGETINTERFACETEXT( ID_STRING_NEW_2725 ), _XSC_INFORMATION, FALSE ); 
								}
								else if( pCurrentFocusedArea->m_SpecialFunction == 2 || 
										 pCurrentFocusedArea->m_SpecialFunction == 5 ) // 백도 공통수구 
								{
									g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, _XGETINTERFACETEXT( ID_STRING_NEW_2726 ), _XSC_INFORMATION, FALSE );	 
								}
#endif
								else if( pCurrentFocusedArea->m_SpecialFunction == 6 ) // 멀티수구
								{
									if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || 
										g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
										g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
										g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )//타국가 임시 처리
									{
										g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_WARPZONE), _XSC_INFORMATION, FALSE );
									}
									else
									{
										g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, _XGETINTERFACETEXT( ID_STRING_NPCSCRIPT_WARPZONE ), _XSC_INFORMATION, FALSE );	 
//										TCHAR messagestring[128];
//										memset( messagestring, 0, 128 );
//										sprintf( messagestring,  pCurrentFocusedArea->m_Name ); // %s 입구
//										g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, messagestring, _XSC_INFORMATION, FALSE );							
									}
								}
								else
								{									
									TCHAR messagestring[128];
									memset( messagestring, 0, sizeof(messagestring) );
									_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ZONE), g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].zonename ); // %s 입구
									g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, messagestring, _XSC_INFORMATION, FALSE );							
								}
							}
							else
							{
								g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, _XGETINTERFACETEXT(ID_STRING_MAINGAME_1819), _XSC_INFORMATION, FALSE );
							}

							CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
							eodapp->ReadyCursor( _XMOUSETYPE_GOTOGATE );
						}
					}
				}
			}

		}
		if( !g_WarpProcessFadeInOutFlag ) g_IMEWindow.Draw();
					
		g_MessageBox.Draw();

		if( !g_WarpProcessFadeInOutFlag ) g_InterfaceTooltip.Draw();
		
		// Pk Zone일 때 메세지 처리
#ifdef _XTS_PK
#ifdef _XTS_PKZONE_SERVERSETTING
		if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo)
#else
		if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo)
#endif
		{
			DWORD elapsedtime = g_LocalSystemTime - m_dwMapEnteringTime;
			if(elapsedtime <= 30000)
			{
				g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
				g_XLargeFont->SetColor( D3DCOLOR_ARGB( 255 , 250, 10, 10 ) );
				g_XLargeFont->Puts_SeparateAlign((gnWidth>>1) , (gnHeight>>1)-145-70, _XGETINTERFACETEXT(ID_STRING_NEW_2724), 500, _XFONT_ALIGNTYPE_CENTER);
				g_XLargeFont->Flush();
				g_XLargeFont->DisableGrowMode();
			}
		}
#endif

#ifdef _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI
		g_CBManager.Draw();
#endif
		
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		g_MatchEventManager.Draw();
#endif

#ifdef _XDWDEBUG	

		
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 ); 
		
		D3DXVECTOR2 pos;
		pos.x = 100;//gnWidth  - g_ShadowTextureSize-1;
		pos.y = 100;//gnHeight - g_ShadowTextureSize-1;
		//g_pd3dxSprite->Draw(g_HTMLWrapper.m_pHTMLTexture, NULL, NULL, NULL, NULL, &pos, D3DCOLOR_ARGB( 255, 255,255, 255 ) );		
		/*
		_XLODTILE* pTile = g_LodTerrain.GetTerrainTile( g_pLocalUser->m_Position );
		if(pTile)
		{
			D3DXVECTOR2 pos;
			pos.x = 100;//gnWidth  - g_ShadowTextureSize-1;
			pos.y = 100;//gnHeight - g_ShadowTextureSize-1;
			g_pd3dxSprite->Draw(pTile->m_pPreRenderedTextureObject, NULL, NULL, NULL, NULL, &pos, D3DCOLOR_ARGB( 255, 255,255, 255 ) );		
		}
		*/
		
		if( m_ViewShadowTexture )
		{
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 ); 

			D3DXVECTOR2 pos;
			pos.x = 100;//gnWidth  - g_ShadowTextureSize-1;
			pos.y = 100;//gnHeight - g_ShadowTextureSize-1;
			g_pd3dxSprite->Draw(g_lpShadowTexture, NULL, NULL, NULL, NULL, &pos, D3DCOLOR_ARGB( 255, 255,255, 255 ) );
		}
		extern BOOL g_DrawReflectionMap;
		if( g_DrawReflectionMap )
		{	
			D3DXVECTOR2 pos;
			pos.x = 0; pos.y = 0;
			extern LPDIRECT3DTEXTURE9 g_pWaterEnvRenderTexture;
			if(g_pWaterEnvRenderTexture)
				g_pd3dxSprite->Draw(g_pWaterEnvRenderTexture, NULL, NULL, NULL, NULL, &pos, D3DCOLOR_ARGB( 255, 255,255, 255 ) );
		}

		extern BOOL g_DrawGraphLog;
		if( g_DrawGraphLog )
		{	
			DrawGraphLog();
		}

		extern BOOL g_DrawEnvSoundObject;
		extern int	g_EnvSoundObjectViewFilter;
		extern void _XDrawCircle( D3DXVECTOR3 *pCenter, FLOAT fRadius, UINT nLineSegs,DWORD color );

		if( g_DrawEnvSoundObject )
		{
			D3DCOLOR g_SoundEventTypeStringColor[5] =
			{
					D3DCOLOR_ARGB(255,255,255,255),
					D3DCOLOR_ARGB(255,0,0,255),
					D3DCOLOR_ARGB(255,255,255,0),
					D3DCOLOR_ARGB(255,255,0,255),
					D3DCOLOR_ARGB(255,0,255,0)
			};

			TCHAR envstring[5][10];
			memset(envstring, 0, sizeof(TCHAR)*5*10);
			strcpy( envstring[0], _XGETINTERFACETEXT(ID_STRING_MAINGAME_2398) );
			strcpy( envstring[1], _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_DAWN) );
			strcpy( envstring[2], _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_DAY) );
			strcpy( envstring[3], _XGETINTERFACETEXT(ID_STRING_MAINGAME_2399) );
			strcpy( envstring[4], _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NIGHT) );
			
			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);
			gpDev->SetTransform( D3DTS_WORLD, &matWorld );
			gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

			D3DXVECTOR3 vPosition;
			FLOAT fDistance;
			_XSoundObjectManager::svXObject::iterator it;
			for (it = g_LodTerrain.m_SoundObjectManager.m_svXObject.begin(); it != g_LodTerrain.m_SoundObjectManager.m_svXObject.end(); ++ it)
			{
				_XSoundObject* pSoundObject = (*it);

				if( g_EnvSoundObjectViewFilter != -1 )
				{
					if( g_EnvSoundObjectViewFilter == 0 ) // day
					{
						if( pSoundObject->m_byEvent != _XSoundObject::EVENT_DAY )
						{
							continue;
						}
					}						
					else if( g_EnvSoundObjectViewFilter == 1 ) // nightfall
					{
						if( pSoundObject->m_byEvent != _XSoundObject::EVENT_NIGHTFALL )
						{
							continue;
						}
					}
					else if( g_EnvSoundObjectViewFilter == 2 ) // night
					{
						if( pSoundObject->m_byEvent != _XSoundObject::EVENT_NIGHT )
						{
							continue;
						}
					}
					else if( g_EnvSoundObjectViewFilter == 3 ) // dawn
					{
						if( pSoundObject->m_byEvent != _XSoundObject::EVENT_DAWN )
						{
							continue;
						}
					}
				}

				vPosition.x = pSoundObject->m_matWorldPosition._41;
				vPosition.y = pSoundObject->m_matWorldPosition._42;
				vPosition.z = pSoundObject->m_matWorldPosition._43;

				FLOAT dx = (vPosition.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
				FLOAT dz = (vPosition.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
				fDistance = _XFastDistance( dx , dz );

				BOOL insidefrustum = 
					( (fDistance < g_LodTerrain.m_3PCamera.GetDistance() + 200.0f ) &&
					  ( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( vPosition, pSoundObject->m_byDistance ) != NOT_VISIBLE ) );
				
				if( !insidefrustum ) continue;

				_XDrawCircle(&vPosition, pSoundObject->m_byDistance, 64, g_SoundEventTypeStringColor[pSoundObject->m_byEvent]);

				D3DXVECTOR3 screenpos;
				D3DXVec3Project( &screenpos, &vPosition, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
				
				g_XBaseFont->SetColor(g_SoundEventTypeStringColor[pSoundObject->m_byEvent]);
				if (pSoundObject->m_byAttribute & _XSoundObject::ATTRIBUTE_REPEAT)
					g_XBaseFont->PrintAlign( screenpos.x, screenpos.y, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s : %s : Repeate", envstring[pSoundObject->m_byEvent], g_pEnvironmentSound[pSoundObject->m_ObjectIndex].envsoundname );
				else
					g_XBaseFont->PrintAlign( screenpos.x, screenpos.y, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s : %s : int:%d", envstring[pSoundObject->m_byEvent], g_pEnvironmentSound[pSoundObject->m_ObjectIndex].envsoundname, pSoundObject->m_byInterval );
			}

			g_XBaseFont->Flush();
		}

#else
#ifdef _XGMCLIENT
		extern BOOL g_DrawGraphLog;
		if( g_DrawGraphLog )
		{	
			DrawGraphLog();
		}		
#endif	
#endif

		if(g_QuestScriptManager.GetQuestWideView())
			DrawQuestWideView();
/*
		g_XBaseFont->Print( gnWidth - 74, 3, 1.0f, "FPS: %d", gnFPS );
		g_XBaseFont->Flush();
*/
		g_ParticleEffectManager.Render();

	}

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	
	// Draw AD Border
	if( g_ViewRubiconAD )
	{
		_XDrawSolidBar( 0.0f, 0.0f, gnWidth, g_RubiconADWindowSize.cy, 0xFF000000 );
		
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->SetBoldMode(FALSE);
		g_XBaseFont->Puts( 16, 20, _T("This ad is clickable!") );
		g_XBaseFont->Puts( 16, 40, _T("Browser may open") );
		g_XBaseFont->Puts( 16, 54, _T("behind game.") );

		g_XBaseFont->Puts( gnWidth-140, 20, _T("Purchase Premium") );
		g_XBaseFont->Puts( gnWidth-140, 32, _T("Package to disable ads.") );
		g_XBaseFont->Puts( gnWidth-140, 56, _T("BUY NOW!") );

		g_XBaseFont->Flush();

		int textwidth = g_XBaseFont->GetWidth( _T("BUY NOW!") );
		if( gpInput->CheckMousePos( gnWidth-140, 56, gnWidth-140+textwidth, 56 + g_XBaseFont->GetHeight() ) )
		{
			_XDrawLine2D( (FLOAT)(gnWidth-140), (FLOAT)(51 + g_XBaseFont->GetHeight()), (FLOAT)(gnWidth-140+textwidth), (FLOAT)(51 + g_XBaseFont->GetHeight()), 
						  0xFFFFFF00,0xFFFFFF00 );
			g_XBaseFont->SetColor( 0xFFFFFF00 );
			g_XBaseFont->Puts( gnWidth-140, 56, _T("BUY NOW!") );
			g_XBaseFont->Flush();

			if( gpInput->GetMouseState()->bButton[0] && ( g_CurrentFocusedObject == NULL || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND ) )
			{
				if( !g_bShowItemMallWindow )
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_BTN_CASHMALL, 0), (LPARAM)gHWnd);
			}			
		}		
		else
		{
			_XDrawLine2D( (FLOAT)(gnWidth-140), (FLOAT)(51 + g_XBaseFont->GetHeight()), (FLOAT)(gnWidth-140+textwidth), (FLOAT)(51 + g_XBaseFont->GetHeight()), 
						  0x8FFFFFFF,0x8FFFFFFF );
		}

		if( g_bShowItemMallWindow )
		{
 			if( g_pCashMall_Window )
 			{
				g_pCashMall_Window->Draw();
			}
		}
	}

#endif

 
	/*
	FLOAT fontscale = 1.0f;

	if( gpInput->CheckKeyPress( DIK_LCONTROL ) )
	{
		fontscale = 1.02f;
	}
	else if( gpInput->CheckKeyPress( DIK_LSHIFT) )
	{
		fontscale = 0.98f;
	}
	
	g_XLargeFont->SetColor( 0xFFFFFFFF );		
	g_XLargeFont->PrintAlign( (gnWidth>>1), 200, fontscale, _XFONT_ALIGNTYPE_CENTER, "Normal : 의(義)와 협(俠)이 살아있는 강호를 꿈꾸는 현공구선(운영팀) 입니다. %d", gnFrameCounter );	
	g_XLargeFont->Flush();

	g_XLargeFont->SetAlphaBlendingEnable(TRUE);
	g_XLargeFont->SetBoldMode( TRUE );
	g_XLargeFont->PrintAlign( (gnWidth>>1), 230, fontscale, _XFONT_ALIGNTYPE_CENTER, "Bold & alpha : 의(義)와 협(俠)이 살아있는 강호를 꿈꾸는 현공구선(운영팀) 입니다. %d", gnFrameCounter );	
	g_XLargeFont->Flush();
	g_XLargeFont->SetBoldMode( FALSE );
	g_XLargeFont->SetAlphaBlendingEnable(FALSE);

	g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );	
	g_XLargeFont->PrintAlign( (gnWidth>>1), 260, fontscale, _XFONT_ALIGNTYPE_CENTER, "Edge 4 : 의(義)와 협(俠)이 살아있는 강호를 꿈꾸는 현공구선(운영팀) 입니다. %d", gnFrameCounter );
	g_XLargeFont->Flush();

	g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XLargeFont->PrintAlign( (gnWidth>>1), 290, fontscale, _XFONT_ALIGNTYPE_CENTER, "Edge 8 : 의(義)와 협(俠)이 살아있는 강호를 꿈꾸는 현공구선(운영팀) 입니다. %d", gnFrameCounter );
	g_XLargeFont->Flush();
	
	g_XLargeFont->SetAlphaBlendingEnable(TRUE);
	g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XLargeFont->PrintAlign( (gnWidth>>1), 320, fontscale, _XFONT_ALIGNTYPE_CENTER, "Edge 8 & alpha : 의(義)와 협(俠)이 살아있는 강호를 꿈꾸는 현공구선(운영팀) 입니다. %d", gnFrameCounter );
	g_XLargeFont->Flush();
	
	g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_GLOW, 0xFF000000 );
	g_XLargeFont->PrintAlign( (gnWidth>>1), 350, fontscale, _XFONT_ALIGNTYPE_CENTER, "Grow & alpha : 의(義)와 협(俠)이 살아있는 강호를 꿈꾸는 현공구선(운영팀) 입니다. %d", gnFrameCounter );
	g_XLargeFont->Flush();

	g_XLargeFont->DisableGrowMode();
	g_XLargeFont->SetAlphaBlendingEnable(FALSE);
	*/	
	

	/* focus 확인

	g_XBaseFont->SetGrowMode( TRUE, 0xFF000000 );
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Print( 200, 200, 1.0f, "%p", g_CurrentFocusedObject );
	g_XBaseFont->Flush();
	g_XBaseFont->SetGrowMode( !TRUE, 0xFF000000 );
	*/
	/*
	g_XBaseFont->SetGrowMode( TRUE, 0xFF000000 );
	_XLODTILE*	pTerrainTile = g_LodTerrain.GetTerrainTile( g_pLocalUser->m_Position, NULL );
	if(pTerrainTile)
	{
		for( int xx = 0; xx < pTerrainTile->UsedSplatLayerCount; xx++ )
		{
			if(  pTerrainTile->SplatData[xx].m_LayerIndex == 2 )
			{
				for( int i = 0; i < _XDEF_SPLATBLENDTEXTURESIZE; i++ )
				{
					for( int j = 0; j < _XDEF_SPLATBLENDTEXTURESIZE; j++ )
					{
						g_XBaseFont->Print( j<<4, i<<4, 1.0f, "%d", pTerrainTile->SplatData[xx].m_BlendLevelArray[ j + i*_XDEF_SPLATBLENDTEXTURESIZE ] );
					}
				}		
				g_XBaseFont->Flush();

				D3DXVECTOR2 pos;
				pos.x = 300; pos.y = 20;
				D3DXVECTOR2 scale;
				scale.x = 16;
				scale.y = 16;
				g_pd3dxSprite->Draw(pTerrainTile->SplatData[xx].m_pBlendTexture, NULL, &scale, NULL, NULL, &pos, D3DCOLOR_ARGB( 255, 255,255, 255 ) );
			}
		}
	}
	g_XBaseFont->SetGrowMode( FALSE, 0xFF000000 );
	*/

	// End the scene
	gpDev->EndScene();			
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr ) gApp->m_bDeviceLost = true;

	((CEmperorOfDragonsApp*)gApp)->SetCursor( g_ReadyMouseCursorMode );

	g_LodTerrain.ProcessEnvScript(g_fElapsedFrameMilisecondTime, g_LodTerrain.m_3PCamera.m_CameraPosition);

	CheckTextureUseHitrate();

	Sleep( 1 );

#ifdef _ACCLAIM_RUBICONADSYSTEM
	
	if( g_ViewRubiconAD )
	{
		if( g_RubiconADFirstTimeView )
		{
			::ShowWindow(g_hRubiconADWindow, SW_SHOW);
			::UpdateWindow( g_hRubiconADWindow );	
			::RedrawWindow( g_hRubiconADWindow, NULL, NULL, RDW_UPDATENOW );

			g_RubiconADFirstTimeView = FALSE;
		}		
	}

#endif

}

void XProc_MainGame::OnPaint(WPARAM wparam, LPARAM lparam)
{	
	Draw();
}

void XProc_MainGame::DrawPickedObjectInfoTooltip( void )
{	
	D3DXVECTOR3 vert;
		
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE);
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpDev->SetRenderState( D3DRS_ALPHAREF, 0 );
	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	 
	 if( !g_LastPickedObject ) return;

	/*if( g_LastPickedObjectType == _XATTACKTARGET_PC )
	{	
		_XUser* pUser = ((_XUser*)g_LastPickedObject);
		
		D3DXVECTOR3 rvec;
		D3DXVECTOR3 mpos = pUser->m_Position;		
		mpos.y = pUser->m_Position.y + 1.5;
		D3DXVec3Project( &rvec, &mpos, &g_MainViewPort,
			             &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
						 &g_LodTerrain.m_3PCamera.GetViewMatrix(), 
						 NULL );
			
		g_InterfaceTooltip.SetToolTipString( rvec.x, rvec.y, pUser->m_CharacterInfo.charactername, D3DCOLOR_ARGB( 255, 255, 255, 255 ), TRUE );			
	}
	else*/	
	if( g_LastPickedObjectType == _XPICKINGTARGET_ITEM )
	{	
		_XGameItem* pItem = ((_XGameItem*)g_LastPickedObject);
	
		if(!g_ViewItemNameTooltip)
		{
			D3DXVECTOR3 rvec;
			D3DXVECTOR3 mpos = pItem->m_Position;
			mpos.y = pItem->m_Position.y + (pItem->m_pMeshContainer->OBB_Center.y + fabs(pItem->m_pMeshContainer->OBB_Extent[1]));
			
			D3DXVec3Project( &rvec, &mpos, &g_MainViewPort,
							 &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
							 &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL );
			
			if(pItem->m_cType == _XGI_FC_MONEY)
			{
				TCHAR str[64];
				memset(str, 0, sizeof(str));

                //Author : 양희왕 //breif : 미국에 돈표시 문제
                _snprintf(str, sizeof(str), _XGETINTERFACETEXT(ID_STRING_NPCSKILL_MONEY), pItem->m_uiCount); // "금 %d"
				//sprintf(str, "%s %d", _XGameItem::GetItemName(pItem->m_cType, pItem->m_sID), pItem->m_uiCount);
				g_ItemTooltip.SetToolTipStringEx( rvec.x, rvec.y, str, _XSC_INFORMATION, 32, TRUE );
			}
			else
			{
				g_ItemTooltip.SetToolTipStringEx( rvec.x, rvec.y, _XGameItem::GetItemName(pItem->m_cType, pItem->m_sID), _XSC_INFORMATION, 32, TRUE );
			}

			g_ItemTooltip.ShowWindow( TRUE );
			g_ItemTooltip.Draw();
		}
	}
	
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	gpDev->SetRenderState(D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
}

//_XMOTION_MOVETYPE g_PrevMoveType = _XMOVETYPE_WALK;

//#define _XDEF_DETECTSPEEDHACK	

bool XProc_MainGame::Process( void )
{

#ifdef _XGMCLIENT
if( g_pLocalUser->m_CharacterInfo.Get_gm_level() == 0 )
{
#endif

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD) && !defined(_XJAPANESE)	// 일본은 gameguard basic license라서 서버 인증을 사용 안함.
	if( !g_pLocalUser->m_bIsBossBattle &&
		( g_QuestScriptManager.m_indexVirtualZone != 22 ) && 
		!m_CompleteGameGuardAuth && !m_CancleGameGuardAuth ) // 첫번째 서버인증이 안되있을 경우 Process를 처리 하지 않는다.
	{
		if( g_GGServerAuthReadyTime == 0 )
		{
			g_GGServerAuthReadyTime = g_LocalSystemTime;
		}
		
		if( (g_LocalSystemTime - g_GGServerAuthReadyTime ) > 60000 ) // 60초 내로 서버에서 아무것도 안오면 인증 실패 게임종료.
		{
			if( !m_GameExitFlag )
			{
				_XLog( "Canceled GGSA." );
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GAMECLOSE, 0), (LPARAM)gHWnd);
			}
		}
		else
		{
			//_XLog("Skipped Process");
			Sleep( 1 );
			return true;
		}		
	}
#endif

#ifdef _XGMCLIENT
}
#endif

#ifdef _XDEF_DETECTSPEEDHACK	

	FLOAT checkrepeattime = g_fLocalSystemOrgTime - g_SpeedHackCheck_PrevCheckTime;
			
	// Detect speed hack
	if( checkrepeattime > _XDEF_DETECTSPEEDHACK_REPEATTIME )
	{
		if( g_SpeedHackCheck_DetectFrameTime >  (_XDEF_DETECTSPEEDHACK_MAXDETECTEDCOUNT<<1) )
		{
			if( g_SpeedHackCheck_DetectedCounter >= _XDEF_DETECTSPEEDHACK_MAXDETECTEDCOUNT )
			{
				g_SpeedHackDetected			 = TRUE;
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORSPEED), _XDEF_CHATMESSAGEMODE_ADMINNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			}
			else
			{				
				if( g_SpeedHackDetected )
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_NORMALSPEED),_XDEF_CHATMESSAGEMODE_ADMINNOTICE + ,_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );

				g_SpeedHackDetected			 = FALSE;
			}

			g_SpeedHackCheck_PrevCheckTime = g_fLocalSystemOrgTime;
			g_SpeedHackCheck_DetectedCounter = 0;
			g_SpeedHackCheck_SystemPrevTime = 0;
			g_SpeedHackCheck_DetectFrameTime = 0;
		}
		else
		{
			g_SpeedHackCheck_DetectFrameTime++;
		}

		CheckUseSpeedHack();
	}	
	
	if( g_SpeedHackDetected	)
	{		
		g_fLocalSystemTime -= g_fElapsedFrameTime;
		g_fLocalSystemTime += 1.0f;
		g_fElapsedFrameTime = 1.0f;
	}	
#endif

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.Update();
#endif

#ifdef _ACCLAIM_IGAADSYSTEM
	if( m_AcclaimADMode_MainGame != _XACCLAIMAD_MAINGAME_NONE )
	{
		g_IGAADWrapper.Process();

		if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_AREACHANGE )
		{
			g_IGAADWrapper.Process();

			if( !g_IGAADWrapper.GetShowStatus() )
			{
				m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
			}
		}
		else if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_DEATH )
		{
			if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_STATICAD )
			{
				if( !g_IGAADWrapper.GetShowStatus() )
				{
					m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_NONE;
					m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
				}
			}
			else if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_VIDEOAD )
			{
				if( !g_IGAADWrapper.GetShowStatus() )
				{
					m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_RESULTVIEW;
					m_DeathADRecievedResult = FALSE;
					m_DeathADResultViewCounter = 0;
					m_DeathADRecievedResultAutoCancelCounter = 0;


					int windowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );

					POINT pos;
					pos.x = (gnWidth>>1) - 200; // width 400
					pos.y = (gnHeight>>1) - 100; // height 200
					
					m_AcclaimAdResultTitleLeftBar.Create( pos.x, pos.y, pos.x+80, pos.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
					m_AcclaimAdResultTitleLeftBar.SetClipRect( 1, 126, 81, 138 );

					m_AcclaimAdResultTitleCenterBar.Create( pos.x+80, pos.y, pos.x+400, pos.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
					m_AcclaimAdResultTitleCenterBar.SetClipRect( 84, 126, 94, 138 );

					FLOAT scalefactor = ((FLOAT)(400-86)) / 10.0f;
					m_AcclaimAdResultTitleCenterBar.SetScale( scalefactor, 1.0f );

					m_AcclaimAdResultTitleRightBar.Create( pos.x+400-6, pos.y, pos.x+400-1, pos.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
					m_AcclaimAdResultTitleRightBar.SetClipRect( 177, 126, 183, 138 );

					int moneyiconimageindex = g_MainInterfaceTextureArchive.FindResource( "item_mon.tga" );
					m_AcclaimAdResultItemIconStatic.Create( (gnWidth>>1) - 16, (gnHeight>>1) - 40, (gnWidth>>1) + 16, (gnHeight>>1) - 40 + 32, &g_MainInterfaceTextureArchive, moneyiconimageindex );
					m_AcclaimAdResultItemIconStatic.SetClipRect( 0, 0, 32, 32 );

					// Send ad packet
					g_NetworkKernel.SendPacket( MSG_NO_US_AD_REWARD_REQ, UART_DEAD );

					_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
					if(pRebirth_Window) pRebirth_Window->m_VideoADCanceled = FALSE;

					//////////////////////////////////////////////////////////////////////////
					// test code......
					//m_DeathADRecievedResult = TRUE; 
					//////////////////////////////////////////////////////////////////////////
				}
			}
			else  if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_RESULTVIEW )
			{
				// 서버에서 결과가 왔으면 3초간 디스플레이 후 광고 모드 종료

				if( m_DeathADRecievedResult ) 
				{
					if( m_DeathADResultViewCounter == 0 )
					{
						m_DeathADResultViewCounter = g_LocalSystemTime;
					}
					else
					{
						if( g_LocalSystemTime - m_DeathADResultViewCounter > 3000 )
						{
							if( m_AcclaimADMode_DeathPhase != _XACCLAIMAD_DEATHPHASE_NONE )
							{
								m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
								m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_NONE;

								_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
								if(pRebirth_Window)
								{
									pRebirth_Window->m_ShowIGAADButton = FALSE;
									pRebirth_Window->ShowWindow( TRUE );
									pRebirth_Window->m_AcclaimAdVideoButton->ShowWindow( FALSE );									
									pRebirth_Window->m_VideoADCanceled = FALSE;
								}
							}
						}
					}
				}
				else
				{
					// 20초가 지나도 서버에서 결과가 안오면 광고모드 강제 종료

					if( m_DeathADRecievedResultAutoCancelCounter == 0 )
					{
						m_DeathADRecievedResultAutoCancelCounter = g_LocalSystemTime;
					}
					else if( g_LocalSystemTime - m_DeathADRecievedResultAutoCancelCounter > 10000 ) 
					{
						if( m_AcclaimADMode_DeathPhase != _XACCLAIMAD_DEATHPHASE_NONE )
						{
							m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
							m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_NONE;							

							_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
							if(pRebirth_Window)
							{
								pRebirth_Window->m_ShowIGAADButton = FALSE;
								pRebirth_Window->ShowWindow( TRUE );
								pRebirth_Window->m_AcclaimAdVideoButton->ShowWindow( FALSE );
								pRebirth_Window->m_VideoADCanceled = FALSE;
							}
						}
					}
				}
			}
			else
			{
				
			}
		}
	}
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM
	
	if( g_ViewRubiconAD )
	{
		if( g_RubiconADRefreshTimer == 0 )
		{
			g_RubiconADRefreshTimer = g_LocalSystemTime;
		}

		if( g_LocalSystemTime - g_RubiconADRefreshTimer > 30000 )
		{
			g_RubiconADRefreshTimer = g_LocalSystemTime;
			DisplayHTMLPage(g_hRubiconADWebBrowserObject, "http://www.acclaim.com/ads/9d_rubicon_728_90.html");
		}
	}

#endif

	// 서버 시간 업데이트

#ifdef _XDEF_SERVERTIMECHECK_071112_KERYGMA

	DWORD ElapsedTime = g_LocalSystemTime - g_LocalServerTimeCode;
	g_LocalServerTimeCode = g_LocalSystemTime;

	g_fServerTimeCode += (DOUBLE)ElapsedTime / 1000.0f;

	if(g_fServerTimeCode >= 1.0f)
	{
		g_ServerTimeCode += (int)g_fServerTimeCode; // 1초 오버되면 서버시간 1초 단위 증가.
		g_fServerTimeCode -= (int)g_fServerTimeCode; // 나머지 ms 단위 누적.
	}
#else

	DWORD ElapsedTime = g_LocalSystemTime - g_LocalServerTimeCode;
	g_LocalServerTimeCode = g_LocalSystemTime;
	g_fServerTimeCode += (DOUBLE)ElapsedTime * 0.001;
	if(g_fServerTimeCode >= 1.0f)
	{
		g_ServerTimeCode += (int)g_fServerTimeCode;
		g_fServerTimeCode -= (int)g_fServerTimeCode;
	}

#endif

	// 자리 비움 체크
#ifdef _XDEF_PARTY_NOTAVAILABLE

#define _XDEF_OUT_ELAPSEDTIME	300000		// 5분

	if(m_dwLastInputTime == 0)
	{
		// 처음 한번 세팅해준다
		m_dwLastInputTime = g_LocalSystemTime;
	}

	if(g_LocalSystemTime - m_dwLastInputTime >= _XDEF_OUT_ELAPSEDTIME)
	{
		// 자리비움 세팅
		if(!g_pLocalUser->m_bOut)
			g_pLocalUser->SetOut(true);
	}
	else
	{
		// 자리비움 풀기
		if(g_pLocalUser->m_bOut)
			g_pLocalUser->SetOut(false);
	}
#endif

//	Author : 양희왕 
#ifdef _XDEF_NOTICE_STRING_PROCESS
	m_pXNoticeString->Update( g_SystemTimeCounter.GetElapsedMiliSecondTime() );
#endif
//Last updated: 07/01/10

	static DWORD iCurrentCheckTime = 0;

	if( iCurrentCheckTime == 0 )
	{
		iCurrentCheckTime = g_LocalSystemTime;
	}


	
//게임가드 유/무 검사 10초 마다
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	if( (g_LocalSystemTime - iCurrentCheckTime) > 30000 )
	{
		iCurrentCheckTime = g_LocalSystemTime;

		if( g_GameGuardInstance.Check() != NPGAMEMON_SUCCESS ) 
		{
			extern LPTSTR g_nProtectModuleCheckMessage;
			_XFatalError(g_nProtectModuleCheckMessage);
			return false;
		}
	}
#endif

#ifdef _XDEF_HACKTOOLCHECK_071112_KERYGMA
	static DWORD iCurrentSecondCheckTime = 0;
	static int	 iDetectedCount = 0;
	static int	 iDetectedCount2 = 0;	

	extern TCHAR g_szLastUsedCheatEngine[64];
	
	if( iCurrentSecondCheckTime == 0 || (g_LocalSystemTime - iCurrentSecondCheckTime > 223000) )
	{
		iCurrentSecondCheckTime = g_LocalSystemTime;

		TCHAR filename[10][32];
		memset( filename, 0, sizeof(TCHAR) * 10 * 32 );

		filename[0][0] = _T('P');
		filename[1][1] = _T('u');
		filename[2][3] = _T('a');
		filename[2][4] = _T('e');
		filename[0][1] = _T('u');
		filename[0][2] = _T('m');
		filename[0][3] = _T('a');
		filename[0][4] = _T('E');
		filename[0][5] = _T('n');
		filename[1][0] = _T('A');
		filename[1][6] = _T('.');
		filename[1][7] = _T('e');
		filename[1][5] = _T('D');
		filename[5][4] = _T('l');
		filename[5][5] = _T('i');
		filename[0][6] = _T('g');
		filename[0][7] = _T('i');
		filename[1][2] = _T('t');
		filename[0][8] = _T('n');
		filename[0][9] = _T('e');
		filename[0][10] = _T('.');
		filename[0][11] = _T('e');
		filename[0][12] = _T('x');
		filename[0][13] = _T('e');	
		filename[1][3] = _T('o');
		filename[1][4] = _T('9');		
		filename[1][8] = _T('x');
		filename[2][5] = _T('g');
		filename[4][0] = _T('s');
		filename[4][1] = _T('h');
		filename[2][6] = _T('i');		
		filename[1][9] = _T('e');
		filename[2][0] = _T('p');		
		filename[2][8] = _T('e');
		filename[3][0] = _T('a');		
		filename[7][5] = _T(' ');
		filename[7][2] = _T('e');
		filename[7][3] = _T('a');
		filename[6][11] = _T('e');		
		filename[7][8] = _T('g');
		filename[7][9] = _T('i');
		filename[7][6] = _T('E');
		filename[7][7] = _T('n');
		filename[4][3] = _T('k');
		filename[6][9] = _T('i');
		filename[6][8] = _T('g');
		filename[6][0] = _T('C');
		filename[6][10] = _T('n');		
		filename[6][3] = _T('a');
		filename[3][1] = _T('u');
		filename[3][2] = _T('t');
		filename[3][3] = _T('o');		
		filename[2][7] = _T('n');
		filename[6][14] = _T('x');
		filename[6][15] = _T('e');
		filename[2][2] = _T('m');		
		filename[3][4] = _T(' ');
		filename[3][5] = _T('9');
		filename[5][7] = _T('h');	
		filename[7][10] = _T('n');
		filename[7][11] = _T('e');		
		filename[5][8] = _T('t');		
		filename[6][4] = _T('t');
		filename[6][5] = _T(' ');
		filename[4][4] = _T('3');
		filename[5][2] = _T('o');
		filename[7][0] = _T('C');
		filename[5][6] = _T('g');		
		filename[5][3] = _T('n');
		filename[2][1] = _T('u');		
		filename[3][6] = _T('d');		
		filename[4][2] = _T('a');		
		filename[5][0] = _T('m');
		filename[5][1] = _T('o');
		filename[6][6] = _T('E');
		filename[6][7] = _T('n');		
		filename[6][1] = _T('h');		
		filename[6][2] = _T('e');				
		filename[6][13] = _T('e');		
		filename[7][1] = _T('h');		
		filename[6][12] = _T('.');
		filename[7][4] = _T('t');				
		filename[8][0] = _T('s');				
		filename[8][1] = _T('o');				
		filename[8][2] = _T('r');				
		filename[8][3] = _T('a');
		
		/* //인포마스터 매크로. 일단 안막음.
		filename[9][0] = _T('9');
		filename[9][1] = _T('D');
		filename[9][2] = _T('r');
		filename[9][3] = _T('a');
		filename[9][4] = _T('g');
		filename[9][5] = _T('o');
		filename[9][6] = _T('n');

		filename[10][0] = _T('N');
		filename[10][1] = _T('D');
		filename[10][2] = _T('G');
		filename[10][3] = _T('r');
		filename[10][4] = _T('o');
		filename[10][5] = _T('u');
		filename[10][6] = _T('p');
		filename[10][7] = _T('.');
		filename[10][8] = _T('e');
		filename[10][9] = _T('x');
		filename[10][10] = _T('e');		
		*/

		// 불사툴
		filename[9][0] = _T('B');
		filename[9][1] = _T('a');
		filename[9][2] = _T('t');
		filename[9][3] = _T('T');
		filename[9][4] = _T('u');
		filename[9][5] = _T('.');
		filename[9][6] = _T('e');
		filename[9][7] = _T('x');
		filename[9][8] = _T('e');

		//for(int i = 0; i < 10; i++)
		//	_XLog( filename[i] );
		
		BOOL found = FALSE;
		_XProcessKill( filename[0], _T(""), found );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[0] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		_XProcessKill( filename[1], _T(""), found );
		if( found )
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[1] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		_XProcessKill( filename[6], _T(""), found );
		if( found )
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[6] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		_XProcessKill( filename[9], _T(""), found );
		if( found )
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[9] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		/*
		_XProcessKill( filename[10], _T(""), found );
		if( found )
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[10] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);

#ifdef _XKOREAN
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T("게임에 유해한 해킹툴이나 매크로로 의심되는 프로그램이 발견되었습니다."), TRUE );
#endif
		}
		*/

		found = _XKillWindow( filename[2] );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[2] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		found = _XKillWindow( filename[3] );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[3] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		found = _XKillWindow( filename[4] );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[4] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		found = _XKillWindow( filename[5] );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[5] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		found = _XKillWindow( filename[7] );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[7] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}

		found = _XKillWindow( filename[8] );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[8] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
		}		

#ifdef _XVIETNAMESE

		found = _XKillWindow( _T("Internet Explorer") );
		if( found ) 
		{
			if( strlen( g_szFoundKillWindowPath ) )
			{
				FILE* pFile = fopen( g_szFoundKillWindowPath, _T("rb") );
				if( pFile )
				{
					LONG curptr = ftell( pFile );
					fseek( pFile , 0L, SEEK_END );
					LONG endptr = ftell( pFile );
					LONG filesize = endptr - curptr;
					fclose( pFile );

					if( filesize < 100000 )
					{
						DWORD nInfoSize = GetFileVersionInfoSize(g_szFoundKillWindowPath, 0);
	
						BYTE* pBlock = new BYTE[nInfoSize+1];
						ZeroMemory(pBlock, nInfoSize+1);
						
						GetFileVersionInfo(g_szFoundKillWindowPath, 0, nInfoSize, pBlock);
						
						UINT cbTranslate;
						
						struct VS_FIXEDFILEINFO 
						{ 
							DWORD dwSignature; 
							DWORD dwStrucVersion; 
							DWORD dwFileVersionMS; 
							DWORD dwFileVersionLS; 
							DWORD dwProductVersionMS; 
							DWORD dwProductVersionLS; 
							DWORD dwFileFlagsMask; 
							DWORD dwFileFlags; 
							DWORD dwFileOS; 
							DWORD dwFileType; 
							DWORD dwFileSubtype; 
							DWORD dwFileDateMS; 
							DWORD dwFileDateLS; 
						} *lpTranslate;
						
						// Read the list of languages and code pages.
						
						VerQueryValue(pBlock, TEXT("\\"), (LPVOID*)&lpTranslate, &cbTranslate);
																		
						delete pBlock;

						iDetectedCount2++; found = FALSE;
						memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
						sprintf( g_szLastUsedCheatEngine, _T("%s,v%d"), filename[9], HIWORD( lpTranslate->dwFileVersionMS ) );
						
						//_XLog( g_szLastUsedCheatEngine );
												
						g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);
					}
				}
			}
			else
			{
				//_XLog("Process name is NULL");
			}
		}

#endif
		/*
		found = _XKillWindow( filename[9] );
		if( found ) 
		{
			iDetectedCount2++; found = FALSE;
			memset( g_szLastUsedCheatEngine, 0, sizeof(TCHAR) * 64 );
			strcpy( g_szLastUsedCheatEngine, filename[9] );
			g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 1);

#ifdef _XKOREAN
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T("게임에 유해한 해킹툴이나 매크로로 의심되는 프로그램이 발견되었습니다."), TRUE );
#endif
		}
		*/

		/*
		if( iDetectedCount2 >= 3 )
		{
			//_XFatalError(_XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2595));
			_XMsgClose = TRUE;
			return FALSE;
		}
		*/

#ifdef _XDEF_MEMORYHACKDEFENCE		
		
#endif

	}
#endif

// 일본은 GameGuard basic license 이므로 실행파일 압축을 사용 안하므로 파일 사이즈 체크를 안함.
#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT) && !defined(_XJAPANESE) && !defined(_XRUSSIAN) && !defined(_XVIETNAMESE) && !defined(_XTAIWANESE) 

	static DWORD iCurrentThirdCheckTime = 0;

	if( iCurrentThirdCheckTime == 0 )
	{
		iCurrentThirdCheckTime = g_LocalSystemTime;
	}

	if( g_LocalSystemTime - iCurrentThirdCheckTime > 120000 )
	{
		iCurrentThirdCheckTime = g_LocalSystemTime;

		LPCTSTR pModuleFilename = _XArgv(0);
		_XSetDefaultPath();
		if( pModuleFilename )
		{
			FILE* pFile = fopen( pModuleFilename, _T("rb") );
			if( pFile )
			{
				LONG curptr = ftell( pFile );
				fseek( pFile , 0L, SEEK_END );
				LONG endptr = ftell( pFile );
				LONG filesize = endptr - curptr;
				fclose( pFile );

				if( filesize > 2500000 )
				{
					//_XLog( "%d,%d", 31, g_LocalSystemTime );
					_XMsgClose = TRUE;
					return FALSE;
				}
			}
			else
			{
				//_XLog( "%d,%d", 32, g_LocalSystemTime );
				_XMsgClose = TRUE;
				return FALSE;
			}
		}
		else
		{
			//_XLog( "%d,%d", 33, g_LocalSystemTime );
			_XMsgClose = TRUE;
			return FALSE;
		}
	}

#endif

#ifdef _XVIETNAMESE
	static DWORD iCurrentFourthCheckTime = 0;

	if( iCurrentFourthCheckTime == 0 )
	{
		iCurrentFourthCheckTime = g_LocalSystemTime;
	}

	if( g_LocalSystemTime - iCurrentFourthCheckTime > 240000 )
	{
		iCurrentFourthCheckTime = g_LocalSystemTime;

		extern BOOL _XCheckLoginServerIP( LPCTSTR szHostName );	

		if( _XCheckLoginServerIP( _T("login.cuulong.com.vn") ) )
		{
			_XLog( "%d", g_LocalSystemTime+1000 );
			_XMsgClose = TRUE;
			return FALSE;
		}
	}
#endif

//Last updated: 07/01/22

	
	// 베트남 셧다운 
#ifdef _XDEF_SHUTDOWN_20061027	
	if(g_PlayTime == 0)
	{
		g_PlayTime = g_LocalSystemTime;
	}
#endif
	
#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
	if(g_pJinStatus_Window)
	{
		if(g_pJinStatus_Window->GetSDSState() == 0)
		{
			// on 상태이면 남은 시간 체크
			int lefttime = 0;
			if( g_PlayLeftTime > 0 )
				lefttime = g_PlayLeftTime - (g_LocalSystemTime - g_PlayTime);

			if(lefttime <= 0)
			{
				// 시간 다됐음
				g_pJinStatus_Window->SetSDSState(2);
			}
		}
	}
#endif
	
#ifdef _XDEF_ZONECHATTING_20070108
	// 지역 대화 10초 딜레이 처리
	if( g_bProgressZoneChatting )
	{
		if( !g_bEnableZoneChatting )
		{
			if( g_DelayTimeZoneChatting != 0 ) 
			{
				DWORD playelapsedtime = g_LocalSystemTime - g_DelayTimeZoneChatting;
				
				if(playelapsedtime >= 10000)		// 10초후 
				{
					g_bEnableZoneChatting = TRUE;
					//다시 지역대화 버튼으로 변경
					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
					pChatDefault_Window->SetChatMode( _XCHATMODE_ZONE );
					//시스템 메세지에 다시 지역대화를 사용할 수 있다고 알려준다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3215),_XDEF_CHATMESSAGECOLOR_SYSTEMGET);
					pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
				}
			}
		}
	}
	else
	{
		if( !g_bEnableZoneChatting )
		{
			if( g_DelayTimeZoneChatting != 0 ) 
			{
				DWORD playelapsedtime = g_LocalSystemTime - g_DelayTimeZoneChatting;
				
				if(playelapsedtime >= 10000)		// 10초후 
				{
					g_bEnableZoneChatting = TRUE;
					//다시 지역대화 버튼으로 변경
					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
					//시스템 메세지에 다시 지역대화를 사용할 수 있다고 알려준다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3215),_XDEF_CHATMESSAGECOLOR_SYSTEMGET);
					pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
				}
			}
		}
	}
#endif

	if( g_pLocalUser->m_ChatMuteTime > 0 ) // 채팅 금지 처리
	{
		g_pLocalUser->m_ChatMuteTime -= g_fElapsedFrameTime;

		int minute = g_pLocalUser->m_ChatMuteTime / 60000; //분단위 계산 및 남은시간 공지 

		if( g_pLocalUser->m_LastChatLockNoticeMinute != minute )
		{
			g_pLocalUser->m_LastChatLockNoticeMinute = minute;

			if( minute > 0 )
			{
				TCHAR _szString[128];
				_snprintf( _szString, sizeof(_szString), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2400), minute );
				g_NetworkKernel.InsertChatString( _szString ,_XDEF_CHATMESSAGECOLOR_GMBLOCKSTATUS);
			}
		}

		if( g_pLocalUser->m_ChatMuteTime < 0 )
		{
			g_pLocalUser->m_ChatMuteTime = 0;
			g_pLocalUser->m_LastChatLockNoticeMinute = 0;
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_2401),_XDEF_CHATMESSAGECOLOR_GMBLOCKSTATUS);
		}
	}


#ifdef _XDEF_SERVERTIMECHECK_071112_KERYGMA
	// Speed Hack Check
	if(g_SendTimerPacketTime == 0 || (g_LocalSystemTime - g_SendTimerPacketTime) >= 180000)
	{
		// 1분마다 한번씩 보내기
		g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK, 0);
	}
#else
	// Speed Hack Check
	if(g_SendTimerPacketTime == 0 || (g_LocalSystemTime - g_SendTimerPacketTime) >= 60000)
	{
		// 1분마다 한번씩 보내기
		g_NetworkKernel.SendPacket(MSG_NO_TIME_CHECK);
	}
#endif

	//Author : 양희왕 //breif : 무기 교체 해야 하는지 판단하는 곳
	if( g_pLocalUser->GetChangingWeapon() && g_pLocalUser->GetChangingWeaponPacket() )
	{
	#ifdef _XDEF_BLOCK_MOVINGATTACK //Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
		if(g_pLocalUser->GetMotionClass() != _XACTION_ATTACK )
		{
			//무기 패킷을 받았지만 무기를 교체 하지않았다.
			//무기 패킷을 받았는데 교체 하지 않았을 때 = 유저가 공격 모션 중일때 
			//공격 모션중에 무기를 바꾸게 되면 updatelevel() 을 해줘야 함으로 에니메이션이 중단됨
			
			// 경고 icon이 떠있을경우 inven number update
			if(g_pState_Window)
			{
				g_pState_Window->UpdatePlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType(),
					_XINVENTORY_SLOTTYPE_WEAPON_1, _XINVENTORY_SLOTTYPE_WEAPON_2);

				g_pState_Window->UpdatePlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_cType(),
					_XINVENTORY_SLOTTYPE_WEAPON_2, _XINVENTORY_SLOTTYPE_WEAPON_1);
			}
			
			_XUserItem temp = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1];
			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1] = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2];
			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2] = temp;

			int effectid = _XGameItem::GetInchantEffectId(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_cType(),
								g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_ucStrength() );

			if( effectid > 0 )
			{
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
			}
			g_pLocalUser->m_CharacterInfo.weaponStrength = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength();

			const int nTo = g_pLocalUser->GetChangeWeaponModel();
			DWORD lparam = (nTo*100000) + g_pLocalUser->m_UserItemList[nTo].Get_m_sID();
			ChangeModel(lparam, g_pLocalUser->m_UserItemList[nTo].Get_m_cType());

			g_pLocalUser->UpdateLocalUserAttackLevel();

			_XPlayInterfaceSound(ID_SR_INTERFACE_PICK_ITEM_001_WAV);

			//Author : 양희왕  breif : 무기 교체가 완료 되었다
			g_pLocalUser->SetChangingWeaponPacket( FALSE, 0 ); 
			g_pLocalUser->SetChangingWeapon( FALSE );

			//Author : 양희왕 //breif : 아이템 교체 시 슬롯 막기 해제
			g_pQuickSlot_Window->ReplacementOfItems(FALSE);

			if(g_pInventory_Window)
			{
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				if( g_pDefaultTooltip_Window )
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
		}
	#endif
	}

	g_LodTerrain.m_3PCamera.Advance();
	
	ProcessBgmTimer();
	ProcessChangeBgm();
	
	//if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_PC )
	if( m_ExperiencePCRoomConnect )	
	{
		ProcessPCRoomConnectNotice();
	}

	// 마우스 커서 모드 초기화 (시작은 이곳부터 끝은 드로우 쪽 )
	if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE ) 
	{
		((CEmperorOfDragonsApp*)gApp)->ResetReadyCursor( _XMOUSETYPE_PEACE );		
	}
	else
	{
		((CEmperorOfDragonsApp*)gApp)->ResetReadyCursor( _XMOUSETYPE_BATTLE );
	}	
	
	// 퀘스트 스크립트 처리
	if( g_QuestScriptManager.m_bQuestStartConditionCheck )
	{
#ifdef _XDEF_SHUTDOWN_20061027
		// 플레이 가능 시간이 지나면 퀘스트를 진행 할 수 없다.
//		if( g_PlayLeftTime > 0 )// g_PlayLeftTime가 0이 아니면 진행
			g_QuestScriptManager.Process();
#else
		g_QuestScriptManager.Process();
#endif
	}

	// 장원전 처리
#ifdef _XDEF_CASTLEBATTLE
	g_CBManager.Process();
#endif

	if(g_pLocalUser->GetMotionClass() == _XACTION_DYING)
	{
		// 빈사 상태
		ProcessDyingState();
		
		// 에러 발생시 프로세스 종료 메세지 및 시그널 처리
		if( m_ProcessStopFlag ) Process_ProcessStop();

		// 게임 종료시 딜레이 처리
		if( m_GameExitFlag ) 
		{	
			Process_GameExit();
		}
	}
	else if(g_pLocalUser->GetMotionClass() == _XACTION_DIE)
	{
		// 혼수 상태
		ProcessComaState();

		// 에러 발생시 프로세스 종료 메세지 및 시그널 처리
		if( m_ProcessStopFlag ) Process_ProcessStop();

		// 게임 종료시 딜레이 처리
		if( m_GameExitFlag ) 
		{	
			Process_GameExit();
		}
	}
	else
	{
		// 일반 상태	
		g_MessageBox.Process();
		g_IMEWindow.Process();

		if(g_QuestScriptManager.GetQuestWideView())
		{
			if(m_pNPCScriptWindowPtr)
			{
				m_pNPCScriptWindowPtr->Process();
			}
		}
		else
		{
			ProcessInterface();
		}
		
		// NPC Script 처리
		if(g_ScriptStarted)
		{
			if( g_LastNPCScriptOwner ) g_LastNPCScriptOwner->Process();
		}

		if( g_bFunctionalObjectScriptStarted && !m_bFunctionalObjectWideView)
		{
			if( g_LastFunctionalObject ) g_LastFunctionalObject->ProcessAction();
		}
				
		if( !g_pLocalUser->m_bVirtualMode )
		{
			g_LodTerrain.m_FunctionalObjectManager.Process();	// Functional Object의 Animation관련 프로세스
			g_Moblist.Process();
			g_Userlist.Process();
		}
		else// 튜토리얼 모드일때  몬스터 AI는 클라이언트에서 조정한다.
		{
			g_LodTerrain.m_FunctionalObjectManager.Process();	// Functional Object의 Animation관련 프로세스
			g_Moblist.ClientMobAIProcess();
			g_Moblist.Process_VirtualMode();
			g_pLocalUser->Process_VirtualMode();
		}

		
		//멀리서 NPC 클릭했을 때 NPC 범위 안에 오면 자동으로 창 띄우기, NPC 말풍선 처리
		ProcessNPCScriptAutoPopUp();
		
		// 에러 발생시 프로세스 종료 메세지 및 시그널 처리
		if( m_ProcessStopFlag ) Process_ProcessStop();
		
		// 게임 종료시 딜레이 처리
		if( m_GameExitFlag ) 
		{	
			Process_GameExit();
		}


		if( g_WarpProcessFadeInOutFlag )
		{
			if( g_LocalSystemTime - m_WarpStartTime > 14000 ) 
			{
				//TCHAR tstr[256];
				//sprintf( tstr, _XGETINTERFACETEXT(ID_STRING_MAINGAME_ZONEMOVEFAIL), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename );
				//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, " ", tstr, TRUE, _XDEF_MG_WARPINGNEXTZONE, _XDEF_MG_CANCELWARPINGNEXTZONE );

				if( !m_GameExitFlag )
				{
					//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", _XGETINTERFACETEXT(ID_STRING_MAIGAME_WARNEXITGAME), TRUE, _XDEF_MG_CANCELWARPINGNEXTZONE, _XDEF_MG_CANCELWARPINGNEXTZONE );
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_MAIGAME_WARNEXITGAME, TRUE, _XDEF_GOTOLOGINSERVER );
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GOTOLOGINSERVER, 0), 0 );
				}

				//m_WarpStartTime = g_LocalSystemTime;
			}
		}

#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	//보스전 안에서는 풍운방 받지 않음
#endif
		{
			// 시작 조건에서 풍운방이 거부 되었을때 30분 후에 다시 보낸다. 
			if( !g_QuestScriptManager.m_listQuestMailBuffer.empty() )
			{
				list <_XQUESTMAIL_INFO>::iterator iter_questmailbuffer;
				for(iter_questmailbuffer = g_QuestScriptManager.m_listQuestMailBuffer.begin() ; iter_questmailbuffer != g_QuestScriptManager.m_listQuestMailBuffer.end() ; )
				{
					_XQUESTMAIL_INFO questmailbuffer = *iter_questmailbuffer;
					if( g_LocalSystemTime - questmailbuffer.resendstarttime > questmailbuffer.delaytime )	// 30분 간격으로 재접속을 시도한다.
					{
						_XWindow_QuestMail* pQuestMail_Window= _XWindow_QuestMail::CreateQuestMail( &g_MainWindowManager, questmailbuffer.questid );
						if( pQuestMail_Window )
						{
							_XQuestScriptItem* currentquest = NULL;
							
							if( questmailbuffer.questid >= 10000 && questmailbuffer.questid < 20000  )
							{
								map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questmailbuffer.questid);
								if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
									currentquest = iter_quest->second;
							}
							else
							{
								map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questmailbuffer.questid);
								if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
									currentquest = iter_quest->second;
							}

							if( currentquest )
							{
								if( currentquest->m_lpszStartQuestMessage )
								{
									switch( questmailbuffer.questmailtype ) {
									case ID_QUEST_QUESTEMAIL_OK:
										{
											pQuestMail_Window->SetMode( _XQUESTMAILMODE_OK );
											pQuestMail_Window->SetMessage( currentquest->m_lpszStartQuestMessage );
											pQuestMail_Window->m_bStartQuest = TRUE;
										}
										break;
									case ID_QUEST_QUESTEMAIL_YESNO:
										{
											pQuestMail_Window->SetMode( _XQUESTMAILMODE_YESNO );
											pQuestMail_Window->SetMessage( currentquest->m_lpszStartQuestMessage );
											pQuestMail_Window->m_bStartQuest = TRUE;
										}
										break;
									case ID_QUEST_QUESTEMAIL_OK_NOSTART:
										{
											pQuestMail_Window->SetMode( _XQUESTMAILMODE_OK );
											pQuestMail_Window->SetMessage( currentquest->m_lpszStartQuestMessage );
											pQuestMail_Window->m_bStartQuest = FALSE;
										}
										break;
									case ID_QUEST_QUESTEMAIL_YESNO_NOSTART:
										{
											pQuestMail_Window->SetMode( _XQUESTMAILMODE_YESNO );
											pQuestMail_Window->SetMessage( currentquest->m_lpszStartQuestMessage );
											pQuestMail_Window->m_bStartQuest = FALSE;
										}
										break;
									}
								}

							}

						}

						iter_questmailbuffer = g_QuestScriptManager.m_listQuestMailBuffer.erase(iter_questmailbuffer);
					}
					else
					{
						iter_questmailbuffer++;
					}										
				}
			}//if( !g_QuestScriptManager.m_listQuestMailBuffer.empty() )
		}

		if( !m_bGotoLoginProcess ) // 게임 종료 시에는 메신져 서버에 재접속을 하지 않는다
		{
			if( !g_NetworkKernel.m_bMessengerServerConnected   )
			{
				if( g_NetworkKernel.m_ConnectTryCount < 4)
				{
					if(m_MessengerConnectTimer == 0)	
					{						
						m_MessengerConnectTimer = g_LocalSystemTime;
					}
					else if( g_LocalSystemTime - m_MessengerConnectTimer > 15000 )	// 15초 간격으로 재접속을 시도한다.
					{
						_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
						pMessenger_Window->m_CurrentUserStatus = 0;
						
						_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
						if(pMainMenu_Window)
						{
							pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
						}
						pMessenger_Window->ReleaseMessengerList();
						
						g_NetworkKernel.InitializeMessengerSocket();
						g_NetworkKernel.m_ConnectTryCount++;
						m_MessengerConnectTimer = g_LocalSystemTime;
					}	
				}
				else
				{
					if(m_MessengerConnectTimer == 0)	
					{						
						m_MessengerConnectTimer = g_LocalSystemTime;
					}
					else if( g_LocalSystemTime - m_MessengerConnectTimer > 180000 )	// 3분 간격으로 재접속을 시도한다.
					{
						_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
						pMessenger_Window->m_CurrentUserStatus = 0;
						
						_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
						if(pMainMenu_Window)
						{
							pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
						}
						pMessenger_Window->ReleaseMessengerList();
						
						g_NetworkKernel.InitializeMessengerSocket();
						g_NetworkKernel.m_ConnectTryCount++;
						m_MessengerConnectTimer = g_LocalSystemTime;
					}	
				}
			}
		}
		
	}


	g_ParticleEffectManager.Update(g_fElapsedFrameMilisecondTime);
	
	// monster spawn test...
#ifdef _XDWDEBUG	
	extern BOOL  g_AllMonsterSpawnMode;
	extern int	 g_CurrentSpawnMonsterType;
	extern DWORD g_LastMobSpawnTime;	

	if( g_AllMonsterSpawnMode )
	{		
		if( g_LocalSystemTime - g_LastMobSpawnTime > 200 )
		{
			g_LastMobSpawnTime = g_LocalSystemTime;

			while(1)
			{
				g_CurrentSpawnMonsterType++;

				if( g_CurrentSpawnMonsterType >= _XDEF_MODELCLASSCOUNT_MOB ) 
				{
					g_AllMonsterSpawnMode = FALSE;
					g_CurrentSpawnMonsterType = 0;
					g_LastMobSpawnTime = 0;
					break;
				}
				else
				{
					if(g_MonsterName[g_CurrentSpawnMonsterType])
					{
						if( strlen( g_MonsterName[ g_CurrentSpawnMonsterType ] ) > 0 )
						{
							g_NetworkKernel.SendPacket( MSG_NO_CTRL_MONSTER_SPAWN, g_CurrentSpawnMonsterType, 1 );
							break;
						}
					}
				}
			}
		}
	}

#endif
	
#ifdef _XUSEFMOD
	g_FMODWrapper.ProcessBGM();
	g_FMODWrapper.FMODUpdate();
#endif

#ifdef _XDWDEBUG	
	_Process_AutoAttack();
#endif


#ifdef _XTS_ITEMMALLBROWSER
//	_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
//	if( pCashMall_Window )
//	{
//		if( pCashMall_Window->GetShowStatus() )
//		{
//			if( !g_MainWindowManager.IsTopWindow( pCashMall_Window ) )
//			{
//				g_MainWindowManager.SetTopWindow( pCashMall_Window );
//			}			
//		}
//	}
#endif	

	return true;
}

void XProc_MainGame::DrawInterface( void )
{		
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
	
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );	
	gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );
	
	if( g_ViewMainInterface )
	{

#ifdef _XDEF_MADSYSTEM
		g_MadWrapper.DrawMADWindow();
#endif

		// 이동중일때만 랜드마크 영역 검사.
		if( g_pLocalUser->GetMotionClass() == _XACTION_MOVE ) CheckLandMarkPoint();

		if( !m_bNPCDialogMode && !m_bFunctionalObjectWideView )
			ProcessInformationSystem();

		g_MainWindowManager.Draw();

		if( g_pLocalUser->m_bIsBossBattle )
		{			
			if( g_pLocalUser->m_BossStartTime != 0 )
			{				
				if( g_pLocalUser->m_bExitBossBattleRoom )//보스전 탈출 타이머 일때 
				{
					if( (g_LocalSystemTime - g_pLocalUser->m_BossStartTime) < g_pLocalUser->m_BossTimeLimit*1000	 )
					{
						int temptime = g_pLocalUser->m_BossTimeLimit - (g_LocalSystemTime - g_pLocalUser->m_BossStartTime)/1000;
						int	minute	 =  temptime / 60;
						int	second	 =  temptime % 60;
						
						if( minute < 10)
						{
							if( second < 10 )
							{
								m_NumberSmallImage[0].Draw(485, 237);
								m_NumberSmallImage[minute].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'								
								m_NumberSmallImage[0].Draw(518, 237);
								m_NumberSmallImage[second].Draw(529, 237);
							}
							else
							{
								m_NumberSmallImage[0].Draw(485, 237);
								m_NumberSmallImage[minute].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'									
								m_NumberSmallImage[second / 10].Draw(518, 237);
								m_NumberSmallImage[second % 10].Draw(529, 237);
							}	
						}
						else
						{
							if( second < 10 )
							{
								m_NumberSmallImage[minute / 10].Draw(485, 237);
								m_NumberSmallImage[minute % 10].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'	
								m_NumberSmallImage[0].Draw(518, 237);
								m_NumberSmallImage[second % 10].Draw(529, 237);
							}
							else
							{
								m_NumberSmallImage[minute / 10].Draw(485, 237);
								m_NumberSmallImage[minute % 10].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'	
								m_NumberSmallImage[second / 10].Draw(518, 237);
								m_NumberSmallImage[second % 10].Draw(529, 237);			
							}	
						}						
					}
					else
					{
						m_NumberSmallImage[0].Draw(485, 237);
						m_NumberSmallImage[0].Draw(496, 237);
						m_NumberSmallImage[11].Draw(507, 237); // ':'	
						m_NumberSmallImage[0].Draw(518, 237);
						m_NumberSmallImage[0].Draw(529, 237);	
						
						g_pLocalUser->m_BossStartTime = 0;
						g_pLocalUser->m_BossTimeLimit = 0;
					}
				}
			}			
		}
		else if( g_QuestScriptManager.m_bStartQuestTimer )	// 퀘스트 타이머 시작시 표시 
		{			
			if( g_QuestScriptManager.m_QuestTimerStartTime != 0 )
			{
				if( g_QuestScriptManager.m_QuestTimerTimeLimit != 0 )
				{
					if( (g_ServerTimeCode - g_QuestScriptManager.m_QuestTimerStartTime) < g_QuestScriptManager.m_QuestTimerTimeLimit )
					{
						int temptime = g_QuestScriptManager.m_QuestTimerTimeLimit - (g_ServerTimeCode - g_QuestScriptManager.m_QuestTimerStartTime);
						int	minute	 =  temptime / 60;
						int	second	 =  temptime % 60;
						
						if( minute < 10)
						{
							if( second < 10 )
							{
								m_NumberSmallImage[0].Draw(485, 237);
								m_NumberSmallImage[minute].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'								
								m_NumberSmallImage[0].Draw(518, 237);
								m_NumberSmallImage[second].Draw(529, 237);
							}
							else
							{
								m_NumberSmallImage[0].Draw(485, 237);
								m_NumberSmallImage[minute].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'									
								m_NumberSmallImage[second / 10].Draw(518, 237);
								m_NumberSmallImage[second % 10].Draw(529, 237);
							}	
						}
						else
						{
							if( second < 10 )
							{
								m_NumberSmallImage[minute / 10].Draw(485, 237);
								m_NumberSmallImage[minute % 10].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'	
								m_NumberSmallImage[0].Draw(518, 237);
								m_NumberSmallImage[second % 10].Draw(529, 237);
							}
							else
							{
								m_NumberSmallImage[minute / 10].Draw(485, 237);
								m_NumberSmallImage[minute % 10].Draw(496, 237);
								m_NumberSmallImage[11].Draw(507, 237); // ':'	
								m_NumberSmallImage[second / 10].Draw(518, 237);
								m_NumberSmallImage[second % 10].Draw(529, 237);			
							}	
						}						
					}
					else
					{
						m_NumberSmallImage[0].Draw(485, 237);
						m_NumberSmallImage[0].Draw(496, 237);
						m_NumberSmallImage[11].Draw(507, 237); // ':'	
						m_NumberSmallImage[0].Draw(518, 237);
						m_NumberSmallImage[0].Draw(529, 237);	
						
						g_QuestScriptManager.m_TimerStartQuestID	= -1;
						g_QuestScriptManager.m_QuestTimerStartTime	= 0;
						g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
						g_QuestScriptManager.m_bStartQuestTimer		= FALSE;
					}
				}
				else//g_QuestScriptManager.m_QuestTimerTimeLimit 이 0이면 증가하는 타이머이다.
				{
					int temptime = g_ServerTimeCode - g_QuestScriptManager.m_QuestTimerStartTime;
					int	minute	 =  temptime / 60;
					int	second	 =  temptime % 60;
					
					if( minute == 0)
					{
						if( second < 10 )
						{
							m_NumberSmallImage[0].Draw(485, 237);
							m_NumberSmallImage[0].Draw(496, 237);
							m_NumberSmallImage[11].Draw(507, 237); // ':'								
							m_NumberSmallImage[0].Draw(518, 237);
							m_NumberSmallImage[second].Draw(529, 237);
						}
						else
						{
							m_NumberSmallImage[0].Draw(485, 237);
							m_NumberSmallImage[0].Draw(496, 237);
							m_NumberSmallImage[11].Draw(507, 237); // ':'									
							m_NumberSmallImage[second / 10].Draw(518, 237);
							m_NumberSmallImage[second % 10].Draw(529, 237);
						}	
					}
					else if( minute < 10)
					{
						if( second < 10 )
						{
							m_NumberSmallImage[0].Draw(485, 237);
							m_NumberSmallImage[minute].Draw(496, 237);
							m_NumberSmallImage[11].Draw(507, 237); // ':'								
							m_NumberSmallImage[0].Draw(518, 237);
							m_NumberSmallImage[second].Draw(529, 237);
						}
						else
						{
							m_NumberSmallImage[0].Draw(485, 237);
							m_NumberSmallImage[minute].Draw(496, 237);
							m_NumberSmallImage[11].Draw(507, 237); // ':'									
							m_NumberSmallImage[second / 10].Draw(518, 237);
							m_NumberSmallImage[second % 10].Draw(529, 237);
						}	
					}
					else
					{
						if( second < 10 )
						{
							m_NumberSmallImage[minute / 10].Draw(485, 237);
							m_NumberSmallImage[minute % 10].Draw(496, 237);
							m_NumberSmallImage[11].Draw(507, 237); // ':'	
							m_NumberSmallImage[0].Draw(518, 237);
							m_NumberSmallImage[second % 10].Draw(529, 237);
						}
						else
						{
							m_NumberSmallImage[minute / 10].Draw(485, 237);
							m_NumberSmallImage[minute % 10].Draw(496, 237);
							m_NumberSmallImage[11].Draw(507, 237); // ':'	
							m_NumberSmallImage[second / 10].Draw(518, 237);
							m_NumberSmallImage[second % 10].Draw(529, 237);			
						}	
					}						
				}
			}			
		}

		// 퀘스트 시스템 메세지 시작시 표시 
		
		if( !g_CenterSystemMessageList.listEmpty() )
		{			
			g_CenterSystemMessageList.resetList();
			
			_XStringItem* curstring;
			int ypos = 0;
			//DWORD colorindex;
			
			int deleteindex = -1;
			do
			{			
				curstring = (_XStringItem*)g_CenterSystemMessageList.currentItem();	

				// 시간이 지나면 Fade Out 처리를 한다.
				if( (g_LocalSystemTime - curstring->Get_Attribute(1)) > 3000)
				{
					int alphavalue =((DWORD)curstring->Get_Attribute(0) & 0xFF000000 ) >> 24;
					DWORD color = ((DWORD)curstring->Get_Attribute(0) & 0xFFFFFF );
					alphavalue -= 20;
					curstring->Set_Attribute( (alphavalue << 24)| color); 
						
					if( alphavalue > 0 )
					{						
						g_XLargeFont->SetAlphaBlendingEnable(TRUE);
						g_XLargeFont->SetColor( curstring->Get_Attribute(0) );
						//g_XLargeFont->SetColor( D3DCOLOR_ARGB( alphavalue , 250, 230, 170 ) );
						g_XLargeFont->PutsAlign( (gnWidth>>1), /*314*/238-24*( g_CenterSystemMessageList.getitemcount() - 1 - ypos ), _XFONT_ALIGNTYPE_CENTER, curstring->Get_String() );
						g_XLargeFont->Flush();
						g_XLargeFont->SetAlphaBlendingEnable(FALSE);

					}
					else
					{
						deleteindex = ypos;
					}
				}	
				else
				{
					g_XLargeFont->SetAlphaBlendingEnable(TRUE);
					g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
					
					int alphavalue =(((DWORD)curstring->Get_Attribute(0) & 0xFF000000) >> 24);
					DWORD color = ((DWORD)curstring->Get_Attribute(0) & 0xFFFFFF );
					DWORD starttime = (DWORD)curstring->Get_Attribute(1);
					alphavalue = 255;
					curstring->Set_Attribute( (alphavalue << 24)| color); 
					curstring->Set_Attribute2( starttime );
					
					g_XLargeFont->SetColor( curstring->Get_Attribute(0) );
					//g_XLargeFont->SetColor( D3DCOLOR_ARGB( 255, 250, 230, 170 ) );
					g_XLargeFont->PutsAlign( (gnWidth>>1), /*314*/238-24*( g_CenterSystemMessageList.getitemcount() - 1 - ypos ), _XFONT_ALIGNTYPE_CENTER, curstring->Get_String() );
					g_XLargeFont->Flush();
					g_XLargeFont->DisableGrowMode();
					g_XLargeFont->SetAlphaBlendingEnable(FALSE);
				}				
				ypos++;  				
				
				g_CenterSystemMessageList.nextItem();				
			}while( !g_CenterSystemMessageList.atHeadOfList() );
					
			if( deleteindex != -1 ) g_CenterSystemMessageList.deleteItem( deleteindex );
		}	
		
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 170, 48 ) );

		static int  twinklecounter = 0;
		static BOOL	twinklemode = FALSE;
		if( twinklecounter == 0 )
		{
			twinklecounter = g_LocalSystemTime;
		}
		if( g_LocalSystemTime - twinklecounter > 500 )
		{
			twinklemode = !twinklemode;
			twinklecounter = g_LocalSystemTime;
		}

		if( twinklemode )
		{
			if( g_pLocalUser->m_CharacterInfo.Get_constitution() == 8 &&
				g_pLocalUser->m_CharacterInfo.Get_zen() == 8 &&
				g_pLocalUser->m_CharacterInfo.Get_intelligence() == 8 &&
				g_pLocalUser->m_CharacterInfo.Get_dexterity() == 8 &&
				g_pLocalUser->m_CharacterInfo.Get_strength() == 8 )
			{
				g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
				g_XBaseFont->PutsAlign( (gnWidth>>1), 200, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_2402) );
				g_XBaseFont->PutsAlign( (gnWidth>>1), 200+20, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_2403) );
				g_XBaseFont->PutsAlign( (gnWidth>>1), 200+40, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_2404) );
				g_XBaseFont->Flush();
				g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_NONE, 0xFF000000 );
			}
		}		
		
		
		// 퀘스트 카운터 메세지 시작시 표시 
		if( g_QuestScriptManager.m_bStartQuestCounterMessage )
		{			
			if( g_QuestScriptManager.m_QuestSystemMessageStartTime != 0 )
			{
				if( (g_LocalSystemTime - g_QuestScriptManager.m_QuestSystemMessageStartTime) < 3000	 )
				{
					#if defined(_XTAIWANESE)
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 0, 255));
					#else
					g_XBaseFont->SetColor(_XSC_INFORMATION);
					#endif
					g_XBaseFont->SetBoldMode( TRUE );
					g_XBaseFont->PutsAlign( (gnWidth>>1), 211, _XFONT_ALIGNTYPE_CENTER, g_QuestScriptManager.m_lpszQuestSystemMessage );
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode( FALSE );
					
				}
				else
				{
					g_QuestScriptManager.m_QuestSystemMessageStartTime = 0;
					g_QuestScriptManager.m_bStartQuestCounterMessage = FALSE;
				}
			}			
		}
		
		if( g_pInventory_Window ) 
		{
			if(g_pInventory_Window->m_bHaveMouseSlot)
				g_pInventory_Window->DrawDragIcon();
		}
		
		if(m_pNPCTradeWindowPtr)
		{
			if(m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
				m_pNPCTradeWindowPtr->DrawDragIcon();
		}
		
		if(m_pWarehouseWindowPtr)
		{
			if(m_pWarehouseWindowPtr->m_bHaveMouseSlot)
				m_pWarehouseWindowPtr->DrawDragIcon();
		}
		
		if(m_pPCTradeWindowPtr)
		{
			if(m_pPCTradeWindowPtr->m_bHaveMouseSlot)
				m_pPCTradeWindowPtr->DrawDragIcon();
		}
		
		if( m_pSocketPlugInWindowPtr )
		{
			if(m_pSocketPlugInWindowPtr->m_bHaveMouseSlot )
				m_pSocketPlugInWindowPtr->DrawDragIcon();
		}
		if(g_pQuickSlot_Window)
		{
			if(g_pQuickSlot_Window->GetDrawSelectedIcon())
				g_pQuickSlot_Window->DrawMouseSlotIcon();
		}
		if(g_pSkill_Window)
		{
			if(g_pSkill_Window->GetHaveMouseSlot())
				g_pSkill_Window->DrawDragIcon();
		}
		if( m_pRepairItemWindowPtr )
		{
			if( m_pRepairItemWindowPtr->m_bHaveMouseSlot )
				m_pRepairItemWindowPtr->DrawDragIcon();
		}
		if( m_pPocketItemWindowPtr )
		{
			if( m_pPocketItemWindowPtr->m_bHaveMouseSlot )
				m_pPocketItemWindowPtr->DrawDragIcon();
		}
		if( g_pLifeSkill_Window )
		{
			if( g_pLifeSkill_Window->GetHaveMouseSlot() )
				g_pLifeSkill_Window->DrawDragIcon();
		}
		if( m_pGambleBoxWindowPtr )
		{
			if( m_pGambleBoxWindowPtr->m_bHaveMouseSlot )
				m_pGambleBoxWindowPtr->DrawDragIcon();
		}
		if( m_pSocketPlugInExWindowPtr )
		{
			if( m_pSocketPlugInExWindowPtr->m_bHaveMouseSlot )
				m_pSocketPlugInExWindowPtr->DrawDragIcon();
		}
	}
	else //if( g_ViewMainInterface )
	{
		if( m_bNPCDialogMode || m_bNPCDialogWideView || m_bFunctionalObjectWideView)
			g_ViewMainInterface = TRUE;		
	}
	
	// 흑백 대전 남은 시간 Rendering
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
	if(g_pLocalUser->m_dwBWLeftTimeFromServer > 0)
		g_pLocalUser->DrawBWLeftTime();
#endif


#ifdef _XDEF_AGEGRADE_070607_MAGVIPER //Author : 양희왕 //breif : 이용등급
	_XWindow_AgeGrade* pAgeGradeWindow = _XWindow_AgeGrade::GetInstance();
	pAgeGradeWindow->Draw();
#endif
}

#define _XDEF_INFOHELPERSYSTEMVIEWTIMER			400000
#define _XDEF_INFOHELPERSTRINGSCROLLTIMER		150
#define _XDEF_INFOHELPERSTRINGINSERTTIMER	    15000
#define _XDEF_INFOHELPERSTRINGHIDETIMER			13000

void XProc_MainGame::ProcessInformationSystem( void )
{
	if( !m_InfoHelperSystemView )
	{
		if( m_InfoHelperStartTimer == 0 )
			m_InfoHelperStartTimer = g_LocalSystemTime;
		
		if( (g_LocalSystemTime - m_InfoHelperStartTimer) > _XDEF_INFOHELPERSYSTEMVIEWTIMER	 )
		{
			m_InfoHelperSystemView = TRUE;
			m_CurrenStringScrollEffectDirection = 2;
			m_InfoHelperStartTimer = g_LocalSystemTime;
		}
		else return;
	}	

	if( !m_pInfoHelperString ) return;

	if( m_pInfoHelperString->Get_Strlength() > 0 )
	{			
		if( m_InfoHelperScrollTimer == 0 )
		{
			m_InfoHelperScrollTimer = g_LocalSystemTime;
		}		

		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
		g_XBaseFont->Puts( gnWidth+1-m_CurrenStringScrollEffectPosition, gnHeight-65, m_pInfoHelperString->Get_String() );
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 210, 210, 64 ) );
		g_XBaseFont->Puts( gnWidth - m_CurrenStringScrollEffectPosition, gnHeight-66, m_pInfoHelperString->Get_String() );
		g_XBaseFont->Flush();

		if( m_CurrenStringScrollEffectDirection == 0 )
		{
			if( m_CurrenStringScrollEffectPosition >= m_CurrentStringWidth ) // 화면에 등장 
			{
				m_CurrenStringScrollEffectPosition = m_CurrentStringWidth;
				m_CurrenStringScrollEffectProgress = m_CurrentStringWidth;
				m_CurrenStringScrollEffectDirection = 2;				
				m_InfoHelperScrollTimer = 0;
			}
			else
			{
				m_CurrenStringScrollEffectProgress += (FLOAT)(g_LocalSystemTime - m_InfoHelperScrollTimer) / 15.0f;
				m_CurrenStringScrollEffectPosition = _XFC_FloatToIntRet( m_CurrenStringScrollEffectProgress );

				if( m_CurrenStringScrollEffectPosition > m_CurrentStringWidth )
				{
					m_CurrenStringScrollEffectPosition = m_CurrentStringWidth;
				}
			}
		}
		else if( m_CurrenStringScrollEffectDirection == 1 ) // 화면밖으로 사라짐
		{
			if( m_CurrenStringScrollEffectPosition >= gnWidth + m_CurrentStringWidth )
			{
				m_CurrenStringScrollEffectPosition = gnWidth + m_CurrentStringWidth;
				m_CurrenStringScrollEffectProgress = gnWidth + m_CurrentStringWidth;
				m_CurrenStringScrollEffectDirection = 2;				
				m_InfoHelperScrollTimer = 0;
			}
			else
			{
				m_CurrenStringScrollEffectProgress += (FLOAT)(g_LocalSystemTime - m_InfoHelperScrollTimer) / 7.0f;
				m_CurrenStringScrollEffectPosition = _XFC_FloatToIntRet( m_CurrenStringScrollEffectProgress );

				if( m_CurrenStringScrollEffectPosition > gnWidth + m_CurrentStringWidth )
				{
					m_CurrenStringScrollEffectPosition = gnWidth + m_CurrentStringWidth;
				}
			}			
		}
		else // if( m_CurrenStringScrollEffectDirection == 2 ) // 화면 고정
		{
			if( (g_LocalSystemTime - m_InfoHelperScrollTimer) > _XDEF_INFOHELPERSTRINGHIDETIMER )
			{
				m_CurrenStringScrollEffectDirection = 1;
				m_InfoHelperScrollTimer = 0;
			}			
		}
	}

	// 새 메세지를 20초마다 삽입
	if( m_InfoHelperInsertTimer == 0 )
	{
		m_InfoHelperInsertTimer = g_LocalSystemTime;
	}

	if( m_CurrenStringScrollEffectDirection == 2 ) // 스크롤이 완전히 끝난후 재삽입
	{
		if( (g_LocalSystemTime - m_InfoHelperInsertTimer) > _XDEF_INFOHELPERSTRINGINSERTTIMER )
		{
			if( m_InformationStringList.getitemcount() )
			{
				_XStringItem* pString = (_XStringItem*)m_InformationStringList.getItem( m_LastInsertedInformationStringIndex );

				if( pString )
				{			
					m_InfoHelperScrollTimer = 0;

					m_pInfoHelperString->Set_String( pString->Get_String() );
					m_CurrentStringWidth = g_XBaseFont->GetWidth( m_pInfoHelperString->Get_String() ) + 205;
					
					m_CurrenStringScrollEffectPosition = 0;
					m_CurrenStringScrollEffectProgress = 0.0f;
					m_CurrenStringScrollEffectDirection = 0;
					m_CurrenStringScrollEffectSpeed = 150.0f;

					m_LastInsertedInformationStringIndex++;

					if( m_InformationStringList.getitemcount() <= m_LastInsertedInformationStringIndex ) 
					{
						m_LastInsertedInformationStringIndex = 0;
						m_InfoHelperSystemView = FALSE;
					}
				}
			}

			m_InfoHelperInsertTimer = g_LocalSystemTime;
		}	
	}
}

void XProc_MainGame::ProcessInterface( void )
{			
	if( g_ViewMainInterface ) 
		g_MainWindowManager.Process();

#ifdef _XDEF_AGEGRADE_070607_MAGVIPER //Author : 양희왕 //breif : 이용등급
	_XWindow_AgeGrade* pAgeGradeWindow = _XWindow_AgeGrade::GetInstance();
	pAgeGradeWindow->Process();
#endif
	
	MouseState* mousestate = gpInput->GetMouseState();

#ifdef _XKERYGMASETTING

	#define _XDEF_ROTATECAMERAEDGEAREA		6
	#define _XDEF_ROTATESPEEDINCREASE		0.05f
	#define _XDEF_ROTATESPEEDDECREASE		0.2f

	if( !mousestate->bButton[0] && !mousestate->bButton[1] )
	{
		ScrnPos *pPos = gpInput->GetMouseVirScreenPos();

		m_RotateAccelerate = FALSE;
		int currotateyaw   = 0;
		int currotatepitch = 0;
				
		if( pPos->x > gnWidth-_XDEF_ROTATECAMERAEDGEAREA )
		{
			currotateyaw = 1;
			m_RotateAccelerate = TRUE;
		}
		if( pPos->x < _XDEF_ROTATECAMERAEDGEAREA )
		{
			currotateyaw = -1;			
			m_RotateAccelerate = TRUE;
		}
				
		if( pPos->z > gnHeight-_XDEF_ROTATECAMERAEDGEAREA )
		{
			currotatepitch = 1;			
			m_RotateAccelerate = TRUE;
		}
		if( pPos->z < _XDEF_ROTATECAMERAEDGEAREA )
		{
			currotatepitch = -1;			
			m_RotateAccelerate = TRUE;
		}
		
		if( currotateyaw != 0 )
		{			
			if( m_RotateYaw  != currotateyaw )
			{
				m_RotateYaw  = currotateyaw;
				m_CameraRotateSpeed = 0.0f;
			}
		}
		
		if( currotateyaw == -1 )
		{
			if( currotatepitch == -1 )
			{
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_UPLEFT );
			}
			else if( currotatepitch == 1 )
			{
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_DOWNLEFT );
			}
			else ((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_LEFT );
		}
		else if( currotateyaw == 1 )
		{
			if( currotatepitch == -1 )
			{
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_UPRIGHT );
			}
			else if( currotatepitch == 1 )
			{
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_DOWNRIGHT );
			}
			else ((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_RIGHT );
		}
		else 
		{
			if( currotatepitch == -1 )
			{
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_UP );
			}
			else if( currotatepitch == 1 )
			{
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SCRSCROLL_DOWN );
			}
			/*
			else
			{
				if( !g_LastPickedObject )
				{
					if( g_pLocalUser->m_BattleMode )
						((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_BATTLE );
					else
						((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PEACE );					
				}
			}*/
		}		

		if( currotatepitch != 0 )
		{
			if( m_RotatePitch != currotatepitch )
			{
				m_RotatePitch = currotatepitch;
				m_CameraRotateSpeed = 0.0f;
			}
		}		

		if( m_RotateYaw != 0 )
			g_LodTerrain.m_3PCamera.AddYaw( m_CameraRotateSpeed*m_RotateYaw );

		if( m_RotatePitch != 0 )
			g_LodTerrain.m_3PCamera.AddPitch( m_CameraRotateSpeed * m_RotatePitch );
		
		if( m_RotateAccelerate )
		{
			if( m_CameraRotateSpeed < 2.0f )
			{
				m_CameraRotateSpeed += _XDEF_ROTATESPEEDINCREASE;
			}
			else m_CameraRotateSpeed = 2.0f;
		}
		else
		{
			if( m_CameraRotateSpeed > 0.0f )
			{
				m_CameraRotateSpeed -= _XDEF_ROTATESPEEDDECREASE;
			}
			else 
			{
				m_CameraRotateSpeed = 0.0f;
				m_RotateYaw		=	0;
				m_RotatePitch	=	0;
			}
		}	
		
		if( m_CameraRotateSpeed > 0.0f && 
			g_pLocalUser->GetMotionClass() != _XACTION_MOVE ) // _X3PCamera::Advance와 중복계산 피함.
		{
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();	
			// Rebuild loose quadtree...
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel();			
			
			if( g_pMinimap_Window )
			{
				// Set direction to minimap frustum
				g_pMinimap_Window->SetRotateFrustum( _X_RAD( 180.0f - g_LodTerrain.m_3PCamera.GetYaw() ) );
			}
		}
	}
	else
	{
		m_CameraRotateSpeed = 0.0f;
		m_RotateYaw			= 0;
		m_RotatePitch		= 0;
		m_RotateAccelerate  = FALSE;
	}

	if( !gpInput->GetMouseState()->bButton[0] )
	{
		g_CurrentFocusedObject = NULL;
	}
#endif	//_XKERYGMASETTING

}

int XProc_MainGame::StartNPCScript( void )
{
	if( g_pLocalUser->m_bMatchPRMode ) // 비무 홍보가 아닐때만  
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_1820), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	
		return 1;
	}
	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
		return 1;
	if(g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
		return 1;
	
	if( g_pLocalUser->m_PvpNPCLock )	// PVP 시 Lock 
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_1821), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	
		return 1;
	}
	
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE );
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() || g_pLocalUser->m_bInternalUse || g_pLocalUser->m_bSocketPlugIn || g_pLocalUser->m_bGambleBoxOpen )
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_1822), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	
			return 1;
		}
		else if( g_pLocalUser->m_bReadingBook )
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_1822), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	
			return 1;
		}
	}
	
	_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
	if( pTrainingHelp_Window )
	{
		if( pTrainingHelp_Window->GetShowStatus() )
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_1822), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	
			return 1;
		}
	}

	//퀘스트가 재정렬되는 동안은 NPC대화화면을 막는다.
//	if( g_QuestScriptManager.m_bCancelProcess )
//	{
//		g_NetworkKernel.InsertChatString( _T("강호풍운록을 정리하고 있습니다."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	
//		return 1;
//	}

	if( g_LastPickedObjectType != _XPICKINGTARGET_NPC || !g_LastPickedObject ) return 1;
	_XNPCObject* npcobject = (_XNPCObject*)g_LastPickedObject;
	
	if( npcobject->m_NPCRollType == _XNPCROLL_NONE ) return 1; // 2004.06.15->oneway48 insert
	
	FLOAT dx = (npcobject->m_matWorldPosition._41 - g_pLocalUser->m_Position.x);
	FLOAT dz = (npcobject->m_matWorldPosition._43 - g_pLocalUser->m_Position.z);
	FLOAT distance = _XFC_sqrt( dx*dx + dz*dz );
	
	if( distance > _XDEF_DISTANCEOFSTARTNPCSCRIPT )
	{
		//멀리서 NPC 클릭했을 때 NPC 범위 안에 오면 자동으로 창 띄우기
		m_bAutoNPCScriptPopUp = TRUE;
		m_TempNPCObject = npcobject;
		return 0;
	}
	
	if( npcobject->m_Triggerlist.getitemcount() <= 0 ) return 1;
	
	// 2004.06.01->oneway48 insert
	if( npcobject->m_NPCRollType == _XNPCROLL_WAREHOUSE)
	{
#ifdef _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI
		if(((g_LocalSystemTime - m_pWarehouseWindowPtr->m_dwStorageInfoReqTime) >= 50000) && 
			(m_pWarehouseWindowPtr->m_dwStorageInfoReqTime != 0))
		{
			// 창고 정보 요청한지 5분 지났음 - 다시 요청
			m_pWarehouseWindowPtr->m_bWarehouseFirstLoad = FALSE;
		}
#endif

		if(!m_pWarehouseWindowPtr->m_bWarehouseFirstLoad)
		{
			g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_REQ);
			m_pWarehouseWindowPtr->m_bWarehouseFirstView = FALSE;
			m_pWarehouseWindowPtr->m_dwStorageInfoReqTime = g_LocalSystemTime;
		}				
	}

	// 장원전 중 협박 처리
	if(npcobject->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
	{
		if(g_CBManager.GetThreatenState() == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
		{
			if(!g_pLocalUser->m_bSendThreatenPacket)
			{
				if(((g_LocalSystemTime - g_CBManager.m_dwThreatenRequestTime) >= g_CBManager.GetThreatenDelayTime()*1000))
				{
					_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
					if(pCBPersonalHistory_Window)
					{
						if(pCBPersonalHistory_Window->m_nPoint >= 10)
						{
							g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_threat_start_req);
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
						else
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3361), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"장원혈이 부족합니다."
						}
					}
				}
				else
				{
					_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
					if(pThreatenGauge_Window)
					{
						if(!pThreatenGauge_Window->GetShowStatus())
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3362), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"지금은 협박을 진행할 수 없습니다."
						}
					}
				}
			}
		}

		m_bNPCDialogWideView = FALSE;
		g_NPCDialogScreenEffectFlag = FALSE;
	}
	else
	{
		m_bNPCDialogWideView = TRUE;
		g_NPCDialogScreenEffectFlag = TRUE;

		_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
		defaultsystemmsgwindow->ShowWindow(FALSE);
	}

	// insert end
	g_LastNPCScriptOwner = npcobject;
	g_LastNPCScriptOwner->m_pNPCScriptWindowPtr = m_pNPCScriptWindowPtr;
	m_bNPCTalkLog = TRUE;

	if(npcobject->m_NPCJobClass != _XNPC_JOBCLASS_CASTLECAPTAIN)
		g_LastNPCScriptOwner->SetConversationAction();
	
	_XDWINPRINT("[MAINGAME/StartNPCScript] NPC[%d] talk", npcobject->m_UniqueID);
	
	return 1;
}

int XProc_MainGame::StartFunctionalObjectScript( void )
{
	if( g_LastPickedObjectType != _XPICKINGTARGET_FUNCTIONALOBJECT || !g_LastPickedObject ) return 1;
	if( m_bFunctionalObjectWideView ) return 1;

	if( g_pLocalUser->m_PvpNPCLock )	// 거래중
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_1821), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	
		return 1;
	}
	
	_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LastPickedObject;
	//if( pFunctionalObject->m_bAttackable ) return 1; //공격가능한 상태일때도 계속 아이템 검사를 해야 한다.
		
	FLOAT dx = (pFunctionalObject->m_Position.x - g_pLocalUser->m_Position.x);
	FLOAT dz = (pFunctionalObject->m_Position.z - g_pLocalUser->m_Position.z);
	FLOAT distance = _XFC_sqrt( dx*dx + dz*dz );
	
	if( distance > (g_LodTerrain.m_MeshObjectManager.m_ModelList[ pFunctionalObject->m_ObjectIndex ]->OBB_Extent[2]*2 + 2.7f) )
	{
		m_bAutoFunctionalObjectScriptPopUp = TRUE;
		m_TempFunctionalObject = pFunctionalObject;
		return 0;
	}
	if( pFunctionalObject->m_listFunctionalObjectTrigger.getitemcount() <= 0 ) return 1;
	
	
	g_LastFunctionalObject			= pFunctionalObject;
	m_bFunctionalObjectWideView		= g_LastFunctionalObject->m_bWideViewMode;
	
	if( !m_bFunctionalObjectWideView )	
	{
		g_bFunctionalObjectScriptStarted = TRUE;
	}
	else
	{
		g_NPCDialogScreenEffectFlag = TRUE;
		_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
		defaultsystemmsgwindow->ShowWindow(FALSE);
	}
		
	
	_XDWINPRINT("[MAINGAME/StartOBJECTScript] OBJECT[%d] talk", pFunctionalObject->m_UniqueID);
	
	return 1;
}

void XProc_MainGame::SetMovetoMousePickedPoint( void )
{		
	if( GetMousePosition() )
	{	
		// 캐릭터에서 일정 거리만큼 떨어진곳이 클릭됬는지 체크

		if( g_vMousePosition.x > 0.0f )
		{
			if( g_vMousePosition.x > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth) )
			{
				g_vMousePosition.x = (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth);
			}
		}
		else
		{
			if( g_vMousePosition.x < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth) )
			{
				g_vMousePosition.x = (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth);
			}
		}
		
		if( g_vMousePosition.z > 0.0f )
		{
			if( g_vMousePosition.z > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight) )
			{
				g_vMousePosition.z = (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight);
			}
		}
		else
		{
			if( g_vMousePosition.z < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight) )
			{
				g_vMousePosition.z = (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight);
			}
		}

		FLOAT minimumdistance;
		
		if( g_pLocalUser->m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN ) 
			 minimumdistance = _XDEF_MINIMUMWALKDISTANCE;
		else minimumdistance = _XDEF_MINIMUMRUNDISTANCE;
		
		if( _XFastDistance( (g_vMousePosition.x-g_pLocalUser->m_Position.x), 
			            (g_vMousePosition.z-g_pLocalUser->m_Position.z) ) < minimumdistance )
		{
			g_pLocalUser->m_RotateAngle = _XMeshMath_GetRotateRadian( g_pLocalUser->m_Position, g_vMousePosition );			
			g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
			g_pLocalUser->m_PrevMoveTime = 0;
			g_pLocalUser->UpdatePosition();

			if( !gpInput->GetMouseState()->bButton[0] &&
				g_pLocalUser->GetMotionClass() != _XACTION_IDLE )
			{							
				g_pLocalUser->SetAnimation( _XACTION_IDLE );
			}

			if( g_pMinimap_Window )
			{
				// Set direction to minimap arrow 
				g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI  );
				// Set minimap status
				g_pMinimap_Window->SetLocalUserPosition();
			}
			
			return;
		}
		
		if( !g_LastPickedObject )
		{
			g_pLocalUser->ResetAttackTarget();

			g_pLocalUser->m_TargetPosition = g_vMousePosition;
		}		
	}
}

// Terrain handling -----------------------------------------------------------

BOOL XProc_MainGame::LoadTerrain( LPSTR Filename )
{
	assert( Filename );

	_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);

	if( !g_LodTerrain.LoadTerrain( Filename ) )
	{		
		return FALSE;
	}

	return TRUE;
}

// Renderer handling-----------------------------------------------------------

void XProc_MainGame::InitializeRenderState( void )
{
	gpDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	gpDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);		

	// Wall texture just point mip mapping with linear filtering
	gpDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	gpDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	gpDev->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    gpDev->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	
	gpDev->SetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    gpDev->SetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if( g_bCloudShadow )
	{
		gpDev->SetTextureStageState(g_CloudShadowTextureStage, D3DTSS_TEXCOORDINDEX, g_CloudShadowTextureStage);
		gpDev->SetSamplerState(g_CloudShadowTextureStage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		gpDev->SetSamplerState(g_CloudShadowTextureStage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
		gpDev->SetSamplerState(g_CloudShadowTextureStage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		gpDev->SetTextureStageState(g_CloudShadowTextureStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		gpDev->SetTextureStageState(g_CloudShadowTextureStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
	}
			
	// 16비트 모드일 경우 디더링을 켜준다.
	//if( gfmtDepth == D3DFMT_D16 ) gpDev->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	//else gpDev->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
	
    gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    
	gpDev->SetRenderState( D3DRS_ZENABLE,        g_RS_UseZBuffer );
	gpDev->SetRenderState( D3DRS_ZFUNC,			 g_RS_ZBufferFunc );
	gpDev->SetRenderState( D3DRS_DEPTHBIAS ,			 0 );
    
	gpDev->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL);

#ifdef _XDEF_USETERRAINDIFFUSECOLOR
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_COLOR1 );
#else
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
#endif
	
	gpDev->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetRenderState( D3DRS_LOCALVIEWER, !FALSE );

	gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

	// Alpha blend state
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );		
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// Alpha test state
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	
	//gpDev->SetRenderState( D3DRS_SHADEMODE , D3DSHADE_FLAT );
	
}


// Light handling--------------------------------------------------------------

void XProc_MainGame::InitializeLight( void )
{	
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
		
	gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	gpDev->LightEnable( 0, TRUE );
}

// ETC Methods-----------------------------------------------------------------
#ifdef _XDWDEBUG
void XProc_MainGame::DrawDebugState( void )
{
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// never erase...
	gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );
	gpDev->SetRenderState( D3DRS_ZENABLE,   D3DZB_FALSE );

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	g_XBaseFont->SetColor( _XSC_INFORMATION );
//	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 2+_XDEF_DBGVIEWYOFFSET, 1.0f, "FPS: %d / %d || MX = %d, MY = %d, ENV : %d", 
//											gnFPS, _XGetFPS(), scrnpos->x, scrnpos->z, g_LodTerrain.m_EnvironmentTime );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 15+_XDEF_DBGVIEWYOFFSET, 1.0f, 
					 "X: %4.4f Y: %4.4f Z: %4.4f Yaw: %4.4f Pitch: %4.4f Roll: %4.4f",
					 g_LodTerrain.m_3PCamera.m_CameraPosition.x, g_LodTerrain.m_3PCamera.m_CameraPosition.y, 
					 g_LodTerrain.m_3PCamera.m_CameraPosition.z, g_LodTerrain.m_3PCamera.GetYaw(), 
					 g_LodTerrain.m_3PCamera.GetPitch(), g_LodTerrain.m_3PCamera.GetRoll());
	
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 28+_XDEF_DBGVIEWYOFFSET, 1.0f, "User pos: %f %f %f -> %f %f %f", 
				   g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y, g_pLocalUser->m_Position.z,
				   g_pLocalUser->m_TargetPosition.x, g_pLocalUser->m_TargetPosition.y, g_pLocalUser->m_TargetPosition.z );
	
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 41+_XDEF_DBGVIEWYOFFSET, 1.0f, "Dist : %f,%f", g_LodTerrain.m_3PCamera.GetDistance(), g_LodTerrain.m_3PCamera.m_CurrentTargetDistance );	
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 54+_XDEF_DBGVIEWYOFFSET, 1.0f, "Fog density : %f", g_EnvironmentManager.m_FogDensity );	

	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 67+_XDEF_DBGVIEWYOFFSET, 1.0f, "Mouse : %f %f %f", g_vMousePosition.x, g_vMousePosition.y, g_vMousePosition.z );

	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 80+_XDEF_DBGVIEWYOFFSET, 1.0f, "Culling frustum : x : %f z : %f %f %f %f %f", 
										g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.x,
										g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.z,
										g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nx[0],
										g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nz[0],
										g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nx[1],
										g_LodTerrain.m_ObjectQuadTree.m_CurrentFrustum.nz[1]);

	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 93+_XDEF_DBGVIEWYOFFSET, 1.0f, "Rendered object [ N : %d / A : %d ]", 
		                   g_LodTerrain.m_ObjectQuadTree.m_RenderedObject, g_LodTerrain.m_ObjectQuadTree.m_AlphaBlendObject );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 106+_XDEF_DBGVIEWYOFFSET, 1.0f, "Culling far plane : %f", g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 118+_XDEF_DBGVIEWYOFFSET, 1.0f, "VR : %f %f %f VO : %f %f %f", 
							g_vPickRayDir.x, g_vPickRayDir.y, g_vPickRayDir.z,
							g_vPickRayOrig.x,g_vPickRayOrig.y,g_vPickRayOrig.z );
	
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 130+_XDEF_DBGVIEWYOFFSET, 1.0f, "UI : constitution:%f Zen:%f intelligence:%f dexterity:%f strength:%f Level:%d",
							g_pLocalUser->m_CharacterInfo.Get_constitution(),
							g_pLocalUser->m_CharacterInfo.Get_zen(),
							g_pLocalUser->m_CharacterInfo.Get_intelligence(),
							g_pLocalUser->m_CharacterInfo.Get_dexterity(),
							g_pLocalUser->m_CharacterInfo.Get_strength(), 
							g_pLocalUser->m_CharacterInfo.Get_level() );
	
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 154+_XDEF_DBGVIEWYOFFSET, 1.0f, "Camera speed %f [%d]", m_CameraRotateSpeed, m_RotateAccelerate );	
	
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 166+_XDEF_DBGVIEWYOFFSET, 1.0f, "DIMPOS : %d %d %d : %d %d %d",
							gpInput->GetMouseVirScreenPos()->x, gpInput->GetMouseVirScreenPos()->y, gpInput->GetMouseVirScreenPos()->z,	
							gpInput->GetMouseScreenPos()->x, gpInput->GetMouseScreenPos()->y, gpInput->GetMouseScreenPos()->z ); 
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 178+_XDEF_DBGVIEWYOFFSET, 1.0f, "Available Texture mem : %f MByte",
																									gpDev->GetAvailableTextureMem() / 1048576.0f );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 190+_XDEF_DBGVIEWYOFFSET, 1.0f, "Local user motion index : %d/%d", 
																									g_pLocalUser->GetMotionClass(),
																									g_pLocalUser->m_ModelDescriptor.GetCurMotion() );		

	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 202+_XDEF_DBGVIEWYOFFSET, 1.0f, "Jin Level / Need : %d   Cur : %d",
					   g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience,
					   g_pLocalUser->m_CharacterInfo.Get_jinlevel() );

	g_XBaseFont->Puts( _XDEF_DBGVIEWXOFFSET, 214+_XDEF_DBGVIEWYOFFSET, gApp->m_strDeviceStats );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 226+_XDEF_DBGVIEWYOFFSET, 1.0f, "System elapsed time : %f", g_SystemTimeCounter.GetElapsedTime() );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 238+_XDEF_DBGVIEWYOFFSET, 1.0f, "Detected speed hack counter : %d", g_SpeedHackCheck_DetectedCounter );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 250+_XDEF_DBGVIEWYOFFSET, 1.0f, "user effect instance count : %d", g_pLocalUser->m_ModelDescriptor.m_EffectManager->getitemcount() );

	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 274+_XDEF_DBGVIEWYOFFSET, 1.0f, "FPS : %d, ElapsedT : %f", gnFPS, g_fElapsedFrameTime );

	if( g_pLocalUser->m_PathNodeCount == -1 )
	{
		g_XBaseFont->Puts( _XDEF_DBGVIEWXOFFSET, 262+_XDEF_DBGVIEWYOFFSET, "A path could not be found", 1.0f );
	}
	else if( g_pLocalUser->m_PathNodeCount == -2 )
	{
		g_XBaseFont->Puts( _XDEF_DBGVIEWXOFFSET, 262+_XDEF_DBGVIEWYOFFSET, "Points are too far apart", 1.0f );
	}
	else if( g_pLocalUser->m_PathNodeCount == -5 )
	{
		g_XBaseFont->Puts( _XDEF_DBGVIEWXOFFSET, 262+_XDEF_DBGVIEWYOFFSET, "Too many midpoints for array size", 1.0f );
	}
	else
	{
		g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 262+_XDEF_DBGVIEWYOFFSET, 1.0f, "Path node count : %d", g_pLocalUser->m_PathNodeCount );
	}

	extern FLOAT g_TerrainRenderingTime;
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 286+_XDEF_DBGVIEWYOFFSET, 1.0f, "Terrain render time : %fsec face [%d]", g_TerrainRenderingTime/1000000, g_LodTerrain.m_FacesDrawn );

	extern FLOAT g_LodLevelCheckTime;
	extern int	 g_RebuildLevelCalledCount;
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 298+_XDEF_DBGVIEWYOFFSET, 1.0f, "Rebuild level time : %fsec called count [%d]", g_LodLevelCheckTime/1000000, g_RebuildLevelCalledCount );
	g_RebuildLevelCalledCount = 0;

	//g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 262+_XDEF_DBGVIEWYOFFSET, 1.0f, "Loaded sound object : %d", AudioMgr()->GetNumBuffers()  );
	g_XBaseFont->Print( _XDEF_DBGVIEWXOFFSET, 310+_XDEF_DBGVIEWYOFFSET, 1.0f, "SSK:%d, CFOV:%f, HFF:%d", 
																			g_pLocalUser->m_UseSpeedSkill, 
																			g_LodTerrain.m_3PCamera.mp_fFov, 
																			g_pLocalUser->m_HiperRunFailedFlag );

	g_XBaseFont->Flush();
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
}
#endif

// Utility methods ------------------------------------------------------------

void XProc_MainGame::GetVectorFromMousePos( POINT& pt, D3DXVECTOR3& raydir, D3DXVECTOR3& rayorg )
{		    
    // Compute the vector of the pick ray in screen space
    D3DXVECTOR3 v;
    v.x =  ( ( ( 2.0f * pt.x ) / gnWidth  ) - 1.0f ) / g_LodTerrain.m_3PCamera.mp_proj_matrix._11;
    v.y = -( ( ( 2.0f * pt.y ) / gnHeight ) - 1.0f ) / g_LodTerrain.m_3PCamera.mp_proj_matrix._22;
    v.z =  1.0f;

    // Get the inverse view matrix
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, &g_LodTerrain.m_3PCamera.mp_view_matrix);

    // Transform the screen space pick ray into 3D space
    raydir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    raydir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    raydir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    rayorg.x  = m._41;
    rayorg.y  = m._42;
    rayorg.z  = m._43;
}

BOOL XProc_MainGame::GetMousePosition( void )
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

#ifdef _XADMINISTRATORMODE
	if( scrnpos->x < 5 || scrnpos->z < 5 || scrnpos->x >= gnWidth-5 || scrnpos->z >= gnHeight-5 ) return FALSE;
#endif

	if( (g_pLocalUser->GetMotionClass() == _XACTION_MOVE) &&
		(m_LastPickingPoint.x == scrnpos->x && m_LastPickingPoint.y == scrnpos->z) ) return TRUE;

	if( m_LastPickingPoint.x != scrnpos->x || m_LastPickingPoint.y != scrnpos->z ) // 기존 좌표와 동일한 경우 스킵
	{	
		m_LastPickingPoint.x = scrnpos->x;
		m_LastPickingPoint.y = scrnpos->z;
	}
		
	return TRUE;
}


// Windows message handler-----------------------------------------------------

void XProc_MainGame::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( message == WM_COMMAND )
	{
		// Tutorial Dialog window message handling ------------------------------------------=
		if( ((wparam & 0xFFFF0000) >> 16) >= 40000 && ((wparam & 0xFFFF0000) >> 16) < 40101 )
		{
			TutorialDialogWindowMessageHandling(wparam, lparam);
			return;
		}// Messenger Dialog window messge handling-----------------------------------------
		else if( ((wparam & 0xFFFF0000) >> 16) >= 60000 && ((wparam & 0xFFFF0000) >> 16) < 60051 ) //메신져 인스턴스 대화창에 대한 처리 
		{
			MessengerDialogWindowMessageHandling(wparam, lparam);
			return;
		}// Quest Mail Dialog window messge handling-----------------------------------------
		else if( ((wparam & 0xFFFF0000) >> 16) >= 60051 && ((wparam & 0xFFFF0000) >> 16) < 60057 ) //풍운방 인스턴스 대한 처리 
		{
			QuestMailWindowMessageHandling(wparam, lparam);
			return;
		}
		
		switch( wparam )
		{
		// Quick slot window---------------------------------------------------
		case _XDEF_QS_PREVBUTTON					:
		case _XDEF_QS_NEXTBUTTON					:
		case _XDEF_QS_LEVELDOWNBUTTON				:
		case _XDEF_QS_LEVELUPBUTTON					:
		case _XDEF_QS_SHORTCUT1BUTTON				:
		case _XDEF_QS_SHORTCUT2BUTTON				:
		case _XDEF_QS_SHORTCUT3BUTTON				:
		case _XDEF_QS_SHORTCUT4BUTTON				:
		case _XDEF_QS_SHORTCUT5BUTTON				:
		case _XDEF_QS_SHORTCUT6BUTTON				:
		case _XDEF_QS_SHORTCUT7BUTTON				:
		case _XDEF_QS_SHORTCUT8BUTTON				:
		case _XDEF_QS_SHORTCUT9BUTTON				:
		case _XDEF_QS_SHORTCUT0BUTTON				:

		case _XDEF_QS_BATTLEMODEBUTTON				:
			QuickSlotMessageHandling(wparam, lparam);
			break;
			
		// Quest window-------------------------------------------------------------		
		case _XDEF_QW_CLOSEBUTTON					:
		case _XDEF_QW_QUESTCOMPLETETABBUTTON		:
		case _XDEF_QW_QUESTPROGRESSTABBUTTON		:
		case _XDEF_QW_QUESTLISTBOX					:
		case _XDEF_QW_QUESTDESCLISTBOX				:
		case _XDEF_QW_QUESTCANCEL_BUTTON			:
		case _XDEF_QW_QUESTTABBUTTON01				:
		case _XDEF_QW_QUESTTABBUTTON02				:
		case _XDEF_QW_QUESTTABBUTTON03				:
		case _XDEF_QW_QUESTTABBUTTON04				:
		case _XDEF_QW_QUESTTABBUTTON05				:
		case _XDEF_QW_QUESTCANCEL_OK				:
		case _XDEF_QW_QUESTCANCEL_CANCLE			:
		case _XDEF_QW_QUESTTAB_PREV					:
		case _XDEF_QW_QUESTTAB_NEXT					:
			QuestWindowMessageHandling(wparam, lparam);
			break;			

		// Inventory window message handling-------------------------------------------
		case _XDEF_INV_CLOSEBUTTON					:

		case _XDEF_INV_BASETAB_POCKET				:
		case _XDEF_INV_BASETAB_BACKPACK				:
		case _XDEF_INV_BASETAB_BACKPACK2			:
		case _XDEF_INV_CHARINFO						:
		
		case _XDEF_INV_INVENTORYMONEY				:

		case _XDEF_INV_LISTBUTTON					:
		case _XDEF_INV_HATBUTTON					:
		case _XDEF_INV_EARRINGBUTTON				:
		case _XDEF_INV_NECKLACEBUTTON				:
		case _XDEF_INV_JACKETBUTTON					:
		case _XDEF_INV_MANTLEBUTTON					:
		case _XDEF_INV_RINGBUTTON					:
		case _XDEF_INV_WEAPONBUTTON					:
		case _XDEF_INV_FISTBUTTON					:
		case _XDEF_INV_PANTSBUTTON					:
		case _XDEF_INV_SUPPORTBUTTON				:
		case _XDEF_INV_AMMUNITIONBUTTON				:
		case _XDEF_INV_FOOTWEARBUTTON				:
		case _XDEF_INV_WEARINGBUTTON				:
		case _XDEF_INV_THROWBUTTON					:
		case _XDEF_INV_REMOVEBUTTON					:
		case _XDEF_INV_HELPBUTTON					:
		case _XDEF_INV_USEELIXIRYES					:
		case _XDEF_INV_USEELIXIRNO					:
		case _XDEF_INV_USEBOOKYES					:
		case _XDEF_INV_USEBOOKNO					:
		case _XDEF_INV_CASHITEMBUTTON				:
		case _XDEF_INV_WASTBASKET_CANCLE			:
		case _XDEF_INV_WASTBASKET_CHECKPASSWOAD		:
		case _XDEF_INV_WASTBASKET_CASHITEM			:
		case _XDEF_INV_WASTBASKET_NORMALITEM		:
		case _XDEF_INV_BASETAB_CASHBACKPACK1		: //Author : 양희왕 //breif : 행낭 확장
		case _XDEF_INV_BASETAB_CASHBACKPACK2		: //Author : 양희왕 //breif : 행낭 확장
			InventoryMessageHandling(wparam, lparam);
			break;

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
		case _XDEF_FUNCTONIONALOBJRECTVIEW_CLOSE	:
		case _XDEF_CHATDEFAULT_EMOTIONLISTBOX		:
#ifdef _XDEF_CASHITEM_SHOUT
		case _XDEF_CHATDEFAULT_FUNCTION_SHOUTBUTTON	:
#endif
#ifdef _XDEF_ZONECHATTING_20070108
		case _XDEF_CHATDEFAULT_FUNCTION_ZONEBUTTON		:
#endif //_XDEF_ZONECHATTING_20070108
		case _XDEF_CHATDEFAULT_RETURNTOTOWN			:
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

		case _XDEF_CHATDEFAULT_NORMALFILTERMENUBUTTON	:
		case _XDEF_CHATDEFAULT_WHISPERFILTERMENUBUTTON	:
		case _XDEF_CHATDEFAULT_PARTYFILTERMENUBUTTON	:
		case _XDEF_CHATDEFAULT_HEROBANDFILTERMENUBUTTON	:
		case _XDEF_CHATDEFAULT_LOCKFILTERMENUBUTTON		:
		case _XDEF_CHATDEFAULT_CHATBLACKLISTUBUTTON		:
		case _XDEF_CHATDEFAULT_USER_CHATBLACKLISTUBUTTON:
			ChatWindowMessageHandling( wparam, lparam );			
			break;
		
		// Character informations window message handling---------------------------
		case _XDEF_CI_CLOSEBUTTON					:
		case _XDEF_CI_MODIFYBUTTON1					:
		case _XDEF_CI_MODIFYBUTTON2					:
		case _XDEF_CI_MODIFYBUTTON3					:
		case _XDEF_CI_MODIFYBUTTON4					:
		case _XDEF_CI_MODIFYBUTTON5					:
		case _XDEF_CI_IVENTORYBUTTON				:
		case _XDEF_CI_NICKBUTTON_NICKNAME			:
		case _XDEF_CI_FAVORITESNICKNAMELIST			:
		case _XDEF_CI_FAVORITESNICKNAMELIST_BTN		:
		case _XDEF_CI_MATCHBUTTON					:
		case _XDEF_CI_NICKBUTTON_MASTERANDPUPIL		: //Author : 양희왕 //breif : 인물창에서 사제 쓸수 있게
			CIWindowMessageHandling( wparam, lparam );
			break;

		// Minimap window message handling------------------------------------------
		case _XDEF_MINIMAP_CLOSEBUTTON				:
		case _XDEF_MINIMAP_ZOOMINBUTTON				:			
		case _XDEF_MINIMAP_ZOOMOUTBUTTON			:
		case _XDEF_MINIMAP_ZOOMMAXBUTTON			:
		case _XDEF_MINIMAP_WORLDMAPBUTTON			:
		case _XDEF_MINIMAP_NPCSEARCHBTN				:
		case _XDEF_MINIMAP_BTN_WEBBOARD				:			
		case _XDEF_MAXMINIMAP_CLOSEBUTTON			:		
		case _XDEF_MAXMINIMAP_NPCVIEWBUTTON			:
		case _XDEF_MAXMINIMAP_PARTYVIEWBUTTON		:
		case _XDEF_MAXMINIMAP_APPLYBUTTON			:
		case _XDEF_MINIMAP_BTN_PKTRACING			: //Author : 양희왕
		case _XDEF_MAXMINIMAP_ALPHASLIDER			: //Author : 양희왕 //breif : 투명도 슬라이더
			MinimapWindowMessageHandling( wparam, lparam );
			break;
		case _XDEF_MINIMAP_HELPBOOKBTN				:
			{
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_HEDEK, 0), (LPARAM)gHWnd);		
			}
			break;
		// NPC Search window message handling------------------------------------------
		case _XDEF_NPCSEARCH_LISTBOX				:
			{
				if( g_pMinimap_Window )
				{			
					
					g_pMinimap_Window->m_bDrawNPCSearchEffect = TRUE;
					int index = (DWORD)lparam -1;
					switch(index) {
					case 0:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_FOOD;
						}
						break;
					case 1:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_CLOTHES;
						}
						break;
					case 2:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_REFINING;
						}
						break;
					case 3:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_HOTEL;
						}
						break;
					case 4:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_WAREHOUSE;
						}
						break;
					case 5:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_DRUGSTORE;
						}
						break;
					case 6:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_SHOP;
						}
						break;
					case 7:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_WEAPON;
						}
						break;
					case 8:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_MILITARYOFFICER;
						}
						break;
					case 9:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_HEANGSANGIN;
						}
						break;
					case 10:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_BUFF;
						}
						break;
					case 11:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_RESOURCE;
						}
						break;
					case 12:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_COLLECTION;
						}
						break;
					case 13:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_GUARD;
						}
						break;
					case 14:
						{
							g_pMinimap_Window->m_SelectedNPCJobIndex = _XNPC_JOBCLASS_REPAIR;
						}
						break;
					default:
						{
							g_pMinimap_Window->m_bDrawNPCSearchEffect = FALSE;
						}
						break;
					}

					if( g_pMinimap_Window->m_bDrawNPCSearchEffect )
					{
						bool bexist = false;
						for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; i++)
						{		
							if( g_Npclist.m_pNPCObjectArray[i].m_bShowNPCModel )
							{
								if( g_Npclist.m_pNPCObjectArray[i].m_NPCJobClass == g_pMinimap_Window->m_SelectedNPCJobIndex )
								{
									bexist = true;
									break;
								}
							}
						}
						
						if( !bexist )
						{
							g_pMinimap_Window->m_bDrawNPCSearchEffect = FALSE;
						}
					}
				}
				
			}
			break;
		case _XDEF_NPCSEARCH_CLOSE					:
			{
				_XWindow_NPCSearch* pNPCSearch_Window = (_XWindow_NPCSearch*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSEARCHWINDOW );	
				if( pNPCSearch_Window )
				{
					pNPCSearch_Window->ShowWindow( FALSE );
					
					if( g_pMinimap_Window )
					{							
						g_pMinimap_Window->m_bDrawNPCSearchEffect = FALSE;
					}
				}
			}
			break;
			

		// Skill window message handling---------------------------------------------
		case _XDEF_SKILLWINDOW_LISTBOX				:
		case _XDEF_SKILLWINDOW_LEFT					:
		case _XDEF_SKILLWINDOW_RIGHT				:
		case _XDEF_SKILLWINDOW_SKILL_1				:
		case _XDEF_SKILLWINDOW_SKILL_2				:
		case _XDEF_SKILLWINDOW_SKILL_3				:
		case _XDEF_SKILLWINDOW_SKILL_4				:
		case _XDEF_SKILLWINDOW_SKILL_5				:
		case _XDEF_SKILLWINDOW_SKILL_6				:
		case _XDEF_SKILLWINDOW_SKILL_7				:
		case _XDEF_SKILLWINDOW_SKILL_8				:
#ifdef _XTS_NEWSKILLWINDOW
		case _XDEF_SKILLWINDOW_SKILLSORT_1			:
		case _XDEF_SKILLWINDOW_SKILLSORT_2			:
		case _XDEF_SKILLWINDOW_SKILLSORT_3			:
		case _XDEF_SKILLWINDOW_SKILLSORT_4			:
		case _XDEF_SKILLWINDOW_SKILLSORT_5			:
		case _XDEF_SKILLWINDOW_SKILLSORT_6			:
		case _XDEF_SKILLWINDOW_SKILLSORT_7			:
		case _XDEF_SKILLWINDOW_SKILLSORT_8			:
#endif
			SkillWindowMessageHandling(wparam, lparam);
			break;
		
		// Party window message handling---------------------------------------------
		case _XDEF_PARTYWINDOW_CLOSE :
		case _XDEF_PARTYWINDOW_TAB_BANGHOI			:
		case _XDEF_PARTYWINDOW_TAB_SEGA				:
		case _XDEF_PARTYWINDOW_TAB_FRIEND			:
		case _XDEF_PARTYWINDOW_TAB_PARTY			:
		case _XDEF_PARTYWINDOW_MASTERMINI			:
		case _XDEF_PARTYWINDOW_LEAVE				:
		case _XDEF_PARTYWINDOW_USERMINI				:
		case _XDEF_PARTYWINDOW_USERSECESSION		:
		case _XDEF_PARTYWINDOW_USERLEAVE			:
		case _XDEF_PARTYWINDOW_LISTBOX				:
		case _XDEF_PARTYWINDOW_DISSOLUTION			:

		// Mini party member info window...
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON1		:
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON2		:	
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON3		:
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON4		:	
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON5		:	
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON6		:	
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON7		:	
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON8		:	
		case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON9		:
		case _XDEF_MINIPARTYWINDOW_MINIMIZEBUTTON	:	// partymaster
		case _XDEF_MINIPARTYWINDOW_MAXIMIZEBUTTON	:	// partymaster
		case _XDEF_MINIPARTYWINDOW_EXPSHARINGBUTTON	:	// partymaster
		case _XDEF_MINIPARTYWINDOW_ITEMSHARINGBUTTON:	// partymaster
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON1		:	
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON2		:
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON3		:	
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON4		:	
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON5		:	
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON6		:	
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON7		:	
		case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON8		:	
		case _XDEF_MINIPARTYWINDOW_YES_PARTYDISSOLUTION	:	
		case _XDEF_MINIPARTYWINDOW_YES_PARTYMEMBERLEAVE	:	
		case _XDEF_MINIPARTYWINDOW_YES_PARTYLEAVE		:	
		case _XDEF_MINIPARTYWINDOW_YES_PARTYMASTERENTRUST:	
		case _XDEF_MINIPARTYWINDOW_EXPSHAERINGLISTBOX	:	// partymaster
		case _XDEF_MINIPARTYWINDOW_ITEMSHAERINGLISTBOX	:	// partymaster
		case _XDEF_MINIPARTYWINDOW_NO_PARTYREQUEST		:	// partymaster
			
			PartyWindowMessageHandling(wparam, lparam);
			break;

		// Option window message handling---------------------------------------------
		case _XDEF_OPTIONWINDOW_CLOSE				:
		case _XDEF_OPTIONWINDOW_HELPBUTTON			:
		case _XDEF_DLGTAB_GRAPHIC					:
		case _XDEF_DLGTAB_SOUND						:
		case _XDEF_DLGTAB_GAME						:
		case _XDEF_DLGTAB_COMMUNITY					:
		case _XDEF_DLGTAB_SYSTEMEND					:
		case _XDEF_OPTION_SCREEN_RESOLUTION			:
		case _XDEF_OPTION_SCREEN_DEPTHBUFFER		:
		case _XDEF_OPTION_SCREEN_GAMMA				:
		case _XDEF_OPTION_SCREEN_TERRAINLOD			:
		case _XDEF_OPTION_SCREEN_SIGHTRANGE			:
		case _XDEF_OPTION_SCREEN_GRASSQUALITY		:
		case _XDEF_OPTION_SCREEN_GRASSANIM			:
		case _XDEF_OPTION_SCREEN_TREEANIM			:
		case _XDEF_OPTION_SCREEN_CHARACTORSHADOW	:
		case _XDEF_OPTION_SCREEN_USEBLOOMFILTER		:
		case _XDEF_OPTION_SCREEN_ENABLEBLOOMFILTER	:
		case _XDEF_OPTION_SCREEN_EFFECTQUALITY		:
		case _XDEF_OPTION_SCREEN_REFLECTION			:
		case _XDEF_OPTION_SCREEN_LENSFLARE			:
		case _XDEF_OPTION_SOUND_USEBGM				:
		case _XDEF_OPTION_SOUND_BGMVOLUME			:
		case _XDEF_OPTION_SOUND_USEEFFECTSOUND		:
		case _XDEF_OPTION_SOUND_EFFECTSOUNDVOLUME	:
		case _XDEF_OPTION_SOUND_USEENVSOUND			:
		case _XDEF_OPTION_SOUND_ENVVOLUMESLIDER		:
		case _XDEF_OPTION_SOUND_USEGUIDESOUND		:
		case _XDEF_OPTION_COMMUNITYREJECTIONWHISPER :
		case _XDEF_OPTION_COMMUNITYREJECTFRIEND     :
		case _XDEF_OPTION_COMMUNITYREJECTPARTY		:
		case _XDEF_OPTION_COMMUNITYREJECTORGANIZATION	:
		case _XDEF_OPTION_COMMUNITYEXCHANGE		    :
		case _XDEF_OPTION_COMMUNITYCONNECTMESSENGER :
		case _XDEF_OPTION_COMMUNITYDISABLEZONECHAT :
		case _XDEF_OPTION_COMMUNITYTARGETINFO		:
		case _XDEF_OPTION_GAME_GAMEGUIDE			:
		case _XDEF_OPTION_GAME_VIEWUSERTOOLTIP		:
		case _XDEF_OPTION_GAME_VIEWMYTOOLTIP		:
		case _XDEF_OPTION_GAME_VIEWITEMTOOLTIP		:
		case _XDEF_OPTION_GAME_HELPSYSTEM			:
		case _XDEF_OPTION_GAME_CAMERAQUATERVIEW		:
		case _XDEF_OPTION_GAME_USEFIRSTPERSONVIEW	:
		case _XDEF_OPTION_GAME_VIEWMONSTERGAUGE		:
		case _XDEF_OPTION_GAME_VIEWBATTLEGAUGE		:
		case _XDEF_OPTION_GAME_RANKHIDING			:
		case _XDEF_OPTION_GAME_DENYMATCH			:		
		case _XDEF_OPTION_GAME_CHATTINGAUTOCLOSE	:
		case _XDEF_OPTION_GAME_KEYTYPE_A :
		case _XDEF_OPTION_GAME_KEYTYPE_B :
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI			
		case _XDEF_OPTION_GAME_SELECTCLOTHES		:
#endif
			OptionWindowMessageHandling( wparam, lparam );
			break;

		case _XDEF_OPTIONMENU_CLOSEBUTTON			:
		case _XDEF_OPTIONMENU_HELPBUTTON			:
		case _XDEF_OPTIONMENU_GRAPHICTABBUTTON		:
		case _XDEF_OPTIONMENU_SOUNDTABBUTTON		:
		case _XDEF_OPTIONMENU_GAMETABBUTTON			:
		case _XDEF_OPTIONMENU_COMMUNITYTABBUTTON	:
		case _XDEF_OPTIONMENU_ANOTHORUSERTABBUTTON	:
		case _XDEF_OPTIONMENU_GOTOLOGINTABBUTTON	:
		case _XDEF_OPTIONMENU_EXITGAMETABBUTTON		:
			OptionMenuWindowMessageHandling( wparam, lparam );
			break;

		// NPC Script window message handling---------------------------------------------		
		case _XDEF_NPCSCRIPT_CLOSE					:
		case _XDEF_NPCSCRIPT_NEXTSCRIPT				:
		case _XDEF_NPCSCRIPT_RENEW					:
		case _XDEF_NPCSCRIPT_QA						:
		case _XDEF_NPCSCRIPT_SAVEPOINT				:
		case _XDEF_NPCSCRIPT_QUESTOK				:
		case _XDEF_NPCSCRIPT_BACKTONPCMODE			:
		case _XDEF_NPCSCRIPT_LISTBOXQUESTREWARD		:
		case _XDEF_NPCSCRIPT_LISTBOXSELECTION		:
		case _XDEF_NPCSCRIPT_HEALOK					:
		case _XDEF_NPCSCRIPT_MINISTOPOK				:	
		case _XDEF_NPCSCRIPT_NPCDIALOGNEXTPAGE		:	
		case _XDEF_NPCSCRIPT_EVENTREWARDYES			:
		case _XDEF_NPCSCRIPT_EVENTREWARDNO			:	
		case _XDEF_NPCSCRIPT_LISTBOXHAVEQUESTLIST	:
		case _XDEF_NPCSCRIPT_MASTERNICKNAMEYES		: //Author : 양희왕 //breif : 사제 사호신청
		case _XDEF_NPCSCRIPT_MASTERNICKNAMENO		: //Author : 양희왕 //breif : 사제 사호취소
			NPCScriptWindowMessageHandling(wparam, lparam);
			break;

		// NPC Trade Window Message handling ---------------------------------------------
		case _XDEF_NPCTRADE_CLOSE :
		case _XDEF_NPCTRADE_FIRST_WEAPONTAB			:
		case _XDEF_NPCTRADE_FIRST_CLOTHESTAB		:
		case _XDEF_NPCTRADE_FIRST_POTIONTAB			:
		case _XDEF_NPCTRADE_SECOND_SHORTTAB			:
		case _XDEF_NPCTRADE_SECOND_LONGTAB			:
		case _XDEF_NPCTRADE_SECOND_BOWTAB			:
		case _XDEF_NPCTRADE_SECOND_SOFTTAB			:
		case _XDEF_NPCTRADE_SECOND_HIDDENTAB		:
		case _XDEF_NPCTRADE_SECOND_MUSICTAB			:
		case _XDEF_NPCTRADE_LISTBOX					:
		case _XDEF_NPCTRADE_BUYOK					:
		case _XDEF_NPCTRADE_BUYNO					:
		case _XDEF_NPCTRADE_SELLOK					:
		case _XDEF_NPCTRADE_SELLNO					:
		case _XDEF_NPCTRADE_HELP					:
		case _XDEF_NPCTRADE_ONE						:
		case _XDEF_NPCTRADE_TWO						:
		case _XDEF_NPCTRADE_THREE					:
		case _XDEF_NPCTRADE_FOUR					:
		case _XDEF_NPCTRADE_FIVE					:
			NPCTradeWindowMessageHandling(wparam, lparam);
			break;
	
		// Trade confirm window Message handling
		case _XDEF_TRADECONFIRM_CLOSE				:
		case _XDEF_TRADECONFIRM_OK					:
		case _XDEF_TRADECONFIRM_ALL					:
			TradeConfirmWindowMessageHandling(wparam, lparam);
			break;

		// Request party window message handling
		case _XDEF_REQUESTPARTY_CLOSE				:
		case _XDEF_REQUESTPARTY_SENDMESSAGE			:	
		case _XDEF_REQUESTPARTY_REQUESTPARTY		:
		case _XDEF_REQUESTPARTY_JOINPARTYOK			:
		case _XDEF_REQUESTPARTY_JOINPARTYNO			:
		case _XDEF_REQUESTPARTY_REQUESTTRADE		:
		case _XDEF_REQUESTPARTY_REQUESTFRIEND		:
		case _XDEF_REQUESTPARTY_FRIENDOK			:
		case _XDEF_REQUESTPARTY_FRIENDNO			:
		case _XDEF_REQUESTPARTY_JOINBOSSBATTLE		:
		case _XDEF_REQUESTPARTY_REQUESTBOSSBATTLE	:
		case _XDEF_REQUESTPARTY_SOLOBOSSBATTLE		:
		case _XDEF_REQUESTPARTY_REQPVP				:
		case _XDEF_REQUESTPARTY_REQGROUP			:
		case _XDEF_REQUESTPARTY_REQPVPGROUP			:
#ifdef _XTS_ALIANCE_20061018
		case _XDEF_REQUESTPARTY_REQALIENCE			:	//영웅단 동맹 신청
#endif
			RequestPartyWindowMessageHandling(wparam, lparam);
			break;

		// User Warehouse window message handling
		case _XDEF_WAREHOUSEWINDOW_CLOSE :
		case _XDEF_WAREHOUSEWINDOW_HELP  :
		case _XDEF_WAREHOUSEWINDOW_SENDADDITEM		:
		case _XDEF_WAREHOUSEWINDOW_ALLSELECT		:
		case _XDEF_WAREHOUSEWINDOW_STORAGEMONEY		:
		case _XDEF_WAREHOUSEWINDOW_INVENTORYMONEY	:
		case _XDEF_WAREHOUSEWINDOW_ONE				:
		case _XDEF_WAREHOUSEWINDOW_TWO				:
        case _XDEF_WAREHOUSEWINDOW_THREE            : //Author : 양희왕 //breif : 창고확장
		case _XDEF_WAREHOUSEWINDOW_REMOVAL			:
			WarehouseWindowMessageHandling(wparam, lparam);
			break;
			
		// AddSaveItem window message handling
		case _XDEF_ADDSAVEITEMWINDOW_CLOSE			:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN1		:// 2004.05.20->oneway48 insert 
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN2		:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN3		:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN4		:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN5		:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN6		:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN7		:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN8		:
		case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN9		: // insert end
		case _XDEF_ADDSAVEITEMWINDOW_OK				:
		case _XDEF_ADDSAVEITEMWINDOW_HELP			:
		case _XDEF_ADDSAVEITEMWINDOW_POCKET1		: //Author : 양희왕 //breif : 창고 확장, 이미지를 버튼으로
		case _XDEF_ADDSAVEITEMWINDOW_POCKET2		:
			AddSaveItemWindowMessageHandling(wparam, lparam);
			break;

			// PC Trade window message handling----------------------------------------
		case _XDEF_PCTRADEWINDOW_CLOSE				:
		case _XDEF_PCTRADEWINDOW_YES				:
		case _XDEF_PCTRADEWINDOW_NO					:		
		case _XDEF_PCTRADEWINDOW_OK					:
		case _XDEF_PCTRADEWINDOW_CANCEL				:
		case _XDEF_PCTRADEWINDOW_HELP				:
		case _XDEF_PCTRADEWINDOW_MONEY				:
			PCTradeWindowMessageHanlding(wparam, lparam);
			break;

			// Messenger window messge handling-----------------------------------------
		case _XDEF_MESSENGERWINDOW_CLOSEBUTTON:	
		case _XDEF_MESSENGERWINDOW_HELPBUTTON:	
		case _XDEF_MESSENGERWINDOW_MESSENGERLOGIN:
		case _XDEF_MESSENGERWINDOW_MESSENGERLOGOUT:		
		case _XDEF_MESSENGERWINDOW_ADDGROUPBUTTON:
		case _XDEF_MESSENGERWINDOW_ADDFRIENDBUTTON:	
		case _XDEF_MESSENGERWINDOW_CHANGESTATUSBUTTON:
		case _XDEF_MESSENGERWINDOW_RECALLBUTTON : //Author : 양희왕 //breif : 친구소환 버튼
			MessengerWindowMessageHandling( wparam, lparam );
			break;
			
		// MessengerTooltip window messge handling-----------------------------------------
		case _XDEF_MESSENGERTOOLTIPWINDOW_ADDGROUPBUTTON:
		case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPBUTTON:
		case _XDEF_MESSENGERTOOLTIPWINDOW_CHANGERGROUPNAMEBUTTON:
		case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDBUTTON:
		case _XDEF_MESSENGERTOOLTIPWINDOW_CLOSEBUTTON:
		case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPOK:
		case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPNO:
		case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDOK:
		case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDNO:			
			MessengerTooltipWindowMessageHandling( wparam, lparam );
			break;
						
		// Name Input window messge handling-----------------------------------------
		case _XDEF_NAMEINPUTWINDOW_OK:
		case _XDEF_NAMEINPUTWINDOW_CLOSE:
			NameInputWindowMessageHandling( wparam, lparam );
			break;
			
		// Email window messge handling-----------------------------------------
		case _XDEF_EMAILWINDOW_EMAILRECEIVEBOX:
		case _XDEF_EMAILWINDOW_EMAILSENDBOX:
		case _XDEF_EMAILWINDOW_EMAILKEEPBOX:
		case _XDEF_EMAILWINDOW_EMAILCLOSE:
		case _XDEF_EMAILWINDOW_EMAILHELP:
		case _XDEF_EMAILWINDOW_EMAILNEXT:
		case _XDEF_EMAILWINDOW_EMAILPREV:
		case _XDEF_EMAILWINDOW_EMAILKEEPINGMAILDELETE:
		case _XDEF_EMAILWINDOW_EMAILRECEIVEREPLY:
		case _XDEF_EMAILWINDOW_EMAILRECEIVESAVE:
		case _XDEF_EMAILWINDOW_EMAILRECEIVEDELETE:
		case _XDEF_EMAILWINDOW_EMAILSENDDELETE:
		case _XDEF_EMAILWINDOW_EMAILSENDCANCLE:
		case _XDEF_EMAILWINDOW_EMAILSEND:
			EmailWindowMessageHandling( wparam, lparam );
			break;
			
		// NPC Skill window message handling---------------------------------------------
		case _XDEF_NPCSKILLWINDOW_LISTBOX			:
		case _XDEF_NPCSKILLWINDOW_REQLEARNSKILL		:
		case _XDEF_NPCSKILLWINDOW_REQLEARNSKILLBUTTON	:
		case _XDEF_NPCSKILLWINDOW_HELP :
		case _XDEF_NPCSKILLWINDOW_LEFT :
		case _XDEF_NPCSKILLWINDOW_RIGHT :
		case _XDEF_NPCSKILLWINDOW_SKILL_1 :
		case _XDEF_NPCSKILLWINDOW_SKILL_2 :
		case _XDEF_NPCSKILLWINDOW_SKILL_3 :
		case _XDEF_NPCSKILLWINDOW_SKILL_4 :
		case _XDEF_NPCSKILLWINDOW_SKILL_5 :
		case _XDEF_NPCSKILLWINDOW_SKILL_6 :
		case _XDEF_NPCSKILLWINDOW_SKILL_7 :
		case _XDEF_NPCSKILLWINDOW_SKILL_8 :
			NPCSkillWindowMessageHandling(wparam, lparam);
			break;
				
		// System menu window message handling----------------------------------------
		case _XDEF_MAINMENU_MAINWINDOW				:
		case _XDEF_MAINMENU_INVENTORY				:
		case _XDEF_MAINMENU_OPTION					:
		case _XDEF_MAINMENU_HEDEK					:
		case _XDEF_MAINMENU_MESSENGER				:
		case _XDEF_MAINMENU_EMAIL					:	
		case _XDEF_MAINMENU_MEDBUTTON				:
#ifdef _XTS_ITEMMALLBROWSER
		case _XDEF_MAINMENU_BTN_CASHMALL:
		case _XDEF_MAINMENU_BTN_CASHBOX:
#endif
			SystemMenuWindowMessageHandling( wparam, lparam );
			break;

			// Rebirth window message handling -------------------------------------------
		case _XDEF_REBIRTHWINDOW_TOWN				:
		case _XDEF_REBIRTHWINDOW_FIELD				:
		case _XDEF_REBIRTHWINDOW_USEITEM			:
		case _XDEF_REBIRTHWINDOW_SAVEPOINT			:
		case _XDEF_REBIRTHWINDOW_ACCLAIMVIDEOAD		:	
			RebirthWindowMessageHandling(wparam, lparam);
			break;
			
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK범위치추적, 레벨차 PK제한	
			// PKTracingRequest window message handling -------------------------------------------
		case _XDEF_PKTRACINGREQUEST_BTN_REQUEST1	:
		case _XDEF_PKTRACINGREQUEST_BTN_REQUEST2	:
		case _XDEF_PKTRACINGREQUEST_BTN_REQUEST3	:
		case _XDEF_PKTRACINGREQUEST_BTN_CANCLE		:
		case _XDEF_PK_TRACINGSERVICES_BTN_CLOSE		: //Author : 양희왕
		case _XDEF_PK_TRACINGSERVICES_LISTBOX		: //Author : 양희왕
		case _XDEF_PKTRACINGREQUEST_BTN_REQUEST1_YES	:
		case _XDEF_PKTRACINGREQUEST_BTN_REQUEST2_YES	:
		case _XDEF_PKTRACINGREQUEST_BTN_REQUEST3_YES	:
		case _XDEF_PKTRACINGREQUEST_BTN_REQUESTCANCE	:	
			PKTracingRequestWindowMessageHandling(wparam, lparam);
#endif
			//2004.06.17->oneway48 insert
			// RepairItem window message handling -------------------------------------------
		case _XDEF_REPAIRITEMWINDOW_ALLBUTTON		:
		case _XDEF_REPAIRITEMWINDOW_REPAIRBUTTON	:
		case _XDEF_REPAIRITEMWINDOW_CANCLE			:
		case _XDEF_REPAIRITEMWINDOW_ALLREPAIRYES	:
		case _XDEF_REPAIRITEMWINDOW_ALLREPAIRNO		:
		case _XDEF_REPAIRITEMWINDOW_CLOSE			:
		case _XDEF_REPAIRITEMWINDOW_HELP			:
		case _XDEF_REPAIRITEMWINDOW_SELECTEDALL		:
		case _XDEF_REPAIRITEMWINDOW_SELECTEDCANCLE	:	
			RepairItemWindowMessageHandling(wparam, lparam);
			break;
			
		// SocketPlugIn window message handling ------------------------------------------=
		case _XDEF_SOCKETPLUGIN_APPLY				:
		case _XDEF_SOCKETPLUGIN_CANCLE				:
		case _XDEF_SOCKETPLUGIN_APPLYOK				:
		case _XDEF_SOCKETPLUGIN_APPLYCANCLE			:
		case _XDEF_SOCKETPLUGIN_CHANGEOK			:
		case _XDEF_SOCKETPLUGIN_CHANGECANCLE		:
		case _XDEF_SOCKETPLUGIN_CLOSEOK				:
		case _XDEF_SOCKETPLUGIN_CLOSECANCLE			:
		case _XDEF_SOCKETPLUGIN_CLOSE				:
		case _XDEF_SOCKETPLUGIN_HELP				:
			SocketPlugInWindowMessageHandling(wparam, lparam);
			break;
		
		case _XDEF_SOCKETPLUGINEXP_START			:
		case _XDEF_SOCKETPLUGINEXP_CANCLE			:
		case _XDEF_SOCKETPLUGINEXP_SEPARATION		:
		case _XDEF_SOCKETPLUGINEXP_DESTROY			:
		case _XDEF_SOCKETPLUGINEXP_HELP				:
			SocketPlugInExpandWindowMessageHandling(wparam, lparam);
			break;

		// Pocket window message handling ------------------------------------------=
		case _XDEF_POCKET_HELP						:
		case _XDEF_POCKET_CLOSE						:
		case _XDEF_POCKET_CLOSEOK					:		
			PocketWindowMessageHandling(wparam, lparam);
			break;
		
		case _XDEF_MONSTERSTATUS_CLOSEBUTTON		:
		case _XDEF_MONSTERSTATUS_FUNCTIONBUTTON1	:
		case _XDEF_MONSTERSTATUS_FUNCTIONBUTTON2	:
		case _XDEF_MONSTERSTATUS_FUNCTIONBUTTON3	:
		case _XDEF_MONSTERSTATUS_POPUPBUTTON1		:	
		case _XDEF_MONSTERSTATUS_POPUPBUTTON2		:	
		case _XDEF_MONSTERSTATUS_POPUPBUTTON1YES	:		
		case _XDEF_MONSTERSTATUS_POPUPBUTTON1NO		:		
		case _XDEF_MONSTERSTATUS_POPUPBUTTON2YES	:		
		case _XDEF_MONSTERSTATUS_POPUPBUTTON2NO		:
		case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON1	:
		case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON2	:
		case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON3	:
		case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON4	:
		case _XDEF_MONSTERSTATUS_INFOPOPUPYES		:
		case _XDEF_MONSTERSTATUS_INFOPOPUPNO		:
		case _XDEF_MONSTERSTATUS_INFOPOPUPCANCLE	:
		case _XDEF_TARGETINFOWINDOW_CLOSEBUTTON		:
		case _XDEF_TARGETINFOWINDOW_HELPBUTTON		:
			TargetInfoWindowMessageHandling(wparam, lparam);

		case  _XDEF_MAINFRAME_CLOSEBUTTON			:
		case  _XDEF_MAINFRAME_HELPBUTTON			:
		case  _XDEF_MAINFRAME_CHARACTORTAB			:
		case  _XDEF_MAINFRAME_PKRECORDTAB			:
		case  _XDEF_MAINFRAME_MASTERYTAB			:
		case  _XDEF_MAINFRAME_SKILLTAB				:
		case  _XDEF_MAINFRAME_LIFETAB				:
		case  _XDEF_MAINFRAME_QUESTTAB				:
		case  _XDEF_MAINFRAME_COMMUNITYTAB			:
			MainFrameWindowMessageHandling(wparam, lparam);
			break;		
		case _XDEF_CONTRIWINDOW_CLOSEBUTTON			:
		case _XDEF_CONTRIWINDOW_HELPBUTTON			:
		case _XDEF_CONTRIWINDOW_APPLYBUTTON			:
		case _XDEF_CONTRIWINDOW_CANCLE				:
			ContributionMessageHandling(wparam, lparam);
			break;
		
		case _XDEF_TUTORIAL_ICON1					:
		case _XDEF_TUTORIAL_ICON2					:
		case _XDEF_TUTORIAL_ICON3					:
		case _XDEF_TUTORIAL_ICON4					:
		case _XDEF_TUTORIAL_ICON5					:
		case _XDEF_TUTORIAL_ICON6					:
		case _XDEF_TUTORIAL_ICON7					:
		case _XDEF_TUTORIAL_ICON8					:
			TutorialIconMessageHandling(wparam, lparam);
			break;
		case _XDEF_HEDEK_CLOSE :		// 추후 수정
			{
				if( m_pHedekWindowPtr )
				{
					if( m_pHedekWindowPtr->GetShowStatus() )
						m_pHedekWindowPtr->ShowWindow( FALSE );
				}
			}			
			break;
		case _XDEF_HEDEK_HELPBOOKCALL :
			{
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_HEDEK, 0), (LPARAM)gHWnd);
			}
			break;
			
		// Help book message handler ----------------------------------------------
		case _XDEF_HELPBOOK_HELPLIST		:	
		case _XDEF_HELPBOOK_CLOSE			:
		case _XDEF_HELPBOOK_LEFTSTEP		:
		case _XDEF_HELPBOOK_RIGHTSTEP		:
			HelpBookMessageHandling(wparam, lparam);			
			break;
			
		// Personal Store window message handling -------------------------------------------
		case _XDEF_PERSONALSTORE_HELPBUTTON				:
		case _XDEF_PERSONALSTORE_CLOSEBUTTON			:
		case _XDEF_PERSONALSTORE_OPENBUTTON				:
		case _XDEF_PERSONALSTORE_CANCLEBUTTON			:
		case _XDEF_PERSONALSTORE_BUYOK					:
		case _XDEF_PERSONALSTORE_BUYNO					:
		case _XDEF_PERSONALSTORE_INPUTOKBUTTON			:
			PersonalStoreWindowMessageHandling(wparam, lparam);
			break;
			
			// LifeSkill window message handling -------------------------------------------
		case _XDEF_LIFESKILL_CLASSBUTTON1				:
		case _XDEF_LIFESKILL_CLASSBUTTON2				:
		case _XDEF_LIFESKILL_CLASSBUTTON3				:
		case _XDEF_LIFESKILL_CLASSBUTTON4				:
		case _XDEF_LIFESKILL_CLASSBUTTON5				:
		case _XDEF_LIFESKILL_TYPEBUTTON1				:
		case _XDEF_LIFESKILL_TYPEBUTTON2				:
		case _XDEF_LIFESKILL_TYPEBUTTON3				:
		case _XDEF_LIFESKILL_TYPEBUTTON4				:
		case _XDEF_LIFESKILL_TYPEBUTTON5				:
		case _XDEF_LIFESKILL_TYPEBUTTON6				:
		case _XDEF_LIFESKILL_TYPEBUTTON7				:
		case _XDEF_LIFESKILL_COLLECTIONLISTBOX			:
		case _XDEF_LIFESKILL_RESOURCELISTBOX			:
		case _XDEF_LIFESKILL_RESOURCENEEDITEMLISTBOX	:
		case _XDEF_LIFESKILL_INPUTCOLLECTIONBUTTON		:
		case _XDEF_LIFESKILL_INPUTCOLLECTION_OK			:
		case _XDEF_LIFESKILL_INPUTCOLLECTION_NO			:
		case _XDEF_LIFESKILL_USEBUTTON					:
		case _XDEF_LIFESKILL_PVPPRBUTTON				:
		case _XDEF_LIFESKILL_EVENTBUTTON				:
			LifeSkillWindowMessageHandling(wparam, lparam);
			break;
			
		// Event window message handling -------------------------------------------
		case _XDEF_EVENT_BTN_CLOSE						:
		case _XDEF_EVENT_BTN_HELP						:
		case _XDEF_EVENT_LISTBOX_TITLE					:
		case _XDEF_EVENT_LISTBOX_DESCRIPTION			:
		case _XDEF_EVENT_BTN_OK							:
		case _XDEF_EVENT_BTN_GET : //Author : 양희왕 추가
		case _XDEF_EVENT_BTN_APPLY :
			EventWindowMessageHandling(wparam, lparam);
			break;
			
			
			
		// GM Command message handler ---------------------------------------------
		case _XDEF_GMWINDOW_OPENBUTTON		:			
		case _XDEF_GMWINDOW_COMMANDBUTTON0	:			
		case _XDEF_GMWINDOW_COMMANDBUTTON1	:			
		case _XDEF_GMWINDOW_COMMANDBUTTON2	:
		case _XDEF_GMWINDOW_COMMANDBUTTON3	:
		case _XDEF_GMWINDOW_COMMANDBUTTON4	:
		case _XDEF_GMWINDOW_COMMANDBUTTON5	:
		case _XDEF_GMWINDOW_COMMANDBUTTON6	:
		case _XDEF_GMWINDOW_COMMANDBUTTON7	:
		case _XDEF_GMWINDOW_COMMANDBUTTON8	:
		case _XDEF_GMWINDOW_COMMANDBUTTON9	:

		case  _XDEF_GMCHARACTERSETTINGWINDOWCLOSEBTN		:			
		case  _XDEF_GMSTATUSCONTROLHIDDENBUTTON				:
		case  _XDEF_GMSTATUSCONTROLCAMERASPEEDSLIDER		:
		case  _XDEF_GMSTATUSCONTROLFOGDISTANCESLIDER		:
		case  _XDEF_GMSTATUSCONTROLENVIRONMENTSLIDER		:
		case  _XDEF_GMSTATUSCONTROLTIMEFLOWCHECKBUTTON		:
		case  _XDEF_GMSTATUSCONTROLFLYCAMMODE				:
		case  _XDEF_GMSTATUSCONTROLCURSORVIEW				:
		
		case  _XDEF_GMCHARACTERCOORDSETTINGWINDOWCLOSEBTN	:
		case  _XDEF_GMCHARACTERCOORDSETTINGGMMOVE			:
		case  _XDEF_GMCHARACTERCOORDSETTINGGMMOVETOUSER		:
		case  _XDEF_GMCHARACTERCOORDSETTINGMOVEUSER			:

		case  _XDEF_GMCHARACTERINFOSETTINGWINDOWCLOSEBTN	:
		case  _XDEF_GMUSERMANAGEMENTWINDOWCLOSEBTN			:
		case  _XDEF_GMCONSULTATIONWINDOWCLOSEBTN			:
		
		case  _XDEF_GMNOTICEMESSAGEWINDOWCLOSEBTN			:
		case  _XDEF_GMNOTICEMESSAGEINSERTBTN				:
		case  _XDEF_GMNOTICEMESSAGEDELETEBTN				:
		case  _XDEF_GMNOTICEMESSAGECONFIRMDELETE			:
		case  _XDEF_GMNOTICEMESSAGEDELETEALLBTN				:
		case  _XDEF_GMNOTICEMESSAGECONFIRMDELETEALL			:

		case  _XDEF_GMCONTROLMONSTERWINDOWCLOSEBTN			:
		case  _XDEF_GMMONSTERSPAWNBUTTON					:
		case  _XDEF_GMMONSTERSPAWN_MOBLISTBOX				:

		case  _XDEF_GMEVENTCONTROLWINDOWCLOSEBTN			:
		case  _XDEF_GMDEBUGCOMMANDWINDOWCLOSEBTN			:
		case  _XDEF_GMCOMMANDLOGWINDOWCLOSEBTN				:

		case _XDEF_GMEVENTCONTROLWINDOW_BGM1				:
		case _XDEF_GMEVENTCONTROLWINDOW_BGM2				:
		case _XDEF_GMEVENTCONTROLWINDOW_BGM3				:
		case _XDEF_GMEVENTCONTROLWINDOW_BGMSTOP				:
		case _XDEF_GMEVENTCONTROLWINDOW_FIREWORK			:
		case _XDEF_GMEVENTCONTROLWINDOW_FLOWER1				:
		case _XDEF_GMEVENTCONTROLWINDOW_FLOWER2				:
		case _XDEF_GMEVENTCONTROLWINDOW_FLOWER3				:
		case _XDEF_GMEVENTCONTROLWINDOW_WEDDINGSTAGE		:
		case _XDEF_GMEVENTCONTROLWINDOW_WEDDINGSTAGEEND		:
		case _XDEF_GMEVENTCONTROLWINDOW_ENVDAY				:
		case _XDEF_GMEVENTCONTROLWINDOW_NIGHTFALL			:
		case _XDEF_GMEVENTCONTROLWINDOW_NIGHT				:
			GMCommandMessageHandling(wparam, lparam);
			break;
			
		// 장원전 관련
		case _XDEF_CASTLEINFO_REQWAR						:
		case _XDEF_CASTLEINFO_REQWAR_YES					:
		case _XDEF_CASTLEINFO_CANCELWAR						:
		case _XDEF_CHALLENGERLIST_REQLIST					:

		//장원전 사자후 관련
		case _XDEF_CASTLEBATTLE_SHOUT_BTN_CLOSE				:
		case _XDEF_CASTLEBATTLE_SHOUT_BTN_SHOUT				:

		// 장원전 결과 창
		case _XDEF_CBRESULTNOTICE_CLOSE						:

		// 장원전 시간변경 관련
		case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_CLOSE		:
		case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_CHANGE		:
		case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN1	:
		case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN2	:
		case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN3	:
		case _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME1	:	
		case _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME2	:	
		case _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME3	:	
		case _XDEF_CASTLEBATTLE_REWARDLIST_BTN_OK			:
		case _XDEF_CBORGRECORD_OK							:
		case _XDEF_CBREWARDSMALL_OK							:
		case _XDEF_CBPERSONAL_VIEWRECORD					:
			CastleWarWindowMessageHandling(wparam, lparam);
			break;
			
		// Area select message
		case _XDEF_AREASELECTWINDOW_MOVEBUTTON				:
			{
				bool multiarea = false;
				_XArea* pCurrentFocusedArea = NULL;				
				if(g_pWhiteClanAreaSelect_Window)
				{
					if( g_pWhiteClanAreaSelect_Window->GetShowStatus() )
					{
						pCurrentFocusedArea = g_pWhiteClanAreaSelect_Window->m_pFocusedArea;
					}
					g_pWhiteClanAreaSelect_Window->ShowWindow( FALSE );
				}
				
				if(g_pBlackClanAreaSelect_Window)
				{
					if( g_pBlackClanAreaSelect_Window->GetShowStatus() )
					{
						pCurrentFocusedArea = g_pBlackClanAreaSelect_Window->m_pFocusedArea;
					}

					g_pBlackClanAreaSelect_Window->ShowWindow( FALSE );
				}

				if(g_pMultiAreaSelect_Window)
				{
					if( g_pMultiAreaSelect_Window->GetShowStatus() )
					{
						pCurrentFocusedArea = g_pMultiAreaSelect_Window->m_pFocusedArea;
						multiarea = true;
					}
					
					g_pMultiAreaSelect_Window->ShowWindow( FALSE );
				}

				if( pCurrentFocusedArea )
				{
					if( !multiarea )
					{
#ifdef _XDEF_MOVE_PKSERVER_SELECT
						if(g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].pkclientinfo)
						{
							m_strMoveZoneInfo.currentwarpzone = pCurrentFocusedArea->m_Num;
							m_strMoveZoneInfo.targetzone = pCurrentFocusedArea->m_TargetZone+1;
							m_strMoveZoneInfo.nextwarpzone = pCurrentFocusedArea->m_TargetWarpzone;

							TCHAR tstr[256];
							_snprintf(tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_NEW_3198), g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].zonename);
							g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tstr, TRUE, _XDEF_MG_MOVEPKSERVER_YES, _XDEF_MG_MOVEPKSERVER_NO);

							m_LockMoveProcess = TRUE;
						}
						else
						{

#endif
						g_WarpProcessFadeInOutFlag = TRUE;
						g_FadeInOutLevel = 0;
						
						m_PrevCheckedWarpingZoneTimer = g_LocalSystemTime;
						m_CurrentWarpZoneIndex = pCurrentFocusedArea->m_Num; //i;		 // 현재 서있는 곳의 포탈 인덱스 서버 전송용
						m_PrevZoneInfoIndex	   = g_CurrentZoneInfoIndex;
						g_CurrentZoneInfoIndex = pCurrentFocusedArea->m_TargetZone+1;    // 현재존을 다음존인덱스로 세팅한다. 실패일경우 되돌림.
						m_NextWarpZoneIndex	   = pCurrentFocusedArea->m_TargetWarpzone;	 // 다음 존의 리스폰 리젼 인덱스
						
						m_WarpStartTime = g_LocalSystemTime; 
						
						TCHAR tstr[256];
						_snprintf( tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename );
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );
						
						
						g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 4 );// 서버이동 중이라는 상태를 알려준다.
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MG_WARPINGNEXTZONE, 0), m_NextWarpZoneIndex_COMMONGATE+1 );
						m_LockMoveProcess = TRUE;
#ifdef _XDEF_MOVE_PKSERVER_SELECT
						}
#endif
					}
					else
					{
						int targetzone = -1;
						int warpzone = -1;
						if( pCurrentFocusedArea->m_MultiTargetZone1 == m_NextWarpZoneIndex_COMMONGATE )
						{
							if( pCurrentFocusedArea->m_MultiTargetWarpzone1 != -1 || pCurrentFocusedArea->m_WarpZoneName1[0] == 0)
							{
								targetzone = pCurrentFocusedArea->m_MultiTargetZone1;
								warpzone = pCurrentFocusedArea->m_MultiTargetWarpzone1;
							}
						}
						else if( pCurrentFocusedArea->m_MultiTargetZone2 == m_NextWarpZoneIndex_COMMONGATE )
						{
							if( pCurrentFocusedArea->m_MultiTargetWarpzone2 != -1 || pCurrentFocusedArea->m_WarpZoneName2[0] == 0)
							{
								targetzone = pCurrentFocusedArea->m_MultiTargetZone2;
								warpzone = pCurrentFocusedArea->m_MultiTargetWarpzone2;
							}
						}
						else if( pCurrentFocusedArea->m_MultiTargetZone3 == m_NextWarpZoneIndex_COMMONGATE )
						{
							if( pCurrentFocusedArea->m_MultiTargetWarpzone3 != -1 || pCurrentFocusedArea->m_WarpZoneName3[0] == 0)
							{
								targetzone = pCurrentFocusedArea->m_MultiTargetZone3;
								warpzone = pCurrentFocusedArea->m_MultiTargetWarpzone3;
							}
						}
						
						if( targetzone != -1 && warpzone != -1 )
						{
#ifdef _XDEF_MOVE_PKSERVER_SELECT
							if(g_ZoneInfoTable[targetzone].pkclientinfo)
							{
								m_strMoveZoneInfo.currentwarpzone = pCurrentFocusedArea->m_Num;
								m_strMoveZoneInfo.targetzone = targetzone;
								m_strMoveZoneInfo.nextwarpzone = warpzone;

								TCHAR tstr[256];
								_snprintf(tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_NEW_3198), g_ZoneInfoTable[targetzone].zonename);
								g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tstr, TRUE, _XDEF_MG_MOVEPKSERVER_YES, _XDEF_MG_MOVEPKSERVER_NO);
							}
							else
							{

#endif
							g_WarpProcessFadeInOutFlag = TRUE;
							g_FadeInOutLevel = 0;						

							m_PrevCheckedWarpingZoneTimer = g_LocalSystemTime;
							m_CurrentWarpZoneIndex = pCurrentFocusedArea->m_Num; //i;		 // 현재 서있는 곳의 포탈 인덱스 서버 전송용
							m_PrevZoneInfoIndex	   = g_CurrentZoneInfoIndex;
							g_CurrentZoneInfoIndex = targetzone;    // 현재존을 다음존인덱스로 세팅한다. 실패일경우 되돌림.
							m_NextWarpZoneIndex	   = warpzone;	 // 다음 존의 리스폰 리젼 인덱스
							
							m_WarpStartTime = g_LocalSystemTime; 
							
							TCHAR tstr[256];
							_snprintf( tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex].zonename );
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );
							
							
							g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 4 );// 서버이동 중이라는 상태를 알려준다.
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MG_WARPINGNEXTZONE, 0), m_NextWarpZoneIndex_COMMONGATE+1 );
							m_LockMoveProcess = TRUE;
#ifdef _XDEF_MOVE_PKSERVER_SELECT
							}
#endif
						}

					}

				}
			}
			break;

		case _XDEF_AREASELECTWINDOW_CLOSEBUTTON				:
			{
				if(g_pBlackClanAreaSelect_Window)
				{
					g_pBlackClanAreaSelect_Window->ShowWindow( FALSE );
				}				
				if(g_pWhiteClanAreaSelect_Window)
				{
					g_pWhiteClanAreaSelect_Window->ShowWindow( FALSE );
				}
				if(g_pMultiAreaSelect_Window)
				{
					g_pMultiAreaSelect_Window->ShowWindow( FALSE );
				}
			}
			break;

		case  _XDEF_AREASELECTWINDOW_LISTBOX				:
			{
				m_NextWarpZoneIndex_COMMONGATE = (DWORD)lparam -1;
			}
			break;

		case _XDEF_JINSTATUS_CONVERGENCEBUTTON		:
			{
				if(g_pJinStatus_Window)
				{
					if(g_SinglePlayMode)
					{
						g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_CONVERGENCE;
						g_pJinStatus_Window->m_ConvergenceButton->ShowWindow(FALSE);
					}
					else
					{
						// 폭주 시작 요청
						g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_start_convergence);
					}
				}	
			}
			break;
			
		case _XDEF_JINSTATUS_SHUTDOWNONOFFBUTTON :
			{
				if(g_pJinStatus_Window)
				{
					if(g_pJinStatus_Window->GetSDSState() == 0)	// on
					{
						// on->off는 딜레이 없음
						g_NetworkKernel.SendPacket(MSG_NO_NATIONAL_DIST, en_national_vina_sds_ctrl, 0);	// request off
						g_pJinStatus_Window->m_btnSDSOnOff->EnableWindow(FALSE);
					}
					else if(g_pJinStatus_Window->GetSDSState() == 1) // off
					{
						// off->on은 10분 딜레이
						if(g_LocalSystemTime - g_pJinStatus_Window->GetSDSSetTime() > 600000 ||
							g_pJinStatus_Window->GetSDSSetTime() == 0)
						{
							// sds setting time이 초기화 되었거나, 세팅한지 10분이 지났거나
							g_NetworkKernel.SendPacket(MSG_NO_NATIONAL_DIST, en_national_vina_sds_ctrl, 1);	// request on
							g_pJinStatus_Window->m_btnSDSOnOff->EnableWindow(FALSE);
						}
						else
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3727), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//아직 SDS를 변경할 수 없습니다.
						}
					}
				}
			}
			break;
			
		case _XDEF_EXPWINDOW_CONVERGENCE :
			{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
				if(g_pExp_Window)
				{
					if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
					{
						if(g_SinglePlayMode)
						{
							g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_CONVERGENCE;
							g_pExp_Window->ShowConvergenceAnimation(FALSE);
						}
						else
						{
							// 폭주 시작 요청
							g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_start_convergence);
						}
					}
					else if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_NORMAL)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3694), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//분노상태가 아니므로 폭주 상태로 전환할 수 없습니다.
					}
				}
#endif
			}
			break;

		case _XDEF_DEATHBLOW_RIGHTBLOWBUTTON		:
			{

			}
			break;
		
		// Game system message handling---------------------------------------------
		case _XDEF_MG_WARPINGNEXTZONE				:
			{
				BOOL bMoveServer = TRUE;

				if(m_pPCTradeWindowPtr)
				{
					if(m_pPCTradeWindowPtr->GetShowStatus())
					{
						// 교환중에는 이동 불가
						bMoveServer = FALSE;
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2405), TRUE );
						g_MessageBox.StartTimer( 3000 );
					}
				}
				if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
					(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// 내공심법 시전중
				{
					// 운기요상 중에는 이동 불가
					bMoveServer = FALSE;
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2406), TRUE );
					g_MessageBox.StartTimer( 3000 );
				}

#ifdef _XTS_PK
				if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
				{
					// PK중에는 PK 맵이 아닌 맵으로는 이동 불가
#ifdef _XTS_PKZONE_SERVERSETTING
					if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo == 0)
#else
					if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo == 0)
#endif
					{
						bMoveServer = FALSE;

						TCHAR messagestring[256];
						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT( ID_STRING_NEW_2727 ), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename);	// "대전 중에는 %s로 이동할 수 없습니다."
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE );
						g_MessageBox.StartTimer( 3000 );
					}
				}
#endif
				
				if( !bMoveServer )
				{
					m_MoveServer	=	FALSE;
					m_MovingGameServer = FALSE;
					g_CurrentZoneInfoIndex = m_PrevZoneInfoIndex;
					m_NextWarpZoneIndex = -2;
					m_LockMoveProcess = FALSE;
					g_WarpProcessFadeInOutFlag = FALSE;
					m_WarpStartTime = 0;
					g_ClickedWarpZoneIndex = -1;
				}
				else
				{
					m_MovingGameServer = TRUE;

					// 서버간 이동전에 퀵슬롯을 저장한다. 실패하도 저장하기 위해 미리함.
					_XQuickSlotInfoList	quickslotinfolist;
					quickslotinfolist.LoadQuickSlotInfo();
					quickslotinfolist.UpdateInfoList();
					quickslotinfolist.SaveQuickSlotInfo();

					//TCHAR tstr[256];
					//sprintf( tstr, _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename );
					//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_CANCEL, " ", tstr, TRUE, _XDEF_MG_CANCELWARPINGNEXTZONE, _XDEF_MG_CANCELWARPINGNEXTZONE, _XDEF_MG_CANCELWARPINGNEXTZONE );
					
					g_NetworkKernel.SendPacket(MSG_NO_SV_MOVE_REQ, (int)lparam );

				}
			}
			break;
		case _XDEF_MG_CANCELWARPINGNEXTZONE			:
			{
				m_MoveServer	=	FALSE;
				m_MovingGameServer = FALSE;
				g_CurrentZoneInfoIndex = m_PrevZoneInfoIndex;
				m_NextWarpZoneIndex = -2;
				m_LockMoveProcess = FALSE;
				g_WarpProcessFadeInOutFlag = FALSE;
				m_WarpStartTime = 0;
				g_ClickedWarpZoneIndex = -1;
				
				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 1 );// 메신져 상태 창에서 로그인이라는 상태를 알려준다.
			}
			break;

		case _XDEF_MG_MOVEPKSERVER_YES :
			{
				g_WarpProcessFadeInOutFlag = TRUE;
				g_FadeInOutLevel = 0;

				m_PrevCheckedWarpingZoneTimer = g_LocalSystemTime;
				m_CurrentWarpZoneIndex = m_strMoveZoneInfo.currentwarpzone;		// 현재 서있는 곳의 포탈 인덱스 서버 전송용
				m_PrevZoneInfoIndex	   = g_CurrentZoneInfoIndex;
				g_CurrentZoneInfoIndex = m_strMoveZoneInfo.targetzone;			// 현재존을 다음존인덱스로 세팅한다. 실패일경우 되돌림.
				m_NextWarpZoneIndex	   = m_strMoveZoneInfo.nextwarpzone;		// 다음 존의 리스폰 리젼 인덱스

				m_WarpStartTime = g_LocalSystemTime; 

				TCHAR tstr[256];
				_snprintf( tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex].zonename );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );


				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 4 );// 서버이동 중이라는 상태를 알려준다.
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MG_WARPINGNEXTZONE, 0), m_strMoveZoneInfo.targetzone+1 );
				m_LockMoveProcess = TRUE;
			}
			break;

		case _XDEF_MG_MOVEPKSERVER_NO :
			{
				m_strMoveZoneInfo.currentwarpzone = -1;
				m_strMoveZoneInfo.nextwarpzone = -1;
				m_strMoveZoneInfo.targetzone = -1;

				g_ClickedWarpZoneIndex = -1;
				m_LockMoveProcess = FALSE;
			}
			break;

		case _XDEF_NOTICEWINDOW_CLOSEBUTTON			:
			{
				_XWindow_AdminNotice* pAdminNotice_Window = 
					(_XWindow_AdminNotice*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NOTICEWINDOW );
				if( pAdminNotice_Window )
				{
					pAdminNotice_Window->ShowWindow( FALSE );
				}
			}
			break;
			
		case _XDEF_PKRECORD_TAB_1 :
		case _XDEF_PKRECORD_TAB_2 :
		case _XDEF_PKRECORD_TAB_3 :
		case _XDEF_PKRECORD_TAB_4 :
		case _XDEF_PKRECORD_TAB_5 :
			{
				PKRecordWindowMessageHandling(wparam, lparam);
			}
			break;
			
#ifdef _XDEF_MIXWEAPON_071009_KUKURI
		case _XDEF_MIXWEAPON_OK :
		case _XDEF_MIXWEAPON_CANCEL :
			{
				MixWeaponWindowMessageHandling(wparam, lparam);
			}
			break;
#endif
			

#ifdef _XDWDEBUG
		// _XDebug window message handling-----------------------------------------
		case  _XDEF_XDEBUGINFOMODE_BASE				:
		case  _XDEF_XDEBUGINFOMODE_SYSTEM			:
		case  _XDEF_XDEBUGINFOMODE_VIDEOINFO		:
		case  _XDEF_XDEBUGINFOMODE_WORLD			:
		DebugWindowMessageHandling( wparam, lparam );			
			break;
#endif					

		case _XDEF_GAMECLOSE :
			{
#ifdef _XDWDEBUG
				if( g_SinglePlayMode )
				{
					PostMessage( gHWnd, WM_CLOSE, NULL, NULL );
				}
				else
				{

#endif

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
					if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_logout))
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3759), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"비무 중에는 종료할 수 없습니다."
						break;
					}
#endif

#ifdef _XTS_PK					
	#ifdef _XDEF_SHUTDOWN_20061027	
					// 셧다운 제도일 때 남은 시간이 없으면 PK대전 검사를 넘어간다.
					if( g_PlayLeftTime > 0 )
					{
						if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전이 종료되지 않아 접속 화면으로 돌아갈 수 없습니다."
							break;
						}
					}
	#else
					if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전이 종료되지 않아 접속 화면으로 돌아갈 수 없습니다."
						break;
					}
	#endif//_XDEF_SHUTDOWN_20061027	
#endif//_XTS_PK
			#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
				//Author : 양희왕 //breif : 클리어,
				_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
				if( pPKTracingRequest_Window )
				{
					memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));// 등록할 PK범 이름 
					memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
					pPKTracingRequest_Window->ShowWindow(FALSE);
				}
			#endif

				//Author : 양희왕 //brief : 버프 이벤트 때문에 아이콘 깜빡이는거 초기화
				_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
				pEvent_Window->m_bBuffEvent = FALSE;
				pEvent_Window->m_bFirstSetListItem = FALSE;

				if( !m_GameExitFlag )
				{				
					m_GameExitFlag = TRUE;
					m_GameExitSignalTime = 0;
				}
#ifdef _XDWDEBUG
				}
#endif
				_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
				if(pRebirth_Window)
				{
					if(pRebirth_Window->GetShowStatus())
						pRebirth_Window->ShowWindow(FALSE);
				}
				g_pLocalUser->ResetSkillAnimation();		// 전투 상태 초기화

				_XWindow_AgeGrade* pAgeGradeWindow = _XWindow_AgeGrade::GetInstance();
				if(pAgeGradeWindow)
					pAgeGradeWindow->Destroy();

				if(g_BattleLog.GetWriteLog())
					g_BattleLog.Release();
			}
			break;
		case _XDEF_CANCELGAMECLOSE					:
			m_GameExitFlag = FALSE;
			m_GameExitSignalTime = 0;
			//g_MessageBox.ShowWindow( FALSE );
			break;
		case _XDEF_GOTOLOGINSERVER : // 게임 종료중 취소를 누르면 로그아웃하고 로그인 창으로 이동
			{
			
	#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
				if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_logout))
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3760), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"비무 중에는 접속 화면으로 돌아갈 수 없습니다."
					break;
				}
	#endif
				
	#ifdef _XTS_PK
				if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2752), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전이 종료되지 않아 접속 화면으로 돌아갈 수 없습니다."
					break;
				}
	#endif
			#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
				//Author : 양희왕 //breif : 클리어,
				_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
				if( pPKTracingRequest_Window )
				{
					memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));// 등록할 PK범 이름 
					memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
					pPKTracingRequest_Window->ShowWindow(FALSE);
				}
			#endif

				//Author : 양희왕 //brief : 버프 이벤트 때문에 아이콘 깜빡이는거 초기화
				_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
				pEvent_Window->m_bBuffEvent = FALSE;
				pEvent_Window->m_bFirstSetListItem = FALSE;

				_XLog( "TEST LOG : _XDEF_GOTOLOGINSERVER in"); //Author : 양희왕
				
				TCHAR temp[1024];
				memset( temp, 0, sizeof(temp));

				_snprintf( temp, sizeof(temp), "%010x  check g_pTitleFlashObject", g_pTitleFlashObject );
				_XLog( temp ); //Author : 양희왕
				memset( temp, 0, sizeof(temp));

				// Save quickslot informations...
				{
					_XQuickSlotInfoList	quickslotinfolist;
					quickslotinfolist.LoadQuickSlotInfo();
					quickslotinfolist.UpdateInfoList();
					quickslotinfolist.SaveQuickSlotInfo();
				}

				_XLog( "TEST LOG : ResetData()"); //Author : 양희왕
				ResetData();//2004.06.19->oneway48 insert

				_snprintf( temp, sizeof(temp), "%010x  check g_pTitleFlashObject", g_pTitleFlashObject );
				_XLog( temp ); //Author : 양희왕
				memset( temp, 0, sizeof(temp));

				_XLog( "TEST LOG : ResetSkillAnimation()"); //Author : 양희왕
				g_pLocalUser->ResetSkillAnimation();		// 전투 상태 초기화

				_snprintf( temp, sizeof(temp), "%010x  check g_pTitleFlashObject", g_pTitleFlashObject );
				_XLog( temp ); //Author : 양희왕
				memset( temp, 0, sizeof(temp));

				_XLog( "TEST LOG : g_BattleLog()"); //Author : 양희왕
				if(g_BattleLog.GetWriteLog())
					g_BattleLog.Release();

				//g_NetworkKernel.SendMessengerPacket(MMSG_NO_LOGOUT_C_S);

				_snprintf( temp, sizeof(temp), "%010x  check g_pTitleFlashObject", g_pTitleFlashObject );
				_XLog( temp ); //Author : 양희왕
				memset( temp, 0, sizeof(temp));

				SetFocus( gHWnd );
				_XLog( "TEST LOG : ReleaseNetwork()"); //Author : 양희왕
				g_NetworkKernel.ReleaseNetwork();

				_snprintf( temp, sizeof(temp), "%010x  check g_pTitleFlashObject", g_pTitleFlashObject );
				_XLog( temp ); //Author : 양희왕
				memset( temp, 0, sizeof(temp));

				_XLog( "TEST LOG : ReleasePrivateData()"); //Author : 양희왕
				g_QuestScriptManager.ReleasePrivateData();
				

				_snprintf( temp, sizeof(temp), "%010x  check g_pTitleFlashObject", g_pTitleFlashObject );
				_XLog( temp ); //Author : 양희왕
				memset( temp, 0, sizeof(temp));

				m_bGotoLoginProcess = FALSE;//2004.06.29->oneway48 insert


				_XLog( "TEST LOG : ChangeProcess()"); //Author : 양희왕
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( 
							&((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
				
				g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_MESSAGEWINDOW);
				g_MessageBox.ShowWindow( FALSE );			

				

				_XLog( "TEST LOG : _XDEF_GOTOLOGINSERVER out"); //Author : 양희왕
			}
			break;
		case _XDEF_GOTOSELECTCHARACTER				:
			{

			// Save quickslot informations...
			{
				_XQuickSlotInfoList	quickslotinfolist;
				quickslotinfolist.LoadQuickSlotInfo();
				quickslotinfolist.UpdateInfoList();
				quickslotinfolist.SaveQuickSlotInfo();
			}

			ResetData();
			//g_pLocalUser->Reset(); // 현재 로컬 유저의 데이터를 초기화한다.
			
			if(g_BattleLog.GetWriteLog())
				g_BattleLog.Release();

			g_pLocalUser->ResetSkillAnimation();		// 전투 상태 초기화

		#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
			//Author : 양희왕 //breif : 클리어,
			_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
			if( pPKTracingRequest_Window )
			{
				memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));// 등록할 PK범 이름 
				memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
				pPKTracingRequest_Window->ShowWindow(FALSE);
			}
		#endif

			//Author : 양희왕 //brief : 버프 이벤트 때문에 아이콘 깜빡이는거 초기화
			_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
			pEvent_Window->m_bBuffEvent = FALSE;
			pEvent_Window->m_bFirstSetListItem = FALSE;

#ifdef _XDEF_PLAYER_CHARACTER_SELECT //Author : 양희왕
			g_NetworkKernel.SendPacket( MSG_NO_CHARAC_REQ_LIST );
#else
			g_NetworkKernel.SendPacket(MSG_NO_CHARAC_RETURN_LOBBY);
#endif

			memset( ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName, 0, sizeof(TCHAR) * 13 );
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter );
			g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_MESSAGEWINDOW);
			g_MessageBox.ShowWindow( FALSE );
			
			}
			break;

		case _XDEF_PROCESSTOPSIGNAL					:
			SendSignal_ProcessStop();
			break;

		case _XDEF_MEDITATION_YES					:		// hotblood
		case _XDEF_MEDITATION_NO					:
			MeditationMessageHandling(wparam, lparam);		
			break;
			
		case _XDEF_JINSTATUS_LEVELUPBUTTON			:
		case _XDEF_TRAINING_MEDITATION_START		:
		case _XDEF_TRAINING_MEDITATION_EXIT			:
		case _XDEF_TRAINING_DUMMY_START				:
		case _XDEF_TRAINING_HELP_CLOSE				:
		case _XDEF_TRAINING_CANCEL_OK				:
		case _XDEF_TRAINING_CANCEL_NO				:
		case _XDEF_TRAINING_SPELL_START				:
		case _XDEF_TRAINING_HELP_START				:
		case _XDEF_TRAINING_HELP_END				:
		case _XDEF_TRAINING_HELP_SKILL_1			:
		case _XDEF_TRAINING_HELP_SKILL_2			:
		case _XDEF_TRAINING_HELP_SKILL_3			:
		case _XDEF_TRAINING_HELP_SKILL_4			:
		case _XDEF_TRAINING_HELP_SKILL_5			:
		case _XDEF_TRAINING_HELP_SKILL_6			:
		case _XDEF_TRAINING_HELP_SKILL_7			:
		case _XDEF_TRAINING_HELP_SKILL_8			:
		case _XDEF_TRAINING_HELP_LEFT				:
		case _XDEF_TRAINING_HELP_RIGHT				:
		case _XDEF_JINSTATUS_ABILITYUPBUTTON		:
			TrainingMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_BOSSROOM_ENTER				:
		case _XDEF_BOSSROOM_CANCLE				:
		case _XDEF_BOSSROOM_ENTEROK				:
		case _XDEF_BOSSROOM_ASKPARTYMEMBER		:
		case _XDEF_BOSSROOM_USEITEMCANCEL		:
			EnterBossRoomMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_NICKNAME_APPLY				:
		case _XDEF_NICKNAME_NOTAPPLY					:		
		case _XDEF_NICKNAME_SPECIALNICKNAMELIST :
		case _XDEF_NICKNAME_GENERALNICKNAMEIST :
		case _XDEF_NICKNAME_UNIQUENICKNAMEIST :
		case _XDEF_NICKNAME_CLOSE :
		case _XDEF_NICKNAME_HELP :
		case _XDEF_NICKNAME_DELETE :
		case _XDEF_NICKNAME_DELETE_YES :
		case _XDEF_NICKNAME_RANKHIDING :
			NickNameMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_MATCH_REGISTRATION :
		case _XDEF_MATCH_REQUESTPVP :
		case _XDEF_MATCH_REQUESTPVPGROUP :
		case _XDEF_MATCH_REQUESTLIST :
		case _XDEF_PVPBROKER_CLOSE :
		case _XDEF_PVPBROKER_LISTBOX :
		case _XDEF_PVPBROKER_REQUESTPVP :
		case _XDEF_MATCH_REQUESTFROMOTHER_OK :
		case _XDEF_MATCH_REQUESTFROMOTHER_NO :
		case _XDEF_MATCH_WAITEXIT :
		case _XDEF_MATCH_REGISTCANCEL :
			MatchWindowMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_MATCHTABLEPVP_CLOSE :
		case _XDEF_MATCHTABLEPVP_MONEY :
		case _XDEF_MATCHTABLEPVP_CONFIRM :
		case _XDEF_MATCHTABLEPVP_CANCEL :
		case _XDEF_MATCHTABLEPVP_MATCHCLASSBTN :
		case _XDEF_MATCHTABLEPVP_MATCHTYPEBTN :
		case _XDEF_MATCHTABLEPVP_MATCHTIMEBTN :
		case _XDEF_MATCHTABLEPVP_USEHYPERRUNBTN :
		case _XDEF_MATCHTABLEPVP_USEBUFFBTN :
		case _XDEF_MATCHTABLEPVP_MATCHBALANCEBTN :
		case _XDEF_MATCHTABLEPVP_MATCHCLASSLBOX :
		case _XDEF_MATCHTABLEPVP_MATCHTYPELBOX :
		case _XDEF_MATCHTABLEPVP_MATCHTIMELBOX :
		case _XDEF_MATCHTABLEPVP_USEHYPERRUNLBOX :
		case _XDEF_MATCHTABLEPVP_USEBUFFLBOX :
		case _XDEF_MATCHTABLEPVP_MATCHBALANCELBOX :
		case _XDEF_MATCHTABLEPVP_MATCHBALANCESLIDER :
			MatchTablePVPWindowMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_MATCHNOTICE_CLOSE :
			MatchNoticeWindowMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_MATCHPR_CLOSEBUTTON :
		case _XDEF_MATCHPR_OPENBUTTON :
		case _XDEF_MATCHPR_HELPBUTTON :
			MatchPRWindowMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_MATCHRECORD_CLOSE :
		case _XDEF_MATCHINFO_TAB :
		case _XDEF_MATCHTABLEPVP_TAB :
			MatchRecordWindowMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_MASTERYSKILL_CLASS_1 :
		case _XDEF_MASTERYSKILL_CLASS_2 :
		case _XDEF_MASTERYSKILL_CLASS_3 :
		case _XDEF_MASTERYSKILL_CLASS_4 :
		case _XDEF_MASTERYSKILL_APPLYOK	:		
		case _XDEF_MASTERYSKILL_APPLYCANCLE :
			MasterySkillWindowMessageHandling(wparam, lparam);
			break;
		case _XDEF_GROUP_ORGANIZATION				:	
		case _XDEF_GROUP_ALIENCE_ALIENCETAB			:	
		case _XDEF_GROUP_ALIENCE_ALIENCEDESTROY		:
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER1	:	
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER2	:
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER3	:	
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER4	:
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER5	:
		case _XDEF_GROUP_ORGANIZATION_LEAVEBTN		:	
		case _XDEF_GROUP_ORGANIZATION_LEAVE_OK		:
		case _XDEF_GROUP_ORGANIZATION_LEAVE_NO		:				
		case _XDEF_GROUP_ORGANIZATION_LINELEVEL		:
		case _XDEF_GROUP_ORGANIZATION_LINEZONE		:
		case _XDEF_GROUP_ORGANIZATION_LINECONNECT	:
		case _XDEF_GROUP_ORGANIZATION_LISTBOX		:
		case _XDEF_GROUP_ORGANIZATION_CREATEYES		:
		case _XDEF_GROUP_ORGANIZATION_CREATENO		:
		case _XDEF_GROUP_ORGANIZATION_NAMECOMPLETE	:
		case _XDEF_GROUP_ORGANIZATION_NAMECANCLE	:
		case _XDEF_GROUP_ORGANIZATION_FRIEND		:
		case _XDEF_GROUP_ORGANIZATION_EMAIL			:
		case _XDEF_GROUP_ORGANIZATION_WHISPER		:
		case _XDEF_GROUP_ORGANIZATION_PARTY			:
		case _XDEF_GROUP_ORGANIZATION_GRANT			:
		case _XDEF_GROUP_ORGANIZATION_GRANTCANCLE	:
		case _XDEF_GROUP_ORGANIZATION_TRUST			:
		case _XDEF_GROUP_ORGANIZATION_PRUGE			:
		case _XDEF_GROUP_ORGANIZATION_INVITE_OK		:
		case _XDEF_GROUP_ORGANIZATION_INVITE_NO		:
		case _XDEF_GROUP_ORGANIZATION_ENTRUST_OK	:
		case _XDEF_GROUP_ORGANIZATION_ENTRUST_NO	:
		case _XDEF_GROUP_ORGANIZATION_SORTBTN		:
		case _XDEF_GROUP_ORGANIZATION_SORTLIST		:
		case _XDEF_GROUP_ORGANIZATION_NOTICEWRITE	:
		case _XDEF_GROUP_ORGANIZATION_NOTICEMODIFY	:
		case _XDEF_GROUP_ORGANIZATION_WRITEFINISH	:
		case _XDEF_GROUP_ORGANIZATION_WRITECANCLE	:
		case _XDEF_GROUP_ALIENCE_BTN_INVITE_OK		:
		case _XDEF_GROUP_ALIENCE_BTN_INVITE_NO		:
		case _XDEF_GROUP_CASTLEBATTLE_CASTLEBATTLETAB:
		case _XDEF_GROUP_CASTLEBATTLE_BTN_TIMECHANGE:
		case _XDEF_GROUP_CASTLEBATTLE_BTN_GETMONEY	:
		case _XDEF_GROUP_CASTLEBATTLE_WARSITUATIONTAB:
		case _XDEF_GROUP_ALIENCE_BTN_GOTOORGANIZATION	:
		case _XDEF_GROUP_ALIENCE_BTN_GOTOALIENCE:
		case _XDEF_GROUP_MASTERNDISCIPLETAB :  //Author : 양희왕 //breif : 사제
		case _XDEF_GROUP_MASTERNDISCIPLE_NOTICEWRITE :  //Author : 양희왕 //breif : 사제
		case _XDEF_GROUP_MASTERNDISCIPLE_NOTICEMODIFY : //Author : 양희왕 //breif : 사제
		case _XDEF_GROUP_MASTERNDISCIPLE_WRITEFINISH: //Author : 양희왕 //breif : 사제
		case _XDEF_GROUP_MASTERNDISCIPLE_WRITECANCLE: //Author : 양희왕 //breif : 사제
			OrganizationWindowMessageHandling(wparam, lparam);
			break;
		case _XDEF_GAMBLE_OPEN :
		case _XDEF_GAMBLE_CANCLE :
		case _XDEF_GAMBLE_HELP :
		case _XDEF_GAMBLE_CLOSE :
		case _XDEF_GAMBLE_STOPOK :		
		case _XDEF_GAMBLE_STOPNO :
		case _XDEF_GAMBLE_OPENOK :
		case _XDEF_GAMBLE_OPENNO :		
			GambleBoxWindowMessageHandling(wparam, lparam);
			break;
		case _XDEF_EVENTNOTICE_CLOSE :	// 이벤트 공지 관련 
			{
				_XWindow_EventNotice* pEventNotice_Window = (_XWindow_EventNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTNOTICEWINDOW);
				if( pEventNotice_Window )
					pEventNotice_Window->ShowWindow(FALSE);
			}
			break;
		case _XDEF_EVENTNOTICE_CHECK :	// 이벤트 공지 관련 : 오늘 다시 안봄
			{
				_XWindow_EventNotice* pEventNotice_Window = (_XWindow_EventNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTNOTICEWINDOW);
				if( pEventNotice_Window )
				{
					SaveEventNoticeInfo();
					pEventNotice_Window->ShowWindow(FALSE);
				}
			}
			break;
		case _XDEF_CASHITEM_REFRESH_BTN		:
		case _XDEF_CASHITEM_ALLMOVE_BTN		:
		case _XDEF_CASHITEM_BUYITEM_BTN		:
		case _XDEF_CASHITEM_PRESENTITEM_BTN	:
		case _XDEF_CASHITEM_CLOSE_BTN		:
		case _XDEF_CASHITEM_HELPE_BTN		:
		case _XDEF_CASHITEM_ALLMOVEYES_BTN	:
			CashItemWindowMessageHandling(wparam, lparam);
			break;
			
		case _XDEF_CASHITEMUSE_SHOUTYES		:
		case _XDEF_CASHITEMUSE_SHOUTNO		:
		case _XDEF_CASHITEMUSE_RESETPOINTYES :
		case _XDEF_CASHITEMUSE_RESETPOINTNO	:
		case _XDEF_CASHITEMUSE_ADDEXPYES	:
		case _XDEF_CASHITEMUSE_ADDEXPNO		:
		case _XDEF_CASHITEMUSE_CLEARNICKNAMEYES :
		case _XDEF_CASHITEMUSE_CLEARNICKNAMENO :
			CashItemUseMesssageHandling(wparam, lparam);
			break;
		case _XDEF_NPCCOLLECTION_HELP_BTN			:
		case _XDEF_NPCCOLLECTION_REWARDSET01_BTN	:
		case _XDEF_NPCCOLLECTION_REWARDSET02_BTN	:
		case _XDEF_NPCCOLLECTION_REWARDSET03_BTN	:
		case _XDEF_NPCCOLLECTION_REWARDSET04_BTN	:
		case _XDEF_NPCCOLLECTION_COLLECTIONLISTBOX	:
		case _XDEF_NPCCOLLECTION_SELECTREWARD_OK	:
		case _XDEF_NPCCOLLECTION_SELECTREWARD_NO	:
			NPCCollectionWindowMessageHandling(wparam, lparam);
			break;
		case _XDEF_NPCRESOURCE_HELP_BTN				:
		case _XDEF_NPCRESOURCE_EXCHANGE_BTN			:
		case _XDEF_NPCRESOURCE_RESOURCELISTBOX		:
		case _XDEF_NPCRESOURCE_EXCHANGE_OK			:
		case _XDEF_NPCRESOURCE_EXCHANGE_NO			:
			NPCResourceWindowMessageHandling(wparam, lparam);
			break;

		case _XDEF_NEWRESOURCEWINDOW_RESOURCELIST	:
		case _XDEF_NEWRESOURCEWINDOW_EXCHANGE		:
		case _XDEF_NEWRESOURCEWINDOW_CLOSE			:
		case _XDEF_NEWRESOURCEWINDOW_EXCHANGE_OK	:
		case _XDEF_NEWRESOURCEWINDOW_EXCHANGE_NO	:
			NewNPCResourceWindowMessageHandling(wparam, lparam);
			break;

		case _XDEF_INCHANT_STARTBUTTON		:
		case _XDEF_INCHANT_CANCLEBUTTON		:
		case _XDEF_INCHANT_HELPBUTTON			:
		case _XDEF_INCHANT_STARTYESBUTTON		:
		case _XDEF_INCHANT_STARTNOBUTTON		:
		case _XDEF_INCHANT_PERIODITEM_YES		:
		case _XDEF_INCHANT_PERIODITEM_NO		:
			InchantWindowMessageHandling(wparam, lparam);
			break;
		case _XDEF_SMELT_CONFIRMBUTTON :
		case _XDEF_SMELT_CANCLEBUTTON :
		case _XDEF_SMELT_CLOSEBUTTON :
		case _XDEF_SMELT_PERIODITEM_YES :
		case _XDEF_SMELT_PERIODITEM_NO :
			SmeltItemWindowMessageHandling(wparam, lparam); //Author : 양희왕
			break;
		case _XDEF_NPCBUFF_CLOSE :
		case _XDEF_NPCBUFF_LISTBOX :
		case _XDEF_NPCBUFF_APPLY :
		case _XDEF_NPCBUFF_APPLY_OK :
			NPCBuffWindowMessageHandling(wparam, lparam);
			break;
		case _XDEF_BOOKOPEN_CLOSE :
			{
				_XWindow_BookOpen* pBookOpen_Window = (_XWindow_BookOpen*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BOOKOPENWINDOW);
				if( pBookOpen_Window )
				{
					if( pBookOpen_Window->GetShowStatus() )
						pBookOpen_Window->ShowWindow(FALSE);
				}
			}
			break;
			
#ifdef _XTS_ITEMMALLBROWSER
		case _XDEF_CASHMALL_BTN_CLOSE			:
		case _XDEF_CASHBOX_BTN_CLOSE			:
		case _XDEF_CASHBOX_BTN_RENEW			:
		case _XDEF_CASHBOX_BTN_MOVE				:
		case _XDEF_CASHBOX_BTN_ALLMOVE			:
		case _XDEF_CASHBOX_BTN_MOVECASHMALL		:
		case _XDEF_CASHBOX_LISTBOX_CASHITEM		:
		case _XDEF_CASHBOX_TABBUTTON_PERSONAL	:
		case _XDEF_CASHBOX_TABBUTTON_PRESENT	:
		case _XDEF_CASHBOX_BTN_FIRST			:
		case _XDEF_CASHBOX_BTN_END				:
		case _XDEF_CASHBOX_BTN_ADJACENT			:
		case _XDEF_CASHBOX_BTN_NEXT				:
		case _XDEF_CASHBOX_BTN_PAGE1			:
		case _XDEF_CASHBOX_BTN_PAGE2			:
		case _XDEF_CASHBOX_BTN_PAGE3			:
		case _XDEF_CASHBOX_BTN_PAGE4			:
		case _XDEF_CASHBOX_BTN_PAGE5			:
		case _XDEF_CASHBOX_BTN_MSG1				:
		case _XDEF_CASHBOX_BTN_MSG2				:
		case _XDEF_CASHBOX_BTN_MSG3				:
		case _XDEF_CASHBOX_BTN_MSG4				:
		case _XDEF_CASHBOX_BTN_MSG5				:
		case _XDEF_CASHBOX_BTN_MSG6				:
		case _XDEF_CASHBOX_BTN_MSG7				:
		case _XDEF_CASHBOX_BTN_MSG8				:
		case _XDEF_CASHBOX_BTN_MSG9				:		
		case _XDEF_CASHBOX_BTN_MSGCLOSE			:			
			CashItemBoxWindowMessageHandling(wparam, lparam);
			break;			
#endif

		case _XDEF_STATEWINDOW_STATECANCLEYES :
			{
				if( g_pState_Window )
					g_pState_Window->ClickSkillAction();
			}
			break;

#ifdef _XTS_BLOODPOINT
		case _XDEF_BLOODPOINT_REQITEM :
			{
				if(g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE || g_pLocalUser->m_CharacterLog.isTrade) //노점 개설중,교환중
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2730 ), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"행낭이 사용 중이면 혈정을 획득할 수 없습니다."
					break;
				}

				_XGameItem pItem;
				pItem.m_cType = _XGI_FC_LIFE;
				pItem.m_cSecond = _XGI_SC_LIFE_INCHANTSTUFF;
				pItem.m_sID = 19;

				int slotnumber = g_pLocalUser->GetEmptyInventorySlot(&pItem);
				if(slotnumber != -1)
				{
					g_NetworkKernel.SendPacket(MSG_NO_BLOODPOINT_CTRL, en_bppk_mode_request_item, slotnumber);
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAIGAME_NOTEMPTY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
			}
			break;
#endif
#ifdef _XTS_NEWSKILLWINDOW
		case _XDEF_WTITLE_DT_DELECTSKILLBTN :
			{
				if( g_pDefaultTooltip_Window )
				{
					g_pDefaultTooltip_Window->ShowWindow(FALSE);					

					_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_DELETESKILLMESSAGEWINDOW);
					if( !pMessageBox )
					{
						g_pDefaultTooltip_Window->SetDeleteSkillId();
						TCHAR tempBuf[128];
						memset(tempBuf, 0, sizeof(tempBuf));
						_snprintf( tempBuf, sizeof(tempBuf), _XGETINTERFACETEXT(ID_STRING_NEW_2731), g_SkillProperty[g_pDefaultTooltip_Window->GetDeleteSkillID()]->skillName ); //"%s 무공을 영구 삭제하시겠습니까?"
						
						pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_DELETESKILLMESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tempBuf, TRUE, _XDEF_WTITLE_DT_DELECTSKILLSTEP1YES, 0);
						pMessageBox->StartTimer(20000, TRUE);
					}					
				}
			}
			break;
		case _XDEF_WTITLE_DT_DELECTSKILLSTEP1YES :
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_DELETESKILLMESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT( ID_STRING_NEW_2732 ), TRUE, _XDEF_WTITLE_DT_DELECTSKILLSTEP2YES, 0); //_T("삭제되는 무공은 어떠한 경우에도 복구되지 않습니다. 정말 삭제하시겠습니까?")
				pMessageBox->StartTimer(20000, TRUE);
			}
			break;
		case _XDEF_WTITLE_DT_DELECTSKILLSTEP2YES :
			{
				if( g_pDefaultTooltip_Window )
				{
					TCHAR tempBuf[128];
					memset(tempBuf, 0, sizeof(tempBuf));
					_snprintf( tempBuf, sizeof(tempBuf), _XGETINTERFACETEXT( ID_STRING_NEW_2733 ), g_SkillProperty[g_pDefaultTooltip_Window->GetDeleteSkillID()]->skillName ); //"%s 무공을 영구 삭제합니다."
					
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_DELETESKILLMESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tempBuf, TRUE, _XDEF_WTITLE_DT_DELECTSKILLSTEP3YES, 0);
					pMessageBox->StartTimer(20000, TRUE);
				}
			}
			break;
		case _XDEF_WTITLE_DT_DELECTSKILLSTEP3YES :
			{
				if( g_pDefaultTooltip_Window )
				{
					g_NetworkKernel.SendPacket(MSG_NO_SKILL_CTRL_REQ, 5, g_pDefaultTooltip_Window->GetDeleteSkillID());
				}				
			}
			break;
#endif
			
#ifdef _XDEF_WEBBOARD_20061211
		case _XDEF_WEBBOARD_BTN_CLOSE:
			{
				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				if( pWebBoard_Window  )
				{
					pWebBoard_Window->ShowWindow(FALSE);
					ShowWindow(g_hWebBoardWindow, SW_HIDE );
					g_bShowItemMallWindow = FALSE;					
				}
			}
			break;
#endif
		case _XDEF_MOVEVILLAGE_MOVEBUTTON :
		case _XDEF_MOVEVILLAGE_LISTBOX :
		case _XDEF_MOVEVILLAGE_DECIDEMOVING :
		case _XDEF_MOVEVILLAGE_CANCEL :
			{
				MoveVillageWindowMessageHandling(wparam, lparam);
			}
			break;
			
#ifdef _XDEF_ESCAPE_20070205
		case _XDEF_ESCAPE_BTN_CANCLE:
			{
				_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
				if(pEscape_Window)
				{					
					pEscape_Window->ShowWindow(FALSE);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3243), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					
				}
			}
			break;
#endif
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		case _XDEF_BLACKLISTWINDOW_CLOSEBUTTON				:
		case _XDEF_BLACKLISTWINDOW_HELPBUTTON				:
		case _XDEF_MESSENGERWINDOW_ADDBLACKLISTBUTTON		:
		case _XDEF_MESSENGERWINDOW_DELETEBLACKLISTBUTTON	:
		case _XDEF_MESSENGERWINDOW_BLACKLISTBOX				:			
		case _XDEF_MESSENGERWINDOW_ADD_MESSAGEBOX			:		
		case _XDEF_MESSENGERWINDOW_DELETE_MESSAGEBOX		:
			{
				BlackListWindowMessageHandling(wparam, lparam);
			}
			break;
#endif
		//Author : 양희왕 //breif : 신구룡전도
		case _XDEF_WORLDMAP_STARTBUTTON :
		case _XDEF_WORLDMAP_CLOSEBUTTON :
		case _XDEF_WORLDMAP_MAPBUTTON_01 :
		case _XDEF_WORLDMAP_MAPBUTTON_02 :
		case _XDEF_WORLDMAP_MAPBUTTON_03 :
		case _XDEF_WORLDMAP_MAPBUTTON_04 :
		case _XDEF_WORLDMAP_MAPBUTTON_05 :
		case _XDEF_WORLDMAP_MAPBUTTON_06 :
		case _XDEF_WORLDMAP_MAPBUTTON_07 :
		case _XDEF_WORLDMAP_MAPBUTTON_08 :
		case _XDEF_WORLDMAP_MAPBUTTON_09 :
		case _XDEF_WORLDMAP_MAPBUTTON_10 :
		case _XDEF_WORLDMAP_MAPBUTTON_11 :
		case _XDEF_WORLDMAP_MAPBUTTON_12 :
		case _XDEF_WORLDMAP_MAPBUTTON_13 :
		case _XDEF_WORLDMAP_MAPBUTTON_14 :
		case _XDEF_WORLDMAP_MAPBUTTON_15 :
		case _XDEF_WORLDMAP_MAPBUTTON_16 :
		case _XDEF_WORLDMAP_MAPBUTTON_17 :
		case _XDEF_WORLDMAP_MAPBUTTON_18 :
		case _XDEF_WORLDMAP_MAPBUTTON_19 :
			NewWorldMapWindowMessageHandling( wparam, lparam );
			break;
		//Author : 양희왕 //breif : 친구소환 관련
		case _XDEF_RECALLITEM_CALLBUTTON :
		case _XDEF_RECALLITEM_MOVEBUTTON :
		case _XDEF_RECALLITEM_CLOSEBUTTON :
		case _XDEF_RECALLITEM_LISTBOX :
		case _XDEF_RECALLITEM_MINICLOSEBUTTON:
		case _XDEF_RECALLITEM_CALLBUTTON_YES :
		case _XDEF_RECALLITEM_CALL_AGREE :
		case _XDEF_RECALLITEM_CALL_REJECT :
		case _XDEF_RECALLITEM_MOVEBUTTON_YES :
		case _XDEF_RECALLITEM_MOVE_AGREE :
		case _XDEF_RECALLITEM_MOVE_REJECT :
			RecallItemWIndowMessageHandling( wparam, lparam );
			break;

		//Author : 양희왕 //breif : 소켓제거
		case _XDEF_SOCKETDETACH_PROCESSBUTTON :
		case _XDEF_SOCKETDETACH_YESBUTTON :
		case _XDEF_SOCKETDETACH_CANCEL :
		case _XDEF_SOCKETDETACH_CLOSE :
		case _XDEF_SOCKETDETACH_HELP :
			SocketDetachWIndowMessageHandling( wparam, lparam );
			break;

		default:
			break;
		}
	}	
}

bool XProc_MainGame::OnKeyboardPoll(_XInput *pInput)
{
	BOOL keypressed = FALSE;
	FLOAT direction = 0.0f;
		
	// NPC View에서는 경공 사용 불가 Functional Object View에서는 경공 사용 불가
	if( m_bNPCDialogMode || m_bNPCDialogWideView || m_bFunctionalObjectWideView) return true;
	
	
	//if( !_XIMEKERNEL.GetIMEContainer() ) // IME가 활성화 되있지 않을때만 경공 사용 
	{
// 경공 모드 
#ifdef _XADMINISTRATORMODE
/*		if( gpInput->CheckKeyPress(DIK_LMENU) )
		{		
			if(g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE 
				&& g_pLocalUser->GetMotionClass() != _XACTION_REBIRTH)
			{
				direction = 25.0f;
				g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position + g_LodTerrain.m_3PCamera.m_LookVector;			
				keypressed = TRUE;
			}
		}*/
#endif		
		/*if(pInput->CheckKeyPress(DIK_S))
		{		
			direction = -25.0f;
			g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position - g_LodTerrain.m_3PCamera.m_LookVector;			
			keypressed = TRUE;
		}*/
		
		if(g_pLocalUser->m_Lock.camerarotatelock == 1)
			return true;
	
		#define _XDEF_KEY_CAMERAROTATESPEED	2.5f
		
		if(pInput->CheckKeyPress(DIK_LEFT))
		{
			if(g_pLocalUser->m_Lock.camerareverse == 1)
				g_LodTerrain.m_3PCamera.AddYaw( _XDEF_KEY_CAMERAROTATESPEED );
			else
				g_LodTerrain.m_3PCamera.AddYaw( -_XDEF_KEY_CAMERAROTATESPEED );

			keypressed = TRUE;
			if( g_pMinimap_Window )
			{
				// Set direction to minimap arrow 
				g_pMinimap_Window->SetRotateFrustum( _X_RAD( 180.0f - g_LodTerrain.m_3PCamera.GetYaw() ) );
			}
		}
		
		if(pInput->CheckKeyPress(DIK_RIGHT))
		{
			if(g_pLocalUser->m_Lock.camerareverse == 1)
				g_LodTerrain.m_3PCamera.AddYaw( -_XDEF_KEY_CAMERAROTATESPEED );
			else
				g_LodTerrain.m_3PCamera.AddYaw( _XDEF_KEY_CAMERAROTATESPEED );

			keypressed = TRUE;
			if( g_pMinimap_Window )
			{
				// Set direction to minimap arrow 
				g_pMinimap_Window->SetRotateFrustum( _X_RAD( 180.0f - g_LodTerrain.m_3PCamera.GetYaw() ) );
			}
			
		}
		
		if(pInput->CheckKeyPress(DIK_UP))
		{
			if(g_pLocalUser->m_Lock.camerareverse == 1)
				g_LodTerrain.m_3PCamera.AddPitch( _XDEF_KEY_CAMERAROTATESPEED );
			else
				g_LodTerrain.m_3PCamera.AddPitch( -_XDEF_KEY_CAMERAROTATESPEED );

			keypressed = TRUE;			
		}

		if(pInput->CheckKeyPress(DIK_DOWN))
		{
			if(g_pLocalUser->m_Lock.camerareverse == 1)
				g_LodTerrain.m_3PCamera.AddPitch( -_XDEF_KEY_CAMERAROTATESPEED );
			else
				g_LodTerrain.m_3PCamera.AddPitch( _XDEF_KEY_CAMERAROTATESPEED );

			keypressed = TRUE;						
		}
		/*
		//2004.06.28->oneway48 insert
		if(pInput->CheckKeyPress(DIK_LEFT))
		{
			g_LodTerrain.m_3PCamera.AddYaw( -_XDEF_KEY_CAMERAROTATESPEED );
			keypressed = TRUE;						
		}
		
		if(pInput->CheckKeyPress(DIK_RIGHT))
		{
			g_LodTerrain.m_3PCamera.AddYaw( _XDEF_KEY_CAMERAROTATESPEED );
			keypressed = TRUE;						
		}
		*/
	}


	// keyboard move 
	if( direction != 0.0f )
	{
		g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * direction);		
				
		//if( g_pLocalUser->GetMotionClass()	!= _XACTION_MOVE ) g_pLocalUser->SetAnimation( _XACTION_MOVE );
		if( g_pLocalUser->m_TargetPosition.x > 0.0f )
		{
			if( g_pLocalUser->m_TargetPosition.x > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth) )
			{
				g_pLocalUser->m_TargetPosition.x = (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth);
			}
		}
		else
		{
			if( g_pLocalUser->m_TargetPosition.x < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth) )
			{
				g_pLocalUser->m_TargetPosition.x = (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth);
			}
		}
		
		if( g_pLocalUser->m_TargetPosition.z > 0.0f )
		{
			if( g_pLocalUser->m_TargetPosition.z > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight) )
			{
				g_pLocalUser->m_TargetPosition.z = (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight);
			}
		}
		else
		{
			if( g_pLocalUser->m_TargetPosition.z < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight) )
			{
				g_pLocalUser->m_TargetPosition.z = (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight);
			}
		}
	}	
	
	if( keypressed )
	{		
		//g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position, &g_pLocalUser->m_CurrentPositionTerrainNormal );
		g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
														   g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
														   g_pLocalUser->m_Position.z ) );
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.RebuildLevel();
						
		if( g_pMinimap_Window )
		{			
			g_pMinimap_Window->SetLocalUserPosition();
		}
	}
	
	return true; 
}

bool XProc_MainGame::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
		g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
	{
		//인사나 박수 중일때는 이동 불가 
		// 경고 메세지?
		// 무기 삭제된 상태에서는 다시 장착하기 전까지 다른 행동을 할 수 없다.
		return true;
	}
	
#ifdef _XDEF_KEYCHANGE_20070115
	if( g_bGameOptionKeyTypeASelected )
#endif
	{
		//2004.06.29->oneway48 insert
		_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
		if( m_ChatEditContainer.GetShowStatus() ) return true;

		DWORD scancode = (HIWORD(lparam) & 0xFF);

		int index;
		switch(scancode)
		{
		case DIK_1 :
		case DIK_2 :
		case DIK_3 :
		case DIK_4 :
		case DIK_5 :
		case DIK_6 :
		case DIK_7 :
		case DIK_8 :
		case DIK_9 :
		case DIK_0 :
			{
				
#ifdef _XDEF_ESCAPE_20070205
				// 탈출 중에는 무공, 포션 키를 사용할 수 없다.
				_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
				if(pEscape_Window)
				{
					if(pEscape_Window->GetShowStatus())	return true;
				}
#endif

				if(_XIMEKERNEL.GetIMEContainer())
					return true;

				if(g_pLocalUser->m_bMatchPRMode)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2407), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return true;
				}

#ifdef _XDEF_CASTLEBATTLE
				if(g_pLocalUser->m_bThreatening)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3355), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"장주 협박 중에는 사용할 수 없습니다."
					return true;
				}
#endif
				
				if(g_pLocalUser->GetMotionClass() == _XACTION_DYING || g_pLocalUser->GetMotionClass() == _XACTION_DIE)
				{
					// 빈사, 혼수 상태에서는 아이템 사용할 수 없음
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//지금은 사용할 수 없습니다
					return true;
				}

				//Author : 양희왕 //breif : 노점 상태일 때 아이템 사용 못하게
				if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return true;
				}

				if(g_pQuickSlot_Window)
				{
					if( gpInput->CheckKeyPress(DIK_LSHIFT) || gpInput->CheckKeyPress(DIK_RSHIFT) )
					{
						if( scancode == DIK_1 || scancode == DIK_2 || scancode == DIK_3 )
						{
							if( !m_ChatEditContainer.GetShowStatus() || !m_ChatEditContainer.GetFocus() )					
							{
								_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );				
								if( pChatDefault_Window )
								{
									m_ChatEditContainer.ShowWindow( TRUE );
									m_ChatEditContainer.SetFocus();
									
									pChatDefault_Window->m_MovingChatWindow = TRUE;
									pChatDefault_Window->SetDrawBorder(TRUE);
									
									switch(scancode)
									{
									case DIK_1 :
										pChatDefault_Window->SetChatMode( _XCHATMODE_PARTY );							
										break;
									case DIK_2 :
										pChatDefault_Window->SetChatMode( _XCHATMODE_SUBCLAN );
										break;
									case DIK_3 :
										pChatDefault_Window->SetChatMode( _XCHATMODE_CLAN );
										break;
									}
								}
							}
							return true;
						}
					}
					switch(scancode)
					{
					case DIK_1 :
						index = 0;
						break;
					case DIK_2 :
						index = 1;
						break;
					case DIK_3 :
						index = 2;
						break;
					case DIK_4 :
						index = 3;
						break;
					case DIK_5 :
						index = 4;
						break;
					case DIK_6 :
						index = 5;
						break;
					case DIK_7 :
						index = 6;
						break;
					case DIK_8 :
						index = 7;
						break;
					case DIK_9 :
						index = 8;
						break;
					case DIK_0 :
						index = 9;
						break;
					}

					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);
					_XPlayInterfaceSound(ID_SR_INTERFACE_QUICK_USE_WAV);

					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 처리
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(g_pLocalUser->m_SelectedSkillID == skillid)
						{
							if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHARGESKILL)
							{
								// charge skill
								switch(g_pLocalUser->m_CurrentBattleState)
								{
								case _XUSER_STATE_1 :
									{
										if(!g_pLocalUser->m_bStartCharge)
											g_pLocalUser->ResetSkillAnimation();
									}
									break;
								case _XUSER_STATE_2 :
									{
										if(!g_pLocalUser->m_bStartCharge)
										{
											if(g_pLocalUser->m_ChargeSendStartPacket)
												g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
											else
												g_pLocalUser->ResetSkillAnimation();
										}
									}
									break;
								case _XUSER_STATE_3 :
								case _XUSER_STATE_4 :
								case _XUSER_STATE_5 :
									{
										if(!g_pLocalUser->m_bStartCharge)
											g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
									}
									break;
								case _XUSER_STATE_6 :
									g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_7;
									break;
								}
							}
						}
					} // if(type == _XQS_SLOTTYPE_SKILL)
					else if(type == _XQS_SLOTTYPE_ITEM)
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    	
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//_T("기간제 행낭의 사용기간이 만료되어 아이템을 사용할 수 없습니다.")
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						g_pLocalUser->UseItem(invennumber);
					}
					else if( type == _XQS_SLOTTYPE_LIFESKILL )
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						
						int selectindex = g_pQuickSlot_Window->GetSelectedSet()*10 + index;
						if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 1 ) // 노점 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_USEBUTTON, 0), (LPARAM)gHWnd); 
						}
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 2 ) // 비무홍보 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_PVPPRBUTTON, 0), (LPARAM)gHWnd); 
						}					
#ifdef _XDEF_EVENTWINDOW
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 3 ) // 이벤트 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_EVENTBUTTON, 0), (LPARAM)gHWnd); 
						}	
#endif
						
						
					}
				}
			}
			return true;
		case DIK_A :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					
				}
				else
				{			
					if(_XIMEKERNEL.GetIMEContainer())
						return true;
				}
			}
			return true;
		case DIK_P : // 운기 버튼 
			{
				if(_XIMEKERNEL.GetIMEContainer())
					return true;

				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_MEDBUTTON, 0), (LPARAM)gHWnd);
			}
			return true;

		case DIK_R :
			{
				_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );				
				if( pChatDefault_Window )
				{
					if( strlen(pChatDefault_Window->m_SavedWhisperTargetName) )
					{
						if( _XIMEKERNEL.GetIMEContainer() == NULL )
						{			
							m_ChatEditContainer.ShowWindow( TRUE );
							m_ChatEditContainer.SetFocus();
							
							pChatDefault_Window->m_MovingChatWindow = TRUE;
							pChatDefault_Window->SetDrawBorder(TRUE);
							
							pChatDefault_Window->InsertRecentWhisperUserList(pChatDefault_Window->m_SavedWhisperTargetName, 3);
							strcpy( pChatDefault_Window->m_WhisperTargetName, pChatDefault_Window->m_SavedWhisperTargetName );
							pChatDefault_Window->SetChatMode( _XCHATMODE_WHISPER );				
						}
					}
				}
			}
			return true;
		case DIK_SLASH :
			{
				if( _XIMEKERNEL.GetIMEContainer() == NULL )
				{
					m_ChatEditContainer.ShowWindow( TRUE );
					m_ChatEditContainer.SetFocus();
						
					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );				
					if( pChatDefault_Window )
					{
						pChatDefault_Window->m_MovingChatWindow = TRUE;
						pChatDefault_Window->SetDrawBorder(TRUE);
						
						strcpy( pChatDefault_Window->m_WhisperTargetName, _T("") );
						pChatDefault_Window->SetChatMode( _XCHATMODE_WHISPER );
					}
				}
			}
			return true;
		}	
		return false;

	}
#ifdef _XDEF_KEYCHANGE_20070115
	else
	{
		switch( (U32)wparam ) 
		{
		case VK_F10		:
			{
				
#ifdef _XDEF_ESCAPE_20070205
				// 탈출 중에는 무공, 포션 키를 사용할 수 없다.
				_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
				if(pEscape_Window)
				{
					if(pEscape_Window->GetShowStatus())	return true;
				}
#endif
				if(_XIMEKERNEL.GetIMEContainer())
					return true;

				if(g_pLocalUser->m_bMatchPRMode)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2407), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return true;
				}

				if(g_pLocalUser->GetMotionClass() == _XACTION_DYING || g_pLocalUser->GetMotionClass() == _XACTION_DIE)
				{
					// 빈사, 혼수 상태에서는 아이템 사용할 수 없음
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//지금은 사용할 수 없습니다
					return true;
				}

				if(g_pQuickSlot_Window)
				{
					int index = 9;
					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);
					_XPlayInterfaceSound(ID_SR_INTERFACE_QUICK_USE_WAV);

					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 처리
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(g_pLocalUser->m_SelectedSkillID == skillid)
						{
							if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHARGESKILL)
							{
								// charge skill
								switch(g_pLocalUser->m_CurrentBattleState)
								{
								case _XUSER_STATE_1 :
									{
										if(!g_pLocalUser->m_bStartCharge)
											g_pLocalUser->ResetSkillAnimation();
									}
									break;
								case _XUSER_STATE_2 :
									{
										if(!g_pLocalUser->m_bStartCharge)
										{
											if(g_pLocalUser->m_ChargeSendStartPacket)
												g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
											else
												g_pLocalUser->ResetSkillAnimation();
										}
									}
									break;
								case _XUSER_STATE_3 :
								case _XUSER_STATE_4 :
								case _XUSER_STATE_5 :
									{
										if(!g_pLocalUser->m_bStartCharge)
											g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
									}
									break;
								case _XUSER_STATE_6 :
									g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_7;
									break;
								}
							}
						}
					} // if(type == _XQS_SLOTTYPE_SKILL)
					else if(type == _XQS_SLOTTYPE_ITEM)
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						g_pLocalUser->UseItem(invennumber);
					}
					else if( type == _XQS_SLOTTYPE_LIFESKILL )
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						
						int selectindex = g_pQuickSlot_Window->GetSelectedSet()*10 + index;
						if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 1 ) // 노점 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_USEBUTTON, 0), (LPARAM)gHWnd); 
						}
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 2 ) // 비무홍보 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_PVPPRBUTTON, 0), (LPARAM)gHWnd); 
						}					
#ifdef _XDEF_EVENTWINDOW
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 3 ) // 이벤트 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_EVENTBUTTON, 0), (LPARAM)gHWnd); 
						}	
#endif
						
						
					}
				}
			}
			return true;

				
		}

				//2004.06.29->oneway48 insert
		_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
		if( m_ChatEditContainer.GetShowStatus() ) return true;

		DWORD scancode = (HIWORD(lparam) & 0xFF);

		int index;
		switch(scancode)
		{
		case DIK_F1 :
		case DIK_F2 :
		case DIK_F3 :
		case DIK_F4 :
		case DIK_F5 :
		case DIK_F6 :
		case DIK_F7 :
		case DIK_F8 :
		case DIK_F9 :
		case DIK_F10 :
			{
				
#ifdef _XDEF_ESCAPE_20070205
				// 탈출 중에는 무공, 포션 키를 사용할 수 없다.
				_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
				if(pEscape_Window)
				{
					if(pEscape_Window->GetShowStatus())	return true;
				}
#endif
				if(_XIMEKERNEL.GetIMEContainer())
					return true;

				if(g_pLocalUser->m_bMatchPRMode)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2407), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return true;
				}

				if(g_pLocalUser->GetMotionClass() == _XACTION_DYING || g_pLocalUser->GetMotionClass() == _XACTION_DIE)
				{
					// 빈사, 혼수 상태에서는 아이템 사용할 수 없음
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//지금은 사용할 수 없습니다
					return true;
				}

				if(g_pQuickSlot_Window)
				{
					if( gpInput->CheckKeyPress(DIK_LSHIFT) || gpInput->CheckKeyPress(DIK_RSHIFT) )
					{
						if( scancode == DIK_F1 || scancode == DIK_F2 || scancode == DIK_F3 )
						{
							if( !m_ChatEditContainer.GetShowStatus() || !m_ChatEditContainer.GetFocus() )					
							{
								_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );				
								if( pChatDefault_Window )
								{
									m_ChatEditContainer.ShowWindow( TRUE );
									m_ChatEditContainer.SetFocus();
									
									pChatDefault_Window->m_MovingChatWindow = TRUE;
									pChatDefault_Window->SetDrawBorder(TRUE);
									
									switch(scancode)
									{
									case DIK_F1 :
										pChatDefault_Window->SetChatMode( _XCHATMODE_PARTY );							
										break;
									case DIK_F2 :
										pChatDefault_Window->SetChatMode( _XCHATMODE_SUBCLAN );
										break;
									case DIK_F3 :
										pChatDefault_Window->SetChatMode( _XCHATMODE_CLAN );
										break;
									}
								}
							}
							return true;
						}
					}
					switch(scancode)
					{
					case DIK_F1 :
						index = 0;
						break;
					case DIK_F2 :
						index = 1;
						break;
					case DIK_F3 :
						index = 2;
						break;
					case DIK_F4 :
						index = 3;
						break;
					case DIK_F5 :
						index = 4;
						break;
					case DIK_F6 :
						index = 5;
						break;
					case DIK_F7 :
						index = 6;
						break;
					case DIK_F8 :
						index = 7;
						break;
					case DIK_F9 :
						index = 8;
						break;
					case DIK_F10 :
						index = 9;
						break;
					}

					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);
					_XPlayInterfaceSound(ID_SR_INTERFACE_QUICK_USE_WAV);

					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 처리
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(g_pLocalUser->m_SelectedSkillID == skillid)
						{
							if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHARGESKILL)
							{
								// charge skill
								switch(g_pLocalUser->m_CurrentBattleState)
								{
								case _XUSER_STATE_1 :
									{
										if(!g_pLocalUser->m_bStartCharge)
											g_pLocalUser->ResetSkillAnimation();
									}
									break;
								case _XUSER_STATE_2 :
									{
										if(!g_pLocalUser->m_bStartCharge)
										{
											if(g_pLocalUser->m_ChargeSendStartPacket)
												g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
											else
												g_pLocalUser->ResetSkillAnimation();
										}
									}
									break;
								case _XUSER_STATE_3 :
								case _XUSER_STATE_4 :
								case _XUSER_STATE_5 :
									{
										if(!g_pLocalUser->m_bStartCharge)
											g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
									}
									break;
								case _XUSER_STATE_6 :
									g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_7;
									break;
								}
							}
						}
					} // if(type == _XQS_SLOTTYPE_SKILL)
					else if(type == _XQS_SLOTTYPE_ITEM)
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						g_pLocalUser->UseItem(invennumber);
					}
					else if( type == _XQS_SLOTTYPE_LIFESKILL )
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						
						int selectindex = g_pQuickSlot_Window->GetSelectedSet()*10 + index;
						if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 1 ) // 노점 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_USEBUTTON, 0), (LPARAM)gHWnd); 
						}
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 2 ) // 비무홍보 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_PVPPRBUTTON, 0), (LPARAM)gHWnd); 
						}					
#ifdef _XDEF_EVENTWINDOW
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 3 ) // 이벤트 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_EVENTBUTTON, 0), (LPARAM)gHWnd); 
						}	
#endif
						
						
					}
				}
			}
			return true;
			
		case DIK_ADD		:
			{
				if( g_FreeMoveMode )
				{
					g_FreeMoveModeSpeed += 0.05;	

					if( g_FreeMoveModeSpeed > 5.0f )
						g_FreeMoveModeSpeed = 5.0f;

					_XInsertSystemMessage( 0, "Camera speed : %2.2f", g_FreeMoveModeSpeed );
				}
				else
				{
					if( g_pMinimap_Window )
					{
						if(!g_pMinimap_Window->m_bDisableZoomButton)
						{
							g_pMinimap_Window->m_MinimapZoomRate /= 1.05f;
							
							if( g_pMinimap_Window->m_MinimapZoomRate < 0.075f )
								g_pMinimap_Window->m_MinimapZoomRate = 0.075f;
							
							g_pMinimap_Window->m_RateOfMapSizeToTerrain *= 1.05f;
							
							if(g_pMinimap_Window->m_RateOfMapSizeToTerrain > 1.0f*(1024.0f/g_LodTerrain.m_Width))
								g_pMinimap_Window->m_RateOfMapSizeToTerrain = 1.0f*(1024.0f/g_LodTerrain.m_Width);
							
							g_pMinimap_Window->SetLocalUserPosition();
						}
					}
				}
			}
			return true;
		case DIK_SUBTRACT	:
			{
				if( g_FreeMoveMode )
				{
					g_FreeMoveModeSpeed -= 0.05;	

					if( g_FreeMoveModeSpeed < 0.0f )
						g_FreeMoveModeSpeed = 0.0f;

					_XInsertSystemMessage( 0, "Camera speed : %2.2f", g_FreeMoveModeSpeed );
				}
				else
				{
					if( g_pMinimap_Window )
					{
						if(!g_pMinimap_Window->m_bDisableZoomButton)
						{
							g_pMinimap_Window->m_MinimapZoomRate *= 1.05f;
							
							if( g_pMinimap_Window->m_MinimapZoomRate > 0.25f )
								g_pMinimap_Window->m_MinimapZoomRate = 0.25f;
							
							g_pMinimap_Window->m_RateOfMapSizeToTerrain /= 1.05;
							
							if( g_pMinimap_Window->m_RateOfMapSizeToTerrain < 0.3f*(1024.0f/g_LodTerrain.m_Width))
								g_pMinimap_Window->m_RateOfMapSizeToTerrain = 0.3f*(1024.0f/g_LodTerrain.m_Width);
							
							g_pMinimap_Window->SetLocalUserPosition();
						}
					}
				}
			}		
			return true;	
		case DIK_A :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					
				}
				else
				{			
					if(_XIMEKERNEL.GetIMEContainer())
						return true;
				}
			}
			return true;
		case DIK_P : // 운기 버튼 
			{
				if(_XIMEKERNEL.GetIMEContainer())
					return true;

				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_MEDBUTTON, 0), (LPARAM)gHWnd);
			}
			return true;

		case DIK_R :
			{
				_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );				
				if( pChatDefault_Window )
				{
					if( strlen(pChatDefault_Window->m_SavedWhisperTargetName) )
					{
						if( _XIMEKERNEL.GetIMEContainer() == NULL )
						{			
							m_ChatEditContainer.ShowWindow( TRUE );
							m_ChatEditContainer.SetFocus();
							
							pChatDefault_Window->m_MovingChatWindow = TRUE;
							pChatDefault_Window->SetDrawBorder(TRUE);
							
							pChatDefault_Window->InsertRecentWhisperUserList(pChatDefault_Window->m_SavedWhisperTargetName, 3);
							strcpy( pChatDefault_Window->m_WhisperTargetName, pChatDefault_Window->m_SavedWhisperTargetName );
							pChatDefault_Window->SetChatMode( _XCHATMODE_WHISPER );				
						}
					}
				}
			}
			return true;
		case DIK_SLASH :
			{
				if( _XIMEKERNEL.GetIMEContainer() == NULL )
				{
					m_ChatEditContainer.ShowWindow( TRUE );
					m_ChatEditContainer.SetFocus();
						
					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );				
					if( pChatDefault_Window )
					{
						pChatDefault_Window->m_MovingChatWindow = TRUE;
						pChatDefault_Window->SetDrawBorder(TRUE);
						
						strcpy( pChatDefault_Window->m_WhisperTargetName, _T("") );
						pChatDefault_Window->SetChatMode( _XCHATMODE_WHISPER );
					}
				}
			}
			return true;
		}	
		return false;



	}
#endif
}

bool XProc_MainGame::OnKeyDown(WPARAM wparam, LPARAM lparam)
{

//Author : 양희왕
#ifdef _XDEF_INPUTPRESS
	int prevKeystate = lparam & (0x40000000);

	BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
	if(!freecameramode)
	{
	
	if( prevKeystate ) // 0 일때 키가 처음 눌려 있다
		return FALSE;

	}

	//int presscount = lparam & 0xFFFF;
	//DWORD Tempscancode = (HIWORD( lparam ) & 0xFF);
	//TCHAR message[64];
	//sprintf( message, "%d, %d, %d, %d\n", prevKeystate, presscount, Tempscancode, lparam );
	//OutputDebugString( message );
#endif
//Last Updated : 07/03/30

	
#ifdef _XDEF_PARTY_NOTAVAILABLE
	m_dwLastInputTime = g_LocalSystemTime;		// 키 입력된 시간 저장
#endif
	
	if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
		g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
	{
		//인사나 박수 중일때는 이동 불가 
		// 경고 메세지?
		// 무기 삭제된 상태에서는 다시 장착하기 전까지 다른 행동을 할 수 없다.
		return true;
	}	
    
    // 운기조식 중에 키가 눌러져선 안될꺼같은데 //Author : 양희왕 //breif : 08.04.23
	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
	{
		if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
			return true;
	}

 	if( g_bShowItemMallWindow ) 
	{
		DWORD scancode = (HIWORD( lparam ) & 0xFF);
	
		if( scancode == DIK_ESCAPE )
		{
#ifdef _XTS_ITEMMALLBROWSER	
			_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
			if( pCashMall_Window )
			{
#ifdef _XVIETNAMESE				
				if( g_hItemMallWebBrowserObject )
				{
					//DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://www.cuulong.com.vn/ktc/logout.aspx");	
					DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulongid.zing.vn/ktc/logout.aspx");	
				}
#endif

				pCashMall_Window->ShowWindow( FALSE );
				ShowWindow(g_hItemMallWindow, SW_HIDE);
				g_bShowItemMallWindow = FALSE;
			}		
#endif
		}
		else if( scancode == DIK_X )
		{
#ifdef _XDEF_WEBBOARD_20061211	// 웹 게시판 기f능 추가 12월 21일 업데이트?
		/*	_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
			if( pWebBoard_Window  )
			{
				pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
				
				if( pWebBoard_Window->GetShowStatus() )
				{
					
					int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					
					TCHAR tempStr[256];
					memset( tempStr, 0, sizeof(TCHAR)*256 );
					
					if( !g_bTestWebPageAddress )
					{
						sprintf( tempStr, "http://nine.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
							,servernumber //서버번호 game_server=%d
							,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //케릭터레벨 game_lev=%d
							,g_pLocalUser->m_CharacterInfo.gender //성별 game_sex=%d
							,g_pLocalUser->m_CharacterInfo.Get_cClass() // 클래스 문파레벨 추후에 구현되면 추가 munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //영웅단이름 game_guild=%s&
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //영웅단레벨 추후에 구현되면 추가  game_guild_lev=%d
							,g_pLocalUser->m_CharacterInfo.groupindex //문파 munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.charactername //캐릭터이름 game_char=%s
							,g_pLocalUser->m_UserName //게임ID user_game_id=%s
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//영웅단레벨 game_hero_level1=%d (0:단원 1:부단주 2:단주)
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //영웅회이름 game_hero_name=%s
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //영웅회레벨 game_hero_level2=%d (0:임시 1:회원 2:부회주 3:회주)
							,1 //게시판 자동 이동 변수 hero_bbs=%d (0:게임게시판 1:영웅단 게시판 2:영웅회 게시판)
							);
					}
					else
					{						
						sprintf( tempStr, "http://9dtest.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
							,servernumber //서버번호 game_server=%d
							,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //케릭터레벨 game_lev=%d
							,g_pLocalUser->m_CharacterInfo.gender //성별 game_sex=%d
							,g_pLocalUser->m_CharacterInfo.Get_cClass() // 클래스 문파레벨 추후에 구현되면 추가 munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //영웅단이름 game_guild=%s&
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //영웅단레벨 추후에 구현되면 추가  game_guild_lev=%d
							,g_pLocalUser->m_CharacterInfo.groupindex //문파 munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.charactername //캐릭터이름 game_char=%s
							,g_pLocalUser->m_UserName //게임ID user_game_id=%s
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//영웅단레벨 game_hero_level1=%d (0:단원 1:부단주 2:단주)
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //영웅회이름 game_hero_name=%s
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //영웅회레벨 game_hero_level2=%d (0:임시 1:회원 2:부회주 3:회주)
							,1 //게시판 자동 이동 변수 hero_bbs=%d (0:게임게시판 1:영웅단 게시판 2:영웅회 게시판)
							);
					}
					
					DisplayHTMLPage(g_hIWebBoardWebBrowserObject, tempStr);
					
					
					g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
					POINT pt = { 0, 0 };
					ClientToScreen( gHWnd, &pt );
					SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
					
					ShowWindow(g_hWebBoardWindow, SW_SHOW);
					g_bShowItemMallWindow = TRUE;
				}
				else
				{
					ShowWindow(g_hWebBoardWindow, SW_HIDE );
					g_bShowItemMallWindow = FALSE;
				}
			}*/
#endif
		}
		return false;
	}
	
	// remove Tooltip
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}

	if( g_pMiniTooltip_Window )
	{
		if( g_pMiniTooltip_Window->GetShowStatus() )
			g_pMiniTooltip_Window->ShowWindow( FALSE );
	}	

	if( ::g_CurrentFocusedObject ) return false;

	if( g_MainWindowManager.OnKeyDown( wparam, lparam ) ) return TRUE;

	DWORD scancode = (HIWORD( lparam ) & 0xFF);

	// Tutorial 창 닫기
	if( scancode == DIK_ESCAPE )
	{
		_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
		if( pTutorialIcon )
		{
			if( pTutorialIcon->EscapeKeyDown() )
				return TRUE;
		}	
		_XWindow_SocketPlugIn* pSocketPlugin = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
		if( pSocketPlugin )
		{
			if( pSocketPlugin->GetShowStatus() )
			{
				//PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_SOCKETPLUGIN_CLOSE, 0), 0 );
				return TRUE;
			}
		}
		_XWindow_Gamble* pGamble_Window = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
		if( pGamble_Window )
		{
			if( pGamble_Window->GetShowStatus() )
			{
				//PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GAMBLE_CLOSE, 0), 0 );
				return TRUE;				
			}
		}
	}

	// 빈사상태일때 ESC - 빈사 상태 skip
	if( scancode == DIK_ESCAPE )
	{
		if(g_pLocalUser->GetMotionClass() == _XACTION_DYING)
		{
			if(!g_pLocalUser->m_bWaitDyingSkip)
			{
				g_NetworkKernel.SendPacket(MSG_NO_CHARAC_SELECT_MODE, 4);		// 빈사상태 skip
				g_pLocalUser->m_bWaitDyingSkip = TRUE;
				
				_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_Window )
				{
					if(pChargeGauge_Window->GetShowStatus())
					{
						if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_BINSA )
							pChargeGauge_Window->SetGaugeStop();
					}
				}
			}
		}
	}

	// 죽었을 때 ESC - 부활창 띄우기
	if(g_pLocalUser->GetMotionClass() == _XACTION_DIE)
	{
		if(scancode == DIK_ESCAPE)
		{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(!g_pLocalUser->GetEnterMatchEvent())
			{
#endif
				_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
				if(pRebirth_Window)
				{
					if(!pRebirth_Window->GetShowStatus())
					{
	#ifdef _ACCLAIM_IGAADSYSTEM
						if( g_IGAADWrapper.CheckExistContent( _XIGAADZONE_DEATHVIDEOAD ) )
						{
							pRebirth_Window->m_ShowIGAADButton = TRUE;
						}
						else
						{
							pRebirth_Window->m_ShowIGAADButton = FALSE;
						}
	#endif
						pRebirth_Window->ShowWindow(TRUE);
					}				
					return TRUE;
				}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			}
#endif
		}
	}
	// FUNCTIONAL OBJECT 와이드 모드 빠져나가기 
	if( m_bFunctionalObjectWideView )
	{
		if( scancode == DIK_ESCAPE )
		{			
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_FUNCTONIONALOBJRECTVIEW_CLOSE, 0), 0 );
			//m_bExitFunctionalObjectWideView = TRUE;
		}
	}

	// NPC와 대화중 ESC - 대화창 빠져나가기
	if( m_bNPCDialogMode || m_bNPCDialogWideView )
	{		
		if( scancode == DIK_ESCAPE )
		{			

#ifdef _ACCLAIM_IGAADSYSTEM // 광고중일때는 NPC 모드 취소 안됨.			
			if( m_UseNPCAD )
			{
				if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NPC || g_IGAADWrapper.GetShowStatus() )
				{
					return TRUE;
				}
			}
#endif

			if( m_pNPCScriptWindowPtr )
			{
				_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_Window )
				{
					if( !pChargeGauge_Window->GetShowStatus() )
					{
						if( g_LastNPCScriptOwner )
						{
							if( g_LastNPCScriptOwner->m_MessageType != _XDEF_NPCMESSAGE_HAVEQUESTLIST )
							{
								if( !m_pNPCScriptWindowPtr->m_listboxSelectionItem->GetShowStatus() )
								{
									if( !g_LastNPCScriptOwner->m_bStartQuestDialog )
										PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0 );
								}
							}
							else
							{
								if( !g_LastNPCScriptOwner->m_bStartQuestDialog )
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0 );
							}

						}
					}
					else
					{
						g_MainWindowManager.SetTopWindow(pChargeGauge_Window);
					}
				}
			}
			return TRUE;
		}
#ifdef _XDEF_AGEGRADE_070607_MAGVIPER //Author : 양희왕 //breif : 이용등급
		else if( scancode != DIK_G )
#endif
			return FALSE;
	}
	
	if(scancode == DIK_ESCAPE)
	{
		if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL)
		{
			if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_6 || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_7)
			{
				g_pLocalUser->m_bUsingSkill = FALSE;
				g_pLocalUser->m_bCheckDelayTime = TRUE;
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
				g_pLocalUser->m_ChangeDelayStartTime = g_LocalSystemTime;

				//Author : 양희왕 //breif : 비격기 사용시 fire 패킷이 안보내져도.. 쿨타임 적용
				g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
				// 예외처리 - 제자리 뛰기 할수 있으니
				g_pLocalUser->SetAnimation(_XACTION_IDLE);
				g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

				if(g_pLocalUser->m_bSendFinishFirePacket)
				{
					if(g_pLocalUser->m_SelectedSkillItem)
					{
						// 이미 Fire packet이 보내졌으므로 cooldown 적용
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
							//g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->detailInfo[g_pLocalUser->m_SelectedSkillItem->m_cSkillLevel].coolDown;
					}
				}
			}
		}
	}
	
	switch( (U32)wparam ) 
	{
	case VK_DELETE  :
		{
			g_LodTerrain.m_3PCamera.SetDefaultViewMode( TRUE );
		}
		return true;
	case VK_CONTROL :
		{
			g_UsernameTooltipViewTimer = 255;
		}		
		return true;
	case VK_PAGE_UP :
		{
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.m_MaxDistance );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);	
			g_pLocalUser->m_ModelDescriptor.m_CameraDistance = g_LodTerrain.m_3PCamera.GetDistance();
		}
		return true;
	case VK_PAGE_DOWN :
		{	
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.m_MinDistance );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);				
			g_pLocalUser->m_ModelDescriptor.m_CameraDistance = g_LodTerrain.m_3PCamera.GetDistance();
		}
		return true;
	case VK_HOME :
		{
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - 0.3f );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);	
			g_pLocalUser->m_ModelDescriptor.m_CameraDistance = g_LodTerrain.m_3PCamera.GetDistance();
		}
		return true;
	case VK_END :
		{	
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() + 0.3f );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);				
			g_pLocalUser->m_ModelDescriptor.m_CameraDistance = g_LodTerrain.m_3PCamera.GetDistance();
		}
		return true;
	case VK_NUMPAD0 :
	case VK_NUMPAD1 :
	case VK_NUMPAD2 :
	case VK_NUMPAD3 :
	case VK_NUMPAD4 :
	case VK_NUMPAD5 :
	case VK_NUMPAD6 :
	case VK_NUMPAD7 :
	case VK_NUMPAD8 :
	case VK_NUMPAD9 :
		{
			if(g_pLocalUser->m_bMatchPRMode)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2408), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				return true;
			}
			
			if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING )
			{
				return true;
			}			
			if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				return true;
			}			
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
			{
				return true;
			}
			if( g_pLocalUser->m_bSocketPlugIn || g_pLocalUser->m_bGambleBoxOpen || 
				g_pLocalUser->m_bReadingBook || g_pLocalUser->m_bInternalUse )
			{
				return true;
			}
			if(g_pLocalUser->m_BattleMode != _XBATTLE_PEACE)
			{
				return true;
			}
			
			// 메세지 박스가 떠 있는 상태에서는 이모콘티 표현을 할 수 없다.
			_XMessageWindow* pMessageBoxWindow = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
			if( pMessageBoxWindow && pMessageBoxWindow->GetShowStatus() ) return true;
			
			_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
			if( pTradeMessageBox && pTradeMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pPCTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADEMESSAGEWINDOW);
			if( pPCTradeMessageBox && pPCTradeMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pEventMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTMESSAGEWINDOW);
			if( pEventMessageBox && pEventMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pHealMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HEALMESSAGEWINDOW);
			if( pHealMessageBox && pHealMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pInchantMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
			if( pInchantMessageBox && pInchantMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pSkillMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_DELETESKILLMESSAGEWINDOW);
			if( pSkillMessageBox && pSkillMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pStateMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
			if( pStateMessageBox && pStateMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pTargetInfoMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
			if( pTargetInfoMessageBox && pTargetInfoMessageBox->GetShowStatus() ) return true;
			
			_XMessageWindow* pSocketMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			if( pSocketMessageBox && pSocketMessageBox->GetShowStatus() ) return true;
				
			extern _XCHARACTER_MOTION	g_EmotionActionTable[16];

			if( !( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_BOW && 
				   g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_CLAPPINGHAND ) ||
				 ( g_pLocalUser->m_ModelDescriptor.GetCurMotion() != g_EmotionActionTable[((U32)wparam - VK_NUMPAD0)]) )
			{
				if(	g_pLocalUser->GetMotionClass() != _XACTION_DYING	&&
					g_pLocalUser->GetMotionClass() != _XACTION_DIE		&&
					g_pLocalUser->GetMotionClass() != _XACTION_REBIRTH  && 
					g_pLocalUser->GetMotionClass() != _XACTION_INSENSIBLE &&
					g_pLocalUser->m_CharacterInfo.Get_current_lifepower() > 0 ) 
				{
					BOOL selfchanneling = FALSE;
					char usefultype = 0;
					char targettype = 0;
					if(g_pLocalUser->m_SelectedSkillID > 0)
					{
						usefultype = g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType;
						targettype = _XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID); //g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->targetType;
					}
					else
						usefultype = _XSI_USEFULTYPE_ACTIVESKILL;
					
					if( g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST )
					{
						g_pLocalUser->ResetAttackTarget();
						g_pLocalUser->SetAnimation(_XACTION_IDLE);
						g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
						g_pLocalUser->SetAnimationIndex( g_EmotionActionTable[((U32)wparam - VK_NUMPAD0)] );
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
			}
		}
		return true;
	}
		
	// Key 타입이 A일때 처리 - 단축키가 숫자
#ifdef _XDEF_KEYCHANGE_20070115
	if( g_bGameOptionKeyTypeASelected )
#endif
	{
        //Author : 양희왕 //breif : 베트남 게시판은 alt + x
        BOOL bCtrlKey = FALSE;

        if(gpInput->CheckKeyPress(DIK_LCONTROL))
            bCtrlKey = TRUE;
            
		switch( scancode )
		{
		case DIK_RETURN :
			{
	#ifdef _XDWDEBUG
				if( gpInput->CheckKeyPress(DIK_LCONTROL) ) 
				{
					if( !g_DebugCommandEditContainer.GetFocus() )
					{	
						_XWindow_Debug* pDebugWindow = 
							(_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );

						if( pDebugWindow->GetShowStatus() )
							g_DebugCommandEditContainer.SetFocus();
					}
				}			
				else
	#endif
				{
					if( m_bNPCDialogMode || m_bNPCDialogWideView || m_bFunctionalObjectWideView)
						return true;

					// 운기조식 미니게임 중 채팅 불가
					if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
					{
						if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
							return true;
					}
					
					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
					
					if( !m_ChatEditContainer.GetShowStatus() || !m_ChatEditContainer.GetFocus() )
					{
						_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA						
						m_ChatEditContainer.ClearBuffer();						
#endif
						m_ChatEditContainer.ShowWindow( TRUE );
						m_ChatEditContainer.SetFocus();

						static bool firstfocused = true;
						if( firstfocused )
						{
							firstfocused = false;
							_XIMEKERNEL.SetNativeMode( true );
						}

#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA						
						LPTSTR pStr = m_ChatEditContainer.GetText();
						if(pStr)
						{
							if( strlen( pStr ) == 0 )
							{
#endif
								switch(pChatDefault_Window->m_ChatMode) 
								{
								case _XCHATMODE_CLAN :
									break;
								case _XCHATMODE_SUBCLAN :
									m_ChatEditContainer.AddString( "@", 1);
									break;
								case _XCHATMODE_PARTY :
									m_ChatEditContainer.AddString( "!", 1);
									break;
								case _XCHATMODE_WHISPER :
									pChatDefault_Window->SetChangeWhisperName(0);
									break;
			#ifdef _XDEF_CASHITEM_SHOUT
								case _XCHATMODE_SHOUT :
									m_ChatEditContainer.AddString("%", 1);
									break;
			#endif
			#ifdef _XDEF_ZONECHATTING_20070108
								case _XCHATMODE_ZONE :
									m_ChatEditContainer.AddString("*", 1);
									break;
			#endif
								case _XCHATMODE_NORMAL :
									break;
								}							

#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
							}
						}
#endif

						pChatDefault_Window->m_MovingChatWindow = TRUE;
						pChatDefault_Window->SetDrawBorder(TRUE);
					}
				}
			}
			return true;
			
		case DIK_TAB:		
			{
				if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE || g_pLocalUser->GetMotionClass() == _XACTION_MOVE)
				{
					//Author : 양희왕 //breif : 무공사용중 평화상태로 캔슬하는 버그
					if(g_pLocalUser->m_CurrentBattleState >= _XUSER_STATE_2 && g_pLocalUser->m_CurrentBattleState <= _XUSER_STATE_9)
						return true;

					if((g_LocalSystemTime - g_pLocalUser->m_LastModeChangeTime) > 1000)
					{
						if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
							g_pLocalUser->RequestBattleMode(_XBATTLE_PEACE);
						else if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
							g_pLocalUser->RequestBattleMode(_XBATTLE_BATTLE);
						else
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2728 ), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전 중에는 다른 공격 상태로 변경할 수 없습니다."
					}
				}
			}
			return true;

		// 2004.06.07->oneway48 modify
			//  도움말 보기
		case DIK_F1 :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_HEDEK, 0), (LPARAM)gHWnd);		
			return true;

			// 메신져 열기
		case DIK_F7 :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_MESSENGER, 0), (LPARAM)gHWnd);
			return true;

			// 전서구 열기
		case DIK_F8 :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_EMAIL, 0), (LPARAM)gHWnd);
			return true;
			
			// BMG On/Off
		case DIK_F9 :
			{
				_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
				if( optionwindow )
				{	
					_XCheckButton* pButton = ((_XWindow_OptionSound*)optionwindow->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pUseBGMButton;

					if( pButton )
					{			
						pButton->SetCheck( !pButton->GetCheck() );
					
						g_BgmPlayFlag = pButton->GetCheck();
						g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, g_BgmPlayFlag );
						
	#ifdef _XUSEFMOD
						if( g_BgmPlayFlag )
						{
							if( m_CurrentBGMType == 0 )
								g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename1 );
							else
								g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename2 );
						}
						else
						{
							g_FMODWrapper.StopBGM();
						}
	#else	
						if( g_BgmPlayFlag )
						{
							if( g_pBGMSoundObject[m_CurrentBGMType+1] )
							{					
								g_pBGMSoundObject[m_CurrentBGMType+1]->Play();
							}						
						}
						else
						{
							if( g_pBGMSoundObject[m_CurrentBGMType+1] )
							{					
								g_pBGMSoundObject[m_CurrentBGMType+1]->Stop();
							}
						}
	#endif
					}
				}			
			}
			return true;

			// Window Clear
		case DIK_F5 :
			g_ViewMainInterface = !g_ViewMainInterface;
			return true;
			
			// Window default
		case DIK_F6 :
			{
				g_MainWindowManager.SetDefaultPosition();

				if( g_pInventory_Window )
				{
					g_pInventory_Window->ShowWindow( TRUE );
					g_pInventory_Window->ShowWindow( FALSE );
				}
				
				/*int ypos = 86;
				for( int i=0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
				{
					DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + i;
					if(g_pLocalUser->m_PartyInfo[i].isLeader )
					{
						_XWindow_PartyMemberMiniBar* partyminiwindow = partyminiwindow = (_XWindow_PartyMemberMiniBar *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
						if(partyminiwindow)
						{
							if( partyminiwindow->GetShowStatus() )
							{
								partyminiwindow->MoveWindow( 6, 46 );
							}
						}
					}
					else
					{
						_XWindow_PartyMemberMiniBar* partyminiwindow = partyminiwindow = (_XWindow_PartyMemberMiniBar *)g_MainWindowManager.FindWindow(windowid);
						if(partyminiwindow)
						{
							if( partyminiwindow->GetShowStatus() )
							{
								partyminiwindow->MoveWindow( 6, ypos );
								ypos+=27;
							}
						}
					}
				}*/
			}
			return true;

		case DIK_F12 :	
	#ifdef _XDWDEBUG			
			if( gpInput->CheckKeyPress(DIK_LCONTROL) )
			{
				gApp->Pause(true);
				gApp->UserSelectNewDevice();
				gApp->Pause(false);
			}
			else
	#endif
			{
	#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
				if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_logout))
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3759), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"비무 중에는 종료할 수 없습니다."
					break;
				}
	#endif
	#ifdef _XTS_PK
				if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전이 종료되지 않아 접속 화면으로 돌아갈 수 없습니다."
					break;
				}
	#endif
				#ifdef _XDEF_LOCK_F12BUTTON_071203_MAGVIPER
				if(g_pLocalUser->GetMotionClass() == _XACTION_DIE ) //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03
				{
					//메시지 추가하자..
					break;
				}
				#endif
				if( !m_GameExitFlag )
				{
					if( !g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMEEXITMESSAGEWINDOW) )
					{
//						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_GAMEEXITMESSAGEWINDOW);
//						pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_GAMECLOSE );
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_GAMECLOSE );
					}
				}
				
	#ifdef _XCHECKMEMORYUSAGE
				if( gpInput->CheckKeyPress(DIK_LCONTROL) )
				{
					extern void ReportUsingSystemResource( void );
					ReportUsingSystemResource();
				}
	#endif
			}
			return true;

		// Quick slot 단축키 처리
		case DIK_1 :
		case DIK_2 :
		case DIK_3 :
		case DIK_4 :
		case DIK_5 :
		case DIK_6 :
		case DIK_7 :
		case DIK_8 :
		case DIK_9 :
		case DIK_0 :
			{
				if( gpInput->CheckKeyPress(DIK_LSHIFT) || gpInput->CheckKeyPress(DIK_RSHIFT) )
				{
					if( scancode == DIK_1 || scancode == DIK_2 || scancode == DIK_3 )
					{
						return true;
					}
				}
				if(g_pLocalUser->m_bMatchPRMode)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2407), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return true;
				}

				if(g_pQuickSlot_Window)
				{
					int		index;

					switch(scancode)
					{
					case DIK_1 :
						index = 0;
						break;
					case DIK_2 :
						index = 1;
						break;
					case DIK_3 :
						index = 2;
						break;
					case DIK_4 :
						index = 3;
						break;
					case DIK_5 :
						index = 4;
						break;
					case DIK_6 :
						index = 5;
						break;
					case DIK_7 :
						index = 6;
						break;
					case DIK_8 :
						index = 7;
						break;
					case DIK_9 :
						index = 8;
						break;
					case DIK_0 :
						index = 9;
						break;
					}

					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);

					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 사용
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(_XSkillSelectCheckCallBack(0, skillid))
						{
							if(SelectSkill(skillid))
							{
								switch(g_SkillProperty[skillid]->usefulType)
								{
								case _XSI_USEFULTYPE_ACTIVESKILL :
									g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(index);
									break;
								case _XSI_USEFULTYPE_CHARGESKILL :
								case _XSI_USEFULTYPE_CASTINGSPELL :
								case _XSI_USEFULTYPE_CHANNELINGSPELL :
								case _XSI_USEFULTYPE_FINISHSKILL :
								case _XSI_USEFULTYPE_ROUNDINGSPELL :
								case _XSI_USEFULTYPE_SMASHINGSKILL :
								case _XSI_USEFULTYPE_ACTIONSKILL :
									{
										g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(index);
									}
									break;
								}
							}
						}

					}
					else if(type == _XQS_SLOTTYPE_ITEM)
					{
						// item 사용 - Key up에서 처리함
					}
				}
			}
			return true;

		case DIK_F2			:		
			{
				_XWindow_HelpScreen* pHelpScreen_Window = (_XWindow_HelpScreen*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_HELPSCREENWINDOW );
				if( pHelpScreen_Window )
				{
					if( pHelpScreen_Window->GetShowStatus() )
						pHelpScreen_Window->ShowWindow(FALSE);
					else
					{
						g_MainWindowManager.SetDefaultPosition();

						if( g_pInventory_Window )
						{
							g_pInventory_Window->ShowWindow( TRUE );
							g_pInventory_Window->ShowWindow( FALSE );
						}

						pHelpScreen_Window->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow(pHelpScreen_Window);

						if( g_pMinimap_Window )
						{
							if( g_pMinimap_Window->m_MiniMapMode == _XMINIMAPMODE_LARGE )
							{
								g_pMinimap_Window->SetMode( _XMINIMAPMODE_SMALL );
							}
						}
					}				
				}		
			}
			return true;

		case DIK_ADD		:
		case DIK_F3			:
			{
				if( g_FreeMoveMode )
				{
					g_FreeMoveModeSpeed += 0.05;	

					if( g_FreeMoveModeSpeed > 3.0f )
						g_FreeMoveModeSpeed = 3.0f;

					_XInsertSystemMessage( 0, "Camera speed : %2.2f", g_FreeMoveModeSpeed );
				}
				else
				{
					if( g_pMinimap_Window )
					{
						if(!g_pMinimap_Window->m_bDisableZoomButton)
						{
							g_pMinimap_Window->m_MinimapZoomRate /= 1.05f;
							
							if( g_pMinimap_Window->m_MinimapZoomRate < 0.075f )
								g_pMinimap_Window->m_MinimapZoomRate = 0.075f;
							
							g_pMinimap_Window->m_RateOfMapSizeToTerrain *= 1.05f;
							
							if(g_pMinimap_Window->m_RateOfMapSizeToTerrain > 1.0f*(1024.0f/g_LodTerrain.m_Width))
							   g_pMinimap_Window->m_RateOfMapSizeToTerrain = 1.0f*(1024.0f/g_LodTerrain.m_Width);
							
							g_pMinimap_Window->SetLocalUserPosition();
						}
					}
				}
			}
			return true;
		case DIK_SUBTRACT	:
		case DIK_F4			:
			{
				if( g_FreeMoveMode )
				{
					g_FreeMoveModeSpeed -= 0.05;	

					if( g_FreeMoveModeSpeed < 0.0f )
						g_FreeMoveModeSpeed = 0.0f;

					_XInsertSystemMessage( 0, "Camera speed : %2.2f", g_FreeMoveModeSpeed );
				}
				else
				{
					if( g_pMinimap_Window )
					{
						if(!g_pMinimap_Window->m_bDisableZoomButton)
						{
							g_pMinimap_Window->m_MinimapZoomRate *= 1.05f;
							
							if( g_pMinimap_Window->m_MinimapZoomRate > 0.25f )
								g_pMinimap_Window->m_MinimapZoomRate = 0.25f;
							
							g_pMinimap_Window->m_RateOfMapSizeToTerrain /= 1.05;
							
							if( g_pMinimap_Window->m_RateOfMapSizeToTerrain < 0.3f*(1024.0f/g_LodTerrain.m_Width))
								g_pMinimap_Window->m_RateOfMapSizeToTerrain = 0.3f*(1024.0f/g_LodTerrain.m_Width);
							
							g_pMinimap_Window->SetLocalUserPosition();
						}
					}
				}
			}		
			return true;	
		case DIK_ESCAPE :	
			{
#ifdef _XDEF_ESCAPE_20070205
				_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
				if(pEscape_Window)
				{					
					if( pEscape_Window->GetShowStatus() )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_ESCAPE_BTN_CANCLE, 0), 0 );
						return true;
					}
				}
#endif
				// target 삭제
				if(g_pLocalUser->m_ViewTarget)
				{
					if(g_pLocalUser->m_ViewTarget == g_pLocalUser->m_AttackTarget)
					{
						g_pLocalUser->ResetAttackTarget();
						return true;
					}
					g_pLocalUser->ResetViewTarget();
					return true;
				}

				if(g_pLocalUser->m_AttackTarget)
				{
					g_pLocalUser->ResetAttackTarget();
					return true;
				}

				// interface 닫기
				if( g_MainWindowManager.EscapeWindow() )
				{
					if( (m_pPCTradeWindowPtr && m_pPCTradeWindowPtr->m_bTrading ) || g_pLocalUser->m_CharacterLog.isTrade )
					{
						g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_CANCEL);
						m_pPCTradeWindowPtr->ShowWindow( FALSE );
					}

					BaseInterfaceWindowOpen(); //Author : 양희왕 //breif : 추가
					_XPlayInterfaceSound(ID_SR_INTERFACE_WINDOWCLOSE01_WAV); //2004.06.29->oneway48 insert
					return true;
				}
				
				// 수련 취소
				if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
				{
					if(g_pLocalUser->m_SelectedTrainingMode != _XTRAINING_MEDITATION)
					{
						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_CANCLETRAINING), TRUE, _XDEF_TRAINING_CANCEL_OK, _XDEF_TRAINING_CANCEL_NO);
						return true;
					}
				}

				// 비무장에서 나가기
				if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_WAITEXIT)
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2409), TRUE, _XDEF_MATCH_WAITEXIT);
					return true;
				}

				// 게임 종료
				if( !m_GameExitFlag )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_OPTION, 0), (LPARAM)gHWnd);
					//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_MAIGAME_WARNEXITGAME, TRUE, _XDEF_GOTOLOGINSERVER );
				}
			}
			return true;	

	#ifdef _XDWDEBUG			
			/*
		case DIK_T :
			{
				int index = g_FMODWrapper.FindSound( _T("DUEL_BGM_MAIN_00.wav") );
				g_FMODWrapper.PlaySound2D( index );
				//SetSpecialBGMPlay( "DUEL_BGM_MAIN_00.ogg" );
			}
			return true;
			*/
			
	#endif

		case DIK_SPACE:	// 아이템 줍기
			{		
				g_pLocalUser->m_bGrabItem = g_GameItemlist.GrabWorldItem();
			}
			return true;	
		case DIK_W :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = 0.0f;
					targetpos.y = 0.0f;
					targetpos.z = g_FreeMoveModeSpeed;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					//g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pOBBData );
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = NULL;//pOBBData;
									
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel(TRUE);

					g_pLocalUser->SetAnimation(_XACTION_MOVE);
					
					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
				}
				else
				{			
					if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					
					if( g_pLocalUser->m_CharacterLog.isTrade )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1824), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}

					if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1825), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					if( g_pLocalUser->m_bMatchPRMode ) // 비무 홍보일때 
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1826), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					if(g_pLocalUser->m_bUsingSkill)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					if( g_pLocalUser->m_bReadingBook )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //지금은 교체할 수 없습니다.
						return true;
					}
					if( g_pLocalUser->m_bInternalUse ) //영약 복용ㅇ중
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //지금은 교체할 수 없습니다.
						return true;
					}
                    if( g_pLocalUser->GetMotionClass() == _XACTION_DYING || g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_REBIRTH)
                    {
                        g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //지금은 교체할 수 없습니다.
						return true;
                    }

					#ifdef _XDEF_BLOCK_MOVINGATTACK //Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
					if(g_pLocalUser->GetMotionClass() == _XACTION_ATTACK) 
					{
						return true;
					}
					if(g_pLocalUser->m_CurrentBattleState >= _XUSER_STATE_2 && g_pLocalUser->m_CurrentBattleState <= _XUSER_STATE_LAST )
					{
						return true;
					}
					#endif
					
					if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
						g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
					{
						//인사나 박수 중일때는 이동 불가 
						// 경고 메세지?
						// 무기 삭제된 상태에서는 다시 장착하기 전까지 다른 행동을 할 수 없다.
						return true;
					}
					if(g_pLocalUser->m_CurrentSelectedSkillType != _XSI_USEFULTYPE_ACTIVESKILL && g_pLocalUser->m_CurrentSelectedSkillType != 0)
					{
						if(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							return true;
						}
					}
					if( g_pLocalUser->GetChangingWeapon() ) //Author : 양희왕  breif : 무기를 교체 중이면.. (데이터를 받는 중)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}

					if( (g_LocalSystemTime - m_LastWeaponChangeTime) > 2000 ) //1초 에서 2초로 늘림
					{
						m_LastWeaponChangeTime = g_LocalSystemTime;

						if( g_pCInfo_Window )
						{
							// 무기 교체
							_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
							if( pInventory_Window )
							{
								int prevMouseSlot = pInventory_Window->m_MouseSlotItem.m_InventoryNumber; // 잠깐 백업

								pInventory_Window->m_MouseSlotItem.m_InventoryNumber = _XINVENTORY_SLOTTYPE_WEAPON_2;

								g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 2, _XINVENTORY_SLOTTYPE_WEAPON_1 );

								pInventory_Window->m_MouseSlotItem.m_InventoryNumber = prevMouseSlot; // 마우스 슬롯 원래대로
							}
						}
					}
				}
			}
			return true;			
		case DIK_C :	// Character info
			{						
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_CHARACTORTAB)
						{
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_CHARACTORTAB);
							g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
						}
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_CHARACTORTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;		
			
		case DIK_I :	// Inventory
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_INVENTORY, 0), (LPARAM)gHWnd);		
			return true;		
			
		case DIK_Q : // Quest window
			{
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_QUESTTAB)
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						else
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_QUESTTAB);
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_QUESTTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;
		case DIK_L : // LifeSkill window
			{
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_LIFETAB)
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						else
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_LIFETAB);
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_LIFETAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;
		case DIK_D :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = g_FreeMoveModeSpeed;
					targetpos.y = 0.0f;
					targetpos.z = 0.0f;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = NULL;//pOBBData;
					
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel(TRUE);
					
					g_pLocalUser->SetAnimation(_XACTION_MOVE);

					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
				}
				else
				{			
					if(m_pMainFrameWindowPtr)
					{
						if(m_pMainFrameWindowPtr->GetShowStatus())
						{
							if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_MASTERYTAB)
								m_pMainFrameWindowPtr->ShowWindow(FALSE);
							else
								m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_MASTERYTAB);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_MASTERYTAB);
							m_pMainFrameWindowPtr->ShowWindow(TRUE);
						}
					}
				}
			}
			return true;

		case DIK_S : // Skill window
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = 0.0f;
					targetpos.y = 0.0f;
					targetpos.z = -g_FreeMoveModeSpeed;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = NULL;//pOBBData;
					
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel(TRUE);

					g_pLocalUser->SetAnimation(_XACTION_MOVE);
					
					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );				
				}
				else
				{			
					if( !g_TutorialInfo->_XTutorial_041 )
					{
						_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
						pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_041);
						g_TutorialInfo->_XTutorial_041 = TRUE;				
	#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
						g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
	#endif
					}
					
					if(m_pMainFrameWindowPtr)
					{
						if(m_pMainFrameWindowPtr->GetShowStatus())
						{
							if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_SKILLTAB)
								m_pMainFrameWindowPtr->ShowWindow(FALSE);
							else
								m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_SKILLTAB);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_SKILLTAB);
							m_pMainFrameWindowPtr->ShowWindow(TRUE);
							g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
						}
					}
				}
			}
			return true;
			
		case DIK_M :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MINIMAP_ZOOMMAXBUTTON, 0), (LPARAM)gHWnd);
			return true;
			
		case DIK_N :
			{
	#ifdef _XDEF_NEW_WORLDMAP_070528_MAGVIPER //Author : 양희왕 //breif : 신구룡전도
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_WORLDMAP_STARTBUTTON, 0), (LPARAM)gHWnd);
	#else
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MINIMAP_WORLDMAPBUTTON, 0), (LPARAM)gHWnd);
	#endif
			}
			return true;
			
		case DIK_O : // Option window 
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_OPTION, 0), (LPARAM)gHWnd);
			return true;
			
	#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
		#ifdef _XTS_MOVEVILLAGE
		case DIK_E :
			{
				if(m_pMoveVillageWindowPtr)
				{
					if(!m_pMoveVillageWindowPtr->GetShowStatus())
					{
						if(m_pMoveVillageWindowPtr->CheckUse())
						{
							if(g_pLocalUser->GetUserItemInvenSlotFromSecondType(_XGI_FC_RESOURCE, 4/*_XGI_SC_RESOURCE_MOVE*/) > 0)
							{
								m_pMoveVillageWindowPtr->ShowWindow(TRUE);
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
								pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3044), TRUE, 0 );	
								pMessageBox->StartTimer(5000, TRUE);
							}
						}
					}
					else
					{
						m_pMoveVillageWindowPtr->ShowWindow(FALSE);
					}
				}
			}
			return true;
		#endif
	#endif

	#ifdef _XDEF_NPCBUFF_CASH_080410_KUKURI
		case DIK_T :
			{
				if(m_pNPCBuffWindowPtr)
				{
					if(!m_pNPCBuffWindowPtr->GetShowStatus())
					{
						if(m_pNPCBuffWindowPtr->CheckUse())
						{
							if(g_pLocalUser->GetUserItemInvenSlotFromSecondType(_XGI_FC_RESOURCE, 5) > 0)
							{
								m_pNPCBuffWindowPtr->ShowWindow(TRUE);
								m_pNPCBuffWindowPtr->SetViewMode(EN_MODE_ITEM_VIEW);
								m_pNPCBuffWindowPtr->SetListFreeItem();
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
								pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3809), TRUE, 0 );//소지하고 있는 무명 신침이 없습니다.
								pMessageBox->StartTimer(5000, TRUE);
							}
						}
					}
					else
					{
						m_pNPCBuffWindowPtr->ShowWindow(FALSE);
					}
				}
			}
			return true;
	#endif
			
	#ifdef _XGMCLIENT
		case DIK_B :
			{
				_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
				if(pMessenger_Window)
				{
					pMessenger_Window->ShowWindow(!pMessenger_Window->GetShowStatus());
				}
			}
			return true;
    #else
        #if defined(_XTAIWANESE)
        case DIK_B :
			{
				_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
				if(pMessenger_Window)
				{
					pMessenger_Window->ShowWindow(!pMessenger_Window->GetShowStatus());
				}
			}
			return true;
        #endif
	#endif

		case DIK_F :
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			if( g_InnerUser && gpInput->CheckKeyPress( DIK_LCONTROL ) && gpInput->CheckKeyPress( DIK_LSHIFT ) )
			{
				#ifdef _XDWDEBUG
				g_FreeMoveMode = !g_FreeMoveMode;			
				#endif
			}			 
			else
			{
	#ifdef _XJAPANESE
				// 일본 버전일때 전적창 열지 않음
	#else
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if( m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_PKRECORDTAB )
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						else
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_PKRECORDTAB);
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_PKRECORDTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
	#endif
			}
			return true;
#else
	#ifdef _XDWDEBUG // 기존코드 유지용.
		if( g_InnerUser && gpInput->CheckKeyPress( DIK_LCONTROL ) && gpInput->CheckKeyPress( DIK_LSHIFT ) )
		{
			g_FreeMoveMode = !g_FreeMoveMode;
		}
	#endif
#endif
			return true;
		case DIK_V : 
			{
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_COMMUNITYTAB)
						{
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_COMMUNITYTAB);
							g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
						}
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_COMMUNITYTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;
		case DIK_A :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = -g_FreeMoveModeSpeed;
					targetpos.y = 0.0f;
					targetpos.z = 0.0f;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = NULL;//pOBBData;
					
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );

					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel(TRUE);
					
					g_pLocalUser->SetAnimation(_XACTION_MOVE);

					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );				
				}
				else
				{
					g_pLocalUser->SetViewTargetToAttackTarget();
				}
			}
			return true;

		case DIK_U :
			{
				// 자신을 타겟으로 잡기
				if(g_pLocalUser->m_AttackTargetObjectType != _XPICKINGTARGET_SELF)
				{
					g_pLocalUser->SetViewTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
					g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
				}
			}
			return true;

		case DIK_H :
			{	
	#ifdef _XDEF_EVENTWINDOW		
				// 이벤트 창 띄우기
				_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
				if( pEvent_Window )
				{
					pEvent_Window->ShowWindow( !pEvent_Window->GetShowStatus() );
				}
	#endif
			}
			return true;

		case DIK_BACKSLASH :
			{
	#ifdef _XDWDEBUG
				_XWindow_Debug* debugwindow = (_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );	
				if( debugwindow )
				{
					debugwindow->ShowWindow( !debugwindow->GetShowStatus() );
				}
	#endif
			}
			return true;
			
		case DIK_GRAVE :
			{
				if(gpInput->CheckKeyPress(DIK_LCONTROL))
				{
					// Quickslot 현재 10칸 clear
					if(g_pQuickSlot_Window)
						g_pQuickSlot_Window->ResetQuickSlotCurrentSet();
				}
				else
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_QS_NEXTBUTTON, 0), (LPARAM)gHWnd);
				}
			}
			return true;
		case DIK_X : 
			{				
	#ifdef _XDEF_WEBBOARD_20061211
            #ifdef _XVIETNAMESE //Author : 양희왕 //breif : 베트남 게시판 추가
                if(!bCtrlKey)
                    return true;
            #endif

				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				if( pWebBoard_Window  )
				{
					pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
					
					if( pWebBoard_Window->GetShowStatus() )
					{
						
						int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
						
						TCHAR tempStr[256];
						memset( tempStr, 0, sizeof(tempStr) );
						
                    #ifdef _XVIETNAMESE //Author : 양희왕 //breif : 베트남 게시판 추가
						_snprintf( tempStr, sizeof(tempStr), "http://9dingame.zing.vn/9d/");
					#else
						if( !g_bTestWebPageAddress )
						{
							_snprintf( tempStr, sizeof(tempStr), "http://nine.nexon.com/community/game_bbs/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s"
								,servernumber //서버번호 game_server=%d
								,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //케릭터레벨 game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //성별 game_sex=%d
								,g_pLocalUser->m_CharacterInfo.Get_cClass() // 클래스 문파레벨 추후에 구현되면 추가 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //영웅단이름 game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //영웅단레벨 추후에 구현되면 추가  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //문파 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //캐릭터이름 game_char=%s
								,g_pLocalUser->m_UserName//넥슨아이디 user_game_id=%s
								);
						}
						else
						{						
							_snprintf( tempStr, sizeof(tempStr), "http://9dtest.nexon.com/_temp/bbs/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s"
								,servernumber //서버번호 game_server=%d
								,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //케릭터레벨 game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //성별 game_sex=%d
								,g_pLocalUser->m_CharacterInfo.Get_cClass() // 클래스 문파레벨 추후에 구현되면 추가 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //영웅단이름 game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //영웅단레벨 추후에 구현되면 추가  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //문파 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //캐릭터이름 game_char=%s
								,g_pLocalUser->m_UserName//넥슨아이디 user_game_id=%s
								);
						}
                    #endif
						
						DisplayHTMLPage(g_hIWebBoardWebBrowserObject, tempStr);
						
						
						g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
						POINT pt = { 0, 0 };
						ClientToScreen( gHWnd, &pt );
						SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
						
						ShowWindow(g_hWebBoardWindow, SW_SHOW);
						g_bShowItemMallWindow = TRUE;
					}
					else
					{
						ShowWindow(g_hWebBoardWindow, SW_HIDE );
						g_bShowItemMallWindow = FALSE;
					}
				}
	#endif			
			}
			return true;
		case DIK_G :
			{
#ifdef _XDEF_AGEGRADE_070607_MAGVIPER //Author : 양희왕 //breif : 이용등급
				_XWindow_AgeGrade* pAgeGradeWindow = _XWindow_AgeGrade::GetInstance();
				if( !pAgeGradeWindow->GetShowStatus()) 
					pAgeGradeWindow->ShowWindow(TRUE);
				else
					pAgeGradeWindow->SetTime( g_LocalSystemTime );
#endif
			}
			return true;

	#ifdef _XDWDEBUG		
		case DIK_APOSTROPHE : // Mini map window
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CHATDEFAULT_RESIZEBUTTON, 0), (LPARAM)gHWnd);
			return true;		
		

		case DIK_Z : 
			{
#ifdef _XDWDEBUG
// 	#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI
// 				_XWindow_NPCResource_New* pNEWNPCResource_Window = (_XWindow_NPCResource_New*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NEW_RESOURCEWINDOW);
// 				if(pNEWNPCResource_Window)
// 				{
// 					if(!pNEWNPCResource_Window->GetShowStatus())
// 						pNEWNPCResource_Window->ShowWindow(TRUE);
// 					else
// 						pNEWNPCResource_Window->ShowWindow(FALSE);
// 				}
// 	#endif
#endif

			}
			return true;
	#endif
		}	
		
		return false; 


	}
	// Key 타입이 B일때 처리 - 단축키가 기능키(Function Key)
#ifdef _XDEF_KEYCHANGE_20070115
	else
	{
		
		switch( (U32)wparam ) 
		{
		case VK_INSERT		:		
			{
				_XWindow_HelpScreen* pHelpScreen_Window = (_XWindow_HelpScreen*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_HELPSCREENWINDOW );
				if( pHelpScreen_Window )
				{
					if( pHelpScreen_Window->GetShowStatus() )
						pHelpScreen_Window->ShowWindow(FALSE);
					else
					{
						
						if( g_bGameOptionKeyTypeASelected )
						{
							pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_QUICKSLOT, _XGETINTERFACETEXT(ID_STRING_NEW_2671), _XGETINTERFACETEXT(ID_STRING_NEW_2684) );	
							pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MESSENGER, _XGETINTERFACETEXT(ID_STRING_NEW_2673), _XGETINTERFACETEXT(ID_STRING_NEW_2686) );
							pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MAIL, _XGETINTERFACETEXT(ID_STRING_NEW_2674), _XGETINTERFACETEXT(ID_STRING_NEW_2687) );
							
						}
						else
						{
							pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_QUICKSLOT, _XGETINTERFACETEXT(ID_STRING_NEW_3239), _XGETINTERFACETEXT(ID_STRING_NEW_2684) );	
							pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MESSENGER, _XGETINTERFACETEXT(ID_STRING_NEW_3240), _XGETINTERFACETEXT(ID_STRING_NEW_2686) );
							pHelpScreen_Window->SetHelpDescriptionBox(_XHS_HELPBOX_MAIL, _XGETINTERFACETEXT(ID_STRING_NEW_3241), _XGETINTERFACETEXT(ID_STRING_NEW_2687) );
						}

						g_MainWindowManager.SetDefaultPosition();
						
						if( g_pInventory_Window )
						{
							g_pInventory_Window->ShowWindow( TRUE );
							g_pInventory_Window->ShowWindow( FALSE );
						}
						
						pHelpScreen_Window->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow(pHelpScreen_Window);
						
						if( g_pMinimap_Window )
						{
							if( g_pMinimap_Window->m_MiniMapMode == _XMINIMAPMODE_LARGE )
							{
								g_pMinimap_Window->SetMode( _XMINIMAPMODE_SMALL );
							}
						}
					}				
				}		
			}
			return true;
		case VK_F10		:
			{
				if(g_pLocalUser->m_bMatchPRMode)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2407), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return true;
				}
				
				if(g_pQuickSlot_Window)
				{
					int		index = 9;
					
					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);
					
					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 사용
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(_XSkillSelectCheckCallBack(0, skillid))
						{
							if(SelectSkill(skillid))
							{
								switch(g_SkillProperty[skillid]->usefulType)
								{
								case _XSI_USEFULTYPE_ACTIVESKILL :
									g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(index);
									break;
								case _XSI_USEFULTYPE_CHARGESKILL :
								case _XSI_USEFULTYPE_CASTINGSPELL :
								case _XSI_USEFULTYPE_CHANNELINGSPELL :
								case _XSI_USEFULTYPE_FINISHSKILL :
								case _XSI_USEFULTYPE_ROUNDINGSPELL :
								case _XSI_USEFULTYPE_SMASHINGSKILL :
								case _XSI_USEFULTYPE_ACTIONSKILL :
									{
										g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(index);
									}
									break;
								}
							}
						}
						
					}
					else if(type == _XQS_SLOTTYPE_ITEM)
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						g_pLocalUser->UseItem(invennumber);
					}
					else if( type == _XQS_SLOTTYPE_LIFESKILL )
					{
						int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }

                        if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	                    {
		                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            {
                                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                                return true;
                            }
	                    }
                    #endif
						g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
						g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
						
						int selectindex = g_pQuickSlot_Window->GetSelectedSet()*10 + index;
						if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 1 ) // 노점 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_USEBUTTON, 0), (LPARAM)gHWnd); 
						}
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 2 ) // 비무홍보 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_PVPPRBUTTON, 0), (LPARAM)gHWnd); 
						}					
#ifdef _XDEF_EVENTWINDOW
						else if( g_pQuickSlot_Window->m_QuickSlot[selectindex].lifeskillid == 3 ) // 이벤트 버튼
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_EVENTBUTTON, 0), (LPARAM)gHWnd); 
						}	
#endif
						
						
					}
				}
			}
			return true;

				
		}
		
		switch( scancode )
		{
		case DIK_RETURN :
			{
	#ifdef _XDWDEBUG
				if( gpInput->CheckKeyPress(DIK_LCONTROL) ) 
				{
					if( !g_DebugCommandEditContainer.GetFocus() )
					{	
						_XWindow_Debug* pDebugWindow = 
							(_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );

						if( pDebugWindow->GetShowStatus() )
							g_DebugCommandEditContainer.SetFocus();
					}
				}			
				else
	#endif
				{
					if( m_bNPCDialogMode || m_bNPCDialogWideView || m_bFunctionalObjectWideView)
						return true;

					// 운기조식 미니게임 중 채팅 불가
					if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
					{
						if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
							return true;
					}
					
					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
					
					if( !m_ChatEditContainer.GetShowStatus() || !m_ChatEditContainer.GetFocus() )
					{
						_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
						m_ChatEditContainer.ClearBuffer();						
#endif
						m_ChatEditContainer.ShowWindow( TRUE );
						m_ChatEditContainer.SetFocus();

						static bool firstfocused2 = true;
						if( firstfocused2 )
						{
							firstfocused2 = false;
							_XIMEKERNEL.SetNativeMode( true );
						}
							
#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA						
						LPTSTR pStr = m_ChatEditContainer.GetText();
						if(pStr)
						{
							if( strlen( pStr ) == 0 )
							{
#endif
								switch(pChatDefault_Window->m_ChatMode) 
								{
								case _XCHATMODE_CLAN :
									break;
								case _XCHATMODE_SUBCLAN :
									m_ChatEditContainer.AddString( "@", 1);
									break;
								case _XCHATMODE_PARTY :
									m_ChatEditContainer.AddString( "!", 1);										
									break;
								case _XCHATMODE_WHISPER :
									pChatDefault_Window->SetChangeWhisperName(0);
									break;
				#ifdef _XDEF_CASHITEM_SHOUT
								case _XCHATMODE_SHOUT :
									m_ChatEditContainer.AddString("%", 1);
									break;
				#endif
				#ifdef _XDEF_ZONECHATTING_20070108
								case _XCHATMODE_ZONE :
									m_ChatEditContainer.AddString("*", 1);
									break;
				#endif
								case _XCHATMODE_NORMAL :
									break;
								}

#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
							}	
						}
#endif
						pChatDefault_Window->m_MovingChatWindow = TRUE;
						pChatDefault_Window->SetDrawBorder(TRUE);
					}
				}
			}
			return true;
			
		case DIK_TAB:		
			{
				if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE || g_pLocalUser->GetMotionClass() == _XACTION_MOVE)
				{
					if((g_LocalSystemTime - g_pLocalUser->m_LastModeChangeTime) > 1000)
					{
						if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
							g_pLocalUser->RequestBattleMode(_XBATTLE_PEACE);
						else if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
							g_pLocalUser->RequestBattleMode(_XBATTLE_BATTLE);
						else
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2728 ), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전 중에는 다른 공격 상태로 변경할 수 없습니다."
					}
				}
			}
			return true;

		// 2004.06.07->oneway48 modify
			//  도움말 보기
		case DIK_F11 :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_HEDEK, 0), (LPARAM)gHWnd);		
			return true;
			
			// 메신져 열기
		case DIK_SEMICOLON :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_MESSENGER, 0), (LPARAM)gHWnd);
			return true;

			// 전서구 열기
		case DIK_APOSTROPHE :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_EMAIL, 0), (LPARAM)gHWnd);
			return true;
			
			// BMG On/Off
//		case DIK_F9 :
//			{
//				_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
//				if( optionwindow )
//				{	
//					_XCheckButton* pButton = ((_XWindow_OptionSound*)optionwindow->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pUseBGMButton;
//
//					if( pButton )
//					{			
//						pButton->SetCheck( !pButton->GetCheck() );
//					
//						g_BgmPlayFlag = pButton->GetCheck();
//						g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, g_BgmPlayFlag );
//						
//	#ifdef _XUSEFMOD
//						if( g_BgmPlayFlag )
//						{
//							if( m_CurrentBGMType == 0 )
//								g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename1 );
//							else
//								g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename2 );
//						}
//						else
//						{
//							g_FMODWrapper.StopBGM();
//						}
//	#else	
//						if( g_BgmPlayFlag )
//						{
//							if( g_pBGMSoundObject[m_CurrentBGMType+1] )
//							{					
//								g_pBGMSoundObject[m_CurrentBGMType+1]->Play();
//							}						
//						}
//						else
//						{
//							if( g_pBGMSoundObject[m_CurrentBGMType+1] )
//							{					
//								g_pBGMSoundObject[m_CurrentBGMType+1]->Stop();
//							}
//						}
//	#endif
//					}
//				}			
//			}
//			return true;

			// Window Clear
		case DIK_LBRACKET :
			g_ViewMainInterface = !g_ViewMainInterface;
			return true;
			
			// Window default
		case DIK_RBRACKET :
			{
				g_MainWindowManager.SetDefaultPosition();

				if( g_pInventory_Window )
				{
					g_pInventory_Window->ShowWindow( TRUE );
					g_pInventory_Window->ShowWindow( FALSE );
				}
				
				/*int ypos = 86;
				for( int i=0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
				{
					DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + i;
					if(g_pLocalUser->m_PartyInfo[i].isLeader )
					{
						_XWindow_PartyMemberMiniBar* partyminiwindow = partyminiwindow = (_XWindow_PartyMemberMiniBar *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
						if(partyminiwindow)
						{
							if( partyminiwindow->GetShowStatus() )
							{
								partyminiwindow->MoveWindow( 6, 46 );
							}
						}
					}
					else
					{
						_XWindow_PartyMemberMiniBar* partyminiwindow = partyminiwindow = (_XWindow_PartyMemberMiniBar *)g_MainWindowManager.FindWindow(windowid);
						if(partyminiwindow)
						{
							if( partyminiwindow->GetShowStatus() )
							{
								partyminiwindow->MoveWindow( 6, ypos );
								ypos+=27;
							}
						}
					}
				}*/
			}
			return true;

		case DIK_F12 :	
	#ifdef _XDWDEBUG			
			if( gpInput->CheckKeyPress(DIK_LCONTROL) )
			{
				gApp->Pause(true);
				gApp->UserSelectNewDevice();
				gApp->Pause(false);
			}
			else
	#endif
			{
	#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
				if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_logout))
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3759), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"비무 중에는 종료할 수 없습니다."
					break;
				}
	#endif
	#ifdef _XTS_PK
				if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전이 종료되지 않아 접속 화면으로 돌아갈 수 없습니다."
					break;
				}
	#endif
				#ifdef _XDEF_LOCK_F12BUTTON_071203_MAGVIPER
				if(g_pLocalUser->GetMotionClass() == _XACTION_DIE ) //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03
				{
					//메시지 추가하자..
					break;
				}
				#endif
				if( !m_GameExitFlag )
				{
					if( !g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMEEXITMESSAGEWINDOW) )
					{
//						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_GAMEEXITMESSAGEWINDOW);
//						pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_GAMECLOSE );
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_GAMECLOSE );
					}
				}
				
	#ifdef _XCHECKMEMORYUSAGE
				if( gpInput->CheckKeyPress(DIK_LCONTROL) )
				{
					extern void ReportUsingSystemResource( void );
					ReportUsingSystemResource();
				}
	#endif
			}
			return true;

		// Quick slot 단축키 처리
		case DIK_F1 :
		case DIK_F2 :
		case DIK_F3 :
		case DIK_F4 :
		case DIK_F5 :
		case DIK_F6 :
		case DIK_F7 :
		case DIK_F8 :
		case DIK_F9 :
		case DIK_F10 :
			{
				if( gpInput->CheckKeyPress(DIK_LSHIFT) || gpInput->CheckKeyPress(DIK_RSHIFT) )
				{
					if( scancode == DIK_F1 || scancode == DIK_F2 || scancode == DIK_F3 )
					{
						return true;
					}
				}
				if(g_pLocalUser->m_bMatchPRMode)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2407), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return true;
				}

				if(g_pQuickSlot_Window)
				{
					int		index;
					switch(scancode)
					{
					case DIK_F1 :
						index = 0;
						break;
					case DIK_F2 :
						index = 1;
						break;
					case DIK_F3 :
						index = 2;
						break;
					case DIK_F4 :
						index = 3;
						break;
					case DIK_F5 :
						index = 4;
						break;
					case DIK_F6 :
						index = 5;
						break;
					case DIK_F7 :
						index = 6;
						break;
					case DIK_F8 :
						index = 7;
						break;
					case DIK_F9 :
						index = 8;
						break;
					case DIK_F10 :
						index = 9;
						break;
					}

					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);

					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 사용
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(_XSkillSelectCheckCallBack(0, skillid))
						{
							if(SelectSkill(skillid))
							{
								switch(g_SkillProperty[skillid]->usefulType)
								{
								case _XSI_USEFULTYPE_ACTIVESKILL :
									g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(index);
									break;
								case _XSI_USEFULTYPE_CHARGESKILL :
								case _XSI_USEFULTYPE_CASTINGSPELL :
								case _XSI_USEFULTYPE_CHANNELINGSPELL :
								case _XSI_USEFULTYPE_FINISHSKILL :
								case _XSI_USEFULTYPE_ROUNDINGSPELL :
								case _XSI_USEFULTYPE_SMASHINGSKILL :
								case _XSI_USEFULTYPE_ACTIONSKILL :
									{
										g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(index);
									}
									break;
								}
							}
						}

					}
					else if(type == _XQS_SLOTTYPE_ITEM)
					{
						// item 사용 - Key up에서 처리함
					}
				}
			}
			return true;


		case DIK_ESCAPE :	
			{
				
#ifdef _XDEF_ESCAPE_20070205
				_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
				if(pEscape_Window)
				{					
					if( pEscape_Window->GetShowStatus() )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_ESCAPE_BTN_CANCLE, 0), 0 );
						return true;
					}
				}
#endif
				// target 삭제
				if(g_pLocalUser->m_ViewTarget)
				{
					if(g_pLocalUser->m_ViewTarget == g_pLocalUser->m_AttackTarget)
					{
						g_pLocalUser->ResetAttackTarget();
						return true;
					}
					g_pLocalUser->ResetViewTarget();
					return true;
				}

				if(g_pLocalUser->m_AttackTarget)
				{
					g_pLocalUser->ResetAttackTarget();
					return true;
				}

				// interface 닫기
				if( g_MainWindowManager.EscapeWindow() )
				{
					if( (m_pPCTradeWindowPtr && m_pPCTradeWindowPtr->m_bTrading ) || g_pLocalUser->m_CharacterLog.isTrade )
					{
						g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_CANCEL);
						m_pPCTradeWindowPtr->ShowWindow( FALSE );
					}

					_XPlayInterfaceSound(ID_SR_INTERFACE_WINDOWCLOSE01_WAV); //2004.06.29->oneway48 insert
					return true;
				}
				
				// 수련 취소
				if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
				{
					if(g_pLocalUser->m_SelectedTrainingMode != _XTRAINING_MEDITATION)
					{
						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_CANCLETRAINING), TRUE, _XDEF_TRAINING_CANCEL_OK, _XDEF_TRAINING_CANCEL_NO);
						return true;
					}
				}

				// 비무장에서 나가기
				if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_WAITEXIT)
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2409), TRUE, _XDEF_MATCH_WAITEXIT);
					return true;
				}

				// 게임 종료
				if( !m_GameExitFlag )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_OPTION, 0), (LPARAM)gHWnd);
					//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_MAIGAME_WARNEXITGAME, TRUE, _XDEF_GOTOLOGINSERVER );
				}
			}
			return true;	

	#ifdef _XDWDEBUG			
			/*
		case DIK_T :
			{
				int index = g_FMODWrapper.FindSound( _T("DUEL_BGM_MAIN_00.wav") );
				g_FMODWrapper.PlaySound2D( index );
				//SetSpecialBGMPlay( "DUEL_BGM_MAIN_00.ogg" );
			}
			return true;
			*/
			
	#endif

		case DIK_SPACE:	// 아이템 줍기
			{		
				g_pLocalUser->m_bGrabItem = g_GameItemlist.GrabWorldItem();
			}
			return true;	
		case DIK_W :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = 0.0f;
					targetpos.y = 0.0f;
					targetpos.z = g_FreeMoveModeSpeed;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = NULL;
					
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel(TRUE);

					g_pLocalUser->SetAnimation(_XACTION_MOVE);
					
					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
				}
				else
				{			
					if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					
					if( g_pLocalUser->m_CharacterLog.isTrade )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1824), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}

					if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1825), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					if( g_pLocalUser->m_bMatchPRMode ) // 비무 홍보일때 
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1826), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					if(g_pLocalUser->m_bUsingSkill)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return true;
					}
					#ifdef _XDEF_BLOCK_MOVINGATTACK
					if(g_pLocalUser->GetMotionClass() == _XACTION_ATTACK) //Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
					{
						return true;
					}
					if(g_pLocalUser->m_CurrentBattleState >= _XUSER_STATE_2 && g_pLocalUser->m_CurrentBattleState <= _XUSER_STATE_LAST )
					{
						return true;
					}
					#endif
					
					if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
						g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
					{
						//인사나 박수 중일때는 이동 불가 
						// 경고 메세지?
						// 무기 삭제된 상태에서는 다시 장착하기 전까지 다른 행동을 할 수 없다.
						return true;
					}

					if( (g_LocalSystemTime - m_LastWeaponChangeTime) > 1000 )
					{
						m_LastWeaponChangeTime = g_LocalSystemTime;

						if( g_pCInfo_Window )
						{
							// 무기 교체
							_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
							if( pInventory_Window )
							{
								int prevMouseSlot = pInventory_Window->m_MouseSlotItem.m_InventoryNumber; // 잠깐 백업

								pInventory_Window->m_MouseSlotItem.m_InventoryNumber = _XINVENTORY_SLOTTYPE_WEAPON_2;

								g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 2, _XINVENTORY_SLOTTYPE_WEAPON_1 );

								pInventory_Window->m_MouseSlotItem.m_InventoryNumber = prevMouseSlot; // 마우스 슬롯 원래대로
							}
						}
					}
				}
			}
			return true;			
		case DIK_C :	// Character info
			{						
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_CHARACTORTAB)
						{
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_CHARACTORTAB);
							g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
						}
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_CHARACTORTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;		
			
		case DIK_I :	// Inventory
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_INVENTORY, 0), (LPARAM)gHWnd);		
			return true;		
			
		case DIK_Q : // Quest window
			{
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_QUESTTAB)
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						else
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_QUESTTAB);
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_QUESTTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;
		case DIK_L : // LifeSkill window
			{
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_LIFETAB)
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						else
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_LIFETAB);
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_LIFETAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;
		case DIK_D :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = g_FreeMoveModeSpeed;
					targetpos.y = 0.0f;
					targetpos.z = 0.0f;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = NULL;
					
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel();
					
					g_pLocalUser->SetAnimation(_XACTION_MOVE);

					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
				}
				else
				{			
					if(m_pMainFrameWindowPtr)
					{
						if(m_pMainFrameWindowPtr->GetShowStatus())
						{
							if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_MASTERYTAB)
								m_pMainFrameWindowPtr->ShowWindow(FALSE);
							else
								m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_MASTERYTAB);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_MASTERYTAB);
							m_pMainFrameWindowPtr->ShowWindow(TRUE);
						}
					}
				}
			}
			return true;

		case DIK_S : // Skill window
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = 0.0f;
					targetpos.y = 0.0f;
					targetpos.z = -g_FreeMoveModeSpeed;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = pOBBData;
					
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel();

					g_pLocalUser->SetAnimation(_XACTION_MOVE);
					
					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );				
				}
				else
				{			
					if( !g_TutorialInfo->_XTutorial_041 )
					{
						_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
						pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_041);
						g_TutorialInfo->_XTutorial_041 = TRUE;				
	#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
						g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
	#endif
					}
					
					if(m_pMainFrameWindowPtr)
					{
						if(m_pMainFrameWindowPtr->GetShowStatus())
						{
							if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_SKILLTAB)
								m_pMainFrameWindowPtr->ShowWindow(FALSE);
							else
								m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_SKILLTAB);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_SKILLTAB);
							m_pMainFrameWindowPtr->ShowWindow(TRUE);
							g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
						}
					}
				}
			}
			return true;
			
		case DIK_M :
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MINIMAP_ZOOMMAXBUTTON, 0), (LPARAM)gHWnd);
			return true;
			
		case DIK_N :
			{
#ifdef _XDEF_NEW_WORLDMAP_070528_MAGVIPER //Author : 양희왕 //breif : 신구룡전도
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_WORLDMAP_STARTBUTTON, 0), (LPARAM)gHWnd);
#else
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MINIMAP_WORLDMAPBUTTON, 0), (LPARAM)gHWnd);
#endif
			}
			return true;
			
		case DIK_O : // Option window 
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_OPTION, 0), (LPARAM)gHWnd);
			return true;

	#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
		#ifdef _XTS_MOVEVILLAGE
		case DIK_E :
			{
				if(m_pMoveVillageWindowPtr)
				{
					if(!m_pMoveVillageWindowPtr->GetShowStatus())
					{
						if(m_pMoveVillageWindowPtr->CheckUse())
						{
							if(g_pLocalUser->GetUserItemInvenSlotFromSecondType(_XGI_FC_RESOURCE, 4/*_XGI_SC_RESOURCE_MOVE*/) > 0)
							{
								m_pMoveVillageWindowPtr->ShowWindow(TRUE);
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
								pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3044), TRUE, 0 );	
								pMessageBox->StartTimer(5000, TRUE);
							}
						}
					}
					else
					{
						m_pMoveVillageWindowPtr->ShowWindow(FALSE);
					}
				}
			}
			return true;
		#endif
	#endif
			
	#ifdef _XDEF_NPCBUFF_CASH_080410_KUKURI
		case DIK_T :
			{
				if(m_pNPCBuffWindowPtr)
				{
					if(!m_pNPCBuffWindowPtr->GetShowStatus())
					{
						if(m_pNPCBuffWindowPtr->CheckUse())
						{
							if(g_pLocalUser->GetUserItemInvenSlotFromSecondType(_XGI_FC_RESOURCE, 5) > 0)
							{
								m_pNPCBuffWindowPtr->ShowWindow(TRUE);
								m_pNPCBuffWindowPtr->SetViewMode(EN_MODE_ITEM_VIEW);
								m_pNPCBuffWindowPtr->SetListFreeItem();
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
								pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3809), TRUE, 0 );//소지하고 있는 무명 신침이 없습니다.	
								pMessageBox->StartTimer(5000, TRUE);
							}
						}
					}
					else
					{
						m_pNPCBuffWindowPtr->ShowWindow(FALSE);
					}
				}
			}
			return true;
	#endif

	#ifdef _XDWDEBUG
		case DIK_B :
			{
			}
			return true;		
	#endif		
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	case DIK_F :	
			if( g_InnerUser && gpInput->CheckKeyPress( DIK_LCONTROL ) && gpInput->CheckKeyPress( DIK_LSHIFT ) )
			{
				#ifdef _XDWDEBUG
				g_FreeMoveMode = !g_FreeMoveMode;			
				#endif
			}			 
			else
			{
#ifdef _XJAPANESE
				// 일본 버전일 때 전적창 열지 않음
#else
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if( m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_PKRECORDTAB )
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						else
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_PKRECORDTAB);
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_PKRECORDTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
#endif
			}
			return true;
#else
	#ifdef _XDWDEBUG // 기존코드 유지용.
		case DIK_F :	
		if( g_InnerUser && gpInput->CheckKeyPress( DIK_LCONTROL ) && gpInput->CheckKeyPress( DIK_LSHIFT ) )
		{
			g_FreeMoveMode = !g_FreeMoveMode;
		}
	#endif
#endif
		case DIK_V : 
			{
				if(m_pMainFrameWindowPtr)
				{
					if(m_pMainFrameWindowPtr->GetShowStatus())
					{
						if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_COMMUNITYTAB)
						{
							m_pMainFrameWindowPtr->ShowWindow(FALSE);
						}
						else
						{
							m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_COMMUNITYTAB);
							g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
						}
					}
					else
					{
						m_pMainFrameWindowPtr->ChangeWindowTab(_XDEF_MAINFRAME_COMMUNITYTAB);
						m_pMainFrameWindowPtr->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( m_pMainFrameWindowPtr );
					}
				}
			}
			return true;
		case DIK_A :
			{
				BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
				if(  freecameramode )
				{
					D3DXVECTOR3 targetpos;
					D3DXMATRIX matOrientation;			
					targetpos.x = -g_FreeMoveModeSpeed;
					targetpos.y = 0.0f;
					targetpos.z = 0.0f;
					D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
					D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );
					
					g_pLocalUser->m_Position.x += targetpos.x;
					g_pLocalUser->m_Position.y += targetpos.y;
					g_pLocalUser->m_Position.z += targetpos.z;
					
					_XOBBData* pOBBData = NULL;
					
					FLOAT currentheight = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );
					if( g_pLocalUser->m_Position.y < currentheight )
						g_pLocalUser->m_Position.y = currentheight;
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition.x += (targetpos.x*0.5f);
					g_pLocalUser->m_TargetPosition.z += (targetpos.z*0.5f);
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition;
					g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
					g_pLocalUser->m_PathNodeCount = 0;
					g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;
					
					g_pLocalUser->m_pGoundCollideOBB = pOBBData;
					
					g_LodTerrain.m_3PCamera.m_TargetPosition = 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel();
					
					g_pLocalUser->SetAnimation(_XACTION_MOVE);

					if( g_pMinimap_Window )
					{
						// Set direction to minimap arrow 
						g_pMinimap_Window->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
						// Set minimap status
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );				
				}
				else
				{
					g_pLocalUser->SetViewTargetToAttackTarget();
				}
			}
			return true;

		case DIK_U :
			{
				// 자신을 타겟으로 잡기
				if(g_pLocalUser->m_AttackTargetObjectType != _XPICKINGTARGET_SELF)
				{
					g_pLocalUser->SetViewTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
					g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
				}
			}
			return true;

		case DIK_H :
			{	
	#ifdef _XDEF_EVENTWINDOW		
				// 이벤트 창 띄우기
				_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
				if( pEvent_Window )
				{
					pEvent_Window->ShowWindow( !pEvent_Window->GetShowStatus() );
				}
	#endif
			}
			return true;

		case DIK_BACKSLASH :
			{
	#ifdef _XDWDEBUG
				_XWindow_Debug* debugwindow = (_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );	
				if( debugwindow )
				{
					debugwindow->ShowWindow( !debugwindow->GetShowStatus() );
				}
	#endif
			}
			return true;
			
		case DIK_GRAVE :
			{
				if(gpInput->CheckKeyPress(DIK_LCONTROL))
				{
					// Quickslot 현재 10칸 clear
					if(g_pQuickSlot_Window)
						g_pQuickSlot_Window->ResetQuickSlotCurrentSet();
				}
				else
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_QS_NEXTBUTTON, 0), (LPARAM)gHWnd);
				}
			}
			return true;
		case DIK_X : 
			{				
	#ifdef _XDEF_WEBBOARD_20061211
            #ifdef _XVIETNAMESE //Author : 양희왕 //breif : 베트남 게시판 추가
                if(!bCtrlKey)
                    return true;
            #endif

				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				if( pWebBoard_Window  )
				{
					pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
					
					if( pWebBoard_Window->GetShowStatus() )
					{
						
						int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
						
						TCHAR tempStr[256];
						memset( tempStr, 0, sizeof(TCHAR)*256 );

		            #ifdef _XVIETNAMESE //Author : 양희왕 //breif : 베트남 게시판 추가                        
						_snprintf( tempStr, sizeof(tempStr), "http://9dingame.zing.vn/9d/");
					#else
						if( !g_bTestWebPageAddress )
						{
							_snprintf( tempStr, sizeof(tempStr), "http://nine.nexon.com/community/game_bbs/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s"
								,servernumber //서버번호 game_server=%d
								,g_pLocalUser->m_CharacterInfo.innerlevel //케릭터레벨 game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //성별 game_sex=%d
								,g_pLocalUser->m_CharacterInfo.cClass // 클래스 문파레벨 추후에 구현되면 추가 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //영웅단이름 game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //영웅단레벨 추후에 구현되면 추가  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //문파 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //캐릭터이름 game_char=%s
								,g_pLocalUser->m_UserName//넥슨아이디 user_game_id=%s
								);
						}
						else
						{						
							_snprintf( tempStr, sizeof(tempStr), "http://9dtest.nexon.com/_temp/bbs/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s"
								,servernumber //서버번호 game_server=%d
								,g_pLocalUser->m_CharacterInfo.innerlevel //케릭터레벨 game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //성별 game_sex=%d
								,g_pLocalUser->m_CharacterInfo.cClass // 클래스 문파레벨 추후에 구현되면 추가 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //영웅단이름 game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //영웅단레벨 추후에 구현되면 추가  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //문파 munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //캐릭터이름 game_char=%s
								,g_pLocalUser->m_UserName//넥슨아이디 user_game_id=%s
								);
						}
						#endif
						
						DisplayHTMLPage(g_hIWebBoardWebBrowserObject, tempStr);
						
						
						g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
						POINT pt = { 0, 0 };
						ClientToScreen( gHWnd, &pt );
						SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
						
						ShowWindow(g_hWebBoardWindow, SW_SHOW);
						g_bShowItemMallWindow = TRUE;
					}
					else
					{
						ShowWindow(g_hWebBoardWindow, SW_HIDE );
						g_bShowItemMallWindow = FALSE;
					}
				}
	#endif			
			}
			return true;

		case DIK_G :
			{
#ifdef _XDEF_AGEGRADE_070607_MAGVIPER //Author : 양희왕 //breif : 이용등급
				_XWindow_AgeGrade* pAgeGradeWindow = _XWindow_AgeGrade::GetInstance();
				if( !pAgeGradeWindow->GetShowStatus()) 
					pAgeGradeWindow->ShowWindow(TRUE);
				else
					pAgeGradeWindow->SetTime( g_LocalSystemTime );
#endif
			}
			return true;

	#ifdef _XDWDEBUG		
//		case DIK_APOSTROPHE : // Mini map window
//			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CHATDEFAULT_RESIZEBUTTON, 0), (LPARAM)gHWnd);
//			return true;		
		

		case DIK_Z : 
			{	
			}
			return true;	
	#endif		
		}	
		
		return false; 

	}
#endif

}

bool XProc_MainGame::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{ 	
	// Get the pick ray from the mouse position	
	POINT mousept = { pPos->x, pPos->z };
	GetVectorFromMousePos( mousept, g_vPickRayDir, g_vPickRayOrig );
	g_vPickInfinityRayDir = g_vPickRayDir * 1000.0f;

	//////////////////////////////////////////////////////////////////////////	
	// Check portal hovering
	int index = 0;
	_XArea* currentarea = NULL;
	g_CurrentHoveredWarpzoneIndex = -1;
	list <_XArea *>::iterator iter;
	for(iter = g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.begin() ; iter != g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.end() ; iter++, index++)
	{
		currentarea = *iter;
		
		// 같은존 같은 리젼이면 스킵 (다른 지역에서 들어오는 영역)
		if( !(g_CurrentZoneInfoIndex == (currentarea->m_TargetZone + 1) &&	currentarea->m_TargetWarpzone == currentarea->m_Num) )
		{
			D3DXVECTOR3 pickedpoint;
			if( g_AreaManager.m_PortalOBBDataList[index].InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir,  pickedpoint ) )
			{
				FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - pickedpoint.x);
				FLOAT dy = ( g_LodTerrain.m_3PCamera.m_CameraPosition.y - pickedpoint.y);
				FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - pickedpoint.z);
				FLOAT _distance = _XFC_sqrt( dx*dx + dy*dy + dz*dz );
				
				if( ( _distance > 0.0f ) && ( _distance < 50.0f ) )
				{
					g_CurrentHoveredWarpzoneIndex = currentarea->m_Num;					
				}				
			}
		}
	} // EOB : Check portal hovering

	if( !::g_CurrentFocusedObject || ::g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND )
	{	
		float fmx = pState->AX*0.5*m_fMoveFactor;
		float fmy = pState->AY*0.5*m_fMoveFactor;

		if(g_pLocalUser->m_Lock.camerareverse == 1)
		{
			fmx *= -1.0f;
			fmy *= -1.0f;
		}

		if(!m_bNPCDialogMode && !m_bFunctionalObjectWideView && g_pLocalUser->m_Lock.camerarotatelock == 0)
		{
			// insert NPC 대화 모드/상태효과 - 카메라 회전 불가능 일때는 마우스피킹을 막음
			if( pState->bButton[1] )
			{	
				// Camera rotation...

				g_LodTerrain.m_3PCamera.AddPitch(fmy);
				g_LodTerrain.m_3PCamera.AddYaw(fmx);

				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();		

				// Rebuild loose quadtree...
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
				g_LodTerrain.RebuildLevel();			

				if( g_pMinimap_Window )
				{
					// Set direction to minimap arrow 
					g_pMinimap_Window->SetRotateFrustum( _X_RAD( 180 - g_LodTerrain.m_3PCamera.GetYaw() ) );
				}

				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_CROSSPOINTER );			
			}
		}
	}
	
	return true; 
}

bool XProc_MainGame::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{	
	if( ::g_CurrentFocusedObject && ::g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND  )
	{		
		return true;
	}
	
	switch( p_MB )
	{
	case _XmbLeft :
		g_LodTerrain.m_3PCamera.SetDefaultViewMode( FALSE );
		if( bPushed )
		{
#ifdef _XDEF_PARTY_NOTAVAILABLE
			m_dwLastInputTime = g_LocalSystemTime;		// 키 입력된 시간 저장
#endif
		}		
		else
		{
			// 지형의 픽킹 재계산 방지 플래그를 꺼줌.
			g_MouseFirstClickPassed = FALSE;
		}
		break;	
	case _XmbRight :

		g_LodTerrain.m_3PCamera.m_AutoBackTrace = FALSE;
		g_LodTerrain.m_3PCamera.SetDefaultViewMode( FALSE );

		if( bPushed )
		{				
			((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_CROSSPOINTER );

			// 창이 닫힌후 mouse에 붙은 icon 없애기
			if(g_pInventory_Window)
			{
				if(!g_pInventory_Window->GetShowStatus())
				{
					if(g_pInventory_Window->m_bHaveMouseSlot)
					{
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
			if(m_pNPCTradeWindowPtr)
			{
				if(!m_pNPCTradeWindowPtr->GetShowStatus())
				{
					if(m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
					{
						m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
			if(m_pPCTradeWindowPtr)
			{
				if(!m_pPCTradeWindowPtr->GetShowStatus())
				{
					if(m_pPCTradeWindowPtr->m_bHaveMouseSlot)
					{
						m_pPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
			if(m_pWarehouseWindowPtr)
			{
				if(!m_pWarehouseWindowPtr->GetShowStatus())
				{
					if(m_pWarehouseWindowPtr->m_bHaveMouseSlot)
					{
						m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
			if(m_pSocketPlugInWindowPtr)
			{
				if( !m_pSocketPlugInWindowPtr->GetShowStatus() )
				{
					if( m_pSocketPlugInWindowPtr->m_bHaveMouseSlot )
					{
						m_pSocketPlugInWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pSocketPlugInWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
			if(m_pSocketPlugInExWindowPtr)
			{
				if( !m_pSocketPlugInExWindowPtr->GetShowStatus() )
				{
					if( m_pSocketPlugInExWindowPtr->m_bHaveMouseSlot )
					{
						m_pSocketPlugInExWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pSocketPlugInExWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
			if(m_pMainFrameWindowPtr)
			{
				if(!m_pMainFrameWindowPtr->GetShowStatus() || m_pMainFrameWindowPtr->GetCurrentSelectedTab() != _XDEF_MAINFRAME_SKILLTAB)
				{
					g_pSkill_Window->ResetMouseSlot();
				}
			}
			if(m_pGambleBoxWindowPtr)
			{
				if( !m_pGambleBoxWindowPtr->GetShowStatus() )
				{
					if( m_pGambleBoxWindowPtr->m_bHaveMouseSlot )
					{
						m_pGambleBoxWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pGambleBoxWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
		}
		
		g_CurrentFocusedObject = NULL;
		break;
	case _XmbMiddle :
		
		g_LodTerrain.m_3PCamera.m_AutoBackTrace = FALSE;
		g_CurrentFocusedObject = NULL;
		break;
	}
	
	return true; 
}	

void XProc_MainGame::OnMouseWheel( short zDelta )
{
	if( !g_MainWindowManager.ProcessWheelMessage(zDelta) )
	{
		FLOAT MoveDelta  = (FLOAT)zDelta*0.002F;

		if(g_pLocalUser->m_Lock.camerareverse == 1)
			MoveDelta *= -1.0f;

		if( gpInput->CheckKeyPress( DIK_LCONTROL ) ) MoveDelta *= _XDEF_RAPID_RATE;

		BOOL bTraining = FALSE;
		if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING && g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
			bTraining = TRUE;
		
		if(!m_bNPCDialogMode && !bTraining && g_pLocalUser->m_Lock.camerazoomlock == 0)
		{
			// NPC 대화 모드/운기조식 미니게임/상태효과 카메라 줌불가능 중일때는 마우스휠작용을 막음
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - MoveDelta );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );

			g_pLocalUser->m_ModelDescriptor.m_CameraDistance = g_LodTerrain.m_3PCamera.GetDistance();
		}
	}
}

void XProc_MainGame::OnMouseLBtnDblClick( WPARAM wparam )
{
	// remove Tooltip
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )
		{
			if( g_pDefaultTooltip_Window->CheckMousePosition() )
			{
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
				return;
			}
		}
	}

	// 인벤토리 더블클릭
	if(g_pInventory_Window)
	{
		if(g_pInventory_Window->GetShowStatus() )
		{
			if( g_pInventory_Window->CheckMousePosition() )
			{
				g_pInventory_Window->ProcessDblClick();
				return;
			}
		}
	}
	
	// NPC Trade 창 더블클릭
	if(m_pNPCTradeWindowPtr)
	{
		if(m_pNPCTradeWindowPtr->GetShowStatus() && m_pNPCTradeWindowPtr->CheckMousePosition())
		{
			if( m_pNPCTradeWindowPtr->CheckMousePosition() )
			{
				m_pNPCTradeWindowPtr->ProcessDblClick();
				return;
			}
		}
	}
	
	// Warehouse 창 더블 클릭
	if(m_pWarehouseWindowPtr)
	{
		if( m_pWarehouseWindowPtr->GetShowStatus() )
		{
			if( m_pWarehouseWindowPtr->CheckMousePosition() )
			{
				m_pWarehouseWindowPtr->ProcessDblClick();
				return;
			}
		}
	}
	
	// PvP 창 더블 클릭
	if(m_pPCTradeWindowPtr)
	{
		if(m_pPCTradeWindowPtr->GetShowStatus() && m_pPCTradeWindowPtr->CheckMousePosition())
		{
			if( m_pPCTradeWindowPtr->CheckMousePosition() )
			{
				m_pPCTradeWindowPtr->ProcessDblClick();
				return;
			}
		}
	}

	// Skill 창 더블 클릭
	if(g_pSkill_Window)
	{
		if(m_pMainFrameWindowPtr)
		{
			if(m_pMainFrameWindowPtr->GetShowStatus() && m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_SKILLTAB)
			{
				if( g_pSkill_Window->CheckMousePosition() )
				{
					g_pSkill_Window->ProcessDblClick();
					return;
				}
			}
		}
	}

	// CInfo 창 더블 클릭
	if( m_pMainFrameWindowPtr )
	{
		if( m_pMainFrameWindowPtr->GetShowStatus() )
		{
			if(m_pMainFrameWindowPtr->GetCurrentSelectedTab() == _XDEF_MAINFRAME_CHARACTORTAB)
			{
				if( g_pCInfo_Window->CheckMousePosition() )
				{
					g_pCInfo_Window->ProcessDblClick();
					return;
				}
			}
		}	
	}

	// NPC스킬창 더블클릭 처리
	if(m_pNPCSkillWindowPtr && g_pSkill_Window)
	{
		if(m_pNPCSkillWindowPtr->GetShowStatus())
		{
			if( m_pNPCSkillWindowPtr->CheckMousePosition() )
			{
				m_pNPCSkillWindowPtr->ProcessDblClick();
				return;
			}
		}
	}

	// Socket 창 더블 클릭
	if( m_pSocketPlugInWindowPtr )
	{
		if( m_pSocketPlugInWindowPtr->GetShowStatus() )
		{
			if( m_pSocketPlugInWindowPtr->CheckMousePosition() )
			{
				m_pSocketPlugInWindowPtr->ProcessDblClick();
				return;
			}
		}
	}

	if( m_pGambleBoxWindowPtr )
	{
		if( m_pGambleBoxWindowPtr->GetShowStatus() )
		{
			if( m_pGambleBoxWindowPtr->CheckMousePosition() )
			{
				m_pGambleBoxWindowPtr->ProcessDblClick();
				return;
			}
		}
	}

	if( m_pSocketPlugInExWindowPtr )
	{
		if( m_pSocketPlugInExWindowPtr->GetShowStatus() )
		{
			m_pSocketPlugInExWindowPtr->ProcessDbClick();
			return;
		}
	}
	
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	if(pMessenger_Window )
	{
		if(pMessenger_Window->GetShowStatus())
		{
			if( pMessenger_Window->CheckMousePosition() )
			{
				pMessenger_Window->ProcessDblClick();
				return;
			}
		}
	}
	if(g_pMinimap_Window )
	{
		if(g_pMinimap_Window->GetShowStatus())
		{
			if( g_pMinimap_Window->CheckMousePosition() )
			{
				g_pMinimap_Window->ProcessDblClick();
				return;
			}
		}
	}

	if( g_pLocalUser->m_InParty )
	{		
		for( int partywindowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 ; partywindowid < _XDEF_WTITLE_MINIPARTY_MEMBER1+9 ; partywindowid++ )
		{
			if( partywindowid != _XDEF_WTITLE_MINIPARTY_MEMBER1 )
			{
				_XWindow_PartyMemberMiniBar* pMemberMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(partywindowid);
				if(pMemberMiniBar_Window)
				{
					if(  pMemberMiniBar_Window->GetShowStatus() )
					{
						if( pMemberMiniBar_Window->CheckMousePosition() )
						{
							pMemberMiniBar_Window->ProcessDblClick();
							return;
						}
					}
				}
			}
			else
			{
				_XWindow_PartyMasterMiniBar* pMasterMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(partywindowid);
				if(pMasterMiniBar_Window)
				{
					if(  pMasterMiniBar_Window->GetShowStatus() )
					{
						if( pMasterMiniBar_Window->CheckMousePosition() )
						{
							pMasterMiniBar_Window->ProcessDblClick();
							return;
						}
					}
				}
			}
		}			
	}
	
	for( int windowid = 60000; windowid < 60050; windowid++)
	{
		_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
		if(pMessengerDialog_Window )
		{
			if(pMessengerDialog_Window->GetShowStatus())
			{
				if( pMessengerDialog_Window->CheckMousePosition() )
				{
					pMessengerDialog_Window->ProcessDblClick();
					return;
				}
			}
		}
	}
	
	
}

void XProc_MainGame::OnMouseRBtnDblClick( WPARAM wparam )
{	
	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING && g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
		return;

	if( !g_LastPickedObject ) 
	{		
		_XWindow_RequestParty* ptrackpopupmenu = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REQUESTPARTY );
		if( ptrackpopupmenu )
		{
			ptrackpopupmenu->ShowWindow( FALSE );
		}
		return;
	}

	if( g_LastPickedObjectType == _XPICKINGTARGET_PC )
	{	
		_XUser* pUser = ((_XUser*)g_LastPickedObject);
		
		if( pUser )
		{
			if(pUser->m_BattleMode != _XBATTLE_PK)
			{
				D3DXVECTOR3 rvec;
				D3DXVECTOR3 mpos = pUser->m_Position;		
				mpos.y = pUser->m_Position.y + 1.5;
				D3DXVec3Project( &rvec, &mpos, &g_MainViewPort,&g_LodTerrain.m_3PCamera.GetProjMatrix(), 
					&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL );
				
				_XWindow_RequestParty* ptrackpopupmenu = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REQUESTPARTY );
				if( ptrackpopupmenu )
				{
					ptrackpopupmenu->m_LastPickedObjectPosition = pUser->m_Position;
					ptrackpopupmenu->m_LastPickedObjectPosition.y += 1.5f;			
					ptrackpopupmenu->MoveWindow( rvec.x, rvec.y );
					ptrackpopupmenu->ShowWindow( TRUE );
					strcpy(ptrackpopupmenu->m_PickedCharacterName, pUser->m_CharacterInfo.charactername);
					ptrackpopupmenu->m_PickedCharacterID = pUser->m_Uniqueid;

					// 가입권유버튼 처리
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass >= _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
						ptrackpopupmenu->m_RequestGroupButton->EnableWindow(TRUE);
					else
						ptrackpopupmenu->m_RequestGroupButton->EnableWindow(FALSE);
					
					// 상대가 복면 상태일때 처리
					if( pUser->m_bFaceMaskMode )
						ptrackpopupmenu->m_SendMessageButton->EnableWindow( FALSE );
					else
						ptrackpopupmenu->m_SendMessageButton->EnableWindow( TRUE );
				}			
			}
		}
	}
}

void XProc_MainGame::OnMouseMBtnDblClick( WPARAM wparam )
{
	g_LodTerrain.m_3PCamera.SetDefaultViewMode( TRUE );
}

void XProc_MainGame::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	//if( m_MoveServer ) return;

	if (WSAGETASYNCERROR(lparam) != 0) 
	{
		int errorcode = WSAGetLastError();

		if( errorcode == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : MGGS : TCP I/O blocking");
			Sleep( 50 );
		}
		else if( !m_MoveServer )
		{
			if( errorcode != ERROR_IO_PENDING )
			{
				g_NetworkKernel.ProcessNetworkError();
			}
			else
			{
				_XLog( "WARNING : MGGS : TCP I/O pending" );
				Sleep(50);
			}
			
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
			m_CancleGameGuardAuth = TRUE;
#endif

			_XUpdateWindow();
			return;
		}
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{	
	case FD_CONNECT:	
				
		WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
		g_NetworkKernel.m_bConnected = TRUE;
				
		g_pLocalUser->ResetZoneData();
		m_MoveServer = FALSE;
		
		_XLog( "Initiated on socket completed.[Gs]" );
		break;
	case FD_READ :
		if( !m_MoveServer )
			 g_NetworkKernel.ReadSocket();
		break;
	case FD_CLOSE :
		if( m_MoveServer )
		{			
			/*g_NetworkKernel.InitializeNetwork();
			
			if( g_NetworkKernel.ConnectServer() )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, IDS_LOGINGAMESERVER,
					TRUE, 0, 0, _XDEF_LS_CANCELLOGINSERVER );
				g_MessageBox.SetMessageColor();				
			}			
			else
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, IDS_CANNOTCONNECTEGAMESERVER,
					TRUE, 0, 0, 0 );
				g_MessageBox.SetMessageColor();
			}*/
			_XDWINPRINT( "SERVER MESSAGE : Socket close message <Move server>" );
			
			int ret = WSAGetLastError();
			_XLog( "Socket has been closed. [Gs:MtS,_ECode:%d]", ret );
		}		
		else
		{
			/* 
			//이미 Close message가 발생했으므로 IO pending 처리 불가.
			int errorcode = WSAGetLastError();

			if( errorcode != ERROR_IO_PENDING ) 
			{
				g_NetworkKernel.ProcessNetworkError();
				_XDWINPRINT( "SERVER MESSAGE : Socket close message" );
			}
			else
			{
				_XLog( "SERVER MESSAGE : I/O Pending [E_Code:%d]", errorcode );
			}
			*/
			int ret = WSAGetLastError();
			_XLog( "Socket has been closed. [Gs:_ECode:%d]", ret );
		}

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
			m_CancleGameGuardAuth = TRUE;
#endif
		
		break;
	}	
}

void XProc_MainGame::OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		int errorcode = WSAGetLastError();

		if( errorcode == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : MGMS : TCP I/O blocking");
			Sleep( 50 ); 
		}
		else
		{
			if( errorcode != ERROR_IO_PENDING )
			{
				if( !g_NetworkKernel.m_bConnected )
				{
					g_NetworkKernel.ProcessMessengerNetworkError();
				}
			}
			else
			{
				_XLog( "WARNING : MGMS : TCP I/O pending" );
				Sleep( 50 );
			}
			return;
		}
	}
	
	switch (WSAGETSELECTEVENT(lparam)) 
	{
	case FD_CONNECT:
		WSAAsyncSelect(g_NetworkKernel.m_hMessengerSocket, gHWnd, WM_USER_MESSENGERSOCKET, FD_READ | FD_CLOSE );
		g_NetworkKernel.m_bMessengerServerConnected = TRUE;
		_XDWINPRINT( "Connected Messenger server" );
		
		//		m_MessengerConnectTimer = 0;
		//		g_NetworkKernel.m_ConnectTryCount = 0;
		//		g_NetworkKernel.SendMessengerPacket(MMSG_NO_LOGIN_C_S); // 캐릭터 이름으로 메신저 서버에 접속		
		break;
		
	case FD_READ :
		g_NetworkKernel.ReadMessengerPacket();
		break;
	case FD_CLOSE :
		g_NetworkKernel.ProcessMessengerNetworkError();
		m_MessengerLoginTimer = 0;
		//DWORD err = GetLastError() ;
		_XDWINPRINT( "SERVER MESSAGE : Messenger Socket close message" );
		break;
	}
}

#ifdef _XADMINISTRATORMODE
void XProc_MainGame::ProcRenderStateCommand( LPSTR commandstring )
{
	m_RenderStateController.ProcCommand( commandstring );	
}
#endif


void XProc_MainGame::DrawFadeInOut(void)
{
	D3DCOLOR color = ((DWORD) g_FadeInOutLevel << 24);
	g_ScreenBurnOut[0].color = color;
	g_ScreenBurnOut[1].color = color;
	g_ScreenBurnOut[2].color = color;
	g_ScreenBurnOut[3].color = color;
	
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	
	gpDev->SetTexture(0, NULL);
	
	gpDev->SetFVF(D3DFVF_XTLVERTEX);
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX));
	
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	gpDev->SetRenderState(D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog);
}

#define _XDEF_TEXTUREHITRATE_CHECKTIME1	300000
#define _XDEF_TEXTUREHITRATE_CHECKTIME2	650000
#define _XDEF_MESHHITRATE_CHECKTIME		500000
DWORD	g_CheckTextureHitrateIndex1 = 0;
DWORD	g_CheckTextureHitrateIndex2 = 0;

void XProc_MainGame::CheckTextureUseHitrate( void )
{
	if( (g_LocalSystemTime - m_LastTextureHitrateCheckTime1) > _XDEF_TEXTUREHITRATE_CHECKTIME1 )
	{
		switch(g_CheckTextureHitrateIndex1) 
		{		
		case 0:
			g_TerrainTextureArchive.CheckTextureUseHitRate();
			break;
		case 1:
			g_WorldObjectTextureArchive.CheckTextureUseHitRate();	
			break;
		case 2:
			g_MainInterfaceTextureArchive.CheckTextureUseHitRate();	
			break;
		}
		g_CheckTextureHitrateIndex1++;
		if( g_CheckTextureHitrateIndex1 > 3 ) g_CheckTextureHitrateIndex1 = 0;
			
		m_LastTextureHitrateCheckTime1 = g_LocalSystemTime;
	}

	if( (g_LocalSystemTime - m_LastTextureHitrateCheckTime2) > _XDEF_TEXTUREHITRATE_CHECKTIME2 )
	{
		switch(g_CheckTextureHitrateIndex2) 
		{		
		case 0:
			g_EffectTextureArchive.CheckTextureUseHitRate();	
			break;
		case 1:
			g_CharacterTextureArchive.CheckTextureUseHitRate();	
			break;
		case 2:
			g_MobTextureArchive.CheckTextureUseHitRate();	
			break;		
		}	

		g_CheckTextureHitrateIndex2++;
		if( g_CheckTextureHitrateIndex2 > 3 ) g_CheckTextureHitrateIndex2 = 0;

		m_LastTextureHitrateCheckTime2 = g_LocalSystemTime;
	}

#ifdef _XDEF_MODELTHREADLOADING
	if( (g_LocalSystemTime - m_LastMeshHitrateCheckTime) > _XDEF_MESHHITRATE_CHECKTIME )
	{
		int i;

		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_FACE; ++i )  g_UserModel_FaceType[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_UBODY; ++i ) g_UserModel_UBodyType[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LBODY; ++i ) g_UserModel_LBodyType[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LHAND; ++i ) g_UserModel_LHandType[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_RHAND; ++i ) g_UserModel_RHandType[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LFOOT; ++i ) g_UserModel_LFootType[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_RFOOT; ++i ) g_UserModel_RFootType[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_CAP; ++i ) g_UserModel_Cap[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_ADAE; ++i ) g_UserModel_Adae[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_EARRING; ++i ) g_UserModel_Earring[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_BACKPACK; ++i ) g_UserModel_BackPack[i]->CheckLastUsedTime();	
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_HAIR; ++i ) g_UserModel_HairType[i]->CheckLastUsedTime();

		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_MASK; ++i ) g_UserModel_Mask[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_USERMODELTYPECOUNT_FACEMASK; ++i ) g_UserModel_FaceMask[i]->CheckLastUsedTime();	
		
		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_WEAPON; ++i ) 
		{
			g_ItemModel_Weapon[0][i]->CheckLastUsedTime();
			g_ItemModel_Weapon[1][i]->CheckLastUsedTime();
		}

		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_MARTIALBOOK; ++i ) g_ItemModel_MartialBook[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_POTION; ++i ) g_ItemModel_Potion[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_THROW; ++i ) g_ItemModel_Throw[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_SOCERY; ++i ) g_ItemModel_Socery[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_ETC; ++i ) g_ItemModel_ETC[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_JEWELBOX; ++i ) g_ItemModel_JewelBox[i]->CheckLastUsedTime();
		for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_ELIXIR; ++i) g_ItemModel_Elixir[i]->CheckLastUsedTime();	

		m_LastMeshHitrateCheckTime = g_LocalSystemTime;
	}	
#endif

}

void XProc_MainGame::DrawQuestWideView(void)
{
	m_TopScreenVertex[2].y += 1.0f;
	if(m_TopScreenVertex[2].y >= 92.0f)
		m_TopScreenVertex[2].y = 92.0f;
	
	m_TopScreenVertex[3].y += 1.0f;
	if(m_TopScreenVertex[3].y >= 92.0f)
		m_TopScreenVertex[3].y = 92.0f;
	
	m_BottomScreenVertex[0].y -= 3.0f;
	if(m_BottomScreenVertex[0].y <= 496.0f)
		m_BottomScreenVertex[0].y = 496.0f;
	
	m_BottomScreenVertex[1].y -= 3.0f;
	if(m_BottomScreenVertex[1].y <= 496.0f)
		m_BottomScreenVertex[1].y = 496.0f;
	
	gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );	// don't erase...
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetRenderState( D3DRS_ZENABLE,   D3DZB_FALSE );
	gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	
	gpDev->SetTexture(0, NULL);
	
	gpDev->SetFVF(D3DFVF_XTLVERTEX);
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));
	
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	gpDev->SetRenderState(D3DRS_DEPTHBIAS , 0);
	gpDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	
}

void XProc_MainGame::RepositionTopBottomVertex(void)
{
	m_TopScreenVertex[2].y = 0.0f;
	m_TopScreenVertex[3].y = 0.0f;
	m_BottomScreenVertex[0].y = (FLOAT)gnHeight;
	m_BottomScreenVertex[1].y = (FLOAT)gnHeight;
}


BOOL XProc_MainGame::SelectSkill(short skillid, BOOL keydown)
{
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_031 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);				
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_031);
				g_TutorialInfo->_XTutorial_031 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}
		}		

		if( !g_TutorialInfo->_XTutorial_033 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHARGESKILL )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_033);
				g_TutorialInfo->_XTutorial_033 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}					
		}
		
		if( !g_TutorialInfo->_XTutorial_034 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_FINISHSKILL )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_034);
				g_TutorialInfo->_XTutorial_034 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}					
		}
		
		if( !g_TutorialInfo->_XTutorial_035 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_ROUNDINGSPELL )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_035);
				g_TutorialInfo->_XTutorial_035 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}					
		}
		
		if( !g_TutorialInfo->_XTutorial_036 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CASTINGSPELL )
			{					
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_036);
				g_TutorialInfo->_XTutorial_036 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}
		}		
		
		if( !g_TutorialInfo->_XTutorial_037 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_037);
				g_TutorialInfo->_XTutorial_037 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}					
		}

		if( !g_TutorialInfo->_XTutorial_038 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_038);
				g_TutorialInfo->_XTutorial_038 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}					
		}
	}

	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2410), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return FALSE;
	}

	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_MATCHING || g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_BOW)
	{
		// 비무 준비중
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2411), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return FALSE;
	}

	if(g_pLocalUser->m_bMatchPRMode)
	{
		// 비무 홍보중
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2411), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return FALSE;
	}

	if( g_pLocalUser->m_bReadingBook )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2412), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return FALSE;
	}
	else if( g_pLocalUser->m_bInternalUse)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2413), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return FALSE;
	}
	
	// 노점 열고 있을 때
	if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2411), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return FALSE;
	}

	// 장원전 중 - 장주 협박 중
	if(g_pLocalUser->m_bThreatening)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2411), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return FALSE;
	}
	
	if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL || g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
	{
		if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
			(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// 내공심법 시전중
		{
			// Passive On/Off 불가능
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2414), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			return FALSE;
		}
		else
		{
			if(!g_pLocalUser->m_bWaitPassiveReq)
			{
				if(g_SkillProperty[skillid]->usefulType == g_pLocalUser->m_LastPassiveUsefulType &&
					g_SkillProperty[skillid]->skillSType == g_pLocalUser->m_LastPassiveSType)
				{
					if(g_pLocalUser->m_LastPassiveOnOffTime > 0)
					{
						if(g_LocalSystemTime - g_pLocalUser->m_LastPassiveOnOffTime >= 2000)
						{
							return ChangeToPassive(skillid);
						}
						else
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_DELAYTIME), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "아직 선택할 수 없습니다.(지연시간중)"
						}
					}
					else
					{
						return ChangeToPassive(skillid);
					}
				}
				else
				{
					return ChangeToPassive(skillid);
				}
			}

			return FALSE;
		}
	}

	if(g_pLocalUser->m_bUsingSkill)
	{
		// 스킬 시전 중
		if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHARGESKILL && g_pLocalUser->m_SelectedSkillID == skillid)//g_pLocalUser->m_ChargeIterateValue > 0)
		{
			// charge 중 - skill 예약이 아님
			if(!keydown)
			{
				// 마우스로 charge 상태에서 다시 클릭
				g_pLocalUser->m_bUseMouseChargeSkill = TRUE;
			}
		}
		else
		{
			_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(skillid);
			
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
			if(!pSkillItem)
			{
				if(skillid == g_pLocalUser->m_EventSkillList.m_sSkillID)
					pSkillItem = &g_pLocalUser->m_EventSkillList;
			}
#endif

			if(pSkillItem)
			{
				if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
					_XSkillItem::GetTargetType(skillid)/*g_SkillProperty[skillid]->targetType*/ == _XSI_TARGETTYPE_SELF &&
					g_SkillProperty[skillid]->skillSType == _XSI_STYPE_03)
				{
					// 경공일 때 Next skill 등록하지 않음
				}
				else
				{
					if(pSkillItem->m_dwCooldownStartTime <= 0)
					{
						// Cooldown time 종료
						if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || skillid == 0)
						{
							if(g_pLocalUser->m_SelectedSkillID != skillid)
							{
								if(CheckValidSkillActive(skillid))
								{
									g_pLocalUser->m_NextSkillID = skillid;
								}
							}
						}
						else
						{
							if(g_pLocalUser->m_SelectedSkillID != skillid) // 현재 스킬과 같지 않을 경우 예약
							{
								if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL)
								{
									if(CheckValidSkillChanneling(skillid))
										g_pLocalUser->m_NextSkillID = skillid;
								}
								else if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_HIDDENMA)
								{
									if(CheckValidSkillHiddenMA(skillid))
										g_pLocalUser->m_NextSkillID = skillid;
								}
								else if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL || g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
								{
									if(CheckValidSkillPassive(skillid))
										g_pLocalUser->m_NextSkillID = skillid;
								}
								else
								{
									if(CheckValidSkillOther(skillid))
										g_pLocalUser->m_NextSkillID = skillid;
								}
							}
						}
							
						return FALSE;
					}
					else
					{
						// Cool down이 종료되지 않았을때는 아무것도 하지않음
					}
				}
			}
		}
	}
	else
	{
		if(g_pLocalUser->m_UseSpeedSkill)
		{
			if(	g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
				_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF			 &&
				g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
			{
				if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL	 &&
					_XSkillItem::GetTargetType(skillid) == _XSI_TARGETTYPE_SELF				 &&
					g_SkillProperty[skillid]->skillSType == _XSI_STYPE_03 )	// 경공
				{
				}
				else
				{
					// 경공 중에 다른 스킬 사용하려면 경공 취소하고 쿨다운 적용
					g_pLocalUser->m_UseSpeedSkill = FALSE;
					g_pLocalUser->m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
					g_pLocalUser->SetAnimation(_XACTION_IDLE);
					g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
					g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
				}
			}

		}
		
		if(g_pLocalUser->m_bCheckDelayTime)
		{
			// Delaytime 적용중 - 스킬을 변경할 수 없음
			if(keydown)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_DELAYTIME), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // "아직 선택할 수 없습니다.(지연시간중)"
			}
			return FALSE;
		}
		else
		{
			// 바로 변경 가능
			if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || skillid == 0)
			{
				return ChangeToActive(skillid);
			}
			else if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL)
			{
				return ChangeToChanneling(skillid, keydown);
			}
			else if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_HIDDENMA)
			{
				return ChangeToHiddenMA(skillid, keydown);
			}
			else
			{
				return ChangeToOther(skillid, keydown);
			}
		}
	}

	return FALSE;
}

BOOL XProc_MainGame::ChangeToActive(short skillid)
{
	if(!CheckValidSkillActive(skillid))
		return FALSE;

	if(skillid == 0)
	{
		if(g_pLocalUser->m_SelectedSkillID > 0)
		{
			if(g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime > 0)
			{
				g_pLocalUser->ResetSkillAnimation();
			}
		}
		
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_1;
		g_pLocalUser->m_CurrentSelectedSkillType = _XSI_USEFULTYPE_ACTIVESKILL;
		g_pLocalUser->m_SelectedSkillItem = NULL;
		
		short prevskillid = g_pLocalUser->m_SelectedSkillID;
		short prevactiveslotid = g_pLocalUser->m_ActiveSlotID;
		g_pLocalUser->m_SelectedSkillID = skillid;

//Author : 양희왕
#ifdef _XDEF_SKILLDEALY_MODIFY
		g_pLocalUser->SetFinallySelectedSkillID( skillid );
		g_pLocalUser->SetFinallySelectedSkillItem( NULL );
#endif
//Last Updated : 07/03/22

		g_pLocalUser->m_ActiveSlotID = skillid;
		
		g_pLocalUser->UpdateLocalUserAttackLevel();
		
		short stancemotion = g_pLocalUser->GetStanceMotion(g_pLocalUser->m_SelectedSkillID);
		if(prevskillid != g_pLocalUser->m_SelectedSkillID)
		{
			g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
			g_NetworkKernel.SendPacket(MSG_NO_STANCE_MOTION_CHANGE, stancemotion);
		}
		
		if(prevactiveslotid != g_pLocalUser->m_ActiveSlotID && g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
		{
			// stance motion 보여주기
			g_pLocalUser->m_bHaveStance = TRUE;
			g_pLocalUser->m_StanceMotionFirst = TRUE;
		}
		
		return TRUE;
	}

	_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(skillid);
	if(pSkillItem)
	{
		if(g_pLocalUser->m_SelectedSkillID > 0)
		{
			if(g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime > 0)
			{
				g_pLocalUser->ResetSkillAnimation();
			}
		}

		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_1;
		g_pLocalUser->m_CurrentSelectedSkillType = _XSI_USEFULTYPE_ACTIVESKILL;
		g_pLocalUser->m_SelectedSkillItem = pSkillItem;

		short prevskillid = g_pLocalUser->m_SelectedSkillID;
		short prevactiveslotid = g_pLocalUser->m_ActiveSlotID;
		g_pLocalUser->m_SelectedSkillID = skillid;

//Author : 양희왕
#ifdef _XDEF_SKILLDEALY_MODIFY
		g_pLocalUser->SetFinallySelectedSkillID( skillid );
		g_pLocalUser->SetFinallySelectedSkillItem( pSkillItem );
#endif
//Last Updated : 07/03/22

		g_pLocalUser->m_ActiveSlotID = skillid;

		g_pLocalUser->UpdateLocalUserAttackLevel();

		short stancemotion = g_pLocalUser->GetStanceMotion(g_pLocalUser->m_SelectedSkillID);
		if(prevskillid != g_pLocalUser->m_SelectedSkillID)
		{
			g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
			g_NetworkKernel.SendPacket(MSG_NO_STANCE_MOTION_CHANGE, stancemotion);
		}

		// Quick slot이 바뀌지 않았을 경우 바꾸기
		if(g_pQuickSlot_Window)
			g_pQuickSlot_Window->SetCurrentSelectedActiveIndexFromSkillID(g_pLocalUser->m_SelectedSkillID);

		if(pSkillItem)
		{
			if(g_pLocalUser->m_CharacterInfo.Get_current_forcepower() < _XSkillItem::GetCostForce(g_pLocalUser->m_SelectedSkillID, pSkillItem->m_cSelectedSkillLevel))//g_SkillProperty[skillid]->detailInfo[pSkillItem->m_cSelectedSkillLevel].costForce)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1827), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
		}

		if(prevactiveslotid != g_pLocalUser->m_ActiveSlotID && g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
		{
			// stance motion 보여주기
			g_pLocalUser->m_bHaveStance = TRUE;
			g_pLocalUser->m_StanceMotionFirst = TRUE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL XProc_MainGame::ChangeToChanneling(short skillid, BOOL keydown)
{
	if(!CheckValidSkillChanneling(skillid))
	{
		return FALSE;
	}
	
	_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(skillid);

#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	if(!pSkillItem)
	{
		if(skillid == g_pLocalUser->m_EventSkillList.m_sSkillID)
			pSkillItem = &g_pLocalUser->m_EventSkillList;
	}
#endif

	if(pSkillItem)
	{
		if(_XSkillItem::GetTargetType(skillid) != _XSI_TARGETTYPE_SELF)
		{
			if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
			{
				g_pLocalUser->ResetAttackTarget();
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORPEACE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 평화모드에서는 무공을 사용할 수 없습니다.
				return FALSE;
			}
			else
			{
				if(g_pLocalUser->m_SelectedSkillID > 0)
				{
					if(g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						g_pLocalUser->ResetSkillAnimation();
					}
				}

				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_1;
			}
		}
		else
		{
			if(g_pLocalUser->m_SelectedSkillID > 0)
			{
				if(g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime > 0)
				{
					g_pLocalUser->ResetSkillAnimation();
				}
			}

			g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_1;
		}
		
		g_pLocalUser->m_CurrentSelectedSkillType = g_SkillProperty[skillid]->usefulType;
		g_pLocalUser->m_SelectedSkillID = skillid;
		g_pLocalUser->m_SelectedSkillItem = pSkillItem;
		
		g_pLocalUser->UpdateLocalUserAttackLevel();

		// Quick slot이 바뀌지 않았을 경우 바꾸기
		if(g_pQuickSlot_Window)
			g_pQuickSlot_Window->SetCurrentSelectedOtherIndexFromSkillID(g_pLocalUser->m_SelectedSkillID);
		
		return TRUE;
	}
	
	return FALSE;
}

BOOL XProc_MainGame::ChangeToHiddenMA(short skillid, BOOL keydown)
{

	if(!CheckValidSkillHiddenMA(skillid))
		return FALSE;

	short parentskillid = g_SkillProperty[skillid]->insideType;

	_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(parentskillid);
	if(pSkillItem)
	{
		if(g_pLocalUser->m_SelectedSkillID > 0)
		{
			if(g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime > 0)
			{
				g_pLocalUser->ResetSkillAnimation();
			}
		}
		
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_1;
				
		g_pLocalUser->m_CurrentSelectedSkillType = g_SkillProperty[skillid]->usefulType;
		g_pLocalUser->m_SelectedSkillID = skillid;
		g_pLocalUser->m_SelectedSkillItem = pSkillItem;
		
		g_pLocalUser->UpdateLocalUserAttackLevel();
				
		return TRUE;
	}
	
	return FALSE;
}

BOOL XProc_MainGame::ChangeToOther(short skillid, BOOL keydown)
{
	if(!CheckValidSkillOther(skillid))
		return FALSE;

	_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(skillid);
	if(pSkillItem)
	{
		if(g_pLocalUser->m_SelectedSkillID > 0)
		{
			if(g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime > 0)
			{
				g_pLocalUser->ResetSkillAnimation();
			}
		}

		if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
		{
			switch(g_SkillProperty[skillid]->usefulType)
			{
			case _XSI_USEFULTYPE_CHARGESKILL :
				{
					g_pLocalUser->m_ChargeStartTime = 0;
					g_pLocalUser->m_ChargeIterateValue = 0;
				}
				break;
			case _XSI_USEFULTYPE_ROUNDINGSPELL :
				{
					g_pLocalUser->m_RoundingDelayStartTime = 0;
					g_pLocalUser->ResetSpellAttackTarget();
				}
				break;
			case _XSI_USEFULTYPE_SMASHINGSKILL :
				{
					g_pLocalUser->m_SmashingSendPacket = FALSE;
				}
				break;
			case _XSI_USEFULTYPE_ACTIONSKILL :
				{
					g_pLocalUser->m_ActionSendPacket = FALSE;
				}
				break;
			case _XSI_USEFULTYPE_CHANNELINGSPELL :
				{
					g_pLocalUser->m_bChannelingSendStartPacket = FALSE;
					g_pLocalUser->m_ChannelingSendLastPacketTime = 0;
					g_pLocalUser->m_ChannelingSpellRepeatTime = 0;
					g_pLocalUser->m_RegionTargetPosX = 0.0f;
					g_pLocalUser->m_RegionTargetPosZ = 0.0f;
					g_pLocalUser->ResetSpellAttackTarget();
				}
				break;
			case _XSI_USEFULTYPE_CASTINGSPELL :
				{
					g_pLocalUser->ResetSpellAttackTarget();
				}
				break;
			}
		}
		
//		if(keydown)
		{
			g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_1;
		}

		g_pLocalUser->m_CurrentSelectedSkillType = g_SkillProperty[skillid]->usefulType;
		g_pLocalUser->m_SelectedSkillID = skillid;
		g_pLocalUser->m_SelectedSkillItem = pSkillItem;

//Author : 양희왕
#ifdef _XDEF_SKILLDEALY_MODIFY
		g_pLocalUser->SetFinallySelectedSkillID( skillid );
		g_pLocalUser->SetFinallySelectedSkillItem( pSkillItem );
#endif
//Last Updated : 07/03/22

		g_pLocalUser->UpdateLocalUserAttackLevel();

		// Quick slot이 바뀌지 않았을 경우 바꾸기
		if(g_pQuickSlot_Window)
			g_pQuickSlot_Window->SetCurrentSelectedOtherIndexFromSkillID(g_pLocalUser->m_SelectedSkillID);

		return TRUE;
	}

	return FALSE;
}

BOOL XProc_MainGame::ChangeToPassive(short skillid)
{
	int learnedindex = g_pLocalUser->GetLearnedSkillIndex(skillid);

	if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
	{
		// 수련 필요 check
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTCOMPLETETRAINING), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // "수련을 완료하지 않았으므로 사용할 수 없습니다."
		return FALSE;
	}
	
	if(g_SkillProperty[skillid]->skillSType == _XSI_STYPE_02)
	{
		// 내공심법은 내력 체크하지 않음
		g_NetworkKernel.SendPacket(MSG_NO_SKILL_CTRL_REQ, 2, skillid);
		g_pLocalUser->m_LastPassiveUsefulType = g_SkillProperty[skillid]->usefulType;
		g_pLocalUser->m_LastPassiveSType = g_SkillProperty[skillid]->skillSType;
		g_pLocalUser->m_LastPassiveOnOffTime = g_LocalSystemTime;

		return TRUE;
	}

	if(learnedindex >= 0 && g_pLocalUser->m_SkillList[learnedindex].m_cOnOff == 0)	// 켤때만 체크
	{
		if(_XSkillItem::GetCostForce(skillid, g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel)/*g_SkillProperty[skillid]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].costForce*/ > g_pLocalUser->m_CharacterInfo.Get_current_forcepower())
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_LACKFORCE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 내공이 부족해서 사용할 수 없습니다.
			return FALSE;
		}
	}

	g_NetworkKernel.SendPacket(MSG_NO_SKILL_CTRL_REQ, 2, skillid);
	g_pLocalUser->m_LastPassiveUsefulType = g_SkillProperty[skillid]->usefulType;
	g_pLocalUser->m_LastPassiveSType = g_SkillProperty[skillid]->skillSType;
	g_pLocalUser->m_LastPassiveOnOffTime = g_LocalSystemTime;
	return TRUE;
}

BOOL XProc_MainGame::CheckValidSkillActive(short skillid)
{
	if(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
	{
		if(g_pLocalUser->m_SelectedSkillID == skillid)
		{
			return FALSE;
		}
	}
	
	_XUserSkill* pSkillItem = NULL;
	
	if(skillid > 0)
		pSkillItem = g_pLocalUser->GetSkillItem(skillid);
	else
		pSkillItem = NULL;
	
	if(pSkillItem)
	{
		if(pSkillItem->m_ucSkillState == 1)
		{
			// 수련 필요 check
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTCOMPLETETRAINING), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // "수련을 완료하지 않았으므로 사용할 수 없습니다."
			return FALSE;
		}
	}
	
	if(!CheckReqCondition(skillid))
	{
		return FALSE;
	}
	if(!CheckReqItem(skillid))
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL XProc_MainGame::CheckValidSkillChanneling(short skillid)
{
	if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
	{
		if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			g_SkillProperty[skillid]->skillSType == _XSI_STYPE_03 )	// 경공
		{
			// 경공일때는 skip
		}
		else
		{
			g_pLocalUser->ResetAttackTarget();

			if(!g_pLocalUser->m_bMatchPRMode) // 비무 홍보일때 메세지 처리 안함.
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2415), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			return FALSE;
		}
	}

	_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(skillid);

#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	if(!pSkillItem)
	{
		if(skillid == g_pLocalUser->m_EventSkillList.m_sSkillID)
			pSkillItem = &g_pLocalUser->m_EventSkillList;
	}
#endif

	if(pSkillItem)
	{
		
		if( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL	  &&
			_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF				  &&
			g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03					  &&
			g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )	// 경공
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_1828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			return FALSE;
		}

		if(pSkillItem->m_dwCooldownStartTime > 0)
		{
			// Cooldown time check
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // "아직 사용할 수 없습니다.(재사용시간 종료 되지않음)"
			return FALSE;
		}
		else
		{
			if(pSkillItem->m_ucSkillState == 1)
			{
				// 수련 필요 check
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTCOMPLETETRAINING), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // "수련을 완료하지 않았으므로 사용할 수 없습니다."
				return FALSE;
			}
			else
			{
				if(_XSkillItem::GetCostForce(skillid, pSkillItem->m_cSelectedSkillLevel) > g_pLocalUser->m_CharacterInfo.Get_current_forcepower())
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_LACKFORCE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 내공이 부족해서 사용할 수 없습니다.
					return FALSE;
				}
				
				if(!CheckTargetType(skillid))
				{
					//Author : 양희왕 //breif : 레벨 패널티 일때와 아닐 떄
                    if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
                    {
                        _XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
                        if(!g_pLocalUser->CheckPKPenalty(pUser)) 
                        {//레벨 패널티가 아니면 들어온다.
                            g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                        }
                    }
                    else
                        g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

					return FALSE;
				}
				if(!CheckReqCondition(skillid))
				{
					return FALSE;
				}
				if(!CheckReqItem(skillid))
				{
					return FALSE;
				}
			}
		}

		return TRUE;
	}
	
	return FALSE;
}

BOOL XProc_MainGame::CheckValidSkillHiddenMA(short skillid)
{
	short parentskillid = g_SkillProperty[skillid]->insideType;
	
	_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(parentskillid);
	if(pSkillItem)
	{
		// Hidden은 내공 소모 체크 하지 않음
		
		if(!CheckTargetType(skillid))
		{
			//Author : 양희왕 //breif : 레벨 패널티 일때와 아닐 떄
            if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
            {
                _XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
                if(!g_pLocalUser->CheckPKPenalty(pUser)) 
                {//레벨 패널티가 아니면 들어온다.
                    g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                }
            }
            else
                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

			return FALSE;
		}
		if(!CheckReqCondition(skillid))
		{
			return FALSE;
		}
		if(!CheckReqItem(skillid))
		{
			return FALSE;
		}

		return TRUE;
	}
	
	return FALSE;
}

BOOL XProc_MainGame::CheckValidSkillOther(short skillid)
{
	if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
	{
		g_pLocalUser->ResetAttackTarget();
		if(!g_pLocalUser->m_bMatchPRMode) // 비무 홍보일때 메세지 처리 안함.
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2415), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		}
		return FALSE;
	}

	_XUserSkill* pSkillItem = g_pLocalUser->GetSkillItem(skillid);
	if(pSkillItem)
	{
		if(pSkillItem->m_dwCooldownStartTime > 0)
		{
			// Cooldown time check
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // "아직 사용할 수 없습니다.(재사용시간 종료 되지않음)"
			return FALSE;
		}
		else
		{
			if(pSkillItem->m_ucSkillState == 1)
			{
				// 수련 필요 check
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTCOMPLETETRAINING), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // "수련을 완료하지 않았으므로 사용할 수 없습니다."
				return FALSE;
			}
			else
			{
				if(_XSkillItem::GetCostForce(skillid, pSkillItem->m_cSelectedSkillLevel)/*g_SkillProperty[skillid]->detailInfo[pSkillItem->m_cSelectedSkillLevel].costForce*/ > g_pLocalUser->m_CharacterInfo.Get_current_forcepower())
				{
					switch(g_SkillProperty[skillid]->usefulType)
					{
					case _XSI_USEFULTYPE_CHARGESKILL :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1829), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
						break;
					case _XSI_USEFULTYPE_ROUNDINGSPELL :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1830), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
						break;
					case _XSI_USEFULTYPE_SMASHINGSKILL :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1831), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
						break;
					case _XSI_USEFULTYPE_CASTINGSPELL :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1832), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
						break;
					case _XSI_USEFULTYPE_FINISHSKILL :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1833), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
						break;
					case _XSI_USEFULTYPE_CHANNELINGSPELL :	
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1834), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						break;
					case _XSI_USEFULTYPE_ACTIONSKILL :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2734), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// 내공이 부족하여 무림잡기를 사용할 수 없습니다.
						break;
					}

					return FALSE;
				}
				
				if(!CheckTargetType(skillid))
				{
                    //Author : 양희왕 //breif : 레벨 패널티 일때와 아닐 떄
                    if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
                    {
                        _XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
                        if(!g_pLocalUser->CheckPKPenalty(pUser)) 
                        {//레벨 패널티가 아니면 들어온다.
                            g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                        }
                    }
                    else
                        g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

					return FALSE;
				}
				if(!CheckReqCondition(skillid))
				{
					return FALSE;
				}
				if(!CheckReqItem(skillid))
				{
					return FALSE;
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL XProc_MainGame::CheckValidSkillPassive(short skillid) // ChangeToPassive에서는 쓰지말것
{
	int learnedindex = g_pLocalUser->GetLearnedSkillIndex(skillid);
	
	if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
	{
		// 수련 필요 check
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTCOMPLETETRAINING), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // "수련을 완료하지 않았으므로 사용할 수 없습니다."
		return FALSE;
	}
	
	if(g_SkillProperty[skillid]->skillSType != _XSI_STYPE_02)
	{
		if(learnedindex >= 0 && g_pLocalUser->m_SkillList[learnedindex].m_cOnOff == 0)	// 켤때만 체크
		{
			if(_XSkillItem::GetCostForce(skillid, g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel)/*g_SkillProperty[skillid]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].costForce*/ > g_pLocalUser->m_CharacterInfo.Get_current_forcepower())
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_LACKFORCE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 내공이 부족해서 사용할 수 없습니다.
				return FALSE;
			}
		}	
	}
	
	return TRUE;
}

BOOL XProc_MainGame::CheckDelayTime(short skillid)
{
	if(g_pLocalUser->m_SelectedSkillID == skillid)
		return TRUE;

	if(g_pLocalUser->m_ChangeDelayStartTime == 0)
	{
		return TRUE;
	}
	else
	{
		int delaytime = 0;
		if(g_pLocalUser->m_SelectedSkillID > 0)
			delaytime = _XSkillItem::GetDelay(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->detailInfo[g_pLocalUser->m_SelectedSkillItem->m_cSkillLevel].delay;

		// skillid와 combination check

		if(g_LocalSystemTime - g_pLocalUser->m_ChangeDelayStartTime > delaytime)
		{
			g_pLocalUser->m_ChangeDelayStartTime = 0;
			return TRUE;
		}
	}

	return FALSE;
}

void XProc_MainGame::DrawNPCDialogWideView()
{
	if(g_LastNPCScriptOwner)
	{
		D3DCOLOR color = ((DWORD)m_NPCDialogScreenAlphaLevel << 24);
		g_ScreenBurnOut[0].color = color;
		g_ScreenBurnOut[1].color = color;
		g_ScreenBurnOut[2].color = color;
		g_ScreenBurnOut[3].color = color;

		// Fade in
		if( g_NPCDialogScreenEffectFlag )
		{
			m_NPCDialogScreenAlphaLevel+= _XDEF_NPCDIALOGMODE_FADE_LEVEL;

			if( m_NPCDialogScreenAlphaLevel >= 255 )
			{
				m_NPCDialogScreenAlphaLevel = 255;
				g_NPCDialogScreenEffectFlag = FALSE;

#ifdef _ACCLAIM_IGAADSYSTEM
				m_ADScreeenDimmedLevel = 128;
#endif

				m_vTempCameraPosition = g_LodTerrain.m_3PCamera.m_CameraPosition;
				m_vTempTargetPosition = g_LodTerrain.m_3PCamera.m_TargetPosition;
				m_fTempTargetDistance = g_LodTerrain.m_3PCamera.m_TargetDistance;
				m_fTempFov = g_LodTerrain.m_3PCamera.mp_fFov;
				m_fTempPitch = g_LodTerrain.m_3PCamera.mp_fPitch;
				m_fTempYaw = g_LodTerrain.m_3PCamera.mp_fYaw;

				m_NPCWideViewMove = -0.15f;
				m_NPCWindowMoveFast = 0;
				m_NPCWindowMoveSlow = 0;
				
				WindowCloseBeforeWideView(); 
				
				g_pLocalUser->m_CharacterLog.isTalk = TRUE;
				g_pLocalUser->m_CharacterLog.talkTime = g_ServerTimeCode;

				//g_pLocalUser->m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
				
				m_bNPCDialogMode = TRUE;	// 대화모드로 전환될 때 PC의 Rendering을 막는다. 
				
				// 퀘스트 시작조건에 NPC 타게팅 조건이 들어간 퀘스트 시작조건을 검사 
				g_QuestScriptManager.QuestStartConditionNPCTargettingMode();
				g_QuestScriptManager.Process();

#ifdef _XTS_HAVEQUESTLISTNPC
				// 타게팅 조건을 만족하는 퀘스트가 한개 이상으면 특별한 처리가 필요하다.
				// NPC 창에서 조건을 만족하는 퀘스트를 보여주고 선택해야 한다.				
				if( !g_QuestScriptManager.m_listNPCTargettingQuest.empty() )
				{
					bool bexist = false;
					list <_XQUEST_QUESTLIST>::iterator iter_npctargetting;
					for(iter_npctargetting = g_QuestScriptManager.m_listNPCTargettingQuest.begin() ; iter_npctargetting != g_QuestScriptManager.m_listNPCTargettingQuest.end() ;iter_npctargetting++ )
					{
						_XQUEST_QUESTLIST npctargetting = *iter_npctargetting;
						// 퀘스트 리스트에서 npcid가 동일 할 때만 리스트를 보여준다.
						if( g_LastNPCScriptOwner->m_UniqueID  == npctargetting.npcid )
						{
							bexist = true;
							break;
						}
					}

					if( bexist )
					{
						g_LastNPCScriptOwner->SetMessageType( _XDEF_NPCMESSAGE_HAVEQUESTLIST );
						g_LastNPCScriptOwner->SetChangeMessage(TRUE);
					}
					else
					{
						// 퀘스트 대사를 가지고 있는지 체크한다.
						if( g_LastNPCScriptOwner->m_HaveQuestType != _XNPC_HAVEQUEST_NONE )
						{
							g_LastNPCScriptOwner->m_bStartQuestDialog = TRUE;
							m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(FALSE);
							
						}				
					}
				}	
				else
				{
					// 퀘스트 대사를 가지고 있는지 체크한다.
					if( g_LastNPCScriptOwner->m_HaveQuestType != _XNPC_HAVEQUEST_NONE )
					{
						g_LastNPCScriptOwner->m_bStartQuestDialog = TRUE;
						m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(FALSE);

					}				
				}
#else
				// 퀘스트 대사를 가지고 있는지 체크한다.
				if( g_LastNPCScriptOwner->m_HaveQuestType != _XNPC_HAVEQUEST_NONE )
				{
					g_LastNPCScriptOwner->m_bStartQuestDialog = TRUE;
					m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(FALSE);
				}				
				
#endif
				g_LastNPCScriptOwner->m_pNPCScriptWindowPtr = m_pNPCScriptWindowPtr;
				g_LastNPCScriptOwner->SetNPCDialogMode(TRUE);
				

				// NPC 거래창 아이콘 셋팅
				m_pNPCTradeWindowPtr->SetNPCJobIcon(g_LastNPCScriptOwner->m_NPCJobClass);
				
				if( g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_HOTEL )
				{
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_048 )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_048);
							g_TutorialInfo->_XTutorial_048 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}
				}
				else if( g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_DRUGSTORE )
				{
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_049 )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_049);
							g_TutorialInfo->_XTutorial_049 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}
				}
				
				_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
				defaultsystemmsgwindow->ShowWindow(TRUE);
				defaultsystemmsgwindow->MoveWindow(gnWidth - 374, gnHeight-150);
				//defaultsystemmsgwindow->m_SystemMsgListScrollBar->SetWindowHeight(88);
				defaultsystemmsgwindow->m_SystemMsgListScrollBar->MoveWindow(gnWidth - 13 ,gnHeight - 137);
				defaultsystemmsgwindow->m_SystemMsgListScrollBar->SetLineScrollPos( 999999 ); 

				_XWindow_TutorialIcon* pTutorialIcon_Window = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_TUTORIALICON );
				if( pTutorialIcon_Window )
				{
					pTutorialIcon_Window->AllCloseExternalTutorialWindow();
				}				
			}
		
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );						
			gpDev->SetFVF( D3DFVF_XTLVERTEX );
			gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
					
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel();

			m_TopScreenVertex[2].y = 0.0f;
			m_TopScreenVertex[3].y = 0.0f;
			m_BottomScreenVertex[0].y = gnHeight;
			m_BottomScreenVertex[1].y = gnHeight;

		}
		else // Fade out
		{
			if( m_NPCDialogScreenAlphaLevel > 0 )
			{
				m_NPCDialogScreenAlphaLevel-= _XDEF_NPCDIALOGMODE_FADE_LEVEL;

#ifdef _ACCLAIM_IGAADSYSTEM
				if( m_UseNPCAD )
				{
					if( m_NPCDialogScreenAlphaLevel <= 0 )
					{
						if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NONE )
						{
							m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NPC;
							m_ADScreeenDimmedLevel = 128;
							g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_NPCDIALOG );
							g_IGAADWrapper.ShowWindow( TRUE );
							g_IGAADWrapper.StartDisplay();

							m_NPCDialogScreenAlphaLevel = 0;
						}
					}
				}
#endif
			}

			if( m_NPCDialogScreenAlphaLevel <= 0 )
			{				
				m_NPCDialogScreenAlphaLevel = 0;
				
				m_TopScreenVertex[2].y = 60; 
				m_TopScreenVertex[3].y = 60; 
				m_BottomScreenVertex[0].y = gnHeight - 150;
				m_BottomScreenVertex[1].y = gnHeight - 150;
				
				m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
				m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
				m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
				m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
				
				m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
				m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
				m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
				m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
				
				gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
				g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );

				gpDev->SetFVF(D3DFVF_XTLVERTEX);
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));

				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));
				
				// NPC 이름과 역할을 나타낸다.
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
					g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
					g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_NPCNameWindow.Draw( 3, 3, g_LastNPCScriptOwner->m_NPCRoll, g_LastNPCScriptOwner->m_NPCName, 
						FALSE, _XNPC_HAVEQUEST_NONE,  g_LastNPCScriptOwner->m_NPCJobClass, _XNPCNAMEMODE_NPC, _XFONT_ALIGNTYPE_LEFT );
				}
				else
				{
					g_NPCNameWindow.Draw( 85, 15, g_LastNPCScriptOwner->m_NPCRoll, g_LastNPCScriptOwner->m_NPCName, 
										  FALSE, _XNPC_HAVEQUEST_NONE,  g_LastNPCScriptOwner->m_NPCJobClass );		
				}

				D3DXVECTOR3 position;		// 카메라의 위치를 나타내는 변수
				float rad_fYaw, deg_fYaw;	// 카메라의 바라보는 각도를 나타내는 변수
				float sin_theta ,cos_theta; // theta = fYaw 카메라의 각도의 sin값, cos값
				float rad_temp, deg_temp;
			
				// Y축을 중심으로 theta만큼 회전한 4X4 매트릭스에서 
				// cos(theta)=_11, -sin(theta)=_13,
				// sin(theta)=_31, cos(theta)=_33, 으로 표현되므로
				sin_theta = g_LastNPCScriptOwner->m_matWorldPosition._31;
				cos_theta = g_LastNPCScriptOwner->m_matWorldPosition._11;

				// npc 정면을 바라보도록 카메라를 회전시키기 위한 각도를 구하는 부분
				// cos값이나 sin값을 알고 있으면 arccos이나 arcsin함수를 이용하여 각도를 알 수 있다
				rad_fYaw = acos(cos_theta); 
			
				//
				// 하지만 arccos함수를 이용하여 구한 값은 0 ~ 180 의  값이므로
				// 180도가 넘으면 음수가 되므로 판별해서 카메라의 각도를 조절해야 한다.
				// 
				// 1.cos(theta)>0,sin(theta)>0  0~90, -270~-360
				// 2.cos(theta)>0,sin(theta)<0  -90~0, 270~360
				// 3.cos(theta)<0,sin(theta)>0  90~180, -180~-270
				// 4.cos(theta)<0,sin(theta)<0  -90~-180, 180~270
				// 
				// 0~180의 음수값에 대해서만 카메라의 각도를 조절하면 되므로 
				// 2번과 4번 경우만 고려해 주면 된다.
				// 
				if(rad_fYaw>0.0f && rad_fYaw< _X_PI && sin_theta<0 && cos_theta>0)
				{
					rad_fYaw =  _X_PI - rad_fYaw ;
				}
				else if(rad_fYaw>0.0f && rad_fYaw<_X_PI && sin_theta<0 && cos_theta<0)
				{
					rad_fYaw = _X_PI - rad_fYaw ;
				}
				else //나머지 부분은 양수 
				{
					rad_fYaw = _X_PI + rad_fYaw ;
				}

				deg_fYaw = _X_DEG(rad_fYaw);
				
				
				// NPC가 대화 모드일 때 화면 왼쪽에 위치하기 위해 필요한 계산 
				// 화면 왼쪽으로 얼만큼 갔는지 각도 계산
				if( m_pNPCScriptWindowPtr->m_NPCMode == _XNPC_NONE )
				{
					D3DXVECTOR3 baseaxis = D3DXVECTOR3(0.0f, 0.0f, -1.5f);		
					D3DXVECTOR3 currentaxis = D3DXVECTOR3( m_NPCWideViewMove, 0.0f, -1.5f); // 왼쪽으로 이동한 거리, 0, Target과의 거리
					
					rad_temp = _XGetAngleFromVectors(&baseaxis, &currentaxis);
					deg_temp = _X_DEG(rad_temp);				
					
				}
				else
				{
					D3DXVECTOR3 baseaxis = D3DXVECTOR3(0.0f, 0.0f, -1.5f);		
					D3DXVECTOR3 currentaxis = D3DXVECTOR3( m_NPCWideViewMove, 0.0f, -1.5f); // 왼쪽으로 이동한 거리, 0, Target과의 거리
					
					rad_temp = _XGetAngleFromVectors(&baseaxis, &currentaxis);
					deg_temp = _X_DEG(rad_temp);				
				}

				if( m_bMoveNPCWindow )
					DrawNPCWindowMove(); // NPC 대화창 움직이는 함수 
				
				D3DXMATRIX matTrans;	// 왼쪽으로 이동한 거리를 적용하기 위한 변수		
				D3DXMATRIX matRotY;		// NPC 정면을 바라보는 각도를 적용하기 위한 변수 
				D3DXMATRIX matResult;
				D3DXMATRIX matTargetPos;// 카메라가 바라보는 방향
				
				D3DXMatrixIdentity(&matTrans);
				D3DXMatrixIdentity(&matRotY);
				D3DXMatrixIdentity(&matResult);
				D3DXMatrixIdentity(&matTargetPos);
				
				
				D3DXMatrixTranslation( &matTrans, m_NPCWideViewMove, 0.0f, 0.0f );
				D3DXMatrixRotationY( &matRotY, rad_fYaw );
				
				D3DXMatrixMultiply( &matResult, &matTrans, &matRotY );
				
				D3DXMatrixTranslation( &matTrans, g_LastNPCScriptOwner->m_matWorldPosition._41
					, g_LastNPCScriptOwner->m_matWorldPosition._42
					, g_LastNPCScriptOwner->m_matWorldPosition._43 );
				
				D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matResult );// 원점에서 계산 (이동, 회전)
				D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matTrans );// 원점에서 계산 후에 NPC 위치로 이동 변환
				
				position.x = matTargetPos._41;	
				position.y = matTargetPos._42 + g_LastNPCScriptOwner->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1]/2 *3 ;
				position.z = matTargetPos._43;	
				
				g_LodTerrain.m_3PCamera.SetTargetPosition( position );
				
				if( g_LastNPCScriptOwner->m_ObjectIndex == 46 )//개방방주일경우.
				{
					g_LodTerrain.m_3PCamera.m_TargetDistance = 2.0f;
				}
				else
				{
					g_LodTerrain.m_3PCamera.m_TargetDistance = 1.5f;
				}
				
				g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
				g_LodTerrain.m_3PCamera.mp_fYaw = deg_fYaw + deg_temp;  // NPC 정면을 바라보는 각도 + 왼쪽으로 이동하여 변한 각도 
				
				// reset camera rotate & zoom velocity...
				g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
				g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	
				
				g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.RebuildLevel(!FALSE);

				
				/* 2004.06.01->oneway48 delete : 단골 시스템이 완성된 후에 적용
				_XDrawRectAngle( gnWidth - 75, 10, gnWidth - 20 , 28, 0.0f, D3DCOLOR_ARGB(255,255,255,255) );
				_XDrawRectAngle( gnWidth - 75, 29, gnWidth - 20 , 47, 0.0f, D3DCOLOR_ARGB(255,255,255,255) );
				_XDrawRectAngle( gnWidth - 76, 9, gnWidth - 19 , 48, 0.0f, D3DCOLOR_ARGB(255,255,255,255) );
				
				  g_XBaseFont->SetColor( _XSC_INFORMATION );
				  g_XBaseFont->Puts( gnWidth - 65 , 14, _T("친밀도") );
				  
					g_XBaseFont->SetColor( _XSC_DEFAULT );
					g_XBaseFont->Puts( gnWidth - 60 , 33, _T("단골") );
				*/
				// 2004.07.06->oneway48 insert : 헤어질 때 대사를 한 후에 종료한다.
				if( m_NPCWideViewExitTimer != 0 )
				{
					
					
					if( m_pNPCScriptWindowPtr )
					{
						
						//if((g_LocalSystemTime - m_NPCWideViewExitTimer) > 1000)
						if( !m_pNPCScriptWindowPtr->m_bProgressNPCDialogMessage )
						{
							m_NPCWideViewExitTimer = 0;
							m_pNPCScriptWindowPtr->ShowWindow( FALSE );
							
							RepositionTopBottomVertex();
							
							if(g_LastNPCScriptOwner)
							{
								g_LastNPCScriptOwner->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
								g_LastNPCScriptOwner->m_SelectedMainScriptIndex = 0;
								g_LastNPCScriptOwner->m_PrevMainScriptIndex = -1;
								g_LastNPCScriptOwner->m_SelectedSubScriptIndex = -1;
								g_LastNPCScriptOwner->m_bCheckHostility= FALSE;
								g_LastNPCScriptOwner->SetHaveQuestID(-1);
								
								g_LastNPCScriptOwner->SetIdleAction();				
								g_LastNPCScriptOwner->SetNPCDialogMode(FALSE);
								
								if(g_LastNPCScriptOwner->m_bStartQuestDialog) 
								{
									g_LastNPCScriptOwner->m_bStartQuestDialog = FALSE;
									m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(TRUE);
								}

								g_LastNPCScriptOwner->m_pNPCScriptWindowPtr = NULL;
								g_LastNPCScriptOwner = NULL;
							}
							
							g_ScriptStarted = FALSE;
							
							if(m_bNPCDialogWideView)
							{
								m_bNPCDialogWideView = FALSE; 
								g_NPCDialogScreenEffectFlag = TRUE;
								m_bNPCFadeInOutFlag = TRUE;
							}
							
							if(m_pMainFrameWindowPtr)
							{
								if(m_pMainFrameWindowPtr->GetShowStatus())
									m_pMainFrameWindowPtr->ShowWindow(FALSE);
							}
														
							_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
							_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
							_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
														
							if(g_pInventory_Window && m_pNPCTradeWindowPtr  && pTradeConfirm_Window  && m_pAddSaveItemWindowPtr
								&& m_pWarehouseWindowPtr && m_pNPCSkillWindowPtr && m_pRepairItemWindowPtr &&
								m_pSocketPlugInExWindowPtr && pPaymentMethod_Window )
							{
								if(g_pInventory_Window->GetShowStatus())
									g_pInventory_Window->ShowWindow(FALSE);
								
								if(m_pNPCTradeWindowPtr->GetShowStatus())
									m_pNPCTradeWindowPtr->ShowWindow(FALSE);
								
								if(m_pWarehouseWindowPtr->GetShowStatus())
									m_pWarehouseWindowPtr->ShowWindow(FALSE);
								
								if( pTradeConfirm_Window->GetShowStatus())
									pTradeConfirm_Window->ShowWindow(FALSE);
								
								if( m_pAddSaveItemWindowPtr->GetShowStatus())
									m_pAddSaveItemWindowPtr->ShowWindow(FALSE);
								
								if(m_pNPCSkillWindowPtr->GetShowStatus())
									m_pNPCSkillWindowPtr->ShowWindow(FALSE);
								
								if(m_pRepairItemWindowPtr->GetShowStatus())
									m_pRepairItemWindowPtr->ShowWindow(FALSE);

								if( m_pSocketPlugInExWindowPtr->GetShowStatus() )
									m_pSocketPlugInExWindowPtr->ShowWindow(FALSE);

								if( m_pInchantWindowPtr->GetShowStatus() )
									m_pInchantWindowPtr->ShowWindow(FALSE);

								if( pPaymentMethod_Window->GetShowStatus() )
									pPaymentMethod_Window->ShowWindow( FALSE );

								if( pPCRPremium_Window->GetShowStatus() )
									pPCRPremium_Window->ShowWindow( FALSE );
							}
							
							// 2004.11.3->hotblood insert -------------------------------------------------┓
							_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
							if( pTradeMessageBox ) 
								g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
														
							_XMessageWindow* pEventMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTMESSAGEWINDOW);
							if( pEventMessageBox ) 
								g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_EVENTMESSAGEWINDOW );
							
							_XMessageWindow* pHealMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HEALMESSAGEWINDOW);
							if( pHealMessageBox ) 
								g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_HEALMESSAGEWINDOW );

							if(m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
							{
								m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
								m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
							}
							
							if(g_pInventory_Window->m_bHaveMouseSlot)
							{
								g_pInventory_Window->m_bHaveMouseSlot = FALSE;
								g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
							}	
							if(m_pWarehouseWindowPtr->m_bHaveMouseSlot )
							{
								m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
								m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
							}	
							if(m_pSocketPlugInExWindowPtr->m_bHaveMouseSlot )							
							{
								m_pSocketPlugInExWindowPtr->m_bHaveMouseSlot = FALSE;
								m_pSocketPlugInExWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
							}
							// ------------------------------------------------------------------------┛
							
							// NPC 대화모드에서 다른 창이 뜰 경우 화면처리를 위해서 
							SetNPCMode( _XNPC_NONE );
							
							m_pNPCScriptWindowPtr->m_NPCDialogCount = 0;//2004.07.08->oneway48 insert
							m_pNPCScriptWindowPtr->m_bFirstDialog = TRUE;

							m_pNPCScriptWindowPtr->m_btnQuestOK->ShowWindow(FALSE);
							m_pNPCScriptWindowPtr->m_btnBackToNPCMode->ShowWindow(FALSE);
						//	m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(FALSE);
							m_pNPCScriptWindowPtr->m_listboxSelectionItem->ShowWindow(FALSE);
							if( m_pNPCScriptWindowPtr->m_pTitleString[0] != 0 ) 
								memset(m_pNPCScriptWindowPtr->m_pTitleString, 0, sizeof(TCHAR)*256);
							if( m_pNPCScriptWindowPtr->m_HaveQuestName[0] != 0 ) 
								memset(m_pNPCScriptWindowPtr->m_HaveQuestName, 0, sizeof(m_pNPCScriptWindowPtr->m_HaveQuestName));
							if( m_pNPCScriptWindowPtr->m_HaveQuestType[0] != 0 ) 
								memset(m_pNPCScriptWindowPtr->m_HaveQuestType, 0, sizeof(m_pNPCScriptWindowPtr->m_HaveQuestType));
							
							m_pWarehouseWindowPtr->m_bWarehouseFirstView = TRUE; // 2004.06.01->oneway48 insert
							
							g_pMinimap_Window->m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);

							_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
							defaultsystemmsgwindow->ShowWindow(FALSE);
						}
					}

				}

				/*
#ifdef _ACCLAIM_IGAADSYSTEM		
				if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NPC )
				{
					g_IGAADWrapper.Render();
				}
#endif
				*/
				return;
			}	
		
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );

#ifdef _ACCLAIM_IGAADSYSTEM
			D3DCOLOR color = ((DWORD)m_ADScreeenDimmedLevel << 24);
			g_ScreenBurnOut[0].color = color;
			g_ScreenBurnOut[1].color = color;
			g_ScreenBurnOut[2].color = color;
			g_ScreenBurnOut[3].color = color;
			
			gpDev->SetFVF( D3DFVF_XTLVERTEX );
			gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
#else
			gpDev->SetFVF( D3DFVF_XTLVERTEX );
			gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
#endif

			m_TopScreenVertex[2].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
			if(m_TopScreenVertex[2].y >= 60 )
				m_TopScreenVertex[2].y = 60;
			
			m_TopScreenVertex[3].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
			if(m_TopScreenVertex[3].y >= 60 )
				m_TopScreenVertex[3].y = 60;
		
			m_BottomScreenVertex[0].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
			if(m_BottomScreenVertex[0].y <= gnHeight - 150)
				m_BottomScreenVertex[0].y = gnHeight - 150;
		
			m_BottomScreenVertex[1].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
			if(m_BottomScreenVertex[1].y <= gnHeight - 150)
				m_BottomScreenVertex[1].y = gnHeight - 150;
			
			m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
			m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
			m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
			m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
			
			m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
			m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
			m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
			m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
			
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );
		
			gpDev->SetFVF(D3DFVF_XTLVERTEX);
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));
			
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));

			
			D3DXVECTOR3 position;		// 카메라의 위치를 나타내는 변수
			float rad_fYaw, deg_fYaw;	// 카메라의 바라보는 각도를 나타내는 변수
			float sin_theta ,cos_theta; // theta = fYaw 카메라의 각도의 sin값, cos값
		
			sin_theta = g_LastNPCScriptOwner->m_matWorldPosition._31;
			cos_theta = g_LastNPCScriptOwner->m_matWorldPosition._11;
		
			rad_fYaw = acos(cos_theta); 
		
			if(rad_fYaw>0.0f && rad_fYaw< _X_PI && sin_theta<0 && cos_theta>0)
			{
				rad_fYaw =  _X_PI - rad_fYaw ;
			}
			else if(rad_fYaw>0.0f && rad_fYaw<_X_PI && sin_theta<0 && cos_theta<0)
			{
				rad_fYaw = _X_PI - rad_fYaw ;
			}
			else //나머지 부분은 양수 
			{
				rad_fYaw = _X_PI + rad_fYaw ;
			}
		
			deg_fYaw = _X_DEG(rad_fYaw);

			float rad_temp, deg_temp;

			D3DXVECTOR3 baseaxis = D3DXVECTOR3(0.0f, 0.0f, -1.5f);		
			D3DXVECTOR3 currentaxis = D3DXVECTOR3( m_NPCWideViewMove, 0.0f, -1.5f); // 왼쪽으로 이동한 거리, 0, Target과의 거리
			
			rad_temp = _XGetAngleFromVectors(&baseaxis, &currentaxis);
			deg_temp = _X_DEG(rad_temp);				
					
			D3DXMATRIX matTrans;	// 왼쪽으로 이동한 거리를 적용하기 위한 변수		
			D3DXMATRIX matRotY;		// NPC 정면을 바라보는 각도를 적용하기 위한 변수 
			D3DXMATRIX matResult;
			D3DXMATRIX matTargetPos;// 카메라가 바라보는 방향
			
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixIdentity(&matRotY);
			D3DXMatrixIdentity(&matResult);
			D3DXMatrixIdentity(&matTargetPos);
						
			//D3DXMatrixTranslation( &matTrans, -0.5f, 0.0f, 0.0f );
			D3DXMatrixTranslation( &matTrans, m_NPCWideViewMove, 0.0f, 0.0f );
			D3DXMatrixRotationY( &matRotY, rad_fYaw );
			
			D3DXMatrixMultiply( &matResult, &matTrans, &matRotY );
			
			D3DXMatrixTranslation( &matTrans, g_LastNPCScriptOwner->m_matWorldPosition._41
				, g_LastNPCScriptOwner->m_matWorldPosition._42
				, g_LastNPCScriptOwner->m_matWorldPosition._43 );
			
			D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matResult );// 원점에서 계산 (이동, 회전)
			D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matTrans );// 원점에서 계산 후에 NPC 위치로 이동 변환
			
			position.x = matTargetPos._41;	
			position.y = matTargetPos._42 + g_LastNPCScriptOwner->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1]/2 *3 ;
			position.z = matTargetPos._43;	
			
			g_LodTerrain.m_3PCamera.SetTargetPosition( position );

			if( g_LastNPCScriptOwner->m_ObjectIndex == 46 )//개방방주일경우.
			{
				g_LodTerrain.m_3PCamera.m_TargetDistance = 2.0f;
			}
			else
			{
				g_LodTerrain.m_3PCamera.m_TargetDistance = 1.5f;
			}
			
			g_LodTerrain.m_3PCamera.mp_fFov = 40.0f;
			g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
			g_LodTerrain.m_3PCamera.mp_fYaw = deg_fYaw + deg_temp;  // NPC 정면을 바라보는 각도 + 왼쪽으로 이동하여 변한 각도 
			
			// reset camera rotate & zoom velocity...
			g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
			g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.RebuildLevel();		
			
			g_LastNPCScriptOwner->Process();
			g_ScriptStarted = TRUE;
		}
	}
}

void XProc_MainGame::DrawNPCFadeInOut()
{
	D3DCOLOR color = ((DWORD)m_NPCDialogScreenAlphaLevel << 24);
	g_ScreenBurnOut[0].color = color;
	g_ScreenBurnOut[1].color = color;
	g_ScreenBurnOut[2].color = color;
	g_ScreenBurnOut[3].color = color;
	
				
	// Fade in
	if( g_NPCDialogScreenEffectFlag )
	{
		m_NPCDialogScreenAlphaLevel+= _XDEF_NPCDIALOGMODE_FADE_LEVEL;
		
		if( m_NPCDialogScreenAlphaLevel >= 255 )
		{						
			m_NPCDialogScreenAlphaLevel = 255;
			g_NPCDialogScreenEffectFlag = FALSE;
			
			//g_pLocalUser->m_TargetPosition
			g_LodTerrain.m_3PCamera.SetCameraPosition(m_vTempCameraPosition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(m_vTempTargetPosition);
			g_LodTerrain.m_3PCamera.SetDistance(m_fTempTargetDistance);
			g_LodTerrain.m_3PCamera.mp_fFov = m_fTempFov;
			g_LodTerrain.m_3PCamera.mp_fPitch = m_fTempPitch;
			g_LodTerrain.m_3PCamera.mp_fYaw = m_fTempYaw;
			
			// reset camera rotate & zoom velocity...
			g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
			g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel();

			_XWindow_TutorialIcon* pTutorialIcon_Window = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_TUTORIALICON );
			if( pTutorialIcon_Window )
			{
				pTutorialIcon_Window->AllOpenExternalTutorialWindow();
			}
		}
		
		m_TopScreenVertex[2].y = 60; 
		m_TopScreenVertex[3].y = 60; 
		m_BottomScreenVertex[0].y = gnHeight - 150;
		m_BottomScreenVertex[1].y = gnHeight -         150;
		
		m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
		m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
		m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
		m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
		
		m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
		m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
		m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
		m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
		
		gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );
		
		gpDev->SetFVF(D3DFVF_XTLVERTEX);
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));
		
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));
		
	}
	else // Fade out
	{
		m_bNPCDialogMode = FALSE;	// 대화가 끝나고 Fade out 될 때 PC는 다시 Rendering 시작 

        BaseInterfaceWindowOpen();		
		// ---------------------------------------------------------------=
	}
	
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );
	
	gpDev->SetTexture(0,NULL);
	
	gpDev->SetFVF( D3DFVF_XTLVERTEX );
	gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
	
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
}


void XProc_MainGame::DrawFunctionalObjectWideView()
{
	if(g_LastFunctionalObject)
	{
		D3DCOLOR color = ((DWORD)m_NPCDialogScreenAlphaLevel << 24);
		g_ScreenBurnOut[0].color = color;
		g_ScreenBurnOut[1].color = color;
		g_ScreenBurnOut[2].color = color;
		g_ScreenBurnOut[3].color = color;

		// Fade in
		if( g_NPCDialogScreenEffectFlag )
		{
			m_NPCDialogScreenAlphaLevel+= _XDEF_NPCDIALOGMODE_FADE_LEVEL;

			if( m_NPCDialogScreenAlphaLevel >= 255 )
			{						
				m_NPCDialogScreenAlphaLevel = 255;
				g_NPCDialogScreenEffectFlag = FALSE;

				m_vTempCameraPosition = g_LodTerrain.m_3PCamera.m_CameraPosition;
				m_vTempTargetPosition = g_LodTerrain.m_3PCamera.m_TargetPosition;
				m_fTempTargetDistance = g_LodTerrain.m_3PCamera.m_TargetDistance;
				m_fTempFov = g_LodTerrain.m_3PCamera.mp_fFov;
				m_fTempPitch = g_LodTerrain.m_3PCamera.mp_fPitch;
				m_fTempYaw = g_LodTerrain.m_3PCamera.mp_fYaw;
				
				WindowCloseBeforeWideView(); 
												
				_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
				defaultsystemmsgwindow->ShowWindow(TRUE);
				defaultsystemmsgwindow->MoveWindow(gnWidth - 374, gnHeight-150);
				defaultsystemmsgwindow->m_SystemMsgListScrollBar->MoveWindow(gnWidth - 13 ,gnHeight - 137);
				defaultsystemmsgwindow->m_SystemMsgListScrollBar->SetLineScrollPos( 999999 ); 
				defaultsystemmsgwindow->m_btnFunctionalObjectViewMode->ShowWindow(TRUE);
				defaultsystemmsgwindow->m_btnFunctionalObjectViewMode->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAME_1835);
												
				// 퀘스트 시작조건에 Object 타게팅 조건이 들어간 퀘스트 시작조건을 검사 
				g_QuestScriptManager.QuestStartConditionObjectClickMode();			
				
			}
		
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );						
			gpDev->SetFVF( D3DFVF_XTLVERTEX );
			gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
					
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);

			m_TopScreenVertex[2].y = 0.0f;
			m_TopScreenVertex[3].y = 0.0f;
			m_BottomScreenVertex[0].y = gnHeight;
			m_BottomScreenVertex[1].y = gnHeight;

		}
		else // Fade out
		{
			m_NPCDialogScreenAlphaLevel-= _XDEF_NPCDIALOGMODE_FADE_LEVEL;
			if( m_NPCDialogScreenAlphaLevel <= 0 )
			{				
				m_NPCDialogScreenAlphaLevel = 0;
				
				m_TopScreenVertex[2].y = 60; 
				m_TopScreenVertex[3].y = 60; 
				m_BottomScreenVertex[0].y = gnHeight - 150;
				m_BottomScreenVertex[1].y = gnHeight - 150;
				
				m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
				m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
				m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
				m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
				
				m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
				m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
				m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
				m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
				
				gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
				g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );

				gpDev->SetFVF(D3DFVF_XTLVERTEX);
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));

				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
				gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));
				
				
				if( g_LastFunctionalObject->m_ObjectName )
				{
					int length = strlen( g_LastFunctionalObject->m_ObjectName );
					length = (gnWidth>>1) - (length*6);
					g_XLargeFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
					g_XLargeFont->Puts( length, 20, g_LastFunctionalObject->m_ObjectName );
					g_XLargeFont->Flush();
				}
				if( g_LastFunctionalObject->m_SystemMessage )
				{
					int length = strlen( g_LastFunctionalObject->m_SystemMessage );
					length = (gnWidth>>1) - (length*3);
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) );
					g_XBaseFont->SetBoldMode(TRUE);
					g_XBaseFont->Puts( length, (gnHeight*0.703125f), g_LastFunctionalObject->m_SystemMessage );
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode(FALSE);
				}
			
				
				D3DXVECTOR3 position;		// 카메라의 위치를 나타내는 변수
				float rad_fYaw, deg_fYaw;	// 카메라의 바라보는 각도를 나타내는 변수
				float sin_theta ,cos_theta; // theta = fYaw 카메라의 각도의 sin값, cos값
				//float rad_temp, deg_temp;
			
				// Y축을 중심으로 theta만큼 회전한 4X4 매트릭스에서 
				// cos(theta)=_11, -sin(theta)=_13,
				// sin(theta)=_31, cos(theta)=_33, 으로 표현되므로
				sin_theta = g_LastFunctionalObject->m_matWorldPosition._31;
				cos_theta = g_LastFunctionalObject->m_matWorldPosition._11;

				// npc 정면을 바라보도록 카메라를 회전시키기 위한 각도를 구하는 부분
				// cos값이나 sin값을 알고 있으면 arccos이나 arcsin함수를 이용하여 각도를 알 수 있다
				rad_fYaw = acos(cos_theta) + _X_RAD(g_LastFunctionalObject->m_CameraYaw); 
			
				//
				// 하지만 arccos함수를 이용하여 구한 값은 0 ~ 180 의  값이므로
				// 180도가 넘으면 음수가 되므로 판별해서 카메라의 각도를 조절해야 한다.
				// 
				// 1.cos(theta)>0,sin(theta)>0  0~90, -270~-360
				// 2.cos(theta)>0,sin(theta)<0  -90~0, 270~360
				// 3.cos(theta)<0,sin(theta)>0  90~180, -180~-270
				// 4.cos(theta)<0,sin(theta)<0  -90~-180, 180~270
				// 
				// 0~180의 음수값에 대해서만 카메라의 각도를 조절하면 되므로 
				// 2번과 4번 경우만 고려해 주면 된다.
				// 
				if(rad_fYaw>0.0f && rad_fYaw< _X_PI && sin_theta<0 && cos_theta>0)
				{
					rad_fYaw =  _X_PI - rad_fYaw ;
				}
				else if(rad_fYaw>0.0f && rad_fYaw<_X_PI && sin_theta<0 && cos_theta<0)
				{
					rad_fYaw = _X_PI - rad_fYaw ;
				}
				else //나머지 부분은 양수 
				{
					rad_fYaw = _X_PI + rad_fYaw ;
				}

				deg_fYaw = _X_DEG(rad_fYaw);
				
//				D3DXVECTOR3 baseaxis = D3DXVECTOR3(0.0f, 0.0f, -g_LastFunctionalObject->m_CameraDistance);		
//				D3DXVECTOR3 currentaxis = D3DXVECTOR3( 0.0f, 0.0f, -g_LastFunctionalObject->m_CameraDistance); // 왼쪽으로 이동한 거리, 0, Target과의 거리
//				
//				rad_temp = _XGetAngleFromVectors(&baseaxis, &currentaxis);
//				deg_temp = _X_DEG(rad_temp);				

					
				
				D3DXMATRIX matTrans;	// 왼쪽으로 이동한 거리를 적용하기 위한 변수		
				D3DXMATRIX matRotY;		// NPC 정면을 바라보는 각도를 적용하기 위한 변수 
				D3DXMATRIX matResult;
				D3DXMATRIX matTargetPos;// 카메라가 바라보는 방향
				
				D3DXMatrixIdentity(&matTrans);
				D3DXMatrixIdentity(&matRotY);
				D3DXMatrixIdentity(&matResult);
				D3DXMatrixIdentity(&matTargetPos);
				
				
				D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, g_LastFunctionalObject->m_CameraDistance );
				D3DXMatrixRotationY( &matRotY, rad_fYaw );
				
				D3DXMatrixMultiply( &matResult, &matTrans, &matRotY );
				
				D3DXMatrixTranslation( &matTrans, g_LastFunctionalObject->m_matWorldPosition._41
					, g_LastFunctionalObject->m_matWorldPosition._42
					, g_LastFunctionalObject->m_matWorldPosition._43 );
				
				D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matResult );// 원점에서 계산 (이동, 회전)
				D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matTrans );// 원점에서 계산 후에 NPC 위치로 이동 변환
				
				position.x = matTargetPos._41;	
				//position.y = matTargetPos._42 + g_LodTerrain.m_MeshObjectManager.m_ModelList[g_LastFunctionalObject->m_ObjectIndex]->OBB_Extent[1] ;
				position.y = matTargetPos._42 + g_LastFunctionalObject->m_CameraTargetHeight;
				position.z = matTargetPos._43;	 
				
				g_LodTerrain.m_3PCamera.SetTargetPosition( position );
				
				//g_LodTerrain.m_3PCamera.m_TargetDistance = g_LodTerrain.m_MeshObjectManager.m_ModelList[g_LastFunctionalObject->m_ObjectIndex]->OBB_Extent[1]*5 ;
				g_LodTerrain.m_3PCamera.m_TargetDistance = g_LastFunctionalObject->m_CameraDistance ;
				
				//g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
				g_LodTerrain.m_3PCamera.mp_fPitch = g_LastFunctionalObject->m_CameraPitch;
				g_LodTerrain.m_3PCamera.mp_fYaw = deg_fYaw;// + deg_temp;  // NPC 정면을 바라보는 각도 + 왼쪽으로 이동하여 변한 각도 
				
				// reset camera rotate & zoom velocity...
				g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
				g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	
				
				g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.RebuildLevel(FALSE);

				
				g_LastFunctionalObject->ProcessAction();	

				// 2004.07.06->oneway48 insert : 헤어질 때 대사를 한 후에 종료한다.
				if( m_bExitFunctionalObjectWideView )
				{					
					m_bExitFunctionalObjectWideView = FALSE;
					
					RepositionTopBottomVertex();
										
					g_LastFunctionalObject = NULL;
					g_bFunctionalObjectScriptStarted = FALSE;
					
					m_bFunctionalObjectWideView = FALSE; 
					g_NPCDialogScreenEffectFlag = TRUE;
					m_bFunctionalObjectFadeInOut = TRUE;
												
					_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
					defaultsystemmsgwindow->ShowWindow(FALSE);
					defaultsystemmsgwindow->m_btnFunctionalObjectViewMode->ShowWindow(FALSE);
				}

				return;
			}	
		
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );

			gpDev->SetFVF( D3DFVF_XTLVERTEX );
			gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );

			m_TopScreenVertex[2].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
			if(m_TopScreenVertex[2].y >= 60 )
				m_TopScreenVertex[2].y = 60;
			
			m_TopScreenVertex[3].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
			if(m_TopScreenVertex[3].y >= 60 )
				m_TopScreenVertex[3].y = 60;
		
			m_BottomScreenVertex[0].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
			if(m_BottomScreenVertex[0].y <= gnHeight - 150)
				m_BottomScreenVertex[0].y = gnHeight - 150;
		
			m_BottomScreenVertex[1].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
			if(m_BottomScreenVertex[1].y <= gnHeight - 150)
				m_BottomScreenVertex[1].y = gnHeight - 150;
			
			m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
			m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
			m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
			m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
			
			m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
			m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
			m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
			m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
			
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );
		
			gpDev->SetFVF(D3DFVF_XTLVERTEX);
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));
			
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
			gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));
			
			D3DXVECTOR3 position;		// 카메라의 위치를 나타내는 변수
			float rad_fYaw, deg_fYaw;	// 카메라의 바라보는 각도를 나타내는 변수
			float sin_theta ,cos_theta; // theta = fYaw 카메라의 각도의 sin값, cos값
		
			sin_theta = g_LastFunctionalObject->m_matWorldPosition._31;
			cos_theta = g_LastFunctionalObject->m_matWorldPosition._11;
			
			rad_fYaw = acos(cos_theta) + _X_RAD(g_LastFunctionalObject->m_CameraYaw); 
		
			if(rad_fYaw>0.0f && rad_fYaw< _X_PI && sin_theta<0 && cos_theta>0)
			{
				rad_fYaw =  _X_PI - rad_fYaw ;
			}
			else if(rad_fYaw>0.0f && rad_fYaw<_X_PI && sin_theta<0 && cos_theta<0)
			{
				rad_fYaw = _X_PI - rad_fYaw ;
			}
			else //나머지 부분은 양수 
			{
				rad_fYaw = _X_PI + rad_fYaw ;
			}
		
			deg_fYaw = _X_DEG(rad_fYaw);

//			float rad_temp, deg_temp;
//
//			D3DXVECTOR3 baseaxis = D3DXVECTOR3(0.0f, 0.0f, -5.0f);		
//			D3DXVECTOR3 currentaxis = D3DXVECTOR3( 0.0f, 0.0f, -5.0f); // 왼쪽으로 이동한 거리, 0, Target과의 거리
//			
//			rad_temp = _XGetAngleFromVectors(&baseaxis, &currentaxis);
//			deg_temp = _X_DEG(rad_temp);				

					
			D3DXMATRIX matTrans;	// 왼쪽으로 이동한 거리를 적용하기 위한 변수		
			D3DXMATRIX matRotY;		// NPC 정면을 바라보는 각도를 적용하기 위한 변수 
			D3DXMATRIX matResult;
			D3DXMATRIX matTargetPos;// 카메라가 바라보는 방향
			
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixIdentity(&matRotY);
			D3DXMatrixIdentity(&matResult);
			D3DXMatrixIdentity(&matTargetPos);
						
			//D3DXMatrixTranslation( &matTrans, -0.5f, 0.0f, 0.0f );
			D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, g_LastFunctionalObject->m_CameraDistance );
			D3DXMatrixRotationY( &matRotY, rad_fYaw );
			
			D3DXMatrixMultiply( &matResult, &matTrans, &matRotY );
			
			D3DXMatrixTranslation( &matTrans, g_LastFunctionalObject->m_matWorldPosition._41
				, g_LastFunctionalObject->m_matWorldPosition._42
				, g_LastFunctionalObject->m_matWorldPosition._43 );
			
			D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matResult );// 원점에서 계산 (이동, 회전)
			D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matTrans );// 원점에서 계산 후에 NPC 위치로 이동 변환
			position.x = matTargetPos._41;	
			//position.y = matTargetPos._42 + g_LodTerrain.m_MeshObjectManager.m_ModelList[g_LastFunctionalObject->m_ObjectIndex]->OBB_Extent[1] ;
			position.y = matTargetPos._42 + g_LastFunctionalObject->m_CameraTargetHeight;
			position.z = matTargetPos._43;	
			
			g_LodTerrain.m_3PCamera.SetTargetPosition( position );
			
			//g_LodTerrain.m_3PCamera.m_TargetDistance = g_LodTerrain.m_MeshObjectManager.m_ModelList[g_LastFunctionalObject->m_ObjectIndex]->OBB_Extent[1]*5;
			g_LodTerrain.m_3PCamera.m_TargetDistance = g_LastFunctionalObject->m_CameraDistance;
			
			g_LodTerrain.m_3PCamera.mp_fFov = 40.0f;
			//g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
			g_LodTerrain.m_3PCamera.mp_fPitch = g_LastFunctionalObject->m_CameraPitch;
			g_LodTerrain.m_3PCamera.mp_fYaw = deg_fYaw;// + deg_temp;  // NPC 정면을 바라보는 각도 + 왼쪽으로 이동하여 변한 각도 
			
			// reset camera rotate & zoom velocity...
			g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
			g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.RebuildLevel(FALSE);
			
			g_bFunctionalObjectScriptStarted = TRUE;

		}
	}	

}

void XProc_MainGame::DrawFunctionalObjectFadeInOut()
{
	D3DCOLOR color = ((DWORD)m_NPCDialogScreenAlphaLevel << 24);
	g_ScreenBurnOut[0].color = color;
	g_ScreenBurnOut[1].color = color;
	g_ScreenBurnOut[2].color = color;
	g_ScreenBurnOut[3].color = color;
	
				
	// Fade in
	if( g_NPCDialogScreenEffectFlag )
	{
		m_NPCDialogScreenAlphaLevel+= _XDEF_NPCDIALOGMODE_FADE_LEVEL;
		
		if( m_NPCDialogScreenAlphaLevel >= 255 )
		{						
			m_NPCDialogScreenAlphaLevel = 255;
			g_NPCDialogScreenEffectFlag = FALSE;
			
			//g_pLocalUser->m_TargetPosition
			g_LodTerrain.m_3PCamera.SetCameraPosition(m_vTempCameraPosition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(m_vTempTargetPosition);
			g_LodTerrain.m_3PCamera.SetDistance(m_fTempTargetDistance);
			g_LodTerrain.m_3PCamera.mp_fFov = m_fTempFov;
			g_LodTerrain.m_3PCamera.mp_fPitch = m_fTempPitch;
			g_LodTerrain.m_3PCamera.mp_fYaw = m_fTempYaw;
			
			// reset camera rotate & zoom velocity...
			g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
			g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel();
		}
		
		m_TopScreenVertex[2].y = 60; 
		m_TopScreenVertex[3].y = 60; 
		m_BottomScreenVertex[0].y = gnHeight - 150;
		m_BottomScreenVertex[1].y = gnHeight - 150;
		
		m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
		m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
		m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
		m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
		
		m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
		m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
		m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
		m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
		
		gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );
		
		gpDev->SetFVF(D3DFVF_XTLVERTEX);
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));
		
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));
		
	}
	else // Fade out
	{
		BaseInterfaceWindowOpen(); //Author : 양희왕 //breif : 윈도우창 오픈 수정		
	}
	
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );
	
	gpDev->SetTexture(0,NULL);
	
	gpDev->SetFVF( D3DFVF_XTLVERTEX );
	gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
	
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );

}

void XProc_MainGame::ProcessNPCScriptAutoPopUp(void)
{
	if(m_bAutoNPCScriptPopUp)
	{
		FLOAT dx = (m_TempNPCObject->m_matWorldPosition._41 - g_pLocalUser->m_Position.x);
		FLOAT dz = (m_TempNPCObject->m_matWorldPosition._43 - g_pLocalUser->m_Position.z);
		FLOAT distance = _XFC_sqrt( dx*dx + dz*dz );

		FLOAT checkdistance = _XDEF_DISTANCEOFSTARTNPCSCRIPT;

		if(m_TempNPCObject->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
		{
			checkdistance = 1.0f;
		}
		
		if( distance < checkdistance )
		{
			g_LastNPCScriptOwner = m_TempNPCObject;
			
			if( g_LastNPCScriptOwner->m_NPCRollType == _XNPCROLL_WAREHOUSE)
			{
#ifdef _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI
				if(((g_LocalSystemTime - m_pWarehouseWindowPtr->m_dwStorageInfoReqTime) >= 50000) && 
					(m_pWarehouseWindowPtr->m_dwStorageInfoReqTime != 0))
				{
					// 창고 정보 요청한지 5분 지났음 - 다시 요청
					m_pWarehouseWindowPtr->m_bWarehouseFirstLoad = FALSE;
				}
#endif

				if(!m_pWarehouseWindowPtr->m_bWarehouseFirstLoad)
				{
					g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_REQ);
					m_pWarehouseWindowPtr->m_bWarehouseFirstView = FALSE;
					m_pWarehouseWindowPtr->m_dwStorageInfoReqTime = g_LocalSystemTime;
				}				
			}
			
			if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
			{
				// 장원전의 장주일 때 
				// 협박 시작 packet 보내기 - 총대장 체크 하자.
				if(g_CBManager.GetThreatenState() == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
				{
					if(!g_pLocalUser->m_bSendThreatenPacket)
					{
						if(((g_LocalSystemTime - g_CBManager.m_dwThreatenRequestTime) >= g_CBManager.GetThreatenDelayTime()*1000))
						{
							_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
							if(pCBPersonalHistory_Window)
							{
								if(pCBPersonalHistory_Window->m_nPoint >= 10)
								{
									g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_threat_start_req);
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
								}
								else
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3361), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"장원혈이 부족합니다."
								}
							}
						}
						else
						{
							_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
							if(pThreatenGauge_Window)
							{
								if(!pThreatenGauge_Window->GetShowStatus())
								{
									// 3362
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3362), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"지금은 협박을 진행할 수 없습니다."
								}
							}
						}
					}
				}

				m_bNPCDialogWideView = FALSE;
				g_NPCDialogScreenEffectFlag = FALSE;
				m_bAutoNPCScriptPopUp = FALSE;
				m_bNPCTalkLog = TRUE;
			}
			else
			{
				m_bNPCDialogWideView = TRUE;
				g_NPCDialogScreenEffectFlag = TRUE;
				m_bAutoNPCScriptPopUp = FALSE;
				m_bNPCTalkLog = TRUE;
							
				_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
				defaultsystemmsgwindow->ShowWindow(FALSE);
			}

			_XDWINPRINT("[MAINGAME/ProcessNPCScriptAutoPopUp] NPC[%d] talk", g_LastNPCScriptOwner->m_UniqueID);
		}
	}
}

void XProc_MainGame::DrawDyingState(void)
{
	// Wide View
	m_TopScreenVertex[2].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
	if(m_TopScreenVertex[2].y >= 60 )
		m_TopScreenVertex[2].y = 60;
	
	m_TopScreenVertex[3].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
	if(m_TopScreenVertex[3].y >= 60 )
		m_TopScreenVertex[3].y = 60;
	
	m_BottomScreenVertex[0].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
	if(m_BottomScreenVertex[0].y <= gnHeight - 150)
		m_BottomScreenVertex[0].y = gnHeight - 150;
	
	m_BottomScreenVertex[1].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
	if(m_BottomScreenVertex[1].y <= gnHeight - 150)
		m_BottomScreenVertex[1].y = gnHeight - 150;
	
	m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
	m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
	m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
	m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
	
	m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
	m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
	m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
	m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );
	
	gpDev->SetFVF(D3DFVF_XTLVERTEX);
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));
	
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));


	/*
	if(g_pLocalUser->m_DyingStartTime > 0)
	{
		int lasttime = (g_pLocalUser->m_DyingTimer - (g_LocalSystemTime - g_pLocalUser->m_DyingStartTime))/1000;

		if( g_pLocalUser->m_LastCheckedDyingStartTime != lasttime )
		{
			g_pLocalUser->m_LastCheckedDyingStartTime = lasttime;

			if( lasttime <= 9 && lasttime > 0 )
			{
				if( !g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance( _XDEF_INTERFACEID_COUNT + lasttime ) )
				{
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( 
									&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_COUNT + lasttime, 
									&g_pLocalUser->m_ModelDescriptor.m_Position );
				}
			}
			else if( lasttime <= 0 )
			{
				if( !g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance( _XDEF_INTERFACEID_COUNT + lasttime ) )
		
		  {
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( 
															&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_COUNT + lasttime, 
															&g_pLocalUser->m_ModelDescriptor.m_Position );
				}
			}
		}
	}*/

	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255 , 250, 230, 170 ) );
	g_XBaseFont->Puts((gnWidth>>1)-111 , (gnHeight>>1)-114, _XGETINTERFACETEXT(ID_STRING_MAINGAME_1836));
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
	{		
		pRebirth_Window->Draw();
	}

	_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
	if(pChat_Window)
	{
		pChat_Window->Draw();
	}

#ifdef ALL_RB_REBIRTH_USERITEM_071108_KUKURI
	_XWindow_SystemMsgDefault* pSystemMSg_Window = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	if(pSystemMSg_Window)
	{
		pSystemMSg_Window->Draw();
	}
#endif

	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=	
	_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
	if( pTutorialIcon )
	{
		pTutorialIcon->AllTutorialDraw();
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=

#ifdef _ACCLAIM_IGAADSYSTEM

	g_IGAADWrapper.Render();

#endif

	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE );
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->Draw();
	}
#ifdef _XTS_ITEMMALLBROWSER	
	_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
	if( pCashMall_Window )
	{
		if( pCashMall_Window->GetShowStatus() )
			pCashMall_Window->Draw();
	}		
#endif
}

void XProc_MainGame::DrawComaState(void)
{
	// Interface draw
	
	// Wide View
	m_TopScreenVertex[2].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
	if(m_TopScreenVertex[2].y >= 60 )
		m_TopScreenVertex[2].y = 60;
	
	m_TopScreenVertex[3].y += _XDEF_NPCDIALOGMODE_FADE_LEVEL/3; 
	if(m_TopScreenVertex[3].y >= 60 )
		m_TopScreenVertex[3].y = 60;
	
	m_BottomScreenVertex[0].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
	if(m_BottomScreenVertex[0].y <= gnHeight - 150)
		m_BottomScreenVertex[0].y = gnHeight - 150;
	
	m_BottomScreenVertex[1].y -= _XDEF_NPCDIALOGMODE_FADE_LEVEL; 
	if(m_BottomScreenVertex[1].y <= gnHeight - 150)
		m_BottomScreenVertex[1].y = gnHeight - 150;
	
	m_GridTopScreenVertex[0].y = m_TopScreenVertex[2].y;
	m_GridTopScreenVertex[1].y = m_TopScreenVertex[3].y;	
	m_GridTopScreenVertex[2].y = m_TopScreenVertex[2].y + 16; 
	m_GridTopScreenVertex[3].y = m_TopScreenVertex[3].y + 16; 
	
	m_GridBottomScreenVertex[0].y = m_BottomScreenVertex[0].y - 16;
	m_GridBottomScreenVertex[1].y = m_BottomScreenVertex[1].y - 16;
	m_GridBottomScreenVertex[2].y = m_BottomScreenVertex[0].y; 
	m_GridBottomScreenVertex[3].y = m_BottomScreenVertex[1].y; 
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );

	if(g_pLocalUser->m_UserState != _XDEF_USERSTATE_MATCH_FIGHT)
	{	
		gpDev->SetFVF(D3DFVF_XTLVERTEX);
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TopScreenVertex, sizeof(_XTLVERTEX));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_BottomScreenVertex, sizeof(_XTLVERTEX));
		
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridTopScreenVertex, sizeof(_XTLVERTEX));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_GridBottomScreenVertex, sizeof(_XTLVERTEX));

		g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
		g_XLargeFont->SetColor( D3DCOLOR_ARGB( 255 , 250, 230, 170 ) );

		BOOL bCBDrawMode = FALSE;
		DWORD lefttime = 0;
//		FLOAT percentage = 0;
		if(((g_LocalSystemTime - g_pLocalUser->m_dwDieStartTime) <= g_pLocalUser->m_dwDeadPenaltyTime) && g_pLocalUser->m_dwDieStartTime != 0)
		{
			bCBDrawMode = TRUE;
			lefttime = g_pLocalUser->m_dwDeadPenaltyTime - (g_LocalSystemTime - g_pLocalUser->m_dwDieStartTime);
			lefttime = (DWORD)(lefttime / 1000.f);
//			percentage = (FLOAT)(g_LocalSystemTime - g_pLocalUser->m_dwDieStartTime)/(FLOAT)g_pLocalUser->m_dwDeadPenaltyTime;
		}

		if(bCBDrawMode)
		{
//			g_XLargeFont->Print((gnWidth>>1)-243 , (gnHeight>>1)-145, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3247), lefttime); // 혼수 상태에 빠져있습니다. %d초 후에 소생할 수 있습니다.
			g_XLargeFont->PrintAlign((gnWidth>>1) , (gnHeight>>1)-145, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3247), lefttime);// 혼수 상태에 빠져있습니다. %d초 후에 소생할 수 있습니다.
		}
		else
		{

//			g_XLargeFont->Puts((gnWidth>>1)-140 , (gnHeight>>1)-145, _XGETINTERFACETEXT(ID_STRING_MAINGAME_HONSU)); // 당신은 혼수상태에 빠져 있습니다
			g_XLargeFont->PutsAlign((gnWidth>>1) , (gnHeight>>1)-145, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_HONSU)); // 당신은 혼수상태에 빠져 있습니다
		}

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(g_pLocalUser->GetEnterMatchEvent())
		{
			g_XLargeFont->PutsAlign((gnWidth>>1) , (gnHeight>>1)-120, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3765)); //3초 후 비무대회장 밖에서 부활됩니다. 경험치와 혈의 하락은 없습니다.
		}
		else
#endif
		{
			if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
			   g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() <= 24)	// 응신입기혈 이하일때 후유증 들어가지 않음
				{
					g_XLargeFont->PutsAlign((gnWidth>>1) , (gnHeight>>1)-120, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3670)); //응신입기혈 12성 이하는 경험치 하락과 부활 후유증이 적용되지 않습니다.
				}
			}
		}

		g_XLargeFont->Flush();
		g_XLargeFont->DisableGrowMode();

		_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);

		if(pRebirth_Window)
		{
#ifdef _ACCLAIM_IGAADSYSTEM
			if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_NONE )
			{
				pRebirth_Window->Draw();
			}
			else
			{
				if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_RESULTVIEW )
				{
					//////////////////////////////////////////////////////////////////////////
					// Result View
					POINT pos;
					pos.x = (gnWidth>>1) - 200; // width 400
					pos.y = (gnHeight>>1) - 100; // height 200

					_XDrawRectAngle( pos.x, pos.y + 12, pos.x+400-1, pos.y+200-1, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0 ) );
					_XDrawSolidBar( pos.x+1, pos.y + 13, pos.x+400-1, pos.y+200-1, D3DCOLOR_ARGB(255, 0, 0, 0 ) );
					
					m_AcclaimAdResultItemIconStatic.m_FColor = m_AcclaimAdResultTitleRightBar.m_FColor = 
						m_AcclaimAdResultTitleLeftBar.m_FColor = m_AcclaimAdResultTitleCenterBar.m_FColor = 0xFFFFFFFF;

					m_AcclaimAdResultTitleCenterBar.Draw();
					m_AcclaimAdResultTitleLeftBar.Draw();
					m_AcclaimAdResultTitleRightBar.Draw();

					g_XBaseFont->SetColor( 0xFFFFFFFF );
					g_XBaseFont->SetAlphaBlendingEnable( TRUE );

					if( !m_DeathADRecievedResult )
					{
						if( m_DeathADRecievedResultAutoCancelCounter == 0 )
							m_DeathADRecievedResultAutoCancelCounter = g_LocalSystemTime;

						DWORD lefttime = (10000 - (g_LocalSystemTime - m_DeathADRecievedResultAutoCancelCounter))  / 1000;

						if( lefttime < 20 && lefttime > 0 )						
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Wait... [%d sec]"), lefttime );
						else
						{
							g_XBaseFont->SetColor( 0xFFFF0000 );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("[ Canceled. ]") );
						}
					}
					else
					{
						m_AcclaimAdResultItemIconStatic.Draw();

						// 일단 금만 처리한다. 나중에 아이템도 같이 처리.
						if( m_DeathAD_Gold == 0 )
						{
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Karma has denied you.") );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_DeathAD_Gold );
						}
						else if( m_DeathAD_Gold >= 1 && m_DeathAD_Gold < 5 )
						{
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Gold from Heaven.") );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_DeathAD_Gold );
						}
						else if( m_DeathAD_Gold >= 5 && m_DeathAD_Gold < 10 )
						{
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Buddha smiles upon you.") );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_DeathAD_Gold );
						}
						else if( m_DeathAD_Gold >= 10 && m_DeathAD_Gold < 50 )
						{
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Emperor’s Good Fortune.") );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_DeathAD_Gold );
						}
						else if( m_DeathAD_Gold >= 50 && m_DeathAD_Gold < 100 )
						{
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Karma Payday.") );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_DeathAD_Gold );
						}
						else if( m_DeathAD_Gold >= 100 && m_DeathAD_Gold < 500 )
						{
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Blue Moon Karma Payday.") );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_DeathAD_Gold );
						}
						else if( m_DeathAD_Gold >= 500 )
						{
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Future Dragon's Luck.") );
							g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_DeathAD_Gold );
						}
					}
					g_XBaseFont->Flush();
					g_XBaseFont->SetAlphaBlendingEnable( FALSE );
				}
				else
				{
					if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_DEATH && 
						(m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_STATICAD ||
						 m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_VIDEOAD ) )
					{
						D3DCOLOR color;
						if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_VIDEOAD )
						{
							color = ((DWORD)(255.0f * g_IGAADWrapper.GetStaticImageAlphaLevel()) << 24);
						}
						else
						{
							color = ((DWORD) m_DeathStaticBackgroundAlphaLevel << 24);
						}						
						g_ScreenBurnOut[0].color = color;
						g_ScreenBurnOut[1].color = color;
						g_ScreenBurnOut[2].color = color;
						g_ScreenBurnOut[3].color = color;
						
						gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
						gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
						gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
						gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
						gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
						gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);				
						gpDev->SetTexture(0, NULL);				
						gpDev->SetFVF(D3DFVF_XTLVERTEX);
						gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX));				
						gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
					}

					g_IGAADWrapper.Render();
				}
			}
#else
			pRebirth_Window->Draw();
#endif
		}
	}
	
#ifdef _XTS_BLOODPOINT
	if(g_pLocalUser->m_BloodPoint > 0)
	{
	#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(!g_pLocalUser->GetEnterMatchEvent())
		{
	#endif
			_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
			if(pBloodPoint_Window)
			{
				if(g_LocalSystemTime - pBloodPoint_Window->GetDeadMessageStartTime() <= 3000)
				{
					g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
					g_XLargeFont->SetColor( D3DCOLOR_ARGB( 255 , 250, 230, 170 ) );
					g_XLargeFont->PutsAlign((gnWidth>>1), (gnHeight>>1)-145+20, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2735) ); // 사망으로 인해 혈이 소모 되었습니다.
					g_XLargeFont->Flush();
					g_XLargeFont->DisableGrowMode();
				}
			}
	#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		}
	#endif
	}
#endif

	_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
	if(pChat_Window)
	{
		pChat_Window->Draw();
	}

#ifdef ALL_RB_REBIRTH_USERITEM_071108_KUKURI
	_XWindow_SystemMsgDefault* pSystemMSg_Window = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	if(pSystemMSg_Window)
	{
		pSystemMSg_Window->Draw();
	}
#endif

	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=	
	_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
	if( pTutorialIcon )
	{
		pTutorialIcon->AllTutorialDraw();
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=

#ifdef _ACCLAIM_IGAADSYSTEM

#else
	// 임시
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE );
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->Draw();
	}
#endif
	
#ifdef _XTS_ITEMMALLBROWSER	
	_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
	if( pCashMall_Window )
	{
		if( pCashMall_Window->GetShowStatus() )
			pCashMall_Window->Draw();
	}		
#endif
	g_MessageBox.Draw();
	g_IMEWindow.Draw();
}

void XProc_MainGame::ProcessDyingState(void)
{
	g_Moblist.Process();
	g_Userlist.Process();

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
	{
#ifdef _ACCLAIM_IGAADSYSTEM
		if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_NONE )
		{
			pRebirth_Window->Process();
		}
#else
		pRebirth_Window->Process();
#endif
	}

	_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
	if(pChat_Window)
	{
		pChat_Window->Process();
	}
	
#ifdef ALL_RB_REBIRTH_USERITEM_071108_KUKURI
	_XWindow_SystemMsgDefault* pSystemMSg_Window = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	if(pSystemMSg_Window)
	{
		pSystemMSg_Window->Process();
	}
#endif

	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=	
	_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
	if( pTutorialIcon )
	{
		pTutorialIcon->AllTutorialProcess();
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=

	// 임시
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE );
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->Process();
	}
	
#ifdef _XDEF_USESCREEENMOTIONBLUR 
	if(g_ScrrenMotionBlurMode)
	{
		g_ScreenMotionBlurEffectManager.SetMinBlurAlpha(0.0f);
		g_ScreenMotionBlurEffectManager.SetMaxBlurAlpha(0.7f);
		g_ScreenMotionBlurEffectManager.AddBlurAlpha(0.001f);
	}
#endif
	
#ifdef _XTS_ITEMMALLBROWSER	
	_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
	if( pCashMall_Window )
	{
		if( pCashMall_Window->GetShowStatus() )
			pCashMall_Window->Process();
	}		
#endif
}

void XProc_MainGame::ProcessComaState(void)
{
#ifdef _XDEF_USESCREEENMOTIONBLUR 
	if(g_ScrrenMotionBlurMode)
	{
		g_ScreenMotionBlurEffectManager.AddBlurAlpha(-0.001f);
	}
#endif

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);

	g_Moblist.Process();
	g_Userlist.Process();
	
	if(((g_LocalSystemTime - g_pLocalUser->m_dwDieStartTime) >= g_pLocalUser->m_dwDeadPenaltyTime) && g_pLocalUser->m_dwDieStartTime != 0)
	{
		if(pRebirth_Window)
		{
			// penalty 시간 끝났음 - 장원전 진행 여부에 관련없이 모드 풀어준다.
			if(pRebirth_Window->GetCastleBattleState())
				pRebirth_Window->SetCastleBattleState(FALSE);
		}
	}

	if(pRebirth_Window)
	{
#ifdef _ACCLAIM_IGAADSYSTEM
		if( m_AcclaimADMode_DeathPhase == _XACCLAIMAD_DEATHPHASE_NONE )
		{		
			pRebirth_Window->Process();
		}
#else
		pRebirth_Window->Process();
#endif
	}

	_XWindow_ChattingDefault* pChattingDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
	if(pChattingDefault_Window)
	{
		pChattingDefault_Window->Process();
	}

#ifdef ALL_RB_REBIRTH_USERITEM_071108_KUKURI
	_XWindow_SystemMsgDefault* pSystemMSg_Window = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	if(pSystemMSg_Window)
	{
		pSystemMSg_Window->Process();
	}
#endif
	
	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=	
	_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
	if( pTutorialIcon )
	{
		pTutorialIcon->AllTutorialProcess();
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=

	// 임시
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE );
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->Process();
	}
	
#ifdef _XTS_ITEMMALLBROWSER	
	_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
	if( pCashMall_Window )
	{
		if( pCashMall_Window->GetShowStatus() )
			pCashMall_Window->Process();
	}		
#endif
	g_MessageBox.Process();
	g_IMEWindow.Process();

	// 게임 종료시 딜레이 처리
	if( m_GameExitFlag ) 
	{	
		Process_GameExit();
	}
}

//2004.06.19->oneway48 insert
void XProc_MainGame::ResetData()
{
	g_NetworkKernel.SendMessengerPacket(MMSG_NO_LOGOUT_C_S);//2004.06.21->oneway48 insert

	m_pWarehouseWindowPtr->m_bWarehouseFirstLoad = FALSE;
	m_pWarehouseWindowPtr->m_UseSlotCount = 0;
	
	m_pPCTradeWindowPtr->ResetTradeInfo();
	m_pPersonalStoreWindowPtr->ResetTradeInfo();
	m_pPersonalStoreWindowPtr->m_btnOpen->EnableWindow(TRUE);
	m_pPersonalStoreWindowPtr->m_btnCancle->EnableWindow(TRUE);
	
	_XWindow_ChattingDefault* pChattingWindowDefault = 
		(_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	
	memset(pChattingWindowDefault->m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH ); 
	pChattingWindowDefault->m_RecentChatList.disposeList();

	// OLD-----------------------------------------------------
/*	list <_XUsingItem>::iterator iter_item;
	for(iter_item = g_pInventory_Window->m_UsingItemList.begin() ; iter_item != g_pInventory_Window->m_UsingItemList.end() ; )
	{
		iter_item = g_pInventory_Window->m_UsingItemList.erase(iter_item);
	}
	g_pInventory_Window->m_UsingItemList.clear();
*/	// OLD-----------------------------------------------------

	if(g_pState_Window)
	{
		g_pState_Window->DeleteAllStateList();
	}
	
	g_QuestScriptManager.ResetQuest();
	g_QuestScriptManager.m_bQuestStartConditionCheck = FALSE;

		
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	if( pMessenger_Window )
	{
		pMessenger_Window->ReleaseMessengerList();	
		pMessenger_Window->m_listboxGroupList->DeleteAllItem();
		pMessenger_Window->m_CurrentUserStatus = 0;
		pMessenger_Window->m_TotalCountFriends = 0;
	}
	
	
	for( int windowid = 60000; windowid < 60050; windowid++)
	{
		_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
		if(pMessengerDialog_Window )
		{
			if( pMessengerDialog_Window->m_InstanceMessagePositionNumber != -1 )
			{
				g_NetworkKernel.m_bMessengerInstanceMessagePosition[ pMessengerDialog_Window->m_InstanceMessagePositionNumber ] = FALSE;
			}
			g_MainWindowManager.DeleteWindow( windowid );
		}
	}
	
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	if( pEmail_Window )
	{
		pEmail_Window->ResetEmailList();
		pEmail_Window->ResetEmailListReceiveBox();
		
		pEmail_Window->m_EmailTextTotalCount = 0;
		pEmail_Window->m_indexEmailText = 0;
		
		pEmail_Window->m_EmailReceiveTextTotalCount = 0;
		pEmail_Window->m_indexEmailReceiveText = 0;
	}

	_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
	if(pMainMenu_Window)
	{
		pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
	}

	g_pLifeSkill_Window->m_pSelectedCollectionItem = NULL;

	// 수련 정보 Reset
	g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_NONE;

	g_pLocalUser->m_bInternalUse = FALSE;

	// 보스전 오독신무 관련 Data를 초기화 시킨다.
	g_pLocalUser->m_bIsBossBattle = FALSE;
	g_pLocalUser->m_BossStartTime = 0;
	g_pLocalUser->m_BossTimeLimit = 0;
	
	// 툴팁 Reset
	if( g_pDefaultTooltip_Window )
		g_pDefaultTooltip_Window->ShowWindow( FALSE );

	// 분파정보 Reset
	if( g_pGroup_Window )
		g_pGroup_Window->ResetData();
	
	// 장식물장착 Reset
	_XWindow_SocketPlugIn* pSocket_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN );
	if( pSocket_Window )
		pSocket_Window->ResetData();

	// 게이지 Reset
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE );
	if( pChargeGauge_Window )
		pChargeGauge_Window->ShowWindow( FALSE );
	
	// 겜블박스 Reset
	_XWindow_Gamble* pGamble_Window = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
	if( pGamble_Window )
		pGamble_Window->ResetData();
	
	// 별호 창이 열려 있으면 닫는다.
	_XWindow_NickName* pNickNameWindow = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
	if( pNickNameWindow )
	{
		pNickNameWindow->ResetNicknameInfo();

		if( pNickNameWindow->GetShowStatus() )
			pNickNameWindow->ShowWindow( FALSE );
	}	
	
	// 인물 정보 창이 열려 있으면 닫는다.
	_XWindow_TargetInfo* pTargetInfoWindow = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
	if( pTargetInfoWindow )
	{
		if( pTargetInfoWindow->GetShowStatus() )
			pTargetInfoWindow->ShowWindow(FALSE);
	}

	_XWindow_QuestRank* pQuestRank_Window = (_XWindow_QuestRank*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTRANKWINDOW);
	if(pQuestRank_Window)
	{		
		if(pQuestRank_Window->GetShowStatus())
		{
			pQuestRank_Window->ShowWindow(FALSE);
		}
	}
	
	_XWindow_GuideLine* pGuideLine_Window = (_XWindow_GuideLine*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GUIDELINEWINDOW);
	if( pGuideLine_Window )
	{
		if( pGuideLine_Window->GetShowStatus() )
		{
			pGuideLine_Window->ShowWindow(FALSE);
		}
	}
	
	if(g_pJinStatus_Window)
		g_pJinStatus_Window->SetButtonStatus(FALSE);
	
	_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
	if(pEnterBossRoom_Window)
	{
		pEnterBossRoom_Window->ShowWindow(FALSE);
		pEnterBossRoom_Window->ResetNeedItemInfo();
	}

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XWindow_NPCMnDInfo* pNPCMnDInfo_Window = (_XWindow_NPCMnDInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCMNDWINDOW);
	if(pNPCMnDInfo_Window && pNPCMnDInfo_Window->GetShowStatus())
	{
		pNPCMnDInfo_Window->ShowWindow(FALSE);
	}
#endif

	
#ifdef _XTS_ITEMMALLBROWSER	
	// 아이템 몰 로그 오프 시킨다..
	g_bItemMallLogOff = TRUE;
	
#ifdef _XKOREAN
	// 아이템 몰 로그 오프 시킨다..
	if( !g_bTestWebPageAddress )
		DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com/logoff.asp");
	else
		DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://panda.nexon.com/logoff.asp");		
#endif	
	
#ifdef _XENGLISH 
	//	DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com/logoff.asp");	
#endif
	
#ifdef _XVIETNAMESE
	//DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://www.cuulong.com.vn/ktc/logout.aspx");	
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulongid.zing.vn/ktc/logout.aspx");	
#endif
	
#ifdef _XJAPANESE
	//DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninetest.gameleon.jp/item_mall/item_logout.asp");
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://nine.gameleon.jp/item_mall/item_logout.asp");
#endif	

#ifdef _XTAIWANESE
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://member.pfamily.com.tw/9ds_shopping_mall/logout.php");
#endif

	_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
	if( pCashBox_Window )
	{
		pCashBox_Window->m_listboxCashItem->DeleteAllItem();
		pCashBox_Window->ShowWindow( FALSE );
	}
#endif
				
	// 베트남 셧다운 
#ifdef _XDEF_SHUTDOWN_20061027	
	g_PlayTime = 0;
#endif

	//영웅단 동맹
#ifdef _XTS_ALIANCE_20061018	
	if( g_pGroup_Window )
		g_pGroup_Window->ResetAlienceInfo();
	
	_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
	if(pRequestParty_Window)
	{
		pRequestParty_Window->m_RequestAlienceButton->EnableWindow(FALSE);
	}
#endif
	
	
#ifdef _XDEF_CASTLEBATTLE
	//장원전 시간표시 관련
	_XWindow_CastleBattlePersonalHistory* pCastleBattlePersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCastleBattlePersonalHistory_Window)
	{
		pCastleBattlePersonalHistory_Window->ShowWindow(FALSE);
	}
	_XWindow_CastleBattleTimeChange* pCBTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
	if(pCBTimechange_Window)
	{
		pCBTimechange_Window->ShowWindow(FALSE);
	}
#ifdef _XDEF_CASTLEBATTLE_RECORD
	// 장원 전적
	_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
	if(pCBRecord_Window)
	{
		pCBRecord_Window->ShowWindow(FALSE);
	}
#endif
#endif
	
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214
	::ShowWindow(g_hWebEventWindow, SW_HIDE);
#endif
}

//2004.07.01->oneway48 insert
void XProc_MainGame::WindowCloseBeforeWideView()
{		
	_XWindow_QuestWindow* pQuest_Window = (_XWindow_QuestWindow*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUESTWINDOW );
	_XWindow_Party* pParty_Window = (_XWindow_Party*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PARTY );
	_XWindow_Option* pOption_Window = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
	_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );	
	_XWindow_MaximumMap* pMaxM_Window = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
	_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
	_XWindow_NPCSearch *pNPCSearch_Window = (_XWindow_NPCSearch*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSEARCHWINDOW);
	_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
	
	if( pPaymentMethod_Window )
		if(pPaymentMethod_Window->GetShowStatus()) pPaymentMethod_Window->ShowWindow(FALSE);

	if( pPCRPremium_Window )
		if(pPCRPremium_Window->GetShowStatus()) pPCRPremium_Window->ShowWindow(FALSE);

	if( pMainMenu_Window )
		if(pMainMenu_Window->GetShowStatus()) pMainMenu_Window->ShowWindow(FALSE);
		
	if( pMessenger_Window )
		if(pMessenger_Window->GetShowStatus()) pMessenger_Window->ShowWindow(FALSE);
	
	if( g_pJinStatus_Window )
		if(g_pJinStatus_Window->GetShowStatus()) g_pJinStatus_Window->ShowWindow(FALSE);
	
	if( pMonsterStatus_Window )
		if(pMonsterStatus_Window->GetShowStatus()) pMonsterStatus_Window->ShowWindow(FALSE);
	
	if(pMaxM_Window)
		if(pMaxM_Window->GetShowStatus()) pMaxM_Window->ShowWindow(FALSE);
	
	if(g_pInventory_Window)
		if(g_pInventory_Window->GetShowStatus()) g_pInventory_Window->ShowWindow(FALSE);

	if(pQuest_Window)
		if(pQuest_Window->GetShowStatus()) pQuest_Window->ShowWindow(FALSE);

	if(pParty_Window)
		if(pParty_Window->GetShowStatus()) pParty_Window->ShowWindow(FALSE);

	if(pOption_Window)
		if(pOption_Window->GetShowStatus()) pOption_Window->ShowWindow(FALSE);
		
	if(g_pMinimap_Window)
		if(g_pMinimap_Window->GetShowStatus()) g_pMinimap_Window->ShowWindow(FALSE);
			
	if(pNPCSearch_Window)
		if(pNPCSearch_Window->GetShowStatus()) pNPCSearch_Window->ShowWindow(FALSE);
		
	if( pChatDefault_Window )
	{
		if(pChatDefault_Window->GetShowStatus())
		{					
			_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
			m_ChatEditContainer.ClearBuffer();
#endif			
			m_ChatEditContainer.ShowWindow( FALSE );	
			pChatDefault_Window->ShowWindow( FALSE );
			pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();
			pChatDefault_Window->m_WhisperEditContainer.ShowWindow(FALSE);
		}
	}

	if(g_pQuickSlot_Window)
		if(g_pQuickSlot_Window->GetShowStatus()) g_pQuickSlot_Window->ShowWindow(FALSE);
	if(g_pState_Window)
		if(g_pState_Window->GetShowStatus()) g_pState_Window->ShowWindow(FALSE);
				
	if(m_pMainFrameWindowPtr)
		if(m_pMainFrameWindowPtr->GetShowStatus()) m_pMainFrameWindowPtr->ShowWindow(FALSE);

	_XWindow_SocketPlugIn* pSocketWindowPtr = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SOCKETPLUGIN );
	if( pSocketWindowPtr )
	{
		if( pSocketWindowPtr->GetShowStatus() )
		{
			m_pSocketPlugInWindowPtr->ShowWindow(FALSE);
			m_pSocketPlugInWindowPtr->ResetSocketItemList();
			g_pInventory_Window->ResetSocketIndexList();								
		}
	}
	_XWindow_Gamble* pGambleWindowPtr = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
	if( pGambleWindowPtr )
	{
		if( pGambleWindowPtr->GetShowStatus() )
		{
			m_pGambleBoxWindowPtr->ShowWindow(FALSE);
			m_pGambleBoxWindowPtr->ResetSocketItemList();
			g_pInventory_Window->ResetSocketIndexList();
		}
	}
	
	_XWindow_QuestRank* pQuestRank_Window = (_XWindow_QuestRank*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTRANKWINDOW);
	if(pQuestRank_Window)
	{		
		if(pQuestRank_Window->GetShowStatus())
		{
			pQuestRank_Window->ShowWindow(FALSE);
		}
	}

	_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
	if(pBloodPoint_Window)
	{
		pBloodPoint_Window->ShowWindow(FALSE);
	}

	_XWindow_AccountItemUse* pAccountItemUse_Window = (_XWindow_AccountItemUse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ACCOUNTITEMUSE);
	if(pAccountItemUse_Window)
	{
		if(pAccountItemUse_Window->GetShowStatus())
			pAccountItemUse_Window->ShowWindow(FALSE);
	}

//Author : 양희왕
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPk_TracingServicesWindow = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if( pPk_TracingServicesWindow->GetShowStatus() ) 
		pPk_TracingServicesWindow->ShowWindow(FALSE);
#endif
//Last Updated : 07/04/05

//Author : 양희왕 //breif : 용루 용린창 
#ifdef _XDEF_SMELT_ITEM
	XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
	if( pSmletItemWindow->GetShowStatus()) 
		pSmletItemWindow->ShowWindow(FALSE);
#endif

//Author : 양희왕 //breif : 친구소환
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
	_XWindow_RecallItem* pRecallItemWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
	if( pRecallItemWindow && pRecallItemWindow->GetShowStatus() ) 
		pRecallItemWindow->ShowWindow(FALSE);
#endif

//Author : 양희왕 //breif : 장식물제거
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : 양희왕
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);
	if( pSocketDetachWindow && pSocketDetachWindow->GetShowStatus() )
		pSocketDetachWindow->ShowWindow(FALSE);
#endif

#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 아이콘
	_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
	if( pEventIcon  && pEventIcon->GetShowStatus() )
		pEventIcon->ShowWindow(FALSE);
#endif

	if(g_pExp_Window)
	{
		if(g_pExp_Window->GetShowStatus())
			g_pExp_Window->ShowWindow(FALSE);
	}


	//Author : 양희왕 //breif : 독무아이콘
	_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
	if(pPoison_Window && pPoison_Window->GetShowStatus() )
	{
		pPoison_Window->ShowWindow(FALSE);
	}

    _XWindow_Info* pWindowInfo = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
	if(pWindowInfo)
	{
        pWindowInfo->ShowWindow(FALSE);
	}

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(g_pLocalUser->m_nWinnerNickName > 0)
	{
//		if(g_pRewardNick_Window && g_pRewardNick_Window->GetShowStatus())
//			g_pRewardNick_Window->ShowWindow(FALSE);
	}
#endif

#ifdef _XDEF_NPCBUFF_CASH_080410_KUKURI
	if(m_pNPCBuffWindowPtr)
	{
        if(m_pNPCBuffWindowPtr->GetShowStatus())
		{
            m_pNPCBuffWindowPtr->ShowWindow(FALSE);
        }
    }
#endif

    _XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
    {
        //memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));
		pPKTracingRequest_Window->ShowWindow(FALSE);
    }

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XWindow_NPCMnDInfo* pNPCMnDInfo_Window = (_XWindow_NPCMnDInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCMNDWINDOW);
	if(pNPCMnDInfo_Window && pNPCMnDInfo_Window->GetShowStatus())
	{
		pNPCMnDInfo_Window->ShowWindow(FALSE);
	}
	
	_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
	if(pMnDCurrentPoint_Window)
	{
		pMnDCurrentPoint_Window->ShowWindow(FALSE);
	}
	
	_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
	if(pMnDTotalPoint_Window)
	{
		pMnDTotalPoint_Window->ShowWindow(FALSE);
	}
#endif
}

//Author : 양희왕
void XProc_MainGame::BaseInterfaceWindowOpen()
{
	m_NPCDialogScreenAlphaLevel-= _XDEF_NPCDIALOGMODE_FADE_LEVEL;

	if( m_NPCDialogScreenAlphaLevel <= 0 )
	{
		m_NPCDialogScreenAlphaLevel = 0;
		m_bNPCFadeInOutFlag = FALSE;
		m_bFunctionalObjectFadeInOut = FALSE;
		
		g_pLocalUser->m_CharacterLog.isTalk = FALSE;
		g_pLocalUser->m_CharacterLog.talkTime = 0;
		
		g_pLocalUser->m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
		
		
		_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
		defaultsystemmsgwindow->ShowWindow(TRUE);
        defaultsystemmsgwindow->SetDockingMode(g_pInventory_Window->m_bExpandDockingBorderFlag);
		//defaultsystemmsgwindow->MoveWindow(gnWidth - 374, gnHeight-195);
		//defaultsystemmsgwindow->m_SystemMsgListScrollBar->MoveWindow(gnWidth - 13 ,gnHeight - 182);
		defaultsystemmsgwindow->m_SystemMsgListScrollBar->SetLineScrollPos( 999999 ); 
	}
	
	// 대화 모드 끝이 나면 미니맵을 다시 띄운다.
	if(!g_pMinimap_Window->GetShowStatus()) 
		g_pMinimap_Window->ShowWindow(TRUE);		
		

    _XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
	_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		
	// 패왕릉에서도 결재 정보 보이도록 한다 - 위치 변경해줄 것
	_XWindow_AccountItemUse* pAccountItemUse_Window = (_XWindow_AccountItemUse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ACCOUNTITEMUSE);
	if(pAccountItemUse_Window)
	{
		if(g_pLocalUser->m_AccountItemsID > 0)
		{
			pAccountItemUse_Window->ShowWindow(TRUE);
		}
	}

#ifdef _XDEF_SHUTDOWN_20061027
	_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
	if( !pPaymentMethod_Window->GetShowStatus() ) 
	{
		pPaymentMethod_Window->ShowWindow( TRUE );
	}
#endif

	_XWindow_PCRPremium* PCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
	if( !PCRPremium_Window->GetShowStatus() ) 
	{
		if( g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREMIUM ) || g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREPAID ) )
		{
			PCRPremium_Window->ShowWindow( TRUE );
		}
	}
		
//Author : 양희왕
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPk_TracingServicesWindow = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if( pPk_TracingServicesWindow->GetShowStatus() ) 
		pPk_TracingServicesWindow->ShowWindow(TRUE);
#endif
//Last Updated : 07/04/05

    _XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
    {
		//Author : 양희왕  추가
		memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));

        int nLen = strlen(pPKTracingRequest_Window->m_cTargetName);
        if( nLen )
            pPKTracingRequest_Window->ShowWindow(TRUE);
    }
	
	if( pEnterBossRoom_Window ) pEnterBossRoom_Window->ShowWindow(FALSE);		
	if(!pMainMenu_Window->GetShowStatus()) pMainMenu_Window->ShowWindow(TRUE);
	if(!g_pJinStatus_Window->GetShowStatus()) g_pJinStatus_Window->ShowWindow(TRUE);
	if(!pChatDefault_Window->GetShowStatus()) pChatDefault_Window->ShowWindow(TRUE);
	if(!pBloodPoint_Window->GetShowStatus()) pBloodPoint_Window->ShowWindow(TRUE);

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	if(g_pExp_Window)
	{
		if(!g_pExp_Window->GetShowStatus())
			g_pExp_Window->ShowWindow(TRUE);
	}
#endif
		
	if(g_pQuickSlot_Window)
	{
		if(!g_pQuickSlot_Window->GetShowStatus())
			g_pQuickSlot_Window->ShowWindow(TRUE);
	}
	if(g_pState_Window)
	{
		if(!g_pState_Window->GetShowStatus())
			g_pState_Window->ShowWindow(TRUE);
	}

	if(pBloodPoint_Window)
	{
		pBloodPoint_Window->ShowWindow(TRUE);
	}
	
	// 전직 시 나오는 강제 툴팁---------------------------------------=
	if( !g_TutorialInfo->_XTutorial_043 )
	{
		if( g_pLocalUser->m_CharacterInfo.Get_cClass() )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_043);
			g_TutorialInfo->_XTutorial_043 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
		}			
	}

#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 아이콘
	_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
	_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
	if( pEventIcon && pEvent_Window)
	{
		if( pEvent_Window->GetCurrentEventStatus() || //아이템을 받지 않았다. 진행완료 못한 이벤트가 있다.
            pEvent_Window->m_nEventIDinfo || //레벨업 이벤트를 받지 않았다.
            (!pEventIcon->GetShowMessageBox() && g_pLocalUser->m_EventSkillList.m_sSkillID) || //경공 선물 창을 클릭하지 않았다.
			pEvent_Window->m_bBuffEvent ) //버프이벤트 알림
			pEventIcon->ShowWindow(TRUE);
		else
			pEventIcon->ShowWindow(FALSE);
	}
#endif

	//Author : 양희왕 //breif : 독무아이콘 //패왕릉에서는 띄운다 //패왕인지 보스인지는 패킷에서 처리 하므로,, 창만 띄운다.
	if( g_CurrentZoneInfoIndex == 21 || 
        (g_CurrentZoneInfoIndex >= 50 && g_CurrentZoneInfoIndex <= 57) )// 패왕릉에서는 독무 아이콘이 표시된다.
	{
		_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
		if(pPoison_Window && !pPoison_Window->GetShowStatus() )
		{
			pPoison_Window->ShowWindow(TRUE);
		}
	}

    _XWindow_Info* pWindowInfo = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
	if(pWindowInfo)
	{
        _XCastleInfo* pCastleInfo = g_CBManager.GetCastleInfo(g_CurrentZoneInfoIndex);
		if(pCastleInfo)
        {
            pWindowInfo->ShowWindow(TRUE);
        }
	}

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(g_pLocalUser->m_nWinnerNickName > 0)
	{
//		if(g_pRewardNick_Window && !g_pRewardNick_Window->GetShowStatus())
//			g_pRewardNick_Window->ShowWindow(TRUE);
	}
#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
	if(pMnDCurrentPoint_Window)
	{
		pMnDCurrentPoint_Window->ShowWindow(TRUE);
	}
	
	_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
	if(pMnDTotalPoint_Window)
	{
		pMnDTotalPoint_Window->ShowWindow(TRUE);
	}
#endif
}


void XProc_MainGame::SetNPCMode(_XNPC_MODE npcmode)
{
	if( npcmode != _XNPC_NONE)
	{
		if( m_pNPCScriptWindowPtr->m_NPCMode == npcmode )	return;
	}
	
	m_pNPCScriptWindowPtr->m_NPCMode = npcmode;
	m_pNPCScriptWindowPtr->m_bProgressNPCDialogMessage = FALSE;
	m_pNPCScriptWindowPtr->m_listboxNPCDialogMessage->DeleteAllItem();//모드가 바뀌면 대화내용을 지운다.	
	m_pNPCScriptWindowPtr->m_imageDialogNextPageLeftBorder->ShowWindow(FALSE);
	m_pNPCScriptWindowPtr->m_imageDialogNextPageRightBorder->ShowWindow(FALSE);
	m_pNPCScriptWindowPtr->m_btnDialogNextPage->ShowWindow(FALSE);

	// 이벤트 관련 입력창을 초기화 시킨다. 
	if( m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetShowStatus() )
	{
		m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ShowWindow(FALSE);
		m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer(); 
		_XIMEKERNEL.SetFocus( NULL );	
		_XIMEKERNEL.ResetCombinationBuffer();		
	}
	m_NPCWindowMoveFast = m_NPCWindowMoveSlow = 0.0f;
	m_bMoveNPCWindow = TRUE;
	
	if(m_pNPCTradeWindowPtr->GetShowStatus())
	{
		m_pNPCTradeWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pWarehouseWindowPtr->GetShowStatus())
	{ 
		m_pWarehouseWindowPtr->ShowWindow(FALSE);
	}
	
	if(g_pInventory_Window->GetShowStatus())
	{						
		g_pInventory_Window->ShowWindow(FALSE);
	}
	
	if(m_pAddSaveItemWindowPtr->GetShowStatus())
	{						
		m_pAddSaveItemWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pNPCSkillWindowPtr->GetShowStatus())
	{						
		m_pNPCSkillWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pContributionWindowPtr->GetShowStatus())
	{
		m_pContributionWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pRepairItemWindowPtr->GetShowStatus())
	{
		m_pRepairItemWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pInchantWindowPtr->GetShowStatus())
	{
		m_pInchantWindowPtr->ShowWindow(FALSE);
	}

	if(m_pPVPBrokerWindowPtr->GetShowStatus())
	{
		m_pPVPBrokerWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pNPCCollectionWindowPtr->GetShowStatus())
	{
		m_pNPCCollectionWindowPtr->ShowWindow(FALSE);
	}
	
	if(m_pNPCResourceWindowPtr->GetShowStatus())
	{
		m_pNPCResourceWindowPtr->ShowWindow(FALSE);
	}

	if(m_pNPCResource_New_windowptr->GetShowStatus())
	{
		m_pNPCResource_New_windowptr->ShowWindow(FALSE);
	}
	
	if(m_pNPCBuffWindowPtr->GetShowStatus())
	{
		m_pNPCBuffWindowPtr->ShowWindow(FALSE);
	}

#ifdef _XTS_MOVEVILLAGE
	if(m_pMoveVillageWindowPtr->GetShowStatus())
	{
		m_pMoveVillageWindowPtr->ShowWindow(FALSE);
	}
#endif

	_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
	if(pCastleInfo_Window)
	{
		pCastleInfo_Window->ShowWindow(FALSE);
	}

	_XWindow_ChallengerList* pChallengerList_Window = (_XWindow_ChallengerList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHALLENGERLIST);
	if(pChallengerList_Window)
	{
		pChallengerList_Window->ShowWindow(FALSE);
	}
	
	_XWindow_QuestRank* pQuestRank_Window = (_XWindow_QuestRank*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTRANKWINDOW);
	if(pQuestRank_Window)
	{
		
		if(pQuestRank_Window->GetShowStatus())
		{
			pQuestRank_Window->ShowWindow(FALSE);
		}
	}
	
	_XWindow_MixWeapon* pMixWeapon_Window = (_XWindow_MixWeapon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MIXWEAPON_WINDOW);
	if(pMixWeapon_Window)
	{
		if(pMixWeapon_Window->GetShowStatus())
			pMixWeapon_Window->ShowWindow(FALSE);
	}

	_XWindow_FreeMatchInfo* pFMInfo_Window = (_XWindow_FreeMatchInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FREEMATCHINFO);
	if(pFMInfo_Window && pFMInfo_Window->GetShowStatus())
	{
		pFMInfo_Window->ShowWindow(FALSE);
	}

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XWindow_NPCMnDInfo* pNPCMnDInfo_Window = (_XWindow_NPCMnDInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCMNDWINDOW);
	if(pNPCMnDInfo_Window && pNPCMnDInfo_Window->GetShowStatus())
	{
		pNPCMnDInfo_Window->ShowWindow(FALSE);
	}
#endif
}

void XProc_MainGame::DrawNPCWindowMove()
{	
	m_NPCWindowMoveFast -= 150.0f;
	if( m_NPCWindowMoveFast <= _XDEF_NPCSCRIPT_DRAWPOSX )
		m_NPCWindowMoveFast = _XDEF_NPCSCRIPT_DRAWPOSX;
	
	m_NPCWindowMoveSlow -= 100.0f;
	if( m_NPCWindowMoveSlow <= _XDEF_NPCSCRIPT_DRAWPOSX ) 
		m_NPCWindowMoveSlow = _XDEF_NPCSCRIPT_DRAWPOSX;			
	
	
	switch(m_pNPCScriptWindowPtr->m_NPCMode) 
	{
	case _XNPC_NONE:
		{	
			m_NPCWindowMoveFast = m_NPCWindowMoveSlow = _XDEF_NPCSCRIPT_DRAWPOSX;
			m_bMoveNPCWindow = FALSE;
		}
		break;
	case _XNPC_ITEMTRADE: 
		{			
			if( m_pNPCTradeWindowPtr && g_pInventory_Window )
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pNPCTradeWindowPtr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);							
					m_pNPCTradeWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );						
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pNPCTradeWindowPtr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				m_pNPCTradeWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );	
				
			}
		}
		break;
	case _XNPC_WAREHOUSE:
		{			
			if( m_pWarehouseWindowPtr && g_pInventory_Window )
			{							
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pWarehouseWindowPtr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);
					m_pWarehouseWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pWarehouseWindowPtr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				m_pWarehouseWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}
		}
		break;
	case _XNPC_CONTRACTWAREHOUSE:
		{
			if( m_pAddSaveItemWindowPtr && g_pInventory_Window )
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pAddSaveItemWindowPtr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);
					m_pAddSaveItemWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pAddSaveItemWindowPtr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				m_pAddSaveItemWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );	
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}			
		}
		break;
	case _XNPC_LEARNSKILL:
		{			
			if( m_pNPCSkillWindowPtr )
			{					
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pNPCSkillWindowPtr->ShowWindow(TRUE);	
					m_pNPCSkillWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );	
					g_MainWindowManager.SetTopWindow(m_pNPCSkillWindowPtr);
				}		
				m_pNPCSkillWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}						
		}
		break;
	case _XNPC_REPAIRITEM:
		{	
			if( m_pRepairItemWindowPtr && g_pInventory_Window )
			{							
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pRepairItemWindowPtr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);	
					m_pRepairItemWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );						
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pRepairItemWindowPtr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				m_pRepairItemWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );				
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );				
				

			}
		}
		break;
	case _XNPC_CONTRIBUTION:
		{
			if( m_pContributionWindowPtr && g_pInventory_Window )
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pContributionWindowPtr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);	
					m_pContributionWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );						
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pContributionWindowPtr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				m_pContributionWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );				
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );				
				

			}
		}
		break;
	case _XNPC_SOCKETPLUGINEX :
		{
			if( m_pSocketPlugInExWindowPtr && g_pInventory_Window )
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pSocketPlugInExWindowPtr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);	
					m_pSocketPlugInExWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );						
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pSocketPlugInExWindowPtr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				m_pSocketPlugInExWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );				
				
			}
		}
		break;
	case _XNPC_PVPBROKER :
		{			
			if( m_pPVPBrokerWindowPtr )
			{					
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pPVPBrokerWindowPtr->ShowWindow(TRUE);
					m_pPVPBrokerWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );	
					g_MainWindowManager.SetTopWindow(m_pPVPBrokerWindowPtr);
				}		
				m_pPVPBrokerWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}						
		}
		break;
	
	case _XNPC_COLLECTION :
		{
			if( m_pNPCCollectionWindowPtr )
			{					
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pNPCCollectionWindowPtr->ShowWindow(TRUE);
					m_pNPCCollectionWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );	
					g_MainWindowManager.SetTopWindow(m_pNPCCollectionWindowPtr);
				}		
				m_pNPCCollectionWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}					
		}
		break;
	case _XNPC_RESOURCE :
		{
#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI
			if( m_pNPCResource_New_windowptr )
			{					
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pNPCResource_New_windowptr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);
					m_pNPCResource_New_windowptr->SetWindowMoveMode( _XWMOVE_FIXED );
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pNPCResource_New_windowptr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}		
				m_pNPCResource_New_windowptr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
			}
#else
			if( m_pNPCResourceWindowPtr )
			{					
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pNPCResourceWindowPtr->ShowWindow(TRUE);						
					m_pNPCResourceWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );		
					g_MainWindowManager.SetTopWindow(m_pNPCResourceWindowPtr);
				}		
				m_pNPCResourceWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}
#endif
		}
		break;
	case _XNPC_BUFF :
		{
			if( m_pNPCBuffWindowPtr )
			{					
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pNPCBuffWindowPtr->ShowWindow(TRUE);
					m_pNPCBuffWindowPtr->SetViewMode(EN_MODE_NPC_VIEW);
					m_pNPCBuffWindowPtr->SetListItem();
					m_pNPCBuffWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pNPCBuffWindowPtr);
				}		
				m_pNPCBuffWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}					
		}
		break;
	case _XNPC_INCHANT :
		{
			if( m_pInchantWindowPtr && g_pInventory_Window )
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pInchantWindowPtr->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);	
					m_pInchantWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pInchantWindowPtr);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				m_pInchantWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );				
				

			}
		}
		break;
	case _XNPC_RANKQUEST :
		{			
			_XWindow_QuestRank* pQuestRank_Window = (_XWindow_QuestRank*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTRANKWINDOW);
			if(pQuestRank_Window)
			{
				if( pQuestRank_Window )
				{					
					if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
					{
						pQuestRank_Window->ShowWindow(TRUE);
						pQuestRank_Window->SetWindowMoveMode( _XWMOVE_FIXED );	
						g_MainWindowManager.SetTopWindow(pQuestRank_Window);
					}		
					pQuestRank_Window->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
					
				}	
			}
		}
		break;
	case _XNPC_MOVEVILLAGE :
		{
			if( m_pMoveVillageWindowPtr )
			{					
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					m_pMoveVillageWindowPtr->ShowWindow(TRUE);		
					m_pMoveVillageWindowPtr->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(m_pMoveVillageWindowPtr);
				}		
				m_pMoveVillageWindowPtr->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				
			}					
		}
		break;
	case _XNPC_CASTLEINFO :
		{
			_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
			if(pCastleInfo_Window)
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					pCastleInfo_Window->ShowWindow(TRUE);		
					pCastleInfo_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(pCastleInfo_Window);
				}		
				pCastleInfo_Window->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
			}
		}
		break;
	case _XNPC_CHALLENGEINFO :
		{
			_XWindow_ChallengerList* pChallengerList_Window = (_XWindow_ChallengerList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHALLENGERLIST);
			if(pChallengerList_Window)
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					pChallengerList_Window->ShowWindow(TRUE);		
					pChallengerList_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(pChallengerList_Window);
				}		
				pChallengerList_Window->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
			}
		}
		break;
	case _XNPC_MIXWEAPON :
		{
			_XWindow_MixWeapon* pMixWeapon_Window = (_XWindow_MixWeapon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MIXWEAPON_WINDOW);
			if(pMixWeapon_Window && g_pInventory_Window )
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					pMixWeapon_Window->ShowWindow(TRUE);
					g_pInventory_Window->ShowWindow(TRUE);	
					pMixWeapon_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(pMixWeapon_Window);
					g_MainWindowManager.SetTopWindow(g_pInventory_Window);
				}
				pMixWeapon_Window->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
				g_pInventory_Window->MoveWindow( (gnWidth)+ m_NPCWindowMoveSlow + 183, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );				
			}
		}
		break;
	case _XNPC_FREEMATCHINFO :
		{
			_XWindow_FreeMatchInfo* pFMInfo_Window = (_XWindow_FreeMatchInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FREEMATCHINFO);
			if(pFMInfo_Window)
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					pFMInfo_Window->ShowWindow(TRUE);		
					pFMInfo_Window->SetWindowMoveMode( _XWMOVE_FIXED );
					g_MainWindowManager.SetTopWindow(pFMInfo_Window);
				}		
				pFMInfo_Window->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
			}
		}
		break;
	case _XNPC_MASTERINFO :
		{
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
			_XWindow_NPCMnDInfo* pNPCMnDInfo_Window = (_XWindow_NPCMnDInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCMNDWINDOW);
			if(pNPCMnDInfo_Window)
			{
				if( m_NPCWindowMoveFast == -150.0f && m_NPCWindowMoveSlow == -100.0f)
				{
					pNPCMnDInfo_Window->ShowWindow(TRUE);
					pNPCMnDInfo_Window->SetWindowMoveMode( _XWMOVE_FIXED );	
					g_MainWindowManager.SetTopWindow(pNPCMnDInfo_Window);
				}		
				pNPCMnDInfo_Window->MoveWindow( (gnWidth) + m_NPCWindowMoveFast, (gnHeight>>1)-_XDEF_NPCSCRIPT_DRAWPOSY );
			}
		#endif	
		}
		break;

	}

	if( m_NPCWindowMoveFast == _XDEF_NPCSCRIPT_DRAWPOSX && m_NPCWindowMoveSlow == _XDEF_NPCSCRIPT_DRAWPOSX)
	{
		m_bMoveNPCWindow = FALSE;
	}
	
}

void XProc_MainGame::InitializeTraining(void)
{
	g_pLocalUser->ResetTrainingMode();

	_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
	if(pDummy_Window)
	{
		if(pDummy_Window->GetShowStatus())
			pDummy_Window->ShowWindow(FALSE);
	}

	_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
	if(pSpell_Window)
	{
		if(pSpell_Window->GetShowStatus())
			pSpell_Window->ShowWindow(FALSE);
	}

	_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
	if(pMeditation_Window)
	{
		if(pMeditation_Window->GetShowStatus())
			pMeditation_Window->ShowWindow(FALSE);
	}
	
	_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
	if(pTrainingHelp_Window)
	{
		if(pTrainingHelp_Window->GetShowStatus())
			pTrainingHelp_Window->ShowWindow(FALSE);
	}

	// Load Dummy position file
	if(g_CurrentZoneInfoIndex > 6)
		return;

	if( !g_ScriptArchive.ReOpenPackage() )
			return;

	FILE* dummyinfofile = NULL;

	dummyinfofile = g_ScriptArchive.GetPackedFile(_T("dummy.txt"));

	if(!dummyinfofile)
	{
		g_ScriptArchive.ClosePackage();
		return;
	}

	unsigned short index = 0;
	int groupindex = 0, count = 0;
	D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_XGROUPTYPE grouptype;
	BOOL	bReadCount = FALSE;

	TCHAR buffer[256];

	do 
	{		
		if(feof(dummyinfofile)) 
			break;
		
		memset(buffer, 0, sizeof(TCHAR)*256);
		if(!fgets(buffer, 255, dummyinfofile))
		{
			break;
		}
		
		if(strncmp( buffer, _T("_XM_EOS"), 7) == 0) 
			break;
		
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(!bReadCount)
			{
				sscanf(buffer, "%d", &count);
				groupindex++;
				bReadCount = TRUE;
				index = 0;
			}
			else
			{
				sscanf(buffer, "%f %f %f", &position.x, &position.y, &position.z);

/*				float x, z, ratio;
				ratio = (float)(g_LodTerrain.m_VertexCount-1)/(float)(g_LodTerrain.m_VertexCount);
				x = position.x + (g_LodTerrain.m_Width*0.5f);
				z = position.z + (g_LodTerrain.m_Height*0.5f);

				x = (x*g_LodTerrain.m_Width) / (g_LodTerrain.m_Width*ratio);
				z = (z*g_LodTerrain.m_Height) / (g_LodTerrain.m_Height*ratio);

				position.x = x - (g_LodTerrain.m_Width*0.5f);
				position.z = z - (g_LodTerrain.m_Height*0.5f);
*/				
				index++;

				if(g_CurrentZoneInfoIndex == groupindex)
				{
					switch(g_CurrentZoneInfoIndex)
					{
					case 1 :
						grouptype = _XGROUP_SORIM;
						break;
					case 2 :
						grouptype = _XGROUP_GAEBANG;
						break;
					case 3 :
						grouptype = _XGROUP_NOCKRIM;
						break;
					case 4 :
						grouptype = _XGROUP_BEEGOONG;
						break;
					case 5 :
						grouptype = _XGROUP_MOODANG;
						break;
					case 6 :
						grouptype = _XGROUP_MAKYO;
						break;
					}
					
					_XDummy* pDummy = new _XDummy;
					pDummy->Create(index-1, grouptype, position);
					g_DummyList.Add(pDummy);
				}
			}
			if(index == count)
				bReadCount = FALSE;
		}
		
	} while( !feof(dummyinfofile) );

	// Load seat

	FILE* seatinfofile = NULL;

	seatinfofile = g_ScriptArchive.GetPackedFile(_T("seat.txt"));

	if(!seatinfofile)
	{
		g_ScriptArchive.ClosePackage();
		return;
	}

	index = 0;
	groupindex = 0;
	count = 0;
	bReadCount = FALSE;

	do 
	{
		if(feof(seatinfofile))
			break;

		memset(buffer, 0, sizeof(TCHAR)*256);
		if(!fgets(buffer, 255, seatinfofile))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(!bReadCount)
			{
				sscanf(buffer, "%d", &count);
				groupindex++;
				bReadCount = TRUE;
				index = 0;
			}
			else
			{
				sscanf(buffer, "%f %f %f", &position.x, &position.y, &position.z);
				index++;

/*				float x, z, ratio;
				ratio = (float)(g_LodTerrain.m_VertexCount-1)/(float)(g_LodTerrain.m_VertexCount);
				x = position.x + (g_LodTerrain.m_Width*0.5f);
				z = position.z + (g_LodTerrain.m_Height*0.5f);
				
				x = (x*g_LodTerrain.m_Width) / (g_LodTerrain.m_Width*ratio);
				z = (z*g_LodTerrain.m_Height) / (g_LodTerrain.m_Height*ratio);
				
				position.x = x - (g_LodTerrain.m_Width*0.5f);
				position.z = z - (g_LodTerrain.m_Height*0.5f);

				if(groupindex == 4)
					_XDWINPRINT("[비궁[%d]] %f %f %f", index, position.x, position.y, position.z);
*/				
				if(g_CurrentZoneInfoIndex == groupindex)
				{
					switch(g_CurrentZoneInfoIndex)
					{
					case 1 :
						grouptype = _XGROUP_SORIM;
						break;
					case 2 :
						grouptype = _XGROUP_GAEBANG;
						break;
					case 3 :
						grouptype = _XGROUP_NOCKRIM;
						break;
					case 4 :
						grouptype = _XGROUP_BEEGOONG;
						break;
					case 5 :
						grouptype = _XGROUP_MOODANG;
						break;
					case 6 :
						grouptype = _XGROUP_MAKYO;
						break;
					}
					
					_XSeat* pSeat = new _XSeat;
					pSeat->Create(index-1, grouptype, position, g_SEATMODEL[grouptype-1]);
					g_SeatList.Add(pSeat);
				}
			}
			if(index == count)
				bReadCount = FALSE;
		}
	} while(!feof(seatinfofile));
	
	g_ScriptArchive.ClosePackage();
}

BOOL XProc_MainGame::CheckBossMap(int targetzoneindex)
{
	return FALSE;
}
void XProc_MainGame::SetEnterBossBattleRoom(void)// 생사결에 들어갈 때 세팅하는 함수
{
	m_MoveServer = FALSE; // 이플래그를 꺼서 패킷처리가 가능케 한다.
	m_MovingGameServer = FALSE;	// 이동 완료...
	m_LockMoveProcess = FALSE;	
	WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
	g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_ENTER_REQ_C_S );
	g_pLocalUser->ResetSkillAnimation();
	
	if(g_ViewMyBattleGauge)
	{
		if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
		{		
			g_pLocalUser->RequestBattleMode( _XBATTLE_PEACE );	
		}
	}
	
	if(g_pLocalUser->m_InParty)
	{
		int partymembercount = 0 ;
		for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
		{
			DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + partymembercount;
			if( g_pLocalUser->m_PartyInfo[i].isLeader )
			{
				_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
				if(pMiniBar_Window)
				{
					BOOL isCurrent = FALSE;
					pMiniBar_Window->SetPartyMasterVital(i, isCurrent);
				}
				
			}
			else
			{
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
				if(pMiniBar_Window)
				{
					BOOL isCurrent = FALSE;
					pMiniBar_Window->SetPartyPlayerVital(i, isCurrent);
				}
				partymembercount++;
			}
		}				
	}//if(g_pLocalUser->m_InParty)	
}

void XProc_MainGame::SetExitBossBattleRoom(void)// 생사결에서 나올때 세팅하는 함수
{
	// 퀘스트 성공시에 먼저 보상을 하고 생사결 퀘스트를 취소해야 한다.
	g_QuestScriptManager.Process();

	m_MoveServer = FALSE; // 이플래그를 꺼서 패킷처리가 가능케 한다.
	m_MovingGameServer = FALSE;	// 이동 완료...
	m_LockMoveProcess = FALSE;	
	g_LodTerrain.m_bInvisiblePortalMouseView = FALSE;
	g_ClickedWarpZoneIndex = -1;
	
	WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
	g_pLocalUser->m_bBossBattleEndProcess = FALSE;
	g_pLocalUser->ResetSkillAnimation();
	
	g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
	g_ViewPoisonFog = FALSE;
	
	if( g_pJinStatus_Window)
	{
		g_pJinStatus_Window->SetBossroomPoisonTimer(FALSE);
	}

	_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
	if(pPoison_Window)
	{
		pPoison_Window->SetMode(_MODE_NONE);
		pPoison_Window->ShowWindow(FALSE);
	}
	
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->ShowWindow(FALSE);
	}
	
	// 생사결에서 원래 필드로 나올때는 염왕령등 아이템 삭제를 위해서 비정상적인 퀘스트가 있는지 검사를 한다. 			
	_XQuestScriptItem* currentquest = NULL;
	if( g_QuestScriptManager.m_CurrentMainQuestID != 65535 )
	{
		currentquest = g_QuestScriptManager.m_mapQuestScriptList[g_QuestScriptManager.m_CurrentMainQuestID];					
		if( currentquest )
		{
			int nodeid = currentquest->GetCurrentTreeNumber();
			BOOL needitemremove = FALSE;
			int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( g_QuestScriptManager.m_CurrentMainQuestID, nodeid, needitemremove );
			if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
			{
				g_QuestScriptManager.m_bQuestValidCheck = TRUE;
				if( practicablequestnode == 0 )
				{
					// 제룡행 퀘스트 리셋 패킷을 보낸다
					g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, g_QuestScriptManager.m_CurrentMainQuestID, nodeid);
				}
				else
				{
					g_NetworkKernel.SendPacket( MSG_NO_QUEST_VALIDCHECK_INGAME, g_QuestScriptManager.m_CurrentMainQuestID, practicablequestnode, needitemremove );
				}
			}
			else
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(nodeid);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{
						map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
						if(currentsuccesstrigger)
						{					
							for( int i = 0; i < 20; i++ )
							{
								currentsuccesstrigger->m_SuccessConditionid[i] = -1;
							}
						}
					}
				}
				// 퀘스트 정상진행이면 거슬러 올라가서 히스토리 체크를 한다.
				g_QuestScriptManager.QuestHistorySetting( g_QuestScriptManager.m_CurrentMainQuestID, nodeid );
			}
		}
	}

	for( int i = 0; i < ID_QUEST_SYNC-1; i++ ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
	{
		int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
		if( questid != -1)
		{
			currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[questid];					
			if( currentquest )
			{
				int nodeid = currentquest->GetCurrentTreeNumber();
				if( nodeid != 0)
				{
					_XLog("Receive SubQuest Data : SubQuestID [%d] SubQuestNodeID[%d] ", questid, nodeid );
					
					BOOL needitemremove = FALSE;
					int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( questid, nodeid, needitemremove );
					if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
					{
						g_QuestScriptManager.m_bQuestValidCheck = TRUE;
						if( practicablequestnode == 0 )
						{
							// 제룡행이 아닌 퀘스트 리셋 패킷을 보낸다					
							g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, questid, nodeid);
						}
						else
						{
							g_NetworkKernel.SendPacket( MSG_NO_QUEST_VALIDCHECK_INGAME, questid, practicablequestnode, needitemremove );
						}
					}		
					else
					{
						map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(nodeid);
						if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
						{
							_XTriggerTreeItem* currenttree = iter_triggertree->second;
							if(currenttree)
							{
								map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
								_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
								if(currentsuccesstrigger)
								{					
									for( int i = 0; i < 20; i++ )
									{
										currentsuccesstrigger->m_SuccessConditionid[i] = -1;
									}
								}
							}
						}
						// 퀘스트 정상진행이면 거슬러 올라가서 히스토리 체크를 한다.
						g_QuestScriptManager.QuestHistorySetting( questid, nodeid );
					}
				}
			}
		}	
	}

	// 저장되지 않는 퀘스트를 진행한다.
	if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
	{		
		list <int>::iterator iter_questnosavetype;		
		iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin();
		while( iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() )
		{			
			int questid = *iter_questnosavetype;			
			if( questid != -1 )
			{
				_XQuestScriptItem* currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[questid];
				if( currentquest )
				{
					int nodeid = currentquest->GetCurrentTreeNumber();
					if( nodeid != 0)
					{
						_XLog("Receive SubQuest Data : SubQuestID [%d] SubQuestNodeID[%d] ", questid, nodeid );
						
						BOOL needitemremove = FALSE;
						int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( questid, nodeid, needitemremove );
						if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
						{
							g_QuestScriptManager.m_bQuestValidCheck = TRUE;
							if( practicablequestnode == 0 )
							{
								// 제룡행이 아닌 퀘스트 리셋 패킷을 보낸다					
								g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, questid, nodeid);
							}
							else
							{
								g_NetworkKernel.SendPacket( MSG_NO_QUEST_VALIDCHECK_INGAME, questid, practicablequestnode, needitemremove );
							}
						}		
						else
						{
							map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(nodeid);
							if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
							{
								_XTriggerTreeItem* currenttree = iter_triggertree->second;
								if(currenttree)
								{
									map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
									_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
									if(currentsuccesstrigger)
									{					
										for( int i = 0; i < 20; i++ )
										{
											currentsuccesstrigger->m_SuccessConditionid[i] = -1;
										}
									}
								}
							}
							// 퀘스트 정상진행이면 거슬러 올라가서 히스토리 체크를 한다.
							g_QuestScriptManager.QuestHistorySetting( questid, nodeid );
						}
						iter_questnosavetype++;	
					}
					else
					{
						iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.erase(iter_questnosavetype);
					}
				}
				else
				{
					iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.erase(iter_questnosavetype);
				}
			}
			else
			{
				iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.erase(iter_questnosavetype);
			}
		}
	}
	// 끝 - 생사결에서 원래 필드로 나올때는 염왕령등 아이템 삭제를 위해서 비정상적인 퀘스트가 있는지 검사를 한다.
	
	
	if(g_pLocalUser->m_InParty)
	{
		int partymembercount = 0 ;
		for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
		{
			DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + partymembercount;
			if( g_pLocalUser->m_PartyInfo[i].isLeader )
			{
				_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
				if(pMiniBar_Window)
				{
					BOOL isCurrent = FALSE;
					pMiniBar_Window->SetPartyMasterVital(i, isCurrent);
				}

			}
			else
			{
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
				if(pMiniBar_Window)
				{
					BOOL isCurrent = FALSE;
					pMiniBar_Window->SetPartyPlayerVital(i, isCurrent);
				}
				partymembercount++;
			}
		}				
	}//if(g_pLocalUser->m_InParty)	
	
	
	if( g_pLocalUser->m_bSuccessBossBattle )
	{
		g_pLocalUser->m_bSuccessBossBattle = FALSE;
		switch(g_ChannelTempZoneInfoIndex) {
		case 50://정주보스맵
		case 51://석가장보스맵
			{
				g_indexScene = 29;
				if( g_SceneList[g_indexScene ] )
				{							
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_NONE;
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
				}
			}
			break;
		case 52://대동보스맵
		case 53://제남보스맵
			{
				g_indexScene = 30;
				if( g_SceneList[g_indexScene ] )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_NONE;
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
				}
			}
			break;
		case 54://
			{
			}
			break;
		case 55://맹수각
			{
				g_indexScene = 43;
				if( g_SceneList[g_indexScene ] )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_NONE;
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
				}
			}
			break;
		case 56://
			{
			}
			break;
		case 57://천은전
			{
				g_indexScene = 42;
				if( g_SceneList[g_indexScene ] )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_NONE;
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
				}
			}
			break;			
		}
	}
}

void XProc_MainGame::SetEnterVirtualSpace(void)// 가상공간에 들어갈 때 세팅하는 함수
{
	m_MoveServer = FALSE; // 이플래그를 꺼서 패킷처리가 가능케 한다.
	m_MovingGameServer = FALSE;	// 이동 완료...
	m_LockMoveProcess = FALSE;	
	WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
	g_pLocalUser->ResetSkillAnimation();
	
	// 퀘스트들에 대한 검사를 다시 할 필요가 있다. 
	// 기존 맵의 퀘스트 정보들이 그대로 남아있다.
	g_QuestScriptManager.SetQuestStartConditionAtVirtualSpace();//리스트들을 재정렬한다.
	g_QuestScriptManager.QuestStartConditionZoneMode();
	g_QuestScriptManager.QuestStartConditionLevelMode();
	g_QuestScriptManager.Process();
	
}

void XProc_MainGame::SetLeaveVirtualSpace(void)// 생사결에서 나올때 세팅하는 함수
{
	g_QuestScriptManager.SetQuestStartConditionAtVirtualSpace();//리스트들을 재정렬한다.
	g_QuestScriptManager.QuestStartConditionZoneMode();
	g_QuestScriptManager.QuestStartConditionLevelMode();
	g_QuestScriptManager.Process();
	
	g_pLocalUser->m_bVirtualMode = FALSE;
	m_MoveServer = FALSE; // 이플래그를 꺼서 패킷처리가 가능케 한다.
	m_MovingGameServer = FALSE;	// 이동 완료...
	m_LockMoveProcess = FALSE;	
	g_LodTerrain.m_bInvisiblePortalMouseView = FALSE;
	g_ClickedWarpZoneIndex = -1;

	WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
	g_pLocalUser->m_bBossBattleEndProcess = FALSE;
	g_pLocalUser->ResetSkillAnimation();
	
	g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
	g_ViewPoisonFog = FALSE;
	
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->ShowWindow(FALSE);
	}
	
	// 가상공간에서 원래 필드로 나올때는 비정상적인 퀘스트가 있는지 검사를 한다. 			
	_XQuestScriptItem* currentquest = NULL;
	if( g_QuestScriptManager.m_CurrentMainQuestID != 65535 )
	{
		currentquest = g_QuestScriptManager.m_mapQuestScriptList[g_QuestScriptManager.m_CurrentMainQuestID];					
		if( currentquest )
		{
			int nodeid = currentquest->GetCurrentTreeNumber();
			BOOL needitemremove = FALSE;
			int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( g_QuestScriptManager.m_CurrentMainQuestID, nodeid, needitemremove );
			if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
			{
				g_QuestScriptManager.m_bQuestValidCheck = TRUE;
				if( practicablequestnode == 0 )
				{
					// 제룡행 퀘스트 리셋 패킷을 보낸다
					g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, g_QuestScriptManager.m_CurrentMainQuestID, nodeid);
				}
				else
				{
					g_NetworkKernel.SendPacket( MSG_NO_QUEST_VALIDCHECK_INGAME, g_QuestScriptManager.m_CurrentMainQuestID, practicablequestnode, needitemremove );
				}
			}
			else
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(nodeid);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{
						map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
						if(currentsuccesstrigger)
						{					
							for( int i = 0; i < 20; i++ )
							{
								currentsuccesstrigger->m_SuccessConditionid[i] = -1;
							}
						}
					}
				}
				// 퀘스트 정상진행이면 거슬러 올라가서 히스토리 체크를 한다.
				g_QuestScriptManager.QuestHistorySetting( g_QuestScriptManager.m_CurrentMainQuestID, nodeid );
			}
		}
	}

	for( int i = 0; i < ID_QUEST_SYNC-1; i++ ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
	{
		int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
		if( questid != -1)
		{
			currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[questid];					
			if( currentquest )
			{
				int nodeid = currentquest->GetCurrentTreeNumber();
				if( nodeid != 0)
				{
					_XLog("Receive SubQuest Data : SubQuestID [%d] SubQuestNodeID[%d] ", questid, nodeid );
					
					BOOL needitemremove = FALSE;
					int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( questid, nodeid, needitemremove );
					if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
					{
						g_QuestScriptManager.m_bQuestValidCheck = TRUE;
						if( practicablequestnode == 0 )
						{
							// 제룡행이 아닌 퀘스트 리셋 패킷을 보낸다					
							g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, questid, nodeid);
						}
						else
						{
							g_NetworkKernel.SendPacket( MSG_NO_QUEST_VALIDCHECK_INGAME, questid, practicablequestnode, needitemremove );
						}
					}		
					else
					{
						map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(nodeid);
						if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
						{
							_XTriggerTreeItem* currenttree = iter_triggertree->second;
							if(currenttree)
							{
								map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
								_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
								if(currentsuccesstrigger)
								{					
									for( int i = 0; i < 20; i++ )
									{
										currentsuccesstrigger->m_SuccessConditionid[i] = -1;
									}
								}
							}
						}
						// 퀘스트 정상진행이면 거슬러 올라가서 히스토리 체크를 한다.
						g_QuestScriptManager.QuestHistorySetting( questid, nodeid );
					}
				}
			}
		}	
	}

	// 저장되지 않는 퀘스트를 진행한다.
	if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
	{		
		list <int>::iterator iter_questnosavetype;		
		iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin();
		while( iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() )
		{			
			int questid = *iter_questnosavetype;			
			if( questid != -1 )
			{
				_XQuestScriptItem* currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[questid];
				if( currentquest )
				{
					int nodeid = currentquest->GetCurrentTreeNumber();
					if( nodeid != 0)
					{
						_XLog("Receive SubQuest Data : SubQuestID [%d] SubQuestNodeID[%d] ", questid, nodeid );
						
						BOOL needitemremove = FALSE;
						int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( questid, nodeid, needitemremove );
						if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
						{
							g_QuestScriptManager.m_bQuestValidCheck = TRUE;
							if( practicablequestnode == 0 )
							{
								// 제룡행이 아닌 퀘스트 리셋 패킷을 보낸다					
								g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, questid, nodeid);
							}
							else
							{
								g_NetworkKernel.SendPacket( MSG_NO_QUEST_VALIDCHECK_INGAME, questid, practicablequestnode, needitemremove );
							}
						}		
						else
						{
							map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(nodeid);
							if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
							{
								_XTriggerTreeItem* currenttree = iter_triggertree->second;
								if(currenttree)
								{
									map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
									_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
									if(currentsuccesstrigger)
									{					
										for( int i = 0; i < 20; i++ )
										{
											currentsuccesstrigger->m_SuccessConditionid[i] = -1;
										}
									}
								}
							}
							// 퀘스트 정상진행이면 거슬러 올라가서 히스토리 체크를 한다.
							g_QuestScriptManager.QuestHistorySetting( questid, nodeid );
						}
						iter_questnosavetype++;	
					}
					else
					{
						iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.erase(iter_questnosavetype);
					}
				}
				else
				{
					iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.erase(iter_questnosavetype);
				}
			}
			else
			{
				iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.erase(iter_questnosavetype);
			}
		}
	}
	// 끝 - 생사결에서 원래 필드로 나올때는 염왕령등 아이템 삭제를 위해서 비정상적인 퀘스트가 있는지 검사를 한다.
}

void XProc_MainGame::InitializeInterface(void)
{
	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
	{
		if(pRebirth_Window->GetShowStatus())
			pRebirth_Window->ShowWindow(FALSE);
	}

	// Set Option properties...
	_XWindow_Option* pOption_Window = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
	if( pOption_Window )
	{
		pOption_Window->ApplyOptionProperties();
	}
	
	_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
	if(pMonsterStatus_Window)
	{
		pMonsterStatus_Window->ShowWindow( FALSE );
	}
	
	_XWindow_NPCSkill* pNPCSkill_Window = (_XWindow_NPCSkill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSKILL);
	if(pNPCSkill_Window)
	{
		if(pNPCSkill_Window->GetShowStatus())
			pNPCSkill_Window->ShowWindow(FALSE);
	}

	_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

#ifdef _XDEF_SHUTDOWN_20061027
	_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
	if( pPaymentMethod_Window ) if(!pPaymentMethod_Window->GetShowStatus()) pPaymentMethod_Window->ShowWindow(TRUE);	
#endif

	if(!pMainMenu_Window->GetShowStatus()) pMainMenu_Window->ShowWindow(TRUE);
	if(!g_pJinStatus_Window->GetShowStatus()) g_pJinStatus_Window->ShowWindow(TRUE);
	if(!pChatDefault_Window->GetShowStatus()) pChatDefault_Window->ShowWindow(TRUE);
	
	if(g_pQuickSlot_Window)
	{
		if(!g_pQuickSlot_Window->GetShowStatus())
			g_pQuickSlot_Window->ShowWindow(TRUE);
	}
	if(g_pState_Window)
	{
		if(!g_pState_Window->GetShowStatus())
			g_pState_Window->ShowWindow(TRUE);
	}
	
	if(g_ViewMyBattleGauge)
	{
		if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
		{		
			g_pLocalUser->m_MiniHpBorder.SetGaugeFactor(g_pLocalUser->LifePowerPercentage);
		}
	}
	_XWindow_GMCommand* pGMCommandWindow = (_XWindow_GMCommand*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMWINDOW);	
	if( pGMCommandWindow )
	{
#ifdef _XGMCLIENT
		pGMCommandWindow->ShowWindow( FALSE );

		if( g_pLocalUser->m_CharacterInfo.Get_gm_level() > 0 )
		{
			pGMCommandWindow->ShowWindow( FALSE );
			pGMCommandWindow->ShowWindow( TRUE );
			pGMCommandWindow->MoveWindow( 980, 211 );
			pGMCommandWindow->SetWindowAnimationMode( _XGMWINDOW_ANIMATIONMODE_CLOSE );
		}
#else
		{
			pGMCommandWindow->ShowWindow( FALSE );
		}
#endif
	}
	
	_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
	if( optionwindow )
	{	
		_XCheckButton* pButton = ((_XWindow_OptionSound*)optionwindow->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pUseBGMButton;
		
		if( pButton )
		{			
			pButton->SetCheck( g_BgmPlayFlag );
		}
	}

	_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
	if(pBloodPoint_Window)
	{
		//Author : 양희왕 //breif : 4월30일 혈정 포인트 관련
        if(g_pLocalUser->m_BloodPoint >= 999)
            pBloodPoint_Window->SetShowReqItem(TRUE);
        else
            pBloodPoint_Window->SetShowReqItem(FALSE);
		
		if(!pBloodPoint_Window->GetShowStatus())
			pBloodPoint_Window->ShowWindow(TRUE);
	}
	
	_XWindow_AccountItemUse* pAccountItemUse_Window = (_XWindow_AccountItemUse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ACCOUNTITEMUSE);
	if(pAccountItemUse_Window)
	{	
		if(g_pLocalUser->m_AccountItemsID > 0)
			pAccountItemUse_Window->ShowWindow(TRUE);
		else
			pAccountItemUse_Window->ShowWindow(FALSE);
	}

	_XWindow_GuideLine* pGuideLine_Window = (_XWindow_GuideLine*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GUIDELINEWINDOW);
	if( pGuideLine_Window )
	{
		if( pGuideLine_Window->GetShowStatus() )
		{
			pGuideLine_Window->ShowWindow(FALSE);	// 강제 도움말 창 닫기
		}
	}

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	if(g_pExp_Window) //Author : 양희왕 //breif : 추가했음
	{
		if(!g_pExp_Window->GetShowStatus())
			g_pExp_Window->ShowWindow(TRUE);
	}
#endif

#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 아이콘
	_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
	_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
	if( pEventIcon && pEvent_Window)
	{
		pEvent_Window->SetListItem(); //셋리스트 아이템을 해준다,, 버프이벤트 있는지 여부를 체크함 최초 체크하면 들어가지않는다
		if( pEvent_Window->GetCurrentEventStatus() ||
            pEvent_Window->m_nEventIDinfo || //레벨업 이벤트를 받지 않았다.
            (!pEventIcon->GetShowMessageBox() && g_pLocalUser->m_EventSkillList.m_sSkillID) || //경공 선물 창을 클릭하지 않았다.
			pEvent_Window->m_bBuffEvent )
			pEventIcon->ShowWindow(TRUE);
	}
#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
	if(pMnDCurrentPoint_Window)
	{
		pMnDCurrentPoint_Window->ShowWindow(TRUE);
	}
	
	_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
	if(pMnDTotalPoint_Window)
	{
		pMnDTotalPoint_Window->ShowWindow(TRUE);
	}
#endif
}

void XProc_MainGame::CheckLandMarkPoint( void )
{
	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;

	_XWindow_ZoneName* pZoneName_Window = (_XWindow_ZoneName *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ZONENAMEWINDOW);
	if(!pZoneName_Window) return;
	
	for(iter = g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_EVENTAREA].AreaList.begin() ; iter != g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_EVENTAREA].AreaList.end() ; iter++)
	{
		currentarea = *iter;

		if( g_AreaManager.CheckInArea( g_pLocalUser->m_Position , currentarea ) )
		{
			if( !pZoneName_Window->CompareLastLandMark( currentarea->m_Name ) )
			{
				if( currentarea->m_Name[0] != _T('#') )
				{
					if( strlen( pZoneName_Window->GetZoneName() ) == 0 )
					{
						if( pZoneName_Window->GetWorldNameDisplayMode() )
						{
							pZoneName_Window->SetWorldNameDisplayMode( FALSE );
#ifdef _XJAPANESE	
							pZoneName_Window->SetZoneName( NULL, NULL );
#else
							pZoneName_Window->SetZoneName( NULL );
#endif

							pZoneName_Window->StartAnimation( _XZoneWindowEffectClass_Stop );
						}
						else
						{
#ifdef _XJAPANESE	
							pZoneName_Window->SetZoneName( currentarea->m_Name, currentarea->m_HiraganaName );
#else					
							pZoneName_Window->SetZoneName( currentarea->m_Name );
#endif
						}
					}
					else
					{
						if( pZoneName_Window->GetWorldNameDisplayMode() )
						{
							pZoneName_Window->SetWorldNameDisplayMode( FALSE );
#ifdef _XJAPANESE	
							pZoneName_Window->SetZoneName( currentarea->m_Name, currentarea->m_HiraganaName );
#else					
							pZoneName_Window->SetZoneName( currentarea->m_Name );
#endif
						}
						else
						{
#ifdef _XJAPANESE	
							pZoneName_Window->SetZoneName( currentarea->m_Name, currentarea->m_HiraganaName );
#else					
							pZoneName_Window->SetZoneName( currentarea->m_Name );
#endif

							pZoneName_Window->StartAnimation( _XZoneWindowEffectClass_Open );
						}
					}
				}
			}			
		}
	}
}

BOOL XProc_MainGame::CheckTargetType(short skillid)
{
	if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_HIDDENMA)
		return TRUE;
	
	if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHARGESKILL)
	{
		// charge 도중 attack target을 다시 잡는것을 방지
		if(g_pLocalUser->m_AttackTarget)
		{
			if(g_pLocalUser->m_ViewTarget == g_pLocalUser->m_AttackTarget)
			{
				return TRUE;
			}
		}
	}

	switch(_XSkillItem::GetTargetType(skillid)/*g_SkillProperty[skillid]->targetType*/)
	{
	case _XSI_TARGETTYPE_SELF :
		{
			return TRUE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_ENEMY :
		{
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_MOB)
				{
					g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
					return TRUE;
				}
				else if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pObj = (_XFunctionalObject*)g_pLocalUser->m_ViewTarget;
					if(pObj)
					{
						if(pObj->m_bAttackable)
						{
							g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
							return TRUE;
						}
					}
				}
				else if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;

					if((g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && g_pLocalUser->m_MatchTargetID == pUser->m_Uniqueid) ||
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) || 
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_FIGHT && g_pLocalUser->CheckPKTarget(pUser)))
					{
						if(pUser->GetMotionClass() == _XACTION_DYING || pUser->GetMotionClass() == _XACTION_DIE)
							return FALSE;

                        if(g_pLocalUser->CheckPKPenalty(pUser)) // 최근 승전 기록이 있는 상대 ( 레벨차 패널티 )
                            return FALSE;


						g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
						return TRUE;
					}
					else
					{
						if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT)
						{
							_XDWINPRINT(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1838), g_pLocalUser->m_MatchTargetID, pUser->m_Uniqueid);
						}
						
						return FALSE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_FRIEND :
		{
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;

					if((g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && g_pLocalUser->m_MatchTargetID == pUser->m_Uniqueid) ||
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_FIGHT && g_pLocalUser->CheckPKTarget(pUser)))
					{
						return FALSE;
					}
					else
					{
						g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
						return TRUE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_SELFOTHER_FRIEND :
		{
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_SELF)
				{
					g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
					return TRUE;
				}
				else if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;

					if((g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && g_pLocalUser->m_MatchTargetID == pUser->m_Uniqueid) ||
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_FIGHT && g_pLocalUser->CheckPKTarget(pUser)))
					{
						return FALSE;
					}
					else
					{
						g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
						return TRUE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_SELFOTHER_ENEMY :
		{
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_SELF || g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_MOB)
				{
					g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
					return TRUE;
				}
				else if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pObj = (_XFunctionalObject*)g_pLocalUser->m_ViewTarget;
					if(pObj)
					{
						if(pObj->m_bAttackable)
						{
							g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
							return TRUE;
						}
					}
				}
				else if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;

					if((g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && g_pLocalUser->m_MatchTargetID == pUser->m_Uniqueid) ||
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
						(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_FIGHT && g_pLocalUser->CheckPKTarget(pUser)))
					{
						if(pUser->GetMotionClass() == _XACTION_DYING || pUser->GetMotionClass() == _XACTION_DIE)
							return FALSE;

						g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_ALL :
		{
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC || g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_MOB 
					|| g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
					return TRUE;
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_ALL :
		{
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_SELF || g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC ||
					g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_MOB || g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
					return TRUE;
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_POINT :
		{
			return TRUE;
		}
		break;
#ifdef _XTS_SKILLCOMATYPE
	case _XSI_TARGETTYPE_COMA :
		{
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
					if(pUser)
					{
						if(pUser->GetMotionClass() == _XACTION_DIE)
						{
							//Author : 양희왕 //breif : 영웅대회중엔 부활을 막는다 //Date : 2월 20일
							if( g_pLocalUser->GetEnterMatchEvent() )
								return FALSE;

							g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
							return TRUE;
						}
						else
							return FALSE;
					}
				}
			}
		}
		break;
#endif
	}

	return FALSE;
}

BOOL XProc_MainGame::CheckReqCondition(short skillid)
{
	
	// 탈출 중이면 무공 사용 불가능하다.
#ifdef _XDEF_ESCAPE_20070205
	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
	if(pEscape_Window)
	{					
		if( pEscape_Window->GetShowStatus() )
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3314), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);// 탈출 중이면 무공 사용 불가능하다.
			return FALSE;
		}
	}
#endif

	if(skillid == 0)
	{
		if(g_pLocalUser->m_Lock.battlelock == 1)
		{
			// 맨손 전투 불가능
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2736), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //상태 이상으로 인하여 전투가 불가능 합니다.
			return FALSE;
		}
		else
			return TRUE;
	}

	// 상태효과에서 스킬을 사용할 수 없음
	if(g_pLocalUser->m_Lock.skilllock == 1)
	{
		switch(g_SkillProperty[skillid]->usefulType)
		{
		case _XSI_USEFULTYPE_ACTIVESKILL :
		case _XSI_USEFULTYPE_CHARGESKILL :
		case _XSI_USEFULTYPE_FINISHSKILL :
		case _XSI_USEFULTYPE_SMASHINGSKILL :
		case _XSI_USEFULTYPE_ACTIONSKILL :
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_STATUSACTIVESKILL), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 상태 이상으로 인하여 모든 무공 사용이 불가능 합니다
				return FALSE;
			}
			break;
		}
	}

	// 상태효과에서 스펠을 사용할 수 없음
	if(g_pLocalUser->m_Lock.spelllock == 1)
	{
		switch(g_SkillProperty[skillid]->usefulType)
		{
		case _XSI_USEFULTYPE_CASTINGSPELL :
		case _XSI_USEFULTYPE_ROUNDINGSPELL :
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_STATUSPASSIVESKILL), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 상태 이상으로 인하여 모든 기공 사용이 불가능 합니다
				return FALSE;
			}
			break;
		case _XSI_USEFULTYPE_CHANNELINGSPELL :
			{
				if(g_SkillProperty[skillid]->skillSType != _XSI_STYPE_02 && g_SkillProperty[skillid]->skillSType != _XSI_STYPE_03)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_STATUSPASSIVESKILL), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 상태 이상으로 인하여 모든 기공 사용이 불가능 합니다
					return FALSE;
				}
			}
			break;
		}
	}

	// 상태효과에서 운기조식을 사용할 수 없음
	if(g_pLocalUser->m_Lock.medlock == 1)
	{
		if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_HIDDENMA)
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_STATUSMEDITATION), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 상태 이상으로 인하여 운기조식 사용이 불가능 합니다
			return FALSE;
		}
	}

	// 상태효과에서 경공을 사용할 수 없음
	if(g_pLocalUser->m_Lock.speedlock == 1)
	{
		if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL && g_SkillProperty[skillid]->skillSType == _XSI_STYPE_03)
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_STATUSSPEEDSKILL), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 상태 이상으로 인하여 경공 사용이 불가능 합니다
			return FALSE;
		}
	}
	
	// 특정 무공 사용 불가
	if(g_pLocalUser->m_Lock.disableskill == 1)
	{
		if(g_pLocalUser->m_Lock.disableskillindex == skillid)
		{
			TCHAR messagestring[256];
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2607), g_SkillProperty[skillid]->skillName);
			g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			return FALSE;
		}
	}

	// 특정 s type 무공 사용 불가
	if(g_pLocalUser->m_Lock.disablestype == 1)
	{
		char stype = g_SkillProperty[skillid]->skillSType;
		if(g_pLocalUser->m_Lock.disablestypeindex == stype)
		{
			TCHAR messagestring[256];
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_2608), g_SkillConceptProperty[g_pLocalUser->m_CharacterInfo.groupindex].stype_name[stype]);
			g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			return FALSE;
		}
	}

	// 경공 사용 불가능한 경우 체크
	if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL && g_SkillProperty[skillid]->skillSType == _XSI_STYPE_03)
	{
		// 장원전 중에는 장원 안에서 경공 사용 불가
#ifdef _XDEF_CASTLEBATTLE
		if(g_CBManager.GetLightFootState() == 1)
		{
			int areanumber = 0;
			if(g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_CASTLE, areanumber))
			{
				// 장원 안에 있음
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3248), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "장원 안에서는 경공을 사용할 수 없습니다."
				return FALSE;
			}
		}
#endif

		// 비무 중이고 경공 금지 일때 사용 불가
		if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT)
		{
			// 비무 중
			if(g_pLocalUser->m_myMatchTableInfo.cUseHyperrun == 1)
			{
				// 경공 금지
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3667), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"경공을 사용할 수 없습니다."
				return FALSE;
			}
		}
		
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_hyperrun))	// 비무 중 경공 금지
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3667), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"경공을 사용할 수 없습니다."
			return FALSE;
		}
#endif

		// 대전 상태에서 경공 사용 불가
#ifdef _XTS_PK
		if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
		{
			// 경공 제한
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2729 ), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전 중에는 경공을 사용할 수 없습니다."
			return FALSE;
		}
#endif

		//경공 금지 구역안에서는 경공 사용 불가
		int areanumber = 0;
		if(g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_NOTUSESPEEDSKILL, areanumber))
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3667), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"경공을 사용할 수 없습니다."
			return FALSE;
		}
	}

	if(g_SkillProperty[skillid]->requirementCondition > 0)
	{
		if(g_pLocalUser->m_StateList.FindConditionState(g_pLocalUser->m_Uniqueid, g_SkillProperty[skillid]->requirementCondition))
		{
			return TRUE;
		}
		else
		{	
			int conditionid = g_SkillProperty[skillid]->requirementCondition - 1;

			TCHAR messagestring[256];

			if(conditionid >= 0 && conditionid < 32)
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORNOTSTATUS), g_CharacterConditionTable[conditionid].name); // %s상태가 아니므로 사용할 수 없습니다
			else
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_MOLRA)); // 알수 없는 상태가 아니므로 사용할 수 없습니다
			
			g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			return FALSE;
		}
	}
	else
	{
		return TRUE;
	}
	return TRUE;
}

BOOL XProc_MainGame::CheckReqItem(short skillid)
{
	if(skillid == 0)
		return TRUE;

	char skilllevel = 0;
	int learnedindex = 0;
	if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_HIDDENMA)
	{
		learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[skillid]->insideType);
		skilllevel = g_pLocalUser->m_SkillList[learnedindex].m_cSkillLevel;
	}
	else
	{
		learnedindex = g_pLocalUser->GetLearnedSkillIndex(skillid);

		if(learnedindex == _XDEF_MAX_LEARNSKILL)
		{
			skilllevel = g_pLocalUser->m_EventSkillList.m_cSkillLevel;
		}
		else
		{
			skilllevel = g_pLocalUser->m_SkillList[learnedindex].m_cSkillLevel;
		}
	}

	short itemid = g_SkillProperty[skillid]->detailInfo[skilllevel].requirementItemID;

	if(itemid > 0)
	{
		switch(g_SkillProperty[skillid]->detailInfo[skilllevel].reqItemCategory)
		{
		case _XGI_FC_WEAPON :
			{
				if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() == itemid)
				{
					return TRUE;
				}
				else
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_WeaponItemProperty[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i < _XINVENTORY_SLOTTYPE_WEAPON_1 ; i++)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					{
						return TRUE;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_WEAPON_1)
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_ClothesItemProperty[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_WEAPON2 :
			{
				if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() == itemid)
				{
					return TRUE;
				}
				else
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_WeaponItemProperty2[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_WEAPON3 :
			{
				if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() == itemid)
				{
					return TRUE;
				}
				else
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_WeaponItemProperty3[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_CLOTHES3 :
			{
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i < _XINVENTORY_SLOTTYPE_WEAPON_1 ; i++)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					{
						return TRUE;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_WEAPON_1)
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_ClothesItemProperty3[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_CLOTHES2 :
			{
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i < _XINVENTORY_SLOTTYPE_WEAPON_1 ; i++)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					{
						return TRUE;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_WEAPON_1)
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_ClothesItemProperty2[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				for(int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1 ; i < _XINVENTORY_SLOTTYPE_BACKPACK2 ; i++)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					{
						return TRUE;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_BACKPACK2)
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_AccessoryItemProperty[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_LIFE :
			{
				if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() == itemid)
				{
					return TRUE;
				}
				else
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_LifeItemProperty[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		case _XGI_FC_SOCKET :
			{
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i < _XINVENTORY_SLOTTYPE_WEAPON_2 ; i++)
				{					
					for(int j = 0 ; j < 4 ; j++)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_sSocket(j) == itemid)
						{
							return TRUE;
						}
					}					
				}
				if(i == _XINVENTORY_SLOTTYPE_WEAPON_2)
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERROREQUIP), g_SocketItemProperty[itemid].cName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
			break;
		default :
			{
				// 나머지는 소유 체크
            #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
                for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	            {
		            if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_SkillProperty[skillid]->detailInfo[skilllevel].reqItemCategory
						&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					{
						return TRUE;
					}
	            }
				            
                for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	            {
		            if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_SkillProperty[skillid]->detailInfo[skilllevel].reqItemCategory
						&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					{
						return TRUE;
					}
	            }
            #else
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT ; i++)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_SkillProperty[skillid]->detailInfo[skilllevel].reqItemCategory
						&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					{
						return TRUE;
					}
				}
            #endif
				if(i == _XINVENTORY_SLOTTYPE_TOTALCOUNT)
				{
					TCHAR messagestring[256];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_NECESSITY), _XGameItem::GetItemName(g_SkillProperty[skillid]->detailInfo[skilllevel].reqItemCategory, itemid)); // %s(이)가 필요합니다
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return FALSE;
				}
			}
			break;
		}
	}

	return TRUE;
}

void XProc_MainGame::SaveTutorialInfo( void )
{
#ifdef _XDEF_HELPSYSTEMSERVERSAVE	
	return;
#endif
	
	if( g_TutorialInfo == NULL ) return;
	
	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) == 0 ) return;
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
	FILE* pFilePtr = fopen( _T("UserTutorial.XUT"), "rb" );
	if( !pFilePtr )
	{
		SAFE_DELETE( g_TutorialInfo );
		_XLog( "WARNING : Can't open tutorial informations file" );
		return;
	}

	int _infocount = 0;
	fread( &_infocount, sizeof(int), 1, pFilePtr );
	
	_XTUTORIALINFO* TutorialInfoList = new _XTUTORIALINFO[_infocount];	

	for( int i = 0; i < _infocount; i++ )
	{
		fread( &TutorialInfoList[i], sizeof(_XTUTORIALINFO), 1, pFilePtr );		
		
		if( strcmpi( TutorialInfoList[i].CharacterName, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
		{
			memcpy( &TutorialInfoList[i], g_TutorialInfo, sizeof( _XTUTORIALINFO ) );
		}
	}
	fclose( pFilePtr );

	pFilePtr = fopen( _T("UserTutorial.XUT"), "wb" );
	
	fwrite( &_infocount, sizeof(int), 1, pFilePtr );
	for( i = 0; i < _infocount; i++ )
	{
		fwrite( &TutorialInfoList[i], sizeof(_XTUTORIALINFO), 1, pFilePtr );
	}	

	SAFE_DELETE_ARRAY( TutorialInfoList );
	SAFE_DELETE( g_TutorialInfo );
	fclose( pFilePtr );
}

void XProc_MainGame::LoadTutorialInfo( void )
{
#ifdef _XDEF_HELPSYSTEMSERVERSAVE	
	return;
#endif
	
	SAFE_DELETE( g_TutorialInfo );
	g_TutorialInfo = new _XTUTORIALINFO;

	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	FILE* pFilePtr = fopen( _T("UserTutorial.XUT"), "rb" );
	
	if( !pFilePtr ) 
	{
		FILE* pFilePtr = fopen( _T("UserTutorial.XUT"), "wb" );

		int _infocount = 1;
		fwrite( &_infocount, sizeof(int), 1, pFilePtr );		
		g_TutorialInfo->Reset();
		strcpy( g_TutorialInfo->CharacterName, g_pLocalUser->m_CharacterInfo.charactername );
		fwrite( g_TutorialInfo, sizeof(_XTUTORIALINFO), 1, pFilePtr );		
		fclose( pFilePtr );
		return;
	}
	
	int _infocount = 0;

	fread( &_infocount, sizeof(int), 1, pFilePtr );
	
	if( _infocount <= 0 ) 
	{
		fclose( pFilePtr );
		return;
	}
	
	_XTUTORIALINFO* TutorialInfoList = new _XTUTORIALINFO[_infocount];

	for( int i = 0; i < _infocount; i++ )
	{
		fread( &TutorialInfoList[i], sizeof(_XTUTORIALINFO), 1, pFilePtr );
		
		if( strcmpi( TutorialInfoList[i].CharacterName, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
		{
			memcpy( g_TutorialInfo, &TutorialInfoList[i], sizeof( _XTUTORIALINFO ) );
			fclose( pFilePtr );
			return;			
		}
	}

	// 여까지 왔으면 등록된 유저가 아니므로 추가
	fclose( pFilePtr );

	pFilePtr = fopen( _T("UserTutorial.XUT"), "wb" );

	_infocount++;
	fwrite( &_infocount, sizeof(int), 1, pFilePtr );		
	
	for( i = 0; i < _infocount-1; i++ )
	{
		fwrite( &TutorialInfoList[i], sizeof(_XTUTORIALINFO), 1, pFilePtr );
	}

	// 추가
	_XTUTORIALINFO curUserInfo;
	curUserInfo.Reset();
	strcpy( curUserInfo.CharacterName, g_pLocalUser->m_CharacterInfo.charactername );
	fwrite( &curUserInfo, sizeof(_XTUTORIALINFO), 1, pFilePtr );
	memcpy( g_TutorialInfo, &curUserInfo, sizeof( _XTUTORIALINFO ) );
	
	SAFE_DELETE_ARRAY( TutorialInfoList );
	fclose( pFilePtr );
}

#define _EVENTNOTICELOGFILEHEADER	0xFCA1

void XProc_MainGame::LoadEventNoticeInfo( void )
{

#ifdef _XDEF_NOTICEPOPUPCONTROL__070910_POPUP
	if( !_XCheckValidPopupPage() )
	{
		return;
	}
#endif

	if( strlen( g_pLocalUser->m_UserName ) <= 0 ) return;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	FILE* pFilePtr = fopen( _T("EventNoticeList.XUT"), "rb" );
	int _infocount = 0;

	if( !pFilePtr ) 
	{
		// 파일을 생성
		pFilePtr = fopen( _T("EventNoticeList.XUT"), "wb" );

		DWORD header = _EVENTNOTICELOGFILEHEADER;
		fwrite( &header, sizeof(DWORD), 1, pFilePtr );

		_infocount++;
		fwrite( &_infocount, sizeof(int), 1, pFilePtr );
		
		// 추가
		_XEVENTNOTICE curUserInfo;
		curUserInfo.Reset();		
		strcpy( curUserInfo.UserName, g_pLocalUser->m_UserName );
		fwrite( &curUserInfo, sizeof(_XEVENTNOTICE), 1, pFilePtr );

		fclose( pFilePtr );

		_XWindow_EventNotice* pEventNotice_Window = (_XWindow_EventNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTNOTICEWINDOW);
		if( pEventNotice_Window )
		{
			pEventNotice_Window->SetEventNotice();
		}
		return;
	}
	
	BOOL  oldversion = FALSE;
	DWORD header = 0; //_EVENTNOTICELOGFILEHEADER
	fread( &header, sizeof(DWORD), 1, pFilePtr );
	if( header == _EVENTNOTICELOGFILEHEADER )
	{
		fread( &_infocount, sizeof(int), 1, pFilePtr );
	}
	else
	{
		_infocount = (int)header;
		oldversion = TRUE;
	}
	
	if( _infocount <= 0 ) 
	{
		fclose( pFilePtr );
		return;
	}
	
	_XEVENTNOTICE* EventNoticeInfoList = new _XEVENTNOTICE[_infocount];

	for( int i = 0; i < _infocount; i++ )
	{
		if( oldversion )
		{
			_XEVENTNOTICE_OLD	readdata;
			memset( &readdata, 0, sizeof(_XEVENTNOTICE_OLD) );
			fread( &readdata, sizeof(_XEVENTNOTICE_OLD), 1, pFilePtr );
			
			EventNoticeInfoList[i].Reset();
			memcpy( EventNoticeInfoList[i].LastConnectdate, readdata.LastConnectdate, sizeof(EventNoticeInfoList[i].LastConnectdate) );
			memcpy( EventNoticeInfoList[i].UserName, readdata.UserName, sizeof(readdata.UserName) );
		}
		else
		{
			fread( &EventNoticeInfoList[i], sizeof(_XEVENTNOTICE), 1, pFilePtr );
		}

		if( strcmpi( EventNoticeInfoList[i].UserName, g_pLocalUser->m_UserName ) == 0 )
		{
			SYSTEMTIME TimeInfo;
			GetLocalTime( &TimeInfo );
			TCHAR tempLastConnectDate[64];
			memset( tempLastConnectDate, 0, sizeof(tempLastConnectDate) );
			_snprintf( tempLastConnectDate, sizeof(tempLastConnectDate), "%d%d%d", TimeInfo.wYear, TimeInfo.wMonth, TimeInfo.wDay );			
			
			if( strcmpi(EventNoticeInfoList[i].LastConnectdate, tempLastConnectDate) != 0 )
			{
				_XWindow_EventNotice* pEventNotice_Window = (_XWindow_EventNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTNOTICEWINDOW);
				if( pEventNotice_Window )
				{
					pEventNotice_Window->SetEventNotice();
				}				
			}	
			
			fclose( pFilePtr );

			SAFE_DELETE_ARRAY( EventNoticeInfoList );

			return;
		}
	}

	// 여까지 왔으면 등록된 유저가 아니므로 추가
	fclose( pFilePtr );

	pFilePtr = fopen( _T("EventNoticeList.XUT"), "wb" );
	
	header = _EVENTNOTICELOGFILEHEADER;
	fwrite( &header, sizeof(DWORD), 1, pFilePtr );

	_infocount++;
	fwrite( &_infocount, sizeof(int), 1, pFilePtr );		
	
	for( i = 0; i < _infocount-1; i++ )
	{
		fwrite( &EventNoticeInfoList[i], sizeof(_XEVENTNOTICE), 1, pFilePtr );
	}

	// 추가
	_XEVENTNOTICE curUserInfo;
	curUserInfo.Reset();
	strcpy( curUserInfo.UserName, g_pLocalUser->m_UserName );
	fwrite( &curUserInfo, sizeof(_XEVENTNOTICE), 1, pFilePtr );
	
	_XWindow_EventNotice* pEventNotice_Window = (_XWindow_EventNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTNOTICEWINDOW);
	if( pEventNotice_Window )
	{
		pEventNotice_Window->SetEventNotice();
	}				
				
	SAFE_DELETE_ARRAY( EventNoticeInfoList );
	fclose( pFilePtr );
}

void XProc_MainGame::SaveEventNoticeInfo( void )
{
	if( strlen( g_pLocalUser->m_UserName ) == 0 ) return;
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	int _infocount = 0;
	FILE* pFilePtr = fopen( _T("EventNoticeList.XUT"), "rb" );
	if( !pFilePtr )
	{
		_XLog( "WARNING : Can't open eventnotice file" );
		fclose( pFilePtr );
		return;
	}	

	BOOL  oldversion = FALSE;
	DWORD header = 0; //_EVENTNOTICELOGFILEHEADER
	fread( &header, sizeof(DWORD), 1, pFilePtr );
	if( header == _EVENTNOTICELOGFILEHEADER )
	{
		fread( &_infocount, sizeof(int), 1, pFilePtr );
	}
	else
	{
		_infocount = (int)header;
		oldversion = TRUE;
	}

	_XEVENTNOTICE* EventNoticeInfoList = new _XEVENTNOTICE[_infocount];	

	// 저장된 유저 목록에서 자기아이디를 찾아서 찾아서 업데이트
	for( int i = 0; i < _infocount; i++ )
	{
		if( oldversion )
		{
			_XEVENTNOTICE_OLD	readdata;
			memset( &readdata, 0, sizeof(_XEVENTNOTICE_OLD) );
			fread( &readdata, sizeof(_XEVENTNOTICE_OLD), 1, pFilePtr );
			
			EventNoticeInfoList[i].Reset();
			memcpy( EventNoticeInfoList[i].LastConnectdate, readdata.LastConnectdate, sizeof(EventNoticeInfoList[i].LastConnectdate) );
			memcpy( EventNoticeInfoList[i].UserName, readdata.UserName, sizeof(readdata.UserName) );
		}
		else
		{
			fread( &EventNoticeInfoList[i], sizeof(_XEVENTNOTICE), 1, pFilePtr );
		}
		
		if( strcmpi( EventNoticeInfoList[i].UserName, g_pLocalUser->m_UserName ) == 0 )
		{
			SYSTEMTIME TimeInfo;
			GetLocalTime( &TimeInfo );
			
			TCHAR tempLastConnectDate[64];
			memset( tempLastConnectDate, 0, sizeof(tempLastConnectDate) );
			_snprintf( tempLastConnectDate, sizeof(tempLastConnectDate), "%d%d%d", TimeInfo.wYear, TimeInfo.wMonth, TimeInfo.wDay );
			strncpy( EventNoticeInfoList[i].LastConnectdate, tempLastConnectDate, sizeof(EventNoticeInfoList[i].LastConnectdate) );
			break;
		}
	}
	fclose( pFilePtr );

	// 
	pFilePtr = fopen( _T("EventNoticeList.XUT"), "wb" );
	
	header = _EVENTNOTICELOGFILEHEADER;
	fwrite( &header, sizeof(DWORD), 1, pFilePtr );

	fwrite( &_infocount, sizeof(int), 1, pFilePtr );		
	for( i = 0; i < _infocount; i++ )
	{
		fwrite( &EventNoticeInfoList[i], sizeof(_XEVENTNOTICE), 1, pFilePtr );
	}	

	SAFE_DELETE_ARRAY( EventNoticeInfoList );
	fclose( pFilePtr );
}

void XProc_MainGame::CloseNPCScriptWindow()
{
	// npc script 창 변수 초기화
	if(m_pNPCScriptWindowPtr)
		m_pNPCScriptWindowPtr->ShowWindow(FALSE);

	RepositionTopBottomVertex();

	if(g_LastNPCScriptOwner)
	{
		g_LastNPCScriptOwner->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
		g_LastNPCScriptOwner->m_SelectedMainScriptIndex = 0;
		g_LastNPCScriptOwner->m_PrevMainScriptIndex = -1;
		g_LastNPCScriptOwner->m_SelectedSubScriptIndex = -1;
		g_LastNPCScriptOwner->m_bCheckHostility = FALSE;
		g_LastNPCScriptOwner->SetHaveQuestID(-1);
		g_LastNPCScriptOwner->SetIdleAction();
		g_LastNPCScriptOwner->SetNPCDialogMode(FALSE);

		if(g_LastNPCScriptOwner->m_bStartQuestDialog)
		{
			g_LastNPCScriptOwner->m_bStartQuestDialog = FALSE;
			m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(FALSE);
		}

		g_LastNPCScriptOwner->m_pNPCScriptWindowPtr = NULL;
		g_LastNPCScriptOwner = NULL;
	}

	if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
		g_pLocalUser->ResetAttackTarget();
	if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_NPC)
		g_pLocalUser->ResetViewTarget();

	g_ScriptStarted = FALSE;
	m_bNPCDialogWideView = FALSE;
	g_NPCDialogScreenEffectFlag = FALSE;
	m_bNPCFadeInOutFlag = TRUE;

	// 창 닫기
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
    {
        //memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));
		pPKTracingRequest_Window->ShowWindow(FALSE);
    }

	if(m_pMainFrameWindowPtr)
		m_pMainFrameWindowPtr->ShowWindow(FALSE);

	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if(pTradeConfirm_Window)
		pTradeConfirm_Window->ShowWindow(FALSE);

	if(g_pInventory_Window)
		g_pInventory_Window->ShowWindow(FALSE);

	if(m_pNPCTradeWindowPtr)
		m_pNPCTradeWindowPtr->ShowWindow(FALSE);

	if(m_pWarehouseWindowPtr)
		m_pWarehouseWindowPtr->ShowWindow(FALSE);

	if(m_pNPCSkillWindowPtr)
		m_pNPCSkillWindowPtr->ShowWindow(FALSE);

	if(m_pRepairItemWindowPtr)
		m_pRepairItemWindowPtr->ShowWindow(FALSE);

	if(m_pContributionWindowPtr)
		m_pContributionWindowPtr->ShowWindow(FALSE);

	if(m_pSocketPlugInExWindowPtr)
		m_pSocketPlugInExWindowPtr->ShowWindow(FALSE);

	if(m_pNPCCollectionWindowPtr)
		m_pNPCCollectionWindowPtr->ShowWindow(FALSE);

	if(m_pNPCResourceWindowPtr)
		m_pNPCResourceWindowPtr->ShowWindow(FALSE);

	if(m_pNPCResource_New_windowptr)
		m_pNPCResource_New_windowptr->ShowWindow(FALSE);

	if(m_pInchantWindowPtr)
		m_pInchantWindowPtr->ShowWindow(FALSE);

	if(m_pHedekWindowPtr)
		m_pHedekWindowPtr->ShowWindow(FALSE);

	if(m_pAddSaveItemWindowPtr)
		m_pAddSaveItemWindowPtr->ShowWindow(FALSE);

	if(m_pPVPBrokerWindowPtr)
		m_pPVPBrokerWindowPtr->ShowWindow(FALSE);

	if(m_pNPCBuffWindowPtr)
		m_pNPCBuffWindowPtr->ShowWindow(FALSE);

	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if( pTradeMessageBox ) 
		g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
	
	_XMessageWindow* pEventMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTMESSAGEWINDOW);
	if( pEventMessageBox ) 
		g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_EVENTMESSAGEWINDOW );
	
	_XMessageWindow* pHealMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HEALMESSAGEWINDOW);
	if( pHealMessageBox ) 
		g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_HEALMESSAGEWINDOW );

	if(m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
	{
		m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}	
	if(g_pInventory_Window->m_bHaveMouseSlot)
	{
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}	
	if(m_pWarehouseWindowPtr->m_bHaveMouseSlot )
	{
		m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
		m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
	}

	// NPC 처리
	SetNPCMode( _XNPC_NONE );

	m_pNPCScriptWindowPtr->m_NPCDialogCount = 0;
	m_pNPCScriptWindowPtr->m_bFirstDialog = TRUE;
	m_pNPCScriptWindowPtr->m_bProgressNPCDialogMessage = FALSE;
	
	m_pNPCScriptWindowPtr->m_btnQuestOK->ShowWindow(FALSE);
	m_pNPCScriptWindowPtr->m_btnDialogNextPage->ShowWindow(FALSE);
	m_pNPCScriptWindowPtr->m_btnBackToNPCMode->ShowWindow(FALSE);
	m_pNPCScriptWindowPtr->m_listboxSelectionItem->ShowWindow(FALSE);
	
	if( m_pNPCScriptWindowPtr->m_pTitleString[0] != 0 ) 
		memset(m_pNPCScriptWindowPtr->m_pTitleString, 0, sizeof(TCHAR)*256);
	if( m_pNPCScriptWindowPtr->m_HaveQuestName[0] != 0 ) 
		memset(m_pNPCScriptWindowPtr->m_HaveQuestName, 0, sizeof(m_pNPCScriptWindowPtr->m_HaveQuestName));
	if( m_pNPCScriptWindowPtr->m_HaveQuestType[0] != 0 ) 
		memset(m_pNPCScriptWindowPtr->m_HaveQuestType, 0, sizeof(m_pNPCScriptWindowPtr->m_HaveQuestType));
	
	// 이벤트 관련 입력창을 초기화 시킨다.
	m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ShowWindow(FALSE);
	m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
	_XIMEKERNEL.SetFocus( NULL );	
	_XIMEKERNEL.ResetCombinationBuffer();			

	m_pWarehouseWindowPtr->m_bWarehouseFirstView = TRUE;
	
	_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	pminimapwindow->m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);

	// 퀘스트 관련 처리 - 필요할까?
}


extern void _XDrawEdgedSolidBar ( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );
extern void _XDrawSolidBar (FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color);
extern void	_XDrawLine2D( FLOAT px1, FLOAT py1, FLOAT px2, FLOAT py2, D3DCOLOR color1, D3DCOLOR color2 );
extern FLOAT g_TerrainRenderingTime;
#ifdef _XCHECKMEMORYUSAGE
extern long GetMemoryUsageInfo( void );
#endif

extern int g_RenderMobCount;

void XProc_MainGame::DrawGraphLog( void )
{	
	FLOAT fFMODCPUUsage = FSOUND_GetCPUUsage();
	int iFMODChannelUsage = FSOUND_GetChannelsPlaying();
#ifdef _XCHECKMEMORYUSAGE	
	long usedmemsize = GetMemoryUsageInfo();
#endif

	if( (g_LocalSystemTime - m_LastGraphUpdateTime) > 50 )
	{
		m_LastGraphUpdateTime = g_LocalSystemTime;

		if( m_GraphLogListInsertPoint >= 600 ) m_GraphLogListInsertPoint = 0;

		// fps		
		m_GraphLogList[0][m_GraphLogListInsertPoint] = gnFPS;
		if( m_GraphLogList[0][m_GraphLogListInsertPoint] > 100 ) m_GraphLogList[0][m_GraphLogListInsertPoint] = 100;

		// mob count
		m_GraphLogList[1][m_GraphLogListInsertPoint] = g_Moblist.m_smMobList.size();
		if( m_GraphLogList[1][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[1][m_GraphLogListInsertPoint] = 400;

		// tile count
		m_GraphLogList[2][m_GraphLogListInsertPoint] = g_LodTerrain.m_TerrainTileRenderingCacheLength[g_LodTerrain.m_UsedLayerCount];
		if( m_GraphLogList[2][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[2][m_GraphLogListInsertPoint] = 400;

		// terrain tile render time
		m_GraphLogList[3][m_GraphLogListInsertPoint] = g_TerrainRenderingTime / 100;
		if( m_GraphLogList[3][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[3][m_GraphLogListInsertPoint] = 400;

		// terrain tile render time
		m_GraphLogList[4][m_GraphLogListInsertPoint] = g_LodTerrain.m_FacesDrawn / 1000;
		if( m_GraphLogList[4][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[4][m_GraphLogListInsertPoint] = 400;

		// Sound sample
		m_GraphLogList[5][m_GraphLogListInsertPoint] = g_FMODWrapper.m_LoadedSoundSampleCount ;
		if( m_GraphLogList[5][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[5][m_GraphLogListInsertPoint] = 400;

		// Env sound sample
		m_GraphLogList[6][m_GraphLogListInsertPoint] = g_FMODWrapper.m_LoadedFLSoundSampleCount ;
		if( m_GraphLogList[6][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[6][m_GraphLogListInsertPoint] = 400;

		// FMOD CPU usage
		m_GraphLogList[7][m_GraphLogListInsertPoint] = (DWORD)(fFMODCPUUsage * 1200.0f);
		if( m_GraphLogList[7][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[7][m_GraphLogListInsertPoint] = 400;

		// FMOD Channel usage
		m_GraphLogList[8][m_GraphLogListInsertPoint] = iFMODChannelUsage;
		if( m_GraphLogList[8][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[8][m_GraphLogListInsertPoint] = 400;
#ifdef _XCHECKMEMORYUSAGE
		// Memory usage
		m_GraphLogList[9][m_GraphLogListInsertPoint] = usedmemsize / 1048576.0f;
		if( m_GraphLogList[9][m_GraphLogListInsertPoint] > 400 ) m_GraphLogList[9][m_GraphLogListInsertPoint] = 400;
#endif
		

		m_GraphLogListInsertPoint++;
	}

	const FLOAT xbasepoint = 23.0f;
	const FLOAT ybasepoint = 115.0f;
	
	_XDrawEdgedSolidBar(  xbasepoint-22, ybasepoint-19, xbasepoint+601, ybasepoint-1, 0x8F000000 );
	_XDrawEdgedSolidBar(  xbasepoint-22, ybasepoint-1, xbasepoint+601, ybasepoint+401, 0x3F000000 );
	_XDrawEdgedSolidBar(  xbasepoint-22, ybasepoint+401, xbasepoint+601, ybasepoint+419, 0x8F000000 );
	_XDrawLine2D( xbasepoint-1, ybasepoint, xbasepoint-1, ybasepoint+401, 0xFF000000, 0xFF000000 );

	g_XBaseFont->SetColor( 0x7FFFFF00 );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 2, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "FPS : %d", gnFPS );
	g_XBaseFont->SetColor( 0x7FFF0000 );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 16, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "MOB Cnt : %d/%d", g_RenderMobCount, g_Moblist.m_smMobList.size() );
	g_XBaseFont->SetColor( 0x7F00A651 );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "TILE Cnt : %d", g_LodTerrain.m_TerrainTileRenderingCacheLength[g_LodTerrain.m_UsedLayerCount] );
	g_XBaseFont->SetColor( 0x7F00FF51 );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 44, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "Terrain Render : %f", g_TerrainRenderingTime );
	g_XBaseFont->SetColor( 0x7FF7941D );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 60, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "Terrain Tri Cnt : %d", g_LodTerrain.m_FacesDrawn );
	g_XBaseFont->SetColor( 0x7FF194FF );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 74, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "Sound sample Cnt: %d", g_FMODWrapper.m_LoadedSoundSampleCount  );
	g_XBaseFont->SetColor( 0x7FF1942F );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 88, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "Env Sound sample Cnt: %d", g_FMODWrapper.m_LoadedFLSoundSampleCount );
	g_XBaseFont->SetColor( 0x7F113F2F );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 102, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "FMOD CPU Usage : %f%", fFMODCPUUsage );
	g_XBaseFont->SetColor( 0x9F383FFF );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 116, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "FMOD Channel Usage : %d", iFMODChannelUsage );
#ifdef _XCHECKMEMORYUSAGE	
	g_XBaseFont->SetColor( 0x9FFF3F3F );
	g_XBaseFont->PrintAlign( xbasepoint + 590, ybasepoint + 130, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "Memory usage : %fMBytes", usedmemsize / 1048576.0f );
#endif


	g_XBaseFont->SetColor( 0xFFFFFFFF );
	
	int lineheight = 400;
	for( int i = 0; i < 20; i++ )
	{
		lineheight -= 20;
		if( i & 0x01 )
		{
			_XDrawLine2D( xbasepoint+1, ybasepoint+lineheight, xbasepoint+600, ybasepoint+lineheight, 0x7F000000, 0x7F000000 );			
		}
		else
		{
			_XDrawLine2D( xbasepoint+1, ybasepoint+lineheight, xbasepoint+600, ybasepoint+lineheight, 0x3F000000, 0x3F000000 );
		}

		g_XBaseFont->PrintAlign( xbasepoint-4, ybasepoint+lineheight+1, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", (i+1) * 5 );
	}

	int   xposition = xbasepoint+1;
	DWORD fpssum = m_GraphLogList[0][0];

	for( i = 1; i < 600; i++ )
	{
		FLOAT prevheight = ybasepoint + (400-(m_GraphLogList[0][i-1]*4));
		FLOAT height = ybasepoint + (400-(m_GraphLogList[0][i]*4));		
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7FFFFF00, 0x7FFFFF00 );

		// mob count
		prevheight = ybasepoint + 400 - m_GraphLogList[1][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[1][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7FFF0000, 0x7FFF0000 );

		// tile count
		prevheight = ybasepoint + 400 - m_GraphLogList[2][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[2][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7F00A651, 0x7F00A651 );

		// terrain tile render time
		prevheight = ybasepoint + 400 - m_GraphLogList[3][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[3][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7F00FF51, 0x7F00FF51 );

		// terrain face drawn
		prevheight = ybasepoint + 400 - m_GraphLogList[4][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[4][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7FF7941D, 0x7FF7941D );

		// Sound sample count 
		prevheight = ybasepoint + 400 - m_GraphLogList[5][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[5][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7FF194FF, 0x7FF194FF );

		// Env sound sample count 
		prevheight = ybasepoint + 400 - m_GraphLogList[6][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[6][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7FF1942F, 0x7FF1942F );

		// FMOD CPU Usage
		prevheight = ybasepoint + 400 - m_GraphLogList[7][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[7][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x7F113F2F, 0x7F113F2F );

		// FMOD Channel Usage
		prevheight = ybasepoint + 400 - m_GraphLogList[8][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[8][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x9F383FFF, 0x9F383FFF );
#ifdef _XCHECKMEMORYUSAGE
		// Memory usage
		prevheight = ybasepoint + 400 - m_GraphLogList[9][i-1];
		height	   = ybasepoint + 400 - m_GraphLogList[9][i];
		_XDrawLine2D( xposition-1, prevheight, xposition, height, 0x9FFF3F3F, 0x9FFF3F3F );
#endif

		fpssum += m_GraphLogList[0][i];
		xposition++;
	}

	FLOAT faverage = (FLOAT)fpssum / 600.0f;
	_XDrawLine2D( xbasepoint+1, ybasepoint+400-(faverage*4.0f), xbasepoint+600, ybasepoint+400-(faverage*4.0f), 0x7FFF0000, 0x7FFF0000 );

	g_XBaseFont->Puts( xbasepoint - 16, ybasepoint - 16, "XKernel stress reporter" );
	g_XBaseFont->Print( xbasepoint - 16, ybasepoint + 405, 1.0f, "FPS : %d, FPS Average : %3.2f Terrain tile : %d, Face %d, Time : %f", 
					   gnFPS, faverage, 
					   g_LodTerrain.m_TerrainTileRenderingCacheLength[g_LodTerrain.m_UsedLayerCount], g_LodTerrain.m_FacesDrawn,
					   g_TerrainRenderingTime / 1000.0f );
	g_XBaseFont->Flush();
}


void XProc_MainGame::ProcessBilling( void )
{
/* 
author : 양희왕 breif : 주석처리
#ifdef _XTESTSERVER
	if( _XCmdLineOption("xinnertestserverlogin",0) != 0 )
	{
		return;
	}
#endif

	if( g_LocalSystemTime > g_BillingRemainTimeCounter )
	{
		int elapsedtime = g_LocalSystemTime - g_BillingRemainTimeCounter;

		g_LocalUserBillingInformations.BillRemain -= elapsedtime;
		if( g_LocalUserBillingInformations.BillRemain < 0 )
		{
			g_LocalUserBillingInformations.BillRemain = 0;

			if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_Home )
			{
				// 개인무료,프리미엄 유저
				if( g_LocalUserBillingInformations.BillingStatusWhere == _XB_BillingStatusWhere_Home )
				{
					if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Free )
					{
						// 무료이용/집				
						if( !m_GameExitFlag && !m_bGotoLoginProcess  )
						{
							if( !g_MainWindowManager.FindWindow(_XDEF_WTITLE_BILLINGTIMOUTMESSAGEWINDOW) )
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_BILLINGTIMOUTMESSAGEWINDOW );
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_1839), 
														TRUE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE );
								pMessageBox->StartTimer(30000, TRUE);
							}
						}
					}
					else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
					{
						// 개인정액/집				
					}
					else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
					{
						// 개인프리미엄/집				
					}
					else
					{
						// Error
					}
				}
				else if( g_LocalUserBillingInformations.BillingStatusWhere == _XB_BillingStatusWhere_PCRoom )
				{
					// 정액제 이상의 유저들.
					if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
					{
						if( g_LocalUserBillingInformations.BillingETCData == 0  )
						{
							// 정액유저/체험피씨방							
						}
						else
						{
							// 정액유저/프리미엄피씨방
						}
					}
					else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
					{
						if( g_LocalUserBillingInformations.BillingETCData == 0  )
						{
							// 프리미엄유저/체험피씨방
							
						}
						else
						{
							// 프리미엄유저/프리미엄피씨방
						}
					}
					else
					{
						// Error
					}					
				}
				else
				{
					// Error
				}
			}
			else if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_PC )
			{
				if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
				{
					// 무료이용/정액제피씨방
				}
				else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Fixed )
				{
					// 무료이용/정량제피씨방
				}
				else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
				{
					// 무료이용/프리미엄피씨방
				}
				else 
				{
					// 무료이용/체험피씨방					
				}
			}
			else //	시간을 모두 소비한 무료이용 유저.
			{
				if( !m_GameExitFlag && !m_bGotoLoginProcess )
				{
					if( !g_MainWindowManager.FindWindow(_XDEF_WTITLE_BILLINGTIMOUTMESSAGEWINDOW) )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_BILLINGTIMOUTMESSAGEWINDOW );
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAME_1839), 
																			TRUE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE );
						pMessageBox->StartTimer(30000, TRUE);
					}
				}
			}
		}

		g_BillingRemainTimeCounter = g_LocalSystemTime;		
	}
*/
}

