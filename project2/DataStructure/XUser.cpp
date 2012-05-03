// XUserInfo.cpp: implementation of the XUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMob.h"
#include "XUser.h"
#include "SoundEffectList.h"
#include "XProc_MainGameDef.h"
#include "XWindow_Skill.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindow_PKTracingRequest.h"
#include "XPathFinder.h"
#include "XParticleIDDefine.h"
#include "XWindowObjectDefine.h"

#include "XSR_STRINGHEADER.h"

// -------------------------------------------------------------------------------------
// global and extern 
_XUSERLEVEL_LUTTYPE		g_UserLevelTable[241];		// 로컬 유저 레벨관련 계산 LUT
_XUSER_POINTTABLE		g_UsePointTable[12];
D3DXVECTOR3				g_UserIntersectionPoly[8];
TCHAR					g_CharacterRollNameTable[_XGROUP_MAXGROUPCOUNT][_XROLLCLASS_MAXCOUNT][5][64];
extern FLOAT			g_fWaterDepth; // 수심 몇미터까지 들어갈수 있다.

#ifdef _XDWDEBUG	
extern BOOL				g_SpiderMode;
#endif

//////////////////////////////////////////////////////////////////////
// _XUser
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
_XUser::_XUser()
{
	m_RecievedCharacterInfo = FALSE;
}
_XUser::~_XUser()
{
}

void _XUser::InitializeData()
{	
	m_KillSignal					= FALSE;	
	m_LastRecivedMessageTime		= 0;
	m_MoveVector					= m_TargetPosition = m_Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_RotateAngle					= -0.0f;
	Set_m_fMoveSpeed(_XDEF_CHARACTERMOVESPEED_WALK);
	Set_m_fPrevMoveSpeed(_XDEF_CHARACTERMOVESPEED_WALK); 
	Set_m_fHiperMoveSpeed(_XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1);
	m_PrevProcessTime				= -1.0f;		// 반드시 -1을 넣어서 글로벌타임을 받아온다. -1값은 아니지만...
	m_CheckInFrustum				= !FALSE;
	m_PrevPosition = m_CurrentPositionTerrainNormal	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	m_BattleMode					= _XBATTLE_PEACE;
	m_AttackRange					= 0.0f; //0.3f;
	m_StopAnimation					= FALSE;

	m_AttackTarget					= NULL;
	m_AttackTargetObjectType		= _XPICKINGTARGET_NONE;
	
	m_pCollideOBB					= NULL;
	m_pGoundCollideOBB				= NULL;

	m_PrevMoveTime					= 0.0f;
	m_LastRotateAngle				= 0.0f;
	m_RotateDirection				= _XROTATE_NONE;
	
	m_DisarmamentItem				= NULL;

	// 개인 상점 관련
	m_PersonalStoreState			= _XUSER_PERSONALSTORE_NONE;
	memset(m_PersonalStoreName, 0, sizeof(m_PersonalStoreName));
	
	// 비무 홍보 관련
	m_bMatchPRMode					= FALSE;
	memset(m_stringMatchPR, 0, sizeof(m_stringMatchPR));
	
	m_bInPartyMember				= FALSE;

	m_ModelDescriptor.ReserveModelStack( _XDEF_MAXMODEL_COMBINATIONCOUNT );
	m_ModelDescriptor.m_SwordEffectInitialized = FALSE;
	
	m_ModelDescriptor.SetShadowDecalInfo( 0.65f, 0.85f, 2.0f );

	memset(&m_AttackResult, 0, sizeof(m_AttackResult));
	memset(&m_RegionDamageResult, 0, sizeof(m_RegionDamageResult));
	
	// m_CharacterOBB 는 생성할때 메모리를 지우고 다시 생성하므로 
	// 따로 지워주지 않는다. 지우는 함수로 따로 있지 않아서 그냥 있는 
	// 루틴으로 사용한다.
	m_CharacterOBB.SetCenter( D3DXVECTOR3(0.0f, 0.5, 0.0f ) );
	m_CharacterOBB.SetExtents( D3DXVECTOR3(0.16f, 0.5, 0.16f ) );
	D3DXVECTOR3 Axis[3] = 
	{
		D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f )
	};
	m_CharacterOBB.SetAxis( Axis );
	m_CharacterOBB.RecalcRadius();

#ifdef _XDWDEBUG
	m_CharacterOBB.CreateOBBModel();
#endif

	m_Velocity	= D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_StanceMotionFirst = FALSE;

	m_JobIndexOfZeroLevel			= -1;	// hotblood->2004.09.17 
	
	Set_m_MoveSpeedFromEffect(0);
	Set_m_AttackSpeedFromEffect(0);
	Set_m_bInvisible(FALSE);
	m_bFaceMaskMode = FALSE;
		
	m_HiperRunSkillID = 0;
	m_HiperRunStep = 0;

	m_CurrentWeaponID		= 0;
	m_bFinishDashStart		= FALSE;
	m_FinishSpeed			= 0;
	
	m_RecievedCharacterInfo = FALSE;
	m_StanceMotionIndex		= 0;

	m_bEnemy				= FALSE;
	m_bPEMode				= FALSE;
	
	m_nIdleToMoveCnt		= 0;
	m_bOut					= FALSE;
	
	m_bEnterMatchEvent		= FALSE;
	m_nWinnerNickName		= 0;
}

void _XUser::DestroyData()
{
	_XDWINPRINT( "User remove : [%s]", m_CharacterInfo.charactername );
	memset(&m_CharacterInfo, 0, sizeof(m_CharacterInfo));

	m_ModelDescriptor.Reset();
	m_RecievedCharacterInfo = FALSE;
}

//------------------------------------------------------------------------------------------
// static functions
void _XUser::CreateIntersectionPoly( void )
{
	memset( g_UserIntersectionPoly, 0, sizeof( g_UserIntersectionPoly ) );

	FLOAT xsize = 0.3f;
	FLOAT ysize = 1.2f;

	FLOAT halfxsize = xsize / 2.0f;
	FLOAT halfysize = ysize / 2.0f;	
	FLOAT movey = _XDEF_COLLIDEOFFSET_HUMAN;

	//////////////////////////////////////////////////////////////////
	// First plane...
	// Left top
	g_UserIntersectionPoly[0].x = -halfxsize;
	g_UserIntersectionPoly[0].y = movey-halfysize;
	g_UserIntersectionPoly[0].z =  0.0f;

	// Right top
	g_UserIntersectionPoly[1].x =  halfxsize;
	g_UserIntersectionPoly[1].y = movey-halfysize;
	g_UserIntersectionPoly[1].z =  0.0f;

	// right bottom 
	g_UserIntersectionPoly[2].x =  halfxsize;
	g_UserIntersectionPoly[2].y =  movey+halfysize;
	g_UserIntersectionPoly[2].z =  0.0f;

	// left bottom 
	g_UserIntersectionPoly[3].x = -halfxsize;
	g_UserIntersectionPoly[3].y =  movey+halfysize;
	g_UserIntersectionPoly[3].z =  0.0f;

	//////////////////////////////////////////////////////////////////
	// Second cross plane...
	// Left top
	g_UserIntersectionPoly[4].x =  0.0f;
	g_UserIntersectionPoly[4].y =  movey+halfysize;
	g_UserIntersectionPoly[4].z = -halfxsize;

	// Right top
	g_UserIntersectionPoly[5].x =  0.0f;
	g_UserIntersectionPoly[5].y =  movey+halfysize;
	g_UserIntersectionPoly[5].z =  halfxsize;

	// right bottom 
	g_UserIntersectionPoly[6].x =  0.0f;
	g_UserIntersectionPoly[6].y =  movey-halfysize;
	g_UserIntersectionPoly[6].z =  halfxsize;

	// left bottom 
	g_UserIntersectionPoly[7].x =  0.0f;
	g_UserIntersectionPoly[7].y =  movey-halfysize;
	g_UserIntersectionPoly[7].z =  -halfxsize;	
}
	

BOOL _XUser::IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 )
{
    // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &g_vPickRayDir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = g_vPickRayOrig - v0;
    }
    else
    {
        tvec = v0 - g_vPickRayOrig;
        det = -det;
    }

    if( det < 0.0001f ) return FALSE;

	FLOAT u, v;

    // Calculate U parameter and test bounds
    u = D3DXVec3Dot( &tvec, &pvec );
    if( u < 0.0f || u > det ) return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    v = D3DXVec3Dot( &g_vPickRayDir, &qvec );
    if( v < 0.0f || u + v > det ) return FALSE;
	
    return TRUE;
}

//------------------------------------------------------------------------------------------
// Get/Set functions
void _XUser::SetPosition( D3DXVECTOR3 position )
{ 
	m_Position = position; 
}

void _XUser::SetAngle( FLOAT angle )
{ 
	m_RotateAngle = angle; 
}

void _XUser::ResetProcessTime( void )
{
	if( g_fLocalSystemTime == 0.0f)
		 m_PrevProcessTime = -1.0f;
	else m_PrevProcessTime = g_fLocalSystemTime;
}

void _XUser::ResetZoneData( void )
{
	m_pCollideOBB					= NULL;
	m_pGoundCollideOBB				= NULL;
	m_MoveVector					= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_TargetPosition				= m_Position;
	m_CurrentWeaponID				= 0;
}


void _XUser::SetAttackTarget( _XItem* target, _XPICKINGTARGETTYPE targettype )
{
	m_AttackTarget = target;
	m_AttackTargetObjectType = targettype;	
}

void _XUser::ResetAttackTarget(void)
{
	m_AttackTarget = NULL;
	m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
}

void _XUser::SetBattleMode(  _XBATTLEMODETYPE mode )
{
	m_BattleMode = mode;
	
	if(m_BattleMode == _XBATTLE_PEACE)
		m_CharacterInfo.animationdescript.idletype	= _XBATTLE_PEACE;
	else
		m_CharacterInfo.animationdescript.idletype = _XBATTLE_BATTLE;
	
//	_XDWINPRINT("[XUser/SetBattleMode]");
	SetAnimation( m_CharacterInfo.animationdescript.motionclass );
}

void _XUser::ResetAnimationState( void )
{	
//	_XDWINPRINT("[XUser/ResetAnimationState]");
	SetAnimation( m_CharacterInfo.animationdescript.motionclass );	
	m_ModelDescriptor.PlayAnimation( true );
}


//#define _XDEF_ANOTHERUSERCHECKGROUNDNORMALVECTOR

void _XUser::UpdatePosition( void )
{	
	D3DXMATRIX	rotmat;
	D3DXMatrixIdentity( &rotmat );
	D3DXMatrixRotationY( &rotmat, m_RotateAngle );

	if( m_pGoundCollideOBB )
	{
		//m_Position.y = m_pGoundCollideOBB->m_kCenter.y + m_pGoundCollideOBB->m_afExtent[1];
		FLOAT terrainheight = g_LodTerrain.GetTerrainHeight( m_Position );
		if( terrainheight > m_Position.y ) m_Position.y = terrainheight;
	}
	else 
	{
		m_Position.y = g_LodTerrain.GetTerrainHeight( m_Position );
	}
	
	D3DXMatrixTranslation( &m_ModelDescriptor.m_Position, m_Position.x, m_Position.y, m_Position.z );
	
#ifdef _XDEF_ANOTHERUSERCHECKGROUNDNORMALVECTOR
	
	D3DXMATRIX  rotmatrix;
	D3DXQUATERNION normalrotquat;
	D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);

	if( m_CurrentPositionTerrainNormal.x != 0.0f || m_CurrentPositionTerrainNormal.y != 1.0f || m_CurrentPositionTerrainNormal.z != 0.0f ) // check exactly upvector...
	{
		// rotate from terrain normal vector...					
		FLOAT theta = _XGetAngleFromVectors( &upvector, &m_CurrentPositionTerrainNormal );
		
		if( theta > 0.122173022f/* deg 7' */ )
		{
			FLOAT dev = theta / 0.122173022f; //0.0872664444f(5');

			D3DXVECTOR3 modvector = m_CurrentPositionTerrainNormal;

			modvector.x /= dev;
			modvector.z /= dev;

			normalrotquat = _XRotationArcNormalized( &upvector, &modvector );
		}
		else
		{
			normalrotquat = _XRotationArcNormalized( &upvector, &m_CurrentPositionTerrainNormal  );
		}
		
		D3DXMatrixRotationQuaternion( &rotmatrix, &normalrotquat );
		D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmatrix, &m_ModelDescriptor.m_Position );								
	}				
	
#endif

	D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position );

	CheckInsideFrustum();
}

void _XUser::UpdateRotateAngle(void)
{
	if((m_LastRotateAngle - m_RotateAngle) <= 0.2f && (m_LastRotateAngle - m_RotateAngle) >= -0.2f)
	{
		m_RotateAngle = m_LastRotateAngle;
		return;
	}
	
	if(m_RotateDirection == _XROTATE_CLOCKWISE)
	{
		m_RotateAngle += _X_RAD(10);
		if(m_RotateAngle >= _X_PI)
			m_RotateAngle = -_X_PI;
	}
	else
	{
		m_RotateAngle -= _X_RAD(10);
		if(m_RotateAngle <= -_X_PI)
			m_RotateAngle = _X_PI;
	}
}

void _XUser::SetStopAction( void )
{
	if(!DoNotChangeMotion())
	{
//		_XDWINPRINT("[XUser/SetStopAnimaion] SetMotion Idle mode");

		switch(m_ModelDescriptor.m_CurMotion)
		{					
		case _XAN_MOVE_NONE_REPEAT		 :		
			SetAnimationIndex(_XAN_IDLE_NONE);
			break;
		case _XAN_MOVE_SHORTWEAPON_REPEAT:		
			SetAnimationIndex(_XAN_IDLE_SHORTWEAPON);
			break;
		case _XAN_MOVE_LONGWEAPON_REPEAT :		
			SetAnimationIndex(_XAN_IDLE_LONGWEAPON);
			break;
		case _XAN_MOVE_SOFTWEAPON_REPEAT :
			SetAnimationIndex(_XAN_IDLE_NONE);
			break;
		case _XAN_MOVE_SWORD_REPEAT_PEACE :
			SetAnimationIndex(_XAN_IDLE_SWORD_PEACE);
			break;
		case  _XAN_MOVE_SWORD_REPEAT_BATTLE :
			SetAnimationIndex(_XAN_BATTLEIDLE_SWORD_REPEAT);
			break;
		}
	}
}

void _XUser::SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion )
{
	int motionindex = m_ModelDescriptor.GetCurMotion();

	if(motionclass < 0)
		return;

	if(GetMotionClass() == _XACTION_DYING)
	{
		if(motionclass == _XACTION_DYING)
			return;
	}
	
	// 이동 시작 또는 종료 동작교체시는 이동시작시간 초기화 
	if( motionclass != _XACTION_MOVE ) m_PrevMoveTime = 0;

	switch( motionclass )
	{
	case _XACTION_IDLE :		
		{
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;

			if(m_ModelDescriptor.m_LockEndFrame)
				m_ModelDescriptor.SetLockEndFrame(FALSE);
			if(m_StopAnimation)
				m_StopAnimation = FALSE;
			if(!m_ModelDescriptor.m_AniFlag)
				m_ModelDescriptor.PlayAnimation(TRUE);

			if(m_StanceMotionIndex > 0)
			{
				m_ModelDescriptor.SetCurMotion(m_StanceMotionIndex);
				break;
			}
			
			if( m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE )
			{
				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{
					m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{				
							case _XGI_SC_WEAPON_SHORT				:				// 단병기
								
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// 장병기
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// 연병기
								if( m_CharacterInfo.gender == _XGENDER_MAN )
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// 암기
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_MUSICAL				:				// 악기
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							}
						}
					}
#else
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							
							if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							if( m_CharacterInfo.gender == _XGENDER_MAN )
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							
							if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							if( m_CharacterInfo.gender == _XGENDER_MAN )
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							
							if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							if( m_CharacterInfo.gender == _XGENDER_MAN )
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
#endif
				}
			}
			else
			{			
				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{
					m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// 단병기
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// 장병기
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// 연병기
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// 암기
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_MUSICAL					:			// 악기
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							}							
						}
					}
#else
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL					:			// 악기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}							
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL					:			// 악기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL					:			// 악기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
#endif
				}
			}		
		}
		break;
	
	case _XACTION_MOVE :		
		m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
		if( m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN )
		{			
			SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_WALK );
			
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
				m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
							}
							else
							{						
								m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_START, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기					
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_START, TRUE );					
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_START, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_START, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_START, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_START, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_START, TRUE );
						break;
					}
				}
#endif
			}
		}
		else if( m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG )
		{
#ifdef _XDEF_NEWANIMATIONLIST
			SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );
			
			int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
								  g_SkillProperty[m_HiperRunSkillID]->motion_male : 
								  g_SkillProperty[m_HiperRunSkillID]->motion_female;
			
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
				m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SWORD, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_BATTLESWORD, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_LONGWEAPON, TRUE);
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
							break;
						}
					}
				}
#else
				_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
				
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					pWeaponItemProperty = g_WeaponItemProperty;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					pWeaponItemProperty = g_WeaponItemProperty2;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					pWeaponItemProperty = g_WeaponItemProperty3;
				
				switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
					}
					else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SWORD, TRUE);
						else
							m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_BATTLESWORD, TRUE);
					}
					else
					{
						m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_LONGWEAPON, TRUE);
					break;				
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
					break;
				}
#endif
			}
#else
			SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );

			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
				m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}
				}
#endif
			}
#endif
		}

		break;	
	
	case _XACTION_BLOW :
		m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;

		if( m_CharacterInfo.modeldescript.item1 == 0 ) // 아무것도 안들은 경우 
		{
			m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );							
						break;
					case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
						break;				
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );							
					break;
				case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
					break;
				case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
					break;				
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );							
					break;
				case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
					break;
				case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
					break;				
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );							
					break;
				case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
					break;
				case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );
					break;				
				}
			}
#endif
		}		
		break;

	case _XACTION_DYING :
		{
			if(m_CharacterInfo.modeldescript.item1 == 0)
			{
				m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
							break;
						case _XGI_SC_WEAPON_LONG :
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
							break;
						case _XGI_SC_WEAPON_HIDDEN :
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							break;
						case _XGI_SC_WEAPON_MUSICAL :
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_LONG :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_LONG :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_LONG :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						break;
					}
				}
#endif
			}

			{
				int rand = _XRandom() % 3;
				
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}

			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
		}
		break;

	case _XACTION_DIE :
		{
			if( m_CharacterInfo.modeldescript.item1 == 0 ) // 아무것도 안들은 경우 
			{
				m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
								m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
							break;			
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					}
				}
#endif
			}

		m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
		}
		break;

	case _XACTION_ATTACK :		
		{						
//			m_BattleMode = _XBATTLE_BATTLE;
			m_ModelDescriptor.SetCurMotion( m_CharacterInfo.animationdescript.attacktype, changemotion );			
		}				
		break;
	case _XACTION_DEFENCE :
		{				
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );					
							break;
						case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;				
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );					
						break;
					case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;				
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );					
						break;
					case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;				
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );					
						break;
					case _XGI_SC_WEAPON_MUSICAL					:			// 악기				
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL					:			// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;				
					}
				}
#endif
			}
		}
		break;
	case _XACTION_RESCUE :
		{
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	case _XACTION_REBIRTH :
		{
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						}						
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	
	case _XACTION_LPS :
		{
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;

			if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );						
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_LPS_SHORTWEAPON_HANDTOUCH, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_LONGWEAPON_HANDTOUCH, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_SHORTWEAPON_HANDTOUCH, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_LONGWEAPON_HANDTOUCH, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_SHORTWEAPON_HANDTOUCH, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_LONGWEAPON_HANDTOUCH, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_SHORTWEAPON_HANDTOUCH, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_LONGWEAPON_HANDTOUCH, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_LPS_HANDTOUCH, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;

	default:
		break;
	}	
	
	m_CharacterInfo.animationdescript.motionclass = motionclass;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (motionclass == _XACTION_ATTACK);

#ifdef _XDWDEBUG
	if( g_EnableUserPacketFiltering )
	{
		if( strcmpi( m_CharacterInfo.charactername, g_PacketFilteringTargetName ) == 0 )
		{
			TCHAR _messagestring[64];
			_snprintf( _messagestring, sizeof(_messagestring), "%s User change motion : AnimIndex:%d Action:%d X:%4.2f Y:%4.2f", 
				m_CharacterInfo.charactername, 
				m_ModelDescriptor.m_CurMotion, m_CharacterInfo.animationdescript.motionclass, m_Position.x, m_Position.z );
			_XDebugOutputString( _messagestring, 1, 1 );
		}
	}
#endif
}

void _XUser::SetAnimationIndex( int animationindex, bool changemotion )
{
	if(animationindex < 0)
		return;
	
	if(animationindex == _XAN_DIE || animationindex == _XAN_DIE_SHORTWEAPON || animationindex == _XAN_DIE_LONGWEAPON ||
		animationindex == _XAN_DIE_SWORD_PEACE || animationindex == _XAN_DIE_SWORD_BATTLE)
	{
		if(GetMotionClass() == _XACTION_DIE)
		{
			// 이미 죽는 motion 세팅되어있음
			return;
		}
	}
	

#ifdef _XDEF_NEWANIMATIONLIST
	
	BOOL checkfastmove = FALSE;
	
	if( m_HiperRunSkillID )
	{
		if(	g_SkillProperty[m_HiperRunSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			_XSkillItem::GetTargetType(m_HiperRunSkillID) == _XSI_TARGETTYPE_SELF	&&
			g_SkillProperty[m_HiperRunSkillID]->skillSType == _XSI_STYPE_03 )
		{
			
			int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
								  g_SkillProperty[m_HiperRunSkillID]->motion_male : 
								  g_SkillProperty[m_HiperRunSkillID]->motion_female;
			
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
								else
									baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
							}
							else
							{
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							break;
						}
					}
				}
#else
				_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
				
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					pWeaponItemProperty = g_WeaponItemProperty;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					pWeaponItemProperty = g_WeaponItemProperty2;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					pWeaponItemProperty = g_WeaponItemProperty3;
				
				switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
					}
					else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
						else
							baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
					}
					else
					{
						baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
					break;				
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					break;
				}
#endif
			}
			
			if( animationindex == baseanimindex )
			{
				checkfastmove = TRUE;					
				
				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );
			}
			else if( animationindex == baseanimindex+1 )
			{
				checkfastmove = TRUE;
				
				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				
				if( m_HiperRunStep < 12 )
				{
					SetMoveSpeed( g_SkillProperty[m_HiperRunSkillID]->detailInfo[m_HiperRunStep].finishSpeed );
				}
				
			}			
			else if( animationindex == baseanimindex+2 )
			{
				checkfastmove = TRUE;
				
				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			}
		}
	}
	
	if( !checkfastmove )	
		
