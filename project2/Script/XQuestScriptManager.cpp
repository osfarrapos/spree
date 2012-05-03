/** KUQuestScriptManager.cpp
 *
 * Main Function, 진행/완료 리스트와 관련된 일을 주로 한다.
 * \author Sohyun, Park
 * \date 2003.08.26
 */

#include "stdafx.h"
#include <stack>

#pragma warning (disable: 4786)

#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XQuestScriptManager.h"
#include "XQuestScriptItem.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
int		g_LoadQuestScriptText_LastIndex;
int*	g_LoadQuestScriptText_IndexTable = NULL;
#endif

_XQuestScriptManager::_XQuestScriptManager()
{
	m_nQuestScriptCount = 0;
	m_nProgressQuestCount = 0;
	m_nCheckQuestID = 0;
	m_bCheckQuest = FALSE;	

	memset(&m_OriginalCameraInfo, 0, sizeof(_XCAMERA_ORIGINALINFO));

	for( int i = 0; i < 5; ++i )
	{
		m_ProgressSubQuestID[i] = -1;
	}
	
	m_CurrentMainQuestID = -1;
	m_NextMainQuestID = -1;
	m_indexSelectNPCTargettingQuest = -1;
	
	m_TimerStartQuestID	  = -1;
	m_QuestTimerStartTime = 0;
	m_QuestTimerTimeLimit = 0;
	
	m_QuestStartConditionTimer = 0;

	m_QuestSystemMessageStartTime		= 0;
	m_lpszQuestSystemMessage			= NULL;

	m_bQuestCameraMode			= FALSE;
	m_bCheckQuest				= FALSE;
	m_bQuestWideView			= FALSE;
	m_bStartMessage				= FALSE;
	m_bStartQuestTimer			= FALSE; // 퀘스트 타이머 체크 시작을 알수 있는 플래그
	m_bReceiveQuestData			= FALSE;
	m_bQuestStartConditionCheck = FALSE;
	m_bStartQuestCounterMessage	= FALSE;
	m_bQuestValidCheck			= FALSE;
	m_bProgressMainQuest		= FALSE;
	m_bCancelProcess			= FALSE;
	
	m_nVirtualZoneStartPositionX	= -1;	// 가상공간에서의 시작위치를 정보를 저장한다.
	m_nVirtualZoneStartPositionZ	= -1;	// 가상공간에서의 시작위치를 정보를 저장한다.
	m_nRealZoneStartPositionX		= -1;	// 가상공간에서 되돌아갈 위치를 저장해둔다.
	m_nRealZoneStartPositionZ		= -1;	// 가상공간에서 되돌아갈 위치를 저장해둔다.
	m_indexVirtualZone				= -1;	// 가상공간 지역인덱스를 저장한다.
	m_indexRealZone					= -1;	// 가상공간으로 가기 전에 실제공간의 지역인덱스를 저장한다.

	m_VirtualMode				= _XQUEST_VIRTUALMODE_ENTER_OTHERZONE;
}

_XQuestScriptManager::~_XQuestScriptManager()
{
	Release();
}

BOOL _XQuestScriptManager::Release(void)
{
	ReleasePrivateData();

	m_nQuestScriptCount = 0;

	map <int, _XQuestScriptItem*>::iterator iter_quest;
	_XQuestScriptItem* currentquest = NULL;
	for(iter_quest = m_mapQuestScriptList.begin() ; iter_quest != m_mapQuestScriptList.end() ; ++iter_quest)
	{
		currentquest = iter_quest->second;
		if(currentquest)
		{
			delete currentquest;
			currentquest = NULL;
		}
	}	
	m_mapQuestScriptList.clear();

	map <int, _XQuestScriptItem*>::iterator iter_subquest;
	_XQuestScriptItem* currentsubquest = NULL;
	for(iter_subquest = m_mapSubQuestScriptList.begin() ; iter_subquest != m_mapSubQuestScriptList.end() ; ++iter_subquest)
	{
		currentsubquest = iter_subquest->second;
		if(currentsubquest)
		{
			delete currentsubquest;
			currentsubquest = NULL;
		}
	}
	m_mapSubQuestScriptList.clear();
	
	list <_XQUEST_MARKINFO>::iterator iter_questmark;
	for(iter_questmark = m_listQuestMark.begin() ; iter_questmark != m_listQuestMark.end() ; )
	{
		iter_questmark = m_listQuestMark.erase(iter_questmark);
	}
	m_listQuestMark.clear();
	
	list <_XQUEST_MARKINFO>::iterator iter_hintmark;
	for(iter_hintmark = m_listHintMark.begin() ; iter_hintmark != m_listHintMark.end() ; )
	{
		iter_hintmark = m_listHintMark.erase(iter_hintmark);
	}
	m_listHintMark.clear();
	
	// 시작 조건에 아이템 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보 
	list <_XQUEST_ITEMINFO>::iterator iter_checkitem;
	for(iter_checkitem = m_listCheckItem.begin() ; iter_checkitem != m_listCheckItem.end() ; )
	{
		iter_checkitem = m_listCheckItem.erase(iter_checkitem);
	}
	m_listCheckItem.clear();

	// 시작 조건에 아이템 장착 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보 
	list <_XQUEST_ITEMINFO>::iterator iter_checkwearitem;
	for(iter_checkwearitem = m_listCheckWearItem.begin() ; iter_checkwearitem != m_listCheckWearItem.end() ; )
	{
		iter_checkwearitem = m_listCheckWearItem.erase(iter_checkwearitem);
	}
	m_listCheckWearItem.clear();
	
	// 시작 조건에 아이템 파괴 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한
	list <_XQUEST_ITEMINFO>::iterator iter_checkdestroyitem;
	for(iter_checkdestroyitem = m_listCheckDestroyItem.begin() ; iter_checkdestroyitem != m_listCheckDestroyItem.end() ; )
	{
		iter_checkdestroyitem = m_listCheckDestroyItem.erase(iter_checkdestroyitem);
	}
	m_listCheckDestroyItem.clear();
	
	// 시작 조건에 레벨 검사가 있으면 레벨 상승시에 다시 검사하기 위한 정보 ( 미만일때는 계속 체크해야 한다.)
	list <_XQUEST_LEVELINFO>::iterator iter_checklevel;
	for(iter_checklevel = m_listCheckLevel.begin() ; iter_checklevel != m_listCheckLevel.end() ; )
	{
		iter_checklevel = m_listCheckLevel.erase(iter_checklevel);
	}
	m_listCheckLevel.clear();
	
	// 시작 조건에 동행 가입 검사가 있으면 동행 탈퇴시에 다시 검사하기 위한 정보 
	list <int>::iterator iter_checkinparty;
	for(iter_checkinparty = m_listCheckInParty.begin() ; iter_checkinparty != m_listCheckInParty.end() ; )
	{
		iter_checkinparty = m_listCheckInParty.erase(iter_checkinparty );
	}
	m_listCheckInParty.clear();
	
	// 풍운방 결과 체크 
	list <_XQUESTMAIL_INFO>::iterator iter_questmail;
	for(iter_questmail = m_listQuestMailInfo.begin() ; iter_questmail != m_listQuestMailInfo.end() ; )
	{
		iter_questmail = m_listQuestMailInfo.erase(iter_questmail);
	}
	m_listQuestMailInfo.clear();
	
	// 풍운방 보내기 시작 조건에서 거부할때 30분후에 다시 보내기 위한 시간을 저장
	list <_XQUESTMAIL_INFO>::iterator iter_questmailbuffer;
	for(iter_questmailbuffer = m_listQuestMailBuffer.begin() ; iter_questmailbuffer != m_listQuestMailBuffer.end() ; )
	{
		iter_questmailbuffer = m_listQuestMailBuffer.erase(iter_questmailbuffer);
	}
	m_listQuestMailBuffer.clear();
	
	// 저장하지 않는 퀘스트 리스트 
	list <int>::iterator iter_questnosavetype;
	for(iter_questnosavetype = m_listQuestNoSaveType.begin() ; iter_questnosavetype != m_listQuestNoSaveType.end() ; )
	{
		iter_questnosavetype = m_listQuestNoSaveType.erase(iter_questnosavetype);
	}
	m_listQuestNoSaveType.clear();

	
	// 시작 조건에 따른 퀘스트 아이디를 가지고 있는 리스트를 삭제한다. 
	list <int>::iterator iter_npctargetting;
	for(iter_npctargetting = m_listQuestStartTypeNPCTargetting.begin() ; iter_npctargetting != m_listQuestStartTypeNPCTargetting.end() ; )
	{
		iter_npctargetting = m_listQuestStartTypeNPCTargetting.erase(iter_npctargetting);
	}
	m_listQuestStartTypeNPCTargetting.clear();
	
	list <int>::iterator iter_objectclick;
	for(iter_objectclick = m_listQuestStartTypeObjectClick.begin() ; iter_objectclick != m_listQuestStartTypeObjectClick.end() ; )
	{
		iter_objectclick = m_listQuestStartTypeObjectClick.erase(iter_objectclick);
	}
	m_listQuestStartTypeObjectClick.clear();
	
	list <int>::iterator iter_usingitem;
	for(iter_usingitem = m_listrQuestStartTypeUsingItem.begin() ; iter_usingitem != m_listrQuestStartTypeUsingItem.end() ; )
	{
		iter_usingitem = m_listrQuestStartTypeUsingItem.erase(iter_usingitem);
	}
	m_listrQuestStartTypeUsingItem.clear();
	
	list <int>::iterator iter_wearitem;
	for(iter_wearitem = m_listQuestStartTypeWearItem.begin() ; iter_wearitem != m_listQuestStartTypeWearItem.end() ; )
	{
		iter_wearitem = m_listQuestStartTypeWearItem.erase(iter_wearitem);
	}
	m_listQuestStartTypeWearItem.clear();
	
	list <int>::iterator iter_destroyitem;
	for(iter_destroyitem = m_listQuestStartTypeDestroyItem.begin() ; iter_destroyitem != m_listQuestStartTypeDestroyItem.end() ; )
	{
		iter_destroyitem = m_listQuestStartTypeDestroyItem.erase(iter_destroyitem);
	}
	m_listQuestStartTypeDestroyItem.clear();
	
	list <int>::iterator iter_level;
	for(iter_level = m_listQuestStartTypeLevel.begin() ; iter_level != m_listQuestStartTypeLevel.end() ; )
	{
		iter_level = m_listQuestStartTypeLevel.erase(iter_level);
	}
	m_listQuestStartTypeLevel.clear();
	
	list <int>::iterator iter_zone;
	for(iter_zone = m_listQuestStartTypeZone.begin() ; iter_zone != m_listQuestStartTypeZone.end() ; )
	{
		iter_zone = m_listQuestStartTypeZone.erase(iter_zone);
	}
	m_listQuestStartTypeZone.clear();
		
	map <int, _XQUEST_COMPLETE_RESULT>::iterator currentquestresult;
	for(currentquestresult = m_mapCompleteQuestList.begin() ; currentquestresult != m_mapCompleteQuestList.end() ; )
	{
		currentquestresult = m_mapCompleteQuestList.erase(currentquestresult);
	}
	m_mapCompleteQuestList.clear();
	
	// 아이템고정 목록이 들어있는 리스트
	list <_XQUEST_HOLDITEM>::iterator iter_holditem;
	for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
	{
		iter_holditem = m_listHoldItem.erase(iter_holditem);
	}
	m_listHoldItem.clear();
	return TRUE;
}

void _XQuestScriptManager::ReleasePrivateData(void)
{
	m_bQuestCameraMode = FALSE;
	m_nProgressQuestCount = 0;
	m_nCheckQuestID = 0;
	m_bCheckQuest = FALSE;
	m_bQuestWideView = FALSE;
	m_bReceiveQuestData = FALSE;

	m_mapCompleteQuestList.clear();
	m_mapRewordQuestList.clear();

	map <int, _XQuestScriptItem*>::iterator iter_quest;
	for(iter_quest = m_mapQuestScriptList.begin() ; iter_quest != m_mapQuestScriptList.end() ; ++iter_quest)
	{
		int questid = iter_quest->first;

		_XQuestScriptItem* currentquest = iter_quest->second;
		if(currentquest)
			currentquest->ReleasePrivateData();
	}
	for(iter_quest = m_mapSubQuestScriptList.begin() ; iter_quest != m_mapSubQuestScriptList.end() ; ++iter_quest)
	{
		int questid = iter_quest->first;
		
		_XQuestScriptItem* currentquest = iter_quest->second;
		if(currentquest)
			currentquest->ReleasePrivateData();
	}

	memset(&m_OriginalCameraInfo, 0, sizeof(_XCAMERA_ORIGINALINFO));
}

BOOL _XQuestScriptManager::LoadQuestScript(LPCTSTR filename)
{

	if(!filename || filename[0] == NULL)
		return FALSE;

	FILE* fileptr = NULL;
	fileptr = fopen(filename, "rb");
	if(!fileptr)
	{
		_XFatalError(_T("Quest script file open"));
		return FALSE;
	}

	if(!LoadQuestScript(fileptr))
	{
		_XFatalError("Quest script loading [%s]", filename);
		fclose(fileptr);
		return FALSE;
	}

	fclose(fileptr);
	return TRUE;
}

BOOL _XQuestScriptManager::LoadQuestScript(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	map <int, _XQuestScriptItem*>::iterator currentquest;
	for(currentquest = m_mapQuestScriptList.begin() ; currentquest != m_mapQuestScriptList.end() ; ++currentquest)
	{
		delete currentquest->second;
		//m_mapQuestScriptList.erase(currentquest);
	}
	m_mapQuestScriptList.clear();

	map <int, _XQuestScriptItem*>::iterator currentsubquest;
	for(currentsubquest = m_mapSubQuestScriptList.begin() ; currentsubquest != m_mapSubQuestScriptList.end() ; ++currentsubquest)
	{
		delete currentsubquest->second;
		//m_mapQuestScriptList.erase(currentquest);
	}
	m_mapSubQuestScriptList.clear();
	
#ifdef _XDEF_QUESTSCRIPT_11
	int majorVersion = 0, minorVersion = 0;
	if(fread(&majorVersion, sizeof(int), 1, fileptr) < 1)
	{
		_XFatalError(_T("LoadQuestScript : quest major version"));
		return FALSE;
	}
	if(fread(&minorVersion, sizeof(int), 1, fileptr) < 1)
	{
		_XFatalError(_T("LoadQuestScript : quest major version"));
		return FALSE;
	}
#endif

	if(fread(&m_nQuestScriptCount, sizeof(int), 1, fileptr) < 1)
	{
		_XFatalError(_T("LoadQuestScript : quest script count"));
		return FALSE;
	}
	if(m_nQuestScriptCount <= 0)
		return TRUE;

	_XQuestScriptItem* questscriptitem = NULL;
	for(int i = 0 ; i < m_nQuestScriptCount ; ++i)
	{
		questscriptitem = new _XQuestScriptItem;
		if(questscriptitem)
		{
			if(!questscriptitem->Load(fileptr))
			{
				delete questscriptitem;
				_XFatalError(_T("Quest script item load"));
				return FALSE;
			}
			else
			{
				if( questscriptitem->GetMainQuest() ) //if( questscriptitem->GetQuestID() >= 10000 && questscriptitem->GetQuestID() < 20000  )
				{
					m_mapQuestScriptList[questscriptitem->GetQuestID()] = questscriptitem;
				}
				else
				{
					m_mapSubQuestScriptList[ questscriptitem->GetQuestID() ] = questscriptitem;
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

#define _NTEST_

void _XQuestScriptManager::Process(void)
{

#ifdef _NTEST_
	
#ifdef _XDEF_SHUTDOWN_20061027
	// 플레이 가능 시간이 지나면 퀘스트를 진행 할 수 없다.
	if( g_PlayLeftTime <= 0 ) return;// g_PlayLeftTime가 0이 아니면 진행
#endif

	if( (g_LocalSystemTime - m_QuestStartConditionTimer) > 3000)
	{
		QuestStartConditionZoneMode();
		m_QuestStartConditionTimer = g_LocalSystemTime;
	}
	
	// 제룡행 퀘스트 Process
	if( m_bProgressMainQuest )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				if(!currentquest->GetComplete())
				{					
					switch(currentquest->Process())
					{
					case QUEST_QUEST_RESULT_SUCCESS :
						{
							if(!currentquest->GetInsertCompleteList())
							{
								m_bProgressMainQuest = FALSE;
								currentquest->SetFirstStartCondition( FALSE );
								currentquest->SetQuestCompleteResult(QUEST_COMPLETE_RESULT_SOLVE);
								InsertQuestCompleteList(currentquest);
								
								// 하나의 퀘스트가 끝나면 다음 퀘스트 번호를 세팅
								if( m_NextMainQuestID != -1 )
								{
									m_CurrentMainQuestID = m_NextMainQuestID;
									
									SetCheckQuestID(m_CurrentMainQuestID);
									SetCheckQuest(TRUE);		
									QuestRunningSetting( m_CurrentMainQuestID, 0 );

									// 다음 퀘스트의 시작 조건을 세팅한다.
									SetMainQuestStartCondition(m_CurrentMainQuestID);
									QuestStartConditionZoneMode();
									QuestStartConditionLevelMode();
									QuestStartConditionWearItemMode();
								}
								
								_XWindow_QuestWindow* pQuest_Window = (_XWindow_QuestWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTWINDOW);
								if(pQuest_Window)
								{
									if(pQuest_Window->GetShowStatus())
									{
										pQuest_Window->SetListItem();
										g_pQuest_Window->SetQuestTabButton();
										if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST1 )
										{
											g_pQuest_Window->ParseQuestDescString();
										}
									}
								}
								
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_QUESTSCRIPTMANAGER_COMPLETEQUEST), 
									_XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					case QUEST_QUEST_RESULT_PROCESS :
						{					
						
						}
						break;
					}
				}
			}
		}

	}
	
	// 협객행, 기연행 퀘스트 Process
	// 진행 중인 퀘스트의 Process를 진행한다.
	for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
	{
		if( m_ProgressSubQuestID[i] != -1)
		{			
			map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find( m_ProgressSubQuestID[i] );
			if(iter_quest != m_mapSubQuestScriptList.end())
			{
				_XQuestScriptItem* currentquest = iter_quest->second;
				if(currentquest)
				{
					if(!currentquest->GetComplete())
					{						
						switch(currentquest->Process())
						{
						case QUEST_QUEST_RESULT_SUCCESS :
							{
								if(!currentquest->GetInsertCompleteList())
								{
									if( m_ProgressSubQuestID[i] >=10000 )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_QUESTSCRIPTMANAGER_COMPLETEQUEST), 
											_XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
									}
									currentquest->SetFirstStartCondition( FALSE );
									currentquest->SetQuestCompleteResult(QUEST_COMPLETE_RESULT_SOLVE);
									InsertQuestCompleteList(currentquest);
									m_ProgressSubQuestID[i] = -1;
									DecreaseProgressQuestCount();									
									
									// 다른 퀘스트의 시작 조건을 세팅한다.
									ResetQuestStartCondition();
									QuestStartConditionZoneMode();
									QuestStartConditionLevelMode();
									QuestStartConditionWearItemMode();
									
									if( g_pQuest_Window )
									{
										g_pQuest_Window->SetListItem();
										g_pQuest_Window->SetQuestTabButton();
										if( g_pQuest_Window->m_SelectedTab != _XQUESTTAB_QUEST1 )
										{
											g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
											g_pQuest_Window->m_SelectedQuestID = currentquest->GetQuestID();
											g_pQuest_Window->ParseQuestDescString();
										}
									}														
								}
							}
							break;
						case QUEST_QUEST_RESULT_PROCESS :
							{								
							}
							break;
						}
					}
				}
			}

		}
	}
	
	// 저장되지 않는 퀘스트를 진행한다.
	if( !m_listQuestNoSaveType.empty() )
	{		
		list <int>::iterator iter_questnosavetype;		
		iter_questnosavetype = m_listQuestNoSaveType.begin();
		while( iter_questnosavetype != m_listQuestNoSaveType.end() )
		{			
			int questid = *iter_questnosavetype;			
			if( questid != -1 )
			{
				_XQuestScriptItem* currentquest = m_mapSubQuestScriptList[questid];
				if( currentquest )
				{
					if(!currentquest->GetComplete())
					{					
						switch(currentquest->Process())
						{
						case QUEST_QUEST_RESULT_SUCCESS :
							{
								if(!currentquest->GetInsertCompleteList())
								{
									// 무분류는 알려주지 않는다.
									if( currentquest->GetQuestID() >=10000 )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_QUESTSCRIPTMANAGER_COMPLETEQUEST), 
											_XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
									}

									g_pLocalUser->m_bPartyQuestProgress = FALSE;
									
									iter_questnosavetype = m_listQuestNoSaveType.erase(iter_questnosavetype);
									
									currentquest->Reset();
									currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_NONE); // 다시 시작하기 위한 조건 
									// 저장되지 않는 퀘스트는 다시 퀘스트 시작조건 리스트에 들어간다.
									SetQuestStartCondition(currentquest->GetQuestID());

									// 다른 퀘스트의 시작 조건을 세팅한다.
									ResetQuestStartCondition();
									QuestStartConditionZoneMode();
									QuestStartConditionLevelMode();
									QuestStartConditionWearItemMode();
									
									// 저장 되지 않는 퀘스트는 퀘스트가 없어진다.
									if( g_pQuest_Window )
									{
										if( g_pQuest_Window->GetShowStatus() )
										{
											if( g_pQuest_Window->m_SelectedQuestID == currentquest->GetQuestID() ) 
												g_pQuest_Window->m_SelectedItemIndex = -1;
											g_pQuest_Window->SetQuestTabButton();
											g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
										}
									}			
																						
								}
								else
								{
									iter_questnosavetype++;	
								}
							}
							break;
						case QUEST_QUEST_RESULT_PROCESS :
							{			
								iter_questnosavetype++;					
							}
							break;
						}
					}
				}
				else
				{
					iter_questnosavetype = m_listQuestNoSaveType.erase(iter_questnosavetype);
				}
			}
			else
			{
				iter_questnosavetype = m_listQuestNoSaveType.erase(iter_questnosavetype);
			}
		}
	}
	

