// XLocalUser_Training.h
//
// Author : Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_TrainingMed.h"
#include "XWindow_TrainingDummy.h"
#include "XWindow_TrainingSpell.h"
#include "XMessageWindow.h"
#include "XParticleIDDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_TrainingHelp.h"
#include "XWindowObjectDefine.h"
#include "XWindow_MatchMessage.h"
#include "SoundEffectList.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_PK.h"
#include "XWindow_PKTracingRequest.h"

// by mahwang
#include "XLifeEventManager.h"

void _XLocalUser::ProcessTraining(void)
{
	if(m_UserState != _XDEF_USERSTATE_TRAINING)
		return;

	switch(m_SelectedTrainingMode)
	{
	case _XTRAINING_MEDITATION :
		ProcessTrainingMeditation();
		break;
	case _XTRAINING_DUMMY :
		ProcessTrainingDummy();
		break;
	case _XTRAINING_SPELL :
		ProcessTrainingSpell();
		break;

	// by mahwang
	case _XTRAINING_FIREWOODCUT :
		ProcessTrainingFireWood();
		break;
	// by mahwang
	case _XTRAINING_WATERDRAW :
		ProcessTrainingWaterDraw();
		break;
	}
}


void _XLocalUser::ProcessTrainingMeditation(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
	if(!pMeditation_Window)
		return;

	switch(m_CurrentTrainingState)
	{
	case _XUSER_STATE_1 :
		{
			if(!m_bSendTrainingStartPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_START_REQ_C_S, m_SelectedTrainingMode, 1);
				m_bSendTrainingStartPacket = TRUE;

				_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
				if(pTrainingHelp_Window)
					pTrainingHelp_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			SetAnimationIndex(_XAN_EMOTION_SITDOWNSTART);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

			// 들고 있는 아이템을 삭제
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				if(g_WeaponItemProperty[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}	
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				if(g_WeaponItemProperty2[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}	 
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				if(g_WeaponItemProperty3[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}	

			m_CurrentTrainingState = _XUSER_STATE_4;
		}
		break;
	case _XUSER_STATE_4 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNREPEAT, FALSE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentTrainingState = _XUSER_STATE_5;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
		}
		break;
	case _XUSER_STATE_6 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentTrainingState = _XUSER_STATE_7;
				}
			}
			else
				m_CurrentTrainingState = _XUSER_STATE_7;
		}
		break;
	case _XUSER_STATE_7 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();
					SetAnimation(_XACTION_IDLE);
					m_CurrentTrainingState = _XUSER_STATE_LAST;
				}
			}
			else
				m_CurrentTrainingState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			ResetTrainingMode();
		}
		break;
	}
}

void _XLocalUser::ResetTrainingMode(void)
{
	switch(m_SelectedTrainingMode)
	{
	case _XTRAINING_MEDITATION :
		{
			// 들고 있던 무기 복구
			ReloadWeaponModel();
		}
		break;
	case _XTRAINING_DUMMY :
		{
			_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
			if(pDummy_Window)
			{
				pDummy_Window->m_bTrainingStart = FALSE;
				pDummy_Window->m_bWaitForServerRes = FALSE;

				if(pDummy_Window->m_pDummy)
				{
					pDummy_Window->m_pDummy->m_bImpact = FALSE;
					pDummy_Window->m_pDummy = NULL;
				}
				
				if(m_AttackTargetObjectType == _XPICKINGTARGET_DUMMY)
					ResetAttackTarget();
				if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					ResetViewTarget();
			}

			memset(&m_TrainingDummyInfo, 0, sizeof(_XDUMMY_INFO));
			SetAnimation(_XACTION_IDLE);

			m_SelectedSkillID = 0;
			m_SelectedSkillItem = NULL;
			if(g_pQuickSlot_Window)
			{
				g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(-1);
				g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(-1);
			}
		}
		break;
	case _XTRAINING_SPELL :
		{
			// 들고 있던 무기 복구
			ReloadWeaponModel();

			_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
			if(pSpell_Window)
			{
				pSpell_Window->m_bTrainingStart = FALSE;
				pSpell_Window->m_bWaitForServerRes = FALSE;
			}

			if(m_AttackTargetObjectType == _XPICKINGTARGET_SEAT)
				ResetAttackTarget();
			if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
				ResetViewTarget();

			m_TrainingSpellInfo.Reset();
			SetAnimation(_XACTION_IDLE);
		}
		break;

	// by mahwang
	case _XTRAINING_WATERDRAW :
	case _XTRAINING_FIREWOODCUT :
		{
			// 애니메이션이 초기화 되기전에 디폴트로 돌려 놓아야 한다.
			m_UserState = _XDEF_USERSTATE_NORMAL;
			m_SelectedTrainingMode = _XTRAINING_NONE;
			m_CurrentTrainingState = _XUSER_STATE_NONE;

			// 들고 있던 무기 복구
			ReloadWeaponModel();
			SetAnimation(_XACTION_IDLE);
		}
		break;
	}

	m_UserState = _XDEF_USERSTATE_NORMAL;
	m_SelectedTrainingMode = _XTRAINING_NONE;
	m_CurrentTrainingState = _XUSER_STATE_NONE;
	m_bSendTrainingStartPacket = FALSE;
	m_ServerKey = 0;
}


