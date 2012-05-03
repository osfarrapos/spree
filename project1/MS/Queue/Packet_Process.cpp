#include "..\stdafx.h"

#include "..\Global\Global_Define.h"
#include "..\Network\Network.h"
#include "..\VMManager\VMManager.h"
#include "..\Queue\Packet_Queue.h"
#include "..\Map\MapManager.h"
#include "..\Global\Global_LogFile.h"
#include "..\OLE-DB\db_command.h"



// Packet 처리 Functions
bool GSConnect(char cServerNum, _h_DS_Com * dsCom) ;
bool CreateParty(char cServerNum, short uid1, short uid2) ;




extern _queue_Packet * g_pPacketQueue ;
extern _queue_Packet * g_pChatPacketQueue ;
extern _queue_Packet * g_pLoginPacketQueue ;

extern _j_GlobalLog * g_pSystemLog ;
#ifdef _LOGIN_FILE_LOG_WRITE_
extern _j_GlobalLog *		g_pLoginLog ;
#endif
extern _j_MemeberQueue * g_pMemberQueue ;
extern _j_BackupReady * g_pBackupReady ;
extern _j_ComQueue * g_pComQueue ;
extern bool g_pThreadRunning ;
extern CDB_Connector	*	g_pDBCon ;
extern CMYTrace *			g_pTrace ;

extern _j_Templet_Timer * g_pTimeOutTimer ;



