// XParticleList.h: interface for the _XParticleList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPARTICLELIST_H_
#define _XPARTICLELIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

#include "XList.h"
#include "XRecyclingArray.h"
#include "XParticleElement.h"
#include "XParticleEmitter.h"
#include "XRenderStyleManager.h"
#include "XMeshContainer.h"
#include "XRenderStyleManager.h"
#include "XTextureManager.h"
#include "XSplineContainer.h"
#include "XParticleMeshObject.h"

#define _XDEF_PARTICLESTRUCTUREFILEHEADER		0xFEEB
#define _XDEF_PARTICLESTRUCTUREFILEVERSION		0x05a5

typedef std::list<_XParticleElement*>	sldef_XParticleElement;

class _XParticleActionBase;
class _XDecalObject;
class _XParticleGroupManager;

typedef struct _XParticleSendParameter
{
	_XModelDescriptor*		pModelDescryptor;
	_XModelDescriptor*		pTargetModelDescryptor;
	const D3DXMATRIX*		pParentMatrix;
	const D3DXVECTOR3*		pTargetPosition;
	const D3DXVECTOR3*		pItemPosition;	
	_XParticleGroupManager* pTemplateManager;
	FLOAT					fAdditionalHeight;
} _XParticleSendParam;

class _XParticleList : public _XItem
{
public:
	
	struct _XPInstanceData
	{
		// action manager
		_XList						actionlist;

		FLOAT						maxtracktime;
		int							maxparticles;	

		// life type
		_XParticleLifeTimeType		lifetimetype;
		FLOAT						limittime;

		// Emitter instance data
		bool						vertexB_tracks;
		_XParticleEmitter			Size;
		_XParticleEmitter			Velocity;
		_XParticleEmitter			VertexB;
		_XParticleEmitter			Color;
		FLOAT						Alpha;
		FLOAT						Age;
		FLOAT						AgeSigma;

		// Render style	data
		FLOAT						RenderStyleChangeTime;		
		int							RenderStyleCount;
		int*						RenderStyleIndexArray;
				
		int							MeshModelIndex;
		TCHAR						MeshModelName[32];

		// particle instace data
		_XParticleViewTargetType	ViewtTargetType;
		_XParticleRenderingType		RenderingType;
		
		// Effect link type
		_XParticleBipedLinkType		BipedLinkType;			// transforming from the biped node 
		BOOL						LimitBipedRotation;		// disable rotaion & scaling...

		// Emitter link type
		_XParticleBipedLinkType		EmitterBipedLinkType;	// transforming emit point from the biped node
		DWORD						LinkSubTarget;

		// Particle head rotation type
		D3DXVECTOR3					ArbitrarilyPoint;
		D3DXVECTOR3					BaseRotateAngle;
		D3DXMATRIX					BaseRotateMatrix;

		D3DXVECTOR3					RotationFactor;
		
		// Resource list pointer
		_XParticleMeshObject*		m_pParticleMeshList;
		_XSplineContainer*			m_pEmitterPathList;
		_XRenderStyleManager*		m_pRenderStyleManager;
		_XTextureManager*     		m_pTextureManager;

		BYTE						m_NumericData[5];
		int							m_StartNumericDataSlot;
		int							m_NumericDataLength;
		
		D3DXVECTOR3					m_LastEmitPosition[2];
		
		_XPInstanceData();
		~_XPInstanceData();

		void	CreateRenderStyleArray( int size );
		BOOL	RebuildRenderingObject( _XParticleRenderingType renderobjecttype );		
		void	SetBaseRotationAngle( FLOAT yaw, FLOAT pitch, FLOAT roll );
		void	SetRotationFactor( FLOAT yaw, FLOAT pitch, FLOAT roll );

		void	SetColor(float red, float green, float blue, float alpha = 1.0f);
		void	SetColorD(float alpha, _XParticleEmitterType dtype,
				float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
				float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
				float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f);
		void	SetVelocity(float x, float y = 1.0f, float z = 1.0f);	
		void	SetVelocityD(_XParticleEmitterType dtype,
				float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
				float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
				float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f);
		void	SetVertexB(float x, float y, float z);
		void	SetVertexBD(_XParticleEmitterType dtype,
				float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
				float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
				float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f);
		void	SetVertexBTracks(bool trackVertex = true);
		void	SetSize(float size_x, float size_y = 1.0f, float size_z = 1.0f);
		void	SetSizeD(_XParticleEmitterType dtype,
				float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
				float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
				float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f);
		void	SetStartingAge(float age, float sigma = 1.0f);			

		void	SetNumericData( int number );
	};
	
	// Effect instance
	_XPInstanceData*			m_pInstanceData;
	BOOL						m_InstanceCreated;

	// particle manager
	sldef_XParticleElement		m_slParticleElement;

	// base attribute
	FLOAT						dt;
	FLOAT						ParticleLifeTime;
	
	// Render style
	int							CurrentRenderStyleIndex;
	FLOAT						AccumulateRenderStyleChangeTime;		
	
	FLOAT						Accumulatesplinepointindex;
	BOOL						EmitPathDirectionFrag;
	FLOAT						FrameEmitAccumulater;

	D3DXVECTOR3					RotateAnimationFactor;

	BOOL						m_FadeMode;
			
	D3DXMATRIX					m_ParentMatrix;
	D3DXVECTOR3					m_TargetPosition;
	D3DXVECTOR3					m_LinkPosition[3];			// 0 : Weapon top 1 : Weapon bottom  2: Item pos
	_XModelDescriptor*			m_pParentModelDescriptor;

