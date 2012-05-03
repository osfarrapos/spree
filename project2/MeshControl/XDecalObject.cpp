// XDecalObject.cpp: implementation of the _XDecalObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XFastCode.h"
#include "XDecalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const FLOAT _XDEF_MINDECALSIZE =	0.1f;
static const FLOAT _XDEF_MAXDECALSIZE =	16.0f;
D3DXVECTOR3 g_DecalUpVector = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
D3DXVECTOR3 g_DecalTangentVector = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );

D3DXVECTOR3		g_Decal_VertexArray[g_MaxDecalVertices];
D3DXCOLOR		g_Decal_ColorArray[g_MaxDecalVertices];
_XTriangle		g_Decal_TriangleArray[g_MaxDecalVertices];

D3DXVECTOR4		g_Decal_leftPlane;
D3DXVECTOR4		g_Decal_rightPlane;
D3DXVECTOR4		g_Decal_bottomPlane;
D3DXVECTOR4		g_Decal_topPlane;
D3DXVECTOR4		g_Decal_frontPlane;
D3DXVECTOR4		g_Decal_backPlane;
D3DXVECTOR3		g_Decal_decalNormal;
FLOAT			g_Decal_decalEpsilon = 0.000005f;


inline float DotProduct(const D3DXVECTOR4& p, const D3DXVECTOR3& q)
{
	return (p.x * q.x + p.y * q.y + p.z * q.z + p.w);
}


//////////////////////////////////////////////////////////////////////////
// Particle decal

_XDecalObject::_XDecalObject()
{
	m_decalVertexCount = 0;
	m_createddecalVertexCount = 0;
	m_decalTriangleCount = 0;
	m_createdTriangleCount = 0;
	
	m_pDecalVertices = NULL;
	m_pDecalIndeces  = NULL;

	m_decalWidth  = 0.0f;
	m_decalHeight = 0.0f;

	m_decalCenter = D3DXVECTOR3( -99999.0f,-99999.0f,-99999.0f );
}

_XDecalObject::~_XDecalObject()
{
	SAFE_RELEASE( m_pDecalVertices );
	SAFE_RELEASE( m_pDecalIndeces  );
}

void _XDecalObject::Release( void )
{
	m_decalVertexCount = 0;
	m_createddecalVertexCount = 0;
	m_decalTriangleCount = 0;
	m_createdTriangleCount = 0;
	
	SAFE_RELEASE( m_pDecalVertices );
	SAFE_RELEASE( m_pDecalIndeces  );

	m_decalWidth  = 0.0f;
	m_decalHeight = 0.0f;

	m_decalCenter = D3DXVECTOR3( -99999.0f,-99999.0f,-99999.0f );
}

void _XDecalObject::InvalidateDeviceObject( void )
{
	SAFE_RELEASE( m_pDecalVertices );
	SAFE_RELEASE( m_pDecalIndeces  );
	m_createddecalVertexCount = 0;
	m_createdTriangleCount = 0;
}

void _XDecalObject::RestoreDeviceObject( void )
{
	GenerateDecal( m_decalCenter, g_DecalUpVector, g_DecalTangentVector, m_decalWidth, m_decalHeight, 0.8f );
}

