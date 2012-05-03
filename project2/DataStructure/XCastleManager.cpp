// XCastleManager.cpp: implementation of the _XCastleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XCastleManager.h"
#include "XWindow_ThreatenGauge.h"
#include "XWindow_CastleBattleShout.h"
#include "XWindow_CastleBattlePersonalHistory.h"
#include "XSR_STRINGHEADER.H"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XCastleManager::_XCastleManager()
{
	m_enWarState		= _XEN_CB_NONE;
	m_nMapId			= 0;
	m_pScheduler		= NULL;

	m_dwTimeFromServer		= 0;
	m_dwThreatenStartTime	= 0;
	m_fThreatenPercentage	= 0;
	m_dwThreatenRequestTime = 0;
	m_bSuccessThreaten		= FALSE;
	
	m_nSavePointFailResult	= 0;

	ClearState();
}

_XCastleManager::~_XCastleManager()
{
	Release();
}

void _XCastleManager::Release(void)
{
	map <int, TCHAR*>::iterator iter_name;
	for(iter_name = m_mManorScriptName.begin() ; iter_name != m_mManorScriptName.end() ; ++iter_name)
	{
		TCHAR* szScrName = iter_name->second;
		SAFE_DELETE_ARRAY(szScrName);
	}
	m_mManorScriptName.clear();

	map <int, _XCastleInfo*>::iterator iter_info;
	for(iter_info = m_mCastleInfo.begin() ; iter_info != m_mCastleInfo.end() ; ++iter_info)
	{
		_XCastleInfo* pCastleInfo = iter_info->second;
		if(pCastleInfo)
		{
			pCastleInfo->Release();
			SAFE_DELETE(pCastleInfo);
		}
	}
	m_mCastleInfo.clear();

	map <int, _XCastleScheduler*>::iterator iter_schedule;
	for(iter_schedule = m_mCBScheduler.begin() ; iter_schedule != m_mCBScheduler.end() ; ++iter_schedule)
	{
		_XCastleScheduler* pScheduler = iter_schedule->second;
		if(pScheduler)
		{
			pScheduler->Release();
			SAFE_DELETE(pScheduler);
		}
	}
	m_mCBScheduler.clear();
}

void _XCastleManager::ReleaseCastleInfo()
{
	map <int, _XCastleInfo*>::iterator iter_info;
	for(iter_info = m_mCastleInfo.begin() ; iter_info != m_mCastleInfo.end() ; ++iter_info)
	{
		_XCastleInfo* pCastleInfo = iter_info->second;
		if(pCastleInfo)
		{
			pCastleInfo->Release();
			SAFE_DELETE(pCastleInfo);
		}
	}
	m_mCastleInfo.clear();
}

BOOL _XCastleManager::Load(void)
{
	// load 전체 장원전 정보 스크립트	
	if(!g_ScriptArchive.ReOpenPackage())
		return FALSE;
	
	TCHAR filename[64];
	memset( filename, 0, sizeof(TCHAR) * 64 );
	strcpy( filename, _T("manor_map_script.txt") );
	
	FILE* fileptr = NULL;
	fileptr = g_ScriptArchive.GetPackedFile( filename );
	
	if( !fileptr )
	{
		_XFatalError("Can't load manor_map_script");
		g_ScriptArchive.ClosePackage();		
		return FALSE;
	}

	TCHAR cmd[256];
	int mapid = 0;
	BOOL bEnd = FALSE, bComplete = FALSE;

	while(1)
	{
		while(GetCommand(cmd, fileptr))
		{
			switch(cmd[0])
			{
			case 'm' :
				{
					if(strncmp(cmd, "manor_map_id", sizeof("manor_map_id")) == 0)
					{
						if(!ParseManorMapId(fileptr))
							bEnd = TRUE;
					}
					else
					{
						ParseSkipInfo(fileptr);
					}
				}
				break;
			case 'c' :
				{
					if(strncmp(cmd, "commonzone_tax", sizeof("commonzone_tax")) == 0)
					{
					}
					else
					{
						ParseSkipInfo(fileptr);
					}
				}
				break;
			case '_' :
				{
					if(strncmp(cmd, "_XM_EOS", strlen("_XM_EOS")) == 0)
					{
						bEnd = TRUE;
						bComplete = TRUE;
					}
				}
				break;
			}

			if(bEnd)
				break;
		}

		if(bEnd)
			break;
	}

	if(!bComplete)
	{
		Release();
	}

	g_ScriptArchive.ClosePackage();

	if(bComplete)
	{
		if(!LoadCastleScheduler())
			return FALSE;
	}

	return bComplete;
}

