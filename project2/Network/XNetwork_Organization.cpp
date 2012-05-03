#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XWindowObjectDefine.h"
#include "XNPC.h"
#include "xuser.h"
#include "XWindow_TutorialIcon.h"
#include "xmessagewindow.h"
#include "XSR_STRINGHEADER.H"

void _XNetwork::MSGOrganizationUnitUpdate(MSG_ORGANIZATION_UNIT_UPDATE_* unitupdate)
{
	if( g_pGroup_Window )	
	{
		switch( unitupdate->iCode ) 
		{
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_JOIN_ :
			g_pGroup_Window->JoinOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			break;
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_DROP_ :		
			g_pGroup_Window->DeleteOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			break;		
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_KICK_ :
			g_pGroup_Window->DeleteCompulsionOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			break;
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_ENTRUST_ :
			g_pGroup_Window->UpdateOrganizationEntrust(&unitupdate->_unit);
			break;
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_UPDATE_ :
			{
				g_pGroup_Window->UpdateOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			}
			break;
		}
	}
}

void _XNetwork::MSGOrganizationInfoResult(MSG_ORGANIZATION_INFO_RES* inforesult)
{
	if( inforesult->bCreateAble )	// 새로 창설
	{
		if( g_TutorialInfo )
		{
			if( !g_TutorialInfo->_XTutorial_045 )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_045);
				g_TutorialInfo->_XTutorial_045 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif				
			}
		}

		if( _strcmpi( g_pLocalUser->m_CharacterInfo.charactername , inforesult->_organizationBase.m_cCaptainName ) == 0 )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1651), TRUE);
		}
		else
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1652), inforesult->_organizationBase.m_cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);
		}		
	}
	if( g_pGroup_Window )
	{
		g_pGroup_Window->CopyBaseInfo( &inforesult->_organizationBase );
		g_pGroup_Window->CopyMemberList( inforesult->cMemberCount, inforesult->_memberList );
		g_pGroup_Window->CopyMemberOutTimeList( inforesult->cMemberCount, inforesult->outTimeList );
	}
}

void _XNetwork::MSGOrganizationResult(MSG_ORGANIZATION_RESULT* result)
{
	switch(result->iCode) 
	{
	case MSG_ORGANIZATION_RESULT::_OR_DESTROY_ :		// 조직이 삭제되었다..(ucFromMessage 와 상관 없음)
		{
			g_pGroup_Window->BreakUpOrganization();
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_ALREADY_INNER_ :				// 이미 조직에 속한자..
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1653), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_NEED_MORE_MEMBER_ :			// 초기 발기 인원이 부족함(5명 이상)
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR3), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_NOT_PARTY_JOINNER_ :			// 파티 상태가 아닙니다.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1654), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_CANT_PARTY_LEADER :			// 파티장만 가능합니다.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1655), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_UNLIKE_MOONPA :				// 다른 문파가 있습니다.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1656), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_UNLIKE_ZONE :				// 다른 지역에 있습니다.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR1), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_USING_NAME :					// 사용중인 이름입니다.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1657), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_ETC_ :						// 기타 예외적인 오류
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1658), TRUE);
		}		
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_FULL :
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1659), TRUE);	
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_NEED_MORE_LEVEL :
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1660), TRUE);	
		}
		break;
	}
}

