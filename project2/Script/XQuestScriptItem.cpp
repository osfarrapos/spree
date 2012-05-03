/** 퀘스트 스크립트 하나를 나타내는 클래스.
 *
 * \author Sohyun, Park
 * \date 2003.09.04
 */

#include "stdafx.h"

#pragma warning (disable: 4786)

#include "stdio.h"
#include "xerror.h"
#include "XQuestScriptItem.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XTriggerTreeItem.h"
#include "XWindowObjectDefine.h"
#include "XParticleIDDefine.h"
#include "XWindow_QuestMail.h"

#include "XSR_STRINGHEADER.h"

_XQuestScriptItem::_XQuestScriptItem()
{
	m_bEnable		= FALSE;
	m_bAutorun		= FALSE;
	m_bMainQuest	= FALSE;
	m_bPartyQuest	= FALSE;
	m_bTempQuest	= false;

	m_bClientEnable	= FALSE;
	m_bServerEnable	= FALSE;
	m_bConfirmAll	= FALSE;

	m_bRewardProcessNPCMarkCheck = FALSE;

	m_nQuestID				= 0;
	m_QuestChapter			= -1;
	m_lpszQuestName			= NULL;
	m_lpszQuestSubName		= NULL;

	m_lpszStartQuestMessage	= NULL;
	m_lpszFailQuestMessage	= NULL;
	m_lpszOutlineMessage	= NULL;
	m_lpszOutlineMessage2	= NULL;
	m_startquestinformationmessage = NULL;
	m_lpszQuestProgressinformation = NULL;

	m_byQuestCancelCase				= 0;
	m_byStartConditionCompareClass	= ID_TRIGGER_CONDITIONFUNCTION_OR;
	m_byQuestOptionCompareClass	= ID_TRIGGER_CONDITIONFUNCTION_OR;
	
	m_RankQuestType1 = 0;		//0:개인퀘스트 1:동행퀘스트 2:단퀘스트 
	m_RankQuestType2 = 0;		//0:KillCount 1:Timer 2:ItemCount	

	m_QuestType				= _XQUEST_TYPE_NONE;

	m_bComplete				= FALSE;
	m_bInsertCompleteList	= FALSE;

	m_nStartTime	= 0;
	m_nQuestMailDelayTime	= 0;

	m_nNextTriggerID		= 0;
	m_nCurrentTriggerID		= 0;

	m_bWaitingFailResult	= FALSE;

	m_nLastTargettingNPCID	= 0;
	m_bCheckFirstStartCondition = FALSE;

	m_QuestScriptItemIndexNumber = -1;
	m_bSelectedQuestScriptItem = FALSE;

	m_bChangeQuestStartMode = FALSE;

	m_SelectionNumber = -1;
	m_bSelectionNumberCheck = FALSE;
	
	m_queststartprobability = 0.0f;
	
	m_NextTreeNodeNumber		= 0;
	m_CurrentTreeNodeNumber		= 0;

	m_FailConditionTimer		= 0;

	m_QuestStartConditionValue = 0;
	for( int i=0; i < 20; ++i)
	{
		m_SuccessStartConditionid[i] = -1;
	}

	ResetCounterInformation();
	ResetPartyCounterInformation();
	ResetItemCounterInformation();
	
	m_LimitQuestCountStartType	= 0;
	m_LimitQuestCountCompleteType = 0;
	
	m_bSendClientPacketAtPartyQuest = FALSE;

	m_nStartYear	= 0;
	m_nStartMonth	= 0;
	m_nStartDay		= 0;
	m_nStartHour	= 0;

	m_nEndYear		= 0;
	m_nEndMonth		= 0;
	m_nEndDay		= 0;
	m_nEndHour		= 0;

	m_unCountryCode	= 0;
	
}

_XQuestScriptItem::~_XQuestScriptItem()
{
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM

#else
	SAFE_DELETE_ARRAY(m_lpszQuestName);
	SAFE_DELETE_ARRAY(m_lpszQuestSubName);
	SAFE_DELETE_ARRAY(m_lpszStartQuestMessage);
	SAFE_DELETE_ARRAY(m_lpszFailQuestMessage);
	SAFE_DELETE_ARRAY(m_lpszOutlineMessage);
	SAFE_DELETE_ARRAY(m_lpszOutlineMessage2);
	SAFE_DELETE_ARRAY(m_startquestinformationmessage);
#endif

	map <int, _XTriggerItem*>::iterator iter_trigger;
	_XTriggerItem* currenttrigger = NULL;

	for(iter_trigger = m_mapSuccessTriggerList.begin() ; iter_trigger != m_mapSuccessTriggerList.end() ; ++iter_trigger)
	{
		currenttrigger = iter_trigger->second;
		if(currenttrigger)
		{
			delete currenttrigger;
			currenttrigger = NULL;
		}
	}
	m_mapSuccessTriggerList.clear();

//	currenttrigger = NULL;
//	for(iter_trigger = m_mapFailTriggerList.begin() ; iter_trigger != m_mapFailTriggerList.end() ; iter_trigger++)
//	{
//		currenttrigger = iter_trigger->second;
//		if(currenttrigger)
//		{
//			delete currenttrigger;
//			currenttrigger = NULL;
//		}
//	}
//	m_mapFailTriggerList.clear();


	map <int, _XTriggerConditionItem*>::iterator iter_condition;
	_XTriggerConditionItem* currentcondition = NULL;
	for(iter_condition = m_mapStartConditionList.begin() ; iter_condition != m_mapStartConditionList.end() ; ++iter_condition)
	{
		currentcondition = iter_condition->second;
		if(currentcondition)
		{
			delete currentcondition;
			currentcondition = NULL;
		}
	}
	m_mapStartConditionList.clear();
	
	// 옵션 조건식 초기화
	map <int, _XTriggerConditionItem*>::iterator iter_optioncondition;
	for(iter_optioncondition = m_mapOptionConditionList.begin() ; iter_optioncondition != m_mapOptionConditionList.end() ; ++iter_optioncondition)
	{
		_XTriggerConditionItem* currentcondition = iter_optioncondition->second;
		if(currentcondition)
		{
			delete currentcondition;
			currentcondition = NULL;
		}
	}
	m_mapOptionConditionList.clear();
	
	// 퀘스트 실패 조건식 초기화
	m_mapFailConditionList.clear();		
	m_mapTriggerTimer.clear();

	
	map <int, _XTriggerTreeItem*>::iterator iter_tree;
	_XTriggerTreeItem* currenttree = NULL;
	
	for(iter_tree = m_mapTriggerTreeList.begin() ; iter_tree != m_mapTriggerTreeList.end() ; ++iter_tree)
	{
		currenttree = iter_tree->second;
		if(currenttree)
		{
			delete currenttree;
			currenttree = NULL;
		}
	}
	m_mapTriggerTreeList.clear();
}

void _XQuestScriptItem::SetStartTime(int starttime)
{
	m_nStartTime = starttime;
}

int _XQuestScriptItem::GetStartTime(void)
{
	return m_nStartTime;
}



void _XQuestScriptItem::SetConfirmAll(BOOL enable)
{
	m_bConfirmAll = enable;
}

void _XQuestScriptItem::SetTriggerClientEnable(int triggerid, BOOL enable )
{
	map <int, _XTriggerItem*>::iterator iter;
	_XTriggerItem* currenttrigger = NULL;

	iter = m_mapSuccessTriggerList.find(triggerid);
	if(iter != m_mapSuccessTriggerList.end())
	{
		currenttrigger = iter->second;
		if(currenttrigger)
		{
			currenttrigger->SetClientEnable(enable);
		}
	}

}

void _XQuestScriptItem::SetTriggerServerEnable(int currenttreenumber, BOOL enable, BOOL jumpaction)
{
	
	map <int, _XTriggerTreeItem*>::iterator iter_triggertree = m_mapTriggerTreeList.find(currenttreenumber);
	if(iter_triggertree != m_mapTriggerTreeList.end())
	{
		_XTriggerTreeItem* currenttree = iter_triggertree->second;
		if(currenttree)
		{
			map <int, _XTriggerItem*>::iterator iter;
			_XTriggerItem* currenttrigger = NULL;
			
			iter = m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
			if(iter != m_mapSuccessTriggerList.end())
			{
				currenttrigger = iter->second;
				if(currenttrigger)
				{
					currenttrigger->SetServerEnable(enable);
					currenttrigger->SetJumpAction(jumpaction);				
				}
			}
			
		}
	}

}

void _XQuestScriptItem::SetTriggerPlayActionAndServerEnable(int currenttreenumber, BOOL enable, BOOL bnormalaction, BOOL jumpaction)
{
	
	map <int, _XTriggerTreeItem*>::iterator iter_triggertree = m_mapTriggerTreeList.find(currenttreenumber);
	if(iter_triggertree != m_mapTriggerTreeList.end())
	{
		_XTriggerTreeItem* currenttree = iter_triggertree->second;
		if(currenttree)
		{
			map <int, _XTriggerItem*>::iterator iter;
			_XTriggerItem* currenttrigger = NULL;
			
			iter = m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
			if(iter != m_mapSuccessTriggerList.end())
			{
				currenttrigger = iter->second;
				if(currenttrigger)
				{
					currenttrigger->SetServerEnable(enable);
					currenttrigger->SetPlayNormalAction(bnormalaction);
					currenttrigger->SetJumpAction(jumpaction);				
				}
			}
			
		}
	}
	
}

void _XQuestScriptItem::SetTriggerConfirmAll(int triggerid, BOOL enable )
{
	map <int, _XTriggerItem*>::iterator iter;
	_XTriggerItem* currenttrigger = NULL;

	iter = m_mapSuccessTriggerList.find(triggerid);
	if(iter != m_mapSuccessTriggerList.end())
	{
		currenttrigger = iter->second;
		if(currenttrigger)
		{
			currenttrigger->SetConfirmAll(enable);
		}
	}
}

void _XQuestScriptItem::SetProgressTrigger(int currenttreenumber, BOOL bNormal)
{
	
	map <int, _XTriggerTreeItem*>::iterator iter_triggertree = m_mapTriggerTreeList.find(currenttreenumber);
	if(iter_triggertree != m_mapTriggerTreeList.end())
	{
		_XTriggerTreeItem* currenttree = iter_triggertree->second;
		if(currenttree)
		{
			map <int, _XTriggerItem*>::iterator iter;
			_XTriggerItem* currenttrigger = NULL;
			
			iter = m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
			if(iter != m_mapSuccessTriggerList.end())
			{
				currenttrigger = iter->second;
				if(currenttrigger)
				{
					currenttrigger->SetProgressAction( bNormal );
				}
			}
			
		}
	}

}

void _XQuestScriptItem::SetTriggerComplete(int triggerid, BOOL enable)
{
	map <int, _XTriggerItem*>::iterator iter;
	_XTriggerItem* currenttrigger = NULL;

	iter = m_mapSuccessTriggerList.find(triggerid);
	if(iter != m_mapSuccessTriggerList.end())
	{
		currenttrigger = iter->second;
		if(currenttrigger)
		{
			currenttrigger->SetComplete(enable);
		}
	}


}

void _XQuestScriptItem::SetCurrentTriggerID(int triggerid)
{
	m_nCurrentTriggerID = triggerid;
}

int _XQuestScriptItem::GetCurrentTriggerID(void)
{
	return m_nCurrentTriggerID;
}

