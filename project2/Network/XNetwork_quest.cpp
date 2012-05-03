// XNetwork_quest.cpp : quest 관련
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "SoundEffectList.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_JinStatus.h"
#include "XMessageWindow.h"
#include "XWindow_QuickSlot.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_QuestRank.h"

#include "XWindowObjectDefine.h"

extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

BOOL _XNetwork::Send_MSGQuest(int type, int param1, int param2, int param3, int param4)
{
	switch(type)
	{
	case MSG_NO_QUEST_SCON_REQUEST :
		{
			_XDWPACKETLOG("SEND MESSAGE : quest request SCON ");
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("SEND MESSAGE : quest request SCON ========");
			_XDWINPRINT("questid [%d] triggerid[%d]", param1, param2);
#endif
			
			int size = sizeof(MSG_QUEST_AUTH_REQUEST);
			MSG_QUEST_AUTH_REQUEST questrequest;
			ZeroMemory(&questrequest, size);
			questrequest.header.sLength = size;
			questrequest.ucMessage = MSG_NO_QUEST_SCON_REQUEST;
			questrequest.usUniqueID = g_pLocalUser->m_Uniqueid;
			
			questrequest.usQuestID = (short)param1;
			questrequest.cNodeID = (char)param2;
			
#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI

			if(g_pLocalUser->GetInBossRoom())
			{
				// 보스전 안에 있음
				questrequest.ucDungeon = 1;
			}
			else
			{
				questrequest.ucDungeon = 0;
			}

			questrequest.bBypassTorF = (short)param3;

#else
			questrequest.bBypassTorF = (BOOL)param3;
#endif
			
			return _XSendPacket_Encapsulate( (char*)&questrequest );			
		}
		break;
	case MSG_NO_QUEST_SET_SELECTION_NO :
		{
			_XDWPACKETLOG("SEND MESSAGE : quest selection ");
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("SEND MESSAGE : quest request SCON ========");
			_XDWINPRINT("questid [%d] selectionNO[%d]", param1, param2);
#endif
			
			int size = sizeof(MSG_QUEST_SET_SELECTION_NO);
			MSG_QUEST_SET_SELECTION_NO selection;
			ZeroMemory(&selection, size);
			selection.header.sLength = size;
			selection.ucMessage = MSG_NO_QUEST_SET_SELECTION_NO;
						
			selection.usQuestID = (short)param1;
			selection.ucSelectionNo = (char)param2;
			
			return _XSendPacket_Encapsulate( (char*)&selection );
		}
		break;
		
	case MSG_NO_QUEST_USER_CANCEL :
		{
			_XDWPACKETLOG("SEND MESSAGE : quest cancel ");
			
#ifdef _XDWDEBUG
			_XLog("SEND MESSAGE : quest cancel ========");
			_XLog("questid [%d] triggerid[%d]", param1, param2);
#endif

			
			int size = sizeof(MSG_QUEST_USER_CANCEL);
			MSG_QUEST_USER_CANCEL questcancel;
			ZeroMemory(&questcancel, size);
			questcancel.header.sLength = size;
			questcancel.ucMessage = MSG_NO_QUEST_USER_CANCEL;
			questcancel.usUniqueID = g_pLocalUser->m_Uniqueid;
			
			questcancel.usQuestID = (short)param1;
			questcancel.cNodeID = (char)param2;
			
			// 퀘스트가 취소되서 다시 재정렬하는 동안을 알 수 있도록 플래그를 켠다.
			g_QuestScriptManager.m_bCancelProcess = TRUE;
			
			return _XSendPacket_Encapsulate( (char*)&questcancel );
		}
		break;
		
	case MSG_NO_QUEST_SETCOUNT: // 퀘스트 카운터 관련
		{
			_XDWPACKETLOG("SEND MESSAGE : quest count ");
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("SEND MESSAGE : quest count ========");
#endif
						
			int size = sizeof(MSG_QUEST_SETCOUNT);
			MSG_QUEST_SETCOUNT questcount;
			ZeroMemory(&questcount, size);
			questcount.header.sLength = size;
			questcount.ucMessage = MSG_NO_QUEST_SETCOUNT;
			questcount.usQuestId	= (u_short)param1;// 퀘스트 아이디
			questcount.cCounterNo	= (char)param2;// 업데이트해야할 계수기.
#ifdef	_XTS_NEWPARTYCOUNTER
			questcount.cNum			= (char)param3;// 더해질 숫자.
			questcount.usSum		= (u_short)param4;// 클라이언트에서 저장중인 총 카운트 숫자.
#ifdef _XDWDEBUG
			_XLog( " Send Quest Counter Point : questid[%d] counternumber[%d] counterpoint[%d] currentpoint[%d]", questcount.usQuestId, questcount.cCounterNo, questcount.cNum	, questcount.usSum );
#endif
#else
			questcount.ucNum		= (u_char)param3;// 더해질 숫자.
			questcount.ucSum		= (u_char)param4;// 클라이언트에서 저장중인 총 카운트 숫자.
#ifdef _XDWDEBUG
			_XLog( " Send Quest Counter Point : questid[%d] counternumber[%d] counterpoint[%d] currentpoint[%d]", questcount.usQuestId, questcount.cCounterNo, questcount.ucNum, questcount.ucSum );
#endif
			
#endif
			return _XSendPacket_Encapsulate( (char*)&questcount );			
		}
		break;
		
	case MSG_NO_QUEST_VALIDCHECK: // 퀘스트 보안 코드 패킷 :  퀘스트가 유효한지 검사를 한다.
		{
			int size = sizeof(MSG_QUEST_VALIDCHECK);
			MSG_QUEST_VALIDCHECK questcheck;
			ZeroMemory(&questcheck, size);
			questcheck.header.sLength = size;
			questcheck.ucMessage = MSG_NO_QUEST_VALIDCHECK;

			questcheck.usQuestID			= (u_short)param1;//현 퀘스트 아이디.
			questcheck.cNodeID				= (char)param2;// 현 퀘스트가 가야할 노드 번호.
			questcheck.cNeedItemRemove		= (char)param3;// 삭제될 아이템이 있으면 1 없으면 0;
#ifdef _XDWDEBUG	
			_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );	
			_XLog("Send Valid Quest Check : QuestID[%d] GoToNodeID[%d] NeedItemRemove[%d]", questcheck.usQuestID, questcheck.cNodeID, questcheck.cNeedItemRemove );
#endif
			return _XSendPacket_Encapsulate( (char*)&questcheck );						
		}
		break;
		
	case MSG_NO_QUEST_VALIDCHECK_INGAME: // 게임 진행상에서 퀘스트 보안 코드 패킷 :  퀘스트가 유효한지 검사를 한다.
		{
			int size = sizeof(MSG_QUEST_VALIDCHECK);
			MSG_QUEST_VALIDCHECK questcheck;
			ZeroMemory(&questcheck, size);
			questcheck.header.sLength = size;
			questcheck.ucMessage = MSG_NO_QUEST_VALIDCHECK_INGAME;
			
			questcheck.usQuestID			= (u_short)param1;//현 퀘스트 아이디.
			questcheck.cNodeID				= (char)param2;// 현 퀘스트가 가야할 노드 번호.
			questcheck.cNeedItemRemove		= (char)param3;// 삭제될 아이템이 있으면 1 없으면 0;
			
#ifdef _XDWDEBUG	
			_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );	
			_XLog("Send Valid Quest Check : QuestID[%d] GoToNodeID[%d] NeedItemRemove[%d]", questcheck.usQuestID, questcheck.cNodeID, questcheck.cNeedItemRemove );
#endif

			return _XSendPacket_Encapsulate( (char*)&questcheck );
		}
		break;

	case MSG_NO_QUESTRANK_REQ:
		{
			int size = sizeof(MSG_QUESTRANK_REQ);
			MSG_QUESTRANK_REQ questrankinglist;
			ZeroMemory(&questrankinglist, size);
			questrankinglist.header.sLength = size;
			questrankinglist.ucMessage = MSG_NO_QUESTRANK_REQ;
			
			questrankinglist.usQuestID		= (u_short)param1;//현 퀘스트 아이디.
			
#ifdef _XDWDEBUG	
			_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );	
			_XLog("Send Quest Ranking Request : QuestID[%d] ", questrankinglist.usQuestID );
#endif
			
			return _XSendPacket_Encapsulate( (char*)&questrankinglist );
		}
	}
	
	return TRUE;
}

void _XNetwork::MSGQuestData(MSG_QUEST_DATA* questdata)
{
	
#ifdef _XGMCLIENT
	_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (SERVER->CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
	_XLog("Receive MainQuest Data : MainQuestID[%d] MainQuestNodeID[%d]", questdata->MainQuestData.sQuestID, questdata->MainQuestData.ucNodeID );
#endif
	
#ifdef _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI
	ParsingCollectionData(questdata->CollectionData, questdata->EventCollectionData);
#else
	ParsingCollectionData(questdata->CollectionData);
#endif
	
	g_QuestScriptManager.ResetQuest();
	
	g_QuestScriptManager.m_bQuestValidCheck = FALSE;
	g_QuestScriptManager.m_bReceiveQuestData = FALSE;
	g_QuestScriptManager.m_bQuestStartConditionCheck = FALSE;
	
	memset( m_RunningQuestData, 0, sizeof(_sRunningQuestPacketData)*_MAX_CURQUEST_NUM_ );
	for(int i = 0 ; i < _MAX_CURQUEST_NUM_ ; ++i)
	{
		m_RunningQuestData[i] = questdata->RunningQuestData[i];
		
#ifdef _XDWDEBUG	
		_XLog("Running Quest Data : QuestID [%d] ", m_RunningQuestData[i].usQuestID );
#endif
			
	}

	bool bendofmainquest = FALSE;
	// 제룡행 세팅
	if( questdata->MainQuestData.sQuestID == 0 ) // 제룡행 퀘스트 맨 처음일때는 문파에 따라 퀘스트 아이디를 세팅한다.
	{
		switch( g_CurrentZoneInfoIndex-1 ) 
		{
		case 0: // 소림
			{
				g_QuestScriptManager.m_CurrentMainQuestID = 13000;
				g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
				g_QuestScriptManager.SetCheckQuest(TRUE);
			}
			break;
		case 1: // 개방
			{
				g_QuestScriptManager.m_CurrentMainQuestID = 11000;
				g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
				g_QuestScriptManager.SetCheckQuest(TRUE);		
			}
			break;
		case 2: // 녹림
			{
				g_QuestScriptManager.m_CurrentMainQuestID = 14000;
				g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
				g_QuestScriptManager.SetCheckQuest(TRUE);		
			}
			break;
		case 3: // 비궁
			{
				g_QuestScriptManager.m_CurrentMainQuestID = 12000;
				g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
				g_QuestScriptManager.SetCheckQuest(TRUE);		
			}
			break;
		case 4: // 무당
			{
				g_QuestScriptManager.m_CurrentMainQuestID = 15000;
				g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
				g_QuestScriptManager.SetCheckQuest(TRUE);		
			}
			break;
		case 5: // 마교
			{
				g_QuestScriptManager.m_CurrentMainQuestID = 16000;
				g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
				g_QuestScriptManager.SetCheckQuest(TRUE);		
			}
			break;
		} 
		g_QuestScriptManager.SetMainQuestStartCondition(g_QuestScriptManager.m_CurrentMainQuestID);
	}
	else
	{
		if( questdata->MainQuestData.ucNodeID == _QUEST_COMPLETE_SUCCESS_)
		{
			QuestSCONResultSuccessComplete(questdata->MainQuestData.sQuestID, questdata->MainQuestData.ucNodeID);
			g_QuestScriptManager.m_CurrentMainQuestID = 65535;			
			g_QuestScriptManager.m_NextMainQuestID = 65535;
			bendofmainquest = TRUE;
			
			g_QuestScriptManager.SetMainQuestStartCondition(g_QuestScriptManager.m_CurrentMainQuestID);
		}
		else if( questdata->MainQuestData.ucNodeID == _QUEST_COMPLETE_FAIL_ )
		{
			QuestSCONResultFailComplete(questdata->MainQuestData.sQuestID, questdata->MainQuestData.ucNodeID);
			g_QuestScriptManager.m_CurrentMainQuestID = 65535;			
			g_QuestScriptManager.m_NextMainQuestID = 65535;
			bendofmainquest = TRUE;

			g_QuestScriptManager.SetMainQuestStartCondition(g_QuestScriptManager.m_CurrentMainQuestID);
		}
		else
		{
			
			if( questdata->MainQuestData.ucNodeID == 0 )
			{
				g_QuestScriptManager.m_CurrentMainQuestID = questdata->MainQuestData.sQuestID;
				g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
				g_QuestScriptManager.SetCheckQuest(TRUE);	
				
				g_QuestScriptManager.QuestRunningSetting( questdata->MainQuestData.sQuestID, questdata->MainQuestData.ucNodeID );
				g_QuestScriptManager.m_bProgressMainQuest = FALSE;
				
				g_QuestScriptManager.SetMainQuestStartCondition(g_QuestScriptManager.m_CurrentMainQuestID);
			}
			else
			{
				BOOL needitemremove = FALSE;
				int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( questdata->MainQuestData.sQuestID, questdata->MainQuestData.ucNodeID, needitemremove );
				if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
				{
					g_QuestScriptManager.m_bQuestValidCheck = TRUE;
					if( practicablequestnode == 0 )
					{
						// 제룡행 퀘스트 리셋 패킷을 보낸다
						SendPacket(MSG_NO_QUEST_USER_CANCEL, questdata->MainQuestData.sQuestID, questdata->MainQuestData.ucNodeID);
					}
					else
					{
						SendPacket( MSG_NO_QUEST_VALIDCHECK, questdata->MainQuestData.sQuestID, practicablequestnode, needitemremove );
					}
				}
				else// 정상적인 퀘스트 
				{
					g_QuestScriptManager.m_CurrentMainQuestID = questdata->MainQuestData.sQuestID;
					g_QuestScriptManager.SetCheckQuestID(g_QuestScriptManager.m_CurrentMainQuestID);
					g_QuestScriptManager.SetCheckQuest(TRUE);	
					
					g_QuestScriptManager.QuestRunningSetting( questdata->MainQuestData.sQuestID, questdata->MainQuestData.ucNodeID );
					g_QuestScriptManager.m_bProgressMainQuest = TRUE;					
				}
			}
		}
	}
	
			
	if( g_pQuest_Window )
	{
		g_pQuest_Window->SetQuestGroupListItem( questdata->MainQuestData.sQuestID );
		if( g_pQuest_Window->GetShowStatus() )
		{
			g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
			g_pQuest_Window->ProcessSelectedTabButton();	
		}
	}
}

void _XNetwork::MSGQuestCompleteData(MSG_QUESTCOMPLETE_DATA* questcompletedata)
{	
	
#ifdef _XDWDEBUG	
	_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (SERVER->CLIENT)     >>>>>>>>>>>>>>>>>>>>" );	
#endif
	bool questcheck = false;
	for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
	{
		g_QuestScriptManager.m_ProgressSubQuestID[i] = -1;		
	}
	
	for( i = 0 ; i < questcompletedata->sCountCQ ; ++i)
	{
		if(questcompletedata->QuestData[i].ucNodeID == _QUEST_COMPLETE_SUCCESS_)
		{
			g_QuestScriptManager.ResetQuest(questcompletedata->QuestData[i].usQuestID);
			g_QuestScriptManager.QuestSuccessSetting(questcompletedata->QuestData[i].usQuestID);

			// 오의 관련 퀘스트 체크
			switch(g_pLocalUser->m_CharacterInfo.groupindex)
			{
			case _XGROUP_GAEBANG :
				{
					if(questcompletedata->QuestData[i].usQuestID == 21017)
						g_pLocalUser->m_bViewMasterySkill = TRUE;
				}
				break;
			case _XGROUP_BEEGOONG :
				{
					if(questcompletedata->QuestData[i].usQuestID == 22017)
						g_pLocalUser->m_bViewMasterySkill = TRUE;
				}
				break;
			case _XGROUP_SORIM :
				{
					if(questcompletedata->QuestData[i].usQuestID == 23017)
						g_pLocalUser->m_bViewMasterySkill = TRUE;
				}
				break;
			case _XGROUP_NOCKRIM :
				{
					if(questcompletedata->QuestData[i].usQuestID == 24017)
						g_pLocalUser->m_bViewMasterySkill = TRUE;
				}
				break;
			case _XGROUP_MOODANG :
				{

				}
				break;
			case _XGROUP_MAKYO :
				{

				}
				break;
			}
		}
		else if(questcompletedata->QuestData[i].ucNodeID == _QUEST_COMPLETE_FAIL_)
		{
			g_QuestScriptManager.ResetQuest(questcompletedata->QuestData[i].usQuestID);
			g_QuestScriptManager.QuestFailSetting(questcompletedata->QuestData[i].usQuestID);
		}
		else if(questcompletedata->QuestData[i].ucNodeID != 0)
		{
#ifdef _XDWDEBUG	
			_XLog("Receive SubQuest Data : SubQuestID [%d] SubQuestNodeID[%d] ", questcompletedata->QuestData[i].usQuestID, questcompletedata->QuestData[i].ucNodeID );
#endif
			
			BOOL needitemremove = FALSE;
			int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( questcompletedata->QuestData[i].usQuestID, questcompletedata->QuestData[i].ucNodeID, needitemremove );
			if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
			{
				bool bexistrunningquest = false;
				for( int runningindex = 0; runningindex < _MAX_CURQUEST_NUM_; ++runningindex )
				{
					if( m_RunningQuestData[runningindex].usQuestID ==  questcompletedata->QuestData[i].usQuestID )
					{
						bexistrunningquest = true;
						break;
					}
				}

				if( bexistrunningquest )
				{
					g_QuestScriptManager.m_bQuestValidCheck = TRUE;
					questcheck = true;
					if( practicablequestnode == 0 )
					{
						// 제룡행이 아닌 퀘스트 리셋 패킷을 보낸다					
						SendPacket(MSG_NO_QUEST_USER_CANCEL, questcompletedata->QuestData[i].usQuestID, questcompletedata->QuestData[i].ucNodeID);
					}
					else
					{
						SendPacket( MSG_NO_QUEST_VALIDCHECK, questcompletedata->QuestData[i].usQuestID, practicablequestnode, needitemremove );
					}
				}
				else
				{
#ifdef _XDWDEBUG
					_XLog("Error Quest Data : QuestID [%d] ", questcompletedata->QuestData[i].usQuestID );
#endif
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_2605), TRUE, _XDEF_NPCSCRIPT_CLOSE);
					pMessageBox->StartTimer(10000, TRUE) ;					
				}
			}
			else// 정상적인 퀘스트 
			{				
				g_QuestScriptManager.QuestRunningSetting(questcompletedata->QuestData[i].usQuestID, questcompletedata->QuestData[i].ucNodeID);
				
				for( int j = 0; j < ID_QUEST_SYNC-1; ++j ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
				{
					if( g_QuestScriptManager.m_ProgressSubQuestID[j] == -1)
					{
						g_QuestScriptManager.m_ProgressSubQuestID[j] = questcompletedata->QuestData[i].usQuestID;
						g_QuestScriptManager.IncreaseProgressQuestCount();
						break;
					}
				}		
			}			
		}

		if(questcompletedata->QuestData[i].bReword )
		{
			g_QuestScriptManager.InsertRewordQuestList(questcompletedata->QuestData[i].usQuestID, questcompletedata->QuestData[i].ucNodeID, 1);
#ifdef _XDWDEBUG
			_XLog("-- Receive Quest Data ------------------");
			_XLog("questid [%d]  triggerid [%d]", questcompletedata->QuestData[i].usQuestID, questcompletedata->QuestData[i].ucNodeID);
#endif
		}
	}
	

	if( !g_QuestScriptManager.m_bQuestValidCheck )
	{
		SendPacket( MSG_NO_INVEN_REQ );
		SendPacket( MSG_NO_SKILL_INFO_REQ );
		g_QuestScriptManager.m_bReceiveQuestData = TRUE;
		g_QuestScriptManager.m_bQuestStartConditionCheck = TRUE;
		g_QuestScriptManager.QuestStartConditionSort();
		g_QuestScriptManager.QuestStartConditionZoneMode();
		g_QuestScriptManager.QuestStartConditionLevelMode();
		g_pQuest_Window->SetQuestTabButton();

		memset( m_RunningQuestData, 0, sizeof(_sRunningQuestPacketData)*_MAX_CURQUEST_NUM_ );
	}
	else
	{
		g_QuestScriptManager.m_bReceiveQuestData = FALSE;
		g_QuestScriptManager.m_bQuestStartConditionCheck = FALSE;
	}
	// NPC 생성 or 소멸 관련 체크 
	g_Npclist.CheckConditionOfNPCExistence();
}

