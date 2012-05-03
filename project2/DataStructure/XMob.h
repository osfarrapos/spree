// XMob.h: interface for the _XMob class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMOB_H_
#define _XMOB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XUser.h"
#include "XObjectManager.h"
#include "XLodTerrain.h"
#include "XActionTable.h"
#include "EODEXTDEF_ModelData.h"
#include "XProc_MainGameDef.h"

#include "XModelManager.h"
#include "XMemoryPool.h"
#include <map>
#include <list>

#define _XDEF_MOBCOLLIDERADIUS				0.80f
#define _XDEF_MOBCOLLIDEOFFSET_DEFAULT		0.75f

#define _XDEF_MOBCOLLIDEFRUSTUM_DEFAULT		0.8f

#define _XDEF_MOB_KILLTIME					20000    // 마지막 메세지들어온 시간 이후 제한 경과 시간이 지나면 삭제


#define _XDEF_MOB_KILLSIGNAL_STOP			0
#define _XDEF_MOB_KILLSIGNAL_START			1
#define _XDEF_MOB_KILLSIGNAL_END			2

typedef struct 
{	
	DWORD		action      :	8;
	DWORD		idlemode    :	1;
	DWORD		movemode    :	1;
	DWORD		attacktype  :   3;
	DWORD		dietype		:	1;
	DWORD		skilltype	:	1;
	DWORD		reserved	:  17;
} _XMOB_ANIMATIONTYPE;

// 이동 동작 타입
typedef enum tagMonsterMoveType
{
	_XMONSTERMOVETYPE_WALK = 0,
	_XMONSTERMOVETYPE_RUN
} _XMOTION_MONSTERMOVETYPE;

typedef enum tagMonsterAttackType
{
	_XMONSTERATTACKTYPE_ATTACK1 = 0,
	_XMONSTERATTACKTYPE_ATTACK2,
	_XMONSTERATTACKTYPE_ATTACK3,
	_XMONSTERATTACKTYPE_RATTACK
} _XMOTION_MONSTERATTACKTYPE;

typedef enum tagMonsterDieType
{
	_XMONSTERDIETYPE_DIE1 = 0,
	_XMONSTERDIETYPE_DIE2
} _XMOTION_MONSTERDIETYPE;

typedef enum tagMonsterSkillType
{
	_XMONSTERSKILLTYPE_SKILL1 = 0,
	_XMONSTERSKILLTYPE_SKILL2
} _XMOTION_MONSTERSKILLTYPE;

typedef enum tagMonsterTransformStep
{
	_XMONSTERTRANSFORMSTEP_NONE = 0,
	_XMONSTERTRANSFORMSTEP_START,
	_XMONSTERTRANSFORMSTEP_END		
} _XMONSTERTRANSFORMSTEP;

// monster skill id
#define _XMOB_SKILL_TRANSFORM			6		//변신
#define _XMOB_SKILL_TRANSFORM_BACK		7		//변신 되돌아가기
#define _XMOB_SKILL_STRONGBODY			8		//금강불괴
#define _XMOB_SKILL_INVISIBLE			9		//반투명
#define _XMOB_SKILL_MEDITATION			10		//운기조식
#define _XMOB_SKILL_CURE				11		//치료
#define _XMOB_SKILL_INSTIGATION			12		//선동
#define _XMOB_SKILL_RECKLESS			13		//폭주
#define _XMOB_SKILL_EXTERN_TRANSFORM	14		//외형변화
#define _XMOB_SKILL_SIZE_TRANSFORM		15		//크기변신
#define _XMOB_SKILL_CHANGE_WEAPON		16		//무기 변환
#define _XMOB_SKILL_SEPARATION			17		//분리
#define _XMOB_SKILL_UNION				18		//합체
#define _XMOB_SKILL_HALLUCINATION		19		//분신
#define _XMOB_SKILL_SUICIDE				20		//자살
#define _XMOB_SKILL_REBIRTH				21		//부활
#define _XMOB_SKILL_CALL				22		//호출
#define _XMOB_SKILL_SUMMON				23		//소환

///////////////////////////////////////////////////////////////////////
// Monster action number...

