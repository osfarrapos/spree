#include "..\stdafx.h"

#include "..\VMManager\VMManager.h"
#include "MAPManager.h"


extern _j_MemeberQueue *	g_pMemberQueue ;
extern _j_ComQueue *		g_pComQueue ;
extern CMYTrace *			g_pTrace ;


// _j_NodeQueue Class
bool _j_MemeberQueue::Create(  const size_t size ) 
{		
	m_pFriendInfo = new _FriendInfo[size] ;
	assert(m_pFriendInfo) ;

	if( m_pFriendInfo == NULL ){
		return false;
	}


	_FriendInfo * pTemp ;
	userpool_lock() ;
	
	try{
		for(int i = 0; i < size; i++ ){
			pTemp = &m_pFriendInfo[i] ;
			pTemp->releas() ;
			m_NodeQueue.push(pTemp);		
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}
	
	userpool_unlock() ;	
	
	return true;
}

bool _j_ComQueue::Create(const size_t size )
{
	
	pDSCom = new _h_DS_Com[size] ;
	printf( " 2 \n" ) ;
	assert( pDSCom ) ;
	if( pDSCom == NULL )
	{
		return false ;
	}

	printf( " 3 \n" ) ;

	_h_DS_Com * pTemp ;
	
	dspool_lock() ;

	printf( " 4 \n" ) ;
	try{
		for( int i = 0; i < size ; i++ ) {	
			pTemp = &pDSCom[i] ;
			pTemp->Release() ;
			ComQueue.push(pTemp) ;				
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}
	printf( " 5 \n" ) ;
	
	dspool_unlock() ;

	return true ;

}


_FriendInfo * _j_MemeberQueue::GetNode(char * cName) 
{
	if( cName == NULL )
		return NULL ;

	if( cName[0] == 0 )
		return NULL ;

	_FriendInfo * pTemp = NULL ;

	int pos = 0;

	userpool_lock() ;
	pos = 100;
	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.find(cName) ;

		pos = 1 ;
		
		// 맵에 없으면 큐에서 새로운 노드를 꺼내준다.
		if ( m_FriendMap.end() == m_FriendMapIter ) {
			pos = 2 ;
			if( !m_NodeQueue.empty() ) {
				pos = 3 ;
				pTemp =  m_NodeQueue.front() ;
				pos = 4 ;
				if( pTemp )
				{
					pTemp->releas() ;
					pTemp->SetName( cName ) ;		

					pos = 7 ;
					m_NodeQueue.pop() ;	
					pos = 6 ;					
					m_FriendMap[cName] = pTemp ;		// 맵에 등록
					pos = 9 ;
				}
			}
		}
		else
		{
			pos = 10 ;
			pTemp = m_FriendMapIter->second ;	
		}
		
	}
	catch (...) {
		PrintConsole("[EXCEPTION] _FriendInfo * _j_MemeberQueue::GetNode(char * cName) pos: %d\n", pos) ;
		pTemp = NULL  ;	
	}	
	userpool_unlock() ;	

	return pTemp ;
}

void _j_MemeberQueue::PutNode(_FriendInfo * Node)
{	
	if( Node == NULL ) return ;

	
	userpool_lock() ;
	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.find( Node->GetName() ) ;
		if( m_FriendMapIter != m_FriendMap.end() ) {		
			m_FriendMap.erase(m_FriendMapIter) ;	// 맵에서 삭제			
		}		
		m_NodeQueue.push(Node) ;
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] void _j_MemeberQueue::PutNode(_FriendInfo * Node) \n") ;
	}
	userpool_unlock() ;

	Node->releas() ;
	
}

_FriendInfo * _j_MemeberQueue::FindNode(char * cName)
{

	if( cName == NULL )
		return NULL ;

	_FriendInfo * pTemp = NULL ;
	userpool_lock() ;
	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.find(cName) ;
		if ( m_FriendMap.end() != m_FriendMapIter ) {
			pTemp = m_FriendMapIter->second ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _FriendInfo * _j_MemeberQueue::FindNode(char * cName) \n") ;
		pTemp = NULL ;
	}	
	userpool_unlock() ;

	return pTemp ;
}


_FriendInfo * _j_MemeberQueue::MoveNext(_FriendInfo * p )
{
	if( p == NULL )
		return NULL ;

	userpool_lock() ;

	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.find( p->GetName() ) ;
		if( m_FriendMapIter != m_FriendMap.end() )
		{
			++m_FriendMapIter ;
			if( m_FriendMapIter != m_FriendMap.end() )
			{
				p = m_FriendMapIter->second ;
			}
			else
			{
				p = NULL ;
			}
		}
		else
		{
			p = NULL ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::MoveNext(_FriendInfo * p )\n" ) ;
		p = NULL ;
	}

	userpool_unlock() ;

	return p ;

}

_FriendInfo * _j_MemeberQueue::GetBegin()
{
	_FriendInfo * p = NULL ;

	userpool_lock() ;
	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.begin() ;
		if( m_FriendMapIter != m_FriendMap.end() )
		{			
			p = static_cast<_FriendInfo*>(m_FriendMapIter->second) ;
		}
	}
	catch (...) {
		p = NULL ;
	}	
	userpool_unlock() ;

	return p ;
}


void _j_MemeberQueue::DeleteMap(char * cName) 
{
	userpool_lock() ;
	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.find(cName) ;
		if( m_FriendMap.end() != m_FriendMapIter ){
			m_FriendMap.erase(m_FriendMapIter) ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] void _j_MemeberQueue::DeleteMap(char * cName)  \n ") ;
	}	
	userpool_unlock() ;
}

