/** 행동식을 표현하는 클래스.
 *
 * \author Sohyun, Park
 * \date 2003.09.02
 */

#ifndef _KUTRIGGERACTIONITEM_H_
#define _KUTRIGGERACTIONITEM_H_

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "EmperorOfDragons.h"

#pragma warning (disable: 4786)
#include <map>

using namespace std;

#define _XDEF_MAXNUM_CAMERA_ACTION		10
#define _XDEF_MAXNUM_PATHPOINT			1000

typedef struct 
{
	DWORD		starttime;
	int			shakedirection;
	int			rotationvalue;
	D3DXVECTOR3	centerpos;
	D3DXVECTOR3	destination;
	D3DXVECTOR3	direction;
	FLOAT		distance;
	FLOAT		velocity;
	int			iterate;
}_XCAMERA_ACTION;

typedef struct 
{
	BOOL		bStart;
	BOOL		bEnd;
	DWORD		dwStartTime;
	DWORD		dwElapsedTime;
	DWORD		dwTotalTime;
	FLOAT		fVelocity;
	int			nPointPosition;
	int			nShakeDirection;
	FLOAT		fDistance;
	FLOAT		fHeight;
	FLOAT		fRotationAngle;
	D3DXVECTOR3	vecCenterPosition;
	FLOAT		fTargetDistanceVelocity;
	FLOAT		fFovVelocity;
}_XCAMERA_INFO;


class _XTriggerActionItem
{
protected :
	TCHAR					m_szActionString[64];

	int						m_nActionID;
	int						m_nOwnerQuestID;
	int						m_nOwnerTriggerID;

//	BYTE					m_byOwnerTriggerClass;

	BOOL					m_bSuccessClient;
	BOOL					m_bSuccessServer;
	BOOL					m_bConfirmAll;
	
	BOOL					m_bMainQuest;
	_XQUEST_TYPE			m_QuestType;// 퀘스트 타입 ( 제룡행, 무림행, 협객행, 기연행, 무림소사 )

	map <int, _XCAMERA_ACTION> m_mapCameraValue;

public :
	_XTRIGGERDATA_ACTION	m_TDA;
	LPTSTR					m_lpszMessageString;
	
	map <int, _XCAMERA_INFO*> m_mapCameraInfo;

public :
	_XTriggerActionItem();
	~_XTriggerActionItem();

	void			ResetData(void);
	void			SetMessageString(LPCTSTR messagestring);
	void			SetActionString(LPCTSTR actionstring);

	const _XTriggerActionItem& operator =(const _XTriggerActionItem& actionitem)
	{
		if(&actionitem == this)
			return *this;

		m_TDA = actionitem.m_TDA;
		this->SetActionString(actionitem.m_szActionString);
		this->SetMessageString(actionitem.m_lpszMessageString);

		m_nActionID			= actionitem.m_nActionID;
		m_nOwnerQuestID		= actionitem.m_nOwnerQuestID;
		m_nOwnerTriggerID	= actionitem.m_nOwnerTriggerID;
		m_bSuccessClient	= actionitem.m_bSuccessClient;
		m_bSuccessServer	= actionitem.m_bSuccessServer;
		m_bConfirmAll		= actionitem.m_bConfirmAll;

		m_mapCameraValue		= actionitem.m_mapCameraValue;

		return *this;
	}

	BOOL			Load(FILE* fileptr);	
	int				LoadString(FILE* fileptr, LPTSTR& string);

	void			SetActionID(int actionid);
	int				GetActionID(void);

	void			SetOwnerQuestID(int questid);
	int				GetOwnerQuestID(void);

	void			SetOwnerTriggerID(int triggerid);
	int				GetOwnerTriggerID(void);

	void			Reset(void);
	void			ReleasePrivateData(void);

	void			SaveCameraInfo(void);
	void			RestoreCameraInfo(void);

	BOOL			NeedClientProcess(void);

/*
	void			SetOwnerTriggerClass(BYTE triggerclass);
	BYTE			GetOwnerTriggerClass(void);
*/
	
	void					SetQuestType(_XQUEST_TYPE questtype ) { m_QuestType = questtype; }
	_XQUEST_TYPE			GetQuestType(void)	{	return m_QuestType;	};
		
	void					SetMainQuest(BOOL mainquest ) { m_bMainQuest = mainquest; }
	BOOL					GetMainQuest(void);
	
	_XQUESTACTION_RESULT	Process(void);

