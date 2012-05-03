//=================================================================================
// Packet Process 스레드 
// 큐에 패킷이 있을 경우, 

#include "..\stdafx.h"
#include "..\MAPManager\MAPManager.h"
#include "..\Billing\BillingSvrLink.h"
#include "..\Ranking_Manager\Ranking_Manager.h"

#if defined(ITEMSHOP_PATCH_200607) || defined(_CIS_20070326_)
#define INCLUDE_ODBC_HEADER
#endif
#include "../MyLibrary/gGlobal.h"

#ifdef ITEMSHOP_PATCH_200607
#include "../ItemShop/ItemShop_Packet.h"
#endif

#ifdef _CIS_20070326_
#include "../ItemShop/CIS.h"
#endif

#ifdef _PD_CHARAC_LTS_
#include "../Global/Global_struct.h"
#endif

#ifdef _PD_RECALL_FRIEND_
#include "../cs/RecallFriend.h"
#endif

#ifdef _PD_GAME_EVENT_
#include "../GEvent/GameEvent.h"
extern CGameEventManager *		g_pGameEvent;
#endif

#ifdef _PD_COMPANY_JAPAN_
#include "../Japan RMS/RocAuthSession.h"
#endif

#include "../cs/MasterPupil.h"


extern _queue_Packet *		g_pPacketQ ;			// 패킷 큐
extern _queue_Packet *		g_pUpdatePacketQ ;			// 패킷 큐
extern _j_AccMAPManager *	g_pAccMapManager;		// Account ID KEY MAP
extern _j_ServerInfo		g_ServerInfo;			// 서버정보
extern _DSCom_Queue *		g_pDSComQueue ;

extern CRankingManager	*		g_pRankingManager ;


extern CDB_Connector	*	g_pDBCon ;
extern CDB_Connector *		g_pUSERDBCon ;
extern CDB_Connector *		g_pEventDBCon ;
#ifdef ITEMSHOP_PATCH_200607
extern CDB_Connector	*	g_pDBCon_WebShop;
#endif

extern CLogSvr_Com *			g_pLogSvrCom ;
extern char g_ForestName[13]  ;

// Backup 대기 객체
extern _j_BackupReady *		g_pBackupReady ;

extern bool g_Threading ;

extern u_int					g_serverStateFlag ;

extern CND_PacketLengthChecker * g_pPacketChecker ;

extern HANDLE					g_hInitEvent;	// Thread event handler

extern bool						g_bItemShopTestVersion;

#ifdef _PD_MASTER_PUPIL_
extern CDSMasterSet *			g_pMPSet;
#endif

#ifdef _METEL_LOGIN_200608_
std::map<tstring /*tstrUserAccount*/, CUser* /*pUser*/> g_mapBillingUser;
CRITICAL_SECTION g_csBillingUser;
#endif //_METEL_LOGIN_200608_


// 패킷 처리 함수들
void EnterGsFunc(CMSG_ENTERSERVER_CHECK_REQ_GS_DS * pMsgSvEnterGame, _h_DS_Com	* pDSComm,
				 CMSG_USER_CHARAC_LIST_DS_GS *msgUserCharacList,
				 COLE_DB_Executer * g_DB_Command, C_USER_DB_Command * g_USER_DB_Command);
void GSConnect(CMSG_GSCONNECT_GS_DS * pMsgGsconnect, _h_DS_Com	* pDSComm );
void CharacSelect(CMSG_CHARAC_SELECT_GS_DS * pMsgCharacSelect,  _h_DS_Com * pDSComm,  COLE_DB_Executer *g_DB_Command, CMSG_CHARAC_ALL_INFO_DS_GS *msgCharacAllInfo);
void CharacCreate(CMSG_CHARAC_CREATE_GS_DS * pMsgCharacCreate, _h_DS_Com * pDSComm,  COLE_DB_Executer * g_DB_Command, C_USER_DB_Command * g_UserDB_Command );
#ifdef _PD_COMPANY_JAPAN_
	void CharacDelete(CMSG_CHARAC_DELETE_GS_DS * pMsgCharacDelete, _h_DS_Com * pDSComm,  COLE_DB_Executer * g_DB_Command, C_USER_DB_Command * g_UserDB_Command, CRocAuthSession * pSession );
#else
	void CharacDelete(CMSG_CHARAC_DELETE_GS_DS * pMsgCharacDelete, _h_DS_Com * pDSComm,  COLE_DB_Executer * g_DB_Command, C_USER_DB_Command * g_UserDB_Command );
#endif
void CharacListReq(CMSG_CHARAC_LIST_REQ_GS_DS * pMsgChracListReq, _h_DS_Com * pDSComm,
				   CMSG_USER_CHARAC_LIST_DS_GS * msgUserCharacList );
void LogoutFunc(CMSG_LOGOUT_GS_DS * pMsgLogout, _h_DS_Com * pDSComm, C_USER_DB_Command * g_USER_DB_Command, COLE_DB_Executer * g_DB_Command);
void MoveServer(CMSG_REQ_SV_MOVE_GS_DS * pMsgNoSvMoveReq, _h_DS_Com * pDSComm,  COLE_DB_Executer * g_DB_Command, 
				CMSG_SV_MOVE_RESULT_DS_GS * msgSvMoveResult);
void ReturnLobby(CMSG_CHARAC_RETURN_LOBBY_GS_DS * pMsgCharacReturnLobby, _h_DS_Com * pDSComm,  COLE_DB_Executer * g_DB_Command);
//void DepotReq(CMSG_DEPOT_REQ_GS_DS * pMsgDepotReq, _h_DS_Com * pDSComm,  COLE_DB_Executer * g_DB_Command,
//			  CMSG_DEPOT_RES_DS_GS * msgDepotRes );

bool UpdateCharacAll( MSG_UPDATE_CHARAC_ALL * pMsgUpdateCharacAll ) ;
void ProcSerialEvent( CMSG_EVENT_AUTH_REQ_WITH_KEY * pSerialEvent, C_USER_DB_Command * pUserDB, _h_DS_Com * pDSCom );
void ProcEventCheck( CMSG_EVENT_CHECK_REQ * pMsgCheck, C_USER_DB_Command * pUserDB, _h_DS_Com * pDSCom );

#ifdef _PD_LEVEL_PRESENT_
void ProcLP_GetNotRecv( CMSG_LP_GETNOTRECV_REQ * pReq, COLE_DB_Executer * pGameDB, _h_DS_Com * pDSCom );
void ProcLP_Check( CMSG_LP_CHECK_REQ * pReq, COLE_DB_Executer * pGameDB, _h_DS_Com * pDSCom );
void ProcLP_Recv( CMSG_LP_RECV_REQ * pReq, COLE_DB_Executer * pGameDB, _h_DS_Com * pDSCom );
void ProcLP_Cancel( CMSG_LP_CANCEL * pCancel, COLE_DB_Executer * pGameDB );
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
void ProcIF_InfoReq( CMSG_FRIEND_INFO_REQ * pFriendInfoReq, C_USER_DB_Command * pUserDB, _h_DS_Com * pDSCom );
#endif

#ifdef _PD_CHARAC_LTS_

class LTS_UTIL
{
public:
	static u_char Insert_Lvpk();
	static u_char Delete_Lvpk();


	static u_char Insert_pk( CMSG_LTS_PK_INSERT_REQ_GS_DS * pIns, _h_DS_Com * pCom, COLE_DB_Executer * pDB );
	static u_char Delete_pk( CMSG_LTS_PK_DEL_REQ_GS_DS * pDel );

	static u_char Update_sp( CMSG_LTS_SP_INSERT_REQ_GS_DS * pIns, _h_DS_Com * pCom, COLE_DB_Executer * pDB );
	static u_char Reset_sp( CMSG_LTS_SP_DEL_REQ_GS_DS * pDel );
};

#endif




#ifdef _VINA_SHUTDOWN_20061023_
	BOOL SaveFatigueWeight(int iUid, short sFatigueWeight, C_USER_DB_Command* g_USER_DB_Command) ;
#endif

#ifdef	_NXPCB_AWARD_200703_
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Mylibrary/gGlobal.h"
#include "../SmallNet/Packet.h"
#include "../SmallNet/PktMgr.h"
#include "../SmallNet/PktQue.h"
#include "../SmallNet/Sock.h"
#include "../SmallNet/SmallNet.h"
#include "../SmallNet/PktProc/_PktDef.h"
#include "../SmallNet/PktProc/Nexon_PCBangAuth.h"

//extern CSock	gNexonPCB_sock;
//extern CS		gNexonPCB_cs;

extern std::map<string, CUser*>		g_mapNXAcc; // <넥슨계정, 유저객체>
extern CS							g_csNXAcc;
#endif

#ifdef _CIS_20070326_
// main.cpp 에서 콘솔 명령어로 테스트하기 위해 글로벌 변수 선언
COdbc*	g_pCIS = NULL;
#endif

