#include "..\stdafx.h"

#include "..\Global\tcp_packets_party.h"
#include "..\Network\Network.h"
#include "..\Network\LogSvrLink.h"
#include "..\Packet_Queue\Packet_Queue.h"
#include "..\MapManager\MapManager.h"
#include "..\Global\Global_LogFile.h"
#include "..\Global\Global_ServerInfo.h"
#include "..\CS\Organization.h"
#include "../CS/Castle.h"
#ifdef _PD_CASTLE_STEP_2_
#include "../Cs/TaxSystem.h"
#endif



extern _DSCom_Queue *		g_pDSComQueue ;
extern COrganizationManager *	g_pOrganizationManager ;
#ifdef _PD_ALLY_ORG_STEP_1_
extern CAllyManager *		g_pAllyManager;
#endif
extern _j_ServerInfo * g_pServerInfo ;
extern CLogSvr_Com *		g_pLogSvrCom;

extern bool g_Threading ;

unsigned long __stdcall	th_ORWorkThread( LPVOID lp)
{
	_packet *	pack = NULL ;
	u_char		Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;	

	_queue_Packet * pORPacketQ = (_queue_Packet*)lp ;

	int			pos = 0 ;
	

	
	while (g_Threading) {
		if( pORPacketQ->empty() ) {

			Sleep(10) ;
			continue ;
		}
		else {
			pack = pORPacketQ->get() ;


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
			pos = Mainmsg ;
			switch( Mainmsg ) 
			{
			case SMSG_NO_OR_SOME_REQ_GS_DS :
			
				try{
					SMSG_OR_SOME_REQ_GS_DS * pMsgORSomeReq = (SMSG_OR_SOME_REQ_GS_DS*)pack->buf ;
					switch( pMsgORSomeReq->cCode )
					{
					case SMSG_OR_SOME_REQ_GS_DS::_OR_SOME_REQ_INFO_ :
						{
							g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ORG] Send ORG Info : %d - ", pMsgORSomeReq->iIndex );

							if( _Organization * pOr = g_pOrganizationManager->FindOrganization( pMsgORSomeReq->iIndex ) )
							{	
								g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Find\n" );

#ifdef _PD_ALLY_ORG_STEP_1_
								int allyIndex = pOr->m_iAllyIndex;
								if( allyIndex )
								{
									CAllyOrganization * pAlly = g_pAllyManager->find_ally( allyIndex );
									if( pAlly )
									{
										PrintConsole( "[ALLY] Send Ally(%d) Information.\n", allyIndex );
										g_pSystemLog->LogFileWrite( "[ALLY] Send Ally(%d) Information.\n", allyIndex );
										pAlly->send_ally_info( pDSComm, NULL );
									}
									else
									{
										// 영웅단 정보를 업데이트한다.
										pOr->m_iAllyIndex = 0;		// 이것만 하면 되나? 아직 불완전하다.
									}
								}
#endif


								SMSG_OR_INFO_RES_DS_GS msgORInfoRes = {0} ;
								msgORInfoRes.usLength = sizeof(SMSG_OR_INFO_RES_DS_GS) ;
								msgORInfoRes.cMessage = SMSG_NO_OR_INFO_RES_DS_GS ;
								msgORInfoRes._organization = *pOr ;
								msgORInfoRes.cCode = SMSG_OR_INFO_RES_DS_GS::_INFO_REQ_ ;

								pDSComm->Send( msgORInfoRes.usLength, (char*)&msgORInfoRes ) ;
							}	
							else
							{
								// 영웅단 정보가 없다고 보내줘야 한다.
								g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "No\n" );
							}
						}						
						break ;
					// '07.07.24일 확인. 쓰이고 있지 않음.; _NON_USED_
					case SMSG_OR_SOME_REQ_GS_DS::_OR_SOME_REQ_DELETE :
						{

							if( _Organization * pOr = g_pOrganizationManager->FindOrganization( pMsgORSomeReq->iIndex ) )
							{

								if( true == g_pOrganizationManager->DeleteOR( pMsgORSomeReq->iIndex ) )
								{
#ifdef _PD_CASTLE_STEP_1_
									CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_org, pMsgORSomeReq->iIndex );
#endif
									
									SMSG_OR_RESULT_DS_GS msgORResult = {0} ;
									msgORResult.usLength = sizeof(SMSG_OR_RESULT_DS_GS) ;
									msgORResult.cMessage = SMSG_NO_OR_RESULT_DS_GS ;
									msgORResult.iIndex = pMsgORSomeReq->iIndex ;
									msgORResult.iCode = SMSG_OR_RESULT_DS_GS::_OR_RESULT_DELETE_ ;

									g_pDSComQueue->GlobalSend( msgORResult.usLength, (char*)&msgORResult ) ;

								}
							}
						}
						break ;
					}
				}
				catch(...){
					::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
				}
				
				break ;
			case SMSG_NO_OR_CREATE_REQ_GS_DS :
				
				try{
					SMSG_OR_CREATE_REQ_GS_DS * pMsgORCreateReq = (SMSG_OR_CREATE_REQ_GS_DS*)pack->buf ;

					PrintConsole( "[ORG] Create Req : C(%s), Org(%s)\n", pMsgORCreateReq->_organization.m_cCaptainName, pMsgORCreateReq->_organization.m_cOrName );
					g_pSystemLog->LogFileWrite( "[ORG] Create Req : C(%s), Org(%s)\n", pMsgORCreateReq->_organization.m_cCaptainName, pMsgORCreateReq->_organization.m_cOrName );

					_Organization tempOR = pMsgORCreateReq->_organization ;
					_Organization * pOr = g_pOrganizationManager->FindOrganization( pMsgORCreateReq->_organization.m_iIndex ) ;
					if(  pOr == NULL )
					{

						// 실패 처리 해야하는데.. ㅠ.ㅠv							
						SMSG_OR_INFO_RES_DS_GS msgORInfoRes = {0} ;
						msgORInfoRes.usLength = sizeof(SMSG_OR_INFO_RES_DS_GS) ;
						msgORInfoRes.cMessage = SMSG_NO_OR_INFO_RES_DS_GS ;

						// 이미 사용중인 이름인지 확인..
						if( g_pOrganizationManager->IsUsingName( pMsgORCreateReq->_organization.m_cOrName ) )
						{
							PrintConsole( "[ORG] Error : Create ORG - used name(%s).\n", pMsgORCreateReq->_organization.m_cOrName );
							g_pSystemLog->LogFileWrite( "[ORG] Error : Create ORG - used name(%s).\n", pMsgORCreateReq->_organization.m_cOrName );

							msgORInfoRes._organization = tempOR ;
							msgORInfoRes.cCode = SMSG_OR_INFO_RES_DS_GS::_CREATE_FAIL_USING_NAME_ ;

							pDSComm->Send( msgORInfoRes.usLength, (char*)&msgORInfoRes ) ;

							continue ;
						}
						
						int index = g_pOrganizationManager->CreateOR( tempOR ) ;
						if( index > 0 )
						{
							pOr = g_pOrganizationManager->GetORInfo( index ) ;
							if( pOr )
							{
								msgORInfoRes._organization = *pOr ;
								msgORInfoRes.cCode = SMSG_OR_INFO_RES_DS_GS::_CREATE_REQ_ ;

								// 모든 서버에 쏴주자..
								g_pDSComQueue->GlobalSend( msgORInfoRes.usLength, (char*)&msgORInfoRes ) ;							

								PrintConsole( " ==> Success : Create Organization\n" );
								g_pSystemLog->LogFileWrite( " ==> Success : Create Organization\n" );
							}							
						}
						//중복된 이름
						else if ( index == -1 )
						{
							PrintConsole( "[ORG] Error : Create ORG - used name2(%s).\n", pMsgORCreateReq->_organization.m_cOrName );
							g_pSystemLog->LogFileWrite( "[ORG] Error : Create ORG - used name2(%s).\n", pMsgORCreateReq->_organization.m_cOrName );

							msgORInfoRes._organization = tempOR ;
							msgORInfoRes.cCode = SMSG_OR_INFO_RES_DS_GS::_CREATE_FAIL_USING_NAME_ ;

							pDSComm->Send( msgORInfoRes.usLength, (char*)&msgORInfoRes ) ;

							continue ;
						}						
						else
						{
							PrintConsole( "[ORG] Error : Create ORG - etc.\n" );
							g_pSystemLog->LogFileWrite( "[ORG] Error : Create ORG - etc.\n" );

							msgORInfoRes._organization = tempOR ;							
							msgORInfoRes.cCode = SMSG_OR_INFO_RES_DS_GS::_CREATE_FAIL_ETC_ ;

							pDSComm->Send( msgORInfoRes.usLength, (char*)&msgORInfoRes ) ;							
						}
					}
					else
					{
						PrintConsole( "[ORG] Error : Already Exist org\n" );
						g_pSystemLog->LogFileWrite( "[ORG] Error : Already Exist org\n" );
					}
				}
				catch(...){
					::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);					
				}								
				
				break ;
				/*
			case SMSG_NO_OR_BASE_INFO_UPDATE :
				{
					try{
						SMSG_OR_BASE_INFO_UPDATE * pMsgORBaseInfoUpdate = (SMSG_OR_BASE_INFO_UPDATE*)pack->buf ;
						g_pOrganizationManager->BaseUpdate( pMsgORBaseInfoUpdate->_organizationBase.m_iIndex, pMsgORBaseInfoUpdate->_organizationBase ) ;
					}
					catch(...){
						::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
					}
					
				}
				break ;
				*/
			case SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS :
				{
					SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS * pMsgORUnitControl = (SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS*)pack->buf ;
					try{
						switch( pMsgORUnitControl->cCode )
						{
						case SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_CREATE_ :
							if( g_pOrganizationManager->PutORUnit( pMsgORUnitControl->iIndex, pMsgORUnitControl->_unit ) == false )
							{
								continue ;
							}							
							break ;
						case SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_DELETE_ :
							{
								int iRet = g_pOrganizationManager->PopORUnit( pMsgORUnitControl->iIndex, pMsgORUnitControl->_unit.m_cName ) ;
								if( iRet == -1 )
								{
#ifdef _PD_CASTLE_STEP_1_
									CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_org, pMsgORUnitControl->iIndex );
#endif

									SMSG_OR_RESULT_DS_GS msgORResult = {0} ;
									msgORResult.usLength = sizeof(SMSG_OR_RESULT_DS_GS) ;
									msgORResult.cMessage = SMSG_NO_OR_RESULT_DS_GS ;
									msgORResult.iIndex = pMsgORUnitControl->iIndex ;
									msgORResult.iCode = SMSG_OR_RESULT_DS_GS::_OR_RESULT_DELETE_ ;
									g_pDSComQueue->GlobalSend( msgORResult.usLength, (char*)&msgORResult ) ;
									continue ;
								}
							}
							break ;

						case SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_KICK_ :
							{
								int iRet = g_pOrganizationManager->PopORUnit( pMsgORUnitControl->iIndex, pMsgORUnitControl->_unit.m_cName ) ;
								if( iRet == -1 )
								{			
#ifdef _PD_CASTLE_STEP_1_
									CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_org, pMsgORUnitControl->iIndex );
#endif
									SMSG_OR_RESULT_DS_GS msgORResult = {0} ;
									msgORResult.usLength = sizeof(SMSG_OR_RESULT_DS_GS) ;
									msgORResult.cMessage = SMSG_NO_OR_RESULT_DS_GS ;
									msgORResult.iIndex = pMsgORUnitControl->iIndex ;
									msgORResult.iCode = SMSG_OR_RESULT_DS_GS::_OR_RESULT_DELETE_ ;
									g_pDSComQueue->GlobalSend( msgORResult.usLength, (char*)&msgORResult ) ;
									continue ;
								}
								else if( iRet == 0 )
								{
									continue ;
								}
							}

							break ;
						case SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_UPDATE_ :

							if( g_pOrganizationManager->UpdateORUnit( pMsgORUnitControl->iIndex, pMsgORUnitControl->_unit, pMsgORUnitControl->outTime  ) == false )
							{
								continue ;
							}
							break ;
						case SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_ENTRUST_ :
							g_pOrganizationManager->ExchangeCaptain( pMsgORUnitControl->iIndex							
								, pMsgORUnitControl->_unit.m_cName) ;
							break ;
						}

						g_pDSComQueue->GlobalSend( pMsgORUnitControl->usLength, (char*)pMsgORUnitControl ) ;
					}
					catch(...){
						::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);					
					}
				}
				break ;

			case SMSG_NO_OR_NOTICE_EDIT_GS_DS_GS:			
				
				try{
					SMSG_OR_NOTICE_EDIT * pMsgOrNoticeEdit = (SMSG_OR_NOTICE_EDIT*)pack->buf ;
					if( _Organization * pOr = g_pOrganizationManager->FindOrganization( pMsgOrNoticeEdit->iIndex ) )
					{
						memcpy( pOr->m_cNotice, pMsgOrNoticeEdit->cNotice, sizeof(pOr->m_cNotice) ) ;

						if( g_pOrganizationManager->DBUpdate( pOr )  == true )
						{
							g_pDSComQueue->GlobalSend( pMsgOrNoticeEdit->usLength, (char*)pMsgOrNoticeEdit ) ;
						}
					}
				}
				catch(...){
					::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);					
				}
				break ;
