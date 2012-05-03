// XEnvironmentManager.cpp: implementation of the _XEnvironmentManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XEnvironmentManager.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XEnvironmentManager g_EnvironmentManager;

_XEnvironmentManager::_XEnvironmentManager()
{
	m_nTimeSpeed				= 1;
    m_ZValueNear_ViewPlane		=	1.0f;
    m_ZValueFar_ViewPlane		=	250.f;

	m_bInterpolationEnvironment	= FALSE;
	m_EnvironmentLerpLevel		= 2.0f;
	m_EnvironmentTime			=	900000;
	m_PrevEnvironmentTime		=	0;
	m_SunAngle					=	0.0f;

	m_SkyBoxHeightOffset		=   0.0f;

	m_Environment				=	_XE_DAY;

	m_NextWeatherData = m_WeatherData[0];

	D3DUtil_InitLight( m_ObjectLight, D3DLIGHT_DIRECTIONAL, 100.0f, 40.0f, -100.0f );
	m_ObjectLight.Ambient  = m_WeatherData[0].ObjectLightAmbientColor;
	m_ObjectLight.Diffuse  = m_WeatherData[0].ObjectLightDiffuseColor;
	
	m_ObjectLight.Theta		= _X_RAD(30.0f);
	m_ObjectLight.Phi		= _X_RAD(70.0f);
	m_ObjectLight.Range	  = 1000.0f;
	m_ObjectLight.Falloff = 0.0f;
			
	m_ObjectLight.Attenuation0 = 1.0f;
	m_ObjectLight.Attenuation1 = 0.0f;
	m_ObjectLight.Attenuation2 = 0.0f;// 20.0f / powf(300,2);

	D3DXVECTOR3 ldnv = D3DXVECTOR3( 0.650945f, -0.390567f, -0.650945f );
	D3DXVec3Normalize( &ldnv, &ldnv );
	m_ObjectLight.Direction = ldnv;

	m_CharacterLight = m_TerrainLight = m_ObjectLight;
	m_TerrainLight.Ambient  = m_WeatherData[0].TerrainLightAmbientColor;
	m_TerrainLight.Diffuse  = m_WeatherData[0].TerrainLightDiffuseColor;

	m_CharacterLight.Ambient  = m_WeatherData[0].CharacterLightAmbientColor;
	m_CharacterLight.Diffuse  = m_WeatherData[0].CharacterLightDiffuseColor;
	
	D3DXMatrixIdentity( &m_CloudShadowMatrix );	
	m_CloudShadowUVX				=  0;
	m_CloudShadowUVZ				=  0;
	m_CloudShadowMoveSpeed			=  _XDEF_CLOUDMOVESPEED_NORMAL;

	m_TerrainCloudShadowTexureIndex = -1;
	m_SkyBoxTextureIndex[0] = -1;
	m_SkyBoxTextureIndex[1] = -1;
	m_SkyBoxTextureIndex[2] = -1;

	m_OldSkyBoxTextureIndex[0] = -1;
	m_OldSkyBoxTextureIndex[1] = -1;
	m_OldSkyBoxTextureIndex[2] = -1;

	m_BackgroundColor			=	D3DCOLOR_XRGB( 0, 0, 0 );
    m_Fog						=	TRUE;
    m_RangeBaseFog				=	FALSE;
    m_TableFogExpMode			=	D3DFOG_EXP2;
    m_FogMode					=	0;
    m_FogNear					=	100.0f;
    m_FogFar					=	250.0f;
	m_FogDensity				=	0.0038f;
	m_FogColor					=	D3DCOLOR_XRGB( 220, 220, 220 );

	m_ViewLensFlare				=	TRUE;
	m_bViewMiniHPBar			=	TRUE;
	m_TerrainAmbientColor		=	D3DCOLOR_COLORVALUE( 0.50, 0.50, 0.50, 1.0 );
	m_ObjectAmbientColor		=	D3DCOLOR_COLORVALUE( 0.50, 0.50, 0.50, 1.0 );
	m_CharacterAmbientColor		=	D3DCOLOR_COLORVALUE( 0.50, 0.50, 0.50, 1.0 );
}

_XEnvironmentManager::~_XEnvironmentManager()
{
	ReleaseSkyBoxModel();
}

