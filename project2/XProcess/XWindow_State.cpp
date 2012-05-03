// XWindow_State.cpp: implementation of the _XWindow_State class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_State.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"

_XWindow_State* g_pState_Window = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_State::_XWindow_State()
{
	m_BarImage = NULL;
	m_PreProcessTime = 0;	
	m_BuffCancleindex = 0;
	m_BuffCancleMode  = 0;
}

_XWindow_State::~_XWindow_State()
{

}

BOOL _XWindow_State::Initailize(void)
{
	g_pState_Window = this;

	m_BarImage = new _XImageStatic;
	m_BarImage->Create(0, 0, 5, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName));
	m_BarImage->SetClipRect(251, 3, 256, 33);

	_XTOOLTIP_STRUCTURE tooltipstruct = 
	{
		{ 0, 0 },
		{ 78, 13 },
		{ 1, 1 },
		-1,
		NULL
	};	

	return TRUE;
}

void _XWindow_State::DestroyWindow(void)
{
	DeleteAllStateList();

	SAFE_DELETE(m_BarImage);	

	_XWindow::DestroyWindow();
}

void _XWindow_State::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
	DrawStateList();
	DrawStateListTooltip();
}

BOOL _XWindow_State::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	if(m_PreProcessTime <= 0)
		m_PreProcessTime = g_LocalSystemTime;

	DWORD elapsedtime = g_LocalSystemTime - m_PreProcessTime;
	
	// TODO
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; )
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == _XST_ITEM)
			{
				if(currentslot->slot.slot_item.ctype == _XGI_FC_POTION)
				{
					switch(g_PotionItemProperty[currentslot->slot.slot_item.id].cSecondType)
					{
					case _XGI_SC_POTION_HERT_IN :
					case _XGI_SC_POTION_HERT_OUT :
					case _XGI_SC_POTION_FOOD :
					case _XGI_SC_POTION_FATIGUE :
						{
							if((g_PotionItemProperty[currentslot->slot.slot_item.id].sCooldown*1000 - (g_LocalSystemTime - currentslot->slot.slot_item.starttime)) <= 5000)
							{
								// 5초 이하로 남았음
//								currentslot->bSign = true;
//								if(currentslot->showtime <= 0)
//									currentslot->showtime = g_LocalSystemTime;
							}

							if((g_LocalSystemTime - currentslot->slot.slot_item.starttime) > g_PotionItemProperty[currentslot->slot.slot_item.id].sCooldown*1000)
							{
								g_pLocalUser->m_UserItemList[currentslot->slot.slot_item.invennumber].m_dwCooldownStartTime = 0;
								SAFE_DELETE(currentslot->icon);
								SAFE_DELETE(currentslot);
								iter_state = m_PlayerStateList.erase(iter_state);
							}
							else
							{
								iter_state++;
							}
						}
						break;
					}
				}
				else if(currentslot->slot.slot_item.ctype == _XGI_FC_BOOK)
				{
					if(currentslot->showtime <= 0)
					{
//						currentslot->bSign = true;
//						currentslot->showtime = g_LocalSystemTime;
					}

					if((g_LocalSystemTime - currentslot->slot.slot_item.starttime) > 5000)//g_BookItemProperty[currentslot->slot.slot_item.id].usCoolDown*1000)
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						iter_state = m_PlayerStateList.erase(iter_state);
					}
					else
					{
						iter_state++;
					}
				}
				else if(currentslot->slot.slot_item.ctype == _XGI_FC_QUEST)
				{
					if(currentslot->showtime <= 0)
					{
//						currentslot->bSign = true;
//						currentslot->showtime = g_LocalSystemTime;
					}
					
					if((g_LocalSystemTime - currentslot->slot.slot_item.starttime) > 10000)//g_BookItemProperty[currentslot->slot.slot_item.id].usCoolDown*1000)
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						iter_state = m_PlayerStateList.erase(iter_state);
					}
					else
					{
						iter_state++;
					}
				}
				else
				{
					iter_state++;
				}
			}
			else if(currentslot->type == _XST_EXTERNAL)
			{
				if(currentslot->slot.slot_external.beforeexternal > currentslot->slot.slot_external.afterexternal)
				{
//					currentslot->bSign = true;
//					if(currentslot->showtime <= 0)
//						currentslot->showtime = g_LocalSystemTime;

					if(g_LocalSystemTime - currentslot->showtime > g_PotionItemProperty[currentslot->slot.slot_external.useitemid].sCooldown*1000)
					{
						currentslot->slot.slot_external.beforeexternal = currentslot->slot.slot_external.afterexternal;
					}

					iter_state++;
				}
				else
				{
					if(currentslot->slot.slot_external.afterexternal == 0)
					{
						// 외상 모두 감소 - Delete
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						iter_state = m_PlayerStateList.erase(iter_state);
					}
					else
					{
						if(currentslot->slot.slot_external.beforeexternal > 0)
						{
							// 외상 남음 - 치료 끝
							currentslot->slot.slot_external.beforeexternal = 0;
							currentslot->slot.slot_external.decreaserate = 0;
							currentslot->bSign = false;
							currentslot->showtime = 0;
						}
						iter_state++;
					}
				}
			}
			else if(currentslot->type == _XST_INTERNAL)
			{
				if(currentslot->slot.slot_internal.beforeinternal > currentslot->slot.slot_internal.afterinternal)
				{
//					currentslot->bSign = true;
//					if(currentslot->showtime <= 0)
//						currentslot->showtime = g_LocalSystemTime;

					if(g_LocalSystemTime - currentslot->showtime > g_PotionItemProperty[currentslot->slot.slot_internal.useitemid].sCooldown*1000)
					{
						currentslot->slot.slot_internal.beforeinternal = currentslot->slot.slot_internal.afterinternal;
					}

					iter_state++;
				}
				else
				{
					if(currentslot->slot.slot_internal.afterinternal == 0)
					{
						// 내상 모두 감소 - Delete
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						iter_state = m_PlayerStateList.erase(iter_state);
					}
					else
					{
						if(currentslot->slot.slot_internal.beforeinternal > 0)
						{
							// 내상 남음 - 치료 끝
							currentslot->slot.slot_internal.beforeinternal = 0;
							currentslot->slot.slot_internal.decreaserate = 0;
							currentslot->bSign = false;
							currentslot->showtime = 0;
						}
						iter_state++;
					}
				}
			}
			else
			{
				iter_state++;
			}
		}
		else
		{
			iter_state++;
		}

		if(m_PlayerStateList.empty())
			break;
	}

	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == _XST_SKILL)
			{
				int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[currentslot->slot.slot_skill.level].effect[0].id;
				DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_skill, effectid, currentslot->slot.slot_skill.id);
				if(lefttime < 6000)	// 남은 시간이 6초이하
				{
					if(!currentslot->bSign)
					{
						currentslot->bSign = true;
						currentslot->showtime = g_LocalSystemTime;
					}
				}
			}
			else if(currentslot->type == _XST_EFFECT)
			{
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentslot->slot.slot_effect.id);
				if(iter_table == g_CharacterStateTable.end())
					break;
				
				if(currentslot->slot.slot_effect.slotindex >= 0)
				{
#ifdef _XDEF_EXPANSION_POTION_070709_KUKURI
					if( currentslot->slot.slot_effect.slotindex == 0 || currentslot->slot.slot_effect.slotindex == 1 ||
						currentslot->slot.slot_effect.slotindex == 2 || currentslot->slot.slot_effect.slotindex == 4)
#else
					if( currentslot->slot.slot_effect.slotindex == _XGI_SC_POTION_FOOD )
#endif
					{
						MouseState* mousestate = gpInput->GetMouseState();
						if( mousestate->bButton[1] && currentslot->icon->CheckMousePosition() )
						{
							m_BuffCancleindex = currentslot->slot.slot_effect.slotindex;
							m_BuffCancleMode = 1;

							_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
							if( pMessageBox )
								g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
							
							pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2773), TRUE, _XDEF_STATEWINDOW_STATECANCLEYES, _XDEF_STATEWINDOW_STATECANCLENO);	// _T("효과를 해제하시겠습니까?")
						}
					}
					DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_extra, currentslot->slot.slot_effect.id, 0, currentslot->slot.slot_effect.slotindex);
					if(lefttime < 6000)
					{
						if(!currentslot->bSign)
						{
							currentslot->bSign = true;
							currentslot->showtime = g_LocalSystemTime;
						}
					}
				}
				else
				{
					if( currentslot->slot.slot_effect.id == 347 ) // 복면 상태 일 경우
					{
						MouseState* mousestate = gpInput->GetMouseState();
						if( mousestate->bButton[1] && currentslot->icon->CheckMousePosition() )
						{
							if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2772), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "대전이 종료되지 않아 복면을 해제할 수 없습니다."
							}
							else
							{
								m_BuffCancleindex = 0;
								m_BuffCancleMode = 0;
								
								_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
								if( pMessageBox )
									g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
								
								pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2774), TRUE, _XDEF_STATEWINDOW_STATECANCLEYES, _XDEF_STATEWINDOW_STATECANCLENO);	// _T("복면을 해제하시겠습니까?")
							}							
						}						
					}					
					DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_effect, currentslot->slot.slot_effect.id);
					if(lefttime < 6000)
					{
						if(!currentslot->bSign)
						{
							currentslot->bSign = true;
							currentslot->showtime = g_LocalSystemTime;
						}
					}
				}
			}
			else if(currentslot->type == _XST_EXTRASKILL)
			{
				int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;
				DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_extraskill, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.level);
				if(lefttime < 6000)	// 남은 시간이 6초이하
				{
					if(!currentslot->bSign)
					{
						currentslot->bSign = true;
						currentslot->showtime = g_LocalSystemTime;
					}
				}
			}

			//Author :양희왕 //breif : 추가
			else if(currentslot->type == _XST_SPECIALBUFF)
			{
				int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;
				DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_specialbuff, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.slotindex);
				if(lefttime < 6000)	// 남은 시간이 6초이하
				{
					if(!currentslot->bSign)
					{
						currentslot->bSign = true;
						currentslot->showtime = g_LocalSystemTime;
					}
				}
			}

            //Author :양희왕 //breif : 무공서도 남은시간 깜밖이게
            else if(currentslot->type == _XST_STUDYBOOK)
			{
                int lefttime = currentslot->slot.slot_studybook.lefttime - (g_LocalSystemTime - currentslot->slot.slot_studybook.starttime)/1000;
				if(lefttime < 6)	// 남은 시간이 6초이하
				{
					if(!currentslot->bSign)
					{
						currentslot->bSign = true;
						currentslot->showtime = g_LocalSystemTime;
					}
				}
			}
		}
	} // Buff

	for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == _XST_SKILL)
			{
				int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[currentslot->slot.slot_skill.level].effect[0].id;
				DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_skill, effectid, currentslot->slot.slot_skill.id);
				if(lefttime < 6000)	// 남은 시간이 6초이하
				{
					if(!currentslot->bSign)
					{
						currentslot->bSign = true;
						currentslot->showtime = g_LocalSystemTime;
					}
				}
			}
			else if(currentslot->type == _XST_EFFECT)
			{
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentslot->slot.slot_effect.id);
				if(iter_table == g_CharacterStateTable.end())
					break;
				
				if(currentslot->slot.slot_effect.slotindex >= 0)
				{
					DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_extra, currentslot->slot.slot_effect.id, 0, currentslot->slot.slot_effect.slotindex);
					if(lefttime < 6000) // 남은 시간이 6초이하
					{
						if(!currentslot->bSign)
						{
							currentslot->bSign = true;
							currentslot->showtime = g_LocalSystemTime;
						}
					}
				}
				else
				{
					DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_effect, currentslot->slot.slot_effect.id);
					if(lefttime < 6000) // 남은 시간이 6초이하
					{
						if(!currentslot->bSign)
						{
							currentslot->bSign = true;
							currentslot->showtime = g_LocalSystemTime;
						}
					}
				}
			}
			else if(currentslot->type == _XST_EXTRASKILL)
			{
				int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;
				DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_extraskill, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.level);
				if(lefttime < 6000)	// 남은 시간이 6초이하
				{
					if(!currentslot->bSign)
					{
						currentslot->bSign = true;
						currentslot->showtime = g_LocalSystemTime;
					}
				}
			}
			//Author :양희왕 //breif : 추가
			else if(currentslot->type == _XST_SPECIALBUFF)
			{
				int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;
				DWORD lefttime = g_pLocalUser->GetLeftTimeFromStateList(en_from_specialbuff, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.slotindex);
				if(lefttime < 6000)	// 남은 시간이 6초이하
				{
					if(!currentslot->bSign)
					{
						currentslot->bSign = true;
						currentslot->showtime = g_LocalSystemTime;
					}
				}
			}
		}
	} // Hostile

	m_PreProcessTime = g_LocalSystemTime;

	if(_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_State::InsertStateListFromExIn(_XStateType type, short beforevalue, short aftervalue)	// for external, internal
{
	if(type != _XST_EXTERNAL && type != _XST_INTERNAL)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == type)		// 이미 삽입 되어있음
				return;
		}
	}

	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;
	
	int imageindex = 0;
	RECT rect;
	switch(newslot->type)
	{
	case _XST_EXTERNAL :
		{
			rect.left = 1;
			rect.top = 1;
			rect.right = 31;
			rect.bottom = 31;

			imageindex = g_MainInterfaceTextureArchive.FindResource("Condition_icon_01.tga");

			newslot->slot.slot_external.beforeexternal = beforevalue;
			newslot->slot.slot_internal.afterinternal = aftervalue;
		}
		break;
	case _XST_INTERNAL :
		{
			rect.left = 33;
			rect.top = 1;
			rect.right = 63;
			rect.bottom = 31;
			
			imageindex = g_MainInterfaceTextureArchive.FindResource("Condition_icon_01.tga");

			newslot->slot.slot_internal.beforeinternal = beforevalue;
			newslot->slot.slot_internal.afterinternal = aftervalue;
		}
		break;
	}
	
	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	newslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	newslot->icon->SetScale(0.94f, 0.94f);
	
	m_PlayerStateList.push_back(newslot);
}

