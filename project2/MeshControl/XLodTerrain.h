// XLodTerrain.h: interface for the _XLodTerrain class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XLODTERRAIN_H_
#define _XLODTERRAIN_H_

#include "XKernel.h"
#include "XObjectManager.h"
#include "XMeshContainer.h"
#include "X3PCamera.h"
#include "XWater.h"
#include "XPathObject.h"
#include "XParticleList.h"

#include "XGrassManager.h"
#include "XEnvironmentManager.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TILE_VERTS 9	// our tiles are 9x9 vertices

#define _DEF_X3DLODTERRAINFILEINDICATOR			0x0000AE05
#define _DEF_X3DCLIENTLODTERRAINFILEINDICATOR	0x0000AEDE

// Version 처리
#define _DEF_X3DLODTERRAINFILEVERSION			0.88f 

#define _XDEF_DEFAULTWORLDSIZE					1024.0f
#define _XDEF_DEFAULTGRIDSIZE					64.0f
#define _XDEF_DEFAULTTILESIZE					64

#define _XDEF_SPLATBLENDTEXTURESIZE				8
#define _XDEF_MAXSPLATCOUNT						10

enum SIDES
{
	TOP=0,
	LEFT,
	RIGHT,
	BOTTOM,
	TOTAL_SIDES
};

enum LEVEL
{
	LEVEL_0 = 0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	TOTAL_LEVELS
};

extern WORD g_TerrainIndexLUT[ 384 ];
extern WORD	VertexIndexLUT[8][8][2];

#define _XDEF_LODDRAWATTRIB_NONE	0
#define _XDEF_LODDRAWATTRIB_TOP		1
#define _XDEF_LODDRAWATTRIB_LEFT	2
#define _XDEF_LODDRAWATTRIB_RIGHT	4
#define _XDEF_LODDRAWATTRIB_BOTTOM	8

typedef struct 
{
	// Base infomations...
	DWORD		FileIndicator; 
	FLOAT		FileVersion;
	FLOAT		TerrainWidth;
	FLOAT		TerrainHeight;
	FLOAT		GridRealSize;
	DWORD		TileCount;
	
	// texture informations...
	TCHAR		ObjectListFileString[32];	
		
	// Environment informations...
	D3DXCOLOR	BackgroundColor;   
    BOOL		Fog;
    BOOL		RangeBaseFog;
    DWORD		TableFogExpMode;
    DWORD		FogMode;    
    FLOAT		ZValueNear_ViewPlane;
    FLOAT		ZValueFar_ViewPlane;

	FLOAT		LODLevel1;
    FLOAT		LODLevel2;
    FLOAT		LODLevel3;
	
	FLOAT		LODFarPlane;
	
	FLOAT		SkyBoxHieght;
	
	FLOAT		EdgeBoundWidth;
	FLOAT		EdgeBoundHeight;

	FLOAT		Reserved4;
	FLOAT		Reserved5;
	FLOAT		Reserved6;
	FLOAT		Reserved7;
	FLOAT		Reserved8;	
	FLOAT		Reserved9;
} _LODTERRAINFILE_HEADER;

typedef struct _XENVIRONMENTSCRIPTDATA
{
	DWORD			realtimegrass :		1;
	DWORD			grasstype	  :	    3;
	DWORD			noenteringarea:		1;
	DWORD			nopkarea	  :		1;
	DWORD			weather		  :		6;
	
	DWORD			envsound1	  :		6;
	DWORD			envsound2	  :		6;
	DWORD			envsound3	  :		6;	

	DWORD			selected      :		1;
	DWORD			enable        :		1;	

	_XENVIRONMENTSCRIPTDATA()
	{	
		Reset();
	}
	void Reset( void )
	{
		realtimegrass	=  0;
		grasstype		=  0;
		noenteringarea	=  0;
		nopkarea		=  0;
		weather			=  0;
		envsound1		= 63;
		envsound2		= 63;
		envsound3		= 63;		

		selected		=  0;
		enable			=  0;		
	}
		
} _XENVIRONMENTSCRIPTDATA;


// data about each index buffer we create
struct INDEX_BUFFER
{
	LPDIRECT3DINDEXBUFFER9	phIndexBuffer;	
	int						IndexCount;
	int						TriangleCount;
};