BOOL _XDecalObject::GenerateDecal(const D3DXVECTOR3& center, const D3DXVECTOR3& normal, const D3DXVECTOR3& tangent, 
								  float width, float height, float depth )
{
	if( width  < _XDEF_MINDECALSIZE ) width  = _XDEF_MINDECALSIZE;
	if( height < _XDEF_MINDECALSIZE ) height = _XDEF_MINDECALSIZE;
	if( width  > _XDEF_MAXDECALSIZE ) width  = _XDEF_MAXDECALSIZE;
	if( height > _XDEF_MAXDECALSIZE ) height = _XDEF_MAXDECALSIZE;

	if( m_pDecalVertices )
	{
		if( m_decalCenter == center && m_decalWidth == width && m_decalHeight == height ) return TRUE;
	}

	InvalidateDeviceObject();

	m_decalCenter = center;
	g_Decal_decalNormal = normal;
	m_decalWidth  = width;
	m_decalHeight = height;
	
	D3DXVECTOR3 binormal;
	D3DXVec3Cross( &binormal, &normal, &tangent );
	
	// Calculate boundary planes
	float d = D3DXVec3Dot( &center, &tangent );
	g_Decal_leftPlane = D3DXVECTOR4(tangent.x, tangent.y, tangent.z, width * 0.5F - d);
	g_Decal_rightPlane = D3DXVECTOR4(-tangent.x, -tangent.y, -tangent.z, width * 0.5F + d);
	
	d = D3DXVec3Dot( &center, &binormal);
	g_Decal_bottomPlane = D3DXVECTOR4(binormal.x, binormal.y, binormal.z, height * 0.5F - d);
	g_Decal_topPlane = D3DXVECTOR4(-binormal.x, -binormal.y, -binormal.z, height * 0.5F + d);
	
	d = D3DXVec3Dot( &center, &normal );
	g_Decal_frontPlane = D3DXVECTOR4(-normal.x, -normal.y, -normal.z, depth + d);
	g_Decal_backPlane = D3DXVECTOR4(normal.x, normal.y, normal.z, depth - d);
	
	// Begin with empty mesh
	m_decalVertexCount = 0;
	m_decalTriangleCount = 0;
	
	_XOBBData* pOBB;
	FLOAT terrainheight = -9999.0f;
	FLOAT terrainobbheight = g_LodTerrain.GetTerrainOBBHeight( m_decalCenter, pOBB, terrainheight );
	
	if( pOBB )
	{
		if( fabs(terrainobbheight - terrainheight ) > 0.2f ) ClipMesh( FALSE );
		else ClipMesh( TRUE );
	}
	else
	{
		ClipMesh( TRUE );
	}
	
	// Assign texture mapping coordinates
	float one_over_w = 1.0F / width;
	float one_over_h = 1.0F / height;

	if( m_decalVertexCount > 0 )
	{
		if( m_decalVertexCount != m_createddecalVertexCount )
		{
			SAFE_RELEASE( m_pDecalVertices );
			if( FAILED(gpDev->CreateVertexBuffer( m_decalVertexCount*sizeof(_XDECALVERTEXTYPE), g_3DObjectMemoryUsage, 
												  _XFVF_DECALVERTEX, g_3DObjectMemoryPool, &m_pDecalVertices, NULL )) ) 
			{		
				InvalidateDeviceObject();
				_XFatalError("Can't create decal vertex buffer [vertcount = %d]", m_decalVertexCount );
				return FALSE;
			}
			m_createddecalVertexCount = m_decalVertexCount;
		}

		_XDECALVERTEXTYPE* pVertices;
		if( FAILED(m_pDecalVertices->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods ) ) )
		{				
			InvalidateDeviceObject();
			_XFatalError("Can't lock decal vertex buffer" );
			return FALSE;
		}
		
		for (long a = 0; a < m_decalVertexCount; a++)
		{
			D3DXVECTOR3 v		= g_Decal_VertexArray[a] - m_decalCenter;

			pVertices[a].p.x = g_Decal_VertexArray[a].x;
			pVertices[a].p.y = g_Decal_VertexArray[a].y + 0.03f;
			pVertices[a].p.z = g_Decal_VertexArray[a].z;

			pVertices[a].tu		= D3DXVec3Dot(&v, &tangent) * one_over_w + 0.5F;
			pVertices[a].tv		= D3DXVec3Dot(&v, &binormal) * one_over_h + 0.5F;
			pVertices[a].color	= g_Decal_ColorArray[a];
		}

		m_pDecalVertices->Unlock();
	}

	if( m_decalTriangleCount > 0 )
	{
		if( m_decalTriangleCount != m_createdTriangleCount )
		{
			SAFE_RELEASE( m_pDecalIndeces );
			if (FAILED(gpDev->CreateIndexBuffer(m_decalTriangleCount*sizeof(WORD)*3, g_3DObjectMemoryUsage, D3DFMT_INDEX16, 
												g_3DObjectMemoryPool, &m_pDecalIndeces , NULL)))
			{
				InvalidateDeviceObject();
				_XFatalError("Can't create decal vertex buffer [indexcount = %d]", m_decalTriangleCount * 3 );
				return FALSE;
			}
			m_createdTriangleCount = m_decalTriangleCount;
		}

		LPWORD pIndices;
		if( SUCCEEDED( m_pDecalIndeces->Lock( 0, m_decalTriangleCount*sizeof(WORD)*3, (void**)&pIndices, g_3DObjectLockMethods )) )
		{
			memcpy( pIndices, g_Decal_TriangleArray, m_decalTriangleCount*sizeof(WORD)*3 );		
			m_pDecalIndeces->Unlock();
		}
	}

	return TRUE;
}

