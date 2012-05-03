// XNetwork_charac_monster.cpp : 케릭터/몬스터 관련 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "Xwindow_MonsterStatus.h"
#include "xwindow_chargegauge.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_SocketPlugIn.h"
#include "XWindow_Gamble.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_PK.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_QuickSlot.h"
#include "xwindow_mainmenu.h"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"
#include "XParticleIDDefine.h"
#include "XSR_STRINGHEADER.H"
#include "SoundEffectList.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_AccountItemUse.h"
#include "XWindow_Rebirth.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_MoveVillage.h"

// Monster 관련 ==============================

void _XNetwork::MSGMonsterMove(MSG_MONSTER_MOVE* monstermove)
{
	_XMob* pMob = g_Moblist.FindMob(monstermove->usMobID);
	if(pMob)
	{
		if(monstermove->usMobType == pMob->m_MobType+4000)
		{
//			VIEWPACKETLOG
//				_XDWINPRINT("=== [MonsterMove] [MobType:%d  MobID:%d] goto position[x:%f z:%f]", pMob->m_MobType, pMob->m_MobID, monstermove->fGotoX, monstermove->fGotoZ);

			if(pMob->m_KillSignal != _XDEF_MOB_KILLSIGNAL_STOP)
				return;

			pMob->m_LastRecivedMessageTime = g_LocalSystemTime;

			if(monstermove->cMobVitalRate < 0)
			{
				// 죽음
				pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
				pMob->SetAnimation(_XMOBACTION_DIE);
				pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

/*				pMob->SetAnimation(_XMOBACTION_DIE);
				pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);*/
			}
			else
			{
				if(fabs(pMob->m_Position.x-monstermove->fX) >= EPSILON1 || fabs(pMob->m_Position.z-monstermove->fZ) >= EPSILON1)
				{
					VIEWPACKETLOG
						_XDWINPRINT("[MONSTERMOVE] Different Position [id:%d type:%d walkmode:%d] [position:%f/%f] [packet:%f/%f]", monstermove->usMobID, monstermove->usMobType, monstermove->ucMobWalkMode, pMob->m_Position.x, pMob->m_Position.z, monstermove->fX, monstermove->fZ);
				}

				pMob->m_Position.x = monstermove->fX;
				pMob->m_Position.y = 0.0f;
				pMob->m_Position.z = monstermove->fZ;

				pMob->m_EndPosition.x = monstermove->fGotoX;
				pMob->m_EndPosition.y = 0.0f;
				pMob->m_EndPosition.z = monstermove->fGotoZ;

				if((fabs(monstermove->fX - monstermove->fGotoX) <= EPSILON1) && (fabs(monstermove->fZ - monstermove->fGotoZ) <= EPSILON1))
				{
					if(pMob->m_Action.action != _XMOBACTION_ATTACK)
					{
						VIEWPACKETLOG
							_XDWINPRINT("[MonsterMove] fx와 fGotoX의 차이가 EPSILON1 : fx[%f] fZ[%f] fGotoX[%f] fGotoZ[%f]", monstermove->fX, monstermove->fZ, monstermove->fGotoX, monstermove->fGotoZ);
						pMob->SetAnimation(_XMOBACTION_IDLE);
					}
				}
				else
				{
					pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pMob->m_EndPosition);
					pMob->m_MoveVector = pMob->m_EndPosition - pMob->m_Position;
					D3DXVec3Normalize(&pMob->m_MoveVector, &pMob->m_MoveVector);
					
					if( monstermove->ucMobWalkMode == 0 )
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
					else
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RUN);	
				}
				
				pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
				pMob->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
				
				pMob->m_ModelDescriptor.PlayAnimation(TRUE);

				pMob->UpdatePosition();
				
				pMob->m_HealthPercentage = monstermove->cMobVitalRate;
				pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
				
				if(monstermove->ucSpeedRate * 5 == 100)
				{
					// 정상 속도
					pMob->m_SpeedRate = 0;
				}
				else
				{
					// 이동속도 변경
					pMob->m_SpeedRate = monstermove->ucSpeedRate * 5 - 100;
				}

				if(pMob->m_HealthPercentage == 0)
				{
					_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monstermove->cMobVitalRate);
				}
			}
		} // if(monstermove->usMobType == pMob->m_MobType)
		else
		{
			VIEWPACKETLOG
				_XDWINPRINT("=== Diff Mob Type[MonsterMove] [Client:%d  Server:%d]", pMob->m_MobType, monstermove->usMobType);

			pMob = NULL;
			g_Moblist.DeleteMob(monstermove->usMobID);

			pMob = g_Moblist.CreateMobFromMove(monstermove->usMobID, monstermove->usMobType, monstermove->ucMobWalkMode,
				monstermove->cMobVitalRate, 
				D3DXVECTOR3(monstermove->fX, 0, monstermove->fZ),
				D3DXVECTOR3(monstermove->fGotoX, 0, monstermove->fGotoZ));

			if( pMob )
			{
				pMob->m_LastRecivedMessageTime = g_LocalSystemTime;

				if(monstermove->ucSpeedRate * 5 == 100)
				{
					// 정상 속도
					pMob->m_SpeedRate = 0;
				}
				else
				{
					// 이동속도 변경
					pMob->m_SpeedRate = monstermove->ucSpeedRate * 5 - 100;
				}

				if(pMob->m_HealthPercentage == 0)
				{
					_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monstermove->cMobVitalRate);
				}
			}
		}
	}
	else
	{
		_XMob* pMob = g_Moblist.CreateMobFromMove(monstermove->usMobID, monstermove->usMobType, monstermove->ucMobWalkMode,
										  monstermove->cMobVitalRate, 
										  D3DXVECTOR3(monstermove->fX, 0, monstermove->fZ),
										  D3DXVECTOR3(monstermove->fGotoX, 0, monstermove->fGotoZ));


		if( pMob )
		{
			pMob->m_LastRecivedMessageTime = g_LocalSystemTime;

			if(monstermove->ucSpeedRate * 5 == 100)
			{
				// 정상 속도
				pMob->m_SpeedRate = 0;
			}
			else
			{
				// 이동속도 변경
				pMob->m_SpeedRate = monstermove->ucSpeedRate * 5 - 100;
			}

			if(pMob->m_HealthPercentage == 0)
			{
				_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monstermove->cMobVitalRate);
			}
		}
	}
}

void _XNetwork::MSGMonsterVital(MSG_MONSTER_VITAL* monstervital)
{
	_XMob* pMob = (_XMob*)g_Moblist.FindMob(monstervital->usMobID);
	if(pMob)
	{
		VIEWPACKETLOG
			_XDWINPRINT("=== [MonsterVital] [MobType:%d  MobID:%d]", pMob->m_MobType, pMob->m_MobID);

		if(monstervital->cMobVitalRate < 0)
		{
			pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
			pMob->SetAnimation(_XMOBACTION_DIE);
			pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

/*			pMob->SetAnimation(_XMOBACTION_DIE);
			pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);*/
		}
		else
		{
			pMob->m_HealthPercentage = monstervital->cMobVitalRate;
			pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
			pMob->m_LastRecivedMessageTime = g_LocalSystemTime;

			if(pMob->m_HealthPercentage == 0)
			{
				_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monstervital->cMobVitalRate);
			}
		}
	}
	else
	{
		_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(monstervital->usMobID);
		if(pFunctionalObject)
		{			
			if(monstervital->cMobVitalRate > 0)
			{
				pFunctionalObject->m_HealthPercentage = monstervital->cMobVitalRate;
				_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
				if(pMonsterStatus_Window)
				{
					if(pMonsterStatus_Window->GetShowStatus())
					{
						pMonsterStatus_Window->m_pMonsterStaminaGauge->SetGaugeFactor(pFunctionalObject->m_HealthPercentage);
					}
				}
			}
			else
			{
				pFunctionalObject->m_HealthPercentage = monstervital->cMobVitalRate;
				pFunctionalObject->SetDieAnimation();
				pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
				g_pLocalUser->ResetAttackTarget();
				g_pLocalUser->ResetViewTarget();
				g_pLocalUser->ResetSpellAttackTarget();					
			}
		}
	}
}

void _XNetwork::MSGMonsterTransform(MSG_MONSTER_TRANSFORM* monstertransform)
{
	_XMob* pMob = (_XMob*)g_Moblist.FindMob(monstertransform->usMobID);
	if(pMob)
	{	
		VIEWPACKETLOG
			_XDWINPRINT("=== [MonsterTransform] [MobType:%d  MobID:%d]", pMob->m_MobType, pMob->m_MobID);
		
		if( monstertransform->cChangeMode == 0 )
		{			
			pMob->m_NextTransformType = monstertransform->usChangeType;
			if(pMob->m_NextTransformType > 4000) pMob->m_NextTransformType -= 4000;
			
			pMob->m_HealthPercentage = monstertransform->cMobVitalRate;
			pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
			pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
			pMob->m_RestoreOrgMonsterType = FALSE;
			
			if( g_pLocalUser->m_CharacterLog.changeTargetUniqueID == pMob->m_MobID )
			{
				g_pLocalUser->m_CharacterLog.isChange = TRUE;
				g_pLocalUser->m_CharacterLog.changeTime = g_ServerTimeCode;
				g_pLocalUser->m_CharacterLog.changeTargetType = pMob->m_MobType;
			}
			
		}
		else
		{
			pMob->m_NextTransformType = monstertransform->usChangeType;
			if(pMob->m_NextTransformType > 4000) pMob->m_NextTransformType -= 4000;
			
			pMob->m_HealthPercentage = monstertransform->cMobVitalRate;
			pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
			pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
			pMob->m_RestoreOrgMonsterType = TRUE;
			
			if(pMob->m_ModelDescriptor.m_EffectManager)
			{
				pMob->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_MONSTERID_MOB_CON);
				pMob->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_MONSTERID_TM29);
				pMob->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_MONSTERID_TM31);
			}
		}
	}
}

void _XNetwork::MSGMonsterWarp(MSG_MONSTER_WARP* monsterwarp)
{
	if(monsterwarp->usInfo == 0 || monsterwarp->usInfo == 2 || monsterwarp->usInfo == 3)
	{
		if( g_MobAttribTable[monsterwarp->usMobType-4000].scripttype != 85 && // optional lps
			g_MobAttribTable[monsterwarp->usMobType-4000].scripttype != 90 ) // gate lps
		{
			// usInfo = 0 몬스터 생성
			// usInfo = 2 워프에 의한 나타나기
			// usInfo = 3 부활에 의한 나타나기

			_XMob* pMob = g_Moblist.FindMob(monsterwarp->usMobID);
			if(pMob)
			{
				if(pMob->m_MobType+4000 == monsterwarp->usMobType)
				{
					VIEWPACKETLOG
						_XDWINPRINT("=== [MonsterWarp] [MobType:%d  MobID:%d]", pMob->m_MobType, pMob->m_MobID);

					if(monsterwarp->cMobVitalRate < 0)
					{
						pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

/*						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);*/
					}
					else
					{
						if(fabs(pMob->m_Position.x-monsterwarp->fGotoX) >= EPSILON1 || fabs(pMob->m_Position.z-monsterwarp->fGotoZ) >= EPSILON1)
						{
							VIEWPACKETLOG
								_XDWINPRINT("[MONSTERWARP] Different Position [id:%d  type:%d  walkmode:%d][position:%f/%f] [packet:%f/%f]", monsterwarp->usMobID, monsterwarp->usMobType, monsterwarp->ucMobWalkMode, pMob->m_Position.x, pMob->m_Position.z, monsterwarp->fGotoX, monsterwarp->fGotoZ);
						}
						
						pMob->m_Position.x = pMob->m_EndPosition.x = monsterwarp->fGotoX;
						pMob->m_Position.y = pMob->m_EndPosition.y = 0.0f;
						pMob->m_Position.z = pMob->m_EndPosition.z = monsterwarp->fGotoZ;

						int rotateangle = _XRandom() % 360;
						pMob->m_RotateAngle = _X_RAD(rotateangle);
						pMob->m_MoveVector.x = pMob->m_MoveVector.y = pMob->m_MoveVector.z = 0.0f;

						pMob->m_HealthPercentage = monsterwarp->cMobVitalRate;
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
						pMob->UpdatePosition();

						if( monsterwarp->ucMobWalkMode == 0 )
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_WALK);
						else
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RUN);				

						if(monsterwarp->usInfo == 3)
						{
							if(g_MobAttribTable[pMob->m_MobType].skill1_id == _XMOB_SKILL_REBIRTH)
							{
								pMob->SetAnimationIndex(_XMOBACTIONNUMBER_SKILL1);

								// 부활
								if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
								{
									if(((_XMob*)(g_pLocalUser->m_AttackTarget))->m_MobID == pMob->m_MobID)
									{
										g_pLocalUser->ResetAttackTarget();
										g_pLocalUser->ResetSkillAnimation();
//										g_pLocalUser->SetAnimation(_XACTION_IDLE);
									}
								}
							}
							else if(g_MobAttribTable[pMob->m_MobType].skill2_id == _XMOB_SKILL_REBIRTH)
							{
								pMob->SetAnimationIndex(_XMOBACTIONNUMBER_SKILL2);

								// 부활
								if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
								{
									if(((_XMob*)(g_pLocalUser->m_AttackTarget))->m_MobID == pMob->m_MobID)
									{
										g_pLocalUser->ResetAttackTarget();
										g_pLocalUser->ResetSkillAnimation();
//										g_pLocalUser->SetAnimation(_XACTION_IDLE);
									}
								}
							}
							else
								pMob->SetAnimation(_XMOBACTION_ENTERING);
							
							// 부활일 때는 Effect, Condition 모두 삭제
							pMob->ReleaseState();
							pMob->m_ConditionList = 0;
						}
						else
							pMob->SetAnimation(_XMOBACTION_ENTERING);

						if(pMob->m_HealthPercentage == 0)
						{
							_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monsterwarp->cMobVitalRate);
						}
					}
				} // if(pMob->m_MobType == monsterwarp->usMobType)
				else
				{
					VIEWPACKETLOG
						_XDWINPRINT("=== Diff Mob Type [MonsterWarp] [Client:%d  Server:%d]", pMob->m_MobType, monsterwarp->usMobType);
					
					pMob = NULL;
					g_Moblist.DeleteMob(monsterwarp->usMobID);
					
					bool bRebirth = false;
					if(monsterwarp->usInfo == 3)
						bRebirth = true;
					
					pMob = g_Moblist.CreateMob(monsterwarp->usMobID, monsterwarp->usMobType, monsterwarp->ucMobWalkMode,
						monsterwarp->cMobVitalRate, 
						D3DXVECTOR3(monsterwarp->fGotoX, 0, monsterwarp->fGotoZ),
						D3DXVECTOR3(monsterwarp->fGotoX, 0, monsterwarp->fGotoZ), bRebirth);
					
					if( pMob )
					{
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
						
						if(pMob->m_HealthPercentage == 0)
						{
							_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monsterwarp->cMobVitalRate);
						}
					}

				}
			}
			else
			{
				bool bRebirth = false;
				if(monsterwarp->usInfo == 3)
					bRebirth = true;
				
				_XMob* pMob = g_Moblist.CreateMob(monsterwarp->usMobID, monsterwarp->usMobType, monsterwarp->ucMobWalkMode,
					monsterwarp->cMobVitalRate, 
					D3DXVECTOR3(monsterwarp->fGotoX, 0, monsterwarp->fGotoZ),
					D3DXVECTOR3(monsterwarp->fGotoX, 0, monsterwarp->fGotoZ), bRebirth);
				
				if( pMob )
				{
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					
					if(pMob->m_HealthPercentage == 0)
					{
						_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monsterwarp->cMobVitalRate);
					}
				}
			}
		}
		else
		{
			_XDWINPRINT("Monster Warp ====================================");
			_XDWINPRINT("uid:%d  type:%d  vital:%d, scripttype:%d", monsterwarp->usMobID, monsterwarp->usMobType, monsterwarp->cMobVitalRate, g_MobAttribTable[monsterwarp->usMobType-4000].scripttype);
			// 타입을 검사해서 공격가능한 오브젝트일 때 ID를 부여한다.
			g_LodTerrain.m_FunctionalObjectManager.SetAttackableFunctionalObjectID(monsterwarp->usMobID,monsterwarp->usMobType-4000,monsterwarp->cMobVitalRate,D3DXVECTOR3(monsterwarp->fGotoX, 0, monsterwarp->fGotoZ));
		}
	}
	else
	{
		// usInfo : 1 몬스터 사라짐
		g_Moblist.DeleteMob(monsterwarp->usMobID);
	}
}

