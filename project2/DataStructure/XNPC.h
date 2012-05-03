// XNPC.h: interface for the XNPC class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XNPC_H_
#define _XNPC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XObjectManager.h"
#include "XUser.h"
#include "XWindow_NPCChatBalloon.h"

#include <set>
#include "XModelManager.h"

//////////////////////////////////////////////////////////////////////////////////////
////  NPC define section  ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
/////  NPC SubScript Action 관련 /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define _XDEF_NPCSUBSCRIPTCLASS_TOTALCOUNT		    42		

#define _XDEF_NPCSUBSCRIPTCLASS_NONE			   -1		//
#define _XDEF_NPCSUBSCRIPTCLASS_TRADEITEM		    0		// 아이템 매매
#define _XDEF_NPCSUBSCRIPTCLASS_REPAIRITEM			1		// 무기 수리
#define _XDEF_NPCSUBSCRIPTCLASS_INFORMATION			2		// 힌트 제공
#define _XDEF_NPCSUBSCRIPTCLASS_BLACKMARKET			3		// 암거래
#define _XDEF_NPCSUBSCRIPTCLASS_SELLHERB		    4		// 약초 구입
#define _XDEF_NPCSUBSCRIPTCLASS_SELLCHARM			5		// 부적 구입
#define _XDEF_NPCSUBSCRIPTCLASS_JUDGEVALUE		    6		// 아이템 감정
#define _XDEF_NPCSUBSCRIPTCLASS_GAMBLE				7		// 미니게임 시작
#define _XDEF_NPCSUBSCRIPTCLASS_MOVE				8		// 지역 이동
#define _XDEF_NPCSUBSCRIPTCLASS_WARP				9		// 순간 이동
#define _XDEF_NPCSUBSCRIPTCLASS_SAVEITEM			10		// 창고 사용 : 아이템 저장
#define _XDEF_NPCSUBSCRIPTCLASS_EXITGAME			11		// 게임 종료
#define _XDEF_NPCSUBSCRIPTCLASS_STARTQUEST		    12		// 갯수 제한된 퀘스트 주기
#define _XDEF_NPCSUBSCRIPTCLASS_ENDSCRIPT		    13		// 스크립트 종료
#define _XDEF_NPCSUBSCRIPTCLASS_DIALOG				14		// 대화
#define _XDEF_NPCSUBSCRIPTCLASS_QA					15		// 정보
#define _XDEF_NPCSUBSCRIPTCLASS_ADDSAVEITEM			16		// 창고 추가
#define _XDEF_NPCSUBSCRIPTCLASS_LEARNSKILL			17		// 무공 전수
#define _XDEF_NPCSUBSCRIPTCLASS_REPAIRCLOTHES		18		// 의상 수선
#define _XDEF_NPCSUBSCRIPTCLASS_SAVEPOINT			19		// 부활 지점 저장
#define _XDEF_NPCSUBSCRIPTCLASS_CLANSERVICEPOINT	20		// 문파 기여도 
#define _XDEF_NPCSUBSCRIPTCLASS_HEALPLAYER			21		// 치료하기
#define _XDEF_NPCSUBSCRIPTCLASS_EXPLAINCLASS		22		// 역할설명 
#define _XDEF_NPCSUBSCRIPTCLASS_MAKEORGANIZATION	23		// 영웅단창설 
#define _XDEF_NPCSUBSCRIPTCLASS_MINISTOPEVENT		24		// 미니스톱 이벤트
#define _XDEF_NPCSUBSCRIPTCLASS_SHOWRANKQUEST		25		// 퀘스트 순위 보여주기
#define _XDEF_NPCSUBSCRIPTCLASS_PVPLIST				26		// 비무 목록 보여주기
#define _XDEF_NPCSUBSCRIPTCLASS_SOCKETEXPANSION		27		// 소켓 확장하기
#define _XDEF_NPCSUBSCRIPTCLASS_SOCKETDESTROY		28		// 소켓 분리/파괴하기
#define _XDEF_NPCSUBSCRIPTCLASS_VIEWFREEMATCHINFO	29		// 비무대회 정보보기
#define _XDEF_NPCSUBSCRIPTCLASS_COLLECTION			30		// 명품수집
#define _XDEF_NPCSUBSCRIPTCLASS_RESOURCE			31		// 자원수집
#define _XDEF_NPCSUBSCRIPTCLASS_INCHANT				32		// 무기 제련
#define _XDEF_NPCSUBSCRIPTCLASS_NPCBUFF				33		// NPC Buff
#define _XDEF_NPCSUBSCRIPTCLASS_MOVEVILLAGE			34		// 표국이동
#define _XDEF_NPCSUBSCRIPTCLASS_CASTLEINFO			35		// 장원정보 
#define _XDEF_NPCSUBSCRIPTCLASS_CASTLEATTACKLIST	36		// 장원도전자목록
#define _XDEF_NPCSUBSCRIPTCLASS_TRACING				37		// 특정인 위치 추적
#define _XDEF_NPCSUBSCRIPTCLASS_ENTERCONTEST		38		// 비무대회 참가
#define _XDEF_NPCSUBSCRIPTCLASS_VIEWCONTEST			39		// 비무대회 관람
#define _XDEF_NPCSUBSCRIPTCLASS_MIXWEAPON			40		// 무기조합
#define _XDEF_NPCSUBSCRIPTCLASS_CLANPOINTITEM		41		// 문파기여도로 아이템 구입
#define _XDEF_NPCSUBSCRIPTCLASS_MASTERNAME			42		// 사호 입력
#define _XDEF_NPCSUBSCRIPTCLASS_MNDINFO				43		// 사제 정보
#define _XDEF_NPCSUBSCRIPTCLASS_MNDPOINTPURCHASE	44		// 사제기여도로 아이템 사기



