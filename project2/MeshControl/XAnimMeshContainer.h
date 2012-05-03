// XAnimMeshContainer.h: interface for the _XAnimMeshContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XANIMMESHCONTAINER_H__7CA7E3A9_28DE_4324_ABAB_8138521217CA__INCLUDED_)
#define AFX_XANIMMESHCONTAINER_H__7CA7E3A9_28DE_4324_ABAB_8138521217CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XModelContainerDef.h"

// by mahwang
#include "XKeyframeController.h"


#include "XReference.h"

#include <string>

class _XTextureManager;
class _XTextureController;
class _XModelDescriptor;

class _XAnimMeshAbstractClass : public _XItem, public _XReference
{
public:	
	static	BOOL				m_UseVertexShaderAnimation;

	std::string					m_strFileName;

	_LPXM_MESHOBJECT			m_Node;
	int							m_NodeCnt;	
	
	_XTextureManager*			m_pTextureManager;
	_XTextureController*		m_pTextureController;
	
	D3DXVECTOR3					OBB_Center;
    D3DXVECTOR3					OBB_Axis[3];
	float						OBB_Extent[3];
	
	int							m_pLODObjectCount;
	_XAnimMeshAbstractClass**	m_pLODObjectList;
	std::string					m_strLodFileName[2];
	
	bool						m_Initialized;
	bool						m_ThreadLoad;
	
#ifdef _XDWDEBUG
	CSuModelBox					m_OBBBox;
#endif
	
	FLOAT						m_ObjectRadius;
	
	LONG						m_LastUsedTime;

	LPVOID						m_pExtraData;
public:
	
	virtual ~_XAnimMeshAbstractClass(){ }
	static	_XAnimMeshAbstractClass*	CreateAnimationMesh( void );

	virtual BOOL						CreateNode( int nodecount );
	virtual void						DeleteNode( void );
	virtual void						DestroyModel( void );	
	virtual void						SetTextureArchive( _XTextureManager* pTextureManager ){ m_pTextureManager =	pTextureManager; }
	
	virtual inline void					SetNodeCnt(int nodecount){ m_NodeCnt = nodecount; };
				
	virtual _LPXM_MESHOBJECT			GetNode(int index );
	virtual int							GetNodeCnt( void ){ return m_NodeCnt; };
	
	virtual BOOL						Load( LPSTR szFilename, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE ) = 0;
	virtual BOOL						Load( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL ) = 0;
	virtual BOOL						Load_Runtime( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL ) = 0;
	virtual BOOL						Load( FILE* Fileptr, _XTextureController* pTextureController, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL ) = 0;
	
	virtual void						Render( D3DXMATRIX* pmatKeyframe ) = 0;
	virtual void						RenderNoTexture( D3DXMATRIX* pmatKeyframe ) = 0;
	virtual BOOL						InitializeSecondBufferFormTM( _LPXM_KEYFRAME lpCurKeyframe ) = 0;

	virtual void						Render( D3DXMATRIX worldmat ) = 0;
	virtual void						Render( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL ) = 0;
	virtual void						Render_LODCheck( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL ) = 0;
	virtual void						Render_LowLevelLOD( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL ) = 0;
	virtual void						Render_Reflect( _XModelDescriptor* pAniData, D3DXMATRIX* pReflectViewMatrix ) = 0;
	
	virtual void						RenderNoTexture( D3DXMATRIX worldmat ) = 0;
	virtual void						RenderNoTexture( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL ) = 0;

	virtual void						RenderCustomDraw( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL ) = 0;

	virtual void						Render_SecondBuffer( _XModelDescriptor* pAniData ) = 0;	

	virtual void						RenderShadow( _XModelDescriptor* pAniData ) = 0;
	virtual void						RenderShadow_LODCheck( _XModelDescriptor* pAniData ) = 0;

	virtual void						RenderInterfaceItem(D3DXMATRIX* rotmat) = 0;

	virtual void						Render_OriginalPos( D3DXVECTOR3& offset, int index, D3DXMATRIX* rotmat = NULL, BOOL usealphatest = TRUE ) = 0;
	
	virtual void						Animation(	    _XModelDescriptor* pAniData ) = 0;
	virtual void						SlerpAnimation( _XModelDescriptor* pAniData ) = 0;
	virtual void						FrameAnimation( _XModelDescriptor* pAniData ) = 0;

	virtual BOOL						InterOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, D3DXVECTOR3& Offset, D3DXVECTOR3* pPickedPoint = NULL );	
	virtual BOOL						InterOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, D3DXVECTOR3& Offset, FLOAT* pRotateAngle, D3DXVECTOR3* pPickedPoint = NULL );
	virtual BOOL						InterWeaponOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, D3DXVECTOR3 Offset, 
															 D3DXVECTOR3 &Center,D3DXVECTOR3 Axis[3],float Extent[3]);

	virtual void						RebuildOBBInfo( void ) = 0;

	virtual void						CreateLODObject( int lodobjectcount );
	
	virtual void						SortByAlphaBlendAttrib( BOOL usemanager );

	void								CheckLastUsedTime( void );
};