void _XDecalObject::ClipMesh( BOOL computeterrainvertices )
{
	POINT tilecoord;
	D3DXVECTOR3		newVertex[9];
	D3DXVECTOR3		newNormal[9];
	LPWORD			pIndexBuffer;
	D3DXVECTOR3*	pVertex;
	_XLODTILE*		pTile;

	newNormal[2] = newNormal[1] = newNormal[0] = D3DXVECTOR3(0.0f,1.0f,0.0f);

	g_LodTerrain.GetTerrainTile( m_decalCenter, &tilecoord );

	for( int z = tilecoord.y-1; z <= tilecoord.y+1; z++ )
	{
		for( int x = tilecoord.x-1; x <= tilecoord.x+1; x++ )
		{
			pTile = g_LodTerrain.GetTerrainTile( x, z );
			if( !pTile ) continue;
			
			if( computeterrainvertices )
			{
				pIndexBuffer = g_TerrainIndexLUT;
				pVertex		 = pTile->VertexBuffer;
				
				// Clip one triangle at a time
				for (long triangleindex = 0; triangleindex < 128; triangleindex++)
				{
					WORD i1 = *pIndexBuffer;
					WORD i2 = *(pIndexBuffer+1);
					WORD i3 = *(pIndexBuffer+2);
					
					const D3DXVECTOR3& v1 = pVertex[i1];
					const D3DXVECTOR3& v2 = pVertex[i2];
					const D3DXVECTOR3& v3 = pVertex[i3];
					
					D3DXVECTOR3 cross;
					D3DXVECTOR3 a = v2 - v1;
					D3DXVECTOR3 b = v3 - v1;
					D3DXVec3Cross( &cross, &a, &b );
					
					if (D3DXVec3Dot(&g_Decal_decalNormal, &cross) > g_Decal_decalEpsilon * D3DXVec3Length( &cross) )
					{
						newVertex[0] = v1;
						newVertex[1] = v2;
						newVertex[2] = v3;
						
						//newNormal[0] = pVertex[i1].n;
						//newNormal[1] = pVertex[i2].n;
						//newNormal[2] = pVertex[i3].n;
						
						long count = ClipPolygon(3, newVertex, newNormal, newVertex, newNormal);
						if ((count != 0) && (!AddPolygon(count, newVertex, newNormal))) break;			
					}
					
					pIndexBuffer+=3;
				}
			}

			if( tilecoord.x == x && tilecoord.y == z )
			{
				if( !pTile->svObbList.empty() )
				{
					_XOBBData* pobbData;
					svdef_OBBData::iterator it;
					for (it = pTile->svObbList.begin(); it != pTile->svObbList.end(); ++ it)
					{
						pobbData = (*it);
						if(pobbData->m_OBBLevel != 0) continue;
						
						LPWORD pIndexBuffer = g_OBBModelVertIndex;
						
						for (long triangleindex = 0; triangleindex < 12; triangleindex++)
						{
							const D3DXVECTOR3& v1 = pobbData->m_ModelVertex[pIndexBuffer[0]];
							const D3DXVECTOR3& v2 = pobbData->m_ModelVertex[pIndexBuffer[1]];
							const D3DXVECTOR3& v3 = pobbData->m_ModelVertex[pIndexBuffer[2]];
							
							D3DXVECTOR3 cross;
							D3DXVECTOR3 a = v2 - v1;
							D3DXVECTOR3 b = v3 - v1;
							D3DXVec3Cross( &cross, &a, &b );
							
							if (D3DXVec3Dot(&g_Decal_decalNormal, &cross) > g_Decal_decalEpsilon * D3DXVec3Length( &cross) )
							{
								newVertex[0] = v1;
								newVertex[1] = v2;
								newVertex[2] = v3;
								
								long count = ClipPolygon(3, newVertex, newNormal, newVertex, newNormal);
								if ((count != 0) && (!AddPolygon(count, newVertex, newNormal))) break;			
							}
							else
							{
								D3DXVec3Cross( &cross, &b, &a );
								if (D3DXVec3Dot(&g_Decal_decalNormal, &cross) > g_Decal_decalEpsilon * D3DXVec3Length( &cross) )
								{
									newVertex[0] = v1;
									newVertex[1] = v3;
									newVertex[2] = v2;
									
									long count = ClipPolygon(3, newVertex, newNormal, newVertex, newNormal);
									if ((count != 0) && (!AddPolygon(count, newVertex, newNormal))) break;
								}
							}
							
							pIndexBuffer+=3;
						}
					}
				}
			}
		}
	}	
}

