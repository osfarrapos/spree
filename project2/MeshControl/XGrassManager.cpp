// XGrassManager.cpp: implementation of the _XGrassManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XGrassManager.h"

#include "EODEXTDEF_GlobalObject.h"
#include "EODEXTDEF_ModelData.h"

#include "XFastCode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XGrassManager::_XGrassManager()
{
	m_nGrassTypeCount = 0;

	GRASS_ALPHA_DIST		= 32.f;
	GRASS_ALPHA_START		= 48.f;
	GRASS_ALPHA_END			= 43.f;

	m_pGrassMesh			= NULL;
	m_pGrassModelDescriptor = NULL;
	m_pGrassBiped			= NULL;
}

_XGrassManager::~_XGrassManager()
{

}

BOOL _XGrassManager::DeleteData( void )
{
	SAFE_DELETE_ARRAY( m_pGrassMesh );
	SAFE_DELETE_ARRAY( m_pGrassModelDescriptor );
	SAFE_DELETE_ARRAY( m_pGrassBiped );

	m_Pool.DeleteData();

	m_nGrassTypeCount	= 0;
	return TRUE;
}

BOOL _XGrassManager::CreateData( void )
{
	DeleteData();

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_ENVIRONMENT.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_ENVIRONMENT.XP") );
#endif
	
	if( !g_RTimeObjectTextureArchive.SetTexturePackage( packagefilename ) ) 
		return FALSE;
	
	if( !g_ModelArchive.ReOpenPackage() ) 
		return FALSE;

	if( !g_AniArchive.ReOpenPackage() ) 
		return FALSE;
	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* fileptr = NULL;

	fileptr = g_ScriptArchive.GetPackedFile(_T("GrassModel.XMS"));
	
	if(!fileptr)
	{
		g_ScriptArchive.ClosePackage();
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return FALSE;
	}
	
	int		typecount = 0;
	TCHAR	readbuffer[256];
	
	fscanf( fileptr, "%d", &typecount );

	m_nGrassTypeCount = typecount;

	if( typecount <= 0 ) 
	{
		g_ScriptArchive.ClosePackage();
		g_AniArchive.ClosePackage();
		g_ModelArchive.ClosePackage();
		return TRUE;
	}

	m_pGrassMesh = new _XNSAnimMeshContainer[m_nGrassTypeCount];
	m_pGrassModelDescriptor = new _XModelDescriptor[m_nGrassTypeCount];
	m_pGrassBiped = new _XBipedController[m_nGrassTypeCount];

	int loadedcount = 0;

	do 
	{
		if(feof(fileptr)) break;

		memset(readbuffer, 0, sizeof(TCHAR)*256);
		if(!fgets(readbuffer, 255, fileptr)) break;

		if(strncmp(readbuffer, _T("_XM_EOS"), 7) == 0) break;

		if((strlen(readbuffer) > 0) && readbuffer[0] != _T(';') && readbuffer[0] != _T('\n') && readbuffer[0] != _T('\r'))
		{
			TCHAR bipedname[64],animname[64],modelname[64];
			memset( bipedname,0, sizeof(TCHAR)*64 );
			memset( animname,0, sizeof(TCHAR)*64 );
			memset( modelname,0, sizeof(TCHAR)*64 );
			sscanf( readbuffer, "%s %s %s", bipedname, modelname, animname );

			if( !m_pGrassBiped[loadedcount].Load( g_ModelArchive.GetPackedFile( bipedname ) ) )
			{
				g_ScriptArchive.ClosePackage();
				g_AniArchive.ClosePackage();
				g_ModelArchive.ClosePackage();
				return FALSE;
			}
			
			m_pGrassBiped[loadedcount].ReserveAnimationStack( 1 );
			
			if( !m_pGrassBiped[loadedcount].LoadKey( g_AniArchive.GetPackedFile( animname ) ) )
			{
				g_ScriptArchive.ClosePackage();
				g_AniArchive.ClosePackage();
				g_ModelArchive.ClosePackage();
				return FALSE;
			}		

			if( !m_pGrassMesh[loadedcount].Load( g_ModelArchive.GetPackedFile( modelname ),
									  &g_RTimeObjectTextureArchive, TRUE, modelname )  )
			{
				g_ScriptArchive.ClosePackage();
				g_AniArchive.ClosePackage();
				g_ModelArchive.ClosePackage();
				return FALSE;
			}

			m_pGrassModelDescriptor[loadedcount].ReserveModelStack( 1 );
			m_pGrassModelDescriptor[loadedcount].LinkBipedObject( &m_pGrassBiped[loadedcount] );
			m_pGrassModelDescriptor[loadedcount].SetMeshModel( 0, &m_pGrassMesh[loadedcount] );

			m_pGrassModelDescriptor[loadedcount].SetCurMotion( 0, true );
			m_pGrassModelDescriptor[loadedcount].PlayAnimation(TRUE);

			loadedcount++;

			if( loadedcount >= m_nGrassTypeCount ) break;
		}
	} while(!feof(fileptr));

	g_ScriptArchive.ClosePackage();
	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();

	m_Pool.Reserve(1000);

	switch( g_XGQProperty.worldobjectqualitylevel ) 
	{
	default:
	case _XGQLEVEL_MAX :
		g_TextureQuality		= 1;
		break;
	case _XGQLEVEL_HIGH:
		g_TextureQuality		= 2;
		break;
	case _XGQLEVEL_MIDDLE :
		g_TextureQuality		= 4;
		break;
	case _XGQLEVEL_LOW :
		g_TextureQuality		= 8;
		break;	
	} 

	_XTEXTUREPRECACHEING_ON
	if( !g_RTimeObjectTextureArchive.BuildTextureList() ) return FALSE;
	_XTEXTUREPRECACHEING_OFF
	if( !g_RTimeObjectTextureArchive.InitDeviceObject() ) return FALSE;

	g_TextureQuality		= 1;

	return TRUE;
}

