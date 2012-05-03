// XProc_ZeroLevel.cpp: implementation of the XProc_ZeroLevel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ZeroLevel.h"
#include "XProc_ProcessDefine.h"
#include "XMeshContainer.h"
#include "XHeroActionTable.h"

#include "XWindow_Option.h"
#include "XWindow_Debug.h"
#include "XWindow_ZoneName.h"

#include "XWindow_RequestParty.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_AdminNotice.h"
#include "XWindow_ChargeGauge.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_MessengerTooltip.h"
#include "XWindow_MaximumMap.h"
#include "XWindow_MainMenu.h"
#include "XWindow_Messenger.h"
#include "XWindow_JinStatus.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_NameInput.h"
#include "xwindow_chattingdefault.h"
#include "xwindow_Help.h"
#include "Xwindow_QuickSlot.h"
#include "XWindowObjectDefine.h"

#include "xpathfinder.h"
#include "xproc_maingame.h"
#include "XParticleList.h"
#include "XParticleIDDefine.h"

#include "SoundEffectList.h"

#include "XSTWrapper.h"
#include "XSTWrapperCore.h"

#include "XSR_STRINGHEADER.H"

#include "XTextureController.h"

#define _XDEF_DUMMYID_ZEROLEVELPROCESSLOCK		((_XGUIObject *)0x0F)

extern void ReplaceCharacter( LPTSTR _string, TCHAR srcchar, TCHAR changechar );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_ZeroLevel::XProc_ZeroLevel()
{
	setProcessInfo( 0, "Zero Level", __FILE__ );

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
		
	m_FirstEntered = TRUE;	
	m_ToolTipEffectRepeater		= 0;
	m_ToolTipEffectRepeaterFlag = FALSE;

	m_pInventoryWindowPtr = NULL;

	m_pAppearanceList = NULL;
	m_pZerolevelCharacterList = NULL;	
	m_iTotalMobCount = 0;
	m_HeroGroupType = _XGROUP_NANGIN;
	m_EnemyGroupType = _XGROUP_NANGIN;
	m_fEventCtrlTime = 0.0f;	
	for( int i = 0; i < 3; i++ )
	{
		m_CurrentHeroSpeech[i] = NULL;
		m_CurrentEnemyHeroSpeech[i] = NULL;
	}	 
	m_CurrentTalkStageArray = NULL;
	m_CurrentTalkStage = 0;
	

	for( i = 0; i < 9; i++ )
	{
		m_pZeroLevelHeroModel[i] = NULL;
		m_HeroSpeech[i] = NULL;
		m_EnemyHeroSpeech[i] = NULL;
	}	
	
	m_fTempTargetDistance = 0;
	m_fTempFov  = 0;
	m_fTempPitch = 0;
	m_fTempYaw = 0;
	m_fRotateFirstSceneAngle = 0.0f;
	m_bFirstEventSceneEnd = FALSE;
	m_bEndEventSceneStart = FALSE;
	m_bTalkSceneEnd = FALSE;	
	m_bHeroTalking = FALSE;;
	m_bEnemyHeroTalking = FALSE;
	m_bMouseButtonFlag = TRUE;
	m_bEnemyHeroMotioinFlag = FALSE;
	m_bAllEventEnd = FALSE;
	m_vEnemyHeroPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_iEventState = 0;
	m_iCurrentHeroLine = 0;
	m_iCurrentEnemyHeroLine = 0;
	m_SpeechPositionCounter = 0;
	m_SpeechPositionTimeConter = 0;
	m_iHeroImgageDrawPosX = 0;
	m_iHeroImgageDrawPosY = 0;
	m_iEnemyHeroImgageDrawPosX = 0;
	m_iEnemyHeroImgageDrawPosY = 0;
	m_fEventDelayTime = 0.0f;
	m_iEventCameraDistance = 0.0f;
	m_iEventFadeAlphaLevel = 0;
	m_HeroGender = _XGENDER_MAN;
	m_iCurrentTutorial = 0;
	m_bTutorialModeOn = FALSE;
	m_fHelpTime = 0.0f;
	m_fHelpLimiteTime = 0.0f;
	m_NextClickImageCount = 1;
	m_NextClickImageCountRate = 1;
	m_fNextClickTime = 0.0f;
	m_TalkHeroImageOverlap = FALSE;
	m_TalkEnemyHeroImageOverlap = FALSE;
	m_TalkHeroImageSecondRect.left = 0;
	m_TalkHeroImageSecondRect.top = 0;
	m_TalkHeroImageSecondRect.right = 0;
	m_TalkHeroImageSecondRect.bottom = 0;

}

XProc_ZeroLevel::~XProc_ZeroLevel()
{

	
}

BOOL XProc_ZeroLevel::InitializeProcess( void )
{
	g_HeroCurrentUser.InitializeData();
	g_pLocalUser = &g_HeroCurrentUser; // hotblood zerolevel
	
	// timer init
	m_fElapsedFrameTime = 0.0f;
	m_fLocalSystemTime = 0.0f;
	m_LocalSystemTime = 0;
	m_ZeroSystemTimeCounter.Init();

	HeroCharacterLoading();
	LoadZeroLevelInfo();
	#ifdef _XDWDEBUG 
	g_NormalGameExit = TRUE;
#else
	g_NormalGameExit = FALSE;
#endif

	m_TextureArchive.DisposeTexture();	
	
	TCHAR* talkheroimage[9] = { _T(""),
								_T("hotalkface.tga"),
								_T("sootalkface.tga"),								
								_T("bulltalkface.tga"),								
								_T("nocktalkface.tga"),
								_T(""),
								_T(""),
								_T(""),								
								_T("") };
	
	// 대화 이미지 
	m_TextureArchive.GetResourceIndex("talkfaceback.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex(talkheroimage[m_HeroGroupType], _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex(talkheroimage[m_EnemyGroupType], _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);

	m_TextureArchive.BuildTextureList(TRUE);
	
	int drawx=0, drawy=0;
	for( int i = 0; i < 2; i++ )
	{
		m_TalkHeroBackImage[i].Create( drawx, drawy+332, drawx+511, drawy+332+255,
									&m_TextureArchive, m_TextureArchive.FindResource("talkfaceback.tga") );
		m_TalkHeroBackImage[i].SetClipRect(drawx, drawy, drawx+511, drawy+255);
	}
	
	m_TalkHeroImage[0].Create( drawx, drawy+332-63, drawx+512, drawy+332+255-63,
								&m_TextureArchive, m_TextureArchive.FindResource(talkheroimage[m_HeroGroupType]) );
	m_TalkHeroImage[0].SetClipRect(drawx, drawy, drawx+511, drawy+255);
	
	m_TalkHeroImage[1].Create( gnWidth-512, drawy+97-63, gnWidth, drawy+97+255-63,
								&m_TextureArchive, m_TextureArchive.FindResource(talkheroimage[m_EnemyGroupType]) );
	m_TalkHeroImage[1].SetClipRect(drawx, drawy, drawx+511, drawy+255);
	

	m_iHeroImgageDrawPosX = drawx - 512;
	m_iHeroImgageDrawPosY = drawy + 332-63;
	m_TalkHeroBackImage[0].MoveWindow( m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY+63);
	m_TalkHeroImage[0].MoveWindow( m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY);
	
	m_iEnemyHeroImgageDrawPosX = gnWidth;
	m_iEnemyHeroImgageDrawPosY = drawy+97-63;
	m_TalkHeroBackImage[1].MoveWindow( m_iEnemyHeroImgageDrawPosX, m_iEnemyHeroImgageDrawPosY+63 );
	m_TalkHeroImage[1].MoveWindow( m_iEnemyHeroImgageDrawPosX, m_iEnemyHeroImgageDrawPosY );

	m_NextClickImage.Create(0, 0, 25, 25, 
							&m_TextureArchive, m_TextureArchive.FindResource("talkfaceback.tga") );
	m_NextClickImage.SetClipRect(0, 0, 0, 0);
	
	g_MessageBox.SetBorderAlphaLevel( 96 );
	
	g_pLocalUser->ResetLocalUserData();

	// Character move factor===================================================
	m_fMoveFactor			= 0.5f;

	g_vMousePosition		= D3DXVECTOR3( 0.f,0.f,0.f );	
	g_vPickInfinityRayDir   = g_vPickRayDir	= g_vPickRayOrig = g_vMousePosition;
	m_DuplicateCoordCount	= 0;
		
	m_LastTextureHitrateCheckTime1 = m_LastTextureHitrateCheckTime2 = m_LocalSystemTime;
	m_LastPickedItemTime = m_fLocalSystemTime;

	// Set user informations===================================================

	m_CurrentWarpZoneIndex = -1;		
	g_pLocalUser->m_TargetPosition = g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position = g_pLocalUser->m_NextRespawnPosition;	
	
	_XOBBData* OBBData = NULL;
	// get terrain normal
	g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position, &g_pLocalUser->m_CurrentPositionTerrainNormal );
	// Get charater height
	g_pLocalUser->m_PrevPosition.y = g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndOBBHeight( g_pLocalUser->m_Position, OBBData );	
	g_pLocalUser->m_pGoundCollideOBB = OBBData;
	
	g_pLocalUser->ResetAnimationState();		
	g_pLocalUser->ResetProcessTime();
	g_pLocalUser->ResetZoneData();
	g_pLocalUser->MoveStartPosition();
	
	// Set the camera default data ============================================
	g_LodTerrain.m_3PCamera.Set( g_DefaultCameraFov, (float)gnWidth/(float)gnHeight, 
						      g_EnvironmentManager.m_ZValueNear_ViewPlane, 
						      g_EnvironmentManager.m_ZValueFar_ViewPlane);

	// Initialize third person view camera=====================================
	g_LodTerrain.m_3PCamera.SetAdditionalHeightMinDistance( 0.2f ); // 카메라 높이 재조정관련 파라미터를 켜줌.
	g_LodTerrain.m_3PCamera.SetPitchLimits( -85.0f, 80.0f);
	g_LodTerrain.m_3PCamera.SetRollLimits(-0, 0);
	g_LodTerrain.m_3PCamera.Set( 0, 0, 0 );
	g_LodTerrain.m_3PCamera.SetDistanceLimits(0.8f, 150.f);
	g_LodTerrain.m_3PCamera.SetDistance( 5.0f );	

	g_LodTerrain.m_3PCamera.mp_fYaw   = 0.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 8.0f;
	g_LodTerrain.m_3PCamera.mp_fFov = g_DefaultCameraFov-5.0f;	// hotblood->2004.09.08
		
	D3DXVECTOR3 CameraPos;	
	// 플레이어는 처음에 -z 방향으로 보고 있기 때문에, 플레이어의 뒤통수에 카메라를 놓으려면 + 값을 더해주어야 한다.
	CameraPos.x = g_LodTerrain.m_3PCamera.m_TargetPosition.x;
	CameraPos.y = g_LodTerrain.m_3PCamera.m_TargetPosition.y + 1;
	CameraPos.z = g_LodTerrain.m_3PCamera.m_TargetPosition.z + 1;	
	g_LodTerrain.m_3PCamera.SetCameraPosition(CameraPos);

	g_LodTerrain.m_3PCamera.SetTargetPosition( 
		D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y + 2.5f, g_pLocalUser->m_Position.z ) );
	
	// reset camera rotate & zoom velocity...
	g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	

	g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
	g_LodTerrain.RebuildLevel(FALSE);


	// Clear global effect manager=============================================
	g_WorldEffectInstanceManager.Dispose();
		
	// Initialize interface data===============================================	
	m_GameExitSignalTime		=	0;
	m_GameExitFlag				=	FALSE;
	m_GameExitReadyFlag			=	FALSE;
	
	m_ProcessStopTime			=	0;
	m_ProcessStopFlag			=   FALSE;

	// User handler============================================================
	m_EnableAttack				=	FALSE;
	m_ProcessLocalUserDieFlag	=	!FALSE;	
	m_UserDieScreenEffectFlag	=   FALSE;
	m_UserDieScreenAlphaLevel	=	255;
	
	g_ScriptStarted				=	FALSE;

	m_RotateAccelerate			=   FALSE;
	m_CameraRotateSpeed			=	0.0f;
	m_RotateYaw					=	0;
	m_RotatePitch				=	0;

	g_LMenuButtonClicked		=   FALSE;

#ifdef _XDEF_USESCREEENMOTIONBLUR 
	g_ScrrenMotionBlurMode		=	FALSE;
#endif
	
	// Set Timer...	
	g_GameTimer.Init_All();

	m_LastKeyDownTime = 0;
		
	// 초기화가 끝나면 프로세스 활성화..	
	g_MessageBox.ShowWindow( FALSE );

	SetFocus( gHWnd );

	_XIMEKERNEL.SetFocus( NULL );			
	
	((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PEACE );

	m_BgmPlaying = g_BgmPlayFlag;

	g_EnvironmentManager.SetChangeEnvironmentTime(_XDEF_ENVTIMESTARTTIME_NIGHTFALL);

	InitDeviceObject();

	// 미니맵 설정
	g_ServerGameTime.Year = 3;
	g_ServerGameTime.Month = 8;
	g_ServerGameTime.Day = 10;

	g_pLocalUser->m_CharacterCount = 1;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.hair		=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.face		=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.upperbody	=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.lowerbody	=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.lhand		=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.rhand		=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.lfoot		=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.rfoot		=	0;
	g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1		=	0;	
	
	g_pLocalUser->m_SelectedCharacter = 0;
	memcpy( &g_pLocalUser->m_CharacterInfo, &g_pLocalUser->m_AllCharacterInfo[g_pLocalUser->m_SelectedCharacter], sizeof( _XCHARACTER_INFO ) );
	g_pLocalUser->m_CharacterInfo.Set_level(17);
	g_pLocalUser->m_CharacterInfo.Set_sublevel(12);
	//g_pLocalUser->m_CharacterInfo.jinlevel = g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.innerlevel].needjinexperience;
	g_pLocalUser->m_CharacterInfo.minattacklevel = 500;
	g_pLocalUser->m_CharacterInfo.maxattacklevel = 1000;		
	g_pLocalUser->m_CharacterInfo.attacksuccesslevel = 100;	
	g_pLocalUser->m_CharacterInfo.defencelevel	= 220;
	g_pLocalUser->m_CharacterInfo.Set_max_lifepower(10000);
	g_pLocalUser->m_CharacterInfo.Set_max_forcepower(5000);
	g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower(5000);
	
	g_pLocalUser->m_CharacterInfo.Set_current_lifepower(10000);
	g_pLocalUser->m_CharacterInfo.Set_current_forcepower(5000);
	g_pLocalUser->m_CharacterInfo.Set_current_concentrationpower(5000);
	g_pLocalUser->m_CharacterInfo.gender = m_HeroGender;

	g_pLocalUser->m_ModelDescriptor.LinkBipedObject( &m_ZeroLevelHerobipedModel[m_HeroGroupType] );
	g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY, m_pZeroLevelHeroModel[m_HeroGroupType] );	
	
	_XWindow_JinStatus* pJinStatus_Window = (_XWindow_JinStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_JINSTATUSWINDOW);
	if( pJinStatus_Window )
	{
		pJinStatus_Window->SetParameter();	
	}		

	g_pLocalUser->m_PrevPosition.x = g_pLocalUser->m_Position.x = g_pLocalUser->m_TargetPosition.x = m_pZerolevelCharacterList[0].posx;
	g_pLocalUser->m_PrevPosition.z = g_pLocalUser->m_Position.z = g_pLocalUser->m_TargetPosition.z = m_pZerolevelCharacterList[0].posy;	
	
	g_pLocalUser->m_RotateAngle = m_pZerolevelCharacterList[0].rotangle;
	g_pLocalUser->UpdatePosition();	
	g_pLocalUser->m_PrevPosition.y = g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );		
	
	g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
		g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
		g_pLocalUser->m_Position.z ) );
	g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
	g_LodTerrain.RebuildLevel(FALSE);	
	
	g_pLocalUser->m_ModelDescriptor.SetShadowDecalInfo( 0.65f, 0.85f, 1.85f );
	_XUser::CreateIntersectionPoly();
	
	// 무공 셋팅
	HeroStateSetting();	
	
	g_ZeroLevelMode = TRUE;
	AddMob();
		
	m_SendedRequestRestart = FALSE;
	m_FirstEntered = FALSE; // 페이드 아웃 쪽에서 처리	

	g_TerrainTextureArchive.ResetTextureCacheTime();	
	g_WorldObjectTextureArchive.ResetTextureCacheTime();		
	g_EffectTextureArchive.ResetTextureCacheTime();	
	g_CharacterTextureArchive.ResetTextureCacheTime();
	g_MobTextureArchive.ResetTextureCacheTime();	

	int swordeffecttracecount = 0;
	if( g_pLocalUser->m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData )
	{
		swordeffecttracecount = g_pLocalUser->m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData->m_Pointcount;
	}
	g_pLocalUser->m_ModelDescriptor.InitEffectManager(swordeffecttracecount, m_HeroGroupType);	// 검흔
	g_pLocalUser->m_ModelDescriptor.m_EnableEffectProcess = TRUE;
	g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
	g_pLocalUser->m_ModelDescriptor.SetDrawSwordEffect( TRUE );

	
	g_WarpProcessFadeInOutFlag	= FALSE;
	g_DrawFadeInOutFlag			= FALSE;	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Interface window and skill window precacheing	
	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndOBBHeight( g_pLocalUser->m_Position, pLastPickedOBB );
	if( pLastPickedOBB )
	{
		g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = height;
		g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;		
	}
	
	g_pLocalUser->UpdatePosition();

	// reset minimap info...
	_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( pminimapwindow )
	{
		// Set direction to minimap arrow 
		pminimapwindow->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );
		pminimapwindow->SetLocalUserPosition();
		// Set minimap status		
		pminimapwindow->SetCheckLocalUserPosition(g_pLocalUser->m_Position.x,g_pLocalUser->m_Position.z);
		pminimapwindow->SetLocalUserPosition();
	}
	
	_XWindow_Option* pOption_Window = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
	if( pOption_Window )
		pOption_Window->ApplyOptionProperties();
	
	if( pJinStatus_Window )
		pJinStatus_Window->SetButtonStatus(FALSE);

	m_CurrentBGMType=   0;
	m_BGMPlayTimer	=	m_LocalSystemTime + 170000;
	m_ChangingBGM	=	FALSE;

	// string new
	for( i = 0; i < 3; i++ )
	{
		m_CurrentHeroSpeech[i] = new TCHAR[255];	
		memset(m_CurrentHeroSpeech[i], NULL, sizeof(TCHAR)*255);

		m_CurrentEnemyHeroSpeech[i] = new TCHAR[255];
		memset(m_CurrentEnemyHeroSpeech[i], NULL, sizeof(TCHAR)*255);
	}	

