// XNetwork_Party.cpp : 동행 관련
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XMessageWindow.h"
#include "XStringItem.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"

#include "XWindow_RequestParty.h"
#include "XWindow_Party.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_TutorialIcon.h"

BOOL _XNetwork::Send_MSGParty(int type, int param1, int param2, int param3)
{
	switch(type)
	{
	case MSG_NO_PARTY_JOIN_REQ :
		{
			if( g_pLocalUser->m_Lock.partylock )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1670), TRUE);
				break;
			}
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Join Party ");
			
			int size = sizeof(MSG_PARTY_JOIN_REQ);
			MSG_PARTY_JOIN_REQ joinparty;
			ZeroMemory(&joinparty, size);
			joinparty.header.sLength = size;
			joinparty.ucMessage = MSG_NO_PARTY_JOIN_REQ;
			
			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				if(pRequestParty_Window->m_PickedCharacterName[0] != 0)
					strcpy(joinparty.cTo, pRequestParty_Window->m_PickedCharacterName);
			}
			
			g_pLocalUser->m_bPartyProgress = TRUE; // 2004.06.17->oneway48 insert : 요청을 보내면 무조건 진행중이다.
			return _XSendPacket_Encapsulate( (char*)&joinparty );
		}
		break;
		
	case MSG_NO_PARTY_ETC :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Party ETC ========================================");
			
			int size = sizeof(MSG_PARTY_ETC);
			MSG_PARTY_ETC partyetc;
			ZeroMemory(&partyetc, size);
			partyetc.header.sLength = size;
			partyetc.ucMessage = MSG_NO_PARTY_ETC;
			
			partyetc.cCmd = (char)param1;
			if( partyetc.cCmd == 9 )
			{
				partyetc._Flag.uFlag2 = (char)param2;
			}
			else if( partyetc.cCmd == 1 )
			{
				partyetc._Flag.uFlag1 = (char)param2;
				partyetc._Flag.uFlag2 = (char)param3;
			}
			else
			{
				partyetc.cInfo = (char)param2;
			}
			
			return _XSendPacket_Encapsulate( (char*)&partyetc );
		}
		break;
		
	case MSG_NO_PARTY_RESPONSE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Join Party ");
			
			int size = sizeof(MSG_PARTY_JOIN_RESPONSE);
			MSG_PARTY_JOIN_RESPONSE partyresponse;
			ZeroMemory(&partyresponse, size);
			partyresponse.header.sLength = size;
			partyresponse.ucMessage = MSG_NO_PARTY_RESPONSE;
			
			partyresponse.cCmd = (char)param1;			
			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				if( partyresponse.cCmd == 6 )
				{
					if(pRequestParty_Window->m_PickedCharacterName[0] != 0)
						strcpy(partyresponse.cToName, pRequestParty_Window->m_PickedCharacterName);
				}
				else
				{
					if(pRequestParty_Window->m_RequestCharacterName[0] != 0)
						strcpy(partyresponse.cToName, pRequestParty_Window->m_RequestCharacterName);
				}
			}
			
			return _XSendPacket_Encapsulate( (char*)&partyresponse );
		}
		break;
		
	}
	
	return TRUE;
}

void _XNetwork::MSGPartyJoinReq(MSG_PARTY_JOIN_REQ* joinparty)
{
	// 2004.06.17->oneway48 modify
	if( !g_bCommunityOptionRejectParty )
	{
		XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

		if( g_pLocalUser->m_Lock.partylock )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1670), TRUE);

			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
		else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
		{
			// 비무 중에는 동행 신청 거부
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1671), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		else if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_party))
		{
			// 비무 중에는 동행 신청 거부
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1671), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
#endif
		else if( g_pLocalUser->m_bPartyProgress )
		{		
			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
		else if( g_pLocalUser->m_bIsBossBattle )
		{
			// 보스전일 때에 파티 신청이 오면 거부한다.
			// 메신저 기능을 통해서 파티 신청이 올 수 있다.
			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
		else if( g_pLocalUser->m_bPartyQuestProgress)
		{
			// 파티 퀘스트 진행 중일 때에 파티 신청이 오면 거부한다.
			// 메신저 기능을 통해서 파티 신청이 올 수 있다.
			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
		else if(pProc_Maingame->m_bFunctionalObjectWideView)
		{
			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				if(joinparty->cTo[0] != 0)
					strcpy(pRequestParty_Window->m_PickedCharacterName, joinparty->cTo);
			}
			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
#ifdef _XTS_PK
		else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
		{
			// PK 중에는 동행 신청 거부
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2742), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"대전 중이므로 동행 요청을 거절하였습니다."
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			
			SendPacket(MSG_NO_PARTY_RESPONSE, 6); // 다른 작업 중임을 알린다.
		}
#endif
		else
		{
			if( g_pLocalUser->m_InParty )
			{
				SendPacket(MSG_NO_PARTY_RESPONSE, 3); // 이미 파티에 가입되어 있음을 알린다.
			}
			else
			{
				if(joinparty->cTo[0] != 0)
				{
					TCHAR tostring[256];
					memset(tostring, 0, sizeof(tostring));
					_snprintf(tostring, sizeof(tostring), _XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTPARTY), joinparty->cTo);
										
					_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
					if(pRequestParty_Window)
					{
						if(joinparty->cTo[0] != 0)
							strncpy(pRequestParty_Window->m_RequestCharacterName, joinparty->cTo, sizeof(pRequestParty_Window->m_RequestCharacterName));
					}
					
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tostring, TRUE, _XDEF_REQUESTPARTY_JOINPARTYOK, _XDEF_REQUESTPARTY_JOINPARTYNO);
					pMessageBox->StartTimer(10000, TRUE);
				}
			}
		}
	}
	else// 동행신청 거부 옵션에 켜 있을 때 
	{
		VIEWPACKETLOG
			_XDWPACKETLOG("SEND MESSAGE : Join Party ");
		
		int size = sizeof(MSG_PARTY_JOIN_RESPONSE);
		MSG_PARTY_JOIN_RESPONSE partyresponse;
		ZeroMemory(&partyresponse, size);
		partyresponse.header.sLength = size;
		partyresponse.ucMessage = MSG_NO_PARTY_RESPONSE;
		
		partyresponse.cCmd = (char)1;	// 1:거부
		strncpy(partyresponse.cToName, joinparty->cTo, sizeof(partyresponse.cToName));
		
		_XSendPacket_Encapsulate( (char*)&partyresponse );
	}
}

