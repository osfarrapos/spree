#pragma	   once
#include "ole_db_module.h"

class COrganizationManager ;
class CAllyOrganization;


class COleDBExcuterBase
{
protected:
	CDB_Connector * const m_pDBCon ;
	CDB_Session * const  m_pDBSession ;		
protected:
	const CSession * const m_session;

public:
	COleDBExcuterBase(CDB_Connector * pDBCon) 
		: m_pDBCon(pDBCon),
		m_pDBSession( pDBCon->NewSession() ),
		m_session(m_pDBSession->GetSessionPtr())
	{
		if( m_session == NULL )
		{
			throw "" ;
		}
	}

	void DisConnect()
	{
		 m_pDBCon->FreeSession( m_pDBSession ) ;
	}

	virtual ~COleDBExcuterBase() = 0 ;
	virtual void CallReConTh() = 0 ;	
};

/*
#ifdef _CIS_20070326_
class CDBCmd_CIS : public COleDBExcuterBase {
public:
	auto_ptr<C_prGetCashItemList>	m_pGetCashItemList;

public:
	CDBCmd_CIS(CDB_Connector* pDBCon) : 
		COleDBExcuterBase(pDBCon),
		m_pGetCashItemList(new C_prGetCashItemList)
	{}
	~CDBCmd_CIS() {}

	virtual void CallReConTh();

	bool GetCashItemList(BYTE byPageSize, BYTE bySetPage, SHORT shGameServerIdx, char* szAcc, BYTE byPresent);
};
#endif
*/

#ifdef ITEMSHOP_PATCH_200607
class CDBCommand_WebShop : public COleDBExcuterBase {
public:
	auto_ptr<C_prTest>			m_pTest;
	auto_ptr<C_Test2>			m_pTest2;

public:
	CDBCommand_WebShop(CDB_Connector* pDBCon) : COleDBExcuterBase(pDBCon),
		m_pTest(new C_prTest),
		m_pTest2(new C_Test2)
	{}
	~CDBCommand_WebShop() {}

	virtual void CallReConTh();

	BOOL Test();
	BOOL Test2();
};
#endif

class C_USER_DB_Command : public COleDBExcuterBase
{
private:
	auto_ptr<C_prDS_Login>				m_pDSLogin;
	auto_ptr<C_prDS_NexonLogin>			m_pDSNexonLogin;
	auto_ptr<C_prDS_GetAccountUID>		m_pDSGetAccountUID;
	auto_ptr<C_prDS_Logout>				m_pDSLogout;
	auto_ptr<C_prDS_EnterCheck>			m_pDSEnterCheck;
	auto_ptr<C_prDS_NexonEnterCheck>	m_pDSNexonEnterCheck;
	auto_ptr<C_prDS_PW_CHECK>			m_pPW_CHECK;
	auto_ptr<C_prDS_All_Logout>			m_pAllLogout;
	auto_ptr<C_prDS_NewJoiner_CHECK>	m_pDSNewJoinerCheck;
	auto_ptr<C_prLS_GetIsPcbang>		m_pDSGetIsPcbang;
	auto_ptr<C_prEvent_CheckAndGive>	m_prEventCheckAndGive;
	auto_ptr<C_prEvent_PreCheck>		m_prEventPreCheck;
	auto_ptr<C_prEvent_Cancel>			m_prEventCancel;
	auto_ptr<C_prEvent_Success>			m_prEventSuccess;
	auto_ptr<C_prDS_AddPlayTime>		m_prAddPlayTime;

#ifdef _VINA_SHUTDOWN_20061023_
	auto_ptr<C_prDS_GetFatigueInfo>		m_pGetFatigueInfo ;
	auto_ptr<C_prDS_UpdateFatigueInfo>	m_pUpdateFatigueInfo ;
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
	auto_ptr<C_prDS_GetInviteFriendState>		m_pGetInviteFriendState;
	auto_ptr<C_prDS_UpdateInviteFriendState>	m_pUpdateInviteFriendState;
#endif _PD_EVENT_INVITE_FRIEND_

public:

