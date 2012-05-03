// XProc_CreateCharacter.cpp: implementation of the XProc_CreateCharacter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XProc_CreateCharacter.h"
#include "XWindow_CreateCharacter.h"
#include "XWindow_ZoomWindow.h"
#include "XWindow_RotateWindow.h"
#include "XWindow_SelectLoginServer.h"
#include "XWindow_ExplainCharacter.h"
#include "XSR_STRINGHEADER.H"
#include "FlashControlInterface.h"

extern CFlashControlInterface*	g_pTitleFlashObject;
extern LPDIRECT3DTEXTURE9		g_pTitleFlashOffScreenTexture;
extern void _XDrawEdgedSolidBar ( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );
//////////////////////////////////////////////////////////////////////
// Local definition
int g_Modeltype_UpperBody[2][4] = { { 0, 8821, 8822, 8823 }, { 0, 8830, 8831, 8832 } };
int g_Modeltype_LowerBody[2][4] = { { 0, 8824, 8825, 8826 }, { 0, 8833, 8834, 8835 } };
int g_Modeltype_Hand[2][4]		= { { 0, 8841, 8842, 8843 }, { 0, 8844, 8845, 8846 } };
int g_Modeltype_Foot[2][4]		= { { 0, 8827, 8828, 8829 }, { 0, 8836, 8837, 8838 } };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_CreateCharacter::XProc_CreateCharacter()
{	
	setProcessInfo( 0, "Create character", __FILE__ );
	for( int i = 0; i < _XGROUP_COUNT; i++ )
	{
		m_pMinimapTexture[i] = NULL;
	}
}

XProc_CreateCharacter::~XProc_CreateCharacter()
{
	for( int i = 0; i < _XGROUP_COUNT; i++ )
	{
		SAFE_RELEASE( m_pMinimapTexture[i] );
	}
}

