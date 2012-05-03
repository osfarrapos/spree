/** 퀘스트 스크립트 하나를 나타내는 클래스.
 *
 * \author Sohyun, Park
 * \date 2003.08.26
 */

#ifndef _KUQUESTSCRIPTITEM_H_
#define _KUQUESTSCRIPTITEM_H_

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "XTriggerConditionItem.h"
#include "XTriggerItem.h"
#include "XTriggerTreeItem.h"

typedef enum _XQuestStart_Mode
{
	_XQUESTSTART_NONE = 0,
		_XQUESTSTART_NPCTARGETTING,
		_XQUESTSTART_OBJECTCLICK,
		_XQUESTSTART_USINGITEM,
		_XQUESTSTART_WEARITEM,
		_XQUESTSTART_DESTROYITEM,
		_XQUESTSTART_LEVEL,
		_XQUESTSTART_ZONE
}_XQUESTSTART_MODE;

typedef struct _XQuest_CounterInfomation
{
	int		CounterType;
	int		CounterTargetType;
	int		CounterTargetID;
	int		CountTotalPoint;
	int		CountCurrentPoint;
	TCHAR	CounterTargetName[256];
	
}_XQUEST_COUNTINFOMATION;

typedef struct _XQuest_PartyCounterInfomation
{
	int		CounterType;
	int		CounterTargetType;
	int		CounterTargetID;
	int		CountTotalPoint;
	int		CountCurrentPoint;
	int		CounterPointPerMember[9];
	TCHAR	CounterTargetName[256];
	
}_XQUEST_PARTYCOUNTINFOMATION;

#pragma warning (disable: 4786)
#include <map>
using namespace std;

class _XQuestScriptItem
{
protected :
	BOOL			m_bEnable;
	BOOL			m_bAutorun;
	BOOL			m_bMainQuest;		//제룡행 유무
	BOOL			m_bPartyQuest;		//파티 퀘스트 유무
	bool			m_bTempQuest;		//저장되지 않는 퀘스트 무한 반복 가능한 퀘스트 유무

	BOOL			m_bClientEnable;
	BOOL			m_bServerEnable;
	BOOL			m_bConfirmAll;
	
	BOOL			m_bComplete;
	BOOL			m_bInsertCompleteList;
	
	BOOL			m_bWaitingFailResult;
	BOOL			m_bCheckFirstStartCondition;
	
	BOOL			m_bRewardProcessNPCMarkCheck;

	LPTSTR			m_lpszQuestName;
	LPTSTR			m_lpszQuestSubName;

	BYTE			m_byQuestCancelCase;
	BYTE			m_QuestEmailType;
	BYTE			m_byStartConditionCompareClass;
	BYTE			m_byQuestOptionCompareClass;// 옵션 조건식의 모두만족, 하나이상 만족 
	
	BYTE			m_RankQuestType1;		//0:개인퀘스트 1:동행퀘스트 2:단퀘스트 
	BYTE			m_RankQuestType2;		//0:KillCount 1:Timer 2:ItemCount	
	
	float			m_queststartprobability;
	
	int				m_nQuestID;
	int				m_QuestChapter;
	int				m_nStartTime;
	int				m_nQuestMailDelayTime;

	int				m_nNextTriggerID;
	int				m_nCurrentTriggerID;
	
	int				m_NextTreeNodeNumber;
	int				m_CurrentTreeNodeNumber;

	int				m_FailConditionTimer ;		// 실패조건식 검사를 위한 딜레이
	
	int				m_nLastTargettingNPCID;

	_XQUEST_COMPLETE_RESULT		m_QuestServerResult;
	_XQUEST_COMPLETE_RESULT		m_QuestCompleteResult;

	
	// 퀘스트의 시작 조건 타입을 설정하여 타입에 맞는 조건일 때 시작 조건을 검사하여 부하를 줄인다.
	_XQUESTSTART_MODE			m_QuestStartMode;

