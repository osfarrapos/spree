#include <stdafx.h>

#pragma warning (disable: 4786)

#include "XSTWrapper.h"
#include "XSTShader.h"
#include "XSTWrapperCore.h"
#include "EODEXTDEF_GlobalObject.h"

using namespace std;

///////////////////////////////////////////////////////////////////////  
//	Static member variable declarations
LPDIRECT3DTEXTURE9				_XST_AbstractObject::m_texShadow = NULL;

///////////////////////////////////////////////////////////////////////  
//	Global member variable declarations
_XTextureManager	g_SpeedTreeTextureArchive;

///////////////////////////////////////////////////////////////////////  
// HLSL shaders
LPDIRECT3DVERTEXSHADER9		g_dwBranchVertexShader;				// branch vertex shader
LPDIRECT3DVERTEXSHADER9		g_dwLeafVertexShader;				// leaf vertex shader


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::_XST_AbstractObject
//
//	Main constructor

_XST_AbstractObject::_XST_AbstractObject( ) :
	m_pSpeedTree(new CSpeedTreeRT),
	m_bIsInstance(false),
	m_pInstanceOf(NULL),
	m_pGeometryCache(NULL),
	m_usNumLeafLods(0),
	m_pBranchIndexCounts(NULL),
	m_pBranchIndexBuffer(NULL),
	m_pBranchVertexBuffer(NULL),
	m_pFrondIndexCounts(NULL),
	m_pFrondIndexBuffer(NULL),
	m_pFrondVertexBuffer(NULL),
	m_pLeafVertexBuffer(NULL),
	m_pLeavesUpdatedByCpu(NULL),
	m_pTextureInfo(NULL)
{
	m_afPos[0] = m_afPos[1] = m_afPos[2] = 0.0f;
	
	// wind states
	m_pSpeedTree->SetWindStrength(1.0f);
	m_pSpeedTree->SetLocalMatrices(0, 4);
	m_pSpeedTree->SetTextureFlip(true);

	m_STObjectRenderStack  = NULL;
	m_STObjectRenderStackSize  = 0;
}

///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::~_XST_AbstractObject

_XST_AbstractObject::~_XST_AbstractObject( )
{
	// if this is not an instance, then clean up
	if (!m_bIsInstance)
	{
		for (int i = 0; i < m_vInstances.size( ); ++i) delete m_vInstances[i];

		if (m_unBranchVertexCount > 0)
		{
			SAFE_RELEASE(m_pBranchVertexBuffer);
			SAFE_RELEASE(m_pBranchIndexBuffer);
			SAFE_DELETE_ARRAY(m_pBranchIndexCounts);
		}
		
		if (m_unFrondVertexCount > 0)
		{	
			SAFE_RELEASE(m_pFrondVertexBuffer);
			SAFE_RELEASE(m_pFrondIndexBuffer);
			SAFE_DELETE_ARRAY(m_pFrondIndexCounts);
		}
		
		for ( i = 0; i < m_usNumLeafLods; ++i)
		{
			SAFE_RELEASE( m_pLeafVertexBuffer[i] );
			/*
			m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_LeafGeometry, -1, -1, i);
			if (m_pGeometryCache->m_sLeaves0.m_usLeafCount > 0u)
				SAFE_RELEASE(m_pLeafVertexBuffer[i]);
			*/
		}
		SAFE_DELETE_ARRAY(m_pLeafVertexBuffer);
		SAFE_DELETE_ARRAY(m_pLeavesUpdatedByCpu);
		
		SAFE_DELETE(m_pGeometryCache);
		SAFE_DELETE(m_pTextureInfo);
	}
	
	// always delete the SpeedTree
	SAFE_DELETE(m_pSpeedTree);
	
	SAFE_DELETE_ARRAY( m_STObjectRenderStack );	
}


void _XST_AbstractObject::SetupBuffers(void)
{
	// read all the geometry for highest LOD into the geometry cache
	m_pSpeedTree->SetLodLevel(1.0f);
	if (m_pGeometryCache == NULL)
		m_pGeometryCache = new CSpeedTreeRT::SGeometry;
	m_pSpeedTree->GetGeometry(*m_pGeometryCache);
	
	// setup the buffers for each tree part
	SetupBranchBuffers( );
	SetupFrondBuffers( );
	SetupLeafBuffers( );
}

///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::MakeInstance

_XST_AbstractObject* _XST_AbstractObject::MakeInstance(BOOL useshader, float fX, float fY, float fZ)
{
	// create a new object
	_XST_AbstractObject* pInstance;
	
	if( useshader )
	{
		pInstance = new _XST_VSObject;
	}
	else
	{
		pInstance = new _XST_NSObject;
	}
	
	// make an instance of this object's SpeedTree
	pInstance->m_bIsInstance = true;
	pInstance->m_pSpeedTree = m_pSpeedTree->MakeInstance( );
	if (pInstance->m_pSpeedTree)
    {
		// use the same materials
		pInstance->m_cBranchMaterial = m_cBranchMaterial;
		pInstance->m_cLeafMaterial = m_cLeafMaterial;
		pInstance->m_cFrondMaterial = m_cFrondMaterial;		
		pInstance->m_texShadow = m_texShadow;
		pInstance->m_pTextureInfo = m_pTextureInfo;
		
		pInstance->m_LeafTextureIndex =  m_LeafTextureIndex;
		pInstance->m_BranchTextureIndex = m_BranchTextureIndex;
		
		// use the same geometry cache
		pInstance->m_pGeometryCache = m_pGeometryCache;
		
		// use the same buffers
		pInstance->m_pBranchIndexBuffer = m_pBranchIndexBuffer;
		pInstance->m_pBranchIndexCounts = m_pBranchIndexCounts;
		pInstance->m_pBranchVertexBuffer = m_pBranchVertexBuffer;
		pInstance->m_unBranchVertexCount = m_unBranchVertexCount;
		
		pInstance->m_pFrondIndexBuffer = m_pFrondIndexBuffer;
		pInstance->m_pFrondIndexCounts = m_pFrondIndexCounts;
		pInstance->m_pFrondVertexBuffer = m_pFrondVertexBuffer;
		pInstance->m_unFrondVertexCount = m_unFrondVertexCount;
		
		pInstance->m_pLeafVertexBuffer = m_pLeafVertexBuffer;
		pInstance->m_usNumLeafLods = m_usNumLeafLods;
		pInstance->m_pLeavesUpdatedByCpu = m_pLeavesUpdatedByCpu;
		
		pInstance->m_pSpeedTree->SetTreePosition(fX, fY-0.7f, fZ);
		
		// new stuff
		memcpy(pInstance->m_afPos, m_afPos, 3 * sizeof(float));
		memcpy(pInstance->m_afBoundingBox, m_afBoundingBox, 6 * sizeof(float));
		pInstance->m_pInstanceOf = this;
		m_vInstances.push_back(pInstance);
    }
    else
	{
		_XFatalError("SpeedTreeRT Error: %s\n", m_pSpeedTree->GetCurrentError( ));
        delete pInstance;
        pInstance = NULL;
	}
	
	return pInstance;
}


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::GetInstances

_XST_AbstractObject** _XST_AbstractObject::GetInstances(unsigned int& nCount)
{
	nCount = m_vInstances.size( );
	return &(m_vInstances[0]);
}


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::RenderBranches

