// XMeshContainer.cpp: implementation of the _XMeshContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XBipedController.h"
#include "XMeshContainer.h"
#include "XObjectManager.h"

#define _RENDEROBJECT_POOL_

extern BOOL g_SetTextureFailed; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void GetFileNameFromPathString( TCHAR *value, TCHAR *line )
{
	int start = 0, end = 0;
	
	// 92(\), 46(.)
	for( int i = 0; i < (int)strlen(line); i++)
	{
		if(line[i] == 92)
			start = i+1;
	}
	for( i = start; i < (int)strlen(line); i++)
	{
		if(line[i] == 46)
		{
			end = i+4;
			break;
		}
	}
	
	int b = 0;
	TCHAR buff[256];
	memset(buff, 1, 256);
	for(i = start; i < end; i++)
		buff[b++] = line[i];
	
	buff[b] = '\0';
	
	strcpy( value, buff ); 
}

_XMeshContainer::_XMeshContainer()
{	
	m_NodeCnt				= 0;
	m_Node					= NULL;			
	m_pTextureManager		= NULL;
	m_ObjectRadius			= 0;

	m_bBakemap				= FALSE;	
	m_TexturePreCached		= FALSE;
}

_XMeshContainer::~_XMeshContainer()
{
	DestroyModel();
}

void _XMeshContainer::DestroyModel( void )
{		
	m_NodeCnt			=	0;			
	m_ObjectRadius		=	0;
	SAFE_DELETE_ARRAY(m_Node);	
}


BOOL _XMeshContainer::CreateNode(int nodecount)
{
	SAFE_DELETE_ARRAY( m_Node );			
	m_NodeCnt			= 0;
		
	m_Node = new _XM_MESHOBJECT[nodecount];
	if( !m_Node )
	{
		_XFatalError( "XModelcontainer :: CreateNode<%d> memory allocation", nodecount );
		return FALSE;
	}	
	
	m_NodeCnt = nodecount;

	return TRUE;
}

void _XMeshContainer::DeleteNode()
{	
	SAFE_DELETE_ARRAY( m_Node );			
	m_NodeCnt		   = 0;	
}

_LPXM_MESHOBJECT _XMeshContainer::GetNode(int index )
{ 	
	if(m_Node == NULL || m_NodeCnt == 0 || index < 0) return NULL;
	return &m_Node[ index ];
}