#endif
	{

		switch( animationindex )
		{
		case _XAN_IDLE_NONE					:	
		case _XAN_IDLE_NONE_ACTION			:		
		case _XAN_IDLE_SHORTWEAPON			:	
		case _XAN_IDLE_SHORTWEAPON_ACTION	:		
		case _XAN_IDLE_LONGWEAPON			:	
		case _XAN_IDLE_LONGWEAPON_ACTION	:
		case _XAN_IDLE_SWORD_PEACE			:
		case _XAN_IDLE_SWORD_ACTION			:
			m_CharacterInfo.animationdescript.idletype			= _XBATTLE_PEACE;
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			break;	
			
		case _XAN_BATTLEIDLE_NONE			: 
		case _XAN_BATTLEIDLE_SHORTWEAPON	:
		case _XAN_BATTLEIDLE_LONGWEAPON		:
		case _XAN_BATTLEIDLE_SWORD_REPEAT :
			m_CharacterInfo.animationdescript.idletype			= _XBATTLE_BATTLE;
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			break;	
		case _XAN_EMOTION_MARRYCLAPPINGHAND	:		
		case _XAN_EMOTION_MARRYBOX			:		
		case _XAN_EMOTION_MARRYBOWPOLITELY	:
			{
				DeleteWeaponModel();
				m_CharacterInfo.animationdescript.motionclass	= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}			
			break;			
		case _XAN_EMOTION_BOW				:
		case _XAN_EMOTION_BOWPOLITELY		:
		case _XAN_EMOTION_EXCITEMENT		:
		case _XAN_EMOTION_RUSH				:	
		case _XAN_EMOTION_CRINGESTART		:
		case _XAN_EMOTION_CRINGEREPEAT		:
		case _XAN_EMOTION_CRINGEEND			:	
		case _XAN_EMOTION_CHEER				:
		case _XAN_EMOTION_BREAKDOWN			:
		case _XAN_EMOTION_CRY				:
		case _XAN_EMOTION_LAUGH				:				
		case _XAN_EMOTION_ANGRY				:
		case _XAN_EMOTION_DOZ				:
		case _XAN_EMOTION_SITDOWNSTART		:
		case _XAN_EMOTION_SITDOWNREPEAT		:
		case _XAN_EMOTION_SITDOWNEND		:
#ifndef _XTS_NEWINSENSIBLE
		case _XAN_EMOTION_CLAPPINGHAND		:
#endif
		case _XAN_KNEE_SWORD_START			:
		case _XAN_KNEE_SWORD_REPEAT			:
		case _XAN_KNEE_SWORD_END			:
		case _XAN_MATCH_PR_START			:
		case _XAN_MATCH_PR_REPEAT			:
		case _XAN_MATCH_PR_END				:
		case _XAN_MATCH_WINNER_START		:
		case _XAN_MATCH_WINNER_REPEAT		:
		case _XAN_MATCH_LOSER_START			:
		case _XAN_MATCH_LOSER_REPEAT		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;
			
#ifdef _XTS_NEWINSENSIBLE
		case _XAN_EMOTION_CLAPPINGHAND		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_INSENSIBLE;
//			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );	//기절 상태일 때 피킹 제거하지 않음 //Author : 박소현
			break;
#endif
		
		case  _XAN_MOVE_NONE_START				:
		case  _XAN_MOVE_SHORTWEAPON_START		:
		case  _XAN_MOVE_LONGWEAPON_START		:
		case  _XAN_MOVE_SOFTWEAPON_START		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;
			
		case  _XAN_MOVE_NONE_REPEAT				:
		case  _XAN_MOVE_SHORTWEAPON_REPEAT		:	
		case  _XAN_MOVE_LONGWEAPON_REPEAT		:
		case  _XAN_MOVE_SOFTWEAPON_REPEAT		:
		case _XAN_MOVE_SWORD_REPEAT_PEACE		:
		case _XAN_MOVE_SWORD_REPEAT_BATTLE		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			break;
			
		case  _XAN_MOVE_NONE_END				:
		case  _XAN_MOVE_SHORTWEAPON_END			:
		case  _XAN_MOVE_LONGWEAPON_END			:
		case  _XAN_MOVE_SOFTWEAPON_END			:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;
			
		case  _XAN_FASTMOVE_NONE_START			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_START	:
		case  _XAN_FASTMOVE_LONGWEAPON_START	:
		case  _XAN_FASTMOVE_SOFTWEAPON_START	:
		case _XAN_FASTMOVE_SWORD_START_PEACE	:
		case _XAN_FASTMOVE_SWORD_START_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			break;
			
		case  _XAN_FASTMOVE_NONE_REPEAT			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_REPEAT	:	
		case  _XAN_FASTMOVE_LONGWEAPON_REPEAT	:	
		case  _XAN_FASTMOVE_SOFTWEAPON_REPEAT	:
		case _XAN_FASTMOVE_SWORD_REPEAT_PEACE	:
		case _XAN_FASTMOVE_SWORD_REPEAT_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			{
				if( m_HiperRunSkillID )
				{
					if( g_SkillProperty[m_HiperRunSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						g_SkillProperty[m_HiperRunSkillID]->targetType == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[m_HiperRunSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
					{
						if( m_HiperRunStep < 12 )
						{
							SetMoveSpeed( g_SkillProperty[m_HiperRunSkillID]->detailInfo[m_HiperRunStep].finishSpeed );
						}					
					}
				}
				else
				{
					SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1 );
				}
			}		
			break;
			
		case  _XAN_FASTMOVE_NONE_END			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_END		:
		case  _XAN_FASTMOVE_LONGWEAPON_END		:
		case  _XAN_FASTMOVE_SOFTWEAPON_END		:
		case _XAN_FASTMOVE_SWORD_END_PEACE		:
		case _XAN_FASTMOVE_SWORD_END_BATTLE		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;

		case _XAN_DYINGREADY_NONE :
		case _XAN_DYINGREADY_SHORTWEAPON :
		case _XAN_DYINGREADY_LONGWEAPON :
		case _XAN_DYINGREADY_SWORD_PEACE :
		case _XAN_DYINGREADY_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DYING;
			{
				int rand = _XRandom() % 3;
				
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}
			break;
			
		case _XAN_DYINGREPEAT_NONE :
		case _XAN_DYINGREPEAT_SHORTWEAPON :
		case _XAN_DYINGREPEAT_LONGWEAPON :
		case _XAN_DYINGREPEAT_SWORD_PEACE :
		case _XAN_DYINGREPEAT_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DYING;
			SetMoveSpeed(_XDEF_CHARACTERMOVESPEED_RUN);
			break;

		case _XAN_DIE :
		case _XAN_DIE_SHORTWEAPON :
		case _XAN_DIE_LONGWEAPON :
		case _XAN_DIE_SWORD_PEACE :
		case _XAN_DIE_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DIE;
			break;

		case _XAN_HITBACK_NONE :
		case _XAN_HITBACK_SHORTWEAPON :
		case _XAN_HITBACK_LONGWEAPON :
		case _XAN_HITBACK_SWORD_PEACE :
		case _XAN_HITBACK_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_RESCUE;
			break;

		case _XAN_WAKEUP_NONE :
		case _XAN_WAKEUP_SHORTWEAPON :
		case _XAN_WAKEUP_LONGWEAPON :
		case _XAN_WAKEUP_SWORD_PEACE :
		case _XAN_WAKEUP_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_REBIRTH;
			break;

		case _XAN_DEFENCE_NONE			:
		case _XAN_DEFENCE_SHORTWEAPON	:
		case _XAN_DEFENCE_LONGWEAPON	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DEFENCE;
			break;
			
		case _XAN_BLOW_NONE				:
		case _XAN_BLOW_SHORTWEAPON		:
		case _XAN_BLOW_LONGWEAPON		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_BLOW;
			break;	
			
		case _XAN_LPS_HANDTOUCH					:			
		case _XAN_LPS_SHORTWEAPON_HANDTOUCH		:			
		case _XAN_LPS_LONGWEAPON_HANDTOUCH		:			
		case _XAN_LPS_FOOTTOUCH					:			
		case _XAN_LPS_SHORTWEAPON_FOOTTOUCH		:			
		case _XAN_LPS_LONGWEAPON_FOOTTOUCH		:			
		case _XAN_LPS_USEKEY_HAND				:
		case _XAN_LPS_USEKEY_SHORTWEAPON		:
		case _XAN_LPS_USEKEY_LONGWEAPON			:
		case _XAN_LPS_USEKEY_SWORD_PEACE		:
		case _XAN_LPS_USEKEY_SWORD_BATTLE		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_LPS;
			break;	
			
		case _XAN_MATCH_NONE :
		case _XAN_MATCH_SHORTWEAPON :
		case _XAN_MATCH_LONGWEAPON :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_MATCH_START;
			break;

		case _XAN_MATCH_CANCEL :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_MATCH_CANCEL;
			break;

		default :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_ATTACK;
			m_CharacterInfo.animationdescript.attacktype = animationindex;
			break;
		}
	}
	
	m_ModelDescriptor.SetCurMotion( animationindex, changemotion );
	m_ModelDescriptor.m_ViewLeftSwordEffect = (m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK);

	// 이동 시작 또는 종료 동작교체시는 이동시작시간 초기화 
	if( m_CharacterInfo.animationdescript.motionclass != _XACTION_MOVE ) m_PrevMoveTime = 0;

#ifdef _XDWDEBUG
	if( g_EnableUserPacketFiltering )
	{
		if( strcmpi( m_CharacterInfo.charactername, g_PacketFilteringTargetName ) == 0 )
		{
			TCHAR _messagestring[64];
			_snprintf( _messagestring, sizeof(_messagestring), "%s User change motion : AnimIndex:%d Action:%d X:%4.2f Y:%4.2f", 
				     m_CharacterInfo.charactername, 
					 animationindex, m_CharacterInfo.animationdescript.motionclass, m_Position.x, m_Position.z );
			_XDebugOutputString( _messagestring, 1, 1 );
		}
	}
#endif

}

void _XUser::SetCharacterModel( BOOL dntchangemodel )
{			
	int extravalue = 0;
	if( m_CharacterInfo.gender== _XGENDER_WOMAN ) extravalue = g_Model_GenderSeparateIndex;
			
	if( m_CharacterInfo.groupindex == _XGROUP_SORIM )
	{
		if( m_CharacterInfo.gender == _XGENDER_MAN ) 
		{
			// 소림사 머리카락은 없슴.
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );

			if( m_CharacterInfo.modeldescript.face )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType	[g_Model_SorimHairSeparateIndex + m_CharacterInfo.modeldescript.face-1] );
			else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType   [g_Model_SorimHairSeparateIndex] );
		}
		else
		{   // 소림사에 여자일경우 (그럴리는 없겠지만 그래도 기본으로 설정)

			if( m_CharacterInfo.modeldescript.hair )
			{
				if( m_CharacterInfo.modeldescript.cap == 0 ) // 모자를 썻을때는 머리카락 처리 안함. 
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 g_UserModel_HairType	[extravalue+m_CharacterInfo.modeldescript.hair-1] );
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
			}
			else 
			{
				if( m_CharacterInfo.modeldescript.cap == 0 ) // 모자를 썻을때는 머리카락 처리 안함. 
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 g_UserModel_HairType	[extravalue] ); // 기본 머리카락 설정 
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
			}
			
			if( m_CharacterInfo.modeldescript.face )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType	[extravalue+m_CharacterInfo.modeldescript.face-1] );
			else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACE, 	 g_UserModel_FaceType	[extravalue] );  // 얼굴정보가 없으면 기본 출력 
		}
	}
	else
	{
		if( m_CharacterInfo.modeldescript.hair )
		{
			if( m_CharacterInfo.modeldescript.cap == 0 ) 
			{
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 g_UserModel_HairType	[extravalue+m_CharacterInfo.modeldescript.hair-1] );
			}
			else  // 모자를 썻을때는 머리카락 처리 안함. 
			{
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, NULL );
			}
		}
		else 
		{
			if( m_CharacterInfo.modeldescript.cap )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, NULL );
			else
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, g_UserModel_HairType	[extravalue] );
		}
		
		if( m_CharacterInfo.modeldescript.face )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACE, 		 g_UserModel_FaceType	[extravalue+m_CharacterInfo.modeldescript.face-1] );
		else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACE, 	 g_UserModel_FaceType	[extravalue] ); // 얼굴정보가 없으면 기본 출력
	}

	if( m_CharacterInfo.modeldescript.cap )
	{
		// 소림사는 소림 모자만 쓸수 있다.
		if( m_CharacterInfo.groupindex == _XGROUP_SORIM && m_CharacterInfo.gender == _XGENDER_MAN )
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.capCategory) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.capCategory);
				if( ClothesItem )
				{
					if( ClothesItem[m_CharacterInfo.modeldescript.cap].cClan == _XGROUP_SORIM )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +ClothesItem[m_CharacterInfo.modeldescript.cap].sModelIndex] );
					}
					else
					{
						if( ClothesItem[m_CharacterInfo.modeldescript.cap].cClan == _XGROUP_NANGIN &&
							ClothesItem[m_CharacterInfo.modeldescript.cap].sReqSim  == 1 )//소림 팬더모자 쓰게
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +ClothesItem[m_CharacterInfo.modeldescript.cap].sModelIndex] );
						else
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES )
			{
				if( g_ClothesItemProperty[m_CharacterInfo.modeldescript.cap].cClan == _XGROUP_SORIM )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +g_ClothesItemProperty[m_CharacterInfo.modeldescript.cap].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
				}
			}
			else if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES2 )
			{
				if( g_ClothesItemProperty2[m_CharacterInfo.modeldescript.cap].cClan == _XGROUP_SORIM )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +g_ClothesItemProperty2[m_CharacterInfo.modeldescript.cap].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
				}
			}			
			else if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES3 )
			{
				if( g_ClothesItemProperty3[m_CharacterInfo.modeldescript.cap].cClan == _XGROUP_SORIM )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, g_UserModel_Cap[extravalue +g_ClothesItemProperty3[m_CharacterInfo.modeldescript.cap].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
				}
			}
#endif
		}
		else
		{
			if( m_CharacterInfo.modeldescript.hair )
			{
				// 머리 색깔에 맞춰 적용
				int modelcolorindex = (m_CharacterInfo.modeldescript.hair-1) % 3;								 // 머리 색은 3타입이므로 3타입식 건너뛰고 머리색에 맞는 모델 선택
				
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.capCategory) )
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.capCategory);
					if( ClothesItem )
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + ClothesItem[m_CharacterInfo.modeldescript.cap].sModelIndex + modelcolorindex ] );
				}
#else
				if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty[m_CharacterInfo.modeldescript.cap].sModelIndex + modelcolorindex ] );
				else if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES2 )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty2[m_CharacterInfo.modeldescript.cap].sModelIndex + modelcolorindex ] );
				else if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES3 )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty3[m_CharacterInfo.modeldescript.cap].sModelIndex + modelcolorindex ] );
#endif
			}
			else // 머리카락 정보가 없을경우 ( 예외처리 )
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.capCategory) )
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.capCategory);
					if( ClothesItem )
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + ClothesItem[m_CharacterInfo.modeldescript.cap].sModelIndex ] );
				}
#else
				if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty[m_CharacterInfo.modeldescript.cap].sModelIndex ] );
				else if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES2 )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty2[m_CharacterInfo.modeldescript.cap].sModelIndex ] );
				else if( m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES3 )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		g_UserModel_Cap[extravalue + g_ClothesItemProperty3[m_CharacterInfo.modeldescript.cap].sModelIndex ] );
#endif
			}
		}
	}
	else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,		NULL );
			
	if( m_CharacterInfo.modeldescript.upperbody )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.upperbodyCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.upperbodyCategory);
			if( ClothesItem )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.upperbody].sModelIndex] );
		}
#else
		if( m_CharacterInfo.modeldescript.upperbodyCategory == _XGI_FC_CLOTHES )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.upperbody].sModelIndex] );
		else if( m_CharacterInfo.modeldescript.upperbodyCategory == _XGI_FC_CLOTHES2 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.upperbody].sModelIndex] );
		else if( m_CharacterInfo.modeldescript.upperbodyCategory == _XGI_FC_CLOTHES3 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,   g_UserModel_UBodyType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.upperbody].sModelIndex] );
#endif
	}
	else 
		m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,  g_UserModel_UBodyType	[extravalue] );
	
	if( m_CharacterInfo.modeldescript.lowerbody )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.lowerbodyCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.lowerbodyCategory);
			if( ClothesItem )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.lowerbody].sModelIndex] );
		}
#else
		if( m_CharacterInfo.modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lowerbody].sModelIndex] );
		else if( m_CharacterInfo.modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES2 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.lowerbody].sModelIndex] );		
		else if( m_CharacterInfo.modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES3 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,   g_UserModel_LBodyType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.lowerbody].sModelIndex] );		
#endif
	}
	else 
		m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,  g_UserModel_LBodyType	[extravalue] );
		
	if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
	{
		if( m_CharacterInfo.modeldescript.lhand )
		{
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );			
		}
		else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
		
		if( m_CharacterInfo.modeldescript.rhand )
		{
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );			
		}
		else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );		
	}
	else if(m_CharacterInfo.modeldescript.item1 > 0)
	{
		// 권각 아이템일경우 손은 렌더링 안함.
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				if( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
				{
					if( WeaponItem[ m_CharacterInfo.modeldescript.item1 ].cThirdType == _XGI_TC_WEAPON_FIST )
					{				
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
					}			
					else
					{					
						if( m_CharacterInfo.modeldescript.lhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+WeaponItem[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						
						if( m_CharacterInfo.modeldescript.rhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+WeaponItem[m_CharacterInfo.modeldescript.rhand].sModelIndex] );					
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}			
				else
				{
					
					if( m_CharacterInfo.modeldescript.lhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+WeaponItem[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );				
					
					if( m_CharacterInfo.modeldescript.rhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+WeaponItem[m_CharacterInfo.modeldescript.rhand].sModelIndex] );				
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );				
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty[ m_CharacterInfo.modeldescript.item1 ].cThirdType == _XGI_TC_WEAPON_FIST )
				{				
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{					
					if( m_CharacterInfo.modeldescript.lhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					
					if( m_CharacterInfo.modeldescript.rhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );					
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}			
			else
			{
				
				if( m_CharacterInfo.modeldescript.lhand )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
				}
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );				
				
				if( m_CharacterInfo.modeldescript.rhand )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );				
				}
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );				
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty2[ m_CharacterInfo.modeldescript.item1 ].cThirdType == _XGI_TC_WEAPON_FIST )
				{				
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( m_CharacterInfo.modeldescript.lhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					
					if( m_CharacterInfo.modeldescript.rhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );					
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}			
			else
			{
				if( m_CharacterInfo.modeldescript.lhand )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
				}
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );				
				
				if( m_CharacterInfo.modeldescript.rhand )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );				
				}
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty3[ m_CharacterInfo.modeldescript.item1 ].cThirdType == _XGI_TC_WEAPON_FIST )
				{				
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( m_CharacterInfo.modeldescript.lhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					
					if( m_CharacterInfo.modeldescript.rhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );					
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}			
			else
			{
				if( m_CharacterInfo.modeldescript.lhand )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
				}
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );				
				
				if( m_CharacterInfo.modeldescript.rhand )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );				
				}
				else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
			}
		}
#endif
	}	
	
	if( m_CharacterInfo.modeldescript.lfoot )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.lfootCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.lfootCategory);
			if( ClothesItem )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.lfoot].sModelIndex] );
		}
#else
		if( m_CharacterInfo.modeldescript.lfootCategory == _XGI_FC_CLOTHES )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lfoot].sModelIndex] );
		else if( m_CharacterInfo.modeldescript.lfootCategory == _XGI_FC_CLOTHES2 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.lfoot].sModelIndex] );		
		else if( m_CharacterInfo.modeldescript.lfootCategory == _XGI_FC_CLOTHES3 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.lfoot].sModelIndex] );		
#endif
	}
	else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		 g_UserModel_LFootType	[extravalue] );
	
	if( m_CharacterInfo.modeldescript.rfoot )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.rfootCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.rfootCategory);
			if( ClothesItem )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.rfoot].sModelIndex] );
		}
#else
		if( m_CharacterInfo.modeldescript.rfootCategory == _XGI_FC_CLOTHES )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rfoot].sModelIndex] );
		else if( m_CharacterInfo.modeldescript.rfootCategory == _XGI_FC_CLOTHES2 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.rfoot].sModelIndex] );		
		else if( m_CharacterInfo.modeldescript.rfootCategory == _XGI_FC_CLOTHES3 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.rfoot].sModelIndex] );		
#endif
	}
	else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		 g_UserModel_RFootType	[extravalue] );
	
	if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
	{
		if( m_CharacterInfo.modeldescript.adae )
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.adaeCategory) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.adaeCategory);
				if( ClothesItem )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.adae].sModelIndex] );					
				}
			}
#else
			if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES )
			{
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
			}
			else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
			{
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
			}			
			else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
			{
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
			}
#endif
		}
		else 
		{
			if( m_CharacterInfo.modeldescript.lhand )
			{
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );				
			}
			else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
			
			if( m_CharacterInfo.modeldescript.rhand )
			{
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );				
			}
			else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
		}
	}
	else
	{
		// 권각 아이템일경우 손 및 아대는 렌더링 안함.
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				if( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
				{
					if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
					}			
					else
					{
						if( m_CharacterInfo.modeldescript.adae )
						{
							if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.adaeCategory) )
							{
								_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.adaeCategory);
								if( ClothesItem )
								{
									m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.adae].sModelIndex] );
									m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+ClothesItem[m_CharacterInfo.modeldescript.adae].sModelIndex] );									
								}
							}
						}
						else 
						{
							if( m_CharacterInfo.modeldescript.lhand )
							{
								m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );							
							}
							else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
							
							if( m_CharacterInfo.modeldescript.rhand )
							{
								m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );							
							}
							else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
						}
					}
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( m_CharacterInfo.modeldescript.adae )
					{
						if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}
						else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}						
						else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}
					}
					else 
					{
						if( m_CharacterInfo.modeldescript.lhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );							
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( m_CharacterInfo.modeldescript.rhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );							
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}
			}
			else
			{
				if( m_CharacterInfo.modeldescript.adae )
				{
					if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}
					else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}
					else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}
				}
				else 
				{
					if( m_CharacterInfo.modeldescript.lhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );						
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					if( m_CharacterInfo.modeldescript.rhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );						
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( m_CharacterInfo.modeldescript.adae )
					{
						if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}
						else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}						
						else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}						
					}
					else 
					{
						if( m_CharacterInfo.modeldescript.lhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );							
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( m_CharacterInfo.modeldescript.rhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );							
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}
			}
			else
			{
				if( m_CharacterInfo.modeldescript.adae )
				{
					if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}
					else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}					
					else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}					
				}
				else 
				{
					if( m_CharacterInfo.modeldescript.lhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );						
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					if( m_CharacterInfo.modeldescript.rhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );						
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType  == _XGI_SC_WEAPON_SHORT	)				// 단병기
			{
				if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      NULL );
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      NULL );
				}			
				else
				{
					if( m_CharacterInfo.modeldescript.adae )
					{
						if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}
						else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}						
						else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						}						
					}
					else 
					{
						if( m_CharacterInfo.modeldescript.lhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );							
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
						
						if( m_CharacterInfo.modeldescript.rhand )
						{
							m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );							
						}
						else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
					}
				}
			}
			else
			{
				if( m_CharacterInfo.modeldescript.adae )
				{
					if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}
					else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES2 )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}					
					else if( m_CharacterInfo.modeldescript.adaeCategory == _XGI_FC_CLOTHES3 )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.adae].sModelIndex] );
					}					
				}
				else 
				{
					if( m_CharacterInfo.modeldescript.lhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,       g_UserModel_LHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.lhand].sModelIndex] );						
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND,      g_UserModel_LHandType	[extravalue] );
					
					if( m_CharacterInfo.modeldescript.rhand )
					{
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,       g_UserModel_RHandType	[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.rhand].sModelIndex] );						
					}
					else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND,      g_UserModel_RHandType	[extravalue] );
				}
			}
		}
#endif
	}

	if( m_CharacterInfo.modeldescript.backpack )
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.backpackCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.backpackCategory);
			if( ClothesItem )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+ClothesItem[m_CharacterInfo.modeldescript.backpack].sModelIndex] );
		}
#else
		if( m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty[m_CharacterInfo.modeldescript.backpack].sModelIndex] );
		else if( m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES2 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.backpack].sModelIndex] );
		else if( m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES3 )
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 g_UserModel_BackPack[extravalue+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.backpack].sModelIndex] );
#endif
	}
	else m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	 NULL );
	
	if( m_CharacterInfo.modeldescript.item1 )
	{
		if( m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_START  || // 비무 홍보중일때는 아이템 표시 안함.
			m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_REPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_END )
		{
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				
#ifdef _XDEF_MODELTHREADLOADING
				if( WeaponItem && WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,	 g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
				else
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,	 NULL );
#else
				if( WeaponItem )
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,	 g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,	 g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,	 g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,	 g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
		}
	}
	else
	{
		m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		 NULL );		
	}
	if( m_CharacterInfo.modeldescript.mask )
	{
		int genderextravalue = 0;
		int genderextravalue2 = 0;
		if( m_CharacterInfo.gender == _XGENDER_WOMAN ) 
		{
			genderextravalue = g_MaskModel_GenderSeparateIndex;
		}

#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_CLOTHES == GetEqualItemType(m_CharacterInfo.modeldescript.maskCategory) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.maskCategory);
			if( ClothesItem )
			{
				if( m_CharacterInfo.gender == _XGENDER_WOMAN ) 
				{
					// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
					if( ClothesItem[m_CharacterInfo.modeldescript.mask].sModelIndex >= 140 ) 
					{
						// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
						genderextravalue = 0;
						genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
					}
				}

				m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK, g_UserModel_Mask[genderextravalue+genderextravalue2+ClothesItem[m_CharacterInfo.modeldescript.mask].sModelIndex] );
			}
		}
#else
		if( m_CharacterInfo.modeldescript.maskCategory == _XGI_FC_CLOTHES )
		{
			if( m_CharacterInfo.gender == _XGENDER_WOMAN ) 
			{
				// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
				if( g_ClothesItemProperty[m_CharacterInfo.modeldescript.mask].sModelIndex >= 140 ) 
				{
					// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
					genderextravalue = 0;
					genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
				}
			}
			
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK, g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty[m_CharacterInfo.modeldescript.mask].sModelIndex] );
		}
		else if( m_CharacterInfo.modeldescript.maskCategory == _XGI_FC_CLOTHES2 )
		{
			if( m_CharacterInfo.gender == _XGENDER_WOMAN ) 
			{
				// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
				if( g_ClothesItemProperty2[m_CharacterInfo.modeldescript.mask].sModelIndex >= 140 ) 
				{
					// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
					genderextravalue = 0;
					genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
				}
			}

			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK, g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty2[m_CharacterInfo.modeldescript.mask].sModelIndex] );
		}
		else if( m_CharacterInfo.modeldescript.maskCategory == _XGI_FC_CLOTHES3 )
		{
			if( m_CharacterInfo.gender == _XGENDER_WOMAN ) 
			{
				// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
				if( g_ClothesItemProperty3[m_CharacterInfo.modeldescript.mask].sModelIndex >= 140 ) 
				{
					// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
					genderextravalue = 0;
					genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
				}
			}

			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK, g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty3[m_CharacterInfo.modeldescript.mask].sModelIndex] );
		}