#endif
}

DWORD _XQuestScriptManager::GetCompareValue(int comparecount)
{
	switch(comparecount)
	{
	case 1 :
		return 0x80000000;
	case 2 :
		return 0xC0000000;
	case 3 :
		return 0xE0000000;
	case 4 :
		return 0xF0000000;
	case 5 :
		return 0xF8000000;
	case 6 :
		return 0xFC000000;
	case 7 :
		return 0xFE000000;
	case 8 :
		return 0xFF000000;
	case 9 :
		return 0xFF800000;
	case 10 :
		return 0xFFC00000;
	case 11 :
		return 0xFFE00000;
	case 12 :
		return 0xFFF00000;
	case 13 :
		return 0xFFF80000;
	case 14 :
		return 0xFFFC0000;
	case 15 :
		return 0xFFFE0000;
	case 16 :
		return 0xFFFF0000;
	case 17 :
		return 0xFFFF8000;
	case 18 :
		return 0xFFFFC000;
	case 19 :
		return 0xFFFFE000;
	case 20 :
		return 0xFFFFF000;
	case 21 : 
		return 0xFFFFF800;
	case 22 : 
		return 0xFFFFFC00;
	case 23 : 
		return 0xFFFFFE00;
	case 24 :
		return 0xFFFFFF00;
	case 25 : 
		return 0xFFFFFF80;
	case 26 :
		return 0xFFFFFFC0;
	case 27 :
		return 0xFFFFFFE0;
	case 28 :
		return 0xFFFFFFF0;
	case 29 :
		return 0xFFFFFFF8;
	case 30 :
		return 0xFFFFFFFC;
	case 31 :
		return 0xFFFFFFFE;
	case 32 :
		return 0xFFFFFFFF;
	default :
		return 0x00000000;
	}
}

int _XQuestScriptManager::GetCompareCount(DWORD comparevalue)
{
	switch(comparevalue)
	{
	case 0x80000000 :
		return 1;
	case 0xC0000000 :
		return 2;
	case 0xE0000000 :
		return 3;
	case 0xF0000000 :
		return 4;
	case 0xF8000000 :
		return 5;
	case 0xFC000000 :
		return 6;
	case 0xFE000000 :
		return 7;
	case 0xFF000000 :
		return 8;
	case 0xFF800000 :
		return 9;
	case 0xFFC00000 :
		return 10;
	case 0xFFE00000 :
		return 11;
	case 0xFFF00000 :
		return 12;
	case 0xFFF80000 :
		return 13;
	case 0xFFFC0000 :
		return 14;
	case 0xFFFE0000 :
		return 15;
	case 0xFFFF0000 :
		return 16;
	case 0xFFFF8000 :
		return 17;
	case 0xFFFFC000 :
		return 18;
	case 0xFFFFE000 :
		return 19;
	case 0xFFFFF000 :
		return 20;
	case 0xFFFFF800 :
		return 21;
	case 0xFFFFFC00 :
		return 22;
	case 0xFFFFFE00 :
		return 23;
	case 0xFFFFFF00 :
		return 24;
	case 0xFFFFFF80 :
		return 25;
	case 0xFFFFFFC0 :
		return 26;
	case 0xFFFFFFE0 :
		return 27;
	case 0xFFFFFFF0 :
		return 28;
	case 0xFFFFFFF8 :
		return 29;
	case 0xFFFFFFFC :
		return 30;
	case 0xFFFFFFFE :
		return 31;
	case 0xFFFFFFFF :
		return 32;
	default :
		return 0;
	}
}

BOOL _XQuestScriptManager::CheckProgressQuestCount(void)
{
	if(m_nProgressQuestCount < ID_QUEST_SYNC-1)
		return TRUE;
	else
		return FALSE;
}

void _XQuestScriptManager::IncreaseProgressQuestCount(void)
{
	m_nProgressQuestCount++;
}

void _XQuestScriptManager::DecreaseProgressQuestCount(void)
{
	m_nProgressQuestCount--;

	if(m_nProgressQuestCount <= 0)
		m_nProgressQuestCount = 0;
}

int _XQuestScriptManager::GetProgressQuestCount(void)
{
	return m_nProgressQuestCount;
}


void _XQuestScriptManager::InsertQuestCompleteList(_XQuestScriptItem* currentquest)
{
	m_mapCompleteQuestList[currentquest->GetQuestID()] = currentquest->GetQuestCompleteResult();
	currentquest->SetComplete(TRUE);
	currentquest->SetInsertCompleteList(TRUE);
}

void _XQuestScriptManager::InsertQuestCompleteList(int questid, _XQUEST_COMPLETE_RESULT result)
{
	m_mapCompleteQuestList[questid] = result;
	
	_XQuestScriptItem* currentquest = NULL;
	if( questid >= 10000 && questid < 20000)
	{
		currentquest = m_mapQuestScriptList[questid];
	}
	else
	{
		currentquest = m_mapSubQuestScriptList[questid];
	}
	if(currentquest)
	{
		currentquest->SetComplete(TRUE);
		currentquest->SetInsertCompleteList(TRUE);
		currentquest->SetQuestCompleteResult(result);		
		m_mapCompleteQuestList[currentquest->GetQuestID()] = currentquest->GetQuestCompleteResult();
	}
}

_XQUEST_COMPLETE_RESULT _XQuestScriptManager::FindQuestCompleteResult(int questid)
{
	map <int, _XQUEST_COMPLETE_RESULT>::iterator questresult = m_mapCompleteQuestList.find(questid);
	if(questresult != m_mapCompleteQuestList.end())
		return m_mapCompleteQuestList[questid];
	
	return QUEST_COMPLETE_RESULT_NONE;
}

// 퀘스트가 완료 되었는지 여부 판별
BOOL _XQuestScriptManager::CheckCompleteQuest(int questid)
{
	if( questid >= 10000 && questid < 20000 )
	{
		if( questid < m_CurrentMainQuestID )
			return TRUE;
	}
	else
	{
		map <int, _XQUEST_COMPLETE_RESULT>::iterator iter;
		for(iter = m_mapCompleteQuestList.begin() ; iter != m_mapCompleteQuestList.end() ; ++iter)
		{				
			int completequestid = iter->first;
			if( completequestid ==  questid )
			{			
				return TRUE;		
			}
		}
	}
	
	return FALSE;
}

TCHAR*	_XQuestScriptManager::GetQuestName(int questid)
{
	map <int, _XQuestScriptItem*>::iterator qlist1 = m_mapQuestScriptList.find(questid);
	if(qlist1 != m_mapQuestScriptList.end())
	{
		_XQuestScriptItem* currentquest = qlist1->second;
		if(currentquest)
		{
			return currentquest->GetQuestName();
		}
	}

	map <int, _XQuestScriptItem*>::iterator qlist2 = m_mapSubQuestScriptList.find(questid);
	if(qlist2 != m_mapSubQuestScriptList.end())
	{
		_XQuestScriptItem* currentquest = qlist2->second;
		if(currentquest)
		{
			return currentquest->GetQuestName();
		}
	}
	return NULL;
}

BOOL _XQuestScriptManager::GetQuestCameraMode(void)
{
	return m_bQuestCameraMode;
}

void _XQuestScriptManager::SetQuestCameraMode(BOOL mode)
{
	m_bQuestCameraMode = mode;
}

void _XQuestScriptManager::ResetQuest(void)
{
	
	m_bQuestCameraMode = FALSE;
	m_nProgressQuestCount = 0;
	m_nCheckQuestID = 0;
	m_bCheckQuest = FALSE;	
	
	memset(&m_OriginalCameraInfo, 0, sizeof(_XCAMERA_ORIGINALINFO));
	
	m_bQuestWideView = FALSE;
	m_bReceiveQuestData = FALSE;
	
	for( int i = 0; i < 5; ++i )
	{
		m_ProgressSubQuestID[i] = -1;
	}
	
	m_CurrentMainQuestID = -1;
	m_NextMainQuestID = -1;
	m_bQuestStartConditionCheck = FALSE;
	m_bQuestValidCheck			= FALSE;
	m_bProgressMainQuest = FALSE;
	m_bStartMessage		 = FALSE;
	
	m_TimerStartQuestID   = -1;
	m_bStartQuestTimer    = FALSE; // 퀘스트 타이머 체크 시작을 알수 있는 플래그
	m_QuestTimerStartTime = 0;
	m_QuestTimerTimeLimit = 0;
	
	m_QuestSystemMessageStartTime		= 0;
	m_lpszQuestSystemMessage			= NULL;

	// 퀘스트 창 초기화
	g_pQuest_Window->m_SelectedItemIndex = -1;
	g_pQuest_Window->m_QuestGroupList.disposeList();
	g_pQuest_Window->m_QuestGroupNumber = 0;


	map <int, _XQUEST_COMPLETE_RESULT>::iterator currentquestresult;
	for(currentquestresult = m_mapCompleteQuestList.begin() ; currentquestresult != m_mapCompleteQuestList.end() ; )
	{
		currentquestresult = m_mapCompleteQuestList.erase(currentquestresult);
	}
	m_mapCompleteQuestList.clear();

	if( !m_mapQuestScriptList.empty() )
	{
		map <int, _XQuestScriptItem*>::iterator currentquest;
		for(currentquest = m_mapQuestScriptList.begin() ; currentquest != m_mapQuestScriptList.end() ; ++currentquest)
		{
			if( currentquest->second ) currentquest->second->Reset();
		}
	}

	if( !m_mapSubQuestScriptList.empty() )
	{
		map <int, _XQuestScriptItem*>::iterator currentquest;
		for(currentquest = m_mapSubQuestScriptList.begin() ; currentquest != m_mapSubQuestScriptList.end() ; ++currentquest)
		{
			if( currentquest->second ) currentquest->second->Reset();
		}		
	}

	if( !m_listQuestMark.empty() )
	{
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = m_listQuestMark.begin() ; iter_questmark != m_listQuestMark.end() ; )
		{
			iter_questmark = m_listQuestMark.erase(iter_questmark);
		}
		m_listQuestMark.clear();
	}

	if( !m_listHintMark.empty() )
	{
		list <_XQUEST_MARKINFO>::iterator iter_hintmark;
		for(iter_hintmark = m_listHintMark.begin() ; iter_hintmark != m_listHintMark.end() ; )
		{
			iter_hintmark = m_listHintMark.erase(iter_hintmark);
		}
		m_listHintMark.clear();
	}
	
	// 시작 조건에 아이템 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보 
	list <_XQUEST_ITEMINFO>::iterator iter_checkitem;
	for(iter_checkitem = m_listCheckItem.begin() ; iter_checkitem != m_listCheckItem.end() ; )
	{
		iter_checkitem = m_listCheckItem.erase(iter_checkitem);
	}
	m_listCheckItem.clear();
	
	// 시작 조건에 아이템 장착 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보 
	list <_XQUEST_ITEMINFO>::iterator iter_checkwearitem;
	for(iter_checkwearitem = m_listCheckWearItem.begin() ; iter_checkwearitem != m_listCheckWearItem.end() ; )
	{
		iter_checkwearitem = m_listCheckWearItem.erase(iter_checkwearitem);
	}
	m_listCheckWearItem.clear();
	
	// 시작 조건에 아이템 파괴 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보 
	list <_XQUEST_ITEMINFO>::iterator iter_checkdestroyitem;
	for(iter_checkdestroyitem = m_listCheckDestroyItem.begin() ; iter_checkdestroyitem != m_listCheckDestroyItem.end() ; )
	{
		iter_checkdestroyitem = m_listCheckDestroyItem.erase(iter_checkdestroyitem);
	}
	m_listCheckDestroyItem.clear();
	
	// 시작 조건에 레벨 검사가 있으면 레벨 상승시에 다시 검사하기 위한 정보 ( 미만일때는 계속 체크해야 한다.)
	list <_XQUEST_LEVELINFO>::iterator iter_checklevel;
	for(iter_checklevel = m_listCheckLevel.begin() ; iter_checklevel != m_listCheckLevel.end() ; )
	{
		iter_checklevel = m_listCheckLevel.erase(iter_checklevel);
	}
	m_listCheckLevel.clear();
	
	// 시작 조건에 동행 가입 검사가 있으면 동행 탈퇴시에 다시 검사하기 위한 정보 
	list <int>::iterator iter_checkinparty;
	for(iter_checkinparty = m_listCheckInParty.begin() ; iter_checkinparty != m_listCheckInParty.end() ; )
	{
		iter_checkinparty = m_listCheckInParty.erase(iter_checkinparty );
	}
	m_listCheckInParty.clear();
	
	// 풍운방 결과 체크 
	list <_XQUESTMAIL_INFO>::iterator iter_questmail;
	for(iter_questmail = m_listQuestMailInfo.begin() ; iter_questmail != m_listQuestMailInfo.end() ; )
	{
		iter_questmail = m_listQuestMailInfo.erase(iter_questmail);
	}
	m_listQuestMailInfo.clear();
	
	// 풍운방 보내기 시작 조건에서 거부할때 30분후에 다시 보내기 위한 시간을 저장
	list <_XQUESTMAIL_INFO>::iterator iter_questmailbuffer;
	for(iter_questmailbuffer = m_listQuestMailBuffer.begin() ; iter_questmailbuffer != m_listQuestMailBuffer.end() ; )
	{
		iter_questmailbuffer = m_listQuestMailBuffer.erase(iter_questmailbuffer);
	}
	m_listQuestMailBuffer.clear();	
	
	
	// 한 NPC에 여러 퀘스트의 타게팅이 걸려있을때 퀘스트 번호를 저장하는 리스트	
	list <_XQUEST_QUESTLIST>::iterator iter_npctargettingquest;
	for(iter_npctargettingquest = m_listNPCTargettingQuest.begin() ; iter_npctargettingquest != m_listNPCTargettingQuest.end() ; )
	{
		iter_npctargettingquest = m_listNPCTargettingQuest.erase(iter_npctargettingquest);
	}
	m_listNPCTargettingQuest.clear();

	// 저장하지 않는 퀘스트 리스트 
	list <int>::iterator iter_questnosavetype;
	for(iter_questnosavetype = m_listQuestNoSaveType.begin() ; iter_questnosavetype != m_listQuestNoSaveType.end() ; )
	{
		iter_questnosavetype = m_listQuestNoSaveType.erase(iter_questnosavetype);
	}
	m_listQuestNoSaveType.clear();
	

	// 시작 조건에 따른 퀘스트 아이디를 가지고 있는 리스트를 삭제한다. 
	list <int>::iterator iter_npctargetting;
	for(iter_npctargetting = m_listQuestStartTypeNPCTargetting.begin() ; iter_npctargetting != m_listQuestStartTypeNPCTargetting.end() ; )
	{
		iter_npctargetting = m_listQuestStartTypeNPCTargetting.erase(iter_npctargetting);
	}
	m_listQuestStartTypeNPCTargetting.clear();
	
	list <int>::iterator iter_objectclick;
	for(iter_objectclick = m_listQuestStartTypeObjectClick.begin() ; iter_objectclick != m_listQuestStartTypeObjectClick.end() ; )
	{
		iter_objectclick = m_listQuestStartTypeObjectClick.erase(iter_objectclick);
	}
	m_listQuestStartTypeObjectClick.clear();
	
	list <int>::iterator iter_usingitem;
	for(iter_usingitem = m_listrQuestStartTypeUsingItem.begin() ; iter_usingitem != m_listrQuestStartTypeUsingItem.end() ; )
	{
		iter_usingitem = m_listrQuestStartTypeUsingItem.erase(iter_usingitem);
	}
	m_listrQuestStartTypeUsingItem.clear();
	
	list <int>::iterator iter_wearitem;
	for(iter_wearitem = m_listQuestStartTypeWearItem.begin() ; iter_wearitem != m_listQuestStartTypeWearItem.end() ; )
	{
		iter_wearitem = m_listQuestStartTypeWearItem.erase(iter_wearitem);
	}
	m_listQuestStartTypeWearItem.clear();
	
	list <int>::iterator iter_destroyitem;
	for(iter_destroyitem = m_listQuestStartTypeDestroyItem.begin() ; iter_destroyitem != m_listQuestStartTypeDestroyItem.end() ; )
	{
		iter_destroyitem = m_listQuestStartTypeDestroyItem.erase(iter_destroyitem);
	}
	m_listQuestStartTypeDestroyItem.clear();
	
	list <int>::iterator iter_level;
	for(iter_level = m_listQuestStartTypeLevel.begin() ; iter_level != m_listQuestStartTypeLevel.end() ; )
	{
		iter_level = m_listQuestStartTypeLevel.erase(iter_level);
	}
	m_listQuestStartTypeLevel.clear();
	
	list <int>::iterator iter_zone;
	for(iter_zone = m_listQuestStartTypeZone.begin() ; iter_zone != m_listQuestStartTypeZone.end() ; )
	{
		iter_zone = m_listQuestStartTypeZone.erase(iter_zone);
	}
	m_listQuestStartTypeZone.clear();
	
	// 아이템고정 목록이 들어있는 리스트
	if( !m_listHoldItem.empty() )
	{
		list <_XQUEST_HOLDITEM>::iterator iter_holditem;
		for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
		{
			iter_holditem = m_listHoldItem.erase(iter_holditem);
		}
		m_listHoldItem.clear();
	}
	
}

void _XQuestScriptManager::ResetQuest(int questid)
{
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(questid);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->Reset();				
				
				_XWindow_QuestWindow* pQuest_Window = (_XWindow_QuestWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTWINDOW);
				if(pQuest_Window)
				{
					if(pQuest_Window->GetShowStatus())
						pQuest_Window->SetListItem();
				}
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
		if(iter_quest != m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->Reset();

				_XWindow_QuestWindow* pQuest_Window = (_XWindow_QuestWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTWINDOW);
				if(pQuest_Window)
				{
					if(pQuest_Window->GetShowStatus())
						pQuest_Window->SetListItem();
				}
			}
		}
	}
	
	if( m_TimerStartQuestID == questid )
	{
		m_bStartQuestTimer		= FALSE;
		m_QuestTimerStartTime	= 0;
		m_QuestTimerTimeLimit	= 0;
	}
	
	for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
	{
		if( m_ProgressSubQuestID[i] == questid)
		{
			m_ProgressSubQuestID[i] = -1;
			break;
		}
	}		

	if(m_bQuestCameraMode)
		m_bQuestCameraMode = FALSE;
	if(m_bQuestWideView)
		m_bQuestWideView = FALSE;
}

void _XQuestScriptManager::ResetQuest(int questid, int triggerid)
{

}

void _XQuestScriptManager::QuestSuccessSetting(int questid)
{
	
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter->second;
			if(currentquest)
			{
				InsertQuestCompleteList(questid, QUEST_COMPLETE_RESULT_SOLVE);
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter->second;
			if(currentquest)
			{
				InsertQuestCompleteList(questid, QUEST_COMPLETE_RESULT_SOLVE);
			}
		}
	}
}

void _XQuestScriptManager::QuestFailSetting(int questid)
{
	
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter->second;
			if(currentquest)
			{
				InsertQuestCompleteList(questid, QUEST_COMPLETE_RESULT_MISS);
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter->second;
			if(currentquest)
			{
				InsertQuestCompleteList(questid, QUEST_COMPLETE_RESULT_MISS);
			}
		}
	}
}