BOOL _XMeshContainer::Load( LPSTR szFilename, _XTextureManager* pTextureManager, BOOL createsecondbuffer )
{
	assert( szFilename );

	FILE*	Fileptr = NULL;

	_XSetDefaultPath(_XDEF_DEFAULTPATH_MODEL);
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

BOOL _XMeshContainer::Load( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer, LPSTR szFilename )
{
	assert( pTextureManager );
	assert( Fileptr );
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

	if(Header.Fileversion != _XDEF_X3DFILEVERSION && Header.Fileversion != _XDEF_X3DFILEVERSION_0_73)
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
	TCHAR strName[_MAX_PATH];

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		memset( strName, 0, _MAX_PATH );
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
			lpCh->nTextureId = m_pTextureManager->GetResourceIndex( strTextureName, g_TextureMipLevel_Object );
		}
		else
		{
			_XDWINPRINT( "WARNING : Invalid texture id [%s:%s:%d]", (szFilename) ? szFilename : "this" ,strName, i );
			lpCh->nTextureId = -1;
		}

		if (Header.Fileversion >= _XDEF_X3DFILEVERSION_0_73)
		{
			TCHAR strBakemapName[64];
			fread(&len, sizeof(int), 1, Fileptr);
			memset(strBakemapName, 0, sizeof(TCHAR)*64);
			if(len != 0)
			{
				fread(strBakemapName, len, 1, Fileptr);

				lpCh->bBakemap = TRUE;
				m_bBakemap = TRUE;
				lpCh->nBakemapId = m_pTextureManager->GetResourceIndex( strBakemapName, g_TextureMipLevel_Object );
			}
		}

		if( strncmp(strName, "Bip", 3 ) != 0 ) // Skipping biped object
		{
			// Load vertex buffer...
			fread(&lpCh->nVertexCnt, sizeof(lpCh->nVertexCnt), 1, Fileptr);
			if(lpCh->nVertexCnt != 0)
			{
				lpCh->pVertex = NULL;
				
				if (lpCh->bBakemap)
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX_UV2), g_3DObjectMemoryUsage, 
						D3DFVF_XMESH_VERTEX_UV2, g_3DObjectMemoryPool, &(lpCh->pVertex), NULL )) ) 
					{
						_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );
						return FALSE;
					}				

					//_XDWINPRINT( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );
					
					_LPXMESH_VERTEX_UV2 pVertices;
					if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods ) ) )
					{					
						fread(pVertices, sizeof(_XMESH_VERTEX_UV2) * lpCh->nVertexCnt, 1, Fileptr);
						lpCh->pVertex->Unlock();
					}

					// Create second temporary buffer...
					if( createsecondbuffer )
					{
						if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX_UV2), g_3DObjectMemoryUsage, 
							D3DFVF_XMESH_VERTEX_UV2, g_3DObjectMemoryPool, &(lpCh->pSecondVertexBuffer), NULL )) )
						{
							_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
							return FALSE;
						}

						//_XDWINPRINT( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
					}				}
				else
				{
					if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), g_3DObjectMemoryUsage, 
						D3DFVF_XMESH_VERTEX, g_3DObjectMemoryPool, &(lpCh->pVertex), NULL )) ) 
					{
						_XFatalError("Create first vertex buffer [%s]", (szFilename) ? szFilename : "this" );
						return FALSE;
					}				

					//_XDWINPRINT( "First vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pVertex );
					
					_LPXMESH_VERTEX pVertices;
					if( SUCCEEDED(lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods ) ) )
					{					
						fread(pVertices, sizeof(_XMESH_VERTEX) * lpCh->nVertexCnt, 1, Fileptr);
						lpCh->pVertex->Unlock();
					}

					// Create second temporary buffer...
					if( createsecondbuffer )
					{
						if( FAILED(gpDev->CreateVertexBuffer( lpCh->nVertexCnt*sizeof(_XMESH_VERTEX), g_3DObjectMemoryUsage, 
							D3DFVF_XMESH_VERTEX, g_3DObjectMemoryPool, &(lpCh->pSecondVertexBuffer), NULL )) )
						{
							_XFatalError("Create secondary vertex buffer [%s]", (szFilename) ? szFilename : "this" );
							return FALSE;
						}

						//_XDWINPRINT( "Second vertex buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pSecondVertexBuffer );
					}
				}
			}

			// Load index buffer...
			fread(&lpCh->nIndexCnt, sizeof(lpCh->nIndexCnt), 1, Fileptr);    
			
			if(lpCh->nIndexCnt != 0)
			{
				if( FAILED(gpDev->CreateIndexBuffer( lpCh->nIndexCnt*sizeof(WORD), g_3DObjectMemoryUsage, D3DFMT_INDEX16, 
													 g_3DObjectMemoryPool, &(lpCh->pIndex), NULL )) )
				{
					_XFatalError("Create index buffer [%s]", (szFilename) ? szFilename : "this" );
					return FALSE;
				}					

				//_XDWINPRINT( "Index buffer created[%20s:%d [%p] ]", szFilename, i, lpCh->pIndex );
				
				LPWORD pIndices;				
				if( SUCCEEDED( lpCh->pIndex->Lock( 0, lpCh->nIndexCnt*sizeof(WORD), (void**)&pIndices, g_3DObjectLockMethods )) )
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
						_XFatalError( "WARNING : Invalid physique informations\nvertex : %d, Physique number : %d",
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

	m_ObjectRadius = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );

#ifdef _XDWDEBUG
	CsuX3D_Box	obbbox;
	obbbox.Create( Header.OBB_Center, Header.OBB_Axis, Header.OBB_Extent );
	m_OBBBox.InitBox( obbbox.Vertices(), obbbox.Center() ); 
	
	if( szFilename )
		_XDWINPRINT( "Loaded mesh file [%s]", szFilename );
	else
		_XDWINPRINT( "Loaded mesh file [%s]", g_LastAccessPackedFile );	
#endif

	return TRUE;
}


