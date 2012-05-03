// XLocalUser_Battle.h
//
// Author : Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XLocalUser.h"
#include "XParticleIDDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_MainMenu.h"

void _XLocalUser::ProcessBattle(void)
{
	ProcessCommonAction();

	char usefultype = 0;
	char targettype = 0;
	
	if(m_SelectedSkillID > 0)
	{
		usefultype = GetUsefulType(m_SelectedSkillID);		
		targettype = _XSkillItem::GetTargetType(m_SelectedSkillID); //g_SkillProperty[m_SelectedSkillID]->targetType;
	}
	else
		usefultype = _XSI_USEFULTYPE_ACTIVESKILL;
	
	switch(usefultype)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		ProcessBattleActiveSkill();
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{			
			bool checklps = false;
			if(m_AttackTarget)
			{
				if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if( pMob )
					{
						if( g_MobAttribTable[pMob->m_MobType].scripttype == 81 
							|| g_MobAttribTable[pMob->m_MobType].scripttype == 82) 
						{
							checklps = true;
						}
						
					}
				}
			}

			if(!checklps) 
			{
				// 이벤트 때문에 임시로 처리한 코드 : 끝나면 지워야 함.
				bool biscondition = true;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if( pMob->m_MobType == 1055 )
					{
						biscondition = false;
						if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, 380))
							biscondition = true;
						
						if( !biscondition )
							if( g_pState_Window->FindBuffStateList(_XST_EFFECT, 380))
								biscondition = true;
							
						if( !biscondition )
							if( g_pState_Window->FindHostileStateList(_XST_EFFECT, 380))
								biscondition = true;
					}
				}
				
				if( biscondition )
				{
					ProcessBattleChargeSkill();
					
				}
				else
				{					
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2905 ), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //_T("진 위장군의 기가 부족합니다.")
					SetAnimation(_XACTION_IDLE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
			else
			{
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1577), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
				ResetAttackTarget();
			}
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF)
			{
				ProcessBattleSelfCastingSpell();
			}
			else if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_POINT)
			{
				ProcessBattleAreaCastingSpell();
			}
			else
			{				
				bool checklps = false;
				if(m_AttackTarget)
				{
					if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						if( pMob )
						{
							
							if( g_MobAttribTable[pMob->m_MobType].scripttype == 81 
								|| g_MobAttribTable[pMob->m_MobType].scripttype == 82) 
								checklps = true;
						}
					}
				}
				
				if(!checklps) 
				{
					
				// 이벤트 때문에 임시로 처리한 코드 : 끝나면 지워야 함.
					bool biscondition = true;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						if( pMob->m_MobType == 1055 )
						{
							biscondition = false;
							if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, 380))
								biscondition = true;
							
							if( !biscondition )
								if( g_pState_Window->FindBuffStateList(_XST_EFFECT, 380))
									biscondition = true;
								
								if( !biscondition )
									if( g_pState_Window->FindHostileStateList(_XST_EFFECT, 380))
										biscondition = true;
						}
					}
					
					if( biscondition )
					{
						ProcessBattleTargetCastingSpell();
						
					}
					else
					{					
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2905 ), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //_T("진 위장군의 기가 부족합니다.")
						SetAnimation(_XACTION_IDLE);
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
				else
				{
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1577), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					ResetAttackTarget();
				}
			}
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			// process charnnelling spell
			if( g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공 처리
				ProcessSpeedSkill();
			else
			{
				if(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF)
				{
					ProcessBattleSelfChannelingSpell();
				}
				else if(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_POINT)
				{
					// 영역 공격
					 ProcessBattleAreaChannelingSpell();
				}
				else
				{
					ProcessBattleTargetChannelingSpell();
				}
			}
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			bool checklps = false;
			if(m_AttackTarget)
			{
				if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if( pMob )
					{						
						if( g_MobAttribTable[pMob->m_MobType].scripttype == 81 
							|| g_MobAttribTable[pMob->m_MobType].scripttype == 82) 
							checklps = true;
					}
				}
			}
			
			if(!checklps)
			{
				
				// 이벤트 때문에 임시로 처리한 코드 : 끝나면 지워야 함.
				bool biscondition = true;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if( pMob->m_MobType == 1055 )
					{
						biscondition = false;
						if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, 380))
							biscondition = true;
						
						if( !biscondition )
							if( g_pState_Window->FindBuffStateList(_XST_EFFECT, 380))
								biscondition = true;
							
							if( !biscondition )
								if( g_pState_Window->FindHostileStateList(_XST_EFFECT, 380))
									biscondition = true;
					}
				}
				
				if( biscondition )
				{
					ProcessBattleFinishSkill();
					
				}
				else
				{					
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2905 ), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//_T("진 위장군의 기가 부족합니다.")
					SetAnimation(_XACTION_IDLE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}			
			}
			else
			{
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1577), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
				ResetAttackTarget();
			}
		}
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF)
			{
				ProcessBattleSelfRoundingSpell();
			}
			else if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_POINT)
			{
				ProcessBattleAreaRoundingSpell();
			}
			else
			{
				
				bool checklps = false;
				if(m_AttackTarget)
				{
					if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						if( pMob )
						{
							if( g_MobAttribTable[pMob->m_MobType].scripttype == 81 
								|| g_MobAttribTable[pMob->m_MobType].scripttype == 82) 
								checklps = true;
						}
					}
				}
				
				if(!checklps) 
				{
				// 이벤트 때문에 임시로 처리한 코드 : 끝나면 지워야 함.
					bool biscondition = true;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						if( pMob->m_MobType == 1055 )
						{
							biscondition = false;
							if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, 380))
								biscondition = true;
							
							if( !biscondition )
								if( g_pState_Window->FindBuffStateList(_XST_EFFECT, 380))
									biscondition = true;
								
								if( !biscondition )
									if( g_pState_Window->FindHostileStateList(_XST_EFFECT, 380))
										biscondition = true;
						}
					}
					
					if( biscondition )
					{
						ProcessBattleTargetRoundingSpell();
						
					}
					else
					{					
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2905 ), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//_T("진 위장군의 기가 부족합니다.")
						SetAnimation(_XACTION_IDLE);
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}			
				}
				else
				{
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1577), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					ResetAttackTarget();
				}
			}
		}
		break;
	case _XSI_USEFULTYPE_HIDDENMA :
		ProcessBattleHiddenMA();
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
		{
			bool checklps = false;
			if(m_AttackTarget)
			{
				if( m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if( pMob )
					{
						if( g_MobAttribTable[pMob->m_MobType].scripttype == 81 
							|| g_MobAttribTable[pMob->m_MobType].scripttype == 82) 
							checklps = true;
					}
				}
			}
			if(!checklps) 
			{
				
				// 이벤트 때문에 임시로 처리한 코드 : 끝나면 지워야 함.
				bool biscondition = true;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if( pMob->m_MobType == 1055 )
					{
						biscondition = false;
						if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, 380))
							biscondition = true;
						
						if( !biscondition )
							if( g_pState_Window->FindBuffStateList(_XST_EFFECT, 380))
								biscondition = true;
							
							if( !biscondition )
								if( g_pState_Window->FindHostileStateList(_XST_EFFECT, 380))
									biscondition = true;
					}
				}
				
				if( biscondition )
				{
					ProcessBattleSmashingSkill();
					
				}
				else
				{					
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2905 ), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//_T("진 위장군의 기가 부족합니다.")
					SetAnimation(_XACTION_IDLE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}				
			}
			else
			{
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1577), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
				ResetAttackTarget();
			}
		}
		break;
	case _XSI_USEFULTYPE_ACTIONSKILL :
		{
			ProcessBattleActionSkill();
		}
		break;
	default :
		ProcessNormalAction();
		break;
	}

	// cool down time 계산
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(m_SkillList[i].m_sSkillID == 0)
			continue;

		if(m_SkillList[i].m_dwCooldownTime > 0)
		{
			if(m_SkillList[i].m_sSkillID != m_SelectedSkillID)
			{
				if(g_LocalSystemTime - m_SkillList[i].m_dwCooldownStartTime > m_SkillList[i].m_dwCooldownTime && m_SkillList[i].m_dwCooldownStartTime > 0)
				{
//					_XDWINPRINT("cooldown : %d  / elapsedtime : %d", m_SkillList[i].m_dwCooldownTime, g_LocalSystemTime - m_SkillList[i].m_dwCooldownStartTime);

					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SkillList[i].m_sSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SkillList[i].m_dwCooldownStartTime = 0;
				}
			}
			else
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL 
					&& _XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF
					&& g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03)	// 경공
				{
					if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime && m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
				else if(m_BattleMode == _XBATTLE_PEACE || m_CurrentBattleState != _XUSER_STATE_NONE)
				{
					if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime && m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
#ifdef _XTS_PK
				else if(m_BattleMode != _XBATTLE_PEACE && m_CurrentBattleState == _XUSER_STATE_NONE)
#else
				else if(m_BattleMode == _XBATTLE_BATTLE && m_CurrentBattleState == _XUSER_STATE_NONE)
#endif
				{
					// 전투 도중 상태이상등으로 state가 초기화 된 경우
					if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime && m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
			}
		}
		else
		{
			// cool down이 0일 경우
			if(m_SkillList[i].m_dwCooldownStartTime > 0 && m_CurrentBattleState == _XUSER_STATE_NONE)
			{
				// 전투 도중 상태이상등으로 state가 초기화 된 경우
				m_SkillList[i].m_dwCooldownStartTime = 0;
			}
		}
	}
	
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	// cool down time 계산
	if(m_EventSkillList.m_sSkillID > 0)
	{
		if(m_EventSkillList.m_dwCooldownTime > 0)
		{
			if(m_EventSkillList.m_sSkillID != m_SelectedSkillID)
			{
				if(g_LocalSystemTime - m_EventSkillList.m_dwCooldownStartTime > m_EventSkillList.m_dwCooldownTime && 
					m_EventSkillList.m_dwCooldownStartTime > 0)
				{
//					_XDWINPRINT("cooldown : %d  / elapsedtime : %d", m_EventSkillList.m_dwCooldownTime, g_LocalSystemTime - m_EventSkillList.m_dwCooldownStartTime);

					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_EventSkillList.m_sSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_EventSkillList.m_dwCooldownStartTime = 0;
				}
			}
			else
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL 
					&& _XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF
					&& g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03)	// 경공
				{
					if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime && m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
				else if(m_BattleMode == _XBATTLE_PEACE || m_CurrentBattleState != _XUSER_STATE_NONE)
				{
					if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime && m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
#ifdef _XTS_PK
				else if(m_BattleMode != _XBATTLE_PEACE && m_CurrentBattleState == _XUSER_STATE_NONE)
#else
				else if(m_BattleMode == _XBATTLE_BATTLE && m_CurrentBattleState == _XUSER_STATE_NONE)
#endif
				{
					// 전투 도중 상태이상등으로 state가 초기화 된 경우
					if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime && m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
			}
		}
		else
		{
			// cool down이 0일 경우
			if(m_EventSkillList.m_dwCooldownStartTime > 0 && m_CurrentBattleState == _XUSER_STATE_NONE)
			{
				// 전투 도중 상태이상등으로 state가 초기화 된 경우
				m_EventSkillList.m_dwCooldownStartTime = 0;
			}
		}
	}
#endif

	// 경공 사용 예외 처리
	if( m_UseSpeedSkill ) // 경공 사용중 Alt키를 그만 누르는 경우
	{
		if(!gpInput->CheckKeyPress(DIK_LMENU))
		{
			/*if( !(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
					g_SkillProperty[m_SelectedSkillID]->targetType == _XSI_TARGETTYPE_SELF			 &&
					g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 ) )*/
			{
				ResetSkillAnimation();
				m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
				SetAnimation( _XACTION_IDLE );
				m_UseSpeedSkill = FALSE;

				if( m_SelectedSkillID )
				{
					if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
					{
						m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					}
				}
			}	
		}
	}
	else // 카메라 앵글 되돌리기
	{
		if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)
		{
			g_LodTerrain.m_3PCamera.mp_fFov -= _XDEF_FOVDECREASERATE;
			if(g_LodTerrain.m_3PCamera.mp_fFov <= g_DefaultCameraFov)
			{
				g_LodTerrain.m_3PCamera.mp_fFov = g_DefaultCameraFov;
			}
			
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);

			if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps > 3 )
			{
				g_ObjectBlurProcessManager.SetBlurStep( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps - 2 );
			}
			else
			{
				g_ObjectBlurProcessManager.SetBlurStep( 1 );
			}

		} // if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)
	}

	// 경공 사용시 충돌(바닥,물)하였을 경우 설정되는 경공 재시도 불가 플래그 복구(Alt를 떼었을시)
	if( m_HiperRunFailedFlag )
	{
		if( !gpInput->CheckKeyPress(DIK_LMENU) )
		{
			if( g_LodTerrain.m_3PCamera.mp_fFov == g_DefaultCameraFov ) // 카메라가 완전히 복구됬을때
			{
				m_HiperRunFailedFlag = FALSE;
			}
		}
	}
}

void _XLocalUser::ProcessBattleActiveSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_ModelDescriptor.m_LockEndFrame)
				m_ModelDescriptor.SetLockEndFrame(FALSE);
			
#ifdef _XTS_PK
			if(m_AttackTarget && (m_BattleMode == _XBATTLE_BATTLE || m_BattleMode == _XBATTLE_PK))
#else
			if(m_AttackTarget && m_BattleMode == _XBATTLE_BATTLE)
