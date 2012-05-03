/** 트리거를 표현하는 클래스.
*
* \author Sohyun, Park
* \date 2003.09.04
*/

#ifndef _KUTRIGGERITEM_H_
#define _KUTRIGGERITEM_H_

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "XTriggerConditionItem.h"
#include "XTriggerActionItem.h"

class _XTriggerItem
{
protected :
	BOOL			m_bEnable;
	
	BOOL			m_bClientEnable;
	BOOL			m_bServerEnable;
	BOOL			m_bConfirmAll;
	BOOL			m_bComplete;
	
	BOOL			m_bLastTrigger;
	BOOL			m_bExecNormal;
	
	BOOL			m_bFormerTriggerSuccess;
	BOOL			m_bJumpAction;
	BOOL			m_bMainQuest;
	
	BYTE			m_byCompareClass;
	BYTE			m_byTriggerClass;
	
	int				m_nTriggerID;
	int				m_nOwnerQuestID;
	
	int				m_nOwnerNPCID;

	int				m_CurrentTreeNodeNumber;
	
	LPTSTR			m_lpszTriggerName;
	
	_XQUEST_TYPE	m_QuestType;// 퀘스트 타입 ( 제룡행, 무림행, 협객행, 기연행, 무림소사 )
	
public :
	BOOL			m_bWaitRewordPacket;
	int				m_SuccessConditionid[20];
	int				m_SuccessActionid[20];
	
	map <int, _XTriggerConditionItem*> m_mapConditionList;
	map <int, _XTriggerActionItem*> m_mapNormalActionList;
	map <int, _XTriggerActionItem*> m_mapExceptedActionList;
	
	
public :
	_XTriggerItem();
	~_XTriggerItem();
	
	void			SetTriggerName(LPCTSTR triggername);
	
	BOOL			Load(FILE* fileptr);
	int				LoadString(FILE* fileptr, LPTSTR& string);
	
	void			SetTreeNodeNumber(int treenumber);
	int				GetTreeNodeNumber(void);
	
	void			SetTriggerClass(BYTE triggerclass);
	BYTE			GetTriggerClass(void);
	
	void			SetTriggerID(int triggerid);
	int				GetTriggerID(void);
	
	void			SetOwnerQuestID(int questid);
	int				GetOwnerQuestID(void);
	
	void			SetComplete(BOOL complete);
	BOOL			GetComplete(void);
	
	void			SetClientEnable(BOOL enable);
	BOOL			GetClientEnable(void);
	
	void			SetEnable(BOOL enable);
	BOOL			GetEnable(void);
	
	void			SetServerEnable(BOOL enable);
	BOOL			GetServerEnable(void);
	
	void			SetConfirmAll(BOOL enable);
	BOOL			GetConfirmAll(void);
	
	void			SetProgressAction( BOOL bNormal );
	
	void			SetLastTrigger(BOOL last);
	BOOL			GetLastTrigger(void);
	
	void			SetFormerTriggerSuccess(BOOL success);
	BOOL			GetFormerTriggerSuccess(void);
	
	_XQUESTTRIGGER_RESULT		Process(void);
	
	void			Reset(void);
	void			ReleasePrivateData(void);
	
//	void			ProcessTest(int actionid);
//	void			RestoreTest(int actionid);

	
	void			SetOwnerNPCID(int npcid);
	int				GetOwnerNPCID(void);

	void			SetJumpAction(BOOL bjumpaction ) { m_bJumpAction = bjumpaction; }
	BOOL			GetJumpAction(void) { return m_bJumpAction; }
	
	// 퀘스트 타입 ( 제룡행, 무림행, 협객행, 기연행, 무림소사 )
	void			SetQuestType(_XQUEST_TYPE questtype ) { m_QuestType = questtype; }
	_XQUEST_TYPE	GetQuestType(void)	{	return m_QuestType;	};
	
	void			SetMainQuest(BOOL mainquest ) { m_bMainQuest = mainquest; }
	BOOL			GetMainQuest(void);

	void			SetPlayNormalAction(BOOL bexecnormal ) { m_bExecNormal = bexecnormal; }
	BOOL			GetPlayNormalAction(void);
};

#endif