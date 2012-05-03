// XNetwork_NickName.cpp : 별호 관련
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_NickName.h"
#include "XMessageWindow.h"
#include "XNetwork.h"
#include "XSR_STRINGHEADER.H"


BOOL _XNetwork::Send_MSGNickName(int type, int param1, int param2, int param3)
{
	switch(type)
	{
	case MSG_NO_NIC_LIST_REQ_C_S : // 별호 리스트를 요청
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Nickname List");
			
			int size = sizeof(MSG_NIC_LIST_REQ);
			MSG_NIC_LIST_REQ requestnickname;
			ZeroMemory(&requestnickname, size);
			requestnickname.header.sLength = size;
			requestnickname.ucMessage = MSG_NO_NIC_LIST_REQ_C_S;
			
			return _XSendPacket_Encapsulate( (char*)&requestnickname );			
		}
		break;
		
		
	case MSG_NO_NIC_SELECT_REQ_C_S : // 별호를 선택한다.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Select Nickname");
			
			int size = sizeof(MSG_NIC_SELECT_REQ);
			MSG_NIC_SELECT_REQ selectnickname;
			ZeroMemory(&selectnickname, size);
			selectnickname.header.sLength = size;
			selectnickname.ucMessage = MSG_NO_NIC_SELECT_REQ_C_S;
			
			selectnickname.sSlotNum = (short)param1;
			g_pLocalUser->m_CharacterInfo.m_CurrentNIckIndex = selectnickname.sSlotNum;
		
			return _XSendPacket_Encapsulate( (char*)&selectnickname );
		}
		break;
	case MSG_NO_NIC_DELETE_REQ_C_S : // 별호 삭제
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Delete Nickname");

			int size = sizeof(MSG_NIC_DELETE_REQ);
			MSG_NIC_DELETE_REQ selectnickname;
			ZeroMemory(&selectnickname, size);
			selectnickname.header.sLength = size;
			selectnickname.ucMessage = MSG_NO_NIC_DELETE_REQ_C_S;
			
			selectnickname.sSlotNum = (short)param1;
			g_pLocalUser->m_CharacterInfo.m_CurrentNIckIndex = selectnickname.sSlotNum;
		
			return _XSendPacket_Encapsulate( (char*)&selectnickname );
		}
		break;
	case MSG_NO_RANK_HIDING :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : rank hiding");

			int size = sizeof(MSG_RANK_HIDING);
			MSG_RANK_HIDING rankhiding;
			ZeroMemory(&rankhiding, size);
			rankhiding.header.sLength = size;
			rankhiding.ucMessage = MSG_NO_RANK_HIDING;

			rankhiding.cMode = (char)param1;
			
			return _XSendPacket_Encapsulate( (char*)&rankhiding );
		}
		break;
	case MSG_NO_HONOR_PLAYER :
		{
#ifdef _XTS_NEW_HONOR2
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : honor req");

			int size = sizeof(MSG_HONOR_PLAYER);
			MSG_HONOR_PLAYER honorplayer;
			ZeroMemory(&honorplayer, size);
			honorplayer.header.sLength = size;
			honorplayer.ucMessage = MSG_NO_HONOR_PLAYER;

			honorplayer.cMode = MSG_HONOR_PLAYER::_GIVE_REQ;
			_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
			if(pUser)
			{
				strcpy( honorplayer.cToCharacName, pUser->m_CharacterInfo.charactername );
			}			
			honorplayer.iHohorValue =  (int)param1;		// +부여, - 하락
			
			return _XSendPacket_Encapsulate( (char*)&honorplayer );
#endif
		}
		break;
	}
	
	return TRUE;
}