BOOL _XCastleManager::LoadCastleScheduler(void)
{
	map <int, TCHAR*>::iterator iter_name;
	for(iter_name = m_mManorScriptName.begin() ; iter_name != m_mManorScriptName.end() ; ++iter_name)
	{
		int mapid = iter_name->first;
		TCHAR* szScrName = iter_name->second;
		if(szScrName)
		{
			_XCastleScheduler* pCBScheduler = new _XCastleScheduler;
			if(!pCBScheduler->Load(szScrName))
			{
				SAFE_DELETE(pCBScheduler);
				return FALSE;
			}

			m_mCBScheduler[mapid] = pCBScheduler;
		}
	}

	return TRUE;
}

BOOL _XCastleManager::ParseManorMapId(FILE* fp)
{
	char cmd[256];
	char arg1[64], arg2[256];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					memset(arg1, 0, sizeof(arg1));
					memset(arg2, 0, sizeof(arg2));

					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						int mapid = atoi(arg1);

						int strlength = strlen(arg2);
						TCHAR* scrname = new TCHAR[strlength+1];
						scrname[strlength] = 0;
						strncpy(scrname, arg2, strlength);

						m_mManorScriptName[mapid] = scrname;
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleManager::ParseSkipInfo(FILE* fp)
{
	char cmd[256];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

void _XCastleManager::Reset(void)
{
	m_enWarState			= _XEN_CB_NONE;
	m_dwTimeFromServer		= 0;
	m_dwThreatenStartTime	= 0;
	m_fThreatenPercentage	= 0;
	m_dwThreatenRequestTime = 0;
	m_bSuccessThreaten		= FALSE;
	m_nProgress				= 80;
	m_nSavePointFailResult	= 0;
	ClearState();

	_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
	if(pCBPersonalHistory_Window)
	{
		pCBPersonalHistory_Window->SetPoint(0);
	}
}

void _XCastleManager::ClearState(void)
{
	m_bMsgDone			= FALSE;
	m_bBGMDone			= FALSE;
	m_bSEDone			= FALSE;
	m_bShoutDone		= FALSE;
	m_bGateDone			= FALSE;
	m_bThreatenDone		= FALSE;
	m_bLightFootDone	= FALSE;
	m_bAnimationDone	= FALSE;
	m_bEffectDone		= FALSE;
	m_bBuffDone			= FALSE;
	
	m_dwBGMStartTime	= 0;
	m_dwStateTotalBGMStartTime = 0;
	m_dwSEStartTime		= 0;
}

void _XCastleManager::Draw(void)
{
	if(m_bDrawOwnerMessage)
	{
		DWORD elapsedtime = g_LocalSystemTime - m_dwOwnerMessageStartTime;
		if(elapsedtime <= 10000)	// 10초 동안 보여줌
		{
			TCHAR messagestr1[512];
			memset(messagestr1, 0, sizeof(messagestr1));

			TCHAR messagestr2[512];
			memset(messagestr2, 0, sizeof(messagestr2));

			_XCastleInfo* pCastleInfo = GetCastleInfo(g_CurrentZoneInfoIndex);
			if(pCastleInfo)
			{
				if(pCastleInfo->m_tOwner.nOrIndex == 0)
				{
					// 몬스터 소유
					_snprintf(messagestr1, sizeof(messagestr1), _XGETINTERFACETEXT(ID_STRING_NEW_3768), 
						g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename, 
						_XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(g_CurrentZoneInfoIndex)));//%s지역의 %s은 외도세력이 점령하고 있습니다.
				}
				else
				{
					// 유저 소유
					if(pCastleInfo->m_tOwner.nType == 0)
					{
						// 단
						_snprintf(messagestr1, sizeof(messagestr1), _XGETINTERFACETEXT(ID_STRING_NEW_3769),
							g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename, 
							_XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(g_CurrentZoneInfoIndex)),
							pCastleInfo->m_tOwner.szOrgName);//"%s지역의 %s는 %s단이 점령하고 있습니다."

						_snprintf(messagestr2, sizeof(messagestr2), _XGETINTERFACETEXT(ID_STRING_NEW_3771), pCastleInfo->m_tOwner.szOrgName);//%s단 세력은 경험치 증가 혜택을 받을 수 있습니다.
					}
					else
					{
						// 회
						_snprintf(messagestr1, sizeof(messagestr1), _XGETINTERFACETEXT(ID_STRING_NEW_3770),
							g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename, 
							_XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(g_CurrentZoneInfoIndex)),
							pCastleInfo->m_tOwner.szOrgName);//"%s지역의 %s는 %s회가 점령하고 있습니다."

						_snprintf(messagestr2, sizeof(messagestr2), _XGETINTERFACETEXT(ID_STRING_NEW_3772), pCastleInfo->m_tOwner.szOrgName);//%s회 세력은 경험치 증가 혜택을 받을 수 있습니다.
					}
				}
			}

			g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
			g_XLargeFont->SetColor(D3DCOLOR_ARGB(255, 250, 230, 169));

			if(messagestr1[0] != 0)
				g_XLargeFont->Puts_SeparateAlign((gnWidth>>1) , (gnHeight>>1)-145-30, messagestr1, 500, _XFONT_ALIGNTYPE_CENTER);

			if(messagestr2[0] != 0)
				g_XLargeFont->Puts_SeparateAlign((gnWidth>>1) , (gnHeight>>1)-145-10, messagestr2, 500, _XFONT_ALIGNTYPE_CENTER);

			g_XLargeFont->Flush();
		}
		else
		{
			// 10초 지났음
			if(m_bDrawOwnerMessage)
				m_bDrawOwnerMessage = FALSE;
		}
	}
}

