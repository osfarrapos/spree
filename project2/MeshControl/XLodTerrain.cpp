// XLodTerrain.cpp: implementation of the _XLodTerrain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <mmsystem.h>
#include <math.h>
#include <direct.h>
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XLodTerrain.h"
#include "XLodTerrainDefine.h"
#include "XQuestScriptManager.h"
#include "XDecalObject.h"
#include "XWindowObjectDefine.h"


#include "XSR_STRINGHEADER.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern FLOAT			g_RTimeGenerateObjectPos[_XDEF_RTIMEOBJECTPOSTABLE_COUNT][_XDEF_MAXREALTIMEOBJECTPERTILE][2];
FLOAT					g_UVTable[9] = { 0.0f, 0.125f, 0.25f, 0.375f, 0.5f, 0.625f, 0.75f, 0.875f, 1.0f };
WORD					g_TerrainIndexLUT[ 384 ];

D3DXVECTOR3				g_DownNormalVector = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
int						g_GrassGenerateSeedTable[64] = 
{
						14, 10, 19, 13, 30, 53, 39, 36, 35, 33,
						52, 62, 51, 54, 50, 59, 56, 55, 34, 17, 
						5,  7,  18, 61, 48, 43, 63, 0,  60, 6,  
						20, 29, 8,  3,  42, 32, 40, 49, 46, 37, 
						38, 45, 47, 31, 41, 44, 22, 21, 24, 12, 
						11, 26, 25, 27, 16, 15, 28, 23, 2,  1,  
						4,  57, 58, 9
};

#define _XDEF_USESPLATCACHEDATA
static const int _XDEF_SPLATIMAGESIZE = 32; // Splat image size (pre calc)

_XDecalObject			g_TerrainDecalObject;
FLOAT					g_TerrainRenderingTime		= 0.0f;

#ifdef _XDWDEBUG	
BOOL	QUAD_TREE = TRUE;
FLOAT	g_LodLevelCheckTime			= 0.0f;
int		g_RebuildLevelCalledCount	= 0;
BOOL	g_UseSplatIndexBuffer		= TRUE;
BOOL	g_ViewSplatLayer[_XDEF_MAXSPLATCOUNT];
#endif

extern void GetFileNameFromPathString( TCHAR *value, char *line );
void CreateLodTerrainIndexLUT( void )
{
	int index = 0;
	
	for (int center_vert=0;center_vert<216;++center_vert)
	{
		g_TerrainIndexLUT[index++] = Level3_Center[center_vert];
	}
	
	for (int side=0;side<TOTAL_SIDES;++side)
	{
		for (int data=0;data<42;++data)
		{
			g_TerrainIndexLUT[index++] = SidesOfLevel3[side][data];
		}
	}
}


_XLodTerrain::_XLodTerrain()
{
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
	m_FacesDrawn				=	0;
#endif

#ifdef _XDWDEBUG		
	m_ViewOBBObject				=	FALSE;

	for(int idx = 0; idx < _XDEF_MAXSPLATCOUNT; idx++ ) g_ViewSplatLayer[idx] = TRUE;
#endif
	
	m_Width						=	0.0f;
	m_Height					=	0.0f;	
	m_GridRealSize				=	0.0f;
	m_HalfWorldSize				=	0.0f;
	m_EdgeBoundingWidth			=   0.0f;
	m_EdgeBoundingHeight		=	0.0f;

	m_TileCount					=	0;
	m_VertexCount				=	0;
	m_TileStartOffset			=	0;

	m_pQuadTree					=	NULL;
	m_ppPathFindAreaArray		=	NULL;
	
	m_TerrainTileRenderingCache =   NULL;
	m_TerrainTileRenderingCacheLength = NULL;
	
	m_UsedLayerCount			=	0;

	memset( m_ObjectListFilename, 0, _MAX_PATH );
	memset( m_NPCListFilename, 0, _MAX_PATH );
	memset( m_EnvSoundListFilename, 0, _MAX_PATH );

	
			
	m_DefaultLODLevel1 = m_LODLevel1 =	150.0f;
	m_DefaultLODLevel2 = m_LODLevel2 =	110.0f;
	m_DefaultLODLevel3 = m_LODLevel3 =	80.0f;
				
	m_LODFarPlane				=	300.0f;

	m_LastRebuildLevelTileX		=	-1;
	m_LastRebuildLevelTileZ		=	-1;
	m_LastRebuildLevelYAngle	=    0.0f;
	
	m_GenerateRealTimeObject		=  TRUE;
	m_RealTimeObjectGenerateCount   =  30;
	m_RealTimeObjectZDepthSorting   =  FALSE;

#ifdef _XDWDEBUG
	D3DXMatrixIdentity( &axismatrix );	
	viewaxis					= FALSE;
	viewobject					= TRUE;
	viewterrainrenderingtime    = FALSE;
#endif
	
	m_pWaterObjectManager		= NULL;
	m_PathObjectManager			= NULL;

	m_OBBDataList				= NULL;
	m_AllOBBCount				= 0;

	m_bInvisiblePortalMouseView = FALSE;
			
	int i,j,k,l;

	for (i=0;i<TOTAL_LEVELS;++i)
	{
		for (j=0;j<16;++j)
		{
			m_DetailLevel[i].TileBodies[j].phIndexBuffer = NULL;
		}
		for (k=0;k<TOTAL_SIDES;++k)
		{
			for (l=0;l<TOTAL_LEVELS;++l)
			{
				m_DetailLevel[i].TileConnectors[k][l].phIndexBuffer = NULL;
			}
		}
	}

	CreateLodTerrainIndexLUT();	
	
	m_EffectObjectManager.SetTemplateManager( &g_WorldEffectGroupManager );
}

_XLodTerrain::~_XLodTerrain()
{
	ReleaseTerrainTileInfo();
	ReleaseTileRenderingCache();
	ReleaseTerrainIndexTable();	
	Release();
}

BOOL _XLodTerrain::Initialize( void )							   
{		
	g_EnvironmentManager.Initialize();
	m_GrassManager.CreateData();	
	D3DXMatrixIdentity(&m_TerrainMatrix);
	return TRUE;
}

void _XLodTerrain::GenerateDetailLevels()
{	
	// Detail Level 0

	if (SUCCEEDED(gpDev->CreateIndexBuffer(6*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
		                                        g_3DObjectMemoryPool, 
												&m_DetailLevel[0].TileBodies[0].phIndexBuffer, NULL)))
	{
		//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[0].TileBodies[0].phIndexBuffer  );

		WORD* pIndex;
		if (SUCCEEDED(m_DetailLevel[0].TileBodies[0].phIndexBuffer->Lock(0,6*2, 
			                           (void**)&pIndex, g_3DObjectLockMethods)))
		{										
			pIndex[0] = BaseTile0[0];
			pIndex[1] = BaseTile0[1];
			pIndex[2] = BaseTile0[2];
			pIndex[3] = BaseTile0[3];
			pIndex[4] = BaseTile0[4];
			pIndex[5] = BaseTile0[5];
			
			m_DetailLevel[0].TileBodies[0].phIndexBuffer->Unlock();

			m_DetailLevel[0].TileBodies[0].IndexCount = 6;
			m_DetailLevel[0].TileBodies[0].TriangleCount = 2;
		}
	}

	// Detail Level 1

	// create each of the 16 tile bodies
	for (int body=0;body<16;++body)
	{
		m_DetailLevel[1].TileBodies[body].phIndexBuffer=0;
		m_DetailLevel[1].TileBodies[body].IndexCount = 0;

		int total_indexes=0;
		if (!(body & (1<<0))) total_indexes += 6;
		if (!(body & (1<<1))) total_indexes += 6;
		if (!(body & (1<<2))) total_indexes += 6;
		if (!(body & (1<<3))) total_indexes += 6;

		if (total_indexes)
		{
			if (SUCCEEDED(gpDev->CreateIndexBuffer(total_indexes*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
												   g_3DObjectMemoryPool,
												   &m_DetailLevel[1].TileBodies[body].phIndexBuffer, NULL)))
			{
				//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[1].TileBodies[body].phIndexBuffer  );

				WORD* pIndex;
				if (SUCCEEDED(m_DetailLevel[1].TileBodies[body].phIndexBuffer->Lock(0,total_indexes*2, 
					          (void**)&pIndex, g_3DObjectLockMethods)))
				{											
					int index=0;

					for (int side=0;side<TOTAL_SIDES;++side)
					{
						if (!(body & (1<<side)))
						{
							for (int data=0;data<6;++data)
							{
								pIndex[index++] = SidesOfLevel1[side][data];
							}
						}
					}				
					m_DetailLevel[1].TileBodies[body].phIndexBuffer->Unlock();
					m_DetailLevel[1].TileBodies[body].IndexCount = total_indexes;
					m_DetailLevel[1].TileBodies[body].TriangleCount = total_indexes/3;				
				}
			}
		}
	}

	// create the tile connectors
	for (int side=0;side<TOTAL_SIDES;++side)
	{				
		m_DetailLevel[1].TileConnectors[side][0].phIndexBuffer = 0;
		if (SUCCEEDED(gpDev->CreateIndexBuffer(3*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
											   g_3DObjectMemoryPool, 
											   &m_DetailLevel[1].TileConnectors[side][0].phIndexBuffer, NULL)))
		{
			//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[1].TileConnectors[side][0].phIndexBuffer);

			WORD* pIndex;
			if (SUCCEEDED(m_DetailLevel[1].TileConnectors[side][0].phIndexBuffer->Lock(0,3*2, 
				                           (void**)&pIndex, g_3DObjectLockMethods)))
			{
				int index=0;
				
				for (int count=0;count<3;++count)
				{
					pIndex[count] = Connect1to0[side][count];
				}
				m_DetailLevel[1].TileConnectors[side][0].phIndexBuffer->Unlock();
				m_DetailLevel[1].TileConnectors[side][0].IndexCount = 3;
				m_DetailLevel[1].TileConnectors[side][0].TriangleCount = 1;
			}
		}
	}

	//
	// Detail Level 2
	//

	// create each of the 16 tile bodies
	for (body=0;body<16;++body)
	{
		m_DetailLevel[2].TileBodies[body].phIndexBuffer=0;
		m_DetailLevel[2].TileBodies[body].IndexCount = 0;

		int total_indexes=24;
		if (!(body & (1<<0))) total_indexes += 18;
		if (!(body & (1<<1))) total_indexes += 18;
		if (!(body & (1<<2))) total_indexes += 18;
		if (!(body & (1<<3))) total_indexes += 18;

		if (total_indexes)
		{
			if (SUCCEEDED(gpDev->CreateIndexBuffer(total_indexes*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
				                                   g_3DObjectMemoryPool, 
												   &m_DetailLevel[2].TileBodies[body].phIndexBuffer, NULL)))
			{
				//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[2].TileBodies[body].phIndexBuffer);

				WORD* pIndex;
				if (SUCCEEDED(m_DetailLevel[2].TileBodies[body].phIndexBuffer->Lock(0,total_indexes*2, 
					                           (void**)&pIndex, g_3DObjectLockMethods)))
				{
					int index=0;
					
					// start by copying the center portion of the tile
					for (int center_vert=0;center_vert<24;++center_vert)
					{
						pIndex[index++] = Level2_Center[center_vert];
					}
					
					for (int side=0;side<TOTAL_SIDES;++side)
					{
						if (!(body & (1<<side)))
						{
							for (int data=0;data<18;++data)
							{
								pIndex[index++] = SidesOfLevel2[side][data];
							}
						}
					}
					m_DetailLevel[2].TileBodies[body].phIndexBuffer->Unlock();
					m_DetailLevel[2].TileBodies[body].IndexCount = total_indexes;
					m_DetailLevel[2].TileBodies[body].TriangleCount = total_indexes/3;
				}
			}
		}
	}

	// create the tile connectors
	for (side=0;side<TOTAL_SIDES;++side)
	{
		// connections to detail level 0		
		m_DetailLevel[2].TileConnectors[side][0].phIndexBuffer = 0;
		if (SUCCEEDED(gpDev->CreateIndexBuffer(9*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
											   g_3DObjectMemoryPool,
			                                   &m_DetailLevel[2].TileConnectors[side][0].phIndexBuffer, NULL)))
		{
			//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[2].TileConnectors[side][0].phIndexBuffer);

			WORD* pIndex;
			if (SUCCEEDED(m_DetailLevel[2].TileConnectors[side][0].phIndexBuffer->Lock(0,9*2, 
										   (void**)&pIndex, g_3DObjectLockMethods)))
			{
				int index=0;
				
				for (int count=0;count<9;++count)
				{
					pIndex[count] = Connect2to0[side][count];
				}
				m_DetailLevel[2].TileConnectors[side][0].phIndexBuffer->Unlock();
				m_DetailLevel[2].TileConnectors[side][0].IndexCount = 9;
				m_DetailLevel[2].TileConnectors[side][0].TriangleCount = 3;
			}
		}
		
		// connections to detail level 1
		m_DetailLevel[2].TileConnectors[side][1].phIndexBuffer = 0;
		if (SUCCEEDED(gpDev->CreateIndexBuffer(12*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
											   g_3DObjectMemoryPool,
					  &m_DetailLevel[2].TileConnectors[side][1].phIndexBuffer, NULL)))
		{
			//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[2].TileConnectors[side][1].phIndexBuffer);

			WORD* pIndex;
			if (SUCCEEDED(m_DetailLevel[2].TileConnectors[side][1].phIndexBuffer->Lock(0,12*2, 
				                           (void**)&pIndex, g_3DObjectLockMethods)))
			{
				int index=0;
				
				for (int count=0;count<12;++count)
				{
					pIndex[count] = Connect2to1[side][count];
				}
				m_DetailLevel[2].TileConnectors[side][1].phIndexBuffer->Unlock();
				m_DetailLevel[2].TileConnectors[side][1].IndexCount = 12;
				m_DetailLevel[2].TileConnectors[side][1].TriangleCount = 4;
			}
		}
	}

	//
	// Detail Level 3
	//

	// create each of the 16 tile bodies
	for (body=0;body<16;++body)
	{
		m_DetailLevel[3].TileBodies[body].phIndexBuffer=0;
		m_DetailLevel[3].TileBodies[body].IndexCount = 0;
		
		int total_indexes=216;
		if (!(body & (1<<0))) total_indexes += 42;
		if (!(body & (1<<1))) total_indexes += 42;
		if (!(body & (1<<2))) total_indexes += 42;
		if (!(body & (1<<3))) total_indexes += 42;
		
		if (total_indexes)
		{
			if (SUCCEEDED(gpDev->CreateIndexBuffer(total_indexes*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
						  g_3DObjectMemoryPool,
						  &m_DetailLevel[3].TileBodies[body].phIndexBuffer, NULL)))
			{
				//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[3].TileBodies[body].phIndexBuffer);

				WORD* pIndex;
				if (SUCCEEDED(m_DetailLevel[3].TileBodies[body].phIndexBuffer->Lock(0,total_indexes*2, 
											   (void**)&pIndex, g_3DObjectLockMethods)))
				{
					int index=0;
					
					// start by copying the center portion of the tile
					for (int center_vert=0;center_vert<216;++center_vert)
					{
						pIndex[index++] = Level3_Center[center_vert];
					}
					
					for (int side=0;side<TOTAL_SIDES;++side)
					{
						if (!(body & (1<<side)))
						{
							for (int data=0;data<42;++data)
							{
								pIndex[index++] = SidesOfLevel3[side][data];
							}
						}
					}
					m_DetailLevel[3].TileBodies[body].phIndexBuffer->Unlock();
					m_DetailLevel[3].TileBodies[body].IndexCount = total_indexes;
					m_DetailLevel[3].TileBodies[body].TriangleCount = total_indexes/3;
				}
			}
		}
	}

	// create the tile connectors
	for (side=0;side<TOTAL_SIDES;++side)
	{
		// connections to detail level 0
		m_DetailLevel[3].TileConnectors[side][0].phIndexBuffer = 0;
		if (SUCCEEDED(gpDev->CreateIndexBuffer(21*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
			                                   g_3DObjectMemoryPool,
											   &m_DetailLevel[3].TileConnectors[side][0].phIndexBuffer, NULL)))
		{
			//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[3].TileConnectors[side][0].phIndexBuffer);

			WORD* pIndex;
			if (SUCCEEDED(m_DetailLevel[3].TileConnectors[side][0].phIndexBuffer->Lock(0,21*2, 
				                           (void**)&pIndex, g_3DObjectLockMethods)))
			{
				int index=0;
				
				for (int count=0;count<21;++count)
				{
					pIndex[count] = Connect3to0[side][count];
				}
				m_DetailLevel[3].TileConnectors[side][0].phIndexBuffer->Unlock();
				m_DetailLevel[3].TileConnectors[side][0].IndexCount = 21;
				m_DetailLevel[3].TileConnectors[side][0].TriangleCount = 7;
			}
		}
		
		// connections to detail level 1
		m_DetailLevel[3].TileConnectors[side][1].phIndexBuffer = 0;
		if (SUCCEEDED(gpDev->CreateIndexBuffer(24*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
			                                   g_3DObjectMemoryPool, 
											   &m_DetailLevel[3].TileConnectors[side][1].phIndexBuffer, NULL)))
		{
			//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[3].TileConnectors[side][1].phIndexBuffer);

			WORD* pIndex;
			if (SUCCEEDED(m_DetailLevel[3].TileConnectors[side][1].phIndexBuffer->Lock(0,24*2, 
				                           (void**)&pIndex, g_3DObjectLockMethods)))
			{
				int index=0;
				
				for (int count=0;count<24;++count)
				{
					pIndex[count] = Connect3to1[side][count];
				}
				m_DetailLevel[3].TileConnectors[side][1].phIndexBuffer->Unlock();
				m_DetailLevel[3].TileConnectors[side][1].IndexCount = 24;
				m_DetailLevel[3].TileConnectors[side][1].TriangleCount = 8;
			}
		}
		
		// connections to detail level 2
		m_DetailLevel[3].TileConnectors[side][2].phIndexBuffer = 0;
		if (SUCCEEDED(gpDev->CreateIndexBuffer(30*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
											    g_3DObjectMemoryPool,
												&m_DetailLevel[3].TileConnectors[side][2].phIndexBuffer, NULL)))
		{
			//_XDWINPRINT( "Terrain index buffer created [ %p ]", m_DetailLevel[3].TileConnectors[side][2].phIndexBuffer);

			WORD* pIndex;
			if (SUCCEEDED(m_DetailLevel[3].TileConnectors[side][2].phIndexBuffer->Lock(0,30*2, 
				                           (void**)&pIndex, g_3DObjectLockMethods)))
			{
				int index=0;
				
				for (int count=0;count<30;++count)
				{
					pIndex[count] = Connect3to2[side][count];
				}
				m_DetailLevel[3].TileConnectors[side][2].phIndexBuffer->Unlock();
				m_DetailLevel[3].TileConnectors[side][2].IndexCount = 30;
				m_DetailLevel[3].TileConnectors[side][2].TriangleCount = 10;
			}
		}		
	}
}

BOOL _XLodTerrain::CreateTerrainVertex( void )
{
	m_Width				=	_XDEF_DEFAULTWORLDSIZE;
	m_Height			=	_XDEF_DEFAULTWORLDSIZE;
	m_TileCount			=	_XDEF_DEFAULTTILESIZE;	
	m_TileStartOffset	=	0;
	m_HalfWorldSize		=	_XDEF_DEFAULTWORLDSIZE / 2.0f;

	m_VertexCount		=	((m_TileCount*(TILE_VERTS-1))+1);
	if( m_VertexCount > 0 )
	{
		m_GridRealSize = ( m_Width / (FLOAT)(m_VertexCount-1) );
	}
	else
	{
		m_GridRealSize = 0.0f;
	}

	if( m_GridRealSize < 0.0f ) m_GridRealSize = -m_GridRealSize;
	
	TERRAIN_VERTEX**	TerrainVerts;
	
	TerrainVerts = new TERRAIN_VERTEX*[m_VertexCount];
	for( int ff = 0; ff < m_VertexCount; ff++ )
	{
		TerrainVerts[ff] = new TERRAIN_VERTEX[m_VertexCount];
	}
	
	int i,j;

	D3DXVECTOR3 RowVector;
	D3DXVECTOR3 CollumnVector;
	D3DXVECTOR3 DefaultNorm = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	
	FLOAT terrainxstartpos = (-m_Width /2.0f);
	FLOAT terrainzstartpos = (-m_Height/2.0f);
	FLOAT xrate;
	FLOAT zrate = terrainzstartpos;
	FLOAT vertcount = (FLOAT)m_VertexCount;
	FLOAT octvertcount = vertcount / 5.0f;

	for (i=0;i<m_VertexCount;i++)
	{			
		xrate = terrainxstartpos;

		for (j=0;j<m_VertexCount;j++)
		{				
			TerrainVerts[i][j].vert.x = xrate;
			xrate+=m_GridRealSize;

			TerrainVerts[i][j].vert.y = 0.0f;

			TerrainVerts[i][j].vert.z = zrate;
		}		

		zrate += m_GridRealSize;		
	}	

	for( i = 0; i < m_TileCount; i++ )
	{
		for( int j = 0; j < m_TileCount; j++ )
		{
			m_TerrainTile[i][j].VBuffer = NULL;
			m_TerrainTile[i][j].SplatData = NULL;
			m_TerrainTile[i][j].m_pPreRenderedTextureObject = NULL;
			m_TerrainTile[i][j].UsedSplatLayerCount = 0;
			m_TerrainTile[i][j].RenderReflectionMap = FALSE;
			m_TerrainTile[i][j].BodyTile = 0;

#ifdef _XDWDEBUG
			m_TerrainTile[i][j].m_pPathFindAreaBlendTexture = NULL;
#endif
		}
	}
	
	m_TerrainTileRenderingCacheLength = NULL;
	m_TerrainTileRenderingCache = NULL;

	// now create the vertex buffers from the global vertex data
	int onetilevertexsize = TILE_VERTS*TILE_VERTS;
		
	int center_vertex = TILE_VERTS>>1;
	for (i=0;i<m_TileCount;++i)
	{
		int verty = i*(TILE_VERTS-1);

		for (j=0;j<m_TileCount;++j)
		{
			int vertx = j*(TILE_VERTS-1);

			// create a vertex buffer for this tile
			m_TerrainTile[i][j].DetailLevel = LEVEL_0;			
			m_TerrainTile[i][j].m_EnableGrassGenerate = FALSE;
			m_TerrainTile[i][j].m_pGrassAttribArray = NULL;

			if (SUCCEEDED(gpDev->CreateVertexBuffer(sizeof(TERRAIN_VERTEX)*onetilevertexsize, g_3DObjectMemoryUsage, 
				          _XTERRAIN_FVF, g_3DObjectMemoryPool, &m_TerrainTile[i][j].VBuffer, NULL)))
			{
				TERRAIN_VERTEX *pData = NULL;
				D3DXVECTOR3    *pSystemVertexbuffer = m_TerrainTile[i][j].VertexBuffer;
				if (SUCCEEDED(m_TerrainTile[i][j].VBuffer->Lock(0,0, (void**)&pData, g_3DObjectLockMethods)))
				{
					for (int y=0;y<TILE_VERTS;++y)
					{
						for (int x=0;x<TILE_VERTS;++x)
						{
							// if this is the center of the tile, store it for distance checking
							if (y==center_vertex && x==center_vertex)
							{
								m_TerrainTile[i][j].Center = TerrainVerts[vertx+x][verty+y].vert;
								m_TerrainTile[i][j].CenterNormal = TerrainVerts[vertx+x][verty+y].norm;
								m_TerrainTile[i][j].DistanceToCamera = 0.0f;
								m_TerrainTile[j][i].AlphaLevel = 0;
							}

#ifdef _XDEF_USETERRAINUV							
							TerrainVerts[vertx+x][verty+y].tu = g_UVTable[x];
							TerrainVerts[vertx+x][verty+y].tv = g_UVTable[y];
#endif
							// copy the vertex to our buffer
							memcpy(pData, &TerrainVerts[vertx+x][verty+y], sizeof(TERRAIN_VERTEX));

							// copy system vertex buffer...
							pSystemVertexbuffer->x = pData->vert.x;
							pSystemVertexbuffer->z = pData->vert.z;

							++pData;
							++pSystemVertexbuffer;
						}
					}

					m_TerrainTile[i][j].VBuffer->Unlock();
				}
			}
		}
	}	
	
	if( TerrainVerts )
	{
		for( i = 0; i < m_VertexCount; i++ )
		{
			SAFE_DELETE_ARRAY( TerrainVerts[i] );
		}
		SAFE_DELETE_ARRAY( TerrainVerts );
	}	

	return TRUE;
}

void _XLodTerrain::ReleaseTerrainIndexTable( void )
{
	int i,j,k,l;
	
	for (i=0;i<TOTAL_LEVELS;++i)
	{
		for (j=0;j<16;++j)
		{
			SAFE_RELEASE(m_DetailLevel[i].TileBodies[j].phIndexBuffer);
		}
		for (k=0;k<TOTAL_SIDES;++k)
		{
			for (l=0;l<TOTAL_LEVELS;++l)
			{
				SAFE_RELEASE(m_DetailLevel[i].TileConnectors[k][l].phIndexBuffer);
			}
		}
	}
}

void _XLodTerrain::Release( void )
{	
	int i, j;
	for (i=0;i<_XDEF_DEFAULTTILESIZE;++i)
	{
		for (j=0;j<_XDEF_DEFAULTTILESIZE;++j)
		{
			SAFE_RELEASE(m_TerrainTile[i][j].VBuffer);
		}
	}
	m_GrassManager.DeleteData();
}	

