// XLensFlare.cpp: implementation of the _XLensFlare class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XLensFlare.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int g_SunTwinkleAlphaLevel[10] = 
{
	255, 252, 233, 201, 150, 180, 241, 246, 250, 253
};

_XLensFlare::_XLensFlare()
{	
	for (int i=0; i < _XDEF_FLARECOUNT; i++)
		m_flares[i].pLensflare = NULL;

	m_SunZAngle					=	0.0f;
	m_SunTwinkleRateIndex		=	0.0f;
}

_XLensFlare::~_XLensFlare()
{
	DestroyFlare();
}

void _XLensFlare::DestroyFlare( void )
{
	for( int i = 0; i < _XDEF_FLARECOUNT; i++ )
		SAFE_DELETE( m_flares[i].pLensflare );
}

void _XLensFlare::Init( void )
{	
	DestroyFlare();

	m_SunPosition = D3DXVECTOR3(100.0f,0.0f,-40.0f);
	m_SunTextureIndex = g_TerrainTextureArchive.GetResourceIndex( "flare0.tga" );
	m_SunBillboard.SetBillBoard( 32, 32, m_SunPosition );

	m_MoonPosition = D3DXVECTOR3(100.0f,0.0f,-40.0f);
	m_MoonTextureIndex = g_TerrainTextureArchive.GetResourceIndex( "MOON.TGA" );
	m_MoonBillboard.SetBillBoard( 8,8, m_MoonPosition );

	int flaretextureindex[7] = 
	{	
		g_TerrainTextureArchive.GetResourceIndex( "flare1.tga" ),
		g_TerrainTextureArchive.GetResourceIndex( "flare2.tga" ),
		g_TerrainTextureArchive.GetResourceIndex( "flare3.tga" ),
		g_TerrainTextureArchive.GetResourceIndex( "flare4.tga" ),
		g_TerrainTextureArchive.GetResourceIndex( "flare5.tga" ),
		g_TerrainTextureArchive.GetResourceIndex( "flare6.tga" ),
		g_TerrainTextureArchive.GetResourceIndex( "flare7.tga" )
	};


	CreateFlare(0 , 1.2f,  50.0f,flaretextureindex[2]);
	CreateFlare(1 , 2.5f , 38.0f,flaretextureindex[3]);
	CreateFlare(2 , 2.8f,  24.0f,flaretextureindex[4]);
	CreateFlare(3 , 1.2f,  13.0f, flaretextureindex[0]);
	CreateFlare(4 , 0.9f,  6.0f, flaretextureindex[5]);
	CreateFlare(5 , 9.8f,  4.0f,flaretextureindex[6]);
	CreateFlare(6 , 0.7f,  2.8f, flaretextureindex[2]);
	CreateFlare(7 , 1.8f,  0.3f,flaretextureindex[1]);
	 
	
	m_burnout[0]=_XTLVERTEX( 0.0f,0.0f,0.0f						,0.001f,D3DCOLOR_RGBA(0,255,255,0),0,0.0f,0.0f);
	m_burnout[1]=_XTLVERTEX( (FLOAT)gnWidth,0.0f,0.0f			,0.001f,D3DCOLOR_RGBA(255,255,255,0),0,0.0f,0.0f);
	m_burnout[2]=_XTLVERTEX( 0.0f,(FLOAT)gnHeight,0.0f			,0.001f,D3DCOLOR_RGBA(0,255,255,0),0,0.0f,0.0f);
	m_burnout[3]=_XTLVERTEX( (FLOAT)gnWidth,(FLOAT)gnHeight,0.0f,0.001f,D3DCOLOR_RGBA(255,255,255,0),0,0.0f,0.0f);
}