//////////////////////////////////////////////////////////////////////////////////////
/////  NPC SubScript Condition 관련  /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_NPCSUBSCRIPT_CONDITION_COUNT						5

#define ID_NPCSUBSCRIPT_CONDITION_CHECKCLANNPC				0		// 플레이어_NPC문파검사
#define ID_NPCSUBSCRIPT_CONDITION_CHECKCLANFRIENDSHIP		1		// 플레이어_NPC우호문파검사
#define ID_NPCSUBSCRIPT_CONDITION_CHECKCLANHOSTILITY		2		// 플레이어_NPC적대문파검사
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR				3		// 플레이어명성검사
#define ID_NPCSUBSCRIPT_CONDITION_NPCEXISTENCE				4		// NPC 생성 or 삭제 조건
#define ID_NPCSUBSCRIPT_CONDITION_CHECKPLAYERROLE			5		// 플레이어 역할 검사

// NPC SubScript 활성 / 비활성 ---------------------------------------------------------------
#define ID_NPCSUBSCRIPT_DISABLE						0
#define ID_NPCSUBSCRIPT_ENABLE						1

// NPC SubScript 데이터 초기화 ---------------------------------------------------------------
#define ID_NPCSUBSCRIPT_SETDATA_NONE				-1

// NPC SubScript 조건문간 계산 공식 ----------------------------------------------------------
#define ID_NPCSUBSCRIPT_CONDITIONFUNCTION_OR		0
#define ID_NPCSUBSCRIPT_CONDITIONFUNCTION_AND		1


// NPC SubScript Condition 결과
typedef enum
{
	NPC_CONDITION_RESULT_FAIL = 0,				// 실패
	NPC_CONDITION_RESULT_SUCCESS,				// 만족
}_XNPCCONDITION_RESULT;



//////////////////////////////////////////////////////////////////////////////////////
/////  NPC SubScript Condition 관련 : 문파 체크  /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_NPC_CLAN_COUNT						9

#define ID_NPC_CLAN_NANGIN						0	// 낭인
#define ID_NPC_CLAN_GAEBANG						1	// 개방
#define ID_NPC_CLAN_BEEGOONG					2	// 비궁
#define ID_NPC_CLAN_SORIM						3	// 소림
#define ID_NPC_CLAN_NOCKRIM						4	// 녹림
#define ID_NPC_CLAN_MOODANG						5	// 무당
#define ID_NPC_CLAN_MAKYO						6	// 마교
#define ID_NPC_CLAN_SEGA						7	// 세가
#define ID_NPC_CLAN_NOFIXED						8	// 이름없음


