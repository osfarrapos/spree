 // XHeroLocalUser.cpp: implementation of the _XHeroLocalUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XHeroActionTable.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XParticleIDDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XHeroLocalUser.h"
#include "XParticleAction.h"
#include "XWindow_JinStatus.h"

#define	_XDEF_IMPACTPOWER		0.5f
#define _XDEF_FOVINCREASERATE	3.0f
#define _XDEF_FOVDECREASERATE	5.0f					
#define _XDEF_MAXFOV			(g_DefaultCameraFov + 30.0f)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XHeroLocalUser::_XHeroLocalUser()
{
	m_bSpecialFlag = TRUE;	
	fLifeUpDelayTime = 0.0f;
}

_XHeroLocalUser::~_XHeroLocalUser()
{
	m_MiniHpBorder.DestroyWindow();
	m_MonsterMiniHpBorder.DestroyWindow();
}

void _XHeroLocalUser::SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion )
{
	if( motionclass < 0 ) return;

	m_PrevMoveTime = 0;

	switch(motionclass) 
	{
	case _XACTION_IDLE :
		{
			if( m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE )
			{
				m_ModelDescriptor.SetCurMotion( _XHAN_IDLE_NONE, TRUE );
			}
			else
			{
				if( m_ActiveSlotID > 0 )
				{
					short stancemotion = -1;
					stancemotion = _XHAN_STANCE;
					if( m_StanceMotionFirst )
					{
						if(stancemotion >= 0)
						{
							m_ModelDescriptor.SetCurMotion(stancemotion, TRUE);
							m_StanceMotionFirst = FALSE;
						}
						m_StanceMotionFirst = FALSE;
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XHAN_BATTLEIDLE_NONE, TRUE );											
					}
				}
				else
				{
					m_ModelDescriptor.SetCurMotion( _XHAN_BATTLEIDLE_NONE, TRUE );
				}
			}
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		}
		break;
	case _XACTION_MOVE :
		{
			SetMoveSpeed( _XDEF_CHARACTERMOVESPEED_RUN );
			m_ModelDescriptor.SetCurMotion( _XHAN_MOVE, TRUE );			
		}
		break;
	case _XACTION_ATTACK :
		{
			if( m_SelectedSkillID > 0 )
			{
				char usefultype = g_SkillProperty[m_SelectedSkillID]->usefulType;
				if( usefultype == _XSI_USEFULTYPE_ACTIVESKILL )
					m_ModelDescriptor.SetCurMotion( _XHAN_ATTACK1, changemotion );
				else if( usefultype == _XSI_USEFULTYPE_CHARGESKILL )
					m_ModelDescriptor.SetCurMotion( _XHAN_ATTACK2, changemotion );
			}
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );			
		}
		break;	
	case _XACTION_BLOW :
		{
			m_ModelDescriptor.SetCurMotion( _XHAN_BLOW, changemotion );			
		}
		break;
	case _XACTION_DIE :
		{
			m_ModelDescriptor.SetCurMotion( _XHAN_DIE, changemotion );			
		}
		break;
	case _XACTION_SPECIAL :
		{
			if( m_bSpecialFlag )
				m_ModelDescriptor.SetCurMotion( _XHAN_SPECIAL2, changemotion );
			else
				m_ModelDescriptor.SetCurMotion( _XHAN_SPECIAL1, changemotion );
		}
		break;
	case _XACTION_TALK :
		{
			m_ModelDescriptor.SetCurMotion( _XHAN_TALK, changemotion );
		}
		break;
	default:
		break;
	}

	m_SpecialActionTimer = g_LocalSystemTime;

	m_CharacterInfo.animationdescript.motionclass = motionclass;

	m_ModelDescriptor.m_FrameResult = _XDEF_ACI_NONE;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (motionclass == _XACTION_ATTACK);
}