#ifdef _XUSEFMOD
	// FMODProc
#else	
	// BGM Setting
	if( g_pBGMSoundObject[0] )
	{
		if( !g_pBGMSoundObject[0]->IsPlaying() )
		{
			g_pBGMSoundObject[0]->Stop();
		}
		g_pBGMSoundObject[0]->Destroy();
		g_pBGMSoundObject[0] = NULL;
	}
	
	SoundInit initsound;
	initsound.m_bLooping = true;
	initsound.m_bStreaming = false;
	initsound.m_bMusic = true;
	
	initsound.m_sFileName = _T( gModulePath );
	initsound.m_sFileName += _T("\\Data\\Sound\\BGM\\");
	initsound.m_sFileName += _T("zerolevelBGM.ogg");
	
	if(AudioMgr()->CreateSound(g_pBGMSoundObject[0]))
	{		
		if(g_pBGMSoundObject[0]->Init(initsound))
		{
			g_pBGMSoundObject[0]->SetVolume(g_pBGMSoundMaxVolume);
			g_pBGMSoundObject[0]->Play();
		}
	}
#endif

	m_Initialized = TRUE;
	
	m_fAIDelayTime	= 0.0f;		
	SetEnableProcess( TRUE );	
	return TRUE;
}

void XProc_ZeroLevel::DestroyProcess( void )
{
	SAFE_DELETE_ARRAY( m_pAppearanceList );	
	SAFE_DELETE_ARRAY( m_pZerolevelCharacterList );

	m_TextureArchive.DisposeTexture();
	m_HeroCharacterTextureArchive.DisposeTexture();	
	m_BackgroundStatic.ReleaseObject();	
	m_ProgressStatic.ReleaseObject();	
	
	g_Moblist.disposeList_zero();
	g_Userlist.disposeList_zero();
	g_pLocalUser->ResetLocalUserTempData();
	g_pLocalUser->m_ModelDescriptor.Reset();
	g_WorldEffectInstanceManager.Dispose();
	
	for( int i = 0; i < 9; i++ )
	{
		SAFE_DELETE( m_pZeroLevelHeroModel[i] );
		SAFE_DELETE_ARRAY( m_HeroSpeech[i] );
		SAFE_DELETE_ARRAY( m_EnemyHeroSpeech[i] );
	}
	for( i = 0; i < 3; i++ )
	{
		SAFE_DELETE_ARRAY( m_CurrentHeroSpeech[i] );
		SAFE_DELETE_ARRAY( m_CurrentEnemyHeroSpeech[i] );
	}	
	SAFE_DELETE_ARRAY( m_CurrentTalkStageArray );
	
	m_DataLoaded = FALSE;	

	SetEnableProcess( FALSE );

	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "PlayZeroLevel", 0 );	

	g_ZeroLevelMode = FALSE;
	g_pLocalUser = &g_CurrentUser; // hotblood zerolevel
}

bool XProc_ZeroLevel::InitDeviceObject( void )
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
	
#ifdef _XDWDEBUG 	
	// Light position indicator	
	if (FAILED( D3DXCreateCylinder(gpDev, 0.0f, 0.15f, 0.30f, 10, 10, &m_pLightConeMesh, NULL) ) )
        return false;	
#endif

	// Initialize light object=================================================
	InitializeLight();
	
	return true;
}

void XProc_ZeroLevel::FirstScene()
{
	g_ViewMainInterface = FALSE;
	switch(m_iEventState)
	{
	case 0:
		{
			if( m_fRotateFirstSceneAngle + 54.0f < 360.0f )
			{
				m_fRotateFirstSceneAngle += 54.0f;
				g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle)+m_fRotateFirstSceneAngle;
			}
			else
			{
				m_fRotateFirstSceneAngle = 360.0f;
				g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle)+m_fRotateFirstSceneAngle;				
				m_iEventState = 1;
				m_fEventCtrlTime = m_fLocalSystemTime;
				g_pLocalUser->m_StanceMotionFirst = TRUE;
				g_pLocalUser->SetAnimation(_XACTION_IDLE);
			}			
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);

			int stringlength = 0;
			// MAXSPEECHCOUNT에 맞게 자르기 위해 MAXSPEECHCOUNT-1, MAXSPEECHCOUNT 를 비워 둬야 한다.
			TCHAR* tmpstring[9] = { _T(""),
									_T("아아, 무엇 때문에 이렇게 싸워야 한단 말인가.           이 싸움을 끝낼 방법은 한 가지뿐이군."),
									_T("기어이 소림과 결판을 내야겠군."),								
									_T("허허, 부처님께서 중생을 구하기 위해서는 지옥에 뛰어들  어야 한다고 했거늘……여기가 바로 지옥이구나!"),								
									_T("젠장, 난장판이 따로 없군. 내가 나서지 않으면 한도 끝도  없겠어."),
									_T(""),
									_T(""),
									_T(""),								
									_T("") };
			switch(m_HeroGroupType) 
			{		
			case _XGROUP_NANGIN:
				break;
			case _XGROUP_GAEBANG:
				{	
					stringlength= strlen(tmpstring[m_HeroGroupType]);
					SAFE_DELETE_ARRAY( m_HeroSpeech[0] );
					m_HeroSpeech[0] = new TCHAR[ stringlength + 1 ];

					memset( m_HeroSpeech[0], 0, sizeof(TCHAR) * (stringlength+1) );
					strcpy( m_HeroSpeech[0], tmpstring[m_HeroGroupType] );					
				}
				break;
			case _XGROUP_BEEGOONG:
				{
					stringlength= strlen(tmpstring[m_HeroGroupType]);
					SAFE_DELETE_ARRAY( m_HeroSpeech[0] );
					m_HeroSpeech[0] = new TCHAR[ stringlength + 1 ];

					memset( m_HeroSpeech[0], 0, sizeof(TCHAR) * (stringlength+1) );
					strcpy( m_HeroSpeech[0], tmpstring[m_HeroGroupType] );	
				}
				break;
			case _XGROUP_SORIM:
				{
					stringlength= strlen(tmpstring[m_HeroGroupType]);
					SAFE_DELETE_ARRAY( m_HeroSpeech[0] );
					m_HeroSpeech[0] = new TCHAR[ stringlength + 1 ];

					memset( m_HeroSpeech[0], 0, sizeof(TCHAR) * (stringlength+1) );
					strcpy( m_HeroSpeech[0], tmpstring[m_HeroGroupType] );	
				}
				break;
			case _XGROUP_NOCKRIM:
				{
					stringlength= strlen(tmpstring[m_HeroGroupType]);
					SAFE_DELETE_ARRAY( m_HeroSpeech[0] );
					m_HeroSpeech[0] = new TCHAR[ stringlength + 1 ];

					memset( m_HeroSpeech[0], 0, sizeof(TCHAR) * (stringlength+1) );
					strcpy( m_HeroSpeech[0], tmpstring[m_HeroGroupType] );	
				}
				break;
			case _XGROUP_MOODANG:
				break;
			case _XGROUP_MAKYO:
				break;
			case _XGROUP_SEGA:
				break;
			case _XGROUP_NOFIXED:
				break;				
			}			
		}
		break;
	case 1:
		{
			if( m_fLocalSystemTime - m_fEventCtrlTime > 2000 )
			{
				m_fEventCtrlTime = m_fLocalSystemTime;
				m_iEventState = 2;				
			}										
		}
		break;
	case 2:
		{
			FLOAT elapsedtime = 0.0f;
			elapsedtime = m_fLocalSystemTime - m_fEventCtrlTime;
			
			m_iHeroImgageDrawPosX = int( (512.0f * elapsedtime) / 1000.0f )-512;
			
			if( m_iHeroImgageDrawPosX > 0 )
			{
				m_iHeroImgageDrawPosX = 0;								
				m_fEventDelayTime = m_fLocalSystemTime;
				m_iEventState = 3;
			}
			m_TalkHeroBackImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY+63);
			m_TalkHeroImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY);			
		}
		break;
	case 3:
		{
			FLOAT elapsedtime = 0.0f;			
			if( m_bTalkSceneEnd )	// 마지막 대사 끝
			{
				elapsedtime = m_fLocalSystemTime - m_fEventCtrlTime;
				
				m_iHeroImgageDrawPosX = -int( (512.0f * elapsedtime) / 1000.0f );
				
				if( m_iHeroImgageDrawPosX < -512 )
				{
					m_iHeroImgageDrawPosX = -512;
					m_iEventState = 4;								
				}							
				m_TalkHeroBackImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY+63);
				m_TalkHeroImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY);								
			}
			else
			{
				m_bHeroTalking = TRUE;
				SpeechHero();
			}
		}
		break;
	case 4:
		{
			g_ViewMainInterface = TRUE;
			m_bFirstEventSceneEnd = TRUE;
			m_bTalkSceneEnd = FALSE;
			m_iEventState = 0;
			m_iCurrentHeroLine = 0;
			m_CurrentTalkStage = 0;
			m_HeroSpeech[0] = NULL;
			g_CurrentFocusedObject = NULL;			
			m_iCurrentTutorial = 1;		
			m_fHelpLimiteTime = 2000.0f;
			m_fHelpTime = m_fLocalSystemTime;
		}
		break;
	}	
}

void XProc_ZeroLevel::EventScene()
{
	// 적보스 준비동작 제어
	if( !g_Userlist.m_smUserList.empty() )
	{
		smdef_XUser::iterator it = g_Userlist.m_smUserList.begin();
		_XUser* _user = (*it).second;
		if( _user )
		{
			if( _user->m_Uniqueid == 1 )
			{
				if( ( _user->m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME) && m_bEnemyHeroMotioinFlag )
				{
					_user->m_ModelDescriptor.SetCurMotion(_XHAN_BATTLEIDLE_NONE, TRUE);							
				}												
			}
		}
	}
	
	switch(m_iEventState) 
	{
	case 0:
		{
			if( m_fRotateFirstSceneAngle + 2.0f < 405.0f )
			{
				m_fRotateFirstSceneAngle += 2.0f;
				g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle) - m_fRotateFirstSceneAngle;		
			}
			else
			{
				m_fRotateFirstSceneAngle = 405.0f;
				g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle) - m_fRotateFirstSceneAngle;
				m_iEventState = 1;
				m_fEventCtrlTime = m_fLocalSystemTime;
				TextSpeechHero();				
			}	
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
		}
		break;
	case 1:
		{
			FLOAT elapsedtime = 0.0f;
			if( m_CurrentTalkStageArray[m_CurrentTalkStage] != NULL )
			{			
				if( ((m_CurrentTalkStageArray[m_CurrentTalkStage] - '0')%2) == 0 )	// 영웅 대화 차례
				{
					if( m_iCurrentHeroLine == 0 )	// 첫 대사 
					{
						elapsedtime = m_fLocalSystemTime - m_fEventCtrlTime;
						
						m_iHeroImgageDrawPosX = int( (512.0f * elapsedtime) / 1000.0f )-512;
						
						if( m_iHeroImgageDrawPosX > 0 )
						{
							m_iHeroImgageDrawPosX = 0;						
							m_bHeroTalking = TRUE;
							SpeechHero();
							m_fEventDelayTime = m_fLocalSystemTime;
						}
						m_TalkHeroBackImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY+63);
						m_TalkHeroImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY);						
					}
					else 
					{
						if( m_bTalkSceneEnd )	// 마지막 대사 끝
						{
							elapsedtime = m_fLocalSystemTime - m_fEventCtrlTime;
							
							m_iHeroImgageDrawPosX = -int( (512.0f * elapsedtime) / 1000.0f );
							
							if( m_iHeroImgageDrawPosX < -512 )
							{
								m_iHeroImgageDrawPosX = -512;
								g_pLocalUser->m_StanceMotionFirst = TRUE;
								g_pLocalUser->SetAnimation(_XACTION_IDLE);
								m_iEventState = 2;								
							}							
							m_TalkHeroBackImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY+63);
							m_TalkHeroImage[0].MoveWindow(m_iHeroImgageDrawPosX, m_iHeroImgageDrawPosY);							
						}
						else
						{
							// 대사 이미지 체인지
							if( (m_CurrentTalkStageArray[m_CurrentTalkStage] - '0')  == 2 )
							{
								m_TalkHeroImageOverlap = TRUE;
								m_TalkHeroImageSecondRect.left = 0;
								m_TalkHeroImageSecondRect.top = 256;
								m_TalkHeroImageSecondRect.right = 511;
								m_TalkHeroImageSecondRect.bottom = 511;								
							//	m_TalkHeroImage[0].SetClipRect(0, 256, 511, 511);
							}
							else
							{
								m_iEventFadeAlphaLevel = 0;
								m_TalkHeroImage[0].SetClipRect(0, 0, 511, 255);
							}

							m_bHeroTalking = TRUE;
							SpeechHero();
						}
					}	
				}
				else	// 적보스 대화 차례
				{
					if( m_iCurrentEnemyHeroLine == 0 )	// 첫 대사 
					{
						elapsedtime = m_fLocalSystemTime - m_fEventCtrlTime;
						
						m_iEnemyHeroImgageDrawPosX = gnWidth - int( (512.0f * elapsedtime) / 1000.0f );
						
						if( m_iEnemyHeroImgageDrawPosX < 512 )
						{
							m_iEnemyHeroImgageDrawPosX = 512;
							m_bEnemyHeroTalking = TRUE;
							SpeechEnemyHero();
							m_fEventDelayTime = m_fLocalSystemTime;
						}
						m_TalkHeroBackImage[1].SetScale( -1, 1 );
						m_TalkHeroBackImage[1].MoveWindow( m_iEnemyHeroImgageDrawPosX+512, m_iEnemyHeroImgageDrawPosY+63 );		
						m_TalkHeroImage[1].SetScale( -1, 1 );
						m_TalkHeroImage[1].MoveWindow( m_iEnemyHeroImgageDrawPosX+512, m_iEnemyHeroImgageDrawPosY );						
					}
					else 
					{
						if( m_bTalkSceneEnd )	// 마지막 대사 끝
						{
							elapsedtime = m_fLocalSystemTime - m_fEventCtrlTime;
							
							m_iEnemyHeroImgageDrawPosX = int( (512.0f * elapsedtime) / 1000.0f ) + 512;
							
							if( m_iEnemyHeroImgageDrawPosX > gnWidth )
							{
								m_iEnemyHeroImgageDrawPosX = gnWidth;
								m_bTalkSceneEnd = FALSE;
								m_CurrentTalkStage++;

								if( !g_Userlist.m_smUserList.empty() )
								{
									smdef_XUser::iterator it = g_Userlist.m_smUserList.begin();
									_XUser* _user = (*it).second;
									if( _user )
									{
										if( _user->m_Uniqueid == 1 && !m_bEnemyHeroMotioinFlag)
										{
											_user->m_ModelDescriptor.SetCurMotion(_XHAN_STANCE, TRUE);
											m_bEnemyHeroMotioinFlag = TRUE;
											m_fEventCtrlTime = m_fLocalSystemTime;											
										}										
									}
								}
							}							
							m_TalkHeroBackImage[1].SetScale( -1, 1 );
							m_TalkHeroBackImage[1].MoveWindow( m_iEnemyHeroImgageDrawPosX+512, m_iEnemyHeroImgageDrawPosY+63 );		
							m_TalkHeroImage[1].SetScale( -1, 1 );
							m_TalkHeroImage[1].MoveWindow( m_iEnemyHeroImgageDrawPosX+512, m_iEnemyHeroImgageDrawPosY );
						}
						else
						{
							// 대사 이미지 체인지
							if( (m_CurrentTalkStageArray[m_CurrentTalkStage] - '0')  == 3 )
							{
								m_TalkEnemyHeroImageOverlap = TRUE;
								m_TalkHeroImageSecondRect.left = 0;
								m_TalkHeroImageSecondRect.top = 256;
								m_TalkHeroImageSecondRect.right = 511;
								m_TalkHeroImageSecondRect.bottom = 511;

							//	m_TalkHeroImage[1].SetClipRect(0, 256, 511, 511);
							}
							else
							{
								m_iEventFadeAlphaLevel = 0;
								m_TalkHeroImage[1].SetClipRect(0, 0, 511, 255);
							}

							m_bEnemyHeroTalking = TRUE;
							SpeechEnemyHero();
						}
					}				
				}						
			}
		}
		break;	
	case 2:
		{
			// 영웅 기수식
			if( m_fLocalSystemTime - m_fEventCtrlTime > 3000 )
			{
				m_iEventState = 3;				
			}				
		}
		break;	
	case 3:
		{
			// 기수식 후 회전
			if( m_fRotateFirstSceneAngle + 3.0f < 405.0f + 540.0f )
			{
				m_fRotateFirstSceneAngle += 3.0f;
				g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle) - m_fRotateFirstSceneAngle;				
				if( m_fRotateFirstSceneAngle > 765.0f )		
				{
					m_fRotateFirstSceneAngle += 1.0f;
					g_LodTerrain.m_3PCamera.mp_fPitch += 0.3;
					m_iEventCameraDistance += 0.12f;
					g_LodTerrain.m_3PCamera.SetDistance(m_iEventCameraDistance);
					g_pLocalUser->m_ModelDescriptor.m_CameraDistance = m_iEventCameraDistance;

					g_pLocalUser->SetAnimation(_XACTION_SPECIAL);					
					
					if( !g_Userlist.m_smUserList.empty() )
					{
						smdef_XUser::iterator it = g_Userlist.m_smUserList.begin();
						_XUser* _user = (*it).second;
						if( _user->m_Uniqueid == 1 )
						{
							_user->m_ModelDescriptor.SetCurMotion(_XHAN_SPECIAL2);							
						}
					}					
				}
			}
			else		// 캐릭터 스페셜 동작 시작
			{				
				m_bAllEventEnd = TRUE;
				m_iEventFadeAlphaLevel = 0;				
				m_iEventState = 4;				
			}	
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
		}
		break;
	case 4:
		{
			// 페이드 아웃
			if( m_bAllEventEnd )
			{
				m_fRotateFirstSceneAngle += 1.0f;
				g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle) - m_fRotateFirstSceneAngle;
				
				g_LodTerrain.m_3PCamera.mp_fPitch += 0.1;				
				g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
				g_LodTerrain.RebuildLevel(FALSE);

#ifdef _XUSEFMOD
				// FMODProc
#else	
				FLOAT fVolume = 0.0f;
				if( g_pBGMSoundObject[0] )
				{
					if( g_pBGMSoundObject[0]->IsPlaying() )
					{
						g_pBGMSoundObject[0]->GetVolume( fVolume );
						fVolume -= 0.01f;
						if( fVolume < 0.0f ) fVolume = 0.0f;
						g_pBGMSoundObject[0]->SetVolume( fVolume );
					}
				}
#endif
				m_iEventFadeAlphaLevel += 10;
				if( m_iEventFadeAlphaLevel > 255 )
				{
					m_iEventFadeAlphaLevel = 255;
				}

#ifdef _XUSEFMOD
				// FMODProc
#else	
				if( fVolume <= 0.01f ) // 소리가 완전히 작아지면
				{
					g_pBGMSoundObject[0]->Destroy();
					g_pBGMSoundObject[0] = NULL;

					g_CurrentFocusedObject = NULL;
					g_ViewMainInterface = TRUE;
					
					// 프로세스 체인지
					g_indexScene = 1;
					((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_ZEROLEVEL;
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash);
				}
#endif
			}			
		}
		break;
	}
}