void _XQuestScriptItem::ReSetQuestMark()
{
//	if( !g_QuestScriptManager.m_listNPCTargettingQuest.empty() ) return; 
	
	if( !g_QuestScriptManager.m_listQuestMark.empty() )
	{
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		
		iter_questmark = g_QuestScriptManager.m_listQuestMark.begin();
		while( iter_questmark != g_QuestScriptManager.m_listQuestMark.end() )
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
			
			if( questmark.questid == m_nQuestID )
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
							
							// 느낌표와 물음표를 가지는 퀘스트들을 전부 검사해야 한다. 
							// 물음표를 가지는 NPC도 퀘스트를 가지고 있는 것이다.

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
							{								
								g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;			
							}
#else
							g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;	
#endif
							break;
						}
					}
				}
				questmark.MarkPosition_X = -9999;
				questmark.MarkPosition_Y = -9999;
				questmark.questid = -1;
				questmark.npcid = -1;
				questmark.questtype = _XQUEST_TYPE_NONE;
				iter_questmark = g_QuestScriptManager.m_listQuestMark.erase(iter_questmark);
			}
			else
			{
				iter_questmark++;
			}
		}
	}
	
	if( !g_QuestScriptManager.m_listHintMark.empty() )
	{
		list <_XQUEST_MARKINFO>::iterator iter_hinttmark;
		
		iter_hinttmark = g_QuestScriptManager.m_listHintMark.begin();
		while( iter_hinttmark != g_QuestScriptManager.m_listHintMark.end() )
		{
			_XQUEST_MARKINFO hintmark = *iter_hinttmark;
			
			if( hintmark.questid == m_nQuestID )
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
							
							// 느낌표와 물음표를 가지는 퀘스트들을 전부 검사해야 한다. 
							// 물음표를 가지는 NPC도 퀘스트를 가지고 있는 것이다.

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
				hintmark.MarkPosition_X = -9999;
				hintmark.MarkPosition_Y = -9999;
				hintmark.questid = -1;
				hintmark.npcid = -1;
				hintmark.questtype = _XQUEST_TYPE_NONE;
				iter_hinttmark = g_QuestScriptManager.m_listHintMark.erase(iter_hinttmark);
			}
			else
			{
				iter_hinttmark++;
			}
		}
	}

}

