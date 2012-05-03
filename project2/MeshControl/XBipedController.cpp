// XBipedController.cpp: implementation of the _XBipedController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XBipedController.h"
#include "XAnimMeshContainer.h"
#include "XParticleList.h"
#include "XUser.h"
#include "XFunctionalObject.h"

XMemoryPool<_XParticleGroupInstanceManager>			g_ParticleGroupInstanceManagerPool;

void ReplaceCharacter( LPTSTR _string, TCHAR srcchar, TCHAR changechar )
{
	if( !_string ) return;
	
	int length = strlen( _string );
	for( int i = 0; i < length; i++ )
	{
		if( _string[i] == srcchar )
		{
			_string[i] = changechar;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// _XModelDescriptor
//////////////////////////////////////////////////////////////////////

_XModelDescriptor::_XModelDescriptor()
{	

#ifdef _XDEF_MODELTHREADLOADING
#else
	m_ppModelList					= NULL;	
#endif

	m_EffectManager					= NULL;

#ifdef _XDEF_MODELTHREADLOADING
#else
	m_pSwordEffectInstanceObject	= NULL;
#endif

	m_SwordEffectCount				= 0;
	m_SwordEffectInitialized		= FALSE;
	Reset();	
}

_XModelDescriptor::~_XModelDescriptor()
{
	if( m_EffectManager )
	{

#ifdef _XDEF_MODELTHREADLOADING
		m_EffectManager->Dispose();
		g_ParticleGroupInstanceManagerPool.Delete(m_EffectManager);

#ifdef _XDWDEBUG		
		TCHAR messagestr[64];
		_snprintf( messagestr, sizeof(messagestr), "MDEffectManager restore : %p\n", m_EffectManager );
		OutputDebugString( messagestr );
#endif
		m_EffectManager = NULL;
#else
		m_EffectManager->Dispose();
		delete m_EffectManager;
#endif
	}

#ifdef _XDEF_MODELTHREADLOADING
#else
	SAFE_DELETE_ARRAY( m_pSwordEffectInstanceObject );
#endif

#ifdef _XDEF_MODELTHREADLOADING
#else
	SAFE_DELETE_ARRAY( m_ppModelList );
#endif

	m_ShadowDecal.InvalidateDeviceObject();
}

_XModelDescriptor& _XModelDescriptor::operator =(_XModelDescriptor& modeldescriptor)
{
	if(&modeldescriptor == this)
		return *this;
	
	Reset();
	
	m_AniFlag					= modeldescriptor.m_AniFlag;
	m_AniFrame					= 0.0f;
	m_AniLastFrame				= 0.0f;
		
	m_SlerpAniFrame				= 0.0f;
	m_SlerpAnimationTime		= 0.0f;
	
	m_CurMotion					= modeldescriptor.m_CurMotion;
	m_Position					= modeldescriptor.m_Position;
	
	m_PrevMotion				= modeldescriptor.m_PrevMotion;
	m_PrevMotionFrame			= modeldescriptor.m_PrevMotionFrame;
	m_MotionChangeFlag			= modeldescriptor.m_MotionChangeFlag;
	m_MotionChangingFlag		= modeldescriptor.m_MotionChangingFlag;
	m_NextMotion				= modeldescriptor.m_NextMotion;
	
	m_CameraDistance			= 0.0f;
	m_FrameResult				= 0;
		
	m_ModelStackSize			= modeldescriptor.m_ModelStackSize;
	
#ifdef _XDEF_MODELTHREADLOADING
	_ASSERT( m_ModelStackSize <= _XDEF_MAXMODELSTACKSIZE );
#else
	m_ppModelList = new _XAnimMeshAbstractClass*[m_ModelStackSize];
#endif

	for(int i = 0 ; i < m_ModelStackSize ; i++)
	{
		m_ppModelList[i] = modeldescriptor.m_ppModelList[i];		
	}
	
	memset( &m_EventHandler, 0, sizeof(_XANIEVENT_STRUCTURE) );
	
	m_pBipedController = modeldescriptor.m_pBipedController;	
	
	return *this;
}

void _XModelDescriptor::Reset( void )
{
	m_AniFlag				= FALSE;
	m_LockEndFrame			= FALSE;
	m_AniFrame				= 0.0f;
	m_AniLastFrame			= 0.0f;
			
	m_CurMotion				= 0;	
	m_PrevMotion			= 0;
	m_PrevMotionFrame		= 0;
	m_MotionChangeFlag		= FALSE;
	m_MotionChangingFlag	= FALSE;
	m_NextMotion			= 0;
	m_SlerpAniFrame			= 0.0f;
	m_SlerpAnimationTime	= 0.0f;		

	m_CameraDistance		= 0.0f;
	m_FrameResult			= 0;
	m_AttackClass			= _XDEF_ATTACKCLASS_LOW;
	m_WeaponClass			= 0;
	m_EffectPosition		= 0;
	m_NoAnimationAndPresentPipeline = FALSE;

	memset( &m_EventHandler, 0, sizeof(_XANIEVENT_STRUCTURE) );
	m_pBipedController		= NULL;	
	m_ModelStackSize		= 0;
	m_EnableEffectProcess	= FALSE;

	D3DXMatrixIdentity( &m_Position	);
	for (int i = 0; i < _XParticleLinkBone_MaxCount; ++ i)
	{
		m_PartMatrixContainer[i] = m_Position;
	}

/*	m_PartMatrixContainer[0] = m_Position;
	m_PartMatrixContainer[1] = m_Position;
	m_PartMatrixContainer[2] = m_Position;
	m_PartMatrixContainer[3] = m_Position;
	m_PartMatrixContainer[4] = m_Position;
	m_PartMatrixContainer[5] = m_Position;
	m_PartMatrixContainer[6] = m_Position;
	m_PartMatrixContainer[7] = m_Position;
*/
	if( m_EffectManager )
	{
		m_EffectManager->Dispose();
#ifdef _XDEF_MODELTHREADLOADING
		g_ParticleGroupInstanceManagerPool.Delete(m_EffectManager);
		m_EffectManager = NULL;
#else
		SAFE_DELETE( m_EffectManager );
#endif
	}

	m_ViewLeftSwordEffect = FALSE;
	m_ViewRightSwordEffect = FALSE;
#ifdef _XDEF_MODELTHREADLOADING
#else
	SAFE_DELETE_ARRAY( m_pSwordEffectInstanceObject );
#endif

#ifdef _XDEF_MODELTHREADLOADING	
	m_ModelStackSize = 0;

	for( i = 0; i < _XDEF_MAXMODELSTACKSIZE; i++ )
	{
		m_ppModelList[i] = NULL;
	}
#else
	SAFE_DELETE_ARRAY( m_ppModelList );
#endif
	
	m_BlockerCenterHeight		= 0.65f;
	m_ShadowScaleFactor			= 0.85f;
	m_ShadowDecalSize			= 1.85f;

	m_ShadowDecal.InvalidateDeviceObject();
}

BOOL _XModelDescriptor::ReserveModelStack( int modelstacksize )
{
#ifdef _XDEF_MODELTHREADLOADING
#else
	SAFE_DELETE_ARRAY( m_ppModelList );
#endif
	m_ModelStackSize = 0;

#ifdef _XDEF_MODELTHREADLOADING
#else
	m_ppModelList = new _XAnimMeshAbstractClass*[ modelstacksize ];

	if( !m_ppModelList )
	{
		_XFatalError( "_XModelDescriptor :: Reserve model stack memory allocation [%d]", modelstacksize );
		return FALSE;
	}
#endif

	m_ModelStackSize = modelstacksize;
#ifdef _XDEF_MODELTHREADLOADING
	for( int i = 0; i < _XDEF_MAXMODELSTACKSIZE; i++ )
#else
	for( int i = 0; i < modelstacksize; i++ )
#endif
	{
		m_ppModelList[i] = NULL;
	}

	return TRUE;
}

void _XModelDescriptor::InitEffectManager( int swordeffectcount, int tracetextureindex )
{		
	if( !m_EffectManager )
	{
#ifdef _XDEF_MODELTHREADLOADING
		m_EffectManager = g_ParticleGroupInstanceManagerPool.New();
		m_EffectManager->Dispose();

#ifdef _XDWDEBUG		
		TCHAR messagestr[64];
		_snprintf( messagestr, sizeof(messagestr), "MDEffectManager allocated : %p\n", m_EffectManager );
		OutputDebugString( messagestr );
#endif

#else
		m_EffectManager = new _XParticleGroupInstanceManager;
#endif
		m_EffectManager->SetTemplateManager( &g_CharacterEffectGroupManager );
	}

	m_SwordEffectCount =  swordeffectcount;

#ifdef _XDEF_MODELTHREADLOADING	
	for( int i = 0; i < m_SwordEffectCount; i++ )
	{
		m_pSwordEffectInstanceObject[i].ResetControlPoint();
		m_pSwordEffectInstanceObject[i].TextureIndex = tracetextureindex;
	}
#else
	SAFE_DELETE( m_pSwordEffectInstanceObject );

	if( !m_pSwordEffectInstanceObject )
	{
		if( swordeffectcount > 0 )
		{
			m_pSwordEffectInstanceObject = new _XEffect_SwordTrace_InstanceData[swordeffectcount];
			m_pSwordEffectInstanceObject->TextureIndex = tracetextureindex;
		}
	}
#endif
	
	m_EnableEffectProcess = TRUE;
}

void _XModelDescriptor::SetMeshModel( _XUSER_MODELDESCRIPTTYPE& modeldescript, BOOL gender, int groupindex, int clothview )
{
	int extravalue = ( gender == _XGENDER_MAN ) ? 0 : g_Model_GenderSeparateIndex;

	if( groupindex == _XGROUP_SORIM )
	{
		if( gender== _XGENDER_MAN ) 
		{
			// 소림사 머리카락은 없슴.
			SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
			
			if( modeldescript.face )
				SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType	[g_Model_SorimHairSeparateIndex + modeldescript.face-1] );
			else SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType   [g_Model_SorimHairSeparateIndex] ); // 소림사 기본 얼굴
		}
		else
		{   // 소림사에 여자일경우 (그럴리는 없겠지만 그래도 기본으로 설정)
			if( modeldescript.hair )
			{
				if( modeldescript.cap == 0 ) // 모자를 썻을때는 머리카락 처리 안함. 
					SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 g_UserModel_HairType	[extravalue+modeldescript.hair-1] );
				else SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
			}
			else 
			{
				if( modeldescript.cap == 0 ) // 모자를 썻을때는 머리카락 처리 안함. 
					SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 g_UserModel_HairType	[extravalue] ); // 기본 머리카락 설정 
				else SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
			}
			
			if( modeldescript.face )
				SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType	[extravalue+modeldescript.face-1] );
			else SetMeshModel( _XDEF_MODELCLUSTER_FACE, 	 g_UserModel_FaceType	[extravalue] );  // 얼굴정보가 없으면 기본 출력 
		}
	}
	else
	{
		if( modeldescript.hair )
		{
			if( modeldescript.cap == 0 ) // 모자를 썻을때는 머리카락 처리 안함. 
				SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 g_UserModel_HairType	[extravalue+modeldescript.hair-1] );
			else SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
		}
		else 
		{
			if( modeldescript.cap )
				SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
			else
				SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 g_UserModel_HairType[extravalue] );
		}
		
		if( modeldescript.face )
			SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType	[extravalue+modeldescript.face-1] );
		else SetMeshModel( _XDEF_MODELCLUSTER_FACE, 	 g_UserModel_FaceType	[extravalue] ); // 얼굴정보가 없으면 기본 출력 
	}

	if( modeldescript.cap )
	{
		// 소림사는 소림 모자만 쓸수 있다.
		if( groupindex == _XGROUP_SORIM && gender == _XGENDER_MAN )
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.capCategory) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.capCategory);
				if( ClothesItem )
				{
					if( ClothesItem[modeldescript.cap].cClan == _XGROUP_SORIM )
						SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +ClothesItem[modeldescript.cap].sModelIndex] );				
					else
					{
						if( ClothesItem[modeldescript.cap].cClan == _XGROUP_NANGIN &&
							ClothesItem[modeldescript.cap].sReqSim  == 1 ) //소림 특수 모자 쓰게
							SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +ClothesItem[modeldescript.cap].sModelIndex] );
						else
							SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );				
					}
				}
			}
#else
			if( modeldescript.capCategory == _XGI_FC_CLOTHES )
			{
				if( g_ClothesItemProperty[modeldescript.cap].cClan == _XGROUP_SORIM )
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +g_ClothesItemProperty[modeldescript.cap].sModelIndex] );				
				else
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );				
			}
			else if( modeldescript.capCategory == _XGI_FC_CLOTHES2 )
			{
				if( g_ClothesItemProperty2[modeldescript.cap].cClan == _XGROUP_SORIM )
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +g_ClothesItemProperty2[modeldescript.cap].sModelIndex] );				
				else
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );				
			}
			else if( modeldescript.capCategory == _XGI_FC_CLOTHES3 )
			{
				if( g_ClothesItemProperty3[modeldescript.cap].cClan == _XGROUP_SORIM )
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +g_ClothesItemProperty3[modeldescript.cap].sModelIndex] );				
				else
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );				
			}
#endif
		}
		else
		{
			if( modeldescript.hair )
			{
				// 문파모자는 모델인덱스 바로 적용
				/*if( g_ClothesItemProperty[modeldescript.cap].cClan )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +g_ClothesItemProperty[modeldescript.cap].sModelIndex] );
				}
				else*/
				{
					// 머리 색깔에 맞춰 적용
					int modelcolorindex = (modeldescript.hair-1) % 3;								 // 머리 색은 3타입이므로 3타입식 건너뛰고 머리색에 맞는 모델 선택

#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.capCategory) )
					{
						_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.capCategory);
						if( ClothesItem )
							SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue + ClothesItem[modeldescript.cap].sModelIndex + modelcolorindex ] );
					}
					else 
						SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue + g_ClothesItemProperty[modeldescript.cap].sModelIndex + modelcolorindex ] );
#else
					if( modeldescript.capCategory == _XGI_FC_CLOTHES )
						SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty[modeldescript.cap].sModelIndex + modelcolorindex ] );
					else if( modeldescript.capCategory == _XGI_FC_CLOTHES2 )
						SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty2[modeldescript.cap].sModelIndex + modelcolorindex ] );
					else if( modeldescript.capCategory == _XGI_FC_CLOTHES3 )
						SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty3[modeldescript.cap].sModelIndex + modelcolorindex ] );
					else 
						SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty[modeldescript.cap].sModelIndex + modelcolorindex ] );
#endif
				}
			}
			else // 머리카락 정보가 없을경우 ( 예외처리 )
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.capCategory) )
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.capCategory);
					if( ClothesItem )
						SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue + ClothesItem[modeldescript.cap].sModelIndex ] );
				}
				else
					SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue + g_ClothesItemProperty[modeldescript.cap].sModelIndex ] );
#else
				if( modeldescript.capCategory == _XGI_FC_CLOTHES )
					SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty[modeldescript.cap].sModelIndex ] );
				else if( modeldescript.capCategory == _XGI_FC_CLOTHES2 )
					SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty2[modeldescript.cap].sModelIndex ] );
				else if( modeldescript.capCategory == _XGI_FC_CLOTHES3 )
					SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty3[modeldescript.cap].sModelIndex ] );
				else 
					SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty[modeldescript.cap].sModelIndex ] );
#endif
			}
		}
	}
	else SetMeshModel( _XDEF_MODELCLUSTER_CAP,		NULL );
	

	if( modeldescript.upperbody )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.upperbodyCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.upperbodyCategory);
			if( ClothesItem )
				SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType[extravalue+ClothesItem[modeldescript.upperbody].sModelIndex] );
		}
		else
			SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType[extravalue+g_ClothesItemProperty[modeldescript.upperbody].sModelIndex] );
#else
		if( modeldescript.upperbodyCategory == _XGI_FC_CLOTHES )
			SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType[extravalue+g_ClothesItemProperty[modeldescript.upperbody].sModelIndex] );
		else if( modeldescript.upperbodyCategory == _XGI_FC_CLOTHES2 )
			SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType[extravalue+g_ClothesItemProperty2[modeldescript.upperbody].sModelIndex] );
		else if( modeldescript.upperbodyCategory == _XGI_FC_CLOTHES3 )
			SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType[extravalue+g_ClothesItemProperty3[modeldescript.upperbody].sModelIndex] );
		else
			SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType[extravalue+g_ClothesItemProperty[modeldescript.upperbody].sModelIndex] );
#endif
	}
	else SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,  g_UserModel_UBodyType[extravalue] );
	
	if( modeldescript.lowerbody )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.lowerbodyCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.lowerbodyCategory);
			if( ClothesItem )
				SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType[extravalue+ClothesItem[modeldescript.lowerbody].sModelIndex] );
		}
		else
			SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,  g_UserModel_LBodyType[extravalue+g_ClothesItemProperty[modeldescript.lowerbody].sModelIndex]  );			
#else
		if( modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES )
			SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType[extravalue+g_ClothesItemProperty[modeldescript.lowerbody].sModelIndex] );
		else if( modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES2 )
			SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType[extravalue+g_ClothesItemProperty2[modeldescript.lowerbody].sModelIndex] );
		else if( modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES3 )
			SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType[extravalue+g_ClothesItemProperty3[modeldescript.lowerbody].sModelIndex] );
		else 
			SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType[extravalue+g_ClothesItemProperty[modeldescript.lowerbody].sModelIndex] );
#endif
	}
	else SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,  g_UserModel_LBodyType[extravalue]  );
		
	if( modeldescript.item1 == 0 )// 아무것도 안들은 경우 
	{
		if( modeldescript.lhand )
		{
			SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
		}
		else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
		
		if( modeldescript.rhand )
		{
			SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
		}
		else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );		
	}
	else
	{
		// 권각 아이템일경우 손은 렌더링 안함.
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(modeldescript.item1Category);
			if( WeaponItem )
			{
				if( WeaponItem[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
				{
					if( WeaponItem[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{				
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
					}			
					else
					{
						if( modeldescript.lhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( modeldescript.rhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}			
				else
				{
					if( modeldescript.lhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					if( modeldescript.rhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );				
				}
			}
		}
#else
		if( modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if( g_WeaponItemProperty[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{				
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( modeldescript.lhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );

					if( modeldescript.rhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}			
			else
			{
				if( modeldescript.lhand )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
				}
				else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
				
				if( modeldescript.rhand )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
				}
				else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );				
			}
		}		
		else if( modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if( g_WeaponItemProperty2[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty2[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{				
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					
					if( modeldescript.lhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );

					if( modeldescript.rhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );					
				}
			}			
			else
			{
				if( modeldescript.lhand )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
				}
				else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
				
				if( modeldescript.rhand )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
				}
				else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
			}
		}
		else if( modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if( g_WeaponItemProperty3[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty3[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{				
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					
					if( modeldescript.lhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );

					if( modeldescript.rhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );					
				}
			}			
			else
			{
				if( modeldescript.lhand )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
				}
				else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
				
				if( modeldescript.rhand )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
				}
				else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
			}
		}
#endif
	}	

	if( modeldescript.lfoot )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.lfootCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.lfootCategory);
			if( ClothesItem )
				SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+ClothesItem[modeldescript.lfoot].sModelIndex] );
		}
		else
			SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,	 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty[modeldescript.lfoot].sModelIndex] );
#else
		if( modeldescript.lfootCategory == _XGI_FC_CLOTHES )
			SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty[modeldescript.lfoot].sModelIndex] );
		else if( modeldescript.lfootCategory == _XGI_FC_CLOTHES2 )
			SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty2[modeldescript.lfoot].sModelIndex] );
		else if( modeldescript.lfootCategory == _XGI_FC_CLOTHES3 )
			SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty3[modeldescript.lfoot].sModelIndex] );
		else 
			SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty[modeldescript.lfoot].sModelIndex] );
#endif
	}
	else SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,	 g_UserModel_LFootType	[extravalue] );
	
	if( modeldescript.rfoot )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.rfootCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.rfootCategory);
			if( ClothesItem )
				SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+ClothesItem[modeldescript.rfoot].sModelIndex] );
		}
		else
			SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,	 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty[modeldescript.rfoot].sModelIndex] );
#else
		if( modeldescript.rfootCategory == _XGI_FC_CLOTHES )
			SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty[modeldescript.rfoot].sModelIndex] );
		else if( modeldescript.rfootCategory == _XGI_FC_CLOTHES2 )
			SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty2[modeldescript.rfoot].sModelIndex] );
		else if( modeldescript.rfootCategory == _XGI_FC_CLOTHES3 )
			SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty3[modeldescript.rfoot].sModelIndex] );
		else
			SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty[modeldescript.rfoot].sModelIndex] );
#endif
	}
	else SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,	 g_UserModel_RFootType	[extravalue] );

	if( modeldescript.item1 == 0 )// 아무것도 안들은 경우 
	{
		if( modeldescript.adae )
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.adaeCategory) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.adaeCategory);
				if( ClothesItem )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+ClothesItem[modeldescript.adae].sModelIndex] );
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+ClothesItem[modeldescript.adae].sModelIndex] );
				}
			}
			else
			{
				SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );					
				SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
			}
#else
			if( modeldescript.adaeCategory == _XGI_FC_CLOTHES )
			{
				SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
				SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
			}
			else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
			{
				SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
				SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
			}
			else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
			{
				SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
				SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
			}
			else
			{
				SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
				SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
			}
#endif
		}
		else 
		{
			if( modeldescript.lhand )
			{
				SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
			}
			else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
			
			if( modeldescript.rhand )
			{
				SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
			}
			else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
		}
	}
	else
	{
		// 권각 아이템일경우 손 및 아대는 렌더링 안함.
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(modeldescript.item1Category);
			if( WeaponItem )			
			{
				if( WeaponItem[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
				{
					if( WeaponItem[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
					}			
					else
					{
						if( modeldescript.adae )
						{
							if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.adaeCategory) )
							{
								_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.adaeCategory);
								if( ClothesItem )
								{
									SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+ClothesItem[modeldescript.adae].sModelIndex] );
									SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+ClothesItem[modeldescript.adae].sModelIndex] );									
								}
							}
							else
							{
								SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
								SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
							}
						}
						else 
						{
							if( modeldescript.lhand )
							{
								SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
							}
							else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
							
							if( modeldescript.rhand )
							{
								SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
							}
							else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
						}
					}
				}
				else
				{
					if( modeldescript.adae )
					{
						if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.adaeCategory) )
						{
							_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.adaeCategory);
							if( ClothesItem )
							{
								SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+ClothesItem[modeldescript.adae].sModelIndex] );
								SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+ClothesItem[modeldescript.adae].sModelIndex] );
							}
						}
						else
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
						}
					}
					else 
					{
						if( modeldescript.lhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( modeldescript.rhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}
			}
		}
#else
		if( modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if( g_WeaponItemProperty[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( modeldescript.adae )
					{
						if( modeldescript.adaeCategory == _XGI_FC_CLOTHES )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						}
						else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
						}
						else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
						}
						else 
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						}
					}
					else 
					{
						if( modeldescript.lhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( modeldescript.rhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}
			}
			else
			{
				if( modeldescript.adae )
				{
					if( modeldescript.adaeCategory == _XGI_FC_CLOTHES )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
					}
					else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
					}
					else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
					}
					else
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
					}
				}
				else 
				{
					if( modeldescript.lhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					if( modeldescript.rhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}
		}
		else if( modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if( g_WeaponItemProperty2[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty2[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( modeldescript.adae )
					{
						if( modeldescript.adaeCategory == _XGI_FC_CLOTHES )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						}
						else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
						}
						else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
						}
						else
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						}
					}
					else 
					{
						if( modeldescript.lhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( modeldescript.rhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}
			}
			else
			{
				if( modeldescript.adae )
				{
					if( modeldescript.adaeCategory == _XGI_FC_CLOTHES )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
					}
					else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
					}
					else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
					}
					else
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
					}
				}
				else 
				{
					if( modeldescript.lhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					if( modeldescript.rhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}
		}
		else if( modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if( g_WeaponItemProperty3[modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty3[modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( modeldescript.adae )
					{
						if( modeldescript.adaeCategory == _XGI_FC_CLOTHES )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						}
						else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
						}
						else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
						}
						else
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						}
					}
					else 
					{
						if( modeldescript.lhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( modeldescript.rhand )
						{
							SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
						}
						else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}
			}
			else
			{
				if( modeldescript.adae )
				{
					if( modeldescript.adaeCategory == _XGI_FC_CLOTHES )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
					}
					else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[modeldescript.adae].sModelIndex] );
					}
					else if( modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[modeldescript.adae].sModelIndex] );
					}
					else
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.adae].sModelIndex] );
					}
				}
				else 
				{
					if( modeldescript.lhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[modeldescript.lhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					if( modeldescript.rhand )
					{
						SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[modeldescript.rhand].sModelIndex] );
					}
					else SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}
		}
#endif
	}
	
	if( modeldescript.backpack )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.backpackCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.backpackCategory);
			if( ClothesItem )
				SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+ClothesItem[modeldescript.backpack].sModelIndex] );
		}
		else
			SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty[modeldescript.backpack].sModelIndex] );
#else
		if( modeldescript.backpackCategory == _XGI_FC_CLOTHES )
			SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty[modeldescript.backpack].sModelIndex] );		
		else if( modeldescript.backpackCategory == _XGI_FC_CLOTHES2 ) 
			SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty2[modeldescript.backpack].sModelIndex] );		
		else if( modeldescript.backpackCategory == _XGI_FC_CLOTHES3 ) 
			SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty3[modeldescript.backpack].sModelIndex] );		
		else
			SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty[modeldescript.backpack].sModelIndex] );		
#endif
	}
	else SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 NULL );
	
	if( modeldescript.mask )
	{
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
		if(clothview == 0)
#endif
		{
			// 치장의상 착용시 다른 아이템들은 보이지 않게 한다.
			SetMeshModel( _XDEF_MODELCLUSTER_HAIR, NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,  NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,  NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,	NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,	NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_LHAND, NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_RHAND, NULL );
			SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 NULL );

			int genderextravalue = 0;
			int genderextravalue2 = 0;
			if( gender== _XGENDER_WOMAN ) genderextravalue = g_MaskModel_GenderSeparateIndex;
			
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_CLOTHES == GetEqualItemType(modeldescript.maskCategory) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(modeldescript.maskCategory);
				if( ClothesItem )			
				{
					if( gender == _XGENDER_WOMAN ) 
					{
						// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
						if( ClothesItem[modeldescript.mask].sModelIndex >= 140 ) 
						{
							// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
							genderextravalue = 0;
							genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
						}
					}

					SetMeshModel( _XDEF_MODELCLUSTER_MASK,	 g_UserModel_Mask[genderextravalue+genderextravalue2+ClothesItem[modeldescript.mask].sModelIndex] );
				}
			}
#else
			if( modeldescript.maskCategory == _XGI_FC_CLOTHES )
			{
				// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
				if( g_ClothesItemProperty[modeldescript.mask].sModelIndex >= 140 ) 
				{
					// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
					genderextravalue = 0;
					genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
				}

				SetMeshModel( _XDEF_MODELCLUSTER_MASK,	 g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty[modeldescript.mask].sModelIndex] );
			}
			else if( modeldescript.backpackCategory == _XGI_FC_CLOTHES2 ) 
			{
				// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
				if( g_ClothesItemProperty2[modeldescript.mask].sModelIndex >= 140 ) 
				{
					// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
					genderextravalue = 0;
					genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
				}

				SetMeshModel( _XDEF_MODELCLUSTER_MASK,	 g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty2[modeldescript.mask].sModelIndex] );		
			}
			else if( modeldescript.backpackCategory == _XGI_FC_CLOTHES3 ) 
			{
				// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
				if( g_ClothesItemProperty3[modeldescript.mask].sModelIndex >= 140 ) 
				{
					// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
					genderextravalue = 0;
					genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
				}

				SetMeshModel( _XDEF_MODELCLUSTER_MASK,	 g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty3[modeldescript.mask].sModelIndex] );		
			}
#endif
		}
	}

	if( modeldescript.item1 )
	{
		m_ViewLeftSwordEffect = TRUE;

#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(modeldescript.item1Category);
#ifdef _XDEF_MODELTHREADLOADING
			if( WeaponItem && WeaponItem[modeldescript.item1].sModelIndex > 0 )
				SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		g_ItemModel_Weapon[( gender == _XGENDER_MAN ) ? 0 : 1][WeaponItem[modeldescript.item1].sModelIndex] );
			else
				SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		NULL );