void _XNetwork::MSGNicknameListReceive(MSG_NIC_LIST* nicknamelistreceive)// 별호 리스트를 반환
{
	_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NICKNAME );
	if( pNickName_Window )
	{
		if( nicknamelistreceive->nicData.sUsedTime == 0 )
		{
			pNickName_Window->m_ApplyButton->EnableWindow(TRUE);
			pNickName_Window->m_OKButton->EnableWindow(TRUE);
		}
		else
		{
			pNickName_Window->m_ApplyButton->EnableWindow(FALSE);
			pNickName_Window->m_OKButton->EnableWindow(FALSE);
		}
		pNickName_Window->m_RemainChangeTime = nicknamelistreceive->nicData.sUsedTime/60;
	}

	if( g_pLocalUser )
	{
		g_pLocalUser->m_StateList.DeleteStateFromNicknameEffect();

		// icon 삭제
		if( g_pState_Window )	
			g_pState_Window->DeleteNicknameBuff();	// 버프 제거
		
		g_pLocalUser->m_CharacterInfo.m_CurrentNIckIndex = nicknamelistreceive->nicData.sSelectNic;
		
		if( nicknamelistreceive->nicData.sSelectNic == 0 )
		{
			memset( g_pLocalUser->m_CharacterInfo.m_NickName, 0, sizeof(TCHAR)*256);
		}
		for( int i = 0; i < _NIC_COUNT_TOTAL ; ++i )
		{
			g_pLocalUser->m_CharacterInfo.m_NickNameList[i] = nicknamelistreceive->nicData.usNicID[i];
		}

		if( pNickName_Window )
		{
			pNickName_Window->SetNickNameList();
		}
		
		for( i = 0; i < g_MaxNickNameCount; ++i)
		{
			if( g_NickNameInfoTable[i].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[nicknamelistreceive->nicData.sSelectNic] )
			{
				strcpy( g_pLocalUser->m_CharacterInfo.m_NickName , g_NickNameInfoTable[i].cNickName );

				// 버프 추가
				for( int j = 0; j < 5; ++j )
				{
					if( g_NickNameInfoTable[i]._Effect[j].usEffectIndex > 0 )
					{
						g_pLocalUser->m_StateList.InsertStateFromNicknameEffect( g_NickNameInfoTable[i]._Effect[j].usEffectIndex, g_NickNameInfoTable[i]._Effect[j].usEffectValue);
						
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_NickNameInfoTable[i]._Effect[j].usEffectIndex);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[g_NickNameInfoTable[i]._Effect[j].usEffectIndex]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								g_pLocalUser->UpdateLocalUserAttackLevel();
							}
						}						

					}
				}
				if( g_pState_Window )
					g_pState_Window->InsertStateListFromOther(_XST_NICKBUFF, i );

				break;
			}
		}
	}
	
	if( g_pCInfo_Window )
		memset( g_pCInfo_Window->m_FavoritesNickNameIndexList, 0, sizeof(g_pCInfo_Window->m_FavoritesNickNameIndexList) );
}

void _XNetwork::MSGNicknameSetting(MSG_NIC_SET* nicknamesetting)// 슬롯에 별호를 설정한다
{
	_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NICKNAME );
	if( pNickName_Window )
	{
		if( nicknamesetting->sSlotNum > 0 && nicknamesetting->usNicID == 0 )
		{
			pNickName_Window->m_SelectedNickNameSlotNumber = -1;

			TCHAR tempNick[256];	
			memset(tempNick, 0, sizeof(tempNick) );

			for( int i = 0; i < g_MaxNickNameCount; ++i )
			{
				if( g_NickNameInfoTable[i].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[nicknamesetting->sSlotNum] )
				{
					if( strcmp(g_pLocalUser->m_CharacterInfo.m_NickName, g_NickNameInfoTable[i].cNickName) == 0 )
					{
						_snprintf( tempNick, sizeof(tempNick), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1629), g_NickNameInfoTable[i].cNickName );
						memset( g_pLocalUser->m_CharacterInfo.m_NickName, 0, sizeof(g_pLocalUser->m_CharacterInfo.m_NickName));
						
						g_pLocalUser->m_StateList.DeleteStateFromNicknameEffect();
						
						// icon 삭제
						if( g_pState_Window )	
							g_pState_Window->DeleteNicknameBuff();	// 버프 제거
						
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
					else
						_snprintf( tempNick, sizeof(tempNick), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1630), g_NickNameInfoTable[i].cNickName );
					
					InsertChatString(tempNick, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0202 );
					g_pLocalUser->m_CharacterInfo.m_NickNameList[nicknamesetting->sSlotNum] = nicknamesetting->usNicID;
					break;
				}
			}
		}
		else
		{
			TCHAR tempNick[256];	
			memset(tempNick, 0, sizeof(tempNick) );
			for( int i = 0; i < g_MaxNickNameCount; ++i )
			{
				if( g_NickNameInfoTable[i].usIndex == nicknamesetting->usNicID )
				{
					_snprintf( tempNick, sizeof(tempNick), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1631), g_NickNameInfoTable[i].cNickName );
					InsertChatString(tempNick, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
					g_pLocalUser->m_CharacterInfo.m_NickNameList[nicknamesetting->sSlotNum] = nicknamesetting->usNicID;

					if( nicknamesetting->sSlotNum == 10 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1632), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 20 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1633), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 30 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1634), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 80 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1635), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 90 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1636), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 140 )
					{						
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1637), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 151 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1638), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 161 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1639), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 181 )
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1640), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					else if( nicknamesetting->sSlotNum == 211 )
					{						
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1641), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
					}
					break;
				}
			}
		}
		
		// 별호 리스트를 재요청
		g_NetworkKernel.SendPacket(MSG_NO_NIC_LIST_REQ_C_S);		
	}	
}

