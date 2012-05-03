// XNetwork_Battle.cpp : 전투 관련 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XWindow_TrainingHelp.h"
#include "XWindow_TrainingMed.h"
#include "XWindow_TrainingDummy.h"
#include "XWindow_TrainingSpell.h"
#include "XWindow_JinStatus.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_ChargeGauge.h"
#include "XwindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XMessageWindow.h"
#include "XParticleidDefine.h"
#include "XSR_STRINGHEADER.H"

extern _map_NPCBuffStr				g_NPCBuffStr;
extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

void _XNetwork::MSGRegionDamage(MSG_REGION_DAMAGE* regiondamage)
{
	// 기록
	if(regiondamage->ucAttackerType == 1)	// attacker가 pc
	{
		if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
		{
			// 자기 자신
			memset(&g_pLocalUser->m_RegionDamageResult, 0, sizeof(g_pLocalUser->m_RegionDamageResult));

			g_pLocalUser->m_RegionDamageResult.bValid = TRUE;
			g_pLocalUser->m_RegionDamageResult.nObjectCnt = regiondamage->ucObjectCount;
			g_pLocalUser->m_RegionDamageResult.nSkillID = regiondamage->sSkillID;

			for(int i = 0 ; i < regiondamage->ucObjectCount ; ++i)
			{
				g_pLocalUser->m_RegionDamageResult.sResult[i].nObjectType = regiondamage->Result[i].ucObjType;
				g_pLocalUser->m_RegionDamageResult.sResult[i].nID = regiondamage->Result[i].usID;
			}
		}
		else
		{
			_XUser* pUser = g_Userlist.FindUser(regiondamage->usAttackerID);
			if(pUser)
			{
				memset(&pUser->m_RegionDamageResult, 0, sizeof(pUser->m_RegionDamageResult));

				pUser->m_RegionDamageResult.bValid = TRUE;
				pUser->m_RegionDamageResult.nObjectCnt = regiondamage->ucObjectCount;
				pUser->m_RegionDamageResult.nSkillID = regiondamage->sSkillID;

				for(int i = 0 ; i < regiondamage->ucObjectCount ; ++i)
				{
					pUser->m_RegionDamageResult.sResult[i].nObjectType = regiondamage->Result[i].ucObjType;
					pUser->m_RegionDamageResult.sResult[i].nID = regiondamage->Result[i].usID;
				}
			}
		}
	}

	for(int i = 0 ; i < regiondamage->ucObjectCount ; ++i)
	{
		if(regiondamage->Result[i].ucObjType == 0)
		{
			// 맞은 대상이 몬스터
			_XMob* pMob = g_Moblist.FindMob(regiondamage->Result[i].usID);
			if(pMob)
			{
				if(g_BattleLog.GetWriteLog())
				{
					if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
					{
						// 자기자신의 공격만 기록한다.
						g_BattleLog.SetTime();
						g_BattleLog.SetTargetGroup(_XPICKINGTARGET_MOB);
						g_BattleLog.SetTargetType(pMob->m_MobType);
						g_BattleLog.SetAtkInfo(regiondamage->Result[i].ucResult);
						g_BattleLog.SetDamage(pMob->m_HealthPercentage, regiondamage->Result[i].cMobVitalRate);
						g_BattleLog.DumpLogResult();
					}
				}

			#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
				if(g_ViewBattleDetail)
				{
					if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						if(regiondamage->Result[i].ucResult == 1)
							_snprintf(messagestring, sizeof(messagestring), _T("RegionDamage To monster CRITICAL damage[%d]"), pMob->m_HealthPercentage-regiondamage->Result[i].cMobVitalRate);
						else if(regiondamage->Result[i].ucResult == 2)
							_snprintf(messagestring, sizeof(messagestring), _T("RegionDamage To monster MISS damage[%d]"), pMob->m_HealthPercentage-regiondamage->Result[i].cMobVitalRate);
						else
							_snprintf(messagestring, sizeof(messagestring), _T("RegionDamage To monster HIT damage[%d]"), pMob->m_HealthPercentage-regiondamage->Result[i].cMobVitalRate);

						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
					}
				}
			#endif

				pMob->m_HealthPercentage = regiondamage->Result[i].cMobVitalRate;
				pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;		// Monster Health 적용(Display용)
				pMob->m_LastRecivedMessageTime = g_LocalSystemTime;

				// 이펙트 보여주지 않음 - type에 따라 나중에 처리.
/*				
				switch(regiondamage->Result[i].ucResult)
				{
				case 0 :
					{
						if(pMob->m_ModelDescriptor.m_EffectManager)
						{
							if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
							{
								D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
								resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
								
								if( g_ViewDamageValue )
								{

								}
								else
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
								}
							}

							if(regiondamage->ucAttackerType == 1)
							{
								if(_XSkillItem::FindSkillProperty(regiondamage->sSkillID))
								{
									// visual effect는 다른 사람이 쏜 것도 보이도록
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[regiondamage->sSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
						}
					}
					break;
				case 1 :
				case 4 :
				case 5 :
				case 6 :
					{
						if(regiondamage->ucAttackerType == 1)
						{
							if(pMob->m_ModelDescriptor.m_EffectManager)
							{
								if(_XSkillItem::FindSkillProperty(regiondamage->sSkillID))
								{
									// visual effect는 다른 사람이 쏜 것도 보이도록 : miss/block 일때 제외
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[regiondamage->sSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
						}
					}
					break;
				case 2 :
					{
						if(pMob->m_ModelDescriptor.m_EffectManager)
						{
							if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
							{
								D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
								resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
								
								if( g_ViewDamageValue )
								{

								}
								else
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}
							}
						}
					}
					break;
				}
*/

				if(regiondamage->Result[i].ucMobDead == 1)
				{
					pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
					pMob->SetAnimation(_XMOBACTION_DIE);
					pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
				}
			}
			else
			{
				_XFunctionalObject* pFunctionalObject = 
					(_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(regiondamage->Result[i].usID);
				if(pFunctionalObject)
				{

					if(g_BattleLog.GetWriteLog())
					{
						if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
						{
							// 자기자신의 공격만 기록한다.
							g_BattleLog.SetTime();
							g_BattleLog.SetTargetGroup(_XPICKINGTARGET_FUNCTIONALOBJECT);
							g_BattleLog.SetTargetType(pFunctionalObject->m_ObjectType);
							g_BattleLog.SetAtkInfo(regiondamage->Result[i].ucResult);
							g_BattleLog.SetDamage(pFunctionalObject->m_HealthPercentage, regiondamage->Result[i].cMobVitalRate);
							g_BattleLog.DumpLogResult();
						}
					}

					pFunctionalObject->m_HealthPercentage = regiondamage->Result[i].cMobVitalRate;
					pFunctionalObject->m_DisplayHealthPercentage = regiondamage->Result[i].cMobVitalRate;

					// 이펙트 보여주지 않음 - type에 따라 나중에 처리.
					/*
					switch(regiondamage->Result[i].ucResult)
					{
					case 0 :
					case 1 :
					case 4 :
					case 5 :
					case 6 :
						{
							if(regiondamage->ucAttackerType == 1)
							{
								if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
								{
									if(_XSkillItem::FindSkillProperty(regiondamage->sSkillID))
									{
										// visual effect는 다른 사람이 쏜 것도 보이도록 : miss/block 일때 제외
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[regiondamage->sSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);
									}
								}
							}
						}
						break;
					}
					*/

					if(regiondamage->Result[i].ucMobDead == 1)
					{
						pFunctionalObject->SetDieAnimation();
					}
				}
			}
		}
		else if(regiondamage->Result[i].ucObjType == 1)
		{
			// 맞은 대상이 유저
			if(regiondamage->Result[i].usID == g_pLocalUser->m_Uniqueid)
			{
				if(regiondamage->ucAttackerType == 1)
				{
					// 내가 맞았음
					if(g_pLocalUser->m_ModelDescriptor.m_EffectManager)
					{
						// 이펙트 보여주지 않음 - type에 따라 나중에 처리.
						/*
						if(_XSkillItem::FindSkillProperty(regiondamage->sSkillID))
						{
							// visual effect는 다른 사람이 쏜 것도 보이도록 : miss/block 일때 제외
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								g_SkillProperty[regiondamage->sSkillID]->visualEffect, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE);
						}*/
					}
				}

				// 자리 비움 상태 처리
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
			}
			else
			{
				// 다른 유저
				_XUser* pUser = g_Userlist.FindUser(regiondamage->Result[i].usID);
				if(pUser)
				{

					if(g_BattleLog.GetWriteLog())
					{
						if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
						{
							// 자기자신의 공격만 기록한다.
							g_BattleLog.SetTime();
							g_BattleLog.SetTargetGroup(_XPICKINGTARGET_PC);
							g_BattleLog.SetTargetType(pUser->m_Uniqueid);
							g_BattleLog.SetAtkInfo(regiondamage->Result[i].ucResult);
							g_BattleLog.SetDamage(0);
							g_BattleLog.DumpLogResult();
						}
					}

					if(g_ViewBattleDetail)
					{
						if(regiondamage->usAttackerID == g_pLocalUser->m_Uniqueid)
						{
							TCHAR messagestring[128];
							memset(messagestring, 0, sizeof(messagestring));

							if(regiondamage->Result[i].ucResult == 1)
								_snprintf(messagestring, sizeof(messagestring), _T("RegionDamage To User CRITICAL"));
							else if(regiondamage->Result[i].ucResult == 2)
								_snprintf(messagestring, sizeof(messagestring), _T("RegionDamage To User MISS"));
							else
								_snprintf(messagestring, sizeof(messagestring), _T("RegionDamage To User HIT"));

							g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
						}
					}

					pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

					// 이펙트 보여주지 않음 - type에 따라 나중에 처리.
					/*
					switch(regiondamage->Result[i].ucResult)
					{
					case 0 :
					case 1 :
					case 4 :
					case 5 :
					case 6 :
						{
							if(regiondamage->ucAttackerType == 1)
							{
								if(pUser->m_ModelDescriptor.m_EffectManager)
								{
									if(_XSkillItem::FindSkillProperty(regiondamage->sSkillID))
									{
										// visual effect는 다른 사람이 쏜 것도 보이도록 : miss/block 일때 제외
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[regiondamage->sSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
									}
								}
							}
						}
						break;
					}
					*/
				}
			}
		}
	}
}

