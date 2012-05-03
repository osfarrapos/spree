// XFunctionalObjectTriggerItem.h: interface for the _XFunctionalObjectTriggerItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFUNCTIONALOBJECTTRIGGERITEM_H__7114D361_312F_42CA_BAF0_2F9B3C66FCFA__INCLUDED_)
#define AFX_XFUNCTIONALOBJECTTRIGGERITEM_H__7114D361_312F_42CA_BAF0_2F9B3C66FCFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "XFunctionalObjectScriptDefine.h"
#include "XFunctionalObjectActionItem.h"
#include "XFunctionalObjectConditionItem.h"

#include <map>
using namespace std;

typedef struct 
{	
	DWORD			condition;/// 조건식 진행 리스트	
	DWORD			normalaction;/// normal 행동식 진행 리스트	
	DWORD			exceptedaction;/// excepted 행동식 진행 리스트
}_XPROGRESS_FUNCTIONALOBJECTTRIGGER;


class _XFunctionalObjectTriggerItem  : public _XItem
{
public:
	LPTSTR			m_lpszTriggerName;
	BOOL			m_bEnable;
	
	BYTE			m_byCompareClass;
		
	int				m_TriggerID;	

	map <int, _XFunctionalObjectConditionItem*> m_mapConditionList;
	map <int, _XFunctionalObjectActionItem*>	m_mapNormalActionList;
	map <int, _XFunctionalObjectActionItem*>	m_mapExceptedActionList;
	
	_XPROGRESS_FUNCTIONALOBJECTTRIGGER			m_ProgressFunctionalObjectTrigger;
		
public:
	_XFunctionalObjectTriggerItem();
	virtual ~_XFunctionalObjectTriggerItem();
	
	void			SetTriggerName(LPCTSTR triggername);
	BOOL			Load(FILE* fileptr);	
	BOOL			LoadString(FILE* fileptr, LPTSTR& string);	

	void			SetProgressCondition(int conditionid);
	void			SetProgressNormalAction(int actionid);
	void			SetProgressExceptedAction(int actionid);
	DWORD			GetCompareValue(int comparecount);
	
};

#endif // !defined(AFX_XFUNCTIONALOBJECTTRIGGERITEM_H__7114D361_312F_42CA_BAF0_2F9B3C66FCFA__INCLUDED_)
