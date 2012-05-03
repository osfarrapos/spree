/** 트리거를 표현하는 클래스.
 *
 * \author Sohyun, Park
 * \date 2003.09.04
 */

#include "stdafx.h"

#pragma warning (disable: 4786)

#include "stdio.h"
#include "XKernel.h"
#include "XTriggerItem.h"
#include "XQuestScriptItem.h"
#include "EODEXTDEF_GlobalObject.h"

_XTriggerItem::_XTriggerItem()
{
	m_bMainQuest	= FALSE;
	m_QuestType		= _XQUEST_TYPE_NONE;
	m_bEnable		= FALSE;
	m_bClientEnable	= FALSE;
	m_bServerEnable = FALSE;
	m_bConfirmAll	= FALSE;
	

	m_lpszTriggerName	= NULL;
	
	m_byCompareClass	= 0;
	m_byTriggerClass	= ID_TRIGGER_CLASS_NORMAL;

	m_nTriggerID	= 0;
	m_nOwnerQuestID	= 0;

//	m_bComplete		= FALSE;
//	m_bLastTrigger	= FALSE;
	m_bExecNormal	= TRUE;

	m_bFormerTriggerSuccess = FALSE;

	m_nOwnerNPCID	= 0;

	m_bWaitRewordPacket = FALSE;
	m_bJumpAction = FALSE;

	m_CurrentTreeNodeNumber = 0;

	for( int i=0; i < 20; ++i)
	{
		m_SuccessConditionid[i] = -1;
		m_SuccessActionid[i] = -1;
	}
	
}

_XTriggerItem::~_XTriggerItem()
{
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
#else
	SAFE_DELETE_ARRAY(m_lpszTriggerName);
#endif

	map <int, _XTriggerConditionItem*>::iterator iter_condition;
	_XTriggerConditionItem* currentcondition = NULL;
	for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
	{
		currentcondition = iter_condition->second;
		if(currentcondition)
		{
			delete currentcondition;
			currentcondition = NULL;
		}
	}
	m_mapConditionList.clear();

	map <int, _XTriggerActionItem*>::iterator iter_action;
	_XTriggerActionItem* currentaction = NULL;
	for(iter_action = m_mapNormalActionList.begin() ; iter_action != m_mapNormalActionList.end() ; ++iter_action)
	{
		currentaction = iter_action->second;
		if(currentaction)
		{
			delete currentaction;
			currentaction = NULL;
		}
	}
	m_mapNormalActionList.clear();

	currentaction = NULL;
	for(iter_action = m_mapExceptedActionList.begin() ; iter_action != m_mapExceptedActionList.end() ; ++iter_action)
	{
		currentaction = iter_action->second;
		if(currentaction)
		{
			delete currentaction;
			currentaction = NULL;
		}
	}
	m_mapExceptedActionList.clear();
}

void _XTriggerItem::SetTreeNodeNumber(int treenumber)
{
	m_CurrentTreeNodeNumber = treenumber;
}

int _XTriggerItem::GetTreeNodeNumber(void)
{
	return m_CurrentTreeNodeNumber;
}

void _XTriggerItem::SetTriggerClass(BYTE triggerclass)
{
	m_byTriggerClass = triggerclass;
}

BYTE _XTriggerItem::GetTriggerClass(void)
{
	return m_byTriggerClass;
}


void _XTriggerItem::SetTriggerID(int triggerid)
{
	m_nTriggerID = triggerid;
}

int _XTriggerItem::GetTriggerID(void)
{
	return m_nTriggerID;
}

void _XTriggerItem::SetComplete(BOOL complete)
{
	m_bComplete	= complete;
}

BOOL _XTriggerItem::GetComplete(void)
{
	return m_bComplete;
}

void _XTriggerItem::SetClientEnable(BOOL enable)
{
	m_bClientEnable = enable;
}

void _XTriggerItem::SetServerEnable(BOOL enable)
{
	m_bServerEnable = enable;
}