void XProc_ZeroLevel::DrawEventScene()
{	
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 6 );

	if( m_TalkHeroImageOverlap )
	{
		m_iEventFadeAlphaLevel += 10;
		if( m_iEventFadeAlphaLevel > 255 )
			m_iEventFadeAlphaLevel = 255;

		m_TalkHeroBackImage[0].DrawWithRegion();

		m_TalkHeroImage[0].SetClipRect(0, 0, 511, 255);
		m_TalkHeroImage[0].m_FColor = ((DWORD)(255 - m_iEventFadeAlphaLevel) << 24) | 0xffffff;			
		m_TalkHeroImage[0].DrawWithRegion();

		m_TalkHeroImage[0].SetClipRect(m_TalkHeroImageSecondRect.left, m_TalkHeroImageSecondRect.top, m_TalkHeroImageSecondRect.right, m_TalkHeroImageSecondRect.bottom );
		m_TalkHeroImage[0].m_FColor = ((DWORD)m_iEventFadeAlphaLevel << 24) | 0xffffff;
		m_TalkHeroImage[0].DrawWithRegion();

		m_TalkHeroBackImage[1].DrawWithRegion();
		m_TalkHeroImage[1].DrawWithRegion();
	}
	else if( m_TalkEnemyHeroImageOverlap )
	{
		m_iEventFadeAlphaLevel += 10;
		if( m_iEventFadeAlphaLevel > 255 )
			m_iEventFadeAlphaLevel = 255;

		m_TalkHeroBackImage[0].DrawWithRegion();
		m_TalkHeroImage[0].DrawWithRegion();

		m_TalkHeroBackImage[1].DrawWithRegion();
		m_TalkHeroImage[1].SetClipRect(0, 0, 511, 255);
		m_TalkHeroImage[1].m_FColor = ((DWORD)(255 - m_iEventFadeAlphaLevel) << 24) | 0xffffff;			
		m_TalkHeroImage[1].DrawWithRegion();

		m_TalkHeroImage[1].SetClipRect(m_TalkHeroImageSecondRect.left, m_TalkHeroImageSecondRect.top, m_TalkHeroImageSecondRect.right, m_TalkHeroImageSecondRect.bottom );
		m_TalkHeroImage[1].m_FColor = ((DWORD)m_iEventFadeAlphaLevel << 24) | 0xffffff;
		m_TalkHeroImage[1].DrawWithRegion();			
	}
	else
	{
		m_TalkHeroBackImage[0].DrawWithRegion();
		m_TalkHeroImage[0].DrawWithRegion();
		m_TalkHeroBackImage[1].DrawWithRegion();
		m_TalkHeroImage[1].DrawWithRegion();
	}
	
	m_NextClickImage.DrawWithRegion();
	
	int i = 0;
	if( m_bHeroTalking )
	{
		for( i = 0; i < 3; i++ )
		{
			if( m_CurrentHeroSpeech[i] )
			{
				g_XBaseFont->SetColor( 0xffffffff );
				g_XBaseFont->Puts( 5, m_iHeroImgageDrawPosY+265+(16*i), m_CurrentHeroSpeech[i] );
				g_XBaseFont->Flush();
			}
		}	
	}
	
	if( m_bEnemyHeroTalking )
	{
		for( i = 0; i < 3; i++ )
		{
			if( m_CurrentEnemyHeroSpeech[i] )
			{
				g_XBaseFont->SetColor( 0xffffffff );
				g_XBaseFont->Puts( gnWidth-360, m_iEnemyHeroImgageDrawPosY+265+(16*i), m_CurrentEnemyHeroSpeech[i] );
				g_XBaseFont->Flush();
			}
		}
	}

	if( m_bAllEventEnd )
	{
		D3DCOLOR color = ((DWORD)m_iEventFadeAlphaLevel << 24) | 0xffffff;
		g_ScreenBurnOut[0].color = color;
		g_ScreenBurnOut[1].color = color;
		g_ScreenBurnOut[2].color = color;
		g_ScreenBurnOut[3].color = color;

		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );
		gpDev->SetFVF( D3DFVF_XTLVERTEX );
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );		
	}

	if( m_bTutorialModeOn )
	{
		D3DCOLOR color = 0x50000000;
		g_ScreenBurnOut[0].color = color;
		g_ScreenBurnOut[1].color = color;
		g_ScreenBurnOut[2].color = color;
		g_ScreenBurnOut[3].color = color;

		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );
		gpDev->SetFVF( D3DFVF_XTLVERTEX );
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );		
	}
}

void XProc_ZeroLevel::SpeechHero()
{
	if( !gpInput->GetMouseState()->bButton[0] ) 
		m_bMouseButtonFlag = TRUE;

	if( m_SpeechPositionTimeConter == 0 )
	{
		m_SpeechPositionTimeConter = m_LocalSystemTime;
	}

	DWORD elpasedtime = m_LocalSystemTime - m_SpeechPositionTimeConter;

	if( m_SpeechPositionCounter == 0 )
	{
		m_SpeechPositionCounter+=2;
	}
	if( elpasedtime > 20 )
	{
		if( m_SpeechPositionCounter >= strlen( m_HeroSpeech[m_iCurrentHeroLine] ) )
		{
			if( gpInput->GetMouseState()->bButton[0] && m_bMouseButtonFlag )
			{
				m_NextClickImage.SetClipRect(0, 0, 0, 0);
				m_bMouseButtonFlag = FALSE;
				m_SpeechPositionCounter = 0;			
				m_bHeroTalking = FALSE;
				m_fEventCtrlTime = m_fLocalSystemTime;
				m_iCurrentHeroLine++;
				m_CurrentTalkStage++;
				m_NextClickImageCount = 1;
				m_NextClickImageCountRate = 1;
				m_TalkHeroImageOverlap = FALSE;
				for( int i = 0; i < 3; i++ )
				{
					_ASSERT( !m_CurrentHeroSpeech[i] );
					memset(m_CurrentHeroSpeech[i], 0, sizeof(TCHAR)*255);
				}				
				if( m_HeroSpeech[m_iCurrentHeroLine] == NULL ) 
				{
					// end										
					m_bTalkSceneEnd = TRUE;
					m_CurrentTalkStage--;
					return;
				}
			}
			else
			{
				m_NextClickImage.MoveWindow(333, 534);
				m_NextClickImage.SetClipRect(25*m_NextClickImageCount, 487, 25*m_NextClickImageCount+25, 487+25);
				
				if( m_fNextClickTime == 0.0f )
					m_fNextClickTime = g_fLocalSystemTime;
				
				if( g_fLocalSystemTime - m_fNextClickTime > 40 )
				{
					m_fNextClickTime = g_fLocalSystemTime;
					m_NextClickImageCount += m_NextClickImageCountRate;

					if( m_NextClickImageCount > 3 || m_NextClickImageCount <= 0 )
						m_NextClickImageCountRate *= -1;
				}				

				return;
			}			
		}

		if( gpInput->GetMouseState()->bButton[0] && m_bMouseButtonFlag )
		{
			m_bMouseButtonFlag = FALSE;
			while(1) 
			{
				if( m_SpeechPositionCounter >= strlen( m_HeroSpeech[m_iCurrentHeroLine] ) )
					return;
				if( m_HeroSpeech[m_iCurrentHeroLine][m_SpeechPositionCounter] & 0x80 )
				{
					m_SpeechPositionCounter+=2;		
				}
				else
				{
					m_SpeechPositionCounter++;
				}
				int copysize;
				int stringlength = strlen( m_HeroSpeech[m_iCurrentHeroLine] );
				
				if( stringlength < m_SpeechPositionCounter )
				{
					copysize = stringlength;
				}
				else
				{
					copysize = m_SpeechPositionCounter;
				}
				
				int line = copysize / MAXSPEECHCOUNT;
				
				if( line > 0 )
				{
					strncpy( m_CurrentHeroSpeech[line], m_HeroSpeech[m_iCurrentHeroLine] + (MAXSPEECHCOUNT*line), copysize - (MAXSPEECHCOUNT*line) );
				}
				else
				{
					strncpy( m_CurrentHeroSpeech[line], m_HeroSpeech[m_iCurrentHeroLine], copysize );
				}				
			}			
		}

		m_SpeechPositionTimeConter = m_LocalSystemTime;			
		
		if( m_HeroSpeech[m_iCurrentHeroLine][m_SpeechPositionCounter] & 0x80 )
		{
			m_SpeechPositionCounter+=2;		
		}
		else
		{
			m_SpeechPositionCounter++;
		}
		int copysize;
		int stringlength = strlen( m_HeroSpeech[m_iCurrentHeroLine] );

		if( stringlength < m_SpeechPositionCounter )
		{
			copysize = stringlength;
		}
		else
		{
			copysize = m_SpeechPositionCounter;
		}
		
		int line = copysize / MAXSPEECHCOUNT;
		
		if( line > 0 )
		{
			strncpy( m_CurrentHeroSpeech[line], m_HeroSpeech[m_iCurrentHeroLine] + (MAXSPEECHCOUNT*line), copysize - (MAXSPEECHCOUNT*line) );
		}
		else
		{
			strncpy( m_CurrentHeroSpeech[line], m_HeroSpeech[m_iCurrentHeroLine], copysize );
		}		
	}
}

void XProc_ZeroLevel::SpeechEnemyHero()
{	
	if( !gpInput->GetMouseState()->bButton[0] ) 
		m_bMouseButtonFlag = TRUE;

	if( m_SpeechPositionTimeConter == 0 )
	{
		m_SpeechPositionTimeConter = m_LocalSystemTime;
	}

	DWORD elpasedtime = m_LocalSystemTime - m_SpeechPositionTimeConter;

	if( m_SpeechPositionCounter == 0 )
	{
		m_SpeechPositionCounter+=2;
	}
	if( elpasedtime > 20 )
	{
		if( m_SpeechPositionCounter >= strlen( m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine] ) )
		{
			if( gpInput->GetMouseState()->bButton[0] && m_bMouseButtonFlag )
			{
				m_NextClickImage.SetClipRect(0, 0, 0, 0);
				m_bMouseButtonFlag = FALSE;
				m_SpeechPositionCounter = 0;
				m_bEnemyHeroTalking = FALSE;
				m_fEventCtrlTime = m_fLocalSystemTime;
				m_iCurrentEnemyHeroLine++;
				m_CurrentTalkStage++;
				m_NextClickImageCount = 1;
				m_NextClickImageCountRate = 1;
				m_TalkEnemyHeroImageOverlap = FALSE;
				for( int i = 0; i < 3; i++ )
				{
					memset(m_CurrentEnemyHeroSpeech[i], 0, sizeof(TCHAR)*255);
				}				
				if ( m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine] == NULL ) 
				{
					// end							
					m_bTalkSceneEnd = TRUE;
					m_CurrentTalkStage--;

					return;
				}	
			}
			else
			{
				m_NextClickImage.MoveWindow(994, 299);
				m_NextClickImage.SetClipRect(25*m_NextClickImageCount, 487, 25*m_NextClickImageCount+25, 487+25);
				
				if( m_fNextClickTime == 0.0f )
					m_fNextClickTime = g_fLocalSystemTime;
				
				if( g_fLocalSystemTime - m_fNextClickTime > 40 )
				{
					m_fNextClickTime = g_fLocalSystemTime;
					m_NextClickImageCount += m_NextClickImageCountRate;

					if( m_NextClickImageCount > 3 || m_NextClickImageCount <= 0 )
						m_NextClickImageCountRate *= -1;
				}
				return;
			}
		}
		
		if( gpInput->GetMouseState()->bButton[0] && m_bMouseButtonFlag )
		{
			m_bMouseButtonFlag = FALSE;
			while(1) 
			{
				if( m_SpeechPositionCounter >= strlen( m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine] ) )
					return;
				if( m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine][m_SpeechPositionCounter] & 0x80 )
				{
					m_SpeechPositionCounter+=2;		
				}
				else
				{
					m_SpeechPositionCounter++;
				}
				int copysize;
				int stringlength = strlen( m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine] );
				
				if( stringlength < m_SpeechPositionCounter )
				{
					copysize = stringlength;
				}
				else
				{
					copysize = m_SpeechPositionCounter;
				}
				
				int line = copysize / MAXSPEECHCOUNT;	
				
				if( line > 0 )
					strncpy( m_CurrentEnemyHeroSpeech[line], m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine] + (MAXSPEECHCOUNT*line), copysize - (MAXSPEECHCOUNT*line) );
				else
					strncpy( m_CurrentEnemyHeroSpeech[line], m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine], copysize );				
			}			
		}

		m_SpeechPositionTimeConter = m_LocalSystemTime;	
		
		if( m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine][m_SpeechPositionCounter] & 0x80 )
		{
			m_SpeechPositionCounter+=2;		
		}
		else
		{
			m_SpeechPositionCounter++;
		}
		int copysize;
		int stringlength = strlen( m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine] );

		if( stringlength < m_SpeechPositionCounter )
		{
			copysize = stringlength;
		}
		else
		{
			copysize = m_SpeechPositionCounter;
		}
		
		int line = copysize / MAXSPEECHCOUNT;	
		  
		if( line > 0 )
			strncpy( m_CurrentEnemyHeroSpeech[line], m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine] + (MAXSPEECHCOUNT*line), copysize - (MAXSPEECHCOUNT*line) );
		else
			strncpy( m_CurrentEnemyHeroSpeech[line], m_EnemyHeroSpeech[m_iCurrentEnemyHeroLine], copysize );			
	}
}

