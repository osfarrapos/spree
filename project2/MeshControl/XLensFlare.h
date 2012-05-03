// XLensFlare.h: interface for the _XLensFlare class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XLENSFLARE_H_
#define _XLENSFLARE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_FLARECOUNT	8

#define D3DFVF_XTLVERTEX  ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1 )

typedef struct _XTLVERTEX
{     
	FLOAT x; 
    FLOAT y; 
    FLOAT z; 
	FLOAT rhw;
    
	D3DCOLOR color; 
    D3DCOLOR specular; 

    FLOAT tu; 
    FLOAT tv; 

	_XTLVERTEX(){}
	_XTLVERTEX( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _rhw, D3DCOLOR _color, D3DCOLOR _specular, FLOAT _tu, FLOAT _tv )
	{
		x = _x; y = _y; z = _z; rhw = _rhw;
		color = _color; specular = _specular;
		tu = _tu; tv = _tv;
	}	
	
    ~_XTLVERTEX(){ }

} _XTLVERTEX, *_LPXTLVERTEX; 


typedef struct
{
	_XBillboard *pLensflare;	// the billboard class
	float		 AxisPos;		// CameraPosition on flare Axis
	int			 textureindex;  
} _XLENSFLARE_STRUCTURE;


class _XLensFlare  
{
public:
	_XBillboard				m_SunBillboard;	
	int						m_SunTextureIndex;
	D3DXVECTOR3				m_SunPosition;

	_XBillboard				m_MoonBillboard;
	int						m_MoonTextureIndex;
	D3DXVECTOR3				m_MoonPosition;

	_XLENSFLARE_STRUCTURE	m_flares[_XDEF_FLARECOUNT];
	_XTLVERTEX				m_burnout[4];	

	FLOAT					m_SunZAngle;
	int						m_SunTwinkleRateIndex;

public:
	_XLensFlare();
	virtual ~_XLensFlare();
	
	void Init( void );

	void CreateFlare(int index,float size,float flare_CameraPosition,int tex);
	void DestroyFlare( void );

	void RenderSun( int daynight );	
	void Render( void );

	void RebuildPosition( void );

};

#endif // !defined(_XLENSFLARE_H_)