void _XCastleManager::Process(void)
{
	if(!g_pLocalUser->m_bCastleBattle)		// 장원전에 참가하지 않은 경우 skip
		return;

	if(!m_pScheduler)
		return;

	// 현재 맵의 장원전을 의미한다.

#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
	ProcessBGM();
	ProcessSE();
#endif

	if(!m_bMsgDone)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->bMsg)
		{
			// msg
			ProcessAlarmMsg();
		}
		m_bMsgDone = TRUE;
	}
	if(!m_bBGMDone)
	{
#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
#else
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->nPlayBGM == 1)
		{
			// BGM
			if( g_BgmPlayFlag )
			{
				g_FMODWrapper.StopBGM();
				g_FMODWrapper.PlayBGM( m_pScheduler->m_mCastleScheduler[m_enWarState]->szBGMFileName );

				m_dwBGMStartTime = g_LocalSystemTime;
			}
		}
#endif
		m_bBGMDone = TRUE;
	}
	if(!m_bShoutDone)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->dwShout == 1)
		{
			// 장원전 사자후 on
			// show window
			if(g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				_XWindow_CastleBattleShout* pCastleBattleShout_Window = (_XWindow_CastleBattleShout*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_SHOUT);
				if(pCastleBattleShout_Window)
				{
					pCastleBattleShout_Window->ShowWindow(TRUE);
				}
			}
		}
		else if(m_pScheduler->m_mCastleScheduler[m_enWarState]->dwShout == 2)
		{
			// close window
			if(g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				_XWindow_CastleBattleShout* pCastleBattleShout_Window = (_XWindow_CastleBattleShout*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_SHOUT);
				if(pCastleBattleShout_Window)
				{
					pCastleBattleShout_Window->ShowWindow(FALSE);
				}
			}
		}
		m_bShoutDone = TRUE;
	}
	if(!m_bGateDone)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->dwGateState == 1)
		{
			// gate state on - fuctional object 선행
			// functionalobjectConditionItem.cpp에서 작업 할 것.
		}
		m_bGateDone = TRUE;
	}
	if(!m_bThreatenDone)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->dwThreaten == 1)
		{
			// Threaten state on - fuctional object 선행
			// functionalobjectConditionItem.cpp에서 작업 할 것 - 장주 앞 gate 처리
		}
		m_bThreatenDone = TRUE;
	}
	if(!m_bLightFootDone)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->dwLightFoot == 1)
		{
			// 경공 불가 처리 - 장원안에 있을 때만 (영역 처리 선행)
		}
		m_bLightFootDone = TRUE;
	}
	if(!m_bAnimationDone)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->bAni)
		{
			// animation 적용
			ProcessAnimation();
		}
		m_bAnimationDone = TRUE;
	}
	if(!m_bEffectDone)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->bVE)
		{
			// effect - 장원전 내에 있을 때 처리 필요
			ProcessVisualEffect();
		}
		m_bEffectDone = TRUE;
	}

	// state 변경
	if(m_enWarState >= _XEN_CB_READY && m_enWarState < _XEN_CB_PROGRESS)
	{
		map <int, _XCastle_Scheduler*>::iterator iter_schedule = m_pScheduler->m_mCastleScheduler.find(m_enWarState + 1);
		if(iter_schedule != m_pScheduler->m_mCastleScheduler.end())
		{
			// 다음 state 있음
			//시간 계산 후 시간이 맞다면
			if(g_LocalSystemTime >= (m_dwTimeFromServer + (m_pScheduler->m_mCastleScheduler[_XEN_CB_READY]->nTime - m_pScheduler->m_mCastleScheduler[m_enWarState+1]->nTime)))
			{
				m_enWarState = (_XCastle_State)(m_enWarState + 1);
				ClearState();
				_XDWINPRINT("[CB/Process] State Change : %d / %d", m_enWarState, m_pScheduler->m_mCastleScheduler[m_enWarState]->nTime);
				_XLog("[CB/Process] State Change : %d", m_enWarState);
			}
		}
	}