void XProc_ZeroLevel::ReleaseDeviceObject( void )
{
#ifdef _XDWDEBUG		
	SAFE_RELEASE( m_pLightConeMesh );
#endif	
}

void XProc_ZeroLevel::PauseProcess( void )
{

}

void XProc_ZeroLevel::UnPauseProcess( void )
{
	
}

void XProc_ZeroLevel::Draw( void )
{
	g_EnvironmentManager.ProcessSkyBox(g_pLocalUser->m_Position, g_pLocalUser->m_Position, g_fElapsedFrameMilisecondTime);
	
	if( CWater::m_WaterRenderingMode != _XWATER_RM_NORMAL ) 
	{
		g_LodTerrain.m_pWaterObjectManager->PreDrawEnvironment();
	}
	
	if( g_LodTerrain.m_3PCamera.m_ImpactValue != 0.0f )
	{
		g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() + g_LodTerrain.m_3PCamera.m_ImpactValue );
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		
		// Set Third person proj.. matrix
		gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());
		// Set Third person view matrix
		gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
		
		g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - g_LodTerrain.m_3PCamera.m_ImpactValue );
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
	
	
	
	g_CharacterEffectGroupManager.Update( g_fElapsedFrameMilisecondTime, m_fLocalSystemTime );

	gpDev->BeginScene();
	gpDev->Clear(0, NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	g_PostProcessManager.BeginPostProcess();

	// Last picking object info.. data intialize...
	g_LastPickedObjectType = _XPICKINGTARGET_NONE;
	g_LastPickedObjectDistance = 9999.0f;
	g_LastPickedObject = NULL;

	// Render Terrain meshes..

	g_EnvironmentManager.DrawSkyBox();
	g_LodTerrain.DrawTerrain();


	//////////////////////////////////////////////////////////////
	// Render world object...
	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_ObjectAmbientColor );

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
		
		g_LodTerrain.RenderAniObject(g_fElapsedFrameMilisecondTime);
	}

	g_XST_Wrapper.Process();
	g_XST_Wrapper.DrawRenderStack();

#ifdef _XDWDEBUG	
	}
#endif

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	// Render monster
	if( !m_bEndEventSceneStart )
		g_Moblist.Render_ZeroLevel();			

	// Render user
	g_Userlist.Render();
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );

	// Render world water object...	
	if(g_LodTerrain.m_pWaterObjectManager)
	{		
		g_LodTerrain.m_pWaterObjectManager->Draw();
	}

	// Render realtime object...
	if( g_bUseRealtimeGenerateObject ) 
		g_LodTerrain.RenderRealtimeObject();	
	
	// Render battle effect

	g_RenderAlphaBlendFlag = FALSE;
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	
	// Render World effect...
	g_WorldEffectInstanceManager.Update( NULL, NULL, NULL, NULL );	
	g_WorldEffectInstanceManager.Render();
	
	g_LodTerrain.m_EffectObjectManager.Update(NULL, NULL, NULL, NULL);
	g_LodTerrain.m_EffectObjectManager.RenderWorldEffect();	

	g_PostProcessManager.EndPostProcess();

	g_Userlist.RenderUserEffectAndToolTip();

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
		if( m_pLightConeMesh )
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
		if( m_pLightConeMesh )
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
		if( m_pLightConeMesh )
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
		line[0].color = D3DCOLOR_ARGB( 255, 255, 0, 255 );
		line[1].color = D3DCOLOR_ARGB( 255, 0, 255, 255 );

		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX ); 
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	}	

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	if(m_ViewRuntimeDebugData) DrawDebugState();
#endif

#ifdef _XDWDEBUG
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	if( m_ViewFPS && !g_LodTerrain.viewterrainrenderingtime )
		g_XBaseFont->Print( 0, 0, 1.0f, "FPS: %d", gnFPS );
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
#endif			
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	
	if(g_pLocalUser->GetMotionClass() == _XACTION_DIE)
	{
		// 혼수 상태 처리
		//DrawComaState();
	}
	else
	{
		// 일반 상태 처리
		if( m_ProcessLocalUserDieFlag ) 
		{
			ProcessLocalUserDie();
		}
		
		// Render Event Scene
		if( m_bEndEventSceneStart || !m_bFirstEventSceneEnd || m_bTutorialModeOn )
		{
			DrawEventScene();
		}
		
		DrawInterface();
				
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

		if( g_CurrentHoveredWarpzoneIndex > -1 )
		{
			if( !g_WarpProcessFadeInOutFlag )
			{
				_XArea* pCurrentFocusedArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_MAPWARP, g_CurrentHoveredWarpzoneIndex );
				if( pCurrentFocusedArea )
				{
					ScrnPos* pScrnpos = gpInput->GetMouseVirScreenPos();
					TCHAR messagestring[128];
					memset( messagestring, 0, 128 );
					sprintf( messagestring, "%s 수구", g_ZoneInfoTable[pCurrentFocusedArea->m_TargetZone].zonename );
					g_InterfaceTooltip.SetToolTipString( pScrnpos->x + 32, pScrnpos->z, messagestring, _XSC_INFORMATION, FALSE );
				}
			}
		}

		if( !g_WarpProcessFadeInOutFlag )
		{
			g_IMEWindow.Draw();
			g_MessageBox.Draw();	

			g_InterfaceTooltip.Draw();
		}	
			
		// 항상 켜기 
		if( g_AlwaysViewUserNameAndItemTooltip )
		{
			g_UsernameTooltipViewTimer = 255;
			g_ItemnameTooltipViewTimer = 255;
		}
		else
		{
			if( g_UsernameTooltipViewTimer > 0 )
			{
				g_UsernameTooltipViewTimer-=2;
				if( g_UsernameTooltipViewTimer < 0 ) g_UsernameTooltipViewTimer = 0;
			}
			if( g_ItemnameTooltipViewTimer > 0 )
			{
				g_ItemnameTooltipViewTimer-=2;
				if( g_ItemnameTooltipViewTimer < 0 ) g_ItemnameTooltipViewTimer = 0;
			}
		}	
	}

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	// End the scene
	gpDev->EndScene();			
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr ) gApp->m_bDeviceLost = true;

	((CEmperorOfDragonsApp*)gApp)->SetCursor( g_ReadyMouseCursorMode );
	
	g_LodTerrain.ProcessEnvScript(g_fElapsedFrameMilisecondTime, g_LodTerrain.m_3PCamera.m_CameraPosition);
	
	CheckTextureUseHitrate();
}

bool XProc_ZeroLevel::Process( void )
{
	// timer setting
	m_ZeroSystemTimeCounter.Update();
	m_fElapsedFrameTime =  m_ZeroSystemTimeCounter.GetElapsedTime(); 
	m_fLocalSystemTime	+= m_fElapsedFrameTime;
	m_LocalSystemTime += (int)m_fElapsedFrameTime;
	
	// 마우스 커서 모드 초기화 (시작은 이곳부터 끝은 드로우 쪽 )
	if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE ) 
		((CEmperorOfDragonsApp*)gApp)->ResetReadyCursor( _XMOUSETYPE_PEACE );		
	else
		((CEmperorOfDragonsApp*)gApp)->ResetReadyCursor( _XMOUSETYPE_BATTLE );		

	if( m_bEndEventSceneStart ) // 마지막 이벤트 씬 
	{
		g_pLocalUser->Process();
		g_Userlist.Process_ZeroLevel();
		if( m_fLocalSystemTime - m_fAIDelayTime > 1000 )
		{
			m_fAIDelayTime = m_fLocalSystemTime;			
			if( !g_Userlist.m_smUserList.empty() )
			{
				_XUser* _user;
				smdef_XUser::iterator it;
				for (it = g_Userlist.m_smUserList.begin(); it != g_Userlist.m_smUserList.end(); ++ it)
				{
					_user = (*it).second;
					if( _user )
					{
						_user->m_LastRecivedMessageTime = m_LocalSystemTime;				
					}						
				}
			}
		}
		EventScene();
	}
	else if( !m_bFirstEventSceneEnd ) // 처음 이벤트 씬
	{
		g_CurrentFocusedObject = _XDEF_DUMMYID_ZEROLEVELPROCESSLOCK;

		g_pLocalUser->Process();
		g_Userlist.Process_ZeroLevel();
		if( m_fLocalSystemTime - m_fAIDelayTime > 1000 )
		{
			m_fAIDelayTime = m_fLocalSystemTime;			
			if( !g_Userlist.m_smUserList.empty() )
			{
				_XUser* _user;
				smdef_XUser::iterator it;
				for (it = g_Userlist.m_smUserList.begin(); it != g_Userlist.m_smUserList.end(); ++ it)
				{
					_user = (*it).second;
					if( _user )
					{
						_user->m_LastRecivedMessageTime = m_LocalSystemTime;				
					}						
				}
			}
		}
		FirstScene();	
	}
	else if( m_bTutorialModeOn )
	{
		if( gpInput->GetMouseState()->bButton[0] && m_bMouseButtonFlag && m_bTutorialModeOn )
		{
			g_CurrentFocusedObject = NULL;
			m_bMouseButtonFlag = FALSE;
			g_pLocalUser->m_PrevMoveTime = 0.0f;
			
			_XWindow_Help* pHelp_Window = (_XWindow_Help*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_HELP );
			if( pHelp_Window->CheckCloseButton() )
			{
				m_fHelpTime = m_fLocalSystemTime;
				pHelp_Window->ShowWindow( FALSE );
				_XPlayInterfaceSound( ID_SR_INTERFACE_BUTTON_WAV );
				
				switch(m_iCurrentTutorial) 
				{
				case 1:
					m_fHelpLimiteTime = 2000.0f;
					m_iCurrentTutorial = 2;
					m_bTutorialModeOn = FALSE;
					break;
				case 2:						
					m_fHelpLimiteTime = 5000.0f;
					m_iCurrentTutorial = 3;
					m_bTutorialModeOn = FALSE;
					break;
				case 3:
					m_fHelpLimiteTime = 0.0f;
					m_iCurrentTutorial = 4;
					m_bTutorialModeOn = FALSE;
					break;
				case 4:
					m_fHelpLimiteTime = 10000.0f;
					m_iCurrentTutorial = 5;
					m_bTutorialModeOn = FALSE;
					break;
				case 5:
					m_fHelpLimiteTime = 0.0f;
					m_iCurrentTutorial = 6;
					m_bTutorialModeOn = FALSE;
					break;
				case 6:
					m_iCurrentTutorial = 100;	// 종료
					m_bTutorialModeOn = FALSE;
					break;					
				}					
			}			
		}
		if( !gpInput->GetMouseState()->bButton[0] )
			m_bMouseButtonFlag = TRUE;

	}
	else
	{
		// 튜토리얼 발동
		if( m_iCurrentTutorial < 7 )
		{
			if( !m_bTutorialModeOn && ( m_fLocalSystemTime - m_fHelpTime > m_fHelpLimiteTime ) )
			{
				m_bTutorialModeOn = TRUE;
				g_CurrentFocusedObject = _XDEF_DUMMYID_ZEROLEVELPROCESSLOCK;
				TutorialMode();
			}			
		}		

		ProcessInterface();
		
		g_pLocalUser->Process();
		m_fRotateFirstSceneAngle = 0.0f;
		
		g_Moblist.Process_ZeroLevel();
		g_Userlist.Process_ZeroLevel();
		
		if( m_fLocalSystemTime - m_fAIDelayTime > 1000 )
		{
			m_fAIDelayTime = m_fLocalSystemTime;
			g_Moblist.MobAIProcess();
			if( !g_Userlist.m_smUserList.empty() )
			{
				_XUser* _user;
				smdef_XUser::iterator it;
				for (it = g_Userlist.m_smUserList.begin(); it != g_Userlist.m_smUserList.end(); ++ it)
				{
					_user = (*it).second;
					if( _user )
					{
						_user->m_LastRecivedMessageTime = m_LocalSystemTime;				
					}						
				}
			}
		}
		FLOAT dx = m_vEnemyHeroPos.x - g_pLocalUser->m_Position.x;
		FLOAT dz = m_vEnemyHeroPos.z - g_pLocalUser->m_Position.z;
		float length = _XFC_sqrt(dx*dx + dz*dz);
						
		if( length < 5.0f )
		{
			m_bEndEventSceneStart = TRUE;
			D3DXVECTOR3 tmpDir;
			D3DXVECTOR3 targetpos;

			tmpDir = m_vEnemyHeroPos - g_pLocalUser->m_Position;
			D3DXVec3Normalize(&tmpDir, &tmpDir);
			targetpos = g_pLocalUser->m_Position + tmpDir * (length*1.5f);
			_XOBBData* pLastPickedOBB = NULL;
			FLOAT height = g_LodTerrain.GetTerrainAndOBBHeight(targetpos, pLastPickedOBB);			
			targetpos.y = height+0.8f;
			m_iEventCameraDistance = 6.0f;
			g_LodTerrain.m_3PCamera.SetDistance(m_iEventCameraDistance);
			g_LodTerrain.m_3PCamera.SetTargetPosition(targetpos);			
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
			g_pLocalUser->SetAnimation(_XACTION_TALK);
			g_pLocalUser->m_CurrentSelectedSkillType = 0;
			g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
			
			g_ViewMainInterface = FALSE;
			
			g_CurrentFocusedObject = _XDEF_DUMMYID_ZEROLEVELPROCESSLOCK;
		}
	}	

	// 에러 발생시 프로세스 종료 메세지 및 시그널 처리
	if( m_ProcessStopFlag ) Process_ProcessStop();

#ifdef _XUSEFMOD
	g_FMODWrapper.ProcessBGM();
	g_FMODWrapper.FMODUpdate();	
#endif
	
	return true;
}

#define _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE

BOOL XProc_ZeroLevel::LoadHeroDescript( LPTSTR szFilename, int selectedhero )
{
	FILE* pScripterFile = NULL;	

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	pScripterFile = g_ScriptArchive.GetPackedFile( szFilename );
#else
	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	pScripterFile = fopen( szFilename, "rt" );
#endif	


	if( !pScripterFile )
	{
		_XFatalError( "XProc_ZeroLevel::LoadHeroDescript( %s ) File open", szFilename );
		return FALSE; 
	}

	TCHAR argIDString[256];
	TCHAR argValueString[768];
	TCHAR parsingString[768];

	int swordtraceeffectindex = 0;

	while( 1 )
	{				
		if( feof(pScripterFile) ) break;
	
		memset( parsingString, 0, 768 );
		if( !fgets( parsingString, 767, pScripterFile ) ) break;

		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;

		memset( argIDString, 0, 256 );
		memset( argValueString, 0, 768 );
		sscanf( parsingString, "%s %s", argIDString, argValueString );

		if( strncmp( argIDString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}
		else if( strcmpi( argIDString, "_XM_BASEMODEL" ) == 0 )
		{			
			if( !m_ZeroLevelHerobipedModel[selectedhero].Load( g_ModelArchive.GetPackedFile( argValueString ), argValueString ) ) break;
		}
		else if( strcmpi( argIDString, "_XM_RESERVEANISTACK" )== 0 )
		{
			int stackcount = atoi( argValueString );
			if( !m_ZeroLevelHerobipedModel[selectedhero].ReserveAnimationStack( stackcount ) ) break;
		}
		else if( strcmpi( argIDString, "_XM_SWORDTRACECOUNT" ) == 0 )
		{
			m_ZeroLevelHerobipedModel[selectedhero].CreateSwordTraceInfoData( atoi( argValueString ) );
			swordtraceeffectindex = 0;
		}
		else if( strcmpi( argIDString, "_XM_SWORDTRACE" ) == 0 )
		{
			D3DXVECTOR3 toppos, bottompos;
			sscanf( parsingString, "%s %f %f %f %f %f %f", argIDString, &toppos.x,&toppos.y,&toppos.z,&bottompos.x,&bottompos.y,&bottompos.z );

			m_ZeroLevelHerobipedModel[selectedhero].m_pSwordTraceInfoData->SetPointList( swordtraceeffectindex, toppos, bottompos );			
		}
		else  if( strcmpi( argIDString, "_XM_TRACELINK" ) == 0 )
		{
			LPTSTR pParseBipedName = strstr( parsingString, _T( "Bip" ) );
			ReplaceCharacter( pParseBipedName, _T('\r'), _T('\0') );
			ReplaceCharacter( pParseBipedName, _T('\n'), _T('\0') );
			ReplaceCharacter( pParseBipedName, _T('\t'), _T('\0') );
			int nodeindex = m_ZeroLevelHerobipedModel[selectedhero].FindObject( pParseBipedName );

			if( nodeindex < 0 )
			{
				_XFatalError( "Can't find linked biped node : %s", pParseBipedName );
				break;
			}

			m_ZeroLevelHerobipedModel[selectedhero].m_pSwordTraceInfoData->SetLinkedBiped( swordtraceeffectindex, nodeindex );
				
			swordtraceeffectindex++;
		}
		else if( strcmpi( argIDString, "_XM_ANIMATION" ) == 0 )
		{
			_XLog("%s - %d", argValueString, m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt());
			if( !m_ZeroLevelHerobipedModel[selectedhero].LoadKey( g_AniArchive.GetPackedFile( argValueString), argValueString ) ) break;
		}
		else if( strcmpi( argIDString, "_XM_MOTIONNAME" ) == 0 )
		{
		}
		else if( strcmpi( argIDString, "_XM_ATTACKFRAME" ) == 0 )
		{
			if( m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 ) )
			{
				int	 commandframe[9] = { 255, 255, 255, 255, 255, 255, 255, 255, 255 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, &commandframe[0], &commandframe[1], 
																					 &commandframe[2], &commandframe[3],
																					 &commandframe[4], &commandframe[5],
																					 &commandframe[6], &commandframe[7],
																					 &commandframe[8] );

				if( commandframe[0] == 255 ) commandframe[0] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[1] == 255 ) commandframe[1] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[2] == 255 ) commandframe[2] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[3] == 255 ) commandframe[3] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[4] == 255 ) commandframe[4] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[5] == 255 ) commandframe[5] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[6] == 255 ) commandframe[6] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[7] == 255 ) commandframe[7] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[8] == 255 ) commandframe[8] = _XDEF_NOEFFECTVALUE; 

				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 0, commandframe[0] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 1, commandframe[1] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 2, commandframe[2] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 3, commandframe[3] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 4, commandframe[4] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 5, commandframe[5] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 6, commandframe[6] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 7, commandframe[7] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackFrame( 8, commandframe[8] );
			}
		}
		else if( strcmpi( argIDString, "_XM_ATTACKCLASS" ) == 0 )
		{
			if( m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 ) )
			{
				int	 attackclass[9] = { _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, 
					                    _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW,
										_XDEF_ATTACKCLASS_LOW };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
					&attackclass[0], &attackclass[1],  &attackclass[2], &attackclass[3], &attackclass[4],
					&attackclass[5],  &attackclass[6], &attackclass[7], &attackclass[8]	);
				
				if( attackclass[0] == 0 ) attackclass[0] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[1] == 0 ) attackclass[1] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[2] == 0 ) attackclass[2] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[3] == 0 ) attackclass[3] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[4] == 0 ) attackclass[4] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[5] == 0 ) attackclass[5] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[6] == 0 ) attackclass[6] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[7] == 0 ) attackclass[7] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[8] == 0 ) attackclass[8] = _XDEF_ATTACKCLASS_LOW; 
				
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 0, attackclass[0] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 1, attackclass[1] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 2, attackclass[2] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 3, attackclass[3] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 4, attackclass[4] );				
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 5, attackclass[5] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 6, attackclass[6] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 7, attackclass[7] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetAttackClass( 8, attackclass[8] );				
			}
		}
		else if( strcmpi( argIDString, "_XM_WEAPONCLASS" ) == 0 )
		{
			if( m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 ) )
			{
				int	 weaponclass[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
					    &weaponclass[0], &weaponclass[1],  &weaponclass[2], &weaponclass[3], &weaponclass[4],
						&weaponclass[5],  &weaponclass[6], &weaponclass[7], &weaponclass[8] );
				
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 0, weaponclass[0] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 1, weaponclass[1] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 2, weaponclass[2] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 3, weaponclass[3] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 4, weaponclass[4] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 5, weaponclass[5] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 6, weaponclass[6] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 7, weaponclass[7] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetWeaponClass( 8, weaponclass[8] );
			}
		}
		else if( strcmpi( argIDString, "_XM_WEAPONITEMCLASS" ) == 0 )
		{
			if(m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 ))
			{
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->UseItemIndex = atoi( argValueString );
			}
		}		
		else if( strcmpi( argIDString, "_XM_SOUNDEFFECT" ) == 0 )
		{
			if(m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 ))
			{
				int	 soundframe[9];
				TCHAR soundfilename[9][64];
				memset( soundfilename, 0, sizeof(TCHAR) * 64 * 9 );
				sscanf( parsingString, "%s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s", argIDString, 
						&soundframe[0], soundfilename[0], &soundframe[1], soundfilename[1],
						&soundframe[2], soundfilename[2], &soundframe[3], soundfilename[3],
						&soundframe[4], soundfilename[4], &soundframe[5], soundfilename[5],
						&soundframe[6], soundfilename[6], &soundframe[7], soundfilename[7],
						&soundframe[8], soundfilename[8] );
				
				int soundresource[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
				
				for( int i = 0; i < 9; i++ )
				{
					if( strlen(soundfilename[i]) )
					{
#ifdef _XUSEFMOD
						soundresource[i] = g_FMODWrapper.InsertSound( soundfilename[i], FSOUND_HW3D );
						if( soundresource[i] == -1 )
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
							//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
						}
#else	
						soundresource[i] = g_SoundEffectArchive.FindResource( soundfilename[i] );
						if( soundresource[i] == -1 )
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
							//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
						}
#endif
					}
					else 
					{
						soundframe[i] = _XDEF_NOEFFECTVALUE;
						soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
					}

					m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetSoundEffectFrame( i, soundframe[i], soundresource[i] );						
				}								
			}
		}
		else if( strcmpi( argIDString, "_XM_SPECIALEFFECT" ) == 0 )
		{
			if(m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 ))
			{
				int	 effectframe[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
				int  effectid[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", argIDString, 
					&effectframe[0], &effectid[0], &effectframe[1], &effectid[1],
					&effectframe[2], &effectid[2], &effectframe[3], &effectid[3],
					&effectframe[4], &effectid[4], &effectframe[5], &effectid[5],
					&effectframe[6], &effectid[6], &effectframe[7], &effectid[7],
					&effectframe[8], &effectid[8] );

				if( effectframe[0] == -1 ){ effectframe[0] = _XDEF_NOEFFECTVALUE; effectid[0] = _XDEF_NOEFFECTWORDVALUE; }
				if( effectframe[1] == -1 ){ effectframe[1] = _XDEF_NOEFFECTVALUE; effectid[1] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[2] == -1 ){ effectframe[2] = _XDEF_NOEFFECTVALUE; effectid[2] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[3] == -1 ){ effectframe[3] = _XDEF_NOEFFECTVALUE; effectid[3] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[4] == -1 ){ effectframe[4] = _XDEF_NOEFFECTVALUE; effectid[4] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[5] == -1 ){ effectframe[5] = _XDEF_NOEFFECTVALUE; effectid[5] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[6] == -1 ){ effectframe[6] = _XDEF_NOEFFECTVALUE; effectid[6] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[7] == -1 ){ effectframe[7] = _XDEF_NOEFFECTVALUE; effectid[7] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[8] == -1 ){ effectframe[8] = _XDEF_NOEFFECTVALUE; effectid[8] = _XDEF_NOEFFECTWORDVALUE; } 
												
				for( int i = 0; i < 9; i++ )
				{
					m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectFrame( i, effectframe[i], effectid[i] );
				}								
			}
		}
		else if( strcmpi( argIDString, "_XM_EPOSITION" ) == 0 )
		{
			if( m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 ) )
			{
				int	 effectpositionclass[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
					&effectpositionclass[0], &effectpositionclass[1], &effectpositionclass[2], &effectpositionclass[3], &effectpositionclass[4],
					&effectpositionclass[5], &effectpositionclass[6], &effectpositionclass[7], &effectpositionclass[8] );
				
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 0, effectpositionclass[0] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 1, effectpositionclass[1] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 2, effectpositionclass[2] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 3, effectpositionclass[3] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 4, effectpositionclass[4] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 5, effectpositionclass[5] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 6, effectpositionclass[6] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 7, effectpositionclass[7] );
				m_ZeroLevelHerobipedModel[selectedhero].GetMotion( m_ZeroLevelHerobipedModel[selectedhero].GetMotionCnt()-1 )->SetEffectPosition( 8, effectpositionclass[8] );
			}
		}
	};

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
	g_ScriptArchive.ClosePackage();					
#else
	fclose( pScripterFile );
#endif

	return TRUE;
}

BOOL XProc_ZeroLevel::GetMousePosition( void )
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

void XProc_ZeroLevel::SetMovetoMousePickedPoint( void )
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

			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{
				// Set direction to minimap arrow 
				pminimapwindow->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI  );
				// Set minimap status
				pminimapwindow->SetLocalUserPosition();
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

void XProc_ZeroLevel::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( message == WM_COMMAND )
	{
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
			QuickSlotMessageHandling(wparam, lparam);
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
		case _XDEF_OPTION_GAME_GAMEGUIDE			:
		case _XDEF_OPTION_GAME_VIEWUSERTOOLTIP			:
		case _XDEF_OPTION_GAME_HELPSYSTEM			:
		case _XDEF_OPTION_GAME_CAMERAQUATERVIEW		:
		case _XDEF_OPTION_GAME_USEFIRSTPERSONVIEW	:
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

		// Minimap window message handling------------------------------------------
		case _XDEF_MINIMAP_CLOSEBUTTON				:
		case _XDEF_MINIMAP_ZOOMINBUTTON				:			
		case _XDEF_MINIMAP_ZOOMOUTBUTTON			:
		case _XDEF_MINIMAP_ZOOMMAXBUTTON			:
		case _XDEF_MAXMINIMAP_CLOSEBUTTON			:
		case _XDEF_MAXMINIMAP_NPCVIEWBUTTON			:
		case _XDEF_MAXMINIMAP_PARTYVIEWBUTTON		:
		case _XDEF_MAXMINIMAP_APPLYBUTTON			:
			MinimapWindowMessageHandling( wparam, lparam );
			break;

		case  _XDEF_MAINMENU_SYSTEMMENU				:
		case  _XDEF_MAINMENU_MINIMAP				:
			SystemMenuWindowMessageHandling( wparam, lparam );
			break;
		case _XDEF_GAMECLOSE :
			{
				if( !m_GameExitFlag )
				{	
					m_GameExitFlag = TRUE;
					m_GameExitSignalTime = 0;
				}
				PostMessage( gHWnd, WM_CLOSE, NULL, NULL );
			}
			break;
		case _XDEF_CANCELGAMECLOSE	:
			m_GameExitReadyFlag = FALSE;
			m_GameExitFlag = FALSE;
			m_GameExitSignalTime = 0;			
			break;
		}
	}	
}

void XProc_ZeroLevel::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{

}

void XProc_ZeroLevel::OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{

}

bool XProc_ZeroLevel::OnKeyboardPoll(_XInput *pInput)
{
	return true;
}

bool XProc_ZeroLevel::OnKeyDown(WPARAM wparam, LPARAM lparam)
{
	if( ::g_CurrentFocusedObject ) return false;

	DWORD scancode = (HIWORD( lparam ) & 0xFF);
	switch( scancode ) 
	{
			// 미니맵 확대 
		case DIK_F2 :
			{
				if( gpInput->CheckKeyPress( DIK_LCONTROL ) )
				{
					_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
					if( pminimapwindow )
					{
						pminimapwindow->m_MinimapZoomRate = 0.075f;
						pminimapwindow->m_RateOfMapSizeToTerrain = 0.5f*(1024.0f/g_LodTerrain.m_Width);
						pminimapwindow->SetLocalUserPosition();
					}
				}
				else
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MINIMAP_ZOOMINBUTTON, 0), (LPARAM)gHWnd);
				}
			}
			return true;
			
			// 미니맵 축소 
		case DIK_F3 :
			{
				if( gpInput->CheckKeyPress( DIK_LCONTROL ) )
				{
					_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
					if( pminimapwindow )
					{
						pminimapwindow->m_MinimapZoomRate = 0.25f;
						pminimapwindow->m_RateOfMapSizeToTerrain = 0.15f*(1024.0f/g_LodTerrain.m_Width);
						pminimapwindow->SetLocalUserPosition();
					}
				}
				else
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MINIMAP_ZOOMOUTBUTTON, 0), (LPARAM)gHWnd);
				}
			}
			return true;
		// Window Clear
		case DIK_F5 :		
			if(g_ViewMainInterface)
			{				     
				g_ViewMainInterface = !g_ViewMainInterface;
			}
			else
			{			
				g_ViewMainInterface = TRUE;
			}
			return true;

		case DIK_O : // Option window 
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_SYSTEMMENU, 0), (LPARAM)gHWnd);
			return true;
	
		// Quick slot 단축키 처리
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
			{
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
					}

					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);

					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 사용
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(_XSkillSelectCheckCallBack(0, skillid))
						{
							switch(g_SkillProperty[skillid]->usefulType)
							{
							case _XSI_USEFULTYPE_ACTIVESKILL :
								{
									if(SelectActiveSkill(skillid))
										g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(index);
								}
								break;
							case _XSI_USEFULTYPE_CHARGESKILL :
								{
									if(SelectChargeSkill(skillid))
										g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(index);
								}
								break;
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
			// target 삭제
			if(g_pLocalUser->m_ViewTarget)
			{
				if(g_pLocalUser->m_ViewTargetType == g_pLocalUser->m_AttackTargetObjectType)
				{
					if(g_pLocalUser->m_ViewTarget == g_pLocalUser->m_AttackTarget)
					{
						g_pLocalUser->ResetViewTarget();
						g_pLocalUser->ResetAttackTarget();
						return true;
					}
				}
				g_pLocalUser->ResetViewTarget();
				return true;
			}

			if(g_pLocalUser->m_AttackTarget)
			{
				g_pLocalUser->ResetAttackTarget();
				return true;
			}
		}
		return true;	
	}
	
	return false; 	
}

bool XProc_ZeroLevel::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
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
			if(g_pQuickSlot_Window)
			{
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
					// skill 처리
					short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
					if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHARGESKILL)
					{
						// charge skill
						switch(g_pLocalUser->m_CurrentBattleState)
						{
						case _XUSER_STATE_1 :
							g_pLocalUser->ResetSkillAnimation();
							break;
						case _XUSER_STATE_2 :
							{
								if(g_pLocalUser->m_ChargeSendStartPacket)
									g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
								else
									g_pLocalUser->ResetSkillAnimation();
							}
							break;
						case _XUSER_STATE_3 :
						case _XUSER_STATE_4 :
						case _XUSER_STATE_5 :
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
							break;
						case _XUSER_STATE_6 :
							g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_7;
							break;
						}
					}
				} // if(type == _XQS_SLOTTYPE_SKILL)
				else if(type == _XQS_SLOTTYPE_ITEM)
				{
					int invennumber = g_pQuickSlot_Window->GetQuickSlotItemInvenNumber(index);
					g_pQuickSlot_Window->SetCurrentSelectedItemIndex(index);
					g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(g_LocalSystemTime);
					g_pLocalUser->UseItem(invennumber);
				}
			}
		}
	}	
	return true;
}

bool XProc_ZeroLevel::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{
	// Get the pick ray from the mouse position	
	POINT mousept = { pPos->x, pPos->z };
	GetVectorFromMousePos( mousept, g_vPickRayDir, g_vPickRayOrig );
	g_vPickInfinityRayDir = g_vPickRayDir * 1000.0f;
	
	if( !::g_CurrentFocusedObject || ::g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND  )
	{	
		float fmx = pState->AX*0.5*m_fMoveFactor;
		float fmy = pState->AY*0.5*m_fMoveFactor;		
		
		// 이벤트 일때는 마우스피킹을 막음
		if( pState->bButton[1] && !m_bEndEventSceneStart && m_bFirstEventSceneEnd)
		{	
			// Camera rotation...
			
			g_LodTerrain.m_3PCamera.AddPitch(fmy);
			g_LodTerrain.m_3PCamera.AddYaw(fmx);
			
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			
			// Rebuild loose quadtree...
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel();			
			
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{
				// Set direction to minimap arrow 
				pminimapwindow->SetRotateFrustum( _X_RAD( 180 - g_LodTerrain.m_3PCamera.GetYaw() ) );
			}
			
			((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_CROSSPOINTER );			
		}		
	}
					
	return true; 	
}

bool XProc_ZeroLevel::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
	if( ::g_CurrentFocusedObject && ::g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND  )
	{		
		return true;
	}
	
	switch( p_MB )
	{
	case _XmbLeft :
		if( bPushed && m_bFirstEventSceneEnd && !m_bEndEventSceneStart)
		{
			g_LodTerrain.GetPickedPoint();
		}
		else
		{
			g_MouseFirstClickPassed = FALSE;		
		}

		break;			
	}
	
	return true; 
}