void _XNetwork::MSGQuestSConResult(MSG_QUEST_AUTH_RESULT* questresult)
{	
#ifdef _XDWDEBUG
	_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (SERVER->CLIENT)     >>>>>>>>>>>>>>>>>>>>" );	
	_XLog("Result Quest : Result[%d] Info[%d]", questresult->cResult, questresult->cInfo );
#endif
	
 	if(questresult->cResult == _QUEST_RESULT_SUCCESS_ || questresult->cResult == _QUEST_FAILRESULT_SUCCESS_ )
	{
		switch(questresult->cInfo)
		{
		case _QUEST_COMPLETE_SUCCESS_ :
			{
#ifdef _XDWDEBUG
				_XLog("Result Quest Success : QuestID[%d]", questresult->usQuestID );
#endif
				
				// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
				if( g_TutorialInfo )
				{
					if( !g_TutorialInfo->_XTutorial_028 )
					{
						_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
						pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_028);
						g_TutorialInfo->_XTutorial_028 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
						g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif						
					}
				}				
				// ----------------------------------------------------------------------------------------------------------------------------=
				
				// 퀘스트 결과를 success로 설정 - 2
				QuestSCONResultSuccessComplete(questresult->usQuestID, questresult->cNodeID);
				_XPlayInterfaceSound(ID_SR_INTERFACE_QUESTSUCCESS01_WAV);
				
				// 제룡행 퀘스트 일 경우 -> 다음 퀘스트 ID를 알려준다.
				if( questresult->usQuestID >= 10000 && questresult->usQuestID < 20000) 
				{
					g_QuestScriptManager.m_NextMainQuestID = questresult->usNextQuestID;
				}

				// NPC 생성 or 소멸 관련 체크 
				g_Npclist.CheckConditionOfNPCExistence();
				
				// NPC대화 화면일 경우에는 대화모드를 나갈 수 있는 버튼을 생성한다.
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
				if(pNPCScript_Window) 
				{
					if( pNPCScript_Window->GetShowStatus() )
					{
						pNPCScript_Window->m_btnQuestOK->ShowWindow(TRUE);
					}
				}
				
				// 오의 관련 퀘스트 체크
				switch(g_pLocalUser->m_CharacterInfo.groupindex)
				{
				case _XGROUP_GAEBANG :
					{
						if(questresult->usQuestID == 21017)
							g_pLocalUser->m_bViewMasterySkill = TRUE;
					}
					break;
				case _XGROUP_BEEGOONG :
					{
						if(questresult->usQuestID == 22017)
							g_pLocalUser->m_bViewMasterySkill = TRUE;
					}
					break;
				case _XGROUP_SORIM :
					{
						if(questresult->usQuestID == 23017)
							g_pLocalUser->m_bViewMasterySkill = TRUE;
					}
					break;
				case _XGROUP_NOCKRIM :
					{
						if(questresult->usQuestID == 24017)
							g_pLocalUser->m_bViewMasterySkill = TRUE;
					}
					break;
				case _XGROUP_MOODANG :
					{

					}
					break;
				case _XGROUP_MAKYO :
					{

					}
					break;
				}
			}
			break;
		case _QUEST_COMPLETE_FAIL_ :
			{
#ifdef _XDWDEBUG
				_XLog("Result Quest Fail : QuestID[%d]", questresult->usQuestID );
#endif

				// 퀘스트 결과를 fail로 설정 - 3
				QuestSCONResultFailComplete(questresult->usQuestID, questresult->cNodeID);				
				_XPlayInterfaceSound(ID_SR_INTERFACE_QUESTFAIL01_WAV);
			
				// 제룡행 퀘스트 일 경우 -> 다음 퀘스트 ID를 알려준다.
				if( questresult->usQuestID >= 10000 && questresult->usQuestID < 20000) 
				{
					g_QuestScriptManager.m_NextMainQuestID = questresult->usNextQuestID;
				}
				
				// NPC 생성 or 소멸 관련 체크 
				g_Npclist.CheckConditionOfNPCExistence();
				
				// NPC대화 화면일 경우에는 대화모드를 나갈 수 있는 버튼을 생성한다.
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
				if(pNPCScript_Window) 
				{
					if( pNPCScript_Window->GetShowStatus() )
					{
						pNPCScript_Window->m_btnQuestOK->ShowWindow(TRUE);
					}
				}
			}
			break;
		default :
			{
				if( questresult->cResult == _QUEST_RESULT_SUCCESS_ )
				{
					if(questresult->cInfo == 1)
					{
#ifdef _XDWDEBUG
						_XLog("Result Quest Start : QuestID[%d] NextNode[%d]", questresult->usQuestID, questresult->cInfo );
#endif
						
						// 시작 조건 만족 - 1.1	
						// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
						if( g_TutorialInfo )
						{
							if( !g_TutorialInfo->_XTutorial_008 )
							{
								_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
								pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_008);							
							}
						}						
						// ----------------------------------------------------------------------------------------------------------------------------=
						

						// 제룡행 퀘스트 일 경우 -> 퀘스트 창의 장을 업데이트 할지 결정한다.
						if( questresult->usQuestID >= 10000 && questresult->usQuestID < 20000) 
						{							
							g_QuestScriptManager.m_bProgressMainQuest = TRUE;
							QuestSCONResultStartCondition(questresult->usQuestID, questresult->cInfo);
							if( g_pQuest_Window )
							{
								g_pQuest_Window->SetQuestGroupListItem( questresult->usQuestID );
								if( g_pQuest_Window->GetShowStatus() )
								{
									g_pQuest_Window->SetListItem();
									g_pQuest_Window->SetQuestTabButton();
									if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST1 )
									{
										g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
										g_pQuest_Window->ProcessSelectedTabButton();	
									}
								}							
							}
						}
						else
						{								
							_XQuestScriptItem* currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[questresult->usQuestID];
							if( currentquest )
							{
								if( !currentquest->GetPartyQuest() )
								{
									if( !currentquest->GetTempQuest() )
									{
										for( int i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
										{
											if( g_QuestScriptManager.m_ProgressSubQuestID[i] == -1)
											{
												g_QuestScriptManager.m_ProgressSubQuestID[i] = questresult->usQuestID;
												g_QuestScriptManager.IncreaseProgressQuestCount();
												break;
											}
										}	
										
										if( g_QuestScriptManager.GetProgressQuestCount() >= ID_QUEST_SYNC-1 )
										{
											g_QuestScriptManager.ResetQuestStartCondition();//리스트들을 재정렬한다.
											g_QuestScriptManager.QuestStartConditionZoneMode();
											g_QuestScriptManager.QuestStartConditionLevelMode();
										}
									}
									else
									{
										if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
										{
											bool bexist = false;
											list <int>::iterator iter_questnosavetype;
											for(iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin() ; iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
											{
												int questid = *iter_questnosavetype;
												if( questid == questresult->usQuestID )
												{
													bexist = true;
													break;
												}
											}
											if( !bexist )
											{
												g_QuestScriptManager.m_listQuestNoSaveType.push_back(questresult->usQuestID);
												
												g_QuestScriptManager.ResetQuestStartCondition();//리스트들을 재정렬한다.
												g_QuestScriptManager.QuestStartConditionZoneMode();
												g_QuestScriptManager.QuestStartConditionLevelMode();
											}
										}
										else
										{
											g_QuestScriptManager.m_listQuestNoSaveType.push_back(questresult->usQuestID);
											
											g_QuestScriptManager.ResetQuestStartCondition();//리스트들을 재정렬한다.
											g_QuestScriptManager.QuestStartConditionZoneMode();
											g_QuestScriptManager.QuestStartConditionLevelMode();
										}
										
									}
								}
								else
								{
									g_pLocalUser->m_bPartyQuestProgress = TRUE;
									if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
									{
										bool bexist = false;
										list <int>::iterator iter_questnosavetype;
										for(iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin() ; iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
										{
											int questid = *iter_questnosavetype;
											if( questid == questresult->usQuestID )
											{
												bexist = true;
												break;
											}
										}
										if( !bexist )
										{
											g_QuestScriptManager.m_listQuestNoSaveType.push_back(questresult->usQuestID);

											g_QuestScriptManager.ResetQuestStartCondition();//리스트들을 재정렬한다.
											g_QuestScriptManager.QuestStartConditionZoneMode();
											g_QuestScriptManager.QuestStartConditionLevelMode();
										}
									}
									else
									{
										g_QuestScriptManager.m_listQuestNoSaveType.push_back(questresult->usQuestID);
										
										g_QuestScriptManager.ResetQuestStartCondition();//리스트들을 재정렬한다.
										g_QuestScriptManager.QuestStartConditionZoneMode();
										g_QuestScriptManager.QuestStartConditionLevelMode();
									}
									
								}
							}
							
							
							QuestSCONResultStartCondition(questresult->usQuestID, questresult->cInfo);
							if( g_pQuest_Window )
							{
								//if( g_pQuest_Window->GetShowStatus() )//Author : 양희왕 //퀘스트 가득 찬 메시지 표시 할 때.. 없어도 된다.
								{
									g_pQuest_Window->SetListItem();
									g_pQuest_Window->SetQuestTabButton();
									if( g_pQuest_Window->m_SelectedTab != _XQUESTTAB_QUEST1 )
									{
										g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
										g_pQuest_Window->m_SelectedQuestID = questresult->usQuestID;
										g_pQuest_Window->ProcessSelectedListBox();
									}
								}
							}
						}
						g_QuestScriptManager.DeleteRecheckingListAboutQuestStartCondition( questresult->usQuestID  );
					}
					else // Quest 진행에서 Normal Action을 실행한다.
					{
						// 트리거 진행 - 1.2	
#ifdef _XDWDEBUG	
						_XDWINPRINT("Result Quest Progress : QuestID[%d] CurrentNode[%d] NextNode[%d]", questresult->usQuestID, questresult->cNodeID, questresult->cInfo);			
#endif		
						
						//서버에서 자동 노드 진행일때는 특별히 클라이언트 정보를 신뢰한다.
						if( g_QuestScriptManager.CheckHaveQuestAutoNode(questresult->usQuestID) )
						{
							QuestSCONResultContinueTriggerNormalAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID, FALSE, TRUE );
						}
						else
						{
							if( questresult->cInfo > questresult->cNodeID)
							{
								QuestSCONResultContinueTriggerNormalAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID);
							}
							else
							{
								// 정상적인 진행이 아닐때 (행동식 점프 ) 
								if( g_QuestScriptManager.CheckHaveQuestJumpAction(questresult->usQuestID,questresult->cNodeID) )
								{
#ifdef _XDWDEBUG
									_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (SERVER->CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
									_XLog("Setting JumpAction : QuestID[%d] JumpNode[%d]", questresult->usQuestID,questresult->cNodeID); 
#endif
									QuestSCONResultContinueTriggerNormalAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID, TRUE);
								}
								else
								{
									QuestSCONResultContinueTriggerNormalAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID);
								}
							}
						}
					}
				}
				else if( questresult->cResult == _QUEST_FAILRESULT_SUCCESS_ ) // Quest 진행에서 Exception Action을 실행한다.
				{
					
					// 트리거 진행 - 1			
#ifdef _XDWDEBUG
					_XDWINPRINT("Result Quest Progress : QuestID[%d] CurrentNode[%d] NextNode[%d]", questresult->usQuestID, questresult->cNodeID, questresult->cInfo);			
					_XLog("Result Quest Progress : QuestID[%d] CurrentNode[%d] NextNode[%d]", questresult->usQuestID, questresult->cNodeID, questresult->cInfo );
#endif
					//서버에서 자동 노드 진행일때는 특별히 클라이언트 정보를 신뢰한다.
					if( g_QuestScriptManager.CheckHaveQuestAutoNode(questresult->usQuestID) )
					{
						QuestSCONResultContinueTriggerExceptionAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID, FALSE, TRUE );
					}
					else
					{
						if( questresult->cInfo > questresult->cNodeID)
						{
							QuestSCONResultContinueTriggerExceptionAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID);
						}
						else
						{
							// 정상적인 진행이 아닐때 (행동식 점프 ) 
							if( g_QuestScriptManager.CheckHaveQuestJumpAction(questresult->usQuestID,questresult->cNodeID) )
							{
#ifdef _XDWDEBUG
								_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (SERVER->CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
								_XLog("Setting JumpAction : QuestID[%d] JumpNode[%d]", questresult->usQuestID,questresult->cNodeID); 
#endif

								QuestSCONResultContinueTriggerExceptionAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID, TRUE);
							}
							else
							{
								QuestSCONResultContinueTriggerExceptionAction(questresult->usQuestID, questresult->cInfo, questresult->cNodeID);
							}
						}
					}
				}

			}
			break;
		}
	}
	else if(questresult->cResult == _QUEST_CONDITION_RESULT_FAILED_)
	{
		switch(questresult->cInfo)
		{
		case _QUEST_INFO_IMPROPER_ID_ :
			{
				// 잘못된 트리거 아이디 - 4
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST CONDITION ERROR ---- improper id ------------------");
				_XLog("QUEST CONDITION ERROR ---- improper id ------------------");
#endif
			}
			break;
		case _QUEST_INFO_PACKET_ERROR_ :
			{
				// 잘못된 인증 패킷 - 4
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST CONDITION ERROR ---- packet error ------------------");
				_XLog("QUEST CONDITION ERROR ---- packet error ------------------");
#endif
			}
			break;
		case _QUEST_INFO_SCRIPT_ERROR_ :
			{
				// Quest script 에러 - 4
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST CONDITION ERROR ---- script error ------------------");
				_XLog("QUEST CONDITION ERROR ---- script error ------------------");
#endif
			}
			break;
		case _QUEST_INFO_UNKNOWN_ERROR_ :
			{				
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_2606), TRUE, _XDEF_NPCSCRIPT_CLOSE);
				pMessageBox->StartTimer(10000, TRUE) ;
				
				// 알수 없는 에러
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST CONDITION ERROR ---- unknown error -----------------");
				_XLog("QUEST CONDITION ERROR ---- unknown error -----------------");
#endif
			}
			break;
		}
		// 퀘스트 초기화 실행 - 4		
	}
	else if(questresult->cResult == _QUEST_ACTION_RESULT_FAILED_)
	{
		switch(questresult->cInfo)
		{
		case _QUEST_INFO_SCRIPT_ERROR_ :
			{
				// 퀘스트 스크립트 에러 - 5
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST ACTION ERROR ---- script error -----------------");
				_XLog("QUEST ACTION ERROR ---- script error -----------------");
#endif
			}
			break;
		case _QUEST_INFO_QUEST_SETERROR_ :
			{
				// 퀘스트 설정 에러 - 5
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST ACTION ERROR ---- set error -----------------");
				_XLog("QUEST ACTION ERROR ---- set error -----------------");
#endif
			}
			break;
		case _QUEST_INFO_INVEN_FULL_ :
			{
				// 인벤토리 꽉참 - 6
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST ACTION ERROR ---- inventory full ----------");
				_XLog("QUEST ACTION ERROR ---- inventory full -----------");
#endif
				
				// 인벤토리가 가득찼을 때 처리하는 프로세스( Node Back or Node Hold )
				QuestInventoryFullProcess(questresult);
			}
			break;
		case _QUEST_INFO_UNKNOWN_ERROR_ :
			{
				// 알수 없는 에러 - 5
#ifdef _XDWDEBUG
				_XDWINPRINT("QUEST ACTION ERROR ---- unknown error -----------------");
				_XLog("QUEST ACTION ERROR ---- unknown error -----------------");
#endif
			}
			break;
		}
	}
	else
	{
		QuestAndSuccessTriggerClientEnableReset(questresult->usQuestID, questresult->cNodeID);
	}
}

