// XWindow_Option.cpp: implementation of the _XWindow_Option class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_OptionGraphic.h"
#include "XWindow_OptionGame.h"
#include "XWindow_OptionSound.h"
#include "XWindow_OptionCommunity.h"
#include "XWindow_Option.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Option::_XWindow_Option()
{
	m_SelectedTab = _XOPTIONTAB_GRAPHIC;

	for( int i = 0; i < 4; i++ )
	{
		m_pSubWindow[i] = NULL;
	}
}

_XWindow_Option::~_XWindow_Option()
{

}


BOOL _XWindow_Option::Initialize( void )
{
	SetDrawBorder( FALSE );
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
#ifdef _XJAPANESE
	m_BorderTile.Create( m_WindowPosition.x + 3, m_WindowPosition.y + 24, m_WindowPosition.x + 280, m_WindowPosition.y + 400 );
#else
	m_BorderTile.Create( m_WindowPosition.x + 3, m_WindowPosition.y + 24, m_WindowPosition.x + 280, m_WindowPosition.y + 390 );
#endif
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );	
	FLOAT ufactor = 278.0f/128.0f;
#ifdef _XJAPANESE
	FLOAT vfactor = 367.0f/128.0f;
#else
	FLOAT vfactor = 367.0f/128.0f;
#endif

	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );
	
	m_BorderGradationTitle.Create( m_WindowPosition.x + 3, m_WindowPosition.y + 24, m_WindowPosition.x + 278, m_WindowPosition.y + 22, 
		&g_MainInterfaceTextureArchive, mainframeinterface );
	m_BorderGradationTitle.SetClipRect( 206, 227, 244, 246 );
	m_BorderGradationTitle.SetScale( 281.0f/39.0f, 1.0f );

	_XImageStatic* pBottomDecoLeftBar = new _XImageStatic;
#ifdef _XJAPANESE
	pBottomDecoLeftBar->Create( 7, 355+15, 95, 379+15, &g_MainInterfaceTextureArchive, optionmenuinterface );
#else
	pBottomDecoLeftBar->Create( 7, 355, 95, 379, &g_MainInterfaceTextureArchive, optionmenuinterface );
#endif
	pBottomDecoLeftBar->SetClipRect( 160, 219, 248, 243 );
	InsertChildObject( pBottomDecoLeftBar );

	_XImageStatic* pBottomDecoRightBar = new _XImageStatic;
#ifdef _XJAPANESE
	pBottomDecoRightBar->Create( 274, 355+15, 274+88, 355+24+15, &g_MainInterfaceTextureArchive, optionmenuinterface );
#else
	pBottomDecoRightBar->Create( 274, 355, 274+88, 355+24, &g_MainInterfaceTextureArchive, optionmenuinterface );
