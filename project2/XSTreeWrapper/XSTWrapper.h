///////////////////////////////////////////////////////////////////////  
//	SpeedTreeRTExample Class
//
//	(c) 2003 IDV, Inc.
//
//	This class is provided to illustrate one way to incorporate
//	SpeedTreeRT into a DirectX application.  All of the SpeedTreeRT
//	calls that must be made on a per tree basis are done by this class.
//	Calls that apply to all trees (i.e. static SpeedTreeRT functions)
//	are made in the main CD3DApplication derived class.
//
//
//	*** INTERACTIVE DATA VISUALIZATION (IDV) PROPRIETARY INFORMATION ***
//
//	This software is supplied under the terms of a license agreement or
//	nondisclosure agreement with Interactive Data Visualization and may
//	not be copied or disclosed except in accordance with the terms of
//	that agreement.
//
//      Copyright (c) 2001-2003 IDV, Inc.
//      All Rights Reserved.
//
//		IDV, Inc.
//		1233 Washington St. Suite 610
//		Columbia, SC 29201
//		Voice: (803) 799-1699
//		Fax:   (803) 931-0320
//		Web:   http://www.idvinc.com
//

#pragma once

///////////////////////////////////////////////////////////////////////  
//	Include Files
#include "XSTWrapperConfig.h"
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////  
//	_XST_AbstractObject

#define _XDEF_PUREFUNCTION						0
#define _XDEF_MAX_SPEEDTREERENDERSTACK			500

extern void _XMessage ( TCHAR* argv, ... );

#define _DUMMY_MESSAGE							_XMessage( "Warning : invalid overiding" );

class _XPackageArchive;

class _XST_AbstractObject
{
public:
		// construction/destruction
		_XST_AbstractObject();
	    virtual ~_XST_AbstractObject();

	    // initialization routines
	    virtual bool                        LoadTree(const char* pszSptFile, _XPackageArchive* ModelArchive,
													 LPTSTR	pszCompositionMapname, int& compositionmapindex,
													 unsigned int nSeed = 1, float fSize = -1.0f, float fSizeVariance = -1.0f, 
													 float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f){ return false; };
		// rendering
		virtual void						SetupBranchForTreeType(void){ _DUMMY_MESSAGE }
		virtual void						SetupFrondForTreeType(void){ _DUMMY_MESSAGE }
		virtual void						SetupLeafForTreeType(void){ _DUMMY_MESSAGE }
				
		virtual void						RenderBranches(void);
		virtual void						RenderFronds(void);
		virtual void						EndLeafForTreeType(void);
		virtual void						UploadLeafTables(unsigned int uiLocation);

		virtual void						RenderLeaves(void){ _DUMMY_MESSAGE }
		virtual void						RenderBillboards(void);

		// instancing
		virtual _XST_AbstractObject**		GetInstances(unsigned int& nCount);
		virtual _XST_AbstractObject*		InstanceOf(void)								{ return m_pInstanceOf; }
		virtual _XST_AbstractObject*		MakeInstance(BOOL useshader,float fX, float fY, float fZ);
		virtual CSpeedTreeRT*				GetSpeedTree(void)								{ return m_pSpeedTree; }

		virtual void						SetPos(const float* pPos);

		// lighting																	
		virtual const CSpeedTreeMaterial&	GetBranchMaterial(void)							{ return m_cBranchMaterial; }
		virtual const CSpeedTreeMaterial&	GetFrondMaterial(void)							{ return m_cFrondMaterial; }
		virtual const CSpeedTreeMaterial&	GetLeafMaterial(void)							{ return m_cLeafMaterial; }
		virtual float						GetLeafLightingAdjustment(void)					{ return m_pSpeedTree->GetLeafLightingAdjustment( ); }

		// wind																		
		virtual void						SetWindStrength(float fStrength)				{ (void) m_pSpeedTree->SetWindStrength(fStrength); }
		virtual	void						Advance(void) { _DUMMY_MESSAGE }
		
		// utility
		virtual void						CleanUpMemory(void);

protected:
		virtual void						SetupBuffers(void); 
		virtual	void						SetupBranchBuffers(void) { _DUMMY_MESSAGE }
		virtual	void						SetupFrondBuffers(void) { _DUMMY_MESSAGE }
		virtual	void						SetupLeafBuffers(void) { _DUMMY_MESSAGE }
		virtual void						PositionTree(void);
		virtual void						SetShaderConstants(const float* pMaterial);

public:			
		// Rendering stack
		_XST_AbstractObject**				m_STObjectRenderStack;
		int									m_STObjectRenderStackSize;