// the index buffers needed for each of our detail levels
struct DETAIL_LEVEL
{
	INDEX_BUFFER 	TileBodies[16];
	INDEX_BUFFER	TileConnectors[TOTAL_SIDES][TOTAL_LEVELS];
};

#define _XDEF_USETERRAINDIFFUSECOLOR
#define _XDEF_USETERRAINNORMALVECTOR
#define _XDEF_USETERRAINUV

// our vertex format for xterrain
struct TERRAIN_VERTEX
{
	D3DXVECTOR3  vert;
#ifdef _XDEF_USETERRAINNORMALVECTOR
	D3DXVECTOR3  norm;
#endif
#ifdef _XDEF_USETERRAINDIFFUSECOLOR
	D3DCOLOR	 diffuse;
#endif
#ifdef _XDEF_USETERRAINUV
	float		 tu,tv;
#endif
};

// the FVF code for this terrain

#ifdef _XDEF_USETERRAINUV
	static const DWORD _XTERRAIN_FVF = 
						D3DFVF_XYZ |

						#ifdef _XDEF_USETERRAINNORMALVECTOR
						D3DFVF_NORMAL| 
						#endif

						#ifdef _XDEF_USETERRAINDIFFUSECOLOR
						D3DFVF_DIFFUSE|
						#endif

						D3DFVF_TEX1;
#else

static const DWORD _XTERRAIN_FVF = 
				D3DFVF_XYZ |

#ifdef _XDEF_USETERRAINNORMALVECTOR
				D3DFVF_NORMAL| 
#endif

#ifdef _XDEF_USETERRAINDIFFUSECOLOR
				D3DFVF_DIFFUSE;
#endif

#endif

// Environment renderstyle set
#define _XDEF_ENVRENDERSTYLESET_TERRAIN				0
#define _XDEF_ENVRENDERSTYLESET_SKYBOX				1
#define _XDEF_ENVRENDERSTYLESET_CLOUDSPHERE			2
#define _XDEF_ENVRENDERSTYLESET_WATER				3
#define _XDEF_ENVRENDERSTYLESET_GRASS				4
#define _XDEF_ENVRENDERSTYLESET_SUN					5
#define _XDEF_ENVRENDERSTYLESET_MOON				6
#define _XDEF_ENVRENDERSTYLESET_LENSFLARE			7
#define _XDEF_ENVRENDERSTYLESET_SKYBOXALPHA			8
#define _XDEF_ENVRENDERSTYLESET_TERRAIN_NOSHADOW	9
#define _XDEF_ENVRENDERSTYLESET_TERRAIN_CHARSHADOW	10
#define _XDEF_ENVRENDERSTYLESET_TERRAIN_TWOPASSCHARSHADOW	11


struct AXIS_VERTEX 
{ 
	D3DXVECTOR3 v; 
	D3DCOLOR    color;     
}; 

#define _XDEF_FVF_AXIS_VERTEX		(D3DFVF_XYZ | D3DFVF_DIFFUSE )

