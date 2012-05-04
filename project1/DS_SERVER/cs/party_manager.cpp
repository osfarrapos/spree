#include "..\stdafx.h"
#include "..\Network\Network.h"
#include "..\Global\Global_ServerInfo.h"

#include "..\MapManager\MAPManager.h"
#include "..\CS\party_manager.h"
#include "..\Global\tcp_packets_party.h"
#pragma warning(disable:4509)
extern _j_MemeberQueue *	g_pMemberQueue ;
extern _j_PartyQueue *		g_pPartyQueue  ;
extern _h_Party_Timer *		g_pPartyTimer ;
extern _j_PartyManager *	g_pPartyManager ;
extern _j_ServerInfo *		g_pServerInfo ;
extern _j_AccMAPManager *	g_pAccMapManager ;



_j_MemeberQueue::_j_MemeberQueue() : m_pMember(NULL)
{ 		
}
_j_MemeberQueue::~_j_MemeberQueue() 
{ 	
	delete [] m_pMember ;
}


// _j_NodeQueue Class
bool _j_MemeberQueue::Create( const size_t uiSize ) 
{	
	m_pMember = new _Member[uiSize] ;
	if(m_pMember == NULL){
#ifdef _DEBUG
		printf("-> [error]VitualMemory Commit fail\n");
#endif
		return false;
	}

	member_lock() ;
	for(int i = 0; i < _PERMISSIBLE_MAX_USER; i++ ){
		m_NodeQueue.push(&m_pMember[i]);
	}
	member_unlock() ;
	
#ifdef _DEBUG
	printf("-> Map USER DATA Memory zone Reserved (Bytes:%d)\n", sizeof(_Member)*uiSize ) ;
#endif

	return true;
}

_Member * const _j_MemeberQueue::GetNode( const u_short uid, _CHARAC * const pCharac ) 
{
	if( NULL == pCharac || 0 >= uid )
		return NULL ;

	_Member * pMember = NULL ;
	map<u_short, _Member*>::iterator memberMapIter;

	member_lock() ;
	TRY{
		memberMapIter = m_MemberMap.find(uid) ;

		// 맵에 없으면 큐에서 새로운 노드를 꺼내준다.
		if ( m_MemberMap.end() == memberMapIter ) {

			if( !m_NodeQueue.empty() ) {
				pMember =  m_NodeQueue.front() ;
				if(pMember) 
				{
					pMember->reset() ;
					pMember->pCharac = pCharac ;			// 캐릭터의 포인터를 저장한다.
					m_NodeQueue.pop() ;	
					m_MemberMap[uid] = pMember ;		// 맵에 등록
				}				
			}
			else
			{
				::PrintConsole("[ERROR] m_NodeQueue.empty() \n") ;
			}
		}
		else
		{
			pMember = memberMapIter->second ;	
		}		

	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::GetNode \n") ;
		pMember = NULL ;
	}
	member_unlock() ;

	return pMember ;
}

void _j_MemeberQueue::PutNode( _Member * const Node )
{
	map<u_short, _Member*>::iterator memberMapIter;
	member_lock() ;
	TRY{
		//if ( Node->cCheck == 0  ) // 더이상 사용하지 않는 노드인지 확인
		{			
			memberMapIter = m_MemberMap.find(Node->uid) ;
			if( memberMapIter != m_MemberMap.end() ) 
			{
				m_MemberMap.erase(memberMapIter) ;	// 맵에서 삭제
			}
			Node->reset() ;
			m_NodeQueue.push( const_cast<_Member*>(Node) ) ;
		}	
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::PutNode \n") ;
	}	
	member_unlock() ;
}

_Member * const	_j_MemeberQueue::FindNode( const u_short uid )
{
	_Member * pMember = NULL ;
	map<u_short, _Member*>::iterator memberMapIter;
	member_lock() ;
	TRY{
		memberMapIter = m_MemberMap.find(uid) ;
		if ( m_MemberMap.end() == memberMapIter ) {
			pMember = NULL ;
		}
		else
		{
			pMember = memberMapIter->second ;
		}
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::FindNode \n") ;
		pMember = NULL ;
	}
	member_unlock() ;

	return pMember ;
}