// 파티퀘스트 취소시 
void _XNetwork::MSGPartyQuestCancle(MSG_QUEST_USER_CANCEL* partyquestcancle)
{
	g_QuestScriptManager.DeletePartyQuest();
}

void _XNetwork::MSGQuestSetResult(MSG_QUEST_SET_RESULT* questresult)
{
	switch(questresult->cResult)
	{
	case -1 :	// 에러 
		{
			g_QuestScriptManager.m_bCancelProcess = FALSE;
			if( !g_QuestScriptManager.m_bReceiveQuestData )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_2605), TRUE, _XDEF_NPCSCRIPT_CLOSE);
				pMessageBox->StartTimer(10000, TRUE) ;
								
				g_QuestScriptManager.QuestStartConditionSort();
				g_QuestScriptManager.QuestStartConditionZoneMode();
				g_QuestScriptManager.QuestStartConditionLevelMode();
				
				SendPacket( MSG_NO_INVEN_REQ );
				SendPacket( MSG_NO_SKILL_INFO_REQ );
				g_QuestScriptManager.m_bQuestStartConditionCheck = TRUE;
				g_QuestScriptManager.m_bReceiveQuestData = TRUE;
				g_QuestScriptManager.m_bQuestValidCheck = FALSE;
				memset( m_RunningQuestData, 0, sizeof(_sRunningQuestPacketData)*_MAX_CURQUEST_NUM_ );
			}
		}
		break;	
	case 0 :	// 초기화
		{
			// 퀘스트가 취소되서 다시 재정렬하는 동안을 알 수 있도록 플래그를 켠다.
			g_QuestScriptManager.m_bCancelProcess = TRUE;

			if( questresult->uiMoney >= 0 )
				g_pLocalUser->m_Money = questresult->uiMoney;
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("=== MSG_NO_QUEST_SET_RESULT : uiMoney [%d]", questresult->uiMoney);
#endif
			
			for(int i = 0 ; i < 5 ; ++i)
			{
				if(questresult->SkillDelete[i].sSkillID != 0)
				{
					if(questresult->SkillDelete[i].number == -1)
					{
						g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(questresult->SkillDelete[i].sSkillID)].m_sSkillID = 0;
						g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(questresult->SkillDelete[i].sSkillID)].m_cSkillLevel = 0;
						g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(questresult->SkillDelete[i].sSkillID)].m_iSkillExp = 0;
						g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(questresult->SkillDelete[i].sSkillID)].m_cCurMission = 0;
						g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(questresult->SkillDelete[i].sSkillID)].m_ucSkillState = 0;
											}
				}
				
#ifdef _XDWDEBUG	
				_XDWINPRINT("=== MSG_NO_QUEST_SET_RESULT : SkillID [%d]   number [%d]", questresult->SkillDelete[i].sSkillID, questresult->SkillDelete[i].number);
