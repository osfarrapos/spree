// XWindow_OptionGraphic.cpp: implementation of the _XWindow_OptionGraphic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_OptionGraphic.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_OptionGraphic::_XWindow_OptionGraphic()
{
	m_pGammaSlider = NULL;
	m_pGammaSlider = NULL;
	m_pTerrainLodSlider = NULL;
	m_pSightRangeSlider = NULL;
	m_pGrassQualitySlider = NULL;
	m_pGrassAnimationButton = NULL;
	m_pTreeAnimationButton = NULL;
	m_pCharacterShadowButton = NULL;
	m_pUseBloomFilterSlider = NULL;
	m_pUseBloomFilterButton = NULL;
	m_pEffectQualitySlider = NULL;
	m_pReflectionButton = NULL;
	m_pLensFlareButton = NULL;
}

_XWindow_OptionGraphic::~_XWindow_OptionGraphic()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic option
BOOL _XWindow_OptionGraphic::Initialize( void )
{
	SetDrawBorder( FALSE );

	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int optionmenuinterface2 = g_MainInterfaceTextureArchive.FindResource( g_MI_KiUpTextureName );

	_XSLIDERCONTROL_STRUCTURE	slidercreatestruct =
	{
		TRUE, { 156, 132 }, 
		{ 88, 12 }, _XDEF_OPTION_SCREEN_GAMMA,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		{ -5, -4 }, { 12, 12 },
		optionmenuinterface,
		&g_MainInterfaceTextureArchive,
		"HSlider"
	};

	SIZE spinbtnsize = { 12, 12 };
	RECT leftspinbtncliprect[3] =
	{
		{ 200, 17, 212, 29 },
		{ 200, 29, 212, 41 },
		{ 200, 41, 212, 53 },
	};

	RECT rightspinbtncliprect[3] =
	{
		{ 178,  0,  190, 12 },
		{ 178,  12, 190, 24 },
		{ 178,  24, 190, 36 },
	};

	m_pGammaSlider = new _XHSlider;
	m_pGammaSlider->Create( slidercreatestruct );
	m_pGammaSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pGammaSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pGammaSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pGammaSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pGammaSlider->SetClipRect( 2,244, 90, 256 );
	m_pGammaSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_pGammaSlider->SetGabSize( 1 );
	m_pGammaSlider->ShowWindow( TRUE );
	m_pGammaSlider->EnableWindow( TRUE );
	m_pGammaSlider->SetGaugeValue( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_GAMMA, 35, TRUE ) );
	m_pGammaSlider->SetUseSpinButton( TRUE, 1 );
	m_pGammaSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );
		
	InsertChildObject( m_pGammaSlider );

	slidercreatestruct.position.y = 20;
	slidercreatestruct.commandid = _XDEF_OPTION_SCREEN_TERRAINLOD;
	m_pTerrainLodSlider = new _XHSlider;
	m_pTerrainLodSlider->Create( slidercreatestruct );
	m_pTerrainLodSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pTerrainLodSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pTerrainLodSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pTerrainLodSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pTerrainLodSlider->SetClipRect( 2,244, 90, 256 );
	m_pTerrainLodSlider->m_FColor = D3DCOLOR_ARGB( 64, 255, 255, 255 );
	m_pTerrainLodSlider->SetGabSize( 1 );
	m_pTerrainLodSlider->ShowWindow( TRUE );
	m_pTerrainLodSlider->SetGaugeValue( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_TERRAINLODLEVEL, 70, TRUE ) );
	m_pTerrainLodSlider->SetUseSpinButton( TRUE, 5 );
	m_pTerrainLodSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );

	InsertChildObject( m_pTerrainLodSlider );
		
	slidercreatestruct.position.y = 64;
	slidercreatestruct.commandid = _XDEF_OPTION_SCREEN_SIGHTRANGE;
	m_pSightRangeSlider = new _XHSlider;
	m_pSightRangeSlider->Create( slidercreatestruct );
	m_pSightRangeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pSightRangeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pSightRangeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pSightRangeSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pSightRangeSlider->SetClipRect( 2,244, 90, 256 );
	m_pSightRangeSlider->m_FColor = D3DCOLOR_ARGB( 64, 255, 255, 255 );
	m_pSightRangeSlider->SetGabSize( 1 );
	m_pSightRangeSlider->ShowWindow( !TRUE );
	m_pSightRangeSlider->SetGaugeValue( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SIGHTRANGE, 70, TRUE ) );
	m_pSightRangeSlider->SetUseSpinButton( TRUE, 5 );
	m_pSightRangeSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );
	InsertChildObject( m_pSightRangeSlider );

	slidercreatestruct.position.y = 107;
	slidercreatestruct.commandid = _XDEF_OPTION_SCREEN_GRASSQUALITY;
	m_pGrassQualitySlider = new _XHSlider;
	m_pGrassQualitySlider->Create( slidercreatestruct );
	m_pGrassQualitySlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pGrassQualitySlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pGrassQualitySlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pGrassQualitySlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pGrassQualitySlider->SetClipRect( 2,244, 90, 256 );
	m_pGrassQualitySlider->m_FColor = D3DCOLOR_ARGB( 64, 255, 255, 255 );
	m_pGrassQualitySlider->SetGabSize( 1 );
	m_pGrassQualitySlider->ShowWindow( TRUE );
	int grassdensity = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEREALTIMEGENERATEOBJECTRATE, 50, TRUE );
	m_pGrassQualitySlider->SetGaugeValue( grassdensity );
	m_pGrassQualitySlider->SetUseSpinButton( TRUE, 5 );
	m_pGrassQualitySlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );

	FLOAT rate = grassdensity / 100.0f;
	int count = (int)(64.0f * rate);

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

	InsertChildObject( m_pGrassQualitySlider );

	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 243, 164 }, 
											{ 14, 14 }, 
											_XDEF_OPTION_SCREEN_GRASSANIM,
											optionmenuinterface,optionmenuinterface,
											&g_MainInterfaceTextureArchive };
	
	m_pGrassAnimationButton = new _XCheckButton;
	m_pGrassAnimationButton->Create( checkbtnstruct );	
	m_pGrassAnimationButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pGrassAnimationButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pGrassAnimationButton->SetCheck( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEREALTIMEGENEOBJECTANI, TRUE, TRUE ) );
	m_pGrassAnimationButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pGrassAnimationButton );
	
	checkbtnstruct.position.y = 183;
	checkbtnstruct.commandid = _XDEF_OPTION_SCREEN_TREEANIM;
	m_pTreeAnimationButton = new _XCheckButton;
	m_pTreeAnimationButton->Create( checkbtnstruct );		
	m_pTreeAnimationButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pTreeAnimationButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pTreeAnimationButton->SetCheck( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WORLDOBJECTANI, TRUE, TRUE ) );
	m_pTreeAnimationButton->SetButtonSwapMode( FALSE );
	m_pTreeAnimationButton->EnableWindow( TRUE );
	InsertChildObject( m_pTreeAnimationButton );


	checkbtnstruct.position.y = 202;
	checkbtnstruct.commandid = _XDEF_OPTION_SCREEN_CHARACTORSHADOW;
	m_pCharacterShadowButton = new _XCheckButton;
	m_pCharacterShadowButton->Create( checkbtnstruct );	
	m_pCharacterShadowButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pCharacterShadowButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pCharacterShadowButton->SetCheck( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCHARACTERSHADOW, TRUE, TRUE ) );
	m_pCharacterShadowButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pCharacterShadowButton );
	

	_XSLIDERCONTROL_STRUCTURE	bloomslidercreatestruct =
	{
		TRUE, { 156, 221 }, 
		{ 71, 12 }, 
		_XDEF_OPTION_SCREEN_USEBLOOMFILTER,
		optionmenuinterface2,
		optionmenuinterface2,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		{ -5, -4 }, { 12, 12 },
		optionmenuinterface,
		&g_MainInterfaceTextureArchive,
		"HSlider"
	};
	
	m_pUseBloomFilterSlider = new _XHSlider;
	m_pUseBloomFilterSlider->Create( bloomslidercreatestruct );
	m_pUseBloomFilterSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pUseBloomFilterSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pUseBloomFilterSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pUseBloomFilterSlider->m_IndicatorImage.SetClipRect( 167,225 , 238, 237 );
	m_pUseBloomFilterSlider->SetClipRect( 167,211, 238, 223 );
	m_pUseBloomFilterSlider->m_FColor = D3DCOLOR_ARGB( 64, 255, 255, 255 );
	m_pUseBloomFilterSlider->SetGabSize( 1 );
	m_pUseBloomFilterSlider->SetUseSpinButton( TRUE, 25 );
	m_pUseBloomFilterSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );

	BOOL usebloomfilter = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEBLOOMFILTER, TRUE, TRUE );
	
	TCHAR g_RegValue[_REGSTRING_MAXLENGTH];
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BLOOMFILTERPOWER, "0.65", g_RegValue, _REGSTRING_MAXLENGTH, FALSE );
	g_PostProcessManager.m_FinalPixelShaderConst1[0] = 
	g_PostProcessManager.m_FinalPixelShaderConst1[1] = 
	g_PostProcessManager.m_FinalPixelShaderConst1[2] = 
	g_PostProcessManager.m_FinalPixelShaderConst1[3] = atof( g_RegValue );
	
	m_pUseBloomFilterSlider->SetGaugeValue( (g_PostProcessManager.m_FinalPixelShaderConst1[0]-0.65f) * 1000.0f );
	
	m_pUseBloomFilterSlider->ShowWindow( g_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(2,0) );

	InsertChildObject( m_pUseBloomFilterSlider );

	checkbtnstruct.position.y = 221;
	checkbtnstruct.commandid = _XDEF_OPTION_SCREEN_ENABLEBLOOMFILTER;
	m_pUseBloomFilterButton = new _XCheckButton;
	m_pUseBloomFilterButton->Create( checkbtnstruct );		
	m_pUseBloomFilterButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pUseBloomFilterButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pUseBloomFilterButton->SetCheck( usebloomfilter );
	m_pUseBloomFilterButton->SetButtonSwapMode( FALSE );
	m_pUseBloomFilterButton->EnableWindow( g_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(2,0) );
	InsertChildObject( m_pUseBloomFilterButton );
	

	slidercreatestruct.position.y = 40;
	slidercreatestruct.commandid = _XDEF_OPTION_SCREEN_EFFECTQUALITY;

	m_pEffectQualitySlider = new _XHSlider;
	m_pEffectQualitySlider->Create( slidercreatestruct );
	m_pEffectQualitySlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pEffectQualitySlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pEffectQualitySlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pEffectQualitySlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pEffectQualitySlider->SetClipRect( 2,244, 90, 256 );
	m_pEffectQualitySlider->m_FColor = D3DCOLOR_ARGB( 64, 255, 255, 255 );
	m_pEffectQualitySlider->SetGabSize( 1 );
	m_pEffectQualitySlider->ShowWindow( TRUE );
	m_pEffectQualitySlider->SetGaugeValue( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_EFFECTQUALITY, 70, TRUE ) );
	m_pEffectQualitySlider->SetUseSpinButton( TRUE, 5 );
	m_pEffectQualitySlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );
	InsertChildObject( m_pEffectQualitySlider );
	
	extern FLOAT	g_VisualEffectQuality;
	FLOAT lev =	m_pEffectQualitySlider->m_GaugeValue;	
	
	if( lev >= 70.0f )
	{
		g_VisualEffectQuality = 1.0f;
	}
	else
	{
#ifdef _XDEF_VEQUALITYCONTROL
		/*
		lev = lev * 1.2857f; // 70% to 90%
		if( lev < 0.0f )
			lev = 0.0f;
		else if( lev > 90.0f )
			lev = 90.0f;

		g_VisualEffectQuality = ( lev + 10.0f ) / 100.0f;

		if(g_VisualEffectQuality > 1.0f)
			g_VisualEffectQuality = 1.0f;
		else if( g_VisualEffectQuality < 0.1f )
			g_VisualEffectQuality = 0.1f;
			*/
		lev = lev * 1.42857f; // 70% to 100%
		if( lev < 0.0f )
			lev = 0.0f;
		else if( lev > 100.0f )
			lev = 100.0f;

		g_VisualEffectQuality = lev / 100.0f;

		if(g_VisualEffectQuality > 1.0f)
			g_VisualEffectQuality = 1.0f;
		else if( g_VisualEffectQuality < 0.0f )
			g_VisualEffectQuality = 0.0f;
#else				
		lev = lev * 0.7143f; // 70% to 50%
		if( lev > 50.0f )
			lev = 50.0f;
		else if( lev < 0.0f )
			lev = 0.0f;

		g_VisualEffectQuality = (lev / 50.0f) + 0.5f;

		if(g_VisualEffectQuality > 1.0f)
			g_VisualEffectQuality = 1.0f;
		else if( g_VisualEffectQuality < 0.5f )
			g_VisualEffectQuality = 0.5f;
#endif
	}
	
	int tracecount = 2 + (int)(18.0f * (m_pEffectQualitySlider->m_GaugeValue / 100.0f));
	_XEffect_SwordTrace::SetPointStep( tracecount );

	checkbtnstruct.position.y = 239;
	checkbtnstruct.commandid = _XDEF_OPTION_SCREEN_REFLECTION;
	m_pReflectionButton = new _XCheckButton;
	m_pReflectionButton->Create( checkbtnstruct );	
	m_pReflectionButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pReflectionButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	BOOL reflection = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_REFLECTION, TRUE, TRUE );
	if( CWater::m_WaterRenderingMode != _XWATER_RM_EMBM )
	{
		reflection = FALSE;
		m_pReflectionButton->EnableWindow( FALSE );
	}
	
	CWater::m_UseObjectReflection = reflection;

	m_pReflectionButton->SetCheck( reflection );
	m_pReflectionButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pReflectionButton );

	checkbtnstruct.position.y = 258;
	checkbtnstruct.commandid = _XDEF_OPTION_SCREEN_LENSFLARE;
	m_pLensFlareButton = new _XCheckButton;
	m_pLensFlareButton->Create( checkbtnstruct );	
	m_pLensFlareButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pLensFlareButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_EnvironmentManager.m_ViewLensFlare = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LENSFLARE, TRUE, TRUE );	
	m_pLensFlareButton->SetCheck( g_EnvironmentManager.m_ViewLensFlare );
	m_pLensFlareButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pLensFlareButton );

	return TRUE;
}


