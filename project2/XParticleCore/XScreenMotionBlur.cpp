// XScreenMotionBlur.cpp: implementation of the _XScreenMotionBlur class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XScreenMotionBlur.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const DWORD _XScreenBlurVertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1;

//블러 텍스쳐 크기
#define		_XDEF_SCREENBLURSIZEX		512
#define		_XDEF_SCREENBLURSIZEY       512

_XScreenMotionBlur::_XScreenMotionBlur()
{
	m_Initialized = FALSE;
	m_RenderBufferWidth  = _XDEF_SCREENBLURSIZEX;
	m_RenderBufferHeight = _XDEF_SCREENBLURSIZEY;
}

_XScreenMotionBlur::~_XScreenMotionBlur()
{

}


void _XScreenMotionBlur::InitDeviceObjects(void)
{
	m_enable = false;
	m_firstEnable = true;
	m_blurAlpha = 0.0f;
	m_maxAlpha = 0.99f;
	m_minAlpha = 0.1f;
	m_Initialized = TRUE;
}

void _XScreenMotionBlur::RestoreDeviceObjects(void)
{
	m_Initialized = TRUE;
	InitBlur();
}

void _XScreenMotionBlur::InvalidateDeviceObjects(void)
{
	SAFE_RELEASE( m_pBlurTargetSurface);
	SAFE_RELEASE( m_pBlurTexture );

	SAFE_RELEASE( m_pBlurAccumSurface);
	SAFE_RELEASE( m_pBlurAccumTexture );
	
	SAFE_RELEASE( m_pBlurDepthSurface );
}

void _XScreenMotionBlur::DeleteDeviceObjects(void)
{
	InvalidateDeviceObjects();
	m_Initialized = FALSE;
}

bool _XScreenMotionBlur::InitBlur(void)
{
	InvalidateDeviceObjects();
	
	m_RenderBufferWidth  = _XDEF_SCREENBLURSIZEX;
	m_RenderBufferHeight = _XDEF_SCREENBLURSIZEY;

	if( g_MaxTextureWidth < _XDEF_SCREENBLURSIZEX )
	{
		m_RenderBufferWidth = g_MaxTextureWidth;
	}
	if( g_MaxTextureHeight < _XDEF_SCREENBLURSIZEY )
	{
		m_RenderBufferHeight = g_MaxTextureHeight;
	}

	if( FAILED( gpDev->CreateTexture(	m_RenderBufferWidth ,m_RenderBufferHeight, 1, D3DUSAGE_RENDERTARGET, 
		                    D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_pBlurTexture, NULL) ) )
	{
		_XFatalError( "Can't create motion blur buffer" );
		return false;
	}

	if( FAILED( m_pBlurTexture->GetSurfaceLevel(0,&m_pBlurTargetSurface) ) )
	{
		_XFatalError( "Can't access motion blur target surface" );
		return false;
	}


	if( FAILED( gpDev->CreateTexture(	m_RenderBufferWidth,m_RenderBufferHeight, 1, D3DUSAGE_RENDERTARGET,
							D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_pBlurAccumTexture, NULL) ) )
	{
		_XFatalError( "Can't create motion blur accumulate buffer" );
		return false;
	}


	m_pBlurAccumTexture->GetSurfaceLevel(0,&m_pBlurAccumSurface);

	if( FAILED( gpDev->CreateDepthStencilSurface(	m_RenderBufferWidth,m_RenderBufferHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE,	
										 0, FALSE, &m_pBlurDepthSurface, NULL) ) )
	{
		_XFatalError( "Can't create motion blur depth stencil surface" );
		return false;
	}

	return true;
}


void _XScreenMotionBlur::BeginBlur(void)
{
	if( IsInit() == false) return;
	if( IsEnable() == false) return;

	gpDev->GetRenderTarget(0,&m_pBackTargetSurface);
	gpDev->GetDepthStencilSurface(&m_pBackDepthSurface);

	gpDev->SetRenderTarget(0,m_pBlurTargetSurface);
	gpDev->SetDepthStencilSurface(m_pBlurDepthSurface);

	gpDev->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
}


void _XScreenMotionBlur::ApplyAccumBlur(void)
{
	if(m_firstEnable == true )
	{
		m_firstEnable = false;
		return;
	}

	// 여러가지 알파 혼합을 시도 해본다. 
	gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );

	gpDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	gpDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