	_XQUEST_TYPE				m_QuestType;// 퀘스트 타입 ( 제룡행, 무림행, 협객행, 기연행, 무림소사 )
	
	int				m_nStartYear;
	int				m_nStartMonth;
	int				m_nStartDay;
	int				m_nStartHour;

	int				m_nEndYear;
	int				m_nEndMonth;
	int				m_nEndDay;
	int				m_nEndHour;

	unsigned int	m_unCountryCode;

public :
	LPTSTR			m_lpszStartQuestMessage;
	LPTSTR			m_lpszFailQuestMessage;
	LPTSTR			m_lpszOutlineMessage;
	LPTSTR			m_lpszOutlineMessage2;
	LPTSTR			m_startquestinformationmessage;
	LPTSTR			m_lpszQuestProgressinformation;//퀘스트 창에 표시된 퀘스트 진행 정보

	int				m_SelectionNumber;
	BOOL			m_bSelectionNumberCheck;
	
	int				m_QuestScriptItemIndexNumber; // 퀘스트창에서의 퀘스트 리스트 위치를 표시하기 위한 변수
	BOOL			m_bSelectedQuestScriptItem; // 퀘스트창에서의 퀘스트 리스트 위치를 표시하기 위한 변수

	BOOL			m_bChangeQuestStartMode; // 퀘스트 시작모드가 변했을때  

	// 퀘스트 시작 조건의 종류와 갯수를 알 수 있는 변수 
	int				m_QuestStartConditionValue; // 100000 : ObjectClick
												// 10000  : Using Item
												// 1000	  : Wear Item
												// 100	  : Level
												// 10	  : Zone or None
												// 1	  : NPC targetting
												// 1000000: Destroy Item 
	
	int				m_SuccessStartConditionid[20];

	_XQUEST_COUNTINFOMATION m_CounterInformation[4];// 카운터 포인트를 저장하는 배열
	_XQUEST_COUNTINFOMATION m_ItemCounterInformation[8];
	_XQUEST_PARTYCOUNTINFOMATION m_PartyCounterInformation[4];// 카운터 포인트를 저장하는 배열

	map <int, _XTriggerConditionItem*> m_mapStartConditionList;
	map <int, _XTriggerConditionItem*> m_mapOptionConditionList;// 옵션 조건식 리스트
	map <int, _XTriggerConditionItem*> m_mapFailConditionList;// 조건을 만족하지 않으면 실패
	map <int, _XTriggerItem*> m_mapSuccessTriggerList;
	map <int, _XTriggerTreeItem*> m_mapTriggerTreeList;
	map <int, int> m_mapTriggerTimer;

	int				m_LimitQuestCountStartType;		//부여선착순 퀘스트 갯수
	int				m_LimitQuestCountCompleteType;	//완료선착순 퀘스트 갯수

	BOOL			m_bSendClientPacketAtPartyQuest; //파티퀘스트에서 자기 자신이 족너에 성공해서 패킷을 보냈는지를 알 수 있는 변수 

public :
	_XQuestScriptItem();
	~_XQuestScriptItem();
	
	void			SetStartTime(int starttime);
	int				GetStartTime(void);
	
	BOOL			GetPartyQuest(void)				{	return m_bPartyQuest;	};
	bool			GetTempQuest(void)				{	return m_bTempQuest;	};

	void			SetClientEnable(BOOL enable)	{	m_bClientEnable = enable;	};
	BOOL			GetClientEnable(void)			{	return m_bClientEnable;	};
	
	void			SetServerEnable(BOOL enable)	{	m_bServerEnable = enable;	};
	BOOL			GetServerEnable(void)			{	return m_bServerEnable;	};

	void			SetConfirmAll(BOOL enable);

	void			SetTriggerClientEnable(int triggerid, BOOL enable );
	void			SetTriggerServerEnable(int currenttreenumber, BOOL enable, BOOL jumpaction = FALSE );
	void			SetTriggerPlayActionAndServerEnable(int currenttreenumber, BOOL enable, BOOL bnormalaction, BOOL jumpaction = FALSE );
	void			SetTriggerConfirmAll(int triggerid, BOOL enable );