void _XLocalUser::ProcessTrainingDummy(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
	if(!pDummy_Window)
		return;
	
	switch(m_CurrentTrainingState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_TrainingDummyInfo.skillid > 0)
			{
				// Send start packet
				if(!m_bSendTrainingStartPacket)
				{
					if(_XSkillSelectCheckCallBack(0, m_TrainingDummyInfo.skillid))
					{
						g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_START_REQ_C_S, m_SelectedTrainingMode, m_TrainingDummyInfo.skillid);
						m_bSendTrainingStartPacket = TRUE;

						_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
						if(pTrainingHelp_Window)
							pTrainingHelp_Window->ShowWindow(FALSE);
						
						m_SelectedSkillID = m_TrainingDummyInfo.skillid;
						m_SelectedSkillItem = g_pLocalUser->GetSkillItem(m_TrainingDummyInfo.skillid);
					}
					else
					{
//						g_NetworkKernel.InsertChatString("더 이상 수련할 수 없는 무공입니다.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						m_TrainingDummyInfo.skillid = 0;
						_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
						if(pTrainingHelp_Window)
						{
							pTrainingHelp_Window->SetSelectedSkillButtonIndex(-1);
						}
					}
				}
			}
			else
			{
				m_CurrentTrainingState = _XUSER_STATE_2;
			}
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(!m_TrainingDummyInfo.bWarning)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2908), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL); //수련을 시작할 무공을 선택해주십시오.
				m_TrainingDummyInfo.bWarning = TRUE;
			}
			m_CurrentTrainingState = _XUSER_STATE_1;
		}
		break;
	case _XUSER_STATE_3 :
		{
			if(pDummy_Window->m_bTrainingStart)
			{
				if(pDummy_Window->m_bStopMousePoint)
				{
					// 마우스 포인터 멈췄음. 애니매이션 플레이
					short motionindex;
					if(m_CharacterInfo.gender == _XGENDER_MAN)
						motionindex = g_SkillProperty[m_TrainingDummyInfo.skillid]->motion_male;
					else
						motionindex = g_SkillProperty[m_TrainingDummyInfo.skillid]->motion_female;

					SetAnimationIndex(motionindex);
					m_CurrentTrainingState = _XUSER_STATE_4;
					m_TrainingDummyInfo.cTrainingGrade = pDummy_Window->m_TrainingGrade;
				}
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
			if(pDummy_Window->m_bTrainingStart)
			{
				if(GetMotionClass() == _XACTION_ATTACK)
				{
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
					{
						m_CurrentAttackSequence = (_XUSER_ATTACKSEQUENCE)((int)m_CurrentAttackSequence + 1);
						g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, m_CurrentAttackSequence-1);
					}
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
					{
						// 부가 효과 처리
						// 타격 이펙트 삽입
						if(pDummy_Window->m_bSuccess)
						{
							if(pDummy_Window->m_pDummy)
							{
								D3DXMATRIX offsetmat = pDummy_Window->m_pDummy->m_ModelDescriptor.m_Position;
								offsetmat._42 += ( 0.15f + ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f ) );
								
								PlayAttackSound();
								if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
								{
									if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pDummy_Window->m_pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat );
									else
										pDummy_Window->m_pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT1, &offsetmat );
								}
								else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
								{
									if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pDummy_Window->m_pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat );
									else
										pDummy_Window->m_pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT1, &offsetmat );
								}
								else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
								{
									if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pDummy_Window->m_pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat );
									else
										pDummy_Window->m_pDummy->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT1, &offsetmat );
								}
								
								if(!pDummy_Window->m_bComplete)
									pDummy_Window->m_pDummy->m_bImpact = TRUE;
							}
						}
						else
						{
							if(pDummy_Window->m_pDummy)
							{
								int rand = _XRandom() % 3;
								_XPlaySoundEffect(g_DefenceEffectSoundIndex[m_CharacterInfo.gender][rand], m_Position, true);

								pDummy_Window->m_pDummy->m_bImpact = FALSE;
							}
						}
					}
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
					{
						if(g_SkillProperty[m_TrainingDummyInfo.skillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
						{
							if(m_TrainingDummyInfo.cTrainingGrade == 1)
							{
								// 끝
								short motionindex = GetInterpolateMotionIndex(m_TrainingDummyInfo.skillid);
								if(motionindex > 0)
								{
									SetAnimationIndex(motionindex);
									m_CurrentTrainingState = _XUSER_STATE_5;
								}
								else
								{
									m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
									SetAnimation(_XACTION_IDLE);
									pDummy_Window->ResetMousePosition();
									
									m_CurrentTrainingState = _XUSER_STATE_3;
									
									if(pDummy_Window->m_pDummy)
										pDummy_Window->m_pDummy->m_bImpact = FALSE;
								}
							}
							else if(m_TrainingDummyInfo.cTrainingGrade == 2)
							{
								if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
								{
									// 2초식으로
									short motionindex = GetMotionIndex(m_TrainingDummyInfo.skillid);
									motionindex += _XANI_ORDER_REPEAT;
									SetAnimationIndex(motionindex);
								}
								else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
								{
									// 끝
									short motionindex = GetInterpolateMotionIndex(m_TrainingDummyInfo.skillid);
									if(motionindex > 0)
									{
										SetAnimationIndex(motionindex);
										m_CurrentTrainingState = _XUSER_STATE_5;
									}
									else
									{
										m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
										SetAnimation(_XACTION_IDLE);
										pDummy_Window->ResetMousePosition();
										
										m_CurrentTrainingState = _XUSER_STATE_3;
										
										if(pDummy_Window->m_pDummy)
											pDummy_Window->m_pDummy->m_bImpact = FALSE;
									}
								}
							}
							else if(m_TrainingDummyInfo.cTrainingGrade >= 3)
							{
								if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
								{
									// 2초식으로
									short motionindex = GetMotionIndex(m_TrainingDummyInfo.skillid);
									motionindex += _XANI_ORDER_REPEAT;
									SetAnimationIndex(motionindex);
								}
								else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
								{
									// 3초식으로
									short motionindex = GetMotionIndex(m_TrainingDummyInfo.skillid);
									int martialcount = g_SkillProperty[m_TrainingDummyInfo.skillid]->martialNumber;
									if(martialcount >= 3)
									{
										motionindex += _XANI_ORDER_FIRE;
										SetAnimationIndex(motionindex);
									}
									else if(martialcount >= 2)
									{
										// 끝
										short motionindex = GetInterpolateMotionIndex(m_TrainingDummyInfo.skillid);
										if(motionindex > 0)
										{
											SetAnimationIndex(motionindex);
											m_CurrentTrainingState = _XUSER_STATE_5;
										}
										else
										{
											m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
											SetAnimation(_XACTION_IDLE);
											pDummy_Window->ResetMousePosition();
											
											m_CurrentTrainingState = _XUSER_STATE_3;
											
											if(pDummy_Window->m_pDummy)
												pDummy_Window->m_pDummy->m_bImpact = FALSE;
										}
									}
								}
								else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_THIRD)
								{
									// 끝
									short motionindex = GetInterpolateMotionIndex(m_TrainingDummyInfo.skillid);
									if(motionindex > 0)
									{
										SetAnimationIndex(motionindex);
										m_CurrentTrainingState = _XUSER_STATE_5;
									}
									else
									{
										m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
										SetAnimation(_XACTION_IDLE);
										pDummy_Window->ResetMousePosition();
										
										m_CurrentTrainingState = _XUSER_STATE_3;
										
										if(pDummy_Window->m_pDummy)
											pDummy_Window->m_pDummy->m_bImpact = FALSE;
									}
								}
							}
						}
					}
				} // if(GetMotionClass() == _XACTION_ATTACK)
			}

			if(m_TrainingDummyInfo.bComplete)
			{
				// 완료
				if(GetMotionClass() == _XACTION_ATTACK)
				{
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
					{
						m_CurrentAttackSequence = (_XUSER_ATTACKSEQUENCE)((int)m_CurrentAttackSequence + 1);
					}
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
					{
						if(g_SkillProperty[m_TrainingDummyInfo.skillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
						{
							if(m_TrainingDummyInfo.cTrainingGrade == 1)
							{
								// 끝
								ResetTrainingMode();
							}
							else if(m_TrainingDummyInfo.cTrainingGrade == 2)
							{
								if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
								{
									// 2초식으로
									short motionindex = GetMotionIndex(m_TrainingDummyInfo.skillid);
									motionindex += _XANI_ORDER_REPEAT;
									SetAnimationIndex(motionindex);
								}
								else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
								{
									// 끝
									ResetTrainingMode();
								}
							}
							else if(m_TrainingDummyInfo.cTrainingGrade >= 3)
							{
								if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
								{
									// 2초식으로
									short motionindex = GetMotionIndex(m_TrainingDummyInfo.skillid);
									motionindex += _XANI_ORDER_REPEAT;
									SetAnimationIndex(motionindex);
								}
								else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
								{
									// 3초식으로
									short motionindex = GetMotionIndex(m_TrainingDummyInfo.skillid);
									int martialcount = g_SkillProperty[m_TrainingDummyInfo.skillid]->martialNumber;
									if(martialcount >= 3)
									{
										motionindex += _XANI_ORDER_FIRE;
										SetAnimationIndex(motionindex);
									}
									else if(martialcount >= 2)
									{
										// 끝
										ResetTrainingMode();
									}
								}
								else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_THIRD)
								{
									// 끝
									ResetTrainingMode();
								}
							}
						}
					} // last frame
				}
			} // complete
		}
		break;
	case _XUSER_STATE_5 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
				SetAnimation(_XACTION_IDLE);
				pDummy_Window->ResetMousePosition();
				
				m_CurrentTrainingState = _XUSER_STATE_3;
				
				if(pDummy_Window->m_pDummy)
					pDummy_Window->m_pDummy->m_bImpact = FALSE;
			}
		}
		break;
	}
}

