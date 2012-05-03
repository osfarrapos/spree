// XPostProcessCore.cpp: implementation of the _XPostProcessCore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XPostProcessCore.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XScreenMotionBlur.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define TEX_SIZE		256
#define _XMAGICVALUE	-0.5f
_XPostProcessCore::_XPostProcessCore()
{
	/*
	m_ScreenVertex[0]		= _XPP_SCREENVERTEX( -0.5f,		gnHeight-0.5f, 0.0, 1, 0, 1 );
	m_ScreenVertex[1]		= _XPP_SCREENVERTEX( -0.5f,		 -0.5f,         0.0, 1, 0, 0 );
	m_ScreenVertex[2]		= _XPP_SCREENVERTEX( gnWidth-0.5f, gnHeight-0.5f, 0.0, 1, 1, 1 );
	m_ScreenVertex[3]		= _XPP_SCREENVERTEX( gnWidth-0.5f, -0.5f,         0.0, 1, 1, 0 );
	
	m_BlurScreenVertex[0]	= _XPP_SCREENVERTEX( -0.5f,		 TEX_SIZE-0.5f, 0.0, 1, 0, 1 );
	m_BlurScreenVertex[1]	= _XPP_SCREENVERTEX( -0.5f,		 -0.5,         0.0, 1, 0, 0 );
	m_BlurScreenVertex[2]	= _XPP_SCREENVERTEX( TEX_SIZE-0.5f, TEX_SIZE-0.5f, 0.0, 1, 1, 1 );
	m_BlurScreenVertex[3]	= _XPP_SCREENVERTEX( TEX_SIZE-0.5f, -0.5f,         0.0, 1, 1, 0 );
	*/
	
	m_pBackDepthSurface		= NULL;
	m_pBackTargetSurface	= NULL;
	
	m_pOrgSceneTexture		= NULL;
	m_pOrgSceneSurface		= NULL;
	m_pOrgSceneDepthSurface = NULL;

	m_UseBloomFilter		= FALSE;
	m_pTexTarget1			= NULL;
	m_pSurTarget1			= NULL;
	m_pTexTarget2			= NULL;
	m_pSurTarget2			= NULL;
	
	for( int i = 0; i < 4; i++ )
	{	
		m_pPixelShader_Bloomfilter[i] = NULL;		
	}

	m_FinalPixelShaderConst1[0] = 0.62f;
	m_FinalPixelShaderConst1[1] = 0.62f;
	m_FinalPixelShaderConst1[2] = 0.62f;
	m_FinalPixelShaderConst1[3] = 0.62f;
		
	m_GaussianFactor		= 1;
}

_XPostProcessCore::~_XPostProcessCore()
{
}


BOOL _XPostProcessCore::Init( void )
{
	return TRUE;
}

void _XPostProcessCore::InitDeviceObjects(void)
{
	m_ScreenVertex[0]		= _XPP_SCREENVERTEX( -0.5f,		gnHeight-0.5f, 0.0, 1, 0, 1 );
	m_ScreenVertex[1]		= _XPP_SCREENVERTEX( -0.5f,		 -0.5f,         0.0, 1, 0, 0 );
	m_ScreenVertex[2]		= _XPP_SCREENVERTEX( gnWidth-0.5f, gnHeight-0.5f, 0.0, 1, 1, 1 );
	m_ScreenVertex[3]		= _XPP_SCREENVERTEX( gnWidth-0.5f, -0.5f,         0.0, 1, 1, 0 );
	
	m_BlurScreenVertex[0]	= _XPP_SCREENVERTEX( -0.5f,		 TEX_SIZE-0.5f, 0.0, 1, 0, 1 );
	m_BlurScreenVertex[1]	= _XPP_SCREENVERTEX( -0.5f,		 -0.5,         0.0, 1, 0, 0 );
	m_BlurScreenVertex[2]	= _XPP_SCREENVERTEX( TEX_SIZE-0.5f, TEX_SIZE-0.5f, 0.0, 1, 1, 1 );
	m_BlurScreenVertex[3]	= _XPP_SCREENVERTEX( TEX_SIZE-0.5f, -0.5f,         0.0, 1, 1, 0 );
}