void _XWindow_OptionGraphic::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
	
	m_pGammaSlider = NULL;
	m_pGammaSlider = NULL;
	m_pTerrainLodSlider = NULL;
	m_pSightRangeSlider = NULL;
	m_pGrassQualitySlider = NULL;
	m_pGrassAnimationButton = NULL;
	m_pTreeAnimationButton = NULL;
	m_pCharacterShadowButton = NULL;
	m_pUseBloomFilterSlider = NULL;
	m_pEffectQualitySlider = NULL;
	m_pReflectionButton = NULL;
	m_pLensFlareButton = NULL;

}


BOOL _XWindow_OptionGraphic::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	

void _XDrawEdgedSolidBar( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color )
{
	_XDrawSolidBar( left+1, top+1, right, bottom, color );
	_XDrawRectAngle( left, top, right, bottom, 0.0f, 0xFF000000 );
}


void _XWindow_OptionGraphic::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 9,  m_WindowPosition.x + 269, m_WindowPosition.y + 153, 0x40000000 );
		
		_XDrawEdgedSolidBar( m_WindowPosition.x + 13, m_WindowPosition.y + 14,  m_WindowPosition.x + 261, m_WindowPosition.y +  55, 0x40000000 );
		_XDrawEdgedSolidBar( m_WindowPosition.x + 13, m_WindowPosition.y + 58,  m_WindowPosition.x + 261, m_WindowPosition.y +  99, 0x40000000 );
		_XDrawEdgedSolidBar( m_WindowPosition.x + 13, m_WindowPosition.y + 102, m_WindowPosition.x + 261, m_WindowPosition.y + 123, 0x40000000 );
		_XDrawEdgedSolidBar( m_WindowPosition.x + 13, m_WindowPosition.y + 126, m_WindowPosition.x + 261, m_WindowPosition.y + 147, 0x40000000 );
		
		_XDrawEdgedSolidBar( m_WindowPosition.x +  6, m_WindowPosition.y + 158, m_WindowPosition.x + 269, m_WindowPosition.y + 297, 0x80000000 );
	}

	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
		
	g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 180, 180, 180 ) );
	
	#ifdef _XRUSSIAN
	int xstpos = 13;
	#else
	int xstpos = 21;
	#endif


	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 21, _XGETINTERFACETEXT(ID_STRING_OPTION_TERRAINQUALITY) );	// _T("지형 그래픽 품질")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 39, _XGETINTERFACETEXT(ID_STRING_OPTION_EFFECTQUALITY) );	// _T("이펙트 그래픽 품질")

	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 64, _XGETINTERFACETEXT(ID_STRING_OPTION_VIEWANGLE) );		// _T("시야 각도")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 82, _XGETINTERFACETEXT(ID_STRING_OPTION_VIEWRANGE) );		// _T("시야 거리")


	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 107, _XGETINTERFACETEXT(ID_STRING_OPTION_USEGRASS) );		// _T("풀 생성 양")

	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 131, _XGETINTERFACETEXT(ID_STRING_OPTION_BRIGHT) );			// _T("게임 밝기 조절")



	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 164, _XGETINTERFACETEXT(ID_STRING_OPTION_MOVEGRASS) );		// _T("풀 움직임")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 183, _XGETINTERFACETEXT(ID_STRING_OPTION_MOVEBACKGROUND) );	// _T("배경 움직임")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 202, _XGETINTERFACETEXT(ID_STRING_OPTION_CHARACTERSHADOW) );	// _T("캐릭터 그림자")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 222, _XGETINTERFACETEXT(ID_STRING_OPTION_SCENFILTERING) );	// _T("화면 필터링")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 240, _XGETINTERFACETEXT(ID_STRING_OPTION_USESHADER) );	// _T("수면 반사효과")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 259, _XGETINTERFACETEXT(ID_STRING_OPTION_LENSFLAIR) );	// _T("렌즈 플레어 효과")
	g_XBaseFont->Puts( m_WindowPosition.x + xstpos, m_WindowPosition.y + 278, _XGETINTERFACETEXT(ID_STRING_OPTION_SPEEDEFFECT) );	// _T("경공 이펙트")

	g_XBaseFont->Flush();
}