void _XST_AbstractObject::RenderBranches(void)
{
	m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BranchGeometry);
	
	if (m_pGeometryCache->m_fBranchAlphaTestValue > 0.0f)
	{
		PositionTree( );
		
		// set alpha test value
		//gpDev->SetRenderState(D3DRS_ALPHAREF, DWORD(m_pGeometryCache->m_fBranchAlphaTestValue));
				
		// if tree has branches at this LOD, draw them
		if (m_pBranchIndexCounts && m_pGeometryCache->m_sBranches.m_nDiscreteLodLevel > -1 && m_pBranchIndexCounts[m_pGeometryCache->m_sBranches.m_nDiscreteLodLevel] > 0)
			gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_pGeometryCache->m_sBranches.m_usVertexCount, 0, m_pBranchIndexCounts[m_pGeometryCache->m_sBranches.m_nDiscreteLodLevel] - 2);
	}
}


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::RenderFronds

void _XST_AbstractObject::RenderFronds(void)
{
	m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_FrondGeometry);
	
	if (m_pGeometryCache->m_fFrondAlphaTestValue > 0.0f)
	{
		PositionTree( );
		gpDev->SetRenderState(D3DRS_ALPHAREF, DWORD(m_pGeometryCache->m_fFrondAlphaTestValue));
		
		// if tree has fronds, draw them
		if (m_pFrondIndexCounts && m_pGeometryCache->m_sFronds.m_nDiscreteLodLevel > -1 && m_pFrondIndexCounts[m_pGeometryCache->m_sFronds.m_nDiscreteLodLevel] > 0)
			gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_pGeometryCache->m_sFronds.m_usVertexCount, 0, m_pFrondIndexCounts[m_pGeometryCache->m_sFronds.m_nDiscreteLodLevel] - 2);		
	}
}


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::UploadLeafTables

void _XST_AbstractObject::UploadLeafTables(unsigned int uiLocation)
{
	if( g_XST_Wrapper.m_UseVertexShader )
	{
		// query leaf cluster table from RT
		unsigned int uiEntryCount = 0;
		const float* pTable = m_pSpeedTree->GetLeafBillboardTable(uiEntryCount);
		
		// upload for vertex shader use
		gpDev->SetVertexShaderConstantF(uiLocation, pTable, uiEntryCount / 4);
	}
}

///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::EndLeafForTreeType

void _XST_AbstractObject::EndLeafForTreeType(void)
{
	// reset flags for CPU data copying
	for (unsigned int i = 0; i < m_usNumLeafLods; ++i)
		m_pLeavesUpdatedByCpu[i] = false;
}

///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::RenderBillboards

void _XST_AbstractObject::RenderBillboards(void)
{
	// draw billboards in immediate mode (as close as directx gets to immediate mode)
#ifdef WRAPPER_BILLBOARD_MODE
	PositionTree( );	
	
	struct SBillboardVertex 
	{
		float fX, fY, fZ;
		DWORD dColor;
		float fU, fV;
	};
	
	m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BillboardGeometry);
	
	if (m_pGeometryCache->m_sBillboard0.m_bIsActive)
	{
		const float* pCoords = m_pGeometryCache->m_sBillboard0.m_pCoords;
		const float* pTexCoords = m_pGeometryCache->m_sBillboard0.m_pTexCoords;
		SBillboardVertex sVertex[4] = 
		{
			{ pCoords[0], pCoords[1], pCoords[2], 0xffffff, pTexCoords[0], pTexCoords[1] },
			{ pCoords[3], pCoords[4], pCoords[5], 0xffffff, pTexCoords[2], pTexCoords[3] },
			{ pCoords[6], pCoords[7], pCoords[8], 0xffffff, pTexCoords[4], pTexCoords[5] },
			{ pCoords[9], pCoords[10], pCoords[11], 0xffffff, pTexCoords[6], pTexCoords[7] },
		};		
		gpDev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		
		DWORD alphatestvalue = (DWORD)m_pGeometryCache->m_sBillboard0.m_fAlphaTestValue;
		gpDev->SetRenderState(D3DRS_ALPHAREF, alphatestvalue);
		
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, sVertex, sizeof(SBillboardVertex));
	}
	
	// if there is a 360 degree billboard, then we need to draw the second one
	/*
	if (m_pGeometryCache->m_sBillboard1.m_bIsActive)
	{
		const float* pCoords = m_pGeometryCache->m_sBillboard1.m_pCoords;
		const float* pTexCoords = m_pGeometryCache->m_sBillboard1.m_pTexCoords;
		SBillboardVertex sVertex[4] = 
		{
			{ pCoords[0], pCoords[1], pCoords[2], 0xffffff, pTexCoords[0], pTexCoords[1] },
			{ pCoords[3], pCoords[4], pCoords[5], 0xffffff, pTexCoords[2], pTexCoords[3] },
			{ pCoords[6], pCoords[7], pCoords[8], 0xffffff, pTexCoords[4], pTexCoords[5] },
			{ pCoords[9], pCoords[10], pCoords[11], 0xffffff, pTexCoords[6], pTexCoords[7] },
		};
		gpDev->SetRenderState(D3DRS_ALPHAREF, DWORD(m_pGeometryCache->m_sBillboard1.m_fAlphaTestValue));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, sVertex, sizeof(SBillboardVertex));
	}
	
	// if we have a horizontal bilboard and it is enabled, draw it too 
#ifdef WRAPPER_RENDER_HORIZONTAL_BILLBOARD
	if (m_pGeometryCache->m_sHorizontalBillboard.m_bIsActive)
	{	
		const float* pCoords = m_pGeometryCache->m_sHorizontalBillboard.m_pCoords;
		const float* pTexCoords = m_pGeometryCache->m_sHorizontalBillboard.m_pTexCoords;
		SBillboardVertex sVertex[4] = 
		{
			{ pCoords[0], pCoords[1], pCoords[2], 0xffffffff, pTexCoords[0], pTexCoords[1] },
			{ pCoords[3], pCoords[4], pCoords[5], 0xffffffff, pTexCoords[2], pTexCoords[3] },
			{ pCoords[6], pCoords[7], pCoords[8], 0xffffffff, pTexCoords[4], pTexCoords[5] },
			{ pCoords[9], pCoords[10], pCoords[11], 0xffffffff, pTexCoords[6], pTexCoords[7] },
		};
		gpDev->SetRenderState(D3DRS_ALPHAREF, DWORD(m_pGeometryCache->m_sHorizontalBillboard.m_fAlphaTestValue));
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, sVertex, sizeof(SBillboardVertex));
	}
		
#endif
	*/
#endif
}



///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::CleanUpMemory

void _XST_AbstractObject::CleanUpMemory(void)
{
	if (!m_bIsInstance)
		m_pSpeedTree->DeleteTransientData( );
}

///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::SetPos

void _XST_AbstractObject::SetPos(const float* pPos)
{
	m_afPos[0] = pPos[0];
	m_afPos[1] = pPos[1];
	m_afPos[2] = pPos[2];
	
	m_pSpeedTree->SetTreePosition(pPos[0], pPos[1], pPos[2]);
}


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::PositionTree

void _XST_AbstractObject::PositionTree(void)
{
	D3DXVECTOR3 vecPosition = m_pSpeedTree->GetTreePosition( );
	D3DXMATRIX matTranslation;
	D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, vecPosition.x, vecPosition.y, vecPosition.z);
	
	// store translation for client-side transformation
	gpDev->SetTransform(D3DTS_WORLD, &matTranslation);
	
	if( g_XST_Wrapper.m_UseVertexShader )
	{
		// store translation for use in vertex shader
		D3DXVECTOR4 vecConstant(vecPosition[0], vecPosition[1], vecPosition[2], 0.0f);
		gpDev->SetVertexShaderConstantF(c_nVertexShader_TreePos, (float*)&vecConstant, 1);
	}
}


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject::SetShaderConstants