#endif

	pBottomDecoRightBar->SetClipRect( 160, 219, 248, 243 );
	pBottomDecoRightBar->SetScale( -1, 1 );
	InsertChildObject( pBottomDecoRightBar );
	
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 175, 23,
		&g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleLeftBar->SetClipRect( 0, 0, 142, 24 );
	InsertChildObject( pTitleLeftBar );
	
	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create( 282, 0, 282+175, 23,
		&g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleRightBar->SetClipRect( 0, 0, 142, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );

	_XImageStatic* pTopEdgeLeftLine = new _XImageStatic;
	pTopEdgeLeftLine->Create( 0, 24, 2, 279, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pTopEdgeLeftLine->SetClipRect( 253, 0, 256, 256 );
#ifdef _XJAPANESE
	pTopEdgeLeftLine->SetScale( 1.0f, 266.0f/256.0f );
#endif
	InsertChildObject( pTopEdgeLeftLine );
	
	_XImageStatic* pTopEdgeRightLine = new _XImageStatic;
	pTopEdgeRightLine->Create( 279, 24, 281, 279, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pTopEdgeRightLine->SetClipRect( 253, 0, 256, 256 );
#ifdef _XJAPANESE
	pTopEdgeRightLine->SetScale( 1.0f, 266.0f/256.0f );
#endif
	InsertChildObject( pTopEdgeRightLine );

	_XImageStatic* pBottomEdgeLeftLine = new _XImageStatic;	
#ifdef _XJAPANESE
	pBottomEdgeLeftLine->Create( 0, 280+10, 2, 391+40, &g_MainInterfaceTextureArchive, optionmenuinterface );
#else
	pBottomEdgeLeftLine->Create( 0, 280, 2, 391, &g_MainInterfaceTextureArchive, optionmenuinterface );	
#endif
	pBottomEdgeLeftLine->SetClipRect( 245, 0, 248, 112 );
	
	InsertChildObject( pBottomEdgeLeftLine );
	
	_XImageStatic* pBottomEdgeRightLine = new _XImageStatic;
	
#ifdef _XJAPANESE
	pBottomEdgeRightLine->Create( 279, 280+10 , 281, 391+40, &g_MainInterfaceTextureArchive, optionmenuinterface );	
#else
	pBottomEdgeRightLine->Create( 279, 280, 281, 391, &g_MainInterfaceTextureArchive, optionmenuinterface );	
#endif
	pBottomEdgeRightLine->SetClipRect( 249, 0, 252, 112 );	
	
	InsertChildObject( pBottomEdgeRightLine );

	_XImageStatic* pBottomEdgeLine = new _XImageStatic;
#ifdef _XJAPANESE
	pBottomEdgeLine->Create( 3, 389+10, 279, 391+10, &g_MainInterfaceTextureArchive, optionmenuinterface );
#else
	pBottomEdgeLine->Create( 3, 389, 279, 391, &g_MainInterfaceTextureArchive, optionmenuinterface );
#endif
	pBottomEdgeLine->SetClipRect( 0, 211, 236, 214 );
	pBottomEdgeLine->SetScale( 276.0f/236.0f, 1.0f );
	InsertChildObject( pBottomEdgeLine );


	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 262, 5 }, { 12, 12 }, _XDEF_OPTIONWINDOW_CLOSE,
									   charinfointerface,charinfointerface,charinfointerface, 
									   &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );


	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	_XBTN_STRUCTURE closebtn2struct = { TRUE, 
#ifdef _XJAPANESE
		{ 86, 356+16 }, 
#else
		{ 86, 356 }, 
#endif
		{ 111, 26 }, _XDEF_OPTIONWINDOW_CLOSE,
		characterwindowimageindex,characterwindowimageindex,characterwindowimageindex, 
		&g_MainInterfaceTextureArchive };
	pClosebutton = new _XButton;
	pClosebutton->Create( closebtn2struct );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 105, 112, 131 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 105, 112, 131 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 131, 112, 157 );
	pClosebutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_OPTION_CLOSE );	// _T("´Ý  ±â")
	pClosebutton->SetButtonTextColor( 0xFFE0D7BA );
	pClosebutton->SetGrowTextMode( TRUE );
	InsertChildObject( pClosebutton );	


	_XBTN_STRUCTURE helpstruct = { TRUE, { 249, 5 }, { 12, 12 }, _XDEF_OPTIONWINDOW_HELPBUTTON,
								   charinfointerface,charinfointerface,charinfointerface, 
		                           &g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	
	int quest2image = g_MainInterfaceTextureArchive.FindResource( "MI_Quest2.tga" );
	int skillwindimage = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	_XCHECKBTN_STRUCTURE tabbtnstruct = 
#ifdef _XJAPANESE
	{ TRUE, { 31, 323 }, { 108, 24 }, _XDEF_DLGTAB_GRAPHIC, skillwindimage, skillwindimage,&g_MainInterfaceTextureArchive };
#else		
	{ TRUE, { 8, 326 }, { 66, 26 }, _XDEF_DLGTAB_GRAPHIC, quest2image, quest2image,&g_MainInterfaceTextureArchive };
#endif

	int ButtonMenuString[4];	
	ButtonMenuString[0] = ID_STRING_OPTION_GRAPHIC;
	ButtonMenuString[1] = ID_STRING_OPTION_DEFAULTSOUND;
	ButtonMenuString[2] = ID_STRING_OPTION_GAME;
	ButtonMenuString[3] = ID_STRING_OPTION_COMMUNITY;	
	
	for( int i = 0; i < 4; i++ )
	{
		m_OptionTabButton[i] = new _XCheckButton;
		m_OptionTabButton[i]->Create( tabbtnstruct );
#ifdef _XJAPANESE
		m_OptionTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 2, 152, 110, 176 );
		m_OptionTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  2, 200, 110, 224 );
