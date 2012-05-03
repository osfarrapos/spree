// XMob.cpp: implementation of the _XMob class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "XMob.h"
#include "XProc_MainGameDef.h"
#include "EODEXTDEF_GlobalObject.h"
#include "SoundEffectList.h"
#include "XProc_ProcessDefine.h"
#include "XParticleIDDefine.h"
#include "XWindowObjectDefine.h"
#include "XTextureController.h"

//////////////////////////////////////////////////////////////////////
// Mob item class

int				 g_CurrentCachedMobcount = 0;
_MOB_ATTRIBTABLE g_MobAttribTable[_XDEF_MODELCLASSCOUNT_MOB];
_XMobMotionExistTable g_MobMotionExistTable[_XDEF_MODELCLASSCOUNT_MOB];

int g_RenderMobCount = 0;

_XMob::_XMob()
{
}

_XMob::~_XMob()
{
}

void _XMob::InitializeData()
{		
	m_MobID			= 0;
	m_NextTransformType = m_MobType	= 0;
	m_TransformStep	= _XMONSTERTRANSFORMSTEP_NONE;
	m_RestoreOrgMonsterType = FALSE;
		
	D3DXMatrixIdentity( &m_MobMatrix );
	m_MoveVector = m_EndPosition = m_Position = D3DXVECTOR3( 0,0,0 );
	m_RotateAngle	= 0.0f;	
	m_fMoveSpeed	= 0.0f;
	m_PrevTime		= 0.0f;
	
	m_AlphaLevel	= 0;
	m_DieToDeleteTimer = 0;
	m_HealthPercentage	= 0;
	m_DisplayHealthPercentage = 0;
		
	m_LastRecivedMessageTime = 0l;
	
	m_Action.action			= _XMOBACTION_IDLE;
	m_Action.idlemode		= _XMOBACTIONNUMBER_IDLE;
	m_Action.movemode		= _XMONSTERMOVETYPE_WALK;
	m_Action.attacktype		= _XMONSTERATTACKTYPE_ATTACK1;
	m_Action.dietype		= _XMONSTERDIETYPE_DIE2;
	m_Action.skilltype		= _XMONSTERSKILLTYPE_SKILL1;

	m_PrevMotion			= 0;
	
	m_AttackTargetObjectType		= _XPICKINGTARGET_NONE;
	m_AttackTarget	= NULL;
	m_AttackHitValue= 0.0f;
	m_KillSignal    = _XDEF_MOB_KILLSIGNAL_STOP;
	m_AttackInfo	= -1;

	m_InsideFrustum	= TRUE;
	
	m_bImpact		= FALSE;
	m_ImpactValue	= 0;
	m_OriginPosition = m_ViewDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_DieSignalReceiveTime = 0;
	
	m_bClientMob			= FALSE;
	m_bClientDNPC			= FALSE;
	m_bShowMobName			= FALSE;
	m_bShowBoldMobName		= FALSE;
	m_bShowHPGauge			= FALSE;
	m_ConditionList			= 0;
	m_SpeedRate				= 0;
	
	m_ModelDescriptor.m_SwordEffectInitialized = FALSE;
	
	m_ModelDescriptor.SetLockEndFrame(FALSE);

	int gaugeimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_party.tga" );
				
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 600, 350 }, { 91, 5 },	
			gaugeimageindex, -1, &g_MainInterfaceTextureArchive
	};
	
	m_MonsterMiniHpBorder.Create( gaugestruct );
	m_MonsterMiniHpBorder.SetImageClipRect( _XGAUGEIMAGE_BORDER, 157, 63, 248, 68 );
	m_MonsterMiniHpBorder.SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 157, 63, 248, 68 );
	m_MonsterMiniHpBorder.SetBackGroundDraw(FALSE);
	m_MonsterMiniHpBorder.SetViewDelayGauge( TRUE, 20 );
	m_MonsterMiniHpBorder.SetFactorDraw(FALSE);
	
	m_imageMiniHpBackBorder.Create( 0, 0, 108, 9, &g_MainInterfaceTextureArchive, gaugeimageindex );	
	m_imageMiniHpBackBorder.SetClipRect( 148, 77, 256, 86  );
	m_imageMiniHpBackBorder.ShowWindow(TRUE);
}

void _XMob::DestroyData()
{
	ReleaseState();
	m_ModelDescriptor.Reset();

	m_MonsterMiniHpBorder.DestroyWindow();
	m_imageMiniHpBackBorder.DestroyWindow();

	VIEWPACKETLOG
		_XDWPACKETLOG( "Removed monster [ id:%d  type:%d ]", m_MobID, m_MobType );
}

void _XMob::Create( unsigned short mobid, unsigned short mobtype, D3DXVECTOR3 position, 
				    FLOAT angle, unsigned char health, int defaultmotion)
{	
	m_MobID						= mobid;
	m_NextTransformType			= m_MobType	= mobtype;
	m_MoveVector				= D3DXVECTOR3( 0,0,0 );
	m_EndPosition				= D3DXVECTOR3( 0,0,0 );
	
	m_Position					= position;
	m_EndPosition				= position;
	m_RotateAngle				= angle;
	m_HealthPercentage			= health;
	m_DisplayHealthPercentage	= health;

	m_Action.action				= _XMOBACTION_IDLE;
	m_Action.idlemode			= _XMOBACTIONNUMBER_IDLE;
	m_Action.movemode			= _XMONSTERMOVETYPE_WALK;
	m_Action.attacktype			= _XMONSTERATTACKTYPE_ATTACK1;
	m_Action.dietype			= _XMONSTERDIETYPE_DIE2;
	m_Action.skilltype			= _XMONSTERSKILLTYPE_SKILL1;
	m_PrevMotion				= 0;

	m_AttackTargetObjectType	= _XPICKINGTARGET_NONE;
	m_AttackTarget				= NULL;
	m_AttackHitValue			= 0.0f;
	m_KillSignal				= _XDEF_MOB_KILLSIGNAL_STOP;
	m_AttackInfo				= -1;

	m_bClientMob				= FALSE;
	m_bClientDNPC				= FALSE;
	
	m_bImpact					= FALSE;
	m_ImpactValue				= 0;
	m_OriginPosition			= m_ViewDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_DieSignalReceiveTime		= 0;

	memset(&m_AttackResult, 0, sizeof(_XMOB_ATTACKRESULT));

	m_ModelDescriptor.ReserveModelStack( 1 );
	m_ModelDescriptor.LinkBipedObject( &g_MOBBIPEDMODEL[mobtype-1] );
	m_ModelDescriptor.SetMeshModel( 0, g_MOBMODEL[mobtype-1] );
	m_ModelDescriptor.SetCurMotion( defaultmotion );
	m_ModelDescriptor.PlayAnimation();
	m_ModelDescriptor.SetShadowDecalInfo( 0.65f, 0.85f, 1.85f );	

	CreateEffectManager();
}

void _XMob::CreateEffectManager( void )
{
	int swordeffecttracecount = 0;
	if( m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData )
	{
		swordeffecttracecount = m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData->m_Pointcount;
	}

	m_ModelDescriptor.InitEffectManager(swordeffecttracecount);	
}

void _XMob::StartImpactEffect(void)
{
	if( g_MobAttribTable[m_MobType].scripttype < 81 || g_MobAttribTable[m_MobType].scripttype > 85 ) // LPS가 아닌 몹
	{
		m_bImpact = TRUE;
		m_ImpactValue = 1;
		m_OriginPosition = m_Position;
		m_ViewDirection.x = g_pLocalUser->m_Position.x - m_Position.x;
		m_ViewDirection.y = 1.0f;
		m_ViewDirection.z = g_pLocalUser->m_Position.z - m_Position.z;
	}
}

void _XMob::SetImpactPosition(void)
{
	if(m_bImpact)
	{
		VIEWBATTLELOG
			_XDWINPRINT("==== mob impact set =====");

		m_Position.x = m_OriginPosition.x + m_ViewDirection.x*(-0.1f);
		m_Position.z = m_OriginPosition.z + m_ViewDirection.z*(-0.1f);
		
		if(m_ImpactValue > 1 && m_ImpactValue <= 2)
		{
			m_Position += m_ViewDirection*(0.05f);
		}
		if(m_ImpactValue > 2)
		{
			m_bImpact = FALSE;
			m_Position = m_OriginPosition;
			m_ViewDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_ImpactValue = 0;
		}
		m_ImpactValue++;
	}
}

void _XMob::ResetImpactEffect(void)
{
	if(m_bImpact)
	{
		m_bImpact = FALSE;
		m_ImpactValue = 0;
		m_Position = m_OriginPosition;
		m_ViewDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		UpdatePosition();
	}
}

void _XMob::ResetAttackTarget(void)
{
	m_AttackTarget = NULL;
	m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
}


#define _XDEF_MOBENDMOVING_EPSILON	0.3f
#define _XDEF_MOBTRANSFORMTIME		1000