	C_USER_DB_Command(CDB_Connector * pDBCon) : COleDBExcuterBase(pDBCon), 		  
		m_pDSLogin(new C_prDS_Login),		
		m_pDSNexonLogin(new C_prDS_NexonLogin),
		m_pDSGetAccountUID(new C_prDS_GetAccountUID),
		m_pDSLogout(new C_prDS_Logout),
		m_pDSEnterCheck(new C_prDS_EnterCheck),
		m_pDSNexonEnterCheck(new C_prDS_NexonEnterCheck),
		m_pAllLogout(new C_prDS_All_Logout),
		m_pPW_CHECK(new C_prDS_PW_CHECK),
		m_pDSNewJoinerCheck(new C_prDS_NewJoiner_CHECK),
		m_pDSGetIsPcbang(new C_prLS_GetIsPcbang),
		m_prEventCheckAndGive(new C_prEvent_CheckAndGive),
		m_prEventPreCheck(new C_prEvent_PreCheck),
		m_prEventCancel(new C_prEvent_Cancel),
		m_prAddPlayTime(new C_prDS_AddPlayTime),

#ifdef _VINA_SHUTDOWN_20061023_
		m_pGetFatigueInfo(new C_prDS_GetFatigueInfo),
		m_pUpdateFatigueInfo(new C_prDS_UpdateFatigueInfo),
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
		m_pGetInviteFriendState(new C_prDS_GetInviteFriendState),
		m_pUpdateInviteFriendState(new C_prDS_UpdateInviteFriendState),
#endif //_PD_EVENT_INVITE_FRIEND_
		m_prEventSuccess(new C_prEvent_Success)

	{		  
	}
	~C_USER_DB_Command()
	{		  
	}	  

	virtual void CallReConTh() ;

	// User data base
	bool DSLogin( IN OUT char * acc, int forest, u_short temp_uid, int * uid) ;
	bool DSGetAccountUID( IN char * acc, int& uid ) ;
	bool DSNexonLogin( IN int acc_uid, IN char * acc, IN short forest, IN u_short temp ) ;
	bool DSLogout(char * acc) ;
	bool DSEnterCheck(char * acc, int forest, int state, int uid) ;
	bool DSNexonEnterCheck(char * acc, int forest, int state, int uid) ;

	bool AllLogout(int pwd);
	bool PWCheck(int uid, char * pw) ;
	int IsNewJoiner( int uid ) ;

	bool IsPcbang( char * const ip ) ;

	short DSEventCheck( IN short evid, IN char * key, IN char * acc, IN char * chrName, 
		IN short group, OUT int& index, OUT short& packageID );
	short DSEventPreCheck( IN short evid, IN char * acc, IN char * chrName, short group );
	void DSEventCancel( IN short evid, IN int index );
	void DSEventSuccess( IN short evid, IN int index );

#ifdef _PD_ACCOUNT_PLAY_TIME_
	void AddPlayTime( IN int iAccUid, IN char * acc, IN int time, IN short gn );
#endif

#ifdef _VINA_SHUTDOWN_20061023_
	bool GetFatigueInfo( int uid, CUser * pUser ) ;
	bool UpdateFatigueInfo( int uid, short sFatigueWeight ) ;
#endif


#ifdef _PD_EVENT_INVITE_FRIEND_
	bool IF_GetInfo( char * acc, int &index, int &endTime, short &checkState );
	void IF_SetStart( int index, int endTime );
	void IF_EndUse( int index );
#endif //_PD_EVENT_INVITE_FRIEND_


	
	  
protected:
private:
};


class CDSCastleManager;		// 함수 레퍼런스 인자로 필요
class CDSCastleInfo;		// 함수 레퍼런스 인자로 필요
#ifdef _PD_CASTLE_STEP_2_
class CRecordOfCombat;
#endif

#ifdef _PD_GAME_EVENT_
class CGameEventManager;
#endif

#ifdef _PD_MASTER_PUPIL_
class CDSMaster;
#endif


class COLE_DB_Executer : public COleDBExcuterBase
{
private:
	
	auto_ptr<C_prDS_CreateCharac>	m_pCreateCharac;
	auto_ptr<C_prDS_Delete>			m_pCharacDelete;