// item이 아닐 때는 ctype == slotindex
void _XWindow_State::InsertStateListFromOther(_XStateType type, short id, char ctype, char invennumber)
{
	if(type != _XST_ITEM && type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE && type != _XST_INTOXICATION && 
		type != _XST_NICKBUFF && type != _XST_PEMODE && type != _XST_EXTRASKILL && type != _XST_SPECIALBUFF)
		return;

	switch(type)
	{
	case _XST_ITEM :
		InsertPlayerStateList(type, id, ctype, invennumber);
		break;
		
	case _XST_SKILL :
		{
			char skilllevel = 0;
			skilllevel = ctype;
			if(g_SkillProperty[id]->detailInfo[skilllevel].effect)
			{
				short effectid = g_SkillProperty[id]->detailInfo[skilllevel].effect[0].id;

				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectid);
				if(iter_table == g_CharacterStateTable.end())
					break;
				
				if(g_CharacterStateTable[effectid]->type == 0)
					InsertBuffStateList(type, id, skilllevel);
				else 
					InsertHostileStateList(type, id, skilllevel);
			}
		}
		break;
	case _XST_EFFECT :
		{
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
			if(iter_table == g_CharacterStateTable.end())
				break;

			if(g_CharacterStateTable[id]->type == 0)
				InsertBuffStateList(type, id, ctype);
			else 
				InsertHostileStateList(type, id, ctype);
		}
		break;
	case _XST_PASSIVE :
		{
			char skilllevel = 0;
			skilllevel = ctype;
			
			if(g_SkillProperty[id]->detailInfo[skilllevel].effect)
			{
				short effectid = g_SkillProperty[id]->detailInfo[skilllevel].effect[0].id;
				
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectid);
				if(iter_table == g_CharacterStateTable.end())
					break;
				
				if(g_CharacterStateTable[effectid]->type == 0)
					InsertBuffStateList(type, id, skilllevel);
				else 
					InsertHostileStateList(type, id, skilllevel);
			}
		}
		break;
	case _XST_INTOXICATION :
		{
			InsertIntoxication(type, id);			
		}
		break;
	case _XST_NICKBUFF :
		{
			InsertNicknameBuff(type, id);
		}
		break;
	case _XST_PEMODE :
		{
			InsertPEModeIcon(type, id);
		}
		break;
	case _XST_SPECIALBUFF :
		{
			// ctype : slot index
			// invennumber : level
			char slot = 0;
			slot = ctype;

			char level = invennumber;
			if(level < 0 && level > 11)
			{
				level = 0;
			}

			if(g_SkillProperty[id]->detailInfo[level].effect)
			{
				short effectid = g_SkillProperty[id]->detailInfo[level].effect[0].id;

				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectid);
				if(iter_table == g_CharacterStateTable.end())
					break;
				
				if(g_CharacterStateTable[effectid]->type == 0)
					InsertBuffStateList(type, id, slot, level);
				else 
					InsertHostileStateList(type, id, slot, level);
			}
		}
		break;
	case _XST_EXTRASKILL :
		{
			char slot = 0;
			slot = ctype;
			if(g_SkillProperty[id]->detailInfo[slot].effect)
			{
				short effectid = g_SkillProperty[id]->detailInfo[slot].effect[0].id;

				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectid);
				if(iter_table == g_CharacterStateTable.end())
					break;
				
				if(g_CharacterStateTable[effectid]->type == 0)
					InsertBuffStateList(type, id, slot);
				else 
					InsertHostileStateList(type, id, slot);
			}
		}
		break;
	}
}