void _XQuestScriptItem::SetCurrentTreeNumber(int treenumber, BOOL bconditioncheck )
{
	m_CurrentTreeNodeNumber = treenumber;
	
	map <int, _XTriggerTreeItem*>::iterator iter_triggertree = m_mapTriggerTreeList.find(m_CurrentTreeNodeNumber);
	if(iter_triggertree != m_mapTriggerTreeList.end())
	{
		_XTriggerTreeItem* currenttree = iter_triggertree->second;
		if(currenttree)
		{
			if( currenttree->m_TriggerUniqueID )
			{
				SetCurrentTriggerID( currenttree->m_TriggerUniqueID );
				
				map <int, _XTriggerItem*>::iterator iter_successtrigger = m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
				_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
				if(currentsuccesstrigger)
				{
					if( bconditioncheck )
					{
						currentsuccesstrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
						_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", currenttree->m_TriggerUniqueID);
#endif
					}
					else // 트리거에서 조건식 체크를 막는 부분 
					{
						currentsuccesstrigger->SetClientEnable(TRUE);
#ifdef _XDWDEBUG	
						_XLog("Quest Trigger Condition Check Close : TriggerID[%d]", currenttree->m_TriggerUniqueID);
#endif
					}
					currentsuccesstrigger->SetServerEnable(FALSE);
					currentsuccesstrigger->SetJumpAction(FALSE);
					currentsuccesstrigger->SetTreeNodeNumber( m_CurrentTreeNodeNumber );
					
					if( currenttree->m_FLeftNodeUniqueID != 0 && currenttree->m_TRightNodeUniqueID != 0 )
					{
						currentsuccesstrigger->SetTriggerClass( ID_TRIGGER_CLASS_JUMP );						
					}
					else if( currenttree->m_FLeftNodeUniqueID == 0 && currenttree->m_TRightNodeUniqueID == 0 )
					{
						currentsuccesstrigger->SetTriggerClass( ID_TRIGGER_CLASS_END );
					}
					else
					{
						currentsuccesstrigger->SetTriggerClass( ID_TRIGGER_CLASS_NORMAL );
					}
					ReSetQuestMark();	
					
					if( bconditioncheck )
					{
						map <int, _XTriggerConditionItem*>::iterator iter_triggercondition;
						_XTriggerConditionItem* currentcondition = NULL;
						for(iter_triggercondition = currentsuccesstrigger->m_mapConditionList.begin() ; iter_triggercondition != currentsuccesstrigger->m_mapConditionList.end() ; ++iter_triggercondition)
						{
							currentcondition = iter_triggercondition->second;
							if(currentcondition)
							{	
								
								if(currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKATTACK
									&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_CHECKATTACK_DEATH) 
								{
									if( currentcondition->m_TDC.m_numericvalue2 >= 1 )
									{
										int countertype = currentcondition->m_TDC.m_compareclass2;
										if( countertype < 8 ) //몬스터 표시 타입 
										{
											if( countertype >= 4 ) // 파티 카운터 
											{
												countertype -= 4;
												m_PartyCounterInformation[countertype].CounterType = countertype;
												m_PartyCounterInformation[countertype].CounterTargetID = currentcondition->m_TDC.m_numericvalue1;
												m_PartyCounterInformation[countertype].CountTotalPoint = currentcondition->m_TDC.m_numericvalue2;
												m_PartyCounterInformation[countertype].CountCurrentPoint = 0;
												
												int mobid = m_PartyCounterInformation[countertype].CounterTargetID- 4000;
												memset( m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
												if( g_MonsterName[mobid] )
													strcpy( m_PartyCounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
												else
													strcpy( m_PartyCounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
											}
											else
											{
												m_CounterInformation[countertype].CounterType = countertype;
												m_CounterInformation[countertype].CounterTargetID = currentcondition->m_TDC.m_numericvalue1;
												m_CounterInformation[countertype].CountTotalPoint = currentcondition->m_TDC.m_numericvalue2;
												m_CounterInformation[countertype].CountCurrentPoint = 0;
												
												int mobid = m_CounterInformation[countertype].CounterTargetID- 4000;
												memset( m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
												if( g_MonsterName[mobid] )
													strcpy( m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
												else
													strcpy( m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
											}
											
										}
										else if( countertype == 8 )// 아이템 표시 타입 
										{										
											bool bexist = false;
											for( int i = 0; i < 8; ++i )
											{
												if( m_ItemCounterInformation[i].CounterTargetType == currentcondition->m_TDC.m_compareclass3 
													&& m_ItemCounterInformation[i].CounterTargetID == currentcondition->m_TDC.m_numericvalue3
													&& m_ItemCounterInformation[i].CountTotalPoint == currentcondition->m_TDC.m_numericvalue2 )
												{
													bexist = true;
													break;
												}
											}
											
											if( !bexist )
											{
												for( int i = 0; i < 8; ++i )
												{
													if( m_ItemCounterInformation[i].CounterTargetType == -1 )
													{
														m_ItemCounterInformation[i].CounterTargetType = currentcondition->m_TDC.m_compareclass3; // 타입
														m_ItemCounterInformation[i].CounterTargetID = currentcondition->m_TDC.m_numericvalue3;   // 아이디 
														m_ItemCounterInformation[i].CountTotalPoint = currentcondition->m_TDC.m_numericvalue2;	 // 총 개수 
														m_ItemCounterInformation[i].CountCurrentPoint = 0;
														
														char cType = m_ItemCounterInformation[i].CounterTargetType;
														short sID = m_ItemCounterInformation[i].CounterTargetID;
														
														memset(m_ItemCounterInformation[i].CounterTargetName, 0, sizeof(TCHAR)*256);
														strcpy(m_ItemCounterInformation[i].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
														
														break;
													}
												}
											}
										}//else if( countertype == 8 )// 아이템 표시 타입 
									}
								}															
							}
						}// for
					}
					
					if( g_pQuest_Window )
					{
						if( g_pQuest_Window->GetShowStatus() )
						{	
							if( g_pQuest_Window->m_SelectedQuestID == m_nQuestID  )
								g_pQuest_Window->ParseQuestDescString();	
						}					
					}
					
				}
			}
		}
	}
}

int _XQuestScriptItem::GetCurrentTreeNumber(void)
{
	return m_CurrentTreeNodeNumber;
}

void _XQuestScriptItem::SetNextTreeNumber(int nexttreenumber)
{
	m_NextTreeNodeNumber = nexttreenumber;
	
	map <int, _XTriggerTreeItem*>::iterator iter_triggertree = m_mapTriggerTreeList.find(m_NextTreeNodeNumber);
	if(iter_triggertree != m_mapTriggerTreeList.end())
	{
		_XTriggerTreeItem* currenttree = iter_triggertree->second;
		if(currenttree)
		{			
			map <int, _XTriggerItem*>::iterator iter_successtrigger = m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
			_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
			if(currentsuccesstrigger)
			{
				currentsuccesstrigger->SetClientEnable(TRUE);
#ifdef _XDWDEBUG	
				_XLog("Quest Trigger Condition Check Close : TriggerID[%d]", currenttree->m_TriggerUniqueID);
#endif
				currentsuccesstrigger->SetServerEnable(FALSE);
				currentsuccesstrigger->SetTreeNodeNumber( m_NextTreeNodeNumber );
				
				if( currenttree->m_FLeftNodeUniqueID != 0 && currenttree->m_TRightNodeUniqueID != 0 )
				{
					currentsuccesstrigger->SetTriggerClass( ID_TRIGGER_CLASS_JUMP );
				}
				else if( currenttree->m_FLeftNodeUniqueID == 0 && currenttree->m_TRightNodeUniqueID == 0 )
				{
					currentsuccesstrigger->SetTriggerClass( ID_TRIGGER_CLASS_END );
				}
				else
				{
					currentsuccesstrigger->SetTriggerClass( ID_TRIGGER_CLASS_NORMAL );
				}

				ReSetQuestMark();				
				
				// 카운터가 있으면 설정한다
				map <int, _XTriggerConditionItem*>::iterator iter_triggercondition;
				_XTriggerConditionItem* currentcondition = NULL;
				for(iter_triggercondition = currentsuccesstrigger->m_mapConditionList.begin() ; iter_triggercondition != currentsuccesstrigger->m_mapConditionList.end() ; ++iter_triggercondition)
				{
					currentcondition = iter_triggercondition->second;
					if(currentcondition)
					{											
						if(currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKATTACK
							&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_CHECKATTACK_DEATH) 
						{
							if( currentcondition->m_TDC.m_numericvalue2 >= 1 )
							{
								int countertype = currentcondition->m_TDC.m_compareclass2;
								if( countertype < 8 ) //몬스터 표시 타입 
								{
									if( countertype >= 4 ) // 파티 카운터 
									{
										countertype -= 4;
										m_PartyCounterInformation[countertype].CounterType = countertype;
										m_PartyCounterInformation[countertype].CounterTargetID = currentcondition->m_TDC.m_numericvalue1;
										m_PartyCounterInformation[countertype].CountTotalPoint = currentcondition->m_TDC.m_numericvalue2;
										m_PartyCounterInformation[countertype].CountCurrentPoint = 0;
										
										int mobid = m_PartyCounterInformation[countertype].CounterTargetID- 4000;
										memset( m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
										if( g_MonsterName[mobid] )
											strcpy( m_PartyCounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
										else
											strcpy( m_PartyCounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
									}
									else
									{
										m_CounterInformation[countertype].CounterType = countertype;
										m_CounterInformation[countertype].CounterTargetID = currentcondition->m_TDC.m_numericvalue1;
										m_CounterInformation[countertype].CountTotalPoint = currentcondition->m_TDC.m_numericvalue2;
										m_CounterInformation[countertype].CountCurrentPoint = 0;
										
										int mobid = m_CounterInformation[countertype].CounterTargetID- 4000;
										memset( m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
										if( g_MonsterName[mobid] )
											strcpy( m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
										else
											strcpy( m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
									}
									
								}
								else if( countertype == 8) // 아이템 표시 타입 
								{
									
									bool bexist = false;
									for( int i = 0; i < 8; ++i )
									{
										if( m_ItemCounterInformation[i].CounterTargetType == currentcondition->m_TDC.m_compareclass3 
											&& m_ItemCounterInformation[i].CounterTargetID == currentcondition->m_TDC.m_numericvalue3
											&& m_ItemCounterInformation[i].CountTotalPoint == currentcondition->m_TDC.m_numericvalue2 )
										{
											bexist = true;
											break;
										}
									}
									
									if( !bexist )
									{
										for( int i = 0; i < 8; ++i )
										{
											if( m_ItemCounterInformation[i].CounterTargetType == -1 )
											{
												m_ItemCounterInformation[i].CounterTargetType = currentcondition->m_TDC.m_compareclass3; // 타입
												m_ItemCounterInformation[i].CounterTargetID = currentcondition->m_TDC.m_numericvalue3;   // 아이디 
												m_ItemCounterInformation[i].CountTotalPoint = currentcondition->m_TDC.m_numericvalue2;	 // 총 개수 
												m_ItemCounterInformation[i].CountCurrentPoint = 0;
												
												char cType = m_ItemCounterInformation[i].CounterTargetType;
												short sID = m_ItemCounterInformation[i].CounterTargetID;
												
												memset(m_ItemCounterInformation[i].CounterTargetName, 0, sizeof(TCHAR)*256);
												strcpy(m_ItemCounterInformation[i].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
												
												break;
											}
										}
									}
								}
							}
						}											
					}
				}// for
				
				if( g_pQuest_Window )
				{
					if( g_pQuest_Window->GetShowStatus() )
					{	
						if( g_pQuest_Window->m_SelectedQuestID == m_nQuestID  )
							g_pQuest_Window->ParseQuestDescString();	
					}					
				}
			}
		}
	}
}

int _XQuestScriptItem::GetNextTreeNumber(void)
{
	return m_NextTreeNodeNumber;
}

void _XQuestScriptItem::SetNextTriggerID(int triggerid)
{
	m_nNextTriggerID = triggerid;
}

int _XQuestScriptItem::GetNextTriggerID(void)
{
	return m_nNextTriggerID;
}

void _XQuestScriptItem::SetQuestID(int questid)
{
	m_nQuestID = questid;
}

int _XQuestScriptItem::GetQuestID(void)
{
	return m_nQuestID;
}

void _XQuestScriptItem::SetComplete(BOOL complete)
{
	m_bComplete = complete;
}

BOOL _XQuestScriptItem::GetComplete(void)
{
	return m_bComplete;
}

void _XQuestScriptItem::SetInsertCompleteList(BOOL complete)
{
	m_bInsertCompleteList = complete;
}

BOOL _XQuestScriptItem::GetInsertCompleteList(void)
{
	return m_bInsertCompleteList;
}

void _XQuestScriptItem::SetAutoRun(BOOL autorun)
{
	m_bAutorun = autorun;
}

BOOL _XQuestScriptItem::GetAutoRun(void)
{
	return m_bAutorun;
}

void _XQuestScriptItem::SetWaitingFailResult(BOOL waiting)
{
	m_bWaitingFailResult = waiting;
}

BOOL _XQuestScriptItem::GetWaitingFailResult(void)
{
	return m_bWaitingFailResult;
}

_XQUESTSTARTCONDITION_RESULT _XQuestScriptItem::CheckStartCondition(void)
{	
	if(!m_bClientEnable)
	{		

#ifdef _XDEF_QUESTSCRIPT_11

		// 시작 날짜, 종료 날짜 검사
		if(!CheckStartNEndTime())
			return QUEST_STARTCONDITION_RESULT_FAIL;

		// 국가 코드 검사
		if(!CheckCountryCode())
			return QUEST_STARTCONDITION_RESULT_FAIL;

#endif

		if(!m_mapStartConditionList.empty())
		{
			int startconditionsuccesscount = 0;
			m_bChangeQuestStartMode = FALSE;
			map <int, _XTriggerConditionItem*>::iterator iter_condition;
			_XTriggerConditionItem* currentcondition = NULL;
			for(iter_condition = m_mapStartConditionList.begin() ; iter_condition != m_mapStartConditionList.end() ; ++iter_condition)
			{
				currentcondition = iter_condition->second;
				if(currentcondition)
				{
					currentcondition->m_bStartCondition = TRUE;//시작 조건일때 NPC타게팅 체크는 시간 체크를 하지 않는다.
					
					bool checksuccess = FALSE;
					for( int i = 0; i < m_mapStartConditionList.size(); ++i )
					{
						if( m_SuccessStartConditionid[i] == currentcondition->GetConditionID() )
						{
							checksuccess = TRUE;
							startconditionsuccesscount++;
							break;
						}
						
					}

					
					if( !checksuccess )
					{
						BOOL bskipcondition = FALSE;
						switch(currentcondition->m_TDC.m_conditionclass) 
						{
						case ID_CONDITION_SETQUESTMARKAREA :	// QUEST마크(!) 표시하기 영역
						case ID_CONDITION_SETQUESTMARKNPC :		// QUEST마크(!) 표시하기 NPC 
						case ID_CONDITION_SETHINTMARKAREA :		// HINT마크(?) 표시하기 영역
						case ID_CONDITION_SETHINTMARKNPC :		// HINT마크(?) 표시하기 NPC 
							bskipcondition = TRUE;								
							break;
						case ID_CONDITION_CHECKTARGETTING:		// NPC 타게팅 모드가 아닌데 타게팅 체크가 있으면 건너 뛴다.
							{
								if( currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC )
								{
									if( m_QuestStartMode != _XQUESTSTART_NPCTARGETTING )
										bskipcondition = TRUE;	
								}
							}					
							break;
						case ID_CONDITION_CHECKQUESTEMAIL:		// NPC 타게팅 모드가 아닌데 풍운방 체크가 있으면 건너 뛴다
							{
								if( m_QuestStartMode != _XQUESTSTART_NPCTARGETTING )
									bskipcondition = TRUE;	
							}
							break;
						}

						if( !bskipcondition )
						{
							if( currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
							{
								bool applyqueststartvalue = false;
								// 퀘스트 시작 조건 모드를 설정하는 부분(NPC,OBJECT,ITEM,LEVEL,ZONE)
								if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
									&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC )
								{
									if( m_QuestStartMode == _XQUESTSTART_NPCTARGETTING )
									{
										m_QuestStartConditionValue -= 1;
										applyqueststartvalue = true;
									}
								}								
								else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKQUESTEMAIL )
								{
									// 퀘스트 시작에서 풍운방 체크도 NPC Targetting모드로 들어감
									if( m_QuestStartMode == _XQUESTSTART_NPCTARGETTING )
									{
										m_QuestStartConditionValue -= 1;
										applyqueststartvalue = true;
									}
								}
								else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKDESTORYITEM )
								{
									// 아이템 파괴 체크 
									if( m_QuestStartMode == _XQUESTSTART_DESTROYITEM )
									{
										m_QuestStartConditionValue -= 1000000;
										applyqueststartvalue = true;
									}
								}
								else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
									&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_OBJECT )
								{
									// 오브젝트 타게팅 체크 
									if( m_QuestStartMode == _XQUESTSTART_OBJECTCLICK )
									{
										m_QuestStartConditionValue -= 100000;
										applyqueststartvalue = true;
									}
								}
								else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
									&& currentcondition->m_TDC.m_compareclass2 == 1 )//0:아이템 장착체크 1:아이템 사용체크 
								{
									if( m_QuestStartMode == _XQUESTSTART_USINGITEM )
									{
										m_QuestStartConditionValue -= 10000;	
										applyqueststartvalue = true;								
									}
								}
								else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKWEARITEM 
									&& currentcondition->m_TDC.m_compareclass2 == 0 )//0:아이템 장착체크 
								{
									if( m_QuestStartMode == _XQUESTSTART_WEARITEM )
									{
										m_QuestStartConditionValue -= 1000;
										applyqueststartvalue = true;
									}
								}
								else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKVALUE 
									&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_CHECKVALUETYPE_TOTALLEVEL )
								{
									if( m_QuestStartMode == _XQUESTSTART_LEVEL )
									{
										m_QuestStartConditionValue -= 100;
										applyqueststartvalue = true;
									}
								}				
								else if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKAREA )
								{									
									if( m_QuestStartMode == _XQUESTSTART_ZONE )
									{
										m_QuestStartConditionValue -= 10;
										applyqueststartvalue = true;
									}
								}
								else
								{
									if( m_QuestStartMode == _XQUESTSTART_ZONE )
									{
										m_QuestStartConditionValue -= 10;
										applyqueststartvalue = true;
									}
								}
								
								if( applyqueststartvalue )
								{
									for( int i = 0; i < 20; ++i )
									{
										if( m_SuccessStartConditionid[i] == -1 )
										{
											if( m_QuestStartMode == _XQUESTSTART_NPCTARGETTING )
											{
												m_SuccessStartConditionid[i] = currentcondition->GetConditionID();
												
#ifdef _XDWDEBUG	
												_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
												_XLog("Quest StartCondition Success : QuestID[%d] StartConditionID[%d], QuestStartConditionValue[%d] ", m_nQuestID, currentcondition->GetConditionID(),m_QuestStartConditionValue );
#endif
												
											}
											else
											{
												
												if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING 
													&& currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC)
												{
												}
												else if ( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKQUESTEMAIL )
												{
												}
												else
												{
													m_SuccessStartConditionid[i] = currentcondition->GetConditionID();
													
#ifdef _XDWDEBUG	
													_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
													_XLog("Quest StartCondition Success : QuestID[%d] StartConditionID[%d], QuestStartConditionValue[%d] ", m_nQuestID, currentcondition->GetConditionID(),m_QuestStartConditionValue );
#endif
													
												}

											}
											
											break;
										}
									}
									

									// 기존 시작 모드를 삭제한 후 시작 모드를 다시 계산하여 세팅한다.
									if( m_QuestStartConditionValue > 999999 ) // 1000000번대이므로 DestroyItem
									{
										if( m_QuestStartMode != _XQUESTSTART_DESTROYITEM)
										{
											m_QuestStartMode = _XQUESTSTART_DESTROYITEM;
											m_bChangeQuestStartMode = TRUE;				
										}						
									}
									else if( m_QuestStartConditionValue > 99999 ) // 100000번대이므로 ObjectClick
									{
										if( m_QuestStartMode != _XQUESTSTART_OBJECTCLICK)
										{
											m_QuestStartMode = _XQUESTSTART_OBJECTCLICK ;
											m_bChangeQuestStartMode = TRUE;				
										}						
									}
									else if( m_QuestStartConditionValue > 9999 ) // 10000번대이므로 Using Item
									{
										if( m_QuestStartMode != _XQUESTSTART_USINGITEM)
										{
											m_QuestStartMode = _XQUESTSTART_USINGITEM ;
											m_bChangeQuestStartMode = TRUE;			
										}	
									}			
									else if( m_QuestStartConditionValue > 999 ) // 1000번대이므로 Wear Item
									{
										if( m_QuestStartMode != _XQUESTSTART_WEARITEM)
										{
											m_QuestStartMode = _XQUESTSTART_WEARITEM ;
											m_bChangeQuestStartMode = TRUE;				
										}
									}			
									else if( m_QuestStartConditionValue > 99 ) // 100번대이므로 Level Up
									{
										if( m_QuestStartMode != _XQUESTSTART_LEVEL)
										{
											m_QuestStartMode = _XQUESTSTART_LEVEL ;
											m_bChangeQuestStartMode = TRUE;			
										}	
									}			
									else if( m_QuestStartConditionValue > 9 ) // 10번대이므로 Zone or None
									{
										if( m_QuestStartMode != _XQUESTSTART_ZONE)
										{
											m_QuestStartMode = _XQUESTSTART_ZONE ;
											m_bChangeQuestStartMode = TRUE;			
										}	
									}
									else if( m_QuestStartConditionValue >= 0 ) // 1번대이므로 NPC targetting
									{
										if( m_QuestStartMode != _XQUESTSTART_NPCTARGETTING)
										{
											bool balreadyexistpartyquest = false;
											// 파티 퀘스트면 다른 파티 퀘스트가 있는지 확인을 하고 없으면 진행한다. 동시에 두개의 파티퀘스트를 할 수 없다.
											if( m_bPartyQuest ) 
											{
												if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
												{
													list <int>::iterator iter_questnosavetype;
													for(iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin() ; iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
													{
														int questid = *iter_questnosavetype;
														_XQuestScriptItem* currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[questid];
														if( currentquest )
														{
															if( currentquest->GetPartyQuest() )
															{
																balreadyexistpartyquest = true;
																break;
															}
														}													
													}
												}
											}					
											if( !balreadyexistpartyquest )
											{											
												m_QuestStartMode = _XQUESTSTART_NPCTARGETTING ;
												m_bChangeQuestStartMode = TRUE;	
												
												map <int, _XTriggerConditionItem*>::iterator iter_setmark;
												_XTriggerConditionItem* currentcondition = NULL;
												for(iter_setmark = m_mapStartConditionList.begin() ; iter_setmark != m_mapStartConditionList.end() ; ++iter_setmark)
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
																	if( m_SuccessStartConditionid[i] == -1 )
																	{
																		m_SuccessStartConditionid[i] = currentcondition->GetConditionID();
																		
																		//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
																		//_XLog("Quest StartCondition Success : QuestID[%d] StartConditionID[%d], QuestStartConditionValue[%d] ", m_nQuestID, currentcondition->GetConditionID(),m_QuestStartConditionValue );
																		
																		break;
																	}
																}
															}								
															break;
														}						
													}
												}
												//풍운방이 있으면 풍운방을 보낸다.
												if( m_QuestEmailType != ID_QUEST_QUESTEMAIL_NONE )
												{
													// 퀘스트 진행 개수 체크한다 
													BOOL bSendQuestMail = TRUE;
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
													if(!m_bMainQuest)
													{
														if(g_QuestScriptManager.GetProgressQuestCount() >= ID_QUEST_SYNC-1)
														{
															bSendQuestMail = FALSE;
														}

													}
#endif

													if(bSendQuestMail)
													{
														_XQUESTMAIL_INFO questmailbuffer;
														memset(&questmailbuffer, 0, sizeof(_XQUESTMAIL_INFO));
														questmailbuffer.questid	= m_nQuestID;
														questmailbuffer.questmailtype = m_QuestEmailType;
														questmailbuffer.resendstarttime = g_LocalSystemTime;
														questmailbuffer.delaytime = m_nQuestMailDelayTime*1000;
														
														g_QuestScriptManager.m_listQuestMailBuffer.push_back(questmailbuffer);	
													}
													
												}//if( m_QuestEmailType != ID_QUEST_QUESTEMAIL_NONE )
											}
										}										
									}
									

								}
								
								
								switch(m_byStartConditionCompareClass)
								{
								case ID_TRIGGER_CONDITIONFUNCTION_OR :
									{
										//마크 세팅은 계산하지 않는다.
										switch(currentcondition->m_TDC.m_conditionclass) 
										{
										case ID_CONDITION_SETQUESTMARKAREA :	// QUEST마크(!) 표시하기 영역
										case ID_CONDITION_SETQUESTMARKNPC :		// QUEST마크(!) 표시하기 NPC 
										case ID_CONDITION_SETHINTMARKAREA :		// HINT마크(?) 표시하기 영역
										case ID_CONDITION_SETHINTMARKNPC :		// HINT마크(?) 표시하기 NPC 
											break;
										default:
											startconditionsuccesscount++;
											break;
										}
									}
									break;
								case ID_TRIGGER_CONDITIONFUNCTION_AND :
									{
										startconditionsuccesscount++;
									}
									break;
								}	
								
							}//if( currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
						}//if( !bskipcondition )
					}//if( !checksuccess )
				}//if(currentcondition)
			}//for(iter_condition = m_mapStartConditionList.begin() ; iter_condition != m_mapStartConditionList.end() ; iter_condition++)

			if( m_bChangeQuestStartMode )
			{
				if( m_QuestStartConditionValue != 0)
				{					
					//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );					
					if( !m_bMainQuest )//if( m_nQuestID < 10000 || m_nQuestID >=20000 )
					{
						switch( m_QuestStartMode ) {
						case _XQUESTSTART_OBJECTCLICK:
							{					
								g_QuestScriptManager.m_listQuestStartTypeObjectClick.push_back(m_nQuestID);
								//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_OBJECTCLICK ], QuestStartConditionValue[%d] ", m_nQuestID, m_QuestStartConditionValue );
							}
							break;
						case _XQUESTSTART_USINGITEM:
							{
								g_QuestScriptManager.m_listrQuestStartTypeUsingItem.push_back(m_nQuestID);
								//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_USINGITEM ], QuestStartConditionValue[%d] ", m_nQuestID, m_QuestStartConditionValue );
							}
							break;
						case _XQUESTSTART_WEARITEM:
							{
								g_QuestScriptManager.m_listQuestStartTypeWearItem.push_back(m_nQuestID);
								//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_WEARITEM ], QuestStartConditionValue[%d] ", m_nQuestID, m_QuestStartConditionValue );
							}
							break;
						case _XQUESTSTART_DESTROYITEM:
							{
								g_QuestScriptManager.m_listQuestStartTypeDestroyItem.push_back(m_nQuestID);
								//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_DESTROYITEM ], QuestStartConditionValue[%d] ", m_nQuestID, m_QuestStartConditionValue );
							}
							break;
						case _XQUESTSTART_LEVEL:
							{
								g_QuestScriptManager.m_listQuestStartTypeLevel.push_back(m_nQuestID);
								//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_LEVEL ], QuestStartConditionValue[%d] ", m_nQuestID, m_QuestStartConditionValue );
							}
							break;
						case _XQUESTSTART_ZONE:
							{
								g_QuestScriptManager.m_listQuestStartTypeZone.push_back(m_nQuestID);
								//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_ZONE ], QuestStartConditionValue[%d] ", m_nQuestID, m_QuestStartConditionValue );
							}
							break;
						case _XQUESTSTART_NPCTARGETTING:
							{
								g_QuestScriptManager.m_listQuestStartTypeNPCTargetting.push_back(m_nQuestID);
								//_XLog("SubQuest StartMode Setting : SubQuestID[%d] StartConditionMode [ _XQUESTSTART_NPCTARGETTING ], QuestStartConditionValue[%d] ", m_nQuestID, m_QuestStartConditionValue );
							}
							break;
						}
					}
				}
			}
			
			bool result = false;
			int startconditioncount = m_mapStartConditionList.size();
			switch(m_byStartConditionCompareClass)
			{
			case ID_TRIGGER_CONDITIONFUNCTION_OR :
				{					
					if( startconditionsuccesscount > 0 )
						result = true;
				}
				break;
			case ID_TRIGGER_CONDITIONFUNCTION_AND :
				{
					if( startconditioncount == startconditionsuccesscount )
						result = true;
				}
				break;
			}

			if(result)
			{				
				for( int i = 0; i < 20; ++i )
				{
					m_SuccessStartConditionid[i] = -1;
				}
			
				// 모두 만족
				if( m_bMainQuest )//if( m_nQuestID >= 10000 && m_nQuestID < 20000)
				{
					if(m_nQuestID == g_QuestScriptManager.GetCheckQuestID() && g_QuestScriptManager.GetCheckQuest())
					{
						m_bClientEnable = TRUE;
						g_NetworkKernel.SendPacket(MSG_NO_QUEST_SCON_REQUEST, m_nQuestID, 0, TRUE);
#ifdef _XDWDEBUG	
						_XLog("<< QUEST MESSAGE (CLIENT->SERVER): start questid [%d] >>", m_nQuestID );
#endif
						
						// server에 패킷 보냈음. NPC 머리 위에 ! 지워주기
						if( g_QuestScriptManager.m_listNPCTargettingQuest.size() <= 1 )
							ReSetQuestMark();
						
						if( m_startquestinformationmessage )
						{
							int strlength = strlen(m_startquestinformationmessage);
							
							if(strlength > 0)
							{
								m_lpszQuestProgressinformation = m_startquestinformationmessage;
							}					
						}

						
						return QUEST_STARTCONDITION_RESULT_NOTIFYSERVER;
					} 
					else
					{
						if( !m_bCheckFirstStartCondition )
						{
							m_bCheckFirstStartCondition=TRUE;
						}
						
						return QUEST_STARTCONDITION_RESULT_NOTIFYUSER;
					}
				}
				else
				{
					if(g_QuestScriptManager.GetProgressQuestCount() < ID_QUEST_SYNC)
					{
						m_bClientEnable = TRUE;
						g_NetworkKernel.SendPacket(MSG_NO_QUEST_SCON_REQUEST, m_nQuestID, 0, TRUE);
#ifdef _XDWDEBUG	
						_XLog("<< QUEST MESSAGE (CLIENT->SERVER): start questid [%d] >>", m_nQuestID );
#endif
						
						// server에 패킷 보냈음. NPC 머리 위에 ! 지워주기
						if( g_QuestScriptManager.m_listNPCTargettingQuest.size() <= 1 )
							ReSetQuestMark();
						
						if( m_startquestinformationmessage )
						{
							int strlength = strlen(m_startquestinformationmessage);
							
							if(strlength > 0)
							{
								m_lpszQuestProgressinformation = m_startquestinformationmessage;
							}					
						}
						
						return QUEST_STARTCONDITION_RESULT_NOTIFYSERVER;
					}
					else
					{
						// 5개 넘었다는 메세지 띄워주기
						int i = 0;
					}
				}
			}
			else
			{
				return QUEST_STARTCONDITION_RESULT_NOTIFYUSER;
			}

			// 아직 만족하지 않음
			return QUEST_STARTCONDITION_RESULT_FAIL;
		}
	}
	return QUEST_STARTCONDITION_RESULT_ALREADY;
}

BOOL _XQuestScriptItem::CheckFailConditionAtOptionCondition(void)
{	
	// 옵션조건식에서 시작된 실패 트리거 조건에 만족하지 않을 시 실패 
	if( !m_mapFailConditionList.empty() )
	{
		map <int, _XTriggerConditionItem*>::iterator iter_condition;
		iter_condition = m_mapFailConditionList.begin();
		while( iter_condition != m_mapFailConditionList.end() )
		{
			
			_XTriggerConditionItem* currentcondition = NULL;
			currentcondition = iter_condition->second;
			if(currentcondition)
			{
				if( currentcondition->m_bSuccessCancle )
				{
					if(currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
					{
						g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, m_nQuestID, m_CurrentTreeNodeNumber);
						int conditionid = currentcondition->GetConditionID();
						m_mapFailConditionList.erase(iter_condition);
						return FALSE;
					}
					else
					{
						iter_condition++;
					}
				}
				else
				{
					if(currentcondition->Process() == QUEST_CONDITION_RESULT_FAIL)
					{
						g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, m_nQuestID, m_CurrentTreeNodeNumber);
						int conditionid = currentcondition->GetConditionID();
						m_mapFailConditionList.erase(iter_condition);
						return FALSE;
					}
					else
					{
						iter_condition++;
					}
				}
			}				
		}
	}
	return TRUE;
}

_XQUESTITEM_RESULT _XQuestScriptItem::Process(void)
{
	if(m_bWaitingFailResult)
		return QUEST_QUEST_RESULT_PROCESS;

	if(!g_QuestScriptManager.m_bReceiveQuestData)
		return QUEST_QUEST_RESULT_PROCESS;

	// Reword quest 처리
	if(!ProcessRewordQuest())
		return QUEST_QUEST_RESULT_PROCESS;

	if(m_bServerEnable)
	{
		// 옵션조건식에서 시작된 실패 트리거 조건에 만족하지 않을 시 실패 		
		if( (g_LocalSystemTime - m_FailConditionTimer) > 3000)
		{
			if(!CheckFailConditionAtOptionCondition())
				return QUEST_QUEST_RESULT_PROCESS;
			m_FailConditionTimer = g_LocalSystemTime;
		}
		
		map <int, _XTriggerTreeItem*>::iterator iter_triggertree = m_mapTriggerTreeList.find(m_CurrentTreeNodeNumber);
		if(iter_triggertree != m_mapTriggerTreeList.end())
		{
			_XTriggerTreeItem* currenttree = iter_triggertree->second;
			if(currenttree)
			{
				map <int, _XTriggerItem*>::iterator iter_successtrigger = m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
				_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
				if(currentsuccesstrigger)
				{
					switch(currentsuccesstrigger->Process())
					{
					case QUEST_TRIGGER_RESULT_WAITSERVER :
					case QUEST_TRIGGER_RESULT_ERROR :
					case QUEST_TRIGGER_RESULT_PROCESS :
						{
							if( m_bPartyQuest )
							{
								// 홀드 상태의 퀘스트라도 파티 퀘스트에서 퀘스트 완료이면 강제로 퀘스트를 성공시킨다.
								// 서버에서 완료라는 결과가 오면 퀘스트 성공으로 세팅한다. 
								if( m_QuestServerResult == QUEST_COMPLETE_RESULT_SOLVE || m_QuestServerResult == QUEST_COMPLETE_RESULT_MISS )
								{
									m_bConfirmAll = TRUE;
									m_QuestCompleteResult = m_QuestServerResult;
									m_bComplete = TRUE;
									
									return QUEST_QUEST_RESULT_SUCCESS;
								}
							}
							return QUEST_QUEST_RESULT_PROCESS;
						}
					case QUEST_TRIGGER_RESULT_SUCCESS : // 성공일 때 오른쪽 트리 노드 번호로 세팅
					case QUEST_TRIGGER_RESULT_FAIL : // 실패일 때 왼족 트리 노드 번호로 세팅
						{
							// 서버에서 완료라는 결과가 오면 퀘스트 성공으로 세팅한다. 
							if( m_QuestServerResult == QUEST_COMPLETE_RESULT_SOLVE || m_QuestServerResult == QUEST_COMPLETE_RESULT_MISS )
							{
								m_bConfirmAll = TRUE;
								m_QuestCompleteResult = m_QuestServerResult;
								m_bComplete = TRUE;
								
								return QUEST_QUEST_RESULT_SUCCESS;
							}
							else
							{
								currentsuccesstrigger->SetClientEnable(TRUE);
#ifdef _XDWDEBUG	
								_XLog("Quest Trigger Condition Check Close : TriggerID[%d]", currenttree->m_TriggerUniqueID);
#endif
								currentsuccesstrigger->SetServerEnable(FALSE);

								map <int, _XTriggerTreeItem*>::iterator iter_triggertree = m_mapTriggerTreeList.find(m_NextTreeNodeNumber);
								if(iter_triggertree != m_mapTriggerTreeList.end())
								{
									_XTriggerTreeItem* nexttree = iter_triggertree->second;
									if(nexttree)
									{
										map <int, _XTriggerItem*>::iterator iter_nexttrigger = m_mapSuccessTriggerList.find(nexttree->m_TriggerUniqueID);
										if(iter_nexttrigger != m_mapSuccessTriggerList.end())
										{
											_XTriggerItem* nexttrigger = NULL;
											nexttrigger = iter_nexttrigger->second;
											m_CurrentTreeNodeNumber = m_NextTreeNodeNumber;
											m_nCurrentTriggerID = nexttree->m_TriggerUniqueID;
											nexttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
											_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", nexttree->m_TriggerUniqueID);
#endif

											return QUEST_QUEST_RESULT_PROCESS;
										}
									}
								}
							}
						}
					case QUEST_TRIGGER_RESULT_BLOCK :
						{
						}
						break;
					}
				}
			}
		}
		return QUEST_QUEST_RESULT_PROCESS;
	}
	return QUEST_QUEST_RESULT_PROCESS;
}

BOOL _XQuestScriptItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	map <int, _XTriggerConditionItem*>::iterator iter_condition;
	for(iter_condition = m_mapStartConditionList.begin() ; iter_condition != m_mapStartConditionList.end() ; ++iter_condition)
	{
		_XTriggerConditionItem* currentcondition = iter_condition->second;
		if(currentcondition)
			delete currentcondition;
	}
	m_mapStartConditionList.clear();
	
	// 옵션 조건식 초기화
	map <int, _XTriggerConditionItem*>::iterator iter_optioncondition;
	for(iter_optioncondition = m_mapOptionConditionList.begin() ; iter_optioncondition != m_mapOptionConditionList.end() ; ++iter_optioncondition)
	{
		_XTriggerConditionItem* currentcondition = iter_optioncondition->second;
		if(currentcondition)
			delete currentcondition;
	}
	m_mapOptionConditionList.clear();
	
	map <int, _XTriggerItem*>::iterator iter_successtrigger;
	for(iter_successtrigger = m_mapSuccessTriggerList.begin() ; iter_successtrigger != m_mapSuccessTriggerList.end() ; ++iter_successtrigger)
	{
		_XTriggerItem* currenttrigger = iter_successtrigger->second;
		if(currenttrigger)
			delete currenttrigger;
	}
	m_mapSuccessTriggerList.clear();
	
	map <int, _XTriggerTreeItem*>::iterator iter_triggertree;
	for(iter_triggertree = m_mapTriggerTreeList.begin() ; iter_triggertree != m_mapTriggerTreeList.end() ; ++iter_triggertree)
	{
		_XTriggerTreeItem* currenttree = iter_triggertree->second;
		if(currenttree)
			delete currenttree;
	}
	m_mapTriggerTreeList.clear();


	int triggertreecount = 0;
	if(fread(&triggertreecount, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(triggertreecount > 0)
	{
		_XTriggerTreeItem* currenttree = NULL;
		for(int i = 0 ; i < triggertreecount ; ++i)
		{
			currenttree = new _XTriggerTreeItem;
			if(currenttree)
			{
				if(!currenttree->Load(fileptr))
				{
					delete currenttree;
					return FALSE;
				}
				m_mapTriggerTreeList[currenttree->m_SelfNodeUniqueID] = currenttree;
			}
			else
				return FALSE;
		}
	}


	if(fread(&m_bEnable, sizeof(BOOL), 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_bAutorun, sizeof(BOOL), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_RankQuestType1, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_RankQuestType2, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_bTempQuest, sizeof(bool), 1, fileptr) < 1)
		return FALSE;
	BYTE temp = 0;
	if(fread(&temp, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_nQuestID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
//	if( m_nQuestID == 15000)
//		int test = 1;

	if(fread(&m_QuestChapter, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	int index = 0;
	if(fread(&index, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_QuestEmailType, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	
	if( m_RankQuestType1 == 1 )
	{
		m_bPartyQuest = TRUE;
	}
	else
	{
		m_bPartyQuest = FALSE;
	}
	
	if( m_nQuestID >= 10000 && m_nQuestID < 20000 )
	{
		m_bMainQuest = TRUE;
		m_QuestType = _XQUEST_TYPE_1;
	}
	else 
	{
		m_bMainQuest = FALSE;
		if( m_nQuestID >= 20000 && m_nQuestID < 30000 )
		{
			m_QuestType = _XQUEST_TYPE_2;
		}
		else if( m_nQuestID >= 30000 && m_nQuestID < 40000 )
		{
			m_QuestType = _XQUEST_TYPE_3;
		}
		else if( m_nQuestID >= 40000 && m_nQuestID < 50000 )
		{
			m_QuestType = _XQUEST_TYPE_4;
		}
		else if( m_nQuestID >= 0 && m_nQuestID < 10000 )
		{
			m_QuestType = _XQUEST_TYPE_5;
		}
		else if( m_nQuestID >= 50000 && m_nQuestID < 60000 )
		{
			m_QuestType = _XQUEST_TYPE_5;
		}
	}

	int readstringlength = 0;
		
	if( (readstringlength = LoadString(fileptr, m_lpszQuestName)) == -1)
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszQuestName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );			
		//_XLog( "QSI\tQuest name\t%d\t...\t%d\t\t%s", m_nQuestID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszQuestName  );
	}
#endif
	
	if( (readstringlength = LoadString(fileptr, m_lpszQuestSubName)) == -1)
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszQuestSubName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest sub name\t%d\t...\t%d\t\t%s", m_nQuestID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszQuestSubName  );
	}
#endif

	
	if(fread(&m_nQuestMailDelayTime, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_LimitQuestCountStartType, sizeof(int), 1, fileptr) < 1)//부여선착순 갯수
		return FALSE;
	
	if(fread(&m_LimitQuestCountCompleteType, sizeof(int), 1, fileptr) < 1)//완료선착순 갯수
		return FALSE;
	
	if( (readstringlength = LoadString(fileptr, m_lpszStartQuestMessage)) == -1)
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszStartQuestMessage = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest starting message\t%d\t...\t%d\t\t%s", m_nQuestID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszStartQuestMessage );
	}
#endif

	if( (readstringlength = LoadString(fileptr, m_lpszFailQuestMessage)) == -1)
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszFailQuestMessage = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest failed message\t%d\t...\t%d\t\t%s", m_nQuestID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszFailQuestMessage );
	}
#endif

	if( (readstringlength = LoadString(fileptr, m_lpszOutlineMessage)) == -1)
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{	
		m_lpszOutlineMessage = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest outline message\t%d\t...\t%d\t\t%s", m_nQuestID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszOutlineMessage );
	}
#endif

	if( (readstringlength = LoadString(fileptr, m_lpszOutlineMessage2)) == -1)
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszOutlineMessage2 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest second outline message\t%d\t...\t%d\t\t%s", m_nQuestID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszOutlineMessage2 );
	}
#endif

	if( m_nQuestID == 11000 )
	{
		int fjkdsjf = 0;
	}

	// 시작 퀘스트 진행 정보
	if( (readstringlength = LoadString(fileptr, m_startquestinformationmessage)) == -1)
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_startquestinformationmessage = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest information message\t%d\t...\t%d\t\t%s", m_nQuestID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_startquestinformationmessage );
	}
#endif
	
	if(fread(&m_byQuestCancelCase, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_byStartConditionCompareClass, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_byQuestOptionCompareClass, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	

	int startconditioncount = 0;
	if(fread(&startconditioncount, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(startconditioncount > 0)
	{
		_XTriggerConditionItem* currentcondition = NULL;
		for(int i = 0 ; i < startconditioncount ; ++i)
		{
			currentcondition = new _XTriggerConditionItem;
			if(currentcondition)
			{
				currentcondition->SetOwnerQuestID(m_nQuestID);
				currentcondition->SetConditionID(i + 1);
				currentcondition->SetOwnerTriggerClass(ID_TRIGGER_CLASS_STARTCONDITION);
				currentcondition->SetQuestType(m_QuestType);
				currentcondition->SetMainQuest(m_bMainQuest);
				if(!currentcondition->Load(fileptr))
				{
					delete currentcondition;
					return FALSE;
				}
				m_mapStartConditionList[currentcondition->GetConditionID()] = currentcondition;
			}
			else
				return FALSE;
		}
	}
	
	
	int optionconditioncount = 0;
	if(fread(&optionconditioncount, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(optionconditioncount > 0)
	{
		_XTriggerConditionItem* currentcondition = NULL;
		for(int i = 0 ; i < optionconditioncount ; ++i)
		{
			currentcondition = new _XTriggerConditionItem;
			if(currentcondition)
			{
				currentcondition->SetOwnerQuestID(m_nQuestID);
				currentcondition->SetConditionID(i + 1);
				currentcondition->SetOwnerTriggerClass(ID_TRIGGER_CLASS_STARTCONDITION);
				currentcondition->SetQuestType(m_QuestType);
				currentcondition->SetMainQuest(m_bMainQuest);
				if(!currentcondition->Load(fileptr))
				{
					delete currentcondition;
					return FALSE;
				}
				m_mapOptionConditionList[currentcondition->GetConditionID()] = currentcondition;
			}
			else
				return FALSE;
		}
	}
	
	int triggercount = 0;
	if(fread(&triggercount, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(triggercount > 0)
	{
		_XTriggerItem* currenttrigger = NULL;
		for(int i = 0 ; i < triggercount ; ++i)
		{
			currenttrigger = new _XTriggerItem;
			if(currenttrigger)
			{
				currenttrigger->SetOwnerQuestID(m_nQuestID);
				currenttrigger->SetTriggerID(i + 1);
				currenttrigger->SetQuestType(m_QuestType);
				currenttrigger->SetMainQuest(m_bMainQuest);
				if(!currenttrigger->Load(fileptr))
				{
					delete currenttrigger;
					return FALSE;
				}
				m_mapSuccessTriggerList[currenttrigger->GetTriggerID()] = currenttrigger;
			}
			else
				return FALSE;
		}
	}
		
	int tempvlaue;
	if(fread(&tempvlaue, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&tempvlaue, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(&tempvlaue, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_queststartprobability, sizeof(float), 1, fileptr) < 1)
		return FALSE;
	
#ifdef _XDEF_QUESTSCRIPT_11
	if(fread(&m_nStartYear, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_nStartMonth, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_nStartDay, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_nStartHour, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_nEndYear, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_nEndMonth, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_nEndDay, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_nEndHour, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_unCountryCode, sizeof(unsigned int), 1, fileptr) < 1)
		return FALSE;
#endif
	
	return TRUE;
}

int	_XQuestScriptItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;

	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return -1;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	fseek( fileptr, sizeof(TCHAR)*strlength, SEEK_CUR );
	string = NULL;
#else
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
#endif

	return strlength;
}

void _XQuestScriptItem::SetQuestServerResult(_XQUEST_COMPLETE_RESULT result)
{
	m_QuestServerResult = result;
}

_XQUEST_COMPLETE_RESULT _XQuestScriptItem::GetQuestServerResult(void)
{
	return m_QuestServerResult;
}

void _XQuestScriptItem::SetQuestCompleteResult(_XQUEST_COMPLETE_RESULT result)
{
	m_QuestCompleteResult = result;
}

_XQUEST_COMPLETE_RESULT _XQuestScriptItem::GetQuestCompleteResult(void)
{
	return m_QuestCompleteResult;
}

void _XQuestScriptItem::Reset(void)
{
	m_bClientEnable = FALSE;
	m_bServerEnable = FALSE;
	m_bConfirmAll = FALSE;

	m_bComplete = FALSE;
	m_bInsertCompleteList = FALSE;
	
	m_bWaitingFailResult = FALSE;
	m_bCheckFirstStartCondition = FALSE;
	
	// 퀘스트 진행 정보 리셋 
	m_lpszQuestProgressinformation = NULL;
	
	m_mapFailConditionList.clear();

	map <int, _XTriggerItem*>::iterator iter_successtrigger;
	for(iter_successtrigger = m_mapSuccessTriggerList.begin() ; iter_successtrigger != m_mapSuccessTriggerList.end() ; ++iter_successtrigger)
	{
		_XTriggerItem* currenttirgger = NULL;
		currenttirgger = iter_successtrigger->second;
		if(currenttirgger)
		{
			currenttirgger->Reset();
		}
	}

	if(!g_QuestScriptManager.m_listQuestMark.empty())
	{
		bool isquestmark = false;
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
			if( questmark.questid == m_nQuestID )
			{
				isquestmark = true;
				break;
			}
		}
		if( isquestmark )
		{
			ReSetQuestMark();
		}
	}
	
	for( int i = 0; i < 20; ++i )
	{
		m_SuccessStartConditionid[i] = -1;
	}
	
	ResetCounterInformation();
	ResetPartyCounterInformation();
	ResetItemCounterInformation();

	m_QuestStartConditionValue = 0;
	
	m_nStartTime	= 0;
	m_nNextTriggerID		= 0;
	m_nCurrentTriggerID		= 0;	
	
	m_NextTreeNodeNumber		= 0;
	m_CurrentTreeNodeNumber		= 0;

	m_nLastTargettingNPCID	= 0;
	m_bCheckFirstStartCondition = FALSE;	
	m_QuestScriptItemIndexNumber = -1;
	m_bSelectedQuestScriptItem = FALSE;	
	m_SelectionNumber = -1;
	m_bSelectionNumberCheck = FALSE;	
	
	m_bSendClientPacketAtPartyQuest = FALSE;
}

BOOL _XQuestScriptItem::GetTriggerComplete(int triggerid)
{
	map <int, _XTriggerItem*>::iterator iter_trigger;
	_XTriggerItem* currenttrigger = NULL;

	iter_trigger = m_mapSuccessTriggerList.find(triggerid);
	if(iter_trigger != m_mapSuccessTriggerList.end())
	{
		currenttrigger = iter_trigger->second;
		if(currenttrigger)
		{
			return currenttrigger->GetComplete();
		}
	}

	return FALSE;
}

//void _XQuestScriptItem::ProcessTest(int triggerid, int actionid)
//{
//	map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(triggerid);
//	if(iter_trigger != m_mapSuccessTriggerList.end())
//	{
//		_XTriggerItem* currenttrigger = iter_trigger->second;
//		if(currenttrigger)
//		{
//			currenttrigger->ProcessTest(actionid);
//		}
//	}


//void _XQuestScriptItem::RestoreTest(int triggerid, int actionid)
//{
//	map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(triggerid);
//	if(iter_trigger != m_mapSuccessTriggerList.end())
//	{
//		_XTriggerItem* currenttrigger = iter_trigger->second;
//		if(currenttrigger)
//		{
//			currenttrigger->RestoreTest(actionid);
//		}
//	}


BOOL _XQuestScriptItem::GetMainQuest(void)
{
	return m_bMainQuest;
}

LPTSTR _XQuestScriptItem::GetQuestName(void)
{
	if(m_lpszQuestName)
		return m_lpszQuestName;

	return _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA);	//_T("이름없음")

}
LPTSTR _XQuestScriptItem::GetQuestSubName(void)
{
	if(m_lpszQuestSubName)
		return m_lpszQuestSubName;

	return _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA);	//_T("이름없음")
}

BOOL _XQuestScriptItem::ProcessRewordQuest(void)
{
	bool rewardquestsuccess = false;
	map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(m_nQuestID);
	if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
	{
		ReSetQuestMark();

		BOOL	invenfull = TRUE;
		int		needinvencount = 0;
		int		emptyslotcount = 0;
		
		_XQUEST_INVENFULL invenfullstruct = iter_rewordquest->second;
		needinvencount = invenfullstruct.needinven;
		
		for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
		{
			if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
			{
				emptyslotcount++;
				if(emptyslotcount == needinvencount)
				{
					invenfull = FALSE;
					break;
				}
			}
		}

	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
		if( invenfull )
		{
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
				{
					emptyslotcount++;
					if(emptyslotcount == needinvencount)
					{
						invenfull = FALSE;
						break;
					}
				}
			}
		}
	#endif
#ifdef _XTESTSERVER
		/*
//		if(!invenfull)
//		{
//			int triggerid = invenfullstruct.triggerid;
//			if(triggerid > 0)
//			{
//				map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(triggerid);
//				if(iter_trigger != m_mapSuccessTriggerList.end())
//				{
//					_XTriggerItem* currenttrigger = iter_trigger->second;
//					if(currenttrigger)
//					{
//						if(!currenttrigger->m_bWaitRewordPacket)
//						{
//							currenttrigger->SetClientEnable(FALSE);
//							return TRUE;
//						}
//					}
//				}
//			}
//		}
		
		if(!invenfull)
		{
			int triggerid = invenfullstruct.triggerid;
			if(triggerid > 0)
			{
				map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(triggerid);
				if(iter_trigger != m_mapSuccessTriggerList.end())
				{
					_XTriggerItem* currenttrigger = iter_trigger->second;
					if(currenttrigger)
					{
						// trigger의 condition을 모두 체크
						BOOL	bNPCClick = FALSE;
						if(currenttrigger->m_mapConditionList.empty())
							bNPCClick = FALSE;
						else
						{
							map <int, _XTriggerConditionItem*>::iterator iter_condition;
							_XTriggerConditionItem* currentcondition;
							for(iter_condition = currenttrigger->m_mapConditionList.begin() ; iter_condition != currenttrigger->m_mapConditionList.end() ; ++iter_condition)
							{
								currentcondition = iter_condition->second;
								if(currentcondition)
								{
									switch(currentcondition->m_TDC.m_conditionclass)
									{
									case ID_CONDITION_CHECKTARGETTING :		// npc click으로 처리 
										{
											if( currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC )
												bNPCClick = TRUE;
											else
												bNPCClick = FALSE;	
										}
										break;
									default :
										bNPCClick = FALSE;				
										break;
									}
								}
							}
						}

						// 전투를 하거나 owner npc를 클릭하면
						if(!bNPCClick)
						{
							if(!currenttrigger->m_bWaitRewordPacket)
							{
								currenttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
								_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", triggerid);
#endif
								rewardquestsuccess = true;
							}
						}
						else
						{
							if( !m_bRewardProcessNPCMarkCheck )
							{
								// npc id 구하기
								int npcid = this->m_nLastTargettingNPCID;

								if(npcid == 0)
								{
									npcid = GetOwnerNPCID(triggerid);
								}

								for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
								{
									if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid)
									{
										
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2750), //_T("행낭에 공간이 생겨서 풍운록을 진행할 수 있습니다.")
											_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL,TRUE,D3DCOLOR_ARGB(255, 255, 212, 0));
										
										switch(m_QuestType) {
										case _XQUEST_TYPE_1:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_YELLOW;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_2:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_BLUE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_3:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_ORANGE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_4:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_GREEN;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_5:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_WHITE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										}
										break;
									}
								}
							}
							else
							{
								if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
								{
									int npcid = this->m_nLastTargettingNPCID;
									
									if(npcid == 0)
									{
										npcid = GetOwnerNPCID(triggerid);
									}
									_XNPCObject* pNPC = (_XNPCObject*)g_pLocalUser->m_AttackTarget;
									if(pNPC)
									{
										if(pNPC->m_UniqueID == npcid)
										{
											if(g_pLocalUser->m_CharacterLog.isTalk)
											{
												if(g_pLocalUser->m_CharacterLog.talkTime > invenfullstruct.starttime && invenfullstruct.starttime != 0)
												{
													if(!currenttrigger->m_bWaitRewordPacket)
													{
														for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
														{
															if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid)
															{
																g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;
																break;
															}
														}
														m_bRewardProcessNPCMarkCheck = FALSE;
														currenttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
														_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", triggerid);
#endif
														rewardquestsuccess = true;
													}
												}
											} // if(g_pLocalUser->m_CharacterLog.isTalk)
										}
									} // if(pNPC)
								} // if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
							}

						} //else
					}
				} // if(iter_trigger != m_mapSuccessTriggerList.end())
			}
		} // if(!invenfull)		

		*/ //old code

		if(!invenfull)
		{
			int triggerid = invenfullstruct.triggerid;
			if(triggerid > 0)
			{
				map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(triggerid);
				if(iter_trigger != m_mapSuccessTriggerList.end())
				{
					_XTriggerItem* currenttrigger = iter_trigger->second;
					if(currenttrigger)
					{
						// trigger의 condition을 모두 체크
						BOOL	bNPCClick = FALSE;
						if(currenttrigger->m_mapConditionList.empty())
							bNPCClick = TRUE;
						else
						{
							map <int, _XTriggerConditionItem*>::iterator iter_condition;
							_XTriggerConditionItem* currentcondition;
							for(iter_condition = currenttrigger->m_mapConditionList.begin() ; iter_condition != currenttrigger->m_mapConditionList.end() ; ++iter_condition)
							{
								currentcondition = iter_condition->second;
								if(currentcondition)
								{
									switch(currentcondition->m_TDC.m_conditionclass)
									{
									case ID_CONDITION_CHECKATTACK :		// 전투하면 아이템을 주는 경우
										bNPCClick = FALSE;
										break;
									default :
										bNPCClick = TRUE;				// 나머지는 모두 npc click으로 처리
										break;
									}
								}
							}
						}

						// 전투를 하거나 ower npc를 클릭하면
						if(!bNPCClick)
						{
							if(!currenttrigger->m_bWaitRewordPacket)
							{
								currenttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
								_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", triggerid);
#endif
								rewardquestsuccess = true;
							}
						}
						else
						{
							// npc id 구하기
							int npcid = this->m_nLastTargettingNPCID;
							
							if(npcid == 0)
							{
								npcid = GetOwnerNPCID(triggerid);
							}

							if( !m_bRewardProcessNPCMarkCheck )
							{								
								for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
								{
									if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid)
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2750), //_T("행낭에 공간이 생겨서 풍운록을 진행할 수 있습니다.")
											_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL,TRUE,D3DCOLOR_ARGB(255, 255, 212, 0));
										
										switch(m_QuestType) {
										case _XQUEST_TYPE_1:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_YELLOW;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_2:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_BLUE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_3:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_ORANGE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_4:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_GREEN;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_5:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_WHITE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										}
									}
								}
							}

							if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
							{
								_XNPCObject* pNPC = (_XNPCObject*)g_pLocalUser->m_AttackTarget;
								if(pNPC)
								{
									if(pNPC->m_UniqueID == npcid)
									{
										if(g_pLocalUser->m_CharacterLog.isTalk)
										{
											if(g_pLocalUser->m_CharacterLog.talkTime > invenfullstruct.starttime && invenfullstruct.starttime != 0)
											{
												if(!currenttrigger->m_bWaitRewordPacket)
												{
													m_bRewardProcessNPCMarkCheck = FALSE;
													currenttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
													_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", triggerid);
#endif
													rewardquestsuccess = true;
												}
											}
										} // if(g_pLocalUser->m_CharacterLog.isTalk)
									}
								} // if(pNPC)
							} // if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
						} //else
					}
				} // if(iter_trigger != m_mapSuccessTriggerList.end())
			}
		} // if(!invenfull)	
		else // 인벤토리가 비었다가 다시 공간이 없을 때는 Quest List를 보여주는 것이 아니고 NPC대사를 처리한다.
		{
			if( m_bRewardProcessNPCMarkCheck )
			{
				
				int triggerid = invenfullstruct.triggerid;
				// npc id 구하기
				int npcid = this->m_nLastTargettingNPCID;
				
				if(npcid == 0)
				{
					npcid = GetOwnerNPCID(triggerid);
				}
				
				for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
				{
					if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid)
					{						
						g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;
						m_bRewardProcessNPCMarkCheck = FALSE;
					}
				}
			}//if( m_bRewardProcessNPCMarkCheck )
		}
#else
		if(!invenfull)
		{
			int triggerid = invenfullstruct.triggerid;
			if(triggerid > 0)
			{
				map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(triggerid);
				if(iter_trigger != m_mapSuccessTriggerList.end())
				{
					_XTriggerItem* currenttrigger = iter_trigger->second;
					if(currenttrigger)
					{
						// trigger의 condition을 모두 체크
						BOOL	bNPCClick = FALSE;
						if(currenttrigger->m_mapConditionList.empty())
							bNPCClick = TRUE;
						else
						{
							map <int, _XTriggerConditionItem*>::iterator iter_condition;
							_XTriggerConditionItem* currentcondition;
							for(iter_condition = currenttrigger->m_mapConditionList.begin() ; iter_condition != currenttrigger->m_mapConditionList.end() ; ++iter_condition)
							{
								currentcondition = iter_condition->second;
								if(currentcondition)
								{
									switch(currentcondition->m_TDC.m_conditionclass)
									{
									case ID_CONDITION_CHECKATTACK :		// 전투하면 아이템을 주는 경우
										bNPCClick = FALSE;
										break;
									default :
										bNPCClick = TRUE;				// 나머지는 모두 npc click으로 처리
										break;
									}
								}
							}
						}

						// 전투를 하거나 ower npc를 클릭하면
						if(!bNPCClick)
						{
							if(!currenttrigger->m_bWaitRewordPacket)
							{
								currenttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
								_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", triggerid);
#endif
								rewardquestsuccess = true;
							}
						}
						else
						{
							// npc id 구하기
							int npcid = this->m_nLastTargettingNPCID;
							
							if(npcid == 0)
							{
								npcid = GetOwnerNPCID(triggerid);
							}

							if( !m_bRewardProcessNPCMarkCheck )
							{								
								for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
								{
									if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid)
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2750), //_T("행낭에 공간이 생겨서 풍운록을 진행할 수 있습니다.")
											_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL,TRUE,D3DCOLOR_ARGB(255, 255, 212, 0));
										
										switch(m_QuestType) {
										case _XQUEST_TYPE_1:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_YELLOW;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_2:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_BLUE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_3:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_ORANGE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_4:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_GREEN;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										case _XQUEST_TYPE_5:
											g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_WHITE;
											m_bRewardProcessNPCMarkCheck = TRUE;
											break;
										}
									}
								}
							}

							if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
							{
								_XNPCObject* pNPC = (_XNPCObject*)g_pLocalUser->m_AttackTarget;
								if(pNPC)
								{
									if(pNPC->m_UniqueID == npcid)
									{
										if(g_pLocalUser->m_CharacterLog.isTalk)
										{
											if(g_pLocalUser->m_CharacterLog.talkTime > invenfullstruct.starttime && invenfullstruct.starttime != 0)
											{
												if(!currenttrigger->m_bWaitRewordPacket)
												{
													m_bRewardProcessNPCMarkCheck = FALSE;
													currenttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG	
													_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", triggerid);
#endif
													rewardquestsuccess = true;
												}
											}
										} // if(g_pLocalUser->m_CharacterLog.isTalk)
									}
								} // if(pNPC)
							} // if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
						} //else
					}
				} // if(iter_trigger != m_mapSuccessTriggerList.end())
			}
		} // if(!invenfull)	
		else // 인벤토리가 비었다가 다시 공간이 없을 때는 Quest List를 보여주는 것이 아니고 NPC대사를 처리한다.
		{
			if( m_bRewardProcessNPCMarkCheck )
			{
				
				int triggerid = invenfullstruct.triggerid;
				// npc id 구하기
				int npcid = this->m_nLastTargettingNPCID;
				
				if(npcid == 0)
				{
					npcid = GetOwnerNPCID(triggerid);
				}
				
				for(int i = 0; i < g_Npclist.m_NPCObjectCount; ++i)
				{
					if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid)
					{						
						g_Npclist.m_pNPCObjectArray[i].m_HaveQuestType = _XNPC_HAVEQUEST_NONE;
						m_bRewardProcessNPCMarkCheck = FALSE;
					}
				}
			}//if( m_bRewardProcessNPCMarkCheck )
		}
