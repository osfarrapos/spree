// XShaderWaterNoiseGenerator.cpp: implementation of the _XShaderWaterNoiseGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "XKernel.h"
#include "XShaderWaterNoiseGenerator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XShaderWaterNoiseGenerator::_XShaderWaterNoiseGenerator(int sizeX, int sizeY, int octaves, int animlength, _XShaderWaterParameterHandler *prm)
{
	this->animlength	= 256; //animlength;		// must be power of two.. add checking later
	this->octaves		= octaves;
	patches_x			= 32;
	patches_y			= 128;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->prm = prm;
	create_vertexbuffer();
	prepare_textures();
	LoadEffect();
}

_XShaderWaterNoiseGenerator::~_XShaderWaterNoiseGenerator()
{
	rendered_texture->Release();
	rendered_normalmap->Release();
	RenderToSurface->Release();
	RenderToNormalmap->Release();
	rendered_texture_surface->Release();
	VB->Release();
	IB->Release();
	source_texture->Release();
	rendered_normalmap_surface->Release();
	anim_effect->Release();
	normalmap_effect->Release();
}

void _XShaderWaterNoiseGenerator::create_vertexbuffer()
{
	NOISEVERTEX *anim;

	// Create the vertex buffer.
	gpDev->CreateVertexBuffer( (patches_x+1)*(patches_y+1)*sizeof(NOISEVERTEX),
		0, D3DFVF_NOISEVERTEX,
		D3DPOOL_DEFAULT, &VB, NULL );

	// Fill the vertex buffer	
	int i = 0; 
	if( !FAILED( VB->Lock( 0, 0, (void**)&anim, 0 ) ) ){
		for(int v=0; v<(patches_y+1); v++){
			for(int u=0; u<(patches_x+1); u++)
			{
				anim[i++].position = D3DXVECTOR3((float)u/patches_x,(float)v/patches_y,0);
			}
		}
		VB->Unlock();
	}

	gpDev->CreateIndexBuffer(	sizeof(unsigned int) * 6 * (patches_x)*(patches_y),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,	D3DPOOL_DEFAULT,&IB,NULL);

	unsigned int *indexbuffer;
	if( !FAILED( IB->Lock(0,0,(void**)&indexbuffer,0 ) ) )
	{
		i = 0;
		for(int v=0; v<patches_y; v++){
			for(int u=0; u<patches_x; u++){
				// face 1 |/
				indexbuffer[i++]	= v*(patches_x+1) + u;
				indexbuffer[i++]	= v*(patches_x+1) + u + 1;
				indexbuffer[i++]	= (v+1)*(patches_x+1) + u;

				// face 2 /|
				indexbuffer[i++]	= (v+1)*(patches_x+1) + u;
				indexbuffer[i++]	= v*(patches_x+1) + u + 1;
				indexbuffer[i++]	= (v+1)*(patches_x+1) + u + 1;
			}
		}
		IB->Unlock();
	}

	this->offsets = new tc[animlength];
	for( i=0; i<animlength; i++)
	{
		offsets[i].x = (float) rand()/RAND_MAX;
		offsets[i].y = (float) rand()/RAND_MAX;
	}

	// Create the quad vertex buffer
	
	gpDev->CreateVertexBuffer( 6*sizeof(NOISEVERTEX),
		0, D3DFVF_NOISEVERTEX,
		D3DPOOL_DEFAULT, &quad, NULL );
	
	NOISEVERTEX *qV;

	if( !FAILED( quad->Lock( 0, 0, (void**)&qV, 0 ) ) ){
	
		qV[0].position = D3DXVECTOR3(-1,-1,0);
		qV[1].position = D3DXVECTOR3(+1,-1,0);
		qV[2].position = D3DXVECTOR3(-1,+1,0);
		qV[3].position = D3DXVECTOR3(-1,+1,0);
		qV[4].position = D3DXVECTOR3(+1,-1,0);
		qV[5].position = D3DXVECTOR3(+1,+1,0);		

		quad->Unlock();
	}

}

