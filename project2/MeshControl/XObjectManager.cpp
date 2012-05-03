// XObjectManager.cpp: implementation of the _XObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XObjectManager.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMeshContainer.h"

#include "XLifeEventManager.h"

#define _XDEF_MAXOBJECT_ALPHARENDER		400
_XOctreeObject* g_AlphaBlendRenderObject[_XDEF_MAXOBJECT_ALPHARENDER];
int				g_AlphaBlendRenderObjectCount = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// object manager
_XObjectManager::_XObjectManager()
{
	m_pObjectArray = NULL;
	m_ObjectCount = 0;	
}

_XObjectManager::~_XObjectManager()
{
	DisposeList();
}

void _XObjectManager::DisposeList( void  )
{
	SAFE_DELETE_ARRAY( m_pObjectArray );
	m_ObjectCount = 0;
}

void _XObjectManager::RebuildAboveWaterObject( void )
{
	for( int objectindex = 0 ; objectindex < m_ObjectCount; objectindex++ )
	{
		int	  xcoord = (_XFC_FloatToIntRet( ( m_pObjectArray[objectindex].m_matWorldPosition._41 + g_LodTerrain.m_HalfWorldSize ) / g_LodTerrain.m_GridRealSize ) >> 3 );
		int   zcoord = (_XFC_FloatToIntRet( ( m_pObjectArray[objectindex].m_matWorldPosition._43 + g_LodTerrain.m_HalfWorldSize ) / g_LodTerrain.m_GridRealSize ) >> 3 );
		
		m_pObjectArray[objectindex].m_AboveWater = FALSE;

		if( xcoord < 0 || xcoord >= g_LodTerrain.m_TileCount || zcoord < 0 || zcoord >= g_LodTerrain.m_TileCount ) 
		{
			continue;
		}
		
		for( int i = -2; i <= 2; i++ )
		{
			if( zcoord+i < 0 ) continue;
			if( zcoord+i >= g_LodTerrain.m_TileCount ) continue;

			for( int j = -2; j <= 2; j++ )
			{
				if( xcoord+j < 0 ) continue;
				if( xcoord+j >= g_LodTerrain.m_TileCount ) continue;

				S_WATER* curwater = g_LodTerrain.m_pWaterObjectManager->GetWaterObject( (zcoord+i) + ((xcoord+j) * g_LodTerrain.m_TileCount) );
				if( curwater )
				{
					if( curwater->enabled ) 
					{
						m_pObjectArray[objectindex].m_AboveWater = TRUE;
						break;
					}
				}				
			}

			if( m_pObjectArray[objectindex].m_AboveWater )
			{
				break;
			}
		}
	}	
}

BOOL _XObjectManager::LoadObjectList( FILE* Fileptr, LPSTR Filename )
{
	_XMEMORYUSECHECKREADY		
	_XMEMORYUSECHECKSTART

	assert( Fileptr );
	if( !Fileptr ) 
	{
		_XFatalError( "LoadObjectList(%s) : Invalid file pointer", Filename );
		return FALSE;
	}
	
	int objectcount = 0;
	if( fread( &objectcount, sizeof(int), 1, Fileptr ) < 1 )
	{
		_XFatalError( "LoadObjectList(%s) Read object count", Filename );		
		return FALSE;
	}
	
	DisposeList();

	_LPXObject pobjectarray = NULL;
	_EXT_Object* pExtObject = NULL;

	// 생활 이벤트쪽 테스트용으로 쓰기 위해..
	// 우선 임시로 이름으로 비교한다.
/*	if (_stricmp(Filename, "map_noklim.xtg") == 0)
	{
		pExtObject = new _EXT_Object[objectcount];
		if( fread( pExtObject, sizeof(_EXT_Object)*objectcount, 1, Fileptr ) < 1 ) 
		{
			_XFatalError( "LoadTerrain(%s) Read object data", Filename );
			delete[] pExtObject;
			return FALSE;
		}
	}
	else
*/	{
		pobjectarray = new _XObject[objectcount];
		
		if( !pobjectarray )
		{
			_XFatalError( "LoadObjectList(%s) Allocation object temporary array", Filename );
			return FALSE;
		}
		
		if( fread( pobjectarray, sizeof(_XObject)*objectcount, 1, Fileptr ) < 1 ) 
		{
			_XFatalError( "LoadTerrain(%s) Read object data", Filename );
			delete[] pobjectarray;
			return FALSE;
		}
	}

	m_ObjectCount = objectcount;

	m_pObjectArray = new _XOctreeObject[ objectcount ];

	if( !m_pObjectArray )
	{
		_XFatalError( "LoadObjectList(%s) Allocation object array", Filename );
		delete[] pobjectarray;
		return FALSE;
	}

	//memset( m_pObjectArray, 0, sizeof(_XOctreeObject) * objectcount );
	_XMeshContainer* model = NULL; 
	
	for( int i=0; i<objectcount; i++ )
	{
		// 생활 이벤트쪽 때문에 테스트 데이타용으로 쓰기 위해서..
/*		if (_stricmp(Filename, "map_noklim.xtg") == 0)
		{
			m_pObjectArray[i].m_matWorldPosition = pExtObject[i].m_matWorldPosition;
			m_pObjectArray[i].m_ObjectIndex      = pExtObject[i].m_ObjectIndex;
			m_pObjectArray[i].m_nEventType		 = pExtObject[i].m_nEventType;
			m_pObjectArray[i].m_nEventValue      = pExtObject[i].m_nEventValue;
		}
		else
*/		{
			m_pObjectArray[i].m_matWorldPosition = pobjectarray[i].m_matWorldPosition;
			m_pObjectArray[i].m_ObjectIndex      = pobjectarray[i].m_ObjectIndex;
		}
		
		model = (_XMeshContainer*)g_LodTerrain.m_MeshObjectManager.GetModelContainer(m_pObjectArray[i].m_ObjectIndex);
		if( model )
		{
			if (!model->m_strName.empty())
			{
				_XMeshObjectManager::smdef_BipedController::iterator it = g_LodTerrain.m_MeshObjectManager.m_smBipedController.find(model->m_strName.c_str());
				if (it != g_LodTerrain.m_MeshObjectManager.m_smBipedController.end())
				{
					m_pObjectArray[i].m_pBipedController	= (*it).second;			
					_LPXM_MOTION lpMotion = m_pObjectArray[i].m_pBipedController->GetMotion(0);
					if (lpMotion)
					{
						m_pObjectArray[i].m_fAniTime = rand() % static_cast<int>(lpMotion->EndFrame);
					}
				}

/*				// 대소문자를 구별하지 않는 find문을 이용해야 하나 지금 책이 없어 기억이 안 나므로 
				// 우선 이렇게 체크 한다. 차후 수정하기로 한다.
				_XMeshObjectManager::smdef_BipedController::iterator it;
				for (it = g_LodTerrain.m_MeshObjectManager.m_smBipedController.begin(); it != g_LodTerrain.m_MeshObjectManager.m_smBipedController.end(); ++ it)
				{
					if (_stricmp(it->first.c_str(), model->m_strName.c_str()) == 0)
					{
						m_pObjectArray[i].m_pBipedController	= (*it).second;
						_LPXM_MOTION lpMotion = m_pObjectArray[i].m_pBipedController->GetMotion(0);
						if (lpMotion)
						{
							m_pObjectArray[i].m_fAniTime = rand() % static_cast<int>(lpMotion->EndFrame);
						}

						break;
					}
				}
*/
			}

			m_pObjectArray[i].m_Radius	= model->m_ObjectRadius;			
		}		
	}

	delete[] pobjectarray;

	_XDWINPRINT( "%d Object loaded", objectcount );

	_XMEMORYUSECHECKEND( "World object list" );
				
	return TRUE;
}