void _XPostProcessCore::InvalidateDeviceObjects(void)
{
	SAFE_RELEASE( m_pOrgSceneSurface	  );
	SAFE_RELEASE( m_pOrgSceneTexture	  );
	SAFE_RELEASE( m_pOrgSceneDepthSurface );

	SAFE_RELEASE( m_pBackTargetSurface );
	SAFE_RELEASE( m_pBackDepthSurface );
		
	ReleaseBloomFilter();
}

void _XPostProcessCore::DeleteDeviceObjects(void)
{
	SAFE_RELEASE( m_pOrgSceneSurface	  );
	SAFE_RELEASE( m_pOrgSceneTexture      );
	SAFE_RELEASE( m_pOrgSceneDepthSurface );	

	SAFE_RELEASE( m_pBackTargetSurface );
	SAFE_RELEASE( m_pBackDepthSurface );

	ReleaseBloomFilter();
}

void _XPostProcessCore::RestoreDeviceObjects(void)
{	
	InvalidateDeviceObjects();
	
	D3DDISPLAYMODE d3ddm;
	gpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	
	D3DXCreateTexture(gpDev, gnWidth, gnHeight, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &m_pOrgSceneTexture );	
	m_pOrgSceneTexture->GetSurfaceLevel( 0, &m_pOrgSceneSurface );

	if( FAILED( gpDev->CreateDepthStencilSurface(	gnWidth,gnHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE,	
		0, FALSE, &m_pOrgSceneDepthSurface, NULL) ) )
	{
		_XFatalError( "Can't create motion blur depth stencil surface" );
		return;
	}	

	if( m_UseBloomFilter ) 
		InitializeBloomFilter();
}

void _XPostProcessCore::InitializeBloomFilter( void )
{
	D3DDISPLAYMODE d3ddm;
	gpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	D3DXCreateTexture(gpDev, TEX_SIZE, TEX_SIZE, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &m_pTexTarget1 );
	m_pTexTarget1->GetSurfaceLevel( 0, &m_pSurTarget1 );
	D3DXCreateTexture(gpDev, TEX_SIZE, TEX_SIZE, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &m_pTexTarget2 );
	m_pTexTarget2->GetSurfaceLevel( 0, &m_pSurTarget2 );

	_XPackageArchive ShaderFileArchive;
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

#ifdef _XTESTSERVER			
	if( !ShaderFileArchive.OpenPackage( _T("tsShader.XP") ) ) return;
#else
	if( !ShaderFileArchive.OpenPackage( _T("Shader.XP") ) ) return;
#endif	
	
	TCHAR shaderfilename[4][32] = 
	{
		_T( "BloomFiler_GBlur_X.pso" ),_T( "BloomFiler_GBlur_Y.pso" ),_T( "BloomFiler_Power.pso" ),_T( "BloomFiler_Final.pso" ),
	};
	
	for( int i = 0; i < 4; i++ )
	{
		int archiveindex = ShaderFileArchive.FindResource( shaderfilename[i] );
		
		if( archiveindex < 0 )
		{
			_XFatalError("Could not find %s shader file", shaderfilename[i]);
			ShaderFileArchive.FinalizePackage();
			return;
		}
		
		LPDWORD pShader = (LPDWORD)ShaderFileArchive.LoadPackedFileToMemory( archiveindex );
		
		if( !pShader)
		{
			_XFatalError("Failed to allocate memory to load %s shader file [%d]", shaderfilename[i]);
			ShaderFileArchive.FinalizePackage();
			return;
		}
		
		if( FAILED( gpDev->CreatePixelShader( pShader, &m_pPixelShader_Bloomfilter[i] ) ) )
		{
			_XFatalError("Could not create animation vertex shader %s", shaderfilename[i]);			
			ShaderFileArchive.FinalizePackage();
			return;
		}
		
		delete[] pShader;
	}
	
	ShaderFileArchive.FinalizePackage();
}