class CAxis 
{ 	
	AXIS_VERTEX  m_AxisVertex[30]; 
	DWORD dwAxisFVF; 
	
public: 
	CAxis() 
	{ 
		dwAxisFVF = _XDEF_FVF_AXIS_VERTEX; 
		
		int i, index = 0; 
		float fLine = 500.0f;
		float fArrow = fLine - 5.0f; 
		
		// y 축선 (빨간색) 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, fLine, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, -fLine, 0.0f); 
		// 화살표 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, fLine, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(-1.0f, fArrow, -1.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, fLine, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(1.0f, fArrow, -1.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, fLine, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(-1.0f, fArrow, 1.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, fLine, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(1.0f, fArrow, 1.0f); 
		
		// x 축선 (녹색) 
		m_AxisVertex[index++].v = D3DXVECTOR3(-fLine, 0.0f, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fLine, 0.0f, 0.0f); 
		// 화살표 
		m_AxisVertex[index++].v = D3DXVECTOR3(fLine, 0.0f, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fArrow, -1.0f, -1.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fLine, 0.0f, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fArrow, 1.0f, -1.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fLine, 0.0f, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fArrow, -1.0f, 1.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fLine, 0.0f, 0.0f); 
		m_AxisVertex[index++].v = D3DXVECTOR3(fArrow, 1.0f, 1.0f); 
		
		// z 축선 (파란색) 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, 0.0f, -fLine); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, 0.0f, fLine); 
		// 화살표 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, 0.0f, fLine); 
		m_AxisVertex[index++].v = D3DXVECTOR3(-1.0f, -1.0f, fArrow); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, 0.0f, fLine); 
		m_AxisVertex[index++].v = D3DXVECTOR3(1.0f, -1.0f, fArrow); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, 0.0f, fLine); 
		m_AxisVertex[index++].v = D3DXVECTOR3(-1.0f, 1.0f, fArrow); 
		m_AxisVertex[index++].v = D3DXVECTOR3(0.0f, 0.0f, fLine); 
		m_AxisVertex[index++].v = D3DXVECTOR3(1.0f, 1.0f, fArrow); 
		
		for(i=0; i<10; i++) 
		{ 
			m_AxisVertex[i].color = D3DCOLOR_XRGB(255, 0, 0); 
		} 
		for(i=10; i<20; i++) 
		{ 
			m_AxisVertex[i].color = D3DCOLOR_XRGB(0, 255, 0); 
		} 
		for(i=20; i<30; i++) 
		{ 
			m_AxisVertex[i].color = D3DCOLOR_XRGB(0, 0, 255); 
		}   
	} 
	
	void Render() 
	{ 
		gpDev->SetTexture(0, NULL); 
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( dwAxisFVF ); 
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 15, m_AxisVertex, sizeof(AXIS_VERTEX)); 
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);	  	  
	} 
}; 

class _XTileSplat
{
public:
	//BOOL					m_Use;	
	int						m_LayerIndex;
	BYTE					m_BlendLevelArray[_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE];
	LPDIRECT3DTEXTURE9      m_pBlendTexture;
	LPDIRECT3DINDEXBUFFER9	m_pSplatIndexBuffer;
	DWORD					m_SplatTriangleCount;
	
public:
	_XTileSplat()
	{
		//m_Use				= FALSE;
		m_LayerIndex		= 0;
		m_pBlendTexture		= NULL;
		m_pSplatIndexBuffer = NULL;
		memset( m_BlendLevelArray, 0, sizeof(BYTE) * (_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE) );
		m_SplatTriangleCount = 0;
	}
	~_XTileSplat()
	{
		SAFE_RELEASE( m_pBlendTexture );
		SAFE_RELEASE( m_pSplatIndexBuffer );
	}
	
	BOOL CreateZeroLayerIndexBuffer( /*LPBYTE pBlendMap,*/ FILE* pTerrainFilePtr );
	BOOL ApplyBlendLevelToBlendMap( LPBYTE pBlendMap, FILE* pTerrainFilePtr );
	
};

class _XTileLayer
{
	
public:
	// Layer uv transform
	D3DXMATRIX	m_DiffuseTextureMatrix;
	D3DXVECTOR2 m_Translation;
	D3DXVECTOR2 m_Scale;
	FLOAT		m_rotateangle;
	int			m_DetailTextureIndex;
public:
	_XTileLayer()
	{
			Reset();
	}
	void	Reset( void )
	{
			m_Translation.x = m_Translation.y = 0.0f;
			m_Scale.x = m_Scale.y = 1.0f;
			m_rotateangle = 0.0f;
			m_DetailTextureIndex = -1;
			D3DXMatrixIdentity( &m_DiffuseTextureMatrix );
	}
	
	void	UpdateMatrix( void )
	{		
			D3DXMatrixIdentity( &m_DiffuseTextureMatrix );
			m_DiffuseTextureMatrix._31 = m_Translation.x;
			m_DiffuseTextureMatrix._32 = m_Translation.y;
					
			FLOAT rad = _X_RAD( m_rotateangle );
			m_DiffuseTextureMatrix._11 =  cosf(rad); m_DiffuseTextureMatrix._12 = sinf(rad);
			m_DiffuseTextureMatrix._21 = -sinf(rad); m_DiffuseTextureMatrix._22 = cosf(rad);
			
			m_DiffuseTextureMatrix._11 *= m_Scale.x; m_DiffuseTextureMatrix._21 *= m_Scale.x; m_DiffuseTextureMatrix._31 *= m_Scale.x;
			m_DiffuseTextureMatrix._12 *= m_Scale.y; m_DiffuseTextureMatrix._22 *= m_Scale.y; m_DiffuseTextureMatrix._32 *= m_Scale.y;				
	}
};