void _XLodTerrain::ReleaseTerrainTileInfo( void )
{
	SAFE_DELETE( m_pWaterObjectManager );
	SAFE_DELETE( m_PathObjectManager );
			
	// release the tiles
	int i, j;
	for (i=0;i<_XDEF_DEFAULTTILESIZE;++i)
	{
		for (j=0;j<_XDEF_DEFAULTTILESIZE;++j)
		{
			m_TerrainTile[i][j].svObbList.clear();
			SAFE_DELETE_ARRAY( m_TerrainTile[i][j].SplatData );
			SAFE_RELEASE( m_TerrainTile[i][j].m_pPreRenderedTextureObject );
			SAFE_DELETE_ARRAY( m_TerrainTile[i][j].m_pGrassAttribArray );
			m_TerrainTile[i][j].m_EnableGrassGenerate = FALSE;

#ifdef _XDWDEBUG
			SAFE_RELEASE(m_TerrainTile[i][j].m_pPathFindAreaBlendTexture);
#endif
		}
	}

	if( m_ppPathFindAreaArray )
	{
		for (int i=0;i<(m_TileCount<<4);++i)
		{
			SAFE_DELETE_ARRAY(m_ppPathFindAreaArray[i]);
		}
		
		SAFE_DELETE_ARRAY( m_ppPathFindAreaArray );
	} 

	if (!m_svEffectTemplateList.empty())
	{
		std::for_each(m_svEffectTemplateList.begin(), m_svEffectTemplateList.end(), DELETEOBJECT());
		m_svEffectTemplateList.clear();
	}

	m_SoundObjectManager.DisposeList();
	m_EffectObjectManager.disposeList();

	m_ObjectManager.DisposeList();
	m_MeshObjectManager.disposeList();
	SAFE_DELETE_ARRAY( m_OBBDataList );
	m_AllOBBCount = 0;
	
	g_AreaManager.Release();

	DeleteData_TileQuad();
}

void _XLodTerrain::ReleaseTileRenderingCache( void )
{
	if( m_TerrainTileRenderingCache )
	{
		for( int layer = 0 ; layer < m_UsedLayerCount+3; layer++ )
		{
			SAFE_DELETE_ARRAY( m_TerrainTileRenderingCache[layer] );
		}
		SAFE_DELETE_ARRAY( m_TerrainTileRenderingCache );
	}
	
	SAFE_DELETE_ARRAY( m_TerrainTileRenderingCacheLength );
}


BOOL _XLodTerrain::LoadTerrain( LPSTR filename )
{	
	if( !filename || !filename[0] ) return FALSE;

	XProc_Loading* pLoadingProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_loading;

	// Release previous data...	
	ReleaseTerrainTileInfo();
	ReleaseTileRenderingCache();

	_XStringItem	filenamestring;
	filenamestring.Set_String( filename );
	filenamestring.deleteExtention();

	if( !m_MeshObjectManager.LoadAniMeshList( ) )
	{	
		return FALSE;
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT(ID_STRING_NEW_2708) ); //"지형 데이터를 초기화 합니다."
	pLoadingProcess->UpdateProgress(7);

	_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);

	_LODTERRAINFILE_HEADER	header;	
	memset( &header, 0, sizeof( _LODTERRAINFILE_HEADER ) );
	
	FILE* terrainfile = NULL;

	_XPackageArchive terrainpackagearchive;
	_XPackageArchive terrainscriptpackagearchive;

	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	BOOL bTerrainLoadFromFile = bLoadFromFile;

	TCHAR	terrainpackname[64];
	memset( terrainpackname, 0, sizeof(TCHAR)*64 );
	
#ifdef _XTESTSERVER		
	strcpy( terrainpackname, _T("tsVR_ENVIRONMENT.XP") );
#else
	strcpy( terrainpackname, _T("VR_ENVIRONMENT.XP") );
#endif

	if( bLoadFromFile )
	{
		if( !(terrainfile = fopen( filename, "rb" ) ) )
		{
			if( !terrainpackagearchive.OpenPackage( terrainpackname ) )
			{
				return FALSE;
			}
			if( !terrainscriptpackagearchive.OpenPackage( terrainpackname ) )
			{
				return FALSE;
			}

			terrainfile = terrainpackagearchive.GetPackedFile( filename );
			
			if( !terrainfile )
			{
				_XFatalError( "LoadTerrain([%s]) file open", filename );
				terrainpackagearchive.FinalizePackage();
				terrainscriptpackagearchive.FinalizePackage();
				return FALSE;
			}

			bTerrainLoadFromFile = FALSE;
		}
	}
	else
	{
		if( !terrainpackagearchive.OpenPackage( terrainpackname ) )
		{
			return FALSE;
		}
		if( !terrainscriptpackagearchive.OpenPackage( terrainpackname ) )
		{
			return FALSE;
		}

		terrainfile = terrainpackagearchive.GetPackedFile( filename );

		if( !terrainfile )
		{
			terrainpackagearchive.FinalizePackage();
			terrainscriptpackagearchive.FinalizePackage();
			return FALSE;
		}
	}

	// Load header...
	fread( &header, sizeof(_LODTERRAINFILE_HEADER ), 1, terrainfile );

	if( header.FileIndicator != _DEF_X3DCLIENTLODTERRAINFILEINDICATOR )
	{
		_XFatalError( "LoadTerrain([%s] Not supported format", filename );
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}

	BOOL bOldVersion = FALSE;

	if( header.FileVersion != _DEF_X3DLODTERRAINFILEVERSION )
	{
		if( header.FileVersion == 0.87f ) // zone list 정보만 변환 : 차후 버전에서는 삭제, 차후(0.89)버전업시 모든 XTV convert & update 하여야 함.
		{
			bOldVersion = TRUE;
		}
		else
		{
			_XFatalError( "LoadTerrain([%s] Not supported this version [%f]", filename, header.FileVersion );
			if( bTerrainLoadFromFile ) fclose( terrainfile );
			terrainpackagearchive.FinalizePackage();
			terrainscriptpackagearchive.FinalizePackage();
			return FALSE;
		}
	}

	m_Width			= header.TerrainWidth;
	m_Height		= header.TerrainHeight;
	m_TileCount		= header.TileCount;	
	m_GridRealSize	= header.GridRealSize;
	
	m_VertexCount	= ((m_TileCount*(TILE_VERTS-1))+1);
	
	m_TileStartOffset = (_XDEF_DEFAULTTILESIZE>>1) - (m_TileCount>>1);

	m_EdgeBoundingWidth  = 0.0f;//m_Width  - header.EdgeBoundWidth;
	m_EdgeBoundingHeight = 0.0f;//m_Height - header.EdgeBoundHeight;

	m_HalfWorldSize = m_Width / 2.0f;
			
	FLOAT*	  pHeightFieldData = NULL;
	D3DCOLOR* pDiffuseData = NULL;

	pHeightFieldData = new FLOAT[m_VertexCount*m_VertexCount];	
	if( !pHeightFieldData )
	{
		_XFatalError( "LoadTerrain([%s]) Allocation heightfield temporary buffer", filename );
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;	
	}	

	pDiffuseData = new D3DCOLOR[m_VertexCount*m_VertexCount];
	if( !pDiffuseData)
	{
		_XFatalError( "LoadTerrain([%s]) Allocation vertex diffuse temporary buffer", filename );
		delete[] pHeightFieldData;
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;	
	}	

	if( fread( pHeightFieldData, sizeof(FLOAT) * m_VertexCount*m_VertexCount, 1, terrainfile ) < 1 )
	{
		_XFatalError( "LoadTerrain([%s]) Read vertex height data", filename );
		delete[] pHeightFieldData;
		delete[] pDiffuseData;
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}

	if( fread( pDiffuseData, sizeof(D3DCOLOR) * m_VertexCount*m_VertexCount, 1, terrainfile ) < 1 )
	{
		_XFatalError( "LoadTerrain([%s]) Read vertex diffuse data", filename );
		delete[] pHeightFieldData;
		delete[] pDiffuseData;
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}
	
	pLoadingProcess->SetMessage( _XGETINTERFACETEXT(ID_STRING_NEW_2709) );//"지형 정보를 로딩합니다."
	pLoadingProcess->UpdateProgress(10);

	m_pWaterObjectManager = new CWater(m_TileCount, m_VertexCount, m_Width, m_Height);
	
	if( m_pWaterObjectManager )
	{
		m_pWaterObjectManager->Create();
	}
	else
	{
		_XFatalError( "Can't create water manager" );
		delete[] pHeightFieldData;
		delete[] pDiffuseData;
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT(ID_STRING_NEW_2710) ); //"지형을 생성합니다."
	pLoadingProcess->UpdateProgress(13);

	int i,j;

	FLOAT* pHeightFieldLineData = pHeightFieldData;
	D3DCOLOR* pDiffuseLineData = pDiffuseData;

	int center_vertex = TILE_VERTS>>1;
	D3DXVECTOR3 RowVector		= D3DXVECTOR3( 0.0f, 0.0f, -m_GridRealSize );
	D3DXVECTOR3 CollumnVector	= D3DXVECTOR3( -m_GridRealSize, 0.0f,0.0f  );
	D3DXVECTOR3 TempNorm;

	int baseIndex = 0;

	for (i=0;i<m_TileCount;++i)
	{
		int verty = i*(TILE_VERTS-1);

		for (j=0;j<m_TileCount;++j)
		{
			int vertx = j*(TILE_VERTS-1);
			
			_XLODTILE* pCurrentTile = &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j];

			// create a vertex buffer for this tile				
			TERRAIN_VERTEX *pData = NULL;
			D3DXVECTOR3* pSystemVertexbuffer = pCurrentTile->VertexBuffer;
			if (SUCCEEDED(pCurrentTile->VBuffer->Lock(0,0, (void**)&pData, g_3DObjectLockMethods)))
			{

				for (int y=0;y<TILE_VERTS;++y)
				{
					for (int x=0;x<TILE_VERTS;++x)
					{
						// if this is the center of the tile, store it for distance checking

						int index = ((vertx+x)*m_VertexCount)+(verty+y);

						// copy the vertex to our buffer & system vertex buffer...
						pSystemVertexbuffer->y = pData->vert.y = pHeightFieldLineData[index];
						pData->diffuse = pDiffuseLineData[index];

#ifdef _XDEF_USETERRAINNORMALVECTOR
						// calc the normal from the last pixel and row
						if (j || x)
						{
							RowVector.x	= 0.0f;							
							RowVector.y = pHeightFieldLineData[index-m_VertexCount] - pHeightFieldLineData[index];
							RowVector.z = -m_GridRealSize;
						}
						else
						{
							RowVector.x =  0.0f;
							RowVector.y =  0.0f;
							RowVector.z =  -m_GridRealSize;
						}

						if (i || y )
						{
							CollumnVector.x	= -m_GridRealSize;
							CollumnVector.y = pHeightFieldLineData[index-1] - pHeightFieldLineData[index];
							CollumnVector.z = 0.0f;
						}
						else
						{
							CollumnVector.x =  -m_GridRealSize;
							CollumnVector.y =  0.0f;
							CollumnVector.z =  0.0f;
						}

						D3DXVec3Cross(&TempNorm, &RowVector, &CollumnVector);
						D3DXVec3Normalize(&pData->norm, &TempNorm);

						if (y==center_vertex && x==center_vertex)
						{
							pCurrentTile->Center.y = pHeightFieldLineData[index];
							pCurrentTile->CenterNormal = TempNorm;
						}
#endif						
						++pData;						
						++pSystemVertexbuffer;
					}
				}				
				
				pCurrentTile->VBuffer->Unlock();
			}

			// Load grass generate informations	

			if( fread( &pCurrentTile->m_EnableGrassGenerate, sizeof(bool), 1, terrainfile ) < 1 )
			{
				_XFatalError("ERROR : LoadTerrain(%s) Load grass generate flag", filename);
				fclose(terrainfile);
				return FALSE;	
			}
			
			if( pCurrentTile->m_EnableGrassGenerate )
			{
				pCurrentTile->m_pGrassAttribArray = new BYTE[64];
				
				if( fread( pCurrentTile->m_pGrassAttribArray, sizeof(BYTE) * 64, 1, terrainfile ) < 1 )
				{
					_XFatalError("ERROR : LoadTerrain(%s) Load grass generate informations", filename);
					fclose(terrainfile);
					return FALSE;	
				}
			}
			else
			{
				pCurrentTile->m_pGrassAttribArray = NULL;
			}
		}

		baseIndex += m_TileCount;
	}	

	delete[] pDiffuseData;
	delete[] pHeightFieldData;

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT(ID_STRING_NEW_2711) ); //길찾기 정보를 생성합니다.
	pLoadingProcess->UpdateProgress(20);

	m_ppPathFindAreaArray = new LPWORD[ (m_TileCount << 4) ];
	
	// Alloc & Read path finding informations	
	for( i = 0; i < (m_TileCount << 4); i++ )
	{
		m_ppPathFindAreaArray[i] = new WORD[m_TileCount];

		if( fread( m_ppPathFindAreaArray[i], sizeof(WORD) * m_TileCount, 1, terrainfile ) < 1 )
		{
			_XFatalError("LoadTerrain(%s) Read path finding informations", filename);
			if( bTerrainLoadFromFile ) fclose( terrainfile );
			terrainpackagearchive.FinalizePackage();
			terrainscriptpackagearchive.FinalizePackage();
			return FALSE;	
		}
	}	
	
	pLoadingProcess->SetMessage( _XGETINTERFACETEXT(ID_STRING_NEW_2712) ); //"지형 세부 정보를 구성합니다."
	pLoadingProcess->UpdateProgress(20);

	strcpy( m_ObjectListFilename, header.ObjectListFileString );
	_EncodeData( m_ObjectListFilename, _ENCODEKEY );
	
	// Set environment data...
	g_EnvironmentManager.m_BackgroundColor		= header.BackgroundColor;   
    g_EnvironmentManager.m_Fog					= header.Fog;
	g_EnvironmentManager.m_Fog					= TRUE; // Force setting
    g_EnvironmentManager.m_RangeBaseFog			= header.RangeBaseFog;
    g_EnvironmentManager.m_TableFogExpMode		= header.TableFogExpMode;
    g_EnvironmentManager.m_FogMode				= header.FogMode;

    g_EnvironmentManager.m_ZValueNear_ViewPlane	= header.ZValueNear_ViewPlane;
	//m_ZValueNear_ViewPlane	= 1.0f;
	g_EnvironmentManager.m_ZValueFar_ViewPlane	= header.ZValueFar_ViewPlane;
	m_DefaultLODLevel1		= header.LODLevel1;
	m_DefaultLODLevel2		= header.LODLevel2;
	m_DefaultLODLevel3		= header.LODLevel3;
	m_LODFarPlane			= header.LODFarPlane;
		
	g_EnvironmentManager.m_SkyBoxHeightOffset		=	header.SkyBoxHieght;
	g_EnvironmentManager.m_EnvironmentLerpLevel		=	0.0f;
	m_LastRebuildLevelTileX		=	-1;
	m_LastRebuildLevelTileZ		=	-1;

#ifdef _XDWDEBUG	
	if( !ReGeneratePathFindAreaBlendMap() ) 
	{
		fclose(terrainfile);
		return FALSE;	
	}
#endif

	if(!LoadTerrainTileDetailInfo( terrainfile) )
	{
		fclose(terrainfile);
		return FALSE;
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2713 ) );
	pLoadingProcess->UpdateProgress(24);

	if( !m_MeshObjectManager.LoadMeshList( terrainfile ) )
	{	
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2714 ) ); //"건물 정보를 생성합니다."
	pLoadingProcess->UpdateProgress(28);
			
	// Load object list
	if( !this->m_ObjectManager.LoadObjectList( terrainfile, filename ) )
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}
	
	// Load Functional object list
	if( !this->m_FunctionalObjectManager.LoadFunctionalObjectList( terrainfile, filename ) )
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}
	
	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2715 ) ); //"나무 정보를 생성합니다."
	pLoadingProcess->UpdateProgress(32);

	if( !InitialzeSpeedTreeWrapper() )
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}
	
	if( !g_XST_Wrapper.LoadSpeedTreeInstanceData( terrainfile ) )
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2716 ) ); //NPC를 생성합니다.
	pLoadingProcess->UpdateProgress(36);

	TCHAR extscriptfilename[_MAX_PATH];
	memset( extscriptfilename, 0, sizeof(TCHAR) * _MAX_PATH );
	strcpy( extscriptfilename, filenamestring.Get_String() );
	strcat( extscriptfilename, _T("XNS") );
	
	FILE* pFilePointer = NULL;
	BOOL  bScriptFromPackage = FALSE;

	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);
		
		pFilePointer = fopen( extscriptfilename, "rb" );

		if( !pFilePointer )
		{
			pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
			if( !pFilePointer )
			{
				_XFatalError( "Can't open NPC Script file [%s]", extscriptfilename );
				if( bTerrainLoadFromFile ) fclose( terrainfile );
				terrainpackagearchive.FinalizePackage();
				terrainscriptpackagearchive.FinalizePackage();
				return FALSE;
			}
			bScriptFromPackage = TRUE;
		}
	}
	else
	{
		pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
		if( !pFilePointer )
		{
			if( bTerrainLoadFromFile ) fclose( terrainfile );
			terrainpackagearchive.FinalizePackage();
			terrainscriptpackagearchive.FinalizePackage();
			return FALSE;
		}
		bScriptFromPackage = TRUE;
	}

	// Load NPC object list & script
	if( !g_Npclist.LoadNPCObjectList( pFilePointer, filename ) )
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		if( !bScriptFromPackage ) fclose( pFilePointer );
		return FALSE;
	}

	if( !bScriptFromPackage )
	{
		fclose( pFilePointer );
	}
	
	if( !LoadCollidePolyList( terrainfile ) )
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;	
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2717 ) ); //"영역 정보 처리중입니다."
	pLoadingProcess->UpdateProgress(38);

	if( !LoadOBBList( terrainfile ) )
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		return FALSE;	
	}

	memset( extscriptfilename, 0, sizeof(TCHAR) * _MAX_PATH );
	strcpy( extscriptfilename, filenamestring.Get_String() );
	strcat( extscriptfilename, _T("XZI") );
	

	pFilePointer = NULL;
	bScriptFromPackage = FALSE;
	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);

		pFilePointer = fopen( extscriptfilename, "rb" );
		if( !pFilePointer )
		{
			pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
			if( !pFilePointer )
			{
				_XFatalError( "Can't open area informations file [%s]", extscriptfilename );
				if( bTerrainLoadFromFile ) fclose( terrainfile );
				terrainpackagearchive.FinalizePackage();
				terrainscriptpackagearchive.FinalizePackage();
				return FALSE;
			}
			bScriptFromPackage = TRUE;
		}
	}
	else
	{
		pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
		if( !pFilePointer )
		{
			if( bTerrainLoadFromFile ) fclose( terrainfile );
			terrainpackagearchive.FinalizePackage();
			terrainscriptpackagearchive.FinalizePackage();
			return FALSE;
		}
		bScriptFromPackage = TRUE;
	}

	
	if(!g_AreaManager.LoadAreaList(pFilePointer))
	{
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		if( !bScriptFromPackage ) fclose( pFilePointer );
		return FALSE;
	}

	for( i = 0; i < g_GuardMobPrecachingInfo[g_CurrentZoneInfoIndex-1].count; i++ )
	{
		g_MobAttribTable[g_GuardMobPrecachingInfo[g_CurrentZoneInfoIndex-1].spawnlist[i]].usecurrentzone = TRUE;
	}
		
	if( !bScriptFromPackage )
	{
		fclose( pFilePointer );
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2718 ) ); //"수면 정보 처리중입니다."
	pLoadingProcess->UpdateProgress(40);

	if(m_pWaterObjectManager)
	{
		if(!m_pWaterObjectManager->Load(terrainfile, this))
		{
			m_pWaterObjectManager->Release();
			delete m_pWaterObjectManager;

			_XFatalError("LoadTerrain(%s) Load Water object", filename);
			fclose(terrainfile);
			return FALSE;
		}
	}

	m_ObjectManager.RebuildAboveWaterObject();

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2719 ) ); //"환경을 구성합니다."
	pLoadingProcess->UpdateProgress(42);

	// Load environment script data
	if( !LoadEnvScript( terrainfile ) )
	{		
		fclose(terrainfile);
		return FALSE;
	}

#ifdef _XUSEFMOD	
	g_FMODWrapper.FL_StopAllSound();
	g_FMODWrapper.FL_ReleaseFlexibleList();
#else
	// Release env sound list...
	for( i = 0; i < _XDEF_MAXENVIRONMENTSOUND; i++ )
	{	
		if( g_pEnvironmentSound[i].m_pSound3D )
		{
			g_pEnvironmentSound[i].m_pSound3D->Destroy();
			g_pEnvironmentSound[i].m_pSound3D = NULL;
		}
	}
#endif

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2720 ) );//"환경 사운드 초기화중입니다."
	pLoadingProcess->UpdateProgress(44);
	// Load environment sound list...
	memset( m_EnvSoundListFilename, 0, _MAX_PATH );
	if( fread( m_EnvSoundListFilename, 32, 1, terrainfile ) < 1 )
	{
		_XFatalError("LoadTerrain(%s) Load env sound list file name", filename);
		fclose(terrainfile);
		return FALSE;
	}	
	else
	{
		_EncodeData( m_EnvSoundListFilename, _ENCODEKEY );

		TCHAR envsoundname[ _XDEF_MAXENVIRONMENTSOUND ][128];		

		if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

		FILE* pScripterFile = g_ScriptArchive.GetPackedFile( m_EnvSoundListFilename );
		
		if( !pScripterFile )
		{
			_XFatalError("LoadTerrain(%s) Open env sound list file [%s]", filename, m_EnvSoundListFilename );
			g_ScriptArchive.ClosePackage();
			fclose(terrainfile);
			return FALSE;
		}
		
		int envindex = 0;
		do 
		{
			if( envindex >= _XDEF_MAXENVIRONMENTSOUND ) break;

			memset( envsoundname[envindex], 0, sizeof(TCHAR) * 128 );
			if( feof(pScripterFile) )
			{
				break;
			}
			
			if( !fgets( envsoundname[envindex], 127, pScripterFile ) )
			{
				break;
			}
			else
			{
				if( strncmp( envsoundname[envindex], "_XM_EOS", 7 ) == 0 )
				{
					memset( envsoundname[envindex], 0, sizeof(TCHAR) * 128 );
					break;			
				}		

				if( strlen( envsoundname[envindex] ) == 0 || 
					envsoundname[envindex][0] == _T(';')  ||
					envsoundname[envindex][0] == _T('\n'))
				{
					continue;
				}			
				else
				{
					envsoundname[envindex][ strlen(envsoundname[envindex])-2 ] = _T('\0');
					envindex++;
				}
			}
			
		} while( !feof(pScripterFile) );

		g_ScriptArchive.ClosePackage();
			
		// Load effect sound ------------------------------------------------------
#ifdef _XUSEFMOD	
		static const FLOAT _3dmindistance = 8.2f;//0.0f;
		static const FLOAT _3dmaxdistance = 10000.0f;//50.0f;
		
		for( i = 0; i < envindex; i++ )
		{
			int findindex = g_FMODWrapper.FL_InsertSound( envsoundname[i], FSOUND_HW3D, _3dmindistance, _3dmaxdistance );
			if( findindex > -1 )
			{
				g_pEnvironmentSound[i].soundindex = findindex;

#ifdef _XDWDEBUG
				strcpy( g_pEnvironmentSound[i].envsoundname, envsoundname[i] );
#endif
			}
			else
			{
				_XFatalError("Couldn't find 3d env sound effect. [%s]", envsoundname[i] );
				fclose(terrainfile);
				return FALSE;
			}
		}
#else				
		_XSetDefaultPath(_XDEF_DEFAULTPATH);
		
		if( !g_EnvSoundEffectArchive.Open( _XDEF_ENVSOUNDEFFECTARCHIVENAME ) )
		{
			_XFatalError("LoadTerrain(%s) Open Environment sound package [%s]", filename, _XDEF_ENVSOUNDEFFECTARCHIVENAME );
			fclose(terrainfile);
			return FALSE;
		}

		Sound3DInit init3dsound;
		init3dsound.m_bLooping = false;
		init3dsound.m_bStreaming = false;
		init3dsound.m_bMusic = false;	
		init3dsound.m_pPackage = &g_EnvSoundEffectArchive;
				
		for( i = 0; i < envindex; i++ )
		{			
			int findindex = g_EnvSoundEffectArchive.FindResource( envsoundname[i] );

			if( findindex > -1 )
			{
				init3dsound.m_sFileName = envsoundname[i];
				strcpy(g_pEnvironmentSound[i].envsoundname, envsoundname[i]);

				if(!AudioMgr()->CreateSound3D(g_pEnvironmentSound[i].m_pSound3D)) return false;
				if(!g_pEnvironmentSound[i].m_pSound3D->Init(init3dsound))
				{
					_XFatalError("Error initializing 3d audio file");
					return false;
				}		
				g_pEnvironmentSound[i].m_pSound3D->SetVolume( 1.0f );
			}
			else
			{
				_XFatalError("Couldn't find 3d env sound effect. [%s]", envsoundname[i] );
				g_EnvSoundEffectArchive.Close();
				fclose(terrainfile);
				return FALSE;
			}
		}

		g_MaxEnvSoundEffectCount = envindex;
		
		g_EnvSoundEffectArchive.Close( FALSE );
		_XRestorePrevPath();
#endif
	}

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2721 ) ); //"환경을 최적화 중입니다."
	pLoadingProcess->UpdateProgress(48);

	if (g_EnvironmentManager.Load(terrainfile) == FALSE)
	{
		_XFatalError("LoadTerrain(%s) Load env weather data", filename);
		fclose(terrainfile);
		return FALSE;		
	}

	/*
	switch( g_XGQProperty.sightrange ) 
	{	
	case _XGQLEVEL_MAX		:
		m_LODLevel1				= m_DefaultLODLevel1 + 50;	
		m_LODLevel2				= m_DefaultLODLevel2 + 50;	
		m_LODLevel3				= m_DefaultLODLevel3 + 50;
		break;
	case _XGQLEVEL_HIGH		:		
		m_LODLevel1				= m_DefaultLODLevel1;	
		m_LODLevel2				= m_DefaultLODLevel2;	
		m_LODLevel3				= m_DefaultLODLevel3;
		break;
	default					:
	case _XGQLEVEL_MIDDLE	: 		
		m_LODLevel1				= m_DefaultLODLevel1 - 25;	
		m_LODLevel2				= m_DefaultLODLevel2 - 25;	
		m_LODLevel3				= m_DefaultLODLevel3 - 25;
		
		break;
	case _XGQLEVEL_LOW		: 				
		m_LODLevel1				= m_DefaultLODLevel1 - 50;	
		m_LODLevel2				= m_DefaultLODLevel2 - 50;	
		m_LODLevel3				= m_DefaultLODLevel3 - 50;
		break;	
	} 	

	g_XST_Wrapper.m_RenderLimitDistance = m_LODLevel1;
	*/


	int terrainquality = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_TERRAINLODLEVEL, 70, TRUE );

	FLOAT rate = terrainquality / 100.0f;
	
	int modifyfactor = 0;
	if( rate > 0.75f )
	{
		modifyfactor = 50;
	}
	else if( rate > 0.5f )
	{
		modifyfactor = 0;				
	}
	else if( rate > 0.25f )
	{
		modifyfactor = -25;				
	}
	else
	{
		modifyfactor = -50;				
	}
	
	m_LODLevel1 = m_DefaultLODLevel1 + modifyfactor;
	m_LODLevel2 = m_DefaultLODLevel2 + modifyfactor;
	m_LODLevel3 = m_DefaultLODLevel3 + modifyfactor;
	
	g_XST_Wrapper.m_RenderLimitDistance = g_LodTerrain.m_LODLevel1;
	

	// path object load
	m_PathObjectManager = new CPathObject;
	if(!m_PathObjectManager->LoadPathObject(terrainfile))
	{
		_XFatalError("LoadTerrain(%s) Load Path Object", filename);
		fclose(terrainfile);
		return FALSE;
	}
	
	memset( extscriptfilename, 0, sizeof(TCHAR) * _MAX_PATH );
	strcpy( extscriptfilename, filenamestring.Get_String() );
	strcat( extscriptfilename, _T("XSS") );
	
	pFilePointer = NULL;
	bScriptFromPackage = FALSE;
	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);
		pFilePointer = fopen( extscriptfilename, "rb" );
		if( !pFilePointer )
		{
			pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
			if( !pFilePointer )
			{
				_XFatalError( "Can't open environment sound script [%s]", extscriptfilename );
				if( bTerrainLoadFromFile ) fclose( terrainfile );
				terrainpackagearchive.FinalizePackage();
				terrainscriptpackagearchive.FinalizePackage();
				return FALSE;
			}			
			bScriptFromPackage = TRUE;
		}
	}
	else
	{
		pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
		if( !pFilePointer )
		{
			if( bTerrainLoadFromFile ) fclose( terrainfile );
			terrainpackagearchive.FinalizePackage();
			terrainscriptpackagearchive.FinalizePackage();
			return FALSE;
		}
		bScriptFromPackage = TRUE;
	}	

	if( !LoadSound( pFilePointer ) )
	{
		_XFatalError("LoadTerrain(%s) Load environment sound data", filename);		
		fclose(terrainfile);
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		if( !bScriptFromPackage ) fclose( pFilePointer );
		return FALSE;
	}

	if( !bScriptFromPackage ) 
	{
		fclose( pFilePointer );
	}

	memset( extscriptfilename, 0, sizeof(TCHAR) * _MAX_PATH );
	strcpy( extscriptfilename, filenamestring.Get_String() );
	strcat( extscriptfilename, _T("XFS") );
	
	pFilePointer = NULL;
	bScriptFromPackage = FALSE;
	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);
		pFilePointer = fopen( extscriptfilename, "rb" );
		if( !pFilePointer )
		{
			pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
			if( !pFilePointer )
			{
				_XFatalError( "Can't open environment effect script [%s]", extscriptfilename );
				if( bTerrainLoadFromFile ) fclose( terrainfile );
				terrainpackagearchive.FinalizePackage();
				terrainscriptpackagearchive.FinalizePackage();
				return FALSE;
			}
			bScriptFromPackage = TRUE;
		}
	}
	else
	{
		pFilePointer = terrainscriptpackagearchive.GetPackedFile( extscriptfilename );
		if( !pFilePointer )
		{
			if( bTerrainLoadFromFile ) fclose( terrainfile );
			terrainpackagearchive.FinalizePackage();
			terrainscriptpackagearchive.FinalizePackage();
			return FALSE;
		}
		bScriptFromPackage = TRUE;
	}


	if( !LoadEffect( pFilePointer ) )
	{
		_XFatalError("LoadTerrain(%s) Load Environment effect data", filename);
		if( bTerrainLoadFromFile ) fclose( terrainfile );
		terrainpackagearchive.FinalizePackage();
		terrainscriptpackagearchive.FinalizePackage();
		if( !bScriptFromPackage ) fclose( pFilePointer );
		return FALSE;
	}

	if( !bScriptFromPackage )
	{
		fclose( pFilePointer );
	}

	if( !LoadSplatTileImage( terrainfile ) )
	{
		_XFatalError("LoadTerrain(%s) Load tile splat cache data", filename);
		fclose(terrainfile);
		return FALSE;
	}

	// Close terrain file handle
	if( bTerrainLoadFromFile ) fclose( terrainfile );
	terrainpackagearchive.FinalizePackage();
	terrainscriptpackagearchive.FinalizePackage();

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2722 ) ); //"외도 세력 관련 데이터를 준비중입니다."
	pLoadingProcess->UpdateProgress(50);

	if( !PreCachingMonsterModel() )
	{
		_XFatalError("LoadTerrain(%s) Precaching monster model", filename);
		fclose(terrainfile);
		return FALSE;
	}
				
