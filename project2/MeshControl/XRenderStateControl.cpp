// XRenderStateControl.cpp: implementation of the _XRenderStateControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windowsx.h>
#include "commctrl.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XRenderStateControl.h"
#include "XWindow_Debug.h"
#include "SoundEffectList.h"
#include "Xwindow_TrainingMed.h"
#include "XWindow_PaymentWindow.h"
#include "XWindow_BloodPoint.h"
#include "XMessageWindow.h"
#include "XWindow_AccountItemUse.h"
#include "XWindow_TutorialIcon.h"
#include "XSR_STRINGHEADER.H"

#include "LoadThread.h"
#include "XTextureController.h"
// win32 web browser api
#include "./Library/CWebPage/XWebPage.h"

#include "XNetwork.h"
#include "XWindowObjectDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL	g_DrawGraphLog = FALSE;

#ifdef _XDWDEBUG	
BOOL	g_DrawAboveWaterObject = FALSE;
BOOL	g_DrawReflectionMap = FALSE;
BOOL	g_AllMonsterSpawnMode = FALSE;
int		g_CurrentSpawnMonsterType = 0;
DWORD	g_LastMobSpawnTime = 0;
BOOL	g_ShowUserStatus = FALSE;
BOOL	g_DrawEnvSoundObject = FALSE;
int		g_EnvSoundObjectViewFilter = -1;
BOOL	g_3DSoundTargettype = TRUE; // Camera
#endif

extern LoadTextureThread g_LoadTextureThread;

void ReportUsingSystemResource( void )
{
	_XPushLogFile();
	
	_XSetLogFile("resourcereport.log");
	
	_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( pminimapwindow )
	{
		_XLog( "---[ Resource using report ] [%s]-------------------------", g_ZoneInfoTable[pminimapwindow->m_CurrentServerIndex-1].zoneshortname );
	}
	else
	{
		_XLog( "---[ Resource using report ] [Unknown]--------------------" );
	}
	
	_XLog( "= Available Texture mem : %f MByte [ %d Bytes]", gpDev->GetAvailableTextureMem() / 1048576.0f, gpDev->GetAvailableTextureMem() );
	g_TerrainTextureArchive.ReportTextureInfo();
	g_WorldObjectTextureArchive.ReportTextureInfo();
	g_RTimeObjectTextureArchive.ReportTextureInfo();
	g_EffectTextureArchive.ReportTextureInfo();
	g_MainInterfaceTextureArchive.ReportTextureInfo();	
	g_SpeedTreeTextureArchive.ReportTextureInfo();
	g_CharacterTextureArchive.ReportTextureInfo();
	g_MobTextureArchive.ReportTextureInfo();
	_XLog( "---[ end of reporting ]-----------------------------------", 1, 2 );	

	g_LodTerrain.ReportUseResource();

	_XPopLogFile();
}

_XRenderStateControl::_XRenderStateControl()
{

}

_XRenderStateControl::~_XRenderStateControl()
{

}


void _XRenderStateControl::SetRenderState( D3DRENDERSTATETYPE statetype, DWORD value1, DWORD value2, DWORD value3 )
{
	switch( statetype )
	{
	case D3DRS_FILLMODE :
		fillmode	=	value1;
		gpDev->SetRenderState( statetype, fillmode );
		break;
	case D3DRS_FOGENABLE :
		fogenable	=   (BOOL)value1;
		gpDev->SetRenderState( statetype, fogenable );
		g_EnvironmentManager.m_Fog = fogenable;
		break;			
	}	

}

#define _XDEF_INVALIDCOMMAND	0
#define _XDEF_VALIDCOMMAND		1
#define _XDEF_NOTREADYCOMMAND	2
#define _XDEF_NODISPLAYCOMMAND	3

#define _XDEF_COMMADCOUNT		129

#ifdef _XDWDEBUG	
extern BOOL g_SpiderMode;
extern void _XInsertConsoleWindowCommandHistory( LPSTR commandstring );
#endif

