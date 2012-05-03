#ifdef _XDEF_NEXONLOGIN

#ifndef __NMCOCLIENTOBJECT_H_ED4FBADF_AB83_4be8_BCD6_4D7D9E06D9B5__
#define __NMCOCLIENTOBJECT_H_ED4FBADF_AB83_4be8_BCD6_4D7D9E06D9B5__

#include "NMGeneral.h"

class CNMCOClientObject
{
	friend class CNMEventHandler;
	DECLARE_NMSINGLETON( CNMCOClientObject )

public:
	BOOL		SetLocale
	(
				IN	NMLOCALEID			uLocaleID,
				IN	UINT32				uRegionCode			= 0
	);

	BOOL		SetConnConfig
	(
				IN	CNMConnConfig const & ConnConfig
	);

	BOOL		SetPatchOption
	(
				IN	BOOL				bPatch
	);

	BOOL		SetUseFriendModuleOption
	(
				IN	BOOL				bUseFriendModule
	);

	BOOL		Initialize
	(
				IN	NMGameCode			uGameCode
	);

	BOOL		RegisterCallbackMessage
	(	
				IN	HWND				hWnd
	,			IN	UINT32				uMessage
	);

	BOOL		ResetCallbackMessage
	(
				IN	HWND				hWnd				= NULL
	);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Authentication Functions
	//
	//////////////////////////////////////////////////////////////////////////
	
	NMLoginAuthReplyCode		
				AttachAuth
	(
				IN	LPCTSTR				pszPassport								//	NXPASSPORT_SIZE
	);

	BOOL		DetachAuth
	(
				void
	);

	NMLoginAuthReplyCode
				LoginAuth
	(
				IN	LPCTSTR				pszNexonID,								//	LOGINID_SIZE
				IN	LPCTSTR				pszPassword								//	PASSWORD_SIZE
	);

	BOOL		LogoutAuth
	(
				void
	);

	LPCTSTR		GetNexonPassport
	(
				IN	LPTSTR				szPassportOut		= NULL				//	NXPASSPORT_SIZE
	);

	NMLoginAuthReplyCode
				LoginAuthSecondary
	(
				IN	LPCTSTR				pszNexonID,								//	LOGINID_SIZE
				IN	LPCTSTR				pszPassword								//	PASSWORD_SIZE
	);

	BOOL		LogoutAuthSecondary
	(
				void
	);

	LPCTSTR		GetNexonPassportSecondary
	(
				IN	LPTSTR				szPassportOut		= NULL				//	NXPASSPORT_SIZE
	);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Messenger functions
	//
	//////////////////////////////////////////////////////////////////////////
	
	BOOL		LoginMessenger
	(
				IN	UINT32				uSupportFlag		= kSupportFlag_FromGameCode
	);

	BOOL		LoginMessengerWithPassport
	(
				IN	LPCTSTR				pszNexonID,			//	LOGINID_SIZE
				IN	LPCTSTR				pszPassword,		//	PASSWORD_SIZE
				IN	PBYTE				pbyPassportData,	//	PASSPORT_SIZE
				IN	INT32				cbyPassportLength,
				IN	UINT32				uSupportFlag		= kSupportFlag_FromGameCode
	);

	BOOL		LogoutMessenger
	(
				void
	);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Virtual User Session Control Functions
	//
	//////////////////////////////////////////////////////////////////////////
	
	//
	//	NOTE: LoginVirtual() and LogoutVirtual() must be called once in one game session
	// 

	BOOL		LoginVirtual
	(
				IN	LPCTSTR				szUserId						//	NICKNAME_SIZE
	,			IN	UINT32				uVirtualIdCode		= 1			//	support for multiple character
	,			IN	LPCTSTR				szPosition			= NULL		//	POSITION_SIZE
	);

	BOOL		LogoutVirtual( void );