	auto_ptr<C_prDS_UpdateLevel>	m_pUpdateLevelAll;
	auto_ptr<C_prDs_GetCharacLevel>	m_pGet5StarLevel;
	auto_ptr<C_prDs_UpdateCharacState>	m_pUpdateCharacState;
	auto_ptr<C_prDh_GetCharacState>		m_pGetCharacState;
	auto_ptr<C_prDS_UpdateCurBasicInfo>	m_pUpdateCurBasicInfo;

	auto_ptr<C_prDS_GetCurBasicInfo>	m_pGetCurBasicInfo;
	auto_ptr<C_prDS_CountCharac>		m_pCharacCount;
	auto_ptr<C_prDS_GetCharacInfo>		m_pGetCharacInfo;
	auto_ptr<C_prDs_GetInven>			m_pGetInven;
	auto_ptr<C_prDs_UpdateInven>		m_pUpdateInven;
    //auto_ptr<C_prDs_UpdateInven_v3>		m_pUpdateInven_v3;

	auto_ptr<C_prDS_GetQuest>			m_pGetQuest;
	auto_ptr<C_prDS_UpdateQuest>		m_pUpdateQuest;
	//auto_ptr<C_prDs_GetDepot>			m_pGetDepot;
	auto_ptr<C_prDs_UpdateDepot>		m_pUpdateDepot;

	auto_ptr<C_prDS_UpdateCharac>		m_pUpdateCharac;
	auto_ptr<C_prDS_GetMartial>			m_pGetMartial;
	auto_ptr<C_prDs_UpdateMartial>		m_pUpdateMartial;
	auto_ptr<C_prDS_SelectCharac>		m_pSelectCharac ;

	auto_ptr<C_prCharacRename>			m_pCharacRename ;
	auto_ptr<C_prDs_GetAcc>				m_pGetAccName ;

	auto_ptr<C_prDS_GetLastServer>		m_prDSGetLastServer ;
	auto_ptr<C_prDS_SetLastServer>		m_prDSSetLastServer ;

	auto_ptr<C_prDS_GetNic>				m_pGetNic ;
	auto_ptr<C_prDS_SetNic>				m_pSetNic ;

	auto_ptr<C_prDS_GetMatch>			m_pGetMatch ;
	auto_ptr<C_prDS_SetMatch>			m_pSetMatch ;

#ifdef _PD_BUFF_COUNT_MODIFY_
	auto_ptr<C_prDS_GetEffect_V2>		m_pGetEffect2;
	auto_ptr<C_prDS_SetEffect_V2>		m_pSetEffect2;
#else
	auto_ptr<C_prDS_GetEffect>			m_pGetEffect ;
	auto_ptr<C_prDS_SetEffect>			m_pSetEffect ;
#endif


	auto_ptr<C_prDS_GetPkRecord>		m_pGetPkRecord ;
	auto_ptr<C_prDS_UpdatePkRecord>		m_pUpdatePkRecord ;

	auto_ptr<C_prDSGetCharacItemUseStatus>		m_pGetCharacItemUseStatus ;
	auto_ptr<C_prDSUpdateCharacItemUseStatus>	m_pUpdateCharacItemUseStatus ;
	auto_ptr<C_prDSGetAccountItemUseStatus>		m_pGetAccountItemUseStatus ;
	auto_ptr<C_prDSUpdateAccountItemUseStatus>	m_pUpdateAccountItemUseStatus ;

	auto_ptr<C_prDS_CheckAndChangeName>	m_pCheckAndChangeName ;
	auto_ptr<C_prDs_GetBackupData>		m_pGetBackupData ;

	auto_ptr<C_pr_prCharacLogout>		m_pCharacLogout ;

#ifdef _PD_PKEVENT_BOX_SPAWN_
	auto_ptr<C_prDS_GetBWWarWinTime>		m_pGetBWWinTime;
	auto_ptr<C_prDS_UpdateBWWarWinTime>		m_pUpdateBWWinTime;
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
	auto_ptr<C_prDS_GetInviteFriendState>		m_pGetInviteFriendState;
	auto_ptr<C_prDS_UpdateInviteFriendState>	m_pUpdateInviteFriendState;
#endif _PD_EVENT_INVITE_FRIEND_