#else
		m_OptionTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 1, 229,  67, 255 );
		m_OptionTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  66,229, 132, 255 );
#endif
		m_OptionTabButton[i]->SetCheck( i == 0 );
		m_OptionTabButton[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ButtonMenuString[i] );
		InsertChildObject( m_OptionTabButton[i] );
		
#ifdef _XJAPANESE
		tabbtnstruct.position.x += 108;
		if( i == 1 )
		{
			tabbtnstruct.position.x = 31;
			tabbtnstruct.position.y += 24;
		}
#else
		tabbtnstruct.position.x += 67;
#endif
		tabbtnstruct.commandid++;
	}

	_XWindow_OptionGraphic* pGraphicOptionWindow = new _XWindow_OptionGraphic;
	_XWINDOW_STRUCTURE characterinfowindowstruct = 
	{
		TRUE, { 3, 24} , { 276, 303  }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_OPTION_SCREENWINDOW
	};
	pGraphicOptionWindow->Create( characterinfowindowstruct ); 
	pGraphicOptionWindow->Initialize();
	pGraphicOptionWindow->ShowWindow( FALSE );
	pGraphicOptionWindow->EnableWindowEffect( FALSE );
	pGraphicOptionWindow->SetDocking( FALSE, 0 );
	pGraphicOptionWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	InsertChildObject( pGraphicOptionWindow );
	m_pSubWindow[_XOPTIONTAB_GRAPHIC] = pGraphicOptionWindow;

	_XWindow_OptionGame* pGameOptionWindow = new _XWindow_OptionGame;
	_XWINDOW_STRUCTURE gameoptionwindowstruct = 
	{
		TRUE, { 3, 24} , { 276, 303  }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_OPTION_GAMEWINDOW
	};
	pGameOptionWindow->Create( gameoptionwindowstruct ); 
	pGameOptionWindow->Initialize();
	pGameOptionWindow->ShowWindow( FALSE );
	pGameOptionWindow->EnableWindowEffect( FALSE );
	pGameOptionWindow->SetDocking( FALSE, 0 );
	pGameOptionWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	InsertChildObject( pGameOptionWindow );
	m_pSubWindow[_XOPTIONTAB_GAME] = pGameOptionWindow;

	_XWindow_OptionSound* pSoundOptionWindow = new _XWindow_OptionSound;
	_XWINDOW_STRUCTURE soundwindowstruct = 
	{
		TRUE, { 3, 24} , { 276, 303  }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_OPTION_SOUNDWINDOW
	};
	pSoundOptionWindow->Create( soundwindowstruct ); 
	pSoundOptionWindow->Initialize();
	pSoundOptionWindow->ShowWindow( FALSE );
	pSoundOptionWindow->EnableWindowEffect( FALSE );
	pSoundOptionWindow->SetDocking( FALSE, 0 );
	pSoundOptionWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	InsertChildObject( pSoundOptionWindow );
	m_pSubWindow[_XOPTIONTAB_SOUND] = pSoundOptionWindow;


	_XWindow_OptionCommunity* pCommunityOptionWindow = new _XWindow_OptionCommunity;
	_XWINDOW_STRUCTURE communitywindowstruct = 
	{
		TRUE, { 3, 24} , { 276, 303  }, 0, 
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			_XDEF_OPTION_COMMUNITYWINDOW
	};
	pCommunityOptionWindow->Create( communitywindowstruct ); 
	pCommunityOptionWindow->Initialize();
	pCommunityOptionWindow->ShowWindow( FALSE );
	pCommunityOptionWindow->EnableWindowEffect( FALSE );
	pCommunityOptionWindow->SetDocking( FALSE, 0 );
	pCommunityOptionWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	InsertChildObject( pCommunityOptionWindow );
	m_pSubWindow[_XOPTIONTAB_CONMMUNITY] = pCommunityOptionWindow;

	ChangeTab(_XOPTIONTAB_GRAPHIC);
	
	return TRUE;
}