BOOL _XEnvironmentManager::Initialize( void )							   
{
	// set skybox matrix
	D3DXMatrixIdentity(&m_SkyBoxMatrix);
	D3DXMatrixIdentity(&m_SkyBoxReflectionMatrix);
	D3DXMatrixIdentity(&m_SkyRotationMatrix);
	D3DXMatrixIdentity(&m_CloudSphereMatrix);
	D3DXMatrixIdentity(&m_CloudSphereRotationMatrix);

	m_SkyBoxAlphaValue = 0;

	_XLitVertex	SkyBoxVertexList[16];
	USHORT		SkyBoxIndexList[3][24] = 
	{
		{ 1, 3, 0, 2, 0, 3, 6, 7, 4, 5, 4, 7, },	
		{ 8, 1, 6, 3, 6, 1, 4, 0, 9, 2, 9, 0, 12, 7, 10, 14, 10, 7, 11, 15, 13, 5, 13, 15 },
		{ 2, 6, 0, 4, 0, 6, 1, 8, 3, 7, 3, 8, 14, 11, 9, 10, 9, 11, 5, 13, 15, 12, 15, 13 }
	};
	
	SAFE_RELEASE( m_pSkyBoxVertexBuffer[0] );
	SAFE_RELEASE( m_pSkyBoxVertexBuffer[1] );
	SAFE_RELEASE( m_pSkyBoxVertexBuffer[2] );

	SAFE_RELEASE( m_pSkyBoxIndexBuffer[0] );
	SAFE_RELEASE( m_pSkyBoxIndexBuffer[1] );
	SAFE_RELEASE( m_pSkyBoxIndexBuffer[2] );

	HRESULT hr = gpDev->CreateVertexBuffer( sizeof(_XLitVertex) * 8,  D3DUSAGE_WRITEONLY, (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1), D3DPOOL_MANAGED, &m_pSkyBoxVertexBuffer[0], NULL );    
	if ( FAILED( hr ) ) return FALSE;
	hr = gpDev->CreateVertexBuffer( sizeof(_XLitVertex) * 16, D3DUSAGE_WRITEONLY, (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1), D3DPOOL_MANAGED, &m_pSkyBoxVertexBuffer[1], NULL );    
	if ( FAILED( hr ) ) return FALSE;
	hr = gpDev->CreateVertexBuffer( sizeof(_XLitVertex) * 16, D3DUSAGE_WRITEONLY, (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1), D3DPOOL_MANAGED, &m_pSkyBoxVertexBuffer[2], NULL );
	if ( FAILED( hr ) ) return FALSE;

	hr = gpDev->CreateIndexBuffer( sizeof(WORD) * 12, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pSkyBoxIndexBuffer[0], NULL );
    if ( FAILED( hr ) ) return FALSE;
	hr = gpDev->CreateIndexBuffer( sizeof(WORD) * 24, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pSkyBoxIndexBuffer[1], NULL );
    if ( FAILED( hr ) ) return FALSE;
	hr = gpDev->CreateIndexBuffer( sizeof(WORD) * 24, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pSkyBoxIndexBuffer[2], NULL );
    if ( FAILED( hr ) ) return FALSE;

	
	const float skyboxsize = 136.0f;
	float skyboxhalfsize = skyboxsize / 2.0f; 

    SkyBoxVertexList[0] = _XLitVertex( -skyboxhalfsize,	skyboxhalfsize,	-skyboxhalfsize, 0.0f, 1.0f );
	SkyBoxVertexList[1] = _XLitVertex(  skyboxhalfsize,	skyboxhalfsize,	-skyboxhalfsize, 1.0f, 1.0f );
	SkyBoxVertexList[2] = _XLitVertex( -skyboxhalfsize,	skyboxhalfsize,	 skyboxhalfsize, 0.0f,-0.0f );
	SkyBoxVertexList[3] = _XLitVertex(  skyboxhalfsize,	skyboxhalfsize,	 skyboxhalfsize, 1.0f,-0.0f );
	SkyBoxVertexList[4] = _XLitVertex(  skyboxhalfsize, -skyboxhalfsize,	-skyboxhalfsize, 0.0f,-0.0f );
	SkyBoxVertexList[5] = _XLitVertex(  skyboxhalfsize,	-skyboxhalfsize, skyboxhalfsize, 1.0f,-0.0f );
	SkyBoxVertexList[6] = _XLitVertex( -skyboxhalfsize,	-skyboxhalfsize,-skyboxhalfsize, 0.0f, 1.0f );
	SkyBoxVertexList[7] = _XLitVertex( -skyboxhalfsize,	-skyboxhalfsize, skyboxhalfsize, 1.0f, 1.0f );

	// Lock the vertex buffer ready to fill data
	void* pVertex = NULL;
	hr = m_pSkyBoxVertexBuffer[0]->Lock( 0, sizeof(_XLitVertex) * 8, (void**)&pVertex, 0 );	    
	memcpy( pVertex, SkyBoxVertexList, sizeof(_XLitVertex) * 8 ); 
    hr = m_pSkyBoxVertexBuffer[0]->Unlock();

	LPWORD pIndex;
	hr = m_pSkyBoxIndexBuffer[0]->Lock( 0, sizeof(WORD) * 12, (void**)&pIndex, 0 );
    if ( FAILED( hr ) ) return FALSE;	
    memcpy( pIndex, SkyBoxIndexList[0], sizeof(WORD) * 12 );
    m_pSkyBoxIndexBuffer[0]->Unlock();

	SkyBoxVertexList[0] = _XLitVertex( -skyboxhalfsize,  skyboxhalfsize, -skyboxhalfsize, 1.0f, 1.0f );
	SkyBoxVertexList[1] = _XLitVertex( -skyboxhalfsize,  skyboxhalfsize,  skyboxhalfsize, 1.0f,-0.0f );
	SkyBoxVertexList[2] = _XLitVertex( -skyboxhalfsize,  skyboxhalfsize,  skyboxhalfsize, 0.0f, 1.0f );
	SkyBoxVertexList[3] = _XLitVertex(  skyboxhalfsize , skyboxhalfsize,  skyboxhalfsize, 0.0f,-0.0f );
	SkyBoxVertexList[4] = _XLitVertex( -skyboxhalfsize,  0.0f,           -skyboxhalfsize, 1.0f, 0.5f );
	SkyBoxVertexList[5] = _XLitVertex( -skyboxhalfsize,  0.0f,           -skyboxhalfsize, 1.0f, 0.5f );
	SkyBoxVertexList[6] = _XLitVertex(  skyboxhalfsize , 0.0f,            skyboxhalfsize, 0.0f, 0.5f );
	SkyBoxVertexList[7] = _XLitVertex(  skyboxhalfsize , 0.0f,            skyboxhalfsize, 0.0f, 0.5f );
	SkyBoxVertexList[8] = _XLitVertex( -skyboxhalfsize,  0.0f,            skyboxhalfsize, 1.0f, 0.5f );
	SkyBoxVertexList[9] = _XLitVertex( -skyboxhalfsize,  0.0f,            skyboxhalfsize, 0.0f, 0.5f );
	SkyBoxVertexList[10]= _XLitVertex(  skyboxhalfsize , -skyboxhalfsize, -skyboxhalfsize,1.0f,-0.0f );
	SkyBoxVertexList[11]= _XLitVertex(  skyboxhalfsize , -skyboxhalfsize, -skyboxhalfsize,0.0f, 1.0f );
	SkyBoxVertexList[12]= _XLitVertex(  skyboxhalfsize , -skyboxhalfsize,  skyboxhalfsize,0.0f,-0.0f );
	SkyBoxVertexList[13]= _XLitVertex( -skyboxhalfsize,  -skyboxhalfsize, -skyboxhalfsize,1.0f, 1.0f );
	SkyBoxVertexList[14]= _XLitVertex(  skyboxhalfsize , 0.0f,            -skyboxhalfsize,1.0f, 0.5f );
	SkyBoxVertexList[15]= _XLitVertex(  skyboxhalfsize , 0.0f,            -skyboxhalfsize,0.0f, 0.5f );
	hr = m_pSkyBoxVertexBuffer[1]->Lock( 0, sizeof(_XLitVertex) * 16, (void**)&pVertex, 0 );	    
	memcpy( pVertex, SkyBoxVertexList, sizeof(_XLitVertex) * 16 ); 
    hr = m_pSkyBoxVertexBuffer[1]->Unlock(); 
	hr = m_pSkyBoxIndexBuffer[1]->Lock( 0, sizeof(WORD) * 24, (void**)&pIndex, 0 );
    if ( FAILED( hr ) ) return FALSE;	
    memcpy( pIndex, SkyBoxIndexList[1], sizeof(WORD) * 24 );
    m_pSkyBoxIndexBuffer[1]->Unlock();

	SkyBoxVertexList[0] = _XLitVertex(  skyboxhalfsize,  -skyboxhalfsize, skyboxhalfsize, 1.0f,1.0f);
	SkyBoxVertexList[1] = _XLitVertex( -skyboxhalfsize,  -skyboxhalfsize, -skyboxhalfsize,0.0f,0.0f);
	SkyBoxVertexList[2] = _XLitVertex( -skyboxhalfsize,  -skyboxhalfsize, skyboxhalfsize, 0.0f,1.0f);
	SkyBoxVertexList[3] = _XLitVertex( -skyboxhalfsize,	-skyboxhalfsize, skyboxhalfsize, 1.0f,0.0f);
	SkyBoxVertexList[4] = _XLitVertex(  skyboxhalfsize,  0.0f,           skyboxhalfsize,  1.0f,0.5f);
	SkyBoxVertexList[5] = _XLitVertex(  skyboxhalfsize,  0.0f,           skyboxhalfsize,  1.0f,0.5f);
	SkyBoxVertexList[6] = _XLitVertex( -skyboxhalfsize,  0.0f,           skyboxhalfsize,  0.0f,0.5f);
	SkyBoxVertexList[7] = _XLitVertex( -skyboxhalfsize,  0.0f,           skyboxhalfsize,  1.0f,0.5f);
	SkyBoxVertexList[8] = _XLitVertex( -skyboxhalfsize,  0.0f,          -skyboxhalfsize,  0.0f,0.5f);
	SkyBoxVertexList[9] = _XLitVertex( -skyboxhalfsize,  0.0f,          -skyboxhalfsize,  0.0f,0.5f);
	SkyBoxVertexList[10]= _XLitVertex( -skyboxhalfsize,  skyboxhalfsize,-skyboxhalfsize,  0.0f,0.0f);
	SkyBoxVertexList[11]= _XLitVertex(  skyboxhalfsize,  skyboxhalfsize,-skyboxhalfsize,  1.0f,0.0f);
	SkyBoxVertexList[12]= _XLitVertex(  skyboxhalfsize,  skyboxhalfsize,-skyboxhalfsize,  0.0f,1.0f);
	SkyBoxVertexList[13]= _XLitVertex(  skyboxhalfsize,  skyboxhalfsize, skyboxhalfsize,  1.0f,1.0f);
	SkyBoxVertexList[14]= _XLitVertex(  skyboxhalfsize,  0.0f,          -skyboxhalfsize,  1.0f,0.5f);
	SkyBoxVertexList[15]= _XLitVertex(  skyboxhalfsize,  0.0f,          -skyboxhalfsize,  0.0f,0.5f);
	hr = m_pSkyBoxVertexBuffer[2]->Lock( 0, sizeof(_XLitVertex) * 16, (void**)&pVertex, 0 );	    
	memcpy( pVertex, SkyBoxVertexList, sizeof(_XLitVertex) * 16 ); 
    hr = m_pSkyBoxVertexBuffer[2]->Unlock(); 
	hr = m_pSkyBoxIndexBuffer[2]->Lock( 0, sizeof(WORD) * 24, (void**)&pIndex, 0 );
    if ( FAILED( hr ) ) return FALSE;	
    memcpy( pIndex, SkyBoxIndexList[2], sizeof(WORD) * 24 );
    m_pSkyBoxIndexBuffer[2]->Unlock();
	
	return TRUE;
}

