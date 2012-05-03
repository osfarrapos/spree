#pragma once

///////////////////////////////////////////////////////////////////////  
//	Include Files
#include "d3dutil.h"
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <vector>

#include "XSTWrapperConfig.h"
#include "XSTWrapper.h"

#include "SpeedTreeRT.h"
#include "XKernel.h"
#include <map>
#include <string>


///////////////////////////////////////////////////////////////////////  
//	Constants
const int		c_nNumWindMatrices = 4;
const int		c_nDefaultAlphaTestValue = 84;
const float		c_fNearLodFactor = 1.0f;
const float		c_fFarLodFactor = 5.0f;

// vertex shader constant locations
const int		c_nVertexShader_LeafLightingAdjustment = 70;
const int		c_nVertexShader_Light = 71;
const int		c_nVertexShader_Material = 74;
const int		c_nVertexShader_TreePos = 52;
const int		c_nVertexShader_CompoundMatrix = 0;
const int		c_nVertexShader_WindMatrices = 54;
const int		c_nVertexShader_LeafTables = 4;
const int		c_nVertexShader_Fog = 85;

///////////////////////////////////////////////////////////////////////  
//	Branch & Frond Vertex Formats

static DWORD D3DFVF_SPEEDTREE_BRANCH_VERTEX_VS =
		D3DFVF_XYZ |
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
		D3DFVF_NORMAL |
	#else
		D3DFVF_DIFFUSE |
	#endif
		D3DFVF_TEXCOORDSIZE2(0)	|
		D3DFVF_TEXCOORDSIZE2(1) | 
		D3DFVF_TEXCOORDSIZE2(2)
	
		#ifdef WRAPPER_RENDER_SELF_SHADOWS
			| D3DFVF_TEX3;
		#else
			| D3DFVF_TEX2;
		#endif

static DWORD D3DFVF_SPEEDTREE_BRANCH_VERTEX_NS =
			D3DFVF_XYZ |
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
			D3DFVF_NORMAL |
	#else
			D3DFVF_DIFFUSE |
	#endif
			D3DFVF_TEXCOORDSIZE2(0)	|
			D3DFVF_TEXCOORDSIZE2(1) | 
			D3DFVF_TEXCOORDSIZE2(2)

	#ifdef WRAPPER_RENDER_SELF_SHADOWS
			| D3DFVF_TEX2;
	#else
			| D3DFVF_TEX1;

	#endif


///////////////////////////////////////////////////////////////////////
// FVF Branch Vertex Structure

struct SFVFBranchVertex_VS
{
		D3DXVECTOR3		m_vPosition;			// Always Used							
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING			
		D3DXVECTOR3		m_vNormal;				// Dynamic Lighting Only			
	#else										     
		DWORD			m_dwDiffuseColor;		// Static Lighting Only	
	#endif	
		FLOAT			m_fTexCoords[2];		// Always Used
	
		FLOAT			m_fWindIndex;			// GPU Only
		FLOAT			m_fWindWeight;			
	
	#ifdef WRAPPER_RENDER_SELF_SHADOWS
		FLOAT			m_fShadowCoords[2];		// Texture coordinates for the shadows
	#endif
};


struct SFVFBranchVertex_NS
{
	D3DXVECTOR3		m_vPosition;			// Always Used							
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING			
	D3DXVECTOR3		m_vNormal;				// Dynamic Lighting Only			
#else										     
	DWORD			m_dwDiffuseColor;		// Static Lighting Only	
#endif	
	FLOAT			m_fTexCoords[2];		// Always Used

#ifdef WRAPPER_RENDER_SELF_SHADOWS
	FLOAT			m_fShadowCoords[2];		// Texture coordinates for the shadows
#endif
};


///////////////////////////////////////////////////////////////////////  
//	Branch/Frond Vertex Program