//////////////////////////////////////////////////////////////////////////////////////
///// NPC SubScript Condition 관련 : 플레이어명성검사  ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_HONOR_COUNT										6

#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_BUMIN			0	// 범인
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_KANGHO			1	// 강호
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_MULIM			2	// 무림
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_CHUNHA			3	// 천하
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_JULSE			4	// 절세
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_ILDEA			5	// 일대


// NPC 생성 or 삭제 조건 ----------------------------------------------------------
#define ID_NPCSUBSCRIPT_NPCEXISTENCE_ON				0
#define ID_NPCSUBSCRIPT_NPCEXISTENCE_OFF			1

//////////////////////////////////////////////////////////////////////////////////////
///// NPC SubScript Condition 관련 : NPC 생성 소멸 체크  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_NPCSUBSCRIPT_CONDITION_CHECKONOFF_LEVEL			0	// 레벨 체크 
#define ID_NPCSUBSCRIPT_CONDITION_CHECKONOFF_QUEST			1	// 퀘스트 체크

//////////////////////////////////////////////////////////////////////////////////////
/////  NPC Timer 관련  ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#define _XDEF_NPCTIME_CHATBALLOON				5000	// NPC 말풍선이 떠있는 시간 5초							
#define _XDEF_NPCTIME_CHATBALLOONWAITING		30000	// 다음번 말풍선이 뜰 때까지 대기 시간 30초 					
#define _XDEF_NPCTIME_SOUNDPREVGREETING			15000	// 말풍선이 없어지고 혼자말 사운드가 뜨는 시간 15초


typedef enum
{
	_XDEF_NPCMESSAGE_NORMAL = 0,
	_XDEF_NPCMESSAGE_QUEST,
	_XDEF_NPCMESSAGE_SAVEPOINT,
	_XDEF_NPCMESSAGE_ORGANIZATION,
	_XDEF_NPCMESSAGE_MINISTOPEVENT,
	_XDEF_NPCMESSAGE_HAVEQUESTLIST,
	_XDEF_NPCMESSAGE_TRACING,
	_XDEF_NPCMESSAGE_MASTERNAME //Author : 양희왕 //사호 입력
}_XNPCMESSAGETYPE;

typedef enum _XNPC_MOTION
{
	// default motion...
	_XNPCMOTION_IDLE		= 0,	// 일반 대기
	_XNPCMOTION_IDLEACTION_1,		// 대기중 동작 1
	_XNPCMOTION_IDLEACTION_2,		// 대기중 동작 2
	_XNPCMOTION_CONVERSATION,		// 대화 모드

	_XNPCMOTION_WALK,				// 걷기1
	_XNPCMOTION_WALK2,				// 걷기2
	_XNPCMOTION_RUN,				// 달리기

	_XNPCMOTION_ATTACK,				// 공격
	_XNPCMOTION_DAMAGE,				// 맞기
	_XNPCMOTION_DIE,				// 죽기

	_XNPCMOTION_STANDUP,			// 앉은 상태에서 일어서기 (IDLE->STANDUP)
	
	_XNPCMOTION_TOTALCOUNT
};

typedef struct _XNPCMotionExistTable
{
	char	motion_idle;
	char	motion_idleaction1;
	char	motion_idleaction2;
	char	motion_conversation;

	char	motion_walk;
	char	motion_walk2;
	char	motion_run;

	char	motion_attack;
	char	motion_damage;
	char	motion_die;

	char	motion_standup;

	_XNPCMotionExistTable()
	{
		motion_idle				=	-1;
		motion_idleaction1		=	-1;
		motion_idleaction2		=	-1;
		motion_conversation		=	-1;
		motion_walk				=	-1;
		motion_walk2			=	-1;
		motion_run				=	-1;
		motion_attack			=	-1;
		motion_damage			=	-1;
		motion_die				=	-1;
		motion_standup			=	-1;
	}

} _XNPCMotionExistTable;

