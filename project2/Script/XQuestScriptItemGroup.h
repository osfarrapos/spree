// XQuestScriptItemGroup.h: interface for the _XQuestScriptItemGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XQUESTSCRIPTITEMGROUP_H__5E634FF1_F40F_4F4F_88E9_D9BE3AEC3A1E__INCLUDED_)
#define AFX_XQUESTSCRIPTITEMGROUP_H__5E634FF1_F40F_4F4F_88E9_D9BE3AEC3A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XItem.h"
class _XQuestScriptItemGroup   : public _XItem  
{
public:
	TCHAR			m_QuestGroupName[512] ;
	BOOL			m_bSelected;
	int				m_QuestGroupNumber;
	int				m_QuestGroupIndexNumber;
	
public:
	_XQuestScriptItemGroup();
	virtual ~_XQuestScriptItemGroup();

};

#endif // !defined(AFX_XQUESTSCRIPTITEMGROUP_H__5E634FF1_F40F_4F4F_88E9_D9BE3AEC3A1E__INCLUDED_)