#define		_XMOBACTIONNUMBER_IDLE			0
#define		_XMOBACTIONNUMBER_WALK			1
#define		_XMOBACTIONNUMBER_RUN			2
#define		_XMOBACTIONNUMBER_ATTACK1		3
#define		_XMOBACTIONNUMBER_ATTACK2		4
#define		_XMOBACTIONNUMBER_ATTACK3		5
#define		_XMOBACTIONNUMBER_BLOW			6
#define		_XMOBACTIONNUMBER_DIE1			7
#define		_XMOBACTIONNUMBER_DIE2			8
#define		_XMOBACTIONNUMBER_SKILL1		9
#define		_XMOBACTIONNUMBER_SKILL2		10
#define		_XMOBACTIONNUMBER_ENTERING		11
#define		_XMOBACTIONNUMBER_RATTACK		12

// 실제 동작별 타입
typedef enum tagMobMotionclassType
{
	_XMOBACTION_NONE	=	0,
	_XMOBACTION_IDLE,
	_XMOBACTION_MOVE,
	_XMOBACTION_ATTACK,
	_XMOBACTION_BLOW,
	_XMOBACTION_DIE,
	_XMOBACTION_SKILL,
	_XMOBACTION_ENTERING,
} _XMOBMOTIONCLASS_TYPE;

typedef struct _XMobMotionExistTable
{
	char	motion_idle;			
	char	motion_walk;
	char	motion_run;	
	char	motion_attack1;
	char	motion_attack2;
	char	motion_attack3;
	char	motion_blow;	
	char	motion_die1;
	char	motion_die2;
	char	motion_skill1;
	char	motion_skill2;
	char	motion_entering;
	char	motion_rattack;
	
	_XMobMotionExistTable()
	{
		motion_idle		=	-1;
		motion_walk		=	-1;
		motion_run		=	-1;		
		motion_attack1	=	-1;
		motion_attack2	=	-1;
		motion_attack3	=	-1;		
		motion_blow		=	-1;		
		motion_die1		=	-1;
		motion_die2		=	-1;		
		motion_skill1	=	-1;
		motion_skill2	=	-1;
		motion_entering	=	-1;
		motion_rattack	=	-1;
	}	
} _XMobMotionExistTable;

typedef struct 
{
	FLOAT xsize;
	FLOAT ysize;	
	FLOAT collideradius;
	FLOAT movespeed_walk;	
	FLOAT movespeed_run;	
	FLOAT attackrange;
	BOOL  standingerect;
	BYTE  weaponmaterial;
	BYTE  weaponweightclass;
	bool  soundplayed;		// 타입별로 한번씩만 사운드출력

	short level;
	short skill1_id;
	short skill2_id;
	short attackpowermax;	// 클라이언트 몬스터에서만 공격력을 쓴다.
	char  party;
	
#ifdef _XTS_MONSTERREF_13
	unsigned short	scripttype;
#else
	char  scripttype;// LPS 종류를 구분한다.
#endif

	char  key_type;// LPS 종류를 구분한다.
	short key_id;// LPS 종류를 구분한다.
	
	char  rank;		// monster rank - 레이드, 보스 몬스터 etc..
	
	bool  usecurrentzone;
	char  explevel;
#ifdef _XDEF_MONSTERREF_14
	char  droplevel[30];
#else
	char  droplevel[12];
#endif

	int	resistMinus;	//음저항력
	int	resistPlus;		//양저항력
	int	resistHon;		//혼저항력
	int	resistSin;		//신저항력
	
	int	attribute;		// 음양혼신 속성 (몬스터 이름 표시에 참조 v1.5에서 부터 유효)

} _MOB_ATTRIBTABLE;

typedef struct _XMob_AttackResult
{
	short	sAddEffect[3];
	short	sEff_Time[3];
}_XMOB_ATTACKRESULT;

enum STATE_MOBACT { SA_NONE, SA_MOVE, SA_ATTACK };	// hotblood zerolevel

// 상태효과
struct _XMOB_STATE
{
	char	type;				// 0 : spell		1 : effect

	short	spellID;
	char	spellStep;

	short	 effectID;
};

// by mahwang
// 이제는 _XItem 을 상속 받지 않아도 상관 없으나 전역 변수로 포인트 체크 하는 부분이 있어서 
// 그냥 남겨 둔다.
class _XMob : public _XItem  
{

public:	
	unsigned short			m_MobID;					// 서버에서 몬스터에게 부여하는 유니크 아이디 
	unsigned short			m_MobType;					// 몬스터 종류를 나타내는 변수 
	// transform
	unsigned short			m_NextTransformType;
	
