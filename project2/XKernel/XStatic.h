// XStatic.h: interface for the _XStatic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSTATIC_H_
#define _XSTATIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTextureManager.h"
#include "XGUIControlCenter.h"

struct INTERFACETVERTEX
{
    FLOAT			x, y, z, rhw;      // The untransformed, 3D position for the vertex
	D3DCOLOR		diffuse;
	FLOAT			tu, tv;			 // Vertex texture coordinates	
};

#define D3DFVF_INTERFACETVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	

#define _XDEF_SYSTEMVERTEX	

#define _XDEF_UVFLIPMODE_LR		0
#define _XDEF_UVFLIPMODE_UD		1
#define _XDEF_UVFLIPMODE_LRUD	2

class _XStatic 
#ifndef _XDEF_SYSTEMVERTEX
	: public _X3DObject
#endif

{
public:

#ifdef _XDEF_SYSTEMVERTEX	
	INTERFACETVERTEX	pVertices[4];
	WORD				pIndices[6];
	D3DCOLOR			m_FaceColor;
	DWORD				m_dwNumVertices;
	DWORD				m_dwNumIndices;
#endif


public:
	_XStatic();
	virtual ~_XStatic();
	
	HRESULT Create( int left, int top, int right, int bottom, 
		            FLOAT depth = _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR facecolor  = 0xFFFFFFFF );
	HRESULT Create( D3DXVECTOR3 lefttop, D3DXVECTOR3 righttop, D3DXVECTOR3 leftbottom, D3DXVECTOR3 rightbottom, 
		            FLOAT depth = _XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR facecolor  = 0xFFFFFFFF );

	HRESULT CreateVertexBuffer( UINT VertexCount, DWORD Usage, DWORD FVF, D3DPOOL Pool );
	void	ReleaseObject( void );

	HRESULT LockVertexBuffer( DWORD Length, INTERFACETVERTEX*& pVertices );

	void	Render( void );
	void	Render( LPDIRECT3DTEXTURE9	pTexture );
	void	RenderAlphaBlend( void );
	void	RenderAlphaBlend( DWORD Alphalevel );
	void	RenderSolid( void );
	void	RenderClampMode( void );
	void	RenderClampMode( LPDIRECT3DTEXTURE9 pTexture );

	void    SetTexture( _XTextureManager* ptexturearchive, int textureindex1, int textureindex2 = -1 );
	void    SetFaceColor( D3DCOLOR facecolor );

	void	SetMaxRate( FLOAT maxrate ){  m_MaxRate = maxrate; }
	void	SetRate( FLOAT x, FLOAT y, BOOL contollupvertex );
	void	SetPercentage( DWORD percent, FLOAT x, FLOAT y,BOOL contollupvertex );
	DWORD	GetPercentage( void ){ return m_Percentage; }

	void	SetXRate( FLOAT x, BOOL contollupvertex );
	void	SetXPercentage( DWORD percent, FLOAT x,BOOL contollupvertex );	

	void	SetYRate( FLOAT y, BOOL contollupvertex );
	void	SetYPercentage( DWORD percent, FLOAT y,BOOL contollupvertex );	

	void	Rotate( FLOAT angle );
	void	InverseUV( int mode = _XDEF_UVFLIPMODE_UD );
	void	ChangeUV( FLOAT u0,FLOAT u1,FLOAT u2,FLOAT u3, FLOAT v0,FLOAT v1,FLOAT v2,FLOAT v3 );
	void	ChangeUV( FLOAT centerx,FLOAT centery, FLOAT size );

	void	MoveWindow( int X, int Y );	
	void	MoveWindow( int left, int top, int right, int bottom );
	void    MoveWindow( D3DXVECTOR3 topleft, D3DXVECTOR3 topright, D3DXVECTOR3 bottomleft, D3DXVECTOR3 bottomright );

	BOOL	TestAlphaChannel( int x, int y );
	BOOL	CheckMouseRayCollision( void );
	
	void	DrawToolTip(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

public:

	FLOAT m_fLeft;
	FLOAT m_fTop;
	FLOAT m_fRight;
	FLOAT m_fBottom;
	FLOAT m_fDepth;

	FLOAT m_MaxRate;
	DWORD m_Percentage;

	_XGUITooltipCallback	m_TooltipCallbackFunc;

	void SetCallBackFunction_ToolTip( _XGUITooltipCallback pcallbackfunction )
	{
					m_TooltipCallbackFunc = pcallbackfunction;
	}

public:
	
	int   m_TextureIndex1;
	int   m_TextureIndex2;

	_XTextureManager*	m_pTextureArchive;

};

#endif // !defined(_XSTATIC_H_)
