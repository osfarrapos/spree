#ifndef _XOBJECTSCRIPTDEFINE_H_
#define _XOBJECTSCRIPTDEFINE_H_

//////////////////////////////////////////////////////////////////////////////////////
/////  OBJECT Trigger condition 관련 /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_OBJECT_CONDITION_COUNT								9

#define ID_OBJECT_CONDITION_CHECKCLAN							0		// 플레이어_문파비교
#define ID_OBJECT_CONDITION_CHECKITEM							1		// 플레이어_아이템검사
#define ID_OBJECT_CONDITION_CHECKGENDER							2		// 플레이어_성별검사
#define ID_OBJECT_CONDITION_CHECKQUESTCOMPLETE					3		// 퀘스트 완료 검사
#define ID_OBJECT_CONDITION_CHECKPROGRESSQUEST					4		// 진행중인 퀘스트 검사
#define ID_OBJECT_CONDITION_CHECKSTATE							5		// 플레이어_상태효과검사
#define ID_OBJECT_CONDITION_CHECKCASTLEBATTLESTAGE				6		// 장원전_단계검사
#define ID_OBJECT_CONDITION_CHECKCASTLEBATTLETHREAT				7		// 장원전_협박가능상태검사
#define ID_OBJECT_CONDITION_CHECKPLAYERROLE						8		// 플레이어_역할검사


//////////////////////////////////////////////////////////////////////////////////////
/////  OBJECT Trigger action 관련 ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_OBJECT_ACTION_COUNT									7

#define ID_OBJECT_ACTION_PLAYERMOVE								0		// 플레이어_이동
#define ID_OBJECT_ACTION_BOSSMOVE								1		// 플레이어_보스전이동
#define ID_OBJECT_ACTION_SETATTACKOBJECTSTATE					2		// 플레이어_오프젝트공격가능상태세팅
#define ID_OBJECT_ACTION_TEXTDISPLAY							3		// TEXT 출력
#define ID_OBJECT_ACTION_MATCHINFO								4		// 비무 전광판 출력
#define ID_OBJECT_ACTION_MATCHZONEENTEROREXIT					5		// 비무장 입장/퇴장
#define ID_OBJECT_ACTION_CONDITIONALATTACTSETTING				6		// 플레이어_조건별공격세팅



//////////////////////////////////////////////////////////////////////////////////////
/////  OBJECT Script Item Type	  ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_OBJECT_TYPE_GATE										0		// 출입문
#define ID_OBJECT_TYPE_BOSSGATE									1		// 보스전 출입문
#define ID_OBJECT_TYPE_TRAPLPS									2		// TRAP_LPS
#define ID_OBJECT_TYPE_MATCHINFO								3		// 비무전광판


// 트리거 조건문간 계산 공식 ----------------------------------------------------------
#define ID_OBJECT_TRIGGER_CONDITIONFUNCTION_OR			        0
#define ID_OBJECT_TRIGGER_CONDITIONFUNCTION_AND				    1

#endif