void _XEnvironmentManager::ReleaseSkyBoxModel( void )
{
	SAFE_RELEASE( m_pSkyBoxVertexBuffer[0] );
	SAFE_RELEASE( m_pSkyBoxVertexBuffer[1] );
	SAFE_RELEASE( m_pSkyBoxVertexBuffer[2] );
	
	SAFE_RELEASE( m_pSkyBoxIndexBuffer[0] );
	SAFE_RELEASE( m_pSkyBoxIndexBuffer[1] );
	SAFE_RELEASE( m_pSkyBoxIndexBuffer[2] );
}

#define _XGET_ALPHA( x ) (( (x) & 0xFF000000 ) >> 24)
#define _XGET_RED( x )   (( (x) & 0xFF0000 ) >> 16)
#define _XGET_GREEN( x ) (( (x) & 0xFF00 ) >> 8)
#define _XGET_BLUE( x )  (( (x) & 0xFF ) )

BOOL _XEnvironmentManager::Load(FILE* terrainfile)
{
	// Load weather property data...
	if( fread( m_WeatherData, sizeof( _XENVIRONMENT_WEATHERDATA ) * _XDEF_MAXWEATHERTYPE, 1, terrainfile ) < 1 )
	{
		return FALSE;		
	}

	if( !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation )
	{
		for( int i = 0; i < _XDEF_MAXWEATHERTYPE; i++ )
		{
			int red   = _XGET_RED(m_WeatherData[i].CharacterAmbientColor) + 50;
			int green = _XGET_GREEN(m_WeatherData[i].CharacterAmbientColor) + 50;
			int blue  = _XGET_BLUE(m_WeatherData[i].CharacterAmbientColor) + 50;

			if( red > 255 ) red = 255;
			if( green > 255 ) green = 255;
			if( blue > 255 ) blue = 255;

			m_WeatherData[i].CharacterAmbientColor = D3DCOLOR_ARGB( 0xFF, red, green, blue );
		}
	}

	// Set Env value...		
	m_NextWeatherData = m_WeatherData[m_Environment];


	gpDev->SetRenderState( D3DRS_FOGSTART,   FtoDW(m_FogNear) );
	gpDev->SetRenderState( D3DRS_FOGEND,     FtoDW(m_FogFar) );
	gpDev->SetRenderState( D3DRS_FOGDENSITY, FtoDW(m_FogDensity) );
		
	m_FogColor					= m_NextWeatherData.FogColor;
	gpDev->SetRenderState( D3DRS_FOGCOLOR,  m_FogColor );
	
	m_TerrainAmbientColor		= m_WeatherData[0].TerrainAmbientColor;
	m_TerrainLight.Ambient		= m_WeatherData[0].TerrainLightAmbientColor;
	m_TerrainLight.Diffuse		= m_WeatherData[0].TerrainLightDiffuseColor;

	m_ObjectAmbientColor		= m_WeatherData[0].ObjectAmbientColor;
	m_ObjectLight.Ambient		= m_WeatherData[0].ObjectLightAmbientColor;
	m_ObjectLight.Diffuse		= m_WeatherData[0].ObjectLightDiffuseColor;	

	m_CharacterAmbientColor		= m_WeatherData[0].CharacterAmbientColor;
	m_CharacterLight.Ambient	= m_WeatherData[0].CharacterLightAmbientColor;
	m_CharacterLight.Diffuse	= m_WeatherData[0].CharacterLightDiffuseColor;

	m_TerrainCloudShadowTexureIndex = g_TerrainTextureArchive.GetResourceIndex( _T("cloudsshadow.tga"), 0 );

	m_SkyBoxTextureIndex[0]			= g_TerrainTextureArchive.GetResourceIndex( m_WeatherData[0].skybox_texturename[0], 1 );
	m_SkyBoxTextureIndex[1]			= g_TerrainTextureArchive.GetResourceIndex( m_WeatherData[0].skybox_texturename[1], 1 );
	m_SkyBoxTextureIndex[2]			= g_TerrainTextureArchive.GetResourceIndex( m_WeatherData[0].skybox_texturename[2], 1 );

	D3DXMatrixRotationY( &m_SkyRotationMatrix, 0.0002f );
	D3DXMatrixRotationY( &m_CloudSphereRotationMatrix, 0.0003f );

	// 플레어가 사용하는 텍스쳐 때문에 여기에 둔다.
	m_LensFlare.Init();
	return TRUE;
}

