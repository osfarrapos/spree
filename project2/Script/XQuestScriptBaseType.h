//
//	KUQuestScriptBaseType.h
//
// Created Date : 2003.01.09 
// Author : Sohyun Park (kukuri), Iljun Park
// Desc : 퀘스트 스크립트 condition/action structure
///////////////////////////////////////////////////////////////////

#ifndef _KUQUESTSCRIPTBASETYPE_H_
#define _KUQUESTSCRIPTBASETYPE_H_

#define _XQUEST_MAX_INVENTORYSLOT			5

#include "XKernel.h"

// Trigger Condition Structure
typedef struct 
{
	BOOL				m_enable;				// 조건식 활성화
	int					m_conditionclass;		// 트리거 조건식 클래스

	int					m_compareclass1 : 8;	// 계산 관련 클래스
	int					m_compareclass2 : 8;
	int					m_compareclass3 : 8;
	int					m_compareclass4 : 8;

	int					m_numericvalue1;			// 조건식 관련 수치
	int					m_numericvalue2;
	int					m_numericvalue3;
	int					m_numericvalue4;
}_XTRIGGERDATA_CONDITION;


// trigger action structure
typedef struct 
{
	BOOL				m_enable;			// 행동식 활성화 
	int					m_actionclass;		// 액션 종류

	int					m_actiondata1 : 8;	// 행동식 데이타
	int					m_actiondata2 : 8;
	int					m_actiondata3 : 8;
	int					m_actiondata4 : 8;

	int					m_numericvalue1;		// 행동식 관련 수치
	int					m_numericvalue2;
	int					m_numericvalue3;
	int					m_numericvalue4;
	int					m_numericvalue5;
}_XTRIGGERDATA_ACTION;


// 퀘스트의 실행 결과 값
typedef enum
{
	QUEST_COMPLETE_RESULT_NONE = 0,		// 퀘스트 처리 되지 않음
	QUEST_COMPLETE_RESULT_SOLVE,			// 퀘스트 해결
	QUEST_COMPLETE_RESULT_UNSOLVE,			// 퀘스트 미해결 (퀘스트 reset)
	QUEST_COMPLETE_RESULT_MISS				// 퀘스트 실패 (다시 할수없음)
}_XQUEST_COMPLETE_RESULT;


// 퀘스트 결과
typedef enum
{
	QUEST_QUEST_RESULT_SUCCESS = 0,		// 성공
	QUEST_QUEST_RESULT_PROCESS,			// 퀘스트 처리중 (트리거 처리중)
	
}_XQUESTITEM_RESULT;


// 시작 조건 체크 결과
typedef enum
{
	QUEST_STARTCONDITION_RESULT_NOTIFYSERVER = 0,	// 시작 조건 만족, 퀘스트 시작하기를 원함
	QUEST_STARTCONDITION_RESULT_NOTIFYUSER,			// 시작 조건 만족, 유저에게 알려줌
	QUEST_STARTCONDITION_RESULT_FAIL,				// 시작 조건 만족하지 않음
	QUEST_STARTCONDITION_RESULT_ALREADY				// 시작 조건이 이미 만족됨
}_XQUESTSTARTCONDITION_RESULT;

// condition 결과
typedef enum
{
	QUEST_CONDITION_RESULT_FAIL = 0,			// 실패
	QUEST_CONDITION_RESULT_SUCCESS,				// 만족
	QUEST_CONDITION_RESULT_PROCESS,				// 진행중 (진행 리스트)
	QUEST_CONDITION_RESULT_ALREADYPROCESS,		// 완료 (진행 리스트)
	QUEST_CONDITION_RESULT_ERROR				// 에러
}_XQUESTCONDITION_RESULT;


// action 결과
typedef enum
{
	QUEST_ACTION_RESULT_FAIL = 0,				// 실패
	QUEST_ACTION_RESULT_SUCCESS,				// 실행 끝
	QUEST_ACTION_RESULT_PROCESS,				// 실행 중 혹은 진행중 (진행리스트가 아직 0)
	QUEST_ACTION_RESULT_ALREADYPROCESS,			// 완료 (진행리스트 1)
	QUEST_ACTION_RESULT_ERROR					// 에러
}_XQUESTACTION_RESULT;

// trigger 결과
typedef enum
{
	QUEST_TRIGGER_RESULT_WAITSERVER = 0,		// 서버로 request를 보냈음 (조건을 다 만족해서 인증을 기다림)
	QUEST_TRIGGER_RESULT_SUCCESS,				// 트리거 실행 완료
	QUEST_TRIGGER_RESULT_FAIL,					// 트리거 실행 완료( 분기시에 조건을 만족하지 않았을 때 행동식을 실행하고 리턴하는 값 )
	QUEST_TRIGGER_RESULT_PROCESS,				// 진행중 (조건을 다 만족하지 않았거나 액션을 다 실행하지 않았거나)
	QUEST_TRIGGER_RESULT_ERROR,					// 에러
	QUEST_TRIGGER_RESULT_BLOCK					// 분기 트리거로 인해 점프한 트리거
}_XQUESTTRIGGER_RESULT;


// 트리거 종류
typedef enum
{
	TRIGGER_SUCCESS = 0,
	TRIGGER_FAIL,
}_XTRIGGER_CLASS;

// action 종류
typedef enum
{
	ACTION_NORMAL = 0,
	ACTION_EXCEPTED
}_XACTION_CLASS;


// Quest inventory
typedef struct 
{
	char			cItemType;
	unsigned short	usItemID;
	unsigned int	unCount;
}_XQUEST_ITEM;

// other server quest inventory
typedef struct  
{
	_XQUEST_ITEM	questinven[5];
	int				triggertimer;
}_XQUEST_OTHERINVEN;

// Quest Type
typedef enum _XQuest_Type
{
	_XQUEST_TYPE_NONE = 0,
		_XQUEST_TYPE_1,
		_XQUEST_TYPE_2,
		_XQUEST_TYPE_3,
		_XQUEST_TYPE_4,
		_XQUEST_TYPE_5
}_XQUEST_TYPE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////         Functional Object 관련        //////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                  
// condition 결과
typedef enum
{
	FUNCTIONALOBJECT_CONDITION_RESULT_FAIL = 0,			// 실패
		FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS		// 만족
}_XFUNCTIONALOBJECT_CONDITION_RESULT;


// action 결과
typedef enum
{
	FUNCTIONALOBJECT_ACTION_RESULT_FAIL = 0,				// 실패
		FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS,			// 실행 끝
}_XFUNCTIONALOBJECT_ACTION_RESULT;


#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
extern int	g_LoadQuestScriptText_LastIndex;
extern int*	g_LoadQuestScriptText_IndexTable;
#endif

#endif