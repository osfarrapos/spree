#include "..\stdafx.h"

#include "..\Global\tcp_packets_party.h"
#include "..\Network\Network.h"
#include "..\Packet_Queue\Packet_Queue.h"
#include "..\MapManager\MapManager.h"
#include "..\Global\Global_LogFile.h"
#include "..\Global\Global_ServerInfo.h"
#include "..\CS\party_manager.h"
#include "..\CS\Organization.h"
#include "../CS/MasterPupil.h"


// Packet 처리 Functions
bool GSConnect(char cServerNum, _h_DS_Com * dsCom) ;
bool CreateParty(char cServerNum, short uid1, short uid2) ;

extern _queue_Packet * g_pCSPacketQ ;
extern _queue_Packet * g_pChatPacketQueue ;
extern _j_PartyManager * g_pPartyManager ;
extern _j_ServerInfo * g_pServerInfo ;
extern _h_Party_Timer *	g_pPartyTimer ;
extern _DSCom_Queue *		g_pDSComQueue ;
extern _j_AccMAPManager *		g_pAccMapManager;

extern bool g_Threading ;

#ifdef _PD_MASTER_PUPIL_
extern CDSMasterSet *			g_pMPSet;
#endif

unsigned long __stdcall	th_CSWorkThread( LPVOID lp)
{
	_packet *	pack = NULL ;
	u_char		Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;	
	_Party *	pParty ;
	list<_Member*> * pMemberList = NULL ;	
	list<_Member*>::iterator tempListItr = NULL ;
	//list<_Server> * pServerList = NULL ;
	char		g_cServerNumList[MAX_SERVER_COUNT] ;

	int			iErrCount = 0;


	int			iRes = 0 ;
	short		sRes = 0 ;
	u_short		usRes = 0 ;
	bool		bRes = 0 ;
	int			pos = 0 ;

	_j_PartyManager *  pTempPartyManager = g_pPartyManager;

	//int i ;


	// Result MSG Initalize
	SMSG_CREATE_PARTY_RES_CS_GS msgCreatePartyRes = {0} ;
	msgCreatePartyRes.cMessage = SMSG_NO_CREATE_PARTY_RES_CS_GS ;
	msgCreatePartyRes.usLength = sizeof(SMSG_CREATE_PARTY_RES_CS_GS) ;

	SMSG_JOIN_PARTY_RES_CS_GS	msgJoinPartyRes = {0} ;
	msgJoinPartyRes.cMessage = SMSG_NO_JOIN_PARTY_RES_CS_GS ;
	msgJoinPartyRes.usLength = sizeof(SMSG_JOIN_PARTY_RES_CS_GS) ;

	SMSG_MOVE_PARTY_RES_CS_GS	msgMovePartyRes = {0} ;
	msgMovePartyRes.cMessage = SMSG_NO_MOVE_PARTY_RES_CS_GS ;
	msgMovePartyRes.usLength =  sizeof(SMSG_MOVE_PARTY_RES_CS_GS) ;

	SMSG_ENTRUST_RES_CS_GS		msgEntrustRes = {0} ;
	msgEntrustRes.cMessage  = SMSG_NO_ENTRUST_RES_CS_GS ;
	msgEntrustRes.usLength = sizeof(SMSG_ENTRUST_RES_CS_GS) ;

	SMSG_PARTY_INFO_RES_CS_GS	msgPartyInfoRes = {0} ;
	msgPartyInfoRes.cMessage = SMSG_NO_PARTY_INFO_RES_CS_GS ;
	msgPartyInfoRes.usLength = sizeof(SMSG_PARTY_INFO_RES_CS_GS) ;

	SMSG_DELETE_PARTY_RES_CS_GS	msgDeletePartyRes = {0} ;
	msgDeletePartyRes.cMessage = SMSG_NO_DELETE_PARTY_RES_CS_GS ;
	msgDeletePartyRes.usLength = sizeof(SMSG_DELETE_PARTY_RES_CS_GS) ;

	SMSG_KICK_PARTY_RES_CS_GS	msgKickPartyRes = {0} ;
	msgKickPartyRes.cMessage = SMSG_NO_KICK_PARTY_RES_CS_GS ;
	msgKickPartyRes.usLength = sizeof(SMSG_KICK_PARTY_RES_CS_GS) ;	

	SMSG_UPDATE_STATE		msgUpdateState;
	msgUpdateState.usLength = sizeof(SMSG_UPDATE_STATE);
	msgUpdateState.cMessage = SMSG_NO_UPDATE_STATE;


	while (g_Threading) {
		if( g_pCSPacketQ->empty() ) {

			Sleep(10) ;
			continue ;
		}
		else {
			pack = g_pCSPacketQ->get() ;


			if( pack == NULL ) {
				Sleep(10) ;
				continue ;
			}
			else {
				Mainmsg = static_cast<u_char>(pack->buf[2]) ;
				pDSComm = pack->pDSComm ;
				if( !pDSComm )	// 2004.6.17	ozzywow add
					continue ;
			}
		}


		try{
			switch( Mainmsg ) 
			{							

				//	파티 생성 요청
			case SMSG_NO_CREATE_PARTY_REQ_GS_CS:
				SMSG_CREATE_PARTY_REQ_GS_CS * pMsgCreatePartyReq ;
				pMsgCreatePartyReq = (SMSG_CREATE_PARTY_REQ_GS_CS *)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[RECV] SMSG_NO_CREATE_PARTY_REQ_GS_CS \n") ;
#endif

				try{

					short sProp = 0 ;
					sRes = g_pPartyManager->CreateParty(pMsgCreatePartyReq->cServerNum, 
						pMsgCreatePartyReq->uid1, pMsgCreatePartyReq->uid2, 
						pMsgCreatePartyReq->cCharacName1, pMsgCreatePartyReq->cCharacName2
						,sProp ) ;			
					msgCreatePartyRes.sMoonpaProp = sProp ;
					msgCreatePartyRes.sPartyIndex = sRes ;
					msgCreatePartyRes.uid1 = pMsgCreatePartyReq->uid1 ;
					msgCreatePartyRes.uid2 = pMsgCreatePartyReq->uid2 ;	



					if( pDSComm->Send(msgCreatePartyRes.usLength, (char*)&msgCreatePartyRes)  == false ){
						::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
					}

#ifdef _TRACE_
					if(sRes == 0) {
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY]Create party failed (Index:%d, chr1:%s chr2:%s )\n",sRes,pMsgCreatePartyReq->cCharacName1, pMsgCreatePartyReq->cCharacName2) ;
					}
					else {
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY]Create party success (Index:%d, chr1:%s chr2:%s )\n",sRes,pMsgCreatePartyReq->cCharacName1, pMsgCreatePartyReq->cCharacName2) ;
					}					
#endif
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_CREATE_PARTY_REQ_GS_CS : \n") ;
				}


				break;

			case SMSG_NO_DELETE_PARTY_REQ_GS_CS:	

				SMSG_DELETE_PARTY_REQ_GS_CS * pMsgDeletePartyReq ;
				pMsgDeletePartyReq = (SMSG_DELETE_PARTY_REQ_GS_CS *)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Recv 'SMSG_NO_DELETE_PARTY_REQ_GS_CS' (index:%d)\n", pMsgDeletePartyReq->sPartyIndex) ;
#endif


				try{
					pParty = g_pPartyManager->FindList(pMsgDeletePartyReq->sPartyIndex) ;
					if( !pParty ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] !pParty == NULL (index : %d )\n", pMsgDeletePartyReq->sPartyIndex) ;
#endif
						continue ;
					}

					bRes = g_pPartyManager->DeleteParty(pParty) ;	// 맵에서 파티리스트를 완전히 삭제..	
					msgDeletePartyRes.sPartyIndex = pMsgDeletePartyReq->sPartyIndex ;

					// 결과 보낼 주소를 얻어옴..					
					memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;

					if ( bRes ) {	// 파티 삭제 성공
						msgDeletePartyRes.cResult = 1 ;				
						for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
							if ( g_cServerNumList[i] == '&' ) {
								break ;
							}
							pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
							if(pDSComm){

								if(pDSComm->Send(msgDeletePartyRes.usLength, (char*)&msgDeletePartyRes) == false){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								}
							}					
						}
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Delete party success (Index%d)\n",pMsgDeletePartyReq->sPartyIndex) ;

#ifdef	_PARTY_LOG_WRITE
						g_pPartyLog->LogFileWrite("[PARTY] Delete party success (Index%d)\n",pMsgDeletePartyReq->sPartyIndex) ;
#endif

#endif

					}
					else{			// 실패
						msgDeletePartyRes.cResult = 0 ;

						if(pDSComm->Send(msgDeletePartyRes.usLength, (char*)&msgDeletePartyRes) == false ){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
						}
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Delete party failed (Index%d)\n",pMsgDeletePartyReq->sPartyIndex) ;

#ifdef	_PARTY_LOG_WRITE
						g_pPartyLog->LogFileWrite("[PARTY] Delete party failed (Index%d)\n",pMsgDeletePartyReq->sPartyIndex) ;
#endif

#endif
					}

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_DELETE_PARTY_REQ_GS_CS : \n") ;
				}				

				break;

			case SMSG_NO_KICK_PARTY_REQ_GS_CS:
				SMSG_KICK_PARTY_REQ_GS_CS * pMsgKickPartyReq ;
				pMsgKickPartyReq = (SMSG_KICK_PARTY_REQ_GS_CS*)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Recv 'SMSG_NO_KICK_PARTY_REQ_GS_CS' index:%d \n", pMsgKickPartyReq->sPartyIndex) ;