#ifdef _XDWDEBUG
	axismatrix._42 = GetTerrainHeight( D3DXVECTOR3( 0.0f,0.0f,0.0f) ) + 1.5f;
#endif

	pLoadingProcess->SetMessage( _XGETINTERFACETEXT( ID_STRING_NEW_2723 ) ); //지형 정보를 최적화합니다.
	pLoadingProcess->UpdateProgress(73);

	_XMEMORYUSECHECKREADY		
	_XMEMORYUSECHECKSTART

	// Create loose quad tree
	if( !m_ObjectQuadTree.CreateTree( this->m_Width, this->m_ObjectManager.m_ObjectCount, this->m_ObjectManager.m_pObjectArray ) )
	{
		return FALSE;		
	}

	m_ObjectQuadTree.SetCameraInfo( m_3PCamera );

	_XMEMORYUSECHECKEND( "Create object quad tree" );

	CheckWaterAboveTile();

	Initial_TileQuad();
		
	_XDWINPRINT( "Loaded %s terrain file", filename );
			
	return TRUE;
}

BOOL _XLodTerrain::PreCachingMonsterModel( void )
{
	XProc_Loading* pLoadingProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_loading;

	FLOAT progressincreasefactor = 20.0f / (FLOAT)g_CurrentCachedMobcount;
	int   lastupdateprogress = 50;

	if( !g_ModelArchive.ReOpenPackage() ) 
		return FALSE;

	if( !g_AniArchive.ReOpenPackage() ) 
	{
		g_ModelArchive.ClosePackage();
		return FALSE;
	}
	
	int progresspoint = 0;
	for( int i = 1; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
	{
		if( g_MobAttribTable[i].usecurrentzone )
		{
			g_Moblist.AddReferenceModel_Precaching( i );

			int currentprogressfactor = (int)(50.0f + ((FLOAT)progresspoint * progressincreasefactor));
			if( currentprogressfactor != lastupdateprogress )
			{
				lastupdateprogress = currentprogressfactor;
			}
			progresspoint++;
		}
		else
		{
			g_Moblist.ReleaseReferenceModel( i, TRUE );
		}
	}

	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();

	return TRUE;
}

BOOL _XLodTerrain::DisposeCachedMonsterModel( void )
{
	for( int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
	{
		g_Moblist.ReleaseReferenceModel( i, TRUE );
	}
	return TRUE;
}


void _XLodTerrain::CheckWaterAboveTile( void )
{
	for (int i=0;i<m_TileCount;++i)
	{
		for (int j=0;j<m_TileCount;++j)
		{
			_XLODTILE* pCurrentTile = &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j];
			pCurrentTile->RenderReflectionMap = FALSE;

			for( int z = -1; z <= 1; z++ )
			{
				if( i+z < 0 ) continue;
				if( i+z >= m_TileCount ) continue;
				
				for( int x = -1; x <= 1; x++ )
				{
					if( j+x < 0 ) continue;
					if( j+x >= m_TileCount ) continue;
					
					S_WATER* curwater = m_pWaterObjectManager->GetWaterObject( (j+x) + ((i+z) * m_TileCount) );
					if( curwater )
					{
						if( curwater->enabled ) 
						{
							pCurrentTile->RenderReflectionMap = TRUE;
							break;
						}
					}				
				}
				
				if( pCurrentTile->RenderReflectionMap )
				{
					break;
				}
			}
		}
	}
}

BOOL _XLodTerrain::LoadSound( FILE* terrainfile )
{
	int nCount = 0;
	fread(&nCount,		sizeof(int),		1,		terrainfile );
	if (nCount <= 0) return TRUE;

#ifndef _XUSEFMOD
	_XSetDefaultPath(_XDEF_DEFAULTPATH);
	g_EnvSoundEffectArchive.Open( _XDEF_ENVSOUNDEFFECTARCHIVENAME );
#endif

	for (int i = 0; i < nCount; ++ i)
	{
		_XSoundObject* object = new _XSoundObject;
		fread(&object->m_ObjectIndex,		sizeof(int),			1,		terrainfile );
		fread(&object->m_matWorldPosition,	sizeof(D3DXMATRIX),		1,		terrainfile );
		fread(&object->m_byAttribute,		sizeof(BYTE),			1,		terrainfile );
		fread(&object->m_byEvent,			sizeof(BYTE),			1,		terrainfile );
		fread(&object->m_byDistance,		sizeof(BYTE),			1,		terrainfile );
		
		fread(&object->m_byInterval,		sizeof(BYTE),			1,		terrainfile );
		fread(&object->m_byCount,			sizeof(BYTE),			1,		terrainfile );

		object->m_fDistance2 = ((FLOAT)object->m_byDistance * (FLOAT)object->m_byDistance);

		object->m_pSound = &g_pEnvironmentSound[object->m_ObjectIndex];

		m_SoundObjectManager.insertObject( object );
	}

#ifndef _XUSEFMOD
	g_EnvSoundEffectArchive.Close( FALSE );
	_XRestorePrevPath();
#endif

	return TRUE;
}

BOOL _XLodTerrain::LoadSplatTileImage( FILE* terrainfile )
{

#ifdef _XDEF_USESPLATCACHEDATA

	BYTE	ImageLoadBuffer[1024];
	UINT    readsize = 0;

	fread( &readsize, sizeof(UINT), 1, terrainfile );

	if( readsize <= 0 )
	{
		return FALSE;
	}

	for (int i=0;i<m_TileCount;++i)
	{
		for (int j=0;j<m_TileCount;++j)
		{
			_XLODTILE* pCurrentTile = &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j];
			if( !pCurrentTile->SplatData ) return FALSE;

			SAFE_RELEASE( pCurrentTile->m_pPreRenderedTextureObject );

			if( fread( ImageLoadBuffer, readsize, 1, terrainfile ) < 1 )
			{
				_XFatalError( "Can't read splat texture [%d:%d]", i, j );
				return FALSE;
			}

			if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, ImageLoadBuffer, readsize, 
				_XDEF_SPLATIMAGESIZE, _XDEF_SPLATIMAGESIZE, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
				D3DX_FILTER_NONE, D3DX_FILTER_NONE,
				0, NULL, NULL, &pCurrentTile->m_pPreRenderedTextureObject) ) )
			{
				_XFatalError( "Can't create splat texture [%d:%d]", i, j );
				return FALSE;
			}
		}
	}

#endif

	return TRUE;
}


BOOL _XLodTerrain::LoadEffect( FILE* terrainfile )
{
	int nCount = 0;
	fread(&nCount,		sizeof(int),		1,		terrainfile );
	if (nCount <= 0) return TRUE;

	for (int i = 0; i < nCount; ++ i)
	{
		_XEffectObject* object = new _XEffectObject;

		int templateid = 0;
		fread(&templateid,					sizeof(int),			1,		terrainfile );		
		fread(&object->m_matWorldPosition,	sizeof(D3DXMATRIX),		1,		terrainfile );
		fread(&object->m_byEvent,			sizeof(BYTE),			1,		terrainfile );
		fread(&object->m_byDistance,		sizeof(BYTE),			1,		terrainfile );

		g_WorldEffectGroupManager.FindTemplate( templateid, object->m_ObjectIndex );

		if( object->m_ObjectIndex < 0 )
		{
			_XFatalError( "Can't find env effect template [id:%d]", templateid );
			return FALSE;
		}

		object->m_pParticleGroupItem = NULL;

		if (object->m_byEvent == _XEffectObject::EVENT_ALL)
		{
			_XParticleGroupItem* pParticleGroupItem  = &g_WorldEffectGroupManager.m_ParticleTemplateList[object->m_ObjectIndex];
			object->m_pParticleGroupItem = m_EffectObjectManager.InsertNewInstance( &g_WorldEffectGroupManager, pParticleGroupItem->m_ID, &object->m_matWorldPosition);
		}

		m_svEffectTemplateList.push_back(object);
	}

	return TRUE;
}

BOOL _XLodTerrain::LoadEnvScript( FILE* FilePtr )
{
	if( !FilePtr ) return FALSE;
	
	for (int i=0;i<m_TileCount;++i)
	{
		for (int j=0;j<m_TileCount;++j)
		{
			if( fread( &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j].EnvScriptData, sizeof(_XENVIRONMENTSCRIPTDATA), 1, FilePtr ) < 1 )
			{
				_XFatalError( "Load Environments script : read script informations" );
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL _XLodTerrain::LoadTerrainTileDetailInfo(FILE *fileptr)
{
	if(!fileptr) return FALSE;
	
	// reset layer informations...
	m_UsedLayerCount = 0;
	for( int layer = 0; layer < _XDEF_MAXSPLATCOUNT; layer++ )
	{
		m_LayerInfo[layer].Reset();
	}

	if( fread( &m_UsedLayerCount, sizeof(int), 1, fileptr ) < 1 )
	{
		return FALSE;
	}
	
	if( m_UsedLayerCount > _XDEF_MAXSPLATCOUNT ) 
	{
		_XFatalError( "Overflowed splat layer count" );
		return FALSE;
	}

	// Load layer informations...
		
	for( int i = 0; i < m_UsedLayerCount; i++ )
	{
		if( fread( &m_LayerInfo[i].m_Translation, sizeof(D3DXVECTOR2), 1, fileptr ) < 1 )	return FALSE;
		if( fread( &m_LayerInfo[i].m_Scale, sizeof(D3DXVECTOR2), 1, fileptr ) < 1 )			return FALSE;
		if( fread( &m_LayerInfo[i].m_rotateangle, sizeof(FLOAT), 1, fileptr ) < 1 )			return FALSE;
		m_LayerInfo[i].UpdateMatrix();
	}

	switch( g_XGQProperty.terrainqualitylevel ) 
	{
	default:
	case _XGQLEVEL_MAX :
		g_TextureQuality		= 1;
		break;
	case _XGQLEVEL_HIGH:
		g_TextureQuality		= 2;
		break;
	case _XGQLEVEL_MIDDLE :
		g_TextureQuality		= 4;
		break;
	case _XGQLEVEL_LOW :
		g_TextureQuality		= 8;
		break;	
	}

	// Load diffuse texture names...
	int stringlength = 0;
	TCHAR filenamebuffer[_MAX_PATH];
	for( i = 0; i < m_UsedLayerCount; i++ )
	{		
		memset( filenamebuffer, 0, sizeof(TCHAR)*_MAX_PATH );
		if( fread( &stringlength, sizeof(int), 1, fileptr ) < 1 )	return FALSE;
		if( fread( filenamebuffer, sizeof(TCHAR)*stringlength, 1, fileptr ) < 1 )	return FALSE;

		m_LayerInfo[i].m_DetailTextureIndex = g_WorldObjectTextureArchive.GetResourceIndex( filenamebuffer, g_TextureMipLevel_TerrainDetail );
	}

	g_TextureQuality		= 1;
	g_TextureMemoryPool			=	D3DPOOL_MANAGED;


	// create splat layer stack...
	BYTE BlendLevelArray[_XDEF_MAXSPLATCOUNT][_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE];
	//BYTE BlendLevelAccumulateArray[_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE];
	BOOL UseLayer[_XDEF_MAXSPLATCOUNT];
	
	int  TileUseCount[_XDEF_MAXSPLATCOUNT];
	for( int layerindex = 0; layerindex < _XDEF_MAXSPLATCOUNT; layerindex++ ) TileUseCount[layerindex] = 0;
	
	for (i=0;i<m_TileCount;++i)
	{
		for (int j=0;j<m_TileCount;++j)
		{
			int usedlayercount = 0;
			for( int layer = 0; layer < m_UsedLayerCount; layer++ )
			{
				if( fread( &UseLayer[layer], sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;

				if( UseLayer[layer] )
				{
					usedlayercount++;
					TileUseCount[layer]++;

					if( layer > 0 )
					{
						if( fread( BlendLevelArray[layer], sizeof(BYTE)*(_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE), 1, fileptr ) < 1 )
						{
							return FALSE;
						}
					}
				}
			}

			_XLODTILE* pCurrentTile = &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j];
			
			if( usedlayercount > 0 )
			{
				pCurrentTile->SplatData = new _XTileSplat[ usedlayercount+1 ]; // include zero level splat
				
				int curlayerindex = 0;
				for( layer = 0; layer < m_UsedLayerCount; layer++ )
				{
					if( UseLayer[layer] )
					{
						//pCurrentTile->SplatData[curlayerindex].m_Use = UseLayer[layer];
						pCurrentTile->SplatData[curlayerindex].m_LayerIndex = layer;

						if( layer > 0 ) // disable splat layer 0
						{
							if( !pCurrentTile->SplatData[curlayerindex].ApplyBlendLevelToBlendMap(BlendLevelArray[layer], fileptr ) )
							{
								return FALSE;
							}
						}
						curlayerindex++;
					}
				}
				pCurrentTile->UsedSplatLayerCount = curlayerindex;

				if( !UseLayer[0] )
				{
					/*int splattrianglecount = 0;
					if( fread( &splattrianglecount, sizeof(DWORD), 1, fileptr ) < 1 ) 
					{
						return FALSE;
					}
					*/
					fseek( fileptr, sizeof(DWORD) + sizeof(BYTE) * (_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE), SEEK_CUR );
				}
				else
				{
					if( !pCurrentTile->SplatData[0].CreateZeroLayerIndexBuffer( fileptr ) )
					{
						return FALSE;
					}
				}
			}
			else
			{
				pCurrentTile->UsedSplatLayerCount = 0;
			}
		}
	}

	// Create rendering cache buffer
	m_TerrainTileRenderingCache = new _XLODTILEINDEXFORMAT*[ m_UsedLayerCount+3 ];
	m_TerrainTileRenderingCacheLength = new int[m_UsedLayerCount+3];
	
	for( layerindex = 0 ; layerindex < m_UsedLayerCount+3; layerindex++ )
		 m_TerrainTileRenderingCacheLength[layerindex] = 0;	

	for( layer = 0; layer < m_UsedLayerCount; layer++ )
	{
		_XDWINPRINT( "CTRB_CREATE : Layer:%d Cnt:%d", layer, TileUseCount[layer] );

		m_TerrainTileRenderingCache[ layer ] = new _XLODTILEINDEXFORMAT[ TileUseCount[layer] ];
		memset( m_TerrainTileRenderingCache[ layer ], 0, sizeof( _XLODTILEINDEXFORMAT ) * TileUseCount[layer] );
	}

	// Total layer
	m_TerrainTileRenderingCache[ m_UsedLayerCount ] = new _XLODTILEINDEXFORMAT[ m_TileCount * m_TileCount ];
	memset( m_TerrainTileRenderingCache[ m_UsedLayerCount ], 0, sizeof( _XLODTILEINDEXFORMAT ) * m_TileCount * m_TileCount );

	// Use prerendered image layer
	m_TerrainTileRenderingCache[ m_UsedLayerCount+1 ] = new _XLODTILEINDEXFORMAT[ m_TileCount * m_TileCount ];
	memset( m_TerrainTileRenderingCache[ m_UsedLayerCount+1 ], 0, sizeof( _XLODTILEINDEXFORMAT ) * m_TileCount * m_TileCount );

	// Reflection tile layer
	m_TerrainTileRenderingCache[ m_UsedLayerCount+2 ] = new _XLODTILEINDEXFORMAT[ m_TileCount * m_TileCount ];
	memset( m_TerrainTileRenderingCache[ m_UsedLayerCount+2 ], 0, sizeof( _XLODTILEINDEXFORMAT ) * m_TileCount * m_TileCount );

	return TRUE;
}

BOOL _XTileSplat::ApplyBlendLevelToBlendMap( LPBYTE pBlendMap, FILE* pTerrainFilePtr )
{
	if( !pTerrainFilePtr ) return FALSE;

	memcpy( m_BlendLevelArray, pBlendMap, sizeof(BYTE) * _XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE );
	
	if( !m_pBlendTexture )
	{		
		if( FAILED( gpDev->CreateTexture( _XDEF_SPLATBLENDTEXTURESIZE, _XDEF_SPLATBLENDTEXTURESIZE, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &m_pBlendTexture, NULL ) ) )
		{
			_XFatalError( "Can't create splat blend map" );
			return FALSE;
		}
	}
	
	D3DLOCKED_RECT LockData;
	if ( FAILED(m_pBlendTexture->LockRect( 0, &LockData, NULL, 0 )) )
	{
		_XFatalError( "Can't lock splat blend texture" );
		return false;
	}
	
	USHORT * pBuffer = (USHORT*)LockData.pBits;
	
	LPBYTE pBlendMapPtr = pBlendMap;
	BYTE blendvalue;

	for ( int z = 0; z < _XDEF_SPLATBLENDTEXTURESIZE; z++ )
	{
		for ( int x = 0; x < _XDEF_SPLATBLENDTEXTURESIZE; x++, pBuffer++ )
		{
#ifdef _XDEF_USETERRAINUV
			blendvalue = *pBlendMapPtr++;
#else
			blendvalue = pBlendMapPtr[z+x*_XDEF_SPLATBLENDTEXTURESIZE];
#endif
			*pBuffer = (USHORT)( ((LONG)blendvalue << 12) & 0xF000 ) + 0xFFF;
		}
		
		pBuffer += (LockData.Pitch - ( _XDEF_SPLATBLENDTEXTURESIZE * sizeof(USHORT) ) );
	}
	m_pBlendTexture->UnlockRect( 0 );
	
	if( fread( &m_SplatTriangleCount, sizeof(DWORD), 1, pTerrainFilePtr ) < 1 ) return FALSE;

	SAFE_RELEASE( m_pSplatIndexBuffer );

	if( m_SplatTriangleCount> 0 )
	{
		// Create index buffer
		if ( FAILED( gpDev->CreateIndexBuffer( sizeof(WORD) * (m_SplatTriangleCount * 3), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pSplatIndexBuffer, NULL ) ) )
		{
			return FALSE;
		}
		
		LPWORD pIndex = NULL;
		if (SUCCEEDED(m_pSplatIndexBuffer->Lock(0, sizeof(WORD) * (m_SplatTriangleCount * 3), (void**)&pIndex, g_3DObjectLockMethods)))
		{
			if( fread( pIndex, sizeof(WORD) * m_SplatTriangleCount * 3, 1, pTerrainFilePtr ) < 1 )
			{
				m_pSplatIndexBuffer->Unlock();
				return FALSE;
			}
			m_pSplatIndexBuffer->Unlock();
		}
	}
		
	/*
	DWORD blendpolycount = 0; // use index buffer...
	BYTE  blurringmap[_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE];
	memset( blurringmap, 0, sizeof(BYTE) * _XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE );

	for ( z = 0; z < _XDEF_SPLATBLENDTEXTURESIZE; z++ )
	{
		for ( int x = 0; x < _XDEF_SPLATBLENDTEXTURESIZE; x++ )
		{
			int arrayindex = x+z*_XDEF_SPLATBLENDTEXTURESIZE;

			if( pBlendMap[arrayindex] == 0 )
			{	
				BOOL looping = TRUE;
				for( int i = -1; i <= 1; i++ )
				{
					for( int j = -1; j <= 1; j++ )
					{
						if( z+i	>= 0 && z+i < _XDEF_SPLATBLENDTEXTURESIZE &&
							x+j	>= 0 && x+j < _XDEF_SPLATBLENDTEXTURESIZE )
						{
							if( pBlendMap[(x+j)+(z+i)*_XDEF_SPLATBLENDTEXTURESIZE] )
							{
								blurringmap[arrayindex] = 1;
								blendpolycount++;
								looping = FALSE;
								break;
							}
						}
					}

					if(!looping) break;
				}
			}
			else
			{
				blurringmap[arrayindex] = 1;
				blendpolycount++;
			}
		}
	}

	if( blendpolycount >= 64 ) // 모든 인덱스를 포함하면 LOD측 인덱스 사용
	{
		return TRUE;
	}

	if( blendpolycount > 0 )
	{
		// Create index buffer
		if ( FAILED( gpDev->CreateIndexBuffer( sizeof(WORD) * (blendpolycount * 6), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pSplatIndexBuffer, NULL ) ) )
		{
			return FALSE;
		}
		
		LPWORD pIndex = NULL;
		if (SUCCEEDED(m_pSplatIndexBuffer->Lock(0, sizeof(WORD) * (blendpolycount * 6), (void**)&pIndex, g_3DObjectLockMethods)))
		{
			pBlendMapPtr = blurringmap;
			for ( z = 0; z < _XDEF_SPLATBLENDTEXTURESIZE; z++ )
			{
				for ( int x = 0; x < _XDEF_SPLATBLENDTEXTURESIZE; x++ )
				{
					if( *pBlendMapPtr > 0 )
					{
						int indexstpos = VertexIndexLUT[z][x][0] * 3;
						*pIndex++ = g_TerrainIndexLUT[ indexstpos   ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+1 ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+2 ];
						
						indexstpos = VertexIndexLUT[z][x][1] * 3;
						*pIndex++ = g_TerrainIndexLUT[ indexstpos   ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+1 ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+2 ];
					}
					pBlendMapPtr++;
				}
			}
			
			m_pSplatIndexBuffer->Unlock();

			m_SplatTriangleCount = (blendpolycount<<1);
		}		
	}
	*/

	return TRUE;
}

BOOL _XTileSplat::CreateZeroLayerIndexBuffer( FILE* pTerrainFilePtr )
{
	if( !pTerrainFilePtr ) 
	{
		_XFatalError( "Failed to read : invalid file pointer" );
		return FALSE;
	}

	if( fread( &m_SplatTriangleCount, sizeof(DWORD), 1, pTerrainFilePtr ) < 1 )
	{
		_XFatalError( "Failed to read : splat triangle count" );
		return FALSE;
	}

	//_ASSERT( m_pSplatIndexBuffer );
	//SAFE_RELEASE( m_pSplatIndexBuffer );

	if( m_SplatTriangleCount> 0 )
	{
		// Create index buffer
		if ( FAILED( gpDev->CreateIndexBuffer( sizeof(WORD) * (m_SplatTriangleCount * 3), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pSplatIndexBuffer, NULL ) ) )
		{
			_XFatalError( "Failed to create : splat triangle index buffer" );
			return FALSE;
		}
		
		LPWORD pIndex = NULL;
		if (SUCCEEDED(m_pSplatIndexBuffer->Lock(0, sizeof(WORD) * (m_SplatTriangleCount * 3), (void**)&pIndex, g_3DObjectLockMethods)))
		{
			if( fread( pIndex, sizeof(WORD) * m_SplatTriangleCount * 3, 1, pTerrainFilePtr ) < 1 )
			{
				m_pSplatIndexBuffer->Unlock();
				_XFatalError( "Failed to read : splat triangle index buffer" );
				return FALSE;
			}
			m_pSplatIndexBuffer->Unlock();
		}
	}
	
	if( fread( m_BlendLevelArray, sizeof(BYTE) * _XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE , 1, pTerrainFilePtr ) < 1 )
	{
		_XFatalError( "Failed to read : zero level blend array" );
		return FALSE;
	}
	
	/*
	DWORD blendpolycount = 0; // use index buffer...
	BYTE  blurringmap[_XDEF_SPLATBLENDTEXTURESIZE*_XDEF_SPLATBLENDTEXTURESIZE];
	
	for ( int z = 0; z < _XDEF_SPLATBLENDTEXTURESIZE; z++ )
	{
		for ( int x = 0; x < _XDEF_SPLATBLENDTEXTURESIZE; x++ )
		{
			int arrayindex = x+z*_XDEF_SPLATBLENDTEXTURESIZE;
			
			if( pBlendMap[arrayindex] == 15 )
			{
				BOOL looping = TRUE;
				for( int i = -1; i <= 1; i++ )
				{
					for( int j = -1; j <= 1; j++ )
					{
						if( z+i	>= 0 && z+i < _XDEF_SPLATBLENDTEXTURESIZE &&
							x+j	>= 0 && x+j < _XDEF_SPLATBLENDTEXTURESIZE )
						{
							if( pBlendMap[(x+j)+(z+i)*_XDEF_SPLATBLENDTEXTURESIZE] < 15 )
							{
								blurringmap[arrayindex] = 0;
								blendpolycount++;
								looping = FALSE;
								break;
							}
						}
					}
					
					if(!looping) break;
				}

				if( looping )
				{
					blurringmap[arrayindex] = 15;
				}
			}
			else
			{
				blurringmap[arrayindex] = 0;
				blendpolycount++;
			}
		}
	}
	
	if( blendpolycount >= 64 ) // 모든 인덱스를 포함하면 LOD측 인덱스 사용
	{
		return TRUE;
	}

	if( blendpolycount > 0 )
	{
		// Create index buffer
		if ( FAILED( gpDev->CreateIndexBuffer( sizeof(WORD) * (blendpolycount * 6), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pSplatIndexBuffer, NULL ) ) )
		{
			return FALSE;
		}
		
		LPWORD pIndex = NULL;
		LPBYTE	pBlendMapPtr = blurringmap;
		if (SUCCEEDED(m_pSplatIndexBuffer->Lock(0, sizeof(WORD) * (blendpolycount * 6), (void**)&pIndex, g_3DObjectLockMethods)))
		{
			for ( int z = 0; z < _XDEF_SPLATBLENDTEXTURESIZE; z++ )
			{
				for ( int x = 0; x < _XDEF_SPLATBLENDTEXTURESIZE; x++ )
				{
					if( *pBlendMapPtr < 15 )
					{
						int indexstpos = VertexIndexLUT[z][x][0] * 3;
						*pIndex++ = g_TerrainIndexLUT[ indexstpos   ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+1 ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+2 ];
						
						indexstpos = VertexIndexLUT[z][x][1] * 3;
						*pIndex++ = g_TerrainIndexLUT[ indexstpos   ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+1 ];
						*pIndex++ = g_TerrainIndexLUT[ indexstpos+2 ];
					}
					pBlendMapPtr++;
				}
			}
			
			m_pSplatIndexBuffer->Unlock();
			
			m_SplatTriangleCount = (blendpolycount<<1);
		}		
	}
	*/
	return TRUE;
}

BOOL _XLodTerrain::LoadCollidePolyList( FILE* fileptr )
{
	if( !fileptr )
	{
		_XFatalError( "Load obb list : Invalid file handler" );
		return FALSE;
	}

	int collidepolycount = 0;

	if( fread( &collidepolycount, sizeof( int ), 1, fileptr ) < 1 )
	{
		_XFatalError( "Load Collide polygon list / read poly count" );
		return FALSE;
	}
	
	return TRUE;
}

BOOL _XLodTerrain::LoadOBBList( FILE* fileptr )
{
	_XMEMORYUSECHECKREADY		
	_XMEMORYUSECHECKSTART
	
	if( !fileptr )
	{
		_XFatalError( "Load obb list : Invalid file handler" );
		return FALSE;
	}
	
	DWORD	indicator = 0;
	if( fread( &indicator, sizeof( DWORD ), 1, fileptr ) < 1 )
	{
		_XFatalError( "Load OBB List / read indicator" );
		return FALSE;
	}

	if( indicator != _XDEF_X3DOBBFILEINDICATOR )
	{
		_XFatalError( "Load OBB List / check indicator" );
		return FALSE;		
	}

	int obbnodecount = 0;

	if( fread( &obbnodecount, sizeof( int ), 1, fileptr ) < 1 )
	{
		_XFatalError( "Load OBB List / read obb node count" );
		return FALSE;
	}

	_XOBB_SaveFormat saveobbformat;

	SAFE_DELETE_ARRAY( m_OBBDataList );
	m_AllOBBCount = obbnodecount;

	if( obbnodecount <= 0 ) 
	{
		return TRUE;
	}

	m_OBBDataList = new _XOBBData[obbnodecount];
	
	if( !m_OBBDataList )
	{
		_XFatalError( "Allocate obb node" );
	}

	FLOAT tilerange = _XFastDistance( 8, 8 );

	D3DXVECTOR3 CollisionPoint;
	D3DXVECTOR3 vTileCenter(0, 0, 0);
	D3DXVECTOR3 vTileAxis[3];
	FLOAT fExtent[3];

	vTileAxis[0] = D3DXVECTOR3(1, 0, 0);
	vTileAxis[1] = D3DXVECTOR3(0, 1, 0);
	vTileAxis[2] = D3DXVECTOR3(0, 0, 1);

	fExtent[0] = 13.f;
	fExtent[1] = 13.f;
	fExtent[2] = 13.f;

	_XOBBData TileObb;
	TileObb.Create(vTileCenter, vTileAxis, fExtent);
	TileObb.RecalcRadius();

	FLOAT fRadius;
	int nStartZ, nEndZ;
	int nStartX, nEndX;
	
	for( int i = 0; i < obbnodecount; i++ )
	{				
		if( fread( &saveobbformat, sizeof(_XOBB_SaveFormat), 1, fileptr ) < 1 )
		{
			_XFatalError( "Load obb structure" );
			return FALSE;
		}
		
		m_OBBDataList[i].m_kCenter = saveobbformat.kCenter;
		m_OBBDataList[i].m_OBBLevel = saveobbformat.OBBLevel;
		memcpy( m_OBBDataList[i].m_akAxis, saveobbformat.akAxis, sizeof(D3DXVECTOR3) * 3 );
		memcpy( m_OBBDataList[i].m_afExtent, saveobbformat.afExtent, sizeof(FLOAT) * 3 );		
				
		if( m_OBBDataList[i].m_akAxis[0].x == 0.0f ) m_OBBDataList[i].m_akAxis[0].x = 1.0f;
		if( m_OBBDataList[i].m_akAxis[1].y == 0.0f ) m_OBBDataList[i].m_akAxis[1].y = 1.0f;
		if( m_OBBDataList[i].m_akAxis[2].z == 0.0f ) m_OBBDataList[i].m_akAxis[2].z = 1.0f;
		
		m_OBBDataList[i].RecalcRadius();
		m_OBBDataList[i].CreateModelVertex();
		
#ifdef _XDWDEBUG
		m_OBBDataList[i].CreateOBBModel();
#endif					

		fRadius = _XFC_sqrt(m_OBBDataList[i].m_Radius);

		nStartZ = ((m_OBBDataList[i].m_kCenter.z - fRadius) + m_HalfWorldSize) / 16;
		nEndZ = ((m_OBBDataList[i].m_kCenter.z + fRadius) + m_HalfWorldSize) / 16 + 1;

		nStartX = ((m_OBBDataList[i].m_kCenter.x - fRadius) + m_HalfWorldSize) / 16;
		nEndX = ((m_OBBDataList[i].m_kCenter.x + fRadius) + m_HalfWorldSize) / 16 + 1;
	
		_XLODTILE* curtile;
		for( int z = nStartZ; z < nEndZ; z++ )
		{
			for( int x = nStartX; x < nEndX; x++ )
			{
				//if( m_OBBDataList[i].m_OBBLevel == 0 )
				{
					curtile = GetTerrainTile( x, z );
					if( curtile )
					{
						vTileCenter	= curtile->Center;
						vTileCenter.y = m_OBBDataList[i].Center().y;
						TileObb.SetCenter(vTileCenter);

						if (TileObb.IntersectOBBvsOBB(m_OBBDataList[i], CollisionPoint))
						{
							curtile->svObbList.push_back(&m_OBBDataList[i]);
						}
					}
				}
			}
		}
	}	

	_XMEMORYUSECHECKEND( "_XLodTerrain::LoadOBBList" );

	return TRUE;
}

BOOL _XLodTerrain::IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
									  FLOAT* t, FLOAT* u, FLOAT* v )
{
	// Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &g_vPickRayDir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );
    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = g_vPickRayOrig - v0;
    }
    else
    {
        tvec = v0 - g_vPickRayOrig;
        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
        return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &g_vPickRayDir, &qvec );
    if( *v < 0.0f || *u + *v > det )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return TRUE;

}

BOOL _XLodTerrain::IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
		                              D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 )
{
	// Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
    }

    if( det < 0.0001f ) return FALSE;

	FLOAT u, v;

    // Calculate U parameter and test bounds
    u = D3DXVec3Dot( &tvec, &pvec );
    if( u < 0.0f || u > det ) return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    v = D3DXVec3Dot( &dir, &qvec );
    if( v < 0.0f || u + v > det ) return FALSE;
	
    return TRUE;
}