void _XNetwork::MSGNicknameSelect(MSG_NIC_SELECT* nicknameselect)// 선택한 별호 정보를 반환한다.
{	
	if( nicknameselect->usID == g_pLocalUser->m_Uniqueid )
	{
		if( nicknameselect->usNicID == MSG_NIC_SELECT::_SELECTED_FREE_ )	// 선택 해제
		{
			memset( g_pLocalUser->m_CharacterInfo.m_NickName, 0, sizeof(TCHAR)*256);

			g_pLocalUser->m_StateList.DeleteStateFromNicknameEffect();
			
			// icon 삭제
			if( g_pState_Window )	
				g_pState_Window->DeleteNicknameBuff();	// 버프 제거

			g_pLocalUser->UpdateLocalUserAttackLevel();
		}
		else
		{
			for( int i = 0; i < g_MaxNickNameCount; ++i)
			{
				if( g_NickNameInfoTable[i].usIndex == nicknameselect->usNicID )
				{
					if( g_pCInfo_Window )
					{
						g_pCInfo_Window->InsertNickNameIndex( nicknameselect->usNicID );
					}
					strcpy( g_pLocalUser->m_CharacterInfo.m_NickName , g_NickNameInfoTable[i].cNickName );
					
					g_pLocalUser->m_StateList.DeleteStateFromNicknameEffect();
					
					// icon 삭제
					if( g_pState_Window )	
						g_pState_Window->DeleteNicknameBuff();	// 버프 제거
					
					// 버프 추가
					for( int j = 0; j < 5; ++j )
					{
						if( g_NickNameInfoTable[i]._Effect[j].usEffectIndex > 0 )
						{
							g_pLocalUser->m_StateList.InsertStateFromNicknameEffect( g_NickNameInfoTable[i]._Effect[j].usEffectIndex, g_NickNameInfoTable[i]._Effect[j].usEffectValue);
							
							map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_NickNameInfoTable[i]._Effect[j].usEffectIndex);
							if(iter_table != g_CharacterStateTable.end())
							{
								if(g_CharacterStateTable[g_NickNameInfoTable[i]._Effect[j].usEffectIndex]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
								{
									g_pLocalUser->UpdateLocalUserAttackLevel();
								}
							}
						}
					}

					if( g_pState_Window )
						g_pState_Window->InsertStateListFromOther(_XST_NICKBUFF, i );
					break;
				}
			}
		}
	}
	else
	{	
		_XUser* pUser = g_Userlist.FindUser(nicknameselect->usID);
		if(pUser)
		{
			for( int i = 0; i < g_MaxNickNameCount; ++i)
			{
				if( g_NickNameInfoTable[i].usIndex == nicknameselect->usNicID )
				{
					strcpy( pUser->m_CharacterInfo.m_NickName , g_NickNameInfoTable[i].cNickName );
					break;
				}
			}
		}
	}
	
}

void _XNetwork::MSGNicknameResult(MSG_NIC_RES* nicknameresult)
{
	switch(nicknameresult->ucCommand) {
	case MSG_NO_NIC_LIST_REQ_C_S:
		{
			switch(nicknameresult->ucCode) {
			case _RESULT_CODE_SUCCESS:
				{
				}
				break;
			case _RESULT_CODE_NOTCASE:
				{
				}
				break;
			case _RESULT_CODE_ERROR:
				{
				}
				break;
			}
		}
		break;

	case MSG_NO_NIC_SELECT_REQ_C_S:
		{
			switch(nicknameresult->ucCode) {
			case _RESULT_CODE_SUCCESS:
				{
				}
				break;
			case _RESULT_CODE_NOTCASE:
				{
				}
				break;
			case _RESULT_CODE_ERROR:
				{
				}
				break;
			}
		}
		break;
	}
}