BOOL XProc_CreateCharacter::InitializeProcess(void)
{
#ifdef _XDWDEBUG 
	g_NormalGameExit = TRUE;
#else 
	g_NormalGameExit = FALSE;
#endif

	for( int i = 0; i < _XGROUP_COUNT; i++ )
	{
		SAFE_RELEASE( m_pMinimapTexture[i] );
		m_pMinimapTextureAlphaLevel[i] = 0;
	}
	
	_XSetDefaultPath();

	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = FALSE; // 이플래그를 켜서 캐릭터 리스트를 받아옴.

	// Build Texture =============================================================
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
	
	m_TextureArchive.GetResourceIndex("MI_Login_00.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("MI_Login_01.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("MI_Login_02.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	
#ifdef _XTS_NEWCREATECHARACTER
	m_TextureArchive.GetResourceIndex("MI_CreateCharacter.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
#endif

	m_TextureArchive.BuildTextureList(TRUE);
	
	// window create ===================================================================

	FLOAT xfactor = (FLOAT)gnWidth  / 1024.0f;
	FLOAT yfactor = (FLOAT)gnHeight / 768.0f;

	int halfsize = (196>>1);
	_XWindow_CreateCharacter* pCreateCharacter_Window = new _XWindow_CreateCharacter;
	_XWINDOW_STRUCTURE createcharacterstructure = 
	{
		TRUE, { (int)((700.0f+halfsize)*xfactor) - halfsize, (int)(78*yfactor) }, {196, 508}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(64, 0, 0, 0),
			_XDEF_WTITLE_CREATECHARACTER
	};
	pCreateCharacter_Window->Create(createcharacterstructure);
	pCreateCharacter_Window->Initialize(m_TextureArchive);
	pCreateCharacter_Window->ShowWindow(TRUE);
	pCreateCharacter_Window->EnableWindowEffect(FALSE);
	pCreateCharacter_Window->SetDocking(FALSE);
	m_WindowManager.InsertWindow(pCreateCharacter_Window);

#ifdef _XTS_NEWCREATECHARACTER	
	// 캐릭터 설명창 생성
	_XWindow_ExplainCharacter* pExplainCharacter_Window = new _XWindow_ExplainCharacter;
	_XWINDOW_STRUCTURE explaincharacterstructure = 
	{
		TRUE, { 1, 118 }, {300, 500}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(1, 0, 0, 0),
			_XDEF_WTITLE_EXPLAINCHARACTER
	};
	pExplainCharacter_Window->Create(explaincharacterstructure);
	pExplainCharacter_Window->Initialize(m_TextureArchive);
	pExplainCharacter_Window->ShowWindow(FALSE);
	pExplainCharacter_Window->EnableWindowEffect(FALSE);
	pExplainCharacter_Window->SetDocking(FALSE);
	pExplainCharacter_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	m_WindowManager.InsertWindow(pExplainCharacter_Window);
#endif	
	
	_XIMEKERNEL.SetNativeMode( true );

	m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, NULL, -1 );
	m_BackgroundStatic.m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	int btnimageindex = m_TextureArchive.FindResource("MI_Login_00.tga");
	int edgeimageindex = m_TextureArchive.FindResource("MI_Login_01.tga");
	int overlayimageindex = m_TextureArchive.FindResource("MI_login_02.tga");

	m_BottomOverlayLeftImage.Create( 264, 680-100, 520, 680+28, &m_TextureArchive, overlayimageindex );
	m_BottomOverlayLeftImage.SetClipRect( 0, 0, 256, 128 );

	m_BottomOverlayRightImage.Create( 520, 680-100, 520+256, 680+28, &m_TextureArchive, overlayimageindex );
	m_BottomOverlayRightImage.SetClipRect( 0, 128, 256, 256 );

	m_ZoneNameImage.Create( 227, 112, 227+228, 112+45, &m_TextureArchive, edgeimageindex );
	m_ZoneNameImage.SetClipRect( 30, 215, 256, 256 );
	m_ZoneNameImage.SetScale( -1.0f, 1.0f );

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_MinimapImageStatic.Create( 28, 295+64, 220, 487+64, NULL, -1 );
	}
	else
	{
		m_MinimapImageStatic.Create( 28, 295, 220, 487, NULL, -1 );
	}
	m_MinimapImageStatic.SetClipRect( 0, 0, 256, 256 );
	m_MinimapImageStatic.SetScale( 192.0f / 256.0f, 192.0f / 256.0f );
	
	// Default Model Setting =============================================================
	m_CharacterGender = _XGENDER_MAN;
	m_FaceTypeManIndex = 1;
	m_FaceTypeWomanIndex = 1;
	m_HairTypeWomanIndex = 1;
	m_HairTypeManIndex = 1;
	m_ClothTypeManIndex = 1;
	m_ClothTypeWomanIndex = 1;

	memset( &m_PreviewModelDescript, 0, sizeof(_XUSER_MODELDESCRIPTTYPE) );
	SetCharacter();

	m_PreviewCharacter.ReserveModelStack( _XDEF_MAXMODEL_COMBINATIONCOUNT );
	
	if( m_CharacterGender == _XGENDER_MAN )
	{
		m_PreviewCharacter.LinkBipedObject( &g_MaleBipedObject );
	}
	else
	{
		m_PreviewCharacter.LinkBipedObject( &g_FemaleBipedObject );
	}

	m_PreviewCharacter.SetMeshModel( m_PreviewModelDescript, m_CharacterGender, _XGROUP_NANGIN );
	m_PreviewCharacter.SetShadowDecalInfo( 0.65f, 0.85f, 1.85f );
	m_PreviewCharacter.SetCurMotion( 0 );
	m_PreviewCharacter.PlayAnimation( TRUE );
	m_PreviewCharacter.m_CameraDistance = 1.0f;

	m_Parameter[0] = _XDEF_DEFALTPARAMETER_1;
	m_Parameter[1] = _XDEF_DEFALTPARAMETER_2;
	m_Parameter[2] = _XDEF_DEFALTPARAMETER_3;
	m_Parameter[3] = _XDEF_DEFALTPARAMETER_4;
	m_Parameter[4] = _XDEF_DEFALTPARAMETER_5;	

	memset( m_CharacterNameString, 0, sizeof(TCHAR) * 13 );

	m_PrevSendedCreateCharacterTime = 0;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );
#ifdef _XTESTSERVER			
	if( !g_TerrainTextureArchive.m_RenderStyleManager.Load( _T("tsEnvironment.xrs") ) ) return false;	
#else
	if( !g_TerrainTextureArchive.m_RenderStyleManager.Load( _T("Environment.xrs") ) ) return false;	
#endif
	
	if( !CreateOffScreen() )
	{
		return FALSE;
	}
	
	if( !g_pTitleFlashObject )
	{
		g_pTitleFlashObject = GetFlashControl();
		
		if( !g_pTitleFlashObject ) return FALSE;
		
		g_pTitleFlashObject->initialize( gHWnd,gHInst,g_pTitleFlashOffScreenTexture);
		g_pTitleFlashObject->SetLowQuality();
	}
	
	HDC pDC = GetDC( gHWnd );
	g_pTitleFlashObject->SetDC( pDC );
	
	TCHAR flashfilename[64];

	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strcpy(flashfilename, _T("MAKING00.swf"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy(flashfilename, _T("vn_MAKING00.swf"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy(flashfilename, _T("tw_MAKING00.swf"));
	}
	else
	{
		strcpy(flashfilename, _T("MAKING00.swf"));
	}
	
	_XPackageArchive package;
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
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
	g_pTitleFlashObject->FreeBuffer();
	g_pTitleFlashObject->ControlClose();
	int resourceid = package.FindResource( flashfilename );
	if( resourceid >= 0 )
	{
		long filesize = package.GetPackedFileSize( resourceid );
		g_pTitleFlashObject->SetResourceFilePtr( package.GetPackedFile( resourceid ), filesize );
		g_pTitleFlashObject->ControlOpen( flashfilename );
		g_pTitleFlashObject->SetLowQuality();
	}

	package.FinalizePackage();
	
	g_pTitleFlashObject->SetDC( 0 );
	ReleaseDC( gHWnd, pDC );
	
	g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );
	
	g_EnvironmentManager.m_ObjectLight.Position = D3DXVECTOR3( -100.0f, 40.0f, -100.0f );

	m_SelectedZone = 0;
	
#ifdef _XTS_NEWCREATECHARACTER	
#else
	for( i = 0; i < _XGROUP_COUNT; i++ )
	{
		LoadMinimapTexture( i, g_ZoneInfoTable[i].zonefilename );
	}
#endif
			
	InitDeviceObject();	
	
	// Initialize live packet timer
	m_LivePacketTimer = 0;
				
	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

	return TRUE;
}

BOOL XProc_CreateCharacter::CreateOffScreen( void )
{
	if( g_pTitleFlashOffScreenTexture ) return TRUE;
	
	SIZE size = m_BackgroundStatic.GetWindowSize();
	
	if( FAILED( gpDev->CreateTexture( size.cx, size.cy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTitleFlashOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
	
	return TRUE;
}

void XProc_CreateCharacter::DestroyProcess( void )
{
	m_TextureArchive.DisposeTexture();
	m_WindowManager.DestroyManager();
	m_PreviewCharacter.Reset();

	g_EnvironmentManager.m_ObjectLight.Position = D3DXVECTOR3( 100.0f, 40.0f, -100.0f );

	g_QuestScriptManager.SetQuestCameraMode(FALSE);

	for( int i = 0; i < _XGROUP_COUNT; i++ )
	{
		SAFE_RELEASE( m_pMinimapTexture[i] );
	}
	
//	if(g_BgmPlayFlag && g_BgmObject.Getm_dwDeviceID())  
//		g_BgmObject.Close();
	SetEnableProcess( FALSE );	
}

bool XProc_CreateCharacter::InitDeviceObject(void)
{
	m_TextureArchive.InitDeviceObject();
	
	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = gnWidth;
	vp.Height = gnHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	gpDev->SetViewport(&vp);

	gpDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	gpDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

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
	gpDev->SetRenderState( D3DRS_DEPTHBIAS ,			 0 );
	
#ifdef _XDEF_USETERRAINDIFFUSECOLOR
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_COLOR1 );
#else
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
#endif

	gpDev->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetRenderState( D3DRS_LOCALVIEWER, FALSE );
	
	gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	
	// Alpha blend state
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );		
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	// Alpha test state
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	g_QuestScriptManager.SetQuestCameraMode(FALSE);
	g_LodTerrain.m_3PCamera.Set( 45.0f, (float)gnWidth/(float)gnHeight, 1.0f, 100.0f );
	g_LodTerrain.m_3PCamera.SetPitchLimits( -50.0f, 73.0f);
	g_LodTerrain.m_3PCamera.SetRollLimits(-0, 0);
	g_LodTerrain.m_3PCamera.SetDistanceLimits(1.4f, 10.f);
	g_LodTerrain.m_3PCamera.SetDistance( 2.6f );	
	g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3( 0.125f, 0.495f, 0.0f ) );
	g_LodTerrain.m_3PCamera.mp_fYaw = 0.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 180.0f;
	g_LodTerrain.m_3PCamera.mp_fRoll = 0.0f;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix();
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();

	m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
	m_CameraMovingTargetDistance = 2.6f;
	m_CameraMoving = FALSE;

	//g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3( 0.125f, 0.895f, 0.0f ) );

	m_PreviewCharacterRotateAngle = 16.0f;
	D3DXMatrixRotationY( &m_PreviewCharacter.m_Position, _X_RAD( m_PreviewCharacterRotateAngle ) );
	
	D3DUtil_InitLight( g_EnvironmentManager.m_CharacterLight, D3DLIGHT_DIRECTIONAL, 100.0f, 40.0f, -100.0f );
	D3DXVECTOR3 ldnv;
	
	if( !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation )
	{
		ldnv = D3DXVECTOR3( 0.650945f, -0.390567f, 0.650945f );
		D3DXVec3Normalize( &ldnv, &ldnv );
		g_EnvironmentManager.m_CharacterLight.Direction = g_EnvironmentManager.m_ObjectLight.Direction = ldnv;	

		g_EnvironmentManager.m_CharacterLight.Diffuse.r = 0.68F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.g = 0.68F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.b = 0.68F;
		
		g_EnvironmentManager.m_CharacterLight.Ambient.r = 0.43f;
		g_EnvironmentManager.m_CharacterLight.Ambient.g = 0.43f;
		g_EnvironmentManager.m_CharacterLight.Ambient.b = 0.43f;
	}
	else
	{
		ldnv = D3DXVECTOR3( 0.650945f, -0.390567f, 0.650945f );
		D3DXVec3Normalize( &ldnv, &ldnv );
		g_EnvironmentManager.m_CharacterLight.Direction = g_EnvironmentManager.m_ObjectLight.Direction = ldnv;		

		g_EnvironmentManager.m_CharacterLight.Diffuse.r = 0.78F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.g = 0.78F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.b = 0.78F;
		
		g_EnvironmentManager.m_CharacterLight.Ambient.r = 1.0f;
		g_EnvironmentManager.m_CharacterLight.Ambient.g = 1.0f;
		g_EnvironmentManager.m_CharacterLight.Ambient.b = 1.0f;
	}
	

	CreateOffScreen();
	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );
	}

	return true;
}