void _XNetwork::MSGMonsterExist(MSG_MONSTER_EXIST* monsterexist)
{	
	if( g_MobAttribTable[monsterexist->usMobType-4000].scripttype != 85 && // optional lps
		g_MobAttribTable[monsterexist->usMobType-4000].scripttype != 90 ) // gate lps
	 {	
		 _XMob* pMob = g_Moblist.FindMob(monsterexist->usMobID);
		 if(pMob)
		 {
	//		 VIEWPACKETLOG
	//			 _XDWINPRINT("=== [MonsterExist] [MobType:%d  MobID:%d] position[x:%f z:%f]", pMob->m_MobType, pMob->m_MobID, monsterexist->fX, monsterexist->fZ);

			 if(monsterexist->usMobType == pMob->m_MobType + 4000)
			 {
				 if(pMob->m_KillSignal != _XDEF_MOB_KILLSIGNAL_STOP)
					 return;

				 pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				 
				 if(monsterexist->cMobVitalRate < 0)
				 {
					 // 죽음
					 pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
					 pMob->SetAnimation(_XMOBACTION_DIE);
					 pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

/*					 pMob->SetAnimation(_XMOBACTION_DIE);
					 pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);*/
				 }
				 else
				 {
					 pMob->m_Position.x = monsterexist->fX;
					 pMob->m_Position.y = 0.0f;
					 pMob->m_Position.z = monsterexist->fZ;
					 
					 pMob->m_EndPosition.x = pMob->m_Position.x;
					 pMob->m_EndPosition.y = 0.0f;
					 pMob->m_EndPosition.z = pMob->m_Position.z;
					 
					 if(pMob->m_Action.action != _XMOBACTION_ATTACK && pMob->m_Action.action != _XMOBACTION_MOVE)
						pMob->SetAnimation(_XMOBACTION_IDLE);
					 
					 pMob->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
					 pMob->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
					 
					 pMob->m_ModelDescriptor.PlayAnimation(TRUE);
					 
					 pMob->UpdatePosition();
					 pMob->m_HealthPercentage = monsterexist->cMobVitalRate;
					 pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;

					 // condition 처리
					 DWORD conditionOld = 0, conditionNew = 0;
					 DWORD resultOld = 0, resultNew = 0;
					 for(int i = 1 ; i < 33 ; ++i)
					 {
						 conditionOld = pMob->m_ConditionList >> (i-1);
						 resultOld = conditionOld & 0x00000001;
						 
						 conditionNew = monsterexist->dwConditionBit >> (i-1);
						 resultNew = conditionNew & 0x00000001;
						 
						 if(resultOld == 1 && resultNew == 0)
						 {
							 pMob->DeleteCondition(i-1);
						 }
						 if(resultOld == 0 && resultNew == 1)
						 {
							 pMob->InsertCondition(i-1);
						 }
					 }
					 
					 pMob->m_ConditionList = monsterexist->dwConditionBit;
					 
					 if(pMob->m_HealthPercentage == 0)
					 {
						_XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monsterexist->cMobVitalRate);
					 }
				 }
			 } // if(monsterexist->usMobType == pMob->m_MobType + 4000)
			 else
			 {
				 VIEWPACKETLOG
					 _XDWINPRINT("=== Diff Mob Type[MonsterExist] [Client:%d  Server:%d]", pMob->m_MobType, monsterexist->usMobType);
				 
				 pMob = NULL;
				 g_Moblist.DeleteMob(monsterexist->usMobID);
				 
				 pMob = g_Moblist.CreateMob(monsterexist->usMobID, monsterexist->usMobType, 0,
					 monsterexist->cMobVitalRate, 
					 D3DXVECTOR3(monsterexist->fX, 0, monsterexist->fZ),
					 D3DXVECTOR3(monsterexist->fX, 0, monsterexist->fZ));
				 
				 if(pMob)
				 {
					 pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					 pMob->m_ConditionList = monsterexist->dwConditionBit;
					 DWORD condition = 0;
					 DWORD result = 0;
					 for(int i = 1 ; i < 33 ; ++i)
					 {
						 condition = pMob->m_ConditionList >> (i-1);
						 result = condition & 0x00000001;
						 
						 if(result == 1)
						 {
							 pMob->InsertCondition(i-1);
						 }
					 }
					 
					 if(pMob->m_HealthPercentage == 0)
					 {
						 _XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monsterexist->cMobVitalRate);
					 }
				 }				
			 }
		 }
		 else
		 {
			 _XMob* pMob = g_Moblist.CreateMob(monsterexist->usMobID, monsterexist->usMobType, 0,
				 monsterexist->cMobVitalRate, 
				 D3DXVECTOR3(monsterexist->fX, 0, monsterexist->fZ),
				 D3DXVECTOR3(monsterexist->fX, 0, monsterexist->fZ));
			 
			 
			 if( pMob )
			 {
				 pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				 pMob->m_ConditionList = monsterexist->dwConditionBit;
				 pMob->m_ConditionList = monsterexist->dwConditionBit;
				 DWORD condition = 0;
				 DWORD result = 0;
				 for(int i = 1 ; i < 33 ; ++i)
				 {
					 condition = pMob->m_ConditionList >> (i-1);
					 result = condition & 0x00000001;
					 
					 if(result == 1)
					 {
						 pMob->InsertCondition(i-1);
					 }
				 }
				 
				 if(pMob->m_HealthPercentage == 0)
				 {
					 _XDWINPRINT("[MONSTER WARNING] Monster Health : %d", monsterexist->cMobVitalRate);
				 }
			 }
			
		 }
	 }
	 else
	 {
		 // 타입을 검사해서 공격가능한 오브젝트일 때 ID를 부여한다.
		 _XDWINPRINT("Monster Exist ====================================");
		 _XDWINPRINT("uid:%d  type:%d  vital:%d scripttype:%d", monsterexist->usMobID, monsterexist->usMobType, monsterexist->cMobVitalRate, g_MobAttribTable[monsterexist->usMobType-4000].scripttype);
 		 g_LodTerrain.m_FunctionalObjectManager.SetAttackableFunctionalObjectID(monsterexist->usMobID,monsterexist->usMobType-4000, monsterexist->cMobVitalRate,D3DXVECTOR3(monsterexist->fX, 0, monsterexist->fZ));
	 }
}

void _XNetwork::MSGResSpellEffect(MSG_RES_SPELLEFFECT* spelleffect)
{
	_XMob* pMob = g_Moblist.FindMob(spelleffect->usMobID);
	if(pMob)
	{
		for(int i = 0 ; i < MAX_SPELLEFFECTDATA_PER_PACKET ; ++i)
		{
			if(spelleffect->aSpellID[i] > 0)
			{
				pMob->InsertState(0, spelleffect->aSpellID[i], spelleffect->aSpellLevel[i]);
				
				VIEWPACKETLOG
					_XDWINPRINT("== MObID [%d]]   spellid [%d]", spelleffect->usMobID, spelleffect->aSpellID[i]);
			}
		}
		
		for(i = 0 ; i < MAX_SPELLEFFECTDATA_PER_PACKET ; ++i)
		{
			if(spelleffect->aEffectID[i] > 0)
			{
				pMob->InsertState(1, spelleffect->aEffectID[i]);
				
				VIEWPACKETLOG
					_XDWINPRINT("== MObID [%d]]   effectid [%d]", spelleffect->usMobID, spelleffect->aEffectID[i]);
			}
		}
	}
}

void _XNetwork::MSGSpellListChanged(MSG_SPELLLIST_CHANGED* spellchanged)
{
	_XMob* pMob = g_Moblist.FindMob(spellchanged->usMobID);
	if(pMob)
	{
		switch(spellchanged->ucMsgType)
		{
		case 0 :
			{
				// delete spell
				pMob->DeleteState(0, spellchanged->usSpellEffectID, spellchanged->ucSpellLevel);
				
				VIEWPACKETLOG
					_XDWINPRINT("== [Delete Spell] MObID [%d]]   spellid [%d]", spellchanged->usMobID, spellchanged->usSpellEffectID);
			}
			break;
		case 1 :
			{
				// add spell
				pMob->InsertState(0, spellchanged->usSpellEffectID, spellchanged->ucSpellLevel);
				
				VIEWPACKETLOG
					_XDWINPRINT("== [Add Spell] MObID [%d]]   spellid [%d]", spellchanged->usMobID, spellchanged->usSpellEffectID);
			}
			break;
		case 2 :
			{
				// delete effect
				pMob->DeleteState(1, spellchanged->usSpellEffectID);
				
				VIEWPACKETLOG
					_XDWINPRINT("== [Delete Effect] MObID [%d]]   effectid [%d]", spellchanged->usMobID, spellchanged->usSpellEffectID);
			}
			break;
		case 3 :
			{
				// add effect
				pMob->InsertState(1, spellchanged->usSpellEffectID);
				
				VIEWPACKETLOG
					_XDWINPRINT("== [Add Effect] MObID [%d]]   effectid [%d]", spellchanged->usMobID, spellchanged->usSpellEffectID);
			}
			break;
		}
		
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
		if(pMonsterStatus_Window)
		{
			if(pMonsterStatus_Window->GetShowStatus())
			{
				if(pMonsterStatus_Window->m_TargetType == _XPICKINGTARGET_MOB && pMonsterStatus_Window->m_Target == pMob)
				{
					pMonsterStatus_Window->SetStateImage();
				}
			}
		}
	}
}

void _XNetwork::MSGConditionChanged(MSG_CONDITION_CHANGED* conditionchanged)
{
	_XMob* pMob = g_Moblist.FindMob(conditionchanged->usMobID);
	
	if(pMob)
	{
		DWORD conditionOld = 0, conditionNew = 0;
		DWORD resultOld = 0, resultNew = 0;
		for(int i = 1 ; i < 33 ; ++i)
		{
			conditionOld = pMob->m_ConditionList >> (i-1);
			resultOld = conditionOld & 0x00000001;
			
			conditionNew = conditionchanged->dwConditionBit >> (i-1);
			resultNew = conditionNew & 0x00000001;
			
			if(resultOld == 1 && resultNew == 0)
			{
				pMob->DeleteCondition(i-1);
			}
			if(resultOld == 0 && resultNew == 1)
			{
				pMob->InsertCondition(i-1);
			}
		}
		
		pMob->m_ConditionList = conditionchanged->dwConditionBit;
	}
}

void _XNetwork::MSGMonsterChaseNotice(MSG_MONSTER_CHASE_NOTICE* monsterchase)
{
	// 사용하지 않음	
}


// Character 관련 ==============================

void _XNetwork::MSGSetLevel(MSG_SET_LEVEL* setlevel)
{
	int prevlevel = g_pLocalUser->m_CharacterInfo.Get_sublevel(); 
	DWORD prevjinlevel = g_pLocalUser->m_CharacterInfo.Get_jinlevel();	
	
	g_pLocalUser->m_CharacterInfo.Set_jinlevel( setlevel->uiJin );
	g_pLocalUser->UpdateLocalUserLevel();
	
	if(prevlevel != g_pLocalUser->m_CharacterInfo.Get_sublevel())
	{
		if(((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_Initialized)
		{
			DWORD sublevel = g_pLocalUser->m_CharacterInfo.Get_sublevel();
			InsertChatString( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_LEVELUP, 
																g_LevelName[g_pLocalUser->m_CharacterInfo.Get_level()],
																&sublevel ),
							  _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	// 단계가 %s %d성으로 상승되었습니다.
		}
	}
	
	if(((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_Initialized && (prevjinlevel != g_pLocalUser->m_CharacterInfo.Get_jinlevel()))
	{
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));

		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_GETEXP));
		InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
		
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
			_XDEF_PLAYERID_JINUP, 
			&g_pLocalUser->m_ModelDescriptor.m_Position );
	}
	
	if( g_pJinStatus_Window )
	{
		g_pJinStatus_Window->SetParameter();	
		
		if(g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience > 0)
		{
			if( g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience )
			{
				if(g_pLocalUser->m_CharacterInfo.Get_sublevel() < 12)
				{
					g_NetworkKernel.SendPacket(MSG_NO_LEVEL_UP_REQ);
				}
				else
				{
					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_014 )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_014);
							g_TutorialInfo->_XTutorial_014 = TRUE;
	#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
	#endif						
						}
					}				
					// ----------------------------------------------------------------------------------------------------------------------------=
					g_pJinStatus_Window->SetButtonStatus(TRUE);
				}
			}
		}
	}
}