void _j_MemeberQueue::InsertMap( const u_short uid, const _Member * const Node) 
{
	member_lock() ;
	TRY{
		m_MemberMap[uid] = const_cast<_Member*>(Node) ;
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::InsertMap \n") ;
	}
	member_unlock() ;

}

void _j_MemeberQueue::DeleteMap( const u_short uid) 
{
	map<u_short, _Member*>::iterator memberMapIter;
	member_lock() ;
	TRY{
		memberMapIter = m_MemberMap.find(uid) ;
		if( m_MemberMap.end() != memberMapIter ){
			m_MemberMap.erase(memberMapIter) ;
		}
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::DeleteMap \n") ;
	}
	member_unlock() ;

}


/*===========================================================================
_j_PartyQueue : 파티들을 큐에 미리 할당하여 사용한다.
===========================================================================*/


// Queue에 있는 Party포인터를 반환받는다.
_Party * const _j_PartyQueue::GetNode() 
{
	_Party * pParty = NULL ;
	party_queue_lock() ;
	TRY{
		if( m_PartyQueue.size() > 0 )
		{
			pParty = m_PartyQueue.front() ;
			m_PartyQueue.pop() ;
			if( pParty ) pParty->Init() ;		
		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] _Party * const _j_PartyQueue::GetNode() \n") ;
	}	
	party_queue_unlock() ;
	return pParty ;

}


// 다쓴 Party 포인터를 Queue에 반환한다.
void _j_PartyQueue::PutNode( const _Party * const pParty)
{

	if ( pParty == NULL ) return ;

	party_queue_lock() ;
	TRY{
		m_PartyQueue.push( const_cast<_Party*>(pParty) ) ;		
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] void _j_PartyQueue::PutNode( const _Party * const pParty) \n") ;
	}	
	party_queue_unlock() ;
}



//============================================================================

/*===========================================================================
_j_MAP : 파티 리스트 맵을 관리한다..
===========================================================================*/




// 파티 리스트 삭제
/*
bool  _j_PartyManager::DeleteList(short sListIndex) 
{
	bool b = false ;
	map<short, _Party*>::iterator tempItr;	

	party_map_lock() ;
	TRY{
		tempItr = m_Map.find(sListIndex) ;

		if ( tempItr != m_Map.end() ) 
		{
			// Map에서 List 삭제	
			m_Map.erase(tempItr) ;
			b = true ;
		}

	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] bool  _j_PartyManager::DeleteList(short sListIndex) \n") ;
	}	
	party_map_unlock() ;	
	return b ;
}
*/

_Party *  _j_PartyManager::FindList(short sListIndex) 
{
	_Party * pTemp = NULL ;	

	party_map_lock() ;
	TRY{
		map<short, _Party*>::iterator tempItr = m_Map.find(sListIndex) ;		
		if ( tempItr != m_Map.end() ) {	
			pTemp = tempItr->second ;
		}
	}
	CATCH_ALL{
		PrintConsole("[EXCEPTION] _Party *  _j_PartyManager::FindList(short sListIndex) \n") ;
	}	
	party_map_unlock() ;
	return pTemp ;
}




/*===========================================================================
_j_PartyManager
===========================================================================*/
// PartyMap Manager

