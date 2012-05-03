// XPostProcessCore.h: interface for the _XPostProcessCore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPOSTPROCESSCORE_H__E8C8DE49_4AF5_475E_AC67_277A67066FA7__INCLUDED_)
#define AFX_XPOSTPROCESSCORE_H__E8C8DE49_4AF5_475E_AC67_277A67066FA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

struct _XPP_SCREENVERTEX
{
	enum { FVF=D3DFVF_XYZRHW|D3DFVF_TEX1 };
	float px, py, pz, pw;
	float tx, ty;
	
	_XPP_SCREENVERTEX(){}
	_XPP_SCREENVERTEX( float _px, float _py, float _pz, float _pw, float _tx, float _ty )
	{
		px = _px;
		py = _py;
		pz = _pz;
		pw = _pw;
		tx = _tx;
		ty = _ty;
	}
};

typedef enum _XBLOOMFILTER_SHADER
{
	_XBLOOMFILTER_SHADER_GAUSSIANBLUR_X = 0,
	_XBLOOMFILTER_SHADER_GAUSSIANBLUR_Y,
	_XBLOOMFILTER_SHADER_GAUSSIANBLUR_POWER,
	_XBLOOMFILTER_SHADER_GAUSSIANBLUR_FINAL,
};

class _XPostProcessCore  
{
public:
	_XPostProcessCore();
	virtual ~_XPostProcessCore();

	BOOL							Init( void );
	
	void							InitDeviceObjects( void );
	void							InvalidateDeviceObjects( void );
	void							DeleteDeviceObjects( void );
	void							RestoreDeviceObjects( void );
		
	void							BeginPostProcess(void);
	void							EndPostProcess(void);

    void							EnableBloomFilter( BOOL enable );
	void							InitializeBloomFilter( void );
	void							ReleaseBloomFilter( void );
	void							GaussianBlur( void );

public:
	//프론트버퍼 백업용 포인터
	LPDIRECT3DSURFACE9				m_pBackDepthSurface;
	LPDIRECT3DSURFACE9				m_pBackTargetSurface;

	LPDIRECT3DTEXTURE9				m_pOrgSceneTexture;
	LPDIRECT3DSURFACE9				m_pOrgSceneSurface;
	LPDIRECT3DSURFACE9				m_pOrgSceneDepthSurface;

	_XPP_SCREENVERTEX				m_ScreenVertex[4];
	_XPP_SCREENVERTEX				m_BlurScreenVertex[4];


	// Bloom filter section
	BOOL							m_UseBloomFilter;
	LPDIRECT3DTEXTURE9				m_pTexTarget1;
	LPDIRECT3DSURFACE9				m_pSurTarget1;
	LPDIRECT3DTEXTURE9				m_pTexTarget2;
	LPDIRECT3DSURFACE9				m_pSurTarget2;

	FLOAT							m_FinalPixelShaderConst1[4];
	LPDIRECT3DPIXELSHADER9			m_pPixelShader_Bloomfilter[4];
			
	int								m_GaussianFactor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Radial blur...

struct _XScreenBlurOverayVertex
{
	FLOAT		x, y, z;	
	FLOAT       rhw;
    DWORD       color;
	FLOAT		tu, tv;

	_XScreenBlurOverayVertex(){}
	_XScreenBlurOverayVertex( float _x, float _y, float _z, float _rhw, D3DCOLOR _color, float _tu, float _tv )
	{
		x		= _x;
		y		= _y;
		z		= _z;
		rhw		= _rhw;
		color	= _color;
		tu		= _tu;
		tv		= _tv;
	}
};

#define _XFVF_OVERLAYVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

typedef	void (__stdcall *_XRadialBlurRenderCallback)( void );

class _XScreenBlurCore
{
public :
	_XScreenBlurCore();
	virtual ~_XScreenBlurCore();

	// Common methods
	virtual BOOL				Init( void );	
	virtual void				InitDeviceObjects( void );
	virtual void				InvalidateDeviceObjects( void );
	virtual void				DeleteDeviceObjects( void );
	virtual void				RestoreDeviceObjects( void );
	
	// Radial blur methods
	virtual void				EnableRadialBlur( BOOL enable )
	{
								m_bEnableRadialBlur = enable;
	}
	virtual void				ProcessRadialBlur( void );
	
	virtual void				RestoreBlurOption( void );
	virtual void				SetBlurStep( int step = 30 )
	{
								m_iNumRadialBlurSteps = step;
								m_fAlphaDelta			= 0.1f / m_iNumRadialBlurSteps;
	}
	virtual void				SetUVDelta( float uvdelta = 0.016f )
	{
								m_fUVDelta = uvdelta;
	}
	virtual void				SetBlendColor( D3DCOLOR blendcolor = 0xFFFFFFFF )
	{
								m_BlendColor = blendcolor;
								m_OverayScreenVertex[0].color = m_OverayScreenVertex[1].color = 
								m_OverayScreenVertex[2].color = m_OverayScreenVertex[3].color = m_BlendColor;
	}

	// Motion blur methods
	virtual void				ProcessMotionBlur( void );
	virtual void				SetMotionBlurIntensity( FLOAT fintensity )
	{
								m_fIntensity = fintensity;
	}
	
public	:

	// common data
	static int					m_iBlurTextureResolution;
	
	// Radial blur	
	BOOL						m_bEnableRadialBlur;

	int							m_iNumRadialBlurSteps;
	FLOAT						m_fUVDelta;
	FLOAT						m_fAlphaDelta;
	D3DCOLOR					m_BlendColor;	

	// Motion blur
	BOOL						m_bEnableMotionBlur;
	float						m_fIntensity;

	// device object
	LPDIRECT3DSURFACE9			m_pOldRenderSurface;	
	LPDIRECT3DSURFACE9			m_pDepthStencilSurface;

	LPDIRECT3DTEXTURE9			m_pSceneTexture;	
	LPDIRECT3DSURFACE9			m_pSceneRenderSurface;

	LPDIRECT3DTEXTURE9			m_pRadialBlurTexture;
	LPDIRECT3DSURFACE9			m_pBlurRenderSurface;

	LPDIRECT3DTEXTURE9			m_pAccumulateTexture;
	LPDIRECT3DSURFACE9			m_pAccumulateSurface;

	_XScreenBlurOverayVertex	m_BlurScreenVertex[4];
	_XScreenBlurOverayVertex	m_OverayScreenVertex[4];
	_XScreenBlurOverayVertex	m_MotionBlurScreenVertex[4];

	// Rendering callback
	_XRadialBlurRenderCallback	m_pRadialBlurRenderingCallback;
	_XRadialBlurRenderCallback	m_pMotionBlurRenderingCallback;
};

#endif // !defined(AFX_XPOSTPROCESSCORE_H__E8C8DE49_4AF5_475E_AC67_277A67066FA7__INCLUDED_)