void _XWindow_Option::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
	
	m_pUseBMGButton = NULL;	
	m_pBGMVolumeSlider = NULL;		
	m_pUseSoundEffectButton = NULL;
	m_pEffectSoundVolumeSlider = NULL;
	m_pCommunityButton = NULL;
	m_ExitButton = NULL;
	m_GotoCharacterSelectButton = NULL;
}

void _XWindow_Option::SetDefaultPosition( void )
{	
	MoveWindow( (gnWidth>>1)-(282>>1), (gnHeight>>1)-(392>>1) );
	CheckOutside();
}

void _XWindow_Option::SaveWindowProperty( void )
{
	/*
	if( m_pGammaSlider ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_GAMMA,  (int)m_pGammaSlider->m_GaugeValue );
	if( m_pTerrainLodSlider ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_TERRAINLODLEVEL, (int)m_pTerrainLodSlider->m_GaugeValue );	
	if( m_pSightRangeSlider ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SIGHTRANGE, (int)m_pSightRangeSlider->m_GaugeValue );
	if( m_pGrassQualitySlider )g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEREALTIMEGENERATEOBJECTRATE, (int)m_pGrassQualitySlider->m_GaugeValue );
	if( m_pGrassAnimationButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEREALTIMEGENEOBJECTANI, m_pGrassAnimationButton->GetCheck() );
	if( m_pTreeAnimationButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WORLDOBJECTANI, m_pTreeAnimationButton->GetCheck() );
	if( m_pCharacterShadowButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCHARACTERSHADOW,m_pCharacterShadowButton->GetCheck() );
	if( m_pCloudeShadowButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCLOUDSHADOW, m_pCloudeShadowButton->GetCheck() );
	if( m_pEffectQualitySlider ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_EFFECTQUALITY, (int)m_pEffectQualitySlider->m_GaugeValue );
	if( m_pReflectionButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_REFLECTION, m_pReflectionButton->GetCheck() );
	if( m_pLensFlareButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LENSFLARE, m_pLensFlareButton->GetCheck()  );

	if( m_pUseBMGButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, m_pUseBMGButton->GetCheck() );
	if( m_pBGMVolumeSlider ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMVOLUME, m_pBGMVolumeSlider->m_GaugeValue );
	if( m_pUseSoundEffectButton ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, m_pUseSoundEffectButton->GetCheck() );
	if( m_pEffectSoundVolumeSlider ) g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDEFFECTVOLUME, (int)m_pEffectSoundVolumeSlider->m_GaugeValue );

	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_ALWAYSVIEWTOOLTIP, g_AlwaysViewUserNameAndItemTooltip );
	*/
}