void _XEnvironmentManager::SetChangeEnvironmentTime(int time )
{
	m_EnvironmentTime = time;
	
	if( m_EnvironmentTime >= _XDEF_ENVTIMESTARTTIME_NIGHT ) // Night
	{
		m_Environment = _XE_NIGHT;
		m_SkyBoxAlphaValue = 255;
	}
	else if( m_EnvironmentTime >= _XDEF_ENVTIMESTARTTIME_NIGHTFALL ) // NightFall
	{
		m_Environment = _XE_NIGHTFALL;		
	}
	else if( m_EnvironmentTime >= _XDEF_ENVTIMESTARTTIME_DAY ) // Day
	{
		m_Environment = _XE_DAY;		
	}
	else // Dawn
	{
		m_Environment = _XE_DAWN;
		m_SkyBoxAlphaValue = 0;
	}	
	
	m_NextWeatherData = m_WeatherData[m_Environment];

	m_FogNear = m_NextWeatherData.FogStart;
	m_FogFar = m_NextWeatherData.FogEnd;
	m_FogColor = m_NextWeatherData.FogColor;

	m_TerrainAmbientColor = m_NextWeatherData.TerrainAmbientColor;
	m_ObjectAmbientColor = m_NextWeatherData.ObjectAmbientColor;

	m_TerrainAmbientColor = m_NextWeatherData.TerrainAmbientColor;
	m_TerrainLight.Ambient  = m_NextWeatherData.TerrainLightAmbientColor;
	m_TerrainLight.Diffuse  = m_NextWeatherData.TerrainLightDiffuseColor;

	m_ObjectAmbientColor = m_NextWeatherData.ObjectAmbientColor;
	m_ObjectLight.Ambient  = m_NextWeatherData.ObjectLightAmbientColor;
	m_ObjectLight.Diffuse  = m_NextWeatherData.ObjectLightDiffuseColor;	

	m_CharacterAmbientColor	= m_NextWeatherData.CharacterAmbientColor;
	m_CharacterLight.Ambient	= m_NextWeatherData.CharacterLightAmbientColor;
	m_CharacterLight.Diffuse	= m_NextWeatherData.CharacterLightDiffuseColor;

	D3DXVECTOR3 ldnv = /*D3DXVECTOR3( 100.0f, 40.0f, -100.0f );//*/D3DXVECTOR3( 0.650945f, -0.390567f, -0.650945f );
	D3DXVec3Normalize( &ldnv, &ldnv );
	m_TerrainLight.Direction = ldnv;
	m_ObjectLight.Direction = ldnv;
	m_CharacterLight.Direction = ldnv;

	m_SkyBoxTextureIndex[0]			= g_TerrainTextureArchive.AddTextureList( m_NextWeatherData.skybox_texturename[0] );
	m_SkyBoxTextureIndex[1]			= g_TerrainTextureArchive.AddTextureList( m_NextWeatherData.skybox_texturename[1] );
	m_SkyBoxTextureIndex[2]			= g_TerrainTextureArchive.AddTextureList( m_NextWeatherData.skybox_texturename[2] );

	SetEnvironmentFog();
}