	BOOL		GetMyVirtualUserList
	(	
				OUT NMVirtualUserList&	aVirtualUser
	,			IN	NMGameCode			uGameCode			= kGameCode_NULL 
	);
	
	BOOL		ChangeMyPosition
	( 
				IN	LPCTSTR				szPosition			//	POSITION_SIZE
	);

	BOOL		SendRefreshMessage
	( 
				IN	NMREFRESHTYPE		uRefreshType
	);

	BOOL		ChangeMyLevel
	(
				IN UINT32				uLevel,
				IN UINT32				uLevelType = 0
	);

	BOOL		RemoveMyVirtualUser( NMVirtualKey & keyVirtual );

	UINT64		GetDllVersion( void );
	UINT64		GetNMCOSrvVersion( void );

	/* Get My Info Functions*/
	NMIDCode		GetMyIdCode( void );
	UINT32			GetMyOid( void );
	LPCTSTR			GetMyLoginId( void );					//	LOGINID_SIZE
	LPCTSTR			GetMyWord( void );						//	WORD_SIZE
	NMUSERSTATUS	GetMyStatus( void );
	UINT32			GetMyLocalIP( void );
	UINT32			GetMyVirtualIP( void );
	LPCTSTR			GetMyName( void );						//	NXNAME_SIZE
	LPCTSTR			GetMyNickName( void );					//	NICKNAME_SIZE
	UINT32			GetMyMaskInfo( void );
	NMSEXTYPE		GetMySex( void );
	UINT32			GetMyAge( void );
	UINT32			GetMyAreaCode( void );
	LPCTSTR			GetMyAreaName( void );					//	NXAREA_SIZE
	UINT32			GetMySchoolCode( void );
	LPCTSTR			GetMySchoolName( void );				//	NXSCHOOL_SIZE
	SYSTEMTIME		GetMyBirthday( void );
	CNMRealUserInfo	GetMyInfo( void );

	UINT32			GetUnreadNoteCount( void );

	//	행사 등 이벤트
	BOOL			RequestNews();
	BOOL			CheckNews( NMGameCode uEventGameCode, NMVirtualKey keyVirtual = keyVirtualAll );

	/* Change My Info Functions */
	BOOL		ChangeMyStatus
	( 
				IN	NMUSERSTATUS		uStatus
	);

	BOOL		ChangeMyWord
	( 
				IN	LPCTSTR				szWord				//	WORD_SIZE
	);

	BOOL		ChangeMyNickName
	( 
				IN	LPCTSTR				szNickName			//	NICKNAME_SIZE
	);

	BOOL		ChangeMyMaskInfo
	( 
				IN	UINT32				uMaskInfo
	);

	BOOL		ChangeNickname
	(
				IN NMVirtualKey			keyCharacter
	,			IN LPCTSTR				szNickname
	);

	BOOL		ChangeMyInfo
	(	
				IN	NMUSERSTATUS		uStatus			= kStatus_Unknown
	,			IN	LPCTSTR				szWord			= NULL					//	WORD_SIZE
	,			IN	LPCTSTR				szNickName		= NULL					//	NICKNAME_SIZE
	,			IN	UINT32				uMaskInfo		= 0xFFFFFFFF
	);

	/* Get User Config */
	NMALLOWTYPE		GetAllowChatType( void );
	NMALLOWTYPE		GetAllowNoteType( void );
	NMALLOWTYPE		GetAllowGameInviteType( void );
	NMALLOWTYPE		GetAllowRequestFriendType( void );

	/* Set User Config */
	BOOL		ChangeAllowChatType
	( 
				IN	NMALLOWTYPE			uType
	);

	BOOL		ChangeAllowNoteType
	( 
				IN	NMALLOWTYPE			uType
	);

	BOOL		ChangeAllowGameInviteType
	( 
				IN	NMALLOWTYPE			uType
	);
	
	BOOL		ChangeAllowRequestFriendType
	( 
				IN	NMALLOWTYPE			uType
	);

