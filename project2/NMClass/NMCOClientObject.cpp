#include "stdafx.h"
#include "XKernel.h"
#include "NMDataObject.h"
#include "NMFunctionObject.h"
#include "NMManager.h"
#include "NMCOClientObject.h"

#ifndef STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_DEPRECATE
#endif
#include <strsafe.h>

CNMCOClientObject::CNMCOClientObject()
{
	::ZeroMemory( &this->m_cs, sizeof( this->m_cs ) );
	::InitializeCriticalSection( &this->m_cs );
}

CNMCOClientObject::~CNMCOClientObject()
{
	::DeleteCriticalSection( &this->m_cs );
}

void CNMCOClientObject::ResetMyInfo( void )
{
	NMCSLOCK( this->m_cs )
	{
		CNMRealUserInfo		ruiRealUserInfo;

		this->m_ruiRealUserInfo		= ruiRealUserInfo;
		this->m_uSessionKeyHigh		= 0;
		this->m_uSessionKeyLow		= 0;
		::ZeroMemory( this->m_szPassport, sizeof( this->m_szPassport ) );
		this->m_aGOIs.clear();
	}
}

BOOL CNMCOClientObject::SetLocale
(
	NMLOCALEID	uLocaleID,
	UINT32		uRegionCode
)
{
	if ( CNMManager::GetInstance().SetLocale( uLocaleID, uRegionCode ) )
	{
		CNMSetLocaleFunc func( uLocaleID, uRegionCode );
		return CNMManager::GetInstance().CallNMFunc( &func );
	}
	return FALSE;
}