DWORD	WINAPI	th_WorkThread( LPVOID lp)
{

	// ole-db	
	COLE_DB_Executer *	g_pDBCommand = new COLE_DB_Executer(g_pDBCon) ;
	if(g_pDBCommand == NULL )
	{	
		g_pSystemLog->LogFileWrite( "th_WorkThread _j_DB_Command Initalize failed\n" ) ;
		return 0 ;
	}	

#ifdef _DEBUG
	//_FriendInfo * f = g_pMemberQueue->GetNode("청화백") ;
	//_FriendInfo  f ;
	//g_pDBCommand->GetMyData( f ) ;
	//g_pDBCommand->UpdateMyData( f ) ;
	//g_pDBCommand->GetMyDataByUID(&f, 29) ;
#endif	

	
	int			Mainmsg = 0 ;	
	_packet *	pack = NULL ;	
	_h_DS_Com * pDSComm = NULL ;
	_Member		member = {0} ;	
	_FriendInfo * pMyInfo = NULL ;	
	_FriendInfo * pFriendInfo = NULL ;	
	_FriendInfo * pTemp = NULL ;
	list<_Member> * pMyFriendList = NULL ;	
	list<_Member>::iterator memberListIter ;

	list<_Mail> * pMyMailList = NULL ;
	list<_Mail>::iterator mailListIter ;

	list<_Mail> * g_pMailList = new list<_Mail>;

	int			iRes ;
	bool		bRes  ;
	_Mail		mail = {0} ;
	

	// loop 용..
	int i = 0, j = 0 ;	

	// Result MSG Initalize
	MMSG_MAKE_FRIEND_RES	msgMakeFriendRes = {0} ;
	msgMakeFriendRes.sLength = sizeof(MMSG_MAKE_FRIEND_RES) ;
	msgMakeFriendRes.sCrypto = false;
	msgMakeFriendRes.sCompressed = false ;
	msgMakeFriendRes.cMessage = MMSG_NO_MAKE_FRIEND_RES_S_C ;

	MMSG_FRIEND_LIST_RES	msgFriendListRes = {0} ;
	//msgFriendListRes.sLength = sizeof(MMSG_FRIEND_LIST_RES) ; <-가변길이..
	msgFriendListRes.sCrypto = false ;
	msgFriendListRes.sCompressed = false ;
	msgFriendListRes.cMessage = MMSG_NO_FRIEND_LIST_RES_S_C ;	

	MMSG_FRIEND_STATE_REPORT msgFriendStateReport = {0} ;
	msgFriendStateReport.sLength = sizeof(MMSG_FRIEND_STATE_REPORT) ;
	msgFriendStateReport.sCrypto = false ;
	msgFriendStateReport.sCompressed = false ;
	msgFriendStateReport.cMessage = MMSG_NO_FRIEND_STATE_REPORT_S_C ;

	MMSG_MAKE_GROUP_RES msgMakeGroupRes = {0} ;
	msgMakeGroupRes.sLength = sizeof(MMSG_MAKE_GROUP_RES) ;
	msgMakeGroupRes.sCrypto = false ;
	msgMakeGroupRes.sCompressed = false ;	
	msgMakeGroupRes.cMessage = MMSG_NO_MAKE_GROUP_RES_S_C ;

	MMSG_CUT_GROUP_RES msgCutGroupRes = {0} ;
	msgCutGroupRes.sLength = sizeof(MMSG_CUT_GROUP_RES) ;
	msgCutGroupRes.sCrypto = false ;
	msgCutGroupRes.sCompressed = false ;	
	msgCutGroupRes.cMessage = MMSG_NO_CUT_GROUP_RES_S_C ;

	MMSG_JOIN_GROUP_RES msgJoinGroupRes = {0} ;
	msgJoinGroupRes.sLength = sizeof(MMSG_JOIN_GROUP_RES) ;
	msgJoinGroupRes.sCrypto = false ;
	msgJoinGroupRes.sCompressed = false ;	
	msgJoinGroupRes.cMessage = MMSG_NO_JOIN_GROUP_RES_S_C ;

	MMSG_CUT_FRIEND_REPORT msgCutFriendReport = {0} ;
	msgCutFriendReport.sLength = sizeof(MMSG_CUT_FRIEND_REPORT) ;
	msgCutFriendReport.sCrypto = false ;
	msgCutFriendReport.sCompressed = false ;
	msgCutFriendReport.cMessage = MMSG_NO_CUT_FRIEND_REPORT_S_C ;

	MMSG_RENAME_GROUP_RES msgRenameGroupRes = {0} ;
	msgRenameGroupRes.sLength = sizeof(MMSG_RENAME_GROUP_RES) ;
	msgRenameGroupRes.sCrypto = false ;
	msgRenameGroupRes.sCompressed = false ;
	msgRenameGroupRes.cMessage = MMSG_NO_RENAME_GROUP_RES_S_C ;


	// Mail =======================================================
	MMSG_MAIL_LIST_RES msgMailListRes = {0} ;
	msgMailListRes.header.sLength = sizeof(MMSG_MAIL_LIST_RES) ;
	msgMailListRes.header.sCrypto = false ;
	msgMailListRes.header.sCompressed = false ;
	msgMailListRes.cMessage = MMSG_NO_MAIL_LIST_RES_S_C ;
	
	MMSG_MAIL_TEXT_RES msgMailTextRes = {0} ;
	msgMailTextRes.header.sLength = sizeof(MMSG_MAIL_TEXT_RES) ;
	msgMailTextRes.header.sCrypto = false ;
	msgMailTextRes.header.sCompressed = false ;
	msgMailTextRes.cMessage = MMSG_NO_MAIL_TEXT_RES_S_C ;

	MMSG_MAIL_SEND_RES msgMailSendRes = {0} ;
	msgMailSendRes.header.sLength = sizeof(MMSG_MAIL_SEND_RES) ;
	msgMailSendRes.header.sCrypto = false ;
	msgMailSendRes.header.sCompressed = false ;
	msgMailSendRes.cMessage = MMSG_NO_MAIL_SEND_RES_S_C ;

	MMSG_MAIL_RECV msgMailRecv = {0} ;
	msgMailRecv.header.sLength = sizeof(MMSG_MAIL_RECV) ;
	msgMailRecv.header.sCrypto = false ;
	msgMailRecv.header.sCompressed = false ;
	msgMailRecv.cMessage = MMSG_NO_MAIL_RECV_S_C ;


	MMSG_MAIL_FAIL msgMailFail = {0} ;
	msgMailFail.header.sLength = sizeof(MMSG_MAIL_FAIL) ;
	msgMailFail.header.sCrypto = false ;
	msgMailFail.header.sCompressed = false ;
	msgMailFail.cMessage = MMSG_NO_MAIL_FAIL_S_C ;


	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;


	while (g_pThreadRunning) {
		if( g_pPacketQueue->empty() ) {			
			WaitForSingleObject( hTimer, INFINITE ) ;
			continue ;
		}
		else {
			pack = g_pPacketQueue->get() ;			
			
			if( pack == NULL ) {
				WaitForSingleObject( hTimer, INFINITE ) ;
				continue ;
			}
			else {

			
				Mainmsg = (u_char)pack->buf[2] ;

				
				// 포인터 초기화..													
				//pMyCharacName = NULL;
				pMyInfo = NULL ;		
				pFriendInfo = NULL ;	
				pTemp = NULL ;
				pMyFriendList = NULL ;

				int pos = 0 ;
				try{
					// 쓰레드 전역에서 쓰일 포인터들이다..
					pDSComm = pack->pDSComm ;
					if(!pDSComm){
						::PrintConsole("[ERROR] 'pDSComm' is NULL \n") ;
						continue ;
					}

					pos = 1 ;
					
					pMyInfo = (_FriendInfo*)pDSComm->m_pFriendInfo ;		// 자기 정보
					if( !pMyInfo )
					{
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] 'WORK:pDSComm->m_pFriendInfo' is NULL \n") ;
						continue ;
					}

					pos = 2 ;

					//pMyCharacName = pMyInfo->GetName() ;		// 캐릭이름	
					pMyFriendList = pMyInfo->GetFriendListPtr() ;				// 친구들 정보 리스트
					pMyMailList = pMyInfo->GetMailListPtr() ;

					pos = 3 ;

					if( !pMyMailList  || !pMyFriendList ) {
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] '!pMyMailList || !pMyFriendList' is NULL \n") ;
						continue ;
					}

					pos = 4 ;
					if( pMyInfo->IsLogon() == false ) {	// 로그오프중이다.
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] pMyInfo->ds_com == NULL \n" ) ;
						continue ;
					}
					//if( Mainmsg != MMSG_NO_MY_STATE_UPDATE_C_S && pMyInfo->state == CHARAC_STATE_LOGOFF ){
					//	::PrintConsole("[ERROR] Mainmsg != MMSG_NO_MY_STATE_UPDATE_C_S && pMyInfo->state == CHARAC_STATE_LOGOFF \n" ) ;
					//	continue ;
					//}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION]  Get Queue : g_pPacketQueue->get() pos : %d, msg : %d\n", pos, Mainmsg ) ;
					continue ;
				}
				
								
			}				
		}
		

		try{
			switch( Mainmsg ) {		

			case MMSG_NO_MAKE_FRIEND_REQ_C_S_C :
				MMSG_MAKE_FRIEND_REQUEST * pMsgMakeFriendRequest ;
				pMsgMakeFriendRequest = (MMSG_MAKE_FRIEND_REQUEST*)pack->buf ;

				pMsgMakeFriendRequest->cCharacName[12] = 0 ;

				try{
						
#ifdef _TRACE_		
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_MAKE_FRIEND_REQUEST' Packet <%s>\n",pMyInfo->GetName() ) ;					
#endif
					
					if( strcmp( pMyInfo->GetName(), pMsgMakeFriendRequest->cCharacName ) == 0 )
					{
						bRes = true ;
					}
					else if( pMyInfo->GetName()[0] == 0 )
					{						
						pMyInfo->DisconnectForLogout() ;

						continue ;
					}
					else
					{
						bRes = pMyInfo->IsAlreadyFriend( pMsgMakeFriendRequest->cCharacName ) ;
					}				

					// 친구 맺기 전에 이미 등록되어 있는지 확인하고 보내자.. 
					if( bRes == true ) {
						// 이미 존재하는 친구라는 통보..
						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 5 ;	// 이미 등록중인 친구.
						iRes = pMyInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pMyInfo->SendMsg code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						continue ;
					}

					pFriendInfo = g_pMemberQueue->FindNode(pMsgMakeFriendRequest->cCharacName) ;
					
					
					// 상대 친구가 친구 맺기가 가능하지를 먼저 검사한다..
					if( pFriendInfo == NULL ) {								// 로그오프 중..

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[ERROR] 해당 친구(%s)는 접속해 있지 않습니다. \n", pMsgMakeFriendRequest->cCharacName) ;						
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 2 ;
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					else if( pFriendInfo->GetName()[0] == 0 )
					{
						g_pMemberQueue->DeleteMap( msgMakeFriendRes.cCharacName ) ;
						::PrintConsole( "[ERROR] %s, %d \n", __FILE__, __LINE__  ) ;

						pFriendInfo->DisconnectForLogout() ;
					}
					else if( pFriendInfo->IsGm() )
					{
						// GM 캐릭터 입니다..
						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 6 ;	// GM 캐릭터..
						iRes = pMyInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pMyInfo->SendMsg code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						continue ;
						
					}
					else if( pFriendInfo->IsLogon() == false ) {				// 로그오프 중..

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[ERROR] 해당 친구(%s)는 로그오프중 입니다. \n", pFriendInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;
						msgMakeFriendRes.cResult = 2 ;

						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}					
					else if( pFriendInfo->GetOption() == 1 ) {					// 상대가 거부상태..

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] 해당 친구(%s)는 거부상태 입니다. \n", pFriendInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 1 ;
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
						
					}
					else if( pFriendInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pFriendInfo->GetMemberCount() >= MAX_MEMBER ) {	// 친구쪽 초과

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] 해당 친구(%s) 저장공간 부족 \n", pFriendInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 3 ;

						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					else if( pMyInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pMyInfo->GetMemberCount() >= MAX_MEMBER ) {		// 내쪽 초과

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] 나(%s)의 저장공간 부족 \n", pMyInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 4 ;

						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					
					// 친구에게 이미 내가 등록되어 있다면 바로 등록 완료 시키자
					else if( pFriendInfo->IsAlreadyFriend( pMyInfo->GetName() ) == true )
					{

						if( pMyInfo->AddFriend( pFriendInfo->GetName(), pFriendInfo ) == true )
						{
							// 백업 옵션 걸기.
							pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
							g_pBackupReady->Insert(pMyInfo) ;	

							// 내쪽에 결과 통보			
							memcpy(msgMakeFriendRes.cCharacName, pFriendInfo->GetName(), 13) ;
							msgMakeFriendRes.cState = pFriendInfo->GetState() ;
							msgMakeFriendRes.cResult = 0 ;
							
							
							if ( pDSComm ) {
								iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
									continue ;
								}
							}
						}

						continue ;
					}

					// 친구맺기 요청 매세지 통과~~ 
			
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] 친구 맺기 요청 ( %s -> %s ) \n",pMyInfo->GetName(), pMsgMakeFriendRequest->cCharacName) ;
#endif

					memcpy( pMsgMakeFriendRequest->cCharacName, pMyInfo->GetName(), 13 ) ;	// cCharacName 을 싹~ 바꿔치기 해야한다.
					iRes = pFriendInfo->SendMsg( (char*)pMsgMakeFriendRequest, pMsgMakeFriendRequest->sLength ) ;						
					if(iRes != 0){
						::PrintConsole( "[SO_ERR] pFriendInfo->SendMsg : code = %d\n", iRes ) ;
						::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						continue ;
					}						
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAKE_FRIEND_REQ_C_S_C : \n") ;				
				}

		

				break;

		

			case MMSG_NO_MAKE_FRIEND_CON_C_S :
				MMSG_MAKE_FRIEND_CONFIRM * pMsgMakeFriendConfirm ;
				pMsgMakeFriendConfirm = (MMSG_MAKE_FRIEND_CONFIRM*) pack->buf ;

				pMsgMakeFriendConfirm->cCharacName[12] = 0 ;

				
				try{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_MAKE_FRIEND_CONFIRM' packet<%s>\n",pMyInfo->GetName()) ;
#endif					
					
					if( pMsgMakeFriendConfirm->cCharacName[0] == 0 || pMsgMakeFriendConfirm->cCharacName[0] == ' ' ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[ERROR] pMsgMakeFriendReq->cToCharac[0] == 0 || pMsgMakeFriendReq->cToCharac[0] == ' ' \n") ;
#endif						

						continue ;
					}

					pFriendInfo = g_pMemberQueue->FindNode( pMsgMakeFriendConfirm->cCharacName ) ;

					
					// 친구쪽 친구 등록 가능여부 검사..
					if( pFriendInfo  == NULL ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[ERROR] (%s) is logoff * \n",pMsgMakeFriendConfirm->cCharacName) ;
#endif

						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 2 ;	// 해당 캐릭터가 로그오프중이거나 없음..								
										
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					else if( pFriendInfo->IsLogon() == false ){

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[ERROR] (%s) is logoff ** \n",pMsgMakeFriendConfirm->cCharacName) ;
#endif

						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 2 ;	// 해당 캐릭터가 로그오프중이거나 없음..
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
						
					}	
					

					else if( pMsgMakeFriendConfirm->cResult == 1 ) {	//거부 ( 거부패킷은 검사 할것도 없이 바로 보낸다..)
#ifdef _TRACE_
						// 이 트레이스는 꼭 확인해볼것..
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] 친구맺기 거부. (%s ->%s) \n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
#endif			

						memcpy( msgMakeFriendRes.cCharacName, pMyInfo->GetName(), 13 ) ;				
						msgMakeFriendRes.cResult = 1 ;	// 상대가 거부 (거부당한 캐릭터(cFromCharacName)에게 통보)
													
						iRes = pFriendInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pFriendInfo->SendMsg : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						
						continue ;
						
					}

					else if( pFriendInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pFriendInfo->GetMemberCount() >= MAX_MEMBER ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] (%s) 친구 저장공간 부족 \n", pFriendInfo->GetName() ) ;
#endif


						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 3 ;	// 상태 캐릭터의 허용 친구 인원 초과..				
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}

					
					// 내쪽 친구 등록 가능여부 검사..
					else if( pMyInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pMyInfo->GetMemberCount() >= MAX_MEMBER ) {				

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] (%s) 나의 저장공간 부족 \n", pMyInfo->GetName() ) ;
#endif

						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 4 ;	// 내쪽 캐릭터의 허용 친구 인원 초과..				
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}

					
					// 내쪽 친구 정보에 대상 친구 이미 존재 여부 검사	
					bRes = pMyInfo->IsAlreadyFriend( pMsgMakeFriendConfirm->cCharacName ) ;
					if( bRes == true ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[ERROR] 내(%s) 친구(%s) 정보에 이미 존재하는 친구입니다. \n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
#endif
						// 이미 존재하는 친구라는 통보..
						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;
						msgMakeFriendRes.cResult = 5 ;	// 내쪽 캐릭터의 허용 친구 인원 초과..				
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}


					// 친구의 친구 정보에 대상 친구 이미 존재 여부 검사

					bRes = pFriendInfo->IsAlreadyFriend( pMyInfo->GetName() ) ;
					if( bRes == true )
					{
						msgMakeFriendRes.cResult = 5 ;
						// 상대 친구한테 등록 결과 보고(결과는 양쪽에 모두 보내준다..)			
						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;						
						if ( pDSComm ) {
							iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
								continue ;
							}
						}

						memcpy( msgMakeFriendRes.cCharacName, pMyInfo->GetName(), 13 ) ;						
						iRes = pFriendInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE,"[ERROR] 친구(%s)의 친구 정보에 이미 나(%s)의 정보가 존재합니다. \n", pFriendInfo->GetName(), pMyInfo->GetName() ) ;