//	gpDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR );
//	gpDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR );

	gpDev->SetRenderState( D3DRS_COLORVERTEX ,TRUE);
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE , D3DMCS_COLOR1 );

	gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	gpDev->SetTextureStageState(0,  D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		
	DWORD	alphaColor = 0xFFFFFF | ((DWORD)(GetBlurAlpha()*255.0f) << 24);

	_XScreenBlurVertex V[4] = { _XScreenBlurVertex(D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,		alphaColor, 0,0) ,
								_XScreenBlurVertex(D3DXVECTOR3(m_RenderBufferWidth,0.0f,0.0f),1.0f,		alphaColor, 1,0) , 
								_XScreenBlurVertex(D3DXVECTOR3(0.0f,m_RenderBufferHeight,0.0f),1.0f,		alphaColor, 0,1) , 
								_XScreenBlurVertex(D3DXVECTOR3(m_RenderBufferWidth,m_RenderBufferHeight,0.0f),1.0f,	alphaColor, 1,1) };


	//누적 블러 텍스쳐를 찍는다.
	gpDev->SetTexture(0,m_pBlurAccumTexture);
	gpDev->SetFVF( _XScreenBlurVertex::FVF );
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,V,sizeof(_XScreenBlurVertex));
}


void _XScreenMotionBlur::UpdateAccumBlur(void)
{
	//누적 블러로 갱신.
	gpDev->SetRenderTarget(0,m_pBlurAccumSurface);


	//현재의 블러 렌더 타겟 텍스쳐를 누적 블러 텍스쳐로 찍는다.
	gpDev->SetTexture(0,m_pBlurTexture);
	gpDev->SetFVF( _XScreenBlurVertex::FVF );

	gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);

	_XScreenBlurVertex V[4] = { _XScreenBlurVertex(D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,		0xFFFFFF, 0,0) ,
								_XScreenBlurVertex(D3DXVECTOR3(m_RenderBufferWidth,0.0f,0.0f),1.0f,		0xFFFFFF, 1,0) , 
								_XScreenBlurVertex(D3DXVECTOR3(0.0f,m_RenderBufferHeight,0.0f),1.0f,		0xFFFFFF, 0,1) , 
								_XScreenBlurVertex(D3DXVECTOR3(m_RenderBufferWidth,m_RenderBufferHeight,0.0f),1.0f,	0xFFFFFF, 1,1) };

	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,V,sizeof(_XScreenBlurVertex));
}

void _XScreenMotionBlur::ApplyBlur(void)
{
	//렌더 타겟을 원래 데로.
	gpDev->SetRenderTarget(0,m_pBackTargetSurface);
	gpDev->SetDepthStencilSurface(m_pBackDepthSurface);

	//화면 크기 얻기
	D3DSURFACE_DESC	desc;
	m_pBackTargetSurface->GetDesc(&desc);

	//릴리즈 해줘야 한다.
	SAFE_RELEASE( m_pBackTargetSurface );
	SAFE_RELEASE( m_pBackDepthSurface );

	//최종 블러 텍스쳐를 화면에다가 찍는다.
	gpDev->SetTexture(0,m_pBlurTexture);
	gpDev->SetFVF( _XScreenBlurVertex::FVF );

	gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);

	float sx = (float)desc.Width ;
	float sy = (float)desc.Height;

	_XScreenBlurVertex V[4] = {	_XScreenBlurVertex(D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f	,0xFFFFFF, 0,0) ,
								_XScreenBlurVertex(D3DXVECTOR3(sx,0.0f,0.0f),1.0f	,0xFFFFFF, 1,0) , 
								_XScreenBlurVertex(D3DXVECTOR3(0.0f,sy,0.0f),1.0f	,0xFFFFFF, 0,1) , 
								_XScreenBlurVertex(D3DXVECTOR3(sx,sy,0.0f),1.0f		,0xFFFFFF, 1,1) };

	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,V,sizeof(_XScreenBlurVertex));


	//테스트... 느리다. 
	//gpDev->UpdateSurface(m_pBlurAccumSurface,NULL,m_pBlurTargetSurface,NULL);
	//gpDev->GetRenderTargetData(m_pBlurTargetSurface,m_pBlurAccumSurface);
}


void _XScreenMotionBlur::EndBlur(void)
{
	if( IsInit() == false) return;
	if( IsEnable() == false) return;
		
	//렌더링된 화면에다가 
	//전 프레임까지 누적된 블러 텍스쳐를 찍어 버린다. 
	ApplyAccumBlur();

	//결과(이번프레임+누적된 블러 텍스쳐)를 누적 블러 텍스쳐에다가 보관한다.
	UpdateAccumBlur();

	//화면에 복사한다.
	ApplyBlur();
}

void _XScreenMotionBlur::Enable(bool flg)
{
	if(IsEnable()==false) 
	{
		m_firstEnable= true; 
		m_blurAlpha = m_minAlpha;
	}
	
	m_enable = flg; 
}

void _XScreenMotionBlur::SetBlurAlpha(float alpha)
{
	if(alpha < GetMinBlurAlpha()) alpha = GetMinBlurAlpha();
	if(alpha > GetMaxBlurAlpha() ) alpha = GetMaxBlurAlpha();

	m_blurAlpha = alpha;
}

