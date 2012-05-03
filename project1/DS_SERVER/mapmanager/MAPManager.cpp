#include "..\stdafx.h"
#include "MAPManager.h"

extern _j_AccMAPManager * g_pAccMapManager;	// AccMap Manager

void _j_AccMAPManager::Insert(PUSER pUser)
{	

	if( pUser == NULL || pUser->cNDAccount[0] == 0 )
		return ;

	user_map_lock() ;
	try{		
		//  계정 등록
		m_UserMap[pUser->cNDAccount] = pUser;
		pUser->bMemState = US_USING ;		
		
		PCHARAC pCharac ;
		for( int i = 0 ; i < pUser->cCharacCount ; i++)
		{
			pCharac = static_cast<PCHARAC>(&pUser->stCharac[i]) ;
			CharacInsert( pCharac ) ;				
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_AccMAPManager::Insert() \n");		
	}
	user_map_unlock() ;
	
}

bool _j_AccMAPManager::Erase(PUSER pUser)
{

	if( pUser == NULL || pUser->cNDAccount[0] == 0 )
		return false ;


	bool bReturn = false ;
	map<std::string, PUSER>::iterator useritr ;

	user_map_lock() ;
	try{
		useritr = m_UserMap.find(pUser->cNDAccount);
	
		if (useritr != m_UserMap.end()) 
		{
			m_pUser = useritr->second;			
			if( m_pUser )
			{
				PCHARAC pCharac ;

				for( int i = 0 ; i < 3 ; i++ )
				{						
					pCharac = static_cast<PCHARAC>(&m_pUser->stCharac[i]) ;
					if( pCharac && pCharac->CHARAC_BASIC.iUniqueID != 0 )
					{
						pCharac->charac_lock() ;
						try{
							if( CharacErase(pCharac) == false ) 
							{
								::PrintConsole( "[ERROR] if( CharacErase(pCharac) == false )  \n %s, %d \n", __FILE__, __LINE__ ) ;
							}
						}
						catch(...){
							::PrintConsole("[EXEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						pCharac->charac_unlock() ;
					}
				}
				// 맵에서 계정 삭제				
				m_UserMap.erase(useritr);
				m_UserQueue.push( m_pUser ) ;
				m_pUser->bMemState = US_SAVED ;

				bReturn = true ;
			}
			else
			{
				bReturn = false ;
			}
		}
		else
		{
			bReturn = false ;
		}	
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_AccMAPManager::Erase() \n");
		bReturn = false ;
	}
	user_map_unlock() ;

	return bReturn ;	
}

PUSER _j_AccMAPManager::Find_User(char * szIndex)		//szIndex 는 캐릭터의 유니크 아이디가 되겠다..
{	

	if( szIndex == NULL || szIndex[0] == 0 ) 
	{
		return NULL ;
	}

	PUSER pUser ;
	map<std::string, PUSER>::iterator useritr ;
	
	user_map_lock() ;

	try{

		useritr = m_UserMap.find(szIndex);
		if (useritr == m_UserMap.end())
		{
			pUser = NULL ;
		}
		else
		{
			pUser = useritr->second;
		}
		
	}
	catch (...) {		
		::PrintConsole("[EXCEPTION] _j_AccMAPManager::find(%s) \n", szIndex);
		pUser = NULL ;
	}	

	user_map_unlock() ;
	
	return pUser;
}

PUSER _j_AccMAPManager::GetWritePtr()
{

	if( m_UserQueue.empty() ){	// 메로리 풀이 바닥났다..
		::PrintConsole("-->[ME_ERR] PUSER Mem pool is empty !!\n");
		return NULL;
	}
	
	PUSER pUser ;

	user_map_lock() ;
	try{
		pUser = m_UserQueue.front() ;
		if( pUser )
		{
			pUser->Reset() ;
			m_UserQueue.pop() ;
		}		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION]_j_AccMAPManager::GetWritePtr() \n");
		pUser = NULL ;
	}
	user_map_unlock() ;

	return pUser ;
	
}


void _j_AccMAPManager::PutMemPtr( PUSER pUser )	// use with GetWritePtr()
{
	if( pUser == NULL )
	{
		return ;
	}

	user_map_lock() ;
	try{		
		m_UserQueue.push( pUser ) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION]_j_AccMAPManager::PutMemPtr() \n");
	}
	user_map_unlock() ;
}



PUSER _j_AccMAPManager::MoveBeginUser()
{

	PUSER pUser ;
	map<std::string, PUSER>::iterator useritr ;

	user_map_lock() ;
	try{
		if(m_UserMap.empty()){
			pUser = NULL ;
		}
		else{
			useritr = m_UserMap.begin();	
			pUser = useritr->second;
		}	
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_AccMAPManager::MoveBegin() \n");
		pUser = NULL ;
	}
	user_map_unlock() ;
	

	return pUser ;
}

PUSER _j_AccMAPManager::MoveNextUser( PUSER pUser )
{

	PUSER pTempUser = NULL ;
	map<std::string, PUSER>::iterator useritr ;
	

	user_map_lock() ;
	try{
		useritr = m_UserMap.find(pUser->cNDAccount);
		if (useritr != m_UserMap.end())
		{
			++useritr ;		
			if (useritr != m_UserMap.end())
			{
				pTempUser = useritr->second;
				if( pTempUser->cNDAccount[0] == 0 )
				{					
					printf( "[EEROR] USER BAD DATA : M_Key. <%s> \n", pTempUser->cNDAccount ) ;
					pTempUser->user_lock() ;
					try{
						if( Erase( pTempUser) == false )
						{
							::PrintConsole("[ERROR] if( g_pAccMapManager->Erase( pUser) == false ) \n %s, %d \n", __FILE__,__LINE__) ;
						}
					}
					catch(...){
						::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
					}		
					pTempUser->user_unlock() ;
					pTempUser = NULL ;
				}
			}
			else
			{			
				pTempUser = NULL ;				
			}
		}
		else
		{
			pTempUser = NULL ;
		}
		
	}
	
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_AccMAPManager::MoveNext() \n");
		pTempUser = NULL ;
	}
	user_map_unlock() ;

	return pTempUser;
}

