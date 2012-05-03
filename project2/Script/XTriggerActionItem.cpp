/** 행동식을 표현하는 클래스.
 *
 * \author Sohyun, Park
 * \date 2003.09.02
 */

#include "stdafx.h"

#pragma warning (disable: 4786)

#include <stdio.h>
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XTriggerActionItem.h"
#include "XWindow_NPCScript.h"
#include "XWindow_MaximumMap.h"
#include "Xwindow_TrainingMed.h"
#include "Xwindow_QuestMail.h"

#include "XWindowObjectDefine.h"

#include "XSR_STRINGHEADER.h"

extern  D3DCOLOR g_ChattingStringColor[16];

extern void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo );

_XTriggerActionItem::_XTriggerActionItem()
{
	
	m_bMainQuest		= FALSE;
	m_QuestType			= _XQUEST_TYPE_NONE;
	m_lpszMessageString = NULL;
	ResetData();
}

_XTriggerActionItem::~_XTriggerActionItem()
{
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
#else
	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		//m_lpszMessageString = NULL;
	}
#endif
}

void _XTriggerActionItem::ResetData(void)
{
	m_TDA.m_enable		= ID_TRIGGER_DISABLE;

	m_TDA.m_actionclass	= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata1 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata2 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata3 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata4 = ID_TRIGGER_SETDATA_NONE;

	m_TDA.m_numericvalue1 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue2 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue3 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue4 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue5 = ID_TRIGGER_SETDATA_NONE;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	m_lpszMessageString = NULL;
#else
	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		m_lpszMessageString = NULL;
	}
#endif


	memset(m_szActionString, 0, sizeof(TCHAR)*64);

	m_nOwnerQuestID		= -1;
	m_nOwnerTriggerID	= -1;

	m_bSuccessClient	= FALSE;
	m_bSuccessServer	= FALSE;
	m_bConfirmAll		= FALSE;
}

void _XTriggerActionItem::SetMessageString(LPCTSTR messagestring)
{

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	m_lpszMessageString = (LPTSTR)messagestring;
#else
	if(!messagestring)
		return;

	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		m_lpszMessageString = NULL;
	}

	int strlength = strlen(messagestring);
	if(strlength > 0)
	{
		m_lpszMessageString = new TCHAR[strlength + 1];
		m_lpszMessageString[strlength] = 0;
		strcpy(m_lpszMessageString, messagestring);
	}
#endif
}

void _XTriggerActionItem::SetActionString(LPCTSTR actionstring)
{
	memset(m_szActionString, 0, sizeof(TCHAR)*64);

	if(!actionstring)
		return;

	int strlength = strlen(actionstring);
	if(strlength < 64)
	{
		strcpy(m_szActionString, actionstring);
	}
	else
	{
		strncpy(m_szActionString, actionstring, 63);
	}
}

BOOL _XTriggerActionItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	int actionid;

	if(fread(&actionid, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(m_szActionString, sizeof(TCHAR)*64, 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_TDA, sizeof(_XTRIGGERDATA_ACTION), 1, fileptr) < 1)
		return FALSE;

	int readstringlength = 0;
	if( (readstringlength = LoadString(fileptr, m_lpszMessageString)) == -1 )
		return FALSE;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszMessageString = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest action message string\t...\t%d\t%d\t\t%s", actionid, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszMessageString );
	}
#endif


	return TRUE;
}

int _XTriggerActionItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;

	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return -1;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	fseek( fileptr, sizeof(TCHAR)*strlength, SEEK_CUR );
	string = NULL;
#else	
	if(string)
	{
		delete[] string;
		string = NULL;
	}    

	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
			return -1;

		memset(string, 0, sizeof(TCHAR)*(strlength + 1));

		if(fread(string, strlength, 1, fileptr) < 1)
			return -1;
	}
#endif

	return strlength;
}

void _XTriggerActionItem::SetActionID(int actionid)
{
	m_nActionID = actionid;
}

int _XTriggerActionItem::GetActionID(void)
{
	return m_nActionID;
}

void _XTriggerActionItem::SetOwnerQuestID(int questid)
{
	m_nOwnerQuestID = questid;
}

int _XTriggerActionItem::GetOwnerQuestID(void)
{
	return m_nOwnerQuestID;
}

void _XTriggerActionItem::SetOwnerTriggerID(int triggerid)
{
	m_nOwnerTriggerID = triggerid;
}

int _XTriggerActionItem::GetOwnerTriggerID(void)
{
	return m_nOwnerTriggerID;
}

void _XTriggerActionItem::Reset(void)
{
	m_mapCameraValue.clear();
}

void _XTriggerActionItem::SaveCameraInfo(void)
{
	g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition = g_LodTerrain.m_3PCamera.m_CameraPosition;
	g_QuestScriptManager.m_OriginalCameraInfo.vecTargetPosition = g_LodTerrain.m_3PCamera.m_TargetPosition;
	g_QuestScriptManager.m_OriginalCameraInfo.fTargetDistance = g_LodTerrain.m_3PCamera.m_TargetDistance;
	g_QuestScriptManager.m_OriginalCameraInfo.fFov = g_LodTerrain.m_3PCamera.mp_fFov;
	g_QuestScriptManager.m_OriginalCameraInfo.fPitch = g_LodTerrain.m_3PCamera.mp_fPitch;
	g_QuestScriptManager.m_OriginalCameraInfo.fYaw = g_LodTerrain.m_3PCamera.mp_fYaw;
}

void _XTriggerActionItem::RestoreCameraInfo(void)
{
	g_LodTerrain.m_3PCamera.SetCameraPosition(g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition);
	g_LodTerrain.m_3PCamera.SetTargetPosition(g_QuestScriptManager.m_OriginalCameraInfo.vecTargetPosition);
	g_LodTerrain.m_3PCamera.SetDistance(g_QuestScriptManager.m_OriginalCameraInfo.fTargetDistance);
	g_LodTerrain.m_3PCamera.mp_fFov = g_QuestScriptManager.m_OriginalCameraInfo.fFov;
	g_LodTerrain.m_3PCamera.mp_fPitch = g_QuestScriptManager.m_OriginalCameraInfo.fPitch;
	g_LodTerrain.m_3PCamera.mp_fYaw = g_QuestScriptManager.m_OriginalCameraInfo.fYaw;

	g_QuestScriptManager.ClearOriginalCameraInfo();
}

/*
void _XTriggerActionItem::SetOwnerTriggerClass(BYTE triggerclass)
{
	m_byOwnerTriggerClass = triggerclass;
}

BYTE _XTriggerActionItem::GetOwnerTriggerClass(void)
{
	return m_byOwnerTriggerClass;
}
*/

BOOL _XTriggerActionItem::NeedClientProcess(void)
{
	switch(m_TDA.m_actionclass)
	{
	case ID_ACTION_ADDSKILL :							// 플레이어_무공설정
	case ID_ACTION_DELETESKILL :						// 플레이어_무공삭제
	case ID_ACTION_GIVEITEM :							// 플레이어_아이템주기
	case ID_ACTION_TAKEITEM :							// 플레이어_아이템삭제
	case ID_ACTION_GIVEEXPERIENCE :						// 플레이어_경험치 주기 명성 악명
	case ID_ACTION_MOVE :								// 플레이어_이동
	case ID_ACTION_SETSTATUS :							// 플레이어_스탯설정
	case ID_ACTION_JOINGROUP :							// 문파가입
	case ID_ACTION_GOTOTRIGGER :						// 해당 트리거로 이동
	case ID_ACTION_DEFAULT :							// 아무행동도 안하는 트리거
	case ID_ACTION_SETWEARITEM :						// 아이템 착용 세팅
//	case ID_ACTION_SETMOBSPAWN :						// 몬스터 불러오기 
	case ID_ACTION_SETOBJECTSPAWN :						// 오브젝트 불러오기
	case ID_ACTION_GIVENICKNAME :						// 별호주기
	case ID_ACTION_SETSKILLEXP :						// 플레이어_무공숙련도(주기,빼앗기)
	case ID_ACTION_SETSKILLSTATE :						// 플레이어_무공상태(사용중지,사용가능)
	case ID_ACTION_SETABNORMALSTATE :					// 플레이어_상태이상설정(주기, 해제)
	case ID_ACTION_GIVESETTINGITEM:						// 미리 설정되어있는 아이템 주기
	case ID_ACTION_RESETVALUE:							// 스텟 초기화
	case ID_ACTION_SETPLAYERCLASS:						// 플레이어_클래스지정(주기,빼앗기)
	case ID_ACTION_SETCLANSERVICEPOINT:					// 플레이어_문파기여도(주기,빼앗기)
	case ID_ACTION_SETCONDITIONEFFECT:					// 플레이어_상태효과부여		
	case ID_ACTION_PARTYGIVEITEM:						// 파티원_아이템주기or뺏기(행수,행원,MVP)
	case ID_ACTION_PARTYGIVEEXPERIENCE:					// 파티원_스텟주기or뺏기(행수,행원,MVP)   
	case ID_ACTION_PARTYSETABNORMALSTATE:				// 파티원_상태이상주기		
	case ID_ACTION_PARTYGIVEEQUALLYREWARD:				// 파티원_균등한보상물주기(보상/파티원수)
	case ID_ACTION_INSERTRANK:							// 순위결정하기		
	case ID_ACTION_CHANGESERVERGROUP:					// 서버군상태변경(봉문해제)
	case ID_ACTION_GIVEEXPERIENCEBYPERCENTAGE:			// 플레이어_n%경험치주기
	case ID_ACTION_GIVECLASSITEM:						// 역할별 아이템주기(개인,동행)
	case ID_ACTION_PARTYGIVENICKNAME:					// 파티원_별호주기
	case ID_ACTION_PARTYTAKEITEM:						// 파티원_아이템뺏기(파티전체)
	case ID_ACTION_GIVEEXPERIENCEBYLEVEL:				// 플레이어_레벨%경험치주기
	case ID_ACTION_SETWIDECONDITIONEFFECT:				// 플레이어_광역상태효과부여
	case ID_ACTION_SETCLASSGRADE :						// 직책 등급 설정
	case ID_ACTION_MND_GIVEMASTER_RANK :				// 사제시스템 스승등급설정
	case ID_ACTION_MND_SETMASTERPOINT :					// 사제시스템 스승기여도(주기,뺏기)
		return FALSE;		
		
	case ID_ACTION_SOUNDSTART :
	case ID_ACTION_MUSICSTART :
	case ID_ACTION_SETENV :
	case ID_ACTION_EFFECTSTART :
	case ID_ACTION_TIMERSTART :
	case ID_ACTION_SENDMESSAGE :
	case ID_ACTION_AUTOSTART :
	case ID_ACTION_RESULTQUEST :
	case ID_ACTION_MOVECAMERA :
	case ID_ACTION_ROTATECAMERA :
	case ID_ACTION_CAMERASETTING :
	case ID_ACTION_SHAKECAMERA :
	case ID_ACTION_SCREENEFFECT :
	case ID_ACTION_SETWEATHER :
	case ID_ACTION_BLOCKTRIGGER :
	case ID_ACTION_QUESTPROGRESSINFO :
	case ID_ACTION_SELECTION :
	case ID_ACTION_STARTMINIGAME :
	case ID_ACTION_TALKINGTOONESELF :
	case ID_ACTION_SETMOVEVELOCITY :
	case ID_ACTION_STARTOPTIONCONDITION :
	case ID_ACTION_ENDOPTIONCONDITION :
	case ID_ACTION_GIVECOUNTPOINT :				// 카운터 포인트 주기 
	case ID_ACTION_SETMOBSPAWN :				// 몬스터 불러오기 
	case ID_ACTION_SETMOTION :					// 강제로 행동 시키기
	case ID_ACTION_SETMOVIE :					// 영상 보여주기
	case ID_ACTION_SETMAP :						// 지도 ON/OFF	
	case ID_ACTION_MONSTERTALKINGTOONESELF :	// 몬스터 혼자말하기 
	case ID_ACTION_SETITMECOUNTERSTART :		// 아이템 카운터 시작하기	
	case ID_ACTION_STOPCOUNTER :				// 카운터 멈추기 
	case ID_ACTION_SETSYSTEMMESSAGE :			// 시스템메세지출력 
	case ID_ACTION_SETNPCONOFF :				// NPC 생성 & 삭제 
	case ID_ACTION_SENDQUESTEMAIL :				// 풍운방 보내기 
	case ID_ACTION_HOLDITEM:					// 아이템장착해제불가설정
	case ID_ACTION_PARTYMESSAGE:				// 파티원_메세지출력
	case ID_ACTION_DISPLAYQUESTRANKING:			// 특정퀘스트랭킹보여주기
	case ID_ACTION_STARTPARTYITMECOUNTER:		// 파티아이템카운터시작하기
	case ID_ACTION_SETVIRTUALMODE:				// 가상공간설정하기
		return TRUE;				
	}
	return FALSE;
}