void _XShaderWaterNoiseGenerator::prepare_textures()
{
	// load source noise texture
	if( FAILED( D3DXCreateTextureFromFile( gpDev, "textures/noise-uniform.png", &source_texture ) ) )
	{		
			MessageBox(NULL, "Could not find noise texture", "projgriddemo", MB_OK);		
	}
	HRESULT hr = gpDev->CreateTexture(sizeX,sizeY,1,D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &(this->rendered_texture),NULL);
	
	D3DSURFACE_DESC desc;

	rendered_texture->GetSurfaceLevel( 0, &rendered_texture_surface );
	rendered_texture_surface->GetDesc( &desc );

	D3DXCreateRenderToSurface( gpDev, 
		desc.Width, 
		desc.Height, 
		desc.Format, 
		FALSE, 
		D3DFMT_UNKNOWN, 
		&RenderToSurface );

	hr = gpDev->CreateTexture(sizeX,sizeY,1,D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16, D3DPOOL_DEFAULT, &(this->rendered_normalmap),NULL);
	rendered_normalmap->GetSurfaceLevel( 0, &rendered_normalmap_surface );
	rendered_normalmap_surface->GetDesc( &desc );


	D3DXCreateRenderToSurface( gpDev, 
		desc.Width, 
		desc.Height, 
		desc.Format, 
		FALSE, 
		D3DFMT_UNKNOWN, 
		&RenderToNormalmap );

	// create all the intermediary texture octaves
	for(int i=0; i<octaves; i++)
	{
		hr = gpDev->CreateTexture(128,128,0,D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16, D3DPOOL_DEFAULT, &(this->noise_octaves[i]),NULL);	//D3DFMT_R32F
		noise_octaves[i]->GetSurfaceLevel( 0, &noise_octave_surface[i] );
	}
		
	noise_octave_surface[0]->GetDesc( &desc );

	D3DXCreateRenderToSurface( gpDev, 
		desc.Width, 
		desc.Height, 
		desc.Format, 
		FALSE, 
		D3DFMT_UNKNOWN, 
		&RenderToOctaves );

}

void _XShaderWaterNoiseGenerator::render_noise_octaves()
{
	
	DWORD itime = timeGetTime() & ((1<<23) - 1);

	
	double	r_timemulti = prm->params[p_fAnimspeed].fData;

	for(int i=0; i<octaves; i++)
	{
		if ( RenderToOctaves->BeginScene(noise_octave_surface[i],NULL) )
			{
			noise_octaves_effect->Begin(NULL,NULL);
			noise_octaves_effect->Pass(0);
	
			gpDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	
			gpDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			gpDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			unsigned int image[3];
			float amount[3];
			double dImage, fraction = modf((double)itime*r_timemulti,&dImage);
			int iImage = (int)dImage;
			amount[0] = pow(sin((fraction+2)*_X_PI/3),2)/1.5;
			amount[1] = pow(sin((fraction+1)*_X_PI/3),2)/1.5;
			amount[2] = pow(sin((fraction)*_X_PI/3),2)/1.5;
			image[0] = (iImage) & (animlength-1);
			image[1] = (iImage+1) & (animlength-1);
			image[2] = (iImage+2) & (animlength-1);

			noise_octaves_effect->SetFloat("a",amount[0]);
			noise_octaves_effect->SetFloat("b",amount[1]);
			noise_octaves_effect->SetFloat("c",amount[2]);
			noise_octaves_effect->SetFloat("mapsize",128.0f);
			noise_octaves_effect->SetFloat("srcsize",512.0f);
			noise_octaves_effect->SetFloat("ratio",0.25f);			

			noise_octaves_effect->SetTexture("NoiseTex",source_texture);

			noise_octaves_effect->SetVector("tc_offset_a", &D3DXVECTOR4(offsets[image[0]].x,offsets[image[0]].y,0,1));
			noise_octaves_effect->SetVector("tc_offset_b", &D3DXVECTOR4(offsets[image[1]].x,offsets[image[1]].y,0,1));
			noise_octaves_effect->SetVector("tc_offset_c", &D3DXVECTOR4(offsets[image[2]].x,offsets[image[2]].y,0,1));
	
			gpDev->SetStreamSource( 0, quad, 0, sizeof(NOISEVERTEX) );
			gpDev->SetFVF( D3DFVF_NOISEVERTEX );		
			gpDev->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2);
			r_timemulti *= prm->params[p_fTimemulti].fData;			
			
			// end rendertarget
			noise_octaves_effect->End();
			RenderToOctaves->EndScene(0);
			}
		}	
		
}