	auto_ptr<C_prDSInsertOrganization>	m_pInsertOrganization ;
	auto_ptr<C_prDSUpdateOrganization>	m_pUpdateOrganization ;
	auto_ptr<C_prDSDeleteOrganization>	m_pDeleteOrganization;

#ifdef _PD_ALLY_ORG_STEP_1_
	auto_ptr<C_AllyCreate>				m_pAllyCreate;
	auto_ptr<C_AllyUpdate>				m_pAllyUpdate;
	auto_ptr<C_AllyBreakup>				m_pAllyBreakup;
	auto_ptr<C_AllyUpdateOfOrg>			m_pAllyUpdateOfOrg;
#ifdef _PD_CASTLE_STEP_1_
	auto_ptr<C_AllyGetFirstOrg>			m_pAllyGetFirstOrg;
#endif
#endif

	auto_ptr<C_prDS_GetQuickSlot>		m_pGetQuickSlot ;
	auto_ptr<C_prDS_UpdateQuickSlot>	m_pUpdateQuickSlot ;

#ifdef _PD_CHARAC_LTS_
	auto_ptr<C_prDS_GetLts>				m_pGetLts;
	auto_ptr<C_prDS_SetLts>				m_pSetLts;
	auto_ptr<C_prDS_GetCUID>			m_pGetCUID;
#endif

#ifdef _PD_CASTLE_STEP_1_
	auto_ptr<C_prDSSetDefendCastle>		m_pSetDefendCastle;
	auto_ptr<C_prDSUpdateAttackCastle>	m_pUpdateAttackCastle;
	auto_ptr<C_prDSSetManorCombat>		m_pSetManorCombat;
	auto_ptr<C_prDSCastleInsert>		m_pCastleInsert;
#endif

#ifdef _PD_CASTLE_STEP_2_
	auto_ptr<C_prDSInsertOrgStandings>		m_pInsertOrgStandings;	
	auto_ptr<C_prDSUpdateOrgStandings_bi>	m_pUpdateOrgStandings_bi;	
	auto_ptr<C_prDSUpdateOrgRecordInManor>	m_pUpdateOrgRecordInManor;	
#endif //_PD_CASTLE_STEP_2_

#ifdef _PD_GAME_EVENT_
	auto_ptr<C_prDS_FM_UpdateWinner>			m_pFMUpdateWinnerInfo;
#endif

#ifdef _PD_LEVEL_PRESENT_
	auto_ptr<C_prDS_LevelPresent_GetNotRecv>	m_prLP_GetNotRecv;
	auto_ptr<C_prDS_LevelPresent_Check>			m_prLP_Check;
	auto_ptr<C_prDS_LevelPresent_Recv>			m_prLP_Recv;
	auto_ptr<C_prDS_LevelPresent_Cancel>		m_prLP_Cancel;
#endif

#ifdef _PD_MASTER_PUPIL_
	auto_ptr< CDB_Command<_db_prDS_MP_GetMasterFromPupil> >	m_prMP_GetMasterFromPupil;
	auto_ptr< CDB_Command<_db_prDS_MP_GetMasterInfo> >		m_prMP_GetMasterInfo;
	auto_ptr< CDB_Command<_db_prDS_MP_GetPupilInfo> >		m_prMP_GetPupilInfo;
	auto_ptr< CDB_Command<_db_prDS_MP_CreateMasterInfo> >	m_prMP_CreateMasterInfo;
	auto_ptr< CDB_Command<_db_prDS_MP_UpdateMasterClass> >	m_prMP_UpdateMasterClass;
	auto_ptr< CDB_Command<_db_prDS_MP_UpdateMasterNick> >	m_prMP_UpdateMasterNick;
	auto_ptr< CDB_Command<_db_prDS_MP_UpdateMasterInfo> >	m_prMP_UpdateMasterInfo;
	auto_ptr< CDB_Command<_db_prDS_MP_AddNewPupil> >		m_prMP_AddNewPupil;
	auto_ptr< CDB_Command<_db_prDS_MP_UpdatePupilBase> >	m_prMP_UpdatePupilBase;
	auto_ptr< CDB_Command<_db_prDS_MP_UpdatePupilPoint> >	m_prMP_UpdatePupilPoint;
	auto_ptr< CDB_Command<_db_prDS_MP_UpdatePupilLastCon> >	m_prMP_UpdatePupilLastCon;
	auto_ptr< CDB_Command<_db_prDS_MP_BackupPupil> >		m_prMP_BackupPupil;

#endif
public:


	COLE_DB_Executer(CDB_Connector * pDBCon) : COleDBExcuterBase(pDBCon),		
		m_pCreateCharac(new C_prDS_CreateCharac),
		m_prDSGetLastServer(new C_prDS_GetLastServer),
		m_prDSSetLastServer(new C_prDS_SetLastServer),
		m_pSelectCharac(new C_prDS_SelectCharac),		
		m_pCharacDelete(new C_prDS_Delete),		
		m_pUpdateLevelAll(new C_prDS_UpdateLevel),		
		m_pGet5StarLevel(new C_prDs_GetCharacLevel),		
		m_pUpdateCharacState(new C_prDs_UpdateCharacState),
		m_pGetCharacState(new C_prDh_GetCharacState),		
		m_pUpdateCurBasicInfo(new C_prDS_UpdateCurBasicInfo),
		m_pGetCurBasicInfo(new C_prDS_GetCurBasicInfo),		
		m_pCharacCount(new C_prDS_CountCharac),
		m_pGetCharacInfo(new C_prDS_GetCharacInfo),
		m_pGetInven(new C_prDs_GetInven),		
		m_pUpdateInven(new C_prDs_UpdateInven),
//		m_pUpdateInven_v3( new C_prDs_UpdateInven_v3),
		m_pGetQuest(new C_prDS_GetQuest ),
		m_pUpdateQuest(new C_prDS_UpdateQuest),
		//m_pGetDepot(new C_prDs_GetDepot ),
		m_pUpdateDepot(new C_prDs_UpdateDepot ),
		m_pUpdateCharac(new C_prDS_UpdateCharac),
		m_pGetMartial(new C_prDS_GetMartial),
		m_pUpdateMartial(new C_prDs_UpdateMartial),
		m_pCharacRename(new C_prCharacRename),
		m_pGetAccName(new C_prDs_GetAcc),
		m_pGetNic(new C_prDS_GetNic ),
		m_pSetNic(new C_prDS_SetNic),
		m_pGetMatch(new C_prDS_GetMatch),
		m_pSetMatch(new C_prDS_SetMatch),
#ifdef _PD_BUFF_COUNT_MODIFY_
		m_pGetEffect2(new C_prDS_GetEffect_V2),
		m_pSetEffect2(new C_prDS_SetEffect_V2),
#else
		m_pGetEffect(new C_prDS_GetEffect),
		m_pSetEffect(new C_prDS_SetEffect),
#endif
		m_pGetPkRecord(new C_prDS_GetPkRecord),
		m_pUpdatePkRecord(new C_prDS_UpdatePkRecord),

		m_pGetCharacItemUseStatus( new C_prDSGetCharacItemUseStatus ),
		m_pUpdateCharacItemUseStatus( new C_prDSUpdateCharacItemUseStatus ),
		m_pGetAccountItemUseStatus( new C_prDSGetAccountItemUseStatus ),
		m_pUpdateAccountItemUseStatus( new C_prDSUpdateAccountItemUseStatus ),

		m_pGetBackupData(new C_prDs_GetBackupData),

#ifdef _PD_PKEVENT_BOX_SPAWN_
		m_pGetBWWinTime(new C_prDS_GetBWWarWinTime),
		m_pUpdateBWWinTime(new C_prDS_UpdateBWWarWinTime),
#endif

		m_pCharacLogout(new C_pr_prCharacLogout),

		m_pGetQuickSlot(new C_prDS_GetQuickSlot),
		m_pUpdateQuickSlot(new C_prDS_UpdateQuickSlot),

#ifdef _PD_CHARAC_LTS_
		m_pGetLts(new C_prDS_GetLts),
		m_pSetLts(new C_prDS_SetLts),
		m_pGetCUID(new C_prDS_GetCUID),
#endif