#endif

						//Goto First Loop
						continue ;

					}

					
					bool bSuccess = true ;
					// 친구 추가
					if( pMyInfo->AddFriend( pFriendInfo->GetName(), pFriendInfo ) == false )
					{
						bSuccess = false ;
					}
					

					// 친구 추가
					if( bSuccess && pFriendInfo->AddFriend( pMyInfo->GetName(), pMyInfo ) == false )
					{
						bSuccess = false ;
					}

					if( bSuccess == true )
					{
						// 백업 옵션 걸기.
						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
						g_pBackupReady->Insert(pMyInfo) ;						
						
						// 백업 옵션 걸기.
						pFriendInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
						g_pBackupReady->Insert(pFriendInfo) ;
						
						
						msgMakeFriendRes.cResult = 0 ;
						
						// 내쪽에 결과 통보			
						memcpy(msgMakeFriendRes.cCharacName, pFriendInfo->GetName(), 13) ;
						msgMakeFriendRes.cState = pFriendInfo->GetState() ;
						
						
						if ( pDSComm ) {
							iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
								continue ;
							}
						}
						
						
						// 상대 친구한테 등록 결과 보고(결과는 양쪽에 모두 보내준다..)
						memcpy(msgMakeFriendRes.cCharacName, pMyInfo->GetName(), 13) ;
						msgMakeFriendRes.cState = pMyInfo->GetState() ;
						
						iRes = pFriendInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;		
							continue ;
						}
						
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] 친구 등록 성공 ( %s + %s ). \n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
#endif
					}
					else
					{
						::PrintConsole("[ERROR] Make friend is failed (%s <-> %s)\n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
					}
					
				}
				catch (...) {
					PrintConsole("[EXCEPTION] case MMSG_NO_MAKE_FRIEND_CON_C_S : \n") ;
				}
				
				break;

				

			case MMSG_NO_CUT_FRIEND_REQ_C_S :
				MMSG_CUT_FRIEND_REQ * pMsgCutFriendReq ;
				pMsgCutFriendReq = (MMSG_CUT_FRIEND_REQ*)pack->buf ;

				pMsgCutFriendReq->cFriendCharac[12] = 0 ;

				try{
	
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_CUT_FRIEND_REQ' Packet (%s -> %s)\n", pMyInfo->GetName(), pMsgCutFriendReq->cFriendCharac ) ;
#endif
					
					// MyInfo 에서 삭제..
					pMyInfo->DeleteFriend( pMsgCutFriendReq->cFriendCharac ) ;
					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					pMyInfo->SetMyChatFriend( NULL ) ;
					g_pBackupReady->Insert( pMyInfo ) ;
					
					
					// 맵에 친구의 데이타가 없다.. DB에 가서 긁어와야한다..					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgCutFriendReq->cFriendCharac ) ;
					
					
					if( pFriendInfo == NULL ) {						
						pFriendInfo = g_pMemberQueue->GetNode( pMsgCutFriendReq->cFriendCharac ) ;				
												
						if( pFriendInfo ) {							
							bRes = pFriendInfo->ReadFromDB( g_pDBCommand ) ;
														
							if( bRes == false ) {								
								g_pMemberQueue->PutNode( pFriendInfo ) ;
								
								// DB 에 캐릭터가 없는것이다..(아마 삭제된듯..)
								// 암튼.. 그래도 삭제 통보를 해주자..
								memcpy(msgCutFriendReport.cCharacName, pMsgCutFriendReq->cFriendCharac, 13) ;
								if( pDSComm ) {	// 온라인.. 중..				
									iRes = pDSComm->m_pNetIO->SendRequest(msgCutFriendReport.sLength, (char*)&msgCutFriendReport) ;
									if(iRes != 0){
										::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
										::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
										pDSComm->DisConnect() ;
									}
								}							
								continue ;
							}
						}
						else {
							continue ;
						}
					}
					
					if( pFriendInfo ) {

						pFriendInfo->DeleteFriend( pMyInfo->GetName() ) ;
						// Backup option checking
						//pFriendInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						//pFriendInfo->SetMyChatFriend( NULL ) ;
						//g_pBackupReady->Insert(pFriendInfo) ;						

					}
									
					
					memcpy(msgCutFriendReport.cCharacName, pMsgCutFriendReq->cFriendCharac, 13) ;
					if( pDSComm ) {	// 온라인.. 중..				
						iRes = pDSComm->m_pNetIO->SendRequest(msgCutFriendReport.sLength, (char*)&msgCutFriendReport) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}			
					
					
					memcpy(msgCutFriendReport.cCharacName, pMyInfo->GetName(), 13) ;
					if(  pFriendInfo && pFriendInfo->IsLogon() ) {	// 온라인.. 중..				
						iRes = pFriendInfo->SendMsg( (char*)&msgCutFriendReport, msgCutFriendReport.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							continue ;
							
						}
					}
					else {	// 오프라인

						if( pFriendInfo->GetMailCurCount() < MAX_MAIL_COUNT ) {
							// 매일 인덱스 번호를 얻어온다.							
							u_int uiMailIndex = 0 ;
							
							
							// 차후에 로그인 했을때 전서구로 알려줘야 한다..
							// 이 곳에 구현하자..
							if( g_pDBCommand->MSMessage( &uiMailIndex, pMyInfo->GetName(), MSMESSAGE_CUT_FRIEND) )
							{
								// 해당 친구의 메일 리스트에 인덱스를 넣어줘야 한다.

								memset( &mail, 0, sizeof(mail) ) ;
								mail.check = false ;
								mail.uiIndex = uiMailIndex ;
								pFriendInfo->AddMail( mail ) ;

								pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;								

							}						
							
						}					
						
					}

					pFriendInfo->SetMyChatFriend( NULL ) ;
					pFriendInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					g_pBackupReady->Insert( pFriendInfo ) ;
									

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_CUT_FRIEND_REQ_C_S : \n") ;
				}

				break; 

			case MMSG_NO_FRIEND_LIST_REQ_C_S :
				MMSG_FRIEND_LIST_REQ * pMsgFriendListReq ;
				pMsgFriendListReq = (MMSG_FRIEND_LIST_REQ*)pack->buf ;

				try{
			
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_FRIEND_LIST_REQ' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif
					
					memset(&msgFriendListRes, 0, sizeof(MMSG_FRIEND_LIST_RES)) ;
					
					// 친구 리스트를 가변 길이로 보내줘야 한다..
					i = 0, j = 0 ;			
					// 가변길이 리스트 만들기..
					pMyInfo->SetFriendListPakcet( &msgFriendListRes ) ;					
					if( pDSComm->m_pNetIO )
					{
						iRes = pDSComm->m_pNetIO->SendRequest(msgFriendListRes.sLength, (char*)&msgFriendListRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
							
						}
					}					
		
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Friend List Send (Response) (CharacName : %s, Count : %d) \n", pMyInfo->GetName(), msgFriendListRes.cCount ) ;
#endif

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] ase MMSG_NO_FRIEND_LIST_REQ_C_S : \n") ;
				}
				
				

				break;
				

			case MMSG_NO_MY_STATE_UPDATE_C_S :
				MMSG_MY_STATE_UPDATE * pMsgMyStateUpdate ;
				pMsgMyStateUpdate = (MMSG_MY_STATE_UPDATE *)pack->buf ;
				
				try{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_MY_STATE_UPDATE' Packet <%s>\n",pMyInfo->GetName() ) ;				
#endif
					
					pMyInfo->SetState( pMsgMyStateUpdate->cState ) ;
					
					memcpy(msgFriendStateReport.cCharacName, pMyInfo->GetName(), 13) ;
					msgFriendStateReport.cState = pMyInfo->GetState() ;
					
					// 친구들에게 전파하라..
					iRes = pMyInfo->SendMsg( (char*)&msgFriendStateReport, msgFriendStateReport.sLength, _SEND_MODE_MY_FRIENDS ) ;
					if( iRes != 0 )
					{
						::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
						::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;	
						continue ;
					}
					
				}
				catch (...) {					
					::PrintConsole("[EXCEPTION] case MMSG_NO_MY_STATE_UPDATE_C_S : \n") ;
				}


				break;

			//#define MMSG_NO_MAKE_GROUP_REQ_C_S		// 그릅 만들기 요청
			case MMSG_NO_MAKE_GROUP_REQ_C_S :

				MMSG_MAKE_GROUP_REQ * pMsgMakeGroupReq ;
				pMsgMakeGroupReq = (MMSG_MAKE_GROUP_REQ*)pack->buf ;

				pMsgMakeGroupReq->cGroupName[14] = 0 ;

				try{
#ifdef _TRACE_					
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_NO_MAKE_GROUP_REQ_C_S' Packet <ID : %s, GROUP : %s>\n",pMyInfo->GetName(), pMsgMakeGroupReq->cGroupName ) ;
#endif
					
					if(pMsgMakeGroupReq->cGroupName[0] == 0 
						|| pMsgMakeGroupReq->cGroupName[0] == ' ' 
						|| strcmp( pMsgMakeGroupReq->cGroupName, "친구") == 0 ) {
						continue ;
					}
					
					memcpy(msgMakeGroupRes.cGroupName, pMsgMakeGroupReq->cGroupName, 15 ) ;
					
										
					// 그릅 등록 가능여부 검사..
					iRes = 0 ;
					if( pMyInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pMyInfo->GetGroupCurCount() >= MAX_GROUP ) 
					{
						msgMakeGroupRes.cResult = 1 ;	// 실패 메시지..				
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeGroupRes.sLength, (char*)&msgMakeGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop						
						continue ;
					}			
					
					
					
					// 내 리스트 정보에 대상 그릅 등록
					memset(&member, 0, sizeof(_Member)) ;
					member.characName[0] = _GROUP_PARSING_CHAR_ ;	// 그릅 이름임을 구분시켜준다..
					strncpy(&member.characName[1], pMsgMakeGroupReq->cGroupName, 14) ;										
					pMyInfo->AddFriend( member.characName, NULL ) ;
					// 백업 옵션 걸기.
					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
					g_pBackupReady->Insert(pMyInfo) ;
					
										
					// 그릅 생성 성공 결과 
					memcpy(msgMakeGroupRes.cGroupName, pMsgMakeGroupReq->cGroupName, 15 ) ;
					msgMakeGroupRes.cResult = 0 ;	// 0 .성공 메시지..
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] 그릅 생성 성공 \n" ) ;
#endif
					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeGroupRes.sLength, (char*)&msgMakeGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAKE_GROUP_REQ_C_S : \n " ) ;
				}

					

				break;		
				
			//#define MMSG_NO_CUT_GROUP_REQ_C_S			// 그릅 삭제 요청
			case MMSG_NO_CUT_GROUP_REQ_C_S :
				MMSG_CUT_GROUP_REQ * pMsgCutGroup ;
				pMsgCutGroup = (MMSG_CUT_GROUP_REQ*)pack->buf ;

				pMsgCutGroup->cGroupName[14] = 0 ;

				try{
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_CUT_GROUP_REQ' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif
					
					msgCutGroupRes.cResult = 1 ;
					memcpy(msgCutGroupRes.cGroupName, pMsgCutGroup->cGroupName, 15) ;


					// MyInfo 에서 삭제..
					memset(&member, 0, sizeof(_Member)) ;
					member.characName[0] = _GROUP_PARSING_CHAR_ ;	// 그릅 이름임을 구분시켜준다..
					strcpy(&member.characName[1], pMsgCutGroup->cGroupName ) ;

					if( pMyInfo->DeleteFriend( member.characName ) )
					{
						msgCutGroupRes.cResult = 0 ;
						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						g_pBackupReady->Insert(pMyInfo) ;
					}
					
					
					// 그릅 삭제 결과 			
					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgCutGroupRes.sLength, (char*)&msgCutGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}	
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_CUT_GROUP_REQ_C_S : \n") ;
				}
				

				break;
						
			//#define MMSG_NO_JOIN_GROUP_REQ_C_S		// 친구를 그릅에 등록하도록 요청..
			case MMSG_NO_JOIN_GROUP_REQ_C_S :
				MMSG_JOIN_GROUP_REQ * pMsgJoinGroup ;
				pMsgJoinGroup = (MMSG_JOIN_GROUP_REQ*)pack->buf ;

				pMsgJoinGroup->cFriendName[12] = 0 ;
				pMsgJoinGroup->cGroupName[14] = 0 ;

				try{	// label A
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_JOIN_GROUP_REQ' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif
					
					if( pMsgJoinGroup->cFriendName[0] == 0 || pMsgJoinGroup->cFriendName[0] == ' '){
						continue;
					}
					if( pMsgJoinGroup->cGroupName[0] == 0 || pMsgJoinGroup->cGroupName[0] == ' ' ) {
						continue ;
					}
					
					memcpy(msgJoinGroupRes.cFriendName, pMsgJoinGroup->cFriendName, 13) ;
					memcpy(msgJoinGroupRes.cGroupName, pMsgJoinGroup->cGroupName, 15) ;
					msgJoinGroupRes.cResult = 1 ;
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_JOIN_GROUP_REQ_C_S : label A \n") ;
				}
				
				
				memset(&member, 0, sizeof(_Member) ) ;
				member.characName[0] = _GROUP_PARSING_CHAR_ ;
				strcpy(&member.characName[1], pMsgJoinGroup->cGroupName ) ;

				if( pMyInfo->MoveFriend( pMsgJoinGroup->cFriendName, member.characName ) )
				{					
					msgJoinGroupRes.cResult = 0 ;

					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					g_pBackupReady->Insert( pMyInfo ) ;
				}

				try{	// label C
					if ( pDSComm ) {	
						iRes = pDSComm->m_pNetIO->SendRequest(msgJoinGroupRes.sLength, (char*)&msgJoinGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}			
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_JOIN_GROUP_REQ_C_S : label C \n") ;
				}		
			


				break;


			case MMSG_NO_RENAME_GROUP_REQ_C_S :
				MMSG_RENAME_GROUP_REQ * pMsgRenameGroupReq ;
				pMsgRenameGroupReq = (MMSG_RENAME_GROUP_REQ*)pack->buf ;

				pMsgRenameGroupReq->cGroupName[14] = 0 ;
				pMsgRenameGroupReq->cReName[14] = 0 ;

				try{
					msgRenameGroupRes.cResult = 1 ;

					if( pMyInfo->GroupRename( pMsgRenameGroupReq->cGroupName, pMsgRenameGroupReq->cReName ) )
					{						
						msgRenameGroupRes.cResult = 0 ;

						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						g_pBackupReady->Insert( pMyInfo ) ;
					}					
					
					
					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgRenameGroupRes.sLength, (char*)&msgRenameGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_RENAME_GROUP_REQ_C_S : \n") ;
				}
				

				break; 


////////////////////////////
// Mail part
////////////////////////////

			case MMSG_NO_MAIL_LIST_REQ_C_S :
				MMSG_MAIL_LIST_REQ * pMailListReq ;
				pMailListReq = (MMSG_MAIL_LIST_REQ*)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_LIST_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{
									
					memset( msgMailListRes.mail, 0, sizeof(msgMailListRes.mail) ) ;
					pMyInfo->SetMailListPacket( &msgMailListRes ) ;
					

					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailListRes.header.sLength, (char*)&msgMailListRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}


				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_LIST_REQ_C_S : \n") ;
				}
				

				break ;


			case MMSG_NO_MAIL_TEXT_REQ_C_S :
				MMSG_MAIL_TEXT_REQ * pMailTextReq ;
				pMailTextReq = (MMSG_MAIL_TEXT_REQ*)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_TEXT_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{
					
					bRes = g_pDBCommand->GetMail( pMailTextReq->uiIndex, &msgMailTextRes.mailInfo ) ;
					if(bRes == false)	// 실패
					{
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_TEXT_RES_S_C ;

						if ( pDSComm ) {
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}
						continue ;
					}

					if ( pDSComm ) {
						// header(3) + MailHeader(23) + strLen
						msgMailTextRes.header.sLength = 26 + msgMailTextRes.mailInfo.cStrLen ;	
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailTextRes.header.sLength, (char*)&msgMailTextRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}

						// 메일 읽음 표시..
						for( mailListIter = pMyMailList->begin() ; mailListIter != pMyMailList->end() ; mailListIter++ )
						{
							if( mailListIter->uiIndex == pMailTextReq->uiIndex )
							{
								mailListIter->check = true ;
								
								// 백업 준비..
								pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;								
								g_pBackupReady->Insert( pMyInfo ) ;
								
								break;
							}
						}					
						
						
					}

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_TEXT_REQ_C_S : \n") ;
				}

				break ;


			case MMSG_NO_MAIL_SEND_REQ_C_S :
				{
				MMSG_MAIL_SEND_REQ * pMailSendReq ;
				pMailSendReq = (MMSG_MAIL_SEND_REQ*)pack->buf ;

				pMailSendReq->cToCharac[12] = 0 ;
				pMailSendReq->cText[100] = 0 ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_SEND_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{

					list<_Mail> * pMailList = NULL ;
					int	uid ;

					pFriendInfo = g_pMemberQueue->FindNode( pMailSendReq->cToCharac ) ;
					
					//============================================================================
					// 1. Mail list 읽어오기..
					// 데이타 있다.
					if( pFriendInfo )
					{
						pMailList = pFriendInfo->GetMailListPtr() ;
						if( pMailList == NULL )
						{
							::PrintConsole("[error] MMSG_NO_MAIL_SEND_REQ_C_S::pMailList == NULL \n") ;
							continue ;
						}
						if( pFriendInfo->IsGm() )
						{
							MMSG_MAIL_FAIL msgMailFail = {0} ;
							msgMailFail.header.sLength = sizeof(MMSG_MAIL_FAIL) ;
							msgMailFail.cMessage = MMSG_NO_MAIL_FAIL_S_C ;
							msgMailFail.cCmd = 4 ;	// GM 에게는 쪽지를 보낼 수 없습니다.
							
							if ( pDSComm ) {					
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}							
							continue ;							
						}
						
					}
					// 데이타 없다.
					else
					{	
						// list<_Mail> mailList ;
						pMailList = g_pMailList ;
						pMailList->clear() ;
						
						bRes = g_pDBCommand->GetOffMailList(pMailSendReq->cToCharac, &uid, pMailList) ;
						if( bRes == false ){
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->GetMailList() (%s) \n", pMailSendReq->cToCharac) ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
							msgMailFail.cResult = 0 ;
							if ( pDSComm ) {
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}					
							continue ;
						}
					}

					//============================================================================
					// 2. check size
					if( pMailList->size() >= MAX_MAIL_COUNT ) {
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
						msgMailFail.cResult = 1 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}							
						continue ;
					}



					//============================================================================
					// 3. Send mail
					u_int uiMailIndex = 0 ;

					if( pMyInfo->IsGm() )
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMailSendReq->cText, pMailSendReq->cStrLen, _MailInfo::_FROM_GM ) ;
					}
					else
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMailSendReq->cText, pMailSendReq->cStrLen, _MailInfo::_PRIVATE ) ;
					}


					if( bRes == false ){
						g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->SendMail( iRes, pMyInfo->characName, pMailSendReq->cText, pMailSendReq->cStrLen )  \n") ;
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
						msgMailFail.cResult = 3 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}						
						continue ;
					}			
					
					
					
					//============================================================================
					// 4. Mail list update
					memset( &mail, 0, sizeof(_Mail) ) ;
					mail.uiIndex = uiMailIndex ;
					mail.check = false ;
					
					if( pFriendInfo )
					{
						
						pFriendInfo->AddMail( mail ) ;
															
						pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;						
						g_pBackupReady->Insert( pFriendInfo ) ;
												
						
						// 대상이 로그인중이라면 메일이 도착했음을 알린다.
						if( pFriendInfo->IsLogon() )
						{
							msgMailRecv.uiIndex = mail.uiIndex ;
							iRes = pFriendInfo->SendMsg( (char*)&msgMailRecv, msgMailRecv.header.sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}
						}
					}
					else
					{

						pMailList->push_back( mail ) ;

						bRes = g_pDBCommand->UpdateMailList( uid, pMailList ) ;
						if( bRes == false ) {
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->UpdateMailList() \n") ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
							msgMailFail.cResult = 3 ;
							if ( pDSComm ) {					
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}							
							continue ;
						}

					}									


					// 4. result send
					if ( pDSComm ) {					
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailSendRes.header.sLength, (char*)&msgMailSendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_SEND_REQ_C_S : \n") ;
				}
				}
				break;

			case MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S :
				{
				MMSG_MAIL_PUBLIC_SEND_REQ * pMailPublicSendReq ;
				pMailPublicSendReq = (MMSG_MAIL_PUBLIC_SEND_REQ*)pack->buf ;

				
				pMailPublicSendReq->cText[100] = 0 ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{

					list<_Mail> * pMailList = NULL ;
					int	uid ;
					char * pToName ;

					for( int i =0 ; i<100 ; i++ )
					{

						if( pMailPublicSendReq->cNames[i][0] == 0 )
							break; 


						pToName = pMailPublicSendReq->cNames[i] ;

						pFriendInfo = g_pMemberQueue->FindNode( pToName ) ;

						//============================================================================
						// 1. Mail list 읽어오기..
						// 데이타 있다.
						if( pFriendInfo )
						{
							pMailList = pFriendInfo->GetMailListPtr() ;
							if( pMailList == NULL )
							{
								::PrintConsole("[error] MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S::pMailList == NULL \n") ;
								continue ;
							}
							
						}
						// 데이타 없다.
						else
						{	
							// list<_Mail> mailList ;
							pMailList = g_pMailList ;
							pMailList->clear() ;
							
							bRes = g_pDBCommand->GetOffMailList(pToName, &uid, pMailList) ;
							if( bRes == false ){
								g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->GetMailList() (%s) \n", pToName) ;
								continue ;
							}
						}
	
						//============================================================================
						// 2. check size
						if( pMailList->size() >= MAX_MAIL_COUNT ) {
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] pMailList->size() >= MAX_MAIL_COUNT (%s) \n", pToName) ;
							continue ;
						}
	
	
	
						//============================================================================
						// 3. Send mail
						u_int uiMailIndex = 0 ;						

						bRes = g_pDBCommand->SendMail( &uiMailIndex
							, pMyInfo->GetName()
							, pMailPublicSendReq->cText
							, static_cast<char>(strlen(pMailPublicSendReq->cText))
							, _MailInfo::_PUBLIC ) ;
						
						if( bRes == false ){
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->SendMail( iRes, pMyInfo->characName, pMailPublicSendReq->cText, pMailPublicSendReq->cStrLen )  \n") ;
							continue ;
						}			
						
						
						
						//============================================================================
						// 4. Mail list update
						memset( &mail, 0, sizeof(_Mail) ) ;
						mail.uiIndex = uiMailIndex ;
						mail.check = false ;
						
						if( pFriendInfo )
						{
							pFriendInfo->AddMail( mail ) ;

							pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;
							g_pBackupReady->Insert( pFriendInfo ) ;


							// 대상이 로그인중이라면 메일이 도착했음을 알린다.
							if( pFriendInfo->IsLogon() )
							{
								msgMailRecv.uiIndex = mail.uiIndex ;
								iRes = pFriendInfo->SendMsg( (char*)&msgMailRecv, msgMailRecv.header.sLength, _SEND_MODE_ME_ ) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;
								}
							}
						}
						else
						{
	
							pMailList->push_back( mail ) ;
	
							bRes = g_pDBCommand->UpdateMailList( uid, pMailList ) ;
							if( bRes == false ) {
								g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->UpdateMailList() \n") ;								
								continue ;
							}
	
						}
					}	// for(
					
					// 4. result send
					if ( pDSComm ) {					
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailSendRes.header.sLength, (char*)&msgMailSendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
					
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S : \n") ;
				}
				}
				break ;

			case MMSG_NO_MAIL_DROP_C_S :
				{
					MMSG_MAIL_DROP * pMsgMailDrop = (MMSG_MAIL_DROP*)pack->buf ;

					pMyInfo->MailDrop( pMsgMailDrop->uiIndex, g_pDBCommand ) ;
					
					pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;														
					g_pBackupReady->Insert( pMyInfo );

				}
				break;

			default :
					// 여기까지 왔다면 비정상적인 패킷.. 비정상적인 패킷을 보낸 소켓은 제거하자.. 썅
					if(pDSComm) {
						::PrintConsole("[WORK_ERR] 비정상 적인 패킷 수신/ 소켓을 절단했음 \n" ) ;
						pDSComm->DisConnect() ;
					}
					

			}	// switch
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_workerthread \n") ;
		}


			

	} // while

	delete g_pDBCommand ;
	delete g_pMailList ;


	::PrintConsole( "[WARNING] th_WorkThread Exit \n") ;
	g_pSystemLog->LogFileWrite( "[WARNING] th_WorkThread Exit \n") ;
	return 0 ;

	
}