void _XST_AbstractObject::SetShaderConstants(const float* pMaterial)
{
	if( g_XST_Wrapper.m_UseVertexShader )
	{
		const float afUsefulConstants[] = 
		{
			m_pSpeedTree->GetLeafLightingAdjustment( ), 0.0f, 0.0f, 0.0f
		};
		gpDev->SetVertexShaderConstantF(c_nVertexShader_LeafLightingAdjustment, afUsefulConstants, 1);
		
		const float afMaterial[] = 
		{
			pMaterial[0], pMaterial[1], pMaterial[2], 1.0f,
				pMaterial[3], pMaterial[4], pMaterial[5], 1.0f
		};
		gpDev->SetVertexShaderConstantF(c_nVertexShader_Material, afMaterial, 2);
	}
}

///////////////////////////////////////////////////////////////////////  
//	Speed tree object rendering core...
BOOL _XST_AbstractObject::ReserveRenderStack( void )
{
	m_STObjectRenderStack = new _XST_AbstractObject*[_XDEF_MAX_SPEEDTREERENDERSTACK];
	m_STObjectRenderStackSize = 0;
	
	return TRUE;
}

void _XST_AbstractObject::ResetRenderStack( void )
{
	m_STObjectRenderStackSize = 0;
	if( m_STObjectRenderStack )
	{
		memset( m_STObjectRenderStack, 0, sizeof(_XST_AbstractObject*) * _XDEF_MAX_SPEEDTREERENDERSTACK );		
	}
}

void _XST_AbstractObject::InsertRenderingInstance( _XST_AbstractObject* pinstance )
{
	if( !m_STObjectRenderStack ) return;
	if( m_STObjectRenderStackSize >= _XDEF_MAX_SPEEDTREERENDERSTACK-1 ) return;
	
	m_STObjectRenderStack[m_STObjectRenderStackSize] = pinstance;
	m_STObjectRenderStackSize++;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	_XST_VSObject
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::LoadTree
//
//  Returns true upon success, false in failure

bool _XST_VSObject::LoadTree(const char* pszSptFile, _XPackageArchive* ModelArchive,
									LPTSTR	pszCompositionMapname, int& compositionmapindex,
									unsigned int nSeed, float fSize, float fSizeVariance, 
									float fX, float fY, float fZ)
{
	if( !ModelArchive ) return false;

    bool bSuccess = false;

	int modelindex = ModelArchive->FindResource( (LPTSTR)pszSptFile );

	if( modelindex == -1 )
	{
		_XFatalError( "Cannot find tree archive[%s]", pszSptFile );
		return false;
	}

	LPBYTE pmodelarchive = NULL;
	LONG   archivesize = 0;
	
	pmodelarchive = ModelArchive->LoadPackedFileToMemory( (DWORD)modelindex, &archivesize );
	if( !pmodelarchive )
	{
		return false;
	}

	// directx, so allow for flipping of the texture coordinate
	//m_pSpeedTree->SetTextureFlip(true);
	m_pSpeedTree->SetTextureFlip(true);
        
	// Load the tree file (true == success)		

	if (m_pSpeedTree->LoadTree((const unsigned char*)pmodelarchive,(unsigned int)archivesize))
	{
		// override the lighting method stored in the spt file
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
		m_pSpeedTree->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		m_pSpeedTree->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		m_pSpeedTree->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
#else
		m_pSpeedTree->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_STATIC);
		m_pSpeedTree->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_STATIC);
		m_pSpeedTree->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_STATIC);
#endif
		
		// set the wind method
		m_pSpeedTree->SetBranchWindMethod(CSpeedTreeRT::WIND_GPU);
		m_pSpeedTree->SetLeafWindMethod(CSpeedTreeRT::WIND_GPU);
		m_pSpeedTree->SetFrondWindMethod(CSpeedTreeRT::WIND_GPU);

		m_pSpeedTree->SetNumLeafRockingGroups(1);

		// override the size, if necessary
        if (fSize >= 0.0f && fSizeVariance >= 0.0f)
            m_pSpeedTree->SetTreeSize(fSize, fSizeVariance);
        
		// generate tree geometry
        if (m_pSpeedTree->Compute(NULL, nSeed))
        {
			// get the dimensions & adjust LOD values
			m_pSpeedTree->GetBoundingBox(m_afBoundingBox);

			// make the leaves rock in the wind
			m_pSpeedTree->SetLeafRockingState(true);

			// billboard setup
#ifdef WRAPPER_BILLBOARD_MODE
			CSpeedTreeRT::SetDropToBillboard(true);
#else
			CSpeedTreeRT::SetDropToBillboard(false);
#endif

			// query & set materials
			m_cBranchMaterial.Set(m_pSpeedTree->GetBranchMaterial( ));
			m_cFrondMaterial.Set(m_pSpeedTree->GetFrondMaterial( ));
			m_cLeafMaterial.Set(m_pSpeedTree->GetLeafMaterial( ));

			// adjust lod distances
			FLOAT _fNearLodFactor = 50;
			FLOAT _fFarLodFactor = 150;
            float fHeight = (m_afBoundingBox[5] - m_afBoundingBox[2]);
            m_pSpeedTree->SetLodLimits( (fHeight * _fNearLodFactor)/ fSize, (fHeight * _fFarLodFactor) / fSize );
			//m_pSpeedTree->SetLodLimits(c_fNearLodFactor, c_fFarLodFactor);
            
			// query textures
			m_pTextureInfo = new CSpeedTreeRT::STextures;
			m_pSpeedTree->GetTextures(*m_pTextureInfo);

			// load branch texture
			compositionmapindex  = m_LeafTextureIndex = g_SpeedTreeTextureArchive.GetResourceIndex( pszCompositionMapname, 3 );
			m_BranchTextureIndex = g_SpeedTreeTextureArchive.GetResourceIndex( (LPTSTR)m_pTextureInfo->m_pBranchTextureFilename, 3 );

			/*
#ifdef WRAPPER_RENDER_SELF_SHADOWS
			if (m_pTextureInfo->m_pSelfShadowFilename != NULL && m_texShadow == NULL)
				m_texShadow = LoadTexture((CFilename(pszSptFile).GetPath( ) + CFilename(m_pTextureInfo->m_pSelfShadowFilename).NoExtension( ) + ".dds").c_str( ));
#endif
			*/

			// setup the vertex and index buffers
			SetupBuffers();

			// set the position
			m_pSpeedTree->SetTreePosition(fX, fY, fZ);
			
            // everything appeared to go well
            bSuccess = true;
        }
        else // tree failed to compute			
        {
			_XFatalError( "Cannot compute tree [%s]", CSpeedTreeRT::GetCurrentError() );
        }
	}
	else // tree failed to load
    {
		_XFatalError( (LPTSTR)CSpeedTreeRT::GetCurrentError() );            
    }    
		
	SAFE_DELETE_ARRAY( pmodelarchive );
	
    return bSuccess;
}



///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::SetupBranchBuffers

