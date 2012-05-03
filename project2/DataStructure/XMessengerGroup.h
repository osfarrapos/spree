// _XMessengerGroup.h: interface for the _XMessengerGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__XMESSENGERGROUP_H__1362C250_67C4_457C_AAF3_A0D18C3F3C09__INCLUDED_)
#define AFX__XMESSENGERGROUP_H__1362C250_67C4_457C_AAF3_A0D18C3F3C09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"
#include "XList.h"

class _XMessengerGroup  : public _XItem  
{
public:
	TCHAR			m_GroupName[15] ;
	_XList			m_FriendList;
	BOOL			m_bSelected;
	int				m_GroupNumber;

public:
	_XMessengerGroup();
	virtual ~_XMessengerGroup();

};

#endif // !defined(AFX__XMESSENGERGROUP_H__1362C250_67C4_457C_AAF3_A0D18C3F3C09__INCLUDED_)