void _XMob::Process( void )
{
	if( m_NextTransformType != m_MobType )
	{
		if( m_TransformStep == _XMONSTERTRANSFORMSTEP_NONE )
		{
			m_TransformStep = _XMONSTERTRANSFORMSTEP_START;
			m_TranformTimer = g_LocalSystemTime;
			
			SetAnimation(_XMOBACTION_DIE);
			if(this == (_XMob*)g_LastPickedObject) g_LastPickedObject = NULL;
			if(this == (_XMob*)g_pLocalUser->m_AttackTarget) g_pLocalUser->ResetAttackTarget();
			if(this == (_XMob*)g_pLocalUser->m_ViewTarget) g_pLocalUser->ResetViewTarget();
			
			_XPlaySoundEffect(ID_SR_INTERFACE_TURN_POINT00_WAV, this->m_Position );
		}
	}

	if( m_Action.action == _XMOBACTION_IDLE ) // 대기모드에서는 사운드 플레이를 제한한다.
	{
		if( g_MobAttribTable[m_MobType].soundplayed )
		{
			bool playsound = false;
			m_ModelDescriptor.Advance( playsound, 0 );
		}
		else
		{
			bool playsound = true;
			m_ModelDescriptor.Advance(playsound, 0);
			if( !playsound )
				g_MobAttribTable[m_MobType].soundplayed = true;
		}
	}
	else
	{
		bool playsound = true;
		m_ModelDescriptor.Advance( playsound, 0 );
	}

	if( m_Action.action == _XMOBACTION_ATTACK )
	{
		if( (fabs( m_Position.x - m_EndPosition.x ) <= _XDEF_MOBENDMOVING_EPSILON) &&
			(fabs( m_Position.z - m_EndPosition.z ) <= _XDEF_MOBENDMOVING_EPSILON) )
		{
			m_PrevTime = 0.0f;
			m_MoveVector.x = m_MoveVector.y = m_MoveVector.z = 0.0f;
			m_Position = m_EndPosition;
		}
		else
		{
			MoveNextPostion(TRUE);
		}

/*		m_PrevTime = 0.0f;
		m_MoveVector.x = m_MoveVector.y = m_MoveVector.z = 0.0f;
		
		if( (m_EndPosition.x != m_Position.x) ||
			(m_EndPosition.z != m_Position.z) )
		{
			m_Position = m_EndPosition;			
		}
*/
		if(m_AttackTarget)
		{
			if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				_XMob* pTarget = (_XMob*)m_AttackTarget;

				// 공격일때는 항상 타겟을 바라본다.
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pTarget->m_Position);
				UpdatePosition();

				if(pTarget->m_Action.action != _XMOBACTION_DIE)
				{
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME) // 2005-4-26 kerygma 옮김.
					{
						D3DXMATRIX offsetmat = pTarget->m_ModelDescriptor.m_Position;

						if(g_ZeroLevelMode)
							offsetmat._42 += (0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f));
						else
						{
							BOOL bIntersect = FALSE;
							D3DXVECTOR3 pickedpoint, originpos;
							D3DXVECTOR3 direction = pTarget->m_Position - m_Position;
							originpos = m_Position;
							originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f);

							bIntersect = pTarget->m_ModelDescriptor.InterOBBvsRay(originpos, direction, pTarget->m_Position, pTarget->m_RotateAngle, &pickedpoint);
							if(!bIntersect)
								offsetmat._42 += 0.98f;
							else
							{
								offsetmat._41 = pickedpoint.x;
								offsetmat._42 = pickedpoint.y;
								offsetmat._43 = pickedpoint.z;
							}
						}

						switch(m_AttackInfo)
						{
						case 0 :
						case 5 :
							{
								// Hit
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [HIT]", this->m_MobType, pTarget->m_MobType);

								pTarget->m_AttackTarget = this;
								pTarget->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;

								PlayAttackSound();
								pTarget->SetBlowMotion();

								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
																										_XDEF_COMMONID_HIT2, &offsetmat );
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [CRITICAL]", this->m_MobType, pTarget->m_MobType);

								pTarget->m_AttackTarget = this;
								pTarget->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;

								PlayAttackSound();
								pTarget->SetBlowMotion();

								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
																									   _XDEF_COMMONID_HIT2, &offsetmat );
								}
							}
							break;
						case 2 :
							{
								// Evation
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [EVATION]", this->m_MobType, pTarget->m_MobType);
							}
							break;
						case 3 :
							{
								// Block
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [BLOCK]", this->m_MobType, pTarget->m_MobType);

								PlayAttackSound();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
																										_XDEF_COMMONID_HIT2, &offsetmat );
								}
							}
							break;
						case 4 :
							{
								// Shock
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [SHOCK]", this->m_MobType, pTarget->m_MobType);

								pTarget->m_AttackTarget = this;
								pTarget->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;

								PlayAttackSound();
								pTarget->SetBlowMotion();
								
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
																										_XDEF_COMMONID_HIT2, &offsetmat );
								}
							}
							break;
						default :
							break;
						}
						if(m_AttackHitValue < 0.0f)
						{
							pTarget->SetAnimation(_XMOBACTION_DIE);
							pTarget->m_ModelDescriptor.SetLockEndFrame(TRUE);
							m_AttackHitValue = 0.0f;
						}
					} // if Command frame
				} // if(pTarget->m_Action != _XMOBACTION_DIE)
			} // if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			else if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
			{
				if(g_pLocalUser->GetMotionClass() != _XACTION_DIE)
				{
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, g_pLocalUser->m_Position);
					UpdatePosition();

					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME) // 2005-4-26 kerygma 옮김.
					{
						D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;

						if( g_ZeroLevelMode )
							offsetmat._42 += (0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f));
						else
						{
							BOOL bIntersect = FALSE;
							D3DXVECTOR3 pickedpoint, originpos;
							D3DXVECTOR3 direction = g_pLocalUser->m_Position - m_Position;
							originpos = m_Position;
							originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f);
							
							bIntersect = g_pLocalUser->m_ModelDescriptor.InterOBBvsRay(originpos, direction, g_pLocalUser->m_Position, g_pLocalUser->m_RotateAngle, &pickedpoint);
							if(!bIntersect)
							{
								offsetmat._42 += 0.98f;
							}
							else
							{
								offsetmat._41 = pickedpoint.x;
								offsetmat._42 = pickedpoint.y;
								offsetmat._43 = pickedpoint.z;
							}
						}

						switch(m_AttackInfo)
						{
						case 0 :
						case 5 :
							{
								// Hit
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To LocalUser [HIT]");

								PlayAttackSound();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To LocalUser [CRITICAL]");

								PlayAttackSound();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 2 :
							{
								// Evation
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster [EVATION]");
							}
							break;
						case 3 :
							{
								// Block
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster [BLOCK]");

								PlayAttackSound();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 4 :
							{
								// Shock
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Monster [SHOCK]");

								PlayAttackSound();

								g_pLocalUser->SetBlowMotion();

								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						default :
							break;
						}
					} // if command frame
				} // if(g_pLocalUser->GetMotionClass() != _XACTION_DIE)
			} // if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
			else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
			{
				_XUser* pTarget = (_XUser*)m_AttackTarget;

				if(pTarget->GetMotionClass() != _XACTION_DIE)
				{
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pTarget->m_Position);
					UpdatePosition();

					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME) // 2005-4-26 kerygma 옮김.
					{
						D3DXMATRIX offsetmat = pTarget->m_ModelDescriptor.m_Position;

						if( g_ZeroLevelMode )
							offsetmat._42 += (0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f));
						else
						{
							BOOL bIntersect = FALSE;
							D3DXVECTOR3 pickedpoint, originpos;
							D3DXVECTOR3 direction = pTarget->m_Position - m_Position;
							originpos = m_Position;
							originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f);
							
							bIntersect = pTarget->m_ModelDescriptor.InterOBBvsRay(originpos, direction, pTarget->m_Position, pTarget->m_RotateAngle, &pickedpoint);
							
							if(!bIntersect)
							{
								offsetmat._42 += 0.98f;
							}
							else
							{
								offsetmat._41 = pickedpoint.x;
								offsetmat._42 = pickedpoint.y;
								offsetmat._43 = pickedpoint.z;
							}
						}

						switch(m_AttackInfo)
						{
						case 0 :
						case 5 :
							{
								// Hit
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other User[HIT]");

								PlayAttackSound();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
																									   _XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [CRITICAL]");

								PlayAttackSound();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
																										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 2 :
							{
								// Evation
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [EVATION]");
							}
							break;
						case 3 :
							{
								// Block
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [BLOCK]");

								PlayAttackSound();
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
																										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 4 :
							{
								// Shock
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [SHOCK]");

								PlayAttackSound();
								pTarget->SetBlowMotion();
								
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									if(pTarget->m_ModelDescriptor.m_EffectManager)
										pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
																										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						default :
							break;
						}
					} // if command frame
				} // if(pTarget->GetMotionClass() != _XACTION_DIE)
			}// else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
		} // has attack target

		// 한대 때리고 동작 끝나면 대기 동작으로 전환.
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			ProcessTargetMobDie();

			ResetAttackTarget();

			if( !g_ZeroLevelMode && !g_pLocalUser->m_bVirtualMode )
				m_AttackHitValue = 0.0f;
			m_AttackInfo = -1;
			if( g_ZeroLevelMode )
				this->SetCharacterMobAnimation(_XMOBACTION_IDLE);
			else
				this->SetAnimation(_XMOBACTION_IDLE);
		}
		
		SetImpactPosition();
		UpdatePosition();		
	}
	else if( m_Action.action == _XMOBACTION_MOVE )
	{
		if( (fabs( m_Position.x - m_EndPosition.x ) <= _XDEF_MOBENDMOVING_EPSILON) &&
			(fabs( m_Position.z - m_EndPosition.z ) <= _XDEF_MOBENDMOVING_EPSILON) )
		{
			m_MoveVector.x = m_MoveVector.y = m_MoveVector.z = 0.0f;
			m_Position = m_EndPosition;
			if( g_ZeroLevelMode )
				this->SetCharacterMobAnimation(_XMOBACTION_IDLE);
			else
				this->SetAnimation(_XMOBACTION_IDLE);
		}
		else
		{
			MoveNextPostion();
		}

		SetImpactPosition();
		UpdatePosition();
	}
	else if( m_Action.action == _XMOBACTION_IDLE )
	{
/*		if( (m_EndPosition.x != m_Position.x) ||
			(m_EndPosition.z != m_Position.z) )
		{
			m_Position = m_EndPosition;
		}
*/

		if( (fabs( m_Position.x - m_EndPosition.x ) <= _XDEF_MOBENDMOVING_EPSILON) &&
			(fabs( m_Position.z - m_EndPosition.z ) <= _XDEF_MOBENDMOVING_EPSILON) )
		{
			m_Position = m_EndPosition;
		}
		else
		{
			_XDWINPRINT("[Monster/Process] Mob[Type:%d ID:%d] Mob Idle인데 Position/Endposition 같지 않음 current[x:%f z:%f]  end[x:%f  z:%f]", m_MobType, m_MobID, m_Position.x, m_Position.z, m_EndPosition.x, m_EndPosition.z);
			// 위치 보정
			MoveNextPostion(TRUE);
			this->SetAnimation(_XMOBACTION_MOVE);
		}
		
		SetImpactPosition();
		UpdatePosition();
	}
	else if(m_Action.action == _XMOBACTION_BLOW)
	{
		// 한대 얻어 맞고 동작 끝나면 대기 동작으로 전환.
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			ResetAttackTarget();

			if(m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP)
			{
				if( g_ZeroLevelMode )
				this->SetCharacterMobAnimation(_XMOBACTION_IDLE);
			else
				this->SetAnimation(_XMOBACTION_IDLE);
			}
		}

		SetImpactPosition();
		UpdatePosition();
	}
	else if(m_Action.action == _XMOBACTION_SKILL)
	{
		// 스킬 애니매이션 끝나면 대기 동작으로 전환
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP)
			{
				if( g_ZeroLevelMode )
					this->SetCharacterMobAnimation(_XMOBACTION_IDLE);
				else
					this->SetAnimation(_XMOBACTION_IDLE);
			}
		}
		
		SetImpactPosition();
		UpdatePosition();
	}
	else if(m_Action.action == _XMOBACTION_DIE)
	{
		if( m_TransformStep == _XMONSTERTRANSFORMSTEP_START )
		{
			if( (g_LocalSystemTime - m_TranformTimer) > _XDEF_MOBTRANSFORMTIME )
			{
				g_Moblist.ReleaseReferenceModel(m_MobType);
				g_Moblist.AddReferenceModel(m_NextTransformType);

				m_MobType = m_NextTransformType;
				m_TranformTimer = 0;
				
				//m_ModelDescriptor.ReserveModelStack(1);
				m_ModelDescriptor.LinkBipedObject(&g_MOBBIPEDMODEL[m_MobType]);
				m_ModelDescriptor.SetMeshModel(0, g_MOBMODEL[m_MobType]);
				ResetAnimationState();
				
				m_Action.action =_XMOBACTION_IDLE; // 현재 죽기모드이므로 강제로 IDLE 상태 적용
				
				this->SetAnimation(_XMOBACTION_IDLE);
				 
				m_ModelDescriptor.PlayAnimation(TRUE);
				UpdatePosition();
				
				m_KillSignal = _XDEF_MOB_KILLSIGNAL_STOP;
				m_TransformStep = _XMONSTERTRANSFORMSTEP_NONE;
				
				if(m_ModelDescriptor.m_EffectManager)
				{
					if( this->m_MobType == 65 ) // 거산투영일때 변신 Effect 예외 처리 
					{
						m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
							_XDEF_MONSTERID_TM29, &m_ModelDescriptor.m_Position, NULL, FALSE );
					}
					else
					{
						m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
											_XDEF_PLAYERID_TRANSFORM, &m_ModelDescriptor.m_Position, NULL, FALSE );
					}
				}

				if( !m_RestoreOrgMonsterType )
				{
					if(m_ModelDescriptor.m_EffectManager)
					{
						if( this->m_MobType == 629 ) // 거산투영 변신 후 Effect 예외처리 
						{
							m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
								_XDEF_MONSTERID_TM31, &m_ModelDescriptor.m_Position, NULL, TRUE );
						}
						else
						{						
							m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
											_XDEF_MONSTERID_MOB_CON, &m_ModelDescriptor.m_Position, NULL, TRUE );
						}
					}
				}
			}
		}
		else
		{
			if(m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP)
			{
				UpdatePosition();
			}

			// 죽기 동작이 끝나면 사라지는 애니메이션 시작
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP)
				{
					m_KillSignal = _XDEF_MOB_KILLSIGNAL_START;
					m_AlphaLevel = 255;
				}
			}
		}
	}
	else if(m_Action.action == _XMOBACTION_ENTERING)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if( (fabs( m_Position.x - m_EndPosition.x ) <= _XDEF_MOBENDMOVING_EPSILON) &&
				(fabs( m_Position.z - m_EndPosition.z ) <= _XDEF_MOBENDMOVING_EPSILON) )
			{
				m_MoveVector.x = m_MoveVector.y = m_MoveVector.z = 0.0f;
				m_Position = m_EndPosition;
				if( g_ZeroLevelMode )
					this->SetCharacterMobAnimation(_XMOBACTION_IDLE);
				else
					this->SetAnimation(_XMOBACTION_IDLE);
			}
			else
			{
				// 위치 보정
				_XDWINPRINT("[Monstser/Process] Action Entering  [type:%d  id:%d] current[x:%f  z:%f]  next[x:%f  z:%f]", m_MobType, m_MobID, m_Position.x, m_Position.z, m_EndPosition.x, m_EndPosition.z);

				MoveNextPostion(TRUE);

				if( g_ZeroLevelMode )
					this->SetCharacterMobAnimation(_XMOBACTION_IDLE);
				else
					this->SetAnimation(_XMOBACTION_MOVE);
//					this->SetAnimation(_XMOBACTION_IDLE);

				UpdatePosition();
			}			
		}
	}

	if(g_LocalSystemTime - m_DieSignalReceiveTime >= 3000 && m_DieSignalReceiveTime > 0)
	{
		SetAnimation(_XMOBACTION_DIE);
		m_ModelDescriptor.SetLockEndFrame(TRUE);
	}
}

void _XMob::ProcessTargetMobDie(void)
{
	if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
	{
		if(m_AttackHitValue < 0.0f)
		{
			_XMob* pTarget = (_XMob*)m_AttackTarget;
			if(pTarget)
			{
				if(pTarget->m_Action.action != _XMOBACTION_DIE)
				{
					pTarget->SetAnimation(_XMOBACTION_DIE);
					pTarget->m_ModelDescriptor.SetLockEndFrame(TRUE);
				}
				m_AttackHitValue = 0.0f;
			}
		}
	}
}

void _XMob::PlayAttackSound(void)
{
	if(!m_AttackTarget)
		return;

	D3DXVECTOR3 position;
	if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
	{
		position = g_pLocalUser->m_Position;
	}
	else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
	{
		position = ((_XUser*)m_AttackTarget)->m_Position;
	}
	else if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
	{
		position = ((_XMob*)m_AttackTarget)->m_Position;
	}


	int weaponmaterial = g_MobAttribTable[m_MobType].weaponmaterial;
	if( weaponmaterial < g_MobWeaponSoundCount )
	{
		_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
	}
}

#define _XDEF_MOBFADEOUT_RATE		8
#define _XDEF_MOBFADEIN_RATE		6
#define _XDEF_MOBDIETODELETETIMER	1500

