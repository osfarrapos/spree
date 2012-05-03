// KUWater.cpp: implementation of the CWater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <direct.h>
#include <mmsystem.h>
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWater.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=__FILE__;
#endif


extern void GetFileNameFromPathString(TCHAR *value, TCHAR *line);
extern void _EncodeData(LPSTR string, BYTE key);

BOOL CWater::m_UseObjectReflection = TRUE;
BOOL CWater::m_UseTerrainReflection = TRUE;
BOOL CWater::m_bUseClipPlanes = FALSE;
_XWATER_RENDERINGMODE CWater::m_WaterRenderingMode = _XWATER_RM_NORMAL;

D3DXPLANE						g_planeWater;
D3DXMATRIXA16					g_matWaterBumpMat;

IDirect3DIndexBuffer9*			g_WaterObjectIndices		 = NULL;
LPDIRECT3DTEXTURE9				g_pBumpMap					 = NULL;
LPDIRECT3DVERTEXSHADER9			g_pWaterBumpVertexShader	 = NULL;
LPDIRECT3DVERTEXDECLARATION9	g_pWaterBumpShaderDeclation	 = NULL;
LPDIRECT3DSURFACE9				g_pWaterEnvRenderSurface	 = NULL;
LPDIRECT3DTEXTURE9				g_pWaterEnvRenderTexture	 = NULL;