void _XNetwork::MSGPartyResponse(MSG_PARTY_JOIN_RESPONSE* partyresponse)
{
	g_pLocalUser->m_bPartyProgress = FALSE; // 2004.06.17->oneway48 insert 

	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);

	if(partyresponse->cToName[0] != 0)
	{		
		switch(partyresponse->cCmd)
		{
		case -1 :
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILPARTY));
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			break;
		case 1 :
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_REJECTPARTY), partyresponse->cToName);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			break;
		case 2 :
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_NOTARGET));
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			break;
		case 3 :
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1673));
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_OTHERPARTY), partyresponse->cToName);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			break;
		case 4 :
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_FULLPARTY));
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			break;
		case 5 :
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_REQUESTLATER, TRUE);
			}
			break;
		case 6 :
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_OTHERWORK), partyresponse->cToName);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			break;
		case 7 :
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1674));
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			break;
		default :
			break;
		}
	}
	else
	{		
		if( partyresponse->cCmd ==1 )
		{
			if( partyresponse->cToName[0] == 0 )	// 복면인
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_REJECTPARTY), _XGETINTERFACETEXT(ID_STRING_NEW_2659) );	// _T("복면인(覆面人)")
			}
			else
			{
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_REJECTPARTY), partyresponse->cToName);
			}
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
		}		
	}
}

void _XNetwork::PartyMemberUpdate(MSG_PARTY_MEMBER_UPDATE* memberupdate)
{
	switch(memberupdate->cCommand)
	{
	case 0 :
		{
			if(memberupdate->cCharacName[0] != 0)
			{
				strcpy(g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].playername, memberupdate->cCharacName);
				g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].serverno = memberupdate->cServerNo;
				g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].playeruniqueid = memberupdate->ucUID;
				
				_XUser* pUser = g_Userlist.FindUser(memberupdate->ucUID);
				if(pUser)
				{
					pUser->m_bInPartyMember = TRUE;
				}
				PartyCloseMiniBar();
				PartyShowMiniBar();

				// 업데이트 되는 정보를 적용한다.
				if( strcmp(g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].playername, memberupdate->cCharacName ) != 0 )
				{
					// 파티가 해체되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
					g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();
				}

			}
		}
		break;
	case 1 :
		{
			// 서버 이동
			g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].serverno = memberupdate->cServerNo;
			g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].playeruniqueid = memberupdate->ucUID;
			g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].vitalpercent = 0;
			g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].zenpercent = 0;
			
			
			if( g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].isLeader )
			{
				_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
				if(pMiniBar_Window)
				{
					BOOL isCurrentUser = FALSE;
					pMiniBar_Window->SetPartyMasterVital(memberupdate->cMemberNo, isCurrentUser);
					pMiniBar_Window->SetZoneIndex( g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].serverno );
					pMiniBar_Window->m_PlayerUniqueid = g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].playeruniqueid;
				}
				

			}
			else
			{
				int partymembercount = 0 ;
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( !g_pLocalUser->m_PartyInfo[i].isLeader )
					{
						partymembercount++;
						if( i == memberupdate->cMemberNo ) break;
					}
				}
				
				DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + partymembercount;
				
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
				if(pMiniBar_Window)
				{
					BOOL isCurrentUser = FALSE;
					pMiniBar_Window->SetPartyPlayerVital(memberupdate->cMemberNo, isCurrentUser);
					pMiniBar_Window->SetZoneIndex( g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].serverno );
					pMiniBar_Window->m_PlayerUniqueid = g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].playeruniqueid;
				}
			}
			
			// 동일 맵에 동행이 있는지를 검사하는 조건식을 다시 검사한다.
			g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();
		}
		break;
