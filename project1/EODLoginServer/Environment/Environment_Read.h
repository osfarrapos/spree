#pragma once

// 필드의 개수는 0번 인덱스를 포함하여 25로 하였고, 인덱스0은 서버이동 요청 실패시에 쓰인다. 
#define _COUNT_FIELD_			25


//=================================================================================
// 한 필드의 정보
struct _sDSInfo {
	unsigned char		ucIndex ;
	char				udpIP[16];	
	unsigned short		udpPort ;
	SOCKADDR_IN			SockAddr ;				// 서버의 주소
};

// GS 하나의 정보
struct _sGSInfo
{
	unsigned char		ucIndex ;
	unsigned long		acceptIP ;
	unsigned long		udpIP ;
	unsigned short		acceptPort ;
	unsigned short		udpPort ;
	short				sCount ;

	SOCKADDR_IN		SockAddr ;				// 서버의 주소
	_sGSInfo() : ucIndex(0), udpIP(0), acceptIP(0), acceptPort(0), udpPort(0) 
	{
		memset( &SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	}
};

struct _sMessenger
{
	unsigned char		ucIndex ;
	unsigned long		acceptIP ;
	unsigned short		AcceptPort ;
};


// 군에 포함된 GS List
struct _sGSList
{	
	_sGSInfo	  GSInfo[_COUNT_FIELD_] ;
};


//=================================================================================
// 그룹 인덱스는 0부터 순서대로 가야 한다. 
// 객체 생성후, create 호출, set_myInfo 차례로 호출해야 한다.
struct _sDSAddress
{
	int				group;
	SOCKADDR_IN		addr;
};

class _h_ServerInfo 
{
#ifdef _PD_COMPANY_JAPAN_
protected:
	
	char			m_rocAuthIP[16];
	u_short			m_rocAuthPort;

public:
	void set_roc_auth_ip( char * ip )		{ strcpy( m_rocAuthIP, ip ); }
	void set_roc_auth_port( u_short port )	{ m_rocAuthPort = port; }

	inline char * get_roc_auth_ip()			{ return m_rocAuthIP; }
	inline u_short get_roc_auth_port()		{ return m_rocAuthPort; }
#endif

public:

	int				m_nForestCount;				// 현재 그룹의 수 : 그룹 인덱스는 0부터 시작이다. 
	
	// Group info
	_ForestInfo		m_ForestInfo;
	_sGSList		m_GSList[_COUNT_MAX_GROUP_];
	_sDSInfo		m_DSList[_COUNT_MAX_GROUP_];
	_sMessenger		m_Messenger[_COUNT_MAX_GROUP_];
	char			m_ForestCheckFlag[_COUNT_MAX_GROUP_];

	MSG_LOGIN_FOREST_LIST	m_MsgLoginForestList;
	list<_sDSAddress*>		m_lstSockAddr;

	_h_ServerInfo() : m_nForestCount(0) 
	{

#ifdef _PD_COMPANY_JAPAN_
		ZeroMemory( m_rocAuthIP, sizeof(m_rocAuthIP) );
		m_rocAuthPort = 0;
#endif
		memset( &m_ForestInfo, 0, sizeof(m_ForestInfo) ) ;
		memset( m_GSList, 0, sizeof(_sGSList) * _COUNT_MAX_GROUP_ ) ;
		memset( m_DSList, 0, sizeof(_sDSInfo) * _COUNT_MAX_GROUP_ ) ;
		memset( m_Messenger, 0, sizeof(_sMessenger) * _COUNT_MAX_GROUP_) ;

		memset( &m_MsgLoginForestList, 0, sizeof(MSG_LOGIN_FOREST_LIST) ) ;
		m_MsgLoginForestList.ForestInfo.bServerRun = true ;

		for( int i = 0 ; i < _COUNT_MAX_GROUP_ ; i++ )
		{
			m_ForestInfo.cForestState[i] = _FOREST_STATE_OFF ;
			m_MsgLoginForestList.ForestInfo.cForestState[i] = _FOREST_STATE_OFF ;
		}
	}
	
	//void init();
	bool create(char * fn);
	bool SetGSList(_sGSList * pGSList, u_char nForestIndex);
	bool SetDSInfo(_sDSInfo * pDSInfo, u_char nForestIndex);
	bool SetForestInfo(_ForestInfo * pForest, unsigned char nForestIndex) ;

	inline void SetVersion( int version, DWORD dwClientVer ) 
	{ 
		m_ForestInfo.nVersion = version ; 
		m_ForestInfo.dwClientVer = dwClientVer ;
		m_MsgLoginForestList.ForestInfo.nVersion = version ;		
	}
	inline void GetVersion( int & iPatchNum, DWORD & dwClientVer ) 
	{ 
		iPatchNum = m_ForestInfo.nVersion ;
		dwClientVer = m_ForestInfo.dwClientVer ;
	}

	inline	void SetForestState( char state, u_char nForestIndex ) 
	{ 
		
		if( nForestIndex >= 0 
			&& nForestIndex < _COUNT_MAX_GROUP_  )
		{

			m_MsgLoginForestList.ForestInfo.cForestState[nForestIndex] = state ;
			if( state > 0 )
			{
				state = _FOREST_STATE_RUNNING ;
			}
			m_ForestInfo.cForestState[nForestIndex] = state ; 
		}
		
	} 
	inline int GetForestState (u_char nForestIndex) { return m_ForestInfo.cForestState[nForestIndex] ; }	
	inline MSG_LOGIN_FOREST_LIST * GetForestListPacket() { return &m_MsgLoginForestList ; }

	inline	unsigned long	GetGSAcceptIP(u_char nForestIndex, u_char nGSIndex) { 
		return m_GSList[nForestIndex].GSInfo[nGSIndex].acceptIP ;
	};
	inline	unsigned short	GetGSAcdeptPort(u_char nForestIndex, u_char nGSIndex) { 
		return m_GSList[nForestIndex].GSInfo[nGSIndex].acceptPort ;
	};	
	
	inline	SOCKADDR_IN *	GetDsSockAddr_In(u_char nForestIndex) { 
		if( nForestIndex < _COUNT_MAX_GROUP_ ) {
			return &m_DSList[nForestIndex].SockAddr ;
		}
		return NULL ;
		
	} ;	
	unsigned long		GetDSUdpIP(u_char nForestIndex);
	unsigned short		GetDSUdpPort(u_char nForestIndex);  

	inline unsigned long	GetMessengerIP(u_char nForestIndex) {
		return m_Messenger[nForestIndex].acceptIP ;
	}
	inline unsigned short	GetMessengerPort(u_char nForestIndex) {
		return m_Messenger[nForestIndex].AcceptPort ;
	}

	bool checkInfo() ;

	void print() ;
} ;


