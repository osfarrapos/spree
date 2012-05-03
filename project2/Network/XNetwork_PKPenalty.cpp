// XNetwork_PKPenalty.cpp : PK 패널티 관련 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"

#include "XWindow_WorldMinimap.h"
#include "XWindow_PKTracingRequest.h"
#include "XWindow_PK_ListBox.h"
#include "XWindow_Rebirth.h"

#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한

void _XNetwork::MSGPKLevelPenaltyInfo(MSG_LTS_LVPK_INFO_GS_CL* pklevelinfo)						//레벨차 모든정보
{
	if( pklevelinfo->ucCount == 0 ) return;	
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 1 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
			}
			else
				++iter_pktracing;
		}
	}

	for(int i =0; i < pklevelinfo->ucCount; ++i) 
	{		
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  1;																				// 1:PK레벨차 패널티 2:PK추적 3:특정인 추적 
		pPKTracingItem->ucTimeMode = pklevelinfo->LvPkData[i].ucTimeMode;											// 0:1시간 1:24시간 2:48시간
		pPKTracingItem->nStartTrackingTime = pklevelinfo->LvPkData[i].iStartTrackingTime;							//추적 시작 시간
		strncpy( pPKTracingItem->szCharacName, pklevelinfo->LvPkData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//추적자 이름
		
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
}


void _XNetwork::MSGPKLevelPenaltyInsert(MSG_LTS_LVPK_INSERT_GS_CL* pklevel)						//레벨차 패널티 적용자 리스트 추가
{
	BOOL bExistData = FALSE;
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 1 )
			{
				if( strcmp( pklevel->PkData.szCharacName, pktracing->szCharacName ) == 0 )
				{
					//이미 있는 데이타이다.
					pktracing->ucTimeMode = pklevel->PkData.ucTimeMode;						// 0:1시간 1:24시간 2:48시간
					pktracing->nStartTrackingTime = pklevel->PkData.iStartTrackingTime;		//추적 시작 시간
					bExistData = TRUE;
					break;
				}
				else
					++iter_pktracing;
			}
			else
				++iter_pktracing;
		}
	}
	if( !bExistData )
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  1;																			// 1:PK레벨차 패널티 2:PK추적 3:특정인 추적 
		pPKTracingItem->ucTimeMode = pklevel->PkData.ucTimeMode;											// 0:1시간 1:24시간 2:48시간
		pPKTracingItem->nStartTrackingTime = pklevel->PkData.iStartTrackingTime;							//추적 시작 시간
		strncpy( pPKTracingItem->szCharacName, pklevel->PkData.szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//추적자 이름
		
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
}

void _XNetwork::MSGPKLevelPenaltyDelete(MSG_LTS_LVPK_DEL_REQ_GS_CL* pklevel)			//레벨차페널티 리스트 삭제 성공
{
	
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
	{
		pktracing = *iter_pktracing;
		if( pktracing->ucPKType = 1 )
		{
			if( strcmp( pktracing->szCharacName, pklevel->szDelCharacName) == 0 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
				break;
			}
			else
				++iter_pktracing;
		}
		else
			++iter_pktracing;
	}
	
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		memset(pPKTracingRequest_Window->m_cDeleteTargetName,0,sizeof(TCHAR)*13);
	}
}

void _XNetwork::MSGPKTracingInfo(MSG_LTS_PK_INFO_GS_CL* pktracinginfo)							//PK범 모든정보
{	
	if( pktracinginfo->ucCount == 0 ) return;	
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 2 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
			}
			else
				++iter_pktracing;
		}
	}
	for(int i =0; i < pktracinginfo->ucCount; ++i) 
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  2;																		// 1:PK레벨차 패널티 2:PK추적 3:특정인 추적 
		pPKTracingItem->ucTimeMode = pktracinginfo->PkData[i].ucTimeMode;												// 0:1시간 1:24시간 2:48시간
		pPKTracingItem->nStartTrackingTime = pktracinginfo->PkData[i].iStartTrackingTime;								//추적 시작 시간
		strncpy( pPKTracingItem->szCharacName, pktracinginfo->PkData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//추적자 이름

		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif
}




