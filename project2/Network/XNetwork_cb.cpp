// XNetwork_cb.cpp : 장원전 관련
////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_ThreatenGauge.h"
#include "XWindowObjectDefine.h"
#include "Xwindow_CastleInfo.h"
#include "XWindow_ChallengerList.h"
#include "XWindow_CastleBattleTimeChange.h"
#include "XWindow_CastleBattleShout.h"
#include "XWindow_CastleBattlePersonalHistory.h"
#include "XWindow_CBResultNotice.h"
#include "Xwindow_CBRecord.h"
#include "XWindow_Rebirth.h"
#include "XWindow_CBRewardSmall.h"
#include "XWindow_CastleBattleRewardList.h"
#include "XWindow_CBOrgRecord.h"
#include "XMessageWindow.h"
#include "XWindow_Info.h"

extern void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo );

// Send ============

BOOL _XNetwork::SendMSGCBOwnerReqScheduleList(int type, int param1, int param2, int param3, int param4)
{
	_XDWINPRINT("SEND PACKET : Request owner Req Schedule list");

	int size = sizeof(MSG_CB_OWNER_REQ_SCHEDULE_LIST);
	MSG_CB_OWNER_REQ_SCHEDULE_LIST reqschedule;
	ZeroMemory(&reqschedule, size);
	reqschedule.header.sLength = size;
	reqschedule.ucMessage = type;

	reqschedule.ucMode = (unsigned char)param1;
	reqschedule.ucMapIndex = (unsigned char)g_pLocalUser->m_nCastleMapId;

	return _XSendPacket_Encapsulate((char*)&reqschedule);
}

BOOL _XNetwork::SendMSGCBReqScheduleList(int type, int param1, int param2, int param3, int param4)
{
	_XDWINPRINT("SENDPACKET : Request CB Scheduler list");

	int size = sizeof(MSG_CB_REQ_SCHEDULE_LIST);
	MSG_CB_REQ_SCHEDULE_LIST reqschedule;
	ZeroMemory(&reqschedule, size);
	reqschedule.header.sLength = size;
	reqschedule.ucMessage = type;

	reqschedule.ucMode = (unsigned char)param1;

	return _XSendPacket_Encapsulate((char*)&reqschedule);
}

BOOL _XNetwork::SendMSGCBReqChallengeList(int type, int param1, int param2, int param3, int param4)
{
	_XDWINPRINT("SENDPACKET : Request CB Challenge list");

	int size = sizeof(MSG_CB_REQ_CHALLENGE_LIST);
	MSG_CB_REQ_CHALLENGE_LIST reqchallengelist;
	ZeroMemory(&reqchallengelist, size);
	reqchallengelist.header.sLength = size;
	reqchallengelist.ucMessage = type;

	reqchallengelist.ucMode = (unsigned char)param1;

	return _XSendPacket_Encapsulate((char*)&reqchallengelist);
}

BOOL _XNetwork::SendMSGCBChallengeCancel(int type, int param1, int param2, int param3, int param4)
{
	_XDWINPRINT("SENDPACKET : Request CB Challenge Cancel");

	int size = sizeof(MSG_CB_CHALLENGE_CANCEL);
	MSG_CB_CHALLENGE_CANCEL cancel;
	ZeroMemory(&cancel, size);
	cancel.header.sLength = size;
	cancel.ucMessage = type;

	cancel.ucMode = (unsigned char)param1;

	return _XSendPacket_Encapsulate((char*)&cancel);
}

BOOL _XNetwork::SendMSGCBReqChangeTime(int type, int param1, int param2, int param3, int param4)
{
	// param2 : 맵 번호
	_XDWINPRINT("SEND PACKET : Request CB Change Time");

	int size = sizeof(MSG_CB_REQ_CHANGE_TIME);
	MSG_CB_REQ_CHANGE_TIME reqchangetime;
	ZeroMemory(&reqchangetime, size);
	reqchangetime.header.sLength = size;
	reqchangetime.ucMessage = type;

	reqchangetime.ucMode = (unsigned char)param1;
	reqchangetime.ucMapIndex = (unsigned char)param2;

	_XWindow_CastleBattleTimeChange* pCBTimeChange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
	if(pCBTimeChange_Window)
	{
		reqchangetime.time[0].uiHour = (unsigned int)(pCBTimeChange_Window->m_tempChangeTime[0] / 100.f);
		reqchangetime.time[0].uiMinute = (unsigned int)(pCBTimeChange_Window->m_tempChangeTime[0] % 100);

		reqchangetime.time[1].uiHour = (unsigned int)(pCBTimeChange_Window->m_tempChangeTime[1] / 100.f);
		reqchangetime.time[1].uiMinute = (unsigned int)(pCBTimeChange_Window->m_tempChangeTime[1] % 100);

		reqchangetime.time[2].uiHour = (unsigned int)(pCBTimeChange_Window->m_tempChangeTime[2] / 100.f);
		reqchangetime.time[2].uiMinute = (unsigned int)(pCBTimeChange_Window->m_tempChangeTime[2] % 100);
	}

	return _XSendPacket_Encapsulate((char*)&reqchangetime);
}

BOOL _XNetwork::SendMSGCBReqChallenge(int type, int param1, int param2, int param3, int param4)
{
	// param2 : 몇 차전
	// param3 : 필요한 돈 (g_CastleScheduler.m_nChallengeMoney*단원수)

	_XDWINPRINT("SEND PACKET : Request CB Challenge");

	int size = sizeof(MSG_CB_REQ_CHALLENGE);
	MSG_CB_REQ_CHALLENGE reqchallenge;
	ZeroMemory(&reqchallenge, size);
	reqchallenge.header.sLength = size;
	reqchallenge.ucMessage = type;

	reqchallenge.ucMode = (unsigned char)param1;
	reqchallenge.uiNeedMoney = (unsigned int)param2;

	return _XSendPacket_Encapsulate((char*)&reqchallenge);
}

// Recieve ============

void _XNetwork::MSGCBOnwerScheduleList(MSG_CB_OWNER_SCHEDULE_LIST* schedulelist)
{
	// 장원 정보, 도전 일정 setting
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_OWNER_SCHEDULE_LIST");

	g_CBManager.SetScheduleList(g_pLocalUser->m_nCastleMapId, &schedulelist->CastleInfo, schedulelist->uiSaveMoney);

	//도전자 목록 모두 다시 setting
	g_CBManager.ResetChallengeList(g_pLocalUser->m_nCastleMapId);
	for(int i = 0 ; i < max_count_of_challenge_list ; ++i)
	{
		if(schedulelist->info[i].cMasterName[0] == 0)
			continue;

		g_CBManager.SetChallengeList(g_pLocalUser->m_nCastleMapId, &schedulelist->info[i]);
	}

	if(g_pGroup_Window)
	{
		g_pGroup_Window->SetCastleData();
		g_pGroup_Window->SetChallengerListItem();
	}
}

void _XNetwork::MSGCBScheduleList(MSG_CB_SCHEDULE_LIST* schedulelist)
{
	// 도전 일정
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_SCHEDULE_LIST");


	g_CBManager.SetScheduleList(g_CurrentZoneInfoIndex, &schedulelist->CastleInfo);

	_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
	if(pCastleInfo_Window)
	{
		pCastleInfo_Window->SetData();
	}

	_XWindow_ChallengerList* pChallengeList_Window = (_XWindow_ChallengerList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHALLENGERLIST);
	if(pChallengeList_Window)
	{
		pChallengeList_Window->SetData();
	}
}

