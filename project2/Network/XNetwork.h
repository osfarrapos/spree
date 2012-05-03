// XNetwork.h: interface for the _XNetwork class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XNETWORK_H_
#define _XNETWORK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _XCOMMENT / ## / 

#ifdef _XDWDEBUG
#define VIEWPACKETLOG if(g_ViewPacketLog)
#else
#define VIEWPACKETLOG _XCOMMENT
#endif


#include <windows.h>
#include <winsock2.h>

//#include "XUser.h"
#include "XKernel.h"
#include "XNetworkDefine.h"
#include "Game_packets.h"


#define _XDEF_LOGINPROCESS_READY			0
#define _XDEF_LOGINPROCESS_CONFIRM			1
#define _XDEF_LOGINPROCESS_LOGINSUCCESS		2
#define _XDEF_LOGINPROCESS_LOGINFAILED		3


//=================================================================================
// Game time
//=================================================================================
// 초로 계산했을 때.
#define _SECOND_YEAR_		32140800			// 1년 
#define _SECOND_MONTH_		2678400				// 31 일 
#define _SECOND_DAY_		86400				// 1일
#define _SECOND_HOUR_		3600				// 1시간
#define _SECOND_MINUTE_		60					// 1분

#define _GAME_TIME_YEAR_		5184000
#define _GAME_TIME_MONTH_		432000
#define _GAME_TIME_DAY_			14400
//=================================================================================
//=================================================================================
struct sDate
{
	int Year;
	int Month;
	int Day;
	int Second;
};

void GetGameTime( int time, sDate * Date );

#ifndef _XCOMMENT
#define _XCOMMENT / ## / 
#endif

#ifdef _XDWDEBUG
void	_XDWINBATTLELOG( TCHAR *msg, ... );
void	_XDWINBATTLERESULTLOG( TCHAR *msg, ... );
#else
#define _XDWINBATTLELOG			_XCOMMENT
#define _XDWINBATTLERESULTLOG	_XCOMMENT
#endif



/*
	CND_LoginChecker
		passport 에는 아이피, 패스워드 아이디가 들어간다
		이때, 들어가는 위치는 조금씩 다른데, 무작위로 1 ~ 8 씩 건너뛰며 있다.
*/
class CND_LoginChecker
{
public:
	CND_LoginChecker( u_int lmbcMode ) 
		: m_password(NULL), m_id(NULL) 
	{
		switch( lmbcMode )
		{
		case en_lmbc_nexon:
			m_password = new char[en_lpl_nexon+1] ;
			m_id = new char[en_lil_nexon+1] ;
			break;
		case en_lmbc_metel:
			m_password = new char[en_lpl_metel+1] ;
			m_id = new char[en_lil_metel+1] ;
			break;
		case en_lmbc_vina:
			m_password = new char[en_lpl_vina+1] ;
			m_id = new char[en_lil_vina+1] ;
			break;
		case en_lmbc_acclaim:
			m_password = new char[en_lpl_acclaim+1] ;
			m_id = new char[en_lil_acclaim+1] ;
			break;
		case en_lmbc_pwc:
			m_password = new char[en_lpl_pwc+1] ;
			m_id = new char[en_lil_pwc+1] ;
			break;
		case en_lmbc_rocworks:
			m_password = new char[en_lpl_rocworks+1] ;
			m_id = new char[en_lil_rocworks+1] ;
			break;			
		case en_lmbc_russia:
			m_password = new char[en_lpl_playten+1] ;
			m_id = new char[en_lil_playten+1] ;
			break;			
		}
	}
	
	~CND_LoginChecker()
	{
		SAFE_DELETE_ARRAY( m_id );
		SAFE_DELETE_ARRAY( m_password );
	}

	BOOL lc_MakePacket( u_int lmbcMode IN, char * passport OUT, int passlength IN, const char * pID IN, const char * pPW IN, const char * pIP IN );
	BOOL lc_CheckPakcet( u_int mode IN, const char * passport );

protected:
	char		m_ip[16];
	char *		m_password;
	char *		m_id;
};


//////////////////////////////////////////////////////////////////////
// Class interface...
class _XNetwork  
{
public:
	TCHAR			m_LOGINSERVER_ADDRESS[32];	
	DWORD			m_LOGINSERVER_PORT;
	SOCKADDR_IN		m_UDPAddress;
	int				m_LoginProcessMode;

	DWORD			m_SERVER_ADDRESS;	
	DWORD			m_SERVER_PORT;

	DWORD			m_NextSERVER_ADDRESS;	
	DWORD			m_NextSERVER_PORT;

	SOCKET			m_hLoginUDPSocket;
	BOOL			m_bLoginServerConnected;

	SOCKET			m_hClientSocket;
	BOOL			m_bConnected;

	BOOL			m_bLoginConnected;
	
	TCHAR			m_HostBuffer[MAXGETHOSTSTRUCT];
	HANDLE			m_hGetHostTask;
	
	int				m_LastConfirmID;

	ULONG			m_MESSENGERSERVER_ADDRESS;	
	DWORD			m_MESSENGERSERVER_PORT;
	
	SOCKET			m_hMessengerSocket;
	BOOL			m_bMessengerServerConnected;
	BOOL			m_bMessengerServerLoginSuccess;
	int				m_ConnectTryCount;
	
	int				m_Messenger_nPI;
	int				m_Messenger_nRI;
	BYTE			m_Messenger_szPacket[_XDEF_PACKET_MAXBUFFER];
	short			m_Messenger_sPacketLen;
	int				m_Messenger_mode;
	
	BOOL			m_bMessengerInstanceMessagePosition[50];
	
	DWORD			m_MovePacket_lastSendedTime;	

	_sRunningQuestPacketData m_RunningQuestData[_MAX_CURQUEST_NUM_];
	
	BOOL			m_bDeleteCharacter;

public:
	_XNetwork();
	virtual ~_XNetwork();
	
	void	InitializePacketBuffer( void );

#ifdef _XDEF_NEXONLOGIN // Nexon login
	
	BOOL	InitNMCO( void );	// Nexon messager server login
	BOOL	ReleaseNMCO( void );

#endif

	// Login UDP Socket handler...
	BOOL	InitializeUDPSocket( void );
	BOOL	ReleaseUDPSocket( void );

	void    UDPReadPacket( void );
	void    ParsingLoginPacket(LPBYTE pPacket, UINT packetsize);
	BOOL	UDPSendPacket( int type, int param1 = 0 , int param2 = 0 );
	BOOL	_XUDPSendPacket_Encapsulate( char* pPacket );

	// Game TCP/IP Socket handler...
	BOOL	InitializeNetwork( void );
	BOOL	ReleaseNetwork( void );

	BOOL	ConnectServer( void );
	BOOL 	DisconnectServer( BOOL bNextConnect = FALSE );

	void    ReadSocket( void );
	
	BOOL    SendPacket( int type, int param1 = 0 , int param2 = 0, int param3 = 0, int param4 = 0 );
	BOOL	ParsingPacket(LPBYTE pPacket);
	