#endif			

				try{
					pParty = g_pPartyManager->FindList( pMsgKickPartyReq->sPartyIndex ) ;
					if( !pParty ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] pParty == NULL (index : %d ) \n", pMsgKickPartyReq->sPartyIndex) ;
#endif
						continue ;
					}


					iRes = g_pPartyManager->KickParty(pParty, pMsgKickPartyReq->cSlotNo) ;
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] KieckParty() return value = (%d) / uid(%d), slot(%d)  \n", iRes, pMsgKickPartyReq->uid, pMsgKickPartyReq->cSlotNo ) ;
#endif								

					msgKickPartyRes.sPartyIndex = pMsgKickPartyReq->sPartyIndex ;					
					msgKickPartyRes.uid = pMsgKickPartyReq->uid ;					
					msgKickPartyRes.cResult = (char)iRes ;
					msgKickPartyRes.cMode = pMsgKickPartyReq->cMode ;
					msgKickPartyRes.cSlotNo = pMsgKickPartyReq->cSlotNo ;
					if( iRes == 2)	// 자동 위임..
					{						
						msgKickPartyRes.cCapNo = g_pPartyManager->EntrustCaptain(pParty, 0, 0) ;						
					}
					memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;

					if ( iRes >= 0 ) {	// 탈퇴가 성공했거나, 파티가 깨졌을경우
						for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
							if ( g_cServerNumList[i] == '&' ) {
								break ;
							}
							pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
							if(pDSComm){

								if(pDSComm->Send(msgKickPartyRes.usLength, (char*)&msgKickPartyRes) == false){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								}						
							}
						}
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Kick party success (result:%d,Index:%d, uid:%d) \n",msgKickPartyRes.cResult, pMsgKickPartyReq->sPartyIndex, pMsgKickPartyReq->uid) ;

