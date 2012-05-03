// XBipedController.h: interface for the _XBipedController class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XBIPEDCONTROLLER_H_
#define _XBIPEDCONTROLLER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XModelContainerDef.h"
#include "XEffect_SwordTrace.h"
#include "XParticleDefine.h"
#include "XReference.h"
#include "XDecalObject.h"

#include <vector>
#include <string>

class  _XMeshContainer;
class  _XBipedController;
class  _XAnimMeshAbstractClass;
class  _XUser;
class  _XMob;
class  _XParticleGroupInstanceManager;
struct _XUSER_MODELDESCRIPTTYPE;
class _XFunctionalObject;
class _XCHARACTER_INFO;

#define _XDEF_MAXMODELSTACKSIZE		20

class _XModelDescriptor
{
private:

public:
	BOOL					m_AniFlag;
	BOOL					m_LockEndFrame;
	FLOAT					m_AniFrame;
	FLOAT					m_AniLastFrame;
	FLOAT					m_AniTime;
		
	FLOAT					m_SlerpAniFrame;
	FLOAT					m_SlerpAnimationTime;

	int						m_CurMotion;
	D3DXMATRIX				m_Position;

	int						m_PrevMotion;
	FLOAT					m_PrevMotionFrame;
	BOOL					m_MotionChangeFlag;
	BOOL					m_MotionChangingFlag;
	int						m_NextMotion;

	BOOL					m_NoAnimationAndPresentPipeline;

	FLOAT					m_CameraDistance;
	DWORD					m_FrameResult;	
	BYTE					m_AttackClass;				// 디스크립터쪽에서 넘어오는 타격이펙트 인덱스 관련 플래그
	BYTE					m_EffectPosition;			// 디스크립터쪽에서 넘어오는 이펙트 타격위치 관련 플래그 <상중하>
	BYTE					m_WeaponClass;				// 메인무기 공격이면 0 발차기면 1	

	int						m_ModelStackSize;

#ifdef _XDEF_MODELTHREADLOADING
	_XAnimMeshAbstractClass*  m_ppModelList[_XDEF_MAXMODELSTACKSIZE];
#else
	_XAnimMeshAbstractClass** m_ppModelList;
#endif

	_XANIEVENT_STRUCTURE	m_EventHandler;
	_XBipedController*		m_pBipedController;

	BOOL					m_EnableEffectProcess;
	D3DXMATRIX				m_PartMatrixContainer[_XParticleLinkBone_MaxCount];	//0 = 오른손, 1 = 왼손, 3 = 오른 다리, 4 = 왼다리, 단, 장병기, Pelvis, Head
	
	_XParticleGroupInstanceManager*	m_EffectManager;

	BOOL					m_ViewLeftSwordEffect;
	BOOL					m_ViewRightSwordEffect;
	
	BOOL							  m_SwordEffectInitialized;
	int								  m_SwordEffectCount;
	
#ifdef _XDEF_MODELTHREADLOADING
	#define _XDEF_MAXSWORDINSTANCECOUNT		4
	_XEffect_SwordTrace_InstanceData  m_pSwordEffectInstanceObject[_XDEF_MAXSWORDINSTANCECOUNT];
#else	
	_XEffect_SwordTrace_InstanceData* m_pSwordEffectInstanceObject;	
#endif
	

	//--------------------------------------------------------------------------------------
	// 캐릭터 그림자
	D3DXMATRIX				m_matLocalToWorld;			// [0] 그림자 객체 좌표 -> 월드
	D3DXMATRIX				m_matWorldToLight;			// [1] 월드             -> 광원 좌표
	D3DXMATRIX				m_matShadowProj;			// [3] 광원 좌표        -> 텍스쳐 좌표
	D3DXMATRIX				m_ShadowUVMatrix;
		
	_XShadowDecalObject		m_ShadowDecal;
	FLOAT					m_BlockerCenterHeight;
	FLOAT					m_ShadowScaleFactor;
	FLOAT					m_ShadowDecalSize;
	
public:
	_XModelDescriptor();
	~_XModelDescriptor();

	_XModelDescriptor& operator =(_XModelDescriptor& modeldescriptor);

	void			Reset( void );

	BOOL			ReserveModelStack( int modelstacksize );
	
	void			InitEffectManager( int swordeffectcount, int tracetextureindex = 0 );
	void			SetEnableEffectManager( BOOL enable )
	{
					m_EnableEffectProcess = enable;
	}
	
