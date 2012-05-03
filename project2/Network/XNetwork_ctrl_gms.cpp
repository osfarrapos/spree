// XNetwork_ctrl_gms.cpp : 게임 control 관련
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XWindow_JinStatus.h"
#include "XWindow_CInfo.h"
#include "XWindow_Skill.h"
#include "XWindowObjectDefine.h"
#include "XWindow_MainMenu.h"
#include "XSR_STRINGHEADER.h"

void _XNetwork::MSGCtrlCharacSetting(MSG_CTRL_CHARAC_SETTING* characsetting)
{
	switch(characsetting->cCommand)
	{
	case ctrl_ch_point :
		{
			// 5기, 레벨, 포인트 조작
			switch(characsetting->point.cWhere)
			{
			case 0 :
				{
					// 건강과 생명
					g_pLocalUser->m_CharacterInfo.Set_constitution( characsetting->point.sValue1 );
					g_pLocalUser->m_CharacterInfo.Set_current_lifepower( characsetting->point.sValue2 );
					g_pLocalUser->m_CharacterInfo.Set_max_lifepower( characsetting->point.sValue2 );

					g_pLocalUser->m_Constitution.Set_org(g_pLocalUser->m_CharacterInfo.Get_constitution());
					g_pLocalUser->m_MaxLifePower.Set_org(g_pLocalUser->m_CharacterInfo.Get_max_lifepower());

					g_pLocalUser->UpdateLocalUserAttackLevel();

					if( g_pJinStatus_Window )
					{
						g_pJinStatus_Window->SetParameter();	
					}

					if(g_pCInfo_Window)
					{
						if(g_pCInfo_Window->GetShowStatus())
						   g_pCInfo_Window->SetParameter();
					}
				}
				break;
			case 1 :
				{
					// 진기와 내공
					g_pLocalUser->m_CharacterInfo.Set_zen(characsetting->point.sValue1);
					g_pLocalUser->m_CharacterInfo.Set_current_forcepower( characsetting->point.sValue2 );
					g_pLocalUser->m_CharacterInfo.Set_max_forcepower( characsetting->point.sValue2 );

					g_pLocalUser->m_Zen.Set_org(g_pLocalUser->m_CharacterInfo.Get_zen());
					g_pLocalUser->m_MaxForcePower.Set_org(g_pLocalUser->m_CharacterInfo.Get_max_forcepower());

					g_pLocalUser->UpdateLocalUserAttackLevel();

					if( g_pJinStatus_Window )
					{
						g_pJinStatus_Window->SetParameter();	
					}		

					if(g_pCInfo_Window)
					{
						if(g_pCInfo_Window->GetShowStatus())
						   g_pCInfo_Window->SetParameter();
					}
				}
				break;
			case 2 :
				{
					// 지혜와 의지
					g_pLocalUser->m_CharacterInfo.Set_intelligence( characsetting->point.sValue1 );
					g_pLocalUser->m_CharacterInfo.Set_current_concentrationpower( characsetting->point.sValue2 );
					g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower( characsetting->point.sValue2 );

					g_pLocalUser->m_Intelligence.Set_org(g_pLocalUser->m_CharacterInfo.Get_intelligence());
					g_pLocalUser->m_MaxConcentrationPower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );

					g_pLocalUser->UpdateLocalUserAttackLevel();

					if( g_pJinStatus_Window )
					{
						g_pJinStatus_Window->SetParameter();	
					}		

					if(g_pCInfo_Window)
					{
						if(g_pCInfo_Window->GetShowStatus())
						   g_pCInfo_Window->SetParameter();
					}
				}
				break;
			case 3 :
				{
					// 민첩
					g_pLocalUser->m_CharacterInfo.Set_dexterity( characsetting->point.sValue1 );
					g_pLocalUser->m_Dex.Set_org( g_pLocalUser->m_CharacterInfo.Get_dexterity() );

					g_pLocalUser->UpdateLocalUserAttackLevel();

					if( g_pJinStatus_Window )
					{
						g_pJinStatus_Window->SetParameter();	
					}		

					if(g_pCInfo_Window)
					{
						if(g_pCInfo_Window->GetShowStatus())
						   g_pCInfo_Window->SetParameter();
					}
				}
				break;
			case 4 :
				{
					// 근력
					g_pLocalUser->m_CharacterInfo.Set_strength( characsetting->point.sValue1 );
					g_pLocalUser->m_Strength.Set_org( g_pLocalUser->m_CharacterInfo.Get_strength() );

					g_pLocalUser->UpdateLocalUserAttackLevel();

					if( g_pJinStatus_Window )
					{
						g_pJinStatus_Window->SetParameter();	
					}		

					if(g_pCInfo_Window)
					{
						if(g_pCInfo_Window->GetShowStatus())
						   g_pCInfo_Window->SetParameter();
					}
				}
				break;
			case 5 :
				{
					// 남은 포인트
					g_pLocalUser->m_CharacterInfo.Set_leftpoint( characsetting->point.sValue1 );

					if( g_pJinStatus_Window )
					{
						g_pJinStatus_Window->SetParameter();	
					}		
				}
				break;
			}
		}
		break;

	case ctrl_ch_position :
		{
			// 캐릭터 위치 조작
			if(characsetting->position.cZone == 0 || g_CurrentZoneInfoIndex == characsetting->position.cZone)
			{
				//g_pLocalUser->m_PrevPosition.x = characsetting->position.fX;
				//g_pLocalUser->m_PrevPosition.z = characsetting->position.fZ;
				//g_pLocalUser->m_PrevPosition.y = g_LodTerrain.GetTerrainAndRideOBBHeight(g_pLocalUser->m_Position, g_pLocalUser->m_pGoundCollideOBB);

				g_pLocalUser->m_TargetPosition.x = characsetting->position.fX + 0.01;
				g_pLocalUser->m_TargetPosition.z = characsetting->position.fZ + 0.01;

				// 서버 이동 없음
				g_pLocalUser->m_Position.x = characsetting->position.fX;
				g_pLocalUser->m_Position.z = characsetting->position.fZ;
				g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(g_pLocalUser->m_Position, g_pLocalUser->m_pGoundCollideOBB);
			}
		}
		break;

	case ctrl_ch_charac_moonpa :
		{
			// 캐릭터 문파 변경
			g_pLocalUser->m_CharacterInfo.groupindex = (_XGROUPTYPE)characsetting->moonpa.cToMoonpa;
			
			if(g_pSkill_Window)
				g_pSkill_Window->SetSkillButton();

			_XWindow_NPCSkill* pNPCSkill_Window = (_XWindow_NPCSkill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSKILL);
			if(pNPCSkill_Window)
			{
				if(pNPCSkill_Window->GetShowStatus())
					pNPCSkill_Window->SetSkillTypeButton();
//					pNPCSkill_Window->SetListItem();
			}
		}
		break;

	case ctrl_ch_name :
		{
			 // 캐릭터 이름 변경
			if(characsetting->nickname.cMode == 0)
			{
				// 이름 변경
				strncpy(g_pLocalUser->m_CharacterInfo.charactername, characsetting->nickname.cName, 12);
				g_pLocalUser->m_CharacterInfo.charactername[12] = 0;
			}
			else
			{
				// 닉네임 변경
			}
		}
		break;

	case ctrl_ch_item :
		{
			// 인벤토리, 저장소 아이템 변경
			if(characsetting->item.cMode == 0)
			{
				// 인벤토리 세팅
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_cType(characsetting->item.useritem.cType);
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_cSecond(characsetting->item.useritem.cSecond);
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_sID( characsetting->item.useritem.sID );
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_cIdentify = characsetting->item.useritem.Item_Money.S_NormalItem.cIdentify;

				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_cThird( _XGameItem::GetThird(characsetting->item.useritem.cType, characsetting->item.useritem.sID));
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucCount( characsetting->item.useritem.Item_Money.S_NormalItem.ucCount);		
					
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_usCurDur(characsetting->item.useritem.Item_Money.S_NormalItem.usCurDur);
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_usMaxDur(_XGameItem::GetMaxDurability(characsetting->item.useritem.cType, characsetting->item.useritem.sID));

				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucStrength( characsetting->item.useritem.ucStrength );

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_usCurProtect = characsetting->item.useritem.uselimit.usCurProtect;
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_usCurMaxProtect = characsetting->item.useritem.uselimit.usCurMaxProtect;
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_usUsedProtect = characsetting->item.useritem.uselimit.usUsedProtect;
				
				//무기강도
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature( characsetting->item.useritem.cType, characsetting->item.useritem.sID) );
#endif

				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucSocketCount( characsetting->item.useritem.Item_Money.S_NormalItem.ucSocketCount );
				if( characsetting->item.useritem.Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_sSocket( i, characsetting->item.useritem.sSocket[i] );
				}

				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_ucFlag1 = characsetting->item.useritem.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_ucFlag2	= characsetting->item.useritem.uselimit.ucFlag2;	
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_ucFlag3	= characsetting->item.useritem.uselimit.ucFlag3;
#endif
				g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].m_usValue1	= characsetting->item.useritem.uselimit.usValue1;

				if( characsetting->item.useritem.uselimit.ucYear > 0 )
				{					
					g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucYear(characsetting->item.useritem.uselimit.ucYear);	
					g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucMonth(characsetting->item.useritem.uselimit.ucMonth);	
					g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucDay(characsetting->item.useritem.uselimit.ucDay);	
					g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucHour(characsetting->item.useritem.uselimit.ucHour);	
					g_pLocalUser->m_UserItemList[characsetting->item.useritem.cSlot].Set_m_ucMinute(characsetting->item.useritem.uselimit.ucMinute);
				}
			}
			else
			{
				// 창고 아이템 변경
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_cType(characsetting->item.useritem.cType);
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_cSecond(characsetting->item.useritem.cSecond);
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_sID( characsetting->item.useritem.sID );
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_cIdentify = characsetting->item.useritem.Item_Money.S_NormalItem.cIdentify;
				
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_cThird(_XGameItem::GetThird(characsetting->item.useritem.cType, characsetting->item.useritem.sID));
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucCount(characsetting->item.useritem.Item_Money.S_NormalItem.ucCount);
				
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_usCurDur( characsetting->item.useritem.Item_Money.S_NormalItem.usCurDur );
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(characsetting->item.useritem.cType, characsetting->item.useritem.sID) );

				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucStrength( characsetting->item.useritem.ucStrength );

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_usCurProtect = characsetting->item.useritem.uselimit.usCurProtect;
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_usCurMaxProtect = characsetting->item.useritem.uselimit.usCurMaxProtect;
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_usUsedProtect = characsetting->item.useritem.uselimit.usUsedProtect;

				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature( characsetting->item.useritem.cType, characsetting->item.useritem.sID) );