BOOL _XLodTerrain::IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 )
{
    // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &g_vPickRayDir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = g_vPickRayOrig - v0;
    }
    else
    {
        tvec = v0 - g_vPickRayOrig;
        det = -det;
    }

    if( det < 0.0001f ) return FALSE;

	FLOAT u, v;

    // Calculate U parameter and test bounds
    u = D3DXVec3Dot( &tvec, &pvec );
    if( u < 0.0f || u > det ) return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    v = D3DXVec3Dot( &g_vPickRayDir, &qvec );
    if( v < 0.0f || u + v > det ) return FALSE;
	
    return TRUE;
}

#define _XDEF_RTIMEOBJECTRANGE		2

#define _XDEF_MAX_TILE_GRASS_COUNT		64

void _XLodTerrain::GenerateGrassObject( void )
{		
/*	if( m_LastRebuildLevelTileX < 0 || m_LastRebuildLevelTileX >= m_TileCount )
	{
		return;
	}
		
	if( m_LastRebuildLevelTileZ < 0 || m_LastRebuildLevelTileZ >= m_TileCount )
	{
		return;		
	}
*/
	m_GrassManager.ResetCount();

	int nTile = 1;
	if (m_RealTimeObjectGenerateCount > 0)
	{
		nTile = ((m_RealTimeObjectGenerateCount-1) / 10) + 1;
	}

	m_GrassManager.GRASS_ALPHA_END		= nTile * 16.f;
	m_GrassManager.GRASS_ALPHA_START	= m_GrassManager.GRASS_ALPHA_END - 32.f;


	int stx, stz;
	int endx, endz;

	int xcoord = (_XFC_FloatToIntRet( ( g_pLocalUser->m_Position.x + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );
	int zcoord = (_XFC_FloatToIntRet( ( g_pLocalUser->m_Position.z + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );

	// Clipping...
	if( xcoord < nTile ) stx = 0;
	else stx = xcoord-nTile;

	if( zcoord < nTile ) stz = 0;
	else stz = zcoord-nTile;

	if( xcoord > m_TileCount-(nTile+1) ) endx = (m_TileCount - 1);
	else endx = xcoord+nTile;

	if( zcoord > m_TileCount-(nTile+1) ) endz = (m_TileCount - 1);
	else endz = zcoord+nTile;

	_XRealTimeObject* realtimeobject = NULL;

	D3DXVECTOR3	ropos;
	D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);
	D3DXVECTOR3	facenormal;
	D3DXMATRIX  rotmatrix;
	D3DXQUATERNION normalrotquat;

	int nGrassCount = m_RealTimeObjectGenerateCount;
	if( nGrassCount > 64 ) nGrassCount = 64;

	for( int i = stz; i <= endz; i++ )
	{
		for( int j = stx; j <= endx; j++ )
		{
			_XLODTILE* pCurrentTile = &m_TerrainTile[m_TileStartOffset+j][m_TileStartOffset+i];

			// 찍을 풀이 없으면 스킵
			if( !pCurrentTile->m_EnableGrassGenerate ) continue;

			// 프러스텀에 걸리지 않으면 스킵
			if( m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pCurrentTile->Center, 30.0f ) == NOT_VISIBLE )
			{
				continue;
			}

			D3DXMATRIX rotmat;

			for( int li = 0; li < nGrassCount; ++li )
			{
				int seedpos = g_GrassGenerateSeedTable[li];

				if( pCurrentTile->m_pGrassAttribArray[ seedpos ] )
				{
					srand( seedpos );

					realtimeobject = m_GrassManager.New_RealTimeObject();

					FLOAT newxpos = ((float)(rand() % 50) / 50.f) + 1.0f;
					FLOAT newzpos = ((float)(rand() % 50) / 50.f) + 1.0f;
					
					ropos.x = pCurrentTile->Center.x - 8.0f + (FLOAT)((seedpos / 8)<<1) + newxpos; 
					ropos.z = pCurrentTile->Center.z - 8.0f + (FLOAT)((seedpos % 8)<<1) + newzpos;
					ropos.y = this->GetTerrainHeight( ropos, &facenormal ) - 0.05f;
					
					// rotate from terrain normal vector...
					normalrotquat = _XRotationArcNormalized( &upvector, &facenormal );
					D3DXMatrixRotationQuaternion( &realtimeobject->m_matWorldPosition, &normalrotquat );

					D3DXMatrixRotationY( &rotmat, _X_RAD( (FLOAT)(rand() % 360) ) );
					D3DXMatrixMultiply( &realtimeobject->m_matWorldPosition, &rotmat, &realtimeobject->m_matWorldPosition );
					
					realtimeobject->m_matWorldPosition._41 = ropos.x;
					realtimeobject->m_matWorldPosition._42 = ropos.y;
					realtimeobject->m_matWorldPosition._43 = ropos.z;
					
					if( pCurrentTile->m_pGrassAttribArray[ seedpos ] <= m_GrassManager.GetGrassTypeCount() )
						realtimeobject->m_ObjectIndex = pCurrentTile->m_pGrassAttribArray[ seedpos ]-1;
					else
						realtimeobject->m_ObjectIndex = m_GrassManager.GetGrassTypeCount()-1;
					
					m_GrassManager.InsertObject(realtimeobject);
				}
			}

			/*
			for( int splatindex = 0; splatindex < pCurrentTile->UsedSplatLayerCount; splatindex++ )
			{
				int grasstype = 1;//m_LayerInfo[pCurrentTile->SplatData[ splatindex ].m_LayerIndex].m_RTimeGrassType;
				if( grasstype > 0 )
				{
					srand(j + i * _XDEF_DEFAULTTILESIZE + splatindex);
					for( int limittrycount = 0; limittrycount < nGrassCount; ++ limittrycount)
					{
						FLOAT newxpos = (float)(rand() % 1600) / 100.f;
						FLOAT newzpos = (float)(rand() % 1600) / 100.f;
						
						int newxindex = _XFC_FloatToIntRet(newxpos) >> 1;
						int newzindex = _XFC_FloatToIntRet(newzpos) >> 1;
						
						if( pCurrentTile->SplatData[ splatindex ].m_BlendLevelArray[ newzindex + ((newxindex)<<3) ] > 12 )
						{
							realtimeobject = m_GrassManager.New_RealTimeObject();

							ropos.x = pCurrentTile->Center.x + newxpos - 8.0f;
							ropos.z = pCurrentTile->Center.z + newzpos - 8.0f;
							ropos.y = this->GetTerrainHeight( ropos, &facenormal ) - 0.05f;

							// rotate from terrain normal vector...
							normalrotquat = _XRotationArcNormalized( &upvector, &facenormal );
							D3DXMatrixRotationQuaternion( &realtimeobject->m_matWorldPosition, &normalrotquat );

							realtimeobject->m_matWorldPosition._41 = ropos.x;
							realtimeobject->m_matWorldPosition._42 = ropos.y;
							realtimeobject->m_matWorldPosition._43 = ropos.z;

							if( grasstype <= 3 )
								realtimeobject->m_ObjectIndex = grasstype-1;
							else
								realtimeobject->m_ObjectIndex = m_GrassManager.GetMeshCount()-1;

							realtimeobject->m_TilePos.x = j;
							realtimeobject->m_TilePos.y = i;

							m_GrassManager.InsertObject(realtimeobject);
						}
					}
				}
			}
			*/
		}
	}	
}

int CompareRealTimeObjectDistance(const void *pArg1, const void *pArg2)
{
	_XRealTimeObject** o1 = (_XRealTimeObject**)pArg1;
	_XRealTimeObject** o2 = (_XRealTimeObject**)pArg2;
	return ( (o1[0]->m_Distance < o2[0]->m_Distance) ? 1 : -1);	
}

void _XLodTerrain::RenderRealtimeObject( void )
{	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_ObjectAmbientColor );
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );

	if (g_bUseRealtimeGenerateObject)
		m_GrassManager.Render(m_3PCamera.m_CameraPosition);

	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
}

void _XLodTerrain::RebuildLevel( BOOL check )
{
	int	  xcoord = (_XFC_FloatToIntRet( ( g_pLocalUser->m_Position.x + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );
	int   zcoord = (_XFC_FloatToIntRet( ( g_pLocalUser->m_Position.z + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );

	if( xcoord < 0 || xcoord >= m_TileCount )
	{
		return;
	}
		
	if( zcoord < 0 || zcoord >= m_TileCount )
	{
		return; 
	}

	if( check ) 
	{	
		if( m_LastRebuildLevelTileX  == xcoord && 
			m_LastRebuildLevelTileZ  == zcoord && 
			fabs( m_LastRebuildLevelYAngle - m_3PCamera.GetYaw() ) < 10.0f )
		{			
			return;
		}	
	}

	static long lastcalculatetime = 0;
	if( g_LocalSystemTime - lastcalculatetime > 30 )
	{
		lastcalculatetime = g_LocalSystemTime;
	}
	else
	{

		return;
	}

	g_Userlist.CheckInsideFrustum();
	g_Npclist.CheckInsideFrustum();
	g_Moblist.CheckInsideFrustum();
	g_GameItemlist.CheckInsideFrustum();
	g_XST_Wrapper.CheckInsideFrustum();
	
	g_DummyList.CheckInsideFrustum();
	g_SeatList.CheckInsideFrustum();
	m_FunctionalObjectManager.CheckInsideFrustum();

	//extern void _XInsertSystemMessage( int messagemode, TCHAR* args, ... );
	//_XInsertSystemMessage( 0, "Rebuild terrain. [%d] [Check:%d]", g_LocalSystemTime, check );

#ifdef _XDWDEBUG	
	g_RebuildLevelCalledCount++;
	_PERFORMANCECHECKSTART	
#endif

	if (g_bUseRealtimeGenerateObject && m_GenerateRealTimeObject)
		GenerateGrassObject();

	RebuildRenderObjectStack();
		
	m_LastRebuildLevelTileX = xcoord;
	m_LastRebuildLevelTileZ = zcoord;
	m_LastRebuildLevelYAngle = m_3PCamera.GetYaw();

	// water object의 frustum을 Terrain의 frunstum check할때 같이 체크 해준다.
	int		baseindex = 0;
	FLOAT	mintiledistanceinfrustum = 9999.0f;
	m_pWaterObjectManager->m_WaterReflectionClipPlaneHeight = g_pLocalUser->m_Position.y;

	for( int layerindex = 0 ; layerindex < m_UsedLayerCount+3; layerindex++ )
		 m_TerrainTileRenderingCacheLength[layerindex] = 0;	
	
	
	_XLODTILE*	 pCurrentTile;
#ifdef _XDWDEBUG	
	if (QUAD_TREE == FALSE)
	{
	// choose detail levels for each tile
		for (int i=0;i<m_TileCount;++i)
		{
			for (int j=0;j<m_TileCount;++j)
			{						
				pCurrentTile = &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j];			

				pCurrentTile->DistanceToCamera = _XFastDistance( pCurrentTile->Center.x - g_pLocalUser->m_Position.x,
																 pCurrentTile->Center.z - g_pLocalUser->m_Position.z);
			
				if( CWater::m_WaterRenderingMode != _XWATER_RM_NORMAL ) 
				{
					if( mintiledistanceinfrustum > pCurrentTile->DistanceToCamera )
					{
						mintiledistanceinfrustum = pCurrentTile->DistanceToCamera;

						S_WATER* pWaterPiece = m_pWaterObjectManager->GetWaterObject(baseindex + j);
						if( pWaterPiece )
						{
							if( pWaterPiece->enabled )
							{
								m_pWaterObjectManager->m_WaterReflectionClipPlaneHeight = pWaterPiece->height;
							}
						}
					}
				}

				// Check inside frustum 
				if( pCurrentTile->DistanceToCamera <= m_LODFarPlane + 60.0f )
				{
					if( m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pCurrentTile->Center, 30.0f ) != NOT_VISIBLE )
					{
						// Insert tile rendering cache
						for( int splatlayer = 0; splatlayer < pCurrentTile->UsedSplatLayerCount; splatlayer++ )
						{
							int curlayerindex = pCurrentTile->SplatData[splatlayer].m_LayerIndex; // org layer index						
							m_TerrainTileRenderingCache[curlayerindex][m_TerrainTileRenderingCacheLength[curlayerindex]].xcoord = j;
							m_TerrainTileRenderingCache[curlayerindex][m_TerrainTileRenderingCacheLength[curlayerindex]].zcoord = i;
							m_TerrainTileRenderingCache[curlayerindex][m_TerrainTileRenderingCacheLength[curlayerindex]].layerindex = splatlayer; // layer stack index

							m_TerrainTileRenderingCacheLength[curlayerindex]++;
						}

						m_TerrainTileRenderingCache[m_UsedLayerCount][m_TerrainTileRenderingCacheLength[m_UsedLayerCount]].xcoord = j;
						m_TerrainTileRenderingCache[m_UsedLayerCount][m_TerrainTileRenderingCacheLength[m_UsedLayerCount]].zcoord = i;
						m_TerrainTileRenderingCache[m_UsedLayerCount][m_TerrainTileRenderingCacheLength[m_UsedLayerCount]].layerindex = 0;
						m_TerrainTileRenderingCacheLength[m_UsedLayerCount]++;

						// Rebuild detail level....
						if( pCurrentTile->DistanceToCamera < m_LODLevel3 )
						{
							pCurrentTile->DetailLevel = LEVEL_3;
						}
						else if ( pCurrentTile->DistanceToCamera < m_LODLevel2)
						{
							pCurrentTile->DetailLevel = LEVEL_2;
						}
						else if ( pCurrentTile->DistanceToCamera < m_LODLevel1)
						{
							pCurrentTile->DetailLevel = LEVEL_1;
						}
						else 
						{
							pCurrentTile->DetailLevel = LEVEL_0;
						}

						m_pWaterObjectManager->SetFrustum(baseindex + j, TRUE );
					}
					else 
					{
						pCurrentTile->DetailLevel = LEVEL_0;
						m_pWaterObjectManager->SetFrustum(baseindex + j, FALSE);
					}
				}
				else 
				{
					pCurrentTile->DetailLevel = LEVEL_0;
					m_pWaterObjectManager->SetFrustum(baseindex + j, FALSE);
				}
			}
			baseindex += m_TileCount;
		}	
	}
	else

#endif // QUAD_TREE

	{
		m_LastCheckedDistanceWaterpiece = 99999.0f;
		RebuildLevel_TileQuad();
	}

	for( int tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[m_UsedLayerCount]; tilerenderingcacheindex++ )
	{
		register int tileindexX = m_TerrainTileRenderingCache[m_UsedLayerCount][tilerenderingcacheindex].xcoord;
		register int tileindexZ = m_TerrainTileRenderingCache[m_UsedLayerCount][tilerenderingcacheindex].zcoord;

		pCurrentTile = &m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX];
		
		pCurrentTile->BodyTile = 0;
		LEVEL MyLevel = pCurrentTile->DetailLevel;
		
		if (tileindexZ && m_TerrainTile[m_TileStartOffset+tileindexZ-1][m_TileStartOffset+tileindexX].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<TOP;
		}
		
		// examine the tile below this tile							
		if (tileindexZ<(m_TileCount-1) && m_TerrainTile[m_TileStartOffset+tileindexZ+1][m_TileStartOffset+tileindexX].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<BOTTOM;
		}
		
		// examine the tile to the left this tile							
		if (tileindexX && m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX-1].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<LEFT;
		}
		
		// examine the tile to the right this tile
		if (tileindexX<(m_TileCount-1) && m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX+1].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<RIGHT;
		}
	}

	layerindex = m_UsedLayerCount+1;
	for( tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[layerindex]; tilerenderingcacheindex++ )
	{
		register int tileindexX	= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].xcoord;
		register int tileindexZ	= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].zcoord;
		
		pCurrentTile			= &m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX];

		pCurrentTile->BodyTile = 0;
		LEVEL MyLevel = pCurrentTile->DetailLevel;
		
		if (tileindexZ && m_TerrainTile[m_TileStartOffset+tileindexZ-1][m_TileStartOffset+tileindexX].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<TOP;
		}
		
		// examine the tile below this tile							
		if (tileindexZ<(m_TileCount-1) && m_TerrainTile[m_TileStartOffset+tileindexZ+1][m_TileStartOffset+tileindexX].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<BOTTOM;
		}
		
		// examine the tile to the left this tile							
		if (tileindexX && m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX-1].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<LEFT;
		}
		
		// examine the tile to the right this tile
		if (tileindexX<(m_TileCount-1) && m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX+1].DetailLevel < MyLevel)
		{
			pCurrentTile->BodyTile |= 1<<RIGHT;
		}
	}

	m_pWaterObjectManager->SetSelected( m_LastRebuildLevelTileX + (m_LastRebuildLevelTileZ*m_TileCount), TRUE, TRUE );	