void _XNetwork::MSGSelfChannelCtrl(MSG_SELF_CHANNEL_CTRL* selfchannelctrl)
{
	if(selfchannelctrl->skillID != g_pLocalUser->m_SelectedSkillID)
	{
		if(selfchannelctrl->skillID == g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->insideType)
		{
			// Hidden
			switch(selfchannelctrl->cResultInfo)
			{
			case 0 :
				{
					// 시작
					g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_2;
					g_pLocalUser->m_HiddenReceivePacketTime = g_LocalSystemTime;
				}
				break;
			case 1 :
				{
					// 끝
					g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_6;
					
#ifdef _XTS_AUTOMEDITATION
					if(g_pLocalUser->m_bAutoMeditation)
					{
						SendPacket(MSG_NO_SKILL_CTRL_REQ, en_skill_ctrl_req_passive, selfchannelctrl->skillID);
					}
#endif
				}
				break;
			case 2 :
				{
					// 공격에 의해 취소
					g_pLocalUser->m_bUsingSkill = FALSE;
					g_pLocalUser->m_bCheckDelayTime = TRUE;
					g_pLocalUser->m_ChangeDelayStartTime = g_LocalSystemTime;
					
					g_pLocalUser->ResetSkillAnimation();

#ifdef _XTS_AUTOMEDITATION
					if(g_pLocalUser->m_bAutoMeditation)
					{
						SendPacket(MSG_NO_SKILL_CTRL_REQ, en_skill_ctrl_req_passive, selfchannelctrl->skillID);
					}
#endif
				}
				break;
			case 3 :
				{
					// 스킬 아이디 안맞음
					g_pLocalUser->ResetSkillAnimation();

#ifdef _XTS_AUTOMEDITATION
					if(g_pLocalUser->m_bAutoMeditation)
					{
						SendPacket(MSG_NO_SKILL_CTRL_REQ, en_skill_ctrl_req_passive, selfchannelctrl->skillID);
					}
#endif
				}
				break;
			case 4 :
				{
					// 운기조식 스킬이 아님
					g_pLocalUser->ResetSkillAnimation();

#ifdef _XTS_AUTOMEDITATION
					if(g_pLocalUser->m_bAutoMeditation)
					{
						SendPacket(MSG_NO_SKILL_CTRL_REQ, en_skill_ctrl_req_passive, selfchannelctrl->skillID);
					}
#endif
				}
				break;
			case 5 : 
				{
					// 이미 채널링 시전중
				}
				break;
			default :
				{
					_XDWINBATTLELOG("=== MSGSelfChannelCtrl(Hidden) : Invalid Result Info [%d]", selfchannelctrl->cResultInfo);
				}
				break;
			}
		}
		else
		{
			_XDWINBATTLELOG("=== MSGSelfChannelCtrl(Hidden) Error");
			g_pLocalUser->ResetSkillAnimation();
		}
	}
	else
	{
		// Self Channeling
		switch(selfchannelctrl->cResultInfo)
		{
		case 0 :
			{
				// 시작
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_2;
//				g_pLocalUser->m_ChannelingReceivePacketTime = g_LocalSystemTime;

				_XDWINBATTLELOG("============ Self Channeling start");
				
			}
			break;
		case 1 :
			{
				// 끝
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_6;

				_XDWINBATTLELOG("============ Self Channeling end");
			}
			break;
		case 2 :
			{
				// 공격에 의해 취소
				g_pLocalUser->m_bUsingSkill = FALSE;
				g_pLocalUser->m_bCheckDelayTime = TRUE;

				g_pLocalUser->ResetSkillAnimation();

				_XDWINBATTLELOG("============ Self Channeling cancel");

				if( g_pLocalUser->m_SelectedSkillID )
				{
					if(	g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
					{
						g_pLocalUser->m_HiperRunFailedFlag = TRUE;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
					}
				}
			}
			break;
		case 3 :
			{
				// 스킬 아이디 안맞음
				g_pLocalUser->ResetSkillAnimation();

				_XDWINBATTLELOG("=== MSGSelfChannelCtrl : Wrong Skill ID");

				if( g_pLocalUser->m_SelectedSkillID )
				{
					if(	g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
					{
						g_pLocalUser->m_HiperRunFailedFlag = TRUE;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
					}
				}
			}
			break;
		case 4 :
			{
				// 운기조식 스킬이 아님
				g_pLocalUser->ResetSkillAnimation();

				VIEWPACKETLOG
					_XDWINBATTLELOG("=== MSGSelfChannelCtrl : Not SelfChanneling spell");

				if( g_pLocalUser->m_SelectedSkillID )
				{
					if(	g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID)/*g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
					{
						g_pLocalUser->m_HiperRunFailedFlag = TRUE;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
					}
				}
			}
			break;
		case 6 :
			{
				// 경공 시전 할 수 없음
				g_pLocalUser->ResetSkillAnimation();

				if( g_pLocalUser->m_SelectedSkillID )
				{
					if(	g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF &&
						g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
					{
						g_pLocalUser->m_UseSpeedSkill = FALSE;						
						g_pLocalUser->m_HiperRunFailedFlag = TRUE;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
					}
				}
				
				VIEWPACKETLOG
					_XDWPACKETLOG("=== MSGSelfChannelCtrl(SpeedSkill) : Cancel");
			}
			break;
		default :
			{
				_XDWINBATTLELOG("=== MSGSelfChannelCtrl : Invalid Result Info [%d]", selfchannelctrl->cResultInfo);

				if( g_pLocalUser->m_SelectedSkillID )
				{
					if(	g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID)/*g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
					{
						g_pLocalUser->m_HiperRunFailedFlag = TRUE;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->detailInfo[g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					}
				}
			}
			break;
		}
	}
}

void _XNetwork::MSGSelfChannelInfo(MSG_SELF_CHANNEL_INFO* selfchannelinfo)
{
	if(selfchannelinfo->usID != g_pLocalUser->m_Uniqueid)
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(selfchannelinfo->usID);
		if(pUser)
		{
			switch(selfchannelinfo->cFlag)
			{
			case 0 :
				{
					// 스킬 시작
					memset(&pUser->m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));

					short skillid = 0;
					if(g_SkillProperty[selfchannelinfo->skillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
						skillid = g_SkillProperty[selfchannelinfo->skillID]->insideType;
					else 
						skillid = selfchannelinfo->skillID;

					short motionindex = -1;
					if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
						motionindex = g_SkillProperty[skillid]->motion_male;
					else
						motionindex = g_SkillProperty[skillid]->motion_female;

					pUser->SetAnimationIndex(motionindex);
					pUser->m_AttackResult.skillType = g_SkillProperty[skillid]->usefulType;
					pUser->m_AttackResult.sSkillID = skillid;
					pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

					pUser->DeleteWeaponModel(skillid);
				}
				break;
			case 1 :
				{
					// 스킬 중지
					pUser->m_AttackResult.display[0] = TRUE;
					pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

					// 들고 있던 무기 복구
					pUser->ReloadWeaponModel();
				}
				break;
			case 2 :
				{
					// 시전 중
					pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

					short skillid = 0;
					if(g_SkillProperty[selfchannelinfo->skillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
						skillid = g_SkillProperty[selfchannelinfo->skillID]->insideType;
					else 
						skillid = selfchannelinfo->skillID;

					if(pUser->m_AttackResult.skillType != g_SkillProperty[skillid]->usefulType ||
						pUser->m_AttackResult.sSkillID != skillid)
					{
						// 스킬 시전 중인 유저의 정보를 처음 받음 - 스킬 시작
						memset(&pUser->m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
						
						short motionindex = -1;
						if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
							motionindex = g_SkillProperty[skillid]->motion_male;
						else
							motionindex = g_SkillProperty[skillid]->motion_female;
						
						pUser->SetAnimationIndex(motionindex);
						pUser->m_AttackResult.skillType = g_SkillProperty[skillid]->usefulType;
						pUser->m_AttackResult.sSkillID = skillid;
						pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
					
						pUser->DeleteWeaponModel(skillid);
					}
				}
				break;
			}
		}
	}
}

void _XNetwork::MSGBattleAtkResFromMobWithPos(MSG_B_ATKRES_FROM_MOB_WITH_POS* monsterattackwithpos)
{
	_XMob* pMob = (_XMob*)g_Moblist.FindMob(monsterattackwithpos->usAttackerID);
	if(pMob)
	{
		switch(monsterattackwithpos->ucToType)
		{
		case 0 :	// monster
			{
				_XDWINBATTLELOG("[NETWORK/MSGBattleArkResFromMob] Monster Attackto Monster [%d/%d]", monsterattackwithpos->usAttackerID, monsterattackwithpos->usDefenderID);
				
				_XMob* pTarget = (_XMob*)g_Moblist.FindMob(monsterattackwithpos->usDefenderID);
				if(pTarget)
				{					
					if(monsterattackwithpos->ucAtkType == 0)
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK1);
					else if(monsterattackwithpos->ucAtkType == 1)
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK2);
					else if(monsterattackwithpos->ucAtkType == 2)
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK3);
					else if(monsterattackwithpos->ucAtkType == 3)
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RATTACK);
					
					pMob->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;
					pMob->m_AttackTarget = pTarget;
					pMob->m_AttackHitValue = monsterattackwithpos->sDamage;
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					pMob->m_AttackInfo = monsterattackwithpos->ucAtkInfo;

					pTarget->m_LastRecivedMessageTime = g_LocalSystemTime;

					// 위치 보정
					pMob->m_EndPosition.x = monsterattackwithpos->fItemDropPosX;
					pMob->m_EndPosition.y = 0.0f;
					pMob->m_EndPosition.z = monsterattackwithpos->fItemDropPosZ;

					_XDWINBATTLELOG("[NETWORK/MSGBattleAtkResFromMOb] EndPosition 보정 [x:%f  z:%f]", monsterattackwithpos->fItemDropPosX, monsterattackwithpos->fItemDropPosZ);

					pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pTarget->m_Position);
					pMob->UpdatePosition();

					if(monsterattackwithpos->sDamage > 0)
					{
						// Damage 적용 - 필요한가?
					}
					else
					{
						_XDWINBATTLELOG("[NETWORK/MSGBattleAtkResFromMOb] TargetType : monster / Target Dead");

						// 특수공격 2 일때는 대상에게 상태효과 Visual effect 출력, 
						// 임시 코드이며 차후에 스크립트로 각 상태효과별 이펙트 출력 예정 : kerygma
						if(monsterattackwithpos->ucAtkType == 2)
						{
							if( pTarget->m_ModelDescriptor.m_EffectManager )
							{
								D3DXMATRIX offsetmat;
								D3DXMatrixIdentity(&offsetmat);

								pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance(
									&g_CharacterEffectGroupManager, 10036, &offsetmat );
							}
						}

						pTarget->m_HealthPercentage = 0;
						pTarget->m_DisplayHealthPercentage = pTarget->m_HealthPercentage;		// Monster Health 적용(Display용)
						pTarget->m_DieSignalReceiveTime = g_LocalSystemTime;
					}					
				}
			}
			break;
		case 1 :	// PC
			{
				if(monsterattackwithpos->usDefenderID == g_pLocalUser->m_Uniqueid)
				{	
					_XDWINBATTLELOG("[NETWORK/MSGBattleArkResFromMob] Monster Attackto localUSer [%d]", monsterattackwithpos->usAttackerID);

					// Local user를 공격					
					if( monsterattackwithpos->ucAtkType == 0 )
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK1);
					else if( monsterattackwithpos->ucAtkType == 1 )
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK2);
					else if( monsterattackwithpos->ucAtkType == 2 )					
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK3);
					else if(monsterattackwithpos->ucAtkType == 3)
						pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RATTACK);
						
					pMob->m_AttackTargetObjectType = _XPICKINGTARGET_SELF;
					pMob->m_AttackTarget = g_pLocalUser;
					pMob->m_AttackHitValue = g_pLocalUser->m_CharacterInfo.Get_current_lifepower() - monsterattackwithpos->sDamage;
					pMob->m_AttackInfo = monsterattackwithpos->ucAtkInfo;

					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					
					// 위치 보정
					pMob->m_EndPosition.x = monsterattackwithpos->fItemDropPosX;
					pMob->m_EndPosition.y = 0.0f;
					pMob->m_EndPosition.z = monsterattackwithpos->fItemDropPosZ;

					pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, g_pLocalUser->m_Position);
					pMob->UpdatePosition();
					
					if(monsterattackwithpos->sDamage > 0)
					{
						g_pLocalUser->m_CharacterInfo.Set_current_lifepower( monsterattackwithpos->sDamage );
					}
					else if(monsterattackwithpos->sDamage <= 0)
					{
						g_pLocalUser->m_CharacterInfo.Set_current_lifepower( 0 );		

						_XDWINBATTLELOG("[NETWORK/MSGBattleAtkResFromMob] Dead");
					}
					
					// 전투 로그 기록
					g_pLocalUser->m_CharacterLog.isAttack = TRUE;
					g_pLocalUser->m_CharacterLog.attackTime = g_LocalSystemTime;
					g_pLocalUser->m_CharacterLog.attackTargetType = pMob->m_MobType;

					_XDWINBATTLELOG("[NETWORK/MSGBattleAtkResFromMob] Mob[%d] Attack", pMob->m_MobType);
					
					if( g_pJinStatus_Window )
					{
						g_pJinStatus_Window->SetParameter();
					}		
					
					// 노점을 열었을 때 몬스터에게 공격당하면 노점을 닫는다.
					if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY || g_pLocalUser->m_PersonalStoreState ==_XUSER_PERSONALSTORE_OWNER_OPEN ) 
					{
						_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
						if( pPersonalStore_Window )
						{
							g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_stop_req,pPersonalStore_Window->m_ServerPersonalStoreID , 0);
							//g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
						}
					}
					else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE )
					{
						// 들고 있던 무기 복구
						g_pLocalUser->ReloadWeaponModel();
						
						g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
						g_pLocalUser->SetAnimation(_XACTION_IDLE );
						g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 

						_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
						if( pPersonalStore_Window )
						{
							g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
							g_pLocalUser->SetAnimation(_XACTION_IDLE );
							
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
											g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + pPersonalStore_Window->m_TradeItem[i].Get_m_ucCount());
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
					else if( g_pLocalUser->m_bInternalUse )		// 영약을 복용 중
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, pChargeGauge_Window->GetSelectedNumber(), 1);
						
						if( pChargeGauge_Window )
						{
							if(pChargeGauge_Window->GetShowStatus())
							{								
								pChargeGauge_Window->SetGaugeStop();
							}
						}
					}
					else if( g_pLocalUser->m_bReadingBook )	// 비급을 읽는 중
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						
						if( g_pInventory_Window )
							g_pInventory_Window->m_BookInvenNumber = -1;
						if( g_pLocalUser )
							g_pLocalUser->m_CurrentReadBookState = _XUSER_STATE_5;
						
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1611), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );	

						if( pChargeGauge_Window )
						{
							if(pChargeGauge_Window->GetShowStatus())
							{								
								pChargeGauge_Window->SetGaugeStop();
							}
						}						
					}

					// 자리 비움 상태 처리
					if(g_pLocalUser->m_bOut)
						g_pLocalUser->SetOut(FALSE);
				}
				else
				{
					// 다른 유저를 공격
					_XDWINBATTLELOG("[NETWORK/MSGBattleArkResFromMob] Monster Attackto OtherUser [%d/%d]", monsterattackwithpos->usAttackerID, monsterattackwithpos->usDefenderID);

					_XUser* pUser = (_XUser*)g_Userlist.FindUser(monsterattackwithpos->usDefenderID);
					if(pUser)
					{
						if( monsterattackwithpos->ucAtkType == 0 )
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK1);
						else if( monsterattackwithpos->ucAtkType == 1 )
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK2);
						else if( monsterattackwithpos->ucAtkType == 2 )
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK3);
						else if(monsterattackwithpos->ucAtkType == 3)
							pMob->SetAnimationIndex(_XMOBACTIONNUMBER_RATTACK);
						
						pMob->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
						pMob->m_AttackTarget = pUser;
						pMob->m_AttackHitValue = pUser->m_CharacterInfo.Get_current_lifepower() - monsterattackwithpos->sDamage;
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
						pMob->m_AttackInfo = monsterattackwithpos->ucAtkInfo;

						pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

						// 위치 보정
						pMob->m_EndPosition.x = monsterattackwithpos->fItemDropPosX;
						pMob->m_EndPosition.y = 0.0f;
						pMob->m_EndPosition.z = monsterattackwithpos->fItemDropPosZ;
						
						pMob->m_RotateAngle = _XMeshMath_GetRotateRadian(pMob->m_Position, pUser->m_Position);
						pMob->UpdatePosition();
						
						if(monsterattackwithpos->sDamage <= 0)
						{
							_XDWINBATTLELOG("monster attack other user life : <= 0");
						}
					}
					else
					{
						g_LastRequestCharacterNameID = monsterattackwithpos->usDefenderID;
						SendPacket(MSG_NO_ZONE_REQ_CHARAC_INFO);
					}
				}
			}
			break;
		}
		

		// skill 처리
		if(monsterattackwithpos->cPassiveTitle > 0)
		{
			if(monsterattackwithpos->cPassiveTitle == _XMOB_SKILL_SUICIDE)
			{
				// 자살
				pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE1;
				pMob->SetAnimation(_XMOBACTION_DIE);
			}
			else if(monsterattackwithpos->cPassiveTitle == g_MobAttribTable[pMob->m_MobType].skill1_id)
			{
				pMob->SetAnimationIndex(_XMOBACTIONNUMBER_SKILL1);
				
				if(monsterattackwithpos->cPassiveTitle == _XMOB_SKILL_REBIRTH)
				{
					// 부활
					if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						if(((_XMob*)(g_pLocalUser->m_AttackTarget))->m_MobID == pMob->m_MobID)
						{
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetSkillAnimation();
//							g_pLocalUser->SetAnimation(_XACTION_IDLE);
						}
					}
				}
			}
			else if(monsterattackwithpos->cPassiveTitle == g_MobAttribTable[pMob->m_MobType].skill2_id)
			{
				pMob->SetAnimationIndex(_XMOBACTIONNUMBER_SKILL2);

				if(monsterattackwithpos->cPassiveTitle == _XMOB_SKILL_REBIRTH)
				{
					// 부활
					if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						if(((_XMob*)(g_pLocalUser->m_AttackTarget))->m_MobID == pMob->m_MobID)
						{
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetSkillAnimation();
//							g_pLocalUser->SetAnimation(_XACTION_IDLE);
						}
					}
				}
			}
		}
	}
	else
	{	
		// Functional Object가 선공형 Trap LPS일때 공격 처리를 해야 한다.
		// 1. Trap LPS의 Animation을 처리한다.
		// 2. 공격대상에 Damage처리를 한다.
		// 3. 공격대상의 이펙트를 처리한다.

		_XDWINPRINT("Functional 공격 - UID : %d", monsterattackwithpos->usAttackerID);

		_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(monsterattackwithpos->usAttackerID);
		if(pFunctionalObject)
		{
			switch(monsterattackwithpos->ucToType)
			{
			case 0 :	// monster
				{
					pFunctionalObject->m_DefenderType = 0;
					pFunctionalObject->m_DefenderID = monsterattackwithpos->usDefenderID;

					_XDWINBATTLELOG("[NETWORK/MSGBattleArkResFromMob] Monster Attackto Monster [%d/%d]", monsterattackwithpos->usAttackerID, monsterattackwithpos->usDefenderID);
					
					_XMob* pTarget = (_XMob*)g_Moblist.FindMob(monsterattackwithpos->usDefenderID);
					if(pTarget)
					{					
						// 1. Trap LPS의 Animation을 처리한다.
						if(monsterattackwithpos->ucAtkType == 0)
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK1);
						else if(monsterattackwithpos->ucAtkType == 1)
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK2);
						else if(monsterattackwithpos->ucAtkType == 2)
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK3);
						else if(monsterattackwithpos->ucAtkType == 3)
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_RATTACK);
						
						pFunctionalObject->m_AttackTargetObjectType = 3;//_XPICKINGTARGET_MOB;
						pFunctionalObject->m_AttackTarget = pTarget;
						pFunctionalObject->m_AttackHitValue = monsterattackwithpos->sDamage;
						pFunctionalObject->m_AttackInfo = monsterattackwithpos->ucAtkInfo;
						
						pTarget->m_LastRecivedMessageTime = g_LocalSystemTime;

						// 2. 공격대상에 Damage처리를 한다.
						if(monsterattackwithpos->sDamage > 0)
						{
							// Damage 적용 - 필요한가?
						}
						else
						{
							_XDWINBATTLELOG("[NETWORK/MSGBattleAtkResFromMOb] TargetType : monster / Target Dead");

							pTarget->m_HealthPercentage = 0;
							pTarget->m_DisplayHealthPercentage = pTarget->m_HealthPercentage;		// Monster Health 적용(Display용)
							pTarget->m_DieSignalReceiveTime = g_LocalSystemTime;
						}					
					}
				}
				break;
			case 1 :	// PC
				{
					// Local user를 공격					
					pFunctionalObject->m_DefenderType = 1;
					pFunctionalObject->m_DefenderID = monsterattackwithpos->usDefenderID;

					if(monsterattackwithpos->usDefenderID == g_pLocalUser->m_Uniqueid)
					{	
						_XDWINBATTLELOG("[NETWORK/MSGBattleArkResFromMob] Monster Attackto localUSer [%d]", monsterattackwithpos->usAttackerID);
						
						if(!pFunctionalObject->m_bAttackable)
							pFunctionalObject->m_bAttackable = TRUE;
						
						// 1. Trap LPS의 Animation을 처리한다.
						if( monsterattackwithpos->ucAtkType == 0 )
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK1);
						else if( monsterattackwithpos->ucAtkType == 1 )
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK2);
						else if( monsterattackwithpos->ucAtkType == 2 )
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK3);
						else if(monsterattackwithpos->ucAtkType == 3)
							pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_RATTACK);
						
						pFunctionalObject->m_AttackTargetObjectType = 2;//_XPICKINGTARGET_SELF;
						pFunctionalObject->m_AttackTarget = g_pLocalUser;
						pFunctionalObject->m_AttackHitValue = g_pLocalUser->m_CharacterInfo.Get_current_lifepower() - monsterattackwithpos->sDamage;
						pFunctionalObject->m_AttackInfo = monsterattackwithpos->ucAtkInfo;
													
						// 2. 공격대상에 Damage처리를 한다.
						if(monsterattackwithpos->sDamage > 0)
						{
							g_pLocalUser->m_CharacterInfo.Set_current_lifepower( monsterattackwithpos->sDamage );
						}
						else if(monsterattackwithpos->sDamage <= 0)
						{
							g_pLocalUser->m_CharacterInfo.Set_current_lifepower( 0 );		

							_XDWINBATTLELOG("[NETWORK/MSGBattleAtkResFromMob] Dead");
						}
						
						// 전투 로그 기록
						g_pLocalUser->m_CharacterLog.isAttack = TRUE;
						g_pLocalUser->m_CharacterLog.attackTime = g_LocalSystemTime;
						g_pLocalUser->m_CharacterLog.attackTargetType = pFunctionalObject->m_ObjectType;

						_XDWINBATTLELOG("[NETWORK/MSGBattleAtkResFromMob] Mob[%d] Attack", pFunctionalObject->m_ObjectType);
						
						if( g_pJinStatus_Window )
						{
							g_pJinStatus_Window->SetParameter();	
						}		
						
						// 노점을 열었을 때 몬스터에게 공격당하면 노점을 닫는다.
						if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY || g_pLocalUser->m_PersonalStoreState ==_XUSER_PERSONALSTORE_OWNER_OPEN ) 
						{
							_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
							if( pPersonalStore_Window )
							{
								g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_stop_req,pPersonalStore_Window->m_ServerPersonalStoreID , 0);
								//g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
							}
						}
						else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE )
						{
							// 들고 있던 무기 복구
							g_pLocalUser->ReloadWeaponModel();
							
							g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
							g_pLocalUser->SetAnimation(_XACTION_IDLE );
							g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 

							_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
							if( pPersonalStore_Window )
							{
								g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
								g_pLocalUser->SetAnimation(_XACTION_IDLE );
								
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
						else if( g_pLocalUser->m_bInternalUse )		// 영약을 복용 중
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, pChargeGauge_Window->GetSelectedNumber(), 1);
							
							if( pChargeGauge_Window )
							{
								if(pChargeGauge_Window->GetShowStatus())
								{								
									pChargeGauge_Window->SetGaugeStop();
								}
							}
						}
						else if( g_pLocalUser->m_bReadingBook )	// 비급을 읽는 중
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							
							if( g_pInventory_Window )
								g_pInventory_Window->m_BookInvenNumber = -1;
							if( g_pLocalUser )
								g_pLocalUser->m_CurrentReadBookState = _XUSER_STATE_5;
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1611), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );	

							if( pChargeGauge_Window )
							{
								if(pChargeGauge_Window->GetShowStatus())
								{								
									pChargeGauge_Window->SetGaugeStop();
								}
							}						
						}

						// 자리 비움 상태 처리
						if(g_pLocalUser->m_bOut)
							g_pLocalUser->SetOut(FALSE);
					}
					else
					{
						// 다른 유저를 공격
						_XDWINBATTLELOG("[NETWORK/MSGBattleArkResFromMob] Monster Attackto OtherUser [%d/%d]", monsterattackwithpos->usAttackerID, monsterattackwithpos->usDefenderID);

						_XUser* pUser = (_XUser*)g_Userlist.FindUser(monsterattackwithpos->usDefenderID);
						if(pUser)
						{
							if(!pFunctionalObject->m_bAttackable)
								pFunctionalObject->m_bAttackable = TRUE;

							// 1. Trap LPS의 Animation을 처리한다.
							if( monsterattackwithpos->ucAtkType == 0 )
								pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK1);
							else if( monsterattackwithpos->ucAtkType == 1 )
								pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK2);
							else if( monsterattackwithpos->ucAtkType == 2 )
								pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_ATTACK3);
							else if(monsterattackwithpos->ucAtkType == 3)
								pFunctionalObject->SetAnimationIndex(_XMOBACTIONNUMBER_RATTACK);
							
							pFunctionalObject->m_AttackTargetObjectType = 1;//_XPICKINGTARGET_PC;
							pFunctionalObject->m_AttackTarget = pUser;
							pFunctionalObject->m_AttackHitValue = pUser->m_CharacterInfo.Get_current_lifepower() - monsterattackwithpos->sDamage;
							pFunctionalObject->m_AttackInfo = monsterattackwithpos->ucAtkInfo; 

							pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

							// 2. 공격대상에 Damage처리를 한다.
							if(monsterattackwithpos->sDamage <= 0)
							{
								_XDWINBATTLELOG("monster attack other user life : <= 0");
							}
						}
						else
						{
							g_LastRequestCharacterNameID = monsterattackwithpos->usDefenderID;
							SendPacket(MSG_NO_ZONE_REQ_CHARAC_INFO);
						}
					}
				}
				break;
			}
		
		}
	
	}
}

///////////////////////////////////////////////////////////////////////////////
// New Packet
///////////////////////////////////////////////////////////////////////////////

BOOL _XNetwork::Send_MSGMartialReady(int type, int param1, int param2, int param3)
{
	switch(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Charge Skill Ready");

			int size = sizeof(MSG_CHARGE_READY);
			MSG_CHARGE_READY chargeready;
			ZeroMemory(&chargeready, size);
			chargeready.header.sLength = size;
			chargeready.ucMessage = type;

			chargeready.ucMartialType = _XSI_USEFULTYPE_CHARGESKILL;
			switch(g_pLocalUser->m_AttackTargetObjectType)
			{
			case _XPICKINGTARGET_MOB :
				{
					_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
					if(pMob)
						chargeready.usTargetID = pMob->m_MobID;
					chargeready.ucTargetType = (unsigned char)en_target_type_monster;
					g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT :
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
						chargeready.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
					chargeready.ucTargetType = (unsigned char)en_target_type_object;
					g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
				}
				break;
			case _XPICKINGTARGET_NPC :
				break;
			case _XPICKINGTARGET_PC :
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
					if(pUser)
						chargeready.usTargetID = pUser->m_Uniqueid;
					chargeready.ucTargetType = (unsigned char)en_target_type_others;
					g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
				}
				break;
			}

			chargeready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);

			return _XSendPacket_Encapsulate( (char*)&chargeready );			
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Channeling spell Ready");
			
			if(param1 == 0)
			{
				// other - friend, monster etc..
				int size = sizeof(MSG_CHANNEL_READY_PC);
				MSG_CHANNEL_READY_PC channelready;
				ZeroMemory(&channelready, size);
				channelready.header.sLength = size;
				channelready.ucMessage = type;
				
				channelready.ucMartialType = _XSI_USEFULTYPE_CHANNELINGSPELL;
				channelready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				
				if(g_pLocalUser->m_SelectedSkillItem)
					channelready.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					channelready.cSpellStep = 0;

				switch(g_pLocalUser->m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
						channelready.ucTargetType = (unsigned char)en_target_type_monster;
						if(pMob)
							channelready.usTargetID = pMob->m_MobID;
						g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
						channelready.ucTargetType = (unsigned char)en_target_type_object;
						if(pFunctionalObject)
							channelready.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
						g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
					}
					break;
				case _XPICKINGTARGET_NPC :
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
						channelready.ucTargetType = (unsigned char)en_target_type_others;
						if(pUser)
							channelready.usTargetID = pUser->m_Uniqueid;
						g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
					}
					break;
				}

				return _XSendPacket_Encapsulate( (char*)&channelready );
			}
			else if(param1 == 1)			
			{
				// self
				int size = sizeof(MSG_CHANNEL_READY_SELF);
				MSG_CHANNEL_READY_SELF channelready;
				ZeroMemory(&channelready, size);
				channelready.header.sLength = size;
				channelready.ucMessage = type;
				
				channelready.ucMartialType = _XSI_USEFULTYPE_CHANNELINGSPELL;
				channelready.ucTargetType = (unsigned char)en_target_type_self;
				channelready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				
				if(g_pLocalUser->m_SelectedSkillItem)
					channelready.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					channelready.cSpellStep = 0;
				
				g_pLocalUser->SetSpellAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
				
				return _XSendPacket_Encapsulate( (char*)&channelready );
			}
			else if(param1 == 2)
			{
				// Area
				int size = sizeof(MSG_CHANNEL_READY_POINT);
				MSG_CHANNEL_READY_POINT channelready;
				ZeroMemory(&channelready, size);
				channelready.header.sLength = size;
				channelready.ucMessage = type;
				
				channelready.ucMartialType = _XSI_USEFULTYPE_CHANNELINGSPELL;
				channelready.ucTargetType = (unsigned char)en_target_type_point;
				channelready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				
				if(g_pLocalUser->m_SelectedSkillItem)
					channelready.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					channelready.cSpellStep = 0;

				channelready.fTargetPosX = g_pLocalUser->m_RegionTargetPosX;
				channelready.fTargetPosZ = g_pLocalUser->m_RegionTargetPosZ;

				return _XSendPacket_Encapsulate( (char*)&channelready );
			}
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Finish skill Ready");

			int size = sizeof(MSG_FINISH_READY);
			MSG_FINISH_READY finishready;
			ZeroMemory(&finishready, size);
			finishready.header.sLength = size;
			finishready.ucMessage = type;

			finishready.ucMartialType = _XSI_USEFULTYPE_FINISHSKILL;

			switch(g_pLocalUser->m_AttackTargetObjectType)
			{
			case _XPICKINGTARGET_MOB :
				{
					_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
					finishready.ucTargetType = (unsigned char)en_target_type_monster;
					if(pMob)
						finishready.usTargetID = pMob->m_MobID;
					g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT :
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					finishready.ucTargetType = (unsigned char)en_target_type_object;
					if(pFunctionalObject)
						finishready.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
					g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
					
				}
				break;
			case _XPICKINGTARGET_NPC :
				break;
			case _XPICKINGTARGET_PC :
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
					finishready.ucTargetType = (unsigned char)en_target_type_others;
					if(pUser)
						finishready.usTargetID = pUser->m_Uniqueid;
					g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
				}
				break;
			}

			finishready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);

			if(g_pLocalUser->m_SelectedSkillItem)
				finishready.cSkillStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
			else
				finishready.cSkillStep = 0;

			return _XSendPacket_Encapsulate( (char*)&finishready );
		}
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Casting spell Ready");

			if(param1 == 0)
			{
				// Other or all
				int size = sizeof(MSG_CASTING_READY_TO_PC);
				MSG_CASTING_READY_TO_PC castingready;
				ZeroMemory(&castingready, size);
				castingready.header.sLength = size;
				castingready.ucMessage = type;
				
				castingready.ucMartialType = _XSI_USEFULTYPE_CASTINGSPELL;

				switch(g_pLocalUser->m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
						castingready.ucTargetType = (unsigned char)en_target_type_monster;
						if(pMob)
							castingready.usTargetID = pMob->m_MobID;
						g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
						castingready.ucTargetType = (unsigned char)en_target_type_object;
						if(pFunctionalObject)
							castingready.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
						g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);	
					}
					break;
				case _XPICKINGTARGET_NPC :
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
						castingready.ucTargetType = (unsigned char)en_target_type_others;
						if(pUser)
							castingready.usTargetID = pUser->m_Uniqueid;
						g_pLocalUser->SetSpellAttackTarget(g_pLocalUser->m_AttackTarget, g_pLocalUser->m_AttackTargetObjectType);	
					}
					break;
				}

				castingready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				
				if(g_pLocalUser->m_SelectedSkillItem)
					castingready.cStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					castingready.cStep = 0;
				
				return _XSendPacket_Encapsulate( (char*)&castingready );
			}
			else if(param1 == 1)
			{
				// self
				int size = sizeof(MSG_CASTING_READY_TO_PC);
				MSG_CASTING_READY_TO_PC castingready;
				ZeroMemory(&castingready, size);
				castingready.header.sLength = size;
				castingready.ucMessage = type;
				
				castingready.ucMartialType = _XSI_USEFULTYPE_CASTINGSPELL;
				castingready.ucTargetType = (unsigned char)en_target_type_self;
				castingready.usTargetID = g_pLocalUser->m_Uniqueid;
				g_pLocalUser->SetSpellAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);

				castingready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);			
				if(g_pLocalUser->m_SelectedSkillItem)
					castingready.cStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					castingready.cStep = 0;
				
				return _XSendPacket_Encapsulate( (char*)&castingready );
			}
			else if(param1 == 2)
			{
				// Area
				int size = sizeof(MSG_CASTING_READY_TO_POINT);
				MSG_CASTING_READY_TO_POINT castingready;
				ZeroMemory(&castingready, size);
				castingready.header.sLength = size;
				castingready.ucMessage = type;
				
				castingready.ucMartialType = _XSI_USEFULTYPE_CASTINGSPELL;
				castingready.ucTargetType = (unsigned char)en_target_type_point;
				castingready.fTargetPosX = g_pLocalUser->m_TargetPosition.x;
				castingready.fTargetPosZ = g_pLocalUser->m_TargetPosition.z;
				castingready.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				
				if(g_pLocalUser->m_SelectedSkillItem)
					castingready.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					castingready.cSpellStep = 0;
				
				return _XSendPacket_Encapsulate( (char*)&castingready );
			}
		}
		break;
	}

	return TRUE;
}

BOOL _XNetwork::Send_MSGMartialFire(int type, int param1, int param2, int param3)
{
	if(g_pQuickSlot_Window->GetChangingItem()) //Author :양희왕 //breif : 현재 아이템을 교체중 공격 패킷을 보내지 않는다
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//지금은 사용할 수 없습니다.
		return TRUE;
	}
		
	unsigned char usefultype = 0;
	if(g_pLocalUser->m_SelectedSkillID == 0)
		usefultype = _XSI_USEFULTYPE_ACTIVESKILL;
	else
		usefultype = g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType;

	//_XLog("Send attack packet [%d]", usefultype );
	
	switch(usefultype)
	{
	case _XSI_USEFULTYPE_ACTIVESKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("====== Active Skill : Sequence[%d]", param1);
			
			int size = sizeof(MSG_ACTIVE_FIRE);
			MSG_ACTIVE_FIRE activefire;
			ZeroMemory(&activefire, size);
			activefire.header.sLength = size;
			activefire.ucMessage = type;
						
			activefire.ucMartialType = usefultype;

			switch(g_pLocalUser->m_AttackTargetObjectType)
			{
			case _XPICKINGTARGET_MOB :
				{
					_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
					activefire.ucTargetType = (unsigned char)en_target_type_monster;
					if(pMob)
						activefire.usTargetID = pMob->m_MobID;
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT :
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					activefire.ucTargetType = (unsigned char)en_target_type_object;
					if(pFunctionalObject)
						activefire.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
				}
				break;
			case _XPICKINGTARGET_NPC :
				break;
			case _XPICKINGTARGET_PC :
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
					activefire.ucTargetType = (unsigned char)en_target_type_others;
					if(pUser)
						activefire.usTargetID = pUser->m_Uniqueid;
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				break;
			}

			if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
			{
				if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_DUMMY)
				{
					_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
					if(pDummy_Window)
					{
						activefire.usTargetID = pDummy_Window->m_pDummy->m_DummyID;
						activefire.ucTargetType = (unsigned char)en_target_type_dummy;
					}
				}
			}

			activefire.ucSequence = (unsigned char)param1;
			activefire.sAnimation = (short)g_pLocalUser->m_ModelDescriptor.GetCurMotion();

			return _XSendPacket_Encapsulate( (char*)&activefire );
		}
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Charge Skill Fire");

			int size = sizeof(MSG_CHARGE_FIRE);
			MSG_CHARGE_FIRE chargefire;
			ZeroMemory(&chargefire, size);
			chargefire.header.sLength = size;
			chargefire.ucMessage = type;
			
			chargefire.ucMartialType = usefultype;
			switch(g_pLocalUser->m_AttackTargetObjectType)
			{
			case _XPICKINGTARGET_MOB :
				{
					chargefire.ucTargetType = (unsigned char)en_target_type_monster;
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT :
				{
					chargefire.ucTargetType = (unsigned char)en_target_type_object;										
				}
				break;
			case _XPICKINGTARGET_NPC :
				break;
			case _XPICKINGTARGET_PC :
				{
					chargefire.ucTargetType = (unsigned char)en_target_type_others;
				}
				break;
			}
			chargefire.sChargeTime = (short)param1;

			return _XSendPacket_Encapsulate( (char*)&chargefire );
		}
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Rounding Spell Fire");

			if(param1 == 0)
			{
				// other or all
				int size = sizeof(MSG_ROUNDING_FIRE);
				MSG_ROUNDING_FIRE roundingfire;
				ZeroMemory(&roundingfire, size);
				roundingfire.header.sLength = size;
				roundingfire.ucMessage = type;
								
				roundingfire.ucMartialType = usefultype;

				switch(g_pLocalUser->m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
						if(pMob)
							roundingfire.usTargetID = pMob->m_MobID;
						roundingfire.ucTargetType = (unsigned char)en_target_type_monster;
					}
					break;
				case _XPICKINGTARGET_NPC :
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
						if(pUser)
							roundingfire.usTargetID = pUser->m_Uniqueid;
						roundingfire.ucTargetType = (unsigned char)en_target_type_others;
					}
					break;
				case _XPICKINGTARGET_SELF :
					{
						roundingfire.usTargetID = g_pLocalUser->m_Uniqueid;
						roundingfire.ucTargetType = (unsigned char)en_target_type_self;
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT:
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
						if(pFunctionalObject)
							roundingfire.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
						
						roundingfire.ucTargetType = (unsigned char)en_target_type_object;
					}
					break;
				}

				roundingfire.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);				
				if(g_pLocalUser->m_SelectedSkillItem)
					roundingfire.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					roundingfire.cSpellStep = 0;
				
				return _XSendPacket_Encapsulate( (char*)&roundingfire );
			}
			else if(param1 == 1)
			{
				// Self
				int size = sizeof(MSG_ROUNDING_FIRE);
				MSG_ROUNDING_FIRE roundingfire;
				ZeroMemory(&roundingfire, size);
				roundingfire.header.sLength = size;
				roundingfire.ucMessage = type;
								
				roundingfire.ucMartialType = usefultype;
				roundingfire.usTargetID = g_pLocalUser->m_Uniqueid;
				roundingfire.ucTargetType = (unsigned char)en_target_type_self;
				roundingfire.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				if(g_pLocalUser->m_SelectedSkillItem)
					roundingfire.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					roundingfire.cSpellStep = 0;
				
				return _XSendPacket_Encapsulate( (char*)&roundingfire );
			}
			else if(param1 == 2)
			{
				// Area
				int size = sizeof(MSG_ROUNDING_FIRE_TO_POINT);
				MSG_ROUNDING_FIRE_TO_POINT roundingfire;
				ZeroMemory(&roundingfire, size);
				roundingfire.header.sLength = size;
				roundingfire.ucMessage = type;
				
				roundingfire.ucMartialType = usefultype;
				roundingfire.ucTargetType = (unsigned char)en_target_type_point;

				roundingfire.fTargetX = g_pLocalUser->m_RegionTargetPosX;
				roundingfire.fTargetZ = g_pLocalUser->m_RegionTargetPosZ;

				roundingfire.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				if(g_pLocalUser->m_SelectedSkillItem)
					roundingfire.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
				else
					roundingfire.cSpellStep = 0;
				
				return _XSendPacket_Encapsulate((char*)&roundingfire);
			}
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Channeling Spell Fire");

			int size = sizeof(MSG_CHANNEL_FIRE);
			MSG_CHANNEL_FIRE channelfire;
			ZeroMemory(&channelfire, size);
			channelfire.header.sLength = size;
			channelfire.ucMessage = type;
			
			channelfire.ucMartialType = usefultype;

			if(param1 == 0)
			{
				switch(g_pLocalUser->m_SpellAttackTargetType)
				{
				case _XPICKINGTARGET_MOB :
					channelfire.ucTargetType = (unsigned char)en_target_type_monster;
					break;
				case _XPICKINGTARGET_NPC :
					break;
				case _XPICKINGTARGET_PC :
					channelfire.ucTargetType = (unsigned char)en_target_type_others;
					break;
				case _XPICKINGTARGET_SELF :
					channelfire.ucTargetType = (unsigned char)en_target_type_self;
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					channelfire.ucTargetType = (unsigned char)en_target_type_object;
					break;
				}
			}
			else if(param1 == 1)
			{
				channelfire.ucTargetType = (unsigned char)en_target_type_self;
			}
			else if(param1 == 2)
			{
				channelfire.ucTargetType = (unsigned char)en_target_type_point;
			}

			return _XSendPacket_Encapsulate( (char*)&channelfire );
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Finish Skill Fire");

			int size = sizeof(MSG_FINISH_FIRE);
			MSG_FINISH_FIRE finishfire;
			ZeroMemory(&finishfire, size);
			finishfire.header.sLength = size;
			finishfire.ucMessage = type;
			
			finishfire.ucMartialType = usefultype;
			switch(g_pLocalUser->m_AttackTargetObjectType)
			{
			case _XPICKINGTARGET_MOB :
				{
					finishfire.ucTargetType = (unsigned char)en_target_type_monster;
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT :
				{
					finishfire.ucTargetType = (unsigned char)en_target_type_object;										
				}
				break;
			case _XPICKINGTARGET_NPC :
				break;
			case _XPICKINGTARGET_PC :
				{
					finishfire.ucTargetType = (unsigned char)en_target_type_others;
				}
				break;
			}
			
			return _XSendPacket_Encapsulate( (char*)&finishfire );
		}
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Casting Spell Fire");

			int size = sizeof(MSG_CASTING_FIRE);
			MSG_CASTING_FIRE castingfire;
			ZeroMemory(&castingfire, size);
			castingfire.header.sLength = size;
			castingfire.ucMessage = type;
			
			castingfire.ucMartialType = usefultype;

			if(param1 == 0)
			{
				// other or all
				switch(g_pLocalUser->m_SpellAttackTargetType)
				{
				case _XPICKINGTARGET_MOB :
					castingfire.ucTargetType = (unsigned char)en_target_type_monster;
					break;
				case _XPICKINGTARGET_NPC :
					break;
				case _XPICKINGTARGET_PC :
					castingfire.ucTargetType = (unsigned char)en_target_type_others;
					break;
				case _XPICKINGTARGET_SELF :
					castingfire.ucTargetType = (unsigned char)en_target_type_self;
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT:
					castingfire.ucTargetType = (unsigned char)en_target_type_object;
					break;
				}
			}
			else if(param1 == 1)
			{
				// Self
				castingfire.ucTargetType = (unsigned char)en_target_type_self;
			}
			else if(param1 == 2)
			{
				// Area
				castingfire.ucTargetType = (unsigned char)en_target_type_point;
			}

			return _XSendPacket_Encapsulate( (char*)&castingfire );
		}
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Smashing Skill Fire");
			
			int size = sizeof(MSG_SMASHING_FIRE);
			MSG_SMASHING_FIRE smashingfire;
			ZeroMemory(&smashingfire, size);
			smashingfire.header.sLength = size;
			smashingfire.ucMessage = type;
						
			smashingfire.ucMartialType = usefultype;
			switch(g_pLocalUser->m_AttackTargetObjectType)
			{
			case _XPICKINGTARGET_MOB :
				{
					_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
					if(pMob)
						smashingfire.usTargetID = pMob->m_MobID;
					smashingfire.ucTargetType = (unsigned char)en_target_type_monster;
				}
				break;
			case _XPICKINGTARGET_NPC :
				break;
			case _XPICKINGTARGET_PC :
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
					if(pUser)
						smashingfire.usTargetID = pUser->m_Uniqueid;
					smashingfire.ucTargetType = (unsigned char)en_target_type_others;
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					_XDummy* pDummy = (_XDummy*)g_pLocalUser->m_AttackTarget;
					if(pDummy)
						smashingfire.usTargetID = pDummy->m_DummyID;
					
					smashingfire.ucTargetType = (unsigned char)en_target_type_dummy;

					VIEWPACKETLOG
						_XDWPACKETLOG("=== Smashing Skill Fire : Targettype dummy [id :%d]", smashingfire.usTargetID);
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
						smashingfire.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
					
					smashingfire.ucTargetType = (unsigned char)en_target_type_object;
				}
				break;
			}
			
			smashingfire.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
			smashingfire.cSpellStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
		
			return _XSendPacket_Encapsulate( (char*)&smashingfire );			
		}
		break;
	case _XSI_USEFULTYPE_ACTIONSKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("=== Action Skill Fire");
			
			int size = sizeof(MSG_ACTION_FIRE);
			MSG_ACTION_FIRE actionfire;
			ZeroMemory(&actionfire, size);
			actionfire.header.sLength = size;
			actionfire.ucMessage = type;
						
			actionfire.ucMartialType = usefultype;
			switch(g_pLocalUser->m_AttackTargetObjectType)
			{
			case _XPICKINGTARGET_MOB :
				{
					_XMob* pMob = (_XMob*)g_pLocalUser->m_AttackTarget;
					if(pMob)
						actionfire.usTargetID = pMob->m_MobID;
					actionfire.ucTargetType = (unsigned char)en_target_type_monster;
				}
				break;
			case _XPICKINGTARGET_NPC :
				break;
			case _XPICKINGTARGET_PC :
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
					if(pUser)
						actionfire.usTargetID = pUser->m_Uniqueid;
					actionfire.ucTargetType = (unsigned char)en_target_type_others;
				}
				break;
			case _XPICKINGTARGET_SELF :
				{
					actionfire.ucTargetType = (unsigned char)en_target_type_self;
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					_XDummy* pDummy = (_XDummy*)g_pLocalUser->m_AttackTarget;
					if(pDummy)
						actionfire.usTargetID = pDummy->m_DummyID;
					
					actionfire.ucTargetType = (unsigned char)en_target_type_dummy;
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
						actionfire.usTargetID = pFunctionalObject->m_AttackalbeTypeUniqueID;
					
					actionfire.ucTargetType = (unsigned char)en_target_type_object;
				}
				break;
			}
			
			actionfire.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
			actionfire.cSkillStep = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
			
			return _XSendPacket_Encapsulate( (char*)&actionfire );			
		}
		break;
	}
	
	return TRUE;
}

BOOL _XNetwork::Send_MSGMartialEnd(int type, int param1, int param2, int param3)
{
	switch(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType)
	{
	case _XSI_USEFULTYPE_ACTIVESKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Active skill end");
		}
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Charge skill end");
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Channeling spell end");
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Finish skill end");
		}
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Rounding spell end");
		}
		break;
	case _XSI_USEFULTYPE_HIDDENMA :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Hidden MA end");
		}
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Smashing skill end");
		}
		break;
	case _XSI_USEFULTYPE_ACTIONSKILL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("[SEND_MARTIAL_END] Action skill end");
		}
		break;
	}

	int size = sizeof(MARTIAL_END);
	MARTIAL_END martialend;
	ZeroMemory(&martialend, size);
	martialend.header.sLength = size;
	martialend.ucMessage = type;
	
	martialend.ucMartialType = g_pLocalUser->m_CurrentSelectedSkillType;
	martialend.ucTargetType = 0;
	martialend.usAttackerID = g_pLocalUser->m_Uniqueid;
	martialend.cReason = 0;

	return _XSendPacket_Encapsulate((char*)&martialend);
}

void _XNetwork::MSGMartialChargeReadyResult(MSG_CHARGE_READY_RES* chargeready)
{
	if(chargeready->usAttackerID == g_pLocalUser->m_Uniqueid)
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(chargeready->usAttackerID);
		if(pUser)
		{
			_XDWINBATTLELOG("=== Charge Ready Result Other user [%s]", pUser->m_CharacterInfo.charactername);
			_XDWINBATTLELOG("=== skillid [%d] targettype [%d] targetid [%d]", chargeready->sChargeSkillID, chargeready->ucTargetType, chargeready->usTargetID);			

			memset(&pUser->m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));

			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[chargeready->sChargeSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[chargeready->sChargeSkillID]->motion_female;

			pUser->SetAnimationIndex(motionindex);
			pUser->m_AttackResult.skillType = g_SkillProperty[chargeready->sChargeSkillID]->usefulType;
			pUser->m_AttackResult.sSkillID = chargeready->sChargeSkillID;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			switch(chargeready->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(chargeready->usTargetID);
					if(pMob)
					{
						pUser->m_AttackTarget = pMob;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;

						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pMob->m_Position);
						pUser->UpdatePosition();
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(chargeready->usTargetID);
					if(pFunctionalObject)
					{
						pUser->m_AttackTarget = (_XItem*)pFunctionalObject;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pFunctionalObject->m_Position);
						pUser->UpdatePosition();
						//pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_npc :
				break;
			case en_target_type_dummy :
				break;
			case en_target_type_others :
				{
					if(chargeready->usTargetID == g_pLocalUser->m_Uniqueid)
					{
						// Target이 local user
						pUser->m_AttackTarget = g_pLocalUser;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
						pUser->UpdatePosition();
					}
					else
					{
						_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(chargeready->usTargetID);
						if(pTargetUser)
						{
							pUser->m_AttackTarget = pTargetUser;
							pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

							pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
							pUser->UpdatePosition();
							pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;
						}
					}
				}
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialChannelReadyResultSelf(MSG_CHANNEL_READY_RES_SELF* channelreadyres)
{
	if(channelreadyres->usAttackerID == g_pLocalUser->m_Uniqueid)
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
	else
	{
		// other user
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(channelreadyres->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = channelreadyres->ucMartialType;
			pUser->m_AttackResult.sSkillID = channelreadyres->sChannelSpellID;
			pUser->m_AttackResult.cSkillLevel = channelreadyres->cSpellStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.ucToType = channelreadyres->ucTargetType;

			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[channelreadyres->sChannelSpellID]->motion_male;
			else
				motionindex = g_SkillProperty[channelreadyres->sChannelSpellID]->motion_female;
			pUser->SetAnimationIndex(motionindex);

			_XDWINBATTLELOG("=== Channeling[Self] other user [%s]  ", pUser->m_CharacterInfo.charactername);

			// 모든 무기 type이면 들고 있던 무기 삭제
			pUser->DeleteWeaponModel(pUser->m_AttackResult.sSkillID);
		} // if(pUser)
	}
}

void _XNetwork::MSGMartialChannelReadyResultPoint(MSG_CHANNEL_READY_RES_POINT* channelreadyresult)
{
	if(channelreadyresult->usAttackerID == g_pLocalUser->m_Uniqueid)
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(channelreadyresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = channelreadyresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = channelreadyresult->sChannelSpellID;
			pUser->m_AttackResult.cSkillLevel = channelreadyresult->cSpellStep;
			pUser->m_AttackResult.fX = channelreadyresult->fTargetPosX;
			pUser->m_AttackResult.fZ = channelreadyresult->fTargetPosZ;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[channelreadyresult->sChannelSpellID]->motion_male;
			else
				motionindex = g_SkillProperty[channelreadyresult->sChannelSpellID]->motion_female;
			pUser->SetAnimationIndex(motionindex);

			D3DXVECTOR3 targetposition;
			targetposition.x = channelreadyresult->fTargetPosX;
			targetposition.z = channelreadyresult->fTargetPosZ;
			targetposition.y = g_LodTerrain.GetTerrainAndRideOBBHeight(targetposition, pUser->m_pGoundCollideOBB);

			pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, targetposition);
			pUser->UpdatePosition();

			_XDWINBATTLELOG("=== Channeling[Point] other user [%s]  ", pUser->m_CharacterInfo.charactername);
			
			// 모든 무기 type이면 들고 있던 무기 삭제
			pUser->DeleteWeaponModel(pUser->m_AttackResult.sSkillID);
			

			if( pUser->m_ModelDescriptor.m_EffectManager )
			{
				D3DXMATRIX targetpointmat;
				D3DXMatrixIdentity( &targetpointmat );
				targetpointmat._41 = targetposition.x;
				targetpointmat._42 = targetposition.y;
				targetpointmat._43 = targetposition.z;
				
				//_XParticleGroupItem* pParticleGroupItem = m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, m_SelectedSkillID + 40000, &mousemat );
				_XParticleGroupItem* pParticleGroupItem = pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, channelreadyresult->sChannelSpellID + 40000, &targetpointmat );
				if(pParticleGroupItem)
				{
					D3DXVECTOR3 maxsize;
					maxsize.x = g_SkillProperty[channelreadyresult->sChannelSpellID]->radius;
					maxsize.y = g_SkillProperty[channelreadyresult->sChannelSpellID]->radius;
					maxsize.z = g_SkillProperty[channelreadyresult->sChannelSpellID]->radius;
					pParticleGroupItem->SetParticleMaxSize( maxsize );
				}
			}
		} // if(pUser)
	}
}

void _XNetwork::MSGMartialChannelReadyResultPC(MSG_CHANNEL_READY_RES_PC* channelreadyresult)
{
	if(channelreadyresult->usAttackerID == g_pLocalUser->m_Uniqueid)
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
	else
	{
		_XUser* pUser = g_Userlist.FindUser(channelreadyresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = channelreadyresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = channelreadyresult->sChannelSpellID;
			pUser->m_AttackResult.cSkillLevel = channelreadyresult->cSpellStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.ucToType = channelreadyresult->ucTargetType;

			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[channelreadyresult->sChannelSpellID]->motion_male;
			else
				motionindex = g_SkillProperty[channelreadyresult->sChannelSpellID]->motion_female;
			pUser->SetAnimationIndex(motionindex);
			
			_XDWINBATTLELOG("=== Channel(PC) other user [%s]  targettype [%d]  targetid [%d]", pUser->m_CharacterInfo.charactername, channelreadyresult->ucTargetType, channelreadyresult->usTargetID);

			switch(channelreadyresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(channelreadyresult->usTargetID);
					if(pMob)
					{
						pUser->m_AttackTarget = pMob;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pMob->m_Position);
						pUser->UpdatePosition();
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(channelreadyresult->usTargetID);
					if(pFunctionalObject)
					{
						pUser->m_AttackTarget = (_XItem*)pFunctionalObject;;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pFunctionalObject->m_Position);
						pUser->UpdatePosition();
					}
				}
				break;
			case en_target_type_npc :
				break;
			case en_target_type_dummy :
				break;
			case en_target_type_others :
				{
					if(channelreadyresult->usTargetID == g_pLocalUser->m_Uniqueid)
					{
						// target이 localuser
						pUser->m_AttackTarget = g_pLocalUser;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
						pUser->UpdatePosition();
					}
					else
					{
						_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(channelreadyresult->usTargetID);
						if(pTargetUser)
						{
							pUser->m_AttackTarget = pTargetUser;
							pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
							
							pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
							pUser->UpdatePosition();
//							pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;
						}
					}
				}
				break;
			}
		} // if(pUser)
	}
}

void _XNetwork::MSGMartialFinishReadyResult(MSG_FINISH_READY_RES* finishreadyresult)
{
	if(finishreadyresult->usAttackerID == g_pLocalUser->m_Uniqueid)
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(finishreadyresult->usAttackerID);
		if(pUser)
		{
			_XDWINBATTLELOG("=== Finish skill other user [%s]  targettype [%d]  targetid [%d]", pUser->m_CharacterInfo.charactername, finishreadyresult->ucTargetType, finishreadyresult->usTargetID);

			pUser->m_AttackResult.skillType = g_SkillProperty[finishreadyresult->sFinishSkillID]->usefulType;
			pUser->m_AttackResult.sSkillID = finishreadyresult->sFinishSkillID;
			pUser->m_AttackResult.cSkillLevel = finishreadyresult->cSkillStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.ucToType = finishreadyresult->ucTargetType;

			pUser->m_bFinishDashStart = FALSE;
			pUser->m_FinishSpeed = g_SkillProperty[finishreadyresult->sFinishSkillID]->detailInfo[finishreadyresult->cSkillStep].finishSpeed;

			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[finishreadyresult->sFinishSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[finishreadyresult->sFinishSkillID]->motion_female;
			pUser->SetAnimationIndex(motionindex);

			switch(finishreadyresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(finishreadyresult->usTargetID);
					if(pMob)
					{
						pUser->m_AttackTarget = pMob;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;

						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pMob->m_Position);
						pUser->UpdatePosition();
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(finishreadyresult->usTargetID);
					if(pFunctionalObject)
					{
						pUser->m_AttackTarget = (_XItem*)pFunctionalObject;;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pFunctionalObject->m_Position);
						pUser->UpdatePosition();
						//pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_npc :
				break;
			case en_target_type_dummy :
				break;
			case en_target_type_others :
				{
					if(finishreadyresult->usTargetID == g_pLocalUser->m_Uniqueid)
					{
						// Target이 local user
						pUser->m_AttackTarget = g_pLocalUser;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
						pUser->UpdatePosition();
					}
					else
					{
						_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(finishreadyresult->usTargetID);
						if(pTargetUser)
						{
							pUser->m_AttackTarget = pTargetUser;
							pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

							pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
							pUser->UpdatePosition();
							pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;
						}
					}
				}
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialCastingReadyResultPoint(MSG_CASTING_READY_RES_TO_POINT* castingreadyresult)
{
	if(castingreadyresult->usAttackerID == g_pLocalUser->m_Uniqueid)
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(castingreadyresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = castingreadyresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = castingreadyresult->sCastingSpellID;
			pUser->m_AttackResult.cSkillLevel = castingreadyresult->cSpellStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[castingreadyresult->sCastingSpellID]->motion_male;
			else
				motionindex = g_SkillProperty[castingreadyresult->sCastingSpellID]->motion_female;
			pUser->SetAnimationIndex(motionindex);

			D3DXVECTOR3 targetposition;
			targetposition.x = castingreadyresult->fTargetPosX;
			targetposition.z = castingreadyresult->fTargetPosZ;
			targetposition.y = g_LodTerrain.GetTerrainAndRideOBBHeight(targetposition, pUser->m_pGoundCollideOBB);

			pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, targetposition);
			pUser->UpdatePosition();
		}
	}
}

void _XNetwork::MSGMartialCastingReadyResultPC(MSG_CASTING_READY_RES_TO_PC* castingreadyresult)
{
	if(castingreadyresult->usAttackerID == g_pLocalUser->m_Uniqueid)
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(castingreadyresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = castingreadyresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = castingreadyresult->sCastingSpellID;
			pUser->m_AttackResult.cSkillLevel = castingreadyresult->cSpellStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.ucToType = castingreadyresult->ucTargetType;

			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[castingreadyresult->sCastingSpellID]->motion_male;
			else
				motionindex = g_SkillProperty[castingreadyresult->sCastingSpellID]->motion_female;

			_XDWINBATTLELOG("=== Casting skill other user [%s]  targettype [%d]  targetid [%d]", pUser->m_CharacterInfo.charactername, castingreadyresult->ucTargetType, castingreadyresult->usTargetID);

			pUser->SetAnimationIndex(motionindex);

			switch(castingreadyresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(castingreadyresult->usTargetID);
					if(pMob)
					{
						pUser->m_AttackTarget = pMob;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;

						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pMob->m_Position);
						pUser->UpdatePosition();
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(castingreadyresult->usTargetID);
					if(pFunctionalObject)
					{
						pUser->m_AttackTarget = (_XItem*)pFunctionalObject;;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pFunctionalObject->m_Position);
						pUser->UpdatePosition();
						//pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_npc :
				break;
			case en_target_type_dummy :
				break;
			case en_target_type_others :
				{
					if(castingreadyresult->usTargetID == g_pLocalUser->m_Uniqueid)
					{
						// target이 localuser
						pUser->m_AttackTarget = g_pLocalUser;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
						pUser->UpdatePosition();
					}
					else
					{
						_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(castingreadyresult->usTargetID);
						if(pTargetUser)
						{
							pUser->m_AttackTarget = pTargetUser;
							pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

							pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
							pUser->UpdatePosition();
//							pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;
						}
					}
				}
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialRoundingFireResult(MSG_ROUNDING_FIRE_RES* roundingfireresult)
{
	if(roundingfireresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( roundingfireresult->sLeftForce );
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(roundingfireresult->usAttackerID);
		if(pUser)
		{
			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[roundingfireresult->sSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[roundingfireresult->sSkillID]->motion_female;

			_XDWINBATTLELOG("=== Rounding Fire Result other [%s], motionindex [%d]", pUser->m_CharacterInfo.charactername, motionindex);

			pUser->SetAnimationIndex(motionindex);
			pUser->m_AttackResult.skillType = g_SkillProperty[roundingfireresult->sSkillID]->usefulType;
			pUser->m_AttackResult.sSkillID = roundingfireresult->sSkillID;
			pUser->m_AttackResult.cSkillLevel = roundingfireresult->cSpellStep;
			pUser->m_AttackResult.ucToType = roundingfireresult->ucTargetType;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			// 모든 무기 type이면 들고 있던 무기 삭제
			pUser->DeleteWeaponModel(pUser->m_AttackResult.sSkillID);

			switch(roundingfireresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(roundingfireresult->usTargetID);
					if(pMob)
					{
						pUser->m_AttackTarget = pMob;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;

						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pMob->m_Position);
						pUser->UpdatePosition();
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(roundingfireresult->usTargetID);
					if(pFunctionalObject)
					{
						pUser->m_AttackTarget = (_XItem*)pFunctionalObject;;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pFunctionalObject->m_Position);
						pUser->UpdatePosition();
						//pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_npc :
				break;
			case en_target_type_dummy :
				break;
			case en_target_type_others :
				{
					if(roundingfireresult->usTargetID == g_pLocalUser->m_Uniqueid)
					{
						// Target이 local user
						pUser->m_AttackTarget = g_pLocalUser;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
						pUser->UpdatePosition();

						if(g_pLocalUser->m_bOut)
							g_pLocalUser->SetOut(FALSE);
					}
					else
					{
						_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(roundingfireresult->usTargetID);
						if(pTargetUser)
						{
							pUser->m_AttackTarget = pTargetUser;
							pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

							pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
							pUser->UpdatePosition();
							pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;
						}
					}
				}
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialRoundingFireResToPoint(MSG_ROUNDING_FIRE_RES_TO_POINT* roundingfireresult)
{
	if(roundingfireresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( roundingfireresult->sLeftForce ); 
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
		
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(roundingfireresult->usAttackerID);
		if(pUser)
		{
			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[roundingfireresult->sSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[roundingfireresult->sSkillID]->motion_female;
			
			_XDWINBATTLELOG("=== Rounding Fire Result Point [%s], motionindex [%d]", pUser->m_CharacterInfo.charactername, motionindex);
			
			pUser->SetAnimationIndex(motionindex);
			pUser->m_AttackResult.skillType = g_SkillProperty[roundingfireresult->sSkillID]->usefulType;
			pUser->m_AttackResult.sSkillID = roundingfireresult->sSkillID;
			pUser->m_AttackResult.cSkillLevel = roundingfireresult->cSpellStep;
			pUser->m_AttackResult.ucToType = roundingfireresult->ucTargetType;
			pUser->m_AttackResult.fX = roundingfireresult->fTargetX;
			pUser->m_AttackResult.fZ = roundingfireresult->fTargetZ;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			D3DXVECTOR3 targetpos;
			targetpos.x = roundingfireresult->fTargetX;
			targetpos.z = roundingfireresult->fTargetZ;
			targetpos.y = g_LodTerrain.GetTerrainAndRideOBBHeight(targetpos, NULL);

			pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, targetpos);
			pUser->UpdatePosition();
			
			// 모든 무기 type이면 들고 있던 무기 삭제
			pUser->DeleteWeaponModel(pUser->m_AttackResult.sSkillID);
		}
	}
}

void _XNetwork::MSGMartialSmashingResultMob(MSG_SMASHING_RESULT_MOB* smashingresult)
{
	if(smashingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
			
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		g_pLocalUser->m_AttackResult.usDefenderID = smashingresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = smashingresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = smashingresult->usDamage;
		g_pLocalUser->m_AttackResult.ucToType = smashingresult->ucTargetType;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( smashingresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = smashingresult->cBattleResult;

		_XMob* pMob = (_XMob*)g_Moblist.FindMob(smashingresult->usTargetID);
		if(pMob)
		{
			pMob->m_HealthPercentage = smashingresult->cMobVitalRate;
			pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(smashingresult->usAttackerID);
		if(pUser)
		{
			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[smashingresult->sSmashingSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[smashingresult->sSmashingSkillID]->motion_female;
			
			_XDWINBATTLELOG("=== Smashing Result other [%s], motionindex [%d]", pUser->m_CharacterInfo.charactername, motionindex);
			
			pUser->SetAnimationIndex(motionindex);
			pUser->m_AttackResult.skillType = g_SkillProperty[smashingresult->sSmashingSkillID]->usefulType;
			pUser->m_AttackResult.sSkillID = smashingresult->sSmashingSkillID;
			pUser->m_AttackResult.cSkillLevel = smashingresult->cSkillStep;
			pUser->m_AttackResult.ucToType = smashingresult->ucTargetType;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			pUser->m_AttackResult.usDefenderID = smashingresult->usTargetID;

			pUser->m_AttackResult.usAtkInfo[0] = smashingresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = smashingresult->usDamage;
			pUser->m_AttackResult.ucToType = smashingresult->ucTargetType;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_AttackResult.cBattleResult = smashingresult->cBattleResult;
			
			_XMob* pMob = (_XMob*)g_Moblist.FindMob(smashingresult->usTargetID);
			if(pMob)
			{
				pUser->m_AttackTarget = pMob;
				pUser->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;
				
				pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pMob->m_Position);
				pUser->UpdatePosition();

				pMob->m_HealthPercentage = smashingresult->cMobVitalRate;
				pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;	// Display용

				pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
			}
		}
	}
}

void _XNetwork::MSGMartialSmashingResultPVP(MSG_SMASHING_RESULT_PVP* smashingresult)
{
	if(smashingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
		
		g_pLocalUser->m_AttackResult.usDefenderID = smashingresult->usTargetID;

		g_pLocalUser->m_AttackResult.usAtkInfo[0] = smashingresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = smashingresult->usDamage;
		g_pLocalUser->m_AttackResult.ucToType = smashingresult->ucTargetType;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( smashingresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = smashingresult->cBattleResult;
		
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
		
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(smashingresult->usTargetID);
		if(pUser)
		{
//			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			
			_XDWINBATTLELOG("== smashing result [LocalUser/Attack[%s]]", pUser->m_CharacterInfo.charactername);
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(smashingresult->usAttackerID);
		if(pUser)
		{
			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[smashingresult->sSmashingSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[smashingresult->sSmashingSkillID]->motion_female;
			
			_XDWINBATTLELOG("=== Smashing Fire Result other [%s], motionindex [%d]", pUser->m_CharacterInfo.charactername, motionindex);
			
			pUser->SetAnimationIndex(motionindex);
			pUser->m_AttackResult.skillType = g_SkillProperty[smashingresult->sSmashingSkillID]->usefulType;
			pUser->m_AttackResult.sSkillID = smashingresult->sSmashingSkillID;
			pUser->m_AttackResult.cSkillLevel = smashingresult->cSkillStep;
			pUser->m_AttackResult.ucToType = smashingresult->ucTargetType;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			pUser->m_AttackResult.usDefenderID = smashingresult->usTargetID;

			pUser->m_AttackResult.usAtkInfo[0] = smashingresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = smashingresult->usDamage;
			pUser->m_AttackResult.ucToType = smashingresult->ucTargetType;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.cBattleResult = smashingresult->cBattleResult;

			if(smashingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
			{
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
			}
			
			_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(smashingresult->usTargetID);
			if(pTargetUser)
			{
				pUser->m_AttackTarget = pTargetUser;
				pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
				
				pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
				pUser->UpdatePosition();
//				pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;

				_XDWINBATTLELOG("== smashing result [%s/Attack[%s]]", pUser->m_CharacterInfo.charactername, pTargetUser->m_CharacterInfo.charactername);
			}
		}
	}
}

void _XNetwork::MSGMartialActionResult(MSG_ACTION_RESULT* actionresult)
{
	if(actionresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		// 0성공 1회피 2내력부족 3복면상태아님 4대상없음 5no.effect 6no.skill 7cool.time.error
		switch(actionresult->ucResult)
		{
		case en_action_result_success :
			{
				// 성공
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
				
				g_pLocalUser->m_AttackResult.usDefenderID = actionresult->usTargetID;
				g_pLocalUser->m_CharacterInfo.Set_current_forcepower( actionresult->sLeftForce );
				
				if(g_pJinStatus_Window)
					g_pJinStatus_Window->SetParameter();
				
				_XUser* pUser = (_XUser*)g_Userlist.FindUser(actionresult->usTargetID);
				if(pUser)
				{
				}
			}
			break;
		case en_action_result_fail_or_avoid :
			{
				// 실패 
				if( g_pState_Window )
					g_pState_Window->PrintSystemMessage(3);
				
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
			}
			break;
		case en_action_result_need_force :
			{
				// 내력부족
				if( g_pState_Window )
					g_pState_Window->PrintSystemMessage(0);
				
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
			}
			break;
		case en_action_result_no_mask :
			{
				// 복면상태아님
				if( g_pState_Window )
					g_pState_Window->PrintSystemMessage(2);
				
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
			}
			break;
		case en_action_result_no_target :
			{
				// 대상 없음 (몬스터에게 썼을때로 한정)
				if( g_pState_Window )
					g_pState_Window->PrintSystemMessage(4);
				
				g_pLocalUser->ResetSkillAnimation();
			}
			break;
		case en_action_result_no_effect :
		case en_action_result_no_skill :
		case en_action_result_cool_time_error :
			{
				// 실패로 처리한다
				if( g_pState_Window )
					g_pState_Window->PrintSystemMessage(3);
				
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
			}
			break;
		default :
			{
				g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_3;
			}
			break;
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(actionresult->usAttackerID);
		if(pUser)
		{
			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[actionresult->sActionSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[actionresult->sActionSkillID]->motion_female;
			
			_XDWINBATTLELOG("=== Action Fire Result other [%s], motionindex [%d]", pUser->m_CharacterInfo.charactername, motionindex);
			
			pUser->SetAnimationIndex(motionindex);
			pUser->m_AttackResult.skillType = g_SkillProperty[actionresult->sActionSkillID]->usefulType;
			pUser->m_AttackResult.sSkillID = actionresult->sActionSkillID;
			pUser->m_AttackResult.cSkillLevel = actionresult->cSkillStep;
			pUser->m_AttackResult.ucToType = actionresult->ucTargetType;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			
			pUser->m_AttackResult.usDefenderID = actionresult->usTargetID;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			
			_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(actionresult->usTargetID);
			if(pTargetUser)
			{
				pUser->m_AttackTarget = pTargetUser;
				pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
				
				pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pTargetUser->m_Position);
				pUser->UpdatePosition();
			}
		}
	}
}

#define _XXCOMMENT / ## / 
#ifdef _XDWDEBUG
#define _XDWSENDPACKETLOG(_str_) _XDebugOutputString( _str_, 1, 1 );
#else
#define _XDWSENDPACKETLOG		_XXCOMMENT
#endif

void _XNetwork::MSGMartialActiveResultMob(MSG_ACTIVE_RESULT_MOB* activeresult)
{
	if(activeresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		// Local user의 공격 결과
		g_pLocalUser->m_AttackResult.usDefenderID = activeresult->usTargetID;
		g_pLocalUser->m_AttackResult.ucSequence = activeresult->ucSequence;

		g_pLocalUser->m_AttackResult.usAtkInfo[0] = activeresult->usAtkInfo_1;

		g_pLocalUser->m_AttackResult.sDamage[0] = activeresult->usDamage_1;

		g_pLocalUser->m_AttackResult.display[0] = TRUE;

		g_pLocalUser->m_AttackResult.usAtkInfo[1] = activeresult->usAtkInfo_2;

		g_pLocalUser->m_AttackResult.sDamage[1] = activeresult->usDamage_2;
		g_pLocalUser->m_AttackResult.display[1] = TRUE;

		g_pLocalUser->m_AttackResult.usAtkInfo[2] = activeresult->usAtkInfo_3;

		g_pLocalUser->m_AttackResult.sDamage[2] = activeresult->usDamage_3;
		g_pLocalUser->m_AttackResult.display[2] = TRUE;

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( activeresult->sLeftForce );

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		g_pLocalUser->m_AttackResult.ucToType = activeresult->ucTargetType;
		g_pLocalUser->m_AttackResult.cBattleResult = activeresult->cBattleResult;

		// 전투 로그 기록
		g_pLocalUser->m_CharacterLog.isAttack = TRUE;
		g_pLocalUser->m_CharacterLog.attackTime = g_ServerTimeCode;

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0], g_pLocalUser->m_AttackResult.sDamage[1], g_pLocalUser->m_AttackResult.sDamage[2]);
			g_BattleLog.DumpLogResult();
		}

		switch(activeresult->ucTargetType)
		{
		case en_target_type_monster :
			{
				_XMob* pMob = (_XMob*)g_Moblist.FindMob(activeresult->usTargetID);
				if(pMob)
				{
					g_pLocalUser->m_CharacterLog.attackTargetType = pMob->m_MobType;
					pMob->m_HealthPercentage = activeresult->cMobVitalRate_3;
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
			break;
		case en_target_type_object:
			{
				_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(activeresult->usTargetID);
				if(pFunctionalObject)
				{
					g_pLocalUser->m_CharacterLog.attackTargetType = pFunctionalObject->m_ObjectType;
					pFunctionalObject->m_HealthPercentage = activeresult->cMobVitalRate_3;	
				}
			}
			break;
		case en_target_type_npc :
		case en_target_type_dummy :
			break;
		default :
			break;
		}
	}
	else
	{
		// 다른 유저의 공격 결과
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(activeresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = _XSI_USEFULTYPE_ACTIVESKILL;
			pUser->SetAnimationIndex(activeresult->sAnimation);

			pUser->m_AttackResult.usDefenderID = activeresult->usTargetID;
			pUser->m_AttackResult.ucSequence = activeresult->ucSequence;

			pUser->m_AttackResult.usAtkInfo[0] = activeresult->usAtkInfo_1;
			pUser->m_AttackResult.sDamage[0] = activeresult->usDamage_1;
			pUser->m_AttackResult.display[0] = TRUE;

			pUser->m_AttackResult.usAtkInfo[1] = activeresult->usAtkInfo_2;
			pUser->m_AttackResult.sDamage[1] = activeresult->usDamage_2;
			pUser->m_AttackResult.display[1] = TRUE;

			pUser->m_AttackResult.usAtkInfo[2] = activeresult->usAtkInfo_3;
			pUser->m_AttackResult.sDamage[2] = activeresult->usDamage_3;
			pUser->m_AttackResult.display[2] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			
			pUser->m_AttackResult.ucToType = activeresult->ucTargetType;
			pUser->m_AttackResult.cBattleResult = activeresult->cBattleResult;

			_XDWINBATTLELOG("== Active result [%s/Attack[Mob]] [%d/%d]", pUser->m_CharacterInfo.charactername, activeresult->ucTargetType, activeresult->usTargetID);

			switch(activeresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(activeresult->usTargetID);
					if(pMob)
					{
						pUser->m_AttackTarget = pMob;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_MOB;
						pMob->m_HealthPercentage = activeresult->cMobVitalRate_3;
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;	// Display
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;

						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pMob->m_Position);
						pUser->UpdatePosition();
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(activeresult->usTargetID);
					if(pFunctionalObject)
					{
						pUser->m_AttackTarget = (_XItem*)pFunctionalObject;;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
						pFunctionalObject->m_HealthPercentage = activeresult->cMobVitalRate_3;
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;	// Display
						
						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pFunctionalObject->m_Position);
						pUser->UpdatePosition();			
					}
				}
				break;
			case en_target_type_npc :
				break;
			case en_target_type_dummy :
				{
					_XDummy* pDummy = (_XDummy*)g_DummyList.Find(activeresult->usTargetID);
					if(pDummy)
					{
						pUser->m_AttackTarget = pDummy;
						pUser->m_AttackTargetObjectType = _XPICKINGTARGET_DUMMY;

						pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pDummy->m_Position);
						pUser->UpdatePosition();
					}
				}
				break;
			default :
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialActiveResultPVP(MSG_ACTIVE_RESULT_PVP* activeresultpvp)
{
	if(activeresultpvp->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		// Local user의 공격 결과
		g_pLocalUser->m_AttackResult.usDefenderID = activeresultpvp->usTargetID;
		g_pLocalUser->m_AttackResult.ucSequence = activeresultpvp->ucSequence;
		
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = activeresultpvp->usAtkInfo_1;
		g_pLocalUser->m_AttackResult.sDamage[0] = activeresultpvp->usDamage_1;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		
		g_pLocalUser->m_AttackResult.usAtkInfo[1] = activeresultpvp->usAtkInfo_2;
		g_pLocalUser->m_AttackResult.sDamage[1] = activeresultpvp->usDamage_2;
		g_pLocalUser->m_AttackResult.display[1] = TRUE;
		
		g_pLocalUser->m_AttackResult.usAtkInfo[2] = activeresultpvp->usAtkInfo_3;
		g_pLocalUser->m_AttackResult.sDamage[2] = activeresultpvp->usDamage_3;
		g_pLocalUser->m_AttackResult.display[2] = TRUE;
		
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( activeresultpvp->sLeftForce );
		g_pLocalUser->m_AttackResult.ucToType = activeresultpvp->ucTargetType;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
		
		// 전투 로그 기록
		g_pLocalUser->m_CharacterLog.isAttack = TRUE;
		g_pLocalUser->m_CharacterLog.attackTime = g_ServerTimeCode;
		
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(activeresultpvp->usTargetID);
		if(pUser)
		{
			g_pLocalUser->m_CharacterLog.attackTargetType = 0;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			_XDWINBATTLELOG("== Active Result [LocalUser/Attack[%s]]", pUser->m_CharacterInfo.charactername);
		}
		
		// Battle Result 처리
		g_pLocalUser->m_AttackResult.cBattleResult = activeresultpvp->cBattleResult;
		
		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0], g_pLocalUser->m_AttackResult.sDamage[1], g_pLocalUser->m_AttackResult.sDamage[2]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		// 다른 유저의 공격 결과
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(activeresultpvp->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = _XSI_USEFULTYPE_ACTIVESKILL;
			pUser->SetAnimationIndex(activeresultpvp->sAnimation);
			
			pUser->m_AttackResult.usDefenderID = activeresultpvp->usTargetID;
			pUser->m_AttackResult.ucSequence = activeresultpvp->ucSequence;
			
			pUser->m_AttackResult.usAtkInfo[0] = activeresultpvp->usAtkInfo_1;
			pUser->m_AttackResult.sDamage[0] = activeresultpvp->usDamage_1;
			pUser->m_AttackResult.display[0] = TRUE;
			
			pUser->m_AttackResult.usAtkInfo[1] = activeresultpvp->usAtkInfo_2;
			pUser->m_AttackResult.sDamage[1] = activeresultpvp->usDamage_2;
			pUser->m_AttackResult.display[1] = TRUE;
			
			pUser->m_AttackResult.usAtkInfo[2] = activeresultpvp->usAtkInfo_3;
			pUser->m_AttackResult.sDamage[2] = activeresultpvp->usDamage_3;
			pUser->m_AttackResult.display[2] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			
			pUser->m_AttackResult.ucToType = activeresultpvp->ucTargetType;

			if(g_pLocalUser->m_Uniqueid == activeresultpvp->usTargetID)
			{
				pUser->m_AttackTarget = g_pLocalUser;
				pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
				
				if( g_pJinStatus_Window )
				{
					g_pJinStatus_Window->SetParameter();	
				}
				
				// 자리 비움 상태 처리
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
			}
			else
			{
				_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(activeresultpvp->usTargetID);
				if(pTargetUser)
				{
					pUser->m_AttackTarget = pTargetUser;
					pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
					pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;

					_XDWINBATTLELOG("== Active Result [%s/Attack[%s]]", pUser->m_CharacterInfo.charactername, pTargetUser->m_CharacterInfo.charactername);
				}
			}
			
			// Battle Result 처리
			pUser->m_AttackResult.cBattleResult = activeresultpvp->cBattleResult;
		}
	}
}

void _XNetwork::MSGMartialChargeResultMob(MSG_CHARGE_RESULT_MOB* chargeresult)
{
	if(chargeresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.ucToType = chargeresult->ucTargetType;
		g_pLocalUser->m_AttackResult.usDefenderID = chargeresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = chargeresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = chargeresult->usDamage;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( chargeresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = chargeresult->cBattleResult;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		switch(chargeresult->ucTargetType)
		{
		case en_target_type_monster :
			{
				_XMob* pMob = (_XMob*)g_Moblist.FindMob(chargeresult->usTargetID);
				if(pMob)
				{
					pMob->m_HealthPercentage = chargeresult->cMobVitalRate;
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
			break;
		case en_target_type_object:
			{
				_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(chargeresult->usTargetID);
				if(pFunctionalObject)
				{
					pFunctionalObject->m_HealthPercentage = chargeresult->cMobVitalRate;
				}
			}
			break;
		case en_target_type_npc :
		case en_target_type_dummy :
			break;
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0], g_pLocalUser->m_AttackResult.sDamage[1], g_pLocalUser->m_AttackResult.sDamage[2]);
			g_BattleLog.DumpLogResult();
		}
	}
	else 
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(chargeresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.ucToType = chargeresult->ucTargetType;
			pUser->m_AttackResult.usDefenderID = chargeresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = chargeresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = chargeresult->usDamage;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_AttackResult.cSkillLevel = chargeresult->cSkillStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.cBattleResult = chargeresult->cBattleResult;

			switch(chargeresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(chargeresult->usTargetID);
					if(pMob)
					{
						pMob->m_HealthPercentage = chargeresult->cMobVitalRate;
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;	// display
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(chargeresult->usTargetID);
					if(pFunctionalObject)
					{
						pFunctionalObject->m_HealthPercentage = chargeresult->cMobVitalRate;
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;	// display
//						if(pFunctionalObject->m_HealthPercentage > 0)
//							pFunctionalObject->SetBlowMotion();
//						else
//							pFunctionalObject->SetDieAnimation();

					}
				}
				break;
			case en_target_type_npc :
			case en_target_type_dummy :
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialChargeResultPVP(MSG_CHARGE_RESULT_PVP* chargeresult)
{
	if(chargeresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.ucToType = chargeresult->ucTargetType;
		g_pLocalUser->m_AttackResult.usDefenderID = chargeresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = chargeresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = chargeresult->usDamage;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( chargeresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = chargeresult->cBattleResult;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		_XUser* pUser = (_XUser*)g_Userlist.FindUser(chargeresult->usTargetID);
		if(pUser)
		{
			g_pLocalUser->m_CharacterLog.attackTargetType = 0;
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0], g_pLocalUser->m_AttackResult.sDamage[1], g_pLocalUser->m_AttackResult.sDamage[2]);
			g_BattleLog.DumpLogResult();
		}
	}
	else 
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(chargeresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.ucToType = chargeresult->ucTargetType;
			pUser->m_AttackResult.usDefenderID = chargeresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = chargeresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = chargeresult->usDamage;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_AttackResult.cSkillLevel = chargeresult->cSkillStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.cBattleResult = chargeresult->cBattleResult;

			if(g_pLocalUser->m_Uniqueid == chargeresult->usTargetID)
			{
				pUser->m_AttackTarget = g_pLocalUser;
				pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;
				
				if( g_pJinStatus_Window )
				{
					g_pJinStatus_Window->SetParameter();	
				}
				
				// 자리 비움 상태 처리
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
				
				_XDWINBATTLELOG("=== charge result [%s/Attack[LocalUser]] ", pUser->m_CharacterInfo.charactername);
			}
			else
			{
				_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(chargeresult->usTargetID);
				if(pTargetUser)
				{
					pUser->m_AttackTarget = pTargetUser;
					pUser->m_AttackTargetObjectType = _XPICKINGTARGET_PC;

					_XDWINBATTLELOG("=== charge result [%s/Attack[%s]]", pUser->m_CharacterInfo.charactername, pTargetUser->m_CharacterInfo.charactername);
				}
			}
		}
	}
}

void _XNetwork::MSGMartialCastingResultToPoint(MSG_CASTING_RESULT_TO_POINT* castingresultpoint)
{
	if(castingresultpoint->usAttackerID != g_pLocalUser->m_Uniqueid)
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(castingresultpoint->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = castingresultpoint->ucMartialType;
			pUser->m_AttackResult.sSkillID = castingresultpoint->sCastingSpellID;
			pUser->m_AttackResult.cSkillLevel = castingresultpoint->cSpellStep;
			pUser->m_AttackResult.fX = castingresultpoint->fTargetPosX;
			pUser->m_AttackResult.fZ = castingresultpoint->fTargetPosZ;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
		}
	}
	else
	{
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( castingresultpoint->sLeftForce );

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
	}

	g_pLocalUser->ViewCastingDamage(); //Author : 양희왕 //breif : 기공대미지 표시
}

void _XNetwork::MSGMartialCastingResultToMob(MSG_CASTING_RESULT_TO_MOB* castingresult)
{
	if(castingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.ucToType = castingresult->ucTargetType;
		g_pLocalUser->m_AttackResult.usDefenderID = castingresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = castingresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = castingresult->usDamage;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( castingresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = castingresult->cBattleResult;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		switch(castingresult->ucTargetType)
		{
		case en_target_type_monster :
			{
				_XMob* pMob = (_XMob*)g_Moblist.FindMob(castingresult->usTargetID);
				if(pMob)
				{
					pMob->m_HealthPercentage = castingresult->cMobVitalRate;
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
			break;
		case en_target_type_object:
			{
				_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(castingresult->usTargetID);
				if(pFunctionalObject)
				{
					pFunctionalObject->m_HealthPercentage = castingresult->cMobVitalRate;
//					if(pFunctionalObject->m_HealthPercentage > 0)
//						pFunctionalObject->SetBlowMotion();
//					else
//						pFunctionalObject->SetDieAnimation();

				}
			}
			break;
		case en_target_type_npc :
		case en_target_type_dummy :
			break;
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(castingresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.ucToType = castingresult->ucTargetType;
			pUser->m_AttackResult.usDefenderID = castingresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = castingresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = castingresult->usDamage;
			pUser->m_AttackResult.display[0] = TRUE;

			pUser->m_AttackResult.sSkillID = castingresult->sCastingSpellID;
			pUser->m_AttackResult.cSkillLevel = castingresult->cSpellStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			pUser->m_AttackResult.cBattleResult = castingresult->cBattleResult;

			switch(castingresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(castingresult->usTargetID);
					if(pMob)
					{
						pMob->m_HealthPercentage = castingresult->cMobVitalRate;
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(castingresult->usTargetID);
					if(pFunctionalObject)
					{
						pFunctionalObject->m_HealthPercentage = castingresult->cMobVitalRate;
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;
//						if(pFunctionalObject->m_HealthPercentage > 0)
//							pFunctionalObject->SetBlowMotion();
//						else
//							pFunctionalObject->SetDieAnimation();

					}
				}
				break;
			case en_target_type_npc :
			case en_target_type_dummy :
				break;
			}
		}
	}

	g_pLocalUser->ViewCastingDamage(); //Author : 양희왕 //breif : 기공대미지 표시
}

void _XNetwork::MSGMartialCastingResultToPVP(MSG_CASTING_RESULT_TO_PVP* castingresult)
{
	if(castingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.ucToType = castingresult->ucTargetType;
		g_pLocalUser->m_AttackResult.usDefenderID = castingresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = castingresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = castingresult->usDamage;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( castingresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = castingresult->cBattleResult;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		_XUser* pUser = (_XUser*)g_Userlist.FindUser(castingresult->usTargetID);
		if(pUser)
		{
			g_pLocalUser->m_CharacterLog.attackTargetType = 0;
//			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			_XDWINBATTLELOG("== Casting Result [LocalUser/Attack[%s]]", pUser->m_CharacterInfo.charactername);
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(castingresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.ucToType = castingresult->ucTargetType;
			pUser->m_AttackResult.usDefenderID = castingresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = castingresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = castingresult->usDamage;
			pUser->m_AttackResult.display[0] = TRUE;
			
			pUser->m_AttackResult.sSkillID = castingresult->sCastingSpellID;
			pUser->m_AttackResult.cSkillLevel = castingresult->cSpellStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			
			pUser->m_AttackResult.cBattleResult = castingresult->cBattleResult;

			if(castingresult->usTargetID == g_pLocalUser->m_Uniqueid)
			{
				// 자리 비움 상태 처리
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
			}

			_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(castingresult->usTargetID);
			if(pTargetUser)
			{
//				pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;

				_XDWINBATTLELOG("== Casting Result [%s/Attack[%s]]", pUser->m_CharacterInfo.charactername, pTargetUser->m_CharacterInfo.charactername);
			}
		}
	}

	g_pLocalUser->ViewCastingDamage(); //Author : 양희왕 //breif : 기공대미지 표시
}

void _XNetwork::MSGMartialChannelingResultPoint(MSG_CHANNEL_RESULT_POINT* channelresult)
{
	if(channelresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( channelresult->sLeftForce );
		
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(channelresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = channelresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = channelresult->sChannelSpellID;
			pUser->m_AttackResult.cSkillLevel = channelresult->cSpellStep;
			pUser->m_AttackResult.fX = channelresult->fTargetPosX;
			pUser->m_AttackResult.fZ = channelresult->fTargetPosZ;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			if( pUser->m_ModelDescriptor.m_EffectManager )
				pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance( channelresult->sChannelSpellID + 40000 );
		}
	}
}

void _XNetwork::MSGMartialChannelingResultPC(MSG_CHANNEL_RESULT_PC* channelresult)
{
	if(channelresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		if(channelresult->ucTargetType == en_target_type_self)
		{
			// self
		}
		else
		{
			// other user - target
		}

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( channelresult->sLeftForce );
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
	}
	else
	{
		// other user - attacker
		_XUser* pUser = g_Userlist.FindUser(channelresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = channelresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = channelresult->sChannelSpellID;
			pUser->m_AttackResult.cSkillLevel = channelresult->cSpellStep;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			if(channelresult->usTargetID == g_pLocalUser->m_Uniqueid)
			{
				// 자리 비움 상태 처리
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
			}
		}
	}
}

void _XNetwork::MSGMartialChannelingResultNPC(MSG_CHANNEL_RESULT_NPC* channelresult)
{
	//Author : 양희왕 //breif : 이건 대미지 표시가 전혀 없었다..... 2월 25일 수정
	/*
    if(channelresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
        g_pLocalUser->m_AttackResult.ucToType = channelresult->ucTargetType;
		g_pLocalUser->m_AttackResult.usDefenderID = channelresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = channelresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = channelresult->usDamage;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( channelresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = channelresult->cBattleResult;

        if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		switch(castingresult->ucTargetType)
		{
		case en_target_type_monster :
			{
				_XMob* pMob = (_XMob*)g_Moblist.FindMob(channelresult->usTargetID);
				if(pMob)
				{                 
					pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
					pMob->m_HealthPercentage = channelresult->cMobVitalRate;
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
			break;
		case en_target_type_object:
			{
				_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(castingresult->usTargetID);
				if(pFunctionalObject)
				{
					pFunctionalObject->m_HealthPercentage = channelresult->cMobVitalRate;
				}
			}
			break;
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
    }
    else
    {
        _XUser* pUser = (_XUser*)g_Userlist.FindUser(channelresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.ucToType = channelresult->ucTargetType;
			pUser->m_AttackResult.usDefenderID = channelresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = channelresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = channelresult->usDamage;
			pUser->m_AttackResult.display[0] = TRUE;

            pUser->m_AttackResult.skillType = channelresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = channelresult->sCastingSpellID;
			pUser->m_AttackResult.cSkillLevel = channelresult->cSpellStep;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			pUser->m_AttackResult.cBattleResult = channelresult->cBattleResult;

			switch(castingresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(channelresult->usTargetID);
					if(pMob)
					{
						pMob->m_HealthPercentage = channelresult->cMobVitalRate;
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(channelresult->usTargetID);
					if(pFunctionalObject)
					{
						pFunctionalObject->m_HealthPercentage = channelresult->cMobVitalRate;
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;
					}
				}
				break;
			}
		}
    }

    g_pLocalUser->ViewCastingDamage(); //Author : 양희왕 //breif : 기공대미지 표시*/

	if(channelresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		if(channelresult->ucTargetType == en_target_type_monster)
		{
			// mob 
			if(channelresult->cBattleResult == en_battle_result_binsa_or_die || channelresult->cBattleResult == en_battle_result_honsu_or_dead)
			{
				_XMob* pMob = g_Moblist.FindMob(channelresult->usTargetID);
				if(pMob)
				{
					pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
					pMob->SetAnimation(_XMOBACTION_DIE);
					pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
		}
		else if(channelresult->ucTargetType == en_target_type_object)
		{
			if(channelresult->cBattleResult == en_battle_result_binsa_or_die || channelresult->cBattleResult == en_battle_result_honsu_or_dead)
			{
				_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(channelresult->usTargetID);
				if(pFunctionalObject)
				{
					pFunctionalObject->m_HealthPercentage = 0;
					pFunctionalObject->m_DisplayHealthPercentage = 0;
					pFunctionalObject->SetDieAnimation();
				}
			}
		}

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( channelresult->sLeftForce );
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
	}
	else
	{
		// other user - attacker
		_XUser* pUser = g_Userlist.FindUser(channelresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.skillType = channelresult->ucMartialType;
			pUser->m_AttackResult.sSkillID = channelresult->sChannelSpellID;
			pUser->m_AttackResult.cSkillLevel = channelresult->cSpellStep;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			if(channelresult->ucTargetType == en_target_type_monster)
			{
				// mob 
				if(channelresult->cBattleResult == en_battle_result_binsa_or_die || channelresult->cBattleResult == en_battle_result_honsu_or_dead)
				{
					_XMob* pMob = g_Moblist.FindMob(channelresult->usTargetID);
					if(pMob)
					{
						pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
			}
			else if(channelresult->ucTargetType == en_target_type_object)
			{
				if(channelresult->cBattleResult == en_battle_result_binsa_or_die || channelresult->cBattleResult == en_battle_result_honsu_or_dead)
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(channelresult->usTargetID);
					if(pFunctionalObject)
					{
						pFunctionalObject->m_HealthPercentage = 0;
						pFunctionalObject->m_DisplayHealthPercentage = 0;
						pFunctionalObject->SetDieAnimation();
					}
				}
			}
		}
	}
}

void _XNetwork::MSGMartialFinishResultMob(MSG_FINISH_RESULT_TO_MOB* finishresult)
{
	if(finishresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.usDefenderID = finishresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = finishresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = finishresult->usDamage;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		g_pLocalUser->m_AttackResult.ucToType = finishresult->ucTargetType;

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( finishresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = finishresult->cBattleResult;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		switch(finishresult->ucTargetType)
		{
		case en_target_type_monster :
			{
				_XMob* pMob = (_XMob*)g_Moblist.FindMob(finishresult->usTargetID);
				if(pMob)
				{
					pMob->m_HealthPercentage = finishresult->cMobVitalRate;
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
			break;
		case en_target_type_object:
			{
				_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(finishresult->usTargetID);
				if(pFunctionalObject)
				{
					pFunctionalObject->m_HealthPercentage = finishresult->cMobVitalRate;
//					if(pFunctionalObject->m_HealthPercentage > 0)
//						pFunctionalObject->SetBlowMotion();
//					else
//						pFunctionalObject->SetDieAnimation();
				}
			}
			break;
		case en_target_type_npc :
			break;
		case en_target_type_dummy :
			break;
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(finishresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.usDefenderID = finishresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = finishresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = finishresult->usDamage;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_AttackResult.ucToType = finishresult->ucTargetType;

			pUser->m_AttackResult.sSkillID = finishresult->sFinishSkillID;
			pUser->m_AttackResult.cSkillLevel = finishresult->cSkillStep;
			pUser->m_AttackResult.cBattleResult = finishresult->cBattleResult;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			switch(finishresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(finishresult->usTargetID);
					if(pMob)
					{
						pMob->m_HealthPercentage = finishresult->cMobVitalRate;
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(finishresult->usTargetID);
					if(pFunctionalObject)
					{
						pFunctionalObject->m_HealthPercentage = finishresult->cMobVitalRate;
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;
//						if(pFunctionalObject->m_HealthPercentage > 0)
//							pFunctionalObject->SetBlowMotion();
//						else
//							pFunctionalObject->SetDieAnimation();

					}
				}
				break;
			case en_target_type_npc :
				break;
			case en_target_type_dummy :
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialFinishResultPC(MSG_FINISH_RESULT_TO_PC* finishresult)
{
	if(finishresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.usDefenderID = finishresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = finishresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = finishresult->usDamage;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		g_pLocalUser->m_AttackResult.ucToType = finishresult->ucTargetType;
		
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( finishresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = finishresult->cBattleResult;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		_XUser* pUser = (_XUser*)g_Userlist.FindUser(finishresult->usTargetID);
		if(pUser)
		{
//			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			_XDWINBATTLELOG("== finish result [LocalUser/Attack[%s]]", pUser->m_CharacterInfo.charactername);			
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(finishresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.usDefenderID = finishresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = finishresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = finishresult->usDamage;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_AttackResult.ucToType = finishresult->ucTargetType;
			
			pUser->m_AttackResult.sSkillID = finishresult->sFinishSkillID;
			pUser->m_AttackResult.cSkillLevel = finishresult->cSkillStep;
			pUser->m_AttackResult.cBattleResult = finishresult->cBattleResult;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			if(finishresult->usTargetID == g_pLocalUser->m_Uniqueid)
			{
				// 자리 비움 상태 처리
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
			}

			_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(finishresult->usTargetID);
			if(pTargetUser)
			{
//				pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;

				_XDWINBATTLELOG("== fininsh result [%s/Attack[%s]]", pUser->m_CharacterInfo.charactername, pTargetUser->m_CharacterInfo.charactername);
			}
		}
	}
}

void _XNetwork::MSGMartialRoundingResultMob(MSG_ROUNDING_RESULT_TO_MOB* roundingresult)
{
	if(roundingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.usDefenderID = roundingresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = roundingresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = roundingresult->usDamage;
		g_pLocalUser->m_AttackResult.ucToType = roundingresult->ucTargetType;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;

		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( roundingresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = roundingresult->cBattleResult;

		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();

		switch(roundingresult->ucTargetType)
		{
		case en_target_type_monster :
			{
				_XMob* pMob = (_XMob*)g_Moblist.FindMob(roundingresult->usTargetID);
				if(pMob)
				{
					pMob->m_HealthPercentage = roundingresult->cMobVitalRate;
					pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
				}
			}
			break;
		case en_target_type_object:
			{
				_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(roundingresult->usTargetID);
				if(pFunctionalObject)
				{
					pFunctionalObject->m_HealthPercentage = roundingresult->cMobVitalRate;
//					if(pFunctionalObject->m_HealthPercentage > 0)
//						pFunctionalObject->SetBlowMotion();
//					else
//						pFunctionalObject->SetDieAnimation();

				}
			}
			break;
		case en_target_type_npc :
		case en_target_type_dummy :
			break;
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(roundingresult->usAttackerID);
		if(pUser)
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			pUser->DeleteWeaponModel(roundingresult->sRoundingSpellID);

			pUser->m_AttackResult.usDefenderID = roundingresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = roundingresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = roundingresult->usDamage;
			pUser->m_AttackResult.ucToType = roundingresult->ucTargetType;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.cBattleResult = roundingresult->cBattleResult;

			switch(roundingresult->ucTargetType)
			{
			case en_target_type_monster :
				{
					_XMob* pMob = (_XMob*)g_Moblist.FindMob(roundingresult->usTargetID);
					if(pMob)
					{
						pMob->m_HealthPercentage = roundingresult->cMobVitalRate;
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
						pMob->m_LastRecivedMessageTime = g_LocalSystemTime;
					}
				}
				break;
			case en_target_type_object:
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(roundingresult->usTargetID);
					if(pFunctionalObject)
					{
						pFunctionalObject->m_HealthPercentage = roundingresult->cMobVitalRate;
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;
//						if(pFunctionalObject->m_HealthPercentage > 0)
//							pFunctionalObject->SetBlowMotion();
//						else
//							pFunctionalObject->SetDieAnimation();

					}
				}
				break;
			case en_target_type_npc :
			case en_target_type_dummy :
				break;
			}
		}
	}
}

void _XNetwork::MSGMartialRoundingResultPC(MSG_ROUNDING_RESULT_TO_PC* roundingresult)
{
	if(roundingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.usDefenderID = roundingresult->usTargetID;
		g_pLocalUser->m_AttackResult.usAtkInfo[0] = roundingresult->ucAtkInfo;
		g_pLocalUser->m_AttackResult.sDamage[0] = roundingresult->usDamage;
		g_pLocalUser->m_AttackResult.ucToType = roundingresult->ucTargetType;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( roundingresult->sLeftForce );
		g_pLocalUser->m_AttackResult.cBattleResult = roundingresult->cBattleResult;
		
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();
		
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(roundingresult->usTargetID);
		if(pUser)
		{
//			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			
			_XDWINBATTLELOG("== rounding result [LocalUser/Attack[%s]]", pUser->m_CharacterInfo.charactername);
		}

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.SetAtkInfo(g_pLocalUser->m_AttackResult.usAtkInfo[0]);
			g_BattleLog.SetDamage(g_pLocalUser->m_AttackResult.sDamage[0]);
			g_BattleLog.DumpLogResult();
		}
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(roundingresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.usDefenderID = roundingresult->usTargetID;
			pUser->m_AttackResult.usAtkInfo[0] = roundingresult->ucAtkInfo;
			pUser->m_AttackResult.sDamage[0] = roundingresult->usDamage;
			pUser->m_AttackResult.ucToType = roundingresult->ucTargetType;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
			pUser->m_AttackResult.cBattleResult = roundingresult->cBattleResult;

			if(roundingresult->usTargetID == g_pLocalUser->m_Uniqueid)
			{
				// 자리 비움 상태 처리
				if(g_pLocalUser->m_bOut)
					g_pLocalUser->SetOut(FALSE);
			}
			
			_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(roundingresult->usTargetID);
			if(pTargetUser)
			{
//				pTargetUser->m_LastRecivedMessageTime = g_LocalSystemTime;
				
				_XDWINBATTLELOG("== rounding result [%s/Attack[%s]]", pUser->m_CharacterInfo.charactername, pTargetUser->m_CharacterInfo.charactername);
			}
		}
	}
}

void _XNetwork::MSGMartialRoundingResultPoint(MSG_ROUNDING_RESULT_TO_POINT* roundingresult)
{
	if(roundingresult->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_AttackResult.ucToType = roundingresult->ucTargetType;
		g_pLocalUser->m_AttackResult.display[0] = TRUE;
		g_pLocalUser->m_CharacterInfo.Set_current_forcepower( roundingresult->sLeftForce );
		
		if(g_pJinStatus_Window)
			g_pJinStatus_Window->SetParameter();		
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(roundingresult->usAttackerID);
		if(pUser)
		{
			pUser->m_AttackResult.ucToType = roundingresult->ucTargetType;
			pUser->m_AttackResult.display[0] = TRUE;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
		}
	}
}

void _XNetwork::MSGMartialChargeEnd(MSG_CHARGE_END* chargeend)
{
	if(chargeend->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		VIEWPACKETLOG
			_XDWPACKETLOG("=== Charge end : Current User battle state [%d]", g_pLocalUser->m_CurrentBattleState);

		switch(chargeend->cReason)
		{
		case en_martial_end_normal :
			_XDWPACKETLOG("=== normal end");
			break;
		case en_martial_end_no_martial :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1617), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_force :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_select :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1615), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_invalid_target :
			{
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{					
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
					{						
						if( pFunctionalObject->m_bAttackable )
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetViewTarget();
							g_pLocalUser->ResetSpellAttackTarget();
						}
						else
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1614), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							break;
						}
					}
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			break;
		case en_martial_end_need_training :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1613), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_cooltime_error :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1612), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		}
		g_pLocalUser->ResetSkillAnimation();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(chargeend->usAttackerID);
		if(pUser)
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
	}
}

void _XNetwork::MSGMartialChannelEnd(MSG_CHANNEL_END* channelend)
{
	if(channelend->usAttackerID != g_pLocalUser->m_Uniqueid)
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(channelend->usAttackerID);
		if(pUser)
		{
			short motionindex = -1;
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
				motionindex = g_SkillProperty[pUser->m_AttackResult.sSkillID]->motion_male;
			else
				motionindex = g_SkillProperty[pUser->m_AttackResult.sSkillID]->motion_female;
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

			pUser->SetAnimationIndex(motionindex + _XANI_ORDER_FIRE);
		}
	}
	else
	{
		switch(channelend->cReason)
		{
		case en_martial_end_normal :
			_XDWPACKETLOG("=== normal end");
			break;
		case en_martial_end_no_martial :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1617), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				g_pLocalUser->ResetSkillAnimation();
			}
			break;
		case en_martial_end_no_force :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				g_pLocalUser->ResetSkillAnimation();
			}
			break;
		case en_martial_end_no_select :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1615), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				g_pLocalUser->ResetSkillAnimation();
			}
			break;
		case en_martial_end_invalid_target :
			{
				g_pLocalUser->ResetSkillAnimation();
				
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{					
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
					{						
						if( pFunctionalObject->m_bAttackable )
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetViewTarget();
							g_pLocalUser->ResetSpellAttackTarget();
						}
						else
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1614), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							break;
						}
					}
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			break;
		case en_martial_end_need_training :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1613), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				g_pLocalUser->ResetSkillAnimation();
			}
			break;
		case en_martial_end_cooltime_error :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1612), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				g_pLocalUser->ResetSkillAnimation();
			}
			break;
		case en_martial_end_distance_too_far :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1618), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				g_pLocalUser->ResetSkillAnimation();
				g_pLocalUser->m_bLeftClickProcess = TRUE;
			}
			break;
		default :
			{
				_XDWPACKETLOG("=== Channel End [Reason:%d]", channelend->cReason);
				g_pLocalUser->ResetSkillAnimation();
			}
			break;
		}
	}
}

