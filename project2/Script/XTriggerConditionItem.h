/** 조건식을 표현하는 클래스.
 *
 * \author Sohyun, Park
 * \date 2003.08.28
 */

#ifndef _KUTRIGGERCONDITION_H_
#define _KUTRIGGERCONDITION_H_

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"

class _XTriggerConditionItem
{
public :
	_XTRIGGERDATA_CONDITION		m_TDC;
	BOOL						m_bSuccessCancle;
	BOOL						m_bStartCondition;
	
protected :
	LPTSTR						m_lpszCompareString;
	TCHAR						m_szConditionString[64];
	
	int							m_nConditionID;
	int							m_nOwnerQuestID;
	int							m_nOwnerTriggerID;
	
	BYTE						m_byOwnerTriggerClass;
	
	DWORD						m_LastCheckTime;
	
	BOOL						m_bHoldCheckingOfNPCTargetting; // NPC 타게팅 검사에서 잡고 있기 위한 변수
	BOOL						m_bMainQuest;
	_XQUEST_TYPE				m_QuestType;// 퀘스트 타입 ( 제룡행, 무림행, 협객행, 기연행, 무림소사 )

public :
	_XTriggerConditionItem();
	~_XTriggerConditionItem();

	void						ResetData(void);

	const _XTriggerConditionItem& operator =(const _XTriggerConditionItem& conditionitem)
	{
		if(&conditionitem == this) 
			return *this;

		m_TDC = conditionitem.m_TDC;
		this->SetCompareString(conditionitem.m_lpszCompareString);
		this->SetConditionString(conditionitem.m_szConditionString);

		m_nConditionID		= conditionitem.m_nConditionID;
		m_nOwnerQuestID		= conditionitem.m_nOwnerQuestID;
		m_nOwnerTriggerID	= conditionitem.m_nOwnerTriggerID;
		m_bSuccessCancle	= conditionitem.m_bSuccessCancle;
		m_bStartCondition	= conditionitem.m_bStartCondition;

		return *this;
	}

	void					SetCompareString(LPCTSTR comparestring);
	void					SetConditionString(LPCTSTR conditionstring);

	BOOL					Load(FILE* fileptr);
	int						LoadString(FILE* fileptr, LPTSTR& string);

	void					SetConditionID(int conditionid);
	int						GetConditionID(void);

	void					SetOwnerQuestID(int questid);
	int						GetOwnerQuestID(void);

	void					SetOwnerTriggerID(int triggerid);
	int						GetOwnerTriggerID(void);

	void					SetOwnerTriggerClass(BYTE triggerclass);
	BYTE					GetOwnerTriggerClass(void);
	
	void					SetQuestType(_XQUEST_TYPE questtype ) { m_QuestType = questtype; }
	_XQUEST_TYPE			GetQuestType(void)	{	return m_QuestType;	};
	
	void					SetMainQuest(BOOL mainquest ) { m_bMainQuest = mainquest; }
	BOOL					GetMainQuest(void);
	
	void					SetHoldCheckingOfNPCTargetting(BOOL bholdcheckingofnpctargetting ) { m_bHoldCheckingOfNPCTargetting = bholdcheckingofnpctargetting; }
	BOOL					GetHoldCheckingOfNPCTargetting(void) {	return m_bHoldCheckingOfNPCTargetting;	};
	
	BOOL					CheckJoinGroup(int groupindex );

	_XQUESTCONDITION_RESULT		Process(void);