bool _XDecalObject::AddPolygon(long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal)
{
	long count = m_decalVertexCount;
	if (count + vertexCount >= g_MaxDecalVertices) return (false);
	
	// Add polygon as a triangle fan
	_XTriangle *triangle = g_Decal_TriangleArray + m_decalTriangleCount;
	m_decalTriangleCount += vertexCount - 2;
	for (long a = 2; a < vertexCount; a++)
	{
		triangle->index[0] = (unsigned short) count;
		triangle->index[1] = (unsigned short) (count + a - 1);
		triangle->index[2] = (unsigned short) (count + a);
		triangle++;
	}
	
	// Assign vertex colors
	float f = 1.0F / (1.0F - g_Decal_decalEpsilon);
	for (long b = 0; b < vertexCount; b++)
	{
		g_Decal_VertexArray[count] = vertex[b];
		const D3DXVECTOR3& n = normal[b];
		float alpha = (D3DXVec3Dot(&g_Decal_decalNormal, &n) / D3DXVec3Length( &n ) - g_Decal_decalEpsilon) * f;
		g_Decal_ColorArray[count] = D3DXCOLOR(1.0F, 1.0F, 1.0F, (alpha > 0.0F) ? alpha : 0.0F);
		count++;
	}
	
	m_decalVertexCount = count;
	return (true);
}

long _XDecalObject::ClipPolygon(long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal, D3DXVECTOR3 *newVertex, D3DXVECTOR3 *newNormal) const
{
	D3DXVECTOR3		tempVertex[9];
	D3DXVECTOR3		tempNormal[9];
	
	// Clip against all six planes
	long count = ClipPolygonAgainstPlane(g_Decal_leftPlane, vertexCount, vertex, normal, tempVertex, tempNormal);
	if (count != 0)
	{
		count = ClipPolygonAgainstPlane(g_Decal_rightPlane, count, tempVertex, tempNormal, newVertex, newNormal);
		if (count != 0)
		{
			count = ClipPolygonAgainstPlane(g_Decal_bottomPlane, count, newVertex, newNormal, tempVertex, tempNormal);
			if (count != 0)
			{
				count = ClipPolygonAgainstPlane(g_Decal_topPlane, count, tempVertex, tempNormal, newVertex, newNormal);
				if (count != 0)
				{
					count = ClipPolygonAgainstPlane(g_Decal_backPlane, count, newVertex, newNormal, tempVertex, tempNormal);
					if (count != 0)
					{
						count = ClipPolygonAgainstPlane(g_Decal_frontPlane, count, tempVertex, tempNormal, newVertex, newNormal);
					}
				}
			}
		}
	}
	
	return (count);
}

long _XDecalObject::ClipPolygonAgainstPlane(const D3DXVECTOR4& plane, long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal, D3DXVECTOR3 *newVertex, D3DXVECTOR3 *newNormal)
{
	bool	negative[10];
	
	// Classify vertices
	long negativeCount = 0;
	for (long a = 0; a < vertexCount; a++)
	{
		bool neg = (DotProduct(plane, vertex[a]) < 0.0F);
		negative[a] = neg;
		negativeCount += neg;
	}
	
	// Discard this polygon if it's completely culled
	if (negativeCount == vertexCount) return (0);
	
	long count = 0;
	for (long b = 0; b < vertexCount; b++)
	{
		// c is the index of the previous vertex
		long c = (b != 0) ? b - 1 : vertexCount - 1;
		
		if (negative[b])
		{
			if (!negative[c])
			{
				// Current vertex is on negative side of plane,
				// but previous vertex is on positive side.
				const D3DXVECTOR3& v1 = vertex[c];
				const D3DXVECTOR3& v2 = vertex[b];
				float t = DotProduct(plane, v1) / (plane.x * (v1.x - v2.x) + plane.y * (v1.y - v2.y) + plane.z * (v1.z - v2.z));
				newVertex[count] = v1 * (1.0F - t) + v2 * t;
				
				const D3DXVECTOR3& n1 = normal[c];
				const D3DXVECTOR3& n2 = normal[b];
				newNormal[count] = n1 * (1.0F - t) + n2 * t;
				count++;
			}
		}
		else
		{
			if (negative[c])
			{
				// Current vertex is on positive side of plane,
				// but previous vertex is on negative side.
				const D3DXVECTOR3& v1 = vertex[b];
				const D3DXVECTOR3& v2 = vertex[c];
				float t = DotProduct(plane, v1) / (plane.x * (v1.x - v2.x) + plane.y * (v1.y - v2.y) + plane.z * (v1.z - v2.z));
				newVertex[count] = v1 * (1.0F - t) + v2 * t;
				
				const D3DXVECTOR3& n1 = normal[b];
				const D3DXVECTOR3& n2 = normal[c];
				newNormal[count] = n1 * (1.0F - t) + n2 * t;
				count++;
			}
			
			// Include current vertex
			newVertex[count] = vertex[b];
			newNormal[count] = normal[b];
			count++;
		}
	}
	
	// Return number of vertices in clipped polygon
	return (count);
}

void _XDecalObject::Render( _XTextureManager* pTextureManager, int textureindex )
{
	if( !pTextureManager || !m_decalTriangleCount ) return;
	
	gpDev->SetFVF( _XFVF_DECALVERTEX );
	pTextureManager->SetTexture( textureindex );

	gpDev->SetStreamSource( 0, m_pDecalVertices, 0, sizeof(_XDECALVERTEXTYPE) );
	gpDev->SetIndices( m_pDecalIndeces );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_decalVertexCount, 0, m_decalTriangleCount );
}