#ifdef	_PARTY_LOG_WRITE
						g_pPartyLog->LogFileWrite("[PARTY] Kick party success (result:%d,Index:%d, uid:%d) \n",msgKickPartyRes.cResult, pMsgKickPartyReq->sPartyIndex, pMsgKickPartyReq->uid) ;
#endif

#endif
					}
					else {	// 실패

						if(pDSComm->Send(msgKickPartyRes.usLength, (char*)&msgKickPartyRes) == false){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
						}
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Kick party failed (result:%d,Index:%d, uid:%d) \n",msgKickPartyRes.cResult, pMsgKickPartyReq->sPartyIndex, pMsgKickPartyReq->uid) ;

#ifdef	_PARTY_LOG_WRITE
						g_pPartyLog->LogFileWrite("[PARTY] Kick party failed (result:%d,Index:%d, uid:%d) \n",msgKickPartyRes.cResult, pMsgKickPartyReq->sPartyIndex, pMsgKickPartyReq->uid) ;
#endif

#endif
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_KICK_PARTY_REQ_GS_CS : \n") ;
				}

				break;

			case SMSG_NO_JOIN_PARTY_REQ_GS_CS:
				SMSG_JOIN_PARTY_REQ_GS_CS * pMsgJoinPartyReq ;
				pMsgJoinPartyReq = (SMSG_JOIN_PARTY_REQ_GS_CS *) pack->buf ;				