void _XNetwork::MSGSetPower(MSG_SET_POWER* setpower)
{
	if(setpower->bMax)
	{
		// 최대값 설정
		g_pLocalUser->m_CharacterInfo.Set_max_lifepower( setpower->sLifePower );
		g_pLocalUser->m_CharacterInfo.Set_max_forcepower( setpower->sForcePower );
		g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower( setpower->sConcentrationPower );
		
		if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0)
		{
			int diff = 0;
			if(g_pLocalUser->m_CharacterInfo.Get_max_lifepower() > g_pLocalUser->m_MaxLifePower.Get_org())
				diff = g_pLocalUser->m_CharacterInfo.Get_max_lifepower() - g_pLocalUser->m_MaxLifePower.Get_org();
			else
				diff = g_pLocalUser->m_MaxLifePower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_lifepower();
			
			g_pLocalUser->m_MaxLifePower.Set_result( g_pLocalUser->m_MaxLifePower.Get_result() + diff );
			g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );
		}
		else
		{
			g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );
		}
		
		if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0)
		{
			int diff = 0;
			if(g_pLocalUser->m_CharacterInfo.Get_max_forcepower() > g_pLocalUser->m_MaxForcePower.Get_org())
				diff = g_pLocalUser->m_CharacterInfo.Get_max_forcepower() - g_pLocalUser->m_MaxForcePower.Get_org();
			else
				diff = g_pLocalUser->m_MaxForcePower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_forcepower();
			
			g_pLocalUser->m_MaxForcePower.Set_result( g_pLocalUser->m_MaxForcePower.Get_result() + diff );
			g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );
		}
		else
		{
			g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );
		}
		
		if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0)
		{
			int diff = 0;
			if(g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() > g_pLocalUser->m_MaxConcentrationPower.Get_org())
				diff = g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() - g_pLocalUser->m_MaxConcentrationPower.Get_org();
			else
				diff = g_pLocalUser->m_MaxConcentrationPower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower();
			
			g_pLocalUser->m_MaxConcentrationPower.Set_result( g_pLocalUser->m_MaxConcentrationPower.Get_result() + diff );
			g_pLocalUser->m_MaxConcentrationPower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );
		}
		else
		{
			g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );
		}
	}
	else
	{
		// 현재값 설정
		g_pLocalUser->m_CharacterInfo.Set_current_lifepower( setpower->sLifePower );
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( setpower->sForcePower );
		g_pLocalUser->m_CharacterInfo.Set_current_concentrationpower( setpower->sConcentrationPower );
	}
	
	if( g_pJinStatus_Window )
	{
		g_pJinStatus_Window->SetParameter();	
	}
}

void _XNetwork::MSGLevelUpReq(MSG_LEVEL_UP_RESULT* levelupresult)
{
	if(levelupresult->ucLevelupInfo == 0)
	{
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNLEVELUP));
		InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE); // 단계 상승에 실패하였습니다.
		_XPlayInterfaceSound( ID_SR_INTERFACE_LEVELFAIL_WAV );
	}
	else
	{
		// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
		if( g_TutorialInfo )
		{
			if( !g_TutorialInfo->_XTutorial_032 )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_032);
				g_TutorialInfo->_XTutorial_032 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}
			
			if( !g_TutorialInfo->_XTutorial_021 && levelupresult->sLeftMateryPoint > 0 )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_021);
				g_TutorialInfo->_XTutorial_021 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}
			
			if( !g_TutorialInfo->_XTutorial_044 && levelupresult->sInnerLevel == 49 )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_044);
				g_TutorialInfo->_XTutorial_044 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}
			
			if( !g_TutorialInfo->_XTutorial_047 && levelupresult->sInnerLevel == 25 )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_047);
				g_TutorialInfo->_XTutorial_047 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}
		}		
		// ----------------------------------------------------------------------------------------------------------------------------=
		
		int prevlevel = g_pLocalUser->m_CharacterInfo.Get_sublevel();
		g_pLocalUser->m_CharacterInfo.Set_innerlevel( levelupresult->sInnerLevel );
		g_pLocalUser->m_CharacterInfo.Set_leftpoint( levelupresult->sLeftPoint );
		g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( levelupresult->sLeftMateryPoint );
		g_pLocalUser->UpdateLocalUserLevel();
		
		g_pLocalUser->m_CharacterInfo.Set_jinlevel( 0 );
		
#ifdef _ACCLAIM_RUBICONADSYSTEM	// local user level이 변경되면 광고관련 기능 재계산.
		g_RubiconADRefreshTimer = 0;	
		if( (g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0) || 
			g_RubiconADFreeLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
		{
			g_ViewRubiconAD = FALSE;
		}
		else
		{
			g_ViewRubiconAD = TRUE;
		}

		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetUIforRubiconADSystem();
#endif
		
		if( g_pJinStatus_Window )
		{
			g_pJinStatus_Window->SetParameter();	
			g_pJinStatus_Window->SetButtonStatus(FALSE);
			
			// 능력치업 버튼 활성화  ----------------------------------=			
			g_pJinStatus_Window->m_AbilityUpButton->ShowWindow(TRUE);
			g_pJinStatus_Window->m_AbilityUpFrame = g_fLocalSystemTime;
			g_pJinStatus_Window->m_CurAbilityUpCount = 0;
			// --------------------------------------------------------=
		}
		
		if(((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_Initialized)
		{
			_XPlayInterfaceSound( ID_SR_INTERFACE_LEVELUP01_WAV );
			// Level up effect				
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
				_XDEF_PLAYERID_LEVELUP, 
				&g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE );

			DWORD sublevel = g_pLocalUser->m_CharacterInfo.Get_sublevel();

			InsertChatString( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_LEVELUP, 
																g_LevelName[g_pLocalUser->m_CharacterInfo.Get_level()],
																&sublevel ),
							  _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE); // 단계가 %s %d성으로 상승되었습니다.
		}
		
		// 퀘스트 시작조건에 레벨 조건이 들어간 퀘스트 시작조건을 검사 
		g_QuestScriptManager.QuestStartConditionLevelMode();
		
		// 시작 조건에 레벨 검사가 있으면 레벨 상승시에 다시 검사하기 위한 정보 ( 미만일때는 계속 체크해야 한다.)
		g_QuestScriptManager.RecheckLevelTypeQuestStartCondition();
		
		// NPC 생성 or 소멸 관련 체크 
		g_Npclist.CheckConditionOfNPCExistence();
		
		// Item Check
		for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i <= _XINVENTORY_SLOTTYPE_WEAPON_2 ; ++i)
			CheckValidItem(i);
	}
}

void _XNetwork::MSGOtherCharacLevelUp(MSG_OTHER_CHARAC_LEVEL_UP* otherlevelup)
{
	_XUser* pUser = (_XUser*)g_Userlist.FindUser(otherlevelup->usUniqueID);
	if(pUser)
	{
		pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_PLAYERID_LEVELUP, 
			&pUser->m_ModelDescriptor.m_Position);
	}
}

void _XNetwork::MSGPointUse(MSG_POINT_USE_RESULT* pointuseresult)
{
	if( g_pJinStatus_Window )
	{
		// 0 : 생명(건강), 1 : 내공(진기), 2. 의지(지혜), 5로 가면 실패.
		//2004.06.18->oneway48 modify
		switch( pointuseresult->cWhere )
		{
		case 0 : // 정기(근력)
			{
				g_pLocalUser->m_CharacterInfo.Set_strength( pointuseresult->sPoint );
				//g_pLocalUser->m_CharacterInfo.m .Set_max_concentrationpower( pointuseresult->sPower );//2004.06.19->oneway48 modify
				g_pLocalUser->m_CharacterInfo.Set_leftpoint( pointuseresult->sLeftPoint );

				if(g_pLocalUser->m_Strength.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_strength() > g_pLocalUser->m_Strength.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_strength() - g_pLocalUser->m_Strength.Get_org();
					else
						diff = g_pLocalUser->m_Strength.Get_org() - g_pLocalUser->m_CharacterInfo.Get_strength();

					g_pLocalUser->m_Strength.Set_result( g_pLocalUser->m_Strength.Get_result() + diff );
					g_pLocalUser->m_Strength.Set_org( g_pLocalUser->m_CharacterInfo.Get_strength() );
				}
				else
					g_pLocalUser->m_Strength.Set_org( g_pLocalUser->m_CharacterInfo.Get_strength() );

				g_pLocalUser->UpdateLocalUserAttackLevel();

				// Item Check
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i <= _XINVENTORY_SLOTTYPE_WEAPON_2 ; ++i)
					CheckValidItem(i);
				
			}
			break;
		case 1 : // 내공(진기)
			{
				g_pLocalUser->m_CharacterInfo.Set_zen( pointuseresult->sPoint );
				g_pLocalUser->m_CharacterInfo.Set_max_forcepower( pointuseresult->sPower );
				g_pLocalUser->m_CharacterInfo.Set_leftpoint( pointuseresult->sLeftPoint );

				if(g_pLocalUser->m_Zen.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_zen() > g_pLocalUser->m_Zen.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_zen() - g_pLocalUser->m_Zen.Get_org();
					else
						diff = g_pLocalUser->m_Zen.Get_org() - g_pLocalUser->m_CharacterInfo.Get_zen();
					
					g_pLocalUser->m_Zen.Set_result( g_pLocalUser->m_Zen.Get_result() + diff );
					g_pLocalUser->m_Zen.Set_org( g_pLocalUser->m_CharacterInfo.Get_zen() );
				}
				else
					g_pLocalUser->m_Zen.Set_org( g_pLocalUser->m_CharacterInfo.Get_zen() );

				if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_max_forcepower() > g_pLocalUser->m_MaxForcePower.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_max_forcepower() - g_pLocalUser->m_MaxForcePower.Get_org();
					else
						diff = g_pLocalUser->m_MaxForcePower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_forcepower();
					
					g_pLocalUser->m_MaxForcePower.Set_result( g_pLocalUser->m_MaxForcePower.Get_result() + diff );
					g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );
				}
				else
					g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );

					
				g_pLocalUser->UpdateLocalUserAttackLevel();

				// Item Check
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i <= _XINVENTORY_SLOTTYPE_WEAPON_2 ; ++i)
					CheckValidItem(i);
			}
			break;
		case 2 : // 의지(지혜)
			{
				g_pLocalUser->m_CharacterInfo.Set_intelligence( pointuseresult->sPoint );
				g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower( pointuseresult->sPower );
				g_pLocalUser->m_CharacterInfo.Set_leftpoint( pointuseresult->sLeftPoint );

				if(g_pLocalUser->m_Intelligence.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_intelligence() > g_pLocalUser->m_Intelligence.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_intelligence() - g_pLocalUser->m_Intelligence.Get_org();
					else
						diff = g_pLocalUser->m_Intelligence.Get_org() - g_pLocalUser->m_CharacterInfo.Get_intelligence();
					
					g_pLocalUser->m_Intelligence.Set_result( g_pLocalUser->m_Intelligence.Get_result() + diff );
					g_pLocalUser->m_Intelligence.Set_org( g_pLocalUser->m_CharacterInfo.Get_intelligence() );
				}
				else
					g_pLocalUser->m_Intelligence.Set_org( g_pLocalUser->m_CharacterInfo.Get_intelligence() );

				if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() > g_pLocalUser->m_MaxConcentrationPower.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() - g_pLocalUser->m_MaxConcentrationPower.Get_org();
					else
						diff = g_pLocalUser->m_MaxConcentrationPower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower();
					
					g_pLocalUser->m_MaxConcentrationPower.Set_result( g_pLocalUser->m_MaxConcentrationPower.Get_result() + diff );
					g_pLocalUser->m_MaxConcentrationPower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );
				}
				else
					g_pLocalUser->m_MaxConcentrationPower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );

				g_pLocalUser->UpdateLocalUserAttackLevel();

				// Item Check
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i <= _XINVENTORY_SLOTTYPE_WEAPON_2 ; ++i)
					CheckValidItem(i);
			}
			break;
		case 3 : // 생명(건강)
			{				
				g_pLocalUser->m_CharacterInfo.Set_constitution( pointuseresult->sPoint );
				g_pLocalUser->m_CharacterInfo.Set_max_lifepower( pointuseresult->sPower );
				g_pLocalUser->m_CharacterInfo.Set_leftpoint( pointuseresult->sLeftPoint );

				if(g_pLocalUser->m_Constitution.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_constitution() > g_pLocalUser->m_Constitution.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_constitution() - g_pLocalUser->m_Constitution.Get_org();
					else
						diff = g_pLocalUser->m_Constitution.Get_org() - g_pLocalUser->m_CharacterInfo.Get_constitution();
					
					g_pLocalUser->m_Constitution.Set_result( g_pLocalUser->m_Constitution.Get_result() + diff );
					g_pLocalUser->m_Constitution.Set_org( g_pLocalUser->m_CharacterInfo.Get_constitution() );
				}
				else
					g_pLocalUser->m_Constitution.Set_org( g_pLocalUser->m_CharacterInfo.Get_constitution() );

				if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_max_lifepower() > g_pLocalUser->m_MaxLifePower.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_max_lifepower() - g_pLocalUser->m_MaxLifePower.Get_org();
					else
						diff = g_pLocalUser->m_MaxLifePower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_lifepower();
					
					g_pLocalUser->m_MaxLifePower.Set_result( g_pLocalUser->m_MaxLifePower.Get_result() + diff );
					g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );
				}
				else
					g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );

				g_pLocalUser->UpdateLocalUserAttackLevel();

				// Item Check
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i <= _XINVENTORY_SLOTTYPE_WEAPON_2 ; ++i)
					CheckValidItem(i);
			}
			break;
		case 4 : // 심기(민첩)
			{
				g_pLocalUser->m_CharacterInfo.Set_dexterity( pointuseresult->sPoint );
				//g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower( pointuseresult->sPower );//2004.06.19->oneway48 modify
				g_pLocalUser->m_CharacterInfo.Set_leftpoint( pointuseresult->sLeftPoint );

				if(g_pLocalUser->m_Dex.Get_applyCount() > 0)
				{
					int diff = 0;
					if(g_pLocalUser->m_CharacterInfo.Get_dexterity() > g_pLocalUser->m_Dex.Get_org())
						diff = g_pLocalUser->m_CharacterInfo.Get_dexterity() - g_pLocalUser->m_Dex.Get_org();
					else
						diff = g_pLocalUser->m_Dex.Get_org() - g_pLocalUser->m_CharacterInfo.Get_dexterity();
					
					g_pLocalUser->m_Dex.Set_result( g_pLocalUser->m_Dex.Get_result() + diff );
					g_pLocalUser->m_Dex.Set_org( g_pLocalUser->m_CharacterInfo.Get_dexterity() );
				}
				else
					g_pLocalUser->m_Dex.Set_org( g_pLocalUser->m_CharacterInfo.Get_dexterity() );

				g_pLocalUser->UpdateLocalUserAttackLevel();

				// Item Check
				for(int i = _XINVENTORY_SLOTTYPE_AMOR ; i <= _XINVENTORY_SLOTTYPE_WEAPON_2 ; ++i)
					CheckValidItem(i);
			}
			break;
		case 5 :
			{
				// 오류
				g_pLocalUser->m_CharacterInfo.Set_leftpoint( pointuseresult->sLeftPoint );
				
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNGETPOINT));
				InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); // 기를 사용할 수 없습니다.
			}
			break;
		}

		// 능력치업버튼 닫기 ----------------------------------------------------------------------------------------------------------=		
		if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() == 0 && g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() == 0 )
		{
			g_pJinStatus_Window->m_AbilityUpButton->ShowWindow(FALSE);
		}
		// ----------------------------------------------------------------------------------------------------------------------------=
	}
}