#else
			if( WeaponItem )
				SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		g_ItemModel_Weapon[( gender == _XGENDER_MAN ) ? 0 : 1][WeaponItem[modeldescript.item1].sModelIndex] );
#endif

		}
#else
		if( modeldescript.item1Category == _XGI_FC_WEAPON )
			SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		g_ItemModel_Weapon[( gender == _XGENDER_MAN ) ? 0 : 1][g_WeaponItemProperty[modeldescript.item1].sModelIndex] );
		else if( modeldescript.item1Category == _XGI_FC_WEAPON2 )
			SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		g_ItemModel_Weapon[( gender == _XGENDER_MAN ) ? 0 : 1][g_WeaponItemProperty2[modeldescript.item1].sModelIndex] );
		else if( modeldescript.item1Category == _XGI_FC_WEAPON3 )
			SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		g_ItemModel_Weapon[( gender == _XGENDER_MAN ) ? 0 : 1][g_WeaponItemProperty3[modeldescript.item1].sModelIndex] );
		else
			SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		g_ItemModel_Weapon[( gender == _XGENDER_MAN ) ? 0 : 1][g_WeaponItemProperty[modeldescript.item1].sModelIndex] );
#endif
	}
	else
	{
		m_ViewLeftSwordEffect = FALSE;
		SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		NULL );
	}
	
	SetMeshModel( _XDEF_MODELCLUSTER_ITEM2, NULL );
	SetMeshModel( _XDEF_MODELCLUSTER_ITEM3, NULL ); 
	SetMeshModel( _XDEF_MODELCLUSTER_ITEM4, NULL ); 
	SetMeshModel( _XDEF_MODELCLUSTER_ITEM5, NULL ); 
}

void _XModelDescriptor::SetShadowDecalInfo( FLOAT blockercenterheight, FLOAT maxradius, FLOAT decalsize  )
{	
	m_BlockerCenterHeight	=	blockercenterheight;
	m_ShadowScaleFactor		=	maxradius;
	m_ShadowDecalSize		=	decalsize;
}

void _XModelDescriptor::RenderPlaneShadow( void )
{
	/*
	if( !g_bCharacterShadow ) return;
	
	RenderObjectShadow();
	
	gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
	D3DXMATRIX prevtextureuv;
	//gpDev->SetRenderState( D3DRS_DEPTHBIAS , 7 );
	//gpDev->GetTransform( D3DTS_TEXTURE0, &prevtextureuv );
	
	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_TERRAIN_TWOPASSCHARSHADOW );
	
	D3DXMATRIX matCameraToWorld;// [카메라 좌표] -> [월드 좌표]
	D3DXMatrixTranspose(&matCameraToWorld, &g_LodTerrain.m_3PCamera.GetViewMatrix());
	matCameraToWorld._41	= g_LodTerrain.m_3PCamera.m_CameraPosition.x;
	matCameraToWorld._42	= g_LodTerrain.m_3PCamera.m_CameraPosition.y;
	matCameraToWorld._43	= g_LodTerrain.m_3PCamera.m_CameraPosition.z;
	
	matCameraToWorld._14	= 0.0f;
	matCameraToWorld._24	= 0.0f;
	matCameraToWorld._34	= 0.0f;
	matCameraToWorld._44	= 1.0f;
	
	D3DXMATRIX		matShadowProjOffset;		
	D3DXMatrixMultiply( &matShadowProjOffset, &m_matShadowProj, &g_ShadowDefaultoffset );
	D3DXMatrixMultiply( &m_ShadowUVMatrix, &matCameraToWorld, &m_matWorldToLight);
	D3DXMatrixMultiply( &m_ShadowUVMatrix, &m_ShadowUVMatrix, &matShadowProjOffset );
	
	gpDev->SetTransform( D3DTS_TEXTURE0, &m_ShadowUVMatrix );	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	
#ifdef _XDWDEBUG
	if( !g_UseTexture )
	{
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		gpDev->SetTexture( 0, NULL );		
	}
	else
	{
#endif
		gpDev->SetTexture( 0, g_lpShadowTexture );
#ifdef _XDWDEBUG
	}
#endif
	
	int nTileX, nTileZ;
	nTileX = m_Position._41 / 2;
	nTileZ = m_Position._43 / 2;
	
	if(m_ShadowCurTile.x != nTileX || m_ShadowCurTile.y != nTileZ)
	{
		m_ShadowCurTile.x = nTileX;
		m_ShadowCurTile.y = nTileZ;
		
		m_LastGenerateDecalPos = D3DXVECTOR3( m_Position._41, m_Position._42, m_Position._43 );
		g_LodTerrain.GetTerrainDecal(m_nShadowVertexCount, m_nShadowIndexCount, m_LastGenerateDecalPos, m_nShadowTileCount, &m_ShadowVertextBuffer, &m_ShadowIndexBuffer);
		MakePlaneDecal();
	}
	
	gpDev->SetFVF(D3DFVF_XYZ);
	gpDev->SetStreamSource(0,m_ShadowVertextBuffer, 0, sizeof(D3DXVECTOR3));
	gpDev->SetIndices(m_ShadowIndexBuffer);
	gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nShadowVertexCount, 0, m_nShadowIndexCount/3);
	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	D3DXMATRIX		orgmatrix;
	D3DXMatrixIdentity(&orgmatrix);
	gpDev->SetTransform( D3DTS_TEXTURE0, &orgmatrix );
	
	gpDev->SetSamplerState( 0, D3DSAMP_ADDRESSU,	D3DTADDRESS_WRAP);
	gpDev->SetSamplerState( 0, D3DSAMP_ADDRESSV,	D3DTADDRESS_WRAP);
	*/
}


/*
void _XModelDescriptor::RenderShadow( void )
{		
	if( !g_bCharacterShadow || !m_ShadowVertextBuffer ) return;

	RenderObjectShadow();
	
	gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
	D3DXMATRIX prevtextureuv;
	//gpDev->SetRenderState( D3DRS_DEPTHBIAS , 7 );
	//gpDev->GetTransform( D3DTS_TEXTURE0, &prevtextureuv );

	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_TERRAIN_TWOPASSCHARSHADOW );

	D3DXMATRIX matCameraToWorld;// [카메라 좌표] -> [월드 좌표]
	D3DXMatrixTranspose(&matCameraToWorld, &g_LodTerrain.m_3PCamera.GetViewMatrix());
	matCameraToWorld._41	= g_LodTerrain.m_3PCamera.m_CameraPosition.x;
	matCameraToWorld._42	= g_LodTerrain.m_3PCamera.m_CameraPosition.y;
	matCameraToWorld._43	= g_LodTerrain.m_3PCamera.m_CameraPosition.z;
	
	matCameraToWorld._14	= 0.0f;
	matCameraToWorld._24	= 0.0f;
	matCameraToWorld._34	= 0.0f;
	matCameraToWorld._44	= 1.0f;
	
	D3DXMATRIX		matShadowProjOffset;		
	D3DXMatrixMultiply( &matShadowProjOffset, &m_matShadowProj, &g_ShadowDefaultoffset );
	D3DXMatrixMultiply( &m_ShadowUVMatrix, &matCameraToWorld, &m_matWorldToLight);
	D3DXMatrixMultiply( &m_ShadowUVMatrix, &m_ShadowUVMatrix, &matShadowProjOffset );

	gpDev->SetTransform( D3DTS_TEXTURE0, &m_ShadowUVMatrix );	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

#ifdef _XDWDEBUG
	if( !g_UseTexture )
	{
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		gpDev->SetTexture( 0, NULL );		
	}
	else
	{
#endif
		gpDev->SetTexture( 0, g_lpShadowTexture );
#ifdef _XDWDEBUG
	}
#endif

	int nTileX, nTileZ;
	nTileX = m_Position._41 / 2;
	nTileZ = m_Position._43 / 2;

	if( m_nShadowVertexCount == 0 || m_ShadowCurTile.x != nTileX || m_ShadowCurTile.y != nTileZ)
	{
		m_ShadowCurTile.x = nTileX;
		m_ShadowCurTile.y = nTileZ;

		m_LastGenerateDecalPos = D3DXVECTOR3( m_Position._41, m_Position._42, m_Position._43 );
		g_LodTerrain.GetTerrainDecal(m_nShadowVertexCount, m_nShadowIndexCount, m_LastGenerateDecalPos, m_nShadowTileCount, &m_ShadowVertextBuffer, &m_ShadowIndexBuffer);
	}

	if( m_nShadowVertexCount > 0 )
	{	
		gpDev->SetFVF(D3DFVF_XYZ);
		gpDev->SetStreamSource(0,m_ShadowVertextBuffer, 0, sizeof(D3DXVECTOR3));
		gpDev->SetIndices(m_ShadowIndexBuffer);
		gpDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nShadowVertexCount, 0, m_nShadowIndexCount/3);
	}

	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	D3DXMATRIX		orgmatrix;
	D3DXMatrixIdentity(&orgmatrix);
	gpDev->SetTransform( D3DTS_TEXTURE0, &orgmatrix );

	gpDev->SetSamplerState( 0, D3DSAMP_ADDRESSU,	D3DTADDRESS_WRAP);
	gpDev->SetSamplerState( 0, D3DSAMP_ADDRESSV,	D3DTADDRESS_WRAP);
}
*/

extern D3DXVECTOR3 g_DecalUpVector;
extern D3DXVECTOR3 g_DecalTangentVector;

void _XModelDescriptor::RenderShadow( void )
{		
	if( !g_bCharacterShadow ) return;
	
	RenderObjectShadow();
	
	gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );		
	g_TerrainTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, _XDEF_ENVRENDERSTYLESET_TERRAIN_TWOPASSCHARSHADOW );
	
	D3DXMATRIX matCameraToWorld;// [카메라 좌표] -> [월드 좌표]
	D3DXMatrixTranspose(&matCameraToWorld, &g_LodTerrain.m_3PCamera.GetViewMatrix());
	matCameraToWorld._41	= g_LodTerrain.m_3PCamera.m_CameraPosition.x;
	matCameraToWorld._42	= g_LodTerrain.m_3PCamera.m_CameraPosition.y;
	matCameraToWorld._43	= g_LodTerrain.m_3PCamera.m_CameraPosition.z;
	
	matCameraToWorld._14	= 0.0f;
	matCameraToWorld._24	= 0.0f;
	matCameraToWorld._34	= 0.0f;
	matCameraToWorld._44	= 1.0f;
	
	D3DXMATRIX		matShadowProjOffset;		
	D3DXMatrixMultiply( &matShadowProjOffset, &m_matShadowProj, &g_ShadowDefaultoffset );
	D3DXMatrixMultiply( &m_ShadowUVMatrix, &matCameraToWorld, &m_matWorldToLight);
	D3DXMatrixMultiply( &m_ShadowUVMatrix, &m_ShadowUVMatrix, &matShadowProjOffset );
	
	gpDev->SetTransform( D3DTS_TEXTURE0, &m_ShadowUVMatrix );	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	
#ifdef _XDWDEBUG
	if( !g_UseTexture )
	{
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		gpDev->SetTexture( 0, NULL );		
	}
	else
	{
#endif
		gpDev->SetTexture( 0, g_lpShadowTexture );
#ifdef _XDWDEBUG
	}
#endif
	
	m_ShadowDecal.GenerateDecal( D3DXVECTOR3( m_Position._41,m_Position._42,m_Position._43 ), m_ShadowDecalSize, m_ShadowDecalSize, 0.8f );
	m_ShadowDecal.Render();
	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	gpDev->SetTransform( D3DTS_TEXTURE0, &g_IdentMatrix );
	gpDev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS,  D3DTTFF_DISABLE);
	
	gpDev->SetSamplerState( 0, D3DSAMP_ADDRESSU,	D3DTADDRESS_WRAP);
	gpDev->SetSamplerState( 0, D3DSAMP_ADDRESSV,	D3DTADDRESS_WRAP);
}

#define _XDEF_MODELSHADOWTEXTURERANGE	0.85f

/*
void _XModelDescriptor::GenerateShadowMap( void )
{	
	D3DXMATRIX		matLocalToWorld;
	D3DXMATRIX		matWorldToLight;
	D3DXMATRIX		matShadowProj;

	D3DXMatrixIdentity( &m_matLocalToWorld );
	D3DXMatrixIdentity( &m_matWorldToLight );
	D3DXMatrixIdentity( &matShadowProj   );
		
	D3DXVECTOR3 vBlockerCenter;
	vBlockerCenter.x	= m_Position._41;
	vBlockerCenter.y	= m_Position._42;
	vBlockerCenter.z	= m_Position._43;
	
	D3DXVECTOR3		vOrig	= g_EnvironmentManager.m_ObjectLight.Position;
	D3DXVECTOR3		vEnd	= vBlockerCenter;
	
	D3DXVECTOR3		vLightPo;
	vLightPo    = ( vEnd - vOrig );	
	vLightPo	= vOrig + 0.9999f * ( vEnd - vOrig );
					
	D3DXVECTOR3		vecFrom = (vLightPo - vBlockerCenter);
	D3DXVec3Normalize( &vecFrom, &vecFrom );
	vecFrom *= 20.0f;	
	vecFrom.y += 30.0f;
	vecFrom = vBlockerCenter + vecFrom;
	
	float			fXMax		= 0.0f;
	float			fYMax		= 0.0f;
	D3DXVECTOR3		vCur[4];
	D3DXVECTOR4		VOut;

	vCur[0].x = vBlockerCenter.x - _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[0].z = vBlockerCenter.z - _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[0].y = g_LodTerrain.GetTerrainHeight( vCur[0] );

	vCur[1].x = vBlockerCenter.x + _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[1].z = vBlockerCenter.z - _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[1].y = g_LodTerrain.GetTerrainHeight( vCur[1] );

	vCur[2].x = vBlockerCenter.x - _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[2].z = vBlockerCenter.z + _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[2].y = g_LodTerrain.GetTerrainHeight( vCur[2] );

	vCur[3].x = vBlockerCenter.x + _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[3].z = vBlockerCenter.z + _XDEF_MODELSHADOWTEXTURERANGE;
	vCur[3].y = g_LodTerrain.GetTerrainHeight( vCur[3] );


	D3DXMatrixLookAtLH( &m_matWorldToLight, &vecFrom, &vBlockerCenter, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	D3DXMATRIX matLocalToLight; // [그림자 객체 좌표] -> [광원 좌표] 행렬
	D3DXMatrixMultiply( &matLocalToLight, &m_matLocalToWorld, &m_matWorldToLight );

	D3DXVec3Transform( &VOut, &vCur[0], &matLocalToLight );
	fXMax = (float)fabs(VOut.x/VOut.z);
	fYMax = (float)fabs(VOut.y/VOut.z);

	D3DXVec3Transform( &VOut, &vCur[1], &matLocalToLight );
	if( (float)fabs(VOut.x / VOut.z)	> fXMax )	fXMax = (float)fabs(VOut.x/VOut.z);
	if( (float)fabs(VOut.y / VOut.z)	> fYMax )	fYMax = (float)fabs(VOut.y/VOut.z);

	D3DXVec3Transform( &VOut, &vCur[2], &matLocalToLight );
	if( (float)fabs(VOut.x / VOut.z)	> fXMax )	fXMax = (float)fabs(VOut.x/VOut.z);
	if( (float)fabs(VOut.y / VOut.z)	> fYMax )	fYMax = (float)fabs(VOut.y/VOut.z);

	D3DXVec3Transform( &VOut, &vCur[3], &matLocalToLight );
	if( (float)fabs(VOut.x / VOut.z)	> fXMax )	fXMax = (float)fabs(VOut.x/VOut.z);
	if( (float)fabs(VOut.y / VOut.z)	> fYMax )	fYMax = (float)fabs(VOut.y/VOut.z);

	//======그림자 프로젝션 프로젝션 행렬의 _11, _22는 Z가 1인 뷰 평면에서의 최대 X, Y값이다.
	D3DXMatrixPerspectiveFovLH( &m_matShadowProj, _X_RAD(g_LodTerrain.m_3PCamera.mp_fFov), 1.0f, 1.0f, 1000.0f );
	//m_matShadowProj._11	= (0.96f/fXMax);
	//m_matShadowProj._22	= (0.96f/fYMax);
	m_matShadowProj._11	= (1.0f/fXMax);
	m_matShadowProj._22	= (1.0f/fYMax);

	m_matLocalToWorld = m_Position;
}
*/

void _XModelDescriptor::GenerateShadowMap( void )
{	
	D3DXMATRIX		matLocalToWorld;
	D3DXMATRIX		matWorldToLight;
	D3DXMATRIX		matShadowProj;
	
	D3DXMatrixIdentity( &m_matLocalToWorld );
	D3DXMatrixIdentity( &m_matWorldToLight );
	D3DXMatrixIdentity( &matShadowProj   );
	
	D3DXVECTOR3 vBlockerCenter;
	vBlockerCenter.x	= m_Position._41;
	vBlockerCenter.y	= m_Position._42 + m_BlockerCenterHeight;
	vBlockerCenter.z	= m_Position._43;
	
	D3DXVECTOR3		vOrig	= g_EnvironmentManager.m_ObjectLight.Position;
	D3DXVECTOR3		vEnd	= vBlockerCenter;
	
	D3DXVECTOR3		vLightPo;
	vLightPo    = ( vEnd - vOrig );	
	vLightPo	= vOrig + 0.9999f * ( vEnd - vOrig );
	
	D3DXVECTOR3		vecFrom = (vLightPo - vBlockerCenter);
	D3DXVec3Normalize( &vecFrom, &vecFrom );
	vecFrom *= 20.0f;	
	vecFrom.y += 30.0f;
	vecFrom = vBlockerCenter + vecFrom;
	
	float			fXMax		= 0.0f;
	float			fYMax		= 0.0f;
	D3DXVECTOR3		vCur[4];
	D3DXVECTOR4		VOut;
	
	vCur[0].x = vBlockerCenter.x - m_ShadowScaleFactor;
	vCur[0].z = vBlockerCenter.z - m_ShadowScaleFactor;
	vCur[0].y = vBlockerCenter.y;
	
	vCur[1].x = vBlockerCenter.x + m_ShadowScaleFactor;
	vCur[1].z = vBlockerCenter.z - m_ShadowScaleFactor;
	vCur[1].y = vBlockerCenter.y;
	
	vCur[2].x = vBlockerCenter.x - m_ShadowScaleFactor;
	vCur[2].z = vBlockerCenter.z + m_ShadowScaleFactor;
	vCur[2].y = vBlockerCenter.y;
	
	vCur[3].x = vBlockerCenter.x + m_ShadowScaleFactor;
	vCur[3].z = vBlockerCenter.z + m_ShadowScaleFactor;
	vCur[3].y = vBlockerCenter.y;	
	
	D3DXMatrixLookAtLH( &m_matWorldToLight, &vecFrom, &vBlockerCenter, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
	
	D3DXMATRIX matLocalToLight; // [그림자 객체 좌표] -> [광원 좌표] 행렬
	D3DXMatrixMultiply( &matLocalToLight, &m_matLocalToWorld, &m_matWorldToLight );
	
	D3DXVec3Transform( &VOut, &vCur[0], &matLocalToLight );
	fXMax = (float)fabs(VOut.x/VOut.z);
	fYMax = (float)fabs(VOut.y/VOut.z);
	
	D3DXVec3Transform( &VOut, &vCur[1], &matLocalToLight );
	if( (float)fabs(VOut.x / VOut.z)	> fXMax )	fXMax = (float)fabs(VOut.x/VOut.z);
	if( (float)fabs(VOut.y / VOut.z)	> fYMax )	fYMax = (float)fabs(VOut.y/VOut.z);
	
	D3DXVec3Transform( &VOut, &vCur[2], &matLocalToLight );
	if( (float)fabs(VOut.x / VOut.z)	> fXMax )	fXMax = (float)fabs(VOut.x/VOut.z);
	if( (float)fabs(VOut.y / VOut.z)	> fYMax )	fYMax = (float)fabs(VOut.y/VOut.z);
	
	D3DXVec3Transform( &VOut, &vCur[3], &matLocalToLight );
	if( (float)fabs(VOut.x / VOut.z)	> fXMax )	fXMax = (float)fabs(VOut.x/VOut.z);
	if( (float)fabs(VOut.y / VOut.z)	> fYMax )	fYMax = (float)fabs(VOut.y/VOut.z);
	
	//======그림자 프로젝션 프로젝션 행렬의 _11, _22는 Z가 1인 뷰 평면에서의 최대 X, Y값이다.
	D3DXMatrixPerspectiveFovLH( &m_matShadowProj, _X_RAD(g_LodTerrain.m_3PCamera.mp_fFov), 1.0f, 1.0f, 1000.0f );
	m_matShadowProj._11	= (1.0f/fXMax);
	m_matShadowProj._22	= (1.0f/fYMax);
	
	m_matLocalToWorld = m_Position;
}


void _XModelDescriptor::RenderObjectShadow( void )
{
	GenerateShadowMap();
	
	g_DefaultRenderStyleManager.SetRenderStyle( NULL, _XDEF_DEFAULTRENDERSTYLE_RENDERSHADOWMAP );
	
	LPDIRECT3DSURFACE9	prevrenderingtarget = NULL;
	gpDev->GetRenderTarget( 0, &prevrenderingtarget );

	gpDev->EndScene();

	gpDev->SetRenderTarget( 0, g_lpShadowSurface );	
	gpDev->BeginScene();
	
	gpDev->Clear( 0, NULL, D3DCLEAR_TARGET , g_ShadowTextureClearColor, 0, 0 ); 
	
	gpDev->SetRenderState( D3DRS_FOGENABLE,	FALSE );	
	gpDev->SetRenderState( D3DRS_TEXTUREFACTOR, g_ShadowTextureRenderColor );
	
	// 변환 행렬 설정.
	// [그림자 객체 좌표] -> [월드좌표] -> [광원좌표] -> [텍스쳐 좌표]	
	gpDev->SetTransform( D3DTS_VIEW,		&m_matWorldToLight );
	gpDev->SetTransform( D3DTS_PROJECTION,	&m_matShadowProj );
	
	for( int j = 0; j < m_ModelStackSize; j++ )
	{			
		if( m_ppModelList[j] )
		{
			m_ppModelList[j]->RenderShadow_LODCheck( this );
		}
	}
	
	gpDev->EndScene();
		
	gpDev->SetRenderTarget( 0, prevrenderingtarget );
	SAFE_RELEASE( prevrenderingtarget );
	
	gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());	
	gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
	gpDev->BeginScene();	
}

void _XModelDescriptor::SetLastFrame( void )
{
	if( !m_pBipedController ) return;

	_LPXM_MOTION pMotion = m_pBipedController->GetMotion(m_CurMotion);

	if( pMotion )
	{		
		Animation((FLOAT)pMotion->EndFrame);
	}	
}

void _XModelDescriptor::SetCurMotion( int motionindex, bool changemotion, bool changesamemotion)
{
	if(!m_pBipedController) return;
	if( motionindex < 0 || m_pBipedController->GetMotionCnt() <= motionindex) return;

	if(!changesamemotion)
	{
		if( m_CurMotion == motionindex ) 
			return;
	}

	m_SlerpAniFrame		= 0.0f;
	m_PrevMotion		= m_CurMotion;
	m_PrevMotionFrame	= m_AniFrame;

	m_CurMotion			= motionindex;
	m_MotionChangeFlag	= changemotion;
	/*m_NextMotion			= motionindex;
	m_MotionChangingFlag	= TRUE;//changemotion;*/
	
	//m_AniFrame			= 0.0f;
	_LPXM_MOTION pMotion = m_pBipedController->GetMotion( m_CurMotion );

	if( pMotion )
	{
		m_AniFrame = (float)pMotion->StartFrame;
	}

	memset( &m_EventHandler, 0, sizeof(_XANIEVENT_STRUCTURE) );

	if( m_EffectManager )
	{
		// 이미 존재하는 이펙트를 페이드 모드로 진입시킨다.
		if( m_pBipedController->m_BaseEffectIndex >= 0 )
		{
			m_EffectManager->StartFade_Except( m_pBipedController->m_BaseEffectIndex );
		}
		else
		{
			m_EffectManager->StartFade();
		}
	}
}


void _XModelDescriptor::ResetEventHandler( void )
{
	memset( &m_EventHandler, 0, sizeof(_XANIEVENT_STRUCTURE) );
}

#define _XDEF_USESLERPANIMATION	

void _XModelDescriptor::Advance_NoCheckEvent( FLOAT fElapsedTime )
{
	if( !m_pBipedController ) return;
	if( !m_AniFlag || m_pBipedController->GetMotionCnt() == 0) return;

	m_FrameResult = _XDEF_ACI_NONE;

#ifdef _XDEF_USESLERPANIMATION	
	if(m_MotionChangeFlag)
	{
	}
	else
#endif		
	{
		m_pBipedController->Advance_NoCheckEvent( this, fElapsedTime );
	}
}

void _XModelDescriptor::Advance( bool& playsound, int eventlevel, FLOAT particlesize )
{
	if( !m_pBipedController ) return;
	if( !m_AniFlag || m_pBipedController->GetMotionCnt() == 0) return;
	
	m_FrameResult = _XDEF_ACI_NONE;
	m_AttackClass = _XDEF_ATTACKCLASS_LOW;

#ifdef _XDEF_USESLERPANIMATION	
	if(m_MotionChangeFlag)
	{
	}
	else
#endif		
	{
		m_pBipedController->Advance( this, playsound, eventlevel, particlesize );
	}
}

void _XModelDescriptor::Animation( void )
{
	if( m_NoAnimationAndPresentPipeline ) return;

	if( !m_pBipedController ) return;
	if( !m_AniFlag || m_pBipedController->GetMotionCnt() == 0) return;
	
#ifdef _XDEF_USESLERPANIMATION	
	if(m_MotionChangeFlag)
	{
		m_pBipedController->SlerpAnimation(this);

		for( int i = 0; i < m_ModelStackSize; i++ )
		{
			if( m_ppModelList[i] ) m_ppModelList[i]->SlerpAnimation( this );
		}
	}
	else
#endif

	{
		m_pBipedController->FrameAnimation(this);

		for( int i = 0; i < m_ModelStackSize; i++ )
		{
			if( m_ppModelList[i] ) m_ppModelList[i]->FrameAnimation( this );
		}		
	}
}

void _XModelDescriptor::Animation( FLOAT frame )
{
	if( m_NoAnimationAndPresentPipeline ) return;
	if( !m_pBipedController ) return;
	if( m_pBipedController->GetMotionCnt() == 0 ) return;
	

	_LPXM_MOTION pMotion =  m_pBipedController->GetMotion( m_CurMotion );

	if( pMotion->StartFrame > frame )
	{
		frame = pMotion->StartFrame;
	}
	else if( pMotion->EndFrame < frame )
	{
		frame = pMotion->EndFrame;
	}

	m_AniFrame = frame;
	
	//m_FrameResult = _XDEF_ACI_NONE;
	
	m_pBipedController->FrameAnimation(this, frame);
		
	for( int i = 0; i < m_ModelStackSize; i++ )
	{
		if( m_ppModelList[i] ) m_ppModelList[i]->FrameAnimation( this );
	}
	m_FrameResult = _XDEF_ACI_NONE;
}

void _XModelDescriptor::Render_Reflect( D3DXMATRIX& viewmat )
{
	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
	gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

	for( int i = 0; i < m_ModelStackSize; i++ )
	{
		if( m_ppModelList[i] )
		{
			m_ppModelList[i]->Render_Reflect( this, &viewmat );
		}	
	}
	gpDev->SetLight( 0, &g_EnvironmentManager.m_ObjectLight );
}

void _XModelDescriptor::Render_SecondBuffer( void )
{
	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
	gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

	for( int i = 0; i < m_ModelStackSize; i++ )
	{
		if( m_ppModelList[i] )
		{
			m_ppModelList[i]->Render_SecondBuffer( this );			
		}	
	}
}

