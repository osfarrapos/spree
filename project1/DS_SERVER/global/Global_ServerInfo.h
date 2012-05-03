#ifndef _GLOBAL_SERVERINFO
#define _GLOBAL_SERVERINFO


#include "Global_struct.h"


// GS 서버 상태
#define GS_NONSTATE							0		// 꺼진상태
#define GS_OK								1		// 정상상태
#define GS_OVER								3		// 포화
#define GS_ERROR							4		// 비정상 상태


// Forest state
#define  _FOREST_STATE_RUNNING				0					// 정상 작동중
#define  _FOREST_STATE_OFF					-1					// 서비스 중지
#define  _FOREST_STATE_CHECKUP_				-2					// 서버군 점검중
#define  _FOREST_STATE_FULL_				-3					// 포화상태



class _j_ServerInfo
{

public:		

	_SERVER_SYSTEM_INFO		m_ServerSystemInfo[GS_MAX];
	_SERVER_NET_INFO		m_ServerNetInfo[GS_MAX];
	_SERVER_STATE_INFO		m_ServerStateInfo[GS_MAX];

	int						m_iForestUserCount ;
	int						m_iLastRBServer ;			// GS 로비를 round robin 으로 순차적으로 접속하게 하기 위해서..
	char					m_cForestState ;

	u_short					m_usDSAcceptPort;		// Game Server 를 기다리는 Accept Port
	u_short					m_usDSUdpPort;			// DS 의 UDP Recv Port
	u_short					m_usDS_GMSAcceptPort;	// GMS 를 기다리는 Accept Port

	u_short					m_usLoginUdpPort ;		// LS 의 Recv Port 번호

	char 					m_cGSAcceptIP[16];		//
	char					m_cGMSAcceptIP[16];		//

	char 					m_cMessangerIP[16] ;	//
	u_short					m_usMessangerPort ;		//

	SOCKADDR_IN				m_sockaddrMS ;			// 

	int						m_iServiceMode;			// 서버 서비스 모드 : EN_ND_SERVICE_MODE
	int						m_iBillingUse;			// Default : TRUE(use), FALSE(not use)

	int GetForestUserCount() { return m_iForestUserCount ;}
	void GetEachServerConPlayer( MSG_GM_SERVER_PLAYER * pPlayer )
	{
		for( int i = 1; i < GS_MAX; ++i )
		{
			pPlayer->Count[i].usConnectCount = static_cast<u_short>(m_ServerStateInfo[i].iConnectCount);
			pPlayer->Count[i].usPlayerCount = static_cast<u_short>(m_ServerStateInfo[i].iPlayingCount);
		}
	}

public:

	inline void SetForestState( char state ) { m_cForestState = state ; } //enum _FOREST_STATE
	inline char GetForestState() { return m_cForestState ; }	//enum _FOREST_STATE

	_j_ServerInfo() : m_cForestState(_FOREST_STATE_RUNNING), m_iLastRBServer(0), m_iForestUserCount(0)
	{
		memset(m_ServerSystemInfo, 0, sizeof(_SERVER_SYSTEM_INFO)*GS_MAX );
		memset(m_ServerNetInfo, 0, sizeof(_SERVER_NET_INFO)*GS_MAX );
		memset(m_ServerStateInfo, 0, sizeof(_SERVER_STATE_INFO)*GS_MAX );		

		memset(&m_sockaddrMS, 0, sizeof(m_sockaddrMS) ) ;
	}

	bool	Init( const char * const fileName, const int forestNum );

	inline u_short GetDSAcceptPort()	{ return m_usDSAcceptPort ; }
	inline u_short GetDSUdpPort()		{ return m_usDSUdpPort ; }

	inline u_short GetLSUdpPort()		{ return m_usLoginUdpPort ; }
	inline u_short GetGMSAcceptPort()	{ return m_usDS_GMSAcceptPort; }

	inline char * GetGSAcceptIP()		{ return m_cGSAcceptIP; }
	inline char * GetGMSAcceptIP()		{ return m_cGMSAcceptIP; }

	int get_service_mode()				{ return m_iServiceMode; }
	int get_billing_use()				{ return m_iBillingUse; } // true: 빌링사용, false: 빌링사용안함
	
	//inline char * GetMSUdpIP()			{ return m_cMessangerIP ; }
	//inline u_short GetMSUdpPort()		{ return m_usMessangerPort ; }

	SOCKADDR_IN *	GetMSSockAddrIn()	{ return &m_sockaddrMS ; }