#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[RECV] SMSG_NO_JOIN_PARTY_REQ_GS_CS \n") ;
#endif

				try{
					pParty = g_pPartyManager->FindList( pMsgJoinPartyReq->sPartyIndex ) ;
					if( !pParty ) continue ;

					short sProp = 0 ;
					iRes = g_pPartyManager->JoinParty(pParty, pMsgJoinPartyReq->uid, pMsgJoinPartyReq->cCharacName, sProp ) ;
					msgJoinPartyRes.sMoonpaProp = sProp ;


					msgJoinPartyRes.sPartyIndex = pMsgJoinPartyReq->sPartyIndex ; 
					msgJoinPartyRes.cServerNum = pMsgJoinPartyReq->cServerNum ;
					msgJoinPartyRes.uid = pMsgJoinPartyReq->uid ;
					memcpy( msgJoinPartyRes.cCharacName, pMsgJoinPartyReq->cCharacName, 13 ) ;
					msgJoinPartyRes.cSlotNo = (char)iRes ;
					msgJoinPartyRes.sInnerLevel = pMsgJoinPartyReq->sInnerLevel ;

#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Join party Result (Index:%d, slot:%d, result:%d) \n",pMsgJoinPartyReq->sPartyIndex , msgJoinPartyRes.cSlotNo, iRes) ;

#ifdef	_PARTY_LOG_WRITE
					g_pPartyLog->LogFileWrite("[PARTY] Join party Result (Index:%d, slot:%d, result:%d) \n",pMsgJoinPartyReq->sPartyIndex , msgJoinPartyRes.cSlotNo, iRes) ;
#endif

#endif


					memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;

					if ( iRes >= 0 ) {	// 파티 참가가 성공했을경우
						for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
							if ( g_cServerNumList[i] == '&' ) {
								break ;
							}
							pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
							if(pDSComm){

								if(pDSComm->Send(msgJoinPartyRes.usLength, (char*)&msgJoinPartyRes) == false){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								}
							}					
						}

					}
					else{

						if(pDSComm->Send(msgJoinPartyRes.usLength, (char*)&msgJoinPartyRes) == false){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
						}	
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_JOIN_PARTY_REQ_GS_CS : \n") ;
				}


				break;

			case SMSG_NO_MOVE_PARTY_REQ_GS_CS:
				{				
					SMSG_MOVE_PARTY_REQ_GS_CS * pMsgMovePartyReq ;
					pMsgMovePartyReq = (SMSG_MOVE_PARTY_REQ_GS_CS *) pack->buf ;

					try{
						pParty = g_pPartyManager->FindList( pMsgMovePartyReq->sPartyIndex ) ;
						if ( pParty == NULL ) continue ;
						//pParty->lock() ;
						//memcpy( g_cServerNumList, pParty->cServerNumList, sizeof(g_cServerNumList) ) ;
						//pParty->unlock() ;

						bRes = g_pPartyManager->MoveParty(pParty, pMsgMovePartyReq->cServerNum, 
							pMsgMovePartyReq->uid, pMsgMovePartyReq->cSlotNo) ;			

						if( bRes == false ) {

							msgMovePartyRes.cServerNum = 0 ;
							msgMovePartyRes.uid = pMsgMovePartyReq->uid ;
							msgMovePartyRes.sPartyIndex =pMsgMovePartyReq->sPartyIndex ;
							msgMovePartyRes.cSlotNo = pMsgMovePartyReq->cSlotNo ;	


							if(pDSComm->Send(msgMovePartyRes.usLength, (char*)&msgMovePartyRes)  == false){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}
						}			
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] case SMSG_NO_MOVE_PARTY_REQ_GS_CS : \n") ;
					}
				}
				break;

			case SMSG_NO_PARTY_INFO_REQ_GS_CS:
				{				
					SMSG_PARTY_INFO_REQ_GS_CS * pMsgPartyInfoReq = reinterpret_cast<SMSG_PARTY_INFO_REQ_GS_CS *>(pack->buf);

					msgPartyInfoRes.bCreateAble = pMsgPartyInfoReq->bCreateAble ;

					if( pMsgPartyInfoReq->sPartyIndex < 0 )
					{
						::PrintConsole("[ERROR] SMSG_NO_PARTY_INFO_REQ_GS_CS / if( pMsgPartyInfoReq->sPartyIndex(%d) < 0 ) \n", pMsgPartyInfoReq->sPartyIndex) ;
						continue ;
					}

					pos = 0 ;

					
					try{
						// 이해할 수 없지만 이 부분에서 가끔 exception이 발생한다. 도무지 못찾겠다. ㅠ.ㅠ
						pParty = g_pPartyManager->FindList(pMsgPartyInfoReq->sPartyIndex) ;

						pos = 1 ;
						if ( NULL == pParty ) {				

							SMSG_DELETE_PARTY_RES_CS_GS msgDeletePartyRes = {0} ;
							msgDeletePartyRes.cMessage = SMSG_NO_DELETE_PARTY_RES_CS_GS ;
							msgDeletePartyRes.usLength = sizeof(SMSG_DELETE_PARTY_RES_CS_GS) ;
							msgDeletePartyRes.sPartyIndex = pMsgPartyInfoReq->sPartyIndex ;
							msgDeletePartyRes.cResult = true ;

							pos = 2 ;

							if(pDSComm->Send(msgDeletePartyRes.usLength, (char*)&msgDeletePartyRes)  == false){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}

							pos = 3 ;

							continue ;
						}

						iRes = 0 ;
						_Member * pMember = NULL ;	

						pParty->party_lock();

						try
						{
							for( iRes = 0; iRes < MAX_PARTY_MEMBER; iRes++ ) 
							{
								memset(&msgPartyInfoRes.member[iRes], 0, sizeof(SMSG_PARTY_INFO_RES_CS_GS::IN_MEMBER) ) ;
								pMember = pParty->GetPartyMemberInfo(iRes) ;
								if( pMember && pMember->pCharac )
								{							
									msgPartyInfoRes.member[iRes].uid = pMember->uid ;
									memcpy( msgPartyInfoRes.member[iRes].cCharacName, pMember->pCharac->CHARAC_BASIC.cChrName, 13 ) ;
									msgPartyInfoRes.member[iRes].cServerNum = static_cast<char>(pMember->pCharac->CHARAC_CUR_BASIC.sZone) ;
									msgPartyInfoRes.member[iRes].sInnerLevel = pMember->pCharac->CHARAC_STATE.sInnerLevel ;
									msgPartyInfoRes.member[iRes].cCheck = pMember->cCheck ;
									msgPartyInfoRes.member[iRes].cMoonpa = pMember->pCharac->CHARAC_BASIC.cParty ;
									msgPartyInfoRes.member[iRes].cClass = pMember->pCharac->CHARAC_BASIC.cClass ;
#ifdef _PD_PARTY_INFO_CLASSGRADE_
									msgPartyInfoRes.member[iRes].cClassGrade = pMember->pCharac->CHARAC_BASIC.cClassGrade ;
#endif
									msgPartyInfoRes.member[iRes].iHonor = pMember->pCharac->CHARAC_STATE.iHonor ;
								}						
							}		
						}
						catch( ... )
						{
							if( iErrCount++ % 500 == 0 )
								PrintConsole( "[EXCEPTION] PARTY_INFR_REQ_GS_CS - for(%u)\n", GetLastError() );
						}

						pParty->party_unlock();

						pos = 4 ;

						msgPartyInfoRes.sPartyIndex = pMsgPartyInfoReq->sPartyIndex ;

						if(pDSComm->Send(msgPartyInfoRes.usLength, (char*)&msgPartyInfoRes)  == false){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;				
						}					
						pos = 5 ;

					}
					catch (...) {
						if( iErrCount++ % 1000 == 0 )
							PrintConsole( "[EXCEPTION] PARTY_INFO_REQ_GS_CS : P(%d) <pos. %d>\n", pMsgPartyInfoReq->sPartyIndex, pos ) ;
					}				
				}
				break;

			case SMSG_NO_ENTRUST_REQ_GS_CS :
				{
				
					SMSG_ENTRUST_REQ_GS_CS * pMsgEntrustReq ;
					pMsgEntrustReq = (SMSG_ENTRUST_REQ_GS_CS *)pack->buf ;

					try{
						pParty = g_pPartyManager->FindList( pMsgEntrustReq->sPartyIndex ) ;
						if ( !pParty ) {			
							continue ;
						}

						char cRes = g_pPartyManager->EntrustCaptain(pParty, pMsgEntrustReq->cSlotNo ,pMsgEntrustReq->uid) ;

						msgEntrustRes.sPartyIndex = pMsgEntrustReq->sPartyIndex ;
						msgEntrustRes.uid = pMsgEntrustReq->uid ;
						msgEntrustRes.cSlotNo = cRes ;


						memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;
						if ( cRes >= 0 ) {	
							for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
								if ( g_cServerNumList[i] == '&' ) {
									break ;
								}
								pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
								if(pDSComm){

									if(pDSComm->Send(msgEntrustRes.usLength, (char*)&msgEntrustRes)== false){
										::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									}
								}					
							}
						}
						else {

							if(pDSComm->Send(msgEntrustRes.usLength, (char*)&msgEntrustRes) == false){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}
						}
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] case SMSG_NO_ENTRUST_REQ_GS_CS : \n") ;
					}

					break;	
				}

			case SMSG_NO_CHANGE_ITEM_PROP:
				SMSG_CHANGE_PROP * pMsgChangeItemProp ;
				pMsgChangeItemProp = (SMSG_CHANGE_PROP*)pack->buf ;

				pParty = g_pPartyManager->FindList(pMsgChangeItemProp->sPartyIndex) ;
				if ( !pParty ) {				
					continue ;
				}

				// 아이템 분배 설정 방식 변경..
				pParty->SetExpShareType( pMsgChangeItemProp->cProperty ) ;

				memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;
				try{
					for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
						if ( g_cServerNumList[i] == '&' ) {
							break ;
						}
						pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
						if(pDSComm){

							if(pDSComm->Send(pMsgChangeItemProp->usLength, (char*)pMsgChangeItemProp)== false){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}
						}					
					}
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Change item prop (Index:%d) \n", pMsgChangeItemProp->sPartyIndex ) ;
#endif
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_CHANGE_ITEM_PROP : \n") ;
				}
				break; 

			case SMSG_NO_CHANGE_EXP_PROP:
				SMSG_CHANGE_PROP * pMsgChangeExpProp ;
				pMsgChangeExpProp = (SMSG_CHANGE_PROP*)pack->buf ;

				pParty = g_pPartyManager->FindList(pMsgChangeExpProp->sPartyIndex) ;
				if ( !pParty ) {				
					continue ;
				}

				// 아이템 분배 설정 방식 변경..
				pParty->SetExpShareType( pMsgChangeExpProp->cProperty ) ;

				memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;
				try{
					for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
						if ( g_cServerNumList[i] == '&' ) {
							break ;
						}
						pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
						if(pDSComm){

							if(pDSComm->Send(pMsgChangeExpProp->usLength, (char*)pMsgChangeExpProp)== false){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}
						}					
					}
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Change exp prop (Index:%d) \n", pMsgChangeExpProp->sPartyIndex ) ;
#endif
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_CHANGE_EXP_PROP : \n") ;
				}
				break;


			case SMSG_NO_MOVE_COMPLETE :
				SMSG_MOVE_COMPLETE * pMsgMoveComplete ;
				pMsgMoveComplete = (SMSG_MOVE_COMPLETE*)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Recv Move complete (%d) \n", pMsgMoveComplete->uid) ;