void _XModelDescriptor::RenderFunctionalObject( _XFunctionalObject* pFunctionalObject )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

		for( int i = 0; i < m_ModelStackSize; i++ )
		{
			if( m_ppModelList[i] )
			{
				m_ppModelList[i]->Render_LODCheck( this );
			}	
		}
	}
		
	if( m_EffectManager && pFunctionalObject )
	{
		g_RenderAlphaBlendFlag = FALSE;
		g_pCurrentSelectMaterial = NULL;

		if( pFunctionalObject->m_DefenderID > 0 )
		{
			_XModelDescriptor* pTargetModelDescryptor = NULL;

			if( pFunctionalObject->m_DefenderType == 0 ) // Monster
			{
				_XMob* pMob = g_Moblist.FindMob( pFunctionalObject->m_DefenderID );

				if( pMob )
				{
					pTargetModelDescryptor = &(pMob->m_ModelDescriptor);
				}
				
				m_EffectManager->Update(this, NULL, NULL, NULL, pTargetModelDescryptor );
			}
			else if( pFunctionalObject->m_DefenderType == 1 ) // User
			{
				if( g_pLocalUser->m_Uniqueid == pFunctionalObject->m_DefenderID ) // to local user
				{
					pTargetModelDescryptor = &(g_pLocalUser->m_ModelDescriptor);
				}
				else
				{
					_XUser* pUser = g_Userlist.FindUser( pFunctionalObject->m_DefenderID );
					if( pUser )
					{
						pTargetModelDescryptor = &(pUser->m_ModelDescriptor);
					}
				}
				
				m_EffectManager->Update(this, NULL, NULL, NULL, pTargetModelDescryptor );
			}
		}
		else
		{
			m_EffectManager->Update( this, NULL, NULL, NULL );
		}
		
		if( !m_NoAnimationAndPresentPipeline )
		{	
			m_EffectManager->Render();
		}
	}
	
#ifdef _XDWDEBUG	
/*	extern _XMeshContainer		g_AxisModel;

	if( m_EnableEffectProcess )
	{
		D3DXMatrixMultiply( &m_PartMatrixContainer[0], &m_PartMatrixContainer[0], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[1], &m_PartMatrixContainer[1], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[2], &m_PartMatrixContainer[2], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[3], &m_PartMatrixContainer[3], &m_Position );

		g_AxisModel.Render( m_PartMatrixContainer[0] );
		g_AxisModel.Render( m_PartMatrixContainer[1] );
		g_AxisModel.Render( m_PartMatrixContainer[2] );
		g_AxisModel.Render( m_PartMatrixContainer[3] );
	}*/
#endif

}


void _XModelDescriptor::RenderMob( _XMob* pMob )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

		for( int i = 0; i < m_ModelStackSize; i++ )
		{
			if( m_ppModelList[i] )
			{
				m_ppModelList[i]->Render_LODCheck( this );
			}	
		}
	}
		
	if( m_EffectManager )
	{
		g_RenderAlphaBlendFlag = FALSE;
		g_pCurrentSelectMaterial = NULL;
		
		FLOAT obbheight = 0.0f;
		if( m_ppModelList[0] ) obbheight = m_ppModelList[0]->OBB_Extent[1] * 2.0f;
		
		if( pMob->m_AttackTarget )
		{
			_XModelDescriptor* pTargetModelDescryptor = NULL;
			
			switch( pMob->m_AttackTargetObjectType ) 
			{
			case _XPICKINGTARGET_SELF	:
			case _XPICKINGTARGET_PC		:
				pTargetModelDescryptor = &((_XUser*)pMob->m_AttackTarget)->m_ModelDescriptor;
				break;
			case _XPICKINGTARGET_MOB	:
				pTargetModelDescryptor = &((_XMob*)pMob->m_AttackTarget)->m_ModelDescriptor;
				break;
			case _XPICKINGTARGET_NPC	:
				pTargetModelDescryptor = &((_XNPCObject*)pMob->m_AttackTarget)->m_ModelDescriptor;
				break;
			}
			m_EffectManager->Update(this, NULL, NULL, NULL, pTargetModelDescryptor, obbheight );
		}
		else
		{
			m_EffectManager->Update( this, NULL, NULL, NULL, NULL, obbheight );
		}
		if( !m_NoAnimationAndPresentPipeline )
		{	
			m_EffectManager->Render();
		}
	}
	
#ifdef _XDWDEBUG	
/*	extern _XMeshContainer		g_AxisModel;

	if( m_EnableEffectProcess )
	{
		D3DXMatrixMultiply( &m_PartMatrixContainer[0], &m_PartMatrixContainer[0], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[1], &m_PartMatrixContainer[1], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[2], &m_PartMatrixContainer[2], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[3], &m_PartMatrixContainer[3], &m_Position );

		g_AxisModel.Render( m_PartMatrixContainer[0] );
		g_AxisModel.Render( m_PartMatrixContainer[1] );
		g_AxisModel.Render( m_PartMatrixContainer[2] );
		g_AxisModel.Render( m_PartMatrixContainer[3] );
	}*/
#endif	
}


void _XModelDescriptor::Render( void )
{	
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

		for( int i = 0; i < m_ModelStackSize; i++ )
		{
			if( m_ppModelList[i] )
			{
				m_ppModelList[i]->Render_LODCheck( this );
			}	
		}
	}
		
	if( m_EffectManager )
	{
		FLOAT obbheight = 0.0f;
		if( m_ppModelList[0] ) obbheight = m_ppModelList[0]->OBB_Extent[1] * 2.0f;

		g_RenderAlphaBlendFlag = FALSE;
		g_pCurrentSelectMaterial = NULL;
		m_EffectManager->Update(this, NULL, NULL, NULL, NULL, obbheight );

		if( !m_NoAnimationAndPresentPipeline )
		{
			m_EffectManager->Render();
		}
	}
	
#ifdef _XDWDEBUG	
/*	extern _XMeshContainer		g_AxisModel;

	if( m_EnableEffectProcess )
	{
		D3DXMatrixMultiply( &m_PartMatrixContainer[0], &m_PartMatrixContainer[0], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[1], &m_PartMatrixContainer[1], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[2], &m_PartMatrixContainer[2], &m_Position );
		D3DXMatrixMultiply( &m_PartMatrixContainer[3], &m_PartMatrixContainer[3], &m_Position );

		g_AxisModel.Render( m_PartMatrixContainer[0] );
		g_AxisModel.Render( m_PartMatrixContainer[1] );
		g_AxisModel.Render( m_PartMatrixContainer[2] );
		g_AxisModel.Render( m_PartMatrixContainer[3] );
	}*/
#endif	


}

void _XModelDescriptor::Render( D3DXMATRIX& worldmat )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

		for( int i = 0; i < m_ModelStackSize; i++ )
		{
			if( m_ppModelList[i] )
			{
				m_ppModelList[i]->Render( this, &worldmat );
			}	
		}	
	}

	if( m_EffectManager )
	{
		FLOAT obbheight = 0.0f;
		if( m_ppModelList[0] ) obbheight = m_ppModelList[0]->OBB_Extent[1] * 2.0f;
		m_EffectManager->Update(this, NULL, NULL, NULL, NULL, obbheight);
		if( !m_NoAnimationAndPresentPipeline )
		{
			m_EffectManager->Render();
		}
	}
}

void _XModelDescriptor::RenderNoTexture( D3DXMATRIX& worldmat )
{
	//gpDev->SetRenderState( D3DRS_AMBIENT, g_LodTerrain.m_CharacterAmbientColor );
	//gpDev->SetLight( 0, &g_LodTerrain.m_CharacterLight );

	for( int i = 0; i < m_ModelStackSize; i++ )
	{
		if( m_ppModelList[i] )
		{
			m_ppModelList[i]->RenderNoTexture( this, &worldmat );
		}	
	}	
}

void _XModelDescriptor::RenderCustomDraw( D3DXMATRIX& worldmat )
{		
	//gpDev->SetRenderState( D3DRS_AMBIENT, g_LodTerrain.m_CharacterAmbientColor );
	//gpDev->SetLight( 0, &g_LodTerrain.m_CharacterLight );

	for( int i = 0; i < m_ModelStackSize; i++ )
	{
		if( m_ppModelList[i] )
		{
			m_ppModelList[i]->RenderCustomDraw( this, &worldmat );
		}	
	}	
}

void _XModelDescriptor::RenderCharacterModel_RadialBlur( _XUser* pUser, BOOL uselowlevelLOD )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );
		
		if( uselowlevelLOD )
		{
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACE]		)	m_ppModelList[_XDEF_MODELCLUSTER_FACE		]->Render_LowLevelLOD( this );
			if( m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY  ]->Render_LowLevelLOD( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY  ]->Render_LowLevelLOD( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_LHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_LHAND 		]->Render_LowLevelLOD( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_RHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_RHAND 		]->Render_LowLevelLOD( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_LFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_LFOOT 		]->Render_LowLevelLOD( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_RFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_RFOOT 		]->Render_LowLevelLOD( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ADAE]		)	m_ppModelList[_XDEF_MODELCLUSTER_ADAE 		]->Render_LowLevelLOD( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_EARRING]	)	m_ppModelList[_XDEF_MODELCLUSTER_EARRING	]->Render_LowLevelLOD( this );				
			//if( m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK]	)	m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK   ]->Render_LowLevelLOD( this );

			if( m_ppModelList[_XDEF_MODELCLUSTER_MASK]		)	m_ppModelList[_XDEF_MODELCLUSTER_MASK		]->Render_LowLevelLOD( this );	
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK]	)	m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK	]->Render_LowLevelLOD( this );	

			if( m_ppModelList[_XDEF_MODELCLUSTER_CAP]		)	m_ppModelList[_XDEF_MODELCLUSTER_CAP		]->Render_LowLevelLOD( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM1]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM1		]->Render( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM2]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM2		]->Render( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM3]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM3		]->Render( this );
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM4]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM4		]->Render( this );
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM5]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM5		]->Render( this );
			
			g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
			if( m_ppModelList[_XDEF_MODELCLUSTER_HAIR]		)	m_ppModelList[_XDEF_MODELCLUSTER_HAIR		]->Render_LowLevelLOD( this );
		}
		else
		{
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACE]		)	m_ppModelList[_XDEF_MODELCLUSTER_FACE		]->Render_LODCheck( this );
			if( m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY  ]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY  ]->Render_LODCheck( this );
			//if( m_ppModelList[_XDEF_MODELCLUSTER_LHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_LHAND 		]->Render_LODCheck( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_RHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_RHAND 		]->Render_LODCheck( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_LFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_LFOOT 		]->Render_LODCheck( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_RFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_RFOOT 		]->Render_LODCheck( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ADAE]		)	m_ppModelList[_XDEF_MODELCLUSTER_ADAE 		]->Render( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_EARRING]	)	m_ppModelList[_XDEF_MODELCLUSTER_EARRING	]->Render_LODCheck( this );				
			//if( m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK]	)	m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK   ]->Render_LODCheck( this );

			if( m_ppModelList[_XDEF_MODELCLUSTER_MASK]		)	m_ppModelList[_XDEF_MODELCLUSTER_MASK		]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK]	)	m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK	]->Render_LODCheck( this );			

			if( m_ppModelList[_XDEF_MODELCLUSTER_CAP]		)	m_ppModelList[_XDEF_MODELCLUSTER_CAP		]->Render_LODCheck( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM1]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM1		]->Render( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM2]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM2		]->Render( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM3]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM3		]->Render( this );
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM4]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM4		]->Render( this );
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM5]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM5		]->Render( this );
			
			g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
			if( m_ppModelList[_XDEF_MODELCLUSTER_HAIR]		)	m_ppModelList[_XDEF_MODELCLUSTER_HAIR		]->Render_LODCheck( this );		
		}
	}
	
	g_RenderAlphaBlendFlag = FALSE;
}

void _XModelDescriptor::RenderCharacterModel( _XUser* pUser, BOOL uselowlevelLOD )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );
		
		if( uselowlevelLOD )
		{
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACE]		)	m_ppModelList[_XDEF_MODELCLUSTER_FACE		]->Render_LowLevelLOD( this );						
			if( m_ppModelList[_XDEF_MODELCLUSTER_LHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_LHAND 		]->Render_LowLevelLOD( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_RHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_RHAND 		]->Render_LowLevelLOD( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_LFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_LFOOT 		]->Render_LowLevelLOD( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_RFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_RFOOT 		]->Render_LowLevelLOD( this );
			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ADAE]		)	m_ppModelList[_XDEF_MODELCLUSTER_ADAE 		]->Render_LowLevelLOD( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_EARRING]	)	m_ppModelList[_XDEF_MODELCLUSTER_EARRING	]->Render_LowLevelLOD( this );							
			if( m_ppModelList[_XDEF_MODELCLUSTER_CAP]		)	m_ppModelList[_XDEF_MODELCLUSTER_CAP		]->Render_LowLevelLOD( this );			

			if( m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY  ]->Render_LowLevelLOD( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY  ]->Render_LowLevelLOD( this );			

			if( m_ppModelList[_XDEF_MODELCLUSTER_MASK]		)	m_ppModelList[_XDEF_MODELCLUSTER_MASK		]->Render_LowLevelLOD( this );
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK]	)	m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK	]->Render_LowLevelLOD( this );
			if( m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK]	)	m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK   ]->Render_LowLevelLOD( this );

			if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM1]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM1		]->Render( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM2]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM2		]->Render( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM3]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM3		]->Render( this );
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM4]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM4		]->Render( this );
			if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM5]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM5		]->Render( this );
			
			g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
			if( m_ppModelList[_XDEF_MODELCLUSTER_HAIR]		)	m_ppModelList[_XDEF_MODELCLUSTER_HAIR		]->Render_LowLevelLOD( this );
		}
		else
		{
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACE]		)	m_ppModelList[_XDEF_MODELCLUSTER_FACE		]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_LHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_LHAND 		]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_RHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_RHAND 		]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_LFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_LFOOT 		]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_RFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_RFOOT 		]->Render_LODCheck( this );
			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ADAE]		)	m_ppModelList[_XDEF_MODELCLUSTER_ADAE 		]->Render( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_EARRING]	)	m_ppModelList[_XDEF_MODELCLUSTER_EARRING	]->Render_LODCheck( this );							
			if( m_ppModelList[_XDEF_MODELCLUSTER_CAP]		)	m_ppModelList[_XDEF_MODELCLUSTER_CAP		]->Render_LODCheck( this );			

			if( m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY  ]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY  ]->Render_LODCheck( this );

			if( m_ppModelList[_XDEF_MODELCLUSTER_MASK]		)	m_ppModelList[_XDEF_MODELCLUSTER_MASK		]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK]	)	m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK	]->Render_LODCheck( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK]	)	m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK   ]->Render_LODCheck( this );

			if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM1]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM1		]->Render( this );			
			if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM2]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM2		]->Render( this );			
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM3]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM3		]->Render( this );
			//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM4]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM4		]->Render( this );
			if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM5]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM5		]->Render( this );
			
			g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
			if( m_ppModelList[_XDEF_MODELCLUSTER_HAIR]		)	m_ppModelList[_XDEF_MODELCLUSTER_HAIR		]->Render_LODCheck( this );		
		}
	}
	
	g_RenderAlphaBlendFlag = FALSE;
}

void _XModelDescriptor::RenderCharacter( _XCHARACTER_INFO& UserCharacterInfo )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );
		
		if( m_ppModelList[_XDEF_MODELCLUSTER_FACE]		)	m_ppModelList[_XDEF_MODELCLUSTER_FACE		]->Render_LODCheck( this );				
		if( m_ppModelList[_XDEF_MODELCLUSTER_LHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_LHAND 		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_RHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_RHAND 		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_LFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_LFOOT 		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_RFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_RFOOT 		]->Render_LODCheck( this );		
		
		//if( m_ppModelList[_XDEF_MODELCLUSTER_ADAE]		)	m_ppModelList[_XDEF_MODELCLUSTER_ADAE 		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_EARRING]	)	m_ppModelList[_XDEF_MODELCLUSTER_EARRING	]->Render_LODCheck( this );				
						
		if( m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY  ]->Render_LODCheck( this );		
		if( m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY  ]->Render_LODCheck( this );			

		if( m_ppModelList[_XDEF_MODELCLUSTER_MASK]		)	m_ppModelList[_XDEF_MODELCLUSTER_MASK		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK]	)	m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK	]->Render_LODCheck( this );			

		if( m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK]	)	m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK   ]->Render_LODCheck( this );

		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM1]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM1		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM2]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM2		]->Render( this );			
		//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM3]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM3		]->Render( this );
		//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM4]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM4		]->Render( this );
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM5]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM5		]->Render( this );

		g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
		if( m_ppModelList[_XDEF_MODELCLUSTER_HAIR]		)	m_ppModelList[_XDEF_MODELCLUSTER_HAIR		]->Render_LODCheck( this );		

		if( m_ppModelList[_XDEF_MODELCLUSTER_CAP]		)	m_ppModelList[_XDEF_MODELCLUSTER_CAP		]->Render_LODCheck( this );			
	}

	g_RenderAlphaBlendFlag = FALSE;
	//g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
	gpDev->SetTransform( D3DTS_WORLD, &m_Position );
	gpDev->SetVertexShader( NULL );

	if( !m_NoAnimationAndPresentPipeline )
	{	
		int genderindex = ( UserCharacterInfo.gender == _XGENDER_MAN ) ? 0 : 2;
		
		if( !m_MotionChangeFlag )
		{
#ifdef _XDEF_MODELTHREADLOADING	
			if( (m_SwordEffectCount > 0 ) && m_ViewLeftSwordEffect && UserCharacterInfo.modeldescript.item1 != 0 )
#else
			if( m_pSwordEffectInstanceObject && m_ViewLeftSwordEffect && UserCharacterInfo.modeldescript.item1 != 0 )
#endif
			{	
				int referencematrix = -64651;
				D3DXVECTOR3 toppos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 bottompos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(UserCharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(UserCharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[UserCharacterInfo.modeldescript.item1].cSecondType )
						{			
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[UserCharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								referencematrix = 0;
							}
							else
							{
								referencematrix = 4;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							referencematrix = 5;
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							referencematrix = 0;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
							referencematrix = 0;
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							referencematrix = 0;
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							referencematrix = 0;
							break;
						}			
						
						toppos = WeaponItem[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
						bottompos = WeaponItem[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
					}
				}
#else
				if( UserCharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[UserCharacterInfo.modeldescript.item1].cSecondType )
					{			
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[UserCharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							referencematrix = 0;
						}
						else
						{
							referencematrix = 4;
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						referencematrix = 5;
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						referencematrix = 0;
						break;
					}			
					
					toppos = g_WeaponItemProperty[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
					bottompos = g_WeaponItemProperty[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
				}
				else if( UserCharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[UserCharacterInfo.modeldescript.item1].cSecondType )
					{			
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[UserCharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							referencematrix = 0;
						}
						else
						{
							referencematrix = 4;
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						referencematrix = 5;
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						referencematrix = 0;
						break;
					}			
					
					toppos = g_WeaponItemProperty2[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
					bottompos = g_WeaponItemProperty2[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
				}
				else if( UserCharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[UserCharacterInfo.modeldescript.item1].cSecondType )
					{			
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[UserCharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							referencematrix = 0;
						}
						else
						{
							referencematrix = 4;
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						referencematrix = 5;
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						referencematrix = 0;
						break;
					}			
					
					toppos = g_WeaponItemProperty3[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
					bottompos = g_WeaponItemProperty3[UserCharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
				}
#endif

				//D3DXMATRIX mat;
				//D3DXMatrixMultiply( &mat, &m_PartMatrixContainer[0], &m_Position );		 
				D3DXVec3TransformCoord( &toppos, &toppos, &m_PartMatrixContainer[referencematrix] );
				D3DXVec3TransformCoord( &bottompos, &bottompos, &m_PartMatrixContainer[referencematrix] );
				
				if( !m_SwordEffectInitialized )
				{
					m_SwordEffectInitialized = TRUE;

					m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;
					
					m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;
				}
				
				if( m_FrameResult & _XDEF_ACI_FIRSTFRAME )
				{
					m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;

					m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;
				}
				else
				{
					//if( !((int)m_AniFrame % 3) )
					//if( ((int)m_AniFrame & 0x01) )
					{
						m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = m_pSwordEffectInstanceObject[0].ControlPoint[0][1];
						m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = m_pSwordEffectInstanceObject[0].ControlPoint[0][2];
						m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = m_pSwordEffectInstanceObject[0].ControlPoint[0][3];

						m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = m_pSwordEffectInstanceObject[0].ControlPoint[1][1];
						m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = m_pSwordEffectInstanceObject[0].ControlPoint[1][2];
						m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = m_pSwordEffectInstanceObject[0].ControlPoint[1][3];
					}
					
					m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;			

					g_SwordEffectController.Process(m_pSwordEffectInstanceObject);
					g_SwordEffectController.Render( _XDEF_DEFAULTRENDERSTYLE_CHARACTERSWORDEFFECT );
				}			
			}
		}
	
		if( m_EffectManager )
		{
			D3DXVECTOR3 itempos[3];
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(UserCharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(UserCharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					itempos[0] = WeaponItem[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
					itempos[1] = WeaponItem[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
				}
			}
#else
			if( UserCharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				itempos[0] = g_WeaponItemProperty[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
				itempos[1] = g_WeaponItemProperty[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
			}
			else if( UserCharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				itempos[0] = g_WeaponItemProperty2[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
				itempos[1] = g_WeaponItemProperty2[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
			}
			else if( UserCharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				itempos[0] = g_WeaponItemProperty3[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
				itempos[1] = g_WeaponItemProperty3[ UserCharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
			}
#endif
			m_EffectManager->Update(this, NULL, NULL, itempos, NULL, 1.35f );
			
			if( !m_NoAnimationAndPresentPipeline )
			{
				m_EffectManager->Render();
			}
		}	
	}
}

void _XModelDescriptor::RenderCharacter(  _XUser* pUser )
{		
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );
		
		if( m_ppModelList[_XDEF_MODELCLUSTER_FACE]		)	m_ppModelList[_XDEF_MODELCLUSTER_FACE		]->Render_LODCheck( this );				
		if( m_ppModelList[_XDEF_MODELCLUSTER_LHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_LHAND 		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_RHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_RHAND 		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_LFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_LFOOT 		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_RFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_RFOOT 		]->Render_LODCheck( this );		
		
		//if( m_ppModelList[_XDEF_MODELCLUSTER_ADAE]		)	m_ppModelList[_XDEF_MODELCLUSTER_ADAE 		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_EARRING]	)	m_ppModelList[_XDEF_MODELCLUSTER_EARRING	]->Render_LODCheck( this );						
				
		if( m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY  ]->Render_LODCheck( this );		
		if( m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY  ]->Render_LODCheck( this );			

		if( m_ppModelList[_XDEF_MODELCLUSTER_MASK]		)	m_ppModelList[_XDEF_MODELCLUSTER_MASK		]->Render_LODCheck( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK]	)	m_ppModelList[_XDEF_MODELCLUSTER_FACEMASK	]->Render_LODCheck( this );			

		if( m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK]	)	m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK   ]->Render_LODCheck( this );

		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM1]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM1		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM2]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM2		]->Render( this );			
		//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM3]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM3		]->Render( this );
		//if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM4]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM4		]->Render( this );
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM5]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM5		]->Render( this );

		g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
		if( m_ppModelList[_XDEF_MODELCLUSTER_HAIR]		)	m_ppModelList[_XDEF_MODELCLUSTER_HAIR		]->Render_LODCheck( this );		

		if( m_ppModelList[_XDEF_MODELCLUSTER_CAP]		)	m_ppModelList[_XDEF_MODELCLUSTER_CAP		]->Render_LODCheck( this );
	}

	g_RenderAlphaBlendFlag = FALSE;
	//g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
	gpDev->SetTransform( D3DTS_WORLD, &m_Position );
	gpDev->SetVertexShader( NULL );

	if( pUser && !m_NoAnimationAndPresentPipeline )
	{	
		int genderindex = ( pUser->m_CharacterInfo.gender == _XGENDER_MAN ) ? 0 : 2;
		
		if( !m_MotionChangeFlag )
		{
#ifdef _XDEF_MODELTHREADLOADING
			if( (m_SwordEffectCount > 0 ) && m_ViewLeftSwordEffect && pUser->m_CharacterInfo.modeldescript.item1 != 0 )
#else
			if( m_pSwordEffectInstanceObject && m_ViewLeftSwordEffect && pUser->m_CharacterInfo.modeldescript.item1 != 0 )
#endif			
			{	
				int referencematrix = -64651;
				D3DXVECTOR3 toppos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 bottompos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(pUser->m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(pUser->m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cSecondType )
						{			
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								referencematrix = 0;
							}
							else
							{
								referencematrix = 4;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							referencematrix = 5;
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							referencematrix = 0;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
							referencematrix = 0;
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							referencematrix = 0;
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							referencematrix = 0;
							break;
						}			
						
						toppos = WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
						bottompos = WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
					}
				}
#else
				if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cSecondType )
					{			
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							referencematrix = 0;
						}
						else
						{
							referencematrix = 4;
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						referencematrix = 5;
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						referencematrix = 0;
						break;
					}			
					
					toppos = g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
					bottompos = g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
				}
				else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cSecondType )
					{			
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							referencematrix = 0;
						}
						else
						{
							referencematrix = 4;
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						referencematrix = 5;
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						referencematrix = 0;
						break;
					}			
					
					toppos = g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
					bottompos = g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
				}
				else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cSecondType )
					{			
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							referencematrix = 0;
						}
						else
						{
							referencematrix = 4;
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						referencematrix = 5;
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						referencematrix = 0;
						break;
					}			
					
					toppos = g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex];
					bottompos = g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].vecItemPosition[genderindex+1];
				}
#endif

				//D3DXMATRIX mat;
				//D3DXMatrixMultiply( &mat, &m_PartMatrixContainer[0], &m_Position );		 
				D3DXVec3TransformCoord( &toppos, &toppos, &m_PartMatrixContainer[referencematrix] );
				D3DXVec3TransformCoord( &bottompos, &bottompos, &m_PartMatrixContainer[referencematrix] );
				
				if( !m_SwordEffectInitialized )
				{
					m_SwordEffectInitialized = TRUE;

					m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;
					
					m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;
				}
				
				if( m_FrameResult & _XDEF_ACI_FIRSTFRAME )
				{
					m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;

					m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = bottompos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;
				}
				else
				{
					//if( !((int)m_AniFrame % 3) )
					//if( ((int)m_AniFrame & 0x01) )
					{
						m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = m_pSwordEffectInstanceObject[0].ControlPoint[0][1];
						m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = m_pSwordEffectInstanceObject[0].ControlPoint[0][2];
						m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = m_pSwordEffectInstanceObject[0].ControlPoint[0][3];

						m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = m_pSwordEffectInstanceObject[0].ControlPoint[1][1];
						m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = m_pSwordEffectInstanceObject[0].ControlPoint[1][2];
						m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = m_pSwordEffectInstanceObject[0].ControlPoint[1][3];
					}
					
					m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;
					m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;			

					g_SwordEffectController.Process(m_pSwordEffectInstanceObject);
					g_SwordEffectController.Render( _XDEF_DEFAULTRENDERSTYLE_CHARACTERSWORDEFFECT );
				}			
			}
		}
	
		if( m_EffectManager )
		{
			D3DXVECTOR3 itempos[3];
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(pUser->m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(pUser->m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					itempos[0] = WeaponItem[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
					itempos[1] = WeaponItem[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
				}
			}
#else
			if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				itempos[0] = g_WeaponItemProperty[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
				itempos[1] = g_WeaponItemProperty[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
			}
			else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				itempos[0] = g_WeaponItemProperty2[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
				itempos[1] = g_WeaponItemProperty2[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
			}
			else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				itempos[0] = g_WeaponItemProperty3[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex];
				itempos[1] = g_WeaponItemProperty3[ pUser->m_CharacterInfo.modeldescript.item1 ].vecItemPosition[genderindex+1];
			}
#endif

			if( pUser->m_AttackTarget )
			{
				_XModelDescriptor* pTargetModelDescryptor = NULL;

				switch( pUser->m_AttackTargetObjectType ) 
				{
				case _XPICKINGTARGET_PC		:
					pTargetModelDescryptor = &((_XUser*)pUser->m_AttackTarget)->m_ModelDescriptor;
					break;
				case _XPICKINGTARGET_MOB	:
					pTargetModelDescryptor = &((_XMob*)pUser->m_AttackTarget)->m_ModelDescriptor;
					break;
				case _XPICKINGTARGET_NPC	:
					pTargetModelDescryptor = &((_XNPCObject*)pUser->m_AttackTarget)->m_ModelDescriptor;
					break;
				case _XPICKINGTARGET_ITEM	:
					break;
				case _XPICKINGTARGET_DUMMY	:
					pTargetModelDescryptor = &((_XDummy*)pUser->m_AttackTarget)->m_ModelDescriptor;
					break;
				case _XPICKINGTARGET_SEAT	:
					break;
				case _XPICKINGTARGET_SELF	:
					pTargetModelDescryptor = &((_XUser*)pUser->m_AttackTarget)->m_ModelDescriptor;
					break;
				case _XPICKINGTARGET_COLLECT :
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					pTargetModelDescryptor = &((_XFunctionalObject*)pUser->m_AttackTarget)->m_ModelDescriptor;
					break;
				}
				m_EffectManager->Update(this, NULL, &pUser->m_TargetPosition, itempos, pTargetModelDescryptor, 1.35f );
			}
			else
			{
				m_EffectManager->Update(this, NULL, &pUser->m_TargetPosition, itempos, NULL, 1.35f );
			}

			if( !m_NoAnimationAndPresentPipeline )
			{
				m_EffectManager->Render();
			}
		}	
	}
}

void _XModelDescriptor::RenderHeroCharacter( _XUser* pUser )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

		if( m_ppModelList[_XDEF_MODELCLUSTER_FACE]		)	m_ppModelList[_XDEF_MODELCLUSTER_FACE		]->Render( this );
		if( m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_LOWERBODY  ]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY] )	m_ppModelList[_XDEF_MODELCLUSTER_UPPERBODY  ]->Render( this );
		if( m_ppModelList[_XDEF_MODELCLUSTER_LHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_LHAND 		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_RHAND]		)	m_ppModelList[_XDEF_MODELCLUSTER_RHAND 		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_LFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_LFOOT 		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_RFOOT]		)	m_ppModelList[_XDEF_MODELCLUSTER_RFOOT 		]->Render( this );			
		//if( m_ppModelList[_XDEF_MODELCLUSTER_ADAE]		)	m_ppModelList[_XDEF_MODELCLUSTER_ADAE 		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_EARRING]	)	m_ppModelList[_XDEF_MODELCLUSTER_EARRING	]->Render( this );						
		if( m_ppModelList[_XDEF_MODELCLUSTER_CAP]		)	m_ppModelList[_XDEF_MODELCLUSTER_CAP		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK]	)	m_ppModelList[_XDEF_MODELCLUSTER_BACKPACK   ]->Render( this );
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM1]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM1		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM2]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM2		]->Render( this );			
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM3]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM3		]->Render( this );
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM4]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM4		]->Render( this );
		if( m_ppModelList[_XDEF_MODELCLUSTER_ITEM5]		)	m_ppModelList[_XDEF_MODELCLUSTER_ITEM5		]->Render( this );

		g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
		if( m_ppModelList[_XDEF_MODELCLUSTER_HAIR]		)	m_ppModelList[_XDEF_MODELCLUSTER_HAIR		]->Render( this );		
	}

	g_RenderAlphaBlendFlag = FALSE;
	//g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
	gpDev->SetTransform( D3DTS_WORLD, &m_Position );
	gpDev->SetVertexShader( NULL );

	if( pUser && !m_NoAnimationAndPresentPipeline )
	{	
		if( !m_MotionChangeFlag )
		{
#ifdef _XDEF_MODELTHREADLOADING	
			if( (m_SwordEffectCount > 0 ) && m_ViewLeftSwordEffect  )
#else
			if( m_pSwordEffectInstanceObject && m_ViewLeftSwordEffect  )
#endif
			{
				if( m_pBipedController && m_pBipedController->m_pSwordTraceInfoData )
				{
					D3DXVECTOR3 toppos;
					D3DXVECTOR3 bottompos;
					_LPXM_BIPEDOBJECT pBipedNode;

					for( int i = 0; i < m_SwordEffectCount; i++ )
					{
						pBipedNode = m_pBipedController->GetNode( m_pBipedController->m_pSwordTraceInfoData->m_pLinkedBipedIndex[i] );
						if( !pBipedNode ) continue;
						D3DXVec3TransformCoord( &toppos, &m_pBipedController->m_pSwordTraceInfoData->m_pTopPosList[i],		 &pBipedNode->m_matLocalAni );
						D3DXVec3TransformCoord( &bottompos, &m_pBipedController->m_pSwordTraceInfoData->m_pBottomPosList[i], &pBipedNode->m_matLocalAni );
						
						if( m_FrameResult & _XDEF_ACI_FIRSTFRAME )
						{
							m_pSwordEffectInstanceObject[i].ControlPoint[0][0] = toppos;
							m_pSwordEffectInstanceObject[i].ControlPoint[0][1] = toppos;
							m_pSwordEffectInstanceObject[i].ControlPoint[0][2] = toppos;
							m_pSwordEffectInstanceObject[i].ControlPoint[0][3] = toppos;
							
							m_pSwordEffectInstanceObject[i].ControlPoint[1][0] = bottompos;
							m_pSwordEffectInstanceObject[i].ControlPoint[1][1] = bottompos;
							m_pSwordEffectInstanceObject[i].ControlPoint[1][2] = bottompos;
							m_pSwordEffectInstanceObject[i].ControlPoint[1][3] = bottompos;
						}
						else
						{
							//if( !((int)m_AniFrame % 3) )
							//if( ((int)m_AniFrame & 0x01) )
							{
								m_pSwordEffectInstanceObject[i].ControlPoint[0][0] = m_pSwordEffectInstanceObject[0].ControlPoint[0][1];
								m_pSwordEffectInstanceObject[i].ControlPoint[0][1] = m_pSwordEffectInstanceObject[0].ControlPoint[0][2];
								m_pSwordEffectInstanceObject[i].ControlPoint[0][2] = m_pSwordEffectInstanceObject[0].ControlPoint[0][3];
								
								m_pSwordEffectInstanceObject[i].ControlPoint[1][0] = m_pSwordEffectInstanceObject[0].ControlPoint[1][1];
								m_pSwordEffectInstanceObject[i].ControlPoint[1][1] = m_pSwordEffectInstanceObject[0].ControlPoint[1][2];
								m_pSwordEffectInstanceObject[i].ControlPoint[1][2] = m_pSwordEffectInstanceObject[0].ControlPoint[1][3];
							}
							
							m_pSwordEffectInstanceObject[i].ControlPoint[0][3] = toppos;
							m_pSwordEffectInstanceObject[i].ControlPoint[1][3] = bottompos;
							
							g_SwordEffectController.Process(&m_pSwordEffectInstanceObject[i]);
							g_SwordEffectController.Render( _XDEF_DEFAULTRENDERSTYLE_MONSTERSWORDEFFECT );
						}		
					}
				}
			}
		}
	}

	if( m_EffectManager && m_pBipedController->m_pSwordTraceInfoData )
	{			
		int genderindex = ( pUser->m_CharacterInfo.gender == _XGENDER_MAN ) ? 0 : 2;

		D3DXVECTOR3 itempos[3];

		if( m_pBipedController->m_pSwordTraceInfoData->m_pLinkedBipedIndex )
		{
			_LPXM_BIPEDOBJECT pBipedNode = m_pBipedController->GetNode( m_pBipedController->m_pSwordTraceInfoData->m_pLinkedBipedIndex[0] );
			if( pBipedNode ) 
			{
				D3DXVec3TransformCoord( &itempos[0], &m_pBipedController->m_pSwordTraceInfoData->m_pTopPosList[0],	&pBipedNode->m_matLocalAni );
				D3DXVec3TransformCoord( &itempos[1], &m_pBipedController->m_pSwordTraceInfoData->m_pBottomPosList[0], &pBipedNode->m_matLocalAni );
			}
			else
			{
				itempos[0] = itempos[1] = D3DXVECTOR3(0.0f,0.0f,0.0f);
			}			
		}
		else
		{
			itempos[0] = itempos[1] = D3DXVECTOR3(0.0f,0.0f,0.0f);
		}			
			

		m_EffectManager->Update(this, NULL, &pUser->m_TargetPosition, itempos);
		if( !m_NoAnimationAndPresentPipeline )
		{
			m_EffectManager->Render();
		}
	}
}