#ifdef _XDEF_PARTY_NOTAVAILABLE
	case 2 :
		{
			// 자리비움 -> 정상
			g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].bOut = FALSE;

			if(g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].isLeader)
			{

			}
			else
			{
				int partymembercount = 0 ;
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( !g_pLocalUser->m_PartyInfo[i].isLeader )
					{
						partymembercount++;
						if( i == memberupdate->cMemberNo ) break;
					}
				}
				
				DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + partymembercount;
				
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
				if(pMiniBar_Window)
				{
					pMiniBar_Window->SetOut(false);
				}
			}
		}
		break;
	case 3 :
		{
			// 정상 -> 자리비움
			g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].bOut = TRUE;

			if(g_pLocalUser->m_PartyInfo[memberupdate->cMemberNo].isLeader)
			{

			}
			else
			{
				int partymembercount = 0 ;
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( !g_pLocalUser->m_PartyInfo[i].isLeader )
					{
						partymembercount++;
						if( i == memberupdate->cMemberNo ) break;
					}
				}
				
				DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + partymembercount;
				
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
				if(pMiniBar_Window)
				{
					pMiniBar_Window->SetOut(true);
				}
			}
		}
		break;
#endif
	}
}

void _XNetwork::PartyMemberInfo(MSG_PARTY_MEMBER_INFO* memberinfo)
{	
		
	g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].serverno = memberinfo->cServerNo;
	g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].posX = memberinfo->sPosX;
	g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].posZ = memberinfo->sPosZ;	
	g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].innerlevel = memberinfo->sInnerLevel;
	g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].vitalpercent = memberinfo->cVitalPercent;
	g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].zenpercent = memberinfo->cZenPercent;
	g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].isValid = TRUE;

	
	_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
	if(pMiniBar_Window)
	{
		if( pMiniBar_Window->m_PartyMasterMode == _XPARTYMASTERMODE_MAXIMUM ) 
		{
			if(g_pLocalUser->m_PartyInfo[memberinfo->cMemberNo].isLeader )
			{
				BOOL isCurrent = FALSE;
				pMiniBar_Window->SetPartyMasterVital(memberinfo->cMemberNo, isCurrent);
			}
			else
			{
				int partymembercount = 0 ;
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( !g_pLocalUser->m_PartyInfo[i].isLeader )
					{
						partymembercount++;
						if( i == memberinfo->cMemberNo ) break;
					}
				}		
				
				DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + partymembercount;
				
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
				if(pMiniBar_Window)
				{
					BOOL isCurrent = FALSE;
					pMiniBar_Window->SetPartyPlayerVital(memberinfo->cMemberNo, isCurrent);
				}
			}
		}

	}
}

void _XNetwork::PartyETCProcess(MSG_PARTY_ETC* partyetc)
{
	switch(partyetc->cCmd)
	{
	case 0 :	// 파티원들에게 파티의 해체를 알린다.
		{
			for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
			{
				_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[i].playeruniqueid);
				if(pUser)
				{
					pUser->m_bInPartyMember = FALSE;
				}
				memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
				g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
				g_pLocalUser->m_PartyInfo[i].posX = 0;
				g_pLocalUser->m_PartyInfo[i].posZ = 0;
				g_pLocalUser->m_PartyInfo[i].serverno = 0;
				g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
				g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
				g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
				g_pLocalUser->m_PartyInfo[i].clan = 0;
				g_pLocalUser->m_PartyInfo[i].clanclass= 0;
				g_pLocalUser->m_PartyInfo[i].honor = 0;
				g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
				g_pLocalUser->m_PartyInfo[i].isValid = FALSE;				
			}
			g_pLocalUser->m_PartySelfIndex = -1;
			PartyCloseMiniBar();

			// 2004.05.25->oneway48 insert
			g_pLocalUser->m_InParty = FALSE;
			
			g_pLocalUser->m_PartyMemberCount = 0;
			// 파티퀘스트가 진행 중이면 취소시킨다.
			if( g_pLocalUser->m_bPartyQuestProgress )
			{
				g_QuestScriptManager.DeletePartyQuest();
			}
			
			// 파티가 해체되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
			g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();

			_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
			if( pChatDefault_Window ) pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503			
			if( pChatDefault_Window )
			{
				if( pChatDefault_Window->m_pChatFilterButton[2] )
				{
					pChatDefault_Window->m_pChatFilterButton[2]->EnableWindow( FALSE );
				}	
			}
#endif

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1675), TRUE);
			// insert end
		}
		break;
	case 1 :	// S->C 경험치 분배 방식이 바뀌었을 때, 파티원들에게 알림
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
			if(pMiniBar_Window)
			{
				if( partyetc->_Flag.uFlag1 == 0 )// 경험치 분배 
				{
					pMiniBar_Window->m_indexPartyExperienceSharing = partyetc->_Flag.uFlag2 + 1;
				}
				else// 아이템 분배 
				{
					pMiniBar_Window->m_indexPartyItemSharing = partyetc->_Flag.uFlag2 + 1;
				}
				//pMiniBar_Window->m_indexPartyExperienceSharing = partyetc->cInfo + 1;
			}
		}
		break;
	case 2 :
		{
			// 서버측 파티 이상으로 해체
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_DISSOLUTION), TRUE);
			
			for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
			{
				_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[i].playeruniqueid);
				if(pUser)
				{
					pUser->m_bInPartyMember = FALSE;
				}
				memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
				g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
				g_pLocalUser->m_PartyInfo[i].posX = 0;
				g_pLocalUser->m_PartyInfo[i].posZ = 0;
				g_pLocalUser->m_PartyInfo[i].serverno = 0;
				g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
				g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
				g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
				g_pLocalUser->m_PartyInfo[i].clan = 0;
				g_pLocalUser->m_PartyInfo[i].clanclass= 0;
				g_pLocalUser->m_PartyInfo[i].honor = 0;
				g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
				g_pLocalUser->m_PartyInfo[i].isValid = FALSE;
			}
			g_pLocalUser->m_PartySelfIndex = -1;
			PartyCloseMiniBar();

			g_pLocalUser->m_InParty = FALSE;	
			
			g_pLocalUser->m_PartyMemberCount = 0;	
			// 파티퀘스트가 진행 중이면 취소시킨다.
			if( g_pLocalUser->m_bPartyQuestProgress )
			{
				g_QuestScriptManager.DeletePartyQuest();
			}		
			
			// 파티가 해체되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
			g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();
			
			_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
			if( pChatDefault_Window ) pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503			
			if( pChatDefault_Window )
			{
				if( pChatDefault_Window->m_pChatFilterButton[2] )
				{
					pChatDefault_Window->m_pChatFilterButton[2]->EnableWindow( FALSE );
				}	
			}
