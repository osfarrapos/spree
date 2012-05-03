#include "stdafx.h"
#include "XKernel.h"

#include "NMEventHandler.h"
#include "NMManager.h"
#include "NMCOClientObject.h"

void CNMEventHandler::HandleNMEvent( WPARAM wParam, LPARAM lParam )
{
	CNMEvent*	pEvent = CNMUtil::CreateEventFromCodeAndBuffer( (_UInt32_) wParam, (BYTE*) lParam );

	if ( pEvent )
	{
		if ( ( pEvent->GetCode() == CNMRefreshEvent::kCode
				&& ( ( CNMRefreshEvent* )pEvent )->uType == CNMRefreshEvent::kType_MyInfo )
			|| ( pEvent->GetCode() == CNMMessengerReplyEvent::kCode
				&& ( ( ( CNMMessengerReplyEvent* )pEvent )->nResultCode == kMessengerReplyOK
					|| ( ( CNMMessengerReplyEvent* )pEvent )->nResultCode == kMessengerReplyNewUser ) ) )
		{
			CNMCOClientObject::GetInstance().GetMyInfo();
		}
		else if ( pEvent->GetCode() == CNMGuildOnlineInfoExEvent::kCode )
		{
			CNMGuildOnlineInfoExEvent*	pGOIEvent;
			CNMCOClientObject::NMGOIList::iterator			
										iterBegin, iterEnd, iterCur;

			pGOIEvent	= (CNMGuildOnlineInfoExEvent*) pEvent;

			NMCSLOCK( CNMCOClientObject::GetInstance().m_cs )
			{
				iterBegin	= CNMCOClientObject::GetInstance().m_aGOIs.begin();
				iterEnd		= CNMCOClientObject::GetInstance().m_aGOIs.end();

				for ( iterCur = iterBegin; iterCur != iterEnd; ++iterCur )
				{
					if ( (*iterCur).uGuildKey == pGOIEvent->goiInfo.uGuildKey )
					{
						(*iterCur)	= pGOIEvent->goiInfo;
						break;
					}
				}

				if ( iterCur == iterEnd )
				{
					CNMCOClientObject::GetInstance().m_aGOIs.push_back( pGOIEvent->goiInfo );
				}
			}
		}
		else if ( pEvent->GetCode() == CNMMsgConnectionClosedEvent::kCode )
		{
			CNMCOClientObject::GetInstance().GetMyInfo();
		}
		else if ( pEvent->GetCode() == CNMAuthConnectionClosedEvent::kCode )
		{
			CNMCOClientObject::GetInstance().ResetMyInfo();
		}

		this->OnNMEvent( pEvent );
		delete pEvent;
	}

	CNMManager::GetInstance().MemFree( (LPVOID) lParam );
}