	BOOL						m_bSoundReady;
	BOOL						m_bLightReady;
	BOOL						m_bScreenReady;

	_XDecalObject*				m_pDecalObject;
	
	BOOL						m_UsedTargetMatrix;
	D3DXVECTOR3					m_LastUsedTargetPosition;
	
	//D3DXVECTOR3					m_EmitPosition;
	
	BOOL						m_CreatedParticle;

	FLOAT						m_LimitMaxParticleSize;
	FLOAT						m_AdditionalHeight;
	
public:
	_XParticleList();
	_XParticleList( _XPInstanceData* pInstanceData );
	virtual ~_XParticleList();

	void			Initialize( _XPInstanceData* pInstanceData );
	void			Release( void );

	static BOOL		CreateParticleBillboardObject( void );
	static void		ReleaseParticleBillboardObject( void );

	HRESULT			RestoreDeviceObjects( void );
	void			InvalidateDeviceObjects( void );
	
	BOOL			Load( FILE* fileptr );

	void			SetTimeStep(float newDT);

	void			AddParticle( _XParticleElement& particledata );
	void			RemoveParticle( _XParticleElement* particledata );
	void			DeleteAllParticles(void);
		
	void			Update( _XParticleSendParameter* pParameter );
	void			Render( void );
	
	_XParticleList* MakeInstance( const D3DXMATRIX* pParentMatrix = NULL );

};

class _XParticleGroupItem : public _XList
{
public:	
	int			m_ID;
	int			m_UniqueID;
	DWORD		m_CreatedTime;
	BOOL		m_NoFadeObject;	
	D3DXVECTOR3	m_vPosition;
	int			m_MaxInstanceCount;
	
public:	
	_XParticleGroupItem()
	{		
		Initialize();
	}
	
	~_XParticleGroupItem()
	{
	}	

	void	Initialize( void )
	{
			m_ID = -1;
			m_UniqueID = -1;
			m_NoFadeObject = FALSE;
			m_vPosition = D3DXVECTOR3(0, 0, 0);
			m_MaxInstanceCount = 5;
	}
	void	Release( void );
			
	BOOL	InsertParticle( _XParticleList* pNewParticle );
	HRESULT RestoreDeviceObjects( void );
	void	InvalidateDeviceObjects( void );
	void	Update( _XParticleSendParameter* pParameter );
	void	Render( void );

	BOOL	Load( FILE* fileptr );	

	_XParticleGroupItem* MakeInstance( const D3DXMATRIX* pParentMatrix = NULL, BOOL nofadeobject = FALSE );

	void	StartFade( void );
	void	SetNumericData( int number );
	
	void	MovePosition( D3DXMATRIX newmat );
	void	SetParticleMaxSize( D3DXVECTOR3& size );
};


class _XParticleGroupManager 
{
public:
	FLOAT					m_fRemainTime;
	int						m_ParticleMeshCount;
	_XParticleMeshObject*	m_pParticleMeshList;

	int						m_EmitterPathCount;
	_XSplineContainer*		m_pEmitterPathList;

	_XRenderStyleManager	m_RenderStyleManager;
	_XTextureManager		m_TextureManager;
	
	int						m_ParticleTemplateCount;
	_XParticleGroupItem*	m_ParticleTemplateList;

public:
	_XParticleGroupManager();
	~_XParticleGroupManager();
	
	void	Dispose( void );
		
	HRESULT RestoreDeviceObjects( void );
	void	InvalidateDeviceObjects( void );
	void	Update( FLOAT elapsedtime, FLOAT apptime );

	BOOL	Load( FILE* fileptr );
	BOOL	Load( LPTSTR filename );

	_XParticleGroupItem* MakeNewInstance( int templateid, const D3DXMATRIX* pParentMatrix = NULL );
	_XParticleGroupItem* FindTemplate( int templateid );
	void FindTemplate( int templateid, int& index );
};


class _XParticleGroupInstanceManager : public _XList
{
private:
	_XParticleGroupManager*	m_pTemplateManager;

public:
	_XParticleGroupInstanceManager();
	~_XParticleGroupInstanceManager();	

	void					SetTemplateManager( _XParticleGroupManager*	pTemplateManager )
	{
							m_pTemplateManager = pTemplateManager;
	}
	
	BOOL					InsertParticleGroup( _XParticleGroupItem* pNewParticle );
	_XParticleGroupItem*	InsertNewInstance( _XParticleGroupManager* pParticleTemplate, 
											   int templateid, const D3DXMATRIX* pParentMatrix,
											   int* pNumericData = NULL, BOOL nofadeobject = FALSE, FLOAT maxsize = -1.0f );
	
	void					Update( _XModelDescriptor* pModelDescryptor,const D3DXMATRIX* pParentMatrix, 
									const D3DXVECTOR3* pTargetPosition, const D3DXVECTOR3* pItemPosition = NULL, 
									_XModelDescriptor* pTargetModelDescryptor = NULL, FLOAT additionalheight = 0.0f );

	void					Render( void );
	void					RenderWorldEffect( void );

	void					disposeList(void); // Virtual inherited.
	void					Dispose( void );	
	void					StartFade( void );
	void					StartFade( int templateid );	
	void					StartFade_Except( int templateid );

	void					DeleteInstance( _XParticleGroupItem* pDeleteParticle );
	void					DeleteInstance( int templateid );
	void					DeleteUniqueInstance( int uniqueid );
	_XParticleGroupItem*	FindInstance( int templateid );

	void					SetParticleMaxSize( D3DXVECTOR3& size );
};

#endif // #ifndef _XPARTICLELIST_H_