/*	else if(m_enWarState == _XEN_CB_START)
	{
		// 시간 계산 후 시간이 맞으면
		if(g_LocalSystemTime >= (m_dwTimeFromServer + m_pScheduler->m_mCastleScheduler[_XEN_CB_PROGRESS]->nTime))
		{
			m_enWarState = _XEN_CB_PROGRESS;
			ClearState();
			_XDWINPRINT("[CB/Process] State Change : %d", m_enWarState);
		}
	}*/
	else if(m_enWarState >= _XEN_CB_PROGRESS && m_enWarState < _XEN_CB_END)
	{
		map <int, _XCastle_Scheduler*>::iterator iter_schedule = m_pScheduler->m_mCastleScheduler.find(m_enWarState + 1);
		if(iter_schedule != m_pScheduler->m_mCastleScheduler.end())
		{
			// 다음 state 있음
			//시간 계산 후 시간이 맞다면
			if(g_LocalSystemTime >= (m_dwTimeFromServer + m_pScheduler->m_mCastleScheduler[m_enWarState+1]->nTime))
			{
				m_enWarState = (_XCastle_State)(m_enWarState + 1);
				ClearState();
				_XDWINPRINT("[CB/Process] State Change : %d", m_enWarState);
				_XLog("[CB/Process] State Change : %d", m_enWarState);
			}
		}
	}
	else if(m_enWarState >= _XEN_CB_END)
	{
		map <int, _XCastle_Scheduler*>::iterator iter_schedule = m_pScheduler->m_mCastleScheduler.find(m_enWarState + 1);
		if(iter_schedule != m_pScheduler->m_mCastleScheduler.end())
		{
			// 다음 state 있음
			//시간 계산 후 시간이 맞다면
			if(g_LocalSystemTime >= (m_dwTimeFromServer + m_pScheduler->m_mCastleScheduler[m_enWarState+1]->nTime))
			{
				m_enWarState = (_XCastle_State)(m_enWarState + 1);
				ClearState();
				_XDWINPRINT("[CB/Process] State Change : %d", m_enWarState);
				_XLog("[CB/Process] State Change : %d", m_enWarState);
				
				int i = GetGateState();
			}
		}
		else
		{
			// 끝났음
			Reset();
			g_pLocalUser->ResetCastleBattleInfo();
			_XDWINPRINT("[CB/Process] Complete");
			_XLog("[CB/Process] Complete");
		}
	}
	
	// 협박 시간 체크
	if(m_dwThreatenStartTime != 0)
	{
 		m_fThreatenPercentage = ((g_LocalSystemTime - m_dwThreatenStartTime)/((FLOAT)m_pScheduler->m_nThreatenTime*1000.f))*100.0f;

		if(g_pLocalUser->m_dwRoleInCastleBattle == 1)
		{
			if(m_fThreatenPercentage >= (FLOAT)m_nProgress)
			{
				if(g_pLocalUser->m_bThreatening)
				{
					if(!m_bSuccessThreaten)
					{
						if(!g_pLocalUser->m_bSendThreatenPacket)
						{
							// 실패 이면 80%일때 cancel packet 보내기
							g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_threat_cancel_req);
						}
						m_fThreatenPercentage = (FLOAT)m_nProgress;
					}
				}
			}
			if(m_fThreatenPercentage >= 100.0f)
			{
				// 끝 패킷 
				if(g_pLocalUser->m_bThreatening)
				{
					if(!g_pLocalUser->m_bSendThreatenPacket)
					{
						g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_threat_complete_req);
					}
				}

				m_fThreatenPercentage = 100.0f;
			}
		}

		_XWindow_ThreatenGauge* pThreatenGauge_Window = (_XWindow_ThreatenGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_THREATENGAUGE);
		if(pThreatenGauge_Window)
		{
			pThreatenGauge_Window->SetParameter(m_fThreatenPercentage);
		}
		_XWindow_CastleBattlePersonalHistory* pCBPersonalHistory_Window = (_XWindow_CastleBattlePersonalHistory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_PERSONALHISTORY);
		if(pCBPersonalHistory_Window)
		{
			pCBPersonalHistory_Window->SetThreatenParameter(m_fThreatenPercentage);
		}
	}
}