	void			LinkBipedObject( _XBipedController* pBipedController )
					{	m_pBipedController = pBipedController;	}
	
	void			SetMeshModel( int index, _XAnimMeshAbstractClass* pModel )
	{
					if( index >= 0 && index < m_ModelStackSize )
					{
						m_ppModelList[index] = pModel;
					}
	}

	void			SetMeshModel( _XUSER_MODELDESCRIPTTYPE& modeldescript, BOOL gender, int groupindex, int clothview = 0 );
	
	void			SetLockEndFrame( BOOL lock ){ m_LockEndFrame = lock; }

	inline void		PlayAnimation( BOOL flag )	{ m_AniFlag = flag; };
	inline void		PlayAnimation( void )		{ m_AniFlag = !m_AniFlag; };
	inline void		PauseAnimation( void )		{ m_AniFlag = false; };
	inline void		StopAnimation( void )		{ m_AniFlag = false; m_AniFrame = 0; m_CurMotion = 0; };

	inline BOOL		GetAnimation( void )		{ return m_AniFlag;		};
	inline float	GetCurFrame( void )			{ return m_AniFrame;	};
	inline int		GetCurMotion( void )		{ return m_CurMotion;	};

	inline void		SetCurFrame(FLOAT frame)	{ m_AniFrame = frame; };	
	void			SetCurMotion( int motionindex, bool changemotion = false, bool changesamemotion = false);
	void			SetLastFrame( void );

	void			ResetEventHandler( void );	

	void			Advance_NoCheckEvent( FLOAT fElapsedTime );
	void			Advance( bool& playsound, int eventlevel = 0, FLOAT particlesize = 0.0f );

	void			Animation( void );
	void			Animation( FLOAT frame );

	void			Render( void );
	void			RenderMob( _XMob* pMob );
	void			RenderFunctionalObject( _XFunctionalObject* pFunctionalObject );
	void			Render_ZeroLevelMob( _XMob* pMob );
	void			Render_Reflect( D3DXMATRIX& viewmat );
	void			Render_SecondBuffer( void );
	void			Render( D3DXMATRIX& worldmat );		
	void			RenderNoTexture( D3DXMATRIX& worldmat );
	void			RenderCustomDraw( D3DXMATRIX& worldmat );
	void			RenderCharacter( _XUser* pUser );
	void			RenderCharacter( _XCHARACTER_INFO& UserCharacterInfo );
	void			RenderCharacterModel( _XUser* pUser, BOOL uselowlevelLOD );
	void			RenderCharacterModel_RadialBlur( _XUser* pUser, BOOL uselowlevelLOD );
	void			RenderHeroCharacter( _XUser* pUser );

	
	void			SetDrawSwordEffect( BOOL enable );
	void			RenderSwordEffect( int renderstyleindex );

	void			SetShadowDecalInfo( FLOAT blockercenterheight, FLOAT maxradius, FLOAT decalsize );

	void			GenerateShadowMap( void );
	void			RenderObjectShadow( void );	
	void			RenderShadow( void );		
	
	void			MakePlaneDecal( FLOAT height = 0.0f );
	void			RenderPlaneShadow( void );

	BOOL			InterOBBvsRay( D3DXVECTOR3& mouserayorigin, D3DXVECTOR3& mouseraydirection, D3DXVECTOR3& Position, 
								   FLOAT& RotateAngle, D3DXVECTOR3* pPickedPoint = NULL );
};


#define _XMODELPART_RIGHTHAND	0
#define _XMODELPART_LEFTHAND	1
#define _XMODELPART_RIGHTFOOT	2
#define _XMODELPART_LEFTFOOT	3

class _XSwordTraceInfoStructure
{
public:
	int			 m_Pointcount;
	D3DXVECTOR3* m_pTopPosList;
	D3DXVECTOR3* m_pBottomPosList;
	int*		 m_pLinkedBipedIndex;

	_XSwordTraceInfoStructure()
	{
		m_Pointcount		= 0;
		m_pLinkedBipedIndex	= NULL;
		m_pTopPosList		= NULL;
		m_pBottomPosList	= NULL;
	}

	~_XSwordTraceInfoStructure()
	{
		DestroyPointList();
	}

	void DestroyPointList( void )
	{
		m_Pointcount	=	0;
		SAFE_DELETE_ARRAY( m_pLinkedBipedIndex	);
		SAFE_DELETE_ARRAY( m_pTopPosList			);
		SAFE_DELETE_ARRAY( m_pBottomPosList		);
	}