#endif
				
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucSocketCount( characsetting->item.useritem.Item_Money.S_NormalItem.ucSocketCount );
				if( characsetting->item.useritem.Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_sSocket( i, characsetting->item.useritem.sSocket[i] );
				}

				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_ucFlag1 = characsetting->item.useritem.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다 
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_ucFlag2		= characsetting->item.useritem.uselimit.ucFlag2;	
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_ucFlag3		= characsetting->item.useritem.uselimit.ucFlag3;
#endif
				g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].m_usValue1	= characsetting->item.useritem.uselimit.usValue1;

				if( characsetting->item.useritem.uselimit.ucYear > 0 )
				{					
					g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucYear( characsetting->item.useritem.uselimit.ucYear);	
					g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucMonth(characsetting->item.useritem.uselimit.ucMonth);	
					g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucDay(characsetting->item.useritem.uselimit.ucDay);	
					g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucHour( characsetting->item.useritem.uselimit.ucHour);	
					g_pLocalUser->m_WarehouseItemList[characsetting->item.useritem.cSlot].Set_m_ucMinute(characsetting->item.useritem.uselimit.ucMinute);
				}
			}
		}
		break;

	case ctrl_ch_money :
		{
			// 돈
			if(characsetting->money.cWhere == 0)
			{
				// 인벤토리
				g_pLocalUser->m_Money = characsetting->money.uiGold;
			}
			else
			{
				// 창고
				g_pLocalUser->m_WarehouseMoney = characsetting->money.uiGold;
			}
		}
		break;

	case ctrl_ch_skill :
		{
			// 스킬
			switch(characsetting->skill.cMode)
			{
			case 0 :
				{
					// Add skill
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_sSkillID = characsetting->skill.sSkillID;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cSkillLevel = characsetting->skill.cSkillStep;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cCurMission = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_dwCooldownStartTime = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_ucSkillState = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cSelectedSkillLevel = characsetting->skill.cSkillStep;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = characsetting->skill.iSkillExp;

					if(g_pSkill_Window)
					{
						g_pSkill_Window->SetListItem();
					}

					if(g_pMasterySkill_Window)
					{
						g_pMasterySkill_Window->ParseMasterySkill();
						g_pMasterySkill_Window->CheckDependency();
					}
				}
				break;
			case 1 :
				{
					// remove skill
					if(g_pQuickSlot_Window)
					{
						g_pQuickSlot_Window->DeleteQuickSlot((short)characsetting->skill.sSkillID);
					}

					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_sSkillID = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cSkillLevel = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cCurMission = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_dwCooldownStartTime = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_ucSkillState = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = 0;

					if(g_pSkill_Window)
					{
						g_pSkill_Window->SetSkillButton();
					}
					if(g_pMasterySkill_Window)
					{
						g_pMasterySkill_Window->ParseMasterySkill();
						g_pMasterySkill_Window->CheckDependency();
					}
				}
				break;
			case 2 :
				{
					// edit skill
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_sSkillID = characsetting->skill.sSkillID;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cSkillLevel = characsetting->skill.cSkillStep;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cCurMission = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_dwCooldownStartTime = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_ucSkillState = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cSelectedSkillLevel = characsetting->skill.cSkillStep;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = characsetting->skill.iSkillExp;
					
					if(g_pSkill_Window)
					{
						g_pSkill_Window->SetSkillButton();
					}
				}
				break;
			case 3 :
				{
					// trainingstate = 1
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_sSkillID = characsetting->skill.sSkillID;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_sSkillID = characsetting->skill.sSkillID;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cSkillLevel = characsetting->skill.cSkillStep;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cCurMission = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_dwCooldownStartTime = 0;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_ucSkillState = 1;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_cSelectedSkillLevel = characsetting->skill.cSkillStep;
					g_pLocalUser->m_SkillList[characsetting->skill.ucSkillSlot].m_iSkillExp = characsetting->skill.iSkillExp;

					if(g_pSkill_Window)
					{
						g_pSkill_Window->SetSkillButton();
					}
				}
				break;
			}
			
			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
			{
				pMainMenu_Window->SetMedButton();
			}
		}
		break;
	case ctrl_ch_class_grade :
		{
			g_pLocalUser->m_CharacterInfo.Set_cClass( characsetting->Class.cClass );
			g_pLocalUser->m_CharacterInfo.Set_cClassGrade( characsetting->Class.cGrade );
			g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() + characsetting->Class.sAddMasteryPoint );

			// 찍었던 Mastery point 모두 초기화
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
//				g_pMasterySkill_Window->ParseMasterySkill();
//				g_pMasterySkill_Window->CheckDependency();
			}

			if(g_pSkill_Window)
				g_pSkill_Window->SetSkillButton();
			
			_XWindow_NPCSkill* pNPCSkill_Window = (_XWindow_NPCSkill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSKILL);
			if(pNPCSkill_Window)
			{
				if(pNPCSkill_Window->GetShowStatus())
					pNPCSkill_Window->SetSkillTypeButton();
//					pNPCSkill_Window->SetListItem();
			}

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			if(g_pJinStatus_Window)
				g_pJinStatus_Window->SetClassIcon();	// class icon 세팅
