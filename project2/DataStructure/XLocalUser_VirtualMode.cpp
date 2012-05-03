// XLocalUser_Virtual.h
//
// Author : Hangil, Yang
// 
// 클라이언트만 처리하는 가상의 공간에서의 프로세스를 처리하는 함수들
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
#include "Xwindow_TrainingComplete.h"



void _XLocalUser::Process_VirtualMode(void)
{
	if(m_PrevProcessTime == -1.0f)
	{
		m_PrevProcessTime = g_fLocalSystemTime;
	}

	FLOAT progresstime = (g_fLocalSystemTime - m_PrevProcessTime) / 1000.0f;
	if(progresstime <= 0.0f)
		return;

	m_PrevProcessTime = g_fLocalSystemTime;

	///////////////////////////////////////////////////////////////////////
	// Apply gravity
	_XOBBData*	pCollideOBB = NULL;
	FLOAT terrainheight = g_LodTerrain.GetTerrainAndOBBHeight( m_Position, pCollideOBB );
	
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
			g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, m_Position.z) );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain, FALSE );

		}
		else if( m_Position.y > terrainheight )
		{
			if( m_UseSpeedSkill )
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
			g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, m_Position.z) );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain, FALSE );
		}
	}
	else
	{
		m_Velocity.y = 0.0f;
	}
	
	///////////////////////////////////////////////////////////////////////
	// Animation 전처리
	int eventlevel = 0; // Effect의 종류를 결정함.
		
	switch(m_CurrentSelectedSkillType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{				
			if( m_CurrentBattleState == _XUSER_STATE_8 )
			{
				if( m_ChargeIterateValue >= 0 && m_ChargeIterateValue < 2 ) eventlevel = 0;
				else if( m_ChargeIterateValue >= 2 && m_ChargeIterateValue < 4 ) eventlevel = 1;
				else if( m_ChargeIterateValue >= 4 && m_ChargeIterateValue <= 11 ) eventlevel = 2;
				if( g_ZeroLevelMode )		// hotblood zerolevel
					eventlevel = 0;
			}
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			if(m_CurrentBattleState == _XUSER_STATE_6)
				eventlevel = 0;
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{				
			if(  m_CurrentBattleState == _XUSER_STATE_4 )
				eventlevel = 0;
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			if(  m_CurrentBattleState == _XUSER_STATE_4 )
				eventlevel = 0;
		}
		break;
	}
	
	// animation 전처리
	bool soundplay = true;

	if( m_SelectedSkillID )
	{
		// 스킬의 적용 반경이 정해진 경우 이펙트의 적용 최대 반경을 제한한다.
		if( g_SkillProperty[m_SelectedSkillID]->radius > 0.0f )
		{
			m_ModelDescriptor.Advance( soundplay, eventlevel, g_SkillProperty[m_SelectedSkillID]->radius * 2.0f );
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
	
	// by mahwang
//	if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
//		g_LifeEventManager.Process(g_fElapsedFrameTime / 1000.f);

	GetTargetPosition();

	// 다음에 이동할 위치 처리 - 상태에 따라서 움직이지 않음
	BOOL	bCanNotMove = CanNotMove();	
	if(!bCanNotMove)
		ProcessNextMovePosition();
	else
		m_TargetPosition = m_Position;

	// 전투 처리
	ProcessBattle_VirtualMode();
	
	// Delay tiem check
	CheckDelayTime();
	
	// 다음 스킬 적용 
	ApplyNextSkill();
	
	// 수련 처리
	ProcessTraining();
	
	// Animaiton 처리
	ProcessAnimation_VirtualMode();

//	// 영약 중독 감소 처리
//	if( g_LocalSystemTime - m_IntoxicationTimer > 60000 )	// 20초마다 감소
//	{
//		if( m_ElixirInfo._sIntoxication -1 >= 0 )
//		{
//			m_IntoxicationTimer = g_LocalSystemTime;
//			int tempinto = m_ElixirInfo._sIntoxication;
//			m_ElixirInfo._sIntoxication--;
//			if( tempinto == 100 && m_ElixirInfo._sIntoxication < 100 )
//			{
//				g_pState_Window->DeleteIntoxication(_XST_INTOXICATION, 2003);
//			}			
//		}
//		else 
//			m_ElixirInfo._sIntoxication = 0;
//	}
//	
//	
//	//  수련 완료 표시
//	_XWindow_TrainingComplete* pComplete_Window = (_XWindow_TrainingComplete*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGCOMPLETE);
//	if(pComplete_Window)
//	{
//		if((g_LocalSystemTime - pComplete_Window->m_CompleteTime > 3000) && pComplete_Window->m_CompleteTime > 0)
//		{
//			switch(m_SelectedTrainingMode)
//			{
//			case _XTRAINING_MEDITATION :
//				{
//				}
//				break;
//			case _XTRAINING_DUMMY :
//				{
//				}
//				break;
//			}
//			pComplete_Window->ShowWindow(FALSE);
//		}
//	}	

}

void _XLocalUser::ProcessAnimation_VirtualMode(void)
{
	if(GetMotionClass() == _XACTION_IDLE)
	{
		// Emotion
		if( m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_CRINGESTART )
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_EMOTION_CRINGEREPEAT,FALSE);// Repeate
				//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if( m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
				 m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				ReloadWeaponModel();
				SetAnimation( _XACTION_IDLE );
			}			
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_KNEE_SWORD_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_KNEE_SWORD_REPEAT, TRUE);
				//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_CRINGEREPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_KNEE_SWORD_REPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_REPEAT )
		{
			// 처리 하면 안됨 - 수련 용
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetLockEndFrame(FALSE);
				SetAnimationIndex(_XAN_MATCH_PR_REPEAT, TRUE);
				//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_END)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
				ReloadWeaponModel();
				SetAnimation( _XACTION_IDLE );
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_WINNER_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_MATCH_WINNER_REPEAT);
				//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_LOSER_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_MATCH_LOSER_REPEAT);
				//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else 
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
#ifdef _XDEF_NEWANIMATIONLIST
				/*
				if( m_SelectedSkillID )
				{
					if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
						g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
					{
						int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
											  g_SkillProperty[m_SelectedSkillID]->motion_male : 
											  g_SkillProperty[m_SelectedSkillID]->motion_female;
						
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{
						}
						else
						{
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
						}
						
						if( m_ModelDescriptor.m_CurMotion != baseanimindex+2 ) // 경공 마지막 동작 
						{
							SetAnimation( _XACTION_IDLE );
						}
					}
					{
						SetAnimation( _XACTION_IDLE );
					}
				}
				else
				*/
#endif
				{
					SetAnimation( _XACTION_IDLE );
				}
			}
		}		

		if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
		{
			if(m_SpecialActionTimer == 0)
				m_SpecialActionTimer = g_LocalSystemTime;

			if((g_LocalSystemTime - m_SpecialActionTimer) > _XDEF_CHANGEIDLEMOTIONTIME)
			{
				m_SpecialActionTimer = g_LocalSystemTime;

				switch(m_ModelDescriptor.GetCurMotion())
				{
				case _XAN_IDLE_NONE :
					{
						SetAnimationIndex(_XAN_IDLE_NONE_ACTION, TRUE);
					}
					break;				
				case _XAN_IDLE_SHORTWEAPON :
					{
						SetAnimationIndex(_XAN_IDLE_SHORTWEAPON_ACTION, TRUE);
					}
					break;				
				case _XAN_IDLE_LONGWEAPON :
					{
						SetAnimationIndex(_XAN_IDLE_LONGWEAPON_ACTION, TRUE);
					}
					break;
				case _XAN_IDLE_SWORD_PEACE :
					{
						SetAnimationIndex(_XAN_IDLE_SWORD_ACTION, TRUE);
					}
					break;
				}
			}

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.GetCurMotion())
				{
				case _XAN_IDLE_NONE_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				case _XAN_IDLE_SHORTWEAPON_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;				
				case _XAN_IDLE_LONGWEAPON_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				case _XAN_IDLE_SWORD_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				}
			}
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie_VirtualMode();
	}
	else if(GetMotionClass() == _XACTION_MOVE)
	{
		if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
		{
#ifdef _XDEF_NEWANIMATIONLIST

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{

				int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
									  g_SkillProperty[m_SelectedSkillID]->motion_male : 
									  g_SkillProperty[m_SelectedSkillID]->motion_female;


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
				}
#endif
				if( m_ModelDescriptor.m_CurMotion == baseanimindex )
				{
					SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate
				}
				//else if( m_ModelDescriptor.m_CurMotion == baseanimindex+1 )
				//			{
				//
				//			}			
				else if( m_ModelDescriptor.m_CurMotion == baseanimindex+2 )
				{
					SetAnimation(_XACTION_IDLE);
					m_UseSpeedSkill = FALSE;
					//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
					m_TargetPosition = m_Position;
					
					if( m_SelectedSkillID )
					{
						if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
							_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
							g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
						{
							m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
							m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);
						}
					}
				}
			
				//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}

#else //_XDEF_NEWANIMATIONLIST
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
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate
						//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
				case _XAN_FASTMOVE_SWORD_END_PEACE :
				case _XAN_FASTMOVE_SWORD_END_BATTLE :
					{
						SetAnimation(_XACTION_IDLE);
						m_UseSpeedSkill = FALSE;
						//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						m_TargetPosition = m_Position;
						
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
					break;
				}
			}