void _XPostProcessCore::ReleaseBloomFilter( void )
{
	for( int i = 0; i < 4; i++ )
	{	
		SAFE_RELEASE( m_pPixelShader_Bloomfilter[i] );
	}

	SAFE_RELEASE( m_pSurTarget1 );	
	SAFE_RELEASE( m_pTexTarget1 );
	SAFE_RELEASE( m_pSurTarget2 );
	SAFE_RELEASE( m_pTexTarget2 );
}

void _XPostProcessCore::BeginPostProcess(void)
{
	if(!m_UseBloomFilter) return;

	gpDev->GetRenderTarget(0,&m_pBackTargetSurface);
	gpDev->GetDepthStencilSurface(&m_pBackDepthSurface);
	
	gpDev->EndScene();
	
	gpDev->SetRenderTarget( 0, m_pOrgSceneSurface );
	gpDev->SetDepthStencilSurface(m_pOrgSceneDepthSurface);
	gpDev->BeginScene();
	
    gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_EnvironmentManager.m_FogColor, 1.0f, 0);
}

void _XPostProcessCore::EndPostProcess(void)
{
	if(!m_UseBloomFilter) return;

	gpDev->EndScene();
	
	if( m_UseBloomFilter )
	{
		gpDev->SetVertexShader( NULL );
		gpDev->SetPixelShader( NULL );
		
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		gpDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		
		gpDev->SetRenderTarget( 0, m_pSurTarget1 );
		
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
		
		gpDev->BeginScene();
		gpDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
		gpDev->SetTexture( 0, m_pOrgSceneTexture );
		gpDev->SetTexture( 1, NULL );
		gpDev->SetTexture( 2, NULL );
		gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);
		gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		gpDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );	
		gpDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
		gpDev->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE );
		
		
		gpDev->SetFVF( _XPP_SCREENVERTEX::FVF );
		//gpDev->SetPixelShader( m_pPixelShader_Bloomfilter[ _XBLOOMFILTER_SHADER_GAUSSIANBLUR_POWER ] );
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_BlurScreenVertex, sizeof(_XPP_SCREENVERTEX) );
		gpDev->EndScene();
		
		gpDev->SetPixelShader( NULL );		
		
		GaussianBlur();
		
		//Restore render target...
		gpDev->SetRenderTarget(0,m_pBackTargetSurface);
		gpDev->SetDepthStencilSurface(m_pBackDepthSurface);
		
		SAFE_RELEASE( m_pBackTargetSurface );
		SAFE_RELEASE( m_pBackDepthSurface );
		
		gpDev->BeginScene();	
		gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_EnvironmentManager.m_FogColor, 1.0f, 0);
		
		gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());	
		gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
		
		gpDev->SetVertexShader( NULL );
		gpDev->SetPixelShader( NULL );
		
		
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 6 );
		
		gpDev->SetTexture(0,m_pOrgSceneTexture);	
		gpDev->SetTexture(1,m_pTexTarget1);
		
		gpDev->SetFVF( _XPP_SCREENVERTEX::FVF );

		gpDev->SetPixelShaderConstantF( 0, m_FinalPixelShaderConst1, 1 );
				
		gpDev->SetPixelShader( m_pPixelShader_Bloomfilter[_XBLOOMFILTER_SHADER_GAUSSIANBLUR_FINAL ] );
		
		//gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);
		//gpDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_ScreenVertex,sizeof(_XPP_SCREENVERTEX));
		
		gpDev->SetPixelShader( NULL );

	}
	else
	{
		//Restore render target...
		gpDev->SetRenderTarget(0,m_pBackTargetSurface);
		gpDev->SetDepthStencilSurface(m_pBackDepthSurface);
		
		SAFE_RELEASE( m_pBackTargetSurface );
		SAFE_RELEASE( m_pBackDepthSurface );
		
		gpDev->BeginScene();
		gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_EnvironmentManager.m_FogColor, 1.0f, 0);
		
		gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());	
		gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
		
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 7 );	
		gpDev->SetTexture(0,m_pOrgSceneTexture);
		
		gpDev->SetFVF( _XPP_SCREENVERTEX::FVF );
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_ScreenVertex,sizeof(_XPP_SCREENVERTEX));		
	}	
}



