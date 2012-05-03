#ifdef _XDEF_NEXONLOGIN

#ifndef __NMFDSFILEDOWNLOADSESSION_H_0E1504C8_828D_4aff_9816_CE5ECC00CE93__
#define __NMFDSFILEDOWNLOADSESSION_H_0E1504C8_828D_4aff_9816_CE5ECC00CE93__

#include "NMGSGeneralSession.h"

class CNMFDSFileDownloadSession : public CNMGSGeneralSession
{
public:
	CNMFDSFileDownloadSession();
	virtual ~CNMFDSFileDownloadSession();

	// For File Share
	BOOL	GetFile( _UInt32_ uEventId );
	BOOL	CancelFile( _UInt32_ uEventId );
	BOOL	RemoveFile( _UInt32_ uEventId );
	BOOL	GetFileEvents( NMFileEventList& aEventList );

	BOOL	GetDownloadDir( LPTSTR szDir );		// MAX_PATH
	BOOL	SetDownloadDir( LPCTSTR szDir );	// MAX_PATH
};

#endif	//	#ifndef __NMFDSFILEDOWNLOADSESSION_H_0E1504C8_828D_4aff_9816_CE5ECC00CE93__

#endif