void _XNetwork::MSGCharacCtrl(MSG_CHARAC_CTRL* characctrl)
{
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	
//	VIEWPACKETLOG
		_XDWINPRINT("[CharacCtrl] uid [%d], flag [%d]", characctrl->uid, characctrl->cFlag);

	if(characctrl->cFlag == charac_ctrl_binsa)
	{
		if(characctrl->uid == g_pLocalUser->m_Uniqueid)
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_017 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_017);
					g_TutorialInfo->_XTutorial_017 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=
			
			// 게이지 처리
			if( pChargeGauge_Window )
			{
				if( pChargeGauge_Window->GetShowStatus() )
				{
					pChargeGauge_Window->StatusBinsa();
				}
				else
				{
					if( g_pLocalUser->m_bSocketPlugIn )
					{
						_XWindow_SocketPlugIn* pSocketPlugIn_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
						if( pSocketPlugIn_Window )
						{
							pSocketPlugIn_Window->ShowWindow(FALSE);
							pSocketPlugIn_Window->ResetSocketItemList();					
							g_pInventory_Window->ResetSocketIndexList();
							g_pInventory_Window->SetDefaultPosition();
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1609), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
						}
						g_pLocalUser->m_bSocketPlugIn = FALSE;
						g_pLocalUser->m_bSocketPlugInLock = FALSE;
					}
					else if( g_pLocalUser->m_bGambleBoxOpen )
					{
						if( g_pInventory_Window )
						{
							_XWindow_Gamble* pGamble_Window = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
							if( pGamble_Window )
							{
								pGamble_Window->ShowWindow(FALSE);
								pGamble_Window->ResetSocketItemList();					
								g_pInventory_Window->ResetSocketIndexList_GambleBox();
								g_pInventory_Window->SetDefaultPosition();
								
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1610), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, FALSE );
							}
							
							g_pLocalUser->m_bGambleBoxOpen = FALSE;
							g_pLocalUser->m_bGambleBoxOpenLock = FALSE;
						}						
					}
				}
			}

			// 빈사 상태 설정
			g_pLocalUser->SetDyingMode();
			g_pLocalUser->m_DyingStartTime = g_LocalSystemTime;
			g_pLocalUser->m_LastCheckedDyingStartTime = -1;
			g_pLocalUser->m_DyingTimer = characctrl->context.usTime * 1000;		// ms
			
			// 빈사 상태 게이지 ------------------------------------------------------------------------------------------------------------=			
			if( pChargeGauge_Window )
			{
				if(!pChargeGauge_Window->GetShowStatus())
				{
					if( g_pLocalUser->m_DyingTimer > 0 )
					{
						pChargeGauge_Window->SetDelayTime( characctrl->context.usTime * 1000 );
						pChargeGauge_Window->SetGaugeMode( _XGAUGE_BINSA );
						pChargeGauge_Window->ShowWindow(TRUE);
					}					
				}
			}
			// -----------------------------------------------------------------------------------------------------------------------------=
			
			// 장원전 중일때 협박 중지
			if(g_pLocalUser->m_bCastleBattle)
			{
				if(g_pLocalUser->m_bThreatening)
				{
					if(!g_pLocalUser->m_bSendThreatenPacket)
					{
						g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_threat_cancel_req);
					}
				}
			}
			
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			// 채팅창 위치 변경
			_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
			if( defaultchatwindow )
			{
				defaultchatwindow->SetPositionDie(TRUE);
			}
#endif

						
			VIEWPACKETLOG
				_XDWPACKETLOG("[CharacCtrl/Binsa] LocalUser : Dying Timer [%d]", g_pLocalUser->m_DyingTimer);
		}
		else
		{
			// 빈사상태 설정
			_XUser* pUser =(_XUser*)g_Userlist.FindUser(characctrl->uid);
			if(pUser)
			{
				pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
				memset(&pUser->m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
				pUser->SetDyingMode();

				VIEWPACKETLOG
					_XDWPACKETLOG("[ChracCtrl/Binsa] Other user [%s]", pUser->m_CharacterInfo.charactername);
			}
		}
	}
	else if(characctrl->cFlag == charac_ctrl_dead)
	{
		if(characctrl->uid == g_pLocalUser->m_Uniqueid)
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_018 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_018);
					g_TutorialInfo->_XTutorial_018 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=

#ifdef _XTS_BLOODPOINT

			if(g_pLocalUser->m_BloodPoint > 0)
			{
				// 혈정 방출
				D3DXMATRIX offsetmat;
				D3DXMatrixIdentity(&offsetmat);
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
					_XDEF_MONSTERID_TM255, &offsetmat, NULL, TRUE);
				
				_XPlayInterfaceSound(g_BloodOutSoundIndex);

				_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
				if(pBloodPoint_Window)
				{
					pBloodPoint_Window->SetShowReqItem(FALSE);
					pBloodPoint_Window->SetDeadMessageStartTime(g_LocalSystemTime);
				}
			}

#endif		
			// 장원전 중일때 협박 중지
			if(g_pLocalUser->m_bCastleBattle)
			{
				if(g_pLocalUser->m_bThreatening)
				{
					if(!g_pLocalUser->m_bSendThreatenPacket)
					{
						g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_threat_cancel_req);
					}
				}

				// 만약 delay packet 안온 경우을 대비해서 
				_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
				if(pRebirth_Window)
				{
					pRebirth_Window->SetCastleBattleState(TRUE);
				}
			}

			// 혼수 상태 설정
			g_pLocalUser->SetDieMode();
			g_pLocalUser->m_bWaitDyingSkip = FALSE;
			
			// 죽음 로그 처리
			g_pLocalUser->m_CharacterLog.isDead = TRUE;
			g_pLocalUser->m_CharacterLog.deadTime = g_ServerTimeCode;

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			// 채팅창 위치 변경
			_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
			if( defaultchatwindow )
			{
				defaultchatwindow->SetPositionDie(TRUE);
			}
#endif
			
			VIEWPACKETLOG
				_XDWPACKETLOG("[NETWORK/MSGBattleAtkResFromMob] Dead");
			VIEWPACKETLOG
				_XDWPACKETLOG("[CharacCtrl/Dead] LocalUser ");
		}
		else
		{
			// 혼수상태 설정
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(characctrl->uid);
			if(pUser)
			{
				memset(&pUser->m_AttackResult, 0 , sizeof(_XUSER_ATTACKRESULT));
				pUser->SetDieMode();
				pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
				
				// target에서 삭제
				if(pUser == (_XUser*)g_LastPickedObject) g_LastPickedObject = NULL;
				if(pUser == (_XUser*)g_pLocalUser->m_AttackTarget) g_pLocalUser->ResetAttackTarget();
				if(pUser == (_XUser*)g_pLocalUser->m_ViewTarget) g_pLocalUser->ResetViewTarget();
				
				VIEWPACKETLOG
					_XDWPACKETLOG("[CharacCtrl/DEAD] Other user die [%s]", pUser->m_CharacterInfo.charactername);
			}
		}
	}
	else if(characctrl->cFlag == charac_ctrl_awake_binsa)
	{
		if(characctrl->uid == g_pLocalUser->m_Uniqueid)
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_019 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_019);
					g_TutorialInfo->_XTutorial_019 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=

			if( pChargeGauge_Window )
			{
				if( pChargeGauge_Window->GetShowStatus() )
					pChargeGauge_Window->ShowWindow(FALSE);
			}
			
			// 빈사상태에서 깨어나기
			g_pLocalUser->SetRebirthBinsaMode();
			g_pLocalUser->m_bWaitDyingSkip = FALSE;

			CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);
#ifdef ALL_RB_NPCSCRIPT_DEATH_070611_KUKURI
			if( pApp->m_proc_maingame.m_bNPCDialogMode || pApp->m_proc_maingame.m_bNPCDialogWideView )
			{
				pApp->m_proc_maingame.CloseNPCScriptWindow();
			}
#endif
#ifdef VN_RB_BOSSROOM_BUGFIX_070615_KUKURI
			if(pApp->m_proc_maingame.m_bFunctionalObjectWideView)
			{
				_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
				if(pEnterBossRoom_Window)
				{
					if(pEnterBossRoom_Window->GetShowStatus())
					{
						// 결 준비 중이고, 행수이면, 취소
						if( g_pLocalUser->m_InParty )
						{
							if( g_pLocalUser->m_bWaitBossBattle )
							{
								g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 10 ); //행수가 생사결을 취소 
							}				
						}
						pEnterBossRoom_Window->ShowWindow(FALSE);
						g_pInventory_Window->ShowWindow(FALSE);
						pApp->m_proc_maingame.m_LockMoveProcess = FALSE;
					}
				}
			}
#endif

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			// 채팅창 위치 변경
			_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
			if( defaultchatwindow )
			{
				defaultchatwindow->SetPositionDie(FALSE);
			}
#endif

			VIEWPACKETLOG
				_XDWPACKETLOG("[CharacCtrl/Rebirth Binsa] Local user");
		}
		else
		{
			// 빈사상태에서 깨어나기
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(characctrl->uid);
			if(pUser)
			{
				pUser->SetRebirthBinsaMode();
				pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

				VIEWPACKETLOG
					_XDWPACKETLOG("[CharacCtrl/Rebirth Binsa] Other user [%s]", pUser->m_CharacterInfo.charactername);
			}
		}
	}
	else if(characctrl->cFlag == charac_ctrl_awake_honsu)
	{
		if(characctrl->uid == g_pLocalUser->m_Uniqueid)
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_019 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_019);
					g_TutorialInfo->_XTutorial_019 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=

			if( pChargeGauge_Window )
			{
				if( pChargeGauge_Window->GetShowStatus() )
					pChargeGauge_Window->ShowWindow(FALSE);
			}

			CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);
#ifdef ALL_RB_NPCSCRIPT_DEATH_070611_KUKURI
			// NPC 창 닫기
			if( pApp->m_proc_maingame.m_bNPCDialogMode || pApp->m_proc_maingame.m_bNPCDialogWideView )
			{
				pApp->m_proc_maingame.CloseNPCScriptWindow();
			}
#endif
			
#ifdef VN_RB_BOSSROOM_BUGFIX_070615_KUKURI
		if(pApp->m_proc_maingame.m_bFunctionalObjectWideView)
		{
			_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
			if(pEnterBossRoom_Window)
			{
				if(pEnterBossRoom_Window->GetShowStatus())
				{
					// 결 준비 중이고, 행수이면, 취소
					if( g_pLocalUser->m_InParty )
					{
						if( g_pLocalUser->m_bWaitBossBattle )
						{
							g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 10 ); //행수가 생사결을 취소 
						}				
					}
					pEnterBossRoom_Window->ShowWindow(FALSE);
					g_pInventory_Window->ShowWindow(FALSE);
					pApp->m_proc_maingame.m_LockMoveProcess = FALSE;
				}
			}
		}
#endif

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			// 채팅창 위치 변경
			_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
			if( defaultchatwindow )
			{
				defaultchatwindow->SetPositionDie(FALSE);
			}
