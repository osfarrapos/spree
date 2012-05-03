#ifdef _XDEF_NEXONLOGIN

#pragma once

// standard windows headers
#include <winsock2.h>
#include <tchar.h>

// standard c / c++ headers
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

// atl headers
#include <atlbase.h>
#include <comdef.h>

// stl header
#pragma warning ( disable:4786 )
#pragma warning ( disable:4018 )

#include <vector>
#include <map>
#include <deque>

#pragma warning ( default:4786 )
#pragma warning ( default:4018 )

#ifndef check
#define	check( x )
#endif

#ifdef _NMDEBUG
#pragma comment( lib, "NMClass/nmcogame_d" )
#else
#ifdef _NMTEST
#pragma comment( lib, "NMClass/nmcogame_t" )
#else
#pragma comment( lib, "NMClass/nmcogame" )
#endif
#endif

#include "NMGeneral.h"

#include "NMDataObject.h"
#include "NMFunctionObject.h"
#include "NMEventObject.h"

#include "NMCOHelpers.h"
#include "NMCOClientObject.h"
#include "NMGSGeneralSession.h"
#include "NMCSChatSession.h"

#include "NMCONew.h"

#endif