// 하이슈퍼 울트라급 노가다 파져
void _XRenderStateControl::ProcCommand( LPSTR commandstring )
{
	if( !commandstring || strlen(commandstring) <= 0 ) return;

	TCHAR ParseCommandString[10][64];
	memset( ParseCommandString, 0, sizeof(TCHAR) * 10 * 64 );

	int checkvalidcommand = _XDEF_INVALIDCOMMAND;

	static TCHAR commandlist[_XDEF_COMMADCOUNT][70] = 
	{
		"[Set command]",
		"fillmode [point, wire, solid<X>]",
		"fog [toggle]",
		"coord [curuser XCoord ZCoord ]",
		"gototo [character name]",
		"gototomob [mob id]",
		"gotonpc [npc name]",
		"gotoarea [areaindex, areacategory]",
		"user [username XCoord ZCoord ] / not ready",
		"command [command] / not ready",
		"userculling",
		"cullingfarplane",
		"env [ dawn, day, nightfall, night, time ]",		
		"fov [fov angle]",
		"nplane (near plane)",
		"fplane (far plane)",
		"lod [add set]",
		"rtimeobject (Realtime generate object)",
		"spidermode",
		"shadow",
		"clearquest",
		"usetexture",
		"reloadrenderstyle",
		"reloadmobdescript",
		"resetmonsterpos",
		"monsterspawn",		
		"language [korean chinese japan]",
		"rotatechar",
		"listenerdistancefactor, ldf",
		"addskill",
		"deleteskill",
		"setskilllevel",
		"addallskill",
		"resetskill",
		"settrainingskill",
		"effect <i(insert),d(delete)> <c(character),w(world)> templateid",
		"startmed",
		"setfury",
		"fury",
		"setclass",
		"setmasterypoint",
		"startinvi",
		"endinvi",
		"lock",
		"exit",
		"vt",
		"viewmastery",
		"setbloodpoint"
		"reloadstring",
		"changelanguage or cl [ kr, vn, tw, eng, cn, jp ]",
		"setlevel",
		"deletetexture",
		"setlife"
		"setforce"
		"setgroup"
		"setmoney"
		"----------------",
		"[View command]",
		"coord [curuser]",
		"command / view command list",
		"scrinfo [toggle]",
		"fps [toggle]",
		"axis [toggle]",
		"object [toggle]",
		"frustum [toggle]",
		"all",
		"lens",
		"userlist",
		"moblist",
		"obb",
		"shadowtexture",
		"lightmodel",
		"movetargetpos",
		"mouseray",
		"terrainrendertime",
		"normal",
		"pathobject",		
		"windstrength up/down",
		"speedtreelod near far",
		"useshader",
		"abovewaterobject",
		"reflectionmap",
		"viewgraph",
		"resreport",
		"pathfindinfo",
		"usesplatindex",
		"viewlayer",
		"quad",
		"mouselock",
		"bloomfilter",
		"bfpower",
		"camerashake"
		"viewlog",
		"battledetail"
		"radialblur [ step, uvdelta ]",
		"motionblur [ intensity ]",
		"userpacketlog",
		"questflash",
		"poisonfog",
		"billinfo",
		"userstatus",
		"setbillinfo [who where method etc]",
		"envsound",
		"3dsound [user camera]",
		"clientmob",
		"tutorial",				//100
		"changewebpageaddress",
		"viewdamagevalue",
		"deleterubiconad",
	};

	sscanf( (LPCTSTR)commandstring, "%s %s %s %s %s %s %s %s %s %s",
			&ParseCommandString[1], &ParseCommandString[2], &ParseCommandString[3],
			&ParseCommandString[4], &ParseCommandString[5], &ParseCommandString[6], 
			&ParseCommandString[7], &ParseCommandString[8], &ParseCommandString[9],
			&ParseCommandString[0] );

	if( strcmpi( ParseCommandString[1], "camerashake" ) == 0 )
	{
		if( strlen(ParseCommandString[2]) > 0 )
		{
			FLOAT timer = atof(ParseCommandString[2]);
			g_LodTerrain.m_3PCamera.SetCameraShakeMode( !g_LodTerrain.m_3PCamera.GetCameraShakeMode(), timer , TRUE);
		}
		else
		{
			g_LodTerrain.m_3PCamera.SetCameraShakeMode( !g_LodTerrain.m_3PCamera.GetCameraShakeMode() );
		}

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}	
	else if( strcmpi( ParseCommandString[1], "delad" ) == 0 )
	{
		#ifdef _ACCLAIM_RUBICONADSYSTEM
			g_ViewRubiconAD = !g_ViewRubiconAD;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetUIforRubiconADSystem();

			/*
			MSG_AIUS_GRADE_SET aiusset;
			aiusset.curGrade.cItemType = 1;
			aiusset.curGrade.sItemID = 1011;
			aiusset.curGrade.uiYear = 8;
			aiusset.curGrade.uiMonth = 9;
			aiusset.curGrade.uiDay = 10;
			aiusset.curGrade.uiHour = 11;
			aiusset.curGrade.uiMinute = 12;
			aiusset.curGrade.uiSecond = 13;
			g_NetworkKernel.MSGAiusGradeSet(&aiusset);
			*/
			
			/*
			if(g_ViewRubiconAD)
			{
				#include "EmperorOfDragons.h"
				#include "EODEXTDEF_GlobalObject.h"
				#include "XProc_ProcessDefine.h"

				#include "XWindow_PK.h"
				_XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
				pPK_Window->ShowWindow(TRUE);
			}
			*/
		#endif

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
#ifdef _XDWDEBUG	
	else if( strcmpi( ParseCommandString[1], "thread" ) == 0 )
	{
		if( strcmpi( ParseCommandString[1], "off" ) == 0 )
		{
			g_LoadTextureThread.DeleteThread();
			g_LoadTextureThread.m_bEnable = FALSE;
			g_LoadMeshThread.DeleteThread();
			g_LoadMeshThread.m_bEnable = FALSE;

			g_CharacterTextureArchive.m_bUseThread = TRUE;

			g_CharacterEffectGroupManager.m_TextureManager.m_bUseThread = TRUE;
			g_WorldEffectGroupManager.m_TextureManager.m_bUseThread = TRUE;
		}
		else
		{
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
		}
	}
	else if( strcmpi( ParseCommandString[1], "deletetexture" ) == 0 )
	{
		g_TerrainTextureArchive.ReleaseTextureData();
		g_WorldObjectTextureArchive.ReleaseTextureData();
		g_WorldEffectGroupManager.m_TextureManager.ReleaseTextureData();
		//g_MainInterfaceTextureArchive.ReleaseTextureData();
		//g_EffectTextureArchive.ReleaseTextureData();
		//g_CharacterTextureArchive.ReleaseTextureData();
		//g_MobTextureArchive.ReleaseTextureData();
	}
	else if( strcmpi( ParseCommandString[1], "envsound" ) == 0 )
	{
		if( strlen( ParseCommandString[2] ) == 0 )
		{
			g_DrawEnvSoundObject = !g_DrawEnvSoundObject;
		}
		else if( strcmpi( ParseCommandString[2], "day" ) == 0 )
		{
			g_EnvSoundObjectViewFilter = 0;
		}
		else if( strcmpi( ParseCommandString[2], "nightfall" ) == 0 )
		{
			g_EnvSoundObjectViewFilter = 1;
		}
		else if( strcmpi( ParseCommandString[2], "night" ) == 0 )
		{
			g_EnvSoundObjectViewFilter = 2;
		}
		else if( strcmpi( ParseCommandString[2], "dawn" ) == 0 )
		{
			g_EnvSoundObjectViewFilter = 3;
		}
		else if( strcmpi( ParseCommandString[2], "all" ) == 0 )
		{
			g_EnvSoundObjectViewFilter = -1;
		}
		else
		{
			g_DrawEnvSoundObject = !g_DrawEnvSoundObject;
		}

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}	
	else if( strcmpi( ParseCommandString[1], "chatmutetime" ) == 0 )
	{
		g_pLocalUser->m_ChatMuteTime = atoi( ParseCommandString[2] );
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "userstatus" ) == 0 )
	{
		g_ShowUserStatus = !g_ShowUserStatus;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
#endif
	else if( strcmpi( ParseCommandString[1], "reloadstring" ) == 0 ) 
	{
		((CEmperorOfDragonsApp*)gApp)->ChangeLanguage( g_LanguageType );
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "changelanguage" ) == 0 || 
			 strcmpi( ParseCommandString[1], "cl" ) == 0)
	{
		_XLANGUAGE_TYPE	languagetype = _XLANGUAGE_TYPE_KOREAN;

		if( strcmpi( ParseCommandString[2], "kr" ) == 0 )
		{
			languagetype = _XLANGUAGE_TYPE_KOREAN;
		}
		else if( strcmpi( ParseCommandString[2], "vn" ) == 0 )
		{
			languagetype = _XLANGUAGE_TYPE_VIETNAMESE;			
		}
		else if( strcmpi( ParseCommandString[2], "tw" ) == 0 )
		{
			languagetype = _XLANGUAGE_TYPE_TAIWANESE;			
		}
		else if( strcmpi( ParseCommandString[2], "eng" ) == 0 )
		{
			languagetype = _XLANGUAGE_TYPE_ENGLISH;			
		}
		else if( strcmpi( ParseCommandString[2], "cn" ) == 0 )
		{
			languagetype = _XLANGUAGE_TYPE_CHINESE;
		}
		else if( strcmpi( ParseCommandString[2], "jp" ) == 0 )
		{
			languagetype = _XLANGUAGE_TYPE_JAPANESE;			
		}
		else if( strcmpi( ParseCommandString[2], "rs" ) == 0 )
		{
			languagetype = _XLANGUAGE_TYPE_RUSSIAN;			
		}

		((CEmperorOfDragonsApp*)gApp)->ChangeLanguage( languagetype );
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "bloomfilter" ) == 0 )
	{
		g_PostProcessManager.EnableBloomFilter( !g_PostProcessManager.m_UseBloomFilter );
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "motionblur" ) == 0 )
	{
#ifdef _XTS_MOTIONBLURTEST
		if(!g_ScrrenMotionBlurMode)
		{
			g_ScrrenMotionBlurMode = TRUE;
			g_ScreenMotionBlurEffectManager.Enable(true);
			
			g_ScreenMotionBlurEffectManager.SetMinBlurAlpha(0.0f);
			g_ScreenMotionBlurEffectManager.SetMaxBlurAlpha(0.7f);
			g_ScreenMotionBlurEffectManager.AddBlurAlpha(0.001f);
		}
		else
		{
			g_ScrrenMotionBlurMode = FALSE;
			g_ScreenMotionBlurEffectManager.Enable(false);
		}
#else
		g_ObjectBlurProcessManager.m_fIntensity = atoi( ParseCommandString[2] );
#endif

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "radialblur" ) == 0 )
	{
		g_ObjectBlurProcessManager.m_iNumRadialBlurSteps = atoi( ParseCommandString[2] );
		g_ObjectBlurProcessManager.m_fUVDelta = atof( ParseCommandString[3] );
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "gaussianfactor") == 0)
	{
		g_PostProcessManager.m_GaussianFactor = atoi(ParseCommandString[2]);		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "bfpower") == 0)
	{		
		g_PostProcessManager.m_FinalPixelShaderConst1[0] = atof(ParseCommandString[2]);
		g_PostProcessManager.m_FinalPixelShaderConst1[1] = g_PostProcessManager.m_FinalPixelShaderConst1[0];
		g_PostProcessManager.m_FinalPixelShaderConst1[2] = g_PostProcessManager.m_FinalPixelShaderConst1[0];
		g_PostProcessManager.m_FinalPixelShaderConst1[3] = g_PostProcessManager.m_FinalPixelShaderConst1[0];
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}	
	else if( strcmpi( ParseCommandString[1], "fillmode" ) == 0 )
	{
		if( strcmpi( ParseCommandString[2], "point" ) == 0 )
		{
			SetRenderState( D3DRS_FILLMODE, D3DFILL_POINT );
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
		else if( strcmpi( ParseCommandString[2], "wire" ) == 0 )
		{
			SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
		else //if( strcmpi( ParseCommandString[2], "solid" ) == 0 )
		{
			SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );			
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
	}
	else if( strcmpi( ParseCommandString[1], "fog" ) == 0 )
	{	
		g_EnvironmentManager.m_Fog = !g_EnvironmentManager.m_Fog;
		SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( (strcmpi( ParseCommandString[1], "listenerdistancefactor" ) == 0) ||
			 (strcmpi( ParseCommandString[1], "ldf" ) == 0) )
	{		
#ifdef _XUSEFMOD		
		FLOAT distancefactor = atof(ParseCommandString[2]);
		FSOUND_3D_SetDistanceFactor( distancefactor );
#else
		FLOAT distancefactor = atof(ParseCommandString[2]);
		g_XAudioSystem.m_pListener->SetDistanceFactor( distancefactor );
		g_XAudioSystem.m_pListener->CommitDeferredSettings();
#endif
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}	
	else if( strcmpi( ParseCommandString[1], "coord" ) == 0 )
	{
		if( strcmpi( ParseCommandString[2], "curuser" ) == 0 )
		{
			FLOAT x	= atof(ParseCommandString[3]);
			FLOAT z	= atof(ParseCommandString[4]);
	
			g_pLocalUser->m_Position.x = x - g_LodTerrain.m_HalfWorldSize;
			g_pLocalUser->m_Position.z = z - g_LodTerrain.m_HalfWorldSize;

			_XOBBData* pOBBData = NULL;

			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pOBBData );
			g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
			g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
			g_pLocalUser->m_PathNodeCount = 0;
			g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;			

			g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
			g_pLocalUser->m_pGoundCollideOBB = pOBBData;

			g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
												   g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
												   g_pLocalUser->m_Position.z ) );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);

			_XWindow_WorldMinimap* pminimapwindow = 
				(_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{
				// Set direction to minimap arrow 
				pminimapwindow->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
				// Set minimap status
				pminimapwindow->SetLocalUserPosition();
			}

			g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
			
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
		else if( strcmpi( ParseCommandString[2], "user" ) == 0 )
		{				
			checkvalidcommand = _XDEF_NOTREADYCOMMAND;				
		}
	}		
	else if( strcmpi( ParseCommandString[1], "goto" ) == 0 )
	{
		_XUser* _user = g_Userlist.FindUser( ParseCommandString[2] );

		if( _user )
		{
			g_pLocalUser->m_Position = _user->m_Position;				
			g_pLocalUser->m_TargetPosition = _user->m_Position;				
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );

			g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
												   g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
												   g_pLocalUser->m_Position.z ) );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);

			g_NetworkKernel.SendPacket( 0/*MSG_NUMBER_PLAYER_MOVE*/ );
		}

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "gotomob" ) == 0 )
	{
		_XMob* _mob = g_Moblist.FindMob( atoi(ParseCommandString[2]) );

		if( _mob )
		{
			g_pLocalUser->m_Position = _mob->m_Position;
			g_pLocalUser->m_TargetPosition = _mob->m_Position;				
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainHeight( g_pLocalUser->m_Position );

			g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
												   g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
												   g_pLocalUser->m_Position.z ) );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);

			g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
		}

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "gotonpc" ) == 0 )
	{
		_XNPCObject* pNPC = NULL;

		for( int i = 0; i < g_Npclist.m_NPCObjectCount; i++ )
		{
			if( strcmpi( g_Npclist.m_pNPCObjectArray[i].m_NPCName, ParseCommandString[2] ) == 0 )
			{
				pNPC = &g_Npclist.m_pNPCObjectArray[i];
			}
		}
		
		if( pNPC )
		{
			g_pLocalUser->m_Position.x = pNPC->m_ModelDescriptor.m_Position._41;			
			g_pLocalUser->m_Position.z = pNPC->m_ModelDescriptor.m_Position._43;
			
			_XOBBData* pOBB;
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndOBBHeight( g_pLocalUser->m_Position, pOBB );
			g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;

			if( pOBB )
			{
				g_pLocalUser->m_pGoundCollideOBB = 	pOBB;
			}
			
			g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
				g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
				g_pLocalUser->m_Position.z ) );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
			
			g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
		}
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "gotoarea" ) == 0 )
	{		
		D3DXVECTOR3 position;

		int	areaindex = atoi( ParseCommandString[2] );
		int	areacategory = atoi( ParseCommandString[3] );
		
		if( g_AreaManager.FindAreaCenterPosition( position, areaindex, areacategory ) )
		{
			g_pLocalUser->m_Position = position;			
			
			_XOBBData* pOBB;
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndOBBHeight( g_pLocalUser->m_Position, pOBB );
			g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
			
			if( pOBB )
			{
				g_pLocalUser->m_pGoundCollideOBB = 	pOBB;
			}
			
			g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
				g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
				g_pLocalUser->m_Position.z ) );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
			
			g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
		}
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "stuv" ) == 0 )
	{
		FLOAT addvalue = atof( ParseCommandString[3] );

		if( strcmpi( ParseCommandString[2], "0" ) == 0 )
			g_ShadowDefaultoffset._11 +=  addvalue;
		else if( strcmpi( ParseCommandString[2], "1" ) == 0 )
			g_ShadowDefaultoffset._22 +=  addvalue;
		else if( strcmpi( ParseCommandString[2], "2" ) == 0 )
			g_ShadowDefaultoffset._31 +=  addvalue;
		else if( strcmpi( ParseCommandString[2], "3" ) == 0 )			
			g_ShadowDefaultoffset._32 +=  addvalue;

		_XDWINPRINT( "11 : %f" , g_ShadowDefaultoffset._11 );
		_XDWINPRINT( "22 : %f" , g_ShadowDefaultoffset._22 );
		_XDWINPRINT( "31 : %f" , g_ShadowDefaultoffset._31 );
		_XDWINPRINT( "32 : %f" , g_ShadowDefaultoffset._32 );
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "env" ) == 0 )
	{
		if( strcmpi( ParseCommandString[2], "dawn" ) == 0 )
		{
			g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_DAWN;				
		}
		else if( strcmpi( ParseCommandString[2], "day" ) == 0 )
		{
			g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_DAY;
		}
		else if( strcmpi( ParseCommandString[2], "nightfall" ) == 0 )
		{
			g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_NIGHTFALL;
		}
		else if( strcmpi( ParseCommandString[2], "night" ) == 0 )
		{
			g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_NIGHT;
		}
		else if( strcmpi( ParseCommandString[2], "time" ) == 0 )
		{
			g_EnvironmentManager.m_EnvironmentTime = atoi(ParseCommandString[3]);
		}
		else if( strcmpi( ParseCommandString[2], "speed" ) == 0 )
		{
			g_EnvironmentManager.m_nTimeSpeed = atoi(ParseCommandString[3]);
		}

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if( strcmpi( ParseCommandString[1], "billinfo" ) == 0 )
	{
		g_ViewLocalUserBillingInfo = !g_ViewLocalUserBillingInfo;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "billremaintime" ) == 0 )
	{
		g_LocalUserBillingInformations.BillRemain  = atoi( ParseCommandString[2] );
	}
	else if( strcmpi( ParseCommandString[1], "setbillinfo" ) == 0 )
	{
		int _who	= atoi( ParseCommandString[2] );
		int _where	= atoi( ParseCommandString[3] );		
		int _method = atoi( ParseCommandString[4] );
		int _etc	= atoi( ParseCommandString[5] );

		g_LocalUserBillingInformations.BillMethod = _method;
		g_LocalUserBillingInformations.BillingStatusWho = _who;
		g_LocalUserBillingInformations.BillingStatusWhere = _where;
		g_LocalUserBillingInformations.BillingETCData = _etc;

		_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
		if(pPaymentMethod_Window)
			pPaymentMethod_Window->ResetTitleBar();
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
#ifdef _XDWDEBUG	
	else if( strcmpi( ParseCommandString[1], "3dsound" ) == 0 )
	{
		if( strcmpi( ParseCommandString[2], "camera" ) == 0 )
		{
			
		}
		else if( strcmpi( ParseCommandString[2], "user" ) == 0 )
		{

		}		
	}
	else if( strcmpi( ParseCommandString[1], "autoattack" ) == 0 )
	{	
		extern BOOL g_AutoAttackMode;
		g_AutoAttackMode = !g_AutoAttackMode;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "fury" ) == 0 )
	{
		if( strcmpi( ParseCommandString[2], "off" ) == 0 )
		{
			g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_NORMAL;
			if(g_pJinStatus_Window)
			{
				g_pJinStatus_Window->m_ConvergenceButton->ShowWindow(FALSE);
				g_pJinStatus_Window->SetConvergenceStartTime(g_LocalSystemTime);
			}
			
			_XPlayInterfaceSound(ID_SR_INTERFACE_ANGERFIRE01_WAV);
		}
		else if( strcmpi( ParseCommandString[2], "on" ) == 0 )
		{	
			g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_FURY;
			if(g_pJinStatus_Window)
			{
				//g_pJinStatus_Window->m_ConvergenceButton->ShowWindow(TRUE);
				//g_pJinStatus_Window->SetConvergenceStartTime(g_LocalSystemTime);
			}
			
			_XPlayInterfaceSound(ID_SR_INTERFACE_ANGERFIRE01_WAV);
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "mouselock" ) == 0 )
	{
		extern BOOL g_MouseLockFlag;

		g_MouseLockFlag = !g_MouseLockFlag;

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "userculling" ) == 0 )
	{
		g_LodTerrain.m_ObjectQuadTree.m_PlayerCulling = !g_LodTerrain.m_ObjectQuadTree.m_PlayerCulling;

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if( strcmpi( ParseCommandString[1], "cullingfarplane" ) == 0 )
	{
		FLOAT farplane = atof(ParseCommandString[2]);

		g_LodTerrain.m_ObjectQuadTree.m_CullingFarPlane = farplane;

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "spidermode" ) == 0 )
	{
		g_SpiderMode = !g_SpiderMode;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "shadow" ) == 0 )
	{
		g_bCharacterShadow = !g_bCharacterShadow;
		g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCHARACTERSHADOW,  g_bCharacterShadow );
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "usetexture" ) == 0 )
	{
		g_UseTexture = !g_UseTexture;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}	
	else if(strcmpi(ParseCommandString[1], "reloadrenderstyle") == 0)
	{
		_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );
#ifdef _XTESTSERVER			
		g_TerrainTextureArchive.AttachRenderStyle( "tsEnvironment.xrs" );
		g_CharacterTextureArchive.AttachRenderStyle( "tsCharacter.xrs" );			
		g_MobTextureArchive.AttachRenderStyle( "tsMonster.xrs" );
		g_MainInterfaceTextureArchive.AttachRenderStyle( "tsInterface.xrs" );
		g_WorldObjectTextureArchive.AttachRenderStyle( "tsObject.xrs" );				
		g_DefaultRenderStyleManager.Load( "tsDefault.xrs" );
		g_SpeedTreeTextureArchive.AttachRenderStyle( "tsSTModel.xrs" );
#else
		g_TerrainTextureArchive.AttachRenderStyle( "Environment.xrs" );
		g_CharacterTextureArchive.AttachRenderStyle( "Character.xrs" );			
		g_MobTextureArchive.AttachRenderStyle( "Monster.xrs" );
		g_MainInterfaceTextureArchive.AttachRenderStyle( "Interface.xrs" );
		g_WorldObjectTextureArchive.AttachRenderStyle( "Object.xrs" );				
		g_DefaultRenderStyleManager.Load( "Default.xrs" );
		g_SpeedTreeTextureArchive.AttachRenderStyle( "STModel.xrs" );
