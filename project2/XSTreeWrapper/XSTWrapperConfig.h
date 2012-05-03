#pragma once

///////////////////////////////////////////////////////////////////////  
//	Include Files
#include "SpeedTreeRT.h"
#include <d3dx9.h>

// setup lighting (enable ONE of the two below)
//#define WRAPPER_USE_STATIC_LIGHTING
#define WRAPPER_USE_DYNAMIC_LIGHTING

#if defined WRAPPER_USE_STATIC_LIGHTING && defined WRAPPER_USE_DYNAMIC_LIGHTING
#error Please define exactly one lighting mode
#endif

// setup wind (enable ONE of the three below)
#define WRAPPER_USE_GPU_WIND
//#define WRAPPER_USE_CPU_WIND
//#define WRAPPER_USE_NO_WIND

#define WRAPPER_USE_GPU_LEAF_PLACEMENT

// billboard modes 
//#define WRAPPER_BILLBOARD_MODE
//#define WRAPPER_RENDER_HORIZONTAL_BILLBOARD


// render self-shadows
//#define WRAPPER_RENDER_SELF_SHADOWS

// use fog
#define WRAPPER_USE_FOG

///////////////////////////////////////////////////////////////////////  
//	Color conversion macro
#define AGBR2ARGB(dwColor) (dwColor & 0xff00ff00) + ((dwColor & 0x00ff0000) >> 16) + ((dwColor & 0x000000ff) << 16)


///////////////////////////////////////////////////////////////////////  
//	class CSpeedTreeMaterial declaration/definiton

extern LPDIRECT3DDEVICE9			gpDev;

class CSpeedTreeMaterial
{
public:
	CSpeedTreeMaterial( )
	{
		m_cMaterial.Ambient.r = m_cMaterial.Diffuse.r = m_cMaterial.Specular.r = m_cMaterial.Emissive.r = 1.0f;
		m_cMaterial.Ambient.g = m_cMaterial.Diffuse.g = m_cMaterial.Specular.g = m_cMaterial.Emissive.g = 1.0f;
		m_cMaterial.Ambient.b = m_cMaterial.Diffuse.b = m_cMaterial.Specular.b = m_cMaterial.Emissive.b = 1.0f;
		m_cMaterial.Ambient.a = m_cMaterial.Diffuse.a = m_cMaterial.Specular.a = m_cMaterial.Emissive.a = 1.0f;
		m_cMaterial.Power = 5.0f;
	}
	
	void Set(const float* pMaterialArray)
	{
		memcpy(&m_cMaterial.Diffuse, pMaterialArray, 3 * sizeof(float));
		m_cMaterial.Diffuse.a = 1.0f;
		
		memcpy(&m_cMaterial.Ambient, pMaterialArray + 3, 3 * sizeof(float));
		m_cMaterial.Ambient.a = 1.0f;
		
		memcpy(&m_cMaterial.Specular, pMaterialArray + 6, 3 * sizeof(float));
		m_cMaterial.Specular.a = 1.0f;
		
		memcpy(&m_cMaterial.Emissive, pMaterialArray + 9, 3 * sizeof(float));
		m_cMaterial.Emissive.a = 1.0f;
		
		m_cMaterial.Power = pMaterialArray[12];
	}
	
	void Activate(void) const
	{
		(void) gpDev->SetMaterial(&m_cMaterial);
	}
	
private:
	D3DMATERIAL9	m_cMaterial;	// material object
};