void _XMob::Render( void )
{	
	m_ModelDescriptor.m_NoAnimationAndPresentPipeline = !m_InsideFrustum;

	if( m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP ) 
	{
		m_ModelDescriptor.Animation();
	}
	else // 죽기시 정지 동작 별도 처리
	{		
		//m_Action.action = _XMOBACTION_DIE;
		//m_ModelDescriptor.SetCurMotion( _XMOBACTIONNUMBER_DIE, FALSE );
		m_ModelDescriptor.m_EventHandler.soundplayed[0] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[1] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[2] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[3] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[4] = 1;

		_LPXM_MOTION pMotion = NULL;
		if( m_Action.dietype == _XMONSTERDIETYPE_DIE1 )
		{
			pMotion = m_ModelDescriptor.m_pBipedController->GetMotion( g_MobMotionExistTable[m_MobType].motion_die1 );
		}
		else if( m_Action.dietype == _XMONSTERDIETYPE_DIE2 )
		{
			pMotion = m_ModelDescriptor.m_pBipedController->GetMotion( g_MobMotionExistTable[m_MobType].motion_die2 );
		}
		
		if(pMotion)
		{			
			m_ModelDescriptor.Animation(pMotion->EndFrame-1);
		}
		else
		{
			return;
		}
	}
		
	if( m_ModelDescriptor.m_CameraDistance < 12.0f && m_InsideFrustum )
	{
		if( m_KillSignal != _XDEF_MOB_KILLSIGNAL_START && ( m_AlphaLevel >= 255 ) )
			m_ModelDescriptor.RenderShadow();
	}	
	
	if( m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP ) // 몹이 처음 등장했을 경우 페이드 아웃 처리
	{
		if( m_AlphaLevel < 255 )
		{						
			m_AlphaLevel+=_XDEF_MOBFADEOUT_RATE;
			if( m_AlphaLevel > 255 ) m_AlphaLevel = 255;

			/*g_RenderAlphaBlendFlag = TRUE;
			g_RenderAlphaBlendTextureFactor = ( (m_AlphaLevel<<24) | 0xFFFFFF );
			g_LastUsedRenderStyle = NULL;*/
			m_ModelDescriptor.RenderMob( this );
			/*g_RenderAlphaBlendTextureFactor = 0xFFFFFFFF;
			g_RenderAlphaBlendFlag = FALSE;
			gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE); */
		}
		else
		{
			// 맞았을때 메터리얼 변경
			if( m_Action.action == _XMOBACTION_BLOW &&
				m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
			{
				g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
				g_LastUsedRenderStyle = NULL;
				m_ModelDescriptor.RenderMob( this );
				g_pCurrentSelectMaterial = NULL;				
			}
			else
			{				
				if( g_LastPickedObject == (_XItem*)this )
				{
					g_pCurrentSelectMaterial = &g_SelectedObjectMaterial;
					g_LastUsedRenderStyle = NULL;
					m_ModelDescriptor.RenderMob( this );
					g_pCurrentSelectMaterial = NULL;					
				}
				else
				{
					m_ModelDescriptor.RenderMob( this );
				}
			}
		}
	}
	else if( m_KillSignal == _XDEF_MOB_KILLSIGNAL_START )  // 몹이 죽었을 경우 사라지는 애니메이션 처리		
	{
		if( m_AlphaLevel > 0 )
		{	
			if( m_DieToDeleteTimer == 0 ) m_DieToDeleteTimer = g_LocalSystemTime;
			if( (g_LocalSystemTime - m_DieToDeleteTimer) > _XDEF_MOBDIETODELETETIMER )
			{			
				if( (m_AlphaLevel-_XDEF_MOBFADEIN_RATE) <= 0 )
				{
					m_AlphaLevel = 0;
					m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
					return;
				}
				else
				{
					m_AlphaLevel-=_XDEF_MOBFADEIN_RATE;
					m_ModelDescriptor.m_Position._42 -= 0.003f; // 땅 밑으로
				}
			}
		}
		else
		{
			m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
			return;
		}
		/*g_RenderAlphaBlendFlag = TRUE;
		g_RenderAlphaBlendTextureFactor = ( (m_AlphaLevel<<24) | 0xFFFFFF );
		g_LastUsedRenderStyle = NULL;		*/
		m_ModelDescriptor.RenderMob( this );
		/*g_RenderAlphaBlendTextureFactor = 0xFFFFFFFF;
		g_RenderAlphaBlendFlag = FALSE;
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE); */
	}
	
	if( m_Action.action == _XMOBACTION_ATTACK ) 
	{
		m_ModelDescriptor.RenderSwordEffect( _XDEF_DEFAULTRENDERSTYLE_MONSTERSWORDEFFECT );
	}
	
	BOOL intersect = FALSE;
	m_bShowMobName	= FALSE;
	m_bShowBoldMobName	= FALSE;
	m_bShowHPGauge = FALSE;
	
	if( m_Action.action != _XMOBACTION_DIE ) // 몹이 죽기모드로 들어가면 픽킹을 막아서 재공격이 불가능 하게 만듬.
	{			
		D3DXVECTOR3 nRayDir;
		D3DXVECTOR3 mpos = m_Position;
		mpos.y += m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1];
		D3DXVec3Normalize( &nRayDir, &g_vPickRayDir );
		
		// Check intersect sphere space...
		intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius );
		
		// Check intersect obb
		if( intersect )
		{
			D3DXVECTOR3 PickedPoint;
			intersect = m_ModelDescriptor.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, m_Position, m_RotateAngle, &PickedPoint );
			
			if( intersect )
			{	
				FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - PickedPoint.x);
				FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - PickedPoint.z);
				FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
				
				if( _distance < g_LastPickedObjectDistance )
				{
					if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
					{
						g_LastPickedObjectDistance = _distance;
						g_LastPickedObjectType = _XPICKINGTARGET_MOB;
						g_LastPickedObject = this;
						
						// picking target은 항상 보임
						m_bShowMobName = TRUE;

						if( g_MobAttribTable[m_MobType].scripttype >= 81 && g_MobAttribTable[m_MobType].scripttype < 85 ) // LPS
						{
							((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_LPSOVER );
						}
						else
						{					
							((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_MOBOVER );
						}
					}
				}
			}	
		}
	}
	
	if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
	{
		if(g_pLocalUser->m_AttackTarget)
		{
			if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				if(this == (_XMob*)g_pLocalUser->m_AttackTarget)
				{
					if(g_ViewMyBattleGauge)
					{
						// attack target은 g_ViewMyBattleGauge가 TRUE일때만 보임
						m_bShowMobName = TRUE;
					}
				}
			}
		}
		else if(g_pLocalUser->m_ViewTarget)
		{
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_MOB)
			{
				if( this == (_XMob*)g_pLocalUser->m_ViewTarget )
				{
					if(g_ViewMyBattleGauge)
					{
						// 전투 상태일 경우 view target은 g_ViewMyBattleGauge가 TRUE일때만 보임
						m_bShowMobName = TRUE;
					}
				}
			}
		}
	}
	else
	{
		if(g_pLocalUser->m_ViewTarget) 
		{
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_MOB)
			{
				if( this == (_XMob*)g_pLocalUser->m_ViewTarget )
				{
					// view target은 항상 보임
					m_bShowMobName = TRUE;
				}
			}
		}
	}

	if(g_ViewMonsterBattleGauge)
	{
		// g_ViewMonsterBattleGauge가 TRUE일때는 모두 다 보임
		if(!m_bShowMobName)
			m_bShowMobName = TRUE;
	}
	else
	{
		if(gpInput->CheckKeyPress(DIK_LCONTROL))
		{
			// g_ViewMonsterBattleGauge가 FALSE일때는 Shift key를 눌러야지만 보임
			if(!m_bShowMobName)
				m_bShowMobName = TRUE;
		}
	}
	
	// trap lps hard coding
	if(m_MobType == 1081 || m_MobType == 1091 || m_MobType == 1092 || m_MobType == 1093)
	{
		// 천독금쇄진, 만천화우진, 폭염화망진, 구주파황진
		m_bShowMobName = FALSE;
	}

	if( m_InsideFrustum )
	{
		g_RenderMobCount++;

		if(m_bShowMobName)
		{
			if( g_MobAttribTable[m_MobType].scripttype < 81 || g_MobAttribTable[m_MobType].scripttype > 85 ) // LPS가 아닌 몹
			{
				// Monster 체력 게이지
				g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
				
				gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );	
				gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );
				
				D3DXVECTOR3 mpos, rvec;
				
				mpos.x = m_ModelDescriptor.m_Position._41;
				mpos.y = m_ModelDescriptor.m_Position._42 + (m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2)+0.4f;
				mpos.z = m_ModelDescriptor.m_Position._43;
				
				D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
					&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
				
				m_imageMiniHpBackBorder.Draw( rvec.x-54, rvec.y+17 );
				m_MonsterMiniHpBorder.MoveWindow( rvec.x-45, rvec.y+18  );
				m_MonsterMiniHpBorder.SetGaugeFactor( this->m_DisplayHealthPercentage );
				m_MonsterMiniHpBorder.Process();
				m_MonsterMiniHpBorder.Draw();
				
				// 몬스터 이름 표시
				int length = 0;
				if(g_MonsterName[m_MobType])
				{
					length = strlen(g_MonsterName[m_MobType])*3;
				}
				
				int difflevel = g_MobAttribTable[m_MobType].level - g_pLocalUser->m_CharacterInfo.Get_innerlevel();		
				
				if(difflevel > 4)	// 5 이상
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 255, 0, 0));
				}				
				else if(difflevel > 2 && difflevel <= 4)	// 3, 4
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 255, 138, 0));
				}
				else if(difflevel > 0 && difflevel <= 2)	// 1, 2
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 255, 228, 0));
				}
				else if(difflevel == 0)						// 0
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 255, 255, 255));
				}
				else if(difflevel < 0 && difflevel >= -2)	// -1, -2
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 14, 241, 14));
				}
				else if(difflevel < -2 && difflevel >= -4)	// -3, -4
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 143, 196, 251));
				}
				else if(difflevel < -4)						// -5 이상
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 111, 134, 249));
				}
				else
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);	
				}
				
				g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000 );

				if(g_MonsterName[m_MobType])
					g_XBaseFont->Puts( rvec.x-length, rvec.y, g_MonsterName[m_MobType] );

				g_XBaseFont->Flush();
				g_XBaseFont->DisableGrowMode();
			}
		}
	}
	
#ifdef _XDWDEBUG

	if( g_LodTerrain.m_ViewOBBObject )
	{
		CsuX3D_Box	obbboxdata;
		CSuModelBox OBBBox;

		D3DXVECTOR3 Offset = m_ModelDescriptor.m_ppModelList[0]->OBB_Center + m_Position;
		
		D3DXMATRIX  rotmat;/* = m_ModelDescriptor.m_Position;
		rotmat._41 = 0.0f;
		rotmat._42 = 0.0f;
		rotmat._43 = 0.0f;*/

		D3DXMatrixRotationY( &rotmat, m_RotateAngle );
		
		D3DXVECTOR3 NewAxis[3];
		
		D3DXMATRIX mat;		
		D3DXMatrixIdentity( &mat );		
		mat._11 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[0].x;
		mat._21 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[0].y;
		mat._31 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[0].z;
		mat._12 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[1].x;
		mat._22 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[1].y;
		mat._32 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[1].z;
		mat._13 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[2].x;
		mat._23 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[2].y;
		mat._33 = m_ModelDescriptor.m_ppModelList[0]->OBB_Axis[2].z;
		mat._41 = m_ModelDescriptor.m_ppModelList[0]->OBB_Center.x;
		mat._42 = m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y;
		mat._43 = m_ModelDescriptor.m_ppModelList[0]->OBB_Center.z;
		D3DXMatrixMultiply( &mat, &mat, &rotmat );

		NewAxis[0].x = mat._11;
		NewAxis[0].y = mat._21;
		NewAxis[0].z = mat._31;
		NewAxis[1].x = mat._12;
		NewAxis[1].y = mat._22;
		NewAxis[1].z = mat._32;
		NewAxis[2].x = mat._13;
		NewAxis[2].y = mat._23;
		NewAxis[2].z = mat._33;
		Offset.x = mat._41 + m_Position.x;
		Offset.y = mat._42 + m_Position.y;
		Offset.z = mat._43 + m_Position.z;

		obbboxdata.Create( Offset, NewAxis, m_ModelDescriptor.m_ppModelList[0]->OBB_Extent );
		
		OBBBox.InitBox( obbboxdata.Vertices(), obbboxdata.Center() ); 
		D3DXMATRIX idemat;
		D3DXMatrixIdentity( &idemat );
		OBBBox.Render( idemat, NULL );
		//m_ModelDescriptor.m_ppModelList[0]->m_OBBBox.Render( idemat, NULL );

		g_LastUsedRenderStyle = NULL;
	}

	if( ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewArrowModel )
	{
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		AXIS_VERTEX	line[2];
		line[0].v = m_Position;
		line[0].color = D3DCOLOR_ARGB( 96, 64, 64, 255 );
		line[1].v = m_EndPosition;
		line[1].color = D3DCOLOR_ARGB( 96, 64, 64, 255 );
		
		line[1].v.y = line[0].v.y;
		
		gpDev->SetTexture(0, NULL); 
		gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX ); 
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
#endif
}

void _XMob::MoveNextPostion(BOOL bInterpolate)
{
	if( m_PrevTime == 0.0f ) 
		m_PrevTime = g_fLocalSystemTime;
	else 
	{
		if(m_Action.action == _XMOBACTION_IDLE)
		{
			_XDWINPRINT("[Monstser/MoveNextPosition] [type:%d  id:%d] current[x:%f  z:%f]  next[x:%f  z:%f]", m_MobType, m_MobID, m_Position.x, m_Position.z, m_EndPosition.x, m_EndPosition.z);
		}

		m_MoveVector	 = m_EndPosition - m_Position;
		m_MoveVector.y   = 0.0f;
		D3DXVec3Normalize(&m_MoveVector, &m_MoveVector);

		FLOAT movespeed = m_fMoveSpeed;
		if(bInterpolate)
			movespeed *= 2.0f;
		else if(m_SpeedRate != 0)
		{
			// 이동 속도 변경 - 다시 계산
			movespeed = m_fMoveSpeed + (m_fMoveSpeed * ((FLOAT)m_SpeedRate/100.0f));
		}
		
		FLOAT fTimeDelta = (g_fLocalSystemTime - m_PrevTime);
		// 이동시 보간 처리				
		m_PrevTime = g_fLocalSystemTime;
		m_Position += (m_MoveVector * ( movespeed * fTimeDelta ));

		if(m_bImpact)
			m_OriginPosition = m_Position;
	}
	
	if(!bInterpolate)
		m_RotateAngle = _XMeshMath_GetRotateRadian( m_Position, m_EndPosition );
}

void _XMob::ResetMobProcessTime( void )
{
	
}

void _XMob::ResetAnimationState( void )
{
	m_ModelDescriptor.SetCurMotion( m_Action.action );
	m_ModelDescriptor.PlayAnimation( true );
}

void _XMob::SetBlowMotion(BOOL bCritical)
{
	if( m_Action.action == _XMOBACTION_DIE ) return;
	
	if(!CheckAnimationPriority(_XMOBACTION_BLOW))
		return;

	if( m_Action.action != _XMOBACTION_BLOW )
		m_PrevMotion = m_Action.action;

	m_Action.action = _XMOBACTION_BLOW;

	if( g_MobMotionExistTable[m_MobType].motion_blow > -1 )
		m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_blow, TRUE, TRUE );		
	else
	{
		if( g_MobMotionExistTable[m_MobType].motion_idle > -1 )
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_idle, FALSE );
	}

	if(m_AttackTarget)
	{
		switch(m_AttackTargetObjectType)
		{
		case _XPICKINGTARGET_PC :
			{
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if( m_ModelDescriptor.m_EffectManager )
				{					
					D3DXMATRIX resulteffectmatrix = m_ModelDescriptor.m_Position;
					resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

					if( g_ViewDamageValue )
					{
						// damage effect 추가 처리해야 함.
					}
					else
					{
						if(bCritical)
							m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
						else
							m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
					}
				}
			}
			break;
		case _XPICKINGTARGET_MOB :
			{
			}
			break;
		}
	}
}

void _XMob::SetCharacterMobAnimation( _XMOBMOTIONCLASS_TYPE motionclass, bool changemotion )
{
	if( m_Action.action == _XMOBACTION_DIE ) return;

	if( m_Action.action != _XMOBACTION_BLOW )		
		m_PrevMotion = m_Action.action;
	m_Action.action = motionclass;

	m_PrevTime = 0.0f;

	switch( motionclass )
	{
	case _XMOBACTION_IDLE :
		{	
			short stancemotion = -1;
			if(m_Gender == _XGENDER_MAN)
				stancemotion = g_SkillProperty[m_SkillIndex]->stance_male;
			else
				stancemotion = g_SkillProperty[m_SkillIndex]->stance_female;

			
			m_ModelDescriptor.SetCurMotion(stancemotion + _XANI_ORDER_REPEAT, changemotion);		
		}
		break;
		
	case _XMOBACTION_MOVE :
		{			
			// 앞에 보간 동작 없음
			if( m_ItemNo == 0 )
			{
				m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
			}
			else
			{
				switch( g_WeaponItemProperty[m_ItemNo].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty[m_ItemNo].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
					}
					else
					{						
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기					
					m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
					break;
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
					break;
				}		

			}
			m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		}		
		break;		
	case _XMOBACTION_BLOW :
		{
			switch( g_WeaponItemProperty[m_ItemNo].cSecondType )
			{
			case _XGI_SC_WEAPON_SHORT				:				// 단병기
				if( g_WeaponItemProperty[m_ItemNo].cThirdType == _XGI_TC_WEAPON_FIST )
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
			case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
				m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
				break;
			case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
				m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
				break;
			}							
		}
		break;

	case _XMOBACTION_DIE :
		if( m_ModelDescriptor.m_EffectManager ) 
			m_ModelDescriptor.m_EffectManager->Dispose();
		
		m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;

	case _XMOBACTION_ATTACK :		
		{
			m_ModelDescriptor.SetCurMotion( m_AttackType, changemotion );						
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		}				
		break;
	}		

	m_ModelDescriptor.m_FrameResult = _XDEF_ACI_NONE;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (motionclass == _XMOBACTION_ATTACK);
}

BOOL _XMob::CheckAnimationPriority(int animationindex)
{
	if(m_Action.action == _XMOBACTION_MOVE && animationindex == _XMOBACTION_BLOW)
		return FALSE;

	if(m_Action.action == _XMOBACTION_ATTACK && animationindex == _XMOBACTION_BLOW)
		return FALSE;
		
	if(m_Action.action == _XMOBACTION_SKILL && animationindex == _XMOBACTION_BLOW)
		return FALSE;
	
	if(m_Action.action == _XMOBACTION_DIE && m_Action.dietype == _XMONSTERDIETYPE_DIE1)
		return FALSE;
	
	if(m_Action.action == _XMOBACTION_DIE && animationindex == _XMOBACTION_BLOW)
		return FALSE;
	
	if(m_Action.action == _XMOBACTION_DIE && animationindex == _XMOBACTION_IDLE)
		return FALSE;

	return TRUE;
}