short _j_PartyManager::CreateParty(char cServerNum, u_short uid1, u_short uid2, char * name1, char * name2, OUT short & sProp )
{

	if(cServerNum <= 0 || uid1 <= 0 || uid2 <= 0)
		return 0 ;	

	_C_CHARAC * pCharac1 = g_pAccMapManager->Find_Charac( name1 ) ;
	_C_CHARAC * pCharac2 = g_pAccMapManager->Find_Charac( name2 ) ;

	if(NULL == pCharac1 || NULL == pCharac2 /*|| pCharac1->CHARAC_STATE.sPartyIndex > 0 || pCharac2->CHARAC_STATE.sPartyIndex > 2 */)
		return 0 ;

	short sProp1 = 0, sProp2 = 0, sPartyProp = 0;
	sProp1 = g_sMoonpaProperty[pCharac1->CHARAC_BASIC.cParty] ;
	sProp2 = g_sMoonpaProperty[pCharac2->CHARAC_BASIC.cParty] ;


	if( ::IsFriendly( sProp1, sProp2 ) == false )
		return 0 ;


	_Member *	pMember1 = NULL ;
	_Member *	pMember2 = NULL ;
	_Party *	pParty = NULL ;

	TRY{			

		pMember1 = g_pMemberQueue->GetNode(uid1, pCharac1) ;
		pMember2 = g_pMemberQueue->GetNode(uid2, pCharac2) ;

		pParty = g_pPartyQueue->GetNode() ;
		
		//if( (!pMember1 || pMember1->attribute.bParty == true) ||
		//	(!pMember2 || pMember2->attribute.bParty == true )|| 
			//!pParty ) // 어느것이라도 NULL 이면 실패처리해야한다.
		if( pMember1 == NULL || pMember2 == NULL || pParty == NULL ) // 어느것이라도 NULL 이면 실패처리해야한다.
		{
			
			
			// 반환할것이 있다면.. 반환한다..
			if( pMember1 ) 
			{				
				g_pMemberQueue->PutNode( pMember1 ) ;				
			}
			else
			{
				::PrintConsole("[ERROR] pMember1 == NULL \n") ;
			}
			if( pMember2 ) {				
				g_pMemberQueue->PutNode( pMember2 ) ;
			}
			else
			{
				::PrintConsole("[ERROR] pMember2 == NULL \n") ;
			}
			if( pParty ) {
				g_pPartyQueue->PutNode( pParty ) ;				
			}
			else
			{
				::PrintConsole("[ERROR] pParty == NULL \n") ;
			}

			return 0 ;
		}

		pParty->Init() ;


		// 파티장..			
		pMember1->attribute.bParty = true ;
		pMember1->attribute.bCaptian = true ;
		pMember1->uid = uid1 ;
		pMember1->pCharac = pCharac1 ;

		// 파티원
		pMember2->attribute.bParty = true ;
		pMember2->attribute.bCaptian = false ;
		pMember2->uid = uid2 ;
		pMember2->pCharac = pCharac2 ;

		pParty->SetPartyIndex( GetPartyIndex() ) ;


		::WhatIsMoonpaProp( sProp1, sProp2, OUT sPartyProp ) ;
		pParty->SetMoonpaProp( sPartyProp ) ;
		sProp = pParty->GetMoonpaProp() ;


		// 파티의 속성(정보) 셋팅..
		pParty->CreateParty( pMember1, pMember2) ;

		// 서버 정보(?) 셋팅
		pParty->SetServer( cServerNum ) ;		

	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::CreateParty \n") ;
		return 0 ;
	}

	party_map_lock() ;	
	TRY{
		m_Map[pParty->GetPartyIndex()] = pParty ;	// 생성된 파티 리스트를 맵에 삽입	
	}
	CATCH_ALL{
		::PrintConsole("[EXCEOTION] m_Map[pParty->GetPartyIndex()] = pParty  \n") ;
	}
	party_map_unlock() ;

#ifdef _TRACE_
	g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Create party success \n") ;
#endif

	return pParty->GetPartyIndex() ;	
}

bool _j_PartyManager::DeleteParty(_Party * pParty)
{	

	if( pParty == NULL ) {
		return false ;	// 해당 파티 인덱스를 찾지 못한경우다.
	}

	/*
	if ( pParty->GetMemberCount() == 0 ) {
		DeleteList(pParty->GetPartyIndex()) ;
		g_pPartyQueue->PutNode( pParty ) ;
		return false ;  // 파티 리스트가 비어있다..
	}
	*/

	pParty->DeleteParty() ;
	// Map에서 List 삭제	
	map<short, _Party*>::iterator tempItr;	
	bool b = false ;
	party_map_lock() ;
	TRY{		
		tempItr = m_Map.find(pParty->GetPartyIndex()) ;
		if ( tempItr != m_Map.end() ) {			
			m_Map.erase(tempItr) ;
			g_pPartyQueue->PutNode( pParty ) ;
			b = true ;
		}
		else
		{
			b = false ;
		}

	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::DeleteParty\n") ;
		b = false ;
	}
	party_map_unlock() ;
	

	return b ;	
}


