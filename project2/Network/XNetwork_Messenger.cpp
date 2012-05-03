// XNetwork_Messenger.cpp : 전투 관련 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"

#include "XWindow_ChattingDefault.h"
#include "XWindow_SystemMsgDefault.h"
#include "XWindow_RequestParty.h"

#include "XWindow_Messenger.h"
#include "XWindow_MessengerDialog.h"
#include "XWindow_MessengerTooltip.h"
#include "XWindow_NameInput.h" 
#include "XWindow_Email.h"

#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_BlackList.h"
#include "XWindow_RecallItem.h" // Author : 양희왕
#include "SoundEffectList.h" //Author : 양희왕
#include "XParticleList.h" //Author : 양희왕 //이펙트
#include "XParticleIDDefine.h" //Author : 양희왕 //이펙트

#define _GROUP_PARSING_CHAR_				'\r'		// '&'그룹이름 구분
#define _CHARAC_PARSING_CHAR_				'\n'		// '#'캐릭이름 구분..

extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

/////////////////////////////////////////////////////////////////////////////////////
//                              메신져 관련 함수                                   //
/////////////////////////////////////////////////////////////////////////////////////		

BOOL _XNetwork::Send_MMSGMessenger(int type, int param1, int param2)
{	
	switch ( type )
	{
	case MMSG_NO_LOGIN_C_S : // 메신저 서버 로그인 요청 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Messenger Server Log in =================================");
			
			int size = sizeof(MMSG_MSLOGIN);
			MMSG_MSLOGIN messengerserverlogin;
			ZeroMemory(&messengerserverlogin, size);
			messengerserverlogin.sLength = size;
			messengerserverlogin.cMessage = MMSG_NO_LOGIN_C_S;
			strcpy( messengerserverlogin.cCharacName, g_pLocalUser->m_CharacterInfo.charactername );
			
			if(send(m_hMessengerSocket, (char*)&messengerserverlogin, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
		}
		break;
				
	case MMSG_NO_LOGOUT_C_S:// 로그오프 요청
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Messenger Server Log Off =================================");
			
			int size = sizeof(MMSG_MSLOGOUT);
			MMSG_MSLOGOUT messengerserverlogoff;
			ZeroMemory(&messengerserverlogoff, size);
			messengerserverlogoff.sLength = size;
			messengerserverlogoff.cMessage = MMSG_NO_LOGOUT_C_S;
			
			if(send(m_hMessengerSocket, (char*)&messengerserverlogoff, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
		}
		break;
		
	case MMSG_NO_MAKE_FRIEND_REQ_C_S_C:// 친구 맺기 제안..
		{		
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE : Join Friend ");

			int size = sizeof(MMSG_MAKE_FRIEND_REQUEST);
			MMSG_MAKE_FRIEND_REQUEST joinfriend;
			ZeroMemory(&joinfriend, size);
			joinfriend.sLength = size;
			joinfriend.cMessage = MMSG_NO_MAKE_FRIEND_REQ_C_S_C;
	
			if(pMessenger_Window->m_CharacterName[0] != 0)
				strcpy( joinfriend.cCharacName, pMessenger_Window->m_CharacterName);

			
			if(send(m_hMessengerSocket, (char *)&joinfriend, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
		}
		break;
		
	case MMSG_NO_MAKE_FRIEND_CON_C_S:// 친구 맺기 수락 or 거부  : 서버에 친구 등록 요청
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE : Response Making Friend ");
			
			int size = sizeof(MMSG_MAKE_FRIEND_CONFIRM);
			MMSG_MAKE_FRIEND_CONFIRM acceptfriend;
			ZeroMemory(&acceptfriend, size);
			acceptfriend.sLength = size;
			acceptfriend.cMessage = MMSG_NO_MAKE_FRIEND_CON_C_S;
			acceptfriend.cResult = (char)param1; // 0 : 수락 , 1 : 거부 

			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				strcpy( acceptfriend.cCharacName, pRequestParty_Window->m_RequestCharacterName );
			}
			
			if(send(m_hMessengerSocket, (char *)&acceptfriend, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}			
		}
		break;

	case MMSG_NO_CUT_FRIEND_REQ_C_S:// 친구 삭제 요청..
		{			

 			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			if(pMessenger_Window->m_SelectedCharactername[0] != 0 )
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("SEND MESSENGER  MESSAGE : Cutting off Friend ");
				
				int size = sizeof(MMSG_CUT_FRIEND_REQ);
				MMSG_CUT_FRIEND_REQ cutfriend;
				ZeroMemory(&cutfriend, size);
				cutfriend.sLength = size;
				cutfriend.cMessage = MMSG_NO_CUT_FRIEND_REQ_C_S;

				strcpy( cutfriend.cFriendCharac, pMessenger_Window->m_SelectedCharactername  );
				
				if(send(m_hMessengerSocket, (char *)&cutfriend, size, 0) == SOCKET_ERROR)
				{
					ProcessMessengerNetworkError();
					return FALSE;
				}
			}			
		}
		break;

	case MMSG_NO_FRIEND_LIST_REQ_C_S : // 메신저 친구 리스트 요청
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Request Friend List =================================");
			
			int size = sizeof(MMSG_FRIEND_LIST_REQ);
			MMSG_FRIEND_LIST_REQ requestfriendlist;
			ZeroMemory(&requestfriendlist, size);
			requestfriendlist.sLength = size;
			requestfriendlist.cMessage = MMSG_NO_FRIEND_LIST_REQ_C_S;
			
			if(send(m_hMessengerSocket, (char*)&requestfriendlist, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
		}
		break;
		
		
	case MMSG_NO_MY_STATE_UPDATE_C_S:// 나의 상태 정보 업데이트 요청
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  My State Update =================================");
			
			int size = sizeof(MMSG_MY_STATE_UPDATE);
			MMSG_MY_STATE_UPDATE mystateupdate;
			ZeroMemory(&mystateupdate, size);
			mystateupdate.sLength = size;
			mystateupdate.cMessage = MMSG_NO_MY_STATE_UPDATE_C_S;
			mystateupdate.cState = (char)param1;
			
			if(send(m_hMessengerSocket, (char*)&mystateupdate, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
		}
		break;
				
	case MMSG_NO_MAKE_GROUP_REQ_C_S:// 그룹 만들기 요청
		{
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE : Make Friend's Group ");

			int size = sizeof(MMSG_MAKE_GROUP_REQ);
			MMSG_MAKE_GROUP_REQ makegroup;
			ZeroMemory(&makegroup, size);
			makegroup.sLength = size;
			makegroup.cMessage = MMSG_NO_MAKE_GROUP_REQ_C_S;
			
			strcpy( makegroup.cGroupName, pMessenger_Window->m_GroupName);
			
			if(send(m_hMessengerSocket, (char *)&makegroup, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
			
		}
		break;
		
	case MMSG_NO_CUT_GROUP_REQ_C_S:// 그룹 삭제 요청
		{
			
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			if(pMessenger_Window->m_SelectedGroupname[0] != 0)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("SEND MESSENGER  MESSAGE : Delete Friend's Group ");
				
				int size = sizeof(MMSG_CUT_GROUP_REQ);
				MMSG_CUT_GROUP_REQ deletegroup;
				ZeroMemory(&deletegroup, size);
				deletegroup.sLength = size;
				deletegroup.cMessage = MMSG_NO_CUT_GROUP_REQ_C_S;

				strcpy( deletegroup.cGroupName, pMessenger_Window->m_SelectedGroupname );

				if(send(m_hMessengerSocket, (char *)&deletegroup, size, 0) == SOCKET_ERROR)
				{
					ProcessMessengerNetworkError();
					return FALSE;
				}			
			}
		}
		break;
		
	case MMSG_NO_JOIN_GROUP_REQ_C_S:// 그룹 이동..
		{
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			
			if( pMessenger_Window->m_SelectedGroupname[0] != 0 && pMessenger_Window->m_SelectedCharactername[0] != 0 )
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("SEND MESSENGER  MESSAGE : Move Friend's Group ");
				
				int size = sizeof(MMSG_JOIN_GROUP_REQ);
				MMSG_JOIN_GROUP_REQ movegroup;
				ZeroMemory(&movegroup, size);
				movegroup.sLength = size;
				movegroup.cMessage = MMSG_NO_JOIN_GROUP_REQ_C_S;
				
				strcpy( movegroup.cFriendName, pMessenger_Window->m_SelectedCharactername );
				strcpy( movegroup.cGroupName , pMessenger_Window->m_SelectedGroupname );
				
				if(send(m_hMessengerSocket, (char *)&movegroup, size, 0) == SOCKET_ERROR)
				{
					ProcessMessengerNetworkError();
					return FALSE;
				}
			}			
		}
		break;

	case MMSG_NO_RENAME_GROUP_REQ_C_S:// 그룹 이름 바꾸기 ..
		{
			_XWindow_NameInput* pNameInput_Window = (_XWindow_NameInput*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NAMEINPUTWINDOW);
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			
			if(pNameInput_Window)
			{
				if( pMessenger_Window->m_SelectedGroupname[0] != 0 )
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("SEND MESSENGER  MESSAGE : Change GroupName ");
					
					int size = sizeof(MMSG_RENAME_GROUP_REQ);
					MMSG_RENAME_GROUP_REQ changegroupname;
					ZeroMemory(&changegroupname, size);
					changegroupname.sLength = size;
					changegroupname.cMessage = MMSG_NO_RENAME_GROUP_REQ_C_S;
					
					strcpy( changegroupname.cGroupName, pMessenger_Window->m_SelectedGroupname);					
					strcpy( changegroupname.cReName, pMessenger_Window->m_GroupReName);					
					
					if(send(m_hMessengerSocket, (char *)&changegroupname, size, 0) == SOCKET_ERROR)
					{
						ProcessMessengerNetworkError();
						return FALSE;
					}
				}
			}			
		}
		break;
	}
	return TRUE;
}
void _XNetwork::MMSGMessengerChattingMessage(MMSG_CHAT* chattingmessage, BOOL GMchat)
{
	int windowid = 0;
	bool bfindmessengerdialog = FALSE;
	for( windowid = 60000; windowid < 60050; ++windowid )
	{
		_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
		if( pMessengerDialog_Window )
		{
			// 보낸 사람으로 윈도우를 찾는다 
			if(strcmpi(chattingmessage->cFromCharacName, pMessengerDialog_Window->m_ToCharactername) == 0)
			{
				if(strcmpi(chattingmessage->cToCharacName, g_pLocalUser->m_CharacterInfo.charactername) == 0)
				{
					bfindmessengerdialog = TRUE;
					
					if(pMessengerDialog_Window->m_MessengerChatList.getitemcount() > _XDEF_CHATLIST_QUESIZE)
					{
						pMessengerDialog_Window->m_MessengerChatList.deleteItem(0);
					}
					
					
					// 말하는 사람 표시
					TCHAR	speaker[256];
					DWORD	attrib;
					int		tabwidth = 5;
											
					memset(speaker, 0, sizeof(speaker));
					_snprintf( speaker, sizeof(speaker), "%s :", chattingmessage->cFromCharacName );
					
					attrib = _XDEF_CHATMESSAGECOLOR_NORMAL;
					_XStringItem* messengerchatmessage = new _XStringItem( speaker, attrib, g_LocalSystemTime);
					if(messengerchatmessage)
					{
						pMessengerDialog_Window->m_MessengerChatList.resetList();
						pMessengerDialog_Window->m_MessengerChatList.insertItem(messengerchatmessage);
					}

					// 말하는 내용 표시 					
					TCHAR	chatbuffer[256];
					memset(chatbuffer, 0, sizeof(TCHAR)*256);  
					memcpy(chatbuffer, chattingmessage->cChatMsg, chattingmessage->sLength - 29 ); // 29는 패킷에서 채팅메세지를 제외한 길이이다.
					
#ifdef _XDEF_NEWVERSONFILETERING
					g_WordFilteringManager.Filtering( chatbuffer,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
					g_WordFilteringManager.Filtering( chatbuffer, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif

					_XStringSplitInfo splitinfo;
					int viewsize = pMessengerDialog_Window->GetWindowSize().cx - 48;

					if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					{
						g_XBaseFont->SplitString(chatbuffer, viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
					}
					else
					{
						g_XBaseFont->SplitString(chatbuffer, viewsize, splitinfo);
					}
					
					TCHAR	splitstring[256];
					for(int i = 0 ; i < splitinfo.splitcount ; ++i)
					{
						memset(splitstring, 0, sizeof(TCHAR)*256);
						strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
						
						attrib = (tabwidth << 16) | _XDEF_CHATMESSAGECOLOR_NORMAL;
						_XStringItem* messengerchatmessage = new _XStringItem(splitstring, attrib, g_LocalSystemTime);
						if(messengerchatmessage)
						{
							pMessengerDialog_Window->m_MessengerChatList.resetList();
							pMessengerDialog_Window->m_MessengerChatList.insertItem(messengerchatmessage);
						}
					}

					
					if( pMessengerDialog_Window->m_MessengeChatListScrollBar )
					{
						pMessengerDialog_Window->m_MessengeChatListScrollBar->SetTotalLineCount( pMessengerDialog_Window->m_MessengerChatList.getitemcount() );
						pMessengerDialog_Window->m_MessengeChatListScrollBar->UpdateData();
						pMessengerDialog_Window->m_MessengeChatListScrollBar->SetLineScrollPos( 999999 ); 
					}	
					
					if( pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_INSTANCE )
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

	if( !bfindmessengerdialog )// 메신져 대화창을 새로 만들어서 대화를 표시한다
	{		
		_XWindow_MessengerDialog* pMessengerDialog_Window = _XWindow_MessengerDialog::CreateMessengerDialogBox( &g_MainWindowManager );
		if( pMessengerDialog_Window )
		{
			pMessengerDialog_Window->SetGMChatting(GMchat);// GM과의 채팅인지 아닌지를 저장한다.
			for( int i = 0; i < 50; ++i)
			{
				if( !m_bMessengerInstanceMessagePosition[i] )
				{
					m_bMessengerInstanceMessagePosition[i] = TRUE;
					pMessengerDialog_Window->m_InstanceMessagePositionNumber = i;
					pMessengerDialog_Window->MoveWindow( gnWidth - _XDEF_INSTANCEMESSAGEWINDOW_WIDTH - (4*i), 214 + (27*i) );
					break;
				}
			}

			strcpy( pMessengerDialog_Window->m_ToCharactername, chattingmessage->cFromCharacName );			
			if(strcmpi(chattingmessage->cToCharacName, g_pLocalUser->m_CharacterInfo.charactername) == 0)
			{				
				if(pMessengerDialog_Window->m_MessengerChatList.getitemcount() > _XDEF_CHATLIST_QUESIZE)
				{
					pMessengerDialog_Window->m_MessengerChatList.deleteItem(0);
				}
				
				
				// 말하는 사람 표시
				TCHAR	speaker[256];
				DWORD	attrib;
				int		tabwidth = 5;
				
				memset(speaker, 0, sizeof(speaker));
				_snprintf( speaker, sizeof(speaker), "%s :", chattingmessage->cFromCharacName );
				
				attrib = _XDEF_CHATMESSAGECOLOR_NORMAL;
				_XStringItem* messengerchatmessage = new _XStringItem( speaker, attrib, g_LocalSystemTime);
				if(messengerchatmessage)
				{
					pMessengerDialog_Window->m_MessengerChatList.resetList();
					pMessengerDialog_Window->m_MessengerChatList.insertItem(messengerchatmessage);
				}
				
				// 말하는 내용 표시 					
				TCHAR	chatbuffer[256];
				memset(chatbuffer, 0, sizeof(TCHAR)*256);  
				memcpy(chatbuffer, chattingmessage->cChatMsg, chattingmessage->sLength - 29 ); // 29는 패킷에서 채팅메세지를 제외한 길이이다.
				
#ifdef _XDEF_NEWVERSONFILETERING
				g_WordFilteringManager.Filtering( chatbuffer,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
				g_WordFilteringManager.Filtering( chatbuffer, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif

				_XStringSplitInfo splitinfo;
				int viewsize = pMessengerDialog_Window->GetWindowSize().cx - 30;

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SplitString(chatbuffer, viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
				}
				else
				{
					g_XBaseFont->SplitString(chatbuffer, viewsize, splitinfo);
				}
				
				TCHAR	splitstring[256];
				for(int i = 0 ; i < splitinfo.splitcount ; ++i)
				{
					memset(splitstring, 0, sizeof(TCHAR)*256);
					strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
					
					attrib = _XDEF_CHATMESSAGECOLOR_NORMAL;
					
					attrib = (tabwidth << 16) | _XDEF_CHATMESSAGECOLOR_NORMAL;
					_XStringItem* messengerchatmessage = new _XStringItem(splitstring, attrib, g_LocalSystemTime);
					if(messengerchatmessage)
					{
						pMessengerDialog_Window->m_MessengerChatList.resetList();
						pMessengerDialog_Window->m_MessengerChatList.insertItem(messengerchatmessage);
					}
				}
				
				if( pMessengerDialog_Window->m_MessengeChatListScrollBar )
				{
					pMessengerDialog_Window->m_MessengeChatListScrollBar->SetTotalLineCount( pMessengerDialog_Window->m_MessengerChatList.getitemcount() );
					pMessengerDialog_Window->m_MessengeChatListScrollBar->UpdateData();
					pMessengerDialog_Window->m_MessengeChatListScrollBar->SetLineScrollPos( 999999 ); 
				}	
				
				pMessengerDialog_Window->SetMode( _XMESSENGERDIALOGMODE_INSTANCE );
				g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );
				pMessengerDialog_Window->m_bMessageReceive = TRUE;
			}
		}
	}	

}

void _XNetwork::MMSGMessengerRequestFriend(MMSG_MAKE_FRIEND_REQUEST*joinfriend)
{
	if( !g_bCommunityOptionRejectFriend )
	{
		if(joinfriend->cCharacName[0] != 0)
		{
			TCHAR tostring[256];
			memset(tostring, 0, sizeof(tostring));

			_snprintf(tostring, sizeof(tostring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_REQUESTFRIEND), joinfriend->cCharacName);
			
			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				memset( pRequestParty_Window->m_RequestCharacterName, 0, sizeof(pRequestParty_Window->m_RequestCharacterName) );
				strncpy( pRequestParty_Window->m_RequestCharacterName , joinfriend->cCharacName , sizeof(pRequestParty_Window->m_RequestCharacterName));
			}
			
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			
			if( pMessenger_Window->m_TotalCountFriends <= 50) // 최대 50명까지 가능
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tostring, TRUE, _XDEF_REQUESTPARTY_FRIENDOK, _XDEF_REQUESTPARTY_FRIENDNO);
				pMessageBox->StartTimer(10000, TRUE);
			}
			else
			{
				// 50명이 넘어서 자동으로 거부함.
				g_NetworkKernel.SendMessengerPacket(MMSG_NO_MAKE_FRIEND_CON_C_S, 1);
			}
		}
		else
		{
			_XLog("RECEIVE JOIN_REQ : Playername is NULL");
		}
	}
	else // 친구 등록 거부 옵션에 켜 있을 때 
	{
		_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
		if(pRequestParty_Window)
		{
			strncpy( pRequestParty_Window->m_RequestCharacterName, joinfriend->cCharacName, sizeof(pRequestParty_Window->m_RequestCharacterName) );
		}
		SendMessengerPacket(MMSG_NO_MAKE_FRIEND_CON_C_S, 1);
	}

}

void _XNetwork::MMSGMessengerMemberInfo(MMSG_FRIEND_LIST_RES* messengermemberinfo)
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	pMessenger_Window->m_TotalCountFriends = 0;
	pMessenger_Window->ReleaseMessengerList();	
	
	_XMessengerGroup* messengergroup = NULL;
	_XMessengerFriend* messengerfriend = NULL;
	
	_XLog( "<<<<<<<<<<<<<<<<<<<<<<<<<<<      Messenger Memberinfo Receive     >>>>>>>>>>>>>>>>>>>>>>>>>" );
	
	bool completemessengergroup = false;
	for( int i = 0; i < messengermemberinfo->cCount; ++i ) 
	{
		if( messengermemberinfo->_friends[i].cChoiceName[0] == _GROUP_PARSING_CHAR_ ) 
		{	
			if(messengergroup)
			{
				pMessenger_Window->m_MessengerGroupList.insertItem(messengergroup);
				messengergroup = NULL;
				completemessengergroup = true;
			}
			
			
			completemessengergroup = false;
			messengergroup = new _XMessengerGroup;
			messengergroup->m_FriendList.disposeList();
			if(messengergroup)
			{
				strcpy(messengergroup->m_GroupName,messengermemberinfo->_friends[i].cChoiceName+1);
				_XLog("Group Name [%s] ", messengermemberinfo->_friends[i].cChoiceName+1 );
			}
		}
//		else if( messengermemberinfo->_friends[i].cChoiceName[0] == 0 ) 
//		{
//			// 끝이면 마지막 그룹을 저장하고 끝낸다.
//			if(messengergroup)
//			{
//				pMessenger_Window->m_MessengerGroupList.insertItem(messengergroup);
//				messengergroup = NULL;
//				completemessengergroup = true;
//			}
//			break;
//		}
		else 
		{
			if(messengergroup)
			{
				messengerfriend = new _XMessengerFriend;
				if(messengerfriend)
				{
					strcpy(messengerfriend->m_FriendName,messengermemberinfo->_friends[i].cChoiceName);					
					messengerfriend->m_FriendState = (int)messengermemberinfo->_friends[i].cState;
					_XLog("Friend Name [%s] ", messengermemberinfo->_friends[i].cChoiceName );
				}
				messengergroup->m_FriendList.insertItem(messengerfriend);
				pMessenger_Window->m_TotalCountFriends ++;
				messengerfriend = NULL;
			}
			else
			{
				// 처음에 그룹이 체크되지 않고 친구명단에 들어간 경우 : 친구 
				// 그룹이 체크되지 않는 명단이 있다면 처음에 한번 이곳에 들어온다. 
				completemessengergroup = false;
				messengergroup = new _XMessengerGroup;
				messengergroup->m_FriendList.disposeList();
				if(messengergroup)
				{
					if( messengermemberinfo->_friends[i].cChoiceName[0] != 0 )
					{
						strcpy(messengergroup->m_GroupName, _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FRIEND));
						messengergroup->m_bSelected = TRUE;

						messengerfriend = new _XMessengerFriend;
						if(messengerfriend)
						{
							strcpy(messengerfriend->m_FriendName,messengermemberinfo->_friends[i].cChoiceName);
							messengerfriend->m_FriendState =(int) messengermemberinfo->_friends[i].cState;
							_XLog("Friend Name [%s] ", messengermemberinfo->_friends[i].cChoiceName );
							messengergroup->m_FriendList.insertItem(messengerfriend);
							pMessenger_Window->m_TotalCountFriends ++;
							messengerfriend = NULL;						
						}
					}					
				}
			}
		}
	}	

	if( !completemessengergroup )
	{
		pMessenger_Window->m_MessengerGroupList.insertItem(messengergroup);
		messengergroup = NULL;
		completemessengergroup = true;
	}

	pMessenger_Window->SortGroupName();
	pMessenger_Window->SortFriendName();
	
	if(pMessenger_Window->GetShowStatus())
		pMessenger_Window->SetListItem();
	
}

void _XNetwork::MMSGMessengerResponseMakeFriend(MMSG_MAKE_FRIEND_RES *responsemakefriend)
{
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	
	switch(responsemakefriend->cResult) 
	{
	case 0:// 성공	(양쪽다 통보)
		{
			
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			_XMessengerFriend* messengerfriend = NULL;
			
			if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
			{
				pMessenger_Window->m_MessengerGroupList.resetList();
				_XMessengerGroup* currentgroup = NULL;
				bool check = FALSE;

				do 
				{
					if(check) break;
					currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
					if( strcmp(currentgroup->m_GroupName, _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FRIEND)) == 0 )
					{
						currentgroup->m_FriendList.resetList();
						messengerfriend = new _XMessengerFriend;

						if(messengerfriend)
						{
							strcpy( messengerfriend->m_FriendName, responsemakefriend->cCharacName);
							messengerfriend->m_FriendState = (int)responsemakefriend->cState;
							
							currentgroup->m_FriendList.insertItem(messengerfriend);	
							pMessenger_Window->m_TotalCountFriends ++;	
							check = TRUE;
						}
					}
					pMessenger_Window->m_MessengerGroupList.nextItem();
				} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
				
				if( !check )
				{
					pMessenger_Window->m_MessengerGroupList.resetList();
					
					_XMessengerGroup* messengergroup = new _XMessengerGroup;
					if(messengergroup)
					{
						messengergroup->m_FriendList.resetList();
						
						strcpy(messengergroup->m_GroupName,_XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FRIEND));
						
						messengerfriend = new _XMessengerFriend;
						if(messengerfriend)
						{
							strcpy(messengerfriend->m_FriendName,responsemakefriend->cCharacName);
							messengerfriend->m_FriendState = (int)responsemakefriend->cState;
							
							messengergroup->m_FriendList.insertItem(messengerfriend);
							pMessenger_Window->m_TotalCountFriends ++;
						}
					}
					
					pMessenger_Window->m_MessengerGroupList.insertItem(messengergroup);				
					pMessenger_Window->SortGroupName();
				}
			}
			else // 그룹이 없을 때 
			{
				pMessenger_Window->m_MessengerGroupList.resetList();
				
				_XMessengerGroup* messengergroup = new _XMessengerGroup;
				if(messengergroup)
				{
					messengergroup->m_FriendList.resetList();
					
					strcpy(messengergroup->m_GroupName,_XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FRIEND));

					messengerfriend = new _XMessengerFriend;
					if(messengerfriend)
					{
						strcpy(messengerfriend->m_FriendName,responsemakefriend->cCharacName);
						messengerfriend->m_FriendState = (int)responsemakefriend->cState;
						
						messengergroup->m_FriendList.insertItem(messengerfriend);
						pMessenger_Window->m_TotalCountFriends ++;
					}
				}

				pMessenger_Window->m_MessengerGroupList.insertItem(messengergroup);
				pMessenger_Window->SortGroupName();
			}

			pMessenger_Window->SortFriendName();
			
			if(pMessenger_Window->GetShowStatus())
				pMessenger_Window->SetListItem();
			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_REGISTFRIEND), responsemakefriend->cCharacName);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
			
		}
		break;
	case 1:// 상대가 거부 (거부당한 캐릭터(cFromCharacName)에게 통보)
		{			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_REJECTFRIEND), responsemakefriend->cCharacName );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
			
		}
		break;
	//(여기 부터는 'MMSG_MAKE_FRIEND_REQ' Packet 을 보낸 캐릭터에게만 통보한다.)
	case 2:// 2 : 상대 캐릭터가 없거나.. 로그오프중..
		{			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FAILFRIEND) );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
			
		}
		break;
	case 3:// 3 : 상대 캐릭터의 친구 허용 인원 초과..
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_EXCESSFRIEND) );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
		}
		break;
	case 4:// 4 : 나의 캐릭터 허용 인원 초과.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FAILFRIEND) );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
		}
		break;
	case 5:// 5 : 이미 등록된 캐릭터
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_ALREADYFRIEND) );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
		}
		break;
	case 6:// 6 : GM 캐릭터 입니다
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2775));	// "GM에게는 친구 신청을 할 수 없습니다."
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
		}
		break;
	default:
		break;
	}

	// 2004.05.19->oneway48 insert
