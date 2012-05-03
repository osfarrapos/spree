// XMessengerGroup.cpp: implementation of the _XMessengerGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMessengerGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

 _XMessengerGroup::_XMessengerGroup()
{
	memset(m_GroupName, 0, sizeof(TCHAR)*15);
	m_bSelected = FALSE;
	m_GroupNumber = -1;
}

_XMessengerGroup::~_XMessengerGroup()
{
	m_FriendList.disposeList();

}
