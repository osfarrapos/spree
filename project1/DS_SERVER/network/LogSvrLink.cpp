#include "..\stdafx.h"
#include "LogSvrLink.h"



bool CLogSvr_Com::ReadInfoFile( const char * fileName, const short sGroupNo, char * const cIP, u_short * const port )
{

	FILE * fp = NULL ;
	int nRead = 0 ;
	char buf[128] = {0} ;
	short sGNo = 0 ;			// 그룹번호..

	char cBind[3][32] ;

	bool bRet = false ;

	try{

		if( ( fp = fopen( fileName, "rt" ) ) == NULL )
		{
			return false ;
		}

		while (1) {

			if( fgets( buf, sizeof(buf), fp ) == NULL )
			{
				bRet = false ;
				break ;
			}			

			if( buf[0] == ';')
			{
				continue ;
			}

			nRead = sscanf( buf, "%s %s %s", cBind[0], cBind[1], cBind[2] ) ;

			if( nRead != 3 )		// 완벽하지 않은건 용서할수 없다.. ㅡㅡ;
			{
				continue ;
			}

			if( atoi(cBind[0]) == sGroupNo )			// 그룹 번호가 일치하는 정보를 찾았을때만 true를 리턴한다.
			{
				strcpy( cIP, cBind[1] ) ;
				*port = atoi( cBind[2] ) ;

				bRet = true ;
				break ;
			}
		}

		fclose( fp ) ;

	}
	catch(...){
		fclose(fp) ;
		return false ;
	}	

	return bRet ;

}


