// XParticleList.cpp: implementation of the _XParticleList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XParticleList.h"
#include "XParticleAction.h"
#include "EODEXTDEF_GlobalObject.h"

// by mahwang
#include "XKeyframeController.h"
#include "XKeyframeDescriptor.h"

#include "XParticleRenderManager.h"

#include "XDecalObject.h"

extern FLOAT _XMeshMath_GetRotateRadian( D3DXVECTOR3& a, D3DXVECTOR3& b );
extern D3DXQUATERNION _XRotationArcNormalized( D3DXVECTOR3* vec1, D3DXVECTOR3* vec2 );

static D3DXVECTOR3 vecUp( 0.000001f, 1.0f, 0.000001f);

static	_XParticleRenderManager		g_ParticleRenderManager;
_XEffect_LineEffect					g_LineEffectFactory;

XMemoryPool<_XParticleList>			g_ParticleListPool;
XMemoryPool<_XParticleGroupItem>	g_ParticleGroupItemPool;
XMemoryPool<_XDecalObject>			g_ParticleDecalObjectPool;

extern D3DXVECTOR3	g_DecalUpVector;
extern D3DXVECTOR3	g_DecalTangentVector;
extern BOOL			g_SetTextureFailed;

#ifdef _XTESTSERVER		
#define _XDEF_VISUALEFFECTMODELARCHIVENAME				"Model\\tsMR_VISUALEFFECT.XP"
#define _XDEF_TEXTUREFILE_VISUALEFFECT					"tsTR_VISUALEFFECT.XP"
#else
#define _XDEF_VISUALEFFECTMODELARCHIVENAME				"Model\\MR_VISUALEFFECT.XP"
#define _XDEF_TEXTUREFILE_VISUALEFFECT					"TR_VISUALEFFECT.XP"
#endif

#define _XDEF_MAXEVENTACTIONINTANCINGCOUNT	5
int		g_LastEventParticleIDArray[_XDEF_MAXEVENTACTIONINTANCINGCOUNT];
int		g_LastEventCount = 0;
_XModelDescriptor* g_pLastEventTargetModelDescriptor = NULL;


//////////////////////////////////////////////////////////////////////////////////////////////////
// _XParticleList::_XPInstanceData

_XParticleList::_XPInstanceData::_XPInstanceData()
{
	lifetimetype			= _XParticleLT_OneTime;
	maxtracktime			= 20.0f;	
	limittime				= 20.0f;
	
	vertexB_tracks			= FALSE;
	Size					= _XParticleEmitter(_XPE_Point, 1.0f, 1.0f, 1.0f);
	Velocity				= _XParticleEmitter(_XPE_Sphere, 0.0f, 5.0f, 0.0f, 0.0f, 7.0f);
	VertexB					= _XParticleEmitter(_XPE_Point, 0.0f, 0.0f, 0.0f);
	Color					= _XParticleEmitter(_XPE_Point, 1.0f, 1.0f, 1.0f);
	Alpha					= 1.0f;
	Age						= 0.0f;
	AgeSigma				= 0.0f;

	maxparticles			= 1;

	RenderStyleCount		= 0;
	RenderStyleIndexArray	= NULL;
	RenderStyleChangeTime	= 0.1f;

	CreateRenderStyleArray( 1 );
	
	m_pParticleMeshList		= NULL;
	m_pEmitterPathList		= NULL;
	m_pRenderStyleManager	= NULL;

	BipedLinkType			= _XParticleBLT_Character;
	LimitBipedRotation		= FALSE;
	EmitterBipedLinkType	= _XParticleBLT_Character;
	LinkSubTarget			= _XEMITTERSUBLINKTARGET_NONE;
	
	memset( MeshModelName, 0, sizeof(TCHAR) * 32 );
	MeshModelIndex			= -1;	

	ViewtTargetType			= _XParticleVT_Camera;
	
	RebuildRenderingObject( _XParticleRT_Billboard );

	D3DXMatrixIdentity( &BaseRotateMatrix );

	RotationFactor.x = ArbitrarilyPoint.x = BaseRotateAngle.x = 0.0f;
	RotationFactor.y = ArbitrarilyPoint.y = BaseRotateAngle.y = 0.0f;
	RotationFactor.z = ArbitrarilyPoint.z = BaseRotateAngle.z = 0.0f;
}

_XParticleList::_XPInstanceData::~_XPInstanceData()
{
	SAFE_DELETE( RenderStyleIndexArray );	
	actionlist.disposeList();
}

void _XParticleList::_XPInstanceData::CreateRenderStyleArray( int size )
{
	if( RenderStyleCount != size || !RenderStyleIndexArray )
	{
		RenderStyleCount = size;
		SAFE_DELETE( RenderStyleIndexArray );
		RenderStyleIndexArray = new int[RenderStyleCount];
	}
}

BOOL _XParticleList::_XPInstanceData::RebuildRenderingObject( _XParticleRenderingType renderobjecttype )
{
	RenderingType = renderobjecttype;
	return TRUE;
}

void _XParticleList::_XPInstanceData::SetNumericData( int number )
{
	if( number > 99999 ) number = number % 100000;
	TCHAR buffer[32];
	itoa( number, buffer, 10 );
	int stringlength = strlen( buffer );
	
	if( stringlength == 1 )
	{
		m_StartNumericDataSlot = 2;
		m_NumericDataLength = 1;
		m_NumericData[0] = 255;
		m_NumericData[1] = 255;
		m_NumericData[2] = buffer[0] - 0x30; //'0'
		m_NumericData[3] = 255;
		m_NumericData[4] = 255;
	}
	else if( stringlength == 2 )
	{
		m_StartNumericDataSlot = 1;
		m_NumericDataLength = 2;
		m_NumericData[0] = 255;
		m_NumericData[1] = buffer[0] - 0x30; //'0'
		m_NumericData[2] = buffer[1] - 0x30;
		m_NumericData[3] = 255;
		m_NumericData[4] = 255;
	}
	else if( stringlength == 3 )
	{
		m_StartNumericDataSlot = 1;
		m_NumericDataLength = 3;
		m_NumericData[0] = 255;
		m_NumericData[1] = buffer[0] - 0x30; //'0'
		m_NumericData[2] = buffer[1] - 0x30;
		m_NumericData[3] = buffer[2] - 0x30;
		m_NumericData[4] = 255;
	}
	else if( stringlength == 4 )
	{
		m_StartNumericDataSlot = 0;
		m_NumericDataLength = 4;
		m_NumericData[0] = buffer[0] - 0x30; //'0'
		m_NumericData[1] = buffer[1] - 0x30;
		m_NumericData[2] = buffer[2] - 0x30;
		m_NumericData[3] = buffer[3] - 0x30;
		m_NumericData[4] = 255;
	}		 
	else if( stringlength == 5 )
	{
		m_StartNumericDataSlot = 0;
		m_NumericDataLength = 5;
		m_NumericData[0] = buffer[0] - 0x30; //'0'
		m_NumericData[1] = buffer[1] - 0x30;
		m_NumericData[2] = buffer[2] - 0x30;
		m_NumericData[3] = buffer[3] - 0x30;
		m_NumericData[4] = buffer[4] - 0x30;
	}		 
	else
	{
		m_StartNumericDataSlot = 0;
		m_NumericDataLength = 0;
		m_NumericData[0] = 255;
		m_NumericData[1] = 255;
		m_NumericData[2] = 255;
		m_NumericData[3] = 255;
		m_NumericData[4] = 255;
	}
}


BOOL _XParticleList::CreateParticleBillboardObject( void )
{
//	_XParticleList::ReleaseParticleBillboardObject();
	g_ParticleRenderManager.Initialize();
	return TRUE;
}

void _XParticleList::ReleaseParticleBillboardObject( void )
{
	g_ParticleRenderManager.Destroy();
}

void _XParticleList::_XPInstanceData::SetBaseRotationAngle( FLOAT yaw, FLOAT pitch, FLOAT roll )
{
	BaseRotateAngle.x = yaw;
	BaseRotateAngle.y = pitch;
	BaseRotateAngle.z = roll;
	
	D3DXMatrixRotationYawPitchRoll( &BaseRotateMatrix, yaw, pitch, roll );
}

void _XParticleList::_XPInstanceData::SetRotationFactor( FLOAT yaw, FLOAT pitch, FLOAT roll )
{
	RotationFactor.x = yaw;
	RotationFactor.y = pitch;
	RotationFactor.z = roll;
}

void _XParticleList::_XPInstanceData::SetColor(float red, float green, float blue, float alpha)
{		
	Alpha = alpha;
	Color = _XParticleEmitter(_XPE_Point, red, green, blue);
}

