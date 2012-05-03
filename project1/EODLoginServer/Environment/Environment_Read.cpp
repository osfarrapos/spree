
//#define _EXCLUDE_WINSOCK
#include "..\stdafx.h"

#include "Environment_Read.h"
#include "..\timer\TimeOut.h"

#include <stdlib.h>

extern _j_Templet_Timer *	g_pTimeOut ;

#define _PD_NEW_SERVER_INFO_		// 2006/07/11 apply

#ifdef _PD_NEW_SERVER_INFO_

bool _h_ServerInfo::create( char * fn )
{
	FILE * fp = NULL ;
	short group, server ;				//
	unsigned short accept, recv ;		// 접속 포트, 서버간 통신 포트
	unsigned long ip = 0 ;
	unsigned long innerip = 0 ;
	char line[256] ;					// 
	char cGroup[12], cServer[12], cNameKorea[52], cNameCh[52], cIP[20], cAcc[12], cInner[20], cRecv[12] ;
	char cWidth[10], cHeight[10] ;
	
	char cForestType[13] = {0} ;
	char cForestIndex[3] = {0};
	char cForestName[13] = {0};
	
	_sDSAddress * pSockAddr;

	int index = 0 ; // 인덱스(그룹번호)

	int i ;
	int bWh = 0 ;

	bool bSuccess = true ;
#ifdef _PD_COMPANY_JAPAN_
	bool bRocAuth = false;
#endif
	
	if( ( fp = fopen( _FILE_SERVER_ENVIRONMENT_, "r" ) ) == NULL ) {
		printf( "  Env :: %s 파일을 찾을 수 없습니다.\n", _FILE_SERVER_ENVIRONMENT_ ) ;
		return false ;
	}
	
	i = 1 ;
	while( 1 ) {

		
		memset( line, 0, 256 ) ;	
		if( fgets( line, 256, fp ) == NULL ) {
			break ;
		}

		i++ ;

		if( !isalpha( line[0] ) )
			continue ;

		if( strncmp( line, "group", 5 ) == 0 )
		{
			if( 3 != sscanf( line, "%s %s %s", cGroup, cServer, cNameKorea ) )
				continue ;

			group = atoi( cServer ) ;
			if( group < 0 || group >= _COUNT_MAX_GROUP_ )
			{
				printf( "[Error] Init Server Info - Group Index Over %d\n", group ) ;
				bSuccess = false ;
				break ;
			}
		}
#ifdef _PD_COMPANY_JAPAN_
		else if( strncmp( line, "rms", 3 ) == 0 )
		{
			if( sscanf( line, "%s %s %s", cGroup, cIP, cAcc ) < 3 )
			{
				printf( "  [Error] 'rms' ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
				bSuccess = false ;
				break ;
			}

			set_roc_auth_ip( cIP );
			set_roc_auth_port( static_cast<u_short>(atoi(cAcc)) );

			bRocAuth = true;

			continue;
		}
#endif
		else
		{
			continue ;
		}

		m_nForestCount++ ;

		while( 1 )
		{
			if( false == bSuccess )
				break ;

			m_ForestInfo.cForestState[group] = _FOREST_STATE_OFF;
			m_MsgLoginForestList.ForestInfo.cForestState[group] = _FOREST_STATE_OFF ;

			memset( line, sizeof(line), 0 ) ;
			if( fgets( line, 256, fp ) == NULL )
				break ;

			i++ ;

			if( isalpha( line[0] ) )
			{
				if( strncmp( line, "ds", 2 ) == 0 )
				{
					int count = sscanf( line, "%s %s %s %s %s", cGroup, cIP, cAcc, cRecv, cInner );
					if( count < 4 )
					{
						printf( "  [Error] ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
						bSuccess = false ;
						break ;
					}

					ip = inet_addr( cIP ) ;
					accept = (unsigned short) atoi( cAcc ) ;
					recv = (unsigned short) atoi( cRecv ) ;

					if( ip == INADDR_NONE ) {
						printf( "  [Error] ServerInfo::incorrect address ip = %s, accept = %s, recv = %s at line %d, see %s file\n", 
							cIP, cAcc, cRecv, i, _FILE_SERVER_ENVIRONMENT_ ) ;
						bSuccess = false ;
						break ;
					}

					m_DSList[group].ucIndex = (unsigned char)group ;
					strcpy(m_DSList[group].udpIP, cIP) ;
					m_DSList[group].udpPort = (unsigned short)atoi(cRecv) ;						
					m_DSList[group].SockAddr.sin_family = AF_INET ;
					m_DSList[group].SockAddr.sin_addr.S_un.S_addr = ip ;
					m_DSList[group].SockAddr.sin_port = htons( recv ) ;


					pSockAddr = new _sDSAddress;
					ZeroMemory( pSockAddr, sizeof(_sDSAddress) );
					pSockAddr->group = group;
					pSockAddr->addr.sin_family = AF_INET;
					pSockAddr->addr.sin_addr.S_un.S_addr = ip;
					pSockAddr->addr.sin_port = htons( recv );

					m_lstSockAddr.push_back( pSockAddr );

					// 최초 DS 정보 요청...예약..
					_Temp_timerentry entry = {0} ;
					entry.cWork = _TM_DS_INFO_REQ ;
					entry.iValue = group ;
					entry.pVoid = &m_DSList[group].SockAddr ;
					entry.time = GetTime_ByIntSec() + 10 ;
					g_pTimeOut->send_Message( entry ) ;
					


					// bWh = 1 ;
				} // ds
				else if( strncmp( line, "end", 3 ) == 0 )
				{
					break ;
				}
				else if( strncmp( line, "effect", 6 ) == 0 )
				{
					;
				}
				else if( strncmp( line, "pvp", 3 ) == 0 )
				{
					;
				}
				else if( strncmp( line, "pk", 2 ) == 0 )
				{
					;
				}
				else if( strncmp( line, "ls", 2 ) == 0 )
				{
					;
				}
				else if( strncmp( line, "ms", 2 ) == 0 )
				{
					if( sscanf( line, "%s %s %s", cGroup, cIP, cAcc ) < 3 )
					{
						printf( "  [Error] 'ms' ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
						bSuccess = false ;
						break ;
					}

					ip = inet_addr( cIP ) ;
					accept = (unsigned short) atoi( cAcc ) ;
								
					if( ip == INADDR_NONE ) {
						printf( "  [Error] ServerInfo::incorrect address ip = %s, accept = %s, recv = %s at line %d, see %s file\n", 
							cIP, cAcc, cRecv, i, _FILE_SERVER_ENVIRONMENT_ ) ;
						bSuccess = false ;
						break ;
					}
					
					m_Messenger[group].ucIndex = (unsigned char)group ;
					m_Messenger[group].acceptIP = ip ;
					m_Messenger[group].AcceptPort = accept ;
				}
			} // isalpha()
			else if( isdigit(line[0]) )
			{
				if( sscanf( line, "%s %s %s %s %s %s %s %s %s", cServer, cNameKorea, cNameCh, cWidth, cHeight, cIP, cAcc, cInner, cRecv ) < 9 ) {
					printf( "  [Error] ServerInfo::scan error,\n  see %s\n", line ) ;
					bSuccess = false ;
					break ;
				}

				// 정보 채워넣기..
				server = atoi( cServer ) ;
				if( server >= _COUNT_FIELD_ || server <= 0 ) {
					printf( "  [Error] ServerInfo::server number incorrect %d at line %d, see %s file\n", server, i, _FILE_SERVER_ENVIRONMENT_ ) ;
					bSuccess = false ;
					break ;
				}

				ip = inet_addr( cIP ) ;
				accept = (unsigned short) atoi( cAcc ) ;
				recv = (unsigned short) atoi( cRecv ) ;

				if( ip == INADDR_NONE ) {
					printf( "  [Error] ServerInfo::incorrect address ip = %s, accept = %s, recv = %s at line %d, see %s file\n", 
						cIP, cAcc, cRecv, i, _FILE_SERVER_ENVIRONMENT_ ) ;
					bSuccess = false ;
					break ;
				}

				m_GSList[group].GSInfo[server].acceptIP = ip ;
				m_GSList[group].GSInfo[server].acceptPort = accept ;
				m_GSList[group].GSInfo[server].SockAddr.sin_family = AF_INET ;
				m_GSList[group].GSInfo[server].SockAddr.sin_addr.S_un.S_addr = ip ;
				m_GSList[group].GSInfo[server].SockAddr.sin_port = htons( recv ) ;
				m_GSList[group].GSInfo[server].ucIndex = (unsigned char)group ;
				m_GSList[group].GSInfo[server].udpIP = (unsigned short)inet_addr( cInner ) ;
				m_GSList[group].GSInfo[server].udpPort = recv ;		
			}
		}	
	}
	
	fclose( fp ) ;

//#ifdef _TEST_
	int k ;
	for( i = 0 ; i < _COUNT_MAX_GROUP_ ; i++) 
	{		
		for( k = 0 ; k < _COUNT_FIELD_ ; k++ ) 
		{
			if( m_GSList[i].GSInfo[k].acceptIP ) 
			{
				printf( "*INFO* Group %2d : Field = %2d, ip = %s, acc = %5u, recv = %5u \n",
					i, k, inet_ntoa( m_GSList[i].GSInfo[k].SockAddr.sin_addr ), 
					m_GSList[i].GSInfo[k].acceptPort,  m_GSList[i].GSInfo[k].udpPort ) ;
			}
		}
	}
//#endif

#ifdef _PD_COMPANY_JAPAN_
	if( bSuccess )
	{
		if( false == bRocAuth )
		{
			printf( "please insert RMS Information(rms ip port) into ServerEnv.inf\n" );
			bSuccess = false;
		}
	}
#endif

	return bSuccess ;
}

#else	// _PD_NEW_SERVER_INFO_ else

bool _h_ServerInfo::create( char * fn )
{
	FILE * fp = NULL ;
	short group, server ;				//
	unsigned short accept, recv ;		// 접속 포트, 서버간 통신 포트
	unsigned long ip = 0 ;
	unsigned long innerip = 0 ;
	char line[256] ;					// 
	char cGroup[12], cServer[12], cNameKorea[52], cNameCh[52], cIP[20], cAcc[12], cInner[20], cRecv[12] ;
	
	char cForestType[13] = {0} ;
	char cForestIndex[3] = {0};
	char cForestName[13] = {0};

	int index = 0 ; // 인덱스(그룹번호)

	int i ;
	bool bSuccess = true ;
	int bWh = 0 ;
	
	if( ( fp = fopen( _FILE_SERVER_ENVIRONMENT_, "r" ) ) == NULL ) {
		printf( "  Env :: %s 파일을 찾을 수 없습니다.\n", _FILE_SERVER_ENVIRONMENT_ ) ;
		return false ;
	}
	
	i = 1 ;
	while( 1 ) {

		
		memset( line, 0, 256 ) ;	
		if( fgets( line, 256, fp ) == NULL ) {
			break ;
		}
		
		if( strlen( line ) < 3 ) {
			i++ ;
			continue ;
		}
		
		if( line[0] == ';' || line[0] == ' ' || line[0] == '!' || line[0] == '\t' || line[0] == '\r' ) {
			i++ ;
			continue ;
		}

		if ( line[0] == '#' ){
			memset(cForestType, 0 ,13);
			memset(cForestIndex, 0, 3) ;
			memset(cForestName, 0 , 13) ;
			if( sscanf( &line[1], "%s %s %s", cForestType, cForestIndex, cForestName ) < 1 ){
				printf( "  [Error] ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
				bSuccess = false ;
				break ;
			}
			
			if( strcmp(cForestType, "BEGIN_FOREST") == 0 ){
				
				group = atoi(cForestIndex);
							
				m_ForestInfo.cForestState[group] = _FOREST_STATE_OFF;
				m_MsgLoginForestList.ForestInfo.cForestState[group] = _FOREST_STATE_OFF ;

				
				m_nForestCount++;

				bWh = 0;
				continue;
			}
			

		}
		
		// Get DS info
		if( bWh == 0 )
		{
			// 
			if( sscanf( line, "%s %s %s %s", cGroup, cIP, cAcc, cRecv ) < 4 )
			{
				printf( "  [Error] ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
				bSuccess = false ;
				break ;
			}

			group = atoi(cGroup) ;
			ip = inet_addr( cIP ) ;
			accept = (unsigned short) atoi( cAcc ) ;
			recv = (unsigned short) atoi( cRecv ) ;

			if( ip == INADDR_NONE ) {
				printf( "  [Error] ServerInfo::incorrect address ip = %s, accept = %s, recv = %s at line %d, see %s file\n", 
					cIP, cAcc, cRecv, i, _FILE_SERVER_ENVIRONMENT_ ) ;
				bSuccess = false ;
				break ;
			}

			m_DSList[group].ucIndex = (unsigned char)group ;
			strcpy(m_DSList[group].udpIP, cIP) ;
			m_DSList[group].udpPort = (unsigned short)atoi(cRecv) ;						
			m_DSList[group].SockAddr.sin_family = AF_INET ;
			m_DSList[group].SockAddr.sin_addr.S_un.S_addr = ip ;
			m_DSList[group].SockAddr.sin_port = htons( recv ) ;


			// 최초 DS 정보 요청...예약..
			_Temp_timerentry entry = {0} ;
			entry.cWork = _TM_DS_INFO_REQ ;
			entry.iValue = group ;
			entry.pVoid = &m_DSList[group].SockAddr ;
			entry.time = GetTime_ByIntSec() + 10 ;
			g_pTimeOut->send_Message( entry ) ;
			


			bWh = 1 ;
			continue ;
		}

		// Get Cs Info
		else if( bWh == 1 )
		{
			if( sscanf( line, "%s %s %s", cGroup, cIP, cAcc ) < 3 )
			{
				printf( "  [Error] ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
				bSuccess = false ;
				break ;
			}
			
			/*strcpy( m_CS_IP, cIP ) ;
			m_usCS_Port = (unsigned short)atoi(cAcc) ;*/
			bWh = 2 ;
			continue ;
		}


		// Get Messenger Server Info
		else if( bWh == 2 )
		{
			if( sscanf( line, "%s %s %s", cGroup, cIP, cAcc ) < 3 )
			{
				printf( "  [Error] ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
				bSuccess = false ;
				break ;
			}

			group = atoi(cGroup) ;
			ip = inet_addr( cIP ) ;
			accept = (unsigned short) atoi( cAcc ) ;
						
			if( ip == INADDR_NONE ) {
				printf( "  [Error] ServerInfo::incorrect address ip = %s, accept = %s, recv = %s at line %d, see %s file\n", 
					cIP, cAcc, cRecv, i, _FILE_SERVER_ENVIRONMENT_ ) ;
				bSuccess = false ;
				break ;
			}
			
			m_Messenger[group].ucIndex = (unsigned char)group ;
			m_Messenger[group].acceptIP = ip ;
			m_Messenger[group].AcceptPort = accept ;
			
			
			bWh = 3 ;
			continue ;
		}
		

		if( sscanf( line, "%s %s %s %s %s %s %s %s", cGroup, cServer, cNameKorea, cNameCh, cIP, cAcc, cInner, cRecv ) < 8 ) {
			printf( "  [Error] ServerInfo::scan error at line %d, see %s file\n", i, _FILE_SERVER_ENVIRONMENT_ ) ;
			bSuccess = false ;
			break ;
		}

		// 그룹 번호 읽고
		group = atoi( cGroup ) ;
		if( group >= _COUNT_MAX_GROUP_ || group < 0 ) {
			printf( "  [Error] ServerInfo::group number incorrect %d at line %d, see %s file\n", group, i, _FILE_SERVER_ENVIRONMENT_ ) ;
			bSuccess = false ;
			break ;
		}		

		// 정보 채워넣기..
		server = atoi( cServer ) ;
		if( server >= _COUNT_FIELD_ || server <= 0 ) {
			printf( "  [Error] ServerInfo::server number incorrect %d at line %d, see %s file\n", server, i, _FILE_SERVER_ENVIRONMENT_ ) ;
			bSuccess = false ;
			break ;
		}

		ip = inet_addr( cIP ) ;
		accept = (unsigned short) atoi( cAcc ) ;
		recv = (unsigned short) atoi( cRecv ) ;

		if( ip == INADDR_NONE ) {
			printf( "  [Error] ServerInfo::incorrect address ip = %s, accept = %s, recv = %s at line %d, see %s file\n", 
				cIP, cAcc, cRecv, i, _FILE_SERVER_ENVIRONMENT_ ) ;
			bSuccess = false ;
			break ;
		}

		m_GSList[group].GSInfo[server].acceptIP = ip ;
		m_GSList[group].GSInfo[server].acceptPort = accept ;
		m_GSList[group].GSInfo[server].SockAddr.sin_family = AF_INET ;
		m_GSList[group].GSInfo[server].SockAddr.sin_addr.S_un.S_addr = ip ;
		m_GSList[group].GSInfo[server].SockAddr.sin_port = htons( recv ) ;
		m_GSList[group].GSInfo[server].ucIndex = (unsigned char)group ;
		m_GSList[group].GSInfo[server].udpIP = (unsigned short)inet_addr( cInner ) ;
		m_GSList[group].GSInfo[server].udpPort = recv ;		
		
		
		i++ ;
	}

	fclose( fp ) ;

//#ifdef _TEST_
	int k ;
	for( i = 0 ; i < _COUNT_MAX_GROUP_ ; i++) {		

		for( k = 0 ; k < _COUNT_FIELD_ ; k++ ) {
			if( m_GSList[i].GSInfo[k].acceptIP ) {
				printf( "*INFO* Group %2d : Field = %2d, ip = %s, acc = %5u, recv = %5u \n",
					i, k, inet_ntoa( m_GSList[i].GSInfo[k].SockAddr.sin_addr ), 
					m_GSList[i].GSInfo[k].acceptPort,  m_GSList[i].GSInfo[k].udpPort ) ;
			}
		}
	}
//#endif

	return bSuccess ;
}

#endif		// _PD_NEW_SERVER_INFO_ end


bool _h_ServerInfo::checkInfo()
{
	bool bCorrect = true ;

	printf( "  ## Check Server Information ##\n" ) ;

	for( int g = 0 ; g < _COUNT_MAX_GROUP_ ; ++g ) 
	{
		if( m_DSList[g].udpPort )
		{
			if( m_Messenger[g].AcceptPort == 0 )
			{
				printf( "Error : Group %3d - Don't have Messenger\n", g ) ;
				bCorrect = false ;
			}
		}
	}

	return bCorrect ;
}

void _h_ServerInfo::print()
{
	int k ;
	for( int i = 0 ; i < _COUNT_MAX_GROUP_ ; i++) 
	{		
		for( k = 0 ; k < _COUNT_FIELD_ ; k++ ) {
			if( m_GSList[i].GSInfo[k].acceptIP ) {
				printf( "*INFO* Group %2d : Field = %2d, ip = %s, acc = %5u, recv = %5u \n",
					i, k, inet_ntoa( m_GSList[i].GSInfo[k].SockAddr.sin_addr ), 
					m_GSList[i].GSInfo[k].acceptPort,  m_GSList[i].GSInfo[k].udpPort ) ;
			}
		}
	}
}