// XQuestScriptItemGroup.cpp: implementation of the _XQuestScriptItemGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XQuestScriptItemGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XQuestScriptItemGroup::_XQuestScriptItemGroup()
{
	
	memset(m_QuestGroupName, 0, sizeof(TCHAR)*512);
	m_bSelected = FALSE;
	m_QuestGroupNumber = -1;
	m_QuestGroupIndexNumber = -1;
}

_XQuestScriptItemGroup::~_XQuestScriptItemGroup()
{
}