void _XNetwork::MSGOrganizationClient( MSG_ORGANIZATION_CLIENT_* clientresult )
{
	switch(clientresult->iCode) 
	{
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_ :					// 타조직 짱이 자기네 조직에 들어오라고 권유
		{
			if(g_bCommunityOptionRefuseOrganization)
			{
				// 자동 거절
				SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_NO_);
			}
			else
			{
				TCHAR message[256];
				memset( message, 0, sizeof(message));
				_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1661), clientresult->cName );
				if( g_pGroup_Window )
					g_pGroup_Window->SetSendTargetName(clientresult->cName);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ORGANIZATION_INVITE_OK, _XDEF_GROUP_ORGANIZATION_INVITE_NO);
				pMessageBox->StartTimer(10000, TRUE);
			}
			
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_ :					// 니가 짱 먹으라고 권유..
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1662), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );
			if( g_pGroup_Window )
				g_pGroup_Window->SetSendTargetName(clientresult->cName);
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ORGANIZATION_ENTRUST_OK, _XDEF_GROUP_ORGANIZATION_ENTRUST_NO);
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_MOONPA_CHECK_ :				// 파티원들의 문파가 동일한지 검사한다.	
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_OK_ :					// 조직에 가입하겠다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1663), clientresult->cName, g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_NO_ :					// 조직에 가입하지 않겠다.		
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1664), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_OK_ :				// 짱 하겠다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1665), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_NO_ :				// 짱 안하겠다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1666), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_LOGOFF_ :					// 상대방은 로그오프중..
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1667), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ALREADY_INNER_ :				// 이미 다른조직에 몸담고 있다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1668), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_MOONPA_OK_ :					// 문파가 정상이다.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1669), TRUE, 
									_XDEF_GROUP_ORGANIZATION_CREATEYES, _XDEF_GROUP_ORGANIZATION_CREATENO);
		}
		break;	
	}
}

void _XNetwork::MSGOrganizationGrobalInfo(MSG_ORGANIZATION_GLOBAL_INFO* grobalinfo)
{
	if( grobalinfo->m_iIndex > 0 )
	{
		_XUSER_ORGANIZATION_GLOBAL_INFO* fineUserInfo = g_Userlist.SearchOrganizationInfo(grobalinfo->m_iIndex);

		if( fineUserInfo )
		{
			fineUserInfo->m_iIndex			= grobalinfo->m_iIndex;
			strcpy( fineUserInfo->m_cName, grobalinfo->m_cName );
			fineUserInfo->m_cProperty		= grobalinfo->m_cProperty;
			fineUserInfo->m_cBaseParty		= grobalinfo->m_cBaseParty;
			fineUserInfo->m_uiExperience	= grobalinfo->m_uiExperience;			
		}
		else
		{
			_XUSER_ORGANIZATION_GLOBAL_INFO* tempInfo = new  _XUSER_ORGANIZATION_GLOBAL_INFO;
			tempInfo->m_iIndex			= grobalinfo->m_iIndex;
			strcpy( tempInfo->m_cName, grobalinfo->m_cName );
			tempInfo->m_cProperty		= grobalinfo->m_cProperty;
			tempInfo->m_cBaseParty		= grobalinfo->m_cBaseParty;
			tempInfo->m_uiExperience	= grobalinfo->m_uiExperience;			
			g_Userlist.AddOrganizationInfo(tempInfo);
		}
	}	
}

void _XNetwork::MSGOrganizationNoticeInfo(MSG_ORGANIZATION_NOTICE* noticeinfo)
{
	if( g_pGroup_Window )	
	{
		g_pGroup_Window->m_NoticeText.reset();
		memcpy( g_pGroup_Window->m_NoticeText.writer, noticeinfo->m_NoticeData.writer, 13 );
		memcpy( g_pGroup_Window->m_NoticeText.writedate, noticeinfo->m_NoticeData.writedate, 15 );
		memcpy( g_pGroup_Window->m_NoticeText.notice, noticeinfo->m_NoticeData.notice, 272 );
		g_pGroup_Window->SelectNoticeMode(_XNOTICEMODE_READ);
	}
}



