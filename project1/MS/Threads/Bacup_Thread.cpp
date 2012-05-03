#include "..\stdafx.h"
#include "..\OLE-DB\db_command.h"
#include "..\VMManager\VMManager.h"
#include "..\Map\MAPManager.h"

extern _j_BackupReady * g_pBackupReady ;
extern _j_MemeberQueue *	g_pMemberQueue ;
extern bool g_pThreadRunning ;
extern CDB_Connector	*	g_pDBCon ;


DWORD WINAPI th_FriendListBackupthread( LPVOID lp )	// 캐릭터 백없 쓰레드
{

	COLE_DB_Executer *		pBackupDBCommand ;

	pBackupDBCommand = new COLE_DB_Executer(g_pDBCon ) ;
	assert(pBackupDBCommand) ;	

	_FriendInfo * pFirendInfo ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, _BACKUP_INTAERVAL_TIME_, NULL, NULL, TRUE ) ;

	
	while (g_pThreadRunning) {		
		pFirendInfo = g_pBackupReady->GetNode() ;
		if( pFirendInfo == NULL) {
			WaitForSingleObject( hTimer, INFINITE ) ;
			//Sleep(_BACKUP_INTAERVAL_TIME_) ;
			continue ;
		}

		try{
			pFirendInfo->UpdateDB( pBackupDBCommand ) ;				
			if( pFirendInfo->IsLogon() == false )
			{
				g_pMemberQueue->PutNode( pFirendInfo ) ;				
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_FriendListBackupthread \n") ;
		}
		Sleep( 5 ) ;
		
	}


	printf("[EXIT] th_FriendListBackupthread exit \n") ;
	::PrintConsole("[EXIT] th_FriendListBackupthread exit \n") ;
	delete pBackupDBCommand ;
	return 0 ;
	

}