void _XDecalObject::Render( _XRenderStyleManager* pRenderStyleManager, _XTextureManager* pTextureManager, int renderstyleindex )
{
	if( !pRenderStyleManager || !pTextureManager || !m_decalTriangleCount  ) return;
	
	gpDev->SetFVF( _XFVF_DECALVERTEX );
	
	pRenderStyleManager->SetRenderStyle( pTextureManager, renderstyleindex );		
	gpDev->SetStreamSource( 0, m_pDecalVertices, 0, sizeof(_XDECALVERTEXTYPE) );
	gpDev->SetIndices( m_pDecalIndeces );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_decalVertexCount, 0, m_decalTriangleCount );
}

void _XDecalObject::Render( void )
{
	//gpDev->SetFVF( _XFVF_DECALVERTEX );		
	gpDev->SetStreamSource( 0, m_pDecalVertices, 0, sizeof(_XDECALVERTEXTYPE) );
	gpDev->SetIndices( m_pDecalIndeces );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_decalVertexCount, 0, m_decalTriangleCount );
}



//////////////////////////////////////////////////////////////////////////
// Shadow decal

_XShadowDecalObject::_XShadowDecalObject()
{
	m_decalVertexCount = 0;
	m_createddecalVertexCount = 0;
	m_decalTriangleCount = 0;
	m_createdTriangleCount = 0;
	
	m_pDecalVertices = NULL;
	m_pDecalIndeces  = NULL;

	m_decalWidth  = 0.0f;
	m_decalHeight = 0.0f;

	m_decalCenter = D3DXVECTOR3( -99999.0f,-99999.0f,-99999.0f );
}

_XShadowDecalObject::~_XShadowDecalObject()
{
	SAFE_RELEASE( m_pDecalVertices );
	SAFE_RELEASE( m_pDecalIndeces  );
}

void _XShadowDecalObject::InvalidateDeviceObject( void )
{
	SAFE_RELEASE( m_pDecalVertices );
	SAFE_RELEASE( m_pDecalIndeces  );	
	m_createddecalVertexCount = 0;
	m_createdTriangleCount = 0;
}

void _XShadowDecalObject::RestoreDeviceObject( void )
{
	//GenerateDecal( m_decalCenter, g_DecalUpVector, g_DecalTangentVector, m_decalWidth, m_decalHeight, 0.8f );
}

