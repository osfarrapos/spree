// XAnimMeshContainer.cpp: implementation of the _XAnimMeshContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <direct.h>
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XVSAConstants.h"
#include "XAnimMeshContainer.h"
#include "XBipedController.h"

#include "LoadThread.h"
#include "XTextureController.h"

extern void GetFileNameFromPathString( TCHAR *value, TCHAR *line );
extern BOOL g_SetTextureFailed; 

#define _XDEF_USESLERPANIMATION				
#define _XDEF_SLERPANIMATIONTIME			5.0f
#define _XDEF_ANIMATIONVERTEXSHADERCOUNT	11

#define _XDEF_INTERFACEMODELALPHAREF		5

#ifdef _XTAIWANESE
	const FLOAT _XDEF_CHARACTERDISTANCE_LODLEVEL1 =	12.0f;
	const FLOAT _XDEF_CHARACTERDISTANCE_LODLEVEL2 =	18.0f;

	//const FLOAT _XDEF_CHARACTERDISTANCE_LODLEVEL1 =	8.0f;
	//const FLOAT _XDEF_CHARACTERDISTANCE_LODLEVEL2 =	13.0f;
#else
	const FLOAT _XDEF_CHARACTERDISTANCE_LODLEVEL1 =	8.0f;
	const FLOAT _XDEF_CHARACTERDISTANCE_LODLEVEL2 =	13.0f;
#endif

LPDIRECT3DVERTEXSHADER9			g_pAnimationShaderPalette[_XDEF_ANIMATIONVERTEXSHADERCOUNT];
LPDIRECT3DVERTEXDECLARATION9	g_pAnimationShaderDeclation;

BOOL _XAnimMeshAbstractClass::m_UseVertexShaderAnimation = TRUE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common global functions

int FindIDX(WORD* pWord, int nTot, WORD wData)
{
	for( INT i = 0; i< nTot; i++)
	{
		if( pWord[i] == wData )	return i;
	}
	return 0;
}

int SortAndBoneIDX(LPWORD wpOut, LPWORD pIDX, int nCount )
{
	int nTotal = 1;
	wpOut[0] = pIDX[0];
	BOOL bIsFind = FALSE;
	
	for( int n = 1; n< nCount; n++)
	{
		WORD wIdx = pIDX[n];
		bIsFind = FALSE;
		
		BOOL bIsOut = FALSE;
		
		for( int i=0; i<nTotal; i++)
		{
			
			if(wIdx == wpOut[i])	continue;
			
			for( int ii =0; ii<nTotal; ii++)
			{
				if( wpOut[ii] == wIdx)
				{
					
					bIsOut = TRUE;
					break;
				}
			}
			if(bIsOut)
			{
				i = nTotal;
				break;
			}
			
			wpOut[nTotal] = wIdx;
			nTotal++;
			bIsFind = TRUE;
			i = nTotal; //루프를 빠져 나가기 위해서
			
		}
	}
	
	return nTotal;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// _XAnimMeshAbstractClass

_XAnimMeshAbstractClass* _XAnimMeshAbstractClass::CreateAnimationMesh( void )
{
	if( m_UseVertexShaderAnimation )
	{
		return new _XVSAnimMeshContainer;
	}
	else
	{
		return new _XNSAnimMeshContainer;
	}

	return NULL;
}

void _XAnimMeshAbstractClass::DestroyModel( void )
{
#ifdef _XDEF_MODELTHREADLOADING
	if( m_ThreadLoad ) return;
	//if( !m_Initialized ) return;
#endif	

	SAFE_DELETE_ARRAY(m_Node);

	if( m_pLODObjectList )
	{
		for( int i = 0; i < m_pLODObjectCount; i++ )
		{
			SAFE_DELETE( m_pLODObjectList[i] );
		}

		SAFE_DELETE_ARRAY(m_pLODObjectList);
	}	
	
	m_NodeCnt			=	0;	
	m_pLODObjectCount	=   0;
	m_Initialized		=	false;
}

#define _XDEF_MESHDESTROYTIME	60000	// 1분이상 사용하지 않으면 파괴.

void _XAnimMeshAbstractClass::CheckLastUsedTime( void )
{
	if( m_LastUsedTime == 0 )
		m_LastUsedTime = g_LocalSystemTime;

	if( g_LocalSystemTime - m_LastUsedTime > _XDEF_MESHDESTROYTIME )
	{
		if( !m_ThreadLoad ) // 백그라운드 로딩중이때는 파괴 안함.
			DestroyModel();
	}
}

void _XAnimMeshAbstractClass::CreateLODObject( int lodobjectcount )
{	
	if( lodobjectcount <= 0 ) return;
	
	if( m_pLODObjectList )
	{
		if( lodobjectcount != m_pLODObjectCount )
		{
			for( int i = 0; i < m_pLODObjectCount; i++ )
			{
				SAFE_DELETE( m_pLODObjectList[i] );
			}

			SAFE_DELETE_ARRAY(m_pLODObjectList);

			m_pLODObjectCount = lodobjectcount;
			m_pLODObjectList = new _XAnimMeshAbstractClass*[m_pLODObjectCount];
			for( i = 0; i < m_pLODObjectCount; i++ )
			{
				m_pLODObjectList[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
				m_pLODObjectList[i]->SetTextureArchive( m_pTextureManager );
			}
		}
	}
	else
	{
		m_pLODObjectCount = lodobjectcount;
		m_pLODObjectList = new _XAnimMeshAbstractClass*[m_pLODObjectCount];
		for( int i = 0; i < m_pLODObjectCount; i++ )
		{
			m_pLODObjectList[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
			m_pLODObjectList[i]->SetTextureArchive( m_pTextureManager );
		}
	}	
}

BOOL _XAnimMeshAbstractClass::CreateNode(int nodecount)
{
	SAFE_DELETE_ARRAY( m_Node );			
	m_NodeCnt			= 0;
	
	m_Node = new _XM_MESHOBJECT[nodecount];
	if( !m_Node )
	{
		_XFatalError( "CreateNode<%d> memory allocation", nodecount );
		return FALSE;
	}	
	
	m_NodeCnt = nodecount;
	
	return TRUE;
}

void _XAnimMeshAbstractClass::DeleteNode()
{	
	SAFE_DELETE_ARRAY( m_Node );			
	m_NodeCnt		   = 0;	
}

_LPXM_MESHOBJECT _XAnimMeshAbstractClass::GetNode(int index )
{ 	
	if(m_Node == NULL || m_NodeCnt == 0 || index < 0) return NULL;
	return &m_Node[ index ];
}

extern bool ClipBoxRay (D3DXVECTOR3& rkOrigin, D3DXVECTOR3& rkDirection, float afExtent[3], float& rfT0, float& rfT1);

BOOL _XAnimMeshAbstractClass::InterOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, D3DXVECTOR3& Offset, D3DXVECTOR3* pPickedPoint )
{
	// convert ray to box coordinates
    D3DXVECTOR3 diff = mouserayorigin - ( OBB_Center + Offset );
    D3DXVECTOR3 Origin(D3DXVec3Dot(&diff,&OBB_Axis[0]), D3DXVec3Dot(&diff,&OBB_Axis[1]), D3DXVec3Dot(&diff,&OBB_Axis[2]) );
    D3DXVECTOR3 Direction( D3DXVec3Dot(&mouseraydirection,&OBB_Axis[0]), D3DXVec3Dot(&mouseraydirection,&OBB_Axis[1]), D3DXVec3Dot(&mouseraydirection,&OBB_Axis[2]) );
	
    float fT0 = 0.0f, fT1 = 1.0f;
    BOOL bIntersects = ClipBoxRay(Origin,Direction,OBB_Extent,fT0,fT1);
	
	if ( bIntersects && pPickedPoint )
    {
        if ( fT0 > 0.0f )
        {
			*pPickedPoint = mouserayorigin + fT0*mouseraydirection;			
        }
        else  // fT0 == 0
        {
			
            *pPickedPoint = mouserayorigin + fT1*mouseraydirection;
        }
    }
	
    return bIntersects;
}

BOOL _XAnimMeshAbstractClass::InterOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, D3DXVECTOR3& Offset, FLOAT* pRotateAngle, D3DXVECTOR3* pPickedPoint )
{
	// convert ray to box coordinates
		
	float fT0 = 0.0f, fT1 = 1.0f;
	BOOL bIntersects;

	if( pRotateAngle )
	{
		D3DXVECTOR3 NewAxis[3], NewOffset;
		D3DXMATRIX  rotmat;

		D3DXMatrixRotationY( &rotmat, *pRotateAngle );
			
		D3DXMATRIX mat;		
		D3DXMatrixIdentity( &mat );		
		mat._11 = OBB_Axis[0].x; mat._21 = OBB_Axis[0].y; mat._31 = OBB_Axis[0].z;
		mat._12 = OBB_Axis[1].x; mat._22 = OBB_Axis[1].y; mat._32 = OBB_Axis[1].z;
		mat._13 = OBB_Axis[2].x; mat._23 = OBB_Axis[2].y; mat._33 = OBB_Axis[2].z;
		mat._41 = OBB_Center.x;  mat._42 = OBB_Center.y;  mat._43 = OBB_Center.z;
		D3DXMatrixMultiply( &mat, &mat, &rotmat );
		
		NewAxis[0].x = mat._11;
		NewAxis[0].y = mat._21;
		NewAxis[0].z = mat._31;
		NewAxis[1].x = mat._12;
		NewAxis[1].y = mat._22;
		NewAxis[1].z = mat._32;
		NewAxis[2].x = mat._13;
		NewAxis[2].y = mat._23;
		NewAxis[2].z = mat._33;
		NewOffset.x = mat._41 + Offset.x;
		NewOffset.y = mat._42 + Offset.y;
		NewOffset.z = mat._43 + Offset.z;

		D3DXVECTOR3 diff = mouserayorigin - NewOffset;
		D3DXVECTOR3 Origin(D3DXVec3Dot(&diff,&NewAxis[0]), D3DXVec3Dot(&diff,&NewAxis[1]), D3DXVec3Dot(&diff,&NewAxis[2]) );
		D3DXVECTOR3 Direction( D3DXVec3Dot(&mouseraydirection,&NewAxis[0]), D3DXVec3Dot(&mouseraydirection,&NewAxis[1]), D3DXVec3Dot(&mouseraydirection,&NewAxis[2]) );
		bIntersects = ClipBoxRay(Origin,Direction,OBB_Extent,fT0,fT1);
	}
	else
	{
		D3DXVECTOR3 diff = mouserayorigin - ( OBB_Center + Offset );
		D3DXVECTOR3 Origin(D3DXVec3Dot(&diff,&OBB_Axis[0]), D3DXVec3Dot(&diff,&OBB_Axis[1]), D3DXVec3Dot(&diff,&OBB_Axis[2]) );
		D3DXVECTOR3 Direction( D3DXVec3Dot(&mouseraydirection,&OBB_Axis[0]), D3DXVec3Dot(&mouseraydirection,&OBB_Axis[1]), D3DXVec3Dot(&mouseraydirection,&OBB_Axis[2]) );
		bIntersects = ClipBoxRay(Origin,Direction,OBB_Extent,fT0,fT1);
	}
	
	if ( bIntersects && pPickedPoint )
    {
        if ( fT0 > 0.0f )
        {
			*pPickedPoint = mouserayorigin + fT0*mouseraydirection;			
        }
        else  // fT0 == 0
        {
			
            *pPickedPoint = mouserayorigin + fT1*mouseraydirection;
        }
    }
	
    return bIntersects;
}

BOOL _XAnimMeshAbstractClass::InterWeaponOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, D3DXVECTOR3 Offset, 
										  D3DXVECTOR3 &Center,D3DXVECTOR3 Axis[3],float Extent[3])
{
	// convert ray to box coordinates
    D3DXVECTOR3 diff = mouserayorigin - ( Center + Offset );
    D3DXVECTOR3 Origin(D3DXVec3Dot(&diff,&Axis[0]), D3DXVec3Dot(&diff,&Axis[1]), D3DXVec3Dot(&diff,&Axis[2]) );
    D3DXVECTOR3 Direction( D3DXVec3Dot(&mouseraydirection,&Axis[0]), D3DXVec3Dot(&mouseraydirection,&Axis[1]), D3DXVec3Dot(&mouseraydirection,&Axis[2]) );
	
    float fT0 = 0.0f, fT1 = 1.0f;
    return ClipBoxRay(Origin,Direction,Extent,fT0,fT1);
}

void _XAnimMeshAbstractClass::SortByAlphaBlendAttrib( BOOL usemanager )
{
	/*
	if(m_Node == NULL || m_NodeCnt < 2 ) return;

	if( usemanager )
	{
		for( int i = 0; i < m_NodeCnt; i++ )
		{
			m_Node[i].bBakemap = m_pTextureManager->IsAlphaBlending( m_Node[i].nTextureId );
		}
	}
	else
	{
		for( int i = 0; i < m_NodeCnt; i++ )
		{
			m_Node[i].bBakemap = m_pTextureController->IsAlphaBlending( m_Node[i].nTextureId );
		}
	}

	_XM_MESHOBJECT* pTNode = NULL;

	for( int i = 0; i < m_NodeCnt; i++ )
	{
		for( int j = 0; j < m_NodeCnt-1; j++ )
		{
			if( m_Node[j].bBakemap )
			{
				if( m_Node[j].bBakemap > m_Node[j+1].bBakemap )
				{
					
				}
			}
		}
	}
	
	for( i = 0; i < m_NodeCnt; i++ )
	{
		m_Node[i].bBakemap = FALSE;
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// _XVSAnimMeshContainer

_XVSAnimMeshContainer::_XVSAnimMeshContainer()
{
	m_NodeCnt				= 0;
	m_Node					= NULL;			
	m_pTextureManager		= NULL;
	m_pTextureController	= NULL;
	m_ObjectRadius			= 0;
	m_pLODObjectCount		= 0;
	m_pLODObjectList		= NULL;
	
	m_Initialized			= false;
	m_ThreadLoad			= false;
	m_LastUsedTime			= 0L;

	m_pExtraData			= NULL;
}

_XVSAnimMeshContainer::~_XVSAnimMeshContainer()
{
	DestroyModel();
}

BOOL _XVSAnimMeshContainer::InitializeVertexAnimationShader( void )
{
	if( !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation ) return TRUE;

	ReleaseVertexAnimationShader();

	
	//D3DVERTEXELEMENT9 decl[] =
	//{
	//	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	//	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },   //normal
	//	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //Normal map texcoord
	//	{ 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 }, //Skinnig에 사용되는 Index와 weight 정보		
	//	/*{ 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 }, //Skinnig에 사용되는 Index와 weight 정보
	//	{ 0, 60, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 }, //Skinnig에 사용되는 Index와 weight 정보
	//	{ 0, 72, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 }, //Skinnig에 사용되는 Index와 weight 정보*/
	//	D3DDECL_END()
	//};

	D3DVERTEXELEMENT9   decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF( D3DFVF_XMESH_PSVERTEX, decl );
    	
	if( FAILED( gpDev->CreateVertexDeclaration( decl, &g_pAnimationShaderDeclation ) ) )
	{
		_XFatalError( "Can't create animation vertex shader declaration" );
		return FALSE;
	}
	
	TCHAR shaderfilename[_XDEF_ANIMATIONVERTEXSHADERCOUNT][32] = 
	{
		_T( "PaletteSkin.vso" ),_T( "model.vso" ),_T( "Normal.vso" ),_T( "ModelShadow.vso" ),
		_T( "PaletteSkinShadow.vso" ),_T( "Toon.vso" ),_T( "SilhouetteEdge.vso" ),
		_T( "ToonPaletteSkin.vso" ),_T( "SkinEdge.vso" ),_T( "ToonRigidAni.vso" ),_T( "RigidAniEdge.vso" )
	};
	
	_XPackageArchive ShaderFileArchive;
	
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
#ifdef _XTESTSERVER			
	if( !ShaderFileArchive.OpenPackage( _T("tsShader.XP") ) ) return FALSE;
#else
	if( !ShaderFileArchive.OpenPackage( _T("Shader.XP") ) ) return FALSE;
#endif

	for( int i = 0; i < _XDEF_ANIMATIONVERTEXSHADERCOUNT; i++ )
	{
		int archiveindex = ShaderFileArchive.FindResource( shaderfilename[i] );

		if( archiveindex < 0 )
		{
			_XFatalError("Could not find %s shader file", shaderfilename[i]);
			ShaderFileArchive.FinalizePackage();
			return FALSE;
		}
		
		LPDWORD pShader = (LPDWORD)ShaderFileArchive.LoadPackedFileToMemory( archiveindex );
		
		if( !pShader)
		{
			_XFatalError("Failed to allocate memory to load %s shader file [%d]", shaderfilename[i]);
			ShaderFileArchive.FinalizePackage();
			return FALSE;
		}
		
		if( FAILED( gpDev->CreateVertexShader( pShader, &g_pAnimationShaderPalette[i] ) ) )
		{
			_XFatalError("Could not create animation vertex shader %s", shaderfilename[i]);			
			ShaderFileArchive.FinalizePackage();
			return FALSE;
		}

		delete[] pShader;
	}

	ShaderFileArchive.FinalizePackage();
	return TRUE;
}


