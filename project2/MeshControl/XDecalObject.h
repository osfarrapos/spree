// XDecalObject.h: interface for the _XDecalObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XDECALOBJECT_H_
#define _XDECALOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

struct _XDECALVERTEXTYPE
{
    D3DXVECTOR3 p;    
    D3DCOLOR    color;
	FLOAT       tu, tv;
};

#define _XFVF_DECALVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct _XTriangle
{
	WORD index[3];
};

const long	g_MaxDecalVertices = 4096;

//////////////////////////////////////////////////////////////////////////
// Particle decal
class _XDecalObject  
{
public: 
	
	D3DXVECTOR3				m_decalCenter;	
	
	long					m_decalVertexCount;
	long					m_createddecalVertexCount;
	LPDIRECT3DVERTEXBUFFER9 m_pDecalVertices;
	
	long					m_decalTriangleCount;
	long					m_createdTriangleCount;
	LPDIRECT3DINDEXBUFFER9	m_pDecalIndeces;	

	FLOAT					m_decalWidth;
	FLOAT					m_decalHeight;

public:

	_XDecalObject();
	virtual ~_XDecalObject();
	
	void					Release( void );

	// Terrain decal
	void					InvalidateDeviceObject( void );
	void					RestoreDeviceObject( void );
	BOOL					GenerateDecal(const D3DXVECTOR3& center, const D3DXVECTOR3& normal, const D3DXVECTOR3& tangent, 
										  float width, float height, float depth );

	void					Render( void );
	void					Render( _XTextureManager* pTextureManager, int textureindex );
	void					Render( _XRenderStyleManager* pRenderStyleManager, _XTextureManager* pTextureManager, int renderstyleindex );

private:

	void					ClipMesh( BOOL computeterrainvertices );
	bool					AddPolygon(long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal);
	long					ClipPolygon(long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal, D3DXVECTOR3 *newVertex, D3DXVECTOR3 *newNormal) const;
	static long				ClipPolygonAgainstPlane(const D3DXVECTOR4& plane, long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal, D3DXVECTOR3 *newVertex, D3DXVECTOR3 *newNormal);
};

//////////////////////////////////////////////////////////////////////////
// Shadow decal
class _XShadowDecalObject  
{
public: 
	
	D3DXVECTOR3				m_decalCenter;	
	
	long					m_decalVertexCount;
	long					m_createddecalVertexCount;
	LPDIRECT3DVERTEXBUFFER9 m_pDecalVertices;
	
	long					m_decalTriangleCount;
	long					m_createdTriangleCount;
	LPDIRECT3DINDEXBUFFER9	m_pDecalIndeces;	
	
	FLOAT					m_decalWidth;
	FLOAT					m_decalHeight;
	
public:
	
	_XShadowDecalObject();
	virtual ~_XShadowDecalObject();
	
	// Terrain decal
	void					InvalidateDeviceObject( void );
	void					RestoreDeviceObject( void );
	BOOL					GenerateDecal(const D3DXVECTOR3& center, float width, float height, float depth );
	
	void					Render( void );
	void					Render( _XTextureManager* pTextureManager, int textureindex );
	void					Render( _XRenderStyleManager* pRenderStyleManager, _XTextureManager* pTextureManager, int renderstyleindex );
	
private:

	void					ClipMesh( BOOL computeterrainvertices );
	bool					AddPolygon(long vertexCount, const D3DXVECTOR3 *vertex);
	long					ClipPolygon(long vertexCount, const D3DXVECTOR3 *vertex, D3DXVECTOR3 *newVertex) const;
	static long				ClipPolygonAgainstPlane(const D3DXVECTOR4& plane, long vertexCount, const D3DXVECTOR3 *vertex, D3DXVECTOR3 *newVertex );
};

#endif // !defined(_XDECALOBJECT_H_)