void _XMob::SetAnimation( int animationindex )
{
	if(!CheckAnimationPriority(animationindex))
		return;

	m_PrevMotion = m_Action.action;
	m_Action.action = animationindex;
	m_PrevTime = 0.0f;

	switch( animationindex )
	{
	case _XMOBACTION_IDLE :		
		if( g_MobMotionExistTable[m_MobType].motion_idle > -1 )
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_idle, TRUE );
		//m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;

	case _XMOBACTION_MOVE :
		if( m_Action.movemode == _XMONSTERMOVETYPE_WALK )
		{
			if( g_MobMotionExistTable[m_MobType].motion_walk > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_walk, TRUE );
				m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_walk;
			}
			else
			{
				if( g_MobMotionExistTable[m_MobType].motion_run > -1 )
				{
					m_Action.movemode = _XMONSTERMOVETYPE_RUN;
					m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_run, TRUE );	
					m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_run;
				}
			}
		}
		else
		{
			if( g_MobMotionExistTable[m_MobType].motion_run > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_run, TRUE );	
				m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_run;
			}
			else
			{
				if( g_MobMotionExistTable[m_MobType].motion_walk > -1 )
				{
					m_Action.movemode = _XMONSTERMOVETYPE_WALK;
					m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_walk, TRUE );
					m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_walk;
				}
			}
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;

	case _XMOBACTION_ATTACK :
		if( m_Action.attacktype == _XMONSTERATTACKTYPE_ATTACK1 )
		{
			if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK2;
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack3 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack3, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK3;
			}
		}
		else if( m_Action.attacktype == _XMONSTERATTACKTYPE_ATTACK2 )
		{
			if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK1;
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack3 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack3, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK3;
			}
		}
		else if( m_Action.attacktype == _XMONSTERATTACKTYPE_ATTACK3 )
		{
			if( g_MobMotionExistTable[m_MobType].motion_attack3 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack3, TRUE );
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK1;
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK2;
			}
		}
		else if(m_Action.attacktype == _XMONSTERATTACKTYPE_RATTACK)
		{
			if(g_MobMotionExistTable[m_MobType].motion_rattack > -1)
			{
				m_ModelDescriptor.SetCurMotion(g_MobMotionExistTable[m_MobType].motion_rattack, TRUE);
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK1;
			}
			else if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK2;
			}
			else if(g_MobMotionExistTable[m_MobType].motion_attack3 > -1)
			{
				m_ModelDescriptor.SetCurMotion(g_MobMotionExistTable[m_MobType].motion_attack3, TRUE);
				m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK3;
			}
		}

		//m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;

	case _XMOBACTION_BLOW :
		if( g_MobMotionExistTable[m_MobType].motion_blow > -1 )
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_blow, TRUE );		
		else
		{
			if( g_MobMotionExistTable[m_MobType].motion_idle > -1 )
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_idle, FALSE );
		}			
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;

	case _XMOBACTION_DIE :
		if( m_ModelDescriptor.m_EffectManager ) m_ModelDescriptor.m_EffectManager->Dispose();
		
		if( m_Action.dietype == _XMONSTERDIETYPE_DIE1 )
		{
			if( g_MobMotionExistTable[m_MobType].motion_die1 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die1, TRUE );
			}
			else 
			{
				if( g_MobMotionExistTable[m_MobType].motion_die2 > -1 )
					m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die2, TRUE );
				m_Action.dietype = _XMONSTERDIETYPE_DIE2;
			}
		}
		else
		{
			if( g_MobMotionExistTable[m_MobType].motion_die2 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die2, TRUE );
			}
			else 
			{
				if( g_MobMotionExistTable[m_MobType].motion_die1 > -1 )
					m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die1, TRUE );
				m_Action.dietype = _XMONSTERDIETYPE_DIE1;
			}
		}
		
		if(this == (_XMob*)g_LastPickedObject) 
			g_LastPickedObject = NULL;
		if(this == (_XMob*)g_pLocalUser->m_AttackTarget)
			g_pLocalUser->ResetAttackTarget();
		if(this == (_XMob*)g_pLocalUser->m_ViewTarget)
			g_pLocalUser->ResetViewTarget();

		m_DieSignalReceiveTime = 0;

		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		ReleaseState();		// 상태효과 삭제
		break;

	case _XMOBACTION_SKILL :
		{
			if( m_ModelDescriptor.m_EffectManager ) m_ModelDescriptor.m_EffectManager->Dispose();

			if( m_Action.skilltype == _XMONSTERSKILLTYPE_SKILL1 )
			{
				if( g_MobMotionExistTable[m_MobType].motion_skill1 > -1 )
					m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_skill1, TRUE );
			}
			else
			{
				if( g_MobMotionExistTable[m_MobType].motion_skill2 > -1 )
					m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_skill2, TRUE );
				m_Action.skilltype = _XMONSTERSKILLTYPE_SKILL2;
			}			
		}

		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;

	case _XMOBACTION_ENTERING :
		if( g_MobMotionExistTable[m_MobType].motion_entering > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_entering, FALSE );
		}
		else
		{
			// Entering이 없는 경우
			if( g_MobMotionExistTable[m_MobType].motion_idle > -1 )
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_idle, FALSE );
			m_Action.action = _XMOBACTION_IDLE;
		}		

		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;

	default:		
		break;
	}	
}

void _XMob::SetAnimationIndex( int animationindex )
{
	int actionindex = 0;
	switch(animationindex)
	{
	case _XMOBACTIONNUMBER_IDLE :
		actionindex = _XMOBACTION_IDLE;
		break;
	case _XMOBACTIONNUMBER_WALK :
	case _XMOBACTIONNUMBER_RUN :
		actionindex = _XMOBACTION_MOVE;
		break;
	case _XMOBACTIONNUMBER_ATTACK1 :
	case _XMOBACTIONNUMBER_ATTACK2 :
	case _XMOBACTIONNUMBER_ATTACK3 :
	case _XMOBACTIONNUMBER_RATTACK :
		actionindex = _XMOBACTION_ATTACK;
		break;
	case _XMOBACTIONNUMBER_BLOW :
		actionindex = _XMOBACTION_BLOW;
		break;
	case _XMOBACTIONNUMBER_DIE1 :
	case _XMOBACTIONNUMBER_DIE2 :
		{
			actionindex = _XMOBACTION_DIE;
			m_DieSignalReceiveTime = 0;
		}
		break;
	case _XMOBACTIONNUMBER_SKILL1 :
	case _XMOBACTIONNUMBER_SKILL2 :
		actionindex = _XMOBACTION_SKILL;
		break;
	case _XMOBACTIONNUMBER_ENTERING :
		actionindex = _XMOBACTION_ENTERING;
		break;
	}

	if(!CheckAnimationPriority(actionindex))
		return;

	m_PrevMotion = m_Action.action;
	m_PrevTime = 0.0f;

	switch( animationindex )
	{
	case _XMOBACTIONNUMBER_IDLE :
		m_Action.action = _XMOBACTION_IDLE;
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		if( g_MobMotionExistTable[m_MobType].motion_idle > -1 )
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_idle, TRUE );
		break;
	case _XMOBACTIONNUMBER_WALK :
		m_Action.action   = _XMOBACTION_MOVE;

		if( g_MobMotionExistTable[m_MobType].motion_walk > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_walk, TRUE );
			m_Action.movemode = _XMONSTERMOVETYPE_WALK;
			m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_walk;
		}
		else // 걷기 없으면 달리기.
		{
			if( g_MobMotionExistTable[m_MobType].motion_run > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_run, TRUE );
				m_Action.movemode = _XMONSTERMOVETYPE_RUN;
				m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_run;
			}
		}
		
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );		
		break;
	case _XMOBACTIONNUMBER_RUN :
		m_Action.action   = _XMOBACTION_MOVE;

		if( g_MobMotionExistTable[m_MobType].motion_run > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_run, TRUE );
			m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_run;
			m_Action.movemode = _XMONSTERMOVETYPE_RUN;
		}
		else // 달리기 동작이 없으면 걷기
		{
			if( g_MobMotionExistTable[m_MobType].motion_walk > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_walk, TRUE );
				m_fMoveSpeed	  =	g_MobAttribTable[ m_MobType ].movespeed_walk;
				m_Action.movemode = _XMONSTERMOVETYPE_WALK;
			}
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;
	case _XMOBACTIONNUMBER_ATTACK1 :
		m_Action.action = _XMOBACTION_ATTACK;
		if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );			
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK1;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK2;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack3, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK3;
		}
			
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_ATTACK2 :
		m_Action.action = _XMOBACTION_ATTACK;
		if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK2;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK1;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack3, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK3;
		}
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_ATTACK3 :
		m_Action.action = _XMOBACTION_ATTACK;
		if( g_MobMotionExistTable[m_MobType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack3, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK3;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK1;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK2;
		}
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_RATTACK :
		m_Action.action = _XMOBACTION_ATTACK;
		if(g_MobMotionExistTable[m_MobType].motion_rattack > -1)
		{
			m_ModelDescriptor.SetCurMotion(g_MobMotionExistTable[m_MobType].motion_rattack, TRUE);
			m_Action.attacktype = _XMONSTERATTACKTYPE_RATTACK;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack1, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK1;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack2, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK2;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_attack3, TRUE );
			m_Action.attacktype = _XMONSTERATTACKTYPE_ATTACK3;
		}
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_BLOW	:
		m_Action.action = _XMOBACTION_BLOW;
		if( g_MobMotionExistTable[m_MobType].motion_blow > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_blow, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;
	case _XMOBACTIONNUMBER_DIE1	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}
		m_Action.action = _XMOBACTION_DIE;		

		if( g_MobMotionExistTable[m_MobType].motion_die1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die1, TRUE );
			m_Action.dietype = _XMONSTERDIETYPE_DIE1;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_die2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die2, TRUE );
			m_Action.dietype = _XMONSTERDIETYPE_DIE2;
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;	
	case _XMOBACTIONNUMBER_DIE2	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}
		m_Action.action = _XMOBACTION_DIE;		
		
		if( g_MobMotionExistTable[m_MobType].motion_die2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die2, TRUE );
			m_Action.dietype = _XMONSTERDIETYPE_DIE2;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_die1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_die1, TRUE );
			m_Action.dietype = _XMONSTERDIETYPE_DIE1;
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;	
	case _XMOBACTIONNUMBER_SKILL1	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}
		m_Action.action = _XMOBACTION_SKILL;		
		
		if( g_MobMotionExistTable[m_MobType].motion_skill1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_skill1, TRUE );
			m_Action.skilltype = _XMONSTERSKILLTYPE_SKILL1;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_skill2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_skill2, TRUE );
			m_Action.skilltype = _XMONSTERSKILLTYPE_SKILL2;
		}		
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;		
	case _XMOBACTIONNUMBER_SKILL2	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}
		m_Action.action = _XMOBACTION_SKILL;		
		
		if( g_MobMotionExistTable[m_MobType].motion_skill2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_skill2, TRUE );
			m_Action.skilltype = _XMONSTERSKILLTYPE_SKILL2;
		}
		else if( g_MobMotionExistTable[m_MobType].motion_skill1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_skill1, TRUE );
			m_Action.skilltype = _XMONSTERSKILLTYPE_SKILL1;
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;		
		
	case _XMOBACTIONNUMBER_ENTERING :
		m_Action.action = _XMOBACTION_ENTERING;
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		if( g_MobMotionExistTable[m_MobType].motion_entering > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_entering, TRUE );
		}
		else
		{
			if( g_MobMotionExistTable[m_MobType].motion_idle > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_MobType].motion_idle, TRUE );
				m_Action.action = _XMOBACTION_IDLE;
			}
		}
		break;
	}
}

void _XMob::UpdatePosition( void )
{
	D3DXMATRIX	rotmat;
	D3DXVECTOR3	terrainnormal;
	_XOBBData*  pOBB = NULL;

	D3DXMatrixIdentity( &rotmat );
	D3DXMatrixRotationY( &rotmat, m_RotateAngle );
	m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( m_Position, pOBB, &terrainnormal );	
	D3DXMatrixTranslation( &m_ModelDescriptor.m_Position, m_Position.x, m_Position.y, m_Position.z );
	
	if( !pOBB && ( (!g_MobAttribTable[ this->m_MobType ].standingerect) || ( m_Action.action == _XMOBACTION_DIE ) ) )
	{		
		D3DXMATRIX  rotmatrix;
		D3DXQUATERNION normalrotquat;
		D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);
		
		if( terrainnormal.x != 0.0f || terrainnormal.y != 1.0f || terrainnormal.z != 0.0f ) // check exactly upvector...
		{	
			normalrotquat = _XRotationArcNormalized( &upvector, &terrainnormal  );			
			D3DXMatrixRotationQuaternion( &rotmatrix, &normalrotquat );
			D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmatrix, &m_ModelDescriptor.m_Position );
		}		
	}

	D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position );		
	
	CheckInsideFrustum();
}

void _XMob::CheckInsideFrustum(void)
{
	FLOAT dx = (m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
	FLOAT dz = (m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
	m_ModelDescriptor.m_CameraDistance = _XDistance( dx , dz );
				
	m_InsideFrustum = ( (m_ModelDescriptor.m_CameraDistance < g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS) &&
						( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( m_Position, 3.5f ) != NOT_VISIBLE ) );
}

void _XMob::InsertState(char type, short id, char spellstep)
{
	if(type == 1)		// Effect
	{
		map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
		if(iter_table != g_CharacterStateTable.end())
		{
			_XCharacterStateTable* pSTable = iter_table->second;
			if(pSTable)
			{
				if(pSTable->condition_flag_id <= 0)
				{
					list <_XMOB_STATE*>::iterator iter_state;
					for(iter_state = m_StateList.begin() ; iter_state != m_StateList.end() ; ++iter_state)
					{
						_XMOB_STATE* currentstate = *iter_state;
						if(currentstate)
						{
							if(currentstate->spellID == id)
							{
								return;
							}
						}
					}
					
					_XMOB_STATE* pState = new _XMOB_STATE;
					pState->type = type;
					pState->spellID = id;
					pState->spellStep = spellstep;
					
					m_StateList.push_back(pState);

					// visual effect
					if(m_ModelDescriptor.m_EffectManager)
						m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
						pSTable->effectNumber, &m_ModelDescriptor.m_Position, NULL, TRUE );
				}
			}
		}
	}
	else	// Spell
	{
		list <_XMOB_STATE*>::iterator iter_state;
		for(iter_state = m_StateList.begin() ; iter_state != m_StateList.end() ; ++iter_state)
		{
			_XMOB_STATE* currentstate = *iter_state;
			if(currentstate)
			{
				if(currentstate->spellID == id)
				{
					return;
				}
			}
		}

		_XMOB_STATE* pState = new _XMOB_STATE;
		pState->type = type;
		pState->spellID = id;
		pState->spellStep = spellstep;
		
		m_StateList.push_back(pState);
	}
}

void _XMob::DeleteState(char type, short id, char spellstep)
{
	list <_XMOB_STATE*>::iterator iter_state;
	for(iter_state = m_StateList.begin() ; iter_state != m_StateList.end() ; )
	{
		_XMOB_STATE* currentstate = *iter_state;
		if(currentstate)
		{
			if(currentstate->type == type)
			{
				// effect - visual effect 삭제
				if(currentstate->type == 1)
				{
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
					if(iter_table != g_CharacterStateTable.end())
					{
						_XCharacterStateTable* pSTable = iter_table->second;
						if(pSTable)
						{
							if(m_ModelDescriptor.m_EffectManager)
								m_ModelDescriptor.m_EffectManager->DeleteInstance(pSTable->effectNumber);
						}
					}
				}

				if(currentstate->spellID == id && currentstate->spellStep == spellstep)
				{
					SAFE_DELETE(currentstate);
					iter_state = m_StateList.erase(iter_state);
				}
				else
					iter_state++;
			}
			else
				iter_state++;
		}
		else
			iter_state++;
	}
}

void _XMob::ReleaseState(void)
{
	list <_XMOB_STATE*>::iterator iter_state;
	for(iter_state = m_StateList.begin() ; iter_state != m_StateList.end() ; )
	{
		_XMOB_STATE* currentstate = *iter_state;
		if(currentstate)
		{
			// effect - visual effect 삭제
			if(currentstate->type == 1)
			{
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->spellID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* pSTable = iter_table->second;
					if(pSTable)
					{
						if(m_ModelDescriptor.m_EffectManager)
							m_ModelDescriptor.m_EffectManager->DeleteInstance(pSTable->effectNumber);
					}
				}
			}

			SAFE_DELETE(currentstate);
			iter_state = m_StateList.erase(iter_state);
		}
	}

	m_StateList.clear();
}