void _XPostProcessCore::EnableBloomFilter( BOOL enable )
{
	if( g_d3dCaps.PixelShaderVersion < D3DPS_VERSION(2,0) )
	{
		return;
	}

	m_UseBloomFilter = enable;
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEBLOOMFILTER, m_UseBloomFilter );

	if( !m_UseBloomFilter )
	{
		ReleaseBloomFilter();
		return;
	}
	
	TCHAR g_RegValue[_REGSTRING_MAXLENGTH];
	memset( g_RegValue, 0, sizeof(TCHAR) * _REGSTRING_MAXLENGTH );
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BLOOMFILTERPOWER, "0.65", g_RegValue, _REGSTRING_MAXLENGTH, TRUE );
	m_FinalPixelShaderConst1[0] = m_FinalPixelShaderConst1[1] = m_FinalPixelShaderConst1[2] = m_FinalPixelShaderConst1[3] = atof( g_RegValue );	

	ReleaseBloomFilter();
	InitializeBloomFilter();	
}

void _XPostProcessCore::GaussianBlur( void )
{
	LPDIRECT3DSURFACE9	p = NULL;
	gpDev->GetRenderTarget( 0, &p );
	
	for( int i = 0 ; i < m_GaussianFactor ; i++ )
	{
		// x방향 블러
		gpDev->SetRenderTarget( 0, m_pSurTarget2 );
		gpDev->BeginScene();
		gpDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
		gpDev->SetTexture( 0, m_pTexTarget1 );
		gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		gpDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		gpDev->SetFVF( _XPP_SCREENVERTEX::FVF );
		
		// TEX_SIZE은 텍스처의 가로,세로 크기
		D3DXVECTOR4	v = D3DXVECTOR4( TEX_SIZE, TEX_SIZE, TEX_SIZE, TEX_SIZE );
		gpDev->SetPixelShaderConstantF( 0, (float*)v, 1 );
		gpDev->SetPixelShader( m_pPixelShader_Bloomfilter[_XBLOOMFILTER_SHADER_GAUSSIANBLUR_X] );
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_BlurScreenVertex, sizeof(_XPP_SCREENVERTEX) );
		gpDev->EndScene();
		
		// y방향 블러
		gpDev->SetRenderTarget( 0, m_pSurTarget1 );
		gpDev->BeginScene();
		gpDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
		gpDev->SetTexture( 0, m_pTexTarget2 );
		gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		gpDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		gpDev->SetFVF( _XPP_SCREENVERTEX::FVF );
		gpDev->SetPixelShader( m_pPixelShader_Bloomfilter[_XBLOOMFILTER_SHADER_GAUSSIANBLUR_Y] );
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_BlurScreenVertex, sizeof(_XPP_SCREENVERTEX) );
		gpDev->EndScene();		
	}
	
	gpDev->SetRenderTarget( 0, p );
	SAFE_RELEASE( p );		
	
	gpDev->SetTexture( 0, NULL );
	gpDev->SetPixelShader( NULL );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Radial motion blur
int _XScreenBlurCore::m_iBlurTextureResolution = 256;
static const int g_MotionBlurAccBufferWidth = 256;
static const int g_MotionBlurAccBufferHeight = 256;