void _XQuestScriptManager::QuestRunningSetting(int questid, BYTE treestep)
{	
	ResetQuest( questid );

	_XQuestScriptItem* currentquest = NULL;
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}		
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}
	
	if(currentquest)
	{
		if( treestep == 0 )
		{
		}
		else if(treestep == 1)
		{
			// 시작 조건
			currentquest->SetClientEnable(TRUE);
			currentquest->SetServerEnable(TRUE);
			currentquest->SetConfirmAll(TRUE);

			// 시작 조건만 만족하고 맵을 넘어가는 경우 퀘스트 시작시간 기록하도록 - insert by kukuri
			currentquest->SetStartTime(g_ServerTimeCode);

			currentquest->SetCurrentTreeNumber(treestep); // 트리거 ID대신 트리 번호로 온다.
														  // SetCurrentTreeNumber함수 안에서 트리거 ID를 설정한다.
		}
		else if( treestep == 127 )
		{
			
			currentquest->SetProgressTrigger(treestep,TRUE);
			currentquest->SetTriggerServerEnable(treestep, TRUE );
			currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_SOLVE);
			currentquest->SetFirstStartCondition( FALSE );	
			
			currentquest->m_lpszQuestProgressinformation = NULL;

			// Reword list에 있으면 삭제
			map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = m_mapRewordQuestList.find(questid);
			if(iter_rewordquest != m_mapRewordQuestList.end())
			{
				DeleteRewordQuestList(questid);
			}
		}
		else if( treestep == 126 )
		{
			currentquest->SetProgressTrigger(treestep,FALSE);
			currentquest->SetTriggerServerEnable(treestep, TRUE );
			currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_MISS);
			currentquest->SetFirstStartCondition( FALSE );
			
			// Reword list에 있으면 삭제
			map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = m_mapRewordQuestList.find(questid);
			if(iter_rewordquest != m_mapRewordQuestList.end())
			{
				DeleteRewordQuestList(questid);
			}
		}
		else
		{
			// 트리거 실행중
			currentquest->SetClientEnable(TRUE);
			currentquest->SetServerEnable(TRUE);
			currentquest->SetConfirmAll(TRUE);
			currentquest->SetStartTime(g_ServerTimeCode);
			
			currentquest->SetCurrentTreeNumber(treestep); // 트리거 ID대신 트리 번호로 온다.
			
			
			for( int i = 0 ; i < 6 ; ++i)
			{
				if( currentquest->GetQuestID() == g_NetworkKernel.m_RunningQuestData[i].usQuestID)
				{
					if( !currentquest->GetTempQuest() )
					{
						QuestRunningTimerSetting( currentquest->GetQuestID(), g_NetworkKernel.m_RunningQuestData[i].dwQuestStartTime, g_NetworkKernel.m_RunningQuestData[i].cTimerNodeNo, g_NetworkKernel.m_RunningQuestData[i].dwQuestTimer);
						
						// 선택지 번호 저장
						currentquest->m_SelectionNumber = g_NetworkKernel.m_RunningQuestData[i].ucSelectNo;
						
						//카운터 저장
						for( int j = 0; j < 4; ++j )
						{
							currentquest->m_CounterInformation[j].CountCurrentPoint = g_NetworkKernel.m_RunningQuestData[i].ucCounter[j];
						}				
					}
					
				}
			}
			if( !QuestHistorySetting( questid, treestep ) )
			{
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT DATA ERROR)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Quest Node Error : QuestID[%d] CurrentNode[%d]", questid,treestep); 
			}
		} // else
	}

}

struct sHistory
{
	int triggerid;
	int treenodenumber;
	BOOL bSuccess;
};

// 퀘스트 취소 시에 퀘스트에서 세팅한 정보를 취소시키기 위한 함수
BOOL _XQuestScriptManager::QuestCancleHistorySetting(int questid)
{	
	_XQuestScriptItem* currentquest = NULL;
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}		
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}

	if( currentquest )
	{				
		int treestep = currentquest->GetCurrentTreeNumber();
		// 시작 퀘스트 정보 세팅 
		if( treestep > 0 )
		{
			if(currentquest->m_startquestinformationmessage)
			{
				int strlength = strlen(currentquest->m_startquestinformationmessage);
				
				if(strlength > 0)
				{					
					currentquest->m_lpszQuestProgressinformation = currentquest->m_startquestinformationmessage;
				}					
			}
		}					

		int searchtreenode = 0;
		int selftreenode = 0;
		int currentquesttriggerid = 0;
		// 히스토리를 구현해서 필요한 정보들을 세팅한다. -> 진행정보, 옵션조건식 세팅....
		typedef std::stack<sHistory>	HistoryStack;
		HistoryStack			stackQuestHistory;
		
		map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treestep);
		if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
		{
			_XTriggerTreeItem* currenttree = iter_triggertree->second;
			if(currenttree)
			{				
				searchtreenode = currenttree->m_ParentsNodeUniqueID;
				selftreenode = currenttree->m_SelfNodeUniqueID;
			}
		}

		sHistory historystruct;
		do {				
			map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(searchtreenode);
			if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
			{
				_XTriggerTreeItem* currenttree = iter_triggertree->second;
				if(currenttree)
				{
					if( currenttree->m_TRightNodeUniqueID == selftreenode )
					{
						historystruct.bSuccess = TRUE;
					}
					else if( currenttree->m_FLeftNodeUniqueID == selftreenode )
					{
						historystruct.bSuccess = FALSE;
					}
					selftreenode = currenttree->m_SelfNodeUniqueID;
					searchtreenode = currenttree->m_ParentsNodeUniqueID;
					
					// 세팅으로 온 노드는 실행해야할 노드이기 때문에 서버, 클라이언트 완료 세팅을 하지 않는다.
					if( selftreenode == treestep )
					{
						currentquesttriggerid = currenttree->m_TriggerUniqueID;
					}
					historystruct.treenodenumber = selftreenode;
					historystruct.triggerid = currenttree->m_TriggerUniqueID;

					stackQuestHistory.push( historystruct );
				}
				else
				{
					return FALSE;
				}
			}	
			else
			{
				return FALSE;
			}
		} while( selftreenode != 1 );
		
		while (!stackQuestHistory.empty())
		{
			sHistory temphistorystruct = stackQuestHistory.top();
			stackQuestHistory.pop();

			int treenodenumber = temphistorystruct.treenodenumber;
			int triggerid = temphistorystruct.triggerid;
			BOOL bSuccess = temphistorystruct.bSuccess;
			
			// 세팅으로 온 노드는 실행해야할 노드이기 때문에 서버, 클라이언트 완료 세팅을 하지 않는다.
			if( currentquesttriggerid != triggerid )
			{
				currentquest->SetTriggerClientEnable(triggerid, TRUE);
				currentquest->SetTriggerServerEnable(triggerid, TRUE);
				currentquest->SetTriggerConfirmAll(triggerid, TRUE );
				currentquest->SetTriggerComplete(triggerid, TRUE);
			}
			

			map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(triggerid);
			_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
			if(currentsuccesstrigger)
			{
				// 성공일때 행동식에서 퀘스트 진행정보나 옵션 조건식등을 세팅한다.
				if( bSuccess )
				{
					map <int, _XTriggerActionItem*>::iterator iter_normalaction;
					_XTriggerActionItem* currentaction = NULL;
					for(iter_normalaction = currentsuccesstrigger->m_mapNormalActionList.begin() ; iter_normalaction != currentsuccesstrigger->m_mapNormalActionList.end() ; ++iter_normalaction)
					{
						currentaction = iter_normalaction->second;
						if(currentaction)
						{
							if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )
							{								
								for( int _i = 0; _i < g_Npclist.m_NPCObjectCount; ++_i )
								{			
									if( g_Npclist.m_pNPCObjectArray[_i].m_UniqueID == currentaction->m_TDA.m_numericvalue1 )
									{
										if( g_Npclist.m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
										{
											if( currentaction->m_TDA.m_actiondata1 == 0 ) // ON
											{
												g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE; // NPC를 켯으면 꺼준다.
											}
											else // OFF
											{
											//	g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = TRUE;	// NPC를 껏으면 켜준다.
											}
											break;
										}	
									}
								}
								
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
							{				
								if( currentaction->m_TDA.m_actiondata2 == 0 )//0:아이템고정시작
								{
									// 아이템 고정 리스트가 있으면 취소 시킨다.
									if( !m_listHoldItem.empty() )
									{
										list <_XQUEST_HOLDITEM>::iterator iter_holditem;
										for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
										{
											_XQUEST_HOLDITEM questholditem = *iter_holditem;
											if( questholditem.m_cHoldItemType == currentaction->m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == currentaction->m_TDA.m_numericvalue1 )
											{
												iter_holditem = m_listHoldItem.erase(iter_holditem);
												break;
											}
											else
											{
												iter_holditem++;
											}										
										}
									}
								}
								else//1:아이템고정끝
								{
								}				
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
							
							
						}
					}			
				}	
				else
				{
					// 실패일때 행동식에서 퀘스트 진행정보나 옵션 조건식등을 세팅한다.
					map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
					_XTriggerActionItem* currentaction = NULL;
					for(iter_exceptedaction = currentsuccesstrigger->m_mapExceptedActionList.begin() ; iter_exceptedaction != currentsuccesstrigger->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
					{
						currentaction = iter_exceptedaction->second;
						if(currentaction)
						{
							if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )// NPC 생성 삭제 다른 맵으로 넘어가면 다시 설정해야 한다.
							{								
								for( int _i = 0; _i < g_Npclist.m_NPCObjectCount; ++_i )
								{			
									if( g_Npclist.m_pNPCObjectArray[_i].m_UniqueID == currentaction->m_TDA.m_numericvalue1 )
									{
										if( g_Npclist.m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
										{
											if( currentaction->m_TDA.m_actiondata1 == 0 ) // ON
											{
												g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE;
											}
											else // OFF
											{
											//	g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = TRUE;
											}
											break;
										}	
									}
								}
								
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
							{				
								if( currentaction->m_TDA.m_actiondata2 == 0 )//0:아이템고정시작
								{
									// 아이템 고정 리스트가 있으면 취소 시킨다.
									if( !m_listHoldItem.empty() )
									{
										list <_XQUEST_HOLDITEM>::iterator iter_holditem;
										for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
										{
											_XQUEST_HOLDITEM questholditem = *iter_holditem;
											if( questholditem.m_cHoldItemType == currentaction->m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == currentaction->m_TDA.m_numericvalue1 )
											{
												iter_holditem = m_listHoldItem.erase(iter_holditem);
												break;
											}
											else
											{
												iter_holditem++;
											}										
										}
									}
								}
								else//1:아이템고정끝
								{
								}				
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
						}
					}
				}
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL _XQuestScriptManager::QuestHistorySetting(int questid, BYTE treestep)
{	
	_XQuestScriptItem* currentquest = NULL;
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}		
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}

	if( currentquest )
	{				
		// 시작 퀘스트 정보 세팅 
		if( treestep > 0 )
		{
			if(currentquest->m_startquestinformationmessage)
			{
				int strlength = strlen(currentquest->m_startquestinformationmessage);
				
				if(strlength > 0)
				{					
					currentquest->m_lpszQuestProgressinformation = currentquest->m_startquestinformationmessage;
				}					
			}
		}					

		int searchtreenode = 0;
		int selftreenode = 0;
		int currentquesttriggerid = 0;
		// 히스토리를 구현해서 필요한 정보들을 세팅한다. -> 진행정보, 옵션조건식 세팅....
		typedef std::stack<sHistory>	HistoryStack;
		HistoryStack			stackQuestHistory;
		
		map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treestep);
		if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
		{
			_XTriggerTreeItem* currenttree = iter_triggertree->second;
			if(currenttree)
			{				
				searchtreenode = currenttree->m_ParentsNodeUniqueID;
				selftreenode = currenttree->m_SelfNodeUniqueID;
			}
		}

		sHistory historystruct;
		do {				
			map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(searchtreenode);
			if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
			{
				_XTriggerTreeItem* currenttree = iter_triggertree->second;
				if(currenttree)
				{
					if( currenttree->m_TRightNodeUniqueID == selftreenode )
					{
						historystruct.bSuccess = TRUE;
					}
					else if( currenttree->m_FLeftNodeUniqueID == selftreenode )
					{
						historystruct.bSuccess = FALSE;
					}
					selftreenode = currenttree->m_SelfNodeUniqueID;
					searchtreenode = currenttree->m_ParentsNodeUniqueID;
					
					// 세팅으로 온 노드는 실행해야할 노드이기 때문에 서버, 클라이언트 완료 세팅을 하지 않는다.
					if( selftreenode == treestep )
					{
						currentquesttriggerid = currenttree->m_TriggerUniqueID;
					}
					historystruct.treenodenumber = selftreenode;
					historystruct.triggerid = currenttree->m_TriggerUniqueID;

					stackQuestHistory.push( historystruct );
				}
				else
				{
					return FALSE;
				}
			}	
			else
			{
				return FALSE;
			}
		} while( selftreenode != 1 );
		
		while (!stackQuestHistory.empty())
		{
			sHistory temphistorystruct = stackQuestHistory.top();
			stackQuestHistory.pop();

			int treenodenumber = temphistorystruct.treenodenumber;
			int triggerid = temphistorystruct.triggerid;
			BOOL bSuccess = temphistorystruct.bSuccess;
			
			// 세팅으로 온 노드는 실행해야할 노드이기 때문에 서버, 클라이언트 완료 세팅을 하지 않는다.
			if( currentquesttriggerid != triggerid )
			{
				currentquest->SetTriggerClientEnable(triggerid, TRUE);
				currentquest->SetTriggerServerEnable(triggerid, TRUE);
				currentquest->SetTriggerConfirmAll(triggerid, TRUE );
				currentquest->SetTriggerComplete(triggerid, TRUE);
			}
			

			map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(triggerid);
			_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
			if(currentsuccesstrigger)
			{
				// 성공일때 행동식에서 퀘스트 진행정보나 옵션 조건식등을 세팅한다.
				if( bSuccess )
				{
					map <int, _XTriggerActionItem*>::iterator iter_normalaction;
					_XTriggerActionItem* currentaction = NULL;
					for(iter_normalaction = currentsuccesstrigger->m_mapNormalActionList.begin() ; iter_normalaction != currentsuccesstrigger->m_mapNormalActionList.end() ; ++iter_normalaction)
					{
						currentaction = iter_normalaction->second;
						if(currentaction)
						{
							// 퀘스트 정보 
							if( currentaction->m_TDA.m_actionclass == ID_ACTION_QUESTPROGRESSINFO )
							{
								if( currentaction->m_lpszMessageString )
								{
									int strlength = strlen(currentaction->m_lpszMessageString);
									
									if(strlength > 0)
									{
										currentquest->m_lpszQuestProgressinformation = currentaction->m_lpszMessageString;										
									}					
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_STARTOPTIONCONDITION )
							{	
								
								_XTriggerConditionItem* currentconditiontrigger = NULL;
								map <int, _XTriggerConditionItem*>::iterator iter_trigger;
								iter_trigger = currentquest->m_mapOptionConditionList.find(currentaction->m_TDA.m_numericvalue1);
								if(iter_trigger != currentquest->m_mapOptionConditionList.end())
								{
									currentconditiontrigger = iter_trigger->second;	
									if(currentconditiontrigger)
									{				
										_XTriggerConditionItem* failcondition = new _XTriggerConditionItem;
										
										if(failcondition)
										{
											failcondition = currentconditiontrigger;
											
											if( currentaction->m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLESUCCESS )
											{
												failcondition->m_bSuccessCancle = TRUE;
											}
											else if( currentaction->m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLEFAIL )
											{
												failcondition->m_bSuccessCancle = FALSE;
											}

											currentquest->m_mapFailConditionList[ currentaction->m_TDA.m_numericvalue1 ] = failcondition;
										}
									}	
								}
								
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_ENDOPTIONCONDITION )
							{
								
								if( !currentquest->m_mapFailConditionList.empty() )
								{
									currentquest->m_mapFailConditionList.erase(currentaction->m_TDA.m_numericvalue1);
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETITMECOUNTERSTART )
							{
								if( currentaction->m_TDA.m_actiondata1 <= _XGI_FC_SOCKET )
								{
									bool bexist = false;
									for( int i = 0; i < 8; ++i )
									{
										if( currentquest->m_ItemCounterInformation[i].CounterTargetType == currentaction->m_TDA.m_actiondata1 
											&& currentquest->m_ItemCounterInformation[i].CounterTargetID == currentaction->m_TDA.m_numericvalue1
											&& currentquest->m_ItemCounterInformation[i].CountTotalPoint == currentaction->m_TDA.m_numericvalue2 )
										{
											bexist = true;
											break;
										}
									}
									
									if( !bexist )
									{	
										for( i = 0; i < 8; ++i )
										{
											if( currentquest->m_ItemCounterInformation[i].CounterTargetType == -1 )
											{
												currentquest->m_ItemCounterInformation[i].CounterTargetType = currentaction->m_TDA.m_actiondata1; // 타입
												currentquest->m_ItemCounterInformation[i].CounterTargetID = currentaction->m_TDA.m_numericvalue1;   // 아이디 
												currentquest->m_ItemCounterInformation[i].CountTotalPoint = currentaction->m_TDA.m_numericvalue2;	 // 총 개수 
												
												char cType = currentquest->m_ItemCounterInformation[i].CounterTargetType;
												short sID = currentquest->m_ItemCounterInformation[i].CounterTargetID;
												
												memset(currentquest->m_ItemCounterInformation[i].CounterTargetName, 0, sizeof(currentquest->m_ItemCounterInformation[i].CounterTargetName));
												strncpy(currentquest->m_ItemCounterInformation[i].CounterTargetName, _XGameItem::GetItemName(cType, sID), sizeof(currentquest->m_ItemCounterInformation[i].CounterTargetName) );
												
												break;
											}											
										}
									}
									if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
								}
								else
								{
									int countertype = currentaction->m_TDA.m_actiondata1 - _XGI_FC_SOCKET -1;
									currentquest->m_CounterInformation[countertype].CounterType = countertype;
									currentquest->m_CounterInformation[countertype].CounterTargetID = currentaction->m_TDA.m_numericvalue1;
									currentquest->m_CounterInformation[countertype].CountTotalPoint = currentaction->m_TDA.m_numericvalue2;
									
									if( currentaction->m_lpszMessageString )
									{
										int strlength = strlen(currentaction->m_lpszMessageString);
										
										if(strlength > 0)
										{
											memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
											strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, currentaction->m_lpszMessageString, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
										}		
										else
										{
											int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
											memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
											if( g_MonsterName[mobid] )
												strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid], sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName));
											else
												strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR), sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
										}
									}
									else
									{
										int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
										memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
										if( g_MonsterName[mobid] )
											strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid], sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
										else
											strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR), sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
									}
									if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_STOPCOUNTER )
							{								
								currentquest->ResetCounterInformation();
								currentquest->ResetPartyCounterInformation();
								currentquest->ResetItemCounterInformation();								
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_TIMERSTART )
							{			
								if( currentaction->m_TDA.m_actiondata1 == 0 )
								{
									if( currentaction->m_TDA.m_actiondata2 == 0 )//표시하기
									{
										m_TimerStartQuestID		= currentquest->GetQuestID();
										m_bStartQuestTimer		= TRUE;
										m_QuestTimerStartTime	= currentquest->m_mapTriggerTimer[treenodenumber];
										m_QuestTimerTimeLimit	= (60*currentaction->m_TDA.m_numericvalue1) + currentaction->m_TDA.m_numericvalue2;
									}
								}	
								else
								{
									currentquest->m_mapTriggerTimer[treenodenumber] = 0;
									
									m_TimerStartQuestID		= -1;
									m_bStartQuestTimer		= FALSE;
									m_QuestTimerStartTime	= 0;
									m_QuestTimerTimeLimit	= 0;
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )
							{								
								for( int _i = 0; _i < g_Npclist.m_NPCObjectCount; ++_i )
								{			
									if( g_Npclist.m_pNPCObjectArray[_i].m_UniqueID == currentaction->m_TDA.m_numericvalue1 )
									{
										if( g_Npclist.m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
										{
											if( currentaction->m_TDA.m_actiondata1 == 0 ) // ON
											{
												g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = TRUE;
											}
											else // OFF
											{
												g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE;
											}
											break;
										}	
									}
								}
								
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
							{				
								if( currentaction->m_TDA.m_actiondata2 == 0 )//0:아이템고정시작
								{
									if( !m_listHoldItem.empty() )
									{
										_XQUEST_HOLDITEM	questholditem;
										memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
										questholditem.m_cHoldItemType = currentaction->m_TDA.m_actiondata1;
										questholditem.m_sHoldItemID = currentaction->m_TDA.m_numericvalue1;
										m_listHoldItem.push_back(questholditem);
									}
									else
									{
										// 먼저 아이템 고정 리스트에 존재하는지 검사
										bool alreadyexist = false;
										list <_XQUEST_HOLDITEM>::iterator iter_holditem;
										for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
										{
											_XQUEST_HOLDITEM questholditem = *iter_holditem;
											if( questholditem.m_cHoldItemType == currentaction->m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == currentaction->m_TDA.m_numericvalue1 )
											{
												alreadyexist = true;
												break;
											}
											else
											{
												iter_holditem++;
											}										
										}
										
										// 존재하고 있지 않다면 리스트에 추가
										if( !alreadyexist )
										{
											_XQUEST_HOLDITEM	questholditem;
											memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
											questholditem.m_cHoldItemType = currentaction->m_TDA.m_actiondata1;
											questholditem.m_sHoldItemID = currentaction->m_TDA.m_numericvalue1;
											m_listHoldItem.push_back(questholditem);
										}
									}
								}
								else//1:아이템고정끝
								{
									if( !m_listHoldItem.empty() )
									{
										list <_XQUEST_HOLDITEM>::iterator iter_holditem;
										for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
										{
											_XQUEST_HOLDITEM questholditem = *iter_holditem;
											if( questholditem.m_cHoldItemType == currentaction->m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == currentaction->m_TDA.m_numericvalue1 )
											{
												iter_holditem = m_listHoldItem.erase(iter_holditem);
												break;
											}
											else
											{
												iter_holditem++;
											}										
										}
									}
								}				
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
							
							
						}
					}			
				}	
				else
				{
					// 실패일때 행동식에서 퀘스트 진행정보나 옵션 조건식등을 세팅한다.
					map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
					_XTriggerActionItem* currentaction = NULL;
					for(iter_exceptedaction = currentsuccesstrigger->m_mapExceptedActionList.begin() ; iter_exceptedaction != currentsuccesstrigger->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
					{
						currentaction = iter_exceptedaction->second;
						if(currentaction)
						{
							if( currentaction->m_TDA.m_actionclass == ID_ACTION_QUESTPROGRESSINFO )
							{
								if( currentaction->m_lpszMessageString )
								{
									int strlength = strlen(currentaction->m_lpszMessageString);
									
									if(strlength > 0)
									{
										currentquest->m_lpszQuestProgressinformation = currentaction->m_lpszMessageString;										
									}					
								}
							}							
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_STARTOPTIONCONDITION )
							{			
								_XTriggerConditionItem* currentconditiontrigger = NULL;
								map <int, _XTriggerConditionItem*>::iterator iter_trigger;
								iter_trigger = currentquest->m_mapOptionConditionList.find(currentaction->m_TDA.m_numericvalue1);
								if(iter_trigger != currentquest->m_mapOptionConditionList.end())
								{
									currentconditiontrigger = iter_trigger->second;	
									if(currentconditiontrigger)
									{				
										_XTriggerConditionItem* failcondition = new _XTriggerConditionItem;
										
										if(failcondition)
										{
											failcondition = currentconditiontrigger;
											
											if( currentaction->m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLESUCCESS )
											{
												failcondition->m_bSuccessCancle = TRUE;
											}
											else if( currentaction->m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLEFAIL )
											{
												failcondition->m_bSuccessCancle = FALSE;
											}
											
											currentquest->m_mapFailConditionList[ currentaction->m_TDA.m_numericvalue1 ] = failcondition;
										}
									}	
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_ENDOPTIONCONDITION )
							{
								
								if( !currentquest->m_mapFailConditionList.empty() )
								{
									currentquest->m_mapFailConditionList.erase(currentaction->m_TDA.m_numericvalue1);
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETITMECOUNTERSTART )
							{
								if( currentaction->m_TDA.m_actiondata1 <= _XGI_FC_SOCKET )
								{
									bool bexist = false;
									for( int i = 0; i < 8; ++i )
									{
										if( currentquest->m_ItemCounterInformation[i].CounterTargetType == currentaction->m_TDA.m_actiondata1 
											&& currentquest->m_ItemCounterInformation[i].CounterTargetID == currentaction->m_TDA.m_numericvalue1
											&& currentquest->m_ItemCounterInformation[i].CountTotalPoint == currentaction->m_TDA.m_numericvalue2 )
										{
											bexist = true;
											break;
										}
									}
									
									if( !bexist )
									{	
										for( i = 0; i < 8; ++i )
										{
											if( currentquest->m_ItemCounterInformation[i].CounterTargetType == -1 )
											{
												currentquest->m_ItemCounterInformation[i].CounterTargetType = currentaction->m_TDA.m_actiondata1; // 타입
												currentquest->m_ItemCounterInformation[i].CounterTargetID = currentaction->m_TDA.m_numericvalue1;   // 아이디 
												currentquest->m_ItemCounterInformation[i].CountTotalPoint = currentaction->m_TDA.m_numericvalue2;	 // 총 개수 
												
												char cType = currentquest->m_ItemCounterInformation[i].CounterTargetType;
												short sID = currentquest->m_ItemCounterInformation[i].CounterTargetID;
												
												memset(currentquest->m_ItemCounterInformation[i].CounterTargetName, 0, sizeof(currentquest->m_ItemCounterInformation[i].CounterTargetName));
												strncpy(currentquest->m_ItemCounterInformation[i].CounterTargetName, _XGameItem::GetItemName(cType, sID) , sizeof(currentquest->m_ItemCounterInformation[i].CounterTargetName));
												
												break;
											}											
										}
									}
									if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
								}
								else
								{
									int countertype = currentaction->m_TDA.m_actiondata1 - _XGI_FC_SOCKET -1;
									currentquest->m_CounterInformation[countertype].CounterType = countertype;
									currentquest->m_CounterInformation[countertype].CounterTargetID = currentaction->m_TDA.m_numericvalue1;
									currentquest->m_CounterInformation[countertype].CountTotalPoint = currentaction->m_TDA.m_numericvalue2;
									
									if( currentaction->m_lpszMessageString )
									{
										int strlength = strlen(currentaction->m_lpszMessageString);
										
										if(strlength > 0)
										{
											memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
											strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, currentaction->m_lpszMessageString, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );			
										}		
										else
										{
											int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
											memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
											if( g_MonsterName[mobid] )
												strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid], sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
											else
												strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR), sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
										}
									}
									else
									{
										int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
										memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
										if( g_MonsterName[mobid] )
											strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid], sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
										else
											strncpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR), sizeof(currentquest->m_CounterInformation[countertype].CounterTargetName) );
									}
									if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_STOPCOUNTER )// 카운터를 정지시킨다.
							{								
								currentquest->ResetCounterInformation();	
								currentquest->ResetPartyCounterInformation();
								currentquest->ResetItemCounterInformation();								
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_TIMERSTART )
							{			
								if( currentaction->m_TDA.m_actiondata1 == ID_QUEST_SOUNDTARGET_START )
								{
									if( currentaction->m_TDA.m_actiondata2 == 0 )//표시하기
									{
										m_TimerStartQuestID		= currentquest->GetQuestID();
										m_bStartQuestTimer		= TRUE;
										m_QuestTimerStartTime	= currentquest->m_mapTriggerTimer[treenodenumber];
										m_QuestTimerTimeLimit	= (60*currentaction->m_TDA.m_numericvalue1) + currentaction->m_TDA.m_numericvalue2;
									}
								}	
								else
								{
									currentquest->m_mapTriggerTimer[treenodenumber] = 0;
									
									m_TimerStartQuestID		= -1;
									m_bStartQuestTimer		= FALSE;
									m_QuestTimerStartTime	= 0;
									m_QuestTimerTimeLimit	= 0;
								}
							}
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )// NPC 생성 삭제 다른 맵으로 넘어가면 다시 설정해야 한다.
							{								
								for( int _i = 0; _i < g_Npclist.m_NPCObjectCount; ++_i )
								{			
									if( g_Npclist.m_pNPCObjectArray[_i].m_UniqueID == currentaction->m_TDA.m_numericvalue1 )
									{
										if( g_Npclist.m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
										{
											if( currentaction->m_TDA.m_actiondata1 == 0 ) // ON
											{
												g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = TRUE;
											}
											else // OFF
											{
												g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE;
											}
											break;
										}	
									}
								}
								
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_SETNPCONOFF )
							else if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
							{				
								if( currentaction->m_TDA.m_actiondata2 == 0 )//0:아이템고정시작
								{
									if( !m_listHoldItem.empty() )
									{
										_XQUEST_HOLDITEM	questholditem;
										memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
										questholditem.m_cHoldItemType = currentaction->m_TDA.m_actiondata1;
										questholditem.m_sHoldItemID = currentaction->m_TDA.m_numericvalue1;
										m_listHoldItem.push_back(questholditem);
									}
									else
									{
										// 먼저 아이템 고정 리스트에 존재하는지 검사
										bool alreadyexist = false;
										list <_XQUEST_HOLDITEM>::iterator iter_holditem;
										for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
										{
											_XQUEST_HOLDITEM questholditem = *iter_holditem;
											if( questholditem.m_cHoldItemType == currentaction->m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == currentaction->m_TDA.m_numericvalue1 )
											{
												alreadyexist = true;
												break;
											}
											else
											{
												iter_holditem++;
											}										
										}
										
										// 존재하고 있지 않다면 리스트에 추가
										if( !alreadyexist )
										{
											_XQUEST_HOLDITEM	questholditem;
											memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
											questholditem.m_cHoldItemType = currentaction->m_TDA.m_actiondata1;
											questholditem.m_sHoldItemID = currentaction->m_TDA.m_numericvalue1;
											m_listHoldItem.push_back(questholditem);
										}
									}
								}
								else//1:아이템고정끝
								{
									if( !m_listHoldItem.empty() )
									{
										list <_XQUEST_HOLDITEM>::iterator iter_holditem;
										for(iter_holditem = m_listHoldItem.begin() ; iter_holditem != m_listHoldItem.end() ; )
										{
											_XQUEST_HOLDITEM questholditem = *iter_holditem;
											if( questholditem.m_cHoldItemType == currentaction->m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == currentaction->m_TDA.m_numericvalue1 )
											{
												iter_holditem = m_listHoldItem.erase(iter_holditem);
												break;
											}
											else
											{
												iter_holditem++;
											}										
										}
									}
								}				
							}//if( currentaction->m_TDA.m_actionclass == ID_ACTION_HOLDITEM )
						}
					}
				}
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void _XQuestScriptManager::QuestRunningTimerSetting(int questid, int time, int treenumber, int triggertimer)
{
	_XQuestScriptItem* currentquest = NULL;
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}

	if(currentquest)
	{
		currentquest->SetStartTime(time);	
		map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treenumber);
		if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
		{
			_XTriggerTreeItem* currenttree = iter_triggertree->second;
			if(currenttree)
			{		
				currentquest->m_mapTriggerTimer[treenumber] = triggertimer;
				return;
			}
		}
	}
}

void _XQuestScriptManager::InsertRewordQuestList(int questid, int treenumber, int needinven)
{
	_XQuestScriptItem* currentquest = NULL;
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}	

	if(currentquest)
	{			
		map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treenumber);
		if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
		{
			_XTriggerTreeItem* currenttree = iter_triggertree->second;
			if(currenttree)
			{
				map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = m_mapRewordQuestList.find(questid);
				if(iter_rewordquest != m_mapRewordQuestList.end())
				{
					m_mapRewordQuestList[questid].triggerid = currenttree->m_TriggerUniqueID;
					m_mapRewordQuestList[questid].needinven = needinven;
					m_mapRewordQuestList[questid].starttime = g_ServerTimeCode;
				}
				else
				{
					m_mapRewordQuestList[questid].triggerid = currenttree->m_TriggerUniqueID;
					m_mapRewordQuestList[questid].needinven = needinven;
					m_mapRewordQuestList[questid].starttime = g_ServerTimeCode;
				}

				map <int, _XTriggerItem*>::iterator iter_trigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
				if(iter_trigger != currentquest->m_mapSuccessTriggerList.end())
				{
					_XTriggerItem* currenttrigger = iter_trigger->second;
					if(currenttrigger)
					{
						currenttrigger->m_bWaitRewordPacket = FALSE;
					}
				}

			}
		}

	}

}

void _XQuestScriptManager::DeleteRewordQuestList(int questid)
{
	map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = m_mapRewordQuestList.find(questid);
	if(iter_rewordquest != m_mapRewordQuestList.end())
	{
		m_mapRewordQuestList.erase(iter_rewordquest);
	}
}

void _XQuestScriptManager::SetCheckQuestID(int questid)
{
	m_nCheckQuestID = questid;
}

int _XQuestScriptManager::GetCheckQuestID(void)
{
	return m_nCheckQuestID;
}

void _XQuestScriptManager::SetCheckQuest(BOOL check)
{
	m_bCheckQuest = check;
}

BOOL _XQuestScriptManager::GetCheckQuest(void)
{
	return m_bCheckQuest;
}

void _XQuestScriptManager::ClearOriginalCameraInfo(void)
{
	memset(&m_OriginalCameraInfo, 0, sizeof(_XCAMERA_ORIGINALINFO));
}


void _XQuestScriptManager::SetQuestWideView(BOOL bWide)
{
	m_bQuestWideView = bWide;
}

BOOL _XQuestScriptManager::GetQuestWideView(void)
{
	return m_bQuestWideView;
}


void _XQuestScriptManager::SetStartMessage(int questid)
{
//	_XQuestScriptItem* currentquest = NULL;
//	if( questid >= 10000 && questid < 20000 )
//	{
//		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
//		if(iter != m_mapQuestScriptList.end())
//		{
//			currentquest = iter->second;
//		}
//	}
//	else
//	{
//		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
//		if(iter != m_mapSubQuestScriptList.end())
//		{
//			currentquest = iter->second;
//		}
//	}	
//	
//	if(currentquest)
//	{
//		int questownerid = currentquest->GetQuestOwnerNPC();
//		for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; i++)
//		{
//			if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == questownerid)
//			{
//				g_Npclist.m_pNPCObjectArray[i].SetMessageType(_XDEF_NPCMESSAGE_QUEST);
//				g_Npclist.m_pNPCObjectArray[i].SetChangeMessage(TRUE);
//				g_Npclist.m_pNPCObjectArray[i].SetChangeButton(FALSE);
//				g_Npclist.m_pNPCObjectArray[i].SetMessageString(currentquest->m_lpszStartQuestMessage);
//				g_Npclist.m_pNPCObjectArray[i].SetHaveQuestID(questid);
//				
//				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CHECKING)     >>>>>>>>>>>>>>>>>>>>" );						
//				_XLog("Setting Quest NPCDialogMode : QuestID[%d] NPCID[%d]" , questid,  questownerid );
//				
//				break;
//			}
//		}
//	}

}

void _XQuestScriptManager::SetProgressMessage(int questid, int npcid, LPCTSTR string)
{
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{
		if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid)
		{
			g_Npclist.m_pNPCObjectArray[i].SetMessageType(_XDEF_NPCMESSAGE_QUEST);
			g_Npclist.m_pNPCObjectArray[i].SetChangeMessage(TRUE);
			g_Npclist.m_pNPCObjectArray[i].SetChangeButton(FALSE);
			g_Npclist.m_pNPCObjectArray[i].SetMessageString(string);
			g_Npclist.m_pNPCObjectArray[i].SetHaveQuestID(questid);		
			
			//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CHECKING)     >>>>>>>>>>>>>>>>>>>>" );					
			//_XLog("Setting Quest NPCDialogMode : QuestID[%d] NPCID[%d]" , questid,  npcid );
			
			break;
		}
	}
}

void _XQuestScriptManager::SetFailMessage(int questid)
{
	
//	_XQuestScriptItem* currentquest = NULL;
//	if( questid >= 10000 && questid < 20000 )
//	{
//		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
//		if(iter != m_mapQuestScriptList.end())
//		{
//			currentquest = iter->second;
//		}
//	}
//	else
//	{
//		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
//		if(iter != m_mapSubQuestScriptList.end())
//		{
//			currentquest = iter->second;
//		}
//	}	
//	
//	if(currentquest)
//	{
//		int questownerid = currentquest->GetQuestOwnerNPC();
//		for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; i++)
//		{
//			if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == questownerid)
//			{
//				g_Npclist.m_pNPCObjectArray[i].SetMessageType(_XDEF_NPCMESSAGE_QUEST);
//				g_Npclist.m_pNPCObjectArray[i].SetChangeMessage(TRUE);
//				g_Npclist.m_pNPCObjectArray[i].SetChangeButton(FALSE);
//				g_Npclist.m_pNPCObjectArray[i].SetMessageString(currentquest->m_lpszFailQuestMessage);
//				g_Npclist.m_pNPCObjectArray[i].SetHaveQuestID(questid);
//				
//				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CHECKING)     >>>>>>>>>>>>>>>>>>>>" );					
//				_XLog("Setting Quest NPCDialogMode : QuestID[%d] NPCID[%d]" , questid,  questownerid );
//				
//				break;
//			}
//		}
//	}


}

int _XQuestScriptManager::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;
	
	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return -1;
	
	if(string)
	{
		delete[] string;
		string = NULL;
	}
	
	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
			return -1;
		
		memset(string, 0, sizeof(TCHAR)*(strlength + 1));
		if(fread(string, strlength , 1, fileptr) < 1)
			return -1;
	}
	return strlength;
}