void _XParticleList::_XPInstanceData::SetColorD(float alpha, 
							   _XParticleEmitterType dtype,
							   float a0, float a1, float a2,
							   float a3, float a4, float a5,
							   float a6, float a7, float a8)
{		
	Alpha = alpha;
	Color = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

void _XParticleList::_XPInstanceData::SetVelocity(float x, float y, float z)
{
	Velocity = _XParticleEmitter(_XPE_Point, x, y, z);
}

void _XParticleList::_XPInstanceData::SetVelocityD(
						          _XParticleEmitterType dtype,
								  float a0, float a1, float a2,
								  float a3, float a4, float a5,
								  float a6, float a7, float a8)
{
	Velocity = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

void _XParticleList::_XPInstanceData::SetVertexB(float x, float y, float z)
{
	VertexB = _XParticleEmitter(_XPE_Point, x, y, z);
}

void _XParticleList::_XPInstanceData::SetVertexBD(
						         _XParticleEmitterType dtype,
								 float a0, float a1, float a2,
								 float a3, float a4, float a5,
								 float a6, float a7, float a8)
{
	VertexB = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}


void _XParticleList::_XPInstanceData::SetVertexBTracks(bool trackVertex)
{
	vertexB_tracks = trackVertex;
}

void _XParticleList::_XPInstanceData::SetSize(float size_x, float size_y, float size_z)
{
	Size = _XParticleEmitter(_XPE_Point, size_x, size_y, size_z);
}

void _XParticleList::_XPInstanceData::SetSizeD(_XParticleEmitterType dtype,
							  float a0, float a1, float a2,
							  float a3, float a4, float a5,
							  float a6, float a7, float a8)
{
	Size = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

void _XParticleList::_XPInstanceData::SetStartingAge(float age, float sigma)
{
	Age = age;
	AgeSigma = sigma;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// _XParticleList
_XParticleList::_XParticleList()
{	
#ifdef _XDEF_MODELTHREADLOADING
	m_pInstanceData			= NULL;
	m_InstanceCreated		= FALSE;
#else
	m_pInstanceData			= NULL;//new _XPInstanceData;	
	m_InstanceCreated		= TRUE;	
#endif

	m_pParentModelDescriptor= NULL;	
	m_FadeMode				= FALSE;

	m_bSoundReady			= FALSE;
	m_bLightReady			= FALSE;
	m_bScreenReady			= FALSE;

	m_pDecalObject			= NULL;
	
	m_UsedTargetMatrix		= FALSE;
	
	m_CreatedParticle		= FALSE;

	memset( m_LinkPosition, 0, sizeof( D3DXVECTOR3 ) * 3 );
	//m_EmitPosition.x		= m_EmitPosition.y = m_EmitPosition.z = 0.0f;
	
	m_LimitMaxParticleSize	= -1.0f;
	m_AdditionalHeight		= 0.0f;
	FrameEmitAccumulater	= 0.0f;
}

_XParticleList::_XParticleList( _XParticleList::_XPInstanceData* pInstanceData )
{
	Initialize( pInstanceData );
}

void _XParticleList::Initialize( _XPInstanceData* pInstanceData )
{
	m_pInstanceData			= pInstanceData;

	dt						= 0.0f;
	ParticleLifeTime		= -1.0f;
	
	CurrentRenderStyleIndex = 0;
	AccumulateRenderStyleChangeTime = 0.0f;

	Accumulatesplinepointindex = 0.0f;
	EmitPathDirectionFrag = TRUE; // true = foward..
	FrameEmitAccumulater	= 0.0f;
	
	D3DXMatrixIdentity( &m_ParentMatrix );
	m_TargetPosition.x = 0.0f;
	m_TargetPosition.y = 0.0f;
	m_TargetPosition.z = 0.0f;

	m_pParentModelDescriptor = NULL;
	
	RotateAnimationFactor.x = 0.0f;
	RotateAnimationFactor.y = 0.0f;
	RotateAnimationFactor.z = 0.0f;
	
	memset( m_LinkPosition, 0, sizeof( D3DXVECTOR3 ) * 3 );

	//m_EmitPosition.x		= m_EmitPosition.y = m_EmitPosition.z = 0.0f;

	m_InstanceCreated = FALSE;
	m_FadeMode		  = FALSE;

	m_bSoundReady			= FALSE;
	m_bLightReady			= FALSE;
	m_bScreenReady			= FALSE;

	m_pDecalObject			= NULL;

	m_UsedTargetMatrix		= FALSE;
	
	m_LastUsedTargetPosition.x = 0.0f;
	m_LastUsedTargetPosition.y = 0.0f;
	m_LastUsedTargetPosition.z = 0.0f;

	m_CreatedParticle		= FALSE;

	if( m_pInstanceData->RenderingType == _XParticleRT_TerrainDecal )
	{
#ifdef _XDEF_MODELTHREADLOADING
		_ASSERT( !m_pDecalObject );
		m_pDecalObject = g_ParticleDecalObjectPool.New();
#else
		if( m_pDecalObject )
			SAFE_DELETE(m_pDecalObject);
		m_pDecalObject = new _XDecalObject;
#endif
	}

	m_LimitMaxParticleSize	= -1.0f;
	m_AdditionalHeight		= 0.0f;
	
}

HRESULT _XParticleList::RestoreDeviceObjects( void )
{	
	//HRESULT hr;

	InvalidateDeviceObjects();	
	if( m_pDecalObject )
	{
		m_pDecalObject->RestoreDeviceObject();
	}
				
	return S_OK;
}

void _XParticleList::InvalidateDeviceObjects( void )
{	
	if( m_pDecalObject )
	{
		m_pDecalObject->InvalidateDeviceObject();
	}
}

_XParticleList::~_XParticleList()
{
	Release();
}

void _XParticleList::Release( void )
{
	if( m_InstanceCreated )
	{
		SAFE_DELETE( m_pInstanceData );
		_ASSERT( m_slParticleElement.size() == 0 );
	}
	else
	{
		DeleteAllParticles();
	}

	InvalidateDeviceObjects();

#ifdef _XDEF_MODELTHREADLOADING
	if( m_pDecalObject )
	{
		m_pDecalObject->Release();
		g_ParticleDecalObjectPool.Delete( m_pDecalObject );
		m_pDecalObject = NULL;
	}
#else
	SAFE_DELETE( m_pDecalObject );
#endif
}

void _XParticleList::AddParticle( _XParticleElement& particledata )
{
	_XParticleElement* pNewParticle = g_ParticleRenderManager.NewElement();
	if( pNewParticle )
	{
		*pNewParticle = particledata;
		m_slParticleElement.push_back(pNewParticle);
		m_CreatedParticle = TRUE;

		if( m_pInstanceData->RenderingType == _XParticleRT_LineEffect )
		{
			if(!pNewParticle->pLineData)
				pNewParticle->pLineData = new _XEffect_LineEffect_InstanceData;
		}
	}
}

void _XParticleList::RemoveParticle( _XParticleElement* particledata )
{
	if( particledata ) 
	{
		if( m_pInstanceData->RenderingType == _XParticleRT_LineEffect )
		{
			SAFE_DELETE( particledata->pLineData );
		}

		g_ParticleRenderManager.DeleteElement(particledata);
	}
}

void _XParticleList::DeleteAllParticles(void)
{
	sldef_XParticleElement::iterator it;
	for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it)
	{
		g_ParticleRenderManager.DeleteElement(*it);
	}
	m_slParticleElement.clear();
}

void _XParticleList::SetTimeStep(float newDT)
{
	dt = newDT;
}

void _XParticleList::Update( _XParticleSendParameter* pParameter )
{
	if( !m_pInstanceData ) return;

	g_LastEventCount = 0;

	if( m_pInstanceData->actionlist.listEmpty() ) return;

	// Set parent matrix...
	if( pParameter->pModelDescryptor )
	{
		if( m_pInstanceData->BipedLinkType == _XParticleBLT_World )
		{
			//D3DXMatrixIdentity( &m_ParentMatrix);
		}
		else if( m_pInstanceData->BipedLinkType == _XParticleBLT_Character )
		{
			if( m_pInstanceData->LimitBipedRotation )
			{				
				D3DXMatrixTranslation( &m_ParentMatrix, pParameter->pModelDescryptor->m_Position._41, 
										pParameter->pModelDescryptor->m_Position._42, pParameter->pModelDescryptor->m_Position._43 );
			}
			else m_ParentMatrix = pParameter->pModelDescryptor->m_Position;
		}
		else // link biped node...
		{
			/*
			if( m_pInstanceData->RenderingType == _XParticleRT_LineEffect )
			{
				if( m_pInstanceData->EmitterBipedLinkType >= _XParticleBLT_CharacterRightHand )
				{
					m_ParentMatrix = pParameter->pModelDescryptor->m_Position;
				}
				else
				{
					D3DXMatrixMultiply( &m_ParentMatrix, &pParameter->pModelDescryptor->m_PartMatrixContainer[m_pInstanceData->BipedLinkType-_XParticleBLT_CharacterRightHand], 
													 &pParameter->pModelDescryptor->m_Position );
				}
			}
			else*/
			{
				D3DXMatrixMultiply( &m_ParentMatrix, &pParameter->pModelDescryptor->m_PartMatrixContainer[m_pInstanceData->BipedLinkType-_XParticleBLT_CharacterRightHand], 
													 &pParameter->pModelDescryptor->m_Position );
			}
		}
	}
	else if( pParameter->pParentMatrix ) m_ParentMatrix = *pParameter->pParentMatrix;

	// Set target position...
	if( pParameter->pTargetPosition ) m_TargetPosition = *pParameter->pTargetPosition;

	m_pParentModelDescriptor = pParameter->pModelDescryptor;
	
	g_pLastEventTargetModelDescriptor = pParameter->pTargetModelDescryptor;
		
	m_AdditionalHeight = pParameter->fAdditionalHeight;
		
	_XParticleActionBase::localtime = fmod( ParticleLifeTime, m_pInstanceData->maxtracktime );

	if( _XParticleActionBase::localtime < 0.0f ) _XParticleActionBase::localtime = 0.0f;
	else if( _XParticleActionBase::localtime >= m_pInstanceData->maxtracktime ) _XParticleActionBase::localtime = 0.0f;
		
	m_pInstanceData->actionlist.resetList();
	_XParticleActionBase* pActionItem = NULL;
		
	do 
	{
		pActionItem = (_XParticleActionBase*)m_pInstanceData->actionlist.currentItem();
		if( !pActionItem ) break;

		if( m_FadeMode )
		{
			if( pActionItem->actiontype == _XPA_SourceID )
			{
				if( !m_CreatedParticle )
				{
					pActionItem->Execute( this );
				}
			}
			else
			{
				pActionItem->Execute( this );
			}
		}
		else
		{
			pActionItem->Execute( this );

			/*
			if( m_FadeMode )
			{
				break;
			}
			*/
		}
		

		m_pInstanceData->actionlist.nextItem();
	} while( !m_pInstanceData->actionlist.atHeadOfList() );

	if( pParameter->pTargetModelDescryptor && g_LastEventCount > 0 )
	{
		if( pParameter->pTargetModelDescryptor->m_EffectManager )
		{
			for( int i = 0; i < g_LastEventCount; i++ )
			{
				pParameter->pTargetModelDescryptor->m_EffectManager->InsertNewInstance( pParameter->pTemplateManager,
																						g_LastEventParticleIDArray[i], 
																						&pParameter->pTargetModelDescryptor->m_Position );
			}
		}
	}	

	g_pLastEventTargetModelDescriptor = NULL;
}

D3DXMATRIX  g_PROrgViewmatrix;
D3DXMATRIX  g_PRWorldmatrix;
D3DXMATRIX  g_PRRotmat;
D3DXMATRIX  g_PRBaseMatrix;
D3DXVECTOR3 g_PRCamposition;

typedef	void (__stdcall *_XGetViewMatrixCallback)( _XParticleElement& element );

void __stdcall _XGetViewMatrixCallback_Fixed( _XParticleElement& element )
{		
	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRBaseMatrix);
	D3DXMatrixMultiply( &g_PRWorldmatrix, &g_PRRotmat, &g_PRWorldmatrix);
		
	// set the translation portion of the InvViewMatrix to the location of the particles in world space	
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_Camera( _XParticleElement& element )
{			
	// Scaling...
	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );		
	D3DXMatrixMultiply( &g_PRWorldmatrix, &g_PRRotmat, &g_PROrgViewmatrix );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix);

	// set the translation portion of the InvViewMatrix to the location of the particles in world space

	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_FixedAndXRollAnimation( _XParticleElement& element )
{
	D3DXMATRIX rotmatrix;
	D3DXMatrixRotationX( &rotmatrix, element.randomdirection );
	D3DXMatrixMultiply( &rotmatrix, &g_PRRotmat, &rotmatrix );

	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &rotmatrix, &g_PROrgViewmatrix);
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix);	
		
	// set the translation portion of the InvViewMatrix to the location of the particles in world space	
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_FixedAndYRollAnimation( _XParticleElement& element )
{
	D3DXMATRIX rotmatrix;
	D3DXMatrixRotationY( &rotmatrix, element.randomdirection );
	D3DXMatrixMultiply( &rotmatrix, &g_PRRotmat, &rotmatrix );

	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &rotmatrix, &g_PROrgViewmatrix);
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix);	
		
	// set the translation portion of the InvViewMatrix to the location of the particles in world space	
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_FixedAndZRollAnimation( _XParticleElement& element )
{
	D3DXMATRIX rotmatrix;
	D3DXMatrixRotationZ( &rotmatrix, element.randomdirection );
	D3DXMatrixMultiply( &rotmatrix, &g_PRRotmat, &rotmatrix );

	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &rotmatrix, &g_PROrgViewmatrix);
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix);	
		
	// set the translation portion of the InvViewMatrix to the location of the particles in world space	
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_CameraAndRollAnimation( _XParticleElement& element )
{			
	// Scaling...
	D3DXMATRIX scalematrix;
	D3DXMATRIX rotmatrix;
	D3DXMatrixRotationZ( &rotmatrix, element.randomdirection );
	D3DXMatrixMultiply( &rotmatrix, &g_PRRotmat, &rotmatrix );
	
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );		
	D3DXMatrixMultiply( &g_PRWorldmatrix, &rotmatrix, &g_PROrgViewmatrix );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix);
	
	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_LockX( _XParticleElement& element )
{	
	D3DXVECTOR3 vecEye = g_PRCamposition;
	D3DXVECTOR3 vecPos = element.position;
	
	vecEye.x = element.position.x;	
	vecEye.y -= g_PRBaseMatrix._42; // 로컬에서 계산되기 때문에 카메라를 로컬로 보냄
	vecEye.z -= g_PRBaseMatrix._43;

	D3DXMatrixLookAtLH( &g_PRWorldmatrix, &vecEye, &vecPos, &vecUp);		
	D3DXMatrixInverse( &g_PRWorldmatrix, NULL, &g_PRWorldmatrix);
	
	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );	
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix );

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}


