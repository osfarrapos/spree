#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0500
#endif

#ifdef INCLUDE_NETWORK_BASE_HEADER
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#endif

// Base Headers
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>
#include <process.h>
#include "FileLog.h"
#include "CriticalSection.h"
#include "gDefine.h"

#ifdef INCLUDE_ODBC_HEADER
#include <sql.h>
#include <sqlext.h>
#include "ODBC.h"
#endif