void _XHeroLocalUser::SetAnimationIndex( int animationindex, bool changemotion )
{	
	// 이동 시작 또는 종료 동작교체시는 이동시작시간 초기화 
	m_PrevMoveTime = 0;
	
	if(animationindex < 0)
		return;

	if( g_LocalSystemTime == 0.0f )
		 m_PrevProcessTime = -1.0f;
	else m_PrevProcessTime = g_fLocalSystemTime;
	
	switch( animationindex )
	{
	case _XHAN_IDLE_NONE				:
		m_CharacterInfo.animationdescript.idletype			= _XBATTLE_PEACE;
		m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );	
		break;	
	case _XHAN_STANCE					:
	case _XHAN_BATTLEIDLE_NONE			:
		m_CharacterInfo.animationdescript.idletype			= _XBATTLE_BATTLE;
		m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;
	case _XHAN_MOVE						:
		m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
		m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
		SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
		break;	
	case _XHAN_BLOW						:
		m_CharacterInfo.animationdescript.motionclass		= _XACTION_BLOW;
		break;
	case _XHAN_DIE						:
		m_CharacterInfo.animationdescript.motionclass		= _XACTION_DIE;		
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;
	case _XHAN_SPECIAL1					:		
	case _XHAN_SPECIAL2					:
		m_CharacterInfo.animationdescript.motionclass		= _XACTION_SPECIAL;		
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;
	case _XHAN_TALK						:
		m_CharacterInfo.animationdescript.motionclass		= _XACTION_TALK;		
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;		
	default :
		m_CharacterInfo.animationdescript.motionclass = _XACTION_ATTACK;
		m_CharacterInfo.animationdescript.attacktype = animationindex;
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;
	}	

	m_ModelDescriptor.SetCurMotion( animationindex, changemotion );

	m_ModelDescriptor.m_FrameResult = _XDEF_ACI_NONE;
	m_SpecialActionTimer = g_LocalSystemTime;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK);
}

// CheckDistanceToTarget() : target이 공격거리에 들어와있으면 TRUE, 아니면 FALSE
BOOL _XHeroLocalUser::CheckDistanceToTarget(D3DXVECTOR3 targetposition, FLOAT& range, unsigned short mobType)
{
	FLOAT dx = m_Position.x - targetposition.x;
	FLOAT dz = m_Position.z - targetposition.z;
	FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

	if(m_SelectedSkillID == 0)
		range = 0.6f + m_AttackRange;
	else
		range = 0.6f + m_AttackRange + g_SkillProperty[m_SelectedSkillID]->distance;

	if(distance <= range)
	{
		m_TargetPosition = m_Position;
		return TRUE;
	}
	else
		return FALSE;
}

// SetSkillAnimation - Active skill 용
void _XHeroLocalUser::SetSkillAnimation(void)
{	
	short motionindex = -1;
	if( m_SelectedSkillID > 0 )
	{
		char usefultype = g_SkillProperty[m_SelectedSkillID]->usefulType;
		if( usefultype == _XSI_USEFULTYPE_ACTIVESKILL )
			motionindex = _XHAN_ATTACK1;
		else if( usefultype == _XSI_USEFULTYPE_CHARGESKILL )
			motionindex = _XHAN_ATTACK2;
		else
			motionindex = _XHAN_ATTACK1;
	}		
		
	m_MartialStep = 2;				
	// 자르기 전
	m_CharacterInfo.animationdescript.attacktype = motionindex;
}

void _XHeroLocalUser::UpdateLocalUserAttackLevel( void )
{
	if( m_SelectedSkillID > 0 )
	{
		char usefultype = g_SkillProperty[m_SelectedSkillID]->usefulType;
		if( usefultype == _XSI_USEFULTYPE_ACTIVESKILL )
		{
			m_CharacterInfo.minattacklevel = 500;
			m_CharacterInfo.maxattacklevel = 800;
		}		
		else if( usefultype == _XSI_USEFULTYPE_CHARGESKILL )
		{
			m_CharacterInfo.minattacklevel = 2500;
			m_CharacterInfo.maxattacklevel = 3000;
		}		
	}	
	// 공격 성공률	
	m_CharacterInfo.attacksuccesslevel = 100;
	
	// 방어력
	m_CharacterInfo.defencelevel	= 220;
}

short _XHeroLocalUser::GetStanceMotion(short activeslotid)
{
	return _XHAN_STANCE;
}

short _XHeroLocalUser::GetMotionIndex(short selectedskillid)
{
	short motionindex = -1;
	if( selectedskillid > 0 )
	{
		char usefultype = g_SkillProperty[selectedskillid]->usefulType;
		if( usefultype == _XSI_USEFULTYPE_ACTIVESKILL )
			motionindex = _XHAN_ATTACK1;
		else if( usefultype == _XSI_USEFULTYPE_CHARGESKILL )
			motionindex = _XHAN_ATTACK2;		
	}	
	return motionindex;	
}