static const char g_WaterShaderVertexProgram[] = 
{
	"vs.1.1\n"
		
	"dcl_position v0\n"
	"dcl_texcoord v1\n"
		
	"m4x4 oPos, v0, c3\n" //transform position to the projection space
		
	// Compute vertex position in the camera space - this is our texture coordinates
	"dp4 r0.x, v0, c0\n" 
	"dp4 r0.y, v0, c1\n" 
	"dp4 r0.z, v0, c2\n" 
		
	// Do the rest of texture transform (first part was combined with the camera matrix) 
	"rcp r0.z, r0.z\n" 
	"mad oT1.x, r0.x, r0.z, c8.x\n" 
	"mad oT1.y, r0.y, r0.z, c8.y\n" 
		
	"mov oT0.xy, v1\n"	//Copy input texture coordinates for the stage 0;	
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWater::CWater()
{
	m_pWaterObjectList = NULL;
			
	m_iSelectedTile = -1;

	m_iLapDirection = DEF_LAP_CLIMB;

	for(int i = 0; i < DEF_WATERCLASS_COUNT; i++ )
	{
		m_WaterDiffuseTextureIndex[i] = 0;
		m_WaterDetailTextureIndex[i] = 0;
	}

	m_iSeaIndexArray = NULL;
	m_iRiverIndexArray = NULL;
	m_iLakeIndexArray = NULL;
	m_iSwampIndexArray = NULL;

	m_iSeaCount = 0;
	m_iRiverCount = 0;
	m_iLakeCount = 0;
	m_iSwampCount = 0;

	m_fHeightOffset = 0.0f;	
	m_WaterReflectionClipPlaneHeight = 0.0f;	
	
	D3DXMatrixIdentity(&m_matWaterMatrix);	
	D3DXMatrixIdentity( &g_matWaterBumpMat );
}

CWater::CWater(int iTileCount, int iTerrainVertexCount, FLOAT width, FLOAT height)
{
	m_pWaterObjectList = NULL;
		
	m_iWaterTileCount = iTileCount * iTileCount;
	m_iTerrainTileCount = iTileCount;
	m_iTerrainVertexCount = iTerrainVertexCount;
	
	m_iSelectedTile = -1;

	m_fWidth = width;
	m_fHeight = height;

	m_iLapDirection = DEF_LAP_CLIMB;

	m_iSeaIndexArray = NULL;
	m_iRiverIndexArray = NULL;
	m_iLakeIndexArray = NULL;
	m_iSwampIndexArray = NULL;
	
	m_iSeaCount = 0;
	m_iRiverCount = 0;
	m_iLakeCount = 0;
	m_iSwampCount = 0;

	m_fHeightOffset = 0.0f;
	m_WaterReflectionClipPlaneHeight = 0.0f;
	
	D3DXMatrixIdentity(&m_matWaterMatrix);
}

CWater::~CWater()
{
	Release();
}

void CWater::SetSelected(int index, DWORD dwSelected, BOOL bIsSelectOne)
{
	if(bIsSelectOne)
	{
		m_iSelectedTileList.clear();
		for(int i = 0 ; i < m_iWaterTileCount ; i++)
			m_pWaterObjectList[i].selected = FALSE;
	}

	m_pWaterObjectList[index].selected = dwSelected;
	
	BOOL		bFound = FALSE;
	list <int>::iterator iter;
	for(iter = m_iSelectedTileList.begin() ; iter != m_iSelectedTileList.end() ; iter++)
		if(index == *iter)
			bFound = TRUE;
	if(!bFound)
		m_iSelectedTileList.push_back(index);
}

// create water objects
BOOL CWater::Create(void)
{
	// water object 생성에 필요한 값 체크
	if(m_iWaterTileCount <= 0)
	{
		_XFatalError("Invalid Water Tile Count ");
		return FALSE;
	}
	if(m_iTerrainVertexCount <= 0)
	{
		_XFatalError("Invalid Ground Vertex Count ");
		return FALSE;
	}

	m_pWaterObjectList = new S_WATER[m_iWaterTileCount];
	if(!m_pWaterObjectList)
	{
		_XFatalError("Water Object memory allocation Fail ");
		return FALSE;
	}

	return TRUE;
}

void CWater::ReleaseWaterIndexBuffer( void )
{
	SAFE_RELEASE( g_WaterObjectIndices );
}

BOOL CWater::CreateWaterIndexBuffer( void )
{
	SAFE_RELEASE( g_WaterObjectIndices );

	if (SUCCEEDED(gpDev->CreateIndexBuffer(6*2, g_3DObjectMemoryUsage, D3DFMT_INDEX16 , 
		g_3DObjectMemoryPool, &g_WaterObjectIndices, NULL)))
	{
		WORD* pIndex;
		if (SUCCEEDED(g_WaterObjectIndices->Lock(0,6*2,	(void**)&pIndex, g_3DObjectLockMethods)))
		{										
			pIndex[0] = 0;
			pIndex[1] = 1;
			pIndex[2] = 2;
			pIndex[3] = 2;
			pIndex[4] = 3;
			pIndex[5] = 0;
			
			g_WaterObjectIndices->Unlock();
		}
		else 
		{
			return FALSE;
		}
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}

void CWater::Release(void)
{
	if(m_pWaterObjectList)
	{
		/*
		for( int vertindex = 0; vertindex < m_iWaterTileCount; vertindex++ )
		{
			SAFE_RELEASE( m_pWaterObjectList[vertindex].vertices );
		}
		*/

		SAFE_DELETE_ARRAY(m_pWaterObjectList);		
	}


	SAFE_DELETE_ARRAY( m_iSeaIndexArray   );
	SAFE_DELETE_ARRAY( m_iRiverIndexArray );	
	SAFE_DELETE_ARRAY( m_iLakeIndexArray  );
	SAFE_DELETE_ARRAY( m_iSwampIndexArray );
	
	m_iSelectedTileList.clear();
}

void CWater::SetWaterTextureIndex(int waterclass, LPSTR basefilename, LPSTR detailfilename)
{	
	if( waterclass >= DEF_WATERCLASS_COUNT ) return; 

	m_WaterDiffuseTextureIndex[waterclass] = g_TerrainTextureArchive.GetResourceIndex( basefilename, g_TextureMipLevel_Water );
	m_WaterDetailTextureIndex[waterclass]  = g_TerrainTextureArchive.GetResourceIndex( detailfilename, g_TextureMipLevel_Water );	
}

// Draw water
void CWater::Draw(void)
{	
	gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );

	if( m_WaterRenderingMode != _XWATER_RM_NORMAL )
	{
		Draw_UseShader();
		return;
	}

	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );
	
	if(SUCCEEDED(gpDev->SetFVF(_XWATERVERTEX_FVF)))
	{
		if (SUCCEEDED(gpDev->SetIndices( g_WaterObjectIndices)))
		{
			SetTextures(DEF_WATERCLASS_SEA);
			EnableAnimation();
			for(int i = 0 ; i < m_iSeaCount ; i++)
			{
				if(m_pWaterObjectList[m_iSeaIndexArray[i]].infrustum)
				{
					if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iSeaIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
					{						
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
					}					
				}
			}

			SetTextures(DEF_WATERCLASS_RIVER);
			for(i = 0 ; i < m_iRiverCount ; i++)
			{
				if(m_pWaterObjectList[m_iRiverIndexArray[i]].infrustum)
				{
					if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iRiverIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
					{						
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
					}					
				}					
			}

			SetTextures(DEF_WATERCLASS_LAKE);
			for(i = 0 ; i < m_iLakeCount ; i++)
			{
				if(m_pWaterObjectList[m_iLakeIndexArray[i]].infrustum)
				{
					if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iLakeIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
					{						
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
					}					
				}					
			}

			SetTextures(DEF_WATERCLASS_SWAMP);
			for(i = 0 ; i < m_iSwampCount ; i++)
			{
				if(m_pWaterObjectList[m_iSwampIndexArray[i]].infrustum)
				{
					if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iSwampIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
					{						
						gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
					}
				}
			}

		}
	}
	
	DiableAnimation();
}