void _XEnvironmentManager::SetEnvironmentFog( void )
{	
	// Set fog state machine...
	gpDev->SetRenderState( D3DRS_FOGENABLE, m_Fog );
	gpDev->SetRenderState( D3DRS_FOGCOLOR,  m_FogColor );

	if( m_FogMode == 0 )
	{
		// Vertex fog...		
		if( g_bCanDoVertexFog )
		{
			gpDev->SetRenderState( D3DRS_FOGSTART,   FtoDW(m_FogNear) );
			gpDev->SetRenderState( D3DRS_FOGEND,     FtoDW(m_FogFar) );

			gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
			gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
			gpDev->SetRenderState( D3DRS_RANGEFOGENABLE, g_bCanDoRangeFog );
		}
		else
		{
			gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
			gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
		}
	}
	else
	{
		// Table fog...
		if(g_bCanDoTableFog)
		{
			gpDev->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE );
			gpDev->SetRenderState( D3DRS_FOGDENSITY,	FtoDW(m_FogDensity) );
			gpDev->SetRenderState( D3DRS_FOGTABLEMODE,  m_TableFogExpMode );
		}
		else
		{
			if( g_bCanDoVertexFog )
			{
				gpDev->SetRenderState( D3DRS_FOGSTART,   FtoDW(m_FogNear) );
				gpDev->SetRenderState( D3DRS_FOGEND,     FtoDW(m_FogFar) );

				gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
				gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
				gpDev->SetRenderState( D3DRS_RANGEFOGENABLE, g_bCanDoRangeFog );
			}
			else
			{
				gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
				gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
			}
		}
	}	
}

void _XEnvironmentManager::DrawSkyBox( void )
{
	if( m_SkyBoxHeightOffset >= 1000.0f ) return;
	
	/*
	if(m_ViewLensFlare )
	{		
		m_LensFlare.RenderSun( m_Environment );		
	}	
	// Render cloud sphere
	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_CLOUDSPHERE );
	m_CloudSphere.RenderCustomDraw( m_CloudSphereMatrix );*/

	gpDev->SetLight( 0, &m_ObjectLight );
	gpDev->SetRenderState( D3DRS_AMBIENT, m_ObjectAmbientColor );

	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_SKYBOX );
	
	Draw();

	if(m_ViewLensFlare )
	{		
		if( g_CurrentZoneInfoIndex != 6 ) // 마교 염마산일때는 렌즈플래어 출력 않함.
		{
			m_LensFlare.RenderSun( m_Environment );		
		}
	}
}