void _XWindow_State::InsertPlayerStateList(_XStateType type, short id, char ctype, char invennumber, int index)
{
	if(type != _XST_ITEM && type != _XST_SHOUT)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			switch(type)
			{
			case _XST_SHOUT :
			case _XST_ITEM :
				{
					if(currentslot->slot.slot_item.invennumber == invennumber)
					{
						if(currentslot->slot.slot_item.ctype == ctype && currentslot->slot.slot_item.id == id)
						{
							return;
						}
					}
				}
				break;
			}
		}
	} // for

	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;

	newslot->slot.slot_item.ctype = ctype;
	newslot->slot.slot_item.id = id;
	newslot->slot.slot_item.invennumber = invennumber;
	newslot->slot.slot_item.starttime = g_LocalSystemTime;
	newslot->slot.slot_item.bRedicon = false;
	
	// cooldown start time 기록
	g_pLocalUser->m_UserItemList[invennumber].m_dwCooldownStartTime = newslot->slot.slot_item.starttime;

	int imageindex = 0;
	RECT rect;

	if(g_pInventory_Window)
		g_pInventory_Window->GetIconRect(newslot->slot.slot_item.ctype, newslot->slot.slot_item.id, imageindex, rect, TRUE);
	
	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	newslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	newslot->icon->SetScale(0.94f, 0.94f);

	// 치료제 일때 아이템 ID 등록
	if(newslot->slot.slot_item.ctype == _XGI_FC_POTION)
	{
		switch(g_PotionItemProperty[newslot->slot.slot_item.id].cSecondType)
		{
		case _XGI_SC_POTION_HERT_IN :
			{
				// 내상 치료제 사용
				for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
				{
					_XStateSlot* currentslot = *iter_state;
					if(currentslot)
					{
						if(currentslot->type == _XST_INTERNAL)
							currentslot->slot.slot_internal.useitemid = id;
						break;
					}
				}
			}
			break;
		case _XGI_SC_POTION_HERT_OUT :
			{
				// 외상 치료제 사용
				for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
				{
					_XStateSlot* currentslot = *iter_state;
					if(currentslot)
					{
						if(currentslot->type == _XST_EXTERNAL)
							currentslot->slot.slot_internal.useitemid = id;
						break;
					}
				}
			}
			break;
		case _XGI_SC_POTION_FATIGUE :
			break;
		case _XGI_SC_POTION_FOOD :
			break;
		}
	}

	m_PlayerStateList.push_back(newslot);

	// message 처리
	if( index == 1 )
	{
		TCHAR messagestring[256];		
		sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_STATE_2298), _XGameItem::GetItemName( ctype, id ) );
		g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
	}
	else if( index == 2 )	 
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_STATE_2299), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
	}
	else if( index == 3 )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2665), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
	}
}

void _XWindow_State::InsertBuffStateList(_XStateType type, short id, char slotindex, int level)
{
	if(type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE && type != _XST_EXTRASKILL && type != _XST_SPECIALBUFF)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(type == _XST_SKILL)
			{
				if(currentslot->type == type && currentslot->slot.slot_skill.id == id)
				{
					currentslot->slot.slot_skill.level = slotindex;
					currentslot->bSign = false;
					return;
				}
			}
			else if(type == _XST_PASSIVE)
			{
				if(currentslot->type == type && currentslot->slot.slot_skill.id == id)
				{
					currentslot->slot.slot_skill.level = slotindex;
					currentslot->bSign = false;
					return;
				}
			}
			else if( type == _XST_SPECIALBUFF )
			{
				// special buff 일때는 slot index 체크해야 한다. id가 다르면 원래 있던 것에 업데이트 한다.
				if( currentslot->type == type && currentslot->slot.slot_skill.slotindex == slotindex )
				{
					if(currentslot->slot.slot_skill.id == id)
					{
						currentslot->bSign = false;
						return;
					}
					else
					{
						int imageindex = 0;
						RECT rect;

						currentslot->slot.slot_skill.id = id;
						if(g_pQuickSlot_Window)
							g_pQuickSlot_Window->GetIconRect(id, imageindex, rect, true, true);
						currentslot->icon->SetTexture(&g_MainInterfaceTextureArchive, imageindex);
						currentslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
						return;
					}
				}
			}
			else
			{
				if(currentslot->type == type && currentslot->slot.slot_effect.id == id && currentslot->slot.slot_effect.slotindex == slotindex)
				{
					currentslot->bSign = false;
					return;
				}
			}
		}
	} // for
	
	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;

	if(type == _XST_SKILL || type == _XST_PASSIVE || type == _XST_EXTRASKILL)
	{
		newslot->slot.slot_skill.id = id;
		newslot->slot.slot_skill.level = slotindex;
	}
	else if(type == _XST_SPECIALBUFF)
	{
		newslot->slot.slot_skill.id = id;
		newslot->slot.slot_skill.level = level;
		newslot->slot.slot_skill.slotindex = slotindex;
	}
	else
	{
		newslot->slot.slot_effect.id = id;
		newslot->slot.slot_effect.slotindex = slotindex;
	}

	int imageindex = 0;
	RECT rect;
	switch(newslot->type)
	{
	case _XST_PASSIVE :
	case _XST_SKILL :
	case _XST_EXTRASKILL :
	case _XST_SPECIALBUFF :
		{
			if(g_pQuickSlot_Window)
				g_pQuickSlot_Window->GetIconRect(id, imageindex, rect, true, true);
		}
		break;
	case _XST_EFFECT :
		{
			GetIconRect(id, imageindex, rect, true);
		}
		break;
	}

	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	newslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	newslot->icon->SetScale(0.94f, 0.94f);
	
	if( newslot->type == _XST_EFFECT && newslot->slot.slot_effect.id == 347 )	// 복면 상태 일 경우 
		m_BuffStateList.push_front(newslot);
	else
		m_BuffStateList.push_back(newslot);
}

void _XWindow_State::InsertHostileStateList(_XStateType type, short id, char slotindex, int level)
{
	if(type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE && type != _XST_EXTRASKILL && type != _XST_SPECIALBUFF)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(type == _XST_SKILL)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					currentslot->slot.slot_skill.level = slotindex;
					currentslot->bSign = false;
					return;
				}
			}
			else if(type == _XST_PASSIVE)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					currentslot->slot.slot_skill.level = slotindex;
					currentslot->bSign = false;
					return;
				}
			}
			else if( type == _XST_SPECIALBUFF )
			{
				// special buff 일때는 slot index로 구분한다.
				if( currentslot->type == type && currentslot->slot.slot_skill.slotindex == slotindex )
				{
					if(currentslot->slot.slot_skill.id == id)
					{
						currentslot->bSign = false;
						return;
					}
					else
					{
						int imageindex = 0;
						RECT rect;

						currentslot->slot.slot_skill.id = id;
						if(g_pQuickSlot_Window)
							g_pQuickSlot_Window->GetIconRect(id, imageindex, rect, true, true);
						currentslot->icon->SetTexture(&g_MainInterfaceTextureArchive, imageindex);
						currentslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
						return;
					}
				}
			}
			else
			{
				if(currentslot->slot.slot_effect.id == id && currentslot->slot.slot_effect.slotindex == slotindex)
				{
					currentslot->bSign = false;
					return;
				}
			}
		}
	} // for
	
	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;

	if(type == _XST_SKILL || type == _XST_PASSIVE || type == _XST_EXTRASKILL)
	{
		newslot->slot.slot_skill.id = id;
		newslot->slot.slot_skill.level = slotindex;
	}
	else if(type == _XST_SPECIALBUFF)
	{
		newslot->slot.slot_skill.id = id;
		newslot->slot.slot_skill.level = level;
		newslot->slot.slot_skill.slotindex = slotindex;
	}
	else
	{
		newslot->slot.slot_effect.id = id;
		newslot->slot.slot_effect.slotindex = slotindex;
	}

	int imageindex = 0;
	RECT rect;
	switch(newslot->type)
	{
	case _XST_PASSIVE :
	case _XST_SKILL :
	case _XST_EXTRASKILL :
	case _XST_SPECIALBUFF :
		{
			if(g_pQuickSlot_Window)
				g_pQuickSlot_Window->GetIconRect(id, imageindex, rect, true, true);
		}
		break;
	case _XST_EFFECT :
		{
			GetIconRect(id, imageindex, rect, true);
		}
		break;
	}

	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	newslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	newslot->icon->SetScale(0.94f, 0.94f);
	
	m_HostileStateList.push_back(newslot);
}