		m_pInsertOrganization(new C_prDSInsertOrganization),		
		m_pUpdateOrganization(new C_prDSUpdateOrganization),
		m_pDeleteOrganization(new C_prDSDeleteOrganization),

#ifdef _PD_ALLY_ORG_STEP_1_
		m_pAllyCreate(new C_AllyCreate),
		m_pAllyUpdate(new C_AllyUpdate),
		m_pAllyBreakup(new C_AllyBreakup),
		m_pAllyUpdateOfOrg(new C_AllyUpdateOfOrg),
#ifdef _PD_CASTLE_STEP_1_
		m_pAllyGetFirstOrg(new C_AllyGetFirstOrg),
#endif
#endif

#ifdef _PD_CASTLE_STEP_1_
		m_pSetDefendCastle(new C_prDSSetDefendCastle),
		m_pUpdateAttackCastle(new C_prDSUpdateAttackCastle),
		m_pSetManorCombat(new C_prDSSetManorCombat),
		m_pCastleInsert(new C_prDSCastleInsert),
#endif

#ifdef _PD_CASTLE_STEP_2_
		m_pInsertOrgStandings(new C_prDSInsertOrgStandings ),		
		m_pUpdateOrgStandings_bi(new C_prDSUpdateOrgStandings_bi),		
		m_pUpdateOrgRecordInManor(new C_prDSUpdateOrgRecordInManor),		
#endif //_PD_CASTLE_STEP_2_

#ifdef _PD_LEVEL_PRESENT_
		m_prLP_GetNotRecv(new C_prDS_LevelPresent_GetNotRecv),
		m_prLP_Check(new C_prDS_LevelPresent_Check),
		m_prLP_Recv(new C_prDS_LevelPresent_Recv),
		m_prLP_Cancel(new C_prDS_LevelPresent_Cancel),
#endif

#ifdef _PD_GAME_EVENT_
		m_pFMUpdateWinnerInfo(new C_prDS_FM_UpdateWinner),
#endif

#ifdef _PD_MASTER_PUPIL_
		m_prMP_GetMasterFromPupil( new CDB_Command<_db_prDS_MP_GetMasterFromPupil> ),
		m_prMP_GetMasterInfo( new CDB_Command<_db_prDS_MP_GetMasterInfo> ),
		m_prMP_GetPupilInfo( new CDB_Command<_db_prDS_MP_GetPupilInfo> ),
		m_prMP_UpdateMasterInfo( new CDB_Command<_db_prDS_MP_UpdateMasterInfo> ),
		m_prMP_CreateMasterInfo( new CDB_Command<_db_prDS_MP_CreateMasterInfo> ),
		m_prMP_UpdateMasterClass( new CDB_Command<_db_prDS_MP_UpdateMasterClass> ),
		m_prMP_UpdateMasterNick( new CDB_Command<_db_prDS_MP_UpdateMasterNick> ),
		m_prMP_AddNewPupil( new CDB_Command<_db_prDS_MP_AddNewPupil> ),
		m_prMP_UpdatePupilBase ( new CDB_Command<_db_prDS_MP_UpdatePupilBase> ),
		m_prMP_UpdatePupilPoint ( new CDB_Command<_db_prDS_MP_UpdatePupilPoint> ),
		m_prMP_UpdatePupilLastCon ( new CDB_Command<_db_prDS_MP_UpdatePupilLastCon> ),
		m_prMP_BackupPupil ( new CDB_Command<_db_prDS_MP_BackupPupil> ),
#endif

		m_pCheckAndChangeName(new C_prDS_CheckAndChangeName)
	{
	}

	~COLE_DB_Executer()
	{		
	}

	virtual void CallReConTh() ;

	bool GetCharacAllInfo( int uid, _C_CHARAC * pCharac ) ;
	
	// Charac data base
	int GetLastServer(int unique_id) ;
	void SetLastServer(int unique_id, int last_server) ;	

	bool SelectCharac( char * characName ) ;

	int pr_Charac_Create( char * acc, CMSG_CHARAC_CREATE_GS_DS * pCreate, int accid, short luck, int * uid ) ;	// 캐릭터 생성 함수
	int pr_Charac_Delete( int uid ) ;					// 캐릭터 삭제
	
	bool pr_Update_LevelAll( int uid, _C_CHARAC * pCharac ) ;	
	bool pr_Get_Attr( int uid, _C_CHARAC * pCharac ) ;

	bool pr_Update_CharacState( int uid, _C_CHARAC * pCharac ) ;
	bool pr_Get_CharacState( int uid, _C_CHARAC * pCharac ) ;