void _XST_VSObject::SetupBranchBuffers(void)
{
	// reference to the branch structure
	CSpeedTreeRT::SGeometry::SIndexed* pBranches = &(m_pGeometryCache->m_sBranches);
	m_unBranchVertexCount = pBranches->m_usVertexCount; // we asked for a contiguous strip

	// check if this tree has branches
	if (m_unBranchVertexCount > 1)
	{
		// create the vertex buffer for storing branch vertices
		HRESULT ret = gpDev->CreateVertexBuffer(m_unBranchVertexCount * sizeof(SFVFBranchVertex_VS), D3DUSAGE_WRITEONLY, D3DFVF_SPEEDTREE_BRANCH_VERTEX_VS, g_3DObjectMemoryPool, &m_pBranchVertexBuffer, NULL);
		
		// fill the vertex buffer by interleaving SpeedTree data
		SFVFBranchVertex_VS* pVertexBuffer = NULL;
		ret = m_pBranchVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);
		for (unsigned int i = 0; i < m_unBranchVertexCount; ++i)
		{
			// position
			memcpy(&pVertexBuffer->m_vPosition, &(pBranches->m_pCoords[i * 3]), 3 * sizeof(float));

			// normal or color
			#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
				memcpy(&pVertexBuffer->m_vNormal, &(pBranches->m_pNormals[i * 3]), 3 * sizeof(float));
			#else			
				pVertexBuffer->m_dwDiffuseColor = AGBR2ARGB(pBranches->m_pColors[i]);
			#endif

			// texcoords for layer 0
			pVertexBuffer->m_fTexCoords[0] = pBranches->m_pTexCoords0[i * 2];
			pVertexBuffer->m_fTexCoords[1] = pBranches->m_pTexCoords0[i * 2 + 1];

			// texcoords for layer 1 (if enabled)
			#ifdef WRAPPER_RENDER_SELF_SHADOWS
				pVertexBuffer->m_fShadowCoords[0] = pBranches->m_pTexCoords1[i * 2];
				pVertexBuffer->m_fShadowCoords[1] = pBranches->m_pTexCoords1[i * 2 + 1];
			#endif

			// gpu wind data
			pVertexBuffer->m_fWindIndex = 4.0f * pBranches->m_pWindMatrixIndices[i];
			pVertexBuffer->m_fWindWeight = (float)pBranches->m_pWindWeights[i];
			
			++pVertexBuffer;
		}
		m_pBranchVertexBuffer->Unlock( );

		// create and fill the index counts for each LOD
		unsigned int unNumLodLevels = m_pSpeedTree->GetNumBranchLodLevels( );
		m_pBranchIndexCounts = new unsigned short[unNumLodLevels];
		for (i = 0; i < unNumLodLevels; ++i)
		{
			// force geometry update for this LOD
			m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BranchGeometry, (short)i);

			// check if this LOD has branches
			if (pBranches->m_usNumStrips > 0)
				m_pBranchIndexCounts[i] = pBranches->m_pStripLengths[0];
			else
				m_pBranchIndexCounts[i] = 0;
		}
		// force update of geometry to highest LOD
		m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BranchGeometry, 0);

		// the first LOD level contains the most indices of all the levels, so
		// we use its size to allocate the index buffer
		gpDev->CreateIndexBuffer(m_pBranchIndexCounts[0] * sizeof(unsigned short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, g_3DObjectMemoryPool, &m_pBranchIndexBuffer, NULL);

		// fill the index buffer
		unsigned short* pIndexBuffer = NULL;
		m_pBranchIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndexBuffer), 0);
		memcpy(pIndexBuffer, pBranches->m_pStrips[0], pBranches->m_pStripLengths[0] * sizeof(unsigned short));
		m_pBranchIndexBuffer->Unlock( );
	}
}

///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::SetupFrondBuffers

void _XST_VSObject::SetupFrondBuffers(void)
{
	// reference to frond structure	
	CSpeedTreeRT::SGeometry::SIndexed* pFronds = &(m_pGeometryCache->m_sFronds);
	m_unFrondVertexCount = pFronds->m_usVertexCount; // we asked for a contiguous strip

	// check if this tree has fronds
	if (m_unFrondVertexCount > 1)
	{
		// create the vertex buffer for storing frond vertices
		gpDev->CreateVertexBuffer(m_unFrondVertexCount * sizeof(SFVFBranchVertex_VS), D3DUSAGE_WRITEONLY, D3DFVF_SPEEDTREE_BRANCH_VERTEX_VS, g_3DObjectMemoryPool, &m_pFrondVertexBuffer, NULL);
		
		// fill the vertex buffer by interleaving SpeedTree data
		SFVFBranchVertex_VS* pVertexBuffer = NULL;
		m_pFrondVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);
		for (unsigned short i = 0; i < m_unFrondVertexCount; ++i)
		{
			// position
			memcpy(&pVertexBuffer->m_vPosition, &(pFronds->m_pCoords[i * 3]), 3 * sizeof(float));

			// normal or color
			#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
				memcpy(&pVertexBuffer->m_vNormal, &(pFronds->m_pNormals[i * 3]), 3 * sizeof(float));
			#else
				pVertexBuffer->m_dwDiffuseColor = AGBR2ARGB(pFronds->m_pColors[i]);
			#endif

			// texcoords for layer 0
			pVertexBuffer->m_fTexCoords[0] = pFronds->m_pTexCoords0[i * 2];
			pVertexBuffer->m_fTexCoords[1] = pFronds->m_pTexCoords0[i * 2 + 1];

			// texcoords for layer 1 (if enabled)
			#ifdef WRAPPER_RENDER_SELF_SHADOWS
				pVertexBuffer->m_fShadowCoords[0] = pFronds->m_pTexCoords1[i * 2];
				pVertexBuffer->m_fShadowCoords[1] = pFronds->m_pTexCoords1[i * 2 + 1];
			#endif
		
			// gpu wind data
			pVertexBuffer->m_fWindIndex = 4.0f * pFronds->m_pWindMatrixIndices[i];
			pVertexBuffer->m_fWindWeight = pFronds->m_pWindWeights[i];
			
			++pVertexBuffer;
		}
		m_pFrondVertexBuffer->Unlock( );

		// create and fill the index counts for each LOD
		unsigned int unNumLodLevels = m_pSpeedTree->GetNumFrondLodLevels( );
		m_pFrondIndexCounts = new unsigned short[unNumLodLevels];
		for (i = 0; i < unNumLodLevels; ++i)
		{
			// force update of geometry for this LOD
			m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_FrondGeometry, -1, i);

			// check if this LOD has fronds
			if (pFronds->m_usNumStrips > 0)
				m_pFrondIndexCounts[i] = pFronds->m_pStripLengths[0];
			else
				m_pFrondIndexCounts[i] = 0;
		}
		// force update of geometry to highest LOD
		m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_FrondGeometry, -1, 0);

		// the first LOD level contains the most indices of all the levels, so
		// we use its size to allocate the index buffer
		gpDev->CreateIndexBuffer(m_pFrondIndexCounts[0] * sizeof(unsigned short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, g_3DObjectMemoryPool, &m_pFrondIndexBuffer, NULL);

		// fill the index buffer
		unsigned short* pIndexBuffer = NULL;
		m_pFrondIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndexBuffer), 0);
		memcpy(pIndexBuffer, pFronds->m_pStrips[0], pFronds->m_pStripLengths[0] * sizeof(unsigned short));
		m_pFrondIndexBuffer->Unlock( );
	}
}


///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::SetupLeafBuffers