#endif
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if(strcmpi(ParseCommandString[1], "reloadmobdescript") == 0)
	{
		_XReLoadMOBModelScripter( "MobModel.XMS" );

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if(strcmpi(ParseCommandString[1], "resetmonsterpos") == 0)
	{
		if( !g_Moblist.m_smMobList.empty() )
		{
			int aaa = 0;
			_XMob* pMob;
			smdef_XMob::iterator it;
			for (it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ aaa, ++ it)
			{
				pMob = (*it).second;
				pMob->m_Position.x = aaa * 2 ;// - (FLOAT)_XRand( 200l );
				pMob->m_Position.y = 0.0f;
				pMob->m_Position.z = 110.0f;// - (FLOAT)_XRand( 200l );
				 
				pMob->m_EndPosition.x = aaa * 2 ;// - (FLOAT)_XRand( 200l );
				pMob->m_EndPosition.y = 0.0f;
				pMob->m_EndPosition.z = -100.0f;// - (FLOAT)_XRand( 200l );
				
				pMob->SetAnimation( _XMOBACTION_MOVE );
				pMob->m_fMoveSpeed = g_MobAttribTable[ pMob->m_MobType ].movespeed_walk;

				pMob->m_MoveVector	 = pMob->m_EndPosition - pMob->m_Position;
				D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
			}
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if( strcmpi( ParseCommandString[1], "language" ) == 0 )
	{				
		if( strcmpi( ParseCommandString[2], "korean" ) == 0 )
		{
			strcpy( g_XFontName, _T("gulim12.xft") );
		}
		else if( strcmpi( ParseCommandString[2], "chinese" ) == 0  )
		{
			strcpy( g_XFontName, _T("SimSun12.xft") );
		}
		else 
		{
			strcpy( g_XFontName, _T("gulim12.xft") );
		}
		
		g_XBaseFont->Load( g_XFontName );
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "rotatechar") == 0)
	{
		if(strcmpi(ParseCommandString[2], "+") == 0)
		{
			g_pLocalUser->m_RotateAngle += 0.5f;
			g_pLocalUser->UpdatePosition();
		}
		else if(strcmpi(ParseCommandString[2], "-") == 0)
		{
			g_pLocalUser->m_RotateAngle -= 0.5f;
			g_pLocalUser->UpdatePosition();
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "questflash") == 0)
	{
		int flashnumber = atoi(ParseCommandString[2]);
		g_indexScene = flashnumber;
		if( g_SceneList[g_indexScene ] )
		{
			
			if( strcmpi( ParseCommandString[3], "quest" ) == 0 )
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_QUEST;
			}
			else if( strcmpi( ParseCommandString[3], "none" ) == 0 )
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_NONE;
			}
			else if( strcmpi( ParseCommandString[3], "boss" ) == 0 )
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_BOSS;
			}
			else if( strcmpi( ParseCommandString[3], "map" ) == 0 )
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_MAP;
			}
			else if( strcmpi( ParseCommandString[3], "class" ) == 0 )
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_CLASSEXPLAIN;
			}
			else
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_QUEST;
			}
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "poisonfog") == 0)
	{
		if( !g_ViewPoisonFog ) 
		{
			g_ViewPoisonFog = TRUE;
		}
		else
		{
			g_DeleteStartViewPoisonFog = TRUE;
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
#endif
	else if( strcmpi( ParseCommandString[1], "fov" ) == 0 )
	{
		FLOAT fov = atof(ParseCommandString[2]);
		g_DefaultCameraFov = fov;
		g_LodTerrain.m_3PCamera.mp_fFov = fov;
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.SetCameraInfo( g_LodTerrain.m_3PCamera );
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.RebuildLevel(FALSE);

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "nplane" ) == 0 )
	{
		FLOAT nearplane = atof(ParseCommandString[2]);
		g_LodTerrain.m_3PCamera.mp_fNearPlane = nearplane;
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.SetCameraInfo( g_LodTerrain.m_3PCamera );
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.RebuildLevel(FALSE);
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "fplane" ) == 0 )
	{
		FLOAT farplane = atof(ParseCommandString[2]);
		g_LodTerrain.m_3PCamera.mp_fFarPlane = farplane;
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.SetCameraInfo( g_LodTerrain.m_3PCamera );
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.RebuildLevel(FALSE);
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "lod" ) == 0 )
	{
		if( strcmpi( ParseCommandString[2], "add" ) == 0 )
		{
			FLOAT lodlevel = atof(ParseCommandString[3]);
			g_LodTerrain.m_LODLevel1 += lodlevel;
			g_LodTerrain.m_LODLevel2 += lodlevel;
			g_LodTerrain.m_LODLevel3 += lodlevel;
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else if( strcmpi( ParseCommandString[2], "set" ) == 0 )
		{
			FLOAT lodlevel = atof(ParseCommandString[5]);
			g_LodTerrain.m_LODLevel1 = lodlevel;
			lodlevel = atof(ParseCommandString[4]);
			g_LodTerrain.m_LODLevel2 = lodlevel;
			lodlevel = atof(ParseCommandString[3]);
			g_LodTerrain.m_LODLevel3 = lodlevel;				
			g_LodTerrain.RebuildLevel(FALSE);
		}		
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "rtimeobject" ) == 0 )
	{
		g_bUseRealtimeGenerateObject = !g_bUseRealtimeGenerateObject;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}					
	else if(strcmpi(ParseCommandString[1], "clearquest") == 0)
	{
		if(g_SinglePlayMode || g_ZeroLevelMode )	// hotblood zerolevel
		{
			g_QuestScriptManager.ResetQuest();
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
	}
	else if(strcmpi(ParseCommandString[1], "resetskill") == 0)
	{
		g_pLocalUser->ResetSkillAnimation();
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "startmed") == 0)
	{
		int step = atoi(ParseCommandString[2]);

		_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
		if(pMeditation_Window)
		{
			pMeditation_Window->m_bQuestMiniGameStart = TRUE;
			pMeditation_Window->SetMeditationStep(step);
		}
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRAINING_MEDITATION_START, 0), (LPARAM)gHWnd);

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setfury") == 0)
	{
		g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, ctrl_ch_full_fury);	// 분노 만땅 상태
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setclass") == 0)
	{
		int cClass = atoi(ParseCommandString[2]);
//		int cClassGrade = atoi(ParseCommandString[3]);
//		int point = atoi(ParseCommandString[4]);

//		g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 8, cClass, cClassGrade, point);
		g_pLocalUser->m_CharacterInfo.Set_cClass( (char)cClass );
		g_pLocalUser->m_CharacterInfo.Set_cClassGrade( 1 );

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setmasterypoint") == 0)
	{
		int point = atoi(ParseCommandString[2]);

		g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( (short)point );

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "startinvi") == 0)
	{
		g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 9, 1);
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "endinvi") == 0)
	{
		g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 9, 0);
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "lock") == 0)
	{
		if(strcmpi(ParseCommandString[2], "move") == 0)
			g_pLocalUser->m_Lock.movelock = !g_pLocalUser->m_Lock.movelock;
		else if(strcmpi(ParseCommandString[2], "skill") == 0)
			g_pLocalUser->m_Lock.skilllock = !g_pLocalUser->m_Lock.skilllock;
		else if(strcmpi(ParseCommandString[2], "med") == 0)
			g_pLocalUser->m_Lock.medlock = !g_pLocalUser->m_Lock.medlock;
		else if(strcmpi(ParseCommandString[2], "spell") == 0)
			g_pLocalUser->m_Lock.spelllock = !g_pLocalUser->m_Lock.spelllock;
		else if(strcmpi(ParseCommandString[2], "speed") == 0)
			g_pLocalUser->m_Lock.speedlock = !g_pLocalUser->m_Lock.speedlock;
		else if(strcmpi(ParseCommandString[2], "battle") == 0)
			g_pLocalUser->m_Lock.battlelock = !g_pLocalUser->m_Lock.battlelock;
		else if(strcmpi(ParseCommandString[2], "chat") == 0)
			g_pLocalUser->m_Lock.chatlock = !g_pLocalUser->m_Lock.chatlock;
		else if(strcmpi(ParseCommandString[2], "trade") == 0)
			g_pLocalUser->m_Lock.tradelock = !g_pLocalUser->m_Lock.tradelock;
		else if(strcmpi(ParseCommandString[2], "mode") == 0)
			g_pLocalUser->m_Lock.modelock = !g_pLocalUser->m_Lock.modelock;
		else if(strcmpi(ParseCommandString[2], "party") == 0)
			g_pLocalUser->m_Lock.partylock = !g_pLocalUser->m_Lock.partylock;
		else if(strcmpi(ParseCommandString[2], "camerarotate") == 0)
			g_pLocalUser->m_Lock.camerarotatelock = !g_pLocalUser->m_Lock.camerarotatelock;
		else if(strcmpi(ParseCommandString[2], "camerazoom") == 0)
			g_pLocalUser->m_Lock.camerazoomlock = !g_pLocalUser->m_Lock.camerazoomlock;
		else if(strcmpi(ParseCommandString[2], "camerareverse") == 0)
			g_pLocalUser->m_Lock.camerareverse = !g_pLocalUser->m_Lock.camerareverse;

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "exit") == 0)
	{
		int index = atoi(ParseCommandString[2]);
		g_pLocalUser->SetAnimationIndex(index);

		/*
		if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE)
		{
			if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
			{
				D3DXVECTOR3 position = g_pLocalUser->m_Position;
				if(g_pLocalUser->m_pCollideOBB)
				{
					D3DXVECTOR3 obbcenter = g_pLocalUser->m_pCollideOBB->Center();
					D3DXVECTOR3 direction =  position - obbcenter;
					
					FLOAT distance = D3DXVec3Length(&direction);
					FLOAT scale = 1 + distance/100.f;
					
					D3DXVec3Normalize(&direction, &direction);
					
					D3DXVECTOR3 newposition = g_pLocalUser->m_Position + direction*scale;
					
					_XOBBData* pOBBData = NULL;			
					g_pLocalUser->m_Position = newposition;
					g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( newposition, pOBBData );
					
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_pGoundCollideOBB = pOBBData;
					g_pLocalUser->UpdatePosition();
					
					g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
						g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
						g_pLocalUser->m_Position.z ) );
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel(FALSE);
					
					_XWindow_WorldMinimap* pminimapwindow = 
						(_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
					if( pminimapwindow )
					{
						// Set direction to minimap arrow 
						pminimapwindow->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI );					
						// Set minimap status
						pminimapwindow->SetLocalUserPosition();
					}
				}
			}
		}*/

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "vt") == 0)
	{
		if(strcmpi(ParseCommandString[2], "x") == 0)
		{
			int arg = atoi(ParseCommandString[3]);
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);	
			pTutorialIcon->RemoveTutorialIcon((_XTUTORIALTYPE)arg);
		}
		else if(strcmpi(ParseCommandString[2], "n") == 0)
		{
			int arg = atoi(ParseCommandString[3]);
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);	
			pTutorialIcon->InsertTutorialIcon((_XTUTORIALTYPE)arg);
		}

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setlevel") == 0)
	{
		int level = atoi(ParseCommandString[2]);
		g_pLocalUser->m_CharacterInfo.Set_innerlevel( level );

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setlife") == 0)
	{
		int life = atoi(ParseCommandString[2]);
		g_pLocalUser->m_CharacterInfo.Set_max_lifepower( life );
		g_pLocalUser->m_CharacterInfo.Set_current_lifepower( life );
		g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setforce") == 0)
	{
		int force = atoi(ParseCommandString[2]);
		g_pLocalUser->m_CharacterInfo.Set_max_forcepower( force );
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( force );
		g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );


		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setgroup") == 0)
	{
		int group = atoi(ParseCommandString[2]);
		switch(group)
		{
		case 0 :
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_NANGIN;
			break;
		case 1:
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_GAEBANG;
			break;
		case 2 :
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_BEEGOONG;
			break;
		case 3 :
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_SORIM;
			break;
		case 4 :
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_NOCKRIM;
			break;
		case 5 :
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_MOODANG;
			break;
		case 6 :
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_MAKYO;
			break;
		default :
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_NANGIN;
			break;
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setmoney") == 0)
	{
		UINT money = atol(ParseCommandString[2]);
		g_pLocalUser->m_Money = money;

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "viewmastery") == 0)
	{
		g_pLocalUser->m_bViewMasterySkill = TRUE;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "setbloodpoint") == 0)
	{
		int point = atoi(ParseCommandString[2]);
		g_pLocalUser->m_BloodPoint = point;

//		g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, ctrl_ch_full_bloodpoint);	// 혈정 Pt 만땅 상태

/*		g_pLocalUser->m_BloodPoint = 999;
		_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		if(pBloodPoint_Window)
		{
			if(g_pLocalUser->m_BloodPoint == 999)
			{
				pBloodPoint_Window->SetShowReqItem(TRUE);
			}
			else
			{
				pBloodPoint_Window->SetDrawAnimation(TRUE);
			}
		}
*/
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	
#ifdef _XADMINISTRATORMODE
	else if( (strcmpi(ParseCommandString[1], "monsterspawn") == 0 ) || 
			 (strcmpi(ParseCommandString[1], "ms") == 0 ) )
	{
		int monstercount = atoi(ParseCommandString[2]);

		TCHAR name[256];
		memset(name, 0, sizeof(name));

		if(strlen(ParseCommandString[5]) > 0)
			_snprintf(name, sizeof(name), "%s %s %s", ParseCommandString[3], ParseCommandString[4], ParseCommandString[5]);
		else if(strlen(ParseCommandString[4]) > 0)
			_snprintf(name, sizeof(name), "%s %s", ParseCommandString[3], ParseCommandString[4]);
		else if(strlen(ParseCommandString[3]) > 0)
			_snprintf(name, sizeof(name), "%s", ParseCommandString[3]);

		if( strcmpi( name, "all" ) == 0 )
		{
//			g_AllMonsterSpawnMode = TRUE;
//			g_CurrentSpawnMonsterType = 0;
//			g_LastMobSpawnTime = 0;
			/*
			int mobindex = -1;
			for( int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
			{
				if( strlen( g_MonsterName[ i ] ) > 0 )
				{
					g_NetworkKernel.SendPacket( MSG_NO_CTRL_MONSTER_SPAWN, i, 1 );
				}
			}
			*/
		}
		else
		{
			int mobindex = -1;
			for( int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
			{
				if(g_MonsterName[i])
				{
					if( strcmpi( g_MonsterName[ i ], name ) == 0 )
					{
						mobindex = i;
						break;
					}
				}
			}

			if( mobindex > -1 )
			{
				g_NetworkKernel.SendPacket( MSG_NO_CTRL_MONSTER_SPAWN, mobindex, monstercount );
			}
			else
			{
	#ifdef _XDWDEBUG	
				_XDebugOutputString( "아직 준비되지 않은 몬스터입니다.", 1, 3 );
				_XWindow_Debug* pDebugWindow = 
					(_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );
				
				if(pDebugWindow)
				{
					pDebugWindow->SetDebugInfoMode( 3 );
				}
	#endif
			}
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "addskill") == 0)
	{
		int skillid = atoi(ParseCommandString[2]);
		int step =  atoi(ParseCommandString[3]);

		if(skillid > 0)
		{
			g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 6, 0, skillid, step);
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
	}
	else if(strcmpi(ParseCommandString[1], "deleteskill") == 0)
	{
		int skillid = atoi(ParseCommandString[2]);

		if(skillid >= 0)
		{
			g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 6, 1, skillid);
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
	}
	else if(strcmpi(ParseCommandString[1], "setskilllevel") == 0)
	{
		int skillid =  atoi(ParseCommandString[2]);
		int skilllevel = atoi(ParseCommandString[3]);
		
		if(skillid > 0)
		{
			g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 6, 2, skillid, skilllevel);
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
	}
	else if(strcmpi(ParseCommandString[1], "settrainingskill") == 0)
	{
		int skillid = atoi(ParseCommandString[2]);

		if(skillid > 0)
		{
			g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 6, 3, skillid);
			checkvalidcommand = _XDEF_VALIDCOMMAND;
		}
	}
	
#endif
	if( strcmpi( ParseCommandString[1], "coord" ) == 0 )
	{
		if( strcmpi( ParseCommandString[2], "curuser" ) == 0 )
		{
			_XDWINPRINT( "Current user position : X : %f  Y : %f  Z : %f", g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y, g_pLocalUser->m_Position.z );

			checkvalidcommand = _XDEF_NODISPLAYCOMMAND;
		}
	}
	if( strcmpi( ParseCommandString[1], "command" ) == 0 )
	{			
#ifdef _XDWDEBUG	
		_XDebugOutputString( "---[ Command list ]---------------------------------------", 1, 3 );
		for( int i = 0; i < _XDEF_COMMADCOUNT; i++ )
		{				
			if( strlen( commandlist[i] ) <= 0 ) break;
			_XDebugOutputString( commandlist[i], 1, 3 );
		}
		_XDebugOutputString( "---end of list -------------------------------------------", 1, 3 );

		_XWindow_Debug* pDebugWindow = 
			(_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );

		if(pDebugWindow)
		{
			pDebugWindow->SetDebugInfoMode( 3 );
		}
#endif
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "viewdamagevalue" ) == 0 )
	{
		g_ViewDamageValue = !g_ViewDamageValue;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}

#ifdef _XDWDEBUG	
	else if( strcmpi( ParseCommandString[1], "scrinfo" ) == 0 )
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewRuntimeDebugData = 
			!((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewRuntimeDebugData;			

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "fps" ) == 0 )
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewFPS = 
			!((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewFPS;
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "axis" ) == 0 )
	{
		g_LodTerrain.viewaxis = !g_LodTerrain.viewaxis;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "object" ) == 0 )
	{
		g_LodTerrain.viewobject = !g_LodTerrain.viewobject;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "frustum" ) == 0 )
	{
		g_LodTerrain.m_ObjectQuadTree.m_ViewFrustum = 
			!g_LodTerrain.m_ObjectQuadTree.m_ViewFrustum;

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "lens" ) == 0 )
	{
		g_EnvironmentManager.m_ViewLensFlare = !g_EnvironmentManager.m_ViewLensFlare;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "obb" ) == 0 )
	{
		g_LodTerrain.m_ViewOBBObject = !g_LodTerrain.m_ViewOBBObject;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "shadowtexture" ) == 0 )
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewShadowTexture = 
			!((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewShadowTexture;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if( strcmpi( ParseCommandString[1], "lightmodel" ) == 0 )
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewLightConeModel = 
			!((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewLightConeModel;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "movetargetpos" ) == 0 )
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewArrowModel = 
			!((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewArrowModel;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "mouseray" ) == 0 )
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewMouseRay = 
			!((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewMouseRay;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "terrainrendertime" ) == 0 )		
	{
		g_LodTerrain.viewterrainrenderingtime = !g_LodTerrain.viewterrainrenderingtime;

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "normal" ) == 0 )		
	{
		g_ViewNormalVector = !g_ViewNormalVector;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if( strcmpi( ParseCommandString[1], "pathobject" ) == 0 )		
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewPathObject = 
			!((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewPathObject;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "effect" ) == 0 )		
	{
		if( strcmpi( ParseCommandString[2], "i" ) == 0 )
		{	
			if( strcmpi( ParseCommandString[3], "c" ) == 0 )
			{
				int templateid =  atoi(ParseCommandString[4]);
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
														templateid, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE );
			}
			if( strcmpi( ParseCommandString[3], "w" ) == 0 )
			{
				int templateid =  atoi(ParseCommandString[4]);
				g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, templateid, &g_pLocalUser->m_ModelDescriptor.m_Position );
			}
		}
		else if( strcmpi( ParseCommandString[2], "d" ) == 0 )
		{	
			if( strcmpi( ParseCommandString[3], "c" ) == 0 )
			{
				int templateid =  atoi(ParseCommandString[4]);
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(templateid);								
			}
			if( strcmpi( ParseCommandString[3], "w" ) == 0 )
			{
				int templateid =  atoi(ParseCommandString[4]);
				g_WorldEffectInstanceManager.DeleteInstance( templateid );
			}			
		}
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}	
#endif
	else if( strcmpi( ParseCommandString[1], "all" ) == 0 )
	{
		SetRenderState( D3DRS_FOGENABLE, FALSE );

		_XDWINPRINT( "Current user position : X : %f  Y : %f  Z : %f",  g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y,	g_pLocalUser->m_Position.z );
#ifdef _XDWDEBUG	
		g_LodTerrain.m_ObjectQuadTree.m_PlayerCulling = TRUE;			
		g_LodTerrain.viewaxis = TRUE;
		g_LodTerrain.m_ObjectQuadTree.m_ViewFrustum = TRUE;

		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewRuntimeDebugData = TRUE;
#endif
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "userlist" ) == 0 )
	{			
		_XDWINPRINT( "---[ Connected character list ]---------------------------" );
		_XDWINPRINT( "Total user count : %d", g_Userlist.m_smUserList.size() );

		int i = 0;
		_XUser* curuser;
		smdef_XUser::iterator it;
		for (it = g_Userlist.m_smUserList.begin(); it != g_Userlist.m_smUserList.end(); ++ i, ++ it)
		{
			curuser = (*it).second;
			_XDWINPRINT( "%4d %s [ %4.4f %4.4f %4.4f ]", i, curuser->m_CharacterInfo.charactername, curuser->m_Position.x,curuser->m_Position.y,curuser->m_Position.z );
		}
		_XDWINPRINT( "---end of list -------------------------------------------" );
		checkvalidcommand = _XDEF_NODISPLAYCOMMAND;
	}