#endif
			{
				D3DXVECTOR3 targetposition;
				unsigned short targettype;
				FLOAT range = 0.0f;
				
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					targetposition = pMob->m_Position;
					targettype = pMob->m_MobType;
					
					if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
					{
						if( !ProcessLPSSystem(pMob) )
						{
							m_CurrentBattleState = _XUSER_STATE_2;
							SetSkillAnimation();
							SetAnimation(_XACTION_ATTACK);
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
					}
					else
					{
						SetTargetPosition(targetposition, range);
//						SetAnimation(_XACTION_MOVE);
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
                    //Author : 양희왕 //breif : 레벨차 공격 안되게
                    _XUser* pUser = NULL;
		            pUser = (_XUser*)m_AttackTarget;
                    if( !pUser )
                        break;

                    if(CheckPKPenalty(pUser))
                        break;
    
					targetposition = ((_XUser*)m_AttackTarget)->m_Position;
					targettype = 0;

					if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
					{
						if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == ((_XUser*)m_AttackTarget)->m_Uniqueid) ||
							(m_UserState == _XDEF_USERSTATE_PK_READY && ((_XUser*)m_AttackTarget)->m_bEnemy) ||
							(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget((_XUser*)m_AttackTarget)))
						{
							m_CurrentBattleState = _XUSER_STATE_2;
							SetSkillAnimation();
							SetAnimation(_XACTION_ATTACK);
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
					}
					else
					{
						SetTargetPosition(targetposition, range);
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
					if( pFunctionalObject->m_bAttackable )
					{
						targetposition = pFunctionalObject->m_Position;
						targettype = pFunctionalObject->m_ObjectType;
						
						if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
						{
							m_CurrentBattleState = _XUSER_STATE_2;
							SetSkillAnimation();
							SetAnimation(_XACTION_ATTACK);
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
						else
						{
							SetTargetPosition(targetposition, range);
						}
					}
					else
					{
						m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
						m_bCheckFirstFrame = FALSE;
						
						m_bUsingSkill = FALSE;
						m_bCheckDelayTime = TRUE;
						m_CurrentBattleState = _XUSER_STATE_LAST;
						
						// delay time 계산 시작
						m_ChangeDelayStartTime = g_LocalSystemTime;
						m_bAddDelayTime = TRUE;

						break;
					}
				}
				else
				{
					m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					m_bCheckFirstFrame = FALSE;
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					m_CurrentBattleState = _XUSER_STATE_LAST;
					
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
					m_bAddDelayTime = TRUE;

					break;
				}
			}
			else
			{
				// target이 없음
				_XDWINPRINT("No Target / checkfirstframe [%d]", m_bCheckFirstFrame);
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(GetMotionClass() != _XACTION_ATTACK)
			{
				m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
				m_bCheckFirstFrame = FALSE;

				m_bUsingSkill = FALSE;
				m_bCheckDelayTime = TRUE;
				m_CurrentBattleState = _XUSER_STATE_LAST;

				// delay time 계산 시작
				m_ChangeDelayStartTime = g_LocalSystemTime;
				break;
			}

			if(m_ModelDescriptor.m_pBipedController->GetFirstFrame()+5 >= m_ModelDescriptor.m_AniFrame &&
				m_ModelDescriptor.m_AniFrame > 0.0f && !m_bCheckFirstFrame)
			{
				if(m_AttackTarget)
				{
					_XDWINPRINT("--- CurrentAttackSequence : %d  /  motion index : %d Frame [%f]", m_CurrentAttackSequence, m_CharacterInfo.animationdescript.attacktype, m_ModelDescriptor.m_AniFrame);
					
					// 초식 시작
					m_bCheckFirstFrame = TRUE;

					if(g_BattleLog.GetWriteLog())
						g_BattleLog.SetTime();
					
					D3DXVECTOR3 targetposition;
					unsigned short targettype;
					FLOAT range = 0.0f;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						targetposition = ((_XMob*)m_AttackTarget)->m_Position;
						targettype = ((_XMob*)m_AttackTarget)->m_MobType;
					}
					else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					{
						targetposition = ((_XUser*)m_AttackTarget)->m_Position;
						targettype = 0;
					}
					else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
						if( pFunctionalObject->m_bAttackable )
						{
							targetposition = pFunctionalObject->m_Position;
							targettype = pFunctionalObject->m_ObjectType;
						}
						else
						{
							m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
							m_bCheckFirstFrame = FALSE;
							
							m_bUsingSkill = FALSE;
							m_bCheckDelayTime = TRUE;
							m_CurrentBattleState = _XUSER_STATE_LAST;
							
							// delay time 계산 시작
							m_ChangeDelayStartTime = g_LocalSystemTime;
							m_bAddDelayTime = TRUE;
							
							break;
						}
					}
					else
					{
						m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
						m_bCheckFirstFrame = FALSE;
						
						m_bUsingSkill = FALSE;
						m_bCheckDelayTime = TRUE;
						m_CurrentBattleState = _XUSER_STATE_LAST;
						
						// delay time 계산 시작
						m_ChangeDelayStartTime = g_LocalSystemTime;
						m_bAddDelayTime = TRUE;
						
						break;
					}

					m_CurrentAttackSequence = (_XUSER_ATTACKSEQUENCE)((int)m_CurrentAttackSequence + 1);
					//Author: 양희왕 //breif : 초식기 1단계만 나가게 하는 버그 수정 //Date : 07//11/06
					if( m_CurrentAttackSequence > _XDEF_ATK_SEQ_THIRD )
						m_CurrentAttackSequence = _XDEF_ATK_SEQ_THIRD;

					if(g_BattleLog.GetWriteLog())
					{
						g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
						g_BattleLog.SetTargetType(targettype);
						g_BattleLog.SetSkillId(m_SelectedSkillID);
						g_BattleLog.SetAttackSeq(m_CurrentAttackSequence);
					}

					if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
					{
						// 무조건 친다 - 거리 check하지 않음
						m_bUsingSkill = TRUE;

						g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, m_CurrentAttackSequence-1);
						
						if(m_SelectedSkillID > 0)
							m_AttackPointNumber = g_SkillProperty[m_SelectedSkillID]->attackPointFor1;
						else 
							m_AttackPointNumber = 2;
						m_CurrentAttackPoint = 0;
						
						// target 쪽으로 돌아선다.
						m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
						UpdatePosition();
						
						if(g_pMinimap_Window)
						{
							g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
							g_pMinimap_Window->SetLocalUserPosition();
						}
					}
					else
					{
						if(m_NextSkillID > 0)
						{
							short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
							if(motionindex > 0)
							{
								SetAnimationIndex(motionindex);
								m_ModelDescriptor.SetDrawSwordEffect(FALSE);
								m_CurrentBattleState = _XUSER_STATE_3;
							}
							else
							{
								// Queue에 쌓인 스킬이 있음 - 다음 스킬로 진행
								m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
								m_CurrentAttackPoint = 0;
								
								m_bUsingSkill = FALSE;
								m_bCheckDelayTime = TRUE;
								ResetSkillAnimation();
								
								// delay time 계산 시작
								m_ChangeDelayStartTime = g_LocalSystemTime;
							}
						}
						else
						{
							if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
							{
								// 공격
								m_bUsingSkill = TRUE;
								
								g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, m_CurrentAttackSequence-1);
								
								if(m_SelectedSkillID > 0)
									m_AttackPointNumber = g_SkillProperty[m_SelectedSkillID]->attackPointFor2 - g_SkillProperty[m_SelectedSkillID]->attackPointFor1;
								else
									m_AttackPointNumber = 2;
								m_CurrentAttackPoint = 0;
								
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
								UpdatePosition();
								
								if(g_pMinimap_Window)
								{
									g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
									g_pMinimap_Window->SetLocalUserPosition();
								}
							}
							else
							{
								// 따라간다
								m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
								m_bCheckFirstFrame = FALSE;
								
								m_bUsingSkill = FALSE;
								m_CurrentBattleState = _XUSER_STATE_1;
								m_ModelDescriptor.SetLockEndFrame(FALSE);
							}
						}
					}
				}
				else
				{
					// target 없음 - 보간 동작 실행하지 않음(Moving Attack 방지용)
					m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					m_bCheckFirstFrame = FALSE;
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					m_CurrentBattleState = _XUSER_STATE_LAST;
					
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
					m_bAddDelayTime = TRUE;
					
					m_ModelDescriptor.SetLockEndFrame(FALSE);
				}
			} // first frame
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
//				m_ModelDescriptor.SetLockEndFrame(TRUE);
				m_bCheckFirstFrame = FALSE;
				
				if(m_NextSkillID > 0)
				{
					short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
					if(motionindex >= 0)
					{
						SetAnimationIndex(motionindex);
						m_ModelDescriptor.SetDrawSwordEffect(FALSE);
						m_CurrentBattleState = _XUSER_STATE_3;
					}
					else
					{
						// Queue에 쌓인 스킬이 있음 - 다음 스킬로 진행
						m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
						m_CurrentAttackPoint = 0;
						
						m_bUsingSkill = FALSE;
						m_bCheckDelayTime = TRUE;
						ResetSkillAnimation();
						
						// delay time 계산 시작
						m_ChangeDelayStartTime = g_LocalSystemTime;
					}
				}
				else if(m_SelectedSkillID > 0)
				{
					if(m_MartialStep == 0)
					{
						short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
						if(motionindex >= 0)
						{
							SetAnimationIndex(motionindex);
							m_ModelDescriptor.SetDrawSwordEffect(FALSE);
							m_CurrentBattleState = _XUSER_STATE_3;
						}
						else
						{
							// 보간 없음 - state 넘기지 말고 그냥 끝
							m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
							m_bCheckFirstFrame = FALSE;
							
							m_bUsingSkill = FALSE;
							m_bCheckDelayTime = TRUE;
							ResetSkillAnimation();
							m_bRestartActive = TRUE;
							
							// delay time 계산 시작
							m_ChangeDelayStartTime = g_LocalSystemTime;
							
							if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
								ProcessMobDie();
						}
					}
					else if(m_MartialStep == 1)
					{
						if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
						{
							if(m_AttackTarget)
							{
								// 2초식 시전
								short motionindex = GetMotionIndex(m_SelectedSkillID);
								motionindex += _XANI_ORDER_REPEAT;
								SetAnimationIndex(motionindex);
							}
							else
							{
								short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
								if(motionindex >= 0)
								{
									SetAnimationIndex(motionindex);
									m_ModelDescriptor.SetDrawSwordEffect(FALSE);
									m_CurrentBattleState = _XUSER_STATE_3;
								}
								else
								{
									// 보간 없음 - state 넘기지 말고 그냥 끝
									m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
									m_bCheckFirstFrame = FALSE;
									
									m_bUsingSkill = FALSE;
									m_bCheckDelayTime = TRUE;
									ResetSkillAnimation();
									m_bRestartActive = TRUE;
									
									// delay time 계산 시작
									m_ChangeDelayStartTime = g_LocalSystemTime;
									
									if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
										ProcessMobDie();
								}
							}
						}
						else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
						{
							short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
							if(motionindex >= 0)
							{
								SetAnimationIndex(motionindex);
								m_ModelDescriptor.SetDrawSwordEffect(FALSE);
								m_CurrentBattleState = _XUSER_STATE_3;
							}
							else
							{
								// 보간 없음 - state 넘기지 말고 그냥 끝
								m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
								m_bCheckFirstFrame = FALSE;
								
								m_bUsingSkill = FALSE;
								m_bCheckDelayTime = TRUE;
								ResetSkillAnimation();
								m_bRestartActive = TRUE;
								
								// delay time 계산 시작
								m_ChangeDelayStartTime = g_LocalSystemTime;
								
								if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
									ProcessMobDie();
							}
						}
					}
					else if(m_MartialStep == 2)
					{
						if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
						{
							if(m_AttackTarget)
							{
								// 다음 초식 시전
								short motionindex = GetMotionIndex(m_SelectedSkillID);
								motionindex += _XANI_ORDER_REPEAT;
								SetAnimationIndex(motionindex);
							}
							else
							{
								short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
								if(motionindex >= 0)
								{
									SetAnimationIndex(motionindex);
									m_ModelDescriptor.SetDrawSwordEffect(FALSE);
									m_CurrentBattleState = _XUSER_STATE_3;
								}
								else
								{
									// 보간 없음 - state 넘기지 말고 그냥 끝
									m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
									m_bCheckFirstFrame = FALSE;
									
									m_bUsingSkill = FALSE;
									m_bCheckDelayTime = TRUE;
									ResetSkillAnimation();
									m_bRestartActive = TRUE;
									
									// delay time 계산 시작
									m_ChangeDelayStartTime = g_LocalSystemTime;
									
									if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
										ProcessMobDie();
								}
							}
						}
						else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
						{
							if(m_AttackTarget)
							{
								// 다음 초식 시전
								short motionindex = GetMotionIndex(m_SelectedSkillID);
								motionindex += _XANI_ORDER_FIRE;
								SetAnimationIndex(motionindex);
							}
							else
							{
								short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
								if(motionindex >= 0)
								{
									SetAnimationIndex(motionindex);
									m_ModelDescriptor.SetDrawSwordEffect(FALSE);
									m_CurrentBattleState = _XUSER_STATE_3;
								}
								else
								{
									// 보간 없음 - state 넘기지 말고 그냥 끝
									m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
									m_bCheckFirstFrame = FALSE;
									
									m_bUsingSkill = FALSE;
									m_bCheckDelayTime = TRUE;
									ResetSkillAnimation();
									m_bRestartActive = TRUE;
									
									// delay time 계산 시작
									m_ChangeDelayStartTime = g_LocalSystemTime;
									
									if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
										ProcessMobDie();
								}
							}
						}
						else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_THIRD)
						{
							short motionindex = GetInterpolateMotionIndex(m_SelectedSkillID);
							if(motionindex >= 0)
							{
								SetAnimationIndex(motionindex);
								m_ModelDescriptor.SetDrawSwordEffect(FALSE);
								m_CurrentBattleState = _XUSER_STATE_3;
							}
							else
							{
								// 보간 없음 - state 넘기지 말고 그냥 끝
								m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
								m_bCheckFirstFrame = FALSE;
								
								m_bUsingSkill = FALSE;
								m_bCheckDelayTime = TRUE;
								ResetSkillAnimation();
								m_bRestartActive = TRUE;
								
								// delay time 계산 시작
								m_ChangeDelayStartTime = g_LocalSystemTime;
								
								if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
									ProcessMobDie();
							}
						}
					}
				}
				else
				{
					// 기본 때리기 일때
					// 보간 없음 - state 넘기지 말고 그냥 끝
					m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					m_bCheckFirstFrame = FALSE;
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					ResetSkillAnimation();
					m_bRestartActive = TRUE;
					
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
					
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
						ProcessMobDie();
				}
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				if(!m_AttackTarget)
					break;

				if(m_AttackTargetObjectType != _XPICKINGTARGET_MOB && m_AttackTargetObjectType != _XPICKINGTARGET_PC && m_AttackTargetObjectType != _XPICKINGTARGET_FUNCTIONALOBJECT)
					break;

				if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
					if(!pFunctionalObject->m_bAttackable)
						break;
				}

				m_CurrentAttackPoint++;
//				m_bCheckFirstFrame = FALSE;

				VIEWPACKETLOG
					_XDWINPRINT("--- [COMMANDFRAME]Current Attack Point [%d/%f/motionindex:%d]", m_CurrentAttackPoint, m_ModelDescriptor.m_AniFrame, m_CharacterInfo.animationdescript.attacktype);

				// damage 계산
				short totaldamage = 0;
				if(m_CurrentAttackPoint == m_AttackPointNumber)
				{
					// 마지막 command frame
					for(int i = 0 ; i < 3 ; ++i)
					{
						// 적용이 안된 damage 모두 적용
						if(m_AttackResult.display[i])
						{
							if(m_AttackResult.sDamage[i] < 0)
								totaldamage = m_AttackResult.sDamage[i];
							else
								totaldamage += m_AttackResult.sDamage[i];
						}
						m_AttackResult.display[i] = FALSE;
					}
				}
				else
				{
					if(m_CurrentAttackPoint-1 >= 1 && m_AttackResult.display[m_CurrentAttackPoint-2])
					{
						// 2번째 이상 command frame
						if(m_AttackResult.sDamage[m_CurrentAttackPoint-1] < 0)
							totaldamage = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
						else if(m_AttackResult.sDamage[m_CurrentAttackPoint-2] < 0)
							totaldamage = m_AttackResult.sDamage[m_CurrentAttackPoint-2];
						else
							totaldamage = m_AttackResult.sDamage[m_CurrentAttackPoint-1] + m_AttackResult.sDamage[m_CurrentAttackPoint-2];
						m_AttackResult.display[m_CurrentAttackPoint-2] = m_AttackResult.display[m_CurrentAttackPoint-1] = FALSE;
					}
					else
					{
						totaldamage = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
						m_AttackResult.display[m_CurrentAttackPoint-1] = FALSE;
					}
				} // damage 계산 끝
				
				if(m_AttackResult.ucToType == en_target_type_monster)
				{
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						_XMob* pMob = NULL;
						pMob = (_XMob*)m_AttackTarget;
						if(!pMob)
							break;
						
						// 타격 이펙트 삽입
						D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;

						BOOL bIntersect = FALSE;
						D3DXVECTOR3 pickedpoint, originpos;
						D3DXVECTOR3 direction = pMob->m_Position - m_Position;
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
						
						// Monster Health 적용(Display용)
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;
						
						switch(m_AttackResult.usAtkInfo[m_CurrentAttackPoint-1])
						{
						case 0 :
							{
								// HIT
								//int iTotalDamage = totaldamage;

								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
																	
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );

									}										
									else
									{
										// Visual Effect - 첫타에 한번만 출력(타)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
										}
									}
								}								

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] HIT damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);

                                	g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								pMob->m_AttackTarget = g_pLocalUser;
								pMob->m_AttackTargetObjectType = _XPICKINGTARGET_SELF;

								pMob->SetBlowMotion();
								pMob->StartImpactEffect();

								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pMob->m_MobType;

								PlayAttackSound();

								// Visual Effect - 터지는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
	#endif
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}