#endif

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
		if(m_CharacterInfo.m_nClothView == 1)	// 일반 의상 보기 체크
		{
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_MASK, NULL);
		}
		else	// 치장 의상 보기 체크
#endif
		{
			// 치장의상 착용시 다른 아이템들은 보이지 않게 한다.
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND, NULL );
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK, NULL );
		}

	}
	else
	{
		m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK,		 NULL );		
	}
	
	if( m_bFaceMaskMode )
	{
		int extravalue = 0;
		if( m_CharacterInfo.gender == _XGENDER_WOMAN ) extravalue = g_FaceMaskModel_GenderSeparateIndex;
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACEMASK, g_UserModel_FaceMask[extravalue] );		
	}
	else 
		m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACEMASK, NULL );
	
	m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM2, NULL );
	m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM3, NULL );
	m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM4, NULL );
//	m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM5, NULL );
	
	if(!dntchangemodel)
		SetAnimation(  m_CharacterInfo.animationdescript.motionclass  );
}

//------------------------------------------------------------------------------------------
// Operation functions
BOOL _XUser::CheckIntersectMouseRay( void )
{
	D3DXVECTOR3 a,b,c;	
	int pos = 0;

	for(int i = 0; i < 8; ++i )
	{
		a = m_Position + g_UserIntersectionPoly[ g_CommonIntersectionIndices[pos  ] ];
		b = m_Position + g_UserIntersectionPoly[ g_CommonIntersectionIndices[pos+1] ];
		c = m_Position + g_UserIntersectionPoly[ g_CommonIntersectionIndices[pos+2] ];

		if( IntersectTriangle( a, b, c ) )
		{
			return TRUE;
		}

		pos+=3;
	}	

	return FALSE;
}

void _XUser::Process( void )
{
	int eventlevel = 0; // Effect의 종류를 결정함.
	int seperator = 0;
	
	if( m_AttackResult.sSkillID > 0 )
	{
		seperator = 4;
	}
	
	switch(m_AttackResult.skillType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{	
			short motionindex = -1;
			if(m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;
			
			if( m_ModelDescriptor.GetCurMotion() == (motionindex + _XANI_ORDER_FIRE) )
			{
/*				int iteratevalue = 0;
				// 임시 처리
				int casting = g_SkillProperty[m_AttackResult.sSkillID]->detailInfo[m_AttackResult.cSkillLevel].casting;
				if( casting )
					iteratevalue = m_AttackResult.dwChargeTime / casting;
				
				if( iteratevalue >= 0 && iteratevalue < 3 ) eventlevel = 0;
				else if( iteratevalue >= 3 && iteratevalue < 6 ) eventlevel = 1;
				else if( iteratevalue >= 6 && iteratevalue <= 11 ) eventlevel = 2;*/

				char skilllevel = m_AttackResult.cSkillLevel;
				if( skilllevel >= 0 && skilllevel < 3 ) eventlevel = 0;
				else if( skilllevel >= 3 && skilllevel < 6 ) eventlevel = 1;
				else if( skilllevel >= 6 && skilllevel <= 11 ) eventlevel = 2;
			}
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{				
			short motionindex = -1;
			if(m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;
			
			eventlevel = 0;

			/*
			if( m_ModelDescriptor.GetCurMotion() == (motionindex+_XANI_ORDER_REPEAT) )
			{			
				char skilllevel = m_AttackResult.cSkillLevel;
				if( skilllevel >= 0 && skilllevel < 3 ) eventlevel = 0;
				else if( skilllevel >= 3 && skilllevel < 6 ) eventlevel = 1;
				else if( skilllevel >= 6 && skilllevel <= 11 ) eventlevel = 2;			
			}
			*/
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			short motionindex = -1;
			if(m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;
			
			eventlevel = 0;

			/*
			if( m_ModelDescriptor.GetCurMotion() == motionindex )
			{
				char skilllevel = m_AttackResult.cSkillLevel;
				if( skilllevel >= 0 && skilllevel < 3 ) eventlevel = 0;
				else if( skilllevel >= 3 && skilllevel < 6 ) eventlevel = 1;
				else if( skilllevel >= 6 && skilllevel <= 11 ) eventlevel = 2;
			}
			*/
		}
		break;
	case _XSI_USEFULTYPE_HIDDENMA :
		{				
			short motionindex = -1;
			if(m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;
			
			eventlevel = 0;
			/*
			if( m_ModelDescriptor.GetCurMotion() == (motionindex+_XANI_ORDER_REPEAT) )
			{
				char skilllevel = m_AttackResult.cSkillLevel;
				if( skilllevel >= 0 && skilllevel < 3 ) eventlevel = 0;
				else if( skilllevel >= 3 && skilllevel < 6 ) eventlevel = 1;
				else if( skilllevel >= 6 && skilllevel <= 11 ) eventlevel = 2;
			}
			*/
		}
		break;
	}

	bool soundplay = true;
		
	if( m_AttackResult.sSkillID )
	{
		// 스킬의 적용 반경이 정해진 경우 이펙트의 적용 최대 반경을 제한한다.
		if( g_SkillProperty[m_AttackResult.sSkillID]->radius > 0.0f )
		{
			m_ModelDescriptor.Advance( soundplay, eventlevel, g_SkillProperty[m_AttackResult.sSkillID]->radius * 2.0f );
		}
		else
		{
			m_ModelDescriptor.Advance(soundplay, eventlevel);
		}
	}
	else
	{
		m_ModelDescriptor.Advance(soundplay, eventlevel);
	}


	///////////////////////////////////////////////////////////////////////
	// Apply gravity
	_XOBBData*	pCollideOBB = NULL;
	FLOAT terrainheight = g_LodTerrain.GetTerrainAndRideOBBHeight( m_Position, pCollideOBB );
	
	if( fabs(m_Position.y - terrainheight) > EPSILON3 )
	{
		if( m_Position.y < terrainheight && (terrainheight - m_Position.y) < _XDEF_RIDEOBBLIMITHEIGHT )
		{
			m_Position.y = terrainheight;
			
			if( m_Velocity.y > 0.0f )
			{
				m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;
				
				FLOAT Length = _XFC_sqrt(m_Velocity.y * m_Velocity.y);
				if ( Length > _XDEF_MAXJUMPVELOCITY )
				{
					m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
				}
				
				m_Position.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;
				
				if( terrainheight+_XDEF_MAXJUMPHEIGHT < m_Position.y )  // 특정 높이 이상 점프 불가 
				{
					m_Position.y = terrainheight+_XDEF_MAXJUMPHEIGHT;
					m_Velocity.y = 0.0f;
				}
				
				D3DXVECTOR3 vecDec = -m_Velocity;
				D3DXVec3Normalize( &vecDec, &vecDec );
				
				Length = D3DXVec3Length( &m_Velocity );
				FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
				if ( Dec > Length ) Dec = Length;					
				m_Velocity += vecDec * Dec;					
				if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
			}
			
			UpdatePosition();			
		}
		else if( m_Position.y > terrainheight )
		{
			if( m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG )
			{
				m_Position.y += (_XDEF_CHARACTERGRAVITY * 4.0f) * g_fElapsedFrameMilisecondTime;
			}
			else
			{
				m_Position.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;
			}
			
			if( m_Velocity.y > 0.0f )
			{
				m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;
				
				FLOAT Length = _XFC_sqrt(m_Velocity.y * m_Velocity.y);
				if ( Length > _XDEF_MAXJUMPVELOCITY )
				{
					m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
				}
				
				m_Position.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;
				
				if( terrainheight+_XDEF_MAXJUMPHEIGHT < m_Position.y ) // 특정 높이 이상 점프 불가 
				{
					m_Position.y = terrainheight+_XDEF_MAXJUMPHEIGHT;
					m_Velocity.y = 0.0f;
				}
				
				D3DXVECTOR3 vecDec = -m_Velocity;
				D3DXVec3Normalize( &vecDec, &vecDec );
				
				Length = D3DXVec3Length( &m_Velocity );
				FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
				if ( Dec > Length ) Dec = Length;
				m_Velocity += vecDec * Dec;					
				if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
			}
			
			if( m_Position.y < terrainheight ) m_Position.y = terrainheight;
			UpdatePosition();
		}
	}
	else
	{
		m_Velocity.y = 0.0f;
	}

	D3DXVECTOR3 nextpos;
	if( !GetNextMove( nextpos ) )
	{	
		// Packet으로 온다, 다시 처리할 필요없음
	}
	else
	{
		m_Position = nextpos;
		UpdatePosition();
	}
	
	if(m_CharacterInfo.animationdescript.motionclass == _XACTION_IDLE)
	{		
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();

		int curmotion = m_ModelDescriptor.GetCurMotion();
		if(curmotion == _XAN_EMOTION_SITDOWNEND)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				SetAnimation(_XACTION_IDLE);
		}
		else if( curmotion >= _XAN_EMOTION_BOW && curmotion <=_XAN_EMOTION_RUSH)
		{
			if(curmotion == _XAN_EMOTION_BOWPOLITELY)
			{
				if(m_CurrentWeaponID <= 0)
					DeleteWeaponModel();
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(curmotion == _XAN_EMOTION_BOWPOLITELY)
				{
					if(m_CurrentWeaponID > 0)
						ReloadWeaponModel();
				}
				SetAnimation(_XACTION_IDLE);
			}
		}
		else if( curmotion >= _XAN_EMOTION_CHEER && curmotion <=_XAN_EMOTION_DOZ)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				SetAnimation(_XACTION_IDLE);
		}
		else if( curmotion >= _XAN_EMOTION_MARRYCLAPPINGHAND && curmotion <=_XAN_EMOTION_MARRYBOWPOLITELY)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_CurrentWeaponID > 0)
					ReloadWeaponModel();
				SetAnimation(_XACTION_IDLE);
			}
		}
		else if(curmotion == _XAN_MATCH_PR_END)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimation(_XACTION_IDLE);
			}
		}
		else if(curmotion == _XAN_MATCH_WINNER_REPEAT || curmotion == _XAN_MATCH_LOSER_REPEAT)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimation(_XACTION_IDLE);
			}
		}
	
		if(m_ModelDescriptor.m_LockEndFrame) // && !bLockEndFrame)
			m_ModelDescriptor.SetLockEndFrame(FALSE);
	}
	else if( m_CharacterInfo.animationdescript.motionclass == _XACTION_MOVE )
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();

		if(m_ModelDescriptor.m_LockEndFrame)
			m_ModelDescriptor.SetLockEndFrame(FALSE);

		if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XAN_MOVE_NONE_START		 :
				case _XAN_MOVE_SHORTWEAPON_START :
				case _XAN_MOVE_LONGWEAPON_START  :
				case _XAN_MOVE_SOFTWEAPON_START  :
					{
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate
					}
					break;
					
				case _XAN_MOVE_NONE_REPEAT		 :
				case _XAN_MOVE_SHORTWEAPON_REPEAT:
				case _XAN_MOVE_LONGWEAPON_REPEAT :
				case _XAN_MOVE_SOFTWEAPON_REPEAT :
				case _XAN_MOVE_SWORD_REPEAT_PEACE :
				case _XAN_MOVE_SWORD_REPEAT_BATTLE :
					{						
					}
					break;
					
				case _XAN_MOVE_NONE_END			 :
				case _XAN_MOVE_SHORTWEAPON_END	 :
				case _XAN_MOVE_LONGWEAPON_END	 :
				case _XAN_MOVE_SOFTWEAPON_END	 :
					{
//						_XDWINPRINT("[XUser/Process - Move]");
						SetAnimation(_XACTION_IDLE);
					}
					break;
				}
			}
		}
		else if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XAN_FASTMOVE_NONE_START		 :
				case _XAN_FASTMOVE_SHORTWEAPON_START :
				case _XAN_FASTMOVE_LONGWEAPON_START  :
				case _XAN_FASTMOVE_SOFTWEAPON_START  :
				case _XAN_FASTMOVE_SWORD_START_PEACE :
				case _XAN_FASTMOVE_SWORD_START_BATTLE :
					{
						SetMoveSpeed( _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1 );
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1,FALSE);// Repeate
					}
				break;
				/*
				case _XAN_FASTMOVE_NONE_REPEAT		 :
				case _XAN_FASTMOVE_SHORTWEAPON_REPEAT:
				case _XAN_FASTMOVE_LONGWEAPON_REPEAT :
				case _XAN_FASTMOVE_SOFTWEAPON_REPEAT :
					{
					}
				break;*/

				case _XAN_FASTMOVE_NONE_END			 :
				case _XAN_FASTMOVE_SHORTWEAPON_END	 :
				case _XAN_FASTMOVE_LONGWEAPON_END	 :
				case _XAN_FASTMOVE_SOFTWEAPON_END	 :
				case _XAN_FASTMOVE_SWORD_END_PEACE	 :
				case _XAN_FASTMOVE_SWORD_END_BATTLE	 :
					{
//						_XDWINPRINT("[XUser/Process - ]");
						SetMoveSpeed( _XDEF_CHARACTERMOVESPEED_WALK );
						SetAnimation(_XACTION_IDLE/*, FALSE*/);
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						m_TargetPosition = m_Position;
					}
				break;
				}
			}
		}
	}
	else if(m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK)
	{
		if(m_AttackTarget)
		{
			if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XMob*)m_AttackTarget)->m_Position);
			}
			else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
			{
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XUser*)m_AttackTarget)->m_Position);
			}
			UpdatePosition();
		}

		// battle
		switch(m_AttackResult.skillType)
		{
		case _XSI_USEFULTYPE_ACTIVESKILL :
			ProcessBattleActiveSkill();
			break;
		case _XSI_USEFULTYPE_CASTINGSPELL :
			ProcessBattleCastingSpell();
			break;
		case _XSI_USEFULTYPE_CHARGESKILL :
			ProcessBattleChargeSkill();
			break;
		case _XSI_USEFULTYPE_CHANNELINGSPELL :
			{
				if( g_SkillProperty[m_AttackResult.sSkillID]->skillSType == _XSI_STYPE_03 )	// 경공 처리
				{
				}
				else
				{
					ProcessBattleChannelingSpell();
				}
/*				else if(g_SkillProperty[m_AttackResult.sSkillID]->targetType != _XSI_TARGETTYPE_SELF)
					ProcessBattleTargetChannelingSpell();
				else
				{
					// self이면서 경공이나 내공심법이 아닌경우 처리 필요
					ProcessBattleSelfChannelingSpell();
				}*/
			}
			break;
		case _XSI_USEFULTYPE_FINISHSKILL :
			ProcessBattleFinishSkill();
			break;
		case _XSI_USEFULTYPE_ROUNDINGSPELL :
			ProcessBattleRoundingSpell();
			break;
		case _XSI_USEFULTYPE_SMASHINGSKILL :
			ProcessBattleSmashingSkill();
			break;
		case _XSI_USEFULTYPE_HIDDENMA :
			ProcessBattleHiddenMA();
			break;
		case _XSI_USEFULTYPE_ACTIONSKILL :
			ProcessBattleActionSkill();
			break;
		}
	}
	else if(m_CharacterInfo.animationdescript.motionclass == _XACTION_DYING)
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();
		
		if(m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_NONE ||
			m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_SHORTWEAPON ||
			m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_LONGWEAPON
			 || m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_SWORD_PEACE
			 || m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_SWORD_BATTLE
			)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				// 뒤로 밀어주기 시작
				_XMob* pMob = (_XMob*)m_AttackTarget;
				if(pMob)
				{
					D3DXVECTOR3 viewdirection;
					viewdirection.x = pMob->m_Position.x - m_Position.x;
					viewdirection.y = 1.0f;
					viewdirection.z = pMob->m_Position.z - m_Position.z;
					
					m_TargetPosition.x = m_Position.x + viewdirection.x*(-1.0f);
					m_TargetPosition.z = m_Position.z + viewdirection.z*(-1.0f);
					
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
					ResetAttackTarget();
				}
			}
			m_ModelDescriptor.SetLockEndFrame(TRUE);
		
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetLockEndFrame(FALSE);
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XAN_DYINGREADY_NONE :
					{
						SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
					}
					break;
				case _XAN_DYINGREADY_SHORTWEAPON :
					{
						SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
					}
					break;
				case _XAN_DYINGREADY_LONGWEAPON :
					{
						SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
					}
					break;
				case _XAN_DYINGREADY_SWORD_PEACE :
					{
						SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
					}
					break;
				case _XAN_DYINGREADY_SWORD_BATTLE :
					{
						SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
					}
					break;
				}
			}
		}
	}
	else if(m_CharacterInfo.animationdescript.motionclass == _XACTION_DIE)
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			int rand = _XRandom() % 2;
			
			if( m_CharacterInfo.gender == _XGENDER_MAN )
				_XPlaySoundEffect( g_DieEffectSoundIndex[0][rand] , m_Position );
			else 
				_XPlaySoundEffect( g_DieEffectSoundIndex[1][rand] , m_Position );
		}

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(TRUE);
		}
	}
	else if(GetMotionClass() == _XACTION_RESCUE)
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();
		
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
//			_XDWINPRINT("[XUser/Process - Rescue]");
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_REBIRTH)
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
//			_XDWINPRINT("[XUser/Process - Rebirth]");
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_DEFENCE)
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
//			_XDWINPRINT("[XUser/Process - Defence]");
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_BLOW)
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
//			_XDWINPRINT("[XUser/Process - Blow]");
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_MATCH_START)
	{
		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();
		
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_MATCH_CANCEL)
	{
		if(this == (_XUser*)g_LastPickedObject) 
			g_LastPickedObject = NULL;
		if(this == (_XUser*)g_pLocalUser->m_AttackTarget)
			g_pLocalUser->ResetAttackTarget();
		if(this == (_XUser*)g_pLocalUser->m_ViewTarget)
			g_pLocalUser->ResetViewTarget();

		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();
		
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}
#ifdef _XTS_NEWINSENSIBLE
	else if(GetMotionClass() == _XACTION_INSENSIBLE)
	{
		//기절 상태일 때 타겟 제거하지 않음 //Author : 박소현
/*		if(this == (_XUser*)g_LastPickedObject) 
			g_LastPickedObject = NULL;
		if(this == (_XUser*)g_pLocalUser->m_AttackTarget)
			g_pLocalUser->ResetAttackTarget();
		if(this == (_XUser*)g_pLocalUser->m_ViewTarget)
			g_pLocalUser->ResetViewTarget();*/

		// display 되지 않은 전투 결과 처리
		this->ProcessMobDie();
	}
#endif

	if( g_ZeroLevelMode )		// hotblood->zerolevel
		ProcessBattleActiveSkill();
}