void _XObjectManager::PreCachingTexture( void )
{
	for( int i=0; i<m_ObjectCount; i++ )
	{
		// Check distance
		FLOAT dx = (g_pLocalUser->m_NextRespawnPosition.x - m_pObjectArray[i].m_matWorldPosition._41);
		FLOAT dz = (g_pLocalUser->m_NextRespawnPosition.z - m_pObjectArray[i].m_matWorldPosition._43);
		
		if( _XFastDistance( dx, dz ) < g_LodTerrain.m_LODFarPlane )
		{
			_XMeshContainer* model = (_XMeshContainer*)g_LodTerrain.m_MeshObjectManager.GetModelContainer(m_pObjectArray[i].m_ObjectIndex);

			if( model )
			{
				model->TexturePreCaching();
			}			
			else
			{
				_XFatalError( "Can't access wolrd object model [%d]", m_pObjectArray[i].m_ObjectIndex );
				return;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mesh object manager
_XMeshObjectManager::_XMeshObjectManager()
{
	m_ModelStackMaxCount = 0;
	m_ModelStackHeight	 = 0;
	m_ModelList	 = NULL;
}

_XMeshObjectManager::~_XMeshObjectManager()
{
	disposeList();
}

void _XMeshObjectManager::disposeList( void )
{
	if( m_ModelList )
	{
		for( int i = 0; i < m_ModelStackMaxCount; i++ )
		{
			delete	m_ModelList[i];
		}
	}
	
	SAFE_DELETE_ARRAY( m_ModelList );

	smdef_BipedController::iterator it;
	for (it = m_smBipedController.begin(); it != m_smBipedController.end(); ++ it)
	{
		SAFE_DELETE( (*it).second );
	}
	m_smBipedController.clear();
	m_slRenderObject.clear();
	m_slRenderBakemapObject.clear();
}


BOOL _XMeshObjectManager::ReserveModelStack( int modelstacksize )
{
	if( m_ModelList )
	{
		for( int i = 0; i < m_ModelStackMaxCount; i++ )
		{
			delete	m_ModelList[i];
		}
	}	
	SAFE_DELETE_ARRAY( m_ModelList );

	m_ModelStackHeight	 = 0;
	m_ModelStackMaxCount = 0;

	m_ModelList = new _XMeshContainer*[modelstacksize];	
	if( !m_ModelList )
	{
		_XFatalError( "_XMeshObjectManager :: Reserve model stack memory allocation [%d]", modelstacksize );
		return FALSE;
	}

	memset( m_ModelList, 0, sizeof(_XMeshContainer*) *  modelstacksize );
	m_ModelStackMaxCount = modelstacksize;

	return TRUE;
}

_XMeshContainer* _XMeshObjectManager::GetModelContainer( int index )
{
	if( index >= m_ModelStackMaxCount ) return NULL;
	return m_ModelList[index];
}

void _XMeshObjectManager::IntersectionEventObject(D3DXVECTOR3& vRayPos, D3DXVECTOR3& vRayDir)
{
	if(g_CurrentFocusedObject && g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND) return;

	D3DXVECTOR3 vLocalRayDir;
	D3DXVec3Normalize(&vLocalRayDir, &vRayDir);

	sldef_XOctreeObject::iterator it;
	for (it = m_slEventObjectList.begin(); it != m_slEventObjectList.end(); ++ it)
	{
		_XOctreeObject* pObject = (*it);

		D3DXVECTOR3 vPosition(pObject->m_matWorldPosition._41, pObject->m_matWorldPosition._42, pObject->m_matWorldPosition._43);

		if (RayAndSphereIntersect(vRayPos, vLocalRayDir, vPosition, pObject->m_Radius) == TRUE)
		{
			FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - vPosition.x);
			FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - vPosition.z);
			FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );

			if( _distance < g_LastPickedObjectDistance )
			{
				g_LastPickedObjectDistance = _distance;
				g_LastPickedObjectType = _XPICKINGTARGET_COLLECT;
				g_LastPickedObject = pObject;
			}
		}
	}
}

void _XMeshObjectManager::_RenderObject( _XOctreeObject& object )
{	
	if( object.m_ObjectIndex >= m_ModelStackMaxCount )
	{
		_XFatalError( "WARNING : Invalid model index [%d/%d]", object.m_ObjectIndex, m_ModelStackMaxCount );
		return;
	}
	
	if( m_ModelList[object.m_ObjectIndex] )
	{		
		// animation object
		if (object.m_pBipedController)
		{
			m_slRenderObject.push_back(&object);
		}
		// baked map object
		else if (m_ModelList[object.m_ObjectIndex]->m_bBakemap)
		{
			//m_slRenderBakemapObject.push_back(&object);
			m_ModelList[object.m_ObjectIndex]->CustomLightmapDraw( &object );
		}
		// standard object
		else
		{
			m_ModelList[object.m_ObjectIndex]->CustomDraw( &object );
		}

		if (object.m_nEventType != OBJECT_EVENT_NONE)
		{
			if (object.m_nEventType == OBJECT_EVENT_WATER_TUBE)
			{
				g_LifeEventManager.m_matTubeWorld = object.m_matWorldPosition;
				g_LifeEventManager.m_matTubeWorld._42 += 0.5f;
			}
			else if (object.m_nEventType == OBJECT_EVENT_WATER_POINT)
			{
				g_LifeEventManager.m_matPointWorld = object.m_matWorldPosition;
				g_LifeEventManager.m_matPointWorld._42 += 0.5f;
			}
			m_slEventObjectList.push_back(&object);
		}
		
#ifdef _XDWDEBUG	
		if( g_ViewNormalVector )
		{	
			m_ModelList[object.m_ObjectIndex]->RenderNormal( object.m_matWorldPosition );
		}
#endif
	}
}

void _XMeshObjectManager::_RenderObject( _XFunctionalObject& object )
{
	if( object.m_ObjectIndex >= m_ModelStackMaxCount )
	{
		_XFatalError( "WARNING : Invalid model index [%d/%d]", object.m_ObjectIndex, m_ModelStackMaxCount );
		return;
	}
	
	if( m_ModelList[object.m_ObjectIndex] )
	{		
		if( m_ModelList[object.m_ObjectIndex]->m_bBakemap )
		{
			m_ModelList[object.m_ObjectIndex]->Render_BakemapBuffer( object.m_matWorldPosition );
		}
		else
		{
			m_ModelList[object.m_ObjectIndex]->_Render( object.m_matWorldPosition );
		}
	}
}

void _XMeshObjectManager::_RenderObject( _XRealTimeObject& object )
{	
	if( object.m_ObjectIndex >= m_ModelStackMaxCount )
	{
		_XFatalError( "WARNING : Invalid model index [%d/%d]", object.m_ObjectIndex, m_ModelStackMaxCount );
		return;
	}
	
	if( m_ModelList[object.m_ObjectIndex] ) 
	{
		m_ModelList[object.m_ObjectIndex]->_Render( object.m_matWorldPosition );

#ifdef _XDWDEBUG
		if( g_ViewNormalVector )
		{
			m_ModelList[object.m_ObjectIndex]->RenderNormal( object.m_matWorldPosition );
		}
#endif

	}
}

BOOL _XMeshObjectManager::insertMeshObject( _XMeshContainer* pMeshObject )
{	
	if( !m_ModelList ) return FALSE;
	if( m_ModelStackHeight >= m_ModelStackMaxCount ) return FALSE;

	m_ModelList[m_ModelStackHeight] = pMeshObject;
	m_ModelStackHeight++;

	return TRUE;
}

BOOL _XMeshObjectManager::LoadMeshList( FILE* pTerrainFile )
{
	_XMEMORYUSECHECKREADY		
	_XMEMORYUSECHECKSTART
	
	if( !pTerrainFile )
	{
		_XFatalError( "LoadMeshList( fp ) NULL File pointer" );	
		return FALSE;
	}
	
	int MeshFileCount = 0;
	fread( &MeshFileCount, sizeof(int), 1, pTerrainFile );
		
	TCHAR szFilenameBuffer[_MAX_PATH];
	int  filenamelength = 0;

	if( !g_ModelArchive.ReOpenPackage() )
	{
		return FALSE;
	}	
	
	if( !ReserveModelStack( MeshFileCount ) )
	{
		g_ModelArchive.ClosePackage();
		return FALSE;
	}

	for( int i=0; i < MeshFileCount ; i++ )
	{
		memset( szFilenameBuffer, 0, _MAX_PATH );

		fread( &filenamelength, sizeof(int), 1, pTerrainFile );

		if( fread( szFilenameBuffer, filenamelength, 1, pTerrainFile ) == 1 )
		{	
			_EncodeData( szFilenameBuffer, _ENCODEKEY );

			if( strlen(szFilenameBuffer) > 0 )
			{
				_XMeshContainer* model = NULL;
				model = new _XMeshContainer;

				smdef_BipedController::iterator it = m_smBipedController.find(szFilenameBuffer);
				if (it != m_smBipedController.end())
				{
					model->m_strName = szFilenameBuffer;
				}
/*
				// 대소문자를 구별하지 않는 find문을 이용해야 하나 지금 책이 없어 기억이 안 나므로 
				// 우선 이렇게 체크 한다. 차후 수정하기로 한다.
				smdef_BipedController::iterator it;
				for (it = m_smBipedController.begin(); it != m_smBipedController.end(); ++ it)
				{
					if (_stricmp(it->first.c_str(), szFilenameBuffer) == 0)
					{
						model->m_strName = szFilenameBuffer;
						break;
					}
				}
*/
				if(!model)
				{					
					_XFatalError( "LoadMeshList::create mesh object" );
					g_ModelArchive.ClosePackage();
					return FALSE;
				}
								
				if( !model->Load( g_ModelArchive.GetPackedFile( szFilenameBuffer), 
								  &g_WorldObjectTextureArchive,
								  model->m_strName.empty() ? FALSE : TRUE,
								  szFilenameBuffer ) )
				{					
					delete model;
					g_ModelArchive.ClosePackage();
					return FALSE;
				}

				if( !insertMeshObject( model ) )
				{
					delete model;
					g_ModelArchive.ClosePackage();
					return FALSE;
				}
			}
		}
	}

	g_ModelArchive.ClosePackage();
	
	_XDWINPRINT( "NOTICE : Loaded mesh list" );

	_XMEMORYUSECHECKEND( "World mesh template data" );

	return TRUE;
}

BOOL _XMeshObjectManager::LoadAniMeshList( void )
{
	if( !g_ModelArchive.ReOpenPackage() )
	{
		return FALSE;
	}
	
	if( !g_AniArchive.ReOpenPackage() )
	{
		g_ModelArchive.ClosePackage();
		return FALSE;	
	}
	
	if( !g_ScriptArchive.ReOpenPackage() )
	{
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}	

	FILE* pScript = NULL;
	pScript = g_ScriptArchive.GetPackedFile( _T("AniMeshList.ini") );

	if( !pScript )
	{
		g_ScriptArchive.ClosePackage();				
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		_XFatalError("Load LoadAniMeshList [%s] file open", _T("AniMeshList.ini"));
		return FALSE;
	}

	char buffer[256];
	char parsebuffer[256];
	char dest_buffer[256];

	while( 1 )
	{
		if( feof( pScript ) ) break;
		memset( buffer, 0, sizeof(char)*256 );

		if( !fgets( buffer, 255, pScript ) )
		{
			break;
		}
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;

		if( strlen(buffer) > 0 )
		{
			sscanf( buffer, "%s", parsebuffer );

			strcpy(dest_buffer, parsebuffer);
			dest_buffer[ strlen( parsebuffer )-4 ] = '\0';
			strcat(dest_buffer, ".xbp");

			_XBipedController* pBipedController = new _XBipedController;
			
			if( !pBipedController->Load( g_ModelArchive.GetPackedFile( dest_buffer), dest_buffer))
			{					
				delete pBipedController;
				g_ScriptArchive.ClosePackage();				
				g_AniArchive.ClosePackage();
				g_ModelArchive.ClosePackage();
				return FALSE;
			}
			
			strcpy(dest_buffer, parsebuffer);
			dest_buffer[ strlen( parsebuffer)-4 ] = '\0';
			strcat(dest_buffer, ".xa\0");

			
			pBipedController->ReserveAnimationStack(1);
			if( !pBipedController->LoadKey( g_AniArchive.GetPackedFile( dest_buffer), dest_buffer))
			{					
				delete pBipedController;
				g_ScriptArchive.ClosePackage();				
				g_AniArchive.ClosePackage();
				g_ModelArchive.ClosePackage();
				return FALSE;
			}
			
			m_smBipedController.insert(smdef_BipedController::value_type(parsebuffer, pBipedController));
		}
		else
		{
			break;
		}
	}	

	g_ScriptArchive.ClosePackage();				
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();

	return TRUE;
}


void _XMeshObjectManager::RenderAniObject( FLOAT fElaspedTime )
{
	_XMeshContainer* pMeshContainer;
	sldef_OctreeObject::iterator it;
	for (it = m_slRenderObject.begin(); it != m_slRenderObject.end(); ++ it)
	{
		pMeshContainer = m_ModelList[(*it)->m_ObjectIndex];
		
		_LPXM_MOTION pMotion = (*it)->m_pBipedController->GetMotion(0);
		
		if( pMotion )
			(*it)->m_fAniTime += (fElaspedTime / (FLOAT)pMotion->FrameSpeed);
		else
			(*it)->m_fAniTime = 0;
		

		if ((*it)->m_pBipedController)
		{
			(*it)->m_pBipedController->FrameAnimation(0, (*it)->m_fAniTime);
			pMeshContainer->FrameAnimation((*it)->m_pBipedController);	
		}
		pMeshContainer->Render_SecondBuffer((*it)->m_matWorldPosition);
	}
}

void _XMeshObjectManager::RenderBakemapObject( )
{
	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle(&g_TerrainTextureArchive, 19);

	gpDev->SetFVF( D3DFVF_XMESH_VERTEX_UV2 );

	_XMeshContainer* pMeshContainer;
	sldef_OctreeObject::iterator it;
	for (it = m_slRenderBakemapObject.begin(); it != m_slRenderBakemapObject.end(); ++ it)
	{
		pMeshContainer = m_ModelList[(*it)->m_ObjectIndex];
		pMeshContainer->Render_BakemapBuffer((*it)->m_matWorldPosition);
	}
}

void _XMeshObjectManager::RenderReflectBakemapObject( )
{
	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle(&g_TerrainTextureArchive, 19);
	
	gpDev->SetFVF( D3DFVF_XMESH_VERTEX_UV2 );
	
	_XMeshContainer* pMeshContainer;
	sldef_OctreeObject::iterator it;
	for (it = m_slRenderBakemapObject.begin(); it != m_slRenderBakemapObject.end(); ++ it)
	{
		pMeshContainer = m_ModelList[(*it)->m_ObjectIndex];
		
		if( (*it)->m_AboveWater )
			pMeshContainer->Render_BakemapBuffer((*it)->m_matWorldPosition);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// Object frustum culling...

_XQuadTree::_XQuadTree()
{
	m_pRoot = NULL;	
	DisposeTree();

	m_CullingFarPlane = _XDEF_DEFAULTCULLINGFARPLANE;

#ifdef _XDWDEBUG
	m_PlayerCulling = FALSE;
	m_ViewFrustum	= FALSE;
	m_FrustumHeight = 5.0f;
#endif	
}	

_XQuadTree::~_XQuadTree()
{
	DisposeTree();
}	


void _XQuadTree::DisposeTree( void )
{
	SAFE_DELETE( m_pRoot );	
	
	m_pObjectList	= NULL;
	m_ObjectCount	= 0;
	m_pMeshObjectManager = NULL;

	m_LooseFactor = _XDEF_DEFAULT_LOOSEFACTOR;

	memset( m_FrustumTotals, 0, sizeof(int) * _XDEF_FRUSTUM_COUNT );
	memset( m_DepthTotals, 0, sizeof(int) * _XDEF_MAX_QTREEDEPTH );
	
	m_NodesChecked = 0;
	m_ObjectsActuallyInFrustum = 0;
	m_ObjectsChecked = 0;

	m_LastMouseX = m_LastMouseY = 0;

}

BOOL _XQuadTree::CreateTree( FLOAT worldsize, int objectcount, _XOctreeObject* objectlist )
{
	assert( objectlist );
	if( !objectlist ) return FALSE;
	
	DisposeTree();

	m_WorldSize = worldsize;

	m_ObjectCount = objectcount;
	m_pObjectList  = objectlist;
	
	// Set frumstum...
	for (int i = 0; i < _XDEF_FRUSTUM_COUNT; i++) 
	{
		_XFrustum&	f = m_FrustumList[i];
		f.x = (rand() / float(RAND_MAX) - 0.5) * m_WorldSize;
		f.z = (rand() / float(RAND_MAX) - 0.5) * m_WorldSize;
		float	theta = (rand() / float(RAND_MAX)) * 2 * _X_PI;

		SetupFrustumNormals(&f, theta, _X_RAD(g_LodTerrain.m_3PCamera.mp_fFov * _XDEF_EXPANDFRUSTUMFOV));
	}	

	m_CurrentFrustum.x = 0;
	m_CurrentFrustum.z = 0;
	
	SetupFrustumNormals(&m_CurrentFrustum, _XDEF_DEFAULT_ANGLE, _X_RAD(g_LodTerrain.m_3PCamera.mp_fFov * _XDEF_EXPANDFRUSTUMFOV) );
	
	memset( m_DepthTotals, 0, sizeof(int) * _XDEF_MAX_QTREEDEPTH );

	m_pRoot = new _XQNode(0, 0, 0, 0);
	for ( i = 0; i < m_ObjectCount; i++ )
	{
		_XOctreeObject& o = m_pObjectList[i];
		int	d = LooseQuadTreeInsert(m_pRoot, &o);
	}
		
	_XDWINPRINT("Quad tree culling informations==========================================", i, m_DepthTotals[i]);
	_XDWINPRINT("Quad tree depth totals : %d", i, m_DepthTotals[i]);

	for (i = 0; i < _XDEF_MAX_QTREEDEPTH; i++) 
	{
		_XDWINPRINT("ct[%d] = %d", i, m_DepthTotals[i]);
	}

	_XDWINPRINT("Node count = %d", CountNodes(m_pRoot));

	int ct = 0;
	m_NodesChecked = 0;
	m_ObjectsActuallyInFrustum = 0;
		
	for (i = 0; i < _XDEF_FRUSTUM_COUNT; i++) 
	{
		ct += LooseQuadTreeCountObjectsInFrustum(m_pRoot, &m_FrustumList[i], SOME_CLIP);
	}

	_XDWINPRINT("%d objects possibly in frustum %d objects actually in frustum  %d nodes checked.", ct, m_ObjectsActuallyInFrustum, m_NodesChecked);

	m_ObjectsChecked = 0;
	m_NodesChecked = 0;
	int	Contacts = 0;

	for (i = 0; i < m_ObjectCount; i++) 
	{
		Contacts += LooseCountContactingObjects(m_pRoot, &m_pObjectList[i]);
	}
	
	_XDWINPRINT("%d contacts, %d objects checked, %d nodes checked", Contacts, m_ObjectsChecked, m_NodesChecked);

	_XDWINPRINT("End of quad tree information reporting==================================", i, m_DepthTotals[i]);

	#define _XDEF_DISTANCENORMALRENDER	160.0f	//80.0f*2
	m_Alpharate = (_XDEF_DEFAULTCULLINGFARPLANE-_XDEF_DISTANCENORMALRENDER) / 256.0f;

	return TRUE;
}

BOOL _XQuadTree::FitsInBox(_XOctreeObject* o, FLOAT cx, FLOAT cz, FLOAT HalfSize)
{
	if (o->m_matWorldPosition._41 - o->m_Radius < cx - HalfSize ||
	    o->m_matWorldPosition._41 + o->m_Radius > cx + HalfSize ||
	    o->m_matWorldPosition._43 - o->m_Radius < cz - HalfSize ||
	    o->m_matWorldPosition._43 + o->m_Radius > cz + HalfSize)
	{
		return FALSE;
	} 
	
	return TRUE;	
}


_XVisibility _XQuadTree::CheckBoxAgainstFrustum(float cx, float cz, float HalfSize, _XFrustum* f)
{
	int	OrCodes = 0, AndCodes = ~0;

	int	i, j, k;
	// Check each box corner against each edge of the frustum.
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 2; i++) {
			int	mask = 1 << (j * 2 + i);
			
			FLOAT	x = cx + (i == 0 ? -HalfSize : HalfSize);
			FLOAT	z = cz + (j == 0 ? -HalfSize : HalfSize);

			// Check the corner against the two sides of the frustum.
			int	Code = 0;
			int	Bit = 1;
			for (k = 0; k < 3; k++, Bit <<= 1) {
				FLOAT	dot = f->nx[k] * (x - f->x) + f->nz[k] * (z - f->z);

				if (dot < 0) {
					// The point is outside this edge.
					Code |= Bit;
				}
			}

			OrCodes |= Code;
			AndCodes &= Code;
		}
	}

	// Based on bit-codes, return culling results.
	if (OrCodes == 0) 
	{
		// The box is completely within the frustum (no box vert is outside any frustum edge).
		return NO_CLIP;
	} 
	else if (AndCodes != 0) 
	{
		// All the box verts are outside one of the frustum edges.
		return NOT_VISIBLE;
	} 
	else 
	{
		return SOME_CLIP;
	}
}

_XVisibility _XQuadTree::CheckObjectAgainstFrustum(_XOctreeObject* o, _XFrustum* f)
// Returns the visibility of the object w/r/t the frustum.
{
	BOOL	In = TRUE;
	bool	On = FALSE;

	int	i;
	
	for (i = 0; i < 3; i++) 
	{
		FLOAT dist = f->nx[i] * (o->m_matWorldPosition._41 - f->x) + f->nz[i] * (o->m_matWorldPosition._43 - f->z);
		if (dist < - o->m_Radius)
		{
			// Object is completely on wrong side of this frustum plane.
			In = FALSE;
		} 
		else if (dist > o->m_Radius)
		{
			// Object is completely on right side of this plane.
		} 
		else 
		{
			// Object straddles the plane.
			On = TRUE;
		}
	}

	if (In) 
	{
		if (On) return SOME_CLIP;
		else return NO_CLIP;
	} 
	else 
	{
		return NOT_VISIBLE;
	}
}

_XVisibility _XQuadTree::CheckObjectAgainstCurrrentFrustum(D3DXVECTOR3 oVec, FLOAT radius )
{
	BOOL	In = TRUE;
	bool	On = FALSE;

	int	i;
	
	for (i = 0; i < 3; i++) 
	{
		FLOAT dist = m_CurrentFrustum.nx[i] * (oVec.x - m_CurrentFrustum.x) + m_CurrentFrustum.nz[i] * (oVec.z - m_CurrentFrustum.z);
		if (dist < - radius)
		{
			// Object is completely on wrong side of this frustum plane.
			In = FALSE;
		} 
		else if (dist > radius)
		{
			// Object is completely on right side of this plane.
		} 
		else 
		{
			// Object straddles the plane.
			On = TRUE;
		}
	}

	if (In) 
	{
		if (On) return SOME_CLIP;
		else return NO_CLIP;
	} 
	else 
	{
		return NOT_VISIBLE;
	}
}

BOOL _XQuadTree::CheckObjectAgainstObject(_XOctreeObject* o, _XOctreeObject* p)
// Returns true if the objects overlap.
{
	FLOAT	dx = o->m_matWorldPosition._41 - p->m_matWorldPosition._41;
	FLOAT	dz = o->m_matWorldPosition._43 - p->m_matWorldPosition._43;
	FLOAT	r2 = dx * dx + dz * dz;

	if (r2 > (o->m_Radius + p->m_Radius) * (o->m_Radius + p->m_Radius)) 
	{
		return FALSE;
	} 
	else 
	{
		return TRUE;
	}
}


int	_XQuadTree::QuadTreeInsert( _XQNode* q, _XOctreeObject* o)
// Insert the given object into the tree given by qnode.
// Returns the depth of the node the object was placed in.
{
	int	i, j;
	
	// Check child nodes to see if object fits in one of them.
	if (q->depth + 1 < _XDEF_MAX_QTREEDEPTH) 
	{
		float	HalfSize = m_WorldSize / (2 << q->depth);
		float	QuarterSize = HalfSize / 2;
		float	offset = QuarterSize;
		
		for (j = 0; j < 2; j++) 
		{
			for (i = 0; i < 2; i++) 
			{
				float	cx = q->cx + ((i == 0) ? -offset : offset);
				float	cz = q->cz + ((j == 0) ? -offset : offset);

				if (FitsInBox(o, cx, cz, QuarterSize)) 
				{
					// Recurse into this node.
					if (q->child[j][i] == 0) 
					{
						q->child[j][i] = new _XQNode(q, cx, cz, q->depth + 1);
					}
					
					return QuadTreeInsert(q->child[j][i], /* cx, cz, depth + 1, */ o);
				}
			}
		}
	}

	// Keep object in this node.
	o->m_nextObject = q->objects;
	q->objects = o;
	
	//DepthTotals[q->depth]++;
	
	return q->depth;
}


int	_XQuadTree::CountQTreeNodes( _XQNode* q )
{
	int	count = 1;	// Count ourself.

	// Count descendants.
	int	i, j;
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 2; i++) {
			if (q->child[j][i]) count += CountQTreeNodes(q->child[j][i]);
		}
	}

	return count;
}