// Load water object from file
BOOL CWater::Load( FILE *FilePtr, _XLodTerrain* pLodTerrain )
{
	if(!FilePtr) return FALSE;

	S_WATERPROPERTY			*pWaterInfo = NULL;
	S_WATERHEIGHT			*pWaterHeight = NULL;

	TCHAR					szTextureFilename[8][32];	
	
	fread(&m_iWaterTileCount, sizeof(int), 1, FilePtr);

	pWaterInfo		= new S_WATERPROPERTY[m_iWaterTileCount];		
	pWaterHeight	= new S_WATERHEIGHT[ m_iWaterTileCount ];
		
	fread(pWaterInfo, m_iWaterTileCount * sizeof(S_WATERPROPERTY), 1, FilePtr);
	fread(pWaterHeight, m_iWaterTileCount * sizeof(S_WATERHEIGHT), 1, FilePtr);
	
	for(int i = 0 ; i < DEF_WATERCLASS_COUNT*2 ; i++)
	{
		fread(szTextureFilename[i], 32, 1, FilePtr);
	}

	m_iSeaCount = m_iRiverCount = m_iLakeCount = m_iSwampCount = 0;
	for(i = 0 ; i < m_iWaterTileCount ; i++)
	{
		m_pWaterObjectList[i].infrustum = FALSE;
		m_pWaterObjectList[i].vertices	= NULL;

		m_pWaterObjectList[i].waterclass = pWaterInfo[i].waterclass;
		m_pWaterObjectList[i].animationclass = pWaterInfo[i].animationclass;
		m_pWaterObjectList[i].enabled = pWaterInfo[i].enabled;

		m_pWaterObjectList[i].usepixelshader = pWaterInfo[i].usepixelshader;
		m_pWaterObjectList[i].direction		 = pWaterInfo[i].direction;
		m_pWaterObjectList[i].height		 = pWaterHeight[i].height;

		m_pWaterObjectList[i].uvDirection	 = pWaterHeight[i].UVDirection;
		m_pWaterObjectList[i].uvVelocity	 = pWaterHeight[i].uvVelocity;
		m_pWaterObjectList[i].lapHeightMax	 = pWaterHeight[i].lapHeightMax;
		m_pWaterObjectList[i].lapVelocity	 = pWaterHeight[i].lapVelocity;

		if( m_pWaterObjectList[i].enabled )
		{
			switch(m_pWaterObjectList[i].waterclass)
			{
			default :
			case DEF_WATERCLASS_SEA		:	m_iSeaCount++;		break;
			case DEF_WATERCLASS_RIVER	:	m_iRiverCount++;	break;
			case DEF_WATERCLASS_LAKE	:	m_iLakeCount++;		break;
			case DEF_WATERCLASS_SWAMP	:	m_iSwampCount++;	break;
			}
		}
	}

	delete[] pWaterInfo;
	delete[] pWaterHeight;

	SAFE_DELETE_ARRAY( m_iSeaIndexArray );
	if(m_iSeaCount > 0) m_iSeaIndexArray = new int[m_iSeaCount];
	
	SAFE_DELETE_ARRAY( m_iRiverIndexArray ); 
	if(m_iRiverCount > 0) m_iRiverIndexArray = new int[m_iRiverCount];

	SAFE_DELETE_ARRAY( m_iLakeIndexArray );
	if(m_iLakeCount > 0) m_iLakeIndexArray = new int[m_iLakeCount];
	
	SAFE_DELETE_ARRAY( m_iSwampIndexArray );
	if(m_iSwampCount > 0) m_iSwampIndexArray = new int[m_iSwampCount];

	int insertpoint[4];
	insertpoint[3] = insertpoint[2] = insertpoint[1] = insertpoint[0] = 0;

	for(i = 0 ; i < m_iWaterTileCount ; i++)
	{
		if( m_pWaterObjectList[i].enabled )
		{
			switch(m_pWaterObjectList[i].waterclass)
			{
			default :
			case DEF_WATERCLASS_SEA		:	
				{
					m_iSeaIndexArray[insertpoint[0]] = i;										
					insertpoint[0]++;
				}
				break;
			case DEF_WATERCLASS_RIVER	:
				{
					m_iRiverIndexArray[insertpoint[1]] = i;
					insertpoint[1]++;
				}
				break;
			case DEF_WATERCLASS_LAKE	:
				{
					m_iLakeIndexArray[insertpoint[2]] = i;
					insertpoint[2]++;
				}
				break;
			case DEF_WATERCLASS_SWAMP	:	
				{
					m_iSwampIndexArray[insertpoint[3]] = i;
					insertpoint[3]++;
				}
				break;
			}

			if (FAILED(gpDev->CreateVertexBuffer(sizeof(WATER_VERTEX)*4, g_3DObjectMemoryUsage, 
												_XWATERVERTEX_FVF, g_3DObjectMemoryPool, 
												&(m_pWaterObjectList[i].vertices), NULL)))
			{
				_XFatalError("Can't create water vertices");
				return FALSE;
			}
			else
			{
				int xpos = i % m_iTerrainTileCount;
				int zpos = i / m_iTerrainTileCount;
				
				_XLODTILE* pTerrainTile = pLodTerrain->GetTerrainTile( zpos, xpos );

				WATER_VERTEX* pVertices;
				if (SUCCEEDED(m_pWaterObjectList[i].vertices->Lock(0,0, (void**)&pVertices, g_3DObjectLockMethods)))
				{
					pVertices[0].vert = pTerrainTile->VertexBuffer[ 0 ];
					pVertices[1].vert = pTerrainTile->VertexBuffer[ 8 ];
					pVertices[2].vert = pTerrainTile->VertexBuffer[ 80 ];
					pVertices[3].vert = pTerrainTile->VertexBuffer[ 72 ];

					pVertices[3].vert.y = pVertices[2].vert.y = pVertices[1].vert.y = pVertices[0].vert.y = m_pWaterObjectList[i].height;
					
					// texture 좌표 생성
					pVertices[0].tu = xpos;
					pVertices[0].tv = zpos;
					pVertices[1].tu = pVertices[0].tu;
					pVertices[1].tv = pVertices[0].tv + 1;
					pVertices[2].tu = pVertices[1].tu + 1;
					pVertices[2].tv = pVertices[1].tv;
					pVertices[3].tu = pVertices[0].tu + 1;
					pVertices[3].tv = pVertices[0].tv;		
					
					m_pWaterObjectList[i].vertices->Unlock();
				}
			}
		}
	}

	// base texture	
	_EncodeData(szTextureFilename[0], _ENCODEKEY);	
	_EncodeData(szTextureFilename[1], _ENCODEKEY);	
	_EncodeData(szTextureFilename[2], _ENCODEKEY);	
	_EncodeData(szTextureFilename[3], _ENCODEKEY);

	// detail texture	
	_EncodeData(szTextureFilename[4], _ENCODEKEY);	
	_EncodeData(szTextureFilename[5], _ENCODEKEY);	
	_EncodeData(szTextureFilename[6], _ENCODEKEY);	
	_EncodeData(szTextureFilename[7], _ENCODEKEY);

	//load texture	
	SetWaterTextureIndex( DEF_WATERCLASS_SEA,    szTextureFilename[0]	   , szTextureFilename[4] );
	SetWaterTextureIndex( DEF_WATERCLASS_RIVER,  szTextureFilename[1]	   , szTextureFilename[5] );
	SetWaterTextureIndex( DEF_WATERCLASS_LAKE,   szTextureFilename[2]	   , szTextureFilename[6] );
	SetWaterTextureIndex( DEF_WATERCLASS_SWAMP,  szTextureFilename[3]	   , szTextureFilename[7] );	
		
	return TRUE;
}