void _XModelDescriptor::Render_ZeroLevelMob( _XMob* pMob )
{
	if( !m_NoAnimationAndPresentPipeline )
	{
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );

		for( int i = 0; i < m_ModelStackSize; i++ )
		{
			if( m_ppModelList[i] )
			{
				m_ppModelList[i]->Render( this );
			}	
		}	
	}
	
	g_RenderAlphaBlendFlag = FALSE;
	//g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
	gpDev->SetTransform( D3DTS_WORLD, &m_Position );
	gpDev->SetVertexShader( NULL );

	if( pMob  )	
	{
		int genderindex = ( pMob->m_Gender == _XGENDER_MAN ) ? 0 : 2;

		if( !m_NoAnimationAndPresentPipeline )
		{
			if( !m_MotionChangeFlag )
			{
#ifdef _XDEF_MODELTHREADLOADING
				if( (m_SwordEffectCount > 0 ) && m_ViewLeftSwordEffect && pMob->m_ItemNo != 0 )
#else
				if( m_pSwordEffectInstanceObject && m_ViewLeftSwordEffect && pMob->m_ItemNo != 0 )
#endif
				{	
					int referencematrix;
					
					switch( g_WeaponItemProperty[pMob->m_ItemNo].cSecondType )
					{			
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[pMob->m_ItemNo].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							referencematrix = 0;
						}
						else
						{
							referencematrix = 4;
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						referencematrix = 5;
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						referencematrix = 0;
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						referencematrix = 0;
						break;
					}			
					
					D3DXVECTOR3 toppos = g_WeaponItemProperty[pMob->m_ItemNo].vecItemPosition[genderindex];
					D3DXVECTOR3 bottompos = g_WeaponItemProperty[pMob->m_ItemNo].vecItemPosition[genderindex+1];
					
					D3DXMATRIX mat;
					D3DXMatrixMultiply( &mat, &m_PartMatrixContainer[0], &m_Position );		 
					D3DXVec3TransformCoord( &toppos, &toppos, &m_PartMatrixContainer[referencematrix] );
					D3DXVec3TransformCoord( &bottompos, &bottompos, &m_PartMatrixContainer[referencematrix] );
					
					if( !m_SwordEffectInitialized )
					{
						m_SwordEffectInitialized = TRUE;

						m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = toppos;
						m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = toppos;
						m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = toppos;
						m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;
						
						m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = bottompos;
						m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = bottompos;
						m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = bottompos;
						m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;
					}
					
					if( m_FrameResult & _XDEF_ACI_FIRSTFRAME )
					{
						m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = toppos;
						m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = toppos;
						m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = toppos;
						m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;

						m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = bottompos;
						m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = bottompos;
						m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = bottompos;
						m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;
					}
					else
					{
						//if( !((int)m_AniFrame % 3) )
						//if( ((int)m_AniFrame & 0x01) )
						{
							m_pSwordEffectInstanceObject[0].ControlPoint[0][0] = m_pSwordEffectInstanceObject[0].ControlPoint[0][1];
							m_pSwordEffectInstanceObject[0].ControlPoint[0][1] = m_pSwordEffectInstanceObject[0].ControlPoint[0][2];
							m_pSwordEffectInstanceObject[0].ControlPoint[0][2] = m_pSwordEffectInstanceObject[0].ControlPoint[0][3];

							m_pSwordEffectInstanceObject[0].ControlPoint[1][0] = m_pSwordEffectInstanceObject[0].ControlPoint[1][1];
							m_pSwordEffectInstanceObject[0].ControlPoint[1][1] = m_pSwordEffectInstanceObject[0].ControlPoint[1][2];
							m_pSwordEffectInstanceObject[0].ControlPoint[1][2] = m_pSwordEffectInstanceObject[0].ControlPoint[1][3];
						}
						
						m_pSwordEffectInstanceObject[0].ControlPoint[0][3] = toppos;
						m_pSwordEffectInstanceObject[0].ControlPoint[1][3] = bottompos;			

						g_SwordEffectController.Process(m_pSwordEffectInstanceObject);
						g_SwordEffectController.Render( _XDEF_DEFAULTRENDERSTYLE_MONSTERSWORDEFFECT );
					}			
				}
			}
		}

		if( m_EffectManager )
		{
			D3DXVECTOR3 itempos[3];
			itempos[0] = g_WeaponItemProperty[ pMob->m_ItemNo ].vecItemPosition[genderindex];
			itempos[1] = g_WeaponItemProperty[ pMob->m_ItemNo ].vecItemPosition[genderindex+1];

			if( pMob->m_AttackTarget )
			{
				_XModelDescriptor* pTargetModelDescryptor = NULL;
				
				switch( pMob->m_AttackTargetObjectType ) 
				{
				case _XPICKINGTARGET_PC		:
					pTargetModelDescryptor = &((_XUser*)pMob->m_AttackTarget)->m_ModelDescriptor;
					break;
				case _XPICKINGTARGET_MOB	:
					pTargetModelDescryptor = &((_XMob*)pMob->m_AttackTarget)->m_ModelDescriptor;
					break;
				case _XPICKINGTARGET_NPC	:
					pTargetModelDescryptor = &((_XNPCObject*)pMob->m_AttackTarget)->m_ModelDescriptor;
					break;
				}
				m_EffectManager->Update(this, NULL, NULL, NULL, pTargetModelDescryptor );
			}
			else
			{
				m_EffectManager->Update( this, NULL, NULL, NULL );
			}

			if( !m_NoAnimationAndPresentPipeline )
				m_EffectManager->Render();
		}	
	}		
}


void _XModelDescriptor::SetDrawSwordEffect( BOOL enable )
{
	m_ViewLeftSwordEffect = enable;

	if( m_ViewLeftSwordEffect )
	{
		if( m_pBipedController && m_pBipedController->m_pSwordTraceInfoData )
		{
			D3DXVECTOR3 toppos;
			D3DXVECTOR3 bottompos;
			_LPXM_BIPEDOBJECT pBipedNode;

			for( int i = 0; i < m_SwordEffectCount; i++ )
			{
				pBipedNode = m_pBipedController->GetNode( m_pBipedController->m_pSwordTraceInfoData->m_pLinkedBipedIndex[i] );
				if( !pBipedNode ) continue;
				D3DXVec3TransformCoord( &toppos, &m_pBipedController->m_pSwordTraceInfoData->m_pTopPosList[i],		 &pBipedNode->m_matLocalAni );
				D3DXVec3TransformCoord( &bottompos, &m_pBipedController->m_pSwordTraceInfoData->m_pBottomPosList[i], &pBipedNode->m_matLocalAni );
				
				m_pSwordEffectInstanceObject[i].ControlPoint[0][0] = toppos;
				m_pSwordEffectInstanceObject[i].ControlPoint[0][1] = toppos;
				m_pSwordEffectInstanceObject[i].ControlPoint[0][2] = toppos;
				m_pSwordEffectInstanceObject[i].ControlPoint[0][3] = toppos;
				
				m_pSwordEffectInstanceObject[i].ControlPoint[1][0] = bottompos;
				m_pSwordEffectInstanceObject[i].ControlPoint[1][1] = bottompos;
				m_pSwordEffectInstanceObject[i].ControlPoint[1][2] = bottompos;
				m_pSwordEffectInstanceObject[i].ControlPoint[1][3] = bottompos;
			}
		}
	}
}

void _XModelDescriptor::RenderSwordEffect( int renderstyleindex )
{
	if( !m_ViewLeftSwordEffect || m_MotionChangeFlag ) return;
#ifdef _XDEF_MODELTHREADLOADING		
	if( !(m_SwordEffectCount > 0 ) || !m_pBipedController ) return;
#else
	if( !m_pSwordEffectInstanceObject || !m_pBipedController ) return;
#endif

	if( !m_pBipedController->m_pSwordTraceInfoData ) return;

	if( !m_SwordEffectInitialized )
	{
		SetDrawSwordEffect( TRUE );
		m_SwordEffectInitialized = TRUE;
	}

	D3DXVECTOR3 toppos;
	D3DXVECTOR3 bottompos;
	_LPXM_BIPEDOBJECT pBipedNode;

	gpDev->SetTransform( D3DTS_WORLD, &m_Position );
	gpDev->SetVertexShader( NULL );

	for( int i = 0; i < m_SwordEffectCount; i++ )
	{
		pBipedNode = m_pBipedController->GetNode( m_pBipedController->m_pSwordTraceInfoData->m_pLinkedBipedIndex[i] );
		if( !pBipedNode ) continue;

		D3DXVec3TransformCoord( &toppos, &m_pBipedController->m_pSwordTraceInfoData->m_pTopPosList[i],		 &pBipedNode->m_matLocalAni );
		D3DXVec3TransformCoord( &bottompos, &m_pBipedController->m_pSwordTraceInfoData->m_pBottomPosList[i], &pBipedNode->m_matLocalAni );
		
		if( m_FrameResult & _XDEF_ACI_FIRSTFRAME )
		{
			m_pSwordEffectInstanceObject[i].ControlPoint[0][0] = toppos;
			m_pSwordEffectInstanceObject[i].ControlPoint[0][1] = toppos;
			m_pSwordEffectInstanceObject[i].ControlPoint[0][2] = toppos;
			m_pSwordEffectInstanceObject[i].ControlPoint[0][3] = toppos;
			
			m_pSwordEffectInstanceObject[i].ControlPoint[1][0] = bottompos;
			m_pSwordEffectInstanceObject[i].ControlPoint[1][1] = bottompos;
			m_pSwordEffectInstanceObject[i].ControlPoint[1][2] = bottompos;
			m_pSwordEffectInstanceObject[i].ControlPoint[1][3] = bottompos;
		}
		else
		{
			//if( !((int)m_AniFrame % 3) )
			//if( ((int)m_AniFrame & 0x01) )
			{
				m_pSwordEffectInstanceObject[i].ControlPoint[0][0] = m_pSwordEffectInstanceObject[0].ControlPoint[0][1];
				m_pSwordEffectInstanceObject[i].ControlPoint[0][1] = m_pSwordEffectInstanceObject[0].ControlPoint[0][2];
				m_pSwordEffectInstanceObject[i].ControlPoint[0][2] = m_pSwordEffectInstanceObject[0].ControlPoint[0][3];
				
				m_pSwordEffectInstanceObject[i].ControlPoint[1][0] = m_pSwordEffectInstanceObject[0].ControlPoint[1][1];
				m_pSwordEffectInstanceObject[i].ControlPoint[1][1] = m_pSwordEffectInstanceObject[0].ControlPoint[1][2];
				m_pSwordEffectInstanceObject[i].ControlPoint[1][2] = m_pSwordEffectInstanceObject[0].ControlPoint[1][3];
			}
			
			m_pSwordEffectInstanceObject[i].ControlPoint[0][3] = toppos;
			m_pSwordEffectInstanceObject[i].ControlPoint[1][3] = bottompos;
			
			g_SwordEffectController.Process(&m_pSwordEffectInstanceObject[i]);
			g_SwordEffectController.Render( renderstyleindex );
		}		
	}
}

