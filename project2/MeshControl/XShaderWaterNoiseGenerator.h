// XShaderWaterNoiseGenerator.h: interface for the _XShaderWaterNoiseGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSHADERWATERNOISEGENERATOR_H__79654A90_205F_4AFA_8A2F_89A2D4C54675__INCLUDED_)
#define AFX_XSHADERWATERNOISEGENERATOR_H__79654A90_205F_4AFA_8A2F_89A2D4C54675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#include "XShaderWaterParameterHandler.h"

struct tc
{
	float x, y;
};

struct NOISEVERTEX
{
	D3DXVECTOR3	position;
};

#define D3DFVF_NOISEVERTEX (D3DFVF_XYZ)

class _XShaderWaterNoiseGenerator 
{
public:
	_XShaderWaterNoiseGenerator(int sizeX, int sizeY, int octaves, int animlength, _XShaderWaterParameterHandler *prm);	
	~_XShaderWaterNoiseGenerator();

	void render(const D3DXMATRIXA16 *mProjector);
	void render_projected_noise(const D3DXMATRIXA16 *mProjector);
	void render_normalmap();
	void render_noise_octaves();
	LPDIRECT3DTEXTURE9			rendered_texture;
	LPDIRECT3DTEXTURE9			rendered_normalmap;
	LPD3DXRENDERTOSURFACE		RenderToSurface;
	LPD3DXRENDERTOSURFACE		RenderToNormalmap;
	LPDIRECT3DSURFACE9			rendered_texture_surface;
	LPDIRECT3DTEXTURE9			noise_octaves[16];
	
private:
	D3DXMATRIXA16 mProjector;
	void prepare_textures();
	void create_vertexbuffer();
	void LoadEffect();
	int sizeX, sizeY, sourcesize, animlength, octaves, patches_x, patches_y; 
	float falloff;

	LPDIRECT3DVERTEXBUFFER9		VB,quad;
	LPDIRECT3DINDEXBUFFER9		IB;
	LPDIRECT3DTEXTURE9			source_texture;
	
	LPDIRECT3DSURFACE9			noise_octave_surface[16];
	LPDIRECT3DSURFACE9			backbuffer, rendertarget;
	
	LPD3DXRENDERTOSURFACE		RenderToOctaves;
	LPDIRECT3DSURFACE9			rendered_normalmap_surface;
	LPD3DXEFFECT				anim_effect, normalmap_effect, noise_octaves_effect;
	
	tc							  *offsets;
	_XShaderWaterParameterHandler *prm;
};



#endif // !defined(AFX_XSHADERWATERNOISEGENERATOR_H__79654A90_205F_4AFA_8A2F_89A2D4C54675__INCLUDED_)