void _XUser::ProcessBattleActiveSkill(void)
{
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
	{
		if(m_AttackResult.ucToType == en_target_type_monster)
		{
			_XMob* pMob = NULL;
			if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				if(((_XMob*)m_AttackTarget)->m_MobID == m_AttackResult.usDefenderID)
					pMob = (_XMob*)m_AttackTarget;
			}
			if(!pMob)
				pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);

			if(!pMob)
				return;
			
			if(pMob->m_Action.action == _XMOBACTION_DIE)
				return;

			// 타격 이펙트 삽입
			D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
			
			BOOL bIntersect = FALSE;
			D3DXVECTOR3 pickedpoint, originpos;
			D3DXVECTOR3 direction = pMob->m_Position - m_Position;
			D3DXVec3Normalize(&direction, &direction);
			originpos = m_Position;
			originpos.y = m_Position.y  + 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ); //+ 0.5f;
			
			bIntersect = pMob->m_ModelDescriptor.InterOBBvsRay(originpos, direction, pMob->m_Position, pMob->m_RotateAngle, &pickedpoint);
			
			if(!bIntersect)
			{
				offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
			}
			else
			{
				offsetmat._41 = pickedpoint.x;
				offsetmat._42 = pickedpoint.y;
				offsetmat._43 = pickedpoint.z;
			}

			for(int i = 0 ; i < 3 ; ++i)
			{
				if(m_AttackResult.display[i])
				{
					switch(m_AttackResult.usAtkInfo[i])
					{
					case 0 :
					case 5 :
						{
							// Hit - 상극일반
							pMob->m_AttackTarget = this;
							pMob->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

							pMob->SetBlowMotion();
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
									
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical - 상극크리
							pMob->m_AttackTarget = this;
							pMob->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

							pMob->SetBlowMotion(TRUE);
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 2 :
						{
							// Miss
						}
						break;
					}
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

						if(pMob == (_XMob*)g_LastPickedObject)
							g_LastPickedObject = NULL;
					}
					m_AttackResult.display[i] = FALSE;
					break;
				}
			}
		} // target_type_monster
		else if(m_AttackResult.ucToType == en_target_type_object)
		{
			_XFunctionalObject* pFunctionalObject = NULL;
			
			if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
			{
				if(((_XFunctionalObject*)m_AttackTarget)->m_AttackalbeTypeUniqueID == m_AttackResult.usDefenderID)
					pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
			}
			
			if(!pFunctionalObject)
				pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
			
			if(!pFunctionalObject)
				return;
			
			// 타격 이펙트 삽입
			D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
			
			BOOL bIntersect = FALSE;
			D3DXVECTOR3 pickedpoint;
			bIntersect = g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[pFunctionalObject->m_UniqueID].InterOBBvsRay(g_vPickRayOrig, g_vPickInfinityRayDir, pickedpoint );
			
			if(!bIntersect)
			{
				offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
			}
			else
			{
				offsetmat._41 = pickedpoint.x;
				offsetmat._42 = pickedpoint.y;
				offsetmat._43 = pickedpoint.z;
			}

			for(int i = 0 ; i < 3 ; ++i)
			{
				if(m_AttackResult.display[i])
				{
					switch(m_AttackResult.usAtkInfo[i])
					{
					case 0 :
					case 5 :
						{
							// Hit - 상극일반
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical - 상극크리
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 2 :
						{
							// Miss
						}
						break;
					}
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pFunctionalObject->m_HealthPercentage = 0;
						pFunctionalObject->SetDieAnimation();
					}
					m_AttackResult.display[i] = FALSE;
					break;
				}
			}

		}//en_target_type_object
		else if(m_AttackResult.ucToType == en_target_type_others)
		{
			if(m_AttackResult.usDefenderID == g_pLocalUser->m_Uniqueid)
			{
				// 타격 이펙트 삽입
				D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
				
				BOOL bIntersect = FALSE;
				D3DXVECTOR3 pickedpoint, originpos;
				D3DXVECTOR3 direction = g_pLocalUser->m_Position - m_Position;
				D3DXVec3Normalize(&direction, &direction);
				originpos = m_Position;
				originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f );
				
				bIntersect = g_pLocalUser->m_ModelDescriptor.InterOBBvsRay(originpos, direction, g_pLocalUser->m_Position, g_pLocalUser->m_RotateAngle, &pickedpoint);
				
				if(!bIntersect)
				{
					offsetmat._42 += 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f );
				}
				else
				{
					offsetmat._41 = pickedpoint.x;
					offsetmat._42 = pickedpoint.y;
					offsetmat._43 = pickedpoint.z;
				}
				
				for(int i = 0 ; i < 3 ; ++i)
				{
					if(m_AttackResult.display[i])
					{
						switch(m_AttackResult.usAtkInfo[i])
						{
						case 0 :
						case 5 :
							{
								// Hit - 상극일반
//								g_pLocalUser->SetBlowMotion();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];

									if(m_AttackResult.usAtkInfo[i] == 0)
										_snprintf(messagestring, sizeof(messagestring), _T("Active HIT damage[%d]"), m_AttackResult.sDamage[i]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Active 상극일반 damage[%d]"), m_AttackResult.sDamage[i]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical - 상극크리
//								g_pLocalUser->SetBlowMotion();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									if(m_AttackResult.usAtkInfo[i] == 1)
										_snprintf(messagestring, sizeof(messagestring), _T("Active CRITICAL damage[%d]"), m_AttackResult.sDamage[i]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Active 상극크리 damage [%d]"), m_AttackResult.sDamage[i]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 2 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Miss
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active MISS"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 3 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Block
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active BLOCK"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 4 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Shock
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active SHOCK damage [%d]"), m_AttackResult.sDamage[i]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
//							g_pLocalUser->SetAnimation(_XACTION_MATCH_CANCEL);
//							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
						m_AttackResult.display[i] = FALSE;
						break;
					}
				}
			} // local user
			else
			{
				_XUser* pUser = NULL;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					if(((_XUser*)m_AttackTarget)->m_Uniqueid == m_AttackResult.usDefenderID)
						pUser = (_XUser*)m_AttackTarget;
				}
				if(!pUser)
					pUser = (_XUser*)g_Userlist.FindUser(m_AttackResult.usDefenderID);

				if(!pUser)
					return;

				if(pUser->GetMotionClass() == _XACTION_DIE)
					return;

				// 타격 이펙트 삽입
				D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;

				BOOL bIntersect = FALSE;
				D3DXVECTOR3 pickedpoint, originpos;
				D3DXVECTOR3 direction = pUser->m_Position - m_Position;
				D3DXVec3Normalize(&direction, &direction);
				originpos = m_Position;
				originpos.y = m_Position.y  + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f );

				bIntersect = pUser->m_ModelDescriptor.InterOBBvsRay(originpos, direction, pUser->m_Position, pUser->m_RotateAngle, &pickedpoint);
				
				if(!bIntersect)
				{
					offsetmat._42 += 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f );
				}
				else
				{
					offsetmat._41 = pickedpoint.x;
					offsetmat._42 = pickedpoint.y;
					offsetmat._43 = pickedpoint.z;
				}

				for(int i = 0 ; i < 3 ; ++i)
				{
					if(m_AttackResult.display[i])
					{
						switch(m_AttackResult.usAtkInfo[i])
						{
						case 0 :
						case 5 :
							{
								// Hit - 상극일반
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical - 상극크리
								pUser->SetBlowMotion();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 2 :
							{
								// Miss
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							if(pUser == (_XUser*)g_LastPickedObject)
								g_LastPickedObject = NULL;
						}
						m_AttackResult.display[i] = FALSE;
						break;
					}
				}
			} // other user
		} // target_type_others
		else if(m_AttackResult.ucToType == en_target_type_dummy)
		{
			_XDummy* pDummy = NULL;
			if(m_AttackTargetObjectType == _XPICKINGTARGET_DUMMY)
			{
				if(((_XDummy*)m_AttackTarget)->m_DummyID == m_AttackResult.usDefenderID)
					pDummy = (_XDummy*)m_AttackTarget;
			}
			if(!pDummy)
				pDummy = (_XDummy*)g_DummyList.Find(m_AttackResult.usDefenderID);

			if(!pDummy)
				return;

			D3DXMATRIX offsetmat = pDummy->m_ModelDescriptor.m_Position;
			offsetmat._42 += (0.15f + ((FLOAT)m_ModelDescriptor.m_EffectPosition+2)/3.0f);

			if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
			{
				PlayAttackSound();
				if( pDummy->m_ModelDescriptor.m_EffectManager )
				{
					pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
				}
			}
		} // target_type_dummy
	} // command frame
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
	{
		if(m_AttackResult.ucToType == en_target_type_monster)
		{
//			ProcessMobDie();
			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
		else if(m_AttackResult.ucToType == en_target_type_object)
		{
			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
		else if(m_AttackResult.ucToType == en_target_type_dummy)
		{
			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
		else if(m_AttackResult.ucToType == en_target_type_others)
		{
			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XUser::PlayAttackSound( void )
{
	int randhumanindex = _XRandom() % 6;
	if( randhumanindex < 3 )
		_XPlaySoundEffect(g_DefenceEffectSoundIndex[m_CharacterInfo.gender][randhumanindex], m_Position, true);
	
	if( m_ModelDescriptor.m_WeaponClass ) // 메인무기라면...
	{
		if( m_CharacterInfo.modeldescript.item1 == 0 )
		{
			_XPlaySoundEffect( g_WeaponSoundIndexList[ 4 ], m_Position );
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					int weaponmaterial = WeaponItem[m_CharacterInfo.modeldescript.item1].ucQuality2;
					if( weaponmaterial < g_WeaponSoundCount )
					{
						_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				int weaponmaterial = g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].ucQuality2;
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				int weaponmaterial = g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].ucQuality2;
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				int weaponmaterial = g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].ucQuality2;
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
#endif
		}
	}
	else // 발차기라면...
	{
		_XPlaySoundEffect( g_WeaponSoundIndexList[ 4 ], m_Position );
	}
}

void _XUser::PlayCriticalAttackSound( void )
{
	int randhumanindex = _XRandom() % 3;
	if( randhumanindex < 3 )
		_XPlaySoundEffect(g_DefenceEffectSoundIndex[m_CharacterInfo.gender][randhumanindex], m_Position);
	
	if( m_ModelDescriptor.m_WeaponClass ) // 메인무기라면...
	{
		if( m_CharacterInfo.modeldescript.item1 == 0 )
		{
			_XPlaySoundEffect( g_CriticalEffectSoundIndex[0], m_Position );
		}
		else
		{
			_XPlaySoundEffect( g_CriticalEffectSoundIndex[1], m_Position );
		}
	}
	else // 발차기라면...
	{
		_XPlaySoundEffect( g_CriticalEffectSoundIndex[0], m_Position );
	}
}

void _XUser::ProcessBattleChargeSkill(void)
{
	short motionindex = -1;
	if(m_CharacterInfo.gender == _XGENDER_MAN)
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
	else
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;

//	_XDWINPRINT("[XUSER/ProcessChargeSkill] motionindex [%d/%d]  Frame [%f]", m_ModelDescriptor.GetCurMotion(), motionindex, m_ModelDescriptor.GetCurFrame());
	
	if(m_ModelDescriptor.GetCurMotion() == motionindex)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
	{
		if(m_AttackResult.display[0])
		{
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
		{
			if(m_AttackResult.ucToType == en_target_type_monster)
			{
				_XMob* pMob = NULL;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					if(((_XMob*)m_AttackTarget)->m_MobID == m_AttackResult.usDefenderID)
						pMob = (_XMob*)m_AttackTarget;
				}
				if(!pMob)
					pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);

				if(!pMob)
					return;

				D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
				offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );

				if(m_AttackResult.display[0])
				{
					switch(m_AttackResult.usAtkInfo[0])
					{
					case 0 :
					case 5 :
						{
							// Hit
							pMob->SetBlowMotion();
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical
							pMob->SetBlowMotion();
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 2 :
						{
							// Miss
						}
						break;
					}
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
					}
					m_AttackResult.display[0] = FALSE;
				} 
			} // target_type_monster
			else if(m_AttackResult.ucToType == en_target_type_object)
			{
				_XFunctionalObject* pFunctionalObject = NULL;
				
				if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{
					if(((_XFunctionalObject*)m_AttackTarget)->m_AttackalbeTypeUniqueID == m_AttackResult.usDefenderID)
						pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
				}
				
				if(!pFunctionalObject)
					pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
				
				if(!pFunctionalObject)
					return;
				
				D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
				offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
				
				if(m_AttackResult.display[0])
				{
					switch(m_AttackResult.usAtkInfo[0])
					{
					case 0 :
					case 5 :
						{
							// Hit
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								PlayAttackSound();
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 2 :
						{
							// Miss
						}
						break;
					}
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pFunctionalObject->m_HealthPercentage = 0;
						pFunctionalObject->SetDieAnimation();
					}
					m_AttackResult.display[0] = FALSE;
				} 
			}//en_target_type_object
			else if(m_AttackResult.ucToType == en_target_type_others)
			{
				if(m_AttackResult.usDefenderID == g_pLocalUser->m_Uniqueid)
				{
					D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
					offsetmat._42 += 0.98f;
					
					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
						case 5 :
							{
								// Hit
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									if(m_AttackResult.usAtkInfo[0] == 0)
										_snprintf(messagestring, sizeof(messagestring), _T("Charge HIT damage[%d]"), m_AttackResult.sDamage[0]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
//								g_pLocalUser->SetBlowMotion();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									if(m_AttackResult.usAtkInfo[0] == 1)
										_snprintf(messagestring, sizeof(messagestring), _T("Charge CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 2 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Miss
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge MISS"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 3 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Block
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge BLOCK"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 4 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Shock
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge SHOCK damage [%d]"), m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
//							g_pLocalUser->SetAnimation(_XACTION_MATCH_CANCEL);
//							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
						m_AttackResult.display[0] = FALSE;
					} 
				}
				else
				{
					_XUser* pUser = NULL;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					{
						if(((_XUser*)m_AttackTarget)->m_Uniqueid == m_AttackResult.usDefenderID)
							pUser = (_XUser*)m_AttackTarget;
					}
					if(!pUser)
						pUser = (_XUser*)g_Userlist.FindUser(m_AttackResult.usDefenderID);

					if(!pUser)
						return;

					D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
					offsetmat._42 += 0.98f;
					
					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
						case 5 :
							{
								// Hit
//								pUser->SetBlowMotion();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
//								pUser->SetBlowMotion();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 2 :
							{
								// Miss
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							
						}
						m_AttackResult.display[0] = FALSE;
					} 
				}
			} // target_type_others
		} // command frame
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(m_AttackResult.ucToType == en_target_type_monster)
			{
				ProcessMobDie();
			}
			else if(m_AttackResult.ucToType == en_target_type_object)
			{
				// 문이 파쾨되는 과정 			
			}


			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		} // last frame
	} // motion == fire
}

void _XUser::ProcessBattleRoundingSpell(void)
{
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
	{
		if(g_SkillProperty[m_AttackResult.sSkillID]->targetType == _XSI_TARGETTYPE_OTHER_ENEMY)
		{
			if(m_AttackResult.ucToType == en_target_type_monster)
			{
				_XMob* pMob = NULL;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					pMob = (_XMob*)m_AttackTarget;
				if(!pMob)
					pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);

				if(!pMob)
					return;
		
				if(m_AttackResult.display[0])
				{
					if(m_AttackResult.sDamage[0] != 0)
					{
						PlayAttackSound();
						pMob->SetBlowMotion();
						D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
						offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
						if( pMob->m_ModelDescriptor.m_EffectManager )
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
							if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
							{
								pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
							}
#endif
						}
					}
					
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
						
						if(pMob == (_XMob*)g_LastPickedObject)
							g_LastPickedObject = NULL;
					}
					m_AttackResult.display[0] = FALSE;
				}
			} // monster
			else if(m_AttackResult.ucToType == en_target_type_object)
			{
				_XFunctionalObject* pFunctionalObject = NULL;
				
				if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{
					if(((_XFunctionalObject*)m_AttackTarget)->m_AttackalbeTypeUniqueID == m_AttackResult.usDefenderID)
						pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
				}
				
				if(!pFunctionalObject)
					pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
				
				if(!pFunctionalObject)
					return;
				
				if(m_AttackResult.display[0])
				{
					if(m_AttackResult.sDamage[0] != 0)
					{
						PlayAttackSound();
						D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
						offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
						if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
							if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
							{
								pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
							}
#endif
						}
					}
					
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pFunctionalObject->m_HealthPercentage = 0;
						pFunctionalObject->SetDieAnimation();
					}
					m_AttackResult.display[0] = FALSE;
				}
			} // object
			else if(m_AttackResult.ucToType == en_target_type_others)
			{
				if(m_AttackResult.usDefenderID == g_pLocalUser->m_Uniqueid)
				{
					if(m_AttackResult.display[0])
					{
						if(m_AttackResult.sDamage[0] != 0)
						{
							D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
							offsetmat._42 += 0.98f;

							switch(m_AttackResult.usAtkInfo[0])
							{
							case 0 :
							case 5 :
								{
									// Hit
									if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
									{
										PlayAttackSound();
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
									
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										if(m_AttackResult.usAtkInfo[0] == 0)
											_snprintf(messagestring, sizeof(messagestring), _T("Rounding HIT damage[%d]"), m_AttackResult.sDamage[0]);
										else
											_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							case 1 :
							case 6 :
								{
									// Critical
									if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
									{
										PlayAttackSound();
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
									
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										if(m_AttackResult.usAtkInfo[0] == 1)
											_snprintf(messagestring, sizeof(messagestring), _T("Rounding CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);
										else
											_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							case 2 :
								{
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									// Miss
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Rounding MISS"));
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							case 3 :
								{
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									// Block
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Rounding BLOCK"));
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							case 4 :
								{
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									// Shock
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Rounding SHOCK damage [%d]"), m_AttackResult.sDamage[0]);
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							}

						}
						
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
//							g_pLocalUser->SetAnimation(_XACTION_MATCH_CANCEL);
//							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
						m_AttackResult.display[0] = FALSE;
					}
				}
				else
				{
					_XUser* pUser = NULL;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
						pUser = (_XUser*)m_AttackTarget;
					if(!pUser)
						pUser = (_XUser*)g_Userlist.FindUser(m_AttackResult.usDefenderID);

					if(!pUser)
						return;

					if(m_AttackResult.display[0])
					{
						if(m_AttackResult.sDamage[0] != 0)
						{
							PlayAttackSound();
							pUser->SetBlowMotion();
							D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
							offsetmat._42 += 0.98f;
							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
								}
#endif
							}
						}
						
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							if(pUser == (_XUser*)g_LastPickedObject)
								g_LastPickedObject = NULL;
						}
						m_AttackResult.display[0] = FALSE;
					}
				}
			} // other
		} // target type other_enemy
	} // command frame
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
	{
		// 들고 있던 무기 복구
		ReloadWeaponModel();
		
		if(m_AttackResult.ucToType == en_target_type_monster)
		{
			ProcessMobDie();
		}
		else if(m_AttackResult.ucToType == en_target_type_object)
		{
			// 문이 파쾨되는 과정 			
		}
		memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
		SetAnimation(_XACTION_IDLE);
	}
}

void _XUser::ProcessBattleHiddenMA(void)
{
	short motionindex = -1;
	if(m_CharacterInfo.gender == _XGENDER_MAN)
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
	else
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;
	
	if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT);
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
	{
		if(m_AttackResult.display[0])
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE);
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			// 들고 있던 무기 복구
			ReloadWeaponModel();
			
			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XUser::ProcessBattleChannelingSpell(void)
{
	short motionindex = -1;
	if(m_CharacterInfo.gender == _XGENDER_MAN)
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
	else
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;
	
	if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT);
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
	{
		// Martial_End 에서 Fire로 바뀜
//		if(m_AttackResult.display[0])
//			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE);
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			// 들고 있던 무기 복구
			ReloadWeaponModel();
			
			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XUser::ProcessBattleSelfChannelingSpell(void)
{
}

void _XUser::ProcessBattleTargetChannelingSpell(void)
{
}

void _XUser::ProcessBattleFinishSkill(void)
{
	short motionindex = -1;
	if(m_CharacterInfo.gender == _XGENDER_MAN)
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
	else
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;

	if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			_XDWINPRINT("READY motion Last frame");
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
	{
		if(m_AttackTarget && !m_bFinishDashStart)
		{
			D3DXVECTOR3 targetposition;
			if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				targetposition = ((_XMob*)m_AttackTarget)->m_Position;
			}
			else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
			{
				targetposition = ((_XUser*)m_AttackTarget)->m_Position;
			}
			else
			{
				targetposition = m_Position;
			}

			D3DXVECTOR3 pos, direction;
			direction = targetposition - m_Position;
			D3DXVec3Normalize(&direction, &direction);

			if(g_SkillProperty[m_AttackResult.sSkillID]->finishAttackRange > 0)
				pos = targetposition - (direction*g_SkillProperty[m_AttackResult.sSkillID]->finishAttackRange);
			else
				pos = targetposition - (direction*0.7f);

			pos.y = g_LodTerrain.GetTerrainAndRideOBBHeight(pos);
			m_TargetPosition = pos;

			m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
			UpdatePosition();

			m_bFinishDashStart = TRUE;
		}

		if(m_AttackResult.display[0])
		{
			_XDWINPRINT("=== Finish motion change [FIRE]");

			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);

			// target position 재조정
			if(g_SkillProperty[m_AttackResult.sSkillID]->finishAttackRange > 0)
			{
				D3DXVECTOR3 targetposition;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					targetposition = ((_XMob*)m_AttackTarget)->m_Position;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					targetposition = ((_XUser*)m_AttackTarget)->m_Position;
				}
				else
				{
					targetposition = m_Position;
				}
				
				D3DXVECTOR3 pos, direction;
				direction = targetposition - m_Position;
				D3DXVec3Normalize(&direction, &direction);
				
				pos = targetposition - (direction*0.7f);
				pos.y = g_LodTerrain.GetTerrainAndRideOBBHeight(pos);
				m_TargetPosition = pos;

				m_FinishSpeed = g_SkillProperty[m_AttackResult.sSkillID]->finishAttackSpeed;
				
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
				UpdatePosition();
			}
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
		{
			if(m_AttackResult.ucToType == en_target_type_monster)
			{
				_XMob* pMob = NULL;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					if(((_XMob*)m_AttackTarget)->m_MobID == m_AttackResult.usDefenderID)
						pMob = (_XMob*)m_AttackTarget;
				}
				if(!pMob)
					pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
				
				if(!pMob)
					return;
				
				D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
				offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
				
				if(m_AttackResult.display[0])
				{
					switch(m_AttackResult.usAtkInfo[0])
					{
					case 0 :
					case 5 :
						{
							// Hit
							pMob->SetBlowMotion();
							if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
							{
								PlayAttackSound();
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif	
								}
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical
							pMob->SetBlowMotion();
							if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
							{
								PlayAttackSound();
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif	
								}
							}
						}
						break;
					case 2 :
						{
							// Miss
						}
						break;
					default :
						break;
					}
					
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
					}
					m_AttackResult.display[0] = FALSE;
				}
			}
			else if(m_AttackResult.ucToType == en_target_type_object)
			{
				_XFunctionalObject* pFunctionalObject = NULL;
				
				if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{
					if(((_XFunctionalObject*)m_AttackTarget)->m_AttackalbeTypeUniqueID == m_AttackResult.usDefenderID)
						pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
				}
				
				if(!pFunctionalObject)
					pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
				
				if(!pFunctionalObject)
					return;
				
				D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
				offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
				
				if(m_AttackResult.display[0])
				{
					switch(m_AttackResult.usAtkInfo[0])
					{
					case 0 :
					case 5 :
						{
							// Hit
							if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
							{
								PlayAttackSound();
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif	
								}
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical
							if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
							{
								PlayAttackSound();
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
							}
						}
						break;
					case 2 :
						{
							// Miss
						}
						break;
					default :
						break;
					}
					
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
					{
						pFunctionalObject->m_HealthPercentage = 0;
						pFunctionalObject->SetDieAnimation();
					}
					m_AttackResult.display[0] = FALSE;
				}
			}
			else if(m_AttackResult.ucToType == en_target_type_others)
			{
				if(m_AttackResult.usDefenderID == g_pLocalUser->m_Uniqueid)
				{
					D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
					offsetmat._42 += 0.98f;
					
					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
						case 5 :
							{
								// Hit
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									if(m_AttackResult.usAtkInfo[0] == 0)
										_snprintf(messagestring, sizeof(messagestring), _T("Finish HIT damage[%d]"), m_AttackResult.sDamage[0]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
//								g_pLocalUser->SetBlowMotion();
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									if(m_AttackResult.usAtkInfo[0] == 1)
										_snprintf(messagestring, sizeof(messagestring), _T("Finish Critical damage[%d]"), m_AttackResult.sDamage[0]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 2 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Miss
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish MISS"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 3 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Block
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish BLOCK"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 4 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Shock
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish SHOCK damage [%d]"), m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						default :
							break;
						}
						
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
//							g_pLocalUser->SetAnimation(_XACTION_MATCH_CANCEL);
//							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
						m_AttackResult.display[0] = FALSE;
					}
				}
				else
				{
					_XUser* pUser = NULL;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					{
						if(((_XUser*)m_AttackTarget)->m_Uniqueid == m_AttackResult.usDefenderID)
							pUser = (_XUser*)m_AttackTarget;
					}
					if(!pUser)
						pUser = (_XUser*)g_Userlist.FindUser(m_AttackResult.usDefenderID);
					
					if(!pUser)
						return;
					
					D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
					offsetmat._42 += 0.98f;
					
					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
						case 5 :
							{
								// Hit
								pUser->SetBlowMotion();
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								pUser->SetBlowMotion();
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 2 :
							{
								// Miss
							}
							break;
						default :
							break;
						}
						
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							if(pUser == (_XUser*)g_LastPickedObject)
								g_LastPickedObject = NULL;
						}
						m_AttackResult.display[0] = FALSE;
						
					}
				}
			}

			m_bFinishDashStart = FALSE;
		} // command frame
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(m_AttackResult.ucToType == en_target_type_monster)
			{
				ProcessMobDie();
			}
			else if(m_AttackResult.ucToType == en_target_type_object)
			{
				// 문이 파쾨되는 과정 			
			}
			
			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XUser::ProcessBattleCastingSpell(void)
{
	short motionindex = -1;
	if(m_CharacterInfo.gender == _XGENDER_MAN)
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_male;
	else
		motionindex = g_SkillProperty[m_AttackResult.sSkillID]->motion_female;

//	_XDWINPRINT("XUser/ProcessCastingSpell] motion [%d/%d] frame [%f]", m_ModelDescriptor.GetCurMotion(), motionindex, m_ModelDescriptor.GetCurFrame());

	if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);

		// 모든 무기 type이면 들고 있던 무기 삭제
		DeleteWeaponModel(m_AttackResult.sSkillID);
		
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
//			_XDWINPRINT("READY motion Last frame");
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
	{
		if(m_AttackResult.display[0])
		{
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
		}
	}
	else if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
		{
			if(g_SkillProperty[m_AttackResult.sSkillID]->targetType == _XSI_TARGETTYPE_OTHER_ENEMY)
			{
				if(m_AttackResult.ucToType == en_target_type_monster)
				{
					_XMob* pMob = NULL;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						if(((_XMob*)m_AttackTarget)->m_MobID == m_AttackResult.usDefenderID)
							pMob = (_XMob*)m_AttackTarget;
					}
					if(!pMob)
						pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
					
					if(!pMob)
						return;
					
					D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
					offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
					
					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
						case 5 :
							{
								// Hit
								pMob->SetBlowMotion();
								
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								pMob->SetBlowMotion();
								
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 2 :
							{
								// Miss
							}
							break;
						default :
							break;
						}					
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
							pMob->SetAnimation(_XMOBACTION_DIE);
							pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
						}
						m_AttackResult.display[0] = FALSE;
					}				
				} // target_type_monster
				else if(m_AttackResult.ucToType == en_target_type_object)
				{
					_XFunctionalObject* pFunctionalObject = NULL;
					
					if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
					{
						if(((_XFunctionalObject*)m_AttackTarget)->m_AttackalbeTypeUniqueID == m_AttackResult.usDefenderID)
							pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
					}
					
					if(!pFunctionalObject)
						pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
					
					if(!pFunctionalObject)
						return;
					
					D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
					offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
					
					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
						case 5 :
							{
								// Hit
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									PlayAttackSound();
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
										}
#endif
									}
								}
							}
							break;
						case 2 :
							{
								// Miss
							}
							break;
						default :
							break;
						}					
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();
						}
						m_AttackResult.display[0] = FALSE;
					}				
				} // target_type_object
				else if(m_AttackResult.ucToType == en_target_type_others)
				{
					if(m_AttackResult.usDefenderID == g_pLocalUser->m_Uniqueid)
					{
						D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;
						
						if(m_AttackResult.display[0])
						{
							switch(m_AttackResult.usAtkInfo[0])
							{
							case 0 :
							case 5 :
								{
									// Hit
									if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
									{
										PlayAttackSound();
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
										}
#endif
									}

								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										if(m_AttackResult.usAtkInfo[0] == 0)
											_snprintf(messagestring, sizeof(messagestring), _T("Casting HIT damage[%d]"), m_AttackResult.sDamage[0]);
										else
											_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif							
								}
								break;
							case 1 :
							case 6 :
								{
									// Critical
//									g_pLocalUser->SetBlowMotion();
									
									if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
									{
										PlayAttackSound();
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
										}
#endif
									}
								
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										if(m_AttackResult.usAtkInfo[0] == 1)
											_snprintf(messagestring, sizeof(messagestring), _T("Casting CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);
										else
											_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							case 2 :
								{
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									// Miss
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Casting MISS"));
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							case 3 :
								{
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									// Block
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Casting BLOCK"));
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							case 4 :
								{
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									// Shock
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Casting SHOCK damage [%d]"), m_AttackResult.sDamage[0]);
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
									}
								#endif
								}
								break;
							default :
								break;
							}					
							if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
							{
//								g_pLocalUser->SetAnimation(_XACTION_MATCH_CANCEL);
//								g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							}
							m_AttackResult.display[0] = FALSE;
							
						}
					}
					else
					{
						_XUser* pUser = NULL;
						if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
						{
							if(((_XUser*)m_AttackTarget)->m_Uniqueid == m_AttackResult.usDefenderID)
								pUser = (_XUser*)m_AttackTarget;
						}
						if(!pUser)
							pUser = (_XUser*)g_Userlist.FindUser(m_AttackResult.usDefenderID);
						
						if(!pUser)
							return;
						
						D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;
						
						if(m_AttackResult.display[0])
						{
							switch(m_AttackResult.usAtkInfo[0])
							{
							case 0 :
							case 5 :
								{
									// Hit
									pUser->SetBlowMotion();
									
									if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
									{
										PlayAttackSound();
										if( pUser->m_ModelDescriptor.m_EffectManager )
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
											if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
											{
												pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
													m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
											}
#endif
										}
									}
								}
								break;
							case 1 :
							case 6 :
								{
									// Critical
									pUser->SetBlowMotion();
									
									if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
									{
										PlayAttackSound();
										if( pUser->m_ModelDescriptor.m_EffectManager )
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
											if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
											{
												pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
													m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
											}
#endif
										}
									}
								}
								break;
							case 2 :
								{
									// Miss
								}
								break;
							default :
								break;
							}					
							if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
							{
								// user의 죽음 처리
								if(pUser == (_XUser*)g_LastPickedObject)
									g_LastPickedObject = NULL;
							}
							m_AttackResult.display[0] = FALSE;
							
						}
					}
				}
			} // target type other_enemy
		} // command frame
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			// 들고 있던 무기 복구
			ReloadWeaponModel();
			
			if(m_AttackResult.ucToType == en_target_type_monster)
			{
				ProcessMobDie();
			}
			else if(m_AttackResult.ucToType == en_target_type_object)
			{
				// 문이 파쾨되는 과정 			
			}

			memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XUser::ProcessBattleSmashingSkill(void)
{
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
	{
		if(m_AttackResult.ucToType == en_target_type_monster)
		{
			_XMob* pMob = NULL;
			if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				pMob = (_XMob*)m_AttackTarget;
			if(!pMob)
				pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);

			if(!pMob)
				return;
	
			if(m_AttackResult.display[0])
			{
				if(m_AttackResult.sDamage[0] != 0)
				{
					PlayAttackSound();
					pMob->SetBlowMotion();
					D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
					offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
					if( pMob->m_ModelDescriptor.m_EffectManager )
					{
						pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
						}
#endif
					}
				}
				
				if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
				{
					pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
					pMob->SetAnimation(_XMOBACTION_DIE);
					pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
					
					if(pMob == (_XMob*)g_LastPickedObject)
						g_LastPickedObject = NULL;
				}
				m_AttackResult.display[0] = FALSE;
			}
		}
		else if(m_AttackResult.ucToType == en_target_type_object)
		{
			_XFunctionalObject* pFunctionalObject = NULL;
			
			if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
			{
				if(((_XFunctionalObject*)m_AttackTarget)->m_AttackalbeTypeUniqueID == m_AttackResult.usDefenderID)
					pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
			}
			
			if(!pFunctionalObject)
				pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
			
			if(!pFunctionalObject)
				return;
			
			
			if(m_AttackResult.display[0])
			{
				if(m_AttackResult.sDamage[0] != 0)
				{
					PlayAttackSound();
					D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
					offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
					if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
					{
						pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
						}
#endif
					}
				}
				
				if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
				{
					pFunctionalObject->m_HealthPercentage = 0;
					pFunctionalObject->SetDieAnimation();
				}
				m_AttackResult.display[0] = FALSE;
			}
		}
		else if(m_AttackResult.ucToType == en_target_type_others)
		{
			if(m_AttackResult.usDefenderID == g_pLocalUser->m_Uniqueid)
			{
				if(m_AttackResult.display[0])
				{
					if(m_AttackResult.sDamage[0] != 0)
					{
						D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
						case 5 :
							{
								// Hit
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									if(m_AttackResult.usAtkInfo[0] == 0)
										_snprintf(messagestring, sizeof(messagestring), _T("Smashing HIT damage[%d]"), m_AttackResult.sDamage[0]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									PlayAttackSound();
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &g_pLocalUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									if(m_AttackResult.usAtkInfo[0] == 1)
										_snprintf(messagestring, sizeof(messagestring), _T("Smashing CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);
									else
										_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 2 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Miss
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing MISS"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 3 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Block
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing BLOCK"));
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							#endif
							}
							break;
						case 4 :
							{
								// Shock
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing SHOCK damage [%d]"), m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_WHISPER);
								}
							}
							break;
						}
					}
					
					if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match)
					{
//						g_pLocalUser->SetAnimation(_XACTION_MATCH_CANCEL);
//						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}					
					m_AttackResult.display[0] = FALSE;
				}
			}
			else
			{
				_XUser* pUser = NULL;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					pUser = (_XUser*)m_AttackTarget;
				if(!pUser)
					pUser = (_XUser*)g_Userlist.FindUser(m_AttackResult.usDefenderID);

				if(!pUser)
					return;

				if(m_AttackResult.display[0])
				{
					if(m_AttackResult.sDamage[0] != 0)
					{
						PlayAttackSound();
						pUser->SetBlowMotion();
						D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;
						if( pUser->m_ModelDescriptor.m_EffectManager )
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
							if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
							{
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
							}
#endif
						}
					}
					
					if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
					{
						if(pUser == (_XUser*)g_LastPickedObject)
							g_LastPickedObject = NULL;
					}
					m_AttackResult.display[0] = FALSE;
				}
			}
		}
		else if(m_AttackResult.ucToType == en_target_type_dummy)
		{
			_XDummy* pDummy = NULL;
			if(m_AttackTargetObjectType == _XPICKINGTARGET_DUMMY)
			{
				if(((_XDummy*)m_AttackTarget)->m_DummyID == m_AttackResult.usDefenderID)
					pDummy = (_XDummy*)m_AttackTarget;
			}
			if(!pDummy)
				pDummy = (_XDummy*)g_DummyList.Find(m_AttackResult.usDefenderID);
			
			if(!pDummy)
				return;
			
			D3DXMATRIX offsetmat = pDummy->m_ModelDescriptor.m_Position;
			offsetmat._42 += (0.15f + ((FLOAT)m_ModelDescriptor.m_EffectPosition+2)/3.0f);
			
			if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
			{
				PlayAttackSound();
				if( pDummy->m_ModelDescriptor.m_EffectManager )
				{
					pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
				}
			}
		} // target_type_dummy
	}
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
	{
		if(m_AttackResult.ucToType == en_target_type_monster)
		{
			ProcessMobDie();
		}
		else if(m_AttackResult.ucToType == en_target_type_object)
		{
			// 문이 파쾨되는 과정 			
		}
		memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
		SetAnimation(_XACTION_IDLE);
	}
}

