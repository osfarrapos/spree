#include "stdafx.h"
#include "XKernel.h"

#include "NMManager.h"
#include "NMGSGeneralSession.h"

#include "NMCOClientObject.h"

CNMGSGeneralSession::CNMGSGeneralSession( NMSESSIONTYPE uType )
{
	this->m_siInfo.uType	= uType;
	this->m_uSerialKey		= 0;
	this->m_bNickNameValid	= FALSE;

	::ZeroMemory( this->m_szTempBuffer, sizeof( this->m_szTempBuffer ) );
	::ZeroMemory( this->m_szMyNickName, sizeof( this->m_szMyNickName ) );
	::ZeroMemory( this->m_szOppositeNickName, sizeof( this->m_szOppositeNickName ) );
}

CNMGSGeneralSession::~CNMGSGeneralSession()
{
}

void CNMGSGeneralSession::SetSerialKey( _UInt32_ uSerialKey )
{
	this->m_uSerialKey = uSerialKey;
}

_UInt32_ CNMGSGeneralSession::GetSerialKey( void ) const
{
	return this->m_uSerialKey;
}

_UInt32_ CNMGSGeneralSession::GetType( void ) const
{
	return this->m_siInfo.uType;
}

NMFriendCode CNMGSGeneralSession::GetAssocFriendCode( void )
{
	CNMGSGetSessionInfoFunc	
				func( this->m_uSerialKey );
	
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		this->m_siInfo	= func.siInfo;
	}

	return this->m_siInfo.fcAssocFriend;
}

BOOL CNMGSGeneralSession::IsPassive( void )
{
	CNMGSGetSessionInfoFunc	
				func( this->m_uSerialKey );
	
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		(*(&this->m_siInfo))	= func.siInfo;
	}

	return this->m_siInfo.bPassive;
}

_UInt32_ CNMGSGeneralSession::GetServingProcessID( void )
{
	CNMGSGetSessionInfoFunc	
				func( this->m_uSerialKey );
	
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		(*(&this->m_siInfo))	= func.siInfo;
	}

	return this->m_siInfo.uServingProcessID;
}

LPCTSTR CNMGSGeneralSession::GetMyNickName( BOOL bHasTail )
{
	if ( !this->m_bNickNameValid )
	{
		_tcscpy( this->m_szMyNickName, CNMCOClientObject::GetInstance().GetMyNickName() );

		if ( this->m_siInfo.uStatus == CNMSessionInfo::kStatus_SessionEstablished )
		{
			NMVirtualUserList	aVirtualUser;
			
			CNMCOClientObject::GetInstance().GetMyVirtualUserList( aVirtualUser, this->m_siInfo.keyVirtuality.uGameCode );

			if ( !aVirtualUser.empty() )
			{
				for ( _UInt32_ i = 0; i < aVirtualUser.size(); ++i )
				{
					if ( aVirtualUser[ i ].keyVirtual == this->m_siInfo.keyVirtuality )
					{
						_tcscpy( this->m_szMyNickName, aVirtualUser[ i ].szNickName );
						this->m_bNickNameValid	= TRUE;
						break;
					}
				}
			}
		}
	}

	if ( bHasTail )
	{
		CNMGetGameShortNameFunc func( m_siInfo.keyVirtuality.uGameCode );
		if ( CNMManager::GetInstance().CallNMFunc(&func) )
		{
			::_stprintf( this->m_szTempBuffer, _T("%s@%s"), this->m_szMyNickName, func.szGameShortName );
		}
	}
	else
	{
		_tcscpy( this->m_szTempBuffer, this->m_szMyNickName );
	}

	return this->m_szTempBuffer;
}

LPCTSTR CNMGSGeneralSession::GetOppositeNickName( BOOL bHasTail )
{
	BOOL					bValid	= FALSE;
	NMSessionMemberList		aMembers;

	this->GetMemberList( aMembers );

	if ( aMembers.size() == 2 )
	{
		if ( aMembers[ 0 ].uIDCode == CNMCOClientObject::GetInstance().GetMyIdCode() )
			_tcscpy( this->m_szOppositeNickName, aMembers[ 1 ].szNickName );
		else
			_tcscpy( this->m_szOppositeNickName, aMembers[ 0 ].szNickName );

		bValid	= TRUE;
	}
	else
	{
		_tcscpy( this->m_szOppositeNickName, _T( "¾øÀ½" ) );
	}

	if ( bValid && bHasTail )
	{
		CNMGetGameShortNameFunc func( m_siInfo.keyVirtuality.uGameCode );
		if ( CNMManager::GetInstance().CallNMFunc(&func) )
		{
			::_stprintf( this->m_szTempBuffer, _T("%s@%s"), this->m_szOppositeNickName, func.szGameShortName );
		}
	}
	else
	{
		_tcscpy( this->m_szTempBuffer, this->m_szOppositeNickName );
	}

	return this->m_szTempBuffer;
}

CNMSessionMemberInfo CNMGSGeneralSession::GetOppositeMemberInfo( void )
{
	CNMSessionMemberInfo	smiInfo;
	NMSessionMemberList		aMembers;

	this->GetMemberList( aMembers );

	if ( aMembers.size() == 2 )
	{
		if ( aMembers[ 0 ].uIDCode == CNMCOClientObject::GetInstance().GetMyIdCode() )
			smiInfo	= aMembers[ 1 ];
		else
			smiInfo	= aMembers[ 0 ];
	}

	return	smiInfo;
}

BOOL CNMGSGeneralSession::RegisterCallback( HWND hWnd, _UInt32_ uMsg )
{
	CNMGSRegisterCallbackFunc	
				func( this->m_uSerialKey, hWnd, uMsg );
	return		( CNMManager::GetInstance().CallNMFunc( &func ) );
}

BOOL CNMGSGeneralSession::WantClose( void )
{
	CNMGSWantCloseFunc	
				func( this->m_uSerialKey, FALSE );
	return		( CNMManager::GetInstance().CallNMFunc( &func ) );
}

BOOL CNMGSGeneralSession::InviteUser( NMFriendCode fcFriend )
{
	CNMGSInviteUserFunc	
				func( this->m_uSerialKey, fcFriend );
	return		( CNMManager::GetInstance().CallNMFunc( &func ) );
}

BOOL CNMGSGeneralSession::InviteUser( NMFriendKey keyFriend )
{
	CNMGSInviteUserExFunc	
				func( this->m_uSerialKey, keyFriend );
	return		( CNMManager::GetInstance().CallNMFunc( &func ) );
}

BOOL CNMGSGeneralSession::GetSessionInfo( void )
{
	if ( this->m_uSerialKey == 0 )
	{
		CNMSessionInfo	siNew;

		this->m_siInfo	= siNew;

		return TRUE;
	}
	else
	{
		CNMGSGetSessionInfoFunc	
					func( this->m_uSerialKey );
		
		if ( CNMManager::GetInstance().CallNMFunc( &func ) )
		{
			this->m_siInfo		= func.siInfo;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CNMGSGeneralSession::GetMemberList( NMSessionMemberList& aMembers )
{
	CNMGSGetMemberListFunc	
				func( this->m_uSerialKey );
	
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		aMembers	= func.aMembers;
		return TRUE;
	}

	return FALSE;
}

BOOL CNMGSGeneralSession::GetInviteCandidateList( NMFriendList& aCandidates )
{
	CNMGSGetInviteCandidateListFunc	
				func( this->m_uSerialKey );
	
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		aCandidates		= func.aCandidates;
		return TRUE;
	}

	return FALSE;
}