BOOL _XModelDescriptor::InterOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, 
									   D3DXVECTOR3& Position, FLOAT& RotateAngle, D3DXVECTOR3* pPickedPoint )
{
	for( int i = 0; i < m_ModelStackSize; i++ )
	{
		if( m_ppModelList[i] )
		{
			if( m_ppModelList[i]->InterOBBvsRay( mouserayorigin, mouseraydirection, Position, &RotateAngle, pPickedPoint ) )
			{
				return TRUE;
			}
		}	
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// _XBipedController
//////////////////////////////////////////////////////////////////////

_XBipedController::_XBipedController()
{
	// Model --------------------------------------------------------------------
	m_BipedNodeCnt				= 0;	
	m_MotionCnt					= 0;
	m_ReservedMotionStackCount	= 0;
	m_CheckSoundPlaying			= true;

	m_pMotionStack				= NULL;
	m_BipedNode					= NULL;
	m_pMotionStack				= NULL;	

	for (int i = 0; i < _XParticleLinkBone_MaxCount; ++ i)
	{
		m_CharacterPartIndex[i]		= -1;
	}
/*	m_CharacterPartIndex[0]		= -1;
	m_CharacterPartIndex[1]		= -1;
	m_CharacterPartIndex[2]		= -1;
	m_CharacterPartIndex[3]		= -1;
	m_CharacterPartIndex[4]		= -1;
	m_CharacterPartIndex[5]		= -1;
	m_CharacterPartIndex[6]		= -1;
	m_CharacterPartIndex[7]		= -1;
*/
	m_pSwordTraceInfoData		= NULL;

	m_BaseEffectIndex			= -1;
}

BOOL _XBipedController::ReserveAnimationStack( int motionstacksize )
{
	SAFE_DELETE_ARRAY( m_pMotionStack );
	m_ReservedMotionStackCount = 0;
	m_pMotionStack = new _XM_MOTION[motionstacksize];

	if( !m_pMotionStack )
	{
		_XFatalError( "_XBipedController :: Reserve motion stack memory allocation [%d]", motionstacksize );
		return FALSE;
	}

	m_ReservedMotionStackCount = motionstacksize;
	for( int i = 0; i < m_ReservedMotionStackCount; i++ ) m_pMotionStack[i].ResetMotionData();

	// allocation animation key stack...
	for( i = 0; i < m_BipedNodeCnt; i++ )
	{
		m_BipedNode[i].ReserveAnimationKeyStack( m_ReservedMotionStackCount );
	}

	return TRUE;
}

_XBipedController::~_XBipedController()
{
	svdef_TEMP_MOTION::iterator it;
	for (it = m_svTempMotion.begin(); it != m_svTempMotion.end(); ++ it)
	{
		delete (*it);
	}
	m_svTempMotion.clear();

	DestroyModel();
	ResetMotionList();	
}

void _XBipedController::DestroyModel( void )
{		
	m_BipedNodeCnt		=	0;	
	m_FirstFrame		=	0;
	m_LastFrame			=	0;
	m_TickPerFrame		=	0;
	m_FrameSpeed		=	0;
	m_MotionCnt			=	0;

	for( int i = 0; i < m_ReservedMotionStackCount; i++ ) 
	{
		if( m_pMotionStack )
			m_pMotionStack[i].ResetMotionData();
	}
	
	SAFE_DELETE_ARRAY( m_BipedNode );
	SAFE_DELETE_ARRAY( m_pMotionStack );

	SAFE_DELETE( m_pSwordTraceInfoData );
}


BOOL _XBipedController::CreateNode(int nodecount)
{
	SAFE_DELETE_ARRAY( m_BipedNode );			
	m_BipedNodeCnt	= 0;
	
	m_BipedNode = new _XM_BIPEDOBJECT[nodecount];
	if( !m_BipedNode )
	{
		_XFatalError( "_XBipedController :: CreateNode<%d> memory allocation", nodecount );
		return FALSE;
	}	
	
	m_BipedNodeCnt = nodecount;
	return TRUE;
}

void _XBipedController::DeleteNode()
{	
	SAFE_DELETE_ARRAY(m_BipedNode);
	m_BipedNodeCnt = 0;
}

BOOL _XBipedController::CreateSwordTraceInfoData( int infocount )
{
	SAFE_DELETE( m_pSwordTraceInfoData );

	m_pSwordTraceInfoData = new _XSwordTraceInfoStructure;

	if( !m_pSwordTraceInfoData )
	{
		_XFatalError( "_XBipedController :: Can't create sword trace information data" );
		return FALSE;
	}

	if( !m_pSwordTraceInfoData->CreatePointList( infocount ) )
	{
		_XFatalError( "_XBipedController :: CreateSwordTraceInfoData::CreatePointList" );
		return FALSE;
	}

	return TRUE;
}

_LPXM_BIPEDOBJECT _XBipedController::GetNode(int index )
{ 	
	if(m_BipedNode == NULL || m_BipedNodeCnt == 0 || index < 0) return NULL;
	return &m_BipedNode[ index ];
}

TEMP_MOTION* _XBipedController::GetTempMotion(int index )
{ 	
	if(index >= m_svTempMotion.size()) return NULL;
	return m_svTempMotion[ index ];
}

int	_XBipedController::FindObject( LPSTR objectname )
{
	if(m_BipedNode == NULL || m_BipedNodeCnt == 0 ) return -1;

	for( int i = 0; i < m_BipedNodeCnt; i++ )
	{
		if( strcmpi( m_BipedNode[ i ].strName, objectname ) == 0 )
		{
			//_XDWINPRINT( "_XBipedController : Found part index [%s:%d]", nodename[partindex], i );
			return i;
		}
	}
	return -1;
}

void _XBipedController::FindPartIndex( void )
{
	if(m_BipedNode == NULL || m_BipedNodeCnt == 0 ) return;

	TCHAR* nodename[_XParticleLinkBone_MaxCount] =
	{
		_T("Bip01 R Hand"), 
		_T("Bip01 L Hand"), 
		_T("Bip01 R Foot"), 
		_T("Bip01 L Foot"), 
		_T("Bip01 bo05"  ),		// 단병기 사용되는 본
		_T("Bip01 bo01"  ),		// 장병기 사용되는 본
		_T("Bip01 Pelvis"),	
		_T("Bip01 Head"),	
		_T("Bip01 R Calf"),
		_T("Bip01 L Calf"),
		_T("Bip01 R Forearm"),
		_T("Bip01 L Forearm"),
		_T("Bip01 bo07"  )		// 단병기 사용되는 왼쪽 본
	};
	
	for( int partindex = 0; partindex < _XParticleLinkBone_MaxCount; partindex++ )
	{
		for( int i = 0; i < m_BipedNodeCnt; i++ )
		{
			if( strcmpi( m_BipedNode[ i ].strName, nodename[partindex] ) == 0 )
			{
				//_XDWINPRINT( "_XBipedController : Found part index [%s:%d]", nodename[partindex], i );
				m_CharacterPartIndex[ partindex ] = i;
				break;
			}
		}
	}
}

BOOL _XBipedController::Load( LPSTR szFilename )
{
	assert( szFilename );

	FILE*	Fileptr = NULL;

	_XSetDefaultPath(_XDEF_DEFAULTPATH_MODEL);
	if( ( Fileptr = fopen( szFilename , "rb" ) ) == NULL )
	{
		_XFatalError( "_XBipedController : [%s] file open", szFilename );
		return FALSE;
	}

	if( !Load( Fileptr, szFilename ) )
	{
		fclose( Fileptr );
		return FALSE;
	}

	fclose( Fileptr );
	return TRUE;
}

BOOL _XBipedController::Load( FILE* Fileptr, LPSTR szFilename )
{
	assert( Fileptr );
	if( !Fileptr ) return FALSE;
		
	DestroyModel();

	_XMESHHEADER Header;
	fread(&Header,	sizeof(_XMESHHEADER), 1, Fileptr);

	if(Header.HeaderIndicator != _XDEF_X3DFILEINDICATOR )
	{
		_XFatalError("_XBipedController : Not supported [%s] file", (szFilename) ? szFilename : "this" );		
		return FALSE; 
	}

	if(Header.Fileversion != _XDEF_X3DFILEVERSION)
	{
		_XFatalError("_XBipedController : Not supported [%s] file version [%f]",  (szFilename) ? szFilename : "this" , Header.Fileversion );		
		return FALSE; 
	}

	int i,len;
	int NodeNum;
	int BipedCount = 0;
	fread(&NodeNum,	sizeof(int), 1, Fileptr);	// read object count

	if( !CreateNode(NodeNum) ) return FALSE;		

	// Load model mesh data...	

	_LPXM_BIPEDOBJECT lpCh = NULL;

	for(i = 0; i < NodeNum; i++)
	{
		lpCh = GetNode(i);
	
		// Load mesh object id...
		fread(&lpCh->nId, sizeof(int), 1, Fileptr);

		fread(&len, sizeof(int), 1, Fileptr);
		lpCh->strName = new TCHAR[len+1];
		lpCh->strName[len] = 0;		
		if(len != 0) fread(lpCh->strName, len, 1, Fileptr);

		// Load mesh object parent id...
		fread(&lpCh->nParentId, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);		
		/*lpCh->strParentName = new TCHAR[len+1];
		lpCh->strParentName[len] = 0;
		if(len != 0) fread(lpCh->strParentName, len, 1, Fileptr);*/		
		if(len != 0) fseek(Fileptr, sizeof(TCHAR)*len, SEEK_CUR);

		// Load object matrix...
		fread(&lpCh->m_matTM, sizeof(D3DXMATRIX), 1, Fileptr);

		// preprocess etc matrix...
		D3DXMATRIX matOrthogornalTM;		
		matOrthogornalTM = lpCh->m_matTM;		

		_XMeshMath_UnitMatrixFromMatrix(matOrthogornalTM );  //서로 세축이 직각인 Orthogonal 메트릭스로 만든다.
		D3DXMatrixInverse( &lpCh->m_matInvTM, NULL, &matOrthogornalTM);

		D3DXMATRIX	mat;
		_XMeshMath_SetIdentityMatrix(mat);
		
		if(lpCh->nParentId == -1)
		{
			mat = matOrthogornalTM;
		}
		/*else 
		{
			_LPXM_BIPEDOBJECT lpParent; 
			lpParent = GetNode(lpCh->nParentId);
		}
		*/

		lpCh->m_matAniTM = mat;

		// Load texture id & setting texture index
		fread(&len, sizeof(int), 1, Fileptr);
		if( len > 0)
			fseek( Fileptr, len*sizeof(TCHAR), SEEK_CUR ); // Skipping texture info...
	}

//	SetSkinning();
#ifdef _XDWDEBUG	
	if( szFilename )
		_XDWINPRINT( "_XBipedController : Loaded mesh file [%s]", szFilename );
	else
		_XDWINPRINT( "_XBipedController : Loaded mesh file [%s]", g_LastAccessPackedFile );	
#endif

	FindPartIndex();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Animation control interface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _XBipedController::SetMotion(int index, int SFrame, int EFrame, int FrameSpeed)
{
	if(m_MotionCnt >= index)
	{		
		_XFatalError( "_XBipedController : Invalid [%d] motion informations", index );
		return;
	}
	
	if(SFrame != -1) m_pMotionStack[m_MotionCnt].StartFrame = SFrame;
	if(EFrame != -1) m_pMotionStack[m_MotionCnt].EndFrame   = EFrame;
	if(FrameSpeed != -1) m_pMotionStack[m_MotionCnt].FrameSpeed = FrameSpeed;
}

_LPXM_MOTION _XBipedController::GetMotion( int motionindex )
{
	if( motionindex < 0 ) return NULL;
	if( !m_pMotionStack ) return NULL;	
	if( m_ReservedMotionStackCount <= motionindex ) return NULL;
	
	return &m_pMotionStack[motionindex];
}

_LPXM_POSITIONKEY _XBipedController::GetPositionAniKey(_LPXM_POSITIONKEY AniKey, int KeyNum, float Frame)
{
	if(AniKey[KeyNum-1].nTime <= Frame) return &AniKey[KeyNum-1];
	
	for(int i = 0; i < KeyNum; i++)
	{
		if(AniKey[i].nTime == Frame) return &AniKey[i];		
		if(AniKey[i].nTime < Frame && AniKey[i+1].nTime > Frame) return &AniKey[i];
	}
	
	return &AniKey[0];
}

_LPXM_ROTATIONKEY _XBipedController::GetRotationAniKey(_LPXM_ROTATIONKEY AniKey, int KeyNum, float Frame)
{
	if(AniKey[KeyNum-1].nTime <= Frame) return &AniKey[KeyNum-1];
	
	for(int i = 0; i < KeyNum; i++)
	{
		if(AniKey[i].nTime == Frame) return &AniKey[i];		
		if(AniKey[i].nTime < Frame && AniKey[i+1].nTime > Frame) return &AniKey[i];
	}
	
	return &AniKey[0];
}

bool _XBipedController::AddMotion(int SFrame, int EFrame, int FrameSpeed)
{
	if( !m_pMotionStack ) return false;

	if(m_MotionCnt >= m_ReservedMotionStackCount) 
	{		
		_XFatalError( "_XBipedController : Not enough motion stack [%s]", g_LastAccessPackedFile );
		return false;
	}
	
	m_pMotionStack[m_MotionCnt].MotionNum =  m_MotionCnt;
	m_pMotionStack[m_MotionCnt].StartFrame = SFrame;
	m_pMotionStack[m_MotionCnt].EndFrame =   EFrame;
	m_pMotionStack[m_MotionCnt].FrameSpeed = FrameSpeed;	

	m_MotionCnt++;
	return true;
}

bool _XBipedController::AddMotion(int index, int SFrame, int EFrame, int FrameSpeed)
{
	if( !m_pMotionStack ) return false;

	if(m_MotionCnt >= m_ReservedMotionStackCount) 
	{		
		_XFatalError( "_XBipedController : Not enough motion stack [%s]", g_LastAccessPackedFile );
		return false;
	}

	for(int i = m_MotionCnt; i > index; i--)
	{		
		m_pMotionStack[i+1].MotionNum  = m_pMotionStack[i].MotionNum ;
		m_pMotionStack[i+1].StartFrame = m_pMotionStack[i].StartFrame;
		m_pMotionStack[i+1].EndFrame   = m_pMotionStack[i].EndFrame  ;

		m_pMotionStack[i+1].FrameSpeed = m_pMotionStack[i].FrameSpeed;
	}

	m_pMotionStack[index].MotionNum =  m_MotionCnt;
	m_pMotionStack[index].StartFrame = SFrame;
	m_pMotionStack[index].EndFrame =   EFrame;
	
	m_pMotionStack[index].FrameSpeed = FrameSpeed;

	m_MotionCnt++;
	return true;
}

bool _XBipedController::AddMotion(_XAHEADER Header)
{
	if( !m_pMotionStack ) return false;

	if(m_MotionCnt >= m_ReservedMotionStackCount) 
	{		
		_XFatalError( "_XBipedController : Not enough motion stack [%s]", g_LastAccessPackedFile );
		return false;
	}
	
	m_pMotionStack[m_MotionCnt].MotionNum =  m_MotionCnt;
	m_pMotionStack[m_MotionCnt].StartFrame = Header.FirstFrame;
	m_pMotionStack[m_MotionCnt].EndFrame =   Header.LastFrame;
	m_pMotionStack[m_MotionCnt].FrameSpeed = Header.FrameSpeed;
	
	m_MotionCnt++;
	return true;
}

bool _XBipedController::AddMotion(int index, _XAHEADER Header)
{
	if( !m_pMotionStack ) return false;

	if(m_MotionCnt >= m_ReservedMotionStackCount) 
	{		
		_XFatalError( "_XBipedController : Not enough motion stack [%s]", g_LastAccessPackedFile );
		return false;
	}
		
	for(int i = m_MotionCnt; i > index; i--)
	{		
		m_pMotionStack[i+1].MotionNum  = m_pMotionStack[i].MotionNum ;
		m_pMotionStack[i+1].StartFrame = m_pMotionStack[i].StartFrame;
		m_pMotionStack[i+1].EndFrame   = m_pMotionStack[i].EndFrame  ;

		m_pMotionStack[i+1].FrameSpeed = m_pMotionStack[i].FrameSpeed;
	}	

	m_pMotionStack[index].MotionNum =  m_MotionCnt;
	m_pMotionStack[index].StartFrame = Header.FirstFrame;
	m_pMotionStack[index].EndFrame =   Header.LastFrame;
	m_pMotionStack[index].FrameSpeed = Header.FrameSpeed;

	m_MotionCnt++;
	return true;
}

void _XBipedController::DeleteMotion( void )
{
	if( !m_pMotionStack ) return;

	m_MotionCnt--;	

	m_pMotionStack[m_MotionCnt].MotionNum		= 0;
	m_pMotionStack[m_MotionCnt].StartFrame	= 0;
	m_pMotionStack[m_MotionCnt].EndFrame		= 0;
	m_pMotionStack[m_MotionCnt].FrameSpeed	= 0;
}

void _XBipedController::ResetMotionList( void )
{
	if( !m_pMotionStack ) return;

	for(int i = 0; i < m_MotionCnt; i++)
	{		
		m_pMotionStack[i].MotionNum = 0;
		m_pMotionStack[i].StartFrame = 0;
		m_pMotionStack[i].EndFrame = 0;
		m_pMotionStack[i].FrameSpeed = 0;
	}
	m_MotionCnt = 0;
}

bool _XBipedController::LoadKey( LPSTR szFilename )
{
	assert( szFilename );

	FILE*	Fileptr = NULL;

	_XSetDefaultPath(_XDEF_DEFAULTPATH_ANIMATION);

	if( ( Fileptr = fopen( szFilename , "rb" ) ) == NULL )
	{
		_XFatalError( "_XBipedController : [%s] file open", szFilename );
		return false;
	}

	if( !LoadKey( Fileptr, szFilename ) )
	{
		fclose( Fileptr );
		return false;
	}

	fclose( Fileptr );
	return true;
}

bool _XBipedController::LoadKey( FILE* Fileptr, LPSTR szFilename )
{
	assert( Fileptr );

	if( !Fileptr ) return false;

	_XAHEADER Header;
	fread(&Header,	sizeof(_XAHEADER), 1, Fileptr);	
		
	if(Header.HeaderIndicator != _XDEF_XAFILEINDICATOR )
	{		
		_XFatalError("_XBipedController : Not supported [%s] file", szFilename );
		fclose(Fileptr);
		return false; 
	}

	if(Header.Fileversion != _XDEF_XAFILEVERSION)
	{
		_XFatalError("_XBipedController : Not supported [%s] file version [%f]", szFilename, Header.Fileversion );
		fclose(Fileptr);
		return false;
	}

	TCHAR PtnName[256];
	
	int CurMotion = m_MotionCnt;
		
	if( szFilename )
	{		
		GetFileNameFromPathString(PtnName, szFilename);	
		AddMotion( Header.FirstFrame, Header.LastFrame, Header.FrameSpeed );
	}
	else
	{
		if( strlen(g_LastAccessPackedFile) )
		{
			GetFileNameFromPathString(PtnName, g_LastAccessPackedFile);	
			AddMotion( Header.FirstFrame, Header.LastFrame, Header.FrameSpeed );
		}
		else
		{			
			AddMotion( Header.FirstFrame, Header.LastFrame, Header.FrameSpeed );
		}
	}

	int BipedNum = 0;
	fread( &BipedNum, sizeof(int), 1, Fileptr );

	int  i,j,len;
	int  ID, ParentID;
	TCHAR buff[MAX_PATH];
	_LPXM_BIPEDOBJECT lpNode = NULL, lpParent = NULL, lpCh = NULL;

	for(i = 0; i < BipedNum; i++)
	{
		fread(&ID, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		memset(buff, 0, sizeof(TCHAR)*MAX_PATH);
		if(len != 0) fread(buff, len, 1, Fileptr);

		lpNode = NULL;
		for(j = 0; j < m_BipedNodeCnt; j++)
		{
			lpCh = GetNode(j);
			if( strcmp( lpCh->strName, buff ) == 0 )
			{
				ID = lpCh->nId;
				lpNode = lpCh;
				break;
			}
		}
		
		if(ID == -1 || lpNode == NULL) 
		{			
			_XFatalError( "Invalid biped informations [%s]", szFilename );			
			fclose(Fileptr);
			DeleteMotion();
			return false;
		}

		// Load parent node informations...
		fread(&ParentID, sizeof(int), 1, Fileptr);
		fread(&len, sizeof(int), 1, Fileptr);
		memset(buff, 0, sizeof(TCHAR)*MAX_PATH);
		if(len != 0) fread(buff, len, 1, Fileptr);

		lpParent = NULL;
		
		// Search parent node...
		for(j = 0; j < m_BipedNodeCnt; j++)
		{
			lpCh = GetNode(j);
			if( strcmp( lpCh->strName, buff ) == 0 )
			{
				ParentID = lpCh->nId;
				lpParent = lpCh;
				break;
			}
		}

		if(ParentID != lpNode->nParentId ) 
		{			
			//_XLog( "WARNING : [%s]:[%d %d(%s)] biped parent link error", szFilename, ParentID,lpNode->nParentId, lpNode->strName );
			_XFatalError( "[%s]:[%d %d(%s)] biped parent link error", szFilename, ParentID,lpNode->nParentId, lpNode->strName );
			fclose(Fileptr);
			DeleteMotion();
			return false;
		}

		D3DXMATRIX objectmatrix;
		fread(&objectmatrix, sizeof(D3DXMATRIX), 1, Fileptr);

		D3DXMATRIX	mat;
		D3DXMatrixIdentity(&mat);

		if(lpNode->nParentId != -1)
		{
			D3DXMatrixMultiply(&mat, &lpNode->m_matTM, &lpParent->m_matInvTM);
		}
		else mat = lpNode->m_matTM;

		lpNode->m_localTM = mat;
		D3DXMatrixIdentity(&lpNode->m_matAniTM);

		int nAniKey = 0;
		
		_XM_POSITIONKEY TempPositionAniKey[_XDEF_XKEYBUFFERSIZE];
		_XM_ROTATIONKEY TempRotationAniKey[_XDEF_XKEYBUFFERSIZE];
		
		memset( TempPositionAniKey, 0, sizeof( _XM_POSITIONKEY )*_XDEF_XKEYBUFFERSIZE );
		memset( TempRotationAniKey, 0, sizeof( _XM_ROTATIONKEY )*_XDEF_XKEYBUFFERSIZE );
		
		// Read position key ---------------------------------------------------------------------------------------------
		fread(&nAniKey, sizeof(int), 1, Fileptr);

		if(nAniKey != 0)
		{
			for(j = 0; j < nAniKey; j++)
				fread(&TempPositionAniKey[j], sizeof(_XM_POSITIONKEY), 1, Fileptr);

			if(TempPositionAniKey[0].nTime != 0)
			{
				// 0번 키값을을 추가한다..
				lpNode->m_pnPosKey[CurMotion] = nAniKey+1;

				SAFE_DELETE_ARRAY(lpNode->m_pPosKey[CurMotion]);
				lpNode->m_pPosKey[CurMotion] = new _XM_POSITIONKEY[lpNode->m_pnPosKey[CurMotion]];

				lpNode->m_pPosKey[CurMotion][0].nTime = 0;
				lpNode->m_pPosKey[CurMotion][0].d3dVKeyVal.x = mat._41;
				lpNode->m_pPosKey[CurMotion][0].d3dVKeyVal.y = mat._42;
				lpNode->m_pPosKey[CurMotion][0].d3dVKeyVal.z = mat._43;

				for(j = 1; j < lpNode->m_pnPosKey[CurMotion]; j++)
				{
					memcpy(&lpNode->m_pPosKey[CurMotion][j], &TempPositionAniKey[j-1], sizeof(_XM_POSITIONKEY));
				}
			}
			else
			{
				lpNode->m_pnPosKey[CurMotion] = nAniKey;
				SAFE_DELETE_ARRAY(lpNode->m_pPosKey[CurMotion]);
				lpNode->m_pPosKey[CurMotion] = new _XM_POSITIONKEY[lpNode->m_pnPosKey[CurMotion]];

				for(j = 0; j < nAniKey; j++)
				{
					memcpy(&lpNode->m_pPosKey[CurMotion][j], &TempPositionAniKey[j], sizeof(_XM_POSITIONKEY));
				}
			}

			/* no effect code... <lpParent->PosMatrixTM : 단위행렬 / lpParent->PosMatrixTM = TM * TM의 인버스 >
			if(lpNode->nParentId != -1)
			{
				for(j = 1; j < lpNode->m_pnPosKey[CurMotion]; j++)
				{
					D3DXVECTOR3 Vect = lpNode->m_pPosKey[CurMotion][j].d3dVKeyVal;
					_XMeshMath_VectorMatrixMultiply(lpNode->m_pPosKey[CurMotion][j].d3dVKeyVal, Vect, lpParent->PosMatrixTM);
				}
			}
			*/
		}

		nAniKey = 0;
		
		// Read rotation key ---------------------------------------------------------------------------------------------
		fread(&nAniKey, sizeof(int), 1, Fileptr);		

		if(nAniKey != 0)
		{
			for(j = 0; j < nAniKey; j++)
			{
				fread(&TempRotationAniKey[j], sizeof(_XM_ROTATIONKEY), 1, Fileptr);

				D3DXQUATERNION rot = TempRotationAniKey[j].Qt;

				// 회전을 음수로..
				rot.w = -rot.w;

				// translate quaternion table
				TempRotationAniKey[j].Qt.x = sinf(rot.w/2.0f)*rot.x;
				TempRotationAniKey[j].Qt.y = sinf(rot.w/2.0f)*rot.y;
				TempRotationAniKey[j].Qt.z = sinf(rot.w/2.0f)*rot.z;
				TempRotationAniKey[j].Qt.w = cosf(rot.w/2.0f);
			}

			if(TempRotationAniKey[0].nTime != 0)
			{
				lpNode->m_pnRotKey[CurMotion] = nAniKey+1;

				SAFE_DELETE_ARRAY(lpNode->m_pRotKey[CurMotion]);
				lpNode->m_pRotKey[CurMotion] = new _XM_ROTATIONKEY[lpNode->m_pnRotKey[CurMotion]];

				lpNode->m_pRotKey[CurMotion][0].nTime = 0.0f;

				// Generate quaternion...
				// may be...little accidental error

				D3DXQUATERNION rot; 
				_XMeshMath_QuaternionFromMatrix(rot, mat); 

				lpNode->m_pRotKey[CurMotion][0].Qt.w=rot.w;
				lpNode->m_pRotKey[CurMotion][0].Qt.x=rot.x;
				lpNode->m_pRotKey[CurMotion][0].Qt.y=rot.y;
				lpNode->m_pRotKey[CurMotion][0].Qt.z=rot.z;

				for(j = 1; j < lpNode->m_pnRotKey[CurMotion]; j++)
				{
					memcpy(&lpNode->m_pRotKey[CurMotion][j], &TempRotationAniKey[j-1], sizeof(_XM_ROTATIONKEY));
				}
			}
			else
			{
				lpNode->m_pnRotKey[CurMotion] = nAniKey;
				SAFE_DELETE_ARRAY(lpNode->m_pRotKey[CurMotion]);
				lpNode->m_pRotKey[CurMotion] = new _XM_ROTATIONKEY[lpNode->m_pnRotKey[CurMotion]];

				for(j = 0; j < lpNode->m_pnRotKey[CurMotion]; j++)
				{
					memcpy(&lpNode->m_pRotKey[CurMotion][j], &TempRotationAniKey[j], sizeof(_XM_ROTATIONKEY));
				}
			}

			// accumulation quaternion...
			for(j = 1; j < nAniKey; j++)
			{
				D3DXQUATERNION r;
				_XMeshMath_QuaternionMultiply( r, lpNode->m_pRotKey[CurMotion][j-1].Qt, lpNode->m_pRotKey[CurMotion][j].Qt );
				lpNode->m_pRotKey[CurMotion][j].Qt = r;
			}
		}
	}
	
	SetLastFrame(m_LastFrame+Header.LastFrame);	

	//_XDWINPRINT( "Inserted [%s] motion table", PtnName );

	return true;
}

void _XBipedController::FrameAnimation( _XModelDescriptor* pAniData, FLOAT curframe )
{
	int currentmotion = pAniData->m_CurMotion;
	int framespeed	 = m_pMotionStack[currentmotion].FrameSpeed;
		
	pAniData->m_AniFrame = curframe;

	if( pAniData->m_AniFrame < 0 ) pAniData->m_AniFrame = 0;
	if( pAniData->m_AniFrame > m_pMotionStack[currentmotion].EndFrame ) 
		pAniData->m_AniFrame = m_pMotionStack[currentmotion].EndFrame;	
	
	int					frame;
	_LPXM_ROTATIONKEY	RotationSFrame=NULL, RotationEFrame=NULL;
	_LPXM_POSITIONKEY	PositionSFrame=NULL, PositionEFrame=NULL;
	_LPXM_BIPEDOBJECT	lpCh=NULL,   lpParent=NULL;
	

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{			
		lpCh			= GetNode(objindex );
		lpParent		= GetNode(lpCh->nParentId);

		D3DXMATRIX		d3dMatrix, mat;        
		D3DXQUATERNION	q;
		D3DXMatrixIdentity( &d3dMatrix );

		// Check animation list
		if(lpCh->m_pnRotKey[currentmotion] == 0 && lpCh->m_pnPosKey[currentmotion] == 0)
		{
			d3dMatrix = lpCh->m_localTM;
		}
		else
		{
			// Check rotation key
			if(lpCh->m_pnRotKey[currentmotion] == 0)
			{
				d3dMatrix		= lpCh->m_localTM;
				d3dMatrix._41	= 0.0f;
				d3dMatrix._42	= 0.0f;
				d3dMatrix._43	= 0.0f;
				d3dMatrix._44	= 1.0f;
			}
			// Process rotate animation key
			else 
			{
				RotationSFrame = GetRotationAniKey(lpCh->m_pRotKey[currentmotion], lpCh->m_pnRotKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].StartFrame);
				RotationEFrame = GetRotationAniKey(lpCh->m_pRotKey[currentmotion], lpCh->m_pnRotKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].EndFrame);

				// Check start frame
				if(pAniData->m_AniFrame == (FLOAT)m_pMotionStack[currentmotion].StartFrame)	
				{	
					q = RotationSFrame->Qt;
				}
				// Check last frame
				else if(RotationEFrame->nTime <= pAniData->m_AniFrame) q = RotationEFrame->Qt;
				else
				{
					for(frame = 0; frame < lpCh->m_pnRotKey[currentmotion]; frame++)
					{
						if(lpCh->m_pRotKey[currentmotion][frame].nTime == pAniData->m_AniFrame)
						{
							q = lpCh->m_pRotKey[currentmotion][frame].Qt;
							break;
						}
						if(lpCh->m_pRotKey[currentmotion][frame  ].nTime < pAniData->m_AniFrame &&
						   lpCh->m_pRotKey[currentmotion][frame+1].nTime > pAniData->m_AniFrame )
						{
							// frame 과 frame+1으로 보간한 쿼터니언값..
							// 진행률은 ( 현재Frame - 이전 Key Frame )/( 다음 Key Frame - 이전 Key Frame );
							float sx, ex;
							sx = lpCh->m_pRotKey[currentmotion][frame  ].nTime;
							ex = lpCh->m_pRotKey[currentmotion][frame+1].nTime;

							float xx = (pAniData->m_AniFrame-sx)/(ex-sx);
							D3DXQuaternionSlerp(&q, &lpCh->m_pRotKey[currentmotion][frame  ].Qt, 
													&lpCh->m_pRotKey[currentmotion][frame+1].Qt, xx);
							break;
						}
					}
				}				
				_XMeshMath_MatrixFromQuaternion(d3dMatrix, q);
			}

			// Check position key
			if(lpCh->m_pnPosKey[currentmotion] == 0) // 프레임이 1개. 즉 0번키..
			{
				d3dMatrix._41 = lpCh->m_localTM._41;
				d3dMatrix._42 = lpCh->m_localTM._42;
				d3dMatrix._43 = lpCh->m_localTM._43;
			}
			// Process position key
			else 
			{
				PositionSFrame = GetPositionAniKey(lpCh->m_pPosKey[currentmotion], lpCh->m_pnPosKey[currentmotion], 
										   (FLOAT)m_pMotionStack[currentmotion].StartFrame);
				PositionEFrame = GetPositionAniKey(lpCh->m_pPosKey[currentmotion], lpCh->m_pnPosKey[currentmotion], 
										   (FLOAT)m_pMotionStack[currentmotion].EndFrame);

				if(pAniData->m_AniFrame == (FLOAT)m_pMotionStack[currentmotion].StartFrame)
				{
					d3dMatrix._41 = PositionSFrame->d3dVKeyVal.x;
					d3dMatrix._42 = PositionSFrame->d3dVKeyVal.y;
					d3dMatrix._43 = PositionSFrame->d3dVKeyVal.z;
				}
				else if(PositionEFrame->nTime <= pAniData->m_AniFrame)
				{
					d3dMatrix._41 = PositionEFrame->d3dVKeyVal.x;
					d3dMatrix._42 = PositionEFrame->d3dVKeyVal.y;
					d3dMatrix._43 = PositionEFrame->d3dVKeyVal.z;
				}
				else
				{
					for(frame = 0; frame < lpCh->m_pnPosKey[currentmotion]; frame++)
					{
						if(lpCh->m_pPosKey[currentmotion][frame].nTime == pAniData->m_AniFrame)
						{
							d3dMatrix._41 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x;
							d3dMatrix._42 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y;
							d3dMatrix._43 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z;
							break;
						}
						if(lpCh->m_pPosKey[currentmotion][frame  ].nTime < pAniData->m_AniFrame && 
						   lpCh->m_pPosKey[currentmotion][frame+1].nTime > pAniData->m_AniFrame )
						{
							// frame 과 frame+1으로 보간한 쿼터니언값..
							// 진행률은 ( 현재Frame - 이전 Key Frame )/( 다음 Key Frame - 이전 Key Frame );
							float sx, ex;
							sx = lpCh->m_pPosKey[currentmotion][frame  ].nTime;
							ex = lpCh->m_pPosKey[currentmotion][frame+1].nTime;
							
							float xx = (pAniData->m_AniFrame-sx)/(ex-sx);

							// 벡터 보간..
							d3dMatrix._41 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x + 
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.x - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x ) * xx;

							d3dMatrix._42 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y +
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.y - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y ) * xx;

							d3dMatrix._43 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z +
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.z - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z ) * xx;
							break;
						}
					}
				}
			}
		}
		_XMeshMath_UnitMatrixFromMatrix(d3dMatrix);

		if(lpCh->nParentId == -1)
		{
			//자식을위해 결과매트릭스를 저장
			lpCh->m_matAniTM = d3dMatrix;
		}
		else
		{
			// d3dMatrix은 오브젝트의 Local Ani TM 이된다..
			D3DXMatrixMultiply(&lpCh->m_matAniTM, &d3dMatrix, &lpParent->m_matAniTM);
		}

		D3DXMatrixMultiply( &lpCh->m_matLocalAni, &lpCh->m_matInvTM , &lpCh->m_matAniTM);

		if( pAniData->m_EnableEffectProcess )
		{
			for (int i = 0; i < _XParticleLinkBone_MaxCount; ++ i)
			{
				if( objindex == m_CharacterPartIndex[i] )
				{
					pAniData->m_PartMatrixContainer[i] = lpCh->m_matLocalAni;
					break;
				}
			}

/*			//이펙트노드의  결과 메트릭스			
			if( objindex == m_CharacterPartIndex[0] )	   pAniData->m_PartMatrixContainer[0] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[1] ) pAniData->m_PartMatrixContainer[1] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[2] ) pAniData->m_PartMatrixContainer[2] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[3] ) pAniData->m_PartMatrixContainer[3] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[4] ) pAniData->m_PartMatrixContainer[4] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[5] ) pAniData->m_PartMatrixContainer[5] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[6] ) pAniData->m_PartMatrixContainer[6] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[7] ) pAniData->m_PartMatrixContainer[7] = lpCh->m_matLocalAni;
*/
		}
		
	}
		
	//pAniData->m_AniLastFrame	= pAniData->m_AniFrame;
}

void _XBipedController::Advance_NoCheckEvent( _XModelDescriptor* pAniData, FLOAT fElapsedTime )
{
	int currentmotion = pAniData->m_CurMotion;
	
	if( m_pMotionStack[currentmotion].StartFrame >= (int)pAniData->m_AniFrame )
	{
		//pAniData->m_EventHandler.lastframe = 0;

		if( pAniData->m_EventHandler.lastframe )
			pAniData->ResetEventHandler();

		if( !pAniData->m_EventHandler.firstframe )
		{				
			pAniData->m_EventHandler.firstframe = true;
			pAniData->m_FrameResult = _XDEF_ACI_FIRSTFRAME;
		}
	}
				
	pAniData->m_AniFrame = (fElapsedTime * (FLOAT)m_pMotionStack[pAniData->m_CurMotion].FrameSpeed);
	if (pAniData->m_AniFrame > m_pMotionStack[currentmotion].EndFrame)
	{
		pAniData->m_AniFrame = pAniData->m_AniFrame - ((int)(pAniData->m_AniFrame / m_pMotionStack[currentmotion].EndFrame) * m_pMotionStack[currentmotion].EndFrame); 
	}
	
	// Check last frame
	if( (int)pAniData->m_AniFrame >= (m_pMotionStack[currentmotion].EndFrame-1)  )
	{		
		pAniData->m_EventHandler.lastframe = 1;
		pAniData->m_FrameResult |= _XDEF_ACI_LASTFRAME;
	}
	
	if( pAniData->m_AniFrame >= (FLOAT)m_pMotionStack[currentmotion].EndFrame)
	{
		if( pAniData->m_LockEndFrame )
		{
			pAniData->m_AniFrame =  (FLOAT)m_pMotionStack[currentmotion].EndFrame;
		}
		else
		{
			pAniData->m_AniFrame =  (FLOAT)m_pMotionStack[currentmotion].StartFrame;
		}

		pAniData->m_EventHandler.lastframe = 1;
		pAniData->m_FrameResult |= _XDEF_ACI_LASTFRAME;
	}
}

