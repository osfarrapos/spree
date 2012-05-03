
#if !defined(AFX_STDAFX_H__CF1D0265_C2DA_44EC_8322_292535CD00E2__INCLUDED_)
#define AFX_STDAFX_H__CF1D0265_C2DA_44EC_8322_292535CD00E2__INCLUDED_


#pragma once

#pragma warning(disable:4786)

#define _WIN32_WINNT	0x500
#define WIN32_LEAN_AND_MEAN

#include <afx.h>
#include <stdio.h>
#include <Winsock2.h>
#include <Windows.h>
#include <assert.h>
#include <process.h>
#include <Mmsystem.h>
#include <stdlib.h>
#include <atldbcli.h>


#define  _NEO_LOG
#define _USE_HEADER_

//#define  _LOG_WRITE_TIME
#define _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_	// 20070601 : 로그DB 수정 시작.

#include "Global\Global_Log_Packet.h"
#include "Network\Network.h"
#include "Global\Global_Define.h"
#include "Global\Global_Function.h"
#include "Global\Global_LogFile.h"

#endif