int _j_PartyManager::JoinParty(_Party * party, u_short uid, char * name, OUT short & sProp )
{	
	int slot ;
	TRY{

		if( uid <= 0 || !party ) {
			::PrintConsole("[ERROR] 'cServerNum <= 0 || uid <= 0 || !party'\n") ;
			return -1 ;
		}
		if( party->GetMemberCount() >= MAX_PARTY_MEMBER ){
			::PrintConsole("[ERROR] party->cMemberCount >= MAX_PARTY_MEMBER \n") ;
			return -1 ;
		}

		_CHARAC * pCharac = g_pAccMapManager->Find_Charac( name ) ;
		if( NULL == pCharac )
		{
			::PrintConsole("[ERROR] _j_PartyManager::JoinParty \n") ;		
			return -1 ;
		}

		_Member * const pMember = g_pMemberQueue->GetNode(uid, pCharac ) ;
		if ( !pMember || pMember->attribute.bParty == true  ) {			
			::PrintConsole("[ERROR] if ( !pMember || pMember->attribute.bParty == true  )  \n") ;		
			return -1 ;
		}

		pMember->pCharac = pCharac ;
		pMember->uid = uid ;	
		pMember->attribute.bCaptian = false ;
		pMember->attribute.bParty = true ;

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[JOIN] Joined (%s) \n", name) ;
#endif


		short sTempProp = g_sMoonpaProperty[pCharac->CHARAC_BASIC.cParty] ;		
		if( party->GetMoonpaProp() == 0 )
		{
			party->SetMoonpaProp( sTempProp ) ;
		}
		else if( ::IsFriendly( sTempProp, party->GetMoonpaProp() ) == false )
		{
			pMember->attribute.bParty = false ;
			g_pMemberQueue->PutNode( pMember ) ;
			//::PrintConsole("[ERROR] if( ::IsFriendly( %d, %d ) == false ) \n", sTempProp, party->GetMoonpaProp()) ;
			return -1 ;
		}		
		sProp = party->GetMoonpaProp() ;


		slot = party->SetMember( pMember ) ;	
	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] int _j_PartyManager::JoinParty(_Party * party, char cServerNum, u_short uid, char * name) \n") ;
		slot = -1 ;
	}	
	return slot ;
}

int _j_PartyManager::KickParty(_Party * pParty, char slotNo)
{	
	int iRes ;
	int pos = 0 ;


	TRY{
		if( !pParty || slotNo<0 )
			return -1 ;

		pos = 2 ;
		// 파티 인원이 2명 이하일 경우는 파티를 삭제한다...
		if ( pParty->GetMemberCount() < 3 ) {	

			//pParty->DeleteParty() ;
#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] 파티원 미달로 삭제되었음 \n") ;
#endif
			//DeleteList(pParty->GetPartyIndex()) ;
			//g_pPartyQueue->PutNode( pParty ) ;
			DeleteParty( pParty ) ;
			return 1 ;
		}

		iRes = 0 ;

		pos = 3 ;	

		iRes = pParty->KickMember( slotNo ) ;
	}
	CATCH_ALL {		
		::PrintConsole( "[EXCEPTION] _j_PartyManager::KickParty (pos.%d)\n", pos) ;		
	}


	return iRes ;

	//return -3 ;	// 파티원을 찾지 못한 경우다.	
}


char _j_PartyManager::EntrustCaptain(_Party * pParty, char slotNo, u_short uid )
{	

	char  slot ;

	TRY {

		bool bCheck = (pParty == NULL) 
			|| ( pParty->GetMemberCount() == 0 ) 
			|| (slotNo >= MAX_PARTY_MEMBER)
			|| (slotNo < 0 ) ;

		if( bCheck ) {
			return -1 ;
		}

		if( uid == 0 )
		{
			slot = pParty->Entrust() ;
		}
		else
		{
			slot = pParty->Entrust( slotNo ) ;
		}		

	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::EntrustCaptain \n") ;
		return -1 ;	
	}

	return slot ;

}