void _XNetwork::MSGCBChallengeList(MSG_CB_CHALLENGE_LIST* challengelist)
{
	// 도전자 목록 모두 다시 setting
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_CHALLENGE_LIST");

	g_CBManager.ResetChallengeList(g_CurrentZoneInfoIndex);
	for(int i = 0 ; i < max_count_of_challenge_list ; ++i)
	{
		if(challengelist->info[i].cMasterName[0] == 0)
			continue;

		g_CBManager.SetChallengeList(g_CurrentZoneInfoIndex, &challengelist->info[i]);	
	}

	_XWindow_ChallengerList* pChallengeList_Window = (_XWindow_ChallengerList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHALLENGERLIST);
	if(pChallengeList_Window)
	{
		pChallengeList_Window->SetListItem();
		pChallengeList_Window->EnableButton(TRUE);
	}
}

void _XNetwork::MSGCBChangeTimeSuccess(MSG_CB_CHANGE_TIME_SUCCESS* changetime)
{
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_CHANGE_TIME_SUCCESS");

	for(int i = 0 ; i < 3 ; ++i)
	{
		g_CBManager.ChangeTime(changetime->ucMapIndex, i, &changetime->timeResult[i]);
	}

	_XWindow_CastleBattleTimeChange* pCBTimeChange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
	if(pCBTimeChange_Window)
	{
		pCBTimeChange_Window->ChangeTime();
		pCBTimeChange_Window->ShowWindow(FALSE);
	}
}

void _XNetwork::MSGCBChangeTimeFail(MSG_CB_CHANGE_TIME_FAIL* changetime)
{
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_CHANGE_TIME_FAIL");

	if(changetime->errorCode == 0)
	{
		TCHAR msg[256];
		memset(msg, 0, sizeof(msg));

		_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NEW_3209)); // 도전 시간을 지정할 수 있는 기간이 지났습니다.
		InsertChatString(msg, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

		_XWindow_CastleBattleTimeChange* pCBTimeChange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
		if(pCBTimeChange_Window)
		{
			pCBTimeChange_Window->ShowWindow(FALSE);
		}

		_XDWINPRINT(msg);
	}
	else
	{
#ifdef _XDWDEBUG
		TCHAR msg[256];
		memset(msg, 0, sizeof(msg));

		_snprintf(msg, sizeof(msg), "Unknown Error [ErrorCode : %d]", changetime->errorCode);
		InsertChatString(msg, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

		_XDWINPRINT(msg);
#endif
	}
}

void _XNetwork::MSGCBChallengeSuccess(MSG_CB_CHALLENGE_SUCCESS* challengeresult)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_CHALLENGE_SUCCESS");

	g_CBManager.SetChallengeList(g_CurrentZoneInfoIndex, &challengeresult->challengeInfo);
	g_pLocalUser->m_Money = challengeresult->uiLeftMoney;
	
	_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
	if(pCastleInfo_Window)
	{
		pCastleInfo_Window->EnableButton(TRUE);
	}
	
	// 3356
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3356), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	// 신청이 완료 되었습니다.
}

void _XNetwork::MSGCBChallengeFail(MSG_CB_CHALLENGE_FAIL* challengeresult)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_CHALLENGE_FAIL");

	TCHAR msg[256];
	memset(msg, 0, sizeof(msg));

	switch(challengeresult->errorCode)
	{
	case 0 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1761));	// 금액이 부족합니다.
		}
		break;
	case 1 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NEW_3210));	// 더 이상 도전할 수 없습니다.
		}
		break;
	case 2 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NEW_3222));	// 단주나 회주가 아닙니다.
		}
		break;
	case 3 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NEW_3223));	// 현재 장주 이므로 도전할 수 없습니다. 
		}
		break;
	case 4 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTLATER));	// 잠시 후에 다시 시도하십시오.
		}
		break;
	case 6 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NEW_3268));	//장원 도전 시간이 아닙니다.
		}
		break;
	case 7 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NEW_3396));		//"다른 장원에 이미 도전 신청 하였습니다."
		}
		break;
	case 8 :
		{
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NETWORK_2333));	//성취단계가 부족합니다.
		}	
		break;
	default :
	case 5 :
		{
#ifdef _XDWDEBUG
			_snprintf(msg, sizeof(msg), "system error [ErrorCode : %d]", challengeresult->errorCode);
#endif
		}
		break;
	}

	_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
	if(pCastleInfo_Window)
	{
		pCastleInfo_Window->EnableButton(TRUE);
	}

	InsertChatString(msg, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

	_XLog(msg);

	// 차후 적용
/*	if(challengeresult->errorCode == 7)
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), "이미 다른 지역의 장원에 도전한 기록이 있습니다. 이전 도전을 취소하시겠습니까?", 
			TRUE, _XDEF_CASTLEINFO_CANCELWAR);	// add string table
	}*/
}

void _XNetwork::MSGCBUAreOwner(MSG_CB_U_ARE_OWNER* owner)
{
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_U_ARE_OWNER");
	_XLog("RP : You are owner");

	g_pLocalUser->m_nCastleMapId = owner->ucMapIndex;

	// schedulecanchange 처리

	_XDWINPRINT("[CB/Network] mapindex : %d", owner->ucMapIndex);
}

void _XNetwork::MSGCBUAreNotOwner(MSG_CB_U_ARE_NOT_OWNER* owner)
{
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_U_ARE_NOT_OWNER");
	_XLog("RP : You are not owner");

	if(g_pGroup_Window)
	{
		g_pGroup_Window->ResetCastleData();
	}

	g_pLocalUser->m_nCastleMapId = 0;
}

void _XNetwork::MSGCBNotOwner(MSG_CB_NOT_OWNER* owner)
{
	_XDWINPRINT("RECEIVE PACKET : MSG_CB_NOT_OWNER");

	// 3288
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3288), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//소유하고 있는 장원이 없습니다.
	_XLog(_XGETINTERFACETEXT(ID_STRING_NEW_3288));
}

void _XNetwork::MSGCBNoSuchCastle(MSG_CB_NO_SUCH_CASTLE* nocastle)
{
	_XDWINPRINT("RECEIVE PACKET : MSG_CB_NO_SUCH_CASTLE");

	// 3289
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3289), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		//없는 장원입니다.
	_XLog(_XGETINTERFACETEXT(ID_STRING_NEW_3289));
}

void _XNetwork::MSGCBScheduleError(MSG_CB_SCHEDULE_ERROR* error)
{
	_XDWINPRINT("RECEIVE PACKET : MSG_CB_SCHEDULE_ERROR");
}