void XProc_CreateCharacter::ReleaseDeviceObject( void )
{		
	
}

void XProc_CreateCharacter::PauseProcess( void )
{

}

void XProc_CreateCharacter::UnPauseProcess( void )
{
	
}

void XProc_CreateCharacter::Draw(void)
{
	// Begin the scene
	gpDev->BeginScene();	

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Set Third person view matrix
	gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
	gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());
		
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState( D3DRS_LIGHTING, FALSE );	
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
	m_BackgroundStatic.DrawWithRegion( g_pTitleFlashOffScreenTexture );


	if( !g_pTitleFlashObject->IsPlaying() )
	{
		// Draw Character ===============================================
		if(m_CharacterGender == _XGENDER_MAN)
			m_PreviewCharacter.SetCurMotion(0);
		else
			m_PreviewCharacter.SetCurMotion(0);

		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight);

		bool playsound = true;
		m_PreviewCharacter.Advance(playsound);
		m_PreviewCharacter.Animation();
		m_PreviewCharacter.RenderShadow();
		m_PreviewCharacter.RenderCharacter( NULL );

		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );

		gpDev->LightEnable( 1, FALSE );	
		gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
		
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );

		m_ZoneNameImage.Draw();
		
		if( m_SelectedZone >= 1 )
		{
			
#ifdef _XTS_NEWCREATECHARACTER	
#else
			g_XBaseFont->SetColor( 0xFFFFFFFF );
			g_XBaseFont->PrintAlign( 116, 128, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s(%s)", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname, g_ZoneInfoTable[m_SelectedZone-1].zonename2 );
			g_XBaseFont->PrintAlign( 117, 128, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s(%s)", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname, g_ZoneInfoTable[m_SelectedZone-1].zonename2 );
			g_XBaseFont->Flush();		

			g_XBaseFont->SetColor( 0xFFC9BBB8 );
			g_XBaseFont->Puts_Separate( 28, 169, g_ZoneExplaneString[m_SelectedZone-1], 196, 1.0f, 3 );
			g_XBaseFont->Flush();
		
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				_XDrawEdgedSolidBar( 27, 294+64, 220, 487+64, 0x80000000 );
			}
			else
			{			
				_XDrawEdgedSolidBar( 27, 294, 220, 487, 0x80000000 );
			}

			const alphaanispeed = 10;
		
			for( int i = 0; i < _XGROUP_COUNT; i++ )
			{
				if( m_SelectedZone - 1 == i)
				{
					if( m_pMinimapTextureAlphaLevel[i] < 255 )
					{
						m_pMinimapTextureAlphaLevel[i] += alphaanispeed;
						if( m_pMinimapTextureAlphaLevel[i] > 255 )
						{
							m_pMinimapTextureAlphaLevel[i] = 255;
						}
					}
				}
				else
				{
					if( m_pMinimapTextureAlphaLevel[i] > 0 )
					{
						m_pMinimapTextureAlphaLevel[i] -= alphaanispeed;
						if( m_pMinimapTextureAlphaLevel[i] < 0 )
						{
							m_pMinimapTextureAlphaLevel[i] = 0;
						}
					}
				}

				if( m_pMinimapTextureAlphaLevel[i] > 0 )
				{
					m_MinimapImageStatic.m_FColor = D3DCOLOR_ARGB( m_pMinimapTextureAlphaLevel[i], 0xFF,0xFF,0xFF );
					m_MinimapImageStatic.DrawWithRegion( m_pMinimapTexture[i] );
				}
			}
#endif
		}

		//m_BottomOverlayLeftImage.Draw();
		//m_BottomOverlayRightImage.Draw();
		//_XDrawSolidBar( 264, 680+28, 776, 768, 0xFF000000 );

		m_WindowManager.Draw();
		g_IMEWindow.Draw();	
		g_MessageBox.Draw();
		
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Puts( 16, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_CREATECHARACTERINFO) );	// "새로운 인물의 정보를 입력하여 주십시오"
		g_XBaseFont->Puts( 17, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_CREATECHARACTERINFO) );	// "새로운 인물의 정보를 입력하여 주십시오"
		g_XBaseFont->Flush();
	}


	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		
#ifdef _XDWDEBUG		
	if( gpInput->CheckKeyPress(DIK_LCONTROL) )
	{
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Print( 4, gnHeight-13, 1.0f, "FPS: %d", gnFPS );
		g_XBaseFont->Print( 4, gnHeight-39, 1.0f, "X: %f Y: %f Z: %f Yaw: %f Pitch: %f Roll: %f",
			g_LodTerrain.m_3PCamera.m_CameraPosition.x,g_LodTerrain.m_3PCamera.m_CameraPosition.y,
			g_LodTerrain.m_3PCamera.m_CameraPosition.z, g_LodTerrain.m_3PCamera.GetYaw(),
			g_LodTerrain.m_3PCamera.GetPitch(),g_LodTerrain.m_3PCamera.GetRoll() );
		g_XBaseFont->Print( 4, gnHeight-26, 1.0f, "Target X: %f Y: %f Z: %f Distance : %f %f",
			g_LodTerrain.m_3PCamera.m_TargetPosition.x,g_LodTerrain.m_3PCamera.m_TargetPosition.y,
			g_LodTerrain.m_3PCamera.m_TargetPosition.z,g_LodTerrain.m_3PCamera.m_TargetDistance, m_PreviewCharacterRotateAngle);
		g_XBaseFont->Flush();
	}
#endif	
	
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	
	gpDev->SetRenderState( D3DRS_ZENABLE,   g_RS_UseZBuffer );
	
	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;
}

void XProc_CreateCharacter::OnPaint(WPARAM wparam, LPARAM lparam)
{	
	Draw();
}