_XQUESTACTION_RESULT _XTriggerActionItem::Process(void)
{
	if(NeedClientProcess())
	{
		switch(m_TDA.m_actionclass)
		{
		case ID_ACTION_SOUNDSTART :
			return ProcessSoundSetting();
			
		case ID_ACTION_MUSICSTART :
			return ProcessMusicSetting();

		case ID_ACTION_RESULTQUEST :
			return ProcessResultQuest();
			
		case ID_ACTION_MOVECAMERA :
			return ProcessMoveCameraUsePath();
			
		case ID_ACTION_SHAKECAMERA :
			return ProcessShakeCamera();
			
		case ID_ACTION_ROTATECAMERA :
			return ProcessRotateCamera();
			
		case ID_ACTION_EFFECTSTART :
			return ProcessEffectSetting();
			
		case ID_ACTION_TIMERSTART :
			return ProcessTimerSetting();
			
		case ID_ACTION_SENDMESSAGE :
			return ProcessSendMessage();
			
		case ID_ACTION_CAMERASETTING :
			return ProcessCameraSetting();
			
		case ID_ACTION_SCREENEFFECT :
			return ProcessScreenEffect();
			
		case ID_ACTION_SETENV :
			return ProcessEnvStart();
			
		case ID_ACTION_SETWEATHER :
			return ProcessWeatherStart();

		case ID_ACTION_AUTOSTART :
			return ProcessAutoStart();

		case ID_ACTION_BLOCKTRIGGER :
			return ProcessBlockTrigger();
			
		case ID_ACTION_QUESTPROGRESSINFO :
			return ProcessQuestProgressInfo();

		case ID_ACTION_SELECTION :			
			return ProcessSelection();
			
		case ID_ACTION_STARTMINIGAME :			
			return ProcessStartMiniGame();
			
		case ID_ACTION_TALKINGTOONESELF :			
			return ProcessTalkingToOneself();

		case ID_ACTION_SETMOVEVELOCITY :		
			return ProcessMoveVelocity();			

		case ID_ACTION_STARTOPTIONCONDITION :		
			return ProcessStartOptionCondition();
			
		case ID_ACTION_ENDOPTIONCONDITION :		
			return ProcessEndOptionCondition();
			
		case ID_ACTION_GIVECOUNTPOINT :		
			return ProcessGiveCountPoint();			
			
		case ID_ACTION_SETMOBSPAWN :		
			return ProcessSpawnMonster();			
			
		case ID_ACTION_SETMOTION :		
			return ProcessSetMotion();
			
		case ID_ACTION_SETMOVIE :		
			return ProcessSetMovie();
						
		case ID_ACTION_SETMAP :// 지도 ON/OFF	
			return ProcessSetMap();
	
		case ID_ACTION_MONSTERTALKINGTOONESELF :// 몬스터 혼자말하기
			return ProcessMobtalkingToOneself();
			
		case ID_ACTION_SETITMECOUNTERSTART :// 아이템 카운터 시작하기	
			return ProcessStartItemCounter();

		case ID_ACTION_STOPCOUNTER :// 카운터 멈추기 
			return ProcessStopCounter();
			
		case ID_ACTION_SETSYSTEMMESSAGE :// 시스템메세지출력 
			return ProcessSetSystemMessage();
			
		case ID_ACTION_SETNPCONOFF :// NPC 생성 & 삭제 
			return ProcessSetNPCOnOff();

		case ID_ACTION_SENDQUESTEMAIL :// 풍운방 보내기 
			return ProcessSendQuestMail();

		case ID_ACTION_HOLDITEM :// 아이템장착해제불가설정 
			return ProcessHoldItem();
			
		case ID_ACTION_PARTYMESSAGE:				// 파티원_메세지출력
			return ProcessPartyMessage();

		case ID_ACTION_DISPLAYQUESTRANKING:				// 특정퀘스트랭킹보여주기
			return ProcessDisplayQuestRanking();

		case ID_ACTION_STARTPARTYITMECOUNTER:				// 파티아이템카운터시작하기
			return ProcessStartPartyItemCounter();
			
		case ID_ACTION_SETVIRTUALMODE:
			return ProcessSetVirtualMode();

		default :
			return QUEST_ACTION_RESULT_PROCESS;
		}
	}
	else
	{
		return QUEST_ACTION_RESULT_PROCESS;
	}
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSoundSetting(void)
{
	int soundfileindex = g_FMODWrapper.InsertSound(m_lpszMessageString, FSOUND_HW2D);
	_XPlayInterfaceSound(soundfileindex);
	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMusicSetting(void)
{
	switch(m_TDA.m_numericvalue2)
	{
	case ID_QUEST_SOUNDTARGET_START :
		return ProcessMusicStart();

	case ID_QUEST_SOUNDTARGET_STOP :
		return ProcessMusicStop();
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMusicStart(void)
{
	// m_actiondata1 : Music file index
	// m_actiondata2 : play type
	// m_numericvalue1 : volume
	// m_numericvalue2 : target class

	// music start
	if(!g_BgmPlayFlag)
		return QUEST_ACTION_RESULT_SUCCESS;

	switch(m_TDA.m_actiondata2)
	{
	case ID_QUEST_PLAYTYPE_LOOP :
		{
			g_BgmPlayOnce = FALSE;
		}
		break;
	case ID_QUEST_PLAYTYPE_ONCE :
		{
			g_BgmPlayOnce = TRUE;
		}
		break;

	default :
		return QUEST_ACTION_RESULT_SUCCESS;
	}

/*	if(g_BgmPlayFlag && g_BgmObject.Getm_dwDeviceID())
		g_BgmObject.Close();

	g_BgmObject.OpenDevice(gHWnd, g_BgmList[m_TDA.m_actiondata1]);
	g_BgmObject.Play(gHWnd, 1);

	long volume = g_MixerVolume.Getm_lmaxVol() - g_MixerVolume.Getm_lminVol();
	long value = long(volume * (m_TDA.m_numericvalue1/100.));

	g_CurrentMixerVolume = value;
	g_MixerVolume.SetFaderValue(value);*/

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMusicStop(void)
{
	// m_actiondata1 : Music file index

/*	// music stop
	if(!g_BgmPlayFlag)
		return QUEST_ACTION_RESULT_SUCCESS;
	if(g_BgmPlayFlag && g_BgmObject.Getm_dwDeviceID())
		g_BgmObject.Close();*/

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessEnvStart(void)
{
	// env setting
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMoveCameraUsePath(void)
{
	// move camera
	D3DXVECTOR3 baseaxis = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 currentaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	FLOAT		rad_theta = 0.0f, deg_theta = 0.0f;

	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// 처음 실행
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		memset(currentcamerainfo, 0, sizeof(_XCAMERA_INFO));

		currentcamerainfo->bStart = TRUE;
		currentcamerainfo->dwTotalTime = (DWORD)(m_TDA.m_numericvalue1*60*1000 + m_TDA.m_numericvalue2*1000);		// ms
		g_LodTerrain.m_PathObjectManager->GeneratePathIterate(m_TDA.m_actiondata2, _XDEF_MAXNUM_PATHPOINT);
		currentcamerainfo->fVelocity = 1000. / currentcamerainfo->dwTotalTime;
		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		currentcamerainfo->nPointPosition = 0;
		m_mapCameraInfo[m_nActionID] = currentcamerainfo;

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		g_LodTerrain.m_3PCamera.SetCameraPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition]);
		g_LodTerrain.m_3PCamera.SetTargetPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition+1]);
		currentaxis = g_LodTerrain.m_3PCamera.m_TargetPosition - g_LodTerrain.m_3PCamera.m_CameraPosition;
		currentaxis.y = 0.0f;
		rad_theta = _XGetAngleFromVectors(&baseaxis, &currentaxis);
		deg_theta = _X_DEG(rad_theta);
		if(currentaxis.x > 0)
		{
			deg_theta = 180 - deg_theta;
			deg_theta += 180;
		}
		g_LodTerrain.m_3PCamera.mp_fYaw = deg_theta;
		g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;

		g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
		g_LodTerrain.RebuildLevel(FALSE);

		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// 진행중
			DWORD elapsedtime = g_LocalSystemTime - currentcamerainfo->dwElapsedTime;
			currentcamerainfo->nPointPosition += (int)(elapsedtime * currentcamerainfo->fVelocity);

			g_LodTerrain.m_3PCamera.SetCameraPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition]);
			g_LodTerrain.m_3PCamera.SetTargetPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition+1]);
			currentaxis = g_LodTerrain.m_3PCamera.m_TargetPosition - g_LodTerrain.m_3PCamera.m_CameraPosition;
			currentaxis.y = 0.0f;
			rad_theta = _XGetAngleFromVectors(&baseaxis, &currentaxis);
			deg_theta = _X_DEG(rad_theta);
			if(currentaxis.x > 0)
			{
				deg_theta = 180 - deg_theta;
				deg_theta += 180;
			}
			g_LodTerrain.m_3PCamera.mp_fYaw = deg_theta;
			g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
			
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);

			currentcamerainfo->dwElapsedTime += elapsedtime;
			
			return QUEST_ACTION_RESULT_PROCESS;
		}
		else
		{
			if(!currentcamerainfo->bEnd)
			{
				// 마지막 frame
				currentcamerainfo->bEnd = TRUE;
				currentcamerainfo->nPointPosition = _XDEF_MAXNUM_PATHPOINT - 1;

				g_LodTerrain.m_3PCamera.SetCameraPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition]);
				g_LodTerrain.m_3PCamera.SetTargetPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition+1]);
				currentaxis = g_LodTerrain.m_3PCamera.m_TargetPosition - g_LodTerrain.m_3PCamera.m_CameraPosition;
				currentaxis.y = 0.0f;
				rad_theta = _XGetAngleFromVectors(&baseaxis, &currentaxis);
				deg_theta = _X_DEG(rad_theta);
				if(currentaxis.x > 0)
				{
					deg_theta = 180 - deg_theta;
					deg_theta += 180;
				}
				g_LodTerrain.m_3PCamera.mp_fYaw = deg_theta;
				g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
				
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
				
				return QUEST_ACTION_RESULT_PROCESS;
			}
			else
			{
				//  다 끝났음. 원래로 되돌리기
				delete currentcamerainfo;
				m_mapCameraInfo.erase(iter_camera);
				if(m_TDA.m_numericvalue4 == ID_QUEST_CAMERA_RESTORE)
				{
					if(g_QuestScriptManager.GetQuestCameraMode())
						g_QuestScriptManager.SetQuestCameraMode(FALSE);

					RestoreCameraInfo();
					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
					g_LodTerrain.RebuildLevel(FALSE);
				}

				return QUEST_ACTION_RESULT_SUCCESS;
			}
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessShakeCamera(void)
{
	// m_actiondata1 : Intensity (흔들림의 강도)
	// m_actiondata2 : Direction (흔들림의 방향)
	// m_numericvalue1 : Minutes
	// m_numericvalue2 : Seconds
	// m_numericvalue3 : Setting (실행후 동작)

	// shake camera
	map <int, _XCAMERA_INFO*>::iterator iter_camerainfo = m_mapCameraInfo.find(m_nActionID);
	if(iter_camerainfo == m_mapCameraInfo.end())
	{
		// 처음 실행
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		currentcamerainfo->dwTotalTime = (DWORD)(m_TDA.m_numericvalue1*60*1000 + m_TDA.m_numericvalue2*1000);
		FLOAT velocity;
		switch(m_TDA.m_actiondata1)
		{
		case ID_QUEST_CAMERAINTENSITY_FIRST :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 5;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_SECOND :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 4;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_THIRD :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 3;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FORTH :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 2;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FIFTH :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT;
			}
			break;
		default :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT;
			}
			break;
		}
		currentcamerainfo->fVelocity = (velocity*gnFPS) / 1000;
		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		currentcamerainfo->nShakeDirection = 0;

		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camerainfo->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camerainfo);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// 실행중
			switch(m_TDA.m_actiondata2)
			{
			case ID_QUEST_CAMERASHAKE_TOPDOWN :
				return ProcessShakeCameraTopDown(currentcamerainfo);
				
			case ID_QUEST_CAMERASHAKE_LEFTRIGHT :
				return ProcessShakeCameraLeftRight(currentcamerainfo);

			default :
				{
					delete currentcamerainfo;
					m_mapCameraInfo.erase(iter_camerainfo);
					if(m_mapCameraInfo.empty())
					{
						g_QuestScriptManager.SetQuestCameraMode(FALSE);
						RestoreCameraInfo();
						g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
						g_LodTerrain.RebuildLevel(FALSE);
					}
					return QUEST_ACTION_RESULT_SUCCESS;
				}
			}
		}
		else
		{
			// 끝
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camerainfo);
			if(m_TDA.m_numericvalue3 == ID_QUEST_CAMERA_RESTORE || m_mapCameraInfo.empty())
			{
				if(g_QuestScriptManager.GetQuestCameraMode())
					g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessShakeCameraTopDown(_XCAMERA_INFO* camerainfo)
{
	if(!camerainfo)
	{
//		if(m_mapCameraInfo.empty())
		{
			g_QuestScriptManager.SetQuestCameraMode(FALSE);
			RestoreCameraInfo();
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		return QUEST_ACTION_RESULT_SUCCESS;
	}
	
	D3DXVECTOR3	currentcameraposition = g_LodTerrain.m_3PCamera.m_CameraPosition;
	D3DXVECTOR3	currenttargetposition = g_LodTerrain.m_3PCamera.m_TargetPosition;
	D3DXVECTOR3	originalcameraposition = g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition;
	
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	
	if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_UP)
	{
		if(fabs(currentcameraposition.y - originalcameraposition.y) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition.y = currentcameraposition.y + (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y + (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition.y = currentcameraposition.y - (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y - (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_DOWN;
		}
	}
	else if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_DOWN)
	{
		if(fabs(currentcameraposition.y - originalcameraposition.y) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition.y = currentcameraposition.y - (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y - (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition.y = currentcameraposition.y + (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y + (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_UP;
		}
	}
	camerainfo->dwElapsedTime += elapsedtime;
	
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessShakeCameraLeftRight(_XCAMERA_INFO* camerainfo)
{
	if(!camerainfo)
	{
//		if(m_mapCameraInfo.empty())
		{
			g_QuestScriptManager.SetQuestCameraMode(FALSE);
			RestoreCameraInfo();
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		return QUEST_ACTION_RESULT_SUCCESS;
	}

	D3DXVECTOR3 currentcameraposition = g_LodTerrain.m_3PCamera.m_CameraPosition;
	D3DXVECTOR3	currenttargetposition = g_LodTerrain.m_3PCamera.m_TargetPosition;
	D3DXVECTOR3	originalcameraposition = g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition;

	D3DXVECTOR3 vDistance = currentcameraposition - originalcameraposition;
	FLOAT distance = D3DXVec3Length(&vDistance);
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;

	if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_LEFT)
	{
		if(fabs(distance) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition = currentcameraposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition = currentcameraposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_RIGHT;
		}
	}
	else if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_RIGHT)
	{
		if(fabs(distance) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition = currentcameraposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition = currentcameraposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_LEFT;
		}
	}
	camerainfo->dwElapsedTime += elapsedtime;

	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessRotateCamera(void)
{
	// m_actiondata1 : area category
	// m_actiondata2 : Direction (방향)
	// m_actiondata3 : Distance (거리)
	// m_actiondata4 : Height (높이)
	// m_numericvalue1 : Minutes
	// m_numericvalue2 : Seconds
	// m_numericvalue3 : Intensity (회전의 강도)
	// m_numericvalue4 : Setting (실행 후 카메라 유지/되돌리기)
	// m_numericvalue5 : Center (회전의 중심)

	// rotate camera
	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// 처음 실행
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		currentcamerainfo->dwTotalTime = (DWORD)(60*m_TDA.m_numericvalue1*1000 + m_TDA.m_numericvalue2*1000);
		FLOAT velocity;
		switch(m_TDA.m_numericvalue3)
		{
		case ID_QUEST_CAMERAINTENSITY_FIRST :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 5;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_SECOND :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 4;	
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_THIRD :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 3;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FORTH :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 2;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FIFTH :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE;
			}
			break;

		default :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE;
			}
			break;
		}
		currentcamerainfo->fVelocity = (velocity*gnFPS) / 1000;

		switch(m_TDA.m_actiondata3)
		{
		case ID_QUEST_CAMERADISTANCE_HIGH :
			{
				currentcamerainfo->fDistance = 15.0f;
			}
			break;
		case ID_QUEST_CAMERADISTANCE_MIDDLE :
			{
				currentcamerainfo->fDistance = 10.0f;
			}
			break;
		case ID_QUEST_CAMERADISTANCE_LOW :
			{
				currentcamerainfo->fDistance = 5.0f;
			}
			break;

		default :
			{
				currentcamerainfo->fDistance = 10.0f;
			}
			break;
		}

		switch(m_TDA.m_actiondata4)
		{
		case ID_QUEST_CAMERAHEIGHT_HIGH :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT * 2.5f;
			}
			break;
		case ID_QUEST_CAMERAHEIGHT_MIDDLE :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		case ID_QUEST_CAMERAHEIGHT_LOW :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT / 2;
			}
			break;

		default :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		}

		if(!g_AreaManager.FindAreaCenterPosition(currentcamerainfo->vecCenterPosition ,m_TDA.m_numericvalue5, m_TDA.m_actiondata1))
		{
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// 회전중
			if(m_TDA.m_actiondata2 == ID_QUEST_CAMERADIRECTION_LEFT)
			{
				return ProcessRotateCameraLeft(currentcamerainfo);
			}
			else
			{
				return ProcessRotateCameraRight(currentcamerainfo);
			}
		}
		else
		{
			// 끝
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
			if(m_TDA.m_numericvalue4 == ID_QUEST_CAMERA_RESTORE)
			{
				if(g_QuestScriptManager.GetQuestCameraMode())
					g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}


_XQUESTACTION_RESULT _XTriggerActionItem::ProcessRotateCameraLeft(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	camerainfo->fRotationAngle += camerainfo->fVelocity*elapsedtime;

	D3DXVECTOR3 position;
	position.x = 0.0f;
	position.y = camerainfo->fHeight;
	position.z = camerainfo->fDistance;
	
	D3DXMATRIX rotmat;
	D3DXMatrixIdentity(&rotmat);
	D3DXMatrixRotationY(&rotmat, _X_RAD(camerainfo->fRotationAngle));
	D3DXVec3TransformCoord(&position, &position, &rotmat);
	
	position.x += camerainfo->vecCenterPosition.x;
	position.y += camerainfo->vecCenterPosition.y;
	position.z += camerainfo->vecCenterPosition.z;
	g_LodTerrain.m_3PCamera.SetCameraPosition(position);
	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	g_LodTerrain.m_3PCamera.mp_fYaw = camerainfo->fRotationAngle;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	camerainfo->dwElapsedTime += elapsedtime;

	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessRotateCameraRight(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	camerainfo->fRotationAngle -= camerainfo->fVelocity*elapsedtime;
	
	D3DXVECTOR3 position;
	position.x = 0.0f;
	position.y = camerainfo->fHeight;
	position.z = camerainfo->fDistance;
	
	D3DXMATRIX rotmat;
	D3DXMatrixIdentity(&rotmat);
	D3DXMatrixRotationY(&rotmat, _X_RAD(camerainfo->fRotationAngle));
	D3DXVec3TransformCoord(&position, &position, &rotmat);
	
	position.x += camerainfo->vecCenterPosition.x;
	position.y += camerainfo->vecCenterPosition.y;
	position.z += camerainfo->vecCenterPosition.z;
	g_LodTerrain.m_3PCamera.SetCameraPosition(position);
	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	g_LodTerrain.m_3PCamera.mp_fYaw = camerainfo->fRotationAngle;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	camerainfo->dwElapsedTime += elapsedtime;
	
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessEffectSetting(void)
{
	// m_actiondata1 : target class
	// m_actiondata2 : area category
	// m_numericvalue1 : area id
	// m_numericvalue2 : visual effect id

	switch(m_TDA.m_actiondata1)
	{
	case 0 :
		{
			// 자기 자신
			D3DXMATRIX offsetmat;
			D3DXMatrixIdentity(&offsetmat);
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
		}
		break;
	case 1 :
		{
			// 현재 타겟
			switch(g_pLocalUser->m_ViewTargetType)
			{
			case _XPICKINGTARGET_MOB :
				{
					_XMob* pMob = (_XMob*)g_pLocalUser->m_ViewTarget;
					if(pMob)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					_XNPCObject* pNPC = (_XNPCObject*)g_pLocalUser->m_ViewTarget;
					if(pNPC)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pNPC->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			case _XPICKINGTARGET_PC :
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
					if(pUser)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT :
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_ViewTarget;
					if(pFunctionalObject)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			}
		}
		break;
	case 2 :
		{
			// 영역
			D3DXVECTOR3 position;
			g_AreaManager.FindAreaCenterPosition(position, m_TDA.m_numericvalue1, m_TDA.m_actiondata2);

			D3DXMATRIX offsetmat;
			D3DXMatrixIdentity(&offsetmat);
			offsetmat._41 = position.x;
			offsetmat._42 = position.y;
			offsetmat._43 = position.z;
			
			g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat );
		}
		break;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTimerSetting(void)
{
	// m_actiondata1 : target class

	switch(m_TDA.m_actiondata1)
	{
	case ID_QUEST_SOUNDTARGET_START :
		return ProcessTimerStart();

	case ID_QUEST_SOUNDTARGET_STOP :
		return ProcessTimerStop();
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTimerStart(void)
{
	// timer start	
	//	scriptdata->m_TDA.m_actiondata2	= m_TargetClass2;	
	//	scriptdata->m_TDA.m_numericvalue1 = m_Minutes;
	//	scriptdata->m_TDA.m_numericvalue2 = m_Seconds;
	
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}

	if(currentquest)
	{
		//노드번호 넣어주기 
		DWORD starttime = timeGetTime();
		currentquest->m_mapTriggerTimer[currentquest->GetCurrentTreeNumber()] =  starttime;// g_ServerTimeCode;
				
		if( m_TDA.m_actiondata2 == 0 )//표시하기
		{
			g_QuestScriptManager.m_TimerStartQuestID	= currentquest->GetQuestID();
			g_QuestScriptManager.m_bStartQuestTimer		= TRUE;
			g_QuestScriptManager.m_QuestTimerStartTime	= g_ServerTimeCode;
			if( m_TDA.m_numericvalue1 == 0 && m_TDA.m_numericvalue2 == 0 )
			{
				g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
			}
			else
			{
				g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
				g_QuestScriptManager.m_QuestTimerTimeLimit	= (60*m_TDA.m_numericvalue1) + m_TDA.m_numericvalue2;
			}
		}
		else//표시하지 않기
		{
		}
	}


	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTimerStop(void)
{
	// timer stop
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		//노드번호 넣어주기 
		currentquest->m_mapTriggerTimer[currentquest->GetCurrentTreeNumber()] = 0;

		g_QuestScriptManager.m_TimerStartQuestID	= -1;
		g_QuestScriptManager.m_bStartQuestTimer		= FALSE;
		g_QuestScriptManager.m_QuestTimerStartTime	= 0;
		g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
	}
	
	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSendMessage(void)
{
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if( m_lpszMessageString )
		{
			g_QuestScriptManager.SetProgressMessage(m_nOwnerQuestID, currentquest->GetLastTarggetingNPCID(), m_lpszMessageString);
		}
		g_QuestScriptManager.m_bStartMessage = TRUE;

	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessCameraSetting(void)
{
	// m_actiondata1 : Camera mode (Zoom in / Zoom out)
	// m_actiondata2 : Setting (실행 후 카메라 유지/되돌리기)
	// m_actiondata3 : Target (대상)
	// m_numericvalue1 : Area (영역)
	// m_numericvalue2 : Area category 

	// camera setting
	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// 처음
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;
		currentcamerainfo->dwTotalTime = 2 * 1000;		// total time = 3sec

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		switch(m_TDA.m_actiondata3)
		{
		case ID_QUEST_CAMERASETTING_PLAYER :
			{
				currentcamerainfo->vecCenterPosition = g_pLocalUser->m_Position;
				currentcamerainfo->vecCenterPosition.y += _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		case ID_QUEST_CAMERASETTING_AREA :
			{
				if(!g_AreaManager.FindAreaCenterPosition(currentcamerainfo->vecCenterPosition, m_TDA.m_numericvalue1, m_TDA.m_numericvalue2))
				{
					if(m_mapCameraInfo.empty())
					{
						g_QuestScriptManager.SetQuestCameraMode(FALSE);
						RestoreCameraInfo();
						g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
						g_LodTerrain.RebuildLevel(FALSE);
					}
					return QUEST_ACTION_RESULT_SUCCESS;
				}
				currentcamerainfo->vecCenterPosition.y += _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		default :
			{
				currentcamerainfo->vecCenterPosition = g_pLocalUser->m_Position;
				currentcamerainfo->vecCenterPosition.y += _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		}
		currentcamerainfo->fTargetDistanceVelocity = (0.5*gnFPS) / 1000;
		currentcamerainfo->fFovVelocity = (1.0*gnFPS) / 1000;

		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;

		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// 실행중
			switch(m_TDA.m_actiondata1)
			{
			case ID_QUEST_CAMERASETTING_ZOOMIN :
				return ProcessCameraSettingZoomIn(currentcamerainfo);
			case ID_QUEST_CAMERASETTING_ZOOMOUT :
				return ProcessCameraSettingZoomOut(currentcamerainfo);
			default :
				{
					delete currentcamerainfo;
					m_mapCameraInfo.erase(iter_camera);
//					if(m_mapCameraInfo.empty())
					{
						g_QuestScriptManager.SetQuestCameraMode(FALSE);
						RestoreCameraInfo();
						g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
						g_LodTerrain.RebuildLevel(FALSE);
					}
					return QUEST_ACTION_RESULT_SUCCESS;
				}
			}
		}
		else
		{
			// 끝
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
			if(m_TDA.m_actiondata2 == ID_QUEST_CAMERA_RESTORE)
			{
				if(g_QuestScriptManager.GetQuestCameraMode())
					g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessCameraSettingZoomIn(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;

	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	if(g_LodTerrain.m_3PCamera.m_TargetDistance > 2.0f)
		g_LodTerrain.m_3PCamera.m_TargetDistance -= camerainfo->fTargetDistanceVelocity*elapsedtime;
	if(g_LodTerrain.m_3PCamera.mp_fFov > 10.0f)
		g_LodTerrain.m_3PCamera.mp_fFov -= camerainfo->fFovVelocity*elapsedtime;

	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);

	camerainfo->dwElapsedTime += elapsedtime;

	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessCameraSettingZoomOut(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	
	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	if(g_LodTerrain.m_3PCamera.m_TargetDistance < 15.0f)
		g_LodTerrain.m_3PCamera.m_TargetDistance += camerainfo->fTargetDistanceVelocity*elapsedtime;
	if(g_LodTerrain.m_3PCamera.mp_fFov < 50.0f)
		g_LodTerrain.m_3PCamera.mp_fFov += camerainfo->fFovVelocity*elapsedtime;
	
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	camerainfo->dwElapsedTime += elapsedtime;
	
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessScreenEffect(void)
{
	// m_actiondata1 : Screen Effect (Fade in / Fade out)

	// screen effect
	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// 처음
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;
		
		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
		}
		
		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		currentcamerainfo->dwTotalTime = 1*1000;		// 3 sec
		
		if(m_TDA.m_actiondata1 == ID_QUEST_SCREENEFFECT_FADEOUT)
			g_FadeInOutLevel = 0;
		else
			g_FadeInOutLevel = 255;
		
		g_DrawFadeInOutFlag = TRUE;
		
		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				g_FadeInOutLevel = 0;
				g_DrawFadeInOutFlag = FALSE;
				return QUEST_ACTION_RESULT_SUCCESS;
			}
		}
		
		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			DWORD elapsedtime = g_LocalSystemTime - currentcamerainfo->dwElapsedTime;
			g_DrawFadeInOutFlag = TRUE;
			
			switch(m_TDA.m_actiondata1)
			{
			case ID_QUEST_SCREENEFFECT_FADEOUT :
				{
					g_FadeInOutLevel += _XDEF_SCREENFADE_LEVEL;
					if(g_FadeInOutLevel >= 255)
						g_FadeInOutLevel = 255;
				}
				break;
			case ID_QUEST_SCREENEFFECT_FADEIN :
				{
					g_FadeInOutLevel -= _XDEF_SCREENFADE_LEVEL;
					if(g_FadeInOutLevel <= 0)
						g_FadeInOutLevel = 0;
				}
				break;
			}
			currentcamerainfo->dwElapsedTime += elapsedtime;
			return QUEST_ACTION_RESULT_PROCESS;
		}
		else
		{
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				g_QuestScriptManager.SetQuestWideView(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			g_FadeInOutLevel = 0;
			g_DrawFadeInOutFlag = FALSE;
			
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessWeatherStart(void)
{
	// weather start
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessBlockTrigger(void)
{
	// block trigger
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessAutoStart(void)
{
	
	_XQuestScriptItem* currentquest = NULL;
	if( m_TDA.m_numericvalue1 >= 10000 && m_TDA.m_numericvalue1 < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_TDA.m_numericvalue1);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_TDA.m_numericvalue1);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		currentquest->SetAutoRun(TRUE);
	}


	return QUEST_ACTION_RESULT_SUCCESS;
}

void _XTriggerActionItem::ReleasePrivateData(void)
{
	m_bSuccessClient		= FALSE;
	m_bSuccessServer		= FALSE;
	m_bConfirmAll			= FALSE;

	m_mapCameraValue.clear();

	map <int, _XCAMERA_INFO*>::iterator iter_camera;
	for(iter_camera = m_mapCameraInfo.begin() ; iter_camera != m_mapCameraInfo.end() ; ++iter_camera)
	{
		_XCAMERA_INFO* currentcamera = iter_camera->second;
		if(currentcamera)
			SAFE_DELETE(currentcamera);
	}
	m_mapCameraInfo.clear();
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessQuestProgressInfo(void)
{
	//퀘스트 창에 진행 정보 표시
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if(!m_lpszMessageString)
			return QUEST_ACTION_RESULT_ERROR;
		
		int strlength = strlen(m_lpszMessageString);
		
		if(strlength > 0)
		{
			currentquest->m_lpszQuestProgressinformation = m_lpszMessageString;
			if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
		}		
	}


	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSelection(void)
{	
	//	scriptdata->m_TDA.m_actiondata1 = m_ItemClass1;
	//	scriptdata->m_TDA.m_actiondata2 = m_ItemClass2;
	//	scriptdata->m_TDA.m_actiondata3 = m_ItemClass3;
	//	scriptdata->m_TDA.m_actiondata4 = m_ItemClass4;
	//	scriptdata->m_TDA.m_numericvalue1 = m_ItemID1;
	//	scriptdata->m_TDA.m_numericvalue2 = m_ItemID2;
	//	scriptdata->m_TDA.m_numericvalue3 = m_ItemID3;
	//	scriptdata->m_TDA.m_numericvalue4 = m_ItemID4;

	
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if(!m_lpszMessageString)
			return QUEST_ACTION_RESULT_ERROR;
		
		if( g_QuestScriptManager.m_bStartMessage )
		{
			return QUEST_ACTION_RESULT_PROCESS;
		}
		else
		{			
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if( pNPCScript_Window )
			{
				if( !pNPCScript_Window->m_bNewChatDisplay )
				{
					pNPCScript_Window->m_listboxSelectionItem->DeleteAllItem();

					// 아이템 선택일 때 
					if( m_TDA.m_actiondata1 > -1 || m_TDA.m_actiondata2 > -1 || m_TDA.m_actiondata3 > -1 || m_TDA.m_actiondata4 > -1  )
					{

						pNPCScript_Window->m_listboxSelectionItem->SetIconReportMode( TRUE );
						pNPCScript_Window->m_listboxSelectionItem->SetIconReportModeInfo( 2, 2, 45, 181 );
						
						pNPCScript_Window->m_listboxSelectionItem->SetTrackWidth(181);
						pNPCScript_Window->m_listboxSelectionItem->SetTrackHeight(45);
						
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(3, _XLCOLUMNSTYLE_ICONRECT);	// column style 
						
						POINT selectioncoloffset1 = { 0, 0 };
						POINT selectioncoloffset2 = { 67, 17 };
						POINT selectioncoloffset3 = { 27, 8 };
						POINT selectioncoloffset4 = { 24, 5 };
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(0, selectioncoloffset1);	// column offset
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(1, selectioncoloffset2);	// column offset
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(2, selectioncoloffset3);	// column offset
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(3, selectioncoloffset4);	// column offset
						
						TCHAR	buffer[2048];
						_XStringSplitInfo splitinfo;

						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
						{
							g_XBaseFont->SplitString(m_lpszMessageString, 342, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
						}
						else
						{
							g_XBaseFont->SplitStringByCR(m_lpszMessageString, splitinfo);
						}
						
						for(int i = 0 ; i < splitinfo.splitcount ; ++i)
						{
							memset(buffer, 0, sizeof(buffer));
							strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
							
							
							int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
							RECT rect1 = {6, 412, 187, 457};	
							RECT rect2 = {0, 0, 0, 0};							
							RECT rect3 = {450, 240, 486, 276};	

							int imageiconindex = 0, category = -1, itemid = -1;															
							
							switch(i) {
							case 0:
								{
									category = m_TDA.m_actiondata1;
									itemid = m_TDA.m_numericvalue1;
								}
								break;
							case 1:
								{
									category = m_TDA.m_actiondata2;
									itemid = m_TDA.m_numericvalue2;
								}
								break;
							case 2:
								{
									category = m_TDA.m_actiondata3;
									itemid = m_TDA.m_numericvalue3;
								}
								break;
							case 3:
								{
									category = m_TDA.m_actiondata4;
									itemid = m_TDA.m_numericvalue4;
								}
								break;
							}
							g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect2, TRUE);
							

							pNPCScript_Window->m_listboxSelectionItem->InsertListItem(_T(""), 0, i+1);
							
							pNPCScript_Window->m_listboxSelectionItem->SetIconArchive(i, &g_MainInterfaceTextureArchive);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 0, npcresourceindex1, rect1);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 2, imageiconindex, rect2);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 3, npcresourceindex1, rect3);

							if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
							{
								pNPCScript_Window->m_listboxSelectionItem->SetSeparateText(i, 1, buffer, 100 );
							}
							else
							{
								pNPCScript_Window->m_listboxSelectionItem->SetItemText(i, 1, buffer);
							}
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 1, _XSC_DEFAULT);	
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttribHighlight(i, 1, _XSC_DEFAULT_HIGHLIGHT);		
							
						}
						
						POINT winpos = pNPCScript_Window->GetWindowPos();
						pNPCScript_Window->m_listboxSelectionItem->MoveWindow( winpos.x + 35, winpos.y + 300 );

						pNPCScript_Window->m_NPCDialogSubBorderB1->ShowWindow(TRUE);
						pNPCScript_Window->m_NPCDialogSubBorderB2->ShowWindow(TRUE);
						pNPCScript_Window->m_btnQuestOK->ShowWindow(FALSE);
						pNPCScript_Window->m_btnBackToNPCMode->ShowWindow(FALSE);
						pNPCScript_Window->m_listboxSelectionItem->ShowWindow(TRUE);
						pNPCScript_Window->m_btnNPCDialogModExit->ShowWindow(FALSE);
						
						currentquest->m_bSelectionNumberCheck = FALSE;
						currentquest->m_SelectionNumber = -1;
						return QUEST_ACTION_RESULT_SUCCESS;

					}
					else // 문장을 선택할 때 					
					{
						pNPCScript_Window->m_listboxSelectionItem->SetIconReportMode( FALSE );
						
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							POINT selectioncoloffset1 = { 0, 3 };
							POINT selectioncoloffset2 = { 14, 2 };
							POINT selectioncoloffset3 = { 27, 8 };
							POINT selectioncoloffset4 = { 24, 5 };
							
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(0, selectioncoloffset1);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(1, selectioncoloffset2);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(2, selectioncoloffset3);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(3, selectioncoloffset4);	// column offset
						}
						else
						{
							POINT selectioncoloffset1 = { 0, 3 };
							POINT selectioncoloffset2 = { 14, 4 };
							POINT selectioncoloffset3 = { 27, 8 };
							POINT selectioncoloffset4 = { 24, 5 };
							
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(0, selectioncoloffset1);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(1, selectioncoloffset2);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(2, selectioncoloffset3);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(3, selectioncoloffset4);	// column offset
						}

						pNPCScript_Window->m_listboxSelectionItem->SetTrackWidth(400);
						pNPCScript_Window->m_listboxSelectionItem->SetTrackHeight(20);
						
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(3, _XLCOLUMNSTYLE_ICONRECT);	// column style 
						
//						pNPCScript_Window->m_listboxSelectionItem->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
//						pNPCScript_Window->m_listboxSelectionItem->SetSelectedImagePosition(0, 1);
//						pNPCScript_Window->m_listboxSelectionItem->SetSelectedImageHeight(20);
//						pNPCScript_Window->m_listboxSelectionItem->SetTrackWidth(330);


						TCHAR	buffer[2048];
						_XStringSplitInfo splitinfo;
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							g_XBaseFont->SplitString(m_lpszMessageString, 600, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
							//g_XBaseFont->SplitStringByCR(m_lpszMessageString, splitinfo);
						}
						else
						{
							g_XBaseFont->SplitStringByCR(m_lpszMessageString, splitinfo);
						}
						
						for(int i = 0 ; i < splitinfo.splitcount ; ++i)
						{
							memset(buffer, 0, sizeof(buffer));
							strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
							
							
							int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
							RECT rect = {0, 487, 18, 499};												
							
							pNPCScript_Window->m_listboxSelectionItem->InsertListItem(_T(""), 0, i+1);
							
							pNPCScript_Window->m_listboxSelectionItem->SetIconArchive(i, &g_MainInterfaceTextureArchive);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 0, npcresourceindex1, rect);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 2, -1, rect);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 3, -1, rect);
							
							pNPCScript_Window->m_listboxSelectionItem->SetItemText(i, 1, buffer);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 1, _XSC_DEFAULT_BLACK);	
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttribHighlight(i, 1, _XSC_INFORMATION);		
							
						}

						POINT winpos = pNPCScript_Window->GetWindowPos();
						
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							pNPCScript_Window->m_listboxSelectionItem->MoveWindow( winpos.x + 19, winpos.y + 338 - ( 8 * splitinfo.splitcount ) );
						}
						else
						{
							pNPCScript_Window->m_listboxSelectionItem->MoveWindow( winpos.x + 65, winpos.y + 338 - ( 8 * splitinfo.splitcount ) );
						}

						pNPCScript_Window->m_NPCDialogSubBorderB1->ShowWindow(TRUE);
						pNPCScript_Window->m_NPCDialogSubBorderB2->ShowWindow(TRUE);
						pNPCScript_Window->m_btnQuestOK->ShowWindow(FALSE);
						pNPCScript_Window->m_btnBackToNPCMode->ShowWindow(FALSE);
						pNPCScript_Window->m_btnNPCDialogModExit->ShowWindow(FALSE);
						pNPCScript_Window->m_listboxSelectionItem->ShowWindow(TRUE);
						
						currentquest->m_bSelectionNumberCheck = FALSE;
						currentquest->m_SelectionNumber = -1;
						return QUEST_ACTION_RESULT_SUCCESS;
					}
				}
				else
				{
					return QUEST_ACTION_RESULT_PROCESS;
				}
			}			
		}
		
	}
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartMiniGame(void)
{
	//m_actiondata1 0:운기조식
	switch(m_TDA.m_actiondata1) {
	case 0: // 운기조식
		{
			_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
			if(pMeditation_Window)
			{
				pMeditation_Window->m_bQuestMiniGameStart = TRUE;
				pMeditation_Window->SetMeditationStep(0);
			}
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRAINING_MEDITATION_START, 0), (LPARAM)gHWnd);
/*			g_pLocalUser->m_UserState = _XDEF_USERSTATE_TRAINING;
			g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_MEDITATION;
			g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_10;
			
			_XWindow_TrainingMeditation* pMeditation_Window = (_XWindow_TrainingMeditation*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
		
			if( pMeditation_Window )
			{
				pMeditation_Window->GetMeditationSkillid();
				pMeditation_Window->GetPathIndex();
				pMeditation_Window->ShowWindow(TRUE);
				pMeditation_Window->SetMeditationMode(MM_READY2);
				pMeditation_Window->m_bMustGameStart = TRUE;				
				
				XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;		
				pProc_Maingame->m_bMiniGameStart = TRUE;	
				
				extern BOOL g_PrevSoundEffectFlag;
				g_PrevSoundEffectFlag = g_SoundEffectFlag;
				g_SoundEffectFlag = FALSE;
			}*/
		}
		break;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTalkingToOneself(void)
{
	// send message
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{		
		if( m_lpszMessageString )
		{
			g_pLocalUser->m_ChatBalloonObject.m_MessageColor = g_ChattingStringColor[7];//귓속말 색깔
			g_pLocalUser->m_ChatBalloonObject.SetMessage(m_lpszMessageString);		
		}
	}

	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMoveVelocity(void)
{
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartOptionCondition(void)
{
	// m_actiondata1 : m_TargetClass
	// ID_QUEST_OPTIONRESULT_CANCLERESET				0
	// ID_QUEST_OPTIONRESULT_CANCLEFAIL					1
	// m_numericvalue1 : m_OptionConditionID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{		
		_XTriggerConditionItem* currentconditiontrigger = NULL;
		map <int, _XTriggerConditionItem*>::iterator iter_trigger;
		iter_trigger = currentquest->m_mapOptionConditionList.find(m_TDA.m_numericvalue1);
		if(iter_trigger != currentquest->m_mapOptionConditionList.end())
		{
			currentconditiontrigger = iter_trigger->second;	
			if(currentconditiontrigger)
			{				
				
				_XTriggerConditionItem* failcondition = new _XTriggerConditionItem;

				if(failcondition)
				{
					failcondition = currentconditiontrigger;
					
					if( m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLESUCCESS )
					{
						failcondition->m_bSuccessCancle = TRUE;
					}
					else if( m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLEFAIL )
					{
						failcondition->m_bSuccessCancle = FALSE;
					}

					currentquest->m_mapFailConditionList[ m_TDA.m_numericvalue1 ] = failcondition;
				}
			}	
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessEndOptionCondition(void)
{
	// m_numericvalue1 : m_OptionConditionID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if( !currentquest->m_mapFailConditionList.empty() )
		{
			
			map <int, _XTriggerConditionItem*>::iterator iter_trigger;
			iter_trigger = currentquest->m_mapFailConditionList.find(m_TDA.m_numericvalue1);
			if(iter_trigger != currentquest->m_mapFailConditionList.end())
			{
				_XTriggerConditionItem* failcondition = iter_trigger->second;	
				if(failcondition)
				{
					//delete failcondition;
					//failcondition = NULL;
					currentquest->m_mapFailConditionList.erase(iter_trigger);
				}
			}
		}
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessGiveCountPoint(void)
{	
	//m_actiondata1 = m_CounterType;	
	//m_actiondata2 = m_DataSet;	// 0: 추가 1: 삭제
	//m_numericvalue1  = m_CountPoint;	


	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_ACTION_RESULT_SUCCESS;

	// 파티 퀘스트면 서버에서 오는 값으로 세팅해야 한다.
	if( m_TDA.m_actiondata1 >= 4 )
		return QUEST_ACTION_RESULT_SUCCESS;

	switch(m_TDA.m_actiondata2) {
	case ID_TRIGGER_DATASET_ADD:		
		{
			currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint += m_TDA.m_numericvalue1;	
						
			TCHAR	buffer[256];
			memset(buffer, 0, sizeof(buffer));
			if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= 1 )
			{
				if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName[0] != 0 )
				{
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint > currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );						
					}
					else if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint )
					{
						_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),//_T("%s : %d/%d (완료)")
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );							
					}	
					if( g_pQuest_Window )
					{
						if( g_pQuest_Window->GetShowStatus() )
						{
							if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
							{
								g_pQuest_Window->ParseQuestDescString();	
							}
						}
					}				
					
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						if( buffer[0] != 0 )
						{
							int strlength = strlen(buffer);
							
							if(g_QuestScriptManager.m_lpszQuestSystemMessage )
							{
								delete g_QuestScriptManager.m_lpszQuestSystemMessage;
								g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
							}
							
							if(strlength > 0)
							{
								g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
								g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
								strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
							}
							
							g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
							g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
						}				
					}					
				}							
			}				
			else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )
			{		
				_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint );
				
				if( g_pQuest_Window )
				{
					if( g_pQuest_Window->GetShowStatus() )
					{
						if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
						{
							g_pQuest_Window->ParseQuestDescString();	
						}
					}
				}				

				if( buffer[0] != 0 )
				{
					int strlength = strlen(buffer);
					
					if(g_QuestScriptManager.m_lpszQuestSystemMessage )
					{
						delete g_QuestScriptManager.m_lpszQuestSystemMessage;
						g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
					}
					
					if(strlength > 0)
					{
						g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
						g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
						strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
					}
					
					g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
					g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
				}								
			}// else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )		
		}
		break;
	case ID_TRIGGER_DATASET_SUBTRACT:
		{
			currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint -= m_TDA.m_numericvalue1;

			TCHAR	buffer[256];
			memset(buffer, 0, sizeof(TCHAR)*256);
			if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= 1 )
			{
				if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName[0] != 0 )
				{
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint > currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );	
						
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
								{
									g_pQuest_Window->ParseQuestDescString();	
								}
							}
						}					
					}
					else if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint )
					{
						_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (완료)")
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );	
						
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
								{
									g_pQuest_Window->ParseQuestDescString();	
								}
							}
						}										
					}	
					
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						if( buffer[0] != 0 )
						{
							int strlength = strlen(buffer);
							
							if(g_QuestScriptManager.m_lpszQuestSystemMessage )
							{
								delete g_QuestScriptManager.m_lpszQuestSystemMessage;
								g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
							}
							
							if(strlength > 0)
							{
								g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
								g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
								strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
							}
							
							g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
							g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
						}				
					}					
				}	
			}
			else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )
			{		
				_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint );

				if( g_pQuest_Window )
				{
					if( g_pQuest_Window->GetShowStatus() )
					{
						if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
						{
							g_pQuest_Window->ParseQuestDescString();	
						}
					}
				}				
				
				if( buffer[0] != 0 )
				{
					int strlength = strlen(buffer);
					
					if(g_QuestScriptManager.m_lpszQuestSystemMessage )
					{
						delete g_QuestScriptManager.m_lpszQuestSystemMessage;
						g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
					}
					
					if(strlength > 0)
					{
						g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
						g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
						strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
					}
					
					g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
					g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
				}							
			}// else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )		
		}
		break;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSpawnMonster(void)// 몬스터 불러오기
{
	//scriptdata->m_TDA.nActionData1  = m_index1;				// 플레이어,영역,DNPC플레이어,DNPC영역,DNPC플레이어(RANDOM),DNPC영역(RANDOM)
	// 클라이언트몬스터부르기(영역),클라이언트DNPC부르기(영역),클라이언트DNPC삭제하기
	//scriptdata->m_TDA.nActionData2  = m_value6;				// 거리
	//scriptdata->m_TDA.nActionData3  = m_index2;				// 영역카테고리
	//scriptdata->m_TDA.nActionData4  = m_value3;				// 패스 타입
	//scriptdata->m_TDA.nNumericValue1  = m_value1;			// 영역아이디
	//scriptdata->m_TDA.nNumericValue2  = m_value2;			// 몬스터 공격력
	//scriptdata->m_TDA.nNumericValue3  = m_value4;			// 몬스터아이디
	//scriptdata->m_TDA.nNumericValue4  = m_value5;			// 몬스터 개수

	if( m_TDA.m_actiondata1 == 6 )//클라이언트몬스터부르기(영역)
	{
		_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata3, m_TDA.m_numericvalue1 );
		
		if( pArea )
		{
			int monstercount = m_TDA.m_numericvalue4;
			int monsterindex = m_TDA.m_numericvalue3;
			FLOAT position_x = 0;
			FLOAT position_z = 0;
			FLOAT x = 0;
			FLOAT z = 0;
			D3DXVECTOR3 monsterposition;
			for( int i = 0; i < monstercount; ++i )
			{				
				position_x = _XRandom()%100;
				position_z = _XRandom()%100;

				x = (pArea->m_Point[2].x - pArea->m_Point[0].x)*((FLOAT)position_x/(FLOAT)100.0f);
				z = (pArea->m_Point[2].z - pArea->m_Point[0].z)*((FLOAT)position_z/(FLOAT)100.0f);

				monsterposition.x = pArea->m_Point[0].x + x;
				monsterposition.y = 0;
				monsterposition.z = pArea->m_Point[0].z + z;
			
				int clientmobID = g_Moblist.FindClientMobID();
				if( clientmobID != 0 )
				{
					_XMob* pMob = g_Moblist.CreateMob(clientmobID, monsterindex, 0, 100, monsterposition,monsterposition);	
					if( pMob )
					{
						pMob->m_bClientMob = TRUE;
						pMob->m_OriginPosition = monsterposition;
						pMob->m_AttackHitValue = g_MobAttribTable[monsterindex-4000].attackpowermax; 
						pMob->m_CurrentLife = pMob->m_MaxLife = 100;
					}
				}
			}
		}
	}
	else if( m_TDA.m_actiondata1 == 7 )//클라이언트DNPC부르기(영역)
	{
		_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata3, m_TDA.m_numericvalue1 );
		
		if( pArea )
		{
			int monstercount = m_TDA.m_numericvalue4;
			int monsterindex = m_TDA.m_numericvalue3;
			FLOAT position_x = 0;
			FLOAT position_z = 0;
			FLOAT x = 0;
			FLOAT z = 0;
			D3DXVECTOR3 monsterposition;
			for( int i = 0; i < monstercount; ++i )
			{								
				x = (pArea->m_Point[2].x - pArea->m_Point[0].x)*(0.5f);
				z = (pArea->m_Point[2].z - pArea->m_Point[0].z)*(0.5f);
				
				monsterposition.x = pArea->m_Point[0].x + x;
				monsterposition.y = 0;
				monsterposition.z = pArea->m_Point[0].z + z;
				
				int clientmobID = g_Moblist.FindClientMobID();
				if( clientmobID != 0 )
				{
					_XMob* pMob = g_Moblist.CreateMob(clientmobID, monsterindex, 0, 100, monsterposition,monsterposition);	
					if( pMob )
					{
						pMob->m_bClientMob = TRUE;
						pMob->m_bClientDNPC = TRUE;
						pMob->m_OriginPosition = monsterposition;
						pMob->m_AttackHitValue = g_MobAttribTable[monsterindex-4000].attackpowermax; 
						pMob->m_CurrentLife = pMob->m_MaxLife = 100;
						
						
						pMob->m_EndPosition.x = -67.574074f;
						pMob->m_EndPosition.y = 0.0f;
						pMob->m_EndPosition.z = -61.161400f;
					}
				}
			}
		}
	}
	else if( m_TDA.m_actiondata1 == 8 )//클라이언트DNPC삭제하기
	{
		_XMob* pMob = NULL;
		for (smdef_XMob::iterator it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ it)
		{
			pMob = (*it).second;
			
			if( pMob->m_bClientMob ) // 클라이언트 몬스터로 바꾼다.
			{
				if( pMob->m_bClientDNPC )//클라이언트 몬스터
				{
					if( pMob->m_MobType == (m_TDA.m_numericvalue3) )
					{
						pMob->m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
					}
				}
			}
		}
	}

	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetMotion(void)
{
	// m_numericvalue1 : m_MotionIndex
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetMovie(void)
{	
	// m_numericvalue1 : m_MovieIndex
	g_indexScene = m_TDA.m_numericvalue1;
	if( g_SceneList[g_indexScene ] )
	{
		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_QUEST;
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetMap(void)// 지도 ON/OFF
{
	// m_actiondata1 : 0:ON, 1:OFF
	switch(m_TDA.m_actiondata1) {
	case 0://ON
		{
			_XWindow_MaximumMap* pMaximumMap_Window = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pMaximumMap_Window ) pMaximumMap_Window->ShowWindow(TRUE);			
		}
		break;
	case 1://OFF
		{
			_XWindow_MaximumMap* pMaximumMap_Window = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pMaximumMap_Window ) pMaximumMap_Window->ShowWindow(FALSE);		
		}
		break;
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMobtalkingToOneself(void)// 몬스터 혼자말하기
{
	// m_numericvalue1 : Mob ID
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessResultQuest(void)
{
	// m_actiondata1 : 퀘스트 결과 0:성공 1:실패 2:취소;

	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		return QUEST_ACTION_RESULT_FAIL;
//		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
//		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
//			currentquest = iter_quest->second;
//		else
//			return QUEST_ACTION_RESULT_FAIL;

	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_ACTION_RESULT_FAIL;
	
	if( m_TDA.m_actiondata1 == 2 )
	{
		g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, m_nOwnerQuestID, currentquest->GetCurrentTreeNumber() );	
		return QUEST_ACTION_RESULT_SUCCESS;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartItemCounter(void)// 아이템 카운터 시작하기
{
	
	//scriptdata->m_TDA.m_actiondata1  = m_ItemClass;	
	//scriptdata->m_TDA.m_numericvalue1  = m_ItemID;
	//scriptdata->m_TDA.m_numericvalue2  = m_Count;
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if( m_TDA.m_actiondata1 <= _XGI_FC_SOCKET )
	{
		bool bexist = false;
		for( int i = 0; i < 8; ++i )
		{
			if( currentquest->m_ItemCounterInformation[i].CounterTargetType == m_TDA.m_actiondata1 
				&& currentquest->m_ItemCounterInformation[i].CounterTargetID == m_TDA.m_numericvalue1
				&& currentquest->m_ItemCounterInformation[i].CountTotalPoint == m_TDA.m_numericvalue2 )
			{
				bexist = true;
				break;
			}
		}
		
		if( !bexist )
		{	
			for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
			{
				if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == -1 )
				{	
					// 카운터 세팅
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType = m_TDA.m_actiondata1; // 타입
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID = m_TDA.m_numericvalue1;   // 아이디 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint = m_TDA.m_numericvalue2;	 // 총 개수 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountCurrentPoint = 0;
					
					if( m_lpszMessageString )
					{
						int strlength = strlen(m_lpszMessageString);
						
						if(strlength > 0)
						{
							memset( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256 );
							strcpy( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, m_lpszMessageString );
						}		
						else
						{			
							char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
							short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
							
							memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
							strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
						}	
					}
					else
					{
						char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
						short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
						
						memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
						strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );			
						
					}
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(TCHAR)*256);
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}

				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();		
					}
				#endif
					
					if( itemtotalcount != 0 )
					{
						if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint != 0 )
						{
							if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									itemtotalcount,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
							}
							else if( itemtotalcount >= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (완료)")
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
							}
							
							if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								if( buffer[0] != 0 )
								{
									int strlength = strlen(buffer);
									
									if(g_QuestScriptManager.m_lpszQuestSystemMessage )
									{
										SAFE_DELETE(g_QuestScriptManager.m_lpszQuestSystemMessage);
									}
									
									if(strlength > 0)
									{
										g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
										g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
										strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
									}
									
									g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
									g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
								}		
							}			

						}
						else
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, itemtotalcount);

							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									delete g_QuestScriptManager.m_lpszQuestSystemMessage;
									g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}	
					}
					
					
					break;
				}
				
			}
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	else if( m_TDA.m_actiondata1 > 14 && m_TDA.m_actiondata1 < 19 ) //15,16,17,18 <-카운트 A B C D 
	{
		
		int countertype = m_TDA.m_actiondata1 - _XGI_FC_SOCKET -1;
		
		// 카운터 세팅
		currentquest->m_CounterInformation[countertype].CounterType = countertype;
		currentquest->m_CounterInformation[countertype].CounterTargetID = m_TDA.m_numericvalue1;
		currentquest->m_CounterInformation[countertype].CountTotalPoint = m_TDA.m_numericvalue2;
		currentquest->m_CounterInformation[countertype].CountCurrentPoint = 0;
		
		if( m_lpszMessageString )
		{
			int strlength = strlen(m_lpszMessageString);
			
			if(strlength > 0)
			{
				memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, m_lpszMessageString );
			}		
			else
			{			
				int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
				memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				if( g_MonsterName[mobid] )
					strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
				else
					strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
			}
		}
		else
		{
			int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
			memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
			if( g_MonsterName[mobid] )
				strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
			else
				strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	else if( m_TDA.m_actiondata1 > 18 && m_TDA.m_actiondata1 < 23 ) //19,20,21,22 <-파티 카운트 A B C D 
	{
		int countertype = m_TDA.m_actiondata1 - _XGI_FC_SOCKET - 5;
		
		// 카운터 세팅
		currentquest->m_PartyCounterInformation[countertype].CounterType		= countertype;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetType	= -1;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetID	= m_TDA.m_numericvalue1;
		currentquest->m_PartyCounterInformation[countertype].CountTotalPoint	= m_TDA.m_numericvalue2;
		currentquest->m_PartyCounterInformation[countertype].CountCurrentPoint	= 0;
		
		if( m_lpszMessageString )
		{
			int strlength = strlen(m_lpszMessageString);
			
			if(strlength > 0)
			{
				memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, m_lpszMessageString);			
			}		
			else
			{			
				int mobid = currentquest->m_PartyCounterInformation[countertype].CounterTargetID- 4000;
				memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				if( g_MonsterName[mobid] )
					strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
				else
					strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
			}
		}
		else
		{
			int mobid = currentquest->m_PartyCounterInformation[countertype].CounterTargetID- 4000;
			memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
			if( g_MonsterName[mobid] )
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
			else
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	else if( m_TDA.m_actiondata1 > 22  )
	{
		bool bexist = false;
		for( int i = 0; i < 8; ++i )
		{
			if( currentquest->m_ItemCounterInformation[i].CounterTargetType == (m_TDA.m_actiondata1-8) 
				&& currentquest->m_ItemCounterInformation[i].CounterTargetID == m_TDA.m_numericvalue1
				&& currentquest->m_ItemCounterInformation[i].CountTotalPoint == m_TDA.m_numericvalue2 )
			{
				bexist = true;
				break;
			}
		}
		
		if( !bexist )
		{	
			for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
			{
				if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == -1 )
				{	
					// 카운터 세팅
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType = (m_TDA.m_actiondata1-8); // 타입
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID = m_TDA.m_numericvalue1;   // 아이디 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint = m_TDA.m_numericvalue2;	 // 총 개수 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountCurrentPoint = 0;
					
					if( m_lpszMessageString )
					{
						int strlength = strlen(m_lpszMessageString);
						
						if(strlength > 0)
						{
							memset( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256 );
							strcpy( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, m_lpszMessageString );			
						}		
						else
						{			
							char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
							short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
							
							memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
							strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
						}	
					}
					else
					{
						char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
						short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
						
						memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
						strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );			
						
					}
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(TCHAR)*256);
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}

				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();	
					}
				#endif
					
					if( itemtotalcount != 0 )
					{
						if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint != 0 )
						{
							if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									itemtotalcount,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
							}
							else if( itemtotalcount >= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (완료)")
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
							}
							
							if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								if( buffer[0] != 0 )
								{
									int strlength = strlen(buffer);
									
									if(g_QuestScriptManager.m_lpszQuestSystemMessage )
									{
										delete g_QuestScriptManager.m_lpszQuestSystemMessage;
										g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
									}
									
									if(strlength > 0)
									{
										g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
										g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
										strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
									}
									
									g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
									g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
								}		
							}				
						}
						else
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, itemtotalcount);

							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									SAFE_DELETE(g_QuestScriptManager.m_lpszQuestSystemMessage);
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}
					}
					
					
					break;
				}
				
			}
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStopCounter(void)// 카운터 멈추기
{
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_ACTION_RESULT_FAIL;

	
	currentquest->ResetCounterInformation();	
	currentquest->ResetPartyCounterInformation();
	currentquest->ResetItemCounterInformation();


	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetSystemMessage(void)// 시스템메세지출력
{
	// send message
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{		
		if(m_lpszMessageString)
		{
			g_NetworkKernel.InsertChatString( m_lpszMessageString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}		
	}	
	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetNPCOnOff(void)// NPC 생성 & 삭제
{
	//	m_TDA.m_actiondata1		= m_indexTargetClass; 0:ON 1:OFF
	//	m_TDA.m_numericvalue1	= m_NPCID;

	for( int _i = 0; _i < g_Npclist.m_NPCObjectCount; ++_i )
	{			
		if( g_Npclist.m_pNPCObjectArray[_i].m_UniqueID == m_TDA.m_numericvalue1 )
		{
			if( g_Npclist.m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
			{
				if( m_TDA.m_actiondata1 == 0 ) // ON
				{
					g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = TRUE;
				}
				else // OFF
				{
					g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE;
				}
				break;
			}	
		}
	}
	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSendQuestMail(void)// 풍운방 보내기
{
	//	m_TDA.m_actiondata1 = m_index1; // 종류 0:확인 1:수락or거부
	//	m_TDA.m_actiondata2 = m_index2; // 스킨 0:일반풍운방 1:튜토리얼풍운방
	//	m_TDA.m_numericvalue1	= m_Seconds;  // 지연시간 
	
	if(!m_lpszMessageString)
		return QUEST_ACTION_RESULT_ERROR;
	
	if( m_TDA.m_numericvalue1 == 0 )
	{
		_XWindow_QuestMail* pQuestMail_Window= _XWindow_QuestMail::CreateQuestMail( &g_MainWindowManager, m_nOwnerQuestID, m_TDA.m_actiondata1 );
		if( pQuestMail_Window )
		{
			if( m_TDA.m_actiondata1 == 0 )
				pQuestMail_Window->SetMode( _XQUESTMAILMODE_OK );
			else
			{ //Author : 양희왕 //breif: 튜토리얼 시 예 아니오 로 나오게
				if( m_nOwnerTriggerID == 2 )
					pQuestMail_Window->SetMode( _XQUESTMAILMODE_TUTORIALONLY );
				else
					pQuestMail_Window->SetMode( _XQUESTMAILMODE_YESNO );
			}
			
			pQuestMail_Window->SetMessage( m_lpszMessageString );
		}
	}
	else
	{		
		_XQUESTMAIL_INFO questmailbuffer;
		memset(&questmailbuffer, 0, sizeof(_XQUESTMAIL_INFO));
		questmailbuffer.questid	= m_nOwnerQuestID;
		questmailbuffer.questmailtype = m_TDA.m_actiondata1+2;// 종류 0:확인 1:수락or거부 (2이상은 시작조건이 아닌 풍운방) 2:확인 3:수락or거부
		questmailbuffer.resendstarttime = g_LocalSystemTime;
		questmailbuffer.delaytime = m_TDA.m_numericvalue1*1000;
		g_QuestScriptManager.m_listQuestMailBuffer.push_back(questmailbuffer);	
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessHoldItem(void)// 아이템장착해제불가설정
{
	//m_TDA.m_actiondata1  = m_ItemClass;	
	//m_TDA.m_actiondata2  = m_TargetClass;	//0:아이템고정시작 1:아이템고정끝
	//m_TDA.m_numericvalue1  = m_ItemID;
	
	if( m_TDA.m_actiondata2 == 0 )//0:아이템고정시작
	{
		if( !g_QuestScriptManager.m_listHoldItem.empty() )
		{
			_XQUEST_HOLDITEM	questholditem;
			memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
			questholditem.m_cHoldItemType = m_TDA.m_actiondata1;
			questholditem.m_sHoldItemID = m_TDA.m_numericvalue1;
			g_QuestScriptManager.m_listHoldItem.push_back(questholditem);
		}
		else
		{
			// 먼저 아이템 고정 리스트에 존재하는지 검사
			bool alreadyexist = false;
			list <_XQUEST_HOLDITEM>::iterator iter_holditem;
			for(iter_holditem = g_QuestScriptManager.m_listHoldItem.begin() ; iter_holditem != g_QuestScriptManager.m_listHoldItem.end() ; )
			{
				_XQUEST_HOLDITEM questholditem = *iter_holditem;
				if( questholditem.m_cHoldItemType == m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == m_TDA.m_numericvalue1 )
				{
					alreadyexist = true;
					break;
				}
				else
				{
					iter_holditem++;
				}										
			}
			
			// 존재하고 있지 않다면 리스트에 추가
			if( !alreadyexist )
			{
				_XQUEST_HOLDITEM	questholditem;
				memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
				questholditem.m_cHoldItemType = m_TDA.m_actiondata1;
				questholditem.m_sHoldItemID = m_TDA.m_numericvalue1;
				g_QuestScriptManager.m_listHoldItem.push_back(questholditem);
			}
		}
	}
	else//1:아이템고정끝
	{
		if( !g_QuestScriptManager.m_listHoldItem.empty() )
		{
			list <_XQUEST_HOLDITEM>::iterator iter_holditem;
			for(iter_holditem = g_QuestScriptManager.m_listHoldItem.begin() ; iter_holditem != g_QuestScriptManager.m_listHoldItem.end() ; )
			{
				_XQUEST_HOLDITEM questholditem = *iter_holditem;
				if( questholditem.m_cHoldItemType == m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == m_TDA.m_numericvalue1 )
				{
					iter_holditem = g_QuestScriptManager.m_listHoldItem.erase(iter_holditem);
					break;
				}
				else
				{
					iter_holditem++;
				}										
			}
		}
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessPartyMessage(void)// 파티원_메세지출력
{
	//m_TDA.m_actiondata1  = m_TargetClass2;	// 0:행수 1:행원 2:MVP 3:MVP외행원	
	//m_TDA.m_actiondata2  = m_TargetClass;		// 0: 시스템메세지 1:혼잣말하기
	
	bool bsystemmessage = false;
	bool bshowmessage	= false;

	if( m_TDA.m_actiondata2 == 0 )
	{
		bsystemmessage = true;
	}

	switch( m_TDA.m_actiondata1 ) {
	case 0://행수 
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				bshowmessage = true;
		}
		break;
	case 1://행원
		{
			if( !g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				bshowmessage = true;
		}
		break;
	case 2://MVP
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isMVP )
				bshowmessage = true;
		}
		break;
	case 3://MVP외 동행원
		{
			if( !g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isMVP )
				bshowmessage = true;
		}
		break;		
	}
	
	if(m_lpszMessageString)
	{
		if( bshowmessage )
		{
			if( bsystemmessage )//시스템 메세지일때 
			{
					g_NetworkKernel.InsertChatString( m_lpszMessageString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			else//혼잣말일때
			{
					g_pLocalUser->m_ChatBalloonObject.m_MessageColor = g_ChattingStringColor[7];//귓속말 색깔
					g_pLocalUser->m_ChatBalloonObject.SetMessage(m_lpszMessageString);		
			}
		}
	}
	
	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessDisplayQuestRanking(void)// 특정퀘스트랭킹보여주기
{
	//scriptdata->m_TDA.m_actiondata1		= m_TargetClass; // 0:생성 1:삭제
	//scriptdata->m_TDA.m_numericvalue1	= m_QuestID;
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartPartyItemCounter(void)// 파티아이템카운터시작하기
{        
//	m_TDA.m_actionclass = ID_ACTION_STARTPARTYITMECOUNTER;
//	m_TDA.m_actiondata1  = m_indexCountType;                    // 0:파티A타입 1:파티B타입 2:파티C타입 3:파티D타입
//	m_TDA.m_actiondata2  = m_indexItemClass;                    // 아이템 분류 
//	m_TDA.m_numericvalue1  = m_ItemID;							// 아이템 아이디
//	m_TDA.m_numericvalue2  = m_Count;                           // 총 갯수 
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}

	if( currentquest )
	{
		int countertype = m_TDA.m_actiondata1;

		// 카운터 세팅
		currentquest->m_PartyCounterInformation[countertype].CounterType		= countertype;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetType	= m_TDA.m_actiondata2;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetID	= m_TDA.m_numericvalue1;
		currentquest->m_PartyCounterInformation[countertype].CountTotalPoint	= m_TDA.m_numericvalue2;
		currentquest->m_PartyCounterInformation[countertype].CountCurrentPoint	= 0;

		if( m_lpszMessageString )
		{
			int strlength = strlen(m_lpszMessageString);
			
			if(strlength > 0)
			{
				memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, m_lpszMessageString );			
			}		
			else
			{			
				char cType = currentquest->m_PartyCounterInformation[countertype].CounterTargetType;
				short sID = currentquest->m_PartyCounterInformation[countertype].CounterTargetID;
				
				memset(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
				strcpy(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
			}	
		}
		else
		{
			char cType = currentquest->m_PartyCounterInformation[countertype].CounterTargetType;
			short sID = currentquest->m_PartyCounterInformation[countertype].CounterTargetID;
			
			memset(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
			strcpy(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGameItem::GetItemName(cType, sID) );			
			
		}
		
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetVirtualMode(void)// 가상공간 설정하기
{
	//scriptdata->m_TDA.nActionData1  = m_index1;		// 가상공간설정
	// 가상공간들어가기(다른지역다른위치), 가상공간들어가기(현재지역현재위치),가상공간들어가기(현재지역다른위치),
	// 가상공간나가기(이전지역원래위치), 가상공간나가기(이전지역다른위치), 가상공간나가기(현재지역현재위치),가상공간나가기(현재지역다른위치),
	// 가상공간나가기(다른지역다른위치)
	//scriptdata->m_TDA.nActionData2  = m_index2;		// 영역카테고리
	//scriptdata->m_TDA.nNumericValue1  = m_value1;		// 맵인덱스
	//scriptdata->m_TDA.nNumericValue2  = m_value2;		// 영역아이디
	//scriptdata->m_TDA.nNumericValue3  = m_value3;		// 영역 X
	//scriptdata->m_TDA.nNumericValue4  = m_value4;		// 영역 Z

	switch( m_TDA.m_actiondata1 ) {
	case 0: // 가상공간들어가기(다른지역다른위치) 맵로딩
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_ENTER_OTHERZONE;
			g_QuestScriptManager.m_indexVirtualZone = m_TDA.m_numericvalue1;
			g_QuestScriptManager.m_nVirtualZoneStartPositionX = m_TDA.m_numericvalue3;
			g_QuestScriptManager.m_nVirtualZoneStartPositionZ = m_TDA.m_numericvalue4;
			//가상공간으로 들어가기전에 있던 지역인덱스를 저장한다.
			g_QuestScriptManager.m_indexRealZone = g_CurrentZoneInfoIndex;
			//가상공간으로 들어가기 전에 있던위치를 저장한다.
			g_QuestScriptManager.m_nRealZoneStartPositionX = g_pLocalUser->m_Position.x;
			g_QuestScriptManager.m_nRealZoneStartPositionZ = g_pLocalUser->m_Position.z;
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_enter_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 1:// 가상공간들어가기(현재지역현재위치)
		{
			// 서버에 현재 지역에서 가상공간이 되도록 세팅한다.
			// 가상공간에서는 다른 유저의 정보와 몬스터 정보등이 오지 않는다.			
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_ENTER_CURRENTZONE;
			g_Moblist.disposeList();
			g_Userlist.disposeList();
			g_GameItemlist.disposeList();
			
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_enter_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 2:// 가상공간들어가기(현재지역다른위치)
		{
			// 서버에 현재 지역에서 가상공간이 되도록 세팅한다.
			// 가상공간에서는 다른 유저의 정보와 몬스터 정보등이 오지 않는다.			
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_ENTER_CURRENTZONEOTHERPOSITION;
			
			_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata2, m_TDA.m_numericvalue2 );
			if( pArea )
			{				
				g_QuestScriptManager.m_nVirtualZoneStartPositionX = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
				g_QuestScriptManager.m_nVirtualZoneStartPositionZ = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
			}

			//가상공간으로 들어가기 전에 있던위치를 저장한다.
			g_QuestScriptManager.m_nRealZoneStartPositionX = g_pLocalUser->m_Position.x;
			g_QuestScriptManager.m_nRealZoneStartPositionZ = g_pLocalUser->m_Position.z;

			g_Moblist.disposeList();
			g_Userlist.disposeList();
			g_GameItemlist.disposeList();
			
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_enter_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 3:// 가상공간나가기(이전지역원래위치) 맵로딩
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONE;
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 4:// 가상공간나가기(이전지역다른위치) 맵로딩
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONEOTHERPOSITION;
			g_QuestScriptManager.m_nRealZoneStartPositionX = m_TDA.m_numericvalue3;
			g_QuestScriptManager.m_nRealZoneStartPositionZ = m_TDA.m_numericvalue4;

			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 5:// 가상공간나가기(현재지역현재위치)
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_CURRENTZONE;

			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 6:// 가상공간나가기(현재지역다른위치)
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_CURRENTZONEOTHERPOSITION;
			_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata2, m_TDA.m_numericvalue2 );
			if( pArea )
			{				
				g_QuestScriptManager.m_nRealZoneStartPositionX = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
				g_QuestScriptManager.m_nRealZoneStartPositionZ = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
			}

			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 7:// 가상공간나가기(다른지역다른위치)
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_OTHERZONEOTHERPOSITION;
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req,m_TDA.m_numericvalue1,m_TDA.m_actiondata2,m_TDA.m_numericvalue2);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	}
	return QUEST_ACTION_RESULT_FAIL;
}