void _XST_VSObject::SetupLeafBuffers(void)
{
	// set up constants
	const short anVertexIndices[6] = { 0, 1, 2, 0, 2, 3 };

	// set up the leaf counts for each LOD
	m_usNumLeafLods = m_pSpeedTree->GetNumLeafLodLevels( );

	// create arrays for the number of LOD levels
	m_pLeafVertexBuffer = new LPDIRECT3DVERTEXBUFFER9[m_usNumLeafLods];
	memset( m_pLeafVertexBuffer, 0, sizeof(LPDIRECT3DVERTEXBUFFER9) * m_usNumLeafLods );
	m_pLeavesUpdatedByCpu = new bool[m_usNumLeafLods];

	for (unsigned int unLod = 0; unLod < m_usNumLeafLods; ++unLod)
	{
		m_pLeavesUpdatedByCpu[unLod] = false;

		// if this level has no leaves, then skip it
		unsigned short usLeafCount = m_pGeometryCache->m_sLeaves0.m_usLeafCount;
		if (usLeafCount < 1)
			continue;

		// create the vertex buffer for storing leaf vertices
		gpDev->CreateVertexBuffer(usLeafCount * 6 * sizeof(SFVFLeafVertex_VS), D3DUSAGE_WRITEONLY, D3DFVF_SPEEDTREE_LEAF_VERTEX_VS, g_3DObjectMemoryPool, &m_pLeafVertexBuffer[unLod], NULL);
			
		// fill the vertex buffer by interleaving SpeedTree data
		SFVFLeafVertex_VS* pVertexBuffer = NULL;
		m_pLeafVertexBuffer[unLod]->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);
		SFVFLeafVertex_VS* pVertex = pVertexBuffer;
		for (unsigned int unLeaf = 0; unLeaf < usLeafCount; ++unLeaf)
		{
			const CSpeedTreeRT::SGeometry::SLeaf* pLeaf = &(m_pGeometryCache->m_sLeaves0);
			for (unsigned int unVert = 0; unVert < 6; ++unVert)  // 6 verts == 2 triangles
			{
				// position
				memcpy(pVertex->m_vPosition, &(pLeaf->m_pCenterCoords[unLeaf * 3]), 3 * sizeof(float));

				#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
					// normal
					memcpy(&pVertex->m_vNormal, &(pLeaf->m_pNormals[unLeaf * 3]), 3 * sizeof(float));
				#else
					// color
					pVertex->m_dwDiffuseColor = AGBR2ARGB(pLeaf->m_pColors[unLeaf]);
				#endif

				// tex coord
				memcpy(pVertex->m_fTexCoords, &(pLeaf->m_pLeafMapTexCoords[unLeaf][anVertexIndices[unVert] * 2]), 2 * sizeof(float));

				// wind weights
				pVertex->m_fWindIndex = 4.0f * pLeaf->m_pWindMatrixIndices[unLeaf];
				pVertex->m_fWindWeight = pLeaf->m_pWindWeights[unLeaf];
				
				// gpu leaf placement data
				pVertex->m_fLeafPlacementIndex = c_nVertexShader_LeafTables + pLeaf->m_pLeafClusterIndices[unLeaf] * 4.0f + anVertexIndices[unVert];
				pVertex->m_fLeafScalarValue = m_pSpeedTree->GetLeafLodSizeAdjustments( )[unLod];
				
				++pVertex;
			}
			
		}
		m_pLeafVertexBuffer[unLod]->Unlock( );
	}
}

///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::Advance

void _XST_VSObject::Advance(void)
{
	// compute LOD level (based on distance from camera)
	m_pSpeedTree->ComputeLodLevel( );
}




///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::SetupBranchForTreeType

void _XST_VSObject::SetupBranchForTreeType(void)
{
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
	// set lighting material
	m_cBranchMaterial.Activate();
	SetShaderConstants(m_pSpeedTree->GetBranchMaterial( ));
#endif
	
    // set texture map
	g_LastUsedRenderStyle = NULL;
	g_SpeedTreeTextureArchive.SetTexture( m_BranchTextureIndex );

	// bind shadow texture (if enabled)
#ifdef WRAPPER_RENDER_SELF_SHADOWS
	if (m_texShadow)
		gpDev->SetTexture(1, m_texShadow);
#endif
	
	// if tree has branches, bind the buffers
	if (m_pGeometryCache->m_sBranches.m_usVertexCount > 0)
	{
		// activate the branch vertex buffer
		gpDev->SetStreamSource(0, m_pBranchVertexBuffer, 0, sizeof(SFVFBranchVertex_VS));
		// set the index buffer
		gpDev->SetIndices(m_pBranchIndexBuffer);
	}
}


//////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::SetupFrondForTreeType

void _XST_VSObject::SetupFrondForTreeType(void)
{
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
	// set lighting material
	m_cFrondMaterial.Activate();
	SetShaderConstants(m_pSpeedTree->GetFrondMaterial( ));
#endif

	// bind shadow texture (if enabled)
#ifdef WRAPPER_RENDER_SELF_SHADOWS
	if (m_texShadow)
		gpDev->SetTexture(1, m_texShadow);
#endif

	// if tree has fronds, bind the buffers
	if (m_pGeometryCache->m_sFronds.m_usVertexCount > 0)
	{
		// activate the frond vertex buffer
		gpDev->SetStreamSource(0, m_pFrondVertexBuffer, 0, sizeof(SFVFBranchVertex_VS));
		// set the index buffer
		gpDev->SetIndices(m_pFrondIndexBuffer);
	}
}


///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::SetupLeafForTreeType

void _XST_VSObject::SetupLeafForTreeType(void)
{
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
	// set lighting material
	m_cLeafMaterial.Activate();
	SetShaderConstants(m_pSpeedTree->GetLeafMaterial( ));
#endif
	
	// send leaf tables to the gpu
	UploadLeafTables(c_nVertexShader_LeafTables);
	
	// bind shadow texture (if enabled)
#ifdef WRAPPER_RENDER_SELF_SHADOWS
	if (m_texShadow)
		gpDev->SetTexture(1, 0);
#endif
}


///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject::RenderLeaves

void _XST_VSObject::RenderLeaves(void)
{
	m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_LeafGeometry, -1, -1, 0);

	PositionTree( );
	
	// might need to draw 2 LOD's
	for (unsigned int unLeafLevel = 0; unLeafLevel < 2; ++unLeafLevel)
	{
		const CSpeedTreeRT::SGeometry::SLeaf* pLeaf = (unLeafLevel == 0) ?
			&m_pGeometryCache->m_sLeaves0 : pLeaf = &m_pGeometryCache->m_sLeaves1;
		
		int unLod = pLeaf->m_nDiscreteLodLevel;
		
		// if this LOD is active and has leaves, draw it
		if (unLod > -1 && pLeaf->m_bIsActive && pLeaf->m_usLeafCount > 0)
		{
			gpDev->SetStreamSource(0, m_pLeafVertexBuffer[unLod], 0, sizeof(SFVFLeafVertex_VS));
			gpDev->SetRenderState(D3DRS_ALPHAREF, DWORD(pLeaf->m_fAlphaTestValue));
			gpDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, pLeaf->m_usLeafCount * 2);
		}
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	_XST_NSObject
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::LoadTree
//
//  Returns true upon success, false in failure
bool _XST_NSObject::LoadTree(const char* pszSptFile, _XPackageArchive* ModelArchive,
									LPTSTR	pszCompositionMapname, int& compositionmapindex,
									unsigned int nSeed, float fSize, float fSizeVariance, 
									float fX, float fY, float fZ)
{
	if( !ModelArchive ) return false;

    bool bSuccess = false;

	int modelindex = ModelArchive->FindResource( (LPTSTR)pszSptFile );

	if( modelindex == -1 )
	{
		_XFatalError( "Cannot find tree archive[%s]", pszSptFile );
		return false;
	}

	LPBYTE pmodelarchive = NULL;
	LONG   archivesize = 0;
	
	pmodelarchive = ModelArchive->LoadPackedFileToMemory( (DWORD)modelindex, &archivesize );
	if( !pmodelarchive )
	{
		return false;
	}

	// directx, so allow for flipping of the texture coordinate
	m_pSpeedTree->SetTextureFlip(true);
        
	// Load the tree file (true == success)		

	if (m_pSpeedTree->LoadTree((const unsigned char*)pmodelarchive,(unsigned int)archivesize))
	{
		// override the lighting method stored in the spt file
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
		m_pSpeedTree->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		m_pSpeedTree->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		m_pSpeedTree->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
#else
		m_pSpeedTree->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_STATIC);
		m_pSpeedTree->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_STATIC);
		m_pSpeedTree->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_STATIC);