#endif
		}
		break;
	case ctrl_ch_set_transparent :
		{
			if(characsetting->Transparent.cMode == 1)
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2462), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
			else if(characsetting->Transparent.cMode == 0)
				InsertChatString("Invisible skill off", _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
			
		}
		break;
	case ctrl_ch_set_all_point :
		{
			g_pLocalUser->m_CharacterInfo.Set_strength( characsetting->AllPoint.sStrength );
			g_pLocalUser->m_Strength.Set_org( characsetting->AllPoint.sStrength );

			g_pLocalUser->m_CharacterInfo.Set_zen( characsetting->AllPoint.sZen );
			g_pLocalUser->m_Zen.Set_org( characsetting->AllPoint.sZen );

			g_pLocalUser->m_CharacterInfo.Set_intelligence( characsetting->AllPoint.sIntelligence );
			g_pLocalUser->m_Intelligence.Set_org( characsetting->AllPoint.sIntelligence );

			g_pLocalUser->m_CharacterInfo.Set_constitution( characsetting->AllPoint.sConstitution );
			g_pLocalUser->m_Constitution.Set_org( characsetting->AllPoint.sConstitution );

			g_pLocalUser->m_CharacterInfo.Set_dexterity( characsetting->AllPoint.sDexterity );
			g_pLocalUser->m_Dex.Set_org( characsetting->AllPoint.sDexterity );

			g_pLocalUser->m_CharacterInfo.Set_max_lifepower( characsetting->AllPoint.sMaxLifePower );
			g_pLocalUser->m_MaxLifePower.Set_org( characsetting->AllPoint.sMaxLifePower );

			g_pLocalUser->m_CharacterInfo.Set_max_forcepower( characsetting->AllPoint.sMaxForcePower );
			g_pLocalUser->m_MaxForcePower.Set_org( characsetting->AllPoint.sMaxForcePower );

			g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower( characsetting->AllPoint.sMaxConcentrationPower );
			g_pLocalUser->m_MaxConcentrationPower.Set_org( characsetting->AllPoint.sMaxConcentrationPower );

			g_pLocalUser->UpdateLocalUserAttackLevel();

			// 남은 스탯 포인트
			g_pLocalUser->m_CharacterInfo.Set_leftpoint( characsetting->AllPoint.sLeftStat );
			// 남은 오의 포인트
			g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( characsetting->AllPoint.sLeftMastery );
			
			if( g_pJinStatus_Window )
			{
				g_pJinStatus_Window->SetParameter();	
			}		
			
			if(g_pCInfo_Window)
			{
				if(g_pCInfo_Window->GetShowStatus())
					g_pCInfo_Window->SetParameter();
			}
		}
		break;
	case ctrl_ch_contribution :
		{
			g_pLocalUser->m_CharacterInfo.Set_iContribution( characsetting->iContribution );
		}
		break;
	case ctrl_ch_mastery_point :
		{
			g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( characsetting->sMasteryPoint );
		}
		break;
	case ctrl_ch_set_mutetime :
		{
			g_pLocalUser->m_ChatMuteTime = characsetting->iMuteTime * 1000;
		}
		break;
	}
}