void _XLocalUser::ProcessTrainingSpell(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
	if(!pSpell_Window)
		return;

	switch(m_CurrentTrainingState)
	{
	case _XUSER_STATE_1 :
		{
			if(m_TrainingSpellInfo.skillid > 0)
			{	
				if(!m_bSendTrainingStartPacket)
				{
					g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_START_REQ_C_S, m_SelectedTrainingMode, m_TrainingSpellInfo.skillid);
					m_bSendTrainingStartPacket = TRUE;

					_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
					if(pTrainingHelp_Window)
						pTrainingHelp_Window->ShowWindow(FALSE);
				}
			}
			else
			{
				m_CurrentTrainingState = _XUSER_STATE_2;
			}

			// 선택된 스킬 검사
/*			if(g_SkillProperty[m_SelectedSkillID]->skillCType == _XSI_CTYPE_SPELL && m_SelectedSkillID > 0)
			{
				if(g_SkillProperty[m_SelectedSkillID]->skillSType != _XSI_STYPE_03)
				{
					if(m_SelectedSkillItem->m_ucSkillState == 1)
					{
						m_TrainingSpellInfo.skillid = m_SelectedSkillID;

						if(!m_bSendTrainingStartPacket)
						{
							g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_START_REQ_C_S, m_SelectedTrainingMode, m_TrainingSpellInfo.skillid);
							m_bSendTrainingStartPacket = TRUE;
						}
					}
					else
					{
						if(!m_TrainingSpellInfo.bWarningSkillState)
						{
							g_NetworkKernel.InsertChatString("이미 수련을 완료한 무공입니다.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							m_TrainingSpellInfo.bWarningSkillState = TRUE;
						}
					}
				}
			}
			else
			{
				m_CurrentTrainingState = _XUSER_STATE_2;
			}*/
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(!m_TrainingSpellInfo.bWarning)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2908), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL); //"수련을 시작할 무공을 선택해주십시오."
				m_TrainingSpellInfo.bWarning = TRUE;
			}
			m_CurrentTrainingState = _XUSER_STATE_1;
		}
		break;
	case _XUSER_STATE_3 :
		{
			pSpell_Window->m_bTrainingStart = TRUE;
			pSpell_Window->m_TrainingStartTime = g_LocalSystemTime;

			if(pSpell_Window->m_bTrainingStart)
			{
				SetAnimationIndex(_XAN_EMOTION_SITDOWNSTART);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					if(g_WeaponItemProperty[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
					{
						m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
						m_CharacterInfo.modeldescript.item1 = 0;
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
						m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					if(g_WeaponItemProperty2[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
					{
						m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
						m_CharacterInfo.modeldescript.item1 = 0;
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
						m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					if(g_WeaponItemProperty3[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
					{
						m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
						m_CharacterInfo.modeldescript.item1 = 0;
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
						m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
					}
				}

				m_CurrentTrainingState = _XUSER_STATE_4;
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
			if(pSpell_Window->m_bTrainingStart)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
					{
						SetAnimationIndex(_XAN_EMOTION_SITDOWNREPEAT, FALSE);
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						m_CurrentTrainingState = _XUSER_STATE_5;
					}
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
		}
		break;
	case _XUSER_STATE_6 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentTrainingState = _XUSER_STATE_7;
				}
			}
			else
				m_CurrentTrainingState = _XUSER_STATE_7;
		}
		break;
	case _XUSER_STATE_7 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();					
					SetAnimation(_XACTION_IDLE);
					m_CurrentTrainingState = _XUSER_STATE_LAST;
				}
			}
			else
				m_CurrentTrainingState = _XUSER_STATE_LAST;
		}
		break;
	case _XUSER_STATE_LAST :
		{
			ResetTrainingMode();
		}
		break;
	}
}