/*
bool _j_AccMAPManager::IfEndUser( PUSER pUser)
{

	bool bReturn = false ;

	a_lock() ;
	try{
		if(pUser == (m_UserMap.end())->second){
			bReturn = true ;
		}
	}
	catch (...) {
	}
	a_unlock() ;
	return bReturn ;
	
}
*/



/////////////////////////////////////////////////////////////////////////////
/* 캐릭터 이름(char *) 를 Key로 갖는 MAP */
/////////////////////////////////////////////////////////////////////////////

void _j_AccMAPManager::CharacInsert(PCHARAC pCharac)
{
	if( pCharac == NULL || pCharac->CHARAC_BASIC.cChrName[0] == 0 || pCharac->CHARAC_BASIC.iUniqueID == 0 )
		return ;

	charac_map_lock() ;
	try{
		m_CharacMap[pCharac->CHARAC_BASIC.cChrName] = pCharac;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_ChrMAPManager::Insert() \n");		
	}
	charac_map_unlock() ;
	

}

bool _j_AccMAPManager::CharacErase(PCHARAC pCharac)
{

	if( pCharac == NULL || pCharac->CHARAC_BASIC.iUniqueID == 0 )
		return false ;


	bool bReturn = false ;
	map<std::string, PCHARAC>::iterator characitr;

	charac_map_lock() ;
	try{
		characitr = m_CharacMap.find(pCharac->CHARAC_BASIC.cChrName);
		if (characitr == m_CharacMap.end()){

			::PrintConsole("[ME_ERR]해당 데이타가 존재하지 않습니다. \n");

			bReturn = false ;
		}
		else
		{
			pCharac->Reset() ;
			m_CharacMap.erase(characitr);
			bReturn = true ;
		}
		
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_ChrMAPManager::Erase() \n");
        bReturn = false ;
	}
	charac_map_unlock() ;

	return bReturn ;	
}