void _XNetwork::MSGMartialFinishEnd(MSG_FINISH_END* finishend)
{
	if(finishend->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		switch(finishend->cReason)
		{
		case en_martial_end_normal :
			_XDWPACKETLOG("=== normal end");
			break;
		case en_martial_end_no_martial :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1617), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_force :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_select :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1615), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_invalid_target :
			{
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{					
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
					{					
						if( pFunctionalObject->m_bAttackable )
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetViewTarget();
							g_pLocalUser->ResetSpellAttackTarget();
						}	
						else
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1614), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							break;
						}
					}
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			break;
		case en_martial_end_need_training :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1613), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);			
			break;
		case en_martial_end_cooltime_error :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1612), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		}

		if(g_pLocalUser->m_bSendFinishFirePacket)
		{
			g_pLocalUser->m_CurrentBattleState = _XUSER_STATE_LAST;
			g_pLocalUser->SetAnimation(_XACTION_IDLE);
		}
		else
		{
			g_pLocalUser->ResetSkillAnimation();
		}

		_XDWINBATTLELOG("=== Finish skill end Local User");
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(finishend->usAttackerID);
		if(pUser)
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

		_XDWINBATTLELOG("=== Finish skill end other User");
	}
}

void _XNetwork::MSGMartialCastingEnd(MSG_CASTING_END* castingend)
{
	if(castingend->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		switch(castingend->cReason)
		{
		case en_martial_end_normal :
			_XDWPACKETLOG("=== normal end");
			break;
		case en_martial_end_no_martial :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1617), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_force :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_select :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1615), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_invalid_target :
			{
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{					
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
					{						
						if( pFunctionalObject->m_bAttackable )
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetViewTarget();
							g_pLocalUser->ResetSpellAttackTarget();
						}
						else
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1614), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							break;
						}
					}
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			break;
		case en_martial_end_need_training :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1613), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_cooltime_error :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1612), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		}
		g_pLocalUser->ResetSkillAnimation();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(castingend->usAttackerID);
		if(pUser)
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
		
		_XDWINBATTLELOG("=== Casting Spell end other User");
	}
}