struct _XLODTILE;

typedef struct _XTextureManageData
{
	LPDIRECT3DTEXTURE9		pTextureObject;
	DWORD					LastAccessTime;
	_XLODTILE*				pOwner;
	_XTextureManageData()
	{
		pTextureObject = NULL;
		LastAccessTime = 0;
		pOwner		   = NULL;
	}
	
	/*
	~_XTextureManageData()
	{
		if( pTextureObject )
		{
			pTextureObject->Release();
		}		
		LastAccessTime = 0;
	}
	*/
} _XTextureManageData;

class _XTextureGabageCollector
{
private:
	int							texturewidth;
	int							textureheight;
	_D3DFORMAT					format;
	int							stacksize;
	DWORD						throwinteval;
	DWORD						stackincreasefactor;
	_XTextureManageData*		texturearray;

public:
	_XTextureGabageCollector();
	virtual ~_XTextureGabageCollector();

	BOOL Initialize( int width, int height, int reservesize, _D3DFORMAT	textureformat = D3DFMT_A8R8G8B8 );
	void Destroy( void );
	
	BOOL GetNewObject( _XLODTILE* pOwner );
};

// a tile of our terrain
typedef std::vector<_XOBBData*>	svdef_OBBData;

struct _XLODTILE
{
	LEVEL					DetailLevel;// the current detail level to use
	IDirect3DVertexBuffer9* VBuffer;	// a pile of vertices for this tile
	D3DXVECTOR3				VertexBuffer[TILE_VERTS*TILE_VERTS];
	D3DXVECTOR3				Center;		// the center point of the tile
	D3DXVECTOR3				CenterNormal;	// the center point nomal of the tile
	FLOAT					DistanceToCamera;
	_XENVIRONMENTSCRIPTDATA EnvScriptData;
	svdef_OBBData			svObbList;
	BYTE					AlphaLevel;
	_XTileSplat*			SplatData;
	int						UsedSplatLayerCount;
	BOOL					RenderReflectionMap;
	
	int						BodyTile;
	BOOL					CheckedIntersect;

	bool					m_EnableGrassGenerate;
	LPBYTE					m_pGrassAttribArray;

#ifdef _XDWDEBUG
	LPDIRECT3DTEXTURE9      m_pPathFindAreaBlendTexture;
#endif

	LPDIRECT3DTEXTURE9		m_pPreRenderedTextureObject;

	BOOL					RenderSplatImage( void );
};

struct _XPP_SPLATRENDERINGVERTEX
{
	enum { FVF=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 };
	float px, py, pz, pw;
	D3DCOLOR color;
	float tx, ty;
	
	_XPP_SPLATRENDERINGVERTEX(){}
	_XPP_SPLATRENDERINGVERTEX( float _px, float _py, float _pz, float _pw, D3DCOLOR _color, float _tx, float _ty )
	{
		px = _px;
		py = _py;
		pz = _pz;
		pw = _pw;
		color = _color;
		tx = _tx;
		ty = _ty;
	}
};

typedef struct _XLODTILEINDEXFORMAT
{
	DWORD	xcoord		: 12;
	DWORD	zcoord		: 12;
	DWORD	layerindex	:  8;
} _XLODTILEINDEXFORMAT;


#define QUAD_WIDTH	2

class XTileQuadTree  
{
public:
	XTileQuadTree*			m_pQuadTree[QUAD_WIDTH][QUAD_WIDTH];
	
	D3DXVECTOR3				m_vCenter;
	FLOAT					m_fRadius;
	
	BOOL					m_bLeaf;
	
	_XLODTILE*				m_pTile;
	
	int						m_nTileZ;
	int						m_nTileX;
public:
	XTileQuadTree()
	{
		for (int z = 0; z < QUAD_WIDTH; ++ z)
		{
			for (int x = 0; x < QUAD_WIDTH; ++ x)
			{
				m_pQuadTree[z][x] = NULL;
			}
		}
		
		m_vCenter	= D3DXVECTOR3(0, 0, 0);
		m_fRadius	= 0.f;
		
		m_bLeaf		= FALSE;
		m_pTile		= NULL;
		
		m_nTileZ	= 0;
		m_nTileX	= 0;
	}
	virtual ~XTileQuadTree()
	{
	}
};

