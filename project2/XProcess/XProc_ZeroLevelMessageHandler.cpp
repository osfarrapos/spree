#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XProc_ZeroLevel.h"
#include "XMeshContainer.h"
#include "XWindow_Debug.h"
#include "XWindow_Skill.h"
#include "xwindow_option.h" 
#include "XWindow_NameInput.h"
#include "XWindow_MaximumMap.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XwindowObjectDefine.h"


void XProc_ZeroLevel::FSWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	
}

void XProc_ZeroLevel::QuickSlotMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!g_pQuickSlot_Window)
		return;
	
	switch(wparam)
	{
	case _XDEF_QS_BATTLEMODEBUTTON :
		{
			if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE || g_pLocalUser->GetMotionClass() == _XACTION_MOVE)
			{
				if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE )
				{
					if((g_LocalSystemTime - g_pLocalUser->m_LastModeChangeTime) > 1000)
					{
						g_pLocalUser->SetBattleMode( _XBATTLE_PEACE );
						((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PEACE );
						g_pLocalUser->m_LastModeChangeTime = g_LocalSystemTime;
					}
				}
				else
				{
					if((g_LocalSystemTime - g_pLocalUser->m_LastModeChangeTime) > 1000)
					{
						g_pLocalUser->SetBattleMode( _XBATTLE_BATTLE );
						((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_BATTLE );				
						g_pLocalUser->m_LastModeChangeTime = g_LocalSystemTime;
					}
				}
			} // if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE || g_pLocalUser->GetMotionClass() == _XACTION_MOVE)
			
			g_pQuickSlot_Window->ChangePeaceBattleMode();
		}
		break;
	case _XDEF_QS_PREVBUTTON :
		{
			g_pQuickSlot_Window->SetSelectedSet(g_pQuickSlot_Window->GetSelectedSet()-1);
			if(g_pQuickSlot_Window->GetSelectedSet() < 0)
				g_pQuickSlot_Window->SetSelectedSet(2);
		}
		break;
	case _XDEF_QS_NEXTBUTTON :
		{
			g_pQuickSlot_Window->SetSelectedSet(g_pQuickSlot_Window->GetSelectedSet()+1);
			if(g_pQuickSlot_Window->GetSelectedSet() > 2)
				g_pQuickSlot_Window->SetSelectedSet(0);
		}
		break;
	case _XDEF_QS_SHORTCUT1BUTTON :
	case _XDEF_QS_SHORTCUT2BUTTON :
	case _XDEF_QS_SHORTCUT3BUTTON :
	case _XDEF_QS_SHORTCUT4BUTTON :
	case _XDEF_QS_SHORTCUT5BUTTON :
	case _XDEF_QS_SHORTCUT6BUTTON :
	case _XDEF_QS_SHORTCUT7BUTTON :
	case _XDEF_QS_SHORTCUT8BUTTON :
	case _XDEF_QS_SHORTCUT9BUTTON :
	case _XDEF_QS_SHORTCUT0BUTTON :
		{
			short skillid = (short)lparam;
			if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
			{
				if(_XSkillSelectCheckCallBack(0, skillid))
				{
					if(SelectActiveSkill(skillid))
						g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(wparam - _XDEF_QS_SHORTCUT1BUTTON);
				}
			}
		}
		break;
	}
}