// Set Texture
void CWater::SetTextures(int waterclass)
{	
	g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_WaterDiffuseTextureIndex[waterclass], 0 );		

	if(g_bCanDoMultitexture)
	{			
		g_TerrainTextureArchive.SetTexture_NoRenderStyle( m_WaterDetailTextureIndex[waterclass], 1 );
	}
	else  
	{		
		gpDev->SetTexture(1, NULL);
	}
}

void CWater::EnableAnimation(void)
{
	static FLOAT x = 0.0f;
	static FLOAT y = 0.0f;
	
	int			animationclass = 0;
	D3DXMATRIX	mat;
	FLOAT		uvVelocity = 0.f, lapVelocity = 0.f, lapHeightMax = 0.f;
	int			uvDirection = 0;

	if(!m_iSelectedTileList.empty())
	{
		animationclass = m_pWaterObjectList[m_iSelectedTileList.front()].animationclass;

		uvVelocity = m_pWaterObjectList[m_iSelectedTileList.front()].uvVelocity;
		lapVelocity = m_pWaterObjectList[m_iSelectedTileList.front()].lapVelocity;
		lapHeightMax = m_pWaterObjectList[m_iSelectedTileList.front()].lapHeightMax;
		uvDirection = m_pWaterObjectList[m_iSelectedTileList.front()].direction;
	}

	switch(animationclass)
	{
	case DEF_WATERANI_UVDIRECTION :
		D3DXMatrixIdentity(&mat);

		// animation direction 적용
		switch(uvDirection)
		{
		case DEF_UV_RIGHT :
			mat._31 = x;
			x += uvVelocity;
			break;
		case DEF_UV_LEFT :
			mat._31 = x;
			x -= uvVelocity;
			break;
		case DEF_UV_UP :
			mat._32 = y;
			y -= uvVelocity;
			break;
		case DEF_UV_DOWN :
			mat._32 = y;
			y += uvVelocity;
			break;
		case DEF_UV_LEFTUP :
			mat._31 = x;
			mat._32 = y;
			x += uvVelocity;
			y -= uvVelocity;
			break;
		case DEF_UV_RIGHTUP :
			mat._31 = x;
			mat._32 = y;
			x -= uvVelocity;
			y -= uvVelocity;
			break;
		case DEF_UV_LEFTDOWN :
			mat._31 = x;
			mat._32 = y;
			x += uvVelocity;
			y += uvVelocity;
			break;
		case DEF_UV_RIGHTDOWN :
			mat._31 = x;
			mat._32 = y;
			x -= uvVelocity;
			y += uvVelocity;
			break;
		}

		if(x >= 1.0f )
			x = 0.0f; 
		if(x <= -1.0f)
			x = 0.0f;
		if(y >= 1.0f)
			y = 0.0f;
		if(y <= -1.0f)
			y = 0.0f;

		gpDev->SetTransform(D3DTS_TEXTURE0, &mat);
		//gpDev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		//gpDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);

		break;

	case DEF_WATERANI_LAPPING :
		if(m_iLapDirection == DEF_LAP_CLIMB)	// 올라가던 방향
		{
			if(fabs(m_fHeightOffset) < lapHeightMax)
			{
				// 계속 올라간다
				m_fHeightOffset += lapVelocity;
			}
			else
			{
				// 방향 전환
				m_fHeightOffset -= lapVelocity;
				m_iLapDirection = DEF_LAP_DROP;
			}
		}
		else	// 내려가던 방향
		{
			if(fabs(m_fHeightOffset) < lapHeightMax)
			{
				// 계속 내려간다.
				m_fHeightOffset -= lapVelocity;
			}
			else
			{
				// 방향 전환
				m_fHeightOffset += lapVelocity;
				m_iLapDirection = DEF_LAP_CLIMB;
			}
		}

		// world matrix에 적용
		D3DXMatrixTranslation(&m_matWaterMatrix, 0.0f, m_fHeightOffset, 0.0f);
		gpDev->SetTransform(D3DTS_WORLD, &m_matWaterMatrix);

		break;

	default :
		break;
	}	
}