typedef enum
{
	_XNPCROLL_NONE = 0,
	_XNPCROLL_TRADE,
	_XNPCROLL_EVENT,
	_XNPCROLL_WAREHOUSE,	// 204.06.01->oneway48 insert
	_XNPCROLL_BLACKMARKET	// 204.06.01->oneway48 insert
}_XNPCROLLTYPE;

typedef enum
{
	_XNPC_HAVEQUEST_NONE = 0,
	_XNPC_HAVEQUEST_YELLOW,
	_XNPC_HAVEQUEST_GREEN,
	_XNPC_HAVEQUEST_BLUE,
	_XNPC_HAVEQUEST_ORANGE,
	_XNPC_HAVEQUEST_WHITE,
	_XNPC_HAVEQUEST_QUESTIONYELLOW,
	_XNPC_HAVEQUEST_QUESTIONGREEN,
	_XNPC_HAVEQUEST_QUESTIONBLUE,
	_XNPC_HAVEQUEST_QUESTIONORANGE,
	_XNPC_HAVEQUEST_QUESTIONWHITE,
}_XNPCQUESTTYPE;

typedef enum
{
	_XNPC_JOBCLASS_FOOD = 0,
		_XNPC_JOBCLASS_CLOTHES,
		_XNPC_JOBCLASS_REFINING,
		_XNPC_JOBCLASS_HOTEL,
		_XNPC_JOBCLASS_WAREHOUSE,
		_XNPC_JOBCLASS_DRUGSTORE,
		_XNPC_JOBCLASS_SHOP,
		_XNPC_JOBCLASS_WEAPON,
		_XNPC_JOBCLASS_COMMONPEOPLE,
		_XNPC_JOBCLASS_GAEBANGPEOPLE,
		_XNPC_JOBCLASS_SORIMPEOPLE,
		_XNPC_JOBCLASS_NOCKRIMPEOPLE,
		_XNPC_JOBCLASS_BEEGOONGPEOPLE,
		_XNPC_JOBCLASS_MOODANGPEOPLE,
		_XNPC_JOBCLASS_MAKYOPEOPLE,
		_XNPC_JOBCLASS_BLACKMARKET,
		_XNPC_JOBCLASS_MILITARYOFFICER,
		_XNPC_JOBCLASS_HEANGSANGIN,
		_XNPC_JOBCLASS_BUFF,
		_XNPC_JOBCLASS_RESOURCE,
		_XNPC_JOBCLASS_COLLECTION,
		_XNPC_JOBCLASS_GUARD,
		_XNPC_JOBCLASS_REPAIR,
		_XNPC_JOBCLASS_CASTLECAPTAIN,	// 장주 (장원 주인)
		_XNPC_JOBCLASS_CASTLEATTACT,	// 장원안내인(공격측) 
		_XNPC_JOBCLASS_CASTLEDEFFENCE,	// 장원관리인(수비측)
		_XNPC_JOBCLASS_TRACER,			// 추적자

}_XNPCJOBCLASS;

typedef enum
{
	_XNPC_CLASS_COMMON = 0,
		_XNPC_CLASS_CLAN,
		_XNPC_CLASS_ANIMAL,
		_XNPC_CLASS_HERO,
		_XNPC_CLASS_ONOFFTYPE		
}_XNPCCLASS;

class _XWindow_NPCScript;

class _XNPCObject : public _XObject
{
public :
	_XWindow_NPCScript*	m_pNPCScriptWindowPtr;
	int					m_UniqueID;
	_XNPCJOBCLASS		m_NPCJobClass;				// NPC 직업을 나타냄 : 객점, 포목점, 대장간, 여점, 창고, 약방, 상점, 철점
	_XNPCROLLTYPE		m_NPCRollType;
	_XGROUPTYPE			m_NPCClanClass;				// NPC 종류 중 문파 선택시 문파 종류 -> 개방, 배교, 무당, 마교, 소림, 녹림
	_XNPCCLASS			m_NPCClass;					// NPC 종류 -> 일반, 문파, 동물, 영웅 , 생성or삭제
	int					m_TradeItemClass;
	LPTSTR				m_NPCName;
	LPTSTR				m_NPCRoll;
	