void _XMeshContainer::RenderNormal( D3DXMATRIX& trmatrix )
{
	if(m_Node == NULL) return;
	gpDev->SetTransform( D3DTS_WORLD, &trmatrix );
	
	AXIS_VERTEX	line[2];
	line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	
	gpDev->SetTexture(0, NULL); 
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );								
	
	for(int i = 0; i < m_NodeCnt; i++)
	{	
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;
		
		_LPXMESH_VERTEX pVertices;
		if( SUCCEEDED( m_Node[i].pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods ) ) )
		{
			for( int j = 0; j < m_Node[i].nVertexCnt; j++ )
			{
				line[0].v.x = pVertices[j].x;
				line[0].v.y = pVertices[j].y;
				line[0].v.z = pVertices[j].z;
				
				line[1].v.x = line[0].v.x + pVertices[j].nx * 0.3f;
				line[1].v.y = line[0].v.y + pVertices[j].ny * 0.3f;
				line[1].v.z = line[0].v.z + pVertices[j].nz * 0.3f;
				
				gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
			}			
			
			m_Node[i].pVertex->Unlock();
		}
	}	
	
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void _XMeshContainer::RenderNormal( _XModelDescriptor* pAniData )
{
	if(m_Node == NULL) return;
	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );

	AXIS_VERTEX	line[2];
	line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	
	gpDev->SetTexture(0, NULL); 
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );								

	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			_LPXMESH_VERTEX plVertices;
			if( SUCCEEDED( m_Node[i].pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, g_3DObjectLockMethods ) ) )
			{
				for( int j = 0; j < m_Node[i].nVertexCnt; j++ )
				{
					line[0].v.x = plVertices[j].x;
					line[0].v.y = plVertices[j].y;
					line[0].v.z = plVertices[j].z;

					line[1].v.x = line[0].v.x + plVertices[j].nx * 0.1f;
					line[1].v.y = line[0].v.y + plVertices[j].ny * 0.1f;
					line[1].v.z = line[0].v.z + plVertices[j].nz * 0.1f;

					gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
				}

				m_Node[i].pSecondVertexBuffer->Unlock();
			}
		}		
	}
	else
	{
		for(int i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			_LPXMESH_VERTEX pVertices;
			if( SUCCEEDED( m_Node[i].pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods ) ) )
			{
				for( int j = 0; j < m_Node[i].nVertexCnt; j++ )
				{
					line[0].v.x = pVertices[j].x;
					line[0].v.y = pVertices[j].y;
					line[0].v.z = pVertices[j].z;
					
					line[1].v.x = line[0].v.x + pVertices[j].nx * 0.1f;
					line[1].v.y = line[0].v.y + pVertices[j].ny * 0.1f;
					line[1].v.z = line[0].v.z + pVertices[j].nz * 0.1f;
					
					gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
				}			
				
				m_Node[i].pVertex->Unlock();
			}
		}
	}		

	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void _XMeshContainer::RenderNormal( _XModelDescriptor* pAniData, D3DXMATRIX& worldmat )
{
	if(m_Node == NULL) return;
	gpDev->SetTransform( D3DTS_WORLD, &worldmat );

	AXIS_VERTEX	line[2];
	line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	
	gpDev->SetTexture(0, NULL); 
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );								

	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			_LPXMESH_VERTEX plVertices;
			if( SUCCEEDED( m_Node[i].pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, g_3DObjectLockMethods ) ) )
			{
				for( int j = 0; j < m_Node[i].nVertexCnt; j++ )
				{
					line[0].v.x = plVertices[j].x;
					line[0].v.y = plVertices[j].y;
					line[0].v.z = plVertices[j].z;

					line[1].v.x = line[0].v.x + plVertices[j].nx * 0.1f;
					line[1].v.y = line[0].v.y + plVertices[j].ny * 0.1f;
					line[1].v.z = line[0].v.z + plVertices[j].nz * 0.1f;

					gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
				}

				m_Node[i].pSecondVertexBuffer->Unlock();
			}
		}		
	}
	else
	{
		for(int i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			_LPXMESH_VERTEX pVertices;
			if( SUCCEEDED( m_Node[i].pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods ) ) )
			{
				for( int j = 0; j < m_Node[i].nVertexCnt; j++ )
				{
					line[0].v.x = pVertices[j].x;
					line[0].v.y = pVertices[j].y;
					line[0].v.z = pVertices[j].z;

					line[1].v.x = line[0].v.x + pVertices[j].nx * 0.1f;
					line[1].v.y = line[0].v.y + pVertices[j].ny * 0.1f;
					line[1].v.z = line[0].v.z + pVertices[j].nz * 0.1f;

					gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
				}			

				m_Node[i].pVertex->Unlock();
			}
		}
	}		

	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void _XMeshContainer::Render( _XModelDescriptor* pAniData )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