BOOL CNMCOClientObject::SetConnConfig
(
	const CNMConnConfig &	ConnConfig
)
{
	CNMSetConnConfigFunc func( ConnConfig );

	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::SetPatchOption
(
	BOOL		bPatch
)
{
	return CNMManager::GetInstance().SetPatchOption( bPatch );
}

BOOL CNMCOClientObject::SetUseFriendModuleOption
(
	BOOL		bUseFriendModule
)
{
	return CNMManager::GetInstance().SetUseFriendModuleOption( bUseFriendModule );
}

BOOL CNMCOClientObject::Initialize
(
	NMGameCode	uGameCode
)
{
	CNMInitializeFunc func( uGameCode, static_cast<UINT32>( kSupportFlag_FromGameCode ) );
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RegisterCallbackMessage
( 
	HWND		hWnd,
	UINT32		uMessage
)
{
	CNMRegisterCallbackFunc func( hWnd, uMessage );
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ResetCallbackMessage
(
	HWND		hWnd
)
{
	CNMResetCallbackFunc func( hWnd );
	return CNMManager::GetInstance().CallNMFunc( &func );
}


NMLoginAuthReplyCode CNMCOClientObject::AttachAuth( LPCTSTR pszPassport )
{
	CNMLoginAuthFunc func( pszPassport );
	CNMManager::GetInstance().CallNMFunc( &func );
	return (NMLoginAuthReplyCode) func.nErrorCode;
}

BOOL CNMCOClientObject::DetachAuth()
{
	CNMLogoutAuthFunc func;
	return CNMManager::GetInstance().CallNMFunc( &func );
}

NMLoginAuthReplyCode CNMCOClientObject::LoginAuth( LPCTSTR pszNexonID, LPCTSTR pszPassword )
{
	CNMLoginAuthFunc func( pszNexonID, pszPassword );
	CNMManager::GetInstance().CallNMFunc( &func );
	return (NMLoginAuthReplyCode) func.nErrorCode;
}

BOOL CNMCOClientObject::LogoutAuth()
{
	CNMLogoutAuthFunc func;
	return CNMManager::GetInstance().CallNMFunc( &func );
}

LPCTSTR CNMCOClientObject::GetNexonPassport( LPTSTR szPassportOut )
{
	CNMGetNexonPassportFunc func;
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		NMSAFE_STRCOPY( szPassportOut, func.szNexonPassport, NXPASSPORT_SIZE );
		return szPassportOut;
	}

	if ( szPassportOut )
	{
		szPassportOut[0] = 0;
	}

	return NULL;
}

NMLoginAuthReplyCode CNMCOClientObject::LoginAuthSecondary( LPCTSTR pszNexonID, LPCTSTR pszPassword )
{
	CNMLoginAuthFunc func( pszNexonID, pszPassword );
	func.bSecondary = TRUE;
	CNMManager::GetInstance().CallNMFunc( &func );
	return (NMLoginAuthReplyCode) func.nErrorCode;
}

BOOL CNMCOClientObject::LogoutAuthSecondary()
{
	CNMLogoutAuthFunc func;
	func.bSecondary = TRUE;
	return CNMManager::GetInstance().CallNMFunc( &func );
}

LPCTSTR CNMCOClientObject::GetNexonPassportSecondary( LPTSTR szPassportOut )
{
	CNMGetNexonPassportFunc func;
	func.bSecondary = TRUE;
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		NMSAFE_STRCOPY( szPassportOut, func.szNexonPassport, NXPASSPORT_SIZE );
		return szPassportOut;
	}

	if ( szPassportOut )
	{
		szPassportOut[0] = 0;
	}

	return NULL;
}

BOOL CNMCOClientObject::LoginMessenger
(
	UINT32		uSupportFlag
)
{
	CNMLoginMessengerFunc func( uSupportFlag );
	return CNMManager::GetInstance().CallNMFunc(&func);
}

BOOL CNMCOClientObject::LoginMessengerWithPassport
(
	LPCTSTR		pszNexonID,
	LPCTSTR		pszPassword,
	PBYTE		pbyPassportData,
	INT32		cbyPassportLength,
	UINT32		uSupportFlag
)
{
	CNMLoginPassportFunc func( pszNexonID, pszPassword, pbyPassportData, cbyPassportLength, uSupportFlag );
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::LogoutMessenger
(
	void
)
{
	CNMLogoutMessengerFunc func;
	return CNMManager::GetInstance().CallNMFunc(&func);
}

BOOL CNMCOClientObject::LoginVirtual
(	
	LPCTSTR		szUserId,
	UINT32		uVirtualIdCode,
	LPCTSTR		szPosition
)
{
	CNMLoginVirtualFunc	func( szUserId, uVirtualIdCode, szPosition );
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::LogoutVirtual
( 
	void 
)
{
	CNMLogoutVirtualFunc func;
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::GetMyVirtualUserList
(	
	NMVirtualUserList&	
					aVirtualUser
,	_UInt32_		uGameCode				// = kGameCode_NULL
)
{
	CNMGetMyVirtualUserListFunc		
				func( uGameCode );

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		aVirtualUser	= func.aVirtualUser;
		return TRUE;
	}

	return FALSE;
}

BOOL CNMCOClientObject::ChangeMyPosition
( 
	LPCTSTR			szPosition				// POSITION_SIZE
)	
{
	CNMChangeMyPositionFunc		
				func( szPosition );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::SendRefreshMessage
( 
	NMREFRESHTYPE	uRefreshType
)
{
	CNMSendRefreshMessageFunc
					func( uRefreshType );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeMyLevel
(
	UINT32			uLevel,
	UINT32			uLevelType /* = 0 */
)
{
	CNMChangeMyLevelFunc	func( uLevel, uLevelType );
	return					CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RemoveMyVirtualUser
(
	NMVirtualKey & keyVirtual
)
{
	CNMRemoveMyVirtualUserFunc		func( keyVirtual );
	return CNMManager::GetInstance().CallNMFunc( &func );
}

_UInt64_ CNMCOClientObject::GetDllVersion
( 
	void 
)
{
	CNMGetVersionFunc	
				func( CNMGetVersionFunc::kDll );
	return		( CNMManager::GetInstance().CallNMFunc( &func ) ? func.uVersion : 0 );
}

_UInt64_ CNMCOClientObject::GetNMCOSrvVersion
( 
	void
)
{
	CNMGetVersionFunc	
				func( CNMGetVersionFunc::kCore );
	return		( CNMManager::GetInstance().CallNMFunc( &func ) ? func.uVersion : 0 );
}

_UInt64_ CNMCOClientObject::GetMyIdCode
( 
	void 
)
{
	return		this->m_ruiRealUserInfo.uIDCode;
}

_UInt32_ CNMCOClientObject::GetMyOid
( 
	void
)
{
	return		GET_OID( this->m_ruiRealUserInfo.uIDCode );
}

LPCTSTR CNMCOClientObject::GetMyLoginId
(
	void
)		
{
	return		this->m_ruiRealUserInfo.szLoginID;
}

LPCTSTR CNMCOClientObject::GetMyWord
(
	void
)
{
	return		this->m_ruiRealUserInfo.szWord;
}

NMUSERSTATUS CNMCOClientObject::GetMyStatus
( 
	void
)
{
	return		this->m_ruiRealUserInfo.uStatus;
}

_UInt32_ CNMCOClientObject::GetMyLocalIP
( 
	void
)
{
	return		this->m_ruiRealUserInfo.uLocalIP;
}

_UInt32_ CNMCOClientObject::GetMyVirtualIP
( 
	void
)
{
	return		this->m_ruiRealUserInfo.uVirtualIP;
}

LPCTSTR CNMCOClientObject::GetMyName
(
	void
)
{
	return		this->m_ruiRealUserInfo.szName;
}

LPCTSTR CNMCOClientObject::GetMyNickName
(
	void
)
{
	return		this->m_ruiRealUserInfo.szNickName;
}

_UInt32_ CNMCOClientObject::GetMyMaskInfo
(
	void
)
{
	return		this->m_ruiRealUserInfo.uMaskInfo;
}

NMSEXTYPE CNMCOClientObject::GetMySex
( 
	void
)
{
	return		this->m_ruiRealUserInfo.uSex;
}

_UInt32_ CNMCOClientObject::GetMyAge
(
	void
)
{
	return		this->m_ruiRealUserInfo.uAge;
}

_UInt32_ CNMCOClientObject::GetMyAreaCode
(
	void
)
{
	return		this->m_ruiRealUserInfo.uAreaCode;
}

LPCTSTR CNMCOClientObject::GetMyAreaName
( 
	void
)
{
	return		this->m_ruiRealUserInfo.szAreaName;
}

_UInt32_ CNMCOClientObject::GetMySchoolCode
(
	void
)
{
	return		this->m_ruiRealUserInfo.uSchoolCode;
}

LPCTSTR CNMCOClientObject::GetMySchoolName
(
	void
)
{
	return		this->m_ruiRealUserInfo.szSchoolName;
}

SYSTEMTIME CNMCOClientObject::GetMyBirthday
(
	void
)
{
	return		this->m_ruiRealUserInfo.stBirthday;
}

CNMRealUserInfo CNMCOClientObject::GetMyInfo
(
	void
)
{
	CNMGetMyInfoFunc	
					func;

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		NMCSLOCK( this->m_cs )
		{
			this->m_ruiRealUserInfo		= func.ruiUserInfo;
			this->m_uSessionKeyHigh		= func.uSessionKeyHigh;
			this->m_uSessionKeyLow		= func.uSessionKeyLow;
			this->m_uUnreadNoteCount	= func.uUnreadNoteCount;
			::_tcscpy( this->m_szPassport, func.szPassport );
		}
		return this->m_ruiRealUserInfo;
	}

	return this->m_ruiRealUserInfo;
}

BOOL CNMCOClientObject::ChangeMyStatus
( 
	NMUSERSTATUS	uStatus 
)
{
	return		this->ChangeMyInfo( uStatus, NULL, NULL, 0xFFFFFFFF );
}

BOOL CNMCOClientObject::ChangeMyWord
( 
	LPCTSTR			szWord					// WORD_SIZE
)			
{
	return		this->ChangeMyInfo( kStatus_Unknown, szWord, NULL, 0xFFFFFFFF );
}

BOOL CNMCOClientObject::ChangeMyNickName
( 
	LPCTSTR			szNickName				// NICKNAME_SIZE
)	
{
	return		this->ChangeMyInfo( kStatus_Unknown, NULL, szNickName, 0xFFFFFFFF );
}

BOOL CNMCOClientObject::ChangeMyMaskInfo
( 
	_UInt32_		uMaskInfo 
)
{
	return		this->ChangeMyInfo( kStatus_Unknown, NULL, NULL, uMaskInfo );
}

BOOL CNMCOClientObject::ChangeNickname
(
	NMVirtualKey	keyCharacter,
    LPCTSTR			szNickname
)
{
	if (szNickname)
	{
		CNMChangeNicknameFunc func( keyCharacter, szNickname );
		return CNMManager::GetInstance().CallNMFunc( &func );
	}
	return FALSE;
}

BOOL CNMCOClientObject::ChangeMyInfo
(	
	NMUSERSTATUS	uStatus					// = kStatus_Unknown
,	LPCTSTR			szWord					// WORD_SIZE
,	LPCTSTR			szNickName				// NICKNAME_SIZE
,	_UInt32_		uMaskInfo				// = 0xFFFFFFFF
)						
{
	CNMChangeMyInfoFunc	
					func( uStatus, szWord, szNickName, CNMAvatarItemInfo::kEmotion_NULL, uMaskInfo );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

NMALLOWTYPE CNMCOClientObject::GetAllowChatType
( 
	void
)
{
	CNMGetConfigFunc	func( CNMGetConfigFunc::kFlag_AllowChatType );
	return				( CNMManager::GetInstance().CallNMFunc( &func )
						? func.uAllowChatType
						: kConfigAllowValue_Unknown );
}

NMALLOWTYPE CNMCOClientObject::GetAllowNoteType
( 
	void 
)
{
	CNMGetConfigFunc	func( CNMGetConfigFunc::kFlag_AllowNoteType );
	return				( CNMManager::GetInstance().CallNMFunc( &func )
						? func.uAllowNoteType
						: kConfigAllowValue_Unknown );
}

NMALLOWTYPE CNMCOClientObject::GetAllowGameInviteType
( 
	void
)
{
	CNMGetConfigFunc	func( CNMGetConfigFunc::kFlag_AllowGameInviteType );
	return				( CNMManager::GetInstance().CallNMFunc( &func )
						? func.uAllowGameInviteType
						: kConfigAllowValue_Unknown );
}

NMALLOWTYPE CNMCOClientObject::GetAllowRequestFriendType
( 
	void 
)
{
	CNMGetConfigFunc	func( CNMGetConfigFunc::kFlag_AllowRequestFriendType );
	return				( CNMManager::GetInstance().CallNMFunc( &func )
						? func.uAllowRequestFriendType
						: kConfigAllowValue_Unknown );
}

BOOL CNMCOClientObject::ChangeAllowChatType
( 
	NMALLOWTYPE			uType 
)
{
	CNMChangeConfigFunc	func( uType, kConfigAllowValue_Unknown, kConfigAllowValue_Unknown, kConfigAllowValue_Unknown, NULL );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeAllowNoteType
( 
	NMALLOWTYPE			uType 
)
{
	CNMChangeConfigFunc	func( kConfigAllowValue_Unknown, uType, kConfigAllowValue_Unknown, kConfigAllowValue_Unknown, NULL );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeAllowGameInviteType
( 
	NMALLOWTYPE			uType 
)
{
	CNMChangeConfigFunc	func( kConfigAllowValue_Unknown, kConfigAllowValue_Unknown, uType, kConfigAllowValue_Unknown, NULL );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeAllowRequestFriendType
( 
	NMALLOWTYPE			uType 
)
{
	CNMChangeConfigFunc	func( kConfigAllowValue_Unknown, kConfigAllowValue_Unknown, kConfigAllowValue_Unknown, uType, NULL );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::GetLocalConfig
(
	NMLOCALCONFIGTYPE	uConfigType
,	LPTSTR				szValueOut
)
{
	CNMGetLocalConfigFunc	func( uConfigType );
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		NMSAFE_STRCOPY( szValueOut, func.szValueOut, 1024 );
		return TRUE;
	}

	return FALSE;
}

BOOL CNMCOClientObject::ChangeLocalConfig
(
	NMLOCALCONFIGTYPE	uConfigType
,	LPCTSTR				szValue
)
{
	CNMChangeLocalConfigFunc
						func( uConfigType, szValue );
	return				( CNMManager::GetInstance().CallNMFunc( &func ) );
}

BOOL CNMCOClientObject::RequestNewFriend
(
	LPCTSTR		szRequestId				// LOGINID_SIZE
,	LPCTSTR		szMessage				// MSG_SIZE
,	BOOL		bAddToNxFriend			// = FALSE
,	NMGameCode	uGameCode				// = kGameCode_NULL
,	NMGameCode	uRequesteeGameCode		// = kGameCode_NULL
,	_UInt32_	uVirtualIdCode			// = 0
,	_UInt32_	uRequesteeVirtualIdCode	// = 0
)
{
	UNREFERENCED_PARAMETER(uVirtualIdCode);
	UNREFERENCED_PARAMETER(bAddToNxFriend);

	CNMRequestNewFriendFunc func;
	
	_tcsncpy( func.rnfData.szRequestID, szRequestId, LOGINID_SIZE );
	_tcsncpy( func.rnfData.szMessage, szMessage, MSG_SIZE );
	func.rnfData.keyToVirtual.uGameCode			= uGameCode;
	func.rnfData.uRequesteeGameCode				= uRequesteeGameCode;
	func.rnfData.keyToVirtual.uVirtualIDCode	= uRequesteeVirtualIdCode & 0xFF000000;
	func.rnfData.uInsertCategoryCode			= 0;
	func.rnfData.bAddToNxFriend					= FALSE;

	_UInt32_ uSubGameCode	= GET_GAMESUBCODE( uGameCode );

	if ( uSubGameCode == kSubGameCode_lunia || uSubGameCode == kSubGameCode_luniatest 
		|| uSubGameCode == kSubGameCode_zera || uSubGameCode == kSubGameCode_zeratest || uSubGameCode == kSubGameCode_zeratest_internal )
	{
		func.rnfData.keyFromVirtual.uVirtualIDCode = uVirtualIdCode;
	}

	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ConfirmNewFriend
(
	_UInt32_		uSerialNo
,	NMCONFIRMCODE	uConfirmCode
)
{
	CNMConfirmNewFriendFunc
					func( uSerialNo, uConfirmCode, 0 );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::AddCategory
(
	NMVirtualKey		keyOwner
,	LPCTSTR				szCategoryName			// CATEGORY_SIZE
,	NMCATEGORYPROPERTY	uProperty
,	NMALLOWTYPE			uAllowType
)
{
	CNMAddCategoryFunc	
					func( keyOwner, szCategoryName, uProperty, uAllowType );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::DeleteCategory
(
	UINT32				uCategoryCode
,	BOOL				bSystemCall
)
{
	CNMDeleteCategoryFunc
					func( uCategoryCode, bSystemCall );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeCategoryName
(
	UINT32				uCategoryCode
,	LPCTSTR				szCategoryName			// CATEGORY_SIZE
,	BOOL				bSystemCall
)
{
	CNMChangeCategoryNameFunc
					func( uCategoryCode, szCategoryName, bSystemCall );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeCategoryProperty
(
	UINT32				uCategoryCode
 ,	NMCATEGORYPROPERTY	uProperty
 ,	BOOL				bSystemCall
)
{
	CNMChangeCategoryPropertyFunc
					func( uCategoryCode, uProperty, bSystemCall );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeCategoryAllowType
(
	UINT32				uCategoryCode
 ,	NMALLOWTYPE			uAllowType
 ,	BOOL				bSystemCall
)
{
	CNMChangeCategoryAllowTypeFunc
					func( uCategoryCode, uAllowType, bSystemCall );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::GetFriendList
(
	NMCategoryUserList&		
					aCateFriend
,	NMFriendList&
					aUnrelated
,	BOOL			bAll
)
{
	NMVirtualKey	keyMine = { 0xFFFFFFFF, 0 };
	CNMGetFriendListFunc
					func( bAll ? keyVirtualAll : keyMine );
	
	if (  CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		aCateFriend		= func.aCateUserList;
		aUnrelated		= func.aUnrelated;
		return TRUE;
	}

	return FALSE;
}

BOOL CNMCOClientObject::GetFriendListEx
(
	NMCategoryUserList&		
					aCateFriend
,	NMFriendList&
					aUnrelated
,	NMVirtualKey	keyVirtual			// = keyVirtualAll
)
{
//	NMVirtualKey	keyMine = { 0xFFFFFFFF, 0 };
	CNMGetFriendListFunc
					func( keyVirtual );
	
	if (  CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		aCateFriend		= func.aCateUserList;
		aUnrelated		= func.aUnrelated;
		return TRUE;
	}

	return FALSE;
}

BOOL CNMCOClientObject::GetFriendInfo
(
	NMFriendKey		keyFriend
,	CNMFriendInfo&	fiFriendInfo
)
{
	CNMGetFriendInfoFunc
					func( keyFriend );
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		fiFriendInfo = func.fiFriendInfo;
		return TRUE;
	}
	return FALSE;
}

BOOL CNMCOClientObject::BlockFriend
(
	NMFriendKey		keyFriend
,	BOOL			bBlock
)
{
	CNMBlockFriendFunc
					func( keyFriend, bBlock );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::AddFriendToCategory
(
	NMFriendKey		keyFriend
,	UINT32			uCategoryCode
,	BOOL			bSystemCall
)
{
	CNMAddFriendToCategoryFunc
					func( keyFriend, uCategoryCode, bSystemCall );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::DeleteFriendFromCategory
(
	NMFriendKey		keyFriend
,	UINT32			uCategoryCode
,	BOOL			bSystemCall
)
{
	CNMDeleteFriendFromCategoryFunc
					func( keyFriend, uCategoryCode, bSystemCall );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::MoveFriendCategory
(
	NMFriendKey		keyFriend
,	UINT32			uFromCategoryCode
,	UINT32			uToCategoryCode
,	BOOL			bSystemCall
)
{
	CNMMoveFriendCategoryFunc
					func( keyFriend, uFromCategoryCode, uToCategoryCode, bSystemCall );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::ChangeFriendMemo
(
	_UInt64_		uIDCode
,	LPCTSTR			szMemo					// MEMO_SIZE
)
{
	CNMChangeFriendMemoFunc
					func( uIDCode, szMemo );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::GetRejectedUserList
( 
	NMRejectedUserList &	rejectedUserList
)
{
	CNMGetRejectedUserListFunc func;
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		rejectedUserList = func.rejectedUserList;
		return TRUE;
	}
	return FALSE;
}

BOOL CNMCOClientObject::AppendRejectedUser
(
	CNMRejectedUser & rejectedUser
)
{
	CNMAppendRejectedUserFunc func(rejectedUser);
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RemoveRejectedUser
(
	CNMRejectedUser & rejectedUser
)
{
	CNMRemoveRejectedUserFunc func(rejectedUser);
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::SendNoteMsg
(	
	CNMNoteInfo::NMNOTETYPE	uNoteType
,	UINT32 uSubType
,	BOOL			bSaveToPermBox
,	LPCTSTR			szTargetId				// TARGETID_SIZE
,	LPCTSTR			szMessage				// NOTEMSG_SIZE
,	LPCTSTR			szArgument				// NOTEARG_SIZE, = NULL
,	LPCTSTR			szCustomData			// NOTECUSTOM_SIZE, = NULL
,	NMGameCode		uGameCode				// = kGameCode_NULL
,	_UInt32_		uVirtualIdCode			// = 1
)
{
	if ( uGameCode != kGameCode_NULL )
	{
		NMVirtualKey	keyVirtual	= { uGameCode, uVirtualIdCode };
		CNMSendNoteMsgFunc	func( uNoteType, static_cast<CNMNoteInfo::NMNOTESUBTYPE>(uSubType), keyVirtual, szTargetId, szMessage, szArgument, szCustomData, bSaveToPermBox );
		return		CNMManager::GetInstance().CallNMFunc( &func );
	}

	CNMSendNoteMsgFunc	func( uNoteType, static_cast<CNMNoteInfo::NMNOTESUBTYPE>(uSubType), szTargetId, szMessage, szArgument, szCustomData, bSaveToPermBox );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::SendNoteExMsg
(	CNMNoteInfoEx::NMNOTEEXTYPE	
					uNoteExType
,	LPCTSTR			szMessage				// NOTEEXMSG_SIZE
)		
{
	CNMSendNoteExMsgFunc	
					func( uNoteExType, szMessage );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::SendReportMsg
(	
	CNMReportInfo::NMREPORTTYPE		
					uType
,	CNMReportInfo::NMREPORTSUBTYPE	
					uSubType
,	LPCTSTR			szTargetId				// LOGINID_SIZE
,	LPCTSTR			szReason				// 4000
,	LPCTSTR			szEvidence				// 20000
,	_UInt32_		uGameCode				// = kGameCode_NULL
,	_UInt32_		uAppliedGameCode		// = kGameCode_NULL
)
{
	CNMSendReportMsgFunc	
					func( uType, uSubType, szTargetId, szReason, szEvidence, uGameCode, uAppliedGameCode );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

NMNoteList CNMCOClientObject::GetNoteBox
(
	NMNOTEBOXTYPE	uBoxType
,	_UInt32_		uNoteSerial
)
{
	CNMGetNoteBoxFunc
					func( uBoxType, uNoteSerial );
	BOOL			bReturn		= CNMManager::GetInstance().CallNMFunc( &func );
	(void)bReturn;
	return			func.aNotes;
}

BOOL CNMCOClientObject::GetNoteBox
(
	NMNOTEBOXTYPE		uBoxType
,	UINT32				uNoteSerial
,	NMNoteInfoList &	noteInfoList
)
{
	CNMGetNoteBox2Func func( uBoxType, uNoteSerial );
	BOOL bResult = CNMManager::GetInstance().CallNMFunc( &func );
	if (bResult)
	{
		noteInfoList.assign(func.aNotes.begin(), func.aNotes.end());
	}
	return bResult;
}

BOOL CNMCOClientObject::SetNoteReadFlag
(
	_UInt32_		uNoteSerial
)
{
	CNMProcessNoteFunc
					func( CNMProcessNoteFunc::kType_SetReadFlag, uNoteSerial );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::SaveNoteToPermBox
(
	_UInt32_		uNoteSerial
)
{
	CNMProcessNoteFunc
					func( CNMProcessNoteFunc::kType_SaveToPerm, uNoteSerial );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::DeleteNote
(
	_UInt32_		uNoteSerial
)
{
	CNMProcessNoteFunc
					func( CNMProcessNoteFunc::kType_DeleteNote, uNoteSerial );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

CNMGuildListExInfo CNMCOClientObject::GetMyGuildList
(
	void
)
{
	CNMGetMyGuildListExFunc
					func;

	CNMManager::GetInstance().CallNMFunc( &func );
	return			func.gliGuildList;
}

/*
BOOL CNMCOClientObject::GetGuildList( CNMGuildListExInfo & guildList , CNMCharacter const & gameCharacter )
{
	CNMGetGuildListFunc func(guildList, gameCharacter);
	return CNMManager::GetInstance().CallNMFunc(&func);
}
*/

BOOL CNMCOClientObject::MonitorGuildOnlineInfoEx( NMGuildKey uGuildKey, BOOL bAttach )
{
	CNMMonitorGuildOnlineInfoExFunc
					func( uGuildKey, bAttach );
	return			CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::GetGuildOnlineInfoEx( NMGuildKey uGuildKey, CNMGuildOnlineInfoEx& goiInfo )
{
	NMCSLOCK( this->m_cs )
	{
		NMGOIList::iterator		iterBegin, iterEnd, iterCur;

		iterBegin		= this->m_aGOIs.begin();
		iterEnd			= this->m_aGOIs.end();

		for ( iterCur = iterBegin; iterCur != iterEnd; ++iterCur )
		{
			if ( (*iterCur).uGuildKey == uGuildKey )
			{
				goiInfo = (*iterCur);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CNMCOClientObject::SendGuildChatMessage( NMGuildKey uGuildKey, CNMChatMessage cmChatMsg )
{
	CNMSendGuildChatMessageFunc	func;

	func.uGuildKey	= uGuildKey;
	func.cmChatMsg	= cmChatMsg;

	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::GetGameList
(
	CNMGameListData*	pglGameList
)
{
	if ( pglGameList != NULL )
	{
		CNMGetGameListFunc	func;
		if ( CNMManager::GetInstance().CallNMFunc( &func ) )
		{
			*pglGameList = func.glGameList;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNMCOClientObject::GetUserInfo
(
	LPCTSTR				szTargetID						// LOGINID_SIZE
,	NMGameCode			uGameCode						// = kGameCode_NULL
)
{
	CNMGetUserInfoFunc	func( szTargetID, uGameCode );
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::SendCustomNotify
(
	UINT32				uType
,	LPCTSTR				szArgument			// MSG_SIZE (256)
,	UINT32				uPolicyFlag/* = CNMCustomNotifyData::kCustomNotifyPolicy_Default*/
)
{
	CNMCustomNotifyFunc	func( uType, szArgument, uPolicyFlag );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestChatSession
( 
	NMFriendCode		fcFriendCode
)
{
	CNMRequestChatSessionFunc
						func( fcFriendCode, 0 );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestMultiChatSession
( 
	NMFriendCodeList&	aFriendCodeList
,	NMVirtualKey		keyVirtuality
)
{
	CNMRequestMultiChatSessionFunc
						func( keyVirtuality, aFriendCodeList, 0 );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestFileUploadSession
( 
	NMFriendCode		fcFriendCode
,	NMFilePathList&		aFilePath
)
{
	CNMRequestFileUploadSessionFunc
						func( fcFriendCode, 0, aFilePath );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestChatSession
( 
	NMFriendKey			keyFriend
)
{
	CNMRequestChatSessionExFunc
						func( keyFriend, 0 );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestMultiChatSession
( 
	NMFriendKeyList&	aFriendKey
,	NMVirtualKey		keyVirtuality
)
{
	CNMRequestMultiChatSessionExFunc
						func( keyVirtuality, aFriendKey, 0 );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestFileUploadSession
( 
	NMFriendKey			keyFriend
,	NMFilePathList&		aFilePath
)
{
	CNMRequestFileUploadSessionExFunc
						func( keyFriend, 0, aFilePath );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestChatSessionToGuildMember
(
	LPCTSTR				szRequestID
,	NMVirtualKey		keyVirtuality
)
{
	CNMRequestSessionToOtherFunc
						func( kSessionType_Chat, CNMSessionInfo::kInviteMode_GuildMember, keyVirtuality, szRequestID, kGameCode_NULL );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::RequestFileUploadSessionToGuildMember
(
	LPCTSTR				szRequestID
,	NMFilePathList&		aFilePath
,	NMVirtualKey		keyVirtuality
)
{
	CNMRequestSessionToOtherFunc
						func( kSessionType_FileUpload, CNMSessionInfo::kInviteMode_GuildMember, keyVirtuality, szRequestID, kGameCode_NULL, aFilePath );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::CreateChatRoomEx
(
	_UInt32_			uRoomType
,	NMGuildKey			uGuildKey
,	LPCTSTR				szGuildTitle			// GUILDNAME_SIZE
,	LPCTSTR				szRoomTitle				// ROOMTITLE_SIZE
,	_UInt32_			uLimitLevel
,	_UInt32_			uMaxSize
,	_UInt32_			uFlag
,	LPCTSTR				szPassword				// ROOMPWD_SIZE
,	LPCTSTR				szServerAddr			// 32
,	_UInt16_			uServerPort
,	UINT32				uVirtualIdCode			// = 1
)
{
	CNMCreateChatRoomExFunc	
						func( uRoomType, uGuildKey, szGuildTitle, szRoomTitle, uLimitLevel,uMaxSize, uFlag, szPassword, szServerAddr, uServerPort, uVirtualIdCode );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::JoinToChatRoomEx
(	
	NMGuildKey			uGuildKey
,	_UInt32_			uRoomSerialNo
,	LPCTSTR				szPassword				// ROOMPWD_SIZE
,	LPCTSTR				szServerAddr			// 32
,	_UInt16_			uServerPort 
,	_UInt32_			uFlag					// = 0
,	UINT32				uVirtualIdCode			// = 1
)
{
	CNMJoinToChatRoomExFunc	
						func( uGuildKey, uRoomSerialNo, szPassword, szServerAddr, uServerPort, uFlag, uVirtualIdCode );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

UINT32 CNMCOClientObject::GetUnreadNoteCount
(
	void
)
{
	CNMGetCountFunc func( CNMGetCountFunc::kType_UnreadNote );
	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		return func.uCount;
	}
	return 0;
}

BOOL CNMCOClientObject::RequestNews()
{
	CNMRequestNewsFunc	func;
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::CheckNews
( 
	NMGameCode		uEventGameCode,
	NMVirtualKey	keyVirtual/* = keyVirtualAll */
)
{
	CNMCheckNewsFunc	func( uEventGameCode, keyVirtual );
	return				CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMCOClientObject::JoinToDefaultGuildChatRoom
(
	NMVirtualKey keyVirtual
)
{
	CNMJoinToDefaultGuildChatRoomFunc func( keyVirtual );
	return CNMManager::GetInstance().CallNMFunc( &func );
}
