#ifndef __INDY_ND_SERVER_ROCAUTHSESSION_H__20070720__
#define __INDY_ND_SERVER_ROCAUTHSESSION_H__20070720__

#ifdef _PD_COMPANY_JAPAN_

#include "../mylibrary/ND_MD5Dll.h"

struct sRocAuthRequest
{
	char m_contents_code[4];	// "4006"
	char m_command[4];
	char m_account[64];			//
	char m_password[64];		//
	char m_md5_password[33];	// 
	char m_return_code[4];		// "RC01"
	size_t	m_length;

	sRocAuthRequest()
	{
		m_contents_code[0] = '4';
		m_contents_code[1] = '0';
		m_contents_code[2] = '0';
		m_contents_code[3] = '6';

		m_command[0] = 'R';
		m_command[1] = 'B';
		m_command[2] = 'A';
		m_command[3] = 'E';

		m_return_code[0] = 'R';
		m_return_code[1] = 'C';
		m_return_code[2] = '0';
		m_return_code[3] = '1';

		init();
	}
	void init()
	{
		ZeroMemory( m_account, sizeof(m_account));
		ZeroMemory( m_password, sizeof(m_password));
		ZeroMemory( m_md5_password, sizeof(m_md5_password));
	}

	void set_account( char * account )	{ strncpy( m_account, account, 63 ); m_account[63] = 0; }
	void set_password( char * pass )	{ strncpy( m_password, pass, 63 ); m_password[63] = 0; }
	size_t get_length()					{ return m_length; }
	
	void operator>>(char * buffer)
	{
		int pos = 4;
		__try
		{
			memcpy( buffer+pos, m_contents_code, sizeof(m_contents_code) );
			pos += sizeof(m_contents_code);

			*(buffer+pos) = ',';
			pos++;

			memcpy( buffer+pos, m_command, sizeof(m_command) );
			pos += sizeof(m_command);

			*(buffer+pos) = ',';
			pos++;

			strncpy( buffer+pos, m_account, strlen(m_account) );
			pos += strlen(m_account);

			*(buffer+pos) = ',';
			pos++;

			// 문자열 마지막 0 을 포함할 것인가... 
			MD5Hash( m_password, strlen(m_password), buffer+pos );
			pos += 32;

			*(buffer+pos) = ',';
			pos++;

			memcpy( buffer+pos, m_return_code, sizeof(m_return_code) );
			pos += sizeof(m_return_code);

			char temp[5];
			m_length = pos;
			sprintf( temp, "%04d", pos );
			memcpy( buffer, temp, 4 );
		}
		__except(1)
		{
		}
	}
};

#define ROC_LOGIN_SUCCESS	('1')

struct sRocAuthResponse
{
	int		m_user_unique;
	char	m_success_fail_flag;
	char	m_error_message[257];
	char	m_nick[42];

	sRocAuthResponse()
	{
		init();
	}
	void init()
	{
		ZeroMemory( this, sizeof(sRocAuthResponse) );
	}

	void operator<<(char * res)
	{
		init();

		int count = 0;
		//char temp[10];
		char * p = res+2;
		m_success_fail_flag = res[0];		// 성공/실패 얻기.

		//count = sscanf( p, "%s,%d,%s,%s", m_error_message, &m_user_unique, temp, m_nick );
		//if( count < 4 )
		//{
			//m_success_fail_flag = '3';
		//}
	}
	bool IsLoginSuccess()
	{
		return (ROC_LOGIN_SUCCESS == m_success_fail_flag);
			
	}
};


enum
{
	ND_F_SUCCESS=0,			// 함수 성공.
	ND_F_FAIL = -1,				// 함수 실패.
};

class CRocAuthSession
{
public:
	static BOOL SetRocworksAuthServerInformation( const char * ip, const unsigned short port );

protected:
	static SOCKADDR_IN		m_authServer;

	enum
	{
		rockworks_socket_timeout = 2000,
	};

public:
	CRocAuthSession()
		: m_lastError(0), m_iConnectState(rocworks_connect_state_none)
	{
		m_socket = INVALID_SOCKET;
		//ZeroMemory( &m_authRequestObj, sizeof(m_authRequestObj) );
		ZeroMemory( m_recv_data, sizeof(m_recv_data) );
		ZeroMemory( m_send_data, sizeof(m_send_data) );
	}
	
	bool IsAuthReady()	{ return (m_authServer.sin_port != 0); }

protected:
	void SetTimeout( DWORD ms = rockworks_socket_timeout );

	int Connect();

	int Recv( int wanted_byte );
	void SendData( void * pData, size_t length );		//
	int RecvData();

	void SetError( int err )	{ m_lastError = err; }
	int GetError()				{ return m_lastError; }
	int RocWaitResponse( sRocAuthResponse ** pResponse );
	int RocSetResult( char * response );

protected:
	int m_lastError;

public:
	void ShowServerInformation();

	int RocRequestLogin( char * account, char * password, sRocAuthResponse ** pResponse );

	void Disconnect( bool bEnd = false );

	// get state
	int RocGetState()	{ return m_iConnectState; }


protected:
	enum
	{
		rocworks_connect_state_none=0,
		rocworks_connect_state_wait_connect,		// 재연결 시도 상태.
		rocworks_connect_state_connected,
		rocworks_connect_state_end,					// 끝내야 한다. 재연결하지 말것.
	};

	enum
	{
		rocworks_recvbuf_size=512,
		rocworks_sendbuf_size=256,
	};

	int						m_iConnectState;		// 접속 상태
	SOCKET					m_socket;				// 소켓 정보
	sRocAuthRequest			m_authRequestObj;
	sRocAuthResponse		m_authResponse;

	char					m_send_data[rocworks_sendbuf_size];		// 보낼 데이타 버퍼로 쓴다.
	char					m_recv_data[rocworks_recvbuf_size];		// 받을 데이타의 버퍼.
};

void PrintBuffData( char * buf );

#endif	// _PD_COMPANY_JAPAN_

#endif