void _XVSAnimMeshContainer::ReleaseVertexAnimationShader( void )
{
	for( int i = 0 ; i < _XDEF_ANIMATIONVERTEXSHADERCOUNT; i++)
	{
		SAFE_RELEASE( g_pAnimationShaderPalette[i] );
	}
	SAFE_RELEASE( g_pAnimationShaderDeclation );
}

// by mahwang
BOOL _XVSAnimMeshContainer::InitializeSecondBufferFormTM( _LPXM_KEYFRAME lpCurKeyframe)
{
#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
		{
			if( !g_LoadMeshThread.LoadMeshContainer_BlockMode( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation ) )
			{
				return FALSE;
			}
		}		
	}
#endif

	_LPXM_MESHOBJECT	lpCh;
	D3DXMATRIX			d3dMatrix, mat;        
	_XMESH_PSVERTEX		d3dxTmpVec;
	_XMESH_PSVERTEX		d3dxTmpVec2;

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{			
		lpCh = GetNode(objindex );

		if(lpCh->nVertexCnt != 0)
		{
			D3DXMatrixInverse(&d3dMatrix, NULL, &lpCurKeyframe->lpSubNode[objindex].matTM);

			_LPXMESH_PSVERTEX pVertices, plVertices;
			
			lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK );
			lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, D3DLOCK_NOSYSLOCK );			
			
			for( int nIdx = 0 ; nIdx < lpCh->nVertexCnt ; nIdx ++ )
			{
				d3dxTmpVec2 = d3dxTmpVec = pVertices[ nIdx ];
				
				// translate postition...
				d3dxTmpVec2.x	=	d3dxTmpVec.x * d3dMatrix._11 +
									d3dxTmpVec.y * d3dMatrix._21 +
									d3dxTmpVec.z * d3dMatrix._31 + d3dMatrix._41 ;
				d3dxTmpVec2.y	=	d3dxTmpVec.x * d3dMatrix._12 +
									d3dxTmpVec.y * d3dMatrix._22 +
									d3dxTmpVec.z * d3dMatrix._32 + d3dMatrix._42 ;
				d3dxTmpVec2.z	=	d3dxTmpVec.x * d3dMatrix._13 +
									d3dxTmpVec.y * d3dMatrix._23 +
									d3dxTmpVec.z * d3dMatrix._33 + d3dMatrix._43 ;

				// rotate normal vector...
				d3dxTmpVec2.nx	=	d3dxTmpVec.nx * d3dMatrix._11 +
									d3dxTmpVec.ny * d3dMatrix._21 +
									d3dxTmpVec.nz * d3dMatrix._31;

				d3dxTmpVec2.ny	=	d3dxTmpVec.nx * d3dMatrix._12 +
									d3dxTmpVec.ny * d3dMatrix._22 +
									d3dxTmpVec.nz * d3dMatrix._32;

				d3dxTmpVec2.nz	=	d3dxTmpVec.nx * d3dMatrix._13 +
									d3dxTmpVec.ny * d3dMatrix._23 +
									d3dxTmpVec.nz * d3dMatrix._33;
				
				plVertices[ nIdx ] = d3dxTmpVec2;
			}			

			lpCh->pVertex->Unlock();
			lpCh->pSecondVertexBuffer->Unlock();
		}
	}

	return TRUE;
}

BOOL _XVSAnimMeshContainer::Load( LPSTR szFilename, _XTextureManager* pTextureManager, BOOL createsecondbuffer )
{
	FILE*	Fileptr = NULL;
	
	if( ( Fileptr = fopen( szFilename , "rb" ) ) == NULL )
	{
		_XFatalError( "[%s] file open", szFilename );
		return FALSE;
	}

	if( !Load( Fileptr, pTextureManager, createsecondbuffer, szFilename ) )
	{
		fclose( Fileptr );
		return FALSE;
	}

	fclose( Fileptr );

	return TRUE;
}

extern LPBYTE g_TextureLoadBuffer;				// Blocking loading시에 Texture load buffer와 share
extern DWORD  g_TextureLoadBufferSize;			// Blocking loading시에 Texture load buffer와 share

extern LPBYTE g_MeshLoadBufferForThread;		// Background thread loading시에만  사용 
extern DWORD  g_MeshLoadBufferSizeForThread;	// Background thread loading시에만  사용 