void _XEnvironmentManager::Draw( BOOL reflectmode )
{
	if( reflectmode )
		gpDev->SetTransform( D3DTS_WORLD, &m_SkyBoxReflectionMatrix );
	else
		gpDev->SetTransform( D3DTS_WORLD, &m_SkyBoxMatrix);	

	gpDev->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 );

	// Render top bottom sky
	gpDev->SetIndices( m_pSkyBoxIndexBuffer[0] );
	gpDev->SetStreamSource( 0, m_pSkyBoxVertexBuffer[0], 0, sizeof( _XLitVertex ) );
	g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_SkyBoxTextureIndex[0] );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 8, 0, 4 );

	// Render side sky 1
	gpDev->SetIndices( m_pSkyBoxIndexBuffer[1] );
	gpDev->SetStreamSource( 0, m_pSkyBoxVertexBuffer[1], 0, sizeof( _XLitVertex ) );
	g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_SkyBoxTextureIndex[1] );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 16, 0, 8 );

	// Render side sky 2
	gpDev->SetIndices( m_pSkyBoxIndexBuffer[2] );
	gpDev->SetStreamSource( 0, m_pSkyBoxVertexBuffer[2], 0, sizeof( _XLitVertex ) );
	g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_SkyBoxTextureIndex[2] );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 16, 0, 8 );

	if (m_bInterpolationEnvironment && m_EnvironmentLerpLevel < 0.95f)
	{
		DWORD alpha = 254 - (m_EnvironmentLerpLevel * 254);
		alpha = alpha << 24;

		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		gpDev->SetRenderState(D3DRS_TEXTUREFACTOR, alpha);
		gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		gpDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		gpDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );

		// Render top bottom sky
		gpDev->SetIndices( m_pSkyBoxIndexBuffer[0] );
		gpDev->SetStreamSource( 0, m_pSkyBoxVertexBuffer[0], 0, sizeof( _XLitVertex ) );
		g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_OldSkyBoxTextureIndex[0] );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 8, 0, 4 );

		// Render side sky 1
		gpDev->SetIndices( m_pSkyBoxIndexBuffer[1] );
		gpDev->SetStreamSource( 0, m_pSkyBoxVertexBuffer[1], 0, sizeof( _XLitVertex ) );
		g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_OldSkyBoxTextureIndex[1] );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 16, 0, 8 );

		// Render side sky 2
		gpDev->SetIndices( m_pSkyBoxIndexBuffer[2] );
		gpDev->SetStreamSource( 0, m_pSkyBoxVertexBuffer[2], 0, sizeof( _XLitVertex ) );
		g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_OldSkyBoxTextureIndex[2] );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 16, 0, 8 );

		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		gpDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	}
}

BOOL _XEnvironmentManager::Process(D3DXVECTOR3& vPosition, D3DXVECTOR3& vReflectPosition, FLOAT fElapsedFrameMilisecondTime, DWORD dwLocalSystemTime)
{
	ProcessSkyBox(vPosition, vReflectPosition, fElapsedFrameMilisecondTime);

	ProcessLensFlare(vPosition);

	return ProcessEnvironmets(vPosition, dwLocalSystemTime);
}

void _XEnvironmentManager::ProcessSkyBox(D3DXVECTOR3& vPosition, D3DXVECTOR3& vReflectPosition, FLOAT fElapsedFrameMilisecondTime)
{
	m_CloudShadowUVX += ( m_CloudShadowMoveSpeed * fElapsedFrameMilisecondTime );
	if(m_CloudShadowUVX >=  1.0f) m_CloudShadowUVX = 0.0f; 	
	m_CloudShadowMatrix._31 = m_CloudShadowUVX;

	// Reset skybox position
	m_SkyBoxMatrix._41 = vPosition.x;
	m_SkyBoxMatrix._42 = vPosition.y + m_SkyBoxHeightOffset;
	m_SkyBoxMatrix._43 = vPosition.z;	
	// Rotation sky box...
	D3DXMatrixMultiply( &m_SkyBoxMatrix, &m_SkyRotationMatrix, &m_SkyBoxMatrix );
		
	if( g_LodTerrain.m_pWaterObjectManager )
	{
		m_SkyBoxReflectionMatrix = m_SkyRotationMatrix;

		m_SkyBoxReflectionMatrix._41 = vReflectPosition.x;
		m_SkyBoxReflectionMatrix._42 = g_LodTerrain.m_pWaterObjectManager->m_WaterReflectionClipPlaneHeight;
		m_SkyBoxReflectionMatrix._43 = vReflectPosition.z;
	}
}