void _XLocalUser::ProcessTrainingFireWood(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

//	_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
//	if(!pDummy_Window)
//		return;

	switch(m_CurrentTrainingState)
	{
	case _XUSER_STATE_1 :
		{
			_XAnimMeshAbstractClass* pAxe = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_AXE);
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, pAxe );
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;

			_XAnimMeshAbstractClass* pWoodBlock = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_FIREWOOD);
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM2, pWoodBlock );

			m_CurrentTrainingState = _XUSER_STATE_2;
		}
		break;
	case _XUSER_STATE_2 :
		{
			int nIndex = g_LifeEventManager.GetaniIndex(m_CharacterInfo.gender, LIFE_EVENT_ANI_FIREWOOD_READY);
			SetAnimationIndex(nIndex);

			m_CurrentTrainingState = _XUSER_STATE_3;
		}
		break;
	case _XUSER_STATE_3 :
		{
			if(GetMotionClass() == _XACTION_FIREWOODCUT)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimation(_XACTION_IDLE);
					m_CurrentTrainingState = _XUSER_STATE_4;
				}
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
			MouseState* mousestate = gpInput->GetMouseState();

			if(mousestate->bButton[0])
			{
				int nIndex = g_LifeEventManager.GetaniIndex(m_CharacterInfo.gender, LIFE_EVENT_ANI_FIREWOOD_SUCCEED);
				SetAnimationIndex(nIndex);
				m_CurrentTrainingState = _XUSER_STATE_5;
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
			if(GetMotionClass() == _XACTION_FIREWOODCUT)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					++ m_nTrainingCount;
					if (m_nTrainingCount >= 3)
					{
						// 들고 있던 무기 복구
						ReloadWeaponModel();

						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T("장작 다 팼다"), TRUE);	// add string
						SetAnimation(_XACTION_IDLE);
						ResetTrainingMode();
					}
					else
					{
						SetAnimation(_XACTION_IDLE);
						m_CurrentTrainingState = _XUSER_STATE_2;
					}
				}
			}
		}
		break;
	}
}

void _XLocalUser::ProcessTrainingWaterDraw(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

//	_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
//	if(!pDummy_Window)
//		return;

	switch(m_CurrentTrainingState)
	{
	case _XUSER_STATE_1 :
		{
			_XAnimMeshAbstractClass* pBucket = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_BUCKER);
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, pBucket );
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;

			m_CurrentTrainingState = _XUSER_STATE_2;
		}
		break;
	case _XUSER_STATE_2 :
		{
		}
		break;
	case _XUSER_STATE_3 :
		{
			int nIndex = g_LifeEventManager.GetaniIndex(m_CharacterInfo.gender, LIFE_EVENT_ANI_WATER_IN);
			SetAnimationIndex(nIndex);

			m_CurrentTrainingState = _XUSER_STATE_4;
		}
		break;
	case _XUSER_STATE_4 :
		{
			if(GetMotionClass() == _XACTION_WATERDRAW)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimation(_XACTION_IDLE);
					m_CurrentTrainingState = _XUSER_STATE_5;
				}
			}
		}
		break;
	case _XUSER_STATE_5 :
		{
		}
		break;
	case _XUSER_STATE_6 :
		{
			MouseState* mousestate = gpInput->GetMouseState();

			if(mousestate->bButton[0])
			{
				int nIndex = g_LifeEventManager.GetaniIndex(m_CharacterInfo.gender, LIFE_EVENT_ANI_WATER_OUT);
				SetAnimationIndex(nIndex);
				m_CurrentTrainingState = _XUSER_STATE_7;
			}
		}
		break;
	case _XUSER_STATE_7 :
		{
			if(GetMotionClass() == _XACTION_WATERDRAW)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					++ m_nTrainingCount;
					if (m_nTrainingCount >= 3)
					{
						// 들고 있던 무기 복구
						ReloadWeaponModel();

						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T("물 다 펐다"), TRUE);	// add string
						SetAnimation(_XACTION_IDLE);
						ResetTrainingMode();
					}
					else
					{
						SetAnimation(_XACTION_IDLE);
						m_CurrentTrainingState = _XUSER_STATE_2;
					}
				}
			}
		}
		break;
	}
}