BOOL _XVSAnimMeshContainer::Load( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer, LPSTR szFilename )
{
	if( m_Initialized ) 
	{
		return TRUE;
	}

	if( !Fileptr )
	{
		_XFatalError("Invalid file pointer [%s] file", (szFilename) ? szFilename : "this" );
		return FALSE;
	}

	DestroyModel();
	
	if( !pTextureManager )
	{
		return FALSE;
	}

	m_pTextureManager = pTextureManager;

	_XMESHHEADER   Header;	
	fread(&Header,	sizeof(_XMESHHEADER), 1, Fileptr);	
		
	if(Header.HeaderIndicator != _XDEF_X3DFILEINDICATOR )
	{
		_XFatalError("Not supported [%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE; 
	}

	if(Header.Fileversion != _XDEF_X3DFILEVERSION)
	{
		_XFatalError("Not supported [%s] file version [%f]",  (szFilename) ? szFilename : "this" , Header.Fileversion );		
		return FALSE; 		
	}

	int i,len;
	int NodeNum;	
	fread(&NodeNum,	sizeof(int), 1, Fileptr);	// read object count

	if( !CreateNode(NodeNum) ) return FALSE;		

	// Load model mesh data...	

	_LPXM_MESHOBJECT lpCh = NULL;
	TCHAR strName[128];

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		memset( strName, 0, 128 );
		if(len != 0) fread( strName, sizeof(TCHAR)*len, 1, Fileptr );

		// Load mesh object parent id...
		fread(&lpCh->nParentId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		
		if(len != 0) fseek(Fileptr, sizeof(TCHAR)*len, SEEK_CUR);

		// Load object matrix...
		fread(&lpCh->Matrix, sizeof(D3DXMATRIX), 1, Fileptr);
		lpCh->MatrixTM = lpCh->Matrix;

		// preprocess etc matrix...
		_XMeshMath_UnitMatrixFromMatrix(lpCh->Matrix);
		_XMeshMath_MatrixInvert( lpCh->Inverse, lpCh->Matrix);
		_XMeshMath_MatrixMultiply( lpCh->PosMatrixTM, lpCh->Inverse, lpCh->MatrixTM );

		D3DXMATRIX	mat;
		D3DXMatrixIdentity( &mat );

		_LPXM_MESHOBJECT lpParent;
		if(lpCh->nParentId != -1)
		{
			lpParent = GetNode(lpCh->nParentId);
			D3DXMatrixMultiply( &mat, &lpCh->Matrix, &lpParent->Inverse);
		}
		else mat = lpCh->Matrix;

		lpCh->LocalTM = mat;
		_XMeshMath_SetIdentityMatrix(lpCh->LocalAniTM);


		// Load texture id & setting texture index
		TCHAR strTextureName[64];
		fread(&len, sizeof(int), 1, Fileptr);
		memset(strTextureName, 0, sizeof(TCHAR)*64);
		if(len != 0)
		{
			fread(strTextureName, len, 1, Fileptr);
			lpCh->nTextureId = m_pTextureManager->GetResourceIndex( strTextureName, g_TextureMipLevel_CharacterModel );
		}
		else
		{
			//_XFatalError( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			lpCh->nTextureId = -1;
		}

		if( strncmp(strName, "Bip", 3 ) != 0 ) // Skipping biped object
		{
			// Load vertex buffer...
			fread(&lpCh->nVertexCnt, sizeof(lpCh->nVertexCnt), 1, Fileptr);
			if(lpCh->nVertexCnt != 0)
			{
				SAFE_RELEASE( lpCh->pVertex );
				SAFE_RELEASE( lpCh->pSecondVertexBuffer );
				
				if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_PSVERTEX), D3DUSAGE_WRITEONLY, 
					D3DFVF_XMESH_PSVERTEX, D3DPOOL_MANAGED, &(lpCh->pVertex), NULL )) ) 
				{
					_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}				

				//_XFatalError( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );
				
				_LPXMESH_PSVERTEX pVertices;
				_LPXMESH_VERTEX	  pLoadBuffer = (_LPXMESH_VERTEX)g_TextureLoadBuffer;
				/*
				if( !pLoadBuffer )
				{
					_XFatalError("Can't create temporary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}
				*/
				
				if( fread(pLoadBuffer, sizeof(_XMESH_VERTEX) * lpCh->nVertexCnt, 1, Fileptr) < 1 )
				{
					_XFatalError("Can't load vertex data to temporary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}

				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{					
					for( int vertindex = 0; vertindex < lpCh->nVertexCnt; vertindex++  )
					{
						memcpy( pVertices+vertindex, pLoadBuffer+vertindex, sizeof(_XMESH_VERTEX) );
						/*pVertices[vertindex].x	= pLoadBuffer[vertindex].x;
						pVertices[vertindex].y	= pLoadBuffer[vertindex].y;
						pVertices[vertindex].z	= pLoadBuffer[vertindex].z;
						
						pVertices[vertindex].nx	= pLoadBuffer[vertindex].nx;
						pVertices[vertindex].ny	= pLoadBuffer[vertindex].ny;
						pVertices[vertindex].nz	= pLoadBuffer[vertindex].nz;
						
						pVertices[vertindex].tu = pLoadBuffer[vertindex].tu1;
						pVertices[vertindex].tv = pLoadBuffer[vertindex].tv1;*/
					}
					
					lpCh->pVertex->Unlock();
				}

				// Create second temporary buffer...
				if( createsecondbuffer )
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_PSVERTEX), D3DUSAGE_WRITEONLY, 
						D3DFVF_XMESH_PSVERTEX, D3DPOOL_MANAGED, &(lpCh->pSecondVertexBuffer), NULL )) )
					{
						_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
						return FALSE;
					}
					
					//_XFatalError( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
				}
			}

			// Load index buffer...
			fread(&lpCh->nIndexCnt, sizeof(lpCh->nIndexCnt), 1, Fileptr);    

			SAFE_RELEASE( lpCh->pIndex );
						
			if(lpCh->nIndexCnt != 0)
			{
				if( FAILED(gpDev->CreateIndexBuffer( lpCh->nIndexCnt*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
													 D3DPOOL_MANAGED, &(lpCh->pIndex), NULL )) )
				{
					_XFatalError("Create index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}					
				
				//_XFatalError( "Index buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pIndex );
				
				LPWORD pIndices;				
				if( SUCCEEDED( lpCh->pIndex->Lock( 0, lpCh->nIndexCnt*sizeof(WORD), (void**)&pIndices, D3DLOCK_NOSYSLOCK )) )
				{						
					fread(pIndices, sizeof(WORD) * lpCh->nIndexCnt, 1, Fileptr);
					lpCh->pIndex->Unlock();
				}
				else
				{
					_XFatalError("Lock index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}			
			}

			lpCh->nIndexCnt /= 3;

			// Read physique count
			fread(&lpCh->nKeyFactorCnt, sizeof(lpCh->nKeyFactorCnt), 1, Fileptr);

			if(lpCh->nKeyFactorCnt > 0)
			{
				lpCh->pKeyFactor = new _XM_KEYFACTOR[lpCh->nKeyFactorCnt];

				_LPXMESH_PSVERTEX	pVertices = NULL;
				
				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{
					for(int j = 0; j < lpCh->nKeyFactorCnt; j++)
					{
						// Read linked vertex count for Physique...
						fread(&lpCh->pKeyFactor[j].iFactorCnt, sizeof(int), 1, Fileptr);
						
						if(lpCh->pKeyFactor[j].iFactorCnt == 0)
						{					
							_XFatalError( "WARNING : Invalid physique informations\nvertex : %d, Physique number : %d",
																					  j, lpCh->pKeyFactor[j].iFactorCnt );
							lpCh->pVertex->Unlock();
							SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
							return FALSE;
						}
						else
						{
							lpCh->pKeyFactor[j].pFactorData = new _XM_FACTOR[lpCh->pKeyFactor[j].iFactorCnt];

							// Read physique informations
							for(int k = 0; k < lpCh->pKeyFactor[j].iFactorCnt; k++)
							{
								fread(&lpCh->pKeyFactor[j].pFactorData[k], sizeof(_XM_FACTOR), 1, Fileptr);
							}

							pVertices[j].weight0 = lpCh->pKeyFactor[j].pFactorData[0].fBlendFactor;
							pVertices[j].weight1 = 1.0f - pVertices[j].weight0;
						}
					}


					if( lpCh->nKeyFactorCnt > 0 )
					{
						fread(&lpCh->BoneIndexCnt, sizeof(int), 1, Fileptr);
						
						if( lpCh->BoneIndexCnt > 25 )
						{
							_XFatalError( "Overflowed bone index count : max vertex shader const count" );
							lpCh->pVertex->Unlock();
							SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
							return FALSE;
						}

						lpCh->pBoneIndexArray = new WORD[ lpCh->BoneIndexCnt ];
						fread(lpCh->pBoneIndexArray, sizeof(WORD)*lpCh->BoneIndexCnt , 1,Fileptr);

						//Shader를 사용하기 위해서 본을 분할 렌더 하기 위해 메쉬에 적용된 본만 따로
						//Shader에 본을 넘기기 위해서 그 본의 인덱스를 버텍스에 적용한다.

						for( int nVertexIdx = 0 ; nVertexIdx < lpCh->nVertexCnt ; nVertexIdx++ )
						{
							if( lpCh->pKeyFactor != NULL )
							{
								for(int nfactor = 0; nfactor < lpCh->pKeyFactor[nVertexIdx].iFactorCnt; nfactor++)
								{
									_XM_FACTOR pKeyFactor = lpCh->pKeyFactor[nVertexIdx].pFactorData[nfactor];
									
									if( nfactor == 0 )
									{
										pVertices[nVertexIdx].index0	 = CV_BONESTART + (float)( FindIDX(lpCh->pBoneIndexArray, lpCh->BoneIndexCnt, pKeyFactor.wKeyIdx )*3.0f ); 
										pVertices[nVertexIdx].index1	 = pVertices[nVertexIdx].index0;
									}
									else if(nfactor == 1)
									{
										pVertices[nVertexIdx].index1	 = CV_BONESTART + (float)(FindIDX(lpCh->pBoneIndexArray, lpCh->BoneIndexCnt, pKeyFactor.wKeyIdx )*3.0f );
									}									
									else
									{
										//_XLog( "EEEEE:%d", nfactor );
									}
								}
							}
						}
					}

					lpCh->pVertex->Unlock();
				}				

				SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
			}
		}		
	}

	OBB_Center = Header.OBB_Center;
    memcpy( OBB_Axis, Header.OBB_Axis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, Header.OBB_Extent, sizeof(FLOAT) * 3 );	
	
	//float fMagnitude  = _XFC_sqrt( (OBB_Center.x * OBB_Center.x) + (OBB_Center.y * OBB_Center.y) + (OBB_Center.z * OBB_Center.z) );
	float fMagnitude2 = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );	
	m_ObjectRadius = fMagnitude2;// fMagnitude + fMagnitude2;

	if(m_pExtraData)
	{
		_MOB_ATTRIBTABLE*pMobAttrTable = (_MOB_ATTRIBTABLE*)m_pExtraData;
		
		pMobAttrTable->xsize			= OBB_Extent[0];
		pMobAttrTable->ysize			= OBB_Extent[1];
		pMobAttrTable->collideradius	= m_ObjectRadius;
	}

#ifdef _XDWDEBUG
	CsuX3D_Box	obbbox;
	obbbox.Create( Header.OBB_Center, Header.OBB_Axis, Header.OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif
	
//	if( szFilename )
//		_XLog( "Loaded mesh file [%s]", szFilename );
	m_Initialized = true;

	return TRUE;
}

BOOL _XVSAnimMeshContainer::Load( FILE* Fileptr, _XTextureController* pTextureController, BOOL createsecondbuffer, LPSTR szFilename )
{
	if( m_Initialized ) 
	{
		return TRUE;
	}

	if( !Fileptr )
	{
		_XFatalError("Invalid file pointer [%s] file", (szFilename) ? szFilename : "this" );
		return FALSE;
	}

	DestroyModel();
	
	if( !pTextureController )
	{
		return FALSE;
	}

	m_pTextureController = pTextureController;

	_XMESHHEADER   Header;	
	fread(&Header,	sizeof(_XMESHHEADER), 1, Fileptr);	
		
	if(Header.HeaderIndicator != _XDEF_X3DFILEINDICATOR )
	{
		_XFatalError("Not supported [%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE; 
	}

	if(Header.Fileversion != _XDEF_X3DFILEVERSION)
	{
		_XFatalError("Not supported [%s] file version [%f]",  (szFilename) ? szFilename : "this" , Header.Fileversion );		
		return FALSE; 		
	}

	int i,len;
	int NodeNum;	
	fread(&NodeNum,	sizeof(int), 1, Fileptr);	// read object count

	if( !CreateNode(NodeNum) ) return FALSE;		

	// Load model mesh data...	

	_LPXM_MESHOBJECT lpCh = NULL;
	TCHAR strName[128];

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		memset( strName, 0, 128 );
		if(len != 0) fread( strName, sizeof(TCHAR)*len, 1, Fileptr );

		// Load mesh object parent id...
		fread(&lpCh->nParentId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		
		if(len != 0) fseek(Fileptr, sizeof(TCHAR)*len, SEEK_CUR);

		// Load object matrix...
		fread(&lpCh->Matrix, sizeof(D3DXMATRIX), 1, Fileptr);
		lpCh->MatrixTM = lpCh->Matrix;

		// preprocess etc matrix...
		_XMeshMath_UnitMatrixFromMatrix(lpCh->Matrix);
		_XMeshMath_MatrixInvert( lpCh->Inverse, lpCh->Matrix);
		_XMeshMath_MatrixMultiply( lpCh->PosMatrixTM, lpCh->Inverse, lpCh->MatrixTM );

		D3DXMATRIX	mat;
		D3DXMatrixIdentity( &mat );

		_LPXM_MESHOBJECT lpParent;
		if(lpCh->nParentId != -1)
		{
			lpParent = GetNode(lpCh->nParentId);
			D3DXMatrixMultiply( &mat, &lpCh->Matrix, &lpParent->Inverse);
		}
		else mat = lpCh->Matrix;

		lpCh->LocalTM = mat;
		_XMeshMath_SetIdentityMatrix(lpCh->LocalAniTM);


		// Load texture id & setting texture index
		TCHAR strTextureName[64];
		fread(&len, sizeof(int), 1, Fileptr);
		memset(strTextureName, 0, sizeof(TCHAR)*64);
		if(len != 0)
		{
			fread(strTextureName, len, 1, Fileptr);
			lpCh->nTextureId = m_pTextureController->CreateTexture( strTextureName, g_TextureMipLevel_CharacterModel );
		}
		else
		{
			//_XFatalError( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			_XLog( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			lpCh->nTextureId = -1;
		}

		if( strncmp(strName, "Bip", 3 ) != 0 ) // Skipping biped object
		{
			// Load vertex buffer...
			fread(&lpCh->nVertexCnt, sizeof(lpCh->nVertexCnt), 1, Fileptr);
			if(lpCh->nVertexCnt != 0)
			{
				lpCh->pVertex = NULL;
				
				if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_PSVERTEX), D3DUSAGE_WRITEONLY, 
					D3DFVF_XMESH_PSVERTEX, D3DPOOL_MANAGED, &(lpCh->pVertex), NULL )) ) 
				{
					_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}				
				
				//_XFatalError( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );
				
				_LPXMESH_PSVERTEX pVertices;				
				_LPXMESH_VERTEX	  pLoadBuffer = (_LPXMESH_VERTEX)g_TextureLoadBuffer;
				/*pLoadBuffer = new _XMESH_VERTEX[lpCh->nVertexCnt];
				if( !pLoadBuffer )
				{
					_XFatalError("Can't create temporary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}
				*/
				
				if( fread(pLoadBuffer, sizeof(_XMESH_VERTEX) * lpCh->nVertexCnt, 1, Fileptr) < 1 )
				{
					_XFatalError("Can't load vertex data to temporary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}

				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{					
					for( int vertindex = 0; vertindex < lpCh->nVertexCnt; vertindex++  )
					{
						memcpy( pVertices+vertindex, pLoadBuffer+vertindex, sizeof(_XMESH_VERTEX) );
						/*
						pVertices[vertindex].x	= pLoadBuffer[vertindex].x;
						pVertices[vertindex].y	= pLoadBuffer[vertindex].y;
						pVertices[vertindex].z	= pLoadBuffer[vertindex].z;
						
						pVertices[vertindex].nx	= pLoadBuffer[vertindex].nx;
						pVertices[vertindex].ny	= pLoadBuffer[vertindex].ny;
						pVertices[vertindex].nz	= pLoadBuffer[vertindex].nz;
						
						pVertices[vertindex].tu = pLoadBuffer[vertindex].tu1;
						pVertices[vertindex].tv = pLoadBuffer[vertindex].tv1;
						*/
					}
					
					lpCh->pVertex->Unlock();
				}

				// Create second temporary buffer...
				if( createsecondbuffer )
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_PSVERTEX), D3DUSAGE_WRITEONLY, 
						D3DFVF_XMESH_PSVERTEX, D3DPOOL_MANAGED, &(lpCh->pSecondVertexBuffer), NULL )) )
					{
						_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
						return FALSE;
					}
					//_XFatalError( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
				}
			}

			// Load index buffer...
			fread(&lpCh->nIndexCnt, sizeof(lpCh->nIndexCnt), 1, Fileptr);    
			
			if(lpCh->nIndexCnt != 0)
			{
				if( FAILED(gpDev->CreateIndexBuffer( lpCh->nIndexCnt*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
													 D3DPOOL_MANAGED, &(lpCh->pIndex), NULL )) )
				{
					_XFatalError("Create index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}					
				
				//_XFatalError( "Index buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pIndex );
				
				LPWORD pIndices;				
				if( SUCCEEDED( lpCh->pIndex->Lock( 0, lpCh->nIndexCnt*sizeof(WORD), (void**)&pIndices, D3DLOCK_NOSYSLOCK )) )
				{						
					fread(pIndices, sizeof(WORD) * lpCh->nIndexCnt, 1, Fileptr);
					lpCh->pIndex->Unlock();
				}
				else
				{
					_XFatalError("Lock index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}			
			}

			lpCh->nIndexCnt /= 3;

			// Read physique count
			fread(&lpCh->nKeyFactorCnt, sizeof(lpCh->nKeyFactorCnt), 1, Fileptr);

			if(lpCh->nKeyFactorCnt > 0)
			{
				lpCh->pKeyFactor = new _XM_KEYFACTOR[lpCh->nKeyFactorCnt];

				_LPXMESH_PSVERTEX	pVertices = NULL;
				
				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{
					for(int j = 0; j < lpCh->nKeyFactorCnt; j++)
					{
						// Read linked vertex count for Physique...
						fread(&lpCh->pKeyFactor[j].iFactorCnt, sizeof(int), 1, Fileptr);
						
						if(lpCh->pKeyFactor[j].iFactorCnt == 0)
						{					
							_XFatalError( "WARNING : Invalid physique informations\nvertex : %d, Physique number : %d",
																					  j, lpCh->pKeyFactor[j].iFactorCnt );
							lpCh->pVertex->Unlock();
							SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
							return FALSE;
						}
						else
						{
							lpCh->pKeyFactor[j].pFactorData = new _XM_FACTOR[lpCh->pKeyFactor[j].iFactorCnt];

							// Read physique informations
							for(int k = 0; k < lpCh->pKeyFactor[j].iFactorCnt; k++)
							{
								fread(&lpCh->pKeyFactor[j].pFactorData[k], sizeof(_XM_FACTOR), 1, Fileptr);
							}

							pVertices[j].weight0 = lpCh->pKeyFactor[j].pFactorData[0].fBlendFactor;
							pVertices[j].weight1 = 1.0f - pVertices[j].weight0;
						}
					}


					if( lpCh->nKeyFactorCnt > 0 )
					{
						fread(&lpCh->BoneIndexCnt, sizeof(int), 1, Fileptr);

						if( lpCh->BoneIndexCnt > 25 )
						{
							_XFatalError( "Overflowed bone index count : max vertex shader const count" );
							lpCh->pVertex->Unlock();
							SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
							return FALSE;
						}

						lpCh->pBoneIndexArray = new WORD[ lpCh->BoneIndexCnt ];
						fread(lpCh->pBoneIndexArray, sizeof(WORD)*lpCh->BoneIndexCnt , 1,Fileptr);

						//Shader를 사용하기 위해서 본을 분할 렌더 하기 위해 메쉬에 적용된 본만 따로
						//Shader에 본을 넘기기 위해서 그 본의 인덱스를 버텍스에 적용한다.

						for( int nVertexIdx = 0 ; nVertexIdx < lpCh->nVertexCnt ; nVertexIdx++ )
						{
							if( lpCh->pKeyFactor != NULL )
							{
								for(int nfactor = 0; nfactor < lpCh->pKeyFactor[nVertexIdx].iFactorCnt; nfactor++)
								{
									_XM_FACTOR pKeyFactor = lpCh->pKeyFactor[nVertexIdx].pFactorData[nfactor];
									
									if( nfactor == 0 )
									{
										pVertices[nVertexIdx].index0	 = CV_BONESTART + (float)( FindIDX(lpCh->pBoneIndexArray, lpCh->BoneIndexCnt, pKeyFactor.wKeyIdx )*3.0f ); 
										pVertices[nVertexIdx].index1	 = pVertices[nVertexIdx].index0;
									}
									else if(nfactor == 1)
									{
										pVertices[nVertexIdx].index1	 = CV_BONESTART + (float)(FindIDX(lpCh->pBoneIndexArray, lpCh->BoneIndexCnt, pKeyFactor.wKeyIdx )*3.0f );
									}									
									else
									{
										//_XLog( "EEEEE:%d", nfactor );
									}
								}
							}
						}
					}

					lpCh->pVertex->Unlock();
				}				

				SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
			}
		}		
	}

	OBB_Center = Header.OBB_Center;
    memcpy( OBB_Axis, Header.OBB_Axis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, Header.OBB_Extent, sizeof(FLOAT) * 3 );	

	//float fMagnitude  = _XFC_sqrt( (OBB_Center.x * OBB_Center.x) + (OBB_Center.y * OBB_Center.y) + (OBB_Center.z * OBB_Center.z) );
	float fMagnitude2 = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );	
	m_ObjectRadius = fMagnitude2;// fMagnitude + fMagnitude2;

	if(m_pExtraData)
	{
		_MOB_ATTRIBTABLE*pMobAttrTable = (_MOB_ATTRIBTABLE*)m_pExtraData;
		
		pMobAttrTable->xsize			= OBB_Extent[0];
		pMobAttrTable->ysize			= OBB_Extent[1];
		pMobAttrTable->collideradius	= m_ObjectRadius;
	}

#ifdef _XDWDEBUG
	CsuX3D_Box	obbbox;
	obbbox.Create( Header.OBB_Center, Header.OBB_Axis, Header.OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif
	
//	if( szFilename )
//		_XLog( "Loaded mesh file [%s]", szFilename );

	m_Initialized = true;

	return TRUE;
}

BOOL _XVSAnimMeshContainer::Load_Runtime( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer, LPSTR szFilename )
{
	if( m_Initialized ) 
	{
		return TRUE;
	}

	if( !Fileptr )
	{
		_XFatalError("Invalid file pointer [%s] file", (szFilename) ? szFilename : "this" );
		return FALSE;
	}

#ifdef _XDEF_MODELTHREADLOADING
#else
	DestroyModel();
#endif
	
	if( !pTextureManager )
	{
		return FALSE;
	}

	m_pTextureManager = pTextureManager;
	
	_XMESHHEADER   Header;	
	fread(&Header,	sizeof(_XMESHHEADER), 1, Fileptr);	
		
	if(Header.HeaderIndicator != _XDEF_X3DFILEINDICATOR )
	{
		_XFatalError("Not supported [%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE; 
	}

	if(Header.Fileversion != _XDEF_X3DFILEVERSION)
	{
		_XFatalError("Not supported [%s] file version [%f]",  (szFilename) ? szFilename : "this" , Header.Fileversion );		
		return FALSE; 		
	}

	int i,len;
	int NodeNum;	
	fread(&NodeNum,	sizeof(int), 1, Fileptr);	// read object count

	if( !CreateNode(NodeNum) ) return FALSE;		

	// Load model mesh data...	

	_LPXM_MESHOBJECT lpCh = NULL;
	TCHAR strName[128];

#ifdef _XDEF_MODELTHREADLOADING
#else
	int		MeshLoadBufferSize = 327680;
	LPBYTE	pMeshLoadBuffer = new BYTE[MeshLoadBufferSize];

	if( !pMeshLoadBuffer )
	{
		_XFatalError("Can't allocate temporary read buffer[%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE;
	}
#endif

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		memset( strName, 0, 128 );
		if(len != 0) fread( strName, sizeof(TCHAR)*len, 1, Fileptr );

		// Load mesh object parent id...
		fread(&lpCh->nParentId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		
		if(len != 0) fseek(Fileptr, sizeof(TCHAR)*len, SEEK_CUR);

		// Load object matrix...
		fread(&lpCh->Matrix, sizeof(D3DXMATRIX), 1, Fileptr);
		lpCh->MatrixTM = lpCh->Matrix;

		// preprocess etc matrix...
		_XMeshMath_UnitMatrixFromMatrix(lpCh->Matrix);
		_XMeshMath_MatrixInvert( lpCh->Inverse, lpCh->Matrix);
		_XMeshMath_MatrixMultiply( lpCh->PosMatrixTM, lpCh->Inverse, lpCh->MatrixTM );

		D3DXMATRIX	mat;
		D3DXMatrixIdentity( &mat );

		_LPXM_MESHOBJECT lpParent;
		if(lpCh->nParentId != -1)
		{
			lpParent = GetNode(lpCh->nParentId);
			D3DXMatrixMultiply( &mat, &lpCh->Matrix, &lpParent->Inverse);
		}
		else mat = lpCh->Matrix;

		lpCh->LocalTM = mat;
		_XMeshMath_SetIdentityMatrix(lpCh->LocalAniTM);


		// Load texture id & setting texture index
		TCHAR strTextureName[64];
		fread(&len, sizeof(int), 1, Fileptr);
		memset(strTextureName, 0, sizeof(TCHAR)*64);
		if(len != 0)
		{
			fread(strTextureName, len, 1, Fileptr);
			lpCh->nTextureId = m_pTextureManager->FindResource( strTextureName );
		}
		else
		{
			//_XFatalError( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			_XLog( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			lpCh->nTextureId = -1;
		}

		if( strncmp(strName, "Bip", 3 ) != 0 ) // Skipping biped object
		{
			// Load vertex buffer...
			fread(&lpCh->nVertexCnt, sizeof(lpCh->nVertexCnt), 1, Fileptr);
			if(lpCh->nVertexCnt != 0)
			{
				lpCh->pVertex = NULL;
				
				LONG readbuffersize = lpCh->nVertexCnt*sizeof(_XMESH_PSVERTEX);

				if( FAILED(gpDev->CreateVertexBuffer( readbuffersize, D3DUSAGE_WRITEONLY, 
					D3DFVF_XMESH_PSVERTEX, D3DPOOL_MANAGED, &(lpCh->pVertex), NULL )) ) 
				{
					_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );

#ifdef _XDEF_MODELTHREADLOADING
#else
					SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
					return FALSE;
				}				
				
				//_XFatalError( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );

#ifdef _XDEF_MODELTHREADLOADING
				if( readbuffersize > g_MeshLoadBufferSizeForThread )
				{
					SAFE_DELETE_ARRAY( g_MeshLoadBufferForThread );
					g_MeshLoadBufferForThread = new BYTE[ readbuffersize ];
					if( !g_MeshLoadBufferForThread )
					{
						_XFatalError( "Can't allocate memory : mesh buffer for background thread, vsmesh [%dBytes]", readbuffersize );
						return FALSE;
					}

					g_MeshLoadBufferSizeForThread = readbuffersize;
				}
#else
				if( readbuffersize > MeshLoadBufferSize )
				{
					SAFE_DELETE_ARRAY(pMeshLoadBuffer);
					pMeshLoadBuffer = new BYTE[readbuffersize];
					if( !pMeshLoadBuffer )
					{
						_XFatalError("Can't allocate temporary read buffer[%s] file", (szFilename) ? szFilename : "this" );		
						return FALSE;
					}

					MeshLoadBufferSize = readbuffersize;
				}
#endif
				
				_LPXMESH_PSVERTEX pVertices;				
#ifdef _XDEF_MODELTHREADLOADING
				_LPXMESH_VERTEX	  pLoadBuffer = (_LPXMESH_VERTEX)g_MeshLoadBufferForThread; // memory fragment를 줄이기 위하여 미리 할당된 메모리 사용.
#else
				_LPXMESH_VERTEX	  pLoadBuffer = (_LPXMESH_VERTEX)pMeshLoadBuffer;
#endif

				
				/*pLoadBuffer = new _XMESH_VERTEX[lpCh->nVertexCnt];
				if( !pLoadBuffer )
				{
					_XFatalError("Can't create temporary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}
				*/
				
				if( fread(pLoadBuffer, sizeof(_XMESH_VERTEX) * lpCh->nVertexCnt, 1, Fileptr) < 1 )
				{
					_XFatalError("Can't load vertex data to temporary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
#ifdef _XDEF_MODELTHREADLOADING
#else
					SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
					return FALSE;
				}

				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{					
					for( int vertindex = 0; vertindex < lpCh->nVertexCnt; vertindex++  )
					{
						memcpy( pVertices+vertindex, pLoadBuffer+vertindex, sizeof(_XMESH_VERTEX) );
						/*
						pVertices[vertindex].x	= pLoadBuffer[vertindex].x;
						pVertices[vertindex].y	= pLoadBuffer[vertindex].y;
						pVertices[vertindex].z	= pLoadBuffer[vertindex].z;
						
						pVertices[vertindex].nx	= pLoadBuffer[vertindex].nx;
						pVertices[vertindex].ny	= pLoadBuffer[vertindex].ny;
						pVertices[vertindex].nz	= pLoadBuffer[vertindex].nz;
						
						pVertices[vertindex].tu = pLoadBuffer[vertindex].tu1;
						pVertices[vertindex].tv = pLoadBuffer[vertindex].tv1;
						*/
					}
					
					lpCh->pVertex->Unlock();
				}
				else
				{
					_XFatalError("Lock vertext buffer [%s]", (szFilename) ? szFilename : "this" );
#ifdef _XDEF_MODELTHREADLOADING
#else
					SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
					return FALSE;
				}

				// Create second temporary buffer...
				if( createsecondbuffer )
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_PSVERTEX), D3DUSAGE_WRITEONLY, 
						D3DFVF_XMESH_PSVERTEX, D3DPOOL_MANAGED, &(lpCh->pSecondVertexBuffer), NULL )) )
					{
						_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
#ifdef _XDEF_MODELTHREADLOADING
#else
						SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
						return FALSE;
					}
					//_XFatalError( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
				}
			}

			// Load index buffer...
			fread(&lpCh->nIndexCnt, sizeof(lpCh->nIndexCnt), 1, Fileptr);    
			
			if(lpCh->nIndexCnt != 0)
			{
				if( FAILED(gpDev->CreateIndexBuffer( lpCh->nIndexCnt*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
													 D3DPOOL_MANAGED, &(lpCh->pIndex), NULL )) )
				{
					_XFatalError("Create index buffer [%s]", (szFilename) ? szFilename : "this" );
#ifdef _XDEF_MODELTHREADLOADING
#else
					SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
					return FALSE;
				}					
				
				//_XFatalError( "Index buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pIndex );
				
				LPWORD pIndices;				
				if( SUCCEEDED( lpCh->pIndex->Lock( 0, lpCh->nIndexCnt*sizeof(WORD), (void**)&pIndices, D3DLOCK_NOSYSLOCK )) )
				{						
					fread(pIndices, sizeof(WORD) * lpCh->nIndexCnt, 1, Fileptr);
					lpCh->pIndex->Unlock();
				}
				else
				{
					_XFatalError("Lock index buffer [%s]", (szFilename) ? szFilename : "this" );
#ifdef _XDEF_MODELTHREADLOADING
#else
					SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
					return FALSE;
				}			
			}

			lpCh->nIndexCnt /= 3;

			// Read physique count
			fread(&lpCh->nKeyFactorCnt, sizeof(lpCh->nKeyFactorCnt), 1, Fileptr);

			if(lpCh->nKeyFactorCnt > 0)
			{
				lpCh->pKeyFactor = new _XM_KEYFACTOR[lpCh->nKeyFactorCnt];

				_LPXMESH_PSVERTEX	pVertices = NULL;
				
				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{
					for(int j = 0; j < lpCh->nKeyFactorCnt; j++)
					{
						// Read linked vertex count for Physique...
						fread(&lpCh->pKeyFactor[j].iFactorCnt, sizeof(int), 1, Fileptr);
						
						if(lpCh->pKeyFactor[j].iFactorCnt == 0)
						{					
							_XFatalError( "WARNING : Invalid physique informations\nvertex : %d, Physique number : %d",
																					  j, lpCh->pKeyFactor[j].iFactorCnt );

							lpCh->pVertex->Unlock();
							lpCh->pKeyFactor = NULL;
#ifdef _XDEF_MODELTHREADLOADING
#else
							SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
							return FALSE;
						}
						else
						{
							lpCh->pKeyFactor[j].pFactorData = new _XM_FACTOR[lpCh->pKeyFactor[j].iFactorCnt];

							// Read physique informations
							for(int k = 0; k < lpCh->pKeyFactor[j].iFactorCnt; k++)
							{
								fread(&lpCh->pKeyFactor[j].pFactorData[k], sizeof(_XM_FACTOR), 1, Fileptr);
							}

							pVertices[j].weight0 = lpCh->pKeyFactor[j].pFactorData[0].fBlendFactor;
							pVertices[j].weight1 = 1.0f - pVertices[j].weight0;
						}
					}


					if( lpCh->nKeyFactorCnt > 0 )
					{
						fread(&lpCh->BoneIndexCnt, sizeof(int), 1, Fileptr);

						if( lpCh->BoneIndexCnt > 25 )
						{
							_XFatalError( "Overflowed bone index count : max vertex shader const count" );
							lpCh->pVertex->Unlock();
							lpCh->pKeyFactor = NULL;
#ifdef _XDEF_MODELTHREADLOADING
#else
							SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
							return FALSE;
						}

						lpCh->pBoneIndexArray = new WORD[ lpCh->BoneIndexCnt ];
						fread(lpCh->pBoneIndexArray, sizeof(WORD)*lpCh->BoneIndexCnt , 1,Fileptr);

						//Shader를 사용하기 위해서 본을 분할 렌더 하기 위해 메쉬에 적용된 본만 따로
						//Shader에 본을 넘기기 위해서 그 본의 인덱스를 버텍스에 적용한다.

						for( int nVertexIdx = 0 ; nVertexIdx < lpCh->nVertexCnt ; nVertexIdx++ )
						{
							if( lpCh->pKeyFactor != NULL )
							{
								for(int nfactor = 0; nfactor < lpCh->pKeyFactor[nVertexIdx].iFactorCnt; nfactor++)
								{
									_XM_FACTOR pKeyFactor = lpCh->pKeyFactor[nVertexIdx].pFactorData[nfactor];
									
									if( nfactor == 0 )
									{
										pVertices[nVertexIdx].index0	 = CV_BONESTART + (float)( FindIDX(lpCh->pBoneIndexArray, lpCh->BoneIndexCnt, pKeyFactor.wKeyIdx )*3.0f ); 
										pVertices[nVertexIdx].index1	 = pVertices[nVertexIdx].index0;
									}
									else if(nfactor == 1)
									{
										pVertices[nVertexIdx].index1	 = CV_BONESTART + (float)(FindIDX(lpCh->pBoneIndexArray, lpCh->BoneIndexCnt, pKeyFactor.wKeyIdx )*3.0f );
									}									
									else
									{
										//_XLog( "EEEEE:%d", nfactor );
									}
								}
							}
						}
					}

					lpCh->pVertex->Unlock();
				}				
				else
				{
					_XFatalError("Lock vertext buffer 2'nd [%s]", (szFilename) ? szFilename : "this" );
					SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
#ifdef _XDEF_MODELTHREADLOADING
#else
					SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
					return FALSE;

				}

				SAFE_DELETE_ARRAY( lpCh->pKeyFactor );
			}
		}		
	}

	OBB_Center = Header.OBB_Center;
    memcpy( OBB_Axis, Header.OBB_Axis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, Header.OBB_Extent, sizeof(FLOAT) * 3 );	

	//float fMagnitude  = _XFC_sqrt( (OBB_Center.x * OBB_Center.x) + (OBB_Center.y * OBB_Center.y) + (OBB_Center.z * OBB_Center.z) );
	float fMagnitude2 = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );	
	m_ObjectRadius = fMagnitude2;// fMagnitude + fMagnitude2;

	if(m_pExtraData)
	{
		_MOB_ATTRIBTABLE*pMobAttrTable = (_MOB_ATTRIBTABLE*)m_pExtraData;
		
		pMobAttrTable->xsize			= OBB_Extent[0];
		pMobAttrTable->ysize			= OBB_Extent[1];
		pMobAttrTable->collideradius	= m_ObjectRadius;
	}