#endif

				pos = 0 ;

				try{
					pParty = g_pPartyManager->FindList(pMsgMoveComplete->sPartyIndex) ;
					if ( !pParty ) {				
						continue ;
					}

					_Member * pMember = pParty->GetPartyMemberInfo( pMsgMoveComplete->cSlotNo) ;

					if( pMember && pMember->uid ==  pMsgMoveComplete->uid )
					{
						msgMovePartyRes.sPartyIndex = pMsgMoveComplete->sPartyIndex ;				
						msgMovePartyRes.uid = pMsgMoveComplete->uid ;
						msgMovePartyRes.cSlotNo = pMsgMoveComplete->cSlotNo ;
						memcpy(msgMovePartyRes.cCharacName, pMember->pCharac->CHARAC_BASIC.cChrName, 13 ) ;
						msgMovePartyRes.cServerNum = static_cast<char>(pMember->pCharac->CHARAC_CUR_BASIC.sZone) ;

						g_pPartyManager->MoveComplete(pMsgMoveComplete->uid, pMsgMoveComplete->cSlotNo) ;

						pos = 2 ;


						// 새로운 서버에 들어왔으므로 서버번호를 추가..
						pParty->SetServer( static_cast<char>(pMember->pCharac->CHARAC_CUR_BASIC.sZone) ) ;


						// 서버 이동 관련 특성상 서버이동이 완료한 후에 서버 리스트를 가져와야 한다..(잘생각해봐ㅡㅡ)
						memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;
						pos = 4 ;

						int i ;
						for( i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
							if ( g_cServerNumList[i] == '&' ) {
								break ;
							}
							pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
							if(pDSComm){								
								if(pDSComm->Send(msgMovePartyRes.usLength, (char*)&msgMovePartyRes) == false){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								}
							}					
						}						

						pos = 5 ;

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] SMSG_NO_MOVE_COMPLETE : Sever move complete (Index:%d, uid:%d) \n"
							, pMsgMoveComplete->sPartyIndex, pMsgMoveComplete->uid ) ;
