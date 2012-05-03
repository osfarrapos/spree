// XEnvironmentManager.h: interface for the _XEnvironmentManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XENVIRONMENTMANAGER_H__F8B5349C_C1A7_4843_80B6_1055F1199638__INCLUDED_)
#define AFX_XENVIRONMENTMANAGER_H__F8B5349C_C1A7_4843_80B6_1055F1199638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XKernel.h"
#include "XLensFlare.h"

#define _XDEF_ENVTIMESTARTTIME_DAWN				0
#define _XDEF_ENVTIMESTARTTIME_DAY				1800000
#define _XDEF_ENVTIMESTARTTIME_NIGHTFALL	    10800000
#define _XDEF_ENVTIMESTARTTIME_NIGHT		    12600000
#define _XDEF_ENVTIMESTARTTIME_MAX			    14400000

#define _XDEF_CLOUDMOVESPEED_SLOW				0.005f
#define _XDEF_CLOUDMOVESPEED_NORMAL				0.01f
#define _XDEF_CLOUDMOVESPEED_MIDIUM				0.02f
#define _XDEF_CLOUDMOVESPEED_FAST				0.03f

// Environment type

#define _XDEF_MAXWEATHERTYPE	32
enum _XWEATHERINDEX
{	
	_XE_DAY	= 0x00,
	_XE_NIGHTFALL,
	_XE_NIGHT,
	_XE_DAWN,
	_XE_DAY_RAIN,
	_XE_NIGHT_RAIN,
	_XE_DAY_SNOW,
	_XE_NIGHT_SNOW
};

// Skybox vertex type
typedef struct _XLitVertex
{
    _XLitVertex( float fX, float fY, float fZ, float ftu = 0.0f, float ftv = 0.0f ) 
	{ 
		x = fX; y = fY; z = fZ; tu = ftu; tv = ftv;
		normal.x = normal.y = normal.z = 0.0f; 
	}
    
    _XLitVertex() 
	{ 
		x = 0.0f; y = 0.0f; z = 0.0f; tu = 0.0f; tv = 0.0f; 
		normal.x = normal.y = normal.z = 0.0f;
	}
	
    //-------------------------------------------------------------------------
    // Public Variables for This Class
    //-------------------------------------------------------------------------
    float       x;          // Vertex Position X Component
    float       y;          // Vertex Position Y Component
    float       z;          // Vertex Position Z Component    
	D3DXVECTOR3 normal;
    float       tu;         // Texture u coordinate
    float       tv;         // Texture v coordinate
	
}_XLitVertex;

typedef struct _XENVIRONMENT_WEATHERDATA
{
	D3DXCOLOR	FogColor;
	
	D3DCOLOR	TerrainAmbientColor;
	D3DXCOLOR	TerrainLightAmbientColor;
	D3DXCOLOR	TerrainLightDiffuseColor;
	
	D3DCOLOR	ObjectAmbientColor;
	D3DXCOLOR	ObjectLightAmbientColor;
	D3DXCOLOR	ObjectLightDiffuseColor;

	FLOAT		FogStart;
	FLOAT		FogEnd;
	FLOAT		FogDensity;

	D3DCOLOR	CharacterAmbientColor;
	D3DXCOLOR	CharacterLightAmbientColor;
	D3DXCOLOR	CharacterLightDiffuseColor;

	char		skybox_texturename[3][32];
	
	_XENVIRONMENT_WEATHERDATA(){};
} _XENVIRONMENT_WEATHERDATA;

class _XEnvironmentManager  
{
public:
	int						m_nTimeSpeed;
	int						m_SkyBoxTextureIndex[3];
	int						m_OldSkyBoxTextureIndex[3];
	LPDIRECT3DVERTEXBUFFER9 m_pSkyBoxVertexBuffer[3];
	LPDIRECT3DINDEXBUFFER9  m_pSkyBoxIndexBuffer[3];

	int						m_TerrainCloudShadowTexureIndex;

	FLOAT					m_SkyBoxHeightOffset;
	D3DXMATRIX				m_SkyBoxMatrix;
	D3DXMATRIX				m_SkyBoxReflectionMatrix;
	D3DXMATRIX				m_SkyRotationMatrix;
	DWORD					m_SkyBoxAlphaValue;	

