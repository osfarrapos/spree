#include "stdafx.h"
#include "XMnDStructInfo.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XwindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XMessageWindow.h"
#include "XParticleidDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_MessengerDialog.h"
#include "XWindow_MnDGroup.h" // Author : 양희왕 //breif : 사제관련 탭
#include <algorithm>

BOOL _XNetwork::Send_MnDSystem(int type, int param1, int param2, int param3, int param4)
{
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER		//Author : 양희왕 //breif : 사제 시스템
	switch( param1 )
	{
	case en_cg_mp_mode_info_req : // 사제 관계 정보 요청. 주변인이 보낸다.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple info");

			int size = sizeof(MSG_MP_INFO_REQ);
			MSG_MP_INFO_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_change_master_nick_req : // 사호 변경 요청.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple change nick name");

			int size = sizeof(MSG_MP_CHANGE_MASTER_NICK_REQ);
			MSG_MP_CHANGE_MASTER_NICK_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			strcpy( mndreq.cNick, g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName );

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_cg_apply_for_pupil: // // 제자 신청을 GS 로 보냄 : 제자가 되길 원하는 캐릭터가 보냄.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple apply disciple");

			int size = sizeof(MSG_MP_CG_APPLY_FOR_PUPIL);
			MSG_MP_CG_APPLY_FOR_PUPIL mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mdoe_answer_apply_from_master : // 스승 : 제자 신청에 대한 응답.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple apply master");

			int size = sizeof(MSG_MP_ANSWER_APPLY_FROM_MASTER);
			MSG_MP_ANSWER_APPLY_FROM_MASTER mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			//param2 에 윈도우 ID를 받아서 처리하자
			//_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_strike_pupil_req : // 제자 파문 요청
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple strike disciple");

			int size = sizeof(MSG_MP_STRIKE_PUPIL_REQ);
			MSG_MP_STRIKE_PUPIL_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_leave_req :	// 하산 요청.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple leave");

			int size = sizeof(MSG_MP_LEAVE_REQ);
			MSG_MP_LEAVE_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;

	}
#endif

	return TRUE;
}