void _XLocalUser::ResetMatchState(void)
{
	m_UserState = _XDEF_USERSTATE_NORMAL;
	m_MatchTargetID = 0;
	m_MatchStateChangeTime = 0;
	m_myMatchTeamID = -1;
	m_MatchResult = -1;
	m_MatchFieldIndex = 0;

	m_myMatchMemberInfo.Reset();
	m_myMatchTableInfo.Reset();
	
	ResetAttackTarget();
	ResetViewTarget();
	
	_XWindow_MatchMessage* pMatchMessage_Window = (_XWindow_MatchMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHMESSAGE);
	if(pMatchMessage_Window)
	{
		pMatchMessage_Window->ResetData();
	}
}

void _XLocalUser::ProcessMatch()
{
	// animation / effect / sound 처리

	if(m_UserState < _XDEF_USERSTATE_MATCH_MATCHING && m_UserState > _XDEF_USERSTATE_MATCH_WAITEXIT)
		return;

	switch(m_UserState)
	{
	case _XDEF_USERSTATE_MATCH_MATCHING :
		{
			if(g_LocalSystemTime - m_MatchStateChangeTime > 5000)
			{
				// 5초 이상
				m_UserState = _XDEF_USERSTATE_MATCH_BOW;
				DeleteWeaponModel();
				_XPlayInterfaceSound(g_PVPBattleBGMStartIndex);
			}
		}
		break;
	case _XDEF_USERSTATE_MATCH_BOW :
		{
			_XUser* pUser = g_Userlist.FindUser(m_MatchTargetID);
			if(pUser)
			{
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
				UpdatePosition();

				pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, m_Position);
				pUser->UpdatePosition();
			}

			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_BOWPOLITELY)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					ReloadWeaponModel();
					m_UserState = _XDEF_USERSTATE_MATCH_POSITIONING;
					m_MatchStateChangeTime = g_LocalSystemTime;
				}
			}
			else
			{
				SetAnimationIndex(_XAN_EMOTION_BOWPOLITELY);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		break;
	case _XDEF_USERSTATE_MATCH_POSITIONING :
		{
		}
		break;
	case _XDEF_USERSTATE_MATCH_COUNTING :
		{
		}
		break;
	case _XDEF_USERSTATE_MATCH_FIGHT :
		{
		}
		break;
	case _XDEF_USERSTATE_MATCH_END :
		{
			if(g_LocalSystemTime - m_MatchStateChangeTime > 3000)
			{
				m_UserState = _XDEF_USERSTATE_MATCH_WAITEXIT;
				m_MatchStateChangeTime = g_LocalSystemTime;
			}
		}
		break;
	case _XDEF_USERSTATE_MATCH_RESULT :
		{
		}
		break;
	case _XDEF_USERSTATE_MATCH_WAITEXIT :
		{
		}
		break;
	}
}

//#ifdef _XDEF_MATCH_EVENT_071025_KUKURI