void _XHeroLocalUser::ProcessAnimation(void)
{
	// 자동회복처리 보류
	//if( g_fLocalSystemTime - fLifeUpDelayTime > 10000)
	if(GetMotionClass() == _XACTION_IDLE)
	{
		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_MOVE)
	{
		if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XHAN_MOVE		 :
					{
						SetAnimationIndex( _XHAN_MOVE );// Repeate						
					}
					break;	
				}
			}
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_DYING)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);		

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_DIE)
	{
		if( g_fLocalSystemTime - m_DyingStartTime > 3000 )   	
		{
			g_pLocalUser->m_CharacterInfo.Set_current_lifepower( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );
			SetAnimation(_XACTION_IDLE);
		}
		
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );

			int rand = _XRandom() % 2;
			
			if( m_CharacterInfo.gender == _XGENDER_MAN )
				_XPlaySoundEffect( g_DieEffectSoundIndex[0][rand] , m_Position );
			else 
				_XPlaySoundEffect( g_DieEffectSoundIndex[1][rand] , m_Position );
		}

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_StopAnimation = TRUE;
			m_ModelDescriptor.SetLastFrame();
//			m_ModelDescriptor.StopAnimation();
		}		
	}
	else if(GetMotionClass() == _XACTION_RESCUE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			SetAnimation(_XACTION_IDLE);
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_REBIRTH)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );

#ifdef _XDEF_USESCREEENMOTIONBLUR 
			if(g_ScrrenMotionBlurMode)
				g_ScrrenMotionBlurMode = FALSE;
#endif
			
			SetAnimation(_XACTION_IDLE);
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
}

