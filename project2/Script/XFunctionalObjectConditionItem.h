// XFunctionalObjectConditionItem.h: interface for the _XFunctionalObjectConditionItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFUNCTIONALOBJECTCONDITIONITEM_H__24ABF86B_FCE6_408A_B033_A83AF14EBEBE__INCLUDED_)
#define AFX_XFUNCTIONALOBJECTCONDITIONITEM_H__24ABF86B_FCE6_408A_B033_A83AF14EBEBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "XFunctionalObjectScriptDefine.h"

class _XFunctionalObjectConditionItem  
{
public:	
	
	_XTRIGGERDATA_CONDITION m_TDC;	
	TCHAR					m_ConditionString[64];	
	int 					m_ConditionID;
	
public:
	_XFunctionalObjectConditionItem();
	virtual ~_XFunctionalObjectConditionItem();
	
	// 복사 연산자
	const _XFunctionalObjectConditionItem& operator =(const _XFunctionalObjectConditionItem& conditionitem)
	{
		if(&conditionitem == this) return *this;
		
		m_TDC = conditionitem.m_TDC;
		this->SetConditionString(conditionitem.m_ConditionString);
		
		m_ConditionID = conditionitem.m_ConditionID;
		
		return *this;
	}
	
	void					ResetData(void);
	void					SetConditionString(LPCTSTR conditionstring);
	
	BOOL					Load(FILE* fileptr);
	BOOL					LoadString(FILE* fileptr, LPTSTR& string);
		
	_XFUNCTIONALOBJECT_CONDITION_RESULT		Process(void);
	
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckClan(void);					// 플레이어_문파비교
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckItem(void);					// 플레이어_아이템검사
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckGender(void);				// 플레이어_성별검사	
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckQuestComplete(void);		// 퀘스트 완료 검사
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckState(void);				// 플레이어_상태효과검사
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckCastleBattleStage(void);	// 장원전_단계검사	
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckCastleBattleThreat(void);	// 퀘스트 장원전_협박가능상태검사 
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckPlayerRole(void);			// 플레이어_역할검사
};

#endif // !defined(AFX_XFUNCTIONALOBJECTCONDITIONITEM_H__24ABF86B_FCE6_408A_B033_A83AF14EBEBE__INCLUDED_)
