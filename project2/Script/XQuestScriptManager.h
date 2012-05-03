/** 퀘스트 스크립트를 총괄하는 클래스.
 *
 * Main Function, 진행/완료 리스트와 관련된 일을 주로 한다.
 * \author Sohyun, Park
 * \date 2003.08.26
 */

#ifndef _KUQUESTSCRIPTMANAGER_H_
#define _KUQUESTSCRIPTMANAGER_H_

#include "XQuestScriptDefine.h"
#include "XQuestScriptBaseType.h"
#include "XQuestScriptItem.h"

#pragma warning (disable: 4786)
#include <map>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

typedef struct 
{
	D3DXVECTOR3		vecCurrentPosition;
	D3DXVECTOR3		vecTargetPosition;
	FLOAT			fTargetDistance;
	FLOAT			fFov;
	FLOAT			fPitch;
	FLOAT			fYaw;
}_XCAMERA_ORIGINALINFO;

typedef struct 
{
	int		triggerid;
	int		needinven;
	int		starttime;
}_XQUEST_INVENFULL;

typedef struct 
{
	int				questid;
	int				npcid;
	FLOAT			MarkPosition_X;	
	FLOAT			MarkPosition_Y;	
	_XQUEST_TYPE	questtype; 
}_XQUEST_MARKINFO;

// 시작 조건에 아이템 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보 
typedef struct 
{
	int				questid;
	char			itemtype;   // 중분류.
	short			itemid;		// 아이템 소분류. (아이템 아이디)
}_XQUEST_ITEMINFO;

// 시작 조건에 레벨 검사가 있으면 레벨 상승시에 다시 검사하기 위한 정보 ( 미만일때는 계속 체크해야 한다.)
typedef struct 
{
	int				questid;
	int				maximumlevel;   // 시작가능한 최대 레벨 
	int				equallevel;		// 레벨이 같은 경우만 있을때 
}_XQUEST_LEVELINFO;

// 풍운방 결과 체크
typedef struct 
{
	int				questid;
	int				questmailtype;	// 0:확인 1:수락or거부
	int				resendstarttime;// 시작 조건에서 거부할때 다시 보내기 위한 시간을 저장
	int				delaytime;		// 시작 조건에서 몇분후에 보낼지를 위한 시간을 저장 
	BOOL			bYESorNO;		// TRUE:수락 FALSE:거부 
}_XQUESTMAIL_INFO;

// 아이템 고정을 위한 구조체
typedef struct 
{
	char			m_cHoldItemType;				// 대분류
	short			m_sHoldItemID;					// 소분류 (아이디)
}_XQUEST_HOLDITEM;

// 타게팅 리스트를 구성하기 위한 정보
typedef struct 
{
	int				questid;	// npc가 가지고 있는 퀘스트 
	int				npcid;		// 퀘스트ㄹ를 가지고 있는 NPC 아이디
}_XQUEST_QUESTLIST;

typedef enum _XQuest_VirtualMode
{
	_XQUEST_VIRTUALMODE_ENTER_OTHERZONE = 0,
		_XQUEST_VIRTUALMODE_ENTER_CURRENTZONE,
		_XQUEST_VIRTUALMODE_ENTER_CURRENTZONEOTHERPOSITION,
		_XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONE,
		_XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONEOTHERPOSITION,
		_XQUEST_VIRTUALMODE_LEAVE_CURRENTZONE,
		_XQUEST_VIRTUALMODE_LEAVE_CURRENTZONEOTHERPOSITION,
		_XQUEST_VIRTUALMODE_LEAVE_OTHERZONEOTHERPOSITION,
}_XQUEST_VIRTUALMODE;
/** Quest script manager.
 * 퀘스트 스트립트를 총괄하는 클래스
 * \author Sohyun, Park
 * \date 2003.08.26 (revision)
 */
class _XQuestScriptManager
{
protected :
	
	int							m_nQuestScriptCount;			// Quest script 개수
	int							m_nProgressQuestCount;			// 진행중인 퀘스트 개수
	int							m_nCheckQuestID;				// NPC script를 통해서 들어온 실행해야 할 퀘스트 ID
	