void XProc_ZeroLevel::OptionMenuWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_OPTIONMENU_CLOSEBUTTON			:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{				
				optionmenuwindow->ShowWindow( FALSE );
			}
		}
		break;
	case _XDEF_OPTIONMENU_HELPBUTTON			:
		{

		}
		break;
	case _XDEF_OPTIONMENU_GRAPHICTABBUTTON		:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}
			
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_GRAPHIC );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}
		}
		break;
	case _XDEF_OPTIONMENU_SOUNDTABBUTTON		:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}

			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_SOUND );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}			
		}
		break;
	case _XDEF_OPTIONMENU_GAMETABBUTTON			:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}

			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_GAME );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}			
		}
		break;
	case _XDEF_OPTIONMENU_COMMUNITYTABBUTTON	:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}

			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_CONMMUNITY );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}
		}
		break;
	case _XDEF_OPTIONMENU_ANOTHORUSERTABBUTTON	:
		{
			if( !m_GameExitFlag )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_MESSAGEHANDLER_SELECTCHARACTER, TRUE, _XDEF_GOTOSELECTCHARACTER ); // 인물 선택으로
				//			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_MESSAGEHANDLER_SELECTCHARACTER, TRUE, _XDEF_GOTOLOGINSERVER ); // 인물 선택으로
			}
		}
		break;
	case _XDEF_OPTIONMENU_GOTOLOGINTABBUTTON	:
		{
			if( !m_GameExitFlag )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_GOTOLOGIN, TRUE, _XDEF_GOTOLOGINSERVER );
			}
		}
		break;
	case _XDEF_OPTIONMENU_EXITGAMETABBUTTON		:
		{
			if( !m_GameExitFlag )
			{
				//m_ProgramExitFlag = TRUE;
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_GAMECLOSE );
			}			
		}
		break;
	default:
		break;
	}
}