void _XLensFlare::CreateFlare(int index,float size,float flare_CameraPosition,int tex)
{
	if( _XDEF_FLARECOUNT <= index ) return;

	m_flares[index].pLensflare=new _XBillboard(size,size,D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_flares[index].AxisPos=flare_CameraPosition;
	m_flares[index].textureindex=tex;
}

void _XLensFlare::RebuildPosition( void )
{
	m_SunBillboard.SetPosition( D3DXVECTOR3(
								g_LodTerrain.m_3PCamera.m_CameraPosition.x+m_SunPosition.x,
								m_SunPosition.y,
								g_LodTerrain.m_3PCamera.m_CameraPosition.z+m_SunPosition.z) );

	m_MoonBillboard.SetPosition( D3DXVECTOR3(
								g_LodTerrain.m_3PCamera.m_CameraPosition.x+m_MoonPosition.x,
								m_MoonPosition.y,
								g_LodTerrain.m_3PCamera.m_CameraPosition.z+m_MoonPosition.z) );
}

void _XLensFlare::RenderSun( int daynight )
{	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	if( daynight == _XE_NIGHT )
	{	
		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_MOON );
		g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_MoonTextureIndex );		
		m_MoonBillboard.Render( g_LodTerrain.m_3PCamera.m_CameraPosition );
	}
	else if( daynight != _XE_DAWN )
	{		
		/*D3DCOLOR color = ( g_SunTwinkleAlphaLevel[m_SunTwinkleRateIndex++] << 24) + 0xFFFFFF;	
		if( m_SunTwinkleRateIndex >= 10 ) m_SunTwinkleRateIndex = 0;

		gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, color );
		gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    
		gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);*/
				
		if( m_SunZAngle < 360.0f )
		{
			m_SunZAngle+=0.07f;
		}
		else
		{
			m_SunZAngle = 0;
		}

		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_SUN );
		g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_SunTextureIndex );
		m_SunBillboard.Render( g_LodTerrain.m_3PCamera.m_CameraPosition, m_SunZAngle );		
	}

	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
}

void _XLensFlare::Render( void )
{			
	D3DXVECTOR3 DirectionToSun = ( m_SunBillboard.Position - g_LodTerrain.m_3PCamera.m_CameraPosition );  

	D3DXVec3Normalize( &DirectionToSun, &DirectionToSun );
		
	float AngleToSun = D3DXVec3Dot( &g_LodTerrain.m_3PCamera.m_LookVector, &DirectionToSun );

	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_LENSFLARE );

	int alpha = ((int)( (AngleToSun-0.9f) * 1980.0f ) >> 1);
	if (alpha<0) alpha=0;
	if (alpha>32) alpha=32;

	// 해가 질 무렵 번 빌보드 알파 수정
	FLOAT sunangle = _X_DEG(g_EnvironmentManager.m_SunAngle);
	if( sunangle > 140.0f )
	{
		FLOAT alphadecreaserate = 1.0f - ( (  sunangle - 140.0f ) / 40.0f );

		alpha = (int)( (FLOAT)alpha * alphadecreaserate );
	}
	
	if ( sunangle > 3.0f && alpha > 5 )
	{

		if( AngleToSun>0.4f)
		{					
			D3DXVECTOR3 lookatvector;
			D3DXVECTOR3 pivotcenter = g_LodTerrain.m_3PCamera.m_CameraPosition + (15.0f * g_LodTerrain.m_3PCamera.m_LookVector );
			lookatvector = m_SunBillboard.Position - pivotcenter;
			D3DXVec3Normalize( &lookatvector, &lookatvector );
			
			D3DCOLOR color = ((DWORD)alpha << 24) + 0xFFFFFF;	
			gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, color );

			for (int i=0; i < _XDEF_FLARECOUNT; i++)
			{
				m_flares[i].pLensflare->Position = pivotcenter + ( m_flares[i].AxisPos * lookatvector );
				g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_flares[i].textureindex );
				m_flares[i].pLensflare->RenderCheap();
			}

			gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
		}
			
		if (AngleToSun>0.85f)
		{		
			gpDev->SetTexture(0,NULL);			
			m_burnout[0].color = D3DCOLOR_RGBA(255,255,255,alpha);
			m_burnout[1].color = m_burnout[0].color;
			m_burnout[2].color = m_burnout[0].color;
			m_burnout[3].color = m_burnout[0].color;				
			
			gpDev->SetFVF( D3DFVF_XTLVERTEX );
						
			gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2,m_burnout, sizeof(_XTLVERTEX) );
		}
	}

/*	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );

	//gpDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);	
	gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_LodTerrain.m_Fog );
	gpDev->SetRenderState(D3DRS_ZENABLE, g_RS_UseZBuffer );	
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	
	gpDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	gpDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	gpDev->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//gpDev->SetRenderState( D3DRS_AMBIENT, g_LodTerrain.m_AmbientColor );*/
}