int	_XQuadTree::CountNodes(_XQNode* q)
// Returns the number of nodes in the given subtree.
{
	int	count = 1;	// Count ourself.

	// Count descendants.
	int	i, j;
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 2; i++) {
			if (q->child[j][i]) count += CountNodes(q->child[j][i]);
		}
	}

	return count;
}

int	_XQuadTree::QuadTreeCountObjectsInFrustum(_XQNode* q, _XFrustum* f, _XVisibility vis)
// Counts the objects in q which might touch the given frustum, based on
// the information in the quadtree partitioning.
{
	//NodesChecked++;
	
	float	HalfSize = m_WorldSize / (2 << q->depth);
	
	if (vis == SOME_CLIP) 
	{
		vis = CheckBoxAgainstFrustum(q->cx, q->cz, HalfSize, f);
		if (vis == NOT_VISIBLE) return 0;
	}

	int	count = 0;
	
	// Check children.
	int	j, i;
	for (j = 0; j < 2; j++) 
	{
		for (i = 0; i < 2; i++) 
		{
			if (q->child[j][i]) 
			{
				count += QuadTreeCountObjectsInFrustum(q->child[j][i], f, vis);
			}
		}
	}

	// Count objects in this node and check frustum visibility.
	_XOctreeObject*	o = q->objects;
	while (o)
	{
		if (CheckObjectAgainstFrustum(o, f) != NOT_VISIBLE) m_ObjectsActuallyInFrustum++;
		
		count++;
		o = o->m_nextObject;
	}

	return count;
}