void _XNetwork::MSGMartialRoundingEnd(MARTIAL_END* martialend)
{
	if(martialend->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		switch(martialend->cReason)
		{
		case en_martial_end_normal :
			_XDWPACKETLOG("=== normal end");
			break;
		case en_martial_end_no_martial :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1617), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_force :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_select :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1615), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_invalid_target :
			{
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{					
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
					{						
						if( pFunctionalObject->m_bAttackable )
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetViewTarget();
							g_pLocalUser->ResetSpellAttackTarget();
						}
						else
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1614), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							break;
						}
					}
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			break;
		case en_martial_end_need_training :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1613), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_cooltime_error :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1612), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		}
		g_pLocalUser->ResetSkillAnimation();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(martialend->usAttackerID);
		if(pUser)
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
		
		_XDWINBATTLELOG("=== Rounding Spell end other User");
	}
}

void _XNetwork::MSGMartialSmashingEnd(MARTIAL_END* martialend)
{
	if(martialend->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		switch(martialend->cReason)
		{
		case en_martial_end_normal :
			_XDWPACKETLOG("=== normal end");
			break;
		case en_martial_end_no_martial :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1617), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_force :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_select :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1615), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_invalid_target :
			{
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{					
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
					{						
						if( pFunctionalObject->m_bAttackable )
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetViewTarget();
							g_pLocalUser->ResetSpellAttackTarget();
						}
						else
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1614), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							break;
						}
					}
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			break;
		case en_martial_end_need_training :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1613), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_cooltime_error :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1612), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		}
		g_pLocalUser->ResetSkillAnimation();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(martialend->usAttackerID);
		if(pUser)
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
		
		_XDWINBATTLELOG("=== Smashing Spell end other User");
	}
}

