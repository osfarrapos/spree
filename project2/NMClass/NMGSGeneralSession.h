#ifdef _XDEF_NEXONLOGIN

#ifndef __NMGSGENERALSESSION_H_CC270527_CD76_46d8_9BAC_E22EAE06559E__
#define __NMGSGENERALSESSION_H_CC270527_CD76_46d8_9BAC_E22EAE06559E__

class CNMGSGeneralSession
{
	friend class			NMManager;
protected:
	BOOL					m_bNickNameValid;
	TCHAR					m_szTempBuffer[ 256 ];
	TCHAR					m_szMyNickName[ NICKNAME_SIZE ];
	TCHAR					m_szOppositeNickName[ NICKNAME_SIZE ];
	_UInt32_				m_uSerialKey;
	CNMSessionInfo			m_siInfo;

public:
	CNMGSGeneralSession( NMSESSIONTYPE uType = kSessionType_General );
	virtual ~CNMGSGeneralSession();

	void					SetSerialKey( _UInt32_ uSerialKey );
	_UInt32_				GetSerialKey( void ) const;
	_UInt32_				GetType( void ) const;
	NMFriendCode			GetAssocFriendCode( void );
	BOOL					IsPassive( void );
	_UInt32_				GetServingProcessID( void );
	CNMSessionInfo&			GetSIInfo( void ) { return this->m_siInfo; }
	
	LPCTSTR					GetMyNickName( BOOL bHasTail = FALSE );
	LPCTSTR					GetOppositeNickName( BOOL bHasTail= FALSE );
	CNMSessionMemberInfo	GetOppositeMemberInfo( void );

	// For Window Messaging
	BOOL					RegisterCallback( HWND hWnd, _UInt32_ uMsg );
	BOOL					WantClose( void );

	// For Invite
	BOOL					InviteUser( NMFriendCode fcFriend );
	BOOL					InviteUser( NMFriendKey keyFriend );

	// For Member Info
	BOOL					GetSessionInfo( void );
	BOOL					GetMemberList( NMSessionMemberList& aMembers );
	BOOL					GetInviteCandidateList( NMFriendList& aCandidates );
};

#endif	//	#ifndef __NMGSGENERALSESSION_H_CC270527_CD76_46d8_9BAC_E22EAE06559E__

#endif