void _XNetwork::MSGPKTracingInsertSuccess(MSG_LTS_PK_INSERT_SUCCESS_GS_CL* pktracinginfo)		//PK범 리스트 추가 성공
{
	
	BOOL bExistData = FALSE;
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 2 )
			{
				if( strcmp( pktracinginfo->PkData.szCharacName, pktracing->szCharacName ) == 0 )
				{
					//이미 있는 데이타이다.
					pktracing->ucTimeMode = pktracinginfo->PkData.ucTimeMode;						//0:테스트 모드 3분 1:1시간 2:24시간 3:48시간
					pktracing->nStartTrackingTime = pktracinginfo->PkData.iStartTrackingTime;		//추적 시작 시간
					bExistData = TRUE;
					break;
				}
				else
					++iter_pktracing;
			}
			else
				++iter_pktracing;
		}
	}
	
	if( !bExistData )
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  2;																				// 1:PK레벨차 패널티 2:PK추적 3:특정인 추적 
		pPKTracingItem->ucTimeMode = pktracinginfo->PkData.ucTimeMode;												// 0:1시간 1:24시간 2:48시간
		pPKTracingItem->nStartTrackingTime = pktracinginfo->PkData.iStartTrackingTime;								//추적 시작 시간
		strncpy( pPKTracingItem->szCharacName, pktracinginfo->PkData.szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//추적자 이름
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
	
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		memset(pPKTracingRequest_Window->m_cTargetName,0,sizeof(TCHAR)*13);
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif

	//서버에서 오는 돈으로 갱신한다.
	g_pLocalUser->m_Money = pktracinginfo->uiRemainMoney;
	
}


void _XNetwork::MSGPKTracingInsertFail(MSG_LTS_PK_INSERT_FAILED_GS_CL* pktracinginfo)			//PK범 리스트 추가 실패
{
	//에러 메세지
	if( pktracinginfo->ucRes == 1 )//돈이 부족할때 
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NPCTRADE_NOTENOUGHMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//소지금이 부족합니다.
	}
	else if( pktracinginfo->ucRes == 2 )//대상자 이름이 없다.
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//선택된 대상이 없거나 잘못된 대상입니다.
	}
	else if( pktracinginfo->ucRes == 3 )//상대방 PK리스트에 등록되어 있다.
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3464), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("이미 상대방 추적 목록에 등록되어 있습니다.")
	}
	else // 그외 에러
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3465), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("추적할 수 없습니다.")
	}
}


void _XNetwork::MSGPKTracingDeleteSuccess(MSG_LTS_PK_DEL_SUCCESS_GS_CL* pktracinginfo)			//PK범 리스트 삭제 성공
{
	_XWindow_WorldMinimap* pMapWindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( !pMapWindow ) return;
	
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
	{
		pktracing = *iter_pktracing;
		if( pktracing->ucPKType == 2)
		{
			if( strcmp( pktracing->szCharacName, pktracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
				break;
			}
			else
				++iter_pktracing;
		}
		else
			++iter_pktracing;

	}
	
	
	//지도상에 표시된 것도 지운다.
	_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
	list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
	{
		pktracingposition = *iter_pktracingposition;
		if( pktracingposition )
		{
			if( strcmp( pktracingposition->szCharacName, pktracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracingposition );
				iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
				break;
			}
			else
				++iter_pktracingposition;
			
		}
		else
			++iter_pktracingposition;
	}

	if( pMapWindow->m_listPKTracingPosition.empty() )
	{
		pMapWindow->m_PKWarningLevel = 0;
		pMapWindow->m_bDrawPKWarning = FALSE;	
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif


}


void _XNetwork::MSGSomeOneTracingInfo(MSG_LTS_SP_INFO_GS_CL* tracinginfo)						//특정인 모든정보
{
	if( tracinginfo->ucCount == 0 ) return;	
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 3 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
			}
			else
				++iter_pktracing;
		}
	}
	for(int i =0; i < tracinginfo->ucCount; ++i) 
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  3;																		// 1:PK레벨차 패널티 2:PK추적 3:특정인 추적 
		pPKTracingItem->ucTimeMode = tracinginfo->SpData[i].ucTimeMode;												// 0:1시간 1:24시간 2:48시간
		pPKTracingItem->nStartTrackingTime = tracinginfo->SpData[i].iStartTrackingTime;								//추적 시작 시간
		strncpy( pPKTracingItem->szCharacName, tracinginfo->SpData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//추적자 이름
		
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif
}


