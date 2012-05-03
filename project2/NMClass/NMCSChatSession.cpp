#include "stdafx.h"
#include "XKernel.h"

#include "NMManager.h"
#include "NMCSChatSession.h"

CNMCSChatSession::CNMCSChatSession() : CNMGSGeneralSession( kSessionType_Chat )
{
}

CNMCSChatSession::~CNMCSChatSession()
{
}

BOOL CNMCSChatSession::SendChatMessage( CNMChatMessage& chatMsg )
{
	CNMCSSendChatMessageFunc	
				func( this->GetSerialKey(), chatMsg );
	return		CNMManager::GetInstance().CallNMFunc( &func );
}