#endif

					}




					else{
						// 서버 이동 시간이 초과되어 Timer에 의해서 해당 파티원이 삭제된 경우라 볼수 있다.
						msgMovePartyRes.cServerNum = 0 ;	// 서버 번호를 0을 보내면 실패임.

						pos = 6 ;

						if ( pDSComm ) {

							if(pDSComm->Send( msgMovePartyRes.usLength, (char*)&msgMovePartyRes ) == false){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}
						}					

						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] pParty->pMember[pMsgMoveComplete->cSlotNo] == NULL \n" ) ;
					}			



				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_MOVE_COMPLETE : (pos. %d)\n", pos ) ;
				}


				break;

			case SMSG_NO_UPDATE_STATE:
				{
					SMSG_UPDATE_STATE * pMsgUpdateState = reinterpret_cast<SMSG_UPDATE_STATE*>(pack->buf);

					pos = 0 ;

					try{
						pParty = g_pPartyManager->FindList(pMsgUpdateState->sPartyIndex) ;
						if ( !pParty ) 
							continue;

						_Member * pMember = pParty->GetPartyMemberInfo( pMsgUpdateState->slot) ;
						if( pMember )
						{
							// 처리.
							pMember->attribute.ucAbsent = pMsgUpdateState->ucMode ? 1 : 0;

							msgUpdateState.ucMode = pMsgUpdateState->ucMode;
							msgUpdateState.sPartyIndex = pMsgUpdateState->sPartyIndex;
							msgUpdateState.slot = pMsgUpdateState->slot;
							msgUpdateState.usValue = pMsgUpdateState->usValue;

							memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;
							for(int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) 
							{
								if ( g_cServerNumList[i] == '&' ) 
									break ;

								pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
								if(pDSComm)
									pDSComm->Send(msgUpdateState.usLength, (char*)&msgUpdateState);
							}
							
						}
					}
					catch ( ... ) 
					{
						PrintConsole( "[EXCEPTION] case SMSG_NO_UPDATE_STATE(%d)\n", pos );
					}
				}
				break;
				