void _XWindow_Option::ApplyOptionProperties( void )
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Apply screen option

	_XWindow_OptionGraphic* pGraphicsTab = ((_XWindow_OptionGraphic*)m_pSubWindow[_XOPTIONTAB_GRAPHIC]);
	_XWindow_OptionSound*   pSoundTab = ((_XWindow_OptionSound*)m_pSubWindow[_XOPTIONTAB_SOUND]);

	g_fGamma = 0.5f + 1.5f * (pGraphicsTab->m_pGammaSlider->m_GaugeValue / 100.0f);
	gApp->ApplyGammaRamp();

	// Set lod level
	int modifyfactor = 0;
	if( pGraphicsTab->m_pTerrainLodSlider->m_GaugeValue > 0.75f )
	{
		modifyfactor = 50;
	}
	else if( pGraphicsTab->m_pTerrainLodSlider->m_GaugeValue > 0.5f )
	{
		modifyfactor = 0;				
	}
	else if( pGraphicsTab->m_pTerrainLodSlider->m_GaugeValue > 0.25f )
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
	g_XST_Wrapper.m_RenderLimitDistance = g_LodTerrain.m_LODLevel1;

	// Set sight distance
	FLOAT rate = pGraphicsTab->m_pSightRangeSlider->m_GaugeValue / 100.0f;
	rate += 0.5f;
	if( rate < 0.5f ) rate = 0.5f;
	if( rate > 1.0f ) rate = 1.0f;		
	g_LodTerrain.m_LODFarPlane	= (250.0f * rate);
	//g_LodTerrain.m_FogFar	    = (250.0f * rate);
	//g_LodTerrain.m_FogDensity   = (0.0038f * rate);
	g_EnvironmentManager.m_EnvironmentLerpLevel = 2.0f;		
	g_LodTerrain.m_3PCamera.mp_fFarPlane = g_LodTerrain.m_LODFarPlane;

	// Max count Realtime Generate object 	 
	int count = (int)(64.0f * (pGraphicsTab->m_pGrassQualitySlider->m_GaugeValue / 100.0f));
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

	g_bUseRealtimeGenerateObjectAni = pGraphicsTab->m_pGrassAnimationButton->GetCheck();
	
	g_bCharacterShadow = pGraphicsTab->m_pCharacterShadowButton->GetCheck();
	
	g_bCloudShadow = FALSE;

	g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo( g_LodTerrain.m_3PCamera );
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
	g_LodTerrain.RebuildLevel(FALSE);

	int tracecount = 2 + (int)(18.0f * (pGraphicsTab->m_pEffectQualitySlider->m_GaugeValue) / 100.0f);
	_XEffect_SwordTrace::SetPointStep( tracecount );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Apply sound option
	FLOAT fvolume = pSoundTab->m_pEffectSoundVolumeSlider->m_GaugeValue / 100.0f;
	
#ifdef _XUSEFMOD
	// FMODProc
#else
	for( int i = 0; i < g_MaxSoundEffectCount; i++ )
	{	
		g_pSoundEffect[i]->SetVolume( fvolume );
	}	
#endif

	ConfirmDevice();
}

void _XWindow_Option::ConfirmDevice( void )
{
	_XWindow_OptionGraphic* pGraphicsTab = ((_XWindow_OptionGraphic*)m_pSubWindow[_XOPTIONTAB_GRAPHIC]);

	D3DDEVICE_CREATION_PARAMETERS DevCP;	
	gpDev->GetCreationParameters(&DevCP);	
	DWORD MaxSimultaneousTextures = g_d3dCaps.MaxSimultaneousTextures;
		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Confirm hardware
	if( MaxSimultaneousTextures == 1  )
	{					
					g_CloudShadowTextureStage	  = 1;
					g_bCloudShadow  = FALSE;
					g_bBuildingCharacterShadow = FALSE;		
					
					pGraphicsTab->m_pCharacterShadowButton->EnableWindow( TRUE );
					//m_pCloudeShadowButton->EnableWindow( FALSE );
	}
	else if( MaxSimultaneousTextures == 2  )
	{		
					g_CloudShadowTextureStage	  = 1;
					g_bCloudShadow  = FALSE;
					g_bBuildingCharacterShadow = FALSE;		
					
					pGraphicsTab->m_pCharacterShadowButton->EnableWindow( TRUE );		
					//m_pCloudeShadowButton->EnableWindow( FALSE );
	}
	else if( MaxSimultaneousTextures == 3  )
	{		
					g_CloudShadowTextureStage	  = 2;		
					g_bBuildingCharacterShadow = TRUE;
					
					pGraphicsTab->m_pCharacterShadowButton->EnableWindow( TRUE );
					//m_pCloudeShadowButton->EnableWindow( TRUE );
	}
	else if( MaxSimultaneousTextures >= 4  )
	{		
					g_CloudShadowTextureStage	  = 2;				
					g_bBuildingCharacterShadow = TRUE;
					
					pGraphicsTab->m_pCharacterShadowButton->EnableWindow( TRUE );
					//m_pCloudeShadowButton->EnableWindow( TRUE );
	}

	if( g_d3dCaps.Caps2 & D3DCAPS2_FULLSCREENGAMMA )
	{
		pGraphicsTab->m_pGammaSlider->ShowWindow( TRUE );
	}
	else
	{
		pGraphicsTab->m_pGammaSlider->ShowWindow( FALSE );
	}
}

