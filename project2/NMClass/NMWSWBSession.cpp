#include "stdafx.h"
#include "XKernel.h"

#include "NMManager.h"
#include "NMWSWBSession.h"

CNMWSWBSession::CNMWSWBSession() : CNMGSGeneralSession( kSessionType_WhiteBoard )
{
}

CNMWSWBSession::~CNMWSWBSession()
{
}

_UInt32_ CNMWSWBSession::GetAssocSerialKey( void )
{
	CNMWSGetAssocSerialKeyFunc
				func( this->GetSerialKey() );
	return		( ( CNMManager::GetInstance().CallNMFunc( &func ) ) ? func.uAssocSerialKey : 0 );
}

BOOL CNMWSWBSession::SendWhiteBoardMsg( _UInt32_ uContentId, BYTE* pBuf, _SInt32_ nBufLen )
{
	CNMWSSendWBMessageFunc
				func( this->GetSerialKey(), uContentId, pBuf, nBufLen );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}