void XProc_ZeroLevel::OptionWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_OPTIONWINDOW_CLOSE :
		{
			_XWindow* optionwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ShowWindow( FALSE );
			}

			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( TRUE );
			}
		}
		break;	
	case _XDEF_OPTIONWINDOW_HELPBUTTON :
		{

		}
		break;
	case _XDEF_DLGTAB_GRAPHIC		 :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_GRAPHIC );
			}
		}
		break;	
	case _XDEF_DLGTAB_SOUND			 :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_SOUND );
			}
		}
		break;
	case _XDEF_DLGTAB_GAME			 :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_GAME );
			}
		}
		break;
	case _XDEF_DLGTAB_COMMUNITY		 : 
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_CONMMUNITY );
			}
		}
		break;
	
	// Screen option

	case _XDEF_OPTION_SCREEN_RESOLUTION		:
		{

		}
		break;
	case _XDEF_OPTION_SCREEN_DEPTHBUFFER		:
		{
			
		}
		break;
	case _XDEF_OPTION_SCREEN_GAMMA			:
		{
			FLOAT gammalevel = *((FLOAT*)&lparam) / 100.0f;

			g_fGamma = 0.5f + 1.5f * gammalevel;

			gApp->ApplyGammaRamp();
		}
		break;
	case _XDEF_OPTION_SCREEN_TERRAINLOD		:
		{
			FLOAT rate = *((FLOAT*)&lparam) / 100.0f;

			int modifyfactor = 0;
			if( rate > 0.75f )
			{
				modifyfactor = 50;
			}
			else if( rate > 0.5f )
			{
				modifyfactor = 0;				
			}
			else if( rate > 0.25f )
			{
				modifyfactor = -25;				
			}
			else
			{
				modifyfactor = -50;				
			}

			g_LodTerrain.m_LODLevel1 = g_LodTerrain.m_DefaultLODLevel1 + modifyfactor;
			g_LodTerrain.m_LODLevel2 = g_LodTerrain.m_DefaultLODLevel2 + modifyfactor;
			g_LodTerrain.m_LODLevel3 = g_LodTerrain.m_DefaultLODLevel3 + modifyfactor;

			g_XST_Wrapper.m_RenderLimitDistance = g_LodTerrain.m_LODLevel3;
			
			g_LodTerrain.RebuildLevel(FALSE);
		}
		break;		
	case _XDEF_OPTION_SCREEN_SIGHTRANGE		:
		{
			return;
			
			FLOAT rate = *((FLOAT*)&lparam) / 100.0f;
			rate += 0.5f;
			if( rate < 0.5f ) rate = 0.5f;
			if( rate > 1.0f ) rate = 1.0f;
									
			g_LodTerrain.m_LODFarPlane	= (250.0f * rate);
			//g_LodTerrain.m_FogFar	    = (250.0f * rate);
			//g_LodTerrain.m_FogDensity   = (0.0038f * rate);
			g_EnvironmentManager.m_EnvironmentLerpLevel = 2.0f;	

			g_LodTerrain.m_3PCamera.mp_fFarPlane = g_LodTerrain.m_LODFarPlane;
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo( g_LodTerrain.m_3PCamera );
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
		}
		break;
	case _XDEF_OPTION_SCREEN_GRASSQUALITY	:
		{
			FLOAT rate = *((FLOAT*)&lparam) / 100.0f;
			int count = (int)(30.0f * rate);
			
			if( count == 0 )
			{
				g_LodTerrain.m_RealTimeObjectGenerateCount = 0;
				g_bUseRealtimeGenerateObject = FALSE;
			}
			else
			{
				if( count < 4 ) count = 4;
				if( count > 15 ) g_LodTerrain.m_RealTimeObjectZDepthSorting = TRUE;
				else g_LodTerrain.m_RealTimeObjectZDepthSorting = FALSE;
				
				g_LodTerrain.m_RealTimeObjectGenerateCount = count;
				g_bUseRealtimeGenerateObject = TRUE;				
			}
		}
		break;
	case _XDEF_OPTION_SCREEN_GRASSANIM		:
		{
			g_bUseRealtimeGenerateObjectAni = (BOOL)lparam;
		}
		break;
	case _XDEF_OPTION_SCREEN_TREEANIM		:
		{
			
		}
		break;
	case _XDEF_OPTION_SCREEN_CHARACTORSHADOW	:
		{
			g_bCharacterShadow = (BOOL)lparam;
			
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{
				optionwindow->ConfirmDevice();
			}
		}
		break;
	case _XDEF_OPTION_SCREEN_USEBLOOMFILTER	:
		{
			int sliderfactor = (int)(*((FLOAT*)&lparam));
			
			if( sliderfactor == 0 )
				g_PostProcessManager.EnableBloomFilter( FALSE );
			else
			{
				FLOAT filterpower = 0.65f + ((FLOAT)sliderfactor/100.0f);
				g_PostProcessManager.m_FinalPixelShaderConst1[0] = 
				g_PostProcessManager.m_FinalPixelShaderConst1[1] = 
				g_PostProcessManager.m_FinalPixelShaderConst1[2] = 
				g_PostProcessManager.m_FinalPixelShaderConst1[3] = filterpower;
				
				TCHAR RegValue[_REGSTRING_MAXLENGTH];
				sprintf( RegValue, "%f", filterpower );
				g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BLOOMFILTERPOWER, RegValue );
				
				g_PostProcessManager.EnableBloomFilter( TRUE );
			}
		}
		break;
	case _XDEF_OPTION_SCREEN_EFFECTQUALITY	:
		{
			int tracecount = 2 + (int)(18.0f * (*((FLOAT*)&lparam) / 100.0f));
			_XEffect_SwordTrace::SetPointStep( tracecount );			
		}
		break;
	case _XDEF_OPTION_SCREEN_REFLECTION		:
		{
			CWater::m_UseObjectReflection = (BOOL)lparam;
		}
		break;
	case _XDEF_OPTION_SCREEN_LENSFLARE		:
		{
			g_EnvironmentManager.m_ViewLensFlare = (BOOL)lparam;
		}
		break;
	case _XDEF_OPTION_SOUND_USEBGM				:
		{
			g_BgmPlayFlag = (BOOL)lparam;

#ifdef _XUSEFMOD
			// FMODProc
#else
			if( g_BgmPlayFlag )
			{
				if( g_pBGMSoundObject[0] )
					g_pBGMSoundObject[0]->Play();
				else
				{
					SoundInit initsound;
					initsound.m_bLooping = false;
					initsound.m_bStreaming = false;
					initsound.m_bMusic = true;
					
					initsound.m_sFileName = _T( gModulePath );
					initsound.m_sFileName += _T("\\Data\\Sound\\BGM\\");
					initsound.m_sFileName += _T("zerolevelBGM.ogg");
															
					if(AudioMgr()->CreateSound(g_pBGMSoundObject[0]))
					{
						if(g_pBGMSoundObject[0]->Init(initsound))
						{
							g_pBGMSoundObject[0]->Play();
						}
					}
				}
			}
			else
			{
				if( g_pBGMSoundObject[0] )
				{					
					g_pBGMSoundObject[0]->Stop();
				}
			}		
#endif
		}
		break;
	case _XDEF_OPTION_SOUND_BGMVOLUME			:
		{
			FLOAT volume = *((FLOAT*)&lparam) / 100.0f;
#ifdef _XUSEFMOD
			// FMODProc
#else	
			if( g_pBGMSoundObject[0] )
				g_pBGMSoundObject[0]->SetVolume( volume );
#endif
		}
		break;
	case _XDEF_OPTION_SOUND_USEEFFECTSOUND		:
		{
			g_SoundEffectFlag = (BOOL)lparam;			
		}
		break;
	case _XDEF_OPTION_SOUND_EFFECTSOUNDVOLUME   :
		{
			FLOAT volume = *((FLOAT*)&lparam) / 100.0f;
#ifdef _XUSEFMOD
		// FMODProc
#else
			for( int i = 0; i < g_MaxSoundEffectCount; i++ )
			{	
				g_pSoundEffect[i]->SetVolume( volume );
			}
#endif
		}
		break;
	case _XDEF_OPTION_SOUND_USEENVSOUND			:
		{
			
		}
		break;
	case _XDEF_OPTION_SOUND_ENVVOLUMESLIDER		:
		{
			
		}
		break;
	case _XDEF_OPTION_SOUND_USEGUIDESOUND		:
		{

		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTIONWHISPER :
		{
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTFRIEND     :
		{
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTPARTY		:
		{
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTORGANIZATION	:
		{
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYEXCHANGE		    :
		{
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYCONNECTMESSENGER :
		{
			
		}
		break;
	case _XDEF_OPTION_GAME_GAMEGUIDE			:
		{
			
		}
		break;
	case _XDEF_OPTION_GAME_VIEWUSERTOOLTIP			:
		{
			g_AlwaysViewUserNameAndItemTooltip = (BOOL)lparam;
			
			if( !g_AlwaysViewUserNameAndItemTooltip )
			{
				g_UsernameTooltipViewTimer = 0;
				g_ItemnameTooltipViewTimer = 0;
			}
		}
		break;
	case _XDEF_OPTION_GAME_HELPSYSTEM			:
		{
			
		}
		break;
	case _XDEF_OPTION_GAME_CAMERAQUATERVIEW		:
		{
			
		}
		break;
	case _XDEF_OPTION_GAME_USEFIRSTPERSONVIEW	:			
		{
			
		}
		break;
	}
}

void XProc_ZeroLevel::MinimapWindowMessageHandling( WPARAM wparam, LPARAM lparam )
{
	switch(wparam)
	{
	case _XDEF_MINIMAP_CLOSEBUTTON :
		{
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{					
				pminimapwindow->ChangeMinimizeStatus();
			}						 
		}
		break;
	case _XDEF_MINIMAP_ZOOMINBUTTON :
		{
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{					
				pminimapwindow->m_MinimapZoomRate -= 0.004f;
				
				if( pminimapwindow->m_MinimapZoomRate < 0.06f )
					pminimapwindow->m_MinimapZoomRate = 0.06f;
				
				pminimapwindow->SetLocalUserPosition();
			}
		}
		break;
	case _XDEF_MINIMAP_ZOOMOUTBUTTON :
		{
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{					
				pminimapwindow->m_MinimapZoomRate += 0.004f;
				
				if( pminimapwindow->m_MinimapZoomRate > 0.2f )
					pminimapwindow->m_MinimapZoomRate = 0.2f;
				
				pminimapwindow->SetLocalUserPosition();
			}
		}
		break;	
	case _XDEF_MINIMAP_ZOOMMAXBUTTON :
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				pmaxminimapwindow->ShowWindow( !pmaxminimapwindow->GetShowStatus() );
			}
		}
		break;
	case _XDEF_MAXMINIMAP_CLOSEBUTTON		:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				pmaxminimapwindow->ShowWindow( FALSE );
			}
		}
		break;
	case _XDEF_MAXMINIMAP_NPCVIEWBUTTON		:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				
			}
		}
		break;
	case _XDEF_MAXMINIMAP_PARTYVIEWBUTTON	:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				
			}
		}
		break;
	case _XDEF_MAXMINIMAP_APPLYBUTTON		:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				
			}
		}
		break;
	}
}

void XProc_ZeroLevel::SystemMenuWindowMessageHandling( WPARAM wparam, LPARAM lparam )
{
	/*
	switch(wparam)
	{
		default :
			break;
	}
	*/
}