void _XNetwork::MSGMartialEnd(MARTIAL_END* martialend)
{
	if(martialend->usAttackerID == g_pLocalUser->m_Uniqueid)
	{
		switch(martialend->cReason)
		{
		case en_martial_end_normal :
			_XDWPACKETLOG("=== normal end");
			break;
		case en_martial_end_no_martial :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1617), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_force :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_no_select :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1615), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_invalid_target :
			{
				if( g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{					
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_AttackTarget;
					if(pFunctionalObject)
					{						
						if( pFunctionalObject->m_bAttackable )
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							g_pLocalUser->ResetAttackTarget();
							g_pLocalUser->ResetViewTarget();
							g_pLocalUser->ResetSpellAttackTarget();
						}
						else
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1614), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							break;
						}
					}
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			break;
		case en_martial_end_need_training :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1613), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		case en_martial_end_cooltime_error :
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1612), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			break;
		}
		g_pLocalUser->ResetSkillAnimation();
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(martialend->usAttackerID);
		if(pUser)
			pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
	}
}

BOOL _XNetwork::Send_MSGMiniGame(int type, int param1, int param2, int param3)
{
	switch(type)
	{
	case MSG_NO_MINIGAME_START_REQ_C_S :
		{
			// 수련 시작
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : MiniGame Start Req C_S ====");

			int size = sizeof(MSG_MINIGAME_START_REQ);
			MSG_MINIGAME_START_REQ startreq;
			ZeroMemory(&startreq, size);
			startreq.header.sLength = size;
			startreq.ucMessage = type;

			startreq.sMiniGameID = (short)param1;
			startreq.skillID = (short)param2;
			startreq.ucLearnSkillIndex = (unsigned char)g_pLocalUser->GetLearnedSkillIndex(param2);

			return _XSendPacket_Encapsulate( (char*)&startreq );
		}
		break;
	case MSG_NO_MINIGAME_MAST_STAGE_C_S :
		{
			// 수련 계속, 종료
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : MiniGame Mast State C_S ====");

			int size = sizeof(MSG_MINIGAME_MAST_STAGE);
			MSG_MINIGAME_MAST_STAGE maststage;
			ZeroMemory(&maststage, size);
			maststage.header.sLength = size;
			maststage.ucMessage = type;

			maststage.iKey = g_pLocalUser->m_ServerKey;
			maststage.cMastStage = (char)param1;

			_XDWINBATTLELOG("=== ServerKey [%d]  MastStage [%d]", g_pLocalUser->m_ServerKey, param1);

			return _XSendPacket_Encapsulate( (char*)&maststage );
		}
		break;
	}

	return TRUE;
}

