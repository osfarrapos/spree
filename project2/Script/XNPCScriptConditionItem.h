// XNPCScriptConditionItem.h: interface for the _XNPCScriptConditionItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XNPCSCRIPTCONDITIONITEM_H__F28B7133_BEA4_40DF_9EBB_8B63CB9C907E__INCLUDED_)
#define AFX_XNPCSCRIPTCONDITIONITEM_H__F28B7133_BEA4_40DF_9EBB_8B63CB9C907E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"
#include "XList.h"
#include "XNPC.h"

typedef struct
{
	BOOL		m_enable;				// ...
	int			m_conditionclass;		// 트리거 조건식 클래스
	
	int 		m_compareclass1 : 8;	// 계산 관련 클래스  At least, at most, exactly	
	int		    m_compareclass2 : 8;
	int 		m_compareclass3 : 8;
	int 		m_compareclass4 : 8;
	
	int			m_numericvalue1;		// 계산 관련 수치
	int			m_numericvalue2;
	int			m_numericvalue3;
	int			m_numericvalue4;
	
} _XNPCSUBSCRIPT_CONDITION;

class _XNPCScriptConditionItem : public _XItem 
{
public:
	
	_XNPCSUBSCRIPT_CONDITION m_NPCSubscriptCondition;
	
	LPTSTR					m_comparestring;
	TCHAR					m_conditionstring[64];	
	int 					m_conditionid;
	
public:
	_XNPCScriptConditionItem();
	virtual ~_XNPCScriptConditionItem();
	
	// 복사 연산자
	const _XNPCScriptConditionItem& operator =(const _XNPCScriptConditionItem& conditionitem)
	{
		if(&conditionitem == this) return *this;
		
		m_NPCSubscriptCondition = conditionitem.m_NPCSubscriptCondition;
		this->SetCompareString(conditionitem.m_comparestring);
		this->SetConditionString(conditionitem.m_conditionstring);
		
		return *this;
	}
	
	void					ResetData(void);

	void					SetCompareString(LPCTSTR comparestring);
	void					SetConditionString(LPCTSTR conditionstring);
	
	BOOL					Load(FILE* fileptr);
	BOOL					LoadString(FILE* fileptr, LPTSTR& string);

	
	_XNPCCONDITION_RESULT		Process(void);

	_XNPCCONDITION_RESULT		ProcessCheckClanNPC(void);
	_XNPCCONDITION_RESULT		ProcessCheckClanFriendship(void);
	_XNPCCONDITION_RESULT		ProcessCheckClanHostility(void);
	_XNPCCONDITION_RESULT		ProcessCheckHonor(void);	
	_XNPCCONDITION_RESULT		ProcessCheckConditionOfNPCExistence(void);	
	_XNPCCONDITION_RESULT		ProcessCheckPlayerRole(void);	
};

#endif // !defined(AFX_XNPCSCRIPTCONDITIONITEM_H__F28B7133_BEA4_40DF_9EBB_8B63CB9C907E__INCLUDED_)