void _XQuestScriptManager::QuestStartConditionNPCTargettingMode()
{		
	// 제룡행 퀘스트의 시작 조건을 검사한다. 
	if( m_CurrentMainQuestID != -1 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				// NPC 타게팅이 들어간 시작 조건이면 검사시작
				if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_NPCTARGETTING )
				{
					// 퀘스트가 완료되지 않았으면 검사
					if(!currentquest->GetComplete())
					{
						currentquest->CheckStartCondition();
					}
				}
			}
		}
	}

	// 진행중인 퀘스트가 6개 넘으면 제룡행이 아닌 퀘스트의 시작 조건을 검사를 하지 않음.
	if( !CheckProgressQuestCount() ) 
	{
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	// 보스전에 들어가 있으면 통과
			return;
#else
		return;
#endif
	}
	
	// 무림행 협객행 기연행 무분류 퀘스트 시작조건 검사 	
	if( !m_listQuestStartTypeNPCTargetting.empty() )
	{		
		list <int>::iterator iter_queststarttype;
		iter_queststarttype = m_listQuestStartTypeNPCTargetting.begin();
		while( iter_queststarttype != m_listQuestStartTypeNPCTargetting.end() )
		{			
			int questid = *iter_queststarttype;			
			if( questid != -1 )
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
				if(iter_quest != m_mapSubQuestScriptList.end())
				{
					_XQuestScriptItem* currentquest = iter_quest->second;
					if(currentquest)
					{
						// 영역 검사가 들어간 시작 조건이면 검사시작
						if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_NPCTARGETTING  )
						{
							// 퀘스트가 완료되지 않았으면 검사
							if(!currentquest->GetComplete())
							{
								
								if( currentquest->CheckStartCondition() ==  QUEST_STARTCONDITION_RESULT_NOTIFYSERVER )
								{
									iter_queststarttype = m_listQuestStartTypeNPCTargetting.erase(iter_queststarttype);
									
									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_NPCTARGETTING ] ", questid );	
									
								}
								else if( currentquest->m_bChangeQuestStartMode )
								{
									iter_queststarttype = m_listQuestStartTypeNPCTargetting.erase(iter_queststarttype);

									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_NPCTARGETTING ] ", questid );	
									
								}
								else
								{
									iter_queststarttype++;
								}
							}
							else
							{
								iter_queststarttype = m_listQuestStartTypeNPCTargetting.erase(iter_queststarttype);
								
								//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
								//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_NPCTARGETTING ] ", questid );	
								
							}							
						}
						else
						{
							iter_queststarttype = m_listQuestStartTypeNPCTargetting.erase(iter_queststarttype);
							
							//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
							//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_NPCTARGETTING ] ", questid );	
							
						}
					}
					else// if(currentquest)
					{
						iter_queststarttype = m_listQuestStartTypeNPCTargetting.erase(iter_queststarttype);
						
						//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
						//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_NPCTARGETTING ] ", questid );	
						
					}
				}
				else
				{
					iter_queststarttype++;
				}
			}
			else// if( questid != -1 )
			{
				iter_queststarttype = m_listQuestStartTypeNPCTargetting.erase(iter_queststarttype);
				
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_NPCTARGETTING ] ", questid );	
				
			}
		}
	}//if( !m_listQuestStartTypeNPCTargetting.empty() )
	

}

void _XQuestScriptManager::QuestStartConditionObjectClickMode()
{	
	// 제룡행 퀘스트의 시작 조건을 검사한다. 
	if( m_CurrentMainQuestID != -1 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				// OBJECT 클릭이 들어간 시작 조건이면 검사시작
				if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_OBJECTCLICK)
				{
					// 퀘스트가 완료되지 않았으면 검사
					if(!currentquest->GetComplete())
					{
						 currentquest->CheckStartCondition();
					}
				}
			}
		}
	}
	
	// 진행중인 퀘스트가 6개 넘으면 제룡행이 아닌 퀘스트의 시작 조건을 검사를 하지 않음.
	if( !CheckProgressQuestCount() ) 
	{
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	// 보스전에 들어가 있으면 통과
			return;
#else
		return;
#endif
	}
	
	// 무림행 협객행 기연행 무분류 퀘스트 시작조건 검사 	
	if( !m_listQuestStartTypeObjectClick.empty() )
	{		
		list <int>::iterator iter_queststarttype;
		iter_queststarttype = m_listQuestStartTypeObjectClick.begin();
		while( iter_queststarttype != m_listQuestStartTypeObjectClick.end() )
		{			
			int questid = *iter_queststarttype;			
			if( questid != -1 )
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
				if(iter_quest != m_mapSubQuestScriptList.end())
				{
					_XQuestScriptItem* currentquest = iter_quest->second;
					if(currentquest)
					{
						// 영역 검사가 들어간 시작 조건이면 검사시작
						if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_OBJECTCLICK  )
						{
							// 퀘스트가 완료되지 않았으면 검사
							if(!currentquest->GetComplete())
							{
								if( currentquest->GetStartTime() == 0)
								{
									
									
									if( currentquest->CheckStartCondition() ==  QUEST_STARTCONDITION_RESULT_NOTIFYSERVER )
									{
										iter_queststarttype = m_listQuestStartTypeObjectClick.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_OBJECTCLICK ] ", questid );	
										
									}
									else if( currentquest->m_bChangeQuestStartMode )
									{
										iter_queststarttype = m_listQuestStartTypeObjectClick.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_OBJECTCLICK ] ", questid );	
										
									}
									else
									{
										iter_queststarttype++;
									}
								}
								else
								{
									iter_queststarttype = m_listQuestStartTypeObjectClick.erase(iter_queststarttype);
									
									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_OBJECTCLICK ] ", questid );	
									
								}
							}
							else
							{
								iter_queststarttype = m_listQuestStartTypeObjectClick.erase(iter_queststarttype);
								
								//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
								//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_OBJECTCLICK ] ", questid );	
								
							}							
						}
						else
						{
							iter_queststarttype = m_listQuestStartTypeObjectClick.erase(iter_queststarttype);
							
							//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
							//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_OBJECTCLICK ] ", questid );	
							
						}
					}
					else// if(currentquest)
					{
						iter_queststarttype = m_listQuestStartTypeObjectClick.erase(iter_queststarttype);
						
						//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
						//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_OBJECTCLICK ] ", questid );	
						
					}
				}
				else
				{
					iter_queststarttype++;
				}
			}
			else// if( questid != -1 )
			{
				iter_queststarttype = m_listQuestStartTypeObjectClick.erase(iter_queststarttype);
				
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_OBJECTCLICK ] ", questid );	
				
			}
		}

	}//if( !m_listQuestStartTypeObjectClick.empty() )
	
}