	char					m_HealthPercentage;
	char					m_DisplayHealthPercentage;
		
	BOOL					m_InsideFrustum;
	BOOL					m_bShowMobName;
	BOOL					m_bShowBoldMobName;
	BOOL					m_bShowHPGauge;
	// Impact value
	BOOL					m_bImpact;
	// transform
	BOOL					m_RestoreOrgMonsterType;
	// hotblood zerolevel
	BOOL					m_bHoming;
	BOOL					m_bTargetOn;
	// client monster 
	BOOL					m_bClientMob;				// 클라이언트에서만 처리하는 몬스터인지를 알 수 있는 변수
	// client DNPC
	BOOL					m_bClientDNPC;				// 클라이언트에서만 처리하는 DNPC인지를 알 수 있는 변수

	int						m_PrevMotion;
	int						m_AlphaLevel;	
	int						m_AttackInfo;
	int						m_KillSignal;
	int						m_SpeedRate;
	// Impact value
	int						m_ImpactValue;
	// hotblood zerolevel
	int						m_Gender;
	int						m_Groupindex;
	int						m_ItemNo;
	int						m_AttackType;	// 모션 인덱스 	
	int						m_SkillIndex;	// 스킬 인덱스 	
	int						m_MaxLife;	
	int						m_CurrentLife;
	int						m_QueueNumber;	

	FLOAT					m_fMoveSpeed;
	FLOAT					m_PrevTime;	
	FLOAT					m_RotateAngle;
	FLOAT					m_AttackHitValue;
	// hotblood zerolevel
	FLOAT					m_OldRotateAngle;

	DWORD					m_LastRecivedMessageTime;	// 마지막으로 메세지를 받은 시간... 일정 시간이 지나면 자동 소멸
	DWORD					m_DieToDeleteTimer;
	DWORD					m_DieSignalReceiveTime;
	// transform
	DWORD					m_TranformTimer;
	// 상태 효과
	DWORD					m_ConditionList;
	
	
	D3DXVECTOR3				m_Position;	
	D3DXVECTOR3				m_MoveVector;
	D3DXVECTOR3				m_EndPosition;
	// Impact value	
	D3DXVECTOR3				m_OriginPosition;
	D3DXVECTOR3				m_ViewDirection;

	D3DXMATRIX				m_MobMatrix;
				
	_XModelDescriptor		m_ModelDescriptor;	
	_XMOB_ANIMATIONTYPE		m_Action;
	_XMOB_ATTACKRESULT		m_AttackResult;
	STATE_MOBACT			m_MobActState;	

	_XStickGauge			m_MonsterMiniHpBorder;
	_XImageStatic			m_imageMiniHpBackBorder;
	
	_XPICKINGTARGETTYPE		m_AttackTargetObjectType;
	// transform
	_XMONSTERTRANSFORMSTEP  m_TransformStep;

	_XItem*					m_AttackTarget;
	// 상태 효과
	list <_XMOB_STATE*>		m_StateList;

public:
	_XMob();
	virtual ~_XMob();

	virtual	void			InitializeData();
	virtual	void			DestroyData();

	virtual	void			Create( unsigned short mobid, unsigned short mobtype, D3DXVECTOR3 position, FLOAT angle,
									unsigned char health, int defaultmotion );
	
	virtual	void			CheckInsideFrustum( void );

	virtual	void			Process( void );
	virtual	void			Render( void );	

	inline void				SetPosition( D3DXVECTOR3 position ){ m_Position = position; }
	inline void				SetAngle( FLOAT angle ){ m_RotateAngle = angle; }	

	virtual	void			ResetMobProcessTime( void );

	virtual	void			ResetAnimationState( void );
	virtual void			SetAnimation( int animationindex );
	virtual	void			SetAnimationIndex( int animationindex );
	virtual	void			SetBlowMotion(BOOL bCritical = false);

	virtual	void			UpdatePosition( void );

	virtual	void			CreateEffectManager( void );
	virtual	void			StartImpactEffect(void);
	virtual	void			SetImpactPosition(void);
	virtual	void			ResetImpactEffect(void);
	