#endif
			}

			if(g_pSkill_Window)
			{
				g_pSkill_Window->SetSkillButton();
			}

			if(g_pQuickSlot_Window)
			{
				for(int i = 0 ; i < 5 ; ++i)
				{
					if(questresult->SkillDelete[i].sSkillID > 0 && questresult->SkillDelete[i].number == -1)
						g_pQuickSlot_Window->DeleteQuickSlot(questresult->SkillDelete[i].sSkillID);
				}
			}
			//퀘스트 타이머 초기화
			if( g_QuestScriptManager.m_bStartQuestTimer )
			{
				if( g_QuestScriptManager.m_TimerStartQuestID == questresult->usQuestID )
				{
					g_QuestScriptManager.m_TimerStartQuestID	= -1;
					g_QuestScriptManager.m_QuestTimerStartTime	= 0;
					g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
					g_QuestScriptManager.m_bStartQuestTimer		= FALSE;
				}

			}
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("=== MSG_NO_QUEST_SET_RESULT : ucNum [%d]", questresult->ucNum);
#endif
			
			for(i = 0 ; i < questresult->ucNum ; ++i)
			{
				if(questresult->ItemDelete[i].slot >= 0 && questresult->ItemDelete[i].slot < _XINVENTORY_SLOTTYPE_TOTALCOUNT) //Author : 양희왕 //breif: 아이템 삭제
				{
					if(questresult->ItemDelete[i].number == 0)
					{
						g_pLocalUser->m_UserItemList[questresult->ItemDelete[i].slot].Reset();
					}
					else if(questresult->ItemDelete[i].number > 0)
					{
						g_pLocalUser->m_UserItemList[questresult->ItemDelete[i].slot].Set_m_ucCount(questresult->ItemDelete[i].number);
					}

					if(questresult->ItemDelete[i].slot >= _XINVENTORY_SLOTTYPE_AMOR && questresult->ItemDelete[i].slot <= _XINVENTORY_SLOTTYPE_WEAPON_2)
					{
						DWORD lparam = questresult->ItemDelete[i].slot * 100000 + g_pLocalUser->m_UserItemList[questresult->ItemDelete[i].slot].Get_m_sID();
						ChangeModel(lparam, g_pLocalUser->m_UserItemList[questresult->ItemDelete[i].slot].Get_m_cType());
					}
					
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
				
#ifdef _XDWDEBUG	
				_XDWINPRINT("=== MSG_NO_QUEST_SET_RESULT : ItemDelete[%d].slot [%d]  ItemDelete[%d].number [%d]", i, questresult->ItemDelete[i].slot, i, questresult->ItemDelete[i].number);
#endif
			}

			// 취소된 퀘스트에 특별한 설정이 있다면 되돌린다. NPC 생성 삭제 및 아이템 고정
			g_QuestScriptManager.QuestCancleHistorySetting(questresult->usQuestID);

			// 제룡행은 m_mapQuestScriptList에서, 나머지는 m_mapSubQuestScriptList에서.
			_XQuestScriptItem* currentquest = NULL;
			if( questresult->usQuestID >= 10000 && questresult->usQuestID < 20000 )
			{
				currentquest = g_QuestScriptManager.m_mapQuestScriptList[questresult->usQuestID];
			}
			else
			{
				currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[questresult->usQuestID];
			}

			// 저장되지 않는 퀘스트가 취소되면 따로 처리한다.
			if( currentquest )
			{
				if( currentquest->GetPartyQuest() ) g_pLocalUser->m_bPartyQuestProgress = FALSE;

				if( currentquest->GetTempQuest() )
				{
					// 저장되지 않는 퀘스트를 취소한다.
					if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
					{		
						list <int>::iterator iter_questnosavetype;		
						iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin();
						while( iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() )
						{
							int questid = *iter_questnosavetype;
							if( questid == questresult->usQuestID )
							{
								// 무분류는 알려주지 않는다.
								if( currentquest->GetQuestID() >=10000 )
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_QUEST_2382), 
										_XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
								}					
								
								iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.erase(iter_questnosavetype);
								
								currentquest->Reset();
								// 저장되지 않는 퀘스트는 다시 퀘스트 시작조건 리스트에 들어간다.
								g_QuestScriptManager.SetQuestStartCondition(currentquest->GetQuestID());
								
								// 다른 퀘스트의 시작 조건을 세팅한다.
								g_QuestScriptManager.ResetQuestStartCondition();
								g_QuestScriptManager.QuestStartConditionZoneMode();
								g_QuestScriptManager.QuestStartConditionLevelMode();
								g_QuestScriptManager.QuestStartConditionWearItemMode();
								
								// 저장 되지 않는 퀘스트는 퀘스트가 없어진다.
								if( g_pQuest_Window && g_pQuest_Window->GetShowStatus() )
								{
									if( g_pQuest_Window->m_SelectedQuestID == currentquest->GetQuestID() ) 
										g_pQuest_Window->m_SelectedItemIndex = -1;
									g_pQuest_Window->SetQuestTabButton();
									g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
								}		
								break;								
							}
							else
							{
								iter_questnosavetype++;
							}
						}
					}
				}
				else
				{
					
					for( i = 0; i < ID_QUEST_SYNC-1; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
					{
						if( g_QuestScriptManager.m_ProgressSubQuestID[i] == questresult->usQuestID)
						{
							g_QuestScriptManager.m_ProgressSubQuestID[i] = -1;
							g_QuestScriptManager.DecreaseProgressQuestCount();
							break;
						}
					}		
					
					g_QuestScriptManager.ResetQuest(questresult->usQuestID);		
					if( questresult->usQuestID >= 10000 && questresult->usQuestID < 20000 )
					{
						g_QuestScriptManager.m_CurrentMainQuestID = questresult->usQuestID;
						g_QuestScriptManager.SetCheckQuestID(questresult->usQuestID);
						g_QuestScriptManager.SetCheckQuest(TRUE);	
						
						g_QuestScriptManager.QuestRunningSetting( questresult->usQuestID, 0 );
						g_QuestScriptManager.m_bProgressMainQuest = FALSE;
						
						g_QuestScriptManager.SetMainQuestStartCondition(questresult->usQuestID);
						
						//퀘스트시작조건이 레벨베이스이면 체크를 한다.
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find( questresult->usQuestID );
						if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
						{		
							_XQuestScriptItem* currentquest = iter_quest->second;
							if(currentquest)
							{				
								if(currentquest->GetQuestStartConditionMode() == _XQUESTSTART_LEVEL)
								{
									g_QuestScriptManager.QuestStartConditionZoneMode();
									g_QuestScriptManager.QuestStartConditionLevelMode();
								}
							}
						}
						
						if( !g_QuestScriptManager.m_bReceiveQuestData )
						{
							SendPacket( MSG_NO_INVEN_REQ );
							SendPacket( MSG_NO_SKILL_INFO_REQ );
							g_QuestScriptManager.m_bQuestStartConditionCheck = TRUE;
							g_QuestScriptManager.m_bReceiveQuestData = TRUE;
							g_QuestScriptManager.m_bQuestValidCheck = FALSE;
							memset( m_RunningQuestData, 0, sizeof(_sRunningQuestPacketData)*_MAX_CURQUEST_NUM_ );
						}
						
					}
					else
					{
						if( g_QuestScriptManager.m_bReceiveQuestData )
						{					
							//시작조건 리스트 아무곳에나 넣고 시작 조건에 있는 리스트들을 재정렬 한다.
							g_QuestScriptManager.m_listQuestStartTypeZone.push_back(questresult->usQuestID);//아무 곳에나 넣음
							g_QuestScriptManager.ResetQuestStartCondition();//리스트들을 재정렬한다.
							g_QuestScriptManager.QuestStartConditionZoneMode();
							g_QuestScriptManager.QuestStartConditionLevelMode();
							
							if( questresult->usQuestID >= 10000 )
							{
								if( questresult->usQuestID < 29921 || questresult->usQuestID > 29936 )
								{
									_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
									pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORK_CANCELQUEST),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
									_XPlayInterfaceSound(ID_SR_INTERFACE_QUESTFAIL01_WAV);	
								}	
							}	
						}
						else
						{
							g_QuestScriptManager.QuestStartConditionSort();
							g_QuestScriptManager.QuestStartConditionZoneMode();
							g_QuestScriptManager.QuestStartConditionLevelMode();
							
							SendPacket( MSG_NO_INVEN_REQ );
							SendPacket( MSG_NO_SKILL_INFO_REQ );
							g_QuestScriptManager.m_bQuestStartConditionCheck = TRUE;
							g_QuestScriptManager.m_bReceiveQuestData = TRUE;
							g_QuestScriptManager.m_bQuestValidCheck = FALSE;
							memset( m_RunningQuestData, 0, sizeof(_sRunningQuestPacketData)*_MAX_CURQUEST_NUM_ );
						}
					}
					
					
					if( g_pQuest_Window )
					{
						if( g_pQuest_Window->GetShowStatus() )
						{
							if( g_pQuest_Window->m_SelectedQuestID == questresult->usQuestID ) 
								g_pQuest_Window->m_SelectedItemIndex = -1;
							g_pQuest_Window->SetQuestTabButton();
							g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
						}
					}
				}//else    !currentquest->GetTempQuest() 
			}//if( currentquest )
			
			
			g_QuestScriptManager.Process();
			// 퀘스트가 재정렬을 끝냈음을 알린다.
			g_QuestScriptManager.m_bCancelProcess = FALSE;
		}
		break;
		
	case _QUEST_COMPLETE_FAIL_ :	// 실패 완료
	case _QUEST_CANCEL_FAIL_ :	// 퀘스트 취소 -> 리셋 불가
		{
			QuestSetResultFailComplete(questresult->usQuestID);

			if( g_pQuest_Window )
			{
				if( g_pQuest_Window->GetShowStatus() )
				{
					g_pQuest_Window->SetQuestTabButton();
					g_pQuest_Window->ChangeTab( g_pQuest_Window->m_SelectedTab);
					g_pQuest_Window->ProcessSelectedListBox();	
				}
			}
		}
		break;
	}
}

void _XNetwork::QuestItemReceive(MSG_QUESTITEM_RECEIVE* questitem)
{
#ifdef _XDEF_PERIODITEM_20061027	
	#ifdef _XDWDEBUG	
	_XDWINPRINT("RECEIVE QUEST ITEM =======================================");
	_XDWINPRINT("inven[%d]  Type[%d]  ID[%d] ", questitem->userItem.cSlot, questitem->userItem.cType, questitem->userItem.sID );
	#endif
	
	DWORD additemcount = 0;
	if( questitem->userItem.cSlot != 0 )
	{
		//돈이 들어 왔을때 	
		if(questitem->userItem.cSlot == _XINVENTORY_SLOTTYPE_MONEYINDEX)
		{
			if( questitem->userItem.Item_Money.S_MoneyItem.uiCount>= 0 )
			{
				if( g_pLocalUser->m_Money <= questitem->userItem.Item_Money.S_MoneyItem.uiCount )
				{
					additemcount = questitem->userItem.Item_Money.S_MoneyItem.uiCount - g_pLocalUser->m_Money;
					//_XLog("QUEST Get Money : currentmoney[%u], servermoney[%u] addmoney[%u]", g_pLocalUser->m_Money, questitem->userItem.Item_Money.S_MoneyItem.uiCount, additemcount);	
					g_pLocalUser->m_Money = questitem->userItem.Item_Money.S_MoneyItem.uiCount;
					
					if( g_QuestScriptManager.m_bReceiveQuestData )
					{
						TCHAR noticestring[256];
						memset(noticestring, 0, sizeof(noticestring));
						_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_1767), additemcount);
						InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);	
					}		
				}
				else
				{
					additemcount = g_pLocalUser->m_Money - questitem->userItem.Item_Money.S_MoneyItem.uiCount;
					//_XLog("QUEST Get Money : currentmoney[%u], servermoney[%u] addmoney[%u]", g_pLocalUser->m_Money, questitem->userItem.Item_Money.S_MoneyItem.uiCount, additemcount);	
					g_pLocalUser->m_Money = questitem->userItem.Item_Money.S_MoneyItem.uiCount;
					
					if( g_QuestScriptManager.m_bReceiveQuestData )
					{
						TCHAR noticestring[256];
						memset(noticestring, 0, sizeof(noticestring));
						_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_1768), additemcount);
						InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);		
					}		
				}
				return;
			}
		}

		if( questitem->userItem.sID != 0 )
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_024 && questitem->userItem.cType == _XGI_FC_BOOK )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_024);
					g_TutorialInfo->_XTutorial_024 = TRUE;
	#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
	#endif
				}
			}	
			// 일반 Inventory로 삽입
			int inventoryindex = questitem->userItem.cSlot;
			int currentitemcount = g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount();
			g_pLocalUser->m_UserItemList[inventoryindex].Reset();

			if(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_cType() == questitem->userItem.cType && 
				g_pLocalUser->m_UserItemList[inventoryindex].Get_m_sID() == questitem->userItem.sID)
			{
				if(g_pLocalUser->m_UserItemList[inventoryindex].IsStackable())
				{
					
					additemcount = questitem->userItem.Item_Money.S_NormalItem.ucCount - currentitemcount;
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(questitem->userItem.Item_Money.S_NormalItem.ucCount);
				}
				else
				{
					additemcount = questitem->userItem.Item_Money.S_NormalItem.ucCount - currentitemcount;
					
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cType (questitem->userItem.cType);
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sID( questitem->userItem.sID );
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(questitem->userItem.Item_Money.S_NormalItem.ucCount);
					
					// Second Type setting 
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cSecond(_XGameItem::GetItemSecond(questitem->userItem.cType, questitem->userItem.sID));

					
					// 내구력 setting
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usMaxDur( 
						_XGameItem::GetMaxDurability(questitem->userItem.cType, questitem->userItem.sID) );
					
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usCurDur( questitem->userItem.Item_Money.S_NormalItem.usCurDur );
					
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucSocketCount( questitem->userItem.Item_Money.S_NormalItem.ucSocketCount );
					if( questitem->userItem.Item_Money.S_NormalItem.ucSocketCount > 0 )
					{
						for( int i = 0; i < 4; ++i )
							g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sSocket( i, questitem->userItem.sSocket[i] );
					}

//Author : 양희왕 //breif : 2007년 10월 24일 추가					
#ifdef _XDEF_SMELT_ITEM 
					//breif : 08/03/04 수정..
					if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_cType()) )
					{
						
						_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_cType());
						int nCurMaxValue = _XGameItem::GetNature( tempWeaponItem[questitem->userItem.sID].cType, 
																  tempWeaponItem[questitem->userItem.sID].sID);

						g_pLocalUser->m_UserItemList[inventoryindex].m_usCurProtect  = 0;
						g_pLocalUser->m_UserItemList[inventoryindex].m_usCurMaxProtect = nCurMaxValue;
						g_pLocalUser->m_UserItemList[inventoryindex].m_usUsedProtect = 0; //0이면 사용가능
					}
					//g_pLocalUser->m_UserItemList[inventoryindex].m_usCurProtect  = questitem->userItem.uselimit.usCurProtect; // 바른용린의수
					//g_pLocalUser->m_UserItemList[inventoryindex].m_usCurMaxProtect = questitem->userItem.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
					//g_pLocalUser->m_UserItemList[inventoryindex].m_usUsedProtect = questitem->userItem.uselimit.usUsedProtect; // 용루 사용여부
#endif
// 여기까지

					g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag1 = questitem->userItem.uselimit.ucFlag1;
	#ifndef	_XDEF_SMELT_ITEM //Author : 양희왕 //breif : 디파인 되어 들어오지 않는다
					g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag2	= questitem->userItem.uselimit.ucFlag2;	
					g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag3	= questitem->userItem.uselimit.ucFlag3;
	#endif
					g_pLocalUser->m_UserItemList[inventoryindex].m_usValue1	= questitem->userItem.uselimit.usValue1;

					if( questitem->userItem.uselimit.ucYear > 0 )
					{						
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucYear(questitem->userItem.uselimit.ucYear);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMonth(questitem->userItem.uselimit.ucMonth);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucDay(questitem->userItem.uselimit.ucDay);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucHour(questitem->userItem.uselimit.ucHour);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMinute(questitem->userItem.uselimit.ucMinute);
					}
				}
			}
			else
			{
				additemcount = questitem->userItem.Item_Money.S_NormalItem.ucCount - currentitemcount;

				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cType(questitem->userItem.cType);
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sID( questitem->userItem.sID );
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(questitem->userItem.Item_Money.S_NormalItem.ucCount);
				
				// Second Type setting 
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cSecond(_XGameItem::GetItemSecond(questitem->userItem.cType, questitem->userItem.sID));
				
				// 내구력 setting
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usMaxDur(
					_XGameItem::GetMaxDurability(questitem->userItem.cType, questitem->userItem.sID) );

				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usCurDur( questitem->userItem.Item_Money.S_NormalItem.usCurDur );
		
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucSocketCount( questitem->userItem.Item_Money.S_NormalItem.ucSocketCount );
				if( questitem->userItem.Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sSocket( i, questitem->userItem.sSocket[i] );
				}

//Author : 양희왕 //breif : 2007년 10월 24일 추가					
#ifdef _XDEF_SMELT_ITEM 
				//breif : 08/03/04 수정..
				if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_cType()) )
				{
					
					_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_cType());
					int nCurMaxValue = _XGameItem::GetNature( tempWeaponItem[questitem->userItem.sID].cType, 
															  tempWeaponItem[questitem->userItem.sID].sID);

					g_pLocalUser->m_UserItemList[inventoryindex].m_usCurProtect  = 0;
					g_pLocalUser->m_UserItemList[inventoryindex].m_usCurMaxProtect = nCurMaxValue;
					g_pLocalUser->m_UserItemList[inventoryindex].m_usUsedProtect = 0; //0이면 사용가능
				}
				//g_pLocalUser->m_UserItemList[inventoryindex].m_usCurProtect  = questitem->userItem.uselimit.usCurProtect; // 바른용린의수
				//g_pLocalUser->m_UserItemList[inventoryindex].m_usCurMaxProtect = questitem->userItem.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
				//g_pLocalUser->m_UserItemList[inventoryindex].m_usUsedProtect = questitem->userItem.uselimit.usUsedProtect; // 용루 사용여부