int	_XQuadTree::LooseQuadTreeInsert(_XQNode* q, _XOctreeObject* o)
// Insert the given object into the tree given by qnode.
// Returns the depth of the node the object was placed in.
{		
	// Check child nodes to see if object fits in one of them.
//	if (o->radius < WORLD_SIZE / (4 << q->depth)) {
	if (q->depth + 1 < _XDEF_MAX_QTREEDEPTH) 
	{
		FLOAT	HalfSize = _XDEF_DEFAULT_LOOSEFACTOR * m_WorldSize / (2 << q->depth);
		FLOAT	QuarterSize = HalfSize / 2;
		FLOAT	offset = (m_WorldSize / (2 << q->depth)) / 2;

		// Pick child based on classification of object's center point.
		int	i = (o->m_matWorldPosition._41 <= q->cx) ? 0 : 1;
		int	j = (o->m_matWorldPosition._43 <= q->cz) ? 0 : 1;

		FLOAT	cx = q->cx + ((i == 0) ? -offset : offset);
		FLOAT	cz = q->cz + ((j == 0) ? -offset : offset);

		if (FitsInBox(o, cx, cz, QuarterSize)) 
		{
			// Recurse into this node.
			if (q->child[j][i] == 0) 
			{
				q->child[j][i] = new _XQNode(q, cx, cz, q->depth + 1);
			}
			return LooseQuadTreeInsert(q->child[j][i], o);
		}
	}

	// Keep object in this node.
	o->m_nextObject = q->objects;
	q->objects = o;
	
	m_DepthTotals[q->depth]++;
	
	return q->depth;
}