#endif

			// 혼수 상태에서 깨어나기
			VIEWPACKETLOG
				_XDWPACKETLOG("[CharacCtrl/Rebirth Honsu] Local user");

			g_pLocalUser->m_bWaitDyingSkip = FALSE;
		}
		else
		{
			// 혼수 상태에서 깨어나기
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(characctrl->uid);
			if(pUser)
			{
				pUser->SetRebirthHonsuMode();
				pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
				
				VIEWPACKETLOG
					_XDWPACKETLOG("[CharacCtrl/Rebirth Honsu] Other user [%s]", pUser->m_CharacterInfo.charactername);
			}
		}
	}
	else if(characctrl->cFlag == charac_ctrl_dead_in_match)
	{
		// 비무중 사망 - 혼수상태 설정
		if(characctrl->uid != g_pLocalUser->m_Uniqueid)
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(characctrl->uid);
			if(pUser)
			{
				memset(&pUser->m_AttackResult, 0 , sizeof(_XUSER_ATTACKRESULT));
				pUser->SetDieMode();
				pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

				// target에서 삭제
				if(pUser == (_XUser*)g_LastPickedObject) g_LastPickedObject = NULL;
				if(pUser == (_XUser*)g_pLocalUser->m_AttackTarget) g_pLocalUser->ResetAttackTarget();
				if(pUser == (_XUser*)g_pLocalUser->m_ViewTarget) g_pLocalUser->ResetViewTarget();
				
				VIEWPACKETLOG
					_XDWPACKETLOG("[CharacCtrl/Match-DEAD] Other user die [%s]", pUser->m_CharacterInfo.charactername);
			}
		}
	}
	else if(characctrl->cFlag == charac_ctrl_dead_by_pk)
	{
		// pk로 사망 : 주위에 알리는 패킷
		if(characctrl->uid != g_pLocalUser->m_Uniqueid)
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(characctrl->uid);
			if(pUser)
			{
				memset(&pUser->m_AttackResult, 0 , sizeof(_XUSER_ATTACKRESULT));
				pUser->SetDieMode();
				pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

				// target에서 삭제
				if(pUser == (_XUser*)g_LastPickedObject) g_LastPickedObject = NULL;
				if(pUser == (_XUser*)g_pLocalUser->m_AttackTarget) g_pLocalUser->ResetAttackTarget();
				if(pUser == (_XUser*)g_pLocalUser->m_ViewTarget) g_pLocalUser->ResetViewTarget();
				
				VIEWPACKETLOG
					_XDWPACKETLOG("[CharacCtrl/Dead by pk] Other user die [%s]", pUser->m_CharacterInfo.charactername);
			}
		}
		else
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[CharacCtrl/Dead by pk] Local user die - skip");
		}
	}
	else if(characctrl->cFlag == charac_ctrl_dead_in_fm)	// 자유비무중 사망
	{
		if(characctrl->uid == g_pLocalUser->m_Uniqueid)
		{
			// 혼수 상태 설정
			g_pLocalUser->SetDieMode();
			g_pLocalUser->m_bWaitDyingSkip = FALSE;
			
			// 죽음 로그 처리
			g_pLocalUser->m_CharacterLog.isDead = TRUE;
			g_pLocalUser->m_CharacterLog.deadTime = g_ServerTimeCode;

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			// 채팅창 위치 변경
			_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
			if( defaultchatwindow )
			{
				defaultchatwindow->SetPositionDie(TRUE);
			}
#endif

			// bgm 끄기
			XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
			pProc_Maingame->StopSpecialBGMPlay();
			
			VIEWPACKETLOG
				_XDWPACKETLOG("[CharacCtrl/Dead - Dead in FM] LocalUser ");
		}
		else
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(characctrl->uid);
			if(pUser)
			{
				memset(&pUser->m_AttackResult, 0 , sizeof(_XUSER_ATTACKRESULT));
				pUser->SetDieMode();
				pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

				// target에서 삭제
				if(pUser == (_XUser*)g_LastPickedObject) g_LastPickedObject = NULL;
				if(pUser == (_XUser*)g_pLocalUser->m_AttackTarget) g_pLocalUser->ResetAttackTarget();
				if(pUser == (_XUser*)g_pLocalUser->m_ViewTarget) g_pLocalUser->ResetViewTarget();
				
				VIEWPACKETLOG
					_XDWPACKETLOG("[CharacCtrl/Dead - Dead in FM] Other user die [%s]", pUser->m_CharacterInfo.charactername);
			}
		}
	}
}

void _XNetwork::MSGCharacRebirthTarget(MSG_CHARAC_REBIRTH_TARGET* characrebirthtarget)
{
	if(characrebirthtarget->uid != g_pLocalUser->m_Uniqueid)
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(characrebirthtarget->uid);
		if(pUser)
		{
			if(characrebirthtarget->usTargetID == g_pLocalUser->m_Uniqueid)
			{
				pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
				pUser->UpdatePosition();
			}
			else
			{
				_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(characrebirthtarget->usTargetID);
				if(pTargetUser)
				{
					pUser->SetAttackTarget(pTargetUser, _XPICKINGTARGET_PC);
					pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
					pUser->UpdatePosition();
					
					pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
					pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
			
			pUser->SetAnimation(_XACTION_RESCUE);
		}
	}
}

void _XNetwork::MSGSaveRespawnPosResult(MSG_SAVE_RESPAWN_POS_RESULT* respawnposresult)
{
	switch(respawnposresult->cResult)
	{
	case 0 :
		{
			// 성공
			g_pLocalUser->m_bSaveRespawnPos = TRUE;
			memset(g_pLocalUser->m_RespawnName, 0, sizeof(TCHAR)*13);
			strcpy(g_pLocalUser->m_RespawnName, respawnposresult->cName);
			
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2343), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
			}
		}
		break;
	case 1 :
		{
			// NPC 없음
		}
		break;
	case 2 :
		{
			// 역할 없음
		}
		break;
	case 3 :
		{
			// 여점 삭제
			g_pLocalUser->m_bSaveRespawnPos = FALSE;
			memset(g_pLocalUser->m_RespawnName, 0, sizeof(g_pLocalUser->m_RespawnName));
		}
		break;
	}
}

void _XNetwork::MSGCharacSetState(MSG_CHARAC_SET_STATE* characsetstate)
{
	switch(characsetstate->cFlag)
	{
	case charac_state_en_wound_rate :
		{
			if( characsetstate->state.sWoundRate == 0 )
			{
				if( g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() == 0 )
				{
					// 외상 clear
					if(g_pState_Window)
					{
						g_pState_Window->DeleteStateList(_XST_EXTERNAL, 0);
					}
				}
				else
				{
					// 외상 치료 - 외상 모두 치료 됨
					if(g_pState_Window)
					{
						g_pState_Window->UpdatePlayerStateList(_XST_EXTERNAL, g_pLocalUser->m_CharacterInfo.Get_externalwoundrate(), characsetstate->state.sWoundRate);
						g_pLocalUser->m_CharacterInfo.Set_externalwoundrate( characsetstate->state.sWoundRate );
					}
				}
			}
			else if( g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() > characsetstate->state.sWoundRate) 
			{
				// 외상 치료 - 외상 남음
				if(g_pState_Window)
				{
					g_pState_Window->UpdatePlayerStateList(_XST_EXTERNAL, g_pLocalUser->m_CharacterInfo.Get_externalwoundrate(), characsetstate->state.sWoundRate);
					g_pLocalUser->m_CharacterInfo.Set_externalwoundrate( characsetstate->state.sWoundRate );
				}
			}
			else
			{
				// 외상 입음
				if(g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() <= 0)
				{
					if(g_pState_Window)
					{
						int maxlife = g_pLocalUser->m_MaxLifePower.Get_result();
						if(maxlife == 0)
							maxlife = g_pLocalUser->m_CharacterInfo.Get_max_lifepower();

						FLOAT percentage = (((FLOAT)characsetstate->state.sWoundRate)/(FLOAT)maxlife)*100.0f;
						if(percentage >= 40.0f)
							g_pState_Window->InsertStateListFromExIn(_XST_EXTERNAL, g_pLocalUser->m_CharacterInfo.Get_externalwoundrate(), characsetstate->state.sWoundRate);
					}
				}
				else
				{
					if(g_pState_Window)
						g_pState_Window->UpdatePlayerStateList(_XST_EXTERNAL, g_pLocalUser->m_CharacterInfo.Get_externalwoundrate(), characsetstate->state.sWoundRate);
				}

				g_pLocalUser->m_CharacterInfo.Set_externalwoundrate( characsetstate->state.sWoundRate );
			}
		}
		break;
	case charac_state_en_inside_rate :
		{
//			g_pLocalUser->m_CharacterInfo.internalwoundrate = characsetstate->state.sInsideWoundRate;


			if( characsetstate->state.sInsideWoundRate == 0 )
			{
				if( g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() == 0 )
				{
					// 내상 clear
					if(g_pState_Window)
					{
						g_pState_Window->DeleteStateList(_XST_INTERNAL, 0);
					}
				}
				else
				{
					// 내상 치료 - 내상 모두 치료 됨
					if(g_pState_Window)
					{
						g_pState_Window->UpdatePlayerStateList(_XST_INTERNAL, g_pLocalUser->m_CharacterInfo.Get_internalwoundrate(), characsetstate->state.sInsideWoundRate);
						g_pLocalUser->m_CharacterInfo.Set_internalwoundrate( characsetstate->state.sInsideWoundRate );
					}
				}
			}
			else if( g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() > characsetstate->state.sInsideWoundRate) 
			{
				// 내상 치료 - 내상 남음
				if(g_pState_Window)
				{
					g_pState_Window->UpdatePlayerStateList(_XST_INTERNAL, g_pLocalUser->m_CharacterInfo.Get_internalwoundrate(), characsetstate->state.sInsideWoundRate);
					g_pLocalUser->m_CharacterInfo.Set_internalwoundrate( characsetstate->state.sInsideWoundRate );
				}
			}
			else
			{
				// 내상 입음
				if(g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() <= 0)
				{
					if(g_pState_Window)
					{
						int maxforce = g_pLocalUser->m_MaxForcePower.Get_result();
						if(maxforce == 0)
							maxforce = g_pLocalUser->m_CharacterInfo.Get_max_forcepower();

						FLOAT percentage = (((FLOAT)characsetstate->state.sInsideWoundRate)/(FLOAT)maxforce)*100.0f;
						if(percentage >= 40.0f)
							g_pState_Window->InsertStateListFromExIn(_XST_INTERNAL, g_pLocalUser->m_CharacterInfo.Get_internalwoundrate(), characsetstate->state.sInsideWoundRate);
					}
				}
				else
				{
					if(g_pState_Window)
						g_pState_Window->UpdatePlayerStateList(_XST_INTERNAL, g_pLocalUser->m_CharacterInfo.Get_internalwoundrate(), characsetstate->state.sInsideWoundRate);
				}
				
				g_pLocalUser->m_CharacterInfo.Set_internalwoundrate( characsetstate->state.sInsideWoundRate );
			}
		}
		break;
	case charac_state_en_fatigue :
		{
			g_pLocalUser->m_CharacterInfo.Set_fatiguerate( characsetstate->state.usFatigueRate );
		}
		break;
	case charac_state_en_retribution :
		{
			g_pLocalUser->m_CharacterInfo.Set_retribution( characsetstate->state.sRetribution );
		}
		break;
	case charac_state_en_honor :
		{
			g_pLocalUser->m_CharacterInfo.Set_honor( characsetstate->state.sHonor );
		}
		break;
#ifdef _XTS_FAME
	case charac_state_en_fame :
		{
			g_pLocalUser->m_CharacterInfo.Set_Notoriety( characsetstate->state.sHonor );
		}
		break;
#endif
	case charac_state_en_showdown :
		{
			g_pLocalUser->m_CharacterInfo.Set_showdown( characsetstate->state.sShowdowm );
		}
		break;
	case charac_state_en_fury :
		{
			g_pLocalUser->m_CharacterInfo.Set_furyparameter( characsetstate->state.sFuryParameter );
		}
		break;
	case charac_state_en_life_power :
		{
			g_pLocalUser->m_CharacterInfo.Set_current_lifepower( characsetstate->state.power );
		}
		break;
	case charac_state_en_force_power :
		{
			g_pLocalUser->m_CharacterInfo.Set_current_forcepower( characsetstate->state.power );
		}
		break;
	case charac_state_en_concentration_power :
		{
			g_pLocalUser->m_CharacterInfo.Set_current_concentrationpower( characsetstate->state.power );
		}
		break;
	case charac_state_en_max_life :
		{
			g_pLocalUser->m_CharacterInfo.Set_max_lifepower( characsetstate->state.power );
			g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );

			if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0)
			{
				int diff = 0;
				if(g_pLocalUser->m_CharacterInfo.Get_max_lifepower() > g_pLocalUser->m_MaxLifePower.Get_org())
					diff = g_pLocalUser->m_CharacterInfo.Get_max_lifepower() - g_pLocalUser->m_MaxLifePower.Get_org();
				else
					diff = g_pLocalUser->m_MaxLifePower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_lifepower();
				
				g_pLocalUser->m_MaxLifePower.Set_result( g_pLocalUser->m_MaxLifePower.Get_result() + diff );
				g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );
			}
			else
			{
				g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );
			}
		}
		break;
	case charac_state_en_max_force :
		{
			g_pLocalUser->m_CharacterInfo.Set_max_forcepower( characsetstate->state.power );
			g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );

			if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0)
			{
				int diff = 0;
				if(g_pLocalUser->m_CharacterInfo.Get_max_forcepower() > g_pLocalUser->m_MaxForcePower.Get_org())
					diff = g_pLocalUser->m_CharacterInfo.Get_max_forcepower() - g_pLocalUser->m_MaxForcePower.Get_org();
				else
					diff = g_pLocalUser->m_MaxForcePower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_forcepower();
				
				g_pLocalUser->m_MaxForcePower.Set_result( g_pLocalUser->m_MaxForcePower.Get_result() + diff );
				g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );
			}
			else
			{
				g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );
			}
		}
		break;
	case charac_state_en_max_concentration :
		{
			g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower( characsetstate->state.power );
			g_pLocalUser->m_MaxConcentrationPower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );

			if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0)
			{
				int diff = 0;
				if(g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() > g_pLocalUser->m_MaxConcentrationPower.Get_org())
					diff = g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() - g_pLocalUser->m_MaxConcentrationPower.Get_org();
				else
					diff = g_pLocalUser->m_MaxConcentrationPower.Get_org() - g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower();
				
				g_pLocalUser->m_MaxConcentrationPower.Set_result( g_pLocalUser->m_MaxConcentrationPower.Get_result() + diff );
				g_pLocalUser->m_MaxConcentrationPower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );
			}
			else
			{
				g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );
			}
		}
		break;
	case charac_state_en_constitution :
		{
			g_pLocalUser->m_CharacterInfo.Set_constitution( characsetstate->state.stat );
			g_pLocalUser->m_Constitution.Set_org( g_pLocalUser->m_CharacterInfo.Get_constitution() );
		}
		break;
	case charac_state_en_zen :
		{
			g_pLocalUser->m_CharacterInfo.Set_zen( characsetstate->state.stat );
			g_pLocalUser->m_Zen.Set_org( g_pLocalUser->m_CharacterInfo.Get_zen() );
		}
		break;
	case charac_state_en_intelligence :
		{
			g_pLocalUser->m_CharacterInfo.Set_intelligence( characsetstate->state.stat );
			g_pLocalUser->m_Intelligence.Set_org( g_pLocalUser->m_CharacterInfo.Get_intelligence() );
		}
		break;
	case charac_state_en_dexterity :
		{
			g_pLocalUser->m_CharacterInfo.Set_dexterity( characsetstate->state.stat );
			g_pLocalUser->m_Dex.Set_org( g_pLocalUser->m_CharacterInfo.Get_dexterity() );
		}
		break;
	case charac_state_en_strength :
		{
			g_pLocalUser->m_CharacterInfo.Set_strength( characsetstate->state.stat );
			g_pLocalUser->m_Strength.Set_org( g_pLocalUser->m_CharacterInfo.Get_strength() );
		}
		break;
	case charac_state_en_match_grade :
		{
			g_pLocalUser->m_MatchGrade = characsetstate->state.iMatchRecord;
		}
		break;
	case charac_state_en_match_win :
		{
			g_pLocalUser->m_CharacterInfo.winscore = characsetstate->state.iMatchRecord;
		}
		break;
	case charac_state_en_match_lose :
		{
			g_pLocalUser->m_CharacterInfo.losescore = characsetstate->state.iMatchRecord;
		}
		break;
	case charac_state_en_match_tie :
		{
			g_pLocalUser->m_CharacterInfo.tiescore = characsetstate->state.iMatchRecord;
		}
		break;
	case charac_state_en_match_max_win_sr :
		{
			g_pLocalUser->m_CharacterInfo.max_series_wincount = characsetstate->state.iMatchRecord;
		}
		break;
	case charac_state_en_match_cur_win_sr :
		{
			g_pLocalUser->m_CharacterInfo.seriesscore = characsetstate->state.iMatchRecord;

			if(characsetstate->state.iMatchRecord > 0)
				g_pLocalUser->m_CharacterInfo.prevmatchresult = 0;
		}
		break;
	}
	
	//if(g_pCInfo_Window)
	//  g_pCInfo_Window->SetParameter();
	// 아래서 호출됨 

	if(g_pJinStatus_Window)
	{
		g_pJinStatus_Window->SetParameter();	
	}
}