#ifdef _PD_ALLY_ORG_STEP_1_
			case SMSG_NO_ALLY_BASE:
				try
				{
					SMSG_ALLY_BASE * pBase = reinterpret_cast<SMSG_ALLY_BASE*>(pack->buf);

					switch( pBase->ucMode )
					{
					case en_ds_ally_request_create:		// 0 생성 요청
						g_pAllyManager->pk_request_create( static_cast<SMSG_ALLY_REQUEST_CREATE*>(pBase), pDSComm );
						break;
						
					case en_ds_ally_request_info:		// 3 정보요청
						g_pAllyManager->pk_request_info( static_cast<SMSG_ALLY_REQUEST_INFO*>(pBase), pDSComm );
						break;

					case en_ds_ally_request_add_org:	// 5 동맹 추가 요청
						g_pAllyManager->pk_request_add_org( static_cast<SMSG_ALLY_REQUEST_ADD_ORG*>(pBase), pDSComm );
						break;

					case en_ds_ally_request_leave:		// 동맹 탈퇴 요청
						g_pAllyManager->pk_request_leave( static_cast<SMSG_ALLY_REQUEST_LEAVE*>(pBase), pDSComm );
						break;

					case en_ds_ally_request_breakup:	//  해산 요청
						g_pAllyManager->pk_request_breakup( static_cast<SMSG_ALLY_REQUEST_BREAKUP*>(pBase), pDSComm );
						break;

					case en_ds_ally_request_banish:
						g_pAllyManager->pk_request_banish( static_cast<SMSG_ALLY_REQUEST_BANISH*>(pBase), pDSComm );
						break;

#ifdef _PD_CASTLE_STEP_2_
					case en_ds_ally_request_all_ally:
						g_pAllyManager->pk_request_all_ally( pDSComm );
						break;
#endif
					}
				}
				catch ( ... ) 
				{
					g_pSystemLog->LogFileWrite( "[EXCEPTION] SMSG_NO_ALLY_BASE\n" );
				}
				break;
