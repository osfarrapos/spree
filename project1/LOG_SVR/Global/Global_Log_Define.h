#pragma  once

#include <time.h>
// Log 구조체 선언..

struct _TimeStamp
{
	time_t	m_tTimeStamp ;
	void SetTime() { time( &m_tTimeStamp ) ; } ;
};


struct  _ServerState 
{	
	TCHAR m_server_name[21];
	SHORT m_state;

#ifdef _LOG_WRITE_TIME
	_TimeStamp m_TimeStamp ;
#endif
};



//EVENT CODE
#define _LOG_ITEM_DNPC_DROP_	 (1)
#define _LOG_ITEM_PC_DROP_        15
#define _LOG_ITEM_PC_GRAP_        16
#define _LOG_ITEM_PVP_TRADE_      17
#define _LOG_ITEM_BUY_FROM_NPC_   18  // 상점에서 물건을 산다. 
#define _LOG_ITEM_SELL_TO_NPC_    19  // 상점에서 물건을 판다. 
#define _LOG_ITEM_SELL_CLAN_GIVE_ 20  // 문파기부


struct _ItemLog {
	LARGE_INTEGER	 item_uid ;
	SHORT	code ;
	SHORT	zone ;
	int		from_uid ;
	TCHAR	from[13] ;
	int		to_uid ;
	TCHAR	to[13] ;
	UINT	money ;
	SHORT	type1 ;
	SHORT	type2 ;
	BYTE	moreInfo[30] ;	
#ifdef _LOG_WRITE_TIME
	_TimeStamp m_TimeStamp ;
#endif

};


#define _LOG_CHARAC_LOGIN_		11
#define _LOG_CHARAC_LOGOUT_		12
#define _LOG_CHARAC_ENTER_		13
#define _LOG_CHARAC_LEAVE_		14

enum 
{
	en_charac_name_length=12,
	en_max_lil = 24,
};

struct _CharacLog {
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
	char	account[en_max_lil +1] ;
#else
	char	account[13] ;
#endif
	int		charac_uid ;
	char	charac[13] ;
	short	code ;
	short	zone ;
	short	level ;
	short	point ;
	char	ip[17] ;
	BYTE	moreInfo[15] ;
#ifdef _LOG_WRITE_TIME
	_TimeStamp m_TimeStamp ;
#endif
};


#define	_LOG_TYPE_LOGIN			0
struct _LoginLog {
	int			acc_uid ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
	char		account[en_max_lil +1] ;
	char		charac[en_charac_name_length+1] ;
#else
	char		acc[12] ;
	char		charac[12] ;
#endif
	short		zone_no ;
	int			inner_level	;
	short		party ;
	short		m_class ;
	char		ip[15] ;
	char		pe_pay_code[2] ;
	char		pc_pay_code[2] ;
	char		pc_uid[20] ;
	char		login_time[20] ;
	char		logout_time[20] ;
	int			play_time ;
	int			type ;
#ifdef _LOG_WRITE_TIME
	_TimeStamp m_TimeStamp ;
#endif
};