BOOL _XShadowDecalObject::GenerateDecal(const D3DXVECTOR3& center, float width, float height, float depth )
{
	if( width  < _XDEF_MINDECALSIZE ) width  = _XDEF_MINDECALSIZE;
	if( height < _XDEF_MINDECALSIZE ) height = _XDEF_MINDECALSIZE;
	if( width  > _XDEF_MAXDECALSIZE ) width  = _XDEF_MAXDECALSIZE;
	if( height > _XDEF_MAXDECALSIZE ) height = _XDEF_MAXDECALSIZE;

	if( m_pDecalVertices )
	{
		if( (fabs(fabs(m_decalCenter.x) - fabs(center.x)) < 0.1f) &&
			(fabs(fabs(m_decalCenter.y) - fabs(center.y)) < 0.1f) &&
			(fabs(fabs(m_decalCenter.z) - fabs(center.z)) < 0.1f) ) return TRUE;
	}

	m_decalCenter = center;
	g_Decal_decalNormal = g_DecalUpVector;
	m_decalWidth  = width;
	m_decalHeight = height;
	
	D3DXVECTOR3 binormal;
	D3DXVec3Cross( &binormal, &g_DecalUpVector, &g_DecalTangentVector );
	
	// Calculate boundary planes
	float d = D3DXVec3Dot( &center, &g_DecalTangentVector );
	g_Decal_leftPlane = D3DXVECTOR4(g_DecalTangentVector.x, g_DecalTangentVector.y, g_DecalTangentVector.z, width * 0.5F - d);
	g_Decal_rightPlane = D3DXVECTOR4(-g_DecalTangentVector.x, -g_DecalTangentVector.y, -g_DecalTangentVector.z, width * 0.5F + d);
	
	d = D3DXVec3Dot( &center, &binormal);
	g_Decal_bottomPlane = D3DXVECTOR4(binormal.x, binormal.y, binormal.z, height * 0.5F - d);
	g_Decal_topPlane = D3DXVECTOR4(-binormal.x, -binormal.y, -binormal.z, height * 0.5F + d);
	
	d = D3DXVec3Dot( &center, &g_DecalUpVector );
	g_Decal_frontPlane = D3DXVECTOR4(-g_DecalUpVector.x, -g_DecalUpVector.y, -g_DecalUpVector.z, depth + d);
	g_Decal_backPlane = D3DXVECTOR4(g_DecalUpVector.x, g_DecalUpVector.y, g_DecalUpVector.z, depth - d);
	
	// Begin with empty mesh
	m_decalVertexCount = 0;
	m_decalTriangleCount = 0;
	
	_XOBBData* pOBB;
	FLOAT terrainheight = -9999.0f;
	FLOAT terrainobbheight = g_LodTerrain.GetTerrainOBBHeight( m_decalCenter, pOBB, terrainheight );

	if( pOBB )
	{
		if( fabs(terrainobbheight - terrainheight ) > 0.2f ) ClipMesh( FALSE );
		else ClipMesh( TRUE );
	}
	else
	{
		ClipMesh( TRUE );
	}

	if( m_decalVertexCount > 0  )
	{
		if( m_decalVertexCount != m_createddecalVertexCount )
		{
			SAFE_RELEASE( m_pDecalVertices );
			if( FAILED(gpDev->CreateVertexBuffer( m_decalVertexCount*sizeof(D3DXVECTOR3), g_3DObjectMemoryUsage, 
												  D3DFVF_XYZ, g_3DObjectMemoryPool, &m_pDecalVertices, NULL )) ) 
			{		
				InvalidateDeviceObject();
				_XFatalError("Can't create shadow decal vertex buffer [vertcount = %d]", m_decalVertexCount );
				return FALSE;
			}
			m_createddecalVertexCount = m_decalVertexCount;
		}

		D3DXVECTOR3* pVertices;
		if( FAILED(m_pDecalVertices->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods ) ) )
		{				
			InvalidateDeviceObject();
			_XFatalError("Can't lock shadow decal vertex buffer" );
			return FALSE;
		}
		
		for (long a = 0; a < m_decalVertexCount; a++)
		{
			pVertices[a].x = g_Decal_VertexArray[a].x;
			pVertices[a].y = g_Decal_VertexArray[a].y + 0.015f;
			pVertices[a].z = g_Decal_VertexArray[a].z;
		}

		m_pDecalVertices->Unlock();
	}

	if( m_decalTriangleCount > 0 )
	{
		if( m_decalTriangleCount != m_createdTriangleCount )
		{
			SAFE_RELEASE( m_pDecalIndeces );
			if (FAILED(gpDev->CreateIndexBuffer(m_decalTriangleCount*sizeof(WORD)*3, g_3DObjectMemoryUsage, D3DFMT_INDEX16, 
												g_3DObjectMemoryPool, &m_pDecalIndeces , NULL)))
			{
				InvalidateDeviceObject();
				_XFatalError("Can't create shadow decal vertex buffer [indexcount = %d]", m_decalTriangleCount * 3 );
				return FALSE;
			}
			m_createdTriangleCount = m_decalTriangleCount;
		}

		LPWORD pIndices;
		if( SUCCEEDED( m_pDecalIndeces->Lock( 0, m_decalTriangleCount*sizeof(WORD)*3, (void**)&pIndices, g_3DObjectLockMethods )) )
		{
			memcpy( pIndices, g_Decal_TriangleArray, m_decalTriangleCount*sizeof(WORD)*3 );		
			m_pDecalIndeces->Unlock();
		}
	}

	return TRUE;
}