void CNMEventHandler::OnNMEvent( CNMEvent* pEvent )
{
	BEGIN_NMEVENT_MAP
		ON_NMEVENT( CNMMessengerReplyEvent,	OnMessengerReplyEvent )
		ON_NMEVENT( CNMAuthConnectionClosedEvent, OnAuthConnectionClosedEvent )
		ON_NMEVENT( CNMAuthSecondaryConnectionClosedEvent, OnAuthSecondaryConnectionClosedEvent )
		ON_NMEVENT( CNMMsgConnectionClosedEvent, OnMsgConnectionClosedEvent )
		ON_NMEVENT( CNMRefreshEvent, OnRefreshEvent )
		ON_NMEVENT( CNMSpecialEvent, OnSpecialEvent )
		ON_NMEVENT( CNMRequestNewFriendEvent, OnRequestNewFriendEvent )
		ON_NMEVENT( CNMServerMessageEvent, OnServerMessageEvent )
		ON_NMEVENT( CNMCustomMessageEvent, OnCustomMessageEvent )
		ON_NMEVENT( CNMNoteInstantMsgEvent, OnNoteInstantMsgEvent )
		ON_NMEVENT( CNMNoteInfoEvent, OnNoteInfoEvent )
		ON_NMEVENT( CNMRefreshMessageEvent, OnRefreshMessageEvent )
		ON_NMEVENT( CNMFindUserReplyEvent, OnFindUserReplyEvent )
		ON_NMEVENT( CNMInviteVirtualUserEvent, OnInviteVirtualUserEvent )
		ON_NMEVENT( CNMUserInfoEvent, OnUserInfoEvent )
		ON_NMEVENT( CNMGuildOnlineInfoExEvent, OnGuildOnlineInfoExEvent )
		ON_NMEVENT( CNMGuildChatMessageEvent, OnGuildChatMessageEvent )
		ON_NMEVENT( CNMFriendInfoChangedEvent, OnFriendInfoChangedEvent )
		ON_NMEVENT( CNMNotifyEvent, OnNotifyEvent )
		ON_NMEVENT( CNMCustomNotifyEvent, OnCustomNotifyEvent )
		ON_NMEVENT( CNMAttendanceEventEvent, OnAttendanceEventEvent )

		ON_NMEVENT( CNMCRChatRoomCreatedExEvent, OnCRChatRoomCreatedExEvent )
		ON_NMEVENT( CNMCRChatRoomEstablishedExEvent, OnCRChatRoomEstablishedExEvent )
		ON_NMEVENT( CNMCRChatRoomErrorEvent, OnCRChatRoomErrorEvent )
		ON_NMEVENT( CNMCRChatRoomMemberEvent, OnCRChatRoomMemberEvent )
		ON_NMEVENT( CNMCRChatRoomInfoExEvent, OnCRChatRoomInfoExEvent )
		ON_NMEVENT( CNMCRChatRoomMessageReceivedEvent, OnCRChatRoomMessageReceivedEvent )

		ON_NMEVENT( CNMGSSessionCreatedEvent, OnGSSessionCreatedEvent )
		ON_NMEVENT( CNMGSSessionEstablishedEvent, OnGSSessionEstablishedEvent )
		ON_NMEVENT( CNMGSSessionFailedEvent, OnGSSessionFailedEvent )
		ON_NMEVENT( CNMGSSessionClosedEvent, OnGSSessionClosedEvent )
		ON_NMEVENT( CNMGSSessionInfoChangedEvent, OnGSSessionInfoChangedEvent )
		ON_NMEVENT( CNMGSNewMemberJoinedEvent, OnGSNewMemberJoinedEvent )
		ON_NMEVENT( CNMGSMemberLeavedEvent, OnGSMemberLeavedEvent )
		ON_NMEVENT( CNMGSMemberInfoChangedEvent, OnGSMemberInfoChangedEvent )

		ON_NMEVENT( CNMCSChatMessageReceivedEvent, OnCSChatMessageReceivedEvent )
		ON_NMEVENT( CNMFUSFileUploadEventReceivedEvent, OnFUSFileUploadEventReceivedEvent )
		ON_NMEVENT( CNMFDSFileDownloadEventReceivedEvent, OnFDSFileDownloadEventReceivedEvent )
		ON_NMEVENT( CNMWSWhiteBoardMsgReceivedEvent, OnWSWhiteBoardMsgReceivedEvent )
		ON_NMEVENT( CNMWSWhiteBoardAckReceivedEvent, OnWSWhiteBoardAckReceivedEvent )
	END_NMEVENT_MAP
}

