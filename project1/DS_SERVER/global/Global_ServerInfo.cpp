#include "..\stdafx.h"
#include "Global_ServerInfo.h"
//#include "GMS_Packets.h"

#ifdef	_NEXON_NETWORK_
extern char		NEXONPCB_AUTH_SERVER_IP[16];
extern WORD		NEXONPCB_AUTH_SERVER_PORT;
#endif

bool _j_ServerInfo::Init( const char * const fileName, const int forestNum ){

	
	char buf[1024] = {0};	
	char cGroup[12]={0}, cServer[12]={0}, cNameKorea[52]={0},cWidth[10], cHeight[10],  cNameCh[52]={0}, cIP[16]={0}, cAcc[12]={0}, cRecv[12]={0}, cInner[16] ;
	//int i, server ;
	bool bSuccess = true ;
	bool bExist = false ;
#ifdef _PD_COMPANY_JAPAN_
	bool bRocAuth = false;
#endif
	

	FILE *	stream = NULL;

	TRY{
		stream = fopen( fileName ,"r");
	}
	CATCH_ALL {
		printf("[Exception] Environment file open fails \n");
		fclose(stream);
		return false;
	}

	if(stream == NULL) {
		fclose(stream);
		return false;
	}

	int serverNum = 0;

	m_iServiceMode = en_service_mode_standard;
	m_iBillingUse = TRUE;			// Default TRUE( use billing )

	TRY{
		while(!feof(stream)) {
			memset(buf,0,1024);

			if(fgets(buf, sizeof(buf), stream) == NULL) 
				break;
			
			if( !isalpha( buf[0] ) )
				continue ;

			if( strncmp( buf, "rms", 3 ) == 0 )
			{
				if( sscanf( buf, "%s %s %s", cGroup, cIP, cAcc ) < 3 )
				{
					printf( "  [Error] 'rms' ServerInfo::scan error\nsee '%s'\n", buf ) ;
					bSuccess = false ;
					break ;
				}

#ifdef _PD_COMPANY_JAPAN_
				set_roc_auth_ip( cIP );
				set_roc_auth_port( static_cast<u_short>(atoi(cAcc)) );

				bRocAuth = true;
#endif

				continue;
			}
			else if( strncmp( buf, "group", 5 ) == 0 )
			{
				if( 3 != sscanf( buf, "%s %s %s", cGroup, cServer, cNameKorea ) )
					continue ;
			}
			else
				continue;

			if( forestNum != atoi( cServer ) )
				continue ;

			bExist = true ;

			while( 1 )
			{
				if( bSuccess == false )
					break ;

				memset( buf, 0, sizeof(buf) ) ;
				if( fgets( buf, 1024, stream ) == NULL ) 
					break ;

				if( isalpha( buf[0] ) )
				{
					if( strncmp( buf, "ds", 2 ) == 0 )
					{
						int count = sscanf( buf, "%s %s %s %s %s", cGroup, cIP, cAcc, cRecv, cInner );

						if( count < 4 )
						{
							printf( "  [Error] ServerInfo::scan error, \n  see '%s'\n", buf ) ;
							bSuccess = false ;
							break ;
						}

						m_usDSAcceptPort	= static_cast<u_short>( atoi( cAcc ) ) ;
						m_usDSUdpPort		= static_cast<u_short>( atoi( cRecv) ) ;
						m_usDS_GMSAcceptPort= m_usDSUdpPort - 1;

						if( count == 4 )
						{
							CopyMemory( m_cGSAcceptIP, cIP, 16 );
							m_cGSAcceptIP[15] = 0;
							CopyMemory( m_cGMSAcceptIP, cIP, 16 );
							m_cGMSAcceptIP[15] = 0;
						}
						else
						{
							if( INADDR_NONE != inet_addr( cInner ) )
							{
								CopyMemory( m_cGSAcceptIP, cIP, 16 );
								m_cGSAcceptIP[15] = 0;
								CopyMemory( m_cGMSAcceptIP, cInner, 16 );
								m_cGMSAcceptIP[15] = 0;
							}
							else
							{
								CopyMemory( m_cGSAcceptIP, cIP, 16 );
								m_cGSAcceptIP[15] = 0;
								CopyMemory( m_cGMSAcceptIP, cIP, 16 );
								m_cGMSAcceptIP[15] = 0;
							}
						}
					} 
					else if( strncmp( buf, "ls", 2 ) == 0 )
					{
						if( sscanf( buf, "%s %s", cGroup, cRecv ) < 2 )
						{
							printf( "  [Error] ServerInfo::scan error,\n  see %s\n", buf ) ;
							bSuccess = false ;
							break ;
						}

						m_usLoginUdpPort = static_cast<u_short>( atoi( cRecv ) ) ;
					}
					else if( strncmp( buf, "ms", 2) == 0 )
					{
						if( sscanf( buf, "%s %s %s", cGroup, cIP, cRecv ) < 3 )
						{
							printf( "  [Error] ServerInfo::scan error,\n  see %s\n", buf ) ;
							bSuccess = false ;
							break ;
						}

						m_usMessangerPort = static_cast<u_short>( atoi( cRecv ) ) ;
						memcpy( m_cMessangerIP, cIP, 16 ) ;

						m_sockaddrMS.sin_family = AF_INET ;
						m_sockaddrMS.sin_addr.S_un.S_addr = inet_addr( cIP ) ;
						m_sockaddrMS.sin_port = htons( m_usMessangerPort ) ;
					}
#ifdef	_NEXON_NETWORK_
					else if( strncmp( buf, "nxpcbs", 6 ) == 0 )
					{
						if( sscanf( buf, "%s %s %d", cGroup, NEXONPCB_AUTH_SERVER_IP, &NEXONPCB_AUTH_SERVER_PORT ) < 3 )
						{
							printf( "  [Error] ServerInfo::scan error,\n  see %s\n", buf );
							bSuccess = false;
							break;
						}
					}
#endif
					else if( strncmp( buf, "set", 3 ) == 0 ) {
						if( sscanf( buf, "%s %s", cGroup, cServer ) > 1 ) 
						{
							if( 0 == strncmp( cServer, "innertest", 9 ) )
							{
								m_iServiceMode = en_service_mode_innertest;
								printf( "Set Inner Test Mode\n" );
							}
						}
					}
					else if( strncmp( buf, "end", 3 ) == 0 )
					{
						break ;
					}
					// pk, pvp 무시.
				}
				else if( isdigit( buf[0] ) )
				{
					if( sscanf( buf, "%s %s %s %s %s %s %s %s %s", cServer, cNameKorea, cNameCh, cWidth, cHeight, cIP, cAcc, cInner, cRecv ) < 9 ) {
						printf( "  [Error] ServerInfo::scan error,\n  see %s\n", buf ) ;
						bSuccess = false ;
						break ;
					}

					if( atoi(cGroup) != forestNum )
					{
						continue ;
					}

					serverNum = atoi(cServer);

					memcpy(m_ServerSystemInfo[serverNum].cServerName, cNameKorea, 13);
					memcpy(m_ServerNetInfo[serverNum].cIP, cIP, 16);
					m_ServerNetInfo[serverNum].sPort = atoi(cRecv);
					m_ServerNetInfo[serverNum].sockAddr.sin_family = AF_INET ;
					m_ServerNetInfo[serverNum].sockAddr.sin_addr.S_un.S_addr = inet_addr(cIP) ;
					m_ServerNetInfo[serverNum].sockAddr.sin_port = htons( (unsigned short) atoi( cRecv ) ) ;
					m_ServerNetInfo[serverNum].ds_com = NULL ;


				}
			}

			/*

			if(strlen(buf) < 3){
				i++;
				continue;
			}

			if( buf[0] == ';' || buf[0] == ' ' || buf[0] == '!' || buf[0] == '\t' || buf[0] == '\r' ) {
				i++ ;
				continue ;
			}

			if( sscanf( buf, "%s %s %s %s %s %s", cGroup, cServer, cNameKorea, cNameCh, cIP, cRecv ) < 6 ) {
				return false;
			}
			
			if( atoi(cGroup) != forestNum )
			{
				continue ;
			}

			serverNum = atoi(cServer);

			memcpy(m_ServerSystemInfo[serverNum].cServerName, cNameKorea, 13);
			memcpy(m_ServerNetInfo[serverNum].cIP, cIP, 16);
			m_ServerNetInfo[serverNum].sPort = atoi(cRecv);
			m_ServerNetInfo[serverNum].sockAddr.sin_family = AF_INET ;
			m_ServerNetInfo[serverNum].sockAddr.sin_addr.S_un.S_addr = inet_addr(cIP) ;
			m_ServerNetInfo[serverNum].sockAddr.sin_port = htons( (unsigned short) atoi( cRecv ) ) ;
			m_ServerNetInfo[serverNum].ds_com = NULL ;


			memset(cGroup,0,12);
			memset(cServer,0,12); 
			memset(cNameKorea,0,52);
			memset(cNameCh,0,52);
			memset(cIP,0,16);
			memset(cAcc,0,12);
			memset(cRecv,0,12);

			*/
		}

	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] 'bool _j_ServerInfo::Init()'Exception \n");
		return false ;
	}


	if( false == bExist )
	{
		printf( "[ENV] Server Info Not Found : Group %d", forestNum ) ;
		fclose(stream) ;
		return false;
	}

	if( m_usMessangerPort == 0 || m_usLoginUdpPort == 0 )
	{
		printf( "[ENV] Messanger Or Login Port are needed : Group %d", forestNum ) ;
		fclose( stream ) ;
		Sleep( 10000 ) ;
		return false ;
	}

	if(fclose(stream) != 0) return false;
	return true;
}