void _XCastleManager::ProcessAlarmMsg(void)
{
	BOOL bMsg = FALSE;

	vector <_XCastle_AlarmMsg*>::iterator iter_msg;
	for(iter_msg = m_pScheduler->m_mCastleScheduler[m_enWarState]->tAlarmMsg.begin() ; iter_msg != m_pScheduler->m_mCastleScheduler[m_enWarState]->tAlarmMsg.end() ; ++iter_msg)
	{
		_XCastle_AlarmMsg* pMsg = *iter_msg;
		if(pMsg)
		{
			bMsg = GetTargetType(pMsg->dwAlarmMsgTarget);

			if(bMsg)
			{
				if(pMsg->dwAlarmMsgType == 1 || pMsg->dwAlarmMsgType == 2)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(pMsg->nTextIndex), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(pMsg->nTextIndex), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				}
				bMsg = FALSE;

				_XLog(_XGETINTERFACETEXT(pMsg->nTextIndex));
			}
		}
	}
}

void _XCastleManager::ProcessAnimation(void)
{
	BOOL bAnimation = FALSE;

	vector <_XCastle_Animation*>::iterator iter_ani;
	for(iter_ani = m_pScheduler->m_mCastleScheduler[m_enWarState]->tAnimation.begin() ; iter_ani != m_pScheduler->m_mCastleScheduler[m_enWarState]->tAnimation.end() ; ++iter_ani)
	{
		_XCastle_Animation* pAni = *iter_ani;
		if(pAni)
		{
			bAnimation = GetTargetType(pAni->nAnimationTarget);

			if(bAnimation)
			{
				// 맨손, 단병기, 장병기 처리 필요 - 연결 animation 처리?
				if(g_pLocalUser->GetMotionClass() != _XACTION_DYING 
					&& g_pLocalUser->GetMotionClass() != _XACTION_DIE 
					&& g_pLocalUser->GetMotionClass() != _XACTION_REBIRTH 
					&& !g_pLocalUser->m_bOut)
				{
					if(g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN)
						g_pLocalUser->SetAnimationIndex(pAni->nAnimationIndex_male);
					else
						g_pLocalUser->SetAnimationIndex(pAni->nAnimationIndex_female);
				}

				bAnimation = FALSE;
			}
		}
	}
}

void _XCastleManager::ProcessVisualEffect(void)
{
	BOOL bEffect = FALSE;

#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
	int effectcount = m_pScheduler->m_mCastleScheduler[m_enWarState]->tVisualEffect.size();

	if(effectcount <= 0)
		return;

	int selectednumber = _XRandom() % effectcount;

	_XCastle_VisualEffect* pVE = m_pScheduler->m_mCastleScheduler[m_enWarState]->tVisualEffect[selectednumber];
	if(pVE)
	{
		bEffect = GetTargetType(pVE->dwVisualEffectTarget);
		if(bEffect)
		{
			D3DXMATRIX offsetmat;
			D3DXMatrixIdentity(&offsetmat);
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
				pVE->dwVisualEffectId, &offsetmat); //, NULL, TRUE);

			bEffect = FALSE;

			_XDWINPRINT("[CB/ProcessVE] selectednumber:%d  id:%d", selectednumber, pVE->dwVisualEffectId);
		}
	}

#else
	vector <_XCastle_VisualEffect*>::iterator iter_ve;
	for(iter_ve = m_pScheduler->m_mCastleScheduler[m_enWarState]->tVisualEffect.begin() ; iter_ve != m_pScheduler->m_mCastleScheduler[m_enWarState]->tVisualEffect.end() ; ++iter_ve)
	{
		_XCastle_VisualEffect* pVE = *iter_ve;
		if(pVE)
		{
			bEffect = GetTargetType(pVE->dwVisualEffectTarget);

			if(bEffect)
			{
				D3DXMATRIX offsetmat;
				D3DXMatrixIdentity(&offsetmat);
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
					pVE->dwVisualEffectId, &offsetmat); //, NULL, TRUE);

				bEffect = FALSE;
			}
		}
	}
#endif
}

void _XCastleManager::ProcessBGM(void)
{
#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
	int bgmcount = m_pScheduler->m_mCastleScheduler[m_enWarState]->tMainBGMList.size();

	if(bgmcount <= 0)
		return;

	int selectednumber = _XRandom() % bgmcount;

	_XCastle_MainBGM* pBGM = m_pScheduler->m_mCastleScheduler[m_enWarState]->tMainBGMList[selectednumber];
	if(pBGM)
	{
		if(m_dwStateTotalBGMStartTime == 0)
		{
			if(pBGM->nPlayBGM == 1)
			{
				g_FMODWrapper.StopBGM();
				g_FMODWrapper.PlayBGM(pBGM->szBGMFileName);
				m_dwBGMStartTime = g_LocalSystemTime;
				m_dwStateTotalBGMStartTime = g_LocalSystemTime;

				_XDWINPRINT("[CB/ProcessBGM/Start] selectednumber:%d  BGMName:%s", selectednumber, pBGM->szBGMFileName);
			}
		}

		// loop 처리
		if(pBGM->bLoop)
		{
			DWORD elapsedtime = g_LocalSystemTime - m_dwStateTotalBGMStartTime;
			if(elapsedtime < pBGM->nBGMEndTime*1000)
			{
				if(g_LocalSystemTime - m_dwBGMStartTime > 180000)
				{
					g_FMODWrapper.PlayBGM( pBGM->szBGMFileName );
					m_dwBGMStartTime = g_LocalSystemTime;

					_XDWINPRINT("[CB/ProcessBGM/Loop] selectednumber:%d  BGMName:%s", selectednumber, pBGM->szBGMFileName);
				}
			}
		}
	}

#else
	if(m_pScheduler->m_mCastleScheduler[m_enWarState]->nPlayBGM == 1)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->bLoop)
		{
			if(g_LocalSystemTime - m_dwBGMStartTime > 180000)
			{
#ifdef _XUSEFMOD
				g_FMODWrapper.PlayBGM( m_pScheduler->m_mCastleScheduler[m_enWarState]->szBGMFileName );
				m_dwBGMStartTime = g_LocalSystemTime;
#endif
			}
		}
	}