	BOOL CreatePointList( int pointcount )
	{
		m_pTopPosList	= new D3DXVECTOR3[ pointcount ];
		if( !m_pTopPosList ) return FALSE;
		m_pBottomPosList	= new D3DXVECTOR3[ pointcount ];
		if( !m_pBottomPosList ) return FALSE;
		m_pLinkedBipedIndex = new int[pointcount];
		if( !m_pLinkedBipedIndex ) return FALSE;

		m_Pointcount	=	pointcount;
		return TRUE;
	}

	void SetLinkedBiped( int index, int linkedbipedindex )
	{
		if( index >= m_Pointcount ) return;
		m_pLinkedBipedIndex[index] = linkedbipedindex;
	}

	void SetPointList( int index, D3DXVECTOR3& toppos, D3DXVECTOR3& bottompos )
	{
		if( index >= m_Pointcount ) return;		
		m_pTopPosList[index] = toppos;
		m_pBottomPosList[index] = bottompos;
	}
};

struct TEMP_MOTION
{
	std::string	strKeyname;

	BOOL	bAttackClass;
	BOOL	bAttackFrame;
	BOOL	bSoundEffectFrame;
	BOOL	bEffectFrame;

	WORD	SoundIndex[_XDEF_MAXFRAMEEVENT];
	BYTE	SoundOutFrame[_XDEF_MAXFRAMEEVENT];	
	BYTE	AttackCmdFrame[_XDEF_MAXFRAMEEVENT];
	BYTE	AttackClass[_XDEF_MAXFRAMEEVENT];
	WORD	EffectIndex[_XDEF_MAXFRAMEEVENT];
	BYTE	EffectFrame[_XDEF_MAXFRAMEEVENT];	

	TEMP_MOTION()
	{
		bAttackClass		= FALSE;
		bAttackFrame		= FALSE;
		bSoundEffectFrame	= FALSE;
		bEffectFrame		= FALSE;

		for( int i = 0; i < _XDEF_MAXFRAMEEVENT; i++  )
		{
			AttackClass[i]		= _XDEF_ATTACKCLASS_LOW;
			AttackCmdFrame[i]	= _XDEF_NOEFFECTVALUE;
			SoundIndex[i]		= _XDEF_NOEFFECTWORDVALUE;
			SoundOutFrame[i]	= _XDEF_NOEFFECTVALUE;
			EffectIndex[i]		= _XDEF_NOEFFECTWORDVALUE;
			EffectFrame[i]		= _XDEF_NOEFFECTVALUE;
		}		
	}

	void SetAttackClass( DWORD index, BYTE attackclass = _XDEF_NOEFFECTVALUE )
	{ 
		if( index < _XDEF_MAXFRAMEEVENT )	{	AttackClass[index]	  = attackclass; }
	};	
	void SetAttackFrame( DWORD index, BYTE attackcmdframe = _XDEF_NOEFFECTVALUE )
	{ 
		if( index < _XDEF_MAXFRAMEEVENT ) {	AttackCmdFrame[index] = attackcmdframe; }
	};
	
	void SetSoundEffectFrame( DWORD index, BYTE soundoutframe = _XDEF_NOEFFECTVALUE, WORD soundindex = _XDEF_NOEFFECTWORDVALUE )
	{ 
		if( index < _XDEF_MAXFRAMEEVENT )	{ SoundOutFrame[index] = soundoutframe; SoundIndex[index]	 = soundindex; }
	};

	void SetEffectFrame( DWORD index, BYTE effectoutframe = _XDEF_NOEFFECTVALUE, WORD effectindex = _XDEF_NOEFFECTWORDVALUE )	 					 
	{
		if( index < _XDEF_MAXFRAMEEVENT ){ EffectFrame[index]	= effectoutframe; EffectIndex[index]	= effectindex; 	}
	}
};

typedef std::vector<TEMP_MOTION*> svdef_TEMP_MOTION;

class _XBipedController : public _XReference
{

public:
	std::string					m_strBipedName;
	std::string					m_strParseBipedName;
	int							m_swordtraceeffectindex;
	
	int							m_BaseEffectIndex;

	svdef_TEMP_MOTION			m_svTempMotion;

protected:
	_LPXM_BIPEDOBJECT	m_BipedNode;
	int					m_BipedNodeCnt;
	bool				m_CheckSoundPlaying;
public:
	int					m_CharacterPartIndex[_XParticleLinkBone_MaxCount];	
	_XSwordTraceInfoStructure*	m_pSwordTraceInfoData;
	
public:

	_XBipedController();
	virtual ~_XBipedController();

	BOOL				Load( LPSTR szFilename );
	BOOL				Load( FILE* Fileptr, LPSTR szFilename = NULL );
	
	BOOL				CreateNode( int nodecount );
	void				DeleteNode( void );
	void				DestroyModel( void );

	BOOL				CreateSwordTraceInfoData( int infocount );
	
	inline void			SetNodeCnt(int nodecount){ m_BipedNodeCnt = nodecount; };
	void				SetSkinning( void );
		
	_LPXM_BIPEDOBJECT	GetNode(int index );
	_LPXM_BIPEDOBJECT	GetNode(LPSTR objectname );

	inline int			GetNodeCnt( void ){ return m_BipedNodeCnt; };
	int					FindObject( LPSTR objectname );
	void				FindPartIndex( void );
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Animation methods 
protected:
	int					m_FirstFrame;
	int					m_LastFrame;
	int					m_TickPerFrame;
	int					m_FrameSpeed;
	
	int					m_ReservedMotionStackCount;
	int					m_MotionCnt;
	_LPXM_MOTION		m_pMotionStack;	
	
// Animation data handler...
public:

	BOOL				ReserveAnimationStack( int motionstacksize );
		
	void				SetMotion(int index, int SFrame = -1,
								  int EFrame = -1, int FrameSpeed = -1);

	inline void			SetFirstFrame(int Cnt)		{ m_FirstFrame = Cnt; };
	inline void			SetLastFrame(int Cnt)		{ m_LastFrame = Cnt; };
	inline void			SetTickPerFrame(int Cnt)	{ m_TickPerFrame = Cnt; };
	inline void			SetFrameSpeed(int Cnt)		{ m_FrameSpeed = Cnt; };
	
	inline int			GetFirstFrame()				{ return m_FirstFrame; };
	inline int			GetLastFrame()				{ return m_LastFrame; };
	inline int			GetTickPerFrame()			{ return m_TickPerFrame; };
	inline int			GetFrameSpeed()				{ return m_FrameSpeed; };

	int					GetTempMotionCnt( void )	{ return m_svTempMotion.size(); };
	TEMP_MOTION	*		GetTempMotion(int index );

	inline int			GetMotionCnt( void ){ return m_MotionCnt; };
	_LPXM_MOTION		GetMotion( int motionindex );
	_LPXM_POSITIONKEY	GetPositionAniKey(_LPXM_POSITIONKEY AniKey, int KeyNum, float Frame);
	_LPXM_ROTATIONKEY	GetRotationAniKey(_LPXM_ROTATIONKEY AniKey, int KeyNum, float Frame);

	bool				AddMotion(int SFrame, int EFrame, int FrameSpeed);
	bool				AddMotion(int index,  int SFrame, int EFrame, int FrameSpeed);
	bool				AddMotion(_XAHEADER Header);
	bool				AddMotion(int index, _XAHEADER Header);	
	
	void				DeleteMotion( void );
	void				ResetMotionList( void );
	
	bool				LoadKey( LPSTR szFilename );
	bool				LoadKey( FILE* Fileptr, LPSTR szFilename = NULL );

	void				Advance_NoCheckEvent( _XModelDescriptor* pAniData, FLOAT fElapsedTime );
	void				Advance(  _XModelDescriptor* pAniData, bool& playsound, int eventlevel = 0, FLOAT particlesize = 0.0f );
	void				FrameAnimation( _XModelDescriptor* pAniData );	
	void				FrameAnimation( _XModelDescriptor* pAniData, FLOAT curframe );
	void				SlerpAnimation( _XModelDescriptor* pAniData );

	void				SetCheckSoundPlaying( bool check ){  m_CheckSoundPlaying = check; }

	void				FrameAnimation( int currentmotion, FLOAT& curframe );
};

BOOL _XLoadCharacterBipedScripter( _XBipedController* pBipedController, LPSTR szFilename );
BOOL _XLoadModelScripter( LPCTSTR szFilename );
BOOL _XLoadNPCModelScripter( LPCTSTR szFilename );
BOOL _XLoadMOBModelScripter( LPCTSTR szFilename );
BOOL _XReLoadMOBModelScripter( LPCTSTR szFilename );
#endif // !defined(_XBIPEDCONTROLLER_H_)