#ifdef _XDWDEBUG
	CsuX3D_Box	obbbox;
	obbbox.Create( Header.OBB_Center, Header.OBB_Axis, Header.OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif
	
//	if( szFilename )
//		_XLog( "Loaded mesh file [%s]", szFilename );

	//m_Initialized = true; //Lod object 까지 모두 로딩 후 background thread 쪽에서 켜짐.

#ifdef _XDEF_MODELTHREADLOADING
#else	
	SAFE_DELETE_ARRAY( pMeshLoadBuffer );
#endif
	return TRUE;
}

void _XVSAnimMeshContainer::Animation( _XModelDescriptor* pAniData )
{
#ifdef _XDEF_USESLERPANIMATION	
	if(pAniData->m_MotionChangeFlag)
	{
		SlerpAnimation(pAniData);
	}
	else
#endif
		
	{
		FrameAnimation(pAniData);
	}
}
void _XVSAnimMeshContainer::FrameAnimation( _XModelDescriptor* pAniData )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

}
void _XVSAnimMeshContainer::SlerpAnimation( _XModelDescriptor* pAniData )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if( !pAniData->m_pBipedController ) return;
	
	if( pAniData->m_SlerpAniFrame >= _XDEF_SLERPANIMATIONTIME)
	{
		_LPXM_MOTION  pMotion = pAniData->m_pBipedController->GetMotion( pAniData->m_CurMotion );
		pAniData->m_AniFrame = (FLOAT)pMotion->StartFrame;
		pAniData->m_MotionChangeFlag = false;
	}	
}

#define _XGET_ALPHA( x ) (( (x) & 0xFF000000 ) >> 24)
#define _XGET_RED( x )   (( (x) & 0xFF0000 ) >> 16)
#define _XGET_GREEN( x ) (( (x) & 0xFF00 ) >> 8)
#define _XGET_BLUE( x )  (( (x) & 0xFF ) )

#define FLOAT01_TO_INT0255(X) ((int)(X*255.0f))
#define INT0255_TO_FLOAT01(X) ((float)X/255.0f)

void _XVSAnimMeshContainer::SetVertexShaderConst( _XModelDescriptor* pAniData,  D3DXMATRIX* pworldmat, D3DXMATRIX* pviewmat )
{
	D3DXMATRIX matLocal;
	D3DXMATRIX worldViewProjMat; 
	D3DXVECTOR4 lightDirection;	
	D3DXVECTOR3 lightPo = g_EnvironmentManager.m_ObjectLight.Position; 
	D3DXVECTOR3 vCenter;

	if( pworldmat )
	{
		D3DXMatrixInverse(&matLocal, NULL, pworldmat);
		
		if( pviewmat )
			D3DXMatrixMultiply(&worldViewProjMat, pworldmat, pviewmat );
		else
			D3DXMatrixMultiply(&worldViewProjMat, pworldmat, &g_LodTerrain.m_3PCamera.mp_view_matrix );			
		
		vCenter.x = pworldmat->_41;
		vCenter.y = pworldmat->_42;
		vCenter.z = pworldmat->_43;
	}
	else
	{
		D3DXMatrixInverse(&matLocal, NULL, &pAniData->m_Position);
		
		if( pviewmat )
			D3DXMatrixMultiply(&worldViewProjMat, &pAniData->m_Position, pviewmat );
		else
			D3DXMatrixMultiply(&worldViewProjMat, &pAniData->m_Position, &g_LodTerrain.m_3PCamera.mp_view_matrix );			
		
		vCenter.x = pAniData->m_Position._41;
		vCenter.y = pAniData->m_Position._42;
		vCenter.z = pAniData->m_Position._43;
	}
	
	
	D3DXMatrixMultiply(&worldViewProjMat, &worldViewProjMat, &g_LodTerrain.m_3PCamera.mp_proj_matrix);
	D3DXMatrixTranspose(&worldViewProjMat, &worldViewProjMat);
	
	gpDev->SetVertexShaderConstantF(CV_WORLDVIEWPROJ_0, (float*)&worldViewProjMat, 4);
		
	//lightDirection.x = ( lightPo.x - vCenter.x );
	//lightDirection.y = ( lightPo.y - vCenter.y );
	//lightDirection.z = ( lightPo.z - vCenter.z );
	//lightDirection.w = 1.0f;

	lightDirection.x = g_EnvironmentManager.m_ObjectLight.Direction.x;
	lightDirection.y = g_EnvironmentManager.m_ObjectLight.Direction.y;
	lightDirection.z = g_EnvironmentManager.m_ObjectLight.Direction.z;
	lightDirection.w = 1.0f;

	//D3DXVec4Transform(&lightDirection, &lightDirection, &pAniData->m_Position);	// light position -> character front face
	D3DXVec4Transform(&lightDirection, &lightDirection, &matLocal);	
	D3DXVec4Normalize(&lightDirection, &lightDirection );
	gpDev->SetVertexShaderConstantF(CV_LIGHT_DIRECTION, (float*)&lightDirection, 1);

	D3DXVECTOR4 constNums(2.0f, 0.5f, 1.0f, 0.2f);
	gpDev->SetVertexShaderConstantF(CV_CONSTANTS, (float*)&constNums, 1);
	
	D3DXVECTOR4 temp;
	//temp.x = g_LodTerrain.m_CharacterLight.Ambient.r;
	//temp.y = g_LodTerrain.m_CharacterLight.Ambient.g;
	//temp.z = g_LodTerrain.m_CharacterLight.Ambient.b; 
	temp.x = INT0255_TO_FLOAT01(  _XGET_RED( g_EnvironmentManager.m_CharacterAmbientColor   ) );
	temp.y = INT0255_TO_FLOAT01(  _XGET_GREEN( g_EnvironmentManager.m_CharacterAmbientColor ) );
	temp.z = INT0255_TO_FLOAT01(  _XGET_BLUE( g_EnvironmentManager.m_CharacterAmbientColor  ) );
	temp.w = 1.0f;
	gpDev->SetVertexShaderConstantF(CV_AMBIENT_COLOR, (float*)&temp, 1);

	temp.x = g_EnvironmentManager.m_CharacterLight.Diffuse.r;
	temp.y = g_EnvironmentManager.m_CharacterLight.Diffuse.g;
	temp.z = g_EnvironmentManager.m_CharacterLight.Diffuse.b;	
	temp.w = 1.0f;
	gpDev->SetVertexShaderConstantF(CV_LIGHT_DIFFUSE, (float*)&temp, 1);
	
	temp.x = 0.0f;
	temp.y = 0.0f;
	temp.z = 0.0f;
	temp.w = 1.0f;
	gpDev->SetVertexShaderConstantF(CV_SPECULAR_COLOR, (float*)&temp, 1);

	temp.x = g_LodTerrain.m_3PCamera.m_CameraPosition.x;
	temp.y = g_LodTerrain.m_3PCamera.m_CameraPosition.y;
	temp.z = g_LodTerrain.m_3PCamera.m_CameraPosition.z;
	temp.w = 1.0f;
	//Transform the eye position to object space.
	D3DXVec4Transform(&temp, &temp, &matLocal);
	gpDev->SetVertexShaderConstantF(CV_EYE_VECTOR, (float*)&temp, 1);

	/*
	temp.x = 0.3f;
	temp.y = 0.3f;
	temp.z = 0.3f;
	temp.w = 1.0f;
	gpDev->SetVertexShaderConstantF(CV_VDIFFUSE_COLOR, (float*)&temp, 1);
	
	temp.x = 0.5f;
	temp.y = 0.5f;
	temp.z = 0.5f;
	temp.w = 0.0f;
    gpDev->SetVertexShaderConstantF( CV_SILLHOUETTECOLOR , (float*)&temp , 1 );
		
	temp.x = 2.0f;
	temp.y = 0.5;	 // Specular power
	temp.z = 0.005f; //실루엣 사이즈
	temp.w = 0.0f;
	gpDev->SetVertexShaderConstantF(CV_CONSTANTS1, (float*)&temp, 1);
	*/
    /*
	temp.x = 0.0f;  //toon Texture V 축 값
	temp.y = 0.0f;  //현재 사용안함
	temp.z = 0.0f;  //현재 사용안함
	temp.w = 0.0f;  //현재 사용안함
	gpDev->SetVertexShaderConstantF(CV_TOONCONST, (float*)&temp, 1);
	*/
	D3DXMatrixTranspose(&matLocal, &matLocal);
	gpDev->SetVertexShaderConstantF(CV_WORLD_0, (float*)&matLocal, 4);

}