bool _j_PartyManager::MoveParty(_Party * pParty, char cServerNum, u_short uid, char slotNo) 
{

	TRY{
		_Member *			pMember = NULL ;


		if(pParty == NULL) {
			return false ;
		}		

		pMember = g_pMemberQueue->FindNode(uid) ;
		if ( !pMember || pMember->attribute.bParty == false ) {
			return false ;
		}	

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "-> [MOVE] Move Request (%s) \n", pMember->pCharac->CHARAC_BASIC.cChrName ) ;
#endif
		
		pMember->attribute.bIsMoveState = true ;


		_sParty_timerentry timerEnt ;

		timerEnt.pMember = pMember ;
		timerEnt.time = _sParty_timerentry::get_time_by_sec() + _PARTY_MOVE_OUT_TIME_SEC  ;
		timerEnt.work = _ns_party_::_TM_PARTY_MOVE_READY_ ;		

		timerEnt.index = pParty->GetPartyIndex() ;
		timerEnt.uid = ++pMember->usMoveCheckIndex ;	// 이 인덱스 값을 비교해서 여러번 서버이동을 했을 경우 최종 서버이동에 대해서만 검사하게 된다.
		timerEnt.slot = slotNo ;	


		g_pPartyTimer->send_Message( timerEnt ) ;
#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Add timer (uid : %d) \n", uid) ;
#endif	


	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::MoveParty \n") ;
	}		

	return true ;
}


void _j_PartyManager::MoveComplete( u_short uid, char slotNo ) 
{
	TRY{		
		_Member *			pMember = NULL ;	


		pMember = g_pMemberQueue->FindNode(uid) ;


		if ( pMember == NULL || pMember->attribute.bParty == false ) {
#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[MOVE] pMember == NULL || pMember->attribute.bParty == false  (%d) \n", uid ) ;
#endif
			return ;
		}		

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[MOVE] Move complete (%s) \n", pMember->pCharac->CHARAC_BASIC.cChrName) ;
#endif

		pMember->attribute.bIsMoveState = false ;	


	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::MoveComplete \n") ;
	}	
}


//void	_j_PartyManager::PutServer(list<_Server> * serverList, char cServerNum) 
//{
//	list<_Server>::iterator serverListItr ;
//	_Server * pServerInfo ;
//	
//	for( serverListItr = serverList->begin() ; serverListItr != serverList->end() ; serverListItr++)
//	{
//		pServerInfo = &(*serverListItr) ;
//		if( pServerInfo->serverNum == cServerNum ){
//			++pServerInfo->count ;
//			break ;
//		}		
//	}
//}
//void	_j_PartyManager::PopServer(list<_Server> * serverList, char cServerNum)
//{
//}
//void	_j_PartyManager::MoveServer(list<_Server> * serverList, char cFromServerNum, char cToServerNum)
//{



/*#################################################################################

CS Timer Thread

#################################################################################*/
DWORD WINAPI th_Party( LPVOID lp ) 
{
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;

	LARGE_INTEGER li = {0, } ;
	DWORD curTime ;

	_sParty_timerentry entry ;
	int iRes ;

	char		g_cServerNumList[MAX_SERVER_COUNT] ;
	_h_DS_Com * pDSComm ;
	_Party *	pParty ;

	SMSG_KICK_PARTY_RES_CS_GS msgKickPartyRes = {0} ;
	msgKickPartyRes.usLength = sizeof(msgKickPartyRes) ;
	msgKickPartyRes.cMessage = SMSG_NO_KICK_PARTY_RES_CS_GS ;



	printf( "  => Start Party Timer\n" ) ;

	SetWaitableTimer( hTimer, &li, 1000, NULL, NULL, TRUE ) ;

	while(1) {
		WaitForSingleObject( hTimer, INFINITE ) ;

		curTime = _sParty_timerentry::get_time_by_sec() ;
		while ( g_pPartyTimer->get_TimerMessage( curTime, entry ) == true ) {
			// 일처리. 처리할 때는 큐의 Lock을 걸지 않아도 된다. ^^

			TRY	{
				switch( entry.work ) 
				{
				case _ns_party_::_TM_PARTY_MOVE_READY_ :
					// entry.uid == entry.pMember->usMoveCheckIndex 
					// 이 인덱스 값을 비교해서 여러번 서버이동을 했을 경우 최종 서버이동에 대해서만 검사하게 된다.
					if( entry.pMember->attribute.bIsMoveState == true && entry.uid == entry.pMember->usMoveCheckIndex )
					{								

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Check Timer & Kick (uid : %d) \n", entry.uid ) ;
#endif
						pParty = g_pPartyManager->FindList( entry.index ) ;
						if( !pParty )
							continue ;	

						iRes = g_pPartyManager->KickParty(pParty, entry.slot) ;
						if ( iRes < 0 ) 
						{
#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] KickParty fail (%d) \n", iRes) ;
#endif
						}
						else if( iRes == 1 )	// 파티원 미달로 파티 해제..
						{
#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] 파티원 미달로 해체 (partyIndex : %d)\n", pParty->GetPartyIndex() ) ;
#endif							
						}

						else if( iRes == 2)	// 자동 위임..
						{
							msgKickPartyRes.cCapNo = g_pPartyManager->EntrustCaptain(pParty	, 0, 0) ;
#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] 파티장 자동 위임 (partyIndex : %d, 파티장 : %u )\n", 
								pParty->GetPartyIndex(),  pParty->GetCaptain() ) ;
#endif													
						}


						msgKickPartyRes.sPartyIndex = entry.index ;
						msgKickPartyRes.uid = entry.uid ;
						msgKickPartyRes.cResult = (char)iRes ;
						msgKickPartyRes.cMode = 2 ;					// 자진탈퇴로 처리
						msgKickPartyRes.cSlotNo = entry.slot ;

						memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;	
						if ( iRes >= 0 ) {	// 탈퇴가 성공했거나, 파티가 깨졌을경우
							for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
								if ( g_cServerNumList[i] == '&' ) {
									break ;
								}
								pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
								if(pDSComm){								
									if( pDSComm->Send(msgKickPartyRes.usLength, (char*)&msgKickPartyRes) == false ){
										::PrintConsole( "[SO_ERR] pDSComm->send_request \n" ) ;
									}						
								}
							}
						}

					}

					break; 
				}
			}
			CATCH_ALL{
				::PrintConsole("[EXCEPTION] DWORD WINAPI th_Party( LPVOID lp ) \n") ;
			}			
		} // while( g_pPartyTimer->get_TimerMessage( curTime, entry ) == true )
	}

	return 0 ;
}

