// XScreenMotionBlur.h: interface for the _XScreenMotionBlur class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_XSCREENMOTIONBLUR_H_)
#define _XSCREENMOTIONBLUR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

struct _XScreenBlurVertex
{
	D3DXVECTOR3 pos;
	FLOAT       rhw;
    DWORD       color;
	FLOAT		tu, tv;
	
	static const DWORD FVF;
	
	_XScreenBlurVertex(D3DXVECTOR3 p, float w,DWORD c,float u,float v):pos(p),rhw(w),color(c),tu(u),tv(v) {}
	~_XScreenBlurVertex(){};
};

class _XScreenMotionBlur  
{
public:
	bool				m_enable;
	bool				m_firstEnable;
	bool				m_Initialized;
	
	float				m_blurAlpha;
	float				m_maxAlpha;
	float				m_minAlpha;

	//블러 텍스쳐
	LPDIRECT3DSURFACE9	m_pBlurDepthSurface;
	LPDIRECT3DSURFACE9	m_pBlurTargetSurface;
	LPDIRECT3DTEXTURE9	m_pBlurTexture;
	
	//누적 블러 텍스쳐
	LPDIRECT3DSURFACE9	m_pBlurAccumSurface;
	LPDIRECT3DTEXTURE9	m_pBlurAccumTexture;
	
	//프론트버퍼 백업용 포인터
	LPDIRECT3DSURFACE9	m_pBackDepthSurface;
	LPDIRECT3DSURFACE9	m_pBackTargetSurface;
	
	int					m_RenderBufferWidth;
	int					m_RenderBufferHeight;

public:
	_XScreenMotionBlur();
	virtual ~_XScreenMotionBlur();

	bool				InitBlur(void);
	
	void				ApplyAccumBlur(void);
	void				UpdateAccumBlur(void);
	void				ApplyBlur(void);
	
	bool				IsFirstEnable();		

	void				BeginBlur(void);		//위치는 비긴씬 바로 다음이다.
	void				EndBlur(void);			//위치는 블러 하지 않을 것들 이전이다.(예:UI렌더링 이전)
	
	//D3DApplication 관련 함수 
	void				InitDeviceObjects(void);
	void				InvalidateDeviceObjects(void);
	void				DeleteDeviceObjects(void);
	void				RestoreDeviceObjects(void);
	
	bool				IsInit(void)			{ return m_Initialized; }
	bool				IsEnable(void)			{ return m_enable; }
	void				Enable(bool flg);
	
	//블러 알파 값 조작
	void				SetBlurAlpha(float alpha);
	float				GetBlurAlpha(void); 
	
	void				SetMaxBlurAlpha(float alpha);
	void				SetMinBlurAlpha(float alpha);
	
	float				GetMaxBlurAlpha(void)	{ return m_maxAlpha; }
	float				GetMinBlurAlpha(void)	{ return m_minAlpha; }
	
	void				AddBlurAlpha(float alpha);

};


struct _XBF_SCREENVERTEX
{
	enum { FVF=D3DFVF_XYZRHW|D3DFVF_TEX1 };
	float px, py, pz, pw;
	float tx, ty;

	_XBF_SCREENVERTEX(){}
	_XBF_SCREENVERTEX( float _px, float _py, float _pz, float _pw, float _tx, float _ty )
	{
		px = _px;
		py = _py;
		pz = _pz;
		pw = _pw;
		tx = _tx;
		ty = _ty;
	}
};

class _XBroomFilterEffect
{
public:
	_XBroomFilterEffect();
	virtual ~_XBroomFilterEffect();

	BOOL				Init( void );

	void				InitDeviceObjects( void );
	void				InvalidateDeviceObjects( void );
	void				DeleteDeviceObjects( void );
	void				RestoreDeviceObjects( void );
	void				GaussianBlur( void );
	void				DrawFinalScene( void );

	void				BeginBloomFilter(void);
	void				EndBloomFilter(void);

public:

	//프론트버퍼 백업용 포인터
	LPDIRECT3DSURFACE9				m_pBackDepthSurface;
	LPDIRECT3DSURFACE9				m_pBackTargetSurface;
		
	LPDIRECT3DTEXTURE9				m_pOrgSceneTexture;
	LPDIRECT3DSURFACE9				m_pOrgSceneSurface;
	LPDIRECT3DSURFACE9				m_pOrgSceneDepthSurface;
	
	LPDIRECT3DTEXTURE9				m_pTexTarget1;
	LPDIRECT3DSURFACE9				m_pSurTarget1;
	LPDIRECT3DTEXTURE9				m_pTexTarget2;
	LPDIRECT3DSURFACE9				m_pSurTarget2;
	

	LPDIRECT3DPIXELSHADER9			m_pPSGBlurX;	
	LPDIRECT3DPIXELSHADER9			m_pPSGBlurY;	
	LPDIRECT3DPIXELSHADER9			m_pPSPower;	
	LPDIRECT3DPIXELSHADER9			m_pPSFinal;	
	
	int								m_GaussianFactor;

	_XBF_SCREENVERTEX				m_ScreenVertex[4];
	_XBF_SCREENVERTEX				m_BlurScreenVertex[4];
};

#endif // !defined(_XSCREENMOTIONBLUR_H_)