void _XHeroLocalUser::ProcessBattleChargeSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					FLOAT range = 0.0f;
					m_AttackResult.ucToType = en_target_type_monster;
					
					if(CheckDistanceToTarget(pMob->m_Position, range, pMob->m_MobType))
					{
						m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
						UpdatePosition();
						
						g_pLocalUser->m_AttackResult.usDefenderID = pMob->m_MobID;
						m_CurrentBattleState = _XUSER_STATE_2;
					}
					else
					{
						SetTargetPosition(pMob->m_Position, range);
					}				
				}
				else
					ResetSkillAnimation();
			}
			else
			{
				// target이 없음
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					m_ChargeSendStartPacket = TRUE;
					m_CurrentBattleState = _XUSER_STATE_3;
				}
				else
					ResetSkillAnimation();
			}
			else
			{
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);			
			SetAnimationIndex(motionindex + _XANI_ORDER_READY);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_4;
		}
		break;
	case _XUSER_STATE_4 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				short motionindex = GetMotionIndex(m_SelectedSkillID);
				m_ModelDescriptor.SetLockEndFrame(FALSE);
				
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
				{
					// 시전 animation 끝
					m_CurrentBattleState = _XUSER_STATE_5;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);			
			SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
			m_ChargeStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_6;
		}
		break;
	case _XUSER_STATE_6 :
		{
			// key up이면 state_7으로
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);					
					UpdatePosition();
				}
			}
		}
		break;
	case _XUSER_STATE_7 :
		{
			// charge time 계산하기
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			int casting = 0;
			if(m_SelectedSkillItem->m_cSkillLevel == 0)
				casting = g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].casting;
			else
				casting = g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].casting 
								- g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel-1].casting;
			
			FLOAT fchargetime = ((m_ChargeIterateValue * casting) + (g_LocalSystemTime - m_ChargeStartTime)) / 100.0f;
			short chargetime = (short)fchargetime;

			int tmp = _XRandom()%20;
			UpdateLocalUserAttackLevel();
			if( tmp < 5 )
			{
				m_AttackResult.usAtkInfo[0] = 1;
				m_AttackResult.sDamage[0] = m_CharacterInfo.maxattacklevel;
			}
			else
			{
				m_AttackResult.usAtkInfo[0] = 0;
				m_AttackResult.sDamage[0] = m_CharacterInfo.minattacklevel;
			}
			m_AttackResult.display[0] = TRUE;

			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);			
			m_CurrentBattleState = _XUSER_STATE_8;
		}
		break;
	case _XUSER_STATE_8 :
		{
			// draw result
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				if( m_AttackResult.ucToType == en_target_type_monster )
				{
					_XMob* pAttackTarget = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
					_XMob* pMob = NULL;
					
					if( g_Moblist.m_smMobList.empty() ) return;	
					
					smdef_XMob::iterator it;
					for (it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ it)
					{
						pMob = (*it).second;

						FLOAT dx = (pMob->m_Position.x - pAttackTarget->m_Position.x);
						FLOAT dz = (pMob->m_Position.z - pAttackTarget->m_Position.z);
						float length = _XFC_sqrt(dx*dx + dz*dz);
						
						if( length < 1.0f )
						{
							if(m_AttackResult.display[0])
							{
								switch(m_AttackResult.usAtkInfo[0])
								{
								case 0 :
									{
										// Hit
										_XDWINPRINT("-- Attack result : Charge [HIT / damage:%d]", m_AttackResult.sDamage[0]);
										PlayAttackSound();
										
										// 타격 이펙트 삽입
										D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
										offsetmat._42 += ( 0.15f + 1.0f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
										if(pMob->m_ModelDescriptor.m_EffectManager)
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );
									}
									break;
								case 1 :
									{
										// Critical
										_XDWINPRINT("-- Attack result : Charge [CRITICAL / damage:%d]", m_AttackResult.sDamage[0]);
										pMob->SetBlowMotion();							
										PlayAttackSound();
										
										// 타격 이펙트 삽입
										D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
										offsetmat._42 += ( 0.15f + 1.0f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
										if(pMob->m_ModelDescriptor.m_EffectManager)
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat );
									}
									break;
								case 2 :
									{
										// Miss
										_XDWINPRINT("-- Attack result : Charge [MISS / damage:%d]", m_AttackResult.sDamage[0]);
										int rand = _XRandom() % 3;
										_XPlaySoundEffect(g_DefenceEffectSoundIndex[m_CharacterInfo.gender][rand], m_Position, true);
									}
									break;								
								}
								
								pMob->m_CurrentLife -= m_AttackResult.sDamage[0];
								if ( pMob->m_CurrentLife <= 0 )
								{
									pMob->m_HealthPercentage = -1;
								}
								else
									pMob->m_HealthPercentage = char(pMob->m_CurrentLife*100/pMob->m_MaxLife);
								
								if( pMob->m_HealthPercentage < 0 )
								{
									pMob->SetCharacterMobAnimation(_XMOBACTION_DIE);
									pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

									if(pMob == (_XMob*)g_LastPickedObject)
									g_LastPickedObject = NULL;
									
									if(pMob == (_XMob*)m_AttackTarget)
										ResetAttackTarget();
									if(pMob == (_XMob*)m_ViewTarget)
										ResetViewTarget();
									
									if(m_AttackResult.sDamage[0] == -1)
									{
										// 내가 죽인 몹
//										m_CharacterLog.isKill = TRUE;
//										m_CharacterLog.killTime = g_ServerTimeCode;
//										m_CharacterLog.killTargetType = pMob->m_MobType;

										
										_XDWINPRINT("[LOCAL USER/ProcessBattleChargeSkill] Mob[%d] kill", pMob->m_MobType);
									}
								}
							}
						}
					}
					m_AttackResult.display[0] = FALSE;
				}				
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_AttackResult.ucToType == en_target_type_monster)
					ProcessMobDie();

				short motionindex = GetMotionIndex(m_SelectedSkillID);				
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
				{
					SetAnimation(_XACTION_IDLE);
					m_CurrentBattleState = _XUSER_STATE_9;
				}
			}
		}
		break;
	case _XUSER_STATE_9 :
		{
			m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
			m_ChangeDelayStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			int cooldown = g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].coolDown;
			if(cooldown > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > cooldown)
				{
					TCHAR messagestring[512];
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					ResetSkillAnimation();
				}
			}
			else
			{
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XHeroLocalUser::ProcessBattleActiveSkill(void)
{
	switch(GetMotionClass())
	{
	case _XACTION_IDLE :
		{
			if(m_ModelDescriptor.m_LockEndFrame)
				m_ModelDescriptor.SetLockEndFrame(FALSE);

			if(m_AttackTarget)
			{
				if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB )
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					FLOAT range = 0.0f;

					if(CheckDistanceToTarget(pMob->m_Position, range, pMob->m_MobType))
						SetAnimation(_XACTION_ATTACK);
					else
						SetTargetPosition(pMob->m_Position, range);				
				}
			}
			else
			{
#ifdef _XDWDEBUG
				m_ChatBalloonObject.m_Show = FALSE;
#endif
			}
		}
		break;
	case _XACTION_MOVE :
		{
			if(m_ModelDescriptor.m_LockEndFrame)
				m_ModelDescriptor.SetLockEndFrame(FALSE);

			if(m_CharacterInfo.animationdescript.movetype != _XMOVETYPE_KYUNGGONG)
			{
				if(m_AttackTarget)
				{
					if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB )
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						FLOAT range = 0.0f;

						if(CheckDistanceToTarget(pMob->m_Position, range, pMob->m_MobType))
							SetAnimation(_XACTION_ATTACK);
						else
							SetTargetPosition(pMob->m_Position, range);						
					}
				}
				else
				{
#ifdef _XDWDEBUG
					m_ChatBalloonObject.m_Show = FALSE;
#endif
				}
			}
		}
		break;
	case _XACTION_ATTACK :
		{
			if(m_ModelDescriptor.m_pBipedController->GetFirstFrame()+5 >= m_ModelDescriptor.m_AniFrame &&
							m_ModelDescriptor.m_AniFrame > 0.0f && !m_bCheckFirstFrame)
			{
				if(m_AttackTarget)
				{
					if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB )
					{
						// 초식 시작
						_XMob* pMob = (_XMob*)m_AttackTarget;
						FLOAT range = 0.0f;
						m_bCheckFirstFrame = TRUE;
						m_AttackResult.ucToType = en_target_type_monster;

						if(CheckDistanceToTarget(pMob->m_Position, range, pMob->m_MobType))
						{
							// 공격
							m_CurrentAttackSequence = _XDEF_ATK_SEQ_FIRST;
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, m_CurrentAttackSequence-1);
							
							if(m_SelectedSkillID > 0)
								m_AttackPointNumber = g_SkillProperty[m_SelectedSkillID]->attackPointFor1;
							else
								m_AttackPointNumber = 2;
							m_CurrentAttackPoint = 0;
							
							// target 쪽으로 돌아선다.
							m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
							UpdatePosition();
							
							_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
							if(pMinimap_Window)
							{
								pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
								pMinimap_Window->SetLocalUserPosition();
							}
						}
						else
						{
							// 몹을 따라간다
							SetTargetPosition(pMob->m_Position, range);
							SetAnimation(_XACTION_MOVE);
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
					}
					else
						break;
				}
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_SECONDFRAME || m_ModelDescriptor.m_FrameResult & _XDEF_ACI_THIRDFRAME)
			{
				if(m_AttackTarget)
				{
					if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB )
					{
						// 각 초식 시작
						_XMob* pMob = (_XMob*)m_AttackTarget;
						FLOAT range = 0.0f;
						m_bCheckFirstFrame = FALSE;

						if(CheckDistanceToTarget(pMob->m_Position, range, pMob->m_MobType))
						{
							// 공격
							m_CurrentAttackSequence = (_XUSER_ATTACKSEQUENCE)((int)m_CurrentAttackSequence + 1);
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, m_CurrentAttackSequence-1);

							if(m_SelectedSkillID > 0)
								m_AttackPointNumber = g_SkillProperty[m_SelectedSkillID]->attackPointFor2 - g_SkillProperty[m_SelectedSkillID]->attackPointFor1;
							else
								m_AttackPointNumber = 2;
							m_CurrentAttackPoint = 0;

							m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
							UpdatePosition();

							_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
							if(pMinimap_Window)
							{
								pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
								pMinimap_Window->SetLocalUserPosition();
							}
						}
						else
						{
							// 몹을 따라간다
							SetTargetPosition(pMob->m_Position, range);
							SetAnimation(_XACTION_MOVE);
							//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
					}
					else
						break;
				}
			}
			
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
				m_bCheckFirstFrame = FALSE;
				if( m_AttackTarget )
				{
					m_ChangeDelayStartTime = g_LocalSystemTime;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
						ProcessMobDie();
				}
				else
				{
					SetAnimation(_XACTION_IDLE);					
#ifdef _XDWDEBUG
					m_ChatBalloonObject.m_Show = FALSE;
#endif
				}
			}

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				if(!m_AttackTarget)
					break;
				if(m_AttackTargetObjectType != _XPICKINGTARGET_MOB)
					break;

				m_CurrentAttackPoint++;
				m_bCheckFirstFrame = FALSE;
				
				if(m_AttackResult.ucToType == en_target_type_monster)
				{
					_XMob* pMob = NULL;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						pMob = (_XMob*)m_AttackTarget;
					}					
					if(!pMob)	
						break;

					// damage 계산
					int tmp = _XRandom()%100;
					UpdateLocalUserAttackLevel();
					if( tmp > 30 )
					{
						m_AttackResult.usAtkInfo[m_CurrentAttackSequence-1] = 0;
						m_AttackResult.sDamage[m_CurrentAttackSequence-1] = m_CharacterInfo.maxattacklevel;
						pMob->m_CurrentLife -= m_CharacterInfo.maxattacklevel;
					}
					else
					{
						if( tmp < 10 )
						{
							m_AttackResult.usAtkInfo[m_CurrentAttackSequence-1] = 2;										
						}			
						else
						{
							m_AttackResult.usAtkInfo[m_CurrentAttackSequence-1] = 1;
							m_AttackResult.sDamage[m_CurrentAttackSequence-1] = m_CharacterInfo.maxattacklevel;
							pMob->m_CurrentLife -= m_CharacterInfo.maxattacklevel;
						}									
					}
						
					m_AttackResult.display[m_CurrentAttackSequence-1] = TRUE;							
					if ( pMob->m_CurrentLife <= 0 )
						pMob->m_HealthPercentage = -1;
					else
						pMob->m_HealthPercentage = char(pMob->m_CurrentLife*100/pMob->m_MaxLife);			

					// 타격 이펙트 삽입
					D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
					offsetmat._42 += ( 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );

					switch(m_AttackResult.usAtkInfo[m_CurrentAttackPoint-1])
					{
					case 0 :
						{
							// HIT
#ifdef _XDWDEBUG
							m_ChatBalloonObject.m_MessageColor = _XSC_WARNING;
							m_ChatBalloonObject.SetMessage("======== HIT ==========");
#endif									
							//pMob->SetBlowMotion(this);
							pMob->StartImpactEffect();									
							PlayAttackSound();
								
							if(pMob->m_ModelDescriptor.m_EffectManager)
								pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat );
							}
						break;
					case 1 :
						{
							// Critical
#ifdef _XDWDEBUG
							m_ChatBalloonObject.m_MessageColor = _XSC_WARNING;
							m_ChatBalloonObject.SetMessage("======== CRITICAL ==========");
#endif
							pMob->SetBlowMotion();
							pMob->ResetImpactEffect();
							PlayAttackSound();
								
							if(pMob->m_ModelDescriptor.m_EffectManager)
								pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat );
						}
						break;
					case 2 :
						{
							// Miss
#ifdef _XDWDEBUG
							m_ChatBalloonObject.m_MessageColor = _XSC_WARNING;
							m_ChatBalloonObject.SetMessage("======== MISS ==========");
#endif
							pMob->ResetImpactEffect();
							int rand = _XRandom() % 3;
							_XPlaySoundEffect(g_DefenceEffectSoundIndex[m_CharacterInfo.gender][rand], m_Position, true);
						}
						break;
					default :
						break;
					}
					
					if(pMob->m_HealthPercentage < 0 )
					{
						pMob->SetCharacterMobAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
						
						if(pMob == (_XMob*)g_LastPickedObject)
							g_LastPickedObject = NULL;
						
						if(pMob == (_XMob*)m_AttackTarget)
							ResetAttackTarget();
						if(pMob == (_XMob*)m_ViewTarget)
							ResetViewTarget();								
					}					
				}
			}			
		}
		break;

		_XWindow_JinStatus* pJinStatus_Window = (_XWindow_JinStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_JINSTATUSWINDOW);
		if( pJinStatus_Window )
		{
			pJinStatus_Window->SetParameter();	
		}		
	}
}