DWORD WINAPI th_PacketProcess( LPVOID lp )
{

	// Create ole-db object	
	COLE_DB_Executer *	g_DB_Command = new COLE_DB_Executer(g_pDBCon) ;
	assert( g_DB_Command ) ;
	
	C_USER_DB_Command *	g_USER_DB_Command = new C_USER_DB_Command(g_pUSERDBCon);
	assert( g_USER_DB_Command ) ;

	C_USER_DB_Command * pEventDBCommand = new C_USER_DB_Command(g_pUSERDBCon) ;

	_packet *			pack = NULL ;	
	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct

	_h_DS_Com	*		pDSComm = NULL;	
	
	char				cIP[16]={0};				//IP		
	u_char				Mainmsg = 0;				// 메시지 대분류 구분 변수 8로 나눈 몫
	u_short				size;
	
	// 요청받는 메시지다..
	// Login 관련..		
	CMSG_ENTERSERVER_CHECK_REQ_GS_DS *		pMsgSvEnterGame = NULL;			// GS->DS 유저가 LS 에서 GS 로 처음 접속시도(인증필수)
	CMSG_GSCONNECT_GS_DS *					pMsgGsconnect = NULL;				// GS->DS 연결 알림
	// Charac Info Data 관련..
	CMSG_CHARAC_SELECT_GS_DS *				pMsgCharacSelect = NULL;			// GS->DS 캐릭터 창에서 캐릭터를 선택했을경우..
	CMSG_CHARAC_CREATE_GS_DS *				pMsgCharacCreate = NULL;			// GS->DS 캐릭터 생성 정보
	CMSG_CHARAC_DELETE_GS_DS *				pMsgCharacDelete = NULL;			// GS->DS 삭제할 캐릭터 정보

	CMSG_CHARAC_LIST_REQ_GS_DS *			pMsgChracListReq = NULL;
	CMSG_LOGOUT_GS_DS *						pMsgLogout  = NULL;				// LS->DS Logout 요청 (응답은 없음)
	CMSG_REQ_SV_MOVE_GS_DS *				pMsgNoSvMoveReq = NULL;			// GS->DS 존 이동 요청
	CMSG_DEPOT_REQ_GS_DS	*				pMsgDepotReq = NULL;
	CMSG_CHARAC_RETURN_LOBBY_GS_DS	*		pMsgCharacReturnLobby = NULL;		// GS->DS 캐릭터가 존에서 로비로 돌아온다..
	CMSG_EVENT_AUTH_REQ *					pMsgEventAuth = NULL;
	CMSG_EVENT_AUTH_REQ_WITH_KEY *			pMsgEventWithKey = NULL;
#ifdef _PD_LEVEL_PRESENT_
	CMSG_LP_GETNOTRECV_REQ *				pLpGetNotRecv = NULL;
	CMSG_LP_CHECK_REQ *						pLpCheckReq = NULL;
	CMSG_LP_RECV_REQ *						pLpRecvReq = NULL;
	CMSG_LP_CANCEL *						pLpCancel = NULL;
#endif
	CMSG_EVENT_CANCEL *						pMsgEventCancel = NULL;
	CMSG_EVENT_SUCCESS *					pMsgEventSuccess = NULL;

	CMSG_DISCONNECT *						pMsgDisconnect = NULL;

#ifdef _PD_COMPANY_JAPAN_
	CRocAuthSession		rocSession;
#endif
	

	// 응답 메시지들..
	// Charac Info Data 관련..
//	CMSG_LOGIN_DS_LS_RESULT					msgLoginResult = {0} ;
	CMSG_USER_CHARAC_LIST_DS_GS				msgUserCharacList = {0};		// DS->GS 게임서버에 로그인 요청한 캐릭터의 리스트 반환	
	CMSG_CHARAC_ALL_INFO_DS_GS				msgCharacAllInfo = {0};			// DS->GS MSG_CHARAC_SELECT 가 성공했을경우 GS 에 보냄
	CMSG_SV_MOVE_RESULT_DS_GS				msgSvMoveResult = {0};

	// 구조체 초기화
	ZeroMemory(&msgUserCharacList, sizeof(CMSG_USER_CHARAC_LIST_DS_GS));
	msgUserCharacList.usLength = sizeof(CMSG_USER_CHARAC_LIST_DS_GS);
	msgUserCharacList.cMessage = CMSG_NO_USER_CHARAC_LIST_DS_GS;	

	// 선택된 캐릭터의 정보
	ZeroMemory(&msgCharacAllInfo, sizeof(CMSG_CHARAC_ALL_INFO_DS_GS));
	msgCharacAllInfo.usLength = sizeof(CMSG_CHARAC_ALL_INFO_DS_GS);
	msgCharacAllInfo.cMessage = CMSG_NO_CHARAC_ALL_INFO_DS_GS;	

	ZeroMemory(&msgSvMoveResult, sizeof(CMSG_SV_MOVE_RESULT_DS_GS));
	msgSvMoveResult.usLength = sizeof(CMSG_SV_MOVE_RESULT_DS_GS);
	msgSvMoveResult.cMessage = CMSG_NO_SV_MOVE_RESULT_DS_GS;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;

#ifdef ITEMSHOP_PATCH_200607
	// Item Shop(Nexon)
	COdbc*	pDB_Account = NULL;
	COdbc*	pDB_NDGame = NULL;
	COdbc*	pDB_ItemShop = NULL;
	BOOL	bRet = FALSE;

	// Item Shop DB
	pDB_ItemShop = new COdbc;
	if( false == g_bItemShopTestVersion )
		bRet = pDB_ItemShop->Connect(_T("ItemShop"), _T("shop_gw_user_nine"), _T("shop_gw_user_nine_~1@3"));
	else
		bRet = pDB_ItemShop->Connect(_T("ItemShopTest"), _T("shop_gw_user_nine"), _T("shop_gw_user_nine_password"));

	if(bRet == FALSE) {
		char pszLog[256] = {0};
		sprintf(pszLog, "%s(%d) 아이템 샵 디비(%s) 연결 실패", __FILE__, __LINE__, g_bItemShopTestVersion ? "ItemShopTest" : "ItemShop" );
		printf("%s \n", pszLog);
		g_pSystemLog->LogFileWrite("%s \n", pszLog);
		//goto labelPacketProcess;
		delete pDB_ItemShop;
		pDB_ItemShop = NULL ;
	}
	else {
		Logout(_T("pDB_ItemShop->Connect 성공"));
	}
#endif

#ifdef _CIS_20070326_

	COdbc*	pCIS = NULL;
#ifndef _PD_IGNORE_CIS_
	{
		BOOL	bRet = FALSE;

		pCIS = new COdbc;
		assert(pCIS);
		if(g_pCIS == NULL) g_pCIS = pCIS;

		TCHAR	szDir[MAX_PATH] = {0};
		TCHAR	szConStr[MAX_PATH] = {0};
		::GetCurrentDirectory(MAX_PATH, szDir);
		_stprintf(szConStr, "FILEDSN=%s\\Env\\CashItemShop.dsn", szDir);
		bRet = pCIS->Connect(NULL, szConStr);
		if(bRet == TRUE) {
			Logout("Success to connect to CIS DB.");
			printf("Success to connect to CIS DB. \n");
		}
		else {
			Logout("Fail to connect to CIS DB.");
			printf("Fail to connect to CIS DB. \n");
			SAFE_DEL(pCIS);

			std::cout << "Do you wanna continue or stop DS? \n";
			std::cout << "Press 'C' to continue, and press any key to stop. \n";
			char ch = 0;
			std::cin.get(ch);
			if(ch == 'c' || ch == 'C');
			else {
				pCIS->Disconnect();
				exit(-1);
				return 0;
			}
		}
	}
#endif
#endif

	SetEvent( g_hInitEvent );
	
	while( g_Threading ) {

		if( g_pPacketQ->empty() ) {
			WaitForSingleObject( hTimer, INFINITE ) ;			
			continue ;
		}
		else {

			try{
				pack = g_pPacketQ->get() ;								// 패킷을 얻고				
				if(pack == NULL) {
					WaitForSingleObject( hTimer, INFINITE ) ;					
					continue;
				}

				//pUser = NULL ;
				//pCharac = NULL ;
				pDSComm = pack->pDSComm;
				if( pDSComm == NULL )
				{
					WaitForSingleObject( hTimer, INFINITE ) ;					
					continue;
				}


				CopyMemory( &size, pack->buf, 2 );
				pDSComm->set_last_msg( static_cast<u_char>(pack->buf[2]) );
				pDSComm->set_last_size( size );

				// 일처리.
				//////////////////////////////////////////////////////////////////////////
				//Logout("recv: MSG_NO=%d", pack->buf[2]);
				//////////////////////////////////////////////////////////////////////////
				if( static_cast<u_char>(pack->buf[2]) == CMSG_NO_DISCONNECT){
					memset(cIP, 0, 16);
					ip_char(pack->pDSComm->m_RecvOV.m_gs_info.ipAddr, cIP);
					::PrintConsole("[SO_ERR] Disconnected to GS. IP:%s \n",cIP);									
				}
				else{
					Mainmsg = static_cast<u_char>(pack->buf[2]) >> 3 ;	// 8로 나눈 몫
				}
		
			}
			catch (...) {
				::PrintConsole("[EXCEPTION] g_pPacketQ->get()\n ");
				continue;
			}		
			
		}	
			

		int pos = 0 ;
		try{
			/* ===================================================
			0: 여기서부터 메시지 번호별로...패킷처리...			
			======================================================*/
			if(Mainmsg == 0)
			{
				pos = 1 ;

				/* GS 진입 */
				if( pack->buf[2] == CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS ) {
#ifdef	_NXPCB_AWARD_200703_
					/*
					// NEXONPCB_hThreadSync -> 서버 종료할 때, CloseHandle() 호출하지 않음 -_-;;
					static HANDLE	NEXONPCB_hThreadSync = NULL;
					if(NEXONPCB_hThreadSync == NULL) {
						NEXONPCB_hThreadSync = CreateEvent(NULL, FALSE, TRUE, NULL);
					}
					*/
#endif

					pMsgSvEnterGame = (CMSG_ENTERSERVER_CHECK_REQ_GS_DS *)pack->buf;
					EnterGsFunc(pMsgSvEnterGame, pDSComm, &msgUserCharacList, g_DB_Command, g_USER_DB_Command);


#ifdef _METEL_LOGIN_200608_
					if(g_ServerInfo.get_billing_use()) {
						CUser* pUser = g_pAccMapManager->Find_User(pMsgSvEnterGame->cAccount);
						if(pUser != NULL){

							if(pUser->bBillingLogin == FALSE) {
								pUser->bBillingLogin = TRUE;

								char ip[IPV4_BUF_SIZE] = {0};
								ip_char(pUser->iIp, ip);
#ifdef _METEL_LOGIN_NOT_USE_CALL_

#else
								int iRet = BillingLogin(pUser->cNDAccount, pUser->cPassword, ip, -1);
								Logout(_T("%d = BillingLogin(%s, %s, %s)"), iRet, pUser->cNDAccount, pUser->cPassword, ip);
								if(iRet < 0) {
									Logout("Fail to BillingLogin() : A<%s>", pUser->cNDAccount );

									// ... 빌링 실패 처리
									pUser->bBillingLogin = FALSE;
								}
								else {
									pUser->MetelBillingInfo.iLoginSN = iRet;

									tstring tstr = pUser->cNDAccount;
									EnterCriticalSection(&g_csBillingUser);
									g_mapBillingUser[tstr] = pUser;
									LeaveCriticalSection(&g_csBillingUser);
								}
#endif
							}

						}
					}
#endif //_METEL_LOGIN_200608_
				}//else if																																																																																																											

				/* GS 접속 초기 정보 */
				else if( pack->buf[2] == CMSG_NO_GSCONNECT_GS_DS){
					pos = 2 ;
					pMsgGsconnect = (CMSG_GSCONNECT_GS_DS *)pack->buf;
					GSConnect(pMsgGsconnect, pDSComm );
				}
			} //if(Mainmsg == 0)


			/* ===================================================
			1: 여기서부터 메시지 번호별로...패킷처리...			
			======================================================*/
			else if(Mainmsg == 1){		//8~15

				switch( pack->buf[2] ) {
				/* 캐릭터 창에서 캐릭터를 선택 */
				case CMSG_NO_CHARAC_SELECT_GS_DS:
					{// PR:MSG_CHARAC_SELECT			
					pos = 3 ;
					ZeroMemory(&msgCharacAllInfo, sizeof(CMSG_CHARAC_ALL_INFO_DS_GS));
					msgCharacAllInfo.usLength = sizeof(CMSG_CHARAC_ALL_INFO_DS_GS);
					msgCharacAllInfo.cMessage = CMSG_NO_CHARAC_ALL_INFO_DS_GS;	

					pMsgCharacSelect = (CMSG_CHARAC_SELECT_GS_DS *)pack->buf;
					CharacSelect(pMsgCharacSelect, pDSComm, g_DB_Command, &msgCharacAllInfo);

					} // MSG_NO_CHARAC_SELECT_GS_DS
					break;
				/* 캐릭터 생성 */
				case CMSG_NO_CHARAC_CREATE_GS_DS:
					{// PR:MSG_CHARAC_CREATE
					pos = 4 ;
					pMsgCharacCreate = (CMSG_CHARAC_CREATE_GS_DS *)pack->buf;
					CharacCreate(pMsgCharacCreate, pDSComm, g_DB_Command, g_USER_DB_Command );
					} // MSG_NO_CHARAC_CREATE_GS_DS
					break;					
				case CMSG_NO_CHARAC_DELETE_GS_DS:
					{// PR:MSG_CHARAC_DELETE
					pos = 5 ;
					pMsgCharacDelete = (CMSG_CHARAC_DELETE_GS_DS *)pack->buf;
#ifdef _PD_COMPANY_JAPAN_
					CharacDelete(pMsgCharacDelete, pDSComm, g_DB_Command, g_USER_DB_Command, &rocSession );
#else
					CharacDelete(pMsgCharacDelete, pDSComm, g_DB_Command, g_USER_DB_Command );
#endif
					} // MSG_NO_CHARAC_DELETE
					break;
				/* 캐릭터 리스트 요청 */
				case CMSG_NO_CHARAC_LIST_REQ_GS_DS:
					{
					pos = 6 ;
					pMsgChracListReq = (CMSG_CHARAC_LIST_REQ_GS_DS *)pack->buf;
					CharacListReq(pMsgChracListReq, pDSComm, &msgUserCharacList);
					}
					break;				
				}				
			} //else if(Mainmsg == 1)			

			/* ===================================================
			2: 여기서부터 메시지 번호별로...패킷처리...			
			======================================================*/
			else if(Mainmsg == 2)
			{		//16~23
				
				/* 여기부터 로그아웃 처리다.. */			
				if( pack->buf[2] == CMSG_NO_LOGOUT_GS_DS ) 
				{			
					pos = 7 ;
					pMsgLogout = (CMSG_LOGOUT_GS_DS *)pack->buf;					

#ifdef _METEL_LOGIN_200608_
#ifdef _METEL_LOGIN_NOT_USE_CALL_
#else
					if(g_ServerInfo.get_billing_use()) {
						tstring tstr = pMsgLogout->cAccount;
						EnterCriticalSection(&g_csBillingUser);
						CUser* pUser = g_mapBillingUser[tstr];
						LeaveCriticalSection(&g_csBillingUser);
						if(pUser != NULL) {
							BillingLogout(pUser->cNDAccount, pUser->MetelBillingInfo.iLoginSN, -1);
							Logout(_T("BillingLogout(%s, %d), CMSG_NO_LOGOUT_GS_DS"), pUser->cNDAccount, pUser->MetelBillingInfo.iLoginSN);
							pUser->bBillingLogin = FALSE;
							EnterCriticalSection(&g_csBillingUser);
							g_mapBillingUser.erase(tstr);
							LeaveCriticalSection(&g_csBillingUser);
						}
					}
#endif
#endif //_METEL_LOGIN_200608_

					PUSER pUser = g_pAccMapManager->Find_User(pMsgLogout->cAccount);

					LogoutFunc(pMsgLogout, pDSComm, g_USER_DB_Command, g_DB_Command );

#ifdef	_NXPCB_AWARD_200703_
					std::string		strAcc(pUser->cNXAcc);
					g_csNXAcc.Lock();
					g_mapNXAcc.erase(strAcc);
					g_csNXAcc.Unlock();

					//gNexonPCB_cs.Lock();
					NEXONPCB_LogoutNtf(pUser->cNXAcc, "", (DWORD)pUser->iIp);
					//gNexonPCB_cs.Unlock();
#endif
				} // MSG_NO_LOGOUT_LS_DS  

				
				/* GS->GS 서버 이동	  */
				else if( pack->buf[2] == CMSG_NO_REQ_SV_MOVE_GS_DS )
				{
					pos = 8 ;
					pMsgNoSvMoveReq = (CMSG_REQ_SV_MOVE_GS_DS *)pack->buf;
#ifdef	_NXPCB_AWARD_200703_
					pUser = g_pAccMapManager->Find_User(pMsgNoSvMoveReq->cAccount) ;
					//gNexonPCB_cs.Lock();
					NEXONPCB_LogoutNtf(pUser->cNXAcc, "", (DWORD)pUser->iIp);
					//gNexonPCB_cs.Unlock();
#endif
					MoveServer(pMsgNoSvMoveReq, pDSComm, g_DB_Command, &msgSvMoveResult);
				} // CMSG_NO_REQ_SV_MOVE_GS_DS
				else if( pack->buf[2] == CMSG_NO_UNI_GROUP_LOBBY )
				{
					CMSG_UNI_GROUP_LOBBY * pLobby = reinterpret_cast<CMSG_UNI_GROUP_LOBBY*>( pack->buf ) ;
					
					if( en_ug_change_name_request == pLobby->ucMode )
					{
						// 이름 바꾸기 요청.
						pUser = g_pAccMapManager->Find_User( pLobby->cNDAccount ) ;
						if( pUser == NULL )
						{
							// send error
							continue ;
						}

						CMSG_UG_CHANGE_NAME_REQUEST * pReq = static_cast<CMSG_UG_CHANGE_NAME_REQUEST*>( pLobby ) ;
                        
						if( pReq->ucIndex >= 3 )
						{
							// send error
							CMSG_UG_CHANGE_NAME_RESULT pkResult ;
							pkResult.SetAccount( pUser->cNDAccount ) ;

							pkResult.ucSuccess = 2 ;
							pkResult.ucIndex = pReq->ucIndex ;
							CopyMemory( pkResult.ucNewName, pReq->ucNewName, en_charac_name_length+1 ) ;

							pDSComm->Send( pkResult.usLength, (char*)&pkResult ) ;
							continue ;
						}

						int result = 0 ;

						if( g_DB_Command->CheckAndChangeName( pUser->stCharac[pReq->ucIndex].CHARAC_BASIC.cChrName, (char*)pReq->ucNewName, result ) )
						{
							if( result == 0 )
							{
								g_pAccMapManager->CharacRename( pUser->stCharac[pReq->ucIndex].CHARAC_BASIC.cChrName, (char*)pReq->ucNewName ) ;

								// 신서버군에서만 있을 것이다. 따라거, 12 -1 => 1 로.
								g_pLogSvrCom->LogWrite_ChangeName( pUser->stCharac[pReq->ucIndex].CHARAC_BASIC.cChrName, (char*)pReq->ucNewName, g_ForestNum-11 ) ;
								g_pSystemLog->LogFileWrite( "[UG] Change Name A<%s> : C<%s> => C<%s>\n", pUser->characFind, 
									pUser->stCharac[pReq->ucIndex].CHARAC_BASIC.cChrName, (char*)pReq->ucNewName ) ;
								
								CopyMemory( pUser->stCharac[pReq->ucIndex].CHARAC_BASIC.cChrName, pReq->ucNewName, en_charac_name_length+1 ) ;
								pUser->stCharac[pReq->ucIndex].CHARAC_BASIC.ucChangeName = 0 ;


								CMSG_UG_CHANGE_NAME_RESULT pkResult ;
								pkResult.SetAccount( pUser->cNDAccount ) ;
								
								pkResult.ucSuccess = 0 ;
								pkResult.ucIndex = pReq->ucIndex ;
								CopyMemory( pkResult.ucNewName, pReq->ucNewName, en_charac_name_length+1 ) ;

								pDSComm->Send( pkResult.usLength, (char*)&pkResult ) ;

								PrintConsole( "[CHANGE NAME] A<%s> Success\n", pUser->cNDAccount ) ;
							} 
							else
							{
								CMSG_UG_CHANGE_NAME_RESULT pkResult ;
								pkResult.SetAccount( pUser->cNDAccount ) ;

								pkResult.ucSuccess = static_cast<u_char>(result) ;
								pkResult.ucIndex = pReq->ucIndex ;
								CopyMemory( pkResult.ucNewName, pReq->ucNewName, en_charac_name_length+1 ) ;

								pDSComm->Send( pkResult.usLength, (char*)&pkResult ) ;

								PrintConsole( "[CHANGE NAME] A<%s> Fail : %u\n", pUser->cNDAccount, pkResult.ucSuccess ) ;
							}
						}
						else
						{
							// 
							CMSG_UG_CHANGE_NAME_RESULT pkResult ;
							pkResult.SetAccount( pUser->cNDAccount ) ;

							pkResult.ucSuccess = 4 ;
							pkResult.ucIndex = pReq->ucIndex ;
							CopyMemory( pkResult.ucNewName, pReq->ucNewName, en_charac_name_length+1 ) ;

							pDSComm->Send( pkResult.usLength, (char*)&pkResult ) ;

							PrintConsole( "[CHANGE NAME] A<%s> Fail : %u\n", pUser->cNDAccount, pkResult.ucSuccess ) ;
						}
					}
				}
				else if( pack->buf[2] == CMSG_NO_CHECK_DSGS_PACKET )
				{
					CMSG_CHECK_DSGS_PACKET * pCheck = reinterpret_cast<CMSG_CHECK_DSGS_PACKET*>(pack->buf);

					//PrintConsole( "[DS CHECK] Recv from GS : Check DSGS Packet\n" );

					switch( pCheck->ucMode )
					{
					case en_check_packet_test_request:
						printf( "\n Recv test request\n" );
						if( g_pPacketChecker->check_packet( pCheck ) )
							pCheck->ucMode = en_check_packet_test_success;
						else
							pCheck->ucMode = en_check_packet_test_fail;

						pDSComm->Send( pCheck->usLength, pack->buf ) ;
						break;

					case en_check_packet_test_success:
					case en_check_packet_test_fail:
						// 결과 보여주기.
						g_pPacketChecker->show_result( pCheck ) ;
						break;

					case en_check_packet_timeout_request:
						pCheck->ucMode = en_check_packet_timeout_response;
						pDSComm->Send( pCheck->usLength, pack->buf );
						break;
					case en_check_packet_timeout_response:
						g_ServerInfo.RecvTimeoutResponse( pCheck->check_pair[0].msg );
						break;
					}
				}
#ifdef _PD_ACCOUNT_PLAY_TIME_
				else if( pack->buf[2] == CMSG_NO_ACCOUNT_INFO )
				{
					CMSG_ACCOUNT_INFO * pAInfo = reinterpret_cast<CMSG_ACCOUNT_INFO*>(pack->buf);

					pUser = g_pAccMapManager->Find_User( pAInfo->cNDAccount ) ;

					if( !pUser )
					{
						PrintConsole( "[PLAY TIME] No Account!! A<%s>\n", pAInfo->cNDAccount );
						continue;
					}

					switch( pAInfo->ucMode )
					{
					case en_cai_add_play_time:
						{
							CMSG_AI_ADD_PLAY_TIME * pTime = static_cast<CMSG_AI_ADD_PLAY_TIME*>(pAInfo);

							g_USER_DB_Command->AddPlayTime( pUser->iUid, pUser->cNDAccount, pTime->iPlayTime, g_ForestNum );

							g_pTrace->OutputString( _TRACE_CLASS_UPDATE, "[PLAY TIME] A<%s> Add %d Second.\n", pUser->cNDAccount, pTime->iPlayTime );
						}
						break;
					}

				}
#endif
			} //else if(Mainmsg == 2)



			
			/* ===================================================
			3: 여기서부터 메시지 번호별로...패킷처리...			
			======================================================*/
			else if(Mainmsg == 3){		//24~31
				
			} //else if(Mainmsg == 3)

			
			else if(Mainmsg == 4){		//32~39
				/* 존에서 로비로.. 게임 종료하고 로비로 복귀 할때.. */
				if( pack->buf[2] == CMSG_NO_CHARAC_RETURN_LOBBY_GS_DS ) 
				{
					pos = 9 ;
					pMsgCharacReturnLobby = reinterpret_cast<CMSG_CHARAC_RETURN_LOBBY_GS_DS*>(pack->buf);
					ReturnLobby(pMsgCharacReturnLobby, pDSComm, g_DB_Command);
				}// CMSG_NO_CHARAC_RETURN_LOBBY_GS_DS

			} //else if(Mainmsg == 4)

			else if(Mainmsg == 5){		//40~47
#ifdef _VINA_SHUTDOWN_20061023_
				if( pack->buf[2] == CMSG_NO_SAVE_FATIGUE_WEIGHT ) 
				{
					//pUser = g_pAccMapManager->Find_User(pMsgLogout->cAccount);
					//::PrintConsole("pack=%p \n", pack);
					MSG_SAVE_FATIGUE_WEIGHT* pMsg = (MSG_SAVE_FATIGUE_WEIGHT*)pack->buf;
					
					pUser = g_pAccMapManager->Find_User(pMsg->cAccount) ;
					if( pUser )
					{
						pUser->FatigueElement.sFatigueWeight = pMsg->sFatigueWeight;
#ifdef _PD_SDS_GOSTOP_
						pUser->FatigueElement.ucFatigueMode = pMsg->ucFatigueMode;
#endif
						::PrintConsole("[RECV] MSG_SAVE_FATIGUE_WEIGHT account = %s uid = %d, fatigue_weight = %d \n", pUser->cNDAccount, pUser->iUid , pMsg->sFatigueWeight );
						try {
							if( FALSE == SaveFatigueWeight(pUser->iUid, pMsg->sFatigueWeight, g_USER_DB_Command) )
							{
								::PrintConsole("[ERROR] MSG_SAVE_FATIGUE_WEIGHT \n");
							}
							else {
								SYSTEMTIME	SaveTime ;
								GetLocalTime(&SaveTime) ;
								Logout("SaveTime=%d/%d/%d/%d sFatigueWeight=%d",
									SaveTime.wYear,
									SaveTime.wMonth,
									SaveTime.wDay,
									SaveTime.wHour,
									pMsg->sFatigueWeight);
							}
						}
						catch(...) {
							::PrintConsole("catch(SaveFatigueWeight) \n");
						}
					}
				}
#endif
			} //else if(Mainmsg == 5)

			else if(Mainmsg == 6){		//48~55
			} //else if(Mainmsg == 6)

			else if(Mainmsg == 7){		//56~63
			} //else if(Mainmsg == 7)

			else if(Mainmsg == 8){		//63~71
				switch(pack->buf[2]) 
				{
					pos = 24 ;

				case CMSG_NO_RANKING_INFO_REQ :
					{
						CMSG_RANKING_INFO_REQ * pCmsgRankingInfoReq = (CMSG_RANKING_INFO_REQ*)pack->buf ;
						CRankInfo * pRankInfo = g_pRankingManager->GetRankInfoPtr( pCmsgRankingInfoReq->sType )	;
						if( pRankInfo ) 
						{
							CMSG_RANKING_INFO_RES msgRankingInfoRes = {0} ;
							
							msgRankingInfoRes.cMessage = CMSG_NO_RANKING_INFO_RES ;
							msgRankingInfoRes.sType = pCmsgRankingInfoReq->sType ;
							msgRankingInfoRes.sRankCount = pRankInfo->GetMaxCount() ;
							pRankInfo->GetRankInfo( msgRankingInfoRes.rank, msgRankingInfoRes.sRankCount ) ;

							msgRankingInfoRes.usLength = 3/*header*/ + 2 + (sizeof(_Rank) * msgRankingInfoRes.sRankCount) ;

							if( msgRankingInfoRes.usLength <= sizeof(CMSG_RANKING_INFO_RES) )
							{
								if( pDSComm )
								{									
									if( pDSComm->Send( msgRankingInfoRes.usLength, (char *)&msgRankingInfoRes ) == false ){
										::PrintConsole("[SO_ERR] CMSG_NO_RANKING_INFO_RES \n");
									}
								}
							}							
						}

					}
					break;

				case CMSG_NO_RANKING_UPDATE :
					{
						CMSG_RANKING_UPDATE * pCmsgRankingUpdate = (CMSG_RANKING_UPDATE*)pack->buf ;
						if( pCmsgRankingUpdate->usLength != sizeof(CMSG_RANKING_UPDATE ) )
						{						
							continue ;
						}

					}
					break;



				case CMSG_NO_SHIFTSTATEFLAG :
					{
						MSG_SHIFTSTATEFLAG * pMsgShiftStateFlag = (MSG_SHIFTSTATEFLAG*)pack->buf ;
						g_serverStateFlag = pMsgShiftStateFlag->uiStateFlag ;

						g_DB_Command->SetServerStateFlag( &g_serverStateFlag ) ;

						if( g_pDSComQueue->GlobalSend( pMsgShiftStateFlag->usLength, (char*)pMsgShiftStateFlag ) == false ) 
						{
							::PrintConsole( "[SEND_ERROR] CMSG_NO_SHIFTSTATEFLAG \n" ) ;
						}
					}
					break ;

				case CMSG_NO_QUESTCOUNT_REQ :
					{
						MSG_QUESTCOUNT_REQ * pMsgQuestCount = (MSG_QUESTCOUNT_REQ*)pack->buf ;
						// TODO	: 
						int count = 0 ;
						short flag = 0 ;

						if( pMsgQuestCount->ucReqType == 0 
							&& g_DB_Command->GetCountLimit( static_cast<int>(pMsgQuestCount->usQuestID), pMsgQuestCount->ucQuestType, count, flag ) )
						{
							MSG_QUESTCOUNT_RES msgQuestCountRes = {0} ;
							msgQuestCountRes.usLength = sizeof( MSG_QUESTCOUNT_RES ) ;
							msgQuestCountRes.cMessage = CMSG_NO_QUESTCOUNT_RES ;
							msgQuestCountRes.usQuestID = pMsgQuestCount->usQuestID ;
							msgQuestCountRes.ucQuestType = pMsgQuestCount->ucQuestType ;
							msgQuestCountRes.iCount = count ;							
							
							if( pDSComm->Send( msgQuestCountRes.usLength, (char*)&msgQuestCountRes ) == false )
							{
								::PrintConsole("[SEND_ERROR] %s, %d \n", __FILE__, __LINE__ ) ;
							}

						}
						else if( pMsgQuestCount->ucReqType == 1 )
						{
							g_DB_Command->SetCountLimit( static_cast<int>(pMsgQuestCount->usQuestID), pMsgQuestCount->ucQuestType, pMsgQuestCount->iCount, 0 ) ;
						}
					}
					break ;

				case CMSG_NO_QUESTRANK_DS_REQ :
					{
						MSG_QUESTRANK_DS_REQ * pMsgQuestRankReq = (MSG_QUESTRANK_DS_REQ*)pack->buf ;
						MSG_QUESTRANK_DS_RES msgQuestRankRes = {0} ;
						msgQuestRankRes.usLength = sizeof(MSG_QUESTRANK_DS_RES) ;
						msgQuestRankRes.cMessage = CMSG_NO_QUESTRANK_DS_REQ ;
						msgQuestRankRes.usQuestID = pMsgQuestRankReq->usQuestID ;

						if( g_DB_Command->QuestRank( msgQuestRankRes.usQuestID, &msgQuestRankRes.Data, 0 ) == false )
						{
							::PrintConsole( "[ERROR] Quest rank data load error (quest id : %d) \n", msgQuestRankRes.usQuestID ) ;
						}

						if( pDSComm->Send( msgQuestRankRes.usLength, (char*)&msgQuestRankRes ) == false )
						{
							::PrintConsole("[SEND_ERROR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
					}
					break ;
				case CMSG_NO_QUESTRANK_DS_UPDATE :
					{
						MSG_QUESTRANK_DS_UPDATE * pMsgQuestUpdate = (MSG_QUESTRANK_DS_UPDATE*)pack->buf ;

						if( g_DB_Command->QuestRank( pMsgQuestUpdate->usQuestID, &pMsgQuestUpdate->Data, 1 ) == false )
						{
							::PrintConsole( "[ERROR] Quest rank data update error (quest id : %d) \n", pMsgQuestUpdate->usQuestID ) ;
						}

					}
					break ;
				} // switch				
			} //else if(Mainmsg == 8)


			else if(Mainmsg == 9){		//72~79
			} //else if(Mainmsg == 9)

			else if(Mainmsg == 10){		//80~87
			} //else if(Mainmsg == 10)

			else if(Mainmsg == 11){		//88~95
				// Event.
				switch( pack->buf[2] )
				{
				case CMSG_NO_EVENT_AUTH_REQ:
					pMsgEventAuth = reinterpret_cast<CMSG_EVENT_AUTH_REQ*>( pack->buf );
					switch( pMsgEventAuth->ucMode )
					{
					case en_event_auth_req_with_key:
						pMsgEventWithKey = static_cast<CMSG_EVENT_AUTH_REQ_WITH_KEY*>(pMsgEventAuth);

						PrintAll( "[S-EVENT] A<%s> Recv Event ID = %u\n", pMsgEventWithKey->cNDAccount, pMsgEventWithKey->sEventID );

						ProcSerialEvent( pMsgEventWithKey, g_USER_DB_Command, pDSComm );

						break;

					case en_event_auth_cancel:
						pMsgEventCancel = static_cast<CMSG_EVENT_CANCEL*>(pMsgEventAuth);

						PrintAll( "[S-EVENT] Cancel Event ID = %u\n", pMsgEventCancel->sEventID );

						g_USER_DB_Command->DSEventCancel( pMsgEventCancel->sEventID, pMsgEventCancel->index );

						break;						
					case en_event_auth_success:
						pMsgEventSuccess = static_cast<CMSG_EVENT_SUCCESS*>(pMsgEventAuth);

						PrintAll( "[S-EVENT] Success Event ID = %u\n", pMsgEventSuccess->sEventID );

						g_USER_DB_Command->DSEventSuccess( pMsgEventSuccess->sEventID, pMsgEventSuccess->index );

						break;
					case en_event_auth_check_req:
						ProcEventCheck( static_cast<CMSG_EVENT_CHECK_REQ *>(pMsgEventAuth), g_USER_DB_Command, pDSComm );
						break;

#ifdef _PD_LEVEL_PRESENT_
					case en_event_auth_lp_getnotrecv_req:
						pLpGetNotRecv = static_cast<CMSG_LP_GETNOTRECV_REQ*>(pMsgEventAuth);
						ProcLP_GetNotRecv( pLpGetNotRecv, g_DB_Command, pDSComm );
						break;
					case en_event_auth_lp_check_req:
						pLpCheckReq = static_cast<CMSG_LP_CHECK_REQ *>(pMsgEventAuth);
						ProcLP_Check( pLpCheckReq, g_DB_Command, pDSComm );
						break;
					case en_event_auth_lp_recv_req:
						pLpRecvReq = static_cast<CMSG_LP_RECV_REQ *>(pMsgEventAuth);
						ProcLP_Recv( pLpRecvReq, g_DB_Command, pDSComm );
						break;
					case en_event_auth_lp_cancel:
						pLpCancel = static_cast<CMSG_LP_CANCEL *>(pMsgEventAuth);
						ProcLP_Cancel( pLpCancel, g_DB_Command );
						break;
#endif	// _PD_LEVEL_PRESENT_
#ifdef _PD_EVENT_INVITE_FRIEND_
					case en_event_auth_friend_info_req:
						ProcIF_InfoReq( static_cast<CMSG_FRIEND_INFO_REQ*>(pMsgEventAuth), g_USER_DB_Command, pDSComm );
						break;

					case en_event_auth_friend_hyperrun_end:
						g_USER_DB_Command->IF_EndUse( static_cast<CMSG_FRIEND_HYPERRUN_END*>(pMsgEventAuth)->index );
						break;
#endif	//_PD_EVENT_INVITE_FRIEND_
					}

					g_pSystemLog->Reflush();
					break;
#ifdef _PD_GAME_EVENT_
				case CMSG_NO_FREE_MATCH:
					{
						CMSG_FREE_MATCH * pMatch = reinterpret_cast<CMSG_FREE_MATCH*>( pack->buf );

						g_pGameEvent->ProcGSPacket( pMatch, pDSComm );
					}
					break;
#endif
					
				}
			} //else if(Mainmsg == 11)

			else if(Mainmsg == 12){		//96~103
				switch(pack->buf[2]) 
				{
				case CMSG_NO_GSINFO_RES_GS_DS:
					{

						pos = 22 ;

						CMSG_GSINFO_RES_GS_DS * pMsgGsinfoRes = (CMSG_GSINFO_RES_GS_DS *)pack->buf;

						g_ServerInfo.UpdateConnectCount( pMsgGsinfoRes->cRealSN, pMsgGsinfoRes->_ServerStatInfo.iConnectCount ) ;
						g_ServerInfo.UpdatePlayCount( pMsgGsinfoRes->cRealSN, pMsgGsinfoRes->_ServerStatInfo.iPlayingCount ) ;
						g_ServerInfo.m_ServerStateInfo[pMsgGsinfoRes->cRealSN].iLevelExp = pMsgGsinfoRes->_ServerStatInfo.iLevelExp ;
						g_ServerInfo.m_ServerStateInfo[pMsgGsinfoRes->cRealSN].iSkillExp = pMsgGsinfoRes->_ServerStatInfo.iSkillExp ;

						static time_t tSaveTime = 0 ;
						static time_t tCurTime = 0 ;
						time( &tCurTime ) ;
						if( tSaveTime + _GS_INFO_LOG_SAVE_TIME_SEC * 5  < tCurTime )
						{
							tSaveTime = tCurTime ;

							_ServerState * pServerLog = g_pLogSvrCom->GetServerLogSlot() ;
							if( pServerLog )
							{
								memcpy( pServerLog->m_server_name, g_ForestName, 13 ) ;
								pServerLog->m_state = g_ServerInfo.GetForestUserCount() ;

								g_pLogSvrCom->InsertServerLog( pServerLog ) ;
							}							
						}						

						CMSG_GSINFO_RES_GS_DS msgGsInfoRes = {0} ;
						memcpy( &msgGsInfoRes, pMsgGsinfoRes, sizeof( CMSG_GSINFO_RES_GS_DS) ) ;

						g_pDSComQueue->GlobalSend( msgGsInfoRes.usLength, (char*)&msgGsInfoRes ) ;

						continue;

					}
					break;
				case CMSG_SHOWTEXT_REQ_DS_GS:
					g_pDSComQueue->GlobalSend( sizeof(CMSG_SHOWTEXT_REQ), pack->buf ) ;
					break ;

				case CMSG_NO_PKEVENT:
					{
						CMSG_PKEVENT * pkEvent = reinterpret_cast<CMSG_PKEVENT*>(pack->buf);

						// 20080404 수정.
						g_ServerInfo.send_to_gs( pkEvent, pkEvent->usLength, static_cast<short>(pkEvent->value2) );

						switch( pkEvent->event )
						{
						case 0:			// 흑백대전의 승자 알림.
							Logout( "[PKEVENT] Get Winner info\n" );
							break;
						case 1:
							Logout( "[PKEVENT] Send Destroy\n" );
							break;
						}
					}
					break;
				}
			} //else if(Mainmsg == 12)

			else if(Mainmsg == 13){		//104~111			
				

			} //else if(Mainmsg == 12)
			else if( Mainmsg == 14 ) {	// 112 ~ 119
				switch( pack->buf[2])
				{
				case CMSG_NO_ALL_SPELL_REQ:
					PrintConsole( "[RECV] All Spell Req\n" ) ;
					g_pDSComQueue->GlobalSend( sizeof(CMSG_ALL_SPELL_APPLY), pack->buf ) ; 
					break ;
				case CMSG_NO_QUEST_REWARD_NOTICE:
					PrintConsole( "[RECV] Quest Reward Req\n" ) ;
					g_pDSComQueue->GlobalSend( sizeof(CMSG_QUEST_REWARD_NOTICE), pack->buf ) ; 
					break;
				case CMSG_NO_ALL_SYSTEM_MSG:
					PrintConsole( "[RECV] All System Msg\n" ) ;
					g_pDSComQueue->GlobalSend( sizeof(CMSG_ALL_SYSTEM_MSG), pack->buf ) ; 
					break ;
#ifdef _PD_CHARAC_LTS_
				case CMSG_NO_LTS:
					try
					{
						CMSG_LTS * pLts = reinterpret_cast<CMSG_LTS*>(pack->buf);

						switch( pLts->ucPktMode )
						{
						case en_lts_pk_insert_req_gs_ds:
							LTS_UTIL::Insert_pk( static_cast<CMSG_LTS_PK_INSERT_REQ_GS_DS*>(pLts), pDSComm, g_DB_Command );
							break;
						case en_lts_pk_del_req_gs_ds:
							LTS_UTIL::Delete_pk( static_cast<CMSG_LTS_PK_DEL_REQ_GS_DS*>(pLts) );
							break;
						case en_lts_sp_insert_req_gs_ds:
							LTS_UTIL::Update_sp( static_cast<CMSG_LTS_SP_INSERT_REQ_GS_DS*>(pLts), pDSComm, g_DB_Command );
							break;
						case en_lts_sp_del_req_gs_ds:
							LTS_UTIL::Reset_sp( static_cast<CMSG_LTS_SP_DEL_REQ_GS_DS*>(pLts) );
							break;
						}
					}
					catch( ... )
					{
						PrintConsole( "[EXCEPTION] CMSG_LTS\n" );
						g_pSystemLog->LogFileWrite( "[EXCEPTION] MANOR TAX SYSTEM\n" );
					}
					break;	
#endif

#ifdef _PD_RECALL_FRIEND_
				case CMSG_NO_RECALL_FRIEND:
					try
					{
						CMSG_RECALL_FRIEND * pRF = reinterpret_cast<CMSG_RECALL_FRIEND*>(pack->buf);

						PrintConsole( "[RF] Recv : Mode(%u), From(%s), Target(%s)\n", 
							pRF->ucFriendMode, pRF->cFromName, pRF->cTargetName );
						
						switch( pRF->ucFriendMode )
						{
						case en_crf_request_recall:
							RF_RequestRecall( static_cast<CMSG_RF_REQUEST_RECALL*>(pRF), pDSComm );
							break;
						case en_crf_answer:
							RF_Answer( static_cast<CMSG_RF_ANSWER*>(pRF) );
							break;
						case en_crf_force_move:
							RF_ForceMove( static_cast<CMSG_RF_FORCE_MOVE*>(pRF) );
							break;
						case en_crf_fail:
							RF_Fail( static_cast<CMSG_RF_FAIL*>(pRF) );
							break;
						}
					}
					catch( ... )
					{
						PrintConsole( "[EXCEPTION] CMSG_NO_RECALL_FRIEND\n" );
						g_pSystemLog->LogFileWrite( "[EXCEPTION] CMSG_NO_RECALL_FRIEND\n" );
					}
					break;
#endif
				}
			}
			else if( Mainmsg == 16 ) {	// 128 ~ 135
				switch((UCHAR)pack->buf[2]) {
				case -1: // 컴파일을 위한 더미
					break;

#ifdef ITEMSHOP_PATCH_200607
				case CMSG_NO_ITEM_SHOP: // 131
					{
						if( !pDB_ItemShop )
							break ;

						CMSG_ITEM_SHOP_BASE* pBase = (CMSG_ITEM_SHOP_BASE*)pack->buf;
						switch(pBase->ucMode) {
						case PKTMODE_PERSONAL_CASH_INVEN_REQ:
							ItemShop_PersonalCashInvenReq((MSG_GS2DS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ*)pack->buf, pDB_ItemShop, pDSComm);
							break;
						case PKTMODE_PRESENT_CASH_INVEN_REQ:
							ItemShop_PresentCashInvenReq((MSG_GS2DS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ*)pack->buf, pDB_ItemShop, pDSComm);
							break;

						case PKTMODE_MOVE_PERSONAL_PACKAGE_NTF:
							ItemShop_MovePersonalPackageNtf((MSG_GS2DS_ITEM_SHOP__MOVE_PERSONAL_PKG_NTF*)pack->buf, pDB_ItemShop, pDSComm);
							break;
						case PKTMODE_MOVE_PRESENT_PACKAGE_NTF:
							ItemShop_MovePresentPackageNtf((MSG_GS2DS_ITEM_SHOP__MOVE_PRESENT_PKG_NTF*)pack->buf, pDB_ItemShop, pDSComm);
							break;
						}
					}
					break;
#endif

#ifdef	_CIS_20070326_
				case CMSG_NO_COMMON_NTF: // 135
					{
						if(pCIS == NULL) break;

						try {
							MSG_COMMON* pRecv = (MSG_COMMON*)pack->buf;
							switch(pRecv->wMode) {
							//12: GS2DS_REQ	[아이템 리스트 요청] 캐쉬인벤의 아이템 리스트 요청
							//13: DS2GS_ACK	[아아템 리스트 응답] 캐쉬인벤의 아이템 리스트 응답
							case 12:
								CIS_ShowCashInvenReq(pRecv, pCIS, pDSComm);
								break;

							//17:	GS2DS_REQ	[아이템 이동 요청] 캐쉬인벤에서 행낭으로 아이템 이동 요청
							//18:	DS2GS_ACK	[아이템 이동 응답] 캐쉬인벤에서 행낭으로 아이템 이동 응답
							case 17:
								CIS_MoveCashItemReq(pRecv, pCIS, pDSComm);
								break;

							//22: GS2DS_REQ	[선물 아이템 상세정보 요청]
							//23: DS2GS_ACK	[선물 아이템 상세정보 응답]
							case 22:
								CIS_PresentMsgReq(pRecv, pCIS, pDSComm);
								break;
							}
						}
						catch(...) {
							Logout("[EXCEPTION] CMSG_NO_COMMON_NTF wMode=%d", ((MSG_COMMON*)pack->buf)->wMode);
							PrintConsole( "[EXCEPTION] CMSG_NO_COMMON_NTF wMode=%d \n", ((MSG_COMMON*)pack->buf)->wMode);
							g_pSystemLog->LogFileWrite( "[EXCEPTION] CMSG_NO_COMMON_NTF wMode=%d \n", ((MSG_COMMON*)pack->buf)->wMode);
						}
					}
					break;
#endif
				}
			}
			//메시지 처리 끝//////////////////////////////////////////////////////////			
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] packet_process (pos:%d, msg:%d) \n", pos, (UCHAR)(pack->buf[2])) ;
		}
		
	} // end while

	goto labelPacketProcess; // for no warning
labelPacketProcess:
#ifdef ITEMSHOP_PATCH_200607
	SAFE_DELETE(pDB_Account);
	SAFE_DELETE(pDB_NDGame);
	SAFE_DELETE(pDB_ItemShop);
#endif

#ifdef	_CIS_20070326_
	SAFE_DEL(pCIS);
#endif

	//g_pErrorMsg->ErrMsg( "[EXIT] th_PacketProcess Return\n " ) ;
	::PrintConsole( "[INFO] th_PacketProcess Return\n " ) ;	

	::PrintConsole( "DWORD WINAPI th_PacketProcess( LPVOID lp ) Exit \n ") ;
	printf( "DWORD WINAPI th_PacketProcess( LPVOID lp ) Exit \n ") ;
		
	delete g_DB_Command;
	delete g_USER_DB_Command ;
	return 0 ;
} // end thread

/*
상황 처리 시나리오

로그인 단계
A : Nothing
B : LS 에 로그인 성공, 10초가 지나지 않았으며, GS에 진입 전
C : LS 에 로그인 성공, 10초 이내에 GS에 진입 시도
D : LS 에 로그인 성공, 10초가 지나서 GS에 진입 시도
E : LS 에 로그인 성공, 캐릭터를 선택하고 플레이중..

LS 에서의 시나리오

1. A 단계 : 조건없이 로그인 허락
2. B 단계 상태에 또다른 접속이 시도될 경우 기존 인증만 유효하고 새로 시도된 로그인 거부
3. C 단계 경우 GS 진입 허락
4. D 단계 경우 GS 진입 불허하고, LS 인증 취소
5. E 단계 경우 LS 에 로그인 할 경우 기존 유저 강제 종료하고 LS 인증 취소, 새 유저 인증 거부
6. E 단계 경우 GS에 새 유저가 접근할 경우 기존 유저 강제 종료하고 LS 인증 취소, 새 유저 인증 거부

서버 이동 시나리오

서버이동 단계

1. 서버 이동중에 또다른 서버 이동을 요청받으면 가짜(?) GS 요청을 무시
2. 10초 이내에 서버 이동 완료가 이루어 지지 못하면 강제 종료하고 LS 인증 취소


*/





DWORD WINAPI th_UpdateProcess( LPVOID lp )
{

	// Create ole-db object	
	COLE_DB_Executer *	g_DB_Command = new COLE_DB_Executer(g_pDBCon) ;
	assert( g_DB_Command ) ;

	C_USER_DB_Command *	g_USER_DB_Command = new C_USER_DB_Command(g_pUSERDBCon);
	assert( g_pUSERDBCon ) ;


	_packet *			pack = NULL ;	
	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct
	_h_DS_Com	*		pDSComm = NULL;	
	
	char				cIP[16]={0};				//IP		
	u_char				Mainmsg = 0;				// 메시지 대분류 구분 변수 8로 나눈 몫
	

	SetEvent( g_hInitEvent );

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;

		
	while( g_Threading ) {

		if( g_pUpdatePacketQ->empty() ) {
			WaitForSingleObject( hTimer, INFINITE ) ;			
			continue ;
		}
		else {

			try{
				pack = g_pUpdatePacketQ->get() ;								// 패킷을 얻고				
				if(pack == NULL) {
					WaitForSingleObject( hTimer, INFINITE ) ;					
					continue;
				}

				//pUser = NULL ;
				//pCharac = NULL ;
				pDSComm = pack->pDSComm;
				if( pDSComm == NULL )
				{
					WaitForSingleObject( hTimer, INFINITE ) ;					
					continue;
				}

				char cResult = 0 ;
				if( pack->buf[2] == CMSG_NO_UPDATE_CHARAC_ALL_ )
				{					
					MSG_UPDATE_CHARAC_ALL * pMsgUpdateCharacAll = (MSG_UPDATE_CHARAC_ALL*)pack->buf ;
					if( sizeof(MSG_UPDATE_CHARAC_ALL) != pMsgUpdateCharacAll->usLength ) 
					{
						::PrintConsole("[PACKET SIZE ERROR] s:%u t:%u chr:%s\n"
							,sizeof(MSG_UPDATE_CHARAC_ALL)
							, pMsgUpdateCharacAll->usLength
							, pMsgUpdateCharacAll->cCharacName ) ;
						continue ;
					}

					if( pMsgUpdateCharacAll->charac.CHARAC_INVENTORY.sVersion == 0 )
					{
						::PrintConsole( "[ERROR] Recv zeroVersion Inven update (%s)\n", pMsgUpdateCharacAll->charac ) ;
						continue ;
					}

#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_UPDATE, "[INFO] CMSG_NO_UPDATE_CHARAC_ALL_ (ACC : %s, CHR : %s) \n"
						,pMsgUpdateCharacAll->cAccount,pMsgUpdateCharacAll->cCharacName);
#endif
					// 업데이트에 실패했다면 튕겨내야지..
					if( UpdateCharacAll( pMsgUpdateCharacAll ) == false )
					{
						CMSG_USER_LOGOUT_DS_GS msgUserkill = {0} ;
						msgUserkill.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
						msgUserkill.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;							

						msgUserkill.cReason = 2 ;				// 제제( 접금... )	
						memcpy( msgUserkill.cNDAccount,  pMsgUpdateCharacAll->cAccount, en_max_lil+1) ;

						if( pDSComm )
						{									
							if( pDSComm->Send( msgUserkill.usLength, (char *)&msgUserkill ) == false ){
								::PrintConsole("[SO_ERR] MSG_CMD_USERKILL_NUM \n");
							}
						}
						cResult = 1 ;
					}

					if( pMsgUpdateCharacAll->charac.CHARAC_ETC.cState == 1 )
					{
						CMSG_ALL_UPDATE_RESULT msgAllUpdateResult = {0} ;
						msgAllUpdateResult.usLength = sizeof(CMSG_ALL_UPDATE_RESULT) ;
						msgAllUpdateResult.cMessage = CMSG_NO_ALL_UPDATE_RESULT_ ;
						memcpy( msgAllUpdateResult.cAccount, pMsgUpdateCharacAll->cAccount, en_max_lil+1 ) ;
						memcpy( msgAllUpdateResult.cCharacName, pMsgUpdateCharacAll->cCharacName, 13 ) ;
						msgAllUpdateResult.cResult = cResult ;

						if( pDSComm )
						{									
							if( pDSComm->Send( msgAllUpdateResult.usLength, (char *)&msgAllUpdateResult ) == false ){
								::PrintConsole("[SO_ERR] CMSG_NO_ALL_UPDATE_RESULT_ \n");
							}
						}

						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Returned update result (chr:%s)\n", msgAllUpdateResult.cCharacName ) ;
					}
					else if( pMsgUpdateCharacAll->charac.CHARAC_ETC.cState == 2 )
					{
						// 로비 : 20080903 15시.
						CMSG_ALL_UPDATE_RESULT msgAllUpdateResult = {0} ;
						msgAllUpdateResult.usLength = sizeof(CMSG_ALL_UPDATE_RESULT) ;
						msgAllUpdateResult.cMessage = CMSG_NO_ALL_UPDATE_RESULT_ ;
						memcpy( msgAllUpdateResult.cAccount, pMsgUpdateCharacAll->cAccount, en_max_lil+1 ) ;
						memcpy( msgAllUpdateResult.cCharacName, pMsgUpdateCharacAll->cCharacName, 13 ) ;
						msgAllUpdateResult.cResult = 2;

						if( pDSComm )
						{									
							if( pDSComm->Send( msgAllUpdateResult.usLength, (char *)&msgAllUpdateResult ) == false ){
								::PrintConsole("[SO_ERR] CMSG_NO_ALL_UPDATE_RESULT_ \n");
							}
						}

						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Returned update result (chr:%s)\n", msgAllUpdateResult.cCharacName ) ;
					}
				}
		
			}
			catch (...) {
				::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
				::PrintConsole("[EXCEPTION] g_pPacketQ->get()\n ");
				continue;
			}		
			
		}
		
	} // end while

	
	::PrintConsole( "[INFO] th_UpdateProcess Return\n " ) ;	

	::PrintConsole( "DWORD WINAPI th_UpdateProcess( LPVOID lp ) Exit \n ") ;
	printf( "DWORD WINAPI th_UpdateProcess( LPVOID lp ) Exit \n ") ;
		
	delete g_DB_Command;
	delete g_USER_DB_Command ;
	return 0 ;
} // end thread