void _XQuestScriptManager::QuestStartConditionUsingItemMode()
{
	// 제룡행 퀘스트의 시작 조건을 검사한다. 
	if( m_CurrentMainQuestID != -1 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				// 아이템 사용검사가 들어간 시작 조건이면 검사시작
				if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_USINGITEM)
				{
					// 퀘스트가 완료되지 않았으면 검사
					if(!currentquest->GetComplete())
					{
						currentquest->CheckStartCondition();
					}
				}
			}
		}
	}
	
	// 진행중인 퀘스트가 6개 넘으면 제룡행이 아닌 퀘스트의 시작 조건을 검사를 하지 않음.
	if( !CheckProgressQuestCount() ) 
	{
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	// 보스전에 들어가 있으면 통과
			return;
#else
		return;
#endif
	}
		
	// 무림행 협객행 기연행 무분류 퀘스트 시작조건 검사 
	if( !m_listrQuestStartTypeUsingItem.empty() )
	{		
		list <int>::iterator iter_queststarttype;
		iter_queststarttype = m_listrQuestStartTypeUsingItem.begin();
		while( iter_queststarttype != m_listrQuestStartTypeUsingItem.end() )
		{			
			int questid = *iter_queststarttype;			
			if( questid != -1 )
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
				if(iter_quest != m_mapSubQuestScriptList.end())
				{
					_XQuestScriptItem* currentquest = iter_quest->second;
					if(currentquest)
					{
						// 영역 검사가 들어간 시작 조건이면 검사시작
						if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_USINGITEM  )
						{
							// 퀘스트가 완료되지 않았으면 검사
							if(!currentquest->GetComplete())
							{								
								if( currentquest->GetStartTime() == 0)
								{
									if( currentquest->CheckStartCondition() ==  QUEST_STARTCONDITION_RESULT_NOTIFYSERVER )
									{
										iter_queststarttype = m_listrQuestStartTypeUsingItem.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_USINGITEM ] ", questid );	
										
									}
									else if( currentquest->m_bChangeQuestStartMode )
									{
										iter_queststarttype = m_listrQuestStartTypeUsingItem.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_USINGITEM ] ", questid );	
										
									}
									else
									{
										iter_queststarttype++;
									}
								}
								else
								{
									iter_queststarttype = m_listrQuestStartTypeUsingItem.erase(iter_queststarttype);
									
									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_USINGITEM ] ", questid );	
									
								}
							}
							else
							{
								iter_queststarttype = m_listrQuestStartTypeUsingItem.erase(iter_queststarttype);
								
								//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
								//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_USINGITEM ] ", questid );	
								
							}							
						}
						else
						{
							iter_queststarttype = m_listrQuestStartTypeUsingItem.erase(iter_queststarttype);
							
							//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
							//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_USINGITEM ] ", questid );	
							
						}
					}
					else// if(currentquest)
					{
						iter_queststarttype = m_listrQuestStartTypeUsingItem.erase(iter_queststarttype);
						
						//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
						//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_USINGITEM ] ", questid );	
						
					}
				}
				else
				{
					iter_queststarttype++;
				}
			}
			else// if( questid != -1 )
			{
				iter_queststarttype = m_listrQuestStartTypeUsingItem.erase(iter_queststarttype);
				
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_USINGITEM ] ", questid );	
				
			}
		}
	}//if( !m_listrQuestStartTypeUsingItem.empty() )

}

void _XQuestScriptManager::QuestStartConditionWearItemMode()
{	
	// 제룡행 퀘스트의 시작 조건을 검사한다. 
	if( m_CurrentMainQuestID != -1 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				// 아이템 착용 검사가 들어간 시작 조건이면 검사시작
				if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_WEARITEM)
				{
					// 퀘스트가 완료되지 않았으면 검사
					if(!currentquest->GetComplete())
					{
						currentquest->CheckStartCondition();
					}
				}
			}
		}
	}
	
	// 진행중인 퀘스트가 6개 넘으면 제룡행이 아닌 퀘스트의 시작 조건을 검사를 하지 않음.
	if( !CheckProgressQuestCount() ) 
	{
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	// 보스전에 들어가 있으면 통과
			return;
#else
		return;
#endif
	}
		
	// 무림행 협객행 기연행 무분류 퀘스트 시작조건 검사 
	if( !m_listQuestStartTypeWearItem.empty() )
	{		
		list <int>::iterator iter_queststarttype;		
		iter_queststarttype = m_listQuestStartTypeWearItem.begin();
		while( iter_queststarttype != m_listQuestStartTypeWearItem.end() )
		{			
			int questid = *iter_queststarttype;			
			if( questid != -1 )
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
				if(iter_quest != m_mapSubQuestScriptList.end())
				{
					_XQuestScriptItem* currentquest = iter_quest->second;
					if(currentquest)
					{
						// 영역 검사가 들어간 시작 조건이면 검사시작
						if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_WEARITEM  )
						{
							// 퀘스트가 완료되지 않았으면 검사
							if(!currentquest->GetComplete())
							{				
								if( currentquest->GetStartTime() == 0)
								{
									if( currentquest->CheckStartCondition() ==  QUEST_STARTCONDITION_RESULT_NOTIFYSERVER )
									{
										iter_queststarttype = m_listQuestStartTypeWearItem.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_WEARITEM ] ", questid );	
										
									}
									else if( currentquest->m_bChangeQuestStartMode )
									{
										iter_queststarttype = m_listQuestStartTypeWearItem.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_WEARITEM ] ", questid );	
										
									}
									else
									{
										iter_queststarttype++;
									}
								}
								else
								{
									iter_queststarttype = m_listQuestStartTypeWearItem.erase(iter_queststarttype);
									
									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_WEARITEM ] ", questid );	
									
								}
							}
							else
							{
								iter_queststarttype = m_listQuestStartTypeWearItem.erase(iter_queststarttype);
								
								//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
								//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_WEARITEM ] ", questid );	
								
							}							
						}
						else
						{
							iter_queststarttype = m_listQuestStartTypeWearItem.erase(iter_queststarttype);
							
							//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
							//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_WEARITEM ] ", questid );	
							
						}
					}
					else// if(currentquest)
					{
						iter_queststarttype = m_listQuestStartTypeWearItem.erase(iter_queststarttype);
						
						//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
						//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_WEARITEM ] ", questid );	
						
					}
				}
				else
				{
					iter_queststarttype++;
				}
			}
			else// if( questid != -1 )
			{
				iter_queststarttype = m_listQuestStartTypeWearItem.erase(iter_queststarttype);
				
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_WEARITEM ] ", questid );	
				
			}
		}
	}//if( !m_listQuestStartTypeWearItem.empty() )

}

void _XQuestScriptManager::QuestStartConditionDestroyItemMode()
{	
	// 제룡행 퀘스트의 시작 조건을 검사한다. 
	if( m_CurrentMainQuestID != -1 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				// 아이템 착용 검사가 들어간 시작 조건이면 검사시작
				if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_DESTROYITEM)
				{
					// 퀘스트가 완료되지 않았으면 검사
					if(!currentquest->GetComplete())
					{
						currentquest->CheckStartCondition();
					}
				}
			}
		}
	}
	
	// 진행중인 퀘스트가 6개 넘으면 제룡행이 아닌 퀘스트의 시작 조건을 검사를 하지 않음.
	if( !CheckProgressQuestCount() ) 
	{
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	// 보스전에 들어가 있으면 통과
			return;
#else
		return;
#endif
	}
		
	// 무림행 협객행 기연행 무분류 퀘스트 시작조건 검사 
	if( !m_listQuestStartTypeDestroyItem.empty() )
	{		
		list <int>::iterator iter_queststarttype;		
		iter_queststarttype = m_listQuestStartTypeDestroyItem.begin();
		while( iter_queststarttype != m_listQuestStartTypeDestroyItem.end() )
		{			
			int questid = *iter_queststarttype;			
			if( questid != -1 )
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
				if(iter_quest != m_mapSubQuestScriptList.end())
				{
					_XQuestScriptItem* currentquest = iter_quest->second;
					if(currentquest)
					{
						// 영역 검사가 들어간 시작 조건이면 검사시작
						if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_DESTROYITEM  )
						{
							// 퀘스트가 완료되지 않았으면 검사
							if(!currentquest->GetComplete())
							{				
								if( currentquest->GetStartTime() == 0)
								{
									if( currentquest->CheckStartCondition() ==  QUEST_STARTCONDITION_RESULT_NOTIFYSERVER )
									{
										iter_queststarttype = m_listQuestStartTypeDestroyItem.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_DESTROYITEM ] ", questid );	
										
									}
									else if( currentquest->m_bChangeQuestStartMode )
									{
										iter_queststarttype = m_listQuestStartTypeDestroyItem.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_DESTROYITEM ] ", questid );	
										
									}
									else
									{
										iter_queststarttype++;
									}
								}
								else
								{
									iter_queststarttype = m_listQuestStartTypeDestroyItem.erase(iter_queststarttype);
									
									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_DESTROYITEM ] ", questid );	
									
								}
							}
							else
							{
								iter_queststarttype = m_listQuestStartTypeDestroyItem.erase(iter_queststarttype);
								
								//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
								//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_DESTROYITEM ] ", questid );	
								
							}							
						}
						else
						{
							iter_queststarttype = m_listQuestStartTypeDestroyItem.erase(iter_queststarttype);
							
							//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
							//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_DESTROYITEM ] ", questid );	
							
						}
					}
					else// if(currentquest)
					{
						iter_queststarttype = m_listQuestStartTypeDestroyItem.erase(iter_queststarttype);
						
						//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
						//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_DESTROYITEM ] ", questid );	
						
					}
				}
				else
				{
					iter_queststarttype++;
				}
			}
			else// if( questid != -1 )
			{
				iter_queststarttype = m_listQuestStartTypeDestroyItem.erase(iter_queststarttype);
				
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_DESTROYITEM ] ", questid );	
				
			}
		}
	}//if( !m_listQuestStartTypeWearItem.empty() )

}

void _XQuestScriptManager::QuestStartConditionLevelMode()
{
	// 제룡행 퀘스트의 시작 조건을 검사한다. 
	if( m_CurrentMainQuestID != -1 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				// LEVEL 검사가 들어간 시작 조건이면 검사시작
				if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_LEVEL)
				{
					// 퀘스트가 완료되지 않았으면 검사
					if(!currentquest->GetComplete())
					{
						currentquest->CheckStartCondition();
					}
				}
			}
		}
	}
	
	// 진행중인 퀘스트가 6개 넘으면 제룡행이 아닌 퀘스트의 시작 조건을 검사를 하지 않음.
	if( !CheckProgressQuestCount() ) 
	{
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	// 보스전에 들어가 있으면 통과
			return;
#else
		return;
#endif
	}
	
	// 무림행 협객행 기연행 무분류 퀘스트 시작조건 검사 	
	if( !m_listQuestStartTypeLevel.empty() )
	{		
		list <int>::iterator iter_queststarttype;		
		iter_queststarttype = m_listQuestStartTypeLevel.begin();
		while( iter_queststarttype != m_listQuestStartTypeLevel.end() )
		{			
			int questid = *iter_queststarttype;			
			if( questid != -1 )
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
				if(iter_quest != m_mapSubQuestScriptList.end())
				{
					_XQuestScriptItem* currentquest = iter_quest->second;
					if(currentquest)
					{
						// 영역 검사가 들어간 시작 조건이면 검사시작
						if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_LEVEL  )
						{
							// 퀘스트가 완료되지 않았으면 검사
							if(!currentquest->GetComplete())
							{
								if( currentquest->GetStartTime() == 0)
								{
									if( currentquest->CheckStartCondition() ==  QUEST_STARTCONDITION_RESULT_NOTIFYSERVER )
									{
										iter_queststarttype = m_listQuestStartTypeLevel.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_LEVEL ] ", questid );	
										
									}
									else if( currentquest->m_bChangeQuestStartMode )
									{
										iter_queststarttype = m_listQuestStartTypeLevel.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_LEVEL ] ", questid );	
										
									}
									else
									{
										iter_queststarttype++;
									}
								}
								else
								{
									iter_queststarttype = m_listQuestStartTypeLevel.erase(iter_queststarttype);
									
									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_LEVEL ] ", questid );	
									
								}
							}
							else
							{
								iter_queststarttype = m_listQuestStartTypeLevel.erase(iter_queststarttype);
								
								//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
								//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_LEVEL ] ", questid );	
								
							}							
						}
						else
						{
							iter_queststarttype = m_listQuestStartTypeLevel.erase(iter_queststarttype);
							
							//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
							//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_LEVEL ] ", questid );	
							
						}
					}
					else// if(currentquest)
					{
						iter_queststarttype = m_listQuestStartTypeLevel.erase(iter_queststarttype);
						
						//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
						//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_LEVEL ] ", questid );	
						
					}
				}
				else
				{
					iter_queststarttype++;
				}
			}
			else// if( questid != -1 )
			{
				iter_queststarttype = m_listQuestStartTypeLevel.erase(iter_queststarttype);
				
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_LEVEL ] ", questid );	
				
			}
		}
	}//if( !m_listQuestStartTypeLevel.empty() )
	
}

void _XQuestScriptManager::QuestStartConditionZoneMode()
{
	// 제룡행 퀘스트의 시작 조건을 검사한다. 
	if( m_CurrentMainQuestID != -1 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(m_CurrentMainQuestID);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				// 영역 검사가 들어간 시작 조건이면 검사시작
				if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_ZONE || currentquest->GetQuestStartConditionMode() == _XQUESTSTART_NONE )
				{
					// 퀘스트가 완료되지 않았으면 검사
					if(!currentquest->GetComplete())
					{
						currentquest->CheckStartCondition();
					}
				}
			}
		}
	}
	
	// 진행중인 퀘스트가 6개 넘으면 제룡행이 아닌 퀘스트의 시작 조건을 검사를 하지 않음..
	if( !CheckProgressQuestCount() ) 
	{
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
		if(!g_pLocalUser->GetInBossRoom())	// 보스전에 들어가 있으면 통과
			return;
#else
		return;
#endif
	}
	
	// 무림행 협객행 기연행 무분류 퀘스트 시작조건 검사 	
	if( !m_listQuestStartTypeZone.empty() )
	{		
		list <int>::iterator iter_queststarttype;

		iter_queststarttype = m_listQuestStartTypeZone.begin();
		while( iter_queststarttype != m_listQuestStartTypeZone.end() )
		{			
			int questid = *iter_queststarttype;			
			if( questid != -1 )
			{
				if( questid == 51500 )
				{
					questid = 51500;
				}
				map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
				if(iter_quest != m_mapSubQuestScriptList.end())
				{
					_XQuestScriptItem* currentquest = iter_quest->second;
					if(currentquest)
					{
						// 영역 검사가 들어간 시작 조건이면 검사시작
						if( currentquest->GetQuestStartConditionMode() == _XQUESTSTART_ZONE || currentquest->GetQuestStartConditionMode() == _XQUESTSTART_NONE )
						{
							// 퀘스트가 완료되지 않았으면 검사
							if(!currentquest->GetComplete())
							{
								if( currentquest->GetStartTime() == 0)
								{
									if( currentquest->CheckStartCondition() ==  QUEST_STARTCONDITION_RESULT_NOTIFYSERVER )
									{
										iter_queststarttype = m_listQuestStartTypeZone.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_ZONE ] ", questid );	
										
									}
									else if( currentquest->m_bChangeQuestStartMode )
									{
										iter_queststarttype = m_listQuestStartTypeZone.erase(iter_queststarttype);
										
										//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
										//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_ZONE ] ", questid );	
										
									}
									else
									{
										iter_queststarttype++;
									}
								}
								else
								{
									iter_queststarttype = m_listQuestStartTypeZone.erase(iter_queststarttype);
									
									//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
									//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_ZONE ] ", questid );	
									
								}
							}
							else
							{
								iter_queststarttype = m_listQuestStartTypeZone.erase(iter_queststarttype);
								
								//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
								//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_ZONE ] ", questid );	
								
							}							
						}
						else
						{
							iter_queststarttype = m_listQuestStartTypeZone.erase(iter_queststarttype);
							
							//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
							//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_ZONE ] ", questid );	
							
						}
					}
					else// if(currentquest)
					{
						iter_queststarttype = m_listQuestStartTypeZone.erase(iter_queststarttype);
						
						//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
						//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_ZONE ] ", questid );	
						
					}
				}
				else
				{
					iter_queststarttype++;
				}
			}
			else// if( questid != -1 )
			{
				iter_queststarttype = m_listQuestStartTypeZone.erase(iter_queststarttype);
				
				//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
				//_XLog("Delete SubQuest StartMode: SubQuestID[%d] StartConditionMode[ _XQUESTSTART_ZONE ] ", questid );	
				
			}
		}
	}//if( !m_listQuestStartTypeZone.empty() )	
}