void CNMEventHandler::OnMessengerReplyEvent( CNMMessengerReplyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnAuthSecondaryConnectionClosedEvent( CNMAuthSecondaryConnectionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnRefreshEvent( CNMRefreshEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnSpecialEvent( CNMSpecialEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnServerMessageEvent( CNMServerMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCustomMessageEvent( CNMCustomMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnNoteInstantMsgEvent( CNMNoteInstantMsgEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnNoteInfoEvent( CNMNoteInfoEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnRefreshMessageEvent( CNMRefreshMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnFindUserReplyEvent( CNMFindUserReplyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnInviteVirtualUserEvent( CNMInviteVirtualUserEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnUserInfoEvent( CNMUserInfoEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGuildOnlineInfoExEvent( CNMGuildOnlineInfoExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGuildChatMessageEvent( CNMGuildChatMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnNotifyEvent( CNMNotifyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCustomNotifyEvent( CNMCustomNotifyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnAttendanceEventEvent( CNMAttendanceEventEvent * pEvent ) { UNREFERENCED_PARAMETER(pEvent); }

void CNMEventHandler::OnCRChatRoomCreatedExEvent( CNMCRChatRoomCreatedExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCRChatRoomEstablishedExEvent( CNMCRChatRoomEstablishedExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCRChatRoomErrorEvent( CNMCRChatRoomErrorEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCRChatRoomMemberEvent( CNMCRChatRoomMemberEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCRChatRoomInfoExEvent( CNMCRChatRoomInfoExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCRChatRoomMessageReceivedEvent( CNMCRChatRoomMessageReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); } 
void CNMEventHandler::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnFUSFileUploadEventReceivedEvent( CNMFUSFileUploadEventReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnFDSFileDownloadEventReceivedEvent( CNMFDSFileDownloadEventReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnWSWhiteBoardMsgReceivedEvent( CNMWSWhiteBoardMsgReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMEventHandler::OnWSWhiteBoardAckReceivedEvent( CNMWSWhiteBoardAckReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }

/* Main Event Handler */

void CNMMainEventHandler::OnMessengerReplyEvent( CNMMessengerReplyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnAuthSecondaryConnectionClosedEvent( CNMAuthSecondaryConnectionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnRefreshEvent( CNMRefreshEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnSpecialEvent( CNMSpecialEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnServerMessageEvent( CNMServerMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnCustomMessageEvent( CNMCustomMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnNoteInstantMsgEvent( CNMNoteInstantMsgEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnNoteInfoEvent( CNMNoteInfoEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnRefreshMessageEvent( CNMRefreshMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnFindUserReplyEvent( CNMFindUserReplyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnInviteVirtualUserEvent( CNMInviteVirtualUserEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnUserInfoEvent( CNMUserInfoEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnGuildOnlineInfoExEvent( CNMGuildOnlineInfoExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnGuildChatMessageEvent( CNMGuildChatMessageEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnNotifyEvent( CNMNotifyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnCustomNotifyEvent( CNMCustomNotifyEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnAttendanceEventEvent( CNMAttendanceEventEvent * pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMMainEventHandler::OnCRChatRoomCreatedExEvent( CNMCRChatRoomCreatedExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }

/* ChatRoomEventHandler */

void CNMChatRoomEventHandler::OnCRChatRoomEstablishedExEvent( CNMCRChatRoomEstablishedExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatRoomEventHandler::OnCRChatRoomErrorEvent( CNMCRChatRoomErrorEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatRoomEventHandler::OnCRChatRoomMemberEvent( CNMCRChatRoomMemberEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatRoomEventHandler::OnCRChatRoomInfoExEvent( CNMCRChatRoomInfoExEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatRoomEventHandler::OnCRChatRoomMessageReceivedEvent( CNMCRChatRoomMessageReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }

/* ChatSessionEventHandler */

void CNMChatSessionEventHandler::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatSessionEventHandler::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatSessionEventHandler::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatSessionEventHandler::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatSessionEventHandler::OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatSessionEventHandler::OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatSessionEventHandler::OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMChatSessionEventHandler::OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }

/* FileUploadSessionEventHandler */

void CNMFileUploadSessionEventHandler::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileUploadSessionEventHandler::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileUploadSessionEventHandler::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileUploadSessionEventHandler::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileUploadSessionEventHandler::OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileUploadSessionEventHandler::OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileUploadSessionEventHandler::OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileUploadSessionEventHandler::OnFUSFileUploadEventReceivedEvent( CNMFUSFileUploadEventReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }

/* FileDownloadSessionEventHandler */

void CNMFileDownloadSessionEventHandler::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileDownloadSessionEventHandler::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileDownloadSessionEventHandler::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileDownloadSessionEventHandler::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileDownloadSessionEventHandler::OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileDownloadSessionEventHandler::OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileDownloadSessionEventHandler::OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMFileDownloadSessionEventHandler::OnFDSFileDownloadEventReceivedEvent( CNMFDSFileDownloadEventReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }

/* WhiteBoardSessionEventHandler */

void CNMWhiteBoardSessionEventHandler::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnWSWhiteBoardMsgReceivedEvent( CNMWSWhiteBoardMsgReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
void CNMWhiteBoardSessionEventHandler::OnWSWhiteBoardAckReceivedEvent( CNMWSWhiteBoardAckReceivedEvent* pEvent ) { UNREFERENCED_PARAMETER(pEvent); }