class _XVSAnimMeshContainer  : public _XAnimMeshAbstractClass
{
public:
	
	_XVSAnimMeshContainer();
	virtual ~_XVSAnimMeshContainer();

	static	BOOL		InitializeVertexAnimationShader( void );
	static	void		ReleaseVertexAnimationShader( void );
	
	BOOL				InitializeSecondBufferFormTM( _LPXM_KEYFRAME lpCurKeyframe );
	BOOL				Load( LPSTR szFilename, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE );
	BOOL				Load( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL );
	BOOL				Load_Runtime( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL );
	BOOL				Load( FILE* Fileptr, _XTextureController* pTextureController, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL );
					
	void				Animation(	    _XModelDescriptor* pAniData );
	void				FrameAnimation( _XModelDescriptor* pAniData );	
	void				SlerpAnimation( _XModelDescriptor* pAniData );

	void				SetVertexShaderConst( _XModelDescriptor* pAniData,  D3DXMATRIX* pworldmat = NULL, D3DXMATRIX* pviewmat = NULL );
	void				SetBoneConstant( _LPXM_MESHOBJECT lpCh, _XModelDescriptor* pAniData );
	
	void				Render( D3DXMATRIX* pmatKeyframe );
	void				RenderNoTexture( D3DXMATRIX* pmatKeyframe );
	void				Render( D3DXMATRIX worldmat );
	void				Render( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );	
	void				Render_LODCheck( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	void				Render_LowLevelLOD( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	void				Render_Reflect( _XModelDescriptor* pAniData, D3DXMATRIX* pReflectViewMatrix );
	
	void				RenderNoTexture( D3DXMATRIX worldmat );
	void				RenderNoTexture( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	
	void				RenderCustomDraw( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	void				Render_SecondBuffer( _XModelDescriptor* pAniData ){ Render( pAniData ); }

	void				RenderShadow(  _XModelDescriptor* pAniData );
	void				RenderShadow_LODCheck( _XModelDescriptor* pAniData );

	void				RenderInterfaceItem(D3DXMATRIX* rotmat);
	void				Render_OriginalPos( D3DXVECTOR3& offset, int index, D3DXMATRIX* rotmat = NULL, BOOL usealphatest = TRUE  );

	void				RebuildOBBInfo( void );
};

class _XNSAnimMeshContainer  : public _XAnimMeshAbstractClass
{
public:
	
	_XNSAnimMeshContainer();
	virtual ~_XNSAnimMeshContainer();
	
	BOOL				InitializeSecondBufferFormTM( _LPXM_KEYFRAME lpCurKeyframe );
	BOOL				Load( LPSTR szFilename, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE );
	BOOL				Load( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL );
	BOOL				Load_Runtime( FILE* Fileptr, _XTextureManager* pTextureManager, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL );
	BOOL				Load( FILE* Fileptr, _XTextureController* pTextureController, BOOL createsecondbuffer = TRUE, LPSTR szFilename = NULL );

	void				Animation(	    _XModelDescriptor* pAniData );
	void				FrameAnimation( _XModelDescriptor* pAniData );
	void				SlerpAnimation( _XModelDescriptor* pAniData );

	void				Render( D3DXMATRIX* pmatKeyframe );
	void				RenderNoTexture( D3DXMATRIX* pmatKeyframe );

	void				Render( D3DXMATRIX worldmat );
	void				Render( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	void				Render_LODCheck( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	void				Render_LowLevelLOD( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	void				Render_Reflect( _XModelDescriptor* pAniData, D3DXMATRIX* pReflectViewMatrix );

	void				RenderNoTexture( D3DXMATRIX worldmat );
	void				RenderNoTexture( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );

	void				RenderCustomDraw( _XModelDescriptor* pAniData, D3DXMATRIX* pworldmat = NULL );
	void				Render_SecondBuffer( _XModelDescriptor* pAniData );

	void				RenderShadow( _XModelDescriptor* pAniData );
	void				RenderShadow_LODCheck( _XModelDescriptor* pAniData );

	void				RenderInterfaceItem(D3DXMATRIX* rotmat);
	void				Render_OriginalPos( D3DXVECTOR3& offset, int index, D3DXMATRIX* rotmat = NULL, BOOL usealphatest = TRUE  );
	
	void				RebuildOBBInfo( void );
};




#endif // !defined(AFX_XANIMMESHCONTAINER_H__7CA7E3A9_28DE_4324_ABAB_8138521217CA__INCLUDED_)