int	_XQuadTree::LooseQuadTreeCountObjectsInFrustum(_XQNode* q, _XFrustum* f, _XVisibility vis)
// Counts the objects in q which might touch the given frustum, based on
// the information in the loose quadtree partitioning.
{
	FLOAT HalfSize = _XDEF_MAX_QTREEDEPTH * m_WorldSize / (2 << q->depth);
	
	if (vis == SOME_CLIP) 
	{
		vis = CheckBoxAgainstFrustum(q->cx, q->cz, HalfSize, f);
		if (vis == NOT_VISIBLE) return 0;
	}

	m_NodesChecked++;
	
	int	count = 0;
	
	// Check children.
	int	j, i;
	for (j = 0; j < 2; j++) 
	{
		for (i = 0; i < 2; i++) 
		{
			if (q->child[j][i]) 
			{
				count += LooseQuadTreeCountObjectsInFrustum(q->child[j][i], f, vis);
			}
		}
	}

	// Count objects in this node.
	_XOctreeObject*	o = q->objects;
	while (o)
	{
		if (CheckObjectAgainstFrustum(o, f) != NOT_VISIBLE) m_ObjectsActuallyInFrustum++;
		
		count++;
		o = o->m_nextObject;
	}

	return count;
}

int	_XQuadTree::LooseCountContactingObjects(_XQNode* q, _XOctreeObject* o)
// Returns the number of objects within the subtree q which contact
// the given object.  Increments ObjectsChecked for each object which
// is tested for contact.
{
	m_NodesChecked++;
	
	float	HalfSize = _XDEF_DEFAULT_LOOSEFACTOR * m_WorldSize / (2 << q->depth);
	
	// First check to see if the object is completely outside the boundary
	// of this node.
	float	dx = fabs(q->cx - o->m_matWorldPosition._41);
	float	dz = fabs(q->cz - o->m_matWorldPosition._43);
	if (dx - o->m_Radius > HalfSize || dz - o->m_Radius > HalfSize) 
	{
		// Object is completely outside the boundary of this
		// node; don't bother checking contents.
		return 0;
	}

	int	count = 0;
	
	// Check children.
	int	j, i;
	for (j = 0; j < 2; j++) 
	{
		for (i = 0; i < 2; i++) 
		{
			if (q->child[j][i]) count += LooseCountContactingObjects(q->child[j][i], o);
		}
	}

	// Check objects in this node.
	_XOctreeObject*	p = q->objects;
	while (p) 
	{
		if (o != p && CheckObjectAgainstObject(o, p)) 
		{
			count++;
		}
		
		m_ObjectsChecked++;
		p = p->m_nextObject;
	}

	return count;
}