void _XEnvironmentManager::ProcessLensFlare(D3DXVECTOR3& vPosition)
{
	if( m_Environment == _XE_NIGHT )
	{
		FLOAT curangle = ((FLOAT)(m_EnvironmentTime - _XDEF_ENVTIMESTARTTIME_NIGHT )) / (_XDEF_ENVTIMESTARTTIME_MAX-_XDEF_ENVTIMESTARTTIME_NIGHT+200000.0f);	

		m_SunAngle = _X_RAD( 200.0f * curangle );

		m_LensFlare.m_MoonPosition = D3DXVECTOR3(100.0f,0.0f,-40.0f);
		_XMeshMath_XYRotateCoord( m_LensFlare.m_MoonPosition, m_SunAngle - _X_RAD(10.0f) );

		m_LensFlare.m_MoonPosition.y += vPosition.y;
	}
	else if( m_Environment == _XE_DAY || m_Environment == _XE_NIGHTFALL )
	{
		FLOAT curangle = ((FLOAT)(m_EnvironmentTime - _XDEF_ENVTIMESTARTTIME_DAY)) / (_XDEF_ENVTIMESTARTTIME_NIGHT-_XDEF_ENVTIMESTARTTIME_DAY);

		m_SunAngle = _X_RAD( 190.0f * curangle );

		m_LensFlare.m_SunPosition  = D3DXVECTOR3(100.0f,0.0f,-40.0f);

		_XMeshMath_XYRotateCoord( m_LensFlare.m_SunPosition, m_SunAngle );

		m_LensFlare.m_SunPosition.y += vPosition.y;
	}
	else
	{
	}
		
	m_LensFlare.RebuildPosition();
}

BOOL _XEnvironmentManager::ProcessEnvironmets(D3DXVECTOR3& vPosition, DWORD dwLocalSystemTime)
{	
	if( g_EnvTimeFlowStop ) return FALSE;
	
	// 시간에 따라서 타임 증가	
	if( m_PrevEnvironmentTime == 0 )
		m_PrevEnvironmentTime = dwLocalSystemTime;

	DWORD duringtime = (dwLocalSystemTime - m_PrevEnvironmentTime) * m_nTimeSpeed;
	m_PrevEnvironmentTime = dwLocalSystemTime;	

	m_EnvironmentTime += duringtime;

	BYTE byType, byOldType = m_Environment;
	if ((byType = CheckEnvironmentType(m_EnvironmentTime)) != m_Environment)
	{
		SetChangeEnvironmentType(byType);
	}

	if (m_bInterpolationEnvironment)
	{
		ChangingEnvironmets();
	}

	return byOldType != m_Environment ? TRUE : FALSE;
}

void _XEnvironmentManager::ChangingEnvironmets()
{
	if( m_EnvironmentLerpLevel >= 1.0f )
	{
		m_FogColor					= m_NextWeatherData.FogColor;
		m_FogNear					= m_NextWeatherData.FogStart;
		m_FogFar					= m_NextWeatherData.FogEnd;
		m_FogDensity				= m_NextWeatherData.FogDensity;

		m_ObjectAmbientColor		= m_NextWeatherData.ObjectAmbientColor;
		m_ObjectLight.Ambient		= m_NextWeatherData.ObjectLightAmbientColor;
		m_ObjectLight.Diffuse		= m_NextWeatherData.ObjectLightDiffuseColor;

		m_TerrainAmbientColor		= m_NextWeatherData.TerrainAmbientColor;
		m_TerrainLight.Ambient		= m_NextWeatherData.TerrainLightAmbientColor;
		m_TerrainLight.Diffuse		= m_NextWeatherData.TerrainLightDiffuseColor;

		m_CharacterAmbientColor		= m_NextWeatherData.CharacterAmbientColor;
		m_CharacterLight.Ambient	= m_NextWeatherData.CharacterLightAmbientColor;
		m_CharacterLight.Diffuse	= m_NextWeatherData.CharacterLightDiffuseColor;	

		m_EnvironmentLerpLevel		= 0.f;
		m_bInterpolationEnvironment = FALSE;
	}
	else
	{
		// fog
		m_FogNear		= m_OldWeatherData.FogStart + (m_NextWeatherData.FogStart - m_OldWeatherData.FogStart) * m_EnvironmentLerpLevel;
		m_FogFar		= m_OldWeatherData.FogEnd + (m_NextWeatherData.FogEnd - m_OldWeatherData.FogEnd) * m_EnvironmentLerpLevel;
		m_FogDensity	= m_OldWeatherData.FogDensity + (m_NextWeatherData.FogDensity - m_OldWeatherData.FogDensity) * m_EnvironmentLerpLevel;

		// Fog
		EnvironmentColorLerp(m_FogColor, m_OldWeatherData.FogColor, m_NextWeatherData.FogColor, m_EnvironmentLerpLevel);

		// Terrarin Ambient
		EnvironmentColorLerp(m_TerrainAmbientColor, m_OldWeatherData.TerrainAmbientColor, m_NextWeatherData.TerrainAmbientColor, m_EnvironmentLerpLevel);

		// object Ambient
		EnvironmentColorLerp(m_ObjectAmbientColor, m_OldWeatherData.ObjectAmbientColor, m_NextWeatherData.ObjectAmbientColor, m_EnvironmentLerpLevel);

		// character Ambient
		EnvironmentColorLerp(m_CharacterAmbientColor, m_OldWeatherData.CharacterAmbientColor, m_NextWeatherData.CharacterAmbientColor, m_EnvironmentLerpLevel);


		// Terrain Ligih
		EnvironmentColorLerp(m_TerrainLight.Ambient, m_OldWeatherData.TerrainLightAmbientColor, m_NextWeatherData.TerrainLightAmbientColor, m_EnvironmentLerpLevel);
		EnvironmentColorLerp(m_TerrainLight.Diffuse, m_OldWeatherData.TerrainLightDiffuseColor, m_NextWeatherData.TerrainLightDiffuseColor, m_EnvironmentLerpLevel);

		// Object Ligih
		EnvironmentColorLerp(m_ObjectLight.Ambient, m_OldWeatherData.ObjectLightAmbientColor, m_NextWeatherData.ObjectLightAmbientColor, m_EnvironmentLerpLevel);
		EnvironmentColorLerp(m_ObjectLight.Diffuse, m_OldWeatherData.ObjectLightDiffuseColor, m_NextWeatherData.ObjectLightDiffuseColor, m_EnvironmentLerpLevel);

		// Character Ligih
		EnvironmentColorLerp(m_CharacterLight.Ambient, m_OldWeatherData.CharacterLightAmbientColor, m_NextWeatherData.CharacterLightAmbientColor, m_EnvironmentLerpLevel);
		EnvironmentColorLerp(m_CharacterLight.Diffuse, m_OldWeatherData.CharacterLightDiffuseColor, m_NextWeatherData.CharacterLightDiffuseColor, m_EnvironmentLerpLevel);

		m_EnvironmentLerpLevel += 0.01f;//(0.0001f * duringtime);
	}

	gpDev->SetRenderState( D3DRS_FOGCOLOR,		m_FogColor );

	gpDev->SetRenderState( D3DRS_FOGSTART,		FtoDW(m_FogNear) );
	gpDev->SetRenderState( D3DRS_FOGEND,		FtoDW(m_FogFar) );
	gpDev->SetRenderState( D3DRS_FOGDENSITY,	FtoDW(m_FogDensity) );
}