void _XMob::InsertCondition(int index)
{
	// visual effect
	for(int i = 0 ; i < 5 ; ++i)
	{
		if(g_CharacterConditionTable[index-1].visualEffect[i] > 0)
		{
			if(m_ModelDescriptor.m_EffectManager)
				m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
				g_CharacterConditionTable[index-1].visualEffect[i], &m_ModelDescriptor.m_Position, NULL, TRUE );
		}
	}
}

void _XMob::DeleteCondition(int index)
{
	// visual effect
	for(int i = 0 ; i < 5 ; ++i)
	{
		if(g_CharacterConditionTable[index-1].visualEffect[i] > 0)
		{
			if(m_ModelDescriptor.m_EffectManager)
				m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterConditionTable[index-1].visualEffect[i]);
		}
	}
}
	

//////////////////////////////////////////////////////////////////////
// Mob manager

_XMobList::_XMobList()
{
	m_LastPlaySoundTime = 0;
	m_ClientMobAIDelayTime = 0;
}

_XMobList::~_XMobList()
{


}

void _XMobList::DeletePool(void)
{
	m_MobPool.DeleteData();
}

void _XMobList::CreatePool(void)
{
	m_MobPool.Reserve(50);
}

_XMob*	_XMobList::NewMob()
{
	_XMob* pMob = m_MobPool.New();
	pMob->InitializeData(); 
	return pMob;
}

void _XMobList::DeleteMob(_XMob* pMob)
{
	if (pMob == NULL) return;
	
	if( pMob == (_XMob*)g_LastPickedObject ) 
		g_LastPickedObject = NULL;
	
	if( pMob == (_XMob*)g_pLocalUser->m_AttackTarget )
	{
		g_pLocalUser->ResetAttackTarget();
	}
	if(pMob == (_XMob*)g_pLocalUser->m_ViewTarget)
	{
		g_pLocalUser->ResetViewTarget();
	}
	
	smdef_XUser::iterator dt;
	for (dt = g_Userlist.m_smUserList.begin(); dt != g_Userlist.m_smUserList.end(); ++ dt)
	{
		if( (*dt).second->m_AttackTarget )
		{
			if( (*dt).second->m_AttackTarget == pMob )
			{
				(*dt).second->ResetAttackTarget();
			}
		}
	}
	
	smdef_XMob::iterator iter_mob;
	for(iter_mob = m_smMobList.begin() ; iter_mob != m_smMobList.end() ; ++iter_mob)
	{
		_XMob* _mob = iter_mob->second;
		if(_mob)
		{
			if(_mob->m_AttackTarget)
			{
				if(_mob->m_AttackTarget == pMob)
				{
					_mob->ResetAttackTarget();
				}
			}
		}
	}

	pMob->DestroyData();
	m_MobPool.Delete(pMob);
}

void _XMobList::disposeList_zero(void)
{
	g_pLocalUser->ResetAttackTarget();
	g_pLocalUser->ResetViewTarget();

	smdef_XMob::iterator it;
	for (it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
	{
		delete (*it).second;
	}
	m_smMobList.clear();
}

void _XMobList::disposeList(void)
{
	smdef_XMob::iterator it;
	while(!m_smMobList.empty())
	{
		it = m_smMobList.begin();
		_XMob* pMob = (*it).second;
		DeleteMob(pMob->m_MobID);
	}
	m_smMobList.clear();

	if (!m_slDeleteMotType.empty())
	{
		sldef_DeleteMobType::iterator it;
		for (it = m_slDeleteMotType.begin(); it != m_slDeleteMotType.end(); ++ it)
		{
			DELETE_MOB_INFO info = (*it);
			ReleaseReferenceModel(info.usMobType, TRUE);
		}
		m_slDeleteMotType.clear();
	}


#ifdef _XDWDEBUG
	/*
	for (int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; ++ i)
	{
		if (g_MOBBIPEDMODEL[i].GetRef() != 0)
		{
			_XFatalError( "g_MOBBIPEDMODEL[%d] Biped Data Reference Error : %d", i, g_MOBBIPEDMODEL[i].GetRef());
			return;
		}
		if (g_MOBMODEL[i])
		{
			if (g_MOBMODEL[i]->GetRef() != 0)
			{
				_XFatalError( "g_MOBMODEL[%d] Model Data Reference Error : %d", i, g_MOBMODEL[i]->GetRef());
				return;
			}

			
			//for(int j = 0; j < g_MOBMODEL[i]->m_NodeCnt; j++)
			//{
			//	if (g_MobTextureArchive.GetRef(g_MOBMODEL[i]->m_Node[i].nTextureId) != 0)
			//	{
			//		_XFatalError( "g_MobTextureArchive[%d] Texture Reference Error : %d", j, g_MobTextureArchive.GetRef(g_MOBMODEL[i]->m_Node[i].nTextureId));
			//		return;
			//	}
			//}
		}
	}
	*/
#endif
}

BOOL _XMobList::AddReferenceModel(unsigned short usMobType) 
{
	if( usMobType >= _XDEF_MODELCLASSCOUNT_MOB ) return FALSE;

	if( g_MOBBIPEDMODEL[usMobType].m_strBipedName.size() <= 0 ) return FALSE;

	g_MOBBIPEDMODEL[usMobType].AddRef();

	if (g_MOBBIPEDMODEL[usMobType].GetRef() == 1)
	{
		if( !g_ModelArchive.ReOpenPackage() ) 
			return FALSE;
		
		if( !g_AniArchive.ReOpenPackage() ) 
		{
			g_ModelArchive.ClosePackage();
			return FALSE;
		}

		if( !g_MOBBIPEDMODEL[usMobType].Load( g_ModelArchive.GetPackedFile(const_cast<char*>(g_MOBBIPEDMODEL[usMobType].m_strBipedName.c_str())), const_cast<char*>(g_MOBBIPEDMODEL[usMobType].m_strBipedName.c_str()) ) )
		{
			g_ModelArchive.ClosePackage();
			g_AniArchive.ClosePackage();
			return FALSE;
		}
		if( !g_MOBBIPEDMODEL[usMobType].ReserveAnimationStack( g_MOBBIPEDMODEL[usMobType].m_svTempMotion.size() ) )
		{
			g_ModelArchive.ClosePackage();
			g_AniArchive.ClosePackage();
			return FALSE;
		}

		int i = 0;
		svdef_TEMP_MOTION::iterator it;
		for (it = g_MOBBIPEDMODEL[usMobType].m_svTempMotion.begin(); it != g_MOBBIPEDMODEL[usMobType].m_svTempMotion.end(); ++ i, ++ it)
		{
			TEMP_MOTION* pTempBipedNode = (*it);
			if( !g_MOBBIPEDMODEL[usMobType].LoadKey( g_AniArchive.GetPackedFile( const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ), const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ) )
			{
				g_ModelArchive.ClosePackage();
			g_AniArchive.ClosePackage();
				return FALSE;
			}
			_LPXM_MOTION pMotion = g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 );

			if( pMotion )
			{
				for(int j = 0; j < 9; ++ j)
				{
					pMotion->SetSoundEffectFrame(j, pTempBipedNode->SoundOutFrame[j], pTempBipedNode->SoundIndex[j] );
					pMotion->SetAttackFrame( j, pTempBipedNode->AttackCmdFrame[j] );
					pMotion->SetAttackClass( j, pTempBipedNode->AttackClass[j] );
					pMotion->SetEffectFrame( j, pTempBipedNode->EffectFrame[j], pTempBipedNode->EffectIndex[j] );
				}
			}

			/*
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 0, pTempBipedNode->AttackCmdFrame[0] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 1, pTempBipedNode->AttackCmdFrame[1] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 2, pTempBipedNode->AttackCmdFrame[2] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 3, pTempBipedNode->AttackCmdFrame[3] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 4, pTempBipedNode->AttackCmdFrame[4] );				
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 5, pTempBipedNode->AttackCmdFrame[5] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 6, pTempBipedNode->AttackCmdFrame[6] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 7, pTempBipedNode->AttackCmdFrame[7] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 8, pTempBipedNode->AttackCmdFrame[8] );

			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 0, pTempBipedNode->AttackClass[0] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 1, pTempBipedNode->AttackClass[1] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 2, pTempBipedNode->AttackClass[2] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 3, pTempBipedNode->AttackClass[3] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 4, pTempBipedNode->AttackClass[4] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 5, pTempBipedNode->AttackClass[5] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 6, pTempBipedNode->AttackClass[6] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 7, pTempBipedNode->AttackClass[7] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 8, pTempBipedNode->AttackClass[8] );

			for( j = 0; j < 9; ++j)
				g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetEffectFrame(j, pTempBipedNode->EffectFrame[j], pTempBipedNode->EffectIndex[j] );
			*/
		}

		if (!g_MOBBIPEDMODEL[usMobType].m_strParseBipedName.empty())
		{
			int nodeindex = g_MOBBIPEDMODEL[usMobType].FindObject( const_cast<char*>(g_MOBBIPEDMODEL[usMobType].m_strParseBipedName.c_str()) );
			if( nodeindex < 0 )
			{
				_XFatalError( "Can't find linked biped node : %s %s", g_MonsterName[usMobType], g_MOBBIPEDMODEL[usMobType].m_strParseBipedName.c_str() );
				g_ModelArchive.ClosePackage();
				g_AniArchive.ClosePackage();
				return FALSE;
			}
			if (g_MOBBIPEDMODEL[usMobType].m_pSwordTraceInfoData != NULL)
				g_MOBBIPEDMODEL[usMobType].m_pSwordTraceInfoData->SetLinkedBiped( g_MOBBIPEDMODEL[usMobType].m_swordtraceeffectindex, nodeindex );
		}
		OutputDebugString(g_MOBBIPEDMODEL[usMobType].m_strBipedName.c_str());
		OutputDebugString(" : Biped Loaded \n");

		g_ModelArchive.ClosePackage();
		g_AniArchive.ClosePackage();
	}

#ifdef _XDEF_MODELTHREADLOADING
	
	if( g_MOBMODEL[usMobType] )
	{
		g_MOBMODEL[usMobType]->AddRef();
		g_MOBMODEL[usMobType]->m_pExtraData = (LPVOID)&g_MobAttribTable[usMobType];
	}	
		
#else

	if( g_MOBMODEL[usMobType] )
	{
		g_MOBMODEL[usMobType]->AddRef();

		if (g_MOBMODEL[usMobType]->GetRef() == 1)
		{
			_XSetDefaultPath( _XDEF_DEFAULTPATH );	
			if( !g_ModelArchive.ReOpenPackage() ) 
				return FALSE;

			if( !g_AniArchive.ReOpenPackage() ) 
			{
				g_ModelArchive.ClosePackage();
				return FALSE;
			}

			if( !g_MOBMODEL[usMobType]->Load_Runtime( g_ModelArchive.GetPackedFile( const_cast<char*>(g_MOBMODEL[usMobType]->m_strFileName.c_str()) ), 
											  &g_MobTextureArchive, 
											  !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
											  const_cast<char*>(g_MOBMODEL[usMobType]->m_strFileName.c_str()) ) )
			{
				g_ModelArchive.ClosePackage();
				g_AniArchive.ClosePackage();
				return FALSE;
			}
			
			if( g_MOBMODEL[usMobType]->m_strLodFileName[0].length() )
			{
				int lodcount = 1;
				if( g_MOBMODEL[usMobType]->m_strLodFileName[1].length() )
				{
					lodcount = 2;
				}

				g_MOBMODEL[usMobType]->CreateLODObject( lodcount );

				for( int ilod = 0; ilod < lodcount; ++ilod )
				{
					if( !g_MOBMODEL[usMobType]->m_pLODObjectList[ilod]->Load_Runtime( 
									g_ModelArchive.GetPackedFile( const_cast<char*>(g_MOBMODEL[usMobType]->m_strLodFileName[ilod].c_str()) ), 
																  &g_MobTextureArchive, 
																  !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
																  const_cast<char*>(g_MOBMODEL[usMobType]->m_strLodFileName[ilod].c_str()) ) )
					{
						g_ModelArchive.ClosePackage();
						g_AniArchive.ClosePackage();
						return FALSE;
					}
				}
			}

			/*
			if (g_MOBMODEL[usMobType]->m_pTextureController)
			{
				for(int i = 0; i < g_MOBMODEL[usMobType]->m_NodeCnt; i++)
				{
					g_MOBMODEL[usMobType]->m_pTextureController->AddRef( g_MOBMODEL[usMobType]->m_Node[i].nTextureId );
				}
			}
			*/

			OutputDebugString(g_MonsterName[usMobType]);
			OutputDebugString(" : Mob Loaded \n");

			g_ModelArchive.ClosePackage();
			g_AniArchive.ClosePackage();
		}

		g_MobAttribTable[usMobType].xsize			= g_MOBMODEL[usMobType]->OBB_Extent[0];
		g_MobAttribTable[usMobType].ysize			= g_MOBMODEL[usMobType]->OBB_Extent[1];
		g_MobAttribTable[usMobType].collideradius	= g_MOBMODEL[usMobType]->m_ObjectRadius;
	}
#endif

	return TRUE;
}