void __stdcall _XGetViewMatrixCallback_LockY( _XParticleElement& element )
{
	D3DXVECTOR3 vecEye = g_PRCamposition;
	D3DXVECTOR3 vecPos = element.position;

	vecEye.x -= g_PRBaseMatrix._41; // 로컬에서 계산되기 때문에 카메라를 로컬로 보냄
	vecEye.y = element.position.y;
	vecEye.z -= g_PRBaseMatrix._43;

	//vecEye.y = vecPos.y = 0.0f;		
	D3DXMatrixLookAtLH( &g_PRWorldmatrix, &vecEye, &vecPos, &vecUp);		
	D3DXMatrixInverse(&g_PRWorldmatrix, NULL, &g_PRWorldmatrix);

	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix );

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_LockZ( _XParticleElement& element )
{
	D3DXVECTOR3 vecEye = g_PRCamposition;
	D3DXVECTOR3 vecPos = element.position;

	vecEye.x -= g_PRBaseMatrix._41; // 로컬에서 계산되기 때문에 카메라를 로컬로 보냄
	vecEye.y -= g_PRBaseMatrix._42;		
	vecEye.z = element.position.z;
	
	D3DXMatrixLookAtLH( &g_PRWorldmatrix, &vecEye, &vecPos, &vecUp);		
	D3DXMatrixInverse( &g_PRWorldmatrix, NULL, &g_PRWorldmatrix);

	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix );

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_Direction( _XParticleElement& element )
{
	D3DXVECTOR3 vecEye = element.position + (element.velocity*100.0f);

	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixLookAtLH( &g_PRWorldmatrix, &vecEye, &element.position, &vecUp);
	D3DXMatrixInverse( &g_PRWorldmatrix, NULL, &g_PRWorldmatrix);
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &g_PRRotmat, &g_PRWorldmatrix );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &g_PRWorldmatrix, &g_PRBaseMatrix );
		
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void __stdcall _XGetViewMatrixCallback_ArbitrarilyPoint( _XParticleElement& element )
{
	D3DXMatrixLookAtLH( &g_PRWorldmatrix, &g_PRCamposition, &element.position, &vecUp);
	D3DXMatrixInverse( &g_PRWorldmatrix, NULL, &g_PRWorldmatrix);
	D3DXMATRIX scalematrix;
	D3DXMatrixScaling( &scalematrix, element.size.x,element.size.y,element.size.z );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &scalematrix, &g_PRWorldmatrix );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &g_PRRotmat, &g_PRWorldmatrix );
	D3DXMatrixMultiply( &g_PRWorldmatrix, &g_PRWorldmatrix, &g_PRBaseMatrix );
	
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &element.position, &g_PRBaseMatrix );
	g_PRWorldmatrix._41=newpos.x;
	g_PRWorldmatrix._42=newpos.y;
	g_PRWorldmatrix._43=newpos.z;
}

void _XParticleList::Render( void )
{
	if( !m_pInstanceData ) return;

	if( m_pInstanceData->RenderStyleCount > 1 )
	{
		AccumulateRenderStyleChangeTime += g_fElapsedFrameMilisecondTime;
		if( AccumulateRenderStyleChangeTime >= m_pInstanceData->RenderStyleChangeTime )
		{
			AccumulateRenderStyleChangeTime = 0;//fmod( AccumulateRenderStyleChangeTime, m_pInstanceData->RenderStyleChangeTime );
			CurrentRenderStyleIndex++;
		}
		if( CurrentRenderStyleIndex >= m_pInstanceData->RenderStyleCount ) CurrentRenderStyleIndex = 0;

		g_SetTextureFailed = FALSE;
		m_pInstanceData->m_pRenderStyleManager->SetRenderStyle( m_pInstanceData->m_pTextureManager, m_pInstanceData->RenderStyleIndexArray[CurrentRenderStyleIndex] );
		if( g_SetTextureFailed )
			return;
	}
	else
	{
		g_SetTextureFailed = FALSE;
		m_pInstanceData->m_pRenderStyleManager->SetRenderStyle( m_pInstanceData->m_pTextureManager, m_pInstanceData->RenderStyleIndexArray[0] );
		if( g_SetTextureFailed )
			return;
	}
	

	_XParticleMeshObject* pParticleMeshObject = NULL;
	
	// Model particle & animation model particle...
	if( m_pInstanceData->RenderingType >= _XParticleRT_Model && m_pInstanceData->RenderingType <= _XParticleRT_OrgTextureAnimationModel ) 
	{
		if( m_pInstanceData->m_pParticleMeshList )
			pParticleMeshObject = &m_pInstanceData->m_pParticleMeshList[m_pInstanceData->MeshModelIndex];
		else return;
	}
	else 
	{
		gpDev->SetFVF( _XDEF_FVF_XPARTICLEVERTEX );
	}
		
	g_PRCamposition		 = g_LodTerrain.m_3PCamera.m_CameraPosition;
	g_PRBaseMatrix		 = m_ParentMatrix; // setted update function...
		
	D3DXMatrixIdentity( &g_PROrgViewmatrix );
	_XGetViewMatrixCallback	pGetViewMatrixCallback = NULL;

	switch( m_pInstanceData->ViewtTargetType )
	{	
	case _XParticleVT_None		: 
		{			
			if( m_pInstanceData->RotationFactor.x == 0.0f && m_pInstanceData->RotationFactor.y == 0.0f && m_pInstanceData->RotationFactor.z == 0.0f )
			{
				g_PRRotmat = m_pInstanceData->BaseRotateMatrix;
			}
			else
			{
				RotateAnimationFactor.x += m_pInstanceData->RotationFactor.x * _XParticleActionBase::dt;
				RotateAnimationFactor.y += m_pInstanceData->RotationFactor.y * _XParticleActionBase::dt;
				RotateAnimationFactor.z += m_pInstanceData->RotationFactor.z * _XParticleActionBase::dt;
				
				if( RotateAnimationFactor.x >=  6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x,  6.283185f );
				else if( RotateAnimationFactor.x <= -6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x, -6.283185f );
				if( RotateAnimationFactor.y >=  6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y,  6.283185f );
				else if( RotateAnimationFactor.y <= -6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y, -6.283185f );
				if( RotateAnimationFactor.z >=  6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z,  6.283185f );
				else if( RotateAnimationFactor.z <= -6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z, -6.283185f );
				
				D3DXMATRIX rotmat;
				D3DXMatrixRotationYawPitchRoll( &rotmat, RotateAnimationFactor.x, RotateAnimationFactor.y, RotateAnimationFactor.z );
				D3DXMatrixMultiply( &g_PRRotmat, &rotmat, &m_pInstanceData->BaseRotateMatrix );
			}
			pGetViewMatrixCallback = _XGetViewMatrixCallback_Fixed;
		}
		break;
	case _XParticleVT_Camera	: 		
		{	
			g_PROrgViewmatrix = g_LodTerrain.m_3PCamera.GetViewMatrix();
			D3DXMatrixInverse( &g_PROrgViewmatrix, NULL, &g_PROrgViewmatrix);

			if( m_pInstanceData->RotationFactor.x == 0.0f && m_pInstanceData->RotationFactor.y == 0.0f && m_pInstanceData->RotationFactor.z == 0.0f )
			{
				g_PRRotmat = m_pInstanceData->BaseRotateMatrix;
			}
			else
			{
				RotateAnimationFactor.x += m_pInstanceData->RotationFactor.x * _XParticleActionBase::dt;
				RotateAnimationFactor.y += m_pInstanceData->RotationFactor.y * _XParticleActionBase::dt;
				RotateAnimationFactor.z += m_pInstanceData->RotationFactor.z * _XParticleActionBase::dt;
				
				if( RotateAnimationFactor.x >=  6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x,  6.283185f );
				else if( RotateAnimationFactor.x <= -6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x, -6.283185f );
				if( RotateAnimationFactor.y >=  6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y,  6.283185f );
				else if( RotateAnimationFactor.y <= -6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y, -6.283185f );
				if( RotateAnimationFactor.z >=  6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z,  6.283185f );
				else if( RotateAnimationFactor.z <= -6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z, -6.283185f );
				
				D3DXMATRIX rotmat;
				D3DXMatrixRotationYawPitchRoll( &rotmat, RotateAnimationFactor.x, RotateAnimationFactor.y, RotateAnimationFactor.z );
				D3DXMatrixMultiply( &g_PRRotmat, &rotmat, &m_pInstanceData->BaseRotateMatrix );
			}
			
			pGetViewMatrixCallback = _XGetViewMatrixCallback_Camera;
		}
		break;
	case _XParticleVT_LockX		: 
		{			
			pGetViewMatrixCallback = _XGetViewMatrixCallback_LockX;
		}
		break;
	case _XParticleVT_LockY		:
		{				
			pGetViewMatrixCallback = _XGetViewMatrixCallback_LockY;
		}
		break;
	case _XParticleVT_LockZ		:
		{			
			pGetViewMatrixCallback = _XGetViewMatrixCallback_LockZ;
		}
		break;
	case _XParticleVT_Direction	:
		{
			if( m_pInstanceData->RotationFactor.x == 0.0f && 
				m_pInstanceData->RotationFactor.y == 0.0f && 
				m_pInstanceData->RotationFactor.z == 0.0f )
			{
				g_PRRotmat = m_pInstanceData->BaseRotateMatrix;
			}
			else
			{
				RotateAnimationFactor.x += m_pInstanceData->RotationFactor.x * _XParticleActionBase::dt;
				RotateAnimationFactor.y += m_pInstanceData->RotationFactor.y * _XParticleActionBase::dt;
				RotateAnimationFactor.z += m_pInstanceData->RotationFactor.z * _XParticleActionBase::dt;
				
				if( RotateAnimationFactor.x >=  6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x,  6.283185f );
				else if( RotateAnimationFactor.x <= -6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x, -6.283185f );
				if( RotateAnimationFactor.y >=  6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y,  6.283185f );
				else if( RotateAnimationFactor.y <= -6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y, -6.283185f );
				if( RotateAnimationFactor.z >=  6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z,  6.283185f );
				else if( RotateAnimationFactor.z <= -6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z, -6.283185f );
				
				D3DXMATRIX rotmat;
				D3DXMatrixRotationYawPitchRoll( &rotmat, RotateAnimationFactor.x, RotateAnimationFactor.y, RotateAnimationFactor.z );
				D3DXMatrixMultiply( &g_PRRotmat, &rotmat, &m_pInstanceData->BaseRotateMatrix );
			}

			pGetViewMatrixCallback = _XGetViewMatrixCallback_Direction;
		}	
		break;
	case _XParticleVT_ArbitrarilyPoint	:
		{
			if( m_pInstanceData->RotationFactor.x == 0.0f && 
				m_pInstanceData->RotationFactor.y == 0.0f && 
				m_pInstanceData->RotationFactor.z == 0.0f )
			{
				g_PRRotmat = m_pInstanceData->BaseRotateMatrix;
			}
			else
			{
				RotateAnimationFactor.x += m_pInstanceData->RotationFactor.x * _XParticleActionBase::dt;
				RotateAnimationFactor.y += m_pInstanceData->RotationFactor.y * _XParticleActionBase::dt;
				RotateAnimationFactor.z += m_pInstanceData->RotationFactor.z * _XParticleActionBase::dt;
				
				if( RotateAnimationFactor.x >=  6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x,  6.283185f );
				else if( RotateAnimationFactor.x <= -6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x, -6.283185f );
				if( RotateAnimationFactor.y >=  6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y,  6.283185f );
				else if( RotateAnimationFactor.y <= -6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y, -6.283185f );
				if( RotateAnimationFactor.z >=  6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z,  6.283185f );
				else if( RotateAnimationFactor.z <= -6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z, -6.283185f );
				
				D3DXMATRIX rotmat;
				D3DXMatrixRotationYawPitchRoll( &rotmat, RotateAnimationFactor.x, RotateAnimationFactor.y, RotateAnimationFactor.z );
				D3DXMatrixMultiply( &g_PRRotmat, &rotmat, &m_pInstanceData->BaseRotateMatrix );
			}

			g_PRCamposition = m_pInstanceData->ArbitrarilyPoint;
			pGetViewMatrixCallback = _XGetViewMatrixCallback_ArbitrarilyPoint;
		}
		break;
	case _XParticleVT_FixedAndXRollAnimation :
	case _XParticleVT_FixedAndYRollAnimation :
	case _XParticleVT_FixedAndZRollAnimation :
		{	
			if( m_pInstanceData->RotationFactor.x == 0.0f && m_pInstanceData->RotationFactor.y == 0.0f && m_pInstanceData->RotationFactor.z == 0.0f )
			{
				g_PRRotmat = m_pInstanceData->BaseRotateMatrix;
			}
			else
			{
				RotateAnimationFactor.x += m_pInstanceData->RotationFactor.x * _XParticleActionBase::dt;
				RotateAnimationFactor.y += m_pInstanceData->RotationFactor.y * _XParticleActionBase::dt;
				RotateAnimationFactor.z += m_pInstanceData->RotationFactor.z * _XParticleActionBase::dt;
				
				if( RotateAnimationFactor.x >=  6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x,  6.283185f );
				else if( RotateAnimationFactor.x <= -6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x, -6.283185f );
				if( RotateAnimationFactor.y >=  6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y,  6.283185f );
				else if( RotateAnimationFactor.y <= -6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y, -6.283185f );
				if( RotateAnimationFactor.z >=  6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z,  6.283185f );
				else if( RotateAnimationFactor.z <= -6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z, -6.283185f );
				
				D3DXMATRIX rotmat;
				D3DXMatrixRotationYawPitchRoll( &rotmat, RotateAnimationFactor.x, RotateAnimationFactor.y, RotateAnimationFactor.z );
				D3DXMatrixMultiply( &g_PRRotmat, &rotmat, &m_pInstanceData->BaseRotateMatrix );
			}
			
			if( m_pInstanceData->ViewtTargetType == _XParticleVT_FixedAndXRollAnimation )
				pGetViewMatrixCallback = _XGetViewMatrixCallback_FixedAndXRollAnimation;
			else if( m_pInstanceData->ViewtTargetType == _XParticleVT_FixedAndYRollAnimation )
				pGetViewMatrixCallback = _XGetViewMatrixCallback_FixedAndYRollAnimation;
			else //if( m_pInstanceData->ViewtTargetType == _XParticleVT_FixedAndZRollAnimation )
				pGetViewMatrixCallback = _XGetViewMatrixCallback_FixedAndZRollAnimation;
		}
		break;
	case _XParticleVT_CameraAndRollAnimation :
		{
			g_PROrgViewmatrix = g_LodTerrain.m_3PCamera.GetViewMatrix();
			D3DXMatrixInverse( &g_PROrgViewmatrix, NULL, &g_PROrgViewmatrix);
			
			if( m_pInstanceData->RotationFactor.x == 0.0f && m_pInstanceData->RotationFactor.y == 0.0f && m_pInstanceData->RotationFactor.z == 0.0f )
			{
				g_PRRotmat = m_pInstanceData->BaseRotateMatrix;
			}
			else
			{
				RotateAnimationFactor.x += m_pInstanceData->RotationFactor.x * _XParticleActionBase::dt;
				RotateAnimationFactor.y += m_pInstanceData->RotationFactor.y * _XParticleActionBase::dt;
				RotateAnimationFactor.z += m_pInstanceData->RotationFactor.z * _XParticleActionBase::dt;
				
				if( RotateAnimationFactor.x >=  6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x,  6.283185f );
				else if( RotateAnimationFactor.x <= -6.283185f ) RotateAnimationFactor.x = fmod( RotateAnimationFactor.x, -6.283185f );
				if( RotateAnimationFactor.y >=  6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y,  6.283185f );
				else if( RotateAnimationFactor.y <= -6.283185f ) RotateAnimationFactor.y = fmod( RotateAnimationFactor.y, -6.283185f );
				if( RotateAnimationFactor.z >=  6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z,  6.283185f );
				else if( RotateAnimationFactor.z <= -6.283185f ) RotateAnimationFactor.z = fmod( RotateAnimationFactor.z, -6.283185f );
				
				D3DXMATRIX rotmat;
				D3DXMatrixRotationYawPitchRoll( &rotmat, RotateAnimationFactor.x, RotateAnimationFactor.y, RotateAnimationFactor.z );
				D3DXMatrixMultiply( &g_PRRotmat, &rotmat, &m_pInstanceData->BaseRotateMatrix );
			}
			
			pGetViewMatrixCallback = _XGetViewMatrixCallback_CameraAndRollAnimation;			
		}
		break;
	}	

	switch( m_pInstanceData->RenderingType )
	{
	
	case _XParticleRT_Triangle		 :
		{
			g_ParticleRenderManager.Lock();
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
				
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );
				
				g_ParticleRenderManager.SetTriangleBuffer((DWORD)ParticleElement.color, g_PRWorldmatrix);
			}
			g_ParticleRenderManager.Unlock();
			g_ParticleRenderManager.Render();
		}
		break;

	case _XParticleRT_NumericBillboard	:
		{
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
									
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );

				g_ParticleRenderManager.Lock();
				g_ParticleRenderManager.SetNumericBuffer(ParticleElement.color, g_PRWorldmatrix);
				g_ParticleRenderManager.Unlock();

				g_ParticleRenderManager.BeginNumeric();

				for( int bi = 0; bi < m_pInstanceData->m_NumericDataLength; bi++ )
				{						
					if( m_pInstanceData->m_NumericData[m_pInstanceData->m_StartNumericDataSlot+bi] != 255 )
					{
						m_pInstanceData->m_pRenderStyleManager->SetRenderStyle( m_pInstanceData->m_pTextureManager, 
										 m_pInstanceData->RenderStyleIndexArray[m_pInstanceData->m_NumericData[m_pInstanceData->m_StartNumericDataSlot+bi]] );						
						
						g_ParticleRenderManager.RenderNumeric((m_pInstanceData->m_StartNumericDataSlot+bi)*6);
					}
				}
			}
		}
		break;
	
	case _XParticleRT_Billboard		 :
		{
			g_ParticleRenderManager.Lock();
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
									
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );
 
				g_ParticleRenderManager.SetBuffer(4, 2, (DWORD)ParticleElement.color, g_PRWorldmatrix);
			}
			g_ParticleRenderManager.Unlock();
			g_ParticleRenderManager.Render();
		}
		break;
	case _XParticleRT_XZCrossBillboard :
		{
			g_ParticleRenderManager.Lock();
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
										
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );

				g_ParticleRenderManager.SetBuffer(8, 4, (DWORD)ParticleElement.color, g_PRWorldmatrix);
			}
			g_ParticleRenderManager.Unlock();
			g_ParticleRenderManager.Render();
		}
		break;
	case _XParticleRT_XYZCrossBillboard :
		{
			g_ParticleRenderManager.Lock();
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
									
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );

				g_ParticleRenderManager.SetBuffer(12, 6, (DWORD)ParticleElement.color, g_PRWorldmatrix);
			}
			g_ParticleRenderManager.Unlock();
			g_ParticleRenderManager.Render();
		}
		break;
	case _XParticleRT_Model			 :
		{
			if(pParticleMeshObject)
			{
				sldef_XParticleElement::iterator it;
				for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
				{		
					_XParticleElement &ParticleElement = (*(*it));
						
					if( pGetViewMatrixCallback )
						pGetViewMatrixCallback( ParticleElement );
																													
					gpDev->SetRenderState(D3DRS_TEXTUREFACTOR, ParticleElement.color);
					pParticleMeshObject->m_pMeshContainer->RenderNoTexture( g_PRWorldmatrix );
				}
			}			  
		}
		break;
	case _XParticleRT_AnimationModel :
		if(pParticleMeshObject)
		{
			if( pParticleMeshObject->m_pModelDescriptor )
			{
				pParticleMeshObject->m_pModelDescriptor->Advance_NoCheckEvent( ParticleLifeTime );
				pParticleMeshObject->m_pModelDescriptor->Animation();
			}

			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
					
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );
									
				gpDev->SetRenderState(D3DRS_TEXTUREFACTOR, ParticleElement.color);
				if( pParticleMeshObject->m_pModelDescriptor )
				{
// by mahwang
					pParticleMeshObject->m_pModelDescriptor->RenderNoTexture( g_PRWorldmatrix );
				}						