//	gpDev->SetMaterial( &m_Material );

	int i;

	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{
		_XM_SET_ALPHATEST_ON
		for(i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );

			if( g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
							
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}
		}		
		_XM_SET_ALPHATEST_OFF
	}
	else
	{		
		_XM_SET_ALPHATEST_ON
		for(i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
										
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );			
			}
		}		
		_XM_SET_ALPHATEST_OFF
	}		

#ifdef _XDWDEBUG	
	if( g_ViewNormalVector )
	{	
		RenderNormal( pAniData );
	}
#endif

}


void _XMeshContainer::Render_SecondBuffer( _XModelDescriptor* pAniData )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	
	int i;
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
		
	for(i = 0; i < m_NodeCnt; i++)
	{				
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;
		
		g_SetTextureFailed = FALSE;
		m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
					
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
					
#ifdef _XDWDEBUG	
	if( g_ViewNormalVector )
	{	
		RenderNormal( pAniData );
	}
#endif
}

void _XMeshContainer::Render( _XModelDescriptor* pAniData, D3DXMATRIX& worldmat )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmat );

	int i;	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{
		_XM_SET_ALPHATEST_ON
		for(i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}
		}		
		_XM_SET_ALPHATEST_OFF
	}
	else
	{
		_XM_SET_ALPHATEST_ON
		for(i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}
		}
		_XM_SET_ALPHATEST_OFF
	}	
	
#ifdef _XDWDEBUG	
	if( g_ViewNormalVector )
	{	
		RenderNormal( pAniData );
	}
#endif
}

void _XMeshContainer::Render( D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
		
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	_XM_SET_ALPHATEST_ON
	
	for(int i = 0; i < GetNodeCnt(); i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;

		g_SetTextureFailed = FALSE;
		m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );		
		}
	}

	_XM_SET_ALPHATEST_OFF
}

void _XMeshContainer::RenderNoTexture( D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(int i = 0; i < GetNodeCnt(); i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;

		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}	
}

void _XMeshContainer::RenderNoTexture( _XModelDescriptor* pAniData, D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
	
	int i;	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(i = 0; i < m_NodeCnt; i++)
		{				
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
	else
	{
		for(i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;

			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
}

void _XMeshContainer::RenderInterfaceModel(_XModelDescriptor* pAniData, D3DXMATRIX& worldmat)
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform(D3DTS_WORLD, &worldmat);
	gpDev->SetFVF(D3DFVF_XMESH_VERTEX);
									
	if(pAniData->m_AniFlag)
	{
		// 애니매이션 중
		for( int i = 0 ; i < m_NodeCnt ; i++)
		{
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;
			
			m_pTextureManager->SetTexture( m_Node[i].nTextureId );			
			
			gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
			gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
			gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
			gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );
			
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
			if(m_Node[i].nTextureId <  0) continue;

			m_pTextureManager->SetTexture( m_Node[i].nTextureId );			
						
			gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
			gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
			gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
			gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );
			
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
		
#ifdef _XDWDEBUG	
	if( g_ViewNormalVector )
	{	
		RenderNormal( pAniData );
	}
#endif
}

void _XMeshContainer::Render_OriginalPos( D3DXVECTOR3& offset, int index, D3DXMATRIX* rotmat )
{
	if(m_Node == NULL) return;
	
	D3DXMATRIX	curmat;	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
		
	for(int i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;
		
		D3DXMatrixIdentity(&curmat);
		D3DXMatrixMultiply(&curmat, &curmat, &m_Node[i].Inverse);
		if(rotmat) D3DXMatrixMultiply( &curmat, &curmat, rotmat );

		curmat._41 += offset.x;
		curmat._42 += offset.y;
		curmat._43 += offset.z;
		gpDev->SetTransform( D3DTS_WORLD, &curmat );
		
		m_pTextureManager->SetTexture( m_Node[i].nTextureId );

		gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
		gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
		gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
		gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );

		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );		
	}
	
	//D3DXVECTOR3	axis = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	//m_OBBBox.Render( curmat, &axis );
}

