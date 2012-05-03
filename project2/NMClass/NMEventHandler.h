#ifdef _XDEF_NEXONLOGIN

#ifndef __NMEVENTHANDLER_H_9E585DA4_041F_46e6_A3B0_4ED284FEA71E__
#define __NMEVENTHANDLER_H_9E585DA4_041F_46e6_A3B0_4ED284FEA71E__

class CNMEventHandler
{
public:
	void			HandleNMEvent( WPARAM wParam, LPARAM lParam );

protected:
	virtual void	OnNMEvent( CNMEvent* pEvent );

protected:
	virtual	void	OnMessengerReplyEvent( CNMMessengerReplyEvent* pEvent );
	virtual void	OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent* pEvent );
	virtual void	OnAuthSecondaryConnectionClosedEvent( CNMAuthSecondaryConnectionClosedEvent* pEvent );
	virtual void	OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent* pEvent );
	virtual void	OnRefreshEvent( CNMRefreshEvent* pEvent );
	virtual void	OnSpecialEvent( CNMSpecialEvent* pEvent );
	virtual void	OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent );
	virtual void	OnServerMessageEvent( CNMServerMessageEvent* pEvent );
	virtual void	OnCustomMessageEvent( CNMCustomMessageEvent* pEvent );
	virtual void	OnNoteInstantMsgEvent( CNMNoteInstantMsgEvent* pEvent );
	virtual void	OnRefreshMessageEvent( CNMRefreshMessageEvent* pEvent );
	virtual void	OnFindUserReplyEvent( CNMFindUserReplyEvent* pEvent );
	virtual void	OnInviteVirtualUserEvent( CNMInviteVirtualUserEvent* pEvent );
	virtual void	OnUserInfoEvent( CNMUserInfoEvent* pEvent );
	virtual void	OnGuildOnlineInfoExEvent( CNMGuildOnlineInfoExEvent* pEvent );
	virtual void	OnGuildChatMessageEvent( CNMGuildChatMessageEvent* pEvent );
	virtual void	OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent );
	virtual void	OnNotifyEvent( CNMNotifyEvent* pEvent );
	virtual void	OnCustomNotifyEvent( CNMCustomNotifyEvent* pEvent );
	virtual void	OnNoteInfoEvent( CNMNoteInfoEvent* pEvent );
	virtual void	OnAttendanceEventEvent( CNMAttendanceEventEvent * pEvent );

	virtual void	OnCRChatRoomCreatedExEvent( CNMCRChatRoomCreatedExEvent* pEvent );
	virtual void	OnCRChatRoomEstablishedExEvent( CNMCRChatRoomEstablishedExEvent* pEvent );
	virtual void	OnCRChatRoomErrorEvent( CNMCRChatRoomErrorEvent* pEvent );
	virtual void	OnCRChatRoomMemberEvent( CNMCRChatRoomMemberEvent* pEvent );
	virtual void	OnCRChatRoomInfoExEvent( CNMCRChatRoomInfoExEvent* pEvent );
	virtual void	OnCRChatRoomMessageReceivedEvent( CNMCRChatRoomMessageReceivedEvent* pEvent );

	virtual void	OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent );
	virtual void	OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );
	virtual void	OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );
	virtual void	OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );
	virtual void	OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );
	virtual void	OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent );
	virtual void	OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent );
	virtual void	OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent );

	virtual void	OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent );
	virtual void	OnFUSFileUploadEventReceivedEvent( CNMFUSFileUploadEventReceivedEvent* pEvent );
	virtual void	OnFDSFileDownloadEventReceivedEvent( CNMFDSFileDownloadEventReceivedEvent* pEvent );
	virtual void	OnWSWhiteBoardMsgReceivedEvent( CNMWSWhiteBoardMsgReceivedEvent* pEvent );
	virtual void	OnWSWhiteBoardAckReceivedEvent( CNMWSWhiteBoardAckReceivedEvent* pEvent );
};