DWORD	WINAPI	th_ChatWorkThread( LPVOID lp) 
{

		

	_packet *	pack = NULL ;
	int			Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;
	
	
	_FriendInfo * pMyInfo = NULL ;	
	_FriendInfo * pFriendInfo = NULL ;	
	list<_Member> * pMyFriendList = NULL ;
	list<_Member>::iterator memberListIter ;
	//char	*	pMyCharacName = NULL;

	// GM MAIL 을 보내기 위해 필요한 것들 ==============
	list<_Mail> * g_pMailList = new list<_Mail>;
	MMSG_MAIL_FAIL msgMailFail = {0} ;
	msgMailFail.header.sLength = sizeof(MMSG_MAIL_FAIL) ;
	msgMailFail.header.sCrypto = false ;
	msgMailFail.header.sCompressed = false ;
	msgMailFail.cMessage = MMSG_NO_MAIL_FAIL_S_C ;
	
	MMSG_MAIL_RECV msgMailRecv = {0} ;
	msgMailRecv.header.sLength = sizeof(MMSG_MAIL_RECV) ;
	msgMailRecv.header.sCrypto = false ;
	msgMailRecv.header.sCompressed = false ;
	msgMailRecv.cMessage = MMSG_NO_MAIL_RECV_S_C ;
	
	MMSG_MAIL_SEND_RES msgMailSendRes = {0} ;
	msgMailSendRes.header.sLength = sizeof(MMSG_MAIL_SEND_RES) ;
	msgMailSendRes.header.sCrypto = false ;
	msgMailSendRes.header.sCompressed = false ;
	msgMailSendRes.cMessage = MMSG_NO_MAIL_SEND_RES_S_C ;
	
	list<_Mail> * pMailList = NULL ;
	list<_Mail>::iterator mailListIter ;
	//==================================================

	
	int			iRes = 0 ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;
		

	MMSG_CHAT_RESULT	msgChatResult = {0} ;
	msgChatResult.sLength = sizeof(MMSG_CHAT_RESULT) ;
	msgChatResult.cMessage = MMSG_CHAT_RESULT_S_C ;


	COLE_DB_Executer *	g_pDBCommand = new COLE_DB_Executer(g_pDBCon) ;
	if(g_pDBCommand == NULL )
	{	
		g_pSystemLog->LogFileWrite( "th_WorkThread _j_DB_Command Initalize failed\n" ) ;
		return 0 ;
	}	
	
		
	DWORD			dwLastChatTime = 0 ;


	while (g_pThreadRunning) {
		if( g_pChatPacketQueue->empty() ) {			
			WaitForSingleObject( hTimer, INFINITE ) ;
			continue ;
		}
		else {
			pack = g_pChatPacketQueue->get() ;			
			if( pack == NULL ) {
				WaitForSingleObject( hTimer, INFINITE ) ;
				continue ;
			}
			else {
				Mainmsg = (u_char)pack->buf[2] ;

				try{
					// 쓰레드 전역에서 쓰일 포인터들이다..
					pDSComm = pack->pDSComm ;
					if(!pDSComm){
						::PrintConsole("[ERROR] 'pDSComm' is NULL \n") ;
						continue ;
					}				
					//pMyCharacName= pDSComm->m_pFriendInfo->GetName() ;		// 캐릭이름	
					if( !pDSComm->m_pFriendInfo ) {
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] 'CHAT:pDSComm->m_pFriendInfo' is NULL \n") ;
						continue ;
					}
					pMyInfo = pDSComm->m_pFriendInfo ;		// 자기 정보				
					pMyFriendList = pMyInfo->GetFriendListPtr() ;				// 친구들 정보 리스트

					//if( !pMyInfo || !pMyFriendList )
					//{
					//	pDSComm->releaseDSCom() ;
					//	::PrintConsole("[ERROR] '!pMyInfo || !pMyFriendList' is NULL \n") ;
					//	continue ;
					//}

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] g_pChatPacketQueue->get() ; \n") ;
					continue ;
				}
			}
		}

		dwLastChatTime = timeGetTime() ;


		try{
			switch(Mainmsg) {
			case MMSG_NO_CHAT:
				MMSG_CHAT * pMsgChat ;
				pMsgChat = (MMSG_CHAT*)pack->buf ;
				pMsgChat->cFromCharacName[12] = 0 ;
				pMsgChat->cToCharacName[12] = 0 ;
				pMsgChat->cChatMsg[80] = 0 ;

				if( pMyInfo == NULL )
					continue ;
				

				try{

//#ifdef _TRACE_
//					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_CHAT' packet <%s>\n",pMyInfo->GetName()  ) ;					
//#endif 
					

					pMyInfo->SetMyChatFriend( pMsgChat->cToCharacName ) ;	

					pFriendInfo = pMyInfo->GetMyChatFriend() ;
					
					if ( pFriendInfo ) {	// 로그인 여부만 검사..
						
						/*
						if( pFriendInfo->GetState() == _CHARAC_STATE_LOGOFF 
							|| pFriendInfo->GetState() == _CHARAC_STATE_MOVING		)
						{
#ifdef _TRACE_
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[ERROR] (CHAT) 로그오프중 입니다. \n") ;
#endif
							pMyInfo->SetMyChatFriend( NULL ) ;

							
							memcpy( msgChatResult.cToCharacName, pMsgChat->cToCharacName, 13 ) ;
							msgChatResult.cCode = _NOT_FIND_USER_ ;
							
							iRes = pMyInfo->SendMsg( (char*)&msgChatResult, msgChatResult.sLength, _SEND_MODE_ME_ ) ;
							if( iRes != 0 )
							{
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}


							break;
						}						
						*/

						if( pFriendInfo->dwLastChatTime + 1000 > dwLastChatTime )
						{
							pFriendInfo->dwLastChatTime = dwLastChatTime ;
							++pFriendInfo->dwBadChatCount ;
							if( pFriendInfo->dwBadChatCount > 10 )
							{
								pDSComm->DisConnect() ;
							}							
							continue ;
						}
						
						pFriendInfo->dwBadChatCount = 0 ;
						pFriendInfo->dwLastChatTime = dwLastChatTime ;
						
						try{
							
							if ( pFriendInfo->IsLogon() ) {						

								// 친구에게 채팅메세지 발송
								iRes = pFriendInfo->SendMsg( (char*)pMsgChat, pMsgChat->sLength, _SEND_MODE_ME_ ) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;
								}								
							}							
							
							else
							{

								
								memcpy( msgChatResult.cToCharacName, pMsgChat->cToCharacName, 13 ) ;
								msgChatResult.cCode = _NOT_FIND_USER_ ;
								
								iRes = pMyInfo->SendMsg( (char*)&msgChatResult, msgChatResult.sLength, _SEND_MODE_ME_ ) ;
								if( iRes != 0 )
								{
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;

								}
								
							}
							
							
							
						}
						catch (...) {
							::PrintConsole("[EXCEPTION] Chat send exception error \n") ;
							pMyInfo->SetMyChatFriend( NULL ) ;
						}
						
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_CHAT: \n ") ;
				}
										
				break ;


			case MMSG_NO_CHAT_GM:		// GM 과 User 의 채팅이다..
				MMSG_CHAT * pMsgChatGM ;
				pMsgChatGM = (MMSG_CHAT*)pack->buf ;			

				try{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'GM CHAT: MMSG_CHAT' packet <%s> -> <%s>\n"
						,pMyInfo->GetName(), pMsgChatGM->cToCharacName  ) ;
#endif 	

					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgChatGM->cToCharacName ) ;
					
										
					if ( pFriendInfo && pFriendInfo->IsLogon() ) {

						// 친구에게 채팅메세지 발송
						iRes = pFriendInfo->SendMsg( (char*)pMsgChatGM, pMsgChatGM->sLength, _SEND_MODE_ME_ ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							continue ;
						}		
						
					}
					else
					{
						::PrintConsole("[ERROR] GM CHAT : if ( pFriendInfo && pFriendInfo->IsLogon() ) chr:%s\n", pMsgChatGM->cToCharacName ) ;
					}
					
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] Chat send exception error \n") ;					
				}

				break ;

			case MMSG_NO_CHAT_WHISPER :	// 귓속말 
				try{
					MMSG_CHAT * pMsgChatWhisper = (MMSG_CHAT*)pack->buf ;
					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgChatWhisper->cToCharacName ) ;


					// 상대가 로그인일때만 보낸다.
					if( pFriendInfo && pFriendInfo->IsLogon() )
					{

						if( pFriendInfo->dwLastChatTime + 1000 > dwLastChatTime )
						{
							pFriendInfo->dwLastChatTime = dwLastChatTime ;
							++pFriendInfo->dwBadChatCount ;
							if( pFriendInfo->dwBadChatCount > 10 )
							{
								pDSComm->DisConnect() ;
							}							
							continue ;
						}
						
						pFriendInfo->dwBadChatCount = 0 ;
						pFriendInfo->dwLastChatTime = dwLastChatTime ;


						// 상대가 GM 일때..
						if( pFriendInfo->IsGm() )
						{
							// GM 에게 귓말을 시도하면.. 시스템 메시지로 되돌려준다.. 							
							pMsgChatWhisper->sLength = sizeof(MMSG_CHAT) ;
							sprintf( pMsgChatWhisper->cChatMsg,  _GM_RETURN_MESSAGE )  ;							
							memcpy( pMsgChatWhisper->cToCharacName, pMsgChatWhisper->cFromCharacName, 13 ) ;
							sprintf( pMsgChatWhisper->cFromCharacName, "[SYSTEM]" ) ;
							
//#ifdef _TRACE_
//							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[CHAT_WHISPER] %s -> %s : %s \n", pMsgChatWhisper->cFromCharacName, pMsgChatWhisper->cToCharacName, pMsgChatWhisper->cChatMsg ) ;
//#endif
							// 친구에게 채팅메세지 발송
							iRes = pMyInfo->SendMsg( (char*)pMsgChatWhisper, pMsgChatWhisper->sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}		
							
						}
						else
						{
//#ifdef _TRACE_
//							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[CHAT_WHISPER] %s -> %s : %s \n", pMsgChatWhisper->cFromCharacName, pMsgChatWhisper->cToCharacName, pMsgChatWhisper->cChatMsg ) ;
//#endif
							// 친구에게 채팅메세지 발송
							iRes = pFriendInfo->SendMsg( (char*)pMsgChatWhisper, pMsgChatWhisper->sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}		

						}

					}					
					else	// 상대가 로그오프중일때..
					{

						memcpy( msgChatResult.cToCharacName, pMsgChatWhisper->cToCharacName, 13 ) ;
						msgChatResult.cCode = _NOT_FIND_USER_ ;
						
						iRes = pMyInfo->SendMsg( (char*)&msgChatResult, msgChatResult.sLength, _SEND_MODE_ME_ ) ;
						if( iRes != 0 )
						{
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] MMSG_NO_CHAT_WHISPER \n") ;					
				}
				
				break;
				

			case MMSG_NO_MAIL_SEND_REQ_FROM_GM :
				try
				{
					MMSG_MAIL_SEND_REQ * pMsgMailSendReq = (MMSG_MAIL_SEND_REQ*)(pack->buf) ;

#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'GM MAIL: MMSG_NO_MAIL_SEND_REQ_FROM_GM' packet <%s> -> <%s>\n"
						,pMyInfo->GetName(), pMsgMailSendReq->cToCharac  ) ;
