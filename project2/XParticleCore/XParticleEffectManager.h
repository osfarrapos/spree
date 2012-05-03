// XParticleEffectManager.h: interface for the _XParticleEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPARTICLEEFFECTMANAGER_H__C6397BCC_13AF_4F3C_BBB6_DC327071CB0D__INCLUDED_)
#define AFX_XPARTICLEEFFECTMANAGER_H__C6397BCC_13AF_4F3C_BBB6_DC327071CB0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <stack>
#include "d3dx9.h"
#include "XKernel.h"

class _XParticleEffectManager  
{
public:
	typedef unsigned int HLIGHT;
	#define INVALID_HLIGHT 0xffffffff

	struct EFFECT_INFO{
		HLIGHT		hLight;
		FLOAT		fLifeTime;
	};
	typedef std::vector<EFFECT_INFO> svEffectInfo;

	typedef std::stack<HLIGHT>		 ssHLIGHT;


	#define D3DFVF_SCREENVERTEX  ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )
	typedef struct _SCREENVERTEX
	{ 
		FLOAT x, y, z, rhw; 
		D3DCOLOR color; 

		_SCREENVERTEX() {}
		_SCREENVERTEX( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _rhw, D3DCOLOR _color )
		{
			x = _x; y = _y; z = _z; rhw = _rhw;
			color = _color;
		}	
	    ~_SCREENVERTEX() {}
	} _SCREENVERTEX, *_LPSCREENVERTEX; 

	enum
	{
		STATE_NONE = 0x00,
		STATE_FADEIN,
		STATE_FADEOUT
	};

public:
	ssHLIGHT			m_sshLight;
	svEffectInfo		m_svEffectInfo;

	BYTE				m_byState;
	_SCREENVERTEX		m_ScreenVertex[4];

	DWORD				m_dwFadeColor;
	FLOAT				m_fFadeTime;
	FLOAT				m_fMaxFadeTime;
public:
	_XParticleEffectManager();
	virtual ~_XParticleEffectManager();

	void	CreateLightEffect(D3DXVECTOR3& vPosition, FLOAT fLifeTime, D3DLIGHT9& light);
	
	void	Update(FLOAT fElapsedTime);
	void	Render();

public:
	void	StartScreenEffect(DWORD dwTargetColor, FLOAT fMaxFadeTime = 1.f);
	HLIGHT	PopHandle();
	void	PushHandle(HLIGHT hLight);
};

inline void _XParticleEffectManager::StartScreenEffect(DWORD dwTargetColor, FLOAT fMaxFadeTime)
{
	if (m_byState != STATE_NONE)	return;

	m_dwFadeColor = dwTargetColor & 0x00ffffff;
	m_fMaxFadeTime = fMaxFadeTime;

	m_byState = STATE_FADEIN;
	m_fFadeTime = 0.f;

	D3DCOLOR fadecolor = D3DCOLOR_ARGB(255,0,0,0);
	m_ScreenVertex[0]=_SCREENVERTEX( 0.0f,0.0f,0.1f,						1.f,	m_dwFadeColor );
	m_ScreenVertex[1]=_SCREENVERTEX( (FLOAT)gnWidth,0.0f,0.1f,				1.f,	m_dwFadeColor );
	m_ScreenVertex[2]=_SCREENVERTEX( 0.0f,(FLOAT)gnHeight,0.1f,				1.f,	m_dwFadeColor );
	m_ScreenVertex[3]=_SCREENVERTEX( (FLOAT)gnWidth,(FLOAT)gnHeight,0.1f,	1.f,	m_dwFadeColor );
}

inline _XParticleEffectManager::HLIGHT _XParticleEffectManager::PopHandle()
{
	if (m_sshLight.empty()) return INVALID_HLIGHT;
	HLIGHT hLight = m_sshLight.top();
	m_sshLight.pop();
	return hLight;
}
inline void _XParticleEffectManager::PushHandle(HLIGHT hLight)
{
	m_sshLight.push(hLight);
}


#endif // !defined(AFX_XPARTICLEEFFECTMANAGER_H__C6397BCC_13AF_4F3C_BBB6_DC327071CB0D__INCLUDED_)