void _XWindow_State::InsertIntoxication(_XStateType type, short id)
{
	if(type != _XST_INTOXICATION)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if( currentslot->type == type && currentslot->slot.slot_skill.id == id)
				return;
		}
	} // for
	
	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;

	if(type == _XST_INTOXICATION)
	{
		newslot->slot.slot_skill.id = id;
	}
	
	int imageindex = 0;
	RECT rect;
	GetIconRect(id, imageindex, rect, true);

	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	newslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	newslot->icon->SetScale(0.94f, 0.94f);
	
	m_BuffStateList.push_back(newslot);
}

void _XWindow_State::DeleteIntoxication(_XStateType type, short id)
{
	if(type != _XST_INTOXICATION)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{			
			if(currentslot->type == type &&  currentslot->slot.slot_skill.id == id)
			{
				SAFE_DELETE(currentslot->icon);
				SAFE_DELETE(currentslot);
				m_BuffStateList.erase(iter_state);
				return;
			}		
		}
	}
}

void _XWindow_State::InsertNicknameBuff(_XStateType type, short id)
{
	if(type != _XST_NICKBUFF)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if( currentslot->type == type && currentslot->slot.slot_skill.id == id)
				return;
		}
	} // for
	
	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;

	if(type == _XST_NICKBUFF)
	{
		newslot->slot.slot_skill.id = id;
	}
	
	int imageindex = 0;
	RECT rect;
	GetIconRect_NIck(g_NickNameInfoTable[id].sIconNumber, imageindex, rect, true);	

	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	newslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	newslot->icon->SetScale(0.94f, 0.94f);
	
	m_BuffStateList.push_back(newslot);
}

void _XWindow_State::DeleteNicknameBuff(void)
{
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ;)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{			
			if(currentslot->type == _XST_NICKBUFF )
			{
				SAFE_DELETE(currentslot->icon);
				SAFE_DELETE(currentslot);
				iter_state = m_BuffStateList.erase(iter_state);
			}		
			else 
				iter_state++;
		}
	}
}

void _XWindow_State::InsertStudyBookBuff(_XStateType type, short id, int lefttime )
{
	if(type != _XST_STUDYBOOK)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if( currentslot->type == type && currentslot->slot.slot_studybook.id == id)
				return;
		}
	} // for
	
	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;

	if(type == _XST_STUDYBOOK)
	{
		newslot->slot.slot_studybook.ctype = _XGI_FC_BOOK;
		newslot->slot.slot_studybook.id = id;
		newslot->slot.slot_studybook.starttime = g_LocalSystemTime;
		newslot->slot.slot_studybook.lefttime = lefttime;	// 인벤넘버에 남은 시간(초)를 저장하여 씀.
	}
	
	int imageindex = 0;
	RECT rect;
	if(g_pInventory_Window)
		g_pInventory_Window->GetIconRect(newslot->slot.slot_studybook.ctype, newslot->slot.slot_studybook.id, imageindex, rect, TRUE);

	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	newslot->icon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	newslot->icon->SetScale(0.94f, 0.94f);
	
	m_BuffStateList.push_back(newslot);
}

void _XWindow_State::DeleteStudyBookBuff(_XStateType type, short id)
{
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ;)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{			
			if(currentslot->type == _XST_STUDYBOOK )
			{
				SAFE_DELETE(currentslot->icon);
				SAFE_DELETE(currentslot);
				iter_state = m_BuffStateList.erase(iter_state);
			}		
			else 
				iter_state++;
		}
	}
}

void _XWindow_State::UpdatePlayerStateList(_XStateType type, short beforevalue, short aftervalue)
{
	if(type != _XST_EXTERNAL && type != _XST_INTERNAL)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == type)
			{
				if(currentslot->type == _XST_EXTERNAL)
				{
					currentslot->slot.slot_external.beforeexternal = beforevalue;
					currentslot->slot.slot_external.afterexternal = aftervalue;
					return;
				}
				else
				{
					currentslot->slot.slot_internal.beforeinternal = beforevalue;
					currentslot->slot.slot_internal.afterinternal = aftervalue;
					return;
				}
			}
		}
	}

	if(iter_state == m_PlayerStateList.end())
	{
		int maxlife = g_pLocalUser->m_MaxLifePower.Get_result();
		if(maxlife == 0)
			maxlife = g_pLocalUser->m_CharacterInfo.Get_max_lifepower();

		FLOAT percentage = (((FLOAT)aftervalue)/(FLOAT)maxlife)*100.0f;
		if(percentage >= 40.0f)
			InsertStateListFromExIn(_XST_EXTERNAL, beforevalue, aftervalue);
	}
}


void _XWindow_State::DeleteStateList(_XStateType type, short id, char ctype, char invennumber)
{
	switch(type)
	{
	case _XST_EXTERNAL :
	case _XST_INTERNAL :
	case _XST_ITEM :
	case _XST_SHOUT :
		DeletePlayerStateList(type, id, ctype, invennumber);
		break;
		
	case _XST_SKILL :
	case _XST_EFFECT :
	case _XST_PASSIVE :
	case _XST_EXTRASKILL :
	case _XST_SPECIALBUFF :
		{
			if(!DeleteBuffStateList(type, id, ctype))
				DeleteHostileStateList(type, id, ctype);
		}
		break;
	}
}

void _XWindow_State::DeletePlayerStateList(_XStateType type, short id, char ctype, char invennumber)
{
	if(type != _XST_EXTERNAL && type != _XST_INTERNAL && type != _XST_ITEM && type != _XST_SHOUT)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			switch(type)
			{
			case _XST_EXTERNAL :
			case _XST_INTERNAL :
				{
					if(currentslot->type == type)
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_PlayerStateList.erase(iter_state);
						return;
					}
				}
				break;
			case _XST_ITEM :
				{
					if(currentslot->slot.slot_item.ctype == ctype && currentslot->slot.slot_item.id == id 
						&& currentslot->slot.slot_item.invennumber == invennumber)
					{
						g_pLocalUser->m_UserItemList[invennumber].m_dwCooldownStartTime = 0;

						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_PlayerStateList.erase(iter_state);
						return;
					}
				}
				break;
			case _XST_SHOUT :
				{
					if(currentslot->type == type)
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_PlayerStateList.erase(iter_state);
						return;
					}
				}
				break;
			}
		}
	}
}