	/* User Local Config */
	BOOL		GetLocalConfig
	(
				IN	NMLOCALCONFIGTYPE	uConfigType
	,			OUT	LPTSTR				szValueOut			//	1024
	);

	BOOL		ChangeLocalConfig
	(
				IN	NMLOCALCONFIGTYPE	uConfigType
	,			IN	LPCTSTR				szValue				//	1024
	);

	/* Friend Request Related Functions */
	BOOL		RequestNewFriend
	(	
				IN	LPCTSTR				szRequestId				//	LOGINID_SIZE
	,			IN	LPCTSTR				szMessage				//	MSG_SIZE
	,			IN	BOOL				bAddToNxFriend			= FALSE
	,			IN	NMGameCode			uGameCode				= kGameCode_NULL
	,			IN	NMGameCode			uRequesteeGameCode		= kGameCode_NULL
	,			IN	UINT32				uVirtualIdCode			= 0
	,			IN	UINT32				uRequesteeVirtualIdCode	= 0
	);

	BOOL		ConfirmNewFriend
	(	
				IN	UINT32				uSerialNo
	,			IN	NMCONFIRMCODE		uConfirmCode
	);

	/* Category Related Functions */
	BOOL		AddCategory
	(	
				IN	NMVirtualKey		keyOwner
	,			IN	LPCTSTR				szCategoryName		//	CATEGORY_SIZE
	,			IN	NMCATEGORYPROPERTY	uProperty			= kCateProp_None
	,			IN	NMALLOWTYPE			uAllowType			= kConfigAllowValue_FromAll
	);

	BOOL		DeleteCategory
	( 
				IN	UINT32				uCategoryCode
	,			IN	BOOL				bSystemCall			= FALSE
	);

	BOOL		ChangeCategoryName
	(	
				IN	UINT32				uCategoryCode
	,			IN	LPCTSTR				szCategoryName		//	CATEGORY_SIZE
	,			IN	BOOL				bSystemCall			= FALSE
	);

	BOOL		ChangeCategoryProperty
	(
				IN	UINT32				uCategoryCode
	,			IN	NMCATEGORYPROPERTY	uProperty
	,			IN	BOOL				bSystemCall			= FALSE
	);

	BOOL		ChangeCategoryAllowType
	(
				IN	UINT32				uCategoryCode
	,			IN	NMALLOWTYPE			uAllowType
	,			IN	BOOL				bSystemCall			= FALSE
	);

	/* Friends List Functions */
	BOOL		GetFriendList
	(
				OUT	NMCategoryUserList&		
										aCateFriend
	,			OUT	NMFriendList&		aUnrelated
	,			IN	BOOL				bAll				= FALSE
	);

	BOOL		GetFriendListEx
	(
				OUT	NMCategoryUserList&		
										aCateFriend
	,			OUT	NMFriendList&		aUnrelated
	,			IN	NMVirtualKey		keyVirtual			= keyVirtualAll
	);

	BOOL		GetFriendInfo
	(
				IN	NMFriendKey			keyFriend
	,			OUT	CNMFriendInfo&		fiFriendInfo
	);

	BOOL		BlockFriend
	(	
				IN	NMFriendKey			keyFriend
	,			IN	BOOL				bBlock
	);

	BOOL		AddFriendToCategory
	(
				IN	NMFriendKey			keyFriend
	,			IN	UINT32				uCategoryCode
	,			IN	BOOL				bSystemCall			= FALSE
	);

	BOOL		DeleteFriendFromCategory
	(			
				IN	NMFriendKey			keyFriend
	,			IN	UINT32				uCategoryCode
	,			IN	BOOL				bSystemCall			= FALSE
	);

	BOOL		MoveFriendCategory
	(	
				IN	NMFriendKey			keyFriend
	,			IN	UINT32				uFromCategoryCode
	,			IN	UINT32				uToCategoryCode
	,			IN	BOOL				bSystemCall			= FALSE
	);