#endif
			
		}
		break;
	case 4 : 
		{
			// 파티장에 의해 추방
			TCHAR		noticestring[256];

			if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername) == 0)
			{
				for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
				{
					_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[i].playeruniqueid);
					if(pUser)
					{
						pUser->m_bInPartyMember = FALSE;
					}
					memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
					g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
					g_pLocalUser->m_PartyInfo[i].posX = 0;
					g_pLocalUser->m_PartyInfo[i].posZ = 0;
					g_pLocalUser->m_PartyInfo[i].serverno = 0;
					g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
					g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
					g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
					g_pLocalUser->m_PartyInfo[i].clan = 0;
					g_pLocalUser->m_PartyInfo[i].clanclass= 0;
					g_pLocalUser->m_PartyInfo[i].honor = 0;
					g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
					g_pLocalUser->m_PartyInfo[i].isValid = FALSE;
				}
				g_pLocalUser->m_PartySelfIndex = -1;
				PartyCloseMiniBar();
				
				g_pLocalUser->m_InParty = FALSE;
				
				g_pLocalUser->m_PartyMemberCount = 0;		
				// 파티퀘스트가 진행 중이면 취소시킨다.
				if( g_pLocalUser->m_bPartyQuestProgress )
				{
					g_QuestScriptManager.DeletePartyQuest();
				}	

				// 파티가 해체되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
				g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();
				
				_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
				if( pChatDefault_Window ) pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503			
				if( pChatDefault_Window )
				{
					if( pChatDefault_Window->m_pChatFilterButton[2] )
					{
						pChatDefault_Window->m_pChatFilterButton[2]->EnableWindow( FALSE );
					}	
				}
#endif

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1676), TRUE);

			}
			else
			{
				if(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername[0] == 0)
				{
					break;
				}

				_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playeruniqueid);
				if(pUser)
				{
					pUser->m_bInPartyMember = FALSE;
				}

				_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_EXPULTIONOTHER), g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername);
				InsertChatString(noticestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				
				memset(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername, 0, sizeof(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername));
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].posX = 0;
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].posZ = 0;
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].serverno = 0;
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].vitalpercent = 0;
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].zenpercent = 0;
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].innerlevel = 0;
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].isLeader = FALSE;
				g_pLocalUser->m_PartyInfo[partyetc->cInfo].isValid = FALSE;
				
				PartyCloseMiniBar();
				PartyShowMiniBar();
				
				// 파티 인원수가 변경되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
				g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();
			}			
		}
		break;
	case 5 :	// S->C cInfo = 멤버번호 : 탈퇴 하였음.
		{
			// 탈퇴 요청 허가
			if(strcmp(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername, g_pLocalUser->m_CharacterInfo.charactername) == 0)
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_SECESSION), TRUE);
				
				for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
				{
					_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[i].playeruniqueid);
					if(pUser)
					{
						pUser->m_bInPartyMember = FALSE;
					}
					memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(g_pLocalUser->m_PartyInfo[i].playername));
					g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
					g_pLocalUser->m_PartyInfo[i].posX = 0;
					g_pLocalUser->m_PartyInfo[i].posZ = 0;
					g_pLocalUser->m_PartyInfo[i].serverno = 0;
					g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
					g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
					g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
					g_pLocalUser->m_PartyInfo[i].clan = 0;
					g_pLocalUser->m_PartyInfo[i].clanclass= 0;
					g_pLocalUser->m_PartyInfo[i].honor = 0;
					g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
					g_pLocalUser->m_PartyInfo[i].isValid = FALSE;
				}
				g_pLocalUser->m_PartySelfIndex = -1;
				PartyCloseMiniBar();

				g_pLocalUser->m_InParty = FALSE;
				
				g_pLocalUser->m_PartyMemberCount = 0;
				// 파티퀘스트가 진행 중이면 취소시킨다.
				if( g_pLocalUser->m_bPartyQuestProgress )
				{
					g_QuestScriptManager.DeletePartyQuest();
				}
				
				// 파티가 해체되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
				g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();

				_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
				if( pChatDefault_Window ) pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503			
				if( pChatDefault_Window )
				{
					if( pChatDefault_Window->m_pChatFilterButton[2] )
					{
						pChatDefault_Window->m_pChatFilterButton[2]->EnableWindow( FALSE );
					}	
				}
