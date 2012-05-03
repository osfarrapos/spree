// XShaderWaterSurface.cpp: implementation of the _XShaderWaterSurface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XShaderWaterSurface.h"
#include "XShaderWaterNoiseGenerator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XShaderWaterSurface::_XShaderWaterSurface(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *n, 
										   int size_x, int size_y, camera *renderingcamera, parameterhandler *prm)
{
	initialized = true;
	D3DXPlaneFromPointNormal( &plane, pos, n);
	D3DXVec3Normalize(&(this->normal), n);

	// calculate the u and v-vectors
	// take one of two vectors (the one further away from the normal) and force it into the plane
	D3DXVECTOR3 x;
	if(fabs( D3DXVec3Dot(&D3DXVECTOR3(1,0,0),&normal)) < fabs(D3DXVec3Dot(&D3DXVECTOR3(0,0,1),&normal))){
		x = D3DXVECTOR3(1,0,0);
	} else {
		x = D3DXVECTOR3(0,0,1);
	}
	u = x - normal*D3DXVec3Dot(&normal,&x);
	D3DXVec3Normalize(&u,&u);
	// get v (cross)
	D3DXVec3Cross(&v,&u,&normal);

	this->prm = prm;
	this->pos = *pos;	
	this->gridsize_x = size_x+1;
	this->gridsize_y = size_y+1;
	this->rendering_camera = renderingcamera;
	this->projecting_camera = NULL;
	this->observing_camera = NULL;
	this->rendermode = RM_SOLID;
	this->boxfilter = false;


	set_displacement_amplitude(0.0f);

	if (!initbuffers()) initialized = false;		// init vertex & indexbuffers

	software_brus = new _XShaderWaterNoiseGenerator(gridsize_x,gridsize_y, prm, device);

	/*if( FAILED( D3DXCreateTextureFromFile( gpDev, "textures/dirt_crackeddrysoft_df_.dds", &surf_texture ) ) )
	{
	MessageBox(NULL, "Could not find texturehonk", "Textures.exe", MB_OK);
	initialized = false;
	}
	*/
	if( FAILED( D3DXCreateTextureFromFile( gpDev, "textures/fresnel_water_linear.bmp", &surf_fresnel ) ) )
	{
		MessageBox(NULL, "Could not find fresnelmap", "Textures.exe", MB_OK);
		initialized = false;		
	}
	if( FAILED( D3DXCreateTextureFromFile( gpDev, "textures/reflection_underwater.bmp", &underwater_fresnel ) ) )
	{
		MessageBox(NULL, "Could not find underwater fresnelmap", "Textures.exe", MB_OK);
		initialized = false;		
	}
	if( FAILED( D3DXCreateTextureFromFile( gpDev, "textures/XZnoise.png", &noise2D ) ) )
	{
		MessageBox(NULL, "Could not find noise texture", "Textures.exe", MB_OK);
		initialized = false;		
	}
		
	//if( FAILED( D3DXCreateCubeTextureFromFile( gpDev, "textures/evening.dds", &sky_cubemap ) ) )
	if( FAILED( D3DXCreateCubeTextureFromFile( gpDev, "textures/cubemap-evul.dds", &sky_cubemap ) ) )
	{		
		MessageBox(NULL, "Could not find cubemap", "asdf", MB_OK);
		initialized = false;
	}

	gpDev->CreateTexture(reflrefrdetail,reflrefrdetail,1,D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surf_reflection,NULL);	
	gpDev->CreateTexture(reflrefrdetail,reflrefrdetail,1,D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surf_refraction,NULL);		
	
	gpDev->CreateDepthStencilSurface( reflrefrdetail, reflrefrdetail,D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &depthstencil, NULL );

	if(!initialized)
		MessageBox(NULL, "Something went wrong in initialization of the class surface", "surface", MB_OK);

	this->LoadEffect();
}