	BOOL		ChangeFriendMemo
	(
				IN	NMIDCode			uIDCode
	,			IN	LPCTSTR				szMemo				//	MEMO_SIZE
	);

	//
	//	rejected user list
	//

	BOOL		GetRejectedUserList
	(
				OUT	NMRejectedUserList &	aRejectedUserList
	);

	BOOL		AppendRejectedUser
	(
				IN	CNMRejectedUser &		rejectedUser
	);

	BOOL		RemoveRejectedUser
	(
				IN	CNMRejectedUser	&		rejectedUser
	);

	/* Note Functions */
	BOOL		SendNoteMsg
	(	
				IN	CNMNoteInfo::NMNOTETYPE		uNoteType
	,			IN	UINT32				uSubType
	,			IN	BOOL				bSaveToPermBox
	,			IN	LPCTSTR				szTargetId								//	TARGETID_SIZE
	,			IN	LPCTSTR				szMessage								//	NOTEMSG_SIZE
	,			IN	LPCTSTR				szArgument		= NULL					//	NOTEARG_SIZE
	,			IN	LPCTSTR				szCustomData	= NULL					//	NOTECUSTOM_SIZE
	,			IN	NMGameCode			uGameCode		= kGameCode_NULL
	,			IN	UINT32				uVirtualIdCode	= 1
	);

	BOOL		SendNoteExMsg
	(	
				IN	CNMNoteInfoEx::NMNOTEEXTYPE	
										uNoteExType
	,			IN	LPCTSTR				szMessage								//	NOTEEXMSG_SIZE
	);

	BOOL		SendReportMsg
	(	
				IN	CNMReportInfo::NMREPORTTYPE		
										uType
	,			IN	CNMReportInfo::NMREPORTSUBTYPE	
										uSubType
	,			IN	LPCTSTR				szTargetId								//	LOGINID_SIZE
	,			IN	LPCTSTR				szReason								//	4000
	,			IN	LPCTSTR				szEvidence								//	20000
	,			IN	NMGameCode			uGameCode		= kGameCode_NULL
	,			IN	NMGameCode			uAppliedGameCode= kGameCode_NULL
	);

	NMNoteList	GetNoteBox
	(
				IN	NMNOTEBOXTYPE		uBoxType
	,			IN	UINT32				uNoteSerial
	);

	BOOL		GetNoteBox
	(
				IN	NMNOTEBOXTYPE		uBoxType
	,			IN	UINT32				uNoteSerial
	,			OUT NMNoteInfoList &	noteInfoList
	);

	BOOL		SetNoteReadFlag
	(
				IN	UINT32			uNoteSerial
	);

	BOOL		SaveNoteToPermBox
	(
				IN	UINT32			uNoteSerial
	);

	BOOL		DeleteNote
	(
				IN	UINT32			uNoteSerial
	);

	/* Guild Functions */
	CNMGuildListExInfo		
				GetMyGuildList( void );

	//	guild list for character
	/*
	BOOL		GetGuildList
	(
				OUT	CNMGuildListExInfo & guildList
	,			IN	CNMCharacter const & gameCharacter
	);
	*/

	BOOL		MonitorGuildOnlineInfoEx
	(		
				IN	NMGuildKey			uGuildKey
	,			IN	BOOL				bAttach
	);

	BOOL		GetGuildOnlineInfoEx
	(
				IN	NMGuildKey				uGuildKey
	,			OUT	CNMGuildOnlineInfoEx&	goiInfo
	);

	BOOL		SendGuildChatMessage
	(
				IN	NMGuildKey				uGuildKey
	,			IN	CNMChatMessage			cmChatMsg
	);

	/* Client Util */
	BOOL		GetGameList
	(
				OUT	CNMGameListData*	pglGameList
	);

	BOOL		GetUserInfo
	(
				IN	LPCTSTR				szTargetID						//	LOGINID_SIZE
	,			IN	NMGameCode			uGameCode	= kGameCode_NULL
	);