BOOL _XLocalUser::SetPKReadyMode()
{
	// GetTargetPositionBattleMode()에서만 호출되어야 함

	BOOL bPressShiftKey = FALSE;
	BOOL bCompulsionMode = FALSE; //Author : 양희왕 추가
	BOOL bPKPenaltyMode = FALSE;

	if(g_LastPickedObjectType == _XPICKINGTARGET_PC)
	{
		_XUser* pUser = (_XUser*)g_LastPickedObject;
		if(pUser)
		{
			bCompulsionMode = CheckPKTarget(pUser);	// 적대 관계인가?
			bPKPenaltyMode = CheckPKPenalty(pUser);	// PK penalty를 받고 있는 상태 인가?
		}

		if(bCompulsionMode && !bPKPenaltyMode)	// 적대관계이며, penalty 없음
		{
			// 비무대회 중이면, pk mode로.
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(GetEnterMatchEvent())
			{
				if(m_UserState != _XDEF_USERSTATE_PK_FIGHT && m_UserState != _XDEF_USERSTATE_PK_END)
				{
					if(pUser)
					{
						pUser->m_bEnemy = TRUE;
					}

					m_UserState = _XDEF_USERSTATE_PK_READY;

					_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
					if(pMonsterStatus_Window)
					{
						pMonsterStatus_Window->SetPKMode();
					}

					return TRUE;
				}
			}
#endif

#ifdef _XDEF_CASTLEBATTLE
			// 장원전 중이면 or PK 영역이면 pk mode로.
			if(GetEnterCastleBattle())
			{
				if(m_UserState != _XDEF_USERSTATE_PK_FIGHT && m_UserState != _XDEF_USERSTATE_PK_END)
				{
					if(pUser)
					{
						pUser->m_bEnemy = TRUE;
					}

					m_UserState = _XDEF_USERSTATE_PK_READY;

					_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
					if(pMonsterStatus_Window)
					{
						pMonsterStatus_Window->SetPKMode();
					}

					return TRUE;
				}
			}
#endif

			// pk 서버 일때,
			if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo)
			{
				bPressShiftKey = gpInput->CheckKeyPress(DIK_LSHIFT);

				// 안전 지역 체크
				int my_index = m_StateList.FindState(en_from_extraskill, 0, 8042, 3);
				int other_index = pUser->m_StateList.FindState(en_from_extraskill, 0, 8042, 3);
				if(my_index >= 0 || other_index >= 0)
				{
					// message 안내
					TCHAR messagestring[256];
					if(my_index >= 0)
					{
						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3035), g_pLocalUser->m_CharacterInfo.charactername);
					}
					if(other_index >= 0)
					{
                        if( pUser->m_bFaceMaskMode ) //Author : 양희왕 //breif : 복면인은 메시지 안나오게
                            _snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3830)); //상대방은 안전지역에 위치하고 있습니다.안전지역 규칙을 준수하시기 바랍니다.
                        else
                            _snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3035), pUser->m_CharacterInfo.charactername);
					}
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
				}
				else
				{
					if(g_pLocalUser->m_CharacterInfo.groupindex != _XGROUP_NANGIN && pUser->m_CharacterInfo.groupindex != _XGROUP_NANGIN)
					{
						if(g_pLocalUser->GetGroupType() != pUser->GetGroupType())
						{
							// 적대 문파 일 경우, shift key 안눌러도 됨
							bPressShiftKey = bCompulsionMode;
						}
					}
				}

				if(bPressShiftKey)
				{
					if(m_UserState != _XDEF_USERSTATE_PK_FIGHT && m_UserState != _XDEF_USERSTATE_PK_END)
					{
						if(pUser)
						{
							pUser->m_bEnemy = TRUE;
						}

						m_UserState = _XDEF_USERSTATE_PK_READY;

						_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
						if(pMonsterStatus_Window)
						{
							pMonsterStatus_Window->SetPKMode();
						}

						return TRUE;
					}
				}//if(bPressShiftKey)
			}//if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo)
		}//if(bCompulsionMode && !bPKPenaltyMode)	// 적대관계이며, penalty 없음
	}
	return FALSE;
}
/*
#else

BOOL _XLocalUser::SetPKReadyMode(void)
{
#ifdef _XTS_PK
	// GetTargetPositionBattleMode()에서만 호출되어야 함
	
	BOOL bsetpkreadymode = TRUE;
	
	if(g_LastPickedObjectType == _XPICKINGTARGET_PC)
	{
		_XUser* pUser = (_XUser*)g_LastPickedObject;
		if(pUser)
		{
			bsetpkreadymode = CheckPKTarget(pUser);

/*			if(pUser->m_bInPartyMember)
				bsetpkreadymode = FALSE;
			else if(pUser->m_CharacterInfo.groupindex == m_CharacterInfo.groupindex)
			{
				if(m_CharacterInfo.groupindex != _XGROUP_NANGIN)
					bsetpkreadymode = FALSE;
			}
		}
		else
		{
			bsetpkreadymode = FALSE;
		}
		
		if(bsetpkreadymode)
		{
			BOOL bPressShiftKey = FALSE;
			BOOL bCompulsionMode = FALSE; //Author : 양희왕 추가
	#ifdef _XTS_SAFETYZONE
			//bPressShiftKey = gpInput->CheckKeyPress(DIK_LSHIFT);

			if(this->m_CharacterInfo.groupindex != _XGROUP_NANGIN)
			{
				if(pUser->m_CharacterInfo.groupindex != _XGROUP_NANGIN)
				{
					if(this->GetGroupType() != pUser->GetGroupType()) 
					{
						//bPressShiftKey = TRUE;
						bCompulsionMode = TRUE; //적대 관계 //Author : 양희왕
					}
		#ifdef _XDEF_CASTLEBATTLE
					// 장원전 중이고, 적대관계라면
					if(m_bCastleBattle)
					{
						int attacktype = 0;
						if(pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0)
						{
							if(IsEnemyInCB(1, pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex, attacktype))
							{
								// ally이고 적대관계이면
								//bPressShiftKey = TRUE;
								bCompulsionMode = TRUE; //적대 관계 //Author : 양희왕
							}
						}
						else
						{
							if(IsEnemyInCB(0, pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex, attacktype))
							{
								// org이고 적대관계이면
								//bPressShiftKey = TRUE;	// pk 가능
								bCompulsionMode = TRUE; //적대 관계 //Author : 양희왕
							}
						}
					}
		#endif	//_XDEF_CASTLEBATTLE
		#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
					if( !g_pLocalUser->m_listPKTracing.empty() )
					{
						_XPKTRACING_INFO* pktracing = NULL;
						list <_XPKTRACING_INFO*>::iterator iter_pktracing;
						for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
						{
							pktracing = *iter_pktracing;
							if( pktracing->ucPKType == 1 )//레벨차 페널티 관련 처리
							{
								if( strcmp(pktracing->szCharacName,pUser->m_CharacterInfo.charactername ) == 0)
								{
									// 시간 검사
									int elapsedtime = g_ServerTimeCode- pktracing->nStartTrackingTime;
									if( elapsedtime > 3600 )//한시간이 지나면 페널티가 풀린다.
									{
										_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
										if(pPKTracingRequest_Window)
										{
											strcpy( pPKTracingRequest_Window->m_cDeleteTargetName, pktracing->szCharacName );
											g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_lvpk_del_req_cl_gs );	
											//클릭시 시간을 검사해서 1시간이 자났으면 패널티 리스트에서 지우라고 한다.
										}
										//클라이언트에서 알아서 삭제
										SAFE_DELETE(pktracing);
										g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
										//bPressShiftKey = TRUE;	// pk 가능
										bCompulsionMode = TRUE; //적대 관계 //Author : 양희왕
										return TRUE;
									}
									else
									{										
										g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3376), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
//										ResetPKMode();
//										bPressShiftKey = FALSE;	// pk 가능
										++iter_pktracing;
										return FALSE;
									}
								}
								else
								++iter_pktracing;
							}
							else
								++iter_pktracing;
						}
					}
		#endif//_XDEF_PK_ADDTIONALFUNCTION01_20070321
				}
			}
	#else//_XTS_SAFETYZONE
			bPressShiftKey = gpInput->CheckKeyPress(DIK_LSHIFT);
	#endif//_XTS_SAFETYZONE


	#ifdef _XTS_PKZONE_SERVERSETTING
			bPressShiftKey = gpInput->CheckKeyPress(DIK_LSHIFT);
			int areanumber = 0;

			if( (bPressShiftKey && g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo) ||
				(bCompulsionMode && g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo) )// || //Author : 양희왕 , 변경 //PK 영역 추가 //Date : 07/11/09
				//(bCompulsionMode && g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_PUBLICPVPAREA, areanumber)) )
	#else//_XTS_PKZONE_SERVERSETTING
			if(bPressShiftKey && g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo)
	#endif//_XTS_PKZONE_SERVERSETTING
			{
				if(m_UserState != _XDEF_USERSTATE_PK_FIGHT && m_UserState != _XDEF_USERSTATE_PK_END)
				{
	#ifdef _XTS_SAFETYZONE
					// message
					TCHAR messagestring[256];
					int index = m_StateList.FindState(en_from_extraskill, 0, 8042, 3);
					if(index >= 0)
					{
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3035), g_pLocalUser->m_CharacterInfo.charactername);
					}
					index = pUser->m_StateList.FindState(en_from_extraskill, 0, 8042, 3);
					if(index >= 0)
					{
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3035), pUser->m_CharacterInfo.charactername);
					}
					if(index >= 0)
					{
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
					}
	#endif//_XTS_SAFETYZONE

					if( (index >= 0 && bPressShiftKey) || (index < 0)) //Author : 양희왕 //breif : if 문만 추가
					{
						// L shift + mouse click + pk map
						if(pUser)
						{
							pUser->m_bEnemy = TRUE;
						}

						m_UserState = _XDEF_USERSTATE_PK_READY;

						_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
						if(pMonsterStatus_Window)
						{
							pMonsterStatus_Window->SetPKMode();
						}

						return TRUE;
					}
				}
			}
		}
	}
#endif//_XTS_PK

	return FALSE;
}
#endif*/