void _XMeshContainer::RenderInterfaceItem(D3DXMATRIX* rotmat)
{
	if(m_Node == NULL) return;
	
	D3DXMATRIX	curmat;
			
	int i;		
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );

	for(i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;

		D3DXMatrixIdentity(&curmat);
		D3DXMatrixMultiply(&curmat, &curmat, &m_Node[i].Inverse);
				
		if(rotmat) D3DXMatrixMultiply( &curmat, &curmat, rotmat );

		
		gpDev->SetTransform( D3DTS_WORLD, &curmat );
		
		m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		gpDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE);	
		gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
		gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL  ); 
		gpDev->SetRenderState( D3DRS_ZENABLE,  g_RS_UseZBuffer  );
		
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );			
		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
	}	
}

void _XMeshContainer::RenderAlphaBlend( D3DMATRIX& worldmatrix, DWORD  Alphalevel )
{	
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
	//gpDev->SetMaterial( &m_Material );

	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE);
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
    //gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	D3DCOLOR color = (Alphalevel << 24) + 0xFFFFFF;		
	
    gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, color );
    gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );

	for(int i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;						
		if(m_Node[i].nTextureId <  0) continue;

		m_pTextureManager->SetTexture( m_Node[i].nTextureId );
		gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
		gpDev->SetIndices( m_Node[i].pIndex );

		gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );		
	}	
	
	//gpDev->SetRenderState( D3DRS_FOGENABLE, g_LodTerrain.m_Fog );
	//gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );	
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}

void _XMeshContainer::_Render( _XModelDescriptor* pAniData )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &pAniData->m_Position );
	
	int i;	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
			
	if( pAniData->m_AniFlag /*|| pAniData->m_AniFrame != 0.0f*/ )	// 애니메이션 중...
	{		
		for(i = 0; i < m_NodeCnt; i++)
		{
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;
			
			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture( m_Node[i].nTextureId );
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
		for(i = 0; i < m_NodeCnt; i++)
		{	
			if(m_Node[i].nVertexCnt == 0) continue;
		
			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );			
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}
		}		
	}	
	
#ifdef _XDWDEBUG	
	if( g_ViewNormalVector )
	{	
		RenderNormal( pAniData );
	}
#endif
}

void _XMeshContainer::RenderCustomDraw( D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	for(int i = 0; i < m_NodeCnt; i++)
	{	
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;

		g_SetTextureFailed = FALSE;
		m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );
		
		if(!g_SetTextureFailed)
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );		
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
}

void _XMeshContainer::RenderCustomDraw(  _XModelDescriptor* pAniData, D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );		
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
	
	if( pAniData->m_AniFlag )	// 애니메이션 중...
	{		
		for(int i = 0; i < m_NodeCnt; i++)
		{
			if(m_Node[i].nVertexCnt == 0) continue;			
			if(m_Node[i].nTextureId <  0) continue;
			
			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );
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
			if(m_Node[i].nTextureId <  0) continue;

			g_SetTextureFailed = FALSE;
			m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nTextureId );

			if( !g_SetTextureFailed )
			{
				gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );		
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}
		}	
	}
}

void _XMeshContainer::CustomDraw( _XModelDescriptor* pAniData )
{
	if(m_Node == NULL) return;	
	_XRenderObject* pRenderObject;
	
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;

#ifndef _RENDEROBJECT_POOL_
		pRenderObject = new _XRenderObject;
#else
		pRenderObject = m_pTextureManager->PopRenderObject();
#endif
		pRenderObject->pObject = (m_Node+i);
		pRenderObject->pObjectMatrix = &pAniData->m_Position;
		pRenderObject->usesecondbuffer = pAniData->m_AniFlag;

		m_pTextureManager->PushRenderObject( m_Node[i].nTextureId, pRenderObject );			
	}	
	
#ifdef _XDWDEBUG	
	if( g_ViewNormalVector )
	{	
		RenderNormal( pAniData );
	}
#endif
}