void _XQuadTree::SetupFrustumNormals(_XFrustum* f, FLOAT ViewAngle, FLOAT FOV)
{
	FLOAT HALF_PI  = _X_PI/2.0f;
	FLOAT HALF_FOV = m_CameraFov/2.0f;

	f->nx[0] =  cos(ViewAngle - HALF_FOV + HALF_PI);
	f->nz[0] = -sin(ViewAngle - HALF_FOV + HALF_PI);
	
	f->nx[1] =  cos(ViewAngle + HALF_FOV - HALF_PI);
	f->nz[1] = -sin(ViewAngle + HALF_FOV - HALF_PI);

	f->nx[2] =  cos(ViewAngle);
	f->nz[2] = -sin(ViewAngle);
}

void _XQuadTree::SetupFrustumNormals( void )
// Sets the normals of the frustum boundaries using the given parameters.
{
	FLOAT HALF_PI  = _X_PI/2.0f;
	FLOAT HALF_FOV = m_CameraFov/2.0f;

	m_CurrentFrustum.nx[0] =  cos(m_CameraAngle - HALF_FOV + HALF_PI);
	m_CurrentFrustum.nz[0] = -sin(m_CameraAngle - HALF_FOV + HALF_PI);
	
	m_CurrentFrustum.nx[1] =  cos(m_CameraAngle + HALF_FOV - HALF_PI);
	m_CurrentFrustum.nz[1] = -sin(m_CameraAngle + HALF_FOV - HALF_PI);

	m_CurrentFrustum.nx[2] =  cos(m_CameraAngle);
	m_CurrentFrustum.nz[2] = -sin(m_CameraAngle);
}