		// Texture index
		int									m_LeafTextureIndex;
	    int									m_BranchTextureIndex;

		virtual BOOL						ReserveRenderStack( void );
		virtual void						ResetRenderStack( void );
		virtual void						InsertRenderingInstance( _XST_AbstractObject* pinstance );

		// tree properties		
		float								m_afPos[3];						// tree position
		float								m_afBoundingBox[6];				// tree's bounding box
		

protected:
		// SpeedTreeRT data
		CSpeedTreeRT*						m_pSpeedTree;					// SpeedTree object
		CSpeedTreeRT::STextures*			m_pTextureInfo;					// cached texture info
		bool								m_bIsInstance;					// is this tree an instance?
		std::vector<_XST_AbstractObject*>	m_vInstances;					// list of instances of this tree
		_XST_AbstractObject*				m_pInstanceOf;					// this tree is an instance of what
	
		// geometry cache
		CSpeedTreeRT::SGeometry*			m_pGeometryCache;				// geometry cache
	
		// branch buffers
		LPDIRECT3DVERTEXBUFFER9				m_pBranchVertexBuffer;			// branch vertex buffer
		unsigned int						m_unBranchVertexCount;			// number of vertices in branches
		LPDIRECT3DINDEXBUFFER9				m_pBranchIndexBuffer;			// branch index buffer
		unsigned short*						m_pBranchIndexCounts;			// number of indexes per branch LOD level
	
		// frond buffers
		LPDIRECT3DVERTEXBUFFER9				m_pFrondVertexBuffer;			// frond vertex buffer
		unsigned int						m_unFrondVertexCount;			// number of vertices in frond
		LPDIRECT3DINDEXBUFFER9				m_pFrondIndexBuffer;			// frond index buffer
		unsigned short*						m_pFrondIndexCounts;			// number of indexes per frond LOD level
	
		// leaf buffers
		unsigned short						m_usNumLeafLods;				// the number of leaf LODs
		LPDIRECT3DVERTEXBUFFER9*			m_pLeafVertexBuffer;			// leaf vertex buffer
		bool*								m_pLeavesUpdatedByCpu;			// stores which LOD's have been updated already per frame
	
		// materials
		CSpeedTreeMaterial					m_cBranchMaterial;				// branch material
		CSpeedTreeMaterial					m_cLeafMaterial;				// leaf material
		CSpeedTreeMaterial					m_cFrondMaterial;				// frond material
	
		// branch texture
		LPDIRECT3DTEXTURE9					m_texBranchTexture;				// branch texture object
		static	LPDIRECT3DTEXTURE9			m_texShadow;					// shadow texture object			
};

///////////////////////////////////////////////////////////////////////  
//	_XST_VSObject : Vertex shader format

class _XST_VSObject : public _XST_AbstractObject
{
public:
		// construction/destruction
		//_XST_VSObject(){};
	    //~_XST_VSObject(){};

	    // initialization routines
	    bool                                LoadTree(const char* pszSptFile, _XPackageArchive* ModelArchive,
													 LPTSTR	pszCompositionMapname, int& compositionmapindex,
													 unsigned int nSeed = 1, float fSize = -1.0f, float fSizeVariance = -1.0f, 
													 float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);

		// rendering
		void								SetupBranchForTreeType(void);
		void								SetupFrondForTreeType(void);
		void								SetupLeafForTreeType(void);
				
		void								RenderLeaves(void);
		void								Advance(void);		
		
protected:
		void								SetupBranchBuffers(void);
		void								SetupFrondBuffers(void);
		void								SetupLeafBuffers(void);
};





///////////////////////////////////////////////////////////////////////  
//	_XST_NSObject : Vertex shader format

class _XST_NSObject : public _XST_AbstractObject
{
public:
	// construction/destruction
	//_XST_NSObject(){};
	//~_XST_NSObject(){};	

	// initialization routines
	bool                                LoadTree(const char* pszSptFile, _XPackageArchive* ModelArchive,
												 LPTSTR	pszCompositionMapname, int& compositionmapindex,
												 unsigned int nSeed = 1, float fSize = -1.0f, float fSizeVariance = -1.0f, 
												 float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);
	
	// rendering
	void								SetupBranchForTreeType(void);
	void								SetupFrondForTreeType(void);
	void								SetupLeafForTreeType(void);
				
	void								RenderLeaves(void);
	void								Advance(void);		
	
protected:
	void								SetupBranchBuffers(void);
	void								SetupFrondBuffers(void);
	void								SetupLeafBuffers(void);
};