	BOOL	_XSendPacket_Encapsulate( char* pPacket );
	
	void	Login( void );
	void	Logout( BOOL bNextConnect = FALSE );
	
	void	SendChatMessage( LPSTR chatmessage, int messagemode = _XDEF_CHATMESSAGEMODE_NORMAL );
	void	SendMessengerChatMessage( LPSTR chatmessage, int windowid, int messagemode = _XDEF_CHATMESSAGEMODE_NORMAL );
	void	InsertChatString( LPSTR message, int messagemode = _XDEF_CHATMESSAGEMODE_NORMAL, BOOL bCenterMessage = FALSE, D3DCOLOR centermessageinfo = D3DCOLOR_ARGB( 255, 250, 230, 170 ) );
	
	void	ProcessUDPNetworkError( void );
	void	ProcessNetworkError( void );

	// Messenger TCP/IP Socket handler... 
	void	InitializeMessengerPacketBuffer( void );
	
	BOOL	InitializeMessengerSocket( void );
	BOOL	ReleaseMessengerSocket( void );
	
	BOOL	SendMessengerPacket( int type, int param1 = 0 , int param2 = 0 );
	void	ReadMessengerPacket( void );
	void	ParsingMessengerPacket(LPBYTE pPacket);
	
	void	ProcessMessengerNetworkError( void );
	
	// Send packet 처리 함수
	BOOL	Send_MSGMartialReady(int type, int param1 = 0, int param2 = 0, int param3 = 0);
	BOOL	Send_MSGMartialFire(int type, int param1 = 0, int param2 = 0, int param3 = 0);
	BOOL	Send_MSGMartialEnd(int type, int param1 = 0, int param2 = 0, int param3 = 0);
	
	BOOL	Send_MSGMiniGame(int type, int param1 = 0, int param2 = 0, int param3 = 0);
	BOOL	Send_MSGEffectCtrl(int type, int param1 = 0, int param2 = 0, int param3 = 0);