#endif //_XDEF_NEWANIMATIONLIST
		}
		else if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
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
					
				/*case _XAN_MOVE_NONE_REPEAT		 :
				case _XAN_MOVE_SHORTWEAPON_REPEAT:
				case _XAN_MOVE_LONGWEAPON_REPEAT :
				case _XAN_MOVE_SOFTWEAPON_REPEAT :
					{						
					}
					break;*/
					
				case _XAN_MOVE_NONE_END			 :
				case _XAN_MOVE_SHORTWEAPON_END	 :
				case _XAN_MOVE_LONGWEAPON_END	 :
				case _XAN_MOVE_SOFTWEAPON_END	 :
					{
						SetAnimation(_XACTION_IDLE);						
					}
					break;
				}
			}
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie_VirtualMode();
	}
	else if(GetMotionClass() == _XACTION_DYING)
	{
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
				SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
				break;
			case _XAN_DYINGREADY_SWORD_BATTLE :
				SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
				break;
			}
			
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
				m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_PLAYERID_DYINGWAIT, &m_ModelDescriptor.m_Position );
			}
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie_VirtualMode();
	}
	else if(GetMotionClass() == _XACTION_DIE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}

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
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}
			SetAnimation(_XACTION_IDLE);
		}
		
		// display 되지 않은 전투 결과 처리
		ProcessMobDie_VirtualMode();

	}
	else if(GetMotionClass() == _XACTION_LPS)
	{

		if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE )
		{
			// 나중에 LPS동작 처리가 들어가면 따로 분리한다.
			if(m_ViewTargetType == _XPICKINGTARGET_MOB)
			{
				// 두번째 클릭
				_XMob* pMob = (_XMob*)m_ViewTarget;
				ProcessLPSSystem(pMob);
			}
		}

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}		
	else if(GetMotionClass() == _XACTION_REBIRTH)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}