#endif
}

void _XCastleManager::ProcessSE(void)
{
	if(m_dwSEStartTime == 0)
	{
		if(m_pScheduler->m_mCastleScheduler[m_enWarState]->nSEId > 0)
		{
			_XPlayInterfaceSound(m_pScheduler->m_mCastleScheduler[m_enWarState]->nSEId);
			m_dwSEStartTime = g_LocalSystemTime;

			_XDWINPRINT("[CB/ProcessSE/Start] SEId:%d", m_pScheduler->m_mCastleScheduler[m_enWarState]->nSEId);
		}
	}

	if(m_pScheduler->m_mCastleScheduler[m_enWarState]->bSELoop)
	{
		DWORD elapsedtime = g_LocalSystemTime - m_dwSEStartTime;
		if(elapsedtime < m_pScheduler->m_mCastleScheduler[m_enWarState]->nSEEndTime*1000)
		{
			if(g_LocalSystemTime - m_dwSEStartTime > 3000)
			{
				_XPlayInterfaceSound(m_pScheduler->m_mCastleScheduler[m_enWarState]->nSEId);
				m_dwSEStartTime = g_LocalSystemTime;

				_XDWINPRINT("[CB/ProcessSE/Loop] SEId:%d", m_pScheduler->m_mCastleScheduler[m_enWarState]->nSEId);
			}
		}
	}
}

BOOL _XCastleManager::GetTargetType(int index)
{
	BOOL bResult = FALSE;

	switch(index)
	{
	case 0 :
		{
			// all
			bResult = TRUE;
		}
		break;
	case 1 :
		{
			if(g_pLocalUser->m_dwAttackType == 1)
			{
				// attacker
				bResult = TRUE;
			}
		}
		break;
	case 2 :
		{
			if(g_pLocalUser->m_dwAttackType == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// attacker-leader
				bResult = TRUE;
			}
		}
		break;
	case 3 :
		{
			if(g_pLocalUser->m_dwAttackType == 2)
			{
				// defender
				bResult = TRUE;
			}
		}
		break;
	case 4 :
		{
			if(g_pLocalUser->m_dwAttackType == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// defender-leader
				bResult = TRUE;
			}
		}
		break;
	case 5 :
		{
			// 장주를 협박하는 총대장
		}
		break;
	case 6 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1)
			{
				// winners
				bResult = TRUE;
			}
		}
		break;
	case 7 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// winners - leader
				bResult = TRUE;
			}
		}
		break;
	case 8 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2)
			{
				// losers
				bResult = TRUE;
			}
		}
		break;
	case 9 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// losers - leader
				bResult = TRUE;
			}
		}
		break;
	}
	return bResult;
}

void _XCastleManager::SetCurrentScheduler(int mapid)
{
	m_nMapId = mapid;

	map<int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		m_pScheduler = iter->second;
	}
	else
	{
		m_pScheduler = NULL;
	}
}

_XCastleInfo* _XCastleManager::GetCastleInfo(int mapid)
{
	map<int, _XCastleInfo*>::iterator iter = m_mCastleInfo.find(mapid);
	if(iter != m_mCastleInfo.end())
	{
		return iter->second;
	}
	return NULL;
}

int	_XCastleManager::GetCastleNameTextId(int mapid)
{
	map <int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		_XCastleScheduler* pScheduler = iter->second;
		if(pScheduler)
		{
			return pScheduler->m_nMapNameStrindex;
		}
	}

	return 0;
}

int _XCastleManager::GetDeadMaxCount(void)
{
	if(m_pScheduler)
		return m_pScheduler->m_nDeadMaxCnt;
	else
		return 0;
}