	virtual	void			PlayAttackSound(void);
	virtual	void			SetCharacterMobAnimation( _XMOBMOTIONCLASS_TYPE motionclass, bool changemotion = true );//hotblood zerolevel
	
	BOOL					CheckAnimationPriority(int animationindex);
	
	void					ResetAttackTarget(void);
	void					MoveNextPostion(BOOL bInterpolate = FALSE);
	
	void					ProcessTargetMobDie(void);

	// 상태 효과
	void					InsertState(char type, short id, char spellstep = 0);
	void					DeleteState(char type, short id, char spellstep = 0);
	void					ReleaseState(void);

	// Condition
	void					InsertCondition(int index);
	void					DeleteCondition(int index);
};

// hotblood zerolevel
class _XZeroLevelMob : public _XMob
{
public:
	void			UpdatePosition( void );
	void			Render( void );	

};

struct DELETE_MOB_INFO
{
	unsigned short	usMobType;
	FLOAT			fTime;
};

typedef std::map<unsigned short, _XMob*>	smdef_XMob;

typedef std::list<DELETE_MOB_INFO>			sldef_DeleteMobType;

class _XMobList : public _XModelManager
{

private:
public:
	DWORD					m_LastPlaySoundTime;
	DWORD					m_ClientMobAIDelayTime;

	XMemoryPool<_XMob>		m_MobPool;
	smdef_XMob				m_smMobList;	

	sldef_DeleteMobType		m_slDeleteMotType;
	
public:

	_XMobList();
	~_XMobList();

	void	CreatePool(void);
	void	DeletePool(void);

private:
	_XMob*	NewMob();
	void	DeleteMob(_XMob* pMob);
public:

	void	disposeList(void);
	void	disposeList_zero(void);
	
	BOOL	AddReferenceModel(unsigned short usMobType) ;
	BOOL	AddReferenceModel_Precaching(unsigned short usMobType) ;
	void	ReleaseReferenceModel(unsigned short usMobType, BOOL bConstraint = FALSE);

	_XMob*	CreateMob(unsigned short usMobId, unsigned short usMobType, unsigned char ucMobWalkMode, 
								char cMobVitalRate,
								D3DXVECTOR3& vSrcPosition, D3DXVECTOR3& vDestPosition, bool bRebirth = false);
	
	_XMob*	CreateMobFromMove(unsigned short usMobId, unsigned short usMobType, unsigned char ucMobWalkMode,
								char cMobVitalRate, D3DXVECTOR3& vScrPosition, D3DXVECTOR3& vDestPosition);
			
private:
	// AddMob 은 CreateMob 함수 안에서만 호출 되도록 한다.
	// Mob 을 생성할대는 항상 CreateMob 을 이용해서 호출 한다.
	BOOL	AddMob		( _XMob* mob	);
public:
	// zero level 에서의 특수 처리를 위한 함수
	BOOL	AddMob_zero	( _XMob* mob	)		{ return AddMob(mob); }

	_XMob*  FindMob		( unsigned short mobid	);
	void	DeleteMob	( UINT mobid	);
	
	// 클라이언트 몬스터를 생성할 때 클라이언트 고유의 아이디를 얻어낸다.
	int		FindClientMobID	( void);
public:

	void    Process		( void );
	void	Process_ZeroLevel( void );	// hotblood zerolevel
	void	Process_VirtualMode( void );	// 가상공간에서의 몬스터 프로세스를 따로 처리한다.
	void	Render		( void );
	void	Render_ZeroLevel( void );	// hotblood zerolevel

	void    RemoveAttackTarget( _XUser* user );
	void	RemoveEffectInstance( int templateid );

	void	CheckInsideFrustum( void );
	void	MobAIProcess( void );//hotblood zerolevel
	void	ClientMobAIProcess( void );//클라이언트에서 몬스터를 조정함. 튜토리얼 모드에서 사용된다.
};


extern int						g_CurrentCachedMobcount;
extern _MOB_ATTRIBTABLE			g_MobAttribTable[_XDEF_MODELCLASSCOUNT_MOB];
extern _XMobMotionExistTable	g_MobMotionExistTable[_XDEF_MODELCLASSCOUNT_MOB];

#endif // !defined(_XMOB_H_)