void _XHeroLocalUser::ProcessMobDie(void)
{
	_XMob* pAttackTarget = NULL;
	if(m_AttackTarget && m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
	{
		pAttackTarget = reinterpret_cast<_XMob*>(m_AttackTarget);		
	}	

	if( !pAttackTarget ) return;
	
	_XMob* pMob = NULL;
	if( g_Moblist.m_smMobList.empty() ) return;				
	smdef_XMob::iterator it;
	for (it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ it)
	{
		pMob = (*it).second;
		FLOAT dx = (pMob->m_Position.x - pAttackTarget->m_Position.x);
		FLOAT dz = (pMob->m_Position.z - pAttackTarget->m_Position.z);
		float length = _XFC_sqrt(dx*dx + dz*dz);
		
		if( length < 1.0f )
		{
			for(int i = 0 ; i < 3 ; i++)
			{
				if(m_AttackResult.display[i])
				{
					if(pMob->m_HealthPercentage < 0)
					{
						if(pMob)
						{
							pMob->SetCharacterMobAnimation(_XMOBACTION_DIE);					
							pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
							
							if(m_AttackResult.sDamage[i] == -1)
							{
//								m_CharacterLog.isKill = TRUE;
//								m_CharacterLog.killTime = g_ServerTimeCode;
//								m_CharacterLog.killTargetType = pMob->m_MobType;

								
								_XDWINPRINT("[LOCAL USER/ProcessMobDie] Mob[%d] kill", pMob->m_MobType);
							}
							m_AttackResult.display[i] = FALSE;
							break;
						}
					}
					m_AttackResult.display[i] = FALSE;
				}
			}
		}
	}
}

void _XHeroLocalUser::Render( void )
{
	if(m_StopAnimation)
	{
		m_ModelDescriptor.SetLastFrame();
	}
	else
	{
		m_ModelDescriptor.Animation();
	}

	_XParticleActionBase::bLocalUser = TRUE;
	if( g_LodTerrain.m_3PCamera.m_CurrentTargetDistance >= 1.2f )
	{
		if( m_CharacterInfo.animationdescript.motionclass != _XACTION_DIE && !m_pGoundCollideOBB )
			m_ModelDescriptor.RenderShadow();

		// Camera collide alpha blending
		if( g_LodTerrain.m_3PCamera.m_CurrentTargetDistance < 1.5f )
		{
			g_RenderAlphaBlendFlag = TRUE;
			g_RenderAlphaBlendUseAlphaChannelFlag = TRUE;
			g_LastUsedRenderStyle = NULL;
			
			FLOAT distfactor = 255.0f * ( (g_LodTerrain.m_3PCamera.m_CurrentTargetDistance - 1.2f) / 0.3f );
			
			g_RenderAlphaBlendTextureFactor = ( ( ((BYTE)distfactor) <<24) | 0xFFFFFF );
		}

		if( m_CharacterInfo.animationdescript.motionclass == _XACTION_BLOW )
		{	
			if( m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
			{	
				g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
				m_ModelDescriptor.RenderHeroCharacter(this);
				g_pCurrentSelectMaterial = NULL;
			}
			else
			{			
				g_pCurrentSelectMaterial = NULL;
				m_ModelDescriptor.RenderHeroCharacter(this);	
			}	
		}
		else if( m_CharacterInfo.animationdescript.motionclass == _XACTION_DEFENCE )
		{	
			if( m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
			{
				g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
				g_pCurrentSelectMaterial->Emissive.g = 0.8f;
				g_pCurrentSelectMaterial->Emissive.b = 0.8f;
				m_ModelDescriptor.RenderHeroCharacter(this);
				g_pCurrentSelectMaterial->Emissive.g = 0.0f;
				g_pCurrentSelectMaterial->Emissive.b = 0.0f;
				g_pCurrentSelectMaterial = NULL;
			}
			else
			{
				g_pCurrentSelectMaterial = NULL;
				m_ModelDescriptor.RenderHeroCharacter(this);
			}
		}
		else
		{			
			g_pCurrentSelectMaterial = NULL;
			m_ModelDescriptor.RenderHeroCharacter(this);	
		}
	}

	_XParticleActionBase::bLocalUser = FALSE;
	
	if( g_RenderAlphaBlendFlag )
	{
		g_RenderAlphaBlendFlag = FALSE;
		g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
		g_LastUsedRenderStyle = NULL;
	}

	// 전투 관련 Rendering
	DrawBattle();

	// 무장 해제 아이템 Rendering
	if(m_DisarmamentItem)
		m_DisarmamentItem->Render();	

#ifdef _XDWDEBUG	
	if( g_LodTerrain.m_ViewOBBObject )
	{
		gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		m_CharacterOBB.m_OBBModel.Render();
	}
#endif		
}
