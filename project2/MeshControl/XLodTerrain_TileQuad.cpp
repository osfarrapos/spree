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

void _XLodTerrain::DeleteData_TileQuad()
{
	if (m_pQuadTree != NULL)
	{
		DWORD dwTime = GetTickCount();

		DeleteData_TileQuad(m_pQuadTree);
		SAFE_DELETE(m_pQuadTree);


		dwTime = GetTickCount() - dwTime;

		_XDWINPRINT( "delete quad tree time | %d", dwTime);
	}
}

void _XLodTerrain::DeleteData_TileQuad(XTileQuadTree* pQuadTree)
{
	for (int z = 0; z < QUAD_WIDTH; ++ z)
	{
		for (int x = 0; x < QUAD_WIDTH; ++ x)
		{
			if (pQuadTree->m_pQuadTree[z][x] != NULL)
			{
				DeleteData_TileQuad(pQuadTree->m_pQuadTree[z][x]);
				SAFE_DELETE(pQuadTree->m_pQuadTree[z][x]);
			}
		}
	}
}

void _XLodTerrain::Initial_TileQuad()
{
	DWORD dwTime = GetTickCount();

	DeleteData_TileQuad();

	// Quad Tree 의 갯수는 타일의 갯수보다 2배 만아야 한다.
	// 그래야 타일의 중간에서 타일의 정확한 프러스텀 컬링을 할수 있다.
	int	nQuadCount = m_TileCount << 1;

	m_pQuadTree = new XTileQuadTree;

	Initial_TileQuad(nQuadCount >> 1, nQuadCount >> 1, nQuadCount >> 1, m_pQuadTree);

	dwTime = GetTickCount() - dwTime;
	
	_XDWINPRINT( "initial quad tree time %d", dwTime);
}

void _XLodTerrain::Initial_TileQuad(int nQuadZ, int nQuadX, int nSize, XTileQuadTree* pQuadTree)
{
	int nSize2 = nSize >> 1;

	pQuadTree->m_vCenter.x	= -m_HalfWorldSize + (FLOAT)(nQuadX / 2.f) * (m_GridRealSize * 8);
	pQuadTree->m_vCenter.z	= -m_HalfWorldSize + (FLOAT)(nQuadZ / 2.f) * (m_GridRealSize * 8);
	pQuadTree->m_vCenter.y	= pQuadTree->m_vCenter.y;//GetTerrainHeight(pQuadTree->m_vCenter);

	// nSize 가 1이 돼면 nSize >> 1 이 0 이 되므로 정확한 반지름을 구할수 없다.
	// 그래서 ((FLOAT)nSize / 2.f) 로 한다.
	FLOAT fQuadDist = ((FLOAT)nSize / 2.f) * (m_GridRealSize * 8);
//	pQuadTree->m_fRadius	= sqrtf((fQuadDist * fQuadDist) * 2);
//	원래는 위의 식을 써야 하나 프러스텀 약간의 오차로 인해 반지름을 약간 크게 잡아준다.
	pQuadTree->m_fRadius	= fQuadDist * 2.f;

	if (nSize == 1)
	{
		pQuadTree->m_nTileZ		= nQuadZ >> 1;
		pQuadTree->m_nTileX		= nQuadX >> 1;
		pQuadTree->m_bLeaf		= TRUE;

		assert(pQuadTree->m_nTileZ >= 0 && pQuadTree->m_nTileX >= 0);
		assert(pQuadTree->m_nTileZ < m_TileCount && pQuadTree->m_nTileX < m_TileCount);

		pQuadTree->m_pTile		= &m_TerrainTile[m_TileStartOffset + pQuadTree->m_nTileX][m_TileStartOffset + pQuadTree->m_nTileZ];
		return;
	}
	
	for (int z = 0; z < QUAD_WIDTH; ++ z)
	{
		for (int x = 0; x < QUAD_WIDTH; ++ x)
		{
			pQuadTree->m_pQuadTree[z][x] = new XTileQuadTree;
		}
	}

	Initial_TileQuad(nQuadZ - nSize2, nQuadX - nSize2, nSize2, pQuadTree->m_pQuadTree[0][0]);
	Initial_TileQuad(nQuadZ - nSize2, nQuadX + nSize2, nSize2, pQuadTree->m_pQuadTree[0][1]);
	Initial_TileQuad(nQuadZ + nSize2, nQuadX - nSize2, nSize2, pQuadTree->m_pQuadTree[1][0]);
	Initial_TileQuad(nQuadZ + nSize2, nQuadX + nSize2, nSize2, pQuadTree->m_pQuadTree[1][1]);
}

