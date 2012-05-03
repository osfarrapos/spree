
#if !defined(AFX_STDAFX_H__CF1D0265_C2DA_44EC_8322_292535CD00E2__INCLUDED_)
#define AFX_STDAFX_H__CF1D0265_C2DA_44EC_8322_292535CD00E2__INCLUDED_


#pragma once

#pragma warning(disable:4786)

#define _WIN32_WINNT	0x500
#ifndef _EXCLUDE_WINDOWS
#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <afxcoll.h>
#include <Mmsystem.h>
#endif

#ifndef _EXCLUDE_WINSOCK
#include <winsock2.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <atldbcli.h>
#include <process.h>

#include <Mmsystem.h>
#include <stdlib.h>


#ifdef _RELEASE_
#define _SQL_STRING_CONVERT
#endif


#ifdef _DEBUG
#define _SQL_STRING_CONVERT
#endif


#include "Network\Network.h"
#include "Global\MSPacket_Msg.h"
#include "Global\Global_Define.h"
#include "Global\Global_Function.h"
#include "Global\Global_LogFile.h"

//#define _TRACE_
#include "Global\Trace.h"



#endif