_XShaderWaterSurface::~_XShaderWaterSurface()
{
#ifdef inc_hwpath
	this->surf_indicies->Release();
	this->surf_vertices->Release();
#endif
	this->skybox_indicies->Release();
}

void _XShaderWaterSurface::LoadEffect()
{
	//HRESULT hr;
	//TCHAR sz[512];
	//DXUtil_FindMediaFileCb(sz, sizeof(sz), _T("stuff.fx"));
	//LPD3DXEFFECT l_pEffect;
	char *errortext;
	LPD3DXBUFFER errors;
	D3DXHANDLE hTechnique;

	// same for skybox.fx
	D3DXCreateEffectFromFile(gpDev, "skybox.fx", 
		NULL, NULL, 0, NULL, &skybox_effect, &errors );

	if (errors != NULL){
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}

	skybox_effect->FindNextValidTechnique(NULL, &hTechnique);    
	skybox_effect->SetTechnique(hTechnique);

	// same for surf_software_effect
#ifdef CPU_NORMALS
	D3DXCreateEffectFromFile(gpDev, "water_soft.fx", 
		NULL, NULL, 0, NULL, &surf_software_effect, &errors );
#else 
	D3DXCreateEffectFromFile(gpDev, "water_R300.fx", 
		NULL, NULL, 0, NULL, &surf_software_effect, &errors );
#endif

	if (errors != NULL){
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}

	surf_software_effect->FindNextValidTechnique(NULL, &hTechnique);    
	surf_software_effect->SetTechnique(hTechnique);

	/*
	//	underwater_software_effect
		D3DXCreateEffectFromFile(gpDev, "underwater_soft.fx", 
		NULL, NULL, 0, NULL, &underwater_software_effect, &errors );

	if (errors != NULL){
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}

	underwater_software_effect->FindNextValidTechnique(NULL, &hTechnique);    

	underwater_software_effect->SetTechnique(hTechnique);*/
}

/*--------------------------------------------------------------------------------------------------------------
initbuffers

prepare the vertex and indexbuffer with a uniform grid (dependant on the size parameter)	
--------------------------------------------------------------------------------------------------------------*/

