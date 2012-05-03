#ifdef _XDEF_NEXONLOGIN

#ifndef __NMCRCHATROOM_H_B91BC9E5_498D_40ae_A74B_731249EE2F82__
#define __NMCRCHATROOM_H_B91BC9E5_498D_40ae_A74B_731249EE2F82__

class CNMCRChatRoom
{
protected:
	_UInt32_			m_uSerialKey;
	CNMChatRoomInfoEx	m_criInfo;

public:
	CNMCRChatRoom();
	virtual ~CNMCRChatRoom();

	void		SetSerialKey( _UInt32_ uSerialKey );
	_UInt32_	GetSerialKey( void ) const;

	BOOL		RegisterCallback( HWND hWnd, _UInt32_ uMsg );
	BOOL		GetRoomInfo( CNMChatRoomInfoEx& criInfo );
	BOOL		ChangeRoomInfo(	LPCTSTR		szRoomTitle = NULL,		// ROOMTITLE_SIZE
								_UInt32_*	pLimitLevel = NULL,
								_UInt32_*	pMaxSize = NULL,
								_UInt32_*	pFlag = NULL,
								LPCTSTR		szPassword = NULL,		// ROOMPWD_SIZE
								NMIDCode*	pOwnerIdCode = NULL );
	BOOL		GetMyInfo( CNMChatRoomMemberInfo& crmiInfo );
	BOOL		GetMemberList( NMChatRoomMemberList& aMembers );
	BOOL		InviteUser( LPCTSTR szToId, LPCTSTR szMessage );	// 1024, 1024
	BOOL		BanUser( NMIDCode uIdCode );
	BOOL		SendChatMessage( NMIDCode uToIdCode[ 10 ], CNMChatMessage& chatMsg );
	BOOL		Close( void );
};

#endif	//	#ifndef __NMCRCHATROOM_H_B91BC9E5_498D_40ae_A74B_731249EE2F82__

#endif