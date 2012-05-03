// XStringResourceManager.h: interface for the _XStringResourceManager class.
//
// Author : Sohyun, Park
// Created Date : 2004.01.05
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSTRINGRESOURCEMANAGER_H_
#define _XSTRINGRESOURCEMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)
#include <map>
using namespace std;

class _XStringResourceManager  
{
public :
	map <int, LPTSTR>	m_mapStringList;
public:
	_XStringResourceManager();
	virtual ~_XStringResourceManager();

	void				Release(void);
	BOOL				LoadStringResourceFromFile(void);
	LPTSTR				GetString(int id);

};

#endif