#endif
		
		// set the wind method
		m_pSpeedTree->SetBranchWindMethod(CSpeedTreeRT::WIND_NONE);
		m_pSpeedTree->SetLeafWindMethod(CSpeedTreeRT::WIND_NONE);
		m_pSpeedTree->SetFrondWindMethod(CSpeedTreeRT::WIND_NONE);

		m_pSpeedTree->SetNumLeafRockingGroups(1);

		// override the size, if necessary
        if (fSize >= 0.0f && fSizeVariance >= 0.0f)
            m_pSpeedTree->SetTreeSize(fSize, fSizeVariance);
        				
		// generate tree geometry
        if (m_pSpeedTree->Compute(NULL, nSeed))
        {
			// get the dimensions & adjust LOD values
			m_pSpeedTree->GetBoundingBox(m_afBoundingBox);

			// make the leaves rock in the wind
			m_pSpeedTree->SetLeafRockingState(true);

			// billboard setup
#ifdef WRAPPER_BILLBOARD_MODE
			CSpeedTreeRT::SetDropToBillboard(true);
#else
			CSpeedTreeRT::SetDropToBillboard(false);
#endif

			// query & set materials
			m_cBranchMaterial.Set(m_pSpeedTree->GetBranchMaterial( ));
			m_cFrondMaterial.Set(m_pSpeedTree->GetFrondMaterial( ));
			m_cLeafMaterial.Set(m_pSpeedTree->GetLeafMaterial( ));

			// adjust lod distances
			FLOAT _fNearLodFactor = 50;
			FLOAT _fFarLodFactor = 150;
            float fHeight = (m_afBoundingBox[5] - m_afBoundingBox[2]);
            m_pSpeedTree->SetLodLimits( (fHeight * _fNearLodFactor)/ fSize, (fHeight * _fFarLodFactor) / fSize );

			// query textures
			m_pTextureInfo = new CSpeedTreeRT::STextures;
			m_pSpeedTree->GetTextures(*m_pTextureInfo);

			// load branch texture
			compositionmapindex  = m_LeafTextureIndex = g_SpeedTreeTextureArchive.GetResourceIndex( pszCompositionMapname, 3 );
			m_BranchTextureIndex = g_SpeedTreeTextureArchive.GetResourceIndex( (LPTSTR)m_pTextureInfo->m_pBranchTextureFilename, 3 );

			/*
#ifdef WRAPPER_RENDER_SELF_SHADOWS
			if (m_pTextureInfo->m_pSelfShadowFilename != NULL && m_texShadow == NULL)
				m_texShadow = LoadTexture((CFilename(pszSptFile).GetPath( ) + CFilename(m_pTextureInfo->m_pSelfShadowFilename).NoExtension( ) + ".dds").c_str( ));
#endif
			*/

			// setup the vertex and index buffers
			SetupBuffers();

			// set the position
			m_pSpeedTree->SetTreePosition(fX, fY, fZ);
			
            // everything appeared to go well
            bSuccess = true;
        }
        else // tree failed to compute			
        {
			_XFatalError( "Cannot compute tree [%s]", CSpeedTreeRT::GetCurrentError() );
        }
	}
	else // tree failed to load
    {
		_XFatalError( (LPTSTR)CSpeedTreeRT::GetCurrentError() );            
    }    
		
	SAFE_DELETE_ARRAY( pmodelarchive );
	
    return bSuccess;
}


///////////////////////////////////////////////////////////////////////////////////////
//	_XST_NSObject::SetupBranchBuffers

void _XST_NSObject::SetupBranchBuffers(void)
{
	// reference to the branch structure
	CSpeedTreeRT::SGeometry::SIndexed* pBranches = &(m_pGeometryCache->m_sBranches);
	m_unBranchVertexCount = pBranches->m_usVertexCount; // we asked for a contiguous strip

	// check if this tree has branches
	if (m_unBranchVertexCount > 1)
	{
		// create the vertex buffer for storing branch vertices
		HRESULT ret = gpDev->CreateVertexBuffer(m_unBranchVertexCount * sizeof(SFVFBranchVertex_NS), D3DUSAGE_WRITEONLY, D3DFVF_SPEEDTREE_BRANCH_VERTEX_NS, g_3DObjectMemoryPool, &m_pBranchVertexBuffer, NULL);

		if(FAILED(ret))
		{
			_XFatalError( "Can't create the branch vertex buffer" );
			return;
		}
		
		// fill the vertex buffer by interleaving SpeedTree data
		SFVFBranchVertex_NS* pVertexBuffer = NULL;
		ret = m_pBranchVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);
		for (unsigned int i = 0; i < m_unBranchVertexCount; ++i)
		{
			// position
			memcpy(&pVertexBuffer->m_vPosition, &(pBranches->m_pCoords[i * 3]), 3 * sizeof(float));

			// normal or color
			#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
				memcpy(&pVertexBuffer->m_vNormal, &(pBranches->m_pNormals[i * 3]), 3 * sizeof(float));
			#else			
				pVertexBuffer->m_dwDiffuseColor = AGBR2ARGB(pBranches->m_pColors[i]);
			#endif

			// texcoords for layer 0
			pVertexBuffer->m_fTexCoords[0] = pBranches->m_pTexCoords0[i * 2];
			pVertexBuffer->m_fTexCoords[1] = pBranches->m_pTexCoords0[i * 2 + 1];

			// texcoords for layer 1 (if enabled)
			#ifdef WRAPPER_RENDER_SELF_SHADOWS
				pVertexBuffer->m_fShadowCoords[0] = pBranches->m_pTexCoords1[i * 2];
				pVertexBuffer->m_fShadowCoords[1] = pBranches->m_pTexCoords1[i * 2 + 1];
			#endif
			
			++pVertexBuffer;
		}
		m_pBranchVertexBuffer->Unlock( );

		// create and fill the index counts for each LOD
		unsigned int unNumLodLevels = m_pSpeedTree->GetNumBranchLodLevels( );
		m_pBranchIndexCounts = new unsigned short[unNumLodLevels];
		for (i = 0; i < unNumLodLevels; ++i)
		{
			// force geometry update for this LOD
			m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BranchGeometry, (short)i);

			// check if this LOD has branches
			if (pBranches->m_usNumStrips > 0)
				m_pBranchIndexCounts[i] = pBranches->m_pStripLengths[0];
			else
				m_pBranchIndexCounts[i] = 0;
		}
		// force update of geometry to highest LOD
		m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BranchGeometry, 0);

		// the first LOD level contains the most indices of all the levels, so
		// we use its size to allocate the index buffer
		ret = gpDev->CreateIndexBuffer(m_pBranchIndexCounts[0] * sizeof(unsigned short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, g_3DObjectMemoryPool, &m_pBranchIndexBuffer, NULL);
		if(FAILED(ret))
		{
			_XFatalError( "Can't create the branch index buffer" );
			return;
		}

		// fill the index buffer
		unsigned short* pIndexBuffer = NULL;
		ret = m_pBranchIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndexBuffer), 0);
		if(FAILED(ret))
		{
			_XFatalError( "Can't lock the branch index buffer" );
			return;
		}

		memcpy(pIndexBuffer, pBranches->m_pStrips[0], pBranches->m_pStripLengths[0] * sizeof(unsigned short));
		m_pBranchIndexBuffer->Unlock( );
	}
}

