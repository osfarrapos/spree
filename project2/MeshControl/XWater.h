// KUWater.h: interface for the CWater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUWATER_H__AC2E8B51_7FFE_4263_A109_56B082B9E6E6__INCLUDED_)
#define AFX_KUWATER_H__AC2E8B51_7FFE_4263_A109_56B082B9E6E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D3DX9.h"
#include <list>

using namespace std;

// definition 

// 물 종류 (waterclass)
#define DEF_WATERCLASS_SEA			0		//바다
#define DEF_WATERCLASS_RIVER		1		//강
#define DEF_WATERCLASS_LAKE			2		//호수
#define DEF_WATERCLASS_SWAMP		3		//늪지대

// 물 애니매이션 종류 (animation class)
#define DEF_WATERANI_NONE			0
#define DEF_WATERANI_UVDIRECTION	1
#define DEF_WATERANI_LAPPING		2
#define DEF_WATERANI_UVSIN			3

// 물 1타일당 vertex 수
#define DEF_VERTPERTILE_WATER		4

// 물 종류 갯수 - for texturing
#define DEF_WATERCLASS_COUNT		4

// Lap animation 방향
#define DEF_LAP_CLIMB				1
#define DEF_LAP_DROP				0

// UV animation 방향 
#define DEF_UV_RIGHT				0
#define DEF_UV_LEFT					1
#define DEF_UV_UP					2
#define DEF_UV_DOWN					3
#define DEF_UV_LEFTUP				4
#define DEF_UV_RIGHTUP				5
#define DEF_UV_LEFTDOWN				6
#define DEF_UV_RIGHTDOWN			7

#define _XDEF_DEFAULTSEATEXTURE1				"water1.bmp"
#define _XDEF_DEFAULTSEADETAILTEXTURE				"water1.bmp"
#define _XDEF_DEFAULTRIVERTEXTURE1				"water2.bmp"
#define _XDEF_DEFAULTRIVERDETAILTEXTURE				"water2.bmp"
#define _XDEF_DEFAULTLAKETEXTURE1				"water3.bmp"
#define _XDEF_DEFAULTLAKEDETAILTEXTURE				"water3.bmp"
#define _XDEF_DEFAULTSWAMPTEXTURE1				"water5.bmp"
#define _XDEF_DEFAULTSWAMPDETAILTEXTURE				"water5.bmp"

typedef enum _XWATER_RENDERINGMODE
{
	_XWATER_RM_NORMAL = 0,
	_XWATER_RM_EMBM,
	_XWATER_RM_VERTEXSHADER,
	_XWATER_RM_PIXELSHADER,
};

// structures

#define _XWATERVERTEX_FVF (D3DFVF_XYZ|D3DFVF_TEX1)
// water vertex structure
struct WATER_VERTEX
{
	D3DXVECTOR3			vert;
	float				tu, tv;
};

// water object structure
typedef struct S_WATER
{
	//WATER_VERTEX		vertices[DEF_VERTPERTILE_WATER];
	IDirect3DVertexBuffer9* vertices;
	FLOAT					height;

	D3DXVECTOR3				uvDirection;
	
	FLOAT					uvVelocity;
	FLOAT					lapHeightMax;
	FLOAT					lapVelocity;
	
	DWORD					waterclass		:	3;
	DWORD					animationclass	:	3;
	DWORD					selected		:	1;
	DWORD					enabled			:	1;
	DWORD					usepixelshader	:	1;
	DWORD					infrustum		:	1;
	DWORD					direction		:	3;

	DWORD					reserved		:	32-13;
	S_WATER()
	{
		vertices = NULL;
	}
	~S_WATER()
	{
		SAFE_RELEASE( vertices );
	}
}S_WATER;


// for file 
typedef struct S_WATERPROPERTY
{
	DWORD					waterclass		:	3;
	DWORD					animationclass	:	3;
	DWORD					enabled			:	1;
	DWORD					usepixelshader	:	1;
	DWORD					direction		:	3;

	DWORD					reserved		:	32-12;
}S_WATERPROPERTY;

