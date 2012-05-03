#include "stdafx.h"
#include "XKernel.h"

#include "NMManager.h"
#include "NMFDSFileDownloadSession.h"

CNMFDSFileDownloadSession::CNMFDSFileDownloadSession() : CNMGSGeneralSession( kSessionType_FileDownload )
{
}

CNMFDSFileDownloadSession::~CNMFDSFileDownloadSession()
{
}

BOOL CNMFDSFileDownloadSession::GetFile( _UInt32_ uEventId )
{
	CNMFDSControlFunc	
				func( this->GetSerialKey(), CNMFDSControlFunc::kType_GetFile, uEventId );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMFDSFileDownloadSession::CancelFile( _UInt32_ uEventId )
{
	CNMFDSControlFunc	
				func( this->GetSerialKey(), CNMFDSControlFunc::kType_Cancel, uEventId );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMFDSFileDownloadSession::RemoveFile( _UInt32_ uEventId )
{
	CNMFDSControlFunc	
				func( this->GetSerialKey(), CNMFDSControlFunc::kType_Remove, uEventId );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMFDSFileDownloadSession::GetFileEvents( NMFileEventList& aEventList )
{
	CNMFUDSGetFileEventFunc
				func( this->GetSerialKey() );

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		aEventList	= func.aEventList;
		return TRUE;
	}

	return FALSE;
}

BOOL CNMFDSFileDownloadSession::GetDownloadDir( LPTSTR szDir )
{
	CNMFDSGetDownloadDirFunc	
				func( this->GetSerialKey() );

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		NMSAFE_STRCOPY( szDir, func.szDir, MAX_PATH );
		return TRUE;
	}

	return FALSE;
}

BOOL CNMFDSFileDownloadSession::SetDownloadDir( LPCTSTR szDir )
{
	CNMFDSSetDownloadDirFunc	
				func( this->GetSerialKey(), szDir );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}
