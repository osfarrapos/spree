// XMeshContainer.h: interface for the _XMeshContainer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMESHCONTAINER_H_
#define _XMESHCONTAINER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XBipedController.h"

class _XBipedController;
class _XModelDescriptor;
class _XST_AbstractObject;
class _XOctreeObject;

#include <string>

class _XMeshContainer  
{

public:		
	_LPXM_MESHOBJECT	m_Node;
	int					m_NodeCnt;	
			
	_XTextureManager*   m_pTextureManager;

	D3DXVECTOR3			OBB_Center;
    D3DXVECTOR3			OBB_Axis[3];
	float				OBB_Extent[3];
	
#ifdef _XDWDEBUG
	CSuModelBox			m_OBBBox;
#endif

	FLOAT				m_ObjectRadius;

	std::string			m_strName;

	BOOL				m_bBakemap;
	
	BOOL				m_TexturePreCached;
public:

	_XMeshContainer();
	virtual ~_XMeshContainer();

	BOOL				Load( LPSTR szFilename, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE );
	BOOL				Load( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL );

	BOOL				CreateNode( int nodecount );
	void				DeleteNode( void );
	void				DestroyModel( void );
	
	void				SetTextureArchive( _XTextureManager* pTextureManager )
	{
						m_pTextureManager	=	pTextureManager;
	}
 
	void				TexturePreCaching( void );

	inline void			SetNodeCnt(int nodecount){ m_NodeCnt = nodecount; };
				
	_LPXM_MESHOBJECT	GetNode(int index );
	_LPXM_MESHOBJECT	GetNode(LPSTR objectname );
	inline int			GetNodeCnt( void ){ return m_NodeCnt; };
	int					FindObject( LPSTR objectname );
		
	void				Render( _XModelDescriptor* pAniData );
	void				Render_UseGlobalMaterial( _XModelDescriptor* pAniData );
	void				Render_SecondBuffer( _XModelDescriptor* pAniData );
	void				Render( _XModelDescriptor* pAniData, D3DXMATRIX& worldmat );
		
	void				RenderInterfaceModel(_XModelDescriptor* pAniData, D3DXMATRIX& worldmat);	
	void				RenderInterfaceItem(D3DXMATRIX* rotmat);	
	
	void				Render( D3DMATRIX& worldmatrix );
	void				RenderNoTexture( D3DMATRIX& worldmatrix );
	void				RenderNoTexture( _XModelDescriptor* pAniData, D3DMATRIX& worldmatrix );
	void				Render_OriginalPos( D3DXVECTOR3& offset, int index, D3DXMATRIX* rotmat = NULL );

	void				RenderAlphaBlend( D3DMATRIX& worldmatrix, DWORD Alphalevel = 0x40 );
	
	void				_Render( _XModelDescriptor* pAniData );
	void				_Render( D3DMATRIX& worldmatrix );

	void				_RenderAlphaBlend( D3DMATRIX& worldmatrix, DWORD Alphalevel = 0x40 );	

	void				InsertDrawStack( void );
	void				CustomDraw( _XModelDescriptor* pAniData );
	void				CustomDraw( _XOctreeObject* pObject );
	void				CustomLightmapDraw( _XOctreeObject* pObject );
	void				RenderCustomDraw( D3DMATRIX& worldmatrix );
	void				RenderCustomDraw( _XModelDescriptor* pAniData, D3DMATRIX& worldmatrix );

	void				RenderShadow( void );
	void				MakeShadowMatrix( FLOAT& fXMax, FLOAT& fYMax, D3DXMATRIX& matLocalToLight );

	void				RenderNormal(   D3DXMATRIX& trmatrix );
	void				RenderNormal(   _XModelDescriptor* pAniData );
	void				RenderNormal(   _XModelDescriptor* pAniData, D3DXMATRIX& worldmat );

	void				Animation(	    _XModelDescriptor* pAniData );	
	void				SlerpAnimation( _XModelDescriptor* pAniData );
	void				FrameAnimation( _XModelDescriptor* pAniData );		

	BOOL				InterOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXVECTOR3 Offset );
	BOOL				InterOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXMATRIX  Offset );
	BOOL				InterWeaponOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXVECTOR3 Offset, 
											 D3DXVECTOR3 &Center,D3DXVECTOR3 Axis[3],float Extent[3]);

	void				FrameAnimation( _XBipedController* pBipedController );
	void				Render_SecondBuffer( D3DMATRIX& worldmatrix );
	void				Render_BakemapBuffer( D3DMATRIX& worldmatrix );
};

void GetFileNameFromPathString( TCHAR *value, TCHAR *line );

#endif // !defined(_XMESHCONTAINER_H_)