BOOL _XWindow_State::DeleteBuffStateList(_XStateType type, short id, char slotindex)
{
	if(type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE && type != _XST_EXTRASKILL && type != _XST_SPECIALBUFF)
		return TRUE;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(type == _XST_SKILL)
			{
				if( currentslot->type == type && currentslot->slot.slot_skill.id == id)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_BuffStateList.erase(iter_state);
					return TRUE;
				}
			}
			else if(type == _XST_PASSIVE)
			{
				if(currentslot->type == type && currentslot->slot.slot_skill.id == id)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_BuffStateList.erase(iter_state);
					return TRUE;
				}
			}
			else if(type == _XST_EXTRASKILL)
			{
				if( currentslot->type == type && currentslot->slot.slot_skill.id == id && currentslot->slot.slot_skill.level == slotindex)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_BuffStateList.erase(iter_state);
					return TRUE;
				}
			}
			else if(type == _XST_SPECIALBUFF)
			{
				// special buff 일때는 slot index로 구분한다.
				if( currentslot->type == type && currentslot->slot.slot_skill.slotindex == slotindex)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_BuffStateList.erase(iter_state);
					return TRUE;
				}
			}
			else
			{
				if(slotindex >= 0)
				{
					if(currentslot->type == type && currentslot->slot.slot_effect.slotindex == slotindex 
						&& currentslot->slot.slot_effect.id == id)
					{
						_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
						if( pMessageBox )
							g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_BuffStateList.erase(iter_state);
						return TRUE;
					}	
				}
				else
				{
					if(currentslot->type == type && currentslot->slot.slot_effect.slotindex == slotindex 
						&& currentslot->slot.slot_effect.id == id)
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_BuffStateList.erase(iter_state);
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL _XWindow_State::DeleteHostileStateList(_XStateType type, short id, char slotindex)
{
	if(type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE && type != _XST_EXTRASKILL && type != _XST_SPECIALBUFF)
		return TRUE;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(type == _XST_SKILL)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_HostileStateList.erase(iter_state);
					return TRUE;
				}
			}
			else if(type == _XST_PASSIVE)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_HostileStateList.erase(iter_state);
					return TRUE;
				}
			}
			else if(type == _XST_EXTRASKILL)
			{
				if(currentslot->slot.slot_skill.id == id && currentslot->slot.slot_skill.level == slotindex)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_HostileStateList.erase(iter_state);
					return TRUE;
				}
			}
			else if(type == _XST_SPECIALBUFF)
			{
				// special buff 일때는 slot index로 구분한다.
				if(currentslot->type == type && currentslot->slot.slot_skill.slotindex == slotindex)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					m_HostileStateList.erase(iter_state);
					return TRUE;
				}
			}
			else
			{
				if(slotindex >= 0)
				{
					if(currentslot->slot.slot_effect.slotindex == slotindex)
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_HostileStateList.erase(iter_state);
						return TRUE;
					}
				}
				else
				{
					if(currentslot->slot.slot_effect.id == id)
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_HostileStateList.erase(iter_state);
						return TRUE;
					}
				}
			}
		}
	}
	
	return FALSE;
}

void _XWindow_State::DeleteElixirState(_XStateType type, char ctype)
{
	if( type != _XST_ITEM)
		return;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			switch(type)
			{			
			case _XST_ITEM :
				{
					if(currentslot->slot.slot_item.ctype == ctype )
					{
						g_pLocalUser->m_UserItemList[currentslot->slot.slot_item.invennumber].m_dwCooldownStartTime = 0;

						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						m_PlayerStateList.erase(iter_state);
						return;
					}
				}
				break;
			}
		}
	}
}

BOOL _XWindow_State::FindPlayerStateList(_XStateType type, short id, char ctype, char invennumber, int index)
{
	if(type != _XST_ITEM && type != _XST_SHOUT)
		return FALSE;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			switch(type)
			{
			case _XST_ITEM :
				{
					if(currentslot->slot.slot_item.invennumber == invennumber)
					{
						if(currentslot->slot.slot_item.ctype == ctype && currentslot->slot.slot_item.id == id)
						{
							return TRUE;
						}
					}
				}
				break;
			case _XST_SHOUT :
				{
					// 있으면 무조건 TRUE
					return TRUE;
				}
				break;
			}
		}
	} // for

	return FALSE;
}

BOOL _XWindow_State::UpdatePlayerStateList(_XStateType type, short id, char ctype, char oldinvennumber, char newinvennumber)
{
	if(type != _XST_ITEM )
		return FALSE;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			switch(type)
			{
			case _XST_ITEM :
				{
					if(currentslot->slot.slot_item.invennumber == oldinvennumber)
					{
						if(currentslot->slot.slot_item.ctype == ctype && currentslot->slot.slot_item.id == id)
						{
							currentslot->slot.slot_item.invennumber = newinvennumber;
							return TRUE;
						}
					}
				}
				break;
			}
		}
	} // for
	
	return FALSE;
}


void _XWindow_State::DrawStateList(void)
{
	int offsetx = 0, offsety = 0;
	list <_XStateSlot*>::iterator iter_state;
	int count = 0;

	int listcount1 = m_PlayerStateList.size();
	int listcount2 = m_BuffStateList.size();
	int listcount3 = m_HostileStateList.size();

	if(!m_PlayerStateList.empty())
	{
		offsetx = 5;
		offsety = 0;
		for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				currentslot->icon->MoveWindow(m_WindowPosition.x+offsetx+1+29*(count%6), m_WindowPosition.y+offsety+1+29*(count/6));

				int show = 0;
				if(currentslot->bSign)
				{
					show = (int)((g_LocalSystemTime - currentslot->showtime) / 250.0f);
				}
				bool bDraw = true;
				
				if(currentslot->bSign)
				{
					if(show % 2 == 1)
						bDraw = false;
				}

				if(bDraw)
				{
					currentslot->icon->Draw();
					_XDrawRectAngle(m_WindowPosition.x+offsetx+28*(count%6)+1*(count%6), m_WindowPosition.y+offsety+28*(count/6)+1*(count/6),
						m_WindowPosition.x+offsetx+28*(count%6)+28+1*(count%6)+1, m_WindowPosition.y+offsety+28*(count/6)+28+1*(count/6)+1, 0.0f, D3DCOLOR_ARGB(255, 158, 155, 148));

					if(currentslot->slot.slot_item.bRedicon)
					{
						_XDrawSolidBar(m_WindowPosition.x+offsetx+1+29*(count%6), m_WindowPosition.y+offsety+1+29*(count/6), 
							m_WindowPosition.x+offsetx+1+29*(count%6)+28, m_WindowPosition.y+offsety+1+29*(count/6)+28, D3DCOLOR_ARGB(60,237,27,35));
					}
				}
			}
			count++;
		}

		int line = (int)(listcount1/7);
		if(line >= 0)
			m_BarImage->SetScale(1.0f, 1.0f*(line+1));
		m_BarImage->Draw(m_WindowPosition.x, m_WindowPosition.y);
	}
	if(!m_BuffStateList.empty())
	{
		offsetx = 5;
		offsety = 0 + 28*((listcount1+5)/6);

		if(listcount1 >0)
			offsety += 3;

		count = 0;
		for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				currentslot->icon->MoveWindow(m_WindowPosition.x+offsetx+1+29*(count%6), m_WindowPosition.y+offsety+1+29*(count/6));

				int show = (int)((g_LocalSystemTime - currentslot->showtime) / 250.0f);	
				bool bDraw = true;	
				
				if(currentslot->bSign)
				{
					if(show % 2 == 1)
						bDraw = false;
				}
				
				if(bDraw)
				{
					currentslot->icon->Draw();
					_XDrawRectAngle(m_WindowPosition.x+offsetx+28*(count%6)+1*(count%6), m_WindowPosition.y+offsety+28*(count/6)+1*(count/6),
						m_WindowPosition.x+offsetx+28*(count%6)+28+1*(count%6)+1, m_WindowPosition.y+offsety+28*(count/6)+28+1*(count/6)+1, 0.0f, D3DCOLOR_ARGB(255, 87, 137, 188));
				}
			}
			count++;
		}

		int line = (int)(listcount2/7);
		if(line >= 0)
			m_BarImage->SetScale(1.0f, 1.0f*(line+1));
		m_BarImage->Draw(m_WindowPosition.x, m_WindowPosition.y+offsety);
	}
	if(!m_HostileStateList.empty())
	{
		offsetx = 5;
		offsety = 0 + 28*((listcount1+5)/6) + 0 + 28*((listcount2+5)/6);

		if(listcount1 >0)
			offsety += 3;
		if(listcount2 > 0)
			offsety += 3;

		count = 0;
		for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				currentslot->icon->MoveWindow(m_WindowPosition.x+offsetx+1+29*(count%6), m_WindowPosition.y+offsety+1+29*(count/6));

				int show = (int)((g_LocalSystemTime - currentslot->showtime) / 250.0f);
				bool bDraw = true;
				
				if(currentslot->bSign)
				{
					if(show % 2 == 1)
						bDraw = false;
				}
				
				if(bDraw)
				{
					currentslot->icon->Draw();
					_XDrawRectAngle(m_WindowPosition.x+offsetx+28*(count%6)+1*(count%6), m_WindowPosition.y+offsety+28*(count/6)+1*(count/6),
						m_WindowPosition.x+offsetx+28*(count%6)+28+1*(count%6)+1, m_WindowPosition.y+offsety+28*(count/6)+28+1*(count/6)+1, 0.0f, D3DCOLOR_ARGB(255, 216, 97, 101));
				}
			}
			count++;
		}

		int line = (int)(listcount3/7);
		if(line >= 0)
			m_BarImage->SetScale(1.0f, 1.0f*(line+1));
		m_BarImage->Draw(m_WindowPosition.x, m_WindowPosition.y+offsety);
	}
}