#ifdef _XTS_ALIANCE_20061018
//////////////////////////////////////////////////////////////////////////
// 동맹 관련 패킷.
//////////////////////////////////////////////////////////////////////////
BOOL _XNetwork::Send_Alience(int type, int param1, int param2, int param3, int param4)
{
	switch(type)
	{
	case MSG_NO_ALLY_BASE:
		{			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Alience Req");
			
			switch(param1) 
			{
			case en_ally_mode_request_ally:		// 동맹을 요청할 때,
				{
					int size = sizeof(MSG_ALLY_REQUEST_ALLY);
					MSG_ALLY_REQUEST_ALLY aliencereq;
					ZeroMemory(&aliencereq, size);
					
					aliencereq.header.sLength = size;
					aliencereq.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq.ucMode = (unsigned char)en_ally_mode_request_ally;
					
					//요청하는 사람의 유저 아이디
					aliencereq.usWhom = (unsigned short)param2;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq);
				}
				break;
			case en_ally_mode_response_yes:		// 동맹 권유를 받고 Yes로 응답할 때 
				{
					int size = sizeof(MSG_ALLY_MODE_RESPONSE_YES);
					MSG_ALLY_MODE_RESPONSE_YES alienceresponse_yes;
					ZeroMemory(&alienceresponse_yes, size);
					
					alienceresponse_yes.header.sLength = size;
					alienceresponse_yes.ucMessage = MSG_NO_ALLY_BASE;
					alienceresponse_yes.ucMode = (unsigned char)en_ally_mode_response_yes;
					
					return _XSendPacket_Encapsulate((char*)&alienceresponse_yes);
				}
				break;
			case en_ally_mode_response_no:		// 동맹 권유를 받고 No로 응답할 때 
				{
					int size = sizeof(MSG_ALLY_MODE_RESPONSE_NO);
					MSG_ALLY_MODE_RESPONSE_NO alienceresponse_no;
					ZeroMemory(&alienceresponse_no, size);
					
					alienceresponse_no.header.sLength = size;
					alienceresponse_no.ucMessage = MSG_NO_ALLY_BASE;
					alienceresponse_no.ucMode = (unsigned char)en_ally_mode_response_no;
					
					return _XSendPacket_Encapsulate((char*)&alienceresponse_no);
				}
				break;
			case en_ally_mode_request_ally_cancel:		// 동맹 권유를 보냈다가 응답을 받기전에 동맹 권유를 취소할 때
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_ALLY_CANCEL);
					MSG_ALLY_MODE_REQUEST_ALLY_CANCEL aliencereq_cancle;
					ZeroMemory(&aliencereq_cancle, size);
					
					aliencereq_cancle.header.sLength = size;
					aliencereq_cancle.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_cancle.ucMode = (unsigned char)en_ally_mode_request_ally_cancel;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_cancle);
				}
				break;								
			case en_ally_mode_request_ally_info:		// 동맹의 상세 정보 요청
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_ALLY_INFO);
					MSG_ALLY_MODE_REQUEST_ALLY_INFO aliencereq_info;
					ZeroMemory(&aliencereq_info, size);
					
					aliencereq_info.header.sLength = size;
					aliencereq_info.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_info.ucMode = (unsigned char)en_ally_mode_request_ally_info;
					
					aliencereq_info.iAllyIndex	= (int)param2;	// 요청하는 동맹의 인덱스.
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_info);
				}
				break;		
			case en_ally_mode_req_leave:		// 탈퇴 요청
				{
					int size = sizeof(MSG_ALLY_MODE_REQ_LEAVE);
					MSG_ALLY_MODE_REQ_LEAVE aliencereq_leave;
					ZeroMemory(&aliencereq_leave, size);
					
					aliencereq_leave.header.sLength = size;
					aliencereq_leave.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_leave.ucMode = (unsigned char)en_ally_mode_req_leave;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_leave);
				}
				break;
			case en_ally_mode_req_scatter:		// 해산 요청
				{
					int size = sizeof(MSG_ALLY_MODE_REQ_SCATTER);
					MSG_ALLY_MODE_REQ_SCATTER aliencereq_scatter;
					ZeroMemory(&aliencereq_scatter, size);
					
					aliencereq_scatter.header.sLength = size;
					aliencereq_scatter.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_scatter.ucMode = (unsigned char)en_ally_mode_req_scatter;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_scatter);
				}
				break;
			case en_ally_mode_request_banish:		// 추방 요청	
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_BANISH);
					MSG_ALLY_MODE_REQUEST_BANISH aliencereq_leave;
					ZeroMemory(&aliencereq_leave, size);
					
					aliencereq_leave.header.sLength = size;
					aliencereq_leave.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_leave.ucMode = (unsigned char)en_ally_mode_request_banish;
					
					//추방하려는 영웅단의 인덱스
					aliencereq_leave.iOrgIndex = (int)param2;

					return _XSendPacket_Encapsulate((char*)&aliencereq_leave);
				}
				break;
			case en_ally_mode_request_other_ally:		// 다른 동맹의 정보 요청	
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_OTHER_ALLY);
					MSG_ALLY_MODE_REQUEST_OTHER_ALLY aliencereq_otherinfo;
					ZeroMemory(&aliencereq_otherinfo, size);
					
					aliencereq_otherinfo.header.sLength = size;
					aliencereq_otherinfo.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_otherinfo.ucMode = (unsigned char)en_ally_mode_request_other_ally;
					
					// 동맹 인덱스
					aliencereq_otherinfo.iAllyIndex = (int)param2;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_otherinfo);
				}
				break;
			}
			
		}
		break;
	}
	
	return TRUE;
}