//#################################################################################
//
//	_h_Party_Timer
//
//###################################################################### LHS Added....
bool _h_Party_Timer::create()
{
	HANDLE hTh ;
	DWORD dwID ;

	hTh = BEGINTHREADEX( NULL, 0, th_Party, NULL, 0, &dwID ) ;
	if( hTh == INVALID_HANDLE_VALUE ) {
		return false ;
	}
#ifdef _TRACE_
	printf("[PARTY] Begin Timer thread \n") ;
#endif
	CloseHandle( hTh ) ;

	return true ;
}

bool _h_Party_Timer::get_TimerMessage( DWORD time, _sParty_timerentry& Entry )
{
	bool b = false ;
	party_timer_lock() ;
	TRY{
		if( m_TimerQueue.empty() ) {			
			b = false  ;
		}
		else
		{
			map<_Member *, _sParty_timerentry>::iterator itr ;
			for( itr = m_TimerQueue.begin() ; itr != m_TimerQueue.end() ; itr++ )
			{
				if( itr->second.time <= time )
				{
					Entry = itr->second ;
					m_TimerQueue.erase( itr ) ;
					b = true ;
					break ;
				}
			}			
		}		
	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] bool _h_Party_Timer::get_TimerMessage( DWORD time, _sParty_timerentry& Entry ) \n") ;
		b = false ;
	}
	party_timer_unlock() ;
	
	return b ;
}

void _h_Party_Timer::send_Message( _sParty_timerentry& en )
{
	party_timer_lock() ;
	TRY {
		//m_TimerQueue.push( en ) ;
		m_TimerQueue[en.pMember]= en ;	

	}
	CATCH_ALL{
		::PrintConsole( "[EXCEPTION] Party Timer - send_message\n" ) ;
	}
	party_timer_unlock() ;
}


//#################################################################################
//
//	_Party
//
//#################################################################################



void _Party::CreateParty( _Member * const p1, _Member * const p2 )
{
	TRY{
		m_pMember[0] = p1 ;
		m_pMember[1] = p2 ;
		m_cMemberCount = 2 ;
		m_cExpProperty = 0 ;
		m_usCaptainUID = p1->uid ;		
	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] void _Party::CreateParty( _Member * const p1, _Member * const p2 ) \n") ;
	}
	
}