void _XWindow_State::ShowWindow(BOOL show)
{
	if(!show)
		m_PreProcessTime = 0;

	_XWindow::ShowWindow(show);
}

void _XWindow_State::DrawStateListTooltip(void)
{
	bool flag = false;	

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->icon->CheckMousePosition() && g_pMiniTooltip_Window )
			{
				flag = true;
				POINT pos = currentslot->icon->GetWindowPos();
				if(currentslot->type == _XST_EXTERNAL)
				{					
					if(currentslot->slot.slot_external.beforeexternal > currentslot->slot.slot_external.afterexternal)
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_EXTERNAL, 0, 0, currentslot->slot.slot_external.beforeexternal, 0, pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					else
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_EXTERNAL, 0, 0, currentslot->slot.slot_external.afterexternal, 0, pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					
					return;
				}
				else if(currentslot->type == _XST_INTERNAL)
				{
					if(currentslot->slot.slot_internal.beforeinternal > currentslot->slot.slot_internal.afterinternal)
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_INTERNAL, 0, 0, currentslot->slot.slot_internal.beforeinternal, 0, pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					else
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_INTERNAL, 0, 0, currentslot->slot.slot_internal.afterinternal, 0, pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
										
					return;
				}
				else if(currentslot->type == _XST_ITEM)
				{
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_ITEM, currentslot->slot.slot_item.invennumber, 
									currentslot->slot.slot_item.ctype, currentslot->slot.slot_item.id, (g_LocalSystemTime - currentslot->slot.slot_item.starttime), pos.x, pos.y ) )
									g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
				else if(currentslot->type == _XST_SHOUT)
				{
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_ITEM_SHOUT, currentslot->slot.slot_item.invennumber, 
						currentslot->slot.slot_item.ctype, currentslot->slot.slot_item.id, g_pLocalUser->GetShoutLeftTime(), pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
			} // if(currentslot->icon->CheckMousePosition())
		}
	} // for

	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->icon->CheckMousePosition())
			{
				flag = true;
				POINT pos = currentslot->icon->GetWindowPos();
				if(currentslot->type == _XST_SKILL)
				{
//					int index = g_pQuickSlot_Window->GetSelectedSet()*10 + g_pQuickSlot_Window->GetLevelButtonIndex();
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[currentslot->slot.slot_skill.level].effect[0].id;

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, currentslot->slot.slot_skill.level, 0, currentslot->slot.slot_skill.id, 
						g_pLocalUser->GetLeftTimeFromStateList(en_from_skill, effectid, currentslot->slot.slot_skill.id), pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );					
					return;
				}
				else if(currentslot->type == _XST_EFFECT)
				{
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentslot->slot.slot_effect.id);
					if(iter_table == g_CharacterStateTable.end())
						break;

					if(currentslot->slot.slot_effect.slotindex >= 0)
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_EFFECT, 1, currentslot->slot.slot_effect.slotindex, currentslot->slot.slot_effect.id, 
							g_pLocalUser->GetLeftTimeFromStateList(en_from_extra, currentslot->slot.slot_effect.id, 0, currentslot->slot.slot_effect.slotindex), pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					else
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_EFFECT, 0, 0, currentslot->slot.slot_effect.id, 
							g_pLocalUser->GetLeftTimeFromStateList(en_from_effect, currentslot->slot.slot_effect.id), pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					return;
				}
				else if(currentslot->type == _XST_PASSIVE)
				{
					// 남은 시간 디스플레이 하지 않도록
					int index = g_pQuickSlot_Window->GetSelectedSet()*10 + g_pQuickSlot_Window->GetLevelButtonIndex();
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[currentslot->slot.slot_skill.level].effect[0].id;

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, currentslot->slot.slot_skill.level, 0, currentslot->slot.slot_skill.id, 0, pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );					
					return;
				}
				else if( currentslot->type == _XST_INTOXICATION)
				{
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_INTOXICATION, 0, 0, currentslot->slot.slot_skill.id, 0, pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
				else if( currentslot->type == _XST_NICKBUFF )
				{
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_NICKNAME, 0, 0, currentslot->slot.slot_effect.id, 0, pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
				else if( currentslot->type == _XST_STUDYBOOK )
				{
					int lefttime = currentslot->slot.slot_studybook.lefttime - (g_LocalSystemTime - currentslot->slot.slot_studybook.starttime)/1000;
					if( lefttime <= 0 )
						lefttime = 0;
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_STUDYBOOK, 0, _XGI_FC_BOOK, currentslot->slot.slot_item.id, lefttime, pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;					
				}
				else if(currentslot->type == _XST_EXTRASKILL)
				{
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, 0, 0, currentslot->slot.slot_skill.id, 
						g_pLocalUser->GetLeftTimeFromStateList(en_from_extraskill, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.level), 
						pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );					
					return;
				}
				else if(currentslot->type == _XST_SPECIALBUFF)
				{//Author : 양희왕 //breif : 추가
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, currentslot->slot.slot_skill.level, 0, currentslot->slot.slot_skill.id, 
						g_pLocalUser->GetLeftTimeFromStateList(en_from_specialbuff, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.slotindex), pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );					
					return;
				}
			} // if(currentslot->icon->CheckMousePosition())
		}
	} // for

	for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->icon->CheckMousePosition())
			{
				flag = true;
				POINT pos = currentslot->icon->GetWindowPos();
				if(currentslot->type == _XST_SKILL)
				{
					int index = g_pQuickSlot_Window->GetSelectedSet()*10 + g_pQuickSlot_Window->GetLevelButtonIndex();
#ifdef _XTESTSERVER
					//int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[currentslot->slot.slot_skill.level].effect[0].id;
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_pQuickSlot_Window->m_QuickSlot[index].skillid);					
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effect[0].id;
#else
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_pQuickSlot_Window->m_QuickSlot[index].skillid);					
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effect[0].id;
#endif

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, currentslot->slot.slot_skill.level, 0, currentslot->slot.slot_skill.id, 
						g_pLocalUser->GetLeftTimeFromStateList(en_from_skill, effectid, currentslot->slot.slot_skill.id), pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
				else if(currentslot->type == _XST_EFFECT)
				{
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentslot->slot.slot_effect.id);
					if(iter_table == g_CharacterStateTable.end())
						break;
					
					if(currentslot->slot.slot_effect.slotindex >= 0)
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_EFFECT, 1, currentslot->slot.slot_effect.slotindex, currentslot->slot.slot_effect.id, 
							g_pLocalUser->GetLeftTimeFromStateList(en_from_extra, currentslot->slot.slot_effect.id, 0, currentslot->slot.slot_effect.slotindex), pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					else
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_EFFECT, 0, 0, currentslot->slot.slot_effect.id, 
							g_pLocalUser->GetLeftTimeFromStateList(en_from_effect, currentslot->slot.slot_effect.id), pos.x, pos.y ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					return;
				}
				else if(currentslot->type == _XST_PASSIVE)
				{
					// 남은 시간 디스플레이 하지 않음
					int index = g_pQuickSlot_Window->GetSelectedSet()*10 + g_pQuickSlot_Window->GetLevelButtonIndex();
#ifdef _XTESTSERVER
					//int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[currentslot->slot.slot_skill.level].effect[0].id;
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_pQuickSlot_Window->m_QuickSlot[index].skillid);					
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effect[0].id;
#else
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_pQuickSlot_Window->m_QuickSlot[index].skillid);					
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effect[0].id;
#endif

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, currentslot->slot.slot_skill.level, 0, currentslot->slot.slot_skill.id, 0, pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
				else if( currentslot->type == _XST_PEMODE )
				{
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_PEMODE, 0, 0, 1, g_pLocalUser->GetPEModeLeftTime(), pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
				}
				else if(currentslot->type == _XST_EXTRASKILL)
				{
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, 0, 0, currentslot->slot.slot_skill.id, 
						g_pLocalUser->GetLeftTimeFromStateList(en_from_extraskill, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.level), 
						pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
				else if(currentslot->type == _XST_SPECIALBUFF)
				{
					// Todo
					int effectid = g_SkillProperty[currentslot->slot.slot_skill.id]->detailInfo[0].effect[0].id;

					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_BUFF_SKILL, currentslot->slot.slot_skill.level, 0, currentslot->slot.slot_skill.id, 
						g_pLocalUser->GetLeftTimeFromStateList(en_from_specialbuff, effectid, currentslot->slot.slot_skill.id, currentslot->slot.slot_skill.slotindex), pos.x, pos.y ) )
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					return;
				}
			} // if(currentslot->icon->CheckMousePosition())
		}
	} // for

	if( flag == false )
	{
		if( g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_EXTERNAL || 
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_INTERNAL ||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_ITEM		||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_SKILL	||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_EFFECT	||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_INTOXICATION ||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_NICKNAME ||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_STUDYBOOK ||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_BUFF_PEMODE ||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_ITEM_SHOUT )
			g_pMiniTooltip_Window->ShowWindow( FALSE );
	}
}