bool CLogSvr_Com::createCSCom(  const char * cIP, const unsigned short port )
{
	m_bIsCreated = FALSE ;

	if( port == 0 || cIP == NULL )
	{		
		return true ;
	}
	
	int i = 0 ;

	m_cssock = 0 ;

	memset(m_ip,0,sizeof(m_ip)) ;
	strcpy( m_ip, cIP ) ;
	m_port = port ;
		
	// DS 에 연결하기
	m_cssock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ;
	if( m_cssock == INVALID_SOCKET ) {
		printf( "INIT - CS_Com : Create ds socket failed\n" ) ;
		return false ;
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons( port ) ;
	addr.sin_addr.s_addr = inet_addr( cIP ) ;		// 

	if( connect( m_cssock, (sockaddr*)&addr, sizeof(addr) ) == SOCKET_ERROR ) {
		printf( "INIT - CS_Com : connect failed ( code = %d )\n", WSAGetLastError() ) ;
		return false ;
	}

	// 스레드 생성 : TCP/UDP
	DWORD dwID ;
	m_hThread = BEGINTHREADEX( NULL, 0, th_LogSendthread, (VOID*)this, 0, &dwID ) ;
	if( m_hThread == NULL ) 
	{
		printf( "INIT - th_LogSendthread Cannot Start!!\n" ) ;
		return false ;
	}	
	
	m_bIsCreated = TRUE ;
	return true ;
}

void CLogSvr_Com::releaseCSCom()
{
	if( m_bIsCreated )
	{
		WaitForSingleObject( m_hThread, 5000 ) ;

		CloseHandle( m_hThread ) ;

		Sleep(1000) ;

		if( m_cssock )
			closesocket( m_cssock ) ;

		::PrintConsole("[CS] _h_CS_Com::releaseCSCom() \n" ) ;

	}

	// 만약 사용하지 않고 있다면, 그냥 리턴.

}


/* =====================================================================================
send_requset : CS에 패킷 보내기
===================================================================================== */
int CLogSvr_Com::send_request( u_short size, char * pdata )
{
	return send( m_cssock, pdata, size, 0 );
}


bool CLogSvr_Com::RestoreConnection()	// ozzywow
{		
	if( m_cssock )
		closesocket( m_cssock );
	m_cssock = 0 ;

	if( m_port == 0 )
	{
		m_bIsCreated = FALSE ;
		printf( "Don't use to the LogSvr\n" );
		return true ;
	}

	// cS 에 연결하기
	m_cssock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ;
	if( m_cssock == INVALID_SOCKET ) {
		printf( "INIT - LogSvr : Create ds socket failed\n" ) ;
		return false ;
		
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons( m_port ) ;
	addr.sin_addr.s_addr = inet_addr( m_ip ) ;		// 

	if( connect( m_cssock, (sockaddr*)&addr, sizeof(addr) ) == SOCKET_ERROR ) {
		closesocket(m_cssock) ;		//ozzywow add
		m_cssock = INVALID_SOCKET ;
		PrintConsole( "INIT - LogSvr : connect failed ( code = %d )\n", WSAGetLastError() ) ;
		return false ;
	}

	m_bIsCreated = TRUE ;
	return true ;
}

_CharacLog * CLogSvr_Com::GetCharacLogSlot() 
{
	if ( m_bIsCreated ){
		return m_CharacLogQueue.Ready_Pop() ;
	}
	else
	{
		return NULL ;
	}
}

void CLogSvr_Com::InsertCharacLog( _CharacLog * pLog ) 
{
	if( m_bIsCreated )
	{
		m_CharacLogQueue.Use_Put( pLog ) ;		
	}	
}


_ServerState * CLogSvr_Com::GetServerLogSlot() 
{
	if ( m_bIsCreated ){
		return m_ServerLogQueue.Ready_Pop() ;
	}
	else
	{
		return NULL ;
	}

}

void CLogSvr_Com::InsertServerLog( _ServerState * pLog ) 
{
	if( m_bIsCreated )
	{
		m_ServerLogQueue.Use_Put( pLog ) ;		
	}	
}

_ItemLog * CLogSvr_Com::GetItemLogSlot()
{

	if ( m_bIsCreated ){
		return m_ItemLogQueue.Ready_Pop() ;
	}
	else
	{
		return NULL ;
	}

	return NULL ;
}


void CLogSvr_Com::InsertItemLog( _ItemLog * pLog ) 
{

	if( m_bIsCreated )
	{
		m_ItemLogQueue.Use_Put( pLog ) ;		
	}

}



_LoginLog * CLogSvr_Com::GetLoginLogSlot()
{

	if ( m_bIsCreated ){
		return m_LoginLogQueue.Ready_Pop() ;
	}
	else
	{
		return NULL ;
	}

	return NULL ;
}


void CLogSvr_Com::InsertLoginLog( _LoginLog * pLog ) 
{

	if( m_bIsCreated )
	{
		m_LoginLogQueue.Use_Put( pLog ) ;		
	}

}


void CLogSvr_Com::LogWrite_ChangeName( char * from, char * to, short group )
{
	try
	{
		_ItemLog * pLog = GetItemLogSlot() ;
		if( pLog )
		{
			::ZeroMemory( pLog, sizeof(_ItemLog) ) ;

			pLog->item_uid = 0 ;
			pLog->code = _LOG_ITEM_NAME_CHANGE_ ;
			pLog->zone = 0 ;
			pLog->from_uid = 0 ;
			strncpy( pLog->from, from, 13 );
			pLog->to_uid = 0 ;
			strncpy( pLog->to, to, 13 ) ;
			pLog->money = 0 ;
			pLog->type1 = 0 ;
			pLog->type2 = 0 ;

			InsertItemLog( pLog ) ;
		}
	}
	catch( ... )
	{
	}
}


void CLogSvr_Com::LogWrite_ItemTaxAdd( short map_id, u_int total, u_int part, bool bTemp )
{
	// 
	try
	{
		_ItemLog * pLog = GetItemLogSlot() ;
		if( pLog )
		{
			::ZeroMemory( pLog, sizeof(_ItemLog) ) ;

			pLog->item_uid = 0 ;
			pLog->code = _LOG_ITEM_TAX_ADD_ ;
			pLog->zone = map_id ;
			pLog->from_uid = 0 ;
			ZeroMemory( pLog->from, 13 );
			pLog->to_uid = total ;
			ZeroMemory( pLog->to, 13 ) ;
			pLog->money = part ;
			pLog->type1 = 0 ;
			pLog->type2 = 0 ;

			InsertItemLog( pLog ) ;
		}
	}
	catch( ... )
	{
	}
}

void CLogSvr_Com::LogWrite_ItemTaxTake( char * from, short map_id, u_int take )
{
	// 
	try
	{
		_ItemLog * pLog = GetItemLogSlot() ;
		if( pLog )
		{
			::ZeroMemory( pLog, sizeof(_ItemLog) ) ;

			pLog->item_uid = 0 ;
			pLog->code = _LOG_ITEM_TAX_TAKE_ ;
			pLog->zone = map_id ;
			pLog->from_uid = 0 ;
			strncpy( pLog->from, from, 13 );
			pLog->from[12] = 0;
			pLog->to_uid = 0 ;
			ZeroMemory( pLog->to, 13 ) ;
			pLog->money = take ;
			pLog->type1 = 0 ;
			pLog->type2 = 0 ;

			InsertItemLog( pLog ) ;
		}
	}
	catch( ... )
	{
	}
}