#include "XFunctionalObject.h"
class _XLodTerrain  
{
public:

	FLOAT					m_Width;
	FLOAT					m_Height;	
	FLOAT					m_GridRealSize;
	FLOAT					m_HalfWorldSize;
	
	FLOAT					m_EdgeBoundingWidth;
	FLOAT					m_EdgeBoundingHeight;

	int						m_TileCount;
	int						m_VertexCount;
	
	_XLODTILE				m_TerrainTile[_XDEF_DEFAULTTILESIZE][_XDEF_DEFAULTTILESIZE];
	int						m_TileStartOffset;	

	LPWORD*					m_ppPathFindAreaArray;
	
	// Tile cache
	int*					m_TerrainTileRenderingCacheLength;
	_XLODTILEINDEXFORMAT**	m_TerrainTileRenderingCache;

	DETAIL_LEVEL			m_DetailLevel[TOTAL_LEVELS]; 
	int						m_UsedLayerCount;
	_XTileLayer				m_LayerInfo[_XDEF_MAXSPLATCOUNT];

	FLOAT					m_LastPickedDist;

	TCHAR					m_ObjectListFilename[_MAX_PATH];
	TCHAR					m_NPCListFilename[_MAX_PATH];
	TCHAR					m_EnvSoundListFilename[_MAX_PATH];
	
	int						m_nCurEnvironmentType;

	_XObjectManager			m_ObjectManager;
	_XFunctionalObjectManager m_FunctionalObjectManager;
	_XQuadTree				m_ObjectQuadTree;

	_XMeshObjectManager		m_MeshObjectManager;

	_XSoundObjectManager				m_SoundObjectManager;

	typedef std::vector<_XEffectObject*>	svdef_XEffectObject;
	svdef_XEffectObject					m_svEffectTemplateList;
	_XParticleGroupInstanceManager		m_EffectObjectManager;


	D3DXMATRIX				m_TerrainMatrix;

	FLOAT					m_LODLevel1;
	FLOAT					m_LODLevel2;
	FLOAT					m_LODLevel3;

	FLOAT					m_DefaultLODLevel1;
	FLOAT					m_DefaultLODLevel2;
	FLOAT					m_DefaultLODLevel3;

	FLOAT					m_LODFarPlane;

	_X3PCamera				m_3PCamera;	
	
	int						m_LastRebuildLevelTileX;
	int						m_LastRebuildLevelTileZ;
	FLOAT					m_LastRebuildLevelYAngle;


	int						m_FacesDrawn;

#ifdef _XDWDEBUG		
	BOOL					m_ViewOBBObject;
	CAxis					axis;
	D3DXMATRIX				axismatrix;
	BOOL					viewaxis;	
	BOOL					viewobject;
	BOOL					viewterrainrenderingtime;
	
	BOOL					CreatePathFindAreaBlendMap( int tilex, int tilez );
	BOOL					ReGeneratePathFindAreaBlendMap( void );
#endif

	// Water controls...
	CWater*					m_pWaterObjectManager;
	FLOAT					m_LastCheckedDistanceWaterpiece;

	CPathObject*			m_PathObjectManager;

	_XOBBData*				m_OBBDataList;
	int						m_AllOBBCount;

	// Terrain tile quad tree
	XTileQuadTree*			m_pQuadTree;
	
	
	BOOL					m_bInvisiblePortalMouseView;//워프영역의 스마트 마우스 포인터를 막는다

public:
	_XLodTerrain();
	virtual ~_XLodTerrain();

	BOOL					Initialize( void );

	BOOL					CreateTerrainVertex( void );
	void					ReleaseTerrainTileInfo( void );
	void					ReleaseTileRenderingCache( void );
	void					ReleaseTerrainIndexTable( void );
	void					Release( void );


	BOOL					LoadTerrain( LPSTR filename );
	BOOL					SaveTerrain( LPSTR filename );
	BOOL					InitialzeSpeedTreeWrapper( void );

	BOOL					LoadCollidePolyList( FILE* fileptr );
	BOOL					LoadOBBList( FILE* fileptr );
	HRESULT					LoadTerrainTexure( LPSTR Filename  = NULL );
	HRESULT					LoadTerrainDetailTexure( LPSTR Filename = NULL );
	BOOL					LoadTerrainTileDetailInfo(FILE *fileptr);
	BOOL					RenderSplatTile( void );
	
