#include "stdafx.h"
#include "XKernel.h"

#include "PCH.h"
#include "NMGeneral.h"
#include "NMEventObject.h"

#define	INIT_STRING( dstName, srcName, len )													\
	if ( srcName != NULL ) { ::_tcsncpy( dstName, srcName, len ); dstName[ len - 1 ] = 0; }		\
	else { dstName[ 0 ] = 0; }

#define	INIT_STRING2( name, len )		INIT_STRING( this->name, name, len )

#define BEGIN_NMEVENT_SERIALIZE( className )	\
	BEGIN_SERIALIZE( className )				\
	PRIOR_SERIALIZE( CNMEvent )

#define BEGIN_NMEVENT_DESERIALIZE( className )	\
	BEGIN_DESERIALIZE( className )				\
	PRIOR_DESERIALIZE( CNMEvent )

#define END_NMEVENT_SERIALIZE		END_SERIALIZE
#define END_NMEVENT_DESERIALIZE		END_DESERIALIZE

//=========================================
// class CNEvent
//=========================================

CNMEvent::CNMEvent( _UInt32_ uCode, _UInt8_ uObjectVersion, BOOL bHasResult, _UInt32_ uFlag )
	: CNMSerializable( 'eo', 1, uObjectVersion )
	, m_uCode( uCode )
	, m_bHandled( FALSE )
	, m_bPassThrough( TRUE )
	, m_bHasResult( bHasResult )
	, m_uFlag( uFlag )
	, m_uRemainingCount( 0 )
{
}

BEGIN_SERIALIZE( CNMEvent )
//{
	UNREFERENCED_PARAMETER(ssStream);
//}
END_SERIALIZE()

BEGIN_DESERIALIZE( CNMEvent )
//{
	UNREFERENCED_PARAMETER(ssStream);
//}
END_DESERIALIZE()

//=========================================
// class CNMMessengerReplyEvent
//=========================================

CNMMessengerReplyEvent::CNMMessengerReplyEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->nResultCode = 0;
}

CNMMessengerReplyEvent::CNMMessengerReplyEvent( _SInt32_ nResultCode )
	: CNMEvent( kCode, 1 )
{
	this->nResultCode = nResultCode;
}

BEGIN_NMEVENT_SERIALIZE( CNMMessengerReplyEvent )
//{
	encoder.Encode4( this->nResultCode );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMMessengerReplyEvent )
//{
	this->nResultCode = decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMMsgConnectionClosedEvent
//=========================================

CNMMsgConnectionClosedEvent::CNMMsgConnectionClosedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uType = 0;
}

CNMMsgConnectionClosedEvent::CNMMsgConnectionClosedEvent( _UInt32_ uType )
	: CNMEvent( kCode, 1 )
{
	this->uType = uType;
}

BEGIN_NMEVENT_SERIALIZE( CNMMsgConnectionClosedEvent )
//{
	encoder.Encode4( this->uType );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMMsgConnectionClosedEvent )
//{
	this->uType = decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMRefreshEvent
//=========================================

CNMRefreshEvent::CNMRefreshEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uType		= 0;
}

CNMRefreshEvent::CNMRefreshEvent( _UInt32_ uType )
	: CNMEvent( kCode, 1 )
{
	this->uType		= uType;
}

BEGIN_NMEVENT_SERIALIZE( CNMRefreshEvent )
//{
	encoder.Encode4( this->uType );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMRefreshEvent )
//{
	this->uType		= decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMSpecialEvent
//=========================================

CNMSpecialEvent::CNMSpecialEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uType				= 0;
	this->szArgument[ 0 ]	= 0;
}

CNMSpecialEvent::CNMSpecialEvent( _UInt32_ uType, LPCTSTR szArgument )
	: CNMEvent( kCode, 1 )
{
	this->uType				= uType;
	INIT_STRING2( szArgument, 256 );
}

BEGIN_NMEVENT_SERIALIZE( CNMSpecialEvent )
//{
	encoder.Encode4( this->uType );
	encoder.EncodeString( this->szArgument );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMSpecialEvent )
//{
	this->uType		= decoder.Decode4();
	decoder.DecodeString( this->szArgument, 256 );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMRequestNewFriendEvent
//=========================================

CNMRequestNewFriendEvent::CNMRequestNewFriendEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMRequestNewFriendEvent::CNMRequestNewFriendEvent( CNMRequestNewFriendData& rnfData )
	: CNMEvent( kCode, 1 )
{
	this->rnfData		= rnfData;
}

BEGIN_NMEVENT_SERIALIZE( CNMRequestNewFriendEvent )
//{
	OBJECT_ENCODE( this->rnfData );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMRequestNewFriendEvent )
//{
	OBJECT_DECODE( this->rnfData );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMServerMessageEvent
//=========================================

CNMServerMessageEvent::CNMServerMessageEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMServerMessageEvent::CNMServerMessageEvent( CNMMessageData& msgData )
	: CNMEvent( kCode, 1 )
{
	this->msgData	= msgData;
}

BEGIN_NMEVENT_SERIALIZE( CNMServerMessageEvent )
//{
	OBJECT_ENCODE( this->msgData );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMServerMessageEvent )
//{
	OBJECT_DECODE( this->msgData );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCustomMessageEvent
//=========================================

CNMCustomMessageEvent::CNMCustomMessageEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMCustomMessageEvent::CNMCustomMessageEvent( CNMMessageData& msgData )
	: CNMEvent( kCode, 1 )
{
	this->msgData	= msgData;
}

BEGIN_NMEVENT_SERIALIZE( CNMCustomMessageEvent )
//{
	OBJECT_ENCODE( this->msgData );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCustomMessageEvent )
//{
	OBJECT_DECODE( this->msgData );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMNoteInstantMsgEvent
//=========================================

CNMNoteInstantMsgEvent::CNMNoteInstantMsgEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMNoteInstantMsgEvent::CNMNoteInstantMsgEvent( CNMNoteInfo& niInfo )
	: CNMEvent( kCode, 1 )
{
	this->niInfo	= niInfo;
}