bool XProc_CreateCharacter::Process( void )
{	
	if(g_pTitleFlashObject) g_pTitleFlashObject->PlayTimerFire();

	// 서버 연결 유지 패킷 전송
	if( m_LivePacketTimer == 0 )
	{
		m_LivePacketTimer = g_LocalSystemTime;
	}
	
	if( g_LocalSystemTime - m_LivePacketTimer > 180000 ) // 3분에 한번씩 전송
	{		
		m_LivePacketTimer = g_LocalSystemTime;
		g_NetworkKernel.SendPacket( MSG_NO_TIME_CHECK );
	}

	g_MessageBox.Process();	
	g_IMEWindow.Process();
	m_WindowManager.Process();

	if( m_CameraMoving )
	{
		FLOAT fFrameSpeed = 0.0f;

		FLOAT distance = _XMeshMath_DistanceBetweenTwoPoint( m_CameraMovingTarget, g_LodTerrain.m_3PCamera.m_TargetPosition );
		FLOAT maxdistance = _XMeshMath_DistanceBetweenTwoPoint( D3DXVECTOR3( 0.125f, 0.495f, 0.0f ), D3DXVECTOR3( 0.125f, 0.895f, 0.0f ) );

		if( maxdistance - distance < 0.08f )
			 fFrameSpeed = g_fElapsedFrameMilisecondTime / 4.0f;
		else if( maxdistance - distance < 0.18f )
			 fFrameSpeed = g_fElapsedFrameMilisecondTime / 1.2f;
		else 
		{
			if( distance < 0.08f )
				fFrameSpeed = g_fElapsedFrameMilisecondTime / 2.0f;
			else if( distance < 0.18f )
				fFrameSpeed = g_fElapsedFrameMilisecondTime;
			else
				fFrameSpeed = g_fElapsedFrameMilisecondTime * 2.5f;
		}

		BOOL zooming = TRUE;
		if( m_CameraMovingTargetDistance > g_LodTerrain.m_3PCamera.GetDistance() )
		{
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() + fFrameSpeed * 5.0f );

			if( m_CameraMovingTargetDistance < g_LodTerrain.m_3PCamera.GetDistance() )
			{
				g_LodTerrain.m_3PCamera.SetDistance( m_CameraMovingTargetDistance );
				zooming = FALSE;
			}
		}
		else if( m_CameraMovingTargetDistance < g_LodTerrain.m_3PCamera.GetDistance() )
		{
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - fFrameSpeed * 5.0f );

			if( m_CameraMovingTargetDistance > g_LodTerrain.m_3PCamera.GetDistance() )
			{
				g_LodTerrain.m_3PCamera.SetDistance( m_CameraMovingTargetDistance );
				zooming = FALSE;
			}
		}
		else
		{
			zooming = FALSE;
		}
				
		if( distance < EPSILON3 )
		{
			g_LodTerrain.m_3PCamera.m_TargetPosition = m_CameraMovingTarget;
			g_LodTerrain.m_3PCamera.UpdateViewMatrix();
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();

			if( !zooming )
				m_CameraMoving = FALSE;
		}
		else
		{
			D3DXVECTOR3 direction = m_CameraMovingTarget - g_LodTerrain.m_3PCamera.m_TargetPosition;
			D3DXVec3Normalize( &direction, &direction );

			D3DXVECTOR3 prevPosition = g_LodTerrain.m_3PCamera.m_TargetPosition;

			g_LodTerrain.m_3PCamera.m_TargetPosition += direction * fFrameSpeed * 1.68f;

			FLOAT prevdistance = _XMeshMath_DistanceBetweenTwoPoint( prevPosition, m_CameraMovingTarget );
			FLOAT newdistance = _XMeshMath_DistanceBetweenTwoPoint( m_CameraMovingTarget, g_LodTerrain.m_3PCamera.m_TargetPosition );

			if( prevdistance < newdistance )
			{
				g_LodTerrain.m_3PCamera.m_TargetPosition = m_CameraMovingTarget;
				
				if( !zooming )
					m_CameraMoving = FALSE;
			}

			g_LodTerrain.m_3PCamera.UpdateViewMatrix();
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		}
	}

#ifdef _XUSEFMOD
	g_FMODWrapper.ProcessBGM();
	g_FMODWrapper.FMODUpdate();	
#endif

	return true;
}

void XProc_CreateCharacter::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	_XWindow_CreateCharacter* pCreateCharacter_Window = (_XWindow_CreateCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_CREATECHARACTER);
	
	if( message == WM_COMMAND )
	{
		switch( wparam )
		{		
		case _XDEF_CC_CREATEBUTTON  :
			
			strcpy(  m_CharacterNameString, pCreateCharacter_Window->m_NameEdit.GetText() );

			if( strlen(m_CharacterNameString) <= 0 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_EMPTYNAME, TRUE );
			}
#ifdef _XJAPANESE
			else if( strlen(m_CharacterNameString) <= 2 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_SHORTNAME, TRUE );
			}			
#else
			else if( strlen(m_CharacterNameString) <= 1 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_SHORTNAME, TRUE );
			}			