///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::SetupFrondBuffers

void _XST_NSObject::SetupFrondBuffers(void)
{
	// reference to frond structure	
	CSpeedTreeRT::SGeometry::SIndexed* pFronds = &(m_pGeometryCache->m_sFronds);
	m_unFrondVertexCount = pFronds->m_usVertexCount; // we asked for a contiguous strip

	// check if this tree has fronds
	if (m_unFrondVertexCount > 1)
	{
		// create the vertex buffer for storing frond vertices		
		HRESULT ret = gpDev->CreateVertexBuffer(m_unFrondVertexCount * sizeof(SFVFBranchVertex_NS), D3DUSAGE_WRITEONLY, D3DFVF_SPEEDTREE_BRANCH_VERTEX_NS, g_3DObjectMemoryPool, &m_pFrondVertexBuffer, NULL);
		if(FAILED(ret))
		{
			_XFatalError( "Can't create the frond vertex buffer" );
			return;
		}
				
		// fill the vertex buffer by interleaving SpeedTree data
		SFVFBranchVertex_NS* pVertexBuffer = NULL;
		ret = m_pFrondVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);
		if(FAILED(ret))
		{
			_XFatalError( "Can't lock the frond vertex buffer" );
			return;
		}

		for (unsigned short i = 0; i < m_unFrondVertexCount; ++i)
		{
			// position
			memcpy(&pVertexBuffer->m_vPosition, &(pFronds->m_pCoords[i * 3]), 3 * sizeof(float));

			// normal or color
			#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
				memcpy(&pVertexBuffer->m_vNormal, &(pFronds->m_pNormals[i * 3]), 3 * sizeof(float));
			#else
				pVertexBuffer->m_dwDiffuseColor = AGBR2ARGB(pFronds->m_pColors[i]);
			#endif

			// texcoords for layer 0
			pVertexBuffer->m_fTexCoords[0] = pFronds->m_pTexCoords0[i * 2];
			pVertexBuffer->m_fTexCoords[1] = pFronds->m_pTexCoords0[i * 2 + 1];

			// texcoords for layer 1 (if enabled)
			#ifdef WRAPPER_RENDER_SELF_SHADOWS
				pVertexBuffer->m_fShadowCoords[0] = pFronds->m_pTexCoords1[i * 2];
				pVertexBuffer->m_fShadowCoords[1] = pFronds->m_pTexCoords1[i * 2 + 1];
			#endif
					
			++pVertexBuffer;
		}
		m_pFrondVertexBuffer->Unlock( );

		// create and fill the index counts for each LOD
		unsigned int unNumLodLevels = m_pSpeedTree->GetNumFrondLodLevels( );
		m_pFrondIndexCounts = new unsigned short[unNumLodLevels];
		for (i = 0; i < unNumLodLevels; ++i)
		{
			// force update of geometry for this LOD
			m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_FrondGeometry, -1, i);

			// check if this LOD has fronds
			if (pFronds->m_usNumStrips > 0)
				m_pFrondIndexCounts[i] = pFronds->m_pStripLengths[0];
			else
				m_pFrondIndexCounts[i] = 0;
		}
		// force update of geometry to highest LOD
		m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_FrondGeometry, -1, 0);

		// the first LOD level contains the most indices of all the levels, so
		// we use its size to allocate the index buffer
		ret = gpDev->CreateIndexBuffer(m_pFrondIndexCounts[0] * sizeof(unsigned short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, g_3DObjectMemoryPool, &m_pFrondIndexBuffer, NULL);
		if(FAILED(ret))
		{
			_XFatalError( "Can't Create the frond index buffer" );
			return;
		}

		// fill the index buffer
		unsigned short* pIndexBuffer = NULL;
		ret = m_pFrondIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndexBuffer), 0);
		if(FAILED(ret))
		{
			_XFatalError( "Can't lock the frond index buffer" );
			return;
		}

		memcpy(pIndexBuffer, pFronds->m_pStrips[0], pFronds->m_pStripLengths[0] * sizeof(unsigned short));
		m_pFrondIndexBuffer->Unlock( );
	}
}


///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::SetupLeafBuffers

void _XST_NSObject::SetupLeafBuffers(void)
{
	// set up constants
	const short anVertexIndices[6] = { 0, 1, 2, 0, 2, 3 };

	// set up the leaf counts for each LOD
	m_usNumLeafLods = m_pSpeedTree->GetNumLeafLodLevels( );

	// create arrays for the number of LOD levels
	m_pLeafVertexBuffer = new LPDIRECT3DVERTEXBUFFER9[m_usNumLeafLods];	
	memset( m_pLeafVertexBuffer, 0, sizeof(LPDIRECT3DVERTEXBUFFER9) * m_usNumLeafLods );

	m_pLeavesUpdatedByCpu = new bool[m_usNumLeafLods];

	for (unsigned int unLod = 0; unLod < m_usNumLeafLods; ++unLod)
	{
		m_pLeavesUpdatedByCpu[unLod] = false;

		// if this level has no leaves, then skip it
		unsigned short usLeafCount = m_pGeometryCache->m_sLeaves0.m_usLeafCount;
		if (usLeafCount < 1)
			continue;

		// create the vertex buffer for storing leaf vertices		
		HRESULT ret = gpDev->CreateVertexBuffer(usLeafCount * 6 * sizeof(SFVFLeafVertex_NS), D3DUSAGE_WRITEONLY, D3DFVF_SPEEDTREE_LEAF_VERTEX_NS, g_3DObjectMemoryPool, &m_pLeafVertexBuffer[unLod], NULL);		
		
		if(FAILED(ret))
		{
			_XFatalError( "Can't create the leaf vertex buffer" );
			return;
		}
	
		// fill the vertex buffer by interleaving SpeedTree data
		SFVFLeafVertex_NS* pVertexBuffer = NULL;
		ret = m_pLeafVertexBuffer[unLod]->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);

		if(FAILED(ret))
		{
			_XFatalError( "Can't lock the leaf vertex buffer" );
			return;
		}

		SFVFLeafVertex_NS* pVertex = pVertexBuffer;
		for (unsigned int unLeaf = 0; unLeaf < usLeafCount; ++unLeaf)
		{
			const CSpeedTreeRT::SGeometry::SLeaf* pLeaf = &(m_pGeometryCache->m_sLeaves0);
			for (unsigned int unVert = 0; unVert < 6; ++unVert)  // 6 verts == 2 triangles
			{
				// position
				memcpy(pVertex->m_vPosition, &(pLeaf->m_pCenterCoords[unLeaf * 3]), 3 * sizeof(float));

				#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
					// normal
					memcpy(&pVertex->m_vNormal, &(pLeaf->m_pNormals[unLeaf * 3]), 3 * sizeof(float));
				#else
					// color
					pVertex->m_dwDiffuseColor = AGBR2ARGB(pLeaf->m_pColors[unLeaf]);
				#endif

				// tex coord
				memcpy(pVertex->m_fTexCoords, &(pLeaf->m_pLeafMapTexCoords[unLeaf][anVertexIndices[unVert] * 2]), 2 * sizeof(float));

				++pVertex;
			}
			
		}
		m_pLeafVertexBuffer[unLod]->Unlock( );
	}
}