	BOOL	Send_MSGParty(int type, int param1 = 0, int param2 = 0, int param3 = 0);
	BOOL	Send_MSGQuest(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	Send_MSGBoss(int type, int param1 = 0, int param2 = 0, int param3 = 0);
	BOOL	Send_MSGNickName(int type, int param1 = 0, int param2 = 0, int param3 = 0);
	
	BOOL	Send_MMSGMessenger(int type, int param1 = 0, int param2 = 0); // Messenger packet Send 함수
	BOOL	Send_MMSGEmail(int type, int param1 = 0, int param2 = 0); // 전서구 packet Send 함수
	
	BOOL	Send_MSGPersonalStore(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	Send_MSGCollection(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	
	BOOL	Send_MatchList(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	Send_MatchReady(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	
	BOOL	Send_FreeMatch(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	
#ifdef _XTS_ALIANCE_20061018
	BOOL	Send_Alience(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);// 영웅단 동맹 관련 Send 함수
#endif

	BOOL	Send_MnDSystem(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0); //Author : 양희왕 //breif : 사제 패킷 보내기

	// Messenger packet 처리 함수 
	void	MMSGMessengerMemberInfo(MMSG_FRIEND_LIST_RES* messengermemberinfo);
	void	MMSGMessengerChattingMessage(MMSG_CHAT* chattingmessage, BOOL GMchat = FALSE);
	void	MMSGMessengerRequestFriend(MMSG_MAKE_FRIEND_REQUEST*joinfriend);
	void	MMSGMessengerResponseMakeFriend(MMSG_MAKE_FRIEND_RES* responsemakefriend);
	void	MMSGMessengerResultCutFriend(MMSG_CUT_FRIEND_REPORT* resultcutfriend);
	void	MMSGMessengerResponseMakeGroup(MMSG_MAKE_GROUP_RES* responsemakegroup);
	void	MMSGMessengerResponseDeleteGroup(MMSG_CUT_GROUP_REQ* responsedeletegroup);
	void	MMSGMessengerResponseMoveGroup(MMSG_JOIN_GROUP_RES* responsemovegroup);
	void	MMSGMessengerResponseChangeGroupName(MMSG_RENAME_GROUP_RES* responsechagegroupname);
	void	MMSGMessengerUpdateStatus(MMSG_FRIEND_STATE_REPORT* userstateupdate);
	void	MMSGWhisperChatMessage( MMSG_CHAT* chattingmessage );
	void	MMSGWhisperChatFailMessage( MMSG_CHAT_RESULT* whisperchatmsg );
	
	// 전서구 packet 처리 함수 
	void	MMSGEmailListResponse(MMSG_MAIL_LIST_RES* responsemaillist);
	void	MMSGEmailTextResponse(MMSG_MAIL_TEXT_RES* responsemailtext);
	void	MMSGEmailTextSuccess(MMSG_MAIL_SEND_RES* successsendmail);
	void	MMSGEmailTextReceive(MMSG_MAIL_RECV* receivemail);
	void	MMSGEmailFail(MMSG_MAIL_FAIL* failemail);

	// packet 처리 함수들
	void	MSGSVEnterResult(MSG_SV_ENTER_RESULT* serverenteringresult);
	void	MSGCharacList(MSG_CHARAC_LIST* characterlist);
	void	MSGChangeServerGroup(MSG_SHIFT_STATE* changeservergroup);

	void	MSGCharacResult(MSG_CHARAC_RESULT* characterstatusresult);
	void	MSGCharacInfo(MSG_CHARAC_INFO* characterinfo);
	
	void	MSGServerUnification(MSG_UNI_GROUP_LOBBY* serveruniinfo);

	void	MSGSetLevel(MSG_SET_LEVEL* setlevel);
	void	MSGSetPower(MSG_SET_POWER* setpower);
	void	MSGLevelUpReq(MSG_LEVEL_UP_RESULT* levelupresult);
	void	MSGPointUse(MSG_POINT_USE_RESULT* pointuseresult);
	void	MSGCharacSetState(MSG_CHARAC_SET_STATE* characsetstate);
	void	MSGStateChangeResult(MSG_STATE_CHANGE_RESULT* statechangeresult);

	void	MSGStateChangeToPK(MSG_STATE_CHANGE_TO_PK* statechangeresult);
	void	MSGStateChangeNotice(MSG_STATE_CHANGE_NOTICE* statechangenotice);
	void	MSGStateChangeToPE(MSG_STATE_CHANGE_TO_PE* statechangetope);
	void	MSGStateChangeSafeRegionNotice(MSG_STATE_CHANGE_SAFEREGION_NOTICE* saferegionnotice);
	void	MSGStateChangeAbsent(MSG_STATE_CHANGE_ABSENT* statechangeabsent);
	void	MSGStateChangeSelectClothes(MSG_STATE_CHANGE_CLOTHVIEW_SELECT_DECK* statechangeclothes);

	void	MSGStateEnterTutorial(MSG_STATE_CHANGE_ENTER_TURIAL* entertutorial);
	void	MSGStateLeaveTutorial(MSG_STATE_CHANGE_LEAVE_TUTORIAL* leavetutorial);

	void	MSGOtherCharacLevelUp(MSG_OTHER_CHARAC_LEVEL_UP* otherlevelup);

	void	MSGSkillInfo(MSG_SKILL_INFO* skillinfolist);

	void	MSGStorageInfo(MSG_STR_ITEM_INFO* storagelist);
	void	MSGStrItemSaveResult(MSG_STR_ITEM_SAVE* stritemsave);
	void	MSGStrItemEjectResult(MSG_STR_ITEM_EJECT* stritemsave);
	void	MSGStrItemEjectTerminateResult(MSG_STR_ITEM_EJECTEX* stritemeject);// 2004.06.02->oneway48 insert

	// 2004.05.20->oneway48 insert : 창고 사용 추가 관련
	void	MSGAddStrSlotResult(MSG_STR_BUY_SLOT_RESULT* addslotresult);
	// insert end

	void	MSGUseItem(MSG_USE_ITEM_RESULT* useitemresult); // 2004.06.10->oneway48 insert : 회복류 아이템 사용
	void	MSGElixirUseItem(MSG_ELIXIR_USE_RESULT* useitemresult);	
	void	MSGElixirIntox(MSG_ELIXIR_INTOX_TIME* intoxresult);
	void	MSGRepairItemResult(MSG_NPC_ITEM_REPAIR_RESULT* repairitemresult); // 아이템 수리

	BOOL	CheckMSGInvenInfo(unsigned char idx, char type, char second);
	int		CheckEmptyFameItem(void);
	BOOL	CheckFameItem(char cType, short sID);
	void	MSGInvenInfo(MSG_INVEN_INFO* inventorylist);
	void	InvenInfoSetting(MSG_INVEN_INFO* inventorylist); //Author : 양희왕 //breif : 캐시행낭 추가
	void	ElixirInfoSetting(MSG_INVEN_INFO* inventorylist); //Author : 양희왕 //breif : 캐시행낭 추가
	void	MSGInvenMove(MSG_INVEN_MOVE* invenmove);
	void	MSGInvenDrop(MSG_INVEN_DROP* inventoryitemdrop);
	// 2005.02.23->hotblood insert : 템프 인벤 -----------------------------------------------------------=	
	void	MSGTempInven(MSG_TEMP_INVEN* tempinven);
	void	MSGTempInvenCtrlResult(MSG_TEMP_INVEN_CTRL_RESULT* tempinvenctrlresult);
	
	void	MSGTempInvenMove(MSG_TEMP_INVEN_MOVE* invenmove);
	void	MSGTempInvenInfo(MSG_TEMP_INVEN_INFO* inveninfo);
	// ---------------------------------------------------------------------------------------------------=

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 breif : 소켓제거
	void	MSGItemEquipSocketResult( MSG_SOCKET_GAMBLE* pSocketorGambleResult );
#else
	void	MSGItemEquipSocketResult( MSG_ITEM_EQUIP_SOCKET_RESULT* socketitemresult );// 2004.11.9->hotblood insert
#endif

	void	MSGZoneRestart(MSG_ZONE_RESTART* zonerestart);
	void	MSGLogout(MSG_LOGOUT* logout);
	void	MSGSVToGServer(MSG_SV_TOGSERVER movetoserver);

	void	MSGZonePositioning(MSG_ZONE_POSITIONING* zonepositioning);
	void	MSGZoneMove(MSG_ZONE_MOVE* charactermove);
	void	MSGZoneCharacInfo(MSG_ZONE_CHARAC_INFO* characterinfo);

	// Monster
	void	MSGMonsterMove(MSG_MONSTER_MOVE* monstermove);
	void	MSGMonsterWarp(MSG_MONSTER_WARP* monsterwarp);
	void	MSGMonsterVital(MSG_MONSTER_VITAL* monstervital);
	void	MSGMonsterTransform(MSG_MONSTER_TRANSFORM* monstertransform);

	void	MSGMonsterExist(MSG_MONSTER_EXIST* monsterexist);
	void	MSGResSpellEffect(MSG_RES_SPELLEFFECT* spelleffect);
	void	MSGSpellListChanged(MSG_SPELLLIST_CHANGED* spellchanged);
	void	MSGConditionChanged(MSG_CONDITION_CHANGED* condchanged);
	void	MSGMonsterChaseNotice(MSG_MONSTER_CHASE_NOTICE* monsterchase);

	void	MSGBattleAtkResFromMob(MSG_B_ATKRES_FROM_MOB* monsterattack);
	void	MSGBattleAtkResFromMobWithPos(MSG_B_ATKRES_FROM_MOB_WITH_POS* monsterattackwithpos);

	void	MSGRegionDamage(MSG_REGION_DAMAGE* regiondamage);

	void	MSGSelfChannelCtrl(MSG_SELF_CHANNEL_CTRL* selfchannelctrl);
	void	MSGSelfChannelInfo(MSG_SELF_CHANNEL_INFO* selfchannelinfo);
	
	// hotblood->2004.07.27
/*	void	MSGMiniGameCtrl(MSG_MINIGAME_CTRL* minigamectrl);
	void	MSGMiniGameResult(MSG_MINIGAME_RESULT* minigameresult);
*/
	void	MSGMiniGameCtrl(MSG_MINIGAME_CTRL*);
	void	MSGMiniGameMissionStage(MSG_MINIGAME_MISSION_STAGE*);
	void	MSGMiniGameFail(MSG_MINIGAME_FAIL*);
	void	MSGMiniGameResult(MSG_MINIGAME_RESULT*);
	
	void	MSGMartialChargeReadyResult(MSG_CHARGE_READY_RES* chargeready);
	
	void	MSGMartialFinishReadyResult(MSG_FINISH_READY_RES* finishreadyresult);
	void	MSGMartialCastingReadyResultPoint(MSG_CASTING_READY_RES_TO_POINT* castingreadyresult);
	void	MSGMartialCastingReadyResultPC(MSG_CASTING_READY_RES_TO_PC* castingreadyresult);

	void	MSGMartialChannelReadyResultSelf(MSG_CHANNEL_READY_RES_SELF* channelreadyres);
	void	MSGMartialChannelReadyResultPoint(MSG_CHANNEL_READY_RES_POINT* channelreadyresult);
	void	MSGMartialChannelReadyResultPC(MSG_CHANNEL_READY_RES_PC* channelreadyresult);
	
	void	MSGMartialRoundingFireResult(MSG_ROUNDING_FIRE_RES* roundingfireresult);
	void	MSGMartialRoundingFireResToPoint(MSG_ROUNDING_FIRE_RES_TO_POINT* roundingfireresult);

	void	MSGMartialChargeEnd(MSG_CHARGE_END* chargeend);
	void	MSGMartialChannelEnd(MSG_CHANNEL_END* channelend);
	void	MSGMartialFinishEnd(MSG_FINISH_END* finishend);
	void	MSGMartialCastingEnd(MSG_CASTING_END* castingend);
	void	MSGMartialRoundingEnd(MARTIAL_END* martialend);
	void	MSGMartialSmashingEnd(MARTIAL_END* martialend);
	void	MSGMartialEnd(MARTIAL_END* martialend);
	
	void	MSGMartialActiveResultMob(MSG_ACTIVE_RESULT_MOB* activeresult);
	void	MSGMartialActiveResultPVP(MSG_ACTIVE_RESULT_PVP* activeresultpvp);

	void	MSGMartialChargeResultMob(MSG_CHARGE_RESULT_MOB* chargeresult);
	void	MSGMartialChargeResultPVP(MSG_CHARGE_RESULT_PVP* chargeresult);

	void	MSGMartialCastingResultToPoint(MSG_CASTING_RESULT_TO_POINT* castingresultpoint);
	void	MSGMartialCastingResultToMob(MSG_CASTING_RESULT_TO_MOB* castingresult);
	void	MSGMartialCastingResultToPVP(MSG_CASTING_RESULT_TO_PVP* castingresult);

	void	MSGMartialChannelingResultPoint(MSG_CHANNEL_RESULT_POINT* channelresult);
	void	MSGMartialChannelingResultPC(MSG_CHANNEL_RESULT_PC* channelresult);
	void	MSGMartialChannelingResultNPC(MSG_CHANNEL_RESULT_NPC* channelresult);

	void	MSGMartialFinishResultPC(MSG_FINISH_RESULT_TO_PC* finishresult);
	void	MSGMartialFinishResultMob(MSG_FINISH_RESULT_TO_MOB* finishresult);

	void	MSGMartialRoundingResultPC(MSG_ROUNDING_RESULT_TO_PC* roundingresult);
	void	MSGMartialRoundingResultMob(MSG_ROUNDING_RESULT_TO_MOB* roundingresult);
	void	MSGMartialRoundingResultPoint(MSG_ROUNDING_RESULT_TO_POINT* roundingresult);

	void	MSGMartialSmashingResultMob(MSG_SMASHING_RESULT_MOB* smashingresult);
	void	MSGMartialSmashingResultPVP(MSG_SMASHING_RESULT_PVP* smashingresult);
	
	void	MSGMartialActionResult(MSG_ACTION_RESULT* actionresult);

	void	MSGCharacCtrl(MSG_CHARAC_CTRL* characctrl);	
	void	MSGCharacRebirthTarget(MSG_CHARAC_REBIRTH_TARGET* characrebirthtarget);
	void	MSGSaveRespawnPosResult(MSG_SAVE_RESPAWN_POS_RESULT* respawnposresult);
	void	MSGSkillActionReson(MSG_CANCEL_SKILL_CANNOT* skillaction);

	void	MSGZoneChatNormal(MSG_ZONE_CHAT_NORMAL* chatnormal);	
	void	ProcessChatSpecialCode( MSG_ZONE_CHAT_NORMAL* chatnormal );	// process chat special code

	void	MSGItemEquipChange(MSG_EQUIP_CHANGE* itemequip);
	void	MSGItemDrop(MSG_ITEM_DROP* itemdrop);
	void	MSGMoneyDrop(MSG_MONEY_DROP* moneydrop);
	void	MSGItemGrabSuccess(MSG_ITEM_GRAB_SUCCESS* successgrabitem);
	void	MSGMoneyGrabSuccess(MSG_MONEY_GRAB_SUCCESS* successgrabmoney);
	void	MSGWorldItemInfo(MSG_WORLD_ITEM* worlditeminfo);
	void	MSGItemRemove(MSG_ITEM_REMOVE* itemremove);

	// 퀘스트 관련 
	void	MSGQuestSConResult(MSG_QUEST_AUTH_RESULT* questresult);
	void	MSGQuestFConResult(MSG_QUEST_AUTH_RESULT* questresult);
	void	MSGQuestSetResult(MSG_QUEST_SET_RESULT* questresult);
	void	MSGPartyQuestCancle(MSG_QUEST_USER_CANCEL* partyquestcancle);
	void	MSGQuestData(MSG_QUEST_DATA* questdata);
	void	MSGQuestCompleteData(MSG_QUESTCOMPLETE_DATA* questcompletedata);
	void	MSGQuestCounterResult(MSG_QUEST_SETCOUNT_RESULT* questcount);
	void	MSGPartyQuestCounterResult(MSG_QUEST_PARTY_SETCOUNT_RESULT* partyquestcount);
	void	MSGQuestCharacterPointReset(MSG_QUEST_REDISTRIBUTION* questcharterpointreset);
	void	MSGQuestValidCheckResult(MSG_QUEST_VALIDCHECK_RESULT* questcheck);
	void	MSGQuestValidCheckInGameResult(MSG_QUEST_VALIDCHECK_INGAME_RESULT* questcheck);
	void	MSGQuestNodeBackItemResult(MSG_QUEST_NODEBACK_ITEM_RESULT* questcheck);
	void	MSGQuestRankingListResult(MSG_QUESTRANK_RES* questrankinglist);

	void	QuestSCONResultStartCondition(int questid, char nexttreenumber);
	void	QuestSCONResultContinueTriggerNormalAction(int questid, char nexttreenumber, char currenttreenumber, BOOL jumpaction = FALSE, BOOL bautonode = FALSE);
	void	QuestSCONResultContinueTriggerExceptionAction(int questid, char nexttreenumber, char currenttreenumber, BOOL jumpaction = FALSE, BOOL bautonode = FALSE);
	void	QuestSCONResultSuccessComplete(int questid, char currenttreenumber);
	void	QuestSCONResultFailComplete(int questid, char currenttreenumber);
	
	void	QuestFCONResultFailComplete(int questid, char currenttreenumber);
	
	void	QuestSetResultFailComplete(int questid);
	
	void	QuestItemReceive(MSG_QUESTITEM_RECEIVE* questitem);
	void	QuestItemRemove(MSG_QUESTITEM_REMOVE* questitem);	 
	
	void	QuestLearnSkill(MSG_LEARN_SKILL* questmartial);
	void	QuestForgetSkill(MSG_FORGET_SKILL* questmartial);
	
	void	QuestUpdateStatus(MSG_UPDATE_STATUS* queststatus);
	void	QuestJoinGroup(MSG_QUEST_JOIN_GROUP* questjoingroup);
	void	QuestInventoryFullProcess(MSG_QUEST_AUTH_RESULT* questresult);

	void	MSGPartyJoinReq(MSG_PARTY_JOIN_REQ* joinparty);
	void	MSGPartyResponse(MSG_PARTY_JOIN_RESPONSE* partyresponse);

	void	PartyMemberUpdate(MSG_PARTY_MEMBER_UPDATE* memberupdate);
	void	PartyMemberInfo(MSG_PARTY_MEMBER_INFO* memberinfo);
	void	PartyETCProcess(MSG_PARTY_ETC* partyetc);
	void	PartyInfoProcess(MSG_PARTY_INFO* partyinfo);
	void	PartyCloseMiniBar(void);
	void	PartyResetMiniBar(char cCaptainNo, int index);
	void	PartyShowMiniBar(void);

	void	NPCTradeResult(MSG_NPC_TRADE_RESULT* traderesult);

	void	QuestAndSuccessTriggerClientEnableReset(int questid, char currenttreenumber);
//	void	QuestAndFailTriggerClientEnableReset(int questid, char currenttreenumber);

	void	MSGPvPTradeReq(MSG_PVP_TRADE_REQ* pvptradereq);
	void	MSGPvPTradeStart(MSG_PVP_TRADE_START* pvptradestart);
	void	MSGPvPTradeDelete(MSG_PVP_TRADE_DELETE* pvptradedelete);
	void	MSGPvPTradeUpdateInfo(MSG_PVP_TRADE_UPDATE_INFO* pvptradeupdate);
	void	MSGPvPTradeEnd(MSG_PVP_TRADE_END* pvptradeend);
	void	MSGPvPTradeMoney(MSG_PVP_TRADE_MONEY* pvptrademoney);
	void	MSGPvPTradeCancel(MSG_PVP_TRADE_CANCEL* pvptradecancel);

	void	MSGSkillCtrl(MSG_SKILL_CTRL* skillctrl);
	void	MSGInvenCtrlResult(MSG_INVEN_CTRL_RESULT* invenctrlresult);
	void	MSGCtrlCharacSetting(MSG_CTRL_CHARAC_SETTING* characsetting);
	
	// stance motion change
	void	MSGStanceMotionChange(MSG_STANCE_MOTION_CHANGE* stancemotionchange);

	// 보스전 관련 함수
	void	MSGBossInviate(MSG_EX_BATTLE_INVITE* bossinvite);
	void	MSGBossResult(MSG_EX_BATTLE_RESULT* bossresult);
	void	MSGBossPlayMovie(MSG_EX_BATTLE_PLAY_VIDEO* bossplaymovie);
	void	MSGBossTimer(MSG_EX_BATTLE_TIME* bosstimer);
	void	MSGBossRoomType(MSG_EX_BATTLE_TYPE_INFO* bossroomtpye);
	void	MSGBossStageNumber(MSG_EX_BATTLE_CUR_STAGE* bossstagenumber);
	
	// 별호 관련 함수
	void	MSGNicknameListReceive(MSG_NIC_LIST* nicknamelistreceive);
	void	MSGNicknameSetting(MSG_NIC_SET* nicknamesetting);
	void	MSGNicknameSelect(MSG_NIC_SELECT* nicknameselect);
	void	MSGNicknameResult(MSG_NIC_RES* nicknameresult);
	void	MSGNicknameRemainTime(MSG_NIC_USEFULL_TIME* timeresult);
	void	MSGRankHiding(MSG_RANK_HIDING* hidingresult);
	
	// 명성 관련 함수
	void	MSGPlayerHonorResult(MSG_HONOR_PLAYER* honorresult);

	// 비무 관련 함수
	void	MSGMatchListModeRegister(MSG_MATCH_LIST* matchlist);
	void	MSGMatchListModeNotMap(MSG_MATCH_LIST* matchlist);
	void	MSGMatchListModeListUpdate(MSG_MATCH_LIST_INFO* matchlistinfo);
	void	MSGMatchListModeListContinue(MSG_MATCH_LIST_INFO* matchlistinfo);
	void	MSGMatchListModeDelete(MSG_MATCH_LIST* matchlist);
	
	void	MSGMatchReadyChallengeFromOther(MSG_MATCH_READY_CHALLENGE_FROM* matchreadyfrom);
	void	MSGMatchReadyChallengeCancel(MSG_MATCH_READY_CHALLENGE_CANCEL* matchreadycancel);
	void	MSGMatchReadyReply(MSG_MATCH_READY_REPLY* matchreadyreply);
	void	MSGMatchReadyStartMakeTable(MSG_MATCH_READY_START_MAKE_TABLE* matchreadymaketable);
	void	MSGMatchReadyUpdate(MSG_MATCH_READY_UPDATE* matchreadyupdate);
	void	MSGMatchReadySelectMember(MSG_MATCH_READY_SELECT_MEMBER* matchreadyselectmember);
	void	MSGMatchReadyDeselectMember(MSG_MATCH_READY_DESELECT_MEMBER* matchreadydeselectmember);
	void	MSGMatchReadyConfirm(MSG_MATCH_READY* matchready);
	void	MSGMatchReadyCancelAll(MSG_MATCH_READY* matchready);
	void	MSGMatchReadyError(MSG_MATCH_READY* matchready);

	void	MSGMatchCtrlStart(MSG_MATCH_CTRL_START* matchctrlstart);
	void	MSGMatchCtrlWait(MSG_MATCH_CTRL_WAIT* matchctrlwait);
	void	MSGMatchCtrlStartCount(MSG_MATCH_CTRL* matchctrl);
	void	MSGMatchCtrlStartFight(MSG_MATCH_CTRL* matchctrl);
	void	MSGMatchCtrlBeforeCountDown(MSG_MATCH_CTRL* matchctrl);
	void	MSGMatchCtrlEnd(MSG_MATCH_CTRL_END* matchctrlend);
	void	MSGMatchCtrlUpdateRecord(MSG_MATCH_CTRL* matchctrl);
	void	MSGMatchCtrlCannotExit(MSG_MATCH_CTRL* matchctrl);
	void	MSGMatchCtrlExit(MSG_MATCH_CTRL_EXIT* matchctrlexit);
	void	MSGMatchCtrlSystemError(MSG_MATCH_CTRL* matchctrl);
	
	void	MSGMatchCtrlFreeEnter(MSG_MATCH_CTRL_FREE_ENTER* matchctrl);

	void	MSGMatchNoticeInform(MSG_MATCH_NOTICE_INFORM* matchnoticeinfor);
	void	MSGMatchNoticeSBInfo(MSG_MATCH_NOTICE_SB_INFO* matchnoticesbinfo);
	void	MSGMatchNoticeLBInfo(MSG_MATCH_NOTICE_LB_INFO* matchnoticelbinfo);
	void	MSGMatchNoticeSBUpdateVital(MSG_MATCH_NOTICE_SB_UPDATE_VITAL* matchnoticesbvital);
	void	MSGMatchNoticeLBUpdateVital(MSG_MATCH_NOTICE_LB_UPDATE_VITAL* matchnoticelbvital);
	void	MSGMatchNoticeInformEnd(MSG_MATCH_NOTICE_INFORM_END* matchnoticeinformend);

	void	MSGFMApplyFail(MSG_FM_APPLY_FAIL* fmfail);
	void	MSGFMEnterMatch(MSG_FM_ENTER_MATCH* fmenter);
	void	MSGFMCtrl(MSG_FM_CTRL* fmctrl);
	void	MSGFMNotice(MSG_FM_NOTICE* fmnotice);
	void	MSGFMNoticeWinner(MSG_FM_NOTICE_WINNER* packet);
	void	MSGFMNoticeNowinner(MSG_FM_NOTICE_NOWINNER* packet);
	void	MSGFMToWinner(MSG_FM_TO_WINNER* packet);
	void	MSGFMRewardNick(MSG_FM_REWARD_NICK* packet);
	void	MSGFMRewardItem(MSG_FM_REWARD_ITEM* packet);
	void	MSGFMSetNick(MSG_FM_SET_NICK* packet);
	void	MSGFMCurrentState(MSG_FM_CURRENT_STATE* packet);
	void	MSGFMPrivateScore(MSG_FM_PRIVATE_SCORE* packet);

	// 깃발 포함 멈춰있는 오브젝트 관련
/*	void	MSGObjMatchFlagInfo(MSG_OBJ_MATCH_FLAG_INFO* matchflaginfo);
	void	MSGObjectDetailInfo(MSG_MATCH_FLAG_DETAIL_INFO* matchflagdetailinfo);
*/	
	// 인물정보
	void	MSGTargetInfo(MSG_CHARAC_MORE_INFO* targetinfo);

	// 캐릭터 상태 효과
	void	MSGEffectCtrlResultMySpell(MSG_EFFECT_CTRL_RESULT_MY_SPELL* effectctrl);
	void	MSGEffectCtrlResultMyEffect(MSG_EFFECT_CTRL_RESULT_MY_EFFECT* effectctrl);
	void	MSGEffectCtrlResultOther(MSG_EFFECT_CTRL_RESULT_OTHER* effectctrl);

	void	MSGEffectMyBHInfo(MSG_EFFECT_MY_BH_INFO* effectmybhinfo);
	void	MSGEffectMyEntryInfo(MSG_EFFECT_MY_ENTRY_INFO* effectmyentryinfo);
	void	MSGEffectOtherInfo(MSG_EFFECT_OTHER_INFO* effectotherinfo);
	void	MSGEffectOtherEntryInfo(MSG_EFFECT_OTHER_INFO* effectotherinfo);
	void	MSGEffectMySpecialInfo(MSG_EFFECT_SPECIAL_BUFF_INFO* pEffectSpecialInfo);

	void	MSGEffectCtrlResultExtra(MSG_EFFECT_CTRL_RESULT_EXTRA* effectextra);
	void	MSGEffectMyExtraInfo(MSG_EFFECT_MY_EXTRA_INFO* effectextra);
	
	void	MSGEffectCtrlResultAddSpecial(MSG_EFFECT_CTRL_RESULT_ADD_SPECIAL* addspecial);
	void	MSGEffectCtrlResultUpdateSpecial(MSG_EFFECT_CTRL_RESULT_UPDATE_SPECIAL* pUpdateSpecial);
	void	MSGEffectCtrlResultDelSpecial(MSG_EFFECT_CTRL_RESULT_DEL_SPECIAL* delspecial);
	
	void	MSGEffectCtrlResult(MSG_EFFECT_CTRL_RESULT* effectctrl);
	
	// Mastery skill
	void	MSGSkillInfoMastery(MSG_SKILL_INFO_MASTERY* skillinfo);
	void	MSGMasteryPointResult(MSG_MASTERY_POINT_RESULT* pointresult);
	
	// skill 사용 허가
	void	MSGSkillSelect(MSG_SKILL_SELECT* skillselect);
	
	// skill cooltime
	void	MSGSkillInfoCoolTime(MSG_SKILL_INFO_COOLTIME* cooltime);
	
	// GMS 관련 명령
	void	MSGGMSResult(MSG_GM_RESULT* gmsetresult);
	
	void	CheckValidItem(int slotnumber);
	
	// 분파 관련 함수 
	void	MSGOrganizationUnitUpdate(MSG_ORGANIZATION_UNIT_UPDATE_* unitupdate);
	void	MSGOrganizationInfoResult(MSG_ORGANIZATION_INFO_RES* inforesult);
	void	MSGOrganizationResult(MSG_ORGANIZATION_RESULT* result);
	void	MSGOrganizationClient( MSG_ORGANIZATION_CLIENT_* clientresult );
	void	MSGOrganizationGrobalInfo(MSG_ORGANIZATION_GLOBAL_INFO* grobalinfo);	
	void	MSGOrganizationNoticeInfo(MSG_ORGANIZATION_NOTICE* noticeinfo);
	
	// 개인 상점 관련 함수 	

	void	MSGPersonalStoreCtrlStartResult(MSG_PS_CTRL_START_RESULT* psstartresult);
	void	MSGPersonalStoreCtrlNameBroadCasting(MSG_PS_CTRL_BROAD_CAST* psnamebroadcasting);	
	void	MSGPersonalStoreCtrlStopResult(MSG_PS_CTRL_STOP_RESULT* psstopresult);	
	void	MSGPersonalStoreCtrlShoppingListResult(MSG_PS_CTRL_SHOPITEM_LIST_RESULT* psshoplistresult);	
	void	MSGPersonalStoreCtrlBuyResult(MSG_PS_CTRL_SHOPITEM_BUY_RESULT* psbuyresult);	
	void	MSGPersonalStoreCtrlStopBroadCasting(MSG_PS_CTRL_STOP_BROAD_CAST* psstopbroadcasting);	
	void	MSGPersonalStoreCtrlTradeResultBroadCasting(MSG_PS_CTRL_SHOPITEM_SOLD* pstraderesulttoowner);
	void	MSGPersonalStoreCtrlTradeResultToOwner(MSG_PS_CTRL_ITEM_BUY_TO_OWNER* pstraderesulttoowner);
	
	// 수집 관련 함수
	void	MSGCollectionResponse(MSG_COLLECT_RES* collectionresponse);
#ifdef _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI
	void	ParsingCollectionData(_CHARAC_COLLECTION_DATA collectiondata, _CHARAC_COLLECTION_DATA eventcollectiondata);
#else
	void	ParsingCollectionData(_CHARAC_COLLECTION_DATA collectiondata);
#endif
	
	void	MSGCollectionResult(MSG_COLLECT_RESULT* collection);


	// Billing message
	void	MSGBillingInfo(MSG_BILL_INFO* billinginfo);
	void	MSGVietnamBillingInfo(MSG_BILLING_INFO_ACK* billinginfoack);
	
	// 제련
	void	MSGItemInchantResult(MSG_INCHANT_PACKET* inchantresult);
	
	// npc buff
	void	MSGNPCBuffResult(MSG_NPC_BUFF_RESULT* npcbuffresult);
	
	// 혈정
	void	MSGBloodPointSettingPoint(MSG_BLOODPOINT_SETTING_POINT* setpoint);
	void	MSGBloodPointGiveItem(MSG_BLOODPOINT_GIVE_ITEM* giveitem);
	void	MSGBloodPointError(MSG_BLOODPOINT_ERROR* blperror);
	
	// pk 전적
	void	MSGPKWinLoseRecord(MSG_PK_WIN_LOSE_RECORD* record);
	void	MSGPKMoonpaRecord(MSG_PK_MOONPA_RECORD* record);
	void	MSGPKClassRecord(MSG_PK_CLASS_RECORD* record);
	void	MSGPKUpdateWin(MSG_PK_UPDATE_WIN* record);
	void	MSGPKUpdateLose(MSG_PK_UPDATE_LOSE* record);
	void	MSGPKUpdateMoonpaWin(MSG_PK_UPDATE_MOONPA_WIN* record);
	void	MSGPKUpdateMoonpaLose(MSG_PK_UPDATE_MOONPA_LOSE* record);
	void	MSGPKUpdateClassWin(MSG_PK_UPDATE_CLASS_WIN* record);
	void	MSGPKUpdateClassLose(MSG_PK_UPDATE_CLASS_LOSE* record);
	
	// Etc set - 외치기 etc.
	void	MSGEtcsetShoutSet(MSG_ETCSET_SHOUT_SET* shout);
	void	MSGEtcsetShoutEnd(MSG_ETCSET_SHOUT_END* shout);
	void	MSGEtcsetShoutError(MSG_ETCSET_SHOUT_ERROR* shout);

	// 경험치/숙련도/구룡기행
	void	MSGAiusGradeSet(MSG_AIUS_GRADE_SET* aius);
	void	MSGAiusGradeEnd(MSG_AIUS_GRADE_END* aius);

	void	MSGQuickSlot( MSG_QUICKSLOT_ALLINFO* quickslotinfo );
	void	MSGHelpSystem( MSG_HELPLOG_ALLINFO* helpsysteminfo );
	
	// XTrap
	void	SendXTrapSetOptGameInfo(void);
	void	MSGXTrapSCRequest(MSG_XTRAP_SC_REQUEST* request);
	void	MSGXTrapSCErrorNo(MSG_XTRAP_SC_ERRNO* err);
	void	MSGXTrapSCExit(MSG_XTRAP_SC_EXIT* exit);
	
	// 장원전
	BOOL	SendMSGCBReqScheduleList(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	SendMSGCBReqChallengeList(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	SendMSGCBChallengeCancel(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	SendMSGCBReqChangeTime(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	SendMSGCBReqChallenge(int type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);
	BOOL	SendMSGCBOwnerReqScheduleList(int type, int param1, int param2, int param3, int param4);
	BOOL	SendMSGCBEtcLoudChat(int type, int param1, int param2, int param3, int param4);

	void	MSGCBScheduleList(MSG_CB_SCHEDULE_LIST* schedulelist);
	void	MSGCBChallengeList(MSG_CB_CHALLENGE_LIST* challengelist);
	void	MSGCBChangeTimeSuccess(MSG_CB_CHANGE_TIME_SUCCESS* changetime);
	void	MSGCBChangeTimeFail(MSG_CB_CHANGE_TIME_FAIL* changetime);
	void	MSGCBChallengeSuccess(MSG_CB_CHALLENGE_SUCCESS* challengeresult);
	void	MSGCBChallengeFail(MSG_CB_CHALLENGE_FAIL* challengeresult);
	void	MSGCBOnwerScheduleList(MSG_CB_OWNER_SCHEDULE_LIST* schedulelist);
	void	MSGCBUAreOwner(MSG_CB_U_ARE_OWNER* owner);
	void	MSGCBUAreNotOwner(MSG_CB_U_ARE_NOT_OWNER* owner);
	void	MSGCBNotOwner(MSG_CB_NOT_OWNER* owner);
	void	MSGCBNoSuchCastle(MSG_CB_NO_SUCH_CASTLE* nocastle);
	void	MSGCBCurOwnerInfo(MSG_CB_CUR_OWNER_INFO* info);
	void	MSGCBScheduleError(MSG_CB_SCHEDULE_ERROR* error);
	
	void	MSGCBTaxSet(MSG_CB_TAX_SET* taxset);
	void	MSGCBTaxAnswerTake(MSG_CB_TAX_ANSWER_TAKE* answertake);

	void	MSGCBProgReadyEvent(MSG_CB_PROG_READY_EVENT* readyevent);
	void	MSGCBProgStartEvent(MSG_CB_PROG_START_EVENT* startevent);
	void	MSGCBProgEndEvent(MSG_CB_PROG_END_EVENT* endevent);
	void	MSGCBThreatStartOK(MSG_CB_THREAT_START_OK* threatok);
	void	MSGCBThreatStartError(MSG_CB_THREAT_START_ERROR* threaterr);
	void	MSGCBThreatStartNotice(MSG_CB_THREAT_START_NOTICE* notice);
	void	MSGCBThreatCancelOK(MSG_CB_THREAT_CANCEL_OK* cancel);
	void	MSGCBThreatCompleteOK(MSG_CB_THREAT_COMPLETE_OK* complete);
	void	MSGCBThreatCompleteError(MSG_CB_THREAT_COMPLETE_ERROR* complete);
	void	MSGCBThreatCompleteNotice(MSG_CB_THREAT_COMPLETE_NOTICE* complete);
	void	MSGCBInfoCurrentInfo(MSG_CB_INFO_CURRENT_INFO* info);
	void	MSGCBEtcLoudChat(MSG_CB_ETC_LOUD_CHAT* chat);
	void	MSGCBEtcCannotLoud(MSG_CB_ETC_CANNOT_LOUD* chat);
	void	MSGCBEtcNotice(MSG_CB_ETC_NOTICE* notice);
	void	MSGCBDeadPenalty(MSG_CB_DEAD_PENALTY* dead);
	void	MSGCBThreatPointSet(MSG_CB_THREATPOINT_SET* point);
	void	MSGCBRecordCombatAdd(MSG_CB_RECORD_COMBAT_ADD* record);
	void	MSGCBRecordPvpAward(MSG_CB_RECORD_PVP_AWARD* award);
	void	MSGCBRecordRank(MSG_CB_RECORD_RANK* rank);
	void	MSGCBRecordWinnerAward(MSG_CB_RECORD_WINNER_AWARD* winner);
	void	MSGCBRecordInfoRes(MSG_CB_RECORD_INFO_RES* res);
	
	void	MSGNiItemUse(MSG_NI_ITEM_USE* itemuse);

	
#ifdef _XTS_ALIANCE_20061018	
	void	MSGAlienceRequestFrom(MSG_ALLY_REQEUST_FROM* alience_request_from);
	void	MSGAlienceRequestNo(MSG_ALLY_MODE_RESPONSE_NO* alience_request_no);
	void	MSGAlienceMade(MSG_ALLY_MODE_IS_MADE* alience_made);
	void	MSGAlienceErrorMessage(MSG_ALLY_ERROR* alience_error);
	void	MSGAlienceAllInfo(MSG_ALLY_MODE_ALLY_ALL_INFO* alience_allinfo);
	void	MSGAlienceNewMember(MSG_ALLY_MODE_NOTICE_NEW_ORG* alience_newmember);
	void	MSGAlienceLeaveMember(MSG_ALLY_MODE_NOTICE_LEAVE_ORG* alience_leavemember);
	void	MSGAlienceScattered(MSG_ALLY_MODE_SCATTERED* alience_scattered);
	void	MSGAlienceAcceptLeave(MSG_ALLY_MODE_ALLOW_LEAVE* alienceaccept_leave);
	void	MSGAlienceAcceptScatter(MSG_ALLY_MODE_ALLOW_SCATTER* alienceaccept_scatter);
	void	MSGAlienceLeaveResult(MSG_ALLY_MODE_BANISH_COMPLETE* alience_leaveresult);
	void	MSGAlienceOtherInfo(MSG_ALLY_MODE_OTHER_ALLY_INFO * alience_otherinfor);
	void	MSGAlienceNoticeLeave(MSG_ALLY_MODE_NOTICE_BANISH_ORG* alience_noticeleave);
	void	MSGAlienceNoInfo(MSG_ALLY_MODE_NO_ALLY_INFO* alience_noinfo);
	void	MSGAlienceEnter(MSG_ALLY_MODE_ORG_ENTER_ALLY* alience_enter);
#endif		
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	void	MSGPKLevelPenaltyInfo(MSG_LTS_LVPK_INFO_GS_CL* pklevelinfo);					//레벨차 모든정보
	void	MSGPKLevelPenaltyInsert(MSG_LTS_LVPK_INSERT_GS_CL* pklevel);					//레벨차 패널티 적용자 리스트 추가
	void	MSGPKLevelPenaltyDelete(MSG_LTS_LVPK_DEL_REQ_GS_CL* pklevel);					//레벨차 패널티 적용자 리스트 학제
	void	MSGPKTracingInfo(MSG_LTS_PK_INFO_GS_CL* pktracinginfo);							//PK범 모든정보
	void	MSGPKTracingInsertSuccess(MSG_LTS_PK_INSERT_SUCCESS_GS_CL* pktracinginfo);		//PK범 리스트 추가 성공
	void	MSGPKTracingInsertFail(MSG_LTS_PK_INSERT_FAILED_GS_CL* pktracinginfo);			//PK범 리스트 추가 실패
	void	MSGPKTracingDeleteSuccess(MSG_LTS_PK_DEL_SUCCESS_GS_CL* pktracinginfo);			//PK범 리스트 삭제 성공
	void	MSGSomeOneTracingInfo(MSG_LTS_SP_INFO_GS_CL* tracinginfo);						//특정인 모든정보
	void	MSGSomeOneTracingInsertSuccess(MSG_LTS_SP_INSERT_SUCCESS_GS_CL* tracinginfo);	//특정인 리스트 추가 성공
	void	MSGSomeOneTracingInsertFail(MSG_LTS_SP_INSERT_FAILED_GS_CL* tracinginfo);		//특정인 리스트 추가 실패
	void	MSGSomeOneTracingDeleteSuccess(MSG_LTS_SP_DEL_SUCCESS_GS_CL* tracinginfo);		//특정인 리스트 삭제 성공
	void	MSGTracingPositionInfo(MSG_LTS_POS_INFO_GS_CL* tracinginfo);					//3초마다 PK범 특정인 (캐릭명 + 위치정보)
	void	MSGPKTargetName(MSG_LTS_PK_INSERT_REQ_GS_CL* pkname);							//pk범 이름
#endif		
	
#ifdef _XDEF_COMMONITEMMALL_20070402	//범용 아이템 몰 관련 함수
	void	MSGCISInvenAck(MSG_COMMON* pCommon);				//캐쉬 박스에 아이템 정보를 알려준다.
	void	MSGCISMoveItemAck(MSG_COMMON* pCommon);		//캐쉬 박스에서 행낭으로 아이템을 이동한다.
	void	MSGCISPresentMsg(MSG_COMMON* pCommon);		//선물 메세지를 표시한다.
#endif

	// 흑백 대전 
	void	MSGBWLeftTime(MSG_BW_LEFT_TIME* time);
	void	MSGBWEnterGateResult(MSG_BW_ENTER_GATE_SUCCESS* reuslt);

	void	MSGMasternDiscipleResult(MSG_MASTER_AND_PUPIL* pMnDResult); //Author : 양희왕 //breif : 사제 시스템

private:
	void MSGRecallItemResult( MSG_RECALL_FRIEND* pRecallresult ); //Author : 양희왕 //breif : 친구소환 관련

	void PcRoomStateResult( const MSG_COMMON_NTF& CommonNTF ); /*!< 피씨방	@param CommonNTF 피씨방 여러 정보 */
};

/**
@class _XNetwork
@breif Doxygen Test 지우지말아요~
@author 양희왕
*/

#ifdef _XTESTSERVER

#define _XDEF_TESTSERVERSTRUCTINDICATOR		0xFD4E2A4C

typedef struct _XSERVERINFOSTRUCT
{
	DWORD	testserverindicator;
	TCHAR	cdnserverurl[64];
	DWORD	serverport;
	TCHAR	serveraddress[32];
	DWORD	patchserverport;
	TCHAR	patchserveraddress[32];	
} _XSERVERINFOSTRUCT;

#else

typedef struct _XSERVERINFOSTRUCT
{
	TCHAR	patchserveraddress[32];
	DWORD	patchserverport;
	TCHAR	cdnserverurl[64];
	TCHAR	serveraddress[32];
	DWORD	serverport;
} _XSERVERINFOSTRUCT;

#endif

#define _XADDRESSENCODEKEY	0xDC

class _XServerAddressManager
{
	
public:
	_XSERVERINFOSTRUCT	m_ServerInfo;
	
public:
	BOOL GetServerInfo( void );
	
	void SetServerAddress( LPTSTR address );
	void SetPatchServerAddress( LPTSTR address );
	
	void SetServerPort( DWORD port );
	void SetPatchServerPort( DWORD port );
	
	void SetCDNServerUrl( LPTSTR url );
	
	void XOREncrypt( void );
};

extern _XServerAddressManager	g_ServerAddressManager;


#define _XPATCHVERSIONENCODEKEY		0xBF

#ifdef _XTESTSERVER

typedef struct _XPATCHVERSIONINFOSTRUCT
{
	DWORD	testserverindicator;
	DWORD	patchversion_2 : 16;	
	DWORD	patchversion_1 : 16;		
	DWORD	exeversion_3;
	DWORD	exeversion_2;
	DWORD	exeversion_1;	
	DWORD	exeversion_4;	
} _XPATCHVERSIONINFOSTRUCT;

#else

typedef struct _XPATCHVERSIONINFOSTRUCT
{
	DWORD	patchversion_1 : 16;
	DWORD	patchversion_2 : 16;	
	DWORD	exeversion_4;
	DWORD	exeversion_1;	
	DWORD	exeversion_3;
	DWORD	exeversion_2;
} _XPATCHVERSIONINFOSTRUCT;

#endif

class _XPatchVersionManager
{
	
public:
	_XPATCHVERSIONINFOSTRUCT	m_VersionInfo;
	
public:
	BOOL	GetVersionInfo( void );
	DWORD	GetExeVersion( void );
	
	void	SetPatchNumber( DWORD version );
	
	void	SetExeVersionnumber( DWORD v1, DWORD v2, DWORD v3, DWORD v4 );	
		
	void	XOREncrypt( void );
};

extern _XPatchVersionManager	g_PatchVersionManager;

#endif // !defined(_XNETWORK_H_)