void _XNetwork::MSGCBProgReadyEvent(MSG_CB_PROG_READY_EVENT* readyevent)
{
	// 시작 30분 전
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_PROG_READY_EVENT");
	_XLog("RP : Ready");

	_XDWINPRINT("[CB/Network] LeftTimeSec : %d ", readyevent->usLeftTimeSec);
	_XLog("CB : LeftTime : %d", readyevent->usLeftTimeSec);
	_XDWINPRINT("[CB/Network] Defender : %d/%d/%s", readyevent->Defender.ucType, readyevent->Defender.iIndex, readyevent->Defender.cGroupName);
	_XLog("CB : Defender : %s", readyevent->Defender.cGroupName);
	for(int i = 0 ; i < readyevent->ucAttackerCount ; ++i)
	{
		_XDWINPRINT("[CB/Network] Attacker : %d/%d/%s", readyevent->Attackers[i].ucType, readyevent->Attackers[i].iIndex, readyevent->Attackers[i].cGroupName);
		_XLog("CB : Attacker : %s", readyevent->Attackers[i].cGroupName);
	}

	int mytype = 0;
	int myindex = 0;

	if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex >0)
	{
		mytype = 1;
		myindex = g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex;
	}
	else if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
	{
		mytype = 0;
		myindex = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex;
	}

	// attacker or defender
	if(readyevent->Defender.ucType == mytype)
	{
		if(readyevent->Defender.iIndex == myindex)
		{
			g_pLocalUser->m_dwAttackType = 2;		// defender
			g_pLocalUser->m_bCastleBattle = true;

			_XDWINPRINT("[CB/Network]MyInfo : 수성");
			_XLog("CB : 수성");

			// 적대관계 저장
			g_pLocalUser->m_nDefenderType = readyevent->Defender.ucType;
			g_pLocalUser->m_nDefenderIndex = readyevent->Defender.iIndex;

			memset(g_pLocalUser->m_nAttackerType, 0, sizeof(g_pLocalUser->m_nAttackerType));
			memset(g_pLocalUser->m_nAttackerIndex, 0, sizeof(g_pLocalUser->m_nAttackerIndex));
			for(int atkcnt = 0 ; atkcnt < readyevent->ucAttackerCount ; ++atkcnt)
			{
				g_pLocalUser->m_nAttackerType[atkcnt] = readyevent->Attackers[atkcnt].ucType;
				g_pLocalUser->m_nAttackerIndex[atkcnt] = readyevent->Attackers[atkcnt].iIndex;
			}
		}
	}
	for(i = 0 ; i < readyevent->ucAttackerCount ; ++i)
	{
		if(readyevent->Attackers[i].ucType == mytype)
		{
			if(readyevent->Attackers[i].iIndex == myindex)
			{
				g_pLocalUser->m_dwAttackType = 1;		// attacker
				g_pLocalUser->m_bCastleBattle = true;

				_XDWINPRINT("[CB/Network]MyInfo : 공성");
				_XLog("CB : 공성");

				// 적대관계 저장
				g_pLocalUser->m_nDefenderType = readyevent->Defender.ucType;
				g_pLocalUser->m_nDefenderIndex = readyevent->Defender.iIndex;

				memset(g_pLocalUser->m_nAttackerType, 0, sizeof(g_pLocalUser->m_nAttackerType));
				memset(g_pLocalUser->m_nAttackerIndex, 0, sizeof(g_pLocalUser->m_nAttackerIndex));
				for(int atkcnt = 0 ; atkcnt < readyevent->ucAttackerCount ; ++atkcnt)
				{
					g_pLocalUser->m_nAttackerType[atkcnt] = readyevent->Attackers[atkcnt].ucType;
					g_pLocalUser->m_nAttackerIndex[atkcnt] = readyevent->Attackers[atkcnt].iIndex;
				}
				break;
			}
		}
	}

	if(g_pLocalUser->m_bCastleBattle)
	{
		// role
		if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex >0)
		{
			if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_)
			{
				g_pLocalUser->m_dwRoleInCastleBattle = 1;	// 총대장
				{
					_XDWINPRINT("[CB/Network]MyInfo : 총대장");
					_XLog("CB : 총대장");
				}
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
		{
			if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
			{
				g_pLocalUser->m_dwRoleInCastleBattle = 1;	// 총대장
				_XDWINPRINT("[CB/Network]MyInfo : 총대장");
				_XLog("CB : 총대장");
			}
		}

		g_pLocalUser->m_nCBMapIdInWar = g_CurrentZoneInfoIndex;
		g_CBManager.m_enWarState = _XEN_CB_READY;
		g_CBManager.m_dwTimeFromServer = g_LocalSystemTime;
		g_CBManager.ClearState();
	}
}

void _XNetwork::MSGCBProgStartEvent(MSG_CB_PROG_START_EVENT* startevent)
{
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_PROG_START_EVENT");
	_XLog("RP : Start");

	_XDWINPRINT("[CB/Network] Defender : %d/%d/%s", startevent->Defender.ucType, startevent->Defender.iIndex, startevent->Defender.cGroupName);
	_XLog("CB : Defender : %s", startevent->Defender.cGroupName);
	for(int i = 0 ; i < startevent->ucAttackerCount ; ++i)
	{
		_XDWINPRINT("[CB/Network] Attacker : %d/%d/%s", startevent->Attackers[i].ucType, startevent->Attackers[i].iIndex, startevent->Attackers[i].cGroupName);
		_XLog("CB : Attacker : %s", startevent->Attackers[i].cGroupName);
	}

	// 시작
	if(g_pLocalUser->m_bCastleBattle)
	{
		g_CBManager.m_enWarState = _XEN_CB_PROGRESS;
		g_CBManager.m_dwTimeFromServer = g_LocalSystemTime;
		g_CBManager.ClearState();

		//장원전 시간표시 관련
		_XWindow_CastleBattlePersonalHistory* pCastleBattlePersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
		if(pCastleBattlePersonalHistory_Window)
		{
			pCastleBattlePersonalHistory_Window->SetOwner(startevent->Defender.ucType, startevent->Defender.iIndex);
			pCastleBattlePersonalHistory_Window->ShowWindow(TRUE);
		}

#ifdef _XDEF_CASTLEBATTLE_RECORD
		// 장원 전적
		_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
		if(pCBRecord_Window)
		{
			pCBRecord_Window->DeleteAll();
			pCBRecord_Window->ShowWindow(TRUE);
		}
#endif
	}
	else
	{
		// 만약 세팅이 안되어 있다면 
		int mytype = 0;
		int myindex = 0;

		if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex >0)
		{
			mytype = 1;
			myindex = g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex;
		}
		else if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
		{
			mytype = 0;
			myindex = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex;
		}

		// attacker or defender
		if(startevent->Defender.ucType == mytype)
		{
			if(startevent->Defender.iIndex == myindex)
			{
				g_pLocalUser->m_dwAttackType = 2;		// defender
				g_pLocalUser->m_bCastleBattle = true;

				_XDWINPRINT("[CB/Network]MyInfo : 수성");
				_XLog("CB : 수성");

				// 적대관계 저장
				g_pLocalUser->m_nDefenderType = startevent->Defender.ucType;
				g_pLocalUser->m_nDefenderIndex = startevent->Defender.iIndex;

				memset(g_pLocalUser->m_nAttackerType, 0, sizeof(g_pLocalUser->m_nAttackerType));
				memset(g_pLocalUser->m_nAttackerIndex, 0, sizeof(g_pLocalUser->m_nAttackerIndex));
				for(int atkcnt = 0 ; atkcnt < startevent->ucAttackerCount ; ++atkcnt)
				{
					g_pLocalUser->m_nAttackerType[atkcnt] = startevent->Attackers[atkcnt].ucType;
					g_pLocalUser->m_nAttackerIndex[atkcnt] = startevent->Attackers[atkcnt].iIndex;
				}
			}
		}
		for(i = 0 ; i < startevent->ucAttackerCount ; ++i)
		{
			if(startevent->Attackers[i].ucType == mytype)
			{
				if(startevent->Attackers[i].iIndex == myindex)
				{
					g_pLocalUser->m_dwAttackType = 1;		// attacker
					g_pLocalUser->m_bCastleBattle = true;

					_XDWINPRINT("[CB/Network]MyInfo : 공성");
					_XLog("CB : 공성");

					// 적대관계 저장
					g_pLocalUser->m_nDefenderType = startevent->Defender.ucType;
					g_pLocalUser->m_nDefenderIndex = startevent->Defender.iIndex;

					memset(g_pLocalUser->m_nAttackerType, 0, sizeof(g_pLocalUser->m_nAttackerType));
					memset(g_pLocalUser->m_nAttackerIndex, 0, sizeof(g_pLocalUser->m_nAttackerIndex));
					for(int atkcnt = 0 ; atkcnt < startevent->ucAttackerCount ; ++atkcnt)
					{
						g_pLocalUser->m_nAttackerType[atkcnt] = startevent->Attackers[atkcnt].ucType;
						g_pLocalUser->m_nAttackerIndex[atkcnt] = startevent->Attackers[atkcnt].iIndex;
					}
					break;
				}
			}
		}

		if(g_pLocalUser->m_bCastleBattle)
		{
			g_CBManager.m_enWarState = _XEN_CB_PROGRESS;
			g_CBManager.m_dwTimeFromServer = g_LocalSystemTime;
			g_CBManager.ClearState();

			//장원전 시간표시 관련
			_XWindow_CastleBattlePersonalHistory* pCastleBattlePersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
			if(pCastleBattlePersonalHistory_Window)
			{
				pCastleBattlePersonalHistory_Window->SetOwner(startevent->Defender.ucType, startevent->Defender.iIndex);
				pCastleBattlePersonalHistory_Window->ShowWindow(TRUE);
			}

#ifdef _XDEF_CASTLEBATTLE_RECORD
			// 장원 전적
			_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
			if(pCBRecord_Window)
			{
				pCBRecord_Window->ShowWindow(TRUE);
			}
#endif
		}
	}

	// 나머지 패킷 처리 필요하면 할 것
}