	BOOL						m_bQuestCameraMode;				// Quest camera mode - TRUE이면 카메라 자동 이동중
	BOOL						m_bCheckQuest;					// NPC script를 통해서 들어온 것인지를 알려주는 변수
	BOOL						m_bQuestWideView;				// Wide 화면 처리

public :
	
	DWORD						m_QuestTimerStartTime;
	DWORD						m_QuestTimerTimeLimit;	
	DWORD						m_QuestStartConditionTimer;		// 퀘스트 시작 조건 영역검사를 위한 딜레이 	
	DWORD						m_QuestSystemMessageStartTime;
	
	LPTSTR						m_lpszQuestSystemMessage;

	int							m_CurrentMainQuestID;			// 제룡행 퀘스트일 때 현재 퀘스트 번호
	int							m_NextMainQuestID;				// 제룡행 퀘스트일 때 다음 퀘스트 번호
	int							m_TimerStartQuestID;
	int							m_indexSelectNPCTargettingQuest;// 한 NPC에 여러 퀘스트의 타게팅이 걸려있을때 선택한 퀘스트 번호
	int							m_nVirtualZoneStartPositionX;	// 가상공간에서의 시작위치를 정보를 저장한다.
	int							m_nVirtualZoneStartPositionZ;	// 가상공간에서의 시작위치를 정보를 저장한다.
	int							m_nRealZoneStartPositionX;		// 가상공간에서 되돌아갈 위치를 저장해둔다.
	int							m_nRealZoneStartPositionZ;		// 가상공간에서 되돌아갈 위치를 저장해둔다.
	int							m_indexVirtualZone;				// 가상공간 지역인덱스를 저장한다.
	int							m_indexRealZone;				// 가상공간으로 가기 전에 실제공간의 지역인덱스를 저장한다.
	int							m_ProgressSubQuestID[5];		// 제룡행이 아닌 퀘스트일 때 진행중인 퀘스트 번호

	BOOL						m_bStartMessage;				// 대화를 시작 했다는 플래그 : 선택지는 대화가 끝난후에 보이기 위해서 
	BOOL						m_bStartQuestTimer;				// 퀘스트 타이머 체크 시작을 알수 있는 플래그
	BOOL						m_bStartQuestCounterMessage;	// 퀘스트 카운터 메세지 출력 시작을 알수 있는 플래그
	BOOL						m_bQuestStartConditionCheck;
	BOOL						m_bQuestValidCheck;				// 퀘스트가 유효하지 않아서 따로 처리될 때를 알 수 있는 변수
	BOOL						m_bProgressMainQuest;			// 제룡행 퀘스트의 Process를 진행한다.
	BOOL						m_bReceiveQuestData;			// 서버에서 퀘스트 데이타를 전부 받았는지를 알 수 있는 변수
	BOOL						m_bCancelProcess;				// 퀘스트 취소 후에 퀘스트 리스트를 다시 재정렬할 때 퀘스트 동작을 막는 변수
	
	_XQUEST_VIRTUALMODE			m_VirtualMode;					// 가상공간으로의 입출입에 관련된 모드를 저장한다.

	_XCAMERA_ORIGINALINFO		m_OriginalCameraInfo;
		
	// 시작 조건에 따른 퀘스트 아이디를 따로 가지고 검사한다. 
	list <int>					m_listQuestStartTypeNPCTargetting;
	list <int>					m_listQuestStartTypeObjectClick;
	list <int>					m_listrQuestStartTypeUsingItem;
	list <int>					m_listQuestStartTypeWearItem;
	list <int>					m_listQuestStartTypeDestroyItem;
	list <int>					m_listQuestStartTypeLevel;
	list <int>					m_listQuestStartTypeZone;
	
	list <int>					m_listQuestNoSaveType;		// 진행중인 동행 무림소사와 무림소사의 퀘스트 번호를 저장하는 리스트 