	BOOL					LoadEnvScript( FILE* FilePtr );
	BOOL					LoadSound( FILE* terrainfile );
	BOOL					LoadEffect( FILE* terrainfile );
	BOOL					LoadSplatTileImage( FILE* terrainfile );
	void					PreCachingTexture( void );

	BOOL					PreCachingMonsterModel( void );
	BOOL					DisposeCachedMonsterModel( void );

	void					GenerateDetailLevels( void );
		
	void					DrawTerrain( void );
	void					DrawWater( void );
	
	void					RebuildRenderObjectStack( void );
	void					CheckWaterAboveTile( void );
	void					RenderWaterAboveObject( void );

	void					RenderAniObject( FLOAT fElaspedTime );
	void					RenderObject( void );
	
	void					DrawReplectTerrain( void );

	void					RebuildLevel( BOOL check = TRUE );

	BOOL					IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
											   D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 );
	BOOL					IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 );
	BOOL					IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
											   FLOAT* t, FLOAT* u, FLOAT* v );
	
	FLOAT					GetTerrainHeight( D3DXVECTOR3 pos, D3DXVECTOR3* normalvector = NULL );
	FLOAT					GetTerrainAndOBBHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB );
	FLOAT					GetTerrainOBBHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB, FLOAT& terraincollideheight );
	FLOAT					GetTerrainAndRideOBBHeight( D3DXVECTOR3 pos, D3DXVECTOR3* normalvector = NULL );
	FLOAT					GetTerrainAndRideOBBHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB, D3DXVECTOR3* normalvector = NULL );
	FLOAT					GetTerrainAndOBBHeightWithCurrentHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB, D3DXVECTOR3& terrainnormal );
	_XLODTILE*				GetTerrainTile(  D3DXVECTOR3 pos, POINT* tilecoord = NULL );
	_XLODTILE*				GetTerrainTile(  int x, int z );
	void					GetTerrainDecal(int& nVertexCount, int& nIndexCount, D3DXVECTOR3& pos, int nShadowTile, IDirect3DVertexBuffer9** ppVertexBuffer,IDirect3DIndexBuffer9** ppIndexBuffer );
	void					GetTerrainQuadPolygon( int arrayx, int arrayz, D3DXVECTOR3* pVertices );
	//D3DXVECTOR3			GetTerrainNormal(  int x, int z );

	int						GetTerrainTileIndex(D3DXVECTOR3 pos);

	// Object control...
public:	

	_XGrassManager			m_GrassManager;
	void					GenerateGrassObject( void );


	BOOL					m_GenerateRealTimeObject;
	int						m_RealTimeObjectGenerateCount;
	BOOL					m_RealTimeObjectZDepthSorting;

	void					RenderRealtimeObject( void );

	// Environment control...
public:	
	void					ProcessEnvScript(FLOAT fElapsed, const D3DXVECTOR3& vCameraPosition);	
	void					ProcessEnvEffect(int nEnvironmentType);

	// Utility
public:	
	void					ReportUseResource( void );	

	// Path find
public:		
	BOOL					GetPathFindAreaInfo( int arrayx, int arrayz );
	int						GetPathFindAreaNeighborInfo( int arrayx, int arrayz );

	BOOL					CheckDirectPath( int startx, int startz, int& endx, int& endz );
	void					GetPickedPoint( BOOL checkchangestate = TRUE );
	
	BOOL					CheckRayPass( D3DXVECTOR3 startpt, D3DXVECTOR3 endpt, int checksize = 5 );

// Terrain tile quad tree
	void					DeleteData_TileQuad( void );
	void					DeleteData_TileQuad(XTileQuadTree* pQuadTree);
	
	void					Initial_TileQuad( void );
	void					Initial_TileQuad(int nTileZ, int nTileX, int nSize, XTileQuadTree* pQuadTree);
	
	void					RebuildLevel_TileQuad( void );
	void					RebuildLevel_TileQuad(XTileQuadTree* pQuadTree);
	void					RebuildLevel_LODDetailLevel( void );
	
	BOOL					FindSafePoint( D3DXVECTOR3 currentPos );
};

extern BOOL QUAD_TREE;

#endif 