static const char g_achSimpleVertexProgram[] = 
{
		"vs.1.1\n"											// identity shader version

		"dcl_position		v0\n"							// declare the data coming in
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING	
		"dcl_normal			v3\n"
	#else
		"dcl_color			v5\n"
	#endif
		"dcl_texcoord0		v7\n"
		"dcl_texcoord1		v8\n"
	#ifdef WRAPPER_RENDER_SELF_SHADOWS
		"dcl_texcoord2		v9\n"
	#endif

		"mov		oT0.xy,	v7\n"							// always pass texcoord0 through

	#ifdef WRAPPER_RENDER_SELF_SHADOWS
		"mov		oT1.xy, v9\n"							// pass shadow texcoords through
	#endif

		// retrieve and convert wind matrix index
		"mov		a0.x,	v8.x\n"

		// perform wind interpolation
		"m4x4		r1,		v0,			c[54+a0.x]\n"		// compute full wind effect
		"sub		r2,		r1,			v0\n"				// compute difference between full wind and none
		"mov		r3.x,	v8.y\n"							// mad can't access two v's at once, use r3.x as tmp
		"mad		r1,		r2,			r3.x,		v0\n"	// perform interpolation

		"add		r2,		c[52],		r1\n"				// translate to tree's position
		"m4x4		oPos,	r2,			c[0]\n"				// project to screen

	#ifdef WRAPPER_USE_FOG
		"dp4		r1,			r2,			c[2]\n"			// compute distance
		"sub		r2.x,		c[85].y,	r1.z\n"			// linear fog
		"mul		oFog,		r2.x,		c[85].z\n"		// write outputs
	#endif

	#ifdef WRAPPER_USE_STATIC_LIGHTING
		"mov		oD0,	v5\n"							// pass color through
	#else
		"mov		r1,		c[74]\n"						// can only use one const register per instruction
		"mul		r5,		c[73],		r1\n"				// diffuse values

		"mov		r1,		c[75]\n"						// can only use one const register per instruction
		"mul		r4,		c[72],		r1\n"				// ambient values
		
		"dp3		r2.x,   v3,			c[71]\n"			// dot light direction with normal
		"mad		oD0,	r2.x,		r5,			r4\n"	// compute the final color
	#endif
};


///////////////////////////////////////////////////////////////////////  
//	LoadBranchShader

inline LPDIRECT3DVERTEXSHADER9 LoadBranchShader( void )
{
	// assemble shader
	LPDIRECT3DVERTEXSHADER9 dwShader;
	LPD3DXBUFFER pCode, pError;
	if (D3DXAssembleShader(g_achSimpleVertexProgram, sizeof(g_achSimpleVertexProgram) - 1, 0, NULL, 0, &pCode, &pError) == D3D_OK)
	{
		if (gpDev->CreateVertexShader((DWORD*)pCode->GetBufferPointer( ), &dwShader) != D3D_OK)
		{
			char szError[1024];
			sprintf(szError, "Failed to create branch vertex shader.");
			MessageBox(NULL, szError, "Vertex Shader Error", MB_ICONSTOP);
		}
	}
	else
	{
		char szError[1024];
		sprintf(szError, "Failed to assemble branch vertex shader.\nThe error reported is [ %s ].\n", pError->GetBufferPointer( ));
		MessageBox(NULL, szError, "Vertex Shader Error", MB_ICONSTOP);
	}

	if (pCode)
		pCode->Release( );

	return dwShader;
}


///////////////////////////////////////////////////////////////////////  
//	Leaf Vertex Formats

static DWORD D3DFVF_SPEEDTREE_LEAF_VERTEX_VS =
		D3DFVF_XYZ |							// always have the position
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING			// precomputed colors or geometric normals
		D3DFVF_NORMAL |
	#else
		D3DFVF_DIFFUSE |
	#endif
		D3DFVF_TEXCOORDSIZE2(0) | 
		D3DFVF_TEXCOORDSIZE2(1) | 
		D3DFVF_TEXCOORDSIZE2(2) |
	
		D3DFVF_TEX3;


static DWORD D3DFVF_SPEEDTREE_LEAF_VERTEX_NS =
			D3DFVF_XYZ |							// always have the position
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING			// precomputed colors or geometric normals
			D3DFVF_NORMAL |
#else
			D3DFVF_DIFFUSE |
#endif
			D3DFVF_TEXCOORDSIZE2(0) | 
			D3DFVF_TEXCOORDSIZE2(1) | 
			D3DFVF_TEXCOORDSIZE2(2) |
			D3DFVF_TEX1;

///////////////////////////////////////////////////////////////////////
// FVF Leaf Vertex Structure

struct SFVFLeafVertex_VS
{
		D3DXVECTOR3		m_vPosition;			// Always Used							
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING			
		D3DXVECTOR3		m_vNormal;				// Dynamic Lighting Only			
	#else										     
		DWORD			m_dwDiffuseColor;		// Static Lighting Only	
	#endif											
		FLOAT			m_fTexCoords[2];		// Always Used
	
