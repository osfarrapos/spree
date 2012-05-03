// XMessengerFriend.h: interface for the _XMessengerFriend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMESSENGERFRIEND_H__D97C8A0B_C6D1_4F87_B606_41B2FD59662F__INCLUDED_)
#define AFX_XMESSENGERFRIEND_H__D97C8A0B_C6D1_4F87_B606_41B2FD59662F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"

class _XMessengerFriend  : public _XItem  
{
public:
	TCHAR			m_FriendName[13] ;
	int				m_FriendState;
	int				m_FriendNumber;
	
public:
	_XMessengerFriend();
	virtual ~_XMessengerFriend();

};

#endif // !defined(AFX_XMESSENGERFRIEND_H__D97C8A0B_C6D1_4F87_B606_41B2FD59662F__INCLUDED_)