#endif

	}
	else
		return TRUE;

	if( rewardquestsuccess )
	{
		// Reword list에 있으면 삭제
		map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(m_nQuestID);
		if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
		{
			g_QuestScriptManager.DeleteRewordQuestList(m_nQuestID);
		}
		return TRUE;
	}

	return FALSE;
}

void _XQuestScriptItem::ReleasePrivateData(void)
{
	m_bClientEnable			= FALSE;
	m_bServerEnable			= FALSE;
	m_bConfirmAll			= FALSE;

	m_bComplete				= FALSE;
	m_bInsertCompleteList	= FALSE;

	m_nStartTime			= 0;
	m_nNextTriggerID		= 0;
	m_nCurrentTriggerID		= 0;

	m_bWaitingFailResult	= FALSE;

	m_QuestServerResult		= QUEST_COMPLETE_RESULT_NONE;
	m_QuestCompleteResult	= QUEST_COMPLETE_RESULT_NONE;

	m_nLastTargettingNPCID	= 0;

	m_mapTriggerTimer.clear();

	map <int, _XTriggerItem*>::iterator iter_successtrigger;
	for(iter_successtrigger = m_mapSuccessTriggerList.begin() ; iter_successtrigger != m_mapSuccessTriggerList.end() ; ++iter_successtrigger)
	{
		_XTriggerItem* currenttrigger = iter_successtrigger->second;
		if(currenttrigger)
			currenttrigger->ReleasePrivateData();
	}
}