void _XMeshContainer::CustomDraw( _XOctreeObject* pObject )
{
	if(m_Node == NULL) return;	
	_XRenderObject* pRenderObject;
	/*
	BOOL collidecameraray = FALSE;
	if( pObject->m_Distance - (pObject->m_Radius * pObject->m_Radius) < 50.0f )
	{
		D3DXVECTOR3 raydir = (g_LodTerrain.m_3PCamera.m_CameraPosition - g_pLocalUser->m_Position);
		D3DXVec3Normalize( &raydir, &raydir );
		raydir *= 10.0f;

		if( InterOBBvsRay( g_pLocalUser->m_Position, raydir, pObject->m_matWorldPosition ) )
		{
			collidecameraray = TRUE;
		}
	}
	
	if( collidecameraray )
	{
		for(int i = 0; i < m_NodeCnt; i++)
		{
			if(m_Node[i].nVertexCnt == 0) continue;
			if(m_Node[i].nTextureId <  0) continue;
			
			pRenderObject = new _XRenderObject;
			pRenderObject->pObject = (m_Node+i);
			pRenderObject->pObjectMatrix = &pObject->m_matWorldPosition;
			pRenderObject->usesecondbuffer = FALSE;
			pRenderObject->abovewater = pObject->m_AboveWater;
			
			m_pTextureManager->PushAlphaRenderObject( m_Node[i].nTextureId, pRenderObject );
		}
	}
	else
	{*/
		for(int i = 0; i < m_NodeCnt; i++)
		{
			if(m_Node[i].nVertexCnt == 0) continue;			
			if(m_Node[i].nTextureId <  0) continue;

#ifndef _RENDEROBJECT_POOL_
			pRenderObject = new _XRenderObject;
#else
			pRenderObject = m_pTextureManager->PopRenderObject();
#endif
			pRenderObject->pObject = (m_Node+i);
			pRenderObject->pObjectMatrix = &pObject->m_matWorldPosition;
			pRenderObject->usesecondbuffer = FALSE;
			pRenderObject->abovewater = pObject->m_AboveWater;
						
			m_pTextureManager->PushRenderObject( m_Node[i].nTextureId, pRenderObject );
		}
	//}
}

void _XMeshContainer::CustomLightmapDraw( _XOctreeObject* pObject )
{
	if(m_Node == NULL) return;	
	_XRenderObject* pRenderObject;

	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;
		
#ifndef _RENDEROBJECT_POOL_
		pRenderObject = new _XRenderObject;
#else
		pRenderObject = m_pTextureManager->PopRenderLightmapObject();
#endif
		pRenderObject->pObject = (m_Node+i);
		pRenderObject->pObjectMatrix = &pObject->m_matWorldPosition;
		pRenderObject->usesecondbuffer = FALSE;
		pRenderObject->abovewater = pObject->m_AboveWater;
		
		m_pTextureManager->PushLightmapRenderObject( m_Node[i].nTextureId, pRenderObject );
	}
}

void _XMeshContainer::_Render( D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );	
	
	for(int i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;		
		if(m_Node[i].nTextureId <  0) continue;

		g_SetTextureFailed = FALSE;
		m_pTextureManager->SetTexture( m_Node[i].nTextureId );

		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
}

void _XMeshContainer::_RenderAlphaBlend( D3DMATRIX& worldmatrix, DWORD  Alphalevel )
{	
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
	//gpDev->SetMaterial( &m_Material );
    
	D3DCOLOR color = (Alphalevel << 24) + 0xFFFFFF;
    gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, color );

	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
    
	for(int i = 0; i < GetNodeCnt(); i++)
	{		
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;

		g_SetTextureFailed = FALSE;		
		m_pTextureManager->SetTexture( m_Node[i].nTextureId );

		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );		
		}
	}
}



#define _XDEF_USESLERPANIMATION	

void _XMeshContainer::Animation( _XModelDescriptor* pAniData )
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

void _XMeshContainer::FrameAnimation( _XModelDescriptor* pAniData )
{	
	if( !pAniData->m_pBipedController ) return;
		
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
			
			lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods );						
			lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, g_3DObjectLockMethods );			
			
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
	
	pAniData->m_AniLastFrame = pAniData->m_AniFrame;		
}


#define _XDEF_SLERPANIMATIONTIME	5.0f

void _XMeshContainer::SlerpAnimation( _XModelDescriptor* pAniData )
{			
	if( !pAniData->m_pBipedController ) return;
		
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
				else // process biped animation
				{
					// 현재 오브젝트는 로컬이 아니라. 월드로 되어있기 때문에..
					// 인버스에 곱해줘서.. 매트릭스를 로컬로 변경해 줘야 한다..
					// 곱하는 순서에 유의..
					D3DXMatrixMultiply(&d3dMatrix, &lpCh->Inverse, &lpCh->LocalAniTM);
				}
				
				_LPXMESH_VERTEX pVertices;
				lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods );
				d3dxTmpVec2 = d3dxTmpVec = pVertices[ nIdx ];
				lpCh->pVertex->Unlock();								
				
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
				
				lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods );
				pVertices[ nIdx ] = d3dxTmpVec2;
				lpCh->pSecondVertexBuffer->Unlock();
			}
		}
	}	
}