#endif
// 여기까지

				g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag1 = questitem->userItem.uselimit.ucFlag1;
	#ifndef _XDEF_SMELT_ITEM //Auhtor : 양희왕 //breif 디파인되어 들어오지 않는다 
				g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag2	= questitem->userItem.uselimit.ucFlag2;	
				g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag3	= questitem->userItem.uselimit.ucFlag3;
	#endif
				g_pLocalUser->m_UserItemList[inventoryindex].m_usValue1	= questitem->userItem.uselimit.usValue1;

				if( questitem->userItem.uselimit.ucYear > 0 )
				{					
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucYear(questitem->userItem.uselimit.ucYear);	
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMonth(questitem->userItem.uselimit.ucMonth);	
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucDay(questitem->userItem.uselimit.ucDay);
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucHour(questitem->userItem.uselimit.ucHour);
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMinute(questitem->userItem.uselimit.ucMinute);
				}
			}
		}
	}

	// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.
	if( g_pQuest_Window )
	{
		if( g_pQuest_Window->GetShowStatus() )
		{
			if( g_pQuest_Window->m_SelectedQuestID == questitem->usQuestID )
			{				
				g_pQuest_Window->ParseQuestDescString();
			}
		}
	}

	// 아이템 카운터에 관련된 아이템이 들어오는 경우 갱신시킨다.	
	#ifdef _XTS_NEWPARTYCOUNTER
	g_QuestScriptManager.CheckItemCounter(questitem->usQuestID,questitem->userItem.cType,questitem->userItem.sID);
	#else
	_XQuestScriptItem* currentquest = NULL;
	
	if( questitem->usQuestID >= 10000 && questitem->usQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questitem->usQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questitem->usQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if( currentquest )
	{
		for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
		{
			if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType != -1 )
			{
				if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == questitem->userItem.cType 
					&& currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID == questitem->userItem.sID )
				{
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(buffer));
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].m_cType == questitem->userItem.cType
							&& g_pLocalUser->m_UserItemList[i].m_sID == questitem->userItem.sID)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;														
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].m_cType == questitem->userItem.cType
							&& g_pLocalUser->m_UserItemList[i].m_sID == questitem->userItem.sID)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;
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
							// add word order
							_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), 
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
								currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
								currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
						}
						
						if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									delete g_QuestScriptManager.m_lpszQuestSystemMessage;
									g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}	
						else if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint == 0 )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									delete g_QuestScriptManager.m_lpszQuestSystemMessage;
									g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}
					}
					
					break;
				}
			}
		}
	}
	
	// 재료수집 창에 켜져 있을 때는 실시간으로 재료 현황을 갱신한다.
	if( g_pLifeSkill_Window && g_pLifeSkill_Window->GetShowStatus() )
	{
		if(g_pLifeSkill_Window->m_SelectedTab == _XLIFETAB_LIFECLASS2)
			g_pLifeSkill_Window->SetListResourceNeedItem(FALSE);
	}
	#endif

	if( g_QuestScriptManager.m_bReceiveQuestData )
	{
		LPTSTR param1 = _XGameItem::GetItemName(questitem->userItem.cType, questitem->userItem.sID);
		InsertChatString( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_GETSTATUS, param1, &additemcount ),
						  _XDEF_CHATMESSAGECOLOR_SYSTEMGET);		
	}
	
#else //_XDEF_PERIODITEM_20061027
	#ifdef _XDWDEBUG	
	_XDWINPRINT("RECEIVE QUEST ITEM =======================================");
	_XDWINPRINT("inven[%d]  Type[%d]  ID[%d]  count[%d]", questitem->cInvenIndex, questitem->cItemType, questitem->cItemID, questitem->iCount);
	#endif
	
	DWORD additemcount = 0;
	if(questitem->cInvenIndex == _XINVENTORY_SLOTTYPE_MONEYINDEX)
	{
		if( questitem->iCount >= 0 )
		{
			if( g_pLocalUser->m_Money <= questitem->iCount )
			{
				additemcount = questitem->iCount - g_pLocalUser->m_Money;
				//_XLog("QUEST Get Money : currentmoney[%u], servermoney[%u] addmoney[%u]", g_pLocalUser->m_Money, questitem->iCount, additemcount);	
				g_pLocalUser->m_Money = questitem->iCount;
				
				if( g_QuestScriptManager.m_bReceiveQuestData )
				{
					TCHAR noticestring[256];
					memset(noticestring, 0, sizeof(noticestring));
					_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_1767), additemcount);
					InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);	
				}		
			}
			else
			{
				additemcount = g_pLocalUser->m_Money - questitem->iCount;
				//_XLog("QUEST Get Money : currentmoney[%u], servermoney[%u] addmoney[%u]", g_pLocalUser->m_Money, questitem->iCount, additemcount);	
				g_pLocalUser->m_Money = questitem->iCount;
				
				if( g_QuestScriptManager.m_bReceiveQuestData )
				{
					TCHAR noticestring[256];
					memset(noticestring, 0, sizeof(noticestring));
					_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_1768), additemcount);
					InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);		
				}		
			}
			return;
		}
	}
	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_024 && questitem->cItemType == _XGI_FC_BOOK )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_024);
			g_TutorialInfo->_XTutorial_024 = TRUE;
	#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
	#endif
		}
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=
	
	// 일반 Inventory로 삽입
	int currentitemcount = g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_ucCount;
	g_pLocalUser->m_UserItemList[questitem->cInvenIndex].Reset();
	
	if(g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_cType == questitem->cItemType && 
		g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_sID == questitem->cItemID)
	{
		if(g_pLocalUser->m_UserItemList[questitem->cInvenIndex].IsStackable())
		{
			additemcount = questitem->iCount - currentitemcount;
			g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_ucCount = questitem->iCount;
		}
		else
		{
			additemcount = questitem->iCount - currentitemcount;
			
			g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_cType = questitem->cItemType;
			g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_sID = questitem->cItemID;
			g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_ucCount = questitem->iCount;
			
			// Second Type setting 
			g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_cSecond = 
				_XGameItem::GetItemSecond(questitem->cItemType, questitem->cItemID);
			
			// 내구력 setting
			g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_usMaxDur = 
				g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_usCurDur = 
				_XGameItem::GetMaxDurability(questitem->cItemType, questitem->cItemID);
		}
	}
	else
	{
		additemcount = questitem->iCount - currentitemcount;
		
		g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_cType = questitem->cItemType;
		g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_sID = questitem->cItemID;
		g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_ucCount = questitem->iCount;
		
		// Second Type setting 
		g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_cSecond = 
			_XGameItem::GetItemSecond(questitem->cItemType, questitem->cItemID);
		
		// 내구력 setting
		g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_usMaxDur = 
			g_pLocalUser->m_UserItemList[questitem->cInvenIndex].m_usCurDur = 
			_XGameItem::GetMaxDurability(questitem->cItemType, questitem->cItemID);			

	}
	// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.
	if( g_pQuest_Window )
	{
		if( g_pQuest_Window->GetShowStatus() )
		{
			if( g_pQuest_Window->m_SelectedQuestID == questitem->usQuestID )
			{				
				g_pQuest_Window->ParseQuestDescString();
			}
		}
	}

	// 아이템 카운터에 관련된 아이템이 들어오는 경우 갱신시킨다.	
	#ifdef _XTS_NEWPARTYCOUNTER
	g_QuestScriptManager.CheckItemCounter(questitem->usQuestID,questitem->cItemType,questitem->cItemID);
	#else
	_XQuestScriptItem* currentquest = NULL;
	
	if( questitem->usQuestID >= 10000 && questitem->usQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questitem->usQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questitem->usQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if( currentquest )
	{
		for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
		{
			if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType != -1 )
			{
				if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == questitem->cItemType 
					&& currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID == questitem->cItemID )
				{
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(buffer));
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].m_cType == questitem->cItemType
							&& g_pLocalUser->m_UserItemList[i].m_sID == questitem->cItemID)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;														
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].m_cType == questitem->cItemType
							&& g_pLocalUser->m_UserItemList[i].m_sID == questitem->cItemID)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;
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
							// add word order
							_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), 
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
								currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
								currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
						}
						
						if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									SAFE_DELETE(g_QuestScriptManager.m_lpszQuestSystemMessage);
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}	
						else if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint == 0 )
						{
							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									SAFE_DELETE(g_QuestScriptManager.m_lpszQuestSystemMessage); 
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}
					}
					
					break;
				}
			}
		}
	}
	
	// 재료수집 창에 켜져 있을 때는 실시간으로 재료 현황을 갱신한다.
	if( g_pLifeSkill_Window && g_pLifeSkill_Window->GetShowStatus() )
	{
		if(g_pLifeSkill_Window->m_SelectedTab == _XLIFETAB_LIFECLASS2)
			g_pLifeSkill_Window->SetListResourceNeedItem(FALSE);
	}
	#endif


	if( g_QuestScriptManager.m_bReceiveQuestData )
	{
		LPTSTR param1 = _XGameItem::GetItemName(questitem->cItemType, questitem->cItemID);
		InsertChatString( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_GETSTATUS, param1, &additemcount ),
						  _XDEF_CHATMESSAGECOLOR_SYSTEMGET);		
	}
#endif
	
	return;
}

void _XNetwork::QuestItemRemove(MSG_QUESTITEM_REMOVE* questitem)
{
#ifdef _XDWDEBUG	
	_XDWINPRINT("REMOVE QUEST ITEM =======================================");
#endif
	//_XDWINPRINT("inven[%d]  count[%d]", questitem->cInvenIndex, questitem->iCount);
	
	if( questitem->cSlotNum == -1 ) return;
	for( int i = 0; i < questitem->cSlotNum; ++i )
	{
		if( questitem->ResItem[i].number == 0 )
		{
			if(questitem->ResItem[i].slot >= _XINVENTORY_SLOTTYPE_AMOR && questitem->ResItem[i].slot <= _XINVENTORY_SLOTTYPE_WEAPON_2)
			{
				// 아이템에 있던 상태효과 제거
//				AddEffectFromItem(false, g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].m_cType, 
//					g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].m_sID, questitem->ResItem[i].slot);
				
				DWORD lparam = questitem->ResItem[i].slot * 100000 + 0;		// 아이템 벗기기
				ChangeModel(lparam, 0);

				// 퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].Get_m_cType(), g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].Get_m_sID() );
				
			}
			else if( questitem->ResItem[i].slot >= _XINVENTORY_SLOTTYPE_ACCESSORY_1 &&  questitem->ResItem[i].slot <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 )
			{				
				// 빼는 것이 악세사리일때  퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
				// 퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].Get_m_cType(), g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].Get_m_sID() );
				
			}
			
			// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
			g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].Get_m_cType(), g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].Get_m_sID() );
			
			g_pLocalUser->m_UserItemList[ questitem->ResItem[i].slot ].Reset();

			g_pLocalUser->UpdateLocalUserAttackLevel();
		}
		else
		{
			g_pLocalUser->m_UserItemList[ questitem->ResItem[i].slot ].Set_m_ucCount(questitem->ResItem[i].number);
		}		
	}
	
	if( g_QuestScriptManager.m_bReceiveQuestData )
	{
		InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORK_REMOVEQUESTITEM), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
}

void _XNetwork::QuestLearnSkill(MSG_LEARN_SKILL* questmartial)
{
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_sSkillID = questmartial->sSkillID;
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_cSkillLevel = questmartial->ucSkillLev;
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_iSkillExp = 0;
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_cCurMission = 0;
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_ucSkillState = questmartial->ucSkillState;
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_cSelectedSkillLevel = questmartial->ucSkillLev;
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_cOnOff = 0;
	g_pLocalUser->m_SkillList[questmartial->cSlotNo].m_dwCooldownStartTime = 0;
	
	TCHAR noticestring[256];
	memset(noticestring, 0, sizeof(noticestring));
	_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_LEARNMARTIAL), g_SkillProperty[questmartial->sSkillID]->skillName); // // 무공 %s(을)를 배웠습니다.
	
	if( g_QuestScriptManager.m_bReceiveQuestData )
	{
		InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
	}
	
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

void _XNetwork::QuestForgetSkill(MSG_FORGET_SKILL* questmartial)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID == questmartial->sSkillID)
		{
			g_pLocalUser->m_SkillList[i].m_sSkillID = 0;
			g_pLocalUser->m_SkillList[i].m_cSkillLevel = 0;
			g_pLocalUser->m_SkillList[i].m_cCurMission = 0;
			g_pLocalUser->m_SkillList[i].m_iSkillExp = 0;
			g_pLocalUser->m_SkillList[i].m_ucSkillState = 0;
			
			TCHAR noticestring[256];
			memset(noticestring, 0, sizeof(noticestring));
			_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_REMOVEMARTIAL), g_SkillProperty[questmartial->sSkillID]->skillName);
			
			if( g_QuestScriptManager.m_bReceiveQuestData )
			{
				InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}

			
			if(g_pSkill_Window)
			{
				g_pSkill_Window->SetSkillButton();
			}

			if(g_pQuickSlot_Window)
			{
				g_pQuickSlot_Window->DeleteQuickSlot(questmartial->sSkillID);
			}

			if(g_pMasterySkill_Window)
			{
				g_pMasterySkill_Window->ParseMasterySkill();
				g_pMasterySkill_Window->CheckDependency();
			}
			
			return;
		}
	}
}