	list <int>					m_listCheckInParty;		// 시작 조건에 동행 가입 검사가 있으면 동행 탈퇴시에 다시 검사하기
	list <_XQUEST_ITEMINFO>		m_listCheckItem;		// 시작 조건에 아이템 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보
	list <_XQUEST_ITEMINFO>		m_listCheckWearItem;	// 시작 조건에 아이템 착용 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 정보	
	list <_XQUEST_LEVELINFO>	m_listCheckLevel;		// 시작 조건에 레벨 검사가 있으면 레벨 상승시에 다시 검사하기 위한 정보 ( 미만일때는 계속 체크해야 한다.)
	list <_XQUEST_ITEMINFO>		m_listCheckDestroyItem;	// 아이템 파괴 검사
	list <_XQUEST_HOLDITEM>		m_listHoldItem;			// 아이템 고정을 위한 구조체

	list <_XQUEST_MARKINFO>		m_listQuestMark;		// 퀘스트 마크의 타입이 영역일때 직접 좌표값을 저장 
	list <_XQUEST_MARKINFO>		m_listHintMark;			// 힌트 마크의 타입이 영역일때 직접 좌표값을 저장 	
	
	list <_XQUESTMAIL_INFO>		m_listQuestMailInfo;	// 풍운방 결과 체크 	
	list <_XQUESTMAIL_INFO>		m_listQuestMailBuffer;	// 풍운방 보내기 시작 조건에서 거부할때 30분후에 다시 보내기 위한 시간을 저장	

	list <_XQUEST_QUESTLIST>	m_listNPCTargettingQuest;	// 한 NPC에 여러 퀘스트의 타게팅이 걸려있을때 퀘스트 번호를 저장하는 리스트	

	/// Quest script list
	map <int, _XQuestScriptItem*>		m_mapQuestScriptList; // 제룡행 퀘스트
	map <int, _XQuestScriptItem*>		m_mapSubQuestScriptList;// 제룡행 이외의 퀘스트

	map <int, _XQUEST_COMPLETE_RESULT>	m_mapCompleteQuestList;/// 퀘스트 완료 리스트 <Quest id, Quest 결과>
	map <int, _XQUEST_INVENFULL>		m_mapRewordQuestList;/// 보상을 받지 못한 퀘스트 리스트
	  	
public:
	_XQuestScriptManager();
	~_XQuestScriptManager();

	// Release
	BOOL			Release(void);
	void			ReleasePrivateData(void);

	// Quest script load
	BOOL			LoadQuestScript(LPCTSTR filename);
	BOOL			LoadQuestScript(FILE* fileptr);

	// Main Function
	void			Process(void);

	// 진행 리스트 관련 Utility
	DWORD			GetCompareValue(int comparecount);
	int				GetCompareCount(DWORD comparevalue);

	// 현재 수행중인 퀘스트 개수가 동시 실행 가능 개수보다 작은지 체크
	BOOL			CheckProgressQuestCount(void);
	// m_nProgressQuestCount 증가
	void			IncreaseProgressQuestCount(void);
	// m_nProgressQuestCount 감소
	void			DecreaseProgressQuestCount(void);
	// m_nProgressQuestCount return
	int				GetProgressQuestCount(void);

	// 완료 리스트에 삽입
	void			InsertQuestCompleteList(_XQuestScriptItem* currentquest);
	void			InsertQuestCompleteList(int questid, _XQUEST_COMPLETE_RESULT result);
	// 완료 리스트에서 퀘스트 결과 값 알아내기
	_XQUEST_COMPLETE_RESULT		FindQuestCompleteResult(int questid);

	// 퀘스트가 완료 되었는지 여부 판별
	BOOL			CheckCompleteQuest(int questid);
	TCHAR*			GetQuestName(int questid);

	// 카메라 모드 setting
	BOOL			GetQuestCameraMode(void);
	void			SetQuestCameraMode(BOOL mode);

	// 퀘스트 전체 Reset
	void			ResetQuest(void);
	// 특정 퀘스트 Reset
	void			ResetQuest(int questid);
	// 특정 트리거 Reset
	void			ResetQuest(int questid, int triggerid );

	// 서버에서 받은 정보로 진행/완료 리스트 만들기
	void			QuestSuccessSetting(int questid); 
	void			QuestFailSetting(int questid);
	void			QuestRunningSetting(int questid, BYTE treestep);
	void			QuestRunningTimerSetting(int questid, int time, int treenumber, int triggertimer);
	BOOL			QuestCancleHistorySetting(int questid );
	BOOL			QuestHistorySetting(int questid, BYTE treestep);

