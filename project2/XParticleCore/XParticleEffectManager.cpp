// XParticleEffectManager.cpp: implementation of the _XParticleEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XParticleEffectManager.h"

#include "XKernel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XParticleEffectManager::_XParticleEffectManager()
{
	// Create screen burn out billboard================================================================================
	D3DCOLOR fadecolor = D3DCOLOR_ARGB(255,0,0,0);
	m_ScreenVertex[0]=_SCREENVERTEX( 0.0f,0.0f,0.0f,						1.f,	fadecolor );
	m_ScreenVertex[1]=_SCREENVERTEX( (FLOAT)gnWidth,0.0f,0.0f,				1.f,	fadecolor );
	m_ScreenVertex[2]=_SCREENVERTEX( 0.0f,(FLOAT)gnHeight,0.0f,				1.f,	fadecolor );
	m_ScreenVertex[3]=_SCREENVERTEX( (FLOAT)gnWidth,(FLOAT)gnHeight,0.0f,	1.f,	fadecolor );

	PushHandle(3);
	PushHandle(4);
	PushHandle(5);

	m_byState = STATE_NONE;

	m_dwFadeColor = 0x00000000;
	m_fFadeTime = 0.f;
}

_XParticleEffectManager::~_XParticleEffectManager()
{
	m_svEffectInfo.clear();
}

void _XParticleEffectManager::CreateLightEffect(D3DXVECTOR3& vPosition, FLOAT fLifeTime, D3DLIGHT9& light)
{
	HLIGHT hLight = PopHandle();
	if (hLight == INVALID_HLIGHT) return;

	EFFECT_INFO info;
	info.hLight = hLight;
	info.fLifeTime = fLifeTime;

	light.Position.x = vPosition.x;
	light.Position.y = vPosition.y;
	light.Position.z = vPosition.z;

	gpDev->SetLight(info.hLight, &light);
	gpDev->LightEnable(info.hLight, TRUE);

	m_svEffectInfo.push_back(info);
}

void _XParticleEffectManager::Update(FLOAT fElapsedTime)
{
	svEffectInfo::iterator it = m_svEffectInfo.begin();
	while (it != m_svEffectInfo.end())
	{
		EFFECT_INFO& info = (*it);
		info.fLifeTime -= fElapsedTime;
		if (info.fLifeTime <= 0.f)
		{
			PushHandle(info.hLight);
			gpDev->LightEnable(info.hLight, FALSE);
			it = m_svEffectInfo.erase(it);
		}
		else
		{
			++ it;
		}
	}

	switch(m_byState)
	{
	case STATE_FADEIN:
		m_fFadeTime += fElapsedTime;
		BYTE alpha;
		if (m_fFadeTime >= m_fMaxFadeTime)
		{
			m_fFadeTime = m_fMaxFadeTime;
			m_byState	= STATE_FADEOUT;
		}
		alpha = 255 * (m_fFadeTime / m_fMaxFadeTime);
		m_dwFadeColor = (m_dwFadeColor & 0x00ffffff) | (alpha << 24);

		m_ScreenVertex[0].color = m_dwFadeColor;
		m_ScreenVertex[1].color = m_dwFadeColor;
		m_ScreenVertex[2].color = m_dwFadeColor;
		m_ScreenVertex[3].color = m_dwFadeColor;

		break;
	case STATE_FADEOUT:
		m_fFadeTime -= fElapsedTime;
		if (m_fFadeTime < 0)
		{
			m_fFadeTime = 0.f;
			m_byState	= STATE_NONE;
		}
		alpha = 255 * (m_fFadeTime / m_fMaxFadeTime);
		m_dwFadeColor = (m_dwFadeColor & 0x00ffffff) | (alpha << 24);

		m_ScreenVertex[0].color = m_dwFadeColor;
		m_ScreenVertex[1].color = m_dwFadeColor;
		m_ScreenVertex[2].color = m_dwFadeColor;
		m_ScreenVertex[3].color = m_dwFadeColor;

		break;
	}
}

void _XParticleEffectManager::Render()
{
	if (m_byState == STATE_NONE)	return;

	gpDev->SetRenderState(D3DRS_FOGENABLE,			FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING,			FALSE);

	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,	TRUE);
	gpDev->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA);
	gpDev->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA);

	gpDev->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE);

	gpDev->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	gpDev->SetTextureStageState(0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);

	gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE);

	gpDev->SetTexture(0, NULL);

	gpDev->SetFVF(D3DFVF_SCREENVERTEX);
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_ScreenVertex, sizeof(_SCREENVERTEX));

	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,	FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING,			TRUE);
	gpDev->SetRenderState(D3DRS_FOGENABLE,			TRUE);

	gpDev->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE);

	gpDev->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
}