void _XQuestScriptManager::QuestStartConditionSort()
{	
	//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
	
	// 시작 조건에 따른 퀘스트 아이디를 가지고 있는 리스트를 삭제한다. 
	list <int>::iterator iter_npctargetting;
	for(iter_npctargetting = m_listQuestStartTypeNPCTargetting.begin() ; iter_npctargetting != m_listQuestStartTypeNPCTargetting.end() ; )
	{
		iter_npctargetting = m_listQuestStartTypeNPCTargetting.erase(iter_npctargetting);
	}
	m_listQuestStartTypeNPCTargetting.clear();
	
	list <int>::iterator iter_objectclick;
	for(iter_objectclick = m_listQuestStartTypeObjectClick.begin() ; iter_objectclick != m_listQuestStartTypeObjectClick.end() ; )
	{
		iter_objectclick = m_listQuestStartTypeObjectClick.erase(iter_objectclick);
	}
	m_listQuestStartTypeObjectClick.clear();
	
	list <int>::iterator iter_usingitem;
	for(iter_usingitem = m_listrQuestStartTypeUsingItem.begin() ; iter_usingitem != m_listrQuestStartTypeUsingItem.end() ; )
	{
		iter_usingitem = m_listrQuestStartTypeUsingItem.erase(iter_usingitem);
	}
	m_listrQuestStartTypeUsingItem.clear();
	
	list <int>::iterator iter_wearitem;
	for(iter_wearitem = m_listQuestStartTypeWearItem.begin() ; iter_wearitem != m_listQuestStartTypeWearItem.end() ; )
	{
		iter_wearitem = m_listQuestStartTypeWearItem.erase(iter_wearitem);
	}
	m_listQuestStartTypeWearItem.clear();
	
	list <int>::iterator iter_destroyitem;
	for(iter_destroyitem = m_listQuestStartTypeDestroyItem.begin() ; iter_destroyitem != m_listQuestStartTypeDestroyItem.end() ; )
	{
		iter_destroyitem = m_listQuestStartTypeDestroyItem.erase(iter_destroyitem);
	}
	m_listQuestStartTypeDestroyItem.clear();
	
	list <int>::iterator iter_level;
	for(iter_level = m_listQuestStartTypeLevel.begin() ; iter_level != m_listQuestStartTypeLevel.end() ; )
	{
		iter_level = m_listQuestStartTypeLevel.erase(iter_level);
	}
	m_listQuestStartTypeLevel.clear();
	
	list <int>::iterator iter_zone;
	for(iter_zone = m_listQuestStartTypeZone.begin() ; iter_zone != m_listQuestStartTypeZone.end() ; )
	{
		iter_zone = m_listQuestStartTypeZone.erase(iter_zone);
	}
	m_listQuestStartTypeZone.clear();
	

	map <int, _XQuestScriptItem*>::iterator iter_quest;
	for(iter_quest = m_mapSubQuestScriptList.begin() ; iter_quest != m_mapSubQuestScriptList.end() ; ++iter_quest)
	{		
		_XQuestScriptItem* currentquest = iter_quest->second;
		if(currentquest)
		{
			BOOL binsertlist = FALSE;
			int  questid = currentquest->GetQuestID();

			if( !currentquest->GetComplete() )
			{	
				
				BOOL bstartquest = FALSE;
				for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
				{
					if( m_ProgressSubQuestID[i] == questid )
					{
						bstartquest = TRUE;
						break;
					}
				}		

				if( !bstartquest )
				{
					// 문파랑 상관없는 퀘스트는 분류에 따라 리스트에 삽입한다. 
					if( questid >= 9000 && questid < 10000 )
					{
						binsertlist = TRUE;
					}
					else if( questid >= 29000 && questid < 30000 )
					{
						binsertlist = TRUE;
					}
					else if( questid >= 39000 && questid < 40000 )
					{
						binsertlist = TRUE;
					}
					else if( questid >= 49000 && questid < 50000 )
					{
						binsertlist = TRUE;
					}
					else if( questid >= 50000 && questid < 60000  )
					{
						binsertlist = TRUE;
					}

					if( !binsertlist )
					{
						// 제룡행 번호대는 제외  -> 따로 처리함. 
						// 문파를 가지는 경우 문파에 맞는 퀘스트만 분류에 따라 리스트에 삽입한다. 
						switch(g_pLocalUser->m_CharacterInfo.groupindex) 
						{
						case _XGROUP_GAEBANG :
							{
								if( questid >= 1000 && questid < 2000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 21000 && questid < 22000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 31000 && questid < 32000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 41000 && questid < 42000 )
								{
									binsertlist = TRUE;
								}
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								if( questid >= 2000 && questid < 3000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 22000 && questid < 23000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 32000 && questid < 33000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 42000 && questid < 43000 )
								{
									binsertlist = TRUE;
								}
							}
							break;
						case _XGROUP_SORIM :
							{
								if( questid >= 3000 && questid < 4000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 23000 && questid < 24000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 33000 && questid < 34000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 43000 && questid < 44000 )
								{
									binsertlist = TRUE;
								}
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								if( questid >= 4000 && questid < 5000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 24000 && questid < 25000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 34000 && questid < 35000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 44000 && questid < 45000 )
								{
									binsertlist = TRUE;
								}
							}
							break;
						case _XGROUP_MOODANG :
							{
								if( questid >= 5000 && questid < 6000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 25000 && questid < 26000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 35000 && questid < 36000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 45000 && questid < 46000 )
								{
									binsertlist = TRUE;
								}
							}
							break;
						case _XGROUP_MAKYO :
							{
								if( questid >= 6000 && questid < 7000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 26000 && questid < 27000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 36000 && questid < 37000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 46000 && questid < 47000 )
								{
									binsertlist = TRUE;
								}
								
							}
							break;
						case _XGROUP_NANGIN :
							{							
								switch( g_CurrentZoneInfoIndex-1 ) 
								{
								case 0: // 소림
									{
										if( questid >= 3000 && questid < 4000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 23000 && questid < 24000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 33000 && questid < 34000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 43000 && questid < 44000 )
										{
											binsertlist = TRUE;
										}
									}
									break;
								case 1: // 개방
									{
										if( questid >= 1000 && questid < 2000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 21000 && questid < 22000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 31000 && questid < 32000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 41000 && questid < 42000 )
										{
											binsertlist = TRUE;
										}
									}
									break;
								case 2: // 녹림
									{
										if( questid >= 4000 && questid < 5000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 24000 && questid < 25000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 34000 && questid < 35000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 44000 && questid < 45000 )
										{
											binsertlist = TRUE;
										}
									}
									break;
								case 3: // 비궁
									{
										if( questid >= 2000 && questid < 3000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 22000 && questid < 23000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 32000 && questid < 33000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 42000 && questid < 43000 )
										{
											binsertlist = TRUE;
										}	
									}
									break;
								case 4: // 무당
									{
										if( questid >= 5000 && questid < 6000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 25000 && questid < 26000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 35000 && questid < 36000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 45000 && questid < 46000 )
										{
											binsertlist = TRUE;
										}
									}
									break;
								case 5: // 마교
									{
										if( questid >= 6000 && questid < 7000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 26000 && questid < 27000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 36000 && questid < 37000 )
										{
											binsertlist = TRUE;
										}
										else if( questid >= 46000 && questid < 47000 )
										{
											binsertlist = TRUE;
										}
									}
									break;
								} 
								
								if( questid >= 0 && questid < 1000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 20000 && questid < 21000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 30000 && questid < 31000 )
								{
									binsertlist = TRUE;
								}
								else if( questid >= 40000 && questid < 41000 )
								{
									binsertlist = TRUE;
								}
							}
							break;

						}
					}
				}

			}

			if( binsertlist )// 리스트에 삽입해도 되는 경우에 분류별로 리스트에 삽입한다.
			{				
				for( int i = 0; i < 20; ++i )
				{
					currentquest->m_SuccessStartConditionid[i] = -1;
				}
				currentquest->m_QuestStartConditionValue = 0;
				map <int, _XTriggerConditionItem*>::iterator iter_condition;
				_XTriggerConditionItem* currentcondition = NULL;
				for(iter_condition = currentquest->m_mapStartConditionList.begin() ; iter_condition != currentquest->m_mapStartConditionList.end() ; ++iter_condition)
				{
					currentcondition = iter_condition->second;
					if(currentcondition)
					{					
						// 퀘스트 시작 조건 모드를 설정하는 부분(NPC,OBJECT,ITEM,LEVEL,ZONE)
						if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
							&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC )
						{
							//m_QuestStartMode = _XQUESTSTART_NPCTARGETTING;
							currentquest->m_QuestStartConditionValue += 1;
						}
						else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKQUESTEMAIL )
						{
							// 퀘스트 시작에서 풍운방 체크도 NPC Targetting모드로 들어감
							currentquest->m_QuestStartConditionValue += 1;
						}
						else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKDESTORYITEM )
						{
							//m_QuestStartMode = _XQUESTSTART_DESTROYITEM;
							currentquest->m_QuestStartConditionValue += 1000000;
						}
						else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
							&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_OBJECT )
						{
							//m_QuestStartMode = _XQUESTSTART_OBJECTCLICK;
							currentquest->m_QuestStartConditionValue += 100000;
						}
						else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
							&& currentcondition->m_TDC.m_compareclass2 == 1 )//0:아이템 장착체크 1:아이템 사용체크 
						{
							//m_QuestStartMode = _XQUESTSTART_USINGITEM;
							currentquest->m_QuestStartConditionValue += 10000;
						}
						else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
							&& currentcondition->m_TDC.m_compareclass2 == 0 )//0:아이템 장착체크 1:아이템 사용체크
						{
							//m_QuestStartMode = _XQUESTSTART_WEARITEM;
							currentquest->m_QuestStartConditionValue += 1000;
						}
						else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKVALUE 
							&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_CHECKVALUETYPE_TOTALLEVEL )
						{
							//m_QuestStartMode = _XQUESTSTART_LEVEL;
							currentquest->m_QuestStartConditionValue += 100;
						}				
						else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKAREA )
						{
							//m_QuestStartMode = _XQUESTSTART_ZONE;
							currentquest->m_QuestStartConditionValue += 10;
						}
						else
						{
							if( currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETQUESTMARKAREA  &&
								currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETQUESTMARKNPC  &&
								currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETHINTMARKAREA  &&
								currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETHINTMARKNPC )
							{
								currentquest->m_QuestStartConditionValue += 10;
							}
						}
						
					}
				}

				if(currentquest->m_QuestStartConditionValue > 999999 ) // 1000000번대이므로 DestroyItem
				{
					m_listQuestStartTypeDestroyItem.push_back(questid);
					currentquest->SetQuestStartConditionMode( _XQUESTSTART_DESTROYITEM );
					//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_DESTROYITEM ] ", questid );
				}
				else if( currentquest->m_QuestStartConditionValue > 99999 ) // 100000번대이므로 ObjectClick
				{
					
					m_listQuestStartTypeObjectClick.push_back(questid);
					currentquest->SetQuestStartConditionMode( _XQUESTSTART_OBJECTCLICK );
					//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_OBJECTCLICK ] ", questid );
					
				}
				else if( currentquest->m_QuestStartConditionValue > 9999 ) // 10000번대이므로 Using Item
				{
					m_listrQuestStartTypeUsingItem.push_back(questid);
					currentquest->SetQuestStartConditionMode( _XQUESTSTART_USINGITEM );
					//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_USINGITEM ] ", questid );
				}			
				else if( currentquest->m_QuestStartConditionValue > 999 ) // 1000번대이므로 Wear Item
				{
					m_listQuestStartTypeWearItem.push_back(questid);
					currentquest->SetQuestStartConditionMode( _XQUESTSTART_WEARITEM );
					//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_WEARITEM ] ", questid );
				}			
				else if( currentquest->m_QuestStartConditionValue > 99 ) // 100번대이므로 Level Up
				{
					m_listQuestStartTypeLevel.push_back(questid);
					currentquest->SetQuestStartConditionMode( _XQUESTSTART_LEVEL );
					//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_LEVEL ] ", questid );
				}			
				else if( currentquest->m_QuestStartConditionValue > 9 ) // 10번대이므로 Zone or None
				{
					m_listQuestStartTypeZone.push_back(questid);
					currentquest->SetQuestStartConditionMode( _XQUESTSTART_ZONE );
					//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_ZONE ] ", questid );
				}
				else if( currentquest->m_QuestStartConditionValue > 0 ) // 1번대이므로 NPC targetting
				{
					m_listQuestStartTypeNPCTargetting.push_back(questid);
					currentquest->SetQuestStartConditionMode( _XQUESTSTART_NPCTARGETTING );
					//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_NPCTARGETTING ] ", questid );

					// 처음부터 NPC 타게팅이면 마크를 표시해준다.						
					map <int, _XTriggerConditionItem*>::iterator iter_setmark;
					_XTriggerConditionItem* currentcondition = NULL;
					for(iter_setmark = currentquest->m_mapStartConditionList.begin() ; iter_setmark != currentquest->m_mapStartConditionList.end() ; ++iter_setmark)
					{
						currentcondition = iter_setmark->second;
						if(currentcondition)
						{											
							switch(currentcondition->m_TDC.m_conditionclass) 
							{
							case ID_CONDITION_SETQUESTMARKAREA :	// QUEST마크(!) 표시하기 영역
							case ID_CONDITION_SETQUESTMARKNPC :		// QUEST마크(!) 표시하기 NPC 
							case ID_CONDITION_SETHINTMARKAREA :		// HINT마크(?) 표시하기 영역
							case ID_CONDITION_SETHINTMARKNPC :		// HINT마크(?) 표시하기 NPC 
								if( currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
								{
									for( int i = 0; i < 20; ++i )
									{
										if( currentquest->m_SuccessStartConditionid[i] == -1 )
										{
											currentquest->m_SuccessStartConditionid[i] = currentcondition->GetConditionID();
											break;
										}
									}
								}								
								break;
							}											
						}
					}
				}
			}//if( binsertlist )// 리스트에 삽입해도 되는 경우에 분류별로 리스트에 삽입한다.
		}
	}
}

void _XQuestScriptManager::SetQuestStartCondition( int questid )
{		
	//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
	
	map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find( questid );
	if(iter_quest != m_mapSubQuestScriptList.end())
	{		
		_XQuestScriptItem* currentquest = iter_quest->second;
		if(currentquest)
		{				
			currentquest->Reset();
			currentquest->m_QuestStartConditionValue = 0;
			map <int, _XTriggerConditionItem*>::iterator iter_condition;
			_XTriggerConditionItem* currentcondition = NULL;
			for(iter_condition = currentquest->m_mapStartConditionList.begin() ; iter_condition != currentquest->m_mapStartConditionList.end() ; ++iter_condition)
			{
				currentcondition = iter_condition->second;
				if(currentcondition)
				{	
					// 시작조건을 검사할 때 여러 퀘스트를 가지는 NPC에 대한 처리도 이때 초기화한다.
					currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);

					// 퀘스트 시작 조건 모드를 설정하는 부분(NPC,OBJECT,ITEM,LEVEL,ZONE)
					if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
						&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC )
					{
						//m_QuestStartMode = _XQUESTSTART_NPCTARGETTING;
						currentquest->m_QuestStartConditionValue += 1;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKQUESTEMAIL )
					{
						// 퀘스트 시작에서 풍운방 체크도 NPC Targetting모드로 들어감
						currentquest->m_QuestStartConditionValue += 1;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKDESTORYITEM )
					{
						//m_QuestStartMode = _XQUESTSTART_DESTROYITEM;
						currentquest->m_QuestStartConditionValue += 1000000;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
						&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_OBJECT )
					{
						//m_QuestStartMode = _XQUESTSTART_OBJECTCLICK;
						currentquest->m_QuestStartConditionValue += 100000;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
						&& currentcondition->m_TDC.m_compareclass2 == 1 )//0:아이템 장착체크 1:아이템 사용체크 
					{
						//m_QuestStartMode = _XQUESTSTART_USINGITEM;
						currentquest->m_QuestStartConditionValue += 10000;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
						&& currentcondition->m_TDC.m_compareclass2 == 0 )//0:아이템 장착체크 1:아이템 사용체크
					{
						//m_QuestStartMode = _XQUESTSTART_WEARITEM;
						currentquest->m_QuestStartConditionValue += 1000;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKVALUE 
						&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_CHECKVALUETYPE_TOTALLEVEL )
					{
						//m_QuestStartMode = _XQUESTSTART_LEVEL;
						currentquest->m_QuestStartConditionValue += 100;
					}				
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKAREA )
					{
						//m_QuestStartMode = _XQUESTSTART_ZONE;
						currentquest->m_QuestStartConditionValue += 10;
					}
					else
					{
						if( currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETQUESTMARKAREA  &&
							currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETQUESTMARKNPC  &&
							currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETHINTMARKAREA  &&
							currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETHINTMARKNPC )
						{
							currentquest->m_QuestStartConditionValue += 10;
						}
					}
					
				}
			}

			
			if(currentquest->m_QuestStartConditionValue > 999999 ) // 1000000번대이므로 DestroyItem
			{
				m_listQuestStartTypeDestroyItem.push_back(questid);
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_DESTROYITEM );
				//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_DESTROYITEM ] ", questid );
			}
			else if( currentquest->m_QuestStartConditionValue > 99999 ) // 100000번대이므로 ObjectClick
			{
				m_listQuestStartTypeObjectClick.push_back(questid);
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_OBJECTCLICK );
				//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_OBJECTCLICK ] ", questid );
				
			}
			else if( currentquest->m_QuestStartConditionValue > 9999 ) // 10000번대이므로 Using Item
			{
				m_listrQuestStartTypeUsingItem.push_back(questid);
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_USINGITEM );
				//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_USINGITEM ] ", questid );
			}			
			else if( currentquest->m_QuestStartConditionValue > 999 ) // 1000번대이므로 Wear Item
			{
				m_listQuestStartTypeWearItem.push_back(questid);
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_WEARITEM );
				//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_WEARITEM ] ", questid );
			}			
			else if( currentquest->m_QuestStartConditionValue > 99 ) // 100번대이므로 Level Up
			{
				m_listQuestStartTypeLevel.push_back(questid);
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_LEVEL );
				//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_LEVEL ] ", questid );
			}			
			else if( currentquest->m_QuestStartConditionValue > 9 ) // 10번대이므로 Zone or None
			{
				m_listQuestStartTypeZone.push_back(questid);
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_ZONE );
				//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_ZONE ] ", questid );
			}
			else if( currentquest->m_QuestStartConditionValue > 0 ) // 1번대이므로 NPC targetting
			{
				m_listQuestStartTypeNPCTargetting.push_back(questid);
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_NPCTARGETTING );
				//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_NPCTARGETTING ] ", questid );
				
				// 처음부터 NPC 타게팅이면 마크를 표시해준다.						
				map <int, _XTriggerConditionItem*>::iterator iter_setmark;
				_XTriggerConditionItem* currentcondition = NULL;
				for(iter_setmark = currentquest->m_mapStartConditionList.begin() ; iter_setmark != currentquest->m_mapStartConditionList.end() ; ++iter_setmark)
				{
					currentcondition = iter_setmark->second;
					if(currentcondition)
					{											
						switch(currentcondition->m_TDC.m_conditionclass) 
						{
						case ID_CONDITION_SETQUESTMARKAREA :	// QUEST마크(!) 표시하기 영역
						case ID_CONDITION_SETQUESTMARKNPC :		// QUEST마크(!) 표시하기 NPC 
						case ID_CONDITION_SETHINTMARKAREA :		// HINT마크(?) 표시하기 영역
						case ID_CONDITION_SETHINTMARKNPC :		// HINT마크(?) 표시하기 NPC 
							if( currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
							{
								for( int i = 0; i < 20; ++i )
								{
									if( currentquest->m_SuccessStartConditionid[i] == -1 )
									{
										currentquest->m_SuccessStartConditionid[i] = currentcondition->GetConditionID();
										break;
									}
								}
							}								
							break;
						}											
					}
				}//for(iter_setmark = currentquest->m_mapStartConditionList.begin() ; iter_setmark != currentquest->m_mapStartConditionList.end() ; ++iter_setmark)
			}
		}
	}
}

void _XQuestScriptManager::SetMainQuestStartCondition( int questid )
{		
	
	//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
	
	map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find( questid );
	if(iter_quest != m_mapQuestScriptList.end())
	{		
		_XQuestScriptItem* currentquest = iter_quest->second;
		if(currentquest)
		{				
			currentquest->Reset();
			currentquest->m_QuestStartConditionValue = 0;
			map <int, _XTriggerConditionItem*>::iterator iter_condition;
			_XTriggerConditionItem* currentcondition = NULL;
			for(iter_condition = currentquest->m_mapStartConditionList.begin() ; iter_condition != currentquest->m_mapStartConditionList.end() ; ++iter_condition)
			{
				currentcondition = iter_condition->second;
				if(currentcondition)
				{					
					// 시작조건을 검사할 때 여러 퀘스트를 가지는 NPC에 대한 처리도 이때 초기화한다.
					currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);

					// 퀘스트 시작 조건 모드를 설정하는 부분(NPC,OBJECT,ITEM,LEVEL,ZONE)
					if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
						&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC )
					{
						//m_QuestStartMode = _XQUESTSTART_NPCTARGETTING;
						currentquest->m_QuestStartConditionValue += 1;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKQUESTEMAIL )
					{
						// 퀘스트 시작에서 풍운방 체크도 NPC Targetting모드로 들어감
						currentquest->m_QuestStartConditionValue += 1;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
						&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_OBJECT )
					{
						//m_QuestStartMode = _XQUESTSTART_OBJECTCLICK;
						currentquest->m_QuestStartConditionValue += 100000;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
						&& currentcondition->m_TDC.m_compareclass2 == 1 )//0:아이템 장착체크 1:아이템 사용체크 
					{
						//m_QuestStartMode = _XQUESTSTART_USINGITEM;
						currentquest->m_QuestStartConditionValue += 10000;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
						&& currentcondition->m_TDC.m_compareclass2 == 0 )//0:아이템 장착체크 1:아이템 사용체크
					{
						//m_QuestStartMode = _XQUESTSTART_WEARITEM;
						currentquest->m_QuestStartConditionValue += 1000;
					}
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKVALUE 
						&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_CHECKVALUETYPE_TOTALLEVEL )
					{
						//m_QuestStartMode = _XQUESTSTART_LEVEL;
						currentquest->m_QuestStartConditionValue += 100;
					}				
					else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKAREA )
					{
						//m_QuestStartMode = _XQUESTSTART_ZONE;
						currentquest->m_QuestStartConditionValue += 10;
					}
					else
					{
						if( currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETQUESTMARKAREA  &&
							currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETQUESTMARKNPC  &&
							currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETHINTMARKAREA  &&
							currentcondition->m_TDC.m_conditionclass != ID_CONDITION_SETHINTMARKNPC )
						{
							currentquest->m_QuestStartConditionValue += 10;
						}
					}
					
				}
			}

			if( currentquest->m_QuestStartConditionValue > 99999 ) // 100000번대이므로 ObjectClick
			{
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_OBJECTCLICK );
				//_XLog("MainQuest StartMode Setting : MainQuestID[%d]  StartConditionMode [ _XQUESTSTART_OBJECTCLICK ] " , questid );				
			}
			else if( currentquest->m_QuestStartConditionValue > 9999 ) // 10000번대이므로 Using Item
			{
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_USINGITEM );
				//_XLog("MainQuest StartMode Setting : MainQuestID[%d]  StartConditionMode [ _XQUESTSTART_USINGITEM ] " , questid );
			}			
			else if( currentquest->m_QuestStartConditionValue > 999 ) // 1000번대이므로 Wear Item
			{
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_WEARITEM );
				//_XLog("MainQuest StartMode Setting : MainQuestID[%d]  StartConditionMode [ _XQUESTSTART_WEARITEM ] " , questid ) ;
			}			
			else if( currentquest->m_QuestStartConditionValue > 99 ) // 100번대이므로 Level Up
			{
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_LEVEL );
				//_XLog("MainQuest StartMode Setting : MainQuestID[%d]  StartConditionMode [ _XQUESTSTART_LEVEL ] " , questid );
			}			
			else if( currentquest->m_QuestStartConditionValue > 9 ) // 10번대이므로 Zone or None
			{
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_ZONE );
				//_XLog("MainQuest StartMode Setting : MainQuestID[%d]  StartConditionMode [ _XQUESTSTART_ZONE ] " , questid );
			}
			else if( currentquest->m_QuestStartConditionValue > 0 ) // 1번대이므로 NPC targetting
			{
				currentquest->SetQuestStartConditionMode( _XQUESTSTART_NPCTARGETTING );
				//_XLog("MainQuest StartMode Setting : MainQuestID[%d]  StartConditionMode [ _XQUESTSTART_NPCTARGETTING ] " , questid );
			}
		}
	}
}



