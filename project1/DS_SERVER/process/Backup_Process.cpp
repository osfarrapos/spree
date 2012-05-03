#include "..\stdafx.h"

#include "..\MAPManager\MAPManager.h"


extern _j_AccMAPManager *		g_pAccMapManager;	// Account KEY MAP
extern int g_iCharacBackupTime;
extern int g_iAccBackupTime;

extern _j_BackupReady * g_pBackupReady ;
extern CDB_Connector *	g_pDBCon;

extern HANDLE	g_hInitEvent;

extern bool g_Threading ;

DWORD WINAPI th_CharacBackupthread( LPVOID lp )	// 캐릭터 백없 쓰레드
{

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0} ;


	PCHARAC pCharac;
	int	ComplCount;			// 완료한 백업 갯수
	int StepCount;			// 진행된 총 백업 갯수
	

	vector<PCHARAC> * pCharacReady = new vector<PCHARAC> ;
	assert( pCharacReady ) ;
	pCharacReady->reserve( _PERMISSIBLE_MAX_USER ) ;

	int iCReadyCount = 0 ;
	

	COLE_DB_Executer *			g_DB_Command = NULL;
	g_DB_Command = new COLE_DB_Executer(g_pDBCon) ;
	assert( g_DB_Command ) ;

	SetEvent( g_hInitEvent );

	SetWaitableTimer( hTimer, &li, _BACKUP_TIME_USER_, NULL, NULL, TRUE ) ;	// 백업 주기
	

	while ( g_Threading ) {		

		WaitForSingleObject( hTimer, INFINITE ) ;
		if( !g_Threading ){
			break ;
		}
		
		
		if( g_pBackupReady->IsCharacEmpty() ){
			continue;        
		}
		else {
			g_pBackupReady->GetCharacBackupNode( pCharacReady ) ;

		}		
			


		ComplCount = 0;
		StepCount = pCharacReady->size() ;
		
		while ( !pCharacReady->empty() ) {	
			

			try{
				
				pCharac = pCharacReady->back() ;
				pCharacReady->pop_back() ;
				
				if( pCharac == NULL ){					
					continue;
				}
				else{							
										
					if( pCharac )
					{
						pCharac->charac_lock() ;						
						try{
							if( pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED != BS_COMPLETED )
							{
								if(!g_DB_Command->Update_Charac_All(pCharac->CHARAC_BASIC.iUniqueID, pCharac)){						
									// Write log
									::PrintConsole("[DB_ERR] 캐릭터 데이타 백업 실패 (%s) \n", pCharac->CHARAC_BASIC.cChrName ) ;							
								}
								else{
									//g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Backup Character info ( Charac name : %s ) \n", pCharac->CHARAC_BASIC.cChrName ) ;

									pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = BS_COMPLETED;
									ComplCount++;
								}
							}


						}
						catch(...){
							::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
						}
						pCharac->charac_unlock() ;
					}
					
				}				
				
			}//try
			catch (...) {
				::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
				::PrintConsole( "[EXCEPTION] 캐릭터 백업 쓰레드에서 예외오류 발생 \n"  ) ;				
			}			
			//Sleep(10) ;
		}//while(1)
		

		// 로그 남기자..
#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Complete characters backup (%d/%d) \n", ComplCount, StepCount);
#endif
				
	}

	
	delete pCharacReady ;		
	delete g_DB_Command;			

	::PrintConsole("DWORD WINAPI th_Backupthread( LPVOID lp ) Exit \n");
	printf("DWORD WINAPI th_Backupthread( LPVOID lp ) Exit \n");	

		
	return 0;
}


DWORD WINAPI th_AccountBackupThread( LPVOID lp )
{
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0} ;

	PUSER pUser = NULL;
	int	ComplCount;			// 완료한 백업 갯수
	int StepCount;			// 진행된 총 백업 갯수


	vector<PUSER> * pUserReady = new vector<PUSER> ;
	assert( pUserReady ) ;
	pUserReady->reserve( _PERMISSIBLE_MAX_USER ) ;
	
	COLE_DB_Executer * g_DB_Command = new COLE_DB_Executer(g_pDBCon) ;
	assert( g_DB_Command ) ;

    SetEvent( g_hInitEvent );

	SetWaitableTimer( hTimer, &li, _BACKUP_TIME_USER_, NULL, NULL, TRUE ) ;
			
	while ( g_Threading ) {	

		WaitForSingleObject( hTimer, INFINITE ) ;
		if( !g_Threading ){
			break ;
		}
		
		
		if( g_pBackupReady->IsAccEmpty() ){	
			continue;        
		}
		else {
			pUserReady->clear() ;
			g_pBackupReady->GetUserBackupNode( pUserReady ) ;
		}

		StepCount = pUserReady->size() ;
		ComplCount = 0 ;


		while ( !pUserReady->empty() ) {	

			try{

				pUser = pUserReady->back() ;
				pUserReady->pop_back() ;

				if ( pUser == NULL ) {					
					continue ;
				}
				else {

					pUser->user_lock() ;
					try
					{
						if( pUser->cDepotBackupState != BS_COMPLETED ) 
						{
							if( !g_DB_Command->UpdateAccountItemUseStatus( pUser->iUid, pUser ) )
							{
								::PrintConsole( "[DB_ERR] AIUS Backup failed (%s)\n ", pUser->cNDAccount );
							}

							if(!g_DB_Command->pr_UpdateDepot(pUser->iUid, pUser) ) 
							{
								// 로그에 남기자..
								::PrintConsole( "[DB_ERR] Storage data backup failed (%s)\n ", pUser->cNDAccount );	
							}		
							else
							{							
								pUser->cDepotBackupState = BS_COMPLETED;							
								//g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Backup Account's depot ( Charac name : %s ) \n", pUser->cNDAccount ) ;		
							}						
						}
					}
					catch(...){
						::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
					}
					pUser->user_unlock() ;
					
					ComplCount++ ;
				}
				
			}
			catch(...){
				::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
				::PrintConsole( "[EXCEPTION] 계정 백업 쓰레드에서 예외 오류 발생 \n" ) ;
			}
			//Sleep(5);
		}//while(1)
		

		// 로그 남기자..
#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Complete accounts backup (%d/%d) \n", ComplCount, StepCount);
#endif

	}


	delete pUserReady ;		
	delete g_DB_Command;		
    
	::PrintConsole("DWORD WINAPI th_AccountBackupThread( LPVOID lp ) Exit \n ") ;
	printf("DWORD WINAPI th_AccountBackupThread( LPVOID lp ) Exit \n ") ;
		
	return 0;
}