void CWater::DiableAnimation(void)
{
	D3DXMATRIX mat;

	// uv animation diable
	D3DXMatrixIdentity(&mat);
	gpDev->SetTransform(D3DTS_TEXTURE0, &mat);
	gpDev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	gpDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	// lap animation diable
	D3DXMatrixIdentity(&m_matWaterMatrix);
	gpDev->SetTransform(D3DTS_WORLD, &m_matWaterMatrix);

}

BOOL CWater::LoadShader( void )
{
	ReleaseShader();
	
	if( m_WaterRenderingMode != _XWATER_RM_VERTEXSHADER )
	{
		return TRUE;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create shader declaration...
	D3DVERTEXELEMENT9 dwWaterShaderVertexDecl[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // Position
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 }, // Tex coords
		D3DDECL_END()
	};
	
	HRESULT hr = gpDev->CreateVertexDeclaration( dwWaterShaderVertexDecl, &g_pWaterBumpShaderDeclation );
	
	// assemble shader	
	LPD3DXBUFFER pCode, pError = NULL;
	if (D3DXAssembleShader(g_WaterShaderVertexProgram, sizeof(g_WaterShaderVertexProgram) - 1, 0, NULL, 0, &pCode, &pError) == D3D_OK)
	{
		if (gpDev->CreateVertexShader((DWORD*)pCode->GetBufferPointer( ), &g_pWaterBumpVertexShader) != D3D_OK)
		{
			SAFE_RELEASE( pCode );
			_XFatalError( "Failed to create water bump vertex shader." );
			return FALSE;
		}		
	}
	else
	{
		_XFatalError( "Failed to assemble water bump vertex shader.\nThe error reported is [ %s ].\n", pError->GetBufferPointer( ));
		return FALSE;
	}	
	SAFE_RELEASE( pCode );
	SAFE_RELEASE( pError );
	
	return TRUE;
}

void CWater::ReleaseShader( void )
{
	SAFE_RELEASE( g_pWaterBumpShaderDeclation );
	SAFE_RELEASE( g_pWaterBumpVertexShader );
}