	void			SetProgressTrigger(int currenttreenumber, BOOL bNormal );
	void			SetTriggerComplete(int triggerid, BOOL enable);

	void			SetCurrentTriggerID(int triggerid);
	int				GetCurrentTriggerID(void);

	void			SetCurrentTreeNumber(int treenumber, BOOL bconditioncheck = TRUE );
	int				GetCurrentTreeNumber(void);
	
	void			SetNextTreeNumber(int nexttreenumber);
	int				GetNextTreeNumber(void);

	void			SetNextTriggerID(int triggerid);
	int				GetNextTriggerID(void);

	void			SetQuestID(int questid);
	int				GetQuestID(void);

	void			SetComplete(BOOL complete);
	BOOL			GetComplete(void);

	void			SetInsertCompleteList(BOOL complete);
	BOOL			GetInsertCompleteList(void);

	void			SetAutoRun(BOOL autorun);
	BOOL			GetAutoRun(void);

	void			SetWaitingFailResult(BOOL waiting);
	BOOL			GetWaitingFailResult(void);
	
	void			SetFirstStartCondition(BOOL checkfirststartcondition );
	BOOL			GetFirstStartCondition(void);
		
	void			ReSetQuestMark();
	void			ResetCounterInformation(void);
	void			ResetPartyCounterInformation(void);
	void			ResetItemCounterInformation(void);


	_XQUESTSTARTCONDITION_RESULT		CheckStartCondition(void);
	_XQUESTITEM_RESULT					Process(void);

	BOOL			ProcessRewordQuest(void);

	BOOL			Load(FILE* fileptr);
	int				LoadString(FILE* fileptr, LPTSTR& string);

	void							SetQuestServerResult(_XQUEST_COMPLETE_RESULT result);
	_XQUEST_COMPLETE_RESULT			GetQuestServerResult(void);

	void							SetQuestCompleteResult(_XQUEST_COMPLETE_RESULT result);
	_XQUEST_COMPLETE_RESULT			GetQuestCompleteResult(void);

	void			Reset(void);
	void			ReleasePrivateData(void);
	BOOL			GetTriggerComplete(int triggerid);

//	void			ProcessTest(int triggerid, int actionid);
//	void			RestoreTest(int triggerid, int actionid);

	int				GetQuestMailDelayTime(void) { return m_nQuestMailDelayTime;	};
	
	BOOL			GetMainQuest(void);
	
	LPTSTR			GetQuestName(void);
	LPTSTR			GetQuestSubName(void);

	void			SetLastTarggetingNPCID(int npcid)	{	m_nLastTargettingNPCID = npcid;	};
	int				GetLastTarggetingNPCID(void)		{	return m_nLastTargettingNPCID;	};

	int				GetOwnerNPCID(int triggerid);

	// 퀘스트 시작 조건 타입을 반환한다.
	void				SetQuestStartConditionMode(_XQUESTSTART_MODE queststartmode)	{	m_QuestStartMode = queststartmode;	};
	_XQUESTSTART_MODE	GetQuestStartConditionMode(void)	{	return m_QuestStartMode;	};
	
	// 퀘스트 타입 ( 제룡행, 무림행, 협객행, 기연행, 무림소사 )
	_XQUEST_TYPE	GetQuestType(void)	{	return m_QuestType;	};

	BYTE			GetQuestCancleType(void)	{	return m_byQuestCancelCase;	};
	
	void			RemoveQuestStartConditionList(void);

	BOOL			CheckFailConditionAtOptionCondition(void);
	
	BYTE			GetQuestRankType1(void)		{ return m_RankQuestType1; };
	BYTE			GetQuestRankType2(void)		{ return m_RankQuestType2; };
	
	BOOL			CheckStartNEndTime(void);
	BOOL			CheckCountryCode(void);

};


#endif