FLOAT GetNearObjectAngle( _XOctreeObject* o )
{			
	FLOAT ax = g_LodTerrain.m_3PCamera.m_CameraPosition.x - g_pLocalUser->m_Position.x;
	FLOAT az = g_LodTerrain.m_3PCamera.m_CameraPosition.z - g_pLocalUser->m_Position.z;

	FLOAT bx = o->m_matWorldPosition._41 - g_pLocalUser->m_Position.x;
	FLOAT bz = o->m_matWorldPosition._43 - g_pLocalUser->m_Position.z;

	return atan2( ax - bx, az - bz );
}

void _XQuadTree::_Render(_XQNode* q, _XFrustum* f, _XVisibility vis)
// Render the objects in the frustum.
{
	float	HalfSize = _XDEF_DEFAULT_LOOSEFACTOR * m_WorldSize / (2 << q->depth);
	
	if (vis == SOME_CLIP) {
		vis = CheckBoxAgainstFrustum(q->cx, q->cz, HalfSize, f);
		if (vis == NOT_VISIBLE) return;
	}

	float	d = (q->depth / 5.0) + 0.2;
	if (d > 1) d = 1;
		
	// Render objects in this node.	
	_XOctreeObject*	o = q->objects;
	while (o) 
	{
		if (CheckObjectAgainstFrustum(o, f) != NOT_VISIBLE) 
		{				
			FLOAT dx = (f->x - o->m_matWorldPosition._41) ;
			FLOAT dz = (f->z - o->m_matWorldPosition._43) ;
			
			o->m_Distance = dx*dx + dz*dz; //_XFastDistance(dx,dz); // dx*dx + dz*dz;
						
			if( m_CullingFarPlane > o->m_Distance )
			{
				if( o->m_Distance > 7200.0f )
				{
					if( m_pMeshObjectManager->m_ModelList[o->m_ObjectIndex]->m_ObjectRadius > 4.0f )
					{				
						m_pMeshObjectManager->_RenderObject( *o );
#ifdef _XDWDEBUG
						m_RenderedObject++;
#endif
					}
				}
				else
				{
					m_pMeshObjectManager->_RenderObject( *o );
#ifdef _XDWDEBUG
					m_RenderedObject++;
#endif
				}
			}			
		} 		
		
		o = o->m_nextObject;
	}

	// Render children.
	int	j, i;
	for (j = 0; j < 2; j++) 
	{
		for (i = 0; i < 2; i++) 
		{
			if (q->child[j][i]) 
			{
				_Render(q->child[j][i], f, vis);
			}
		}
	}
}