BOOL _XMobList::AddReferenceModel_Precaching(unsigned short usMobType) 
{
	if( g_MOBBIPEDMODEL[usMobType].m_strBipedName.size() <= 0 ) return FALSE;
	
	g_MOBBIPEDMODEL[usMobType].AddRef();

	if (g_MOBBIPEDMODEL[usMobType].GetRef() == 1)
	{
		if( !g_MOBBIPEDMODEL[usMobType].Load( g_ModelArchive.GetPackedFile(const_cast<char*>(g_MOBBIPEDMODEL[usMobType].m_strBipedName.c_str())), const_cast<char*>(g_MOBBIPEDMODEL[usMobType].m_strBipedName.c_str()) ) )
		{
			return FALSE;
		}
		if( !g_MOBBIPEDMODEL[usMobType].ReserveAnimationStack( g_MOBBIPEDMODEL[usMobType].m_svTempMotion.size() ) )
		{
			return FALSE;
		}

		int i = 0;
		svdef_TEMP_MOTION::iterator it;
		for (it = g_MOBBIPEDMODEL[usMobType].m_svTempMotion.begin(); it != g_MOBBIPEDMODEL[usMobType].m_svTempMotion.end(); ++ i, ++ it)
		{
			TEMP_MOTION* pTempBipedNode = (*it);
			if( !g_MOBBIPEDMODEL[usMobType].LoadKey( g_AniArchive.GetPackedFile( const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ), const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ) )
			{
				return FALSE;
			}
			for(int j = 0; j < 9; ++ j)
				g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetSoundEffectFrame(j, pTempBipedNode->SoundOutFrame[j], pTempBipedNode->SoundIndex[j] );

			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 0, pTempBipedNode->AttackCmdFrame[0] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 1, pTempBipedNode->AttackCmdFrame[1] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 2, pTempBipedNode->AttackCmdFrame[2] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 3, pTempBipedNode->AttackCmdFrame[3] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 4, pTempBipedNode->AttackCmdFrame[4] );				
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 5, pTempBipedNode->AttackCmdFrame[5] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 6, pTempBipedNode->AttackCmdFrame[6] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 7, pTempBipedNode->AttackCmdFrame[7] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackFrame( 8, pTempBipedNode->AttackCmdFrame[8] );

			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 0, pTempBipedNode->AttackClass[0] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 1, pTempBipedNode->AttackClass[1] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 2, pTempBipedNode->AttackClass[2] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 3, pTempBipedNode->AttackClass[3] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 4, pTempBipedNode->AttackClass[4] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 5, pTempBipedNode->AttackClass[5] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 6, pTempBipedNode->AttackClass[6] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 7, pTempBipedNode->AttackClass[7] );
			g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetAttackClass( 8, pTempBipedNode->AttackClass[8] );

			for( j = 0; j < 9; ++j)
				g_MOBBIPEDMODEL[usMobType].GetMotion( g_MOBBIPEDMODEL[usMobType].GetMotionCnt()-1 )->SetEffectFrame(j, pTempBipedNode->EffectFrame[j], pTempBipedNode->EffectIndex[j] );
		}

		if (!g_MOBBIPEDMODEL[usMobType].m_strParseBipedName.empty())
		{
			int nodeindex = g_MOBBIPEDMODEL[usMobType].FindObject( const_cast<char*>(g_MOBBIPEDMODEL[usMobType].m_strParseBipedName.c_str()) );
			if( nodeindex < 0 )
			{
				_XFatalError( "Can't find linked biped node : %s %s", g_MonsterName[usMobType], g_MOBBIPEDMODEL[usMobType].m_strParseBipedName.c_str() );
				return FALSE;
			}
			if (g_MOBBIPEDMODEL[usMobType].m_pSwordTraceInfoData != NULL)
				g_MOBBIPEDMODEL[usMobType].m_pSwordTraceInfoData->SetLinkedBiped( g_MOBBIPEDMODEL[usMobType].m_swordtraceeffectindex, nodeindex );
		}
		OutputDebugString(g_MOBBIPEDMODEL[usMobType].m_strBipedName.c_str());
		OutputDebugString(" : Biped Loaded \n");
	}

	if( g_MOBMODEL[usMobType] )
	{
		g_MOBMODEL[usMobType]->AddRef();

		if (g_MOBMODEL[usMobType]->GetRef() == 1)
		{
			if( !g_MOBMODEL[usMobType]->Load_Runtime( g_ModelArchive.GetPackedFile( const_cast<char*>(g_MOBMODEL[usMobType]->m_strFileName.c_str()) ), 
											  &g_MobTextureArchive, 
											  !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
											  const_cast<char*>(g_MOBMODEL[usMobType]->m_strFileName.c_str()) ) )
			{
				return FALSE;
			}
			
			if( g_MOBMODEL[usMobType]->m_strLodFileName[0].length() )
			{
				int lodcount = 1;
				if( g_MOBMODEL[usMobType]->m_strLodFileName[1].length() )
				{
					lodcount = 2;
				}

				g_MOBMODEL[usMobType]->CreateLODObject( lodcount );

				for( int ilod = 0; ilod < lodcount; ++ilod )
				{
					if( !g_MOBMODEL[usMobType]->m_pLODObjectList[ilod]->Load_Runtime( 
									g_ModelArchive.GetPackedFile( const_cast<char*>(g_MOBMODEL[usMobType]->m_strLodFileName[ilod].c_str()) ), 
																  &g_MobTextureArchive, 
																  !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
																  const_cast<char*>(g_MOBMODEL[usMobType]->m_strLodFileName[ilod].c_str()) ) )
					{
						return FALSE;
					}
				}
			}
			
			/*
			if (g_MOBMODEL[usMobType]->m_pTextureController)
			{
				for(int i = 0; i < g_MOBMODEL[usMobType]->m_NodeCnt; i++)
				{
					g_MOBMODEL[usMobType]->m_pTextureController->AddRef( g_MOBMODEL[usMobType]->m_Node[i].nTextureId );
				}
			}
			*/

			OutputDebugString(g_MonsterName[usMobType]);
			OutputDebugString(" : Mob Loaded \n");
		}

		g_MobAttribTable[usMobType].xsize			= g_MOBMODEL[usMobType]->OBB_Extent[0];
		g_MobAttribTable[usMobType].ysize			= g_MOBMODEL[usMobType]->OBB_Extent[1];
		g_MobAttribTable[usMobType].collideradius	= g_MOBMODEL[usMobType]->m_ObjectRadius;
	}

	return TRUE;
}

void _XMobList::ReleaseReferenceModel(unsigned short usMobType, BOOL bConstraint) 
{
	if (bConstraint == FALSE && g_MOBBIPEDMODEL[usMobType].GetRef() == 1)
	{
		DELETE_MOB_INFO info;
		info.usMobType	= usMobType;
		info.fTime		= 30.f;

		m_slDeleteMotType.push_back(info);
		return;
	}

	if (g_MOBBIPEDMODEL[usMobType].GetRef() > 0)
		g_MOBBIPEDMODEL[usMobType].ReleaseRef();

	if (g_MOBBIPEDMODEL[usMobType].GetRef() == 0)
	{
		g_MOBBIPEDMODEL[usMobType].DestroyModel();
		//OutputDebugString(g_MOBBIPEDMODEL[usMobType].m_strBipedName.c_str());
		//OutputDebugString(" : Biped Deleted \n");
	}
	if( g_MOBMODEL[usMobType] )
	{
		g_MOBMODEL[usMobType]->ReleaseRef();
		if (g_MOBMODEL[usMobType]->GetRef() == 0)
		{
			/*
			if (g_MOBMODEL[usMobType]->m_pTextureController)
			{
				for(int i = 0; i < g_MOBMODEL[usMobType]->m_NodeCnt; i++)
				{
					g_MOBMODEL[usMobType]->m_pTextureController->ReleaseRef( g_MOBMODEL[usMobType]->m_Node[i].nTextureId );
				}
			}
			*/
			g_MOBMODEL[usMobType]->DestroyModel();
			//OutputDebugString(g_MonsterName[usMobType]);
			//OutputDebugString(" : Biped Deleted \n");
		}
	}
}


// Move가 아닐경우의 Create
_XMob* _XMobList::CreateMob(unsigned short usMobId, unsigned short usMobType, unsigned char ucMobWalkMode, 
							char cMobVitalRate,
							D3DXVECTOR3& vSrcPosition, D3DXVECTOR3& vDestPosition, bool bRebirth)
{
	if(usMobType > 4000) usMobType -= 4000;
	if(usMobType < 0 || usMobType >= _XDEF_MODELCLASSCOUNT_MOB)
	{
		_XDWINPRINT("Invalid Mob type [%d/%d]", usMobType, usMobId);
		return NULL;
	}

	if (AddReferenceModel(usMobType) == FALSE)
	{
		return NULL;
	}

	if( g_MobAttribTable[usMobType].scripttype == 85 || g_MobAttribTable[usMobType].scripttype == 90 )	return NULL;
 	
	_XMob* pMob = NewMob();
	pMob->m_MobID		= usMobId;

	pMob->m_Position	= vSrcPosition;
	pMob->m_EndPosition = vDestPosition;

	pMob->m_NextTransformType = pMob->m_MobType = usMobType;

	pMob->m_ModelDescriptor.ReserveModelStack(1);
	pMob->m_ModelDescriptor.LinkBipedObject(&g_MOBBIPEDMODEL[usMobType]);
	pMob->m_ModelDescriptor.SetMeshModel(0, g_MOBMODEL[usMobType]);

	pMob->CreateEffectManager();
	pMob->ResetAnimationState();

	pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
	pMob->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;

	pMob->m_ModelDescriptor.PlayAnimation(TRUE);

	if(bRebirth)
	{
		if(g_MobAttribTable[pMob->m_MobType].skill1_id == _XMOB_SKILL_REBIRTH)
			pMob->SetAnimationIndex(_XMOBACTIONNUMBER_SKILL1);
		else if(g_MobAttribTable[pMob->m_MobType].skill2_id == _XMOB_SKILL_REBIRTH)
			pMob->SetAnimationIndex(_XMOBACTIONNUMBER_SKILL2);
		else
			pMob->SetAnimation(_XMOBACTION_ENTERING);
		
		// 부활
		if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
		{
			if(((_XMob*)(g_pLocalUser->m_AttackTarget))->m_MobID == pMob->m_MobID)
			{
				g_pLocalUser->ResetAttackTarget();
				g_pLocalUser->ResetSkillAnimation();
//				g_pLocalUser->SetAnimation(_XACTION_IDLE);
			}
		}
	}
	else
		pMob->SetAnimation(_XMOBACTION_ENTERING);

	pMob->m_RotateAngle = _X_RAD(_XRandom() % 360);
	pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
	D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
	pMob->UpdatePosition();

	pMob->m_LastRecivedMessageTime = g_LocalSystemTime;

	if(cMobVitalRate < 0)
	{
		pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
		pMob->SetAnimation(_XMOBACTION_DIE);
		pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

/*		pMob->SetAnimation(_XMOBACTION_DIE);
		pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);*/
	}
	else
	{
		pMob->m_HealthPercentage = cMobVitalRate;
		pMob->m_DisplayHealthPercentage = cMobVitalRate;
	}

	if( g_MobAttribTable[pMob->m_MobType].standingerect )
	{
		pMob->m_ModelDescriptor.SetShadowDecalInfo( pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y,
													pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y * 2.0f,
												    pMob->m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 2.8f );
	}
	else
	{
		pMob->m_ModelDescriptor.SetShadowDecalInfo( 0.0f,
													pMob->m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 2.5f,
													pMob->m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 3.2f );
	}

	pMob->CheckInsideFrustum();

	_XDWPACKETLOG("New mob added [id:%d type:%d  insidefrustum:%d]", usMobId, usMobType, pMob->m_InsideFrustum);

	AddMob(pMob);

	return pMob;
}


// 움직이고 있을 경우의 Create
_XMob* _XMobList::CreateMobFromMove(unsigned short usMobId, unsigned short usMobType, unsigned char ucMobWalkMode, 
									char cMobVitalRate, D3DXVECTOR3& vSrcPosition, D3DXVECTOR3& vDestPosition)
{
	if(usMobType > 4000) usMobType -= 4000;
	if(usMobType < 0 || usMobType >= _XDEF_MODELCLASSCOUNT_MOB)
	{
		_XDWINPRINT("Invalid Mob type [%d/%d]", usMobType, usMobId);
		return NULL;
	}
	
	if(AddReferenceModel(usMobType) == FALSE)
	{
		return NULL;
	}

	if( g_MobAttribTable[usMobType].scripttype == 85 || g_MobAttribTable[usMobType].scripttype == 90 )	return NULL;
			
	_XMob* pMob = NewMob();
	pMob->m_MobID		= usMobId;
	
	pMob->m_Position	= vSrcPosition;
	pMob->m_EndPosition = vDestPosition;
	
	pMob->m_NextTransformType = pMob->m_MobType = usMobType;
	
	pMob->m_ModelDescriptor.ReserveModelStack(1);
	pMob->m_ModelDescriptor.LinkBipedObject(&g_MOBBIPEDMODEL[usMobType]);
	pMob->m_ModelDescriptor.SetMeshModel(0, g_MOBMODEL[usMobType]);
	
	pMob->CreateEffectManager();
	pMob->ResetAnimationState();
	
	pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
	pMob->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
	
	pMob->m_ModelDescriptor.PlayAnimation(TRUE);
	
	// LPS 모델일때는 IDLE MODE
	if( g_MobAttribTable[usMobType].scripttype > 80 && g_MobAttribTable[usMobType].scripttype < 90 )
	{
		pMob->SetAnimationIndex(_XMOBACTIONNUMBER_IDLE);
	}
	else
	{
		if( ucMobWalkMode == 0 )
			pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
		else
			pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RUN);
	}
	
	pMob->m_RotateAngle = _X_RAD(_XRandom() % 360);
	pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
	D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
	pMob->UpdatePosition();
	
	pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
	
	if(cMobVitalRate < 0)
	{
		pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
		pMob->SetAnimation(_XMOBACTION_DIE);
		pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

/*		pMob->SetAnimation(_XMOBACTION_DIE);
		pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);*/
	}
	else
	{
		pMob->m_HealthPercentage = cMobVitalRate;
		pMob->m_DisplayHealthPercentage = cMobVitalRate;
	}
	
	if( g_MobAttribTable[pMob->m_MobType].standingerect )
	{
		pMob->m_ModelDescriptor.SetShadowDecalInfo( pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y,
			pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y * 2.0f,
			pMob->m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 2.8f );
	}
	else
	{
		pMob->m_ModelDescriptor.SetShadowDecalInfo( 0.0f,
			pMob->m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 2.5f,
			pMob->m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 3.2f );
	}
		
	_XDWPACKETLOG("New mob added [id:%d type:%d  insidefrustum:%d]", usMobId, usMobType, pMob->m_InsideFrustum);
	
	AddMob(pMob);
	return pMob;
}

BOOL _XMobList::AddMob( _XMob* mob )
{
	m_smMobList.insert(smdef_XMob::value_type(mob->m_MobID, mob));

	return TRUE;
}

_XMob* _XMobList::FindMob( unsigned short mobid )
{
	smdef_XMob::iterator it = m_smMobList.find(mobid);
	if (it != m_smMobList.end())
	{
		return (*it).second;
	}
	return NULL;
}


int _XMobList::FindClientMobID( void )
{
	bool bexistmobid = false;
	int findmobid = 0;
	for( int mobid = 1; mobid < 200 ; ++mobid )
	{
		bexistmobid = false;
		smdef_XMob::iterator iter_mob;
		_XMob* currentmob = NULL;
		for(iter_mob = m_smMobList.begin() ; iter_mob != m_smMobList.end() ; ++iter_mob)
		{
			currentmob = iter_mob->second;
			if(currentmob)
			{
				if( currentmob->m_MobID == mobid )
				{
					bexistmobid = true;
					break;
				}
			}
		}
		if( !bexistmobid )
			return mobid;
	}
	return 0;
}


void _XMobList::DeleteMob( UINT mobid )
{
	smdef_XMob::iterator it = m_smMobList.find(mobid);
	if (it == m_smMobList.end())
	{
		return;
	}

	_XMob* mob = (*it).second;
	// by mahwang
	ReleaseReferenceModel(mob->m_MobType, TRUE);

	// DeleteMob(_XMob* mob) 으로 옮김

/*	if( mob == (_XMob*)g_LastPickedObject ) g_LastPickedObject = NULL;
	
	//BOOL bDead = FALSE;
	if( mob == (_XMob*)g_pLocalUser->m_AttackTarget )
	{
		//bDead = TRUE;
		g_pLocalUser->ResetAttackTarget();
	}
	if(mob == (_XMob*)g_pLocalUser->m_ViewTarget)
	{
		//bDead = TRUE;
		g_pLocalUser->ResetViewTarget();
	}
	
	//if(!bDead)
	{
		smdef_XUser::iterator dt;
		for (dt = g_Userlist.m_smUserList.begin(); dt != g_Userlist.m_smUserList.end(); ++ dt)
		{
			if( (*dt).second->m_AttackTarget )
			{
				if( (*dt).second->m_AttackTarget == mob )
				{
					(*dt).second->ResetAttackTarget();
				}
			}
		}
	}
	
	smdef_XMob::iterator iter_mob;
	for(iter_mob = m_smMobList.begin() ; iter_mob != m_smMobList.end() ; ++iter_mob)
	{
		_XMob* pMob = iter_mob->second;
		if(pMob)
		{
			if(pMob->m_AttackTarget)
			{
				if(pMob->m_AttackTarget == mob)
				{
					pMob->ResetAttackTarget();
				}
			}
		}
	}
*/
	DeleteMob(mob);
	m_smMobList.erase(it);
}