void _XTriggerItem::SetConfirmAll(BOOL enable)
{
	m_bConfirmAll = enable;
}

void _XTriggerItem::SetProgressAction( BOOL bNormal )
{	
	if( bNormal )
	{
		_XTriggerActionItem* currentaction = NULL;
		map <int, _XTriggerActionItem*>::iterator iter_normalaction;
		for(iter_normalaction = m_mapNormalActionList.begin() ; iter_normalaction != m_mapNormalActionList.end() ; ++iter_normalaction)
		{
			currentaction = iter_normalaction->second;
			if(currentaction)
			{
				if(!currentaction->NeedClientProcess())
				{	
					bool checksuccess = FALSE;
					for( int i = 0; i < 20; ++i )
					{
						if( m_SuccessActionid[i] == currentaction->GetActionID() )
						{
							checksuccess = TRUE;
							break;
						}
						
					}
					
					if( !checksuccess )
					{
						for( int i = 0; i < 20; ++i )
						{
							if( m_SuccessActionid[i] == -1 )
							{
								m_SuccessActionid[i] = currentaction->GetActionID();
								break;
							}
						}			
					}
				}
			}
		}
	}
	else //if(m_byTriggerClass == ID_TRIGGER_CLASS_JUMP || m_byTriggerClass == ID_TRIGGER_CLASS_END )
	{
		_XTriggerActionItem* currentaction = NULL;
		map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
		for(iter_exceptedaction = m_mapExceptedActionList.begin() ; iter_exceptedaction != m_mapExceptedActionList.end() ; ++iter_exceptedaction)
		{
			currentaction = iter_exceptedaction->second;
			if(currentaction)
			{
				if(!currentaction->NeedClientProcess())
				{
					bool checksuccess = FALSE;
					for( int i = 0; i < 20; ++i )
					{
						if( m_SuccessActionid[i] == currentaction->GetActionID() )
						{
							checksuccess = TRUE;
							break;
						}
						
					}
					
					if( !checksuccess )
					{
						for( int i = 0; i < 20; ++i )
						{
							if( m_SuccessActionid[i] == -1 )
							{
								m_SuccessActionid[i] = currentaction->GetActionID();
								break;
							}
						}		
					}
				}
			}
		}
	}


}

void _XTriggerItem::SetOwnerQuestID(int questid)
{
	m_nOwnerQuestID = questid;
}

int _XTriggerItem::GetOwnerQuestID(void)
{
	return m_nOwnerQuestID;
}

void _XTriggerItem::SetLastTrigger(BOOL last)
{
	m_bLastTrigger = last;
}

BOOL _XTriggerItem::GetLastTrigger(void)
{
	return m_bLastTrigger;
}

void _XTriggerItem::SetFormerTriggerSuccess(BOOL success)
{
	m_bFormerTriggerSuccess = success;
}

BOOL _XTriggerItem::GetFormerTriggerSuccess(void)
{
	return m_bFormerTriggerSuccess;
}