void _j_ServerInfo::send_to_all_gs( void * pData, u_short length )
{
	// 모든 GS 에 쏜다.
	for( int i=1; i<GS_MAX; ++i )
	{
		if( m_ServerNetInfo[i].ds_com )
			m_ServerNetInfo[i].ds_com->Send( length, (char*)pData );
	}
}


void _j_ServerInfo::send_to_all_gs( void * pData, u_short length, short except_server )
{
	// except_server 는 제외하고 보낸다.
	for( int i=1; i<GS_MAX; ++i )
	{
		if( i == except_server )
			continue;

		if( m_ServerNetInfo[i].ds_com )
			m_ServerNetInfo[i].ds_com->Send( length, (char*)pData );
	}
}


void _j_ServerInfo::send_to_gs( void * pData, u_short length, short gs_index )
{
	if( gs_index > 0 && gs_index < GS_MAX )
	{
		if( m_ServerNetInfo[gs_index].ds_com )
			m_ServerNetInfo[gs_index].ds_com->Send( length, (char*)pData );
	}
}


void _j_ServerInfo::send_to_some_gs( void * pData, u_short length, short gs_array[], int count )
{
	if( count >= GS_MAX )
		count = GS_MAX -1;

	for( int i=0; i<count; ++i )
	{
		if( gs_array[i] > 0 && gs_array[i] < GS_MAX )
		{
			if( m_ServerNetInfo[gs_array[i]].ds_com )
				m_ServerNetInfo[gs_array[i]].ds_com->Send( length, (char*)pData );
		}
	}
}