BOOL _XWindow_State::FindUsingItemFromID(_XStateType type, short id, char ctype)
{
	if(type != _XST_ITEM)
		return FALSE;

	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == type)
			{
				if(currentslot->slot.slot_item.ctype == ctype && currentslot->slot.slot_item.id == id)
					return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_State::FindUsingItemFromSecondType(_XStateType type, char ctype, char secondtype)
{
	if(type != _XST_ITEM && type != _XST_SHOUT)
		return FALSE;

	list <_XStateSlot*>::iterator iter_state;

	if(ctype == _XGI_FC_POTION)
	{
		DWORD dwLastUseTime = 0;
		int cooldown = 0;

		for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == type)
				{
#ifdef _XDEF_EXPANSION_POTION_070709_KUKURI
					if(g_pLocalUser->m_UserItemList[currentslot->slot.slot_item.invennumber].m_dwCooldownStartTime > dwLastUseTime)
					{
						dwLastUseTime = g_pLocalUser->m_UserItemList[currentslot->slot.slot_item.invennumber].m_dwCooldownStartTime;
					}

					if(secondtype == _XGI_SC_POTION_COOLDOWN)
						cooldown = g_PotionItemProperty[currentslot->slot.slot_item.id].sCooldown*1000;	//table 값 참조
					else
						cooldown = 3000;	// 일괄 3초 적용
#else
					if(currentslot->slot.slot_item.ctype == ctype && g_PotionItemProperty[currentslot->slot.slot_item.id].cSecondType == secondtype)
					{
						DWORD elapsedtime = 0;
						if(g_LocalSystemTime >= g_pLocalUser->m_UserItemList[currentslot->slot.slot_item.invennumber].m_dwCooldownStartTime)
						{
							elapsedtime = g_LocalSystemTime - g_pLocalUser->m_UserItemList[currentslot->slot.slot_item.invennumber].m_dwCooldownStartTime;
						}

						if(elapsedtime >= (g_PotionItemProperty[currentslot->slot.slot_item.id].sCooldown*1000))	// cooldown 체크
							return FALSE;
						else 
							return TRUE;
					}
#endif
				}
			}
		}

#ifdef _XDEF_EXPANSION_POTION_070709_KUKURI
		if(g_LocalSystemTime - dwLastUseTime < cooldown)	// 사용 후 cooldown 경과하지 않음
			return TRUE;
#else
#endif
		return FALSE;
	}

	if(ctype == _XGI_FC_BOOK)
	{
		for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == type)
				{
					if(currentslot->slot.slot_item.ctype == ctype && g_BookItemProperty[currentslot->slot.slot_item.id].cSecondType == secondtype)
						return TRUE;
				}
			}
		}
		return FALSE;
	}

	if(ctype == _XGI_FC_LIFE)
	{
		for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == type)
				{
					if(currentslot->slot.slot_item.ctype == ctype && g_LifeItemProperty[currentslot->slot.slot_item.id].cSecondType == secondtype)
						return TRUE;
				}
			}
		}
		return FALSE;
	}
	
	if( ctype == _XGI_FC_ELIXIR )
	{
		for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == type)
				{
					if(currentslot->slot.slot_item.ctype == ctype && g_ElixirItemProperty[currentslot->slot.slot_item.id].cSecondType == secondtype)
						return TRUE;
				}
			}
		}
		return FALSE;
	}

	return FALSE;
}

void _XWindow_State::DeleteAllStateList(void)
{
	list <_XStateSlot*>::iterator iter_state;
	
	if( !m_PlayerStateList.empty() )
	{
		for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				SAFE_DELETE(currentslot->icon);
				SAFE_DELETE(currentslot);
			} // if(currentslot)
		} // for
	
		m_PlayerStateList.clear();
	}
	
	if( !m_BuffStateList.empty() )
	{
		for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				SAFE_DELETE(currentslot->icon);
				SAFE_DELETE(currentslot);
			} // if(currentslot)
		} // for
		
		m_BuffStateList.clear();
	}
		
	if( !m_HostileStateList.empty() )
	{
		for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				SAFE_DELETE(currentslot->icon);
				SAFE_DELETE(currentslot);
			} // if(currentslot)
		} // for
		
		m_HostileStateList.clear();
	}
}

void _XWindow_State::DeleteAllBuffStateList(void)
{
	if( !m_BuffStateList.empty() )
	{
		list <_XStateSlot*>::iterator iter_state;

		for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; )
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == _XST_SKILL)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					iter_state = m_BuffStateList.erase(iter_state);
				}
				else
				{
					iter_state++;
				}
			} // if(currentslot)
			else
			{
				iter_state++;
			}
		} // for
	}
}

void _XWindow_State::DeleteAllHostileStateList(void)
{
	if( !m_HostileStateList.empty() )
	{
		list <_XStateSlot*>::iterator iter_state;
		for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; )
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == _XST_SKILL)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					iter_state = m_HostileStateList.erase(iter_state);
				}
				else
				{
					iter_state++;
				}
			} // if(currentslot)
			else
			{
				iter_state++;
			}
		} // for
	}
}

void _XWindow_State::DeleteAllSkill(void)
{
	list <_XStateSlot*>::iterator iter_state;

	if( !m_BuffStateList.empty() )
	{
		for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; )
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == _XST_SKILL)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					iter_state = m_BuffStateList.erase(iter_state);
				}
				else
				{
					iter_state++;
				}
			}
			else
			{
				iter_state++;
			}
		}
	}

	if( !m_HostileStateList.empty() )
	{
		for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; )
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == _XST_SKILL)
				{
					SAFE_DELETE(currentslot->icon);
					SAFE_DELETE(currentslot);
					iter_state = m_HostileStateList.erase(iter_state);
				}
				else
				{
					iter_state++;
				}
			}
			else
			{
				iter_state++;
			}
		}
	}
}

void _XWindow_State::DeleteAllEffect(void)
{
	list <_XStateSlot*>::iterator iter_state;

	if( !m_BuffStateList.empty() )
	{
		for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; )
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == _XST_EFFECT)
				{
					if(currentslot->slot.slot_effect.slotindex < 0)		// extra를 제외한 effect
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						iter_state = m_BuffStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else
				{
					iter_state++;
				}
			}
			else
			{
				iter_state++;
			}
		}
	}
	
	if( !m_HostileStateList.empty() )
	{
		for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; )
		{
			_XStateSlot* currentslot = *iter_state;
			if(currentslot)
			{
				if(currentslot->type == _XST_EFFECT)
				{
					if(currentslot->slot.slot_effect.slotindex < 0)		// extra를 제외한 effect
					{
						SAFE_DELETE(currentslot->icon);
						SAFE_DELETE(currentslot);
						iter_state = m_HostileStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else
				{
					iter_state++;
				}
			}
			else
			{
				iter_state++;
			}
		}
	}
}

BOOL _XWindow_State::GetIconRect(short effectid, int& resourceindex, RECT& rect, bool smallicon)
{
	map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectid);
	if(iter_table == g_CharacterStateTable.end())
		return TRUE;

	_XCharacterStateTable* currenttable = iter_table->second;
	if(currenttable)
	{
		int iconsubid = currenttable->iconNumber;
	
		int subindex = ((iconsubid-1) / 64) + 1;
		int iconid = (iconsubid-1) % 64;
		
		if(iconid < 0)
			return FALSE;

		rect.left = (iconid % 8) * 32;
		rect.right = (iconid % 8 + 1) * 32;
		rect.top = (iconid / 8) * 32;
		rect.bottom = (iconid / 8 + 1) *32;

		if(smallicon)
		{
			rect.left  += 1;
			rect.top   += 1;
			rect.right -= 1;
			rect.bottom-= 1;
		}


		TCHAR resourcename[128];
		sprintf(resourcename, "Condition_icon_%02d.tga", subindex);
		resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
	}
	
	return TRUE;
}

