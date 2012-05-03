//***************************************************************************
// Filename:		MassiveAdObjectSubscriber.h
// Author:			shawn
// Creation Date:	8/5/2005
//
// Declaration file for the Massive Ad Object Subscribers.(External)
//
// © 2003-2005 Massive Incorporated All Rights Reserved.
//
//***************************************************************************

#ifndef __MASSIVEADOBJECTSUBSCRIBER_H__
#define __MASSIVEADOBJECTSUBSCRIBER_H__

#ifdef _XDEF_MADSYSTEM

#include "MassiveMediaTypes.h"

namespace MassiveAdClient3
{
	
	class CMassiveAdObject; // Used Internally by Massive AdClient
	
	//===========================================================================
	// Massive AdObject Subscriber Class
	
	class CMassiveAdObjectSubscriber
	{
	public:
		// Construction
		CMassiveAdObjectSubscriber(const char *szName);
		~CMassiveAdObjectSubscriber();
		
		// Accessors
		const char *GetName();
		massiveU32 GetInvElementID();
		massiveU32 GetCrexID();
		const char *GetCrexFileName();
		
		// Impressions
		void SetImpression(MAD_Impression *pImpression);
		massiveBool SetInteraction(MASSIVE_INTERACTION type);
		
		// View State
		MAD_VIEW_STATE GetViewState() { return m_eViewState; }
		
		// Download Media
		virtual massiveBool MediaDownload(const massiveU32 uCrexID);
		virtual massiveBool MediaDownloadComplete(const char *MediaData, const massiveU32 uDataLength, const massiveU32 uMediaType, const massiveU32 uCrexID);
		
		// Tick
		virtual void Tick();
		
		// State Control
		massiveBool Suspend();
		massiveBool Resume();
		
		MAD_INTERNAL_ERROR GetLastError();
		
		// Clear the downloaded Media Buffer
		void ClearMediaBuffer();
		
	private:
		char *m_pName;							// Subscriber Name
		CMassiveAdObject *m_pMassiveAdObject;	// Used internally by Massive AdClient
		MAD_INTERNAL_ERROR	m_LastError;		// Last Error
		massiveBool			m_bMediaDownloaded;
		
		MAD_Impression m_CurrentImpression;
		MAD_Impression m_CopyImpression; 
		MAD_VIEW_STATE m_eViewState;
	};
	
}	//namespace MassiveAdClient3

#endif

#endif	// __MASSIVEADOBJECTSUBSCRIBER_H__