#endif 	
					
					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgMailSendReq->cToCharac ) ;
										
					
					bool bRes = false ;
					int uid = 0;
					
					//============================================================================
					// 1. Mail list 읽어오기..
					// 데이타 있다.
					if( pFriendInfo )
					{
						pMailList = pFriendInfo->GetMailListPtr() ;
						if( pMailList == NULL )
						{
							::PrintConsole("[error] MMSG_NO_MAIL_SEND_REQ_C_S::pMailList == NULL \n") ;
							continue ;
						}
					}
					// 데이타 없다.
					else
					{	
						// list<_Mail> mailList ;
						pMailList = g_pMailList ;
						pMailList->clear() ;
						
						bRes = g_pDBCommand->GetOffMailList( pMsgMailSendReq->cToCharac, &uid, pMailList) ;
						if( bRes == false ){
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->GetMailList() (%s) \n", pMsgMailSendReq->cToCharac ) ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
							msgMailFail.cResult = 0 ;
							if ( pDSComm ) {
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}					
							continue ;
						}
					}

					//============================================================================
					// 2. check size
					if( pMailList->size() >= MAX_MAIL_COUNT ) {
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
						msgMailFail.cResult = 1 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}							
						continue ;
					}



					//============================================================================
					// 3. Send mail
					u_int uiMailIndex = 0 ;

					if( pMyInfo->IsGm() )
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMsgMailSendReq->cText, pMsgMailSendReq->cStrLen, _MailInfo::_FROM_GM ) ;
					}
					else
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMsgMailSendReq->cText, pMsgMailSendReq->cStrLen, _MailInfo::_PRIVATE ) ;
					}


					if( bRes == false ){
						g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->SendMail( iRes, pMyInfo->characName, pMailSendReq->cText, pMailSendReq->cStrLen )  \n") ;
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
						msgMailFail.cResult = 3 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}						
						continue ;
					}			
					
					
					
					//============================================================================
					// 4. Mail list update
					_Mail mail ;
					memset( &mail, 0, sizeof(_Mail) ) ;
					mail.uiIndex = uiMailIndex ;
					mail.check = false ;
					
					if( pFriendInfo )
					{
						
						pFriendInfo->AddMail( mail ) ;
															
						pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;						
						g_pBackupReady->Insert( pFriendInfo ) ;
												
						
						// 대상이 로그인중이라면 메일이 도착했음을 알린다.
						if( pFriendInfo->IsLogon() )
						{
							msgMailRecv.uiIndex = mail.uiIndex ;
							iRes = pFriendInfo->SendMsg( (char*)&msgMailRecv, msgMailRecv.header.sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}
						}
					}
					else
					{

						pMailList->push_back( mail ) ;

						bRes = g_pDBCommand->UpdateMailList( uid, pMailList ) ;
						if( bRes == false ) {
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->UpdateMailList() \n") ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
							msgMailFail.cResult = 3 ;
							if ( pDSComm ) {					
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}							
							continue ;
						}

					}									


					// 4. result send
					if ( pDSComm ) {					
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailSendRes.header.sLength, (char*)&msgMailSendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) 
				{
					::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
				}
				break ;
				
			default :
				// 여기까지 왔다면 비정상적인 패킷.. 비정상적인 패킷을 보낸 소켓은 제거하자.. 썅
				if(pDSComm) {
					::PrintConsole("[WORK_ERR] 비정상 적인 패킷 수신/ 소켓을 절단했음 \n" ) ;
					pDSComm->DisConnect() ;
				}
			}//switch
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_ChatWorkThread \n") ;
		}

		

	}


	::PrintConsole( "[WARNING] th_ChatWorkThread Exit \n") ;
	g_pSystemLog->LogFileWrite( "[WARNING] th_ChatWorkThread Exit \n") ;
	return 0 ;
	
		
}

