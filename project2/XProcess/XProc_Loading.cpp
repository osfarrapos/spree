// XProc_Loading.cpp: implementation of the XProc_Loading class.
//
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Base
#include "stdafx.h"
#include "resource.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"

///////////////////////////////////////////////////////////////////////
// Network

///////////////////////////////////////////////////////////////////////
// Process
#include "XProc_ProcessDefine.h"
#include "XProc_MainGameDef.h"
#include "XProc_MainGame.h"
#include "XWindow_MaximumMap.h"

///////////////////////////////////////////////////////////////////////
// Pre.. define
#include "SoundEffectList.h"
#define _XDEF_LOADINGBGM					"Data\\Sound\\Bgm\\LoginBGM.mp3"

#include "XProc_Loading.h"
#include "XSR_STRINGHEADER.H"
#include "../Library/CWebPage/XWebPage.h"

extern CFlashControlInterface*	g_pTitleFlashObject;
extern LPDIRECT3DTEXTURE9		g_pTitleFlashOffScreenTexture;
extern FLOAT					g_fWaterDepth; // 수심 몇미터까지 들어갈수 있다.

#ifdef _ACCLAIM_IGAADSYSTEM
BOOL	g_LoadingProcess_ViewAD = TRUE;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_Loading::XProc_Loading()
{	
	setProcessInfo( 0, "Loading data", __FILE__ );

#ifdef _ACCLAIM_IGAADSYSTEM
	m_AcclaimADPhase	=	_XACCLAIMADPHASE_END;
	m_AutoCancelCounter =   0;	
	m_pLoadingAD_ADIteminfo = NULL;
#endif
}

XProc_Loading::~XProc_Loading()
{
#ifdef _ACCLAIM_IGAADSYSTEM
	SAFE_DELETE_ARRAY(m_pLoadingAD_ADIteminfo);	
#endif
}