//	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
	
//	pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
	// insert end
}

void _XNetwork::SendMessengerChatMessage( LPSTR chatmessage, int windowid, int messagemode   )
{

#ifdef _XDWDEBUG	
	if( g_SinglePlayMode || g_ZeroLevelMode )		return;		
#endif

	if( !m_bMessengerServerConnected ) return;
	if( strlen( chatmessage ) <= 0 ) return;

	_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
	if( !pMessengerDialog_Window )return;
		
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	if( !pMessenger_Window ) return;
	
	int   ChatUserStatus = 0; 
		
	if( !pMessengerDialog_Window->m_bGMChatting ) // 대화 상대가 GM이면 메신져 친구 리스트를 검사하지 않는다.
	{
		// 선택한 친구의 상태가 로그오프 상태이면 채팅 메세지를 보내지 않는다.
		if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
		{
			pMessenger_Window->m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			do 
			{
				currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(!currentgroup->m_FriendList.listEmpty())
					{
						currentgroup->m_FriendList.resetList();
						_XMessengerFriend* currentfriend = NULL;
						do 
						{
							currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
							if(currentfriend)
							{
								if( strcmp( currentfriend->m_FriendName, pMessengerDialog_Window->m_ToCharactername ) == 0 )
								{
									ChatUserStatus = currentfriend->m_FriendState;
									break;									
								}
							}
							currentgroup->m_FriendList.nextItem();							
						} while(!currentgroup->m_FriendList.atHeadOfList());						
					}					
				}			
				pMessenger_Window->m_MessengerGroupList.nextItem();
			} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
		}

		if( ChatUserStatus == 0 )
		{
			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_LOGOFFTARGET));
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
			
			return;
		}
	}
				


	if( pMessengerDialog_Window->m_MessengerChatList.getitemcount() >= _XDEF_CHATLIST_QUESIZE )
	{
		pMessengerDialog_Window->m_MessengerChatList.deleteItem( 0 );
	}	
	
	//자신이 말한 내용을 표시하는 부분 
	// 말한 사람 표시
	TCHAR	speaker[256];
	DWORD	attrib;
	int		tabwidth = 5;
	
	memset(speaker, 0, sizeof(speaker));
	_snprintf( speaker, sizeof(speaker), "%s :", g_pLocalUser->m_CharacterInfo.charactername );
	
	attrib = _XDEF_CHATMESSAGECOLOR_NORMAL;
	_XStringItem* messengerchatmessage = new _XStringItem( speaker, attrib, g_LocalSystemTime);
	if(messengerchatmessage)
	{
		pMessengerDialog_Window->m_MessengerChatList.resetList();
		pMessengerDialog_Window->m_MessengerChatList.insertItem(messengerchatmessage);
	}
	
	// 말한 내용 표시 			
	TCHAR	tempbuffer[256];
	memset(tempbuffer, 0, sizeof(TCHAR)*256);
	TCHAR chatbuffer[_XDEF_CHATMAXLENGTH]; 
	memset( chatbuffer, 0x00, _XDEF_CHATMAXLENGTH );
	
	strcpy( chatbuffer, chatmessage );
	memset( tempbuffer, 0, 256 );
	int stringlength = strlen( chatbuffer );		
	if( stringlength > _XDEF_CHATMAXLENGTH-1 )	
	{
		memcpy( tempbuffer, chatbuffer, _XDEF_CHATMAXLENGTH-1 );	
		stringlength = _XDEF_CHATMAXLENGTH-1;
	}
	else	
	{
		memcpy( tempbuffer, chatbuffer, stringlength );
	}
	