#endif
								}
							}
							break;
						case 1 :
							{
								// Critical
								//int iTotalDamage = totaldamage;

								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{	
										// Visual Effect - 첫타에 한번만 출력(살)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
										}
									}
								}								

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] CRITICAL damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);
                                
                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pMob->m_MobType;

								pMob->m_AttackTarget = g_pLocalUser;
								pMob->m_AttackTargetObjectType = _XPICKINGTARGET_SELF;

								pMob->SetBlowMotion(TRUE);
								pMob->StartImpactEffect();
								
								PlayCriticalAttackSound();

								// Visual Effect - 맞는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
							}
							break;
						case 2 :
							{
								// Miss
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] MISS "), m_CurrentAttackPoint-1);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
   								}
							#endif
								
								pMob->ResetImpactEffect();
								
								if( (m_CurrentAttackPoint-1) == 0 )
								{
									// Visual Effect - 첫타에 한번만 출력(실)
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
									}
								}
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] BLOCK "), m_CurrentAttackPoint-1);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
							}
							break;
						case 4 : 
							{
								if( g_ViewDamageValue )
								{
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

								// Shock
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] SHOCK damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);
									
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
							}
							break;
						case 5 :
							{
								// 상극 일반
								//int iTotalDamage = totaldamage;

								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
																		
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										// Visual Effect - 첫타에 한번만 출력 (타)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
										}
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] 상극일반 damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								pMob->m_AttackTarget = g_pLocalUser;
								pMob->m_AttackTargetObjectType = _XPICKINGTARGET_SELF;
								
								pMob->SetBlowMotion();
								pMob->StartImpactEffect();
								
								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pMob->m_MobType;
								
								PlayAttackSound();

								// Visual Effect - 맞는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
	#endif
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}
							}
							break;
						case 6 :
							{
								// 상극 크리
								//int iTotalDamage = totaldamage;

								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}										
									else
									{
										// Visual Effect - 첫타에 한번만 출력 (살)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
										}
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] 상극 크리 damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);		// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								pMob->m_AttackTarget = g_pLocalUser;
								pMob->m_AttackTargetObjectType = _XPICKINGTARGET_SELF;
								
								pMob->SetBlowMotion();
								pMob->StartImpactEffect();
								
								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pMob->m_MobType;
								
								PlayCriticalAttackSound();

								// Visual Effect - 맞는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}
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

							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
								m_AttackResult.cBattleResult = 0;

								for(int i = 0 ; i < 3 ; ++i)
									m_AttackResult.display[i] = FALSE;

								VIEWPACKETLOG
									_XDWINPRINT("[LocalUser/ProcessBattleActiveSkill] Mob [%d] kill", pMob->m_MobType);
							}
						}
					}
				} // target_type_monster
				else if(m_AttackResult.ucToType == en_target_type_object )
				{					
					if( m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT ) // Gate System에서 공격가능한 상태 일때 
					{
						_XFunctionalObject* pFunctionalObject = NULL;
						pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
						if(!pFunctionalObject || !pFunctionalObject->m_bAttackable)
							break;

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
						
						switch(m_AttackResult.usAtkInfo[m_CurrentAttackPoint-1])
						{
						case 0 :
							{
								// HIT
								//int iTotalDamage = totaldamage;

								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										// Visual Effect - 첫타에 한번만 출력 (타)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
										}
									}
								}								

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] HIT damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pFunctionalObject->m_ObjectType;

								PlayAttackSound();

								// Visual Effect - 맞는 이펙트
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
	#endif
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}
							}
							break;
						case 1 :
							{
								// Critical
								//int iTotalDamage = totaldamage;

								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										// Visual Effect - 첫타에 한번만 출력 (살)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
										}
									}
								}								

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] CRITICAL damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pFunctionalObject->m_ObjectType;

								PlayCriticalAttackSound();
								
								// Visual Effect - 맞는 이펙트
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
							}
							break;
						case 2 :
							{
								// Miss
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] MISS "), m_CurrentAttackPoint-1);
                                
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								if( (m_CurrentAttackPoint-1) == 0 )
								{
									// Visual Effect - 첫타에 한번만 출력 (실)
									if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
									{
										D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
									}
								}
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] BLOCK "), m_CurrentAttackPoint-1);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
							}
							break;
						case 4 : 
							{
								// Shock

								if( g_ViewDamageValue )
								{
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] SHOCK damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
							}
							break;
						case 5 :
							{
								// 상극 일반								
								//int iTotalDamage = totaldamage;

								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}										
									else
									{
										// Visual Effect - 첫타에 한번만 출력 (타)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
										}
									}
								}								

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] 상극일반 damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);	// add string

									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pFunctionalObject->m_ObjectType;
								
								PlayAttackSound();

								// Visual Effect - 맞는 이펙트
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
	#endif
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}
							}
							break;
						case 6 :
							{
								// 상극 크리
								//int iTotalDamage = totaldamage;

								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										// Visual Effect - 첫타에 한번만 출력 (살)
										if( (m_CurrentAttackPoint-1) == 0 )
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
										}
									}
								}								

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] 상극 크리 damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);	// add string
									
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
																
								m_CharacterLog.isHit = TRUE;
								m_CharacterLog.hitTime = g_ServerTimeCode;
								m_CharacterLog.hitTargetType = pFunctionalObject->m_ObjectType;
								
								PlayCriticalAttackSound();

								// Visual Effect - 맞는 이펙트
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
	#endif
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}
							}
							break;
						default :
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();				

							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
								m_AttackResult.cBattleResult = 0;

								for(int i = 0 ; i < 3 ; ++i)
									m_AttackResult.display[i] = FALSE;

								VIEWPACKETLOG
									_XDWINPRINT("[LocalUser/ProcessBattleActiveSkill] FO [%d] kill", pFunctionalObject->m_ObjectType);
							}
						}
					}
				}// en_target_type_object
				else if(m_AttackResult.ucToType == en_target_type_others)
				{
					_XUser* pUser = NULL;
					if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					{
						pUser = (_XUser*)m_AttackTarget;
					}

					if(!pUser)
						break;
					
					// 타격 이펙트 삽입
					D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
					
					BOOL bIntersect = FALSE;
					D3DXVECTOR3 pickedpoint, originpos;
					D3DXVECTOR3 direction = pUser->m_Position - m_Position;
					D3DXVec3Normalize(&direction, &direction);
					originpos = m_Position;
					originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f);
					
					bIntersect = pUser->m_ModelDescriptor.InterOBBvsRay(originpos, direction, pUser->m_Position, pUser->m_RotateAngle, &pickedpoint);
					
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
	
					switch(m_AttackResult.usAtkInfo[m_CurrentAttackPoint-1])
					{
					case 0 : 
						{
							// Hit
							//int iTotalDamage = totaldamage;

							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								if( g_ViewDamageValue )
								{
									int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
									if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
								}
								else
								{
									// Visual Effect - 첫타에 한번만 출력 (타)
									if( (m_CurrentAttackPoint-1) == 0 )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}
								}
							}							

						#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
							if(g_ViewBattleDetail)
							{
								TCHAR messagestring[128];
								memset(messagestring, 0, sizeof(messagestring));

								_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] HIT damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);

								g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
							}
						#endif

							m_CharacterLog.isHit = TRUE;
							m_CharacterLog.hitTime = g_ServerTimeCode;
							m_CharacterLog.hitTargetType = 0;

							PlayAttackSound();

							// Visual Effect - 맞는 이펙트
							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
#ifdef _XTS_ITEM_OPTIMIZATION
								if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
								{
									_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
									if( WeaponItem )
									{
										if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
								}
	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
								}
	#endif
#else
								if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
								{
									if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
									else
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
								}
								else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
								{
									if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
									else
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
								}
								else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
								{
									if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
									else
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
								}
#endif
							}
						}
						break;
					case 1 :
						{
							// Critical
							//int iTotalDamage = totaldamage;

							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								if( g_ViewDamageValue )
								{
									int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
									if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
								}
								else
								{
									// Visual Effect - 첫타에 한번만 출력 (살)
									if( (m_CurrentAttackPoint-1) == 0 )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}
								}
							}							

						#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
							if(g_ViewBattleDetail)
							{
								TCHAR messagestring[128];
								memset(messagestring, 0, sizeof(messagestring));

								_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] CRITICAL damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);
                                
								g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                            }
						#endif
							
							m_CharacterLog.isHit = TRUE;
							m_CharacterLog.hitTime = g_ServerTimeCode;
							m_CharacterLog.hitTargetType = 0;

							PlayCriticalAttackSound();

							// Visual Effect - 맞는 이펙트
							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
								}
#endif
							}
							
							g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
						}
						break;
					case 2 :
						{
							// Miss
							if( (m_CurrentAttackPoint-1) == 0 )
							{
								// Visual Effect - 첫타에 한번만 출력 (실)
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage2, &pUser->m_ModelDescriptor.m_Position );
								}
							}

						#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
							if(g_ViewBattleDetail)
							{
								TCHAR messagestring[128];
								memset(messagestring, 0, sizeof(messagestring));

								_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] MISS "), m_CurrentAttackPoint-1);

                                g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
							}
						#endif
						}
						break;
					case 3 :
						{
							// Block
							if( (m_CurrentAttackPoint-1) == 0 )
							{
								// Visual Effect 추가 할 것
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
								}
							}

						#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
							if(g_ViewBattleDetail)
							{
								TCHAR messagestring[128];
								memset(messagestring, 0, sizeof(messagestring));

								_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] BLOCK "), m_CurrentAttackPoint-1);

                                g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
							}
						#endif
						}
						break;
					case 4 :
						{
							// Shock
							//int iTotalDamage = totaldamage;

							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								if( g_ViewDamageValue )
								{
									int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
									if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
								}
								else
								{
									// Visual Effect - 첫타에 한번만 출력 (살)
									if( (m_CurrentAttackPoint-1) == 0 )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}
								}
							}
							
						#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
							if(g_ViewBattleDetail)
							{
								TCHAR messagestring[128];
								memset(messagestring, 0, sizeof(messagestring));

								_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] SHOCK damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);
                            
                                g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
							}
						#endif
							
							m_CharacterLog.isHit = TRUE;
							m_CharacterLog.hitTime = g_ServerTimeCode;
							m_CharacterLog.hitTargetType = 0;
							
							PlayCriticalAttackSound();

							// Visual Effect - 맞는 이펙트
							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
								}
#endif
							}
							
							g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
						}
						break;
					case 5 : 
						{
							// 상극일반
							//int iTotalDamage = totaldamage;

							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								if( g_ViewDamageValue )
								{
									int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
									if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
								}
								else
								{
									// Visual Effect - 첫타에 한번만 출력 (타)
									if( (m_CurrentAttackPoint-1) == 0 )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}
								}
							}

						#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
							if(g_ViewBattleDetail)
							{
								TCHAR messagestring[128];
								memset(messagestring, 0, sizeof(messagestring));

								_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] 상극일반 damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);	// add string

                                g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
							}
						#endif
							
							m_CharacterLog.isHit = TRUE;
							m_CharacterLog.hitTime = g_ServerTimeCode;
							m_CharacterLog.hitTargetType = 0;
							
							PlayAttackSound();

							// Visual Effect - 맞는 이펙트
							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
#ifdef _XTS_ITEM_OPTIMIZATION
								if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
								{
									_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
									if( WeaponItem )
									{
										if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
								}
	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
								}
	#endif
#else
								if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
								{
									if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
									else
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
								}
								else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
								{
									if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
									else
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
								}
								else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
								{
									if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
									else
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
								}
#endif
							}
						}
						break;
					case 6 : 
						{
							// 상극크리
							//int iTotalDamage = totaldamage;

							
							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								if( g_ViewDamageValue )
								{
									int iDamageValue = m_AttackResult.sDamage[m_CurrentAttackPoint-1];
									if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
								}
								else
								{
									// Visual Effect - 첫타에 한번만 출력 (살)
									if( (m_CurrentAttackPoint-1) == 0 )
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}
								}
							}							

						#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
							if(g_ViewBattleDetail)
							{
								TCHAR messagestring[128];
								memset(messagestring, 0, sizeof(messagestring));

								_snprintf(messagestring, sizeof(messagestring), _T("Active/Seq[%d] 상극크리 damage[%d]"), m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);	// add string
                            
                                g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
							}
						#endif
							
							m_CharacterLog.isHit = TRUE;
							m_CharacterLog.hitTime = g_ServerTimeCode;
							m_CharacterLog.hitTargetType = 0;
							
							PlayCriticalAttackSound();

							// Visual Effect - 맞는 이펙트
							if( pUser->m_ModelDescriptor.m_EffectManager )
							{
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
								}
#endif
							}
						}
						break;
					default :
						break;
					}
					if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
					{
						if(pUser == (_XUser*)g_LastPickedObject) 
							g_LastPickedObject = NULL;
						if(pUser == (_XUser*)g_pLocalUser->m_AttackTarget)
							g_pLocalUser->ResetAttackTarget();
						if(pUser == (_XUser*)g_pLocalUser->m_ViewTarget)
							g_pLocalUser->ResetViewTarget();

						// 내가 죽인 유저
						for(int i = 0 ; i < 3 ; ++i)
							m_AttackResult.display[i] = FALSE;

						m_AttackResult.cBattleResult = 0;

						VIEWPACKETLOG
							_XDWINPRINT("[LocalUser/ProcessBattleActiveSkill] User[%s] kill", pUser->m_CharacterInfo.charactername);
					}
				} // target_type_others
			} // command frame
			if(m_bCheckFirstFrame)
			{
				if(!m_AttackTarget)
				{
					// target 없음 - 보간 동작 실행하지 않음(Moving Attack 방지용)
					m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					m_bCheckFirstFrame = FALSE;
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					m_CurrentBattleState = _XUSER_STATE_LAST;
					
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
					m_bAddDelayTime = TRUE;
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 보간 동작 있음
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_NextSkillID > 0)
				{
					// 다음 스킬로
					m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					m_CurrentAttackPoint = 0;
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					ResetSkillAnimation();
					
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
				}
				else
				{
					// 끝
					m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					m_bCheckFirstFrame = FALSE;
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;

					ResetSkillAnimation();
					m_bRestartActive = TRUE;
					
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
					
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
						ProcessMobDie();
				}
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			ResetSkillAnimation();
		}
		break;
	}
}