void _XMeshContainer::RenderShadow( void )
{	
	if( m_NodeCnt > 0)
	{					
		for(int i = 0; i < m_NodeCnt; i++)
		{
			if( m_Node[i].nVertexCnt )
			{	
				if(m_Node[i].nTextureId <  0) continue;

				gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
				gpDev->SetIndices( m_Node[i].pIndex );
				gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
			}			
		}	
	}// Brace if( m_NodeCnt != 0)		
}


//=====================================================================================================================
//	MakeShadowMatrix()	
//  그림자용 메트릭스를 만든다. 모델월드 -> 라이트뷰 -> 쉐도우 프로젝션 행렬
//=====================================================================================================================
/*
float g_fXMax	= 0.0f;
float g_fYMax	= 0.0f;	
float g_fXProj	= 0.0f;
float g_fYProj	= 0.0f;
*/
void _XMeshContainer::MakeShadowMatrix(  FLOAT& fXMax, FLOAT& fYMax, D3DXMATRIX& matLocalToLight )
{		
	// 객체가 텍스쳐에 딱 들어오도록 수평수직 FOV를 계산한다.
	// [그림자 객체 좌표] 들을 [광원 좌표 == 뷰 좌표계]로 변환한다음 적당한 FOV를 계산해냄.
	D3DXVECTOR3		vCur;
	D3DXVECTOR4		VOut;
	
	int n = GetNodeCnt();
	for(int ii = 0; ii < n; ii++)
	{
		_LPXM_MESHOBJECT lpCh = GetNode(ii);
		if(lpCh->nVertexCnt == 0) continue;
		if(lpCh->nTextureId <  0) continue;

		int nVertices = lpCh->nVertexCnt;
		_LPXMESH_VERTEX pVertices;
		
		lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods );		
		for( int n= 0; n<nVertices; n++)
		{
			vCur.x	= pVertices[n].x;
			vCur.y	= pVertices[n].y;
			vCur.z	= pVertices[n].z;
			
			D3DXVec3Transform( &VOut, &vCur, &matLocalToLight );
			if( (float)fabs(VOut.x / VOut.z)	> fXMax )	fXMax = (float)fabs(VOut.x/VOut.z);
			if( (float)fabs(VOut.y / VOut.z)	> fYMax )	fYMax = (float)fabs(VOut.y/VOut.z);
		}
		lpCh->pSecondVertexBuffer->Unlock();
	}	
}

extern bool ClipBoxRay (D3DXVECTOR3& rkOrigin, D3DXVECTOR3& rkDirection, float afExtent[3], float& rfT0, float& rfT1);

BOOL _XMeshContainer::InterOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXMATRIX  Offset )
{
	// convert ray to box coordinates
	D3DXVECTOR3 offsetvector;
	offsetvector.x = Offset._41;
	offsetvector.y = Offset._42;
	offsetvector.z = Offset._43;

    D3DXVECTOR3 diff = mouserayorigin - ( OBB_Center + offsetvector );
    D3DXVECTOR3 Origin(D3DXVec3Dot(&diff,&OBB_Axis[0]),
					   D3DXVec3Dot(&diff,&OBB_Axis[1]),
					   D3DXVec3Dot(&diff,&OBB_Axis[2]) );
    D3DXVECTOR3 Direction(
        D3DXVec3Dot(&mouseraydirection,&OBB_Axis[0]),
		D3DXVec3Dot(&mouseraydirection,&OBB_Axis[1]),
		D3DXVec3Dot(&mouseraydirection,&OBB_Axis[2]) );
	
    float fT0 = 0.0f, fT1 = 1.0f;
    return ClipBoxRay(Origin,Direction,OBB_Extent,fT0,fT1);
}

BOOL _XMeshContainer::InterOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXVECTOR3 Offset )
{
	// convert ray to box coordinates
    D3DXVECTOR3 diff = mouserayorigin - ( OBB_Center + Offset );
    D3DXVECTOR3 Origin(D3DXVec3Dot(&diff,&OBB_Axis[0]),
					   D3DXVec3Dot(&diff,&OBB_Axis[1]),
					   D3DXVec3Dot(&diff,&OBB_Axis[2])
					   
					   );
    D3DXVECTOR3 Direction(
        D3DXVec3Dot(&mouseraydirection,&OBB_Axis[0]),
		D3DXVec3Dot(&mouseraydirection,&OBB_Axis[1]),
		D3DXVec3Dot(&mouseraydirection,&OBB_Axis[2])
        
		);
	
    float fT0 = 0.0f, fT1 = 1.0f;
    return ClipBoxRay(Origin,Direction,OBB_Extent,fT0,fT1);
}