void _XNetwork::QuestUpdateStatus(MSG_UPDATE_STATUS* queststatus)
{
	TCHAR statusname[128];
	memset(statusname, 0, sizeof(statusname));

	switch(queststatus->cStatusType)
	{
	case ID_JINEXP :
		{
			int increase = queststatus->uiUpdatedValue - g_pLocalUser->m_CharacterInfo.Get_jinlevel();
#ifdef _XDWDEBUG
			_XLog("QUEST REWARD EXP : increase [%d] totalvalue [%d] currentvalue [%d]", increase, queststatus->uiUpdatedValue, g_pLocalUser->m_CharacterInfo.Get_jinlevel());
#endif
			g_pLocalUser->m_CharacterInfo.Set_jinlevel( queststatus->uiUpdatedValue );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_1769));
			
			if(g_pJinStatus_Window)
			{
				if(g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience > 0)
				{
					if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
					{
						if(g_pLocalUser->m_CharacterInfo.Get_sublevel() < 12)
						{
							g_NetworkKernel.SendPacket(MSG_NO_LEVEL_UP_REQ);
						}
						else
						{
							g_pJinStatus_Window->SetButtonStatus(TRUE);
						}
					}
					else
					{
						g_pJinStatus_Window->SetButtonStatus(FALSE);
					}
				}
				
				g_pJinStatus_Window->SetParameter();
			}
			if( g_QuestScriptManager.m_bReceiveQuestData )
			{
				TCHAR noticestring[256];

				strcpy( noticestring,  g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKQUEST_2383, statusname, &increase ) );
				InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
			}		
		}
		break;
	case ID_VITALITY :
		{
			g_pLocalUser->m_CharacterInfo.Set_constitution( queststatus->uiUpdatedValue );
			g_pLocalUser->m_Constitution.Set_org( g_pLocalUser->m_CharacterInfo.Get_constitution() );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTHENERGY));
		}
		break;
	case ID_ZEN :
		{
			g_pLocalUser->m_CharacterInfo.Set_zen( queststatus->uiUpdatedValue );
			g_pLocalUser->m_Zen.Set_org( g_pLocalUser->m_CharacterInfo.Get_zen() );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_CINFO_INNERENERGY));
		}
		break;
	case ID_SOUL :
		{
			g_pLocalUser->m_CharacterInfo.Set_intelligence( queststatus->uiUpdatedValue );
			g_pLocalUser->m_Intelligence.Set_org( g_pLocalUser->m_CharacterInfo.Get_intelligence() );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_CINFO_HEART));
		}
		break;
	case ID_HEART :
		{
			g_pLocalUser->m_CharacterInfo.Set_dexterity( queststatus->uiUpdatedValue );
			g_pLocalUser->m_Dex.Set_org( g_pLocalUser->m_CharacterInfo.Get_dexterity() );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_CINFO_DEX));
		}
		break;
	case ID_STAMINA :
		{
			g_pLocalUser->m_CharacterInfo.Set_strength( queststatus->uiUpdatedValue );
			g_pLocalUser->m_Strength.Set_org( g_pLocalUser->m_CharacterInfo.Get_strength() );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_CINFO_STR));
		}
		break;
	case ID_VITALPOWER :
		{
			g_pLocalUser->m_CharacterInfo.Set_current_lifepower( queststatus->uiUpdatedValue );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_NETWORK_HEALTHENERGY));
		}
		break;
	case ID_INNERPOWER :
		{
			g_pLocalUser->m_CharacterInfo.Set_current_forcepower( queststatus->uiUpdatedValue );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_NETWORK_INNERENERGY));
		}
		break;
	case ID_SOULPOWER :
		{
			g_pLocalUser->m_CharacterInfo.Set_current_concentrationpower( queststatus->uiUpdatedValue );
			_snprintf(statusname,sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_NETWORK_SOULENERGY));
		}
		break;
	case ID_HEARTPOWER :
		{
		}
	case ID_STAMINAPOWER :
		{
		}
		break;
	case ID_HONOR :
		{
			g_pLocalUser->m_CharacterInfo.Set_honor( queststatus->uiUpdatedValue );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_NETWORK_REPUTATION));
		}
		break;
	case ID_FAME :
		{
#ifdef _XTS_FAME
			g_pLocalUser->m_CharacterInfo.Set_Notoriety( queststatus->uiUpdatedValue );
			_snprintf(statusname, sizeof(statusname), _XGETINTERFACETEXT(ID_STRING_NEW_2636));//_T("악명")
#endif
			//g_pLocalUser->m_CharacterInfo.retribution = queststatus->uiUpdatedValue;
			//sprintf(statusname, _XGETINTERFACETEXT(ID_STRING_NETWORK_RETRIBUTION));
		}
		break;
		
	default :
		return;
	}
	
	if( g_QuestScriptManager.m_bReceiveQuestData )
	{
		if( queststatus->cStatusType != ID_JINEXP)
		{
			InsertChatString( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_DOSOMETHING, statusname, &queststatus->uiUpdatedValue ), // %s이(가) %d(이)가 되었습니다.
							 _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
		}
	}

}

void _XNetwork::QuestJoinGroup(MSG_QUEST_JOIN_GROUP* questjoingroup)
{
	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_026 )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_026);
			g_TutorialInfo->_XTutorial_026 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
		}
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=

	TCHAR groupname[64];
	memset(groupname, 0, sizeof(groupname));

	switch(questjoingroup->cGroup)
	{
	case _XGROUP_NANGIN :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_NANGIN;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00));
		}
		break;
	case _XGROUP_GAEBANG :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_GAEBANG;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01));
		}
		break;
	case _XGROUP_BEEGOONG :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_BEEGOONG;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02));
		}
		break;
	case _XGROUP_SORIM :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_SORIM;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03));

			// 소림은 빠박이므로 머리를 다시 세팅한다.
			g_pLocalUser->m_ModelDescriptor.SetMeshModel( g_pLocalUser->m_CharacterInfo.modeldescript, 
														  g_pLocalUser->m_CharacterInfo.gender, 
														  g_pLocalUser->m_CharacterInfo.groupindex );
		}
		break;
	case _XGROUP_NOCKRIM :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_NOCKRIM;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04));
		}
		break;
	case _XGROUP_MOODANG :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_MOODANG;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05));
		}
		break;
	case _XGROUP_MAKYO :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_MAKYO;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06));
		}
		break;
	case _XGROUP_SEGA :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_SEGA;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07));
		}
		break;
	case _XGROUP_NOFIXED :
		{
			g_pLocalUser->m_CharacterInfo.groupindex = _XGROUP_NOFIXED;
			_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08));
		}
		break;
		
	default :
		return;
	}

	if(g_pSkill_Window)
		g_pSkill_Window->SetSkillButton();

	_XWindow_NPCSkill* pNPCSkill_Window = (_XWindow_NPCSkill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSKILL);
	if(pNPCSkill_Window)
	{
		if(pNPCSkill_Window->GetShowStatus())
			pNPCSkill_Window->SetListItem();
	}
	
	if( g_QuestScriptManager.m_bReceiveQuestData )
	{
		TCHAR noticestring[256];
		memset(noticestring, 0, sizeof(noticestring));
		_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_JOINGROUP), groupname);
		InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
	}

}

void _XNetwork::QuestInventoryFullProcess(MSG_QUEST_AUTH_RESULT* questresult)
{
	BOOL	invenfull = TRUE;
	int		needinvencount = questresult->cNeedInven;
	int		emptyslotcount = 0;
		
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
	if(invenfull)
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

	if( invenfull )
	{
		BOOL needitemremove = FALSE;
		int practicablequestnode = g_QuestScriptManager.CheckPracticableQuestNode( questresult->usQuestID, questresult->cNodeID, needitemremove );
		if( practicablequestnode != -1)// 비정상적인 퀘스트 -> 백이 필요함 
		{		
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_1770),questresult->cNeedInven);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE); //, _XDEF_NPCSCRIPT_CLOSE);
			pMessageBox->StartTimer(7000, TRUE) ;

			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
			if(pNPCScript_Window) 
			{
				pNPCScript_Window->SetMessageType(_XDEF_NPCMESSAGE_QUEST);
				pNPCScript_Window->SetHaveQuestID(questresult->usQuestID);
				pNPCScript_Window->m_btnNPCDialogModExit->ShowWindow(TRUE);
				pNPCScript_Window->m_btnBackToNPCMode->ShowWindow(FALSE);
			}
			
			g_QuestScriptManager.m_bQuestValidCheck = TRUE;
			if( practicablequestnode == 0 )
			{
				SendPacket(MSG_NO_QUEST_USER_CANCEL, questresult->usQuestID, questresult->cNodeID);
				if( questresult->usQuestID >= 10000 && questresult->usQuestID < 20000 )
				{
					g_QuestScriptManager.m_bProgressMainQuest = FALSE;
				}
			}
			else
			{
				SendPacket( MSG_NO_QUEST_VALIDCHECK_INGAME, questresult->usQuestID, practicablequestnode, needitemremove );
			}
		}
		else// 정상적인 퀘스트 
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKQUEST_1770),questresult->cNeedInven);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE); //, _XDEF_NPCSCRIPT_CLOSE);
			pMessageBox->StartTimer(7000, TRUE) ;

			// NPC대화 화면일 경우에는 대화모드를 나갈 수 있는 버튼을 생성한다.
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
			if(pNPCScript_Window) 
			{
				if( pNPCScript_Window->GetShowStatus() )
				{
					pNPCScript_Window->m_btnQuestOK->ShowWindow(TRUE);
				}
			}
			
			g_QuestScriptManager.InsertRewordQuestList(questresult->usQuestID, questresult->cNodeID, questresult->cNeedInven);
		}
	}
}

void _XNetwork::QuestSCONResultStartCondition(int questid, char nexttreenumber)
{
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetServerEnable(TRUE);
				currentquest->SetStartTime(g_ServerTimeCode);
				currentquest->SetCurrentTreeNumber( nexttreenumber );
				currentquest->RemoveQuestStartConditionList();
				
				currentquest->m_lpszQuestProgressinformation = currentquest->m_startquestinformationmessage;

				if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
				
				g_QuestScriptManager.SetCheckQuestID(0);
				g_QuestScriptManager.SetCheckQuest(FALSE);
				g_QuestScriptManager.SetStartMessage(questid);
								
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_STARTQUEST),  _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
				_XPlayInterfaceSound(ID_SR_INTERFACE_QUESTFIND01_WAV);
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetServerEnable(TRUE);
				currentquest->SetStartTime(g_ServerTimeCode);
				currentquest->SetCurrentTreeNumber( nexttreenumber );
				currentquest->RemoveQuestStartConditionList();
				
				currentquest->m_lpszQuestProgressinformation = currentquest->m_startquestinformationmessage;

				if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();

				g_QuestScriptManager.SetStartMessage(questid);
				
				if( questid >= 20000 )
				{
					_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
					pNPCScript_Window->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_STARTQUEST),  _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
					_XPlayInterfaceSound(ID_SR_INTERFACE_QUESTFIND01_WAV);
				}
			}
		}
	}
}

void _XNetwork::QuestSCONResultContinueTriggerNormalAction(int questid, char nexttreenumber, char currenttreenumber, BOOL jumpaction, BOOL bautonode)
{	
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				
				currentquest->SetProgressTrigger(currenttreenumber, TRUE);
				currentquest->SetTriggerPlayActionAndServerEnable(currenttreenumber, TRUE, TRUE, jumpaction);	
				currentquest->SetNextTreeNumber( nexttreenumber );
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				if( !currentquest->GetPartyQuest() )
				{
					currentquest->SetProgressTrigger(currenttreenumber, TRUE);
					currentquest->SetTriggerPlayActionAndServerEnable(currenttreenumber, TRUE, TRUE, jumpaction);	
					currentquest->SetNextTreeNumber( nexttreenumber );
				}
				else // 파티퀘스트 
				{
					// 파티 퀘스트에서 자기가 다시 자신으로 돌아오는 경우에는 특별한 처리를 한다
					// 다른 사람이 조건식을 만족해서 자동으로 넘어가는 경우
					// 강제로 현재 노드를 세팅해주어야 한다. 
					if( !currentquest->m_bSendClientPacketAtPartyQuest || bautonode)
					{
						currentquest->SetCurrentTreeNumber( currenttreenumber, FALSE );						
					}
					else
					{
						currentquest->m_bSendClientPacketAtPartyQuest = FALSE;
					}
					
					currentquest->SetProgressTrigger(currenttreenumber, TRUE);
					currentquest->SetTriggerPlayActionAndServerEnable(currenttreenumber, TRUE, TRUE, jumpaction);	
					
					map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(currentquest->GetCurrentTreeNumber());
					if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
					{
						_XTriggerTreeItem* currenttree = iter_triggertree->second;
						if(currenttree)
						{
							if( currenttree->m_TriggerUniqueID )
							{
								map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
								_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
								if(currentsuccesstrigger)
								{
									//행동식이 비어있으면 행동식을 실행하지 않는다.
									if( !currentsuccesstrigger->m_mapNormalActionList.empty() )
									{
										int actioncount = currentsuccesstrigger->m_mapNormalActionList.size();
										bool checksuccess = FALSE;
										map <int, _XTriggerActionItem*>::iterator iter_action;
										_XTriggerActionItem* currentaction = NULL;
										for(iter_action = currentsuccesstrigger->m_mapNormalActionList.begin() ; iter_action != currentsuccesstrigger->m_mapNormalActionList.end() ; ++iter_action)
										{
											currentaction = iter_action->second;
											if(currentaction)
											{
												for( int i = 0; i < actioncount; ++i )
												{
													if( currentsuccesstrigger->m_SuccessActionid[i] == currentaction->GetActionID() )
													{
														checksuccess = TRUE;
														break;
													}
													
												}
												
												if( !checksuccess )
												{
													if(currentaction->Process() == QUEST_ACTION_RESULT_SUCCESS)
													{		
														for( int i = 0; i < 20; ++i )
														{
															if( currentsuccesstrigger->m_SuccessActionid[i] == -1 )
															{
																currentsuccesstrigger->m_SuccessActionid[i] = currentaction->GetActionID();	
																break;
															}
														}						
													}
												}		
											}
										}
									}
								}
							}	
						}
					}
					currentquest->SetNextTreeNumber( nexttreenumber );					
				}
				
			}
		}
	}
	// Reword list에 있으면 삭제
	map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(questid);
	if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
	{
		g_QuestScriptManager.DeleteRewordQuestList(questid);
	}
}

// 분기에서 실패시에 예외 행동식을 실행 
void _XNetwork::QuestSCONResultContinueTriggerExceptionAction(int questid, char nexttreenumber, char currenttreenumber, BOOL jumpaction, BOOL bautonode)
{	
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				
				currentquest->SetProgressTrigger(currenttreenumber, FALSE);
				currentquest->SetTriggerPlayActionAndServerEnable(currenttreenumber, TRUE, FALSE, jumpaction);	
				currentquest->SetNextTreeNumber( nexttreenumber );
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				if( !currentquest->GetPartyQuest() )
				{
					currentquest->SetProgressTrigger(currenttreenumber, FALSE);
					currentquest->SetTriggerPlayActionAndServerEnable(currenttreenumber, TRUE, FALSE, jumpaction);	
					currentquest->SetNextTreeNumber( nexttreenumber );
				}
				else
				{
					// 다른 사람이 조건식을 만족해서 자동으로 넘어가는 경우
					// 강제로 현재 노드를 세팅해주어야 한다. 
					if( !currentquest->m_bSendClientPacketAtPartyQuest || bautonode)
					{
						currentquest->SetCurrentTreeNumber( currenttreenumber, FALSE );
					}
					else
					{
						currentquest->m_bSendClientPacketAtPartyQuest = FALSE;
					}
					
					currentquest->SetProgressTrigger(currenttreenumber, FALSE);
					currentquest->SetTriggerPlayActionAndServerEnable(currenttreenumber, TRUE, FALSE, jumpaction);
					
					map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(currentquest->GetCurrentTreeNumber());
					if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
					{
						_XTriggerTreeItem* currenttree = iter_triggertree->second;
						if(currenttree)
						{
							if( currenttree->m_TriggerUniqueID )
							{
								map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
								_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
								if(currentsuccesstrigger)
								{
									//행동식이 비어있으면 행동식을 실행하지 않는다.
									if( !currentsuccesstrigger->m_mapExceptedActionList.empty() )
									{
										int actioncount = currentsuccesstrigger->m_mapExceptedActionList.size();
										bool checksuccess = FALSE;
										map <int, _XTriggerActionItem*>::iterator iter_action;
										_XTriggerActionItem* currentaction = NULL;
										for(iter_action = currentsuccesstrigger->m_mapExceptedActionList.begin() ; iter_action != currentsuccesstrigger->m_mapExceptedActionList.end() ; ++iter_action)
										{
											currentaction = iter_action->second;
											if(currentaction)
											{
												for( int i = 0; i < actioncount; ++i )
												{
													if( currentsuccesstrigger->m_SuccessActionid[i] == currentaction->GetActionID() )
													{
														checksuccess = TRUE;
														break;
													}
													
												}
												
												if( !checksuccess )
												{
													if(currentaction->Process() == QUEST_ACTION_RESULT_SUCCESS)
													{		
														for( int i = 0; i < 20; ++i )
														{
															if( currentsuccesstrigger->m_SuccessActionid[i] == -1 )
															{
																currentsuccesstrigger->m_SuccessActionid[i] = currentaction->GetActionID();	
																break;
															}
														}							
													}
												}		
											}
										}
									}
								}
							}	
						}
					}
					currentquest->SetNextTreeNumber( nexttreenumber );

					// 파티 퀘스트에서 자기가 다시 자신으로 돌아오는 경우에는 특별한 처리를 한다
//					if( currenttreenumber == nexttreenumber )
//					{
//						map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(currenttreenumber);
//						if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
//						{
//							_XTriggerTreeItem* currenttree = iter_triggertree->second;
//							if(currenttree)
//							{
//								map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
//								_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
//								if(currentsuccesstrigger)
//								{
//									currentsuccesstrigger->SetClientEnable(FALSE);
//									_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", nexttreenumber);
//									
//								}
//							}
//						}
//					}

				}
				
			}
		}
	}
	// Reword list에 있으면 삭제
	map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(questid);
	if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
	{
		g_QuestScriptManager.DeleteRewordQuestList(questid);
	}
}

