#include "..\stdafx.h"
#include "..\Global\Global_Define.h"

SOCKET RestoreSocket( const unsigned short usPortNum, char mode )  ;
void PrintConsole( const char *format,... ) ;

DWORD WINAPI th_UDPPingThread( LPVOID lp )
{
	
	// 1. 초기화	
	SOCKADDR_IN fromAddr ;		// recvfrom 에 인자로 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom 에 쓰일 변수	
	char		recvbuf[2048] ;			// 받기 버퍼
	int			bufSize = sizeof(recvbuf)  ;
	int			nReturn = 0 ;
	u_short		sPort = (unsigned short)lp ;
	int			passKey = PASS_KEY ;
	SOCKET s ;
	
	// Initailize socket	
	s = RestoreSocket(sPort, 0) ;
	if( s == INVALID_SOCKET ) 
	{
		printf("[ERROR] Ping - UDP Socket failed : %d\n", WSAGetLastError() ); 
		return 0;
	}
	
	while (1) {
		// 2. Recv
				
		try{
			nReturn = recvfrom( s, recvbuf, bufSize, 0, (sockaddr*)&fromAddr, &frimLen ) ;
			if (nReturn == 0)
			{
				PrintConsole("[SO_ERR] UDP Recv failed / 서버 접속 종료\n");
//				g_pErrorMsg->ErrMsg("[SO_ERR] UDP Recv failed / 서버 접속 종료\n");
				if( s == INVALID_SOCKET ) 
				{
					printf("소켓을 생성했습니다. \n");
					closesocket( s ) ;
					s = RestoreSocket(sPort, 0) ;
					Sleep(1000) ;		// 그냥 쉬어본다..	
				}						
				continue ;
			}
			else if (nReturn == SOCKET_ERROR)
			{			
				nReturn = WSAGetLastError() ;
				if( nReturn == WSAETIMEDOUT )
				{
					continue ;
				}
				PrintConsole("[SO_ERR] UDP Recv failed / Recv 실패. Error No.%d\n", WSAGetLastError());
				//g_pErrorMsg->ErrMsg("[SO_ERR] UDP Recv failed / Recv 실패. Error No.%d\n", WSAGetLastError());
				if( s == INVALID_SOCKET ) 
				{
					printf("소켓을 생성했습니다. \n");
					closesocket( s ) ;
					s = RestoreSocket(sPort, 0) ;
				}			
				Sleep(1000) ;		// 그냥 쉬어본다..			
				continue ;
			}
			else if( nReturn > 0 ) {

				// 리턴하기..
				if( *(int*)recvbuf == passKey )
					sendto(s, recvbuf, nReturn, 0, (sockaddr*)&fromAddr, frimLen ) ;
			}
			else {
				continue ;
			}

		}
		catch (...) {
			::PrintConsole( "[EXCEPTION] thread exception \n" ) ;
			Sleep(50) ;	// 통제없는 무한 루프에 빠질 수 있으니깐.. 
		}		
		
	}
		
	PrintConsole( "[EXIT] th_ServerCom Return\n " ) ;
	
	return 0 ;
}