/*============================================================================

  MSG_NIC_RES 설명

	ucCommand : 어떤 패킷에 대한 결과 패킷인지 구분하기 위해..
	ucCode	  : 성공 또는 실패 코드이다..

--------------------------------------------+-----------------------------------+---------------------------
 	ucCommand								|	iCode							|		뜻
--------------------------------------------+-----------------------------------+---------------------------
 	MSG_NO_NIC_LIST_REQ_C_S					|	_RESULT_CODE_SUCCESS			|	? (성공하면 리스트가 감)
 											|	_RESULT_CODE_NOTCASE			|	조건 불만족
											|	_RESULT_CODE_ERROR				|	기타 오류
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_NIC_SELECT_REQ_C_S				|	_RESULT_CODE_SUCCESS			|	? (성공하면 선택한 NicID 가 감)
											|	_RESULT_CODE_NOTCASE			|	실패
											|	_RESULT_CODE_ERROR				|	기타 오류
--------------------------------------------+-----------------------------+---------------------------------
*/

void _XNetwork::MSGNicknameRemainTime(MSG_NIC_USEFULL_TIME* timeresult)
{
	_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NICKNAME );
	if( pNickName_Window )
	{
		pNickName_Window->m_RemainChangeTime = timeresult->time/60;
		if( timeresult->time == 0 )
		{
			pNickName_Window->m_ApplyButton->EnableWindow(TRUE);
			pNickName_Window->m_OKButton->EnableWindow(TRUE);
		}
		else
		{
			pNickName_Window->m_ApplyButton->EnableWindow(FALSE);
			pNickName_Window->m_OKButton->EnableWindow(FALSE);
		}
	}
}

void _XNetwork::MSGRankHiding(MSG_RANK_HIDING* hidingresult)
{
	g_pLocalUser->m_CharacterInfo.cHindingMode = hidingresult->cMode;

	TCHAR tempStr[256];
	memset(tempStr, 0, sizeof(tempStr));

	if( hidingresult->cMode == MSG_RANK_HIDING::_HIDE_ON )
	{
		_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1642), g_pLocalUser->m_CharacterInfo.charactername );				
	}
	else if( hidingresult->cMode == MSG_RANK_HIDING::_HIDE_OFF )
	{
		_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1643), g_pLocalUser->m_CharacterInfo.charactername );				
	}
	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
	pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), tempStr, TRUE);

	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1644), _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
}

void _XNetwork::MSGPlayerHonorResult(MSG_HONOR_PLAYER* honorresult)
{
	switch(honorresult->cMode) 
	{
		case MSG_HONOR_PLAYER::_GIVE_SUCCESSED :	// 명성 부여 성공
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1645), _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
			}
			break;
		case MSG_HONOR_PLAYER::_GIVE_FAILED :		// 명성 부여 실패(시간 제한)
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1646), _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
			}
			break;
		case MSG_HONOR_PLAYER::_TAKE_FROM_PLAYER :			// PC로 부터 명성을 부여받았습니다.
			{
				if( honorresult->iHohorValue > 0 )	// 선행
				{
					TCHAR tempstr[256];
					memset(tempstr, 0, sizeof(tempstr));
					_snprintf( tempstr, sizeof(tempstr), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1647), honorresult->cToCharacName );
					InsertChatString(tempstr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
					g_pLocalUser->m_CharacterInfo.Set_honor( g_pLocalUser->m_CharacterInfo.Get_honor() + honorresult->iHohorValue );
				}
				else	// 악행
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1648), _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
					g_pLocalUser->m_CharacterInfo.Set_honor( g_pLocalUser->m_CharacterInfo.Get_honor() + honorresult->iHohorValue );
				}
			}
			break;
		case MSG_HONOR_PLAYER::_TAKE_FROM_SERVER :
			{
				if( honorresult->iHohorValue > 0 )	// 선행
				{
					CHAR tempstr[256];
					memset(tempstr, 0, sizeof(tempstr));
					_snprintf( tempstr, sizeof(tempstr), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1649), honorresult->iHohorValue );

					if( honorresult->iHohorValue >= 10000 )
					{						
						InsertChatString( tempstr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
					}
					else if( honorresult->iHohorValue >= 1000 )
					{
						InsertChatString( tempstr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
					}
					g_pLocalUser->m_CharacterInfo.Set_honor( g_pLocalUser->m_CharacterInfo.Get_honor() + honorresult->iHohorValue );
				}
				else	// 악행
				{
					CHAR tempstr[256];
					memset(tempstr, 0, sizeof(tempstr));
					_snprintf( tempstr, sizeof(tempstr), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1650), honorresult->iHohorValue );

					if( honorresult->iHohorValue <= -10000 )
					{
						InsertChatString( tempstr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					}
					else if( honorresult->iHohorValue <= -1000 )
					{
						InsertChatString( tempstr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}

					g_pLocalUser->m_CharacterInfo.Set_honor( g_pLocalUser->m_CharacterInfo.Get_honor() + honorresult->iHohorValue );
				}
			}
			break;
	}
}