// 영웅단 동맹 요청이 왔다.
void _XNetwork::MSGAlienceRequestFrom(MSG_ALLY_REQEUST_FROM* alience_request_from)
{
//	u_short         usFrom;
//	u_char          ucType;   // 0 : 영웅단, 1 :동맹
//	char            cName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	if( alience_request_from->ucType == 0 )
	{
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3110), alience_request_from->cName );

		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		if( pMessageBox )	
		{			
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ALIENCE_BTN_INVITE_OK, _XDEF_GROUP_ALIENCE_BTN_INVITE_NO);
			pMessageBox->StartTimer(14000, TRUE);
		}

	}
	else if( alience_request_from->ucType == 1 )
	{
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3114), alience_request_from->cName );
		
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		if( pMessageBox )	
		{			
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ALIENCE_BTN_INVITE_OK, _XDEF_GROUP_ALIENCE_BTN_INVITE_NO);
			pMessageBox->StartTimer(14000, TRUE);
		}

	}
}

//권유의 응답으로 No
void _XNetwork::MSGAlienceRequestNo(MSG_ALLY_MODE_RESPONSE_NO* alience_request_no)
{
	TCHAR message[256];
	memset( message, 0, sizeof(message) );
	_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3417)); //"동맹 요청을 거부하였습니다."
	InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
	
}

// 영웅단 동맹이 만들어졌다.
void _XNetwork::MSGAlienceMade(MSG_ALLY_MODE_IS_MADE* alience_made)
{
	//  int				iAllyIndex;     // 만들어진 동맹의 인덱스.
	//	int				iOr1Index;		// 영웅단 동맹의 동맹주 영웅단 이름
	//	char			iOr1Name[_XDEF_ORGANIZATIONNAMELENGTH+1];
	//	
	//	int				iOr2Index;		// 영웅단 동맹에 가입하는 영웅단 이름
	//	char			iOr2Name[_XDEF_ORGANIZATIONNAMELENGTH+1];

	if( g_pGroup_Window )
	{
		// 가입하는 영웅단 일때 
		if( g_pGroup_Window->m_OrganizationBaseInfo.m_iIndex == alience_made->iOr2Index )
		{
			// 영웅단 동맹에 가입되었다는 것을 알린다.
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3115), alience_made->iOr1Name );			
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
		}
		else // 영웅단 동맹원일때 
		{
			// 가입하는 영웅단과 동맹을 맺었다는 것을 알린다.
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3113), alience_made->iOr2Name );			
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
		}
	}

}