#endif
			else if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_USEPOINT, TRUE );
			}			
			else
			{	

				TCHAR _HeadFiltering[25][128];
				memset( _HeadFiltering, 0, sizeof(TCHAR)*25*128);

				strcpy( _HeadFiltering[0], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1783) );
				strcpy( _HeadFiltering[1], _T("NINEDRAGONS") );
				strcpy( _HeadFiltering[2], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1784) );
				strcpy( _HeadFiltering[3], _T("INDY21") );
				strcpy( _HeadFiltering[4], _T("INDY") );
				strcpy( _HeadFiltering[5], _T("GM") );
				strcpy( _HeadFiltering[6], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1785) );
				strcpy( _HeadFiltering[7], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1786) );
				strcpy( _HeadFiltering[8], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1787) );
				strcpy( _HeadFiltering[9], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1788) );

				strcpy( _HeadFiltering[10], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );
				strcpy( _HeadFiltering[11], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );
				strcpy( _HeadFiltering[12], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );
				strcpy( _HeadFiltering[13], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );
				strcpy( _HeadFiltering[14], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1789) );
				strcpy( _HeadFiltering[15], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );
				strcpy( _HeadFiltering[16], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1790) );
				strcpy( _HeadFiltering[17], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );
				strcpy( _HeadFiltering[18], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07) );
				strcpy( _HeadFiltering[19], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00) );

				strcpy( _HeadFiltering[20], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1791) );
				strcpy( _HeadFiltering[21], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1792) );
				strcpy( _HeadFiltering[22], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1793) );
				strcpy( _HeadFiltering[23], _T("9DRAGONS") );
				strcpy( _HeadFiltering[24], _T("9D") );
				
				TCHAR warnstring[512];
				strcpy(warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERNAME));

				LPTSTR tempcharacternamestring = _strupr( _strdup( m_CharacterNameString ) );
				
				if( tempcharacternamestring )
				{	

					for( int idx = 0; idx < 25; idx++ )
					{					
						if( strncmp( tempcharacternamestring, _HeadFiltering[idx], strlen(_HeadFiltering[idx]) ) == 0 )
						{
							TCHAR messagestring[128];
							memset(messagestring, 0, sizeof(messagestring));
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1794), _HeadFiltering[idx] );	
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, messagestring, TRUE );
							free( tempcharacternamestring );
							return;
						}
					}

					if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
					{
						if( strstr( tempcharacternamestring, _T("NEXON") ) != NULL ) // 캐릭터 이름내에  NEXON 사용 불가
						{
							TCHAR messagestring[128];
							memset(messagestring, 0, sizeof(messagestring));
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1794), _T("NEXON") );	
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, messagestring, TRUE );
							free( tempcharacternamestring );
							return;
						}
						else if( strstr( tempcharacternamestring, _T("넥슨") ) != NULL ) // 캐릭터 이름내에  NEXON 사용 불가
						{
							TCHAR messagestring[128];
							memset(messagestring, 0, sizeof(messagestring));
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1794), _T("넥슨") );	
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, messagestring, TRUE );
							free( tempcharacternamestring );
							return;
						}
						else 
						{
							LPTSTR pFirstPT = NULL, pSecondPT = NULL, pThirdPT = NULL, pFourth = NULL;
							if( (pFirstPT = strstr( tempcharacternamestring, _T("현") ) ) != NULL && 
								(pSecondPT = strstr( tempcharacternamestring, _T("공") ) ) != NULL && 
								(pThirdPT = strstr( tempcharacternamestring, _T("구") ) ) != NULL && 
								(pFourth = strstr( tempcharacternamestring, _T("선") ) ) != NULL ) // 캐릭터 이름내에  현공구선 사용 불가
							{
								if( (pFirstPT < pSecondPT) && (pSecondPT < pThirdPT ) &&  (pThirdPT < pFourth) )
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));
									_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1794), _T("현공구선") );	
									g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, messagestring, TRUE );
									free( tempcharacternamestring );
									return;
								}
							}
						}

						int strlength = strlen(  tempcharacternamestring );

						if( strlength > 1 )
						{
							for( int i = 0; i < strlength - 1; i++ )
							{
								if( i > 4 ) // 4자 이후의 GM은 필터링 안함.
								{
									break;
								}
								if( tempcharacternamestring[i] == _T('G') && tempcharacternamestring[i+1] == _T('M') )
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));
									_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1794), _T("GM") );	
									g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, messagestring, TRUE );
									free( tempcharacternamestring );
									return;
								}
							}
						}
					}

					free( tempcharacternamestring );
				}			

				wchar_t invalidcharacter = 0;
				_XCREATECHARACTERRESULT namestringcheckresult;
				
				if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
				   g_LanguageType == _XLANGUAGE_TYPE_JAPANESE  || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					namestringcheckresult = _XCheckUsableCharacterName_Unicode( pCreateCharacter_Window->m_NameEdit.GetUnicodeText(), invalidcharacter );
				}
				else
				{
					namestringcheckresult = _XCheckUsableCharacterName( pCreateCharacter_Window->m_NameEdit.GetText() );
				}

				switch( namestringcheckresult ) 
				{
				case _XCREATECHARACTERRESULT_STRINGISNULL :
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_MEMORYERROR), TRUE );	// _T("인물의 이름이 너무 짧거나 메모리 할당에 실패했습니다.")
						pCreateCharacter_Window->m_NameEdit.SetFocus();
					}
					break;
				case _XCREATECHARACTERRESULT_LENGTHISZERO :
					{
						// 인물의 이름이 너무 짧습니다. 한글은 1~6자 영문은 2~12자까지 지을 수 있습니다.
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SHORTNAME), TRUE );
						pCreateCharacter_Window->m_NameEdit.SetFocus();
					}
					break;
				case _XCREATECHARACTERRESULT_INCLUDESPACE :
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_BLANK), TRUE );	// _T("인물 이름에 공백은 사용하실수 없습니다.")
						pCreateCharacter_Window->m_NameEdit.SetFocus();
					}
					break;
				case _XCREATECHARACTERRESULT_INVALIDCHARACTER :
					{
						TCHAR message[512];
						memset( message, 0x00, sizeof(message) );
						_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SPECIALCHAR), g_LastErrorCheckedCharacterName );	// "[%s]와(과) 같은 확장 한글, 한자, 특수문자는 사용하실 수 없습니다."
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
						pCreateCharacter_Window->m_NameEdit.SetFocus();
					}
					break;
				case _XCREATECHARACTERRESULT_SINGLEKOREANCHARACTER :
					{
						TCHAR message[512];
						memset( message, 0x00, sizeof(message) );
						_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ERRORCHAR), g_LastErrorCheckedCharacterName );	// "[%s]와(과) 같은 한글 낱자는 사용하실 수 없습니다."
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
						pCreateCharacter_Window->m_NameEdit.SetFocus();
					}
					break;
				case _XCREATECHARACTERRESULT_NOVALIDASCIICHARACTER :
					{
						TCHAR message[512];
						memset( message, 0x00, 512 );

						if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
						{
							TCHAR tempstring[10];
							memset( tempstring, 0, sizeof(tempstring) );
							itoa( invalidcharacter, tempstring, 16 );
							TCHAR tempstring2[10];
							memset( tempstring2, 0, sizeof(tempstring2) );
							strncpy( tempstring2, _T("0X"), sizeof(tempstring2) );
							strcat( tempstring2, tempstring );
							_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ERRORASCII), tempstring2 );	// "[%s]와(과) 같은 ASCII 문자는 사용하실 수 없습니다. (기호문자는 -,_,[,]만 가능합니다)"
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
							pCreateCharacter_Window->m_NameEdit.SetFocus();
						}
						else
						{
							_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ERRORASCII), g_LastErrorCheckedCharacterName );	// "[%s]와(과) 같은 ASCII 문자는 사용하실 수 없습니다. (기호문자는 -,_,[,]만 가능합니다)"
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
							pCreateCharacter_Window->m_NameEdit.SetFocus();
						}
					}
					break;
				case _XCREATECHARACTERRESULT_NOERROR :
					{
						TCHAR resultstring[256]; 
						memset( resultstring, 0x00, sizeof(resultstring) );
						strncpy( resultstring, pCreateCharacter_Window->m_NameEdit.GetText(), sizeof(resultstring) );
						/*
#ifdef _XTS_CHARACTERFILTERING
						if(!g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_NAME | _XDEF_WORDFILETERINGTYPE_ALWAYS, TRUE ))
#else
						*/
						g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_NAME | _XDEF_WORDFILETERINGTYPE_ALWAYS );
						
						// 필터링 통과
						if( strcmpi( resultstring, pCreateCharacter_Window->m_NameEdit.GetText() ) == 0 ) 