BOOL CWater::InitBumpMap( void )
{
	SAFE_RELEASE( g_pBumpMap );
	SAFE_RELEASE( g_pWaterEnvRenderSurface );
	SAFE_RELEASE( g_pWaterEnvRenderTexture );

	if( m_WaterRenderingMode != _XWATER_RM_VERTEXSHADER && m_WaterRenderingMode != _XWATER_RM_EMBM )
	{
		return TRUE;
	}

	LPDIRECT3DTEXTURE9 psBumpSrc = NULL;
    D3DSURFACE_DESC    d3dsd;
    D3DLOCKED_RECT     d3dlr;

	_XPackageArchive TextureArchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );

	if( !TextureArchive.OpenPackage( packagefilename ) ) 
		return FALSE;	
	
	int resourceindex = TextureArchive.FindResource( _T("Waterbump3.bmp") );
	if( resourceindex < 0 )
	{
		_XFatalError( "Resource not found in the texture archive [%s]", _T("Waterbump3.bmp") );
		TextureArchive.FinalizePackage();
		return FALSE;
	}
	
	int   filesize = TextureArchive.GetPackedFileSize(resourceindex);
	FILE* pFP = TextureArchive.GetPackedFile(resourceindex);
	
	LPBYTE pImageVFMemory = NULL;
	pImageVFMemory = new BYTE[ filesize ];
	if( !pImageVFMemory )
	{
		_XFatalError( "Can't allocate memory for virtual file" );
		TextureArchive.FinalizePackage();
		return FALSE;
	}
	
	if( fread( pImageVFMemory, filesize, 1, pFP ) < 1 )
	{
		_XFatalError( "Can't read virtual file data" );
		delete[] pImageVFMemory;
		TextureArchive.FinalizePackage();
		return FALSE;
	}
	
	TextureArchive.FinalizePackage();
	
	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
				D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
				D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
				D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
				0, NULL, NULL, &psBumpSrc ) ) )
	{
		_XFatalError( "Can't create texture from memory" );
		delete[] pImageVFMemory;		
		return FALSE;
	}
	
	delete[] pImageVFMemory;

    psBumpSrc->GetLevelDesc( 0, &d3dsd );
    // Create the bumpmap's surface and texture objects
    if( FAILED( gpDev->CreateTexture( d3dsd.Width, d3dsd.Height, 1, 0, D3DFMT_V8U8, D3DPOOL_MANAGED, &g_pBumpMap, NULL ) ) )
    {
        return FALSE;
    }
	
    // Fill the bits of the new texture surface with bits from
    // a private format.
    psBumpSrc->LockRect( 0, &d3dlr, 0, 0 );
    DWORD dwSrcPitch = (DWORD)d3dlr.Pitch;
    BYTE* pSrcTopRow = (BYTE*)d3dlr.pBits;
    BYTE* pSrcCurRow = pSrcTopRow;
    BYTE* pSrcBotRow = pSrcTopRow + (dwSrcPitch * (d3dsd.Height - 1) );
	
    g_pBumpMap->LockRect( 0, &d3dlr, 0, 0 );
    DWORD dwDstPitch = (DWORD)d3dlr.Pitch;
    BYTE* pDstTopRow = (BYTE*)d3dlr.pBits;
    BYTE* pDstCurRow = pDstTopRow;
    BYTE* pDstBotRow = pDstTopRow + (dwDstPitch * (d3dsd.Height - 1) );
	
    for( DWORD y=0; y<d3dsd.Height; y++ )
    {
        BYTE* pSrcB0; // addr of current pixel
        BYTE* pSrcB1; // addr of pixel below current pixel, wrapping to top if necessary
        BYTE* pSrcB2; // addr of pixel above current pixel, wrapping to bottom if necessary
        BYTE* pDstT;  // addr of dest pixel;
		
        pSrcB0 = pSrcCurRow;
		
        if( y == d3dsd.Height - 1)
            pSrcB1 = pSrcTopRow;
        else
            pSrcB1 = pSrcCurRow + dwSrcPitch;
		
        if( y == 0 )
            pSrcB2 = pSrcBotRow;
        else
            pSrcB2 = pSrcCurRow - dwSrcPitch;
		
        pDstT = pDstCurRow;
		
        for( DWORD x=0; x<d3dsd.Width; x++ )
        {
            LONG v00; // Current pixel
            LONG v01; // Pixel to the right of current pixel, wrapping to left edge if necessary
            LONG vM1; // Pixel to the left of current pixel, wrapping to right edge if necessary
            LONG v10; // Pixel one line below.
            LONG v1M; // Pixel one line above.
			
            v00 = *(pSrcB0+0);
            
            if( x == d3dsd.Width - 1 )
                v01 = *(pSrcCurRow);
            else
                v01 = *(pSrcB0+4);
            
            if( x == 0 )
                vM1 = *(pSrcCurRow + (4 * (d3dsd.Width - 1)));
            else
                vM1 = *(pSrcB0-4);
            v10 = *(pSrcB1+0);
            v1M = *(pSrcB2+0);
			
            LONG iDu = (vM1-v01); // The delta-u bump value
            LONG iDv = (v1M-v10); // The delta-v bump value
			
			
            *pDstT++ = (BYTE)(iDu / 2);
            *pDstT++ = (BYTE)(iDv / 2);
			
			
            // Move one pixel to the right (src is 32-bpp)
            pSrcB0+=4;
            pSrcB1+=4;
            pSrcB2+=4;
        }
		
        // Move to the next line
        pSrcCurRow += dwSrcPitch;
        pDstCurRow += dwDstPitch;
    }
	
    g_pBumpMap->UnlockRect(0);
    psBumpSrc->UnlockRect(0);

	SAFE_RELEASE(psBumpSrc);

	if( FAILED( gpDev->CreateTexture(	128,128, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pWaterEnvRenderTexture, NULL) ) )
	{
		_XFatalError( "Can't create water reflection rendering buffer" );
		return FALSE;
	}
		
	g_pWaterEnvRenderTexture->GetSurfaceLevel(0,&g_pWaterEnvRenderSurface);	

	return TRUE;	
}
void CWater::SetEMBMStates( void )
{
	if( m_WaterRenderingMode == _XWATER_RM_VERTEXSHADER )
	{
		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 16 );		
		gpDev->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

		D3DXMATRIXA16 matCamera, matFinal;
        D3DXMatrixMultiply( &matCamera, &g_LodTerrain.m_TerrainMatrix, &g_LodTerrain.m_3PCamera.mp_view_matrix );
        D3DXMatrixMultiply( &matFinal, &matCamera, &g_LodTerrain.m_3PCamera.mp_proj_matrix );
        D3DXMatrixTranspose(&matCamera, &matCamera);
        D3DXMatrixTranspose(&matFinal, &matFinal);
        /*matCamera(0, 0) *= 0.8f;
        matCamera(0, 1) *= 0.8f;
        matCamera(0, 2) *= 0.8f;
        matCamera(0, 3) *= 0.8f;
        matCamera(1, 0) *= 0.8f;
        matCamera(1, 1) *= 0.8f;
        matCamera(1, 2) *= 0.8f;
        matCamera(1, 3) *= 0.8f;*/

		gpDev->SetVertexShaderConstantF(0, (float*)&matCamera, 3);
        gpDev->SetVertexShaderConstantF(3, (float*)&matFinal, 4);        
        static FLOAT shaderconstantdata[4] = {0.5f, -0.5f, 0, 0};
        gpDev->SetVertexShaderConstantF(8, (float*)&shaderconstantdata, 1);
	}
	else if( m_WaterRenderingMode == _XWATER_RM_EMBM )
	{
		g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 15 );
	}

	FLOAT r = 0.04f;

	FLOAT m_fTime = DXUtil_Timer( TIMER_GETAPPTIME );

    g_matWaterBumpMat._11 =  r * cosf( m_fTime* 9.0f );
    g_matWaterBumpMat._12 = -r * sinf( m_fTime* 9.0f );
    g_matWaterBumpMat._21 =  r * sinf( m_fTime* 9.0f );
    g_matWaterBumpMat._22 =  r * cosf( m_fTime* 9.0f );	

    // Set up texture stage 0's states for the bumpmap
    gpDev->SetTexture( 0, g_pBumpMap );
	
    gpDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT00,   FtoDW( g_matWaterBumpMat._11 ) );
    gpDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT01,   FtoDW( g_matWaterBumpMat._12 ) );
    gpDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT10,   FtoDW( g_matWaterBumpMat._21 ) );
    gpDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT11,   FtoDW( g_matWaterBumpMat._22 ) );
    gpDev->SetTextureStageState( 0, D3DTSS_BUMPENVLSCALE,  FtoDW(0.8f) );
    gpDev->SetTextureStageState( 0, D3DTSS_BUMPENVLOFFSET, FtoDW(0.0f) );
    
	gpDev->SetTexture( 1, g_pWaterEnvRenderTexture );
	//gpDev->SetTexture( 1, g_PostProgressManager.m_pOrgSceneTexture );
}