void _XNetwork::MSGMiniGameCtrl(MSG_MINIGAME_CTRL* minigamectrl)
{
	if(minigamectrl->sMiniGameID == 1)
	{
		// 운기 조식
		if(minigamectrl->usID == g_pLocalUser->m_Uniqueid)
		{
			_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
			if(pMeditation_Window)
			{
				if(minigamectrl->ucCtrl == minigame_en_allow_start)
				{
					_XDWINBATTLELOG("==== MiniGameCtrl [운기조식] START LocalUser");

					pMeditation_Window->m_bCameraMoveStart = TRUE;
					g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_3;
				}
				else if(minigamectrl->ucCtrl == minigame_en_not_allow_or_finish)
				{
					if(minigamectrl->skillID == _MODE_END_TRAINING_SELF_CANCEL)
					{

						_XDWINBATTLELOG("==== MiniGameCtrl [운기조식] CANCEL LocalUser");

						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						g_pLocalUser->ResetTrainingMode();

						if( !pMeditation_Window->m_bQuestMiniGameStart )
						{
							if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
							{
								if(g_pJinStatus_Window)
								{
									g_pJinStatus_Window->SetButtonStatus(TRUE);
								}
							}
						}
					}
					else if(minigamectrl->skillID == _MODE_END_TRAINING_COMPLETE)
					{
						_XDWINBATTLELOG("==== MiniGameCtrl [운기조식] END LocalUser");

						pMeditation_Window->Complete();
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_6;
						
						// 퀘스트로 인한 운기조식 실행이 완료 되었을 경우
						if( pMeditation_Window->m_bQuestMiniGameStart )
						{
							_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
							if(pTrainingHelp_Window)
							{
								pTrainingHelp_Window->m_EndButton->EnableWindow(TRUE);
							}
						}
						
					}
				}
				else if(minigamectrl->ucCtrl == minigame_en_cancel_from_monster)
				{
					_XDWINPRINT("==== MiniGameCtrl [운기조식] CancelFromMonster LocalUser");

					pMeditation_Window->ShowWindow(FALSE);
					pMeditation_Window->Reset();
					g_pLocalUser->ResetTrainingMode();
					
					if( !pMeditation_Window->m_bQuestMiniGameStart )
					{
						if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
						{
							if(g_pJinStatus_Window)
							{
								g_pJinStatus_Window->SetButtonStatus(TRUE);
							}
						}
					}
				}
			}
		}
	} // 운기 조식
	else if(minigamectrl->sMiniGameID == 2)
	{
		// 더미 치기
		if(minigamectrl->usID == g_pLocalUser->m_Uniqueid)
		{
			_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
			if(pDummy_Window)
			{
				pDummy_Window->m_bWaitForServerRes = FALSE;

				if(minigamectrl->ucCtrl == minigame_en_allow_start)
				{
					_XDWINBATTLELOG("==== MiniGameCtrl [더미치기] START LocalUser");
				}
				else if(minigamectrl->ucCtrl == minigame_en_not_allow_or_finish)
				{
					if(minigamectrl->skillID == _MODE_END_TRAINING_SELF_CANCEL)
					{
						_XDWINBATTLELOG("==== MiniGameCtrl [더미치기] CANCEL LocalUser");

						pDummy_Window->ShowWindow(FALSE);
						pDummy_Window->Reset();
						g_pLocalUser->ResetTrainingMode();
					}
					else if(minigamectrl->skillID == _MODE_END_TRAINING_COMPLETE)
					{
						_XDWINBATTLELOG("==== MiniGameCtrl [더미치기] END LocalUser");

						pDummy_Window->Complete();
						pDummy_Window->ShowWindow(FALSE);
						pDummy_Window->Reset();
//						g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_5;
//						g_pLocalUser->ResetTrainingMode();
					}
				}
				else if(minigamectrl->ucCtrl == minigame_en_cancel_from_monster)
				{
					_XDWINBATTLELOG("==== MiniGameCtrl [더미치기] CANCEL LocalUser");
					
					pDummy_Window->ShowWindow(FALSE);
					pDummy_Window->Reset();
					g_pLocalUser->ResetTrainingMode();
				}
			}
		}
	}
	else if(minigamectrl->sMiniGameID == 4)
	{
		// 기공 수련
		if(minigamectrl->usID == g_pLocalUser->m_Uniqueid)
		{
			_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
			if(pSpell_Window)
			{
				pSpell_Window->m_bWaitForServerRes = FALSE;
				if(minigamectrl->ucCtrl == minigame_en_allow_start)
				{
					_XDWINBATTLELOG("==== MiniGameCtrl [기공수련] START LocalUser");
				}
				else if(minigamectrl->ucCtrl == minigame_en_not_allow_or_finish)
				{
					if(minigamectrl->skillID == _MODE_END_TRAINING_SELF_CANCEL)
					{
						_XDWINBATTLELOG("==== MiniGameCtrl [기공수련] CANCEL LocalUser");

						pSpell_Window->ShowWindow(FALSE);
						pSpell_Window->Reset();
						g_pLocalUser->ResetTrainingMode();
					}
					else if(minigamectrl->skillID == _MODE_END_TRAINING_COMPLETE)
					{
						_XDWINBATTLELOG("==== MiniGameCtrl [기공수련] END LocalUser");

						pSpell_Window->Complete();
						pSpell_Window->ShowWindow(FALSE);
						pSpell_Window->Reset();
						g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_6;
					}
				}
				else if(minigamectrl->ucCtrl == minigame_en_cancel_from_monster)
				{
					_XDWINBATTLELOG("==== MiniGameCtrl [더미치기] CANCELfromMonster LocalUser");
					
					pSpell_Window->ShowWindow(FALSE);
					pSpell_Window->Reset();
					g_pLocalUser->ResetTrainingMode();
				}
			}
		}
	} // 기공 수련
}

void _XNetwork::MSGMiniGameMissionStage(MSG_MINIGAME_MISSION_STAGE* missionstage)
{
	g_pLocalUser->m_ServerKey = missionstage->iKey;
	
	switch(g_pLocalUser->m_SelectedTrainingMode)
	{
	case _XTRAINING_MEDITATION :
		{
			// 운기 조식
			_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
			if(pMeditation_Window)
			{
				if(missionstage->cMissionStage == 1)
				{
					// 시작
					_XDWINBATTLELOG("==== MiniGameMissionStage [운기조식] State : %d  Key : %d", missionstage->cMissionStage, missionstage->iKey);
					
					pMeditation_Window->m_MissionStage = missionstage->cMissionStage;
				}
				else if(missionstage->cMissionStage == 2 || missionstage->cMissionStage == 3)
				{
					// 반복적으로 오는 패킷
					_XDWINBATTLELOG("==== MiniGameMissionStage [운기조식] State : %d  Key : %d", missionstage->cMissionStage, missionstage->iKey);

					pMeditation_Window->m_MissionStage = missionstage->cMissionStage;
				}
			}
		}
		break;
	case _XTRAINING_DUMMY :
		{
			// 더미치기
			_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
			if(pDummy_Window)
			{
				pDummy_Window->m_bWaitForServerRes = FALSE;

				if(missionstage->cMissionStage == 1)
				{
					// 시작
					_XDWINBATTLELOG("==== MiniGameMissionStage [더미치기] State : %d  Key : %d", missionstage->cMissionStage, missionstage->iKey);

					pDummy_Window->m_bTrainingStart = TRUE;
					g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_3;
				}
				else if(missionstage->cMissionStage == 2 || missionstage->cMissionStage == 3)
				{
					// 다음 성 시작
					_XDWINBATTLELOG("==== MiniGameMissionStage [더미치기] State : %d  Key : %d", missionstage->cMissionStage, missionstage->iKey);

					pDummy_Window->Upgrade();
				}
			}
		}
		break;
	case _XTRAINING_SPELL :
		{
			// 기공수련
			_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
			if(pSpell_Window)
			{
				pSpell_Window->m_bWaitForServerRes = FALSE;

				if(missionstage->cMissionStage == 1)
				{
					// 시작
					_XDWINBATTLELOG("==== MiniGameMissionStage [기공수련] State : %d  Key : %d", missionstage->cMissionStage, missionstage->iKey);

					g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_3;
				}
				else if(missionstage->cMissionStage == 2 || missionstage->cMissionStage == 3)
				{
					// 다음 성 시작
					_XDWINBATTLELOG("==== MiniGameMissionStage [기공수련] State : %d  Key : %d", missionstage->cMissionStage, missionstage->iKey);

					pSpell_Window->Upgrade();
				}
			}
		}
		break;
	}
}

void _XNetwork::MSGMiniGameFail(MSG_MINIGAME_FAIL* minigamefail)
{
	switch(minigamefail->ucCode)
	{
	case _FAIL_CODE_HAVENT_SKILL :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2362), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

			if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_DUMMY)
			{
				_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
				if(pDummy_Window)
				{
					pDummy_Window->Reset();
					pDummy_Window->ShowWindow(FALSE);
				}
			}
			else if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_SPELL)
			{
				_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
				if(pSpell_Window)
				{
					pSpell_Window->Reset();
					pSpell_Window->ShowWindow(FALSE);
				}
			}
			else if(g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_MEDITATION)
			{
				_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
				if(pMeditation_Window)
				{
					if( !pMeditation_Window->m_bQuestMiniGameStart )
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
						
						if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
						{
							if(g_pJinStatus_Window)
							{
								g_pJinStatus_Window->SetButtonStatus(TRUE);
							}
						}
					}
					else
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
					}
				}	

			}
			g_pLocalUser->ResetTrainingMode();
		}
		break;
	case _FAIL_CODE_BED_KEY :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2363), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

			if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_DUMMY)
			{
				_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
				if(pDummy_Window)
				{
					pDummy_Window->Reset();
					pDummy_Window->ShowWindow(FALSE);
				}
			}
			else if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_SPELL)
			{
				_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
				if(pSpell_Window)
				{
					pSpell_Window->Reset();
					pSpell_Window->ShowWindow(FALSE);
				}
			}
			else if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
			{
				_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
				if(pMeditation_Window)
				{
					if( !pMeditation_Window->m_bQuestMiniGameStart )
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
						
						if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
						{
							if(g_pJinStatus_Window)
							{
								g_pJinStatus_Window->SetButtonStatus(TRUE);
							}
						}
					}
					else
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
					}
				}	
			}
			g_pLocalUser->ResetTrainingMode();
		}
		break;
	case _FAIL_CODE_NEED_LAST_LEVEL :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2364), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			
			if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
			{
				_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
				if(pMeditation_Window)
				{
					if( !pMeditation_Window->m_bQuestMiniGameStart )
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
						
						if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
						{
							if(g_pJinStatus_Window)
							{
								g_pJinStatus_Window->SetButtonStatus(TRUE);
							}
						}
					}
					else
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
					}
				}	
			}
			g_pLocalUser->ResetTrainingMode();
		}
		break;
	case _FAIL_CODE_NEED_FULL_POWER :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2365), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			
			if(g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
			{
				_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
				if(pMeditation_Window)
				{
					if( !pMeditation_Window->m_bQuestMiniGameStart )
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
						
						if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
						{
							if(g_pJinStatus_Window)
							{
								g_pJinStatus_Window->SetButtonStatus(TRUE);
							}
						}
					}
					else
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
						pMeditation_Window->OpenInterface();
					}
				}	
			}
			g_pLocalUser->ResetTrainingMode();
		}
		break;
	}
}

void _XNetwork::MSGMiniGameResult(MSG_MINIGAME_RESULT* minigameresult)
{
	
}

void _XNetwork::MSGEffectCtrlResultMySpell(MSG_EFFECT_CTRL_RESULT_MY_SPELL* effectctrl)
{
	switch(effectctrl->cMode)
	{
	case en_eff_ctrl_rs_add_skill :
		{
			if(!_XSkillItem::FindSkillProperty(effectctrl->id))
				break;
			if(effectctrl->cStep < 0 || effectctrl->cStep >= 12)
				break;
			
			// 이펙트 존재여부 검사 
			if( g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effect )
			{
				short effectcount = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effectCount;
				for(int i = 0 ; i < effectcount ; ++i)
				{
					// state 추가
					g_pLocalUser->m_StateList.InsertStateFromSpell(effectctrl->id, effectctrl->cStep, effectctrl->lefttimesec, i);

					// Stat 계산
					short id = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effect[i].id;
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
				if(g_SkillProperty[effectctrl->id]->visualEffect > 0)
				{
					D3DXMATRIX offsetmat;
					D3DXMatrixIdentity(&offsetmat);
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
						g_SkillProperty[effectctrl->id]->visualEffect, &offsetmat); //, NULL, TRUE);
				}
				
				// icon 추가
				if(g_pState_Window)
				{
					g_pState_Window->InsertStateListFromOther(_XST_SKILL, effectctrl->id, effectctrl->cStep);
				}
				
				if(g_BattleLog.GetWriteLog())
				{
//					g_BattleLog.SetTime();
//					g_BattleLog.SetSkillId(effectctrl->id, effectctrl->cStep);
					g_BattleLog.SetEffect(effectctrl->id, effectctrl->cStep);
//					g_BattleLog.DumpLogEffect();
				}

				_XDWINPRINT("effect add skill : %d", effectctrl->id);
			}
		}
		break;
	case en_eff_ctrl_rs_update_skill :
		{
			if(!_XSkillItem::FindSkillProperty(effectctrl->id))
				break;
			if(effectctrl->cStep < 0 || effectctrl->cStep >= 12)
				break;

			short effectcount = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effectCount;
			for(int i = 0 ; i < effectcount ; ++i)
			{
				// state Update
				g_pLocalUser->m_StateList.UpdateStateFromSpell(effectctrl->id, effectctrl->cStep, effectctrl->lefttimesec, i);

				// Stat 계산
				short id = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effect[i].id;
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
				}
			}

			// icon 추가
			if(g_pState_Window)
			{
				g_pState_Window->InsertStateListFromOther(_XST_SKILL, effectctrl->id, effectctrl->cStep);
			}

			if(g_BattleLog.GetWriteLog())
			{
//				g_BattleLog.SetTime();
//				g_BattleLog.SetSkillId(effectctrl->id, effectctrl->cStep);
				g_BattleLog.SetEffect(effectctrl->id, effectctrl->cStep);
//				g_BattleLog.DumpLogEffect();
			}

			_XDWINPRINT("effect update skill : %d", effectctrl->id);
		}
		break;
	case en_eff_ctrl_rs_del_skill :
		{
			if(!_XSkillItem::FindSkillProperty(effectctrl->id))
				break;
			if(effectctrl->cStep < 0 || effectctrl->cStep >= 12)
				break;

			short effectcount = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effectCount;
			for(int i = 0 ; i < effectcount ; ++i)
			{
				// state 삭제
				g_pLocalUser->m_StateList.DeleteStateFromSpell(effectctrl->id, effectctrl->cStep, i);

				// Stat 계산
				
				if( g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effect ) // 이펙트 존재여부 검사 
				{
					short id = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effect[i].id;
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
					if(iter_table != g_CharacterStateTable.end())
					{
						if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
						{
							g_pLocalUser->UpdateLocalUserAttackLevel();
						}
					}
				}
			}

			// visual effect 삭제
			if(g_SkillProperty[effectctrl->id]->visualEffect > 0)
			{
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[effectctrl->id]->visualEffect);
			}

			// icon 삭제
			if(g_pState_Window)
			{
				g_pState_Window->DeleteStateList(_XST_SKILL, effectctrl->id);
			}
			
			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(messagestring));
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2366), g_SkillProperty[effectctrl->id]->skillName);
			InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);

			_XDWINPRINT("effect delete skill : %d", effectctrl->id);
		}
		break;
	case en_eff_ctrl_rs_del_skill_rq_fail :
		{
			if(!_XSkillItem::FindSkillProperty(effectctrl->id))
				break;
			if(effectctrl->cStep < 0 || effectctrl->cStep >= 12)
				break;

			short effectcount = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->cStep].effectCount;
			for(int i = 0 ; i < effectcount ; ++i)
			{
				g_pLocalUser->m_StateList.UpdateStateFromSpell(effectctrl->id, effectctrl->cStep, effectctrl->lefttimesec, i);
			}

			// icon 추가
			if(g_pState_Window)
			{
				g_pState_Window->InsertStateListFromOther(_XST_SKILL, effectctrl->id, effectctrl->cStep);
			}
		}
		break;
	case en_eff_ctrl_rs_del_all_buff :		// 모든 buff effect 삭제
		{
			g_pLocalUser->m_StateList.DeleteAllBuff();
			if(g_pState_Window)
			{
				g_pState_Window->DeleteAllBuffStateList();
			}
			g_pLocalUser->UpdateLocalUserAttackLevel();

			_XDWINPRINT("effect all buff delete skill");
		}
		break;
	case en_eff_ctrl_rs_del_all_hostile :	// 모든 hostile effect 삭제
		{
			g_pLocalUser->m_StateList.DeleteAllHostile();
			if(g_pState_Window)
			{
				g_pState_Window->DeleteAllHostileStateList();
			}
			g_pLocalUser->UpdateLocalUserAttackLevel();

			_XDWINPRINT("effect all hostile delete skill");
		}
		break;
	}
}