int _XQuestScriptItem::GetOwnerNPCID(int triggerid)
{
	int npcid = 0;
	int prevtriggerid = 0;

	map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(triggerid);
	_XTriggerItem* currenttrigger = iter_trigger->second;

	if(currenttrigger)
	{
		map <int, _XTriggerConditionItem*>::iterator iter_condition;
		for(iter_condition = currenttrigger->m_mapConditionList.begin() ; iter_condition != currenttrigger->m_mapConditionList.end() ; ++iter_condition)
		{
			_XTriggerConditionItem* currentcondition = iter_condition->second;
			if(currentcondition)
			{
				if(currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING)
				{
					if(currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC)
					{
						npcid = currentcondition->m_TDC.m_numericvalue1;
						break;
					}
				}
			}
		}
	}

	if(npcid == 0)
	{
		prevtriggerid = triggerid - 1;
		while(npcid == 0 && prevtriggerid > 0)
		{
			map <int, _XTriggerItem*>::iterator iter_trigger = m_mapSuccessTriggerList.find(prevtriggerid);
			_XTriggerItem* currenttrigger = iter_trigger->second;
			
			if(currenttrigger)
			{
				map <int, _XTriggerConditionItem*>::iterator iter_condition;
				for(iter_condition = currenttrigger->m_mapConditionList.begin() ; iter_condition != currenttrigger->m_mapConditionList.end() ; ++iter_condition)
				{
					_XTriggerConditionItem* currentcondition = iter_condition->second;
					if(currentcondition)
					{
						if(currentcondition->m_TDC.m_conditionclass == ID_CONDITION_CHECKTARGETTING)
						{
							if(currentcondition->m_TDC.m_compareclass1 == ID_QUEST_TARGETTING_STATICNPC)
							{
								npcid = currentcondition->m_TDC.m_numericvalue1;
								break;
							}
						}
					}
				} // for
			} // if(currenttrigger)

			prevtriggerid--;
		} // while(npcid == 0 || prevtriggerid >= 0)
	}


	return npcid;
}