void CWater::SetClipPlane( D3DXMATRIXA16& matView, D3DXMATRIXA16& matProj )
{
	D3DXMATRIXA16 matWorldSaved;
    D3DXMATRIXA16 matReflectInMirror;
	
	m_ReflectClipPlane.a = 0.0f;
	m_ReflectClipPlane.b = -1.0f;
	m_ReflectClipPlane.c = 0.0f;
	m_ReflectClipPlane.d = m_WaterReflectionClipPlaneHeight;
	
	/*
    if(m_bUseClipPlanes) // if( g_d3dCaps.MaxUserClipPlanes > 0 )	
    {
		gpDev->SetClipPlane( 0, m_ReflectClipPlane);
        gpDev->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
        gpDev->SetTransform( D3DTS_PROJECTION, &matProj);
    }
    else */
    {
        SetClipProjectionMatrix( matView, matProj );
    }
}

void CWater::SetClipProjectionMatrix( D3DXMATRIXA16& matView, D3DXMATRIXA16& matProj )
{
	D3DXMATRIXA16 matClipProj;	
	D3DXMATRIXA16 WorldToProjection;
	
	WorldToProjection = matView * matProj;
	
	// m_ReflectClipPlane is plane definition (world space)
	D3DXPlaneNormalize(&m_ReflectClipPlane, &m_ReflectClipPlane);
	
	D3DXMatrixInverse(&WorldToProjection, NULL, &WorldToProjection);
	D3DXMatrixTranspose(&WorldToProjection, &WorldToProjection);
	
	
	D3DXVECTOR4 clipPlane(m_ReflectClipPlane.a, m_ReflectClipPlane.b, m_ReflectClipPlane.c, m_ReflectClipPlane.d);
	D3DXVECTOR4 projClipPlane;
	
	// transform clip plane into projection space
	D3DXVec4Transform(&projClipPlane, &clipPlane, &WorldToProjection);
	D3DXMatrixIdentity(&matClipProj);
	
	
	if (projClipPlane.w == 0)  // or less than a really small value
	{
		// plane is perpendicular to the near plane
		gpDev->SetTransform( D3DTS_PROJECTION, &matProj);
		return;
	}
	
	if (projClipPlane.w > 0)
	{
		// flip plane to point away from eye
		D3DXVECTOR4 clipPlane(-m_ReflectClipPlane.a, -m_ReflectClipPlane.b, -m_ReflectClipPlane.c, -m_ReflectClipPlane.d);		
		// transform clip plane into projection space
		D3DXVec4Transform(&projClipPlane, &clipPlane, &WorldToProjection);
		
	}
	
	D3DXVec4Normalize(&projClipPlane, &projClipPlane);  //  bug fix by David Whatley
	
	// put projection space clip plane in Z column
	matClipProj(0, 2) = projClipPlane.x;
	matClipProj(1, 2) = projClipPlane.y;
	matClipProj(2, 2) = projClipPlane.z;
	matClipProj(3, 2) = projClipPlane.w;
	
	// multiply into projection matrix
	D3DXMATRIXA16 projClipMatrix = matProj * matClipProj;
	
	gpDev->SetTransform( D3DTS_PROJECTION, &projClipMatrix);
}