void _XShadowDecalObject::ClipMesh( BOOL computeterrainvertices )
{
	POINT tilecoord;
	D3DXVECTOR3		newVertex[9];
	D3DXVECTOR3		newNormal[9];
	D3DXVECTOR3		terrainVertex[6];
		
	newNormal[2] = newNormal[1] = newNormal[0] = D3DXVECTOR3(0.0f,1.0f,0.0f);

	int stposx, stposz;
	int endposx, endposz;
	
	stposx	=	(int)( ((m_decalCenter.x-(m_decalWidth*0.5f) + g_LodTerrain.m_HalfWorldSize ) / g_LodTerrain.m_GridRealSize) );
	stposz	=	(int)( ((m_decalCenter.z-(m_decalHeight*0.5f) + g_LodTerrain.m_HalfWorldSize ) / g_LodTerrain.m_GridRealSize) );

	endposx	=	(int)( ((m_decalCenter.x+(m_decalWidth*0.5f) + g_LodTerrain.m_HalfWorldSize ) / g_LodTerrain.m_GridRealSize) );
	endposz	=	(int)( ((m_decalCenter.z+(m_decalHeight*0.5f) + g_LodTerrain.m_HalfWorldSize ) / g_LodTerrain.m_GridRealSize) );

	if( computeterrainvertices )
	{
		for( int z = stposz; z <= endposz; z++ )
		{
			for( int x = stposx; x <= endposx; x++ )
			{
				g_LodTerrain.GetTerrainQuadPolygon( x, z, terrainVertex );
				// Clip one triangle at a time
				LPD3DXVECTOR3 pVertex = terrainVertex;
				for (long triangleindex = 0; triangleindex < 2; triangleindex++)
				{
					const D3DXVECTOR3& v1 = pVertex[0];
					const D3DXVECTOR3& v2 = pVertex[1];
					const D3DXVECTOR3& v3 = pVertex[2];
					
					D3DXVECTOR3 cross;
					D3DXVECTOR3 a = pVertex[1] - pVertex[0];
					D3DXVECTOR3 b = pVertex[2] - pVertex[0];
					D3DXVec3Cross( &cross, &a, &b );
					
					if (D3DXVec3Dot(&g_Decal_decalNormal, &cross) > g_Decal_decalEpsilon * D3DXVec3Length( &cross) )
					{
						newVertex[0] = pVertex[0];
						newVertex[1] = pVertex[1];
						newVertex[2] = pVertex[2];
						
						long count = ClipPolygon(3, newVertex, newVertex);
						if ((count != 0) && (!AddPolygon(count, newVertex))) break;			
					}
					
					pVertex+=3;
				}
			}
		}
	}

	_XLODTILE*	pTile = g_LodTerrain.GetTerrainTile( m_decalCenter, &tilecoord );
		
	if( pTile && !pTile->svObbList.empty() )
	{
		_XOBBData* pobbData;
		svdef_OBBData::iterator it;
		for (it = pTile->svObbList.begin(); it != pTile->svObbList.end(); ++ it)
		{
			pobbData = (*it);
			if(pobbData->m_OBBLevel != 0) continue;
			
			LPWORD pIndexBuffer = g_OBBModelVertIndex;
			
			for (long triangleindex = 0; triangleindex < 12; triangleindex++)
			{
				const D3DXVECTOR3& v1 = pobbData->m_ModelVertex[pIndexBuffer[0]];
				const D3DXVECTOR3& v2 = pobbData->m_ModelVertex[pIndexBuffer[1]];
				const D3DXVECTOR3& v3 = pobbData->m_ModelVertex[pIndexBuffer[2]];
				
				D3DXVECTOR3 cross;
				D3DXVECTOR3 a = v2 - v1;
				D3DXVECTOR3 b = v3 - v1;
				D3DXVec3Cross( &cross, &a, &b );
				
				if (D3DXVec3Dot(&g_Decal_decalNormal, &cross) > g_Decal_decalEpsilon * D3DXVec3Length( &cross) )
				{
					newVertex[0] = v1;
					newVertex[1] = v2;
					newVertex[2] = v3;
					
					long count = ClipPolygon(3, newVertex, newVertex);
					if ((count != 0) && (!AddPolygon(count, newVertex))) break;			
				}
				else
				{
					D3DXVec3Cross( &cross, &b, &a );
					
					if (D3DXVec3Dot(&g_Decal_decalNormal, &cross) > g_Decal_decalEpsilon * D3DXVec3Length( &cross) )
					{
						newVertex[0] = v1;
						newVertex[1] = v3;
						newVertex[2] = v2;
						
						long count = ClipPolygon(3, newVertex, newVertex);
						if ((count != 0) && (!AddPolygon(count, newVertex))) break;
					}
				}
				
				pIndexBuffer+=3;
			}
		}
	}
}

bool _XShadowDecalObject::AddPolygon(long vertexCount, const D3DXVECTOR3 *vertex)
{
	long count = m_decalVertexCount;
	if (count + vertexCount >= g_MaxDecalVertices) return (false);
	
	// Add polygon as a triangle fan
	_XTriangle *triangle = g_Decal_TriangleArray + m_decalTriangleCount;
	m_decalTriangleCount += vertexCount - 2;
	for (long a = 2; a < vertexCount; a++)
	{
		triangle->index[0] = (unsigned short) count;
		triangle->index[1] = (unsigned short) (count + a - 1);
		triangle->index[2] = (unsigned short) (count + a);
		triangle++;
	}
	
	// Assign vertex colors
	float f = 1.0F / (1.0F - g_Decal_decalEpsilon);
	for (long b = 0; b < vertexCount; b++)
	{
		g_Decal_VertexArray[count] = vertex[b];
		count++;
	}
	
	m_decalVertexCount = count;
	return (true);
}