void _XNetwork::MSGCBProgEndEvent(MSG_CB_PROG_END_EVENT* endevent)
{
	_XDWINPRINT("RECEIEVE PACKET : MSG_CB_PROG_END_EVENT");
	_XLog("RP : End");

	_XDWINPRINT("[CB/Network] WinSide : %d", endevent->ucWinSide);
	_XLog("CB : Win : %s", endevent->cGroupName);
	_XDWINPRINT("[CB/Network] Winner : %d/%d/%s", endevent->ucGroupType, endevent->iIndex, endevent->cGroupName);

	BOOL bMonsterWin = FALSE;

	if(endevent->ucWinSide == 0 && endevent->ucGroupType == 2)
	{
		// defender가 이겼음 && defender는 몬스터
		bMonsterWin = TRUE;
	}

	// 끝
	if(g_pLocalUser->m_bCastleBattle)
	{
		g_CBManager.m_enWarState = _XEN_CB_END;
		g_CBManager.m_dwTimeFromServer = g_LocalSystemTime;
		g_CBManager.ClearState();

		g_pLocalUser->m_dwResultCastleBattle = 2;
		if(endevent->ucGroupType == 1)
		{
			if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex == endevent->iIndex)
			{
				// win
				g_pLocalUser->m_dwResultCastleBattle = 1;
				_XLog("[CB/Network] I win");
			}
		}
		else
		{
			if(endevent->ucGroupType == 0)
			{
				if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == endevent->iIndex)
				{
					// win
					g_pLocalUser->m_dwResultCastleBattle = 1;
					_XLog("[CB/Network] I win");
				}
			}
		}

/*		if(endevent->ucWinSide == 0)
		{
			// defender가 이겼음
			if(g_pLocalUser->m_dwAttackType == 2)
			{
				g_pLocalUser->m_dwResultCastleBattle = 1;
			}
			else
			{
				g_pLocalUser->m_dwResultCastleBattle = 2;
			}
		}
		else if(endevent->ucWinSide == 1)
		{
			// attacker가 이겼음
			if(g_pLocalUser->m_dwAttackType == 1)
			{
				g_pLocalUser->m_dwResultCastleBattle = 1;
			}
			else
			{
				g_pLocalUser->m_dwResultCastleBattle = 2;
			}
		}
*/

		//장원전 시간표시 관련
		_XWindow_CastleBattlePersonalHistory* pCastleBattlePersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
		if(pCastleBattlePersonalHistory_Window)
		{
			pCastleBattlePersonalHistory_Window->ShowWindow(FALSE);
		}

#ifdef _XDEF_CASTLEBATTLE_RECORD
		// 장원 전적
		_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
		if(pCBRecord_Window)
		{
			pCBRecord_Window->ShowWindow(FALSE);
		}
#endif

		_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
		if(pThreatenGauge_Window)
		{
			if(pThreatenGauge_Window->GetShowStatus())
			{
				pThreatenGauge_Window->ShowWindow(FALSE);

				// npc animation idle로
				if(g_LastNPCScriptOwner)
				{
					if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
					{
						g_LastNPCScriptOwner->SetIdleAction();
					}
				}
				else
				{
					int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
					if(index >= 0)
					{
						if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
								g_Npclist.m_pNPCObjectArray[index].SetIdleAction();
					}
				}
			}
		}
		
		// 협박 중이라면 취소
		if(g_pLocalUser->m_bThreatening)
		{
			g_CBManager.m_dwThreatenStartTime = 0;
			g_pLocalUser->m_bSendThreatenPacket = false;
			g_CBManager.m_dwThreatenRequestTime = 0;
			g_CBManager.m_bSuccessThreaten = FALSE;
			g_pLocalUser->m_bThreatening = false;
		}
	}

	if(!g_pLocalUser->m_bIsBossBattle)	// "결"안에서는 메시지 보여주지 않음
	{
		TCHAR messagestr[256];
		memset(messagestr, 0, sizeof(messagestr));

		_XWindow_CBResultNotice* pCBResultNotice_Window = (_XWindow_CBResultNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRESULTNOTICE);
		if(pCBResultNotice_Window)
		{
			pCBResultNotice_Window->SetCastleName(g_CBManager.GetCastleNameTextId(g_CurrentZoneInfoIndex));
			pCBResultNotice_Window->SetOrgName(endevent->cGroupName, bMonsterWin, endevent->ucGroupType);

			if(bMonsterWin)
			{
				_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3295), _XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(g_CurrentZoneInfoIndex)));//%s을 함락시키지 못하였습니다.
			}
			else
			{
				TCHAR groupname[128];
				memset(groupname, 0, sizeof(groupname));
				if(endevent->ucGroupType == 0)
				{
					_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), endevent->cGroupName);
				}
				else if(endevent->ucGroupType == 1)
				{
					_snprintf(groupname, sizeof(groupname), _XGETINTERFACETEXT(ID_STRING_NEW_3125), endevent->cGroupName);
				}

				_snprintf(messagestr, sizeof(messagestr), g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_3255, 
					groupname, _XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(g_CurrentZoneInfoIndex))));	//"%s(이)가 %s(을)를 소유하였습니다."
			}

			pCBResultNotice_Window->ShowWindow(TRUE);
		}

		InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);

		_XLog(messagestr);
	}
}