#ifdef _XDEF_USESCREEENMOTIONBLUR 
			if(g_ScrrenMotionBlurMode)
				g_ScrrenMotionBlurMode = FALSE;
#endif
			
			SetAnimation(_XACTION_IDLE);
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie_VirtualMode();
	}
	else if(GetMotionClass() == _XACTION_MATCH_START)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_MATCH_CANCEL)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			SetAnimation(_XACTION_IDLE);
	}

	// by mahwang
	else if(GetMotionClass() == _XACTION_COLLECT)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(m_CurrentWeaponID > 0)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					
#ifdef _XDEF_MODELTHREADLOADING
					if( WeaponItem && WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}

#else
					if( WeaponItem )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
#endif
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
			else
			{
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			}

			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XLocalUser::ProcessBattle_VirtualMode(void)
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
		//ProcessBattleActiveSkill();
		ProcessActiveSkill_VirtualMode();
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		break;
	case _XSI_USEFULTYPE_HIDDENMA : // 가상공간에서도 내공심법을 할 수 있도록 한다.
		ProcessBattleHiddenMA();
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
		break;
	case _XSI_USEFULTYPE_ACTIONSKILL :
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
					TCHAR messagestring[256];
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SkillList[i].m_sSkillID]->skillName);
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
						TCHAR messagestring[256];
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
				else if(m_BattleMode == _XBATTLE_PEACE || m_CurrentBattleState != _XUSER_STATE_NONE)
				{
					if(g_LocalSystemTime - m_SelectedSkillItem->m_dwCooldownStartTime > m_SelectedSkillItem->m_dwCooldownTime && m_SelectedSkillItem->m_dwCooldownStartTime > 0)
					{
						TCHAR messagestring[256];
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
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
						TCHAR messagestring[256];
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LOCALUSER_STARTSKILL), g_SkillProperty[m_SelectedSkillID]->skillName);
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
						m_SelectedSkillItem->m_dwCooldownStartTime = 0;
					}
				}
			}
		}
	}

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