#endif
			}
			else
			{
				if( g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername[0] == 0 )
				{
					break;
				}


				// 2004.05.25->oneway48 insert : 탈퇴가 파티장이면 위임, 아니면 다음 번 사람이 파티장-> 현재는 파티 해체로 처리
				if( g_pLocalUser->m_PartyInfo[partyetc->cInfo].isLeader )
				{
					
					TCHAR noticestring[512];
					memset(noticestring, 0, sizeof(noticestring));
					_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1677), g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername);
					InsertChatString(noticestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					
					g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 0, 0 ); // 2004.05.31->oneway48 insert

					for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
					{
						_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[i].playeruniqueid);
						if(pUser)
						{
							pUser->m_bInPartyMember = FALSE;
						}
						memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(g_pLocalUser->m_PartyInfo[i].playername));
						g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
						g_pLocalUser->m_PartyInfo[i].posX = 0;
						g_pLocalUser->m_PartyInfo[i].posZ = 0;
						g_pLocalUser->m_PartyInfo[i].serverno = 0;
						g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
						g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
						g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
						g_pLocalUser->m_PartyInfo[i].clan = 0;
						g_pLocalUser->m_PartyInfo[i].clanclass= 0;
						g_pLocalUser->m_PartyInfo[i].honor = 0;
						g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
						g_pLocalUser->m_PartyInfo[i].isValid = FALSE;						
					}
					PartyCloseMiniBar();

					g_pLocalUser->m_InParty = FALSE;
					g_pLocalUser->m_PartyMemberCount = 0;
					// 파티퀘스트가 진행 중이면 취소시킨다.
					if( g_pLocalUser->m_bPartyQuestProgress )
					{
						g_QuestScriptManager.DeletePartyQuest();
					}
					
					// 파티가 해체되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
					g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();

					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
					if( pChatDefault_Window ) pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503			
					if( pChatDefault_Window )
					{
						if( pChatDefault_Window->m_pChatFilterButton[2] )
						{
							pChatDefault_Window->m_pChatFilterButton[2]->EnableWindow( FALSE );
						}	
					}