void _XLocalUser::ProcessBattleChargeSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						FLOAT range = 0.0f;

						if(CheckDistanceToTarget(pMob->m_Position, m_SelectedSkillID, range, pMob->m_MobType))
						{
							D3DXVECTOR3 mypos = m_Position;
							mypos.y += 0.89f;
							D3DXVECTOR3 targetpos = pMob->m_Position;
							targetpos.y += g_MOBMODEL[pMob->m_MobType]->OBB_Extent[1];
							if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
								ResetSkillAnimation();
							}
							else
							{
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
								UpdatePosition();

								m_CurrentBattleState = _XUSER_STATE_2;
							}
						}
						else
						{
							SetTargetPosition(pMob->m_Position, range);
						}
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)m_AttackTarget;
                        //Author : 양희왕 //breif : 레벨차 공격 안되게                    
                        if( !pUser )
                            break;

                        if(CheckPKPenalty(pUser))
                            break;

						FLOAT range = 0.0f;

						if(CheckDistanceToTarget(pUser->m_Position, m_SelectedSkillID, range, 0))
						{
							D3DXVECTOR3 mypos = m_Position;
							mypos.y += 0.89f;
							D3DXVECTOR3 targetpos = pUser->m_Position;
							targetpos.y += 0.89;
							if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
								ResetSkillAnimation();
							}
							else
							{
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
								UpdatePosition();

								m_CurrentBattleState = _XUSER_STATE_2;
							}
						}
						else
						{
							SetTargetPosition(pUser->m_Position, range);
						}
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
						if( pFunctionalObject->m_bAttackable )
						{
							FLOAT range = 0.0f;
							if(CheckDistanceToTarget(pFunctionalObject->m_Position, m_SelectedSkillID, range, 0))
							{
								D3DXVECTOR3 mypos = m_Position;
								mypos.y += 0.89f;
								D3DXVECTOR3 targetpos = pFunctionalObject->m_Position;
								targetpos.y += 0.89;
								if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
									ResetSkillAnimation();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pFunctionalObject->m_Position);
									UpdatePosition();
									
									m_CurrentBattleState = _XUSER_STATE_2;
								}
							}
							else
							{
								SetTargetPosition(pFunctionalObject->m_Position, range);
							}
						}
						
					}
					break;
				default :
					{
						ResetSkillAnimation();
					}
					break;
				}
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
			if(CheckTargetTypeFinal())
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
				case _XPICKINGTARGET_MOB :
				case _XPICKINGTARGET_PC :
					{
						if(!m_ChargeSendStartPacket)
						{
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY);
							m_ChargeSendStartPacket = TRUE;
							m_bUsingSkill = TRUE;

							if(g_BattleLog.GetWriteLog())
							{
								g_BattleLog.SetTime();
								g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);

								if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
								{
									g_BattleLog.SetTargetType(((_XMob*)m_AttackTarget)->m_MobType);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
								{
									g_BattleLog.SetTargetType(((_XUser*)m_AttackTarget)->m_Uniqueid);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
								{
									g_BattleLog.SetTargetType(((_XFunctionalObject*)m_AttackTarget)->m_ObjectType);
								}

								g_BattleLog.SetSkillId(m_SelectedSkillID);
							}
						}
					}
					break;
				default :
					{
						if(!m_ChargeSendStartPacket)
							ResetSkillAnimation();
					}
					break;
				}
			}
			else
			{
				// target이 없어지면 reset
				if(!m_ChargeSendStartPacket)
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

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
					UpdatePosition();
				}
			}

			if(m_bStartCharge)		// 예약 되어있던 Charge skill
			{
				int index = -1;
				if(gpInput->CheckKeyPress(DIK_1))
					index = 0;
				if(gpInput->CheckKeyPress(DIK_2))
					index = 1;
				if(gpInput->CheckKeyPress(DIK_3))
					index = 2;
				if(gpInput->CheckKeyPress(DIK_4))
					index = 3;
				if(gpInput->CheckKeyPress(DIK_5))
					index = 4;
				if(gpInput->CheckKeyPress(DIK_6))
					index = 5;
				if(gpInput->CheckKeyPress(DIK_7))
					index = 6;
				if(gpInput->CheckKeyPress(DIK_8))
					index = 7;
				if(gpInput->CheckKeyPress(DIK_9))
					index = 8;
				if(gpInput->CheckKeyPress(DIK_0))
					index = 9;

				BOOL bEndCharge = TRUE;

				if(index >= 0)
				{
					_XQSlot_Type type = g_pQuickSlot_Window->GetQuickSlotType(index);
					if(type == _XQS_SLOTTYPE_SKILL)
					{
						// skill 처리
						short skillid = g_pQuickSlot_Window->GetQuickSlotSkillID(index);
						if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHARGESKILL)
						{
							bEndCharge = FALSE;
						}
					}
				}
				
				if(bEndCharge)
				{
					//if(m_ChargeIterateValue == 1) //0 으로 수정함
					if(m_ChargeIterateValue == 0) //Author : 양희왕
						m_CurrentBattleState = _XUSER_STATE_7;
				}
			}
			else if(m_bUseMouseChargeSkill)
			{
				m_CurrentBattleState = _XUSER_STATE_7;
			}
		}
		break;
	case _XUSER_STATE_7 :
		{
			// charge time 계산하기
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			int casting = 0;
			if(m_SelectedSkillItem->m_cSelectedSkillLevel == 0)
				casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);
			else
				casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel)
							- _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel-1);


			FLOAT fchargetime = ((m_ChargeIterateValue * casting) 
				+ (g_LocalSystemTime - m_ChargeStartTime)) / 100.0f;
			short chargetime = (short)fchargetime;

			if(chargetime > m_SelectedSkillItem->m_cSelectedSkillLevel * casting)
				chargetime = m_SelectedSkillItem->m_cSelectedSkillLevel * casting;

			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, chargetime);
			
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
			m_CurrentBattleState = _XUSER_STATE_8;
		}
		break;
	case _XUSER_STATE_8 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				if(!m_AttackResult.display[0])
					break;

				// Effect로 처리한다.
				if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_OTHER_ENEMY)
				{
					if(m_AttackResult.ucToType == en_target_type_monster)
					{
						_XMob* pMob = g_Moblist.FindMob(m_AttackResult.usDefenderID);
						if(!pMob)
							break;

						// Monster Health 적용(Display용)
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To Mob [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayCriticalAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To Mob [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), "Charge MISS");

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To Mob [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), "Charge BLOCK ");

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Charge To Mob [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock

								if( g_ViewDamageValue )
								{
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), "Charge SHOCK ");

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pMob->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Charge To Mob [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To Mob [상극일반/damage:%d]", m_AttackResult.sDamage[0]);	// add string
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayCriticalAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To Mob [상극크리/damage:%d]", m_AttackResult.sDamage[0]);	// add string
							}
							break;
						}

						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
							pMob->SetAnimation(_XMOBACTION_DIE);
							pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
							
							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pMob->m_MobType;

								
								m_AttackResult.cBattleResult = 0;
								
								VIEWPACKETLOG
									_XDWINPRINT("[LocalUser/ProcessBattleChargeSkill] Mob [%d] kill", pMob->m_MobType);
							}
						}

						m_AttackResult.display[0] = FALSE;
					}
					else if(m_AttackResult.ucToType == en_target_type_object)
					{
						_XFunctionalObject* pFunctionalObject = g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
						if(!pFunctionalObject)
							break;

						// Monster Health 적용(Display용)
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To FO [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayCriticalAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To FO [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To FO [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge BLOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Charge To FO [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock

								if( g_ViewDamageValue )
								{
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge SHOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Charge To FO [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To FO [상극일반/damage:%d]", m_AttackResult.sDamage[0]);	
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{										
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
											g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayCriticalAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Charge To FO [상극크리/damage:%d]", m_AttackResult.sDamage[0]);	
							}
							break;
						}

						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();			
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							
							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pFunctionalObject->m_ObjectType;

								
								m_AttackResult.cBattleResult = 0;								
							}
							ResetAttackTarget();
							ResetViewTarget();
							ResetSpellAttackTarget();	
						}

						m_AttackResult.display[0] = FALSE;
					}//en_target_type_object
					else if(m_AttackResult.ucToType == en_target_type_others)
					{
						_XUser* pUser = g_Userlist.FindUser(m_AttackResult.usDefenderID);
						if(!pUser)
							break;

						D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;
						
						if(m_AttackResult.display[0])
						{
							switch(m_AttackResult.usAtkInfo[0])
							{
							case 0 :
								{
									// Hit
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										// Visual Effect - 맞는 이펙트(스킬테이블에서)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
										
										// Visual Effect - (타)										
										if( g_ViewDamageValue )
										{
											int iDamageValue = m_AttackResult.sDamage[0];
											if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
										}
										else
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
										}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}

									PlayAttackSound();

								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Charge HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
									}
								#endif

									VIEWPACKETLOG
										_XDWINPRINT("-- Attack Result : Charge To User [Hit/damage:%d]", m_AttackResult.sDamage[0]);
								}
								break;
							case 1 :
								{
									// Critical
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										// Visual Effect - 맞는 이펙트(스킬테이블에서)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);

										if( g_ViewDamageValue )
										{
											int iDamageValue = m_AttackResult.sDamage[0];
											if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
										}
										else
										{
											// Visual Effect - (살)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
										}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}

									PlayCriticalAttackSound();

								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Charge CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
									}
								#endif

									VIEWPACKETLOG
										_XDWINPRINT("-- Attack Result : Charge To User [Critical/damage:%d]", m_AttackResult.sDamage[0]);
								}
								break;
							case 2 :
								{
									// Miss
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										// Visual Effect - 맞는 이펙트(스킬테이블에서)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position);

										// Visual Effect - (실)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage2, &pUser->m_ModelDescriptor.m_Position );
									}

								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Charge MISS"));

                                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
									}
								#endif

									VIEWPACKETLOG
										_XDWINPRINT("-- Attack Result : Charge To User [Miss/damage:%d]", m_AttackResult.sDamage[0]);
								}
								break;
							case 3 :
								{
									// Block
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										// Visual Effect - 맞는 이펙트(스킬테이블에서)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position);
										
										// Visual Effect - 추가 할 것
									}
									
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Charge BLOCK"));

                                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
									}
								#endif
									
									VIEWPACKETLOG
										_XDWINPRINT("-- Attack Result : Charge To User [Block/damage:%d]", m_AttackResult.sDamage[0]);
								}
								break;
							case 4 :
								{
									// Shock
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										// Visual Effect - 맞는 이펙트(스킬테이블에서)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
										
										if( g_ViewDamageValue )
										{
											int iDamageValue = m_AttackResult.sDamage[0];
											if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
										}
										else
										{
											// Visual Effect - (살)
										    pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
										}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}

									PlayCriticalAttackSound();
									
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Charge SHOCK damage[%d]"), m_AttackResult.sDamage[0]);

                                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
									}
								#endif
									
									VIEWPACKETLOG
										_XDWINPRINT("-- Attack Result : Charge To User [Shock/damage:%d]", m_AttackResult.sDamage[0]);
								}
								break;
							case 5 :
								{
									// 상극 일반
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										// Visual Effect - 맞는 이펙트(스킬테이블에서)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
										
										if( g_ViewDamageValue )
										{
											int iDamageValue = m_AttackResult.sDamage[0];
											if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
										}
										else
										{
											// Visual Effect - (타)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
										}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}

									PlayAttackSound();
									
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
									}
								#endif
									
									VIEWPACKETLOG
										_XDWINPRINT("-- Attack Result : Charge To User [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
								}
								break;
							case 6 :
								{
									// 상극 크리
									if( pUser->m_ModelDescriptor.m_EffectManager )
									{
										// Visual Effect - 맞는 이펙트(스킬테이블에서)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
										
										if( g_ViewDamageValue )
										{
											int iDamageValue = m_AttackResult.sDamage[0];
											if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
										}
										else
										{
											// Visual Effect - (살)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
										}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
										if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
										{
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
												m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
										}
#endif
									}

									PlayCriticalAttackSound();
									
								#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
									if(g_ViewBattleDetail)
									{
										TCHAR messagestring[128];
										memset(messagestring, 0, sizeof(messagestring));

										_snprintf(messagestring, sizeof(messagestring), _T("Charge 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
									}
								#endif
									
									VIEWPACKETLOG
										_XDWINPRINT("-- Attack Result : Charge To User [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
								}
								break;
							}
						} // if(m_AttackResult.display[0])

						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							// 내가 죽인 유저
//							m_CharacterLog.isKillClient = TRUE;
//							m_CharacterLog.killTimeClient = g_ServerTimeCode;
//							m_CharacterLog.killTargetTypeClient = 0;

							
							m_AttackResult.cBattleResult = 0;							
						}

						m_AttackResult.display[0] = FALSE;
					}
				} // other-enemy
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


				short motionindex = GetMotionIndex(m_SelectedSkillID);
				
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
				{
#ifdef _XTS_NEWINSENSIBLE
					if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
					SetAnimation(_XACTION_IDLE);
					m_CurrentBattleState = _XUSER_STATE_9;
				}
			} // last frame
		}
		break;
	case _XUSER_STATE_9 :
		{
			m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
			m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			m_ChangeDelayStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleSelfRoundingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(!m_RoundingSendPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 1);
				m_RoundingSendPacket = TRUE;
				m_bUsingSkill = TRUE;

				if(g_BattleLog.GetWriteLog())
				{
					g_BattleLog.SetTime();
					g_BattleLog.SetTargetGroup(_XPICKINGTARGET_SELF);
					g_BattleLog.SetTargetType(m_Uniqueid);
					g_BattleLog.SetSkillId(m_SelectedSkillID);
				}

			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);
			
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex);
			m_CurrentBattleState = _XUSER_STATE_4;
			
			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		break;
	case _XUSER_STATE_4 :
		{
			//			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
			if(m_ModelDescriptor.m_pBipedController->GetFirstFrame()+5 >= m_ModelDescriptor.m_AniFrame &&
				m_ModelDescriptor.m_AniFrame > 0.0f && !m_bCheckFirstFrame)
			{
				m_RoundingDelayStartTime = g_LocalSystemTime;
				m_bCheckFirstFrame = TRUE;
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_bCheckFirstFrame = FALSE;
				
				// 들고 있던 무기 복구
				ReloadWeaponModel();
#ifdef _XTS_NEWINSENSIBLE
				if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
				SetAnimation(_XACTION_IDLE);
				m_CurrentBattleState = _XUSER_STATE_5;
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			m_RoundingDelayStartTime = 0;
			m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
			m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			m_ChangeDelayStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			
			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleTargetRoundingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				BOOL bCheckRayPass = TRUE;
				D3DXVECTOR3 targetposition;
				unsigned short targettype;
				D3DXVECTOR3 targetpos;

                //Author : 양희왕 //breif : 레벨차 공격 안되게
                if( m_AttackTargetObjectType == _XPICKINGTARGET_PC )
                {
                    _XUser* pUser = NULL;
		            pUser = (_XUser*)m_AttackTarget;
                    if( !pUser )
                        break;

                    if(CheckPKPenalty(pUser))
                        break;
                }

				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						targetposition = ((_XMob*)m_AttackTarget)->m_Position;
						targettype = ((_XMob*)m_AttackTarget)->m_MobType;
						targetpos = targetposition;
						targetpos.y += g_MOBMODEL[((_XMob*)m_AttackTarget)->m_MobType]->OBB_Extent[1];
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						targetposition = ((_XUser*)m_AttackTarget)->m_Position;
						targettype = 0;
						targetpos = targetposition;
						targetpos.y += 0.89f;
					}
					break;
				case _XPICKINGTARGET_SELF :
					{
						targetposition = g_pLocalUser->m_Position;
						targettype = 0;
						targetpos = targetposition;
						targetpos.y += 0.89f;
						bCheckRayPass = FALSE;
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT:
					{
						targetposition = ((_XFunctionalObject*)m_AttackTarget)->m_Position;
						targettype = ((_XFunctionalObject*)m_AttackTarget)->m_ObjectType ;
						targetpos = targetposition;
						targetpos.y += 0.89f;
					}
					break;
				default :
					{
						ResetSkillAnimation();
						return;
					}
				}
				FLOAT range = 0.0f;

				if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
				{
					D3DXVECTOR3 mypos = m_Position;
					mypos.y += 0.89f;
					if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) && bCheckRayPass)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
						ResetSkillAnimation();
					}
					else
					{
						m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
						UpdatePosition();
						m_CurrentBattleState = _XUSER_STATE_2;
					}
				}
				else
					SetTargetPosition(targetposition, range);
			}
			else
			{
				// target이 없어지면 reset
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(CheckTargetTypeFinal())
			{
				if(!m_RoundingSendPacket)
				{
					if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
						g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 1);
					else
						g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);

					m_RoundingSendPacket = TRUE;
					m_bUsingSkill = TRUE;

					if(g_BattleLog.GetWriteLog())
					{
						g_BattleLog.SetTime();
						g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
						
						if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
						{
							g_BattleLog.SetTargetType(((_XMob*)m_AttackTarget)->m_MobType);
						}
						else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
						{
							g_BattleLog.SetTargetType(((_XUser*)m_AttackTarget)->m_Uniqueid);
						}
						else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
						{
							g_BattleLog.SetTargetType(((_XFunctionalObject*)m_AttackTarget)->m_ObjectType);
						}
						
						g_BattleLog.SetSkillId(m_SelectedSkillID);
					}
				}
			}
			else
			{
				if(m_RoundingSendPacket)
				{
					// rounding은 패킷 보낸후 취소 되지 않음
//					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
				}
				else
				{
					// packet 보내기 전 target이 없어지면 reset
					ResetSkillAnimation();
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);

			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XMob*)m_AttackTarget)->m_Position);
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XUser*)m_AttackTarget)->m_Position);
				UpdatePosition();
			}
			
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex);
			m_CurrentBattleState = _XUSER_STATE_4;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		break;
	case _XUSER_STATE_4 :
		{
//			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
			if(m_ModelDescriptor.m_pBipedController->GetFirstFrame()+5 >= m_ModelDescriptor.m_AniFrame &&
				m_ModelDescriptor.m_AniFrame > 0.0f && !m_bCheckFirstFrame)
			{
				m_RoundingDelayStartTime = g_LocalSystemTime;
				m_bCheckFirstFrame = TRUE;
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				// Display Region Damage
				if(m_RegionDamageResult.bValid == TRUE)
				{
					if(m_SelectedSkillID == m_RegionDamageResult.nSkillID)
					{
						_XMob* pMob = NULL;
						_XUser* pUser = NULL;

						for(int i = 0 ; i < m_RegionDamageResult.nObjectCnt ; ++i)
						{
							pMob = NULL;
							pUser = NULL;

							if(m_RegionDamageResult.sResult[i].nObjectType == 0)
							{
								pMob = (_XMob*)g_Moblist.FindMob(m_RegionDamageResult.sResult[i].nID);
								if(pMob)
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
							else if(m_RegionDamageResult.sResult[i].nObjectType == 1)
							{
								pUser = (_XUser*)g_Userlist.FindUser(m_RegionDamageResult.sResult[i].nID);
								if(pUser)
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
						}
					}
					m_RegionDamageResult.bValid = FALSE;
				}

				if(!m_AttackResult.display[0])
					break;

				if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_OTHER_ENEMY)
				{
					if(m_AttackResult.ucToType == en_target_type_monster)
					{
						_XMob* pMob = g_Moblist.FindMob(m_AttackResult.usDefenderID);
						if(!pMob)
							break;

						// Monster Health 적용(Display용)
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To Mob [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To Mob [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To Mob [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding BLOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Rounding To Mob [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock
								if( g_ViewDamageValue )
								{
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding SHOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pMob->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Rounding To Mob [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To Mob [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To Mob [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
							pMob->SetAnimation(_XMOBACTION_DIE);
							pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pMob->m_MobType;

								
								m_AttackResult.cBattleResult = 0;
								
								_XDWINPRINT("[LocalUser/ProcessBattleChargeSkill] Mob [%d] kill", pMob->m_MobType);
							}
						}
						m_AttackResult.display[0] = FALSE;
					}
					else if(m_AttackResult.ucToType == en_target_type_object)
					{
						_XFunctionalObject* pFunctionalObject = g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
						if(!pFunctionalObject)
							break;

						// Monster Health 적용(Display용)
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To FO [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To FO [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To FO [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding BLOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Rounding To FO [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock
								if( g_ViewDamageValue )
								{
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];										
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding SHOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Rounding to FO [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (파)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To FO [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (필살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To FO [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();			
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);

							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pFunctionalObject->m_ObjectType;


								
								m_AttackResult.cBattleResult = 0;
								
								//_XDWINPRINT("[LocalUser/ProcessBattleChargeSkill] Mob [%d] kill", pMob->m_MobType);
							}
							ResetAttackTarget();
							ResetViewTarget();
							ResetSpellAttackTarget();	
						}
						m_AttackResult.display[0] = FALSE;
					}//en_target_type_object
					else if(m_AttackResult.ucToType == en_target_type_others)
					{
						_XUser* pUser = g_Userlist.FindUser(m_AttackResult.usDefenderID);
						if(!pUser)
							break;

						D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}


							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To User [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To User [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage2, &pUser->m_ModelDescriptor.m_Position );
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To User [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 :
							{
								// Block
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 추가 해야 함
								}
								
							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), "Rounding BLOCK");

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To User [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 :
							{
								// Shock
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding SHOCK damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To User [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To User [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Rounding 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Rounding To User [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							// 내가 죽인 유저
//							m_CharacterLog.isKillClient = TRUE;
//							m_CharacterLog.killTimeClient = g_ServerTimeCode;
//							m_CharacterLog.killTargetTypeClient = 0;
							
							m_AttackResult.cBattleResult = 0;							
						}

						m_AttackResult.display[0] = FALSE;
					}
				} // other-enemy

				m_AttackResult.display[0] = FALSE;
				m_bCheckFirstFrame = FALSE;
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

				m_bCheckFirstFrame = FALSE;
				m_CurrentBattleState = _XUSER_STATE_5;
				
#ifdef _XTS_NEWINSENSIBLE
				if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
				SetAnimation(_XACTION_IDLE);
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			m_RoundingDelayStartTime = 0;
			m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
			m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			m_ChangeDelayStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			
			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleAreaRoundingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			// 마우스 포인트 변경
			((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_PICKUP);
			
			// 영역을 지정하기 위해 마우스 피킹을 계속 계산함.		
			g_LodTerrain.GetPickedPoint( FALSE );
			
			_XParticleGroupItem* pParticleGroupItem = g_WorldEffectInstanceManager.FindInstance( _XDEF_SKILLID_1076 );
			
			D3DXMATRIX mousemat;
			D3DXMatrixIdentity( &mousemat );
			mousemat._41 = g_vMousePosition.x;
			mousemat._42 = g_vMousePosition.y;
			mousemat._43 = g_vMousePosition.z;
			
			D3DXVECTOR3 maxsize;
			
			if( !pParticleGroupItem )
			{ 
				pParticleGroupItem = g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_SKILLID_1076, 
					&mousemat );
				if(pParticleGroupItem)
				{
					maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;
					pParticleGroupItem->SetParticleMaxSize( maxsize );
				}
			}
			else
			{
				maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;				
				
				pParticleGroupItem->MovePosition( mousemat );
				pParticleGroupItem->SetParticleMaxSize( maxsize );
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_SKILLID_1076 );
			
			if(!m_RoundingSendPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 2);	// point
				m_RoundingSendPacket = TRUE;
				m_bUsingSkill = TRUE;
				
				// 채널링 영역을 지정하기 위해 마우스 피킹을 계속 계산함.		
				g_LodTerrain.GetPickedPoint( FALSE );
				
				D3DXMATRIX mousemat;
				D3DXMatrixIdentity( &mousemat );
				mousemat._41 = g_vMousePosition.x;
				mousemat._42 = g_vMousePosition.y;
				mousemat._43 = g_vMousePosition.z;
				
				D3DXVECTOR3 maxsize;
				
				if( m_ModelDescriptor.m_EffectManager )
				{
					_XParticleGroupItem* pParticleGroupItem = m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, m_SelectedSkillID + 40000, &mousemat );
					//_XParticleGroupItem* pParticleGroupItem = g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, m_SelectedSkillID + 40000, &mousemat );
					if(pParticleGroupItem)
					{
						maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;
						pParticleGroupItem->SetParticleMaxSize( maxsize );
					}
				}

				if(g_BattleLog.GetWriteLog())
				{
					g_BattleLog.SetTime();
					g_BattleLog.SetTargetGroup(-1);
					g_BattleLog.SetSkillId(m_SelectedSkillID);
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);

			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex);
			m_CurrentBattleState = _XUSER_STATE_4;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		break;
	case _XUSER_STATE_4 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				// 들고 있던 무기 복구
				ReloadWeaponModel();
				m_bCheckFirstFrame = FALSE;
				m_CurrentBattleState = _XUSER_STATE_5;
#ifdef _XTS_NEWINSENSIBLE
				if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
				SetAnimation(_XACTION_IDLE);
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			m_RoundingDelayStartTime = 0;
			m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
			m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			m_ChangeDelayStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
			break;
		}
	}
}

void _XLocalUser::ProcessBattleHiddenMA(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(!m_HiddenSendStartPacket)
			{
				m_bUsingSkill = TRUE;
				g_NetworkKernel.SendPacket(MSG_NO_SELF_CHANNEL_START_REQUEST, 0);
				m_HiddenSendStartPacket = TRUE;
				m_HiddenSendLastPacketTime = g_LocalSystemTime;
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);

			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex + _XANI_ORDER_READY, FALSE);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_3;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
				pMainMenu_Window->m_bDrawMedEffectInfinite = TRUE;
		}
		break;
	case _XUSER_STATE_3 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetLockEndFrame(FALSE);
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
				{
					m_CurrentBattleState = _XUSER_STATE_4;
				}
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
			m_CurrentBattleState = _XUSER_STATE_5;
		}
		break;
	case _XUSER_STATE_5 :
		{
			if(g_LocalSystemTime - m_HiddenSendLastPacketTime > 5000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_SELF_CHANNEL_START_REQUEST, 1);
				m_HiddenSendLastPacketTime = g_LocalSystemTime;
			}
		}
		break;
	case _XUSER_STATE_6 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
			{
				m_ModelDescriptor.SetLockEndFrame(TRUE);
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					m_ModelDescriptor.SetLockEndFrame(FALSE);
					m_CurrentBattleState = _XUSER_STATE_7;
				}
			}
			else
			{
				m_ModelDescriptor.SetLockEndFrame(FALSE);
				m_CurrentBattleState = _XUSER_STATE_7;
			}
		}
		break;
	case _XUSER_STATE_7 :
		{			
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_8;
		}
		break;
	case _XUSER_STATE_8 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();

					m_ModelDescriptor.SetLockEndFrame(FALSE);
#ifdef _XTS_NEWINSENSIBLE
					if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
					SetAnimation(_XACTION_IDLE);
					m_ChangeDelayStartTime = g_LocalSystemTime;
					m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					m_CurrentBattleState = _XUSER_STATE_LAST;
				}
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleSelfChannelingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) 
		return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(!m_bChannelingSendStartPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 1);
				m_bChannelingSendStartPacket = TRUE;
				m_bUsingSkill = TRUE;
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);

			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex + _XANI_ORDER_READY);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_4;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
					SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT);
					m_CurrentBattleState = _XUSER_STATE_5;
					m_ChannelingSpellRepeatTime = g_LocalSystemTime;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			// send fire packet - 1초마다 한번
			if(m_ChannelingSendLastPacketTime == 0 || g_LocalSystemTime - m_ChannelingSendLastPacketTime > 1000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 1);
				m_ChannelingSendLastPacketTime = g_LocalSystemTime;
			}

			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
			{
				if((g_LocalSystemTime - m_ChannelingSpellRepeatTime) > casting)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
				// 다른 조건으로 state change
				if(m_CharacterInfo.Get_current_forcepower() < _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel))//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
				if(m_NextSkillID > 0)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
			}
			else
			{
				// 다른 조건으로 state change
				if(m_CharacterInfo.Get_current_forcepower() < _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel))//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
				if(m_NextSkillID > 0)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
			}

			// 영역기 이펙트 처리를 위해 추가
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				// Display Region Damage
				if(m_RegionDamageResult.bValid == TRUE)
				{
					if(m_SelectedSkillID == m_RegionDamageResult.nSkillID)
					{
						_XMob* pMob = NULL;
						_XUser* pUser = NULL;

						for(int i = 0 ; i < m_RegionDamageResult.nObjectCnt ; ++i)
						{
							pMob = NULL;
							pUser = NULL;

							if(m_RegionDamageResult.sResult[i].nObjectType == 0)
							{
								pMob = (_XMob*)g_Moblist.FindMob(m_RegionDamageResult.sResult[i].nID);
								if(pMob)
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
							else if(m_RegionDamageResult.sResult[i].nObjectType == 1)
							{
								pUser = (_XUser*)g_Userlist.FindUser(m_RegionDamageResult.sResult[i].nID);
								if(pUser)
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
						}
					}
					m_RegionDamageResult.bValid = FALSE;
				}
			}//command frame
		}
		break;
	case _XUSER_STATE_6 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);

			m_ModelDescriptor.SetLockEndFrame(FALSE);
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE);
			m_CurrentBattleState = _XUSER_STATE_7;
			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
		}
		break;
	case _XUSER_STATE_7 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);

			if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();

					m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
					m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					m_ChangeDelayStartTime = g_LocalSystemTime;