#ifdef _PD_CHARAC_LTS_
u_char LTS_UTIL::Insert_Lvpk()
{
	u_char result = 0;		// success

	return result;
}

u_char LTS_UTIL::Delete_Lvpk()
{
	u_char result = 0;		// success

	return result;
}


u_char LTS_UTIL::Insert_pk( CMSG_LTS_PK_INSERT_REQ_GS_DS * pIns, _h_DS_Com * pCom, COLE_DB_Executer * pDB )
{
	u_char result = 0;		// success
	PCHARAC pCharac = g_pAccMapManager->Find_Charac( pIns->szCharacName );
	if( pCharac )
	{
		PCHARAC pTarget = g_pAccMapManager->Find_Charac( pIns->PkData.szCharacName );
		if( pTarget )
			pIns->PkData.iCharacUID = pTarget->CHARAC_BASIC.iUniqueID;
		else
		{
			pIns->PkData.iCharacUID = pDB->GetCharacCUID( pIns->PkData.szCharacName );
		}

		if( pIns->PkData.iCharacUID && true == pCharac->lts_add_pk( pIns->PkData ) )
		{
			CMSG_LTS_PK_INSERT_SUCCESS_DS_GS pkSuccess;
			CopyMemory( pkSuccess.szCharacName, pIns->szCharacName, en_charac_name_length+1 );
			CopyMemory( &pkSuccess.PkData, &pIns->PkData, sizeof(_sLTS_Data) );

			if( pCom )
				pCom->Send( pkSuccess.usLength, (char*)&pkSuccess );

			PrintConsole( "[LTS] C<%s> Add pk Success\n", pIns->szCharacName );

			return 0;
		}
	}

	CMSG_LTS_PK_INSERT_FAILED_DS_GS pkFailed;
	CopyMemory( pkFailed.szCharacName, pIns->szCharacName, en_charac_name_length+1 );
	pkFailed.ucRes = 0;

	if( pCom )
		pCom->Send( pkFailed.usLength, (char*)&pkFailed );

	PrintConsole( "[LTS] C<%s> Add pk Fail\n", pIns->szCharacName );

	return result;
}
u_char LTS_UTIL::Delete_pk( CMSG_LTS_PK_DEL_REQ_GS_DS * pDel )
{
	u_char result = 0;		// success

	PCHARAC pCharac = g_pAccMapManager->Find_Charac( pDel->szCharacName );
	if( pCharac )
	{
		pCharac->lts_del_pk( pDel->szDelCharacName );
		PrintConsole( "[LTS] C<%s> Del pk Success\n", pDel->szCharacName );
	}
	else
	{
		PrintConsole( "[LTS] C<%s> Del pk Fail\n", pDel->szCharacName );
	}



	return result;
}