#endif

#ifdef _PD_CASTLE_STEP_1_
			case CMSG_NO_COMBAT_PACKET:
				try
				{
					CMSG_COMBAT_PACKET * pCombat = reinterpret_cast<CMSG_COMBAT_PACKET*>(pack->buf);

					switch( pCombat->ucMode )
					{
					case en_dscb_req_manor_information:
						CDSCastleManager::GetObj().proc_gs_req_manor_information( static_cast<CMSG_CB_REQ_MANOR_INFORMATION*>(pCombat), pDSComm );
						break;
#ifdef _PD_CASTLE_RENEWAL_1_
					case en_dscb_req_change_time:
						CDSCastleManager::GetObj().proc_gs_req_change_time( static_cast<CMSG_CB_REQ_CHANGE_TIME*>(pCombat), pDSComm );
						break;
#endif
					case en_dscb_challenge_insert:
						CDSCastleManager::GetObj().proc_gs_challenge_insert( static_cast<CMSG_CB_CHALLENGE_INSERT*>(pCombat), pDSComm );
						break;
					case en_dscb_challenge_delete:
						CDSCastleManager::GetObj().proc_gs_challenge_delete( static_cast<CMSG_CB_CHALLENGE_DELETE*>(pCombat), pDSComm );
						break;
					case en_dscb_challenge_reset:
						CDSCastleManager::GetObj().proc_gs_challenge_reset( static_cast<CMSG_CB_CHALLENGE_RESET*>(pCombat) );
						break;
					case en_dscb_owner_info_changed:
						CDSCastleManager::GetObj().proc_gs_owner_info_changed( static_cast<CMSG_CB_OWNER_INFO_CHANGED*>(pCombat) );
						break;
					case en_dscb_combat_start:
						CDSCastleManager::GetObj().proc_gs_combat_start( static_cast<CMSG_CB_COMBAT_START*>(pCombat) );
						break;
					case en_dscb_combat_end:
						CDSCastleManager::GetObj().proc_gs_combat_end( static_cast<CMSG_CB_COMBAT_END*>(pCombat) );
						break;
#ifdef _PD_CASTLE_STEP_2_
					case en_dscb_record_info_req:
						CDSCastleManager::GetObj().proc_gs_req_record_info(static_cast<CMSG_RECORD_INFORMATION_REQ*>(pCombat),pDSComm );
						break;					
					case en_dscb_record_update:
						CDSCastleManager::GetObj().proc_gs_update_record_info(static_cast<CMSG_RECORD_UPDATE_GS_DS*>(pCombat) );
						break;
					case en_dscb_record_reset:
						CDSCastleManager::GetObj().proc_gs_reset_record_info(static_cast<CMSG_RECORD_RESET*>(pCombat) );
						break;
#endif
					}
				}
				catch ( ... )
				{
					g_pSystemLog->LogFileWrite( "[EXCEPTION] CMSG_NO_COMBAT_PACKET\n" );
				}
				break;