void _XBipedController::Advance( _XModelDescriptor* pAniData, bool& playsound, int eventlevel, FLOAT particlesize )
{
	int currentmotion = pAniData->m_CurMotion;
	
	if( m_pMotionStack[currentmotion].StartFrame >= (int)pAniData->m_AniFrame )
	{
		//pAniData->m_EventHandler.lastframe = 0;

		if( pAniData->m_EventHandler.lastframe )
			pAniData->ResetEventHandler();

		if( !pAniData->m_EventHandler.firstframe )
		{				
			pAniData->m_EventHandler.firstframe = true;
			pAniData->m_FrameResult = _XDEF_ACI_FIRSTFRAME;
		}
	}
	
	pAniData->m_AniFrame += (g_fElapsedFrameTime / (FLOAT)m_pMotionStack[pAniData->m_CurMotion].FrameSpeed);
	
	// Check last frame
	if( (int)pAniData->m_AniFrame >= (m_pMotionStack[currentmotion].EndFrame-1)  )
	{		
		pAniData->m_EventHandler.lastframe = 1;
		pAniData->m_FrameResult |= _XDEF_ACI_LASTFRAME;
	}
	
	if( pAniData->m_AniFrame >= (FLOAT)m_pMotionStack[currentmotion].EndFrame)
	{
		if( pAniData->m_LockEndFrame )
		{
			pAniData->m_AniFrame =  (FLOAT)m_pMotionStack[currentmotion].EndFrame;
		}
		else
		{
			pAniData->m_AniFrame =  (FLOAT)m_pMotionStack[currentmotion].StartFrame;
		}

		pAniData->m_EventHandler.lastframe = 1;
		pAniData->m_FrameResult |= _XDEF_ACI_LASTFRAME;
	}

	// 2초식 시작 frame check
	if((int)pAniData->m_AniFrame >= _XDEF_SECONDSEQUENCE_FRAME)
	{
		if(!pAniData->m_EventHandler.secondframe)
		{
			if(m_pMotionStack[currentmotion].EndFrame > _XDEF_SECONDSEQUENCE_FRAME)
			{
				pAniData->m_EventHandler.secondframe = 1;
				pAniData->m_FrameResult |= _XDEF_ACI_SECONDFRAME;
			}
		}
	}
	
	// 3초식 시작 frame check
	if((int)pAniData->m_AniFrame >= _XDEF_THIRDSEQUENCE_FRAME)
	{
		if(!pAniData->m_EventHandler.thirdframe)
		{
			if(m_pMotionStack[currentmotion].EndFrame > _XDEF_THIRDSEQUENCE_FRAME)
			{
				pAniData->m_EventHandler.thirdframe = 1;
				pAniData->m_FrameResult |= _XDEF_ACI_THIRDFRAME;
			}
		}
	}

	bool _playsound = playsound;
	
	for( int i = 0; i < _XDEF_MAXFRAMEEVENT; i++ )
	{
		// Check command frame
		if( m_pMotionStack[currentmotion].AttackCmdFrame[i] <= (BYTE)pAniData->m_AniFrame )
		{
			if( !pAniData->m_EventHandler.attackcmd_sended[i] )
			{
				pAniData->m_EventHandler.attackcmd_sended[i] = 1;
				pAniData->m_FrameResult |= _XDEF_ACI_COMMANDFRAME;
				pAniData->m_AttackClass    = m_pMotionStack[currentmotion].AttackClass[i];
				pAniData->m_WeaponClass    = m_pMotionStack[currentmotion].WeaponClass[i];
				pAniData->m_EffectPosition = m_pMotionStack[currentmotion].EffectPosition[i];
			}
		}
		
		// Check sound event frame...
		if( m_pMotionStack[currentmotion].SoundIndex[i] != _XDEF_NOEFFECTWORDVALUE )
		{
			if( m_pMotionStack[currentmotion].SoundOutFrame[i] <= (BYTE)pAniData->m_AniFrame )
			{	
				if( !pAniData->m_EventHandler.soundplayed[i] )
				{
					pAniData->m_EventHandler.soundplayed[i] = 1;

					if( _playsound )
					{
						playsound = false;

						if( pAniData->m_CameraDistance <= (g_LodTerrain.m_3PCamera.m_TargetDistance*3.0f) ) 
						{
							D3DVECTOR pos = { pAniData->m_Position._41, pAniData->m_Position._42, pAniData->m_Position._43 };
							_XPlaySoundEffect( m_pMotionStack[currentmotion].SoundIndex[i], pos, m_CheckSoundPlaying );
						}
					}
				}
			}
		}

		// Check effect event frame...
		if( pAniData->m_EffectManager )
		{
			if(  m_pMotionStack[currentmotion].EffectIndex[i] != _XDEF_NOEFFECTWORDVALUE )
			{
				if( m_pMotionStack[currentmotion].EffectFrame[i] <= (BYTE)pAniData->m_AniFrame )
				{								
					if( !pAniData->m_EventHandler.effectplayed[i] )
					{
						pAniData->m_EventHandler.effectplayed[i] = 1;

						_XParticleGroupItem* pNewParticleGroup = pAniData->m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, m_pMotionStack[currentmotion].EffectIndex[i]+eventlevel, &pAniData->m_Position );

						if( particlesize > 0.0f )
						{
							if( pNewParticleGroup )
							{
								pNewParticleGroup->SetParticleMaxSize( D3DXVECTOR3(particlesize,particlesize,particlesize) );
							}
						}
					}
				}
			}			
		}
	}	

	if( m_BaseEffectIndex > -1 )
	{
		if( pAniData->m_EffectManager )
		{
			if( !pAniData->m_EffectManager->FindInstance( m_BaseEffectIndex ) )
			{
				_XParticleGroupItem* pNewParticleGroup = pAniData->m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, m_BaseEffectIndex, &pAniData->m_Position );
			}
		}
	}
}


void _XBipedController::FrameAnimation( _XModelDescriptor* pAniData )
{
	int currentmotion = pAniData->m_CurMotion;

	//if( pAniData->m_AniLastFrame == pAniData->m_AniFrame ) return;

	int					frame;
	_LPXM_ROTATIONKEY	RotationSFrame=NULL, RotationEFrame=NULL;
	_LPXM_POSITIONKEY	PositionSFrame=NULL, PositionEFrame=NULL;
	_LPXM_BIPEDOBJECT	lpCh=NULL,   lpParent=NULL;
	

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{			
		lpCh			= GetNode(objindex );
		lpParent		= GetNode(lpCh->nParentId);

		D3DXMATRIX		d3dMatrix, mat;        
		D3DXQUATERNION	q;
		//_XMeshMath_SetIdentityMatrix(d3dMatrix);
		D3DXMatrixIdentity( &d3dMatrix );

		// Check animation list
		if(lpCh->m_pnRotKey[currentmotion] == 0 && lpCh->m_pnPosKey[currentmotion] == 0)
		{
			d3dMatrix = lpCh->m_localTM;
		}
		else
		{
			// Check rotation key
			if(lpCh->m_pnRotKey[currentmotion] == 0)
			{
				d3dMatrix = lpCh->m_localTM;
				d3dMatrix._41 = 0.0f;
				d3dMatrix._42 = 0.0f;
				d3dMatrix._43 = 0.0f;
				d3dMatrix._44 = 1.0f;
			}
			// Process rotate animation key
			else 
			{
				RotationSFrame = GetRotationAniKey(lpCh->m_pRotKey[currentmotion], lpCh->m_pnRotKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].StartFrame);
				RotationEFrame = GetRotationAniKey(lpCh->m_pRotKey[currentmotion], lpCh->m_pnRotKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].EndFrame);

				// Check start frame
				if(pAniData->m_AniFrame == (FLOAT)m_pMotionStack[currentmotion].StartFrame)	
				{	
					q = RotationSFrame->Qt;
				}
				// Check last frame
				else if(RotationEFrame->nTime <= pAniData->m_AniFrame) q = RotationEFrame->Qt;
				else
				{
					for(frame = 0; frame < lpCh->m_pnRotKey[currentmotion]; frame++)
					{
						if(lpCh->m_pRotKey[currentmotion][frame].nTime == pAniData->m_AniFrame)
						{
							q = lpCh->m_pRotKey[currentmotion][frame].Qt;
							break;
						}
						if(lpCh->m_pRotKey[currentmotion][frame  ].nTime < pAniData->m_AniFrame &&
						   lpCh->m_pRotKey[currentmotion][frame+1].nTime > pAniData->m_AniFrame )
						{
							// frame 과 frame+1으로 보간한 쿼터니언값..
							// 진행률은 ( 현재Frame - 이전 Key Frame )/( 다음 Key Frame - 이전 Key Frame );
							float sx, ex;
							sx = lpCh->m_pRotKey[currentmotion][frame  ].nTime;
							ex = lpCh->m_pRotKey[currentmotion][frame+1].nTime;

							float xx = (pAniData->m_AniFrame-sx)/(ex-sx);

							D3DXQuaternionSlerp(&q, &lpCh->m_pRotKey[currentmotion][frame  ].Qt, 
													&lpCh->m_pRotKey[currentmotion][frame+1].Qt, xx);
							break;
						}
					}
				}				
				_XMeshMath_MatrixFromQuaternion(d3dMatrix, q);				
			}

			// Check position key
			if(lpCh->m_pnPosKey[currentmotion] == 0) // 프레임이 1개. 즉 0번키..
			{
				d3dMatrix._41 = lpCh->m_localTM._41;
				d3dMatrix._42 = lpCh->m_localTM._42;
				d3dMatrix._43 = lpCh->m_localTM._43;
			}
			// Process position key
			else 
			{
				PositionSFrame = GetPositionAniKey(lpCh->m_pPosKey[currentmotion], lpCh->m_pnPosKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].StartFrame);
				PositionEFrame = GetPositionAniKey(lpCh->m_pPosKey[currentmotion], lpCh->m_pnPosKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].EndFrame);

				if(pAniData->m_AniFrame == (FLOAT)m_pMotionStack[currentmotion].StartFrame)
				{
					d3dMatrix._41 = PositionSFrame->d3dVKeyVal.x;
					d3dMatrix._42 = PositionSFrame->d3dVKeyVal.y;
					d3dMatrix._43 = PositionSFrame->d3dVKeyVal.z;
				}
				else if(PositionEFrame->nTime <= pAniData->m_AniFrame)
				{
					d3dMatrix._41 = PositionEFrame->d3dVKeyVal.x;
					d3dMatrix._42 = PositionEFrame->d3dVKeyVal.y;
					d3dMatrix._43 = PositionEFrame->d3dVKeyVal.z;
				}
				else
				{
					for(frame = 0; frame < lpCh->m_pnPosKey[currentmotion]; frame++)
					{
						if(lpCh->m_pPosKey[currentmotion][frame].nTime == pAniData->m_AniFrame)
						{
							d3dMatrix._41 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x;
							d3dMatrix._42 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y;
							d3dMatrix._43 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z;
							break;
						}
						if(lpCh->m_pPosKey[currentmotion][frame  ].nTime < pAniData->m_AniFrame && 
						   lpCh->m_pPosKey[currentmotion][frame+1].nTime > pAniData->m_AniFrame )
						{
							// frame 과 frame+1으로 보간한 쿼터니언값..
							// 진행률은 ( 현재Frame - 이전 Key Frame )/( 다음 Key Frame - 이전 Key Frame );
							float sx, ex;
							sx = lpCh->m_pPosKey[currentmotion][frame  ].nTime;
							ex = lpCh->m_pPosKey[currentmotion][frame+1].nTime;
							
							float xx = (pAniData->m_AniFrame-sx)/(ex-sx);

							// 벡터 보간..
							d3dMatrix._41 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x + 
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.x - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x ) * xx;

							d3dMatrix._42 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y +
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.y - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y ) * xx;

							d3dMatrix._43 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z +
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.z - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z ) * xx;
							break;
						}
					}
				}
			}
		}
		_XMeshMath_UnitMatrixFromMatrix(d3dMatrix);

		if(lpCh->nParentId == -1)
		{
			//자식을위해 결과매트릭스를 저장
			lpCh->m_matAniTM = d3dMatrix;
		}
		else
		{
			// d3dMatrix은 오브젝트의 Local Ani TM 이된다..
			D3DXMatrixMultiply( &lpCh->m_matAniTM, &d3dMatrix, &lpParent->m_matAniTM  );
		}

		D3DXMatrixMultiply( &lpCh->m_matLocalAni, &lpCh->m_matInvTM , &lpCh->m_matAniTM);

		if( pAniData->m_EnableEffectProcess )
		{
			for (int i = 0; i < _XParticleLinkBone_MaxCount; ++ i)
			{
				if( objindex == m_CharacterPartIndex[i] )
				{
					pAniData->m_PartMatrixContainer[i] = lpCh->m_matLocalAni;
					break;
				}
			}

/*			//이펙트노드의  결과 메트릭스			
			if( objindex == m_CharacterPartIndex[0] )		pAniData->m_PartMatrixContainer[0] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[1] )	pAniData->m_PartMatrixContainer[1] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[2] )  pAniData->m_PartMatrixContainer[2] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[3] )  pAniData->m_PartMatrixContainer[3] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[4] )  pAniData->m_PartMatrixContainer[4] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[5] )  pAniData->m_PartMatrixContainer[5] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[6] )  pAniData->m_PartMatrixContainer[6] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[7] )  pAniData->m_PartMatrixContainer[7] = lpCh->m_matLocalAni;
*/
		}
		
	}
		
	//pAniData->m_AniLastFrame	= pAniData->m_AniFrame;
}


#define _XDEF_SLERPANIMATIONTIME	5.0f

void _XBipedController::SlerpAnimation( _XModelDescriptor* pAniData )
{			
	int   framespeed = m_pMotionStack[pAniData->m_CurMotion].FrameSpeed;
		
	pAniData->m_SlerpAniFrame += (g_fElapsedFrameTime / (FLOAT)framespeed);
		
	if( pAniData->m_SlerpAniFrame >= _XDEF_SLERPANIMATIONTIME)
	{
		pAniData->m_AniFrame = (FLOAT)m_pMotionStack[pAniData->m_CurMotion].StartFrame;
		pAniData->m_MotionChangeFlag = false;
	}

	_LPXM_ROTATIONKEY	RotationSFrame=NULL, RotationEFrame=NULL;
	_LPXM_POSITIONKEY	PositionSFrame=NULL, PositionEFrame=NULL;
	_LPXM_BIPEDOBJECT lpCh=NULL, lpParent=NULL;	

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{
		lpCh		= GetNode(objindex);
		lpParent	= GetNode(lpCh->nParentId);

		D3DXMATRIX		d3dMatrix, mat;
		D3DXQUATERNION	q;

        D3DXMatrixIdentity(&d3dMatrix);

		// Check animation key list
		if(lpCh->m_pnRotKey[pAniData->m_CurMotion] == 0 && lpCh->m_pnPosKey[pAniData->m_CurMotion] == 0) // none...
		{
			d3dMatrix = lpCh->m_localTM; 
		}
		else
		{
			// Check rotate key
			if(lpCh->m_pnRotKey[pAniData->m_CurMotion] == 0) 
			{ 
				d3dMatrix = lpCh->m_localTM;
				d3dMatrix._41 = 0.0f;
				d3dMatrix._42 = 0.0f;
				d3dMatrix._43 = 0.0f;
				d3dMatrix._44 = 1.0f;
			}
			else if(lpCh->m_pnRotKey[pAniData->m_PrevMotion] == 0)
			{
				d3dMatrix = lpCh->m_localTM;
				d3dMatrix._41 = 0.0f;
				d3dMatrix._42 = 0.0f;
				d3dMatrix._43 = 0.0f;
				d3dMatrix._44 = 1.0f;
			}
			else
			{
				RotationSFrame = GetRotationAniKey(lpCh->m_pRotKey[pAniData->m_PrevMotion], lpCh->m_pnRotKey[pAniData->m_PrevMotion], pAniData->m_PrevMotionFrame);
				RotationEFrame = GetRotationAniKey(lpCh->m_pRotKey[pAniData->m_CurMotion], lpCh->m_pnRotKey[pAniData->m_CurMotion], (FLOAT)m_pMotionStack[pAniData->m_CurMotion].StartFrame);

				if( pAniData->m_SlerpAniFrame >= _XDEF_SLERPANIMATIONTIME) q = RotationEFrame->Qt;
				else
				{
					float sx, ex;
					sx = 0.0f;
					ex = _XDEF_SLERPANIMATIONTIME;

					float xx = (pAniData->m_SlerpAniFrame-sx)/(ex-sx);

					D3DXQuaternionSlerp(&q, &RotationSFrame->Qt, &RotationEFrame->Qt, xx);
				}
				_XMeshMath_MatrixFromQuaternion(d3dMatrix, q);
			}

			// Check position key list
			if(lpCh->m_pnPosKey[pAniData->m_CurMotion] == 0) // 프레임이 1개. 0번키..
			{
				d3dMatrix._41 = lpCh->m_localTM._41;
				d3dMatrix._42 = lpCh->m_localTM._42;
				d3dMatrix._43 = lpCh->m_localTM._43;
			}
			else if(lpCh->m_pnPosKey[pAniData->m_PrevMotion] == 0)
			{
				d3dMatrix._41 = lpCh->m_localTM._41;
				d3dMatrix._42 = lpCh->m_localTM._42;
				d3dMatrix._43 = lpCh->m_localTM._43;
			}
			else	// process position key
			{
				PositionSFrame = GetPositionAniKey(lpCh->m_pPosKey[pAniData->m_PrevMotion], lpCh->m_pnPosKey[pAniData->m_PrevMotion], pAniData->m_PrevMotionFrame);
				PositionEFrame = GetPositionAniKey(lpCh->m_pPosKey[pAniData->m_CurMotion], lpCh->m_pnPosKey[pAniData->m_CurMotion], (float)m_pMotionStack[pAniData->m_CurMotion].StartFrame);

				if( pAniData->m_SlerpAniFrame >= _XDEF_SLERPANIMATIONTIME)
				{
					d3dMatrix._41 = PositionEFrame->d3dVKeyVal.x;
					d3dMatrix._42 = PositionEFrame->d3dVKeyVal.y;
					d3dMatrix._43 = PositionEFrame->d3dVKeyVal.z;
				}
				else
				{
					// frame & frame+1 으로 보간한 쿼터니언값..
					// 진행률은 ( 현재Frame - 이전 Key Frame )/( 다음 Key Frame - 이전 Key Frame );
					float sx, ex;
					sx = 0.0f;
					ex = _XDEF_SLERPANIMATIONTIME;
					
					float xx = (pAniData->m_SlerpAniFrame-sx)/(ex-sx);

					// 벡터 보간..
					d3dMatrix._41 = PositionSFrame->d3dVKeyVal.x + ( PositionEFrame->d3dVKeyVal.x - PositionSFrame->d3dVKeyVal.x ) * xx;
					d3dMatrix._42 = PositionSFrame->d3dVKeyVal.y + ( PositionEFrame->d3dVKeyVal.y - PositionSFrame->d3dVKeyVal.y ) * xx;
					d3dMatrix._43 = PositionSFrame->d3dVKeyVal.z + ( PositionEFrame->d3dVKeyVal.z - PositionSFrame->d3dVKeyVal.z ) * xx;
				}
			}
		}
		_XMeshMath_UnitMatrixFromMatrix(d3dMatrix);

		if(lpCh->nParentId == -1)
		{			
			lpCh->m_matAniTM = d3dMatrix; //자식을위해 결과매트릭스를 저장해둔다
		}
		else
		{
			// d3dMatrix은 오브젝트의 Local Ani TM 이된다..
			D3DXMatrixMultiply(&lpCh->m_matAniTM, &d3dMatrix, &lpParent->m_matAniTM);
		}

		D3DXMatrixMultiply(&lpCh->m_matLocalAni, &lpCh->m_matInvTM, &lpCh->m_matAniTM); 

		if( pAniData->m_EnableEffectProcess )
		{
			for (int i = 0; i < _XParticleLinkBone_MaxCount; ++ i)
			{
				if( objindex == m_CharacterPartIndex[i] )
				{
					pAniData->m_PartMatrixContainer[i] = lpCh->m_matLocalAni;
					break;
				}
			}
/*			//이펙트노드의  결과 메트릭스
			if( objindex == m_CharacterPartIndex[0] )		pAniData->m_PartMatrixContainer[0] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[1] )	pAniData->m_PartMatrixContainer[1] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[2] )	pAniData->m_PartMatrixContainer[2] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[3] )	pAniData->m_PartMatrixContainer[3] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[4] )	pAniData->m_PartMatrixContainer[4] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[5] )	pAniData->m_PartMatrixContainer[5] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[6] )	pAniData->m_PartMatrixContainer[6] = lpCh->m_matLocalAni;
			else if( objindex == m_CharacterPartIndex[7] )	pAniData->m_PartMatrixContainer[7] = lpCh->m_matLocalAni;
*/
		}
	}	
}

//#ifndef _XDWDEBUG 
#define _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE
//#endif


BOOL _XLoadCharacterBipedScripter(  _XBipedController* pBipedController, LPSTR szFilename )
{
	if( !pBipedController || !szFilename || !szFilename[0] ) return FALSE;

	FILE* pScripterFile = NULL;	

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	pScripterFile = g_ScriptArchive.GetPackedFile( szFilename );
#else
	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	pScripterFile = fopen( szFilename, "rt" );
#endif	
	
	if( !pScripterFile )
	{
		_XFatalError( "_XBipedController::LoadScripter( %s ) File open", szFilename );
		return FALSE; 
	}
	
	
	TCHAR argIDString[256];
	TCHAR argValueString[768];
	TCHAR parsingString[768];

	while( 1 )
	{				
		if( feof(pScripterFile) ) break;
	
		memset( parsingString, 0, 768 );
		if( !fgets( parsingString, 767, pScripterFile ) ) break;

		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) 
			continue;

		memset( argIDString, 0, 256 );
		memset( argValueString, 0, 768 );
		sscanf( parsingString, "%s %s", argIDString, argValueString );

		if( strncmp( argIDString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}
		else if( strcmpi( argIDString, "_XM_BASEMODEL" ) == 0 )
		{
			if( !pBipedController->Load( g_ModelArchive.GetPackedFile( argValueString ), argValueString ) ) break;
		}
		else if( strcmpi( argIDString, "_XM_RESERVEANISTACK" )== 0 )
		{
			int stackcount = atoi( argValueString );
			if( !pBipedController->ReserveAnimationStack( stackcount ) ) break;
		}
		else if( strcmpi( argIDString, "_XM_ANIMATION" ) == 0 )
		{
//			_XDWINPRINT("%s - %d", argValueString, pBipedController->GetMotionCnt());
			if( !pBipedController->LoadKey( g_AniArchive.GetPackedFile( argValueString), argValueString ) ) break;
		}
		else if( strcmpi( argIDString, "_XM_MOTIONNAME" ) == 0 )
		{
		}
		else if( strcmpi( argIDString, "_XM_ATTACKFRAME" ) == 0 )
		{
			_LPXM_MOTION pMotion = pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 );
			if( pMotion )
			{
				int	 commandframe[9] = { 255, 255, 255, 255, 255, 255, 255, 255, 255 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, &commandframe[0], &commandframe[1], 
																					 &commandframe[2], &commandframe[3],
																					 &commandframe[4], &commandframe[5],
																					 &commandframe[6], &commandframe[7],
																					 &commandframe[8] );
				for( int i = 0; i < 9; i++ )
				{
					if( commandframe[i] == 255 ) 
					{
						pMotion->SetAttackFrame( i, _XDEF_NOEFFECTVALUE );
					}
					else
					{
						pMotion->SetAttackFrame( i, commandframe[i] );
					}
				}
				/*
				if( commandframe[0] == 255 ) commandframe[0] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[1] == 255 ) commandframe[1] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[2] == 255 ) commandframe[2] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[3] == 255 ) commandframe[3] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[4] == 255 ) commandframe[4] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[5] == 255 ) commandframe[5] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[6] == 255 ) commandframe[6] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[7] == 255 ) commandframe[7] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[8] == 255 ) commandframe[8] = _XDEF_NOEFFECTVALUE; 

				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 0, commandframe[0] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 1, commandframe[1] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 2, commandframe[2] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 3, commandframe[3] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 4, commandframe[4] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 5, commandframe[5] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 6, commandframe[6] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 7, commandframe[7] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackFrame( 8, commandframe[8] );
				*/
			}
		}
		else if( strcmpi( argIDString, "_XM_ATTACKCLASS" ) == 0 )
		{
			_LPXM_MOTION pMotion = pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 );
			if( pMotion )
			{
				int	 attackclass[9] = { _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, 
					                    _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW,
										_XDEF_ATTACKCLASS_LOW };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
					&attackclass[0], &attackclass[1],  &attackclass[2], &attackclass[3], &attackclass[4],
					&attackclass[5],  &attackclass[6], &attackclass[7], &attackclass[8]	);
				
				for( int i = 0; i < 9; i++ )
				{
					if( attackclass[i] == 0 )
					{
						pMotion->SetAttackClass( i, _XDEF_ATTACKCLASS_LOW );
					}
					else
					{
						pMotion->SetAttackClass( i, attackclass[i] );
					}
				}
				/*
				if( attackclass[0] == 0 ) attackclass[0] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[1] == 0 ) attackclass[1] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[2] == 0 ) attackclass[2] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[3] == 0 ) attackclass[3] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[4] == 0 ) attackclass[4] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[5] == 0 ) attackclass[5] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[6] == 0 ) attackclass[6] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[7] == 0 ) attackclass[7] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[8] == 0 ) attackclass[8] = _XDEF_ATTACKCLASS_LOW; 
				
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 0, attackclass[0] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 1, attackclass[1] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 2, attackclass[2] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 3, attackclass[3] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 4, attackclass[4] );				
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 5, attackclass[5] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 6, attackclass[6] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 7, attackclass[7] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetAttackClass( 8, attackclass[8] );				
				*/
			}
		}
		else if( strcmpi( argIDString, "_XM_WEAPONCLASS" ) == 0 )
		{
			_LPXM_MOTION pMotion = pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 );
			if( pMotion )
			{
				int	 weaponclass[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
					    &weaponclass[0], &weaponclass[1],  &weaponclass[2], &weaponclass[3], &weaponclass[4],
						&weaponclass[5],  &weaponclass[6], &weaponclass[7], &weaponclass[8] );

				for( int i = 0; i < 9; i++ )
				{
					pMotion->SetWeaponClass( i, weaponclass[i] );
				}
				
				/*
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 0, weaponclass[0] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 1, weaponclass[1] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 2, weaponclass[2] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 3, weaponclass[3] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 4, weaponclass[4] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 5, weaponclass[5] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 6, weaponclass[6] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 7, weaponclass[7] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetWeaponClass( 8, weaponclass[8] );
				*/
			}
		}
		else if( strcmpi( argIDString, "_XM_WEAPONITEMCLASS" ) == 0 )
		{
			if(pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 ))
			{
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->UseItemIndex = atoi( argValueString );
			}
		}		
		else if( strcmpi( argIDString, "_XM_SOUNDEFFECT" ) == 0 )
		{
			_LPXM_MOTION pMotion = pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 );
			if( pMotion )
			{
				int	 soundframe[9];
				TCHAR soundfilename[9][64];
				memset( soundfilename, 0, sizeof(TCHAR) * 64 * 9 );
				sscanf( parsingString, "%s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s", argIDString, 
						&soundframe[0], soundfilename[0], &soundframe[1], soundfilename[1],
						&soundframe[2], soundfilename[2], &soundframe[3], soundfilename[3],
						&soundframe[4], soundfilename[4], &soundframe[5], soundfilename[5],
						&soundframe[6], soundfilename[6], &soundframe[7], soundfilename[7],
						&soundframe[8], soundfilename[8] );
				
				int soundresource[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };

				for( int i = 0; i < 9; i++ )
				{
					if( strlen(soundfilename[i]) )
					{
#ifdef _XUSEFMOD
						soundresource[i] = g_FMODWrapper.InsertSound( soundfilename[i], FSOUND_HW3D );
						if( soundresource[i] == -1 )
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
							//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
						}
#else						
						soundresource[i] = g_SoundEffectArchive.FindResource( soundfilename[i] );
						if( soundresource[i] == -1 )
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
							//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
						}
#endif
					}
					else 
					{
						soundframe[i] = _XDEF_NOEFFECTVALUE;
						soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
					}

					pMotion->SetSoundEffectFrame( i, soundframe[i], soundresource[i] );						
				}								
			}
		}
		else if( strcmpi( argIDString, "_XM_SPECIALEFFECT" ) == 0 )
		{
			_LPXM_MOTION pMotion = pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 );
			if( pMotion )
			{
				int	 effectframe[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
				int  effectid[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", argIDString, 
					&effectframe[0], &effectid[0], &effectframe[1], &effectid[1],
					&effectframe[2], &effectid[2], &effectframe[3], &effectid[3],
					&effectframe[4], &effectid[4], &effectframe[5], &effectid[5],
					&effectframe[6], &effectid[6], &effectframe[7], &effectid[7],
					&effectframe[8], &effectid[8] );
				
				/*
				if( effectframe[0] == -1 ){ effectframe[0] = _XDEF_NOEFFECTVALUE; effectid[0] = _XDEF_NOEFFECTWORDVALUE; }
				if( effectframe[1] == -1 ){ effectframe[1] = _XDEF_NOEFFECTVALUE; effectid[1] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[2] == -1 ){ effectframe[2] = _XDEF_NOEFFECTVALUE; effectid[2] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[3] == -1 ){ effectframe[3] = _XDEF_NOEFFECTVALUE; effectid[3] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[4] == -1 ){ effectframe[4] = _XDEF_NOEFFECTVALUE; effectid[4] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[5] == -1 ){ effectframe[5] = _XDEF_NOEFFECTVALUE; effectid[5] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[6] == -1 ){ effectframe[6] = _XDEF_NOEFFECTVALUE; effectid[6] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[7] == -1 ){ effectframe[7] = _XDEF_NOEFFECTVALUE; effectid[7] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[8] == -1 ){ effectframe[8] = _XDEF_NOEFFECTVALUE; effectid[8] = _XDEF_NOEFFECTWORDVALUE; } 
				*/
				
				for( int i = 0; i < 9; i++ )
				{
					if( effectframe[i] == -1 )
					{ 
						pMotion->SetEffectFrame( i, _XDEF_NOEFFECTVALUE, _XDEF_NOEFFECTWORDVALUE );
					}
					else
					{
						pMotion->SetEffectFrame( i, effectframe[i], effectid[i] );
					}
				}								
			}
		}
		else if( strcmpi( argIDString, "_XM_EPOSITION" ) == 0 )
		{
			_LPXM_MOTION pMotion = pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 );
			if( pMotion )
			{
				int	 effectpositionclass[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
					&effectpositionclass[0], &effectpositionclass[1], &effectpositionclass[2], &effectpositionclass[3], &effectpositionclass[4],
					&effectpositionclass[5], &effectpositionclass[6], &effectpositionclass[7], &effectpositionclass[8] );
				
				for( int i = 0; i < 9; i++ )
				{
					pMotion->SetEffectPosition( i, effectpositionclass[i] );
				}
					
				/*
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 0, effectpositionclass[0] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 1, effectpositionclass[1] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 2, effectpositionclass[2] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 3, effectpositionclass[3] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 4, effectpositionclass[4] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 5, effectpositionclass[5] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 6, effectpositionclass[6] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 7, effectpositionclass[7] );
				pBipedController->GetMotion( pBipedController->GetMotionCnt()-1 )->SetEffectPosition( 8, effectpositionclass[8] );
				*/
			}
		}
	};

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
	g_ScriptArchive.ClosePackage();					