void _XLocalUser::ProcessPKMode(void)
{
	if(m_UserState != _XDEF_USERSTATE_PK_FIGHT)
		return;

	if(g_LocalSystemTime - m_PKModeStartTime > m_PKModeLeftTime && m_PKModeStartTime > 0)
	{
		// request mode change
		if(m_PrevBattleMode == _XBATTLE_BATTLE)
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_to_battle_mode);
		else
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_to_peace_mode);

		m_UserState = _XDEF_USERSTATE_PK_END;
	}
}

// fight 상태 일때
void _XLocalUser::ResetPKMode(void)
{
	if(m_BattleMode == _XBATTLE_PK)
	{
		m_UserState = _XDEF_USERSTATE_NORMAL;

		m_PKModeStartTime = 0;
		m_PKModeLeftTime = 0;

		_XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
		if(pPK_Window)
		{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(!g_pLocalUser->GetEnterMatchEvent())	// 비무 대회 중에는 대전 이미지 없애지 않음
#endif
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2737), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);	// "대전 상태가 해제 되었습니다."
				pPK_Window->ShowWindow(FALSE);
			}
		}

		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
		if(pMonsterStatus_Window)
		{
			pMonsterStatus_Window->ResetPKMode();
		}
	}
}

// ready 상태 일때
void _XLocalUser::ResetPKReadyMode(void)
{
	m_UserState = _XDEF_USERSTATE_NORMAL;
	
	_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
	if(pMonsterStatus_Window)
	{
		pMonsterStatus_Window->ResetPKMode();
	}
}