u_char LTS_UTIL::Update_sp( CMSG_LTS_SP_INSERT_REQ_GS_DS * pIns, _h_DS_Com * pCom, COLE_DB_Executer * pDB )
{
	u_char ucResult = 2;//2, no_name, 3,

	PCHARAC pCharac = g_pAccMapManager->Find_Charac( pIns->szCharacName );
	if( pCharac )
	{
		int i;
		int iTargetUniqueID = 0;
		int iMyUniqueID = pCharac->CHARAC_BASIC.iUniqueID;

		PCHARAC pTarget = g_pAccMapManager->Find_Charac( pIns->SpData.szCharacName );
		//맵에 있으면 맵의 LTS에 있는지 확인 한다.
		if( pTarget )
		{
			iTargetUniqueID = pTarget->CHARAC_BASIC.iUniqueID;

			for( i = 0 ; i < en_lts_pk_data_count ; ++i )
			{
				//대상자의 LTS에 신청자의 iCharacUid가 있으면 실패
				if( pTarget->CHARAC_LTS.m_pPkData[i].iCharacUID == iMyUniqueID )
					break;
			}

			if( i == en_lts_pk_data_count )
				pIns->SpData.iCharacUID = pTarget->CHARAC_BASIC.iUniqueID;
			else 
			{
				pIns->SpData.iCharacUID = 0;
				ucResult = 3;
			}
		}
		else//그렇지 않으면 DB의 LTS를 확인한다.
		{
			iTargetUniqueID = pDB->GetCharacCUID( pIns->SpData.szCharacName );
			if( iTargetUniqueID )
			{
				_C_CHARAC Charac;
				pDB->GetCharacLts(iTargetUniqueID, &Charac);

				for( i = 0 ; i < en_lts_pk_data_count ; ++i )
				{
					if( Charac.CHARAC_LTS.m_pPkData[i].iCharacUID == iMyUniqueID )
						break;
				}

				if( i == en_lts_pk_data_count )
					pIns->SpData.iCharacUID = iTargetUniqueID;
				else 
				{
					pIns->SpData.iCharacUID = 0;
					ucResult = 3;
				}
			}
		}

		// 자기 리스트 검사.
		if( iTargetUniqueID )
		{
			for( i = 0 ; i < en_lts_pk_data_count ; ++i )
			{
				if( iTargetUniqueID == pCharac->CHARAC_LTS.m_pPkData[i].iCharacUID )
				{
					pIns->SpData.iCharacUID = 0;
					ucResult = 3;
					break;
				}
			}
		}

		if( pIns->SpData.iCharacUID && true == pCharac->lts_add_sp( pIns->SpData ) )
		{
			//GetCharacPk(
			CMSG_LTS_SP_INSERT_SUCCESS_DS_GS pkSuccess;
			CopyMemory( pkSuccess.szCharacName, pIns->szCharacName, en_charac_name_length+1 );
			CopyMemory( &pkSuccess.SpData, &pIns->SpData, sizeof(_sLTS_Data) );
			PrintConsole( "[LTS] GetCharacUID characName=%s, insertcharac=%s ggg\n", pIns->szCharacName, pIns->SpData.szCharacName);

			if( pCom )
				pCom->Send( pkSuccess.usLength, (char*)&pkSuccess );

			return 0;
		}
	}

	//2.대상자 이름이없다.
	CMSG_LTS_SP_INSERT_FAILED_DS_GS pkFailed;
	CopyMemory( pkFailed.szCharacName, pIns->szCharacName, en_charac_name_length+1 );
	pkFailed.ucRes = ucResult;

	if( pCom )
		pCom->Send( pkFailed.usLength, (char*)&pkFailed );

	return 1;
}
u_char LTS_UTIL::Reset_sp( CMSG_LTS_SP_DEL_REQ_GS_DS * pDel )
{
	u_char result = 0;		// success

	PCHARAC pCharac = g_pAccMapManager->Find_Charac( pDel->szCharacName );
	if( pCharac )
	{
		pCharac->lts_del_sp( pDel->szDelCharacName );
	}

	return result;
}

#endif