CNMNoteInstantMsgEvent::CNMNoteInstantMsgEvent( CNMNoteInfo2 & noteInfo )
	: CNMEvent( kCode, 1 )
{
	this->niInfo	= noteInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMNoteInstantMsgEvent )
//{
	OBJECT_ENCODE( this->niInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMNoteInstantMsgEvent )
//{
	OBJECT_DECODE( this->niInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMRefreshMessageEvent
//=========================================

CNMRefreshMessageEvent::CNMRefreshMessageEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uType = kRefresh_Avatar;
}

CNMRefreshMessageEvent::CNMRefreshMessageEvent( NMREFRESHTYPE uType )
	: CNMEvent( kCode, 1 )
{
	this->uType	= uType;
}

BEGIN_NMEVENT_SERIALIZE( CNMRefreshMessageEvent )
//{
	encoder.Encode4( this->uType );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMRefreshMessageEvent )
//{
	this->uType	= ( NMREFRESHTYPE )decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMFindUserReplyEvent
//=========================================

CNMFindUserReplyEvent::CNMFindUserReplyEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uToken	= 0;
}

CNMFindUserReplyEvent::CNMFindUserReplyEvent( _UInt32_ uToken )
	: CNMEvent( kCode, 1 )
{
	this->uToken	= uToken;
}

BEGIN_NMEVENT_SERIALIZE( CNMFindUserReplyEvent )
//{
	encoder.Encode4( this->uToken );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMFindUserReplyEvent )
//{
	this->uToken	= decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMInviteVirtualUserEvent
//=========================================

CNMInviteVirtualUserEvent::CNMInviteVirtualUserEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSessionID			= 0;
	this->uSessionType			= 0;
	this->uInviteMode			= CNMSessionInfo::kInviteMode_Stranger;
	INITIALIZE_VIRTUALKEY( this->keyToVirtual );
	INITIALIZE_VIRTUALKEY( this->keyFromVirtual );
	this->szFromLoginID[ 0 ]	= NULL;
	this->szFromNickName[ 0 ]	= NULL;
	this->szMessage[ 0 ]		= NULL;
	this->bIsHandled			= FALSE;
}

CNMInviteVirtualUserEvent::CNMInviteVirtualUserEvent( _UInt64_ uSessionID, _UInt32_ uSessionType, _UInt32_ uInviteMode, NMVirtualKey keyToVirtual, NMVirtualKey keyFromVirtual, LPCTSTR szFromLoginID, LPCTSTR szFromNickName )
	: CNMEvent( kCode, 1 )
{
	this->uSessionID			= uSessionID;
	this->uSessionType			= uSessionType;
	this->uInviteMode			= uInviteMode;
	this->keyToVirtual			= keyToVirtual;
	this->keyFromVirtual		= keyFromVirtual;
	INIT_STRING2( szFromLoginID, LOGINID_SIZE );
	INIT_STRING2( szFromNickName, NICKNAME_SIZE );
	this->szMessage[ 0 ]		= NULL;
	this->bIsHandled			= FALSE;
}

BEGIN_NMEVENT_SERIALIZE( CNMInviteVirtualUserEvent )
//{
	encoder.Encode8( this->uSessionID );
	encoder.Encode4( this->uSessionType );
	encoder.Encode4( this->uInviteMode );
	ENCODE_VIRTUALKEY( this->keyToVirtual );
	ENCODE_VIRTUALKEY( this->keyFromVirtual );
	encoder.EncodeString( this->szFromLoginID );
	encoder.EncodeString( this->szFromNickName );
	encoder.EncodeString( this->szMessage );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMInviteVirtualUserEvent )
//{
	this->uSessionID			= decoder.Decode8();
	this->uSessionType			= decoder.Decode4();
	this->uInviteMode			= decoder.Decode4();
	DECODE_VIRTUALKEY( this->keyToVirtual );
	DECODE_VIRTUALKEY( this->keyFromVirtual );
	decoder.DecodeString( this->szFromLoginID, LOGINID_SIZE );
	decoder.DecodeString( this->szFromNickName, NICKNAME_SIZE );
	decoder.DecodeString( this->szMessage, MSG_SIZE );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMUserInfoEvent
//=========================================

CNMUserInfoEvent::CNMUserInfoEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMUserInfoEvent::CNMUserInfoEvent( CNMRealUserInfo& ruiInfo )
	: CNMEvent( kCode, 1 )
{
	this->ruiInfo		= ruiInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMUserInfoEvent )
//{
	OBJECT_ENCODE( this->ruiInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMUserInfoEvent )
//{
	OBJECT_DECODE( this->ruiInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGuildOnlineInfoEvent
//=========================================

CNMGuildOnlineInfoEvent::CNMGuildOnlineInfoEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMGuildOnlineInfoEvent::CNMGuildOnlineInfoEvent( CNMGuildOnlineInfo& goiInfo )
	: CNMEvent( kCode, 1 )
{
	this->goiInfo	= goiInfo;
}

CNMGuildOnlineInfoEvent::CNMGuildOnlineInfoEvent( CNMGuildOnlineInfoEx& goiInfo )
	: CNMEvent( kCode, 1 )
{
	this->goiInfo	= goiInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMGuildOnlineInfoEvent )
//{
	OBJECT_ENCODE( this->goiInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGuildOnlineInfoEvent )
//{
	OBJECT_DECODE( this->goiInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGuildOnlineInfoExEvent
//=========================================

CNMGuildOnlineInfoExEvent::CNMGuildOnlineInfoExEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMGuildOnlineInfoExEvent::CNMGuildOnlineInfoExEvent( CNMGuildOnlineInfoEx& goiInfo )
	: CNMEvent( kCode, 1 )
{
	this->goiInfo	= goiInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMGuildOnlineInfoExEvent )
//{
	OBJECT_ENCODE( this->goiInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGuildOnlineInfoExEvent )
//{
	OBJECT_DECODE( this->goiInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGuildChatMessageEvent
//=========================================

CNMGuildChatMessageEvent::CNMGuildChatMessageEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uGuildKey		= 0;
	this->uFromIDCode	= 0;
	this->bIsWhisper	= FALSE;
}

CNMGuildChatMessageEvent::CNMGuildChatMessageEvent( NMGuildKey uGuildKey, NMIDCode uFromIDCode, BOOL bIsWhisper, CNMChatMessage cmChatMsg )
	: CNMEvent( kCode, 1 )
{
	this->uGuildKey		= uGuildKey;
	this->uFromIDCode	= uFromIDCode;
	this->bIsWhisper	= bIsWhisper;
	this->cmChatMsg		= cmChatMsg;
}

BEGIN_NMEVENT_SERIALIZE( CNMGuildChatMessageEvent )
//{
	encoder.Encode8( this->uGuildKey );
	encoder.Encode8( this->uFromIDCode );
	encoder.Encode1( static_cast<UINT8>( this->bIsWhisper ) );
	OBJECT_ENCODE( this->cmChatMsg );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGuildChatMessageEvent )
//{
	this->uGuildKey		= decoder.Decode8();
	this->uFromIDCode	= decoder.Decode8();
	this->bIsWhisper	= decoder.Decode1();
	OBJECT_DECODE( this->cmChatMsg );
//}
END_NMEVENT_DESERIALIZE()


//=========================================
// class CNMCustomNotifyEvent
//=========================================

CNMCustomNotifyEvent::CNMCustomNotifyEvent( void )
	: CNMEvent( kCode, 1 )
{
}

CNMCustomNotifyEvent::CNMCustomNotifyEvent( CNMCustomNotifyData& cnData )
	: CNMEvent( kCode, 1 )
{
	this->cnData		= cnData;
}

BEGIN_NMEVENT_SERIALIZE( CNMCustomNotifyEvent )
//{
	OBJECT_ENCODE( this->cnData );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCustomNotifyEvent )
//{
	OBJECT_DECODE( this->cnData );
//}
END_NMEVENT_DESERIALIZE()

//////////////////////////////////////////////////////////////////////////
//	CNMRejectedUserListChangedEvent
//////////////////////////////////////////////////////////////////////////

CNMRejectedUserListChangedEvent::CNMRejectedUserListChangedEvent( void ) 
	: CNMEvent( kCode, 1 )
{
}

BEGIN_NMEVENT_SERIALIZE( CNMRejectedUserListChangedEvent )
//{
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMRejectedUserListChangedEvent )
//{
//}
END_NMEVENT_DESERIALIZE()

//////////////////////////////////////////////////////////////////////////
//	CNMNoteInfoEvent
//////////////////////////////////////////////////////////////////////////

CNMNoteInfoEvent::CNMNoteInfoEvent( void ) : CNMEvent( kCode, 1 )
{
}

CNMNoteInfoEvent::CNMNoteInfoEvent( CNMNoteInfo2 & noteInfo ) : CNMEvent( kCode, 1 )
{
	this->noteInfo = noteInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMNoteInfoEvent )
//{
	OBJECT_ENCODE( this->noteInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMNoteInfoEvent )
//{
	OBJECT_DECODE( this->noteInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMAuthConnectionClosedEvent
//=========================================

CNMAuthConnectionClosedEvent::CNMAuthConnectionClosedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uType	= kType_SystemError;
}

CNMAuthConnectionClosedEvent::CNMAuthConnectionClosedEvent( UINT32 uType )
	: CNMEvent( kCode, 1 )
{
	this->uType	= uType;
}

BEGIN_NMEVENT_SERIALIZE( CNMAuthConnectionClosedEvent )
//{
	encoder.Encode4( this->uType );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMAuthConnectionClosedEvent )
//{
	this->uType = decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMAuthConnectionClosedEvent
//=========================================

CNMAuthSecondaryConnectionClosedEvent::CNMAuthSecondaryConnectionClosedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uType	= kType_SystemError;
}

CNMAuthSecondaryConnectionClosedEvent::CNMAuthSecondaryConnectionClosedEvent( UINT32 uType )
	: CNMEvent( kCode, 1 )
{
	this->uType	= uType;
}

BEGIN_NMEVENT_SERIALIZE( CNMAuthSecondaryConnectionClosedEvent )
//{
	encoder.Encode4( this->uType );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMAuthSecondaryConnectionClosedEvent )
//{
	this->uType = decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMRealFriendInfoChangedEvent
//=========================================

CNMRealFriendInfoChangedEvent::CNMRealFriendInfoChangedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uChangeFlag		= 0;
	this->bAvatarInfoValid	= FALSE;
}

CNMRealFriendInfoChangedEvent::CNMRealFriendInfoChangedEvent( _UInt32_ uChangeFlag, CNMRealFriendInfo& rfiInfo, BOOL bAvatarInfoValid, CNMAvatarFullInfo* paiAvatarInfo )
	: CNMEvent( kCode, 1 )
{
	this->uChangeFlag		= uChangeFlag;
	this->rfiInfo			= rfiInfo;
	this->bAvatarInfoValid	= bAvatarInfoValid;
	if ( this->bAvatarInfoValid && paiAvatarInfo )
		this->aiAvatarInfo	= *paiAvatarInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMRealFriendInfoChangedEvent )
//{
	encoder.Encode4( this->uChangeFlag );
	OBJECT_ENCODE( this->rfiInfo );
	encoder.Encode1( static_cast<UINT8>( this->bAvatarInfoValid ) );
	if ( this->bAvatarInfoValid )
		OBJECT_ENCODE( this->aiAvatarInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMRealFriendInfoChangedEvent )
//{
	this->uChangeFlag		= decoder.Decode4();
	OBJECT_DECODE( this->rfiInfo );
	this->bAvatarInfoValid	= decoder.Decode1();
	if ( this->bAvatarInfoValid )
		OBJECT_DECODE( this->aiAvatarInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMVirtualFriendInfoChangedEvent
//=========================================

CNMVirtualFriendInfoChangedEvent::CNMVirtualFriendInfoChangedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uChangeFlag	= 0;
}