void _XNetwork::MSGStateChangeResult(MSG_STATE_CHANGE_RESULT* statechangeresult)
{
	
	switch(statechangeresult->ucMode)
	{
	case state_change_en_normal_fury :
		{
			if(statechangeresult->ucResult == 0)
			{
				g_pLocalUser->m_CharacterInfo.Set_furyparameter( 0 );

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
				if(g_pExp_Window)
				{
					g_pExp_Window->ClearConvergence();
				}
#else
				if(g_pJinStatus_Window)
					g_pJinStatus_Window->ClearConvergence();
#endif
				
				g_pLocalUser->UpdateLocalUserAttackLevel();
			}
		}
		break;
	case state_change_en_start_fury :
		{
			if(statechangeresult->ucResult == 0)
			{
				g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_FURY;
				
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
				if(g_pExp_Window)
				{
					g_pExp_Window->ShowConvergenceAnimation(TRUE);
					g_pExp_Window->SetConvergenceStartTime(g_LocalSystemTime);
				}
#else
				if(g_pJinStatus_Window)
				{
					g_pJinStatus_Window->m_ConvergenceButton->ShowWindow(TRUE);
					g_pJinStatus_Window->SetConvergenceStartTime(g_LocalSystemTime);
				}
#endif
				
				g_pLocalUser->UpdateLocalUserAttackLevel();
				
				_XPlayInterfaceSound(ID_SR_INTERFACE_ANGERFIRE01_WAV);
			}
		}
		break;
	case state_change_en_start_convergence :
		{
			if(statechangeresult->ucResult == 0)
			{
				g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_CONVERGENCE;

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
				if(g_pExp_Window)
				{
					g_pExp_Window->ShowConvergenceAnimation(FALSE);
				}
#else
				if(g_pJinStatus_Window)
				{
					g_pJinStatus_Window->m_ConvergenceButton->ShowWindow(FALSE);
				}
#endif

				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
					_XDEF_PLAYERID_BERSERK, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE );
				
				g_pLocalUser->UpdateLocalUserAttackLevel();
				
				_XPlayInterfaceSound(ID_SR_INTERFACE_BURNANGER01_WAV);
			}
		}
		break;
	case state_change_en_peace_mode :
		{
			g_pLocalUser->ResetPKMode();
			g_pLocalUser->SetBattleMode(_XBATTLE_PEACE);
		}
		break;
	case state_change_en_battle_mode :
		{
			g_pLocalUser->ResetPKMode();
			g_pLocalUser->SetBattleMode(_XBATTLE_BATTLE);
		}
		break;
	case state_change_en_off_pe_mode :
		{
			g_pLocalUser->m_bPEMode = FALSE;
			g_pLocalUser->m_PEModeLeftTime = 0;
			g_pLocalUser->m_PEModeStartTime = 0;
			
			if(g_pState_Window)
			{
				g_pState_Window->DeletePEModeIcon();
			}
			
			if(g_pLocalUser->m_ModelDescriptor.m_EffectManager)
			{
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_PK_PICKING);
			}
		}
		break;
	}
}

void _XNetwork::MSGStateChangeToPK(MSG_STATE_CHANGE_TO_PK* statechangeresult)
{
	if(g_pLocalUser->m_BattleMode != _XBATTLE_PK)
	{
		g_pLocalUser->m_PrevBattleMode = g_pLocalUser->m_BattleMode;	// 저장
		g_pLocalUser->SetBattleMode(_XBATTLE_PK);
		
		_XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
		if(pPK_Window)
		{
			pPK_Window->ShowWindow(TRUE);
		}

		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
		if(pMonsterStatus_Window)
		{
			pMonsterStatus_Window->SetPKMode();
		}

		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2743), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);	// "대전 상태가 활성화 되었습니다."
		
		// 개인 상점에 관련된 행동 중이면 취소를 한다.
		if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY || g_pLocalUser->m_PersonalStoreState ==_XUSER_PERSONALSTORE_OWNER_OPEN ) 
		{
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_stop_req,pPersonalStore_Window->m_ServerPersonalStoreID , 0);
				g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
			}
		}
		else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE )
		{
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				// 들고 있던 무기 복구
				g_pLocalUser->ReloadWeaponModel();
				
				g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
				g_pLocalUser->SetAnimation(_XACTION_IDLE );
				g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
								
				for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
				{
					if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0)
					{
						int inventoryindex = pPersonalStore_Window->m_TradeItem[i].m_InventoryNumber;
						if( pPersonalStore_Window->m_TradeItem[i].IsStackable() )
						{
							if( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() == 0)
							{
								g_pLocalUser->m_UserItemList[inventoryindex].Copy( pPersonalStore_Window->m_TradeItem[i] );
							}
							else
							{
								g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + pPersonalStore_Window->m_TradeItem[i].Get_m_ucCount());
							}
						}
						else
						{
							g_pLocalUser->m_UserItemList[inventoryindex].Copy( pPersonalStore_Window->m_TradeItem[i] );
						}
					}
				}
				
				pPersonalStore_Window->ResetTradeInfo();
				pPersonalStore_Window->ShowWindow(FALSE);
				if(g_pInventory_Window->GetShowStatus()) 
					g_pInventory_Window->ShowWindow(FALSE);
				
				// 노점이 끝나면 Inventory lock 해제
				if(g_pInventory_Window)
				{
					if(g_pInventory_Window->m_bLockInventory)
						g_pInventory_Window->m_bLockInventory = FALSE;
				}
				
				
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CLOSESTORE), TRUE);
			}
		}

		// 개인 거래 중이면 취소한다.
		if( g_pLocalUser->m_PvpNPCLock )
		{
			_XWindow_PCTrade* pPCTradeWindow = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PCTRADE);
			if(pPCTradeWindow)
			{
				if(pPCTradeWindow->GetShowStatus())
				{
					g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_CANCEL);
					pPCTradeWindow->ShowWindow(FALSE);
				}
			}
		}

		//NPC와 대화 중이면 대화화면에서 나온다.		
		CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);
		if( pApp->m_proc_maingame.m_bNPCDialogMode || pApp->m_proc_maingame.m_bNPCDialogWideView )
		{
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 1 );
		}
		
		// game 종료 중이면 종료를 취소한다.
		XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
		if(pProc_Maingame->m_GameExitFlag)
		{
			pProc_Maingame->m_GameExitFlag = FALSE;
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2744), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);	// "게임 종료가 취소됩니다."
		}
		
		// 표국이동창 닫기
		_XWindow_MoveVillage* pMoveVillage_Window = (_XWindow_MoveVillage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MOVEVILLAGE);
		if(pMoveVillage_Window && pMoveVillage_Window->GetShowStatus())
		{
			pMoveVillage_Window->ShowWindow(FALSE);
		}
	}

	if(g_pLocalUser->m_UserState != _XDEF_USERSTATE_PK_FIGHT)
	{
		g_pLocalUser->m_UserState = _XDEF_USERSTATE_PK_FIGHT;
	}

	// lefttime 처리
	g_pLocalUser->m_PKModeLeftTime = statechangeresult->ucLeftTime * 1000;
	g_pLocalUser->m_PKModeStartTime = g_LocalSystemTime;
}

void _XNetwork::MSGStateChangeNotice(MSG_STATE_CHANGE_NOTICE* statechangenotice)
{
	_XUser* pUser = (_XUser*)g_Userlist.FindUser(statechangenotice->usUniqueID);
	if(pUser)
	{
		switch(statechangenotice->ucMode)
		{
		case state_change_en_notice_to_peace_mode :
			{
				pUser->SetBattleMode(_XBATTLE_PEACE);
				pUser->m_bEnemy = FALSE;
			}
			break;
		case state_change_en_notice_to_battle_mode :
			{
				pUser->SetBattleMode(_XBATTLE_BATTLE);
				pUser->m_bEnemy = FALSE;
			}
			break;
		case state_change_en_notice_to_pk_mode :
			{
				pUser->SetBattleMode(_XBATTLE_PK);
			}
			break;
		case state_change_en_notice_on_pe_mode :
			{
				pUser->m_bPEMode = TRUE;
				
				if(pUser->m_ModelDescriptor.m_EffectManager)
				{
					if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(_XDEF_INTERFACEID_PK_PICKING))
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_PK_PICKING, &offsetmat);
					}
				}
			}
			break;
		case state_change_en_notice_off_pe_mode :
			{
				pUser->m_bPEMode = FALSE;
				
				if(pUser->m_ModelDescriptor.m_EffectManager)
				{
					pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_PK_PICKING);
				}
			}
			break;
		}
	}
}

void _XNetwork::MSGStateChangeToPE(MSG_STATE_CHANGE_TO_PE* statechangetope)
{
	if(!g_pLocalUser->m_bPEMode)
	{
		g_pLocalUser->m_bPEMode = TRUE;
	}
	
	// icon 띄우기
	if(g_pState_Window)
	{
		g_pState_Window->InsertStateListFromOther(_XST_PEMODE, 0);	// id는 필요없으므로 0
	}
	
	if(g_pLocalUser->m_ModelDescriptor.m_EffectManager)
	{
		D3DXMATRIX offsetmat;
		D3DXMatrixIdentity(&offsetmat);
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_PK_PICKING, &offsetmat);
	}
	
	g_pLocalUser->m_PEModeLeftTime = statechangetope->usLeftTime*1000;
	g_pLocalUser->m_PEModeStartTime = g_LocalSystemTime;
}

void _XNetwork::MSGStateChangeAbsent(MSG_STATE_CHANGE_ABSENT* statechangeabsent)
{
	if(statechangeabsent->usUniqueID == g_pLocalUser->m_Uniqueid)
	{
		// normal 상태로 변하면 키 입력 시간 갱신
		if(statechangeabsent->ucMode == state_change_en_to_normal_state)
		{
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_dwLastInputTime = g_LocalSystemTime; 
		}
		return;
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(statechangeabsent->usUniqueID);
		if(pUser)
		{
			if(statechangeabsent->ucMode == state_change_en_to_normal_state)
			{
				// 자리비움 -> 정상
				pUser->m_bOut = FALSE;
			}
			else if(statechangeabsent->ucMode == state_change_en_to_absent_state)
			{
				// 정상 -> 자리비움
				pUser->m_bOut = TRUE;
			}
		}
	}
}

void _XNetwork::MSGStanceMotionChange(MSG_STANCE_MOTION_CHANGE* stancemotionchange)
{
	if(g_pLocalUser->m_Uniqueid != stancemotionchange->usID)
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(stancemotionchange->usID);
		if(pUser)
		{
			pUser->SetStanceMotion(stancemotionchange->sStanceMotion);
		}
	}
}

void _XNetwork::MSGStateChangeSelectClothes(MSG_STATE_CHANGE_CLOTHVIEW_SELECT_DECK* statechangeclothes)
{
	if(statechangeclothes->ucMode == state_change_en_clothview_select_deck)
	{
		// 치장 의상 선택
		g_bSelectClothesMask = TRUE;
		g_pLocalUser->m_CharacterInfo.m_nClothView = 0;
		g_pLocalUser->SetCharacterModel();
	}
	else if(statechangeclothes->ucMode == state_change_en_clothview_select_normal)
	{
		// 일반 의상 선택
		g_bSelectClothesMask = FALSE;
		g_pLocalUser->m_CharacterInfo.m_nClothView = 1;
		g_pLocalUser->SetCharacterModel();
	}
}


// Skill 관련 ======================================================

void _XNetwork::MSGSkillInfo(MSG_SKILL_INFO* skillinfolist)
{
	g_pLocalUser->ResetSkillList();
	for(int i = 0 ; i < skillinfolist->ucLearnSkillCount ; ++i)
	{
		if(skillinfolist->skill_info[i].id > 0)
		{
			if(_XSkillItem::FindSkillProperty(skillinfolist->skill_info[i].id))
			{
				// 스킬 추가
				g_pLocalUser->m_SkillList[i].m_sSkillID		= skillinfolist->skill_info[i].id;
				g_pLocalUser->m_SkillList[i].m_cSkillLevel	= skillinfolist->skill_info[i].cStep;
				g_pLocalUser->m_SkillList[i].m_cCurMission	= skillinfolist->skill_info[i].ucCurMission;
				g_pLocalUser->m_SkillList[i].m_iSkillExp	= skillinfolist->skill_info[i].iCurSkillExp;
				g_pLocalUser->m_SkillList[i].m_ucSkillState	= skillinfolist->skill_info[i].ucSkillState;
				g_pLocalUser->m_SkillList[i].m_cSelectedSkillLevel = skillinfolist->skill_info[i].cStep;
				g_pLocalUser->m_SkillList[i].m_cOnOff		= 0;
				g_pLocalUser->m_SkillList[i].m_dwCooldownStartTime = 0;
			}
			else
			{
				_XFatalError("Network Error - Skill Information");
			}
		}
	}
	
	if(g_pSkill_Window)
	{
		g_pSkill_Window->SetSkillButton();
	}
	
	_XQuickSlotInfoList	quickslotinfolist;
	quickslotinfolist.LoadQuickSlotInfo();
	quickslotinfolist.UpdateQuickSlotInfo();
}