int _XCastleManager::GetPenaltyTime(DWORD level)
{
	vector <_XCastle_DiePenalty*>::iterator iter_die;
	for(iter_die = m_pScheduler->m_vecDiePenalty.begin() ; iter_die != m_pScheduler->m_vecDiePenalty.end() ; ++iter_die)
	{
		_XCastle_DiePenalty* penalty = *iter_die;
		if(penalty)
		{
			if(level >= penalty->nMinLevel && level <= penalty->nMaxLevel)
			{
				if(g_pLocalUser->m_dwAttackType == 1)
				{
					// attacker
					return penalty->nAttackerPenalty;
				}
				else
				{
					// defender
					return penalty->nDefenderPenalty;
				}
			}
		}
	}
	return 0;
}

int _XCastleManager::GetChallengeMoney(int mapid, int index)
{
	if(index < 0 || index >= 5)
		return 0;

	map <int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		_XCastleScheduler* pScheduler = iter->second;
		if(pScheduler)
		{
			return pScheduler->m_nChallengeMoney[index-1];
		}
	}

	return 0;
}

DWORD _XCastleManager::GetThreatenTime()
{
	if(m_pScheduler)
	{
		return m_pScheduler->m_nThreatenTime;
	}

	return 0;
}

DWORD _XCastleManager::GetThreatenDelayTime()
{
	if(m_pScheduler)
	{
		return m_pScheduler->m_nThreatenDelayTime;
	}

	return 0;
}

int _XCastleManager::GetShoutMaxCount()
{
	if(m_pScheduler)
	{
		return m_pScheduler->m_nMaxShoutCnt;
	}

	return 0;
}

DWORD _XCastleManager::GetGateState()
{
	if(!m_pScheduler)
		return 0;

	map <int, _XCastle_Scheduler*>::iterator iter = m_pScheduler->m_mCastleScheduler.find(m_enWarState);
	if(iter != m_pScheduler->m_mCastleScheduler.end())
	{
		DWORD state = 0;

		if(m_enWarState >= _XEN_CB_READY && m_enWarState < _XEN_CB_PROGRESS)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_READY]->dwGateState;
		}
		else if(m_enWarState >= _XEN_CB_PROGRESS && m_enWarState < _XEN_CB_END)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_PROGRESS]->dwGateState;
		}
		else if(m_enWarState >= _XEN_CB_END)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_END]->dwGateState;
		}

		return state;
	}

	return 0;
}

DWORD _XCastleManager::GetThreatenState()
{
	if(!m_pScheduler)
		return 0;

	map <int, _XCastle_Scheduler*>::iterator iter = m_pScheduler->m_mCastleScheduler.find(m_enWarState);
	if(iter != m_pScheduler->m_mCastleScheduler.end())
	{
		DWORD state = 0;

		if(m_enWarState >= _XEN_CB_READY && m_enWarState < _XEN_CB_PROGRESS)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_READY]->dwThreaten;
		}
		else if(m_enWarState >= _XEN_CB_PROGRESS && m_enWarState < _XEN_CB_END)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_PROGRESS]->dwThreaten;
		}
		else if(m_enWarState >= _XEN_CB_END)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_END]->dwThreaten;
		}

		return state;
	}

	return 0;
}

DWORD _XCastleManager::GetLightFootState()
{
	if(!m_pScheduler)
		return 0;

	map <int, _XCastle_Scheduler*>::iterator iter = m_pScheduler->m_mCastleScheduler.find(m_enWarState);
	if(iter != m_pScheduler->m_mCastleScheduler.end())
	{
		DWORD state = 0;

		if(m_enWarState >= _XEN_CB_READY && m_enWarState < _XEN_CB_PROGRESS)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_READY]->dwLightFoot;
		}
		else if(m_enWarState >= _XEN_CB_PROGRESS && m_enWarState < _XEN_CB_END)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_PROGRESS]->dwLightFoot;
		}
		else if(m_enWarState >= _XEN_CB_END)
		{
			state = m_pScheduler->m_mCastleScheduler[_XEN_CB_END]->dwLightFoot;
		}

		return state;
	}

	return 0;
}

int _XCastleManager::GetMinimapPosition()
{
	if(!m_pScheduler)
		return 0;

	return m_pScheduler->m_nMinimapPosition;
}

void _XCastleManager::SetScheduleList(int mapid, _sCastleInfo* castleinfo, DWORD money)
{
	map <int, _XCastleInfo*>::iterator iter = m_mCastleInfo.find(mapid);
	if(iter != m_mCastleInfo.end())
	{
		_XCastleInfo* pCastleInfo = iter->second;
		if(pCastleInfo)
		{
			pCastleInfo->SetScheduleList(castleinfo, money);
		}
	}
	else
	{
		_XCastleInfo* pCastleInfo = new _XCastleInfo;
		pCastleInfo->SetScheduleList(castleinfo, money);
		m_mCastleInfo[mapid] = pCastleInfo;
	}
}