float _XScreenMotionBlur::GetBlurAlpha(void)
{
	return m_blurAlpha;
}

void _XScreenMotionBlur::SetMaxBlurAlpha(float alpha)
{
	if(alpha < 0.0f) alpha = 0.0f;
	if(alpha > 0.99f) alpha = 0.99f;

	m_maxAlpha = alpha;
}

void _XScreenMotionBlur::SetMinBlurAlpha(float alpha)
{
	if(alpha < 0.0f) alpha = 0.0f;
	if(alpha > 0.99f) alpha = 0.99f;

	m_minAlpha = alpha;
}

void _XScreenMotionBlur::AddBlurAlpha(float a)
{
	SetBlurAlpha( GetBlurAlpha() + a );

	if( GetBlurAlpha() > GetMinBlurAlpha() )
	{
		Enable(true);
	}
	else
	{
		Enable(false);
	}
}



#define TEX_SIZE		256


_XBroomFilterEffect::_XBroomFilterEffect()
{
	m_pTexTarget1			= NULL;
	m_pSurTarget1			= NULL;
	m_pTexTarget2			= NULL;
	m_pSurTarget2			= NULL;
	
	m_pPSGBlurX				= NULL;	
	m_pPSGBlurY				= NULL;	
	m_pPSPower				= NULL;	
	m_pPSFinal				= NULL;	
	
	m_GaussianFactor		= 1;

	m_ScreenVertex[0]		= _XBF_SCREENVERTEX( -0.5,		gnHeight-0.5, 0.0, 1, 0, 1 );
	m_ScreenVertex[1]		= _XBF_SCREENVERTEX( -0.5,		 -0.5,         0.0, 1, 0, 0 );
	m_ScreenVertex[2]		= _XBF_SCREENVERTEX( gnWidth-0.5, gnHeight-0.5, 0.0, 1, 1, 1 );
	m_ScreenVertex[3]		= _XBF_SCREENVERTEX( gnWidth-0.5, -0.5,         0.0, 1, 1, 0 );

	m_BlurScreenVertex[0]	= _XBF_SCREENVERTEX( -0.5,		 TEX_SIZE-0.5, 0.0, 1, 0, 1 );
	m_BlurScreenVertex[1]	= _XBF_SCREENVERTEX( -0.5,		 -0.5,         0.0, 1, 0, 0 );
	m_BlurScreenVertex[2]	= _XBF_SCREENVERTEX( TEX_SIZE-0.5, TEX_SIZE-0.5, 0.0, 1, 1, 1 );
	m_BlurScreenVertex[3]	= _XBF_SCREENVERTEX( TEX_SIZE-0.5, -0.5,         0.0, 1, 1, 0 );
	
	m_pBackDepthSurface		= NULL;
	m_pBackTargetSurface	= NULL;

	m_pOrgSceneTexture		= NULL;
	m_pOrgSceneSurface		= NULL;
	m_pOrgSceneDepthSurface = NULL;
}

_XBroomFilterEffect::~_XBroomFilterEffect()
{

}

BOOL _XBroomFilterEffect::Init( void )
{


	return TRUE;
}

void _XBroomFilterEffect::InitDeviceObjects(void)
{

}

void _XBroomFilterEffect::InvalidateDeviceObjects(void)
{
	SAFE_RELEASE( m_pPSGBlurX	);
	SAFE_RELEASE( m_pPSGBlurY	);
	SAFE_RELEASE( m_pPSPower	);
	SAFE_RELEASE( m_pPSFinal	);
	
	SAFE_RELEASE( m_pSurTarget1 );
	SAFE_RELEASE( m_pTexTarget1 );
	SAFE_RELEASE( m_pSurTarget2 );
	SAFE_RELEASE( m_pTexTarget2 );
	
	SAFE_RELEASE( m_pOrgSceneTexture );
	SAFE_RELEASE( m_pOrgSceneSurface	  );
	SAFE_RELEASE( m_pOrgSceneDepthSurface );
}

void _XBroomFilterEffect::DeleteDeviceObjects(void)
{
	SAFE_RELEASE( m_pPSGBlurX	);
	SAFE_RELEASE( m_pPSGBlurY	);
	SAFE_RELEASE( m_pPSPower	);
	SAFE_RELEASE( m_pPSFinal	);
	
	SAFE_RELEASE( m_pSurTarget1 );
	SAFE_RELEASE( m_pTexTarget1 );
	SAFE_RELEASE( m_pSurTarget2 );
	SAFE_RELEASE( m_pTexTarget2 );
	
	SAFE_RELEASE( m_pOrgSceneTexture      );
	SAFE_RELEASE( m_pOrgSceneSurface	  );
	SAFE_RELEASE( m_pOrgSceneDepthSurface );
}