void _XEnvironmentManager::EnvironmentColorLerp(D3DCOLOR& In_CurColor, D3DXCOLOR& In_orgColor, D3DXCOLOR& In_nextColor, FLOAT fLeapTime)
{
	D3DXCOLOR curColor;

	D3DXColorLerp(&curColor, &In_orgColor, &In_nextColor, fLeapTime);
	In_CurColor= curColor;
}

void _XEnvironmentManager::EnvironmentColorLerp(D3DCOLORVALUE& In_CurColor, D3DXCOLOR& In_orgColor, D3DXCOLOR& In_nextColor, FLOAT fLeapTime)
{
	D3DXCOLOR curColor;

	D3DXColorLerp(&curColor, &In_orgColor, &In_nextColor, fLeapTime);
	In_CurColor= curColor;
}

void _XEnvironmentManager::EnvironmentColorLerp(D3DCOLOR& In_CurColor, D3DCOLOR& In_orgColor, D3DCOLOR& In_nextColor, FLOAT fLeapTime)
{
	D3DXCOLOR curColor;

	D3DXCOLOR orgColor(In_orgColor);
	D3DXCOLOR nextColor(In_nextColor);

	D3DXColorLerp(&curColor, &orgColor, &nextColor, fLeapTime);
	In_CurColor= curColor;
}

void _XEnvironmentManager::EnvironmentColorLerp(D3DCOLORVALUE& In_CurColor, D3DCOLOR& In_orgColor, D3DCOLOR& In_nextColor, FLOAT fLeapTime)
{
	D3DXCOLOR curColor;

	D3DXCOLOR orgColor(In_orgColor);
	D3DXCOLOR nextColor(In_nextColor);

	D3DXColorLerp(&curColor, &orgColor, &nextColor, fLeapTime);
	In_CurColor= curColor;
}


BYTE _XEnvironmentManager::CheckEnvironmentType(DWORD dwEnvoronmentTime)
{
	if(dwEnvoronmentTime >= _XDEF_ENVTIMESTARTTIME_MAX)
	{
		m_EnvironmentTime -= _XDEF_ENVTIMESTARTTIME_MAX;

		return _XE_DAWN;
	}
	if( dwEnvoronmentTime >= _XDEF_ENVTIMESTARTTIME_NIGHT )
	{
		return _XE_NIGHT;
	}
	else if(dwEnvoronmentTime >= _XDEF_ENVTIMESTARTTIME_NIGHTFALL)
	{
		return _XE_NIGHTFALL;
	}
	else if(dwEnvoronmentTime >= _XDEF_ENVTIMESTARTTIME_DAY)
	{
		return _XE_DAY;
	}

	return _XE_DAWN;
}

void _XEnvironmentManager::SetChangeEnvironmentType(BYTE type)
{
	m_OldWeatherData = m_WeatherData[m_Environment];

	m_Environment = type;

	m_bInterpolationEnvironment = TRUE;
	m_EnvironmentLerpLevel = 0.0f;

	m_NextWeatherData = m_WeatherData[m_Environment];

	m_OldSkyBoxTextureIndex[0] = m_SkyBoxTextureIndex[0];
	m_OldSkyBoxTextureIndex[1] = m_SkyBoxTextureIndex[1];
	m_OldSkyBoxTextureIndex[2] = m_SkyBoxTextureIndex[2];

	m_SkyBoxTextureIndex[0]	= g_TerrainTextureArchive.AddTextureList( m_NextWeatherData.skybox_texturename[0] );
	m_SkyBoxTextureIndex[1]	= g_TerrainTextureArchive.AddTextureList( m_NextWeatherData.skybox_texturename[1] );
	m_SkyBoxTextureIndex[2]	= g_TerrainTextureArchive.AddTextureList( m_NextWeatherData.skybox_texturename[2] );
}

