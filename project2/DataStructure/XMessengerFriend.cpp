// XMessengerFriend.cpp: implementation of the _XMessengerFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMessengerFriend.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XMessengerFriend::_XMessengerFriend()
{
	memset(m_FriendName, 0, sizeof(TCHAR)*13);
	m_FriendState = 0 ; 
	m_FriendNumber = -1;

}

_XMessengerFriend::~_XMessengerFriend()
{

}