void _XUser::ProcessBattleActionSkill(void)
{
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
	{
	}
	if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
	{
		memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
		SetAnimation(_XACTION_IDLE);
	}
}

void _XUser::Render( void )
{
	m_ModelDescriptor.m_NoAnimationAndPresentPipeline = !m_CheckInFrustum;

	if(m_StopAnimation)
	{
		m_ModelDescriptor.SetLastFrame();
	}
	else
	{
		m_ModelDescriptor.Animation();
	}
	
	if(m_CheckInFrustum)
	{
		if( m_CharacterInfo.animationdescript.motionclass != _XACTION_DIE && !m_pGoundCollideOBB )
		{
			m_ModelDescriptor.RenderShadow();
		}
	}
	
	if( m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 6.0f )
	{
		if( m_CharacterInfo.animationdescript.motionclass == _XACTION_BLOW )
		{
			g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
			m_ModelDescriptor.RenderCharacter(this);
			g_pCurrentSelectMaterial = NULL;
		}
		else if( m_CharacterInfo.animationdescript.motionclass == _XACTION_DEFENCE)
		{
			g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
			g_pCurrentSelectMaterial->Emissive.g = 1.0f;
			g_pCurrentSelectMaterial->Emissive.b = 1.0f;
			m_ModelDescriptor.RenderCharacter(this);	
			g_pCurrentSelectMaterial->Emissive.g = 0.0f;
			g_pCurrentSelectMaterial->Emissive.b = 0.0f;
			g_pCurrentSelectMaterial = NULL;
		}
		else
		{
			m_ModelDescriptor.RenderCharacter(this);
		}
	}
	else
	{		
		m_ModelDescriptor.RenderCharacter(this);	
	}	

	m_ModelDescriptor.RenderSwordEffect( _XDEF_DEFAULTRENDERSTYLE_CHARACTERSWORDEFFECT );


#ifdef _XDWDEBUG	
	if( g_LodTerrain.m_ViewOBBObject )
	{
		gpDev->SetTransform( D3DTS_WORLD, &m_ModelDescriptor.m_Position );	
		m_CharacterOBB.m_OBBModel.Render();
	}
#endif

	if(m_CheckInFrustum)
	{
		BOOL intersect = FALSE;		
										
	//	if( m_CharacterInfo.animationdescript.motionclass != _XACTION_DIE ) // 다른 유저가 죽기모드로 들어가면 픽킹을 막음.
		{			
			D3DXVECTOR3 nRayDir;
			D3DXVECTOR3 mpos = m_Position;
			mpos.y		+= _XDEF_COLLIDEOFFSET_HUMAN;
			D3DXVec3Normalize( &nRayDir, &g_vPickRayDir );
			
			intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, 0.9f );
			
			// Check intersect plane space...
			if( intersect )
			{
				D3DXVECTOR3 PickedPoint;
	//			intersect = m_CharacterOBB.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, PickedPoint );
				intersect = m_ModelDescriptor.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, m_Position, m_RotateAngle, &PickedPoint );
				
				if( intersect )
				{
					FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - m_Position.x);
					FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - m_Position.z);
					FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
					
					if( _distance < g_LastPickedObjectDistance )
					{					
						if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
						{
							if( g_LastPickedObjectType != _XPICKINGTARGET_NPC && !g_LastPickedObject )
							{
								g_LastPickedObjectDistance = _distance;
								g_LastPickedObjectType = _XPICKINGTARGET_PC;
								g_LastPickedObject = this;
								
								CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;

								if( m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
								{
									eodapp->ReadyCursor( _XMOUSETYPE_PERSONALSTORE );
								}
								else
								{
#ifdef _XTS_PK
									if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT )
									{
										if( g_pLocalUser->m_MatchTargetID == this->m_Uniqueid )
										{
											eodapp->ReadyCursor( _XMOUSETYPE_MOBOVER );
										}
										else
										{
											eodapp->ReadyCursor( _XMOUSETYPE_CHARACTORSELECT );
										}									
									}
									else
									{
										// 평화모드, 같은 문파, 혹은 동행일 경우 pk 안됨
										BOOL bsetbattlecursor = TRUE;

										if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
											bsetbattlecursor = FALSE;
										else
										{
											if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_PK_FIGHT)
											{
												bsetbattlecursor = g_pLocalUser->CheckPKTarget(this);
											}
											else
											{
												BOOL bPressShiftKey = FALSE;
#ifdef _XTS_SAFETYZONE
												bPressShiftKey = gpInput->CheckKeyPress(DIK_LSHIFT);

												if( g_pLocalUser->CheckPKTarget( this ) )
												{
													bPressShiftKey= TRUE;
												}
//												if(g_pLocalUser->m_CharacterInfo.groupindex != _XGROUP_NANGIN)
//												{
//													if(this->m_CharacterInfo.groupindex != _XGROUP_NANGIN)
//													{
//														if(g_pLocalUser->GetGroupType() != this->GetGroupType())
//														{
//															bPressShiftKey= TRUE;
//														}
//													}
//												}
#else
												bPressShiftKey = gpInput->CheckKeyPress(DIK_LSHIFT);
#endif
												if(this->m_bEnemy)
													bsetbattlecursor = TRUE;
#ifdef _XTS_PKZONE_SERVERSETTING
					#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
												else if((bPressShiftKey && g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo) ||
													(bPressShiftKey && g_pLocalUser->GetEnterMatchEvent() && this->GetEnterMatchEvent()))
					#else
												else if(bPressShiftKey && g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo)
					#endif
#else
												else if(bPressShiftKey && g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo)
#endif
													bsetbattlecursor = TRUE;
												else
													bsetbattlecursor = FALSE;
											}
										}

										if(bsetbattlecursor)
										{
											eodapp->ReadyCursor( _XMOUSETYPE_MOBOVER );
										}
										else
										{
											eodapp->ReadyCursor( _XMOUSETYPE_CHARACTORSELECT );
										}
									}
#else
									if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT )
									{
										if( g_pLocalUser->m_MatchTargetID == this->m_Uniqueid )
										{
											eodapp->ReadyCursor( _XMOUSETYPE_MOBOVER );
										}
										else
										{
											eodapp->ReadyCursor( _XMOUSETYPE_CHARACTORSELECT );
										}									
									}
									else
									{								
										eodapp->ReadyCursor( _XMOUSETYPE_CHARACTORSELECT );
									}
#endif
								}
							}
						}					
					}
				}	
			}
		}
	}
	
#ifdef _XDWDEBUG
//#define _XVIEWCOLLIDEPOLY
#ifdef  _XVIEWCOLLIDEPOLY
/*	gpDev->SetTexture(0, NULL); 
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	gpDev->SetFVF( D3DFVF_INTERFACETVERTEX );

	_XBILLBOARD_VERTEX	vertex[8];

	for(int i = 0; i < 8; i++ )
	{
		vertex[i].x = m_Position.x + g_UserIntersectionPoly[i].x;
		vertex[i].y = m_Position.y + g_UserIntersectionPoly[i].y;
		vertex[i].z = m_Position.z + g_UserIntersectionPoly[i].z;
		
		if( intersect )
			vertex[i].color		= D3DCOLOR_ARGB( 64, 255, 255, 0 );
		else
			vertex[i].color		= D3DCOLOR_ARGB( 64, 128, 128, 128 );
		
		vertex[i].specular	= D3DCOLOR_ARGB( 255, 255, 255, 255 );
		vertex[i].tu = 0.0f;
		vertex[i].tv = 1.0f;
	}
	
	
	gpDev->SetFVF( D3DFVF_XBILLBOARD_VERTEX ); 	
	D3DXMATRIX	mat;
	D3DXMatrixIdentity( &mat );
	gpDev->SetTransform( D3DTS_WORLD, &mat );
	
	gpDev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8,
		                           8, g_CommonIntersectionIndices, D3DFMT_INDEX16,
	                               vertex, sizeof( _XBILLBOARD_VERTEX )  );
		
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_LodTerrain.m_Fog );
	gpDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gpDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );*/
#endif
	
#endif
		
}

bool _XUser::ProcessCollision(const D3DXVECTOR3& contactpoint, D3DXVECTOR3& movevelocity, D3DXVECTOR3& nextposition)
{
	// Collision plane
	D3DXVECTOR3 N = contactpoint;		
	
	// move the object
	nextposition += N;
	
	float nv = D3DXVec3Dot( &N, &movevelocity );    // impact velocity	
	if (nv > 0.0f)									// don't reflect
		return false;
	
    float n2 = D3DXVec3Dot( &N, &N );				// the normal of collision length squared
	
	if (n2 < 0.00001f)								// to small, can't be of any use
		return false;
	
	static float fElasticity = 2.8f;				// coefficient of elasticity
	static float fFriction   = 0.005f;				// coefficient of friciton
	//static float fElasticity = 1.4f;				// coefficient of elasticity
	//static float fFriction   = 0.010f;				// coefficient of friciton
	
	// collision response. calcualte velocity...
	D3DXVECTOR3 Vn = N * (nv / n2);					// relative velocity alon the normal of collision
	D3DXVECTOR3 Vt = movevelocity - Vn;				// tangencial velocity
	
	// V = -Vn . (1.0f + CoR) + Vt . CoF
	movevelocity -= (1.0f + fElasticity) * Vn + Vt * fFriction;	// reflect
	
	return true;
}

BOOL _XUser::GetNextMove( D3DXVECTOR3& next )
{
	FLOAT dx = ( m_TargetPosition.x - m_Position.x );
	FLOAT dz = ( m_TargetPosition.z - m_Position.z );
	FLOAT leftlength = _XFC_sqrt( dx*dx + dz*dz );

	if( leftlength < EPSILON2 )
	{
		next = m_Position;
		m_PrevMoveTime = 0.0f;
		SetStopAction();
		return FALSE;
	}	

	m_TargetPosition.y = next.y = m_Position.y;
			
	FLOAT		elapsedtime;
	D3DXVECTOR3 directionvector;
	if( m_PrevMoveTime == 0.0f )
	{
		m_PrevMoveTime = g_fLocalSystemTime;
		elapsedtime = 0.0f;
	}
	else
	{
		elapsedtime= g_fLocalSystemTime - m_PrevMoveTime;
	}

	FLOAT movespeed = 0.0f;
	if(m_bFinishDashStart)
	{
		movespeed = m_FinishSpeed;
	}
	else if(Get_m_MoveSpeedFromEffect() != 0)
	{
		movespeed = Get_m_fMoveSpeed() + (Get_m_fMoveSpeed()*((FLOAT)Get_m_MoveSpeedFromEffect()/100.0f));
	}
	else
	{
		movespeed = Get_m_fMoveSpeed();
	}
	
//	_XDWINPRINT("movespeed [%f]   m_fMoveSpeed [%f]   m_MoveSpeedFromEffect [%d]", movespeed, Get_m_fMoveSpeed(), Get_m_MoveSpeedFromEffect();
	
	FLOAT		curtimemovelength  = elapsedtime * movespeed;	// 이번 프레임에 갈 거리	
	if( curtimemovelength > leftlength ) curtimemovelength = leftlength;
	
	// Time slicing...
//	int repeatcollidechecktime = (int) (curtimemovelength / (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f) );
//	FLOAT lastdistance = fmod( curtimemovelength, (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f) );

	int repeatcollidechecktime = (int) (curtimemovelength / (movespeed * 30.0f) );
	FLOAT lastdistance = fmod( curtimemovelength, (movespeed * 30.0f) );
	if( lastdistance > EPSILON4 )
	{
		repeatcollidechecktime++;
	}

	POINT		pt;
	FLOAT		terrainheight = m_Position.y;
	D3DXVECTOR3 nextposition, contactpoint;
	_XOBBData*  pUnderObb = NULL;	
	D3DXVECTOR3 currentposition = m_Position;	// 진행위치

	m_PrevMoveTime = g_fLocalSystemTime;


	// 갈 방향의 벡터 생성	
	directionvector = D3DXVECTOR3(m_TargetPosition.x - currentposition.x, 0.0f, m_TargetPosition.z - currentposition.z) ;
	D3DXVec3Normalize(&directionvector, &directionvector);

	// 최소 시간단위로 쪼갠다...
//	FLOAT separatedmovelength = (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f);

	FLOAT separatedmovelength = (movespeed * 30.0f);
	for( int repeat = 0; repeat < repeatcollidechecktime; ++repeat )
	{
		if( (lastdistance > EPSILON4) && repeat == repeatcollidechecktime-1 ) separatedmovelength = lastdistance;

		// 계속 타겟까지 가야하면 다음 위치 계산
		nextposition = currentposition + (directionvector * separatedmovelength);
				 
		_XLODTILE*	pTerrainTile = g_LodTerrain.GetTerrainTile( nextposition , &pt );

		if( !pTerrainTile )
		{
			next = m_Position;
			m_PrevMoveTime = 0.0f;
			return FALSE;
		}

		terrainheight = g_LodTerrain.GetTerrainAndRideOBBHeight( nextposition, m_pGoundCollideOBB );
		
		// 다음 중력값 적용 높이가 지형보다 낮은경우나 무릅보다 작으면 위치 보정
		if( terrainheight > currentposition.y && terrainheight < currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT )
		{
			directionvector = D3DXVECTOR3(nextposition.x - currentposition.x, terrainheight - currentposition.y, nextposition.z - currentposition.z) ;
			D3DXVec3Normalize(&directionvector, &directionvector);
			nextposition.y = terrainheight;
		}
		else
		{
			directionvector = D3DXVECTOR3(nextposition.x - currentposition.x, 0.0f, nextposition.z - currentposition.z) ;
			D3DXVec3Normalize(&directionvector, &directionvector);
			nextposition.y = currentposition.y;
		}
		
		m_pGoundCollideOBB = NULL;
		m_pCollideOBB = NULL;
			
		// Rotate obb axis
		D3DXMATRIX	rotmat, newobbmat;
		D3DXMatrixRotationY( &rotmat, 6.28318530717958647692528676624f - m_LastRotateAngle );
		D3DXMatrixMultiply( &newobbmat, &rotmat, &g_BaseOBBAxisMatrix );	
		m_CharacterOBB.m_akAxis[0].x = newobbmat._11;
		m_CharacterOBB.m_akAxis[0].y = newobbmat._21;
		m_CharacterOBB.m_akAxis[0].z = newobbmat._31;
		m_CharacterOBB.m_akAxis[1].x = newobbmat._12;
		m_CharacterOBB.m_akAxis[1].y = newobbmat._22;
		m_CharacterOBB.m_akAxis[1].z = newobbmat._32;
		m_CharacterOBB.m_akAxis[2].x = newobbmat._13;
		m_CharacterOBB.m_akAxis[2].y = newobbmat._23;
		m_CharacterOBB.m_akAxis[2].z = newobbmat._33;
		
		// Expand obb
		FLOAT halfmovelength = separatedmovelength / 2.0f;
		m_CharacterOBB.m_afExtent[2] = 0.32f + halfmovelength;
		m_CharacterOBB.m_kCenter = currentposition + ((nextposition - currentposition) / 2.0f);
		m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
		m_CharacterOBB.RecalcRadius();
		
		// Check collision
		if( !pTerrainTile->svObbList.empty() )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = pTerrainTile->svObbList.begin(); it != pTerrainTile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				if( m_CharacterOBB.IntersectOBBvsOBB( *pObbData, contactpoint ) )
				{
					ProcessCollision(contactpoint, directionvector, nextposition );
					m_CharacterOBB.m_kCenter.x = nextposition.x;
					m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
					m_CharacterOBB.m_kCenter.z = nextposition.z;

					/*
					// Regenerate axis
					FLOAT rotateangle = _XMeshMath_GetRotateRadian(m_Position, nextposition);
					D3DXMatrixRotationY( &rotmat, 6.28318530717958647692528676624f - rotateangle );
					D3DXMatrixMultiply( &newobbmat, &rotmat, &g_BaseOBBAxisMatrix );
					m_CharacterOBB.m_akAxis[0].x = newobbmat._11;
					m_CharacterOBB.m_akAxis[0].y = newobbmat._21;
					m_CharacterOBB.m_akAxis[0].z = newobbmat._31;
					m_CharacterOBB.m_akAxis[1].x = newobbmat._12;
					m_CharacterOBB.m_akAxis[1].y = newobbmat._22;
					m_CharacterOBB.m_akAxis[1].z = newobbmat._32;
					m_CharacterOBB.m_akAxis[2].x = newobbmat._13;
					m_CharacterOBB.m_akAxis[2].y = newobbmat._23;
					m_CharacterOBB.m_akAxis[2].z = newobbmat._33;

					FLOAT dx = fabs(nextposition.x - m_Position.x);
					FLOAT dz = fabs(nextposition.z - m_Position.z);
					FLOAT movelength = _XFC_sqrt(dx*dx + dz*dz);
					
					m_CharacterOBB.m_afExtent[2] = 0.16f + (movelength / 2.0f);
					*/
#ifdef _XDWDEBUG
					pObbData->m_Collide	= TRUE;
#endif
				}
#ifdef _XDWDEBUG
				else pObbData->m_Collide = FALSE;
#endif					
			}
		}
		
		currentposition = nextposition;

		terrainheight = g_LodTerrain.GetTerrainAndRideOBBHeight( currentposition, m_pGoundCollideOBB );
		
		if( m_pGoundCollideOBB )
		{
			if( m_Velocity.y < EPSILON4 )
			{
				if( currentposition.y < terrainheight || currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT > terrainheight )
				{
					currentposition.y = terrainheight;
					m_CharacterOBB.m_kCenter.y = currentposition.y + 0.89f;
				}
			}
		}
	}

	if( !m_pGoundCollideOBB )
	{
		if( currentposition.y < terrainheight || currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT > terrainheight )
		{
			currentposition.y = terrainheight;
			m_CharacterOBB.m_kCenter.y = currentposition.y + 0.89f;
		}
	}	

	nextposition = currentposition;

#ifdef _XDWDEBUG	
	m_CharacterOBB.CreateOBBModel();