#ifdef _XDEF_NEWVERSONFILETERING
	g_WordFilteringManager.Filtering( tempbuffer,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
	g_WordFilteringManager.Filtering( tempbuffer, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif
	_XStringSplitInfo splitinfo;
	int viewsize = pMessengerDialog_Window->GetWindowSize().cx - 48;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SplitString(tempbuffer, viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
	}
	else
	{
		g_XBaseFont->SplitString(tempbuffer, viewsize, splitinfo);
	}
	
	TCHAR	splitstring[256];
	for(int i = 0 ; i < splitinfo.splitcount ; ++i)
	{
		memset(splitstring, 0, sizeof(TCHAR)*256);
		strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
		
		
		attrib = (tabwidth << 16) | _XDEF_CHATMESSAGECOLOR_NORMAL;
		_XStringItem* messengerchatmessage = new _XStringItem(splitstring, attrib, g_LocalSystemTime);
		if(messengerchatmessage)
		{
			pMessengerDialog_Window->m_MessengerChatList.resetList();
			pMessengerDialog_Window->m_MessengerChatList.insertItem(messengerchatmessage);
		}
	}
	//자신이 말한 내용을 표시하는 부분 끝 
	
	if( pMessengerDialog_Window->m_MessengeChatListScrollBar )
	{
		pMessengerDialog_Window->m_MessengeChatListScrollBar->SetTotalLineCount( pMessengerDialog_Window->m_MessengerChatList.getitemcount() );
		pMessengerDialog_Window->m_MessengeChatListScrollBar->UpdateData();
		pMessengerDialog_Window->m_MessengeChatListScrollBar->SetLineScrollPos( 999999 ); 
	}
	
	// 메신져 채팅 내용 패킷 보내는 부분	
	if( pMessengerDialog_Window->m_ToCharactername[0] != 0 )
	{
		if( !pMessengerDialog_Window->m_bGMChatting )  
		{
			if( pMessenger_Window->m_CurrentUserStatus != 0 )	// 자신이 로그오프상태이면 메세지를 보내지 않는다.
			{
				if( ChatUserStatus != 0 )
				{
					MMSG_CHAT messengerchat ;
					memset( &messengerchat, 0, sizeof(MMSG_CHAT) );
					
					int size = sizeof( MMSG_CHAT );
					messengerchat.cMessage = MMSG_NO_CHAT;
					strcpy( messengerchat.cFromCharacName, g_pLocalUser->m_CharacterInfo.charactername ); 
					strcpy( messengerchat.cToCharacName, pMessengerDialog_Window->m_ToCharactername  );	
					
					strcpy( messengerchat.cChatMsg , tempbuffer );
					messengerchat.sLength = sizeof(MMSG_CHAT) - ( _XDEF_CHATMAXLENGTH - stringlength ) ;
					messengerchat.cChatMsg[ stringlength ] = 0 ;
					
					VIEWPACKETLOG
						_XDWPACKETLOG( "SEND MESSENGER MESSAGE : Messenger Chatting message [%s]", tempbuffer );
					
					if( send ( m_hMessengerSocket, (char*)&messengerchat, messengerchat.sLength, 0 ) == SOCKET_ERROR )
					{		
						ProcessMessengerNetworkError();		
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTLATER), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
//						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
//						TCHAR messagestring[256];
//						memset(messagestring, 0, sizeof(TCHAR)*256);
//						strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILSENDDATA));
//						g_MessageBox.SetMessage( "%s [_ECode:%d]", messagestring, WSAGetLastError() );
//						g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
						_XUpdateWindow();
						_XLog("Messenger send error : [_ECode:%d]", WSAGetLastError());
						return;
					}
				}
			}
		}
		else// 대화 상대가 GM이면
		{
			MMSG_CHAT messengerchat ;
			memset( &messengerchat, 0, sizeof(MMSG_CHAT) );
			
			int size = sizeof( MMSG_CHAT );
			messengerchat.cMessage = MMSG_NO_CHAT_GM;
			strcpy( messengerchat.cFromCharacName, g_pLocalUser->m_CharacterInfo.charactername ); 
			strcpy( messengerchat.cToCharacName, pMessengerDialog_Window->m_ToCharactername  );	
			
			strcpy( messengerchat.cChatMsg , tempbuffer );
			messengerchat.sLength = sizeof(MMSG_CHAT) - ( _XDEF_CHATMAXLENGTH - stringlength ) ;
			messengerchat.cChatMsg[ stringlength ] = 0 ;
			
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSENGER MESSAGE : Messenger Chatting message [%s]", tempbuffer );
			
			if( send ( m_hMessengerSocket, (char*)&messengerchat, messengerchat.sLength, 0 ) == SOCKET_ERROR )
			{		
				ProcessMessengerNetworkError();
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTLATER), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
//				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
//				TCHAR messagestring[256];
//				memset(messagestring, 0, sizeof(TCHAR)*256);
//				strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILSENDDATA));
//				g_MessageBox.SetMessage( "%s [_ECode:%d]", messagestring, WSAGetLastError() );
//				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				_XUpdateWindow();
				_XLog("Messenger send error : [_ECode:%d]", WSAGetLastError());
				return;
			}
			
		}
	}
}