void _XMobList::Process( void )
{
	if( g_LocalSystemTime - m_LastPlaySoundTime > 3000 )
	{
		m_LastPlaySoundTime = g_LocalSystemTime;

		for( int mobindex = 0; mobindex < _XDEF_MODELCLASSCOUNT_MOB; ++mobindex )
		{
			g_MobAttribTable[mobindex].soundplayed = false;
		}
	}

	_XMob* _mob = NULL;
	smdef_XMob::iterator it = m_smMobList.begin();
	while(it != m_smMobList.end())
	{
		_mob = (*it).second;

		//FLOAT dx = (_mob->m_Position.x - /*g_pLocalUser->m_Position.x);//*/g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		//FLOAT dz = (_mob->m_Position.z - /*g_pLocalUser->m_Position.z);//*/g_LodTerrain.m_3PCamera.m_CameraPosition.z);
		//_mob->m_ModelDescriptor.m_CameraDistance = _XDistance(dx, dz);

		// 최종 메세지를 받은지 _XDEF_MOB_KILLTIME초 이상 메세지를 다시 못받았을 경우 몹 삭제.
		if( !g_SinglePlayMode )
		{ 
			if( (g_LocalSystemTime - _mob->m_LastRecivedMessageTime) > _XDEF_MOB_KILLTIME ) 
			{
				// 여기로 들어 오면 이래나 저래나 지줘주게 되어 있다.
				// 그래서 여기서 한번만 지워준다.
				VIEWPACKETLOG
					_XDWINPRINT("Monster kill time over [20s]  [id:%d] [type:%d]", _mob->m_MobID, _mob->m_MobType);
				
				DeleteMob( _mob );
				ReleaseReferenceModel( _mob->m_MobType );
				it = m_smMobList.erase(it);
			}
			else
			{
				if( _mob->m_KillSignal == _XDEF_MOB_KILLSIGNAL_END )
				{
					VIEWPACKETLOG
						_XDWINPRINT( "Process mob kill signal [id:%d] [type:%d]", _mob->m_MobID, _mob->m_MobType );
					
					DeleteMob( _mob );
					ReleaseReferenceModel( _mob->m_MobType );
					it = m_smMobList.erase(it);
				}
				else
				{
					if(_mob->m_HealthPercentage <= 0 && (g_LocalSystemTime - _mob->m_LastRecivedMessageTime) > 3000)
					{
						// 체력이 0이고 마지막 패킷 받은지 3초가 지났으면 Delete
						_mob->m_DisplayHealthPercentage = _mob->m_HealthPercentage;
						DeleteMob(_mob);
						ReleaseReferenceModel(_mob->m_MobType);
						it = m_smMobList.erase(it);
					}
					else
					{
						_mob->Process();
						++ it;
					}
				}
			}	
		}						
	}

	// by mahwang
	if (!m_slDeleteMotType.empty())
	{
		sldef_DeleteMobType::iterator it = m_slDeleteMotType.begin();
		while(it != m_slDeleteMotType.end())
		{
			DELETE_MOB_INFO& info = (*it);
			info.fTime -= g_fElapsedFrameMilisecondTime;
			if (info.fTime < 0.f)
			{
				ReleaseReferenceModel(info.usMobType, TRUE);
				it = m_slDeleteMotType.erase(it);
			}
			else
			{
				++ it;
			}
		}
	}
}

void _XMobList::Process_VirtualMode( void )
{
	FLOAT orgElapsedFrameTime = g_fElapsedFrameTime;
	
	if( g_LocalSystemTime - m_LastPlaySoundTime > 3000 )
	{
		m_LastPlaySoundTime = g_LocalSystemTime;
		
		for( int mobindex = 0; mobindex < _XDEF_MODELCLASSCOUNT_MOB; ++mobindex )
		{
			g_MobAttribTable[mobindex].soundplayed = false;
		}
	}
	
	_XMob* _mob = NULL;
	smdef_XMob::iterator it = m_smMobList.begin();
	while(it != m_smMobList.end())
	{
		_mob = (*it).second;	
		// 가상공간에서의 몹 삭제루틴은 다르게 처리 한다.
		if( _mob->m_KillSignal == _XDEF_MOB_KILLSIGNAL_END )
		{
			_XDWINPRINT( "Process mob kill signal [ %d ]", _mob->m_MobID );
			DeleteMob( _mob );
			ReleaseReferenceModel( _mob->m_MobType );
			it = m_smMobList.erase(it);
		}
		else
		{
			_mob->Process();
			++ it;
		}	
	}
	
	g_fElapsedFrameTime = orgElapsedFrameTime;
}

void _XMobList::Process_ZeroLevel( void )
{
	FLOAT orgElapsedFrameTime = g_fElapsedFrameTime;
	XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	// hotblood zerolevel
	if( pZeroLevelProcess && pZeroLevelProcess->GetTutorialModeOn() )
		g_fElapsedFrameTime = 0.0f;
	
	if( g_LocalSystemTime - m_LastPlaySoundTime > 3000 )
	{
		m_LastPlaySoundTime = g_LocalSystemTime;

		for( int mobindex = 0; mobindex < _XDEF_MODELCLASSCOUNT_MOB; ++mobindex )
		{
			g_MobAttribTable[mobindex].soundplayed = false;
		}
	}

	_XMob* _mob = NULL;
	smdef_XMob::iterator it = m_smMobList.begin();
	while(it != m_smMobList.end())
	{
		_mob = (*it).second;						
		if ( g_ZeroLevelMode )
		{
			// zero level 에서의 몹 삭제루틴은 다르게 처리 한다.
			if( _mob->m_KillSignal == _XDEF_MOB_KILLSIGNAL_END )
			{
				_XDWINPRINT( "Process mob kill signal [ %d ]", _mob->m_MobID );
				delete _mob;
				it = m_smMobList.erase(it);
			}
			else
			{
				_mob->Process();
				if( pZeroLevelProcess->GetTutorialModeOn() )
					_mob->m_PrevTime = 0.0f;
				++ it;
			}					
		}
	}

 	g_fElapsedFrameTime = orgElapsedFrameTime;
}