void _XLocalUser::ProcessActiveSkill_VirtualMode(void)
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
					
					if( !pMob->m_bClientDNPC )// 클라이언트 NPC는 죽일 수 없다.
					{
						if(CheckDistanceToTarget(targetposition, m_SelectedSkillID, range, targettype))
						{
							if( !ProcessLPSSystem(pMob) )
							{
								m_CurrentBattleState = _XUSER_STATE_2;
								SetSkillAnimation();
								SetAnimation(_XACTION_ATTACK);
								//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							}
						}
						else
						{
							SetTargetPosition(targetposition, range);
	//						SetAnimation(_XACTION_MOVE);
						}
					}
					
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
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
							//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
							//g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
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
					VIEWPACKETLOG
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

					if(g_BattleLog.GetWriteLog())
					{
						g_BattleLog.SetTargetGroup(m_AttackTargetObjectType);
						g_BattleLog.SetTargetType(targettype);
						g_BattleLog.SetSkillId(m_SelectedSkillID);
					}

					if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
					{
						// 무조건 친다 - 거리 check하지 않음
						m_bUsingSkill = TRUE;

						//g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, m_CurrentAttackSequence-1);
						
						if(m_SelectedSkillID > 0)
							m_AttackPointNumber = g_SkillProperty[m_SelectedSkillID]->attackPointFor1;
						else 
							m_AttackPointNumber = 2;
						m_CurrentAttackPoint = 0;
						
						// target 쪽으로 돌아선다.
						m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
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
								
								//g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE, m_CurrentAttackSequence-1);
								
								if(m_SelectedSkillID > 0)
									m_AttackPointNumber = g_SkillProperty[m_SelectedSkillID]->attackPointFor2 - g_SkillProperty[m_SelectedSkillID]->attackPointFor1;
								else
									m_AttackPointNumber = 2;
								m_CurrentAttackPoint = 0;
								
								m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, targetposition);
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
								ProcessMobDie_VirtualMode();
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
										ProcessMobDie_VirtualMode();
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
									ProcessMobDie_VirtualMode();
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
										ProcessMobDie_VirtualMode();
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
										ProcessMobDie_VirtualMode();
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
									ProcessMobDie_VirtualMode();
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
						ProcessMobDie_VirtualMode();
				}
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{				
				if(m_AttackResult.ucToType == en_target_type_monster)
				{
					if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
					{
						_XMob* pMob = NULL;
						pMob = (_XMob*)m_AttackTarget;
						if(!pMob)
							break;
						
						int totaldamage = 0;

						// damage 계산
						int tmp = _XRandom()%100;
						UpdateLocalUserAttackLevel();
						if( tmp > 30 )
						{
							m_AttackResult.usAtkInfo[m_CurrentAttackSequence-1] = 0;
							m_AttackResult.sDamage[m_CurrentAttackSequence-1] = 30;//m_CharacterInfo.maxattacklevel;
							pMob->m_CurrentLife -= 30;
							totaldamage = 45;//m_CharacterInfo.maxattacklevel;
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
								m_AttackResult.sDamage[m_CurrentAttackSequence-1] = 60;//m_CharacterInfo.maxattacklevel;
								pMob->m_CurrentLife -= 60;//m_CharacterInfo.maxattacklevel;
								totaldamage = 60;//m_CharacterInfo.maxattacklevel;
							}									
						}
						
						//m_AttackResult.display[m_CurrentAttackSequence-1] = TRUE;							
						if ( pMob->m_CurrentLife <= 0 )
							pMob->m_HealthPercentage = -1;
						else
							pMob->m_HealthPercentage = char(pMob->m_CurrentLife*100/pMob->m_MaxLife);			
						
					//	totaldamage = 15;//m_CharacterInfo.maxattacklevel;
						// damage 계산 끝

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
								if( (m_CurrentAttackPoint-1) == 0 )
								{
									int iTotalDamage = totaldamage;

									// Visual Effect - 첫타에 한번만 출력(타)
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT, &resulteffectmatrix );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Active/Seq[%d] HIT damage[%d]", m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);
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
								if( (m_CurrentAttackPoint-1) == 0 )
								{
									int iTotalDamage = totaldamage;

									// Visual Effect - 첫타에 한번만 출력(살)
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI, &resulteffectmatrix );
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Active/Seq[%d] CRITICAL damage[%d]", m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);
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
								}

								g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 0.2f );
							}
							break;
						case 2 :
							{
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Miss
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Active/Seq[%d] MISS ", m_CurrentAttackPoint-1);
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
							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								// Block
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Active/Seq[%d] BLOCK ", m_CurrentAttackPoint-1);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
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
									sprintf(messagestring, "Active/Seq[%d] SHOCK damage[%d]", m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_NORMAL);
								}
							#endif
							}
							break;
						case 5 :
							{
								// 상극 일반
								if( (m_CurrentAttackPoint-1) == 0 )
								{
									int iTotalDamage = totaldamage;

									// Visual Effect - 첫타에 한번만 출력 (타)
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_ATT1, &resulteffectmatrix );
										
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Active/Seq[%d] 상극일반 damage[%d]", m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);	// not use string
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
								if( (m_CurrentAttackPoint-1) == 0 )
								{
									int iTotalDamage = totaldamage;

									// Visual Effect - 첫타에 한번만 출력 (살)
									if( pMob->m_ModelDescriptor.m_EffectManager )
									{
										D3DXMATRIX resulteffectmatrix = pMob->m_ModelDescriptor.m_Position;
										resulteffectmatrix._42 = resulteffectmatrix._42 - 1.3f + (pMob->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2.0f);
										pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_CRI1, &resulteffectmatrix );
										
									}
								}

							#ifdef _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
								if(g_ViewBattleDetail)
								{
									TCHAR messagestring[128];
									sprintf(messagestring, "Active/Seq[%d] 상극 크리 damage[%d]", m_CurrentAttackPoint-1, m_AttackResult.sDamage[m_CurrentAttackPoint-1]);	// not use string 
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
						ProcessMobDie_VirtualMode();
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

void _XLocalUser::ProcessMobDie_VirtualMode(void)
{
//	_XMob* pAttackTarget = NULL;
//	if(m_AttackTarget && m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
//	{
//		pAttackTarget = reinterpret_cast<_XMob*>(m_AttackTarget);		
//	}	
//	
//	if( !pAttackTarget ) return;
	
	_XMob* pMob = NULL;
	if( g_Moblist.m_smMobList.empty() ) return;				
	smdef_XMob::iterator it;
	for (it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ it)
	{
		pMob = (*it).second;
//		FLOAT dx = (pMob->m_Position.x - pAttackTarget->m_Position.x);
//		FLOAT dz = (pMob->m_Position.z - pAttackTarget->m_Position.z);
//		float length = _XFC_sqrt(dx*dx + dz*dz);
//		
//		if( length < 1.0f )
		{
			//for(int i = 0 ; i < 3 ; ++i)
			{
				//if(m_AttackResult.display[i])
				{
					if(pMob)
					{
						if(pMob->m_HealthPercentage <= 0)
						{
							if( pMob->m_Action.action != _XMOBACTION_DIE )
							{
								pMob->SetAnimation(_XMOBACTION_DIE);					
								pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
								
								// 가상모드에서는 서버에서 몬스터 죽음 결과를 보내지 않으므로
								// 클라이언트에서 직접 검사한다.
								m_CharacterLog.isKill = TRUE;
								m_CharacterLog.killTime = g_ServerTimeCode;
								m_CharacterLog.killTargetType = pMob->m_MobType;								
									
				//				m_AttackResult.display[i] = FALSE;
								break;
							}
						}
					}
			//		m_AttackResult.display[i] = FALSE;
				}
			}
		}
	}
}