//  자신의 영웅단이 동맹에 가입됬다.
void _XNetwork::MSGAlienceEnter(MSG_ALLY_MODE_ORG_ENTER_ALLY* alience_enter)
{
	
	//	// ucNoOrg 에 따라, 가변길이.ucNoOrg 로 잘라서 파싱할 것.
	//	int		iAllyIndex;		// 동맹 인덱스.
	//	char					cMasterName[13];	// 회주 이름
	//	u_char	ucNoOrg;
	//	__ally_client_info	info[5];               //
	//		
	if( g_pGroup_Window )
	{		
		//정보를 받기 전에 영웅단 정보를 지운다.
		//나중에 AlienceReset()이라는 함수를 만든다.
		g_pGroup_Window->ResetAlienceInfo();

		g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = g_pGroup_Window->m_UniqueIndexAlience = alience_enter->iAllyIndex;
		strcpy(g_pGroup_Window->m_AlienceMasterName, alience_enter->cMasterName );
		
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

		if( pChatDefault_Window->m_pChatFilterButton[3] )
		{
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 || g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( TRUE );
			}
			else
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( FALSE );
			}
		}
#endif
				

		for(int i =0; i < alience_enter->ucNoOrg; ++i) 
		{
			__ally_client_info* pAlienceMember = new __ally_client_info;
			pAlienceMember->iOrIndex = alience_enter->info[i].iOrIndex;
			strcpy(pAlienceMember->iOrName, alience_enter->info[i].iOrName );
			
			// 맨 처음이 방회 이름이다.
			if( i == 0 )
			{
				// 방회의 이름을 넣는다.
				strcpy(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName, alience_enter->info[i].iOrName );
				
				// 회주인지 검사한다.
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_enter->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
					{
						//회주이다.
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_;
					}
					else//기본 방회원
					{
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
					}
				}
			}
			else
			{		
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_enter->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_  )		
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
					else	
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
				}
			}
			
			g_pGroup_Window->m_listAlienceMember.push_back(pAlienceMember);
		}
		
		if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
			g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}
}

//  동맹의 모든 정보.
void _XNetwork::MSGAlienceAllInfo(MSG_ALLY_MODE_ALLY_ALL_INFO* alience_allinfo)
{
	
//	// ucNoOrg 에 따라, 가변길이.ucNoOrg 로 잘라서 파싱할 것.
//	int		iAllyIndex;		// 동맹 인덱스.
//	char					cMasterName[13];	// 회주 이름
//	u_char	ucNoOrg;
//	__ally_client_info	info[5];               //
//		
	if( g_pGroup_Window )
	{
		
		//정보를 받기 전에 영웅단 정보를 지운다.
		//나중에 AlienceReset()이라는 함수를 만든다.
		g_pGroup_Window->ResetAlienceInfo();

		g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = g_pGroup_Window->m_UniqueIndexAlience = alience_allinfo->iAllyIndex;
		
		strcpy(g_pGroup_Window->m_AlienceMasterName, alience_allinfo->cMasterName );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

		if( pChatDefault_Window->m_pChatFilterButton[3] )
		{
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 || g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( TRUE );
			}
			else
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( FALSE );
			}
		}
#endif

		for(int i =0; i < alience_allinfo->ucNoOrg; ++i) 
		{
			__ally_client_info* pAlienceMember = new __ally_client_info;
			pAlienceMember->iOrIndex = alience_allinfo->info[i].iOrIndex;
			strcpy(pAlienceMember->iOrName, alience_allinfo->info[i].iOrName );
			
			// 맨 처음이 방회 이름이다.
			if( i == 0 )
			{
				// 방회의 이름을 넣는다.
				strcpy(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName, alience_allinfo->info[i].iOrName );

				// 회주인지 검사한다.
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_allinfo->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
					{
						//회주이다.
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_;
					}
					else//기본 방회원
					{
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
					}
				}
			}
			else
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_allinfo->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_  )		
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
					else	
					g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
				}
			}

			g_pGroup_Window->m_listAlienceMember.push_back(pAlienceMember);
		}
		
		if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
			g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}
}