bool _XShaderWaterSurface::initbuffers()
{
	SURFACEVERTEX* pdVertices;	
	// create the vertexbuffer used in the softwaremode (it can be empty as it'll be memcpy-ed to)
	if( FAILED( gpDev->CreateVertexBuffer( gridsize_x*gridsize_y*sizeof(SOFTWARESURFACEVERTEX),
		D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_SOFTWARESURFACEVERTEX,
		D3DPOOL_DEFAULT, &surf_software_vertices, NULL ) ) )
	{
		return false;
	}		

	// create the skybox vertexbuffer
	if( FAILED( gpDev->CreateVertexBuffer( skyboxdetail*skyboxdetail*sizeof(SURFACEVERTEX),
		D3DUSAGE_WRITEONLY, D3DFVF_SURFACEVERTEX,
		D3DPOOL_DEFAULT, &skybox_vertices, NULL ) ) )
	{
		return false;
	}
	if( FAILED( skybox_vertices->Lock( 0, 0, (void**)&pdVertices, 0 ) ) )
		return false;
	{
		for(int v=0; v<skyboxdetail; v++){
			for(int u=0; u<skyboxdetail; u++){
				float	al = -2*3.14159265*((float)u/(skyboxdetail-1.0f)),
					th = 0.6*3.14159265*((float)v/(skyboxdetail-1.0f));
				pdVertices[v*skyboxdetail+u].position.x = sin(th)*sin(al);
				pdVertices[v*skyboxdetail+u].position.y = cos(th);
				pdVertices[v*skyboxdetail+u].position.z = sin(th)*cos(al);
				pdVertices[v*skyboxdetail+u].displacement = 0.0f;
			}
		}
	}
	skybox_vertices->Unlock();

	// create/fill the indexbuffer

	if(	FAILED( gpDev->CreateIndexBuffer(	sizeof(unsigned int) * 6 * (gridsize_x-1)*(gridsize_y-1),
		D3DUSAGE_WRITEONLY,			
		D3DFMT_INDEX32,	D3DPOOL_DEFAULT,&surf_indicies,NULL)))
	{
		return false;
	}
	unsigned int *indexbuffer;
	if( FAILED( surf_indicies->Lock(0,0,(void**)&indexbuffer,0 ) ) )
		return false;
	int i = 0;
	{
		for(int v=0; v<gridsize_y-1; v++){
			for(int u=0; u<gridsize_x-1; u++){
				// face 1 |/
				indexbuffer[i++]	= v*gridsize_x + u;
				indexbuffer[i++]	= v*gridsize_x + u + 1;
				indexbuffer[i++]	= (v+1)*gridsize_x + u;

				// face 2 /|
				indexbuffer[i++]	= (v+1)*gridsize_x + u;
				indexbuffer[i++]	= v*gridsize_x + u + 1;
				indexbuffer[i++]	= (v+1)*gridsize_x + u + 1;
			}
		}
	}
	surf_indicies->Unlock();

	// create/fill the indexbuffer

	if(	FAILED( gpDev->CreateIndexBuffer(	sizeof(unsigned int) * 6 * (skyboxdetail-1)*(skyboxdetail-1),
		D3DUSAGE_WRITEONLY,			
		D3DFMT_INDEX32,	D3DPOOL_DEFAULT,&skybox_indicies,NULL)))
	{
		return false;
	}

	if( FAILED( skybox_indicies->Lock(0,0,(void**)&indexbuffer,0 ) ) )
		return false;
	i = 0;
	{
		for(int v=0; v<skyboxdetail-1; v++){
			for(int u=0; u<skyboxdetail-1; u++){
				// face 1 |/
				indexbuffer[i++]	= v*skyboxdetail + u;
				indexbuffer[i++]	= v*skyboxdetail + u + 1;
				indexbuffer[i++]	= (v+1)*skyboxdetail + u;

				// face 2 /|
				indexbuffer[i++]	= (v+1)*skyboxdetail + u;
				indexbuffer[i++]	= v*skyboxdetail + u + 1;
				indexbuffer[i++]	= (v+1)*skyboxdetail + u + 1;
			}
		}
	}
	skybox_indicies->Unlock();

	return true;
}

bool _XShaderWaterSurface::within_frustum(const D3DXVECTOR3 *pos)
{
	D3DXVECTOR3 test;
	D3DXVec3TransformCoord(&test, pos, &(rendering_camera->viewproj));
	if((fabs(test.x) < 1.00001f)&&(fabs(test.y) < 1.00001f)&&(fabs(test.z) < 1.00001f))
		return true;
	return false;
}

float dispmulti(float dist){
	return max(0, min(1, dist-1));
}

/*--------------------------------------------------------------------------------------------------------------
render

render surface
--------------------------------------------------------------------------------------------------------------*/

bool _XShaderWaterSurface::prepare(const camera *cam)
{
	if(!initialized) return false;
	
	delete observing_camera;
	observing_camera = new camera(cam);
	this->SetupMatrices(this->observing_camera);		// obsolete with vertexshaders
	
	plane_within_frustum = this->getMinMax(&range);
	
	if (plane_within_frustum){
		
		software_brus->render_geometry(&range);
		
		D3DVERTEXBUFFER_DESC pDesc;
		SOFTWARESURFACEVERTEX *vertices;
		HRESULT hr = surf_software_vertices->GetDesc( &pDesc );
		
		if( FAILED(surf_software_vertices->Lock( 0, 0, (void**) &vertices, D3DLOCK_DISCARD)))	
		{
			MessageBox(NULL, "Could not lock vertexbuffer", "Textures.exe", MB_OK);
		}
		else
		{
			int size = pDesc.Size;
			memcpy(vertices, software_brus->vertices, size);
			surf_software_vertices->Unlock();
			
		}		
	}
	

	return true;
}