#endif	

	// Check water object
	S_WATER* curwater = g_LodTerrain.m_pWaterObjectManager->GetWaterObject(pt.y + (pt.x * g_LodTerrain.m_TileCount));
	
	if(curwater && curwater->enabled && (curwater->height > (nextposition.y + g_fWaterDepth)))
	{
		next = m_Position;
		m_PrevMoveTime = 0.0f;
		return FALSE;
	}
	
	int	xcoord = _XFC_FloatToIntRet( ( nextposition.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	int zcoord = _XFC_FloatToIntRet( ( nextposition.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;		
	
	if( g_LodTerrain.GetPathFindAreaInfo( zcoord, xcoord ) ) // 갈수 있는 영역일때만 이동.
	{
		next = m_Position;
		m_PrevMoveTime = 0.0f;
		return FALSE;
	}

	m_PrevPosition = m_Position;
	next = nextposition;
	
	if( GetMotionClass() != _XACTION_MOVE )
	{
		if( next.x != m_Position.x || next.z != m_Position.z )
		{
			if(GetMotionClass() != _XACTION_DYING)
			{
				if(!DoNotChangeMotion())
					SetAnimation( _XACTION_MOVE );
				return TRUE;
			}
			else
				return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL _XUser::DoNotChangeMotion(void)
{
	switch(m_AttackResult.skillType)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
	case _XSI_USEFULTYPE_FINISHSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
	case _XSI_USEFULTYPE_CASTINGSPELL :
	case _XSI_USEFULTYPE_HIDDENMA :
		return TRUE;
	}

	return FALSE;
}


void _XUser::SetDieMotion(void)
{
	ProcessMobDie();
	
	ResetAttackTarget();
	
	SetAnimation( _XACTION_DIE );
	m_ModelDescriptor.SetLockEndFrame( TRUE );
}

void _XUser::SetBlowMotion(void)
{
	if(GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_RESCUE) 
		return;

	ProcessMobDie();

	char usefulType = 0;
	usefulType = m_AttackResult.skillType;
	
	switch(usefulType)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
	case _XSI_USEFULTYPE_CASTINGSPELL :
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
	case _XSI_USEFULTYPE_FINISHSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_HIDDENMA :
		return;
	default :
		break;
	}

	m_TargetPosition = m_Position;
	SetAnimation( _XACTION_BLOW );
}

void _XUser::SetDefenceMotion(void)
{
	if(GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_RESCUE) 
		return;
	
	ProcessMobDie();

	char usefulType = 0;
	usefulType = m_AttackResult.skillType;

	switch(usefulType)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
	case _XSI_USEFULTYPE_CASTINGSPELL :
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
	case _XSI_USEFULTYPE_FINISHSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_HIDDENMA :
		return;
	default :
		break;
	}

	m_TargetPosition = m_Position;
	SetAnimation( _XACTION_DEFENCE );
	
	// Defence 하는 sound
	if( m_ModelDescriptor.m_WeaponClass ) // 메인무기라면...
	{
		if( m_CharacterInfo.modeldescript.item1 == 0 )
		{
			_XPlaySoundEffect( g_WeaponSoundIndexList[ 4 ], m_Position );
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					int weaponmaterial = WeaponItem[m_CharacterInfo.modeldescript.item1].ucQuality2;
					if( weaponmaterial < g_WeaponSoundCount )
					{
						_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				int weaponmaterial = g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].ucQuality2;
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				int weaponmaterial = g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].ucQuality2;
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				int weaponmaterial = g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].ucQuality2;
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
#endif
		}
	}
	else // 발차기라면...
	{
		_XPlaySoundEffect( g_WeaponSoundIndexList[ 4 ], m_Position );
	}


	// Defence 하는 sound
	/*
	if( m_CharacterInfo.modeldescript.item1 == 0 )
	{
		int randomindex = (_XRandom() % (g_HitEffectSoundCategoryLUT[0][1]-g_HitEffectSoundCategoryLUT[0][0]+1));
		_XPlaySoundEffect(g_HitEffectSoundIndex[randomindex] , m_Position);
	}
	else
	{
		switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].ucQuality2 ) 
		{
		case _XGI_WP_SOUNDMATERIAL_FIST :
			{
				int randomindex = (_XRandom() % (g_HitEffectSoundCategoryLUT[0][1]-g_HitEffectSoundCategoryLUT[0][0]+1));
				_XPlaySoundEffect(g_HitEffectSoundIndex[randomindex] , m_Position);
			}
			break;
		case _XGI_WP_SOUNDMATERIAL_LIGHTWOOD :
			{
				int randomindex = g_HitEffectSoundCategoryLUT[1][0] + (_XRandom() % (g_HitEffectSoundCategoryLUT[1][1]-g_HitEffectSoundCategoryLUT[1][0]+1));
				_XPlaySoundEffect( g_HitEffectSoundIndex[randomindex] , m_Position);
			}
			break;
		case _XGI_WP_SOUNDMATERIAL_HEAVYWOOD :
			{
				int randomindex = g_HitEffectSoundCategoryLUT[2][0] + (_XRandom() % (g_HitEffectSoundCategoryLUT[2][1]-g_HitEffectSoundCategoryLUT[2][0]+1));
				_XPlaySoundEffect( g_HitEffectSoundIndex[randomindex] , m_Position);			
			}
			break;
		case _XGI_WP_SOUNDMATERIAL_LIGHTMETAL	:
			{
				int randomindex = g_HitEffectSoundCategoryLUT[3][0] + (_XRandom() % (g_HitEffectSoundCategoryLUT[3][1]-g_HitEffectSoundCategoryLUT[3][0]+1));
				_XPlaySoundEffect( g_HitEffectSoundIndex[randomindex], m_Position);
			}
			break;	
		case _XGI_WP_SOUNDMATERIAL_MEDIUMMETAL	:
			{
				int randomindex = g_HitEffectSoundCategoryLUT[4][0] + (_XRandom() % (g_HitEffectSoundCategoryLUT[4][1]-g_HitEffectSoundCategoryLUT[4][0]+1));
				_XPlaySoundEffect( g_HitEffectSoundIndex[randomindex], m_Position);
			}
			break;	
		case _XGI_WP_SOUNDMATERIAL_HEAYVYMETAL	:
			{
				int randomindex = g_HitEffectSoundCategoryLUT[5][0] + (_XRandom() % (g_HitEffectSoundCategoryLUT[5][1]-g_HitEffectSoundCategoryLUT[5][0]+1));
				_XPlaySoundEffect( g_HitEffectSoundIndex[randomindex], m_Position);
			}
			break;	
		}
	}
	*/
}

void _XUser::SetStanceMotion(short index)
{
	if(index > 0)
		m_StanceMotionIndex = index + 1;
	else
		m_StanceMotionIndex = 0;
}

void _XUser::ProcessMobDie(void)
{
	// 다른 유저가 때리는 몹 죽음 처리 - display 안된것이 있다면
	for(int i = 0 ; i < 3 ; ++i)
	{
		if(this->m_AttackResult.display[i])
		{
			if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
			{
				_XMob* pMob = NULL;
				_XMob* pAttackTarget = (_XMob*)m_AttackTarget;

				if(pAttackTarget)
				{
					if(pAttackTarget->m_MobID == m_AttackResult.usDefenderID)
					{
						pMob = pAttackTarget;
					}
					else
					{
						pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
					}
				}
				else
				{
					pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
				}

				if(pMob)
				{
					pMob->SetAnimation(_XMOBACTION_DIE);
					pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
					
					memset(m_AttackResult.display, 0, sizeof(m_AttackResult.display));
					return;
				}
			}
			this->m_AttackResult.display[i] = FALSE;
		}
	}
}

void _XUser::CheckInsideFrustum( void )
{
	FLOAT dx = (m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
	FLOAT dz = (m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
	m_ModelDescriptor.m_CameraDistance = _XFastDistance( dx , dz );
				
	m_CheckInFrustum = ( (m_ModelDescriptor.m_CameraDistance < g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_USER_CULLINGRADIUS) &&
		( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( m_Position, 3.5f ) != NOT_VISIBLE ) );
}

void _XUser::SetDyingMode(void)
{
	if(GetMotionClass() != _XACTION_DYING)
	{
		SetAnimation(_XACTION_DYING);
	}
}

void _XUser::SetDieMode(void)
{
	if(GetMotionClass() != _XACTION_DIE)
		SetDieMotion();
}

void _XUser::SetRebirthBinsaMode(void)
{
	if(GetMotionClass() != _XACTION_REBIRTH)
		SetAnimation(_XACTION_REBIRTH);
}

void _XUser::SetRebirthHonsuMode(void)
{
	m_ModelDescriptor.SetLockEndFrame(FALSE);
	m_ModelDescriptor.PlayAnimation(TRUE);
	SetAnimation(_XACTION_IDLE);
}

void _XUser::DeleteWeaponModel(short skillid)
{
	if(g_SkillProperty[skillid]->weaponCType == -1 && g_SkillProperty[skillid]->weaponSType == 0)
	{
		int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
						m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );
		
		if( effectid > 0 )
		{
			m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
		}
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
#endif
	}
}

void _XUser::DeleteWeaponModel(void)
{
	int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
					m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );
	
	if( effectid > 0 )
	{
		m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
	}

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
	{
		_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
		if( WeaponItem )
		{
			if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
	}
#else
	if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
	{
		if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
	else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
	{
		if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
	else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
	{
		if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
#endif
}

void _XUser::ReloadWeaponModel(void)
{
	// 들고 있던 무기 복구
	if(m_CurrentWeaponID > 0)
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				if(WeaponItem[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
					if( WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex]);
					}
					else 
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
						g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex]);
#endif
					m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
					m_CurrentWeaponID = 0;
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if(g_WeaponItemProperty[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;
#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex]);
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
				}
#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex]);
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if(g_WeaponItemProperty2[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex]);	
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
				}

#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex]);
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if(g_WeaponItemProperty3[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, 
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex]);
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
#endif
		int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
						m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );
		if( effectid > 0 )
		{
			m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
				effectid, &m_ModelDescriptor.m_Position, NULL, TRUE );
		}
	}
}

int _XUser::GetGroupType(void)
{
	switch(m_CharacterInfo.groupindex)
	{
	case _XGROUP_GAEBANG :
	case _XGROUP_SORIM :
	case _XGROUP_MOODANG :
		{
			return _XGROUPTYPE_WHITE;
		}
	case _XGROUP_BEEGOONG :
	case _XGROUP_NOCKRIM :
	case _XGROUP_MAKYO :
		{
			return _XGROUPTYPE_BLACK;
		}
	}

	return _XGROUPTYPE_NONE;
}

void _XUser::SetFarAway(bool state, int index)
{
	if(g_pLocalUser->m_PartyInfo[index].isLeader)
	{
		_XWindow_PartyMasterMiniBar* pMaster_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
		if(pMaster_Window)
		{
			pMaster_Window->SetFarAway(state);
		}
	}
	else
	{
		int partymembercount = 0 ;
		for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
		{
			if(!g_pLocalUser->m_PartyInfo[i].isLeader)
			{
				partymembercount++;
				if(i == index) break;
			}
		}
		
		DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + partymembercount;
		
		_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
		if(pMiniBar_Window)
		{
			pMiniBar_Window->SetFarAway(state);
		}
	}
}

BOOL _XUser::GetEnterMatchEvent()
{
	// 영역 체크 해야 하나?
#ifdef _XDEF_MATCH_EVENT_PREVENT_ABUSE_080718_KUKURI
	if(this->m_bEnterMatchEvent &&
		g_AreaManager.FindPositionInArea(this->m_Position, g_MatchEventManager.GetCurrentEventArea(), _XDEF_AREACATEGORY_PVPAREA))//비무장 안에 있음)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
#else
	return this->m_bEnterMatchEvent;
#endif
}

BOOL _XUser::GetFreeMatchState()
{
	return this->m_bEnterMatchEvent;
}

//////////////////////////////////////////////////////////////////////
// _XUser object list

#define	_XDEF_DEFAULTMAXSORTBUFFER	512

_XUserList::_XUserList()
{
	m_RenderUserZSortListCurrentLength	= 0;
	m_RenderUserZSortListLength			= _XDEF_DEFAULTMAXSORTBUFFER;
	m_pRenderUserZSortList				= NULL;
	m_pRenderUserZSortList				= new _XUser*[m_RenderUserZSortListLength];
}

_XUserList::~_XUserList()
{
	SAFE_DELETE_ARRAY( m_pRenderUserZSortList );
	m_UserPool.DeleteData();
}

void _XUserList::disposeList(void)
{
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		DeleteUser((*it).second);
	}

	m_smUserList.clear();

	if( m_RenderUserZSortListLength > _XDEF_DEFAULTMAXSORTBUFFER )
	{
		m_RenderUserZSortListLength	=	_XDEF_DEFAULTMAXSORTBUFFER;
		SAFE_DELETE_ARRAY( m_pRenderUserZSortList );
		m_pRenderUserZSortList			=	new _XUser*[m_RenderUserZSortListLength];
	}

	m_RenderUserZSortListCurrentLength = 0;
}

void _XUserList::disposeList_zero(void)
{
	smdef_XUser::iterator it;
	for (it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		delete (*it).second;
	}
	m_smUserList.clear();
	
	smdef_OtganizationInfo::iterator it2;
	for( it2 = m_sOrganizationInfoList.begin(); it2 != m_sOrganizationInfoList.end(); ++ it2 )
	{
		delete (*it2).second;
	}
	m_sOrganizationInfoList.clear();	
	smdef_AlienceInfo::iterator it3;
	for( it3 = m_sAlienceInfoList.begin(); it3 != m_sAlienceInfoList.end(); ++it3 )
	{
		delete (*it3).second;
	}
	m_sAlienceInfoList.clear();	

	if( m_RenderUserZSortListLength > _XDEF_DEFAULTMAXSORTBUFFER )
	{
		m_RenderUserZSortListLength	=	_XDEF_DEFAULTMAXSORTBUFFER;
		SAFE_DELETE_ARRAY( m_pRenderUserZSortList );
		m_pRenderUserZSortList			=	new _XUser*[m_RenderUserZSortListLength];
	}

	m_RenderUserZSortListCurrentLength = 0;
}

void _XUserList::DeletePool(void)
{
	m_UserPool.DeleteData();

	m_RenderUserZSortListCurrentLength	= 0;
}

void _XUserList::CreatePool(void)
{
	m_UserPool.Reserve(50);
	m_RenderUserZSortListCurrentLength	= 0;
}

_XUser*	_XUserList::NewUser()
{
	_XUser* pUser = m_UserPool.New();
	pUser->InitializeData(); 
	return pUser;
}

void _XUserList::DeleteUser(_XUser* pUser)
{
	if (pUser == NULL) return;

	if(pUser->m_bInPartyMember)
	{
		int index = g_pLocalUser->CheckPartyMember(pUser->m_Uniqueid);
		if(index >= 0)
		{
			// 거리 이탈 체크
			pUser->SetFarAway(true, index);
		}
	}
	
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
/*	if(pUser->m_nWinnerNickName > 0)
	{
		if(g_pRewardNick_Window->GetShowStatus())
			g_pRewardNick_Window->ShowWindow(FALSE);
	}*/
#endif

	RemoveSortBuffer(pUser);
	pUser->DestroyData();
	m_UserPool.Delete(pUser);	
}

void _XUserList::RemoveSortBuffer( _XUser* pUser )
{
	if( !pUser || m_RenderUserZSortListCurrentLength <= 0 ) return;

	for( int i = 0; i < m_RenderUserZSortListCurrentLength; ++i )
	{
		if( m_pRenderUserZSortList[ i ] == pUser )
		{
			for( int nidx = i; nidx < m_RenderUserZSortListCurrentLength-1; ++nidx )
			{
				m_pRenderUserZSortList[ nidx ] = m_pRenderUserZSortList[ nidx+1 ];
			}

			m_RenderUserZSortListCurrentLength--;
			return;
		}
	}
}

_XUser* _XUserList::CreateUser(unsigned short usUniqueID, short sAnimationNumber,
							   FLOAT fListAngle, D3DXVECTOR3& vMoveVector,
							   D3DXVECTOR3& vSrcPosition, D3DXVECTOR3& vDestPosition)
{
	_XUser* pUser = NewUser();

	pUser->m_ModelDescriptor.InitEffectManager(1);
	pUser->m_Uniqueid = usUniqueID;

	pUser->SetAnimationIndex(sAnimationNumber);

	pUser->m_PrevProcessTime= g_LocalSystemTime;
	pUser->m_PrevMoveTime = 0;
	pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

	pUser->m_LastRotateAngle = fListAngle;
	pUser->m_MoveVector = vMoveVector;

	pUser->m_Position = vSrcPosition;
	pUser->m_TargetPosition = vDestPosition;

	// OBB check
	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight(pUser->m_Position, pLastPickedOBB);
	pUser->m_Position.y = height;
	pUser->m_pGoundCollideOBB = pLastPickedOBB;
	pUser->m_PrevPosition = pUser->m_Position;

	pUser->UpdatePosition();

	AddUser(pUser);

	// 새로 추가된 유저가 파티를 맺고 있는지를 검사하는 부분.
	if( !pUser->m_bInPartyMember )
	{
		int index = g_pLocalUser->CheckPartyMember(usUniqueID);
		if(index >= 0)
		{
			pUser->m_bInPartyMember = TRUE;

			// 거리 이탈 체크
			pUser->SetFarAway(false, index);
		}
		else
		{
			pUser->m_bInPartyMember = FALSE;
		}
	}

	_XDWPACKETLOG("New User added [id:%d name:%s  insidefrustum:%d]", pUser->m_Uniqueid, pUser->m_CharacterInfo.charactername, pUser->m_CheckInFrustum);

	return pUser;
}

_XUser*	_XUserList::FindUser( LPSTR charactername )
{
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		if( strcmp((*it).second->m_CharacterInfo.charactername, charactername ) == 0 )
		{
			return (*it).second;
		}		
	}
	return NULL;
}

_XUser*	_XUserList::FindUser( unsigned short uniqueid )
{
	smdef_XUser::iterator it = m_smUserList.find(uniqueid);
	if (it != m_smUserList.end())
	{
		return (*it).second;
	}
	return NULL;
}

BOOL _XUserList::AddUser( _XUser* user )
{
	m_smUserList.insert(smdef_XUser::value_type(user->m_Uniqueid, user));

	user->CheckInsideFrustum();
	if( user->m_CheckInFrustum )
	{
		if( m_RenderUserZSortListCurrentLength >= m_RenderUserZSortListLength )
		{
			m_RenderUserZSortListLength	+= 50;			
			_XUser** pNewRenderUserZSortList = new _XUser*[m_RenderUserZSortListLength];
			memcpy( pNewRenderUserZSortList, m_pRenderUserZSortList, sizeof(_XUser*) * m_RenderUserZSortListLength - 50 );
			SAFE_DELETE_ARRAY( m_pRenderUserZSortList );
			m_pRenderUserZSortList = pNewRenderUserZSortList;
		}
		
		m_pRenderUserZSortList[m_RenderUserZSortListCurrentLength] = user;
		m_RenderUserZSortListCurrentLength++;
	}

	return TRUE;
}

void _XUserList::DeleteUser( LPSTR charactername )
{		
	_XUser*	pUser = NULL;
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		pUser = (*it).second;

		if( strcmp(pUser->m_CharacterInfo.charactername, charactername ) == 0 )
		{
			g_Moblist.RemoveAttackTarget( pUser ); // 몬스터의 어텍 타겟에 속하면 타겟 삭제 

			if( g_pLocalUser->m_AttackTarget == pUser )
			{
				g_pLocalUser->ResetAttackTarget();
			}
			if(g_pLocalUser->m_ViewTarget == pUser)
			{
				g_pLocalUser->ResetViewTarget();
			}
			RemoveSortBuffer(pUser);
			DeleteUser((*it).second);
			m_smUserList.erase(it);
			return;
		}		
	}
}

void _XUserList::DeleteUser( int uniqueid )
{	
	smdef_XUser::iterator dt = m_smUserList.find(uniqueid);
	if (dt == m_smUserList.end())
	{
		return;
	}
	_XUser* pDeleteUser = (*dt).second;

	_XUser*	pUser = NULL;
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		 pUser = (*it).second;
		if( pUser != pDeleteUser )
		{
			if( pUser->m_AttackTarget == pDeleteUser )
			{
				pUser->ResetAttackTarget();
			}
		}
	}

	if( g_pLocalUser->m_AttackTarget == pDeleteUser )
	{
		g_pLocalUser->ResetAttackTarget();
	}
	if(g_pLocalUser->m_ViewTarget == pDeleteUser)
	{
		g_pLocalUser->ResetViewTarget();
	}

	g_Moblist.RemoveAttackTarget( pDeleteUser ); // 몬스터의 어텍 타겟에 속하면 타겟 삭제
	RemoveSortBuffer(pUser);
	DeleteUser(pDeleteUser);
	m_smUserList.erase(dt);
}

#define _XDEF_USER_KILLTIME				8500	// 08.06.13 15초에서 8.5초로 수정 Author:박소현

void _XUserList::Process( void )
{			
	g_pLocalUser->Process();

	_XUser*	_user = NULL;
	smdef_XUser::iterator it = m_smUserList.begin();
	while(it != m_smUserList.end())
	{
		 _user = (*it).second;

#ifdef _XDEF_MODELTHREADLOADING
		FLOAT dx = (_user->m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		FLOAT dz = (_user->m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
#else
		FLOAT dx = (_user->m_Position.x - g_pLocalUser->m_Position.x);//g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		FLOAT dz = (_user->m_Position.z - g_pLocalUser->m_Position.z);//g_LodTerrain.m_3PCamera.m_CameraPosition.z);
#endif
		_user->m_ModelDescriptor.m_CameraDistance = _XDistance(dx, dz);

		// 최종 메세지를 받은지 _XDEF_USER_KILLTIME초 이상 메세지를 다시 못받았을 경우 유저 삭제.
		if( g_LocalSystemTime - _user->m_LastRecivedMessageTime > _XDEF_USER_KILLTIME  )
		{								
			// 제한 거리에 상관없이 삭제하도록 변경
			// 메모리 풀로 데이타 삽입

			g_Moblist.RemoveAttackTarget( _user ); // 몬스터의 어텍 타겟에 속하면 타겟 삭제 
			
			if( g_pLocalUser->m_AttackTarget == _user )
			{
				g_pLocalUser->ResetAttackTarget();
			}
			if(g_pLocalUser->m_ViewTarget == _user)
			{
				g_pLocalUser->ResetViewTarget();
			}

			DeleteUser(_user);
			it = m_smUserList.erase(it);

		}
		else
		{
			_user->Process();
			++ it;
		}
	}
}

void _XUserList::Process_ZeroLevel( void )
{			
	g_pLocalUser->Process();

	FLOAT orgElapsedFrameTime = g_fElapsedFrameTime;
	XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	// hotblood zerolevel
	if( pZeroLevelProcess  )
	{
		if( pZeroLevelProcess->GetTutorialModeOn() )
			g_fElapsedFrameTime *= 0.f;
		else
			g_fElapsedFrameTime *= 0.8f;
	}
	else
		g_fElapsedFrameTime *= 0.8f;

	_XUser* _user;
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		_user = (_XUser*)(*it).second;
		_user->Process();
		if( _user->m_JobIndexOfZeroLevel == 6 || _user->m_JobIndexOfZeroLevel == 7 )
		{
			if( _user->m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
			{
				int tmp = _XRandom() % 100;
				if( tmp < 100 )		_user->SetAnimation(_XACTION_ATTACK);
				if( tmp < 10 )		_user->SetAnimation(_XACTION_BLOW);
			}					
		}
	}
	
	g_fElapsedFrameTime = orgElapsedFrameTime;
}


void _XUserList::Render( void )
{
	/*
	if( m_RenderUserZSortListCurrentLength > 0 )
	{	
		_XUser* _user;
		for( int uidx = 0; uidx < m_RenderUserZSortListCurrentLength; uidx++ )
		{
			_user = m_pRenderUserZSortList[uidx];

			// 다른유저 경공시 1.5초이상 패킷이 안들어오면 화면에서 숨김.
			if( _user->m_CharacterInfo.animationdescript.motionclass == _XACTION_MOVE &&
				_user->m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG )
			{
				if( g_LocalSystemTime - _user->m_LastRecivedMessageTime > 1500 )
				{
					_user->m_CheckInFrustum = FALSE;
				}
			}

			if(!_user->m_bInvisible)
				_user->Render();
		}
	}
	*/

	_XUser* _user;
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		_user = (_XUser*)(*it).second;

		if( g_ZeroLevelMode )	// zerolevel
		{
#ifdef _XDEF_MODELTHREADLOADING
			
#else
			FLOAT dx = (g_pLocalUser->m_Position.x - _user->m_Position.x);
			FLOAT dz = (g_pLocalUser->m_Position.z - _user->m_Position.z);
			_user->m_ModelDescriptor.m_CameraDistance = _XFastDistance( dx, dz );
#endif
		}
		// 제한 거리 이상이면 디스플레이 안함.
		if( _user->m_ModelDescriptor.m_CameraDistance < g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_USER_CULLINGRADIUS )
		{
			// 다른유저 경공시 1.5초이상 패킷이 안들어오면 화면에서 숨김. // 1.5초에서 1초로 수정 // Author :박소현
			if( _user->m_CharacterInfo.animationdescript.motionclass == _XACTION_MOVE &&
				_user->m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG )
			{
				if( g_LocalSystemTime - _user->m_LastRecivedMessageTime > 1000 )
				{
					_user->m_CheckInFrustum = FALSE;

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
/*					if(_user->m_nWinnerNickName > 0)
					{
						if(g_pRewardNick_Window->GetShowStatus())
							g_pRewardNick_Window->ShowWindow(FALSE);
					}*/
#endif
				}
			}
			

			if(!_user->Get_m_bInvisible())
				_user->Render();
		}
	}
	
	g_pLocalUser->Render();
}

void _XUserList::RenderUserEffectAndToolTip( void )
{	
	//gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);	
	
	static DWORD dwLastTime = 0L;


	D3DXVECTOR3 mpos;
	D3DXVECTOR3 _rvec;	
	mpos = g_pLocalUser->m_Position;	
	mpos.y = g_pLocalUser->m_Position.y + 1.4f;
	
	D3DXVec3Project( &_rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
					 &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
	
	int tooltipxpos = (int)(_rvec.x += 0.5f);
	int tooltipypos = (int)(_rvec.y += 0.5f);

	int drawPosX = 0;
	

	if( g_pLocalUser->m_ChatBalloonObject.m_Show )	
	{
		g_pLocalUser->m_ChatBalloonObject.Draw( tooltipxpos, tooltipypos );
	}

	if( g_pLocalUser->m_CheckInFrustum )
	{
		BOOL freecameramode = ( g_FreeMoveMode && g_InnerUser );
		
		if( !freecameramode && !g_pLocalUser->Get_m_bInvisible())
		{
			if( !g_pLocalUser->m_ChatBalloonObject.m_Show )
			{
				if(g_ViewMyNameTooltip)
				{	
					if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
					{
						// 개인 상점을 열었을때는 상점 명을 그려준다.
						if( g_pLocalUser->m_PersonalStoreName[0] != 0 )
						{	
							if(g_pLocalUser->m_CharacterInfo.charactername[0] != 0)
							{											
								
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );	

								_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2893), g_pLocalUser->m_CharacterInfo.charactername );//"[%s님의 노점]"
								
								TCHAR storenamestring[256];
								memset( storenamestring, 0, sizeof(storenamestring) );	
								int length = strlen( g_pLocalUser->m_PersonalStoreName );
								if( length > 24 )
								{
									if( g_pLocalUser->m_PersonalStoreName[24] & 0x80 ) //한글일때 2바이트 
									{
										int i = 0;
										while( i < 24 )
										{		
											if( g_pLocalUser->m_PersonalStoreName[i] & 0x80 )
												i+=2;
											else
												i+=1;
										}
										strncpy( storenamestring, g_pLocalUser->m_PersonalStoreName, sizeof(TCHAR)*i );	
										strcat( storenamestring, _T(".."));
									}
									else
									{
										strncpy( storenamestring, g_pLocalUser->m_PersonalStoreName, sizeof(TCHAR)*24 );	
										strcat( storenamestring, _T(".."));
									}
								}
								else
								{
									strncpy( storenamestring, g_pLocalUser->m_PersonalStoreName, sizeof(storenamestring) );
								}
								
								g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, storenamestring, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCPERSONALSTORE );
							}
						}
					}
					else if( g_pLocalUser->m_bMatchPRMode ) // 비무 홍보일때 
					{
						// 비무 홍보를 시작하면 비무홍보내용을 보여준다.
						if( g_pLocalUser->m_stringMatchPR[0] != 0 )
						{	
							if(g_pLocalUser->m_CharacterInfo.charactername[0] != 0)
							{																			
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );	
								_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2892), g_pLocalUser->m_CharacterInfo.charactername );//"[%s님의 비무홍보]"
								
								g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, g_pLocalUser->m_stringMatchPR, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCMATCHPRMODE );
								
							}
						}
					}//else if( _user->m_bMatchPRMode ) // 비무 홍보일때 
					else
					{
						// 상태효과에서 이름표 안보여주기 속성이면 draw하지 않음
						if(g_pLocalUser->m_Lock.notdrawusername == 0)
						{
							// 캐릭터 이름 보여주기
							if( g_pLocalUser->m_CharacterInfo.Get_gm_level() > 2 ) 
							{
								TCHAR namebuffer[64];
								memset(namebuffer, 0, sizeof(namebuffer));
								_snprintf( namebuffer, sizeof(namebuffer), _T("GM : %s"), g_pLocalUser->m_CharacterInfo.charactername );
								g_OperaterTooltip.SetToolTipStringEx_Noresize( tooltipxpos, tooltipypos, namebuffer, 0xFFFFE500 );
								g_OperaterTooltip.Draw();
							}
							else
							{
								int DrawPos = 0;
#ifdef _XTS_FAME
								int fame = g_FameStr->GetFameIndex(g_pLocalUser->m_CharacterInfo.Get_honor());
								int infamy = g_FameStr->GetInfamyIndex(g_pLocalUser->m_CharacterInfo.Get_Notoriety());

								if( infamy > -1 || fame > -1 )
								{
									g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, g_pLocalUser->m_CharacterInfo.charactername, D3DCOLOR_ARGB( 255, 230, 230, 230 ), 32, TRUE, FALSE );

									int drawwidth = 0; 
									if( fame <= infamy )
									{				
										drawwidth = g_XBaseFont->GetWidth( g_FameStr[infamy].m_InfamyName )+12;
										int ftxpos = g_WorldObjectTooltip.GetWindowPos().x - drawwidth;
										g_FameTooltip.SetToolTipStringEx( ftxpos, tooltipypos, g_FameStr[infamy].m_InfamyName, D3DCOLOR_ARGB( 255, 241, 51, 51 ), 32, FALSE, FALSE );
									}
									else
									{
										drawwidth = g_XBaseFont->GetWidth( g_FameStr[fame].m_FameName )+12;										
										int ftxpos = g_WorldObjectTooltip.GetWindowPos().x - drawwidth;
										g_FameTooltip.SetToolTipStringEx( ftxpos, tooltipypos, g_FameStr[fame].m_FameName, D3DCOLOR_ARGB( 255, 50, 100, 255 ), 32, FALSE, FALSE );
									}
									
									g_FameTooltip.DrawNoEdge(5);
								}
								else
									g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, g_pLocalUser->m_CharacterInfo.charactername, D3DCOLOR_ARGB( 255, 230, 230, 230 ), 32, TRUE, FALSE );								
#else
								g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, g_pLocalUser->m_CharacterInfo.charactername, D3DCOLOR_ARGB( 255, 230, 230, 230 ), 32, TRUE, FALSE );