void _XShaderWaterNoiseGenerator::LoadEffect(){

	char *errortext;
	LPD3DXBUFFER errors;
	// load the perlin noise effect
	D3DXCreateEffectFromFile(gpDev, "noisemakermk2.fx", 
		NULL, NULL, 0, NULL, &anim_effect, &errors );
	if (errors != NULL){
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}
	D3DXHANDLE hTechnique;
	anim_effect->FindNextValidTechnique(NULL, &hTechnique);    
	anim_effect->SetTechnique(hTechnique);

	// and the normalmap effect
	D3DXCreateEffectFromFile(gpDev, "normalmapgenerator.fx", 
		NULL, NULL, 0, NULL, &normalmap_effect, &errors );
	if (errors != NULL){
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}	
	normalmap_effect->FindNextValidTechnique(NULL, &hTechnique);    
	normalmap_effect->SetTechnique(hTechnique);

	// and the noise_octaves effect
	D3DXCreateEffectFromFile(gpDev, "noise_octaves.fx", 
		NULL, NULL, 0, NULL, &noise_octaves_effect, &errors );
	if (errors != NULL){
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}	
	noise_octaves_effect->FindNextValidTechnique(NULL, &hTechnique);    
	noise_octaves_effect->SetTechnique(hTechnique);

}

void _XShaderWaterNoiseGenerator::render(const D3DXMATRIXA16 *mProjector)
{
	this->mProjector = *mProjector;

	if (RenderToSurface->BeginScene(rendered_texture_surface,NULL))
	{

		DWORD itime = timeGetTime() & ((1<<23) - 1);

		gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		gpDev->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
		anim_effect->Begin(NULL,NULL);
		anim_effect->Pass(0);

		double r_scale	= prm->params[p_fScale].fData,
			r_timemulti = prm->params[p_fAnimspeed].fData,
			r_strength = 1.0f,
			r_falloff = prm->params[p_fFalloff].fData;
		// normalize r_strength
		float sum=0;
		for(int i=0; i<octaves; i++)
		{
			sum += r_strength;
			r_strength *= r_falloff;
		}
		r_strength = 1.0f / sum;

		for(i=0; i<octaves; i++)
		{
			unsigned int image[3];
			float amount[3];
			double dImage, fraction = modf((double)itime*r_timemulti,&dImage);
			int iImage = (int)dImage;
			amount[0] = pow(sin((fraction+2)*_X_PI/3),2)/1.5;
			amount[1] = pow(sin((fraction+1)*_X_PI/3),2)/1.5;
			amount[2] = pow(sin((fraction)*_X_PI/3),2)/1.5;
			image[0] = (iImage) & (animlength-1);
			image[1] = (iImage+1) & (animlength-1);
			image[2] = (iImage+2) & (animlength-1);


			anim_effect->SetFloat("a",amount[0]);
			anim_effect->SetFloat("b",amount[1]);
			anim_effect->SetFloat("c",amount[2]);
			anim_effect->SetFloat("scale",r_scale);
			anim_effect->SetFloat("strength",r_strength);

			anim_effect->SetVector("tc_offset_a", &D3DXVECTOR4(offsets[image[0]].x,offsets[image[0]].y,0,1));
			anim_effect->SetVector("tc_offset_b", &D3DXVECTOR4(offsets[image[1]].x,offsets[image[1]].y,0,1));
			anim_effect->SetVector("tc_offset_c", &D3DXVECTOR4(offsets[image[2]].x,offsets[image[2]].y,0,1));

			anim_effect->SetTexture("NoiseTex",source_texture);
			anim_effect->SetMatrix("mProjector",mProjector);
			gpDev->SetStreamSource( 0, VB, 0, sizeof(NOISEVERTEX) );
			gpDev->SetFVF( D3DFVF_NOISEVERTEX );
			gpDev->SetIndices( IB );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 	
				0,			
				0,
				(patches_x+1)*(patches_y+1),
				0,
				2*patches_x*patches_y-1 );

			r_scale *= 2;
			r_timemulti *= 1.5;
			r_strength *= r_falloff;		
		}
		RenderToSurface->EndScene(0);
		gpDev->SetTexture(0,NULL);
		gpDev->SetTexture(1,NULL);
		gpDev->SetTexture(2,NULL);
		anim_effect->End();
		gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}
}