#ifdef _XTS_NEWINSENSIBLE
					if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
					SetAnimation(_XACTION_IDLE);
					m_CurrentBattleState = _XUSER_STATE_LAST;
				}
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if((g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime) > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleTargetChannelingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) 
		return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						FLOAT range = 0.0f;
						
						if(CheckDistanceToTarget(pMob->m_Position, m_SelectedSkillID, range, pMob->m_MobType))
						{
							D3DXVECTOR3 mypos = m_Position;
							mypos.y += 0.89f;
							D3DXVECTOR3 targetpos = pMob->m_Position;
							targetpos.y += g_MOBMODEL[pMob->m_MobType]->OBB_Extent[1];
							if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
								ResetSkillAnimation();
							}
							else
							{
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
								UpdatePosition();
								
								m_CurrentBattleState = _XUSER_STATE_2;
							}
						}
						else
							SetTargetPosition(pMob->m_Position, range);
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)m_AttackTarget;
						FLOAT range = 0.0f;
						
						if(CheckDistanceToTarget(pUser->m_Position, m_SelectedSkillID, range, 0))
						{
							D3DXVECTOR3 mypos = m_Position;
							mypos.y += 0.89f;
							D3DXVECTOR3 targetpos = pUser->m_Position;
							targetpos.y += 0.89f;
							if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
								ResetSkillAnimation();
							}
							else
							{
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
								UpdatePosition();
								
								m_CurrentBattleState = _XUSER_STATE_2;
							}
						}
						else
							SetTargetPosition(pUser->m_Position, range);
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
						if( pFunctionalObject->m_bAttackable )
						{
							FLOAT range = 0.0f;
							if(CheckDistanceToTarget(pFunctionalObject->m_Position, m_SelectedSkillID, range, 0))
							{
								D3DXVECTOR3 mypos = m_Position;
								mypos.y += 0.89f;
								D3DXVECTOR3 targetpos = pFunctionalObject->m_Position;
								targetpos.y += 0.89f;
								if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
									ResetSkillAnimation();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pFunctionalObject->m_Position);
									UpdatePosition();
									
									m_CurrentBattleState = _XUSER_STATE_2;
								}
							}
							else
								SetTargetPosition(pFunctionalObject->m_Position, range);
						}
						
					}
					break;
				case _XPICKINGTARGET_SELF :
					m_CurrentBattleState = _XUSER_STATE_2;
					break;
				default :
					ResetSkillAnimation();
					break;
				}
			}
			else
			{
				// Target이 없음
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(CheckTargetTypeFinal())
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
				case _XPICKINGTARGET_MOB :
				case _XPICKINGTARGET_PC :
					{
						if(!m_bChannelingSendStartPacket)
						{
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 0);
							m_bChannelingSendStartPacket = TRUE;
							m_bUsingSkill = TRUE;

							if(g_BattleLog.GetWriteLog())
							{
								g_BattleLog.SetTime();
								g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
								
								if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
								{
									g_BattleLog.SetTargetType(((_XMob*)m_AttackTarget)->m_MobType);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
								{
									g_BattleLog.SetTargetType(((_XUser*)m_AttackTarget)->m_Uniqueid);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
								{
									g_BattleLog.SetTargetType(((_XFunctionalObject*)m_AttackTarget)->m_ObjectType);
								}
								
								g_BattleLog.SetSkillId(m_SelectedSkillID);
							}
						}
					}
					break;
				case _XPICKINGTARGET_SELF :
					{
						if(!m_bChannelingSendStartPacket)
						{
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 1);
							m_bChannelingSendStartPacket = TRUE;
							m_bUsingSkill = TRUE;

							if(g_BattleLog.GetWriteLog())
							{
								g_BattleLog.SetTime();
								g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
								g_BattleLog.SetTargetType(m_Uniqueid);
								g_BattleLog.SetSkillId(m_SelectedSkillID);
							}
						}
					}
					break;
				default :
					ResetSkillAnimation();
					break;
				}
			}
			else
			{
				if(m_bChannelingSendStartPacket)
				{
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
					ResetSkillAnimation();
				}
				else
				{
					ResetSkillAnimation();
				}

				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);
			
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex + _XANI_ORDER_READY);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_4;
			
			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
					SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT);
					m_CurrentBattleState = _XUSER_STATE_5;
					m_ChannelingSpellRepeatTime = g_LocalSystemTime;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			if(m_ChannelingSendLastPacketTime == 0 || g_LocalSystemTime - m_ChannelingSendLastPacketTime > 1000)
			{
				if(m_SpellAttackTargetType == _XPICKINGTARGET_SELF)
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 1);
				else 
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 0);
				m_ChannelingSendLastPacketTime = g_LocalSystemTime;
			}

			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
			{	
				if((g_LocalSystemTime - m_ChannelingSpellRepeatTime) > casting)
				{
					// 반복 animation 끝
					m_CurrentBattleState = _XUSER_STATE_6;
				}
				// 다른 조건으로 state change
				if(m_CharacterInfo.Get_current_forcepower() < _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel))//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
				if(m_NextSkillID > 0)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
			}
			else
			{
				// 다른 조건
				if(m_CharacterInfo.Get_current_forcepower() < _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel))//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
				if(m_NextSkillID > 0)
				{
					m_CurrentBattleState = _XUSER_STATE_6;
				}
			}
		}
		break;
	case _XUSER_STATE_6 :
		{
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE);
			m_CurrentBattleState = _XUSER_STATE_7;
			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
		}
		break;
	case _XUSER_STATE_7 :
		{
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
				
				
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
				m_ChangeDelayStartTime = g_LocalSystemTime;
				m_CurrentBattleState = _XUSER_STATE_LAST;
#ifdef _XTS_NEWINSENSIBLE
				if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
				SetAnimation(_XACTION_IDLE);
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			
			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleAreaChannelingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			// 임시로 마우스 포인트 변경
			((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_PICKUP);
			
			// 채널링 영역을 지정하기 위해 마우스 피킹을 계속 계산함.		
			g_LodTerrain.GetPickedPoint( FALSE );
			
			_XParticleGroupItem* pParticleGroupItem = g_WorldEffectInstanceManager.FindInstance( _XDEF_SKILLID_1076 );

			D3DXMATRIX mousemat;
			D3DXMatrixIdentity( &mousemat );
			mousemat._41 = g_vMousePosition.x;
			mousemat._42 = g_vMousePosition.y;
			mousemat._43 = g_vMousePosition.z;

			D3DXVECTOR3 maxsize;

			if( !pParticleGroupItem )
			{ 
				pParticleGroupItem = g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_SKILLID_1076, 
																					 &mousemat );
				if(pParticleGroupItem)
				{
					maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;
					pParticleGroupItem->SetParticleMaxSize( maxsize );
				}
			}
			else
			{
				maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;

				pParticleGroupItem->MovePosition( mousemat );
				pParticleGroupItem->SetParticleMaxSize( maxsize );
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_SKILLID_1076 );
			
			if(!m_bChannelingSendStartPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 2);
				m_bChannelingSendStartPacket = TRUE;
				m_bUsingSkill = TRUE;

				// 채널링 영역을 지정하기 위해 마우스 피킹을 계속 계산함.		
				g_LodTerrain.GetPickedPoint( FALSE );
				
				D3DXMATRIX mousemat;
				D3DXMatrixIdentity( &mousemat );
				mousemat._41 = g_vMousePosition.x;
				mousemat._42 = g_vMousePosition.y;
				mousemat._43 = g_vMousePosition.z;
				
				D3DXVECTOR3 maxsize;
				
				if( m_ModelDescriptor.m_EffectManager )
				{
					_XParticleGroupItem* pParticleGroupItem = m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, m_SelectedSkillID + 40000, &mousemat );
					//_XParticleGroupItem* pParticleGroupItem = g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, m_SelectedSkillID + 40000, &mousemat );
					if(pParticleGroupItem)
					{
						maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;
						pParticleGroupItem->SetParticleMaxSize( maxsize );
					}
				}

				if(g_BattleLog.GetWriteLog())
				{
					g_BattleLog.SetTime();
					g_BattleLog.SetTargetGroup(-1);
					g_BattleLog.SetSkillId(m_SelectedSkillID);
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);
			
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex + _XANI_ORDER_READY);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_4;
		
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		break;
	case _XUSER_STATE_4 :
		{
			D3DXVECTOR3 targetposition;
			targetposition.x = m_RegionTargetPosX;
			targetposition.z = m_RegionTargetPosZ;
			targetposition.y = g_LodTerrain.GetTerrainAndRideOBBHeight(targetposition);

			m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
			UpdatePosition();
			
			if(g_pMinimap_Window)
			{
				g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
				g_pMinimap_Window->SetLocalUserPosition();
			}
			m_CurrentBattleState = _XUSER_STATE_5;
		}
		break;
	case _XUSER_STATE_5 :
		{	
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				short motionindex = GetMotionIndex(m_SelectedSkillID);

				m_ModelDescriptor.SetLockEndFrame(FALSE);
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
				{
					short motionindex = GetMotionIndex(m_SelectedSkillID);
					
					SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT);
					m_CurrentBattleState = _XUSER_STATE_7;
					m_ChannelingSpellRepeatTime = g_LocalSystemTime;
				}
			}
		}
		break;
	case _XUSER_STATE_7 :
		{
			if(m_ChannelingSendLastPacketTime == 0 || g_LocalSystemTime - m_ChannelingSendLastPacketTime > 1000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 2);
				m_ChannelingSendLastPacketTime = g_LocalSystemTime;
			}

			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
			{
				if((g_LocalSystemTime - m_ChannelingSpellRepeatTime) > casting)
				{
					m_CurrentBattleState = _XUSER_STATE_8;
				}
				// 다른 조건으로 state change
				if(m_CharacterInfo.Get_current_forcepower() < _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel))//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce)
				{
					m_CurrentBattleState = _XUSER_STATE_8;
				}
				if(m_NextSkillID > 0)
				{
					m_CurrentBattleState = _XUSER_STATE_8;
				}
			}
			else
			{
				// 다른 조건으로 state change
				if(m_CharacterInfo.Get_current_forcepower() < _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel))//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce)
				{
					m_CurrentBattleState = _XUSER_STATE_8;
				}
				if(m_NextSkillID > 0)
				{
					m_CurrentBattleState = _XUSER_STATE_8;
				}
			}
		}
		break;
	case _XUSER_STATE_8 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE);
			m_CurrentBattleState = _XUSER_STATE_9;
			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
		}
		break;
	case _XUSER_STATE_9 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
#ifdef _XTS_NEWINSENSIBLE
					if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
					SetAnimation(_XACTION_IDLE);
					m_CurrentBattleState = _XUSER_STATE_LAST;
				}
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