void XProc_ZeroLevel::OnMouseWheel( short zDelta )
{
	if( !g_MainWindowManager.ProcessWheelMessage(zDelta) )
	{
		FLOAT MoveDelta  = (FLOAT)zDelta*0.005F;
		
#ifdef _XADMINISTRATORMODE
		if( gpInput->CheckKeyPress( DIK_LCONTROL ) ) MoveDelta *= _XDEF_RAPID_RATE;
#endif
		if(!m_bEndEventSceneStart && m_bFirstEventSceneEnd && (g_LodTerrain.m_3PCamera.GetDistance() - MoveDelta < 5.0f) )	
		{
			// NPC 대화 모드 일때는 마우스휠작용을 막음
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - MoveDelta );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			
			g_pLocalUser->m_ModelDescriptor.m_CameraDistance = g_LodTerrain.m_3PCamera.GetDistance();
		}
	}

}

void XProc_ZeroLevel::OnMouseLBtnDblClick( WPARAM wparam )
{

}

void XProc_ZeroLevel::OnMouseRBtnDblClick( WPARAM wparam )
{

}

void XProc_ZeroLevel::OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{

}

void XProc_ZeroLevel::OnWindowSizeChangeNotify( void )
{

}

void XProc_ZeroLevel::UpdateProgress( int increase )
{
	if( m_Progress >= increase ) return;

	for( int i = m_Progress; i < increase; i++ )
	{
		m_ProgressBarStatic.SetXPercentage( i, 1, FALSE );
		HeroCharacterDraw();
		//_XDWINPRINT( "Loading progress : %d", i );
	}

	m_Progress = increase;	
}

void XProc_ZeroLevel::SetMessage( LPTSTR message )
{
	memset( m_Message, 0, sizeof(TCHAR) * 256 );
	
	if( !message || strlen(message) <= 0 )  return;
	
	strcpy( m_Message, message );
	
	messagecoord.x = (gnWidth>>1);
	messagecoord.y = (gnHeight) - g_XBaseFont->GetHeight() - 19;
}

void XProc_ZeroLevel::InitializeRenderState( void )
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

void XProc_ZeroLevel::InitializeLight( void )
{	
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
		
	gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	gpDev->LightEnable( 0, TRUE );
}

BOOL XProc_ZeroLevel::LoadTerrain( LPSTR Filename )
{
	assert( Filename );

	_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);

	if( !g_LodTerrain.LoadTerrain( Filename ) )
	{		
		return FALSE;
	}

	return TRUE;
}

void XProc_ZeroLevel::CheckTextureUseHitrate( void )
{
	
}

void XProc_ZeroLevel::DrawInterface( void )
{		
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
	
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );	
	gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );
	if( g_ViewMainInterface )
	{
		g_MainWindowManager.Draw();			
	}
}

void XProc_ZeroLevel::ProcessLocalUserDie()
{

}

#ifdef _XDWDEBUG 
void XProc_ZeroLevel::DrawDebugState()
{

}
#endif

void XProc_ZeroLevel::GetVectorFromMousePos(POINT& pt, D3DXVECTOR3& raydir, D3DXVECTOR3& rayorg )
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

void XProc_ZeroLevel::Process_ProcessStop()
{
		if( m_ProcessStopTime == 0 )
	{
		m_ProcessStopTime = m_LocalSystemTime;
	}
	else
	{
		DWORD progresstime = (m_LocalSystemTime - m_ProcessStopTime);
		if( progresstime < _XDEF_PROCESSTOP_DELAYTIME )
		{	
			DWORD seconds = (( _XDEF_PROCESSTOP_DELAYTIME - progresstime ) / 1000);

			if( seconds == 0 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_MAIGAME_EXITGAME, TRUE );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 255, 0 ) );
			}
			else
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_OPTION_EXITGAME, " ", TRUE );
				
				TCHAR messagestring[128];
				LoadString( gHInst, ID_STRING_MAIGAME_COUNTEXIT, messagestring, 127 );
				g_MessageBox.SetMessage( messagestring, seconds );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 64, 64 ) );
			}
		}
		// Send stop process signal...
		else 
		{
			// Save quickslot informations...
			{
/*				_XQuickSlotInfoList	quickslotinfolist;
				quickslotinfolist.LoadQuickSlotInfo();
				quickslotinfolist.UpdateInfoList();
				quickslotinfolist.SaveQuickSlotInfo();
*/			}

			//ResetData();//2004.06.19->oneway48 insert

			g_NetworkKernel.DisconnectServer();			
			g_NetworkKernel.InitializeNetwork();

			g_QuestScriptManager.ReleasePrivateData();

			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
			
			g_MessageBox.ShowWindow( FALSE );

			m_ProcessStopTime = 0;
		}
	}
}

//#define _XDEF_INFOHELPERSYSTEMVIEWTIMER			400000
//#define _XDEF_INFOHELPERSTRINGSCROLLTIMER		150
//#define _XDEF_INFOHELPERSTRINGINSERTTIMER	    15000
//#define _XDEF_INFOHELPERSTRINGHIDETIMER			13000

void XProc_ZeroLevel::ProcessInterface( void )
{
	if( g_ViewMainInterface ) g_MainWindowManager.Process();

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
		
		if( m_CameraRotateSpeed > 0.0f )
		{
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();	
			// Rebuild loose quadtree...
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel();			
			
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{
				// Set direction to minimap frustum
				pminimapwindow->SetRotateFrustum( _X_RAD( 180.0f - g_LodTerrain.m_3PCamera.GetYaw() ) );
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

BOOL XProc_ZeroLevel::SelectChargeSkill(short skillid, BOOL keydown)
{
	if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
	{
		if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
		{
			if(g_pLocalUser->m_SelectedSkillID == skillid)
			{
				// cooldown이 종료되지 않음
				return FALSE;
			}
		}

		_XUserSkill* pSelectedSkillItem = g_pLocalUser->GetSkillItem(skillid);
		if(pSelectedSkillItem)
		{
			if(pSelectedSkillItem->m_cSkillLevel >= 0)
			{
				if(CheckDelayTime(skillid))
				{
					if(g_pLocalUser->m_ViewTarget)
						g_pLocalUser->SetAttackTarget(g_pLocalUser->m_ViewTarget, g_pLocalUser->m_ViewTargetType);
					else
					{
						g_NetworkKernel.InsertChatString("선택된 대상이 없습니다.", _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						return FALSE;
					}

					if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
					{
						g_pLocalUser->m_ChargeStartTime = 0;
						g_pLocalUser->m_ChargeIterateValue = 0;
					}
					
					if(keydown)
					{
						if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
						{
							g_pLocalUser->ResetAttackTarget();
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORPEACE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 평화모드에서는 무공을 사용할 수 없습니다.
							return FALSE;
						}
						else
							g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_1;
					}
					
					g_pLocalUser->m_CurrentSelectedSkillType = g_SkillProperty[skillid]->usefulType;
					
					short prevskillid = g_pLocalUser->m_SelectedSkillID;
					g_pLocalUser->m_SelectedSkillID = skillid;
					g_pLocalUser->m_SelectedSkillItem = g_pLocalUser->GetSkillItem(skillid);
//					g_pLocalUser->m_SkillStarLevel 계산
					
					g_pLocalUser->UpdateLocalUserAttackLevel();
					if(prevskillid != g_pLocalUser->m_SelectedSkillID)
						g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
					
					return TRUE;
				}
				else
				{
					g_NetworkKernel.InsertChatString("아직 선택할 수 없습니다. - charge", _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
		}
	}
	
	return FALSE;
}

BOOL XProc_ZeroLevel::SelectActiveSkill(short skillid)
{
	if(skillid == 0)
	{
		if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
		{
			if(CheckDelayTime(skillid))
			{
//				g_pLocalUser->m_bChangeSkill = TRUE;
				g_pLocalUser->m_NextSkillID = skillid;
				return TRUE;
			}
			else
			{
				g_NetworkKernel.InsertChatString("아직 선택할 수 없습니다.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				return FALSE;
			}
		}
	}

	_XUserSkill* pSelectedSkillItem = g_pLocalUser->GetSkillItem(skillid);
	if(pSelectedSkillItem)
	{
		if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
		{
			if(pSelectedSkillItem->m_cSkillLevel >= 0)
			{
				if(CheckDelayTime(skillid))
				{
//					g_pLocalUser->m_bChangeSkill = TRUE;
					g_pLocalUser->m_NextSkillID = skillid;
					return TRUE;
				}
				else
				{
					g_NetworkKernel.InsertChatString("아직 선택할 수 없습니다.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					return FALSE;
				}
			}
		}
	}

	return FALSE;
}

BOOL XProc_ZeroLevel::CheckDelayTime(short skillid)
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
			delaytime = g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->detailInfo[g_pLocalUser->m_SelectedSkillItem->m_cSkillLevel].delay;

		// skillid와 combination check

		if(m_LocalSystemTime - g_pLocalUser->m_ChangeDelayStartTime > delaytime)
		{
			g_pLocalUser->m_ChangeDelayStartTime = 0;
			return TRUE;
		}
	}

	return FALSE;
}

void XProc_ZeroLevel::AddMob()
{
	int appearanceindex = 0;
	
	for( int i = 1; i < m_iTotalMobCount; i++ )
	{
		if( m_pZerolevelCharacterList[i].jobindex == 1 )		// 적대문파 영웅
		{
			_XUser* pUser = new _XUser;
			pUser->InitializeData();
			pUser->m_ModelDescriptor.SetShadowDecalInfo( 0.65f, 0.85f,  1.85f );
			pUser->m_ModelDescriptor.InitEffectManager(1);
			pUser->m_Uniqueid = m_pZerolevelCharacterList[i].id;
			pUser->m_Position.x = m_vEnemyHeroPos.x = m_pZerolevelCharacterList[i].posx;
			pUser->m_Position.y = m_vEnemyHeroPos.y = 0.0f;
			pUser->m_Position.z = m_vEnemyHeroPos.z = m_pZerolevelCharacterList[i].posy;
			pUser->m_TargetPosition.x = m_pZerolevelCharacterList[i].posx;
			pUser->m_TargetPosition.y = 0.0f;
			pUser->m_TargetPosition.z = m_pZerolevelCharacterList[i].posy;			
			
			pUser->m_PrevProcessTime= m_LocalSystemTime;
			pUser->m_PrevMoveTime = 0;
			pUser->m_CharacterInfo.Set_current_lifepower(100);
			pUser->m_CharacterInfo.Set_max_lifepower(100);
			pUser->m_MoveVector = pUser->m_TargetPosition - pUser->m_Position;
			D3DXVec3Normalize(&pUser->m_MoveVector, &pUser->m_MoveVector);
			// OBB check
			_XOBBData* pLastPickedOBB = NULL;
			FLOAT height = g_LodTerrain.GetTerrainAndOBBHeight(pUser->m_Position, pLastPickedOBB);
			pUser->m_Position.y = height;
			pUser->m_pGoundCollideOBB = pLastPickedOBB;
			pUser->m_PrevPosition = pUser->m_Position;
			pUser->m_RotateAngle = m_pZerolevelCharacterList[i].rotangle;
			pUser->UpdatePosition();
			
			pUser->m_ModelDescriptor.LinkBipedObject( &m_ZeroLevelHerobipedModel[m_EnemyGroupType] );			
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_HAIR, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_FACE, NULL ); 
			pUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY, m_pZeroLevelHeroModel[m_EnemyGroupType] );			
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LOWERBODY, NULL );
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LHAND, NULL );
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_RHAND, NULL );				
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LFOOT, NULL );				
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_RFOOT, NULL );				
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_BACKPACK, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );				
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM2, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM3, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM4, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
			
			pUser->m_ModelDescriptor.SetCurMotion( _XACTION_TALK, TRUE );
			pUser->m_ModelDescriptor.PlayAnimation(TRUE);			
			pUser->m_LastRecivedMessageTime = m_LocalSystemTime;			
			
			g_Userlist.AddUser_zero(pUser);			
		}
		else if( m_pZerolevelCharacterList[i].jobindex == 8 )		// 몹으로 생성
		{
			_XZeroLevelMob* pMob = new _XZeroLevelMob;
			pMob->InitializeData();
			appearanceindex = GetAppearanceIndex(i);
			pMob->m_MobID = m_pZerolevelCharacterList[i].id;		
			pMob->m_Position.x = pMob->m_OriginPosition.x = m_pZerolevelCharacterList[i].posx;
			pMob->m_Position.y = pMob->m_OriginPosition.y =0.0f;
			pMob->m_Position.z = pMob->m_OriginPosition.z = m_pZerolevelCharacterList[i].posy;
			
			pMob->m_EndPosition.x = m_pZerolevelCharacterList[i].posx;
			pMob->m_EndPosition.y = 0.0f;
			pMob->m_EndPosition.z = m_pZerolevelCharacterList[i].posy;
			
			pMob->m_HealthPercentage = m_pAppearanceList[appearanceindex].health;
			pMob->m_AttackHitValue = m_pAppearanceList[appearanceindex].attackhitvalue;
			
			pMob->m_Groupindex = m_EnemyGroupType;
			pMob->m_SkillIndex = m_pAppearanceList[appearanceindex].skill;
			pMob->m_HealthPercentage = 100;
			pMob->m_CurrentLife = m_pAppearanceList[appearanceindex].health;
			pMob->m_MaxLife = m_pAppearanceList[appearanceindex].health;			
			
			pMob->m_ModelDescriptor.ReserveModelStack(_XDEF_MAXMODEL_COMBINATIONCOUNT);
			
			int tmpzender = m_pAppearanceList[appearanceindex].gender;
			pMob->m_Gender = tmpzender;
			int extravalue = 0;
			m_pAppearanceList[appearanceindex].face = _XRandom()%8;
			m_pAppearanceList[appearanceindex].hair = _XRandom()%18;
			if( tmpzender == _XGENDER_MAN )
			{
				pMob->m_ModelDescriptor.LinkBipedObject(&g_MaleBipedObject);
				pMob->m_AttackType = g_SkillProperty[m_pAppearanceList[appearanceindex].skill]->motion_male+1;
			}
			else
			{
				extravalue = g_Model_GenderSeparateIndex;
				pMob->m_ModelDescriptor.LinkBipedObject(&g_FemaleBipedObject);
				pMob->m_AttackType = g_SkillProperty[m_pAppearanceList[appearanceindex].skill]->motion_female+1;
			}
			
			if( appearanceindex > 35 && appearanceindex < 48 )		// 소림일 경우
			{
				pMob->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, NULL );
				pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_FACE,
					g_UserModel_FaceType[g_Model_SorimHairSeparateIndex + m_pAppearanceList[appearanceindex].face]);				
			}
			else
			{
				pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_HAIR, 
					g_UserModel_HairType[extravalue+m_pAppearanceList[appearanceindex].hair]);
				pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_FACE, 
					g_UserModel_FaceType[extravalue+m_pAppearanceList[appearanceindex].face]);
			}
			
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_UPPERBODY, 
				g_UserModel_UBodyType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].ubody].sModelIndex]);
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LOWERBODY, 
				g_UserModel_LBodyType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].lbody].sModelIndex]);				
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LHAND,
				g_UserModel_LHandType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].lhand].sModelIndex]);					
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_RHAND, 
				g_UserModel_RHandType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].rhand].sModelIndex]);
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LFOOT, 
				g_UserModel_LFootType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].lfoot].sModelIndex]);							
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_RFOOT, 
				g_UserModel_RFootType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].rfoot].sModelIndex]);
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_BACKPACK, NULL);			
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
				g_ItemModel_Weapon[( tmpzender == _XGENDER_MAN ) ? 0 : 1][g_WeaponItemProperty[m_pAppearanceList[appearanceindex].weapon].sModelIndex] );
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM2, NULL);
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM3, NULL);
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM4, NULL);
			pMob->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);

			pMob->m_ItemNo = m_pAppearanceList[appearanceindex].weapon;
						
			pMob->ResetAnimationState();
			
			pMob->m_ModelDescriptor.InitEffectManager( 1 );

			pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
			pMob->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			pMob->SetCharacterMobAnimation(_XMOBACTION_IDLE);			
			

			if( pMob->m_MaxLife > 2000 )
				pMob->m_fMoveSpeed = _XDEF_CHARACTERMOVESPEED_RUN - 0.00051f;
			else if( pMob->m_MaxLife > 1000 )
				pMob->m_fMoveSpeed = _XDEF_CHARACTERMOVESPEED_RUN - 0.0007f;
			else
				pMob->m_fMoveSpeed = _XDEF_CHARACTERMOVESPEED_RUN - 0.001f;

			pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
			D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
			pMob->m_RotateAngle = m_pZerolevelCharacterList[i].rotangle;
			pMob->m_OldRotateAngle = m_pZerolevelCharacterList[i].rotangle;
			pMob->UpdatePosition();
			
			pMob->m_LastRecivedMessageTime = m_LocalSystemTime;
			pMob->m_PrevTime = m_LocalSystemTime;			
			g_Moblist.AddMob_zero(pMob);
		}			
		else 	// 유저로 생성 
		{
			_XUser* pUser = new _XUser;
			pUser->InitializeData();
			appearanceindex = GetAppearanceIndex(i);
			pUser->m_ModelDescriptor.InitEffectManager(1);
			pUser->m_ModelDescriptor.SetShadowDecalInfo( 0.65f, 0.85f, 1.85f );
			pUser->m_Uniqueid = m_pZerolevelCharacterList[i].id;
			pUser->m_Position.x = m_pZerolevelCharacterList[i].posx;
			pUser->m_Position.y = 0.0f;
			pUser->m_Position.z = m_pZerolevelCharacterList[i].posy;
			pUser->m_TargetPosition.x = m_pZerolevelCharacterList[i].posx;
			pUser->m_TargetPosition.y = 0.0f;
			pUser->m_TargetPosition.z = m_pZerolevelCharacterList[i].posy;			
			
			pUser->m_PrevProcessTime= m_LocalSystemTime;
			pUser->m_PrevMoveTime = 0;
			pUser->m_CharacterInfo.Set_current_lifepower(100);
			pUser->m_CharacterInfo.Set_max_lifepower(100);
			pUser->m_CharacterInfo.gender = (tagGenderType)m_pAppearanceList[appearanceindex].gender;
			
			pUser->m_LastRotateAngle = m_pZerolevelCharacterList[i].rotangle;
			pUser->m_MoveVector = pUser->m_TargetPosition - pUser->m_Position;
			D3DXVec3Normalize(&pUser->m_MoveVector, &pUser->m_MoveVector);
			// OBB check
			_XOBBData* pLastPickedOBB = NULL;
			FLOAT height = g_LodTerrain.GetTerrainAndOBBHeight(pUser->m_Position, pLastPickedOBB);
			pUser->m_Position.y = height;
			pUser->m_pGoundCollideOBB = pLastPickedOBB;
			pUser->m_PrevPosition = pUser->m_Position;
			pUser->m_RotateAngle = m_pZerolevelCharacterList[i].rotangle;
			pUser->UpdatePosition();
			pUser->m_JobIndexOfZeroLevel = m_pZerolevelCharacterList[i].jobindex;			
			
			int tmpzender = m_pAppearanceList[appearanceindex].gender;			
			int extravalue = 0;
			m_pAppearanceList[appearanceindex].face = _XRandom()%8;
			m_pAppearanceList[appearanceindex].hair = _XRandom()%18;			
			
			if( tmpzender == _XGENDER_MAN )
			{
				pUser->m_ModelDescriptor.LinkBipedObject(&g_MaleBipedObject);
				pUser->m_CharacterInfo.animationdescript.attacktype = g_SkillProperty[m_pAppearanceList[appearanceindex].skill]->motion_male;
			}
			else
			{
				extravalue = g_Model_GenderSeparateIndex;
				pUser->m_ModelDescriptor.LinkBipedObject(&g_FemaleBipedObject);
				pUser->m_CharacterInfo.animationdescript.attacktype = g_SkillProperty[m_pAppearanceList[appearanceindex].skill]->motion_female;
			}
			
			if( appearanceindex > 35 && appearanceindex < 48 )		// 소림일 경우
			{
				pUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, NULL );
				pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_FACE,
					g_UserModel_FaceType[g_Model_SorimHairSeparateIndex + m_pAppearanceList[appearanceindex].face]);				
			}
			else
			{
				pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_HAIR, 
					g_UserModel_HairType[extravalue+m_pAppearanceList[appearanceindex].hair]);
				pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_FACE, 
					g_UserModel_FaceType[extravalue+m_pAppearanceList[appearanceindex].face]);
			}					
			
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_UPPERBODY, 
				g_UserModel_UBodyType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].ubody].sModelIndex]);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LOWERBODY, 
				g_UserModel_LBodyType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].lbody].sModelIndex]);				
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LHAND,
				g_UserModel_LHandType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].lhand].sModelIndex]);					
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_RHAND, 
				g_UserModel_RHandType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].rhand].sModelIndex]);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_LFOOT, 
				g_UserModel_LFootType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].lfoot].sModelIndex]);							
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_RFOOT, 
				g_UserModel_RFootType[extravalue + g_ClothesItemProperty[m_pAppearanceList[appearanceindex].rfoot].sModelIndex]);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_BACKPACK, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
				g_ItemModel_Weapon[( tmpzender == _XGENDER_MAN ) ? 0 : 1][g_WeaponItemProperty[m_pAppearanceList[appearanceindex].weapon].sModelIndex] );
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM2, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM3, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM4, NULL);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
			
			pUser->m_CharacterInfo.modeldescript.item1 = m_pAppearanceList[appearanceindex].weapon;

			if( m_pZerolevelCharacterList[i].jobindex == 6  || m_pZerolevelCharacterList[i].jobindex == 7 )
				pUser->SetAnimation(_XACTION_ATTACK);			
			else
				pUser->SetAnimation(_XACTION_IDLE);
			pUser->m_ModelDescriptor.PlayAnimation(TRUE);
			
			pUser->m_LastRecivedMessageTime = m_LocalSystemTime;	
			
			g_Userlist.AddUser_zero(pUser);
		}
	}		
}