		FLOAT			m_fWindIndex;			// Only used when GPU is involved
		FLOAT			m_fWindWeight;					
		FLOAT			m_fLeafPlacementIndex;
		FLOAT			m_fLeafScalarValue;
};

struct SFVFLeafVertex_NS
{
		D3DXVECTOR3		m_vPosition;			// Always Used							
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING			
		D3DXVECTOR3		m_vNormal;				// Dynamic Lighting Only			
	#else										     
		DWORD			m_dwDiffuseColor;		// Static Lighting Only	
	#endif											
		FLOAT			m_fTexCoords[2];		// Always Used
};


///////////////////////////////////////////////////////////////////////  
//	Leaf Vertex Program

static const char g_achLeafVertexProgram[] = 
{
		"vs.1.1\n"											// identity shader version

		"dcl_position		v0\n"							// declare the data coming in
	#ifdef WRAPPER_USE_DYNAMIC_LIGHTING	
		"dcl_normal			v3\n"
	#else
		"dcl_color			v5\n"
	#endif
		"dcl_texcoord0		v7\n"
		"dcl_texcoord1		v8\n"
		"dcl_texcoord2		v9\n"

		"mov		oT0.xy,	v7\n"							// always pass texcoord0 through
		
	#ifdef WRAPPER_USE_GPU_WIND
		// retrieve and convert wind matrix index
		"mov		a0.x,	v8.x\n"

		// perform wind interpolation
		"m4x4		r1,		v0,			c[54+a0.x]\n"		// compute full wind effect
		"sub		r2,		r1,			v0\n"				// compute difference between full wind and none
		"mov		r3.x,	v8.y\n"							// mad can't access two v's at once, use r3.x as tmp
		"mad		r0,		r2,			r3.x,		v0\n"	// perform interpolation
	#else
		"mov		r0,		v0\n"							// wind already handled, pass the vertex through
	#endif

	#ifdef WRAPPER_USE_GPU_LEAF_PLACEMENT
		"mov		a0.x,	v9.x\n"							// place the leaves
		"mul		r1,		c[a0.x],	v9.y\n"
		"add		r0,		r1,			r0\n"	
	#endif

		"add		r0,		c[52],		r0\n"				// translate to tree's position
		"m4x4		oPos,	r0,			c[0]\n"				// project to screen

	#ifdef WRAPPER_USE_FOG
		"dp4		r1,			r0,			c[2]\n"			// compute distance
		"sub		r2.x,		c[85].y,	r1.z\n"			// linear fog
		"mul		oFog,		r2.x,		c[85].z\n"		// write outputs
	#endif

	#ifdef WRAPPER_USE_STATIC_LIGHTING
		"mov		oD0,	v5\n"							// pass color through
	#else
		"mov		r1,		c[74]\n"						// can only use one const register per instruction
		"mul		r5,		c[73],		r1\n"				// diffuse values

		"mov		r1,		c[75]\n"						// can only use one const register per instruction
		"mul		r4,		c[72],		r1\n"				// ambient values
		
		"dp3		r2.x,   v3,			c[71]\n"			// dot light direction with normal
		"max		r2.x,	r2.x,		c[70].x\n"			// limit it
		"mad		oD0,	r2.x,		r5,			r4\n"	// compute the final color
	#endif
};


///////////////////////////////////////////////////////////////////////  
//	LoadLeafShader

inline LPDIRECT3DVERTEXSHADER9 LoadLeafShader( void )
{
	LPDIRECT3DVERTEXSHADER9 dwShader = NULL;

	// assemble shader
	LPD3DXBUFFER pCode, pError;

	if (D3DXAssembleShader(g_achLeafVertexProgram, sizeof(g_achLeafVertexProgram) - 1, 0, NULL, 0, &pCode, &pError) == D3D_OK)
	{
		if (gpDev->CreateVertexShader((DWORD*) pCode->GetBufferPointer( ), &dwShader) != D3D_OK)
		{
			char szError[1024];
			sprintf(szError, "Failed to create leaf vertex shader.");
			(void) MessageBox(NULL, szError, "Vertex Shader Error", MB_ICONSTOP);
		}
	}
	else
	{
		char szError[1024];
		sprintf(szError, "Failed to assemble leaf vertex shader. The error reported is [ %s ].\n", (char*) pError->GetBufferPointer( ));
		(void) MessageBox(NULL, szError, "Vertex Shader Error", MB_ICONSTOP);
	}

	if (pCode)
    	pCode->Release( );

	return dwShader;
}