void _XNetwork::QuestSCONResultSuccessComplete(int questid, char currenttreenumber)
{
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetProgressTrigger(currenttreenumber, TRUE);
				currentquest->SetTriggerServerEnable(currenttreenumber, TRUE );
				currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_SOLVE);
				currentquest->ReSetQuestMark( );
				currentquest->SetFirstStartCondition( FALSE );	
				
				currentquest->m_lpszQuestProgressinformation = currentquest->m_lpszOutlineMessage2;
				
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetProgressTrigger(currenttreenumber, TRUE);
				currentquest->SetTriggerServerEnable(currenttreenumber, TRUE );
				currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_SOLVE);
				currentquest->ReSetQuestMark( );
				currentquest->SetFirstStartCondition( FALSE );	
				
				currentquest->m_lpszQuestProgressinformation = currentquest->m_lpszOutlineMessage2;
			}
		}
	}

	// Reword list에 있으면 삭제
	map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(questid);
	if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
	{
		g_QuestScriptManager.DeleteRewordQuestList(questid);
	}
}

void _XNetwork::QuestSCONResultFailComplete(int questid, char currenttreenumber)
{
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetProgressTrigger(currenttreenumber, FALSE);
				currentquest->SetTriggerServerEnable(currenttreenumber, TRUE );
				currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_MISS);
				currentquest->ReSetQuestMark( );
				currentquest->SetFirstStartCondition( FALSE );
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetProgressTrigger(currenttreenumber, FALSE);
				currentquest->SetTriggerServerEnable(currenttreenumber, TRUE );
				currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_MISS);
				currentquest->ReSetQuestMark( );
				currentquest->SetFirstStartCondition( FALSE );

			}
		}
	}

	// Reword list에 있으면 삭제
	map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(questid);
	if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
	{
		g_QuestScriptManager.DeleteRewordQuestList(questid);
	}
}

void _XNetwork::QuestFCONResultFailComplete(int questid, char currenttreenumber)
{
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetProgressTrigger(currenttreenumber, FALSE);
				currentquest->SetTriggerServerEnable(currenttreenumber, TRUE );
				currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_MISS);
				currentquest->SetWaitingFailResult(FALSE);
				
				g_QuestScriptManager.SetFailMessage(questid);
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->SetProgressTrigger(currenttreenumber, FALSE);
				currentquest->SetTriggerServerEnable(currenttreenumber, TRUE );
				currentquest->SetQuestServerResult(QUEST_COMPLETE_RESULT_MISS);
				currentquest->SetWaitingFailResult(FALSE);
				
				g_QuestScriptManager.SetFailMessage(questid);
			}
		}
	}
}

void _XNetwork::QuestSetResultFailComplete(int questid)
{	
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->ReSetQuestMark();
				currentquest->SetQuestCompleteResult(QUEST_COMPLETE_RESULT_MISS);
				currentquest->SetComplete(TRUE);
				g_QuestScriptManager.InsertQuestCompleteList(currentquest);
				
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILQUEST), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE ); // 풍운록 수행에 실패하였습니다.
				_XPlayInterfaceSound(ID_SR_INTERFACE_QUESTFAIL01_WAV);
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = NULL;
			currentquest = iter_quest->second;
			if(currentquest)
			{
				currentquest->ReSetQuestMark();
				currentquest->SetQuestCompleteResult(QUEST_COMPLETE_RESULT_MISS);
				currentquest->SetComplete(TRUE);
				g_QuestScriptManager.InsertQuestCompleteList(currentquest);
				
				if( questid >= 20000 )
				{
					InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILQUEST), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE ); // 풍운록 수행에 실패하였습니다.
					_XPlayInterfaceSound(ID_SR_INTERFACE_QUESTFAIL01_WAV);
				}
			}
		}
	}
}

void _XNetwork::QuestAndSuccessTriggerClientEnableReset(int questid, char currenttreenumber)
{
	if( questid >= 10000 && questid < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(currenttreenumber);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{
						map <int, _XTriggerItem*>::iterator iter_trigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						if(iter_trigger != currentquest->m_mapSuccessTriggerList.end())
						{
							_XTriggerItem* currenttrigger = iter_trigger->second;
							if(currenttrigger)
							{
								currenttrigger->SetClientEnable(FALSE);
#ifdef _XDWDEBUG
								_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", currenttree->m_TriggerUniqueID);
#endif
							}
						}
					}
				}
			}
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			_XQuestScriptItem* currentquest = iter_quest->second;
			if(currentquest)
			{
				map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find(currenttreenumber);
				if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
				{
					_XTriggerTreeItem* currenttree = iter_triggertree->second;
					if(currenttree)
					{
						map <int, _XTriggerItem*>::iterator iter_trigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
						if(iter_trigger != currentquest->m_mapSuccessTriggerList.end())
						{
							_XTriggerItem* currenttrigger = iter_trigger->second;
							if(currenttrigger)
							{
								currenttrigger->SetClientEnable(FALSE);
								_XLog("Quest Trigger Condition Check Open : TriggerID[%d]", currenttree->m_TriggerUniqueID);
							}
						}
					}
				}
			}
		}
	}
}

void _XNetwork::MSGQuestCounterResult(MSG_QUEST_SETCOUNT_RESULT* questcount)
{
	_XQuestScriptItem* currentquest = NULL;
	if( questcount->usQuestId >= 10000 && questcount->usQuestId < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questcount->usQuestId);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			currentquest = iter_quest->second;
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questcount->usQuestId);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			currentquest = iter_quest->second;				
		}
	}

	if(currentquest)
	{
		currentquest->m_CounterInformation[questcount->cCounterNo].CountCurrentPoint = questcount->ucSum;
		
		
		TCHAR	buffer[256];
		memset(buffer, 0, sizeof(buffer));
		if( currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint >= 1 )
		{
			if(currentquest->m_CounterInformation[questcount->cCounterNo].CounterTargetName[0] != 0 )
			{				
				if( currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint > currentquest->m_CounterInformation[questcount->cCounterNo].CountCurrentPoint)
				{
					_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
						currentquest->m_CounterInformation[questcount->cCounterNo].CounterTargetName,
						currentquest->m_CounterInformation[questcount->cCounterNo].CountCurrentPoint,
						currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint );
					
					if( g_pQuest_Window )
					{
						if( g_pQuest_Window->GetShowStatus() )
						{
							if( g_pQuest_Window->m_SelectedQuestID == questcount->usQuestId  )
							{
								g_pQuest_Window->ParseQuestDescString();	
							}
						}
					}
				}
				else if( currentquest->m_CounterInformation[questcount->cCounterNo].CountCurrentPoint == currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint )
				{
					// add word order

					_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),
						currentquest->m_CounterInformation[questcount->cCounterNo].CounterTargetName,
						currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint,
						currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint );
					
					if( g_pQuest_Window )
					{
						if( g_pQuest_Window->GetShowStatus() )
						{
							if( g_pQuest_Window->m_SelectedQuestID == questcount->usQuestId  )
							{
								g_pQuest_Window->ParseQuestDescString();	
							}
						}
					}
				}	
				
				if( currentquest->m_CounterInformation[questcount->cCounterNo].CountCurrentPoint <= currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint )
				{
					if( buffer[0] != 0 )
					{
						int strlength = strlen(buffer);
						
						if(g_QuestScriptManager.m_lpszQuestSystemMessage )
						{
							delete g_QuestScriptManager.m_lpszQuestSystemMessage;
							g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
						}
						
						if(strlength > 0)
						{
							g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
							g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
							strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
						}
						
						g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
						g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
					}				
				}
			}							
		}
		else if( currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint == 0 )
		{		
			_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
				currentquest->m_CounterInformation[questcount->cCounterNo].CounterTargetName,
				currentquest->m_CounterInformation[questcount->cCounterNo].CountCurrentPoint );
			
			if( g_pQuest_Window )
			{
				if( g_pQuest_Window->GetShowStatus() )
				{
					if( g_pQuest_Window->m_SelectedQuestID == questcount->usQuestId  )
					{
						g_pQuest_Window->ParseQuestDescString();	
					}
				}
			}			
		}// else if( currentquest->m_CounterInformation[questcount->cCounterNo].CountTotalPoint == 0 )		
	}	
	

}

void _XNetwork::MSGQuestCharacterPointReset(MSG_QUEST_REDISTRIBUTION* questcharterpointreset)
{
	g_pLocalUser->m_CharacterInfo.Set_constitution(8);// 건강 <= 생기
	g_pLocalUser->m_CharacterInfo.Set_zen(8);// 진기 <= 그대로 
	g_pLocalUser->m_CharacterInfo.Set_intelligence(8);// 지혜 <= 영기 
	g_pLocalUser->m_CharacterInfo.Set_dexterity(8);// 민첩 <= 심기 
	g_pLocalUser->m_CharacterInfo.Set_strength(8);// 근력 <= 정기
	g_pLocalUser->m_CharacterInfo.Set_leftpoint( questcharterpointreset->sLeftPoint ); // 남은 포인트 
}	

void _XNetwork::MSGQuestValidCheckResult(MSG_QUEST_VALIDCHECK_RESULT* questcheck)
{
	
	if( !g_QuestScriptManager.m_bQuestStartConditionCheck )
	{
		if( questcheck->usQuestID >= 10000 && questcheck->usQuestID < 20000)// 제룡행 
		{			
			g_QuestScriptManager.m_CurrentMainQuestID = questcheck->usQuestID;
			g_QuestScriptManager.SetCheckQuestID(questcheck->usQuestID);
			g_QuestScriptManager.SetCheckQuest(TRUE);			
			g_QuestScriptManager.QuestRunningSetting( questcheck->usQuestID, questcheck->cNodeID );
			g_QuestScriptManager.m_bProgressMainQuest = TRUE;
			
		}
		else
		{
			g_QuestScriptManager.QuestRunningSetting(questcheck->usQuestID, questcheck->cNodeID);
			
			for( int j = 0; j < ID_QUEST_SYNC-1; ++j ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
			{
				if( g_QuestScriptManager.m_ProgressSubQuestID[j] == -1)
				{
					g_QuestScriptManager.m_ProgressSubQuestID[j] = questcheck->usQuestID;
					g_QuestScriptManager.IncreaseProgressQuestCount();
					break;
				}
			}		
			
			
			g_QuestScriptManager.QuestStartConditionSort();
			g_QuestScriptManager.QuestStartConditionZoneMode();
			g_QuestScriptManager.QuestStartConditionLevelMode();
			g_pQuest_Window->SetQuestTabButton();
		}
		SendPacket( MSG_NO_INVEN_REQ );
		SendPacket( MSG_NO_SKILL_INFO_REQ );
		g_QuestScriptManager.m_bQuestStartConditionCheck = TRUE;
		g_QuestScriptManager.m_bReceiveQuestData = TRUE;
		g_QuestScriptManager.m_bQuestValidCheck = FALSE;
		memset( m_RunningQuestData, 0, sizeof(_sRunningQuestPacketData)*_MAX_CURQUEST_NUM_ );
	}
	else// 게임 진행중에 강제로 노드를 뒤로 돌릴 때는 처음에 게임에 들어왔을 때와는 다른 처리가 필요하다.
	{
		if( g_QuestScriptManager.m_bReceiveQuestData )
		{
			if( g_QuestScriptManager.m_bQuestValidCheck )
			{
				g_QuestScriptManager.QuestRunningSetting(questcheck->usQuestID, questcheck->cNodeID);
				g_QuestScriptManager.m_bQuestValidCheck = FALSE;
			}
		}
	}
}	


void _XNetwork::MSGQuestValidCheckInGameResult(MSG_QUEST_VALIDCHECK_INGAME_RESULT* questcheck)
{
	if( g_QuestScriptManager.m_bQuestValidCheck )
	{
		
		if( questcheck->usQuestID >= 10000 && questcheck->usQuestID < 20000 )
		{
			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questcheck->usQuestID);
			if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			{
				_XQuestScriptItem* currentquest = NULL;
				currentquest = iter_quest->second;
				if(currentquest)
				{
					currentquest->SetCurrentTreeNumber( questcheck->cNodeID );
				}
			}
		}
		else
		{
			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questcheck->usQuestID);
			if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			{
				_XQuestScriptItem* currentquest = NULL;
				currentquest = iter_quest->second;
				if(currentquest)
				{
					currentquest->SetCurrentTreeNumber( questcheck->cNodeID );	
				}
			}
		}
		// Reword list에 있으면 삭제
		map <int, _XQUEST_INVENFULL>::iterator iter_rewordquest = g_QuestScriptManager.m_mapRewordQuestList.find(questcheck->usQuestID);
		if(iter_rewordquest != g_QuestScriptManager.m_mapRewordQuestList.end())
		{
			g_QuestScriptManager.DeleteRewordQuestList(questcheck->usQuestID);
		}
		g_QuestScriptManager.m_bQuestValidCheck = FALSE;
	}
}	