///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::Advance

void _XST_NSObject::Advance(void)
{
	// compute LOD level (based on distance from camera)
	m_pSpeedTree->ComputeLodLevel( );
	
	// compute wind
	m_pSpeedTree->ComputeWindEffects(true, true, true);
}




///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::SetupBranchForTreeType

void _XST_NSObject::SetupBranchForTreeType(void)
{
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
	// set lighting material
	m_cBranchMaterial.Activate();
	//SetShaderConstants(m_pSpeedTree->GetBranchMaterial( ));
#endif
	
	// update the branch geometry for CPU wind
	m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BranchGeometry);
	if (m_pGeometryCache->m_sBranches.m_usNumStrips > 0)
	{
		// update the vertex array
		SFVFBranchVertex_NS* pVertexBuffer = NULL;
		m_pBranchVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);
		for (unsigned int i = 0; i < m_unBranchVertexCount; ++i)
		{
			memcpy(&(pVertexBuffer[i].m_vPosition), &(m_pGeometryCache->m_sBranches.m_pCoords[i * 3]), 3 * sizeof(float));
		}
		m_pBranchVertexBuffer->Unlock( );
	}
	
    // set texture map
	g_LastUsedRenderStyle = NULL;
	g_SpeedTreeTextureArchive.SetTexture( m_BranchTextureIndex );
	
	// bind shadow texture (if enabled)
#ifdef WRAPPER_RENDER_SELF_SHADOWS
	if (m_texShadow)
		gpDev->SetTexture(1, m_texShadow);
#endif
	
	// if tree has branches, bind the buffers
	if (m_pGeometryCache->m_sBranches.m_usVertexCount > 0)
	{
		// activate the branch vertex buffer
		gpDev->SetStreamSource(0, m_pBranchVertexBuffer, 0, sizeof(SFVFBranchVertex_NS));
		// set the index buffer
		gpDev->SetIndices(m_pBranchIndexBuffer);
	}
}


//////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::SetupFrondForTreeType

void _XST_NSObject::SetupFrondForTreeType(void)
{
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
	// set lighting material
	m_cFrondMaterial.Activate();
	//SetShaderConstants(m_pSpeedTree->GetFrondMaterial( ));
#endif
	
	// update the frond geometry for CPU wind
	m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_FrondGeometry);
	if (m_pGeometryCache->m_sFronds.m_usNumStrips > 0)
	{
		// update the vertex array
		SFVFBranchVertex_NS* pVertexBuffer = NULL;
		m_pFrondVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertexBuffer), 0);
		for (unsigned int i = 0; i < m_unFrondVertexCount; ++i)
		{
			memcpy(&(pVertexBuffer[i].m_vPosition), &(m_pGeometryCache->m_sFronds.m_pCoords[i * 3]), 3 * sizeof(float));
		}
		m_pFrondVertexBuffer->Unlock( );
	}
	
	// bind shadow texture (if enabled)
#ifdef WRAPPER_RENDER_SELF_SHADOWS
	if (m_texShadow)
		gpDev->SetTexture(1, m_texShadow);
#endif
	
	// if tree has fronds, bind the buffers
	if (m_pGeometryCache->m_sFronds.m_usVertexCount > 0)
	{
		// activate the frond vertex buffer
		gpDev->SetStreamSource(0, m_pFrondVertexBuffer, 0, sizeof(SFVFBranchVertex_NS));
		// set the index buffer
		gpDev->SetIndices(m_pFrondIndexBuffer);
	}
}


///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::SetupLeafForTreeType

void _XST_NSObject::SetupLeafForTreeType(void)
{
#ifdef WRAPPER_USE_DYNAMIC_LIGHTING
	// set lighting material
	m_cLeafMaterial.Activate();
	//SetShaderConstants(m_pSpeedTree->GetLeafMaterial( ));
#endif

	// bind shadow texture (if enabled)
#ifdef WRAPPER_RENDER_SELF_SHADOWS
	if (m_texShadow)
		gpDev->SetTexture(1, 0);
#endif
}


///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject::RenderLeaves

void _XST_NSObject::RenderLeaves(void)
{
	m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_LeafGeometry);
	
	// update the LOD level vertex arrays we need
	// might need to draw 2 LOD's
		
	for (unsigned int i = 0; i < 2; ++i)
	{
		// reference to leaves structure
		const CSpeedTreeRT::SGeometry::SLeaf* pLeaf = (i == 0) ? &m_pGeometryCache->m_sLeaves0 : &m_pGeometryCache->m_sLeaves1;
		
		if (pLeaf->m_bIsActive && pLeaf->m_usLeafCount >= 1 )
		{
			int unLod = pLeaf->m_nDiscreteLodLevel;

			// update the vertices
			SFVFLeafVertex_NS* pVertex = NULL;
			m_pLeafVertexBuffer[unLod]->Lock(0, 0, reinterpret_cast<void**>(&pVertex),0);
			for (unsigned int unLeaf = 0; unLeaf < pLeaf->m_usLeafCount; ++unLeaf)
			{
				D3DXVECTOR3 vecCenter(&(pLeaf->m_pCenterCoords[unLeaf * 3]));
				D3DXVECTOR3 vec0(&pLeaf->m_pLeafMapCoords[unLeaf][0]);
				D3DXVECTOR3 vec1(&pLeaf->m_pLeafMapCoords[unLeaf][4]);
				D3DXVECTOR3 vec2(&pLeaf->m_pLeafMapCoords[unLeaf][8]);
				D3DXVECTOR3 vec3(&pLeaf->m_pLeafMapCoords[unLeaf][12]);
				
				(pVertex++)->m_vPosition = vecCenter + vec0;		// vertex 0
				(pVertex++)->m_vPosition = vecCenter + vec1;		// vertex 1
				(pVertex++)->m_vPosition = vecCenter + vec2;		// vertex 2
				(pVertex++)->m_vPosition = vecCenter + vec0;		// vertex 0
				(pVertex++)->m_vPosition = vecCenter + vec2;		// vertex 2
				(pVertex++)->m_vPosition = vecCenter + vec3;		// vertex 3
			}
			m_pLeafVertexBuffer[unLod]->Unlock( );
		}
	}
	
	PositionTree( );
	
	// might need to draw 2 LOD's
	for (unsigned int unLeafLevel = 0; unLeafLevel < 2; ++unLeafLevel)
	{
		const CSpeedTreeRT::SGeometry::SLeaf* pLeaf = (unLeafLevel == 0) ?
			&m_pGeometryCache->m_sLeaves0 : pLeaf = &m_pGeometryCache->m_sLeaves1;
		
		int unLod = pLeaf->m_nDiscreteLodLevel;
		
		// if this LOD is active and has leaves, draw it
		if (unLod > -1 && pLeaf->m_bIsActive && pLeaf->m_usLeafCount > 0)
		{
			gpDev->SetStreamSource(0, m_pLeafVertexBuffer[unLod], 0, sizeof(SFVFLeafVertex_NS));			
			gpDev->SetRenderState(D3DRS_ALPHAREF, DWORD(pLeaf->m_fAlphaTestValue));
			gpDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, pLeaf->m_usLeafCount * 2);
		}
	}
}