#ifdef _PD_MASTER_PUPIL_
			case CMSG_NO_MASTER_AND_PUPIL:
				{
					CMSG_MASTER_AND_PUPIL * pMP = reinterpret_cast<CMSG_MASTER_AND_PUPIL*>(pack->buf);
					switch( pMP->ucMP_Mode )
					{
					case en_dg_mp_mode_info_req:
						g_pMPSet->DSProc_MasterInfoReq( static_cast<CMSG_MP_INFO_REQ *>(pMP), pDSComm );
						break;
					case en_dg_mp_mode_add_point:
						g_pMPSet->DSProc_AddPoint( static_cast<CMSG_MP_ADD_POINT*>(pMP) );	
						break;
					case en_dg_mp_mode_add_kill_count:
						g_pMPSet->DSProc_AddKillCount( static_cast<CMSG_MP_ADD_KILL_COUNT*>(pMP) );	
						break;
					case en_dg_mp_mode_create_mp_req:
						{
							CMSG_MP_CREATE_MP_REQ * p = static_cast<CMSG_MP_CREATE_MP_REQ*>(pMP);
							_C_CHARAC * pCharac = g_pAccMapManager->Find_Charac( p->cFromName );

							if( pCharac )
							{
								g_pMPSet->DSProc_CreateMPReq( p, pCharac, pDSComm );
							}
							else
							{
								// 시스템 에러를 보낸다.
								CMSG_MP_CREATE_FAIL pkFail;
								pkFail.SetName( p->cFromName );
								pkFail.iMasterIndex = p->iMasterIndex;
								pkFail.ucFailCode = 250;				// 시스템 에러.

								PrintConsole( "[MP] Create MP Info Fail : Not found user<%s>\n", p->cFromName );

								pDSComm->Send( pkFail.usLength, (char*)&pkFail );
							}
						}
						break;
					case en_dg_mp_mode_notice_connection:
						g_pMPSet->DSProc_NoticeCon( static_cast<CMSG_MP_NOTICE_CONNECTION *>(pMP) );
						break;
					case en_dg_mp_mode_change_master_nick_req:
						g_pMPSet->DSProc_ChangeMasterNick( static_cast<CMSG_MP_CHANGE_MASTER_NICK_REQ*>(pMP) );
						break;
					case en_dg_mp_mode_chat:
						g_pMPSet->DSProc_GroupChat( static_cast<CMSG_MP_CHAT*>(pMP) );
						break;
					case en_dg_mp_mode_gd_apply_for_pupil:
						// 스승을 찾아 보낸다.
						g_pMPSet->DSProc_ApplyForPupil( static_cast<CMSG_MP_GD_APPLY_FOR_PUPIL*>(pMP), pDSComm );
						break;
					case en_dg_mp_mode_apply_for_pupil_error:
						g_pMPSet->DSProc_ApplyForPupilError( static_cast<CMSG_MP_APPLY_FOR_PUPIL_ERROR*>(pMP) );
						break;
					case en_dg_mp_mode_answer_yes:
						g_pMPSet->DSProc_ApplyAnswerYes( static_cast<CMSG_MP_ANSWER_YES*>(pMP), pDSComm );
						break;
					case en_dg_mp_mode_answer_apply_to_pupil_no:
						g_pMPSet->DSProc_ApplyAnswerNo( static_cast<CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO*>(pMP) );
						break;
					case en_dg_mp_mode_complete_join_pupil:
						break;
					case en_dg_mp_mode_apply_error:
						g_pMPSet->DSProc_ApplyError( static_cast<CMSG_MP_APPLY_ERROR*>(pMP) );
						break;
					case en_dg_mp_mode_strike_pupil_req:
						g_pMPSet->DSProc_StrikeReq( static_cast<CMSG_MP_STRIKE_PUPIL_REQ*>(pMP), pDSComm );
						break;
					case en_dg_mp_mode_leave_req:
						g_pMPSet->DSProc_LeaveReq( static_cast<CMSG_MP_LEAVE_REQ*>(pMP), pDSComm );
						break;
					case en_dg_mp_mode_accept_pupil_mode:
						// 없어짐.
						break;
					case en_dg_mp_mode_board_initial_req:
						g_pMPSet->DSProc_BoardInitialReq( static_cast<CMSG_MP_BOARD_INITIAL_REQ*>(pMP), pDSComm );
						break;
					case en_dg_mp_mode_board_req_page:
						g_pMPSet->DSProc_BoardPageReq( static_cast<CMSG_MP_BOARD_REQ_PAGE*>(pMP), pDSComm );
						break;
					case en_dg_mp_mode_del_from_memory:
						break;

					}

				}
				break;