void _XNetwork::MSGSkillCtrl(MSG_SKILL_CTRL* skillctrl)
{
	_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
	if(!pNPCScript_Window)
		return;

	switch(skillctrl->cMode)
	{
	case en_skill_ctrl_mode_add :
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_002 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_002);
					g_TutorialInfo->_XTutorial_002 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
				
				if( !g_TutorialInfo->_XTutorial_006 && (g_SkillProperty[skillctrl->ctrl.S_skill_add.sAddSkillID]->skillCType == _XSI_CTYPE_SPELL) )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_006);
					g_TutorialInfo->_XTutorial_006 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=
			
			// 스킬 추가
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_sSkillID = skillctrl->ctrl.S_skill_add.sAddSkillID;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_cSkillLevel = skillctrl->ctrl.S_skill_add.cStep;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_cCurMission = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_iSkillExp = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_ucSkillState = skillctrl->ctrl.S_skill_add.ucSkillState;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_cSelectedSkillLevel = skillctrl->ctrl.S_skill_add.cStep;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_cOnOff = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_add.ucLearnSkillIndex].m_dwCooldownStartTime = 0;

			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(messagestring));
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_LEARNMARTIAL), g_SkillProperty[skillctrl->ctrl.S_skill_add.sAddSkillID]->skillName); // 무공 %s(을)를 배웠습니다.
			pNPCScript_Window->InsertNPCChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
			
			_XWindow_NPCSkill* pNPCSkill_Window = (_XWindow_NPCSkill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSKILL);
			if(pNPCSkill_Window)
			{
				if(pNPCSkill_Window->GetShowStatus())
					pNPCSkill_Window->SetSkillTypeButton();
			}
			
			if(g_pSkill_Window)
			{
				g_pSkill_Window->SetSkillButton();
			}
		}
		break;

	case en_skill_ctrl_mode_learn_fail :
	case en_skill_ctrl_mode_step_up_fail :
		{
			// 배우기 실패, 스킬 업 실패
			switch(skillctrl->ctrl.S_learn_fail.ucFailReason)
			{
			case _SKILL_CAN_LEARN_CONTRIBUTION_ :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR5), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_LEVEL_ :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2333), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_PRE1_ :
			case _SKILL_CAN_LEARN_PRE2_ :
				{
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2334), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_CLAN_ :
				{
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2335), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_SKILL_INDEX_ :
				{
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2336), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_SKILL_FULL_ :
				{
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2337), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_SKILL_ALREADY_ :
				{
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2338), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_NO_MONEY_ :
				{
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2339), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_STEP_FULL_ :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2340), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_MORE_SKILLEXP_ :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2341), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case _SKILL_CAN_LEARN_GRADE_ :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3795), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//직책등급이 모자랍니다,
				}
				break;
			default :
				{
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2342), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			}
		}
		break;

	case en_skill_ctrl_mode_delete :
		{
			// 스킬 삭제
			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(messagestring));
			short skillid = g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_sSkillID;
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_REMOVEMARTIAL), g_SkillProperty[skillid]->skillName); // 무공 %s이(가) 삭제되었습니다.
			pNPCScript_Window->InsertNPCChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

			if(g_pQuickSlot_Window)
			{
				g_pQuickSlot_Window->DeleteQuickSlot(skillid);
			}

			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_sSkillID = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_cSkillLevel = -1;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_cCurMission = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_iSkillExp = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_ucSkillState = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_cSelectedSkillLevel = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_cOnOff = 0;
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_delete.ucLearnSkillIndex].m_dwCooldownStartTime = 0;

			if(g_pSkill_Window)
			{
				g_pSkill_Window->SetSkillButton();
			}
		}
		break;
		
	case en_skill_ctrl_mode_update_step :
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_040 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_040);
					g_TutorialInfo->_XTutorial_040 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=
			// 스킬 정보 업데이트
			if(g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSkillLevel == 
				g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSelectedSkillLevel)
			{
				g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSelectedSkillLevel = skillctrl->ctrl.S_skill_update_step.cStep;
				g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSkillLevel = skillctrl->ctrl.S_skill_update_step.cStep;
				g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_iSkillExp = 0;
				
				// 선택한 성과 최고 성이 같음 - Passive skill on packet 한번 더 보내기
				if(g_SkillProperty[g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL ||
					g_SkillProperty[g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
				{
					if(g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cOnOff == 1)
					{
						g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cOnOff = 0;

						// Off
						short skillid = g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_sSkillID;
						char skilllevel = g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSkillLevel;

						short effectcount = g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount;
						for(int i = 0 ; i < effectcount ; ++i)
						{
							// state 삭제
							g_pLocalUser->m_StateList.DeleteStateFromPassive(skillid, skilllevel, i);
							
							// Stat 계산
							short id = g_SkillProperty[skillid]->detailInfo[skilllevel].effect[i].id;
							map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
							if(iter_table != g_CharacterStateTable.end())
							{
								if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
								{
									g_pLocalUser->UpdateLocalUserAttackLevel();
								}
							}
						}
						
						// visual effect 삭제
						if(g_SkillProperty[skillid]->visualEffect > 0)
						{
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[skillid]->visualEffect);
						}
						
						// icon 삭제
						if(g_pState_Window)
						{
							g_pState_Window->DeleteStateList(_XST_PASSIVE, skillid);
						}
						
						SendPacket(MSG_NO_SKILL_CTRL_REQ, en_skill_ctrl_req_passive, skillid);
					}
				}
			}
			else
			{
				g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSkillLevel = skillctrl->ctrl.S_skill_update_step.cStep;
				g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_iSkillExp = 0;
			}
			
			TCHAR messagestring[256];
			short skillid = g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_sSkillID;

			LPTSTR param1 = g_SkillProperty[skillid]->skillName;
			int    param2 = g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSkillLevel + 1;

			strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORK_1608, param1, &param2 ) );
			InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

			g_pLocalUser->UpdateLocalUserAttackLevel();

			if(g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_sSkillID == g_pLocalUser->m_SelectedSkillID)
			{
				if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
					g_pLocalUser->SetSkillAnimation();
			}
		}
		break;
		
	case en_skill_ctrl_mode_update_exp :
		{
			// 스킬 경험치 업데이트
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_exp.ucLearnSkillIndex].m_iSkillExp = skillctrl->ctrl.S_skill_update_exp.iCurSkillExp;

#ifdef _XDWDEBUG
			TCHAR messagestring[256];
			short skillid = g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_sSkillID;
			
			LPTSTR param1 = g_SkillProperty[skillid]->skillName;
			int    param2 = g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_step.ucLearnSkillIndex].m_cSkillLevel + 1;			
			strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORK_1608, param1, &param2 ) );
			InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
#endif
		}
		break;
		
	case en_skill_ctrl_mode_update_mission :
		{
			// 미션 진행
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_skill_update_mission_count.ucLearnSkillIndex].m_cCurMission = skillctrl->ctrl.S_skill_update_mission_count.cCurMission;
		}
		break;
		
	case en_skill_ctrl_mode_complete_learn :
		{
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_complete_learn.ucLearnSkillIndex].m_sSkillID = skillctrl->ctrl.S_complete_learn.sSkillID;
		}
		break;

	case en_skill_ctrl_mode_update_training :
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_005 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_005);
					g_TutorialInfo->_XTutorial_005 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_update_training.ucLearnSkillIndex].m_ucSkillState = skillctrl->ctrl.S_update_training.ucSkillState;
			
			if(g_pSkill_Window)
			{
				g_pSkill_Window->SetListItem();
			}
		}
		break;
	case en_skill_ctrl_mode_passive :
		{
			if((g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cOnOff == 1) &&
				(skillctrl->ctrl.S_Passive_OnOff.cOnOff == 1))
			{
				// 중복된 패킷 
				break;
			}
			
			g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cOnOff = skillctrl->ctrl.S_Passive_OnOff.cOnOff;
			g_pLocalUser->m_bWaitPassiveReq = FALSE;
			
			// 상태효과 처리
			if(skillctrl->ctrl.S_Passive_OnOff.cOnOff == 1)
			{
				// On
				short effectcount = g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->detailInfo[g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cSelectedSkillLevel].effectCount;

				// Stat 계산
				for(int i = 0 ; i < effectcount ; ++i)
				{
					// state 추가
					g_pLocalUser->m_StateList.InsertStateFromPassive(skillctrl->ctrl.S_Passive_OnOff.sSkillID, 
						g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cSelectedSkillLevel, i);
					
					short id = g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->detailInfo[g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cSelectedSkillLevel].effect[i].id;
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
					if(iter_table != g_CharacterStateTable.end())
					{
						if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
						{
							g_pLocalUser->UpdateLocalUserAttackLevel();
						}
					}
				}
				
				
				// visual effect 추가
				if(g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->visualEffect > 0)
				{
					D3DXMATRIX offsetmat;
					D3DXMatrixIdentity(&offsetmat);
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
						g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->visualEffect, &offsetmat);
				}
					
				// icon 추가
				if(g_pState_Window)
				{
					g_pState_Window->InsertStateListFromOther(_XST_PASSIVE, skillctrl->ctrl.S_Passive_OnOff.sSkillID, g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cSelectedSkillLevel);
				}

#ifdef _XTS_AUTOMEDITATION
				// 자동 운기요상 처리
				if(g_pLocalUser->m_bAutoMeditation)
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MAINMENU_MEDBUTTON, 0), (LPARAM)gHWnd);
				}
#endif
			}
			else
			{
				// Off
				short effectcount = g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->detailInfo[g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cSelectedSkillLevel].effectCount;
				for(int i = 0 ; i < effectcount ; ++i)
				{
					// state 삭제
					g_pLocalUser->m_StateList.DeleteStateFromPassive(skillctrl->ctrl.S_Passive_OnOff.sSkillID, 
						g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cSelectedSkillLevel, i);
					
					// Stat 계산
					short id = g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->detailInfo[g_pLocalUser->m_SkillList[skillctrl->ctrl.S_Passive_OnOff.ucLearnSkillIndex].m_cSelectedSkillLevel].effect[i].id;
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
					if(iter_table != g_CharacterStateTable.end())
					{
						if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
						{
							g_pLocalUser->UpdateLocalUserAttackLevel();
						}
					}
				}
				
				// visual effect 삭제
				if(g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->visualEffect > 0)
				{
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->visualEffect);
				}
				
				// icon 삭제
				if(g_pState_Window)
				{
					g_pState_Window->DeleteStateList(_XST_PASSIVE, skillctrl->ctrl.S_Passive_OnOff.sSkillID);
				}

#ifdef _XTS_AUTOMEDITATION
				// 자동 운기요상 처리
				if(g_pLocalUser->m_bAutoMeditation)
				{
					g_pLocalUser->m_bAutoMeditation = FALSE;
				}
#endif
			}

			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID <= 0)
					continue;
				if(g_pLocalUser->m_SkillList[i].m_sSkillID == skillctrl->ctrl.S_Passive_OnOff.sSkillID)
					continue;

				short skillid = g_pLocalUser->m_SkillList[i].m_sSkillID;
				
				if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL || g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
				{
					if(g_SkillProperty[skillctrl->ctrl.S_Passive_OnOff.sSkillID]->skillSType == g_SkillProperty[skillid]->skillSType)
					{
						// 같은 SType의 passive는 모두 끄기
						if(g_pLocalUser->m_SkillList[i].m_cOnOff == 1)
						{
							g_pLocalUser->m_SkillList[i].m_cOnOff = 0;

							// 상태효과 끄기
							short effectcount = g_SkillProperty[skillid]->detailInfo[g_pLocalUser->m_SkillList[i].m_cSelectedSkillLevel].effectCount;
							for(int j = 0 ; j < effectcount ; ++j)
							{
								// state 삭제
								g_pLocalUser->m_StateList.DeleteStateFromPassive(skillid, g_pLocalUser->m_SkillList[i].m_cSelectedSkillLevel, j);
								
								// Stat 계산
								short id = g_SkillProperty[skillid]->detailInfo[g_pLocalUser->m_SkillList[i].m_cSelectedSkillLevel].effect[j].id;
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										g_pLocalUser->UpdateLocalUserAttackLevel();
									}
								}
							}
							
							// visual effect 삭제
							if(g_SkillProperty[skillid]->visualEffect > 0)
							{
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[skillid]->visualEffect);
							}
							
							// icon 삭제
							if(g_pState_Window)
							{
								g_pState_Window->DeleteStateList(_XST_PASSIVE, skillid);
							}
						} // if(g_pLocalUser->m_SkillList[i].m_cOnOff == 1)
					}
				}
			}

			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
			{
				pMainMenu_Window->SetMedButton();
			}
		}
		break;
	case en_skill_ctrl_mode_cooltime :
		{
			// Cooldown time update
			int learnedindex = g_pLocalUser->GetLearnedSkillIndex(skillctrl->ctrl.S_Cooltime_Update.skillID);

			if(learnedindex >= 0)
			{
				DWORD elapsedtime = 
					_XSkillItem::GetCoolDown(skillctrl->ctrl.S_Cooltime_Update.skillID, g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel)
					//g_SkillProperty[skillctrl->ctrl.S_Cooltime_Update.skillID]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].coolDown 
					- (DWORD)(skillctrl->ctrl.S_Cooltime_Update.usTimeUpdate * 1000);
				
				g_pLocalUser->m_SkillList[learnedindex].m_dwCooldownTime = _XSkillItem::GetCoolDown(skillctrl->ctrl.S_Cooltime_Update.skillID, g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel);
					//g_SkillProperty[skillctrl->ctrl.S_Cooltime_Update.skillID]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].coolDown;
				
				g_pLocalUser->m_SkillList[learnedindex].m_dwCooldownStartTime = g_LocalSystemTime - elapsedtime;
			}
		}
		break;
	case en_skill_ctrl_mode_bookinfo :
		{
			if( skillctrl->ctrl.S_Bookinfo.sBookID > 0 )
			{
				// icon 추가
				if(g_pState_Window)
					g_pState_Window->InsertStudyBookBuff(_XST_STUDYBOOK, skillctrl->ctrl.S_Bookinfo.sBookID, skillctrl->ctrl.S_Bookinfo.uiLeftSecond );
			}			
		}
		break;
	case en_skill_ctrl_mode_bookend :
		{
			// icon 삭제
			if( g_pState_Window )	
				g_pState_Window->DeleteStudyBookBuff(_XST_STUDYBOOK, skillctrl->ctrl.S_Bookinfo.sBookID);	// 버프 제거
		}
		break;
	case en_skill_ctrl_mode_reset_mastery :
		{
			// 오의 초기화
			g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( skillctrl->ctrl.S_Mastery_Reset.sLeftMasteryPoint );

			for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
			{
				g_pLocalUser->m_MasterySkillList[i].skillid = 0;
				g_pLocalUser->m_MasterySkillList[i].cPoint = 0;
			}
			
			// skill mod structure 초기화
			_XSkillItem::ReleaseSkillModStructure();

			if(g_pMasterySkill_Window)
			{
				g_pMasterySkill_Window->RefreshData();
			}
		}
		break;
	case en_skill_ctrl_mode_del_bonus_hyperrun :
		{
			if(skillctrl->ctrl.S_Del_Bonus_Hyperrun.slot == 250)
			{
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3811), g_SkillProperty[g_pLocalUser->m_EventSkillList.m_sSkillID]->skillName); //%s의 사용 시간이 종료되었습니다.
				InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

				// 스킬 삭제
				g_pLocalUser->m_EventSkillList.m_sSkillID = 0;
				g_pLocalUser->m_EventSkillList.m_cSkillLevel = 0;
				g_pLocalUser->m_EventSkillList.m_cCurMission = 0;
				g_pLocalUser->m_EventSkillList.m_iSkillExp = 0;
				g_pLocalUser->m_EventSkillList.m_ucSkillState = 0;
				g_pLocalUser->m_EventSkillList.m_cSelectedSkillLevel = 0;
				g_pLocalUser->m_EventSkillList.m_cOnOff = 0;
				g_pLocalUser->m_EventSkillList.m_dwCooldownStartTime = 0;

				// 끝나는 시간
				g_pLocalUser->m_EventSkillEndTime.dwYear = 0;
				g_pLocalUser->m_EventSkillEndTime.dwMonth = 0;
				g_pLocalUser->m_EventSkillEndTime.dwDay = 0;
				g_pLocalUser->m_EventSkillEndTime.dwHour = 0;
				g_pLocalUser->m_EventSkillEndTime.dwMinute = 0;
				g_pLocalUser->m_EventSkillEndTime.dwSecond = 0;
			}
		}
		break;
	}
}

