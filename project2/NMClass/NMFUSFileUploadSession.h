#ifdef _XDEF_NEXONLOGIN

#ifndef __NMFUSFILEUPLOADSESSION_H_58261932_8D77_46e4_8688_AF82996BA3FD__
#define __NMFUSFILEUPLOADSESSION_H_58261932_8D77_46e4_8688_AF82996BA3FD__

#include "NMGSGeneralSession.h"

class CNMFUSFileUploadSession : public CNMGSGeneralSession
{
public:
	CNMFUSFileUploadSession();
	virtual ~CNMFUSFileUploadSession();

	// For File Share
	_UInt32_	SendFile( LPCTSTR szFileName );		// MAX_PATH
	BOOL		SendFileAgain( _UInt32_ uEventId );
	BOOL		CancelFile( _UInt32_ uEventId );
	BOOL		RemoveFile( _UInt32_ uEventId );
	BOOL		GetFileEvents( NMFileEventList& aEventList );
};

#endif	//	#ifndef __NMFUSFILEUPLOADSESSION_H_58261932_8D77_46e4_8688_AF82996BA3FD__

#endif