#ifdef _XDWDEBUG
	_PERFORMANCECHECKEND	
	//TCHAR message[128];
	//sprintf( message, "Rebuild culling informations %f", timeinmicroseconds/1000000 );
	//_XDebugOutputString( message, 1, 3 );
#endif
}

//#endif

FLOAT _XLodTerrain::GetTerrainHeight( D3DXVECTOR3 pos, D3DXVECTOR3* normalvector )
{
	DWORD arrayx, arrayy, localx, localy;
	int tilex, tiley;

	// World coord.. clipping ...
	if( pos.x < -m_HalfWorldSize ) pos.x = -m_HalfWorldSize;
	if( pos.x >  m_HalfWorldSize ) pos.x =  m_HalfWorldSize-1.0f;
	
	if( pos.z < -m_HalfWorldSize ) pos.z = -m_HalfWorldSize;
	if( pos.z >  m_HalfWorldSize ) pos.z =  m_HalfWorldSize-1.0f;

	
	arrayx	=	(int)( ((pos.x + m_HalfWorldSize ) / m_GridRealSize) );
	arrayy	=	(int)( ((pos.z + m_HalfWorldSize ) / m_GridRealSize) );

	tilex	=	(arrayx >> 3);
	tiley	=	(arrayy >> 3);

	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tiley >= m_TileCount ) tiley = m_TileCount-1;

	localx  =   arrayx - (tilex<<3);
	localy  =   arrayy - (tiley<<3);

	if( localx > 7 ) localx = 7;
	if( localy > 7 ) localy = 7;
	
	LPWORD pIndex = NULL;
	D3DXVECTOR3 v0, v1, v2;

	LPWORD			pIndices	= g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][0]*3);		
	D3DXVECTOR3*	pData		= m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
	
	v0	= pData[*(pIndices  )];
	v1	= pData[*(pIndices+1)];
	v2	= pData[*(pIndices+2)];
			
	if( !IntersectTriangle( pos, g_DownNormalVector, v0, v1, v2 ) )	
	{
		pIndices = g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][1]*3);

		v0	= pData[*(pIndices  )];
		v1	= pData[*(pIndices+1)];
		v2	= pData[*(pIndices+2)];
	}

	D3DXVECTOR3 vLeft;
	D3DXVECTOR3 vRight;
	D3DXVECTOR3 vNormal;
	vLeft.x		= v1.x - v0.x;
	vLeft.y		= v1.y - v0.y;
	vLeft.z		= v1.z - v0.z;
	vRight.x	= v2.x - v0.x;
	vRight.y	= v2.y - v0.y;
	vRight.z	= v2.z - v0.z;	

	D3DXVec3Cross( &vNormal, &vLeft,  &vRight );

	D3DXVec3Identity( vNormal );

	if(normalvector)
	{
		*normalvector = vNormal;
	}
	
	FLOAT fD	= -D3DXVec3Dot( &vNormal, &v0 ); 
		
    return (-( vNormal.x * pos.x + vNormal.z * pos.z + fD ) / vNormal.y);	
}

FLOAT _XLodTerrain::GetTerrainAndOBBHeightWithCurrentHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB, D3DXVECTOR3& terrainnormal )
{
	DWORD arrayx, arrayy;
	int tilex, tiley;
	
	// World coord.. clipping ...
	if( pos.x < -m_HalfWorldSize ) pos.x = -m_HalfWorldSize;
	if( pos.x >  m_HalfWorldSize ) pos.x =  m_HalfWorldSize-1.0f;
	
	if( pos.z < -m_HalfWorldSize ) pos.z = -m_HalfWorldSize;
	if( pos.z >  m_HalfWorldSize ) pos.z =  m_HalfWorldSize-1.0f;
	
	
	arrayx	=	(int)( ((pos.x + m_HalfWorldSize ) / m_GridRealSize) );
	arrayy	=	(int)( ((pos.z + m_HalfWorldSize ) / m_GridRealSize) );
	
	tilex	=	(arrayx >> 3);
	tiley	=	(arrayy >> 3);
	
	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tiley >= m_TileCount ) tiley = m_TileCount-1;

	D3DXVECTOR3	collidpoint;
	D3DXVECTOR3	org = pos;		
	D3DXVECTOR3	ray = D3DXVECTOR3( 0.0f, -9999.0f, 0.0f );
	_XOBBData* pLastPickedOBB = NULL;
	org.y += (0.65f * 2.0f);
	
	FLOAT obbheight = -9999.0f;
	_XLODTILE* curtile = &m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley];
		
	if( curtile )
	{
		if( !curtile->svObbList.empty() )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = curtile->svObbList.begin(); it != curtile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				//if( worldobbdataitem->m_pOBBdata->m_OBBLevel == 0 )
				{
					int collideface = pObbData->InterOBBvsRay( org, ray, collidpoint );
					if( collideface > 0  )
					{							
						if( obbheight < collidpoint.y )
						{
							obbheight  = collidpoint.y;
							pLastPickedOBB = pObbData;
						}
					}					
				}
			}
		}
	}	


	DWORD localx, localy;
	localx  =   arrayx - (tilex<<3);
	localy  =   arrayy - (tiley<<3);
	
	if( localx > 7 ) localx = 7;
	if( localy > 7 ) localy = 7;
	
	LPWORD pIndex = NULL;
	D3DXVECTOR3 v0, v1, v2;
	FLOAT terrainheight = -9999.0f;
	
	// 지형의 충돌을 체크하기 위해 Ray normalize
	pos.y = 999.0f;
	//pos.y = pos.y + _XDEF_RIDEOBBLIMITHEIGHT;

	LPWORD		 pIndices	= g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][0]*3);	
	D3DXVECTOR3* pData		= m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
	
	v0	= pData[*(pIndices  )];
	v1	= pData[*(pIndices+1)];
	v2	= pData[*(pIndices+2)];
	
	if( !IntersectTriangle( pos, g_DownNormalVector, v0, v1, v2 ) )	
	{
		pIndices = g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][1]*3);
		
		v0	= pData[*(pIndices  )];
		v1	= pData[*(pIndices+1)];
		v2	= pData[*(pIndices+2)];
	}
	
	D3DXVECTOR3 vLeft, vRight, vNormal;
	vLeft.x		= v1.x - v0.x;
	vLeft.y		= v1.y - v0.y;
	vLeft.z		= v1.z - v0.z;
	vRight.x	= v2.x - v0.x;
	vRight.y	= v2.y - v0.y;
	vRight.z	= v2.z - v0.z;	
	
	D3DXVec3Cross( &vNormal, &vLeft,  &vRight );
	D3DXVec3Identity( vNormal );
	terrainnormal = vNormal;
	terrainheight = (-( vNormal.x * pos.x + vNormal.z * pos.z + (-D3DXVec3Dot( &vNormal, &v0 ) ) ) / vNormal.y);	
	

	if( pLastPickedOBB )
	{	
		if( terrainheight > obbheight ) // 지형의 높이가 OBB보다 높으면 지형을 리턴
		{
			pOBB = NULL;
			return terrainheight;
		}
		else
		{
			pOBB = pLastPickedOBB;
			terrainnormal = D3DXVECTOR3( 0.0f,1.0f,0.0f );
		}
	}
	else // OBB가 선택되지 않은경우
	{
		pOBB = NULL;
		return terrainheight;		
	}

	return obbheight;
}

FLOAT _XLodTerrain::GetTerrainAndRideOBBHeight( D3DXVECTOR3 pos, D3DXVECTOR3* normalvector )
{
	DWORD arrayx, arrayy;
	int tilex, tiley;
	
	// World coord.. clipping ...
	if( pos.x < -m_HalfWorldSize ) pos.x = -m_HalfWorldSize;
	if( pos.x >  m_HalfWorldSize ) pos.x =  m_HalfWorldSize-1.0f;
	
	if( pos.z < -m_HalfWorldSize ) pos.z = -m_HalfWorldSize;
	if( pos.z >  m_HalfWorldSize ) pos.z =  m_HalfWorldSize-1.0f;
	
	
	arrayx	=	(int)( ((pos.x + m_HalfWorldSize ) / m_GridRealSize) );
	arrayy	=	(int)( ((pos.z + m_HalfWorldSize ) / m_GridRealSize) );
	
	tilex	=	(arrayx >> 3);
	tiley	=	(arrayy >> 3);
	
	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tiley >= m_TileCount ) tiley = m_TileCount-1;

	D3DXVECTOR3	collidpoint;
	D3DXVECTOR3	org = pos;		
	D3DXVECTOR3	ray = D3DXVECTOR3( 0.0f, -2000.0f, 0.0f );
	_XOBBData* pLastPickedOBB = NULL;
	org.y = 1500.0f;
		
	FLOAT obbheight = -9999.0f;
	_XLODTILE* curtile = &m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley];	
	
	if( curtile )
	{
		if( !curtile->svObbList.empty() )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = curtile->svObbList.begin(); it != curtile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				if( pObbData->m_OBBLevel == 0 )
				{
					int collideface = pObbData->InterOBBvsRay( org, ray, collidpoint );
					if( collideface > 0  )
					{							
						if( obbheight < collidpoint.y )
						{
							obbheight  = collidpoint.y;
							pLastPickedOBB = pObbData;
						}
					}
				}
			}
		}
	}	

	DWORD localx, localy;
	localx  =   arrayx - (tilex<<3);
	localy  =   arrayy - (tiley<<3);
	
	if( localx > 7 ) localx = 7;
	if( localy > 7 ) localy = 7;

	LPWORD pIndex = NULL;
	D3DXVECTOR3 v0, v1, v2;
	FLOAT terrainheight = -9999.0f;

	// 지형의 충돌을 체크하기 위해 Ray normalize
	pos.y = 999.0f;
	
	LPWORD			pIndices	= g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][0]*3);
	D3DXVECTOR3*	pData		= m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
	
	v0	= pData[*(pIndices  )];
	v1	= pData[*(pIndices+1)];
	v2	= pData[*(pIndices+2)];
	
	if( !IntersectTriangle( pos, g_DownNormalVector, v0, v1, v2 ) )	
	{
		pIndices = g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][1]*3);
		
		v0	= pData[*(pIndices  )];
		v1	= pData[*(pIndices+1)];
		v2	= pData[*(pIndices+2)];
	}
		
	D3DXVECTOR3 vLeft, vRight, vNormal;
	vLeft.x		= v1.x - v0.x;
	vLeft.y		= v1.y - v0.y;
	vLeft.z		= v1.z - v0.z;
	vRight.x	= v2.x - v0.x;
	vRight.y	= v2.y - v0.y;
	vRight.z	= v2.z - v0.z;	
	
	D3DXVec3Cross( &vNormal, &vLeft,  &vRight );
	
	D3DXVec3Identity( vNormal );

	if(normalvector)
	{
		*normalvector = vNormal;
	}

	terrainheight = (-( vNormal.x * pos.x + vNormal.z * pos.z + (-D3DXVec3Dot( &vNormal, &v0 ) ) ) / vNormal.y);

	if( pLastPickedOBB )
	{	
		if( terrainheight > obbheight ) // 지형의 높이가 OBB보다 높으면 지형을 리턴
		{
			return terrainheight;
		}
	}
	else // OBB가 선택되지 않은경우
	{
		return terrainheight;
	}

	return obbheight;
}

FLOAT _XLodTerrain::GetTerrainAndRideOBBHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB, D3DXVECTOR3* normalvector )
{
	DWORD arrayx, arrayy;
	int tilex, tiley;
	
	// World coord.. clipping ...
	if( pos.x < -m_HalfWorldSize ) pos.x = -m_HalfWorldSize;
	if( pos.x >  m_HalfWorldSize ) pos.x =  m_HalfWorldSize-1.0f;
	
	if( pos.z < -m_HalfWorldSize ) pos.z = -m_HalfWorldSize;
	if( pos.z >  m_HalfWorldSize ) pos.z =  m_HalfWorldSize-1.0f;
	
	
	arrayx	=	(int)( ((pos.x + m_HalfWorldSize ) / m_GridRealSize) );
	arrayy	=	(int)( ((pos.z + m_HalfWorldSize ) / m_GridRealSize) );
	
	tilex	=	(arrayx >> 3);
	tiley	=	(arrayy >> 3);
	
	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tiley >= m_TileCount ) tiley = m_TileCount-1;

	D3DXVECTOR3	collidpoint;
	D3DXVECTOR3	org = pos;		
	D3DXVECTOR3	ray = D3DXVECTOR3( 0.0f, -2000.0f, 0.0f );
	_XOBBData* pLastPickedOBB = NULL;
	org.y = 1500.0f;
		
	FLOAT obbheight = -9999.0f;
	_XLODTILE* curtile = &m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley];	
	
	if( curtile )
	{
		if( !curtile->svObbList.empty() )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = curtile->svObbList.begin(); it != curtile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				if( pObbData->m_OBBLevel == 0 )
				{
					int collideface = pObbData->InterOBBvsRay( org, ray, collidpoint );
					
					if( collideface > 0  )
					{							
						if( obbheight < collidpoint.y )
						{
							obbheight  = collidpoint.y;
							pLastPickedOBB = pObbData;
						}
					}
				}
			}
		}
	}	

	DWORD localx, localy;
	localx  =   arrayx - (tilex<<3);
	localy  =   arrayy - (tiley<<3);
	
	if( localx > 7 ) localx = 7;
	if( localy > 7 ) localy = 7;

	LPWORD pIndex = NULL;
	D3DXVECTOR3 v0, v1, v2;
	FLOAT terrainheight = -9999.0f;

	// 지형의 충돌을 체크하기 위해 Ray normalize
	pos.y = 999.0f;
	
	LPWORD			pIndices	= g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][0]*3);
	D3DXVECTOR3*	pData		= m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
	
	v0	= pData[*(pIndices  )];
	v1	= pData[*(pIndices+1)];
	v2	= pData[*(pIndices+2)];
	
	if( !IntersectTriangle( pos, g_DownNormalVector, v0, v1, v2 ) )	
	{
		pIndices = g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][1]*3);
		
		v0	= pData[*(pIndices  )];
		v1	= pData[*(pIndices+1)];
		v2	= pData[*(pIndices+2)];
	}
		
	D3DXVECTOR3 vLeft, vRight, vNormal;
	vLeft.x		= v1.x - v0.x;
	vLeft.y		= v1.y - v0.y;
	vLeft.z		= v1.z - v0.z;
	vRight.x	= v2.x - v0.x;
	vRight.y	= v2.y - v0.y;
	vRight.z	= v2.z - v0.z;	
	
	D3DXVec3Cross( &vNormal, &vLeft,  &vRight );
	D3DXVec3Identity( vNormal );
	if(normalvector)
	{
		*normalvector = vNormal;
	}

	terrainheight = (-( vNormal.x * pos.x + vNormal.z * pos.z + (-D3DXVec3Dot( &vNormal, &v0 ) ) ) / vNormal.y);	

	if( pLastPickedOBB )
	{	
		if( terrainheight > obbheight ) // 지형의 높이가 OBB보다 높으면 지형을 리턴
		{
			pOBB = NULL;
			return terrainheight;
		}
		else
		{
			pOBB = pLastPickedOBB;
		}
	}
	else // OBB가 선택되지 않은경우
	{
		pOBB = NULL;
		return terrainheight;
	}

	return obbheight;
}

FLOAT _XLodTerrain::GetTerrainOBBHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB, FLOAT& terraincollideheight )
{
	DWORD arrayx, arrayy;
	int tilex, tiley;
	
	// World coord.. clipping ...
	if( pos.x < -m_HalfWorldSize ) pos.x = -m_HalfWorldSize;
	if( pos.x >  m_HalfWorldSize ) pos.x =  m_HalfWorldSize-1.0f;
	
	if( pos.z < -m_HalfWorldSize ) pos.z = -m_HalfWorldSize;
	if( pos.z >  m_HalfWorldSize ) pos.z =  m_HalfWorldSize-1.0f;
		
	arrayx	=	(int)( ((pos.x + m_HalfWorldSize ) / m_GridRealSize) );
	arrayy	=	(int)( ((pos.z + m_HalfWorldSize ) / m_GridRealSize) );
	
	tilex	=	(arrayx >> 3);
	tiley	=	(arrayy >> 3);
	
	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tiley >= m_TileCount ) tiley = m_TileCount-1;

	D3DXVECTOR3	collidpoint;
	D3DXVECTOR3	org = pos;		
	D3DXVECTOR3	ray = D3DXVECTOR3( 0.0f, -9999.0f, 0.0f );
	_XOBBData* pLastPickedOBB = NULL;
	org.y += (0.65f * 2.0f);
		
	FLOAT obbheight = -9999.0f;
	_XLODTILE* curtile = &m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley];	
	
	if( curtile )
	{
		if( !curtile->svObbList.empty() )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = curtile->svObbList.begin(); it != curtile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				if( pObbData->m_OBBLevel == 0 )
				{
					int collideface = pObbData->InterOBBvsRay( org, ray, collidpoint );
					
					if( collideface > 0  )
					{							
						if( obbheight < collidpoint.y )
						{
							obbheight  = collidpoint.y;
							pLastPickedOBB = pObbData;
						}
					}
				}
			}
		}
	}	

	DWORD localx, localy;
	localx  =   arrayx - (tilex<<3);
	localy  =   arrayy - (tiley<<3);
	
	if( localx > 7 ) localx = 7;
	if( localy > 7 ) localy = 7;

	LPWORD pIndex = NULL;
	D3DXVECTOR3 v0, v1, v2;
	FLOAT terrainheight = -9999.0f;

	// 지형의 충돌을 체크하기 위해 Ray normalize
	pos.y = 999.0f;
	
	LPWORD			pIndices	= g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][0]*3);
	D3DXVECTOR3*	pData		= m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
	
	v0	= pData[*(pIndices  )];
	v1	= pData[*(pIndices+1)];
	v2	= pData[*(pIndices+2)];
	
	if( !IntersectTriangle( pos, g_DownNormalVector, v0, v1, v2 ) )	
	{
		pIndices = g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][1]*3);
		
		v0	= pData[*(pIndices  )];
		v1	= pData[*(pIndices+1)];
		v2	= pData[*(pIndices+2)];
	}
		
	D3DXVECTOR3 vLeft, vRight, vNormal;
	vLeft.x		= v1.x - v0.x;
	vLeft.y		= v1.y - v0.y;
	vLeft.z		= v1.z - v0.z;
	vRight.x	= v2.x - v0.x;
	vRight.y	= v2.y - v0.y;
	vRight.z	= v2.z - v0.z;	
	
	D3DXVec3Cross( &vNormal, &vLeft,  &vRight );
	D3DXVec3Identity( vNormal );
	terrainheight = (-( vNormal.x * pos.x + vNormal.z * pos.z + (-D3DXVec3Dot( &vNormal, &v0 ) ) ) / vNormal.y);	

	terraincollideheight = terrainheight;

	if( pLastPickedOBB )
	{	
		if( terrainheight > obbheight ) // 지형의 높이가 OBB보다 높으면 지형을 리턴
		{
			pOBB = NULL;
			return terrainheight;
		}
		else
		{
			pOBB = pLastPickedOBB;
		}
	}
	else // OBB가 선택되지 않은경우
	{
		pOBB = NULL;
		return terrainheight;
	}

	return obbheight;
}

void _XLodTerrain::GetTerrainQuadPolygon( int arrayx, int arrayz, D3DXVECTOR3* pVertices )
{
	int tilex, tiley;
	tilex	=	(arrayx >> 3);
	tiley	=	(arrayz >> 3);	
	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tiley >= m_TileCount ) tiley = m_TileCount-1;

	DWORD localx, localy;
	localx  =   arrayx - (tilex<<3);
	localy  =   arrayz - (tiley<<3);
	
	if( localx > 7 ) localx = 7;
	if( localy > 7 ) localy = 7;

	LPWORD pIndex = NULL;
			
	LPWORD			pIndices	= g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][0]*3);
	D3DXVECTOR3*	pData		= m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
	
	pVertices[0]	= pData[*(pIndices  )];
	pVertices[1]    = pData[*(pIndices+1)];
	pVertices[2]    = pData[*(pIndices+2)];
	
	pIndices = g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][1]*3);
	pVertices[3]	= pData[*(pIndices  )];
	pVertices[4]	= pData[*(pIndices+1)];
	pVertices[5]	= pData[*(pIndices+2)];
}

FLOAT _XLodTerrain::GetTerrainAndOBBHeight( D3DXVECTOR3 pos, _XOBBData*& pOBB )
{
	DWORD arrayx, arrayy;
	int tilex, tiley;
	
	// World coord.. clipping ...
	if( pos.x < -m_HalfWorldSize ) pos.x = -m_HalfWorldSize;
	if( pos.x >  m_HalfWorldSize ) pos.x =  m_HalfWorldSize-1.0f;
	
	if( pos.z < -m_HalfWorldSize ) pos.z = -m_HalfWorldSize;
	if( pos.z >  m_HalfWorldSize ) pos.z =  m_HalfWorldSize-1.0f;
	
	
	arrayx	=	(int)( ((pos.x + m_HalfWorldSize ) / m_GridRealSize) );
	arrayy	=	(int)( ((pos.z + m_HalfWorldSize ) / m_GridRealSize) );
	
	tilex	=	(arrayx >> 3);
	tiley	=	(arrayy >> 3);
	
	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tiley >= m_TileCount ) tiley = m_TileCount-1;

	D3DXVECTOR3	collidpoint;
	D3DXVECTOR3	org = pos;		
	D3DXVECTOR3	ray = D3DXVECTOR3( 0.0f, -2000.0f, 0.0f );
	_XOBBData* pLastPickedOBB = NULL;
	org.y = 1500.0f;
		
	FLOAT obbheight = -9999.0f;
	_XLODTILE* curtile = &m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley];	
	
	if( curtile )
	{
		if( !curtile->svObbList.empty() )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = curtile->svObbList.begin(); it != curtile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				//if( pObbData->m_OBBLevel == 0 )
				{
					int collideface = pObbData->InterOBBvsRay( org, ray, collidpoint );
					
					if( collideface > 0  )
					{							
						if( obbheight < collidpoint.y )
						{
							obbheight  = collidpoint.y;
							pLastPickedOBB = pObbData;
						}
					}
				}
			}
		}
	}	

	DWORD localx, localy;
	localx  =   arrayx - (tilex<<3);
	localy  =   arrayy - (tiley<<3);
	
	if( localx > 7 ) localx = 7;
	if( localy > 7 ) localy = 7;

	LPWORD pIndex = NULL;
	D3DXVECTOR3 v0, v1, v2;
	FLOAT terrainheight = -9999.0f;

	// 지형의 충돌을 체크하기 위해 Ray normalize
	pos.y = 999.0f;
	
	LPWORD			pIndices	= g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][0]*3);
	D3DXVECTOR3*	pData		= m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
	
	v0	= pData[*(pIndices  )];
	v1	= pData[*(pIndices+1)];
	v2	= pData[*(pIndices+2)];
	
	if( !IntersectTriangle( pos, g_DownNormalVector, v0, v1, v2 ) )	
	{
		pIndices = g_TerrainIndexLUT + (VertexIndexLUT[localx][localy][1]*3);
		
		v0	= pData[*(pIndices  )];
		v1	= pData[*(pIndices+1)];
		v2	= pData[*(pIndices+2)];
	}
		
	D3DXVECTOR3 vLeft, vRight, vNormal;
	vLeft.x		= v1.x - v0.x;
	vLeft.y		= v1.y - v0.y;
	vLeft.z		= v1.z - v0.z;
	vRight.x	= v2.x - v0.x;
	vRight.y	= v2.y - v0.y;
	vRight.z	= v2.z - v0.z;	
	
	D3DXVec3Cross( &vNormal, &vLeft,  &vRight );
	D3DXVec3Identity( vNormal );
	terrainheight = (-( vNormal.x * pos.x + vNormal.z * pos.z + (-D3DXVec3Dot( &vNormal, &v0 ) ) ) / vNormal.y);	

	if( pLastPickedOBB )
	{	
		if( terrainheight > obbheight ) // 지형의 높이가 OBB보다 높으면 지형을 리턴
		{
			pOBB = NULL;
			return terrainheight;
		}
		else
		{
			pOBB = pLastPickedOBB;
		}
	}
	else // OBB가 선택되지 않은경우
	{
		pOBB = NULL;
		return terrainheight;
	}

	return obbheight;
}