void _XNetwork::MMSGMessengerResultCutFriend(MMSG_CUT_FRIEND_REPORT* resultcutfriend)
{

	if( resultcutfriend->cCharacName[0] != 0 )
	{
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(TCHAR)*256);

		_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
		
		_XMessengerFriend* messengerfriend = NULL;
		
		if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
		{
			pMessenger_Window->m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			do 
			{
				currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(!currentgroup->m_FriendList.listEmpty())
					{
						currentgroup->m_FriendList.resetList();
						_XMessengerFriend* currentfriend = NULL;
						do 
						{
							currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
							if(currentfriend)
							{
								if( strcmp( currentfriend->m_FriendName, resultcutfriend->cCharacName ) == 0 )
								{
									currentgroup->m_FriendList.deleteItem( currentfriend );
									pMessenger_Window->m_TotalCountFriends --;
									
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									
									TCHAR messagestring[256];
									memset( messagestring, 0, sizeof(messagestring) );
									_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_DELETEFRIEND), resultcutfriend->cCharacName);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
									
									memset(pMessenger_Window->m_SelectedCharactername, 0 , sizeof(TCHAR)*13); // 임시 저장공간도 지운다.
									
									break;									
								}
							}
							currentgroup->m_FriendList.nextItem();							
						} while(!currentgroup->m_FriendList.atHeadOfList());						
					}					
				}			
				pMessenger_Window->m_MessengerGroupList.nextItem();
			} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
		}
		
		if(pMessenger_Window->GetShowStatus())
			pMessenger_Window->SetListItem();

		//Author : 양희왕 //창을 닫고 리콜이나 이동이 안되게 설정
	#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
		_XWindow_RecallItem* pRecallItemWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
		if( pRecallItemWindow )
		{
			pRecallItemWindow->ShowWindow(FALSE);
			pRecallItemWindow->SetRecallIndex(0);
		}
	#endif
	}

//	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
						
//	pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
}

void _XNetwork::MMSGMessengerResponseMakeGroup(MMSG_MAKE_GROUP_RES* responsemakegroup)
{
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	
	switch(responsemakegroup->cResult) 
	{
	case 0:		//  성공
		{			
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			
			if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
			{
				pMessenger_Window->m_MessengerGroupList.resetList();
				_XMessengerGroup* messengergroup = new _XMessengerGroup;
				
				if(messengergroup && responsemakegroup->cGroupName[0] != 0)
				{
					strcpy( messengergroup->m_GroupName, responsemakegroup->cGroupName );
					
					pMessenger_Window->m_MessengerGroupList.insertItem( messengergroup );
					
					pMessenger_Window->SortGroupName();

					if(pMessenger_Window->GetShowStatus())
						pMessenger_Window->SetListItem();
					
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(messagestring) );
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_CREATEGROUP), responsemakegroup->cGroupName);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
				}				
			}	
			else // 그룹이 없을 때 
			{
				pMessenger_Window->m_MessengerGroupList.resetList();
				
				_XMessengerGroup* messengergroup = new _XMessengerGroup;
				
				if(messengergroup && responsemakegroup->cGroupName[0] != 0)
				{
					strncpy( messengergroup->m_GroupName, responsemakegroup->cGroupName, sizeof(messengergroup->m_GroupName) );
					
					pMessenger_Window->m_MessengerGroupList.insertItem( messengergroup );
					
					pMessenger_Window->SortGroupName();
					
					if(pMessenger_Window->GetShowStatus())
						pMessenger_Window->SetListItem();
					
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(messagestring) );
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_CREATEGROUP), responsemakegroup->cGroupName);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
				}
			}			
		}
		break;
	case 1:		// 실패
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FAILCREATE) );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
		}
		break;
	}

//	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
	
//	pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
	
}


void _XNetwork::MMSGMessengerResponseDeleteGroup(MMSG_CUT_GROUP_REQ* responsedeletegroup)
{
	
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	
	if( responsedeletegroup->cGroupName[0] != 0 ) 
	{		
		_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
		
		if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
		{
			pMessenger_Window->m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			
			do 
			{
				currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if( strcmp( currentgroup->m_GroupName, responsedeletegroup->cGroupName ) == 0 )
					{
						
						pMessenger_Window->m_MessengerGroupList.deleteItem( currentgroup );
						
						if(pMessenger_Window->GetShowStatus())
							pMessenger_Window->SetListItem();
						
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						TCHAR messagestring[256];
						memset( messagestring, 0, sizeof(messagestring) );
						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_DELETEGROUP), responsedeletegroup->cGroupName);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
						break;	
					}				
					
				}	
				pMessenger_Window->m_MessengerGroupList.nextItem();
			} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());			
		}			
	}

//	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
	
//	pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
	
}

void _XNetwork::MMSGMessengerResponseMoveGroup(MMSG_JOIN_GROUP_RES* responsemovegroup)
{
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	

	switch(responsemovegroup->cResult) 
	{
	case 0:		//  성공
		{	
			int friendstatus;

			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			_XMessengerFriend* messengerfriend = NULL;
			
			if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
			{
				bool checkinsertfriend = FALSE;
				bool checkdeletefriend = FALSE;

				//이동하기 전 그룹의 친구명단에서 삭제하는 부분
				pMessenger_Window->m_MessengerGroupList.resetList();
				_XMessengerGroup* currentgroup = NULL;
							
				do 
				{
					if( checkdeletefriend ) break;
					currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
					
					

					if( strcmp( currentgroup->m_GroupName, responsemovegroup->cGroupName) != 0  )
					{						
						if(!currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_FriendList.resetList();
							_XMessengerFriend* currentfriend = NULL;
							do 
							{
								currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
								if(currentfriend)
								{
									if( strcmp( currentfriend->m_FriendName, responsemovegroup->cFriendName) == 0 )
									{										
										friendstatus = currentfriend->m_FriendState;
										currentgroup->m_FriendList.deleteItem( currentfriend );
										checkdeletefriend = TRUE;
										break;																										
									}
								}
								else
								{
									break;
								}
								currentgroup->m_FriendList.nextItem();							
							} while(!currentgroup->m_FriendList.atHeadOfList());						
						}									
					}		
					pMessenger_Window->m_MessengerGroupList.nextItem();
				} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());	


				//이동된 그룹에 친구를 추가하는 부분
				pMessenger_Window->m_MessengerGroupList.resetList();
				currentgroup = NULL;
				do 
				{
					if( checkinsertfriend ) break;
					currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
						
					if( strcmp( currentgroup->m_GroupName, responsemovegroup->cGroupName ) == 0 )
					{
						currentgroup->m_FriendList.resetList();
						messengerfriend = new _XMessengerFriend;
						
						if(messengerfriend)
						{
							strcpy( messengerfriend->m_FriendName, responsemovegroup->cFriendName );
							messengerfriend->m_FriendState = friendstatus;
						}
						currentgroup->m_FriendList.insertItem(messengerfriend);	
						checkinsertfriend = TRUE;							
						
					}

					pMessenger_Window->m_MessengerGroupList.nextItem();
				} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());								
			}
			
			pMessenger_Window->SortFriendName();
			
			if(pMessenger_Window->GetShowStatus())
				pMessenger_Window->SetListItem();
			
		}
		break;
	case 1:		// 실패
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FAILMOVEGROUP) );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
		}
		break;
	}
	