void _XMobList::Render( void )
{
	g_RenderMobCount = 0;

	_XMob* _mob = NULL;
	for (smdef_XMob::iterator it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
	{
		_mob = (*it).second;

		if(_mob->m_ModelDescriptor.m_CameraDistance <= g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
		{
			_mob->Render();
		}
		else
		{
			if(_mob->m_Action.action == _XMOBACTION_DIE)
			{
				_mob->m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
			}
		}

		// flustum culling...
/*		if( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( _mob->m_Position, _XDEF_MOBCOLLIDEFRUSTUM_DEFAULT ) != NOT_VISIBLE )
		{					
//			if( _mob->m_ModelDescriptor.m_CameraDistance < 30.0f )
			{
				_mob->Render();
			}
		}
		else
		{
			if( _mob->m_Action.action == _XMOBACTION_DIE )
			{
				_mob->m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
			}
		}*/
	}
}

void _XMobList::Render_ZeroLevel( void )
{
	_XMob* _mob = NULL;
	for (smdef_XMob::iterator it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
	{
		_mob = (*it).second;
		// flustum culling...
		if( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( _mob->m_Position, 3.5f ) != NOT_VISIBLE )
		{
			FLOAT dx = (g_pLocalUser->m_Position.x - _mob->m_Position.x);
			FLOAT dz = (g_pLocalUser->m_Position.z - _mob->m_Position.z);
			_mob->m_ModelDescriptor.m_CameraDistance = _XFastDistance( dx, dz );
		
			if( _mob->m_ModelDescriptor.m_CameraDistance < 50.0f )
			{
				_mob->Render();
			}
		}				
		else
		{
			if( _mob->m_Action.action == _XMOBACTION_DIE )
			{
				_mob->m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
			}
		}
	}
}

void _XMobList::RemoveAttackTarget( _XUser* user )
{
	_XMob* pMob = NULL;
	for (smdef_XMob::iterator it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
	{
		pMob = (*it).second;
		if( pMob->m_AttackTarget == user )
		{
			pMob->m_AttackTarget = NULL;
			pMob->m_AttackHitValue = 0.0f;
			pMob->m_AttackInfo = -1;
		}
	}
}

void _XMobList::RemoveEffectInstance( int templateid )
{
	_XMob* pMob = NULL;
	for (smdef_XMob::iterator it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
	{
		pMob = (*it).second;
		if( pMob->m_ModelDescriptor.m_EffectManager )
		{
			pMob->m_ModelDescriptor.m_EffectManager->DeleteInstance( templateid );
		}
	}
}

void _XMobList::CheckInsideFrustum( void )
{
	_XMob* pMob = NULL;
	for (smdef_XMob::iterator it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
	{
		pMob = (*it).second;
		FLOAT dx = (pMob->m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		FLOAT dz = (pMob->m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
		pMob->m_ModelDescriptor.m_CameraDistance = _XDistance( dx , dz );

		pMob->m_InsideFrustum = 
			( (pMob->m_ModelDescriptor.m_CameraDistance < g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS) &&
			( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pMob->m_Position, 3.5f ) != NOT_VISIBLE ) );
	}
}

void _XMobList::ClientMobAIProcess( void )
{
	if( g_pLocalUser->GetMotionClass() == _XACTION_DIE ) return;
		
	if( g_LocalSystemTime - m_ClientMobAIDelayTime > 1000 )
	{
		m_ClientMobAIDelayTime = g_LocalSystemTime;
		
		_XMob* pMob = NULL;
		for (smdef_XMob::iterator it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
		{
			pMob = (*it).second;

			if( pMob->m_bClientMob ) // 클라이언트 몬스터로 바꾼다.
			{
				if( !pMob->m_bClientDNPC )//클라이언트 몬스터
				{
					FLOAT dx = g_pLocalUser->m_Position.x - pMob->m_Position.x;
					FLOAT dz = g_pLocalUser->m_Position.z - pMob->m_Position.z;
					float length = _XFC_sqrt(dx*dx + dz*dz);
					if( length < 20)//g_MobAttribTable[pMob->m_MobType].attackrange )
					{
						if( pMob->m_Action.action == _XMOBACTION_IDLE )
						{
							if( length < 20.0f && !pMob->m_bHoming)	// 추격
							{
								pMob->m_MobActState = SA_MOVE;
								pMob->m_bTargetOn = TRUE;						
							}							
							
							if( pMob->m_bHoming )	// 되돌아가기
							{
								pMob->m_MobActState = SA_MOVE;
								FLOAT odx = pMob->m_OriginPosition.x - pMob->m_Position.x;
								FLOAT odz = pMob->m_OriginPosition.z - pMob->m_Position.z;
								float olength = _XFC_sqrt(odx*odx + odz*odz);
								
								if( olength < 0.3f )
								{
									pMob->m_bHoming = FALSE;
									pMob->m_bTargetOn = FALSE;
									pMob->m_RotateAngle = pMob->m_OldRotateAngle;
									pMob->UpdatePosition();
								}
							}
							if( length < 3.0f )
							{
								pMob->m_MobActState = SA_ATTACK;
							}						
							
						}									
						
						if( length > 20.0f && pMob->m_bTargetOn)	// 추격 이후 거리가 멀어지면 되돌아감 
						{
							pMob->m_bHoming = TRUE;					
						}
						
						switch( pMob->m_MobActState ) 
						{
						case SA_MOVE:
							{
								if( pMob->m_bHoming )
								{
									pMob->m_AttackTarget = NULL;
									pMob->m_EndPosition = pMob->m_OriginPosition;
									pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RUN);
								}
								else if( pMob->m_bTargetOn )
								{
									pMob->m_AttackTarget = g_pLocalUser;
									pMob->m_EndPosition = g_pLocalUser->m_Position;
									
									int tmp = _XRandom() % 3;
									int tmp1 = _XRandom() % 2;
									if( tmp1 == 1 )
									{
										pMob->m_EndPosition.x = g_pLocalUser->m_Position.x + tmp;
									}
									else
									{
										pMob->m_EndPosition.x = g_pLocalUser->m_Position.x - tmp;
									}

									tmp = _XRandom() % 3;
									tmp1 = _XRandom() % 2;
									if( tmp1 == 1 )
									{
										pMob->m_EndPosition.z = g_pLocalUser->m_Position.z + tmp;
									}
									else
									{
										pMob->m_EndPosition.z = g_pLocalUser->m_Position.z - tmp;
									}
									
									pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
									pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
									D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);

									//pMob->m_EndPosition = g_pLocalUser->m_Position + pMob->m_MoveVector;// * g_SkillProperty[pMob->m_SkillIndex]->distance;
									
									
									pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RUN);
									
									pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
									pMob->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
									
									pMob->m_ModelDescriptor.PlayAnimation(TRUE);
									pMob->UpdatePosition();
									pMob->m_SpeedRate = 50;
									
								}
								else
								{
									pMob->m_AttackTarget = NULL;
									pMob->m_EndPosition = pMob->m_Position;
									pMob->SetAnimation(_XMOBACTION_IDLE);
								}					
							}
							break;
						case SA_ATTACK:
							{
								if( pMob->m_Action.action == _XMOBACTION_IDLE )
								{
									pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, g_pLocalUser->m_Position);
									pMob->m_AttackTargetObjectType = _XPICKINGTARGET_SELF;
									pMob->m_AttackTarget = g_pLocalUser;								
									int tmp = _XRandom() % 30;
									if( tmp < 30 )		pMob->m_AttackInfo = 0;							
									if( tmp < 4 )		pMob->m_AttackInfo = 1;							
									
									pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK1);
									
									pMob->UpdatePosition();
									
									int lifepower = g_pLocalUser->m_CharacterInfo.Get_current_lifepower();
									if( pMob->m_AttackInfo == 1 )
										lifepower -= pMob->m_AttackHitValue*1.5f;
									else
										lifepower -= pMob->m_AttackHitValue;
									
									if( lifepower > 0 )
									{
										g_pLocalUser->m_CharacterInfo.Set_current_lifepower( lifepower );
									}
									else
									{
										// 캐릭터가 클라이언트 몹에게 죽지 못하게 한다.
									//	g_pLocalUser->m_CharacterInfo.current_lifepower = lifepower;
									//	g_pLocalUser->m_DyingStartTime = g_fLocalSystemTime;
									//	g_pLocalUser->SetDieMotion();
									}
									
									if( g_pJinStatus_Window )
									{
										g_pJinStatus_Window->SetParameter();	
									}		
								}					
							}
							break;				
						}
					}	
				}
				else// 클라이언트 DNPC
				{
					// 특정 경로를 따라 움직이도록 설정한다.
					if( pMob->m_EndPosition.x == -67.574074f &&  pMob->m_EndPosition.z == -61.161400f )
					{
						if((fabs(pMob->m_Position.x - pMob->m_EndPosition.x) <= EPSILON1) && (fabs(pMob->m_Position.z - pMob->m_EndPosition.z) <= EPSILON1))
						{
							pMob->SetAnimation(_XMOBACTIONNUMBER_WALK);
							
							pMob->m_Position.x = -67.574074f;
							pMob->m_Position.y = 0.0f;
							pMob->m_Position.z = -61.161400f;
						
							pMob->m_EndPosition.x = -57.605824f;
							pMob->m_EndPosition.y = 0.0f;
							pMob->m_EndPosition.z = -55.844189f;
						}
						else
						{
							pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
							pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
							D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
							
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
						}
					}
					else if( pMob->m_EndPosition.x == -57.605824f &&  pMob->m_EndPosition.z == -55.844189f )
					{
						if((fabs(pMob->m_Position.x - pMob->m_EndPosition.x) <= EPSILON1) && (fabs(pMob->m_Position.z - pMob->m_EndPosition.z) <= EPSILON1))
						{
							pMob->SetAnimation(_XMOBACTIONNUMBER_WALK);
							
							pMob->m_Position.x = -57.605824f;
							pMob->m_Position.y = 0.0f;
							pMob->m_Position.z = -55.844189f;
							
							pMob->m_EndPosition.x = -35.369267f;
							pMob->m_EndPosition.y = 0.0f;
							pMob->m_EndPosition.z =  -34.208004f;
						}
						else
						{
							pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
							pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
							D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
							
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
						}
					}
					else if( pMob->m_EndPosition.x == -35.369267f &&  pMob->m_EndPosition.z == -34.208004f )
					{
						if((fabs(pMob->m_Position.x - pMob->m_EndPosition.x) <= EPSILON1) && (fabs(pMob->m_Position.z - pMob->m_EndPosition.z) <= EPSILON1))
						{
							pMob->SetAnimation(_XMOBACTIONNUMBER_WALK);
							
							pMob->m_Position.x = -35.369267f;
							pMob->m_Position.y = 0.0f;
							pMob->m_Position.z =  -34.208004f;
							
							pMob->m_EndPosition.x = -21.004185f;
							pMob->m_EndPosition.y = 0.0f;
							pMob->m_EndPosition.z = -16.113670f;
						}
						else
						{
							pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
							pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
							D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
							
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
						}
					}
					else if( pMob->m_EndPosition.x == -21.004185f &&  pMob->m_EndPosition.z == -16.113670f )
					{
						if((fabs(pMob->m_Position.x - pMob->m_EndPosition.x) <= EPSILON1) && (fabs(pMob->m_Position.z - pMob->m_EndPosition.z) <= EPSILON1))
						{
							pMob->SetAnimation(_XMOBACTIONNUMBER_WALK);
							
							pMob->m_Position.x = -21.004185f;
							pMob->m_Position.y = 0.0f;
							pMob->m_Position.z = -16.113670f;
							
							pMob->m_EndPosition.x = -4.904758f;
							pMob->m_EndPosition.y = 0.0f;
							pMob->m_EndPosition.z = -10.110857f;
						}
						else
						{
							pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
							pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
							D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
							
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
						}
					}
					else if( pMob->m_EndPosition.x == -4.904758f &&  pMob->m_EndPosition.z == -10.110857f )
					{
						if((fabs(pMob->m_Position.x - pMob->m_EndPosition.x) <= EPSILON1) && (fabs(pMob->m_Position.z - pMob->m_EndPosition.z) <= EPSILON1))
						{
							pMob->SetAnimation(_XMOBACTIONNUMBER_WALK);
							
							pMob->m_Position.x = -4.904758f;
							pMob->m_Position.y = 0.0f;
							pMob->m_Position.z = -10.110857f;
							
							pMob->m_EndPosition.x = -1.257279f;
							pMob->m_EndPosition.y = 0.0f;
							pMob->m_EndPosition.z = -13.955442f;
							
						}
						else
						{
							pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
							pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
							D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
							
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
						}
					}
					else if( pMob->m_EndPosition.x == -1.257279f &&  pMob->m_EndPosition.z == -13.955442f )
					{
						if((fabs(pMob->m_Position.x - pMob->m_EndPosition.x) <= EPSILON1) && (fabs(pMob->m_Position.z - pMob->m_EndPosition.z) <= EPSILON1))
						{
							pMob->SetAnimation(_XMOBACTIONNUMBER_IDLE);
							//pMob->m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
							
						}
						else
						{
							pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
							pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
							D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
							
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
						}
					}
					
					
					pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
					pMob->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
					
					pMob->m_ModelDescriptor.PlayAnimation(TRUE);
					
					pMob->UpdatePosition();
				
				}
										
			}
		}
	}
	
}
// hotblood zerolevel
void _XMobList::MobAIProcess( void )
{
	if( g_pLocalUser->GetMotionClass() == _XACTION_DIE ) return;

	_XMob* pMob = NULL;
	for (smdef_XMob::iterator it = m_smMobList.begin(); it != m_smMobList.end(); ++ it)
	{
		pMob = (*it).second;

		FLOAT dx = g_pLocalUser->m_Position.x - pMob->m_Position.x;
		FLOAT dz = g_pLocalUser->m_Position.z - pMob->m_Position.z;
		float length = _XFC_sqrt(dx*dx + dz*dz);
		
		if( length < 50.0f)
		{
			if( pMob->m_Action.action == _XMOBACTION_IDLE )
			{
				if( length < 20.0f && !pMob->m_bHoming)	// 추격
				{
					pMob->m_MobActState = SA_MOVE;
					pMob->m_bTargetOn = TRUE;						
				}							
				
				if( pMob->m_bHoming )	// 되돌아가기
				{
					pMob->m_MobActState = SA_MOVE;
					FLOAT odx = pMob->m_OriginPosition.x - pMob->m_Position.x;
					FLOAT odz = pMob->m_OriginPosition.z - pMob->m_Position.z;
					float olength = _XFC_sqrt(odx*odx + odz*odz);
					
					if( olength < 0.1f )
					{
						pMob->m_bHoming = FALSE;
						pMob->m_bTargetOn = FALSE;
						pMob->m_RotateAngle = pMob->m_OldRotateAngle;
						pMob->UpdatePosition();
					}
				}				
				
				if( length < g_SkillProperty[pMob->m_SkillIndex]->distance )
				{
					pMob->m_MobActState = SA_ATTACK;
				}				
			}									
			
			if( length > 20.0f && pMob->m_bTargetOn)	// 추격 이후 거리가 멀어지면 되돌아감 
			{
				pMob->m_bHoming = TRUE;					
			}
			
			switch( pMob->m_MobActState ) 
			{
			case SA_MOVE:
				{
					if( pMob->m_bHoming )
					{
						pMob->m_AttackTarget = NULL;
						pMob->m_EndPosition = pMob->m_OriginPosition;
						pMob->SetCharacterMobAnimation(_XMOBACTION_MOVE);
					}
					else if( pMob->m_bTargetOn )
					{
						pMob->m_AttackTarget = g_pLocalUser;
						D3DXVECTOR3 tmpDir = pMob->m_Position - g_pLocalUser->m_Position;
						D3DXVec3Normalize(&tmpDir, &tmpDir);
						pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
						pMob->m_EndPosition = g_pLocalUser->m_Position + tmpDir * g_SkillProperty[pMob->m_SkillIndex]->distance;
						pMob->SetCharacterMobAnimation(_XMOBACTION_MOVE);
					}
					else
					{
						pMob->m_AttackTarget = NULL;
						pMob->m_EndPosition = pMob->m_Position;
						pMob->SetCharacterMobAnimation(_XMOBACTION_IDLE);
					}					
				}
				break;
			case SA_ATTACK:
				{
					if( pMob->m_Action.action == _XMOBACTION_IDLE )
					{
						pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
						pMob->m_AttackTarget = g_pLocalUser;								
						int tmp = _XRandom() % 30;
						if( tmp < 30 )		pMob->m_AttackInfo = 0;							
						if( tmp < 4 )		pMob->m_AttackInfo = 1;							
						
						pMob->SetCharacterMobAnimation(_XMOBACTION_ATTACK, FALSE);						
						pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
						pMob->m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
						pMob->UpdatePosition();
						
						if( pMob->m_AttackInfo == 1 )
							g_pLocalUser->m_CharacterInfo.Set_current_lifepower( g_pLocalUser->m_CharacterInfo.Get_current_lifepower() - (pMob->m_AttackHitValue*1.5f) );
						else
							g_pLocalUser->m_CharacterInfo.Set_current_lifepower( g_pLocalUser->m_CharacterInfo.Get_current_lifepower() - pMob->m_AttackHitValue );
						
						if( g_pLocalUser->m_CharacterInfo.Get_current_lifepower() < 0 )
						{
							g_pLocalUser->m_DyingStartTime = g_fLocalSystemTime;
							g_pLocalUser->SetDieMotion();
						}
						
						if( g_pJinStatus_Window )
						{
							g_pJinStatus_Window->SetParameter();	
						}		
					}					
				}
				break;				
			}
		}									
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// _XZeroLevelMob

void _XZeroLevelMob::UpdatePosition( void )
{
	D3DXMATRIX	rotmat;
	D3DXVECTOR3	terrainnormal;
	D3DXMatrixIdentity( &rotmat );
	D3DXMatrixRotationY( &rotmat, m_RotateAngle );
	m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( m_Position, &terrainnormal );	
	D3DXMatrixTranslation( &m_ModelDescriptor.m_Position, m_Position.x, m_Position.y, m_Position.z );	
	D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position );		
}

void _XZeroLevelMob::Render( void )	// hotblood zerolevel
{
	if( m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP ) 
	{
		m_ModelDescriptor.Animation();
	}
	else // 죽기시 정지 동작 별도 처리
	{		
		//m_Action.action = _XMOBACTION_DIE;
		//m_ModelDescriptor.SetCurMotion( _XMOBACTIONNUMBER_DIE, FALSE );
		m_ModelDescriptor.m_EventHandler.soundplayed[0] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[1] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[2] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[3] = 1;
		m_ModelDescriptor.m_EventHandler.soundplayed[4] = 1;

		_LPXM_MOTION pMotion = NULL;
		pMotion = m_ModelDescriptor.m_pBipedController->GetMotion( _XAN_DIE );
		
		if(pMotion)
		{			
			m_ModelDescriptor.Animation(pMotion->EndFrame-1);
		}
		else
		{
			return;
		}
	}
		
	BOOL intersect = FALSE;
	
	if( m_Action.action != _XMOBACTION_DIE ) // 몹이 죽기모드로 들어가면 픽킹을 막아서 재공격이 불가능 하게 만듬.
	{			
		D3DXVECTOR3 nRayDir;
		D3DXVECTOR3 mpos = m_Position;
		mpos.y += m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1];
		D3DXVec3Normalize( &nRayDir, &g_vPickRayDir );

		// Check intersect sphere space...
		intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius );
		
		// Check intersect obb
		if( intersect )
		{
			D3DXVECTOR3 PickedPoint;
			intersect = m_ModelDescriptor.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, m_Position, m_RotateAngle, &PickedPoint );
						
			if( intersect )
			{	
				FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - PickedPoint.x);
				FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - PickedPoint.z);
				FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
				
				if( _distance < g_LastPickedObjectDistance )
				{
					if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
					{
						g_LastPickedObjectDistance = _distance;
						g_LastPickedObjectType = _XPICKINGTARGET_MOB;
						g_LastPickedObject = this;
					}
				}
			}	
		}
	}

	if( m_ModelDescriptor.m_CameraDistance < 12.0f )
	{
		if( m_KillSignal != _XDEF_MOB_KILLSIGNAL_START && ( m_AlphaLevel >= 255 ) )
			m_ModelDescriptor.RenderShadow();
	}	

	m_ModelDescriptor.m_ViewLeftSwordEffect = ( m_Action.action == _XMOBACTION_ATTACK );	
	
	if( m_KillSignal == _XDEF_MOB_KILLSIGNAL_STOP ) // 몹이 처음 등장했을 경우 페이드 아웃 처리
	{
		if( m_AlphaLevel < 255 )
		{						
			m_AlphaLevel+=_XDEF_MOBFADEOUT_RATE;
			if( m_AlphaLevel > 255 ) m_AlphaLevel = 255;

			g_RenderAlphaBlendFlag = TRUE;
			g_RenderAlphaBlendTextureFactor = ( (m_AlphaLevel<<24) | 0xFFFFFF );
			g_LastUsedRenderStyle = NULL;
			m_ModelDescriptor.Render_ZeroLevelMob( this );
			g_RenderAlphaBlendTextureFactor = 0xFFFFFFFF;
			g_RenderAlphaBlendFlag = FALSE;					
			gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE); 
		}
		else
		{
			// 맞았을때 메터리얼 변경
			if( m_Action.action == _XMOBACTION_BLOW &&
				m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
			{
				g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
				g_LastUsedRenderStyle = NULL;
				m_ModelDescriptor.Render_ZeroLevelMob( this );
				g_pCurrentSelectMaterial = NULL;				
			}
			else
			{				
				if( g_LastPickedObject == (_XItem*)this )
				{
					g_pCurrentSelectMaterial = &g_SelectedObjectMaterial;
					g_LastUsedRenderStyle = NULL;
					m_ModelDescriptor.Render_ZeroLevelMob( this );
					g_pCurrentSelectMaterial = NULL;					
				}
				else
				{
					m_ModelDescriptor.Render_ZeroLevelMob( this );
				}
			}
		}
	}
	else if( m_KillSignal == _XDEF_MOB_KILLSIGNAL_START )  // 몹이 죽었을 경우 사라지는 애니메이션 처리		
	{
		if( m_AlphaLevel > 0 )
		{	
			if( m_DieToDeleteTimer == 0 ) m_DieToDeleteTimer = g_LocalSystemTime;
			if( (g_LocalSystemTime - m_DieToDeleteTimer) > _XDEF_MOBDIETODELETETIMER )
			{			
				if( (m_AlphaLevel-_XDEF_MOBFADEIN_RATE) <= 0 )
				{
					m_AlphaLevel = 0;
					m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
					return;
				}
				else
				{
					m_AlphaLevel-=_XDEF_MOBFADEIN_RATE;
					m_ModelDescriptor.m_Position._42 -= 0.003f; // 땅 밑으로
				}
			}
		}
		else
		{
			m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
			return;
		}
		g_RenderAlphaBlendFlag = TRUE;
		g_RenderAlphaBlendTextureFactor = ( (m_AlphaLevel<<24) | 0xFFFFFF );
		g_LastUsedRenderStyle = NULL;
		m_ModelDescriptor.Render_ZeroLevelMob( this );
		g_RenderAlphaBlendTextureFactor = 0xFFFFFFFF;
		g_RenderAlphaBlendFlag = FALSE;
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE); 
	}
			

	//2004.07.05->oneway48 modify
	if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
	{		
		if(g_pLocalUser->m_AttackTarget) 
		{
			if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				if( this == (_XMob*)g_pLocalUser->m_AttackTarget )
				{
					g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
					
					gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );	
					gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );
					
					D3DXVECTOR3 mpos, rvec;
					
					mpos.x = m_ModelDescriptor.m_Position._41;
					mpos.y = m_ModelDescriptor.m_Position._42 + (m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2)+0.4f;
					mpos.z = m_ModelDescriptor.m_Position._43;
					
					D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
						&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
					
					g_pLocalUser->m_imageMiniHpBackBorder.Draw( rvec.x-54, rvec.y+19 );
					g_pLocalUser->m_MonsterMiniHpBorder.MoveWindow( rvec.x-45, rvec.y+20  );
					g_pLocalUser->m_MonsterMiniHpBorder.SetGaugeFactor( this->m_DisplayHealthPercentage );
					g_pLocalUser->m_MonsterMiniHpBorder.Process();
					g_pLocalUser->m_MonsterMiniHpBorder.Draw();
					
				}
			}
		}
	}
}