#endif
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_WorkThread \n") ;
		}



	}

	printf("[THREAD] th_CSWorkThread Exit thread \n") ;
	return 0 ;

}



unsigned long __stdcall th_CSChatWorkThread( LPVOID lp) 
{


	_packet *	pack = NULL ;
	u_char		Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;



	_Party * pParty = NULL ;
	char		g_cServerNumList[MAX_SERVER_COUNT] ;


	int			iRes = 0 ;



	while (g_Threading) {


		if( g_pChatPacketQueue->empty() ) {

			Sleep(10) ;
			continue ;
		}
		else {
			pack = g_pChatPacketQueue->get() ;
			if( pack == NULL ) {
				Sleep(10) ;
				continue ;
			}
			else {
				Mainmsg = static_cast<u_char>(pack->buf[2]) ;
				pDSComm = pack->pDSComm ;
				if( !pDSComm )
					continue ;
			}
		}

		try{
			switch(Mainmsg) {
			case SMSG_NO_PARTY_CHAT:
				SMSG_PARTY_CHAT * pMsgPartyChat ;
				pMsgPartyChat = (SMSG_PARTY_CHAT*)pack->buf ;

				try{
					pParty = g_pPartyManager->FindList(pMsgPartyChat->sPartyIndex) ;
					if ( !pParty ) {			

						if(pDSComm->Send(pMsgPartyChat->usLength, (char*)pMsgPartyChat) == false){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
						}			
						g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[PARTY] SMSG_NO_PARTY_CHAT : pParty == NULL \n") ;
						continue ;
					}

					memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;


					for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
						if ( g_cServerNumList[i] == '&' ) {
							break ;
						}
						pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
						if(pDSComm){

							if(pDSComm->Send(pMsgPartyChat->usLength, (char*)pMsgPartyChat) == false){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}
						}					
					}				
				}
				catch ( ...	) {
					::PrintConsole("[EXCEPTION] case SMSG_NO_PARTY_CHAT : \n") ;
				}				
				break;

			case SMSG_NO_OR_CHAT :
				SMSG_OR_CHAT * pMsgORChat = (SMSG_OR_CHAT*)pack->buf ;			
				g_pDSComQueue->GlobalSend( pMsgORChat->usLength, (char*)pMsgORChat ) ;
				break ;
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_ChatWorkThread \n");
		}



	}

	printf("[THREAD] th_CSChatWorkThread Exit thread \n") ;


	return 0 ;


}