//	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
		
//	pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
	
}


void _XNetwork::MMSGMessengerResponseChangeGroupName(MMSG_RENAME_GROUP_RES* responsechagegroupname)
{	
	switch(responsechagegroupname->cResult) 
	{
	case 0:		//  성공
		{	
			_XWindow_NameInput* pNameInput_Window = (_XWindow_NameInput*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NAMEINPUTWINDOW);
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			
			if( pMessenger_Window->m_GroupName[0] == 0 || pMessenger_Window->m_GroupReName[0] == 0 ) break;

			if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
			{
				pMessenger_Window->m_MessengerGroupList.resetList();
				_XMessengerGroup* currentgroup = NULL;
				
				do 
				{
					currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
					if(currentgroup)
					{
						if( strcmp( currentgroup->m_GroupName, pMessenger_Window->m_GroupName ) == 0 )
						{
							memset( currentgroup->m_GroupName, 0 ,sizeof(TCHAR)*15 );
							strcpy( currentgroup->m_GroupName, pMessenger_Window->m_GroupReName);

							
							pMessenger_Window->SortGroupName();
							
							if(pMessenger_Window->GetShowStatus())
								pMessenger_Window->SetListItem();
							
							memset( pMessenger_Window->m_GroupName, 0 ,sizeof(TCHAR)*15 );
							memset( pMessenger_Window->m_GroupReName, 0 ,sizeof(TCHAR)*15 );
							break;	
						}				
						
					}	
					pMessenger_Window->m_MessengerGroupList.nextItem();
				} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());			
			}						
		}
		break;
	case 1:		// 실패
		{			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FAILRENAME) );
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
		}
		break;
	}

//	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
	
//	pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
}

void _XNetwork::MMSGMessengerUpdateStatus(MMSG_FRIEND_STATE_REPORT *userstateupdate)
{
	if( userstateupdate->cCharacName[0] != 0 )
	{
		_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
		
		_XMessengerFriend* messengerfriend = NULL;
		
		if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
		{
			pMessenger_Window->m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			bool check = FALSE;
			do 
			{
				if(check) break;
				currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(!currentgroup->m_FriendList.listEmpty())
					{
						currentgroup->m_FriendList.resetList();
						_XMessengerFriend* currentfriend = NULL;
						do 
						{
							if(check) break;
							currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
							if(currentfriend)
							{
								if( strcmp( currentfriend->m_FriendName, userstateupdate->cCharacName ) == 0 )
								{
									currentfriend->m_FriendState = (int)userstateupdate->cState ;
									
									// 게임상에 친구가 접속 상태를 알려 준다.
									if( currentfriend->m_FriendState == 0 )// 로그오프 상태
									{
										TCHAR messagestring[256];
										memset( messagestring, 0, sizeof(messagestring) );
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_1627), currentfriend->m_FriendName );
										InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
									}
									else if( currentfriend->m_FriendState == 1 )// 로그인 상태
									{
										TCHAR messagestring[256];
										memset( messagestring, 0, sizeof(messagestring) );
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_1628), currentfriend->m_FriendName );
										InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
									}
									
									check = true;							
								}
							}
							currentgroup->m_FriendList.nextItem();							
						} while(!currentgroup->m_FriendList.atHeadOfList());						
					}					
				}			
				pMessenger_Window->m_MessengerGroupList.nextItem();
			} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
		}
		
		if(pMessenger_Window->GetShowStatus())
			pMessenger_Window->SetListItem();
	}
}


/////////////////////////////////////////////////////////////////////////////////////
//                              전서구 관련 함수                                   //
/////////////////////////////////////////////////////////////////////////////////////		

BOOL _XNetwork::Send_MMSGEmail(int type, int param1, int param2)
{
	switch(type) {
	case MMSG_NO_MAIL_LIST_REQ_C_S:// 메일 리스트를 요청한다.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Email List Request =================================");
			
			int size = sizeof(MMSG_MAIL_LIST_REQ);
			MMSG_MAIL_LIST_REQ maillist;
			ZeroMemory(&maillist, size);
			maillist.header.sLength = size;
			maillist.cMessage = MMSG_NO_MAIL_LIST_REQ_C_S;
			
			if(send(m_hMessengerSocket, (char*)&maillist, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}

		}
		break;
		
	case MMSG_NO_MAIL_TEXT_REQ_C_S:// 메일 본문을 요청한다.
		{			
			_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
			if( !pEmail_Window ) break;

			int tempmailindex = -1;
			for( int i = 0; i < 20; ++i )
			{
				if( pEmail_Window->m_TempEmailInfo[i].uiIndex == (unsigned int)param1 )
				{
					tempmailindex = i;
					break;
				}
			}
			
			if( tempmailindex == -1 ) 
			{
				// 임시 저장 공간에 없으므로 서버에 요청한다.

				VIEWPACKETLOG
					_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Email Text Request =================================");
				
				int size = sizeof(MMSG_MAIL_TEXT_REQ);
				MMSG_MAIL_TEXT_REQ mailtext;
				ZeroMemory(&mailtext, size);
				mailtext.header.sLength = size;
				mailtext.cMessage = MMSG_NO_MAIL_TEXT_REQ_C_S;
				
				// 인덱스 번호를 보내는 부분 추가.
				mailtext.uiIndex = (unsigned int)param1;

				if( mailtext.uiIndex <= 0) break;//error
				
				if(send(m_hMessengerSocket, (char*)&mailtext, size, 0) == SOCKET_ERROR)
				{
					ProcessMessengerNetworkError();
					return FALSE;
				}
			}
			else
			{
				// 임시 저장 공간에서 정보를 불러온다.
				
				memset(pEmail_Window->m_EmailSender,0,sizeof(TCHAR)*20);
				memset(pEmail_Window->m_EmailText,0,sizeof(TCHAR)*101);
				strcpy( pEmail_Window->m_EmailSender, pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom );
				strncpy( pEmail_Window->m_EmailText, pEmail_Window->m_TempEmailInfo[tempmailindex].cText, pEmail_Window->m_TempEmailInfo[tempmailindex].cStrLen );
				
				pEmail_Window->m_EmailYear = pEmail_Window->m_TempEmailInfo[tempmailindex].date.year;
				pEmail_Window->m_EmailMonth = pEmail_Window->m_TempEmailInfo[tempmailindex].date.month;
				pEmail_Window->m_EmailDay = pEmail_Window->m_TempEmailInfo[tempmailindex].date.day;
				pEmail_Window->m_EmailHour = pEmail_Window->m_TempEmailInfo[tempmailindex].date.hour;
				pEmail_Window->m_EmailMinute = pEmail_Window->m_TempEmailInfo[tempmailindex].date.minute;
				
				pEmail_Window->SetEmailText( pEmail_Window->m_EmailText );

			if( pEmail_Window->m_TempEmailInfo[tempmailindex].cMode == _MailInfo::_PRIVATE )
				pEmail_Window->m_EmailColor = _XSC_DEFAULT;
			else if( pEmail_Window->m_TempEmailInfo[tempmailindex].cMode == _MailInfo::_PUBLIC )
				pEmail_Window->m_EmailColor = 0xFFBCEB7F;
			else if( pEmail_Window->m_TempEmailInfo[tempmailindex].cMode == _MailInfo::_FROM_GM)
				pEmail_Window->m_EmailColor = 0xFFE68A3F;
			}			
		}
		break;
		
	case MMSG_NO_MAIL_SEND_REQ_C_S:// 메일을 발송한다.
		{
			_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
			if( !pEmail_Window ) break;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Email Send =================================");
			
			int size = sizeof(MMSG_MAIL_SEND_REQ);
			MMSG_MAIL_SEND_REQ sendmail;
			ZeroMemory(&sendmail, size);
			sendmail.header.sLength = size;
			sendmail.cMessage = MMSG_NO_MAIL_SEND_REQ_C_S;
			sendmail.cStrLen = pEmail_Window->m_EmailTextLength;
			strncpy( sendmail.cText, pEmail_Window->m_EmailText, pEmail_Window->m_EmailTextLength);
			strcpy( sendmail.cToCharac, pEmail_Window->m_EmailReceiver );
			
			if(send(m_hMessengerSocket, (char*)&sendmail, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
		}
		break;
		
	case MMSG_NO_MAIL_DROP_C_S:// 메일 삭제.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Email Text Request =================================");
			
			int size = sizeof(MMSG_MAIL_DROP);
			MMSG_MAIL_DROP mailtext;
			ZeroMemory(&mailtext, size);
			mailtext.header.sLength = size;
			mailtext.cMessage = MMSG_NO_MAIL_DROP_C_S;
			
			// 메일 삭제 인덱스 번호를 보내는 부분 추가.
			mailtext.uiIndex = (unsigned int)param1;
			
			if( mailtext.uiIndex <= 0) break;//error
						
			if(send(m_hMessengerSocket, (char*)&mailtext, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
			
			_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
			if( !pEmail_Window ) break;
			
			int tempmailindex = -1;
			for( int i = 0; i < 20; ++i )
			{
				if( pEmail_Window->m_TempEmailInfo[i].uiIndex == (unsigned int)param1 )
				{
					tempmailindex = i;
					break;
				}
			}
			
			if( tempmailindex != -1)
			{				
				pEmail_Window->m_TempEmailInfo[tempmailindex].uiIndex		= 0;
				pEmail_Window->m_TempEmailInfo[tempmailindex].cStrLen		= 0;
				pEmail_Window->m_TempEmailInfo[tempmailindex].date.year		= 0;
				pEmail_Window->m_TempEmailInfo[tempmailindex].date.month	= 0;
				pEmail_Window->m_TempEmailInfo[tempmailindex].date.day		= 0;
				pEmail_Window->m_TempEmailInfo[tempmailindex].date.hour		= 0;
				pEmail_Window->m_TempEmailInfo[tempmailindex].date.minute	= 0;
				
				memset( pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom, 0, sizeof(TCHAR)*13 );
				memset( pEmail_Window->m_TempEmailInfo[tempmailindex].cText, 0, sizeof(TCHAR)*101 );				
			}

		}
		break;
	case MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S:
		{
			_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
			if( !pEmail_Window ) break;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSENGER  MESSAGE :  Organization Email Send =================================");
			
			int size = sizeof(MMSG_MAIL_PUBLIC_SEND_REQ);
			MMSG_MAIL_PUBLIC_SEND_REQ sendmail;
			ZeroMemory(&sendmail, size);
			sendmail.header.sLength = size;
			sendmail.cMessage = MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S;
			
			strcpy( sendmail.cText, pEmail_Window->m_EmailText);
			memset( sendmail.cNames, 0, sizeof(char)*13*100 );
			
			int count = 0;
			if( g_pGroup_Window )
			{
				list<_Unit*>::iterator iter = g_pGroup_Window->m_OrganizationMemberList.begin();
			
				for( ; iter != g_pGroup_Window->m_OrganizationMemberList.end(); ++iter )
				{
					_Unit* tempUnit = *iter;
					if( tempUnit )
					{
						if( strlen(tempUnit->m_cName) > 0 )
						{
							strcpy( sendmail.cNames[count], tempUnit->m_cName );
							count++;
						}
					}
				}
			}			
						
			if(send(m_hMessengerSocket, (char*)&sendmail, size, 0) == SOCKET_ERROR)
			{
				ProcessMessengerNetworkError();
				return FALSE;
			}
		}
		break;		
	default:
		break;
	}
	
	return TRUE;
}
void _XNetwork::MMSGEmailListResponse(MMSG_MAIL_LIST_RES* responsemaillist)
{
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	if( !pEmail_Window ) return;

	int count = 0;
	int notreadcount = 0;
	pEmail_Window->ResetEmailList();
	pEmail_Window->ResetEmailListReceiveBox();
	for( int i = 0; i < 20; ++i )
	{
		if( responsemaillist->mail[i].uiIndex > 0 )
		{
			if( responsemaillist->mail[i].check )//읽은 편지 -> 보관함
			{
				pEmail_Window->m_EmailList[count].uiIndex = responsemaillist->mail[i].uiIndex;
				pEmail_Window->m_EmailList[count].check = responsemaillist->mail[i].check;
				count++;
			}
			else if( !pEmail_Window->m_EmailList[i].check )// 새로 도착한 편지 -> 수신함
			{
				pEmail_Window->m_EmailListReceiveBox[notreadcount].uiIndex = responsemaillist->mail[i].uiIndex;
				pEmail_Window->m_EmailListReceiveBox[notreadcount].check = responsemaillist->mail[i].check;
				notreadcount++;
			}
		}
	}

	pEmail_Window->m_EmailTextTotalCount = count;
	pEmail_Window->m_indexEmailText = count;

	pEmail_Window->m_EmailReceiveTextTotalCount = notreadcount;
	pEmail_Window->m_indexEmailReceiveText = notreadcount;
	
	if( notreadcount != 0 )//새로 도착한 편지가 있을 경우 -> 수신함
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		
		TCHAR messagestring[256];
		memset( messagestring, 0, sizeof(messagestring) );
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2370), notreadcount);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);	

		pEmail_Window->ChangeTab( _XEMAILTAB_RECEIVE );
		g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, pEmail_Window->m_EmailListReceiveBox[pEmail_Window->m_indexEmailReceiveText-1].uiIndex );
	}
	else //새로 도착한 편지가 없을 경우 -> 보관함
	{
		pEmail_Window->ChangeTab( _XEMAILTAB_KEEP );
		g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, pEmail_Window->m_EmailList[pEmail_Window->m_indexEmailText-1].uiIndex );
	}	
}

