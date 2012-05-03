//***************************************************************************
// Filename:		MassiveClientCore.h
// Author:		shawn
// Creation Date:	8/5/2005
//
// Declaration file for the Massive Client Core. (External)
//
// © 2003-2005 Massive Incorporated All Rights Reserved.
//
//***************************************************************************

#ifndef __MASSIVECLIENTCORE_H__
#define __MASSIVECLIENTCORE_H__

#ifdef _XDEF_MADSYSTEM

#include "MassiveMediaTypes.h"

namespace MassiveAdClient3
{
	
	// Forward Declaration
	class CMassiveZoneManager;
	class CRequestObject;
	
	
	//===========================================================================
	// CMassiveClientCore Class
	
	class CMassiveClientCore
	{
	public:
		// Initialization and Instance Retriveal
		static CMassiveClientCore *Instance();
		static CMassiveClientCore *Initialize(MAD_MASSIVE_INIT_STRUCT *initStruct);
		static massiveBool Shutdown(massiveBool bWaitForResponse=true);
		
		// Accessors
		const static MAD_INTERNAL_ERROR GetLastError();
		const static MAD_STATE GetState();
		const static char *GetVersion();
		
		// Logging
		void static Log(MASSIVE_ENUM_LOG_LEVEL uLogLevel, char *szObjectName, char *szFormatedString, ...);
		
		// Client Action Functions
		massiveBool static Tick(massiveBool bForceTick = false);
		massiveBool static SuspendAll();
		massiveBool static ResumeAll();
		massiveBool static FlushImpressions();
		
		massiveBool static EnterZone(const char *szZoneName);
		massiveBool static ExitZone(const char *szZoneName);
		
		// NetworkManager Accessors/Mutators
		void static SetMaxSendKBPS(massiveU16 u16MaxSendKBPS);
		void static SetMaxReceiveKBPS(massiveU16 u16MaxReceiveKBPS);
		
		// MultiPlayer
		char static *MPSessionCreate();
		massiveBool static MPSessionJoin(char *pszGUID);
		massiveBool static MPSessionLeave(void);
		
		// Memory
		static void SetCustomMemoryFunctions(massiveMalloc, massiveFree);
	};
}//namespace MassiveAdClient3

#endif

#endif	// __MASSIVECLIENTCORE_H__