BOOL _XLocalUser::ProcessBattleChannelingNextStep(void)
{
	if(m_SelectedSkillID == 0)
		return FALSE;

	if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL)
	{
		if(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_POINT)
		{
			if(m_CurrentBattleState == _XUSER_STATE_1)
			{
				m_CurrentBattleState = _XUSER_STATE_2;

				if(g_LastPickedObject)
				{
					switch(g_LastPickedObjectType)
					{
					case _XPICKINGTARGET_PC :
						{
							m_RegionTargetPosX = ((_XUser*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XUser*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_MOB :
						{
							m_RegionTargetPosX = ((_XMob*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XMob*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_NPC :
						{
							m_RegionTargetPosX = ((_XNPCObject*)g_LastPickedObject)->m_matWorldPosition._41;
							m_RegionTargetPosZ = ((_XNPCObject*)g_LastPickedObject)->m_matWorldPosition._43;
						}
						break;
					case _XPICKINGTARGET_ITEM :
						{
							m_RegionTargetPosX = ((_XGameItem*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XGameItem*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_DUMMY :
						{
							m_RegionTargetPosX = ((_XDummy*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XDummy*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_SEAT :
						{
							m_RegionTargetPosX = ((_XSeat*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XSeat*)g_LastPickedObject)->m_Position.z;
						}
						break;
						// by mahwang
					case _XPICKINGTARGET_COLLECT :
						{
						}
						break;
					case _XPICKINGTARGET_FUNCTIONALOBJECT :
						{
							m_RegionTargetPosX = ((_XFunctionalObject*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XFunctionalObject*)g_LastPickedObject)->m_Position.z;
						}
						break;
					}
				}
				else
				{
					m_RegionTargetPosX = g_vMousePosition.x;
					m_RegionTargetPosZ = g_vMousePosition.z;
				}

				return TRUE;
			}
			else if(m_CurrentBattleState == _XUSER_STATE_7)
			{
				m_CurrentBattleState = _XUSER_STATE_8;
				return TRUE;
			}
		}
		else
		{
			if(m_CurrentBattleState == _XUSER_STATE_5)
			{
				m_CurrentBattleState = _XUSER_STATE_6;
				return TRUE;
			}
		}
	}
	else if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ROUNDINGSPELL || g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CASTINGSPELL)
	{
		if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_POINT)
		{
			if(m_CurrentBattleState == _XUSER_STATE_1)
			{
				m_CurrentBattleState = _XUSER_STATE_2;
				
				if(g_LastPickedObject)
				{
					switch(g_LastPickedObjectType)
					{
					case _XPICKINGTARGET_PC :
						{
							m_RegionTargetPosX = ((_XUser*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XUser*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_MOB :
						{
							m_RegionTargetPosX = ((_XMob*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XMob*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_NPC :
						{
							m_RegionTargetPosX = ((_XNPCObject*)g_LastPickedObject)->m_matWorldPosition._41;
							m_RegionTargetPosZ = ((_XNPCObject*)g_LastPickedObject)->m_matWorldPosition._43;
						}
						break;
					case _XPICKINGTARGET_ITEM :
						{
							m_RegionTargetPosX = ((_XGameItem*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XGameItem*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_DUMMY :
						{
							m_RegionTargetPosX = ((_XDummy*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XDummy*)g_LastPickedObject)->m_Position.z;
						}
						break;
					case _XPICKINGTARGET_SEAT :
						{
							m_RegionTargetPosX = ((_XSeat*)g_LastPickedObject)->m_Position.x;
							m_RegionTargetPosZ = ((_XSeat*)g_LastPickedObject)->m_Position.z;
						}
						break;
						// by mahwang
					case _XPICKINGTARGET_COLLECT :
						{
						}
						break;
					}
				}
				else
				{
					m_RegionTargetPosX = g_vMousePosition.x;
					m_RegionTargetPosZ = g_vMousePosition.z;
				}
				
				return TRUE;
			}
		}
	}

	return FALSE;
}

void _XLocalUser::ProcessBattleFinishSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						FLOAT range = 0.0f;

						if(CheckDistanceToTarget(pMob->m_Position, m_SelectedSkillID, range, pMob->m_MobType))
						{
							FLOAT dx = m_Position.x - pMob->m_Position.x;
							FLOAT dz = m_Position.z - pMob->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
							if(distance < 10.0f)
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2907), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"거리가 너무 가깝습니다."
								ResetSkillAnimation();
							}
							else
							{
								D3DXVECTOR3 mypos = m_Position;
								mypos.y += 0.89f;
								D3DXVECTOR3 targetpos = pMob->m_Position;
								targetpos.y += g_MOBMODEL[pMob->m_MobType]->OBB_Extent[1];
								if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
									ResetSkillAnimation();
								}
								else
								{								
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
									UpdatePosition();
									
									m_CurrentBattleState = _XUSER_STATE_2;
								}
							}
						}
						else
							SetTargetPosition(pMob->m_Position, range);
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)m_AttackTarget;
                        //Author : 양희왕 //breif : 레벨차 공격 안되게
                        if( !pUser )
                            break;

                        if(CheckPKPenalty(pUser))
                            break;

						FLOAT range = 0.0f;
						
						if(CheckDistanceToTarget(pUser->m_Position, m_SelectedSkillID, range, 0))
						{
							FLOAT dx = m_Position.x - pUser->m_Position.x;
							FLOAT dz = m_Position.z - pUser->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
							if(distance < 10.0f)
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2907), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"거리가 너무 가깝습니다."
								ResetSkillAnimation();
							}
							else
							{
								D3DXVECTOR3 mypos = m_Position;
								mypos.y += 0.89f;
								D3DXVECTOR3 targetpos = pUser->m_Position;
								targetpos.y += 0.89f;
								if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )								
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
									ResetSkillAnimation();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
									UpdatePosition();
									
									m_CurrentBattleState = _XUSER_STATE_2;
								}
							}
						}
						else
							SetTargetPosition(pUser->m_Position, range);
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
						if( pFunctionalObject->m_bAttackable )
						{
							FLOAT range = 0.0f;
							
							if(CheckDistanceToTarget(pFunctionalObject->m_Position, m_SelectedSkillID, range, 0))
							{
								FLOAT dx = m_Position.x - pFunctionalObject->m_Position.x;
								FLOAT dz = m_Position.z - pFunctionalObject->m_Position.z;
								FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
								if(distance < 10.0f)
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2907), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"거리가 너무 가깝습니다."
									ResetSkillAnimation();
								}
								else
								{
									D3DXVECTOR3 mypos = m_Position;
									mypos.y += 0.89f;
									D3DXVECTOR3 targetpos = pFunctionalObject->m_Position;
									targetpos.y += 0.89f;
									if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )								
									{
										g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"시야에 가려 시전할 수 없습니다."
										ResetSkillAnimation();
									}
									else
									{
										m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pFunctionalObject->m_Position);
										UpdatePosition();
										
										m_CurrentBattleState = _XUSER_STATE_2;
									}
								}
							}
							else
								SetTargetPosition(pFunctionalObject->m_Position, range);
						}
					}
					break;
				default :
					ResetSkillAnimation();
					break;
				}
			}
			else
			{
				// Target이 없음
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(CheckTargetTypeFinal())
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
				case _XPICKINGTARGET_MOB :
				case _XPICKINGTARGET_PC :
					{
						if(!m_FinishSendPacket)
						{
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY);
							m_FinishSendPacket = TRUE;
							m_bUsingSkill = TRUE;
							
							if(g_BattleLog.GetWriteLog())
							{
								g_BattleLog.SetTime();
								g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
								
								if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
								{
									g_BattleLog.SetTargetType(((_XMob*)m_AttackTarget)->m_MobType);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
								{
									g_BattleLog.SetTargetType(((_XUser*)m_AttackTarget)->m_Uniqueid);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
								{
									g_BattleLog.SetTargetType(((_XFunctionalObject*)m_AttackTarget)->m_ObjectType);
								}
								
								g_BattleLog.SetSkillId(m_SelectedSkillID);
							}
						}
					}
					break;
				default :
					ResetSkillAnimation();
					break;
				}
			}
			else
			{
				if(m_FinishSendPacket)
				{
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
					m_ChangeDelayStartTime = g_LocalSystemTime;
				}
				else
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

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		break;
	case _XUSER_STATE_4 :
		{
//			_XDWINPRINT("[FINISH] STATE_4 MotionIndex [%d]", m_CharacterInfo.animationdescript.attacktype);

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				short motionindex = GetMotionIndex(m_SelectedSkillID);

				m_ModelDescriptor.SetLockEndFrame(FALSE);
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_READY)
				{
					SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
					m_CurrentBattleState = _XUSER_STATE_5;
					int learnedindex = GetLearnedSkillIndex(m_SelectedSkillID);
					m_FinishSpeed = _XSkillItem::GetFinishSpeed(m_SelectedSkillID, m_SkillList[learnedindex].m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SkillList[learnedindex].m_cSkillLevel].finishSpeed;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			// Dash
			if(m_AttackTarget)
			{
//				_XDWINPRINT("[FINISH] STATE_5 MotionIndex [%d]", m_CharacterInfo.animationdescript.attacktype);

				D3DXVECTOR3 targetposition;
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					targetposition = ((_XMob*)m_AttackTarget)->m_Position;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					targetposition = ((_XUser*)m_AttackTarget)->m_Position;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					targetposition = ((_XFunctionalObject*)m_AttackTarget)->m_Position;
				}
				else
				{
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
					m_ChangeDelayStartTime = g_LocalSystemTime;
				}

				if(m_bUsingSkill)
				{
					D3DXVECTOR3 pos, direction;
					direction = targetposition - m_Position;
					D3DXVec3Normalize(&direction, &direction);

					FLOAT range = g_SkillProperty[m_SelectedSkillID]->finishAttackRange;
//					_XDWINPRINT("finishattackrange : %f", g_SkillProperty[m_SelectedSkillID]->finishAttackRange);

					if(g_SkillProperty[m_SelectedSkillID]->finishAttackRange > 0.0f)
						pos = targetposition - (direction*g_SkillProperty[m_SelectedSkillID]->finishAttackRange);
					else
						pos = targetposition - (direction*0.7f);

					pos.y = g_LodTerrain.GetTerrainAndRideOBBHeight(pos);
					m_TargetPosition = pos;
					
					// target쪽으로 쳐다보기
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
					UpdatePosition();

					m_CurrentBattleState = _XUSER_STATE_6;
				}
			}
			else
			{
				m_bUsingSkill = FALSE;
				m_bCheckDelayTime = TRUE;
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
				m_ChangeDelayStartTime = g_LocalSystemTime;
			}
		}
		break;
	case _XUSER_STATE_6 :
		{
//			_XDWINPRINT("[FINISH] STATE_6 MotionIndex [%d]", m_CharacterInfo.animationdescript.attacktype);

			if(m_Position.x == m_TargetPosition.x || m_Position.z == m_TargetPosition.z)
			{
				short motionindex = GetMotionIndex(m_SelectedSkillID);

				SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
				m_CurrentBattleState = _XUSER_STATE_7;

				if(g_SkillProperty[m_SelectedSkillID]->finishAttackRange > 0.0f)	// targetposition 수정
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
					else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
					{
						targetposition = ((_XFunctionalObject*)m_AttackTarget)->m_Position;
					}
					else
					{
						m_bUsingSkill = FALSE;
						m_bCheckDelayTime = TRUE;
						g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
						m_ChangeDelayStartTime = g_LocalSystemTime;
					}

					if(m_bUsingSkill)
					{
						D3DXVECTOR3 pos, direction;
						direction = targetposition - m_Position;
						D3DXVec3Normalize(&direction, &direction);					
						pos = targetposition - (direction*0.7f);
						pos.y = g_LodTerrain.GetTerrainAndRideOBBHeight(pos);
						m_TargetPosition = pos;

						m_FinishSpeed = g_SkillProperty[m_SelectedSkillID]->finishAttackSpeed;
						
						g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
						m_bSendFinishFirePacket = TRUE;
					}
				}
				else
				{
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
					m_bSendFinishFirePacket = TRUE;
				}
			}
		}
		break;
	case _XUSER_STATE_7 :
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
						break;

					// Monster Health 적용(Display용)
					pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;

					D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
					offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );

					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}									
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								pMob->SetBlowMotion();
								PlayAttackSound();

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To Mob [HIT / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								pMob->SetBlowMotion();
								PlayCriticalAttackSound();

								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To Mob [CRITICAL / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

							 #ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To Mob [MISS / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish BLOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Finish To Mob [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock
								if( g_ViewDamageValue )
								{
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish SHOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pMob->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Finish To Mob [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (파)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								pMob->SetBlowMotion();
								PlayAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To Mob [상극일반 / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (필살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								pMob->SetBlowMotion();
								PlayCriticalAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To Mob [상극크리 / damage:%d]", m_AttackResult.sDamage[0]);
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
							
							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pMob->m_MobType;

								m_AttackResult.cBattleResult = 0;
							
								VIEWPACKETLOG
									_XDWINPRINT("[LOCAL USER/ProcessBattleChargeSkill] Mob[%d] kill", pMob->m_MobType);
							}
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
						break;

					// Monster Health 적용(Display용)
					pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;

					D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
					offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );

					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}									
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To FO [HIT / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayCriticalAttackSound();

								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To FO [CRITICAL / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );							
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To FO [MISS / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish BLOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Finish To FO [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock
								if( g_ViewDamageValue )
								{
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Charge SHOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Finish To FO [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (파)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To FO [상극일반 / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (필살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayCriticalAttackSound();

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To FObj [상극크리 / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						default : 
							break;
						}
						
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();			
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							
							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pFunctionalObject->m_ObjectType;



								m_AttackResult.cBattleResult = 0;
								
								//_XDWINPRINT("[LOCAL USER/ProcessBattleChargeSkill] Mob[%d] kill", pMob->m_MobType);
							}
							ResetAttackTarget();
							ResetViewTarget();
							ResetSpellAttackTarget();	
						}
						m_AttackResult.display[0] = FALSE;
					}

				}//en_target_type_object
				else if(m_AttackResult.ucToType == en_target_type_others)
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
						break;

					D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
					offsetmat._42 += 0.98f;
					
					if(m_AttackResult.display[0])
					{
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
							
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To User [HIT / damage:%d]", m_AttackResult.sDamage[0]);							
							}
							break;
						case 1 :
							{
								// Critical
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								PlayCriticalAttackSound();
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To User [CRITICAL / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage2, &pUser->m_ModelDescriptor.m_Position );
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To User [MISS / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 :
							{
								// Block
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 추가 할 것
								}
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish BLOCK"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To User [Block / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 :
							{
								// Shock
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish SHOCK damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								PlayCriticalAttackSound();
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To User [Shock / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayAttackSound();
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To User [상극일반 / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Finish 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								PlayCriticalAttackSound();
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack result : Finish To User [상극크리 / damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						default : 
							break;
						}
						
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							// 내가 죽인 유저
//							m_CharacterLog.isKillClient = TRUE;
//							m_CharacterLog.killTimeClient = g_ServerTimeCode;
//							m_CharacterLog.killTargetTypeClient = 0;


							m_AttackResult.cBattleResult = 0;

							VIEWPACKETLOG
								_XDWINPRINT("[LOCAL USER/ProcessBattleChargeSkill] User[%s] kill", pUser->m_CharacterInfo.charactername);
						}
						m_AttackResult.display[0] = FALSE;
					}
				}
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

				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
				m_ChangeDelayStartTime = g_LocalSystemTime;
				m_CurrentBattleState = _XUSER_STATE_LAST;
#ifdef _XTS_NEWINSENSIBLE
				if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
				SetAnimation(_XACTION_IDLE);
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			// 위치 업데이트 - 주위 몬스터에게 알림
			if(!m_bSendFinishPosUpdatePacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				m_bSendFinishPosUpdatePacket = TRUE;
			}

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}	
}

void _XLocalUser::ProcessBattleSelfCastingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(!m_CastingSendPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 1);
				m_CastingSendPacket = TRUE;
				m_bUsingSkill = TRUE;

				if(g_BattleLog.GetWriteLog())
				{
					g_BattleLog.SetTime();
					g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
					g_BattleLog.SetTargetType(g_pLocalUser->m_Uniqueid);
					g_BattleLog.SetSkillId(m_SelectedSkillID);
				}

			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);

			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex + _XANI_ORDER_READY);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_4 ;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
					SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
					m_ModelDescriptor.SetLockEndFrame(TRUE);
					m_CurrentBattleState = _XUSER_STATE_5;
					m_CastingSpellRepeatTime = g_LocalSystemTime;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			// casting time 만큼 지속
			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
			{
				if((g_LocalSystemTime - m_CastingSpellRepeatTime) > casting)
				{
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
					{
						short motionindex = GetMotionIndex(m_SelectedSkillID);
						m_ModelDescriptor.SetLockEndFrame(FALSE);
						if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
							m_CurrentBattleState = _XUSER_STATE_6;
					}
				}
			}
			else
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					short motionindex = GetMotionIndex(m_SelectedSkillID);
					m_ModelDescriptor.SetLockEndFrame(FALSE);
					if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_REPEAT)
						m_CurrentBattleState = _XUSER_STATE_6;
				}
			}
		}
		break;
	case _XUSER_STATE_6 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
			m_CurrentBattleState = _XUSER_STATE_7;

			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 1);
		}
		break;
	case _XUSER_STATE_7 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);

			if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
				{
				}
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();

					m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
					m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					m_ChangeDelayStartTime = g_LocalSystemTime;