#define _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE
void XProc_ZeroLevel::LoadZeroLevelInfo()
{
	SAFE_DELETE_ARRAY( m_pAppearanceList );	
	SAFE_DELETE_ARRAY( m_pZerolevelCharacterList );

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	FILE* fp;

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	if( !g_ScriptArchive.ReOpenPackage() )
			return;

	if( (fp = g_ScriptArchive.GetPackedFile( _T("zerolevelinfo.txt") ) ) == NULL )
	{
		_XFatalError("Can't load zerolevel data");
		g_ScriptArchive.ClosePackage();
	}
#else
	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	
	if( (fp = fopen( "zerolevelinfo.txt", "rt" )) == NULL )	
	{
		_XFatalError("Can't load zerolevel data");
		fclose(fp);		
	}
#endif	

	int totaltype = 0;
	fscanf(fp, "%d", &totaltype);	

	if( totaltype < 1 ) 
	{
#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
		g_ScriptArchive.ClosePackage();
#else
		fclose(fp);
#endif
	}

	m_pAppearanceList = new _APPEARANCETYPE[totaltype];

	for( int i = 0; i < totaltype; i++ )
	{
		fscanf( fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &m_pAppearanceList[i].gender, &m_pAppearanceList[i].face, 
				&m_pAppearanceList[i].hair, &m_pAppearanceList[i].ubody, &m_pAppearanceList[i].lbody, &m_pAppearanceList[i].hat,
				&m_pAppearanceList[i].lhand, &m_pAppearanceList[i].rhand, &m_pAppearanceList[i].lfoot, &m_pAppearanceList[i].rfoot, 
				&m_pAppearanceList[i].weapon, &m_pAppearanceList[i].skill, &m_pAppearanceList[i].attackhitvalue, &m_pAppearanceList[i].health );
	}	
	
	fscanf( fp, "%d", &m_iTotalMobCount );
	if( m_iTotalMobCount < 1 ) fclose(fp);

	m_pZerolevelCharacterList = new _ZEROLEVEL_CHAR_INFO[m_iTotalMobCount];

	for( i = 0; i < m_iTotalMobCount; i++ )
	{
		fscanf( fp, "%d %f %f %f %d %d", &m_pZerolevelCharacterList[i].id, &m_pZerolevelCharacterList[i].posx, &m_pZerolevelCharacterList[i].posy, 
			&m_pZerolevelCharacterList[i].rotangle, &m_pZerolevelCharacterList[i].jobindex, &m_pZerolevelCharacterList[i].appearanceindex );
	}

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
	g_ScriptArchive.ClosePackage();
#else
	fclose( fp );
#endif
}

void XProc_ZeroLevel::HeroCharacterLoading()
{
	m_Player.SetPitchLimits(-45, 45);
	m_Player.SetRollLimits(-10, 10);
	m_Player.SetType(camFly);
	
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
	
	m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
	m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex( _T("LoadingBG_Zero.jpg"), _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );

	int resourceindex = -1;

	m_ProgressStatic.Create( (gnWidth>>1) - 151, (gnHeight) - 33, (gnWidth>>1) + 150, (gnHeight) - 19, 
							 _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB( 128, 128, 128, 128 ) );
		
	int l = (gnWidth>>1) - 150;
	int u = (gnHeight) - 32;
	int r = (gnWidth>>1) + 150;
	int d = (gnHeight) - 20;

	m_ProgressBarStatic.Create( D3DXVECTOR3(l, u, 0.5f ),
								D3DXVECTOR3(r, u, 0.5f ),
								D3DXVECTOR3(l, d, 0.5f ),
								D3DXVECTOR3(r, d, 0.5f ),
								_XDEF_DEFAULTDEPTH_FORINTERFACE,
								D3DCOLOR_ARGB( 160, 0, 0, 0 ) );
	m_ProgressBarStatic.SetMaxRate( 300.0f );
	
	m_TextureArchive.BuildTextureList();
	m_TextureArchive.InitDeviceObject();	
	
	HeroCharacterLoadingInitDeviceObject();

	SetMessage( "Loading...." );
		
	m_DataLoaded = FALSE;
	m_Progress   = 0;
	
	g_MessageBox.ShowWindow( FALSE );	

	HeroCharacterLoadingProcess();
	// 끝나면 바로 날려준다.
	HeroCharacterLoadingDestroyProcess();
}

void XProc_ZeroLevel::HeroCharacterLoadingInitDeviceObject()
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
//	if( gfmtDepth == D3DFMT_D16 ) gpDev->SetRenderState( D3DRS_DITHERENABLE, TRUE );
//	else gpDev->SetRenderState( D3DRS_DITHERENABLE,   FALSE ); 
	
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
}

void XProc_ZeroLevel::HeroCharacterLoadingDestroyProcess()
{
	m_TextureArchive.DisposeTexture();
	m_BackgroundStatic.ReleaseObject();	
	m_ProgressStatic.ReleaseObject();	

	m_DataLoaded = FALSE;
	
	/*if( g_BgmPlayFlag )
	{
		if( g_BgmObject.Getm_dwDeviceID() )	g_BgmObject.Close();		
	}*/

	SetEnableProcess( FALSE );
}

bool XProc_ZeroLevel::HeroCharacterLoadingProcess( void )
{
	if( m_DataLoaded ) return true;

	g_MessageBox.Process();

	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_INITDATA) );
	UpdateProgress( 0 );	

	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_CREATETERRAIN) );
	UpdateProgress( 6 );	

	// Load Texture Package ===================================================
	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_ENVIRONMENT.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_ENVIRONMENT.XP") );
#endif

	g_TerrainTextureArchive.SetTexturePackage( packagefilename );

	//g_WorldObjectTextureArchive.m_TextureArchive.EnableVirtualMemory(TRUE);
	g_WorldObjectTextureArchive.SetTexturePackage( packagefilename );
	
	g_TerrainTextureArchive.DisposeTexture();
	g_WorldObjectTextureArchive.DisposeTexture();
	g_LodTerrain.ReleaseTerrainTileInfo();
	
	g_XST_Wrapper.Release();
	
	InitializeLight();
	g_CurrentZoneInfoIndex = 1;	

	// Load Terrain data ======================================================	

	if( !LoadTerrain( _T("map_namkyoung.xtv") ) ) return false; //hotblood
	
	g_LodTerrain.m_GenerateRealTimeObject = TRUE;

	_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( pminimapwindow )
	{
		pminimapwindow->SetMinimapImage( _T("map_namkyoung.xtv") );
		pminimapwindow->SetCurrentZoneIndex( g_CurrentZoneInfoIndex );
		pminimapwindow->SetMinimapRate();
	}

	_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
	if( pmaxminimapwindow )
	{
		pmaxminimapwindow->SetMinimapImage( _T("map_namkyoung.xtv")  );
		pmaxminimapwindow->SetCurrentZoneIndex( g_CurrentZoneInfoIndex );
	}
	
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_OBJECT) );
	UpdateProgress( 40 );	
	
	if( !g_ModelArchive.ReOpenPackage() ) 
		return false;

	if( !g_AniArchive.ReOpenPackage() ) 
		return false;
	
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_CHARACTER.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_CHARACTER.XP") );
#endif

	if( !m_HeroCharacterTextureArchive.SetTexturePackage( packagefilename ) ) 
		return FALSE;	

	// 추후 영웅 추가될시 변경 (현재 3개)----------------┓	
	TCHAR heromodelfilename[9][32] = 
	{
		_T(""),	
		_T("HeroHo_mesh.X3D"),
		_T("Herosu_mesh.X3D"),
		_T("Herobull_mesh.X3D"),
		_T("HeroNok_mesh.X3D"),
		_T(""),
		_T(""),
		_T(""),
		_T("")		
	};

	TCHAR herodescriptfilename[9][32] = 
	{
		_T(""),
		_T("HeroHoModel.xms"),
		_T("HeroSuModel.xms"),
		_T("HeroBullModel.xms"),
		_T("HeroNokModel.xms"),
		_T(""),
		_T(""),
		_T(""),
		_T("")		
	};


	for ( int i = 0; i < 9; i++ )
	{
		m_pZeroLevelHeroModel[i] = NULL;
	}
	
	// 아군 영웅 
	m_pZeroLevelHeroModel[m_HeroGroupType] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	if( !m_pZeroLevelHeroModel[m_HeroGroupType]->Load( g_ModelArchive.GetPackedFile( heromodelfilename[m_HeroGroupType] ), 
		&m_HeroCharacterTextureArchive, 
		!_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, heromodelfilename[m_HeroGroupType]) )
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}
	
	if( !LoadHeroDescript( herodescriptfilename[m_HeroGroupType], m_HeroGroupType ) ) 
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}

	// 적군 영웅
	m_pZeroLevelHeroModel[m_EnemyGroupType] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	if( !m_pZeroLevelHeroModel[m_EnemyGroupType]->Load( g_ModelArchive.GetPackedFile( heromodelfilename[m_EnemyGroupType] ), 
		&m_HeroCharacterTextureArchive, 
		!_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, heromodelfilename[m_EnemyGroupType]) )
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}
	
	if( !LoadHeroDescript( herodescriptfilename[m_EnemyGroupType], m_EnemyGroupType ) ) 
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}

	//-------┛	
	
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

	_XMEMORYUSECHECKREADY
	_XMEMORYUSECHECKSTART
	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );

#ifdef _XTESTSERVER	
	m_HeroCharacterTextureArchive.ReadyRenderStyle(_T("tsCharacter.xrs"));
	if( !m_HeroCharacterTextureArchive.BuildTextureList() )
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}		
	m_HeroCharacterTextureArchive.AttachRenderStyle( _T("tsCharacter.xrs") );
	if( !m_HeroCharacterTextureArchive.InitDeviceObject() )
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}
#else
	m_HeroCharacterTextureArchive.ReadyRenderStyle(_T("Character.xrs"));
	if( !m_HeroCharacterTextureArchive.BuildTextureList() )
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}		
	m_HeroCharacterTextureArchive.AttachRenderStyle( _T("Character.xrs") );
	if( !m_HeroCharacterTextureArchive.InitDeviceObject() )
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}
#endif
			
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();
			
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_CACHINGTERRAIN) );
	UpdateProgress( 55 );	

	// Loaded texure initialize=================================================================================================================	
	switch( g_XGQProperty.terrainqualitylevel ) 
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
	if( !g_TerrainTextureArchive.ReadyRenderStyle( _T("tsEnvironment.xrs") ) ) return false;	

	//_XTEXTUREPRECACHEING_ON
	if( !g_TerrainTextureArchive.BuildTextureList() ) return false;		
	//_XTEXTUREPRECACHEING_OFF

	if( !g_TerrainTextureArchive.AttachRenderStyle( _T("tsEnvironment.xrs") ) ) return false;	
#else
	if( !g_TerrainTextureArchive.ReadyRenderStyle( _T("Environment.xrs") ) ) return false;	
	
	//_XTEXTUREPRECACHEING_ON
	if( !g_TerrainTextureArchive.BuildTextureList() ) return false;		
	//_XTEXTUREPRECACHEING_OFF
	
	if( !g_TerrainTextureArchive.AttachRenderStyle( _T("Environment.xrs") ) ) return false;	
#endif
	
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_INITTERRAIN) );
	UpdateProgress( 75 );	

	if( !g_TerrainTextureArchive.InitDeviceObject() )
	{
		g_TextureQuality		= 1;
		return false;	
	}
	
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
	
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_CACHINGOBJECT) );
	UpdateProgress( 85 );	

	_XMEMORYUSECHECKSTART

#ifdef _XTESTSERVER
	if( !g_WorldObjectTextureArchive.ReadyRenderStyle( _T("tsObject.xrs") ) ) return false;	

	_XTEXTUREPRECACHEING_ON
	if( !g_WorldObjectTextureArchive.BuildTextureList() )
	{
		g_TextureQuality		= 1;
		return false;	
	}
	_XTEXTUREPRECACHEING_OFF

	if( !g_WorldObjectTextureArchive.AttachRenderStyle( _T("tsObject.xrs") ) ) return false;
