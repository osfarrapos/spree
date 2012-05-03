#ifdef _XDEF_NEXONLOGIN

#ifndef __NMCSCHATSESSION_H_20B4B8D8_E71E_4869_A501_E27577FC58F6__
#define __NMCSCHATSESSION_H_20B4B8D8_E71E_4869_A501_E27577FC58F6__

#include "NMGSGeneralSession.h"

class CNMCSChatSession : public CNMGSGeneralSession
{
public:
	CNMCSChatSession();
	virtual ~CNMCSChatSession();

	// For Chatting
	BOOL	SendChatMessage( CNMChatMessage& chatMsg );
};

#endif	//	#ifndef __NMCSCHATSESSION_H_20B4B8D8_E71E_4869_A501_E27577FC58F6__

#endif