#endif


								if(!g_pLocalUser->m_InParty )
								{
									if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
									{
										g_WorldObjectTooltip.SetTextColor(D3DCOLOR_ARGB(255, 255, 0, 0));
									}
									else
									{
										g_WorldObjectTooltip.SetTextColor(D3DCOLOR_ARGB(255, 230, 230, 230));
									}

#ifdef _XTS_PK
									g_WorldObjectTooltip.DrawNoEdge(0);
#else
									g_WorldObjectTooltip.DrawNoEdge();
#endif									
								}
								else
								{
									if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
									{
										g_WorldObjectTooltip.SetTextColor(D3DCOLOR_ARGB(255, 200, 0, 0));
									}
									else
									{
										g_WorldObjectTooltip.SetTextColor(D3DCOLOR_ARGB(255, 230, 230, 230));
									}
									
									g_WorldObjectTooltip.DrawParty();
								}

							#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
								if( strlen(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName) ) //사호가 있으면 사호를 찍는다
								{
									DrawPos -= 20;

									g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 238, 229, 98 ));//황금색

									g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
									g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName );
									g_XBaseFont->Flush();
									g_XBaseFont->DisableGrowMode();
								}
								else if( g_pLocalUser->m_CharacterInfo.m_NickName[0] != 0 )
							#else
								// 별호 보여주기
								if( g_pLocalUser->m_CharacterInfo.m_NickName[0] != 0 )
							#endif
								{
									DrawPos -= 20;

									int tableindex = -1;
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
									if(g_pLocalUser->m_CharacterInfo.m_CurrentNIckIndex == -2)
									{
										if(g_pLocalUser->m_nWinnerNickName > 0)
										{
											for(int nickcount = 0; nickcount < g_MaxNickNameCount; ++nickcount)
											{
												if(g_NickNameInfoTable[nickcount].usIndex == g_pLocalUser->m_nWinnerNickName)
												{
													tableindex = nickcount;
													break;
												}
											}
										}
									}
									else
#endif
									{
										for( int j = 0; j < g_MaxNickNameCount; ++j)
										{
											if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[g_pLocalUser->m_CharacterInfo.m_CurrentNIckIndex] )
											{
												tableindex = j;
												break;
											}
										}
									}
									if( tableindex >= 0 )
									{
										switch(g_NickNameInfoTable[tableindex].cType1) 
										{
										case 1:	// 특수
											g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 250, 160, 40 ));
											break;
										case 2:	// 일반
											{
												switch( g_NickNameInfoTable[tableindex].cType2 )
												{
												case 1 :	// 별호
													g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 230, 200, 120 ));
													break;
												case 2 :	// 별칭
													g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 180, 180, 180 ));
													break;
												}
											}
											break;
										case 3:	// 고유
											g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 180, 180, 180 ));
											break;		
										default :
											g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 180, 180, 180 ));
											break;
										}
									}											
									g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
									g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, g_pLocalUser->m_CharacterInfo.m_NickName );
									g_XBaseFont->Flush();
									g_XBaseFont->DisableGrowMode();
								}
								
								if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
								{                                    
									if( strlen(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName) )	// 영웅단 표시
									{
										DrawPos -= 20;
										g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
										
										TCHAR orgName[256];
										memset(orgName, 0, sizeof(orgName));
										if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
										{
											
											_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3172), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s회 회주]"
											g_XBaseFont->SetColor(0xFF8BC7F6);
											g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
										}
										else if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_ )
										{
											_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3173), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s회 부회주]"
											g_XBaseFont->SetColor(0xFF8BC7F6);
											g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
										}
										else
										{
											_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3174), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s]"
											g_XBaseFont->SetColor(0xFF8BC7F6);
											g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
										}
										g_XBaseFont->Flush();
										g_XBaseFont->DisableGrowMode();

										drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
									}									
                                    else //Author : 양희왕 //breif : 만일 한글자도 안쓰고 방회를 만들었다면..
                                    {
                                        DrawPos -= 20;
										g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
										
										TCHAR orgName[256];
										memset(orgName, 0, sizeof(orgName) );
										if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
										{
											
											_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA)); //"이름없음"
											g_XBaseFont->SetColor(0xFF8BC7F6);
											g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
										}
										else if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_ )
										{
											_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA));
											g_XBaseFont->SetColor(0xFF8BC7F6);
											g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
										}
										else
										{
											_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA));
											g_XBaseFont->SetColor(0xFF8BC7F6);
											g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
										}
										g_XBaseFont->Flush();
										g_XBaseFont->DisableGrowMode();

										drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
                                    }
								}
								else //영웅단 동맹에 가입되지 않았으므로 영웅단을 표시한다.
								{
									if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
									{
										if( strlen(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName) )	// 영웅단 표시
										{
											DrawPos -= 20;
											g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
											
											TCHAR orgName[256];
											memset(orgName, 0, sizeof(orgName));

											if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
											{
												_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_2894), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName ); //"[%s단 단주]"
												g_XBaseFont->SetColor(0xFF8BC7F6);
												g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
											}
											else
											{
												_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_2895), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );//"[%s단]"
												g_XBaseFont->SetColor(0xFF8BC7F6);
												g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
											}
											g_XBaseFont->Flush();
											g_XBaseFont->DisableGrowMode();

											drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
										}
                                        else //Author : 양희왕 //breif : 만일 한글자도 안쓰고 영웅단을 만들었다.
                                        {
                                            DrawPos -= 20;
											g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
											
											TCHAR orgName[256];
											memset(orgName, 0, sizeof(orgName));

											if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
											{
												_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA));
												g_XBaseFont->SetColor(0xFF8BC7F6);
												g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
											}
											else
											{
												_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA));
												g_XBaseFont->SetColor(0xFF8BC7F6);
												g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
											}
											g_XBaseFont->Flush();
											g_XBaseFont->DisableGrowMode();

											drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
                                        }
									}
								}
#ifdef _XDEF_PARTY_NOTAVAILABLE
								// 자리 비움 표시
								if(g_pLocalUser->m_bOut)
								{
									g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 230, 230, 230));
									g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+20, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3391) );	//"<자리 비움>"
									g_XBaseFont->Flush();
								}
#endif
#ifdef _XDEF_CASTLEBATTLE
								// 피아 식별 마크
								if(g_pLocalUser->m_bCastleBattle)
								{
									if(g_pLocalUser->m_dwAttackType == 1 || g_pLocalUser->m_dwAttackType == 2)
									{
										g_pMark_Window->m_pImgAttackType[g_pLocalUser->m_dwAttackType-1]->Draw(tooltipxpos-drawPosX-30-2, tooltipypos+DrawPos-9);
//										g_pLocalUser->m_pImgAttackType[g_pLocalUser->m_dwAttackType-1]->Draw(tooltipxpos-30-2, tooltipypos+DrawPos-9);
									}
									if(g_pLocalUser->m_dwRoleInCastleBattle == 1)
									{
										g_pMark_Window->m_pImgCaptain->Draw(tooltipxpos-drawPosX-30-2+6, tooltipypos+DrawPos-9-2-18);
//										g_pLocalUser->m_pImgCaptain->Draw(tooltipxpos-30-2, tooltipypos+DrawPos-9-2-18);
									}
								}
#endif
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
/*								if(g_pLocalUser->m_nWinnerNickName > 0)
								{
									g_pRewardNick_Window->MoveWindow(tooltipxpos-drawPosX-30-2-68, tooltipypos+DrawPos-9-98);
								}*/
#endif
							}
						}
					}
				}
			}
			else
			{
				if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_OPEN && !g_pLocalUser->m_bMatchPRMode )
				{
					g_pLocalUser->m_ChatBalloonObject.Draw( tooltipxpos, tooltipypos );
					
					if( g_pLocalUser->m_CharacterInfo.Get_gm_level() > 2 ) 
					{
						TCHAR namebuffer[64];
						memset(namebuffer, 0, sizeof(namebuffer));

						_snprintf( namebuffer, sizeof(namebuffer), _T("GM : %s"), g_pLocalUser->m_CharacterInfo.charactername );
						g_OperaterTooltip.SetToolTipStringEx_Noresize( tooltipxpos, tooltipypos - g_XBaseFont->GetHeight(), namebuffer, 0xFFFFE500 );
						g_OperaterTooltip.Draw();
					}
				}
				else // 상점일때는..
				{
					if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
					{
						// 개인 상점을 열었을때는 상점 명을 그려준다.
						if( g_pLocalUser->m_PersonalStoreName[0] != 0 )
						{	
							if(g_pLocalUser->m_CharacterInfo.charactername[0] != 0)
							{											
								
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );	
								_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2893), g_pLocalUser->m_CharacterInfo.charactername );//"[%s님의 노점]"
								
								TCHAR storenamestring[256];
								memset( storenamestring, 0, sizeof(storenamestring) );	
								int length = strlen( g_pLocalUser->m_PersonalStoreName );
								if( length > 24 )
								{									
									if( g_pLocalUser->m_PersonalStoreName[24] & 0x80 ) //한글일때 2바이트 
									{
										int i = 0;
										while( i < 24 )
										{		
											if( g_pLocalUser->m_PersonalStoreName[i] & 0x80 )
												i+=2;
											else
												i+=1;
										}
										
										strncpy( storenamestring, g_pLocalUser->m_PersonalStoreName, sizeof(TCHAR)*i );	
										strcat( storenamestring, _T(".."));
									}
									else
									{
										strncpy( storenamestring, g_pLocalUser->m_PersonalStoreName, sizeof(TCHAR)*24 );	
										strcat( storenamestring, _T(".."));
									}
								}
								else
								{
									strncpy( storenamestring, g_pLocalUser->m_PersonalStoreName, sizeof(storenamestring) );
								}
								
								g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, storenamestring, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCPERSONALSTORE );
							}
						}
					}
					else if( g_pLocalUser->m_bMatchPRMode ) // 비무 홍보일때 
					{
						// 비무 홍보를 시작하면 비무홍보내용을 보여준다.
						if( g_pLocalUser->m_stringMatchPR[0] != 0 )
						{	
							if(g_pLocalUser->m_CharacterInfo.charactername[0] != 0)
							{																			
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );	
								_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2892), g_pLocalUser->m_CharacterInfo.charactername );//"[%s님의 비무홍보]"
								
								g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, g_pLocalUser->m_stringMatchPR, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCMATCHPRMODE );
								
							}
						}
					}//else if( _user->m_bMatchPRMode ) // 비무 홍보일때 
					

				}//상점일때  
			}
			
		} // if(!g_pLocalUser->Get_m_bInvisible())
	}		
	if( m_RenderUserZSortListCurrentLength > 0 )
	{	
		_XUser* _user;
		//for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
		for( int uidx = 0; uidx < m_RenderUserZSortListCurrentLength; ++uidx )
		{
			//_user = (*it).second;
			_user = m_pRenderUserZSortList[uidx];			

			// 제한 거리 이상이면 디스플레이 안함.
			//if( _user->m_ModelDescriptor.m_CameraDistance < g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_USER_CULLINGRADIUS )
			{
				if(_user->m_CheckInFrustum)
				{
					if(!_user->Get_m_bInvisible())
					{
//						drawPosX = 0;
						
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
						if((g_pLocalUser->GetEnterMatchEvent() || g_pLocalUser->GetEnterMatchReadyArea()) 
							&& g_MatchEventManager.GetActionConstraint(_xen_const_hideme))
						{
							// 비무에 참가 중임 - 모든  유저의 이름표 보여주지 않음: 어뷰징 방지를 위해 수정.
							// 비무중 부활 영역에 있을때도 이름표 보여주지 않음
							continue;
						}
#endif
#ifdef _XDEF_MATCH_EVENT_PREVENT_ABUSE_080718_KUKURI
						if(_user->GetFreeMatchState())
						{
							// 영웅대회 참가중인 유저는 이름표 보여주지 않는다
							continue;
						}
#endif

						if( _user->m_CharacterInfo.charactername[0] )
						{
							BOOL bFaceMaskMode = FALSE;

							mpos = _user->m_Position;

							mpos.y = _user->m_Position.y + 1.4f;

							D3DXVec3Project( &_rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
											 &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
										
							tooltipxpos = (int)(_rvec.x += 0.5f);
							tooltipypos = (int)(_rvec.y += 0.5f);

							if( !_user->m_ChatBalloonObject.m_Show )
							{
								if(g_ViewUserNameTooltip)
								{	
									if( _user->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
									{
										// 개인 상점을 열었을때는 상점 명을 그려준다.
										if( _user->m_PersonalStoreName[0] != 0 )
										{	
											if(_user->m_CharacterInfo.charactername[0] != 0)
											{											
												
												TCHAR messagestring[256];
												memset( messagestring, 0, sizeof(messagestring) );	
												_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2893), _user->m_CharacterInfo.charactername );//"[%s님의 노점]"
												
												TCHAR storenamestring[256];
												memset( storenamestring, 0, sizeof(storenamestring) );	

												int length = strlen( _user->m_PersonalStoreName );
												if( length > 24 )
												{
													if( _user->m_PersonalStoreName[24] & 0x80 ) //한글일때 2바이트 
													{
														int i = 0;
														while( i < 24 )
														{		
															if( _user->m_PersonalStoreName[i] & 0x80 )
																i+=2;
															else
																i+=1;
														}
														strncpy( storenamestring, _user->m_PersonalStoreName, sizeof(TCHAR)*i );	
														strcat( storenamestring, _T(".."));
													}
													else
													{
														strncpy( storenamestring, _user->m_PersonalStoreName, sizeof(TCHAR)*24 );	
														strcat( storenamestring, _T(".."));
													}
												}
												else
												{
													strncpy( storenamestring, _user->m_PersonalStoreName, sizeof(storenamestring) );
												}
												
												g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, storenamestring, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCPERSONALSTORE );
												
												// 이름창 클릭시 PC 클릭과 동일하게 처리 
												ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
												if( ((scrnpos->x > tooltipxpos-(g_NPCNameWindow.m_WindowSize.cx>>1) ) && (scrnpos->x < tooltipxpos + (g_NPCNameWindow.m_WindowSize.cx>>1) ) &&
													(scrnpos->z > tooltipypos ) && (scrnpos->z < tooltipypos + g_NPCNameWindow.m_WindowSize.cy) ) )
												{			
													FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - _user->m_Position.x);
													FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - _user->m_Position.z);
													FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
													
													if( _distance < g_LastPickedObjectDistance )
													{		
														if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
														{
															if( g_LastPickedObjectType != _XPICKINGTARGET_NPC && !g_LastPickedObject )
															{
																g_LastPickedObjectDistance = _distance;
																g_LastPickedObjectType = _XPICKINGTARGET_PC;
																g_LastPickedObject = (_XItem*)_user;
																
																//CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
																//eodapp->ReadyCursor( _XMOUSETYPE_PERSONALSTORE );
															}
														}			
													}
													
												}
												//g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, messagestring , D3DCOLOR_ARGB( g_UsernameTooltipViewTimer, 222, 191, 48 ), 32, TRUE, FALSE );
												//g_WorldObjectTooltip.Draw();
												
												//g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos-20, _user->m_PersonalStoreName, D3DCOLOR_ARGB( g_UsernameTooltipViewTimer, 230, 230, 230 ), 32, TRUE, FALSE );
												//g_WorldObjectTooltip.Draw();
												
											}
										}
									}
									else if( _user->m_bMatchPRMode ) // 비무 홍보일때 
									{
										// 비무 홍보를 시작하면 비무홍보내용을 보여준다.
										if( _user->m_stringMatchPR[0] != 0 )
										{	
											if(_user->m_CharacterInfo.charactername[0] != 0)
											{																			
												TCHAR messagestring[256];
												memset( messagestring, 0, sizeof(messagestring) );	
												_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2892), _user->m_CharacterInfo.charactername );//"[%s님의 비무홍보]"
																								
												g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, _user->m_stringMatchPR, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCMATCHPRMODE );
												
												// 이름창 클릭시 PC 클릭과 동일하게 처리 
												ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
												if( ((scrnpos->x > tooltipxpos-(g_NPCNameWindow.m_WindowSize.cx>>1) ) && (scrnpos->x < tooltipxpos + (g_NPCNameWindow.m_WindowSize.cx>>1) ) &&
													(scrnpos->z > tooltipypos ) && (scrnpos->z < tooltipypos + g_NPCNameWindow.m_WindowSize.cy) ) )
												{			
													FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - _user->m_Position.x);
													FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - _user->m_Position.z);
													FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
													
													if( _distance < g_LastPickedObjectDistance )
													{		
														if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
														{
															if( g_LastPickedObjectType != _XPICKINGTARGET_NPC && !g_LastPickedObject )
															{
																g_LastPickedObjectDistance = _distance;
																g_LastPickedObjectType = _XPICKINGTARGET_PC;
																g_LastPickedObject = (_XItem*)_user;;
															}
														}			
													}
													
												}
											}
										}
									}//else if( _user->m_bMatchPRMode ) // 비무 홍보일때 
									else
									{
										// 상태효과에서 이름표 안보여주기 속성이면 draw하지 않음
										if(g_pLocalUser->m_Lock.notdrawusername == 0)
										{
											// 캐릭터 이름 보여주기
											if( _user->m_CharacterInfo.Get_gm_level() > 2 ) 
											{
												TCHAR namebuffer[128];
												memset(namebuffer, 0, sizeof(namebuffer));

												_snprintf( namebuffer, sizeof(namebuffer), _T("GM : %s"), _user->m_CharacterInfo.charactername );
												g_OperaterTooltip.SetToolTipStringEx_Noresize( tooltipxpos, tooltipypos, namebuffer, 0xFFFFE500 );
												g_OperaterTooltip.Draw();
											}
											else
											{
												int DrawPos = 0;
												int orgtype = 0, orindex = 0;
												drawPosX = 0;

#ifdef _XDEF_MASKSYSTEM_SHOW_NAME	// 복면을 써도 이름만 보여준다.
												bFaceMaskMode = FALSE;
#else
												bFaceMaskMode = _user->m_bFaceMaskMode;
#endif

												if( !bFaceMaskMode )
												{
#ifdef _XTS_FAME													
													if( _user->m_CharacterInfo.Get_Notoriety() > 0 || _user->m_CharacterInfo.Get_honor() > 0 )
													{
														g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, _user->m_CharacterInfo.charactername, D3DCOLOR_ARGB( 255, 230, 230, 230 ), 32, TRUE, FALSE );
														
														int drawwidth = 0; 
														if( _user->m_CharacterInfo.Get_honor() <= _user->m_CharacterInfo.Get_Notoriety() )
														{
															drawwidth = g_XBaseFont->GetWidth( g_FameStr[_user->m_CharacterInfo.Get_Notoriety()-1].m_InfamyName )+12;
															int ftxpos = g_WorldObjectTooltip.GetWindowPos().x - drawwidth;
															g_FameTooltip.SetToolTipStringEx( ftxpos, tooltipypos, g_FameStr[_user->m_CharacterInfo.Get_Notoriety()-1].m_InfamyName, D3DCOLOR_ARGB( 255, 241, 51, 51 ), 32, FALSE, FALSE );
														}
														else
														{
															drawwidth = g_XBaseFont->GetWidth( g_FameStr[_user->m_CharacterInfo.Get_honor()-1].m_FameName )+12;
															int ftxpos = g_WorldObjectTooltip.GetWindowPos().x - drawwidth;
															g_FameTooltip.SetToolTipStringEx( ftxpos, tooltipypos, g_FameStr[_user->m_CharacterInfo.Get_honor()-1].m_FameName, D3DCOLOR_ARGB( 255, 50, 100, 255 ), 32, FALSE, FALSE );
														}														
														
														g_FameTooltip.DrawNoEdge(5);
													}
													else
														g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, _user->m_CharacterInfo.charactername, D3DCOLOR_ARGB( 255, 230, 230, 230 ), 32, TRUE, FALSE );								
#else
													g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, _user->m_CharacterInfo.charactername, D3DCOLOR_ARGB( /*g_UsernameTooltipViewTimer*/255, 230, 230, 230 ), 32, TRUE, FALSE );
#endif													
												}