void _XLodTerrain::GetTerrainDecal(int& nVertexCount, int& nIndexCount, D3DXVECTOR3& pos, int nShadowTile, IDirect3DVertexBuffer9** ppVertexBuffer,IDirect3DIndexBuffer9** ppIndexBuffer )
{
	if( !ppVertexBuffer || !ppIndexBuffer ) return;

	int nStartX, nStartZ, nEndX, nEndZ;

	int arrayx, arrayy;
	int localx, localy;
	int tilex, tiley;

	arrayx = (pos.x + m_HalfWorldSize) / m_GridRealSize;
	arrayy = (pos.z + m_HalfWorldSize) / m_GridRealSize;

	nStartX = arrayx - nShadowTile;
	nStartZ = arrayy - nShadowTile;
	nEndX = arrayx + nShadowTile + 1;
	nEndZ = arrayy + nShadowTile + 1;

	if (nStartX < 0) nStartX = 0;
	if (nStartZ < 0) nStartZ = 0;

	if (nEndX > m_VertexCount) nEndX = m_VertexCount;
	if (nEndZ > m_VertexCount) nEndZ = m_VertexCount;

	nVertexCount = 0;
	nIndexCount = 0;

	int nVertexWidthCount = nEndX - nStartX + 1;

	D3DXVECTOR3* pData;
	D3DXVECTOR3* pDestData;
	LPWORD pDestIndex = NULL;
	if (SUCCEEDED((*ppIndexBuffer)->Lock(0, 0, (void**)&pDestIndex, g_3DObjectLockMethods)))
	{		
		if (SUCCEEDED((*ppVertexBuffer)->Lock(0,0, (void**)&pDestData, g_3DObjectLockMethods)))
		{
			for(int z = nStartZ; z <= nEndZ; ++ z)
			{
				for(int x = nStartX; x <= nEndX; ++ x)
				{
					tilex =	x >> 3;
					tiley =	z >> 3;

					localx = x - (tilex << 3);
					localy = z - (tiley << 3);

					pData = m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;

					int prevacc = localy + (localx<<3)+localx;
					*pDestData = pData[prevacc];
					++ pDestData;

					if (x < nEndX && z < nEndZ)
					{
						if( (localx < 4 && localy < 4) || (localx > 3 && localy > 3) )
						{
							*pDestIndex = (((z-nStartZ) + 0) * nVertexWidthCount) + ((x-nStartX) + 0);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 1) * nVertexWidthCount) + ((x-nStartX) + 0);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 1) * nVertexWidthCount) + ((x-nStartX) + 1);	++pDestIndex;

							*pDestIndex = (((z-nStartZ) + 0) * nVertexWidthCount) + ((x-nStartX) + 0);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 1) * nVertexWidthCount) + ((x-nStartX) + 1);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 0) * nVertexWidthCount) + ((x-nStartX) + 1);	++pDestIndex;
						}
						else
						{
							*pDestIndex = (((z-nStartZ) + 0) * nVertexWidthCount) + ((x-nStartX) + 1);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 0) * nVertexWidthCount) + ((x-nStartX) + 0);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 1) * nVertexWidthCount) + ((x-nStartX) + 0);	++pDestIndex;

							*pDestIndex = (((z-nStartZ) + 0) * nVertexWidthCount) + ((x-nStartX) + 1);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 1) * nVertexWidthCount) + ((x-nStartX) + 0);	++pDestIndex;
							*pDestIndex = (((z-nStartZ) + 1) * nVertexWidthCount) + ((x-nStartX) + 1);	++pDestIndex;
						}
						nIndexCount += 6;
					}
					++ nVertexCount;
				}
			}
			(*ppVertexBuffer)->Unlock();
		}
		(*ppIndexBuffer)->Unlock();
	}
}
/*
void _XLodTerrain::GetTerrainDecal( D3DXVECTOR3 pos, IDirect3DVertexBuffer9*& VBuffer,IDirect3DIndexBuffer9*& phIndexBuffer )
{
	if( !VBuffer || !phIndexBuffer ) return;

	DWORD arrayx, arrayy;
	DWORD localx, localy;
	int   tilex, tiley;

	FLOAT selecttilexpos,selecttilezpos;
				
	LPWORD pDestIndex = NULL;
	if (SUCCEEDED(phIndexBuffer->Lock(0, 108, (void**)&pDestIndex, g_3DObjectLockMethods)))
	{		
		selecttilexpos = pos.x - m_GridRealSize;
		selecttilezpos = pos.z - m_GridRealSize;
				
		D3DXVECTOR3 *pDestData;
		if (SUCCEEDED(VBuffer->Lock(0,0, (void**)&pDestData, g_3DObjectLockMethods)))
		{
			D3DXVECTOR3*	pData;
			BOOL			leftsidepoly	= FALSE;
			int				indexstpos		= 0;
			int				vertexstpos		= 0;

			for( int i = 0; i < 3; i++ )
			{
				for( int j = 0; j < 3; j++ )
				{
					// World coord.. clipping ...
					if( selecttilexpos < -m_HalfWorldSize ) selecttilexpos = -m_HalfWorldSize;
					if( selecttilexpos >  m_HalfWorldSize ) selecttilexpos =  m_HalfWorldSize-1.0f;
					if( selecttilezpos < -m_HalfWorldSize ) selecttilezpos = -m_HalfWorldSize;
					if( selecttilezpos >  m_HalfWorldSize ) selecttilezpos =  m_HalfWorldSize-1.0f;

					arrayx	=	(int)( ((selecttilexpos + m_HalfWorldSize ) / m_GridRealSize) );
					arrayy	=	(int)( ((selecttilezpos + m_HalfWorldSize ) / m_GridRealSize) );

					tilex	=	(arrayx >> 3);
					tiley	=	(arrayy >> 3);

					if( tilex >= m_TileCount ) tilex = m_TileCount-1;
					if( tiley >= m_TileCount ) tiley = m_TileCount-1;

					localx  =   arrayx - (tilex<<3);
					localy  =   arrayy - (tiley<<3);
					
					if( localx > 7 ) localx = 7;
					if( localy > 7 ) localy = 7;
					
					if( (localx < 4 && localy < 4) || (localx > 3 && localy > 3) ) leftsidepoly = TRUE;
					else leftsidepoly = FALSE;

					pData = m_TerrainTile[m_TileStartOffset+tilex][m_TileStartOffset+tiley].VertexBuffer;
					
					int prevacc = localy + (localx<<3)+localx;
					pDestData[vertexstpos  ] = pData[ prevacc       ];
					pDestData[vertexstpos+1] = pData[ prevacc+1     ];

					prevacc = localy + ((localx+1)<<3)+(localx+1);
					pDestData[vertexstpos+2] = pData[ prevacc		];
					pDestData[vertexstpos+3] = pData[ prevacc+1		];
					

// testy
					pDestData[vertexstpos  ].y += 0.2f;
					pDestData[vertexstpos+1].y += 0.2f;
					pDestData[vertexstpos+2].y += 0.2f;
					pDestData[vertexstpos+3].y += 0.2f;


					if( leftsidepoly )
					{
						pDestIndex[indexstpos  ] = vertexstpos+2;
						pDestIndex[indexstpos+1] = vertexstpos;
						pDestIndex[indexstpos+2] = vertexstpos+3;
						
						pDestIndex[indexstpos+3] = vertexstpos+3;
						pDestIndex[indexstpos+4] = vertexstpos;
						pDestIndex[indexstpos+5] = vertexstpos+1;					
					}
					else
					{
						pDestIndex[indexstpos  ] = vertexstpos+2;
						pDestIndex[indexstpos+1] = vertexstpos+1;
						pDestIndex[indexstpos+2] = vertexstpos+3;
						
						pDestIndex[indexstpos+3] = vertexstpos+2;	
						pDestIndex[indexstpos+4] = vertexstpos;
						pDestIndex[indexstpos+5] = vertexstpos+1;
					}

					indexstpos+=6;
					vertexstpos+=4;
					selecttilexpos += m_GridRealSize;
				}
				
				selecttilexpos = pos.x - m_GridRealSize;
				//selecttilexpos -= (m_GridRealSize*3.0f);
				selecttilezpos += m_GridRealSize;
			}

			VBuffer->Unlock();
		}

		phIndexBuffer->Unlock();
	}
}
*/

_XLODTILE* _XLodTerrain::GetTerrainTile(  int x, int z )
{
	if( x < 0 || x >= m_TileCount ) return NULL;
	if( z < 0 || z >= m_TileCount ) return NULL;	

	return &(m_TerrainTile[m_TileStartOffset+x][m_TileStartOffset+z]);
}

_XLODTILE* _XLodTerrain::GetTerrainTile(  D3DXVECTOR3 pos, POINT* tilecoord )
{		
	int	  xcoord = (_XFC_FloatToIntRet( ( pos.x + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );
	int   zcoord = (_XFC_FloatToIntRet( ( pos.z + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );

	if( xcoord < 0 || xcoord >= m_TileCount ) return NULL;
	if( zcoord < 0 || zcoord >= m_TileCount ) return NULL;

	if( tilecoord )
	{
		tilecoord->x = xcoord;
		tilecoord->y = zcoord;
	}

	return &(m_TerrainTile[m_TileStartOffset+xcoord][m_TileStartOffset+zcoord]);	
}

int _XLodTerrain::GetTerrainTileIndex(D3DXVECTOR3 pos)
{		
	int	  xcoord = (_XFC_FloatToIntRet( ( pos.x + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );
	int   zcoord = (_XFC_FloatToIntRet( ( pos.z + m_HalfWorldSize ) / m_GridRealSize ) >> 3 );
	
	if( xcoord < 0 || xcoord >= m_TileCount ) return -1;
	if( zcoord < 0 || zcoord >= m_TileCount ) return -1;
	
	return xcoord + zcoord*m_TileCount;	
}

void _XLodTerrain::DrawReplectTerrain( void )
{
	// Draw reflect skybox
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );

	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 17 );
	gpDev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	gpDev->SetLight( 0, &g_EnvironmentManager.m_TerrainLight );
	gpDev->LightEnable( 0, TRUE );

	if( g_EnvironmentManager.m_SkyBoxHeightOffset < 1000.0f )
	{
		g_EnvironmentManager.Draw( TRUE );
	}
	
	if( CWater::m_UseObjectReflection && CWater::m_UseTerrainReflection )
	{
		// Draw reflect terrain
		gpDev->SetTransform( D3DTS_WORLD, &m_TerrainMatrix );
		
		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 18 );

		gpDev->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1  );
		gpDev->SetRenderState( D3DRS_COLORVERTEX , TRUE );	
		
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_TerrainAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_TerrainLight );
		gpDev->LightEnable( 0, TRUE );

		gpDev->SetFVF(_XTERRAIN_FVF);
		
		register int tileindexX;
		register int tileindexZ;
		register int splatstacklayerindex;
		_XLODTILE*	 pCurrentTile;

		/*
		for( int splatlayer = 0; splatlayer < m_UsedLayerCount; splatlayer++ )
		{		
		// Set splat infomations...
		gpDev->SetTransform( D3DTS_TEXTURE0, &m_LayerInfo[splatlayer].m_DiffuseTextureMatrix );
		g_WorldObjectTextureArchive.SetTexture_NoRenderStyle( m_LayerInfo[splatlayer].m_DetailTextureIndex, 0 );
		
		  // render each tile
		  for( int tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[splatlayer]; tilerenderingcacheindex++ )
		  {
		  tileindexX				= m_TerrainTileRenderingCache[splatlayer][tilerenderingcacheindex].xcoord;
		  tileindexZ				= m_TerrainTileRenderingCache[splatlayer][tilerenderingcacheindex].zcoord;
		  splatstacklayerindex	= m_TerrainTileRenderingCache[splatlayer][tilerenderingcacheindex].layerindex;
		  pCurrentTile			= &m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX];
		  
			if( !pCurrentTile->RenderReflectionMap ) continue;
			
			  // activate the vertex buffer for this tile
			  LEVEL MyLevel = pCurrentTile->DetailLevel;			
			  //if( MyLevel == LEVEL_3 ) MyLevel = LEVEL_2;
			  
		 */
		
		// render each reflection tile
		int layerindex = m_UsedLayerCount+2;

		for( int tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[layerindex]; tilerenderingcacheindex++ )
		{
			tileindexX				= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].xcoord;
			tileindexZ				= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].zcoord;
			splatstacklayerindex	= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].layerindex;
			pCurrentTile			= &m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX];

			// activate the vertex buffer for this tile
			LEVEL MyLevel = pCurrentTile->DetailLevel;
			
			if (SUCCEEDED(gpDev->SetStreamSource(0,pCurrentTile->VBuffer, 0, sizeof(TERRAIN_VERTEX))))
			{
				gpDev->SetTexture( 0, pCurrentTile->m_pPreRenderedTextureObject );

				// examine the tile above this tile
				if( pCurrentTile->BodyTile )
				{
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_TOP )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ-1][m_TileStartOffset+tileindexX].DetailLevel;
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].TriangleCount);
						}
					}
					
					// examine the tile below this tile							
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_BOTTOM )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ+1][m_TileStartOffset+tileindexX].DetailLevel;
						
						// draw the connecting piece needed
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].TriangleCount);
						}							
					}
					
					// examine the tile to the left this tile							
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_LEFT )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX-1].DetailLevel;
						// draw the connecting piece needed
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].TriangleCount);
						}	
					}
					
					// examine the tile to the right this tile
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_RIGHT )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX+1].DetailLevel;
						// draw the connecting piece needed
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].TriangleCount);
						}							
					}
				}

				// finally, draw the body tile needed
				if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].phIndexBuffer)))
				{
					gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount);
				}
			}
		}
				
		gpDev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		gpDev->SetRenderState( D3DRS_COLORVERTEX , FALSE );
		gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSU,	D3DTADDRESS_WRAP);
		gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSV,	D3DTADDRESS_WRAP);
	}
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
}

void _XLodTerrain::DrawTerrain()
{
	gpDev->SetTransform( D3DTS_WORLD, &m_TerrainMatrix );
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
		
	/*
	if( g_bCloudShadow ) 
	{		
		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_TERRAIN );
	}	
	else */
	{
		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_TERRAIN_NOSHADOW );
	}

	
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
	m_FacesDrawn = 0;
#endif
		
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
	_PERFORMANCECHECKSTART
#endif

	//gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSU,	D3DTADDRESS_CLAMP);
	//gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSV,	D3DTADDRESS_CLAMP);

	gpDev->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1  );
	gpDev->SetRenderState( D3DRS_COLORVERTEX , TRUE );	
	gpDev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_TerrainAmbientColor );
	gpDev->SetLight( 0, &g_EnvironmentManager.m_TerrainLight );
	gpDev->LightEnable( 0, TRUE );

	gpDev->SetFVF(_XTERRAIN_FVF);
	
#ifndef _XDEF_USETERRAINUV
	D3DXMATRIX InvViewMatrix;
	FLOAT	   fDeterminantViewMatrix = D3DXMatrixDeterminant(&m_3PCamera.GetViewMatrix());
	D3DXMatrixInverse( &InvViewMatrix, &fDeterminantViewMatrix, &m_3PCamera.GetViewMatrix() );
	
	D3DXMATRIX UVScaleMatrix;	
	memset( &UVScaleMatrix, 0, sizeof(D3DXMATRIX) );
	UVScaleMatrix._11=1/(16.0f);
	UVScaleMatrix._32=1/(16.0f);
	
	D3DXMATRIX UVTextureMatrix;
	D3DXMatrixMultiply(&UVTextureMatrix, &InvViewMatrix, &UVScaleMatrix);	
	
	gpDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
	gpDev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	//D3DXMATRIX rotmat;
	//D3DXMatrixRotationZ( &rotmat, _X_RAD(-90.0f) );
	//D3DXMatrixMultiply( &UVTextureMatrix, &UVTextureMatrix, &rotmat );

	gpDev->SetTransform(D3DTS_TEXTURE0, &UVTextureMatrix); 

	gpDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION | 1);
	gpDev->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	gpDev->SetTransform(D3DTS_TEXTURE1, &UVTextureMatrix);
#endif

	register int tileindexX;
	register int tileindexZ;
	register int splatstacklayerindex;
	_XLODTILE*	 pCurrentTile;

	for( int splatlayer = 0; splatlayer < m_UsedLayerCount; splatlayer++ )
	{		
#ifdef _XDWDEBUG	
		if( !g_ViewSplatLayer[splatlayer] ) continue;
#endif

		// Set splat infomations...
		gpDev->SetTransform( D3DTS_TEXTURE0, &m_LayerInfo[splatlayer].m_DiffuseTextureMatrix );
		g_WorldObjectTextureArchive.SetTexture_NoRenderStyle( m_LayerInfo[splatlayer].m_DetailTextureIndex, 0 );
	
		// render each tile
		for( int tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[splatlayer]; tilerenderingcacheindex++ )
		{
			tileindexX				= m_TerrainTileRenderingCache[splatlayer][tilerenderingcacheindex].xcoord;
			tileindexZ				= m_TerrainTileRenderingCache[splatlayer][tilerenderingcacheindex].zcoord;
			splatstacklayerindex	= m_TerrainTileRenderingCache[splatlayer][tilerenderingcacheindex].layerindex;
			pCurrentTile			= &m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX];
					
			// activate the vertex buffer for this tile
			LEVEL MyLevel = pCurrentTile->DetailLevel;
						
			if (SUCCEEDED(gpDev->SetStreamSource(0,pCurrentTile->VBuffer, 0, sizeof(TERRAIN_VERTEX))))
			{
				if( pCurrentTile->SplatData[splatstacklayerindex].m_pBlendTexture )
				{
					gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					gpDev->SetTexture( 1, pCurrentTile->SplatData[splatstacklayerindex].m_pBlendTexture );
				}
				else
				{
					gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
					gpDev->SetTexture( 1, NULL );
				}

				
				if( pCurrentTile->BodyTile )
				{
					// examine the tile above this tile
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_TOP )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ-1][m_TileStartOffset+tileindexX].DetailLevel;
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
							m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].TriangleCount;
#endif
						}
					}
					
					// examine the tile below this tile							
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_BOTTOM )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ+1][m_TileStartOffset+tileindexX].DetailLevel;
						// draw the connecting piece needed
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
							m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].TriangleCount;					
#endif
						}							
					}

					// examine the tile to the left this tile							
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_LEFT )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX-1].DetailLevel;
						// draw the connecting piece needed
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
							m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].TriangleCount;					
#endif
						}	
					}

					// examine the tile to the right this tile
					if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_RIGHT )
					{
						LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX+1].DetailLevel;

						// draw the connecting piece needed
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
							m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].TriangleCount;
#endif
						}							
					}
				}
				
				// finally, draw the body tile needed

				if( pCurrentTile->DetailLevel == LEVEL_3 )
				{
					if( pCurrentTile->SplatData[splatstacklayerindex].m_pSplatIndexBuffer )
					{
						if( pCurrentTile->BodyTile == _XDEF_LODDRAWATTRIB_NONE ) // 맞물린 타일이 아닐때만 스플랫인덱스 사용 
						{
							if (SUCCEEDED(gpDev->SetIndices(pCurrentTile->SplatData[splatstacklayerindex].m_pSplatIndexBuffer)))
							{
								gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, pCurrentTile->SplatData[splatstacklayerindex].m_SplatTriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
								m_FacesDrawn += pCurrentTile->SplatData[splatstacklayerindex].m_SplatTriangleCount;
#endif
							}
						}
						else
						{
							if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].phIndexBuffer)))
							{
								gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
								m_FacesDrawn += m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount;
#endif
							}
						}
					}
					else
					{
						if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].phIndexBuffer)))
						{
							gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
							m_FacesDrawn += m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount;
#endif
						}

					}
				}
				else
				{
					if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].phIndexBuffer)))
					{
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
						m_FacesDrawn += m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount;
#endif
					}
				}
			}
		}
	}


	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 21 );

	gpDev->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1  );
	gpDev->SetRenderState( D3DRS_COLORVERTEX , TRUE );
	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_TerrainAmbientColor );
	
	// render each tile : prerendered layer
	int layerindex = m_UsedLayerCount+1;
	for( int tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[layerindex]; tilerenderingcacheindex++ )
	{
		tileindexX				= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].xcoord;
		tileindexZ				= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].zcoord;
		splatstacklayerindex	= m_TerrainTileRenderingCache[layerindex][tilerenderingcacheindex].layerindex;
		pCurrentTile			= &m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX];
				
		// activate the vertex buffer for this tile
		LEVEL MyLevel = pCurrentTile->DetailLevel;
					
		if (SUCCEEDED(gpDev->SetStreamSource(0,pCurrentTile->VBuffer, 0, sizeof(TERRAIN_VERTEX))))
		{
			gpDev->SetTexture( 0, pCurrentTile->m_pPreRenderedTextureObject );
						
			if( pCurrentTile->BodyTile )
			{
				// examine the tile above this tile
				if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_TOP )
				{
					LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ-1][m_TileStartOffset+tileindexX].DetailLevel;
					if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].phIndexBuffer)))
					{
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
						m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].TriangleCount;
#endif
					}
				}
				
				// examine the tile below this tile							
				if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_BOTTOM )
				{
					LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ+1][m_TileStartOffset+tileindexX].DetailLevel;
					// draw the connecting piece needed
					if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].phIndexBuffer)))
					{
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
						m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].TriangleCount;					
#endif
					}							
				}

				// examine the tile to the left this tile							
				if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_LEFT )
				{
					LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX-1].DetailLevel;
					// draw the connecting piece needed
					if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].phIndexBuffer)))
					{
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
						m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].TriangleCount;					
#endif
					}	
				}

				// examine the tile to the right this tile
				if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_RIGHT )
				{
					LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX+1].DetailLevel;

					// draw the connecting piece needed
					if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].phIndexBuffer)))
					{
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
						m_FacesDrawn += m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].TriangleCount;
#endif
					}							
				}
			}
			
			// finally, draw the body tile needed

			if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].phIndexBuffer)))
			{
				gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount);
#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
				m_FacesDrawn += m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount;
#endif
			}
		}
	}

#ifndef _XDEF_USETERRAINUV
	gpDev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	gpDev->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
#endif


	
#ifdef _XDWDEBUG

	if( g_ViewPathFindInfo )
	{
		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_TERRAIN_NOSHADOW );

		g_TerrainTextureArchive.SetTexture_NoRenderStyle( g_TerrainTextureArchive.FindResource( "redarrow.bmp" ), 0 );

		gpDev->SetSamplerState( 0, D3DSAMP_MINFILTER    , D3DTEXF_POINT);
		gpDev->SetSamplerState( 0, D3DSAMP_MAGFILTER    , D3DTEXF_POINT);
		gpDev->SetSamplerState( 0, D3DSAMP_MIPFILTER    , D3DTEXF_POINT);
		
		gpDev->SetSamplerState( 1, D3DSAMP_MINFILTER    , D3DTEXF_POINT);
		gpDev->SetSamplerState( 1, D3DSAMP_MAGFILTER    , D3DTEXF_POINT);
		gpDev->SetSamplerState( 1, D3DSAMP_MIPFILTER    , D3DTEXF_POINT);

		for( int tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[m_UsedLayerCount]; tilerenderingcacheindex++ )
		{
			pCurrentTile = &m_TerrainTile[m_TileStartOffset+m_TerrainTileRenderingCache[m_UsedLayerCount][tilerenderingcacheindex].zcoord]
										 [m_TileStartOffset+m_TerrainTileRenderingCache[m_UsedLayerCount][tilerenderingcacheindex].xcoord];	
		
			if( pCurrentTile->m_pPathFindAreaBlendTexture )
			{
				LEVEL MyLevel = pCurrentTile->DetailLevel;

				if (SUCCEEDED(gpDev->SetStreamSource(0,pCurrentTile->VBuffer, 0, sizeof(TERRAIN_VERTEX))))
				{
					gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					gpDev->SetTexture( 1, pCurrentTile->m_pPathFindAreaBlendTexture );
										
					
					if( pCurrentTile->BodyTile )
					{
						// examine the tile above this tile
						if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_TOP )
						{
							if( tileindexZ-1 >= 0 )
							{
								LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ-1][m_TileStartOffset+tileindexX].DetailLevel;
								if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].phIndexBuffer)))
								{
									gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[TOP][ThisLevel].TriangleCount);
								}
							}
						}
						
						// examine the tile below this tile							
						if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_BOTTOM )
						{
							if( tileindexZ+1 < m_TileCount )
							{
								LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ+1][m_TileStartOffset+tileindexX].DetailLevel;
								// draw the connecting piece needed
								if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].phIndexBuffer)))
								{
									gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[BOTTOM][ThisLevel].TriangleCount);
								}							
							}
						}
						
						// examine the tile to the left this tile							
						if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_LEFT )
						{
							if( tileindexX-1 >= 0 )
							{
								LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX-1].DetailLevel;
								// draw the connecting piece needed
								if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].phIndexBuffer)))
								{
									gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[LEFT][ThisLevel].TriangleCount);
								}	
							}
						}
						
						// examine the tile to the right this tile
						if( pCurrentTile->BodyTile & _XDEF_LODDRAWATTRIB_RIGHT )
						{
							if( tileindexX+1 < m_TileCount )
							{
								LEVEL ThisLevel = m_TerrainTile[m_TileStartOffset+tileindexZ][m_TileStartOffset+tileindexX+1].DetailLevel;
								
								// draw the connecting piece needed
								if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].phIndexBuffer)))
								{
									gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileConnectors[RIGHT][ThisLevel].TriangleCount);
								}
							}
						}
					}
					
					// finally, draw the body tile needed
					
					if (SUCCEEDED(gpDev->SetIndices(m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].phIndexBuffer)))
					{
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, m_DetailLevel[MyLevel].TileBodies[pCurrentTile->BodyTile].TriangleCount);
					}
				}
			}
		}

		gpDev->SetSamplerState( 0, D3DSAMP_MINFILTER    , D3DTEXF_LINEAR);
		gpDev->SetSamplerState( 0, D3DSAMP_MAGFILTER    , D3DTEXF_LINEAR);
		gpDev->SetSamplerState( 0, D3DSAMP_MIPFILTER    , D3DTEXF_LINEAR);
		
		gpDev->SetSamplerState( 1, D3DSAMP_MINFILTER    , D3DTEXF_LINEAR);
		gpDev->SetSamplerState( 1, D3DSAMP_MAGFILTER    , D3DTEXF_LINEAR);
		gpDev->SetSamplerState( 1, D3DSAMP_MIPFILTER    , D3DTEXF_LINEAR);
	}