#ifdef _XDWDEBUG
	else if( strcmpi( ParseCommandString[1], "moblist" ) == 0 )
	{
		_XWindow_Debug* pDebugWindow = 
			(_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );
		
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));
		
		_XDebugOutputString( "---[ mob list ]---------------------------", 1, 3 );				
		_snprintf( messagestring, sizeof(messagestring), "Total mob count : %d", g_Moblist.m_smMobList.size()  );
		_XDebugOutputString( messagestring, 1, 3 );

		if( !g_Moblist.m_smMobList.empty() )
		{
			int i = 0;				
			_XMob* curmob;
			smdef_XMob::iterator it;
			for (it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ i, ++ it)
			{
				curmob = (*it).second;
				_snprintf( messagestring, sizeof(messagestring), "%4d [%d:%s] [ %4.4f %4.4f %4.4f ]", i, curmob->m_MobID, g_MonsterName[curmob->m_MobType], curmob->m_Position.x,curmob->m_Position.y,curmob->m_Position.z );
				_XDebugOutputString( messagestring, 1, 3 );				
			}
		}
		
		_XDebugOutputString( "---end of list -------------------------------------------", 1, 3 );

		if(pDebugWindow)
		{
			pDebugWindow->SetDebugInfoMode( 3 );
		}

		checkvalidcommand = _XDEF_NODISPLAYCOMMAND;
	}