CNMVirtualFriendInfoChangedEvent::CNMVirtualFriendInfoChangedEvent( _UInt32_ uChangeFlag, CNMVirtualFriendInfo& vfiInfo )
	: CNMEvent( kCode, 1 )
{
	this->uChangeFlag	= uChangeFlag;
	this->vfiInfo		= vfiInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMVirtualFriendInfoChangedEvent )
//{
	encoder.Encode4( this->uChangeFlag );
	OBJECT_ENCODE( this->vfiInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMVirtualFriendInfoChangedEvent )
//{
	this->uChangeFlag	= decoder.Decode4();
	OBJECT_DECODE( this->vfiInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMFriendInfoChangedEvent
//=========================================

CNMFriendInfoChangedEvent::CNMFriendInfoChangedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->bAvatarInfoValid	= FALSE;
}

CNMFriendInfoChangedEvent::CNMFriendInfoChangedEvent( CNMFriendInfo& fiFriendInfo, BOOL bAvatarInfoValid, CNMAvatarFullInfo* paiAvatarInfo )
	: CNMEvent( kCode, 1 )
{
	this->fiFriendInfo		= fiFriendInfo;
	this->bAvatarInfoValid	= bAvatarInfoValid;
	if ( this->bAvatarInfoValid && paiAvatarInfo )
		this->aiAvatarInfo	= *paiAvatarInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMFriendInfoChangedEvent )
//{
	OBJECT_ENCODE( this->fiFriendInfo );
	encoder.Encode1( static_cast<UINT8>( this->bAvatarInfoValid ) );
	if ( this->bAvatarInfoValid )
		OBJECT_ENCODE( this->aiAvatarInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMFriendInfoChangedEvent )
//{
	OBJECT_DECODE( this->fiFriendInfo );
	this->bAvatarInfoValid	= decoder.Decode1();
	if ( this->bAvatarInfoValid )
		OBJECT_DECODE( this->aiAvatarInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMNotifyEvent
//=========================================

CNMNotifyEvent::CNMNotifyEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uType			= kType_NULL;
	this->uParam1		= 0;
	this->uParam2		= 0;
	this->uParam3		= 0;
	this->uParam4		= 0;
	this->szParam1[ 0 ]	= NULL;
	this->szParam2[ 0 ]	= NULL;
}

BEGIN_NMEVENT_SERIALIZE( CNMNotifyEvent )
//{
	encoder.Encode4( this->uType );
	encoder.Encode4( this->uParam1 );
	encoder.Encode4( this->uParam2 );
	encoder.Encode8( this->uParam3 );
	encoder.Encode8( this->uParam4 );
	encoder.EncodeString( this->szParam1 );
	encoder.EncodeString( this->szParam2 );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMNotifyEvent )
//{
	this->uType		= decoder.Decode4();
	this->uParam1	= decoder.Decode4();
	this->uParam2	= decoder.Decode4();
	this->uParam3	= decoder.Decode8();
	this->uParam4	= decoder.Decode8();
	decoder.DecodeString( this->szParam1, 1024 );
	decoder.DecodeString( this->szParam2, 1024 );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomCreatedEvent
//=========================================

CNMCRChatRoomCreatedEvent::CNMCRChatRoomCreatedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= 0;
}

CNMCRChatRoomCreatedEvent::CNMCRChatRoomCreatedEvent( _UInt32_ uSerialKe )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= uSerialKe;
}

CNMCRChatRoomCreatedEvent::CNMCRChatRoomCreatedEvent( _UInt32_ uSerialKey, CNMChatRoomInfo& criInfo, CNMChatRoomMemberInfo& crmiMyInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= uSerialKey;
	this->criInfo			= criInfo;
	this->crmiMyInfo		= crmiMyInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomCreatedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->criInfo );
	OBJECT_ENCODE( this->crmiMyInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomCreatedEvent )
//{
	this->uSerialKey		= decoder.Decode4();
	OBJECT_DECODE( this->criInfo );
	OBJECT_DECODE( this->crmiMyInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomCreatedExEvent
//=========================================

CNMCRChatRoomCreatedExEvent::CNMCRChatRoomCreatedExEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= 0;
}

CNMCRChatRoomCreatedExEvent::CNMCRChatRoomCreatedExEvent( _UInt32_ uSerialKe )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= uSerialKe;
}

CNMCRChatRoomCreatedExEvent::CNMCRChatRoomCreatedExEvent( _UInt32_ uSerialKey, CNMChatRoomInfoEx& criInfo, CNMChatRoomMemberInfo& crmiMyInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= uSerialKey;
	this->criInfo			= criInfo;
	this->crmiMyInfo		= crmiMyInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomCreatedExEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->criInfo );
	OBJECT_ENCODE( this->crmiMyInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomCreatedExEvent )
//{
	this->uSerialKey		= decoder.Decode4();
	OBJECT_DECODE( this->criInfo );
	OBJECT_DECODE( this->crmiMyInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomEstablishedEvent
//=========================================

CNMCRChatRoomEstablishedEvent::CNMCRChatRoomEstablishedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMCRChatRoomEstablishedEvent::CNMCRChatRoomEstablishedEvent( _UInt32_ uSerialKey )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
}

CNMCRChatRoomEstablishedEvent::CNMCRChatRoomEstablishedEvent( _UInt32_ uSerialKey, CNMChatRoomInfo& criInfo, CNMChatRoomMemberInfo& crmiMyInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->criInfo		= criInfo;
	this->crmiMyInfo	= crmiMyInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomEstablishedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->criInfo );
	OBJECT_ENCODE( this->crmiMyInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomEstablishedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->criInfo );
	OBJECT_DECODE( this->crmiMyInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomEstablishedExEvent
//=========================================

CNMCRChatRoomEstablishedExEvent::CNMCRChatRoomEstablishedExEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMCRChatRoomEstablishedExEvent::CNMCRChatRoomEstablishedExEvent( _UInt32_ uSerialKey )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
}

