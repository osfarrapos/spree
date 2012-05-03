// XParticleRenderManager.cpp: implementation of the _XParticleRenderManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XParticleRenderManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XParticleRenderManager::_XParticleRenderManager()
{

}

_XParticleRenderManager::~_XParticleRenderManager()
{
	
}

HRESULT _XParticleRenderManager::Initialize()
{
	Destroy();

	g_IdentityMatrix = D3DXMATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	// dynamic 으로 생성해서 lock 시 D3DLOCK_NOOVERWRITE 으로 주면 렌더링 속도를 높일수 있으나 
	// memory pool 옵션을 default 로 해야 하기 때문에 device Restore 시 재 생성을 해야 한다.
	// 파티클 최대 갯수가 얼마 되지 않기 때문에 그냥 찍는거로 한다.
	HRESULT hr = gpDev->CreateVertexBuffer( sizeof(_XPARTICLEVERTEX) * PARTICLE_BUFFER_MAX_COUNT * 4,  D3DUSAGE_WRITEONLY, _XDEF_FVF_XPARTICLEVERTEX, D3DPOOL_MANAGED, &g_lpParticleVertexBuffer, NULL );
	if ( FAILED( hr ) )
	{
		_XFatalError( "Can't create particle billboard vertex list" );
		return hr;
	}
	hr = gpDev->CreateIndexBuffer( sizeof(WORD) * PARTICLE_BUFFER_MAX_COUNT * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_lpParticleIndexBuffer, NULL );
    if ( FAILED( hr ) )
	{
		_XFatalError( "Can't create particle billboard index list" );
		return hr;
	}
		
	g_TriangleParticleVertex[0].pos.x =  0.0f*0.7f;			g_TriangleParticleVertex[0].pos.y =  1.0f*0.7f;	g_TriangleParticleVertex[0].pos.z = 0.0f;	g_TriangleParticleVertex[0].tu = 0.5f;		g_TriangleParticleVertex[0].tv = -0.167f;	
	g_TriangleParticleVertex[1].pos.x =  0.866025f*0.7f;	g_TriangleParticleVertex[1].pos.y = -0.5f*0.7f;	g_TriangleParticleVertex[1].pos.z = 0.0f;	g_TriangleParticleVertex[1].tu = 1.0835f;	g_TriangleParticleVertex[1].tv = 1.0f-0.167f;
	g_TriangleParticleVertex[2].pos.x =  -0.866025*0.7f;	g_TriangleParticleVertex[2].pos.y = -0.5f*0.7f;	g_TriangleParticleVertex[2].pos.z = 0.0f;	g_TriangleParticleVertex[2].tu = -0.0835f;	g_TriangleParticleVertex[2].tv = 1.0f-0.167f;

	g_ParticleVertex[0].pos.x = -0.5f;	g_ParticleVertex[0].pos.y = -0.5f;	g_ParticleVertex[0].pos.z = 0.0f;	g_ParticleVertex[0].tu = 0.0f;	g_ParticleVertex[0].tv = 1.0f;
	g_ParticleVertex[1].pos.x = -0.5f;	g_ParticleVertex[1].pos.y =  0.5f;	g_ParticleVertex[1].pos.z = 0.0f;	g_ParticleVertex[1].tu = 0.0f;	g_ParticleVertex[1].tv = 0.0f;	
	g_ParticleVertex[2].pos.x =  0.5f;	g_ParticleVertex[2].pos.y = -0.5f;	g_ParticleVertex[2].pos.z = 0.0f;	g_ParticleVertex[2].tu = 1.0f;	g_ParticleVertex[2].tv = 1.0f;	
	g_ParticleVertex[3].pos.x =  0.5f;	g_ParticleVertex[3].pos.y =  0.5f;	g_ParticleVertex[3].pos.z = 0.0f;	g_ParticleVertex[3].tu = 1.0f;	g_ParticleVertex[3].tv = 0.0f;	
	g_ParticleVertex[4].pos.x = 0.0f;	g_ParticleVertex[4].pos.y = -0.5f;	g_ParticleVertex[4].pos.z = -0.5f;	g_ParticleVertex[4].tu = 0.0f;	g_ParticleVertex[4].tv = 1.0f;
	g_ParticleVertex[5].pos.x = 0.0f;	g_ParticleVertex[5].pos.y =  0.5f;	g_ParticleVertex[5].pos.z = -0.5f;	g_ParticleVertex[5].tu = 0.0f;	g_ParticleVertex[5].tv = 0.0f;
	g_ParticleVertex[6].pos.x = 0.0f;	g_ParticleVertex[6].pos.y = -0.5f;	g_ParticleVertex[6].pos.z =  0.5f;	g_ParticleVertex[6].tu = 1.0f;	g_ParticleVertex[6].tv = 1.0f;
	g_ParticleVertex[7].pos.x = 0.0f;	g_ParticleVertex[7].pos.y =  0.5f;	g_ParticleVertex[7].pos.z =  0.5f;	g_ParticleVertex[7].tu = 1.0f;	g_ParticleVertex[7].tv = 0.0f;
	g_ParticleVertex[8].pos.x = -0.5f;	g_ParticleVertex[8].pos.y = 0.0f;	g_ParticleVertex[8].pos.z = -0.5f;	g_ParticleVertex[8].tu = 0.0f;	g_ParticleVertex[8].tv = 1.0f;
	g_ParticleVertex[9].pos.x = -0.5f;	g_ParticleVertex[9].pos.y = 0.0f;	g_ParticleVertex[9].pos.z =  0.5f;	g_ParticleVertex[9].tu = 0.0f;	g_ParticleVertex[9].tv = 0.0f;
	g_ParticleVertex[10].pos.x =  0.5f;	g_ParticleVertex[10].pos.y = 0.0f;	g_ParticleVertex[10].pos.z = -0.5f;	g_ParticleVertex[10].tu = 1.0f;	g_ParticleVertex[10].tv = 1.0f;
	g_ParticleVertex[11].pos.x =  0.5f;	g_ParticleVertex[11].pos.y = 0.0f;	g_ParticleVertex[11].pos.z =  0.5f;	g_ParticleVertex[11].tu = 1.0f;	g_ParticleVertex[11].tv = 0.0f;

	g_ParticleIndex[ 0] =  0;	g_ParticleIndex[ 1] =  1;	g_ParticleIndex[ 2] =  3;
	g_ParticleIndex[ 3] =  0;	g_ParticleIndex[ 4] =  3;	g_ParticleIndex[ 5] =  2;
	g_ParticleIndex[ 6] =  4;	g_ParticleIndex[ 7] =  5;	g_ParticleIndex[ 8] =  7;
	g_ParticleIndex[ 9] =  4;	g_ParticleIndex[10] =  7;	g_ParticleIndex[11] =  6;
	g_ParticleIndex[12] =  8;	g_ParticleIndex[13] =  9;	g_ParticleIndex[14] = 11;
	g_ParticleIndex[15] =  8;	g_ParticleIndex[16] = 11;	g_ParticleIndex[17] = 10;


	for(int i = 0; i < 5; i++ )
	{
		g_NumericParticleVertex[0 + (i*4)].pos.x = -2.5f + (FLOAT)i;	g_NumericParticleVertex[0 + (i*4)].pos.y = -0.5f;	g_NumericParticleVertex[0 + (i*4)].pos.z =  0.0f;
		g_NumericParticleVertex[1 + (i*4)].pos.x = -2.5f + (FLOAT)i;	g_NumericParticleVertex[1 + (i*4)].pos.y =  0.5f;	g_NumericParticleVertex[1 + (i*4)].pos.z =  0.0f;
		g_NumericParticleVertex[2 + (i*4)].pos.x = -1.5f + (FLOAT)i;	g_NumericParticleVertex[2 + (i*4)].pos.y = -0.5f;	g_NumericParticleVertex[2 + (i*4)].pos.z =  0.0f;
		g_NumericParticleVertex[3 + (i*4)].pos.x = -1.5f + (FLOAT)i;	g_NumericParticleVertex[3 + (i*4)].pos.y =  0.5f;	g_NumericParticleVertex[3 + (i*4)].pos.z =  0.0f;

		g_NumericParticleVertex[0 + (i*4)].tu = 0.0f;		g_NumericParticleVertex[0 + (i*4)].tv = 1.0f;
		g_NumericParticleVertex[1 + (i*4)].tu = 0.0f;		g_NumericParticleVertex[1 + (i*4)].tv = 0.0f;
		g_NumericParticleVertex[2 + (i*4)].tu = 1.0f;		g_NumericParticleVertex[2 + (i*4)].tv = 1.0f;
		g_NumericParticleVertex[3 + (i*4)].tu = 1.0f;		g_NumericParticleVertex[3 + (i*4)].tv = 0.0f;		

	}

	g_NumericParticleIndex[ 0] =  0;	g_NumericParticleIndex[ 1] =  1;	g_NumericParticleIndex[ 2] =  3;
	g_NumericParticleIndex[ 3] =  0;	g_NumericParticleIndex[ 4] =  3;	g_NumericParticleIndex[ 5] =  2;
	g_NumericParticleIndex[ 6] =  4;	g_NumericParticleIndex[ 7] =  5;	g_NumericParticleIndex[ 8] =  7;
	g_NumericParticleIndex[ 9] =  4;	g_NumericParticleIndex[10] =  7;	g_NumericParticleIndex[11] =  6;
	g_NumericParticleIndex[12] =  8;	g_NumericParticleIndex[13] =  9;	g_NumericParticleIndex[14] = 11;
	g_NumericParticleIndex[15] =  8;	g_NumericParticleIndex[16] = 11;	g_NumericParticleIndex[17] = 10;
	g_NumericParticleIndex[18] = 12;	g_NumericParticleIndex[19] = 13;	g_NumericParticleIndex[20] = 15;
	g_NumericParticleIndex[21] = 12;	g_NumericParticleIndex[22] = 15;	g_NumericParticleIndex[23] = 14;
	g_NumericParticleIndex[24] = 16;	g_NumericParticleIndex[25] = 17;	g_NumericParticleIndex[26] = 19;
	g_NumericParticleIndex[27] = 16;	g_NumericParticleIndex[28] = 19;	g_NumericParticleIndex[29] = 18;

	m_ElementPool.Reserve(500);
	return hr;
}

HRESULT _XParticleRenderManager::Destroy()
{
	SAFE_RELEASE( g_lpParticleVertexBuffer );
	SAFE_RELEASE( g_lpParticleIndexBuffer );

	m_ElementPool.DeleteData();
	return S_OK;
}