void _XNetwork::MSGCBThreatStartOK(MSG_CB_THREAT_START_OK* threatok)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREAT_START_OK [Progress : %d]", threatok->ucProgressRate);

	// 협박 시작
	//progressive bar
	_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
	if(pThreatenGauge_Window)
	{
		pThreatenGauge_Window->ShowWindow(TRUE);
	}
	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetGaugeMode(_XDEF_CB_GAUGEMODE_THREAT);
		pCBPersonalHistory_Window->SetThreatenString(0, 0, TRUE);
	}

	g_CBManager.m_dwThreatenStartTime	= g_LocalSystemTime;
	g_CBManager.m_dwThreatenRequestTime = g_LocalSystemTime;
	g_pLocalUser->m_bSendThreatenPacket = false;

	g_pLocalUser->m_bThreatening = true;
	
	if(threatok->ucProgressRate == 100)
	{
		g_CBManager.m_bSuccessThreaten = TRUE;
		g_CBManager.m_nProgress = threatok->ucProgressRate;
	}
	else
	{
		g_CBManager.m_bSuccessThreaten = FALSE;
		g_CBManager.m_nProgress = threatok->ucProgressRate;
	}

	// npc animation setting
	if(g_LastNPCScriptOwner)
	{
		if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
		{
			g_LastNPCScriptOwner->SetConversationAction();
		}
		else
		{
			int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
			if(index >= 0)
			{
				if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
					g_Npclist.m_pNPCObjectArray[index].SetConversationAction();
			}
		}
	}
	else
	{
		int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
		if(index >= 0)
		{
			if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
					g_Npclist.m_pNPCObjectArray[index].SetConversationAction();
		}
	}
	
	TCHAR messagestr[256];
	_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3256), g_pLocalUser->m_CharacterInfo.charactername);	//%s님이 장주를 협박하고 있습니다.
	InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

	_XLog(messagestr);
}

void _XNetwork::MSGCBThreatStartError(MSG_CB_THREAT_START_ERROR* threaterr)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREAT_START_ERROR");

	// 협박 에러
	switch(threaterr->ucError)
	{
	case 0 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3357), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"장원전이 진행중인 지역이 아닙니다."
		}
		break;
	case 1 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3358), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"다른 총대장이 협박중입니다."
		}
		break;
	case 2 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3359), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"지금 협박 중입니다."
		}
		break;
	case 3 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3360), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"장원전 진행 중이 아닙니다"
		}
		break;
	case 4 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3361), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //"장원혈이 부족합니다."
		}
		break;
	default :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3362), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"지금은 협박을 진행할 수 없습니다."
		}
		break;
	}

	g_pLocalUser->m_bSendThreatenPacket = false;
#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
	g_CBManager.m_dwThreatenRequestTime = g_LocalSystemTime;
#endif
/*	g_CBManager.m_dwThreatenStartTime = 0;
	g_CBManager.m_bSuccessThreaten = FALSE;
	g_pLocalUser->m_bThreatening = false;*/
}

void _XNetwork::MSGCBThreatStartNotice(MSG_CB_THREAT_START_NOTICE* notice)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREAT_START_NOTICE");
	_XDWINPRINT("[CB/Network] Current Owner Side : %d", notice->ucCurrOwnerSide);
	
	if(g_pLocalUser->m_bThreatening)
	{
		// 자기가 현재 협박 중일 때
		return;
	}

	// 시스템 메세지 보여주기 - 협박 시작 했음
	BOOL bMsg = FALSE;
	TCHAR messagestr[256];
	memset(messagestr, 0, sizeof(messagestr));

	if(notice->ucGroupType == 0)
	{
		// Org
		_XUSER_ORGANIZATION_GLOBAL_INFO* orginfo = g_Userlist.SearchOrganizationInfo(notice->iIndex);
		if(orginfo)
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3363), orginfo->m_cName);	//"%s단이 장주를 협박하고 있습니다."
			bMsg = TRUE;
		}
	}
	else if(notice->ucGroupType == 1)
	{
		// Ally
		_XUSER_ALIENCE_GLOBAL_INFO* allyinfo = g_Userlist.SearchAlienceInfo(notice->iIndex);
		if(allyinfo)
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3364), allyinfo->m_cName);	//"%s회가 장주를 협박하고 있습니다."
			bMsg = TRUE;
		}
	}

	if(!bMsg)
	{
		if(notice->ucCurrOwnerSide == 0)
		{	
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3257));	//방어측이 장주를 협박하고 있습니다.
		}
		else
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3258));	//공격측이 장주를 협박하고 있습니다.
		}
	}

	InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

	_XLog(messagestr);

	// progressive bar display용
	g_CBManager.m_dwThreatenStartTime	= g_LocalSystemTime;
	_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
	if(pThreatenGauge_Window)
	{
		pThreatenGauge_Window->ShowWindow(TRUE);
	}
	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetGaugeMode(_XDEF_CB_GAUGEMODE_THREAT);
		pCBPersonalHistory_Window->SetThreatenString(notice->ucGroupType, notice->iIndex);
	}

	// npc animation setting
	if(g_LastNPCScriptOwner)
	{
		if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
		{
			g_LastNPCScriptOwner->SetConversationAction();
		}
		else
		{
			int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
			if(index >= 0)
			{
				if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
						g_Npclist.m_pNPCObjectArray[index].SetConversationAction();
			}
		}

	}
	else
	{
		int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
		if(index >= 0)
		{
			if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
					g_Npclist.m_pNPCObjectArray[index].SetConversationAction();
		}
	}
}

void _XNetwork::MSGCBThreatCancelOK(MSG_CB_THREAT_CANCEL_OK* cancel)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREAT_CANCEL_OK");

	// 협박 중지
	_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
	if(pThreatenGauge_Window)
	{
		pThreatenGauge_Window->ShowWindow(FALSE);
	}
	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetGaugeMode(_XDEF_CB_GAUGEMODE_DELAY);
	}

	g_CBManager.m_dwThreatenStartTime = 0;
	g_pLocalUser->m_bSendThreatenPacket = false;
	g_CBManager.m_dwThreatenRequestTime = g_LocalSystemTime;
	g_CBManager.m_bSuccessThreaten = FALSE;
	g_pLocalUser->m_bThreatening = false;

	// npc animation idle로
	if(g_LastNPCScriptOwner)
	{
		if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
		{
			g_LastNPCScriptOwner->SetIdleAction();
		}
		else
		{
			int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
			if(index >= 0)
			{
				if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
						g_Npclist.m_pNPCObjectArray[index].SetIdleAction();
			}
		}
	}
	else
	{
		int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
		if(index >= 0)
		{
			if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
					g_Npclist.m_pNPCObjectArray[index].SetIdleAction();
		}
	}

	BOOL bMsg = FALSE;
	TCHAR messagestr[256];
	if(cancel->ucGroupType == 0)
	{
		// Org
		_XUSER_ORGANIZATION_GLOBAL_INFO* orginfo = g_Userlist.SearchOrganizationInfo(cancel->iIndex);
		if(orginfo)
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3365), orginfo->m_cName);	//"장주가 %s단의 협박에 항복하지 않았습니다."
			bMsg = TRUE;
		}
	}
	else if(cancel->ucGroupType == 1)
	{
		// Ally
		_XUSER_ALIENCE_GLOBAL_INFO* allyinfo = g_Userlist.SearchAlienceInfo(cancel->iIndex);
		if(allyinfo)
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3366), allyinfo->m_cName);	//"장주가 %s회의 협박에 항복하지 않았습니다."
			bMsg = TRUE;
		}
	}

	if(!bMsg)
	{
		_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3259));	//장주가 협박에 항복하지 않았습니다.
	}
	
	InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
	_XLog(messagestr);
}