BOOL _XMeshContainer::InterWeaponOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXVECTOR3 Offset, 
										  D3DXVECTOR3 &Center,D3DXVECTOR3 Axis[3],float Extent[3])
{
	// convert ray to box coordinates
    D3DXVECTOR3 diff = mouserayorigin - ( Center + Offset );
    D3DXVECTOR3 Origin(D3DXVec3Dot(&diff,&Axis[0]),
					   D3DXVec3Dot(&diff,&Axis[1]),
					   D3DXVec3Dot(&diff,&Axis[2])
					   
					   );
    D3DXVECTOR3 Direction(
        D3DXVec3Dot(&mouseraydirection,&Axis[0]),
		D3DXVec3Dot(&mouseraydirection,&Axis[1]),
		D3DXVec3Dot(&mouseraydirection,&Axis[2])
        
		);
	
    float fT0 = 0.0f, fT1 = 1.0f;
    return ClipBoxRay(Origin,Direction,Extent,fT0,fT1);
}

void _XMeshContainer::FrameAnimation( _XBipedController* pBipedController )
{	
	_LPXM_MESHOBJECT	lpCh;
	_LPXM_BIPEDOBJECT	lpParent;
	D3DXMATRIX			d3dMatrix, mat;        
	_XMESH_VERTEX		d3dxTmpVec;
	_XMESH_VERTEX		d3dxTmpVec2;

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{			
		lpCh = GetNode(objindex );
		lpParent = pBipedController->GetNode(lpCh->nParentId);	

		if(lpCh->nVertexCnt != 0)
		{
			_LPXMESH_VERTEX pVertices, plVertices;
			
			lpCh->pVertex->Lock( 0, 0, (void**)&pVertices, g_3DObjectLockMethods );						
			lpCh->pSecondVertexBuffer->Lock( 0, 0, (void**)&plVertices, g_3DObjectLockMethods );			
			
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
					
						lpParent = pBipedController->GetNode(keyfactor.wKeyIdx);
						
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

void _XMeshContainer::Render_SecondBuffer( D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
	
	int i;
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX );
		
	for(i = 0; i < m_NodeCnt; i++)
	{				
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;

		g_SetTextureFailed = FALSE;		
		m_pTextureManager->SetTexture( m_Node[i].nTextureId );

		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
			gpDev->SetIndices( m_Node[i].pIndex );
					
			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
}

void _XMeshContainer::Render_BakemapBuffer( D3DMATRIX& worldmatrix )
{
	if(m_Node == NULL) return;
	
	gpDev->SetTransform( D3DTS_WORLD, &worldmatrix );
		
	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;
		
		g_SetTextureFailed = FALSE;
		m_pTextureManager->SetTexture_NoRenderStyleUseCulling( m_Node[i].nTextureId );
		m_pTextureManager->SetTexture_NoRenderStyle( m_Node[i].nBakemapId, 1);

		if( !g_SetTextureFailed )
		{
			gpDev->SetStreamSource( 0, m_Node[i].pVertex, 0, sizeof(_XMESH_VERTEX_UV2) );
			gpDev->SetIndices( m_Node[i].pIndex );

			gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_Node[i].nVertexCnt, 0, m_Node[i].nIndexCnt );
		}
	}
}


void _XMeshContainer::TexturePreCaching( void )
{
	if( m_TexturePreCached )
	{
		return;
	}

	if(m_Node == NULL) return;

	for(int i = 0; i < m_NodeCnt; i++)
	{
		if(m_Node[i].nVertexCnt == 0) continue;
		if(m_Node[i].nTextureId <  0) continue;
		
		if( m_Node[i].nTextureId > -1 )
		{
			m_pTextureManager->PreCachingTexture( m_Node[i].nTextureId );
				
		}

		if( m_Node[i].nBakemapId > -1 )
		{
			m_pTextureManager->PreCachingTexture( m_Node[i].nBakemapId );
		}
	}	


	m_TexturePreCached = TRUE;
}