void _XQuestScriptManager::ResetQuestStartCondition( void )
{
	// 시작 조건을 다시 세팅할 때 퀘스트에 관련된 마크를 전부 지운다.
	if( !m_listQuestMark.empty() )
	{
		bool checkquest = false;
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = m_listQuestMark.begin() ; iter_questmark != m_listQuestMark.end() ; )
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
			
			// 진행중인 퀘스트의 마크 표시는 지우지 않는다.
			// 제룡행과 나머지 퀘스트들과 저장되지 않는 퀘스트의 아이디가
			// 퀘스트 마크가 가지고 있는 퀘스트 아이디와 일치하면 
			// 퀘스트 마크를 삭제하지 않는다.

			// 제룡행 퀘스트를 검사한다.
			if( m_CurrentMainQuestID == questmark.questid )
			{
				checkquest = true;
			}
			
			// 제룡행외의 퀘스트를 검사한다.
			if( !checkquest )
			{
				// 진행중인 퀘스트를 검사한다.
				for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
				{
					if( m_ProgressSubQuestID[i] == questmark.questid )
					{
						checkquest = true;
						break;
					}
				}	
			}

			// 저장하지 않는 퀘스트를 검사한다.
			if( !checkquest )
			{
				if( !m_listQuestNoSaveType.empty() )
				{
					list <int>::iterator iter_questnosavetype;
					for(iter_questnosavetype = m_listQuestNoSaveType.begin() ; iter_questnosavetype != m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
					{
						int nosavequestid = *iter_questnosavetype;	
						if( nosavequestid == questmark.questid )
						{
							checkquest = true;
							break;
						}
					}
				}				
			}
			
			if( !checkquest )
			{
				if( questmark.npcid != -1 )
				{
					for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
					{
						if( questmark.npcid == g_Npclist.m_pNPCObjectArray[i].m_UniqueID)
						{
#ifdef _XTS_HAVEQUESTLISTNPC	
							// npc가 2개 이상의 퀘스트를 가지고 있으면 해당 npc는 퀘스트를 가지고 있는 것이므로
							// 마크를 삭제하면 안된다.
							int npcid = g_Npclist.m_pNPCObjectArray[i].m_UniqueID;
							int npccount = 0;
							
							// npc 마크를 저장하는 구조체에서 동일 npc가 있는지를 검사한다.
							list <_XQUEST_MARKINFO>::iterator iter_questmark;
							for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
							{
								_XQUEST_MARKINFO questmark = *iter_questmark;
								if( questmark.npcid == npcid )
								{
									npccount++;
								}
							}
							
							// 동일한 npc가 있으면 npc 퀘스트 마크를 삭제하지 않는다.
							if( npccount == 1)
								g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;			
#else
							g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;
#endif	
							break;
						}
					}
				}				
				iter_questmark = m_listQuestMark.erase(iter_questmark);
			}
			else
			{
				iter_questmark++;
			}
		}		
	}

	if( !m_listHintMark.empty() )
	{
		bool checkquest = false;
		list <_XQUEST_MARKINFO>::iterator iter_hintmark;
		for(iter_hintmark = m_listHintMark.begin() ; iter_hintmark != m_listHintMark.end() ; )
		{
			_XQUEST_MARKINFO hintmark = *iter_hintmark;	
			
			// 제룡행 퀘스트를 검사한다.
			if( m_CurrentMainQuestID == hintmark.questid )
			{
				checkquest = true;
			}
			
			if( !checkquest )
			{
				// 진행중인 퀘스트를 검사한다.
				for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
				{
					if( m_ProgressSubQuestID[i] == hintmark.questid )
					{
						checkquest = true;
						break;
					}
				}	
			}
			if( !checkquest )
			{
				// 진행중인 저장하지 않는 퀘스트 리스트도 검사한다.
				if( !m_listQuestNoSaveType.empty() )
				{
					list <int>::iterator iter_questnosavetype;
					for(iter_questnosavetype = m_listQuestNoSaveType.begin() ; iter_questnosavetype != m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
					{
						int nosavequestid = *iter_questnosavetype;	
						if( nosavequestid == hintmark.questid )
						{
							checkquest = true;
							break;
						}
					}
				}				
			}
			
			if( !checkquest )
			{
				if( hintmark.npcid != -1 )
				{
					for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
					{
						if( hintmark.npcid == g_Npclist.m_pNPCObjectArray[i].m_UniqueID)
						{
#ifdef _XTS_HAVEQUESTLISTNPC	
							// npc가 2개 이상의 퀘스트를 가지고 있으면 해당 npc는 퀘스트를 가지고 있는 것이므로
							// 마크를 삭제하면 안된다.
							int npcid = g_Npclist.m_pNPCObjectArray[i].m_UniqueID;
							int npccount = 0;
							
							// npc 마크를 저장하는 구조체에서 동일 npc가 있는지를 검사한다.
							list <_XQUEST_MARKINFO>::iterator iter_hinttmark;
							for(iter_hinttmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hinttmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hinttmark)
							{
								_XQUEST_MARKINFO hintmark = *iter_hinttmark;
								if( hintmark.npcid == npcid )
								{
									npccount++;
								}
							}
							
							// 동일한 npc가 있으면 npc 퀘스트 마크를 삭제하지 않는다.
							if( npccount == 1)
								g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;	
							
#else
							g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;	
#endif
							break;
						}
					}
				}
				iter_hintmark = m_listHintMark.erase(iter_hintmark);
			}
			else
			{
				iter_hintmark++;
			}
		}
	}
	// 끝 - 시작 조건을 다시 세팅할 때 퀘스트에 관련된 마크를 전부 지운다.
	
	list <int> tempquestid;
	
	// 분류되어 있는 시작 조건을 한곳으로 모은다. 
	list <int>::iterator iter_npctargetting;
	for(iter_npctargetting = m_listQuestStartTypeNPCTargetting.begin() ; iter_npctargetting != m_listQuestStartTypeNPCTargetting.end() ; )
	{
		int questid = *iter_npctargetting;
		tempquestid.push_back( questid );
		iter_npctargetting = m_listQuestStartTypeNPCTargetting.erase(iter_npctargetting);
	}
	m_listQuestStartTypeNPCTargetting.clear();
	
	list <int>::iterator iter_objectclick;
	for(iter_objectclick = m_listQuestStartTypeObjectClick.begin() ; iter_objectclick != m_listQuestStartTypeObjectClick.end() ; )
	{
		int questid = *iter_objectclick;
		tempquestid.push_back( questid );
		iter_objectclick = m_listQuestStartTypeObjectClick.erase(iter_objectclick);
	}
	m_listQuestStartTypeObjectClick.clear();
	
	list <int>::iterator iter_usingitem;
	for(iter_usingitem = m_listrQuestStartTypeUsingItem.begin() ; iter_usingitem != m_listrQuestStartTypeUsingItem.end() ; )
	{
		int questid = *iter_usingitem;
		tempquestid.push_back( questid );
		iter_usingitem = m_listrQuestStartTypeUsingItem.erase(iter_usingitem);
	}
	m_listrQuestStartTypeUsingItem.clear();
	
	list <int>::iterator iter_wearitem;
	for(iter_wearitem = m_listQuestStartTypeWearItem.begin() ; iter_wearitem != m_listQuestStartTypeWearItem.end() ; )
	{
		int questid = *iter_wearitem;
		tempquestid.push_back( questid );
		iter_wearitem = m_listQuestStartTypeWearItem.erase(iter_wearitem);
	}
	m_listQuestStartTypeWearItem.clear();
	
	list <int>::iterator iter_destroyitem;
	for(iter_destroyitem = m_listQuestStartTypeDestroyItem.begin() ; iter_destroyitem != m_listQuestStartTypeDestroyItem.end() ; )
	{
		int questid = *iter_destroyitem;
		tempquestid.push_back( questid );
		iter_destroyitem = m_listQuestStartTypeDestroyItem.erase(iter_destroyitem);
	}
	m_listQuestStartTypeDestroyItem.clear();
	
	list <int>::iterator iter_level;
	for(iter_level = m_listQuestStartTypeLevel.begin() ; iter_level != m_listQuestStartTypeLevel.end() ; )
	{
		int questid = *iter_level;
		tempquestid.push_back( questid );
		iter_level = m_listQuestStartTypeLevel.erase(iter_level);
	}
	m_listQuestStartTypeLevel.clear();
	
	list <int>::iterator iter_zone;
	for(iter_zone = m_listQuestStartTypeZone.begin() ; iter_zone != m_listQuestStartTypeZone.end() ; )
	{
		int questid = *iter_zone;
		tempquestid.push_back( questid );		
		iter_zone = m_listQuestStartTypeZone.erase(iter_zone);
	}
	m_listQuestStartTypeZone.clear();

	//////////////////////////////////////////////////////////////////////////////////////////
	// 다시 시작 조건을 세팅한다. 
	bool progressquest = false;
	list <int>::iterator iter_tempquestid;
	for(iter_tempquestid = tempquestid.begin() ; iter_tempquestid != tempquestid.end() ; )
	{
		int questid = *iter_tempquestid;	
		if( questid != -1 )
		{			
			// 진행중인 퀘스트를 검사한다.
			for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
			{
				if( m_ProgressSubQuestID[i] == questid)
				{
					progressquest = true;
					break;
				}
			}	
			
			// 진행중인 저장하지 않는 퀘스트 리스트도 검사한다.
			if( !m_listQuestNoSaveType.empty() )
			{
				list <int>::iterator iter_questnosavetype;
				for(iter_questnosavetype = m_listQuestNoSaveType.begin() ; iter_questnosavetype != m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
				{
					int nosavequestid = *iter_questnosavetype;	
					if( nosavequestid == questid )
					{
						progressquest = true;
						break;
					}
				}
			}

			if( !progressquest ) SetQuestStartCondition( questid );			
		}
		iter_tempquestid = tempquestid.erase(iter_tempquestid);
	}
	tempquestid.clear();
}

void _XQuestScriptManager::SetQuestStartConditionAtVirtualSpace( void )
{
	// 가상공간에 들어갈 때는 기존에 진행 중이던 퀘스트 마크를 전부 지워야 한다.
	// 같은 공간에서 퀘스트가 다시 재정렬 되는 것이 아니므로 NPC위치가 전부 바뀌었기 때문이다.

	// 시작 조건을 다시 세팅할 때 퀘스트에 관련된 마크를 전부 지운다.
	if( !m_listQuestMark.empty() )
	{
		bool checkquest = false;
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = m_listQuestMark.begin() ; iter_questmark != m_listQuestMark.end() ; )
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
						
			if( questmark.npcid != -1 )
			{
				for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
				{
					if( questmark.npcid == g_Npclist.m_pNPCObjectArray[i].m_UniqueID)
					{
						g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;	
						break;
					}
				}
			}				
			iter_questmark = m_listQuestMark.erase(iter_questmark);		
		}		
	}

	if( !m_listHintMark.empty() )
	{
		bool checkquest = false;
		list <_XQUEST_MARKINFO>::iterator iter_hintmark;
		for(iter_hintmark = m_listHintMark.begin() ; iter_hintmark != m_listHintMark.end() ; )
		{
			_XQUEST_MARKINFO hintmark = *iter_hintmark;					
			if( hintmark.npcid != -1 )
			{
				for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
				{
					if( hintmark.npcid == g_Npclist.m_pNPCObjectArray[i].m_UniqueID)
					{
						g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;	
						break;
					}
				}
			}
			iter_hintmark = m_listHintMark.erase(iter_hintmark);
		}
	}
	// 끝 - 시작 조건을 다시 세팅할 때 퀘스트에 관련된 마크를 전부 지운다.
	
	list <int> tempquestid;
	
	// 분류되어 있는 시작 조건을 한곳으로 모은다. 
	list <int>::iterator iter_npctargetting;
	for(iter_npctargetting = m_listQuestStartTypeNPCTargetting.begin() ; iter_npctargetting != m_listQuestStartTypeNPCTargetting.end() ; )
	{
		int questid = *iter_npctargetting;
		tempquestid.push_back( questid );
		iter_npctargetting = m_listQuestStartTypeNPCTargetting.erase(iter_npctargetting);
	}
	m_listQuestStartTypeNPCTargetting.clear();
	
	list <int>::iterator iter_objectclick;
	for(iter_objectclick = m_listQuestStartTypeObjectClick.begin() ; iter_objectclick != m_listQuestStartTypeObjectClick.end() ; )
	{
		int questid = *iter_objectclick;
		tempquestid.push_back( questid );
		iter_objectclick = m_listQuestStartTypeObjectClick.erase(iter_objectclick);
	}
	m_listQuestStartTypeObjectClick.clear();
	
	list <int>::iterator iter_usingitem;
	for(iter_usingitem = m_listrQuestStartTypeUsingItem.begin() ; iter_usingitem != m_listrQuestStartTypeUsingItem.end() ; )
	{
		int questid = *iter_usingitem;
		tempquestid.push_back( questid );
		iter_usingitem = m_listrQuestStartTypeUsingItem.erase(iter_usingitem);
	}
	m_listrQuestStartTypeUsingItem.clear();
	
	list <int>::iterator iter_wearitem;
	for(iter_wearitem = m_listQuestStartTypeWearItem.begin() ; iter_wearitem != m_listQuestStartTypeWearItem.end() ; )
	{
		int questid = *iter_wearitem;
		tempquestid.push_back( questid );
		iter_wearitem = m_listQuestStartTypeWearItem.erase(iter_wearitem);
	}
	m_listQuestStartTypeWearItem.clear();
	
	list <int>::iterator iter_destroyitem;
	for(iter_destroyitem = m_listQuestStartTypeDestroyItem.begin() ; iter_destroyitem != m_listQuestStartTypeDestroyItem.end() ; )
	{
		int questid = *iter_destroyitem;
		tempquestid.push_back( questid );
		iter_destroyitem = m_listQuestStartTypeDestroyItem.erase(iter_destroyitem);
	}
	m_listQuestStartTypeDestroyItem.clear();
	
	list <int>::iterator iter_level;
	for(iter_level = m_listQuestStartTypeLevel.begin() ; iter_level != m_listQuestStartTypeLevel.end() ; )
	{
		int questid = *iter_level;
		tempquestid.push_back( questid );
		iter_level = m_listQuestStartTypeLevel.erase(iter_level);
	}
	m_listQuestStartTypeLevel.clear();
	
	list <int>::iterator iter_zone;
	for(iter_zone = m_listQuestStartTypeZone.begin() ; iter_zone != m_listQuestStartTypeZone.end() ; )
	{
		int questid = *iter_zone;
		tempquestid.push_back( questid );
		iter_zone = m_listQuestStartTypeZone.erase(iter_zone);
	}
	m_listQuestStartTypeZone.clear();

	//////////////////////////////////////////////////////////////////////////////////////////
	// 다시 시작 조건을 세팅한다. 
	bool progressquest = false;
	list <int>::iterator iter_tempquestid;
	for(iter_tempquestid = tempquestid.begin() ; iter_tempquestid != tempquestid.end() ; )
	{
		int questid = *iter_tempquestid;	
		if( questid != -1 )
		{			
			// 진행중인 퀘스트를 검사한다.
			for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
			{
				if( m_ProgressSubQuestID[i] == questid)
				{
					progressquest = true;
					break;
				}
			}	
			
			// 진행중인 저장하지 않는 퀘스트 리스트도 검사한다.
			if( !m_listQuestNoSaveType.empty() )
			{
				list <int>::iterator iter_questnosavetype;
				for(iter_questnosavetype = m_listQuestNoSaveType.begin() ; iter_questnosavetype != m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
				{
					int nosavequestid = *iter_questnosavetype;	
					if( nosavequestid == questid )
					{
						progressquest = true;
						break;
					}
				}
			}

			if( !progressquest ) SetQuestStartCondition( questid );			
		}
		iter_tempquestid = tempquestid.erase(iter_tempquestid);
	}
	tempquestid.clear();
}

int _XQuestScriptManager::CheckPracticableQuestNode( int questid, BYTE treestep , BOOL& bneeditemremove ) //실행가능한 노드를 반환한다.
{		
	
	if( treestep == 0 ) 
	{
		bneeditemremove = TRUE;
		return -1;
	}
	
	int searchtreenode = treestep;
	int practicablequestnode = -1;
	bneeditemremove = FALSE;

	_XQuestScriptItem* currentquest = NULL;
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapQuestScriptList.find(questid);
		if(iter != m_mapQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}		
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter = m_mapSubQuestScriptList.find(questid);
		if(iter != m_mapSubQuestScriptList.end())
		{
			currentquest = iter->second;
		}
	}
	
	// 트리 노드의 m_bQuestBackAtCurrentNode의 값이 FALSE일때 까지 찾는다.
	// m_bQuestBackAtCurrentNode의 값이 FALSE일때의 노드를 반환한다.
	if( currentquest )
	{					
		do {				
			map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(searchtreenode);
			if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
			{
				_XTriggerTreeItem* currenttree = iter_triggertree->second;
				if(currenttree)
				{
					if( currenttree->m_bQuestBackAtCurrentNode )
					{
						searchtreenode = currenttree->m_ParentsNodeUniqueID;
						
						//부모노드가 0이며 퀘스트 시작조건으로 세팅되어야 함.
						if( searchtreenode == 0 )
						{
							practicablequestnode = searchtreenode;
						}
						
						BOOL bSuccess = FALSE;
						map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(searchtreenode);
						if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
						{
							_XTriggerTreeItem* parentstree = iter_triggertree->second;
							if(parentstree)
							{
								if( parentstree->m_TRightNodeUniqueID == currenttree->m_SelfNodeUniqueID )
								{
									bSuccess = TRUE;
								}								
							}
						}	
						
									
						map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
						if(currentsuccesstrigger)
						{					
							currentsuccesstrigger->SetClientEnable(TRUE);
							//_XLog("Quest Trigger Condition Check Close : TriggerID[%d]", currenttree->m_TriggerUniqueID);
							for( int i = 0; i < 20; ++i )
							{
								currentsuccesstrigger->m_SuccessConditionid[i] = -1;
							}
							// 성공일때 행동식에서 체크 
							if( bSuccess )
							{
								map <int, _XTriggerActionItem*>::iterator iter_normalaction;
								_XTriggerActionItem* currentaction = NULL;
								for(iter_normalaction = currentsuccesstrigger->m_mapNormalActionList.begin() ; iter_normalaction != currentsuccesstrigger->m_mapNormalActionList.end() ; ++iter_normalaction)
								{
									currentaction = iter_normalaction->second;
									if(currentaction)
									{
										if( currentaction->m_TDA.m_actionclass == ID_ACTION_ADDSKILL )
										{
											bneeditemremove = TRUE;
										}
										else if( currentaction->m_TDA.m_actionclass == ID_ACTION_DELETESKILL )
										{			
											bneeditemremove = TRUE;
										}
										else if( currentaction->m_TDA.m_actionclass == ID_ACTION_GIVEITEM )
										{
											bneeditemremove = TRUE;
										}										
										else if( currentaction->m_TDA.m_actionclass == ID_ACTION_TAKEITEM )
										{	
											bneeditemremove = TRUE;
										}
									}
								}			
							}	
							else// 실패일때 행동식에서 체크
							{
								map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
								_XTriggerActionItem* currentaction = NULL;
								for(iter_exceptedaction = currentsuccesstrigger->m_mapExceptedActionList.begin() ; iter_exceptedaction != currentsuccesstrigger->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
								{
									currentaction = iter_exceptedaction->second;
									if(currentaction)
									{
										if( currentaction->m_TDA.m_actionclass == ID_ACTION_ADDSKILL )
										{
											bneeditemremove = TRUE;
										}
										else if( currentaction->m_TDA.m_actionclass == ID_ACTION_DELETESKILL )
										{			
											bneeditemremove = TRUE;
										}
										else if( currentaction->m_TDA.m_actionclass == ID_ACTION_GIVEITEM )
										{
											bneeditemremove = TRUE;
										}										
										else if( currentaction->m_TDA.m_actionclass == ID_ACTION_TAKEITEM )
										{	
											bneeditemremove = TRUE;
										}
									}
								}
							}
						}
					}
					else
					{
						practicablequestnode = searchtreenode;
					}					
				}
				else
				{
					return -1;
				}
			}	
			else
			{
				return -1;
			}
		} while( practicablequestnode == -1 );
		
		if( treestep == practicablequestnode )
		{
			return -1; // 정상적인 퀘스트 
		}
		else
		{
			return practicablequestnode; //비정상적인 퀘스트 -> 백이 필요함 
		}
	}
	else
	{
		return -1;
	}
		
}

BYTE _XQuestScriptManager::GetQuestCurrentTreeNodeClass( int questid, BYTE treestep ) //현재 노드가 분기인지 끝인지 정상인지를 알아낸다.
{		
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(questid);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treestep);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{			
						map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
						if(currentsuccesstrigger)
						{
							return currentsuccesstrigger->GetTriggerClass();
						}
					}
				}				
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
		if(iter_quest != m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treestep);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{			
						map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
						if(currentsuccesstrigger)
						{
							return currentsuccesstrigger->GetTriggerClass();
						}
					}
				}				
				
			}
		}
	}
	
	return ID_TRIGGER_CLASS_NORMAL;
}