void _XQuestScriptItem::SetFirstStartCondition(BOOL checkfirststartcondition)
{
	m_bCheckFirstStartCondition = checkfirststartcondition;
}

BOOL _XQuestScriptItem::GetFirstStartCondition(void)
{
	return m_bCheckFirstStartCondition;
}

void _XQuestScriptItem::RemoveQuestStartConditionList(void)
{
	switch(m_QuestStartMode) {
	case _XQUESTSTART_NPCTARGETTING:
		{
			list <int>::iterator iter_npctargetting;
			for(iter_npctargetting = g_QuestScriptManager.m_listQuestStartTypeNPCTargetting.begin() ; iter_npctargetting != g_QuestScriptManager.m_listQuestStartTypeNPCTargetting.end() ; ++iter_npctargetting)
			{				
				int questid = *iter_npctargetting;
				if( m_nQuestID == questid )
				{
					iter_npctargetting = g_QuestScriptManager.m_listQuestStartTypeNPCTargetting.erase(iter_npctargetting);
					break;
				}
			}
		}
		break;
	case _XQUESTSTART_OBJECTCLICK:
		{
			list <int>::iterator iter_objectclick;
			for(iter_objectclick = g_QuestScriptManager.m_listQuestStartTypeObjectClick.begin() ; iter_objectclick != g_QuestScriptManager.m_listQuestStartTypeObjectClick.end() ; ++iter_objectclick)
			{
				int questid = *iter_objectclick;
				if( m_nQuestID == questid )
				{
					iter_objectclick = g_QuestScriptManager.m_listQuestStartTypeObjectClick.erase(iter_objectclick);
					break;
				}
			}
		}
		break;
	case _XQUESTSTART_USINGITEM:
		{
			list <int>::iterator iter_usingitem;
			for(iter_usingitem = g_QuestScriptManager.m_listrQuestStartTypeUsingItem.begin() ; iter_usingitem != g_QuestScriptManager.m_listrQuestStartTypeUsingItem.end() ; ++iter_usingitem)
			{
				int questid = *iter_usingitem;
				if( m_nQuestID == questid )
				{
					iter_usingitem = g_QuestScriptManager.m_listrQuestStartTypeUsingItem.erase(iter_usingitem);
					break;
				}
				
			}
		}
		break;
	case _XQUESTSTART_WEARITEM:
		{
			list <int>::iterator iter_wearitem;
			for(iter_wearitem = g_QuestScriptManager.m_listQuestStartTypeWearItem.begin() ; iter_wearitem != g_QuestScriptManager.m_listQuestStartTypeWearItem.end() ; ++iter_wearitem)
			{
				int questid = *iter_wearitem;
				if( m_nQuestID == questid )
				{
					iter_wearitem = g_QuestScriptManager.m_listQuestStartTypeWearItem.erase(iter_wearitem);
					break;
				}
			}
		}
		break;
	case _XQUESTSTART_DESTROYITEM:
		{
			list <int>::iterator iter_destroyitem;
			for(iter_destroyitem = g_QuestScriptManager.m_listQuestStartTypeDestroyItem.begin() ; iter_destroyitem != g_QuestScriptManager.m_listQuestStartTypeDestroyItem.end() ; ++iter_destroyitem)
			{
				int questid = *iter_destroyitem;
				if( m_nQuestID == questid )
				{
					iter_destroyitem = g_QuestScriptManager.m_listQuestStartTypeDestroyItem.erase(iter_destroyitem);
					break;
				}
			}
		}
		break;
	case _XQUESTSTART_LEVEL:
		{
			list <int>::iterator iter_level;
			for(iter_level = g_QuestScriptManager.m_listQuestStartTypeLevel.begin() ; iter_level != g_QuestScriptManager.m_listQuestStartTypeLevel.end() ; ++iter_level)
			{
				int questid = *iter_level;
				if( m_nQuestID == questid )
				{
					iter_level = g_QuestScriptManager.m_listQuestStartTypeLevel.erase(iter_level);
					break;
				}
			}
		}
		break;
	case _XQUESTSTART_ZONE:
	case _XQUESTSTART_NONE:
		{
			list <int>::iterator iter_zone;
			for(iter_zone = g_QuestScriptManager.m_listQuestStartTypeZone.begin() ; iter_zone != g_QuestScriptManager.m_listQuestStartTypeZone.end() ; ++iter_zone)
			{
				int questid = *iter_zone;
				if( m_nQuestID == questid )
				{
					iter_zone = g_QuestScriptManager.m_listQuestStartTypeZone.erase(iter_zone);
					break;
				}
			}
		}
		break;
	}	
}