void _XNetwork::MSGEffectCtrlResultMyEffect(MSG_EFFECT_CTRL_RESULT_MY_EFFECT* effectctrl)
{
	switch(effectctrl->cMode)
	{
	case en_eff_ctrl_rs_add_effect :
		{
			g_pLocalUser->m_StateList.InsertStateFromEffect(g_pLocalUser->m_Uniqueid, effectctrl->id, effectctrl->lefttimesec, effectctrl->value);

			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectctrl->id);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_FEED)
				{
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
			}
						
			//_XLog( "Effect insert : %d %d %d", effectctrl->id, effectctrl->lefttimesec, effectctrl->value );
			
			// icon 추가
			if(g_pState_Window)
			{
				g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectctrl->id, (char)-1);
			}
		}
		break;
	case en_eff_ctrl_rs_update_effect :
		{
			g_pLocalUser->m_StateList.UpdateStateFromEffect(g_pLocalUser->m_Uniqueid, effectctrl->id, effectctrl->lefttimesec, effectctrl->value);

			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectctrl->id);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_FEED)
				{
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
			}

			//_XLog( "Effect update : %d %d %d", effectctrl->id, effectctrl->lefttimesec, effectctrl->value );
			
			// icon 추가
			if(g_pState_Window)
			{
				g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectctrl->id, (char)-1);
			}
		}
		break;
	case en_eff_ctrl_rs_del_effect :
		{
			g_pLocalUser->m_StateList.DeleteStateFromEffect(g_pLocalUser->m_Uniqueid, effectctrl->id);

			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectctrl->id);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_FEED)
				{
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}

				//_XLog( "Effect delete : %d %d %d", effectctrl->id, effectctrl->lefttimesec, effectctrl->value );

				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2366), g_CharacterStateTable[effectctrl->id]->name);
				InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
			}
			
			// icon 삭제
			if(g_pState_Window)
			{
				g_pState_Window->DeleteStateList(_XST_EFFECT, effectctrl->id, -1);
			}		
		}
		break;
	case en_eff_ctrl_rs_del_effect_rq_fail :
		{
			g_pLocalUser->m_StateList.UpdateStateFromEffect(g_pLocalUser->m_Uniqueid, effectctrl->id, effectctrl->lefttimesec, effectctrl->value);
			
			_XLog( "Effect fail: %d %d %d", effectctrl->id, effectctrl->lefttimesec, effectctrl->value );

			// icon 추가
			if(g_pState_Window)
			{
				g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectctrl->id, -1);
			}
		}
		break;
	case en_eff_ctrl_rs_del_all_effect :		// 모든 condition 삭제
		{
			g_pLocalUser->m_StateList.DeleteAllEffect(g_pLocalUser->m_Uniqueid);
			if(g_pState_Window)
			{
				g_pState_Window->DeleteAllEffect();
			}
			g_pLocalUser->UpdateLocalUserAttackLevel();

			//_XLog( "Effect delete all : %d %d %d", effectctrl->id, effectctrl->lefttimesec, effectctrl->value );
		}
		break;
	case en_eff_ctrl_rs_update_value_only : //id에 해당하는 효과 값만 업데이트, 시간은 유지
		{
			g_pLocalUser->m_StateList.UpdateStateFromEffect(g_pLocalUser->m_Uniqueid, effectctrl->id, effectctrl->lefttimesec, effectctrl->value);

			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectctrl->id);
			if(iter_table != g_CharacterStateTable.end())
			{
				if( g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE ||
					g_CharacterStateTable[effectctrl->id]->effect_prop == _XCL_EFFECT_PROP_FEED)
				{
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
			}			
			
			// icon 추가
			if(g_pState_Window)
			{
				g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectctrl->id, (char)-1);
			}
		}
		break;
	}
}

void _XNetwork::MSGEffectCtrlResultOther(MSG_EFFECT_CTRL_RESULT_OTHER* effectctrl)
{
	switch(effectctrl->cMode)
	{
	case en_eff_ctrl_rs_other_add_skill :
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectctrl->uid);
			if(pUser)
			{
				if(!_XSkillItem::FindSkillProperty(effectctrl->id))
					break;
				if(effectctrl->step < 0 || effectctrl->step >= 12)
					break;

				short effectcount = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->step].effectCount;
				for(int i = 0 ; i < effectcount ; ++i)
				{
					pUser->m_StateList.InsertStateFromSpell(effectctrl->id, effectctrl->step, 0, i);	// lefttime은 알수 없음
				}

				if(g_SkillProperty[effectctrl->id]->visualEffect > 0)
				{
					if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_SkillProperty[effectctrl->id]->visualEffect))
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
							g_SkillProperty[effectctrl->id]->visualEffect, &offsetmat, NULL, TRUE);
					}
				}
			}
		}
		break;
	case en_eff_ctrl_rs_other_del_skill :
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectctrl->uid);
			if(pUser)
			{
				if(!_XSkillItem::FindSkillProperty(effectctrl->id))
					break;
				if(effectctrl->step < 0 || effectctrl->step >= 12)
					break;

				short effectcount = g_SkillProperty[effectctrl->id]->detailInfo[effectctrl->step].effectCount;
				for(int i = 0 ; i < effectcount ; ++i)
				{
					pUser->m_StateList.DeleteStateFromSpell(effectctrl->id, effectctrl->step, i);
				}

				if(g_SkillProperty[effectctrl->id]->visualEffect > 0)
				{
					pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[effectctrl->id]->visualEffect);
				}
			}
		}
		break;
	case en_eff_ctrl_rs_other_del_all_skill :		// 모든 skill 효과 삭제
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectctrl->uid);
			if(pUser)
			{
				pUser->m_StateList.DeleteAllSkill(pUser->m_Uniqueid);
			}
		}
		break;
	case en_eff_ctrl_rs_other_skill_res :			// reserved
		break;
	case en_eff_ctrl_rs_other_add_effect :
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectctrl->uid);
			if(pUser)
			{
				pUser->m_StateList.InsertStateFromEffect(pUser->m_Uniqueid, effectctrl->id, 0, 0);	// lefttime, value 알수없음
			}
		}
		break;
	case en_eff_ctrl_rs_other_del_effect :
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectctrl->uid);
			if(pUser)
			{
				pUser->m_StateList.DeleteStateFromEffect(pUser->m_Uniqueid, effectctrl->id);
			}
		}
		break;
	case en_eff_ctrl_rs_other_del_all_effect :		// 모든 condition 삭제
		{
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectctrl->uid);
			if(pUser)
			{
				pUser->m_StateList.DeleteAllEffect(pUser->m_Uniqueid);
			}
		}
		break;
	case en_eff_ctrl_rs_other_effect_res :			// reserved
		break;
	}
}

BOOL _XNetwork::Send_MSGEffectCtrl(int type, int param1, int param2, int param3)
{
	int size = sizeof(MSG_EFFECT_CTRL_REQUEST);
	MSG_EFFECT_CTRL_REQUEST effectctrlreq;
	ZeroMemory(&effectctrlreq, size);
	effectctrlreq.header.sLength = size;
	effectctrlreq.ucMessage = MSG_NO_EFFECT_CTRL;

	effectctrlreq.cMode = (char)param1;
	effectctrlreq.id = (short)param2;
	
	VIEWPACKETLOG
		_XDWINPRINT("Mode : %d    Spell ID : %d ", effectctrlreq.cMode, effectctrlreq.id);

	return _XSendPacket_Encapsulate( (char*)&effectctrlreq );
}

void _XNetwork::MSGEffectMyBHInfo(MSG_EFFECT_MY_BH_INFO* effectmybhinfo)
{
	for(int i = 0 ; i < effectmybhinfo->cCount ; ++i)
	{
		if(!_XSkillItem::FindSkillProperty(effectmybhinfo->entry[i].skillID))
			continue;
		if(effectmybhinfo->entry[i].skillLevel < 0 || effectmybhinfo->entry[i].skillLevel >= 12)
			continue;

		short effectcount = g_SkillProperty[effectmybhinfo->entry[i].skillID]->detailInfo[effectmybhinfo->entry[i].skillLevel].effectCount;
		for(int j = 0 ; j < effectcount ; ++j)
		{
			g_pLocalUser->m_StateList.InsertStateFromSpell(effectmybhinfo->entry[i].skillID, effectmybhinfo->entry[i].skillLevel, 
															effectmybhinfo->entry[i].usLeftTimeSec, j);

			// Stat 계산
			if( g_SkillProperty[effectmybhinfo->entry[i].skillID]->detailInfo[effectmybhinfo->entry[i].skillLevel].effect ) // 이펙트 존재여부 검사 
			{
				short id = g_SkillProperty[effectmybhinfo->entry[i].skillID]->detailInfo[effectmybhinfo->entry[i].skillLevel].effect[j].id;
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
				}
			}
		}

		// effect 추가
		if(g_SkillProperty[effectmybhinfo->entry[i].skillID]->visualEffect > 0)
		{
			if(!g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_SkillProperty[effectmybhinfo->entry[i].skillID]->visualEffect))
			{
				D3DXMATRIX offsetmat;
				D3DXMatrixIdentity(&offsetmat);
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager,
					g_SkillProperty[effectmybhinfo->entry[i].skillID]->visualEffect, &offsetmat, NULL, TRUE);
			}
		}

		// icon 추가
		if(g_pState_Window)
		{
			g_pState_Window->InsertStateListFromOther(_XST_SKILL, effectmybhinfo->entry[i].skillID, effectmybhinfo->entry[i].skillLevel);
		}

		if(g_BattleLog.GetWriteLog())
		{
//			g_BattleLog.SetTime();
//			g_BattleLog.SetSkillId(effectmybhinfo->entry[i].skillID, effectmybhinfo->entry[i].skillLevel);
			g_BattleLog.SetEffect(effectmybhinfo->entry[i].skillID, effectmybhinfo->entry[i].skillLevel);
			g_BattleLog.DumpLogEffect();
		}
	}
}

void _XNetwork::MSGEffectMyEntryInfo(MSG_EFFECT_MY_ENTRY_INFO* effectmyentryinfo)
{
	for(int i = 0 ; i < effectmyentryinfo->cCount; ++i)
	{
		g_pLocalUser->m_StateList.InsertStateFromEffect(g_pLocalUser->m_Uniqueid, effectmyentryinfo->entry[i].id, 
			effectmyentryinfo->entry[i].usLeftTimeSec, effectmyentryinfo->entry[i].value);

		map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectmyentryinfo->entry[i].id);
		if(iter_table != g_CharacterStateTable.end())
		{
			if(g_CharacterStateTable[effectmyentryinfo->entry[i].id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
				g_CharacterStateTable[effectmyentryinfo->entry[i].id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE ||
				g_CharacterStateTable[effectmyentryinfo->entry[i].id]->effect_prop == _XCL_EFFECT_PROP_FEED)
			{
				g_pLocalUser->UpdateLocalUserAttackLevel();
			}
		}

		// icon 추가
		if(g_pState_Window)
		{
			g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectmyentryinfo->entry[i].id, (char)-1);
		}
	}
}

void _XNetwork::MSGEffectOtherInfo(MSG_EFFECT_OTHER_INFO* effectotherinfo)
{
	// 다른 유저의 상태효과 처리
	_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectotherinfo->usID);

	BOOL bInsertVisualEffect = TRUE;

	if(pUser)
	{
#ifdef _XTS_TARGET_STATE
		if( g_pLocalUser->m_Uniqueid != effectotherinfo->usID )
		{
			pUser->m_StateList.DeleteAllEffect(effectotherinfo->usID);
			pUser->m_StateList.DeleteAllSkill(effectotherinfo->usID);
		}	
#endif
		for(int i = 0 ; i < effectotherinfo->cCount ; ++i)
		{
			if(!_XSkillItem::FindSkillProperty(effectotherinfo->id[i]))
				continue;

			short effectcount = g_SkillProperty[effectotherinfo->id[i]]->detailInfo[0].effectCount;
			for(int j = 0 ; j < effectcount ; ++j)
			{
				int index = pUser->m_StateList.FindState(en_from_skill, g_SkillProperty[effectotherinfo->id[i]]->detailInfo[0].effect[j].id, effectotherinfo->id[i]);
				if(index >= 0)
				{
					pUser->m_StateList.UpdateStateFromSpell(effectotherinfo->id[i], 0, 0, j);
					bInsertVisualEffect = FALSE;
				}
				else
				{
					pUser->m_StateList.InsertStateFromSpell(effectotherinfo->id[i], 0, 0, j);	// step, lefttime 알수 없음
					bInsertVisualEffect = TRUE;
				}
			}

			// effect 추가
			if(g_SkillProperty[effectotherinfo->id[i]]->visualEffect > 0)
			{
				if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_SkillProperty[effectotherinfo->id[i]]->visualEffect))
				{
					if(bInsertVisualEffect)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
							g_SkillProperty[effectotherinfo->id[i]]->visualEffect, &offsetmat, NULL, TRUE);
					}
				}
			}
		} // for(int i = 0 ; i < effectotherinfo->cCount ; i++) 
	}
}

void _XNetwork::MSGEffectOtherEntryInfo(MSG_EFFECT_OTHER_INFO* effectotherinfo)
{
	_XUser* pUser = (_XUser*)g_Userlist.FindUser(effectotherinfo->usID);
	if(pUser)
	{
		for(int i = 0 ; i < effectotherinfo->cCount ; ++i)
		{
			pUser->m_StateList.InsertStateFromEffect(pUser->m_Uniqueid, effectotherinfo->id[i], 0, 0);			
		}		
	}
}

void _XNetwork::MSGEffectMySpecialInfo(MSG_EFFECT_SPECIAL_BUFF_INFO* pEffectSpecialInfo)
{
	for( int i = 0; i < pEffectSpecialInfo->cCount; i++ )
	{
		if(!_XSkillItem::FindSkillProperty(pEffectSpecialInfo->entry[i].skillID))
			continue;
		if(pEffectSpecialInfo->entry[i].skillLevel < 0 || pEffectSpecialInfo->entry[i].skillLevel >= 12)
			continue;

		short effectcount = g_SkillProperty[pEffectSpecialInfo->entry[i].skillID]->detailInfo[pEffectSpecialInfo->entry[i].skillLevel].effectCount;
		for( int j = 0; j < effectcount; j++ )
		{
			g_pLocalUser->m_StateList.InsertStateFromSpecialBuff( g_pLocalUser->m_Uniqueid, 
																pEffectSpecialInfo->entry[i].skillID, 
																pEffectSpecialInfo->entry[i].skillLevel,
																pEffectSpecialInfo->entry[i].slot,
																pEffectSpecialInfo->entry[i].usLeftTimeSec, j);
																  
			// Stat 계산
			if( g_SkillProperty[pEffectSpecialInfo->entry[i].skillID]->detailInfo[pEffectSpecialInfo->entry[i].skillLevel].effect ) // 이펙트 존재여부 검사 
			{
				short id = g_SkillProperty[pEffectSpecialInfo->entry[i].skillID]->detailInfo[pEffectSpecialInfo->entry[i].skillLevel].effect[j].id;
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
				}
			}
		}
		
		// effect 추가
		if(g_SkillProperty[pEffectSpecialInfo->entry[i].skillID]->visualEffect > 0)
		{
			if(!g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_SkillProperty[pEffectSpecialInfo->entry[i].skillID]->visualEffect))
			{
				D3DXMATRIX offsetmat;
				D3DXMatrixIdentity(&offsetmat);
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager,
					g_SkillProperty[pEffectSpecialInfo->entry[i].skillID]->visualEffect, &offsetmat, NULL, TRUE);
			}
		}

		// icon 추가
		if(g_pState_Window)
		{
			g_pState_Window->InsertStateListFromOther(_XST_SPECIALBUFF, pEffectSpecialInfo->entry[i].skillID, pEffectSpecialInfo->entry[i].slot, pEffectSpecialInfo->entry[i].skillLevel);
		}
	}
}

void _XNetwork::MSGEffectCtrlResultExtra(MSG_EFFECT_CTRL_RESULT_EXTRA* effectextra)
{
	switch(effectextra->cMode)
	{
	case en_eff_ctrl_rs_add_extra :
		{
			if(effectextra->slot == 3)
			{
				short skillid = 0;
				switch(effectextra->id)
				{
				case 256 :
					{
						skillid = 8042;		//안전 버프
						g_pLocalUser->m_InsideSafetyZoneIndex = 1;
					}
					break;
				case 257 :
					{
						skillid = 8043;		// 경범자
						g_pLocalUser->m_InsideSafetyZoneIndex = 2;
					}
					break;
				case 258 :
					{
						skillid = 8044;		// 중범자
						g_pLocalUser->m_InsideSafetyZoneIndex = 3;
					}
					break;
				}
				
				// 안전 지역 버프 etc.
				if(!_XSkillItem::FindSkillProperty(skillid))
					break;
				
				if(g_SkillProperty[skillid]->detailInfo[0].effect)
				{
					short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
					for(int i = 0 ; i < effectcount ; ++i)
					{
						// state 추가
						g_pLocalUser->m_StateList.InsertStateFromExtraSkill(g_pLocalUser->m_Uniqueid, skillid, 0, effectextra->slot, effectextra->usLeftTimeSec, i);

						// stat 계산
						short id = g_SkillProperty[skillid]->detailInfo[0].effect[i].id;
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
								g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
							{
								g_pLocalUser->UpdateLocalUserAttackLevel();
							}
						}
					}

					// visual effect 추가
					if(g_SkillProperty[skillid]->visualEffect > 0)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
							g_SkillProperty[skillid]->visualEffect, &offsetmat); //, NULL, TRUE);
					}

					// icon 추가
					if(g_pState_Window)
					{
						g_pState_Window->InsertStateListFromOther(_XST_EXTRASKILL, skillid, effectextra->slot);
					}

					// message
					switch(skillid)
					{
					case 8042 : // 안전 지역 버프
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3032), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
						break;
					case 8043 : // 경범법자
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3038), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
						break;
					case 8044 :	// 중범법자
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3040), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
						break;
					}
				}
			}
			else
			{
				g_pLocalUser->m_StateList.InsertStateFromExtra(g_pLocalUser->m_Uniqueid, effectextra->id, effectextra->slot, 
					effectextra->usLeftTimeSec, effectextra->value);

				// icon 추가 //Author : 양희왕 breif : 대만 버프 아이콘 표시 08/02/22
/*#ifdef	_XTAIWANESE 
				if( effectextra->slot != 1 )// 대만일때는 버프 아이콘을 표시하지 않는다.
				{
					if(g_pState_Window)
					{
						g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectextra->id, effectextra->slot);
					}		
				}
#else*/
				if(g_pState_Window)
				{
					g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectextra->id, effectextra->slot);
				}
//#endif
				
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectextra->id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[effectextra->id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
						g_CharacterStateTable[effectextra->id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
				}
			}
		}
		break;
	case en_eff_ctrl_rs_update_extra :
		{
			if(effectextra->slot == 3)
			{
				short skillid = 0;
				switch(effectextra->id)
				{
				case 256 :
					{
						skillid = 8042;		//안전 버프
						g_pLocalUser->m_InsideSafetyZoneIndex = 1;
					}
					break;
				case 257 :
					{
						skillid = 8043;		// 경범자
						g_pLocalUser->m_InsideSafetyZoneIndex = 2;
					}
					break;
				case 258 :
					{
						skillid = 8044;		// 중범자
						g_pLocalUser->m_InsideSafetyZoneIndex = 3;
					}
					break;
				}

				// 안전 지역 버프 etc.
				if(!_XSkillItem::FindSkillProperty(skillid))
					break;
				
				int oldskillid = g_pLocalUser->m_StateList.FindStateExtraSkill(en_from_extraskill, effectextra->slot);
				if(oldskillid == skillid)
				{
					// update
					short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
					for(int i = 0 ; i < effectcount ; ++i)
					{
						// state update
						g_pLocalUser->m_StateList.UpdateStateFromExtraSkill(g_pLocalUser->m_Uniqueid, skillid, 0, effectextra->slot, effectextra->usLeftTimeSec, i);

						// stat 계산
						short id = g_SkillProperty[skillid]->detailInfo[0].effect[i].id;
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
								g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
							{
								g_pLocalUser->UpdateLocalUserAttackLevel();
							}
						}
					}

					//icon 추가
					if(g_pState_Window)
					{
						g_pState_Window->InsertStateListFromOther(_XST_EXTRASKILL, skillid, effectextra->slot);
					}
				}
				else
				{
					// 삭제 후 다시 추가

					// 삭제 ------------------------------------------------------------------------┓
					if(oldskillid > 0)
					{
						short effectcount = g_SkillProperty[oldskillid]->detailInfo[0].effectCount;
						for(int i = 0 ; i < effectcount ; ++i)
						{
							// state 삭제
							g_pLocalUser->m_StateList.DeleteStateFromExtraSkill(g_pLocalUser->m_Uniqueid, oldskillid, 0, i, effectextra->slot);

							// stat 계산
							if(g_SkillProperty[oldskillid]->detailInfo[0].effect)
							{
								short id = g_SkillProperty[oldskillid]->detailInfo[0].effect[i].id;
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
										g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
									{
										g_pLocalUser->UpdateLocalUserAttackLevel();
									}
								}
							}
						}

						// visual effect 삭제
						if(g_SkillProperty[oldskillid]->visualEffect > 0)
						{
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[oldskillid]->visualEffect);
						}

						// icon 삭제
						if(g_pState_Window)
						{
							g_pState_Window->DeleteStateList(_XST_EXTRASKILL, oldskillid, effectextra->slot);
						}
					}
					// -----------------------------------------------------------------------------┛

					// 추가 ------------------------------------------------------------------------┓
					if(g_SkillProperty[skillid]->detailInfo[0].effect)
					{
						short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
						for(int i = 0 ; i < effectcount ; ++i)
						{
							// state 추가
							g_pLocalUser->m_StateList.InsertStateFromExtraSkill(g_pLocalUser->m_Uniqueid, skillid, 0, effectextra->slot, effectextra->usLeftTimeSec, i);

							// stat 계산
							short id = g_SkillProperty[skillid]->detailInfo[0].effect[i].id;
							map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
							if(iter_table != g_CharacterStateTable.end())
							{
								if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
									g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
								{
									g_pLocalUser->UpdateLocalUserAttackLevel();
								}
							}
						}

						// visual effect 추가
						if(g_SkillProperty[skillid]->visualEffect > 0)
						{
							D3DXMATRIX offsetmat;
							D3DXMatrixIdentity(&offsetmat);
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								g_SkillProperty[skillid]->visualEffect, &offsetmat); //, NULL, TRUE);
						}

						// icon 추가
						if(g_pState_Window)
						{
							g_pState_Window->InsertStateListFromOther(_XST_EXTRASKILL, skillid, effectextra->slot);
						}
					}
					// -----------------------------------------------------------------------------┛
				}
			}
			else
			{
				g_pLocalUser->m_StateList.UpdateStateFromExtra(g_pLocalUser->m_Uniqueid, effectextra->id, effectextra->slot, 
					effectextra->usLeftTimeSec, effectextra->value);
			
		// icon 추가 //Author : 양희왕 breif : 대만 버프 아이콘 표시 08/02/22
/*#ifdef	_XTAIWANESE // 대만일때는 버프 아이콘을 표시하지 않는다.
				if( effectextra->slot != 1 )// 대만일때는 버프 아이콘을 표시하지 않는다.
				{
					if(g_pState_Window)
					{
						g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectextra->id, effectextra->slot);
					}		
				}		
#else*/
				if(g_pState_Window)
				{
					g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectextra->id, effectextra->slot);
				}