BOOL _XLocalUser::CheckPKTarget(_XUser* _user)	// 적대 관계인지 아닌지 체크
{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(GetEnterMatchEvent())
	{
		// 비무 대회 참가 중이고,
		if(_user->GetEnterMatchEvent())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
#endif

#ifdef _XDEF_CASTLEBATTLE
	if(m_bCastleBattle)
	{
		// 장원전에 참가중일 때
		int attacktype = 0;
		if(_user->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0)
		{
			if(IsEnemyInCB(1, _user->m_CharacterInfo.m_AlienceInfo.m_iIndex, attacktype))
			{
				// ally이고 적대관계이면
				return TRUE;	// pk 가능
			}
		}
		else
		{
			if(IsEnemyInCB(0, _user->m_CharacterInfo.m_OrganizationInfo.m_iIndex, attacktype))
			{
				// org이고 적대관계이면
				return TRUE;	// pk 가능
			}
		}
	}
#endif	//_XDEF_CASTLEBATTLE

	//같은 영웅단 동맹이면 PK가 불가능하다.
	if( m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
	{
		if( _user->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
		{
			if( m_CharacterInfo.m_AlienceInfo.m_iIndex == _user->m_CharacterInfo.m_AlienceInfo.m_iIndex )
			{
				return FALSE;
			}

		}

	}

	// party
	if(_user->m_bInPartyMember)
		return FALSE;

#ifndef _XDEF_MATCH_EVENT_071025_KUKURI	
	#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
		if( !g_pLocalUser->m_listPKTracing.empty() )
		{
			_XPKTRACING_INFO* pktracing = NULL;
			list <_XPKTRACING_INFO*>::iterator iter_pktracing;
			for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
			{
				pktracing = *iter_pktracing;
				if( pktracing->ucPKType == 1 )//레벨차 페널티 관련 처리
				{
					if( strcmp(pktracing->szCharacName,_user->m_CharacterInfo.charactername ) == 0)
					{
						// 시간 검사
						return FALSE;
					}
					else
					{
						++iter_pktracing;
					}
				}
				else
				{
					++iter_pktracing;
				}
			}
		}
	#endif
#endif
	
	// 같은 문파 불가능 - 낭인은 가능
	if(m_CharacterInfo.groupindex == _user->m_CharacterInfo.groupindex && m_CharacterInfo.groupindex != _XGROUP_NANGIN)
	{
		if(m_bFaceMaskMode)
		{
			// 내가 복면을 쓰고 있을 때 PK 가능
			return TRUE;
		}
		else if(_user->m_bFaceMaskMode)
		{
			// 상대방이 복면을 쓰고 있을 때 PK 가능
			return TRUE;
		}
		else if(g_pLocalUser->m_bPEMode)
		{
			// 내가 문파 공적일 때 Pk 가능
			return TRUE;
		}
		else if(_user->m_bPEMode)
		{
			// 상대방이 문파 공적일 때 PK 가능
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL _XLocalUser::CheckPKPenalty(_XUser* _user)
{
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 1 )//레벨차 페널티 관련 처리
			{
				if( strcmp(pktracing->szCharacName,_user->m_CharacterInfo.charactername ) == 0)
				{
					// 시간 검사
					int elapsedtime = g_ServerTimeCode- pktracing->nStartTrackingTime;
					if( elapsedtime > 3600 )//한시간이 지나면 페널티가 풀린다.
					{
						/* 이 부분 고민해볼 것.*/

						_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
						if(pPKTracingRequest_Window)
						{
							strcpy( pPKTracingRequest_Window->m_cDeleteTargetName, pktracing->szCharacName );
							g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_lvpk_del_req_cl_gs );	
							//클릭시 시간을 검사해서 1시간이 자났으면 패널티 리스트에서 지우라고 한다.
						}
						//클라이언트에서 알아서 삭제
						SAFE_DELETE(pktracing);
						g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
						return FALSE;
					}
					else
					{										
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3376), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
						++iter_pktracing;
						return TRUE;
					}
				}
				else
				++iter_pktracing;
			}
			else
				++iter_pktracing;
		}
	}
#endif//_XDEF_PK_ADDTIONALFUNCTION01_20070321

	return FALSE;
}

DWORD _XLocalUser::GetPEModeLeftTime(void)
{
	DWORD lefttime = 0;

	if(m_PEModeLeftTime >= (g_LocalSystemTime - m_PEModeStartTime))
	{
		lefttime = m_PEModeLeftTime - (g_LocalSystemTime - m_PEModeStartTime);
	}

	return lefttime;
}

#ifdef _XDEF_CASHITEM_SHOUT
void _XLocalUser::ResetShout(void)
{
	m_ShoutMode = _XSHOUT_MODE_NONE;
	m_ShoutStartTime = 0;
	m_ShoutLeftTime = 0;

	_XWindow_ChattingDefault* pChattingWindowDefault = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
	if(pChattingWindowDefault)
	{
		// 외치기 버튼 disable
		pChattingWindowDefault->m_btnChatFunctionMenuButton[1]->EnableWindow(FALSE);
		pChattingWindowDefault->SetChatMode(_XCHATMODE_NORMAL);
	}

	if(g_pState_Window)
	{
		g_pState_Window->DeletePlayerStateList(_XST_SHOUT, 0);
	}
}

void _XLocalUser::ProcessShout(void)
{
	if(m_ShoutMode == _XSHOUT_MODE_NONE)
		return;
	
	if(g_LocalSystemTime - m_ShoutStartTime > m_ShoutLeftTime && m_ShoutStartTime > 0)
	{
		// 그냥 끝낸다.
		ResetShout();
	}
}
#endif

void _XLocalUser::SetExpItemEndTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second)
{
	m_etYear	= year;
	m_etMonth	= month;
	m_etDay		= day;
	m_etHour	= hour;
	m_etMinute	= minute;
	m_etSecond	= second;
}

void _XLocalUser::ResetExpItemEndTime(void)
{
	m_etYear	= 0;
	m_etMonth	= 0;
	m_etDay		= 0;
	m_etHour	= 0;
	m_etMinute	= 0;
	m_etSecond	= 0;
}


DWORD _XLocalUser::GetShoutLeftTime(void)
{
	DWORD lefttime = 0;
	
#ifdef _XDEF_CASHITEM_SHOUT
	if(m_ShoutLeftTime >= (g_LocalSystemTime - m_ShoutStartTime))
	{
		lefttime = m_ShoutLeftTime - (g_LocalSystemTime - m_ShoutStartTime);
	}
#endif
	
	return lefttime;
}

BOOL _XLocalUser::IsEnemyInCB(int ortype, int orindex, int& attacktype)
{
	if(orindex <= 0)
		return FALSE;

	if(ortype == m_nDefenderType)
	{
		if(orindex == m_nDefenderIndex)
		{
			attacktype = 2;		// defender

			if(ortype == 1 && m_CharacterInfo.m_AlienceInfo.m_iIndex == orindex)
			{
				// 나와 같은 회 소속
				return FALSE;
			}
			if(ortype == 0 && m_CharacterInfo.m_OrganizationInfo.m_iIndex == orindex)
			{
				// 나와 같은 단 소속
				return FALSE;
			}

			return TRUE;
		}
	}


	for(int atkcnt = 0 ; atkcnt < max_count_of_challenge_list ; ++atkcnt)
	{
		if(ortype == m_nAttackerType[atkcnt])
		{
			if(orindex == m_nAttackerIndex[atkcnt])
			{
				attacktype = 1;		// attacker

				if(ortype == 1 && m_CharacterInfo.m_AlienceInfo.m_iIndex == orindex)
				{
					// 나와 같은 회 소속
					return FALSE;
				}
				if(ortype == 0 && m_CharacterInfo.m_OrganizationInfo.m_iIndex == orindex)
				{
					// 나와 같은 단 소속
					return FALSE;
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL _XLocalUser::GetInBossRoom()
{
	if( g_CurrentZoneInfoIndex == 50 ||
		g_CurrentZoneInfoIndex == 51 ||
		g_CurrentZoneInfoIndex == 52 ||
		g_CurrentZoneInfoIndex == 53 ||
		g_CurrentZoneInfoIndex == 54 ||
		g_CurrentZoneInfoIndex == 55 ||
		g_CurrentZoneInfoIndex == 56 ||
		g_CurrentZoneInfoIndex == 57 ||
		g_CurrentZoneInfoIndex == 58 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}