VOID _XVSAnimMeshContainer::SetBoneConstant(_LPXM_MESHOBJECT lpCh, _XModelDescriptor* pAniData)
{
	D3DXMATRIX		  matBiped;
	_LPXM_BIPEDOBJECT lpBiped;

	if( lpCh->nKeyFactorCnt )
	{
		for( int i = 0; i< lpCh->BoneIndexCnt; i++)
		{
			lpBiped = pAniData->m_pBipedController->GetNode(lpCh->pBoneIndexArray[i]);

			if( lpBiped )
			{
				D3DXMatrixTranspose( &matBiped, &lpBiped->m_matLocalAni );
				gpDev->SetVertexShaderConstantF( CV_BONESTART + ( i * 3 ),  (float*)&matBiped, 3);
			}
		}
	}
}

// by mahwang
void _XVSAnimMeshContainer::Render( D3DXMATRIX* pmatKeyframe )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	_XRenderStyleManager::m_UseVertexShader = TRUE;

	gpDev->SetVertexShader( NULL );
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
		
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;

		gpDev->SetTransform( D3DTS_WORLD, &pmatKeyframe[i] );

		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_PSVERTEX) );
//		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
	_XRenderStyleManager::m_UseVertexShader = FALSE;
}

void _XVSAnimMeshContainer::RenderNoTexture( D3DXMATRIX* pmatKeyframe )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	gpDev->SetVertexShader( NULL );
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
		
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;

		gpDev->SetTransform( D3DTS_WORLD, &pmatKeyframe[i] );

//		m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		
		gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_PSVERTEX) );
//		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
}

void _XVSAnimMeshContainer::Render( D3DXMATRIX worldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	_XRenderStyleManager::m_UseVertexShader = TRUE;
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	gpDev->SetTransform( D3DTS_WORLD, &worldmat );
		
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
				
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
	_XRenderStyleManager::m_UseVertexShader = FALSE;
}

void _XVSAnimMeshContainer::Render_Reflect( _XModelDescriptor* pAniData, D3DXMATRIX* pReflectViewMatrix )
{	
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData || !m_Node) return;

	_XRenderStyleManager::m_UseVertexShader = TRUE;
	
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	SetVertexShaderConst( pAniData, NULL, pReflectViewMatrix );
	
	gpDev->SetVertexShader( g_pAnimationShaderPalette[0] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		SetBoneConstant( (m_Node+i), pAniData );
		
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		gpDev->SetRenderState(D3DRS_ZENABLE,false);
		gpDev->SetRenderState(D3DRS_ZWRITEENABLE,false); 

		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
	gpDev->SetRenderState(D3DRS_ZENABLE,g_RS_UseZBuffer);
	gpDev->SetRenderState(D3DRS_ZWRITEENABLE,true); 
	gpDev->SetVertexShader( NULL );

	_XRenderStyleManager::m_UseVertexShader = FALSE;
}

void _XVSAnimMeshContainer::Render( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData || !m_Node) return;
	
	_XRenderStyleManager::m_UseVertexShader = TRUE;

	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	SetVertexShaderConst( pAniData, pworldmat );
	
	gpDev->SetVertexShader( g_pAnimationShaderPalette[0] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		SetBoneConstant( (m_Node+i), pAniData );
		
		g_SetTextureFailed = FALSE;
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}

	gpDev->SetVertexShader( NULL );
	_XRenderStyleManager::m_UseVertexShader = FALSE;
}

void _XVSAnimMeshContainer::Render_LODCheck( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData || !m_Node) return;
	if( m_pLODObjectList )
	{
		if( pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL1 )
		{
			if( m_pLODObjectList[1] && m_pLODObjectCount == 2 && pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL2 )
			{
				m_pLODObjectList[1]->Render( pAniData, pworldmat );				
				return;
			}			
			
			m_pLODObjectList[0]->Render( pAniData, pworldmat );
			return;
		}
	}
	
	_XRenderStyleManager::m_UseVertexShader = TRUE;
	
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	SetVertexShaderConst( pAniData, pworldmat );
	
	gpDev->SetVertexShader( g_pAnimationShaderPalette[0] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		SetBoneConstant( (m_Node+i), pAniData );
		
		g_SetTextureFailed = FALSE;
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
	
	gpDev->SetVertexShader( NULL );
	_XRenderStyleManager::m_UseVertexShader = FALSE;	
}

void _XVSAnimMeshContainer::Render_LowLevelLOD( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData || !m_Node) return;
	
	if( m_pLODObjectList )
	{
		if( m_pLODObjectList[1] && m_pLODObjectCount == 2)
		{
			m_pLODObjectList[1]->Render_LowLevelLOD( pAniData, pworldmat );
			return;
		}			
		else
		{			
			m_pLODObjectList[0]->Render_LowLevelLOD( pAniData, pworldmat );
			return;
		}
	}

	_XRenderStyleManager::m_UseVertexShader = TRUE;
	
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	SetVertexShaderConst( pAniData, pworldmat );
	
	gpDev->SetVertexShader( g_pAnimationShaderPalette[0] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		SetBoneConstant( (m_Node+i), pAniData );
		
		g_SetTextureFailed = FALSE;
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
	
	gpDev->SetVertexShader( NULL );
	_XRenderStyleManager::m_UseVertexShader = FALSE;	
}

void _XVSAnimMeshContainer::RenderNoTexture( D3DXMATRIX worldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	gpDev->SetTransform( D3DTS_WORLD, &worldmat );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
	
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
}

void _XVSAnimMeshContainer::RenderNoTexture( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData) return;
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	SetVertexShaderConst( pAniData, pworldmat );
	
	gpDev->SetVertexShader( g_pAnimationShaderPalette[0] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		SetBoneConstant( (m_Node+i), pAniData );
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
	
	gpDev->SetVertexShader( NULL );
}

void _XVSAnimMeshContainer::RenderCustomDraw( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData) return;

	_XRenderStyleManager::m_UseVertexShader = TRUE;
	
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	SetVertexShaderConst( pAniData, pworldmat );
	
	gpDev->SetVertexShader( g_pAnimationShaderPalette[0] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		SetBoneConstant( (m_Node+i), pAniData );
		
		if (m_pTextureManager) m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture_NoRenderStyle( m_Node[i].nTextureId );

		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
	
	gpDev->SetVertexShader( NULL );
	_XRenderStyleManager::m_UseVertexShader = FALSE;
}

void _XVSAnimMeshContainer::RenderShadow_LODCheck( _XModelDescriptor* pAniData )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if( m_pLODObjectList )
	{
		if( m_pLODObjectList[1] && m_pLODObjectCount == 2)
		{
			m_pLODObjectList[1]->RenderShadow( pAniData );				
			return;
		}			
		else
		{			
			m_pLODObjectList[0]->RenderShadow( pAniData );
			return;
		}
	}

	D3DXMATRIX worldViewProjMat; 

	D3DXMatrixMultiply(&worldViewProjMat,  &pAniData->m_matLocalToWorld, &pAniData->m_matWorldToLight );		
	D3DXMatrixMultiply(&worldViewProjMat,  &worldViewProjMat, &pAniData->m_matShadowProj);
	D3DXMatrixTranspose(&worldViewProjMat, &worldViewProjMat);

	gpDev->SetVertexShaderConstantF(CV_WORLDVIEWPROJ_0, (float*)&worldViewProjMat, 4);

	D3DXVECTOR4 constNums(0.0f, 0.5f, 1.0f, 0.2f);
	gpDev->SetVertexShaderConstantF(CV_CONSTANTS, (float*)&constNums, 1);
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	gpDev->SetVertexShader( g_pAnimationShaderPalette[4] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );

	for(int i = 0; i < GetNodeCnt(); i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		
		SetBoneConstant(m_Node+i, pAniData);
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX ) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}

	gpDev->SetVertexShader( NULL );
	//gpDev->SetVertexDeclaration( NULL );
}

void _XVSAnimMeshContainer::RenderShadow( _XModelDescriptor* pAniData )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	D3DXMATRIX worldViewProjMat; 
	
	D3DXMatrixMultiply(&worldViewProjMat,  &pAniData->m_matLocalToWorld, &pAniData->m_matWorldToLight );		
	D3DXMatrixMultiply(&worldViewProjMat,  &worldViewProjMat, &pAniData->m_matShadowProj);
	D3DXMatrixTranspose(&worldViewProjMat, &worldViewProjMat);
	
	gpDev->SetVertexShaderConstantF(CV_WORLDVIEWPROJ_0, (float*)&worldViewProjMat, 4);
	
	D3DXVECTOR4 constNums(0.0f, 0.5f, 1.0f, 0.2f);
	gpDev->SetVertexShaderConstantF(CV_CONSTANTS, (float*)&constNums, 1);
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	gpDev->SetVertexShader( g_pAnimationShaderPalette[4] );
	gpDev->SetVertexDeclaration( g_pAnimationShaderDeclation );

	for(int i = 0; i < GetNodeCnt(); i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		
		SetBoneConstant(m_Node+i, pAniData);
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX ) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}

	gpDev->SetVertexShader( NULL );
	//gpDev->SetVertexDeclaration( NULL );
}

void _XVSAnimMeshContainer::RenderInterfaceItem(D3DXMATRIX* rotmat)
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;

	_XRenderStyleManager::m_UseVertexShader = TRUE;
	
	D3DXMATRIX	curmat;		
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	
	for(int i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;
		
		D3DXMatrixIdentity(&curmat);
		D3DXMatrixMultiply(&curmat, &curmat, &m_Node[i].Inverse);
		
		if(rotmat) D3DXMatrixMultiply( &curmat, &curmat, rotmat );
		
		gpDev->SetTransform( D3DTS_WORLD, &curmat );
		
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
		gpDev->SetRenderState( D3DRS_ALPHAREF, _XDEF_INTERFACEMODELALPHAREF );
		gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
		gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );			
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}	

	_XRenderStyleManager::m_UseVertexShader = FALSE;
}

void _XVSAnimMeshContainer::Render_OriginalPos( D3DXVECTOR3& offset, int index, D3DXMATRIX* rotmat, BOOL usealphatest )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;

	_XRenderStyleManager::m_UseVertexShader = TRUE;

	D3DXMATRIX	curmat;	
	gpDev->SetFVF( D3DFVF_XMESH_PSVERTEX );
	
	for(int i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;
		
		D3DXMatrixIdentity(&curmat);
		D3DXMatrixMultiply(&curmat, &curmat, &m_Node[i].Inverse);
		if(rotmat) D3DXMatrixMultiply( &curmat, &curmat, rotmat );
		
		curmat._41 += offset.x;
		curmat._42 += offset.y;
		curmat._43 += offset.z;
		gpDev->SetTransform( D3DTS_WORLD, &curmat );
		
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		if( usealphatest )
		{
			gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
			gpDev->SetRenderState( D3DRS_ALPHAREF, _XDEF_INTERFACEMODELALPHAREF );
			gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
			gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );
		}
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_PSVERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );		
	}	

	_XRenderStyleManager::m_UseVertexShader = FALSE;
}

extern bool  Jacobi( float a[3][3], float v[3][3], float d[3] );