void _XQuestScriptItem::ResetCounterInformation(void)
{	
	for( int i = 0; i < 4; ++i )
	{
		m_CounterInformation[i].CounterType = -1;
		m_CounterInformation[i].CounterTargetType = -1;		
		m_CounterInformation[i].CounterTargetID = -1;		
		m_CounterInformation[i].CountTotalPoint = -1;
		m_CounterInformation[i].CountCurrentPoint = 0;
		memset( m_CounterInformation[i].CounterTargetName,0, sizeof(TCHAR)*256 );
	}
}
void _XQuestScriptItem::ResetPartyCounterInformation(void)
{	
	for( int i = 0; i < 4; ++i )
	{
		m_PartyCounterInformation[i].CounterType = -1;
		m_PartyCounterInformation[i].CounterTargetType = -1;		
		m_PartyCounterInformation[i].CounterTargetID = -1;		
		m_PartyCounterInformation[i].CountTotalPoint = -1;
		m_PartyCounterInformation[i].CountCurrentPoint = 0;
		for( int j= 0; j < _XDEF_MAXPARTYMEMBERCOUNT; ++j)
		{
			m_PartyCounterInformation[i].CounterPointPerMember[j] = 0;
		}
		memset( m_PartyCounterInformation[i].CounterTargetName,0, sizeof(TCHAR)*256 );
	}
}