void _XShaderWaterSurface::render_skybox()
{
	gpDev->SetStreamSource( 0, skybox_vertices, 0, sizeof(SURFACEVERTEX) );
	gpDev->SetFVF( D3DFVF_SURFACEVERTEX);
	gpDev->SetIndices(skybox_indicies);
	skybox_effect->Begin(NULL,NULL);
	skybox_effect->Pass(0);
	

	// build the 'fake' viweproj with the distance vector set to 0,0,0

	D3DXMATRIXA16 fvproj(observing_camera->view);
	fvproj._41 = 0;
	fvproj._42 = 0;
	fvproj._43 = 0;
	fvproj = fvproj * observing_camera->proj;

	skybox_effect->SetMatrix("mViewProj",&fvproj);
	skybox_effect->SetMatrix("mInvViewProj",&(observing_camera->invviewproj));	
	skybox_effect->SetMatrix("mInvView",&(observing_camera->invview));	
	skybox_effect->SetFloat("sun_alfa", prm->params[p_fSunPosAlpha].fData);
	skybox_effect->SetFloat("sun_theta", prm->params[p_fSunPosTheta].fData);
	skybox_effect->SetFloat("sun_shininess", 4*prm->params[p_fSunShininess].fData);
	skybox_effect->SetFloat("sun_strength", prm->params[p_fSunStrength].fData);	

	skybox_effect->SetVector("view_position", &D3DXVECTOR4(observing_camera->position.x,observing_camera->position.y,observing_camera->position.z,1));	
	skybox_effect->SetTexture("EnvironmentMap",sky_cubemap);	

	gpDev->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST,	0,	0, skyboxdetail*skyboxdetail, 0, 2*(skyboxdetail-1)*(skyboxdetail-1) );

	skybox_effect->End();
}

void _XShaderWaterSurface::render_cutter()
{
	if (plane_within_frustum)
	{		
		//gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW  );
		gpDev->SetStreamSource( 0, surf_software_vertices, 0, sizeof(SOFTWARESURFACEVERTEX) );
		gpDev->SetFVF( D3DFVF_SOFTWARESURFACEVERTEX);			
		gpDev->SetIndices(surf_indicies);
		gpDev->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST, 0,	0, gridsize_x*gridsize_y, 0, 2*(gridsize_x-1)*(gridsize_y-1) );			
	}
}