void _XNetwork::MSGMasternDiscipleResult(MSG_MASTER_AND_PUPIL *pMnDResult)
{
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER		//Author : 양희왕 //breif : 사제 시스템

//#ifdef _XDWDEBUG
	_XLog("사제 패킷 받았음 모드 %d", pMnDResult->ucMP_Mode );
//#endif

	switch(pMnDResult->ucMP_Mode)
	{
	case en_cg_mp_mode_no_mp_info :  // 사제 관계가 없음.
		{
			g_pGroup_Window->m_pMnDWindow->ResetData();
			memset(&g_pLocalUser->m_pLocalMnDInfomation, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation));
		}
		break;
	case en_cg_mp_mode_info_my_master : // 사제 관계 정보. 필드 진입시 전달.
		{
			MSG_MP_INFO_MY_MASTER* pInfoMyMaster = (MSG_MP_INFO_MY_MASTER*)pMnDResult;

			g_pLocalUser->m_pLocalMnDInfomation.nMasterIndex = pInfoMyMaster->iMasterIndex; //스승의 캐릭터 인덱스 값
			g_pLocalUser->m_pLocalMnDInfomation.nMasterRank = pInfoMyMaster->ucMasterRank; //스승의 레벨
			g_pLocalUser->m_pLocalMnDInfomation.nTotalKillCount = pInfoMyMaster->uiTotalKillCount; //총 킬 수
			g_pLocalUser->m_pLocalMnDInfomation.nMasterTotalContribution = pInfoMyMaster->uiMasterTotalPoint;//스승 누적 포인트
			g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution = pInfoMyMaster->uiMasterCurPoint;//스승의 현재 포인트
			g_pLocalUser->m_pLocalMnDInfomation.nMasterConnectState = pInfoMyMaster->ucMasterConnectState; //스승의 현재 접속 상태
			g_pLocalUser->m_pLocalMnDInfomation.nBonusSkillStep = pInfoMyMaster->ucBonusSkillStep; // 사호 효과 스킬??
			
			g_pLocalUser->m_pLocalMnDInfomation.nSex = pInfoMyMaster->ucSex;
			g_pLocalUser->m_pLocalMnDInfomation.nLevel = pInfoMyMaster->ucLevel;              
			g_pLocalUser->m_pLocalMnDInfomation.nMoonpa = pInfoMyMaster->ucMoonpa;
			g_pLocalUser->m_pLocalMnDInfomation.nClass = pInfoMyMaster->ucClass;
			g_pLocalUser->m_pLocalMnDInfomation.nClassGrade = pInfoMyMaster->ucClassGrade;

			_XLog("필드 진입 스승 이름 %s\n 필드 진입 사호 %s\n", pInfoMyMaster->cMasterName, pInfoMyMaster->cMasterNick );

			//스승의 이름
			if( strlen(pInfoMyMaster->cMasterName) )
			{
				memset(g_pLocalUser->m_pLocalMnDInfomation.szMasterName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterName));
				strcpy(g_pLocalUser->m_pLocalMnDInfomation.szMasterName, pInfoMyMaster->cMasterName);

				if(stricmp( g_pLocalUser->m_pLocalMnDInfomation.szMasterName, g_pLocalUser->m_CharacterInfo.charactername ) == 0 ) //이름이 같으면
					g_pLocalUser->m_pLocalMnDInfomation.nMnDClass = 1;
			}

			//사호
			if( strlen(pInfoMyMaster->cMasterNick) )
			{
				memset( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName));
				strcpy( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, pInfoMyMaster->cMasterNick );
			}

			//리스트에 넣어줘야 한다. 우선 스승 정보 부터 넣어준 뒤 제자 정보를 넣어준다
			if( g_pGroup_Window->m_pMnDWindow )
			{
				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.clear();

				MnDGroupList* pMaster = new MnDGroupList;
				pMaster->nLevel = pInfoMyMaster->ucLevel; //스승 레벨
				pMaster->nCurrentZone = pInfoMyMaster->ucMasterCurZone; //스승 지역
				pMaster->nPlayerState = pInfoMyMaster->ucMasterConnectState; //접속 여부
				pMaster->nMnDClass = 1; //클래스  스승
				memset(pMaster->szName, 0, sizeof(pMaster->szName));
				strcpy( pMaster->szName, pInfoMyMaster->cMasterName );

				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.push_back( pMaster );

				for( int i = 0; i < 4; i++ )
				{
					_XLog("필드 진입 제자 이름 %s\n 필드 진입 몇등 제자 인가 %d\n", pInfoMyMaster->Pupils[i].cName, (int)pInfoMyMaster->Pupils[i].ucPosition );

					if(pInfoMyMaster->Pupils[i].ucPosition == 0 ) //포지션이 0 이면 아무것도 아니다
						continue;

					MnDGroupList* pDisciple = new MnDGroupList;

					memset(pDisciple->szName, 0, sizeof(pDisciple->szName));

					strcpy(pDisciple->szName, pInfoMyMaster->Pupils[i].cName ); //이름
					pDisciple->nLevel = pInfoMyMaster->Pupils[i].ucLevel; //레벨
					pDisciple->nDiscipleContribution = pInfoMyMaster->Pupils[i].uiCurPoint; //기여도
					pDisciple->nPlayerState = pInfoMyMaster->Pupils[i].ucConnectState; //접속상태
					pDisciple->nMnDClass = pInfoMyMaster->Pupils[i].ucPosition; //몇등제자 인가

					if(stricmp( pInfoMyMaster->Pupils[i].cName, g_pLocalUser->m_CharacterInfo.charactername ) == 0 ) //이름이 같으면
						g_pLocalUser->m_pLocalMnDInfomation.nMnDClass = pInfoMyMaster->Pupils[i].ucPosition;

					pDisciple->nCurrentZone = pInfoMyMaster->Pupils[i].ucCurZone; //제자의 지역

					g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.push_back( pDisciple );
				}
			}			

			//정렬,
			std::sort(g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.begin(), g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.end(), MnDGroupClassSort);

			g_pGroup_Window->m_pMnDWindow->SetListItem();
			
			if( g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS5)
				g_pGroup_Window->m_pMnDWindow->ShowMnDWindow(TRUE);
			
		}
		break;
	case en_cg_mp_mode_info_res :	// 사제 관계 정보 요청. 응답 간략 정보
		{
			MSG_MP_INFO_RES* pSimpleInfo = (MSG_MP_INFO_RES*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_update_point : // 스승 및 제자의 포인트 수정.
		{
			MSG_MP_UPDATE_POINT* pUpdatePoint = (MSG_MP_UPDATE_POINT*)pMnDResult;

			_XLog("포인트 수정 현재 포인트 %d\n 포인트 수정 토탈 포인트 %d\n 포인트 수정 기여도 %d\n", 
				pUpdatePoint->uiMasterCurPoint, pUpdatePoint->uiMasterTotalPoint, pUpdatePoint->uiCurPoint);

			g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution = pUpdatePoint->uiMasterCurPoint; //스승 현재 포인트
			g_pLocalUser->m_pLocalMnDInfomation.nMasterTotalContribution = pUpdatePoint->uiMasterTotalPoint; //스승 토탈 포인트
			g_pLocalUser->m_pLocalMnDInfomation.nLocalUserContribution = pUpdatePoint->uiCurPoint; //기여도,

			//그룹 리스트 갱신 해야함,
		}
		break;
	case en_cg_mp_mode_update_kill_count : // 킬 카운트 업데이트
		{
			MSG_MP_UPDATE_KILL_COUNT* pUpdateKillCount = (MSG_MP_UPDATE_KILL_COUNT*)pMnDResult;

			_XLog("킬카운트 내 킬 카운트 %d\n 킬카운트 토탈 카운트 %d\n",
				pUpdateKillCount->uiMyKillCount, pUpdateKillCount->uiTotalKillCount);

			g_pLocalUser->m_pLocalMnDInfomation.nCurKillCount = pUpdateKillCount->uiMyKillCount; //내 킬 카운트
			g_pLocalUser->m_pLocalMnDInfomation.nTotalKillCount = pUpdateKillCount->uiTotalKillCount; //총 킬 카운트
		}
		break;
	case en_cg_mp_mode_update_master_level : //스승 등급 설정
		{

			MSG_MP_UPDATE_MASTER_LEVEL* pUpdateLevel = (MSG_MP_UPDATE_MASTER_LEVEL*)pMnDResult;

			_XLog("등급 설정 %d\n", pUpdateLevel->ucMasterLevel);

			g_pLocalUser->m_pLocalMnDInfomation.nMasterRank = pUpdateLevel->ucMasterLevel; //스승 등급 변경
		}
		break;
	case en_cg_mp_mode_udpate_position : // 제1,2,3,4, 제자의 지위 바꾸기.
		{
			MSG_MP_UPDATE_POSITION* pUpdateLevel = (MSG_MP_UPDATE_POSITION*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_master_info_created : // 사제 정보가 생성되었다.( 퀘스트 : rank 1 설정시. )
		{
			MSG_MP_MASTER_INFO_CREATED* pCreateMaster = (MSG_MP_MASTER_INFO_CREATED*)pMnDResult;

			g_pLocalUser->m_pLocalMnDInfomation.nMasterIndex = pCreateMaster->iMasterIndex;
			g_pLocalUser->m_pLocalMnDInfomation.nMasterRank = pCreateMaster->ucMasterLevel;
			g_pLocalUser->m_pLocalMnDInfomation.nMnDClass = 1;

			g_pLocalUser->m_pLocalMnDInfomation.nSex = pCreateMaster->ucSex;
			g_pLocalUser->m_pLocalMnDInfomation.nLevel = pCreateMaster->ucLevel;	
			g_pLocalUser->m_pLocalMnDInfomation.nMoonpa = pCreateMaster->ucMoonpa;
			g_pLocalUser->m_pLocalMnDInfomation.nClass = pCreateMaster->ucClass;
			g_pLocalUser->m_pLocalMnDInfomation.nClassGrade= pCreateMaster->ucClassGrade;

			_XLog("사제 정보 생성 스승 인덱스 %d\n, 사제 정보 스승 등급 %d\n", pCreateMaster->iMasterIndex, pCreateMaster->ucMasterLevel);

			if( g_pGroup_Window->m_pMnDWindow )
			{
				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.clear();

				MnDGroupList* pMaster = new MnDGroupList;
				pMaster->nLevel = pCreateMaster->ucLevel; //스승 레벨
				pMaster->nCurrentZone = g_CurrentZoneInfoIndex; //스승 지역 내가 만들어 진거 니깐 내위치를 넣어주면 된다
				pMaster->nPlayerState = 1;//사제 정보가 생성 된거니깐,, 접속 중이다
				pMaster->nMnDClass = 1; //클래스  스승
				//스승은 목록에 기여도를 표시하지 않는다

				strcpy( pMaster->szName, g_pLocalUser->m_CharacterInfo.charactername ); //만들어진거니,, 내이름 넣어준다

				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.push_back( pMaster );
			}

			g_pGroup_Window->m_pMnDWindow->SetListItem();
			
			if( g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS5)
				g_pGroup_Window->m_pMnDWindow->ShowMnDWindow(TRUE);
		}
		break;
	case en_cg_mp_mode_update_con_state : // 사제의 접속 정보 업데이트
		{
			MSG_MP_UPDATE_CON_STATE* pUpdateState = (MSG_MP_UPDATE_CON_STATE*)pMnDResult;
			_XLog("사제 접속 정보업데이트 이름 %s, 위치번호 %d, 접속상태%d\n", pUpdateState->cName, pUpdateState->ucZone, pUpdateState->ucConState );

			if( g_pGroup_Window->m_pMnDWindow )
			{
				vector<MnDGroupList*>::iterator MnDiter = g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.begin();
				for(; MnDiter != g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.end(); ++MnDiter)
				{
					MnDGroupList* pGroup = (*MnDiter);
					if(pGroup)
					{
						if(stricmp( pGroup->szName, pUpdateState->cName ) == 0 ) //있다
						{
							pGroup->nPlayerState = pUpdateState->ucConState;
							pGroup->nCurrentZone = pUpdateState->ucZone;
							break;
						}
					}
				}
			}

			g_pGroup_Window->m_pMnDWindow->SetListItem();
			
			if( g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS5)
				g_pGroup_Window->m_pMnDWindow->ShowMnDWindow(TRUE);
		}
		break;
	case en_cg_mp_mode_bonus_setting : // 스승의 혜택 On/Off
		{
			MSG_MP_BONUS_SETTING* pUpdateLevel = (MSG_MP_BONUS_SETTING*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_change_master_nick_res :// 사호 변경을 알려준다.// 요청한 개인(스승)에게만 가고, 제자들에게는 MSG_MP_UPDATE_MASTER_NICK 이 간다. 
		{
			MSG_MP_CHANGE_MASTER_NICK_RES* pChangeNick = (MSG_MP_CHANGE_MASTER_NICK_RES*)pMnDResult;

			_XLog("변경될 사호 %s\n", pChangeNick->cNick);

			if( pChangeNick->ucSuccessFail ) // 0 - Fail, 1 - Success
			{
				if( strlen(pChangeNick->cNick) )
				{
					memset( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName));
					strcpy( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, pChangeNick->cNick);
				}

				//사호를 지정하였습니다.
				g_NetworkKernel.InsertChatString(_T("사호를 지정하였습니다."), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
			}
			else
			{
				g_NetworkKernel.InsertChatString(_T("사호 지정에 실패했습니다."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
		}
		break;
	case en_cg_mp_mode_update_master_nick : // 제자 또는 주변에 알린다.
		{
			MSG_MP_UPDATE_MASTER_NICK* pUpdateNick = (MSG_MP_UPDATE_MASTER_NICK*)pMnDResult;

			_XLog("다른 사람에게 변경된 사호 알림 %s\n", pUpdateNick->cNick);

			if( strlen(pUpdateNick->cNick) )
			{
				memset( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName));
				strcpy( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, pUpdateNick->cNick);
			}
		}
		break;
	case en_cg_mp_mode_info_error : // Info : error message.
		{
			MSG_MP_INFO_ERROR* pInfoError = (MSG_MP_INFO_ERROR*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_gc_apply_for_pupil : // 제자 신청을 받음 : 스승이 받음.
		{
			MSG_MP_GC_APPLY_FOR_PUPIL* pDiscipleInfo = (MSG_MP_GC_APPLY_FOR_PUPIL*)pMnDResult;

			//받을 때 윈도우를 띄워줘야한다
			int windowid = 0;
			bool bfindmessengerdialog = FALSE; 
			for( windowid = 60000; windowid < 60050; ++windowid )
			{
				_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
				if( pMessengerDialog_Window )
				{
					// 보낸 사람으로 윈도우를 찾는다 
					if(strcmpi(pDiscipleInfo->cFromPupil, pMessengerDialog_Window->m_ToCharactername) == 0 &&
						strcmpi(pDiscipleInfo->cMasterName, g_pLocalUser->m_CharacterInfo.charactername) == 0 )//나한테 보낸 윈도우가 있다.
					{
						if( pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MNDINSTANCE || //사제창이니 사제에 관련된 대화창이어야 한다
							pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MASTERDIALOG ||
							pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DISCIPLEDIALOG )
						{
							//있다 사제 신청을 마구 누르는 것인가,,, 그럼 여기로 올 수 있다.
							bfindmessengerdialog = TRUE;
							
							if( pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MNDINSTANCE )
							{
								g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );
								pMessengerDialog_Window->m_bMessageReceive = TRUE;
							}
							else
							{
								g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );						
							}
							break;
						}
					}
				}
			}

			if( !bfindmessengerdialog )// 메신져 대화창을 새로 만든다
			{		
				_XWindow_MessengerDialog* pMessengerDialog_Window = _XWindow_MessengerDialog::CreateMessengerDialogBox( &g_MainWindowManager );
				if( pMessengerDialog_Window )
				{
					for( int i = 0; i < 50; ++i)
					{
						if( !g_NetworkKernel.m_bMessengerInstanceMessagePosition[i] )
						{
							g_NetworkKernel.m_bMessengerInstanceMessagePosition[i] = TRUE;
							pMessengerDialog_Window->m_InstanceMessagePositionNumber = i;
							pMessengerDialog_Window->MoveWindow( gnWidth - _XDEF_INSTANCEMESSAGEWINDOW_WIDTH - (4*i), 214 + (27*i) );
							break;
						}
					}

					strcpy( pMessengerDialog_Window->m_ToCharactername, pDiscipleInfo->cFromPupil );			
					if(strcmpi(pDiscipleInfo->cMasterName, g_pLocalUser->m_CharacterInfo.charactername) == 0)
					{				
						if(pMessengerDialog_Window->m_MessengerChatList.getitemcount() > _XDEF_CHATLIST_QUESIZE)
						{
							pMessengerDialog_Window->m_MessengerChatList.deleteItem(0);
						}
						
						pMessengerDialog_Window->SetMode( _XMESSENGERDIALOGMODE_MNDINSTANCE );
						g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );
						pMessengerDialog_Window->m_bMessageReceive = TRUE;
					}

					pMessengerDialog_Window->m_DiscipleInfo.nClassInfo = pDiscipleInfo->ucMoonpa; //직책 정보
					pMessengerDialog_Window->m_DiscipleInfo.nLevel = pDiscipleInfo->ucLevel; //레벨
					
					memset( pMessengerDialog_Window->m_DiscipleInfo.szName, 0, sizeof(pMessengerDialog_Window->m_DiscipleInfo.szName) );
					strcpy( pMessengerDialog_Window->m_DiscipleInfo.szName, pDiscipleInfo->cFromPupil );
				}
			}	
		}
		break;
	case en_cg_mp_mode_answer_apply_no_to_pupil : // 제자가 될 캐릭터에게 응답을 보낸다. ( yes 를 선택했을 경우, 응답대신 complete_join_pupil 이 간다. )
		{
			MSG_MP_ANSWER_APPLY_NO_TO_PUPIL* pApplyNo = (MSG_MP_ANSWER_APPLY_NO_TO_PUPIL*)pMnDResult;
			
			//pApplyNo->cFromMaster; %s님이 사제 신청을 거부 하였습니다.
		}
		break;
	case en_cg_mp_mode_complete_join_pupil : // 제자가 되려는 캐릭터에에게, 제자 추가가 완료되었음을 알림. 
		{
			MSG_MP_COMPLETE_JOIN_PUPIL* pApplyYes = (MSG_MP_COMPLETE_JOIN_PUPIL*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_add_new_pupil : // 새로운 제자가 추가되었음을 스승 및 다른 제자에게 알림.
		{
			MSG_MP_ADD_NEW_PUPIL* pNewDisciple = (MSG_MP_ADD_NEW_PUPIL*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_apply_error :
		{
			MSG_MP_APPLY_ERROR* pError = (MSG_MP_APPLY_ERROR*)pMnDResult;

			switch(pError->ucErrorCode)
			{
			default :
				break;
			}
/*			en_mp_apply_no_error=0,						// 0 값은 그냥 쓴다. 이 값으로는 패킷이 가지 않는다.
			en_mp_apply_error_wait=1,					// 10 초 제한 : 잠시후 다시 시도.
			en_mp_apply_error_pupil_level_limit=2,		// 73 레벨 이상 캐릭터는 제자가 될 수 없습니다.
			en_mp_apply_error_level_diff_limit=3,		// 스승과 제자는 13레벨 이상 차이가 나야합니다.
			en_mp_apply_error_already_has_master=4,		// 이미 다른 스승이 있습니다.
			en_mp_apply_error_pupils_over=5,			// 더 이상 제자를 받을 수 없습니다.
			en_mp_apply_error_you_are_master=6,			// 현재 스승이다.*/
		}
		break;
	case en_cg_mp_mode_strike_result :	// 스승 및 다른 제자에게 감.
		{
			MSG_MP_STRIKE_RESULT* pStrikeRes = (MSG_MP_STRIKE_RESULT*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_you_struck : // 파문 제자에게 파문을 알림.
		{
			MSG_MP_YOU_STRUCK* pStrikeRes = (MSG_MP_YOU_STRUCK*)pMnDResult;

			//하산 하였습니다.
		}
		break;
	case en_cg_mp_mode_strike_error :	// 파문 에러.
		{
			MSG_MP_STRIKE_ERROR* pStrikeError = (MSG_MP_STRIKE_ERROR*)pMnDResult;

			switch(pStrikeError->ucErrorCode)
			{
			default:
				break;
			}

			/*en_mp_strike_error_no=0,			// 의미 없음.
			en_mp_strike_error_not_master=1,	// 파문 자격이 없음( 스승이 아님 )
			en_mp_strike_error_pupil_not_found=2,	// 파문은 제자가 연결되어 있어야 한다.
			en_mp_strike_error_system_error=255,	// 알 수 없는 에러다.*/
		}
		break;
	case en_cg_mp_mode_you_left :	// 하산에 성공.
		{
			MSG_MP_YOU_LEFT* pLeaveRes = (MSG_MP_YOU_LEFT*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_left_pupil : // 제자가 하산했다. 스승 및 다른 제자에게 전달.
		{
			MSG_MP_LEFT_PUPIL* pLeaveRes = (MSG_MP_LEFT_PUPIL*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_left_error :	// 하산 에러.
		{
			MSG_MP_LEFT_ERROR* pLeaveError = (MSG_MP_LEFT_ERROR*)pMnDResult;

			switch(pLeaveError->ucErrorCode)
			{
			default:
				break;
			}

			/*en_mp_leave_error_no=0,				// 의미 없음.
			en_mp_leave_error_not_pupil=1,		// 하산할 수 없음. 제자가 아님.
			en_mp_leave_error_system_error=255,	// 알 수 없는 에러다.*/
		}
		break;

	
	}
#endif
}