#else
	fclose( pScripterFile );
#endif
	
	return TRUE;
}



BOOL _XLoadModelScripter( LPCTSTR szFilename )
{
	if( !szFilename || !szFilename[0] ) return FALSE;

	FILE* pScripterFile = NULL;

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	pScripterFile = g_ScriptArchive.GetPackedFile( (LPTSTR)szFilename );
#else
	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	pScripterFile = fopen( szFilename, "rt" );
#endif	
	
	if( !pScripterFile )
	{
		_XFatalError( "_XLoadModelScripter( %s ) File open", szFilename );
		return FALSE; 
	}

	TCHAR argIDString[256];
	TCHAR argValueString[256];
	TCHAR parsingString[512];
	TCHAR lodname[2][128];

	int modeltype  = 0;
	int modelindex = 0;
	
	while( 1 )
	{				
		if( feof(pScripterFile) ) break;
			
		memset( parsingString, 0, 512 );
		if( !fgets( parsingString, 512, pScripterFile ) ) break;
		
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;

		memset( argIDString, 0, 256 );
		memset( argValueString, 0, 256 );
		memset( lodname[0], 0, sizeof(TCHAR)*128 );
		memset( lodname[1], 0, sizeof(TCHAR)*128 );
		
		sscanf( parsingString, "%s %s %s %s", argIDString, argValueString, lodname[0], lodname[1] );

		// Model type setting
		if( strncmp( argIDString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}		
		else if( strcmpi( argIDString, "_XM_MODEL_GENDERSEPARATECOUNT" ) == 0 )
		{
			g_Model_GenderSeparateIndex = atoi(argValueString);
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_HAIR" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_HAIR;			
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_FACE" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_FACE;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_UPPERBODY" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_UPPERBODY;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_LOWERBODY" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_LOWERBODY;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_LHAND" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_LHAND;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_RHAND" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_RHAND;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_LFOOT" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_LFOOT;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_RFOOT" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_RFOOT;
			modelindex		= 0; // 인덱스 초기화 
		}
		/*else if( strcmpi( argIDString, "_XM_MODELCLUSTER_ADAE" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_ADAE;
			modelindex		= 0; // 인덱스 초기화 
		}*/
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_EARRING" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_EARRING;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_CAP" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_CAP;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_BACKPACK" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_BACKPACK;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_WEAPON" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_WEAPON;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_MARTIALBOOK" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_MARTIALBOOK;
			modelindex		= 0; // 인덱스 초기화 
		}		
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_POTION" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_POTION;
			modelindex		= 0; // 인덱스 초기화 
		}		
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_THROW" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_THROW;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_SOCERY" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_SOCERY;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_ETC" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_ETC;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_JEWELBOX" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_JEWELBOX;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_ELIXIR" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_ELIXIR;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_MASK" ) == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_MASK;
			modelindex		= 0; // 인덱스 초기화 
		}
		else if( strcmpi( argIDString, "_XM_MODELCLUSTER_FACEMASK") == 0 )
		{
			modeltype		= _XDEF_MODELCLUSTER_FACEMASK;
			modelindex		= 0; // 인덱스 초기화
		}
		// Model index
		else if( strcmpi( argIDString, "_XM_MODELINDEX" ) == 0 )
		{
			modelindex			= atoi(argValueString);
		}		
		else if( strcmpi( argIDString, "_XM_LOADMODEL" ) == 0 )
		{				
			_XAnimMeshAbstractClass* modelcontainner = NULL;
			_XAnimMeshAbstractClass* modelcontainner2 = NULL;
			
			switch( modeltype )
			{
			case _XDEF_MODELCLUSTER_HAIR :
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_HAIR )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_HairType[modelindex];
				break;
			case  _XDEF_MODELCLUSTER_FACE:
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_FACE )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_FaceType[modelindex];
				break;
			case  _XDEF_MODELCLUSTER_UPPERBODY:
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_UBODY )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_UBodyType[modelindex];
				break;
			case  _XDEF_MODELCLUSTER_LOWERBODY:
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_LBODY )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_LBodyType[modelindex];
				break;
			case  _XDEF_MODELCLUSTER_LHAND:
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_LHAND )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_LHandType[modelindex];
				break;			
			case  _XDEF_MODELCLUSTER_RHAND:
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_RHAND )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_RHandType[modelindex];
				break;
			case  _XDEF_MODELCLUSTER_LFOOT:
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_LFOOT )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_LFootType[modelindex];
				break;
			case _XDEF_MODELCLUSTER_RFOOT :
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_RFOOT )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_RFootType[modelindex];
				break;
			//case _XDEF_MODELCLUSTER_ADAE :
			//	modelcontainner = g_UserModel_Adae[modelindex];
			//	break;
			case _XDEF_MODELCLUSTER_EARRING :
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_EARRING )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_Earring[modelindex];
				break;			
			case _XDEF_MODELCLUSTER_BACKPACK :
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_BACKPACK )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_BackPack[modelindex];
				break;			
			case _XDEF_MODELCLUSTER_CAP :
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_CAP )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_Cap[modelindex];
				break;		
			case _XDEF_MODELCLUSTER_WEAPON :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_WEAPON )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_Weapon[0][modelindex];
				modelcontainner2= g_ItemModel_Weapon[1][modelindex];
				break;
			case _XDEF_MODELCLUSTER_MARTIALBOOK :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_MARTIALBOOK )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_MartialBook[modelindex];
				break;
			case _XDEF_MODELCLUSTER_POTION :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_POTION )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_Potion[modelindex];
				break;
			case _XDEF_MODELCLUSTER_THROW :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_THROW )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_Throw[modelindex];
				break;
			case _XDEF_MODELCLUSTER_SOCERY :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_SOCERY )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_Socery[modelindex];
				break;			
			case _XDEF_MODELCLUSTER_ETC :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_ETC )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_ETC[modelindex];
				break;
			case _XDEF_MODELCLUSTER_JEWELBOX :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_JEWELBOX )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_JewelBox[modelindex];
				break;
			case _XDEF_MODELCLUSTER_ELIXIR :
				if( modelindex >= _XDEF_ITEMMODELTYPECOUNT_ELIXIR )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_ItemModel_Elixir[modelindex];
				break;
			case _XDEF_MODELCLUSTER_MASK :
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_MASK )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_Mask[modelindex];
				break;
			case _XDEF_MODELCLUSTER_FACEMASK :
				if( modelindex >= _XDEF_USERMODELTYPECOUNT_FACEMASK )
				{
					_XFatalError( _T("Overflow model index : %d"), modelindex );
					break;
				}
				modelcontainner = g_UserModel_FaceMask[modelindex];
				break;
			default :
				break;
			}

			// 하나가 로딩되면 자동 카운트
			modelindex++;
			
			if( modelcontainner )
			{
				if( modeltype == _XDEF_MODELCLUSTER_WEAPON )
				{
					TCHAR tempString[64];
					memset( tempString, 0, 64 );
					tempString[0] = _T('M');
					strcat( tempString, argValueString );


#ifdef _XDEF_MODELTHREADLOADING
					modelcontainner->m_strFileName = tempString;
					modelcontainner->SetTextureArchive( &g_CharacterTextureArchive );

#else
					if( !modelcontainner->Load(g_ModelArchive.GetPackedFile(tempString), 
												&g_CharacterTextureArchive, 
												!_XAnimMeshAbstractClass::m_UseVertexShaderAnimation,
											    tempString ) )
					{	
	#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
						g_ScriptArchive.ClosePackage();					
	#else
						fclose( pScripterFile );
	#endif
						return FALSE;
					}
#endif //_XDEF_MODELTHREADLOADING

					tempString[0] = _T('W');
					
#ifdef _XDEF_MODELTHREADLOADING
					modelcontainner2->m_strFileName = tempString;
					modelcontainner2->SetTextureArchive( &g_CharacterTextureArchive );

#else
					if( !modelcontainner2->Load(g_ModelArchive.GetPackedFile(tempString),
												&g_CharacterTextureArchive, 
												!_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
												tempString) )
					{	
#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
						g_ScriptArchive.ClosePackage();					
#else
						fclose( pScripterFile );
#endif
						return FALSE;
					}
#endif // _XDEF_MODELTHREADLOADING
				}
				else
				{
#ifdef _XDEF_MODELTHREADLOADING
					modelcontainner->m_strFileName = argValueString;
					modelcontainner->SetTextureArchive( &g_CharacterTextureArchive );
#else
					if( !modelcontainner->Load(g_ModelArchive.GetPackedFile(argValueString), 
											  &g_CharacterTextureArchive, 
											  !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
											  argValueString ) )
					{	
#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
						g_ScriptArchive.ClosePackage();					
#else
						fclose( pScripterFile );
#endif
						return FALSE;
					}
#endif //#ifdef _XDEF_MODELTHREADLOADING
					int lodcount = 0;
					if( lodname[0][0] ) lodcount++;
					if( lodname[1][0] ) lodcount++;

#ifdef _XDEF_MODELTHREADLOADING						
#else					
					if( lodcount )
						modelcontainner->CreateLODObject( lodcount );
#endif

					for( int ilod = 0; ilod < lodcount; ilod++ )
					{
#ifdef _XDEF_MODELTHREADLOADING						
						modelcontainner->m_strLodFileName[ilod] = lodname[ilod];
						
#else
						if( !modelcontainner->m_pLODObjectList[ilod]->Load(g_ModelArchive.GetPackedFile(lodname[ilod]), 
																	     &g_CharacterTextureArchive, 
																		 !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
																		 lodname[ilod] ) )
						{	
#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
							g_ScriptArchive.ClosePackage();
#else
							fclose( pScripterFile );
#endif
							return FALSE;
						}
#endif //#ifdef _XDEF_MODELTHREADLOADING
					}
				}
			}
		}
	}

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	g_ScriptArchive.ClosePackage();
#else
	fclose( pScripterFile );
#endif

	return TRUE;
}


BOOL _XLoadNPCModelScripter( LPCTSTR szFilename )
{
	if( !szFilename || !szFilename[0] ) return FALSE;

	FILE* pScripterFile = NULL;

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	pScripterFile = g_ScriptArchive.GetPackedFile( (LPTSTR)szFilename );
#else
	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	pScripterFile = fopen( szFilename, "rt" );
#endif
	
	if( !pScripterFile )
	{
		_XFatalError( "_XLoadNPCModelScripter( %s ) File open", szFilename );
		return FALSE; 
	}
	
	TCHAR argIDString[512];
	TCHAR argValueString[768];
	TCHAR parsingString[768];

	int modelindex = 0;

	BOOL retvalue =  TRUE;
	BOOL InstanceModel =  TRUE;
	BOOL InstanceBiped =  TRUE;
		
	while( 1 )
	{				
		if( feof(pScripterFile) ) break;
	
		memset( parsingString, 0, 768 );
		if( !fgets( parsingString, 767, pScripterFile ) ) break;
		
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;

		memset( argIDString, 0, 512 );
		memset( argValueString, 0, 768 );
		sscanf( parsingString, "%s %s", argIDString, argValueString );

		// Model type setting
		if( strncmp( argIDString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}		
		// Model name
		else if( strcmpi( argIDString, "_XM_MODELNAME" ) == 0 )
		{
			memset( g_NPCName[modelindex], 0, sizeof(TCHAR) * 41 );
			
			LPTSTR pname = NULL;
			pname = strstr( parsingString, argValueString );
			if( pname )
				strcpy( g_NPCName[modelindex], pname );
		}
		// Model index
		else if( strcmpi( argIDString, "_XM_MODELINDEX" ) == 0 )
		{
			modelindex	= atoi(argValueString);
			if( modelindex >= _XDEF_MODELCLASSCOUNT_NPC )
			{
				_XFatalError( "_XLoadNPCModelScripter( %s ) : Invalid model index", szFilename );
				modelindex = _XDEF_MODELCLASSCOUNT_NPC-1;
			}
		}		
		else if( strcmpi( argIDString, "_XM_BIPEDMODEL" ) == 0 )
		{
			g_NPCBIPEDMODEL[modelindex].m_strBipedName = argValueString;
		}
		else if( strcmpi( argIDString, "_XM_RESERVEANISTACK" )== 0 )
		{
			if (InstanceBiped)
			{
				int stackcount = atoi( argValueString );
				g_NPCBIPEDMODEL[modelindex].m_svTempMotion.reserve(stackcount);
			}
		}
		else if( strcmpi( argIDString, "_XM_BASEMODEL" ) == 0 )
		{
			InstanceModel = g_Npclist.CreateAnimMeshAbstractClass(&g_NPCMODEL[modelindex], argValueString);
		}
		else if( strcmpi( argIDString, "_XM_ANIMATION" ) == 0 )
		{
			TCHAR motiontypename[64];
			TCHAR animationfilename[64];
			memset( motiontypename, 0, sizeof(TCHAR) * 64 );
			memset( animationfilename, 0, sizeof(TCHAR) * 64 );
			sscanf( parsingString, "%s %s %s", argIDString, motiontypename, animationfilename );

			if( strlen( motiontypename ) <= 0 )
			{
				_XFatalError( "NPC Descripter : Invalid motion name : [ MI : %d ]", modelindex );
			}

			if( strlen( animationfilename ) <= 0 )
			{
				_XFatalError( "NPC Descripter : Invalid animation file name : [ MI : %d ]", modelindex );
			}
			
			if( strcmpi( motiontypename, _T("IDLE") ) == 0 )
			{				
				g_NPCMotionExistTable[modelindex].motion_idle = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("IDLEACTION1") ) == 0 )
			{				
				g_NPCMotionExistTable[modelindex].motion_idleaction1 = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("IDLEACTION2") ) == 0 )
			{				
				g_NPCMotionExistTable[modelindex].motion_idleaction2 = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("CONVERSATION") ) == 0 )
			{				
				g_NPCMotionExistTable[modelindex].motion_conversation = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("WALK") ) == 0 )
			{
				g_NPCMotionExistTable[modelindex].motion_walk = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("WALK2") ) == 0 )
			{
				g_NPCMotionExistTable[modelindex].motion_walk2 = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("RUN") ) == 0 )
			{
				g_NPCMotionExistTable[modelindex].motion_run = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("ATTACK") ) == 0 )
			{
				g_NPCMotionExistTable[modelindex].motion_attack = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("DAMAGE") ) == 0 )
			{
				g_NPCMotionExistTable[modelindex].motion_damage = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("DIE") ) == 0 )
			{
				g_NPCMotionExistTable[modelindex].motion_die = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("STANDUP") ) == 0 )
			{
				g_NPCMotionExistTable[modelindex].motion_standup = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if(strcmpi(motiontypename, _T("SURRENDER")) == 0)
			{
				// 장원전 처리
				g_NPCMotionExistTable[modelindex].motion_damage = g_NPCBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else
			{
				_XFatalError( "Unknown NPC motion name" );
				retvalue = FALSE;
				break;
			}

			TEMP_MOTION* pTempBipedNode = new TEMP_MOTION;
			pTempBipedNode->strKeyname = animationfilename;
			g_NPCBIPEDMODEL[modelindex].m_svTempMotion.push_back(pTempBipedNode);
		}
		/*else if( strcmpi( argIDString, "_XM_ATTACKFRAME" ) == 0 )
		{
			if( g_NPCBIPEDMODEL[modelindex].GetMotion( g_NPCBIPEDMODEL[modelindex].GetMotionCnt()-1 ) )
			{
				BYTE attckframe = atoi( argValueString );
				g_NPCBIPEDMODEL[modelindex].GetMotion( g_NPCBIPEDMODEL[modelindex].GetMotionCnt()-1 )->SetAttackFrame( attckframe );
			}
		}*/
		else if( strcmpi( argIDString, "_XM_SOUNDEFFECT" ) == 0 )
		{
			if (InstanceBiped)
			{
				TEMP_MOTION* pTempMotion = g_NPCBIPEDMODEL[modelindex].GetTempMotion( g_NPCBIPEDMODEL[modelindex].GetTempMotionCnt()-1 );
				if(pTempMotion)
				{
					int	 soundframe[9];
					TCHAR soundfilename[9][64];
					memset( soundfilename, 0, sizeof(TCHAR) * 64 * 9 );
					sscanf( parsingString, "%s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s", argIDString, 
						&soundframe[0], soundfilename[0], &soundframe[1], soundfilename[1],
						&soundframe[2], soundfilename[2], &soundframe[3], soundfilename[3],
						&soundframe[4], soundfilename[4], &soundframe[5], soundfilename[5],
						&soundframe[6], soundfilename[6], &soundframe[7], soundfilename[7],
						&soundframe[8], soundfilename[8]);
					
					int soundresource[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
					
					for( int i = 0; i < 9; i++ )
					{
						if( strlen(soundfilename[i]) )
						{
#ifdef _XUSEFMOD
							soundresource[i] = g_FMODWrapper.InsertSound( soundfilename[i], FSOUND_HW3D );
							if( soundresource[i] == -1 )
							{
								soundframe[i] = _XDEF_NOEFFECTVALUE;
								soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
								//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
							}
#else						
							soundresource[i] = g_SoundEffectArchive.FindResource( soundfilename[i] );
							if( soundresource[i] == -1 )
							{
								soundframe[i] = _XDEF_NOEFFECTVALUE;
								soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
								//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
							}
#endif							
						}
						else 
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
						}

						pTempMotion->SetSoundEffectFrame( i, soundframe[i], soundresource[i] );						
					}								
					pTempMotion->bSoundEffectFrame = TRUE;
				}
			}
		}
		
	}

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE						
	g_ScriptArchive.ClosePackage();					
#else
	fclose( pScripterFile );
#endif

	return retvalue;
}