#endif

	
#ifdef _XDWDEBUG
	if( m_ViewOBBObject )
	{	
		gpDev->SetTexture(0, NULL); 
		gpDev->SetTexture(1, NULL); 
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		gpDev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		gpDev->SetRenderState( D3DRS_COLORVERTEX , FALSE );
		gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSU,	D3DTADDRESS_WRAP);
		gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSV,	D3DTADDRESS_WRAP);

		for( int portalobbindex = 0; portalobbindex < g_AreaManager.m_PotalOBBCount; portalobbindex++ )
		{
			g_AreaManager.m_PortalOBBDataList[portalobbindex].m_OBBModel.Render( m_TerrainMatrix, g_AreaManager.m_PortalOBBDataList[portalobbindex].Axis(), D3DCOLOR_ARGB(255, 255, 255, 0 ));
		}

		if( !g_EnvironmentManager.m_Fog )
		{
			for( int i = 0; i < m_AllOBBCount; i++ )
			{
				if( &m_OBBDataList[i] == g_pLocalUser->m_pCollideOBB )
					m_OBBDataList[i].m_OBBModel.Render(m_TerrainMatrix, m_OBBDataList[i].Axis(), D3DCOLOR_ARGB(255, 255, 0, 255 ));
				else if( &m_OBBDataList[i] == g_pLocalUser->m_pGoundCollideOBB ) 
					m_OBBDataList[i].m_OBBModel.Render(m_TerrainMatrix, m_OBBDataList[i].Axis(), D3DCOLOR_ARGB(255, 255, 0, 255 ));
				else 
					m_OBBDataList[i].m_OBBModel.Render(m_TerrainMatrix, m_OBBDataList[i].Axis());
			}
		}

		POINT TilePosition;
		_XLODTILE* pCurrentTile = GetTerrainTile( g_pLocalUser->m_Position, &TilePosition );
		if( pCurrentTile )
		{
			if( !pCurrentTile->svObbList.empty() )
			{
				_XOBBData* pobbData;
				svdef_OBBData::iterator it;
				for (it = pCurrentTile->svObbList.begin(); it != pCurrentTile->svObbList.end(); ++ it)
				{
					pobbData = (*it);
					
					if( pobbData->m_OBBLevel == 0 )
					{
						if( pobbData->m_Collide ) pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 255, 64, 0 ));
						else if( pobbData == g_pLocalUser->m_pGoundCollideOBB ) pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 0, 64, 255 ));
						else pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 0, 255, 0 ));
					}
					else if( pobbData->m_OBBLevel == 1 )
					{
						if( pobbData->m_Collide ) pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 255, 0, 0 ));
						else if( pobbData == g_pLocalUser->m_pGoundCollideOBB ) pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 0, 0, 255 ));
						else pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis());
					}
					else //if( pobbData->m_OBBLevel == 2 )
					{
						if( pobbData->m_Collide ) pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 255, 0, 0 ));
						else if( pobbData == g_pLocalUser->m_pGoundCollideOBB ) pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 0, 0, 255 ));
						else pobbData->m_OBBModel.Render(m_TerrainMatrix, pobbData->Axis(), D3DCOLOR_ARGB(255, 255, 0, 255 ));
					}
							
					/*
					if( pobbData->m_OBBLevel == 0 )
					{
						gpDev->SetFVF( D3DFVF_XYZ );
						//gpDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
						gpDev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 12, g_OBBModelVertIndex, D3DFMT_INDEX16, pobbData->m_ModelVertex, sizeof( D3DXVECTOR3 ) );
						//gpDev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLESTRIP, 0, 8, 12, g_OBBModelVertIndex, D3DFMT_INDEX16, pobbData->m_ModelVertex, sizeof( D3DXVECTOR3 ) );					
					}
					*/
				}
			}
		}
		
		if( m_FunctionalObjectManager.m_countFunctionalObject != 0 )
		{
			for( int i = 0; i < m_FunctionalObjectManager.m_countFunctionalObject; i++ )
			{
				if( m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide ) 
					m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_OBBModel.Render(m_TerrainMatrix, m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].Axis(), D3DCOLOR_ARGB(255, 255, 0, 0 ));
				else 
					m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_OBBModel.Render(m_TerrainMatrix, m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].Axis());
			}
		}

		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	/*
	if( g_pLocalUser->m_ViewTarget )
	{
		if( g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_MOB )
		{
			if( !g_LodTerrain.CheckRayPass( g_pLocalUser->m_Position, ((_XMob*)g_pLocalUser->m_ViewTarget)->m_Position ) )
			{
				
			}
		}
	}
	*/

#endif
	
	
	gpDev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	gpDev->SetRenderState( D3DRS_COLORVERTEX , FALSE );
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSU,	D3DTADDRESS_WRAP);
	gpDev->SetSamplerState( 1, D3DSAMP_ADDRESSV,	D3DTADDRESS_WRAP);

			
#ifdef _XDWDEBUG
#ifdef _XDEF_USETERRAINNORMALVECTOR
	if( g_ViewNormalVector )
	{		
		AXIS_VERTEX	line[2];
		line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
		line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
		
		gpDev->SetTexture(0, NULL); 
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
		gpDev->SetTransform( D3DTS_WORLD, &m_TerrainMatrix );

		POINT TilePosition;
		GetTerrainTile( g_pLocalUser->m_Position, &TilePosition );

		for( int i = -1; i <= 1; i++ )
		{
			if( TilePosition.y+i >= 0 && TilePosition.y+i < m_TileCount )
			{
				for( int j = -1; j <= 1; j++ )
				{				
					if( TilePosition.x+j >= 0 && TilePosition.x+j < m_TileCount )
					{
						TERRAIN_VERTEX *pData;
						_XLODTILE* pCurrentTile = &m_TerrainTile[m_TileStartOffset+TilePosition.x+j][m_TileStartOffset+TilePosition.y+i];
						
						if (SUCCEEDED(pCurrentTile->VBuffer->Lock(0,0, (void**)&pData, g_3DObjectLockMethods)))
						{
							for( int k = 0; k < 81; k++ )
							{
								line[0].v = pData[k].vert;
								line[1].v = line[0].v + pData[k].norm * 1.5f;

								gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
							}

							pCurrentTile->VBuffer->Unlock();
						}
					}
				}
			}
		}

		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
#endif
#endif

#if defined( _XDWDEBUG ) || defined(_XGMCLIENT)
	_PERFORMANCECHECKEND
	g_TerrainRenderingTime = timeinmicroseconds;	
#endif

	
	if( g_bCanDoMultitexture )
	{
		gpDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
		gpDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
		gpDev->SetTexture( 1, NULL );
					
		if( g_bCloudShadow )
		{
			gpDev->SetTextureStageState( g_CloudShadowTextureStage, D3DTSS_COLOROP, D3DTOP_DISABLE );
			gpDev->SetTextureStageState( g_CloudShadowTextureStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			gpDev->SetTexture( g_CloudShadowTextureStage, NULL );
		}
	}	
	
	
#ifdef _XDWDEBUG
	if( viewaxis )
	{
		gpDev->SetTransform( D3DTS_WORLD, &axismatrix );
		axis.Render();
	}
#endif

}

void _XLodTerrain::RebuildRenderObjectStack( void )
{
	m_MeshObjectManager.ResetCount();

	g_WorldObjectTextureArchive.ResetObjectStack();	
	m_ObjectQuadTree._Render( &this->m_MeshObjectManager );	
}

void _XLodTerrain::RenderWaterAboveObject( void )
{
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
	g_WorldObjectTextureArchive.RenderAboveWaterObjectStack();
	g_WorldObjectTextureArchive.RenderAvobeWaterLightMapObjectStack();
}

void _XLodTerrain::RenderAniObject( FLOAT fElaspedTime )
{
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
	m_MeshObjectManager.RenderAniObject(fElaspedTime);
}

void _XLodTerrain::RenderObject( void )
{
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
	g_WorldObjectTextureArchive.RenderObjectStack();
	
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
	g_WorldObjectTextureArchive.RenderLightMapObjectStack();

	m_FunctionalObjectManager.Render();
}

void _XLodTerrain::DrawWater( void ) // No use...
{
	if(m_pWaterObjectManager)
	   m_pWaterObjectManager->Draw();
}

#define _XDEF_ENVSOUND_ALLWAYS_INTERVALSTIME	1000
#define _XDEF_ENVSOUND_REPEAT_INTERVALSTIME		20000
#define _XDEF_ENVSOUND_DISTANCE					32.f
#define _XDEF_ENVSOUND_UPDATEINTERVALTIME		1500

LONG	g_EnvScriptLastUpdateTime = 0;

void _XLodTerrain::ProcessEnvScript(FLOAT fElapsed, const D3DXVECTOR3& vCameraPosition)
{
	if( g_EnvScriptLastUpdateTime == 0 )
	{
		g_EnvScriptLastUpdateTime = g_LocalSystemTime;
	}
	else 
	{
		if( g_LocalSystemTime - g_EnvScriptLastUpdateTime > _XDEF_ENVSOUND_UPDATEINTERVALTIME )
		{
			g_EnvScriptLastUpdateTime = g_LocalSystemTime;
		}
		else
		{
			return;
		}
	}

	ProcessEnvEffect(g_EnvironmentManager.m_Environment);

	if( !g_3DSoundEffectFlag ) return;

	D3DXVECTOR3 vPosition;
	FLOAT fDistance;
	_XSoundObjectManager::svXObject::iterator it;
	for (it = m_SoundObjectManager.m_svXObject.begin(); it != m_SoundObjectManager.m_svXObject.end(); ++ it)
	{
		_XSoundObject* pSoundObject = (*it);
		vPosition.x = pSoundObject->m_matWorldPosition._41;
		vPosition.y = pSoundObject->m_matWorldPosition._42;
		vPosition.z = pSoundObject->m_matWorldPosition._43;

		fDistance = (vCameraPosition.x - vPosition.x) * (vCameraPosition.x - vPosition.x) +
					(vCameraPosition.y - vPosition.y) * (vCameraPosition.y - vPosition.y) +
					(vCameraPosition.z - vPosition.z) * (vCameraPosition.z - vPosition.z);

		if (pSoundObject->m_fDistance2 < fDistance)
		{
			pSoundObject->Stop();
			continue;
		}

		if (pSoundObject->m_byEvent != _XSoundObject::EVENT_ALL)
		{
			switch(g_EnvironmentManager.m_Environment)
			{
			case _XE_DAY_RAIN:
			case _XE_DAY_SNOW:
			case _XE_DAY:
				if (pSoundObject->m_byEvent != _XSoundObject::EVENT_DAY)
				{
					pSoundObject->Stop();
					continue;
				}
				break;
			case _XE_NIGHTFALL:
				if (pSoundObject->m_byEvent != _XSoundObject::EVENT_NIGHTFALL)
				{
					pSoundObject->Stop();
					continue;
				}
				break;
			case _XE_NIGHT_RAIN:
			case _XE_NIGHT_SNOW:
			case _XE_NIGHT:
				if (pSoundObject->m_byEvent != _XSoundObject::EVENT_NIGHT)
				{
					pSoundObject->Stop();
					continue;
				}
				break;
			case _XE_DAWN:
				if (pSoundObject->m_byEvent != _XSoundObject::EVENT_DAWN)
				{
					pSoundObject->Stop();
					continue;
				}
				break;
			}
		}

		pSoundObject->m_fPlayTime += fElapsed;
		if (pSoundObject->m_byInterval != 0 && pSoundObject->m_fPlayTime > (FLOAT)pSoundObject->m_byInterval)
		{
			pSoundObject->Stop();
			continue;
		}

		IEnvSound3D* pEnvSound3d = &g_pEnvironmentSound[pSoundObject->m_ObjectIndex];

		if (pSoundObject->m_bPlay == FALSE)
		{
			pSoundObject->m_bPlay = TRUE;

#ifdef _XUSEFMOD
			if (pSoundObject->m_byAttribute & _XSoundObject::ATTRIBUTE_REPEAT)
			{
				_XPlayEnvSoundEffect( pEnvSound3d->soundindex, vPosition, TRUE, TRUE );
			}
			else
			{
				_XPlayEnvSoundEffect( pEnvSound3d->soundindex, vPosition, FALSE );
			}
#else
			if (!pEnvSound3d->m_pSound3D->IsPlaying())
			{
				if (pSoundObject->m_byAttribute & _XSoundObject::ATTRIBUTE_REPEAT)
				{
					if (!pEnvSound3d->m_pSound3D->IsLooping())
					{
						Sound3DInit init3dsound;
						init3dsound.m_bLooping = true;
						init3dsound.m_bStreaming = false;
						init3dsound.m_bMusic = false;	
						init3dsound.m_pPackage = &g_EnvSoundEffectArchive;

						pEnvSound3d->m_pSound3D->Destroy();
						pEnvSound3d->m_pSound3D = NULL;

						AudioMgr()->CreateSound3D(pEnvSound3d->m_pSound3D);

						init3dsound.m_sFileName = pEnvSound3d->envsoundname;
						pEnvSound3d->m_pSound3D->Init(init3dsound);
						pEnvSound3d->m_pSound3D->SetVolume( 1.0f );
					}
				}
				AUDIOVECTOR* pAudioPosition = dynamic_cast<AUDIOVECTOR*>(&vPosition);
				pEnvSound3d->m_pSound3D->SetMinDistance(pSoundObject->m_byDistance / _XDEF_ENVSOUND_DISTANCE);
				pEnvSound3d->m_pSound3D->SetPosition(*pAudioPosition);
				pEnvSound3d->m_pSound3D->Play();
			}
			pEnvSound3d->AddRef();

			OutputDebugString("Add Sound Ref : ");
			OutputDebugString(pEnvSound3d->envsoundname);
			OutputDebugString("\n");
#endif
		}
		else
		{
#ifdef _XUSEFMOD
			if (pSoundObject->m_byAttribute & _XSoundObject::ATTRIBUTE_REPEAT)
			{
				_XPlayEnvSoundEffect( pEnvSound3d->soundindex, vPosition, TRUE, TRUE );
			}			

			//_XPlayEnvSoundEffect( pEnvSound3d->soundindex, vPosition, TRUE );
#else
			ISound3D* pSound3D = pEnvSound3d->m_pSound3D;
			if (!pSound3D->IsPlaying())
			{
				if (pSoundObject->m_byAttribute & _XSoundObject::ATTRIBUTE_REPEAT)
				{
					AUDIOVECTOR* pAudioPosition = dynamic_cast<AUDIOVECTOR*>(&vPosition);
					pSound3D->SetMinDistance(pSoundObject->m_byDistance / _XDEF_ENVSOUND_DISTANCE);
					pSound3D->SetPosition(*pAudioPosition);
					pSound3D->Play();
				}
				else if (pSoundObject->m_nPlayCount < pSoundObject->m_byCount)
				{
					++ pSoundObject->m_nPlayCount;
					
					AUDIOVECTOR* pAudioPosition = dynamic_cast<AUDIOVECTOR*>(&vPosition);
					pSound3D->SetMinDistance(pSoundObject->m_byDistance / _XDEF_ENVSOUND_DISTANCE);
					pSound3D->SetPosition(*pAudioPosition);

					pSound3D->Play();
				}
			}
#endif
		}
	}
}

void _XLodTerrain::ProcessEnvEffect(int nEnvironmentType)
{
	if (m_nCurEnvironmentType == nEnvironmentType) return;
	m_nCurEnvironmentType = nEnvironmentType;

	svdef_XEffectObject::iterator it;
	for (it = m_svEffectTemplateList.begin(); it != m_svEffectTemplateList.end(); ++ it)
	{
		_XEffectObject* pEffectObject = (*it);
		if (pEffectObject->m_byEvent == _XEffectObject::EVENT_ALL)
		{
			continue;
		}

		if (pEffectObject->m_pParticleGroupItem != NULL)
		{
			switch(m_nCurEnvironmentType)
			{
			case _XE_DAY_RAIN:
			case _XE_DAY_SNOW:
			case _XE_DAY:
				if (pEffectObject->m_byEvent != _XEffectObject::EVENT_DAY)
				{
					m_EffectObjectManager.DeleteInstance(pEffectObject->m_pParticleGroupItem);
					pEffectObject->m_pParticleGroupItem = NULL;
					continue;
				}
				break;
			case _XE_NIGHTFALL:
				if (pEffectObject->m_byEvent != _XEffectObject::EVENT_NIGHTFALL)
				{
					m_EffectObjectManager.DeleteInstance(pEffectObject->m_pParticleGroupItem);
					pEffectObject->m_pParticleGroupItem = NULL;
					continue;
				}
				break;
			case _XE_NIGHT_RAIN:
			case _XE_NIGHT_SNOW:
			case _XE_NIGHT:
				if (pEffectObject->m_byEvent != _XEffectObject::EVENT_NIGTH)
				{
					m_EffectObjectManager.DeleteInstance(pEffectObject->m_pParticleGroupItem);
					pEffectObject->m_pParticleGroupItem = NULL;
					continue;
				}
				break;
			case _XE_DAWN:
				if (pEffectObject->m_byEvent != _XEffectObject::EVENT_DAWN)
				{
					m_EffectObjectManager.DeleteInstance(pEffectObject->m_pParticleGroupItem);
					pEffectObject->m_pParticleGroupItem = NULL;
					continue;
				}
				break;
			}
		}

		if (pEffectObject->m_pParticleGroupItem == NULL)
		{
			switch(g_EnvironmentManager.m_Environment)
			{
			case _XE_DAY_RAIN:
			case _XE_DAY_SNOW:
			case _XE_DAY:
				if (pEffectObject->m_byEvent == _XEffectObject::EVENT_DAY)
				{
					_XParticleGroupItem* pParticleGroupItem  = &g_WorldEffectGroupManager.m_ParticleTemplateList[pEffectObject->m_ObjectIndex];
					pEffectObject->m_pParticleGroupItem = m_EffectObjectManager.InsertNewInstance( &g_WorldEffectGroupManager, pParticleGroupItem->m_ID, &pEffectObject->m_matWorldPosition);
				}
				break;
			case _XE_NIGHTFALL:
				if (pEffectObject->m_byEvent == _XEffectObject::EVENT_NIGHTFALL)
				{
					_XParticleGroupItem* pParticleGroupItem  = &g_WorldEffectGroupManager.m_ParticleTemplateList[pEffectObject->m_ObjectIndex];
					pEffectObject->m_pParticleGroupItem = m_EffectObjectManager.InsertNewInstance( &g_WorldEffectGroupManager, pParticleGroupItem->m_ID, &pEffectObject->m_matWorldPosition);
				}
				break;
			case _XE_NIGHT_RAIN:
			case _XE_NIGHT_SNOW:
			case _XE_NIGHT:
				if (pEffectObject->m_byEvent == _XEffectObject::EVENT_NIGTH)
				{
					_XParticleGroupItem* pParticleGroupItem  = &g_WorldEffectGroupManager.m_ParticleTemplateList[pEffectObject->m_ObjectIndex];
					pEffectObject->m_pParticleGroupItem = m_EffectObjectManager.InsertNewInstance( &g_WorldEffectGroupManager, pParticleGroupItem->m_ID, &pEffectObject->m_matWorldPosition);
				}
				break;
			case _XE_DAWN:
				if (pEffectObject->m_byEvent == _XEffectObject::EVENT_DAWN)
				{
					_XParticleGroupItem* pParticleGroupItem  = &g_WorldEffectGroupManager.m_ParticleTemplateList[pEffectObject->m_ObjectIndex];
					pEffectObject->m_pParticleGroupItem = m_EffectObjectManager.InsertNewInstance( &g_WorldEffectGroupManager, pParticleGroupItem->m_ID, &pEffectObject->m_matWorldPosition);
				}
				break;
			}
		}
	}
}

BOOL _XLodTerrain::InitialzeSpeedTreeWrapper( void )
{	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialize Speed tree wrapper
	g_XST_Wrapper.Release();

	if( !g_XST_Wrapper.Initialize() )
	{
		return FALSE;
	}
	
	return TRUE;
}

void _XLodTerrain::ReportUseResource( void )
{
	_XLog( "= Terrain report ========" );
	_XLog( "World size width : %f / height : %f", m_Width, m_Height );
	_XLog( "One grid size : %f", m_GridRealSize );
	_XLog( "Tile count : %d",	m_TileCount );
	_XLog( "Vertex count : %d ", m_VertexCount );

	DWORD terrainvertexmemsize = sizeof(TERRAIN_VERTEX) * (TILE_VERTS*TILE_VERTS) * (m_TileCount * m_TileCount);

	_XLog( "Terrain vertex size : %f MByte [ %d Bytes ]", terrainvertexmemsize / 1048576.0f, terrainvertexmemsize );


	_XLog( "= Terrain splat informations ========" );

	_XLog( "== Splat tile count : %d", m_UsedLayerCount );

	for( int layer = 0; layer < m_UsedLayerCount; layer++ )
	{

	}

	_XLODTILE*	 pCurrentTile;	

	int splatalphatexturecount = 0;

	for (int i=0;i<m_TileCount;++i)
	{
		for (int j=0;j<m_TileCount;++j)
		{
			pCurrentTile = &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j];
			
			for( int splatlayer = 0; splatlayer < pCurrentTile->UsedSplatLayerCount; splatlayer++ )
			{
				if( pCurrentTile->SplatData[ splatlayer ].m_pBlendTexture )
				{
					splatalphatexturecount++;
				}
			}
		}
	}

	_XLog( "== Total Splat texture count : %d", splatalphatexturecount );
	_XLog( "== Total Splat texture memory size : %f MByte [ %d Bytes ]", 
		(_XDEF_SPLATBLENDTEXTURESIZE * _XDEF_SPLATBLENDTEXTURESIZE * 2 * splatalphatexturecount) / 1048576.0f, 
		 _XDEF_SPLATBLENDTEXTURESIZE * _XDEF_SPLATBLENDTEXTURESIZE * 2 * splatalphatexturecount );


	_XLog( "== Total world object  count : %d", m_ObjectManager.m_ObjectCount );
	_XLog( "== Total mesh template count : %d / %d", m_MeshObjectManager.m_ModelStackHeight, m_MeshObjectManager.m_ModelStackMaxCount );

	_XLog( "== Total Sound object  count : %d", m_SoundObjectManager.GetCount() );
	_XLog( "== Total Effect object  count : %d", m_EffectObjectManager.itemcount );
	
}

WORD g_XWordFilter[16] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 0x100, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04,0x02, 0x01 };

BOOL _XLodTerrain::GetPathFindAreaInfo( int arrayx, int arrayz )
{
	if( arrayx < 0 || arrayx >= (m_TileCount<<4) ) return TRUE;
	if( arrayz < 0 || arrayz >= (m_TileCount<<4) ) return TRUE;
	int bitpos = arrayx - ((arrayx>>4)<<4);
	return (BOOL)(m_ppPathFindAreaArray[arrayz][(arrayx>>4)] & g_XWordFilter[bitpos]);
}

int	_XLodTerrain::GetPathFindAreaNeighborInfo( int arrayx, int arrayz )
{
	int sum = 0;
	
	if( GetPathFindAreaInfo( arrayx-1, arrayz-1 ) ) sum++;
	if( GetPathFindAreaInfo( arrayx, arrayz-1 )   ) sum++;
	if( GetPathFindAreaInfo( arrayx+1, arrayz-1 ) ) sum++;

	if( GetPathFindAreaInfo( arrayx-1, arrayz )   ) sum++;
	if( GetPathFindAreaInfo( arrayx+1, arrayz )   ) sum++;

	if( GetPathFindAreaInfo( arrayx-1, arrayz+1 ) ) sum++;
	if( GetPathFindAreaInfo( arrayx,   arrayz+1 ) ) sum++;
	if( GetPathFindAreaInfo( arrayx+1, arrayz+1 ) ) sum++;

	return sum;
}

BOOL _XLodTerrain::CheckDirectPath( int startx, int startz, int& endx, int& endz )
{
	int i,u,s,v,d1x,d1y,d2x,d2y,m,n;
	u   = endx-startx;              
	v   = endz-startz;              
	
	if( u != 0 ){ d1x = d2x = (u < 0) ? -1 : +1; }
	else d1x = d2x = 0;
	
	if( v != 0 ){ d1y = (v < 0) ? -1 : +1; }
	else d1y = 0;             
	
	d2y = 0;
	
	m   = abs(u);             
	n   = abs(v);             
	
	if (m<=n) 
	{                
		d2x = 0;
		if( v != 0 ){ d2y = (v < 0) ? -1 : +1; }
		else d2y = 0;            
		
		m   = abs(v);            
		n   = abs(u);            
	}
	
	s = (m>>1);                 
	
	for (i=0;i<m+1;i++)
	{    
		if( ( ( startx >= (m_TileCount<<4) ) || ( startz >= (m_TileCount<<4) ) || ( startx < 0 ) || (startz < 0 )  ) ) 	
		{
			endx = startx;	// save clipped collide point
			endz = startz;
			return FALSE;
		}

		int bitpos = startz - ((startz>>4)<<4);
			
		if( (m_ppPathFindAreaArray[startx][(startz>>4)] & g_XWordFilter[bitpos]) ) 
		{
			endx = startx;	// save collide point
			endz = startz;
			return FALSE;
		}

	    s += n;                  
	    if (s >= m)
		{            
		   s -= m;
		   startx += d1x;
		   startz += d1y;
		}
	    else
		{
		   startx += d2x;
		   startz += d2y;
		}
	}

	return TRUE;
}