CNMCRChatRoomEstablishedExEvent::CNMCRChatRoomEstablishedExEvent( _UInt32_ uSerialKey, CNMChatRoomInfoEx& criInfo, CNMChatRoomMemberInfo& crmiMyInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->criInfo		= criInfo;
	this->crmiMyInfo	= crmiMyInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomEstablishedExEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->criInfo );
	OBJECT_ENCODE( this->crmiMyInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomEstablishedExEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->criInfo );
	OBJECT_DECODE( this->crmiMyInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomErrorEvent
//=========================================

CNMCRChatRoomErrorEvent::CNMCRChatRoomErrorEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
	this->uType			= kType_NULL;
}

CNMCRChatRoomErrorEvent::CNMCRChatRoomErrorEvent( _UInt32_ uSerialKey, _UInt32_ uType )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->uType			= uType;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomErrorEvent )
//{
	encoder.Encode4( this->uSerialKey );
	encoder.Encode4( this->uType );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomErrorEvent )
//{
	this->uSerialKey	= decoder.Decode4();
	this->uType			= decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomMemberEvent
//=========================================

CNMCRChatRoomMemberEvent::CNMCRChatRoomMemberEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
	this->uType			= kType_NULL;
}
CNMCRChatRoomMemberEvent::CNMCRChatRoomMemberEvent( _UInt32_ uSerialKey )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->uType			= kType_RefreshMember;
}

CNMCRChatRoomMemberEvent::CNMCRChatRoomMemberEvent( _UInt32_ uSerialKey, NMChatRoomMemberList& aMembers )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->uType			= kType_MemberList;
	this->aMembers		= aMembers;
}

CNMCRChatRoomMemberEvent::CNMCRChatRoomMemberEvent( _UInt32_ uSerialKey, _UInt32_ uType, CNMChatRoomMemberInfo& crmiInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->uType			= uType;
	this->crmiInfo		= crmiInfo;
}

CNMCRChatRoomMemberEvent::CNMCRChatRoomMemberEvent( _UInt32_ uSerialKey, CNMChatRoomMemberInfo& crmiInfo, CNMChatRoomMemberInfo& crmiOldInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->uType			= kType_InfoChanged;
	this->crmiInfo		= crmiInfo;
	this->crmiOldInfo	= crmiOldInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomMemberEvent )
//{
	encoder.Encode4( this->uSerialKey );
	encoder.Encode4( this->uType );
	LIST_ENCODE( NMChatRoomMemberList, this->aMembers );
	OBJECT_ENCODE( this->crmiInfo );
	OBJECT_ENCODE( this->crmiOldInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomMemberEvent )
//{
	this->uSerialKey	= decoder.Decode4();
	this->uType			= decoder.Decode4();
	LIST_DECODE( NMChatRoomMemberList, this->aMembers );
	OBJECT_DECODE( this->crmiInfo );
	OBJECT_DECODE( this->crmiOldInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomInfoEvent
//=========================================

CNMCRChatRoomInfoEvent::CNMCRChatRoomInfoEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMCRChatRoomInfoEvent::CNMCRChatRoomInfoEvent( _UInt32_ uSerialKey )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
}

CNMCRChatRoomInfoEvent::CNMCRChatRoomInfoEvent( _UInt32_ uSerialKey, CNMChatRoomInfo& criInfo, CNMChatRoomMemberInfo& crmiMyInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->criInfo		= criInfo;
	this->crmiMyInfo	= crmiMyInfo;
}

CNMCRChatRoomInfoEvent::CNMCRChatRoomInfoEvent( _UInt32_ uSerialKey, CNMChatRoomInfoEx& criInfo, CNMChatRoomMemberInfo& crmiMyInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->criInfo		= criInfo;
	this->crmiMyInfo	= crmiMyInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomInfoEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->criInfo );
	OBJECT_ENCODE( this->crmiMyInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomInfoEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->criInfo );
	OBJECT_DECODE( this->crmiMyInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomInfoExEvent
//=========================================

CNMCRChatRoomInfoExEvent::CNMCRChatRoomInfoExEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMCRChatRoomInfoExEvent::CNMCRChatRoomInfoExEvent( _UInt32_ uSerialKey )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
}

CNMCRChatRoomInfoExEvent::CNMCRChatRoomInfoExEvent( _UInt32_ uSerialKey, CNMChatRoomInfoEx& criInfo, CNMChatRoomMemberInfo& crmiMyInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->criInfo		= criInfo;
	this->crmiMyInfo	= crmiMyInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomInfoExEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->criInfo );
	OBJECT_ENCODE( this->crmiMyInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomInfoExEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->criInfo );
	OBJECT_DECODE( this->crmiMyInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCRChatRoomMessageReceivedEvent
//=========================================

CNMCRChatRoomMessageReceivedEvent::CNMCRChatRoomMessageReceivedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
	this->bIsWhisper	= FALSE;
	this->bIsHandled	= FALSE;
}

CNMCRChatRoomMessageReceivedEvent::CNMCRChatRoomMessageReceivedEvent( _UInt32_ uSerialKey, CNMChatMessage& cmMessage, BOOL bIsWhisper, BOOL bIsHandled )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->cmMessage		= cmMessage;
	this->bIsWhisper	= bIsWhisper;
	this->bIsHandled	= bIsHandled;
}

BEGIN_NMEVENT_SERIALIZE( CNMCRChatRoomMessageReceivedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->cmMessage );
	encoder.Encode1( static_cast<UINT8>( this->bIsWhisper ) );
	encoder.Encode1( static_cast<UINT8>( this->bIsHandled ) );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCRChatRoomMessageReceivedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->cmMessage );
	this->bIsWhisper = decoder.Decode1();
	this->bIsHandled = decoder.Decode1();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSSessionCreatedEvent
//=========================================

CNMGSSessionCreatedEvent::CNMGSSessionCreatedEvent( void )
	: CNMEvent( kCode, 1, TRUE )
{
	this->uSerialKey	= 0;
	this->bWndCreated	= FALSE;
	this->bByMsgRecv	= FALSE;
}

CNMGSSessionCreatedEvent::CNMGSSessionCreatedEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo, NMSessionMemberList& aMembers, BOOL bWndCreated )
	: CNMEvent( kCode, 1, TRUE )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
	this->aMembers		= aMembers;
	this->bWndCreated	= bWndCreated;
	this->bByMsgRecv	= FALSE;
}