class CNMMainEventHandler : public CNMEventHandler
{
protected:
	virtual	void	OnMessengerReplyEvent( CNMMessengerReplyEvent* pEvent );
	virtual void	OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent* pEvent );
	virtual void	OnAuthSecondaryConnectionClosedEvent( CNMAuthSecondaryConnectionClosedEvent* pEvent );
	virtual void	OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent* pEvent );
	virtual void	OnRefreshEvent( CNMRefreshEvent* pEvent );
	virtual void	OnSpecialEvent( CNMSpecialEvent* pEvent );
	virtual void	OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent );
	virtual void	OnServerMessageEvent( CNMServerMessageEvent* pEvent );
	virtual void	OnCustomMessageEvent( CNMCustomMessageEvent* pEvent );
	virtual void	OnNoteInstantMsgEvent( CNMNoteInstantMsgEvent* pEvent );
	virtual void	OnRefreshMessageEvent( CNMRefreshMessageEvent* pEvent );
	virtual void	OnFindUserReplyEvent( CNMFindUserReplyEvent* pEvent );
	virtual void	OnInviteVirtualUserEvent( CNMInviteVirtualUserEvent* pEvent );
	virtual void	OnUserInfoEvent( CNMUserInfoEvent* pEvent );
	virtual void	OnGuildOnlineInfoExEvent( CNMGuildOnlineInfoExEvent* pEvent );
	virtual void	OnGuildChatMessageEvent( CNMGuildChatMessageEvent* pEvent );
	virtual void	OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent );
	virtual void	OnNotifyEvent( CNMNotifyEvent* pEvent );
	virtual void	OnCustomNotifyEvent( CNMCustomNotifyEvent* pEvent );
	virtual void	OnNoteInfoEvent( CNMNoteInfoEvent* pEvent );
	virtual void	OnAttendanceEventEvent( CNMAttendanceEventEvent * pEvent );

	virtual void	OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent );
	virtual void	OnCRChatRoomCreatedExEvent( CNMCRChatRoomCreatedExEvent* pEvent );
};

class CNMChatRoomEventHandler : public CNMEventHandler
{
protected:
	virtual void	OnCRChatRoomEstablishedExEvent( CNMCRChatRoomEstablishedExEvent* pEvent );
	virtual void	OnCRChatRoomErrorEvent( CNMCRChatRoomErrorEvent* pEvent );
	virtual void	OnCRChatRoomMemberEvent( CNMCRChatRoomMemberEvent* pEvent );
	virtual void	OnCRChatRoomInfoExEvent( CNMCRChatRoomInfoExEvent* pEvent );
	virtual void	OnCRChatRoomMessageReceivedEvent( CNMCRChatRoomMessageReceivedEvent* pEvent );
};

class CNMChatSessionEventHandler : public CNMEventHandler
{
protected:
	virtual void	OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );
	virtual void	OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );
	virtual void	OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );
	virtual void	OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );
	virtual void	OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent );
	virtual void	OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent );
	virtual void	OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent );

	virtual void	OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent );
};

class CNMFileUploadSessionEventHandler : public CNMEventHandler
{
protected:
	virtual void	OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );
	virtual void	OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );
	virtual void	OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );
	virtual void	OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );
	virtual void	OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent );
	virtual void	OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent );
	virtual void	OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent );

	virtual void	OnFUSFileUploadEventReceivedEvent( CNMFUSFileUploadEventReceivedEvent* pEvent );
};

class CNMFileDownloadSessionEventHandler : public CNMEventHandler
{
protected:
	virtual void	OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );
	virtual void	OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );
	virtual void	OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );
	virtual void	OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );
	virtual void	OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent );
	virtual void	OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent );
	virtual void	OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent );

	virtual void	OnFDSFileDownloadEventReceivedEvent( CNMFDSFileDownloadEventReceivedEvent* pEvent );
};

class CNMWhiteBoardSessionEventHandler : public CNMEventHandler
{
protected:
	virtual void	OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );
	virtual void	OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );
	virtual void	OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );
	virtual void	OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );
	virtual void	OnGSNewMemberJoinedEvent( CNMGSNewMemberJoinedEvent* pEvent );
	virtual void	OnGSMemberLeavedEvent( CNMGSMemberLeavedEvent* pEvent );
	virtual void	OnGSMemberInfoChangedEvent( CNMGSMemberInfoChangedEvent* pEvent );

	virtual void	OnWSWhiteBoardMsgReceivedEvent( CNMWSWhiteBoardMsgReceivedEvent* pEvent );
	virtual void	OnWSWhiteBoardAckReceivedEvent( CNMWSWhiteBoardAckReceivedEvent* pEvent );
};

#endif	//	#ifndef __NMEVENTHANDLER_H_9E585DA4_041F_46e6_A3B0_4ED284FEA71E__

#endif