#ifdef _XTS_NEWINSENSIBLE
					if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
					SetAnimation(_XACTION_IDLE);
					m_CurrentBattleState = _XUSER_STATE_LAST;
				}
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if((g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime) > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleTargetCastingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						FLOAT range = 0.0f;

						if(CheckDistanceToTarget(pMob->m_Position, m_SelectedSkillID, range, pMob->m_MobType))
						{
							D3DXVECTOR3 mypos = m_Position;
							mypos.y += 0.89f;
							D3DXVECTOR3 targetpos = pMob->m_Position;
							targetpos.y += g_MOBMODEL[pMob->m_MobType]->OBB_Extent[1];
							if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
								ResetSkillAnimation();
							}
							else
							{
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
								UpdatePosition();

								m_CurrentBattleState = _XUSER_STATE_2;
							}
						}
						else
							SetTargetPosition(pMob->m_Position, range);
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)m_AttackTarget;
                        //Author : 양희왕 //breif : 레벨차 공격 안되게
                        if( !pUser )
                            break;

                        if(CheckPKPenalty(pUser))
                            break;

						FLOAT range = 0.0f;

						if(CheckDistanceToTarget(pUser->m_Position, m_SelectedSkillID, range, 0))
						{
							D3DXVECTOR3 mypos = m_Position;
							mypos.y += 0.89f;
							D3DXVECTOR3 targetpos = pUser->m_Position;
							targetpos.y += 0.89f;
							if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
								ResetSkillAnimation();
							}
							else
							{
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
								UpdatePosition();

								m_CurrentBattleState = _XUSER_STATE_2;
							}
						}
						else
							SetTargetPosition(pUser->m_Position, range);
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
						if( pFunctionalObject->m_bAttackable )
						{
							FLOAT range = 0.0f;
							if(CheckDistanceToTarget(pFunctionalObject->m_Position, m_SelectedSkillID, range, 0))
							{
								D3DXVECTOR3 mypos = m_Position;
								mypos.y += 0.89f;
								D3DXVECTOR3 targetpos = pFunctionalObject->m_Position;
								targetpos.y += 0.89f;
								if( !g_LodTerrain.CheckRayPass( mypos, targetpos ) )							
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"시야에 가려 시전할 수 없습니다."
									ResetSkillAnimation();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pFunctionalObject->m_Position);
									UpdatePosition();
									
									m_CurrentBattleState = _XUSER_STATE_2;
								}
							}
							else
								SetTargetPosition(pFunctionalObject->m_Position, range);
						}
					}
					break;
				case _XPICKINGTARGET_SELF :
					m_CurrentBattleState = _XUSER_STATE_2;
					break;
				default :
					ResetSkillAnimation();
					break;
				}
			}
			else
			{
				// Target이 없음
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(CheckTargetTypeFinal())
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
				case _XPICKINGTARGET_MOB :
				case _XPICKINGTARGET_PC :
					{
						if(!m_CastingSendPacket)
						{
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 0);
							m_CastingSendPacket = TRUE;
							m_bUsingSkill = TRUE;

							if(g_BattleLog.GetWriteLog())
							{
								g_BattleLog.SetTime();
								g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
								
								if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
								{
									g_BattleLog.SetTargetType(((_XMob*)m_AttackTarget)->m_MobType);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
								{
									g_BattleLog.SetTargetType(((_XUser*)m_AttackTarget)->m_Uniqueid);
								}
								else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
								{
									g_BattleLog.SetTargetType(((_XFunctionalObject*)m_AttackTarget)->m_ObjectType);
								}
								
								g_BattleLog.SetSkillId(m_SelectedSkillID);
							}
						}
					}
					break;
				case _XPICKINGTARGET_SELF :
					{
						if(!m_CastingSendPacket)
						{
							g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 1);
							m_CastingSendPacket = TRUE;
							m_bUsingSkill = TRUE;

							if(g_BattleLog.GetWriteLog())
							{
								g_BattleLog.SetTime();
								g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);							
								g_BattleLog.SetTargetType(m_Uniqueid);
								g_BattleLog.SetSkillId(m_SelectedSkillID);
							}
						}
					}
					break;
				default :
					ResetSkillAnimation();
					break;
				}
			}
			else
			{
				if(m_CastingSendPacket)
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_END);
				else
					ResetSkillAnimation();

				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);

			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex + _XANI_ORDER_READY);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_4;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
					SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
					m_CurrentBattleState = _XUSER_STATE_5;
					m_CastingSpellRepeatTime = g_LocalSystemTime;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			// casting time만큼 지속
			if(m_AttackTarget)
			{
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						_XMob* pMob = (_XMob*)m_AttackTarget;
						if(pMob)
						{
							m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
							UpdatePosition();
						}
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						_XUser* pUser = (_XUser*)m_AttackTarget;
						if(pUser)
						{
							m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
							UpdatePosition();
						}
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT :
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
						if(pFunctionalObject)
						{
							m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pFunctionalObject->m_Position);
							UpdatePosition();
						}
					}
					break;
				}
			}
			
			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
			{	
				if((g_LocalSystemTime - m_CastingSpellRepeatTime) > casting)
				{
					// 반복 animation 끝
					m_CurrentBattleState = _XUSER_STATE_6;
				}
			}
			else
			{
				// 반복 animation 끝
				m_CurrentBattleState = _XUSER_STATE_6;
			}
		}
		break;
	case _XUSER_STATE_6 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
			m_CurrentBattleState = _XUSER_STATE_7;

			if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 1);
			else 
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 0);
		}
		break;
	case _XUSER_STATE_7 :
		{//Author : 양희왕 //breif : 기공 대미지 표시 때문에 수정함 
			/*if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				if(!m_AttackResult.display[0])
					break;
				
				if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_OTHER_ENEMY)
				{
					if(m_AttackResult.ucToType == en_target_type_monster)
					{
						_XMob* pMob = g_Moblist.FindMob(m_AttackResult.usDefenderID);
						if(!pMob)
							break;

						// Monster Health 적용(Display용)
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting HIT damage[%d]", m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To Mob [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting CRITICAL damage[%d]", m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To Mob [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting MISS");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To Mob [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting BLOCK ");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Casting To Mob [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock

								if( g_ViewDamageValue )
								{
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting SHOCK ");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pMob->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Casting To Mob [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (파)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting 상극일반 damage[%d]", m_AttackResult.sDamage[0]);	// add string
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To Mob [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (필살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting 상극크리 damage[%d]", m_AttackResult.sDamage[0]);	// add string
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To Mob [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
							pMob->SetAnimation(_XMOBACTION_DIE);
							pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
							
							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pMob->m_MobType;

								
								m_AttackResult.cBattleResult = 0;
								
								VIEWPACKETLOG
									_XDWINPRINT("[LocalUser/ProcessBattleChargeSkill] Mob [%d] kill", pMob->m_MobType);
							}
						}

						m_AttackResult.display[0] = FALSE;
					}
					else if(m_AttackResult.ucToType == en_target_type_object)
					{
						_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
						if(!pFunctionalObject)
							break;

						// Monster Health 적용(Display용)
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}									
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting HIT damage[%d]", m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To FO [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting CRITICAL damage[%d]", m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To FO [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting MISS");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To FO [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting BLOCK ");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Casting To FO [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock
								if( g_ViewDamageValue )
								{
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )									
									{
										D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting SHOCK ");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Casting To FO [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (파)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting 상극일반 damage[%d]", m_AttackResult.sDamage[0]);	// add string
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To FO [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (필살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting 상극필살 damage[%d]", m_AttackResult.sDamage[0]);	// add string
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To FO [상극필살/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();			
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
							
							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pFunctionalObject->m_ObjectType;

								
								m_AttackResult.cBattleResult = 0;
								
								//_XDWINPRINT("[LocalUser/ProcessBattleChargeSkill] Mob [%d] kill", pMob->m_MobType);
							}
							ResetAttackTarget();
							ResetViewTarget();
							ResetSpellAttackTarget();	
						}

						m_AttackResult.display[0] = FALSE;
						
					}// en_target_type_object
					else if(m_AttackResult.ucToType == en_target_type_others)
					{
						_XUser* pUser = g_Userlist.FindUser(m_AttackResult.usDefenderID);
						if(!pUser)
							break;

						D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// Hit
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting HIT damage[%d]", m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To User [Hit/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// Critical
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting CRITICAL damage[%d]", m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To User [Critical/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// Miss
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage2, &pUser->m_ModelDescriptor.m_Position );
								}

								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting MISS");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}

								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To User [Miss/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 :
							{
								// Block
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 추가 할 것
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting BLOCK");
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To User [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 :
							{
								// Shock
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting SHOCK damage[%d]", m_AttackResult.sDamage[0]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To User [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (타)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting 상극일반 damage[%d]", m_AttackResult.sDamage[0]);	// add string
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To User [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (살)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Casting 상극크리 damage[%d]", m_AttackResult.sDamage[0]);	// add string
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
								
								VIEWPACKETLOG
									_XDWINPRINT("-- Attack Result : Casting To User [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						}
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							// 내가 죽인 유저
//							m_CharacterLog.isKillClient = TRUE;
//							m_CharacterLog.killTimeClient = g_ServerTimeCode;
//							m_CharacterLog.killTargetTypeClient = 0;

							
							m_AttackResult.cBattleResult = 0;
						}

						m_AttackResult.display[0] = FALSE;
					}
				} // other-enemy
				
				m_AttackResult.display[0] = FALSE;
				m_bCheckFirstFrame = FALSE;
			} // command frame */
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				short motionindex = GetMotionIndex(m_SelectedSkillID);
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
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

					m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
					m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					m_ChangeDelayStartTime = g_LocalSystemTime;
					m_CurrentBattleState = _XUSER_STATE_LAST;
#ifdef _XTS_NEWINSENSIBLE
					if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
					SetAnimation(_XACTION_IDLE);
				}
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleAreaCastingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			// 마우스 포인트 변경
			((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_PICKUP);
			
			// 영역을 지정하기 위해 마우스 피킹을 계속 계산함.		
			g_LodTerrain.GetPickedPoint( FALSE );
			
			_XParticleGroupItem* pParticleGroupItem = g_WorldEffectInstanceManager.FindInstance( _XDEF_SKILLID_1076 );
			
			D3DXMATRIX mousemat;
			D3DXMatrixIdentity( &mousemat );
			mousemat._41 = g_vMousePosition.x;
			mousemat._42 = g_vMousePosition.y;
			mousemat._43 = g_vMousePosition.z;
			
			D3DXVECTOR3 maxsize;
			
			if( !pParticleGroupItem )
			{ 
				pParticleGroupItem = g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_SKILLID_1076, 
					&mousemat );
				if(pParticleGroupItem)
				{
					maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;
					pParticleGroupItem->SetParticleMaxSize( maxsize );
				}
			}
			else
			{
				maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;				
				pParticleGroupItem->MovePosition( mousemat );
				pParticleGroupItem->SetParticleMaxSize( maxsize );
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_SKILLID_1076 );
			
			if(!m_CastingSendPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_READY, 2);
				m_CastingSendPacket = TRUE;
				m_bUsingSkill = TRUE;
				
				// 채널링 영역을 지정하기 위해 마우스 피킹을 계속 계산함.		
				g_LodTerrain.GetPickedPoint( FALSE );
				
				D3DXMATRIX mousemat;
				D3DXMatrixIdentity( &mousemat );
				mousemat._41 = g_vMousePosition.x;
				mousemat._42 = g_vMousePosition.y;
				mousemat._43 = g_vMousePosition.z;
				
				D3DXVECTOR3 maxsize;
				
				if( m_ModelDescriptor.m_EffectManager )
				{
					_XParticleGroupItem* pParticleGroupItem = m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, m_SelectedSkillID + 40000, &mousemat );
					if(pParticleGroupItem)
					{
						maxsize.x = maxsize.y = maxsize.z = g_SkillProperty[m_SelectedSkillID]->radius * 2.0f;
						pParticleGroupItem->SetParticleMaxSize( maxsize );
					}
				}

				if(g_BattleLog.GetWriteLog())
				{
					g_BattleLog.SetTime();
					g_BattleLog.SetTargetGroup(-1);
					g_BattleLog.SetSkillId(m_SelectedSkillID);
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			// 모든 무기 type이면 들고 있던 무기 삭제
			DeleteWeaponModel(m_SelectedSkillID);

			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex);
			m_ModelDescriptor.SetLockEndFrame(TRUE);
			m_CurrentBattleState = _XUSER_STATE_4;
			
			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
					SetAnimationIndex(motionindex + _XANI_ORDER_REPEAT, FALSE);
					m_CurrentBattleState = _XUSER_STATE_5;
					m_CastingSpellRepeatTime = g_LocalSystemTime;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			// casting time만큼 지속
			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);
			if(casting > 0)
			{	
				if((g_LocalSystemTime - m_CastingSpellRepeatTime) > casting)
				{
					// 반복 animation 끝
					m_CurrentBattleState = _XUSER_STATE_6;
				}
			}
			else
			{
				// 반복 animation 끝
				m_CurrentBattleState = _XUSER_STATE_6;
			}
		}
		break;
	case _XUSER_STATE_6 :
		{
			short motionindex = GetMotionIndex(m_SelectedSkillID);

			SetAnimationIndex(motionindex + _XANI_ORDER_FIRE, FALSE);
			m_CurrentBattleState = _XUSER_STATE_7;

			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, 2);
		}
		break;
	case _XUSER_STATE_7 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				short motionindex = GetMotionIndex(m_SelectedSkillID);
				if(m_ModelDescriptor.GetCurMotion() == motionindex + _XANI_ORDER_FIRE)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();
					
					m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
					m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					m_ChangeDelayStartTime = g_LocalSystemTime;
					m_CurrentBattleState = _XUSER_STATE_LAST;
#ifdef _XTS_NEWINSENSIBLE
					if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
					SetAnimation(_XACTION_IDLE);
				}
			}
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			
			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleSmashingSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				D3DXVECTOR3 targetposition;
				unsigned short targettype;

                //Author : 양희왕 //breif : 레벨차 공격 안되게
                if( m_AttackTargetObjectType == _XPICKINGTARGET_PC )
                {
                    _XUser* pUser = NULL;
		            pUser = (_XUser*)m_AttackTarget;
                    if( !pUser )
                        break;

                    if(CheckPKPenalty(pUser))
                        break;
                }

				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						targetposition = ((_XMob*)m_AttackTarget)->m_Position;
						targettype = ((_XMob*)m_AttackTarget)->m_MobType;
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						targetposition = ((_XUser*)m_AttackTarget)->m_Position;
						targettype = 0;
					}
					break;
				case _XPICKINGTARGET_SELF :
					{
						targetposition = g_pLocalUser->m_Position;
						targettype = 0;
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT:
					{
						targetposition = ((_XFunctionalObject*)m_AttackTarget)->m_Position;
						targettype = ((_XFunctionalObject*)m_AttackTarget)->m_ObjectType;
					}
					break;
				default :
					{
						ResetSkillAnimation();
						return;
					}
				}
				FLOAT range = 0.0f;

				if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
				{
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
					UpdatePosition();
					m_CurrentBattleState = _XUSER_STATE_2;
				}
				else
					SetTargetPosition(targetposition, range);
			}
			else
			{
				// target이 없어지면 reset
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(CheckTargetTypeFinal())
			{
				if(!m_SmashingSendPacket)
				{
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
					m_SmashingSendPacket = TRUE;
					m_bUsingSkill = TRUE;

					if(g_BattleLog.GetWriteLog())
					{
						g_BattleLog.SetTime();
						g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
						
						if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
						{
							g_BattleLog.SetTargetType(((_XMob*)m_AttackTarget)->m_MobType);
						}
						else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
						{
							g_BattleLog.SetTargetType(((_XUser*)m_AttackTarget)->m_Uniqueid);
						}
						else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
						{
							g_BattleLog.SetTargetType(((_XFunctionalObject*)m_AttackTarget)->m_ObjectType);
						}
						
						g_BattleLog.SetSkillId(m_SelectedSkillID);
					}
				}
			}
			else
			{
				// packet 보내기 전 target이 없어지면 reset
				if(!m_SmashingSendPacket)
					ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XMob*)m_AttackTarget)->m_Position);
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XUser*)m_AttackTarget)->m_Position);
				UpdatePosition();
			}
			
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex);
			m_CurrentBattleState = _XUSER_STATE_4;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		break;
	case _XUSER_STATE_4 :
		{
//			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
			if(m_ModelDescriptor.m_pBipedController->GetFirstFrame()+5 >= m_ModelDescriptor.m_AniFrame &&
				m_ModelDescriptor.m_AniFrame > 0.0f && !m_bCheckFirstFrame)
			{
				m_bCheckFirstFrame = TRUE;
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				// Display Region Damage
				if(m_RegionDamageResult.bValid == TRUE)
				{
					if(m_SelectedSkillID == m_RegionDamageResult.nSkillID)
					{
						_XMob* pMob = NULL;
						_XUser* pUser = NULL;

						for(int i = 0 ; i < m_RegionDamageResult.nObjectCnt ; ++i)
						{
							pMob = NULL;
							pUser = NULL;

							if(m_RegionDamageResult.sResult[i].nObjectType == 0)
							{
								pMob = (_XMob*)g_Moblist.FindMob(m_RegionDamageResult.sResult[i].nID);
								if(pMob)
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
							else if(m_RegionDamageResult.sResult[i].nObjectType == 1)
							{
								pUser = (_XUser*)g_Userlist.FindUser(m_RegionDamageResult.sResult[i].nID);
								if(pUser)
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서)
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
								}
							}
						}
					}
					m_RegionDamageResult.bValid = FALSE;
				}
				
				// Display result
				if(m_AttackResult.display[0])
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
							break;

						// Monster Health 적용(Display용)
						pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;

						D3DXMATRIX offsetmat = pMob->m_ModelDescriptor.m_Position;
						offsetmat._42 += ( 0.15f + g_MobAttribTable[pMob->m_MobType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
					
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// HIT
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);
									
									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								pMob->SetBlowMotion();
								PlayAttackSound();

								// Visual Effect - 터지는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif									
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/HIT/Mob] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// CRITICAL
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}

								pMob->SetBlowMotion();
								PlayCriticalAttackSound();

								// Visual Effect - 터지는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
								}
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/CRITICAL/Mob] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// MISS
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/MISS/Mob] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing BLOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Smashing To Mob [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock
								if( g_ViewDamageValue )
								{
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

								// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing SHOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pMob->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Smashing To Mob [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (파)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								pMob->SetBlowMotion();
								PlayAttackSound();
								
								// Visual Effect - 터지는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/상극일반/Mob] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pMob->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (필살)
									D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								pMob->SetBlowMotion();
								PlayCriticalAttackSound();
								
								// Visual Effect - 터지는 이펙트
								if( pMob->m_ModelDescriptor.m_EffectManager )
								{
									pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
								}
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/상극크리/Mob] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						}

						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
							pMob->SetAnimation(_XMOBACTION_DIE);
							pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);

							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pMob->m_MobType;


								m_AttackResult.cBattleResult = 0;

								VIEWPACKETLOG
									_XDWINPRINT("[LocalUser/ProcessBattleSmashingSpell] Mob[%d] Kill", pMob->m_MobType);
							}
						}
					}// if(m_AttackResult.ucToType == en_target_type_monster)
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
							break;

						// Monster Health 적용(Display용)
						pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;

						D3DXMATRIX offsetmat = pFunctionalObject->m_ModelDescriptor.m_Position;
						offsetmat._42 += ( 0.15f + g_MobAttribTable[pFunctionalObject->m_ObjectType].ysize * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
					
						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// HIT
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (타)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();

								// Visual Effect - 터지는 이펙트
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), "Smashing HIT damage[%d]", m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/HIT/FO] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// CRITICAL
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );										
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayCriticalAttackSound();

								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
								}
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), "Smashing CRITICAL damage[%d]", m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/CRITICAL/FO] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// MISS
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/MISS/Mob] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 : 
							{
								// Block
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing BLOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Smashing To FO [Block/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 : 
							{
								// Shock
								if( g_ViewDamageValue )
								{
									if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
								}

								// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing SHOCK "));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
								if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
								{
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
								}
#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("Attack result : Smashing To FO [Shock/damage:%d]", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (파)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayAttackSound();
								
								// Visual Effect - 터지는 이펙트
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											}
											else
											{
												pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
											}
										}
									}
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										}
										else
										{
											pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#endif
								}
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/상극일반/FO] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pFunctionalObject->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (필살)
									D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
									resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayCriticalAttackSound();
								
								if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
								{
									pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
								}
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/상극크리/FO] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						}

						if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						{
							pFunctionalObject->m_HealthPercentage = 0;
							pFunctionalObject->SetDieAnimation();			
							pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);

							if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
							{
								// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pFunctionalObject->m_ObjectType;

								m_AttackResult.cBattleResult = 0;

								//_XDWINPRINT("[LocalUser/ProcessBattleSmashingSpell] Mob[%d] Kill", pMob->m_MobType);
							}
							ResetAttackTarget();
							ResetViewTarget();
							ResetSpellAttackTarget();	
						}
					}//en_target_type_object
					else if(m_AttackResult.ucToType == en_target_type_others)
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
							break;

						D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
						offsetmat._42 += 0.98f;

						switch(m_AttackResult.usAtkInfo[0])
						{
						case 0 :
							{
								// HIT
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (타)									
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position);
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayAttackSound();

								// Visual Effect - 맞는 이펙트
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
												pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											else
												pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);										
										}
									}
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing HIT damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/HIT/User] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 1 :
							{
								// CRITICAL
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position);
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}

								PlayCriticalAttackSound();
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );

								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/CRITICAL/User] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 2 :
							{
								// MISS
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - (실)
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage2, &pUser->m_ModelDescriptor.m_Position);
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing MISS"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif

								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/MISS/User] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 3 :
							{
								// BLOCK
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 추가해야 함
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing BLOCK"));

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/BLOCK/User] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 4 :
							{
								// SHOCK
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position);
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayCriticalAttackSound();
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
								
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
								}
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing SHOCK damage[%d]"), m_AttackResult.sDamage[0]);

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/SHOCK/User] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 5 :
							{
								// 상극일반
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (타)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position);
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayAttackSound();
								
								// Visual Effect - 맞는 이펙트
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
										if( WeaponItem )
										{
											if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
												pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
											else
												pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
										}
									}