CNMGSSessionCreatedEvent::CNMGSSessionCreatedEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo, NMSessionMemberList& aMembers, BOOL bWndCreated, BOOL bByMsgRecv )
	: CNMEvent( kCode, 1, TRUE )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
	this->aMembers		= aMembers;
	this->bWndCreated	= bWndCreated;
	this->bByMsgRecv	= bByMsgRecv;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSSessionCreatedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->siInfo );
	LIST_ENCODE( NMSessionMemberList, this->aMembers );
	encoder.Encode1( static_cast<UINT8>( this->bWndCreated ) );
	encoder.Encode1( static_cast<UINT8>( this->bByMsgRecv ) );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSSessionCreatedEvent )
//{
	this->uSerialKey	= decoder.Decode4();
	OBJECT_DECODE( this->siInfo );
	LIST_DECODE( NMSessionMemberList, this->aMembers );
	this->bWndCreated	= decoder.Decode1();
	this->bByMsgRecv	= decoder.Decode1();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSSessionEstablishedEvent
//=========================================

CNMGSSessionEstablishedEvent::CNMGSSessionEstablishedEvent( void )
	: CNMEvent( kCode, 1, TRUE )
{
	this->uSerialKey	= 0;
	this->bWndCreated	= FALSE;
}

CNMGSSessionEstablishedEvent::CNMGSSessionEstablishedEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo, NMSessionMemberList& aMembers, BOOL bWndCreated )
	: CNMEvent( kCode, 1, TRUE )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
	this->aMembers		= aMembers;
	this->bWndCreated	= bWndCreated;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSSessionEstablishedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->siInfo );
	LIST_ENCODE( NMSessionMemberList, this->aMembers );
	encoder.Encode1( static_cast<UINT8>( this->bWndCreated ) );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSSessionEstablishedEvent )
//{
	this->uSerialKey	= decoder.Decode4();
	OBJECT_DECODE( this->siInfo );
	LIST_DECODE( NMSessionMemberList, this->aMembers );
	this->bWndCreated	= decoder.Decode1();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSSessionFailedEvent
//=========================================

CNMGSSessionFailedEvent::CNMGSSessionFailedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMGSSessionFailedEvent::CNMGSSessionFailedEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSSessionFailedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->siInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSSessionFailedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->siInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSSessionClosedEvent
//=========================================

CNMGSSessionClosedEvent::CNMGSSessionClosedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMGSSessionClosedEvent::CNMGSSessionClosedEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSSessionClosedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->siInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSSessionClosedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->siInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSSessionInfoChangedEvent
//=========================================

CNMGSSessionInfoChangedEvent::CNMGSSessionInfoChangedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMGSSessionInfoChangedEvent::CNMGSSessionInfoChangedEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSSessionInfoChangedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->siInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSSessionInfoChangedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->siInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSNewMemberJoinedEvent
//=========================================

CNMGSNewMemberJoinedEvent::CNMGSNewMemberJoinedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMGSNewMemberJoinedEvent::CNMGSNewMemberJoinedEvent( _UInt32_ uSerialKey, CNMSessionMemberInfo& smiInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->smiInfo		= smiInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSNewMemberJoinedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->smiInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSNewMemberJoinedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->smiInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSMemberLeavedEvent
//=========================================

CNMGSMemberLeavedEvent::CNMGSMemberLeavedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMGSMemberLeavedEvent::CNMGSMemberLeavedEvent( _UInt32_ uSerialKey, CNMSessionMemberInfo& smiInfo )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->smiInfo		= smiInfo;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSMemberLeavedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->smiInfo );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSMemberLeavedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->smiInfo );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSMemberInfoChangedEvent
//=========================================

CNMGSMemberInfoChangedEvent::CNMGSMemberInfoChangedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMGSMemberInfoChangedEvent::CNMGSMemberInfoChangedEvent( _UInt32_ uSerialKey, CNMSessionMemberInfo& smiBefore, CNMSessionMemberInfo& smiAfter )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->smiBefore		= smiBefore;
	this->smiAfter		= smiAfter;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSMemberInfoChangedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->smiBefore );
	OBJECT_ENCODE( this->smiAfter );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSMemberInfoChangedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->smiBefore );
	OBJECT_DECODE( this->smiAfter );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSSessionInfoEvent
//=========================================

CNMGSSessionInfoEvent::CNMGSSessionInfoEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
	this->bWndCreated	= FALSE;
}

CNMGSSessionInfoEvent::CNMGSSessionInfoEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo, NMSessionMemberList& aMembers, BOOL bWndCreated )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
	this->aMembers		= aMembers;
	this->bWndCreated	= bWndCreated;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSSessionInfoEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->siInfo );
	LIST_ENCODE( NMSessionMemberList, this->aMembers );
	encoder.Encode1( static_cast<UINT8>( this->bWndCreated ) );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSSessionInfoEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->siInfo );
	LIST_DECODE( NMSessionMemberList, this->aMembers );
	this->bWndCreated = decoder.Decode1();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMGSInviteRejectedEvent
//=========================================

CNMGSInviteRejectedEvent::CNMGSInviteRejectedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey			= 0;
	this->szFromLoginID[ 0 ]	= NULL;
	this->szFromNickName[ 0 ]	= NULL;
	this->uRejectCode			= kRejectCode_Unknown;
}

CNMGSInviteRejectedEvent::CNMGSInviteRejectedEvent( _UInt32_ uSerialKey, CNMSessionInfo& siInfo, LPCTSTR szFromLoginID, LPCTSTR szFromNickName, NMREJECTCODE uRejectCode )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->siInfo		= siInfo;
	_tcsncpy( this->szFromLoginID, szFromLoginID, LOGINID_SIZE );
	_tcsncpy( this->szFromNickName, szFromNickName, NICKNAME_SIZE );
	this->uRejectCode	= uRejectCode;
}

BEGIN_NMEVENT_SERIALIZE( CNMGSInviteRejectedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->siInfo );
	encoder.EncodeString( this->szFromLoginID );
	encoder.EncodeString( this->szFromNickName );
	encoder.Encode4( this->uRejectCode );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMGSInviteRejectedEvent )
//{
	this->uSerialKey	= decoder.Decode4();
	OBJECT_DECODE( this->siInfo );
	decoder.DecodeString( this->szFromLoginID, LOGINID_SIZE );
	decoder.DecodeString( this->szFromNickName, NICKNAME_SIZE );
	this->uRejectCode	= (NMREJECTCODE) decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCSChatMessageReceivedEvent
//=========================================

CNMCSChatMessageReceivedEvent::CNMCSChatMessageReceivedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
	this->bIsHandled	= FALSE;
}

CNMCSChatMessageReceivedEvent::CNMCSChatMessageReceivedEvent( _UInt32_ uSerialKey, CNMChatMessage& cmMessage, BOOL bIsHandled )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->cmMessage		= cmMessage;
	this->bIsHandled	= bIsHandled;
}

BEGIN_NMEVENT_SERIALIZE( CNMCSChatMessageReceivedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	OBJECT_ENCODE( this->cmMessage );
	encoder.Encode1( static_cast<UINT8>( this->bIsHandled ) );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCSChatMessageReceivedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	OBJECT_DECODE( this->cmMessage );
	this->bIsHandled = decoder.Decode1();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMCSMultiChatCreatedEvent
//=========================================

CNMCSMultiChatCreatedEvent::CNMCSMultiChatCreatedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= 0;
	this->uServingProcessID	= 0;
}

CNMCSMultiChatCreatedEvent::CNMCSMultiChatCreatedEvent( _UInt32_ uSerialKey, _UInt32_ uServingProcessID )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= uSerialKey;
	this->uServingProcessID	= uServingProcessID;
}

BEGIN_NMEVENT_SERIALIZE( CNMCSMultiChatCreatedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	encoder.Encode4( this->uServingProcessID );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMCSMultiChatCreatedEvent )
//{
	this->uSerialKey		= decoder.Decode4();
	this->uServingProcessID	= decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMFUSFileUploadEventReceivedEvent
//=========================================

CNMFUSFileUploadEventReceivedEvent::CNMFUSFileUploadEventReceivedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMFUSFileUploadEventReceivedEvent::CNMFUSFileUploadEventReceivedEvent( _UInt32_ uSerialKey, NMFileEventList& aEvent )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->aEvent		= aEvent;
}

BEGIN_NMEVENT_SERIALIZE( CNMFUSFileUploadEventReceivedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	LIST_ENCODE( NMFileEventList, this->aEvent );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMFUSFileUploadEventReceivedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	LIST_DECODE( NMFileEventList, this->aEvent );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMFUSFileUploadEventReceivedEvent
//=========================================

CNMFDSFileDownloadEventReceivedEvent::CNMFDSFileDownloadEventReceivedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
}

CNMFDSFileDownloadEventReceivedEvent::CNMFDSFileDownloadEventReceivedEvent( _UInt32_ uSerialKey, NMFileEventList& aEvent )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->aEvent		= aEvent;
}

BEGIN_NMEVENT_SERIALIZE( CNMFDSFileDownloadEventReceivedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	LIST_ENCODE( NMFileEventList, this->aEvent );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMFDSFileDownloadEventReceivedEvent )
//{
	this->uSerialKey = decoder.Decode4();
	LIST_DECODE( NMFileEventList, this->aEvent );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMWSWhiteBoardMsgReceivedEvent
//=========================================

CNMWSWhiteBoardMsgReceivedEvent::CNMWSWhiteBoardMsgReceivedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
	this->uBufLen		= 0;
}

CNMWSWhiteBoardMsgReceivedEvent::CNMWSWhiteBoardMsgReceivedEvent( _UInt32_ uSerialKey, BYTE* pBuffer, _UInt32_ uBufLen )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->uBufLen		= __min( uBufLen, kMaxDataLen );
	::CopyMemory( this->aBuffer, pBuffer, this->uBufLen );
}

BEGIN_NMEVENT_SERIALIZE( CNMWSWhiteBoardMsgReceivedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	encoder.EncodeBinary( this->aBuffer, this->uBufLen );

END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMWSWhiteBoardMsgReceivedEvent )
//{
	this->uSerialKey	= decoder.Decode4();
	this->uBufLen		= decoder.DecodeBinary( this->aBuffer, kMaxDataLen );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMWSWhiteBoardAckReceivedEvent
//=========================================

CNMWSWhiteBoardAckReceivedEvent::CNMWSWhiteBoardAckReceivedEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= 0;
	this->uContentId	= 0;
}

CNMWSWhiteBoardAckReceivedEvent::CNMWSWhiteBoardAckReceivedEvent( _UInt32_ uSerialKey, _UInt32_ uContentId )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey	= uSerialKey;
	this->uContentId	= uContentId;
}

BEGIN_NMEVENT_SERIALIZE( CNMWSWhiteBoardAckReceivedEvent )
//{
	encoder.Encode4( this->uSerialKey );
	encoder.Encode4( this->uContentId );

END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMWSWhiteBoardAckReceivedEvent )
//{
	this->uSerialKey	= decoder.Decode4();
	this->uContentId	= decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// class CNMWSAssocSerialKeyEvent
//=========================================

CNMWSAssocSerialKeyEvent::CNMWSAssocSerialKeyEvent( void )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= 0;
	this->uAssocSerialKey	= 0;
}

CNMWSAssocSerialKeyEvent::CNMWSAssocSerialKeyEvent( _UInt32_ uSerialKey, _UInt32_ uAssocSerialKey )
	: CNMEvent( kCode, 1 )
{
	this->uSerialKey		= uSerialKey;
	this->uAssocSerialKey	= uAssocSerialKey;
}

BEGIN_NMEVENT_SERIALIZE( CNMWSAssocSerialKeyEvent )
//{
	encoder.Encode4( this->uSerialKey );
	encoder.Encode4( this->uAssocSerialKey );

END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMWSAssocSerialKeyEvent )
//{
	this->uSerialKey		= decoder.Decode4();
	this->uAssocSerialKey	= decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=============================================================================