void _XNetwork::MSGSomeOneTracingInsertSuccess(MSG_LTS_SP_INSERT_SUCCESS_GS_CL* tracinginfo)	//특정인 리스트 추가 성공
{
	BOOL bExistData = FALSE;
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 3 )
			{
				if( strcmp( tracinginfo->SpData.szCharacName, pktracing->szCharacName ) == 0 )
				{
					//이미 있는 데이타이다.
					pktracing->ucTimeMode = tracinginfo->SpData.ucTimeMode;						//0:테스트 모드 3분 1:1시간 2:24시간 3:48시간
					pktracing->nStartTrackingTime = tracinginfo->SpData.iStartTrackingTime;		//추적 시작 시간
					bExistData = TRUE;
					break;
				}
				else
					++iter_pktracing;
			}
			else
				++iter_pktracing;
		}
	}
	
	if( !bExistData )
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  3;																				// 1:PK레벨차 패널티 2:PK추적 3:특정인 추적 
		pPKTracingItem->ucTimeMode = tracinginfo->SpData.ucTimeMode;												// 0:1시간 1:24시간 2:48시간
		pPKTracingItem->nStartTrackingTime = tracinginfo->SpData.iStartTrackingTime;								//추적 시작 시간
		strncpy( pPKTracingItem->szCharacName, tracinginfo->SpData.szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//추적자 이름
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
	
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		//Author : 양희왕  추가
		memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
		//memset(pPKTracingRequest_Window->m_cTargetName,0,sizeof(TCHAR)*13);
	}
	
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif
	
	//서버에서 오는 돈으로 갱신한다.
	g_pLocalUser->m_Money = tracinginfo->uiRemainMoney;
	
}


void _XNetwork::MSGSomeOneTracingInsertFail(MSG_LTS_SP_INSERT_FAILED_GS_CL* tracinginfo)		//특정인 리스트 추가 실패
{
	if( tracinginfo->ucRes == 1 )//돈이 부족할때 
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NPCTRADE_NOTENOUGHMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//소지금이 부족합니다.
	}
	else if( tracinginfo->ucRes == 2 )//대상자 이름이 없다.
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//선택된 대상이 없거나 잘못된 대상입니다.
	}
	else if( tracinginfo->ucRes == 3 )//상대방 PK리스트에 등록되어 있다.
	{
		g_NetworkKernel.InsertChatString(  _XGETINTERFACETEXT(ID_STRING_NEW_3464), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
	}
	else // 그외 에러
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3465), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
	}
}