#endif
	else if( strcmpi( ParseCommandString[1], "windstrength" ) == 0 )		
	{
		if( strcmpi( ParseCommandString[2], "up" ) == 0 )
		{			
			g_XST_Wrapper.UpdateWindStrengths( g_XST_Wrapper.GetWindStrengths() + 0.05f );
		}
		else // down
		{
			g_XST_Wrapper.UpdateWindStrengths( g_XST_Wrapper.GetWindStrengths() - 0.05f );
		}
				
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if( strcmpi( ParseCommandString[1], "speedtreelod" ) == 0 )		
	{
		FLOAT lodnearlevel = atof(ParseCommandString[2]);
		FLOAT lodfarrlevel = atof(ParseCommandString[3]);
		
		for (int i = 0; i < g_XST_Wrapper.m_ObjectContainerCount; ++i)
		{
			g_XST_Wrapper.m_ObjectContainerArray[i].GetSpeedTree( )->SetLodLimits(lodnearlevel, lodfarrlevel);
		}

		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}		
	else if( strcmpi( ParseCommandString[1], "useshader" ) == 0 )		
	{
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
#ifdef _XDWDEBUG	
	else if( strcmpi( ParseCommandString[1], "abovewaterobject" ) == 0 )		
	{
		checkvalidcommand = _XDEF_VALIDCOMMAND;
		g_DrawAboveWaterObject = !g_DrawAboveWaterObject;
	}
	else if( strcmpi( ParseCommandString[1], "reflectionmap" ) == 0 )		
	{
		checkvalidcommand = _XDEF_VALIDCOMMAND;
		g_DrawReflectionMap = !g_DrawReflectionMap;
	}	
	else if( strcmpi( ParseCommandString[1], "viewgraph" ) == 0 )		
	{
		checkvalidcommand = _XDEF_VALIDCOMMAND;
		g_DrawGraphLog = !g_DrawGraphLog;
	}	
	else if( strcmpi( ParseCommandString[1], "resreport" ) == 0 )		
	{
		checkvalidcommand = _XDEF_VALIDCOMMAND;
		ReportUsingSystemResource();
	}
	else if( strcmpi( ParseCommandString[1], "pathfindinfo" ) == 0 )		
	{
		checkvalidcommand = _XDEF_VALIDCOMMAND;
		g_ViewPathFindInfo = !g_ViewPathFindInfo;
	}	
	else if( strcmpi( ParseCommandString[1], "usesplatindex" ) == 0 )		
	{
		checkvalidcommand = _XDEF_VALIDCOMMAND;
		BOOL extern g_UseSplatIndexBuffer;
		g_UseSplatIndexBuffer = !g_UseSplatIndexBuffer;
	}
	else if( strcmpi( ParseCommandString[1], "viewlayer" ) == 0 )		
	{
		extern BOOL g_ViewSplatLayer[_XDEF_MAXSPLATCOUNT];

		int layerindex =  atoi(ParseCommandString[2]);
		g_ViewSplatLayer[layerindex] = !g_ViewSplatLayer[layerindex];
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( strcmpi( ParseCommandString[1], "quad" ) == 0 )		
	{
		QUAD_TREE = !QUAD_TREE;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "viewlog") == 0)
	{
		if(strcmpi(ParseCommandString[2], "packet") == 0)
		{
			g_ViewPacketLog = !g_ViewPacketLog;
			checkvalidcommand = _XDEF_VALIDCOMMAND;
			if(g_ViewPacketLog)
				_XDWINPRINT("---[View Packet Log]-------------------------------------");
			else
				_XDWINPRINT("---------------------------------------------------------");
		}
		else if(strcmpi(ParseCommandString[2], "battle") == 0)
		{
			g_ViewBattleLog = !g_ViewBattleLog;
			checkvalidcommand = _XDEF_VALIDCOMMAND;
			if(g_ViewPacketLog)
				_XDWINPRINT("---[View Battle Log]-------------------------------------");
			else
				_XDWINPRINT("---------------------------------------------------------");
		}
	}
	else if(strcmpi(ParseCommandString[1], "battledetail") == 0)
	{
		g_ViewBattleDetail = !g_ViewBattleDetail;
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "userpacketlog") == 0)
	{
		if( strlen( ParseCommandString[2]) > 0 )
		{
			g_EnableUserPacketFiltering = TRUE;
			strcpy( g_PacketFilteringTargetName, ParseCommandString[2] );
		}
		else
		{
			g_EnableUserPacketFiltering = FALSE;
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "usertrace") == 0)
	{
		if( strlen( ParseCommandString[2]) > 0 )
		{
			g_UserTracing = TRUE;
			memset( g_UserTraceTargetName, 0, sizeof(TCHAR) * 13 );
			strcpy( g_UserTraceTargetName, ParseCommandString[2] );
		}
		else
		{
			g_UserTracing = FALSE;
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if( (strcmpi(ParseCommandString[1], "clientmob") == 0 ) || 
			 (strcmpi(ParseCommandString[1], "cms") == 0 ) )
	{
		int monstercount = atoi(ParseCommandString[2]);

		TCHAR name[256];
		memset(name, 0, sizeof(name));

		if(strlen(ParseCommandString[5]) > 0)
			_snprintf(name, sizeof(name), "%s %s %s", ParseCommandString[3], ParseCommandString[4], ParseCommandString[5]);
		else if(strlen(ParseCommandString[4]) > 0)
			_snprintf(name, sizeof(name), "%s %s", ParseCommandString[3], ParseCommandString[4]);
		else if(strlen(ParseCommandString[3]) > 0)
			_snprintf(name, sizeof(name), "%s", ParseCommandString[3]);

		if( strcmpi( name, "all" ) == 0 )
		{
			g_AllMonsterSpawnMode = TRUE;
			g_CurrentSpawnMonsterType = 0;
			g_LastMobSpawnTime = 0;
		
		}
		else
		{
			int mobindex = -1;
			for( int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
			{
				if(g_MonsterName[i])
				{
					if( strcmpi( g_MonsterName[ i ], name ) == 0 )
					{
						mobindex = i;
						break;
					}
				}
			}

			if( mobindex > -1 )
			{				
				int clientmobID = g_Moblist.FindClientMobID();
				if( clientmobID != 0 )
				{
					_XMob* pMob = g_Moblist.CreateMob(clientmobID, mobindex, 0, 100, g_pLocalUser->m_Position,g_pLocalUser->m_Position);	
					if( pMob )
					{
						pMob->m_bClientMob = TRUE;
						pMob->m_OriginPosition = g_pLocalUser->m_Position;
						pMob->m_AttackHitValue = 10; //공격력
					}
				}						
			}
			else
			{
				_XDebugOutputString( "Not ready monster", 1, 3 );
				_XWindow_Debug* pDebugWindow = 
					(_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );
				
				if(pDebugWindow)
				{
					pDebugWindow->SetDebugInfoMode( 3 );
				}
			}
		}
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}
	else if(strcmpi(ParseCommandString[1], "tutorial") == 0)
	{		
		if(g_pLocalUser->m_bVirtualMode)
		{			
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			//g_pLocalUser->m_bTutorialMode = FALSE;
		}
		else
		{
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_enter_tutorial_req);
			//g_pLocalUser->m_bTutorialMode = TRUE;
		}
		
		checkvalidcommand = _XDEF_VALIDCOMMAND;
	}	
	else if(strcmpi(ParseCommandString[1], "changewebpageaddress") == 0)
	{		
#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)
		if( g_bTestWebPageAddress )
			g_bTestWebPageAddress = FALSE;
		else
			g_bTestWebPageAddress = TRUE;
		
		int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
		TCHAR tempStr[256];
		memset( tempStr, 0, sizeof(tempStr) );

#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514
				TCHAR postdata[128];
				memset(postdata, 0, sizeof(postdata));

				int selectedserver = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
				_snprintf( postdata, sizeof(postdata), "AL_ID=%s&AL_PWD=%s&AL_Server=%d", g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserPW, selectedserver );
				_XDisplayHTMLPageAndPostData(g_hItemMallWebBrowserObject, _T("http://global1.indy21.co.kr:8080"), postdata );
#else
		if( !g_bTestWebPageAddress )
			_snprintf( tempStr, sizeof(tempStr), "http://ninemall.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
		else
			_snprintf( tempStr, sizeof(tempStr), "http://panda.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
#endif
				
		DisplayHTMLPage(g_hItemMallWebBrowserObject, tempStr);
#endif

	}

#endif

#ifdef _XMANAGER
	if( checkvalidcommand == _XDEF_VALIDCOMMAND )
	{		
		g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTCOMMAND, commandstring );
	}
#endif

#ifdef _XDWDEBUG
	
	TCHAR comandresultstr[512];
	memset(comandresultstr, 0, sizeof(comandresultstr));

	if( checkvalidcommand == _XDEF_VALIDCOMMAND )
	{
		_snprintf( comandresultstr, sizeof(comandresultstr), "Command :" );
		g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTCOMMAND, commandstring );
	}
	else if( checkvalidcommand == _XDEF_INVALIDCOMMAND )
	{
		_snprintf( comandresultstr, sizeof(comandresultstr),"Invalid command :" );
	}
	else if( checkvalidcommand == _XDEF_NOTREADYCOMMAND )
	{
		_snprintf( comandresultstr, sizeof(comandresultstr), "Not ready this command :" );
	}
	
	if( checkvalidcommand != _XDEF_NODISPLAYCOMMAND )
	{		
		_XDWINPRINT( "%s %s ", comandresultstr, commandstring );
		
		_XInsertConsoleWindowCommandHistory( commandstring );
	}	
	
#endif

}

#define _XGET_ALPHA( x ) (( (x) & 0xFF000000 ) >> 24)
#define _XGET_RED( x )   (( (x) & 0xFF0000 ) >> 16)
#define _XGET_GREEN( x ) (( (x) & 0xFF00 ) >> 8)
#define _XGET_BLUE( x )  (( (x) & 0xFF ) )

#define FLOAT01_TO_INT0255(X) ((int)(X*255.0f))
#define INT0255_TO_FLOAT01(X) ((float)X/255.0f)



////////////////////////////////////////////////////////////////////////////////////////////////
// Render state machine control dialog
#ifdef _XADMINISTRATORMODE

extern void			_XKernelCommandProcedure( LPSTR commandstring );
extern TCHAR		g_LastCommandString[512];
extern HWND			g_hRenderStateControllerWindow;		// Command window handle
extern HWND			g_hRenderStateControllerEditWindow;	// Command window editbox handle
extern WNDPROC		g_lpfnBaseEditWndProc;
HFONT				g_hSansSerifFont;

int					g_SelectedEnvSet;
int					g_SelectedLightClass = 0;

#define WM_EXTERNALPROGRAMCOMMNAD		WM_USER + 10

LRESULT CALLBACK _XKernelCommandWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam )
{
	switch ( message )
	{
	case WM_INITDIALOG:		
			{			
			g_hRenderStateControllerEditWindow = CreateWindowEx(WS_EX_CLIENTEDGE, "xcommandedit", "",
												WS_CHILD | WS_VISIBLE | ES_LEFT,
												7, 9, 408, 20,
												hDlg,
												(HMENU)NULL,
												(HINSTANCE) GetWindowLong(hDlg, GWL_HINSTANCE),
												NULL);			
			LOGFONT logfont;
			memset(&logfont, 0, sizeof(LOGFONT));
			logfont.lfCharSet = DEFAULT_CHARSET;
			logfont.lfHeight =  8;
			strcpy(logfont.lfFaceName, "MS Sans Serif");
			g_hSansSerifFont = CreateFontIndirect(&logfont);
			SendMessage(g_hRenderStateControllerEditWindow, WM_SETFONT, (WPARAM) g_hSansSerifFont, (LPARAM) 0);

			memset( g_LastCommandString, 0, 512 );

			if( !strlen( g_LastCommandString ) )					
				g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTCOMMAND, "", g_LastCommandString, 512, FALSE );				
			
			SetWindowText( g_hRenderStateControllerEditWindow, g_LastCommandString );
			ShowWindow( g_hRenderStateControllerEditWindow, SW_SHOW );

			RECT rect;
			GetWindowRect( hDlg, &rect );
			int consolewinxpos = (GetSystemMetrics(SM_CXSCREEN)-(rect.right-rect.left))/2;
			int consolewinypos = (GetSystemMetrics(SM_CYSCREEN)-(rect.bottom-rect.top))/2;
			consolewinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CONSOLEWINDOWPOSITIONX, consolewinxpos );			
			consolewinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CONSOLEWINDOWPOSITIONY, consolewinypos );
			int consolewinwidth  = rect.right-rect.left;
			int consolewinheight = rect.bottom-rect.top;
			MoveWindow( hDlg, consolewinxpos, consolewinypos, consolewinwidth, consolewinheight, TRUE );
			
			// Load last commnad list
			HWND hconsolewindowhistorylist = GetDlgItem( hDlg, IDC_COMMANDHISTORYLIST );
			if( hconsolewindowhistorylist )
			{
				TCHAR str[512];
				TCHAR mrucommandlistid[32];
				for( int i = 0; i < 10; i++ )
				{
					memset( str, 0, sizeof(str) );
					_snprintf( mrucommandlistid, sizeof(mrucommandlistid), "Last command %d", i );

					g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, mrucommandlistid, _T(""), str, 511 );

					if( strlen( str ) > 0 )
					{						
						SendMessage( hconsolewindowhistorylist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );
					}					
				}				

				SendMessage( hconsolewindowhistorylist, LB_SETCURSEL, 0, 0 );
			}		

			HWND lightclasscombo = GetDlgItem( hDlg, IDC_LIGHTCLASSCOMBO );
			if( lightclasscombo )
			{
				TCHAR str[512];												
				strcpy( str, _T("Terrain light") );
				SendMessage( lightclasscombo, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );
				strcpy( str, _T("Object light") );
				SendMessage( lightclasscombo, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );
				strcpy( str, _T("Character light") );
				SendMessage( lightclasscombo, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );

				SendMessage( lightclasscombo, CB_SETCURSEL, 0, 0 );	
			}

			HWND envsetlist = GetDlgItem( hDlg, IDC_ENVSETLIST );
			if( envsetlist )
			{
				TCHAR str[512];												
				strcpy( str, _T("Ambient color") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	0
				strcpy( str, _T("Fog color") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	1
								
				strcpy( str, _T("Light1 Ambient") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	7		
				strcpy( str, _T("Light1 Diffuse") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	8			
				strcpy( str, _T("Light1 Specular") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	9
				strcpy( str, _T("Light1 Range") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	10
				strcpy( str, _T("Light1 Falloff") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	11							
				strcpy( str, _T("Light1 Attenuation0") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	12							
				strcpy( str, _T("Light1 Attenuation1") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	13							
				strcpy( str, _T("Light1 Attenuation2") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	14
				strcpy( str, _T("Light1 Theta") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	15
				strcpy( str, _T("Light1 Phi") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	16								

				strcpy( str, _T("Fog near") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	17
				strcpy( str, _T("Fog far") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	18
				strcpy( str, _T("Fog density") );
				SendMessage( envsetlist, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)str );	//	19
				
				SendMessage( envsetlist, LB_SETCURSEL, 0, 0 );				
			}

			HWND redslider = GetDlgItem( hDlg, IDC_REDSLIDER );
			if( redslider )
			{
				SendMessage( redslider, TBM_SETRANGE, (WPARAM)FALSE, (LPARAM)MAKELONG( 0, 255 ) );
				SendMessage( redslider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM) 0 );
			}

			HWND greenslider = GetDlgItem( hDlg, IDC_GREENSLIDER );
			if( greenslider )
			{
				SendMessage( greenslider, TBM_SETRANGE, (WPARAM)FALSE, (LPARAM)MAKELONG( 0, 255 ) );
				SendMessage( greenslider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM) 0 );
			}

			HWND blueslider = GetDlgItem( hDlg, IDC_BLUESLIDER );
			if( blueslider )
			{
				SendMessage( blueslider, TBM_SETRANGE, (WPARAM)FALSE, (LPARAM)MAKELONG( 0, 255 ) );
				SendMessage( blueslider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM) 0 );
			}

			HWND alphaslider = GetDlgItem( hDlg, IDC_ALPHASLIDER );
			if( alphaslider )
			{
				SendMessage( alphaslider, TBM_SETRANGE, (WPARAM)FALSE, (LPARAM)MAKELONG( 0, 255 ) );
				SendMessage( alphaslider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM) 0 );
			}


#ifdef _XDWDEBUG				
			HWND checkbox = GetDlgItem( hDlg, IDC_VIEWLENSFLARECHECK );
			if( checkbox )
			{
				SendMessage( checkbox, BM_SETCHECK, 1, 0 );
			}
#endif
			}
			return TRUE;			
	case WM_HSCROLL :
		{
			HWND redslider   = GetDlgItem( hDlg, IDC_REDSLIDER );
			HWND greenslider = GetDlgItem( hDlg, IDC_GREENSLIDER );
			HWND blueslider  = GetDlgItem( hDlg, IDC_BLUESLIDER );
			HWND alphaslider = GetDlgItem( hDlg, IDC_ALPHASLIDER );
			FLOAT red,green,blue,alpha;

			red   = (FLOAT)SendMessage( redslider, TBM_GETPOS, 0, 0 );
			green = (FLOAT)SendMessage( greenslider, TBM_GETPOS, 0, 0 );
			blue  = (FLOAT)SendMessage( blueslider, TBM_GETPOS, 0, 0 );
			alpha = (FLOAT)SendMessage( alphaslider, TBM_GETPOS, 0, 0 );

			if( g_SelectedEnvSet >= 2 )
			{
				red   /= 255.0f;
				green /= 255.0f;
				blue  /= 255.0f;
				alpha /= 255.0f;
			}

			TCHAR data[20];
			memset(data, 0, sizeof(data));

			if( redslider == (HWND)lparam )
			{			
				_snprintf( data, sizeof(data), "%3.3f", red );				
				HWND rededit = GetDlgItem( hDlg, IDC_REDEDIT );
				Edit_SetSel(rededit, 0, -1);
				Edit_ReplaceSel(rededit, (LPARAM)data);
			}
			else if( greenslider == (HWND)lparam )
			{				
				_snprintf( data, sizeof(data), "%3.3f", green );
				HWND greenedit = GetDlgItem( hDlg, IDC_GREENEDIT );				
				Edit_SetSel(greenedit, 0, -1);
				Edit_ReplaceSel(greenedit, (LPARAM)data);
			}
			else if( blueslider == (HWND)lparam )
			{				
				_snprintf( data, sizeof(data), "%3.3f", blue );
				HWND blueedit = GetDlgItem( hDlg, IDC_BLUEEDIT );				
				Edit_SetSel(blueedit, 0, -1);
				Edit_ReplaceSel(blueedit, (LPARAM)data);
			}
			else if( alphaslider == (HWND)lparam )
			{			
				_snprintf( data, sizeof(data), "%3.3f", alpha );
				HWND alphaedit = GetDlgItem( hDlg, IDC_ALPHAEDIT );				
				Edit_SetSel(alphaedit, 0, -1);
				Edit_ReplaceSel(alphaedit, (LPARAM)data);
			}	
			
			D3DLIGHT9* pLightObject = NULL;
			if( g_SelectedLightClass == 0 )
			{
				pLightObject = &g_EnvironmentManager.m_TerrainLight;				
			}
			else if( g_SelectedLightClass == 1 )
			{
				pLightObject = &g_EnvironmentManager.m_ObjectLight;				
			}
			else if( g_SelectedLightClass == 2 )
			{
				pLightObject = &g_EnvironmentManager.m_CharacterLight;				
			}

			switch(g_SelectedEnvSet) 
			{
			case 0 : if( g_SelectedLightClass == 0 )
					 {
						 g_EnvironmentManager.m_TerrainAmbientColor = D3DCOLOR_ARGB( (BYTE)alpha, (BYTE)red, (BYTE)green, (BYTE)blue );
					 }
					 else if( g_SelectedLightClass == 1 )
					 {
						 g_EnvironmentManager.m_ObjectAmbientColor = D3DCOLOR_ARGB( (BYTE)alpha, (BYTE)red, (BYTE)green, (BYTE)blue );
					 }
					 else if( g_SelectedLightClass == 2 )
					 {
						 g_EnvironmentManager.m_CharacterAmbientColor = D3DCOLOR_ARGB( (BYTE)alpha, (BYTE)red, (BYTE)green, (BYTE)blue );
					 }				
				break;
			case 1 : g_EnvironmentManager.m_FogColor = D3DCOLOR_ARGB( (BYTE)alpha, (BYTE)red, (BYTE)green, (BYTE)blue );
				     gpDev->SetRenderState( D3DRS_FOGCOLOR,  g_EnvironmentManager.m_FogColor );
				break;			
			case 2 : pLightObject->Ambient.r = red;
					 pLightObject->Ambient.g = green;
					 pLightObject->Ambient.b = blue;
					 pLightObject->Ambient.a = alpha;
				break;
			case 3 : pLightObject->Diffuse.r = red;
					 pLightObject->Diffuse.g = green;
					 pLightObject->Diffuse.b = blue;
					 pLightObject->Diffuse.a = alpha;
				break;
			case 4 : pLightObject->Specular.r = red;
					 pLightObject->Specular.g = green;
					 pLightObject->Specular.b = blue;
					 pLightObject->Specular.a = alpha;
				break;	
			
			
			default:
				break;
			}
		}
		break;
#ifdef _XDWDEBUG				
	case WM_EXTERNALPROGRAMCOMMNAD :
		{
			_XDWINPRINTONLY( "Recieved External command" );
			switch( lparam ) 
			{
			case 0 :	
				break;
			case 1 :	// renderstyle & terrain editor
				{
					TCHAR message[256];
					strcpy( message, _T("reloadrenderstyle") );
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.ProcRenderStateCommand( message );
					_XUpdateWindow();
				}
				break;
			case 2 :	// string manager
				{
					TCHAR message[256];
					strcpy( message, _T("reloadstring") );
					((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.ProcRenderStateCommand( message );
					_XUpdateWindow();
				}
				break;
			default:
				break;
			}

		}
		break;
#endif
	case WM_COMMAND:
			{
			UINT wID = LOWORD(wparam);         // item, control, or accelerator identifier 
			HWND hwndCtl = (HWND) lparam;      // handle of control 
			switch( wID )
			{
			case IDC_LIGHTCLASSCOMBO :
				{
					case CBN_SELENDOK :
						g_SelectedLightClass = SendMessage( hwndCtl, CB_GETCURSEL, 0, 0 );
						break;
				}
				break;
			case IDC_VALUEEDIT :
				{
					if( HIWORD( wparam ) == EN_CHANGE )
					{
						HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
						TCHAR data[64] = _T("");
						if( valueedit )	Edit_GetLine(valueedit, 0, data, 63);

						D3DLIGHT9* pLightObject = NULL;
						if( g_SelectedLightClass == 0 )
						{
							pLightObject = &g_EnvironmentManager.m_TerrainLight;				
						}
						else if( g_SelectedLightClass == 1 )
						{
							pLightObject = &g_EnvironmentManager.m_ObjectLight;				
						}
						else if( g_SelectedLightClass == 2 )
						{
							pLightObject = &g_EnvironmentManager.m_CharacterLight;
						}

						switch(g_SelectedEnvSet) 
						{						
						case 5 :pLightObject->Range  = atof( data );							
							break;							
						case 6 :pLightObject->Falloff = atof( data );							
							break;							
						case 7 :pLightObject->Attenuation0 = atof( data );							
							break;							
						case 8 :pLightObject->Attenuation1 = atof( data );							
							break;							
						case 9 :pLightObject->Attenuation2 = atof( data );							
							break;							
						case 10 :pLightObject->Theta = atof( data );							
							break;							
						case 11 :pLightObject->Phi = atof( data );							
							break;
						case 12 :g_EnvironmentManager.m_FogNear = atof( data );							
							gpDev->SetRenderState( D3DRS_FOGSTART,   FtoDW(g_EnvironmentManager.m_FogNear) );							
							break;
						case 13 :g_EnvironmentManager.m_FogFar = atof( data );
							gpDev->SetRenderState( D3DRS_FOGEND,     FtoDW(g_EnvironmentManager.m_FogFar) );
							break;
						case 14 :g_EnvironmentManager.m_FogDensity = atof( data );							
							gpDev->SetRenderState( D3DRS_FOGDENSITY, FtoDW(g_EnvironmentManager.m_FogDensity) );
							break;							
						}
					}
				}
				break;
			case IDC_ENVSETLIST :
				{
					switch( HIWORD(wparam) )
					{
					case LBN_SELCHANGE :
						{
							g_SelectedEnvSet = SendMessage( hwndCtl, LB_GETCURSEL, 0, 0 );				
							
							HWND redslider   = GetDlgItem( hDlg, IDC_REDSLIDER );
							HWND greenslider = GetDlgItem( hDlg, IDC_GREENSLIDER );
							HWND blueslider  = GetDlgItem( hDlg, IDC_BLUESLIDER );
							HWND alphaslider = GetDlgItem( hDlg, IDC_ALPHASLIDER );
							
							float red,green,blue,alpha;
							red = green = blue = alpha = 0.0f;

							/*if( g_SelectedEnvSet <= 1 )
							{
								SendMessage( redslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
								SendMessage( greenslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
								SendMessage( blueslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
								SendMessage( alphaslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
							}
							else*/
							{
								SendMessage( redslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
								SendMessage( greenslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
								SendMessage( blueslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
								SendMessage( alphaslider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG( 0, 255 ) );
							}

							D3DLIGHT9* pLightObject = NULL;
							if( g_SelectedLightClass == 0 )
							{
								pLightObject = &g_EnvironmentManager.m_TerrainLight;				
							}
							else if( g_SelectedLightClass == 1 )
							{
								pLightObject = &g_EnvironmentManager.m_ObjectLight;				
							}
							else if( g_SelectedLightClass == 2 )
							{
								pLightObject = &g_EnvironmentManager.m_CharacterLight;				
							}

							switch(g_SelectedEnvSet) 
							{
							case 0 : if( g_SelectedLightClass == 0 )
									 {
										 red   = (FLOAT)_XGET_RED  (g_EnvironmentManager.m_TerrainAmbientColor);
										 green = (FLOAT)_XGET_GREEN(g_EnvironmentManager.m_TerrainAmbientColor);
										 blue  = (FLOAT)_XGET_BLUE (g_EnvironmentManager.m_TerrainAmbientColor);
										 alpha = (FLOAT)_XGET_ALPHA(g_EnvironmentManager.m_TerrainAmbientColor);
									 }
									 else if( g_SelectedLightClass == 1 )
									 {
										 red   = (FLOAT)_XGET_RED  (g_EnvironmentManager.m_ObjectAmbientColor);
										 green = (FLOAT)_XGET_GREEN(g_EnvironmentManager.m_ObjectAmbientColor);
										 blue  = (FLOAT)_XGET_BLUE (g_EnvironmentManager.m_ObjectAmbientColor);
										 alpha = (FLOAT)_XGET_ALPHA(g_EnvironmentManager.m_ObjectAmbientColor);
									 }
									 else if( g_SelectedLightClass == 2 )
									 {
										 red   = (FLOAT)_XGET_RED  (g_EnvironmentManager.m_CharacterAmbientColor);
										 green = (FLOAT)_XGET_GREEN(g_EnvironmentManager.m_CharacterAmbientColor);
										 blue  = (FLOAT)_XGET_BLUE (g_EnvironmentManager.m_CharacterAmbientColor);
										 alpha = (FLOAT)_XGET_ALPHA(g_EnvironmentManager.m_CharacterAmbientColor);
									 }
								break;
							case 1 : 
									red   = (FLOAT)_XGET_RED  (g_EnvironmentManager.m_FogColor);
									green = (FLOAT)_XGET_GREEN(g_EnvironmentManager.m_FogColor);
									blue  = (FLOAT)_XGET_BLUE (g_EnvironmentManager.m_FogColor);
									alpha = (FLOAT)_XGET_ALPHA(g_EnvironmentManager.m_FogColor);
								break;						
							case 2 :red   = pLightObject->Ambient.r * 255.0f;
									green = pLightObject->Ambient.g * 255.0f;
									blue  = pLightObject->Ambient.b * 255.0f;
									alpha = pLightObject->Ambient.a * 255.0f;								
								break;
							case 3 :red   = pLightObject->Diffuse.r * 255.0f;
									green = pLightObject->Diffuse.g * 255.0f;
									blue  = pLightObject->Diffuse.b * 255.0f;
									alpha = pLightObject->Diffuse.a * 255.0f;								
								break;
							case 4 :red   = pLightObject->Specular.r * 255.0f;
									green = pLightObject->Specular.g * 255.0f;
									blue  = pLightObject->Specular.b * 255.0f;
									alpha = pLightObject->Specular.a * 255.0f;								
								break;
							case 5:
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", pLightObject->Range );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;
							case 6:
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", pLightObject->Falloff );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;
							case 7:
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", pLightObject->Attenuation0 );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;
							case 8:
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", pLightObject->Attenuation1 );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;
							case 9:
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", pLightObject->Attenuation2 );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;
							case 10:
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", pLightObject->Theta );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;
							case 11:
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", pLightObject->Phi );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;
							case 12 :
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", g_EnvironmentManager.m_FogNear );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);									
								}								
								break;
							case 13 :
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", g_EnvironmentManager.m_FogFar );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);									
								}
								break;
							case 14 :
								{
									TCHAR data[20];
									memset(data, 0, sizeof(data));
									_snprintf( data, sizeof(data), "%3.5f", g_EnvironmentManager.m_FogDensity );
									HWND valueedit = GetDlgItem( hDlg, IDC_VALUEEDIT );
									Edit_SetSel(valueedit, 0, -1);
									Edit_ReplaceSel(valueedit, (LPARAM)data);
								}
								break;


							default:
								break;
							}							
														
							SendMessage( redslider,   TBM_SETPOS, 1, (LPARAM)red   );
							SendMessage( greenslider, TBM_SETPOS, 1, (LPARAM)green );
							SendMessage( blueslider,  TBM_SETPOS, 1, (LPARAM)blue  );
							SendMessage( alphaslider, TBM_SETPOS, 1, (LPARAM)alpha );

							if( g_SelectedEnvSet < 2 )
							{
								TCHAR data[20];
								memset(data, 0, sizeof(data));
								_snprintf( data, sizeof(data), "%3.3f", red);
								HWND rededit = GetDlgItem( hDlg, IDC_REDEDIT );				
								Edit_SetSel(rededit, 0, -1);
								Edit_ReplaceSel(rededit, (LPARAM)data);
								
								_snprintf( data, sizeof(data), "%3.3f", green );
								HWND greenedit = GetDlgItem( hDlg, IDC_GREENEDIT );				
								Edit_SetSel(greenedit, 0, -1);
								Edit_ReplaceSel(greenedit, (LPARAM)data);
								
								_snprintf( data, sizeof(data), "%3.3f", blue );
								HWND blueedit = GetDlgItem( hDlg, IDC_BLUEEDIT );				
								Edit_SetSel(blueedit, 0, -1);
								Edit_ReplaceSel(blueedit, (LPARAM)data);
								
								_snprintf( data, sizeof(data), "%3.3f", alpha );
								HWND alphaedit = GetDlgItem( hDlg, IDC_ALPHAEDIT );				
								Edit_SetSel(alphaedit, 0, -1);
								Edit_ReplaceSel(alphaedit, (LPARAM)data);
							}
							else 
							{
								TCHAR data[20];
								memset(data, 0, sizeof(data));

								_snprintf( data, sizeof(data), "%3.3f", red/255.0f );
								HWND rededit = GetDlgItem( hDlg, IDC_REDEDIT );				
								Edit_SetSel(rededit, 0, -1);
								Edit_ReplaceSel(rededit, (LPARAM)data);
								
								_snprintf( data, sizeof(data), "%3.3f", green/255.0f );
								HWND greenedit = GetDlgItem( hDlg, IDC_GREENEDIT );				
								Edit_SetSel(greenedit, 0, -1);
								Edit_ReplaceSel(greenedit, (LPARAM)data);

								_snprintf( data, sizeof(data), "%3.3f", blue/255.0f );
								HWND blueedit = GetDlgItem( hDlg, IDC_BLUEEDIT );				
								Edit_SetSel(blueedit, 0, -1);
								Edit_ReplaceSel(blueedit, (LPARAM)data);

								_snprintf( data, sizeof(data), "%3.3f", alpha/255.0f );
								HWND alphaedit = GetDlgItem( hDlg, IDC_ALPHAEDIT );				
								Edit_SetSel(alphaedit, 0, -1);
								Edit_ReplaceSel(alphaedit, (LPARAM)data);
							}
						}
					}
				}
				break;					
			case IDC_COMMANDHISTORYLIST :
				{
					switch( HIWORD(wparam) )
					{
					case LBN_DBLCLK  :
						int index = SendMessage( hwndCtl, LB_GETCURSEL, 0, 0 );
						TCHAR str[512];
						memset( str, 0, sizeof(TCHAR)*512 );
						if( SendMessage( hwndCtl, LB_GETTEXT, (WPARAM)index, (LPARAM)str ) > 0 )
						{
							memset( g_LastCommandString, 0, 512 );
							strcpy( g_LastCommandString, str );
							_XKernelCommandProcedure( g_LastCommandString );
						}
					}
				}
				break;
			case IDC_RUNCOMMANDBUTTON :
				{ 			   
				TCHAR commandstring[512];
				memset( commandstring, 0, 512 );
				GetWindowText( g_hRenderStateControllerEditWindow, commandstring, 511 );
				SetWindowText(g_hRenderStateControllerEditWindow, "");

				if( strlen( commandstring ) > 0 )
				{
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, commandstring );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				return TRUE;
				}						
			case IDC_SCRINFOCHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("scrinfo") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_AXISCHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("axis") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_OBJECTVIEWCHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("object") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_FRUSTUMVIEWCHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("frustum") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_VIEWLENSFLARECHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("lens") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_OBBCHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("obb") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_LIGHTMODELCHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("lightmodel") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_TARGETPOSITIONCHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("movetargetpos") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_TERRAINRENDERTIMECHECK :
				{					
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("terrainrendertime") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_TERRAINNORMALCHECK :
				{
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("normal") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case IDC_USETEXTURECHECK :
				{
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("usetexture") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			case  IDC_MOUSELOCKCHECK :
				{
					memset( g_LastCommandString, 0, 512 );
					strcpy( g_LastCommandString, _T("mouselock") );
					_XKernelCommandProcedure( g_LastCommandString );
				}
				break;
			}
			}
			break;
	case WM_CLOSE:
		{			
			EndDialog( hDlg, IDOK );
		}
	  break;	
	case WM_DESTROY:
		{
			if( g_hSansSerifFont ) DeleteObject(g_hSansSerifFont);		
			RECT rect;
			GetWindowRect( g_hRenderStateControllerWindow, &rect );
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CONSOLEWINDOWPOSITIONX, rect.left );
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CONSOLEWINDOWPOSITIONY, rect.top );

			// Save last command...
			HWND hconsolewindowhistorylist = GetDlgItem( hDlg, IDC_COMMANDHISTORYLIST );
			if( hconsolewindowhistorylist )
			{
				int count = SendMessage( hconsolewindowhistorylist, LB_GETCOUNT, 0, 0 );
				TCHAR str[512];
				TCHAR mrucommandlistid[32];

				int startpoint = 0;

				if( count > 10 )
				{
					startpoint = count - 10;
				}

				for( int i = 0; i < 10; i++ )
				{
					memset( str, 0, sizeof(str) );
					_snprintf( mrucommandlistid, sizeof(mrucommandlistid), "Last command %d", i );

					if( count > i )
					{						
						SendMessage( hconsolewindowhistorylist, LB_GETTEXT, (WPARAM)startpoint+i, (LPARAM)str );
					}

					g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, mrucommandlistid, str );
				}				
			}		

			g_hRenderStateControllerWindow = NULL;
		}
		break;
	}

	return FALSE;
}

#endif