void _XVSAnimMeshContainer::RebuildOBBInfo( void )
{
	m_LastUsedTime = g_LocalSystemTime;
	
	_LPXM_MESHOBJECT lpCh = NULL;
	
	CsuX3D_Box Box;
	CsuX3D_Box MergedBox;
	_XMESH_PSVERTEX*	pVertices;

	for(int node = 0; node < m_NodeCnt; node++)
	{
		lpCh = GetNode(node);
		
		if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
		{
			// compute mean of points
			D3DXVECTOR3 m( 0, 0, 0 );
			for( int i = 0; i < lpCh->nVertexCnt; ++i )
			{
				m.x += pVertices[i].x;
				m.y += pVertices[i].y;
				m.z += pVertices[i].z;
			}
			m /= (float)lpCh->nVertexCnt;
			
			
			// compute covariances of points
			float C11 = 0, C22 = 0, C33 = 0, C12 = 0, C13 = 0,  C23 = 0;
			for( i = 0; i < lpCh->nVertexCnt; ++i )
			{
				C11 += ( pVertices[i].x - m.x ) * ( pVertices[i].x - m.x );
				C22 += ( pVertices[i].y - m.y ) * ( pVertices[i].y - m.y );
				C33 += ( pVertices[i].z - m.z ) * ( pVertices[i].z - m.z );
				C12 += ( pVertices[i].x - m.x ) * ( pVertices[i].y - m.y );
				C13 += ( pVertices[i].x - m.x ) * ( pVertices[i].z - m.z );
				C23 += ( pVertices[i].y - m.y ) * ( pVertices[i].z - m.z );
			}
			C11 /= lpCh->nVertexCnt;
			C22 /= lpCh->nVertexCnt;
			C33 /= lpCh->nVertexCnt;
			C12 /= lpCh->nVertexCnt;
			C13 /= lpCh->nVertexCnt;
			C23 /= lpCh->nVertexCnt;
			
			// compute eigenvectors for covariance matrix
			float Matrix[3][3] = 
			{
				{ C11, C12, C13 },
				{ C12, C22, C23 },
				{ C13, C23, C33 },
			};
			
			float EigenVectors[3][3];
			float EigenValue[3];
			Jacobi( Matrix, EigenVectors, EigenValue );
			
			
			struct SORT{
				int ID;
				float Value;
			} Sort[3] = { { 0, EigenValue[0] }, { 1, EigenValue[1] }, { 2, EigenValue[2] } };
			
			
			// 축 구하기 
			for(i = 0; i < 3; ++i )
			{
				Box.m_akAxis[i].x = EigenVectors[0][Sort[i].ID];
				Box.m_akAxis[i].y = EigenVectors[1][Sort[i].ID];
				Box.m_akAxis[i].z = EigenVectors[2][Sort[i].ID];
			}
			
			float min[3] = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
			float max[3] = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			for( int j = 0; j < 3; ++j )
			{
				for( int i = 0; i < lpCh->nVertexCnt; ++i )
				{
					// dot product...
					float a = pVertices[i].x * Box.m_akAxis[j].x + pVertices[i].y * Box.m_akAxis[j].y + pVertices[i].z * Box.m_akAxis[j].z;
					if( min[j] > a ) min[j] = a;
					if( max[j] < a ) max[j] = a;
				}
			}
			
			// 중심 구하기 
			Box.m_kCenter = Box.m_akAxis[0] * ( ( min[0] + max[0] ) * 0.5f )
						  + Box.m_akAxis[1] * ( ( min[1] + max[1] ) * 0.5f )
						  + Box.m_akAxis[2] * ( ( min[2] + max[2] ) * 0.5f );
			
			// 길이 구하기
			for(  i = 0; i < 3; ++i ) 
				Box.m_afExtent[i] = 0.5f*(max[i] - min[i]);

			lpCh->pVertex->Unlock();

			if( node == 0 )
			{
				MergedBox = Box;
			}
			else
			{
				MergedBox = _XOBBMergeBoxes( Box, MergedBox );
			}
		}
	}

	OBB_Center = MergedBox.m_kCenter;
	memcpy( OBB_Axis, MergedBox.m_akAxis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, MergedBox.m_afExtent, sizeof(FLOAT) * 3 );

#ifdef _XDWDEBUG
	CsuX3D_Box	obbbox;
	obbbox.Create( OBB_Center, OBB_Axis, OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// _XNSAnimMeshContainer


_XNSAnimMeshContainer::_XNSAnimMeshContainer()
{	
	m_NodeCnt				= 0;
	m_Node					= NULL;			
	m_pTextureManager		= NULL;
	m_pTextureController	= NULL;
	m_ObjectRadius			= 0;
	m_pLODObjectCount		= 0;
	m_pLODObjectList		= NULL;

	m_Initialized			= false;
	m_ThreadLoad			= false;

	m_LastUsedTime			= 0L;

	m_pExtraData			= NULL;
}

_XNSAnimMeshContainer::~_XNSAnimMeshContainer()
{
	DestroyModel();
}

BOOL _XNSAnimMeshContainer::Load( LPSTR szFilename, _XTextureManager* pTextureManager, BOOL createsecondbuffer )
{
	FILE*	Fileptr = NULL;
	
	if( ( Fileptr = fopen( szFilename , "rb" ) ) == NULL )
	{
		_XFatalError( "[%s] file open", szFilename );
		return FALSE;
	}

	if( !Load( Fileptr, pTextureManager, createsecondbuffer, szFilename ) )
	{
		fclose( Fileptr );
		return FALSE;
	}

	fclose( Fileptr );

	return TRUE;
}

BOOL _XNSAnimMeshContainer::Load( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer, LPSTR szFilename )
{
	if( !Fileptr )
	{
		_XFatalError("Invalid file pointer [%s] file", (szFilename) ? szFilename : "this" );
		return FALSE;
	}

	DestroyModel();
	
	if( !pTextureManager )
	{
		return FALSE;
	}

	m_pTextureManager = pTextureManager;

	_XMESHHEADER   Header;	
	fread(&Header,	sizeof(_XMESHHEADER), 1, Fileptr);	
		
	if(Header.HeaderIndicator != _XDEF_X3DFILEINDICATOR )
	{
		_XFatalError("Not supported [%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE; 
	}

	if(Header.Fileversion != _XDEF_X3DFILEVERSION)
	{
		_XFatalError("Not supported [%s] file version [%f]",  (szFilename) ? szFilename : "this" , Header.Fileversion );		
		return FALSE; 		
	}

	int i,len;
	int NodeNum;	
	fread(&NodeNum,	sizeof(int), 1, Fileptr);	// read object count

	if( !CreateNode(NodeNum) ) return FALSE;		

	// Load model mesh data...	

	_LPXM_MESHOBJECT lpCh = NULL;
	TCHAR strName[128];

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		memset( strName, 0, 128 );
		if(len != 0) fread( strName, sizeof(TCHAR)*len, 1, Fileptr );

		// Load mesh object parent id...
		fread(&lpCh->nParentId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		
		if(len != 0) fseek(Fileptr, sizeof(TCHAR)*len, SEEK_CUR);

		// Load object matrix...
		fread(&lpCh->Matrix, sizeof(D3DXMATRIX), 1, Fileptr);
		lpCh->MatrixTM = lpCh->Matrix;

		// preprocess etc matrix...
		_XMeshMath_UnitMatrixFromMatrix(lpCh->Matrix);
		_XMeshMath_MatrixInvert( lpCh->Inverse, lpCh->Matrix);
		_XMeshMath_MatrixMultiply( lpCh->PosMatrixTM, lpCh->Inverse, lpCh->MatrixTM );

		D3DXMATRIX	mat;
		_XMeshMath_SetIdentityMatrix(mat);

		_LPXM_MESHOBJECT lpParent;
		if(lpCh->nParentId != -1)
		{
			lpParent = GetNode(lpCh->nParentId);
			_XMeshMath_MatrixMultiply(mat, lpCh->Matrix, lpParent->Inverse);
		}
		else mat = lpCh->Matrix;

		lpCh->LocalTM = mat;
		_XMeshMath_SetIdentityMatrix(lpCh->LocalAniTM);


		// Load texture id & setting texture index
		TCHAR strTextureName[64];
		fread(&len, sizeof(int), 1, Fileptr);
		memset(strTextureName, 0, sizeof(TCHAR)*64);
		if(len != 0)
		{
			fread(strTextureName, len, 1, Fileptr);
			lpCh->nTextureId = m_pTextureManager->GetResourceIndex( strTextureName, g_TextureMipLevel_CharacterModel );
		}
		else
		{
			//_XFatalError( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			lpCh->nTextureId = -1;
		}

		if( strncmp(strName, "Bip", 3 ) != 0 ) // Skipping biped object
		{
			// Load vertex buffer...
			fread(&lpCh->nVertexCnt, sizeof(lpCh->nVertexCnt), 1, Fileptr);

			SAFE_RELEASE( lpCh->pVertex );
			SAFE_RELEASE( lpCh->pSecondVertexBuffer );

			if(lpCh->nVertexCnt != 0)
			{
				if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), D3DUSAGE_WRITEONLY, 
					D3DFVF_XMESH_VERTEX, D3DPOOL_MANAGED, &(lpCh->pVertex), NULL )) ) 
				{
					_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}				

				//_XFatalError( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );
				
				_LPXMESH_VERTEX pVertices;
				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{					
					fread(pVertices, sizeof(_XMESH_VERTEX) * lpCh->nVertexCnt, 1, Fileptr);
					lpCh->pVertex->Unlock();
				}

				// Create second temporary buffer...
				if( createsecondbuffer )
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), D3DUSAGE_WRITEONLY, 
						D3DFVF_XMESH_VERTEX, D3DPOOL_MANAGED, &(lpCh->pSecondVertexBuffer), NULL )) )
					{
						_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
						return FALSE;
					}
					
					//_XFatalError( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
				}
			}

			// Load index buffer...
			fread(&lpCh->nIndexCnt, sizeof(lpCh->nIndexCnt), 1, Fileptr);
			
			SAFE_RELEASE( lpCh->pIndex );

			if(lpCh->nIndexCnt != 0)
			{
				if( FAILED(gpDev->CreateIndexBuffer( lpCh->nIndexCnt*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
													 D3DPOOL_MANAGED, &(lpCh->pIndex), NULL )) )
				{
					_XFatalError("Create index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}					

				//_XFatalError( "Index buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pIndex );
				
				LPWORD pIndices;				
				if( SUCCEEDED( lpCh->pIndex->Lock( 0, lpCh->nIndexCnt*sizeof(WORD), (void**)&pIndices, D3DLOCK_NOSYSLOCK )) )
				{						
					fread(pIndices, sizeof(WORD) * lpCh->nIndexCnt, 1, Fileptr);
					lpCh->pIndex->Unlock();
				}
				else
				{
					_XFatalError("Lock index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}			
			}

			lpCh->nIndexCnt /= 3;

			// Read physique count
			fread(&lpCh->nKeyFactorCnt, sizeof(lpCh->nKeyFactorCnt), 1, Fileptr);

			if(lpCh->nKeyFactorCnt > 0)
			{
				lpCh->pKeyFactor = new _XM_KEYFACTOR[lpCh->nKeyFactorCnt];

				for(int j = 0; j < lpCh->nKeyFactorCnt; j++)
				{
					// Read linked vertex count for Physique...
					fread(&lpCh->pKeyFactor[j].iFactorCnt, sizeof(int), 1, Fileptr);
					
					if(lpCh->pKeyFactor[j].iFactorCnt == 0)
					{					
						_XFatalError( "Invalid physique informations\nvertex : %d, Physique number : %d",
																				  j, lpCh->pKeyFactor[j].iFactorCnt );
					}
					else
					{
						
						lpCh->pKeyFactor[j].pFactorData = new _XM_FACTOR[lpCh->pKeyFactor[j].iFactorCnt];

						// Read physique informations
						for(int k = 0; k < lpCh->pKeyFactor[j].iFactorCnt; k++)
							fread(&lpCh->pKeyFactor[j].pFactorData[k], sizeof(_XM_FACTOR), 1, Fileptr);
					}
				}

				fread(&lpCh->BoneIndexCnt, sizeof(int), 1, Fileptr);						
				fseek(Fileptr, sizeof(WORD)*lpCh->BoneIndexCnt , SEEK_CUR);
			}
		}		
	}

	//SetSkinning();

	OBB_Center = Header.OBB_Center;
    memcpy( OBB_Axis, Header.OBB_Axis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, Header.OBB_Extent, sizeof(FLOAT) * 3 );	

	float fMagnitude  = _XFC_sqrt( (OBB_Center.x * OBB_Center.x) + (OBB_Center.y * OBB_Center.y) + (OBB_Center.z * OBB_Center.z) );
	float fMagnitude2 = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );	
	m_ObjectRadius = fMagnitude2;// fMagnitude + fMagnitude2;

	if(m_pExtraData)
	{
		_MOB_ATTRIBTABLE*pMobAttrTable = (_MOB_ATTRIBTABLE*)m_pExtraData;
		
		pMobAttrTable->xsize			= OBB_Extent[0];
		pMobAttrTable->ysize			= OBB_Extent[1];
		pMobAttrTable->collideradius	= m_ObjectRadius;
	}

#ifdef _XDWDEBUG

	CsuX3D_Box	obbbox;
	obbbox.Create( Header.OBB_Center, Header.OBB_Axis, Header.OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif
	
//	if( szFilename )
//		_XLog( "Loaded mesh file [%s]", szFilename );

	m_Initialized = true;

	return TRUE;
}


BOOL _XNSAnimMeshContainer::Load( FILE* Fileptr, _XTextureController* pTextureController, BOOL createsecondbuffer, LPSTR szFilename )
{
	if( !Fileptr )
	{
		_XFatalError("Invalid file pointer [%s] file", (szFilename) ? szFilename : "this" );
		return FALSE;
	}

	DestroyModel();
	
	if( !pTextureController )
	{
		return FALSE;
	}

	m_pTextureController= pTextureController;

	_XMESHHEADER   Header;	
	fread(&Header,	sizeof(_XMESHHEADER), 1, Fileptr);	
		
	if(Header.HeaderIndicator != _XDEF_X3DFILEINDICATOR )
	{
		_XFatalError("Not supported [%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE; 
	}

	if(Header.Fileversion != _XDEF_X3DFILEVERSION)
	{
		_XFatalError("Not supported [%s] file version [%f]",  (szFilename) ? szFilename : "this" , Header.Fileversion );		
		return FALSE; 		
	}

	int i,len;
	int NodeNum;	
	fread(&NodeNum,	sizeof(int), 1, Fileptr);	// read object count

	if( !CreateNode(NodeNum) ) return FALSE;		

	// Load model mesh data...	

	_LPXM_MESHOBJECT lpCh = NULL;
	TCHAR strName[128];

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		memset( strName, 0, 128 );
		if(len != 0) fread( strName, sizeof(TCHAR)*len, 1, Fileptr );

		// Load mesh object parent id...
		fread(&lpCh->nParentId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		
		if(len != 0) fseek(Fileptr, sizeof(TCHAR)*len, SEEK_CUR);

		// Load object matrix...
		fread(&lpCh->Matrix, sizeof(D3DXMATRIX), 1, Fileptr);
		lpCh->MatrixTM = lpCh->Matrix;

		// preprocess etc matrix...
		_XMeshMath_UnitMatrixFromMatrix(lpCh->Matrix);
		_XMeshMath_MatrixInvert( lpCh->Inverse, lpCh->Matrix);
		_XMeshMath_MatrixMultiply( lpCh->PosMatrixTM, lpCh->Inverse, lpCh->MatrixTM );

		D3DXMATRIX	mat;
		_XMeshMath_SetIdentityMatrix(mat);

		_LPXM_MESHOBJECT lpParent;
		if(lpCh->nParentId != -1)
		{
			lpParent = GetNode(lpCh->nParentId);
			_XMeshMath_MatrixMultiply(mat, lpCh->Matrix, lpParent->Inverse);
		}
		else mat = lpCh->Matrix;

		lpCh->LocalTM = mat;
		_XMeshMath_SetIdentityMatrix(lpCh->LocalAniTM);


		// Load texture id & setting texture index
		TCHAR strTextureName[64];
		fread(&len, sizeof(int), 1, Fileptr);
		memset(strTextureName, 0, sizeof(TCHAR)*64);
		if(len != 0)
		{
			fread(strTextureName, len, 1, Fileptr);

			lpCh->nTextureId = m_pTextureController->CreateTexture( strTextureName, g_TextureMipLevel_CharacterModel );
		}
		else
		{
			//_XFatalError( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			_XLog( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			lpCh->nTextureId = -1;
		}

		if( strncmp(strName, "Bip", 3 ) != 0 ) // Skipping biped object
		{
			// Load vertex buffer...
			fread(&lpCh->nVertexCnt, sizeof(lpCh->nVertexCnt), 1, Fileptr);
			if(lpCh->nVertexCnt != 0)
			{
				SAFE_RELEASE( lpCh->pVertex );
				SAFE_RELEASE( lpCh->pSecondVertexBuffer );
				
				if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), D3DUSAGE_WRITEONLY, 
					D3DFVF_XMESH_VERTEX, D3DPOOL_MANAGED, &(lpCh->pVertex), NULL )) ) 
				{
					_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}				

				//_XFatalError( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );
				
				_LPXMESH_VERTEX pVertices;
				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{					
					fread(pVertices, sizeof(_XMESH_VERTEX) * lpCh->nVertexCnt, 1, Fileptr);
					lpCh->pVertex->Unlock();
				}

				// Create second temporary buffer...
				if( createsecondbuffer )
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), D3DUSAGE_WRITEONLY, 
						D3DFVF_XMESH_VERTEX, D3DPOOL_MANAGED, &(lpCh->pSecondVertexBuffer), NULL )) )
					{
						_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
						return FALSE;
					}
					
					//_XFatalError( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
				}
			}

			// Load index buffer...
			fread(&lpCh->nIndexCnt, sizeof(lpCh->nIndexCnt), 1, Fileptr);
			SAFE_RELEASE( lpCh->pIndex );			
			
			if(lpCh->nIndexCnt != 0)
			{
				if( FAILED(gpDev->CreateIndexBuffer( lpCh->nIndexCnt*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
													 D3DPOOL_MANAGED, &(lpCh->pIndex), NULL )) )
				{
					_XFatalError("Create index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}					

				//_XFatalError( "Index buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pIndex );
				
				LPWORD pIndices;				
				if( SUCCEEDED( lpCh->pIndex->Lock( 0, lpCh->nIndexCnt*sizeof(WORD), (void**)&pIndices, D3DLOCK_NOSYSLOCK )) )
				{						
					fread(pIndices, sizeof(WORD) * lpCh->nIndexCnt, 1, Fileptr);
					lpCh->pIndex->Unlock();
				}
				else
				{
					_XFatalError("Lock index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}			
			}

			lpCh->nIndexCnt /= 3;

			// Read physique count
			fread(&lpCh->nKeyFactorCnt, sizeof(lpCh->nKeyFactorCnt), 1, Fileptr);

			if(lpCh->nKeyFactorCnt > 0)
			{
				lpCh->pKeyFactor = new _XM_KEYFACTOR[lpCh->nKeyFactorCnt];

				for(int j = 0; j < lpCh->nKeyFactorCnt; j++)
				{
					// Read linked vertex count for Physique...
					fread(&lpCh->pKeyFactor[j].iFactorCnt, sizeof(int), 1, Fileptr);
					
					if(lpCh->pKeyFactor[j].iFactorCnt == 0)
					{					
						_XFatalError( "Invalid physique informations\nvertex : %d, Physique number : %d",
																				  j, lpCh->pKeyFactor[j].iFactorCnt );
					}
					else
					{
						
						lpCh->pKeyFactor[j].pFactorData = new _XM_FACTOR[lpCh->pKeyFactor[j].iFactorCnt];

						// Read physique informations
						for(int k = 0; k < lpCh->pKeyFactor[j].iFactorCnt; k++)
							fread(&lpCh->pKeyFactor[j].pFactorData[k], sizeof(_XM_FACTOR), 1, Fileptr);
					}
				}

				fread(&lpCh->BoneIndexCnt, sizeof(int), 1, Fileptr);						
				fseek(Fileptr, sizeof(WORD)*lpCh->BoneIndexCnt , SEEK_CUR);
			}
		}		
	}

	//SetSkinning();

	OBB_Center = Header.OBB_Center;
    memcpy( OBB_Axis, Header.OBB_Axis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, Header.OBB_Extent, sizeof(FLOAT) * 3 );	

	float fMagnitude  = _XFC_sqrt( (OBB_Center.x * OBB_Center.x) + (OBB_Center.y * OBB_Center.y) + (OBB_Center.z * OBB_Center.z) );
	float fMagnitude2 = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );	
	m_ObjectRadius = fMagnitude2;// fMagnitude + fMagnitude2;

	if(m_pExtraData)
	{
		_MOB_ATTRIBTABLE*pMobAttrTable = (_MOB_ATTRIBTABLE*)m_pExtraData;
		
		pMobAttrTable->xsize			= OBB_Extent[0];
		pMobAttrTable->ysize			= OBB_Extent[1];
		pMobAttrTable->collideradius	= m_ObjectRadius;
	}

#ifdef _XDWDEBUG

	CsuX3D_Box	obbbox;
	obbbox.Create( Header.OBB_Center, Header.OBB_Axis, Header.OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif
	
//	if( szFilename )
//		_XLog( "Loaded mesh file [%s]", szFilename );
	
	m_Initialized = true;

	return TRUE;
}

BOOL _XNSAnimMeshContainer::Load_Runtime( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer, LPSTR szFilename )
{
	if( !Fileptr )
	{
		_XFatalError("Invalid file pointer [%s] file", (szFilename) ? szFilename : "this" );
		return FALSE;
	}

#ifdef _XDEF_MODELTHREADLOADING
#else
	DestroyModel();
#endif
	
	if( !pTextureManager )
	{
		return FALSE;
	}

	m_pTextureManager = pTextureManager;

	_XMESHHEADER   Header;	
	fread(&Header,	sizeof(_XMESHHEADER), 1, Fileptr);	
		
	if(Header.HeaderIndicator != _XDEF_X3DFILEINDICATOR )
	{
		_XFatalError("Not supported [%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE; 
	}

	if(Header.Fileversion != _XDEF_X3DFILEVERSION)
	{
		_XFatalError("Not supported [%s] file version [%f]",  (szFilename) ? szFilename : "this" , Header.Fileversion );		
		return FALSE; 		
	}

	int i,len;
	int NodeNum;	
	fread(&NodeNum,	sizeof(int), 1, Fileptr);	// read object count

	if( !CreateNode(NodeNum) ) return FALSE;		

	// Load model mesh data...	

	_LPXM_MESHOBJECT lpCh = NULL;
	TCHAR strName[128];

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		memset( strName, 0, 128 );
		if(len != 0) fread( strName, sizeof(TCHAR)*len, 1, Fileptr );

		// Load mesh object parent id...
		fread(&lpCh->nParentId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		
		if(len != 0) fseek(Fileptr, sizeof(TCHAR)*len, SEEK_CUR);

		// Load object matrix...
		fread(&lpCh->Matrix, sizeof(D3DXMATRIX), 1, Fileptr);
		lpCh->MatrixTM = lpCh->Matrix;

		// preprocess etc matrix...
		_XMeshMath_UnitMatrixFromMatrix(lpCh->Matrix);
		_XMeshMath_MatrixInvert( lpCh->Inverse, lpCh->Matrix);
		_XMeshMath_MatrixMultiply( lpCh->PosMatrixTM, lpCh->Inverse, lpCh->MatrixTM );

		D3DXMATRIX	mat;
		_XMeshMath_SetIdentityMatrix(mat);

		_LPXM_MESHOBJECT lpParent;
		if(lpCh->nParentId != -1)
		{
			lpParent = GetNode(lpCh->nParentId);
			_XMeshMath_MatrixMultiply(mat, lpCh->Matrix, lpParent->Inverse);
		}
		else mat = lpCh->Matrix;

		lpCh->LocalTM = mat;
		_XMeshMath_SetIdentityMatrix(lpCh->LocalAniTM);


		// Load texture id & setting texture index
		TCHAR strTextureName[64];
		fread(&len, sizeof(int), 1, Fileptr);
		memset(strTextureName, 0, sizeof(TCHAR)*64);
		if(len != 0)
		{
			fread(strTextureName, len, 1, Fileptr);
			lpCh->nTextureId = m_pTextureManager->FindResource( strTextureName );
		}
		else
		{
			//_XFatalError( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			_XLog( "Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			lpCh->nTextureId = -1;
		}

		if( strncmp(strName, "Bip", 3 ) != 0 ) // Skipping biped object
		{
			// Load vertex buffer...
			fread(&lpCh->nVertexCnt, sizeof(lpCh->nVertexCnt), 1, Fileptr);
			if(lpCh->nVertexCnt != 0)
			{
				SAFE_RELEASE( lpCh->pVertex );
				SAFE_RELEASE( lpCh->pSecondVertexBuffer );
				
				if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), D3DUSAGE_WRITEONLY, 
					D3DFVF_XMESH_VERTEX, D3DPOOL_MANAGED, &(lpCh->pVertex), NULL )) ) 
				{
					_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}				

				//_XLog( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );
				
				_LPXMESH_VERTEX pVertices;
				if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
				{					
					fread(pVertices, sizeof(_XMESH_VERTEX) * lpCh->nVertexCnt, 1, Fileptr);
					lpCh->pVertex->Unlock();
				}

				// Create second temporary buffer...
				if( createsecondbuffer )
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), D3DUSAGE_WRITEONLY, 
						D3DFVF_XMESH_VERTEX, D3DPOOL_MANAGED, &(lpCh->pSecondVertexBuffer), NULL )) )
					{
						_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
						return FALSE;
					}
					
					//_XLog( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
				}
			}

			// Load index buffer...
			fread(&lpCh->nIndexCnt, sizeof(lpCh->nIndexCnt), 1, Fileptr);
			SAFE_RELEASE( lpCh->pIndex );			
			
			if(lpCh->nIndexCnt != 0)
			{
				if( FAILED(gpDev->CreateIndexBuffer( lpCh->nIndexCnt*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
													 D3DPOOL_MANAGED, &(lpCh->pIndex), NULL )) )
				{
					_XFatalError("Create index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}					

				//_XLog( "Index buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pIndex );
				
				LPWORD pIndices;				
				if( SUCCEEDED( lpCh->pIndex->Lock( 0, lpCh->nIndexCnt*sizeof(WORD), (void**)&pIndices, D3DLOCK_NOSYSLOCK )) )
				{						
					fread(pIndices, sizeof(WORD) * lpCh->nIndexCnt, 1, Fileptr);
					lpCh->pIndex->Unlock();
				}
				else
				{
					_XFatalError("Lock index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}			
			}

			lpCh->nIndexCnt /= 3;

			// Read physique count
			fread(&lpCh->nKeyFactorCnt, sizeof(lpCh->nKeyFactorCnt), 1, Fileptr);

			if(lpCh->nKeyFactorCnt > 0)
			{
				lpCh->pKeyFactor = new _XM_KEYFACTOR[lpCh->nKeyFactorCnt];

				for(int j = 0; j < lpCh->nKeyFactorCnt; j++)
				{
					// Read linked vertex count for Physique...
					fread(&lpCh->pKeyFactor[j].iFactorCnt, sizeof(int), 1, Fileptr);
					
					if(lpCh->pKeyFactor[j].iFactorCnt == 0)
					{					
						_XFatalError( "Invalid physique informations\nvertex : %d, Physique number : %d",
																				  j, lpCh->pKeyFactor[j].iFactorCnt );
					}
					else
					{
						
						lpCh->pKeyFactor[j].pFactorData = new _XM_FACTOR[lpCh->pKeyFactor[j].iFactorCnt];

						// Read physique informations
						for(int k = 0; k < lpCh->pKeyFactor[j].iFactorCnt; k++)
							fread(&lpCh->pKeyFactor[j].pFactorData[k], sizeof(_XM_FACTOR), 1, Fileptr);
					}
				}

				fread(&lpCh->BoneIndexCnt, sizeof(int), 1, Fileptr);						
				fseek(Fileptr, sizeof(WORD)*lpCh->BoneIndexCnt , SEEK_CUR);
			}
		}		
	}

	//SetSkinning();

	OBB_Center = Header.OBB_Center;
    memcpy( OBB_Axis, Header.OBB_Axis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, Header.OBB_Extent, sizeof(FLOAT) * 3 );	

	float fMagnitude  = _XFC_sqrt( (OBB_Center.x * OBB_Center.x) + (OBB_Center.y * OBB_Center.y) + (OBB_Center.z * OBB_Center.z) );
	float fMagnitude2 = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );	
	m_ObjectRadius = fMagnitude2;// fMagnitude + fMagnitude2;

	if(m_pExtraData)
	{
		_MOB_ATTRIBTABLE*pMobAttrTable = (_MOB_ATTRIBTABLE*)m_pExtraData;
		
		pMobAttrTable->xsize			= OBB_Extent[0];
		pMobAttrTable->ysize			= OBB_Extent[1];
		pMobAttrTable->collideradius	= m_ObjectRadius;
	}

#ifdef _XDWDEBUG

	CsuX3D_Box	obbbox;
	obbbox.Create( Header.OBB_Center, Header.OBB_Axis, Header.OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif
	
//	if( szFilename )
//		_XLog( "Loaded mesh file [%s]", szFilename );

	m_Initialized = true;
	
	return TRUE;
}

// by mahwang
BOOL _XNSAnimMeshContainer::InitializeSecondBufferFormTM( _LPXM_KEYFRAME lpCurKeyframe)
{
	_LPXM_MESHOBJECT	lpCh;
	D3DXMATRIX			d3dMatrix, mat;        
	_XMESH_VERTEX		d3dxTmpVec;
	_XMESH_VERTEX		d3dxTmpVec2;

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{			
		lpCh = GetNode(objindex );

		if(lpCh->nVertexCnt != 0)
		{
			D3DXMatrixInverse(&d3dMatrix, NULL, &lpCurKeyframe->lpSubNode[objindex].matTM);

			_LPXMESH_VERTEX pVertices, plVertices;
			
			lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK );
			lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, D3DLOCK_NOSYSLOCK );			
			
			for( int nIdx = 0 ; nIdx < lpCh->nVertexCnt ; nIdx ++ )
			{
				d3dxTmpVec2 = d3dxTmpVec = pVertices[ nIdx ];
				
				// translate postition...
				d3dxTmpVec2.x	=	d3dxTmpVec.x * d3dMatrix._11 +
									d3dxTmpVec.y * d3dMatrix._21 +
									d3dxTmpVec.z * d3dMatrix._31 + d3dMatrix._41 ;
				d3dxTmpVec2.y	=	d3dxTmpVec.x * d3dMatrix._12 +
									d3dxTmpVec.y * d3dMatrix._22 +
									d3dxTmpVec.z * d3dMatrix._32 + d3dMatrix._42 ;
				d3dxTmpVec2.z	=	d3dxTmpVec.x * d3dMatrix._13 +
									d3dxTmpVec.y * d3dMatrix._23 +
									d3dxTmpVec.z * d3dMatrix._33 + d3dMatrix._43 ;

				// rotate normal vector...
				d3dxTmpVec2.nx	=	d3dxTmpVec.nx * d3dMatrix._11 +
									d3dxTmpVec.ny * d3dMatrix._21 +
									d3dxTmpVec.nz * d3dMatrix._31;

				d3dxTmpVec2.ny	=	d3dxTmpVec.nx * d3dMatrix._12 +
									d3dxTmpVec.ny * d3dMatrix._22 +
									d3dxTmpVec.nz * d3dMatrix._32;

				d3dxTmpVec2.nz	=	d3dxTmpVec.nx * d3dMatrix._13 +
									d3dxTmpVec.ny * d3dMatrix._23 +
									d3dxTmpVec.nz * d3dMatrix._33;
				
				plVertices[ nIdx ] = d3dxTmpVec2;
			}			

			lpCh->pVertex->Unlock();
			lpCh->pSecondVertexBuffer->Unlock();
		}
	}

	return TRUE;
}