void _XNetwork::MMSGEmailTextResponse(MMSG_MAIL_TEXT_RES* responsemailtext)
{
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	if( !pEmail_Window ) return;

	// 클라이언트 자체 임시 저장 공간에 저장.
	int tempmailindex = -1;
	for( int i = 0; i < 20; ++i )
	{
		if( pEmail_Window->m_TempEmailInfo[i].uiIndex == 0 )
		{
			tempmailindex = i;
			break;
		}
	}

	if( tempmailindex != -1 )
	{
		pEmail_Window->m_TempEmailInfo[tempmailindex].uiIndex = responsemailtext->mailInfo.uiIndex;
		
		pEmail_Window->m_TempEmailInfo[tempmailindex].uiIndex		= responsemailtext->mailInfo.uiIndex;
		pEmail_Window->m_TempEmailInfo[tempmailindex].cStrLen		= responsemailtext->mailInfo.cStrLen;
		pEmail_Window->m_TempEmailInfo[tempmailindex].date.year		= responsemailtext->mailInfo.date.year;
		pEmail_Window->m_TempEmailInfo[tempmailindex].date.month	= responsemailtext->mailInfo.date.month;
		pEmail_Window->m_TempEmailInfo[tempmailindex].date.day		= responsemailtext->mailInfo.date.day;
		pEmail_Window->m_TempEmailInfo[tempmailindex].date.hour		= responsemailtext->mailInfo.date.hour;
		pEmail_Window->m_TempEmailInfo[tempmailindex].date.minute	= responsemailtext->mailInfo.date.minute;
		pEmail_Window->m_TempEmailInfo[tempmailindex].cMode			= responsemailtext->mailInfo.cMode;

		if( responsemailtext->mailInfo.cMode == _MailInfo::_SYSTEM_MSG_OR_KICK)
		{
			memset(pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom, 0, sizeof(pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom));			
			strncpy( pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATION), sizeof(pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom)-1 );
			//%s 영웅단에서 강제추방 되셨습니다.
			_snprintf( pEmail_Window->m_TempEmailInfo[tempmailindex].cText, sizeof(pEmail_Window->m_TempEmailInfo[tempmailindex].cText), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2092), responsemailtext->mailInfo.cText );
		}
		else
		{
			strncpy( pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom, responsemailtext->mailInfo.cFrom, sizeof(pEmail_Window->m_TempEmailInfo[tempmailindex].cFrom) );
			strncpy( pEmail_Window->m_TempEmailInfo[tempmailindex].cText, responsemailtext->mailInfo.cText, responsemailtext->mailInfo.cStrLen );
		}
	}
	// end 클라이언트 자체 임시 저장 공간에 저장.
	

	memset(pEmail_Window->m_EmailSender,0,sizeof(pEmail_Window->m_EmailSender));
	memset(pEmail_Window->m_EmailText,0,sizeof(pEmail_Window->m_EmailText));

	if( responsemailtext->mailInfo.cMode == _MailInfo::_SYSTEM_MSG_OR_KICK)
	{
		strncpy( pEmail_Window->m_EmailSender, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATION), sizeof(pEmail_Window->m_EmailSender) );
	}
	else
	{
		strncpy( pEmail_Window->m_EmailSender, responsemailtext->mailInfo.cFrom, sizeof(pEmail_Window->m_EmailSender) );
	}

	strncpy( pEmail_Window->m_EmailText, responsemailtext->mailInfo.cText, responsemailtext->mailInfo.cStrLen );

	pEmail_Window->m_EmailYear = responsemailtext->mailInfo.date.year;
	pEmail_Window->m_EmailMonth = responsemailtext->mailInfo.date.month;
	pEmail_Window->m_EmailDay = responsemailtext->mailInfo.date.day;
	pEmail_Window->m_EmailHour = responsemailtext->mailInfo.date.hour;
	pEmail_Window->m_EmailMinute = responsemailtext->mailInfo.date.minute;

	pEmail_Window->SetEmailText( pEmail_Window->m_EmailText );

	if( responsemailtext->mailInfo.cMode == _MailInfo::_PRIVATE )
		pEmail_Window->m_EmailColor = _XSC_DEFAULT;
	else if( responsemailtext->mailInfo.cMode == _MailInfo::_PUBLIC )
		pEmail_Window->m_EmailColor = 0xFFBCEB7F;	
	else if( responsemailtext->mailInfo.cMode == _MailInfo::_FROM_GM)
		pEmail_Window->m_EmailColor = 0xFFE68A3F;
	else if( responsemailtext->mailInfo.cMode == _MailInfo::_SYSTEM_MSG_OR_KICK)
	{
		pEmail_Window->m_EmailColor = _XSC_DEFAULT;
	}
}

void _XNetwork::MMSGEmailTextSuccess(MMSG_MAIL_SEND_RES* successsendmail)
{
	// 전서구를 발송하였습니다...라는 메세지 출력만 하면 됨...특별한 처리 없음.
	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);

	pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2371), TRUE);
	
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	if( pEmail_Window ) pEmail_Window->ResetEmailText();
}

void _XNetwork::MMSGEmailTextReceive(MMSG_MAIL_RECV* receivemail)
{
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	if( !pEmail_Window ) return;
	
	for( int i = 0; i < 20; ++i )
	{
		if( pEmail_Window->m_EmailListReceiveBox[i].uiIndex <= 0 )
		{
			pEmail_Window->m_EmailListReceiveBox[i].uiIndex = receivemail->uiIndex;
			pEmail_Window->m_EmailListReceiveBox[i].check = FALSE;
			break;
		}
	}
	pEmail_Window->m_EmailReceiveTextTotalCount++;
	pEmail_Window->m_indexEmailReceiveText++;
	
	pEmail_Window->ChangeTab(_XEMAILTAB_RECEIVE);
	
	//_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);	
	//pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2372), TRUE);
	
	InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2372), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
	

}

void _XNetwork::MMSGEmailFail(MMSG_MAIL_FAIL* failemail)
{
	// cCmd : MMSG_NO_MAIL_LIST_RES_S_C : cResult == NULL
	// cCmd : MMSG_NO_MAIL_TEXT_RES_S_C : cResult == NULL
	// cCmd : MMSG_NO_MAIL_SEND_RES_S_C : cResult == 0 : 해당 캐릭터 이름 존재하지 않음
	//				     							 1 : 상대방 메일박스가 꽉차서 더이상 못받음
	//												 2 : 상대는 수신 거부중
	//												 3 : 기타 오류
	//												 4 : GM 에게는 쪽지를 보낼 수 없습니다.

	switch( failemail->cCmd ) {
	case MMSG_NO_MAIL_LIST_RES_S_C:
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2373), TRUE);
			
		}
		break;
	case MMSG_NO_MAIL_TEXT_RES_S_C:
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2374), TRUE);
		}
		break;
	case MMSG_NO_MAIL_SEND_RES_S_C:
		{
			switch(failemail->cResult) {
			case 0:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2375), TRUE);
				}
				break;
			case 1:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2376), TRUE);
				}
				break;
			case 2:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2377), TRUE);
				}
				break;
			case 3:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2378), TRUE);
				}
				break;
			case 4:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2379), TRUE);
				}
				break;
			}
			
			_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
			if( pEmail_Window ) pEmail_Window->ResetEmailText();
		}
		break;
	}
}

