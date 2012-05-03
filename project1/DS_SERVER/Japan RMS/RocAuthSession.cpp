#include "../StdAfx.h"

#ifdef _PD_COMPANY_JAPAN_

#include "RocAuthSession.h"

SOCKADDR_IN CRocAuthSession::m_authServer = {0,};

BOOL CRocAuthSession::SetRocworksAuthServerInformation( const char * ip, const unsigned short port )
{
	m_authServer.sin_family = AF_INET;
	m_authServer.sin_addr.s_addr = inet_addr( ip );
	m_authServer.sin_port = htons( port );
	
	g_pSystemLog->LogFileWrite("Roc Auth Server IP = %s, Port = %u\n\n", 
		inet_ntoa( m_authServer.sin_addr ), ntohs(m_authServer.sin_port) );

	char msg[80];
	sprintf( msg, "[RMS] IP %s - PORT %u", inet_ntoa( m_authServer.sin_addr ), ntohs(m_authServer.sin_port) );
	if( IDOK == MessageBox( NULL, msg, "Check RMS", MB_OKCANCEL ) )
	{
		return TRUE;
	}
	else
		return FALSE;
}


void CRocAuthSession::ShowServerInformation()
{
	printf( "\n  Roc Auth Server IP = %s, Port = %u\n\n", 
		inet_ntoa( m_authServer.sin_addr ), ntohs(m_authServer.sin_port) );
}

//////////////////////////////////////////////////////////////////////////

void CRocAuthSession::SetTimeout( DWORD ms /* = rockworks_socket_timeout  */)
{
	if( m_socket )
	{
		int timeo = static_cast<int>(ms);
		::setsockopt( m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(int) );
	}
}



int CRocAuthSession::Connect()
{
	// 연결.
	// 주소가 설정되었는지 검사한다. 
	if( m_authServer.sin_family )
	{
		m_socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
		if( INVALID_SOCKET != m_socket )
		{
			int nZero = TRUE ;
			::setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, ( char* )&nZero, sizeof( nZero ) );
			
			if( SOCKET_ERROR == ::connect( m_socket, (SOCKADDR*)&m_authServer, sizeof(m_authServer) ) )
			{
				SetError( WSAGetLastError() );
				PrintConsole( "[INIT FAIL] Connecting RocWorks Auth Server fail(%d)\n", GetError() );
				return ND_F_FAIL;
			}
			else
			{
				int nZero = TRUE ;
				::setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, ( char* )&nZero, sizeof( nZero ) );
				
				m_iConnectState = rocworks_connect_state_connected;
				SetTimeout();
				return ND_F_SUCCESS;
			}
		}
		else
		{
			SetError( WSAGetLastError() );
			PrintConsole( "[INIT FAIL] Create socket(RocWorks Auth Server) fail(%d)\n", GetError() );
			return ND_F_FAIL;
		}
	}
	else
	{
		SetError( -1 );
		PrintConsole( "[INIT FAIL] Server address is not ready.\n,Please call SetRocWorksAuthServerInformation() first\n" );
		return ND_F_FAIL;
	}
}



void CRocAuthSession::Disconnect( bool bEnd /* = false  */ )
{
	if( m_socket )
	{
		::closesocket( m_socket );
		m_socket = INVALID_SOCKET;
	}

	if( bEnd )
		m_iConnectState = rocworks_connect_state_end;
	else
		m_iConnectState = rocworks_connect_state_none;
}


void CRocAuthSession::SendData( void * pData, size_t length )
{
	if( rocworks_connect_state_connected == m_iConnectState )
	{
		CopyMemory( m_send_data, pData, length );

		if( 0 >= ::send( m_socket, m_send_data, length, 0 ) )
		{
			m_lastError = GetLastError();

			Disconnect();
		}
	}
}

int CRocAuthSession::RecvData()
{
	try
	{
		if( rocworks_connect_state_connected == m_iConnectState )
		{
			ZeroMemory( m_recv_data, sizeof(m_recv_data) );
			// 우선, 모두 받아본다.
			int length = 0;
			int rt = Recv( 4 );
			if( ND_F_SUCCESS == rt )
			{
				// 프린트 해본다.
//				PrintBuffData( m_recv_data );

				length = atoi( m_recv_data );

				//printf( "[ROC AUTH] RECV Length : %d\n", length );

				ZeroMemory( m_recv_data, sizeof(m_recv_data) );

				if( length > 4 && length < rocworks_recvbuf_size )
				{
					rt = Recv( length );
					if( ND_F_SUCCESS == rt )
					{
						//PrintBuffData( m_recv_data );

						RocSetResult( m_recv_data );
						return ND_F_SUCCESS;
					}
				}
				/*
				else
				{
					Disconnect();
				}
				*/
			}
		}
	}
	catch( ... )
	{
		PrintConsole( "[RMS] Exception in RecvData() : Code %d\n", GetLastError() );
	}

	return ND_F_FAIL;
}

int CRocAuthSession::Recv( int wanted_byte )
{
	if( wanted_byte <= 0 )
		return ND_F_SUCCESS;

	if( rocworks_connect_state_connected == m_iConnectState )
	{
		int recv_byte = 0;
		int ret = 0;
		int count = 2;
		while( recv_byte < wanted_byte && count-- > 0 )
		{
			ret = ::recv( m_socket, &m_recv_data[recv_byte], wanted_byte-recv_byte, 0 );
			if( ret > 0 )
				recv_byte += ret;
			else
			{
				break;
			}
		}

		if( recv_byte >= wanted_byte )
		{
			return ND_F_SUCCESS;
		}
		else
		{
			SetError( WSAGetLastError() );
			PrintConsole( "[RMS] Recv Error : recv %d bytes %d ( code = %d )\n", recv_byte, GetError() );
			//Disconnect();
			return ND_F_FAIL;
		}
	}
	/*
	else if( rocworks_connect_state_none == m_iConnectState )
	{
		Connect();
		return ND_F_FAIL;
	}
	*/

	return ND_F_FAIL;
}


//////////////////////////////////////////////////////////////////////////


int CRocAuthSession::RocRequestLogin( char * account, char * password, sRocAuthResponse ** ppRes )
{
	if( ND_F_SUCCESS == Connect() )
	{
		m_authRequestObj.init();

		m_authRequestObj.set_account( account );
		m_authRequestObj.set_password( password );

		ZeroMemory( m_send_data, sizeof(m_send_data) );

		m_authRequestObj >> m_send_data;

		SendData( m_send_data, m_authRequestObj.get_length() );

		PrintConsole( "[RMS] A<%s> Request Login()\n", account );

		//PrintBuffData( m_send_data );

		return RocWaitResponse( ppRes );
	}
	else
		return ND_F_FAIL;
}

int CRocAuthSession::RocWaitResponse( sRocAuthResponse ** ppRes )
{
	if( ND_F_SUCCESS == RecvData() )
	{
		*ppRes = &m_authResponse;
		return ND_F_SUCCESS;
	}

	*ppRes = NULL;
	return ND_F_FAIL;
}


int CRocAuthSession::RocSetResult( char * response )
{
	// 결과를 셋팅한다.
	m_authResponse << response;

	PrintConsole( "[RMS] Recv Response : Flag = %c\n", m_authResponse.m_success_fail_flag );

	return ND_F_SUCCESS;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void PrintBuffData( char * buf )
{
	char * p = buf;
	int real = 0;

	printf( "BUF " );

	while( *p != 0 && real < 512 )
	{
		printf( "%02x ", *p );
		++p;
		++real;
	}

	printf( "(%d)\n", real );

	printf(  " : %s\n", buf );
}

#endif