void _XNetwork::MSGSomeOneTracingDeleteSuccess(MSG_LTS_SP_DEL_SUCCESS_GS_CL* tracinginfo)		//특정인 리스트 삭제 성공
{
	_XWindow_WorldMinimap* pMapWindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( !pMapWindow ) return;
	
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
	{
		pktracing = *iter_pktracing;
		if( pktracing->ucPKType == 3)
		{
			if( strcmp( pktracing->szCharacName, tracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
				break;
			}
			else
				++iter_pktracing;
		}
		else
			++iter_pktracing;
		
	}
	
	
	//지도상에 표시된 것도 지운다.
	_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
	list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
	{
		pktracingposition = *iter_pktracingposition;
		if( pktracingposition )
		{
			if( strcmp( pktracingposition->szCharacName, tracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracingposition );
				iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
				break;
			}
			else
				++iter_pktracingposition;
			
		}
		else
			++iter_pktracingposition;
	}
	
	if( pMapWindow->m_listPKTracingPosition.empty() )
	{
		pMapWindow->m_PKWarningLevel = 0;
		pMapWindow->m_bDrawPKWarning = FALSE;	
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(pPKListbox_Window->GetListBox().GetItemCount() == 0)
		{
			if(pPKListbox_Window->GetShowStatus())
				pPKListbox_Window->ShowWindow(FALSE);
		}
	}
#endif

}


void _XNetwork::MSGTracingPositionInfo(MSG_LTS_POS_INFO_GS_CL* tracinginfo)						//3초마다 PK범 특정인 (캐릭명 + 위치정보)
{
	
	_XWindow_WorldMinimap* pMapWindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( !pMapWindow ) return;

	//리스트가 없다는 의미이므로 전부 삭제한다.
	if( tracinginfo->ucPkCount == 0 )
	{
		if( !pMapWindow->m_listPKTracingPosition.empty() )
		{
			_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
			list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
			for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
			{
				pktracingposition = *iter_pktracingposition;
				if( pktracingposition )
				{
					SAFE_DELETE( pktracingposition );
					iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
				}
				else
					++iter_pktracingposition;
			}
			pMapWindow->m_bDrawPKWarning = FALSE;
		}
		return;
	}
	
	_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
	list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	if( !pMapWindow->m_listPKTracingPosition.empty() )
	{
		for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); ++iter_pktracingposition)
		{
			pktracingposition = *iter_pktracingposition;
			if( pktracingposition )
			{
				pktracingposition->bRenewalData = FALSE;
			}
		}
	}

	BOOL bExistData = FALSE;
	for(int i =0; i < tracinginfo->ucPkCount; ++i) 
	{		
		bExistData = FALSE;
		//기존 리스트를 검사해서 있는 데이타면 위치정보만 갱신한다.
		if( !pMapWindow->m_listPKTracingPosition.empty() )
		{
			pktracingposition = NULL;
			list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition1;
			for(iter_pktracingposition1 = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition1 != pMapWindow->m_listPKTracingPosition.end(); ++iter_pktracingposition1)
			{
				pktracingposition = *iter_pktracingposition1;
				if( pktracingposition )
				{
					if(strcmp( pktracingposition->szCharacName, tracinginfo->LtsPosData[i].szCharacName ) == 0 )
					{
						pktracingposition->bRenewalData = TRUE;
						pktracingposition->ucPKType = tracinginfo->LtsPosData[i].ucPkOrSpMode;
						pktracingposition->sPosX = tracinginfo->LtsPosData[i].sPosX;
						pktracingposition->sPosZ = tracinginfo->LtsPosData[i].sPosZ;
						
						bExistData = TRUE;
						break;
					}
				}
			}
		}

		//기존 리스트에 정보가 없으면 새로 추가한다.
		if( !bExistData )
		{
			_XPKTRACING_POSITION_INFO* pPKTracingPositionItem = new _XPKTRACING_POSITION_INFO;
			pPKTracingPositionItem->bRenewalData = TRUE;
			pPKTracingPositionItem->ucPKType = tracinginfo->LtsPosData[i].ucPkOrSpMode;						// 1:PK레벨차 패널티 2:PK추적 3:특정인 추적 
			pPKTracingPositionItem->sPosX = tracinginfo->LtsPosData[i].sPosX;
			pPKTracingPositionItem->sPosZ = tracinginfo->LtsPosData[i].sPosZ;
			strncpy( pPKTracingPositionItem->szCharacName, tracinginfo->LtsPosData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//추적자 이름
			
			pMapWindow->m_listPKTracingPosition.push_back(pPKTracingPositionItem);
		}

	}

	
	//기존에 있는 데이타가 갱신이 되지 않았거나 새로운 데이타가 아닌 경우에는 리스트에서 삭제한다.
	pktracingposition = NULL;
	//list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
	{
		pktracingposition = *iter_pktracingposition;
		if( pktracingposition )
		{
			if( !pktracingposition->bRenewalData )
			{
				SAFE_DELETE( pktracingposition );
				iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
			}
			else
				++iter_pktracingposition;

		}
		else
			++iter_pktracingposition;
	}
	
	if( !pMapWindow->m_listPKTracingPosition.empty() ) 
		pMapWindow->ProcessPKTracingDistance();
	else
		pMapWindow->m_bDrawPKWarning = FALSE;

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->UpdateLeftTime();
	}
#endif

}

void _XNetwork::MSGPKTargetName(MSG_LTS_PK_INSERT_REQ_GS_CL* pkname)
{
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));
		strcpy( pPKTracingRequest_Window->m_cTargetName, pkname->szInsertCharacName );
	}

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
		pRebirth_Window->m_bDeadInPK = TRUE;
}

#endif //#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한