void _Party::DeleteParty()
{
	// 파티 멤버들의 구조체를 큐에 반환한다.
	party_lock() ;
	TRY{
		for ( int i = 0 ; i < MAX_PARTY_MEMBER ; i++ ) {
			if( m_pMember[i] )
			{
				m_pMember[i]->attribute.bParty = false ;
				m_pMember[i]->attribute.bCaptian = false ;			

				g_pMemberQueue->PutNode(m_pMember[i] ) ;	// Node 반환

			}
		}	
	}
	CATCH_ALL	{
		::PrintConsole("[EXCEPTION] void _Party::DeleteParty() \n") ;
	}	
	party_unlock() ;
}

void _Party::SetServer(const char cServerNum )
{
	party_lock() ;
	TRY{
		// 서버리스트를 검사해서 서버가 등록되어 있지 않다면 새로 추가..
		for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ )
		{
			if( m_cServerNumList[i] == cServerNum )
			{
				break ;
			}

			if( m_cServerNumList[i] == 0 || m_cServerNumList[i] == '&' )
			{
				m_cServerNumList[i] = cServerNum ;
				m_cServerNumList[i+1] = '&' ;
				break ;
			}
		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] void _Party::SetServer(const char cServerNum ) \n") ;
	}
	party_unlock() ;
}

const int _Party::SetMember( _Member * const pMember )
{
	int slot ;

	party_lock() ;	
	TRY{		
		for(slot = 0 ; slot < MAX_PARTY_MEMBER; slot++)
		{		
			if(m_pMember[slot] == NULL)
			{
				m_pMember[slot] = pMember ;
				m_cMemberCount++ ;				
				break ;
			}
		}		
	}
	CATCH_ALL {		
		::PrintConsole("[EXCEPTION] _j_PartyManager::JoinParty \n") ;
		slot = -1 ;
	}	
	party_unlock() ;

	return slot ;
}

const int _Party::KickMember(const char slot)
{
	int iRes  = 0 ;

	party_lock() ;
	TRY{
		if( m_pMember[slot] )
		{			

			if ( m_pMember[slot]->attribute.bCaptian == true ) {	// 파티장이 나갈경우에는 미지정 자동 위임					
				iRes = 2 ;	// 파티장 위임 메세지 번호
			}

			//int svrNum = pParty->pMember[slotNo]->cServerNum ;

			m_pMember[slot]->attribute.bParty = false ;
			m_pMember[slot]->attribute.bCaptian = false ;


			g_pMemberQueue->PutNode(m_pMember[slot]) ;


			m_pMember[slot] = NULL ;		// 슬롯을 비우고..	
			m_cMemberCount-- ;		// 카운드를 다운카운트 한다..				


#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[KICK] Kicked (Slot : %d ) \n", slot) ;
#endif

		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] const int _Party::KickMember(const char slot) \n") ;
	}	
	party_unlock() ;

	return iRes ;

}

const int _Party::Entrust( const char slot )
{

	party_lock() ;
	TRY	{
		// 우선 초기화..
		for( int i = 0 ; i < MAX_PARTY_MEMBER; i++ )
		{
			if( m_pMember[i] )
			{
				m_pMember[i]->attribute.bCaptian = false ;
			}
		}

		//  지정 위임		
		if( m_pMember[slot] ) {
			m_pMember[slot]->attribute.bCaptian = true ;
			m_usCaptainUID = m_pMember[slot]->uid ;	// 파티 속성중 파티장 아이디 변경..

#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[ENTR] Directical Entrusted (%s) \n", m_pMember[slot]->pCharac->CHARAC_BASIC.cChrName ) ;
#endif

		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] const int _Party::Entrust( const char slot ) \n") ;
	}	
	party_unlock() ;

	return slot;

}

const int _Party::Entrust()
{

	int i ;

	party_lock() ;
	TRY	{
		for( i =0; i < MAX_PARTY_MEMBER; i++)
		{
			if( m_pMember[i] )
			{
				m_pMember[i]->attribute.bCaptian = true ;
				break ;
			}			
		}	

		m_usCaptainUID = m_pMember[i]->uid ;		// 파티 속성중 파티장 아이디 변경..		

#ifdef _TRACE_	
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[ENTR] Automatical Entrusted (%s) \n", m_pMember[i]->pCharac->CHARAC_BASIC.cChrName ) ;
#endif				
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] const int _Party::Entrust() \n") ;
	}
	party_unlock() ;

	return i ;

}