void _XGrassManager::Render(const D3DXVECTOR3& vCameraPos)
{
	if (m_slRenderObject.empty()) return;

	if( g_bUseRealtimeGenerateObjectAni )
	{
		if( g_pLocalUser->GetMotionClass() != _XACTION_MOVE )
		{
			bool soundplay = false;

			for( int i = 0; i < m_nGrassTypeCount; i++ )
			{
				m_pGrassModelDescriptor[i].Advance(soundplay);
				m_pGrassModelDescriptor[i].Animation();
			}
		}
	}
//	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_GRASS );

	FLOAT dx, dz;
	_XRealTimeObject* pRenderObject = NULL;
	sldef_XRealTimeObject::iterator it;
	for (it = m_slRenderObject.begin(); it != m_slRenderObject.end(); ++ it)
	{
		pRenderObject = (*it);

		
		dx = (vCameraPos.x - pRenderObject->m_matWorldPosition._41);
		dz = (vCameraPos.z - pRenderObject->m_matWorldPosition._43);
		
		pRenderObject->m_Distance = _XFC_sqrt(dx*dx + dz*dz);
		if (pRenderObject->m_Distance > GRASS_ALPHA_END) continue;
		if (pRenderObject->m_Distance < GRASS_ALPHA_START)
		{
			gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, 0xffffffff );
		}
		else
		{
			BYTE alpha = 255 - ((pRenderObject->m_Distance - GRASS_ALPHA_START) / GRASS_ALPHA_DIST) * 255;
			gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, (alpha << 24) | 0x00ffffff );
		}

		m_pGrassModelDescriptor[pRenderObject->m_ObjectIndex].RenderCustomDraw(pRenderObject->m_matWorldPosition);
	}

//	gpDev->SetRenderState( D3DRS_FOGENABLE, m_Fog );
}