// by mahwang
				if( pParticleMeshObject->m_pKeyframeDescriptor )
				{
					pParticleMeshObject->m_pKeyframeDescriptor->Animation(ParticleLifeTime, g_PRWorldmatrix);
//						pParticleMeshObject->m_pKeyframeDescriptor->SetWorldMatrix(g_PRWorldmatrix);
					pParticleMeshObject->m_pKeyframeDescriptor->RenderNoTexture( );
				}						
			}
		}			  
		break;
	case _XParticleRT_OrgTextureModel :
		if(pParticleMeshObject)
		{
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
				
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );
				
				gpDev->SetRenderState(D3DRS_TEXTUREFACTOR, ParticleElement.color);
				pParticleMeshObject->m_pMeshContainer->Render( g_PRWorldmatrix );
			}
		}	
		break;
	case _XParticleRT_OrgTextureAnimationModel :
		if(pParticleMeshObject)
		{
			if( pParticleMeshObject->m_pModelDescriptor )
			{
				pParticleMeshObject->m_pModelDescriptor->Advance_NoCheckEvent( ParticleLifeTime );
				pParticleMeshObject->m_pModelDescriptor->Animation();
			}
			
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
				
				if( pGetViewMatrixCallback )
					pGetViewMatrixCallback( ParticleElement );
				
				gpDev->SetRenderState(D3DRS_TEXTUREFACTOR, ParticleElement.color);
				if( pParticleMeshObject->m_pModelDescriptor )
				{
					// by mahwang
					pParticleMeshObject->m_pModelDescriptor->Render( g_PRWorldmatrix );
				}						
				// by mahwang
				if( pParticleMeshObject->m_pKeyframeDescriptor )
				{
					pParticleMeshObject->m_pKeyframeDescriptor->Animation(ParticleLifeTime, g_PRWorldmatrix);
					//						pParticleMeshObject->m_pKeyframeDescriptor->SetWorldMatrix(g_PRWorldmatrix);
					pParticleMeshObject->m_pKeyframeDescriptor->Render();
				}						
			}
		}			  
		break;
	case _XParticleRT_TerrainDecal :
		{
			gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );

			D3DXVECTOR3 worldpos;
			gpDev->SetFVF( _XFVF_DECALVERTEX );
			
			if(m_pDecalObject)
			{
				sldef_XParticleElement::iterator it;
				for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
				{		
					_XParticleElement &ParticleElement = (*(*it));

					if( pGetViewMatrixCallback )
						pGetViewMatrixCallback( ParticleElement );
																
					gpDev->SetRenderState(D3DRS_TEXTUREFACTOR, ParticleElement.color);
			
					D3DXVec3TransformCoord(&worldpos, &ParticleElement.position, &g_PRWorldmatrix );

					m_pDecalObject->GenerateDecal( worldpos, g_DecalUpVector, g_DecalTangentVector,
												   ParticleElement.size.x, ParticleElement.size.z, 5.0f );
					m_pDecalObject->Render();
				}
			}
		}
		break;
	case _XParticleRT_LineEffect :
		{
			gpDev->SetTransform( D3DTS_WORLD, &g_PRBaseMatrix );
			
			sldef_XParticleElement::iterator it;
			for (it = m_slParticleElement.begin(); it != m_slParticleElement.end(); ++ it) 
			{		
				_XParticleElement &ParticleElement = (*(*it));
					
				//g_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, ParticleElement.color);
				g_LineEffectFactory.Process( ParticleElement.pLineData );
				g_LineEffectFactory.SetDiffuseColor(ParticleElement.color);
				g_LineEffectFactory.Render();
			}
		}
		break;
	}
}