typedef struct S_WATERHEIGHT
{
	FLOAT					height;
	D3DXVECTOR3				UVDirection;
	FLOAT					uvVelocity;
	FLOAT					lapHeightMax;
	FLOAT					lapVelocity;
}S_WATERHEIGHT;

class _XLodTerrain;

// CWater class - Water Object Manager
class CWater  
{

protected :
	S_WATER							*m_pWaterObjectList;
	
	int								m_iSeaCount;
	int								m_iRiverCount;
	int								m_iLakeCount;
	int								m_iSwampCount;

	int								*m_iSeaIndexArray;
	int								*m_iRiverIndexArray;
	int								*m_iLakeIndexArray;
	int								*m_iSwampIndexArray;

	D3DXMATRIX						m_matWaterMatrix;
	FLOAT							m_fHeightOffset;
	
	int								m_iTerrainTileCount;
	int								m_iWaterTileCount;
	int								m_iTerrainVertexCount;

	int								m_iLapDirection;

	int								m_iSelectedTile;
	list <int>						m_iSelectedTileList;

	FLOAT							m_fWidth;
	FLOAT							m_fHeight;
	
	// diffuse & detail texture index
	int								m_WaterDiffuseTextureIndex[DEF_WATERCLASS_COUNT];
	int								m_WaterDetailTextureIndex[DEF_WATERCLASS_COUNT];
	
public:
	CWater();
	CWater(int iTileCount, int iTerrainVertexCount, FLOAT width, FLOAT height);
	~CWater();

	void							SetSelected(int index, DWORD dwSelected, BOOL bIsSelectOne);

	BOOL							Create(void);
	void							Release(void);
		
	S_WATER*						GetWaterObject( int index )
	{		
									if( index >= 0 && index < m_iWaterTileCount ) return &m_pWaterObjectList[index];					
									else return NULL;
	}
	
	void							SetWaterTextureIndex(int waterclass, LPSTR basefilename, LPSTR detailfilename);

	// set infrustum	
	void							ResetFrustumInfo( void )
	{
									for( int i = 0; i < m_iWaterTileCount; ++i )
									{
										m_pWaterObjectList[i].infrustum = FALSE;
									}
	}
	void							SetFrustum(int index, BOOL infrustum = TRUE)
	{
									if( m_pWaterObjectList[index].enabled )
										m_pWaterObjectList[index].infrustum = infrustum;
	}

	void							SetTextures(int waterclass);
	void							EnableAnimation(void);
	void							DiableAnimation(void);
	void							Draw(void);
	
	BOOL							AllocTerrainVertex(void);
	
	BOOL							Load(FILE *FilePtr, _XLodTerrain* pLodTerrain );

public :
	static	BOOL					CreateWaterIndexBuffer( void );
	static	void					ReleaseWaterIndexBuffer( void );

	// Shader
public :
	
	static _XWATER_RENDERINGMODE	m_WaterRenderingMode;
	static BOOL						m_UseObjectReflection;
	static BOOL						m_UseTerrainReflection;
	static BOOL						m_bUseClipPlanes;
	
	FLOAT							m_WaterReflectionClipPlaneHeight;
	D3DXPLANE						m_ReflectClipPlane;
	
	static BOOL						LoadShader( void );
	static void						ReleaseShader( void );
	static BOOL						InitBumpMap(void);

	void							SetEMBMStates( void );
	void							Draw_UseShader(void);
	void							PreDrawEnvironment(void);

	static bool						InitDeviceObject( void );
	static void						ReleaseDeviceObject( void );

	void							SetWaterReflectionClipPlaneHeight();
	void							SetClipPlane( D3DXMATRIXA16 & matView, D3DXMATRIXA16 & matProj );
	void							SetClipProjectionMatrix( D3DXMATRIXA16 & matView, D3DXMATRIXA16 & matProj );
};

#endif // !defined(AFX_KUWATER_H__AC2E8B51_7FFE_4263_A109_56B082B9E6E6__INCLUDED_)