	D3DXMATRIX				m_CloudSphereMatrix;
	D3DXMATRIX				m_CloudSphereRotationMatrix;

	D3DXMATRIX				m_CloudShadowMatrix;
	FLOAT					m_CloudShadowUVX;
	FLOAT					m_CloudShadowUVZ;
	FLOAT					m_CloudShadowMoveSpeed;

	D3DCOLOR				m_TerrainAmbientColor;
	D3DCOLOR				m_ObjectAmbientColor;
	D3DCOLOR				m_CharacterAmbientColor;
	
	// Light handler--------------------------------------------------------------------------	
	D3DLIGHT9				m_ObjectLight;
	D3DLIGHT9				m_TerrainLight;
	D3DLIGHT9				m_CharacterLight;

	D3DCOLOR				m_BackgroundColor;
    BOOL					m_Fog;
    BOOL					m_RangeBaseFog;
    DWORD					m_TableFogExpMode;
    DWORD					m_FogMode;
    FLOAT					m_FogNear;
    D3DCOLOR				m_FogColor;	
    FLOAT					m_FogFar;   
    FLOAT					m_FogDensity;
    FLOAT					m_ZValueNear_ViewPlane;
    FLOAT					m_ZValueFar_ViewPlane;

	BOOL					m_ViewLensFlare;
	BOOL					m_bViewMiniHPBar;


	// Environment...
	int						m_Environment;

	BOOL					m_bInterpolationEnvironment;
	FLOAT					m_EnvironmentLerpLevel;

	_XENVIRONMENT_WEATHERDATA m_WeatherData[_XDEF_MAXWEATHERTYPE];

	_XENVIRONMENT_WEATHERDATA m_OldWeatherData;
	_XENVIRONMENT_WEATHERDATA m_NextWeatherData;

	int						m_EnvironmentTime;
	DWORD					m_PrevEnvironmentTime;
	FLOAT					m_SunAngle;	

	_XLensFlare				m_LensFlare;

public:
	_XEnvironmentManager();
	virtual ~_XEnvironmentManager();

	BOOL	Initialize( void );
	void	ReleaseSkyBoxModel( void );

	BOOL	Load(FILE* terrainfile);
	void	SetChangeEnvironmentTime(int time );

	void	SetEnvironmentFog( void );

	void	DrawSkyBox( void );
	void	Draw( BOOL reflectmode = FALSE );

	BOOL	Process(D3DXVECTOR3& vPosition, D3DXVECTOR3& vReflectPosition, FLOAT fElapsedFrameMilisecondTime, DWORD dwLocalSystemTime);
	void	ProcessSkyBox(D3DXVECTOR3& vPosition, D3DXVECTOR3& vReflectPosition, FLOAT fElapsedFrameMilisecondTime);
	BOOL	ProcessEnvironmets(D3DXVECTOR3& vPosition, DWORD dwLocalSystemTime);
	void	ProcessLensFlare(D3DXVECTOR3& vPosition);

	void	EnvironmentColorLerp(D3DCOLOR& In_CurColor, D3DXCOLOR& In_orgColor, D3DXCOLOR& In_nextColor, FLOAT fLeapTime);
	void	EnvironmentColorLerp(D3DCOLORVALUE& In_CurColor, D3DXCOLOR& In_orgColor, D3DXCOLOR& In_nextColor, FLOAT fLeapTime);
	void	EnvironmentColorLerp(D3DCOLOR& In_CurColor, D3DCOLOR& In_orgColor, D3DCOLOR& In_nextColor, FLOAT fLeapTime);
	void	EnvironmentColorLerp(D3DCOLORVALUE& In_CurColor, D3DCOLOR& In_orgColor, D3DCOLOR& In_nextColor, FLOAT fLeapTime);

	void	ChangingEnvironmets();
	BYTE	CheckEnvironmentType(DWORD dwEnvoronmentTime);
	void	SetChangeEnvironmentType(BYTE type);
};

extern _XEnvironmentManager g_EnvironmentManager;

#endif // !defined(AFX_XENVIRONMENTMANAGER_H__F8B5349C_C1A7_4843_80B6_1055F1199638__INCLUDED_)