	_XQUESTCONDITION_RESULT		ProcessCheckValue(void);
	_XQUESTCONDITION_RESULT		ProcessCheckTotalLevel(void);
	_XQUESTCONDITION_RESULT		ProcessCheckVitality(void);
	_XQUESTCONDITION_RESULT		ProcessCheckZen(void);
	_XQUESTCONDITION_RESULT		ProcessCheckSoul(void);
	_XQUESTCONDITION_RESULT		ProcessCheckHeart(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStamina(void);
	_XQUESTCONDITION_RESULT		ProcessCheckVitalPower(void);
	_XQUESTCONDITION_RESULT		ProcessCheckInnerPower(void);
	_XQUESTCONDITION_RESULT		ProcessCheckSoulPower(void);
	_XQUESTCONDITION_RESULT		ProcessCheckHeartPower(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStaminaPower(void);
	_XQUESTCONDITION_RESULT		ProcessCheckHonor(void);
	_XQUESTCONDITION_RESULT		ProcessCheckRetribution(void);
	_XQUESTCONDITION_RESULT		ProcessHaveSkill(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatus(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusBattle(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusRideHorse(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusInParty(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusNotParty(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusJoinGroup(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusNotJoinGroup(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusLevelUp(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusTradeNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusTalkNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusDeath(void);
	_XQUESTCONDITION_RESULT		ProcessCheckStatusRevive(void);
	_XQUESTCONDITION_RESULT		ProcessCheckGender(void);
	_XQUESTCONDITION_RESULT		ProcessCheckJoinGroup(void);
	_XQUESTCONDITION_RESULT		ProcessCheckArea(void);
	_XQUESTCONDITION_RESULT		ProcessCheckMessage(void);
	_XQUESTCONDITION_RESULT		ProcessHaveItem(void);
	_XQUESTCONDITION_RESULT		ProcessCheckAttack(void);
	_XQUESTCONDITION_RESULT		ProcessCheckMartial(void);
	_XQUESTCONDITION_RESULT		ProcessCheckPartyCount(void);
	_XQUESTCONDITION_RESULT		ProcessCheckPartyStatus(void);
	_XQUESTCONDITION_RESULT		ProcessCheckAreaOther(void);
	_XQUESTCONDITION_RESULT		ProcessCheckAreaParty(void);
	_XQUESTCONDITION_RESULT		ProcessCheckAreaStaticNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckAreaDynamicNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckAreaMob(void);
	_XQUESTCONDITION_RESULT		ProcessCheckDistance(void);
	_XQUESTCONDITION_RESULT		ProcessCheckDistanceFromStaticNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckDistanceFromDynamicNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckDistanceFromArea(void);
	_XQUESTCONDITION_RESULT		ProcessCheckTargetting(void);
	_XQUESTCONDITION_RESULT		ProcessCheckTargettingArea(void);
	_XQUESTCONDITION_RESULT		ProcessCheckTargettingStaticNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckTargettingDynamicNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckTargettingObject(void);
	_XQUESTCONDITION_RESULT		ProcessCheckTimer(void);
	_XQUESTCONDITION_RESULT		ProcessCheckPassTime(void);
	_XQUESTCONDITION_RESULT		ProcessCheckProcedingTrigger(void);
	_XQUESTCONDITION_RESULT		ProcessCheckActionID(void);
	_XQUESTCONDITION_RESULT		ProcessCheckProcedingQuest(void);
	_XQUESTCONDITION_RESULT		ProcessCheckIsProgressQuest(void);
	_XQUESTCONDITION_RESULT		ProcessCheckProgressQuest(void);
	_XQUESTCONDITION_RESULT		ProcessCheckSelection(void);
	_XQUESTCONDITION_RESULT		ProcessCheckNPCDialogMode(void);
	_XQUESTCONDITION_RESULT		ProcessCheckWearItem(void);
	_XQUESTCONDITION_RESULT		ProcessSetQuestMarkArea(void);
	_XQUESTCONDITION_RESULT		ProcessSetQuestMarkNPC(void);
	_XQUESTCONDITION_RESULT		ProcessSetHintMarkArea(void);
	_XQUESTCONDITION_RESULT		ProcessSetHintMarkNPC(void);
	_XQUESTCONDITION_RESULT		ProcessCheckSelectionEnd(void);
	_XQUESTCONDITION_RESULT		ProcessCheckCountPoint(void);
	_XQUESTCONDITION_RESULT		ProcessCheckMotion(void);

	_XQUESTCONDITION_RESULT		ProcessCheckAbnormalstate(void);
	_XQUESTCONDITION_RESULT		ProcessCheckBattleRecord(void);
	_XQUESTCONDITION_RESULT		ProcessCheckPartyJoinGroup(void);
	_XQUESTCONDITION_RESULT		ProcessCheckPartyValue(void);
	
	_XQUESTCONDITION_RESULT		ProcessCheckSkillState(void);
	_XQUESTCONDITION_RESULT		ProcessCheckClanServicePoint(void);
	_XQUESTCONDITION_RESULT		ProcessCheckQuestMail(void);

	_XQUESTCONDITION_RESULT		ProcessCheckBossBattleStart(void);
	_XQUESTCONDITION_RESULT		ProcessCheckBossBattleStageNumber(void);
	
	_XQUESTCONDITION_RESULT		ProcessCheckPartyMaster(void);
	_XQUESTCONDITION_RESULT		ProcessCheckPartyMemberClass(void);
	_XQUESTCONDITION_RESULT		ProcessCheckAreaPartyMember(void);
	_XQUESTCONDITION_RESULT		ProcessCheckRecordNumber(void);

	_XQUESTCONDITION_RESULT		ProcessCheckExperience(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckPlayerClass(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckInvenspace(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckPartyMasterDead(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckDestroyItem(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckStartLimitQuest(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckCompleteLimitQuest(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckPartyMemberGender(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckServiceType(void);	
	_XQUESTCONDITION_RESULT		ProcessCheckPartyOrganization(void);	
	_XQUESTCONDITION_RESULT		ProcessPartyHaveItem(void);		

	_XQUESTCONDITION_RESULT		PorcessCheckMasterPoint(void); // 사제시스템_스승기여도검사
	_XQUESTCONDITION_RESULT		PorcessCheckMasterRank(void); // 사제시스템_스승등급검사
	_XQUESTCONDITION_RESULT		PorcessCheckMasterStatus(void); // 사제시스템_상태검사
};


#endif