PCHARAC _j_AccMAPManager::Find_Charac(char * szIndex)		//szIndex 는 캐릭터의 유니크 아이디가 되겠다..
{	
	PCHARAC pCharac = NULL ;
	map<std::string, PCHARAC>::iterator characitr;

	charac_map_lock() ;
	try{
		characitr = m_CharacMap.find(szIndex);
		if (characitr == m_CharacMap.end()){
			pCharac = NULL ;
		}
		else
		{
			pCharac = characitr->second;
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_ChrMAPManager::find() \n");
		pCharac = NULL ;
	}
	charac_map_unlock() ;
	
	return pCharac;
}

PCHARAC _j_AccMAPManager::MoveBeginCharac()
{
	PCHARAC pCharac = NULL ;
	map<std::string, PCHARAC>::iterator characitr;

	charac_map_lock() ;
	try{
		if( m_CharacMap.empty() == false )
		{
			characitr = m_CharacMap.begin();	
			pCharac = characitr->second;
		}	
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_ChrMAPManager::MoveBegin() \n");
		pCharac = NULL ;
	}
	charac_map_unlock() ;
		
	return pCharac ;
}

PCHARAC _j_AccMAPManager::MoveNextCharac( PCHARAC pCharac )
{
	if( pCharac == NULL || pCharac->CHARAC_BASIC.iUniqueID == 0 )
		return NULL ;	

	map<std::string, PCHARAC>::iterator characitr;
	charac_map_lock() ;

	try{
		
		characitr = m_CharacMap.find( pCharac->CHARAC_BASIC.cChrName ) ;
		if( characitr != m_CharacMap.end() )
		{
			++characitr;
			if( characitr != m_CharacMap.end() )
			{			
				pCharac = characitr->second;
				if( pCharac && pCharac->CHARAC_BASIC.cChrName[0] == 0 )
				{
					printf( "[EEROR] CHARAC BAD DATA : Key. <%s> \n", pCharac->CHARAC_BASIC.cChrName) ; ;
					pCharac->charac_lock() ;
					try{
						if( CharacErase(pCharac) == false ) 
						{
							::PrintConsole( "[ERROR] if( g_pAccMapManager->CharacErase(pCharac) == false )  \n %s, %d \n", __FILE__, __LINE__ ) ;
						}
					}
					catch(...){
						::PrintConsole("[EXEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
					}
					pCharac->charac_unlock() ;
					pCharac = NULL ;
				}
			}
			else
			{
				pCharac = NULL ;
			}
		}
		else
		{
			pCharac = NULL ;
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_ChrMAPManager::MoveNext() \n");
		pCharac = NULL ;
	}	

	charac_map_unlock() ;

	return pCharac;
}

bool _j_AccMAPManager::CharacRename(char * oldName, char * newName) 
{
	
	bool bReturn = false ;
	map<std::string, PCHARAC>::iterator characitr;

	charac_map_lock() ;
	try{
		characitr = m_CharacMap.find(oldName);
		if (characitr == m_CharacMap.end()){
#ifdef _DEBUG
			::PrintConsole("[ME_ERR]해당 데이타가 존재하지 않습니다. \n");
#endif					
			bReturn = false ;
		}
		else
		{	
			PCHARAC pCharac = characitr->second;
			m_CharacMap.erase(characitr);
			memcpy( pCharac->CHARAC_BASIC.cChrName, newName, 13 ) ;
			m_CharacMap[newName] = pCharac ;
			bReturn = true ;
		}


	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_ChrMAPManager::Erase() \n");
		bReturn = false ;
	}
	charac_map_unlock() ;

	return bReturn ;
}

/*
bool _j_AccMAPManager::IfEndCharac( PCHARAC pCharac)
{
	bool bReturn = false ;

	c_lock() ;
	
	try{
		if(pCharac == (m_CharacMap.end())->second){
			bReturn = true ;
		}
	}
	catch (...) {
		bReturn =false ;
	}

	c_unlock() ;

	return bReturn ;
	
}
*/




// class CUserMemSaveTimer 
CUserMemSaveTimer::Run()
{
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	
	_sMemSave_timerentry entry ;

	LARGE_INTEGER li = {0, } ;
	DWORD curTime ;


	PUSER	arPUSER[_PERMISSIBLE_MAX_USER] = {0} ;
	INT_PTR	iWorkCount = 0 ;
	PUSER pUser ;	

	SetWaitableTimer( hTimer, &li, 500, NULL, NULL, TRUE ) ;

	while (1) 
	{

		WaitForSingleObject( hTimer, INFINITE ) ;
		curTime = CUserMemSaveTimer::get_time_by_sec() ;

		while ( GetMessage( curTime, entry ) == true ) {
			pUser = entry.pUser ;

			if( pUser )	
			{
				pUser->user_lock() ;
				try{
					if( pUser->bMemState == US_USING 
						&& pUser->cState == US_NONSTATE 
						&& pUser->iUid == entry.iID
						&& pUser->usTempUniqueID == 0 				
						&& pUser->cDepotBackupState == 0
						&& pUser->cSelChrName[0] == 0
						&& pUser->stCharac[0].CHARAC_ETC._sBACKUP_STATE_CHECKED == 0 
						&& pUser->stCharac[0].CHARAC_ETC.cState == 0 
						&& pUser->stCharac[1].CHARAC_ETC._sBACKUP_STATE_CHECKED == 0 
						&& pUser->stCharac[1].CHARAC_ETC.cState == 0 
						&& pUser->stCharac[2].CHARAC_ETC._sBACKUP_STATE_CHECKED == 0 
						&& pUser->stCharac[2].CHARAC_ETC.cState == 0 )
					{
						// 맵에서 삭제하고 메모리를 큐에 반환한다.
						// m_pAccMapManager == g_pAccMapManager
						if( m_pAccMapManager->Erase( pUser ) )
						{					
#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[INFO] User Mem save (%s) \n", pUser->cNDAccount ) ;
#endif
						}
					}
				}
				catch(...){
					::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
				}				
				pUser->user_unlock() ;
				pUser = NULL ;
			}
			else
			{
#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[INFO] User Mem pass \n" ) ;
#endif
			}

			WaitForSingleObject( hTimer, INFINITE ) ;	// 프로세스의 독점을 막기 위해 일부러 지연을 시켰다.
		}			

	}
}