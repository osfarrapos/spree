#pragma once

#define _WIN32_WINNT	0x500

#define WIN32_LEAN_AND_MEAN
#include <afx.h>
//#include <windows.h>
#include <Mmsystem.h>
#include <winsock2.h>

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <assert.h>
#include <atldbcli.h>
#include <oledb.h>


// 내부 네트워크에서 테스트 목적으로 사용할 때. 2008-06-23
#define _PD_INNER_TEST_

//////////////////////////////////////////////////////////////////////////
//  Define Directive
//////////////////////////////////////////////////////////////////////////
#ifdef _PD_COMPANY_NEXON_
#define _NXPCB_AWARD_200703_	// 넥슨 피씨방 혜택
#define _PD_CRYPTO_GETKEY_MOD_
#endif

#ifdef _DEBUG
#endif

#ifdef _PD_COMPANY_METEL_
#include "./Billing/GameAPI.h"
#define _METEL_LOGIN_200608_
#define _PD_CRYPTO_GETKEY_MOD_
#endif

#ifdef _PD_COMPANY_VINA_
//#define _PD_VINA_PCROOM_INFO_					// PC 방 정보 포함하기. 20070918 : DS/GS 함께 
#define _PD_CRYPTO_GETKEY_MOD_
#endif


#ifdef _PD_COMPANY_ACCLAIM_
#define _PD_ACCLAIM_LOGIN_200612_LOG
#define _PD_CRYPTO_GETKEY_MOD_
#endif

#ifdef _PD_COMPANY_RUSSIA_
#define _PD_CRYPTO_GETKEY_MOD_
#endif

#ifdef _PD_COMPANY_JAPAN_
#define _PD_CRYPTO_GETKEY_MOD_
#endif

enum LOGIN_MODE_BY_COMPANY
{
	en_lmbc_nexon = 0xa04b93f2,
	en_lmbc_metel = 0x3d732b0e, 
	en_lmbc_vina =  0x8c203d51,
	en_lmbc_acclaim = 0xef6c9aa1,
	en_lmbc_pwc = 0xef6c9aa2,
	en_lmbc_rocworks = 0xef6c9aa3,
	en_lmbc_russia = 0xad890bd0,
} ;

#if defined(_PD_COMPANY_ACCLAIM_)
	enum LOGIN_ID_LENGTH		// Login ID Lenght
	{
		en_max_lil	= 24,
	};
	#define en_max_lpl		(32)

#elif defined(_PD_COMPANY_PWC_)
	enum LOGIN_ID_LENGTH		// Login ID Lenght
	{
		en_max_lil	= 24,
	};
	#define en_max_lpl		(32)

#elif defined(_PD_COMPANY_RUSSIA_)
	enum LOGIN_ID_LENGTH		// Login ID Lenght
	{
		en_max_lil	= 24,
	};
	#define en_max_lpl		(32)

#elif defined(_PD_COMPANY_VINA_)
	enum LOGIN_ID_LENGTH		// Login ID Lenght
	{
		en_max_lil	= 24,
	};
	#define en_max_lpl		(24)

#elif defined(_PD_COMPANY_JAPAN_)
	enum LOGIN_ID_LENGTH		// Login ID Lenght
	{
		en_max_lil	= 24,
	};
	#define en_max_lpl		(32)
#else
	enum LOGIN_ID_LENGTH		// Login ID Lenght
	{
		en_max_lil	= 24,
	};
	#define en_max_lpl		(24)
#endif

#define IPV4_BUF_SIZE	16

// 넥슨은 요기에..
#ifdef _RELEASE
#define _TRACE_
#define _BILLING_
#define _INNER_TEST
#define _BILL_DISCONNECTED_NO_CHECK
//#define _BILLING_AUTH_DISABLE
#define  _IP_FILTER_

#endif

#include "myServer/MY_FILE.H"
#include "Global\NDTrace.h"
#include "myServer/MY_QUEUE.H"

#include "Global\SystemMessage_ref.h"
#include "Global\Global_Define.h"
#include "Global\Global_Function.h"

#include "Global\Login_packets.h"
#include "Global\ComUdp_packets.h"

#include "VMManager\VMManager.h"
#include "Environment\Environment_read.h"
#include "myServer/MY_CRYPTO.H"

#include "./MyServer/FileLog.h"

#ifndef _EXCLUDE_FILE_OBJ
extern CLinePrintFile *	g_pErrMsg ;				// 에러메세지 출력기
extern CMYTrace *		g_pTrace ;		// trace
#endif


#define  _CONSOLE_OUPUT_CATCH_LOG_


#define  _DB_LINK_AUTO_RECONNECT
//#define _IF_DB_LINK_DISCONNECT__SERVER_KILL_

//#define  _BILL_STRESS_TEST

#ifdef _DEBUG
#define _TRACE_
#define _BILLING_
#define _INNER_TEST
//#define _BILL_DISCONNECTED_NO_CHECK
//#define _BILLING_AUTH_DISABLE
#define _IP_FILTER_
#endif