void _XWindow_Option::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );	
	
#ifdef _XJAPANESE
	m_BorderTile.MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + 24, m_WindowPosition.x + 280, m_WindowPosition.y + 400 );
#else
	m_BorderTile.MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + 24, m_WindowPosition.x + 280, m_WindowPosition.y + 390 );
#endif

	m_BorderGradationTitle.MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + 22 );			
}


void _XWindow_Option::Draw( _XGUIObject*& pfocusobject )
{	
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
		m_BorderGradationTitle.Draw( pfocusobject );
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;		
}

/*
void _XWindow_Option::DrawSubDlg_Sound( _XGUIObject*& pfocusobject )
{
	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+30, 
					 m_WindowPosition.x + 195, m_WindowPosition.y+64, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+66, 
					 m_WindowPosition.x + 195, m_WindowPosition.y+100, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 34, _XGETINTERFACETEXT(ID_STRING_OPTION_USEMUSIC) );	
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 50, _XGETINTERFACETEXT(ID_STRING_OPTION_MUSICVOLUME) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 70, _XGETINTERFACETEXT(ID_STRING_OPTION_USESOUND) );	
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 86, _XGETINTERFACETEXT(ID_STRING_OPTION_SOUNDVOLUME) );

	g_XBaseFont->Flush();

}

void _XWindow_Option::DrawSubDlg_Screen( _XGUIObject*& pfocusobject )
{
	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+30, 
		             m_WindowPosition.x + 195, m_WindowPosition.y+79, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+82, 
					 m_WindowPosition.x + 195, m_WindowPosition.y+131, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+134, 
					 m_WindowPosition.x + 195, m_WindowPosition.y+167, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+171, 
					 m_WindowPosition.x + 195, m_WindowPosition.y+219, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+222, 
					 m_WindowPosition.x + 195, m_WindowPosition.y+255, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+259, 
					 m_WindowPosition.x + 195, m_WindowPosition.y+311, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 34, _XGETINTERFACETEXT(ID_STRING_OPTION_RESOLUTION) );	
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 50, _XGETINTERFACETEXT(ID_STRING_OPTION_DEPTHBUFFER) );	
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 66, _XGETINTERFACETEXT(ID_STRING_OPTION_GAMMA) );


	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 86, _XGETINTERFACETEXT(ID_STRING_OPTION_TERRAINQUALITY) );	
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 102, _XGETINTERFACETEXT(ID_STRING_OPTION_TERRAINDETAIL) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 118, _XGETINTERFACETEXT(ID_STRING_OPTION_FOG) );


	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 138, _XGETINTERFACETEXT(ID_STRING_OPTION_VIEWANGLE) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 154, _XGETINTERFACETEXT(ID_STRING_OPTION_VIEWRANGE) );	


	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 174, _XGETINTERFACETEXT(ID_STRING_OPTION_USEGRASS) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 190, _XGETINTERFACETEXT(ID_STRING_OPTION_MOVEGRASS) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 206, _XGETINTERFACETEXT(ID_STRING_OPTION_MOVEOBJECT) );

	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 226, _XGETINTERFACETEXT(ID_STRING_OPTION_CHARACTERSHADOW) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 242, _XGETINTERFACETEXT(ID_STRING_OPTION_CLOUDSHADOW) );

	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 262, _XGETINTERFACETEXT(ID_STRING_OPTION_EFFECTQUALITY) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 278, _XGETINTERFACETEXT(ID_STRING_OPTION_USESHADER) );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 294, _XGETINTERFACETEXT(ID_STRING_OPTION_LENSFLAIR) );
	
	g_XBaseFont->Flush();
}

void _XWindow_Option::DrawSubDlg_Visual( _XGUIObject*& pfocusobject )
{
	
}

void _XWindow_Option::DrawSubDlg_Community( _XGUIObject*& pfocusobject )
{
	_XDrawRectAngle( m_WindowPosition.x + 25,  m_WindowPosition.y+30, 
		m_WindowPosition.x + 195, m_WindowPosition.y+64, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	
		
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 34, _XGETINTERFACETEXT(ID_STRING_OPTION_USETOOLTIP) );
	
	g_XBaseFont->Flush();
	
}

void _XWindow_Option::DrawSubDlg_Game( _XGUIObject*& pfocusobject )
{
	
}

void _XWindow_Option::DrawSubDlg_Quick( _XGUIObject*& pfocusobject )
{
	
}

void _XWindow_Option::DrawSubDlg_System( _XGUIObject*& pfocusobject )
{
	
}
*/