//#endif

				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectextra->id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[effectextra->id]->effect_prop == _XCL_EFFECT_PROP_GENERAL || 
						g_CharacterStateTable[effectextra->id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
				}
			}
		}
		break;
	case en_eff_ctrl_rs_del_extra :
		{
			if(effectextra->slot == 3)
			{
				short skillid = 0;
				switch(effectextra->id)
				{
				case 256 :
					skillid = 8042;		//안전 버프
					break;
				case 257 :
					skillid = 8043;		// 경범자
					break;
				case 258 :
					skillid = 8044;		// 중범자
					break;
				}

				// 안전 지역 버프 etc.
				if(!_XSkillItem::FindSkillProperty(skillid))
					break;

				g_pLocalUser->m_InsideSafetyZoneIndex = 0;

				short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
				for(int i = 0 ; i < effectcount ; ++i)
				{
					// state 삭제
					g_pLocalUser->m_StateList.DeleteStateFromExtraSkill(g_pLocalUser->m_Uniqueid, skillid, 0, i, effectextra->slot);

					// stat 계산
					if(g_SkillProperty[skillid]->detailInfo[0].effect)
					{
						short id = g_SkillProperty[skillid]->detailInfo[0].effect[i].id;
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
								g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
							{
								g_pLocalUser->UpdateLocalUserAttackLevel();
							}
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
					g_pState_Window->DeleteStateList(_XST_EXTRASKILL, skillid, effectextra->slot);
				}

				// message
				switch(skillid)
				{
				case 8042 : // 안전 지역 버프
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3034), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
					break;
				}
			}
			else
			{
				g_pLocalUser->m_StateList.DeleteStateFromExtra(g_pLocalUser->m_Uniqueid, effectextra->id, effectextra->slot);

				// icon 삭제
				if(g_pState_Window)
				{
					g_pState_Window->DeleteStateList(_XST_EFFECT, effectextra->id, effectextra->slot);
				}

				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectextra->id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[effectextra->id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
						g_CharacterStateTable[effectextra->id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}

					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2366), g_CharacterStateTable[effectextra->id]->name);
					InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				}
			}
		}
		break;
	}
}

void _XNetwork::MSGEffectMyExtraInfo(MSG_EFFECT_MY_EXTRA_INFO* effectextra)
{
	for(int i = 0 ; i < effectextra->cCount; i++)
	{
		if(effectextra->entry[i].cWhere == 3)
		{
			short skillid = 0;
			switch(effectextra->entry[i].id)
			{
			case 256 :
				{
					skillid = 8042;		//안전 버프
					g_pLocalUser->m_InsideSafetyZoneIndex = 1;
				}
				break;
			case 257 :
				{
					skillid = 8043;		// 경범자
					g_pLocalUser->m_InsideSafetyZoneIndex = 2;
				}
				break;
			case 258 :
				{
					skillid = 8044;		// 중범자
					g_pLocalUser->m_InsideSafetyZoneIndex = 3;
				}
				break;
			}
			
			// 안전 지역 버프 etc.
			if(!_XSkillItem::FindSkillProperty(skillid))
				break;
			
			if(g_SkillProperty[skillid]->detailInfo[0].effect)
			{
				short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
				for(int jj = 0 ; jj < effectcount ; ++jj)
				{
					// state 추가
					g_pLocalUser->m_StateList.InsertStateFromExtraSkill(g_pLocalUser->m_Uniqueid, skillid, 0, 
						effectextra->entry[i].cWhere, effectextra->entry[i].usLeftTimeSec, jj);

					// stat 계산
					short id = g_SkillProperty[skillid]->detailInfo[0].effect[jj].id;
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
					if(iter_table != g_CharacterStateTable.end())
					{
						if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
							g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
						{
							g_pLocalUser->UpdateLocalUserAttackLevel();
						}
					}
				}

				// visual effect 추가
				if(g_SkillProperty[skillid]->visualEffect > 0)
				{
					D3DXMATRIX offsetmat;
					D3DXMatrixIdentity(&offsetmat);
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
						g_SkillProperty[skillid]->visualEffect, &offsetmat); //, NULL, TRUE);
				}

				// icon 추가
				if(g_pState_Window)
				{
					g_pState_Window->InsertStateListFromOther(_XST_EXTRASKILL, skillid, effectextra->entry[i].cWhere);
				}

				// message
				switch(skillid)
				{
				case 8042 : // 안전 지역 버프
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3032), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
					break;
				case 8043 : // 경범법자
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3038), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
					break;
				case 8044 :	// 중범법자
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3040), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
					break;
				}
			}
		}
		else
		{
			g_pLocalUser->m_StateList.InsertStateFromExtra(g_pLocalUser->m_Uniqueid, effectextra->entry[i].id, effectextra->entry[i].cWhere,
				effectextra->entry[i].usLeftTimeSec, effectextra->entry[i].value);

			// icon 추가 //Author : 양희왕 breif : 대만 버프 아이콘 표시 08/02/22
/*#ifdef	_XTAIWANESE // 대만일때는 버프 아이콘을 표시하지 않는다.
			if( effectextra->entry[i].cWhere != 1 )// 대만일때는 버프 아이콘을 표시하지 않는다.
			{
				if(g_pState_Window)
				{
					g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectextra->entry[i].id, effectextra->entry[i].cWhere);
				}		
			}		
#else*/
			if(g_pState_Window)
			{
				g_pState_Window->InsertStateListFromOther(_XST_EFFECT, effectextra->entry[i].id, effectextra->entry[i].cWhere);
			}
//#endif
			
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectextra->entry[i].id);
			if(iter_table != g_CharacterStateTable.end())
			{
				if(g_CharacterStateTable[effectextra->entry[i].id]->effect_prop == _XCL_EFFECT_PROP_GENERAL || 
					g_CharacterStateTable[effectextra->entry[i].id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
				{
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
			}
		}
	}
}

void _XNetwork::MSGEffectCtrlResultAddSpecial(MSG_EFFECT_CTRL_RESULT_ADD_SPECIAL* addspecial)
{
//Author : 양희왕 //breif : 수정
	// 장원전 버프
	if(!_XSkillItem::FindSkillProperty(addspecial->id))
		return;
	
	if(g_SkillProperty[addspecial->id]->detailInfo[0].effect)
	{
		short effectcount = g_SkillProperty[addspecial->id]->detailInfo[0].effectCount;
		for(int jj = 0 ; jj < effectcount ; ++jj)
		{
			// state 추가
			g_pLocalUser->m_StateList.InsertStateFromSpecialBuff(g_pLocalUser->m_Uniqueid, addspecial->id, addspecial->level, addspecial->slot, addspecial->usLeftSecond, jj);

			// stat 계산
			short id = g_SkillProperty[addspecial->id]->detailInfo[0].effect[jj].id;
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
			if(iter_table != g_CharacterStateTable.end())
			{
				if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
					g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
				{
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
			}
		}

		// visual effect 추가
		if(g_SkillProperty[addspecial->id]->visualEffect > 0)
		{
			D3DXMATRIX offsetmat;
			D3DXMatrixIdentity(&offsetmat);
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
				g_SkillProperty[addspecial->id]->visualEffect, &offsetmat); //, NULL, TRUE);
		}

		// icon 추가
		if(g_pState_Window)
		{
			g_pState_Window->InsertStateListFromOther(_XST_SPECIALBUFF, addspecial->id, addspecial->slot, addspecial->level);
		}
	}
}

void _XNetwork::MSGEffectCtrlResultUpdateSpecial(MSG_EFFECT_CTRL_RESULT_UPDATE_SPECIAL* pUpdateSpecial)
{
//Author : 양희왕 //breif : 수정
	if(!_XSkillItem::FindSkillProperty(pUpdateSpecial->id))
		return;
	
	// 같은 slot에 effect가 있으면 먼저 삭제한다.
	int skillid = 0, skilllevel = 0;
	int index = g_pLocalUser->m_StateList.FindStateSpecialBuff(pUpdateSpecial->slot, skillid, skilllevel);
	if(index >= 0)
	{
		if(!_XSkillItem::FindSkillProperty(skillid))
			return;

		short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
		for(int i = 0 ; i < effectcount ; ++i)
		{
			// state 삭제
			g_pLocalUser->m_StateList.DeleteStateFromSpecialBuff(g_pLocalUser->m_Uniqueid, skillid, skilllevel, i, pUpdateSpecial->slot);

			// stat 계산
			if(g_SkillProperty[skillid]->detailInfo[0].effect)
			{
				short id = g_SkillProperty[skillid]->detailInfo[0].effect[i].id;
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
						g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
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
			g_pState_Window->DeleteStateList(_XST_SPECIALBUFF, skillid, pUpdateSpecial->slot);
		}
	}

	//삭제
	/*if( pUpdateSpecial->id >= 8054 && pUpdateSpecial->id <= 8057 )
	{
		for( int index = 8054; index <= 8057; index++ )
		{
			short effectcount = g_SkillProperty[index]->detailInfo[0].effectCount;
			
			for(int i = 0 ; i < effectcount ; i++)
			{
				g_pLocalUser->m_StateList.DeleteStateFromSpecialBuff(g_pLocalUser->m_Uniqueid, index, 0, i, 1);

				// stat 계산
				/*short id = g_SkillProperty[index]->detailInfo[0].effect[i].id;
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
						g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
				}
			}

			if(g_SkillProperty[index]->visualEffect > 0)
			{
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[index]->visualEffect);
			}

			if(g_pState_Window)
			{
				g_pState_Window->DeleteStateList(_XST_SPECIALBUFF, index, 1);
			}
		}
	} // 삭제*/
	
	// update
	short effectcount = g_SkillProperty[pUpdateSpecial->id]->detailInfo[0].effectCount;
	for(int i = 0 ; i < effectcount ; ++i)
	{
		// state update
		g_pLocalUser->m_StateList.InsertStateFromSpecialBuff(g_pLocalUser->m_Uniqueid, pUpdateSpecial->id, pUpdateSpecial->level, pUpdateSpecial->slot, pUpdateSpecial->usLeftSecond, i);
		
		// stat 계산
		short id = g_SkillProperty[pUpdateSpecial->id]->detailInfo[0].effect[i].id;
		map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
		if(iter_table != g_CharacterStateTable.end())
		{
			if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
				g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
			{
				g_pLocalUser->UpdateLocalUserAttackLevel();
			}
		}
	}

	// visual effect 추가
	if(g_SkillProperty[pUpdateSpecial->id]->visualEffect > 0)
	{
		D3DXMATRIX offsetmat;
		D3DXMatrixIdentity(&offsetmat);
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
			g_SkillProperty[pUpdateSpecial->id]->visualEffect, &offsetmat); //, NULL, TRUE);
	}
	//icon 추가
	if(g_pState_Window)
	{
		g_pState_Window->InsertStateListFromOther(_XST_SPECIALBUFF, pUpdateSpecial->id, pUpdateSpecial->slot, pUpdateSpecial->level);
	}
}

void _XNetwork::MSGEffectCtrlResultDelSpecial(MSG_EFFECT_CTRL_RESULT_DEL_SPECIAL* delspecial)
{
//Author : 양희왕 //breif : 수정

	int skillid = 0, skilllevel = 0;
	int index = g_pLocalUser->m_StateList.FindStateSpecialBuff(delspecial->slot, skillid, skilllevel);
	if(index >= 0)
	{
		if(!_XSkillItem::FindSkillProperty(skillid))
			return;

		short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
		for(int i = 0 ; i < effectcount ; ++i)
		{
			// state 삭제
			g_pLocalUser->m_StateList.DeleteStateFromSpecialBuff(g_pLocalUser->m_Uniqueid, skillid, skilllevel, i, delspecial->slot);

			// stat 계산
			if(g_SkillProperty[skillid]->detailInfo[0].effect)
			{
				short id = g_SkillProperty[skillid]->detailInfo[0].effect[i].id;
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
				if(iter_table != g_CharacterStateTable.end())
				{
					if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
						g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE)
					{
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
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
			g_pState_Window->DeleteStateList(_XST_SPECIALBUFF, skillid, delspecial->slot);
		}
	}
}

void _XNetwork::MSGNPCBuffResult(MSG_NPC_BUFF_RESULT* npcbuffresult)
{
	if(npcbuffresult->ucResult == 0)
	{
//author : 양희왕
		_XWindow_NPCBuff* pNPCBuff_Window = (_XWindow_NPCBuff*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCBUFF);
		if(pNPCBuff_Window)
		{
			_XNPCBUFFSTR* pNPCBuff = NULL;
			_map_NPCBuffStr::iterator iter_buff = g_NPCBuffStr.find(pNPCBuff_Window->m_SelectedItem);
			if(iter_buff != g_NPCBuffStr.end())
			{
				pNPCBuff = iter_buff->second;
			}

			if(pNPCBuff)
			{
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
 
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2367), pNPCBuff->name);
				InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

				DWORD needMoney = g_pLocalUser->m_Money - npcbuffresult->uiMoney;
				if(needMoney > 0)
				{
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2368), needMoney);
					InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
				}
			}
		}
	}

	else if(npcbuffresult->ucResult == 1)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NPCTRADE_NOTENOUGHMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
	}
	else if(npcbuffresult->ucResult == 2)
	{
		_XWindow_NPCBuff* pNPCBuff_Window = (_XWindow_NPCBuff*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCBUFF);

		_XNPCBUFFSTR* pNPCBuff = NULL;
		_map_NPCBuffStr::iterator iter_buff = g_NPCBuffStr.find(pNPCBuff_Window->m_SelectedItem);
		if(iter_buff != g_NPCBuffStr.end())
		{
			pNPCBuff = iter_buff->second;
		}

		if(pNPCBuff)
		{
			switch( pNPCBuff->type )
			{
			case 1 :
			case 2 :
			case 3 :
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3348), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			case 4 :
			#if defined(_XVIETNAMESE)
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3481), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			#else
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2369), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			#endif
				break;
			case 5 :
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3481), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			case 6 :
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2369), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}
		}
	}
	else if(npcbuffresult->ucResult == 4)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3809), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//소지하고 있는 무명 신침이 없습니다.
	}
//Last Updated : 07/03/13

	g_pLocalUser->m_Money = npcbuffresult->uiMoney;
}

void _XNetwork::MSGEffectCtrlResult(MSG_EFFECT_CTRL_RESULT* effectctrl)
{
	if(effectctrl->cMode == en_eff_ctrl_rs_cancel_current)
	{
		if(g_pLocalUser->m_SelectedSkillID > 0)
		{
			if( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF			 &&
			g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
			{
				// 경공 - 무시
			}
			else
			{
				if(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
				{
					// 경공이 아닌 나머지 무공 시전중
					SendPacket(MSG_NO_MARTIAL_END);
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2603), g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillName);	
					InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
			}
		}
	}
	else if(effectctrl->cMode == en_eff_ctrl_rs_cancel_hyperrun)
	{
		if( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF			 &&
			g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
		{
			// 경공일 경우 취소
//			SendPacket(MSG_NO_MARTIAL_END);

			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(messagestring));
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_2603), g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillName);
			InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

			g_pLocalUser->ResetSkillAnimation();
			g_pLocalUser->m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
//			g_pLocalUser->SetAnimation( _XACTION_IDLE );
			g_pLocalUser->m_UseSpeedSkill = FALSE;
			
			if( g_pLocalUser->m_SelectedSkillID )
			{
				if(	g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
					_XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID) == _XSI_TARGETTYPE_SELF			 &&
					g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
				{
					g_pLocalUser->m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
					g_pLocalUser->m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
				}
			}
		}
	}
}

// pk 전적
void _XNetwork::MSGPKWinLoseRecord(MSG_PK_WIN_LOSE_RECORD* record)
{
	if(g_pPKRecord_Window)
	{
		g_pPKRecord_Window->ResetRecord();

		g_pPKRecord_Window->m_TotalWinCount = record->iKillCount;
		g_pPKRecord_Window->m_TotalLoseCount = record->iDeadCount;

		if(g_pPKRecord_Window->m_TotalWinCount + g_pPKRecord_Window->m_TotalLoseCount > 0)
		{
			FLOAT rate = ((FLOAT)g_pPKRecord_Window->m_TotalWinCount / 
						(g_pPKRecord_Window->m_TotalWinCount+g_pPKRecord_Window->m_TotalLoseCount))*100.f;

			g_pPKRecord_Window->m_TotalWinRate = (int)rate;
		}

		for(int i = 0 ; i < record->ucWinCount ; ++i)
		{
			g_pPKRecord_Window->m_WinRecord[i].option = record->pkRecord[i].uiOption;
			g_pPKRecord_Window->m_WinRecord[i].year = record->pkRecord[i].uiYear;
			g_pPKRecord_Window->m_WinRecord[i].month = record->pkRecord[i].uiMonth;
			g_pPKRecord_Window->m_WinRecord[i].day = record->pkRecord[i].uiDay;
			g_pPKRecord_Window->m_WinRecord[i].hour = record->pkRecord[i].uiHour;
			g_pPKRecord_Window->m_WinRecord[i].minute = record->pkRecord[i].uiMinute;

			strcpy(g_pPKRecord_Window->m_WinRecord[i].name, record->pkRecord[i].name);
			g_pPKRecord_Window->m_WinRecord[i].group = record->pkRecord[i].cMoonpa;
		}

		for(i = 0 ; i < record->ucLoseCount ; ++i)
		{
			g_pPKRecord_Window->m_LoseRecord[i].option = record->pkRecord[i+record->ucWinCount].uiOption;
			g_pPKRecord_Window->m_LoseRecord[i].year = record->pkRecord[i+record->ucWinCount].uiYear;
			g_pPKRecord_Window->m_LoseRecord[i].month = record->pkRecord[i+record->ucWinCount].uiMonth;
			g_pPKRecord_Window->m_LoseRecord[i].day = record->pkRecord[i+record->ucWinCount].uiDay;
			g_pPKRecord_Window->m_LoseRecord[i].hour = record->pkRecord[i+record->ucWinCount].uiHour;
			g_pPKRecord_Window->m_LoseRecord[i].minute = record->pkRecord[i+record->ucWinCount].uiMinute;
			
			strcpy(g_pPKRecord_Window->m_LoseRecord[i].name, record->pkRecord[i+record->ucWinCount].name);
			g_pPKRecord_Window->m_LoseRecord[i].group = record->pkRecord[i+record->ucWinCount].cMoonpa;
		}
	}
}

void _XNetwork::MSGPKMoonpaRecord(MSG_PK_MOONPA_RECORD* record)
{
	if(g_pPKRecord_Window)
	{
		for(int i = 0 ; i < 9 ; ++i)
		{
			g_pPKRecord_Window->m_GroupRecord[i].win = record->MoonpaRecord[i].iWin;
			g_pPKRecord_Window->m_GroupRecord[i].lose = record->MoonpaRecord[i].iLose;
		}
	}
}

void _XNetwork::MSGPKClassRecord(MSG_PK_CLASS_RECORD* record)
{
	if(g_pPKRecord_Window)
	{
		for(int i = 0 ; i < 8 ; ++i)
		{
			g_pPKRecord_Window->m_ClassRecord[i].win = record->ClassRecord[i].iWin;
			g_pPKRecord_Window->m_ClassRecord[i].lose = record->ClassRecord[i].iLose;
		}
	}
}

void _XNetwork::MSGPKUpdateWin(MSG_PK_UPDATE_WIN* record)
{
	if(g_pPKRecord_Window)
	{
		g_pPKRecord_Window->m_TotalWinCount = record->iKillCount;

		if(g_pPKRecord_Window->m_TotalWinCount + g_pPKRecord_Window->m_TotalLoseCount > 0)
		{
			FLOAT rate = ((FLOAT)g_pPKRecord_Window->m_TotalWinCount / 
				(g_pPKRecord_Window->m_TotalWinCount+g_pPKRecord_Window->m_TotalLoseCount))*100.f;
			
			g_pPKRecord_Window->m_TotalWinRate = (int)rate;
		}

		g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].option = record->pkRecordEntry.uiOption;
		g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].year = record->pkRecordEntry.uiYear;
		g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].month = record->pkRecordEntry.uiMonth;
		g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].day = record->pkRecordEntry.uiDay;
		g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].hour = record->pkRecordEntry.uiHour;
		g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].minute = record->pkRecordEntry.uiMinute;
		
		strcpy(g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].name, record->pkRecordEntry.name);
		g_pPKRecord_Window->m_WinRecord[record->ucSlotNo].group = record->pkRecordEntry.cMoonpa;
	}

}

void _XNetwork::MSGPKUpdateLose(MSG_PK_UPDATE_LOSE* record)
{
	if(g_pPKRecord_Window)
	{
		g_pPKRecord_Window->m_TotalLoseCount = record->iDeadCount;

		if(g_pPKRecord_Window->m_TotalWinCount + g_pPKRecord_Window->m_TotalLoseCount > 0)
		{
			FLOAT rate = ((FLOAT)g_pPKRecord_Window->m_TotalWinCount / 
				(g_pPKRecord_Window->m_TotalWinCount+g_pPKRecord_Window->m_TotalLoseCount))*100.f;
			
			g_pPKRecord_Window->m_TotalWinRate = (int)rate;
		}
		
		g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].option = record->pkRecordEntry.uiOption;
		g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].year = record->pkRecordEntry.uiYear;
		g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].month = record->pkRecordEntry.uiMonth;
		g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].day = record->pkRecordEntry.uiDay;
		g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].hour = record->pkRecordEntry.uiHour;
		g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].minute = record->pkRecordEntry.uiMinute;
		
		strcpy(g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].name, record->pkRecordEntry.name);
		g_pPKRecord_Window->m_LoseRecord[record->ucSlotNo].group = record->pkRecordEntry.cMoonpa;
	}
}

void _XNetwork::MSGPKUpdateMoonpaWin(MSG_PK_UPDATE_MOONPA_WIN* record)
{
	if(g_pPKRecord_Window)
	{
		g_pPKRecord_Window->m_GroupRecord[record->cMoonpa].win = record->iValue;
	}
}

void _XNetwork::MSGPKUpdateMoonpaLose(MSG_PK_UPDATE_MOONPA_LOSE* record)
{
	if(g_pPKRecord_Window)
	{
		g_pPKRecord_Window->m_GroupRecord[record->cMoonpa].lose = record->iValue;
	}
}

void _XNetwork::MSGPKUpdateClassWin(MSG_PK_UPDATE_CLASS_WIN* record)
{
	if(g_pPKRecord_Window)
	{
		g_pPKRecord_Window->m_ClassRecord[record->cClass].win = record->iValue;
	}
}

void _XNetwork::MSGPKUpdateClassLose(MSG_PK_UPDATE_CLASS_LOSE* record)
{
	if(g_pPKRecord_Window)
	{
		g_pPKRecord_Window->m_ClassRecord[record->cClass].lose = record->iValue;
	}
}