#else
									if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
									{
										if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
									{
										if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
									else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
									{
										if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT2, &offsetmat);
										else
											pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT1, &offsetmat);
									}
#endif
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/상극일반/User] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						case 6 :
							{
								// 상극크리
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									// Visual Effect - 맞는 이펙트(스킬테이블에서) - 2008.04.14 추가
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_SkillProperty[m_SelectedSkillID]->visualEffect, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);

									// Visual Effect - (살)
									if( g_ViewDamageValue )
									{
										int iDamageValue = m_AttackResult.sDamage[0];
										if(iDamageValue > 0)
												g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
									}
									else
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position);
									}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
									if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
									{
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
											m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
									}
#endif
								}
								
								PlayCriticalAttackSound();
								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
								
								if( pUser->m_ModelDescriptor.m_EffectManager )
								{
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_COMMONID_HIT6, &offsetmat);
								}
								
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									memset(messagestring, 0, sizeof(messagestring));

									_snprintf(messagestring, sizeof(messagestring), _T("Smashing 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                                    g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                                }
							#endif
								
								VIEWPACKETLOG
									_XDWINPRINT("--- AttackResult [Smashing/상극크리/User] Damage:%d", m_AttackResult.sDamage[0]);
							}
							break;
						}
						
						if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
						{
							// 내가 죽인 유저
//							m_CharacterLog.isKillClient = TRUE;
//							m_CharacterLog.killTimeClient = g_ServerTimeCode;
//							m_CharacterLog.killTargetTypeClient = 0;

							m_AttackResult.cBattleResult = 0;

							VIEWPACKETLOG
								_XDWINPRINT("[LocalUser/ProcessBattleSmashingSpell] User[%s] Kill", pUser->m_CharacterInfo.charactername);
						}
					} // else if(m_AttackResult.ucToType == en_target_type_others)
					m_AttackResult.display[0] = FALSE;
					m_bCheckFirstFrame = FALSE;
				} // if(m_AttackResult.display[0])
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
				
				m_bCheckFirstFrame = FALSE;
				m_CurrentBattleState = _XUSER_STATE_5;
#ifdef _XTS_NEWINSENSIBLE
				if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
				SetAnimation(_XACTION_IDLE);
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
			m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			m_ChangeDelayStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			m_bRestartActive = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
//					_XDWINPRINT("cooldown : %d  / elapsedtime : %d", m_SelectedSkillItem->m_dwCooldownTime, g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime);
					
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ProcessBattleActionSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH) return;
	
	switch(m_CurrentBattleState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_AttackTarget)
			{
				D3DXVECTOR3 targetposition;
				unsigned short targettype;
				switch(m_AttackTargetObjectType)
				{
				case _XPICKINGTARGET_MOB :
					{
						targetposition = ((_XMob*)m_AttackTarget)->m_Position;
						targettype = ((_XMob*)m_AttackTarget)->m_MobType;
					}
					break;
				case _XPICKINGTARGET_PC :
					{
						targetposition = ((_XUser*)m_AttackTarget)->m_Position;
						targettype = 0;
					}
					break;
				case _XPICKINGTARGET_SELF :
					{
						targetposition = g_pLocalUser->m_Position;
						targettype = 0;
					}
					break;
				case _XPICKINGTARGET_FUNCTIONALOBJECT:
					{
						targetposition = ((_XFunctionalObject*)m_AttackTarget)->m_Position;
						targettype = ((_XFunctionalObject*)m_AttackTarget)->m_ObjectType;
					}
					break;
				default :
					{
						ResetSkillAnimation();
						return;
					}
				}
				FLOAT range = 0.0f;

				if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
				{
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
					UpdatePosition();
					m_CurrentBattleState = _XUSER_STATE_2;
				}
				else
					SetTargetPosition(targetposition, range);
			}
			else
			{
				// target이 없어지면 reset
				ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(CheckTargetTypeFinal())
			{
				if(!m_ActionSendPacket)
				{
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
					m_ActionSendPacket = TRUE;
					m_bUsingSkill = TRUE;
				}
			}
			else
			{
				// packet 보내기 전 target이 없어지면 reset
				if(!m_ActionSendPacket)
					ResetSkillAnimation();
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XMob*)m_AttackTarget)->m_Position);
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, ((_XUser*)m_AttackTarget)->m_Position);
				UpdatePosition();
			}
			
			short motionindex = GetMotionIndex(m_SelectedSkillID);
			
			SetAnimationIndex(motionindex);
			m_CurrentBattleState = _XUSER_STATE_4;

			// 위치 update
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		break;
	case _XUSER_STATE_4 :
		{
//			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
			if(m_ModelDescriptor.m_pBipedController->GetFirstFrame()+5 >= m_ModelDescriptor.m_AniFrame &&
				m_ModelDescriptor.m_AniFrame > 0.0f && !m_bCheckFirstFrame)
			{
				m_bCheckFirstFrame = TRUE;
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				m_bCheckFirstFrame = FALSE;
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_bCheckFirstFrame = FALSE;
				m_CurrentBattleState = _XUSER_STATE_5;
#ifdef _XTS_NEWINSENSIBLE
				if(GetMotionClass() != _XACTION_INSENSIBLE)
#endif
				SetAnimation(_XACTION_IDLE);
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
			m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			m_ChangeDelayStartTime = g_LocalSystemTime;
			m_CurrentBattleState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			m_bRestartActive = TRUE;

			if(m_SelectedSkillItem->m_dwCooldownTime > 0)
			{
				if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime)
				{
					TCHAR messagestring[512];
					memset(messagestring, 0, sizeof(messagestring));

					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
					m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					ResetSkillAnimation();
				}
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(messagestring));

				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				m_SelectedSkillItem->m_dwCooldownStartTime = 0;
				ResetSkillAnimation();
			}
		}
		break;
	}
}

void _XLocalUser::ViewCastingDamage(void)
{
	if(!m_AttackResult.display[0])
		return;
	
	if(_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_OTHER_ENEMY)
	{
		if(m_AttackResult.ucToType == en_target_type_monster)
		{
			_XMob* pMob = g_Moblist.FindMob(m_AttackResult.usDefenderID);
			if(!pMob)
				return;

			// Monster Health 적용(Display용)
			pMob->m_DisplayHealthPercentage = pMob->m_HealthPercentage;

            //_XLog( "m_AttackResult.usAtkInfo[0] %d", m_AttackResult.usAtkInfo[0] );
			switch(m_AttackResult.usAtkInfo[0])
			{
			case 0 :
				{
					// Hit
					if( pMob->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (타)
						D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting HIT damage[%d]"), m_AttackResult.sDamage[0]);

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To Mob [Hit/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 1 :
				{
					// Critical
					if( pMob->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (살)
						D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
                            //_XLog( "몬스터크리");
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To Mob [Critical/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 2 :
				{
					// Miss
					if( pMob->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (실)
						D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting MISS"));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To Mob [Miss/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 3 : 
				{
					// Block
				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting BLOCK "));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("Attack result : Casting To Mob [Block/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 4 : 
				{
					// Shock

					if( g_ViewDamageValue )
					{
						if( pMob->m_ModelDescriptor.m_EffectManager )
						{
							D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
							resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting SHOCK "));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
					if(pMob->m_ModelDescriptor.m_EffectManager)
					{
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
						}
					}
#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("Attack result : Casting To Mob [Shock/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 5 :
				{
					// 상극일반
					if( pMob->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (파)
						D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To Mob [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 6 :
				{
					// 상극크리
					if( pMob->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (필살)
						D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
                            //_XLog( "몬스터상극크리");    
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pMob->m_ModelDescriptor.m_Position);
						}
#endif
					}
					
				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To Mob [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			}
			if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
			{
				pMob->m_Action.dietype = _XMONSTERDIETYPE_DIE2;
				pMob->SetAnimation(_XMOBACTION_DIE);
				pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
				
				if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
				{
					// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pMob->m_MobType;

					
					m_AttackResult.cBattleResult = 0;
					
					VIEWPACKETLOG
						_XDWINPRINT("[LocalUser/ProcessBattleChargeSkill] Mob [%d] kill", pMob->m_MobType);
				}
			}

			m_AttackResult.display[0] = FALSE;
		}
		else if(m_AttackResult.ucToType == en_target_type_object)
		{
			_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(m_AttackResult.usDefenderID);
			if(!pFunctionalObject)
				return;

			// Monster Health 적용(Display용)
			pFunctionalObject->m_DisplayHealthPercentage = pFunctionalObject->m_HealthPercentage;

			switch(m_AttackResult.usAtkInfo[0])
			{
			case 0 :
				{
					// Hit
					if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (타)
						D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}									
						else
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting HIT damage[%d]"), m_AttackResult.sDamage[0]);

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To FO [Hit/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 1 :
				{
					// Critical
					if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (살)
						D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
                            //_XLog( "오브젝트크리");
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );									
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif			

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To FO [Critical/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 2 :
				{
					// Miss
					if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (실)
						D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_AGI, &resulteffectmatrix );
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting MISS"));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To FO [Miss/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 3 : 
				{
					// Block
				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting BLOCK "));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
					}
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("Attack result : Casting To FO [Block/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 4 : 
				{
					// Shock
					if( g_ViewDamageValue )
					{
						if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )									
						{
							D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
							resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);

							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting SHOCK "));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
					if(pFunctionalObject->m_ModelDescriptor.m_EffectManager)
					{
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
						}
					}
#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("Attack result : Casting To FO [Shock/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 5 :
				{
					// 상극일반
					if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (파)
						D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To FO [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 6 :
				{
					// 상극크리
					if( pFunctionalObject->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (필살)
						D3DXMATRIX resulteffectmatrix = pFunctionalObject->m_ModelDescriptor.m_Position;
						resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pFunctionalObject->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
						
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &resulteffectmatrix, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
                            //_XLog( "오브젝트크리");
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pFunctionalObject->m_ModelDescriptor.m_Position);
						}
#endif
					}
					
				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극필살 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To FO [상극필살/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			}
			if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
			{
				pFunctionalObject->m_HealthPercentage = 0;
				pFunctionalObject->SetDieAnimation();			
				pFunctionalObject->m_ModelDescriptor.SetLockEndFrame(TRUE);
				
				if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die)
				{
					// 내가 죽인 몹
//								m_CharacterLog.isKillClient = TRUE;
//								m_CharacterLog.killTimeClient = g_ServerTimeCode;
//								m_CharacterLog.killTargetTypeClient = pFunctionalObject->m_ObjectType;

					
					m_AttackResult.cBattleResult = 0;
					
					//_XDWINPRINT("[LocalUser/ProcessBattleChargeSkill] Mob [%d] kill", pMob->m_MobType);
				}
				ResetAttackTarget();
				ResetViewTarget();
				ResetSpellAttackTarget();	
			}

			m_AttackResult.display[0] = FALSE;
			
		}// en_target_type_object
		else if(m_AttackResult.ucToType == en_target_type_others)
		{
			_XUser* pUser = g_Userlist.FindUser(m_AttackResult.usDefenderID);
			if(!pUser)
				return;

			D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
			offsetmat._42 += 0.98f;

            //_XLog( "m_AttackResult.usAtkInfo[0] %d", m_AttackResult.usAtkInfo[0] );
			switch(m_AttackResult.usAtkInfo[0])
			{
			case 0 :
				{
					// Hit
					if( pUser->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (타)									
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
                            //_XLog( "PK 일반");
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting HIT damage[%d]"), m_AttackResult.sDamage[0]);

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To User [Hit/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 1 :
				{
					// Critical
					if( pUser->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (살)
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
                            //_XLog( "PK크리");
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting CRITICAL damage[%d]"), m_AttackResult.sDamage[0]);

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To User [Critical/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 2 :
				{
					// Miss
					if( pUser->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (실)
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage2, &pUser->m_ModelDescriptor.m_Position );
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting MISS"));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif		

					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To User [Miss/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 3 :
				{
					// Block
					if( pUser->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - 추가 할 것
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting BLOCK"));

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To User [Block/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 4 :
				{
					// Shock
					if( pUser->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (살)
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting SHOCK damage[%d]"), m_AttackResult.sDamage[0]);

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To User [Shock/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 5 :
				{
					// 상극일반
					if( pUser->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (타)
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_damage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage1, &pUser->m_ModelDescriptor.m_Position );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극일반 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To User [상극일반/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			case 6 :
				{
					// 상극크리
					if( pUser->m_ModelDescriptor.m_EffectManager )
					{
						// Visual Effect - (살)
						if( g_ViewDamageValue )
						{
							int iDamageValue = m_AttackResult.sDamage[0];
							if(iDamageValue > 0)
									g_PostProcessWorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_cdamage, &pUser->m_ModelDescriptor.m_Position, &iDamageValue, FALSE, 5.0f );
						}
						else
						{
                            //_XLog( "PK크리");
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_Damage3, &pUser->m_ModelDescriptor.m_Position );
						}

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
						if(m_CharacterInfo.m_4Eff_MaxDam != _XEN_4EFF_NONE)
						{
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								m_CharacterInfo.m_4Eff_MaxDam+128, &pUser->m_ModelDescriptor.m_Position);
						}
#endif
					}

				#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
					if(g_ViewBattleDetail)
					{
						TCHAR messagestring[128];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _T("Casting 상극크리 damage[%d]"), m_AttackResult.sDamage[0]);	// add string

                        g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
                    }
				#endif
					
					VIEWPACKETLOG
						_XDWINPRINT("-- Attack Result : Casting To User [상극크리/damage:%d]", m_AttackResult.sDamage[0]);
				}
				break;
			}
			if(m_AttackResult.cBattleResult == en_battle_result_dead_in_match || m_AttackResult.cBattleResult == en_battle_result_dead_in_freematch)
			{
				// 내가 죽인 유저
//							m_CharacterLog.isKillClient = TRUE;
//							m_CharacterLog.killTimeClient = g_ServerTimeCode;
//							m_CharacterLog.killTargetTypeClient = 0;

				
				m_AttackResult.cBattleResult = 0;
			}

			m_AttackResult.display[0] = FALSE;
		}
	} // other-enemy
	
	m_AttackResult.display[0] = FALSE;
	m_bCheckFirstFrame = FALSE;
}