#endif

					// insert end
				}
				else
				{
					_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playeruniqueid);
					if(pUser)
					{
						pUser->m_bInPartyMember = FALSE;
					}
					
					TCHAR noticestring[256];
					memset(noticestring, 0, sizeof(noticestring));
					_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_SECESSIONOTHER), g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername);
					InsertChatString(noticestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					
					memset(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername, 0, sizeof(g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername));
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].posX = 0;
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].posZ = 0;
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].serverno = 0;
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].vitalpercent = 0;
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].zenpercent = 0;
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].innerlevel = 0;
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].isLeader = FALSE;
					g_pLocalUser->m_PartyInfo[partyetc->cInfo].isValid = FALSE;

					PartyCloseMiniBar();
					PartyShowMiniBar();
					
					// 파티 인원수가 변경되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
					g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();
				}
			}
						
		}
		break;
		
	case 6 : // S->C cInfo = 멤버번호. 파티장이 권한을 위임
		{
			for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
			{
				if( g_pLocalUser->m_PartyInfo[i].isLeader )
				{
					g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
					break;
				}
			}
			g_pLocalUser->m_PartyInfo[ partyetc->cInfo ].isLeader = TRUE;
			
			PartyCloseMiniBar();
			PartyShowMiniBar();
						
			// 파티장이 위임되면 파티 타입 퀘스트 시작 조건을 다시 검사한다.
			g_QuestScriptManager.RecheckPartyTypeQuestStartCondition();

			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1678), g_pLocalUser->m_PartyInfo[partyetc->cInfo].playername );			

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);			
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
			
		}		
		break;	
	case 8 : // S->C 서버 : 동장이 보스전 하자는데~
		{
			if( !g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
			{
				BOOL bShow = TRUE;
				_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BOSSMESSAGEWINDOW);
				if(pMessageBox)
				{
					if(pMessageBox->GetShowStatus())
					{
						bShow = FALSE;
					}
				}
				
				if(bShow)
				{
					//Author :  양희왕 //breif : 결에 들어갈 때 메시지 표시
					#if defined(_XTAIWANESE)
					if( g_pQuest_Window->m_GetTotalQuestCount >= 5 ) 
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3790), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //퀘스트 목록이 가득 차 있으면 별호 획득이 불가능 합니다.
					#endif

					pMessageBox = NULL;
					pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_BOSSMESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1679), TRUE, _XDEF_REQUESTPARTY_JOINBOSSBATTLE, _XDEF_REQUESTPARTY_REQUESTBOSSBATTLE);
					pMessageBox->StartTimer(10000, TRUE);
				}
			}
		}
		break;
	case 9 : // S->C 서버 : 동장아 나 보스전 참가해..^^
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
			{
				
				_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
				
				if( pEnterBossRoom_Window )
				{					
					for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
					{
						if( pEnterBossRoom_Window->m_EnterPartyMember[i].PartyMemberNo == -1)
						{
							bool alreadyresponse = false;
							for( int j = 0; j < _XDEF_MAXPARTYMEMBERCOUNT ; ++j )
							{
								if( pEnterBossRoom_Window->m_EnterPartyMember[j].PartyMemberNo == partyetc->_Flag.uFlag1 )
								{
									alreadyresponse = true;
									break;
								}
							}
							if( !alreadyresponse )
							{
								pEnterBossRoom_Window->m_EnterPartyMember[i].PartyMemberNo = partyetc->_Flag.uFlag1;
								pEnterBossRoom_Window->m_EnterPartyMember[i].bJoin = partyetc->_Flag.uFlag2;
								if( pEnterBossRoom_Window->m_EnterPartyMember[i].bJoin )
								{
									if( !pEnterBossRoom_Window->m_OKButton->GetShowStatus() )
									{
										pEnterBossRoom_Window->m_OKButton->ShowWindow(TRUE);
										_XPlayInterfaceSound(ID_SR_INTERFACE_BOSSBUTTON00_WAV);
									}
								}
							}
							break;
						}
					}		
				}
			}
		}
		break;
	case 10 : // S->C 서버 (동행원) : 동장이 보스전 들어가기를 취소했음을 동행원들에게 알림..
		{
			
			//    info : 0,진입으로 인한 다이얼로그 박스 소멸  , 1, 동행장이 진입을 취소함버튼취소
			if( partyetc->cInfo == 0 )
			{
				g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_BOSSMESSAGEWINDOW);
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1680), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else
			{				
				if( !g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				{
					g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_BOSSMESSAGEWINDOW);
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKPARTY_1681), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
			}
		}
		break;
	default :
		break;
	}
}