#else
	if( !g_WorldObjectTextureArchive.ReadyRenderStyle( _T("Object.xrs") ) ) return false;	
	
	_XTEXTUREPRECACHEING_ON
	if( !g_WorldObjectTextureArchive.BuildTextureList() )
	{
		g_TextureQuality		= 1;
		return false;	
	}
	_XTEXTUREPRECACHEING_OFF
			
	if( !g_WorldObjectTextureArchive.AttachRenderStyle( _T("Object.xrs") ) ) return false;	
#endif

	_XMEMORYUSECHECKEND( "World object texture memory" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_INITOBJECT) );
	UpdateProgress( 95 );
	
	if( !g_WorldObjectTextureArchive.InitDeviceObject() )
	{
		g_TextureQuality		= 1;
		return false;	
	}

	g_TextureQuality		= 1;
	
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_ENDINIT) );
	UpdateProgress( 100 );
	
	//_XMessage( "End of data loading" );
	return true;
}

void XProc_ZeroLevel::HeroCharacterDraw( void )
{
	gpDev->Clear(0, NULL,
				 D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	// Begin the scene
	gpDev->BeginScene();

	m_Player.UpdateViewMatrix();
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());

	m_BackgroundStatic.Render();
	
	m_ProgressStatic.RenderSolid();
	
	m_ProgressBarStatic.RenderSolid();	
	
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->PrintAlign( messagecoord.x, messagecoord.y,1.0f, _XFONT_ALIGNTYPE_CENTER, m_Message );
	g_XBaseFont->Flush();
				
	g_MessageBox.Draw();
	// End the scene
	gpDev->EndScene();
			
	// Present the backbuffer contents to the display
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;
}

int XProc_ZeroLevel::GetAppearanceIndex(int index)
{	
	int result = 0;
	int jobindex = m_pZerolevelCharacterList[index].jobindex;
	if( jobindex == 8 || jobindex == 7 || jobindex == 5 || jobindex == 3  )	// MOB, 적군전투NPC, 적군NPC, 적군호법 
	{
		result = m_pZerolevelCharacterList[index].appearanceindex + 12 * m_EnemyGroupType;

		if( m_EnemyGroupType == _XGROUP_SORIM && m_pZerolevelCharacterList[index].appearanceindex > 5 )	// 소림일 경우
		{
			result -= 6;			
		}
		else if( m_EnemyGroupType == _XGROUP_BEEGOONG && m_pZerolevelCharacterList[index].appearanceindex < 6 )	// 비궁일 경우
		{
			result += 6;			
		}
	}
	else	// 아군 캐릭터들
	{
		result = m_pZerolevelCharacterList[index].appearanceindex + 12 * m_HeroGroupType;

		if( m_HeroGroupType == _XGROUP_SORIM && m_pZerolevelCharacterList[index].appearanceindex > 5 )	// 소림일 경우
		{
			result -= 6;			
		}
		else if( m_HeroGroupType == _XGROUP_BEEGOONG && m_pZerolevelCharacterList[index].appearanceindex < 6 )	// 비궁일 경우
		{
			result += 6;			
		}
	}		

	return result;
}

void XProc_ZeroLevel::HeroStateSetting()
{
	// 무공 셋팅
	if( g_pQuickSlot_Window )
	{
		int skill1, skill2;
		switch(m_HeroGroupType) 
		{
		case _XGROUP_NANGIN :
			break;
		case _XGROUP_GAEBANG :
			skill1 = 126;
			skill2 = 127;
			break;
		case _XGROUP_BEEGOONG :
			skill1 = 218;
			skill2 = 219;
			break;
		case _XGROUP_SORIM:
			skill1 = 344;
			skill2 = 328;
			break;
		case _XGROUP_NOCKRIM :
			skill1 = 416;
			skill2 = 5;		// zeroskill
			break;
		case _XGROUP_MOODANG :
			break;
		case _XGROUP_MAKYO :
			break;
		case _XGROUP_SEGA :
			break;
		case _XGROUP_NOFIXED :
			break;
		}		
		
		g_pLocalUser->m_SkillList[0].m_sSkillID = skill1;
		g_pLocalUser->m_SkillList[0].m_cSkillLevel = 11;
		g_pLocalUser->m_SkillList[1].m_sSkillID = skill2;
		g_pLocalUser->m_SkillList[1].m_cSkillLevel = 11;
		
		g_pLocalUser->m_ActiveSlotID = skill1;
		g_pLocalUser->m_CurrentSelectedSkillType = _XSI_USEFULTYPE_ACTIVESKILL;
		g_pLocalUser->m_SelectedSkillID = skill1;
		g_pLocalUser->m_SelectedSkillItem = g_pLocalUser->GetSkillItem(skill1);
		
		// 유저의 기본동작을 설정			
		g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect		= TRUE;
		g_pLocalUser->m_CharacterInfo.animationdescript.idletype	= _XBATTLE_BATTLE;
		g_pLocalUser->m_CharacterInfo.animationdescript.motionclass = _XACTION_TALK;
		g_pLocalUser->m_CharacterInfo.animationdescript.attacktype  = _XHAN_ATTACK1;
		g_pLocalUser->m_CharacterInfo.animationdescript.movetype    = _XMOVETYPE_RUN;
		
		g_pLocalUser->m_BattleMode = _XBATTLE_BATTLE;
		g_pLocalUser->m_StanceMotionFirst = FALSE;
		g_pLocalUser->m_CurrentSelectedSkillType = _XSI_USEFULTYPE_ACTIVESKILL;
		g_pLocalUser->m_AttackRange = 1.6f;		
		g_pLocalUser->m_CharacterInfo.groupindex  = m_HeroGroupType;		

		TCHAR heroname[9][12] = {
			"군유명",
			"호산산",
			"수라마후",			
			"불륜화상",
			"녹림왕",
			"능운도장",
			"천마대제",
			"남궁휘",			
			"뇌제"
		};
		strcpy(g_pLocalUser->m_CharacterInfo.charactername, heroname[m_HeroGroupType]);
		
		g_pLocalUser->SetSkillAnimation();	
		
		g_pQuickSlot_Window->m_QuickSlot[0].used = TRUE;
		g_pQuickSlot_Window->m_QuickSlot[0].skillid = skill1;
		g_pQuickSlot_Window->m_QuickSlot[0].type = _XQS_SLOTTYPE_SKILL;
		
		g_pQuickSlot_Window->m_QuickSlot[1].used = TRUE;
		g_pQuickSlot_Window->m_QuickSlot[1].skillid = skill2;
		g_pQuickSlot_Window->m_QuickSlot[1].type = _XQS_SLOTTYPE_SKILL;
		
		if(g_pSkill_Window)
		{
			g_pSkill_Window->SetListItem();
		}
	}	
}

void XProc_ZeroLevel::TextSpeechHero()
{
	// MAXSPEECHCOUNT에 맞게 자르기 위해 MAXSPEECHCOUNT-1, MAXSPEECHCOUNT 를 비워 둬야 한다.
	TCHAR* speech_gaebang[6] = {		_T("오랜만이군요. 녹림왕."),
										_T("왜 이런 싸움을 해야 하지요?"),
										_T("모든 일을 이해하는 어른이 되면 이런 살육을 저질러도    되는 건가요?"),
										_T("나는 당신보다 오래 살지 않았지만, 당신처럼 막무가내로  행동하지 않아요. 썩 이곳에서 물러서세요."),
										_T("말로 설득 안 된다면 다른 방법을 써야겠지요."),
										_T("필요하다면!")	};

	TCHAR* enemyspeech_gaebang[6] ={	_T("귀찮게 되었군."),
										_T("여기는 너 같은 어린애가 올 곳이 아니다.                집으로 돌아가거라."),
										_T("세상에는 너 같은 어린 아이가 이해할 수 없는 일이 있는  법이다."),
										_T("흐흐, 지금 나에게 충고하는 거냐?"),
										_T("흐흐, 나는 말로 설득되는 사람이 아니다."),
										_T("하하! 그 얇은 팔로 내 목이라도 꺾을 생각이냐?")		};

	TCHAR* speech_beegoong[4] = {		_T("그렇군요. 당신과는 별로 친하고 싶지 않은데 매우 유감이 에요."),
										_T("호오! 소림에 나를 데려가 어디에 쓰려고요?              설마 밥하고 빨래를 시키려고 가자는 건 아니겠죠?"),
										_T("흥, 백도의 논리란 언제나 자기들 편한 대로군.           어디 능력이 되면 데려가 보시지!"),
										_T("자, 간다!")		};

	TCHAR* enemyspeech_beegoong[5] = {	_T("수라마후 등장인가? 일이 꼬이는군."),
										_T("요즘 얼굴을 자주 보게 되는구려."),
										_T("껄껄, 그런 섭한 말씀 마시오. 나는 마후와 매우 친해지고 싶다오. 그래서 말인데……"),
										_T("이 참에 마후를 소림에 데려가야겠소.	                   자, 나와 함께 소림으로 갑시다."),
										_T("그럴리가 있겠소. 마후는 그저 참회동에 들어가 조용히 앉 아 있기만 하면 된다오.")		};

	TCHAR* speech_solim[6] = {			_T("이곳은 마후처럼 아름다운 여인과 안 어울리는데,         왜 항상 이런 곳에서 보게 되는지 모르겠소."),
										_T("껄껄! 가끔 열심히 수양을 해 부처가 되는 개도 있다오."),
										_T("긴 말 할 필요 뭐 있소? 나와 함께 소림으로 갑시다."),
										_T("개도 부처가 될 수 있는데 마후께서 부처가 되지 말란     법은 어디 있겠소?"),
										_T("소림의 고승들께 가르침을 받는다면 마후께서도 틀림 없이  부처가 될 수 있을 것이오."),
										_T("가고 싶지 않아도 가야할거요.")		};

	TCHAR* enemyspeech_solim[5] = {		_T("드디어 나타났군. 불륜화상……"),
										_T("개는 부처가 될 수 없다고 하던데. 어떻게 당신이 소림에  있는 지 알 수 없군요."),
										_T("호호! 개는 개다워야 하는 법이에요."),
										_T("호오! 이 가녀린 여인을 소림에 데려가 어쩌시려고?"),
										_T("가고 싶지 않다면?")		};

	TCHAR* speech_nocklim[8] = {		_T("여기는 너 같은 어린애가 있을 곳이 아니다.              그만 집으로 돌아가라."),
										_T("받아갈 것? 사탕이라도 하나 주랴?                       그러면 집으로 돌아갈테냐?"),
										_T("약속? 무슨 약속?"),
										_T("허허, 기가 막히는군.                                   도대체 누가 누구를 침범했다는거야?"),
										_T("흥! 아직 나라는 인간을 모르나 보군.                    나는 말로 설득되는 그런 녹녹한 인간이 아니야."),
										_T("그 얇은 팔로 내 목이라도 꺾을 생각이냐?"),
										_T("용기는 가상하다만 아직 너는 나의 상대가 아니다."),
										_T("어디 네 팔로 이 목을 꺾을 수 있는지 시험해 보려무나.")	};

	TCHAR* enemyspeech_nocklim[7] = {	_T("녹림왕……"),
										_T("집으로 가기 전에 당신에게 받아가야 할 것이 하나있어요."),
										_T("아니요.                                                내가 당신에게 받아야 할 것은 두 가지 약속이에요."),
										_T("당장 당신의 부하들과 함께 이곳을 떠나겠다는 약속.      그리고 다시는 개방의 영역을 침범하지 않겠다는 약속."),
										_T("좋은 말로 할 때 그 두 가지 약속을 하고 이곳을 떠나세요."),
										_T("말로 설득이 안 된다면 다른 방법을 써야겠지요."),
										_T("필요하다면.")		};

	int i = 0;
	int stringlength = 0;
	switch(m_HeroGroupType) 
	{
	case _XGROUP_NANGIN:		
		break;
	case _XGROUP_GAEBANG:
		for( i = 0; i < 6; i++ )
		{
			stringlength= strlen(speech_gaebang[i]);
			
			SAFE_DELETE_ARRAY( m_HeroSpeech[i] );
			m_HeroSpeech[i] = new TCHAR[ stringlength + 1 ];

			memset( m_HeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_HeroSpeech[i], speech_gaebang[i] );

			stringlength = strlen(enemyspeech_gaebang[i]);
			
			SAFE_DELETE_ARRAY( m_EnemyHeroSpeech[i] );
			m_EnemyHeroSpeech[i] = new TCHAR[ stringlength + 1 ];
			
			memset( m_EnemyHeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_EnemyHeroSpeech[i], enemyspeech_gaebang[i] );
		} 	
		stringlength = strlen(_T("101010101032") );
		m_CurrentTalkStageArray = new TCHAR[stringlength+1];
		memset(m_CurrentTalkStageArray, 0, sizeof(TCHAR)*(stringlength+1) );
		strcpy(m_CurrentTalkStageArray, _T("101010101032") );
		break;	
	case _XGROUP_BEEGOONG:
		for( i = 0; i < 4; i++ )
		{
			stringlength = strlen(speech_beegoong[i]);
			SAFE_DELETE_ARRAY( m_HeroSpeech[i] );
			m_HeroSpeech[i] = new TCHAR[ stringlength + 1 ];
			memset( m_HeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_HeroSpeech[i], speech_beegoong[i] );
		}
		for( i = 0; i < 5; i ++ )
		{

			stringlength = strlen(enemyspeech_beegoong[i]);
			SAFE_DELETE_ARRAY( m_EnemyHeroSpeech[i] );
			m_EnemyHeroSpeech[i] = new TCHAR[ stringlength + 1 ];
			
			memset( m_EnemyHeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_EnemyHeroSpeech[i], enemyspeech_beegoong[i] );
		}
		stringlength = strlen(_T("110110302") );
		m_CurrentTalkStageArray = new TCHAR[stringlength+1];
		memset(m_CurrentTalkStageArray, 0, sizeof(TCHAR)*(stringlength+1) );
		strcpy(m_CurrentTalkStageArray, _T("110110302") );
		break;	
	case _XGROUP_SORIM:
		for( i = 0; i < 6; i++ )
		{
			stringlength = strlen(speech_solim[i]);
			SAFE_DELETE_ARRAY( m_HeroSpeech[i] );
			m_HeroSpeech[i] = new TCHAR[ stringlength + 1 ];
			memset( m_HeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_HeroSpeech[i], speech_solim[i] );
		}
		for( i = 0; i < 5; i++ )
		{
			stringlength = strlen(enemyspeech_solim[i]);
			SAFE_DELETE_ARRAY( m_EnemyHeroSpeech[i] );
			m_EnemyHeroSpeech[i] = new TCHAR[ stringlength + 1 ];			
			memset( m_EnemyHeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_EnemyHeroSpeech[i], enemyspeech_solim[i] );
		}	
		stringlength = strlen(_T("10101010032") );
		m_CurrentTalkStageArray = new TCHAR[stringlength+1];
		memset(m_CurrentTalkStageArray, 0, sizeof(TCHAR)*(stringlength+1) );
		strcpy(m_CurrentTalkStageArray, _T("10101010032") );
		break;	
	case _XGROUP_NOCKRIM:
		for( i = 0; i < 8; i++ )
		{
			stringlength = strlen(speech_nocklim[i]);			
			SAFE_DELETE_ARRAY( m_HeroSpeech[i] );
			m_HeroSpeech[i] = new TCHAR[ stringlength + 1 ];			
			memset( m_HeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_HeroSpeech[i], speech_nocklim[i] );			
		}
		
		for( i = 0; i < 7; i++ )
		{
			stringlength= strlen(enemyspeech_nocklim[i]);			
			SAFE_DELETE_ARRAY( m_EnemyHeroSpeech[i] );
			m_EnemyHeroSpeech[i] = new TCHAR[ stringlength + 1 ];
			memset( m_EnemyHeroSpeech[i], 0, sizeof(TCHAR) * (stringlength+1) );
			strcpy( m_EnemyHeroSpeech[i], enemyspeech_nocklim[i] );
		}
		stringlength = strlen(_T("101010101010302") );
		m_CurrentTalkStageArray = new TCHAR[stringlength+1];
		memset(m_CurrentTalkStageArray, 0, sizeof(TCHAR)*(stringlength+1) );
		strcpy(m_CurrentTalkStageArray, _T("101010101010302") );
		break;	
	case _XGROUP_MOODANG:
		break;	
	case _XGROUP_MAKYO:
		break;	
	case _XGROUP_SEGA:
		break;	
	case _XGROUP_NOFIXED:
		break;		
	}
}

void XProc_ZeroLevel::TutorialMode()
{
	_XWindow_Help* pHelp_Window = (_XWindow_Help*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_HELP );
	pHelp_Window->ShowWindow( TRUE );	
	pHelp_Window->SetTutorialState(m_iCurrentTutorial);
	pHelp_Window->SetMiniGameState( MT_TUTORIAL );	
}