BOOL _XLoadMOBModelScripter( LPCTSTR szFilename )
{
	if( !szFilename || !szFilename[0] ) return FALSE;

	FILE* pScripterFile = NULL;

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	pScripterFile = g_ScriptArchive.GetPackedFile( (LPTSTR)szFilename );
#else
	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	pScripterFile = fopen( szFilename, "rt" );
#endif

	if( !pScripterFile )
	{
		_XFatalError( "_XLoadMOBModelScripter( %s ) File open", szFilename );
		return FALSE; 
	}
	
	TCHAR argIDString[512];
	TCHAR argValueString[768];
	TCHAR parsingString[768];
	TCHAR modelname[2][64];
		
	int modelindex = 0;
	int swordtraceeffectindex = 0;
	BOOL retvalue =  TRUE;

	BOOL InstanceModel =  TRUE;
	BOOL InstanceBiped =  TRUE;
	
	int  anikeyreservesize = 0;
	int  anikeycounter = 0;
		
	while( 1 )
	{				
		if( feof(pScripterFile) ) break;
	
		memset( parsingString, 0, 768 );
		if( !fgets( parsingString, 767, pScripterFile ) ) break;
		
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
		
		memset( argIDString, 0, 512 );
		memset( argValueString, 0, 768 );
		memset( modelname, 0, sizeof(TCHAR)*128 );
		sscanf( parsingString, "%s %s %s %s", argIDString, argValueString, modelname[0],modelname[1] );
				
		if( strncmp( argIDString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}		
		// Model name
		else if( strcmpi( argIDString, "_XM_MODELNAME" ) == 0 )
		{
#ifdef _XDEF_NEWSTRINGMANAGER
			g_MonsterName[modelindex] = g_StringDataBase.GetTextString( _XSTRINGDB_SN_MOBMODEL_XMS, modelindex );
			
#ifdef _XJAPANESE
			g_MonsterHiraganaName[modelindex] = g_StringDataBase.GetSubTextString( _XSTRINGDB_SN_MOBMODEL_XMS, modelindex );
#endif

#ifdef _XDWDEBUG
			OutputDebugString( g_MonsterName[modelindex] );
			OutputDebugString( "\n" );
#endif

#else
			memset( g_MonsterName[modelindex], 0, sizeof(TCHAR) * 41 );
			
			LPTSTR pname = NULL;
			pname = strstr( parsingString, argValueString );
			if( pname )
			{				
				strcpy( g_MonsterName[modelindex], pname );
				ReplaceCharacter( g_MonsterName[modelindex], _T('\r'), _T('\0') );
				ReplaceCharacter( g_MonsterName[modelindex], _T('\n'), _T('\0') );
				ReplaceCharacter( g_MonsterName[modelindex], _T('\t'), _T('\0') );
			}			

#endif
		}
		// Model index
		else if( strcmpi( argIDString, "_XM_MODELINDEX" ) == 0 )
		{
			int prevModelIndex = modelindex;
			modelindex	= atoi(argValueString);
			

			if( modelindex == 0 )
			{
				_XFatalError( "_XLoadMOBModelScripter( %s ) : Invalid model index [prev:%d/current:%d]", szFilename, prevModelIndex, modelindex );
				break;
			}

			if( modelindex >= _XDEF_MODELCLASSCOUNT_MOB )
			{
				_XFatalError( "_XLoadMOBModelScripter( %s ) : Invalid model index", szFilename );
				modelindex = _XDEF_MODELCLASSCOUNT_MOB-1;
				break;
			}
		}
		else if( strcmpi( argIDString, "_XM_BIPEDMODEL" ) == 0 )
		{
			g_MOBBIPEDMODEL[modelindex].m_strBipedName = argValueString;
			g_MOBBIPEDMODEL[modelindex].SetCheckSoundPlaying( TRUE );
		}
		else if( strcmpi( argIDString, "_XM_RESERVEANISTACK" )== 0 )
		{
			int stackcount = atoi( argValueString );
			g_MOBBIPEDMODEL[modelindex].m_svTempMotion.reserve(stackcount);
			anikeyreservesize = stackcount;
			anikeycounter = 0;
		}
		else if( strcmpi( argIDString, "_XM_BASEMODEL" ) == 0 )
		{
			InstanceModel = g_Moblist.CreateAnimMeshAbstractClass(&g_MOBMODEL[modelindex], argValueString, modelname[0],modelname[1]);
			g_MOBMODEL[modelindex]->SetTextureArchive( &g_MobTextureArchive );
#ifdef _XDWDEBUG
			if( strlen( modelname[0] ) )
			{
				if( g_ModelArchive.FindResource( modelname[0] ) < 0 )
				{
					_XLog( "ERROR : fnf : %s", modelname[0] );
				}
			}
			if( strlen( modelname[1] ) )
			{
				if( g_ModelArchive.FindResource( modelname[1] ) < 0 )
				{
					_XLog( "ERROR : fnf : %s", modelname[1] );
				}
			}
#endif
		}
		else if( strcmpi( argIDString, "_XM_BASEEFFECT" ) == 0 )
		{
			g_MOBBIPEDMODEL[modelindex].m_BaseEffectIndex = atoi( argValueString );
		}
		else if( strcmpi( argIDString, "_XM_WALKSPEED" ) == 0 )
		{
//			g_MobAttribTable[modelindex].movespeed_walk = atof( argValueString );
		}
		else if( strcmpi( argIDString, "_XM_RUNSPEED" ) == 0 )
		{
//			g_MobAttribTable[modelindex].movespeed_run = atof( argValueString );			
		}
		else if( strcmpi( argIDString, "_XM_ATTACKRANGE" ) == 0 )
		{
			g_MobAttribTable[modelindex].attackrange = atof( argValueString );
		}
		else if( strcmpi( argIDString, "_XM_STANDINGERECT" ) == 0 )
		{
			g_MobAttribTable[modelindex].standingerect = ( atoi( argValueString ) ? TRUE : FALSE );
		}
		else if( strcmpi( argIDString, "_XM_WEAPONMATERIAL" ) == 0 )
		{
			g_MobAttribTable[modelindex].weaponmaterial  = atoi( argValueString );
		}
		else if( strcmpi( argIDString, "_XM_WWEIGHTCLASS" ) == 0 )
		{
			g_MobAttribTable[modelindex].weaponweightclass  = atoi( argValueString );			
		}
		else if( strcmpi( argIDString, "_XM_SWORDTRACECOUNT" ) == 0 )
		{
			if (InstanceBiped)
			{
				g_MOBBIPEDMODEL[modelindex].CreateSwordTraceInfoData( atoi( argValueString ) );
			}
			swordtraceeffectindex = 0;
		}
		else if( strcmpi( argIDString, "_XM_SWORDTRACE" ) == 0 )
		{
			if (InstanceBiped)
			{
				D3DXVECTOR3 toppos, bottompos;
				sscanf( parsingString, "%s %f %f %f %f %f %f", argIDString, &toppos.x,&toppos.y,&toppos.z,&bottompos.x,&bottompos.y,&bottompos.z );

				g_MOBBIPEDMODEL[modelindex].m_pSwordTraceInfoData->SetPointList( swordtraceeffectindex, toppos, bottompos );			
			}
		}
		else  if( strcmpi( argIDString, "_XM_TRACELINK" ) == 0 )
		{
			if (InstanceBiped)
			{
				LPTSTR pParseBipedName = strstr( parsingString, _T( "Bip" ) );
				if(pParseBipedName)
				{
					ReplaceCharacter( pParseBipedName, _T('\r'), _T('\0') );
					ReplaceCharacter( pParseBipedName, _T('\n'), _T('\0') );
					ReplaceCharacter( pParseBipedName, _T('\t'), _T('\0') );

					g_MOBBIPEDMODEL[modelindex].m_strParseBipedName = pParseBipedName;
					g_MOBBIPEDMODEL[modelindex].m_swordtraceeffectindex = swordtraceeffectindex;
				}
			}

			swordtraceeffectindex++;
		}
		else if( strcmpi( argIDString, "_XM_ANIMATION" ) == 0 )
		{
			TCHAR motiontypename[64];
			TCHAR animationfilename[64];
			memset( motiontypename, 0, sizeof(TCHAR) * 64 );
			memset( animationfilename, 0, sizeof(TCHAR) * 64 );
			sscanf( parsingString, "%s %s %s", argIDString, motiontypename, animationfilename );

			if( strlen( motiontypename ) <= 0 )
			{
				_XFatalError( "Monster descripter : Invalid motion name : [ MI : %d ]", modelindex );
			}
			
			if( strlen( animationfilename ) <= 0 )
			{
				_XFatalError( "Monster descripter : Invalid animation file name : [ MI : %d ]", modelindex );
			}

			if( strcmpi( motiontypename, _T("IDLE") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_idle = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("WALK") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_walk = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("RUN") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_run = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("ATTACK") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_attack1 = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("ATTACK1") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_attack2 = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("ATTACK2") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_attack3 = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("DAMAGE") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_blow = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("DIE1") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_die1 = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("DIE2") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_die2 = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("SKILL1") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_skill1 = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("SKILL2") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_skill2 = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if( strcmpi( motiontypename, _T("ENTER1") ) == 0 )
			{				
				g_MobMotionExistTable[modelindex].motion_entering = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
			else if(strcmpi(motiontypename, _T("RATTACK")) == 0)
			{
				g_MobMotionExistTable[modelindex].motion_rattack = g_MOBBIPEDMODEL[modelindex].m_svTempMotion.size();
			}
		
			TEMP_MOTION* pTempBipedNode = new TEMP_MOTION;
			pTempBipedNode->strKeyname = animationfilename;
			g_MOBBIPEDMODEL[modelindex].m_svTempMotion.push_back(pTempBipedNode);

			anikeycounter++;
			if( anikeycounter > anikeyreservesize )
			{
				_XLog( "ERROR : Overflowed animation key stack [%d][%s,%s] [%d/%d]", modelindex, g_MonsterName[modelindex], animationfilename, anikeycounter, anikeyreservesize );
			}
		}
		else if( strcmpi( argIDString, "_XM_ATTACKFRAME" ) == 0 )
		{
			if (InstanceBiped)
			{
				TEMP_MOTION* pTempMotion = g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 );
				if(pTempMotion)
				{
					int	 commandframe[9] = { 255, 255, 255, 255, 255, 255, 255, 255, 255 };
					sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
											&commandframe[0], &commandframe[1], 
											&commandframe[2], &commandframe[3],
											&commandframe[4], &commandframe[5],
											&commandframe[6], &commandframe[7],
											&commandframe[8]);
					
					
					pTempMotion->bAttackFrame = TRUE;

					for( int ipt = 0; ipt < 9; ipt++ )
					{
						if( commandframe[ipt] == 0 ) commandframe[ipt] = _XDEF_NOEFFECTVALUE; 
						pTempMotion->SetAttackFrame( ipt, commandframe[ipt] );
					}
					/*
					if( commandframe[0] == 0 ) commandframe[0] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[1] == 0 ) commandframe[1] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[2] == 0 ) commandframe[2] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[3] == 0 ) commandframe[3] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[4] == 0 ) commandframe[4] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[5] == 0 ) commandframe[5] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[6] == 0 ) commandframe[6] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[7] == 0 ) commandframe[7] = _XDEF_NOEFFECTVALUE; 
					if( commandframe[8] == 0 ) commandframe[8] = _XDEF_NOEFFECTVALUE; 
					
					pTempMotion->bAttackFrame = TRUE;
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 0, commandframe[0] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 1, commandframe[1] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 2, commandframe[2] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 3, commandframe[3] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 4, commandframe[4] );				
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 5, commandframe[5] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 6, commandframe[6] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 7, commandframe[7] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackFrame( 8, commandframe[8] );
					*/
				}
			}
		}
		else if( strcmpi( argIDString, "_XM_ATTACKCLASS" ) == 0 )
		{
			if (InstanceBiped)
			{
				TEMP_MOTION* pTempMotion = g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 );
				if( pTempMotion )
				{
					int	 attackclass[9] = { _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, 
											_XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW,
											_XDEF_ATTACKCLASS_LOW };
					sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
											&attackclass[0], &attackclass[1],  &attackclass[2], &attackclass[3], &attackclass[4],
											&attackclass[5], &attackclass[6],  &attackclass[7], &attackclass[8] );
					
					pTempMotion->bAttackClass = TRUE;
					
					for( int ipt = 0; ipt < 9; ipt++ )
					{
						if( attackclass[ipt] == 0 ) attackclass[ipt] = _XDEF_ATTACKCLASS_LOW; 
						pTempMotion->SetAttackClass( ipt, attackclass[ipt] );
					}

					/*
					if( attackclass[0] == 0 ) attackclass[0] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[1] == 0 ) attackclass[1] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[2] == 0 ) attackclass[2] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[3] == 0 ) attackclass[3] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[4] == 0 ) attackclass[4] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[5] == 0 ) attackclass[5] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[6] == 0 ) attackclass[6] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[7] == 0 ) attackclass[7] = _XDEF_ATTACKCLASS_LOW; 
					if( attackclass[8] == 0 ) attackclass[8] = _XDEF_ATTACKCLASS_LOW;
					
					pTempMotion->bAttackClass = TRUE;
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 0, attackclass[0] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 1, attackclass[1] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 2, attackclass[2] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 3, attackclass[3] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 4, attackclass[4] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 5, attackclass[5] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 6, attackclass[6] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 7, attackclass[7] );
					g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetAttackClass( 8, attackclass[8] );
					*/
				}
			}
		}
		else if( strcmpi( argIDString, "_XM_SOUNDEFFECT" ) == 0 )
		{
			if (InstanceBiped)
			{
				TEMP_MOTION* pTempMotion = g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 );
				if(pTempMotion)
				{
					int	 soundframe[9];
					TCHAR soundfilename[9][64];
					memset( soundfilename, 0, sizeof(TCHAR) * 64 * 9 );
					sscanf( parsingString, "%s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s", argIDString, 
							&soundframe[0], soundfilename[0], &soundframe[1], soundfilename[1],
							&soundframe[2], soundfilename[2], &soundframe[3], soundfilename[3],
							&soundframe[4], soundfilename[4], &soundframe[5], soundfilename[5],
							&soundframe[6], soundfilename[6], &soundframe[7], soundfilename[7],
							&soundframe[8], soundfilename[8] );
					
					int soundresource[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
					
					pTempMotion->bSoundEffectFrame = TRUE;
					for( int i = 0; i < 9; i++ )
					{
						if( strlen(soundfilename[i]) )
						{
#ifdef _XUSEFMOD
							soundresource[i] = g_FMODWrapper.InsertSound( soundfilename[i], FSOUND_HW3D );
							if( soundresource[i] == -1 )
							{
								soundframe[i] = _XDEF_NOEFFECTVALUE;
								soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
								//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
							}
#else						
							soundresource[i] = g_SoundEffectArchive.FindResource( soundfilename[i] );
							if( soundresource[i] == -1 )
							{
								soundframe[i] = _XDEF_NOEFFECTVALUE;
								soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
								//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
							}
#endif
						}
						else 
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
						}
						
						pTempMotion->SetSoundEffectFrame( i, soundframe[i], soundresource[i] );
					}								
				}
			}
		}
		else if( strcmpi( argIDString, "_XM_SPECIALEFFECT" ) == 0 )
		{
			if (InstanceBiped)
			{
				TEMP_MOTION* pTempMotion = g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 );
				if(pTempMotion)
				{
					int	 effectframe[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
					int  effectid[9] = { _XDEF_NOEFFECTWORDVALUE, _XDEF_NOEFFECTWORDVALUE, _XDEF_NOEFFECTWORDVALUE, 
										 _XDEF_NOEFFECTWORDVALUE, _XDEF_NOEFFECTWORDVALUE, _XDEF_NOEFFECTWORDVALUE, 
										 _XDEF_NOEFFECTWORDVALUE, _XDEF_NOEFFECTWORDVALUE, _XDEF_NOEFFECTWORDVALUE };
					sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", argIDString, 
						&effectframe[0], &effectid[0], &effectframe[1], &effectid[1],
						&effectframe[2], &effectid[2], &effectframe[3], &effectid[3],
						&effectframe[4], &effectid[4], &effectframe[5], &effectid[5],
						&effectframe[6], &effectid[6], &effectframe[7], &effectid[7],
						&effectframe[8], &effectid[8] );
					
					pTempMotion->bEffectFrame = TRUE;

					for( int ipt = 0; ipt < 9; ipt++ )
					{
						if( effectid[ipt] == _XDEF_NOEFFECTWORDVALUE )
						{ 
							pTempMotion->SetEffectFrame( ipt, _XDEF_NOEFFECTVALUE, _XDEF_NOEFFECTWORDVALUE );
						}
						else
						{						
							pTempMotion->SetEffectFrame( ipt, effectframe[ipt], effectid[ipt] );
						}
					}

					/*
					if( effectid[0] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[0] = _XDEF_NOEFFECTVALUE; effectid[0] = _XDEF_NOEFFECTWORDVALUE; }
					if( effectid[1] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[1] = _XDEF_NOEFFECTVALUE; effectid[1] = _XDEF_NOEFFECTWORDVALUE; } 
					if( effectid[2] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[2] = _XDEF_NOEFFECTVALUE; effectid[2] = _XDEF_NOEFFECTWORDVALUE; } 
					if( effectid[3] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[3] = _XDEF_NOEFFECTVALUE; effectid[3] = _XDEF_NOEFFECTWORDVALUE; } 
					if( effectid[4] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[4] = _XDEF_NOEFFECTVALUE; effectid[4] = _XDEF_NOEFFECTWORDVALUE; } 
					if( effectid[5] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[5] = _XDEF_NOEFFECTVALUE; effectid[5] = _XDEF_NOEFFECTWORDVALUE; } 
					if( effectid[6] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[6] = _XDEF_NOEFFECTVALUE; effectid[6] = _XDEF_NOEFFECTWORDVALUE; } 
					if( effectid[7] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[7] = _XDEF_NOEFFECTVALUE; effectid[7] = _XDEF_NOEFFECTWORDVALUE; } 
					if( effectid[8] == _XDEF_NOEFFECTWORDVALUE ){ effectframe[8] = _XDEF_NOEFFECTVALUE; effectid[8] = _XDEF_NOEFFECTWORDVALUE; } 
					
					pTempMotion->bEffectFrame = TRUE;
					for( int i = 0; i < 9; i++ )
					{
						g_MOBBIPEDMODEL[modelindex].GetTempMotion( g_MOBBIPEDMODEL[modelindex].GetTempMotionCnt()-1 )->SetEffectFrame( i, effectframe[i], effectid[i] );
					}
					*/
				}
			}
		}	
	}

#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	g_ScriptArchive.ClosePackage();
#else
	fclose( pScripterFile );
#endif

	return retvalue;
}

BOOL _XReLoadMOBModelScripter( LPCTSTR szFilename )
{
#ifdef _XDEF_SCRIPT_LOADTARGET_FROMPACKAGE	
	return FALSE;		
#endif

	if( !szFilename || !szFilename[0] ) return FALSE;

	FILE* pScripterFile = NULL;

	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	pScripterFile = fopen( szFilename, "rt" );

	if( !pScripterFile )
	{
		_XFatalError( "_XLoadMOBModelScripter( %s ) File open", szFilename );
		return FALSE; 
	}
	
	TCHAR argIDString[512];
	TCHAR argValueString[768];
	TCHAR parsingString[768];

	int modelindex = 0;
	int motionindex = -1;

	BOOL retvalue =  TRUE;
	
	while( 1 )
	{				
		if( feof(pScripterFile) ) break;
	
		memset( parsingString, 0, 768 );
		if( !fgets( parsingString, 767, pScripterFile ) ) break;
		
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
		
		memset( argIDString, 0, 512 );
		memset( argValueString, 0, 768 );
		sscanf( parsingString, "%s %s", argIDString, argValueString );
				
		if( strncmp( argIDString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}		
		// Model name
		else if( strcmpi( argIDString, "_XM_MODELNAME" ) == 0 )
		{
			memset( g_MonsterName[modelindex], 0, sizeof(TCHAR) * 41 );
			
			LPTSTR pname = NULL;
			pname = strstr( parsingString, argValueString );
			if( pname )
				strcpy( g_MonsterName[modelindex], pname );
		}
		// Model index
		else if( strcmpi( argIDString, "_XM_MODELINDEX" ) == 0 )
		{
			modelindex	= atoi(argValueString);
			if( modelindex >= _XDEF_MODELCLASSCOUNT_MOB )
			{
				_XFatalError( "_XLoadMOBModelScripter( %s ) : Invalid model index", szFilename );
				modelindex = _XDEF_MODELCLASSCOUNT_MOB-1;
			}

			motionindex = -1;
		}		
		else if( strcmpi( argIDString, "_XM_BIPEDMODEL" ) == 0 )
		{			
		}
		else if( strcmpi( argIDString, "_XM_RESERVEANISTACK" )== 0 )
		{			
		}
		else if( strcmpi( argIDString, "_XM_BASEMODEL" ) == 0 )
		{			
		}
		else if( strcmpi( argIDString, "_XM_WALKSPEED" ) == 0 )
		{
			g_MobAttribTable[modelindex].movespeed_walk = atof( argValueString );
		}
		else if( strcmpi( argIDString, "_XM_RUNSPEED" ) == 0 )
		{
			g_MobAttribTable[modelindex].movespeed_run = atof( argValueString );			
		}
		else if( strcmpi( argIDString, "_XM_ATTACKRANGE" ) == 0 )
		{
			g_MobAttribTable[modelindex].attackrange = atof( argValueString );
		}
		else if( strcmpi( argIDString, "_XM_STANDINGERECT" ) == 0 )
		{
			g_MobAttribTable[modelindex].standingerect = ( atoi( argValueString ) ? TRUE : FALSE );
		}
		else if( strcmpi( argIDString, "_XM_WEAPONMATERIAL" ) == 0 )
		{
			g_MobAttribTable[modelindex].weaponmaterial  = atoi( argValueString );
		}
		else if( strcmpi( argIDString, "_XM_WWEIGHTCLASS" ) == 0 )
		{
			g_MobAttribTable[modelindex].weaponweightclass  = atoi( argValueString );			
		}
		else if( strcmpi( argIDString, "_XM_ANIMATION" ) == 0 )
		{			
			motionindex++;
		}
		else if( strcmpi( argIDString, "_XM_ATTACKFRAME" ) == 0 )
		{
			if( motionindex == -1 ) motionindex = 0;

			if( g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex ) )
			{
				int	 commandframe[9] = { 255, 255, 255, 255, 255, 255, 255, 255, 255 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
										&commandframe[0], &commandframe[1], 
										&commandframe[2], &commandframe[3],
										&commandframe[4], &commandframe[5],
										&commandframe[6], &commandframe[7],
										&commandframe[8]);
				
				if( commandframe[0] == 0 ) commandframe[0] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[1] == 0 ) commandframe[1] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[2] == 0 ) commandframe[2] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[3] == 0 ) commandframe[3] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[4] == 0 ) commandframe[4] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[5] == 0 ) commandframe[5] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[6] == 0 ) commandframe[6] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[7] == 0 ) commandframe[7] = _XDEF_NOEFFECTVALUE; 
				if( commandframe[8] == 0 ) commandframe[8] = _XDEF_NOEFFECTVALUE; 
				
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 0, commandframe[0] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 1, commandframe[1] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 2, commandframe[2] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 3, commandframe[3] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 4, commandframe[4] );				
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 5, commandframe[5] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 6, commandframe[6] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 7, commandframe[7] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackFrame( 8, commandframe[8] );
			}
		}
		else if( strcmpi( argIDString, "_XM_ATTACKCLASS" ) == 0 )
		{
			if( motionindex == -1 ) motionindex = 0;

			if( g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex ) )
			{
				int	 attackclass[9] = { _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, 
										_XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW, _XDEF_ATTACKCLASS_LOW,
										_XDEF_ATTACKCLASS_LOW };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d", argIDString, 
										&attackclass[0], &attackclass[1],  &attackclass[2], &attackclass[3], &attackclass[4],
										&attackclass[5], &attackclass[6],  &attackclass[7], &attackclass[8] );
				
				if( attackclass[0] == 0 ) attackclass[0] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[1] == 0 ) attackclass[1] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[2] == 0 ) attackclass[2] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[3] == 0 ) attackclass[3] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[4] == 0 ) attackclass[4] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[5] == 0 ) attackclass[5] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[6] == 0 ) attackclass[6] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[7] == 0 ) attackclass[7] = _XDEF_ATTACKCLASS_LOW; 
				if( attackclass[8] == 0 ) attackclass[8] = _XDEF_ATTACKCLASS_LOW;
				
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 0, attackclass[0] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 1, attackclass[1] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 2, attackclass[2] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 3, attackclass[3] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 4, attackclass[4] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 5, attackclass[5] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 6, attackclass[6] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 7, attackclass[7] );
				g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetAttackClass( 8, attackclass[8] );
			}
		}
		else if( strcmpi( argIDString, "_XM_SOUNDEFFECT" ) == 0 )
		{
			if( motionindex == -1 ) motionindex = 0;

			if(g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex ))
			{
				int	 soundframe[9];
				TCHAR soundfilename[9][64];
				memset( soundfilename, 0, sizeof(TCHAR) * 64 * 9 );
				sscanf( parsingString, "%s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s", argIDString, 
						&soundframe[0], soundfilename[0], &soundframe[1], soundfilename[1],
						&soundframe[2], soundfilename[2], &soundframe[3], soundfilename[3],
						&soundframe[4], soundfilename[4], &soundframe[5], soundfilename[5],
						&soundframe[6], soundfilename[6], &soundframe[7], soundfilename[7],
						&soundframe[8], soundfilename[8] );
				
				int soundresource[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
				
				for( int i = 0; i < 9; i++ )
				{
					if( strlen(soundfilename[i]) )
					{
#ifdef _XUSEFMOD
						soundresource[i] = g_FMODWrapper.InsertSound( soundfilename[i], FSOUND_HW3D );
						if( soundresource[i] == -1 )
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
							//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
						}
#else						
						soundresource[i] = g_SoundEffectArchive.FindResource( soundfilename[i] );
						if( soundresource[i] == -1 )
						{
							soundframe[i] = _XDEF_NOEFFECTVALUE;
							soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
							//_XFatalError( "Can't find sound resource [%s]", soundfilename[i] );
						}
#endif
					}
					else 
					{
						soundframe[i] = _XDEF_NOEFFECTVALUE;
						soundresource[i] = _XDEF_NOEFFECTWORDVALUE;
					}
					
					g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetSoundEffectFrame( 
														   i, soundframe[i], soundresource[i] );						
				}								
			}			
		}
		else if( strcmpi( argIDString, "_XM_SPECIALEFFECT" ) == 0 )
		{
			if( motionindex == -1 ) motionindex = 0;

			if(g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex ))
			{
				int	 effectframe[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				int  effectid[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				sscanf( parsingString, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", argIDString, 
					&effectframe[0], &effectid[0], &effectframe[1], &effectid[1],
					&effectframe[2], &effectid[2], &effectframe[3], &effectid[3],
					&effectframe[4], &effectid[4], &effectframe[5], &effectid[5],
					&effectframe[6], &effectid[6], &effectframe[7], &effectid[7],
					&effectframe[8], &effectid[8] );
				
				if( effectframe[0] == 0 ){ effectframe[0] = _XDEF_NOEFFECTVALUE; effectid[0] = _XDEF_NOEFFECTWORDVALUE; }
				if( effectframe[1] == 0 ){ effectframe[1] = _XDEF_NOEFFECTVALUE; effectid[1] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[2] == 0 ){ effectframe[2] = _XDEF_NOEFFECTVALUE; effectid[2] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[3] == 0 ){ effectframe[3] = _XDEF_NOEFFECTVALUE; effectid[3] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[4] == 0 ){ effectframe[4] = _XDEF_NOEFFECTVALUE; effectid[4] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[5] == 0 ){ effectframe[5] = _XDEF_NOEFFECTVALUE; effectid[5] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[6] == 0 ){ effectframe[6] = _XDEF_NOEFFECTVALUE; effectid[6] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[7] == 0 ){ effectframe[7] = _XDEF_NOEFFECTVALUE; effectid[7] = _XDEF_NOEFFECTWORDVALUE; } 
				if( effectframe[8] == 0 ){ effectframe[8] = _XDEF_NOEFFECTVALUE; effectid[8] = _XDEF_NOEFFECTWORDVALUE; } 
				
				for( int i = 0; i < 9; i++ )
				{
					g_MOBBIPEDMODEL[modelindex].GetMotion( motionindex )->SetEffectFrame( i, effectframe[i], effectid[i] );
				}								
			}
		}	
	}

	fclose( pScripterFile );

	return retvalue;
}

void _XBipedController::FrameAnimation( int currentmotion, FLOAT& curframe )
{
	int framespeed	 = m_pMotionStack[currentmotion].FrameSpeed;
		
	if( curframe < 0 ) curframe = 0;
	if( curframe > m_pMotionStack[currentmotion].EndFrame ) 
		curframe -= m_pMotionStack[currentmotion].EndFrame;	
	
	int					frame;
	_LPXM_ROTATIONKEY	RotationSFrame=NULL, RotationEFrame=NULL;
	_LPXM_POSITIONKEY	PositionSFrame=NULL, PositionEFrame=NULL;
	_LPXM_BIPEDOBJECT	lpCh=NULL,   lpParent=NULL;
	

	for(int objindex = 0; objindex < GetNodeCnt(); objindex++)
	{			
		lpCh			= GetNode(objindex );
		lpParent		= GetNode(lpCh->nParentId);

		D3DXMATRIX		d3dMatrix, mat;        
		D3DXQUATERNION	q;
		D3DXMatrixIdentity( &d3dMatrix );

		// Check animation list
		if(lpCh->m_pnRotKey[currentmotion] == 0 && lpCh->m_pnPosKey[currentmotion] == 0)
		{
			d3dMatrix = lpCh->m_localTM;
		}
		else
		{
			// Check rotation key
			if(lpCh->m_pnRotKey[currentmotion] == 0)
			{
				d3dMatrix		= lpCh->m_localTM;
				d3dMatrix._41	= 0.0f;
				d3dMatrix._42	= 0.0f;
				d3dMatrix._43	= 0.0f;
				d3dMatrix._44	= 1.0f;
			}
			// Process rotate animation key
			else 
			{
				RotationSFrame = GetRotationAniKey(lpCh->m_pRotKey[currentmotion], lpCh->m_pnRotKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].StartFrame);
				RotationEFrame = GetRotationAniKey(lpCh->m_pRotKey[currentmotion], lpCh->m_pnRotKey[currentmotion], (FLOAT)m_pMotionStack[currentmotion].EndFrame);

				// Check start frame
				if(curframe == (FLOAT)m_pMotionStack[currentmotion].StartFrame)	
				{	
					q = RotationSFrame->Qt;
				}
				// Check last frame
				else if(RotationEFrame->nTime <= curframe) q = RotationEFrame->Qt;
				else
				{
					for(frame = 0; frame < lpCh->m_pnRotKey[currentmotion]; frame++)
					{
						if(lpCh->m_pRotKey[currentmotion][frame].nTime == curframe)
						{
							q = lpCh->m_pRotKey[currentmotion][frame].Qt;
							break;
						}
						if(lpCh->m_pRotKey[currentmotion][frame  ].nTime < curframe &&
						   lpCh->m_pRotKey[currentmotion][frame+1].nTime > curframe )
						{
							// frame 과 frame+1으로 보간한 쿼터니언값..
							// 진행률은 ( 현재Frame - 이전 Key Frame )/( 다음 Key Frame - 이전 Key Frame );
							float sx, ex;
							sx = lpCh->m_pRotKey[currentmotion][frame  ].nTime;
							ex = lpCh->m_pRotKey[currentmotion][frame+1].nTime;

							float xx = (curframe-sx)/(ex-sx);
							D3DXQuaternionSlerp(&q, &lpCh->m_pRotKey[currentmotion][frame  ].Qt, 
													&lpCh->m_pRotKey[currentmotion][frame+1].Qt, xx);
							break;
						}
					}
				}				
				_XMeshMath_MatrixFromQuaternion(d3dMatrix, q);
			}

			// Check position key
			if(lpCh->m_pnPosKey[currentmotion] == 0) // 프레임이 1개. 즉 0번키..
			{
				d3dMatrix._41 = lpCh->m_localTM._41;
				d3dMatrix._42 = lpCh->m_localTM._42;
				d3dMatrix._43 = lpCh->m_localTM._43;
			}
			// Process position key
			else 
			{
				PositionSFrame = GetPositionAniKey(lpCh->m_pPosKey[currentmotion], lpCh->m_pnPosKey[currentmotion], 
										   (FLOAT)m_pMotionStack[currentmotion].StartFrame);
				PositionEFrame = GetPositionAniKey(lpCh->m_pPosKey[currentmotion], lpCh->m_pnPosKey[currentmotion], 
										   (FLOAT)m_pMotionStack[currentmotion].EndFrame);

				if(curframe == (FLOAT)m_pMotionStack[currentmotion].StartFrame)
				{
					d3dMatrix._41 = PositionSFrame->d3dVKeyVal.x;
					d3dMatrix._42 = PositionSFrame->d3dVKeyVal.y;
					d3dMatrix._43 = PositionSFrame->d3dVKeyVal.z;
				}
				else if(PositionEFrame->nTime <= curframe)
				{
					d3dMatrix._41 = PositionEFrame->d3dVKeyVal.x;
					d3dMatrix._42 = PositionEFrame->d3dVKeyVal.y;
					d3dMatrix._43 = PositionEFrame->d3dVKeyVal.z;
				}
				else
				{
					for(frame = 0; frame < lpCh->m_pnPosKey[currentmotion]; frame++)
					{
						if(lpCh->m_pPosKey[currentmotion][frame].nTime == curframe)
						{
							d3dMatrix._41 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x;
							d3dMatrix._42 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y;
							d3dMatrix._43 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z;
							break;
						}
						if(lpCh->m_pPosKey[currentmotion][frame  ].nTime < curframe && 
						   lpCh->m_pPosKey[currentmotion][frame+1].nTime > curframe )
						{
							// frame 과 frame+1으로 보간한 쿼터니언값..
							// 진행률은 ( 현재Frame - 이전 Key Frame )/( 다음 Key Frame - 이전 Key Frame );
							float sx, ex;
							sx = lpCh->m_pPosKey[currentmotion][frame  ].nTime;
							ex = lpCh->m_pPosKey[currentmotion][frame+1].nTime;
							
							float xx = (curframe-sx)/(ex-sx);

							// 벡터 보간..
							d3dMatrix._41 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x + 
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.x - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.x ) * xx;

							d3dMatrix._42 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y +
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.y - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.y ) * xx;

							d3dMatrix._43 = lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z +
											( lpCh->m_pPosKey[currentmotion][frame+1].d3dVKeyVal.z - 
											lpCh->m_pPosKey[currentmotion][frame].d3dVKeyVal.z ) * xx;
							break;
						}
					}
				}
			}
		}
		_XMeshMath_UnitMatrixFromMatrix(d3dMatrix);

		if(lpCh->nParentId == -1)
		{
			//자식을위해 결과매트릭스를 저장
			lpCh->m_matAniTM = d3dMatrix;
		}
		else
		{
			// d3dMatrix은 오브젝트의 Local Ani TM 이된다..
			D3DXMatrixMultiply(&lpCh->m_matAniTM, &d3dMatrix, &lpParent->m_matAniTM);
		}

		D3DXMatrixMultiply( &lpCh->m_matLocalAni, &lpCh->m_matInvTM , &lpCh->m_matAniTM);

	}
		
	//pAniData->m_AniLastFrame	= pAniData->m_AniFrame;
}