void _XNetwork::MSGSkillActionReson(MSG_CANCEL_SKILL_CANNOT* skillaction)
{
	switch(skillaction->ucReason) 
	{
	case en_cannot_act_no_mask_effect :
		{
			
		}
		break;
	case en_cannot_act_no_extra_effect :
		{
			
		}
		break;
	}
}

void _XNetwork::MSGSkillInfoMastery(MSG_SKILL_INFO_MASTERY* skillinfo)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		g_pLocalUser->m_MasterySkillList[i].skillid = 0;
		g_pLocalUser->m_MasterySkillList[i].cPoint = 0;
	}
	
	for(i = 0 ; i < skillinfo->ucMasteryCount ; ++i)
	{
		if(_XSkillItem::FindMasterySkillProperty(skillinfo->mastery_info[i].skillID))
		{
			g_pLocalUser->m_MasterySkillList[i].skillid = skillinfo->mastery_info[i].skillID;
			g_pLocalUser->m_MasterySkillList[i].cPoint = skillinfo->mastery_info[i].cPoint;
		}
	}
	
	// mastery window setting
	_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
	if(pMainFrame_Window)
	{
		if(pMainFrame_Window->GetShowStatus() && pMainFrame_Window->GetCurrentSelectedTab() == _XDEF_MAINFRAME_MASTERYTAB)
		{
			if(g_pMasterySkill_Window)
			{
				g_pMasterySkill_Window->ParseMasterySkill();
				g_pMasterySkill_Window->CheckDependency();
				
			}
		}
	}
	
	g_pLocalUser->UpdateLocalUserAttackLevel();
	
	_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
	if(pMainMenu_Window)
	{
		pMainMenu_Window->SetMedButton();
	}
}

void _XNetwork::MSGMasteryPointResult(MSG_MASTERY_POINT_RESULT* pointresult)
{
	bool bChange = false;
	
	if(pointresult->ucFail == 0)
	{
		g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( pointresult->sLeftMasteryPoint );
		
		int learnedindex = g_pLocalUser->GetLearnedMasterySkillIndex(pointresult->sMasteryID);
		if(learnedindex >= 0)
		{
			g_pLocalUser->m_MasterySkillList[learnedindex].cPoint = pointresult->ucMasteryStep;
			bChange = true;
		}
		else
		{
			for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
			{
				if(g_pLocalUser->m_MasterySkillList[i].skillid == 0)
				{
					g_pLocalUser->m_MasterySkillList[i].skillid = pointresult->sMasteryID;
					g_pLocalUser->m_MasterySkillList[i].cPoint = pointresult->ucMasteryStep;
					bChange = true;
					break;
				}
			}
			if(i == _XDEF_MAX_LEARNMASTERYSKILL)
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2337), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
		}
	}
	else
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2344), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
	
	if(bChange)
	{
		if(g_pMasterySkill_Window)
		{
			g_pMasterySkill_Window->ParseMasterySkill();
			g_pMasterySkill_Window->CheckDependency();
		}
		
		g_pLocalUser->UpdateLocalUserAttackLevel();		
	}
	// 능력치업버튼 닫기 ----------------------------------------------------------------------------------------------------------=		
	if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() == 0 && g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() == 0 )
	{
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->m_AbilityUpButton->ShowWindow(FALSE);
	}
	// ----------------------------------------------------------------------------------------------------------------------------=
}

void _XNetwork::MSGSkillSelect(MSG_SKILL_SELECT* skillselect)
{
	if(skillselect->sSkillID < 0)
	{
		// skill select 실패
		if(g_pQuickSlot_Window)
		{
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_ACTIVESKILL)	
				g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(-1);
			else
				g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(-1);
		}
		
		g_pLocalUser->ResetSkillAnimation();
		g_pLocalUser->m_SelectedSkillID = 0;
		g_pLocalUser->m_SelectedSkillItem = NULL;
		
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2345), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
}

void _XNetwork::MSGSkillInfoCoolTime(MSG_SKILL_INFO_COOLTIME* cooltime)
{
	for(int i = 0 ; i < cooltime->ucCoolTimeListCount ; ++i)
	{
		// cooldown time 처리
		int learnedindex = g_pLocalUser->GetLearnedSkillIndex(cooltime->cooltime[i].skillID);
		
		if(learnedindex >= 0)
		{
			DWORD elapsedtime = 
				_XSkillItem::GetCoolDown(cooltime->cooltime[i].skillID, g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel)
				//g_SkillProperty[cooltime->cooltime[i].skillID]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].coolDown 
				- (DWORD)(cooltime->cooltime[i].usLeftCoolTime * 1000);
			
			g_pLocalUser->m_SkillList[learnedindex].m_dwCooldownTime = _XSkillItem::GetCoolDown(cooltime->cooltime[i].skillID, g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel);
			//g_SkillProperty[cooltime->cooltime[i].skillID]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].coolDown;
			
			g_pLocalUser->m_SkillList[learnedindex].m_dwCooldownStartTime = g_LocalSystemTime - elapsedtime;
		}
	}
}

// 외치기 
void _XNetwork::MSGEtcsetShoutSet(MSG_ETCSET_SHOUT_SET* shout)
{
#ifdef _XDEF_CASHITEM_SHOUT
	if(shout->ucMode == en_etcset_mode_shout_set)
	{
		// 외치기 시작
		g_pLocalUser->m_ShoutMode = shout->cType;
		g_pLocalUser->m_ShoutLeftTime = shout->iLeftSecond * 1000;		// ms
		g_pLocalUser->m_ShoutStartTime = g_LocalSystemTime;
		
		_XWindow_ChattingDefault* pChattingWindowDefault = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
		if(pChattingWindowDefault)
		{
			// 외치기 버튼 enable
			pChattingWindowDefault->m_btnChatFunctionMenuButton[1]->EnableWindow(TRUE);
		}
		
		if(g_pState_Window)
		{
			if(g_pState_Window->FindPlayerStateList(_XST_SHOUT, 0, 0, 0))
			{
				// 기존의 외치기 있음 - 지운다
				g_pState_Window->DeletePlayerStateList(_XST_SHOUT, 0);
			}
			
			g_pState_Window->InsertPlayerStateList(_XST_SHOUT, g_pLocalUser->m_UserItemList[g_pLocalUser->m_UseItemInvenNumber].Get_m_sID(), 
					g_PotionItemProperty[g_pLocalUser->m_UserItemList[g_pLocalUser->m_UseItemInvenNumber].Get_m_sID()].cType, g_pLocalUser->m_UseItemInvenNumber);
		}
	}
	else if(shout->ucMode == en_etcset_mode_shout_update)
	{
		// 시간 업데이트
		g_pLocalUser->m_ShoutMode = shout->cType;
		g_pLocalUser->m_ShoutLeftTime = shout->iLeftSecond * 1000;		// ms
		g_pLocalUser->m_ShoutStartTime = g_LocalSystemTime;

		_XWindow_ChattingDefault* pChattingWindowDefault = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
		if(pChattingWindowDefault)
		{
			// 외치기 버튼 enable
			pChattingWindowDefault->m_btnChatFunctionMenuButton[1]->EnableWindow(TRUE);
		}

		if(g_pState_Window)
		{
			g_pState_Window->InsertPlayerStateList(_XST_SHOUT, g_pLocalUser->m_UserItemList[g_pLocalUser->m_UseItemInvenNumber].Get_m_sID(), 
				g_PotionItemProperty[g_pLocalUser->m_UserItemList[g_pLocalUser->m_UseItemInvenNumber].Get_m_sID()].cType, g_pLocalUser->m_UseItemInvenNumber);
		}
	}
#endif
}

void _XNetwork::MSGEtcsetShoutEnd(MSG_ETCSET_SHOUT_END* shout)
{
	// 외치기 끝
#ifdef _XDEF_CASHITEM_SHOUT
	g_pLocalUser->ResetShout();
#endif
}

void _XNetwork::MSGEtcsetShoutError(MSG_ETCSET_SHOUT_ERROR* shout)
{
	// 에러 - 초기화
#ifdef _XDEF_CASHITEM_SHOUT
	g_pLocalUser->ResetShout();
#endif
}

void _XNetwork::MSGAiusGradeSet(MSG_AIUS_GRADE_SET* aius)
{
	if(aius->curGrade.cItemType != _XGI_FC_POTION)
		return;

	g_pLocalUser->SetExpItemEndTime(aius->curGrade.uiYear, aius->curGrade.uiMonth, aius->curGrade.uiDay, aius->curGrade.uiHour, 
		aius->curGrade.uiMinute, aius->curGrade.uiSecond);

	g_pLocalUser->m_AccountItemcType = aius->curGrade.cItemType;
	g_pLocalUser->m_AccountItemsID = aius->curGrade.sItemID;
	
#ifdef _ACCLAIM_RUBICONADSYSTEM	

	g_RubiconADRefreshTimer = 0;	
	if( (g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0) || 
		g_RubiconADFreeLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
	{
		g_ViewRubiconAD = FALSE;
	}
	else
	{
		g_ViewRubiconAD = TRUE;
	}

	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetUIforRubiconADSystem();
#endif



	// window 띄우기
	_XWindow_AccountItemUse* pAccountItemUse_Window = (_XWindow_AccountItemUse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ACCOUNTITEMUSE);
	if(pAccountItemUse_Window)
	{
		pAccountItemUse_Window->ShowWindow(TRUE);
	}

#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI

	if(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 4 || g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 5 ||
		g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi == 6) //Author : 양희왕 //breif : 구룡천하 심기 6 추가됨
	{	
		if(g_pJinStatus_Window)
		{
			if(g_pJinStatus_Window->GetSDSState() == 2)
			{
				int lefttime = 0;
				if( g_PlayLeftTime > 0 )
					lefttime = g_PlayLeftTime - (g_LocalSystemTime - g_PlayTime);

				if(lefttime > 0)
				{
					// 구룡천하 사용했으므로 on/off 기능 사용 가능
					g_pJinStatus_Window->SetSDSState(0);
				}
			}
		}
	}
#endif
}

void _XNetwork::MSGAiusGradeEnd(MSG_AIUS_GRADE_END* aius)
{
	g_pLocalUser->ResetExpItemEndTime();

	g_pLocalUser->m_AccountItemcType = 0;
	g_pLocalUser->m_AccountItemsID = 0;

	// window 없애기
	_XWindow_AccountItemUse* pAccountItemUse_Window = (_XWindow_AccountItemUse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ACCOUNTITEMUSE);
	if(pAccountItemUse_Window)
	{
		pAccountItemUse_Window->ShowWindow(FALSE);
	}

#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
	if(g_pJinStatus_Window)
	{
		g_pJinStatus_Window->SetSDSState(2);
	}
#endif
	
#ifdef _ACCLAIM_RUBICONADSYSTEM	

	g_RubiconADRefreshTimer = 0;	
	
	if( (g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0) || 
		g_RubiconADFreeLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
	{
		g_ViewRubiconAD = FALSE;
	}
	else
	{
		g_ViewRubiconAD = TRUE;
	}

	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetUIforRubiconADSystem();

#endif

}

void _XNetwork::MSGBWLeftTime(MSG_BW_LEFT_TIME* time)
{
	g_pLocalUser->m_dwBWLeftTimeFromServer = time->iLeftTime * 1000;
	g_pLocalUser->m_dwBWReceiveTimeFromServer = g_LocalSystemTime;	// 받은 시간 기록
}

void _XNetwork::MSGBWEnterGateResult(MSG_BW_ENTER_GATE_SUCCESS* result)
{
	if(result->ucBWMode == en_bwwar_enter_gate_success)
	{
		for(int i = 0; i < g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject; ++i)
		{
			if(g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalObjectArray[i].m_UniqueID == result->usGateIndex)
			{
				g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalObjectArray[i].ProcessMove();
				break;
			}
		}
	}
	else if(result->ucBWMode == en_bwwar_enter_gate_fail)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3806), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//출입할 수 있는 권한이 없습니다.
	}
	
	g_pLocalUser->m_bSendFunctionalMovePacket = FALSE;
	g_pLocalUser->m_nFuctionalNormalAction = 0;
}