BOOL XProc_Loading::InitializeProcess( void )
{	
#ifdef _XDWDEBUG 
	g_NormalGameExit = TRUE;
#else 
	g_NormalGameExit = FALSE;
#endif
	
	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->FreeBuffer();
		g_pTitleFlashObject->ControlClose();
		delete g_pTitleFlashObject;
		g_pTitleFlashObject = NULL;
	}
	if( g_pTitleFlashOffScreenTexture )
	{
		g_pTitleFlashOffScreenTexture->Release();
		g_pTitleFlashOffScreenTexture = NULL;
	}
	//SAFE_RELEASE( g_pTitleFlashOffScreenTexture );
	
	// Default player position
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

	TCHAR backgroundname[4][32] = { _T("MI_FirstLoading.jpg"),
								 _T("MI_FirstLoading.jpg"),
								 _T("MI_FirstLoading.jpg"),
								 _T("MI_FirstLoading.jpg") };
	

	TCHAR existbackgroundimagename[26][64] ={ _T("LoadingBG_map_gaebang.jpg"),
										   _T("LoadingBG_map_jungju.jpg"),
										   _T("LoadingBG_map_noklim.jpg"),
										   _T("LoadingBG_map_beegoong.jpg"),
										   _T("LoadingBG_map_seokgaja.jpg"),
										   _T("LoadingBG_map_sorim.jpg"),
										   _T("LoadingBG_map_namchang.jpg"),
										   _T("LoadingBG_map_daedong.jpg"),
										   _T("LoadingBG_map_jaenam.jpg"),
										   _T("LoadingBG_map_namchang_boss.jpg"),
										   _T("LoadingBG_map_jungju_boss.jpg"),
										   _T("LoadingBG_map_seokgaja_boss.jpg"),
										   _T("LoadingBG_map_daedong_boss.jpg"),
										   _T("LoadingBG_map_jaenam_boss.jpg"),
										   _T("LoadingBG_map_hangjoo.jpg"),
										   _T("LoadingBG_map_hyulsapyeong.jpg"), 
										   _T("LoadingBG_map_moodang.jpg"),
										   _T("LoadingBG_map_magyo.jpg"),
										   _T("LoadingBG_map_geomgangdong.jpg"),
										   _T("LoadingBG_map_yongmoondong.jpg"),
										   _T("LoadingBG_map_hapbi.jpg"),
										   _T("LoadingBG_map_hyulsapyung.jpg"),
										   _T("LoadingBG_map_Paewangdong.jpg"),
										   _T("LoadingBG_map_Yodong.jpg"),
										   _T("LoadingBG_map_Tutorial.jpg"),
										   _T("LoadingBG_map_bookhae.jpg")
									};
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( existbackgroundimagename[0], _T("vn_LoadingBG_map_gaebang.jpg") );
		strcpy( existbackgroundimagename[1], _T("vn_LoadingBG_map_jungju.jpg") );
		strcpy( existbackgroundimagename[2], _T("vn_LoadingBG_map_noklim.jpg") );
		strcpy( existbackgroundimagename[3], _T("vn_LoadingBG_map_beegoong.jpg") );
		strcpy( existbackgroundimagename[4], _T("vn_LoadingBG_map_seokgaja.jpg") );
		strcpy( existbackgroundimagename[5], _T("vn_LoadingBG_map_sorim.jpg") );
		strcpy( existbackgroundimagename[6], _T("vn_LoadingBG_map_namchang.jpg") );
		strcpy( existbackgroundimagename[7], _T("vn_LoadingBG_map_daedong.jpg") );
		strcpy( existbackgroundimagename[8], _T("vn_LoadingBG_map_jaenam.jpg") );
		strcpy( existbackgroundimagename[9], _T("vn_LoadingBG_map_namchang_boss.jpg") );
		strcpy( existbackgroundimagename[10], _T("vn_LoadingBG_map_jungju_boss.jpg") );
		strcpy( existbackgroundimagename[11], _T("vn_LoadingBG_map_seokgaja_boss.jpg") );
		strcpy( existbackgroundimagename[12], _T("vn_LoadingBG_map_daedong_boss.jpg") );
		strcpy( existbackgroundimagename[13], _T("vn_LoadingBG_map_jaenam_boss.jpg") );
		strcpy( existbackgroundimagename[14], _T("vn_LoadingBG_map_hangjoo.jpg") );
		strcpy( existbackgroundimagename[15], _T("vn_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[16], _T("vn_LoadingBG_map_moodang.jpg") );
		strcpy( existbackgroundimagename[17], _T("vn_LoadingBG_map_magyo.jpg") );
		strcpy( existbackgroundimagename[18], _T("vn_LoadingBG_map_geomgangdong.jpg") );
		strcpy( existbackgroundimagename[19], _T("vn_LoadingBG_map_yongmoondong.jpg") );
		strcpy( existbackgroundimagename[20], _T("vn_LoadingBG_map_hapbi.jpg") );
		strcpy( existbackgroundimagename[21], _T("vn_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[22], _T("vn_LoadingBG_map_Paewangdong.jpg") );
		strcpy( existbackgroundimagename[23], _T("vn_LoadingBG_map_Yodong.jpg"));
		strcpy( existbackgroundimagename[24], _T("vn_LoadingBG_map_Tutorial.jpg"));		// 튜토리얼 맵 이미지 필요
		strcpy( existbackgroundimagename[25], _T("vn_LoadingBG_map_Bookhae.jpg"));


		strcpy( backgroundname[0], _T("vn_MI_FirstLoading.jpg") );
		strcpy( backgroundname[1], _T("vn_MI_FirstLoading.jpg") );
		strcpy( backgroundname[2], _T("vn_MI_FirstLoading.jpg") );
		strcpy( backgroundname[3], _T("vn_MI_FirstLoading.jpg") );
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( existbackgroundimagename[0], _T("us_LoadingBG_map_gaebang.jpg") );
		strcpy( existbackgroundimagename[1], _T("us_LoadingBG_map_jungju.jpg") );
		strcpy( existbackgroundimagename[2], _T("us_LoadingBG_map_noklim.jpg") );
		strcpy( existbackgroundimagename[3], _T("us_LoadingBG_map_beegoong.jpg") );
		strcpy( existbackgroundimagename[4], _T("us_LoadingBG_map_seokgaja.jpg") );
		strcpy( existbackgroundimagename[5], _T("us_LoadingBG_map_sorim.jpg") );
		strcpy( existbackgroundimagename[6], _T("us_LoadingBG_map_namchang.jpg") );
		strcpy( existbackgroundimagename[7], _T("us_LoadingBG_map_daedong.jpg") );
		strcpy( existbackgroundimagename[8], _T("us_LoadingBG_map_jaenam.jpg") );
		strcpy( existbackgroundimagename[9], _T("us_LoadingBG_map_namchang_boss.jpg") );
		strcpy( existbackgroundimagename[10], _T("us_LoadingBG_map_jungju_boss.jpg") );
		strcpy( existbackgroundimagename[11], _T("us_LoadingBG_map_seokgaja_boss.jpg") );
		strcpy( existbackgroundimagename[12], _T("us_LoadingBG_map_daedong_boss.jpg") );
		strcpy( existbackgroundimagename[13], _T("us_LoadingBG_map_jaenam_boss.jpg") );
		strcpy( existbackgroundimagename[14], _T("us_LoadingBG_map_hangjoo.jpg") );
		strcpy( existbackgroundimagename[15], _T("us_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[16], _T("us_LoadingBG_map_moodang.jpg") );
		strcpy( existbackgroundimagename[17], _T("us_LoadingBG_map_magyo.jpg") );
		strcpy( existbackgroundimagename[18], _T("us_LoadingBG_map_geomgangdong.jpg") );
		strcpy( existbackgroundimagename[19], _T("us_LoadingBG_map_yongmoondong.jpg") );
		strcpy( existbackgroundimagename[20], _T("us_LoadingBG_map_hapbi.jpg") );
		strcpy( existbackgroundimagename[21], _T("us_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[22], _T("us_LoadingBG_map_Paewangdong.jpg") );
		strcpy( existbackgroundimagename[23], _T("us_LoadingBG_map_Yodong.jpg"));
		strcpy( existbackgroundimagename[24], _T("us_LoadingBG_map_Tutorial.jpg"));		// 튜토리얼 맵 이미지 필요
		strcpy( existbackgroundimagename[25], _T("us_LoadingBG_map_Bookhae.jpg"));
		
		
		strcpy( backgroundname[0], _T("us_MI_FirstLoading.jpg") );
		strcpy( backgroundname[1], _T("us_MI_FirstLoading.jpg") );
		strcpy( backgroundname[2], _T("us_MI_FirstLoading.jpg") );
		strcpy( backgroundname[3], _T("us_MI_FirstLoading.jpg") );
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( existbackgroundimagename[0], _T("tw_LoadingBG_map_gaebang.jpg") );
		strcpy( existbackgroundimagename[1], _T("tw_LoadingBG_map_jungju.jpg") );
		strcpy( existbackgroundimagename[2], _T("tw_LoadingBG_map_noklim.jpg") );
		strcpy( existbackgroundimagename[3], _T("tw_LoadingBG_map_beegoong.jpg") );
		strcpy( existbackgroundimagename[4], _T("tw_LoadingBG_map_seokgaja.jpg") );
		strcpy( existbackgroundimagename[5], _T("tw_LoadingBG_map_sorim.jpg") );
		strcpy( existbackgroundimagename[6], _T("tw_LoadingBG_map_namchang.jpg") );
		strcpy( existbackgroundimagename[7], _T("tw_LoadingBG_map_daedong.jpg") );
		strcpy( existbackgroundimagename[8], _T("tw_LoadingBG_map_jaenam.jpg") );
		strcpy( existbackgroundimagename[9], _T("tw_LoadingBG_map_namchang_boss.jpg") );
		strcpy( existbackgroundimagename[10], _T("tw_LoadingBG_map_jungju_boss.jpg") );
		strcpy( existbackgroundimagename[11], _T("tw_LoadingBG_map_seokgaja_boss.jpg") );
		strcpy( existbackgroundimagename[12], _T("tw_LoadingBG_map_daedong_boss.jpg") );
		strcpy( existbackgroundimagename[13], _T("tw_LoadingBG_map_jaenam_boss.jpg") );
		strcpy( existbackgroundimagename[14], _T("tw_LoadingBG_map_hangjoo.jpg") );
		strcpy( existbackgroundimagename[15], _T("tw_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[16], _T("tw_LoadingBG_map_moodang.jpg") );
		strcpy( existbackgroundimagename[17], _T("tw_LoadingBG_map_magyo.jpg") );
		strcpy( existbackgroundimagename[18], _T("tw_LoadingBG_map_geomgangdong.jpg") );
		strcpy( existbackgroundimagename[19], _T("tw_LoadingBG_map_yongmoondong.jpg") );
		strcpy( existbackgroundimagename[20], _T("tw_LoadingBG_map_hapbi.jpg") );
		strcpy( existbackgroundimagename[21], _T("tw_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[22], _T("tw_LoadingBG_map_Paewangdong.jpg") );
		strcpy( existbackgroundimagename[23], _T("tw_LoadingBG_map_Yodong.jpg"));
		strcpy( existbackgroundimagename[24], _T("tw_LoadingBG_map_Tutorial.jpg"));		// 튜토리얼 맵 이미지 필요
		strcpy( existbackgroundimagename[25], _T("tw_LoadingBG_map_Bookhae.jpg"));
		
		
		strcpy( backgroundname[0], _T("tw_MI_FirstLoading.jpg") );
		strcpy( backgroundname[1], _T("tw_MI_FirstLoading.jpg") );
		strcpy( backgroundname[2], _T("tw_MI_FirstLoading.jpg") );
		strcpy( backgroundname[3], _T("tw_MI_FirstLoading.jpg") );
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( existbackgroundimagename[0], _T("jp_LoadingBG_map_gaebang.jpg") );
		strcpy( existbackgroundimagename[1], _T("jp_LoadingBG_map_jungju.jpg") );
		strcpy( existbackgroundimagename[2], _T("jp_LoadingBG_map_noklim.jpg") );
		strcpy( existbackgroundimagename[3], _T("jp_LoadingBG_map_beegoong.jpg") );
		strcpy( existbackgroundimagename[4], _T("jp_LoadingBG_map_seokgaja.jpg") );
		strcpy( existbackgroundimagename[5], _T("jp_LoadingBG_map_sorim.jpg") );
		strcpy( existbackgroundimagename[6], _T("jp_LoadingBG_map_namchang.jpg") );
		strcpy( existbackgroundimagename[7], _T("jp_LoadingBG_map_daedong.jpg") );
		strcpy( existbackgroundimagename[8], _T("jp_LoadingBG_map_jaenam.jpg") );
		strcpy( existbackgroundimagename[9], _T("jp_LoadingBG_map_namchang_boss.jpg") );
		strcpy( existbackgroundimagename[10], _T("jp_LoadingBG_map_jungju_boss.jpg") );
		strcpy( existbackgroundimagename[11], _T("jp_LoadingBG_map_seokgaja_boss.jpg") );
		strcpy( existbackgroundimagename[12], _T("jp_LoadingBG_map_daedong_boss.jpg") );
		strcpy( existbackgroundimagename[13], _T("jp_LoadingBG_map_jaenam_boss.jpg") );
		strcpy( existbackgroundimagename[14], _T("jp_LoadingBG_map_hangjoo.jpg") );
		strcpy( existbackgroundimagename[15], _T("jp_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[16], _T("jp_LoadingBG_map_moodang.jpg") );
		strcpy( existbackgroundimagename[17], _T("jp_LoadingBG_map_magyo.jpg") );
		strcpy( existbackgroundimagename[18], _T("jp_LoadingBG_map_geomgangdong.jpg") );
		strcpy( existbackgroundimagename[19], _T("jp_LoadingBG_map_yongmoondong.jpg") );
		strcpy( existbackgroundimagename[20], _T("jp_LoadingBG_map_hapbi.jpg") );
		strcpy( existbackgroundimagename[21], _T("jp_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[22], _T("jp_LoadingBG_map_Paewangdong.jpg") );
		strcpy( existbackgroundimagename[23], _T("jp_LoadingBG_map_Yodong.jpg"));
		strcpy( existbackgroundimagename[24], _T("jp_LoadingBG_map_Tutorial.jpg"));		// 튜토리얼 맵 이미지 필요
		strcpy( existbackgroundimagename[25], _T("jp_LoadingBG_map_Bookhae.jpg"));
		
		
		strcpy( backgroundname[0], _T("jp_MI_FirstLoading.jpg") );
		strcpy( backgroundname[1], _T("jp_MI_FirstLoading.jpg") );
		strcpy( backgroundname[2], _T("jp_MI_FirstLoading.jpg") );
		strcpy( backgroundname[3], _T("jp_MI_FirstLoading.jpg") );
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( existbackgroundimagename[0], _T("rs_LoadingBG_map_gaebang.jpg") );
		strcpy( existbackgroundimagename[1], _T("rs_LoadingBG_map_jungju.jpg") );
		strcpy( existbackgroundimagename[2], _T("rs_LoadingBG_map_noklim.jpg") );
		strcpy( existbackgroundimagename[3], _T("rs_LoadingBG_map_beegoong.jpg") );
		strcpy( existbackgroundimagename[4], _T("rs_LoadingBG_map_seokgaja.jpg") );
		strcpy( existbackgroundimagename[5], _T("rs_LoadingBG_map_sorim.jpg") );
		strcpy( existbackgroundimagename[6], _T("rs_LoadingBG_map_namchang.jpg") );
		strcpy( existbackgroundimagename[7], _T("rs_LoadingBG_map_daedong.jpg") );
		strcpy( existbackgroundimagename[8], _T("rs_LoadingBG_map_jaenam.jpg") );
		strcpy( existbackgroundimagename[9], _T("rs_LoadingBG_map_namchang_boss.jpg") );
		strcpy( existbackgroundimagename[10], _T("rs_LoadingBG_map_jungju_boss.jpg") );
		strcpy( existbackgroundimagename[11], _T("rs_LoadingBG_map_seokgaja_boss.jpg") );
		strcpy( existbackgroundimagename[12], _T("rs_LoadingBG_map_daedong_boss.jpg") );
		strcpy( existbackgroundimagename[13], _T("rs_LoadingBG_map_jaenam_boss.jpg") );
		strcpy( existbackgroundimagename[14], _T("rs_LoadingBG_map_hangjoo.jpg") );
		strcpy( existbackgroundimagename[15], _T("rs_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[16], _T("rs_LoadingBG_map_moodang.jpg") );
		strcpy( existbackgroundimagename[17], _T("rs_LoadingBG_map_magyo.jpg") );
		strcpy( existbackgroundimagename[18], _T("rs_LoadingBG_map_geomgangdong.jpg") );
		strcpy( existbackgroundimagename[19], _T("rs_LoadingBG_map_yongmoondong.jpg") );
		strcpy( existbackgroundimagename[20], _T("rs_LoadingBG_map_hapbi.jpg") );
		strcpy( existbackgroundimagename[21], _T("rs_LoadingBG_map_hyulsapyung.jpg") );
		strcpy( existbackgroundimagename[22], _T("rs_LoadingBG_map_Paewangdong.jpg") );
		strcpy( existbackgroundimagename[23], _T("rs_LoadingBG_map_Yodong.jpg"));
		strcpy( existbackgroundimagename[24], _T("rs_LoadingBG_map_Tutorial.jpg"));		// 튜토리얼 맵 이미지 필요
		strcpy( existbackgroundimagename[25], _T("rs_LoadingBG_map_Bookhae.jpg"));
		
		
		strcpy( backgroundname[0], _T("rs_MI_FirstLoading.jpg") );
		strcpy( backgroundname[1], _T("rs_MI_FirstLoading.jpg") );
		strcpy( backgroundname[2], _T("rs_MI_FirstLoading.jpg") );
		strcpy( backgroundname[3], _T("rs_MI_FirstLoading.jpg") );
	}	

	TCHAR loadingimagenamestr[_MAX_PATH];
	memset( loadingimagenamestr, 0, _MAX_PATH );
	
	BOOL ExistLoadingImage = FALSE;
	// 항주 남창 로딩 이미지 특별 처리 
	if( g_CurrentZoneInfoIndex >= 54)
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			if( g_CurrentZoneInfoIndex == 54 )
			{
				strcpy( loadingimagenamestr, "vn_LoadingBG_map_namchang_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 55 )
			{
				strcpy( loadingimagenamestr, "vn_LoadingBG_map_hangjoo_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 56 )
			{
				strcpy( loadingimagenamestr, "vn_LoadingBG_map_hangjoo_main_boss1.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 57 )
			{
				strcpy( loadingimagenamestr, "vn_LoadingBG_map_namchang_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 58 ) //Author : 양희왕 //breif : 승천결 로딩 이미지
			{
				strcpy( loadingimagenamestr, "vn_LoadingBG_map_yodong_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		{
			if( g_CurrentZoneInfoIndex == 54 )
			{
				strcpy( loadingimagenamestr, "tw_LoadingBG_map_namchang_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 55 )
			{
				strcpy( loadingimagenamestr, "tw_LoadingBG_map_hangjoo_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 56 )
			{
				strcpy( loadingimagenamestr, "tw_LoadingBG_map_hangjoo_main_boss1.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 57 )
			{
				strcpy( loadingimagenamestr, "tw_LoadingBG_map_namchang_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 58 ) //Author : 양희왕 //breif : 승천결 로딩 이미지
			{
				strcpy( loadingimagenamestr, "tw_LoadingBG_map_yodong_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		{
			if( g_CurrentZoneInfoIndex == 54 )
			{
				strcpy( loadingimagenamestr, "us_LoadingBG_map_namchang_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 55 )
			{
				strcpy( loadingimagenamestr, "us_LoadingBG_map_hangjoo_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 56 )
			{
				strcpy( loadingimagenamestr, "us_LoadingBG_map_hangjoo_main_boss1.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 57 )
			{
				strcpy( loadingimagenamestr, "us_LoadingBG_map_namchang_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 58 ) //Author : 양희왕 //breif : 승천결 로딩 이미지
			{
				strcpy( loadingimagenamestr, "us_LoadingBG_map_yodong_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			if( g_CurrentZoneInfoIndex == 54 )
			{
				strcpy( loadingimagenamestr, "jp_LoadingBG_map_namchang_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 55 )
			{
				strcpy( loadingimagenamestr, "jp_LoadingBG_map_hangjoo_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 56 )
			{
				strcpy( loadingimagenamestr, "jp_LoadingBG_map_hangjoo_main_boss1.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 57 )
			{
				strcpy( loadingimagenamestr, "jp_LoadingBG_map_namchang_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 58 ) //Author : 양희왕 //breif : 승천결 로딩 이미지
			{
				strcpy( loadingimagenamestr, "jp_LoadingBG_map_yodong_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			if( g_CurrentZoneInfoIndex == 54 )
			{
				strcpy( loadingimagenamestr, "rs_LoadingBG_map_namchang_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 55 )
			{
				strcpy( loadingimagenamestr, "rs_LoadingBG_map_hangjoo_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 56 )
			{
				strcpy( loadingimagenamestr, "rs_LoadingBG_map_hangjoo_main_boss1.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 57 )
			{
				strcpy( loadingimagenamestr, "rs_LoadingBG_map_namchang_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 58 ) //Author : 양희왕 //breif : 승천결 로딩 이미지
			{
				strcpy( loadingimagenamestr, "rs_LoadingBG_map_yodong_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
		}
		else
		{
			if( g_CurrentZoneInfoIndex == 54 )
			{
				strcpy( loadingimagenamestr, "LoadingBG_map_namchang_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 55 )
			{
				strcpy( loadingimagenamestr, "LoadingBG_map_hangjoo_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 56 )
			{
				strcpy( loadingimagenamestr, "LoadingBG_map_hangjoo_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 57 )
			{
				strcpy( loadingimagenamestr, "LoadingBG_map_namchang_main_boss2.jpg" );
				ExistLoadingImage = TRUE;
			}
			else if( g_CurrentZoneInfoIndex == 58 ) //Author : 양희왕 //breif : 승천결 로딩 이미지
			{
				strcpy( loadingimagenamestr, "LoadingBG_map_yodong_boss.jpg" );
				ExistLoadingImage = TRUE;
			}
		}
	}
	else
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			strcpy( loadingimagenamestr, "vn_LoadingBG_" );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		{
			strcpy( loadingimagenamestr, "us_LoadingBG_" );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		{
			strcpy( loadingimagenamestr, "tw_LoadingBG_" );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			strcpy( loadingimagenamestr, "jp_LoadingBG_" );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			strcpy( loadingimagenamestr, "rs_LoadingBG_" );
		}
		else
		{
			strcpy( loadingimagenamestr, "LoadingBG_" );
		}

		strcat( loadingimagenamestr, g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonefilename );
		LPTSTR cutpoint = strstr( loadingimagenamestr, "xtv" );
		if( cutpoint )
		{
			*cutpoint = NULL;
		}
		strcat( loadingimagenamestr, "jpg");
		
		for( int existimageindex = 0; existimageindex < 26; existimageindex++ )
		{
			if( strcmpi( loadingimagenamestr, existbackgroundimagename[existimageindex] ) == 0 )
			{
				ExistLoadingImage = TRUE;
				break;
			}
		}
	}
		
	if( ExistLoadingImage )
	{
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex( loadingimagenamestr, _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
	}
	else
	{
		//m_BackgroundStatic.Create( (gnWidth>>1)-400, (gnHeight>>1)-250, (gnWidth>>1)+400, (gnHeight>>1)+250, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.Create( 0, 0, gnWidth-1, gnHeight-1, _XDEF_DEFAULTDEPTH_FORINTERFACE );
		m_BackgroundStatic.SetTexture( &m_TextureArchive, m_TextureArchive.GetResourceIndex( backgroundname[timeGetTime()%4], _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE) );
	}
	
	int resourceindex = -1;

	m_ProgressStatic.Create( (gnWidth>>1) - 151, (gnHeight) - 33, (gnWidth>>1) + 151, (gnHeight) - 19,
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
	
	InitDeviceObject();

	SetMessage( "Loading...." );
		
	m_DataLoaded = FALSE;
	m_Progress   = 0;

	m_SiganalLastSendedTime = 0;
	
	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );
	g_MessageBox.ShowWindow( FALSE );		

#ifdef _XDEF_MADSYSTEM

	if( g_CurrentZoneInfoIndex >= 54)
	{
		g_MadWrapper.SetMADLocation( _T("Dungeon Transition") );
	}
	else
	{
		g_MadWrapper.SetMADLocation( _T("MapTransition") );
	}

	Draw();

	_XLog( _T("_XMADWrapper : Wait for download complete...") );
	while(1)
	{
		g_MadWrapper.Update();
		if( g_MadWrapper.GetDownloadComplete() )
		{
			break;
		}
		Sleep( 1 );
	}
	_XLog( _T("_XMADWrapper : Media download complete.") );

#endif

#ifdef _ACCLAIM_IGAADSYSTEM
	if( g_CurrentZoneInfoIndex == 22 || g_QuestScriptManager.m_indexVirtualZone == 22 ) // tutorial mode에서는 watch & win 광고 처리 안함.
	{
		m_AcclaimADPhase				=	_XACCLAIMADPHASE_END;
	}
	else
	{
		m_AcclaimADPhase				=	_XACCLAIMADPHASE_WATCHANDWINBUTTON;
	}
	
	m_WatchAndWinResultViewTimer	=	0;
	m_MessageBoxAlphaLevel			=	1.0f;
	m_AutoCancelCounter				=	0;
	m_RecievedADResult				=	FALSE;	
	m_LoadingAD_Gold				=	0;
	m_LoadingAD_ItemCnt				=	0;
	SAFE_DELETE_ARRAY(m_pLoadingAD_ADIteminfo);

	m_ADRecievedResultAutoCancelCounter = 0;

	resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_death01.tga");
	_XBTN_STRUCTURE watchandwinbtnstruct = 
	{ 
		TRUE, { (gnWidth>>1) - (202>>1), (gnHeight) - 80 }, 
		{ 202, 22 }, _XDEF_LOADINGBTN_WATCHANDWINBUTTON,
		resourceindex,resourceindex,resourceindex, 
		&g_MainInterfaceTextureArchive 
	};

	m_WatchAndWinButton.Create( watchandwinbtnstruct );
	m_WatchAndWinButton.SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_WatchAndWinButton.SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_WatchAndWinButton.SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_WatchAndWinButton.SetButtonText( _T("WATCH & WIN: 15 sec video ad") );
	m_WatchAndWinButton.SetButtonTextColor( 0xFFFFFF00 );
	m_WatchAndWinButton.SetButtonTextHighlightColor( 0xFFFFFF40 );
	m_WatchAndWinButton.SetGrowTextMode( !TRUE, 0xFFFFFF00 );
	m_WatchAndWinButton.ShowWindow( TRUE );
	m_WatchAndWinButton.CenterMouse();

	if( g_CurrentZoneInfoIndex == 22 ) // tutorial mode에서는 watch & win 광고 처리 안함.
	{
		m_WatchAndWinButton.ShowWindow( FALSE );
	}
	
	if( !g_IGAADWrapper.CheckExistContent( _XIGAADZONE_MAPLOADINGVIDEOAD ) )
	{
		m_WatchAndWinButton.ShowWindow( FALSE );
		m_AcclaimADPhase = _XACCLAIMADPHASE_BILLBOARD;
		g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_MAPLOADING );
		g_IGAADWrapper.ShowWindow( TRUE );
		g_IGAADWrapper.StartDisplay();
	}

	int windowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );

	POINT pos;
	pos.x = (gnWidth>>1) - 200; // width 400
	pos.y = (gnHeight>>1) - 100; // height 200
	
	m_TitleLeftBar.Create( pos.x, pos.y, pos.x+80, pos.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleLeftBar.SetClipRect( 1, 126, 81, 138 );

	m_TitleCenterBar.Create( pos.x+80, pos.y, pos.x+400, pos.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleCenterBar.SetClipRect( 84, 126, 94, 138 );

	FLOAT scalefactor = ((FLOAT)(400-86)) / 10.0f;
	m_TitleCenterBar.SetScale( scalefactor, 1.0f );

	m_TitleRightBar.Create( pos.x+400-6, pos.y, pos.x+400-1, pos.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleRightBar.SetClipRect( 177, 126, 183, 138 );

	int moneyiconimageindex = g_MainInterfaceTextureArchive.FindResource( "item_mon.tga" );
	m_ItemIconStatic.Create( (gnWidth>>1) - 16, (gnHeight>>1) - 40, (gnWidth>>1) + 16, (gnHeight>>1) - 40 + 32, &g_MainInterfaceTextureArchive, moneyiconimageindex );
	m_ItemIconStatic.SetClipRect( 0, 0, 32, 32 );
#endif
	
	// 일본은 gameguard basic license라서 실행파일 압축 사용 안함.
	#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT) && !defined(_XJAPANESE) && !defined(_XRUSSIAN) && !defined(_XVIETNAMESE) && !defined(_XTAIWANESE) 
//
		LPCTSTR pModuleFilename = _XArgv(0);
		if( pModuleFilename )
		{
			_XSetDefaultPath();
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
					//_XLog( "%d,%d", 21, g_LocalSystemTime );
					_XMsgClose = TRUE;
					return FALSE;
				}
			}
			else
			{
				//_XLog( "%d,%d, %s", 22, g_LocalSystemTime, pModuleFilename );
				_XMsgClose = TRUE;
				return FALSE;
			}
		}
		else
		{
			//_XLog( "%d,%d", 23, g_LocalSystemTime );
			_XMsgClose = TRUE;
			return FALSE;
		}

	#endif
		
#ifdef _XVIETNAMESE
	extern BOOL _XCheckLoginServerIP( LPCTSTR szHostName );	
	if( _XCheckLoginServerIP( _T("login.cuulong.com.vn") ) )
	{
		_XLog( "Can't initialize loading process." );
		_XMsgClose = TRUE;
		return FALSE;
	}
#endif
	
#ifdef _ACCLAIM_RUBICONADSYSTEM	
	
	DisplayHTMLPage(g_hRubiconADWebBrowserObject, "http://www.acclaim.com/ads/9d_house_728_90.html");

	POINT pt = { 0, 0 };
	::ClientToScreen( gHWnd, &pt );

	g_RubiconADWindowPosition.x = 2;
	g_RubiconADWindowPosition.y = gnHeight - 130;
	g_RubiconADWindowSize.cx = 728;
	g_RubiconADWindowSize.cy = 90;

	::SetWindowPos(g_hRubiconADWindow, NULL, pt.x + 2, pt.y + gnHeight - 130, 728, 90, 0);
	::SetWindowPos(g_hRubiconADWebBrowserObject, NULL, 0, 0, 728, 90, 0);
		
	g_RubiconADFirstTimeView = TRUE;

	//::ShowWindow(g_hRubiconADWindow, SW_SHOW);	
	//::UpdateWindow( g_hRubiconADWindow );	
	//::RedrawWindow( g_hRubiconADWindow, NULL, NULL, RDW_UPDATENOW );
	
#endif

	return TRUE;
}

void XProc_Loading::DestroyProcess( void )
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

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.DisableMADLocation( _T("MapTransition") );
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM
	::ShowWindow(g_hRubiconADWindow, SW_HIDE);
#endif

}


void XProc_Loading::SetMessage( LPTSTR message )
{
	memset( m_Message, 0, sizeof(TCHAR) * 256 );
	
	if( !message || strlen(message) <= 0 )  return;
	
	strcpy( m_Message, message );
	
	messagecoord.x = (gnWidth>>1);
	messagecoord.y = (gnHeight) - g_XBaseFont->GetHeight() - 19;

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.Update();
#endif
}

bool XProc_Loading::InitDeviceObject( void )
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
			
	return true;
}

void XProc_Loading::ReleaseDeviceObject( void )
{	
	
}

void XProc_Loading::PauseProcess( void )
{
	
}

void XProc_Loading::UnPauseProcess( void )
{
	ReleaseDeviceObject();
	InitDeviceObject();
}

//////////////////////////////////////////////////////////////////////////
// Draw
//////////////////////////////////////////////////////////////////////////

void XProc_Loading::Draw( void )
{
	gpDev->Clear(0, NULL,
				 D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	// Begin the scene
	gpDev->BeginScene();

	m_Player.UpdateViewMatrix();
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());

#ifdef _ACCLAIM_IGAADSYSTEM
	if( m_AcclaimADPhase != _XACCLAIMADPHASE_WATCHANDWINADVIEW )
	{
#endif
		
		m_BackgroundStatic.Render();
		_XDrawRectAngle( 0, 0, gnWidth-1, gnHeight-1, 0.0f, 0xFF000000 );
		_XDrawRectAngle( 1, 1, gnWidth-2, gnHeight-2, 0.0f, 0xFF000000 );

#ifdef _ACCLAIM_IGAADSYSTEM
	}
#endif

	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );

#ifdef _ACCLAIM_IGAADSYSTEM
	if( m_AcclaimADPhase != _XACCLAIMADPHASE_END )
	{
		if( m_AcclaimADPhase == _XACCLAIMADPHASE_BILLBOARD )
		{
			g_IGAADWrapper.Render();
		}
		else if( m_AcclaimADPhase == _XACCLAIMADPHASE_WATCHANDWINBUTTON )
		{
			m_WatchAndWinButton.Draw();
		}
		if( m_AcclaimADPhase == _XACCLAIMADPHASE_WATCHANDWINADVIEW )
		{
			g_IGAADWrapper.Render();
		}
		else if( m_AcclaimADPhase == _XACCLAIMADPHASE_WATCHANDWINRESULTVIEW )
		{
			POINT pos;
			pos.x = (gnWidth>>1) - 200; // width 400
			pos.y = (gnHeight>>1) - 100; // height 200

			DWORD alphalevel = (int)( (FLOAT)0x7F * m_MessageBoxAlphaLevel );
			_XDrawRectAngle( pos.x, pos.y + 12, pos.x+400-1, pos.y+200-1, 0.0f, D3DCOLOR_ARGB(alphalevel, 0, 0, 0 ) );

			alphalevel = (int)( (FLOAT)0x40 * m_MessageBoxAlphaLevel );
			_XDrawSolidBar( pos.x+1, pos.y + 13, pos.x+400-1, pos.y+200-1, D3DCOLOR_ARGB(alphalevel, 0, 0, 0 ) );

			
			alphalevel = (int)( 255.0f * m_MessageBoxAlphaLevel );
			m_TitleRightBar.m_FColor = m_TitleLeftBar.m_FColor = m_TitleCenterBar.m_FColor = D3DCOLOR_ARGB(alphalevel, 0xFF, 0xFF, 0xFF );

			m_TitleCenterBar.Draw();
			m_TitleLeftBar.Draw();
			m_TitleRightBar.Draw();

			g_XBaseFont->SetColor( m_TitleRightBar.m_FColor );
			g_XBaseFont->SetAlphaBlendingEnable( TRUE );
			if( !m_RecievedADResult )
			{
				if( m_ADRecievedResultAutoCancelCounter == 0 ) m_ADRecievedResultAutoCancelCounter = g_LocalSystemTime;				
				int lefttime = (10000 - (g_LocalSystemTime - m_ADRecievedResultAutoCancelCounter))  / 1000;
				if( lefttime < 10 && lefttime > 0 )
				{
					g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Wait... [%d sec]"), lefttime );
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB(alphalevel, 0xFF, 0x00, 0x00 ) );
					g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("[ Cancel. ]") );
				}
			}
			else
			{
					m_ItemIconStatic.Draw();

					// 일단 금만 처리한다. 나중에 아이템도 같이 처리.
					if( m_LoadingAD_Gold == 0 )
					{
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Karma has denied you.") );
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_LoadingAD_Gold );
					}
					else if( m_LoadingAD_Gold >= 1 && m_LoadingAD_Gold < 5 )
					{
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Gold from Heaven.") );
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_LoadingAD_Gold );
					}
					else if( m_LoadingAD_Gold >= 5 && m_LoadingAD_Gold < 10 )
					{
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Buddha smiles upon you.") );
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_LoadingAD_Gold );
					}
					else if( m_LoadingAD_Gold >= 10 && m_LoadingAD_Gold < 50 )
					{
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Emperor’s Good Fortune.") );
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_LoadingAD_Gold );
					}
					else if( m_LoadingAD_Gold >= 50 && m_LoadingAD_Gold < 100 )
					{
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Karma Payday.") );
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_LoadingAD_Gold );
					}
					else if( m_LoadingAD_Gold >= 100 && m_LoadingAD_Gold < 500 )
					{
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Blue Moon Karma Payday.") );
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_LoadingAD_Gold );
					}
					else if( m_LoadingAD_Gold >= 500 )
					{
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1),1.0f, _XFONT_ALIGNTYPE_CENTER, _T("Future Dragon's Luck.") );
						g_XBaseFont->PrintAlign( (gnWidth>>1), (gnHeight>>1)+14,1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d Gold"), m_LoadingAD_Gold );
					}
			}
			g_XBaseFont->Flush();
			g_XBaseFont->SetAlphaBlendingEnable( FALSE );
		}
	}
	else
	{
#endif
	
#ifdef _ACCLAIM_IGAADSYSTEM
		if( m_AcclaimADPhase != _XACCLAIMADPHASE_WATCHANDWINADVIEW )
		{
#endif
			m_ProgressStatic.RenderSolid();	
			m_ProgressBarStatic.RenderSolid();	

			
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->PrintAlign( messagecoord.x, messagecoord.y + 2,1.0f, _XFONT_ALIGNTYPE_CENTER, m_Message );
			}
			else
			{
				g_XBaseFont->PrintAlign( messagecoord.x, messagecoord.y,1.0f, _XFONT_ALIGNTYPE_CENTER, m_Message );
			}
			g_XBaseFont->Flush();

#ifdef _ACCLAIM_IGAADSYSTEM
		}
	}
#endif
	
#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.Update();
	g_MadWrapper.DrawMADWindow();
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
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;
	
#ifdef _XUSEFMOD
	g_FMODWrapper.ProcessBGM();
	g_FMODWrapper.FMODUpdate();	
#endif

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

void XProc_Loading::OnPaint(WPARAM wparam, LPARAM lparam)
{	
	Draw();
}

//////////////////////////////////////////////////////////////////////////
// Process 
//////////////////////////////////////////////////////////////////////////

bool XProc_Loading::Process( void )
{		
	if( m_DataLoaded ) return true;

	g_MessageBox.Process();

#ifdef _ACCLAIM_IGAADSYSTEM
	if( m_AcclaimADPhase != _XACCLAIMADPHASE_END )
	{
		if( m_AcclaimADPhase == _XACCLAIMADPHASE_WATCHANDWINBUTTON )
		{
			m_WatchAndWinButton.Process();

			if( m_AutoCancelCounter == 0 )
			{
				m_AutoCancelCounter = g_LocalSystemTime;
			}
			else
			{
				if( g_LocalSystemTime - m_AutoCancelCounter > 3000 )
				{
					m_AcclaimADPhase = _XACCLAIMADPHASE_BILLBOARD;

					g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_MAPLOADING );
					g_IGAADWrapper.ShowWindow( TRUE );
					g_IGAADWrapper.StartDisplay();
				}
				else
				{
					TCHAR buttontext[64];
					_snprintf( buttontext, sizeof(buttontext), _T("WATCH & WIN: 15 sec video ad : %d"), 3 - ((g_LocalSystemTime - m_AutoCancelCounter) / 1000) );
					m_WatchAndWinButton.SetButtonText( buttontext );
				}
			}
			return true;
		}
		else if( m_AcclaimADPhase == _XACCLAIMADPHASE_BILLBOARD )
		{
			g_IGAADWrapper.Process();

			if( !g_IGAADWrapper.GetShowStatus() )
			{
				m_AcclaimADPhase = _XACCLAIMADPHASE_END;
				m_WatchAndWinButton.ShowWindow( FALSE );
				return true;
			}
			else
			{
				return true;
			}
		}		
		else if( m_AcclaimADPhase == _XACCLAIMADPHASE_WATCHANDWINADVIEW )
		{
			g_IGAADWrapper.Process();

			if( !g_IGAADWrapper.GetShowStatus() )
			{
				m_AcclaimADPhase = _XACCLAIMADPHASE_WATCHANDWINRESULTVIEW;
				m_WatchAndWinResultViewTimer = g_LocalSystemTime;

				m_RecievedADResult = FALSE;				
				g_NetworkKernel.SendPacket( MSG_NO_US_AD_REWARD_REQ, UART_AFTER_GAME_LOADING );
				m_ADRecievedResultAutoCancelCounter = 0;
				return true;
			}
			else
			{
				return true;
			}
		}
		else if( m_AcclaimADPhase == _XACCLAIMADPHASE_WATCHANDWINRESULTVIEW )
		{
			if( !m_RecievedADResult )
			{
				if( m_ADRecievedResultAutoCancelCounter == 0 )
				{
					m_ADRecievedResultAutoCancelCounter = g_LocalSystemTime;
				}
				else
				{
					if( g_LocalSystemTime - m_ADRecievedResultAutoCancelCounter > 10000 )
					{
						m_MessageBoxAlphaLevel -= 0.05f;

						if( m_MessageBoxAlphaLevel < 0.0f )
						{
							m_AcclaimADPhase = _XACCLAIMADPHASE_END;
						}
					}
				}
			}
			else
			{
				if( m_WatchAndWinResultViewTimer == 0 )
				{
					m_WatchAndWinResultViewTimer = g_LocalSystemTime;
				}

				if( g_LocalSystemTime - m_WatchAndWinResultViewTimer > 2000 )
				{
					m_MessageBoxAlphaLevel -= 0.05f;

					if( m_MessageBoxAlphaLevel < 0.0f )
					{
						m_AcclaimADPhase = _XACCLAIMADPHASE_END;
					}
				}
			}
			return true;
		}
	}
#endif


	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_INITDATA) );
	UpdateProgress( 0 );	
			
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
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

	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_CREATETERRAIN) );
	UpdateProgress( 5 );	
	
	if(g_SinglePlayMode)
	{
		pMainGameProcess->InitializeLight();
		g_CurrentZoneInfoIndex = 1;
	}

	// 현재 장원전 세팅
	g_CBManager.SetCurrentScheduler(g_CurrentZoneInfoIndex);


	// Load Terrain data ======================================================	

	BOOL forceloading = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("forceloadingterrainmap"), FALSE, FALSE );

	if( !forceloading )
	{
		if(_XCmdLineOption("xforcemaploading",0) != 0)
		{
			forceloading = true;
		}
	}

	g_fWaterDepth = 0.28f; // 수심 몇미터까지 들어갈수 있다.

	if( !forceloading )
	{
		if( !pMainGameProcess->LoadTerrain( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonefilename ) ) return false;

		if( strcmpi( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonefilename, _T("Map_namchang_boss.xtv") ) == 0 ) // 남창 보스맵은 수심체크를 좀더 높인다.
		{
			g_fWaterDepth = 1.0f;
		}
	}
	else
	{
		TCHAR mapfilename[512];
		g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("terrainmapname"), _T("Map_namchang_boss.xtv"), mapfilename, 512, TRUE );
		if( !pMainGameProcess->LoadTerrain( mapfilename ) ) return false;	

		g_fWaterDepth = 1.0f; 
	}

	g_LodTerrain.m_GenerateRealTimeObject = TRUE;

//	int minimapindex[_XDEF_FLASH_MAX_ZONECOUNT];
	_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( pminimapwindow )
	{
		//미니맵 이미지 파일명을 찾는 함수
		pminimapwindow->SetMinimapImage( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonefilename );//미니맵 이미지 파일명을 찾는 함수
		pminimapwindow->SetCurrentZoneIndex( g_CurrentZoneInfoIndex );
		pminimapwindow->SetMinimapRate();//2004.05.21->oneway48 insert	
		
	}

	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_OBJECT) );
	UpdateProgress( 75 );
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH );	
	if( !g_ModelArchive.ReOpenPackage() ) 
		return false;
					
#ifdef _XDWDEBUG
	pMainGameProcess->m_ArrowModel.Load( g_ModelArchive.GetPackedFile(_T("Arrow.X3D")), &g_TerrainTextureArchive, FALSE, _T("Arrow.X3D") );
	pMainGameProcess->m_LightConeTextureIndex = g_TerrainTextureArchive.GetResourceIndex( "yellowarrow.bmp" );
	
	g_AxisModel.Load( g_ModelArchive.GetPackedFile(_T("axis.X3D")), &g_TerrainTextureArchive, FALSE, _T("axis.X3D") );
#endif

	g_ModelArchive.ClosePackage();
			
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_CACHINGTERRAIN) );
	UpdateProgress( 80 );

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
#else
	if( !g_TerrainTextureArchive.ReadyRenderStyle( _T("Environment.xrs") ) ) return false;	
#endif

	//_XTEXTUREPRECACHEING_ON
	if( !g_TerrainTextureArchive.BuildTextureList() ) return false;		
	//_XTEXTUREPRECACHEING_OFF

#ifdef _XTESTSERVER				
	if( !g_TerrainTextureArchive.AttachRenderStyle( _T("tsEnvironment.xrs") ) ) return false;	
#else
	if( !g_TerrainTextureArchive.AttachRenderStyle( _T("Environment.xrs") ) ) return false;	
#endif

	
	// Pre caching minimap image...
	if( pminimapwindow )
	{
		g_TerrainTextureArchive.SetTexture( pminimapwindow->m_LargeMinimapImage.m_TextureIndex );
		
//		for( int i = 0; i < _XDEF_FLASH_MAX_ZONECOUNT; i++ )
//		{
//			if( minimapindex[i] != -1 )
//			{
//				g_TerrainTextureArchive.SetTexture( minimapindex[i] );
//			}
//		}
	}
	
	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_INITTERRAIN) );
	UpdateProgress( 85 );

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
	UpdateProgress( 90 );

	_XMEMORYUSECHECKREADY		
	_XMEMORYUSECHECKSTART

#ifdef _XTESTSERVER
	if( !g_WorldObjectTextureArchive.ReadyRenderStyle( _T("tsObject.xrs") ) ) return false;	
#else
	if( !g_WorldObjectTextureArchive.ReadyRenderStyle( _T("Object.xrs") ) ) return false;	
#endif
	
	//_XTEXTUREPRECACHEING_ON
	if( !g_WorldObjectTextureArchive.BuildTextureList() )
	{
		g_TextureQuality		= 1;
		return false;	
	}
	//_XTEXTUREPRECACHEING_OFF
	
	
#ifdef _XTESTSERVER
	if( !g_WorldObjectTextureArchive.AttachRenderStyle( _T("tsObject.xrs") ) ) return false;	
#else
	if( !g_WorldObjectTextureArchive.AttachRenderStyle( _T("Object.xrs") ) ) return false;	
#endif
			 
	//g_WorldObjectTextureArchive.ReportTextureInfo();

	_XMEMORYUSECHECKEND( "World object texture memory" );

	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_INITOBJECT) );
	UpdateProgress( 97 );

	if( !g_WorldObjectTextureArchive.InitDeviceObject() )
	{
		g_TextureQuality		= 1;
		return false;	
	}

	g_TextureQuality		= 1;

	// World 에 사용되는 객체의 텍스쳐를 미리 로드한다. 
	g_LodTerrain.PreCachingTexture();

	SetMessage( _XGETINTERFACETEXT(ID_STRING_LOADING_ENDINIT) );
	UpdateProgress( 100 );

	//_XMessage( "End of data loading" );
	// 로딩이 끝났으므로 메인게임 프로세스로 교체
	((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( 
					&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame );
	
	return true;
}

void XProc_Loading::UpdateProgress( int increase )
{
	if( m_Progress >= increase ) return;

	for( int i = m_Progress; i <= increase; i++ )
	{
		m_ProgressBarStatic.SetXPercentage( i, 1, FALSE );
		Draw();
		Sleep( 0 );
		//_XDWINPRINT( "Loading progress : %d", i );
	}
	
	// 서버 접속유지를 위해 시그널 전송
	if( m_SiganalLastSendedTime == 0 || (g_LocalSystemTime - m_SiganalLastSendedTime > 30000) )
	{
		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		m_SiganalLastSendedTime = g_LocalSystemTime;
	}

	Sleep( 1 );

	m_Progress = increase;	
}


//////////////////////////////////////////////////////////////////////////
// MessageProc
//////////////////////////////////////////////////////////////////////////

void XProc_Loading::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( message == WM_COMMAND )
	{
#ifdef _ACCLAIM_IGAADSYSTEM
		if( wparam == _XDEF_LOADINGBTN_WATCHANDWINBUTTON )
		{
			//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("You've won 0 Gold coins!"), TRUE );

			m_AcclaimADPhase = _XACCLAIMADPHASE_WATCHANDWINADVIEW;
			g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_MAPLOADINGVIDEOAD );
			
			// test
			g_IGAADWrapper.CreateAdWindow();

			g_IGAADWrapper.ShowWindow( TRUE );
			g_IGAADWrapper.StartDisplay();
		}
#endif

	}
}

void XProc_Loading::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{	
	if (WSAGETASYNCERROR(lparam) != 0) 
	{
		int errorcode = WSAGetLastError();

		if( errorcode == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : LG : TCP I/O blocking");
			Sleep( 50 );
		}
		else
		{
			if( errorcode != ERROR_IO_PENDING )
			{
				if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer )
				{
					g_NetworkKernel.ProcessNetworkError();
				}
			}
			else
			{
				_XLog( "WARNING : LG : TCP I/O pending" );
				Sleep( 50 );
			}
		}
		return;		
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{	
	case FD_READ :
		if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer )
		{
			g_NetworkKernel.ReadSocket();
		}
		break;
	case FD_CLOSE :
		if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer )
		{
			g_NetworkKernel.ProcessNetworkError();		
			_XDWINPRINT( "SERVER MESSAGE : Socket close message" );
		}
		break;
	}
}

void XProc_Loading::OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		if( !g_NetworkKernel.m_bConnected )
		{
			g_NetworkKernel.ProcessMessengerNetworkError();
		}
		return;
	}
	
	switch (WSAGETSELECTEVENT(lparam)) 
	{
	case FD_CONNECT:
		WSAAsyncSelect(g_NetworkKernel.m_hMessengerSocket, gHWnd, WM_USER_MESSENGERSOCKET, FD_READ | FD_CLOSE );
		g_NetworkKernel.m_bMessengerServerConnected = TRUE;
		_XDWINPRINT( "Connected Messenger server" );
		break;		
	case FD_READ :
		g_NetworkKernel.ReadMessengerPacket();
		break;
	case FD_CLOSE :
		g_NetworkKernel.ProcessMessengerNetworkError();
		_XDWINPRINT( "SERVER MESSAGE : Messenger Socket close message" );
		break;
	}
}

bool XProc_Loading::OnKeyboardPoll(_XInput *pInput)
{ 
	return true; 
}

bool XProc_Loading::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	return true;
}

bool XProc_Loading::OnKeyDown(WPARAM wparam, LPARAM lparam)
{ 		
	return true; 
}

bool XProc_Loading::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{ 	
		
	return true; 
}

bool XProc_Loading::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
	return true; 
}	










