	inline void	AddServer(char cNum){	
		if ( GS_MAX < cNum ) {
			::PrintConsole("[MEM_ERR] 'inline void	AddServer(char cNum)'서버 범위를 초과 (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].cState = GS_OK;		
	}
	inline void	UpdateConnectCount(char cNum, int i){
		if (  GS_MAX < cNum ) {
			::PrintConsole("[MEM_ERR] 'inline void	UpdateInCount(char cNum, int i)'서버 범위를 초과 (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].iConnectCount = i;

		// 토탈 카운트를 계산한다.
		m_iForestUserCount = 0 ;
		for( int i = 0 ; i < GS_MAX ; i++)
		{
			m_iForestUserCount = m_iForestUserCount + m_ServerStateInfo[i].iPlayingCount ;
		}

	}
	inline void	UpdatePlayCount(char cNum, int i){
		if (cNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline void	UpdateOutCount(char cNum, int i)'서버 범위를 초과 (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].iPlayingCount = i;
	}
	inline void UpdateState(char cNum, char state, int con, int ply){
		if (cNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline void UpdateState(char cNum, char state, int con, int ply)'서버 범위를 초과 (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].cState = state;
		m_ServerStateInfo[cNum].iConnectCount = con;
		m_ServerStateInfo[cNum].iPlayingCount = ply;
	}
	inline void UpdateInfo(char cNum, void * pServerInfo){        
		if (cNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline void UpdateInfo(char cNum, void * pServerInfo)'서버 범위를 초과 (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].iConnectCount = ((MSG_SYSTEM_INFO_RES *)pServerInfo)->_ServerStateInfo.iConnectCount;
		m_ServerStateInfo[cNum].iPlayingCount = ((MSG_SYSTEM_INFO_RES *)pServerInfo)->_ServerStateInfo.iPlayingCount;

	}

	inline int	SelectConCount(short sNum) { 
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline int	SelectConCount(char cNum)'서버 범위를 초과 (value : %d)\n", sNum);
			return -1;
		}
		return m_ServerStateInfo[sNum].iConnectCount; 
	}
	inline int	SelectPlyCount(short sNum) { 
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline int	SelectPlyCount(char cNum)'서버 범위를 초과 (value : %d)\n", sNum);
			return -1;
		}
		return m_ServerStateInfo[sNum].iPlayingCount; 		
	}	
	inline char SelectState(short sNum) {
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline short SelectState(char cNum)'서버 범위를 초과 (value : %d)\n", sNum);
			return -1;
		}
		return m_ServerStateInfo[sNum].cState; 
	}
	inline _SERVER_SYSTEM_INFO * SelectServerInfo(short sNum) { 
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline _SERVER_SYSTEM_INFO * SelectServerInfo(char cNum)'서버 범위를 초과 (value : %d)\n", sNum);
			return NULL;
		}
		return &m_ServerSystemInfo[sNum]; 
	}
	inline SOCKADDR_IN * GetSockAddr(short sNum) {
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline SOCKADDR_IN * GetSockAddr(char cNum)'서버 범위를 초과 (value : %d)\n", sNum);
			return NULL;
		}
		return &m_ServerNetInfo[sNum].sockAddr ; 
	}

	inline _h_DS_Com * GetDSCom( short sNum )
	{
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline _h_DS_Com * GetDSCom( char cNum )'서버 범위를 초과 (value : %d)\n", sNum);
			return NULL;
		}
		return m_ServerNetInfo[sNum].ds_com ; 
	}

	inline _h_DS_Com * GetFirstCom()
	{
		for( int i = 1 ; i < GS_MAX ; ++i )
		{
			if( m_ServerNetInfo[i].ds_com )
				return m_ServerNetInfo[i].ds_com ;
		}

		return NULL ;
	}

	inline int GetRandServerNum() 
	{
		int iLastServer = ++m_iLastRBServer ;
		for( int i = 0 ; i < GS_MAX ; i++)
		{	
			
			if( GS_MAX <= iLastServer )
			{
				iLastServer = 1 ;
			}

			if( m_ServerStateInfo[iLastServer].cState == GS_OK ) 
			{				
				m_iLastRBServer = iLastServer ;
				return iLastServer ;
			}
			++iLastServer ;
		}
		return 1 ;
	}

	// 모든 GS 에 쏘기
	void send_to_all_gs( void * pData, u_short length );
	// except_server 빼고 보내기
	void send_to_all_gs( void * pData, u_short length, short except_server );	// except_server 는 제외하고 보낸다.
	// gs_index 에만 보내기
	void send_to_gs( void * pData, u_short length, short gs_index );
	//gs_array 들에게만 보내기.
	void send_to_some_gs( void * pData, u_short length, short gs_array[], int count );

	// Show Server settings in ServerEnv.inf file.
	void ShowSettings( bool bTo /* false : command pane. true : output pane */ );




private:
	bool			m_bTimeoutCheck[GS_MAX];

public:
	void ResetTimeoutCheck()
	{
		ZeroMemory( m_bTimeoutCheck, sizeof(m_bTimeoutCheck) );
	}
	void RecvTimeoutResponse( u_char server_index )
	{
		if( server_index < GS_MAX )
			m_bTimeoutCheck[server_index] = true;
	}

	void SendTimeoutCheck()
	{
		ResetTimeoutCheck();

		CMSG_CHECK_DSGS_PACKET pkCheck;
		ZeroMemory( &pkCheck, sizeof(pkCheck) );
		pkCheck.usLength = sizeof(pkCheck);
		pkCheck.cMessage = CMSG_NO_CHECK_DSGS_PACKET;
		pkCheck.ucMode = en_check_packet_timeout_request;

		send_to_all_gs( &pkCheck, pkCheck.usLength );
	}

	void ShowTimeoutCheckResult()
	{
		printf( "\n### Check Timeout Result ###\n" );
		for( int i = 1; i < GS_MAX; ++i )
		{
			if( GS_OK == m_ServerStateInfo[i].cState )
			{
				if( false == m_bTimeoutCheck[i] )
				{
					printf( "  [%2d] : no response\n", i );
				}
			}
		}
	}

#ifdef _PD_COMPANY_JAPAN_
	// RocWorks RMS 설정 정보.
protected:
	char			m_rocAuthIP[16];
	u_short			m_rocAuthPort;

public:
	void set_roc_auth_ip( char * ip )		{ strcpy( m_rocAuthIP, ip ); }
	void set_roc_auth_port( u_short port )	{ m_rocAuthPort = port; }

	inline char * get_roc_auth_ip()			{ return m_rocAuthIP; }
	inline u_short get_roc_auth_port()		{ return m_rocAuthPort; }
#endif


};


#endif