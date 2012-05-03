#include <stdafx.h>

#pragma warning (disable: 4786)

#include "XSTShader.h"
#include "XSTWrapperCore.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XObjectManager.h"

using namespace std;

extern _XTextureManager	g_SpeedTreeTextureArchive;

///////////////////////////////////////////////////////////////////////  
// HLSL shaders
extern LPDIRECT3DVERTEXSHADER9		g_dwBranchVertexShader;				// branch vertex shader
extern LPDIRECT3DVERTEXSHADER9		g_dwLeafVertexShader;				// leaf vertex shader

const float     c_afWindDirection[3] = { 1.0f, 0.0f, 0.0f };

///////////////////////////////////////////////////////////////////////  
//	_XST_WrapperCore

_XST_WrapperCore::_XST_WrapperCore()
{
	m_ObjectContainerCount		= 0;
	m_ObjectContainerArray		= NULL;	

	m_TemplateData				= NULL;
	m_TemplateDataCount			= 0;
	
	m_fAccumTime				= 0.0f;
	// global wind value
	m_fWindStrength				= 0.3f;

	m_UseVertexShader			= TRUE;

	m_RenderLimitDistance		= 180.0f;

	m_D3DFVF_SPEEDTREE_BRANCH_VERTEX	= D3DFVF_SPEEDTREE_BRANCH_VERTEX_VS;
	m_D3DFVF_SPEEDTREE_LEAF_VERTEX		= D3DFVF_SPEEDTREE_LEAF_VERTEX_VS;
	m_D3DFVF_SPEEDTREE_BILLBOARD_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;	
}

_XST_WrapperCore::~_XST_WrapperCore()
{
	// by mahwang
	// Release() 함수 안에 다른 전역 변수를 지우는 객체가 있다.(g_SpeedTreeTextureArchive.DisposeTexture())
	// 그런데 둘다 전역 변수 이므로 g_SpeedTreeTextureArchive 가 먼저 지워진다면 
	// 여기서 g_SpeedTreeTextureArchive.DisposeTexture() 를 호출 하면 에러가 날 여지가 있다.
//	Release();
	SAFE_DELETE_ARRAY( m_TemplateData );
}

void _XST_WrapperCore::SetUseVertexShader( BOOL use )
{
	m_UseVertexShader = use;

	if( m_UseVertexShader )
	{
		m_D3DFVF_SPEEDTREE_BRANCH_VERTEX = D3DFVF_SPEEDTREE_BRANCH_VERTEX_VS;
		m_D3DFVF_SPEEDTREE_LEAF_VERTEX   = D3DFVF_SPEEDTREE_LEAF_VERTEX_VS;
	}
	else
	{
		m_D3DFVF_SPEEDTREE_BRANCH_VERTEX = D3DFVF_SPEEDTREE_BRANCH_VERTEX_NS;
		m_D3DFVF_SPEEDTREE_LEAF_VERTEX   = D3DFVF_SPEEDTREE_LEAF_VERTEX_NS;
	}
}

BOOL _XST_WrapperCore::Initialize( void )
{
	//Release();	
	if( !InitDeviceObjects() ) return FALSE;			
	return TRUE;
}


void _XST_WrapperCore::Release( void )
{
	g_SpeedTreeTextureArchive.DisposeTexture();
	SAFE_DELETE_ARRAY( m_ObjectContainerArray );		
	m_ObjectContainerCount = 0;

	SAFE_RELEASE(g_dwBranchVertexShader);
	SAFE_RELEASE(g_dwLeafVertexShader);
}

void _XST_WrapperCore::CleanUpInstance( void )
{
	for (unsigned int i = 0; i < m_ObjectContainerCount; ++i)
	{
		//m_ObjectContainerArray[i].CleanUpInstance();
	}
}