//#endif
						{
							g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_ALWAYS ); // 욕설만으로 필터링을 한번 더 체크 한다.

							if( strcmpi( resultstring, pCreateCharacter_Window->m_NameEdit.GetText() ) == 0 ) 
							{
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_CREATECHARACTER_CREATECHARACTER, TRUE, _XDEF_CC_CREATECHARACTER );
							}
							else
							{
								TCHAR message[512];
								memset( message, 0x00, sizeof(message) );
								_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERCHAR), g_WordFilteringManager.GetLastFilterdWord() );	// "%s(은)는 사용하실 수 없는 단어입니다."
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
								pCreateCharacter_Window->m_NameEdit.SetFocus();
							}
						}				
						else
						{
							TCHAR message[512];
							memset( message, 0x00, sizeof(message) );
							_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERCHAR), g_WordFilteringManager.GetLastFilterdWord() );	// "%s(은)는 사용하실 수 없는 단어입니다."
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
							pCreateCharacter_Window->m_NameEdit.SetFocus();
						}
					}
					break;
				}				
			}
			break;		
		case _XDEF_CC_CREATECHARACTER:
			{
				if( m_SelectedZone >= 1 )
				{
					BOOL gendercheck = TRUE;
				
					if( m_SelectedZone == 1 ) // 소림일때 
					{
						if( m_CharacterGender != _XGENDER_MAN )
						{
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_GROUPERRSORIM), TRUE );	// _T("소림사에는 여자가 출입할 수 없습니다.")
							gendercheck = FALSE;
						}
					}
					else if( m_SelectedZone == 4 ) // 비궁일때 
					{
						if( m_CharacterGender != _XGENDER_WOMAN )
						{
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_GROUPERRBEEGOONG), TRUE );	// _T("자하동에는 남자가 출입할 수 없습니다.")
							gendercheck = FALSE;
						}
					}					
					
					if( gendercheck )
					{
						strcpy(  m_CharacterNameString, pCreateCharacter_Window->m_NameEdit.GetText() );
						if( pCreateCharacter_Window->m_BonusPoint == 0 && ( strlen(m_CharacterNameString) > 0 ) )
						{
							g_NetworkKernel.SendPacket( MSG_NO_CHARAC_CREATE );
						}
					}
				}
				else
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SELECTSTARTZONE), TRUE );	// _T("시작할 지역을 선택하여 주십시오.")
				}
			}			
			break;		
		case _XDEF_CC_CANCELBUTTON	:
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_PrevProcess_Login = FALSE;
				memset( ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName, 0, sizeof(((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName) );
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter );
			}
			break;
		case _XDEF_CC_EXITBUTTON	:
			{
				SetFocus( gHWnd );				
				g_NetworkKernel.DisconnectServer();
				g_NetworkKernel.ReleaseNetwork();
				g_NetworkKernel.InitializeNetwork();
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_PrevProcess_SelectCharacter = TRUE;
				memset( ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName, 0, sizeof(((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName) );
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(  &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
			}
			break;
		case _XDEF_CC_REQUESTCHARACTERLIST:
			{
				SetFocus( gHWnd );
				((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_PrevProcess_Login = FALSE;
				
				memset( ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName, 0, sizeof(((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName) );
				strncpy( ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName, m_CharacterNameString, sizeof(((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName) );
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter );
			}
			break;
		// 성별
		case _XDEF_CC_WOMENBUTTON :
			{
				pCreateCharacter_Window->m_ManButton->SetCheck( FALSE );
				pCreateCharacter_Window->m_WomanButton->SetCheck( TRUE );

				m_CharacterGender = _XGENDER_WOMAN;
				m_PreviewCharacter.LinkBipedObject( &g_FemaleBipedObject );
				SetCharacter();
				
				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;

				pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeWomanIndex;
				pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeWomanIndex;
				pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeWomanIndex;
			}
			break;
		case _XDEF_CC_MANBUTTON	:
			{
				pCreateCharacter_Window->m_ManButton->SetCheck( TRUE );
				pCreateCharacter_Window->m_WomanButton->SetCheck( FALSE );

				m_CharacterGender = _XGENDER_MAN;
				m_PreviewCharacter.LinkBipedObject( &g_MaleBipedObject );
				SetCharacter();
				
				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;

				pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeManIndex;
				pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeManIndex;
				pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeManIndex;
			}
			break;

		// 얼굴
		case _XDEF_CC_FACELEFTBUTTON :
			{
				//if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if( m_CharacterGender == _XGENDER_MAN )
					{
						m_FaceTypeManIndex--;
						if(m_FaceTypeManIndex == 0)
							m_FaceTypeManIndex = 8;						

						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeManIndex;
					}
					else
					{
						m_FaceTypeWomanIndex--;
						if(m_FaceTypeWomanIndex == 0)
							m_FaceTypeWomanIndex = 8;						

						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;
		case _XDEF_CC_FACERIGHTBUTTON :
			{
				//if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if( m_CharacterGender == _XGENDER_MAN )
					{
						m_FaceTypeManIndex++;
						if(m_FaceTypeManIndex == 9)
							m_FaceTypeManIndex = 1;						
						
						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeManIndex;
					}
					else
					{
						m_FaceTypeWomanIndex++;
						if(m_FaceTypeWomanIndex == 9)
							m_FaceTypeWomanIndex = 1;						

						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;

		// 머리
		case _XDEF_CC_HAIRLEFTBUTTON :
			{
				//if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						m_HairTypeManIndex--;
						if(m_HairTypeManIndex == 0)
							m_HairTypeManIndex = 21;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeManIndex;
					}
					if(m_CharacterGender == _XGENDER_WOMAN)
					{
						m_HairTypeWomanIndex--;
						if(m_HairTypeWomanIndex == 0)
							m_HairTypeWomanIndex = 21;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeWomanIndex;
					}
					SetCharacter();					
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;
		case _XDEF_CC_HAIRRIGHTBUTTON :
			{
				//if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						m_HairTypeManIndex++;
						if(m_HairTypeManIndex == 22)
							m_HairTypeManIndex = 1;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeManIndex;
					}
					if(m_CharacterGender == _XGENDER_WOMAN)
					{
						m_HairTypeWomanIndex++;
						if(m_HairTypeWomanIndex == 22)
							m_HairTypeWomanIndex = 1;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;

		// 의상
		case _XDEF_CC_CLOTHLEFTBUTTON :
			{
				//if( g_LodTerrain.m_3PCamera.GetDistance() == 2.6f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						if( m_ClothTypeManIndex == 1 )
						{
							m_ClothTypeManIndex = 3;
						}
						else
						{
							m_ClothTypeManIndex--;
						}
						
						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeManIndex;
					}
					else
					{
						if( m_ClothTypeWomanIndex == 1 )
						{
							m_ClothTypeWomanIndex = 3;
						}
						else
						{
							m_ClothTypeWomanIndex--;
						}

						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeWomanIndex;
					}					
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;
			}
			break;
		case _XDEF_CC_CLOTHRIGHTBUTTON :
			{
				//if( g_LodTerrain.m_3PCamera.GetDistance() == 2.6f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						if( m_ClothTypeManIndex == 3 )
						{
							m_ClothTypeManIndex = 1;
						}
						else
						{
							m_ClothTypeManIndex++;
						}

						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeManIndex;
					}
					else
					{
						if( m_ClothTypeWomanIndex == 3 )
						{
							m_ClothTypeWomanIndex = 1;
						}
						else
						{
							m_ClothTypeWomanIndex++;
						}

						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;
			}
			break;

		case _XDEF_CC_ADDBUTON1 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[0]++;
				pCreateCharacter_Window->m_Parameter[0] = m_Parameter[0];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON1 :
			if( m_Parameter[0] > _XDEF_DEFALTPARAMETER_1 )
			{
				m_Parameter[0]--;
				pCreateCharacter_Window->m_Parameter[0] = m_Parameter[0];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON2 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[1]++;
				pCreateCharacter_Window->m_Parameter[1] = m_Parameter[1];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON2 :
			if( m_Parameter[1] > _XDEF_DEFALTPARAMETER_2 )
			{
				m_Parameter[1]--;
				pCreateCharacter_Window->m_Parameter[1] = m_Parameter[1];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON3 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[2]++;
				pCreateCharacter_Window->m_Parameter[2] = m_Parameter[2];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON3 :
			if( m_Parameter[2] > _XDEF_DEFALTPARAMETER_3 )
			{
				m_Parameter[2]--;
				pCreateCharacter_Window->m_Parameter[2] = m_Parameter[2];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON4 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[3]++;
				pCreateCharacter_Window->m_Parameter[3] = m_Parameter[3];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON4 :
			if( m_Parameter[3] > _XDEF_DEFALTPARAMETER_4 )
			{
				m_Parameter[3]--;
				pCreateCharacter_Window->m_Parameter[3] = m_Parameter[3];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON5 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[4]++;
				pCreateCharacter_Window->m_Parameter[4] = m_Parameter[4];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON5 :
			if( m_Parameter[4] > _XDEF_DEFALTPARAMETER_5 )
			{
				m_Parameter[4]--;
				pCreateCharacter_Window->m_Parameter[4] = m_Parameter[4];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_AREACHECKBUTTON1 :
		case _XDEF_CC_AREACHECKBUTTON2 :
		case _XDEF_CC_AREACHECKBUTTON3 :
		case _XDEF_CC_AREACHECKBUTTON4 :
		case _XDEF_CC_AREACHECKBUTTON5 :
		case _XDEF_CC_AREACHECKBUTTON6 :
		case _XDEF_CC_AREACHECKBUTTON7 :
		case _XDEF_CC_AREACHECKBUTTON8 :
		case _XDEF_CC_AREACHECKBUTTON9 :
			{
				m_SelectedZone = (wparam - _XDEF_CC_AREACHECKBUTTON1) + 1;
				
				pCreateCharacter_Window->m_pAreaSelectCheckButton[0]->SetCheck( (m_SelectedZone) == 1 );
				pCreateCharacter_Window->m_pAreaSelectCheckButton[1]->SetCheck( (m_SelectedZone) == 2 );
				pCreateCharacter_Window->m_pAreaSelectCheckButton[2]->SetCheck( (m_SelectedZone) == 3 );
				pCreateCharacter_Window->m_pAreaSelectCheckButton[3]->SetCheck( (m_SelectedZone) == 4 );
				if( g_bNewClanQuest )
				{
					pCreateCharacter_Window->m_pAreaSelectCheckButton[4]->SetCheck( (m_SelectedZone) == 5 );
					pCreateCharacter_Window->m_pAreaSelectCheckButton[5]->SetCheck( (m_SelectedZone) == 6 );
				}				
#ifdef _XTS_NEWCREATECHARACTER					
				_XWindow_ExplainCharacter* pExplainCharacter_Window = (_XWindow_ExplainCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_EXPLAINCHARACTER);
				if( pExplainCharacter_Window )
				{
					pExplainCharacter_Window->ShowWindow(TRUE);
					pExplainCharacter_Window->m_SelectedZone = m_SelectedZone;
				}
#endif
			}
			break;	
#ifdef _XTS_NEWCREATECHARACTER	
		case _XDEF_EXPLAINCHARACTER_BTN_01 :
		case _XDEF_EXPLAINCHARACTER_BTN_02 :
		case _XDEF_EXPLAINCHARACTER_BTN_03 :
		case _XDEF_EXPLAINCHARACTER_BTN_04 :
		case _XDEF_EXPLAINCHARACTER_BTN_05 :
		case _XDEF_EXPLAINCHARACTER_BTN_06 :
		case _XDEF_EXPLAINCHARACTER_BTN_07 :
		case _XDEF_EXPLAINCHARACTER_BTN_08 :
		case _XDEF_EXPLAINCHARACTER_BTN_09 :
		case _XDEF_EXPLAINCHARACTER_BTN_10 :
		case _XDEF_EXPLAINCHARACTER_BTN_11 :
		case _XDEF_EXPLAINCHARACTER_BTN_12 :	
			{						
				_XWindow_ExplainCharacter* pExplainCharacter_Window = (_XWindow_ExplainCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_EXPLAINCHARACTER);
				if( pExplainCharacter_Window )
				{
					int buttonnumber = (wparam - _XDEF_EXPLAINCHARACTER_BTN_01) + 1;
					pExplainCharacter_Window->SimulationButtonClick(buttonnumber);
				}
			}
			break;
#endif
		default:
			break;
		}
	}
}
	
bool XProc_CreateCharacter::OnKeyboardPoll(_XInput *pInput)
{ 	
#ifdef _XDWDEBUG
	BOOL keypressed = FALSE;
	FLOAT direction = 0.0f;
	
	if( !_XIMEKERNEL.GetIMEContainer() ) 
	{
		
	}
#endif
	return true; 
}

bool XProc_CreateCharacter::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	return true;
}

bool XProc_CreateCharacter::OnKeyDown(WPARAM wparam, LPARAM lparam)
{ 
	if( (U32)wparam == VK_RETURN )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CC_CREATEBUTTON, 0), (LPARAM)gHWnd);
	}	
	else if( (U32)wparam == VK_ESCAPE )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CC_CANCELBUTTON, 0), (LPARAM)gHWnd);
	}
	return true; 
}

#ifdef _XDWDEBUG
int m_CameraOffsetX;
int m_CameraOffsetY;
POINT m_CameraDragPos;
#endif

bool XProc_CreateCharacter::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{ 		
	// Get the pick ray from the mouse position	
	POINT mousept = { pPos->x, pPos->z };
	
	//if( !::g_CurrentFocusedObject || ::g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND )
	{	
		float fmx = pState->AX*0.6;
		float fmy = pState->AY*0.6;
				
		if( pState->bButton[1] )
		{	
			m_PreviewCharacterRotateAngle -= fmx;
			D3DXMatrixRotationY( &m_PreviewCharacter.m_Position, _X_RAD(m_PreviewCharacterRotateAngle) );
		}		
#ifdef _XDWDEBUG
		if( pState->bButton[2] )
		{
			m_CameraOffsetX = pPos->x - m_CameraDragPos.x;
			m_CameraOffsetY = pPos->z - m_CameraDragPos.y;
			m_CameraDragPos.x = pPos->x;
			m_CameraDragPos.y = pPos->z;

			D3DXVECTOR3 targetpos;
			D3DXMATRIX  matOrientation;			
			targetpos.x = -(m_CameraOffsetX*0.025f);
			targetpos.y =  (m_CameraOffsetY*0.025f);
			targetpos.z = 0.0f;			
			D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
			D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );						
			g_LodTerrain.m_3PCamera.SetTargetPosition( g_LodTerrain.m_3PCamera.m_TargetPosition + targetpos );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix();
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		}
#endif
	}
	return true; 
}

void XProc_CreateCharacter::OnMouseWheel( short zDelta )
{
	if( !m_WindowManager.ProcessWheelMessage(zDelta) )
	{
#ifdef _XADMINISTRATORMODE
		FLOAT MoveDelta  = (FLOAT)zDelta*0.005F;

		if( gpInput->CheckKeyPress( DIK_LCONTROL ) ) MoveDelta *= _XDEF_RAPID_RATE;
		g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - MoveDelta );
		g_LodTerrain.m_3PCamera.UpdateViewMatrix();
#endif
	}
}

bool XProc_CreateCharacter::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
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
		
		break;
	}

	return true; 
}	

void XProc_CreateCharacter::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		int errorcode = WSAGetLastError();

		if( errorcode == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : CC : TCP I/O blocking");
			Sleep( 50 );
		}
		else
		{
			if( errorcode != ERROR_IO_PENDING )
			{
				g_NetworkKernel.ProcessNetworkError();
				g_NetworkKernel.DisconnectServer();
				g_NetworkKernel.ReleaseNetwork();
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_PrevProcess_SelectCharacter = TRUE;
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
				return;
			}
			else
			{
				_XLog( "WARNING : CC : TCP I/O pending" );
				Sleep( 50 );
			}
		}
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{	
	case FD_READ :
		g_NetworkKernel.ReadSocket();
		break;
	case FD_CLOSE :
		//_XDWINPRINT( "SERVER MESSAGE : Socket close message" );
		g_NetworkKernel.ProcessNetworkError();
		g_NetworkKernel.DisconnectServer();
		g_NetworkKernel.ReleaseNetwork();
		
		((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_PrevProcess_SelectCharacter = TRUE;
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( 
			&((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );		
		
		break;
	}
}

void XProc_CreateCharacter::OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( g_BgmPlayFlag )
	{		
		
	}
}

void XProc_CreateCharacter::OnWindowSizeChangeNotify( void )
{	
	g_MessageBox.MoveWindow((gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1), (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1) - 80 );
}

void XProc_CreateCharacter::SetCharacter( void )
{			
	int extravalue = 0;
	
	m_PreviewModelDescript.lhand		=	0;
	m_PreviewModelDescript.rhand		=	0;
		
	if( m_CharacterGender == _XGENDER_WOMAN )
	{	
		m_PreviewModelDescript.upperbody	=	g_Modeltype_UpperBody[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.lowerbody	=	g_Modeltype_LowerBody[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.adae			=   g_Modeltype_Hand[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.lfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.rfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.hair			=	m_HairTypeWomanIndex;
		m_PreviewModelDescript.face			=	m_FaceTypeWomanIndex;

		m_PreviewCharacter.LinkBipedObject( &g_FemaleBipedObject );		
	}
	else 
	{
		m_PreviewModelDescript.upperbody	=	g_Modeltype_UpperBody[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.lowerbody	=	g_Modeltype_LowerBody[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.adae			=   g_Modeltype_Hand[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.lfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.rfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.hair			=	m_HairTypeManIndex;				
		m_PreviewModelDescript.face			=	m_FaceTypeManIndex;

		m_PreviewCharacter.LinkBipedObject( &g_MaleBipedObject );
	}

	m_PreviewCharacter.SetMeshModel( m_PreviewModelDescript, m_CharacterGender, _XGROUP_NANGIN );
}

void XProc_CreateCharacter::DrawFadeInOut(void)
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

BOOL XProc_CreateCharacter::LoadMinimapTexture(  int index, LPTSTR zonefilenmae )
{
	if( index >= _XGROUP_COUNT ) return FALSE;

	SAFE_RELEASE( m_pMinimapTexture[index] );
	
	TCHAR minimapnamestr[_MAX_PATH];
	memset( minimapnamestr, 0, sizeof(minimapnamestr) );
	
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strncpy( minimapnamestr, "Mini", sizeof(minimapnamestr) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strncpy( minimapnamestr, "vn_Mini", sizeof(minimapnamestr) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strncpy( minimapnamestr, "us_Mini", sizeof(minimapnamestr) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strncpy( minimapnamestr, "tw_Mini", sizeof(minimapnamestr) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strncpy( minimapnamestr, "jp_Mini", sizeof(minimapnamestr) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strncpy( minimapnamestr, "rs_Mini", sizeof(minimapnamestr) );
	}
	else
	{
		strncpy( minimapnamestr, "Mini", sizeof(minimapnamestr) );
	}		

	strcat( minimapnamestr, zonefilenmae );
	
	LPTSTR cutpoint = strstr( minimapnamestr, "xtv" );
	if( cutpoint )
	{
		*cutpoint = NULL;
	}
	
	strcat( minimapnamestr, "tga");
	
	_XPackageArchive TextureArchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strncpy( packagefilename, gModulePath, sizeof(packagefilename) );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );
	
	if( !TextureArchive.OpenPackage( packagefilename ) ) return FALSE;	
	
	int resourceindex = TextureArchive.FindResource( minimapnamestr );
	if( resourceindex < 0 )
	{
		strncpy( minimapnamestr, "Mini", sizeof(minimapnamestr) );
		strcat( minimapnamestr, zonefilenmae );
		
		LPTSTR cutpoint = strstr( minimapnamestr, "xtv" );
		if( cutpoint )
		{
			*cutpoint = NULL;
		}
		
		resourceindex = TextureArchive.FindResource( minimapnamestr );		
		strcat( minimapnamestr, "tga" );
		
		if( resourceindex < 0 )
		{
			_XFatalError( "Resource not found in the texture archive [%s]", minimapnamestr);
			return FALSE;
		}
	}
	
	int   filesize = TextureArchive.GetPackedFileSize(resourceindex);
	FILE* pFP = TextureArchive.GetPackedFile(resourceindex);
	
	LPBYTE pImageVFMemory = NULL;
	pImageVFMemory = new BYTE[ filesize ];
	if( !pImageVFMemory )
	{
		_XFatalError( "Can't allocate memory for virtual file" );
		TextureArchive.FinalizePackage();
		return FALSE;
	}
	
	if( fread( pImageVFMemory, filesize, 1, pFP ) < 1 )
	{
		_XFatalError( "Can't read virtual file data" );
		delete[] pImageVFMemory;
		TextureArchive.FinalizePackage();
		return FALSE;
	}
	
	TextureArchive.FinalizePackage();
	
	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
		256, 256, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		0, NULL, NULL, &m_pMinimapTexture[index] ) ) )
	{
		_XFatalError( "Can't create texture from memory" );
		delete[] pImageVFMemory;		
		return FALSE;
	}
	
	delete[] pImageVFMemory;
	
	return TRUE;
}