extern int g_iCurLoginMsgNum ;
extern int g_iCurLoginMsgPos ;

DWORD WINAPI th_LoginWorkThread( LPVOID lp) 
{

	
	COLE_DB_Executer *		g_pDBCommand ;
	// ole-db	
	g_pDBCommand = new COLE_DB_Executer( g_pDBCon );
	if(g_pDBCommand == NULL ){	
		g_pSystemLog->LogFileWrite( "th_LoginWorkThread _j_DB_Command Initalize failed\n" ) ;
		return 0 ;
	}

	_packet *	pack = NULL ;
	int			Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;
	
	_FriendInfo * pMyInfo = NULL ;	
	_FriendInfo * pFriendInfo = NULL ;	

	list<_Member> * pMyFriendList = NULL ;
	list<_Member>::iterator memberListIter ;	
	list<_Member>::iterator FriendsListIter ;	
	//char	*	pMyCharacName = NULL;
	
	int			iRes = 0 ;
	bool		bRes = false ;

	
	int & pos = g_iCurLoginMsgPos ;

	
	MMSG_MSLOGIN_RESULT	msgLoginResult = {0} ;
	msgLoginResult.sLength = sizeof(MMSG_MSLOGIN_RESULT) ;
	msgLoginResult.sCrypto = false ;
	msgLoginResult.sCompressed = false ;
	msgLoginResult.cMessage = MMSG_NO_LOGIN_RESULT ;

	MMSG_LOGOUT_REPORT	msgLogoutReport = {0} ;
	msgLogoutReport.sLength = sizeof(MMSG_LOGOUT_REPORT) ;
	msgLogoutReport.sCrypto = false ;
	msgLogoutReport.sCompressed = false ;
	msgLogoutReport.cMessage = MMSG_NO_LOGOUT_REPORT_S_C ;


	MMSG_FRIEND_STATE_REPORT msgFriendStateReport = {0} ;
	msgFriendStateReport.sLength = sizeof(MMSG_FRIEND_STATE_REPORT) ;
	msgFriendStateReport.sCrypto = false ;
	msgFriendStateReport.sCompressed = false ;
	msgFriendStateReport.cMessage = MMSG_NO_FRIEND_STATE_REPORT_S_C ;
	msgFriendStateReport.cState = _CHARAC_STATE_LOGOFF ;


	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;
	
	
	while (g_pThreadRunning) {
		
		try{
			pack = g_pLoginPacketQueue->get() ;
			if( pack == NULL ) {
				WaitForSingleObject( hTimer, INFINITE ) ;
				pos = -1 ;
				g_iCurLoginMsgNum = Mainmsg = 0 ;
				continue ;
			}
			else {
				g_iCurLoginMsgNum = Mainmsg = (u_char)pack->buf[2] ;				
				
				// 포인터 초기화..
				//pMyCharacName = NULL;
				pMyInfo = NULL ;	
				pFriendInfo = NULL ;					
				pMyFriendList = NULL ;
				
				// 쓰레드 전역에서 쓰일 포인터들이다..
				pDSComm = pack->pDSComm ;
				if(!pDSComm) {
					::PrintConsole("[ERROR] LoginWorkThread 'pDSComm' is NULL \n") ;
					continue ;
				}
				
				pos = -2 ;
				
			}		
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
		
		try{
			switch(Mainmsg) {
			
			case MMSG_NO_LOGIN_C_S :

				pos = -3 ;


				MMSG_MSLOGIN * pMsgMSLogin ;
				pMsgMSLogin = (MMSG_MSLOGIN*)pack->buf ;

				int uid ;
				
				pMsgMSLogin->cCharacName[12] = 0 ;
				try{
					if( pMsgMSLogin->cCharacName[0] == 0 || pMsgMSLogin->cCharacName[0] ==' '){
						if( pDSComm )
							pDSComm->DisConnect() ;
						continue ;
					}

					pos = -4 ;

					if( !pDSComm->m_pNetIO ) {
						if( pDSComm )
							pDSComm->DisConnect() ;
						continue ;
					}

					pos = 1 ;
					
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_LOGIN, "[INFO] Recv 'MMSG_MSLOGIN' (CharacName : %s) \n", pMsgMSLogin->cCharacName) ;
#endif

					// Login 검사..			
					uid = g_pDBCommand->GetUID(pMsgMSLogin->cCharacName) ;
					if( uid == 0 ) {
						::PrintConsole("[ERROR] 'g_pDBCommand->Login(%s)' Failed \n", pMsgMSLogin->cCharacName) ;
						msgLoginResult.cResult = 4 ;
						// 로그인 결과							
						iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;					
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;							
						}

						pDSComm->DisConnect() ;

						continue ;
					}

					pos = 2 ;

					
					pMyInfo = g_pMemberQueue->FindNode(pMsgMSLogin->cCharacName) ;

					pos = 1000 ;
									
					if( !pMyInfo ) {
						
						pMyInfo = g_pMemberQueue->GetNode(pMsgMSLogin->cCharacName) ;
						pos = 3 ;
						if( !pMyInfo ) {
							
							pos = 4 ;

							::PrintConsole("[ERROR] Login Failed C<%s> 'pMyInfo == NULL' \n", pMsgMSLogin->cCharacName ) ;
							msgLoginResult.cResult = 4 ;
							// 로그인 결과					
							if( pDSComm->m_pNetIO )
							{
								iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								}
							}							

							pDSComm->DisConnect() ;	
															
							continue ;
						}
						

						pos = 5 ;
						

						if( pMyInfo->GetFriendListPtr() == NULL || pMyInfo->GetMailListPtr() == NULL ){
							pDSComm->DisConnect() ;	
							continue ;
						}
						
						// 여기부터.. 데이타 로딩이 완료될때까지 락이다~~						
						pos = 6 ;
						
						
						pMyInfo->SetUniqueID( uid ) ;						

						if( pMyInfo->ReadFromDB( g_pDBCommand ) == false ) 
						{
							
							::PrintConsole("[ERROR] Login Failed 'pMyInfo->ReadFromDB' charac : %s \n"
								, pMsgMSLogin->cCharacName) ;
							
							// 로그인 결과
							msgLoginResult.cResult = 4 ;
							if( pDSComm->m_pNetIO )
							{
								iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d \n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;
								}
							}
														
							pos = 7 ;
							
							//DISCONNECT MESSAGE
							pDSComm->m_pFriendInfo = NULL ;
							pDSComm->DisConnect() ;
							//Disconnect(pDSComm) ;						
													
							
							g_pMemberQueue->PutNode( pMyInfo ) ;
							

							pos = 8 ;
							
							// Goto First Loop
							continue ;
						}
						pos = 9 ;
					}

					if( pMyInfo->GetName()[0] == 0 )
					{
						g_pMemberQueue->DeleteMap( pMsgMSLogin->cCharacName ) ;
						::PrintConsole( "[ERROR] %s, %d \n", __FILE__, __LINE__  ) ;


						msgLoginResult.cResult = 4 ;
						if( pDSComm->m_pNetIO )
						{
							iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d (name:%s)\n", __FILE__, __LINE__, pMsgMSLogin->cCharacName ) ;
								pDSComm->DisConnect() ;
								continue ;
							}
						}


						continue ;
					}

					if( pMyInfo->IsLogon() ) {	// 이미 로그인중..

						pos = 10 ;


						::PrintConsole( "[LOGIN] Already loginer (%s/%s)\n", pMyInfo->GetName(), pMsgMSLogin->cCharacName ) ;
						// 두놈다 끊는다..
						// 로그 오프상태로 전환...
						pMyInfo->DisconnectForLogout() ;
						
						//pDSComm->DisConnect() ;				// 지금 들어온 놈은 그냥 끊기만..
						

						if( strcmp( pMyInfo->GetName(), pMsgMSLogin->cCharacName ) != 0 )
						{
							g_pMemberQueue->DeleteMap( pMsgMSLogin->cCharacName ) ;
							::PrintConsole( "[ERROR] if( strcmp( pMyInfo->GetName(), pMsgMSLogin->cCharacName ) != 0 ) \n" ) ;


							msgLoginResult.cResult = 4 ;
							if( pDSComm->m_pNetIO )
							{
								iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d (name:%s)\n", __FILE__, __LINE__, pMsgMSLogin->cCharacName ) ;
									pDSComm->DisConnect() ;
									continue ;
								}
							}

							
							continue ;
						}
						else
						{
							// 올 백업이다..
							pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
							pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;
							g_pBackupReady->Insert( pMyInfo ) ;
						}

						//continue ;
						
					}
					
					pos = 11 ;

					
					try{						
						pMyInfo->Login( pDSComm ) ;

						pos = 12 ;
						
						// 로그인한 친구들을 찾아 정보를 갱신한다..						
						pMyInfo->AllFriendsLink() ;
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] \n") ;
					}
					

					pos = 14 ;
					
					// 로그인 결과
					msgLoginResult.cResult = 0 ;
					if( pDSComm->m_pNetIO )
					{
						iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d (name:%s)\n", __FILE__, __LINE__, pMsgMSLogin->cCharacName ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
					
			
					pos = 15 ;

#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Login Success (CharacName : %s) \n", pMsgMSLogin->cCharacName) ;					
					SYSTEMTIME	time ;
					GetLocalTime(&time ) ;

#ifdef _LOGIN_FILE_LOG_WRITE_
					g_pLoginLog->LogFileWrite("[%d:%d:%d][LOGIN_SUCCESS] CHR_NAME : %s \n", time.wHour, time.wMinute, time.wSecond, pMsgMSLogin->cCharacName ) ;					
#endif

#endif
				}
				catch (...) {										
					::PrintConsole("[EXCEPTION] case MMSG_NO_LOGIN_C_S : %d \n ", pos) ;
					if( pDSComm )
						pDSComm->DisConnect() ;

				}

				break;

			case MMSG_NO_GM_LOGIN :
				MMSG_GM_LOGIN * pMsgGmLogin ;
				pMsgGmLogin = (MMSG_GM_LOGIN*)pack->buf ;
				pMsgGmLogin->cCharacName[12] = 0 ;

				try{
					if( pMsgGmLogin->cCharacName[0] == 0 || pMsgGmLogin->cCharacName[0] ==' '){
						if( pDSComm )
							pDSComm->DisConnect() ;
						continue ;
					}
					
					
					
					pMyInfo = g_pMemberQueue->FindNode(pMsgGmLogin->cCharacName) ;				
					
					
					if( !pMyInfo ) {
					
						pMyInfo = g_pMemberQueue->GetNode(pMsgGmLogin->cCharacName) ;
						
						if( !pMyInfo ) {
							::PrintConsole("[ERROR] MMSG_NO_GM_LOGIN '!pMyInfo' \n") ;
							continue ;
						}
					}
					
					
					if( pMyInfo->IsLogon() ) {	// 이미 로그인중..
						
						// 두놈다 끊는다..												
						//pMyInfo->Logout() ;
						pDSComm->DisConnect() ;
						//pMyInfo->ds_com = NULL ;

						// 올 백업이다..
						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;
						g_pBackupReady->Insert( pMyInfo ) ;
						
						::PrintConsole("[ERROR] Login Failed 'Already login' \n") ;
						
						continue ;
					}					
					
					pMyInfo->Login( pDSComm ) ;
										

					::PrintConsole("[INFO] GM Login success (name : %s)\n ", pMsgGmLogin->cCharacName ) ;

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_GM_LOGIN \n") ;
				}			


				break;



			case MMSG_NO_LOGOUT_C_S :

				MMSG_MSLOGOUT * pMsgMSLogout ;
				pMsgMSLogout = (MMSG_MSLOGOUT*)pack->buf ;

				pos = 0 ;


				try{
					
					if (pDSComm->m_pFriendInfo) {
						pMyInfo = (_FriendInfo*)pDSComm->m_pFriendInfo ;
					}
					else {
						pDSComm->DisConnect() ;
						
						// Goto First Loop				
						continue ;
					}

					if( pMyInfo == NULL )
					{
						continue ;
					}

#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_MSLOGOUT' Packet <%s> \n", pMyInfo->GetName() ) ;
#endif
					
						
					pos = 1 ;
					
					pMyFriendList = pMyInfo->GetFriendListPtr() ;
					if( pMyFriendList == NULL ) {
												
						g_pMemberQueue->PutNode(pMyInfo) ;
						
						// Goto First Loop				
						continue ;
					}
					
					pos = 2 ;
					
					//pMyInfo->Logout() ;						
					pDSComm->DisConnect() ;

					pos = 3 ;					
					
					// 올 백업이다..
					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;
					g_pBackupReady->Insert( pMyInfo ) ;
					
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Logout success (%s) \n", pMyInfo->GetName()) ;
#endif
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_LOGOUT_C_S : pos.%d\n ", pos) ;
				}
				
				break;
								

			default :
					// 여기까지 왔다면 비정상적인 패킷.. 비정상적인 패킷을 보낸 소켓은 제거하자.. 썅
					if(pDSComm) {
						::PrintConsole("[WORK_ERR] 비정상 적인 패킷 수신/ 소켓을 절단했음 \n" ) ;
						pDSComm->DisConnect() ;
					}
			
			
			} //switch
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_LoginWorkThread \n") ;
		}
		
		
		
	} // while

	 
	::PrintConsole( "[WARNING] th_LoginWorkThread Exit \n") ;
	g_pSystemLog->LogFileWrite( "[WARNING] th_LoginWorkThread Exit \n") ;

	delete g_pDBCommand ;

	return 0 ;
}