	BOOL		SendCustomNotify
	(
				IN UINT32				uType
	,			IN LPCTSTR				szArgument						//	MSG_SIZE
	,			IN UINT32				uPolicyFlag = CNMCustomNotifyData::kCustomNotifyPolicy_Default
	);

	/* Session Request Functions */
	BOOL		RequestChatSession
	(
				IN	NMFriendCode		fcFriendCode
	);

	BOOL		RequestMultiChatSession
	( 
				IN	NMFriendCodeList&	aFriendCodeList
	,			IN	NMVirtualKey		keyVirtuality	= keyVirtualAll
	);

	BOOL		RequestFileUploadSession
	( 
				IN	NMFriendCode		fcFriendCode
	,			IN	NMFilePathList&		aFilePath
	);

	BOOL		RequestChatSession
	(
				IN	NMFriendKey			keyFriend
	);

	BOOL		RequestMultiChatSession
	( 
				IN	NMFriendKeyList&	aFriendKey
	,			IN	NMVirtualKey		keyVirtuality	= keyVirtualAll
	);

	BOOL		RequestFileUploadSession
	( 
				IN	NMFriendKey			keyFriend
	,			IN	NMFilePathList&		aFilePath
	);

	BOOL		RequestChatSessionToGuildMember
	(
				IN	LPCTSTR				szRequestID
	,			IN	NMVirtualKey		keyVirtuality	= keyVirtualAll
	);

	BOOL		RequestFileUploadSessionToGuildMember
	(
				IN	LPCTSTR				szRequestID
	,			IN	NMFilePathList&		aFilePath
	,			IN	NMVirtualKey		keyVirtuality	= keyVirtualAll
	);

	/* Chat Room Request Functions */
	BOOL		CreateChatRoomEx
	(	
				IN	UINT32				uRoomType
	,			IN	NMGuildKey			uGuildKey
	,			IN	LPCTSTR				szGuildTitle				//	GUILDNAME_SIZE
	,			IN	LPCTSTR				szRoomTitle					//	ROOMTITLE_SIZE
	,			IN	UINT32				uLimitLevel
	,			IN	UINT32				uMaxSize
	,			IN	UINT32				uFlag
	,			IN	LPCTSTR				szPassword					//	ROOMPWD_SIZE
	,			IN	LPCTSTR				szServerAddr				//	32
	,			IN	UINT16				uServerPort
	,			IN	UINT32				uVirtualIdCode = 1
	);

	BOOL		JoinToChatRoomEx
	(	
				IN	NMGuildKey			uGuildKey
	,			IN	UINT32				uRoomSerialNo
	,			IN	LPCTSTR				szPassword					//	ROOMPWD_SIZE
	,			IN	LPCTSTR				szServerAddr				//	32
	,			IN	UINT16				uServerPort
	,			IN	UINT32				uFlag			= 0
	,			IN	UINT32				uVirtualIdCode	= 1
	);

	BOOL		JoinToDefaultGuildChatRoom
	(
				IN	NMVirtualKey		keyVirtual
	);

protected:
	typedef std::vector< CNMGuildOnlineInfoEx >	NMGOIList;

protected:
	CRITICAL_SECTION	m_cs;

	CNMRealUserInfo		m_ruiRealUserInfo;
	UINT64				m_uSessionKeyHigh;
	UINT64				m_uSessionKeyLow;
	UINT32				m_uUnreadNoteCount;
	TCHAR				m_szPassport[ PASSPORT_SIZE ];

	NMGOIList			m_aGOIs;

	CNMCOClientObject( void );
public:
	~CNMCOClientObject( void );

protected:
	void		ResetMyInfo( void );
};

#endif	//	#ifndef __NMCOCLIENTOBJECT_H_ED4FBADF_AB83_4be8_BCD6_4D7D9E06D9B5__

#endif