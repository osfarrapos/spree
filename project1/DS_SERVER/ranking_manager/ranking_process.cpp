#include "..\stdafx.h"

extern bool g_Threading ;
extern CDB_Connector *	g_pDBCon ;
unsigned long __stdcall	th_RankingVersionChecker( LPVOID lp)
{

	COLE_DB_Executer *	g_DB_Command = new COLE_DB_Executer(g_pDBCon) ;
	assert( g_DB_Command ) ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0} ;
	const int iSleepTime = 60000 ;	// 1Ка


	SetWaitableTimer( hTimer, &li, iSleepTime, NULL, NULL, TRUE ) ;	

	while(g_Threading)
	{

		WaitForSingleObject( hTimer, INFINITE ) ;
		if( !g_Threading ){
			break ;
		}

		try{
			g_DB_Command->GetRankVersionInfo() ;
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
	}

	return 0 ;
}
