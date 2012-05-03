// XParticleRenderManager.h: interface for the _XParticleRenderManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPARTICLERENDERMANAGER_H__0D1578EA_252C_4E7D_8B4B_E10F875D8ED1__INCLUDED_)
#define AFX_XPARTICLERENDERMANAGER_H__0D1578EA_252C_4E7D_8B4B_E10F875D8ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XParticleElement.h"
#include "XMemoryPool.h"

const int PARTICLE_BUFFER_MAX_COUNT		= 100;
const int PARTICLE_POLYGON_MAX_COUNT	= (PARTICLE_BUFFER_MAX_COUNT * 2);
	
const int PARTICLE_MAX_INDEX			= 18;
const int NUMERICPARTICLE_MAX_INDEX		= 30;

class _XParticleRenderManager  
{
public:
	LPDIRECT3DVERTEXBUFFER9 g_lpParticleVertexBuffer;
	LPDIRECT3DINDEXBUFFER9  g_lpParticleIndexBuffer;

	_XPARTICLEVERTEX		g_TriangleParticleVertex[3];

	_XPARTICLEVERTEX		g_ParticleVertex[12];
	WORD					g_ParticleIndex[PARTICLE_MAX_INDEX];

	_XPARTICLEVERTEX		g_NumericParticleVertex[20];
	WORD					g_NumericParticleIndex[NUMERICPARTICLE_MAX_INDEX];

	int						m_nRenderFaceCount;
	int						m_nRenderVertexCount;
	D3DXMATRIX				g_IdentityMatrix;

	_XPARTICLEVERTEX*		m_pVertex;
	WORD*					m_pIndex;

// Particle Element
	XMemoryPool<_XParticleElement>	m_ElementPool;
public:
	_XParticleRenderManager();
	virtual ~_XParticleRenderManager();

	HRESULT	Initialize();
	HRESULT Destroy();

public:
	inline void Lock()
	{
		m_nRenderFaceCount = 0;
		m_nRenderVertexCount = 0;

		if ( FAILED( g_lpParticleVertexBuffer->Lock( 0, 0, (void**)&m_pVertex, 0 ) ) ) 	
		{	
			return ;	
		}

		if ( FAILED( g_lpParticleIndexBuffer->Lock( 0, 0, (void**)&m_pIndex, 0 ) ) )	
		{	
			g_lpParticleVertexBuffer->Unlock();
			return;
		}
	}

	inline void Unlock()
	{
		g_lpParticleIndexBuffer->Unlock();
		g_lpParticleVertexBuffer->Unlock();
	}

	inline void SetTriangleBuffer(DWORD color, D3DXMATRIX& matWorld)
	{
		if ((m_nRenderFaceCount + 1) > PARTICLE_POLYGON_MAX_COUNT)
		{
			Unlock();
			Render();
			Lock();
		}
		
		WORD* pIndex;
		pIndex = &m_pIndex[m_nRenderFaceCount * 3];
		for (int i = 0; i < 3; ++ i)
		{
			pIndex[i] = i + m_nRenderVertexCount;
		}
		
		_XPARTICLEVERTEX* pVertex;
		pVertex = &m_pVertex[m_nRenderVertexCount];
		//memcpy( pVertex, g_ParticleVertex, sizeof(_XPARTICLEVERTEX) * 3 );
		for (i = 0; i < 3; ++ i)
		{
			pVertex[i].tu = g_TriangleParticleVertex[i].tu;
			pVertex[i].tv = g_TriangleParticleVertex[i].tv;
			pVertex[i].col = color;
			D3DXVec3TransformCoord(&pVertex[i].pos, &g_TriangleParticleVertex[i].pos, &matWorld);
		}
		
		m_nRenderVertexCount += 3;
		m_nRenderFaceCount += 1;
	}


	inline void SetBuffer(int nVertex, int nFace, DWORD color, D3DXMATRIX& matWorld)
	{
		if ((m_nRenderFaceCount + nFace) > PARTICLE_POLYGON_MAX_COUNT)
		{
			Unlock();
			Render();
			Lock();
		}

		WORD* pIndex;
		pIndex = &m_pIndex[m_nRenderFaceCount * 3];
		for (int i = 0; i < nFace * 3; ++ i)
		{
			pIndex[i] = g_ParticleIndex[i] + m_nRenderVertexCount;
		}

		_XPARTICLEVERTEX* pVertex;
		pVertex = &m_pVertex[m_nRenderVertexCount];
		//memcpy(pVertex, g_ParticleVertex, sizeof(_XPARTICLEVERTEX) * nVertex);
		for (i = 0; i < nVertex; ++ i)
		{
			pVertex[i].tu = g_ParticleVertex[i].tu;
			pVertex[i].tv = g_ParticleVertex[i].tv;
			pVertex[i].col = color;
			D3DXVec3TransformCoord(&pVertex[i].pos, &g_ParticleVertex[i].pos, &matWorld);
		}

		m_nRenderVertexCount += nVertex;
		m_nRenderFaceCount += nFace;
	}
	inline void SetNumericBuffer(DWORD color, D3DXMATRIX& matWorld)
	{
		_XPARTICLEVERTEX* pVertex;
		pVertex = &m_pVertex[m_nRenderVertexCount];
		//memcpy(pVertex, g_NumericParticleVertex, sizeof(_XPARTICLEVERTEX) * 16);
		for (int i = 0; i < 20; ++ i)
		{
			pVertex[i].tu = g_NumericParticleVertex[i].tu;
			pVertex[i].tv = g_NumericParticleVertex[i].tv;
			pVertex[i].col = color;
			D3DXVec3TransformCoord(&pVertex[i].pos, &g_NumericParticleVertex[i].pos, &matWorld);
		}

		WORD* pIndex;
		pIndex = &m_pIndex[m_nRenderFaceCount * 3];
		for (i = 0; i < NUMERICPARTICLE_MAX_INDEX; ++ i)
		{
			pIndex[i] = g_NumericParticleIndex[i] + m_nRenderVertexCount;
		}
		m_nRenderVertexCount += 20;
		m_nRenderFaceCount += 10;
	}

	inline void Render()
	{
		if (m_nRenderFaceCount <= 0) return;

		gpDev->SetIndices( g_lpParticleIndexBuffer );
		gpDev->SetStreamSource( 0, g_lpParticleVertexBuffer, 0, sizeof( _XPARTICLEVERTEX ) );
		gpDev->SetTransform( D3DTS_WORLD, &g_IdentityMatrix );		
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_nRenderVertexCount, 0, m_nRenderFaceCount );
	}

	inline void BeginNumeric()
	{
		gpDev->SetIndices( g_lpParticleIndexBuffer );
		gpDev->SetStreamSource( 0, g_lpParticleVertexBuffer, 0, sizeof( _XPARTICLEVERTEX ) );
		gpDev->SetTransform( D3DTS_WORLD, &g_IdentityMatrix );		
	}
	inline void RenderNumeric(int StartIndex)
	{
		//gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 20, 0, 10 );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, g_NumericParticleIndex[StartIndex], 4, StartIndex, 2 );
	//	gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, StartIndex, 2 );		
	}

	inline _XParticleElement* NewElement()
	{
		return m_ElementPool.New();
	}
	inline void DeleteElement(_XParticleElement* pElement)
	{
		m_ElementPool.Delete(pElement);
	}
};


#endif // !defined(AFX_XPARTICLERENDERMANAGER_H__0D1578EA_252C_4E7D_8B4B_E10F875D8ED1__INCLUDED_)