bool _XShaderWaterSurface::render()
{
	if (plane_within_frustum)
	{		
		{			
			HRESULT hr;
			/*// underwater pass
			gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW  );
			gpDev->SetStreamSource( 0, surf_software_vertices, 0, sizeof(SOFTWARESURFACEVERTEX) );
			gpDev->SetFVF( D3DFVF_SOFTWARESURFACEVERTEX);			
			gpDev->SetIndices(surf_indicies);

			underwater_software_effect->Begin(NULL,NULL);
			underwater_software_effect->Pass(0);			
			underwater_software_effect->SetMatrix("mViewProj",&(observing_camera->viewproj));
			underwater_software_effect->SetFloat("sun_alfa", prm->params[p_fSunPosAlpha].fData);
			underwater_software_effect->SetFloat("sun_theta", prm->params[p_fSunPosTheta].fData);
			underwater_software_effect->SetFloat("sun_shininess", prm->params[p_fSunShininess].fData);
			underwater_software_effect->SetFloat("sun_strength", prm->params[p_fSunStrength].fData);
			underwater_software_effect->SetVector("watercolour", &D3DXVECTOR4(prm->params[p_fWaterColourR].fData,prm->params[p_fWaterColourG].fData,prm->params[p_fWaterColourB].fData,1));

			underwater_software_effect->SetFloat("LODbias", prm->get_float(p_fLODbias) );
			underwater_software_effect->SetVector("view_position", &D3DXVECTOR4(observing_camera->position.x,observing_camera->position.y,observing_camera->position.z,1));
			underwater_software_effect->SetTexture("EnvironmentMap",sky_cubemap);
			underwater_software_effect->SetTexture("FresnelMap",underwater_fresnel);
			underwater_software_effect->SetTexture("Normalmap",software_brus->normalmap);

			
			if ( prm->params[p_bAsPoints].bData )
				gpDev->DrawPrimitive( D3DPT_POINTLIST, 0, gridsize_x*gridsize_y );
			else
				gpDev->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST, 0,	0, gridsize_x*gridsize_y, 0, 2*(gridsize_x-1)*(gridsize_y-1) );				
			
			underwater_software_effect->End();
			*/
			// above water pass
			gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE  );	
			gpDev->SetStreamSource( 0, surf_software_vertices, 0, sizeof(SOFTWARESURFACEVERTEX) );
			gpDev->SetFVF( D3DFVF_SOFTWARESURFACEVERTEX);			
			gpDev->SetIndices(surf_indicies);

#ifndef CPU_NORMALS
			software_brus->generate_normalmap();
#endif			

			gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW  );
			surf_software_effect->Begin(NULL,NULL);
			surf_software_effect->Pass(0);			
			surf_software_effect->SetMatrix("mViewProj",&(observing_camera->viewproj));
			surf_software_effect->SetMatrix("mView",&(observing_camera->view));
			/*surf_software_effect->SetFloat("sun_alfa", prm->params[p_fSunPosAlpha].fData);
			surf_software_effect->SetFloat("sun_theta", prm->params[p_fSunPosTheta].fData);*/
			float sa = prm->params[p_fSunPosAlpha].fData, st = prm->params[p_fSunPosTheta].fData;
			surf_software_effect->SetVector("sun_vec",&D3DXVECTOR4(cos(st)*sin(sa), sin(st), cos(st)*cos(sa),0));
			surf_software_effect->SetFloat("sun_shininess", prm->params[p_fSunShininess].fData);			
			surf_software_effect->SetFloat("sun_strength", prm->params[p_fSunStrength].fData);
			surf_software_effect->SetFloat("reflrefr_offset", prm->params[p_bReflRefrStrength].fData);			
			surf_software_effect->SetBool("diffuseSkyRef", prm->params[p_bDiffuseRefl].bData);

			surf_software_effect->SetVector("watercolour", &D3DXVECTOR4(prm->params[p_fWaterColourR].fData,prm->params[p_fWaterColourG].fData,prm->params[p_fWaterColourB].fData,1));
			surf_software_effect->SetFloat("LODbias", prm->get_float(p_fLODbias) );
			surf_software_effect->SetVector("view_position", &D3DXVECTOR4(observing_camera->position.x,observing_camera->position.y,observing_camera->position.z,1));
			surf_software_effect->SetTexture("EnvironmentMap",sky_cubemap);
			surf_software_effect->SetTexture("FresnelMap",surf_fresnel);
#ifndef CPU_NORMALS
			surf_software_effect->SetTexture("Heightmap",software_brus->heightmap);
			surf_software_effect->SetTexture("Normalmap",software_brus->normalmap);
#endif
			surf_software_effect->SetTexture("Refractionmap",surf_refraction);
			surf_software_effect->SetTexture("Reflectionmap",surf_reflection);

			//surf_software_effect->SetTexture("noiseXZ",noise2D);

			 if ( prm->params[p_bAsPoints].bData )
			 {
				//gpDev->DrawPrimitive( D3DPT_POINTLIST, 0, gridsize_x*gridsize_y );
				 gpDev->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
				gpDev->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST, 0,	0, gridsize_x*gridsize_y, 0, 2*(gridsize_x-1)*(gridsize_y-1) );								
			 }
			else
			{
				gpDev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				gpDev->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST, 0,	0, gridsize_x*gridsize_y, 0, 2*(gridsize_x-1)*(gridsize_y-1) );				
			}

			surf_software_effect->End();
		} 
	}