	bool pr_Update_CurBasicInfo( int uid, _C_CHARAC * pCharac ) ;
	bool pr_Get_CurBasicInfo( int uid, _C_CHARAC * pCHARAC ) ;

	// jk.Hong Edite
	bool Get_Charac_List( int acc_uid, _USER * pUser) ;
	bool Get_Charac_Info( int uid, _C_CHARAC * pCharac );
	bool UpdateCharac(int uid, _C_CHARAC * pCharac );

	bool inven_get( int unique, _C_CHARAC * pCharac ) ;
	bool inven_update( int unique, _C_CHARAC * pCharac );
	//bool inven_update_v2( int unique, _C_CHARAC * pCharac ) ;
	//bool inven_update_v3( int unique, _C_CHARAC * pCharac );

	// 퀘스트
	bool pr_Update_QuestInfo( int uid, _C_CHARAC * pCharac );
	bool pr_Get_QuestInfo( int uid, _C_CHARAC * pCharac );

	// 무공
	bool pr_Get_MartialData( int uid, _C_CHARAC * pCharac );
	bool pr_Update_MartialData( int uid, _C_CHARAC * pCharac );

	bool Update_Charac_All( int uid, _C_CHARAC * pCharac );

	//bool UpdateUserData(char * acc, PUSER pUser);

	// 창고
	bool pr_GetDepot(int uid, CUser * pUser);
	bool pr_UpdateDepot(int uid, CUser * pUser);

	// 캐릭 이름 바꾸기 (GMS 사용)
	bool CharacRename( char * cCurName, char * cNewName ) ;
	void GetAccName(char * cCharacName, char * cAccName ) ;

	//별호 가져오기
	bool GetNic( int uid, _C_CHARAC * pCharac ) ;
	bool SetNic( int uid, _C_CHARAC * pCharac ) ;

	// 비무 정보
	bool GetMatch( int uid, _C_CHARAC * pCharac ) ;
	bool SetMatch( int uid, _C_CHARAC * pCharac ) ;

	bool GetEffect( int uid, _C_CHARAC * pCharac ) ;
	bool SetEffect( int uid, _C_CHARAC * pCharac ) ;

	bool GetPkRecord( int uid, _C_CHARAC * pCharac ) ;
	bool UpdatePkRecord( int uid, _C_CHARAC * pCharac ) ;

	bool GetCharacItemUseStatus( int cid, _C_CHARAC * pCharac ) ;
	bool UpdateCharacItemUseStatus( int cid, _C_CHARAC * pCharac ) ;

	bool GetAccountItemUseStatus( int cid, CUser * pUser ) ;
	bool UpdateAccountItemUseStatus( int cid, CUser * pUser ) ;

	bool GetCharacQuickSlot( int uid, _C_CHARAC * pCharac ) ;
	bool UpdateCharacQuickSlot( int cid, _C_CHARAC * pCharac ) ;

#ifdef _PD_CHARAC_LTS_
	bool GetCharacLts( int uid, _C_CHARAC * pCharac );
	bool SetCharacLts( int uid, _C_CHARAC * pCharac );
	int GetCharacCUID( char * name );
#endif

	//test
	void GetBackupData() ;

	bool GetRank_Level() ;


	bool GetOrInfo( OUT COrganizationManager * pOrManager ) ;

	int InsertOrganization( IN const _Organization & or ) ;
	bool UpdateOrganization( IN const _Organization & or ) ;
	bool DeleteOrganization( int index ) ;

#ifdef _PD_ALLY_ORG_STEP_1_
	int GetAllyInfo( CAllyManager * pAllyManager, COrganizationManager * pOrManager );

	int AllyCreate( int org1, int org2, char * orgName, short pr );
	void AllyUpdate( int index, int org2, int org3, int org4, int org5, short pr );
	void AllyBreak( int index, short reason );	// reason, 1 - active, 2 - passive
	void AllyUpdateOfOrg( int org, int ally );
#ifdef _PD_CASTLE_STEP_1_
	int AllyGetMasterOrgInAllAlly( int ally_index );
#endif

#endif	// _PD_ALLY_ORG_STEP_1_

#ifdef _PD_CASTLE_STEP_1_
	int ShowDefendCastleInfo();
	int ShowAttackCastleInfo();