void _XLodTerrain::RebuildLevel_LODDetailLevel( void )
{
	_XLODTILE*	 pCurrentTile;
	
	for (int i=0;i<m_TileCount;++i)
	{
		for (int j=0;j<m_TileCount;++j)
		{						
			pCurrentTile = &m_TerrainTile[m_TileStartOffset+i][m_TileStartOffset+j];
			pCurrentTile->DistanceToCamera = _XFastDistance( pCurrentTile->Center.x - g_pLocalUser->m_Position.x,
															 pCurrentTile->Center.z - g_pLocalUser->m_Position.z);

			// Check inside frustum 
			if( pCurrentTile->DistanceToCamera <= m_LODFarPlane + 30.0f )
			{
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
			}
			else
			{
				pCurrentTile->DetailLevel = LEVEL_0;
			}
		}
	}		
}


void _XLodTerrain::RebuildLevel_TileQuad()
{
	assert(m_pQuadTree);
	m_pWaterObjectManager->ResetFrustumInfo();
	//RebuildLevel_LODDetailLevel();
	RebuildLevel_TileQuad(m_pQuadTree);	
}

void _XLodTerrain::RebuildLevel_TileQuad(XTileQuadTree* pQuadTree)
{		
	if (pQuadTree->m_bLeaf == FALSE)
	{
		if (m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pQuadTree->m_vCenter, pQuadTree->m_fRadius) == NOT_VISIBLE)
		{
			return;
		}
	}
	else
	{
		pQuadTree->m_pTile->DistanceToCamera = _XFastDistance( pQuadTree->m_pTile->Center.x - g_pLocalUser->m_Position.x,
															   pQuadTree->m_pTile->Center.z - g_pLocalUser->m_Position.z);
		// Check inside frustum 
		if( pQuadTree->m_pTile->DistanceToCamera <= m_LODFarPlane + 30.0f )
		{
			if ( pQuadTree->m_pTile->DistanceToCamera <= 32.0f ||
				 (m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pQuadTree->m_vCenter, pQuadTree->m_fRadius) != NOT_VISIBLE) )
			{
				assert(pQuadTree->m_pTile);

				// Rebuild detail level....
				if( pQuadTree->m_pTile->DistanceToCamera < m_LODLevel3 )
				{
					pQuadTree->m_pTile->DetailLevel = LEVEL_3;
				}
				else if ( pQuadTree->m_pTile->DistanceToCamera < m_LODLevel2)
				{
					pQuadTree->m_pTile->DetailLevel = LEVEL_2;
				}
				else if ( pQuadTree->m_pTile->DistanceToCamera < m_LODLevel1)
				{
					pQuadTree->m_pTile->DetailLevel = LEVEL_1;
				}
				else 
				{
					pQuadTree->m_pTile->DetailLevel = LEVEL_0;
				}
				
				if(pQuadTree->m_pTile->DetailLevel >= LEVEL_2)
				{
					// Insert tile rendering cache
					for( int splatlayer = 0; splatlayer < pQuadTree->m_pTile->UsedSplatLayerCount; splatlayer++ )
					{
						int curlayerindex = pQuadTree->m_pTile->SplatData[splatlayer].m_LayerIndex; // org layer index						
						m_TerrainTileRenderingCache[curlayerindex][m_TerrainTileRenderingCacheLength[curlayerindex]].xcoord = pQuadTree->m_nTileZ;
						m_TerrainTileRenderingCache[curlayerindex][m_TerrainTileRenderingCacheLength[curlayerindex]].zcoord = pQuadTree->m_nTileX;
						m_TerrainTileRenderingCache[curlayerindex][m_TerrainTileRenderingCacheLength[curlayerindex]].layerindex = splatlayer; // layer stack index

						m_TerrainTileRenderingCacheLength[curlayerindex]++;
					}

					m_TerrainTileRenderingCache[m_UsedLayerCount][m_TerrainTileRenderingCacheLength[m_UsedLayerCount]].xcoord = pQuadTree->m_nTileZ;
					m_TerrainTileRenderingCache[m_UsedLayerCount][m_TerrainTileRenderingCacheLength[m_UsedLayerCount]].zcoord = pQuadTree->m_nTileX;
					m_TerrainTileRenderingCache[m_UsedLayerCount][m_TerrainTileRenderingCacheLength[m_UsedLayerCount]].layerindex = 0;
					m_TerrainTileRenderingCacheLength[m_UsedLayerCount]++;

					// Water reflection tile
					if( pQuadTree->m_pTile->RenderReflectionMap )
					{
						int arrayindex = m_UsedLayerCount+2;
						m_TerrainTileRenderingCache[arrayindex][m_TerrainTileRenderingCacheLength[arrayindex]].xcoord = pQuadTree->m_nTileZ;
						m_TerrainTileRenderingCache[arrayindex][m_TerrainTileRenderingCacheLength[arrayindex]].zcoord = pQuadTree->m_nTileX;
						m_TerrainTileRenderingCache[arrayindex][m_TerrainTileRenderingCacheLength[arrayindex]].layerindex = 0;
					
						m_TerrainTileRenderingCacheLength[arrayindex]++;
					}
				}
				// Use prerendered image layer
				else
				{
					int arrayindex = m_UsedLayerCount+1;
					m_TerrainTileRenderingCache[arrayindex][m_TerrainTileRenderingCacheLength[arrayindex]].xcoord = pQuadTree->m_nTileZ;
					m_TerrainTileRenderingCache[arrayindex][m_TerrainTileRenderingCacheLength[arrayindex]].zcoord = pQuadTree->m_nTileX;
					m_TerrainTileRenderingCache[arrayindex][m_TerrainTileRenderingCacheLength[arrayindex]].layerindex = 0;
					m_TerrainTileRenderingCacheLength[arrayindex]++;
				}
			}
		}
		else
		{
			pQuadTree->m_pTile->DetailLevel = LEVEL_0;
		}

		// Water
		int baseindex = pQuadTree->m_nTileX * m_TileCount + pQuadTree->m_nTileZ;
		S_WATER* pWaterPiece = m_pWaterObjectManager->GetWaterObject(baseindex);
		if( pWaterPiece )
		{
			if( pWaterPiece->enabled )
			{
				m_pWaterObjectManager->SetFrustum(baseindex, TRUE);

				if( m_LastCheckedDistanceWaterpiece > pQuadTree->m_pTile->DistanceToCamera )
				{
					m_LastCheckedDistanceWaterpiece = pQuadTree->m_pTile->DistanceToCamera;

					m_pWaterObjectManager->m_WaterReflectionClipPlaneHeight = pWaterPiece->height;
				}
			}
		}
		
		// Leaf 이기 때문에 더 이상 재귀호출할 필요가 없다.
		return;
	}

	for (int z = 0; z < QUAD_WIDTH; ++ z)
	{
		for (int x = 0; x < QUAD_WIDTH; ++ x)
		{
			assert(pQuadTree->m_pQuadTree[z][x]);

			RebuildLevel_TileQuad(pQuadTree->m_pQuadTree[z][x]);
		}
	}
}