int CompareDistance(const void *pArg1, const void *pArg2)
{
	_XOctreeObject** o1 = (_XOctreeObject**)pArg1;
	_XOctreeObject** o2 = (_XOctreeObject**)pArg2;
	return ( (o1[0]->m_Distance < o2[0]->m_Distance) ? 1 : -1);	
}

void _XQuadTree::_Render( _XMeshObjectManager* meshobjectmanager )
{	
#ifdef _XDWDEBUG
	m_RenderedObject = 0;
	m_AlphaBlendObject = 0;
#endif

	if( meshobjectmanager )
	{
		m_pMeshObjectManager = meshobjectmanager;
		g_AlphaBlendRenderObjectCount = 0;

		meshobjectmanager->m_slRenderObject.clear();
		meshobjectmanager->m_slRenderBakemapObject.clear();
		// Blit normal rendering...
		_Render(m_pRoot, &m_CurrentFrustum, SOME_CLIP);	
	}
}

void _XQuadTree::SetCameraInfo( _X3PCamera& camera )
{
	FLOAT expandfrustum;
	switch( g_XGQProperty.sightangle ) 
	{	
	case _XGQLEVEL_MAX		: 
	case _XGQLEVEL_HIGH		: expandfrustum = 2.24f;
		break;
	default					:
	case _XGQLEVEL_MIDDLE	: expandfrustum = 1.83f;
							  //expandfrustum = _XDEF_EXPANDFRUSTUMFOV;
		break;
	case _XGQLEVEL_LOW		: expandfrustum = _XDEF_EXPANDFRUSTUMFOV;
		break;	
	} 

	m_CameraAngle = _X_RAD( camera.mp_fYaw + 90.0f );
	m_CameraFov   = _X_RAD( camera.mp_fFov * expandfrustum ); 
}

void _XQuadTree::UpdateCamera( _X3PCamera& camera )
{	
#ifdef _XDWDEBUG
	if( m_PlayerCulling )
	{
		m_CurrentFrustum.x = camera.m_TargetPosition.x;
		m_CurrentFrustum.z = camera.m_TargetPosition.z;
		m_FrustumHeight	   = camera.m_TargetPosition.y;

		FLOAT pitch = (camera.mp_fPitch >= 0.0f ) ? camera.mp_fPitch : 0.0f;
		if( pitch > 90.0f ) pitch = 90.0f;

		//m_CullingFarPlane = (_XDEF_DEFAULTCULLINGSQRTFARPLANE * ( (90.0f-pitch) / 90.0f ));
		m_CullingFarPlane = ( (g_LodTerrain.m_LODFarPlane*g_LodTerrain.m_LODFarPlane) * ( (90.0f-pitch) / 90.0f ));

		if( m_CullingFarPlane < _XDEF_MINIMUMCULLINGFARPLANE ) 
			m_CullingFarPlane = _XDEF_MINIMUMCULLINGFARPLANE ;
	}
	else
	{
#endif

	m_CurrentFrustum.x = camera.m_CameraPosition.x;
	m_CurrentFrustum.z = camera.m_CameraPosition.z;

	// 전면부 Culling far plane 조절
	FLOAT pitch = (camera.mp_fPitch >= 0.0f ) ? camera.mp_fPitch : 0.0f;
	if( pitch > 90.0f ) pitch = 90.0f;

	//m_CullingFarPlane = ( _XDEF_DEFAULTCULLINGSQRTFARPLANE * ( (90.0f-pitch) / 90.0f ));
	m_CullingFarPlane = ( ((g_LodTerrain.m_LODFarPlane*g_LodTerrain.m_LODFarPlane) * 2.0f ) * ( (90.0f-pitch) / 90.0f ));
	
	if( m_CullingFarPlane < _XDEF_MINIMUMCULLINGFARPLANE ) 
		m_CullingFarPlane = _XDEF_MINIMUMCULLINGFARPLANE ;
	

#ifdef _XDWDEBUG
	
	m_FrustumHeight	= camera.m_TargetPosition.y;
	}
#endif
	
	m_CameraAngle = _X_RAD( camera.mp_fYaw + 90.0f );

	FLOAT expandfrustum;
	switch( g_XGQProperty.sightangle ) 
	{	
	case _XGQLEVEL_MAX		: 
	case _XGQLEVEL_HIGH		: expandfrustum = 2.24f;
		break;
	default					:
	case _XGQLEVEL_MIDDLE	: expandfrustum = 1.82f;
							  //expandfrustum = _XDEF_EXPANDFRUSTUMFOV;
		break;
	case _XGQLEVEL_LOW		: expandfrustum = _XDEF_EXPANDFRUSTUMFOV;
		break;	
	} 
	
	SetupFrustumNormals(&m_CurrentFrustum, m_CameraAngle, _X_RAD(camera.mp_fFov * expandfrustum) );
}