	_XQUESTACTION_RESULT	ProcessSoundSetting(void);
	_XQUESTACTION_RESULT	ProcessMusicSetting(void);
	_XQUESTACTION_RESULT	ProcessMusicStart(void);
	_XQUESTACTION_RESULT	ProcessMusicStop(void);
	_XQUESTACTION_RESULT	ProcessEnvStart(void);
	_XQUESTACTION_RESULT	ProcessMoveCameraUsePath(void);
	_XQUESTACTION_RESULT	ProcessShakeCamera(void);
	_XQUESTACTION_RESULT	ProcessShakeCameraTopDown(_XCAMERA_INFO* camerainfo);
	_XQUESTACTION_RESULT	ProcessShakeCameraLeftRight(_XCAMERA_INFO* camerainfo);
	_XQUESTACTION_RESULT	ProcessRotateCamera(void);
	_XQUESTACTION_RESULT	ProcessRotateCameraLeft(_XCAMERA_INFO* camerainfo);
	_XQUESTACTION_RESULT	ProcessRotateCameraRight(_XCAMERA_INFO* camerainfo);
	_XQUESTACTION_RESULT	ProcessResultQuest(void);
	_XQUESTACTION_RESULT	ProcessEffectSetting(void);
	_XQUESTACTION_RESULT	ProcessTimerSetting(void);
	_XQUESTACTION_RESULT	ProcessTimerStart(void);
	_XQUESTACTION_RESULT	ProcessTimerStop(void);
	_XQUESTACTION_RESULT	ProcessSendMessage(void);
	_XQUESTACTION_RESULT	ProcessCameraSetting(void);
	_XQUESTACTION_RESULT	ProcessCameraSettingZoomIn(_XCAMERA_INFO* camerainfo);
	_XQUESTACTION_RESULT	ProcessCameraSettingZoomOut(_XCAMERA_INFO* camerainfo);
	_XQUESTACTION_RESULT	ProcessScreenEffect(void);
	_XQUESTACTION_RESULT	ProcessWeatherStart(void);
	_XQUESTACTION_RESULT	ProcessAutoStart(void);
	_XQUESTACTION_RESULT	ProcessBlockTrigger(void);
	_XQUESTACTION_RESULT	ProcessQuestProgressInfo(void);
	_XQUESTACTION_RESULT	ProcessSelection(void);
	_XQUESTACTION_RESULT	ProcessTalkingToOneself(void);
	_XQUESTACTION_RESULT	ProcessStartMiniGame(void);
	_XQUESTACTION_RESULT	ProcessMoveVelocity(void);
	_XQUESTACTION_RESULT	ProcessStartOptionCondition(void);
	_XQUESTACTION_RESULT	ProcessEndOptionCondition(void);
	_XQUESTACTION_RESULT	ProcessGiveCountPoint(void);
	_XQUESTACTION_RESULT	ProcessSpawnMonster(void);						// 몬스터 불러오기
	_XQUESTACTION_RESULT	ProcessSetMotion(void);
	_XQUESTACTION_RESULT	ProcessSetMovie(void);
	_XQUESTACTION_RESULT	ProcessSetMap(void);							// 지도 ON/OFF
	_XQUESTACTION_RESULT	ProcessMobtalkingToOneself(void);				// 몬스터 혼자말하기
	_XQUESTACTION_RESULT	ProcessStartItemCounter(void);					// 아이템 카운터 시작하기
	_XQUESTACTION_RESULT	ProcessStopCounter(void);						// 카운터 멈추기
	_XQUESTACTION_RESULT	ProcessSetSystemMessage(void);					// 시스템메세지출력
	_XQUESTACTION_RESULT	ProcessSetNPCOnOff(void);						// NPC 생성 & 삭제
	_XQUESTACTION_RESULT	ProcessSendQuestMail(void);						// 풍운방 보내기
	_XQUESTACTION_RESULT	ProcessHoldItem(void);							// 아이템장착해제불가설정
	_XQUESTACTION_RESULT	ProcessPartyMessage(void);						// 파티원_메세지출력
	_XQUESTACTION_RESULT	ProcessDisplayQuestRanking(void);				// 특정퀘스트랭킹보여주기
	_XQUESTACTION_RESULT	ProcessStartPartyItemCounter(void);				// 파티아이템카운터시작하기
	_XQUESTACTION_RESULT	ProcessSetVirtualMode(void);					// 가상공간 설정하기
	
};

#endif