void _XNetwork::PartyInfoProcess(MSG_PARTY_INFO* partyinfo)
{
	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_020 )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_020);
			g_TutorialInfo->_XTutorial_020 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif			
		}
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=
	PartyCloseMiniBar();
	
	bool checkselfindex = false;
	g_pLocalUser->m_PartySelfIndex = -1;
	for( int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
	{
		
		if(partyinfo->Member[i].cMemberName[0] != 0)
		{
			// 파티중에서 자기 정보가 몇 번째에 있는지 검사한다. 
			if( !checkselfindex )
			{
				if( strcmp(g_pLocalUser->m_CharacterInfo.charactername, partyinfo->Member[i].cMemberName ) == 0)
				{
					checkselfindex = true;
					g_pLocalUser->m_PartySelfIndex = i;	// 파티 리스트 배열에서 자신의 인덱스를 가지고 있는다.
				}
			}

			// 기존이랑 이름이 다르면 정보를 바꾼다. 
			if( strcmp(g_pLocalUser->m_PartyInfo[i].playername, partyinfo->Member[i].cMemberName ) != 0 )
			{				
				memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(g_pLocalUser->m_PartyInfo[i].playername));
				g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
				g_pLocalUser->m_PartyInfo[i].posX = 0;
				g_pLocalUser->m_PartyInfo[i].posZ = 0;
				g_pLocalUser->m_PartyInfo[i].serverno = 0;
				g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
				g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
				g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
				g_pLocalUser->m_PartyInfo[i].clan = 0;
				g_pLocalUser->m_PartyInfo[i].clanclass= 0;
				g_pLocalUser->m_PartyInfo[i].honor = 0;
				g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
				g_pLocalUser->m_PartyInfo[i].isValid = FALSE;
				
				if(partyinfo->Member[i].cMemberName)
					strcpy(g_pLocalUser->m_PartyInfo[i].playername, partyinfo->Member[i].cMemberName);	

				g_pLocalUser->m_PartyInfo[i].serverno = partyinfo->Member[i].cServerNo;
				g_pLocalUser->m_PartyInfo[i].playeruniqueid = partyinfo->Member[i].usUID;

				// 거리이탈, 자리비움 초기화
				PartyResetMiniBar(partyinfo->cCaptainNo, i);	
			}
			else if( partyinfo->Member[i].cServerNo != 0 )
			{
				g_pLocalUser->m_PartyInfo[i].serverno = partyinfo->Member[i].cServerNo;
			}
			
#ifdef _XTS_PARTYMOREINFO
			// 파티 정보를 갱신한다. ( 문파, 역할, 명성, 레벨 )
			g_pLocalUser->m_PartyInfo[i].innerlevel = partyinfo->Member[i].sInnerLevel;
			g_pLocalUser->m_PartyInfo[i].clan = partyinfo->Member[i].cMoonpa;
			g_pLocalUser->m_PartyInfo[i].clanclass= partyinfo->Member[i].cClass;
			g_pLocalUser->m_PartyInfo[i].honor = partyinfo->Member[i].iHonor;
#endif

#ifdef _XDEF_PARTY_NOTAVAILABLE
            //Author : 양희왕 //breif : 2차 직책 정보
            g_pLocalUser->m_PartyInfo[i].nClassGrade = partyinfo->Member[i].ucClassGrade;
			// 자리비움 정보
			if(partyinfo->Member[i].ucGameState == 0)
				g_pLocalUser->m_PartyInfo[i].bOut = false;
			else
				g_pLocalUser->m_PartyInfo[i].bOut = true;
#endif
			if( i == partyinfo->cCaptainNo )
			{				
				g_pLocalUser->m_PartyInfo[i].isLeader = TRUE;
				_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
				if(pMiniBar_Window)
				{				
					pMiniBar_Window->m_indexPartyExperienceSharing = partyinfo->cShareExp + 1;	
					pMiniBar_Window->m_indexPartyItemSharing = partyinfo->cShareItem + 1;
				}
			}

			if( g_pLocalUser->m_PartySelfIndex != i )
			{				
				_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[i].playeruniqueid);
				if(pUser)
				{
					pUser->m_bInPartyMember = TRUE;
				}
			}
		}
		else
		{
			memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(g_pLocalUser->m_PartyInfo[i].playername));
			g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
			g_pLocalUser->m_PartyInfo[i].posX = 0;
			g_pLocalUser->m_PartyInfo[i].posZ = 0;
			g_pLocalUser->m_PartyInfo[i].serverno = 0;
			g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
			g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
			g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
			g_pLocalUser->m_PartyInfo[i].clan = 0;
			g_pLocalUser->m_PartyInfo[i].clanclass= 0;
			g_pLocalUser->m_PartyInfo[i].honor = 0;
			g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
			g_pLocalUser->m_PartyInfo[i].isValid = FALSE;
			g_pLocalUser->m_PartyInfo[i].bOut = FALSE;
			g_pLocalUser->m_PartyInfo[i].bFarAway = FALSE;

			// 거리이탈, 자리비움 초기화
			PartyResetMiniBar(partyinfo->cCaptainNo, i);
		}
	}

	PartyShowMiniBar();
	
	g_pLocalUser->m_bPartyProgress = FALSE; // 파티에 관련된 진행이 끝났음을 알려준다.
	g_pLocalUser->m_InParty = TRUE;

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503			
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

	if( pChatDefault_Window )
	{
		if( pChatDefault_Window->m_pChatFilterButton[2] )
		{
			pChatDefault_Window->m_pChatFilterButton[2]->EnableWindow( TRUE );
		}	
	}
#endif

	if( g_pLocalUser->m_IsServerMoveInParty ) g_pLocalUser->m_IsServerMoveInParty = FALSE;
}

void _XNetwork::PartyCloseMiniBar(void)
{
	for( int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
	{						
		DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + i;
		if( windowid == _XDEF_WTITLE_MINIPARTY_MEMBER1 )
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
			if(pMiniBar_Window)
			{
				if(pMiniBar_Window->GetShowStatus())
				{
					pMiniBar_Window->ShowWindow(FALSE);
					pMiniBar_Window->m_PlayerUniqueid = 0;
				}
			}
		}
		else
		{
			_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
			if(pMiniBar_Window)
			{
				if(pMiniBar_Window->GetShowStatus())
				{
					pMiniBar_Window->ShowWindow(FALSE);
					pMiniBar_Window->m_PlayerUniqueid = 0;
				}
			}
		}
	}
	
}

void _XNetwork::PartyResetMiniBar(char cCaptainNo, int index)
{
	if(cCaptainNo == index)
	{
		_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
		if(pMiniBar_Window)
		{
			pMiniBar_Window->Reset();				
		}
	}
	else
	{
		int partymembercount = 0 ;
		for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
		{
			if(cCaptainNo != i)
			{
				partymembercount++;
				if(i == index) break;
			}
		}
		
		DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + partymembercount;
		_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
		if(pMiniBar_Window)
		{
			pMiniBar_Window->Reset();
		}
	}
}