	// 보상을 받지 못한 퀘스트 리스트에 삽입	
	void			InsertRewordQuestList(int questid, int treenumber, int needinven);
	// 삭제
	void			DeleteRewordQuestList(int questid);

	void			SetCheckQuestID(int questid);
	int				GetCheckQuestID(void);

	void			SetCheckQuest(BOOL check);
	BOOL			GetCheckQuest(void);

	void			ClearOriginalCameraInfo(void);

//	void			ProcessTest(int questid, int triggerid, int actionid);
//	void			RestoreTest(void);


	// Quest Wide view
	void			SetQuestWideView(BOOL bWide);
	BOOL			GetQuestWideView(void);

	// 대사 처리
	void			SetStartMessage(int questid);
	void			SetProgressMessage(int questid, int npcid, LPCTSTR string);
	void			SetFailMessage(int questid);

	BOOL			LoadString(FILE* fileptr, LPTSTR& string);
	
	// 퀘스트 시작 조건별로 검사하는 함수 
	void			QuestStartConditionNPCTargettingMode();
	void			QuestStartConditionObjectClickMode();
	void			QuestStartConditionUsingItemMode();
	void			QuestStartConditionWearItemMode();
	void			QuestStartConditionDestroyItemMode();
	void			QuestStartConditionLevelMode();
	void			QuestStartConditionZoneMode();

	void			QuestStartConditionSort();
	void			SetQuestStartCondition( int questid );
	void			SetMainQuestStartCondition( int questid );
	
	BOOL			CheckItemCounter(int questid, int itemtype, int itemid);		// 특정 퀘스트의 아이템 카운터를 계산하는 함수
	BOOL			CheckPartyItemCounter(int questid, int itemtype, int itemid);	// 특정 퀘스트의 파티 아이템 카운터를 계산하는 함수
	
	BOOL			CheckAllItemCounter(int itemtype, int itemid);			// 실행 중인 모든 퀘스트의 아이템 카운터를 계산한다.	

	void			ResetQuestStartCondition( void );
	void			SetQuestStartConditionAtVirtualSpace( void );			// 가상공간에 들어갈 때 퀘스트 시작조건에 관한 처리를 다시한다.

	// 시작 조건에 아이템 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 함수 
	void			RecheckHaveItemTypeQuestStartCondition( char itemtype, short itemid ); 
	// 시작 조건에 아이템 장착 검사가 있으면 아이템이 없어질 때 다시 검사하기 위한 함수 
	void			RecheckWearItemTypeQuestStartCondition( char itemtype, short itemid );
	// 시작 조건에 레벨 검사가 있으면 레벨 상승시에 다시 검사하기 위한 정보 ( 미만일때는 계속 체크해야 한다.)
	void			RecheckLevelTypeQuestStartCondition( void );
	// 시작 조건에 동행가입 검사가 있으면 동행 탈퇴에 따라 다시 검사하기 위한 정보 
	void			RecheckPartyTypeQuestStartCondition( void );

	// 퀘스트가 시작되면 시작한 퀘스트에 관련된 리스트를 지운다.
	void			DeleteRecheckingListAboutQuestStartCondition( int questid );
	
	int				CheckPracticableQuestNode( int questid, BYTE treestep, BOOL& bneeditemremove ); //실행가능한 노드를 반환한다.
	BYTE			GetQuestCurrentTreeNodeClass( int questid, BYTE treestep ); //현재 노드가 분기인지 끝인지 정상인지를 알아낸다.
	BOOL			CheckHaveQuestJumpAction( int questid, BYTE treestep ); //현재 노드에 Goto가 있는지 체크한다.
	BOOL			CheckHaveQuestAutoNode( int questid ); //현재 노드에 Goto가 있는지 체크한다.
	
	// 파티 퀘스트가 취소되면 파티퀘스트를 지우고 다시 퀘스트 시작조건에 넣는다.
	void			DeletePartyQuest( void );
};

#endif