void CWater::PreDrawEnvironment(void)
{
	LPDIRECT3DSURFACE9	prevrenderingtarget = NULL;
	
	if( SUCCEEDED( gpDev->GetRenderTarget( 0, &prevrenderingtarget ) ) )
	{
		gpDev->SetRenderTarget(0,g_pWaterEnvRenderSurface);
		
		gpDev->BeginScene();
		gpDev->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_EnvironmentManager.m_FogColor, 1.0f, 0L );

		g_planeWater = D3DXPLANE( 0.0f, 1.0f, 0.0f, -m_WaterReflectionClipPlaneHeight );

		D3DXMATRIXA16 matReflectedView;
		D3DXMatrixReflect( &matReflectedView, &g_planeWater);		
		D3DXMatrixMultiply( &matReflectedView, &matReflectedView, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
		gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());
		gpDev->SetTransform( D3DTS_VIEW, &matReflectedView );
		
		D3DXMATRIX localprojectmatrix;
		D3DXMatrixPerspectiveFovLH(&localprojectmatrix, _X_RAD(g_LodTerrain.m_3PCamera.mp_fFov), 
								   g_LodTerrain.m_3PCamera.mp_fAspect, g_LodTerrain.m_3PCamera.mp_fNearPlane, 600.0f );

		SetClipPlane( matReflectedView, (D3DXMATRIXA16)localprojectmatrix );
		
		g_LodTerrain.DrawReplectTerrain();
				
		if( m_UseObjectReflection )
		{
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

			g_LodTerrain.RenderWaterAboveObject();
					
			g_pLocalUser->m_ModelDescriptor.Render_Reflect(matReflectedView);
			gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
		}

		gpDev->SetRenderState( D3DRS_CLIPPLANEENABLE, 0);
		
		gpDev->EndScene();

		gpDev->SetRenderTarget( 0, prevrenderingtarget );
				
		SAFE_RELEASE( prevrenderingtarget );
	}
}

void CWater::Draw_UseShader(void)
{
	SetEMBMStates();

	if( m_WaterRenderingMode == _XWATER_RM_VERTEXSHADER )
	{
		gpDev->SetVertexShader( g_pWaterBumpVertexShader );
        gpDev->SetVertexDeclaration( g_pWaterBumpShaderDeclation );
	}
	else //if( m_WaterRenderingMode == _XWATER_RM_EMBM )
	{
		gpDev->SetFVF(_XWATERVERTEX_FVF);

		LPD3DXMATRIX pmatProj;
		D3DXMATRIX mat;
		D3DXMatrixIdentity( &mat );
		
		mat._11 = 0.5f; mat._12 = 0.0f; mat._13 = 0.0f;
		mat._21 = 0.0f; mat._22 =-0.5f; mat._23 = 0.0f;
		mat._31 = 0.5f; mat._32 = 0.5f; mat._33 = 1.0f;
		mat._41 = 0.0f; mat._42 = 0.1f; mat._43 = 0.0f;
				
		pmatProj = &g_LodTerrain.m_3PCamera.GetProjMatrix();
		mat._11 *= pmatProj->_11;
		mat._22 *= pmatProj->_22;
		mat._31 *= pmatProj->_33;
		mat._32 *= pmatProj->_33;
		mat._33 *= pmatProj->_33;
		
		gpDev->SetTransform( D3DTS_TEXTURE1, &mat);
	}

	if (SUCCEEDED(gpDev->SetIndices( g_WaterObjectIndices )))
	{
		//EnableAnimation();
		for(int i = 0 ; i < m_iSeaCount ; i++)
		{
			if(m_pWaterObjectList[m_iSeaIndexArray[i]].infrustum)
			{
				if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iSeaIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
				{						
					gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
				}					
			}
		}
		
		for(i = 0 ; i < m_iRiverCount ; i++)
		{
			if(m_pWaterObjectList[m_iRiverIndexArray[i]].infrustum)
			{
				if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iRiverIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
				{						
					gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
				}					
			}					
		}
		
		for(i = 0 ; i < m_iLakeCount ; i++)
		{
			if(m_pWaterObjectList[m_iLakeIndexArray[i]].infrustum)
			{
				if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iLakeIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
				{						
					gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
				}					
			}					
		}

		for(i = 0 ; i < m_iSwampCount ; i++)
		{
			if(m_pWaterObjectList[m_iSwampIndexArray[i]].infrustum)
			{
				if (SUCCEEDED(gpDev->SetStreamSource(0,m_pWaterObjectList[m_iSwampIndexArray[i]].vertices, 0, sizeof(WATER_VERTEX))))
				{						
					gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
				}
			}
		}
	}

	gpDev->SetVertexShader( NULL );

	//DiableAnimation();
}

bool CWater::InitDeviceObject( void )
{
	if( !LoadShader() ) return false;

	InitBumpMap();
	if( !CreateWaterIndexBuffer() ) return false;

	return true;
}

void CWater::ReleaseDeviceObject( void )
{
	ReleaseShader();
	ReleaseWaterIndexBuffer();
	SAFE_RELEASE( g_pBumpMap );
	SAFE_RELEASE( g_pWaterEnvRenderSurface );
	SAFE_RELEASE( g_pWaterEnvRenderTexture );
}