#ifdef _XDWDEBUG	

BOOL _XLodTerrain::CreatePathFindAreaBlendMap( int tilex, int tilez )
{
	if( !m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture )
	{
		if( FAILED( gpDev->CreateTexture( 16, 16, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, 
										  &m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture, NULL ) ) )
		{
			_XFatalError( "Can't create path find area blending texture" );
			return FALSE;
		}		
	}
	
	D3DLOCKED_RECT LockData;
	if ( FAILED(m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture->LockRect( 0, &LockData, NULL, 0 )) ) 
	{
		_XFatalError( "Can't lock path find area blending texture" );
		return false;
	}
	
	USHORT * pBuffer = (USHORT*)LockData.pBits;
	memset( pBuffer, 0, sizeof( USHORT ) * 16 * 16 );
	m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture->UnlockRect( 0 );
	
	return TRUE;
}

BOOL _XLodTerrain::ReGeneratePathFindAreaBlendMap( void )
{
	for( int tilez = 0; tilez < m_TileCount; tilez++ )
	{
		for( int tilex = 0; tilex < m_TileCount; tilex++ )
		{
			DWORD attribsumvalue = 0;
			
			for ( int z = 0; z < 16; z++ )
			{
				attribsumvalue += m_ppPathFindAreaArray[(tilez<<4) + z][tilex];
			}
			
			if( attribsumvalue > 0 )
			{
				if( !m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture )
				{
					if( !CreatePathFindAreaBlendMap( tilex, tilez ) )
					{
						return FALSE;
					}
				}
				
				if( m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture )
				{
					D3DLOCKED_RECT LockData;
					if ( FAILED(m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture->LockRect( 0, &LockData, NULL, 0 )) ) 
					{
						_XFatalError( "Can't lock path find area blending texture" );
						return false;
					}
					
					USHORT * pBuffer = (USHORT*)LockData.pBits;
					
					int arrayposx = (tilex << 4); 
					int arrayposz = (tilez << 4); 
					
					for ( int z = 0; z < 16; z++ )
					{
						for ( int x = 0; x < 16; x++, pBuffer++ )
						{
							if( GetPathFindAreaInfo( arrayposx+x, arrayposz+z ) )
							{
								*pBuffer = (USHORT)0x8FFF;
							}
							else
							{
								*pBuffer = (USHORT)0x0000;
							}
						}
						
						pBuffer += (LockData.Pitch - ( 16 * sizeof(USHORT) ) );
					}
					
					m_TerrainTile[m_TileStartOffset+tilez][m_TileStartOffset+tilex].m_pPathFindAreaBlendTexture->UnlockRect( 0 );
				}
			}
		}
	}
	
	return TRUE;
}

#endif


void _XLodTerrain::GetPickedPoint( BOOL checkchangestate )
{
	BOOL checkterrainpicking = TRUE;
	// 버튼이 안눌렸거나 이미 피킹 계산되었으면 리턴
	if( checkchangestate )
	{
		if( !gpInput->GetMouseState()->bButton[0] || g_MouseFirstClickPassed )
		{
			checkterrainpicking = FALSE;
		}
	}
	
	// 포커스를 받았으며 지형이 아닐경우 리턴
	if( ::g_CurrentFocusedObject )
	{
		if( ::g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND ) 
			checkterrainpicking = FALSE;
	}

	D3DXVECTOR3 terrainnoral;

	if( checkterrainpicking )
	{
		g_MouseFirstClickPassed = TRUE;	// 마우스를 떼기전까진 재계산 안함...

		g_fDist = m_LastPickedDist = 9999.0f;

		D3DXVECTOR3 lastpickedpoint;
		_XLODTILE*	pCurrentTile;

		for( int tilerenderingcacheindex = 0; tilerenderingcacheindex < m_TerrainTileRenderingCacheLength[m_UsedLayerCount]; tilerenderingcacheindex++ )
		{
			pCurrentTile = &m_TerrainTile[m_TileStartOffset+m_TerrainTileRenderingCache[m_UsedLayerCount][tilerenderingcacheindex].zcoord]
										 [m_TileStartOffset+m_TerrainTileRenderingCache[m_UsedLayerCount][tilerenderingcacheindex].xcoord];	
		
			if( pCurrentTile->DetailLevel >= LEVEL_3 )
			{
				// Check highlevel tri... intersect...
				LPWORD pDetailIndices = g_TerrainIndexLUT;				
				
				D3DXVECTOR3    *pSystemVertexbuffer = pCurrentTile->VertexBuffer;				
					
				for( int q=0; q<m_DetailLevel[3].TileBodies[0].TriangleCount;q++ )
				{	
					if( IntersectTriangle( pSystemVertexbuffer[*(pDetailIndices)  ], //p1
						                   pSystemVertexbuffer[*(pDetailIndices+1)], //p2
										   pSystemVertexbuffer[*(pDetailIndices+2)], //p3
										   &g_fDist, &g_fBary1, &g_fBary2 ) )
					{				
						if( ( g_fDist > 0.0f ) && ( g_fDist < m_LastPickedDist ) )
						{
							g_vMousePosition = g_vPickRayOrig + g_vPickRayDir * g_fDist;
							m_LastPickedDist	= g_fDist;

							D3DXVECTOR3 vLeft;
							D3DXVECTOR3 vRight;
							D3DXVECTOR3 vNormal;
							vLeft.x		= pSystemVertexbuffer[*(pDetailIndices+1)].x - pSystemVertexbuffer[*(pDetailIndices)  ].x;
							vLeft.y		= pSystemVertexbuffer[*(pDetailIndices+1)].y - pSystemVertexbuffer[*(pDetailIndices)  ].y;
							vLeft.z		= pSystemVertexbuffer[*(pDetailIndices+1)].z - pSystemVertexbuffer[*(pDetailIndices)  ].z;
							vRight.x	= pSystemVertexbuffer[*(pDetailIndices+2)].x - pSystemVertexbuffer[*(pDetailIndices)  ].x;
							vRight.y	= pSystemVertexbuffer[*(pDetailIndices+2)].y - pSystemVertexbuffer[*(pDetailIndices)  ].y;
							vRight.z	= pSystemVertexbuffer[*(pDetailIndices+2)].z - pSystemVertexbuffer[*(pDetailIndices)  ].z;								
							D3DXVec3Cross( &terrainnoral, &vLeft,  &vRight );
							D3DXVec3Identity( terrainnoral );
						}												
					}							
					
					pDetailIndices+=3;
				} // Check terrain picking...
				
				if( !pCurrentTile->svObbList.empty() )
				{							
					_XOBBData* pObbData;
					svdef_OBBData::iterator it;
					for (it = pCurrentTile->svObbList.begin(); it != pCurrentTile->svObbList.end(); ++ it)
					{
						pObbData = (*it);
						if( pObbData->m_OBBLevel == 0 )
						{
							D3DXVECTOR3 pickedpoint;
							if( pObbData->InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir,  pickedpoint ) )
							{
								FLOAT dx = ( g_vPickRayOrig.x - pickedpoint.x);
								FLOAT dy = ( g_vPickRayOrig.y - pickedpoint.y);
								FLOAT dz = ( g_vPickRayOrig.z - pickedpoint.z);
								FLOAT _distance = _XFC_sqrt( dx*dx + dy*dy + dz*dz );
								
								// 가장 가까운 피킹포인트를 저장....
								if( ( _distance > 0.0f ) && (_distance < m_LastPickedDist) )
								{
									m_LastPickedDist = _distance;
									g_vMousePosition = pickedpoint;
								}
							}
						}
					}
				}					
			}  // Check lod terrain level 1
		}
	}

	if( !m_bInvisiblePortalMouseView )
	{
		if( !g_pBlackClanAreaSelect_Window->GetShowStatus() && // 통합수구 GUI가 안보일때만 수구 체크
			!g_pWhiteClanAreaSelect_Window->GetShowStatus()  )
		{
			//////////////////////////////////////////////////////////////////////////	
			// Check portal hovering
			int index = 0;
			_XArea* currentarea = NULL;
			g_CurrentHoveredWarpzoneIndex = -1;
			BOOL clickedportalobb = FALSE;

			list <_XArea *>::iterator iter;
			for(iter = g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.begin() ; iter != g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.end() ; iter++, index++)
			{
				currentarea = *iter;
				
				// 같은존 같은 리젼이면 스킵 (다른 지역에서 들어오는 영역)
				if( !(g_CurrentZoneInfoIndex == (currentarea->m_TargetZone + 1) &&	currentarea->m_TargetWarpzone == currentarea->m_Num) )
				{
					D3DXVECTOR3 pickedpoint;
					if( g_AreaManager.m_PortalOBBDataList[index].InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir,  pickedpoint ) )
					{
						FLOAT dx = ( m_3PCamera.m_CameraPosition.x - pickedpoint.x);
						FLOAT dy = ( m_3PCamera.m_CameraPosition.y - pickedpoint.y);
						FLOAT dz = ( m_3PCamera.m_CameraPosition.z - pickedpoint.z);
						FLOAT _distance = _XFC_sqrt( dx*dx + dy*dy + dz*dz );
						
						if( ( _distance > 0.0f ) && ( _distance < 50.0f ) && (_distance < m_LastPickedDist) )
						{
							m_LastPickedDist = _distance;

							D3DXVECTOR3 normal = pickedpoint - g_AreaManager.m_PortalOBBDataList[index].m_kCenter;
							D3DXVec3Normalize( &normal, &normal );

							dx = ( g_AreaManager.m_PortalOBBDataList[index].m_kCenter.x - pickedpoint.x);
							dy = ( g_AreaManager.m_PortalOBBDataList[index].m_kCenter.y - pickedpoint.y);
							dz = ( g_AreaManager.m_PortalOBBDataList[index].m_kCenter.z - pickedpoint.z);
							_distance = _XFC_sqrt( dx*dx + dy*dy + dz*dz );

							g_vMousePosition = g_AreaManager.m_PortalOBBDataList[index].m_kCenter + (normal * (_distance * 0.5f) );
							g_vMousePosition.y -= fabs( g_AreaManager.m_PortalOBBDataList[index].m_afExtent[1] );

							g_ClickedWarpZoneIndex = currentarea->m_Num;
							clickedportalobb = TRUE;

							g_CurrentHoveredWarpzoneIndex = currentarea->m_Num;
						}				
					}
				}
			} // EOB : Check portal hovering
			
			if( !clickedportalobb ) // 마우스가 클릭되고 포탈 OBB가 클릭되지 않은경우 이전에 선택된 포탈 취소 ( 페이드쪽 사용 )
			{
				g_ClickedWarpZoneIndex = -1;
			}

			if( checkterrainpicking )
			{
				if( m_LastPickedDist == 9999.0f ) // 하늘 또는 멀리 클릭하였을 경우
				{				
					_XOBBData* OBBData = NULL;
					g_vMousePosition = g_vPickRayOrig + g_vPickRayDir * m_LODLevel1;
					g_vMousePosition.y = GetTerrainAndOBBHeight( g_vMousePosition, OBBData ); // 클릭 지점을 바닥으로 내림.
				}
			}
		}
	}
	
	if( m_LastPickedDist != 9999.0f )
	{
		D3DXVECTOR3 vRight = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		g_TerrainDecalObject.GenerateDecal( g_vMousePosition, terrainnoral, vRight, 8.0f,8.0f, 0.8f );
	}
}

_XTextureGabageCollector::_XTextureGabageCollector()
{
	texturewidth		=	32;
	textureheight		=	32;
	stacksize			=	0;	
	throwinteval		=   30000; // 5sec
	stackincreasefactor	=	10;
	format				=	D3DFMT_A8R8G8B8;
	texturearray		=   NULL;

}

_XTextureGabageCollector::~_XTextureGabageCollector()
{
	Destroy();
}

BOOL _XTextureGabageCollector::Initialize( int width, int height, int reservesize, _D3DFORMAT textureformat )
{
	texturewidth  = width;
	textureheight = height;
	format		  = textureformat;

	if( texturearray )
	{
		for( int i = 0; i < stacksize; ++i )
		{
			SAFE_RELEASE( texturearray[i].pTextureObject );
		}

		SAFE_DELETE_ARRAY( texturearray );
	}

	texturearray = new _XTextureManageData[ reservesize ];

	if( !texturearray )
	{
		_XFatalError( "Can't create the gabage collector buffer" );
		return FALSE;
	}

	stacksize = reservesize;

	for( int i = 0; i < stacksize; i++ )
	{
		if( FAILED( gpDev->CreateTexture( texturewidth, textureheight, 1, 0, format, D3DPOOL_MANAGED, &texturearray[i].pTextureObject, NULL ) ) )
		{
			return FALSE;
		}
	}

	return TRUE;	
}

void _XTextureGabageCollector::Destroy( void )
{
	if( texturearray )
	{
		for( int i = 0; i < stacksize; ++i )
		{
			SAFE_RELEASE( texturearray[i].pTextureObject );
		}
		
		SAFE_DELETE_ARRAY( texturearray );
	}
	
	stacksize = 0;
}

BOOL _XTextureGabageCollector::GetNewObject( _XLODTILE* pOwner )
{
	/*8
	for( int i = 0; i < stacksize; i++ )
	{
		if( !texturearray[i].pOwner )
		{
			texturearray[i].pOwner = pOwner;
			texturearray[i].LastAccessTime = g_LocalSystemTime;
			pOwner->m_pPreRenderedTextureObject = texturearray+i;

			return TRUE;
		}
		else
		{
			if( texturearray[i].LastAccessTime == 0 )
			{
				texturearray[i].LastAccessTime = g_LocalSystemTime;
			}

			if( (g_LocalSystemTime - texturearray[i].LastAccessTime) > throwinteval )
			{
				if( texturearray[i].pOwner )
				{
					texturearray[i].pOwner->m_pPreRenderedTextureObject = NULL;
				}

				texturearray[i].pOwner = pOwner;
				pOwner->m_pPreRenderedTextureObject = texturearray+i;
				
				return TRUE;
			}
		}
	}

	// new insert
	_XTextureManageData* pNewTexturearray = NULL;
	pNewTexturearray = new _XTextureManageData[ stacksize + stackincreasefactor ];
	if(!pNewTexturearray)
	{
		_XFatalError( "Can't expand stack [ gabage collector ]" );
		pOwner->m_pPreRenderedTextureObject = NULL;
		return FALSE;
	}

	memcpy( pNewTexturearray, texturearray, sizeof(_XTextureManageData) * stacksize );
	stacksize += stackincreasefactor;

	SAFE_DELETE_ARRAY( texturearray );
	texturearray = pNewTexturearray;
	pOwner->m_pPreRenderedTextureObject = texturearray + (stacksize-stackincreasefactor);
	texturearray[stacksize-stackincreasefactor].pOwner = pOwner;

	for( i = stacksize-stackincreasefactor; i < stacksize; i++ )
	{
		if( FAILED( gpDev->CreateTexture( texturewidth, textureheight, 1, 0, format, D3DPOOL_MANAGED, &texturearray[i].pTextureObject, NULL ) ) )
		{
			_XFatalError( "Can't create texture [gabage collector]" );
			return FALSE;
		}
	}
	*/
	return TRUE;
}


void _XLodTerrain::PreCachingTexture( void )
{	
	m_ObjectManager.PreCachingTexture();
	
}

//#define _XDEF_COLLIDETEST

BOOL _XLodTerrain::CheckRayPass( D3DXVECTOR3 startpt, D3DXVECTOR3 endpt, int checksize )
{
	DWORD	arrayx, arrayz;
	int		tilex, tilez;	
		
	// World coord.. clipping ...
	D3DXVECTOR3 center;
	D3DXVECTOR3 raydirection;
	
	raydirection = endpt - startpt;
	//D3DXVec3Normalize( &raydirection, &raydirection );

	center.x = startpt.x;
	center.y = 0.0f;
	center.z = startpt.z;
	if( center.x < -m_HalfWorldSize ) center.x = -m_HalfWorldSize;
	if( center.x >  m_HalfWorldSize ) center.x =  m_HalfWorldSize-1.0f;
	
	if( center.z < -m_HalfWorldSize ) center.z = -m_HalfWorldSize;
	if( center.z >  m_HalfWorldSize ) center.z =  m_HalfWorldSize-1.0f;
	
	//////////////////////////////////////////////////////////////////////////	
	// Check direct path
	int	xcoord = _XFC_FloatToIntRet( ( startpt.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	int zcoord = _XFC_FloatToIntRet( ( startpt.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );		
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;	
	
	int startxpoint = xcoord;
	int startzpoint = zcoord;
	
	xcoord = _XFC_FloatToIntRet( ( endpt.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	zcoord = _XFC_FloatToIntRet( ( endpt.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );	
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;	
	
	int endxpoint = xcoord;
	int endzpoint = zcoord;
	
	if( !GetPathFindAreaInfo( startzpoint, startxpoint ) && 
		!GetPathFindAreaInfo( endzpoint, endxpoint ) ) // 타게팅된 영역이 갈수 있는 영역일때만 검사.
	{	
		if( !g_LodTerrain.CheckDirectPath( startxpoint, startzpoint, xcoord, zcoord ) ) // 일직선 패스가 가능하면 길찾기 않함.
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Check terrain obb
	arrayx	=	(int)( ((center.x + m_HalfWorldSize ) / m_GridRealSize) );
	arrayz	=	(int)( ((center.z + m_HalfWorldSize ) / m_GridRealSize) );
	
	tilex	=	(arrayx >> 3);
	tilez	=	(arrayz >> 3);
	
	if( tilex >= m_TileCount ) tilex = m_TileCount-1;
	if( tilez >= m_TileCount ) tilez = m_TileCount-1;

	int halfsize = (checksize>>1);
	_XLODTILE* curtile = NULL;

	_XOBBData tileobb;
	{
		tileobb.SetCenter( D3DXVECTOR3(0.0f, 0.0, 0.0f ) );
		tileobb.SetExtents( D3DXVECTOR3(8.0f, 8.0, 8.0f ) );
		D3DXVECTOR3 Axis[3] = 
		{
			D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 0.0f, 1.0f )
		};
		tileobb.SetAxis( Axis );
		tileobb.RecalcRadius();
		
	#ifdef _XDEF_COLLIDETEST
	#ifdef _XDWDEBUG
		tileobb.CreateOBBModel();
		gpDev->SetTransform( D3DTS_WORLD, &m_TerrainMatrix );
	#endif
	#endif
	}

	D3DXVECTOR3 tileraybasepos = startpt;
	tileraybasepos.y = 0.0f;
	D3DXVECTOR3 collidepoint;

	for( int z = -halfsize; z <= halfsize; z++ )
	{
		for( int x = -halfsize; x <= halfsize; x++ )
		{
			// Check tile bounding			
			curtile = GetTerrainTile( tilex + x, tilez + z );

			if( !curtile ) // check outside
			{
				continue;
			}
			
			tileobb.SetCenter( D3DXVECTOR3(curtile->Center.x, 0.0f, curtile->Center.z ) );

			if( (x == 0 && z == 0) ||
				tileobb.InterOBBvsRay( tileraybasepos, raydirection, collidepoint ) )
			{
				tileobb.SetCenter( D3DXVECTOR3(curtile->Center.x, curtile->Center.y, curtile->Center.z ) );
								
#ifdef _XDEF_COLLIDETEST
				tileobb.CreateOBBModel();
				tileobb.m_OBBModel.Render();
#endif

				if( !curtile->svObbList.empty() )
				{
					_XOBBData* pObbData;
					svdef_OBBData::iterator it;
					for (it = curtile->svObbList.begin(); it != curtile->svObbList.end(); ++ it)
					{
						pObbData = (*it);

						if( pObbData->m_OBBLevel == 1 ) // Check collide obb																		
						{
							if( pObbData->InterOBBvsRay( startpt, raydirection, collidepoint ) > 0  )
							{				
#ifdef _XDEF_COLLIDETEST
								pObbData->m_OBBModel.Render( m_TerrainMatrix, pObbData->Axis(), 0xFFFF0000 );
#endif
								return FALSE;
							}
						}
					}
				}				
			}			
		}
	}

	if( m_FunctionalObjectManager.m_countFunctionalObject != 0 )
	{
		for( int i = 0; i < m_FunctionalObjectManager.m_countFunctionalObject; i++ )
		{
			if( m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_bCollidable )// 충돌 가능할 때만 충돌 처리를 한다. ( 문이 부서지면 충돌처리를 하지 않는다.)
			{
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{
					if( (_XFunctionalObject*)g_pLocalUser->m_AttackTarget == &m_FunctionalObjectManager.m_pFunctionalObjectArray[i] )
					{
						continue;
					}
				}
				
				if( m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].InterOBBvsRay( startpt, raydirection, collidepoint ) > 0  )
				{				
#ifdef _XDEF_COLLIDETEST
					m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_OBBModel.Render( m_TerrainMatrix, pObbData->Axis(), 0xFFFF0000 );
#endif
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL _XLodTerrain::FindSafePoint( D3DXVECTOR3 currentPos )
{
	_XOBBData CharacterOBB;

	CharacterOBB.SetCenter( D3DXVECTOR3(0.0f, 0.65, 0.0f ) );
	CharacterOBB.SetExtents( D3DXVECTOR3(0.24f, 0.65f, 0.24f ) );
	D3DXVECTOR3 Axis[3] = 
	{
		D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f )
	};
	CharacterOBB.SetAxis( Axis );
	CharacterOBB.RecalcRadius();

	_XOBBData*	pUnderOBB = NULL;
	D3DXVECTOR3	terrainnormal;
	D3DXVECTOR3 contactpoint;
	POINT		pt;

	BOOL		safepointfound = FALSE;
	int			safepointfoundcount = 0;

	for( FLOAT curradius = 0.5f; curradius < 25.0f; curradius += 0.1f )
	{
		for( FLOAT rotangle = 0.0f; rotangle < 360.0f; rotangle += 1.0f )
		{
			D3DXVECTOR3 newcheckpos = D3DXVECTOR3( 0.0f, 0.0f, curradius );

			D3DXMATRIX rotmat;
			D3DXMatrixRotationY( &rotmat, _X_RAD(rotangle) );
			D3DXVec3TransformCoord( &newcheckpos, &newcheckpos, &rotmat );

			newcheckpos = currentPos + newcheckpos;		
			newcheckpos.y = GetTerrainAndOBBHeightWithCurrentHeight( newcheckpos, pUnderOBB, terrainnormal );		
			CharacterOBB.SetCenter( D3DXVECTOR3(newcheckpos.x, newcheckpos.y + 0.65f, newcheckpos.z ) );

			_XLODTILE*	pTerrainTile = GetTerrainTile( newcheckpos, &pt );
			
			if( !pTerrainTile )
			{
				continue;	
			}

			BOOL collide = FALSE;

			if( !pTerrainTile->svObbList.empty() )
			{
				_XOBBData* pObbData;
				svdef_OBBData::iterator it;
				for (it = pTerrainTile->svObbList.begin(); it != pTerrainTile->svObbList.end(); ++ it)
				{
					pObbData = (*it);
					if( CharacterOBB.IntersectOBBvsOBB( *pObbData, contactpoint ) )
					{
						collide = TRUE;
						break;
					}
				}
			}

			if( !collide ) 
			{
				int	xcoord = _XFC_FloatToIntRet( ( newcheckpos.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
				int zcoord = _XFC_FloatToIntRet( ( newcheckpos.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
				if( xcoord < 0 ) xcoord = 0;
				if( zcoord < 0 ) zcoord = 0;
				if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
				if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;		
				
				if( g_LodTerrain.GetPathFindAreaNeighborInfo(zcoord, xcoord ) < 7 )
				{
					if( !safepointfound )
					{
						safepointfound = TRUE;
						safepointfoundcount = 0;						
					}

					safepointfoundcount++;

					if( safepointfound && safepointfoundcount > 3 ) // 연속해서 3번 이상 이동가능지역을 찾았을 경우.
					{
						// move to new position

						g_pLocalUser->m_Position.x = newcheckpos.x;
						g_pLocalUser->m_Position.z = newcheckpos.z;
						
						_XOBBData* pOBBData = NULL;
						
						g_pLocalUser->SetAnimation(_XACTION_IDLE);
												
						g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndOBBHeight( g_pLocalUser->m_Position, pOBBData );
						g_pLocalUser->UpdatePosition();
												
						g_pLocalUser->m_TargetPosition			= g_pLocalUser->m_Position;
						g_pLocalUser->m_PrevPosition			= g_pLocalUser->m_Position;
						g_pLocalUser->m_FinalTargetPosition		= g_pLocalUser->m_Position;
						g_pLocalUser->m_LeftFinalTargetLength   = 0.0f;
						g_pLocalUser->m_CurrentNavigatePathNodeIndex	=	0;
						g_pLocalUser->m_PathNodeCount			= 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
						
						g_pLocalUser->m_pGoundCollideOBB = pOBBData;
						
						g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
																			   g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
																			   g_pLocalUser->m_Position.z ) );

						g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
						g_LodTerrain.RebuildLevel(FALSE);
						
						_XWindow_WorldMinimap* pminimapwindow = 
							(_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
						if( pminimapwindow )
						{
							// Set direction to minimap arrow 
							pminimapwindow->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
							// Set minimap status
							pminimapwindow->SetLocalUserPosition();
						}
						
						g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );

						return TRUE;
					}
				}
				else
				{
					safepointfound = FALSE;
				}
			}
		}
	}

	g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("이동 가능한 지역을 찾을 수 없었습니다. 마을로 이동하시겠습니까?"), TRUE, _XDEF_CHATDEFAULT_RETURNTOTOWN );
	g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 255, 0 ) );

	return FALSE;
}