// by mahwang
void _XNSAnimMeshContainer::Render( D3DXMATRIX* pmatKeyframe )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
		
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;

		gpDev->SetTransform( D3DTS_WORLD, &pmatKeyframe[i] );

		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
//		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
}

void _XNSAnimMeshContainer::RenderNoTexture( D3DXMATRIX* pmatKeyframe )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
		
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;

		gpDev->SetTransform( D3DTS_WORLD, &pmatKeyframe[i] );

//		m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		
//		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
}

void _XNSAnimMeshContainer::Render( D3DXMATRIX worldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmat );
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{	
		if(m_Node[i].nVertexCnt == 0) continue;
		
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );			
	}
}

void _XNSAnimMeshContainer::RenderNoTexture( D3DXMATRIX worldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmat );
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{	
		if(m_Node[i].nVertexCnt == 0) continue;
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
}

void _XNSAnimMeshContainer::RenderNoTexture( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	if( pworldmat )
		gpDev->SetTransform( D3DTS_WORLD, pworldmat );
	else
		gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			
			gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
	else
	{			
		for(int i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );			
		}
	}
}

void _XNSAnimMeshContainer::Render_LowLevelLOD( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{

	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData) return;
	
	if( m_pLODObjectList )
	{
		if( pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL1 )
		{
			if( m_pLODObjectList[1] && m_pLODObjectCount == 2 && pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL2 )
			{
				m_pLODObjectList[1]->Render_LowLevelLOD( pAniData, pworldmat );				
				return;
			}			
			
			m_pLODObjectList[0]->Render_LowLevelLOD( pAniData, pworldmat );
			return;
		}
	}
	
	if( pworldmat )
		gpDev->SetTransform( D3DTS_WORLD, pworldmat );
	else
		gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			
			if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
			gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
	else
	{			
		for(int i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			
			if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );			
		}
	}
}

void _XNSAnimMeshContainer::Render_LODCheck( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(!pAniData) return;
	
	if( m_pLODObjectList )
	{
		if( pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL1 )
		{
			if( m_pLODObjectList[1] && m_pLODObjectCount == 2 && pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL2 )
			{
				m_pLODObjectList[1]->Render( pAniData, pworldmat );
				return;
			}			
			
			m_pLODObjectList[0]->Render( pAniData, pworldmat );
			return;
		}
	}

	if( pworldmat )
		gpDev->SetTransform( D3DTS_WORLD, pworldmat );
	else
		gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			
			g_SetTextureFailed = FALSE;
			if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
				
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}
		}
	}
	else
	{			
		for(int i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;

			g_SetTextureFailed = FALSE;			
			if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
				
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );			
			}
		}
	}
}

void _XNSAnimMeshContainer::Render( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	if( pworldmat )
		gpDev->SetTransform( D3DTS_WORLD, pworldmat );
	else
		gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );

	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;

			g_SetTextureFailed = FALSE;		
			if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
							
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}
		}
	}
	else
	{			
		for(int i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;

			g_SetTextureFailed = FALSE;		
			if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
										
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );			
			}
		}
	}
}

void _XNSAnimMeshContainer::RenderCustomDraw( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;	
	
	if( pworldmat )
		gpDev->SetTransform( D3DTS_WORLD, pworldmat );
	else
		gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{
			if(m_Node[i].nVertexCnt == 0) continue;			
			if (m_pTextureManager) m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture_NoRenderStyle( m_Node[i].nTextureId );
			gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );		
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
	else
	{
		for(int i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			if (m_pTextureManager) m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );
			if (m_pTextureController) m_pTextureController->SetTexture_NoRenderStyle( m_Node[i].nTextureId );
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );		
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}	
	}
}

void _XNSAnimMeshContainer::Render_Reflect( _XModelDescriptor* pAniData, D3DXMATRIX* pReflectViewMatrix )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{				
		if(m_Node[i].nVertexCnt == 0) continue;
		
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}
}

void _XNSAnimMeshContainer::Render_SecondBuffer( _XModelDescriptor* pAniData )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{				
		if(m_Node[i].nVertexCnt == 0) continue;
		
		g_SetTextureFailed = FALSE;
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );

		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
}

void _XNSAnimMeshContainer::RenderShadow_LODCheck( _XModelDescriptor* pAniData )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if( m_pLODObjectList )
	{
		if( pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL1 )
		{
			if( m_pLODObjectList[1] && m_pLODObjectCount == 2 && pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL2 )
			{
				m_pLODObjectList[1]->RenderShadow( pAniData );
				return;
			}			
			
			m_pLODObjectList[0]->RenderShadow( pAniData );
			return;
		}
	}

	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( m_NodeCnt > 0)
	{					
		for(int i = 0; i < m_NodeCnt; i++)
		{
			if( m_Node[i].nVertexCnt )
			{	
				gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}			
		}	
	}
}

void _XNSAnimMeshContainer::RenderShadow( _XModelDescriptor* pAniData )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );

	if( m_NodeCnt > 0)
	{					
		for(int i = 0; i < m_NodeCnt; i++)
		{
			if( m_Node[i].nVertexCnt )
			{	
				gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}			
		}	
	}
}

void _XNSAnimMeshContainer::RenderInterfaceItem(D3DXMATRIX* rotmat)
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	D3DXMATRIX	curmat;
	
	int i;		
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;
		
		D3DXMatrixIdentity(&curmat);
		D3DXMatrixMultiply(&curmat, &curmat, &m_Node[i].Inverse);
		
		if(rotmat) D3DXMatrixMultiply( &curmat, &curmat, rotmat );
		
		
		gpDev->SetTransform( D3DTS_WORLD, &curmat );
		
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
		gpDev->SetRenderState( D3DRS_ALPHAREF, _XDEF_INTERFACEMODELALPHAREF );
		gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
		gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );			
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}	
}

void _XNSAnimMeshContainer::Render_OriginalPos( D3DXVECTOR3& offset, int index, D3DXMATRIX* rotmat, BOOL usealphatest  )
{
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if(m_Node == NULL) return;
	
	D3DXMATRIX	curmat;	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(int i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;
		
		D3DXMatrixIdentity(&curmat);
		D3DXMatrixMultiply(&curmat, &curmat, &m_Node[i].Inverse);
		if(rotmat) D3DXMatrixMultiply( &curmat, &curmat, rotmat );
		
		curmat._41 += offset.x;
		curmat._42 += offset.y;
		curmat._43 += offset.z;
		gpDev->SetTransform( D3DTS_WORLD, &curmat );
		
		if (m_pTextureManager) m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if (m_pTextureController) m_pTextureController->SetTexture( m_Node[i].nTextureId );
		
		if( usealphatest )
		{
			gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
			gpDev->SetRenderState( D3DRS_ALPHAREF, _XDEF_INTERFACEMODELALPHAREF );
			gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
			gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );
		}
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );		
	}	
}

