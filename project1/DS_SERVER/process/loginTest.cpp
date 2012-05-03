#ifdef _LOGIN_STRESS_TEST_

#include "..\stdafx.h"
#include "..\packet_queue\packet_queue.h"


extern _j_queue_UDP_Packet *	g_pUDPLoinPacketQ ;

unsigned long __stdcall th_loginTest( LPVOID lp )
{
	UMSG_LOGIN_LS_DS msglogin = {0} ;
	msglogin.cMessage = UMSG_LOGIN_LS_DS_NUM ;
	msglogin.iKey = PASS_KEY ;
	msglogin.cServerNum = 0 ;

	SOCKADDR_IN addr ;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons(7410) ;
	addr.sin_addr.s_addr = inet_addr("221.147.34.21") ;
	
	int i = 0 ;
	while (1) {

		memset( msglogin.cAccount, 0, 13 ) ;		
		msglogin.iTempUniqueID = 1 ;		

		if( i < 10 )
		{
			sprintf( msglogin.cAccount, "dump1000%d", i ) ;
		}
		else if( i < 100 )
		{
			sprintf( msglogin.cAccount, "dump100%d", i ) ;
		}
		else if( i < 1000 )
		{
			sprintf( msglogin.cAccount, "dump10%d", i ) ;
		}
		else if( i < 5000 )
		{
			sprintf( msglogin.cAccount, "dump1%d", i ) ;
		}	
		else 
		{
			break ;
		}
		g_pUDPLoinPacketQ->insert( &addr, (char*)&msglogin, sizeof(msglogin) ) ;

		++i ;
		::PrintConsole("-- %s \n", msglogin.cAccount ) ;

		Sleep(0) ;
	}

	return 0 ;

}

#endif