#ifndef CPU_NORMALS
	if (prm->params[p_bDisplayTargets].bData)
	{
		debug_render_quad(gpDev, software_brus->packed_noise_texture[0], 0);
		debug_render_quad(gpDev, software_brus->packed_noise_texture[1], 1);
		debug_render_quad(gpDev, software_brus->heightmap, 2);
		debug_render_quad(gpDev, software_brus->normalmap, 3);
		debug_render_quad(gpDev, surf_refraction, 4);
		debug_render_quad(gpDev, surf_reflection, 5);
	}
#endif

	return true;
}

/*--------------------------------------------------------------------------------------------------------------
getMinMax

get the matrix that defines the minimum rectangle in which the frustum is located
--------------------------------------------------------------------------------------------------------------*/
bool _XShaderWaterSurface::getMinMax(D3DXMATRIXA16 *range)
{
	set_displacement_amplitude(prm->params[p_fStrength].fData);
	float		x_min,y_min,x_max,y_max;
	D3DXVECTOR3 frustum[8],proj_points[24];		// frustum to check the camera against

	int n_points=0;
	int cube[] = {	0,1,	0,2,	2,3,	1,3,
		0,4,	2,6,	3,7,	1,5,
		4,6,	4,5,	5,7,	6,7};	// which frustum points are connected together?

	// transform frustum points to worldspace (should be done to the rendering_camera because it's the interesting one)
	D3DXVec3TransformCoord(&frustum[0], &D3DXVECTOR3(-1,-1,-1), &(rendering_camera->invviewproj));
	D3DXVec3TransformCoord(&frustum[1], &D3DXVECTOR3(+1,-1,-1), &(rendering_camera->invviewproj));
	D3DXVec3TransformCoord(&frustum[2], &D3DXVECTOR3(-1,+1,-1), &(rendering_camera->invviewproj));
	D3DXVec3TransformCoord(&frustum[3], &D3DXVECTOR3(+1,+1,-1), &(rendering_camera->invviewproj));
	D3DXVec3TransformCoord(&frustum[4], &D3DXVECTOR3(-1,-1,+1), &(rendering_camera->invviewproj));
	D3DXVec3TransformCoord(&frustum[5], &D3DXVECTOR3(+1,-1,+1), &(rendering_camera->invviewproj));
	D3DXVec3TransformCoord(&frustum[6], &D3DXVECTOR3(-1,+1,+1), &(rendering_camera->invviewproj));
	D3DXVec3TransformCoord(&frustum[7], &D3DXVECTOR3(+1,+1,+1), &(rendering_camera->invviewproj));	


	// check intersections with upper_bound and lower_bound	
	for(int i=0; i<12; i++){
		int src=cube[i*2], dst=cube[i*2+1];
		if ((upper_bound.a*frustum[src].x + upper_bound.b*frustum[src].y + upper_bound.c*frustum[src].z + upper_bound.d*1)/(upper_bound.a*frustum[dst].x + upper_bound.b*frustum[dst].y + upper_bound.c*frustum[dst].z + upper_bound.d*1)<0){			
			D3DXPlaneIntersectLine( &proj_points[n_points++], &upper_bound, &frustum[src], &frustum[dst]);			
		}
		if ((lower_bound.a*frustum[src].x + lower_bound.b*frustum[src].y + lower_bound.c*frustum[src].z + lower_bound.d*1)/(lower_bound.a*frustum[dst].x + lower_bound.b*frustum[dst].y + lower_bound.c*frustum[dst].z + lower_bound.d*1)<0){			
			D3DXPlaneIntersectLine( &proj_points[n_points++], &lower_bound, &frustum[src], &frustum[dst]);			
		}
	}
	// check if any of the frustums vertices lie between the upper_bound and lower_bound planes
	{
		for(int i=0; i<8; i++){		
			if ((upper_bound.a*frustum[i].x + upper_bound.b*frustum[i].y + upper_bound.c*frustum[i].z + upper_bound.d*1)/(lower_bound.a*frustum[i].x + lower_bound.b*frustum[i].y + lower_bound.c*frustum[i].z + lower_bound.d*1)<0){			
				proj_points[n_points++] = frustum[i];
			}		
		}	
	}

	//
	// create the camera the grid will be projected from
	//
	delete projecting_camera;
	projecting_camera = new camera(rendering_camera);
	// make sure the camera isn't too close to the plane
	float height_in_plane = (lower_bound.a*projecting_camera->position.x +
		lower_bound.b*projecting_camera->position.y +
		lower_bound.c*projecting_camera->position.z);

	bool keep_it_simple = false;
	bool underwater=false;

	if (height_in_plane < 0.0f) underwater = true;

	if(keep_it_simple)
	{
		projecting_camera->forward = rendering_camera->forward;
		projecting_camera->update_lookat();
	}
	else
	{
		D3DXVECTOR3 aimpoint, aimpoint2;		

		if (height_in_plane < (prm->params[p_fStrength].fData+prm->get_float(p_fElevation)))
		{					
			if(underwater)
				projecting_camera->position += D3DXVECTOR3(lower_bound.a,lower_bound.b,lower_bound.c)*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - 2*height_in_plane);															
			else
				projecting_camera->position += D3DXVECTOR3(lower_bound.a,lower_bound.b,lower_bound.c)*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - height_in_plane);
		} 
		
		// aim the projector at the point where the camera view-vector intersects the plane
		// if the camera is aimed away from the plane, mirror it's view-vector against the plane
		if( (D3DXPlaneDotNormal(&plane, &(rendering_camera->forward)) < 0.0f) log_xor (D3DXPlaneDotCoord(&plane, &(rendering_camera->position)) < 0.0f ) )
		{				
			D3DXPlaneIntersectLine( &aimpoint, &plane, &(rendering_camera->position), &(rendering_camera->position + rendering_camera->forward) );			
		}
		else
		{
			D3DXVECTOR3 flipped;
			flipped = rendering_camera->forward - 2*normal*D3DXVec3Dot(&(rendering_camera->forward),&normal);
			D3DXPlaneIntersectLine( &aimpoint, &plane, &(rendering_camera->position), &(rendering_camera->position + flipped) );			
		}

			// force the point the camera is looking at in a plane, and have the projector look at it
			// works well against horizon, even when camera is looking upwards
			// doesn't work straight down/up
			float af = fabs(D3DXPlaneDotNormal(&plane, &(rendering_camera->forward)));
			//af = 1 - (1-af)*(1-af)*(1-af)*(1-af)*(1-af);
			//aimpoint2 = (rendering_camera->position + rendering_camera->zfar * rendering_camera->forward);
			aimpoint2 = (rendering_camera->position + 10.0f * rendering_camera->forward);
			aimpoint2 = aimpoint2 - normal*D3DXVec3Dot(&aimpoint2,&normal);
		
			// fade between aimpoint & aimpoint2 depending on view angle
			
			aimpoint = aimpoint*af + aimpoint2*(1.0f-af);
			//aimpoint = aimpoint2;
			
			projecting_camera->forward = aimpoint-projecting_camera->position;
			projecting_camera->update_lookat();
	}



	sprintf( debugdata, "n_points %i\n",n_points);
	{
		for(int i=0; i<n_points; i++){
			// project the point onto the surface plane
			proj_points[i] = proj_points[i] - normal*D3DXVec3Dot(&proj_points[i],&normal);	
		}
	}

	{
		for(int i=0; i<n_points; i++){
			D3DXVec3TransformCoord( &proj_points[i], &proj_points[i], &(projecting_camera->view));	 
			sprintf( debugdata, "%s%f  %f  %f\n",debugdata,proj_points[i].x,proj_points[i].y,proj_points[i].z);
			D3DXVec3TransformCoord( &proj_points[i], &proj_points[i], &(projecting_camera->proj));
		}
	}

	// debughonk

	/*	for(int i=0; i<n_points; i++){
	sprintf( debugdata, "%s%f  %f  %f\n",debugdata,proj_points[i].x,proj_points[i].y,proj_points[i].z);
	}*/

	// get max/min x & y-values to determine how big the "projection window" must be
	if (n_points > 0){
		x_min = proj_points[0].x;
		x_max = proj_points[0].x;
		y_min = proj_points[0].y;
		y_max = proj_points[0].y;
		for(int i=1; i<n_points; i++){
			if (proj_points[i].x > x_max) x_max = proj_points[i].x;
			if (proj_points[i].x < x_min) x_min = proj_points[i].x;
			if (proj_points[i].y > y_max) y_max = proj_points[i].y;
			if (proj_points[i].y < y_min) y_min = proj_points[i].y;
		}		
		

		sprintf( debugdata, "%sx = [%f..%f] y = [%f..%f]\n",debugdata,x_min,x_max,y_min,y_max);

		sprintf( debugdata, "%sheight_in_plane: %f\n",debugdata,height_in_plane);
		
		//sprintf( debugdata,	"%slimit_y_upper = %f\n",debugdata,limit_y_upper);
		//		sprintf( debugdata, "%sy1 = [%f] y2 = [%f]\n",debugdata,y1,y2);

		// build the packing matrix that spreads the grid across the "projection window"
		D3DXMATRIXA16 pack(	x_max-x_min,	0,				0,		x_min,
							0,				y_max-y_min,	0,		y_min,
							0,				0,				1,		0,	
							0,				0,				0,		1);
		D3DXMatrixTranspose(&pack,&pack);
		*range = pack*projecting_camera->invviewproj;

		return true;
	}
	return false;
}