long _XShadowDecalObject::ClipPolygon(long vertexCount, const D3DXVECTOR3 *vertex, D3DXVECTOR3 *newVertex) const
{
	D3DXVECTOR3		tempVertex[9];
	D3DXVECTOR3		tempNormal[9];
	
	// Clip against all six planes
	long count = ClipPolygonAgainstPlane(g_Decal_leftPlane, vertexCount, vertex, tempVertex);
	if (count != 0)
	{
		count = ClipPolygonAgainstPlane(g_Decal_rightPlane, count, tempVertex, newVertex);
		if (count != 0)
		{
			count = ClipPolygonAgainstPlane(g_Decal_bottomPlane, count, newVertex, tempVertex);
			if (count != 0)
			{
				count = ClipPolygonAgainstPlane(g_Decal_topPlane, count, tempVertex, newVertex);
				if (count != 0)
				{
					count = ClipPolygonAgainstPlane(g_Decal_backPlane, count, newVertex, tempVertex);
					if (count != 0)
					{
						count = ClipPolygonAgainstPlane(g_Decal_frontPlane, count, tempVertex, newVertex);
					}
				}
			}
		}
	}
	
	return (count);
}

long _XShadowDecalObject::ClipPolygonAgainstPlane(const D3DXVECTOR4& plane, long vertexCount, const D3DXVECTOR3 *vertex, D3DXVECTOR3 *newVertex)
{
	bool	negative[10];
	
	// Classify vertices
	long negativeCount = 0;
	for (long a = 0; a < vertexCount; a++)
	{
		bool neg = (DotProduct(plane, vertex[a]) < 0.0F);
		negative[a] = neg;
		negativeCount += neg;
	}
	
	// Discard this polygon if it's completely culled
	if (negativeCount == vertexCount) return (0);
	
	long count = 0;
	for (long b = 0; b < vertexCount; b++)
	{
		// c is the index of the previous vertex
		long c = (b != 0) ? b - 1 : vertexCount - 1;
		
		if (negative[b])
		{
			if (!negative[c])
			{
				// Current vertex is on negative side of plane,
				// but previous vertex is on positive side.
				const D3DXVECTOR3& v1 = vertex[c];
				const D3DXVECTOR3& v2 = vertex[b];
				float t = DotProduct(plane, v1) / (plane.x * (v1.x - v2.x) + plane.y * (v1.y - v2.y) + plane.z * (v1.z - v2.z));
				newVertex[count] = v1 * (1.0F - t) + v2 * t;
				
				count++;
			}
		}
		else
		{
			if (negative[c])
			{
				// Current vertex is on positive side of plane,
				// but previous vertex is on negative side.
				const D3DXVECTOR3& v1 = vertex[b];
				const D3DXVECTOR3& v2 = vertex[c];
				float t = DotProduct(plane, v1) / (plane.x * (v1.x - v2.x) + plane.y * (v1.y - v2.y) + plane.z * (v1.z - v2.z));
				newVertex[count] = v1 * (1.0F - t) + v2 * t;
				
				count++;
			}
			
			// Include current vertex
			newVertex[count] = vertex[b];
			count++;
		}
	}
	
	// Return number of vertices in clipped polygon
	return (count);
}

void _XShadowDecalObject::Render( _XTextureManager* pTextureManager, int textureindex )
{
	if( !pTextureManager || !m_decalTriangleCount ) return;
	
	gpDev->SetFVF( D3DFVF_XYZ );
	pTextureManager->SetTexture( textureindex );

	gpDev->SetStreamSource( 0, m_pDecalVertices, 0, sizeof(D3DXVECTOR3) );
	gpDev->SetIndices( m_pDecalIndeces );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_decalVertexCount, 0, m_decalTriangleCount );
}

void _XShadowDecalObject::Render( _XRenderStyleManager* pRenderStyleManager, _XTextureManager* pTextureManager, int renderstyleindex )
{
	if( !pRenderStyleManager || !pTextureManager || !m_decalTriangleCount  ) return;
	
	gpDev->SetFVF( D3DFVF_XYZ );
	
	pRenderStyleManager->SetRenderStyle( pTextureManager, renderstyleindex );		
	gpDev->SetStreamSource( 0, m_pDecalVertices, 0, sizeof(D3DXVECTOR3) );
	gpDev->SetIndices( m_pDecalIndeces );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_decalVertexCount, 0, m_decalTriangleCount );
}

void _XShadowDecalObject::Render( void )
{
	if( m_decalVertexCount <= 0 ) return;

	gpDev->SetFVF( D3DFVF_XYZ );
	gpDev->SetStreamSource( 0, m_pDecalVertices, 0, sizeof(D3DXVECTOR3) );
	gpDev->SetIndices( m_pDecalIndeces );
	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_decalVertexCount, 0, m_decalTriangleCount );
}