void _XBroomFilterEffect::RestoreDeviceObjects(void)
{
	InvalidateDeviceObjects();
	
	D3DDISPLAYMODE d3ddm;
	gpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	
	D3DXCreateTexture(gpDev, TEX_SIZE, TEX_SIZE, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &m_pTexTarget1 );
	m_pTexTarget1->GetSurfaceLevel( 0, &m_pSurTarget1 );
	D3DXCreateTexture(gpDev, TEX_SIZE, TEX_SIZE, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &m_pTexTarget2 );
	m_pTexTarget2->GetSurfaceLevel( 0, &m_pSurTarget2 );
	
	D3DXCreateTexture(gpDev, gnWidth, gnHeight, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &m_pOrgSceneTexture );	
	m_pOrgSceneTexture->GetSurfaceLevel( 0, &m_pOrgSceneSurface );

	if( FAILED( gpDev->CreateDepthStencilSurface(	gnWidth,gnHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE,	
		0, FALSE, &m_pOrgSceneDepthSurface, NULL) ) )
	{
		_XFatalError( "Can't create motion blur depth stencil surface" );
		return;
	}
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
	
	LPD3DXBUFFER pCode;
	
	if( FAILED( D3DXAssembleShaderFromFile( "PS_GBlur_X.ps", NULL, NULL, 0, &pCode, NULL ) ) )
	{
		return;
	}
	gpDev->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &m_pPSGBlurX );
	SAFE_RELEASE( pCode );
	
	if( FAILED( D3DXAssembleShaderFromFile( "PS_GBlur_Y.ps", NULL, NULL, 0, &pCode, NULL ) ) )
	{
		return;
	}
	gpDev->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &m_pPSGBlurY );
	SAFE_RELEASE( pCode );
	
	if( FAILED( D3DXAssembleShaderFromFile( "PS_Power.ps", NULL, NULL, 0, &pCode, NULL ) ) )
	{
		return;
	}
	gpDev->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &m_pPSPower );
	SAFE_RELEASE( pCode );
	
	if( FAILED( D3DXAssembleShaderFromFile( "PS_Final.ps", NULL, NULL, 0, &pCode, NULL ) ) )
	{
		return;
	}
	gpDev->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &m_pPSFinal );
	
	SAFE_RELEASE( pCode );
}


void _XBroomFilterEffect::GaussianBlur( void )
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
		gpDev->SetFVF( _XBF_SCREENVERTEX::FVF );
		
		// TEX_SIZE은 텍스처의 가로,세로 크기
		D3DXVECTOR4	v = D3DXVECTOR4( TEX_SIZE, TEX_SIZE, TEX_SIZE, TEX_SIZE );
		gpDev->SetPixelShaderConstantF( 0, (float*)v, 1 );
		gpDev->SetPixelShader( m_pPSGBlurX );
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_BlurScreenVertex, sizeof(_XBF_SCREENVERTEX) );
		gpDev->EndScene();
		
		// y방향 블러
		gpDev->SetRenderTarget( 0, m_pSurTarget1 );
		gpDev->BeginScene();
		gpDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
		gpDev->SetTexture( 0, m_pTexTarget2 );
		gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		gpDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		gpDev->SetFVF( _XBF_SCREENVERTEX::FVF );
		gpDev->SetPixelShader( m_pPSGBlurY );
		gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_BlurScreenVertex, sizeof(_XBF_SCREENVERTEX) );
		gpDev->EndScene();		
	}

	gpDev->SetRenderTarget( 0, p );
	SAFE_RELEASE( p );		

	gpDev->SetTexture( 0, NULL );
	gpDev->SetPixelShader( NULL );
}

void _XBroomFilterEffect::BeginBloomFilter(void)
{
	gpDev->GetRenderTarget(0,&m_pBackTargetSurface);
	gpDev->GetDepthStencilSurface(&m_pBackDepthSurface);

	gpDev->EndScene();
	
	gpDev->SetRenderTarget( 0, m_pOrgSceneSurface );
	gpDev->SetDepthStencilSurface(m_pOrgSceneDepthSurface);
	gpDev->BeginScene();

    gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_EnvironmentManager.m_FogColor, 1.0f, 0);
}

BOOL g_DrawBloomFilteredMap = TRUE;

void _XBroomFilterEffect::EndBloomFilter(void)
{
	gpDev->EndScene();

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

	
	gpDev->SetFVF( _XBF_SCREENVERTEX::FVF );
	//gpDev->SetPixelShader( m_pPSPower );
	gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_BlurScreenVertex, sizeof(_XBF_SCREENVERTEX) );
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

	gpDev->SetFVF( _XBF_SCREENVERTEX::FVF );
	gpDev->SetPixelShader( m_pPSFinal );

	//gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);
	//gpDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_ScreenVertex,sizeof(_XBF_SCREENVERTEX));

	gpDev->SetPixelShader( NULL );
}