// 새로운 영웅단이 들어왔다.
void _XNetwork::MSGAlienceNewMember(MSG_ALLY_MODE_NOTICE_NEW_ORG* alience_newmember)
{
	//__ally_client_info	addInfo;              // 추가되는 영웅단의 정보.
	
	if( g_pGroup_Window )
	{
		__ally_client_info* pAlienceMember = new __ally_client_info;
		pAlienceMember->iOrIndex = alience_newmember->addInfo.iOrIndex;
		strcpy(pAlienceMember->iOrName, alience_newmember->addInfo.iOrName );
		
		g_pGroup_Window->m_listAlienceMember.push_back(pAlienceMember);
	}
}

// 영웅단이 탈퇴했다.
void _XNetwork::MSGAlienceLeaveMember(MSG_ALLY_MODE_NOTICE_LEAVE_ORG* alience_leavemember)
{
	//int	iLeaveOrgIndex;               // 탈퇴하는 영웅단의 인덱스.


	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_leavemember->iLeaveOrgIndex )
	{
		// 방회에서 탈퇴하였습니다.
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3122), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName );			
		InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

		g_pGroup_Window->ResetAlienceInfo();

		//Author : 양희왕 //breif: 창이 안켜져 있어도 갱신 //Date : 08/03/06
		//if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
		//	g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
		
	}
	else
	{
		__ally_client_info* aliencemember = NULL;
		list <__ally_client_info*>::iterator iter_alience;
		for(iter_alience = g_pGroup_Window->m_listAlienceMember.begin() ; iter_alience != g_pGroup_Window->m_listAlienceMember.end() ; )
		{
			aliencemember = *iter_alience;
			if( aliencemember )
			{
				if( aliencemember->iOrIndex == alience_leavemember->iLeaveOrgIndex )
				{				
					// 방회에서 영웅단이 탈퇴하였습니다.
					TCHAR message[256];
					memset( message, 0, sizeof(message) );
					_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3121), aliencemember->iOrName );			
					InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					
					SAFE_DELETE(aliencemember);
					iter_alience = g_pGroup_Window->m_listAlienceMember.erase(iter_alience);
					break;
				}
				else
				{
					++iter_alience;
				}
			}
		}

		//if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
		//	g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}

	g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
}

// 동맹이 해체되었다.
void _XNetwork::MSGAlienceScattered(MSG_ALLY_MODE_SCATTERED* alience_scattered)
{
	//u_char	ucScatterReason;            // 0 - 동맹주의 해체. 1 - 영웅단 탈퇴에 의한 해체.
	//	if( alience_scattered->ucScatterReason == 0 )
	//	{
	//	}
	//	else if( alience_scattered->ucScatterReason == 1 )
	//	{
	//	}
	//	TCHAR message[256];
	//	memset( message, 0, 256 );
	//	sprintf( message, _XGETINTERFACETEXT(ID_STRING_NEW_3123), );			
	//	InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
	
	//나중에 AlienceReset()이라는 함수를 만든다.
	if( g_pGroup_Window )
	{
		// 방회가 해체되었습니다.
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3123), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName );			
		InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		
		g_pGroup_Window->ResetAlienceInfo();
		if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
			g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}
}

// 탈퇴 요청 수락
void _XNetwork::MSGAlienceAcceptLeave(MSG_ALLY_MODE_ALLOW_LEAVE* alienceaccept_leave)
{	
}