BOOL _XQuestScriptManager::CheckHaveQuestJumpAction( int questid, BYTE treestep ) //현재 노드에 Goto가 있는지 체크한다.
{		
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(questid);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treestep);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{			
						map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
						if(currentsuccesstrigger)
						{							
							if( !currentsuccesstrigger->m_mapNormalActionList.empty() )
							{
								map <int, _XTriggerActionItem*>::iterator iter_normalaction;
								_XTriggerActionItem* currentaction = NULL;
								for(iter_normalaction = currentsuccesstrigger->m_mapNormalActionList.begin() ; iter_normalaction != currentsuccesstrigger->m_mapNormalActionList.end() ; ++iter_normalaction)
								{
									currentaction = iter_normalaction->second;
									if(currentaction)
									{
										if( currentaction->m_TDA.m_actionclass == ID_ACTION_GOTOTRIGGER )
										{
											return TRUE;
										}
									}
								}
								
								if( !currentsuccesstrigger->m_mapExceptedActionList.empty() )
								{
									map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
									_XTriggerActionItem* currentaction = NULL;
									for(iter_exceptedaction = currentsuccesstrigger->m_mapExceptedActionList.begin() ; iter_exceptedaction != currentsuccesstrigger->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
									{
										currentaction = iter_exceptedaction->second;
										if(currentaction)
										{
											if( currentaction->m_TDA.m_actionclass == ID_ACTION_GOTOTRIGGER )
											{
												return TRUE;
											}
										}
									}
								}//if( !currentsuccesstrigger->m_mapExceptedActionList.empty() )								
							}//if( !currentsuccesstrigger->m_mapNormalActionList.empty() )
						}
					}
				}				
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
		if(iter_quest != m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(treestep);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{			
						map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
						if(currentsuccesstrigger)
						{
							if( !currentsuccesstrigger->m_mapNormalActionList.empty() )
							{
								map <int, _XTriggerActionItem*>::iterator iter_normalaction;
								_XTriggerActionItem* currentaction = NULL;
								for(iter_normalaction = currentsuccesstrigger->m_mapNormalActionList.begin() ; iter_normalaction != currentsuccesstrigger->m_mapNormalActionList.end() ; ++iter_normalaction)
								{
									currentaction = iter_normalaction->second;
									if(currentaction)
									{
										if( currentaction->m_TDA.m_actionclass == ID_ACTION_GOTOTRIGGER )
										{
											return TRUE;
										}
									}
								}
								
								if( !currentsuccesstrigger->m_mapExceptedActionList.empty() )
								{
									map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
									_XTriggerActionItem* currentaction = NULL;
									for(iter_exceptedaction = currentsuccesstrigger->m_mapExceptedActionList.begin() ; iter_exceptedaction != currentsuccesstrigger->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
									{
										currentaction = iter_exceptedaction->second;
										if(currentaction)
										{
											if( currentaction->m_TDA.m_actionclass == ID_ACTION_GOTOTRIGGER )
											{
												return TRUE;
											}
										}
									}
								}//if( !currentsuccesstrigger->m_mapExceptedActionList.empty() )								
							}//if( !currentsuccesstrigger->m_mapNormalActionList.empty() )
						}
					}
				}				
				
			}
		}
	}
	
	return FALSE;
}

BOOL _XQuestScriptManager::CheckHaveQuestAutoNode( int questid) //현재 노드에 AutoNode가 있는지 체크한다.
{		
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(questid);
		if(iter_quest != m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(currentquest->GetCurrentTreeNumber());
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{			
						if( currenttree->m_bAutoNode )
						{
							return TRUE;
						}
						else
						{
							return FALSE;
						}
					}
				}				
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
		if(iter_quest != m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(currentquest->GetCurrentTreeNumber());
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{			
						if( currenttree->m_bAutoNode )
						{
							return TRUE;
						}
						else
						{
							return FALSE;
						}
					}
				}				
				
			}
		}
	}
	
	return FALSE;
}

// 시작 조건에 아이템 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 함수 
void _XQuestScriptManager::RecheckHaveItemTypeQuestStartCondition( char itemtype, short itemid ) 
{		
	// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
	bool balreadyexist = false;
	list <_XQUEST_ITEMINFO>::iterator iter_checkitem;
	for(iter_checkitem = m_listCheckItem.begin() ; iter_checkitem != m_listCheckItem.end() ;++iter_checkitem )
	{
		_XQUEST_ITEMINFO checkitem = *iter_checkitem;
		if( checkitem.itemtype == itemtype 
			&& checkitem.itemid	== itemid	)
		{
			balreadyexist = true;
			if( checkitem.questid >= 10000 && checkitem.questid < 20000 )
				SetMainQuestStartCondition( checkitem.questid );
			else
				SetQuestStartCondition( checkitem.questid );
		}
	}
	if( balreadyexist )
	{
		QuestStartConditionZoneMode();
		QuestStartConditionLevelMode();
	}				
}

// 시작 조건에 아이템 장착 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 함수 
void _XQuestScriptManager::RecheckWearItemTypeQuestStartCondition( char itemtype, short itemid ) 
{		
	// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
	bool balreadyexist = false;
	list <_XQUEST_ITEMINFO>::iterator iter_checkwearitem;
	for(iter_checkwearitem = m_listCheckWearItem.begin() ; iter_checkwearitem != m_listCheckWearItem.end() ;++iter_checkwearitem )
	{
		_XQUEST_ITEMINFO checkwearitem = *iter_checkwearitem;
		if( checkwearitem.itemtype == itemtype 
			&& checkwearitem.itemid	== itemid	)
		{
			balreadyexist = true;
			if( checkwearitem.questid >= 10000 && checkwearitem.questid < 20000 )
				SetMainQuestStartCondition( checkwearitem.questid );
			else
				SetQuestStartCondition( checkwearitem.questid );
		}
	}
	if( balreadyexist )
	{
		QuestStartConditionZoneMode();
		QuestStartConditionLevelMode();
	}				

				
}

// 시작 조건에 레벨 검사가 있으면 레벨 상승시에 다시 검사하기 위한 정보 ( 미만일때는 계속 체크해야 한다.)
void _XQuestScriptManager::RecheckLevelTypeQuestStartCondition( void ) 
{		
	// 퀘스트 시작 조건에 레벨 미만 체크가 있으면 가 있으면 시작조건의 아이템인지 체크를 한다.
	bool balreadyexist = false;
	list <_XQUEST_LEVELINFO>::iterator iter_checklevel;
	for(iter_checklevel = m_listCheckLevel.begin() ; iter_checklevel != m_listCheckLevel.end() ;++iter_checklevel )
	{
		_XQUEST_LEVELINFO checklevel = *iter_checklevel;
		if( checklevel.maximumlevel != 0 )
		{
			if( g_pLocalUser->m_CharacterInfo.Get_innerlevel() >=  checklevel.maximumlevel )
			{				
				balreadyexist = true;
				if( checklevel.questid >= 10000 && checklevel.questid < 20000 )
				{
					SetMainQuestStartCondition( checklevel.questid );
				}
				else
				{
					// 레벨제한이 넘어가면 더이상 퀘스트는 시작되지 않으므로 지워버린다.
					_XQuestScriptItem* currentquest = m_mapSubQuestScriptList[checklevel.questid];
					if( currentquest )
					{
						currentquest->ReSetQuestMark();
						currentquest->RemoveQuestStartConditionList();
						//SetQuestStartCondition( checklevel.questid );
					}
				}
			}
		}
		if( checklevel.equallevel != 0 )
		{
			if( g_pLocalUser->m_CharacterInfo.Get_innerlevel() !=  checklevel.equallevel )
			{
				balreadyexist = true;
				if( checklevel.questid >= 10000 && checklevel.questid < 20000 )
					SetMainQuestStartCondition( checklevel.questid );
				else
					SetQuestStartCondition( checklevel.questid );
			}
		}
	}
	if( balreadyexist )
	{
		QuestStartConditionZoneMode();
		QuestStartConditionLevelMode();
	}			
}

// 시작 조건에 동행가입 검사가 있으면 동행 탈퇴에 따라 다시 검사하기 위한 정보 
void _XQuestScriptManager::RecheckPartyTypeQuestStartCondition( void ) 
{		
	if( !m_listCheckInParty.empty() )
	{
		list <int>::iterator iter_checkinparty;
		for(iter_checkinparty = m_listCheckInParty.begin() ; iter_checkinparty != m_listCheckInParty.end() ; ++iter_checkinparty )
		{
			int partyquestid = *iter_checkinparty;
			if( partyquestid >= 10000 && partyquestid < 20000 )
				SetMainQuestStartCondition( partyquestid );
			else
				SetQuestStartCondition( partyquestid );
		}
		QuestStartConditionZoneMode();
		QuestStartConditionLevelMode();
	}
}

// 퀘스트가 시작되면 시작한 퀘스트에 관련된 리스트를 지운다.
void _XQuestScriptManager::DeleteRecheckingListAboutQuestStartCondition( int questid ) 
{	
	if( !m_listCheckItem.empty() )
	{
		// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
		list <_XQUEST_ITEMINFO>::iterator iter_checkitem;
		for(iter_checkitem = m_listCheckItem.begin() ; iter_checkitem != m_listCheckItem.end() ; )
		{
			_XQUEST_ITEMINFO checkitem = *iter_checkitem;
			if( checkitem.questid == questid )
			{
				iter_checkitem = m_listCheckItem.erase(iter_checkitem);
			}
			else
			{
				iter_checkitem++;
			}
		}
	}
	
	if( !m_listCheckWearItem.empty() )
	{
		// 퀘스트 시작 조건에 아이템 장착 체크가 있으면 시작조건의 아이템인지 체크를 한다.
		list <_XQUEST_ITEMINFO>::iterator iter_checkwearitem;
		for(iter_checkwearitem = m_listCheckWearItem.begin() ; iter_checkwearitem != m_listCheckWearItem.end() ; )
		{
			_XQUEST_ITEMINFO checkwearitem = *iter_checkwearitem;
			if( checkwearitem.questid == questid )
			{
				iter_checkwearitem = m_listCheckWearItem.erase(iter_checkwearitem);
			}
			else
			{
				iter_checkwearitem++;
			}
		}
	}
		
	if( !m_listCheckLevel.empty() )
	{
		// 퀘스트 시작 조건에 레벨 미만 체크가 있으면 가 있으면 시작조건의 아이템인지 체크를 한다.
		list <_XQUEST_LEVELINFO>::iterator iter_checklevel;
		for(iter_checklevel = m_listCheckLevel.begin() ; iter_checklevel != m_listCheckLevel.end() ; )
		{
			_XQUEST_LEVELINFO checklevel = *iter_checklevel;
			if( checklevel.questid == questid )
			{
				iter_checklevel = m_listCheckLevel.erase(iter_checklevel);
			}
			else
			{
				iter_checklevel++;
			}
		}
	}
	
	if( !m_listCheckInParty.empty() )
	{
		list <int>::iterator iter_checkinparty;
		for(iter_checkinparty = m_listCheckInParty.begin() ; iter_checkinparty != m_listCheckInParty.end() ; )
		{
			int partyquestid = *iter_checkinparty;
			if( partyquestid == questid )
			{
				iter_checkinparty = m_listCheckInParty.erase(iter_checkinparty);
			}
			else
			{
				iter_checkinparty++;
			}
		}
	}				
}

// 파티 퀘스트가 취소되면 파티퀘스트를 지우고 다시 퀘스트 시작조건에 넣는다.
void _XQuestScriptManager::DeletePartyQuest( void ) 
{	
	if( !m_listQuestNoSaveType.empty() )
	{		
		list <int>::iterator iter_questnosavetype;		
		iter_questnosavetype = m_listQuestNoSaveType.begin();
		while( iter_questnosavetype != m_listQuestNoSaveType.end() )
		{			
			int questid = *iter_questnosavetype;			
			if( questid != -1 )
			{
				_XQuestScriptItem* currentquest = m_mapSubQuestScriptList[questid];
				if( currentquest->GetPartyQuest() )
				{
					// 무분류는 알려주지 않는다.
					if( currentquest->GetQuestID() >=10000 )
					{
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_QUEST_2382), //_T("강호풍운록이 취소되었습니다")
							_XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
					}					
					g_pLocalUser->m_bPartyQuestProgress = FALSE;
					
					iter_questnosavetype = m_listQuestNoSaveType.erase(iter_questnosavetype);
					
					currentquest->Reset();
					// 저장되지 않는 퀘스트는 다시 퀘스트 시작조건 리스트에 들어간다.
					SetQuestStartCondition(currentquest->GetQuestID());
					
					// 다른 퀘스트의 시작 조건을 세팅한다.
					ResetQuestStartCondition();
					QuestStartConditionZoneMode();
					QuestStartConditionLevelMode();
					QuestStartConditionWearItemMode();
					
					// 저장 되지 않는 퀘스트는 퀘스트가 없어진다.
					if( g_pQuest_Window && g_pQuest_Window->GetShowStatus() )
					{
						if( g_pQuest_Window->m_SelectedQuestID == currentquest->GetQuestID() ) 
							g_pQuest_Window->m_SelectedItemIndex = -1;
						g_pQuest_Window->SetQuestTabButton();
						g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
					}									
				}
				else
				{
					iter_questnosavetype++;
				}
			}
			else
			{
				iter_questnosavetype = m_listQuestNoSaveType.erase(iter_questnosavetype);
			}
		}
	}
}

BOOL _XQuestScriptManager::CheckAllItemCounter(int itemtype, int itemid) 
{
	// 제룡행 퀘스트 검사 	
	if( g_QuestScriptManager.m_bProgressMainQuest )
	{
		if( CheckItemCounter( m_CurrentMainQuestID, itemtype, itemid ) )
			return TRUE;

	}				

	// 제룡행을 제외한 다른 퀘스트 검사
	for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
	{
		if( m_ProgressSubQuestID[i] != -1)
		{
			if( CheckItemCounter( m_ProgressSubQuestID[i], itemtype, itemid ) )
				return TRUE;
		}
	}//for( int i = 0; i < ID_QUEST_SYNC-1; i++ )

	// 저장되지 않는 퀘스트 검사 && 파티 퀘스트 검사
	if(!m_listQuestNoSaveType.empty())
	{
		list <int>::iterator iter_questnosavetype;
		for(iter_questnosavetype = m_listQuestNoSaveType.begin() ; iter_questnosavetype != m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
		{
			int questid = *iter_questnosavetype;
			if( CheckItemCounter( questid, itemtype, itemid ) )
			{
				return TRUE;
			}
			else if ( CheckPartyItemCounter(questid, itemtype, itemid ))
			{
				return TRUE;
			}

		}
	}
	return FALSE;

}	

BOOL _XQuestScriptManager::CheckItemCounter(int questid, int itemtype, int itemid ) 
{		
	_XQuestScriptItem* currentquest = NULL;
	
	if( questid >= 10000 && questid < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(questid);
		if(iter_quest != m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
		if(iter_quest != m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if( currentquest && !currentquest->GetPartyQuest() )
	{
		for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
		{
			if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType != -1 )
			{
				if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == itemtype 
					&& currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID == itemid )
				{
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(TCHAR)*256);
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == itemtype
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}
				
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == itemtype
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}
				#endif
					
					if( itemtotalcount != 0 )
					{
						if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint == 0 )
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,	itemtotalcount );
						}
						else if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
								itemtotalcount,
								currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
						}
						else if( itemtotalcount == currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
						{
							_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (완료)")
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
								currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
								currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
						}
						
						if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(m_lpszQuestSystemMessage )
								{
									delete m_lpszQuestSystemMessage;
									m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									m_lpszQuestSystemMessage[strlength] = 0;
									strncpy(m_lpszQuestSystemMessage, buffer, strlength);
								}
								
								m_bStartQuestCounterMessage = TRUE;
								m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}	
						else if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint == 0 )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(m_lpszQuestSystemMessage )
								{
									delete m_lpszQuestSystemMessage;
									m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									m_lpszQuestSystemMessage[strlength] = 0;
									strcpy(m_lpszQuestSystemMessage, buffer);
								}
								
								m_bStartQuestCounterMessage = TRUE;
								m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}
					}

					// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.								
					_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
					if( pMainFrame_Window && pMainFrame_Window->GetShowStatus() )
					{
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								g_pQuest_Window->ParseQuestDescString();
							}
						}
					}								
					return TRUE;
				}
			}
		}
	}
	else if( currentquest && currentquest->GetPartyQuest() )// 파티 아이템 카운터 일때 
	{
		for( int partyitemcounterindex = 0; partyitemcounterindex < 4; ++partyitemcounterindex )
		{
			if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetType != -1 )
			{
				if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetType == itemtype 
					&& currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetID == itemid )
				{
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(TCHAR)*256);
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == itemtype
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == itemtype
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();		
					}
				#endif
					
					currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterPointPerMember[g_pLocalUser->m_PartySelfIndex];

					int additemcount = itemtotalcount - currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterPointPerMember[g_pLocalUser->m_PartySelfIndex];
					
					currentquest->m_PartyCounterInformation[partyitemcounterindex].CountCurrentPoint = currentquest->m_PartyCounterInformation[partyitemcounterindex].CountCurrentPoint+additemcount;
					// 모든 동행원에게 파티 아이템 카운터의 변화를 알린다.
					g_NetworkKernel.SendPacket(MSG_NO_QUEST_SETCOUNT,questid,partyitemcounterindex,additemcount,currentquest->m_PartyCounterInformation[partyitemcounterindex].CountCurrentPoint);					
					
					
					// 파티아이템 카운터를 표시하는 부분
					if( itemtotalcount != 0 )
					{
						if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint == 0 )
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetName,	currentquest->m_PartyCounterInformation[partyitemcounterindex].CountCurrentPoint );
						}
						else if( itemtotalcount < currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint )
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetName,
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CountCurrentPoint,
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint );
						}
						else if( itemtotalcount == currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint )
						{
							_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (완료)")
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetName,
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint,
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint);
						}
						
						if( itemtotalcount <= currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(m_lpszQuestSystemMessage )
								{
									delete m_lpszQuestSystemMessage;
									m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									m_lpszQuestSystemMessage[strlength] = 0;
									strcpy(m_lpszQuestSystemMessage, buffer);
								}
								
								m_bStartQuestCounterMessage = TRUE;
								m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}	
						else if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint == 0 )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(m_lpszQuestSystemMessage )
								{
									delete m_lpszQuestSystemMessage;
									m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									m_lpszQuestSystemMessage[strlength] = 0;
									strcpy(m_lpszQuestSystemMessage, buffer);
								}
								
								m_bStartQuestCounterMessage = TRUE;
								m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}
					}

					// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.								
					_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
					if( pMainFrame_Window && pMainFrame_Window->GetShowStatus() )
					{
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								g_pQuest_Window->ParseQuestDescString();
							}
						}
					}					
							
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL _XQuestScriptManager::CheckPartyItemCounter(int questid, int itemtype, int itemid) 
{	
	_XQuestScriptItem* currentquest = NULL;
	
	if( questid >= 10000 && questid < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapQuestScriptList.find(questid);
		if(iter_quest != m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = m_mapSubQuestScriptList.find(questid);
		if(iter_quest != m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if( currentquest && currentquest->GetPartyQuest())
	{
		for( int partyitemcounterindex = 0; partyitemcounterindex < 4; ++partyitemcounterindex )
		{
			if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetType != -1 )
			{
				if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetType == itemtype 
					&& currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetID == itemid )
				{
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(TCHAR)*256);
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == itemtype
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == itemtype
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();		
					}
				#endif

					currentquest->m_PartyCounterInformation[partyitemcounterindex].CountCurrentPoint = itemtotalcount;
					
					
					// 파티아이템 카운터를 표시하는 부분
					if( itemtotalcount >= 0 )
					{
						if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint == 0 )
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetName,	itemtotalcount );
						}
						else if( itemtotalcount < currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint )
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetName,
								itemtotalcount,
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint );
						}
						else if( itemtotalcount == currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint )
						{
							_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (완료)")
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CounterTargetName,
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint,
								currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint);
						}
						
						if( itemtotalcount <= currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(m_lpszQuestSystemMessage )
								{
									delete m_lpszQuestSystemMessage;
									m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									m_lpszQuestSystemMessage[strlength] = 0;
									strcpy(m_lpszQuestSystemMessage, buffer);
								}
								
								m_bStartQuestCounterMessage = TRUE;
								m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}	
						else if( currentquest->m_PartyCounterInformation[partyitemcounterindex].CountTotalPoint == 0 )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(m_lpszQuestSystemMessage )
								{
									delete m_lpszQuestSystemMessage;
									m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									m_lpszQuestSystemMessage[strlength] = 0;
									strcpy(m_lpszQuestSystemMessage, buffer);
								}
								
								m_bStartQuestCounterMessage = TRUE;
								m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}
					}

					// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.								
					_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
					if( pMainFrame_Window && pMainFrame_Window->GetShowStatus() )
					{
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								g_pQuest_Window->ParseQuestDescString();
							}
						}
					}							
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}