void _XNetwork::PartyShowMiniBar(void)
{
	
	_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
	if(pMiniBar_Window)
	{		
		if( pMiniBar_Window->m_PartyMasterMode == _XPARTYMASTERMODE_MAXIMUM ) 
		{

			g_pLocalUser->m_PartyMemberCount = 0;

			//Author : 양희왕 //brief : 변경
			int ypos = 0;
		
		#ifdef _ACCLAIM_RUBICONADSYSTEM	
			if(g_ViewRubiconAD)
				ypos = g_RubiconADWindowSize.cy;
		#endif

			int count = 0;
			for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i) 
			{
				if( g_pLocalUser->m_PartyInfo[i].isLeader )
				{
					if(g_pLocalUser->m_PartyInfo[i].playername[0] != 0)
					{
						BOOL isCurrentUser = FALSE;
						pMiniBar_Window->SetMode(_XPARTYMASTERMODE_MAXIMUM);
						pMiniBar_Window->SetPartyMasterName(i, isCurrentUser);
						pMiniBar_Window->SetPartyMasterVital(i, isCurrentUser);
						pMiniBar_Window->SetZoneIndex( g_pLocalUser->m_PartyInfo[i].serverno );
						pMiniBar_Window->m_PlayerUniqueid = g_pLocalUser->m_PartyInfo[i].playeruniqueid;
						
					#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
						#ifdef _ACCLAIM_RUBICONADSYSTEM	
							pMiniBar_Window->MoveWindow( 50, 114+((g_ViewRubiconAD)?g_RubiconADWindowSize.cy:0));
						#endif
					#else						
						#ifdef _ACCLAIM_RUBICONADSYSTEM	
							pMiniBar_Window->MoveWindow( 14, 114+((g_ViewRubiconAD)?g_RubiconADWindowSize.cy:0));
						#endif
					#endif
						pMiniBar_Window->ShowWindow(TRUE);
#ifdef _XTS_TARGET_STATE
						pMiniBar_Window->SetPartyUserStateImage();
#endif
#ifdef _XDEF_PARTY_NOTAVAILABLE
						pMiniBar_Window->SetOut(g_pLocalUser->m_PartyInfo[i].bOut);
#endif
						g_pLocalUser->m_PartyMemberCount++;
					}
				}
				else
				{
					DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + count;
					count++;
					_XWindow_PartyMemberMiniBar* pMemberMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
					if(pMemberMiniBar_Window)
					{
						
						if(g_pLocalUser->m_PartyInfo[i].playername[0] != 0)
						{
							BOOL isCurrentUser = FALSE;
							pMemberMiniBar_Window->SetPartyPlayerName(i, isCurrentUser);
							pMemberMiniBar_Window->SetPartyPlayerVital(i, isCurrentUser);
							pMemberMiniBar_Window->SetZoneIndex( g_pLocalUser->m_PartyInfo[i].serverno );
						#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
							pMemberMiniBar_Window->MoveWindow( 50, 170 + ypos);
						#else
							pMemberMiniBar_Window->MoveWindow( 14, 170 + ypos);
						#endif
							ypos += 36;	
							
							//if( checkleader )
							if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
							{
								pMemberMiniBar_Window->m_btnPartyMasterEntrust->EnableWindow( TRUE );
							}
							else
							{
								pMemberMiniBar_Window->m_btnPartyMasterEntrust->EnableWindow( FALSE );
							}
							pMemberMiniBar_Window->m_PlayerUniqueid = g_pLocalUser->m_PartyInfo[i].playeruniqueid;
							pMemberMiniBar_Window->ShowWindow(TRUE);

#ifdef _XTS_TARGET_STATE
							pMemberMiniBar_Window->SetPartyUserStateImage();
#endif
#ifdef _XDEF_PARTY_NOTAVAILABLE
							pMemberMiniBar_Window->SetOut(g_pLocalUser->m_PartyInfo[i].bOut);
#endif
							g_pLocalUser->m_PartyMemberCount++;
						}
					}
				}
			}
		}
		else
		{
			g_pLocalUser->m_PartyMemberCount = 0;
			int ypos = 0 ;			
			int count = 0;
			for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i) 
			{
				if( g_pLocalUser->m_PartyInfo[i].isLeader )
				{
					g_pLocalUser->m_PartyMemberCount++;					
				}
				else
				{
					
					DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + count;
					count++;
					_XWindow_PartyMemberMiniBar* pMemberMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
					if(pMemberMiniBar_Window)
					{						
						pMemberMiniBar_Window->m_btnPartyMasterEntrust->EnableWindow( FALSE );
						pMemberMiniBar_Window->ShowWindow(FALSE);
						
						if(g_pLocalUser->m_PartyInfo[i].playeruniqueid != 0 ) g_pLocalUser->m_PartyMemberCount++;						
					}
				}
			}
			pMiniBar_Window->SetMode(_XPARTYMASTERMODE_MINIMUM);
			pMiniBar_Window->ShowWindow(TRUE);
		}
	}

}