void _XNetwork::MSGQuestNodeBackItemResult(MSG_QUEST_NODEBACK_ITEM_RESULT* questcheck)
{
	if( questcheck->uiMoney >= 0 ) 
		g_pLocalUser->m_Money = questcheck->uiMoney;

	g_pLocalUser->m_CharacterInfo.Set_constitution( questcheck->contribution );
	
	// 무공을 삭제한다.	
	for( int i = 0 ; i < 5 ; ++i)
	{
		if( questcheck->removeskill[i].sSkillID != 0 )
		{
			if( questcheck->removeskill[i].number != 0 )
			{
				for(int j = 0 ; j < _XDEF_MAX_LEARNSKILL ; ++i)
				{
					if(g_pLocalUser->m_SkillList[j].m_sSkillID == questcheck->removeskill[i].sSkillID)
					{
						g_pLocalUser->m_SkillList[j].m_sSkillID = 0;
						g_pLocalUser->m_SkillList[j].m_cSkillLevel = 0;
						g_pLocalUser->m_SkillList[j].m_cCurMission = 0;
						g_pLocalUser->m_SkillList[j].m_iSkillExp = 0;
						g_pLocalUser->m_SkillList[j].m_ucSkillState = 0;
						
						TCHAR noticestring[256];
						memset(noticestring, 0, sizeof(noticestring));
						_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_REMOVEMARTIAL), g_SkillProperty[questcheck->removeskill[i].sSkillID]->skillName);
						InsertChatString( noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
											
						if(g_pQuickSlot_Window)
						{
							g_pQuickSlot_Window->DeleteQuickSlot(questcheck->removeskill[i].sSkillID);
						}						
						break;
					}
				}
			}
		}
	}

	// 무공을 추가한다.
//	for(int i = 0; i < 5; i++)
//	{
//	}


	//아이템을 삭제한다.
	for( i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT ; ++i)
	{
		if( questcheck->removeitem[i].slot >= 0 && questcheck->removeitem[i].slot < _XINVENTORY_SLOTTYPE_TOTALCOUNT)
		{
			if( questcheck->removeitem[i].number == 0 )
			{
				if(questcheck->removeitem[i].slot >= _XINVENTORY_SLOTTYPE_AMOR && questcheck->removeitem[i].slot <= _XINVENTORY_SLOTTYPE_WEAPON_2)
				{
					// 아이템에 있던 상태효과 제거
					//				AddEffectFromItem(false, g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].m_cType, 
					//					g_pLocalUser->m_UserItemList[questitem->ResItem[i].slot].m_sID, questitem->ResItem[i].slot);
					
					DWORD lparam = questcheck->removeitem[i].slot * 100000 + 0;		// 아이템 벗기기
					ChangeModel(lparam, 0);
				}
				
				g_pLocalUser->m_UserItemList[ questcheck->removeitem[i].slot ].Reset();	
			}
			else
			{
				g_pLocalUser->m_UserItemList[ questcheck->removeitem[i].slot ].Set_m_ucCount(questcheck->removeitem[i].number);
			}
		}
	}

	//아이템을 추가한다.
	for( i = 0; i < 10; ++i)
	{
		if( questcheck->additem[i].cSlot != 0 )
		{
			if( questcheck->additem[i].sID != 0 )
			{
				// 일반 Inventory로 삽입
				int inventoryindex = questcheck->additem[i].cSlot;
				if(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_cType() == questcheck->additem[i].cType && 
					g_pLocalUser->m_UserItemList[inventoryindex].Get_m_sID() == questcheck->additem[i].sID)
				{
					if(g_pLocalUser->m_UserItemList[inventoryindex].IsStackable())
					{
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + questcheck->additem[i].Item_Money.S_NormalItem.ucCount);
					}
					else
					{
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cType(questcheck->additem[i].cType);
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sID( questcheck->additem[i].sID );
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(questcheck->additem[i].Item_Money.S_NormalItem.ucCount);
						
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usMaxDur( _XGameItem::GetMaxDurability(questcheck->additem[i].cType, questcheck->additem[i].sID) );
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usCurDur( questcheck->additem[i].Item_Money.S_NormalItem.usCurDur );

						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucSocketCount( questcheck->additem[i].Item_Money.S_NormalItem.ucSocketCount );
						if( questcheck->additem[i].Item_Money.S_NormalItem.ucSocketCount > 0 )
						{
							for( int i = 0; i < 4; ++i )
								g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sSocket( i, questcheck->additem[i].sSocket[i] );
						}
						g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag1 = questcheck->additem[i].uselimit.ucFlag1;
#ifndef _XDEF_SMELT_ITEM //Auhtor : 양희왕 //breif 디파인되어 들어오지 않는다 
						g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag2	= questcheck->additem[i].uselimit.ucFlag2;	
						g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag3	= questcheck->additem[i].uselimit.ucFlag3;
#endif
						g_pLocalUser->m_UserItemList[inventoryindex].m_usValue1	= questcheck->additem[i].uselimit.usValue1;

						if( questcheck->additem[i].uselimit.ucYear > 0 )
						{							
							g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucYear(questcheck->additem[i].uselimit.ucYear);	
							g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMonth(questcheck->additem[i].uselimit.ucMonth);	
							g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucDay(questcheck->additem[i].uselimit.ucDay);	
							g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucHour(questcheck->additem[i].uselimit.ucHour);	
							g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMinute(questcheck->additem[i].uselimit.ucMinute);
						}
					}
				}
				else
				{
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cType(questcheck->additem[i].cType);
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sID( questcheck->additem[i].sID );
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(questcheck->additem[i].Item_Money.S_NormalItem.ucCount);

					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usMaxDur( _XGameItem::GetMaxDurability(questcheck->additem[i].cType, questcheck->additem[i].sID) );
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usCurDur( questcheck->additem[i].Item_Money.S_NormalItem.usCurDur );

					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucSocketCount( questcheck->additem[i].Item_Money.S_NormalItem.ucSocketCount );
					if( questcheck->additem[i].Item_Money.S_NormalItem.ucSocketCount > 0 )
					{
						for( int i = 0; i < 4; ++i )
							g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sSocket( i, questcheck->additem[i].sSocket[i] );
					}
					g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag1 = questcheck->additem[i].uselimit.ucFlag1;
#ifndef _XDEF_SMELT_ITEM //Auhtor : 양희왕 //breif 디파인되어 들어오지 않는다 
					g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag2	= questcheck->additem[i].uselimit.ucFlag2;	
					g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag3	= questcheck->additem[i].uselimit.ucFlag3;
#endif
					g_pLocalUser->m_UserItemList[inventoryindex].m_usValue1	= questcheck->additem[i].uselimit.usValue1;

					if( questcheck->additem[i].uselimit.ucYear > 0 )
					{						
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucYear(questcheck->additem[i].uselimit.ucYear);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMonth(questcheck->additem[i].uselimit.ucMonth);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucDay(questcheck->additem[i].uselimit.ucDay);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucHour(questcheck->additem[i].uselimit.ucHour);	
						g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMinute(questcheck->additem[i].uselimit.ucMinute);
					}
				}
			}
		}
	}


	
	g_pLocalUser->UpdateLocalUserAttackLevel();
	// 아이템 카운터에 관련된 아이템이 들어오는 경우 갱신시킨다.
	
	if( g_pQuest_Window )
	{
		if( g_pQuest_Window->GetShowStatus() )
		{
			g_pQuest_Window->ParseQuestDescString();
		}
	}
	
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

void _XNetwork::MSGPartyQuestCounterResult(MSG_QUEST_PARTY_SETCOUNT_RESULT* partyquestcount)
{
	g_pLocalUser->m_TotalCountOfPartyQuestCounter = 0;
	int atypecount = 0;
	int btypecount = 0;
	int ctypecount = 0;
	int dtypecount = 0;
	
	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
	{		
		if(g_pLocalUser->m_PartyInfo[i].isValid )
		{
			atypecount += partyquestcount->_sPCounter[i].usPersnal_Counter[0];
			btypecount += partyquestcount->_sPCounter[i].usPersnal_Counter[1];
			ctypecount += partyquestcount->_sPCounter[i].usPersnal_Counter[2];
			dtypecount += partyquestcount->_sPCounter[i].usPersnal_Counter[3];
			g_pLocalUser->m_PartyInfo[i].partycount = partyquestcount->_sPCounter[i].AllCounterAdd();
		}
		// MVP를 초기화 시킨다.
		if( g_pLocalUser->m_PartyInfo[i].isMVP ) g_pLocalUser->m_PartyInfo[i].isMVP = FALSE;
		
		// 파티 전체의 총 카운터를 구한다.
		g_pLocalUser->m_TotalCountOfPartyQuestCounter += partyquestcount->_sPCounter[i].AllCounterAdd();
	}

	// 파티 퀘스트에서 MVP를 세팅한다.
	g_pLocalUser->m_PartyInfo[partyquestcount->cMvp].isMVP = TRUE;

	_XQuestScriptItem* currentquest = NULL;
	if( partyquestcount->usQuestId >= 10000 && partyquestcount->usQuestId < 20000 )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(partyquestcount->usQuestId);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			currentquest = iter_quest->second;
		}
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(partyquestcount->usQuestId);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			currentquest = iter_quest->second;				
		}
	}				
	if(currentquest)
	{				
		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
		{		
			currentquest->m_PartyCounterInformation[0].CounterPointPerMember[i] = 0;
			currentquest->m_PartyCounterInformation[1].CounterPointPerMember[i] = 0;
			currentquest->m_PartyCounterInformation[2].CounterPointPerMember[i] = 0;
			currentquest->m_PartyCounterInformation[3].CounterPointPerMember[i] = 0;
			
			if(g_pLocalUser->m_PartyInfo[i].isValid )
			{
				currentquest->m_PartyCounterInformation[0].CounterPointPerMember[i] = partyquestcount->_sPCounter[i].usPersnal_Counter[0];
				currentquest->m_PartyCounterInformation[1].CounterPointPerMember[i] = partyquestcount->_sPCounter[i].usPersnal_Counter[1];
				currentquest->m_PartyCounterInformation[2].CounterPointPerMember[i] = partyquestcount->_sPCounter[i].usPersnal_Counter[2];
				currentquest->m_PartyCounterInformation[3].CounterPointPerMember[i] = partyquestcount->_sPCounter[i].usPersnal_Counter[3];
			}
		}
		
		int changetype = -1;
		if( currentquest->m_PartyCounterInformation[0].CountCurrentPoint != atypecount )
		{
			currentquest->m_PartyCounterInformation[0].CountCurrentPoint = atypecount;
			changetype = 0;
		}
		else if( currentquest->m_PartyCounterInformation[1].CountCurrentPoint != btypecount )
		{
			currentquest->m_PartyCounterInformation[1].CountCurrentPoint = btypecount;
			changetype = 1;
		}
		else if( currentquest->m_PartyCounterInformation[2].CountCurrentPoint != ctypecount )
		{
			currentquest->m_PartyCounterInformation[2].CountCurrentPoint = ctypecount;
			changetype = 2;
		}
		else if( currentquest->m_PartyCounterInformation[3].CountCurrentPoint != dtypecount )
		{
			currentquest->m_PartyCounterInformation[3].CountCurrentPoint = dtypecount;
			changetype = 3;
		}
		
		if( changetype != -1 )
		{
			TCHAR	buffer[256];
			memset(buffer, 0, sizeof(TCHAR)*256);
			if( currentquest->m_PartyCounterInformation[changetype].CountTotalPoint >= 1 )
			{
				if(currentquest->m_PartyCounterInformation[changetype].CounterTargetName[0] != 0 )
				{				
					if( currentquest->m_PartyCounterInformation[changetype].CountTotalPoint > currentquest->m_PartyCounterInformation[changetype].CountCurrentPoint)
					{
						_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
							currentquest->m_PartyCounterInformation[changetype].CounterTargetName,
							currentquest->m_PartyCounterInformation[changetype].CountCurrentPoint,
							currentquest->m_PartyCounterInformation[changetype].CountTotalPoint );
						
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								if( g_pQuest_Window->m_SelectedQuestID == partyquestcount->usQuestId  )
								{
									g_pQuest_Window->ParseQuestDescString();	
								}
							}
						}
					}
					else if( currentquest->m_PartyCounterInformation[changetype].CountCurrentPoint == currentquest->m_PartyCounterInformation[changetype].CountTotalPoint )
					{
						// add word order
						
						_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),
							currentquest->m_PartyCounterInformation[changetype].CounterTargetName,
							currentquest->m_PartyCounterInformation[changetype].CountTotalPoint,
							currentquest->m_PartyCounterInformation[changetype].CountTotalPoint );
						
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								if( g_pQuest_Window->m_SelectedQuestID == partyquestcount->usQuestId  )
								{
									g_pQuest_Window->ParseQuestDescString();	
								}
							}
						}
					}	
					
					if( currentquest->m_PartyCounterInformation[changetype].CountCurrentPoint <= currentquest->m_PartyCounterInformation[changetype].CountTotalPoint )
					{
						if( buffer[0] != 0 )
						{
							int strlength = strlen(buffer);
							
							if(g_QuestScriptManager.m_lpszQuestSystemMessage )
							{
								delete g_QuestScriptManager.m_lpszQuestSystemMessage;
								g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
							}
							
							if(strlength > 0)
							{
								g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
								g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
								strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
							}
							
							g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
							g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
						}				
					}
				}							
			}
			else if( currentquest->m_PartyCounterInformation[changetype].CountTotalPoint == 0 )
			{		
				_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
					currentquest->m_PartyCounterInformation[changetype].CounterTargetName,
					currentquest->m_PartyCounterInformation[changetype].CountCurrentPoint );
				
				if( buffer[0] != 0 )
				{
					int strlength = strlen(buffer);
					
					if(g_QuestScriptManager.m_lpszQuestSystemMessage )
					{
						delete g_QuestScriptManager.m_lpszQuestSystemMessage;
						g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
					}
					
					if(strlength > 0)
					{
						g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
						g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
						strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
					}
					
					g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
					g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
				}				
				
				if( g_pQuest_Window )
				{
					if( g_pQuest_Window->GetShowStatus() )
					{
						if( g_pQuest_Window->m_SelectedQuestID == partyquestcount->usQuestId  )
						{
							g_pQuest_Window->ParseQuestDescString();	
						}
					}
				}			
			}// else if( currentquest->m_PartyCounterInformation[questcount->cCounterNo].CountTotalPoint == 0 )	
		}
			
	}
			
}

void _XNetwork::MSGQuestRankingListResult(MSG_QUESTRANK_RES* questrankinglist)
{
	_XWindow_QuestRank* pQuestRank_Window = (_XWindow_QuestRank*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTRANKWINDOW);
	if(pQuestRank_Window)
	{
		if( pQuestRank_Window->GetRankQuestID() == questrankinglist->usQuestID )
		{
			pQuestRank_Window->SetListItem(&questrankinglist->Data);
		}
		else
		{
			pQuestRank_Window->ShowWindow(FALSE);
		}
	}
	
}