#endif

#ifdef _PD_CASTLE_STEP_2_
			case CMSG_NO_MANOR_TAX:
				try
				{
					CMSG_MANOR_TAX * pManorPkt = reinterpret_cast<CMSG_MANOR_TAX*>( pack->buf );
					
					switch(pManorPkt->ucMode)
					{
					case en_manor_tax_req_start_compute_gs_ds:	// 장원이 한시간 마다 세금 집계 요청.
						{
							//모든 GS 정보 요청
							CManorTaxManager::GetObj().send_cmsg_manor_tax_info_req_ds_gs();
						}
						break;
					case en_manor_tax_info_gs_ds:				//정보 받음.
						{
							CManorTaxManager::GetObj().proc_cmsg_manor_tax_info_gs_ds( static_cast<CMSG_MANOR_TAX_INFO_GS_DS*>(pManorPkt) );
						}
						break;
					case en_manor_tax_req_total_gs_ds:
						{
							CManorTaxManager::GetObj().proc_cmsg_manor_tax_req_total_gs_ds( static_cast<CMSG_MANOR_TAX_REQ_TOTAL_GS_DS*>(pManorPkt) );
							//::PrintConsole("[TAX_SYSEM] Collect Manor \n");
						}
						break;
							//en_manor_tax_req_total_gs_ds,			// 약 30초 후, 전체 세금을 요청한다.
							//en_manor_tax_notice_total_tax_ds_gs,	// 전체 세금을 알려줌.
					case en_manor_tax_req_take:
						{
							CManorTaxManager::GetObj().proc_cmsg_tax_req_take( static_cast<CMSG_MANOR_TAX_REQ_TAKE*>(pManorPkt), pDSComm );
						}
						break;
					case en_manor_tax_challenge_gs_ds:
						{
							CMSG_MANOR_TAX_CHALLENGE_GS_DS * pPkt = static_cast<CMSG_MANOR_TAX_CHALLENGE_GS_DS*>(pManorPkt);
							CDSCastleManager::GetObj().tax_add_from_tax_system( pPkt->ucMapID , pPkt->uiChallengeTax, 0 );
							
							::PrintConsole("[TAX_SYSEM] CMSG_MANOR_TAX_CHALLENGE_GS_DS Map<%d>, ChallengeTax<%d> \n" \
								, pPkt->ucMapID \
								, pPkt->uiChallengeTax) ;
						}
					}
				}
				catch ( ... )
				{
					PrintConsole( "[EXCEPTION] MANOR TAX SYSTEM\n" );				
				}
				break;
#endif//_PD_CASTLE_STEP_2_

			default:
				::PrintConsole("[ERROR] th_ORWorkThread Bad packet <msg:%d, pos:%d> \n",Mainmsg, pos ) ;
				break ;
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_WorkThread <msg:%d, pos:%d> \n",Mainmsg, pos ) ;
		}
	}
	printf("[THREAD] th_CSWorkThread Exit thread \n") ;
	return 0 ;

}