void __stdcall _XLocalUserRadialBlurRenderingCallback( void )
{
	g_pLocalUser->m_ModelDescriptor.RenderCharacterModel_RadialBlur( g_pLocalUser, FALSE );
}

_XScreenBlurCore::_XScreenBlurCore()
{
	Init();
}

_XScreenBlurCore::~_XScreenBlurCore()
{
	
}

void _XScreenBlurCore::RestoreBlurOption( void )
{
	m_iNumRadialBlurSteps		= 1;//30;
	m_fUVDelta					= 0.016f;
	m_fAlphaDelta				= 0.1f / m_iNumRadialBlurSteps;
	m_BlendColor				= D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

BOOL _XScreenBlurCore::Init( void )
{
	m_bEnableRadialBlur			= FALSE;
	m_iNumRadialBlurSteps		= 1;//30;
	m_fUVDelta					= 0.016f;
	m_fAlphaDelta				= 0.1f / m_iNumRadialBlurSteps;
	
	m_pOldRenderSurface			= NULL;
	m_pDepthStencilSurface		= NULL;
	
	m_pSceneTexture				= NULL;
	m_pSceneRenderSurface		= NULL;
	
	m_pRadialBlurTexture		= NULL;
	m_pBlurRenderSurface		= NULL;

	m_pAccumulateTexture		= NULL;
	m_pAccumulateSurface		= NULL;
	
	m_BlendColor				= 0xFFFFFFFF;//D3DCOLOR_ARGB( 255, 255, 255, 255 );

	m_pRadialBlurRenderingCallback	= _XLocalUserRadialBlurRenderingCallback;
	m_pMotionBlurRenderingCallback  = _XLocalUserRadialBlurRenderingCallback;

	m_bEnableMotionBlur			= FALSE;
	m_fIntensity				= 0.75f;
	
	return TRUE;
}

void _XScreenBlurCore::InitDeviceObjects( void )
{
	InvalidateDeviceObjects();
	
	gpDev->GetRenderTarget( 0, &m_pOldRenderSurface );	
	//gpDev->GetDepthStencilSurface( &m_pDepthStencilSurface );
	
	D3DSURFACE_DESC SurfaceDescription;
	m_pOldRenderSurface->GetDesc( &SurfaceDescription );
	
	int	RenderBufferWidth = m_iBlurTextureResolution;
	int	RenderBufferHeight = m_iBlurTextureResolution;

	if( g_MaxTextureWidth < RenderBufferWidth )
	{
		RenderBufferWidth = g_MaxTextureWidth;
	}
	if( g_MaxTextureHeight < RenderBufferHeight )
	{
		RenderBufferHeight = g_MaxTextureHeight;
	}
	
	gpDev->CreateTexture( //SurfaceDescription.Width, SurfaceDescription.Height, 1, SurfaceDescription.Usage,
						  RenderBufferWidth, RenderBufferHeight, 1, SurfaceDescription.Usage,
						  SurfaceDescription.Format, SurfaceDescription.Pool, &m_pSceneTexture, NULL );
		
	m_pSceneTexture->GetSurfaceLevel( 0, &m_pSceneRenderSurface );
	
	gpDev->CreateTexture( RenderBufferWidth, RenderBufferHeight, 1, SurfaceDescription.Usage,
						  SurfaceDescription.Format, SurfaceDescription.Pool, &m_pRadialBlurTexture, NULL );				
		
	m_pRadialBlurTexture->GetSurfaceLevel( 0, &m_pBlurRenderSurface );


	int AccBufferWidth = g_MotionBlurAccBufferWidth;
	int AccBufferHeight = g_MotionBlurAccBufferHeight;

	if( g_MaxTextureWidth < AccBufferWidth )
	{
		RenderBufferWidth = g_MaxTextureWidth;
	}
	if( g_MaxTextureHeight < AccBufferHeight )
	{
		RenderBufferHeight = g_MaxTextureHeight;
	}

	gpDev->CreateTexture( RenderBufferWidth, RenderBufferHeight, 1, SurfaceDescription.Usage,
						  SurfaceDescription.Format, SurfaceDescription.Pool, &m_pAccumulateTexture, NULL );
	
	m_pAccumulateTexture->GetSurfaceLevel( 0, &m_pAccumulateSurface );

	SAFE_RELEASE( m_pOldRenderSurface	);
	//SAFE_RELEASE( m_pDepthStencilSurface);

	m_BlurScreenVertex[0] = _XScreenBlurOverayVertex( (float)RenderBufferWidth+_XMAGICVALUE, _XMAGICVALUE, 1.0f,  1.0f, 0xFFFFFFFF, 1.0f, 0.0f );		// right top
	m_BlurScreenVertex[1] = _XScreenBlurOverayVertex( (float)RenderBufferWidth+_XMAGICVALUE, (float)RenderBufferHeight+_XMAGICVALUE, 1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f ); // right bottom
	m_BlurScreenVertex[2] = _XScreenBlurOverayVertex( _XMAGICVALUE, _XMAGICVALUE, 1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f ); // left top
	m_BlurScreenVertex[3] = _XScreenBlurOverayVertex( _XMAGICVALUE, (float)RenderBufferHeight+_XMAGICVALUE, 1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f ); // left bottom

	m_OverayScreenVertex[0] = _XScreenBlurOverayVertex( (float)gnWidth+_XMAGICVALUE, _XMAGICVALUE,					1.0f, 1.0f, m_BlendColor, 1.0f, 0.0f ); // right top
	m_OverayScreenVertex[1] = _XScreenBlurOverayVertex( (float)gnWidth+_XMAGICVALUE, (float)gnHeight+_XMAGICVALUE,	1.0f, 1.0f, m_BlendColor, 1.0f, 1.0f ); // right bottom
	m_OverayScreenVertex[2] = _XScreenBlurOverayVertex( _XMAGICVALUE,			     _XMAGICVALUE,					1.0f, 1.0f, m_BlendColor, 0.0f, 0.0f ); // left top
	m_OverayScreenVertex[3] = _XScreenBlurOverayVertex( _XMAGICVALUE, (float)gnHeight+_XMAGICVALUE,					1.0f, 1.0f, m_BlendColor, 0.0f, 1.0f ); // left bottom

	m_MotionBlurScreenVertex[0] = _XScreenBlurOverayVertex( (float)RenderBufferWidth+_XMAGICVALUE, _XMAGICVALUE,			1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f ); // right top
	m_MotionBlurScreenVertex[1] = _XScreenBlurOverayVertex( (float)RenderBufferWidth+_XMAGICVALUE, (float)RenderBufferHeight+_XMAGICVALUE,	1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f ); // right bottom
	m_MotionBlurScreenVertex[2] = _XScreenBlurOverayVertex( _XMAGICVALUE,								  _XMAGICVALUE,			1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f ); // left top
	m_MotionBlurScreenVertex[3] = _XScreenBlurOverayVertex( _XMAGICVALUE, (float)g_MaxTextureHeight+_XMAGICVALUE,			1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f ); // left bottom
}

void _XScreenBlurCore::InvalidateDeviceObjects( void )
{
	SAFE_RELEASE( m_pOldRenderSurface	);
	SAFE_RELEASE( m_pDepthStencilSurface);

	SAFE_RELEASE( m_pSceneRenderSurface );
	SAFE_RELEASE( m_pSceneTexture );
	
	SAFE_RELEASE( m_pBlurRenderSurface	);
	SAFE_RELEASE( m_pRadialBlurTexture	);	

	SAFE_RELEASE( m_pAccumulateSurface  );
	SAFE_RELEASE( m_pAccumulateTexture  );
}

void _XScreenBlurCore::DeleteDeviceObjects( void )
{
	SAFE_RELEASE( m_pOldRenderSurface	);
	SAFE_RELEASE( m_pDepthStencilSurface);
	
	SAFE_RELEASE( m_pSceneRenderSurface );
	SAFE_RELEASE( m_pSceneTexture );
	
	SAFE_RELEASE( m_pBlurRenderSurface	);
	SAFE_RELEASE( m_pRadialBlurTexture	);	

	SAFE_RELEASE( m_pAccumulateSurface  );
	SAFE_RELEASE( m_pAccumulateTexture  );
}

void _XScreenBlurCore::RestoreDeviceObjects( void )
{
	InitDeviceObjects();
}

void _XScreenBlurCore::ProcessRadialBlur( void )
{
	//if( !m_bEnableRadialBlur ) return;
	gpDev->EndScene();
	gpDev->GetRenderTarget( 0, &m_pOldRenderSurface );	
	//gpDev->GetDepthStencilSurface( &m_pDepthStencilSurface );

	gpDev->BeginScene();
	gpDev->SetRenderTarget( 0, m_pSceneRenderSurface );
	//gpDev->SetDepthStencilSurface( m_pDepthStencilSurface );

	gpDev->Clear( 0, NULL, D3DCLEAR_TARGET/* | D3DCLEAR_ZBUFFER*/,
						   D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0x00000000 );

	if( m_pRadialBlurRenderingCallback )
	{
		m_pRadialBlurRenderingCallback();
	}
	
	gpDev->EndScene();

	gpDev->SetRenderTarget( 0, m_pBlurRenderSurface );
	//gpDev->SetDepthStencilSurface( m_pDepthStencilSurface );
	gpDev->BeginScene();
	
	gpDev->Clear( 0, NULL, D3DCLEAR_TARGET/* | D3DCLEAR_ZBUFFER*/,
				  D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0x00000000 );	

	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 10 );
	//gpDev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );	
	//gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//gpDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//gpDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	float fUVOffset = 0.0f;
	float fAlpha = 0.1f;
	float fTVAdjustDelta = 0.0f;
		
	gpDev->SetTexture( 0, m_pSceneTexture );
	gpDev->SetFVF( _XFVF_OVERLAYVERTEX );

	for ( int i = 0; i < m_iNumRadialBlurSteps; i++ )
	{
		fUVOffset += m_fUVDelta;		
		fTVAdjustDelta += 0.002f;
		
		/*
		m_BlurScreenVertex[0].tu = 1.0f - fUVOffset;	m_BlurScreenVertex[0].tv = fUVOffset;							// right top
		m_BlurScreenVertex[1].tu = 1.0f - (fUVOffset*2.0f);	m_BlurScreenVertex[1].tv = 1.0f - (fUVOffset*2.0f);			// right bottom
		m_BlurScreenVertex[2].tu = fUVOffset;		m_BlurScreenVertex[2].tv = fUVOffset;								// left top
		m_BlurScreenVertex[3].tu = (fUVOffset*2.0f);		m_BlurScreenVertex[3].tv = 1.0f - (fUVOffset*2.0f);			// left bottom
		*/
		m_BlurScreenVertex[0].tu = 1.0f - fUVOffset;	m_BlurScreenVertex[0].tv = fUVOffset + fTVAdjustDelta;			// right top
		m_BlurScreenVertex[1].tu = 1.0f - fUVOffset;	m_BlurScreenVertex[1].tv = 1.0f - fUVOffset + fTVAdjustDelta;	// right bottom
		m_BlurScreenVertex[2].tu = fUVOffset;			m_BlurScreenVertex[2].tv = fUVOffset + fTVAdjustDelta;			// left top
		m_BlurScreenVertex[3].tu = fUVOffset;			m_BlurScreenVertex[3].tv = 1.0f - fUVOffset + fTVAdjustDelta;	// left bottom
		
		fAlpha -= m_fAlphaDelta;
		
		m_BlurScreenVertex[0].color = m_BlurScreenVertex[1].color = m_BlurScreenVertex[2].color = m_BlurScreenVertex[3].color = 
								 D3DCOLOR_ARGB( int(fAlpha * 255), 255, 255, 255 );
		
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (PVOID)m_BlurScreenVertex, sizeof( _XScreenBlurOverayVertex ) );
	}

	gpDev->EndScene();

	// Restore org render target
	gpDev->SetRenderTarget( 0, m_pOldRenderSurface );

	SAFE_RELEASE( m_pOldRenderSurface	);
	//SAFE_RELEASE( m_pDepthStencilSurface);

	gpDev->BeginScene();
	
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 11 );

	//gpDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	//gpDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA ); //D3DBLEND_ONE );
	
	// Draw the Scene Texture.
	//gpDev->SetTexture( 0, m_pSceneTexture );
	//gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (PVOID)screenoverlay, sizeof( _XScreenBlurOverayVertex ) );
	
	// Draw the Radial Blur Texture.
	gpDev->SetTexture( 0, m_pRadialBlurTexture );
	gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (PVOID)m_OverayScreenVertex, sizeof( _XScreenBlurOverayVertex ) );
	
	// Restore Render States.
	//gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	
	// Make sure to turn Depth-Testing back on.
	//gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motion blur