	int GetDefendCastleInfo( int dfd_num, CDSCastleInfo * pCastle );
	int GetAttackCastleInfo( int map_id, CDSCastleInfo * pCastle );
	
	int GetManorCombatInfo( CDSCastleManager * pCastleManager );
	int GetDefendDespotInfo( CDSCastleManager * pCastleManager );

	int InsertDefendCastleInfo( int map_id );

	int InsertAttackCastle( int map_id, __cb_challenge_info * pInfo );
	void CompleteChallenge( int db_seq );
	void CancelChallenge( int db_seq );

	bool FillCastleDetailInfo( short castle_id, int db_seq_index, CDSCastleInfo * info );
	int CastleInsert( short castle_id );

	int SetManorCombatInfo();
	int UpdateDefendCastleInfo( CDSCastleInfo * pInfo, int insert_or_update	/* 0 - insert, 1 - update */ );
#endif

	void SelectInvenInfo( char cMode ) ;
	void SelectDepotInfo( char cMode ) ;		


	bool GetRankVersionInfo() ;

	void CharacLogout( int uid ) ;

	void GetServerStateFlag( void * pFlag ) ;
	void SetServerStateFlag( void * pFlag ) ;

#ifdef	_PD_CASTLE_STEP_2_
	int GetAllCombatRecord();
	int GetCombatRecordInManor();
	int InsertOrgStandings(int orgIndex,int againstOrgIndex);
	int UpdateOrgStandings_bi(int index);
	int UpdateOrgRecordInManor(int index);
#endif//_PD_CASTLE_STEP_2_

#ifdef _PD_GAME_EVENT_
	void GetFMWinnerInfo( CGameEventManager * pGE );
	void UpdateFMWinner( int rank, int nick_id, int cuid, char * name );
#endif

#ifdef _PD_LEVEL_PRESENT_
	short LP_GetNotRecv( char * acc );
	BOOL LP_Check( int auid, char * acc, short eid, short level, char * name );
	BOOL LP_Recv( char * acc, short eid, short level, char * name );
	VOID LP_Cancel( char * acc, short eid );
#endif

#ifdef _PD_PKEVENT_BOX_SPAWN_
	bool GetBWWarWinTime( int cuid, int& win_time );
	void UpdateBWWarWinTime( int cuid, int win_time );
#endif

	bool GetCountLimit( IN const int id, IN const short type, OUT int & count, OUT short & flag ) ;
	void SetCountLimit( IN const int id, IN const short type, IN const int count, IN const int flag ) ;

	bool QuestRank( IN const u_short questID, IN OUT _QUEST_RANK_DATA * questData, IN const short flag/*0:get, 1:set*/ ) ;

#ifdef _RESET_LEVEL
	void ResetLevelInfo( char cFirst = 0 ) ;
#endif

	bool CheckAndChangeName( char * curName, char * newName, int& result ) ;

#ifdef _PD_MASTER_PUPIL_
	bool MP_GetCharacMPInfo( int cuid, _C_CHARAC * pCharac );
	int MP_GetMasterFromPupil( int cuid );							// 성공한다면, cuid 의 마스터 인덱스 리턴.
	bool MP_ReadAndFillMasterInfo( CDSMaster * pMaster );		//
	bool MP_ReadMPInfo( CDSMaster * pMaster );
	bool MP_CreateMasterInfo( CDSMaster * pMaster );
	bool MP_UpdateMasterClass( CDSMaster * pMaster );
	bool MP_UpdateMasterNick( CDSMaster * pMaster );
	bool MP_UpdateMasterInfo( CDSMaster * pMaster );
	bool MP_AddNewPupil( _sCSPK_Pupil& temp, int master_index );
	bool MP_UpdatePupilPosition( int pupil, short pos );
	bool MP_UpdatePupilCurLevel( int pupil, short lv );
	bool MP_UpdatePupilClass( int pupil, short cls );
	bool MP_UpdatePupilClassGrade( int pupil, short class_grade );
	bool MP_UpdatePupilMoonpa( int pupil, short moonpa );
	bool MP_UpdatePupilGender( int pupil, short gender );
	bool MP_BackupPupil( int pupil, short reason );
#endif

	void GMS_ResetCharacOrg( int uid );

protected:
private:
};