BOOL _XWindow_Option::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
		
	return TRUE;
}	

void _XWindow_Option::ChangeTab(_XOptionWindowTab selectedtab)
{
	m_SelectedTab = selectedtab;
	
	for( int i = 0; i < 4; i++ )
	{
		m_OptionTabButton[i]->SetCheck( i == m_SelectedTab );

		if( m_pSubWindow[i] )
		{
			m_pSubWindow[i]->ShowWindow( i == selectedtab );
		}		
	}

	/*
	if( g_d3dCaps.Caps2 & D3DCAPS2_FULLSCREENGAMMA )
		m_pGammaSlider->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );
	else
		m_pGammaSlider->ShowWindow( FALSE );
		
	m_pTerrainLodSlider->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );
	
	m_pSightRangeSlider->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );	
	m_pGrassQualitySlider->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );
	m_pGrassAnimationButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );
	m_pTreeAnimationButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );	
	m_pCharacterShadowButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );
	m_pCloudeShadowButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );	
	m_pEffectQualitySlider->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );
	
	if( CWater::m_WaterRenderingMode == _XWATER_RM_EMBM )
	{
		m_pReflectionButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );		
	}
	else
	{
		m_pReflectionButton->ShowWindow( FALSE );
	}

	m_pLensFlareButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SCREEN ) );

	m_pUseBMGButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SOUND ) );
	m_pBGMVolumeSlider->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SOUND ) );
	m_pUseSoundEffectButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SOUND ) );
	m_pEffectSoundVolumeSlider->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_SOUND ) );

	m_pCommunityButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_CONMMUNITY ) );

	m_ExitButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_EXIT ) );
	m_GotoCharacterSelectButton->ShowWindow( ( m_SelectedTab == _XOPTIONTAB_EXIT ) );
	m_GotoCharacterSelectButton->EnableWindow( FALSE );	
	*/
}

void _XWindow_Option::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);

	if( show )
	{
		SetDefaultPosition();
	}
}

void _XWindow_Option::SendRankHidingPacket( void )
{
	if( m_pSubWindow[_XOPTIONTAB_GAME] )
	{
		_XWindow_OptionGame* pOptionGame_Window = (_XWindow_OptionGame*)m_pSubWindow[_XOPTIONTAB_GAME];
		if( pOptionGame_Window )
		{
			if( pOptionGame_Window->m_RankHidingButton->GetCheck() )
				g_NetworkKernel.SendPacket( MSG_NO_RANK_HIDING, 1 );
			else
				g_NetworkKernel.SendPacket( MSG_NO_RANK_HIDING, 0 );
		}		
	}
}

void _XWindow_Option::SetRankHidingState( char check )
{
	if( m_pSubWindow[_XOPTIONTAB_GAME] )
	{
		_XWindow_OptionGame* pOptionGame_Window = (_XWindow_OptionGame*)m_pSubWindow[_XOPTIONTAB_GAME];
		if( pOptionGame_Window )
		{
			pOptionGame_Window->m_RankHidingButton->SetCheck(check);
		}		
	}
}