#ifdef _XTS_PK
												int bordercolor = 0;
												if(_user->m_bInPartyMember)
												{
													// 파티원
													bordercolor = 2;
												}
												else
												{
													if(_user->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
													{
														bordercolor = 3;
													}
													else if(g_pLocalUser->m_CharacterInfo.groupindex == _user->m_CharacterInfo.groupindex)
													{
														// 같은 문파
														bordercolor = 0;
													}
													else
													{
														if(g_pLocalUser->GetGroupType() == _XGROUPTYPE_WHITE)
														{
															// 자기자신이 백도
															if(_user->GetGroupType() == _XGROUPTYPE_BLACK)
																bordercolor = 1;
															else
																bordercolor = 4;
														}
														else if(g_pLocalUser->GetGroupType() == _XGROUPTYPE_BLACK)
														{
															// 자기자신이 흑도
															if(_user->GetGroupType() == _XGROUPTYPE_WHITE)
																bordercolor = 1;
															else 
																bordercolor = 4;
														}
														else if(g_pLocalUser->GetGroupType() == _XGROUPTYPE_NONE)
														{
															// 자기자신이 낭인
															bordercolor = 1;
														}
													}
												}

												if( !bFaceMaskMode )
												{
													if(_user->m_BattleMode == _XBATTLE_PK)
													{
														g_WorldObjectTooltip.SetTextColor(D3DCOLOR_ARGB(255, 200, 0, 0));
													}
													else
													{
														g_WorldObjectTooltip.SetTextColor(D3DCOLOR_ARGB(255, 230, 230, 230));
													}

													g_WorldObjectTooltip.DrawNoEdge(bordercolor);
												}
#else
												if( !_user->m_bInPartyMember )
												{
													BOOL bEnemy = FALSE;
													if(g_pLocalUser->GetGroupType() == _XGROUPTYPE_WHITE)
													{
														if(_user->GetGroupType() == _XGROUPTYPE_BLACK)
															bEnemy = TRUE;
													}
													else if(g_pLocalUser->GetGroupType() == _XGROUPTYPE_BLACK)
													{
														if(_user->GetGroupType() == _XGROUPTYPE_WHITE)
															bEnemy = TRUE;
													}
													g_WorldObjectTooltip.DrawNoEdge(bEnemy);
												}
												else
												{
													g_WorldObjectTooltip.DrawParty();
												}
#endif
												
											#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
												if( !bFaceMaskMode && strlen(_user->m_pLocalMnDInfomation.szMasterNickName) ) //사호가 있으면 사호를 찍는다
												{
													DrawPos -= 20;

													g_XBaseFont->SetColor(_XDEF_CHATMESSAGECOLOR_ITEM); //황금색

													g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
													g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, _user->m_pLocalMnDInfomation.szMasterNickName );
													g_XBaseFont->Flush();
													g_XBaseFont->DisableGrowMode();
												}
												else if( g_pLocalUser->m_CharacterInfo.m_NickName[0] != 0 )
											#else
												// 별호 보여주기
												if( !bFaceMaskMode && _user->m_CharacterInfo.m_NickName[0] != 0 )
											#endif
												{													
													DrawPos -= 20;
													switch(g_NickNameInfoTable[_user->m_CharacterInfo.m_CurrentNIckIndex].cType1) 
													{
													case 1:	// 특수
														g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 250, 160, 40 ));
														break;
													case 2:	// 일반
														{
															switch( g_NickNameInfoTable[_user->m_CharacterInfo.m_CurrentNIckIndex].cType2 )
															{
															case 1 :	// 별호
																g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 230, 200, 120 ));
																break;
															case 2 :	// 별칭
																g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 180, 180, 180 ));
																break;
															}
														}
														break;
													case 3:	// 고유
														g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 180, 180, 180 ));
														break;		
													default :
														g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 180, 180, 180 ));
														break;
													}				
													g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
													g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, _user->m_CharacterInfo.m_NickName );
													g_XBaseFont->Flush();
													g_XBaseFont->DisableGrowMode();
												}
												
												if( !bFaceMaskMode && _user->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
												{
                                                    if( strlen(_user->m_CharacterInfo.m_AlienceInfo.m_cName) ) //Author : 양희왕 //brief : 역시 바꿔준다
													//if( _user->m_CharacterInfo.m_AlienceInfo.m_cName[0] != 0 )	//영웅회 이름을 가지고 있는 경우
													{
														orgtype = 1;	// ally
														orindex = _user->m_CharacterInfo.m_AlienceInfo.m_iIndex;

														DrawPos -= 20;
														g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
														
														TCHAR orgName[512];
														memset(orgName, 0, sizeof(orgName) );
														

														// 등급이 없으면 등급을 설정해야 한다. 
														if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CLASS_NO_UNIT_ )
														{															
															_XUSER_ALIENCE_GLOBAL_INFO* tempInfo = g_Userlist.SearchAlienceInfo(_user->m_CharacterInfo.m_AlienceInfo.m_iIndex);
															if( tempInfo )
															{
																// 영웅단 단주이면 회주 아니면 부회주 이다
																if( _user->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
																{	
																	if( strcmp( _user->m_CharacterInfo.charactername, tempInfo->m_cMasterName ) == 0 ) 
																	{
																		_user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_; 
																	}
																	else 
																	{
																		_user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
																	}
																}
																else
																{				
																	_user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
																}
															}
															else 
															{
																_user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
															}
														}
														


														if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
														{
															_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3172), _user->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s회 회주]"
															g_XBaseFont->SetColor(0xFF8BC7F6);
															g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
														}
														else if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_ )
														{
															_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3173), _user->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s회 부회주]"
															g_XBaseFont->SetColor(0xFF8BC7F6);
															g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
														}
														else
														{
															_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3174), _user->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s]"
															g_XBaseFont->SetColor(0xFF8BC7F6);
															g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
														}
														g_XBaseFont->Flush();
														g_XBaseFont->DisableGrowMode();

														drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
													}	
													else
													{														
														_XUSER_ALIENCE_GLOBAL_INFO* fineUserInfo = g_Userlist.SearchAlienceInfo(_user->m_CharacterInfo.m_AlienceInfo.m_iIndex);
														
														if( fineUserInfo )
														{
                                                            if( strlen(fineUserInfo->m_cName) ) //Author : 양희왕 //brief : 역시 바꿔준다
															//if( fineUserInfo->m_cName[0] != 0 )
															{
																orgtype = 1;
																orindex = _user->m_CharacterInfo.m_AlienceInfo.m_iIndex;

																strcpy( _user->m_CharacterInfo.m_AlienceInfo.m_cName, fineUserInfo->m_cName );	
																
																DrawPos -= 20;
																g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );

                                                                //Author : 양희왕 //breif : 여기는 왜 등급 설정이 없나
                                                                // 등급이 없으면 등급을 설정해야 한다. 
														        if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CLASS_NO_UNIT_ )
														        {															
																    // 영웅단 단주이면 회주 아니면 부회주 이다
																    if( _user->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
																    {	
																	    if( strcmp( _user->m_CharacterInfo.charactername, fineUserInfo->m_cMasterName ) == 0 ) 
																	    {
																		    _user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_; 
																	    }
																	    else 
																	    {
																		    _user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
																	    }
																    }
																    else
																    {				
																	    _user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
																    }
														        }
																
																TCHAR orgName[512];
																memset(orgName, 0, sizeof(orgName) );
																if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
																{
																	_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3172), _user->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s회 회주]"
																	g_XBaseFont->SetColor(0xFF8BC7F6);
																	g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
																}
																else if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_ )
																{
																	_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3173), _user->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s회 부회주]"
																	g_XBaseFont->SetColor(0xFF8BC7F6);
																	g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
																}
																else
																{
																	_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_3174), _user->m_CharacterInfo.m_AlienceInfo.m_cName ); //"[%s]"
																	g_XBaseFont->SetColor(0xFF8BC7F6);
																	g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
																}
																g_XBaseFont->Flush();
																g_XBaseFont->DisableGrowMode();

																drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
															}
                                                            //Author : 양희왕 //brief : 이름이 없을 경우..
                                                            else
                                                            {
                                                                orgtype = 1;
																orindex = _user->m_CharacterInfo.m_AlienceInfo.m_iIndex;

																strcpy( _user->m_CharacterInfo.m_AlienceInfo.m_cName, fineUserInfo->m_cName );	
																
																DrawPos -= 20;
																g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );

                                                                //Author : 양희왕 //breif : 여기는 왜 등급 설정이 없나
                                                                // 등급이 없으면 등급을 설정해야 한다. 
														        if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CLASS_NO_UNIT_ )
														        {															
																    // 영웅단 단주이면 회주 아니면 부회주 이다
																    if( _user->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
																    {	
																	    if( strcmp( _user->m_CharacterInfo.charactername, fineUserInfo->m_cMasterName ) == 0 ) 
																	    {
																		    _user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_; 
																	    }
																	    else 
																	    {
																		    _user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
																	    }
																    }
																    else
																    {				
																	    _user->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
																    }
														        }
																
																TCHAR orgName[512];
																memset(orgName, 0, sizeof(orgName) );

																if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
																{
																	_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA)); // 이름없음
																	g_XBaseFont->SetColor(0xFF8BC7F6);
																	g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
																}
																else if( _user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_ )
																{
																	_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA));
																	g_XBaseFont->SetColor(0xFF8BC7F6);
																	g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
																}
																else
																{
																	_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA));
																	g_XBaseFont->SetColor(0xFF8BC7F6);
																	g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
																}
																g_XBaseFont->Flush();
																g_XBaseFont->DisableGrowMode();

																drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
                                                            }
														}
													}
												}
												else //영웅단 동맹에 가입되지 않았으므로 영웅단을 표시한다.
												{
													if( !bFaceMaskMode && _user->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
													{
														if( strlen(_user->m_CharacterInfo.m_OrganizationInfo.m_cName) )	// 영웅단 표시
														{
															orgtype = 0;
															orindex = _user->m_CharacterInfo.m_OrganizationInfo.m_iIndex;

															DrawPos -= 20;
															g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
															
															TCHAR orgName[512];
															memset(orgName, 0, sizeof(orgName) );
															if( _user->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
															{
																_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_2894), _user->m_CharacterInfo.m_OrganizationInfo.m_cName );//"[%s단 단주]"
																g_XBaseFont->SetColor(0xFF8BC7F6);
																g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
															}
															else
															{
																_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_NEW_2895), _user->m_CharacterInfo.m_OrganizationInfo.m_cName ); //"[%s단]"
																g_XBaseFont->SetColor(0xFF8BC7F6);
																g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
															}
															g_XBaseFont->Flush();
															g_XBaseFont->DisableGrowMode();

															drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
														}
                                                        else //Author : 양희왕 //breif : 영웅단 이름이 없을경우
                                                        {
                                                            orgtype = 0;
															orindex = _user->m_CharacterInfo.m_OrganizationInfo.m_iIndex;

															DrawPos -= 20;
															g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
															
															TCHAR orgName[512];
															memset(orgName, 0, sizeof(orgName) );

															if( _user->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
															{
																_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA)); //이름 없음
																g_XBaseFont->SetColor(0xFF8BC7F6);
																g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
															}
															else
															{
																_snprintf( orgName, sizeof(orgName), _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA));
																g_XBaseFont->SetColor(0xFF8BC7F6);
																g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+DrawPos, _XFONT_ALIGNTYPE_CENTER, orgName );
															}
															g_XBaseFont->Flush();
															g_XBaseFont->DisableGrowMode();

															drawPosX = g_XBaseFont->GetWidth(orgName) / 2;
                                                        }
													}
												}
#ifdef _XDEF_PARTY_NOTAVAILABLE
												// 자리 비움 표시
												if(_user->m_bOut)
												{
													g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 230, 230, 230));
													g_XBaseFont->PutsAlign(tooltipxpos, tooltipypos+20, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3391) );	//"<자리 비움>"
													g_XBaseFont->Flush();
												}
#endif
#ifdef _XDEF_CASTLEBATTLE
												if(bFaceMaskMode)
												{
													// 복면 착용시 orgtype, orgindex
													if(_user->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0)
													{
														orgtype = 1;
														orindex = _user->m_CharacterInfo.m_AlienceInfo.m_iIndex;
													}
													else if(_user->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
													{
														orgtype = 0;
														orindex = _user->m_CharacterInfo.m_OrganizationInfo.m_iIndex;
													}
												}

												if(g_pLocalUser->m_bCastleBattle && orindex > 0)
												{
													int attacktype = 0;
													g_pLocalUser->IsEnemyInCB(orgtype, orindex, attacktype);

													if(attacktype == 1 || attacktype == 2)
													{
														if(!bFaceMaskMode)
															g_pMark_Window->m_pImgAttackType[attacktype-1]->Draw(tooltipxpos-drawPosX-30-2, tooltipypos+DrawPos-9);
														else
															g_pMark_Window->m_pImgAttackType[attacktype-1]->Draw(tooltipxpos-18/*-drawPosX-30-2+64*/, tooltipypos+DrawPos-9-8);
													}

													if(attacktype > 0)	// attacktype == 0이면 장원전에 참가하지 않는 user
													{
														if(orgtype == 0)
														{
															if(_user->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
															{
																if(!bFaceMaskMode)
																	g_pMark_Window->m_pImgCaptain->Draw(tooltipxpos-drawPosX-30-2+6, tooltipypos+DrawPos-9-2-18);
																else
																	g_pMark_Window->m_pImgCaptain->Draw(tooltipxpos-18+6/*-drawPosX-30-2+64*/, tooltipypos+DrawPos-9-2-18-8);
															}
														}
														else if(orgtype == 1)
														{
															if(_user->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_)
															{
																if(!bFaceMaskMode)
																	g_pMark_Window->m_pImgCaptain->Draw(tooltipxpos-drawPosX-30-2+6, tooltipypos+DrawPos-9-2-18);	
																else
																	g_pMark_Window->m_pImgCaptain->Draw(tooltipxpos-18+6/*-drawPosX-30-2+64*/, tooltipypos+DrawPos-9-2-18-8);	
															}
														}
													} //if(attacktype > 0)
												}
#endif
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
/*												if(_user->m_nWinnerNickName > 0)
												{
													g_pRewardNick_Window->MoveWindow(tooltipxpos-drawPosX-30-2+6-68, tooltipypos+DrawPos-9-2-18-98);
												}*/
#endif
												
												// 이름창 클릭시 PC 클릭과 동일하게 처리 
												ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
												SIZE winsize = g_WorldObjectTooltip.GetWindowSize();
												if( scrnpos && // device 종료 시 scrnpos가 NULL이 되는 경우가 있음.
													((scrnpos->x > tooltipxpos-( winsize.cx>>1) ) && (scrnpos->x < tooltipxpos + (winsize.cx>>1) ) &&
													(scrnpos->z > tooltipypos ) && (scrnpos->z < tooltipypos + winsize.cy) ) )
												{			
													FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - _user->m_Position.x);
													FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - _user->m_Position.z);
													FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
													
													if( _distance < g_LastPickedObjectDistance )
													{		
														if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
														{
															if( g_LastPickedObjectType != _XPICKINGTARGET_NPC && !g_LastPickedObject )
															{
																g_LastPickedObjectDistance = _distance;
																g_LastPickedObjectType = _XPICKINGTARGET_PC;
																g_LastPickedObject = (_XItem*)_user;;
																
																/*
																CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
																if( _user->GetMotionClass() == _XACTION_DYING )
																{
																	eodapp->ReadyCursor( _XMOUSETYPE_REBIRTH );
																}
																else
																{
																	eodapp->ReadyCursor( _XMOUSETYPE_CHARACTORSELECT );
																}
																*/
															}
														}			
													}
													
												}
											}
										}
									}
									
	/*#else
									g_WorldObjectTooltip.SetToolTipStringEx( tooltipxpos, tooltipypos, _user->m_CharacterInfo.charactername, D3DCOLOR_ARGB( g_UsernameTooltipViewTimer, 230, 230, 230 ), 32, TRUE, FALSE );
									g_WorldObjectTooltip.Draw();
	#endif*/
								}
							}
							else
							{
								if( _user->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_OPEN && !_user->m_bMatchPRMode )
								{
									_user->m_ChatBalloonObject.Draw( tooltipxpos, tooltipypos );
									
									if( _user->m_CharacterInfo.Get_gm_level() > 2 ) 
									{
										TCHAR namebuffer[128];
										memset(namebuffer, 0, sizeof(namebuffer));

										_snprintf( namebuffer, sizeof(namebuffer), _T("GM : %s"), _user->m_CharacterInfo.charactername );
										g_OperaterTooltip.SetToolTipStringEx_Noresize( tooltipxpos, tooltipypos - g_XBaseFont->GetHeight(), namebuffer, 0xFFFFE500 );
										g_OperaterTooltip.Draw();
									}
								}
								else // 상점일때는..
								{
									if( _user->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
									{
										// 개인 상점을 열었을때는 상점 명을 그려준다.
										if( _user->m_PersonalStoreName[0] != 0 )
										{	
											if(_user->m_CharacterInfo.charactername[0] != 0)
											{											
												
												TCHAR messagestring[256];
												memset( messagestring, 0, sizeof(messagestring) );	
												_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2893), _user->m_CharacterInfo.charactername );//"[%s님의 노점]"
												
												TCHAR storenamestring[256];
												memset( storenamestring, 0, sizeof(storenamestring) );	
												int length = strlen( _user->m_PersonalStoreName );
												if( length > 24 )
												{
													if( _user->m_PersonalStoreName[24] & 0x80 ) //한글일때 2바이트 
													{
														int i = 0;
														while( i < 24 )
														{		
															if( _user->m_PersonalStoreName[i] & 0x80 )
																i+=2;
															else
																i+=1;
														}
														strncpy( storenamestring, _user->m_PersonalStoreName, sizeof(TCHAR)*i );	
														strcat( storenamestring, _T(".."));
													}
													else
													{
														strncpy( storenamestring, _user->m_PersonalStoreName, sizeof(TCHAR)*24 );	
														strcat( storenamestring, _T(".."));
													}
												}
												else
												{
													strncpy( storenamestring, _user->m_PersonalStoreName, sizeof(storenamestring) );
												}
												
												g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, storenamestring, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCPERSONALSTORE );
												
												// 이름창 클릭시 PC 클릭과 동일하게 처리 
												ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
												if( ((scrnpos->x > tooltipxpos-(g_NPCNameWindow.m_WindowSize.cx>>1) ) && (scrnpos->x < tooltipxpos + (g_NPCNameWindow.m_WindowSize.cx>>1) ) &&
													(scrnpos->z > tooltipypos ) && (scrnpos->z < tooltipypos + g_NPCNameWindow.m_WindowSize.cy) ) )
												{			
													FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - _user->m_Position.x);
													FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - _user->m_Position.z);
													FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
													
													if( _distance < g_LastPickedObjectDistance )
													{		
														if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
														{
															if( g_LastPickedObjectType != _XPICKINGTARGET_NPC && !g_LastPickedObject )
															{
																g_LastPickedObjectDistance = _distance;
																g_LastPickedObjectType = _XPICKINGTARGET_PC;
																g_LastPickedObject = (_XItem*)_user;
																
																/*
																CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
																eodapp->ReadyCursor( _XMOUSETYPE_PERSONALSTORE );
																*/
															}
														}			
													}
													
												}
											}
										}
										else if( _user->m_bMatchPRMode ) // 비무 홍보일때 
										{
											// 비무 홍보를 시작하면 비무홍보내용을 보여준다.
											if( _user->m_stringMatchPR[0] != 0 )
											{	
												if(_user->m_CharacterInfo.charactername[0] != 0)
												{																			
													TCHAR messagestring[256];
													memset( messagestring, 0, sizeof(messagestring) );	
													_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2892), _user->m_CharacterInfo.charactername ); //"[%s님의 비무홍보]"
													
													g_NPCNameWindow.Draw( tooltipxpos, tooltipypos, _user->m_stringMatchPR, messagestring, FALSE, _XNPC_HAVEQUEST_NONE, _XNPC_JOBCLASS_SHOP, _XNPCNAMEMODE_PCMATCHPRMODE );
													
													// 이름창 클릭시 PC 클릭과 동일하게 처리 
													ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
													if( ((scrnpos->x > tooltipxpos-(g_NPCNameWindow.m_WindowSize.cx>>1) ) && (scrnpos->x < tooltipxpos + (g_NPCNameWindow.m_WindowSize.cx>>1) ) &&
														(scrnpos->z > tooltipypos ) && (scrnpos->z < tooltipypos + g_NPCNameWindow.m_WindowSize.cy) ) )
													{			
														FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - _user->m_Position.x);
														FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - _user->m_Position.z);
														FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
														
														if( _distance < g_LastPickedObjectDistance )
														{		
															if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
															{
																if( g_LastPickedObjectType != _XPICKINGTARGET_NPC && !g_LastPickedObject )
																{
																	g_LastPickedObjectDistance = _distance;
																	g_LastPickedObjectType = _XPICKINGTARGET_PC;
																	g_LastPickedObject = (_XItem*)_user;;
																}
															}			
														}
														
													}
												}
											}
										}//else if( _user->m_bMatchPRMode ) // 비무 홍보일때 
									}

								}//상점일때  
							}
						}
					} // if(!_user->Get_m_bInvisible())
				}
			}
		}
	}

	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);		
	gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
	dwLastTime = g_LocalSystemTime;
}

void _XUserList::RemoveEffectInstance(int templateid)
{
	_XUser* _user;
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		_user = (*it).second;
		if( _user->m_ModelDescriptor.m_EffectManager )
		{
			_user->m_ModelDescriptor.m_EffectManager->DeleteInstance( templateid );
		}
	}
}


int CompareUserDistance(const void *pArg1, const void *pArg2)
{
	_XUser** o1 = (_XUser**)pArg1;
	_XUser** o2 = (_XUser**)pArg2;
	return ( (o1[0]->m_ModelDescriptor.m_CameraDistance < o2[0]->m_ModelDescriptor.m_CameraDistance ) ? 1 : -1);
}


void _XUserList::CheckInsideFrustum( void )
{
	m_RenderUserZSortListCurrentLength = 0;

	_XUser* pUser;
	for (smdef_XUser::iterator it = m_smUserList.begin(); it != m_smUserList.end(); ++ it)
	{
		pUser = (*it).second;
		FLOAT dx = (pUser->m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		FLOAT dz = (pUser->m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
		pUser->m_ModelDescriptor.m_CameraDistance = _XDistance( dx , dz );
		
		pUser->m_CheckInFrustum = 
			( (pUser->m_ModelDescriptor.m_CameraDistance < g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_USER_CULLINGRADIUS) &&
			( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pUser->m_Position, 3.5f ) != NOT_VISIBLE ) );

		if( pUser->m_CheckInFrustum && !pUser->Get_m_bInvisible() )
		{
			if( m_RenderUserZSortListCurrentLength >= m_RenderUserZSortListLength )
			{
				m_RenderUserZSortListLength	+= 50;			
				_XUser** pNewRenderUserZSortList = new _XUser*[m_RenderUserZSortListLength];
				memcpy( pNewRenderUserZSortList, m_pRenderUserZSortList, sizeof(_XUser*) * m_RenderUserZSortListLength - 50 );
				SAFE_DELETE_ARRAY( m_pRenderUserZSortList );
				m_pRenderUserZSortList = pNewRenderUserZSortList;
			}

			m_pRenderUserZSortList[m_RenderUserZSortListCurrentLength] = pUser;
			m_RenderUserZSortListCurrentLength++;
		}
	}

	if( m_RenderUserZSortListCurrentLength > 1 )
	{
		qsort((void*)m_pRenderUserZSortList, m_RenderUserZSortListCurrentLength, sizeof(_XUser*), CompareUserDistance);
	}
}

void _XUserList::AddOrganizationInfo( _XUSER_ORGANIZATION_GLOBAL_INFO* info )
{
	m_sOrganizationInfoList.insert(smdef_OtganizationInfo::value_type(info->m_iIndex, info));
}

_XUSER_ORGANIZATION_GLOBAL_INFO* _XUserList::SearchOrganizationInfo(int index)
{
	smdef_OtganizationInfo::iterator it = m_sOrganizationInfoList.find(index);
	if (it != m_sOrganizationInfoList.end())
	{
		return (*it).second;
	}
	return NULL;
}



void _XUserList::AddAlienceInfo( _XUSER_ALIENCE_GLOBAL_INFO* info )
{
	m_sAlienceInfoList.insert(smdef_AlienceInfo::value_type(info->m_iIndex, info));
}

_XUSER_ALIENCE_GLOBAL_INFO*	_XUserList::SearchAlienceInfo(int index)
{
	smdef_AlienceInfo::iterator it = m_sAlienceInfoList.find(index);
	if (it != m_sAlienceInfoList.end())
	{
		return (*it).second;
	}
	return NULL;
}
