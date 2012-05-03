#ifdef _XDEF_NEXONLOGIN

#ifndef __NMCOHELPERS_H__AABA93EE_7B94_4f70_87A3_A58A648C3378___
#define __NMCOHELPERS_H__AABA93EE_7B94_4f70_87A3_A58A648C3378___

namespace NMCOHelpers
{
	enum
	{
		kLaunchType_Launch = 1,
		kLaunchType_Invited,
		kLaunchType_Invite,
	};

	/* Get Path Information */
	LPCTSTR		GetCommonPath
	( 
				OUT LPTSTR				szPath						// MAX_PATH
	);

	LPCTSTR		GetMessengerPath
	( 
				OUT	LPTSTR			szPath							// MAX_PATH
	);

	LPCTSTR		GetBannerPath
	(
				OUT	LPTSTR			szBannerDir						// MAX_PATH
	);

	LPCTSTR		GetBannerFileName
	(
				IN	LPCTSTR			szBannerUrl						// MAX_PATH
	,			OUT	LPTSTR			szFileNameOut					// MAX_PATH
	);

	LPCTSTR		GetGameButtonPath
	(
				OUT	LPTSTR			szGameButtonDir					// MAX_PATH
	);

	LPCTSTR		GetGameButtonFileName
	(
				IN	LPCTSTR			szGameButtonUrl					// MAX_PATH
	,			OUT	LPTSTR			szFileNameOut					// MAX_PATH
	);

	void		MakeExecDir
	(
				IN	LPCTSTR			szExecPath						// MAX_PATH
	);

	/* Patch & Launch */
	BOOL		ExecutePatcher
	(	
				IN	LPCTSTR			szUrl							// 256
    ,			IN	LPCTSTR			szPatchDir						// 256
	,			IN	LPCTSTR			szExecCmdLine					// 2000
	,			IN	BOOL			bWaitParentExit		= TRUE
	,			IN	BOOL			bWaitPatchEnd		= FALSE 
	);

	BOOL		ExecuteLauncher
	(	
				IN	NMGameCode		uGameCode
	,			IN	_UInt32_		uLaunchType
	,			IN	LPCTSTR			szArgument			= NULL		// 2000
	,			IN	BOOL			bWaitParent			= FALSE 
	);

	BOOL		ExecuteNGMPatcher
	(	
				IN	_UInt32_		uGameCode
	,			IN	LPCTSTR			szUrl							// 256
    ,			IN	LPCTSTR			szPatchDir						// 256
	,			IN	LPCTSTR			szExecCmdLine					// 1024
	,			IN	BOOL			bWaitParentExit		= TRUE
	,			IN	BOOL			bSmallDialog		= FALSE
	);

	BOOL		ExecuteNGMLauncher
	(	
				IN	NMGameCode		uGameCode
	,			IN	_UInt32_		uLaunchType
	,			IN	LPCTSTR			szArgument			= NULL		// 1024
	,			IN	BOOL			bWaitParent			= FALSE
	,			IN	LPCTSTR			szPassport			= NULL		// 1024
	);
	
	BOOL		IsNGMInstalled( void );

	/* URL Helper */
	BOOL		GetUrlFromPage
	(	
				IN	NMURLPAGE		uPage
	,			OUT	LPTSTR			szUrlOut								// 1024
	,			IN	_UInt32_		uParam			= 0						// for kURL_MemberPopup_OID, kURL_MoneyRefillForWebGame
	,			IN	LPCTSTR			szParam			= NULL					// for kURL_MemberPopup_LoginID, kURL_WriteMail_Target
	,			IN	BOOL			bLoginDirect	= TRUE
	,			IN	BOOL			bIsPopup		= FALSE
	,			IN	BOOL			bNoActiveX		= FALSE
	,			IN	UINT32			uMaskGameCode	= kGameCode_nexoncom
	);

	BOOL		GetUrlFromGameCode
	(	
				IN	NMGameCode		uGameCode
	,			OUT	LPTSTR			szUrlOut								// 1024
	,			IN	BOOL			bLoginDirect	= TRUE
	,			IN	BOOL			bIsPopup		= FALSE
	,			IN	BOOL			bNoActiveX		= FALSE
	,			IN	UINT32			uMaskGameCode	= kGameCode_nexoncom
	);

	BOOL		GetURLFromGuildKey
	(	
				IN	NMGuildKey		uGuildKey
	,			OUT	LPTSTR			szUrlOut								// 1024
	,			IN	BOOL			bLoginDirect	= TRUE
	,			IN	BOOL			bIsPopup		= FALSE 
	,			IN	BOOL			bNoActiveX		= FALSE
	,			IN	UINT32			uMaskGameCode	= kGameCode_nexoncom
	);

	BOOL		GetLoginDirectUrl
	(	
				IN	LPCTSTR			szUrl									// 256
	,			IN	LPTSTR			szUrlOut								// 1024
	,			IN	BOOL			bIsPopup		= FALSE
	,			IN	BOOL			bNoActiveX		= FALSE
	,			IN	UINT32			uMaskGameCode	= kGameCode_nexoncom
	);

	/* Global Session Value */
	BOOL		GetSessionValue
	(	
				IN	LPCTSTR			szName							// 32
	,			OUT	LPTSTR			szValue							// 256
	);

	BOOL		SetSessionValue
	(			
				IN	LPCTSTR			szName							// 32
	,			IN	LPCTSTR			szValue				= NULL		// 256, NULL : delete
	,			IN	BOOL			bAttachToProcess	= FALSE
	);

	BOOL		BringForwardStandAloneMsg
	(
				IN	LPCTSTR			szDomain						// DOMAIN_SIZE
	);

	BOOL		DownloadFile
	(
				IN	LPCTSTR			szUrl							// 256
	,			IN	LPCTSTR			szFileName						// 256
	);

	BOOL		UploadFile
	(
				IN	LPCTSTR			szUrl							// 256
	,			IN	LPCTSTR			szFileName						// 256
	,			IN	LPCTSTR			szPostField						// 256
	,			OUT	LPTSTR			szResponse						// 1024
	);

	BOOL		GetSupportGameList
	(
				OUT NMGameCodeList&			aGameCodeList
	);

	BOOL		GetGameServerList
	(
				IN	NMGameCode				uGameCode
	,			OUT	NMGameServerCodeList&	aGameServerCodeList
	);

	BOOL		GetGameFullName
	(
				IN	NMGameCode				uGameCode
	,			OUT	LPTSTR					szGameFullName			// 128
	);

	BOOL		GetGameShortName
	(
				IN	NMGameCode				uGameCode
	,			OUT	LPTSTR					szGameShortName			// 128
	);

	BOOL		GetGameFriendTitle
	(
				IN	NMGameCode				uGameCode
	,			OUT	LPTSTR					szGameFriendTitle		// 128
	);

	BOOL		GetGameServerName
	(
				IN	NMGameCode				uGameCode
	,			IN	NMGameServerCode		uGameServerCode
	,			OUT	LPTSTR					szGameServerName		// 128
	);

	BOOL		KartComeback
	(
				IN	_UInt32_				uOID
	,			OUT	BOOL&					bComeback
	);

	BOOL		KartComebackList
	(
				IN	NMKartDormantList&		aList
	);

	BOOL		DownloadGuildMarkFile
	(
				IN	NMGameCode				uGameCode
	,			IN	NMGuildKey				uGuildKey
	,			IN	LPCTSTR					szDownloadDir
	);
}

#endif	//	#ifndef __NMCOHELPERS_H__AABA93EE_7B94_4f70_87A3_A58A648C3378___

#endif