void _XCastleManager::SetChallengeList(int mapid, __cb_client_challenge_info* info)
{
	map <int, _XCastleInfo*>::iterator iter = m_mCastleInfo.find(mapid);
	if(iter != m_mCastleInfo.end())
	{
		_XCastleInfo* pCastleInfo = iter->second;
		if(pCastleInfo)
		{
			pCastleInfo->SetChallengeList(info);
		}
	}
	else
	{
		_XCastleInfo* pCastleInfo = new _XCastleInfo;
		pCastleInfo->SetChallengeList(info);
		m_mCastleInfo[mapid] = pCastleInfo;
	}
}

void _XCastleManager::ResetChallengeList(int mapid)
{
	map <int, _XCastleInfo*>::iterator iter = m_mCastleInfo.find(mapid);
	if(iter != m_mCastleInfo.end())
	{
		_XCastleInfo* pCastleInfo = iter->second;
		if(pCastleInfo)
		{
			pCastleInfo->ResetChallengeList();
		}
	}
}

void _XCastleManager::ChangeTime(int mapid, int nth, _sND_TimeUnit* time)
{
	map <int, _XCastleInfo*>::iterator iter = m_mCastleInfo.find(mapid);
	if(iter != m_mCastleInfo.end())
	{
		_XCastleInfo* pCastleInfo = iter->second;
		if(pCastleInfo)
		{
			pCastleInfo->ChangeTime(nth, time);
		}
	}
}

FLOAT _XCastleManager::GetDefaultTax(int mapid, int step)
{
	map <int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		_XCastleScheduler* pCBScheduler = iter->second;
		if(pCBScheduler)
		{
			return pCBScheduler->m_tCastleTax[step].fDefaultTax;
		}
	}
	return 0;
}

FLOAT _XCastleManager::GetAdditiveTax(int mapid, int step)
{
	map <int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		_XCastleScheduler* pCBScheduler = iter->second;
		if(pCBScheduler)
		{
			return pCBScheduler->m_tCastleTax[step].fAdditiveTax;
		}
	}
	return 0;
}

void _XCastleManager::GetDefaultOpenTime(int mapid, int& first, int& second, int& third)
{
	map <int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		_XCastleScheduler* pCBScheduler = iter->second;
		if(pCBScheduler)
		{
			first = pCBScheduler->m_tDefaultOpenTime[0].dwHour;
			second = pCBScheduler->m_tDefaultOpenTime[1].dwHour;
			third = pCBScheduler->m_tDefaultOpenTime[2].dwHour;
		}
	}
}

int _XCastleManager::GetMobMasterNameIndex(int mapid)
{
	map <int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		_XCastleScheduler* pCBScheduler = iter->second;
		if(pCBScheduler)
		{
			return pCBScheduler->m_nMobMasterNameIndex;
		}
	}
	return 0;
}

void _XCastleManager::GetChallengeTime(int mapid, int* hour, int* minute, int& count)
{
	int i = 0;
	map <int, _XCastleScheduler*>::iterator iter = m_mCBScheduler.find(mapid);
	if(iter != m_mCBScheduler.end())
	{
		_XCastleScheduler* pCBScheduler = iter->second;
		if(pCBScheduler)
		{
			vector <_XCastle_OpenTime*>::iterator iter_time;
			for(iter_time = pCBScheduler->m_vecChallengeTime.begin() ; iter_time != pCBScheduler->m_vecChallengeTime.end() ; ++iter_time)
			{
				_XCastle_OpenTime* pTime = *iter_time;
				if(pTime)
				{
					if(i > 9)
					{
						count = i;
						return;
					}

					hour[i] = pTime->dwHour;
					minute[i] = pTime->dwMinute;
					i++;
				}
			}
		}
	}

	count = i;
}

void _XCastleManager::SetCastleTaxMoney(int mapid, DWORD totalmoney, DWORD partmoney)
{
	map <int, _XCastleInfo*>::iterator iter = m_mCastleInfo.find(mapid);
	if(iter != m_mCastleInfo.end())
	{
		_XCastleInfo* pCastleInfo = iter->second;
		if(pCastleInfo)
		{
			pCastleInfo->m_nTax = totalmoney;
			pCastleInfo->m_dwMoney = partmoney;
		}
	}
}

DWORD _XCastleManager::GetCastleTaxMoney(int mapid)
{
	map <int, _XCastleInfo*>::iterator iter = m_mCastleInfo.find(mapid);
	if(iter != m_mCastleInfo.end())
	{
		_XCastleInfo* pCastleInfo = iter->second;
		if(pCastleInfo)
		{
			return pCastleInfo->m_dwMoney;
		}
	}
	
	return 0;
}