//	출석체크 이벤트
//=============================================================================

CNMAttendanceEventEvent::CNMAttendanceEventEvent()
	: CNMEvent( kCode, 1 )
	, m_uGameCode( kGameCode_NULL )
	, m_uAttendanceDays( 0 )
{
}

CNMAttendanceEventEvent::CNMAttendanceEventEvent( NMGameCode uGameCode, UINT32 uAttendanceDays )
	: CNMEvent( kCode, 1 )
	, m_uGameCode( uGameCode )
	, m_uAttendanceDays( uAttendanceDays )
{
}

BEGIN_NMEVENT_SERIALIZE( CNMAttendanceEventEvent )
//{
	encoder.Encode4( m_uGameCode );
	encoder.Encode4( m_uAttendanceDays );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMAttendanceEventEvent )
//{
	m_uGameCode			= decoder.Decode4();
	m_uAttendanceDays	= decoder.Decode4();
//}
END_NMEVENT_DESERIALIZE()

//=============================================================================
//	TEMPORARY
//	카트라이더 휴면 계정 이벤트
//=============================================================================

CNMKartDormantEvent::CNMKartDormantEvent()
	: CNMEvent( kCode, 1 )
{
}

CNMKartDormantEvent::CNMKartDormantEvent( const CNMKartDormantList & aKartDormant )
	: CNMEvent( kCode, 1 )
{
	this->aKartDormant = aKartDormant;
}

BEGIN_NMEVENT_SERIALIZE( CNMKartDormantEvent )
//{
	LIST_ENCODE( NMKartDormantList, this->aKartDormant.aKartDormant );
//}
END_NMEVENT_SERIALIZE()

BEGIN_NMEVENT_DESERIALIZE( CNMKartDormantEvent )
//{
	LIST_DECODE( NMKartDormantList, this->aKartDormant.aKartDormant );
//}
END_NMEVENT_DESERIALIZE()

//=========================================
// Etc.
//=========================================

#define	NMEVENT_MAP( eventClassName )	\
		case eventClassName::kCode: return new eventClassName;

CNMEvent* CNMUtil::CreateEventFromCode( _UInt32_ uCode )
{
	switch ( uCode )
	{
		// control
		NMEVENT_MAP( CNMMessengerReplyEvent )
		NMEVENT_MAP( CNMAuthConnectionClosedEvent )
		NMEVENT_MAP( CNMMsgConnectionClosedEvent )
		NMEVENT_MAP( CNMRefreshEvent )
		NMEVENT_MAP( CNMSpecialEvent )
		NMEVENT_MAP( CNMRequestNewFriendEvent )
		NMEVENT_MAP( CNMServerMessageEvent )
		NMEVENT_MAP( CNMCustomMessageEvent )
		NMEVENT_MAP( CNMNoteInstantMsgEvent )
		NMEVENT_MAP( CNMNoteInfoEvent )
		NMEVENT_MAP( CNMRefreshMessageEvent )
		NMEVENT_MAP( CNMFindUserReplyEvent )
		NMEVENT_MAP( CNMInviteVirtualUserEvent )
		NMEVENT_MAP( CNMUserInfoEvent )
		NMEVENT_MAP( CNMGuildOnlineInfoEvent )
		NMEVENT_MAP( CNMGuildOnlineInfoExEvent )
		NMEVENT_MAP( CNMGuildChatMessageEvent )

		NMEVENT_MAP( CNMRealFriendInfoChangedEvent )
		NMEVENT_MAP( CNMVirtualFriendInfoChangedEvent )
		NMEVENT_MAP( CNMFriendInfoChangedEvent )

		NMEVENT_MAP( CNMNotifyEvent )
		NMEVENT_MAP( CNMCustomNotifyEvent )
		
		// chat room
		NMEVENT_MAP( CNMCRChatRoomCreatedEvent )
		NMEVENT_MAP( CNMCRChatRoomEstablishedEvent )
		NMEVENT_MAP( CNMCRChatRoomErrorEvent )
		NMEVENT_MAP( CNMCRChatRoomMemberEvent )
		NMEVENT_MAP( CNMCRChatRoomInfoEvent )
		NMEVENT_MAP( CNMCRChatRoomMessageReceivedEvent )
		NMEVENT_MAP( CNMCRChatRoomCreatedExEvent )
		NMEVENT_MAP( CNMCRChatRoomEstablishedExEvent )
		NMEVENT_MAP( CNMCRChatRoomInfoExEvent )

		// general session
		NMEVENT_MAP( CNMGSSessionCreatedEvent )
		NMEVENT_MAP( CNMGSSessionEstablishedEvent )
		NMEVENT_MAP( CNMGSSessionFailedEvent )
		NMEVENT_MAP( CNMGSSessionClosedEvent )
		NMEVENT_MAP( CNMGSSessionInfoChangedEvent )
		NMEVENT_MAP( CNMGSNewMemberJoinedEvent )
		NMEVENT_MAP( CNMGSMemberLeavedEvent )
		NMEVENT_MAP( CNMGSMemberInfoChangedEvent )
		NMEVENT_MAP( CNMGSSessionInfoEvent )
		NMEVENT_MAP( CNMGSInviteRejectedEvent )

		//// chat session
		NMEVENT_MAP( CNMCSChatMessageReceivedEvent )
		NMEVENT_MAP( CNMCSMultiChatCreatedEvent )

		//// file upload session
		NMEVENT_MAP( CNMFUSFileUploadEventReceivedEvent )

		//// file download session
		NMEVENT_MAP( CNMFDSFileDownloadEventReceivedEvent )

		//// white-board session
		NMEVENT_MAP( CNMWSWhiteBoardMsgReceivedEvent )
		NMEVENT_MAP( CNMWSWhiteBoardAckReceivedEvent )
		NMEVENT_MAP( CNMWSAssocSerialKeyEvent )

		//	이벤트 (출석체크 등 각종 마케팅 관련 이벤트)
		NMEVENT_MAP( CNMAttendanceEventEvent )
		NMEVENT_MAP( CNMKartDormantEvent )
	}

//	check( FALSE );
	return NULL;
}