void _XNetwork::MMSGWhisperChatMessage( MMSG_CHAT* chattingmessage )
{
	if( !g_bCommunityOptionRejectWhisper )
	{
		if(strcmpi(chattingmessage->cToCharacName, g_pLocalUser->m_CharacterInfo.charactername) == 0)
		{
			_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
			
			if(g_Chatlist.getitemcount() > _XDEF_CHATLIST_QUESIZE)
			{
				g_Chatlist.deleteItem(0);
			}
			
			TCHAR	chatbuffer[256];
			memset( chatbuffer, 0, sizeof(chatbuffer) );		 /* header size */	
			int chatstringsize = chattingmessage->sLength - (sizeof(MMSG_CHAT) - _XDEF_CHATMAXLENGTH);
			if( chatstringsize > 0 )
				strncpy( chatbuffer, chattingmessage->cChatMsg, chatstringsize );
	
			int		tabwidth;
			TCHAR	tempbuffer[256];
			memset(tempbuffer, 0, sizeof(tempbuffer));
			_snprintf( tempbuffer, sizeof(tempbuffer), "%s:(%s) ", chattingmessage->cFromCharacName, chattingmessage->cToCharacName );
			tabwidth = g_XBaseFont->GetWidth(tempbuffer);
			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503		
			_XWindow_BlackList* pBlackList_Window = (_XWindow_BlackList*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_BLACKLISTWINDOW );
			BOOL blacklistuser = pBlackList_Window->FindCharacter( chattingmessage->cFromCharacName );
			
			if( blacklistuser )
			{
				LPTSTR pStr = chatbuffer; 
				if( pStr )
				{
					while( *pStr != NULL )
					{
						if( *pStr != _T(' ') )
						{
							*pStr = _T('*');
						}

						pStr++;
					}
				}
			}
#endif

#ifdef _XDEF_NEWVERSONFILETERING
			g_WordFilteringManager.Filtering( chatbuffer,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
			g_WordFilteringManager.Filtering( chatbuffer, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif

			strcat(tempbuffer, chatbuffer);

			strcpy( pChat_Window->m_SavedWhisperTargetName, chattingmessage->cFromCharacName );
			
			_XStringSplitInfo splitinfo;
			int viewsize = pChat_Window->GetWindowSize().cx - 28;
				
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->SplitString(tempbuffer, viewsize, splitinfo, tabwidth, _XSTRINGSPLITFUNCTION_SPACECHAR );
			}
			else
			{
				g_XBaseFont->SplitString(tempbuffer, viewsize, splitinfo, tabwidth);
			}

			TCHAR	splitstring[256];
			DWORD	attrib;
			for(int i = 0 ; i < splitinfo.splitcount ; ++i)
			{
				memset(splitstring, 0, sizeof(TCHAR)*256);
				strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
				
				attrib = _XDEF_CHATMESSAGECOLOR_WHISPER;
				_XStringItem* chatmessage = new _XStringItem(splitstring, attrib, g_LocalSystemTime);
				if(chatmessage)
				{

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503		
					chatmessage->Set_Attribute3( blacklistuser );
#endif

					if( i == 0 )
					{
						chatmessage->Set_Attribute2( strlen(chattingmessage->cFromCharacName) );
					}
					else
					{
						chatmessage->Set_Attribute2( 0x00000000 );
					}
					
					g_Chatlist.resetList();
					g_Chatlist.insertItem(chatmessage);				
				}
			}		

			if(pChat_Window && pChat_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChat_Window->m_ChatListScrollBar->SetTotalLineCount( pChat_Window->RecalcChattingListLength() );
#else				
				pChat_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChat_Window->m_ChatListScrollBar->UpdateData();
				
				// 트랙바 트래깅중에는 맨 아래아이템 자동스크롤 안함
				if(!pChat_Window->m_ChatListScrollBar->Dragging())
				{
					pChat_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}			
			}
		}

	}
	else // 귀속말 거부 옵션에 켜 있을 때 
	{
		
		MMSG_CHAT packet;
		memset( &packet, 0, sizeof( MMSG_CHAT ) );
		
		int size = sizeof( MMSG_CHAT );
		packet.cMessage = MMSG_NO_CHAT_WHISPER;
		strcpy( packet.cFromCharacName, g_pLocalUser->m_CharacterInfo.charactername );
		strcpy( packet.cToCharacName, chattingmessage->cFromCharacName );
		
		
		// 말한 내용 표시 			
		TCHAR	tempbuffer[256];
		memset(tempbuffer, 0, sizeof(TCHAR)*256);
		TCHAR chatbuffer[_XDEF_CHATMAXLENGTH]; 
		memset( chatbuffer, 0x00, _XDEF_CHATMAXLENGTH );
		strcpy( chatbuffer, _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2380) );
		int stringlength = strlen( chatbuffer );		
		if( stringlength > _XDEF_CHATMAXLENGTH-1 )	
		{
			memcpy( tempbuffer, chatbuffer, _XDEF_CHATMAXLENGTH-1 );	
			stringlength = _XDEF_CHATMAXLENGTH-1;
		}
		else	
		{
			memcpy( tempbuffer, chatbuffer, stringlength );
		}

		strcpy( packet.cChatMsg, tempbuffer );
		packet.sLength = sizeof(MMSG_CHAT) - ( _XDEF_CHATMAXLENGTH - stringlength ) ;
		packet.cChatMsg[ stringlength ] = 0 ;
		
		if( send ( m_hMessengerSocket, (char*)&packet, packet.sLength , 0 ) == SOCKET_ERROR )
		{		
			ProcessMessengerNetworkError();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTLATER), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
//			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
//			TCHAR messagestring[256];
//			memset(messagestring, 0, sizeof(TCHAR)*256);
//			strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILSENDDATA));
//			g_MessageBox.SetMessage( "%s [_ECode:%d]", messagestring, WSAGetLastError() );
//			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			_XUpdateWindow();
			_XLog("Messenger send error : [_ECode:%d]", WSAGetLastError());
			return;
		}
	}
}

void _XNetwork::MMSGWhisperChatFailMessage( MMSG_CHAT_RESULT* whisperchatmsg )
{
	switch(whisperchatmsg->cCode) 
	{
	case _NOT_FIND_USER_:		// 귓속말 상대 없음
		{
			_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);

			TCHAR targetname[256];
			memset(targetname, 0, sizeof( targetname ) );
			_snprintf( targetname, sizeof(targetname), _XGETINTERFACETEXT(ID_STRING_NETWORKMESSENGER_2381), whisperchatmsg->cToCharacName );
			_XStringItem* chattingmessage = new _XStringItem( targetname, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChat_Window && pChat_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChat_Window->m_ChatListScrollBar->SetTotalLineCount( pChat_Window->RecalcChattingListLength() );
#else				
				pChat_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChat_Window->m_ChatListScrollBar->UpdateData();
				
				// 트랙바 트래깅중에는 맨 아래아이템 자동스크롤 안함
				if(!pChat_Window->m_ChatListScrollBar->Dragging())
				{
					pChat_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}			
			}
		}
		break;
	}
}