void _XScreenBlurCore::ProcessMotionBlur( void )
{
	//if( !m_bEnableMotionBlur ) return;

	gpDev->EndScene();
	gpDev->GetRenderTarget( 0, &m_pOldRenderSurface );	
	//gpDev->GetDepthStencilSurface( &m_pDepthStencilSurface );
	
	gpDev->BeginScene();
	gpDev->SetRenderTarget( 0, m_pSceneRenderSurface );
	//gpDev->SetDepthStencilSurface( m_pDepthStencilSurface );
	
	gpDev->Clear( 0, NULL, D3DCLEAR_TARGET/* | D3DCLEAR_ZBUFFER*/,
					 D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0x00000000 );
	
	if( m_pMotionBlurRenderingCallback )
	{
		m_pMotionBlurRenderingCallback();
	}
	
	gpDev->EndScene();	

	gpDev->BeginScene();
	gpDev->SetRenderTarget( 0, m_pAccumulateSurface );

	static BOOL _clearedblurscreen = FALSE;
	if( !_clearedblurscreen )
	{
		_clearedblurscreen = TRUE;
		gpDev->Clear( 0, NULL, D3DCLEAR_TARGET/* | D3DCLEAR_ZBUFFER*/,
					  D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0x00000000 );
	}

	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 12 );
	gpDev->SetFVF( _XFVF_OVERLAYVERTEX );

	m_MotionBlurScreenVertex[0].color = m_MotionBlurScreenVertex[1].color = m_MotionBlurScreenVertex[2].color = m_MotionBlurScreenVertex[3].color = 
									  D3DCOLOR_ARGB( int(255 * (1.0f - m_fIntensity)), 255, 255, 255 );

	gpDev->SetTexture( 0, m_pSceneTexture );
	gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (PVOID)m_MotionBlurScreenVertex, sizeof( _XScreenBlurOverayVertex ) );

	gpDev->EndScene();	

	gpDev->SetRenderTarget( 0, m_pOldRenderSurface );
	SAFE_RELEASE( m_pOldRenderSurface	);
	//SAFE_RELEASE( m_pDepthStencilSurface);
	
	gpDev->BeginScene();

	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 13 );
	//gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//gpDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//gpDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );		
	//gpDev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );		
		
	gpDev->SetTexture( 0, m_pAccumulateTexture );
	gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (PVOID)m_OverayScreenVertex, sizeof( _XScreenBlurOverayVertex ) );
}