	BOOL				m_bCheckClanNPC;			// PC가 NPC와 같은 문파인지 아닌지를 체크
	BOOL				m_bCheckFriendship;			// PC가 NPC와 우호적인 문파인지 아닌지를 체크
	BOOL				m_bCheckHostility;			// PC가 NPC와 적대적인 문파인지 아닌지를 체크
	
	int					m_SelectedMessageIndex;
	int					m_SelectedSubScriptIndex;
	int					m_SelectedMainScriptIndex;
	int					m_PrevMainScriptIndex;

	int					m_MainScriptIndex;
	
	_XModelDescriptor	m_ModelDescriptor;
	DWORD				m_SpecialActionTimer;	
	_XList				m_Triggerlist;
	BOOL				m_InsideFrustum;
	FLOAT				m_Distance;
	FLOAT				m_DistanceFromLocalUserToNPC;
	
	_XNPC_MOTION		m_CurrentMotion;
	
	_XNPCMESSAGETYPE	m_MessageType;
	LPTSTR				m_MessageString;
	BOOL				m_ChangeMessage;
	BOOL				m_ChangeButton;
	BOOL				m_ChangeNormalMessage;
	
	BOOL				m_bStartQuestDialog;	// NPC가 퀘스트에 대한 대사를 가지고 있음.

	_XWindow_NPCChatBalloon		m_NPCChatBalloonObject;
	
	// Quest
	_XNPCQUESTTYPE		m_HaveQuestType;	
	int					m_HaveQuestID;			// NPC가 가지고 있는 퀘스트 아이디 
	
	BOOL				m_bNPCDialogMode;
	
	BOOL				m_bDrawNPCChat;
	DWORD				m_NPCChatBallonTimer;	// NPC 혼자말 유지 시간을 위한 변수 
	
	BOOL				m_bShowNPCModel;		// NPC를 Rendering 할지 안 할지를 결정 
	int					m_EventType;			//이벤트 종류 0:무영검-패밀리마트 1:봉인유저 2:퀴즈맞추기

public :
	_XNPCObject();
	virtual ~_XNPCObject();	
	
	BOOL	LoadScript( LPSTR Filename );
	BOOL	LoadScript( FILE* FilePtr );	
	
	BOOL	CheckIntersectRay( void );
	
	void	Process(void);
	void	ProcessNPCNameAndChat(void);
	void	Render(void);	
	
	void	DrawNPCNameWindow(void);
	void	DrawNPCChat(void);
	void	PlaySoundNPCPrevGreeting(void);

	void	SetMessageString(LPCTSTR string);
	void	SetMessageType(_XNPCMESSAGETYPE messagetype);
	void	SetChangeMessage(BOOL change);
	void	SetChangeButton(BOOL button);

	void	SetConversationAction(void);
	void	SetIdleAction(void);
	void	SetSurrenderAction(void);
	
	void	SetHaveQuestID(int questid)	{	m_HaveQuestID = questid;	};
	int		GetHaveQuestID(void)		{	return m_HaveQuestID;	};

	void	SetNPCDialogMode(BOOL bdialogmode )	{	m_bNPCDialogMode = bdialogmode;	};
	
	void	CreateEffectManager( void );
};


class _XNPCObjectManager : public _XModelManager
{
public:
	int					m_NPCObjectCount;
	_XNPCObject*		m_pNPCObjectArray;
	
	int					m_NPCObjectRenderCount;
	_XNPCObject**		m_pNPCObjectSortRenderArray;
		
	typedef std::set<int> ssdef_INT;
	ssdef_INT		m_ssNpcIndex;
public:
	_XNPCObjectManager();
	virtual ~_XNPCObjectManager();
	
	void			disposeList( void  );
	
	BOOL			LoadNPCObjectList( FILE* Fileptr, LPSTR Filename = NULL );
	
	void			CheckInsideFrustum( void );
	void			CheckConditionOfNPCExistence( void );
	void			Render( void );
	void			RenderNameToolTip( void );
	void			RenderNPCChatBalloon( int index );

	void			RemoveEffectInstance(int templateid);
	
	int				GetNPCIndexByNPCJob(_XNPCJOBCLASS job);		// job이 unique 할때만 valid
};



#endif // !defined(_XNPC_H_)