void _XShaderWaterNoiseGenerator::render_projected_noise(const D3DXMATRIXA16 *mProjector)
{
	this->mProjector = *mProjector;

	if (RenderToSurface->BeginScene(rendered_texture_surface,NULL))
	{

		anim_effect->Begin(NULL,NULL);
		anim_effect->Pass(0);

		float  r_scale	= prm->params[p_fScale].fData,			   
			   r_strength = 1.0f,
			   r_falloff = prm->params[p_fFalloff].fData;

		float scale[4], amplitude[4];

		// normalize r_strength
		float sum=0;
		for(int i=0; i<octaves; i++)
		{
			sum += r_strength;
			amplitude[i] = r_strength;
			r_strength *= r_falloff;
			scale[i] = r_scale;
			r_scale *= 2;
			
		}
		r_strength = 1.0f / sum;

					
		anim_effect->SetFloat("scale",r_scale);
		anim_effect->SetFloat("strength",r_strength);
		anim_effect->SetFloat("sum_inverse",0.5*r_strength);
		
		anim_effect->SetTexture("nOctave0",noise_octaves[0]);
		anim_effect->SetTexture("nOctave1",noise_octaves[1]);
		anim_effect->SetTexture("nOctave2",noise_octaves[2]);
		anim_effect->SetTexture("nOctave3",noise_octaves[3]);		
		anim_effect->SetFloatArray("scale",scale,4);
		anim_effect->SetFloatArray("amplitude",amplitude,4);
			
		anim_effect->SetMatrix("mProjector",mProjector);
		gpDev->SetStreamSource( 0, VB, 0, sizeof(NOISEVERTEX) );
		gpDev->SetFVF( D3DFVF_NOISEVERTEX );
		gpDev->SetIndices( IB );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 	
				0,			
				0,
				(patches_x+1)*(patches_y+1),
				0,
				2*patches_x*patches_y-1 );
		
		
		RenderToSurface->EndScene(0);
		anim_effect->End();		
	}
}

void _XShaderWaterNoiseGenerator::render_normalmap()
{

	if (RenderToNormalmap->BeginScene(rendered_normalmap_surface,NULL))
	{
		normalmap_effect->Begin(NULL,NULL);
		normalmap_effect->Pass(0);

		normalmap_effect->SetFloat("amplitude", prm->params[p_fStrength].fData);
		normalmap_effect->SetFloat("mapsize_x", this->sizeX);
		normalmap_effect->SetFloat("mapsize_y", this->sizeY);
		normalmap_effect->SetMatrix("mProjector",&mProjector);
		D3DXMATRIXA16 mInvProjector;
		D3DXMatrixInverse( &mInvProjector, NULL, &mProjector);
		normalmap_effect->SetMatrix("mInvProjector",&mInvProjector);
		
		gpDev->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
		
		gpDev->SetStreamSource( 0, VB, 0, sizeof(NOISEVERTEX) );
		gpDev->SetFVF( D3DFVF_NOISEVERTEX );
		normalmap_effect->SetTexture("Heightmap",rendered_texture);
		gpDev->SetIndices( IB );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0,
				(patches_x+1)*(patches_y+1), 0, 2*patches_x*patches_y-1 );
				
		normalmap_effect->End();
		
		RenderToNormalmap->EndScene(0);
	}
}