_XQUESTTRIGGER_RESULT _XTriggerItem::Process(void)
{
	if(!m_bEnable)
	{
		m_bConfirmAll = TRUE;
		m_bComplete = TRUE;
		return QUEST_TRIGGER_RESULT_BLOCK;
	}
	if(!m_bClientEnable)
	{
		if(!m_mapConditionList.empty())
		{
			int successconditioncount = 0;
			map <int, _XTriggerConditionItem*>::iterator iter_condition;
			_XTriggerConditionItem* currentcondition = NULL;
			for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
			{
				currentcondition = iter_condition->second;
				if(currentcondition)
				{					
					bool checksuccess = FALSE;
					for( int i = 0; i < m_mapConditionList.size(); ++i )
					{
						if( m_SuccessConditionid[i] == currentcondition->GetConditionID() )
						{
							checksuccess = TRUE;
							successconditioncount++;
							break;
						}

					}

					if( !checksuccess )
					{
						_XQUESTCONDITION_RESULT conditionresult;
						conditionresult = currentcondition->Process();
						if( conditionresult == QUEST_CONDITION_RESULT_SUCCESS)
						{							
							switch(m_byCompareClass)
							{
							case ID_TRIGGER_CONDITIONFUNCTION_OR :
								{
									//하나 이상 만족일때는 마크 세팅은 표시하지 않는다.
									switch(currentcondition->m_TDC.m_conditionclass) 
									{
									case ID_CONDITION_SETQUESTMARKAREA :	// QUEST마크(!) 표시하기 영역
									case ID_CONDITION_SETQUESTMARKNPC :		// QUEST마크(!) 표시하기 NPC 
									case ID_CONDITION_SETHINTMARKAREA :		// HINT마크(?) 표시하기 영역
									case ID_CONDITION_SETHINTMARKNPC :		// HINT마크(?) 표시하기 NPC 
										break;
									default:
										{
											for( int i = 0; i < 20; ++i )
											{
												if( m_SuccessConditionid[i] == -1 )
												{
													m_SuccessConditionid[i] = currentcondition->GetConditionID();
													
													//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
													//_XLog("Quest CurrentNode Condition Success : QuestID[%d] CurrentNode[%d] TriggerID[%d] ConditionID[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber, m_nTriggerID, currentcondition->GetConditionID() );
													
													break;
												}
											}
										}
										successconditioncount++;
										break;
									}
								}
								break;
							case ID_TRIGGER_CONDITIONFUNCTION_AND :
								{									
									for( int i = 0; i < 20; ++i )
									{
										if( m_SuccessConditionid[i] == -1 )
										{
											m_SuccessConditionid[i] = currentcondition->GetConditionID();
											
											//_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT SETTING)     >>>>>>>>>>>>>>>>>>>>" );
											//_XLog("Quest CurrentNode Condition Success : QuestID[%d] CurrentNode[%d] TriggerID[%d] ConditionID[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber, m_nTriggerID, currentcondition->GetConditionID() );
											
											break;
										}
									}			
									successconditioncount++;						
								}
								break;
							}						
						}
						else if( conditionresult == QUEST_CONDITION_RESULT_PROCESS )
						{
							return QUEST_TRIGGER_RESULT_PROCESS;
						}
						
					}
				}
			}
					
			bool result = false;
			int conditioncount = m_mapConditionList.size();

			switch(m_byCompareClass)
			{
			case ID_TRIGGER_CONDITIONFUNCTION_OR :
				{
					if( successconditioncount > 0 )
						result = true;
				}
				break;
			case ID_TRIGGER_CONDITIONFUNCTION_AND :
				{
					if( conditioncount == successconditioncount )
						result = true;
				}
				break;
			}

			if( result )
			{
				m_bExecNormal = TRUE;
				m_bClientEnable = TRUE;
				
				for( int i = 0; i < 20; ++i )
				{
					m_SuccessConditionid[i] = -1;
				}
				g_NetworkKernel.SendPacket(MSG_NO_QUEST_SCON_REQUEST, m_nOwnerQuestID, m_CurrentTreeNodeNumber,TRUE);		
				if( !m_bMainQuest )
				{
					_XQuestScriptItem* currentquest = NULL;
					map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
					if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
					{
						currentquest = iter_quest->second;
						if( currentquest )
						{
							if( currentquest->GetPartyQuest() )
							{
								currentquest->m_bSendClientPacketAtPartyQuest = TRUE;
							}
						}
					}
				}
				
#ifdef _XDWDEBUG	
				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );
				_XLog("Quest CurrentNode Condition Success : QuestID[%d] SucceessNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif

				map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(m_nOwnerQuestID);
				if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
				{
					_XQUEST_INVENFULL invenfullstruct = iter_rewordquest->second;
					int triggerid = invenfullstruct.triggerid;
					if(triggerid == m_nTriggerID)
					{
						m_bWaitRewordPacket = TRUE;
					}
				}
				
				// NPCTagetting 중복체크 플래그를 초기화 시킨다.
				map <int, _XTriggerConditionItem*>::iterator iter_condition;
				_XTriggerConditionItem* currentcondition = NULL;
				for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
				{
					currentcondition = iter_condition->second;
					if(currentcondition)
					{
						currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
					}
				}			
	
				return QUEST_TRIGGER_RESULT_WAITSERVER;
			}
			else
			{
				// 분기 되는 트리에서 실패이면 바로 예외 행동식으로 넘어간다.
				// 또는 끝날 때는 현재 번호를 보낸다.
				if(m_byTriggerClass == ID_TRIGGER_CLASS_JUMP )
				{
					m_bExecNormal = FALSE;
					m_bClientEnable = TRUE;
					for( int i = 0; i < 20; ++i )
					{
						m_SuccessConditionid[i] = -1;
					}
					g_NetworkKernel.SendPacket(MSG_NO_QUEST_SCON_REQUEST, m_nOwnerQuestID, m_CurrentTreeNodeNumber,FALSE);		
					if( !m_bMainQuest )
					{
						_XQuestScriptItem* currentquest = NULL;
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
						if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
						{
							currentquest = iter_quest->second;
							if( currentquest )
							{
								if( currentquest->GetPartyQuest() )
								{
									currentquest->m_bSendClientPacketAtPartyQuest = TRUE;
								}
							}
						}
					}	
					
#ifdef _XDWDEBUG	
					_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );
					_XLog("Quest CurrentNode Condition Success : QuestID[%d] SucceessNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif
					
					map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(m_nOwnerQuestID);
					if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
					{
						_XQUEST_INVENFULL invenfullstruct = iter_rewordquest->second;
						int triggerid = invenfullstruct.triggerid;
						if(triggerid == m_nTriggerID)
						{
							m_bWaitRewordPacket = TRUE;
						}
					}
					
					// NPCTagetting 중복체크 플래그를 초기화 시킨다.
					map <int, _XTriggerConditionItem*>::iterator iter_condition;
					_XTriggerConditionItem* currentcondition = NULL;
					for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
					{
						currentcondition = iter_condition->second;
						if(currentcondition)
						{
							currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
						}
					}			

					return QUEST_TRIGGER_RESULT_WAITSERVER;
				}
				else if( m_byTriggerClass == ID_TRIGGER_CLASS_END )
				{
					if( !m_mapExceptedActionList.empty() )
					{
						m_bExecNormal = FALSE;
						m_bClientEnable = TRUE;
						for( int i = 0; i < 20; ++i )
						{
							m_SuccessConditionid[i] = -1;
						}
						//트리거 아이디에서 트리노드번호로 바뀜
						g_NetworkKernel.SendPacket(MSG_NO_QUEST_SCON_REQUEST, m_nOwnerQuestID, m_CurrentTreeNodeNumber,FALSE);		
						if( !m_bMainQuest )
						{
							_XQuestScriptItem* currentquest = NULL;
							map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
							if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
							{
								currentquest = iter_quest->second;
								if( currentquest )
								{
									if( currentquest->GetPartyQuest() )
									{
										currentquest->m_bSendClientPacketAtPartyQuest = TRUE;
									}
								}
							}
						}	
						
#ifdef _XDWDEBUG	
						_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );
						_XLog("Quest CurrentNode Condition Success : QuestID[%d] SucceessNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif
						
						map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(m_nOwnerQuestID);
						if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
						{
							_XQUEST_INVENFULL invenfullstruct = iter_rewordquest->second;
							int triggerid = invenfullstruct.triggerid;
							if(triggerid == m_nTriggerID)
							{
								m_bWaitRewordPacket = TRUE;
							}
						}
						
						// NPCTagetting 중복체크 플래그를 초기화 시킨다.
						map <int, _XTriggerConditionItem*>::iterator iter_condition;
						_XTriggerConditionItem* currentcondition = NULL;
						for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
						{
							currentcondition = iter_condition->second;
							if(currentcondition)
							{
								currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
							}
						}			
						
						return QUEST_TRIGGER_RESULT_WAITSERVER;
					}
				
				}
			}

			return QUEST_TRIGGER_RESULT_PROCESS;
		}
		else
		{
			// 조건 리스트가 비어있음
			m_bExecNormal = TRUE;
			m_bClientEnable = TRUE;
			
			for( int i = 0; i < 20; ++i )
			{
				m_SuccessConditionid[i] = -1;
			}
			g_NetworkKernel.SendPacket(MSG_NO_QUEST_SCON_REQUEST, m_nOwnerQuestID, m_CurrentTreeNodeNumber,TRUE);		
			if( !m_bMainQuest )
			{
				_XQuestScriptItem* currentquest = NULL;
				map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
				if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
				{
					currentquest = iter_quest->second;
					if( currentquest )
					{
						if( currentquest->GetPartyQuest() )
						{
							currentquest->m_bSendClientPacketAtPartyQuest = TRUE;
						}
					}
				}
			}	
			
#ifdef _XDWDEBUG	
			_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );
			_XLog("Quest CurrentNode Condition Success : QuestID[%d] SucceessNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif
			
			map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(m_nOwnerQuestID);
			if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
			{
				_XQUEST_INVENFULL invenfullstruct = iter_rewordquest->second;
				int triggerid = invenfullstruct.triggerid;
				if(triggerid == m_nTriggerID)
				{
					m_bWaitRewordPacket = TRUE;
				}
			}
		
			
			return QUEST_TRIGGER_RESULT_WAITSERVER;
		}
	}
	else if(m_bServerEnable)
	{
		if(m_bExecNormal) // 조건이 만족할 때 실행 
		{
			if( m_bJumpAction )// 서버에서 에러가 났을 경우 처리를 위한 부분 
			{
				m_bConfirmAll = TRUE;
				m_bComplete = TRUE;
				m_bServerEnable = FALSE;
				
				for( int i = 0; i < 20; ++i )
				{
					m_SuccessActionid[i] = -1;
				}
#ifdef _XDWDEBUG	
				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
				_XLog("Jump Quest CurrentNode Action : QuestID[%d] JumpNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif
				
				return QUEST_TRIGGER_RESULT_SUCCESS;
			}

			if(!m_mapNormalActionList.empty())
			{
				int successactioncount = 0;
				int actioncount = m_mapNormalActionList.size();
				
				map <int, _XTriggerActionItem*>::iterator iter_action;
				_XTriggerActionItem* currentaction = NULL;
				for(iter_action = m_mapNormalActionList.begin() ; iter_action != m_mapNormalActionList.end() ; ++iter_action)
				{
					currentaction = iter_action->second;
					if(currentaction)
					{
						bool checksuccess = FALSE;
						for( int i = 0; i < actioncount; ++i )
						{
							if( m_SuccessActionid[i] == currentaction->GetActionID() )
							{
								checksuccess = TRUE;
								successactioncount++;
								break;
							}
							
						}
						
						if( !checksuccess )
						{
							if(currentaction->Process() == QUEST_ACTION_RESULT_SUCCESS)
							{					
								for( int i = 0; i < 20; ++i )
								{
									if( m_SuccessActionid[i] == -1 )
									{
										m_SuccessActionid[i] = currentaction->GetActionID();
										break;
									}
								}			
								successactioncount++;		
							}
						}		
					}
				}
				if( actioncount == successactioncount )
				{
					for( int i = 0; i < 20; ++i )
					{
						m_SuccessActionid[i] = -1;
					}
					return QUEST_TRIGGER_RESULT_SUCCESS;
				}				
				return QUEST_TRIGGER_RESULT_PROCESS;
			}
			else //행동 리스트가 비어있으면 성공으로 간주하고 다음으로 넘어감
			{				
				m_bConfirmAll = TRUE;
				m_bComplete = TRUE;
				//m_bClientEnable = FALSE;
				m_bServerEnable = FALSE;
				
				for( int i = 0; i < 20; ++i )
				{
					m_SuccessActionid[i] = -1;
				}
#ifdef _XDWDEBUG	
				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
				_XLog("Success Quest CurrentNode Action : QuestID[%d] CurrentNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif
				
				return QUEST_TRIGGER_RESULT_SUCCESS;
			}
		}//if(m_bExecNormal)
		else // 분기시에 조건이 만족되지 않았을 때 실행 
		{ 			
			if( m_bJumpAction )// 서버에서 에러가 났을 경우 처리를 위한 부분 
			{
				m_bConfirmAll = TRUE;
				m_bComplete = TRUE;
				m_bServerEnable = FALSE;
				
				for( int i = 0; i < 20; ++i )
				{
					m_SuccessActionid[i] = -1;
				}
#ifdef _XDWDEBUG	
				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
				_XLog("Jump Quest CurrentNode Action : QuestID[%d] JumpNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif
				
				return QUEST_TRIGGER_RESULT_SUCCESS;
			}

			if(!m_mapExceptedActionList.empty())
			{
				int successactioncount = 0;
				int actioncount = m_mapExceptedActionList.size();
				
				map <int, _XTriggerActionItem*>::iterator iter_action;
				_XTriggerActionItem* currentaction = NULL;
				for(iter_action = m_mapExceptedActionList.begin() ; iter_action != m_mapExceptedActionList.end() ; ++iter_action)
				{
					currentaction = iter_action->second;
					if(currentaction)
					{
						bool checksuccess = FALSE;
						for( int i = 0; i < actioncount; ++i )
						{
							if( m_SuccessActionid[i] == currentaction->GetActionID() )
							{
								checksuccess = TRUE;
								successactioncount++;
								break;
							}
							
						}
						
						if( !checksuccess )
						{
							if(currentaction->Process() == QUEST_ACTION_RESULT_SUCCESS)
							{		
								for( int i = 0; i < 20; ++i )
								{
									if( m_SuccessActionid[i] == -1 )
									{
										m_SuccessActionid[i] = currentaction->GetActionID();	
										break;
									}
								}			
								successactioncount++;				
							}
						}		
					}
				}
				if( actioncount == successactioncount )
				{
					for( int i = 0; i < 20; ++i )
					{
						m_SuccessActionid[i] = -1;
					}
					return QUEST_TRIGGER_RESULT_SUCCESS;
				}
				return QUEST_TRIGGER_RESULT_PROCESS;
			}
			else //행동 리스트가 비어있으면 성공으로 간주하고 다음으로 넘어감
			{				
				m_bConfirmAll = TRUE;
				m_bComplete = TRUE;
				//m_bClientEnable = FALSE;
				m_bServerEnable = FALSE;

				for( int i = 0; i < 20; ++i )
				{
					m_SuccessActionid[i] = -1;
				}
				
#ifdef _XDWDEBUG	
				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
				_XLog("Success Quest CurrentNode Action : QuestID[%d] CurrentNode[%d]", m_nOwnerQuestID,m_CurrentTreeNodeNumber );
#endif
				
				return QUEST_TRIGGER_RESULT_SUCCESS;
			}
		}//else if(!m_bExecNormal)
	}
	return QUEST_TRIGGER_RESULT_WAITSERVER;
}

BOOL _XTriggerItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	m_bEnable = TRUE;

	map <int, _XTriggerConditionItem*>::iterator iter_condition;
	_XTriggerConditionItem* currentcondition = NULL;
	for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
	{
		currentcondition = iter_condition->second;
		if(currentcondition)
			delete currentcondition;
	}
	m_mapConditionList.clear();

	map <int, _XTriggerActionItem*>::iterator iter_normalaction;
	_XTriggerActionItem* currentaction = NULL;
	for(iter_normalaction = m_mapNormalActionList.begin() ; iter_normalaction != m_mapNormalActionList.end() ; ++iter_normalaction)
	{
		currentaction = iter_normalaction->second;
		if(currentaction)
			delete currentaction;
	}
	m_mapNormalActionList.clear();

	map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
	currentaction = NULL;
	for(iter_exceptedaction = m_mapExceptedActionList.begin() ; iter_exceptedaction != m_mapExceptedActionList.end() ; ++iter_exceptedaction)
	{
		currentaction = iter_exceptedaction->second;
		if(currentaction)
			delete currentaction;
	}
	m_mapExceptedActionList.clear();

	int triggerid;

	if(fread(&triggerid, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	int readstringlength = 0;
	if( (readstringlength = LoadString(fileptr, m_lpszTriggerName)) == -1 )
		return FALSE;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszTriggerName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tTrigger name string\t...\t%d\t%d\t\t%s", triggerid, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszTriggerName );
	}
#endif

	if(fread(&m_bEnable, sizeof(BOOL), 1, fileptr) < 1)
		return FALSE;	
	m_bEnable = TRUE;
	
	if(fread(&m_byCompareClass, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;	

	int itemcount = 0;
	if(fread(&itemcount, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(itemcount > 0)
	{
		_XTriggerConditionItem* newcondition = NULL;
		for(int i = 0 ; i < itemcount ; ++i)
		{
			newcondition = new _XTriggerConditionItem;
			newcondition->SetOwnerQuestID(m_nOwnerQuestID);
			newcondition->SetOwnerTriggerID(m_nTriggerID);
			newcondition->SetConditionID(i + 1);
			newcondition->SetQuestType(m_QuestType);
			newcondition->SetMainQuest(m_bMainQuest);
			if(!newcondition->Load(fileptr))
			{
				delete newcondition;
				return FALSE;
			}
			m_mapConditionList[newcondition->GetConditionID()] = newcondition;
		}
	}

	itemcount = 0;
	if(fread(&itemcount, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(itemcount > 0)
	{
		_XTriggerActionItem* newaction = NULL;
		for(int i = 0 ; i < itemcount ; ++i)
		{
			newaction = new _XTriggerActionItem;
			newaction->SetOwnerQuestID(m_nOwnerQuestID);
			newaction->SetOwnerTriggerID(m_nTriggerID);
			newaction->SetActionID(i + 1);
			newaction->SetQuestType(m_QuestType);
			newaction->SetMainQuest(m_bMainQuest);
			if(!newaction->Load(fileptr))
			{
				delete newaction;
				return FALSE;
			}
			m_mapNormalActionList[newaction->GetActionID()] = newaction;
		}
	}

	itemcount = 0;
	if(fread(&itemcount, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(itemcount > 0)
	{
		_XTriggerActionItem* newaction = NULL;
		for(int i = 0 ; i < itemcount ; ++i)
		{
			newaction = new _XTriggerActionItem;
			newaction->SetOwnerQuestID(m_nOwnerQuestID);
			newaction->SetOwnerTriggerID(m_nTriggerID);
			newaction->SetActionID(i + 1);
			newaction->SetQuestType(m_QuestType);
			newaction->SetMainQuest(m_bMainQuest);
			if(!newaction->Load(fileptr))
			{
				delete newaction;
				return FALSE;
			}
			m_mapExceptedActionList[newaction->GetActionID()] = newaction;
		}
	}

	int tempvalue;
	if(fread(&tempvalue, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	BOOL tempcheck;
	if(fread(&tempcheck, sizeof(BOOL), 1, fileptr) < 1)
		return FALSE;
	
	return TRUE;
}

int	_XTriggerItem::LoadString(FILE* fileptr, LPTSTR& string)
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
		if(fread(string, strlength, 1, fileptr) < 1)
			return -1;
	}
#endif
	
	return strlength;
}

void _XTriggerItem::Reset(void)
{
	m_bClientEnable = FALSE;
	m_bServerEnable = FALSE;
	m_bConfirmAll = FALSE;

	m_bComplete = FALSE;
	m_bExecNormal = TRUE;
	m_bFormerTriggerSuccess = FALSE;
	m_bJumpAction = FALSE;
	
	for( int i = 0; i < 20; ++i )
	{
		m_SuccessConditionid[i] = -1;
		m_SuccessActionid[i] = -1;
	}
	
	map <int, _XTriggerConditionItem*>::iterator iter_condition;
	_XTriggerConditionItem* currentcondition = NULL;
	for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
	{
		currentcondition = iter_condition->second;
		if(currentcondition)
		{
			currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
		}
	}
	
	map <int, _XTriggerActionItem*>::iterator iter_normalaction;
	for(iter_normalaction = m_mapNormalActionList.begin() ; iter_normalaction != m_mapNormalActionList.end() ; ++iter_normalaction)
	{
		_XTriggerActionItem* currentaction = NULL;
		currentaction = iter_normalaction->second;
		if(currentaction)
		{
			currentaction->Reset();
		}
	}

	map <int, _XTriggerActionItem*>::iterator iter_exceptedaction;
	for(iter_exceptedaction = m_mapExceptedActionList.begin() ; iter_exceptedaction != m_mapExceptedActionList.end() ; ++iter_exceptedaction)
	{
		_XTriggerActionItem* currentaction = NULL;
		currentaction = iter_exceptedaction->second;
		if(currentaction)
		{
			currentaction->Reset();
		}
	}
}

//void _XTriggerItem::ProcessTest(int actionid)
//{
//	map <int, _XTriggerActionItem*>::iterator iter_action = m_mapNormalActionList.find(actionid);
//	if(iter_action != m_mapNormalActionList.end())
//	{
//		_XTriggerActionItem* currentaction = iter_action->second;
//		if(currentaction)
//		{
//			currentaction->Process();
//		}
//	}
//}
//
//void _XTriggerItem::RestoreTest(int actionid)
//{
//	map <int, _XTriggerActionItem*>::iterator iter_action = m_mapNormalActionList.find(actionid);
//	if(iter_action != m_mapNormalActionList.end())
//	{
//		_XTriggerActionItem* currentaction = iter_action->second;
//		if(currentaction)
//		{
//			currentaction->RestoreCameraInfo();
//		}
//	}


void _XTriggerItem::SetOwnerNPCID(int npcid)
{
	m_nOwnerNPCID = npcid;
}

int _XTriggerItem::GetOwnerNPCID(void)
{
	if(m_nOwnerNPCID != 0)
		return m_nOwnerNPCID;
//	else
//	{		
//		_XQuestScriptItem* currentquest = NULL;
//		
//		if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
//		{
//			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
//			if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
//				currentquest = iter_quest->second;
//		}
//		else
//		{
//			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
//			if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
//				currentquest = iter_quest->second;
//		}
//		if(currentquest)
//		{
//			return currentquest->GetQuestOwnerNPC();
//		}
//	}


	return 0;
}

void _XTriggerItem::ReleasePrivateData(void)
{
	m_bClientEnable		= FALSE;
	m_bServerEnable		= FALSE;
	m_bConfirmAll		= FALSE;

	m_bComplete			= FALSE;
	m_bExecNormal		= FALSE;

	m_bFormerTriggerSuccess	= FALSE;

	m_nOwnerNPCID		= 0;
	m_bWaitRewordPacket = FALSE;
	m_bJumpAction = FALSE;

	map <int, _XTriggerActionItem*>::iterator iter_action;
	for(iter_action = m_mapNormalActionList.begin() ; iter_action != m_mapNormalActionList.end() ; ++iter_action)
	{
		_XTriggerActionItem* currentaction = iter_action->second;
		if(currentaction)
			currentaction->ReleasePrivateData();
	}

	for(iter_action = m_mapExceptedActionList.begin() ; iter_action != m_mapExceptedActionList.end() ; ++iter_action)
	{
		_XTriggerActionItem* currentaction = iter_action->second;
		if(currentaction)
			currentaction->ReleasePrivateData();
	}
}