// 해산 요청 수락
void _XNetwork::MSGAlienceAcceptScatter(MSG_ALLY_MODE_ALLOW_SCATTER* alienceaccept_scatter)
{	
}
// 추방되었다고, 동맹주에게 알려줌.
void _XNetwork::MSGAlienceLeaveResult(MSG_ALLY_MODE_BANISH_COMPLETE* alience_leaveresult)
{	
	//int		iOrgIndex;                      // 
}
// 다른 동맹의 정보	
void _XNetwork::MSGAlienceOtherInfo(MSG_ALLY_MODE_OTHER_ALLY_INFO * alience_otherinfor)
{	
	//int		iAllyIndex;
	//char	cAllyName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	//주변 인물의 방회 정보를 갱신하거나 추가한다.
	if( alience_otherinfor->iAllyIndex > 0 )
	{
		_XUSER_ALIENCE_GLOBAL_INFO* fineUserInfo = g_Userlist.SearchAlienceInfo(alience_otherinfor->iAllyIndex);
		
		if( fineUserInfo )
		{
			fineUserInfo->m_iIndex	= alience_otherinfor->iAllyIndex;
			strcpy( fineUserInfo->m_cMasterName, alience_otherinfor->cAllyMaster);	
			strcpy( fineUserInfo->m_cName, alience_otherinfor->cAllyName );	
		}
		else
		{
			_XUSER_ALIENCE_GLOBAL_INFO* tempInfo = new  _XUSER_ALIENCE_GLOBAL_INFO;
			tempInfo->m_iIndex	= alience_otherinfor->iAllyIndex;
			strcpy( tempInfo->m_cMasterName, alience_otherinfor->cAllyMaster);	
			strcpy( tempInfo->m_cName, alience_otherinfor->cAllyName );
			g_Userlist.AddAlienceInfo(tempInfo);
		}
	}
	
}
// 영웅단이 추방되었다.
void _XNetwork::MSGAlienceNoticeLeave(MSG_ALLY_MODE_NOTICE_BANISH_ORG* alience_noticeleave)
{	
	//int		iOrgIndex;
	//char	cOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];

}
// 동맹의 정보가 없다.
void _XNetwork::MSGAlienceNoInfo(MSG_ALLY_MODE_NO_ALLY_INFO* alience_noinfo)
{	
	//int	iAllyIndex;			// 해당 동맹의 정보가 없다. 
	TCHAR message[256];
	memset( message, 0, sizeof(message) );
	_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3175) );			
	InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
}

//영웅단 동맹에 관련된 에러메세지
void _XNetwork::MSGAlienceErrorMessage(MSG_ALLY_ERROR* alience_error)
{
	switch(alience_error->ucMode) {
	case en_ally_mode_error_we_already_ally :		
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3176) );			//이미 방회에 속해 있습니다
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_they_already_ally :		// 대상 영웅단은 이미 동맹을 맺고 있다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3177) );			//대상 영웅단은 이미 방회에 속해 있습니다.
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_cannot_ally :			// 흑/백도 의 이유로 동맹을 맺을 수 없다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3178) );			//흑도와 백도는 동맹을 맺을 수 없습니다
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_not_found_info :		// 요청한 동맹을 찾을 수 없습니다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3179) );			//요청한 방회를 찾을 수 없습니다.
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;		
	case en_ally_mode_error_cannot_find_user :		// 유저를 찾을 수 없다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3180) );			//유저를 찾을 수 없습니다.
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_i_am_busy :				// 현재 요청중..
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3181)  );			//_T("현재 방회 요청 중 입니다.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_busy :					// 대상이 다른 일 중.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3182) );			//_T("대상이 다른 일을 하고 있습니다.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_cannot_request :		// 영웅단이 없거나, 단주가 아니다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3183) );			//_T("영웅단에 소속되어 있지 않습니다.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_not_hero :				// 대상은 영웅단 단주가 아니다.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3184) );			//_T("영웅단 단주가 아닙니다.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_in_combat :
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3394) );	//"현재 장원전 신청 상태이므로 방회를 결성할 수 없습니다."
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_target_in_combat :
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3395) );	//"상대 영웅단은 현재 장원전 신청 상태이므로 방회를 결성할 수 없습니다."
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;

	}

}

#endif