BOOL _XParticleList::Load( FILE* fileptr )
{
	if( !fileptr ) return FALSE;

	if( !m_pInstanceData ) return FALSE;

	// Load particle list name
	int length;
	fread( &length, sizeof(int), 1, fileptr );
	if( length > 0 )
	{
		// skip particle list name
		fseek( fileptr, sizeof(TCHAR)*length, SEEK_CUR );
	}
	

	// skip enable flag
	fseek( fileptr, sizeof(BOOL), SEEK_CUR );
	fread( &m_pInstanceData->maxtracktime, sizeof(FLOAT), 1, fileptr );

	fread( &m_pInstanceData->lifetimetype, sizeof(_XParticleLifeTimeType), 1, fileptr );
	fread( &m_pInstanceData->limittime, sizeof(FLOAT), 1, fileptr );	
	
	fread( &m_pInstanceData->maxparticles, sizeof(int), 1, fileptr );
	
	fread( &m_pInstanceData->vertexB_tracks, sizeof(bool), 1, fileptr );
	fread( &m_pInstanceData->Alpha, sizeof(FLOAT), 1, fileptr );
	fread( &m_pInstanceData->Age, sizeof(FLOAT), 1, fileptr );
	fread( &m_pInstanceData->AgeSigma, sizeof(FLOAT), 1, fileptr );

	// Render style...
	fread( &m_pInstanceData->RenderStyleChangeTime, sizeof(FLOAT), 1, fileptr );
	int renderstyleindexcount = 1;
	fread( &renderstyleindexcount, sizeof(int), 1, fileptr );
	m_pInstanceData->CreateRenderStyleArray( renderstyleindexcount );
	fread( m_pInstanceData->RenderStyleIndexArray, sizeof(int) * renderstyleindexcount, 1, fileptr );	
	//Skip render style name
	fseek( fileptr, sizeof(TCHAR) * 32 * renderstyleindexcount, SEEK_CUR );
		
	fread( &m_pInstanceData->BipedLinkType, sizeof(_XParticleBipedLinkType), 1, fileptr );
	fread( &m_pInstanceData->LimitBipedRotation, sizeof(BOOL), 1, fileptr );
	fread( &m_pInstanceData->EmitterBipedLinkType, sizeof(_XParticleBipedLinkType), 1, fileptr );
	fread( &m_pInstanceData->LinkSubTarget, sizeof(DWORD), 1, fileptr );

	fread( &m_pInstanceData->MeshModelIndex, sizeof(int), 1, fileptr );
	fread( &m_pInstanceData->MeshModelName, sizeof(TCHAR)*32, 1, fileptr );
	fread( &m_pInstanceData->ViewtTargetType, sizeof(_XParticleViewTargetType), 1, fileptr );
	fread( &m_pInstanceData->RenderingType, sizeof(_XParticleRenderingType), 1, fileptr );	
	m_pInstanceData->RebuildRenderingObject( m_pInstanceData->RenderingType );

	fread( &m_pInstanceData->BaseRotateAngle, sizeof(D3DXVECTOR3), 1, fileptr );
	m_pInstanceData->SetBaseRotationAngle( m_pInstanceData->BaseRotateAngle.x, m_pInstanceData->BaseRotateAngle.y, m_pInstanceData->BaseRotateAngle.z );

	fread( &m_pInstanceData->RotationFactor, sizeof(D3DXVECTOR3), 1, fileptr );	
	//RotateAnimationFactor.x = RotateAnimationFactor.y = RotateAnimationFactor.z = 0.0f;

	fread( &m_pInstanceData->ArbitrarilyPoint, sizeof(D3DXVECTOR3), 1, fileptr );		

	m_pInstanceData->Size.Load( fileptr );
	m_pInstanceData->Velocity.Load( fileptr );
	m_pInstanceData->VertexB.Load( fileptr );
	m_pInstanceData->Color.Load( fileptr );

	m_pInstanceData->actionlist.disposeList();
	int actioncount = 0;
	fread( &actioncount, sizeof(int), 1, fileptr );

	_XParticleActionBase* pActionItem = NULL;
	_XParticleActionType _actiontype;

	for( int i = 0 ; i < actioncount; i++ )
	{
		if( !_XParticleActionBase::LoadActionType( fileptr, _actiontype ) ) return FALSE;

		pActionItem = _XParticleActionBase::CreateAction( _actiontype );
		if( pActionItem )
		{
			if( !pActionItem->Load( fileptr ) )
			{
				delete pActionItem;
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		
		if( pActionItem->enable )
		{
			m_pInstanceData->actionlist.resetList();
			m_pInstanceData->actionlist.insertItem( pActionItem );
		}
		else
		{
			delete pActionItem;
		}
	}
		
	return TRUE;
}

_XParticleList* _XParticleList::MakeInstance( const D3DXMATRIX* pParentMatrix )
{

#ifdef _XDEF_MODELTHREADLOADING
	_XParticleList* pNewParticleList = g_ParticleListPool.New();
	if( pNewParticleList )
	{
		pNewParticleList->Initialize( this->m_pInstanceData );
	}
#else
	_XParticleList* pNewParticleList = new _XParticleList( this->m_pInstanceData );
#endif

	if( pParentMatrix )
	{
		pNewParticleList->m_ParentMatrix = *pParentMatrix;		
	}

	return  pNewParticleList;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// _XParticleGroupItem

HRESULT _XParticleGroupItem::RestoreDeviceObjects( void )
{
	if( listEmpty() ) return S_OK; 
		
	_XParticleList* pParticleList;
	HRESULT ret;

	resetList();
	do 
	{
		pParticleList = (_XParticleList*)currentItem();

		if( !pParticleList )
		{
			return E_FAIL;
		}

		ret = pParticleList->RestoreDeviceObjects();

		if( S_OK != ret )
		{
			return ret;
		}

		nextItem();
	} while( !atHeadOfList() );

	 return S_OK;
}

void _XParticleGroupItem::InvalidateDeviceObjects( void )
{
	if( listEmpty() ) return; 
	
	_XParticleList* pParticleList;
		
	resetList();
	do 
	{
		pParticleList = (_XParticleList*)currentItem();
		
		if( !pParticleList )
		{
			return;
		}
		
		pParticleList->InvalidateDeviceObjects();		
		
		nextItem();
	} while( !atHeadOfList() );	
}

BOOL _XParticleGroupItem::InsertParticle( _XParticleList* pNewParticle )
{
	resetList();
	if( !insertItem( pNewParticle ) )
	{
		return FALSE;
	}
	return TRUE;
}

void _XParticleGroupItem::Update( _XParticleSendParameter* pParameter )
{
	if( listEmpty() ) return; 
	
	_XParticleList* pParticleList;
	
	resetList();
	do 
	{
		pParticleList = (_XParticleList*)currentItem();
		
		if( !pParticleList )
		{
			return;
		}
		
		if( pParticleList->ParticleLifeTime <= -1.0f )
		{
			pParticleList->ParticleLifeTime = 0.0001f;
		}
		else
		{
			pParticleList->ParticleLifeTime += _XParticleActionBase::dt;
		}

		if( pParticleList->m_pInstanceData->lifetimetype == _XParticleLT_LimitTime )
		{
			if( pParticleList->ParticleLifeTime >= pParticleList->m_pInstanceData->limittime )
			{
				pParticleList->ParticleLifeTime = -1.0f;
				pParticleList->DeleteAllParticles();
			}
		}
		else if( pParticleList->m_pInstanceData->lifetimetype == _XParticleLT_OneTime )
		{
			if( pParticleList->ParticleLifeTime >= pParticleList->m_pInstanceData->maxtracktime )
			{
				pParticleList->ParticleLifeTime = -1.0f;
				pParticleList->DeleteAllParticles();
			}
		}

		if( pParameter->pItemPosition )
		{
			memcpy( pParticleList->m_LinkPosition, pParameter->pItemPosition, sizeof(D3DXVECTOR3) * 3 );
		}

		pParticleList->Update( pParameter );		
		
		nextItem();
	} while( !atHeadOfList() );	
}

void _XParticleGroupItem::Render( void )
{
	if( listEmpty() ) return; 
	
	_XParticleList* pParticleList;
	
	resetList();
	do 
	{
		pParticleList = (_XParticleList*)currentItem();
		
		if( !pParticleList )
		{
			return;
		}
		
		pParticleList->Render();
		
		nextItem();
	} while( !atHeadOfList() );	
}

BOOL _XParticleGroupItem::Load( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	
	// Load particle group name
	int length;
	fread( &length, sizeof(int), 1, fileptr );
	if( length > 0 )
	{
		// Skip particle groupname...
		if( fseek( fileptr, sizeof(TCHAR)*length, SEEK_CUR ) != 0 )
		{
			return FALSE;
		}
	}
	
	if( fread( &m_ID, sizeof(int), 1, fileptr ) < 1 )
	{
		_XFatalError( "Can't read particle group ID" );
		return FALSE;
	}

	// Skip enable flag
	if( fseek( fileptr, sizeof(BOOL), SEEK_CUR ) != 0 )
	{
		return FALSE;
	}

	if( fread( &m_MaxInstanceCount, sizeof(int), 1, fileptr ) < 1 )
	{
		return FALSE;
	}

	int particlecount = 0;
	fread( &particlecount, sizeof(int), 1, fileptr );

	disposeList();

	_XParticleList* pParticleList;

	for( int i = 0 ; i < particlecount; i++ )
	{
		pParticleList = new _XParticleList;

		if( !pParticleList )
		{
			return FALSE;
		}		

		pParticleList->m_pInstanceData	 = new _XParticleList::_XPInstanceData;
		pParticleList->m_InstanceCreated = TRUE;

		if( !pParticleList->Load( fileptr ) )
		{
			delete pParticleList;
			return FALSE;
		}

		if( !InsertParticle( pParticleList ) )
		{
			delete pParticleList;
			return FALSE;
		}
	}
	
	return TRUE;
}

void _XParticleGroupItem::Release( void )
{
	if( !listEmpty() )
	{
		resetList();
		while( !listEmpty() )
		{
			_XParticleList* pParticleList = (_XParticleList*)currentItem();
			if(pParticleList)
			{
				pParticleList->Release();
				g_ParticleListPool.Delete( (_XParticleList*)removeItem( pParticleList ) );
			}
		}
	}
}

_XParticleGroupItem* _XParticleGroupItem::MakeInstance( const D3DXMATRIX* pParentMatrix, BOOL nofadeobject )
{
	_XParticleGroupItem* pNewParticleGroupItem = NULL;

#ifdef _XDEF_MODELTHREADLOADING
	pNewParticleGroupItem = g_ParticleGroupItemPool.New();
	if( pNewParticleGroupItem )
	{
		_ASSERT( pNewParticleGroupItem->listEmpty() );

		pNewParticleGroupItem->Initialize();
	}
#else
	pNewParticleGroupItem = new _XParticleGroupItem;
#endif
	
	if( pNewParticleGroupItem )
	{
		pNewParticleGroupItem->m_ID = m_ID;
		pNewParticleGroupItem->m_CreatedTime = g_LocalSystemTime;
		pNewParticleGroupItem->m_NoFadeObject = nofadeobject;

		if( !listEmpty() )
		{		
			_XParticleList* pParticleList;
			_XParticleList* pNewParticleList;
			
			resetList();
			do 
			{
				pParticleList = (_XParticleList*)currentItem();
				
				if( !pParticleList )
				{
					return pNewParticleGroupItem;
				}
				
				pNewParticleList = pParticleList->MakeInstance(pParentMatrix);
				if( pNewParticleList )
				{
					pNewParticleGroupItem->InsertParticle( pNewParticleList );
				}
				
				nextItem();
			} while( !atHeadOfList() );			
		}
	}

	return pNewParticleGroupItem;		
}

void _XParticleGroupItem::StartFade( void )
{
	if( !m_NoFadeObject )
	{
		if( !listEmpty() )
		{		
			_XParticleList* pParticleList;
					
			resetList();
			do 
			{
				pParticleList = (_XParticleList*)currentItem();
				
				if( pParticleList )
				{				
					// Infinity 속성의 Particle list를 Fade mode에 들어가게 하여 파티클이 모두 사라지면 객체를 삭제한다.
					if( pParticleList->m_pInstanceData->lifetimetype != _XParticleLT_LimitTime )
						pParticleList->m_FadeMode = TRUE;
				}
				else
				{
					break;
				}
				
				nextItem();
			} while( !atHeadOfList() );			
		}
	}
}

void _XParticleGroupItem::SetNumericData( int number )
{
	if( !listEmpty() )
	{		
		_XParticleList* pParticleList;
		
		resetList();
		do 
		{
			pParticleList = (_XParticleList*)currentItem();
			
			if( pParticleList )
			{				
				if( pParticleList->m_pInstanceData->RenderingType == _XParticleRT_NumericBillboard )
					pParticleList->m_pInstanceData->SetNumericData( number );
			}
			else
			{
				break;
			}
			
			nextItem();
		} while( !atHeadOfList() );			
	}
}

void _XParticleGroupItem::MovePosition( D3DXMATRIX newmat )
{
	m_vPosition.x = newmat._41;
	m_vPosition.y = newmat._42;
	m_vPosition.z = newmat._43;

	if( !listEmpty() )
	{		
		_XParticleList* pParticleList;
		
		resetList();
		do 
		{
			pParticleList = (_XParticleList*)currentItem();
			
			if( pParticleList )
			{				
				pParticleList->m_ParentMatrix = newmat;
			}
			else
			{
				break;
			}
			
			nextItem();
		} while( !atHeadOfList() );			
	}
}

void _XParticleGroupItem::SetParticleMaxSize( D3DXVECTOR3& size )
{
	/* //test...
	TCHAR str[512];
	sprintf( str, "VES : %d, %5.2f %5.2f %5.2f\n", this->m_ID, size.x, size.y, size.z );
	OutputDebugString(str);	
	if( size.x > 1.0f )
	{
		int aa = 0;
	}
	*/

	if( !listEmpty() )
	{		
		_XParticleList* pParticleList;
		
		resetList();
		do 
		{
			pParticleList = (_XParticleList*)currentItem();
			
			if( pParticleList )
			{	
				pParticleList->m_LimitMaxParticleSize = size.x;

				sldef_XParticleElement::iterator it;									
				for (it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it) 
				{		
					_XParticleElement &ParticleElement = (*(*it));										
					ParticleElement.size = size;
					
					if( pParticleList->m_pDecalObject )
					{
						pParticleList->m_pDecalObject->GenerateDecal(  pParticleList->m_pDecalObject->m_decalCenter,
																	   g_DecalUpVector, g_DecalTangentVector,
																	   size.x, size.z, 0.8f );
					}
				}
			}
			else
			{
				break;
			}
			
			nextItem();
		} while( !atHeadOfList() );			
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// _XParticleGroupManager
_XParticleGroupManager::_XParticleGroupManager()
{
	m_fRemainTime = 0.f;
	m_pParticleMeshList = NULL;
	m_pEmitterPathList = NULL;
	m_ParticleTemplateList = NULL;		
	m_ParticleTemplateCount = 0;
}

_XParticleGroupManager::~_XParticleGroupManager()
{
	Dispose();
}

void _XParticleGroupManager::Dispose( void )
{
	m_TextureManager.DisposeTexture();
	m_RenderStyleManager.Dispose();

	SAFE_DELETE_ARRAY( m_pParticleMeshList);
	SAFE_DELETE_ARRAY( m_pEmitterPathList );
	SAFE_DELETE_ARRAY( m_ParticleTemplateList );
	m_ParticleTemplateCount = 0;
}

HRESULT _XParticleGroupManager::RestoreDeviceObjects( void )
{
	HRESULT ret = S_OK;

	for( int i = 0; i < m_ParticleTemplateCount; i++ )
	{
		ret |= m_ParticleTemplateList[i].RestoreDeviceObjects();
	}	
		
	return ret;
}

void _XParticleGroupManager::InvalidateDeviceObjects( void )
{	
	for( int i = 0; i < m_ParticleTemplateCount; i++ )
	{
		m_ParticleTemplateList[i].InvalidateDeviceObjects();		
	}
}

void _XParticleGroupManager::Update( FLOAT elapsedtime, FLOAT apptime )
{
#define MAX_UPDATE_TIME 0.5f
#define ADD_UPDATE_TIME 0.05f

	if (elapsedtime > 1.f)
	{
		elapsedtime = 0.01f;
	}
	if (m_fRemainTime > ADD_UPDATE_TIME)
	{
		elapsedtime += ADD_UPDATE_TIME;
		m_fRemainTime -= ADD_UPDATE_TIME;
	}
	else if (m_fRemainTime > 0.f)
	{
		elapsedtime += m_fRemainTime;
		m_fRemainTime  = 0.f;
	}

	if (elapsedtime > MAX_UPDATE_TIME)
	{
		m_fRemainTime += elapsedtime - MAX_UPDATE_TIME;
		elapsedtime = MAX_UPDATE_TIME;
	}

	if( gApp->m_bHasFocus )
	{
		if( gnFPS < 10 ) // 프레임율이 떨어지면 고정프레임 처리로 변환
		{
			if( gnFPS <= 6 ) // 프레임율이 떨어지면 고정프레임 처리로 변환
				_XParticleActionBase::dt = 0.12f;
			else
				_XParticleActionBase::dt = 0.09f;
		}
		else
		{
			_XParticleActionBase::dt = elapsedtime;
		}
	}
	else
	{
		_XParticleActionBase::dt = elapsedtime;
	}

	_XParticleActionBase::apptime = apptime;
}

BOOL _XParticleGroupManager::Load( LPTSTR filename )
{		
	if( !filename ) return FALSE;
	
	FILE* fileptr = fopen( filename, "rb" );
	if( !fileptr )
	{
 		_XFatalError( "Can't open %s file", filename );
		return FALSE;
	}
	
	if( !Load(fileptr) ) 
	{
		fclose( fileptr );
		return FALSE;
	}
	
	fclose( fileptr );
	return TRUE;
}


BOOL _XParticleGroupManager::Load( FILE* fileptr )
{
	m_TextureManager.DisposeTexture();
	m_RenderStyleManager.Dispose();

	SAFE_DELETE_ARRAY( m_pParticleMeshList );
	SAFE_DELETE_ARRAY( m_pEmitterPathList  );
	
	SAFE_DELETE_ARRAY( m_ParticleTemplateList );
	m_ParticleTemplateCount = 0;

	if( !fileptr ) return FALSE;

	WORD headerindicator = 0;
	if( fread( &headerindicator, sizeof(WORD), 1, fileptr ) < 1 )
	{
		_XFatalError( "Can't write particle file header" );
		return FALSE;
	}

	if( headerindicator != _XDEF_PARTICLESTRUCTUREFILEHEADER )
	{
		_XFatalError( "ERROR : Unknown type [%d]", headerindicator );
		return FALSE;
	}

	WORD version = 0;
	if( fread( &version, sizeof(WORD), 1, fileptr ) < 1 )
	{
		_XFatalError( "ERROR : Can't read particle file version" );
		return FALSE;
	}

	if( version != _XDEF_PARTICLESTRUCTUREFILEVERSION )
	{
		_XFatalError( "ERROR : Not suppoted this version [0X%x]", version );
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load particle mesh list	

	_XPackageArchive ParticleModelArchive;

	_XSetDefaultPath( _XDEF_DEFAULTPATH );	
	if( !ParticleModelArchive.OpenPackage( _XDEF_VISUALEFFECTMODELARCHIVENAME ) ) return false;

	if( fread( &m_ParticleMeshCount, sizeof(int), 1, fileptr ) < 1 )
	{
		_XFatalError( "ERROR : Can't read particle mesh count" );
		ParticleModelArchive.FinalizePackage();
		return FALSE;
	}
	
	if( m_ParticleMeshCount > 0 )
	{
		m_pParticleMeshList = new _XParticleMeshObject[m_ParticleMeshCount];

		if( !m_pParticleMeshList )
		{
			_XFatalError( "ERROR : _XParticleGroupManager::Load : create the mesh object list" );
			ParticleModelArchive.FinalizePackage();
			return FALSE;
		}
		
		TCHAR filenamebuffer[256];
		
		for( int i = 0; i<m_ParticleMeshCount; i++ )
		{	
			int filenamelength = 0;

			if( fread( &filenamelength, sizeof(int), 1, fileptr ) < 1 )
			{
				_XFatalError( "ERROR : Can't read particle mesh file name length" );
				ParticleModelArchive.FinalizePackage();
				SAFE_DELETE_ARRAY( m_pParticleMeshList );
				m_ParticleMeshCount = 0;
				return FALSE;
			}		
			
			memset( filenamebuffer, 0, sizeof(TCHAR)*256 );
			if( fread( filenamebuffer, sizeof(TCHAR) * filenamelength, 1, fileptr ) < 1 )
			{
				_XFatalError( "ERROR : Can't read particle mesh file name" );
				ParticleModelArchive.FinalizePackage();
				SAFE_DELETE_ARRAY( m_pParticleMeshList );
				m_ParticleMeshCount = 0;
				return FALSE;
			}
			
			FILE* pResourceFilePtr = ParticleModelArchive.GetPackedFile( filenamebuffer );
			if( !pResourceFilePtr ) 
			{
				_XFatalError( "ERROR : Can't find particle mesh file [%s]", filenamebuffer );
				ParticleModelArchive.FinalizePackage();
				SAFE_DELETE_ARRAY( m_pParticleMeshList );
				m_ParticleMeshCount = 0;
				return FALSE;
			}

			m_pParticleMeshList[i].m_pMeshContainer = _XAnimMeshAbstractClass::CreateAnimationMesh();
			if( !m_pParticleMeshList[i].m_pMeshContainer->Load( pResourceFilePtr, &m_TextureManager, TRUE, filenamebuffer ) )
			{
				_XFatalError( "ERROR : Load mesh file [%s]", filenamebuffer );
				ParticleModelArchive.FinalizePackage();
				SAFE_DELETE_ARRAY( m_pParticleMeshList );
				m_ParticleMeshCount = 0;
				return FALSE;
			}			

			// Biped object
			if( fread( &filenamelength, sizeof(int), 1, fileptr ) < 1 )
			{
				_XFatalError( "ERROR : Can't read particle biped mesh file name length" );
				ParticleModelArchive.FinalizePackage();
				SAFE_DELETE_ARRAY( m_pParticleMeshList );
				m_ParticleMeshCount = 0;
				return FALSE;
			}		
			
			if( filenamelength > 0 )
			{
				memset( filenamebuffer, 0, sizeof(TCHAR)*256 );
				if( fread( filenamebuffer, sizeof(TCHAR) * filenamelength, 1, fileptr ) < 1 )
				{
					_XFatalError( "ERROR : Can't read particle biped mesh file name" );
					ParticleModelArchive.FinalizePackage();
					SAFE_DELETE_ARRAY( m_pParticleMeshList );
					m_ParticleMeshCount = 0;
					return FALSE;
				}

				pResourceFilePtr = ParticleModelArchive.GetPackedFile( filenamebuffer );
				if( !pResourceFilePtr ) 
				{
					_XFatalError( "ERROR : Can't find particle biped mesh [%s]", filenamebuffer );
					ParticleModelArchive.FinalizePackage();
					SAFE_DELETE_ARRAY( m_pParticleMeshList );
					m_ParticleMeshCount = 0;
					return FALSE;
				}
								
				SAFE_DELETE( m_pParticleMeshList[i].m_pBipedController );
				m_pParticleMeshList[i].m_pBipedController = new _XBipedController;
				SAFE_DELETE( m_pParticleMeshList[i].m_pModelDescriptor );
				m_pParticleMeshList[i].m_pModelDescriptor = new _XModelDescriptor;
				
				if( m_pParticleMeshList[i].m_pBipedController->Load( pResourceFilePtr, filenamebuffer ) )
				{
					m_pParticleMeshList[i].m_pBipedController->ReserveAnimationStack( 1 );
					m_pParticleMeshList[i].m_pModelDescriptor->ReserveModelStack( 1 );											
					m_pParticleMeshList[i].m_pModelDescriptor->LinkBipedObject( m_pParticleMeshList[i].m_pBipedController );
					m_pParticleMeshList[i].m_pModelDescriptor->SetMeshModel( 0, m_pParticleMeshList[i].m_pMeshContainer );
					m_pParticleMeshList[i].m_pModelDescriptor->PlayAnimation(TRUE);
				}
				else
				{				
					_XFatalError( "ERROR : Load biped mesh file[%s]", filenamebuffer );
					ParticleModelArchive.FinalizePackage();
					SAFE_DELETE_ARRAY( m_pParticleMeshList );
					m_ParticleMeshCount = 0;
					return FALSE;
				}

				int animationkeycount = 0;
				if( fread( &animationkeycount, sizeof(int), 1, fileptr ) < 1 )
				{
					_XFatalError( "ERROR : Can't read particle biped mesh animation count" );
					ParticleModelArchive.FinalizePackage();
					SAFE_DELETE_ARRAY( m_pParticleMeshList );
					m_ParticleMeshCount = 0;
					return FALSE;
				}
				
				for( int j = 0 ; j < animationkeycount; j++ )
				{
					memset( filenamebuffer, 0, sizeof(TCHAR)*256 );
					fread( filenamebuffer, sizeof(TCHAR)*64, 1, fileptr );
				
					pResourceFilePtr = ParticleModelArchive.GetPackedFile( filenamebuffer );
					if( !pResourceFilePtr ) 
					{
						_XFatalError( "ERROR : Can't find particle animation key [%s]", filenamebuffer );
						ParticleModelArchive.FinalizePackage();
						SAFE_DELETE_ARRAY( m_pParticleMeshList );
						m_ParticleMeshCount = 0;
						return FALSE;
					}
					
					if( m_pParticleMeshList[i].m_pBipedController->LoadKey( pResourceFilePtr, filenamebuffer ) )
					{						
						//_XLog( "Loaded [%d:%d] [%s]", i, animationkeycount, filenamebuffer );
					}
					else
					{
						_XFatalError( "ERROR : can't load %s animation key", filenamebuffer );
						ParticleModelArchive.FinalizePackage();
						SAFE_DELETE_ARRAY( m_pParticleMeshList );
						m_ParticleMeshCount = 0;
						return FALSE;
					}
				}			
			}	

			// by mahwang 
			int nKeyframeCount;
			if( fread( &nKeyframeCount, sizeof(int), 1, fileptr ) < 1 )
			{
				_XFatalError( "ERROR : Can't read keyframe count" );
				return FALSE;
			}
			if (nKeyframeCount > 0)
			{
				m_pParticleMeshList[i].m_pKeyframeController = new _XKeyframeController;
				m_pParticleMeshList[i].m_pKeyframeDescriptor = new _XKeyframeDescriptor;
				fread( &m_pParticleMeshList[i].m_pKeyframeController->m_nNodeCount, sizeof(int), 1, fileptr );
				for (int j = 0; j < nKeyframeCount; ++ j)
				{
					fread( &filenamelength, sizeof(int), 1, fileptr );
					fread( filenamebuffer, sizeof(TCHAR) * filenamelength, 1, fileptr );

					pResourceFilePtr = ParticleModelArchive.GetPackedFile( filenamebuffer );
					if( !pResourceFilePtr ) 
					{
						_XFatalError( "ERROR : Can't find particle animation key [%s]", filenamebuffer );
						ParticleModelArchive.FinalizePackage();
						SAFE_DELETE_ARRAY( m_pParticleMeshList );
						m_ParticleMeshCount = 0;
						return FALSE;
					}

					if( m_pParticleMeshList[i].LoadKeyframeKey(pResourceFilePtr,  (LPTSTR)filenamebuffer ) )
					{
					}
					else
					{
						_XFatalError( "ERROR : can't load %s animation key", filenamebuffer );
						ParticleModelArchive.FinalizePackage();
						SAFE_DELETE_ARRAY( m_pParticleMeshList );
						m_ParticleMeshCount = 0;
						return FALSE;
					}
				}
				m_pParticleMeshList[i].m_pKeyframeDescriptor->LinkKeyframeController( m_pParticleMeshList[i].m_pKeyframeController );
				m_pParticleMeshList[i].m_pKeyframeDescriptor->PlayAnimation(TRUE);
				m_pParticleMeshList[i].m_pKeyframeDescriptor->SetMeshModel(m_pParticleMeshList[i].m_pMeshContainer);
			}
		}
	}

	ParticleModelArchive.FinalizePackage();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load emitter path list
	int emitterpathcount = 0;
	if( fread( &emitterpathcount, sizeof(int), 1, fileptr ) < 1 )
	{
		_XFatalError( "ERROR : Can't read the emitter path count" );
		return FALSE;
	}
	
	if( emitterpathcount > 0 )
	{
		m_pEmitterPathList = new _XSplineContainer[ emitterpathcount ];

		if( !m_pEmitterPathList )
		{
			_XFatalError( "ERROR : Can't create the emitter path array" );
			return FALSE; 
		}
				
		for( int i = 0; i < emitterpathcount; i++ )
		{
			if( !m_pEmitterPathList[i].Load( fileptr ) )
			{				
				SAFE_DELETE_ARRAY( m_pEmitterPathList );
				return FALSE;
			}
		}	
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load particle render style list
	if( !m_RenderStyleManager.Load( fileptr ) )
	{
		_XFatalError( "ERROR : Can't read particle render style" );
		return FALSE;
	}
	
	// Load texture in render style

	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );
	//m_TextureManager.m_TextureArchive.EnableVirtualMemory( TRUE );

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_VISUALEFFECT.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_VISUALEFFECT.XP") );
#endif

	if( !m_TextureManager.SetTexturePackage( packagefilename ) )
	{
		return FALSE;
	}

	for( int i = 0; i < m_RenderStyleManager.m_RenderStyleCount; i++ )
	{		
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[0] = 
			m_TextureManager.GetResourceIndex( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[0], _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE );		
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[1] = 
			m_TextureManager.GetResourceIndex( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[1], _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE );		
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[2] = 
			m_TextureManager.GetResourceIndex( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[2], _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE );		
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[3] = 
			m_TextureManager.GetResourceIndex( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[3], _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE );
	}

	if( !m_TextureManager.BuildTextureList() )
	{
		_XFatalError( "ERROR : Can't initialize visual effect texture" );
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load particle list
		
	int particlegroupcount = 0;
	if( fread( &particlegroupcount, sizeof(int), 1, fileptr ) < 1 )
	{
		_XFatalError( "ERROR : Can't read visual effect template count" );
		return FALSE;
	}
	
	m_ParticleTemplateCount = particlegroupcount;
	m_ParticleTemplateList = new _XParticleGroupItem[m_ParticleTemplateCount];

	if( !m_ParticleTemplateList )
	{
		_XFatalError( "ERROR : Can't allocate visual effect template buffer" );
		return FALSE;
	}

	for( i = 0 ; i < particlegroupcount; i++ )
	{		
		if( !m_ParticleTemplateList[i].Load( fileptr ) )
		{
			return FALSE;
		}

		if( !m_ParticleTemplateList[i].listEmpty() )
		{	
			_XParticleList* pParticleList;
			m_ParticleTemplateList[i].resetList();
			do 
			{
				pParticleList = (_XParticleList*)m_ParticleTemplateList[i].currentItem();
				if( !pParticleList ) break;

				pParticleList->m_pInstanceData->m_pParticleMeshList		= m_pParticleMeshList;
				pParticleList->m_pInstanceData->m_pEmitterPathList		= m_pEmitterPathList;
				pParticleList->m_pInstanceData->m_pRenderStyleManager	= &m_RenderStyleManager;
				pParticleList->m_pInstanceData->m_pTextureManager		= &m_TextureManager;
				
				m_ParticleTemplateList[i].nextItem();
			} while( !m_ParticleTemplateList[i].atHeadOfList() );
		}		
	}

	return TRUE;
}

_XParticleGroupItem* _XParticleGroupManager::FindTemplate( int templateid )
{
	if( templateid <= -1 ) return NULL;
	
	for( int i = 0; i < m_ParticleTemplateCount; i++ )
	{
		if( m_ParticleTemplateList[i].m_ID == templateid ) return &m_ParticleTemplateList[i];
	}

	return NULL;
}

void _XParticleGroupManager::FindTemplate( int templateid, int& index )
{
	if( templateid <= -1 )
	{
		index = -1;
		return;
	}
	
	for( int i = 0; i < m_ParticleTemplateCount; i++ )
	{
		if( m_ParticleTemplateList[i].m_ID == templateid ) 
		{
			index = i;
			return;
		}
	}
	
	index = -1;
}

_XParticleGroupItem* _XParticleGroupManager::MakeNewInstance( int templateid, const D3DXMATRIX* pParentMatrix )
{
	_XParticleGroupItem* pEffectTemplate = FindTemplate( templateid );

	if( pEffectTemplate )
	{
		return pEffectTemplate->MakeInstance(pParentMatrix);
	}
	return NULL;
}
 
//////////////////////////////////////////////////////////////////////////////////////////////////
// _XParticleGroupInstanceManager
_XParticleGroupInstanceManager::_XParticleGroupInstanceManager()
{
	m_pTemplateManager = NULL;		
}

_XParticleGroupInstanceManager::~_XParticleGroupInstanceManager()
{
#ifdef _XDEF_MODELTHREADLOADING
	Dispose();
#endif
}
	
BOOL _XParticleGroupInstanceManager::InsertParticleGroup( _XParticleGroupItem* pNewParticleGroup )
{
	if( !pNewParticleGroup ) return FALSE;
	resetList();
	if( !insertItem( pNewParticleGroup ) ) return FALSE;
	return TRUE;
}

#define _XDEF_MAXINSTANCECOUNTONID		5

_XParticleGroupItem* _XParticleGroupInstanceManager::InsertNewInstance( _XParticleGroupManager* pParticleTemplate, 
																	    int templateid, const D3DXMATRIX* pParentMatrix,
																		int* pNumericData, BOOL nofadeobject, FLOAT maxsize )
{	
	if( !pParticleTemplate ) return NULL;
	
	_XParticleGroupItem* pOldEffectInstance = NULL;
	DWORD				 OldEffectInstanceCreateTime = g_LocalSystemTime;

	_XParticleGroupItem* pEffectTemplate = pParticleTemplate->FindTemplate( templateid );
	if( !pEffectTemplate ) return NULL;

	
	int idexactlycount = 0;
	if( !listEmpty() )
	{	
		_XParticleGroupItem* pParticleGroupItem;
		
		resetList();
		do 
		{
			pParticleGroupItem = (_XParticleGroupItem*)currentItem();
			
			if( pParticleGroupItem )
			{
				if( pParticleGroupItem->m_ID == pEffectTemplate->m_ID )
				{
					idexactlycount++;

					if( pParticleGroupItem->m_CreatedTime < OldEffectInstanceCreateTime )
					{
						OldEffectInstanceCreateTime = pParticleGroupItem->m_CreatedTime;
						pOldEffectInstance = pParticleGroupItem;
					}
				}
			}		

			nextItem();

		} while( !atHeadOfList() );
	}

	// Check max instance count
	if( idexactlycount < pEffectTemplate->m_MaxInstanceCount )
	{
		_XParticleGroupItem* pNewInstance = pEffectTemplate->MakeInstance(pParentMatrix,nofadeobject);

		if( pNewInstance )
		{			
			InsertParticleGroup( pNewInstance );
			
			D3DXVECTOR3 size = D3DXVECTOR3(maxsize,maxsize,maxsize);
			pNewInstance->SetParticleMaxSize( size );

			pNewInstance->m_vPosition.x = pParentMatrix->_41;
			pNewInstance->m_vPosition.y = pParentMatrix->_42;
			pNewInstance->m_vPosition.z = pParentMatrix->_43;

			if( pNumericData )
				pNewInstance->SetNumericData( *pNumericData );

			//_XDWINPRINT( "New Instance created : %d, Instance Count:%d", pEffectTemplate->m_ID, idexactlycount );
			return pNewInstance;
		}
	}
	else
	{
		if( pEffectTemplate->m_MaxInstanceCount > 1 )
		{
#ifdef _XDEF_MODELTHREADLOADING
			if( pOldEffectInstance )
			{
				pOldEffectInstance->Release();
				removeItem( pOldEffectInstance );
				g_ParticleGroupItemPool.Delete( pOldEffectInstance );
			}
#else
			if( pOldEffectInstance )
				deleteItem( pOldEffectInstance );
#endif

			_XParticleGroupItem* pNewInstance = pEffectTemplate->MakeInstance(pParentMatrix,nofadeobject);

			if( pNewInstance )
			{
				InsertParticleGroup( pNewInstance );				
				
				D3DXVECTOR3 size = D3DXVECTOR3(maxsize,maxsize,maxsize);
				pNewInstance->SetParticleMaxSize( size );

				pNewInstance->m_vPosition.x = pParentMatrix->_41;
				pNewInstance->m_vPosition.y = pParentMatrix->_42;
				pNewInstance->m_vPosition.z = pParentMatrix->_43;

				if( pNumericData )
					pNewInstance->SetNumericData( *pNumericData );

				//_XDWINPRINT( "New Instance created : %d, Instance Count:%d", pEffectTemplate->m_ID, idexactlycount );
				return pNewInstance;
			}
		}
		else
		{
			return NULL;
		}
	}

		
	return NULL;
}

void _XParticleGroupInstanceManager::Update( _XModelDescriptor* pModelDescryptor,const D3DXMATRIX* pParentMatrix, 
											 const D3DXVECTOR3* pTargetPosition, const D3DXVECTOR3* pItemPosition,
											 _XModelDescriptor* pTargetModelDescryptor, FLOAT additionalheight )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	_XParticleSendParameter particlesendparameter;
	particlesendparameter.pModelDescryptor = pModelDescryptor;
	particlesendparameter.pParentMatrix = pParentMatrix;
	particlesendparameter.pTargetPosition = pTargetPosition;
	particlesendparameter.pItemPosition = pItemPosition;
	particlesendparameter.pTargetModelDescryptor = pTargetModelDescryptor;
	particlesendparameter.pTemplateManager = m_pTemplateManager;
	particlesendparameter.fAdditionalHeight = additionalheight;

	g_LastEventCount = 0;

	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( !pParticleGroupItem )
		{
			return;
		}

		_ASSERT(pParticleGroupItem->m_ID > -1);
		
		BOOL occuredevent = FALSE;
		
		if( !pParticleGroupItem->listEmpty() )
		{		
			_XParticleList* pParticleList;			
			pParticleGroupItem->resetList();

			BOOL donextprocessing;
			
			do 
			{
				donextprocessing = TRUE;
				pParticleList = (_XParticleList*)pParticleGroupItem->currentItem();
				
				if( pParticleList )
				{	
					if( pParticleList->ParticleLifeTime == -1.0f )
					{
						pParticleList->ParticleLifeTime = 0.0001f;
					}

					pParticleList->ParticleLifeTime += _XParticleActionBase::dt;
										
					if( pParticleList->m_pInstanceData->lifetimetype == _XParticleLT_LimitTime )
					{
						if( pParticleList->ParticleLifeTime >= pParticleList->m_pInstanceData->limittime )
						{
							// 제한시간이 지나면 파티클 리스트 객체 삭제

#ifdef _XDEF_MODELTHREADLOADING
							pParticleList->Release();
							pParticleGroupItem->removeItem( pParticleList );							
							g_ParticleListPool.Delete( pParticleList );
#else
							pParticleGroupItem->deleteItem( pParticleList );
#endif

							if( pParticleGroupItem->listEmpty() )
							{
								break;
							}
							else
							{
								donextprocessing = FALSE;
							}
						}
					}
					else if( pParticleList->m_pInstanceData->lifetimetype == _XParticleLT_OneTime )
					{
						 if( pParticleList->ParticleLifeTime >= pParticleList->m_pInstanceData->maxtracktime )
						 {
							 // One time 제한시간이 지나면 파티클 리스트 객체 삭제
#ifdef _XDEF_MODELTHREADLOADING
							 pParticleList->Release();
							 pParticleGroupItem->removeItem( pParticleList );							 
							 g_ParticleListPool.Delete( pParticleList );
#else
							 pParticleGroupItem->deleteItem( pParticleList );
#endif
							
							 if( pParticleGroupItem->listEmpty() )
							 {
								 break;
							 }
							 else
							 {
								 donextprocessing = FALSE;
							 }
						 }
					}
					else // Infinity
					{
						if( pParticleList->m_FadeMode )
						{
							if( pParticleList->m_slParticleElement.empty() )
							{
								// Infinity 상태에서 Fade mode 가 켜지면 Emitter action이 Lock되므로 파티클 개수가 0면 리스트 객체 삭제
#ifdef _XDEF_MODELTHREADLOADING
								pParticleList->Release();
								pParticleGroupItem->removeItem( pParticleList );
								g_ParticleListPool.Delete( pParticleList );
#else
								pParticleGroupItem->deleteItem( pParticleList );
#endif
								
								if( pParticleGroupItem->listEmpty() )
								{
									break;
								}
								else
								{
									donextprocessing = FALSE;
								}
							}							
						}												
					}					
					
					if( donextprocessing )
					{
						if( pItemPosition )
						{
							memcpy( pParticleList->m_LinkPosition, pItemPosition, sizeof(D3DXVECTOR3) * 3 );
						}
						
						pParticleList->Update( &particlesendparameter );

						if( g_LastEventCount > 0 )
						{
							occuredevent = TRUE;
						}
					}
				}
				else
				{
					break;
				}
				
				if( donextprocessing ) pParticleGroupItem->nextItem();
			} while( !pParticleGroupItem->atHeadOfList() );
		}

		if(occuredevent) // call action effect가 발생하면 모든 effect 삭제
		{
			_XParticleList* pParticleList;			
			pParticleGroupItem->resetList();
			
			do 
			{
				pParticleList = (_XParticleList*)pParticleGroupItem->currentItem();

				pParticleList->DeleteAllParticles();				
				pParticleList->m_FadeMode = TRUE;
				pParticleList->m_CreatedParticle = TRUE;

				pParticleGroupItem->nextItem();
			} while( !pParticleGroupItem->atHeadOfList() );
		}

		// Check all empty particle...
		if( pParticleGroupItem->listEmpty() )
		{
#ifdef _XDEF_MODELTHREADLOADING
			pParticleGroupItem->Release();
			removeItem( pParticleGroupItem );
			g_ParticleGroupItemPool.Delete( pParticleGroupItem );
#else
			deleteItem( pParticleGroupItem );
#endif
		}
		else
		{		
			nextItem();
		}

		if( listEmpty() )
			break;

	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::Render( void )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( !pParticleGroupItem )
		{
			return;
		}
		
		pParticleGroupItem->Render();
		
		nextItem();
	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::RenderWorldEffect( void )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( !pParticleGroupItem )
		{
			return;
		}

		FLOAT dx = (pParticleGroupItem->m_vPosition.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		FLOAT dz = (pParticleGroupItem->m_vPosition.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
		
		if( (_XFastDistance(dx, dz) < 100.0f) &&
			g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pParticleGroupItem->m_vPosition, 3.f ) != NOT_VISIBLE )
		{
			pParticleGroupItem->Render();
		}
		
		nextItem();
	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::StartFade( void )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			pParticleGroupItem->StartFade();
		}
		else
		{
			break;
		}
		
		nextItem();
	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::StartFade( int templateid )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			if( pParticleGroupItem->m_ID == templateid )
				pParticleGroupItem->StartFade();
		}
		else
		{
			break;
		}
		
		nextItem();
	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::StartFade_Except( int templateid )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			if( pParticleGroupItem->m_ID != templateid )
				pParticleGroupItem->StartFade();
		}
		else
		{
			break;
		}
		
		nextItem();
	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::DeleteInstance( _XParticleGroupItem* pDeleteParticle )
{
	if( listEmpty() ) return; 
	_XParticleGroupItem* pParticleGroupItem;
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			if( pParticleGroupItem == pDeleteParticle )
			{
#ifdef _XDEF_MODELTHREADLOADING
				pParticleGroupItem->Release();
				removeItem( pParticleGroupItem );
				g_ParticleGroupItemPool.Delete( pParticleGroupItem );
#else
				deleteItem( pParticleGroupItem );
#endif
				if( listEmpty() ) break;
			}
			else
			{
				nextItem();
			}
		}
		else
		{
			break;
		}
		
	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::DeleteInstance( int templateid )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			if( pParticleGroupItem->m_ID == templateid )
			{
#ifdef _XDEF_MODELTHREADLOADING
				pParticleGroupItem->Release();
				removeItem( pParticleGroupItem );				
				g_ParticleGroupItemPool.Delete( pParticleGroupItem );
#else
				deleteItem( pParticleGroupItem );
#endif
				if( listEmpty() ) break;
			}
			else
			{
				nextItem();
			}
		}
		else
		{
			break;
		}
		
	} while( !atHeadOfList() );
}

void _XParticleGroupInstanceManager::DeleteUniqueInstance( int uniqueid )
{
	if( listEmpty() ) return; 
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			if( pParticleGroupItem->m_UniqueID == uniqueid )
			{
#ifdef _XDEF_MODELTHREADLOADING
				pParticleGroupItem->Release();
				removeItem( pParticleGroupItem );				
				g_ParticleGroupItemPool.Delete( pParticleGroupItem );
#else
				deleteItem( pParticleGroupItem );
#endif
				if( listEmpty() ) break;
			}
			else
			{
				nextItem();
			}
		}
		else
		{
			break;
		}
		
	} while( !atHeadOfList() );
}