void _j_ServerInfo::ShowSettings( bool bTo /* false : command pane. true : output pane */ )
{
	if( bTo )
	{
		PrintConsole( " Server Environment Setting\n" );
	
		switch( get_service_mode() )
		{
		case en_service_mode_innertest:
			PrintConsole( "   - Memory Mode : InnerTest mode\n" );
			break;
		case en_service_mode_outertest:
			PrintConsole( "   - Memory Mode : OuterTest mode\n" );
			break;
		case en_service_mode_standard:
			PrintConsole( "   - Memory Mode : Standard mode\n" );
			break;
		}

		if( TRUE == get_billing_use() )
			PrintConsole( "   - Billing Setting : Use\n" );
		else 
			PrintConsole( "   - Billing Setting : Not use\n" );
	}
	else
	{
		printf( " Server Environment Setting\n" );

		switch( get_service_mode() )
		{
		case en_service_mode_innertest:
			printf( "   - Memory Mode : InnerTest mode\n" );
			break;
		case en_service_mode_outertest:
			printf( "   - Memory Mode : OuterTest mode\n" );
			break;
		case en_service_mode_standard:
			printf( "   - Memory Mode : Standard mode\n" );
			break;
		}

		if( TRUE == get_billing_use() )
			printf( "   - Billing Setting : Use\n" );
		else 
			printf( "   - Billing Setting : Not use\n" );
	}
}