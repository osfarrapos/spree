#ifdef _XDEF_NEXONLOGIN

#ifndef __NMPCH_H_5C7BE2B1_CBB4_4f90_AFE8_45733B8612A2__
#define __NMPCH_H_5C7BE2B1_CBB4_4f90_AFE8_45733B8612A2__

#include "XKernel.h"

// standard windows headers
#include <winsock2.h>

// crt headers
#ifndef STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_DEPRECATE
#endif
#include <strsafe.h>

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

//#pragma warning ( default:4786 )
//#pragma warning ( default:4018 )

#ifndef check
#define	check( x )
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

#endif	//	#ifndef __NMPCH_H_5C7BE2B1_CBB4_4f90_AFE8_45733B8612A2__

#endif