_XParticleGroupItem* _XParticleGroupInstanceManager::FindInstance( int templateid )
{
	if( listEmpty() ) return NULL;
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			if( pParticleGroupItem->m_ID == templateid )
			{
				return pParticleGroupItem;
			}
		}	
		else
		{
			return NULL;
		}
		nextItem();
	} while( !atHeadOfList() );

	return NULL;
}

void _XParticleGroupInstanceManager::disposeList(void)
{
	Dispose();
}

void _XParticleGroupInstanceManager::Dispose( void )
{
#ifdef _XDEF_MODELTHREADLOADING

	if( listEmpty() ) return;
	
	resetList();
	while(!listEmpty())
	{
	  _XParticleGroupItem* pParticleGroupItem = (_XParticleGroupItem*)currentItem();
	  if( pParticleGroupItem )
	  {
		  pParticleGroupItem->Release();
		  removeItem(pParticleGroupItem);
		  g_ParticleGroupItemPool.Delete( pParticleGroupItem );
	  }
	}

#else

	_XList::disposeList();

#endif
}

void _XParticleGroupInstanceManager::SetParticleMaxSize( D3DXVECTOR3& size )
{
	if( listEmpty() ) return;
	
	_XParticleGroupItem* pParticleGroupItem;
	
	resetList();
	do 
	{
		pParticleGroupItem = (_XParticleGroupItem*)currentItem();
		
		if( pParticleGroupItem )
		{
			pParticleGroupItem->SetParticleMaxSize( size );
		}	
		else
		{
			break;
		}
		nextItem();
	} while( !atHeadOfList() );
}