void _XNetwork::MSGRecallItemResult( MSG_RECALL_FRIEND* pRecallresult )
{
//Author : 양희왕 
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
	_XWindow_RecallItem* pRecallItemWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
	if( !pRecallItemWindow ) return;
	
	//메세지 박스가 있으면 닫는다
	g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_MESSAGEWINDOW);


	TCHAR tempString[512];
	memset(tempString, 0, sizeof(tempString));

	switch( pRecallresult->ucFriendMode )
	{
	case en_rf_recv_request_recall :			// 친구 소환 요청을 서버에서 받음.
		{
			MSG_RF_RECV_REQUEST_RECALL* tempresult = (MSG_RF_RECV_REQUEST_RECALL*)pRecallresult;

			pRecallItemWindow->SetFromName( tempresult->cFromName );

			//내 상태를 검사하자
			if( !pRecallItemWindow->CharRecallCondition() ) 
			{
				_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3562), tempresult->cFromName, _XGETINTERFACETEXT(ID_STRING_NEW_3489) ); //상태이상으로 %s님의 %s요청을 거절 합니다.   , 호출
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

				//호출 받을 수 없는 상태
				g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_answer_no );
				break;
			}

			//친구가 등록되있는지 확인하자..
			if(!pRecallItemWindow->CheckCharacterName( tempresult->cFromName ))
			{
				_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3563), tempresult->cFromName, _XGETINTERFACETEXT(ID_STRING_NEW_3489) ); // %s님이 친구목록에 없으므로 %s요청을 거절 합니다.   , 호출
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

				//친구가등록이 안되어있음
				g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_answer_no );
				break;
			}

			_snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3540), //%s님께서 자신의 위치(%s)로 호출하였습니다. 이동하시겠습니까?
				tempresult->cFromName, g_ZoneInfoTable[tempresult->ucFromZoneIndex-1].zonename );
			
			//메세지 박스 
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO,  _T(""), tempString, 
									TRUE, _XDEF_RECALLITEM_CALL_AGREE, _XDEF_RECALLITEM_CALL_REJECT );
			pMessageBox->StartTimer( 15000, TRUE );
		}
		break;
	case en_rf_recv_answer_yes : //요청 수락 패킷을 받음 //상대방이 요청을 수락 했으므로 마법진을 만든다 //서버에서 딜레이후 타겟이 이동됨
		{
			//에니메이션
			//g_pLocalUser->SetAnimation();

			//이펙트 
			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);
			matWorld._43 = -2.0f;
			
			D3DXMATRIX matrotation;
			D3DXMatrixRotationY( &matrotation, g_pLocalUser->m_RotateAngle );

			D3DXMatrixMultiply( &matWorld, &matWorld, &matrotation );

			matWorld._41 += g_pLocalUser->m_Position.x;
			matWorld._42 += g_pLocalUser->m_Position.y;
			matWorld._43 += g_pLocalUser->m_Position.z;			

			//캐릭터, 아이디, 행렬
			//g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call, &g_pLocalUser->m_ModelDescriptor.m_Position); //matWorld );
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call, &matWorld );

			//사운드
			_XPlayInterfaceSound(ID_SR_INTERFACE_RECALL_WAV);

			MSG_RF_RECV_ANSWER_YES* tempresult = (MSG_RF_RECV_ANSWER_YES*)pRecallresult;

			_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3541), tempresult->cTargetName ); //%s님께서 소환되었습니다.
			g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

			g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Set_m_ucCount( tempresult->ucLeftCount ); //지정된 슬롯에.. 남은개수를 저장

			memset(tempString, 0, sizeof(tempString));
			_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3542) );//소환령 1개가 소모 되었습니다
			g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

			//0개
			if( g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Get_m_ucCount() < 1 )
				g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Reset();

			//소환버튼 활성화
			pRecallItemWindow->SetRecallButton( TRUE );
		}
		break;
	case en_rf_recv_answer_no : // 요청 거부 패킷을 받음.
		{
			MSG_RF_RECV_ANSWER_NO* tempresult = (MSG_RF_RECV_ANSWER_NO*)pRecallresult;

			switch( tempresult->ucNoReason )
			{
			case 0 : //거절을 선택했을 때.
				{
					_snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3543), tempresult->cTargetName ); //%s님께서 호출을 거부 하였습니다.
					//메세지 박스 
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), tempString, TRUE );
				}
				break;
			case 1 : //( 요청받았을 때, 상태검사룰 수행하여, 못 받는 상태이면 이 값으로 보낸다. )
				{
					_snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3544), _XGETINTERFACETEXT(ID_STRING_NEW_3489) ); //상대방의 상태 이상으로 %s할 수 없습니다., 호출
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), tempString, TRUE );
				}
				break;
			}

			//소환버튼 활성화
			pRecallItemWindow->SetRecallButton( TRUE );
		}
		break;
	case en_rf_fail_to_request : // 요청이 실패했다
		{
			MSG_RF_FAIL_TO_REQUEST* tempresult = (MSG_RF_FAIL_TO_REQUEST*)pRecallresult;

			switch( tempresult->ucFail )
			{
			case en_rf_answer_no : // no 선택.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3549) ); //선택된 상대가 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_no_target :	// 0 타겟이 없다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3550), tempresult->cTargetName);  //%s님께서 로그인 중이 아닙니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_no_item :  // 1 소환 아이템 없다. 
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3551) );//소환령이 존재하지 않습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_wrong_point :	// 소환 지점의 위치가 이상하다. 
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3552) );//소환 지점의 위치가 잘못되어 있습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_enemy : // 적대 세력이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3799) );//적대관계에 있어 호출/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_my_con_trade : // (자신은) 거래중이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3553) );//거래 중에는 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_my_con_combat : // (자신은) 장원전 참여중이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3554) );//장원전 참여중에는 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_my_con_zone :	// (자신은) 패왕동/호출할 수 없는 지역이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3555) );//패왕동 지역이므로 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_my_con_exbattle : // (자신은) 결 진행중이다. 
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3556) );//결 진행 중에는 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_target_con_trade :	// 거래중이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3557) );//상대방이 거래 중에는 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_target_con_combat : // 장원전 참여중이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3558) );//상대방이 장원전 참여중에는 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_target_con_zone : // 패왕동/호출할 수 없는 지역이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3559) );//상대방이 패왕동 지역이므로 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_target_con_exbattle : // 결 진행중이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3560) );//상대방이 결 진행 중에는 소환/이동을 할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_my_busy : 	// (자신은) 다른 캐릭터와 친구 소환 중이다.             
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3548) );//다른 대상과 친구 소환 중 입니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
			case en_rf_fail_target_busy : // 4 다른 대상과 친구 소환 중이다.
				{
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3561) );//상대방이 다른 대상과 친구 소환 중 입니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				break;
            case en_rf_fail_my_state_pk_mode : // (자신은 ) 대전중이다
                {
                    _snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3658) );//대전 중에는 호출/이동할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
                }
                break;
            case en_rf_fail_target_state_pk_mode : //상대방은 대전중이다
                {
                    _snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3832) );//상대방이 대전 중이므로 호출/이동할 수 없습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
                }
                break;
			}

			//소환버튼 활성화
			pRecallItemWindow->SetRecallButton( TRUE );
		}
		break;

	//이동에 관한...
	case en_rf_recv_request_goto : //이동 요청이 들어옴
		{
			MSG_RF_RECV_REQUEST_GOTO* tempresult = (MSG_RF_RECV_REQUEST_GOTO*)pRecallresult;

			pRecallItemWindow->SetFromName( tempresult->cFromName );

			//내 상태를 검사하자
			if( !pRecallItemWindow->CharRecallCondition() ) 
			{
				_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3562), tempresult->cFromName, _XGETINTERFACETEXT(ID_STRING_NEW_3490) ); //상태이상으로 %s님의 %s요청을 거절 합니다.   , 이동
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

				//호출 받을 수 없는 상태
				g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_goto_answer_no );
				break;
			}

			//친구가 등록되있는지 확인하자..
			if(!pRecallItemWindow->CheckCharacterName( tempresult->cFromName ))
			{
				_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3563), tempresult->cFromName, _XGETINTERFACETEXT(ID_STRING_NEW_3490) ); // %s님이 친구목록에 없으므로 %s요청을 거절 합니다.   , 이동
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

				//친구가등록이 안되어있음
				g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_goto_answer_no );
				break;
			}

			_snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3545), tempresult->cFromName ); //%s님께서 이동을 요청 하였습니다. 수락하시겠습니까?
			
			//메세지 박스 
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO,  _T(""), tempString, 
									TRUE, _XDEF_RECALLITEM_MOVE_AGREE, _XDEF_RECALLITEM_MOVE_REJECT );
			pMessageBox->StartTimer( 15000, TRUE );
		}
		break;
	case en_rf_recv_goto_answer_yes : //이동하여 가자 //이동하여 가야하니 이펙트를 한다..
		{
			//이펙트 
			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);

			//캐릭터, 아이디, 행렬
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call2, &g_pLocalUser->m_ModelDescriptor.m_Position );

			//사운드
			_XPlayInterfaceSound(ID_SR_INTERFACE_RECALL_WAV);

			//이펙트가 끝난 후 이동하여 간다.
			MSG_RF_RECV_GOTO_ANSWER_YES* tempresult = (MSG_RF_RECV_GOTO_ANSWER_YES*)pRecallresult;

			/*g_pLocalUser->m_Position.x = tempresult->fTargetX;
			g_pLocalUser->m_Position.z = tempresult->fTargetZ;
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(g_pLocalUser->m_Position, g_pLocalUser->m_pGoundCollideOBB);*/

			_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3546), tempresult->cTargetName ); //"%s님에게 이동되었습니다."
			g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

			g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Set_m_ucCount(tempresult->ucLeftCount); //지정된 슬롯에.. 남은개수를 저장

			memset(tempString, 0, sizeof(tempString));
			_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3542) );//소환령 1개가 소모 되었습니다
			g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

			//0개
			if( g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Get_m_ucCount() < 1 )
				g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Reset();

			//소환버튼 활성화
			pRecallItemWindow->SetRecallButton( TRUE );
		}
		break;
	case en_rf_recv_goto_answer_no : //이동 거절 당했다..
		{
			MSG_RF_RECV_GOTO_ANSWER_NO* tempresult = (MSG_RF_RECV_GOTO_ANSWER_NO*)pRecallresult;

			switch( tempresult->ucNoReason )
			{
			case 0 : //거절을 선택했을 때.
				{
					_snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3547), tempresult->cTargetName ); //%s님께서 이동요청을 거부 하였습니다.
					//메세지 박스 
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), tempString, TRUE );
				}
				break;
			case 1 : //( 요청받았을 때, 상태검사룰 수행하여, 못 받는 상태이면 이 값으로 보낸다. )
				{
					_snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3544), _XGETINTERFACETEXT(ID_STRING_NEW_3490) );//상대방의 상태 이상으로 %s할 수 없습니다. , 이동
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), tempString, TRUE );
				}
				break;
			}

			//소환버튼 활성화
			pRecallItemWindow->SetRecallButton( TRUE );
		}
		break;
	case en_rf_show_me : //소환되었음을 알림 //자기 자신은 이펙트 발생
		{
			MSG_RF_SHOW_ME* tempresult = (MSG_RF_SHOW_ME*)pRecallresult;

			if(tempresult->uiUID == g_pLocalUser->m_Uniqueid) //같으면 자기 자신
			{
				//이펙트 
				D3DXMATRIX matWorld;
				D3DXMatrixIdentity(&matWorld);

				//캐릭터, 아이디, 행렬
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_beam, &g_pLocalUser->m_ModelDescriptor.m_Position);

				//사운드
				_XPlayInterfaceSound(ID_SR_INTERFACE_RECALLEFFECT_WAV);
			}

			//TCHAR message[64];
			//memset( message, 0, sizeof(message));
			//sprintf( message, "recall success %s, x postion : %.2f  z postion : %.2f\n", tempresult->cName, tempresult->fPosX, tempresult->fPosZ );
			//OutputDebugString( message );
		}
		break;
	case en_rf_show_animation ://이동 직전/이동 직후에 주변에 알려, 애니메이션 하도록 한다.
		{
			MSG_RF_SHOW_ANIMATION* tempresult = (MSG_RF_SHOW_ANIMATION*)pRecallresult;

			// 0 - 소환자(기다리는캐릭) 마법진. 1 - 이동자의 애니메이션, 2 - 이동후의 애니메이션.
			switch( tempresult->ucRFAni )
			{
			case 0 :
				{
					if(tempresult->usUID != g_pLocalUser->m_Uniqueid)
					{
						_XUser* pUser = (_XUser*)g_Userlist.FindUser(tempresult->usUID);
						if( pUser )
						{
							D3DXMATRIX matWorld;
							D3DXMatrixIdentity(&matWorld);
							matWorld._43 = -2.0f;
							
							D3DXMATRIX matrotation;
							D3DXMatrixRotationY( &matrotation, pUser->m_RotateAngle );

							D3DXMatrixMultiply( &matWorld, &matWorld, &matrotation );

							matWorld._41 += pUser->m_Position.x;
							matWorld._42 += pUser->m_Position.y;
							matWorld._43 += pUser->m_Position.z;

							//pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call, &pUser->m_ModelDescriptor.m_Position);
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call, &matWorld );
						}
					}
				}
				break;
			case 1 :
				{
					if(tempresult->usUID != g_pLocalUser->m_Uniqueid)
					{
						_XUser* pUser = (_XUser*)g_Userlist.FindUser(tempresult->usUID);
						if( pUser )
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call2, &pUser->m_ModelDescriptor.m_Position);
					}
				}
				break;
			case 2 :
				{
					if(tempresult->usUID != g_pLocalUser->m_Uniqueid)
					{
						_XUser* pUser = (_XUser*)g_Userlist.FindUser(tempresult->usUID);
						if( pUser )
							pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_beam, &pUser->m_ModelDescriptor.m_Position);
					}
				}
				break;
			}
		}
		break;
	}
#endif
}

void _XNetwork::MSGNiItemUse(MSG_NI_ITEM_USE* itemuse)
{
	int visualeffect = 0;
	switch(itemuse->cItemType)
	{
		// 현재는 포션만 
		case _XGI_FC_POTION :
		{
			visualeffect = g_PotionItemProperty[itemuse->sItemID].sVisualEfId;
		}
		break;
	}

	if(itemuse->usUniqueID == g_pLocalUser->m_Uniqueid)
	{
		/*
		D3DXMATRIX offsetmat;
		D3DXMatrixIdentity(&offsetmat);		
		offsetmat._41 = g_pLocalUser->m_ModelDescriptor.m_Position._41;
		offsetmat._42 = g_pLocalUser->m_ModelDescriptor.m_Position._42;
		offsetmat._43 = g_pLocalUser->m_ModelDescriptor.m_Position._43;
		*/

		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, visualeffect, 
										//&offsetmat);				
										&g_pLocalUser->m_ModelDescriptor.m_Position);				
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(itemuse->usUniqueID);
		if(pUser)
		{
			if(pUser->m_ModelDescriptor.m_EffectManager)
			{
				/*
				D3DXMATRIX offsetmat;
				D3DXMatrixIdentity(&offsetmat);
				offsetmat._41 = pUser->m_ModelDescriptor.m_Position._41;
				offsetmat._42 = pUser->m_ModelDescriptor.m_Position._42;
				offsetmat._43 = pUser->m_ModelDescriptor.m_Position._43;
				*/
				pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, visualeffect, 
										//&offsetmat);
										&pUser->m_ModelDescriptor.m_Position);
			}
		}
	}
}