void _XNetwork::MSGCBThreatCompleteOK(MSG_CB_THREAT_COMPLETE_OK* complete)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREAT_COMPLETE_OK");

	// 협박 성공
	_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
	if(pThreatenGauge_Window)
	{
		pThreatenGauge_Window->ShowWindow(FALSE);
	}
	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetGaugeMode(_XDEF_CB_GAUGEMODE_DELAY);
	}

	g_CBManager.m_dwThreatenStartTime = 0;
	g_pLocalUser->m_bSendThreatenPacket = false;
	g_CBManager.m_dwThreatenRequestTime = g_LocalSystemTime;
	g_CBManager.m_bSuccessThreaten = FALSE;
	g_pLocalUser->m_bThreatening = false;

	// npc animation 보여주기
	if(g_LastNPCScriptOwner)
	{
		if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
		{
			g_LastNPCScriptOwner->SetSurrenderAction();
		}
		else
		{
			int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
			if(index >= 0)
			{
				if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
						g_Npclist.m_pNPCObjectArray[index].SetSurrenderAction();
			}
		}
	}
	else
	{
		int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
		if(index >= 0)
		{
			if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
					g_Npclist.m_pNPCObjectArray[index].SetSurrenderAction();
		}
	}

	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3260), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	//장주가 협박에 항복하였습니다.

	_XLog(_XGETINTERFACETEXT(ID_STRING_NEW_3260));
}

void _XNetwork::MSGCBThreatCompleteError(MSG_CB_THREAT_COMPLETE_ERROR* complete)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREAT_COMPLETE_ERROR");

	// 협박 에러 - 실패
	_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
	if(pThreatenGauge_Window)
	{
		pThreatenGauge_Window->ShowWindow(FALSE);
	}
	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetGaugeMode(_XDEF_CB_GAUGEMODE_DELAY);
	}

	g_CBManager.m_dwThreatenStartTime = 0;
	g_pLocalUser->m_bSendThreatenPacket = false;
	g_CBManager.m_dwThreatenRequestTime = g_LocalSystemTime;
	g_CBManager.m_bSuccessThreaten = FALSE;
	g_pLocalUser->m_bThreatening = false;

	// npc animation idle로
	if(g_LastNPCScriptOwner)
	{
		if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
		{
			g_LastNPCScriptOwner->SetIdleAction();
		}
		else
		{
			int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
			if(index >= 0)
			{
				if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
						g_Npclist.m_pNPCObjectArray[index].SetIdleAction();
			}
		}
	}
	else
	{
		int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
		if(index >= 0)
		{
			if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
					g_Npclist.m_pNPCObjectArray[index].SetIdleAction();
		}
	}

	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3259), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	//장주가 협박에 항복하지 않았습니다.

	_XLog(_XGETINTERFACETEXT(ID_STRING_NEW_3259));
}

void _XNetwork::MSGCBThreatCompleteNotice(MSG_CB_THREAT_COMPLETE_NOTICE* complete)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREAT_COMPLETE_NOTICE");
	_XDWINPRINT("[CB/Network] Current Owner Side : %d", complete->ucNewOwnerSide);

	// 현 소유주 display
	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetOwner(complete->ucGroupType, complete->iIndex);
		pCBPersonalHistory_Window->SetGaugeMode(_XDEF_CB_GAUGEMODE_DELAY);
	}

	_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
	if(!pThreatenGauge_Window->GetShowStatus())
	{
		// 총대장일 때는 CompleteOK로 처리 되었음
		return;
	}

	if(pThreatenGauge_Window)
	{
		pThreatenGauge_Window->ShowWindow(FALSE);
	}

	g_CBManager.m_dwThreatenRequestTime = g_LocalSystemTime;

	// 협박 성공 - 알림 . 시스템 메시지
	BOOL bMsg = FALSE;
	TCHAR messagestr[256];
	if(complete->ucGroupType == 0)
	{
		// Org
		_XUSER_ORGANIZATION_GLOBAL_INFO* orginfo = g_Userlist.SearchOrganizationInfo(complete->iIndex);
		if(orginfo)
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3367), orginfo->m_cName);	//"장주가 %s단의 협박에 항복하였습니다."
			bMsg = TRUE;
		}
	}
	else if(complete->ucGroupType == 1)
	{
		// Ally
		_XUSER_ALIENCE_GLOBAL_INFO* allyinfo = g_Userlist.SearchAlienceInfo(complete->iIndex);
		if(allyinfo)
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3368), allyinfo->m_cName);	//"장주가 %s회의 협박에 항복하였습니다."
			bMsg = TRUE;
		}
	}

	if(!bMsg)
	{
		if(complete->ucNewOwnerSide == 0)
		{
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3261));		//장주가 방어측의 협박에 항복하였습니다.
		}
		else
		{	
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3262));		//장주가 공격측의 협박에 항복하였습니다.
		}
	}

	// npc animation
	if(g_LastNPCScriptOwner)
	{
		if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_CASTLECAPTAIN)
		{
			g_LastNPCScriptOwner->SetSurrenderAction();
		}
		else
		{
			int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
			if(index >= 0)
			{
				if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
						g_Npclist.m_pNPCObjectArray[index].SetSurrenderAction();
			}
		}
	}
	else
	{
		int index = g_Npclist.GetNPCIndexByNPCJob(_XNPC_JOBCLASS_CASTLECAPTAIN);
		if(index >= 0)
		{
			if(g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum)
					g_Npclist.m_pNPCObjectArray[index].SetSurrenderAction();
		}
	}

	InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

	_XLog(messagestr);
}