bool _j_MemeberQueue::AddFriend(char * cName, char * cToName) 
{	

	_FriendInfo * pMyInfo = FindNode(cName) ;	
	if( pMyInfo  == NULL ) {
		return false ;
	}

	_FriendInfo * pFriendInfo = FindNode(cToName) ;	
	if( pFriendInfo  == NULL ) {
		return false ;
	}

	bool bReturn = true ;	
	
	try{
		// My Info 에 삽입..		
		pMyInfo->AddFriend( cToName, pFriendInfo ) ;
		
		// Friend's Info 에 삽입..		
		pFriendInfo->AddFriend( cName, pMyInfo ) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] bool _j_MemeberQueue::AddFriend(char * cName, char * cToName)  \n" ) ;
		bReturn = false ;
	}
		
	
	return bReturn ;
}

bool _j_MemeberQueue::DelFriend(char * cName, char * cToName) 
{
	_FriendInfo * pMyInfo = NULL ;
	_FriendInfo * pFriendInfo = NULL ;

	pMyInfo = FindNode(cName) ;
	if( pMyInfo  == NULL ) {
		return false ;
	}

	pFriendInfo = FindNode(cToName) ;
	if( pFriendInfo  == NULL ) {
		return false ;
	}	

	try{

		pMyInfo->DeleteFriend( cToName ) ;
		pFriendInfo->DeleteFriend( cName ) ;
	
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] bool _j_MemeberQueue::DelFriend(char * cName, char * cToName)  \n " ) ;
		return false ;
	}

	
	
	return true ;
}

bool _j_MemeberQueue::TrdFriend(char * cName, char * cToName, char * cGroup) 
{
	_Member member = {0} ;

	_FriendInfo * pTempFriendInfo = NULL ;
	
	pTempFriendInfo = FindNode(cName) ;
	if( pTempFriendInfo  == NULL ) {
		return false ;
	}

	try{
		// 그릅 찾기..
		pTempFriendInfo->MoveFriend( cToName, cGroup ) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] bool _j_MemeberQueue::TrdFriend(char * cName, char * cToName, char * cGroup) \n" ) ;
		return false ;
	}

	

	return true ;

}

/*
bool _j_MemeberQueue::GetFriendList(char * cName, char * cList)
{
	int i = 0 ;

	_FriendInfo * pTempFriendInfo = FindNode(cName) ;
	if( pTempFriendInfo  == NULL ) {
		return false ;
	}

	try{
		pTempFriendInfo->FriendListConvertBinary( cList ) ; 
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] bool _j_MemeberQueue::GetFriendList(char * cName, char * cList) \n ") ;
		return false ;
	}

	
	return true ;
}
*/



bool _j_MemeberQueue::Logout(char * cName)
{

	bool bRet = false ;
	_FriendInfo * pFriend = NULL ;

	userpool_lock() ;
	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.find(cName) ;	
		if( m_FriendMapIter != m_FriendMap.end() )
		{
			bRet = true ;
			pFriend = m_FriendMapIter->second ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] bool _j_MemeberQueue::Logout(char * cName) \n " ) ;
		bRet = false ;
	}
	userpool_unlock() ;	

	if( pFriend )
		pFriend->Logout() ;

	return bRet ;
}





bool _j_MemeberQueue::UpdateState(char * cName, char cState)
{
	bool bRet = false ;
	_FriendInfo * pFriend = NULL ;

	userpool_lock() ;
	try{
		FriendMapIter m_FriendMapIter = m_FriendMap.find(cName) ;
		if( m_FriendMapIter != m_FriendMap.end() )
		{
			bRet = true ;
			pFriend = m_FriendMapIter->second ;
		}		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] bool _j_MemeberQueue::UpdateState(char * cName, char cState) \n") ;
		bRet = false ;
	}
	userpool_unlock() ;

	if( pFriend )
		pFriend->SetState( cState ) ;
	
	return bRet ;
}

//bool _j_MemeberQueue::ChatGroupChoice(char * cName, char * cList)
//{
//	m_FriendMapIter = m_FriendMap.find(cName) ;
//	return true ;
//	
//}

//============================================================================



DWORD WINAPI th_TimeOut( LPVOID lp ) 
{
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	
	LARGE_INTEGER li = {0, } ;
	
	_Temp_timerentry entry ;
	
	_j_Templet_Timer * pTimer ;
	pTimer = (_j_Templet_Timer*)lp ;
	

	int pos = 0 ;
	
	_h_DS_Com * pDSCom ;
	DWORD curTime ;	
		
	printf( "  => Start Timer\n" ) ;
	
	SetWaitableTimer( hTimer, &li, 50, NULL, NULL, TRUE ) ;
	
	while(pTimer->bThreadRun) {

		try{

			WaitForSingleObject( hTimer, INFINITE ) ;
			
			curTime = timeGetTime() ;
			
			while ( pTimer->Get_TimeMessage( curTime, entry ) == true ) {
				
				pDSCom = (_h_DS_Com*)entry.pDS_Com ;
				if( !pDSCom ) continue ;

				pos = 1 ;
				
				if( (pDSCom->get_id() > 0) && (pDSCom->m_pFriendInfo == NULL) )	// 접속만 하고 아직까지 로그인을 안한 사용자다.. 끊어버려라.
				{
					pos = 2 ;
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[KICK OUT] Overtime connection client \n") ;
#endif				
					pos = 3 ;
					pDSCom->DisConnect() ;

				}				
			}

		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_TimeOut pos:%d\n", pos) ;
		}
		
	}

	::PrintConsole("[THREAD EXIT] th_TimeOut \n") ;

	return 0 ;
}
