BOOL _XWindow_State::GetIconRect_NIck(short effectid, int& resourceindex, RECT& rect, bool smallicon)
{
	if( effectid < 0 )
		return FALSE;

	short sIconNo = effectid-1;
	int iconclipcolumn = 0, iconcliprow = 0;

	if( sIconNo < 64 )
	{
		iconclipcolumn = sIconNo % 8;
		iconcliprow    = sIconNo / 8;

		resourceindex = g_MainInterfaceTextureArchive.FindResource("Nickname_Icon_01.tga");
	}

	if(sIconNo != -1 && resourceindex > -1)
	{
		rect.left    = iconclipcolumn * 32;
		rect.right   = rect.left + 32;
		
		rect.top     = iconcliprow	   * 32;
		rect.bottom  = rect.top  + 32;		
					
		if(smallicon)
		{
			rect.left  += 1;
			rect.top   += 1;
			rect.right -= 1;
			rect.bottom-= 1;
		}
	}
	else
	{
		// icon 없는 아이템 처리
		rect.left    = 128;
		rect.top	 = 1;
		
		rect.right   = 160;
		rect.bottom  = 31;
		
		resourceindex = g_MainInterfaceTextureArchive.FindResource("item_pot_w1_001.tga");
	}
	
	return TRUE;
}

void _XWindow_State::SetRedLayer(_XStateType type, short id, char ctype, char invennumber)
{
	if(type != _XST_ITEM)
		return;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == type)
			{
				if(invennumber != 0 && currentslot->slot.slot_item.invennumber == invennumber)
				{
					if(currentslot->slot.slot_item.ctype == ctype && currentslot->slot.slot_item.id == id)
					{
						currentslot->slot.slot_item.bRedicon = true;
						return;
					}
				}
			}
		}
	}
}

void _XWindow_State::SetIconWarning(_XStateType type, short id, char ctype)
{	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(currentslot->type == type)
			{
				if(currentslot->slot.slot_item.ctype == ctype && currentslot->slot.slot_item.id == id)
				{
					currentslot->bSign = true;
					currentslot->showtime = g_LocalSystemTime;
					return;
				}
			}
		}
	}
}

void _XWindow_State::ClickSkillAction(void)
{
	g_NetworkKernel.SendPacket(MSG_NO_SKILL_ACTION, m_BuffCancleMode, m_BuffCancleindex);
}

void _XWindow_State::PrintSystemMessage(int index)
{
	if( index < 0 ) return;

	TCHAR tempString[128];
	memset( tempString, 0, sizeof(TCHAR)*128);

	switch(index) 
	{	
	case 0 :	// 내력 부족 
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NETWORKBATTLE_1616) );	// "내력이 부족합니다."
		break;
	case 1 :	// 복면 없이 복면 해제
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2809) );	//"현재 복면 중이 아닙니다."
		break;
	case 2 :	// 비복면인에 복면 해제
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2810) );	//"복면을 하지 않은 상대입니다."
		break;
	case 3 :	// 타인복면해제 실패
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2811) );//"복면 해제에 실패하였습니다."
		break;
	case 4 :	// Non Player 복면 해제	
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1577) );//"잘못된 대상입니다."
		break;						// --- system message	
	case 5 :	// 비무 중 복면 사용
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2812) );//"비무 중에는 복면을 사용할 수 없습니다."
		break;
	case 6 :	// 복면 중 비무 신청
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2813) );//"복면 중에는 비무를 신청할 수 없습니다."
		break;	
	case 7 :	// 복면인에 비무 신청
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2814) );//"복면인에게는 비무를 신청할 수 없습니다."
		break;
	case 8 :	// 복면 중 노점 시도
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2815) );//"복면 중에는 노점을 열 수 없습니다."
		break;
	case 9 :	// 복면 중 PvP 거래 시도
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2816) );//"복면 중에는 교환을 신청할 수 없습니다."
		break;
	case 10 :	// 복면 중 PvP 거래 시도
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2817) );//"복면인에게는 교환을 신청할 수 없습니다."
		break;
	case 11 :	// 순차적이 아닌 무공서 습득
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2818));//"현재의 숙련도에서 읽을 수 없는 단계의 무공서입니다." 
		break;	
	case 12 :
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2819) );//"복면인에게는 친구 신청을 할 수 없습니다."
		break;
	case 13 :
		strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_2820) );//"문파공적 상태에서는 복면을 사용할 수 없습니다."
		break;
	default :
		{
			strcpy( tempString, _XGETINTERFACETEXT(ID_STRING_NETWORK_1595) );//"알 수 없는 오류"
		}
		break;
	}

	if( index < 5 )
	{
		g_NetworkKernel.InsertChatString(tempString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
	else
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, tempString, TRUE);
		pMessageBox->StartTimer(5000, TRUE);
	}
}

void _XWindow_State::InsertPEModeIcon(_XStateType type, short id)
{
	if(type != _XST_PEMODE)
		return;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if( currentslot->type == type)
				return;
		}
	} // for
	
	_XStateSlot* newslot = new _XStateSlot;
	newslot->type = type;
	
	newslot->icon = new _XImageStatic;
	newslot->icon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Condition_icon_02.tga"));
	newslot->icon->SetClipRect(161, 225, 191, 255);
	newslot->icon->SetScale(0.94f, 0.94f);
	
	m_HostileStateList.push_back(newslot);
}

void _XWindow_State::DeletePEModeIcon(void)
{
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ;)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{			
			if(currentslot->type == _XST_PEMODE )
			{
				SAFE_DELETE(currentslot->icon);
				SAFE_DELETE(currentslot);
				iter_state = m_HostileStateList.erase(iter_state);
			}		
			else 
				iter_state++;
		}
	}
}

BOOL _XWindow_State::FindPlayerStateList(_XStateType type, short id)
{
	if(type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE)
		return FALSE;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_PlayerStateList.begin() ; iter_state != m_PlayerStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(type == _XST_SKILL)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					return TRUE;
				}
			}
			else if(type == _XST_PASSIVE)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					return TRUE;
				}
			}
			else if(type == _XST_EFFECT)
			{
				if(currentslot->slot.slot_effect.id == id )
				{
					return TRUE;
				}
			}
		}
	} // for
	
	return FALSE;
}
BOOL _XWindow_State::FindBuffStateList(_XStateType type, short id)
{
	if(type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE && type != _XST_EXTRASKILL && type != _XST_SPECIALBUFF)
		return FALSE;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_BuffStateList.begin() ; iter_state != m_BuffStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(type == _XST_SKILL || type == _XST_EXTRASKILL || type == _XST_SPECIALBUFF)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					return TRUE;
				}
			}
			else if(type == _XST_PASSIVE)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					return TRUE;
				}
			}
			else if(type == _XST_EFFECT)
			{
				if(currentslot->slot.slot_effect.id == id )
				{
					return TRUE;
				}
			}
		}
	} // for
	
	return FALSE;
}
BOOL _XWindow_State::FindHostileStateList(_XStateType type, short id)
{
	if(type != _XST_SKILL && type != _XST_EFFECT && type != _XST_PASSIVE && type != _XST_EXTRASKILL && type != _XST_SPECIALBUFF)
		return FALSE;
	
	list <_XStateSlot*>::iterator iter_state;
	for(iter_state = m_HostileStateList.begin() ; iter_state != m_HostileStateList.end() ; iter_state++)
	{
		_XStateSlot* currentslot = *iter_state;
		if(currentslot)
		{
			if(type == _XST_SKILL || type == _XST_EXTRASKILL || type == _XST_SPECIALBUFF)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					return TRUE;
				}
			}
			else if(type == _XST_PASSIVE)
			{
				if(currentslot->slot.slot_skill.id == id)
				{
					return TRUE;
				}
			}
			else if(type == _XST_EFFECT)
			{
				if(currentslot->slot.slot_effect.id == id )
				{
					return TRUE;
				}
			}
		}
	} // for
	
	return FALSE;
}