void _XNSAnimMeshContainer::Animation( _XModelDescriptor* pAniData )
{
/*	if( !pAniData ) return;
	if( !pAniData->m_pBipedController ) return;
	if( !pAniData->m_AniFlag || pAniData->m_pBipedController->m_MotionCnt == 0) return;*/
		
#ifdef _XDEF_USESLERPANIMATION	
	if(pAniData->m_MotionChangeFlag)
	{
		SlerpAnimation(pAniData);
	}
	else
#endif

	{
		FrameAnimation(pAniData);
	}

	//Skinning();	
}

void _XNSAnimMeshContainer::FrameAnimation( _XModelDescriptor* pAniData )
{	
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if( !pAniData->m_pBipedController ) return;
	
	if( m_pLODObjectList )
	{
		if( pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL1 )
		{
			if( m_pLODObjectList[1] && m_pLODObjectCount == 2 && pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL2 )
			{
				m_pLODObjectList[1]->FrameAnimation( pAniData );
				return;
			}			
			
			m_pLODObjectList[0]->FrameAnimation( pAniData );
			return;
		}
	}

	_LPXM_MESHOBJECT	lpCh;
	_LPXM_BIPEDOBJECT	lpParent;
	D3DXMATRIX			d3dMatrix, mat;        
	_XMESH_VERTEX		d3dxTmpVec;
	_XMESH_VERTEX		d3dxTmpVec2;

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{			
		lpCh = GetNode(objindex );
		lpParent = pAniData->m_pBipedController->GetNode(lpCh->nParentId);	

		if(lpCh->nVertexCnt != 0)
		{
			_LPXMESH_VERTEX pVertices, plVertices;
			
			lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK );
			lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, D3DLOCK_NOSYSLOCK );			
			
			if( !pVertices || !plVertices )
			{
				_XLog( "Error : Failed lock vertex buffer [%p,%p]", pVertices, plVertices );
			}
			else
			{			
				for( int nIdx = 0 ; nIdx < lpCh->nVertexCnt ; nIdx ++ )
				{
					// Check physique animation info...
					if( lpCh->pKeyFactor != NULL ) // process physique data...
					{
						ZeroMemory(&d3dMatrix, sizeof(d3dMatrix));
						d3dMatrix._44 = 1.0f;

						for(int factor = 0; factor < lpCh->pKeyFactor[nIdx].iFactorCnt; factor++)
						{
							_XM_FACTOR keyfactor = lpCh->pKeyFactor[nIdx].pFactorData[factor];
						
							lpParent = pAniData->m_pBipedController->GetNode(keyfactor.wKeyIdx);
							
							d3dMatrix._11 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._11;
							d3dMatrix._12 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._12;
							d3dMatrix._13 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._13;

							d3dMatrix._21 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._21;
							d3dMatrix._22 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._22;
							d3dMatrix._23 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._23;

							d3dMatrix._31 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._31;
							d3dMatrix._32 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._32;
							d3dMatrix._33 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._33;

							d3dMatrix._41 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._41;
							d3dMatrix._42 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._42;
							d3dMatrix._43 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._43;
						}
					}
					else // process biped...
					{
						// 현재 오브젝트는 로컬이 아니라. 월드로 되어있기 때문에..
						// 인버스에 곱해줘서.. 매트릭스를 로컬로 변경해 줘야 한다..
						// 곱하는 순서에 유의..
						D3DXMatrixMultiply(&d3dMatrix, &lpCh->Inverse, &lpCh->LocalAniTM);
					}
					
					d3dxTmpVec2 = d3dxTmpVec = pVertices[ nIdx ];
					
					// translate postition...
					d3dxTmpVec2.x	=	d3dxTmpVec.x * d3dMatrix._11 +
										d3dxTmpVec.y * d3dMatrix._21 +
										d3dxTmpVec.z * d3dMatrix._31 + d3dMatrix._41 ;
					d3dxTmpVec2.y	=	d3dxTmpVec.x * d3dMatrix._12 +
										d3dxTmpVec.y * d3dMatrix._22 +
										d3dxTmpVec.z * d3dMatrix._32 + d3dMatrix._42 ;
					d3dxTmpVec2.z	=	d3dxTmpVec.x * d3dMatrix._13 +
										d3dxTmpVec.y * d3dMatrix._23 +
										d3dxTmpVec.z * d3dMatrix._33 + d3dMatrix._43 ;

					// rotate normal vector...
					d3dxTmpVec2.nx	=	d3dxTmpVec.nx * d3dMatrix._11 +
										d3dxTmpVec.ny * d3dMatrix._21 +
										d3dxTmpVec.nz * d3dMatrix._31;

					d3dxTmpVec2.ny	=	d3dxTmpVec.nx * d3dMatrix._12 +
										d3dxTmpVec.ny * d3dMatrix._22 +
										d3dxTmpVec.nz * d3dMatrix._32;

					d3dxTmpVec2.nz	=	d3dxTmpVec.nx * d3dMatrix._13 +
										d3dxTmpVec.ny * d3dMatrix._23 +
										d3dxTmpVec.nz * d3dMatrix._33;
					
					plVertices[ nIdx ] = d3dxTmpVec2;
					
				}			

				lpCh->pVertex->Unlock();
				lpCh->pSecondVertexBuffer->Unlock();
			}
		}
	}
	
	pAniData->m_AniLastFrame = pAniData->m_AniFrame;		
}

void _XNSAnimMeshContainer::SlerpAnimation( _XModelDescriptor* pAniData )
{	
	m_LastUsedTime = g_LocalSystemTime;

#ifdef _XDEF_MODELTHREADLOADING
	if( !m_Initialized )
	{
		if( !m_ThreadLoad )
			g_LoadMeshThread.LoadMeshContainer( this, m_pTextureManager, !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation );		
		return;
	}
#endif

	if( !pAniData->m_pBipedController ) return;
	
	if( m_pLODObjectList )
	{
		if( pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL1 )
		{
			if( m_pLODObjectList[1] && m_pLODObjectCount == 2 && pAniData->m_CameraDistance > _XDEF_CHARACTERDISTANCE_LODLEVEL2 )
			{
				m_pLODObjectList[1]->SlerpAnimation( pAniData );
				return;
			}			
			
			m_pLODObjectList[0]->SlerpAnimation( pAniData );
			return;
		}
	}

	if( pAniData->m_SlerpAniFrame >= _XDEF_SLERPANIMATIONTIME)
	{
		_LPXM_MOTION  pMotion = pAniData->m_pBipedController->GetMotion( pAniData->m_CurMotion );
		pAniData->m_AniFrame = (FLOAT)pMotion->StartFrame;
		pAniData->m_MotionChangeFlag = false;
	}

	_LPXM_MESHOBJECT	lpCh;
	_LPXM_BIPEDOBJECT	lpParent;	
	D3DXMATRIX			d3dMatrix, mat;
	_XMESH_VERTEX		d3dxTmpVec;
	_XMESH_VERTEX		d3dxTmpVec2;

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{
		lpCh		= GetNode(objindex);
		lpParent	= pAniData->m_pBipedController->GetNode(lpCh->nParentId);
				
        D3DXMatrixIdentity(&d3dMatrix);

		if(lpCh->nVertexCnt != 0)
		{
			_LPXMESH_VERTEX pVertices, plVertices;
			
			lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK );
			lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, D3DLOCK_NOSYSLOCK );			
			
			if( !pVertices || !plVertices )
			{
				_XLog( "Error : Failed lock vertex buffer [%p,%p]", pVertices, plVertices );
			}
			else
			{
				for( int nIdx = 0 ; nIdx < lpCh->nVertexCnt ; nIdx ++ )
				{
					// Check physique animation info...
					if( lpCh->pKeyFactor != NULL ) // process physique data...
					{
						ZeroMemory(&d3dMatrix, sizeof(d3dMatrix));
						d3dMatrix._44 = 1.0f;

						for(int factor = 0; factor < lpCh->pKeyFactor[nIdx].iFactorCnt; factor++)
						{
							_XM_FACTOR keyfactor = lpCh->pKeyFactor[nIdx].pFactorData[factor];
						
							lpParent = pAniData->m_pBipedController->GetNode(keyfactor.wKeyIdx);
							//D3DXMatrixMultiply(&mat, &lpParent->Inverse, &lpParent->LocalAniTM);

							d3dMatrix._11 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._11;
							d3dMatrix._12 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._12;
							d3dMatrix._13 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._13;

							d3dMatrix._21 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._21;
							d3dMatrix._22 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._22;
							d3dMatrix._23 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._23;

							d3dMatrix._31 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._31;
							d3dMatrix._32 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._32;
							d3dMatrix._33 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._33;

							d3dMatrix._41 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._41;
							d3dMatrix._42 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._42;
							d3dMatrix._43 += keyfactor.fBlendFactor*lpParent->m_matLocalAni._43;
						}
					}
					else // process biped animation
					{
						// 현재 오브젝트는 로컬이 아니라. 월드로 되어있기 때문에..
						// 인버스에 곱해줘서.. 매트릭스를 로컬로 변경해 줘야 한다..
						// 곱하는 순서에 유의..
						D3DXMatrixMultiply(&d3dMatrix, &lpCh->Inverse, &lpCh->LocalAniTM);
					}
					
					d3dxTmpVec2 = d3dxTmpVec = pVertices[ nIdx ];
					
					// translate postition...
					d3dxTmpVec2.x	=	d3dxTmpVec.x * d3dMatrix._11 +
										d3dxTmpVec.y * d3dMatrix._21 +
										d3dxTmpVec.z * d3dMatrix._31 + d3dMatrix._41 ;
					d3dxTmpVec2.y	=	d3dxTmpVec.x * d3dMatrix._12 +
										d3dxTmpVec.y * d3dMatrix._22 +
										d3dxTmpVec.z * d3dMatrix._32 + d3dMatrix._42 ;
					d3dxTmpVec2.z	=	d3dxTmpVec.x * d3dMatrix._13 +
										d3dxTmpVec.y * d3dMatrix._23 +
										d3dxTmpVec.z * d3dMatrix._33 + d3dMatrix._43 ;

					// rotate normal vector...
					d3dxTmpVec2.nx	=	d3dxTmpVec.nx * d3dMatrix._11 +
										d3dxTmpVec.ny * d3dMatrix._21 +
										d3dxTmpVec.nz * d3dMatrix._31;

					d3dxTmpVec2.ny	=	d3dxTmpVec.nx * d3dMatrix._12 +
										d3dxTmpVec.ny * d3dMatrix._22 +
										d3dxTmpVec.nz * d3dMatrix._32;

					d3dxTmpVec2.nz	=	d3dxTmpVec.nx * d3dMatrix._13 +
										d3dxTmpVec.ny * d3dMatrix._23 +
										d3dxTmpVec.nz * d3dMatrix._33;
					
					plVertices[ nIdx ] = d3dxTmpVec2;
				}

				lpCh->pVertex->Unlock();
				lpCh->pSecondVertexBuffer->Unlock();
			}
		}
	}	
}

void _XNSAnimMeshContainer::RebuildOBBInfo( void )
{
	_LPXM_MESHOBJECT lpCh = NULL;
	
	CsuX3D_Box Box;
	CsuX3D_Box MergedBox;
	_XMESH_VERTEX*	pVertices;

	for(int i = 0; i < m_NodeCnt; i++)
	{
		lpCh = GetNode(i);
		
		if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK ) ) )
		{
			// compute mean of points
			D3DXVECTOR3 m( 0, 0, 0 );
			for( int i = 0; i < lpCh->nVertexCnt; ++i )
			{
				m.x += pVertices[i].x;
				m.y += pVertices[i].y;
				m.z += pVertices[i].z;
			}
			m /= (float)lpCh->nVertexCnt;
			
			
			// compute covariances of points
			float C11 = 0, C22 = 0, C33 = 0, C12 = 0, C13 = 0,  C23 = 0;
			for( i = 0; i < lpCh->nVertexCnt; ++i )
			{
				C11 += ( pVertices[i].x - m.x ) * ( pVertices[i].x - m.x );
				C22 += ( pVertices[i].y - m.y ) * ( pVertices[i].y - m.y );
				C33 += ( pVertices[i].z - m.z ) * ( pVertices[i].z - m.z );
				C12 += ( pVertices[i].x - m.x ) * ( pVertices[i].y - m.y );
				C13 += ( pVertices[i].x - m.x ) * ( pVertices[i].z - m.z );
				C23 += ( pVertices[i].y - m.y ) * ( pVertices[i].z - m.z );
			}
			C11 /= lpCh->nVertexCnt;
			C22 /= lpCh->nVertexCnt;
			C33 /= lpCh->nVertexCnt;
			C12 /= lpCh->nVertexCnt;
			C13 /= lpCh->nVertexCnt;
			C23 /= lpCh->nVertexCnt;
			
			// compute eigenvectors for covariance matrix
			float Matrix[3][3] = 
			{
				{ C11, C12, C13 },
				{ C12, C22, C23 },
				{ C13, C23, C33 },
			};
			
			float EigenVectors[3][3];
			float EigenValue[3];
			Jacobi( Matrix, EigenVectors, EigenValue );
			
			
			struct SORT{
				int ID;
				float Value;
			} Sort[3] = { { 0, EigenValue[0] }, { 1, EigenValue[1] }, { 2, EigenValue[2] } };
			
			
			// 축 구하기 
			for(i = 0; i < 3; ++i )
			{
				Box.m_akAxis[i].x = EigenVectors[0][Sort[i].ID];
				Box.m_akAxis[i].y = EigenVectors[1][Sort[i].ID];
				Box.m_akAxis[i].z = EigenVectors[2][Sort[i].ID];
			}
			
			float min[3] = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
			float max[3] = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			for( int j = 0; j < 3; ++j )
			{
				for( int i = 0; i < lpCh->nVertexCnt; ++i )
				{
					// dot product...
					float a = pVertices[i].x * Box.m_akAxis[j].x + pVertices[i].y * Box.m_akAxis[j].y + pVertices[i].z * Box.m_akAxis[j].z;
					if( min[j] > a ) min[j] = a;
					if( max[j] < a ) max[j] = a;
				}
			}
			
			// 중심 구하기 
			Box.m_kCenter = Box.m_akAxis[0] * ( ( min[0] + max[0] ) * 0.5f )
						  + Box.m_akAxis[1] * ( ( min[1] + max[1] ) * 0.5f )
						  + Box.m_akAxis[2] * ( ( min[2] + max[2] ) * 0.5f );
			
			// 길이 구하기
			for(  i = 0; i < 3; ++i ) 
				Box.m_afExtent[i] = 0.5f*(max[i] - min[i]);

			lpCh->pVertex->Unlock();

			if( i == 0 )
			{
				MergedBox = Box;
			}
			else
			{
				if(MergedBox.m_kMin.x > Box.m_kMin.x) MergedBox.m_kMin.x =  Box.m_kMin.x;				
				if(MergedBox.m_kMin.y > Box.m_kMin.y) MergedBox.m_kMin.y =  Box.m_kMin.y;				
				if(MergedBox.m_kMin.z > Box.m_kMin.z) MergedBox.m_kMin.z =  Box.m_kMin.z;				
				if(MergedBox.m_kMax.x < Box.m_kMax.x) MergedBox.m_kMax.x =  Box.m_kMax.x;				
				if(MergedBox.m_kMax.y < Box.m_kMax.y) MergedBox.m_kMax.y =  Box.m_kMax.y;				
				if(MergedBox.m_kMax.z < Box.m_kMax.z) MergedBox.m_kMax.z =  Box.m_kMax.z;				
				
				MergedBox.m_kCenter.x = (MergedBox.m_kMin.x + MergedBox.m_kMax.x)/2;
				MergedBox.m_kCenter.y = (MergedBox.m_kMin.y + MergedBox.m_kMax.y)/2;
				MergedBox.m_kCenter.z = (MergedBox.m_kMin.z + MergedBox.m_kMax.z)/2;
			}
		}
	}

	OBB_Center = MergedBox.m_kCenter;
	memcpy( OBB_Axis, MergedBox.m_akAxis, sizeof(D3DXVECTOR3) * 3 );
	memcpy( OBB_Extent, MergedBox.m_afExtent, sizeof(FLOAT) * 3 );

#ifdef _XDWDEBUG
	CsuX3D_Box	obbbox;
	obbbox.Create( OBB_Center, OBB_Axis, OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
#endif
}