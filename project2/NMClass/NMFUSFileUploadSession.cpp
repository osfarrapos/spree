#include "stdafx.h"
#include "XKernel.h"

#include "NMManager.h"
#include "NMFUSFileUploadSession.h"

CNMFUSFileUploadSession::CNMFUSFileUploadSession() : CNMGSGeneralSession( kSessionType_FileUpload )
{
}

CNMFUSFileUploadSession::~CNMFUSFileUploadSession()
{
}

_UInt32_ CNMFUSFileUploadSession::SendFile( LPCTSTR szFileName )
{
	CNMFUSSendFileFunc	
				func( this->GetSerialKey(), szFileName );
	return		( CNMManager::GetInstance().CallNMFunc( &func ) ? func.uEventId : 0 );
}

BOOL CNMFUSFileUploadSession::SendFileAgain( _UInt32_ uEventId )
{
	CNMFUSControlFunc	
				func( this->GetSerialKey(), CNMFUSControlFunc::kType_SendFileAgain, uEventId );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMFUSFileUploadSession::CancelFile( _UInt32_ uEventId )
{
	CNMFUSControlFunc	
				func( this->GetSerialKey(), CNMFUSControlFunc::kType_Cancel, uEventId );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMFUSFileUploadSession::RemoveFile( _UInt32_ uEventId )
{
	CNMFUSControlFunc	
				func( this->GetSerialKey(), CNMFUSControlFunc::kType_Remove, uEventId );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL CNMFUSFileUploadSession::GetFileEvents( NMFileEventList& aEventList )
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