void _XQuestScriptItem::ResetItemCounterInformation(void)
{	
	for( int i = 0; i < 8; ++i )
	{
		m_ItemCounterInformation[i].CounterType = -1;
		m_ItemCounterInformation[i].CounterTargetType = -1;		
		m_ItemCounterInformation[i].CounterTargetID = -1;		
		m_ItemCounterInformation[i].CountTotalPoint = -1;
		m_ItemCounterInformation[i].CountCurrentPoint = 0;
		memset( m_ItemCounterInformation[i].CounterTargetName,0, sizeof(TCHAR)*256 );
	}
}

BOOL _XQuestScriptItem::CheckStartNEndTime()
{
	SYSTEMTIME TimeInfo;
	GetLocalTime( &TimeInfo );

	BOOL bStart = FALSE, bEnd = FALSE;

	if(this->m_nStartYear < TimeInfo.wYear)
	{
		// 시작 날짜가 작년
		bStart = TRUE;
	}
	else if(this->m_nStartYear == TimeInfo.wYear)
	{
		// 올해
		if(this->m_nStartMonth < TimeInfo.wMonth)
		{
			// 시작 날짜가 지난달
			bStart = TRUE;
		}
		else if(this->m_nStartMonth == TimeInfo.wMonth)
		{
			// 이번달 
			if(this->m_nStartDay < TimeInfo.wDay)
			{
				// 시작 날짜가 어제 이 전
				bStart = TRUE;
			}
			else
			{
				// 오늘
				if(this->m_nStartHour <= TimeInfo.wHour)
				{
					// 시간 체크
					bStart = TRUE;
				}
			}
		}
	}

	if(this->m_nEndYear < TimeInfo.wYear)
	{
		// 종료 날짜가 작년
		bEnd = TRUE;
	}
	else if(this->m_nEndYear == TimeInfo.wYear)
	{
		// 올해
		if(this->m_nEndMonth < TimeInfo.wMonth)
		{
			// 종료 날짜가 지난달
			bEnd = TRUE;
		}
		else if(this->m_nEndMonth == TimeInfo.wMonth)
		{
			// 이번달 
			if(this->m_nEndDay < TimeInfo.wDay)
			{
				// 종료 날짜가 어제 이 전
				bEnd = TRUE;
			}
			else if(this->m_nEndDay == TimeInfo.wDay)
			{
				// 오늘
				if(this->m_nEndHour <= TimeInfo.wHour)
				{
					// 시간 체크
					bEnd = TRUE;
				}
			}
		}
	}

	if(bStart == TRUE)
	{
		// 시작 날짜 이후
		if(bEnd == FALSE)
		{
			// 종료 날짜 지나지 않았음
			return TRUE;
		}
	}

	return FALSE;
}

BOOL _XQuestScriptItem::CheckCountryCode()
{
	if(this->m_unCountryCode == 0)		// 모든 국가
		return TRUE;

	unsigned int result = 0;

	switch(g_LanguageType)
	{
	case _XLANGUAGE_TYPE_KOREAN :
		{
			result = m_unCountryCode & _QUEST_COUNTRYCODE_KR;
			if(result == _QUEST_COUNTRYCODE_KR)
				return TRUE;
		}
		break;
	case _XLANGUAGE_TYPE_VIETNAMESE :
		{
			result = m_unCountryCode & _QUEST_COUNTRYCODE_VN;
			if(result == _QUEST_COUNTRYCODE_VN)
				return TRUE;
		}
		break;
	case _XLANGUAGE_TYPE_TAIWANESE :
		{
			result = m_unCountryCode & _QUEST_COUNTRYCODE_TW;
			if(result == _QUEST_COUNTRYCODE_TW)
				return TRUE;
		}
		break;
	case _XLANGUAGE_TYPE_ENGLISH :
		{
			result = m_unCountryCode & _QUEST_COUNTRYCODE_US;
			if(result == _QUEST_COUNTRYCODE_US)
				return TRUE;
		}
		break;
	case _XLANGUAGE_TYPE_CHINESE :
		{
			result = m_unCountryCode & _QUEST_COUNTRYCODE_CN;
			if(result == _QUEST_COUNTRYCODE_CN)
				return TRUE;
		}
		break;
	case _XLANGUAGE_TYPE_JAPANESE :
		{
			result = m_unCountryCode & _QUEST_COUNTRYCODE_JP;
			if(result == _QUEST_COUNTRYCODE_JP)
				return TRUE;
		}
		break;
	case _XLANGUAGE_TYPE_RUSSIAN :
		{
			result = m_unCountryCode & _QUEST_COUNTRYCODE_RS;
			if(result == _QUEST_COUNTRYCODE_RS)
				return TRUE;
		}
		break;
	}
	

	return FALSE;
}