void _XNetwork::MSGCBInfoCurrentInfo(MSG_CB_INFO_CURRENT_INFO* info)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_INFO_CURRENT_INFO");
	_XDWINPRINT("[CB/Network] CurrentStatus : %d", info->ucCurrStatus);
	_XDWINPRINT("[CB/Network] ElapsedTime : %d", info->usElapsedTime);
	_XDWINPRINT("[CB/Network] Defender : %d/%d/%s", info->Defender.ucType, info->Defender.iIndex, info->Defender.cGroupName);

	_XLog("RP : CB Current Info");
	_XLog("CB : elapsed time : %d", info->usElapsedTime);
	_XLog("CB : Defender : %s", info->Defender.cGroupName);

	for(int i = 0 ; i < info->ucAttackerCount ; ++i)
	{
		_XDWINPRINT("[CB/Network] Attacker : %d/%d/%s", info->Attackers[i].ucType, info->Attackers[i].iIndex, info->Attackers[i].cGroupName);
		_XLog("CB : Attacker : %s", info->Attackers[i].cGroupName);
	}

	// 장원전 중간에 참여 - 정보 세팅
	switch(info->ucCurrStatus)
	{
	case en_combat_status_none :
		{
			g_pLocalUser->m_bCastleBattle = false;
			g_CBManager.m_enWarState = _XEN_CB_NONE;
		}
		break;
	case en_combat_status_before :
		{
			g_pLocalUser->m_bCastleBattle = true;
			g_CBManager.m_enWarState = _XEN_CB_READY;
		}
		break;
	case en_combat_status_current :
		{
			g_pLocalUser->m_bCastleBattle = true;
			g_CBManager.m_enWarState = _XEN_CB_PROGRESS;
		}
		break;
	case en_combat_status_after :
		{
			g_pLocalUser->m_bCastleBattle = true;
			g_CBManager.m_enWarState = _XEN_CB_END;
		}
		break;
	}

	if(info->ucCurrStatus == en_combat_status_none)
		return;

	// 각 단계의 시작한 시간 = 현재 시간 - 경과한 시간
	g_CBManager.m_dwTimeFromServer = g_LocalSystemTime - info->usElapsedTime*1000;
	g_CBManager.ClearState();
	
	//장원전 시간표시 관련
	if(info->ucCurrStatus == en_combat_status_current)
	{
		_XWindow_CastleBattlePersonalHistory* pCastleBattlePersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
		if(pCastleBattlePersonalHistory_Window)
		{
			pCastleBattlePersonalHistory_Window->SetOwner(info->ucGroupType, info->iindex);
			pCastleBattlePersonalHistory_Window->ShowWindow(TRUE);
		}

#ifdef _XDEF_CASTLEBATTLE_RECORD
		// 장원 전적
		_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
		if(pCBRecord_Window)
		{
			pCBRecord_Window->DeleteAll();
			pCBRecord_Window->ShowWindow(TRUE);
		}
#endif
	}

	int mytype = 0;
	int myindex = 0;

	if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex >0)
	{
		mytype = 1;
		myindex = g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex;
	}
	else if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
	{
		mytype = 0;
		myindex = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex;
	}

	// attacker or defender
	if(info->Defender.ucType == mytype)
	{
		if(info->Defender.iIndex == myindex)
		{
			g_pLocalUser->m_dwAttackType = 2;		// defender
			g_pLocalUser->m_bCastleBattle = true;

			_XDWINPRINT("[CB/Network]MyInfo : 수성");
			_XLog("CB : 수성");

			// 적대관계 저장
			g_pLocalUser->m_nDefenderType = info->Defender.ucType;
			g_pLocalUser->m_nDefenderIndex = info->Defender.iIndex;

			memset(g_pLocalUser->m_nAttackerType, 0, sizeof(g_pLocalUser->m_nAttackerType));
			memset(g_pLocalUser->m_nAttackerIndex, 0, sizeof(g_pLocalUser->m_nAttackerIndex));
			for(int atkcnt = 0 ; atkcnt < info->ucAttackerCount ; ++atkcnt)
			{
				g_pLocalUser->m_nAttackerType[atkcnt] = info->Attackers[atkcnt].ucType;
				g_pLocalUser->m_nAttackerIndex[atkcnt] = info->Attackers[atkcnt].iIndex;
			}
		}
	}
	for(i = 0 ; i < info->ucAttackerCount ; ++i)
	{
		if(info->Attackers[i].ucType == mytype)
		{
			if(info->Attackers[i].iIndex == myindex)
			{
				g_pLocalUser->m_dwAttackType = 1;		// attacker
				g_pLocalUser->m_bCastleBattle = true;

				_XDWINPRINT("[CB/Network]MyInfo : 공성");
				_XLog("CB : 공성");

				// 적대관계 저장
				g_pLocalUser->m_nDefenderType = info->Defender.ucType;
				g_pLocalUser->m_nDefenderIndex = info->Defender.iIndex;

				memset(g_pLocalUser->m_nAttackerType, 0, sizeof(g_pLocalUser->m_nAttackerType));
				memset(g_pLocalUser->m_nAttackerIndex, 0, sizeof(g_pLocalUser->m_nAttackerIndex));
				for(int atkcnt = 0 ; atkcnt < info->ucAttackerCount ; ++atkcnt)
				{
					g_pLocalUser->m_nAttackerType[atkcnt] = info->Attackers[atkcnt].ucType;
					g_pLocalUser->m_nAttackerIndex[atkcnt] = info->Attackers[atkcnt].iIndex;
				}
				break;
			}
		}
	}

	if(g_pLocalUser->m_bCastleBattle)
	{
		// role
		if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex >0)
		{
			if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_)
			{
				g_pLocalUser->m_dwRoleInCastleBattle = 1;	// 총대장
				{
					_XDWINPRINT("[CB/Network]MyInfo : 총대장");
					_XLog("CB : 총대장");
				}
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
		{
			if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
			{
				g_pLocalUser->m_dwRoleInCastleBattle = 1;	// 총대장
				_XDWINPRINT("[CB/Network]MyInfo : 총대장");
				_XLog("CB : 총대장");
			}
		}
		g_pLocalUser->m_nCBMapIdInWar = g_CurrentZoneInfoIndex;
	}
}

BOOL _XNetwork::SendMSGCBEtcLoudChat(int type, int param1, int param2, int param3, int param4)
{
	// 외치기 처리
	_XDWINPRINT("SEND MESSAGE : MSG_CB_ETC_LOUD_CHAT");

	_XWindow_CastleBattleShout* pCBShout_Window = (_XWindow_CastleBattleShout*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_SHOUT);
	if(pCBShout_Window)
	{
		int size = sizeof(MSG_CB_ETC_LOUD_CHAT);
		MSG_CB_ETC_LOUD_CHAT chat;
		ZeroMemory(&chat, size);
		chat.header.sLength = size;
		chat.ucMessage = type;

		chat.ucMode = (unsigned char)param1;

		strncpy(chat.cCharName, g_pLocalUser->m_CharacterInfo.charactername, _XDEF_MAX_USERNAMESTRINGLENGTH-1);
		strncpy(chat.msg, pCBShout_Window->m_IMECastleBattleShout.GetText(), MAX_CHAT_LENGTH-1);

		pCBShout_Window->m_IMECastleBattleShout.ClearBuffer();

		return _XSendPacket_Encapsulate((char*)&chat);
	}
	return TRUE;
}

void _XNetwork::MSGCBEtcLoudChat(MSG_CB_ETC_LOUD_CHAT* chat)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_ETC_LOUD_CHAT");
	
	if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, chat->cCharName) == 0)
	{
		_XWindow_CastleBattleShout* pCastleBattleShout_Window = (_XWindow_CastleBattleShout*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_SHOUT);
		if(pCastleBattleShout_Window)
		{
			pCastleBattleShout_Window->m_nCurrentCount++;
		}
	}

	// 외치기 처리
	TCHAR chatmsg[128];
	memset(chatmsg, 0, sizeof(chatmsg));
	_snprintf(chatmsg, sizeof(chatmsg), "%s : ", chat->cCharName);
	strcat(chatmsg, chat->msg);

	_XInsertCenterSystemMessage(chatmsg, D3DCOLOR_ARGB(255, 115, 223, 90));
}

void _XNetwork::MSGCBEtcCannotLoud(MSG_CB_ETC_CANNOT_LOUD* chat)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_ETC_CANNOT_LOUD");

	// 외치기 에러
	switch(chat->ucErrorCode)
	{
	case en_cb_loud_error_not_master :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3292), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//총대장이 아니므로 사용할 수 없습니다.
			_XDWINPRINT(_XGETINTERFACETEXT(ID_STRING_NEW_3292));
		}
		break;
	case en_cb_loud_error_over_count :
		{
			// 3293
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3293), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//사용할 수 있는 회수가 초과 되었습니다.
			_XDWINPRINT(_XGETINTERFACETEXT(ID_STRING_NEW_3293));
		}
		break;
	case en_cb_loud_error_not_time :
		{
			// 3294
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3294), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//사용할 수 있는 시간이 아닙니다.
			_XDWINPRINT(_XGETINTERFACETEXT(ID_STRING_NEW_3294));
		}
		break;
	default :
		{
			// 1585
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//"지금은 사용할 수 없습니다."
			_XDWINPRINT("지금은 사용할 수 없습니다.");
		}
		break;
	}
}

void _XNetwork::MSGCBEtcNotice(MSG_CB_ETC_NOTICE* notice)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_ETC_NOTICE");

	// 공지 사항 처리
	InsertChatString(notice->msg, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
}