/*--------------------------------------------------------------------------------------------------------------
SetupMatrices

sets the matrices according to the camera
--------------------------------------------------------------------------------------------------------------*/
void _XShaderWaterSurface::SetupMatrices(const camera *camera_view)
{
	D3DXMATRIXA16 matWorld,matProj;
	D3DXMatrixIdentity(&matWorld);
	gpDev->SetTransform( D3DTS_WORLD, &matWorld );
	gpDev->SetTransform( D3DTS_VIEW, &(camera_view->view) );
	gpDev->SetTransform( D3DTS_PROJECTION, &(camera_view->proj) );
}

void _XShaderWaterSurface::set_grid_size(int size_x, int size_y)
{
	this->gridsize_x = size_x+1;
	this->gridsize_y = size_y+1;
	this->surf_indicies->Release();
	this->surf_software_vertices->Release();
	this->skybox_indicies->Release();
	this->skybox_vertices->Release();
	this->initbuffers();
	
	software_brus->resize(gridsize_x, gridsize_y);
}

void _XShaderWaterSurface::set_render_mode(int rendermode)
{
	this->rendermode = rendermode;
}

void _XShaderWaterSurface::set_displacement_amplitude(float amplitude)
{
	D3DXPlaneFromPointNormal( &(this->upper_bound), &(this->pos + amplitude * this->normal), &(this->normal));
	D3DXPlaneFromPointNormal( &(this->lower_bound), &(this->pos - amplitude * this->normal), &(this->normal));	
}

float _XShaderWaterSurface::get_height_at( float x, float z )
{
	if (software_brus)
		return software_brus->get_height_at(x,z);
	return 0.0f;
}

void _XShaderWaterSurface::calc_efficiency()
{
	efficiency = 0;
	for(int i=0; i<(gridsize_x*gridsize_y); i++)
	{
		D3DXVECTOR3 pos;
		pos.x = software_brus->vertices[i].x;
		pos.y = software_brus->vertices[i].y;
		pos.z = software_brus->vertices[i].z;
		if (this->within_frustum(&pos))
			efficiency += 1.0f;
	}
	efficiency /= gridsize_x*gridsize_y;
}