BOOL _XST_WrapperCore::ReserveObjectContainerArray( int count )
{	
	m_ObjectContainerCount = 0;
	SAFE_DELETE_ARRAY( m_ObjectContainerArray );
	
	if( count <= 0 )
	{
		_XLog( "WARNING : _XST_WrapperCore::ReserveObjectContainerArray( 0 )" );
		return TRUE;
	}

	if( m_UseVertexShader )
	{
		m_ObjectContainerArray = new _XST_VSObject[ count ];
	}
	else
	{
		m_ObjectContainerArray = new _XST_NSObject[ count ];
	}

	if( !m_ObjectContainerArray )
	{
		_XFatalError( "Can't create tree object template array" );
		return FALSE;
	}

	m_ObjectContainerCount = count;

	for( int i = 0; i < m_ObjectContainerCount; ++i )
	{
		m_ObjectContainerArray[i].ReserveRenderStack();
	}

	return TRUE;
}

BOOL _XST_WrapperCore::LoadSpeedTreeInstanceData(  FILE* Fileptr )
{
	_XMEMORYUSECHECKREADY		
	_XMEMORYUSECHECKSTART
		
	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );
	g_SpeedTreeTextureArchive.DisposeTexture();	

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_SSTMODEL.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_SSTMODEL.XP") );
#endif

	if( !g_SpeedTreeTextureArchive.SetTexturePackage( packagefilename ) )
	{
		return FALSE;
	}
	
	_XPackageArchive TreeModelArchive; 
	_XSetDefaultPath( _XDEF_DEFAULTPATH_MODEL ); 
#ifdef _XTESTSERVER		
	if( !TreeModelArchive.OpenPackage( _T("tsMR_STMODEL.XP") ) )
	{
		_XFatalError( "Can't open speed tree model archive" );
		return FALSE;
	}
#else
	if( !TreeModelArchive.OpenPackage( _T("MR_STMODEL.XP") ) )
	{
		_XFatalError( "Can't open speed tree model archive" );
		return FALSE;
	}
#endif



	int speedtreetemplatecount = 0;  
	if( fread( &speedtreetemplatecount, sizeof(int), 1, Fileptr ) < 1 )
	{
		TreeModelArchive.FinalizePackage();
		_XFatalError( "Can't read the speed tree template informations" );
		return FALSE;
	}

	if( !ReserveObjectContainerArray( speedtreetemplatecount ) )
	{
		TreeModelArchive.FinalizePackage();
		return FALSE;
	}

	TCHAR sptfilename[64];
	memset( sptfilename, 0, sizeof(TCHAR) * 64 );			

	for( int i = 0; i < speedtreetemplatecount; ++i )
	{	
		if( fread( sptfilename, sizeof(TCHAR) * 64, 1, Fileptr ) < 1 )
		{
			_XFatalError( "Can't read speed tree model name" );
			TreeModelArchive.FinalizePackage();
			return FALSE;
		}

		int templatedataindex = this->SearchTemplateData( sptfilename );

		if( templatedataindex <= -1 )
		{
			_XFatalError( "Could not found the speed tree template informations [%s]", sptfilename );
			TreeModelArchive.FinalizePackage();
			return FALSE;
		}

		int compositmapindex = -1;
		if( !m_ObjectContainerArray[i].LoadTree( sptfilename, &TreeModelArchive, 
												 m_TemplateData[templatedataindex].compositemapname,
												 compositmapindex,
												 -1, // seed
												 m_TemplateData[templatedataindex].scale,
												 0.0f, 0.0f, 0.0f, 0.0f ) )
		{
			_XFatalError( "Can't load speed tree object [%s]", sptfilename );
			TreeModelArchive.FinalizePackage();
			return FALSE;
		}
		
		m_ObjectContainerArray[i].SetWindStrength( 0.1f );

		// Load instance info...
		UINT instancelistsize;				
		
		if( fread( &instancelistsize, sizeof(UINT), 1, Fileptr ) < 1 )
		{
			_XFatalError( "Can't read speed tree instance count" );
			TreeModelArchive.FinalizePackage();
			return FALSE;				
		}
		
		FLOAT pos[3];
		for (unsigned int j = 0; j < instancelistsize; ++j)
		{
			if( fread( &pos, sizeof(FLOAT)*3, 1, Fileptr ) < 1 )
			{
				_XFatalError( "Can't read speed tree instance position" );
				TreeModelArchive.FinalizePackage();
				return FALSE;
			}
			else
			{
				_XST_AbstractObject* pInstance = m_ObjectContainerArray[i].MakeInstance( m_UseVertexShader, pos[0],pos[1],pos[2] );
				pInstance->SetPos( pos );
			}
		}
	}

	TreeModelArchive.FinalizePackage();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Attach speed tree texture renderstyle
	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );

#ifdef _XTESTSERVER
	g_SpeedTreeTextureArchive.ReadyRenderStyle(_T("tsSTModel.xrs"));
	if( !g_SpeedTreeTextureArchive.BuildTextureList() )
	{
		return FALSE;
	}		
	g_SpeedTreeTextureArchive.AttachRenderStyle( _T("tsSTModel.xrs") );
#else
	g_SpeedTreeTextureArchive.ReadyRenderStyle(_T("STModel.xrs"));
	if( !g_SpeedTreeTextureArchive.BuildTextureList() )
	{
		return FALSE;
	}		
	g_SpeedTreeTextureArchive.AttachRenderStyle( _T("STModel.xrs") );
#endif

	SetLodLimits();

	_XMEMORYUSECHECKEND( "LoadSpeedTreeInstanceData" );
	
	return TRUE;
}

BOOL _XST_WrapperCore::LoadTemplateData( void )
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load speed tree convert LUT		
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* scriptfp = NULL;

	scriptfp = g_ScriptArchive.GetPackedFile( _T("SpeedTreeList.ini") );

	if( !scriptfp )
	{
		_XFatalError( "Can't open tree convert lut" );
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	char buffer[256];
	char parsingString1[64];
	char parsingString2[64];
	char CompositeMapName[64];
	
	int maxtreecount = 0;

	memset( buffer, 0, sizeof(char)*256 );
	memset( parsingString1, 0, sizeof(char)*64 );
	memset( parsingString2, 0, sizeof(char)*64 );
	memset( CompositeMapName, 0, sizeof(char)*64 );
	
	if( !fgets( buffer, 255, scriptfp ) )
	{
		g_ScriptArchive.ClosePackage();
		return TRUE;
	}
	
	if( strncmp( buffer, _T("<_TOTALCOUNT_>"), 14 ) != 0 )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	sscanf( buffer, "%s %d", parsingString1, &maxtreecount );

	if( maxtreecount <= 0 )
	{
		g_ScriptArchive.ClosePackage();
		return TRUE;
	}

	SAFE_DELETE_ARRAY( m_TemplateData );
	m_TemplateDataCount = maxtreecount;
	m_TemplateData = new _XST_TemplateData[m_TemplateDataCount];
	
	memset( m_TemplateData, 0, sizeof(_XST_TemplateData)*m_TemplateDataCount);
	if( !m_TemplateData )
	{
		_XFatalError( "Can't allocate buffer : speed tree template data" );
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	int treecount = -1;
	while( 1 )
	{
		if( feof( scriptfp ) ) break;
		memset( buffer, 0, sizeof(char)*256 );
		
		if( !fgets( buffer, 255, scriptfp ) )
		{
			break;
		}
				
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		else 
		{	
			if( treecount >= maxtreecount ) break;
			
			if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
			{	

				memset( parsingString1, 0, sizeof(char)*64 );
				memset( parsingString2, 0, sizeof(char)*64 );
				sscanf( buffer, "%s %s", parsingString1, parsingString2 );

				if( strcmp( parsingString1, _T("<_COMPOSITION_MAP_>") ) == 0 )
				{
					memset( CompositeMapName, 0, sizeof(char)*64 );
					strcpy( CompositeMapName, parsingString2 );
				}
				else if( strcmp( parsingString1, _T("<_TREESCALE_>") ) == 0 )
				{
					m_TemplateData[treecount].scale = atof( parsingString2 );
				}			
				else
				{
					treecount++;
					if( treecount >= maxtreecount ) 
					{
						_XFatalError( "Overflowed speed tree stack. %d", maxtreecount );
						break;
					}

					memset( m_TemplateData[treecount].sptmodelname, 0, sizeof(TCHAR)*64 );
					strcpy( m_TemplateData[treecount].sptmodelname, parsingString1 );

					memset( m_TemplateData[treecount].compositemapname, 0, sizeof(TCHAR)*64 );
					strcpy( m_TemplateData[treecount].compositemapname, CompositeMapName );					
					m_TemplateData[treecount].scale = 10.0f;
				}									
			}
		}
	}
	
	scriptfp = g_ScriptArchive.GetPackedFile( "SpeedWind.ini" );
	if( !scriptfp )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	if( m_cSpeedWind.Load( scriptfp ) )
	{
		_XSpeedWind::SWindAttributes sWind = m_cSpeedWind.GetAttributes( );
		sWind.m_uiNumMatrices = c_nNumWindMatrices;
		m_cSpeedWind.SetAttributes(sWind);
		UpdateWindStrengths(m_fWindStrength);
	}
	else
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	g_ScriptArchive.ClosePackage();

	return TRUE;
}

int	 _XST_WrapperCore::SearchTemplateData( LPTSTR lpszSPTModelFilename )
{
	for (unsigned int i = 0; i < m_TemplateDataCount; ++i)
	{
		if( strcmpi( m_TemplateData[i].sptmodelname, lpszSPTModelFilename ) == 0 )
		{
			return i;
		}		
	}
	
	return -1;
}

BOOL _XST_WrapperCore::InitDeviceObjects( void )
{
	// initialize wind matrices
	CSpeedTreeRT::SetNumWindMatrices(4);

	InitVertexShaders();

#ifndef WRAPPER_USE_DYNAMIC_LIGHTING
	InitSpeedTreeLighting();
#endif
	
	return TRUE;
}

void _XST_WrapperCore::DeleteDeviceObjects( void )
{
	SAFE_RELEASE(g_dwBranchVertexShader);
	SAFE_RELEASE(g_dwLeafVertexShader);
}

void _XST_WrapperCore::InitSpeedTreeLighting( void )
{
	// set static light data prior to loading ".spt" files
	float afLight1[ ] =
	{
		0.650945f, -0.390567f, -0.650945f,
		//-1000.0f, 0.0f, 1000.0f,	// pos		
		0.5f, 0.5f, 0.5f,			// diffuse
		0.35f, 0.35f, 0.35f,		// ambient
		0.5f, 0.5f, 0.5f,			// specular
		0.0f,						// directional flag
		1.0f, 0.0f, 0.0f			// attenuation (constant, linear, quadratic)
	};
			
	// set light attributes and enable static light 0
	CSpeedTreeRT::SetLightAttributes(0, afLight1);	
	CSpeedTreeRT::SetLightState(0, true);
}

void _XST_WrapperCore::SetupWindMatrices( void )
{
	m_cSpeedWind.Advance(m_fAccumTime, m_fWindStrength, c_afWindDirection[0], c_afWindDirection[1], c_afWindDirection[2]);
	
	if( !m_UseVertexShader )
	{
		m_cSpeedWind.UpdateSpeedTreeRT( );
	}
	else
	{
		for (unsigned int i = 0; i < c_nNumWindMatrices; ++i)
			UploadWindMatrix(c_nVertexShader_WindMatrices + i * 4, m_cSpeedWind.GetWindMatrix(i));
	}

	/*
	// matrix computational data
	static float afMatrixTimes[c_nNumWindMatrices] = { 0.0f };
	static float afFrequencies[c_nNumWindMatrices][2] = 
	{
		{ 0.15f, 0.17f },
		{ 0.25f, 0.15f },
		{ 0.19f, 0.05f },
		{ 0.15f, 0.22f }
	};
	
	// compute time since last call
	static float fTimeOfLastCall = 0.0f;
	float fTimeSinceLastCall = m_fAccumTime - fTimeOfLastCall;
	fTimeOfLastCall = m_fAccumTime;
	
	// wind strength
	static float fOldStrength = m_fWindStrength;
	
	// increment matrix times
	for (int i = 0; i < c_nNumWindMatrices; ++i)
		afMatrixTimes[i] += fTimeSinceLastCall;
	
	// compute maximum branch throw
	float fBaseAngle = m_fWindStrength * 35.0f;
	
	// build rotation matrices
	for (i = 0; i < c_nNumWindMatrices; ++i)
	{
		// adjust time to prevent "jumping"
		if (m_fWindStrength != 0.0f)
			afMatrixTimes[i] = (afMatrixTimes[i] * fOldStrength) / m_fWindStrength;
		
		// compute percentages for each axis
		float fBaseFreq = m_fWindStrength * 20.0f;
		float fXPercent = sinf(fBaseFreq * afFrequencies[i % c_nNumWindMatrices][0] * afMatrixTimes[i]);
		float fYPercent = cosf(fBaseFreq * afFrequencies[i % c_nNumWindMatrices][1] * afMatrixTimes[i]);
		
		// build compound rotation matrix (rotate on 'x' then on 'y')
		const float c_fDeg2Rad = 57.2957795f;
        float fSinX = sinf(fBaseAngle * fXPercent / c_fDeg2Rad);
        float fSinY = sinf(fBaseAngle * fYPercent / c_fDeg2Rad);
        float fCosX = cosf(fBaseAngle * fXPercent / c_fDeg2Rad);
        float fCosY = cosf(fBaseAngle * fYPercent / c_fDeg2Rad);
		
        float afMatrix[16] = { 0.0f };
        afMatrix[0] = fCosY;
        afMatrix[2] = -fSinY;
        afMatrix[4] = fSinX * fSinY;
        afMatrix[5] = fCosX;
        afMatrix[6] = fSinX * fCosY;
        afMatrix[8] = fSinY * fCosX;
        afMatrix[9] = -fSinX;
        afMatrix[10] = fCosX * fCosY;
        afMatrix[15] = 1.0f;
		
		if( !m_UseVertexShader )
		{
			CSpeedTreeRT::SetWindMatrix(i, afMatrix);
		}
		else
		{
			// graphics API specific
			UploadWindMatrix(c_nVertexShader_WindMatrices + i * 4, afMatrix);
		}
	}
		
	// track wind strength
	fOldStrength = m_fWindStrength;
	*/
}


void _XST_WrapperCore::Process( void )
{
	// compute time
	m_fAccumTime += g_fElapsedFrameMilisecondTime;					
			
	// set the time for all SpeedTrees in seconds
	CSpeedTreeRT::SetTime(m_fAccumTime);

	// setup the wind matrices
	if( m_UseVertexShader ) SetupWindMatrices( );
		
	// advance the trees	
	for (unsigned int i = 0; i < m_ObjectContainerCount; ++i)
    {
		m_ObjectContainerArray[i].Advance();

		UINT instancelistsize;
		
		_XST_AbstractObject** pInstances = m_ObjectContainerArray[i].GetInstances( instancelistsize );
		
		for (unsigned int j = 0; j < instancelistsize; ++j)
		{
			pInstances[j]->Advance();
		}
    }
}


void _XST_WrapperCore::ResetRenderStack( void )
{
	for (unsigned int i = 0; i < m_ObjectContainerCount; ++i)
	{
		m_ObjectContainerArray[i].ResetRenderStack();
	}
}

void _XST_WrapperCore::DrawRenderStack( void )
{
	// setup composite matrix for shader
	D3DXMATRIX matBlendShader; 
	D3DXMatrixMultiply(&matBlendShader,  &g_LodTerrain.m_3PCamera.GetViewMatrix(), &g_LodTerrain.m_3PCamera.GetProjMatrix());

	float afDirection[3];
	afDirection[0] = matBlendShader.m[0][2];
	afDirection[1] = matBlendShader.m[1][2];
	afDirection[2] = matBlendShader.m[2][2];
	CSpeedTreeRT::SetCamera(g_LodTerrain.m_3PCamera.m_CameraPosition, afDirection);

	/*float afDirection[3];
	afDirection[0] = g_LodTerrain.m_3PCamera.m_CameraPosition.x - g_pLocalUser->m_Position.x;
	afDirection[1] = g_LodTerrain.m_3PCamera.m_CameraPosition.y - g_pLocalUser->m_Position.y;
	afDirection[2] = g_LodTerrain.m_3PCamera.m_CameraPosition.z - g_pLocalUser->m_Position.z;
	float fMagnitude = _XFC_sqrt(afDirection[0] * afDirection[0] + afDirection[1] * afDirection[1] + afDirection[2] * afDirection[2]);
	afDirection[0] /= fMagnitude;
	afDirection[1] /= fMagnitude;
	afDirection[2] /= fMagnitude;
	CSpeedTreeRT::SetCamera(g_LodTerrain.m_3PCamera.m_CameraPosition, afDirection);*/

	if( m_UseVertexShader )
	{
		// send matrix to shader
		D3DXMatrixTranspose(&matBlendShader, &matBlendShader);
		gpDev->SetVertexShaderConstantF(c_nVertexShader_CompoundMatrix, (float*)&matBlendShader, 4);

		// setup shader constants for light
		const float afLighting[] =
		{
			-0.650945f, -0.390567f, 0.650945f, 0.0f,
			//g_EnvironmentManager.m_ObjectLight.Direction.x,g_EnvironmentManager.m_ObjectLight.Direction.y,g_EnvironmentManager.m_ObjectLight.Direction.z, 0.0f,	// [71] light direction
			g_EnvironmentManager.m_ObjectLight.Ambient.r, g_EnvironmentManager.m_ObjectLight.Ambient.g, g_EnvironmentManager.m_ObjectLight.Ambient.b, 1.0f,	// [72] light ambient color
			g_EnvironmentManager.m_ObjectLight.Diffuse.r, g_EnvironmentManager.m_ObjectLight.Diffuse.g, g_EnvironmentManager.m_ObjectLight.Diffuse.b, 1.0f		// [73] light diffuse color
		};
		gpDev->SetVertexShaderConstantF(c_nVertexShader_Light, afLighting, 3);

		// send fog data to shader if enabled
#ifdef WRAPPER_USE_FOG
		float	fFogLinearScale = (1.0f / (g_EnvironmentManager.m_FogFar- g_EnvironmentManager.m_FogNear));
		D3DXVECTOR4 vecConstant(g_EnvironmentManager.m_FogNear, g_EnvironmentManager.m_FogFar, fFogLinearScale, 0.0f);
		gpDev->SetVertexShaderConstantF(c_nVertexShader_Fog, (float*)&vecConstant, 1);
#endif
	}

#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
	gpDev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
#else
	gpDev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
#endif

	// set up fog, if enabled
#ifdef WRAPPER_USE_FOG
	gpDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
	gpDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);

	if( m_UseVertexShader )
	{
		gpDev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	}
	else
	{
		gpDev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	}
#else
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
#endif

	/*gpDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/

	// choose fixed function pipeline or custom shader for fronds and branches	
	gpDev->SetVertexShader(g_dwBranchVertexShader);
	gpDev->SetFVF(m_D3DFVF_SPEEDTREE_BRANCH_VERTEX);

	// draw branches
	for (unsigned int i = 0; i < m_ObjectContainerCount; ++i)
	{
		m_ObjectContainerArray[i].SetupBranchForTreeType( );
		//gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//gpDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE );
		//gpDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		//gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		
		for (unsigned int j = 0; j < m_ObjectContainerArray[i].m_STObjectRenderStackSize; ++j) 
			m_ObjectContainerArray[i].m_STObjectRenderStack[j]->RenderBranches();
	}

	// draw fronds
	for (i = 0; i < m_ObjectContainerCount; ++i)
	{
		// setup texture and rendering state
		g_LastUsedRenderStyle = NULL;
		g_SpeedTreeTextureArchive.SetTexture( m_ObjectContainerArray[i].m_LeafTextureIndex );
		m_ObjectContainerArray[i].SetupFrondForTreeType( );
		gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		gpDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE );
		gpDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER ); 
				
		for (unsigned int j = 0; j < m_ObjectContainerArray[i].m_STObjectRenderStackSize; ++j) 
			m_ObjectContainerArray[i].m_STObjectRenderStack[j]->RenderFronds();
	}
	
	gpDev->SetVertexShader(g_dwLeafVertexShader);
	gpDev->SetFVF(m_D3DFVF_SPEEDTREE_LEAF_VERTEX);

#ifdef WRAPPER_USE_FOG
	if( m_UseVertexShader )
	{		
		gpDev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	}
#endif

	// draw leaves
	for (i = 0; i < m_ObjectContainerCount; ++i)
	{
		// setup texture and rendering state
		g_LastUsedRenderStyle = NULL;
		g_SpeedTreeTextureArchive.SetTexture( m_ObjectContainerArray[i].m_LeafTextureIndex );
		
		m_ObjectContainerArray[i].SetupLeafForTreeType( );
		gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		gpDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE );
		gpDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		
		for (unsigned int j = 0; j < m_ObjectContainerArray[i].m_STObjectRenderStackSize; ++j) 
			m_ObjectContainerArray[i].m_STObjectRenderStack[j]->RenderLeaves();
	}

	for (i = 0; i < m_ObjectContainerCount; ++i)
	{
		m_ObjectContainerArray[i].EndLeafForTreeType();
	}

	// render billboards
#ifdef WRAPPER_BILLBOARD_MODE
		
		gpDev->SetVertexShader(0);
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
				
		gpDev->SetFVF(m_D3DFVF_SPEEDTREE_BILLBOARD_VERTEX);
#ifdef WRAPPER_USE_FOG
		gpDev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
#endif
		for (i = 0; i < m_ObjectContainerCount; ++i)
		{
			g_LastUsedRenderStyle = NULL;
			g_SpeedTreeTextureArchive.SetTexture( m_ObjectContainerArray[i].m_LeafTextureIndex );
			gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//gpDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			//gpDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );

			for (unsigned int j = 0; j < m_ObjectContainerArray[i].m_STObjectRenderStackSize; ++j)
			{
				m_ObjectContainerArray[i].m_STObjectRenderStack[j]->RenderBillboards();
			}
		}
		
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
#else
		gpDev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
#endif
	
#endif
				

	// Set fog state machine...	
	if( g_EnvironmentManager.m_FogMode == 0 )
	{
		// Vertex fog...		
		if( g_bCanDoVertexFog )
		{
			gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
			gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
			gpDev->SetRenderState( D3DRS_RANGEFOGENABLE, g_bCanDoRangeFog );
		}
		else
		{
			gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
			gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
		}
	}
	else
	{
		// Table fog...
		if(g_bCanDoTableFog)
		{
			gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
			gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   g_EnvironmentManager.m_TableFogExpMode );
		}
		else
		{
			if( g_bCanDoVertexFog )
			{
				gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
				gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
				gpDev->SetRenderState( D3DRS_RANGEFOGENABLE, g_bCanDoRangeFog );
			}
			else
			{
				gpDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
				gpDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
			}
		}
	}	

	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	gpDev->SetVertexShader( NULL );
}


void _XST_WrapperCore::UpdateWindStrengths( FLOAT strength )
{
	m_fWindStrength = strength;

	if (m_fWindStrength < 0.0f) m_fWindStrength = 0.0f;
	if (m_fWindStrength > 1.0f) m_fWindStrength = 1.0f;

	for (int i = 0; i < m_ObjectContainerCount; ++i)
	{
		//m_ObjectContainerArray[i].SetWindStrength(m_fWindStrength);
		m_ObjectContainerArray[i].GetSpeedTree()->SetWindStrengthAndLeafAngles(m_fWindStrength, m_cSpeedWind.GetLeafAngles(_XSpeedWind::ROCK), m_cSpeedWind.GetLeafAngles(_XSpeedWind::RUSTLE), m_cSpeedWind.GetNumLeafAngles( ));
	}
}

void _XST_WrapperCore::UploadWindMatrix(unsigned int uiLocation, const float* pMatrix) const
{
	//m_cSpeedWind.Advance(m_fAccumTime, m_fWindStrength, c_afWindDirection[0], c_afWindDirection[1], c_afWindDirection[2]);

	if( m_UseVertexShader )
	{
		// set wind matrices to vertex shader
		gpDev->SetVertexShaderConstantF(uiLocation, pMatrix, 4);
	}
}


bool _XST_WrapperCore::InitVertexShaders(void)
{
	SAFE_RELEASE(g_dwBranchVertexShader);
	SAFE_RELEASE(g_dwLeafVertexShader);

	// load the vertex shaders
	if( m_UseVertexShader )
	{
		g_dwBranchVertexShader = LoadBranchShader();
		g_dwLeafVertexShader = LoadLeafShader();
	}
	else
	{
		g_dwBranchVertexShader = NULL;
		g_dwLeafVertexShader = NULL;
	}
	
	return true;
}

void _XST_WrapperCore::CheckInsideFrustum( void )
{
	for (unsigned int i = 0; i < m_ObjectContainerCount; ++i)
	{
		UINT instancelistsize;
		m_ObjectContainerArray[i].ResetRenderStack();
		
		_XST_AbstractObject** pInstances = m_ObjectContainerArray[i].GetInstances( instancelistsize );
		
		for (unsigned int j = 0; j < instancelistsize; ++j)
		{
			FLOAT dx = (pInstances[j]->m_afPos[0] - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
			FLOAT dz = (pInstances[j]->m_afPos[2] - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
			
			if( _XFastDistance( dx , dz ) < m_RenderLimitDistance && 
				( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( 
							(D3DXVECTOR3)pInstances[j]->m_afPos, 
							(pInstances[j]->m_afBoundingBox[5] - pInstances[j]->m_afBoundingBox[2]) ) != NOT_VISIBLE ) )
			{
				m_ObjectContainerArray[i].InsertRenderingInstance( pInstances[j] );
			}
		}
	}
}

void _XST_WrapperCore::SetLodLimits( void )
{
	return;

	float fTallest = -1.0f;

	for (unsigned int i = 0; i < m_ObjectContainerCount; ++i)
	{
		float fHeight = m_ObjectContainerArray[i].m_afPos[5] - m_ObjectContainerArray[i].m_afPos[0];
		fTallest = __max(fHeight, fTallest);
	}

	// assign all trees based on tallest
	for (i = 0; i < m_ObjectContainerCount; ++i)
	{
		m_ObjectContainerArray[i].GetSpeedTree( )->SetLodLimits(80, 250);
		//m_ObjectContainerArray[i].GetSpeedTree( )->SetLodLimits(fTallest * c_fNearLodFactor, fTallest * c_fFarLodFactor);
	}
}