void _XNetwork::MSGCBDeadPenalty(MSG_CB_DEAD_PENALTY* dead)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_DEAD_PENALTY");
	_XDWINPRINT("Penalty : %dsec", dead->usLeftCombatPenalryTime);

	g_pLocalUser->m_dwDieStartTime = g_LocalSystemTime;
	g_pLocalUser->m_dwDeadPenaltyTime = dead->usLeftCombatPenalryTime*1000;
	g_pLocalUser->m_dwDeadCount = dead->ucDeadCountInCombat;

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
	{
        //Author : 양희왕 //breif : 이거 추가..
        pRebirth_Window->m_bDeadInCB = TRUE;
		pRebirth_Window->SetCastleBattleState(TRUE);
	}
}

void _XNetwork::MSGCBThreatPointSet(MSG_CB_THREATPOINT_SET* point)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_THREATPOINT_SET");
	_XDWINPRINT("[Network/CB] Point : %d", point->usThreatPoint);

	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetPoint(point->usThreatPoint);
	}
}

void _XNetwork::MSGCBTaxSet(MSG_CB_TAX_SET* taxset)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_TAX_SET");
	_XDWINPRINT("[CB/Network] mapid:%d   taxtotal:%d  partmoney:%d", taxset->ucManorIndex, taxset->uiTotalMoney, taxset->uiPartMoney);

	g_CBManager.SetCastleTaxMoney(taxset->ucManorIndex, taxset->uiTotalMoney, taxset->uiPartMoney);
}

void _XNetwork::MSGCBTaxAnswerTake(MSG_CB_TAX_ANSWER_TAKE* answertake)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_TAX_ANSWER_TAKE");
	_XDWINPRINT("[CB/Network] result:%d   money(inven):%d  takemoney:%d", answertake->ucResult, answertake->uiLastMoney, answertake->uiTakeMoney);

#ifdef _XDEF_CASTLEBATTLE

	switch(answertake->ucResult)
	{
	case 0 :
		{
			// 세금 남은 돈은 다시 패킷이 따로 와서 처리된다.
			// 인벤토리 업데이트 해주면 됨.
//			g_CBManager.SetCastleTaxMoney(g_pLocalUser->m_nCastleMapId, answertake->uiLeftTotalMoney);

			DWORD getmoney = answertake->uiLastMoney - g_pLocalUser->m_Money;

			TCHAR msg[128];
			_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_NETWORK_GETMONEY), getmoney);		//%d금을 획득했습니다
			InsertChatString(msg, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);

			g_pLocalUser->m_Money = answertake->uiLastMoney;

		}
		break;
	case 1 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1761), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//"금액이 부족합니다."
		}
		break;
	case 2 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3397), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//"장주가 아닙니다."
		}
		break;
	case 3 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3398), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		//"소유한 장원이 없습니다."
		}
		break;
	}

	g_pGroup_Window->m_pBtnCastleBattleGetMoney->EnableWindow(TRUE);
#endif
}

void _XNetwork::MSGCBRecordCombatAdd(MSG_CB_RECORD_COMBAT_ADD* record)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_RECORD_COMBAT_ADD");
	_XLog("TEST LOG : MSGCBRecordCombatAdd in"); //Author : 양희왕

	_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
	if(pCBRecord_Window)
	{
		pCBRecord_Window->InsertRecord(record->_vsOrgRecord);
	}
}

void _XNetwork::MSGCBRecordPvpAward(MSG_CB_RECORD_PVP_AWARD* award)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_RECORD_PVP_AWARD");

	if(award->sRank == 0)
	{
		// 기여 보상 처리
		_XWindow_CBRewardSmall* pCBRewardSmall_Window = (_XWindow_CBRewardSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBREWARDSMALL);
		if(pCBRewardSmall_Window)
		{
			pCBRewardSmall_Window->SetAward(award);
			pCBRewardSmall_Window->ShowWindow(TRUE);
		}
	}
	else
	{
		// 10위권일 경우 MSG_CB_RECORD_WINNER_AWARD로 처리한다
		_XWindow_CastleBattleRewardList* pCBRewardList_Window = (_XWindow_CastleBattleRewardList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_REWARDLIST);
		if(pCBRewardList_Window)
		{
			pCBRewardList_Window->SetNormalAward(award);
		}
	}
}

void _XNetwork::MSGCBRecordRank(MSG_CB_RECORD_RANK* rank)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_RECORD_RANK");

	_XWindow_CBOrgRecord* pCBOrgRecord_Window = (_XWindow_CBOrgRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBORGRECORD);
	if(pCBOrgRecord_Window)
	{
		if(g_pLocalUser->m_bCastleBattle)	// 장원전 참가 중일때만 보이도록
		{
			pCBOrgRecord_Window->SetRank(rank->_rankList);
			pCBOrgRecord_Window->ShowWindow(TRUE);
		}
	}
}

void _XNetwork::MSGCBRecordWinnerAward(MSG_CB_RECORD_WINNER_AWARD* winner)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_RECORD_WINNER_AWARD");

	_XWindow_CastleBattleRewardList* pCBRewardList_Window = (_XWindow_CastleBattleRewardList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_REWARDLIST);
	if(pCBRewardList_Window)
	{
		pCBRewardList_Window->SetSpecialAward(winner);
		pCBRewardList_Window->ShowWindow(TRUE);
	}
}

void _XNetwork::MSGCBRecordInfoRes(MSG_CB_RECORD_INFO_RES* res)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_RECORD_INFO_RES");

	if(g_pGroup_Window)
	{
		g_pGroup_Window->m_nEntryCount	= res->usEntryCount;
		g_pGroup_Window->m_nWinCount	= res->usWinCount;
		g_pGroup_Window->m_nLoseCount	= res->usLoseCount;
		g_pGroup_Window->m_nCombatCount	= res->ucCombatCount;
		g_pGroup_Window->m_nWinRate		= res->usCumulationPercentage;
		g_pGroup_Window->m_nManorIndex	= res->usManorIndex;

		g_pGroup_Window->m_nYear		= res->lastManorCombatDay.uiYear;
		g_pGroup_Window->m_nMonth		= res->lastManorCombatDay.uiMonth;
		g_pGroup_Window->m_nDay			= res->lastManorCombatDay.uiDay;

		int year = g_pGroup_Window->m_nYear + 2000;
		int month = g_pGroup_Window->m_nMonth;
		int day = g_pGroup_Window->m_nDay;
		
		_snprintf(g_pGroup_Window->m_strLastWarDate, sizeof(g_pGroup_Window->m_strLastWarDate), g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2102,
			&year, &month, &day));

		g_pGroup_Window->SetRecordListItem(res->usPerOrgRecordCount, res->_vsOrgRecordList);
	}
}

void _XNetwork::MSGCBCurOwnerInfo(MSG_CB_CUR_OWNER_INFO* info)
{
	_XDWINPRINT("RECEIVE MESSAGE : MSG_CB_CUR_OWNER_INFO");

	if(info->ucMapIndex > 0 && info->ucMapIndex == g_CurrentZoneInfoIndex)
	{

		g_CBManager.SetScheduleList(info->ucMapIndex, &info->OwnerInfo, 0);
		g_CBManager.SetDrawOwnerMessage(TRUE);
		g_CBManager.SetOwnerMessageStartTime(g_LocalSystemTime);

		_XWindow_Info* pInfo_Window = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
		if(pInfo_Window)
		{
			pInfo_Window->ShowWindow(TRUE);
		}
	}
}