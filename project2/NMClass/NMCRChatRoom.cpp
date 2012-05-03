#include "stdafx.h"
#include "XKernel.h"

#include "NMManager.h"
#include "NMCRChatRoom.h"

CNMCRChatRoom::CNMCRChatRoom()
{
	this->m_uSerialKey		= 0;
}

CNMCRChatRoom::~CNMCRChatRoom()
{
}

void CNMCRChatRoom::SetSerialKey( _UInt32_ uSerialKey )
{
	this->m_uSerialKey = uSerialKey;
}

_UInt32_ CNMCRChatRoom::GetSerialKey( void ) const
{
	return this->m_uSerialKey;
}

BOOL CNMCRChatRoom::RegisterCallback( HWND hWnd, _UInt32_ uMsg )
{
	CNMCRRegisterCallbackFunc	func( this->m_uSerialKey, hWnd, uMsg );

	return	CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCRChatRoom::GetRoomInfo( CNMChatRoomInfoEx& criInfo )
{
	CNMCRGetRoomInfoExFunc		func( this->m_uSerialKey );

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		this->m_criInfo	= func.criInfo;
		criInfo			= func.criInfo;
		return TRUE;
	}
	return FALSE;
}
BOOL CNMCRChatRoom::ChangeRoomInfo(	LPCTSTR		szRoomTitle,	// = NULL, ROOMTITLE_SIZE
									_UInt32_*	pLimitLevel,	// = NULL
									_UInt32_*	pMaxSize,		// = NULL
									_UInt32_*	pFlag,			// = NULL
									LPCTSTR		szPassword,		// = NULL, ROOMPWD_SIZE
									NMIDCode*	pOwnerIdCode )	// = NULL
{
	CNMCRChangeRoomInfoFunc	func( this->m_uSerialKey, szRoomTitle, pLimitLevel, pMaxSize, pFlag, szPassword, pOwnerIdCode );

	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCRChatRoom::GetMyInfo( CNMChatRoomMemberInfo& crmiInfo )
{
	CNMCRGetMemberInfoFunc	func( this->m_uSerialKey, CNMCRGetMemberInfoFunc::kType_MySelf );

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		crmiInfo = func.crmiInfo;
		return TRUE;
	}
	return FALSE;
}

BOOL CNMCRChatRoom::GetMemberList( NMChatRoomMemberList& aMembers )
{
	CNMCRGetMemberListFunc	func( this->m_uSerialKey );

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		aMembers = func.aMembers;
		return TRUE;
	}
	return FALSE;
}

BOOL CNMCRChatRoom::InviteUser( LPCTSTR szToId, LPCTSTR szMessage )
{
	CNMCRInviteUserFunc	func( this->m_uSerialKey, szToId, szMessage );

	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCRChatRoom::BanUser( NMIDCode uIdCode )
{
	CNMCRBanUserFunc	func( this->m_uSerialKey, uIdCode );

	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCRChatRoom::SendChatMessage( NMIDCode uToIdCode[], CNMChatMessage& chatMsg )
{
	CNMCRSendChatMessageFunc	func( this->m_uSerialKey, uToIdCode, chatMsg );

	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCRChatRoom::Close( void )
{
	CNMCRCloseFunc	func( this->m_uSerialKey );

	return CNMManager::GetInstance().CallNMFunc( &func );
}
