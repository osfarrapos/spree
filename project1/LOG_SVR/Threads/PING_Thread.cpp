#include "..\stdafx.h"
#include "..\Global\Global_Define.h"




extern _j_GlobalLog * g_pSystemLog ;
extern char g_ip[17] ;



DWORD WINAPI th_UDPPingThread( LPVOID lp )
{
	// 1. 초기화	
	SOCKADDR_IN fromAddr ;		// recvfrom 에 인자로 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom 에 쓰일 변수	
	char		recvbuf[1024] ;			// 받기 버퍼
	int			bufSize = sizeof(char) * 1024 ;
	int			nReturn = 0 ;
	u_short		sPort = (unsigned short)lp ;
	int			passKey = _SERVER_AUTHOR_ ;
	SOCKET s ;
	
	// Initailize socket		
	fromAddr.sin_family = AF_INET ;
	fromAddr.sin_port = htons( sPort ) ;
	fromAddr.sin_addr.s_addr = inet_addr( g_ip ) ;

	
	s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
	if( s == INVALID_SOCKET ) {
		g_pSystemLog->LogFileWrite("[SO_ERR] INVALID_SOCKET. \n");
		_endthreadex(0);
		return 0;
	}
	
	if( bind( s, (SOCKADDR *)&fromAddr, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR ) {
		g_pSystemLog->LogFileWrite( "Error : ServerComThread socket bind %d, port = %u\n", WSAGetLastError(), sPort ) ;
		closesocket( s ) ;
		_endthreadex(0);
		return 0;
	}

	while (1) {
		// 2. Recv
		memset( recvbuf, 0, bufSize ) ;
		
		
		nReturn = recvfrom( s, recvbuf, bufSize, 0, (sockaddr*)&fromAddr, &frimLen ) ;
		if (nReturn == 0)
		{
			if( s == INVALID_SOCKET ) 
			{
				PrintConsole("[SO_ERR] INVALID_SOCKET. \n");
				closesocket( s ) ;
				_endthreadex(0);
				return 0 ;
			}
			continue ;
		}
		else if (nReturn == SOCKET_ERROR)
		{
			if( s == INVALID_SOCKET ) 
			{
				PrintConsole("[SO_ERR] INVALID_SOCKET. \n");
				closesocket( s ) ;
				_endthreadex(0);
				return 0 ;
			}
			Sleep(10000) ;		// 그냥 쉬어본다..
			continue ;
		}
		
		
		try {
			
			if( nReturn > 0 ) {
				
				// 암호가 틀리면 튕겨..
				if( *(int*)recvbuf != passKey ) {
					continue ;
				}
				
				if(  recvbuf[4] == 0 ){	// 0 번은 Ping 이다.
					// 리턴하기..
					sendto(s, recvbuf, nReturn, 0, (sockaddr*)&fromAddr, frimLen ) ;
				}
			}
			else {
				continue ;
			}
		}
		catch ( ... ) {
			::PrintConsole("[EXCEPTION] ping thread work \n") ;
			continue ;
		}
		
	}
	
	PrintConsole( "[EXIT] th_UDPPingThread Return\n " ) ;
	_endthreadex(0);
	return 0 ;
}
