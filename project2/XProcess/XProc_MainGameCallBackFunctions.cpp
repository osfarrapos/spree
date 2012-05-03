#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XProc_MainGame.h"
#include "XMeshContainer.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_MessengerTooltip.h"// 2004.06.03->oneway48 insert
#include "XWindow_Option.h"
#include "XWindow_NPCScript.h"
#include "XWindow_PCTrade.h"
#include "XWindow_Party.h"
#include "XWindow_Skill.h"
#include "XWindow_Escape.h"
#include "XWindow_MatchPR.h"
#include "XWindow_GM_StatusControl.h"
#include "SoundEffectList.h"
#include "XNPCScript.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface call back function


void __stdcall _XCallback_QS_Button1( _XIMEContainer* pIMEContainer )
{
	/*_XWindow_QuickSlot* pquickslotwindow = 
		(_XWindow_QuickSlot*)((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUICKSLOT );
	if( pquickslotwindow )
	{
		POINT pt = pquickslotwindow->m_ShortCutIcon[0]->GetWindowPos();

		g_QuickSlotTooltip.MoveWindow( pt.x+32, pt.y+32 );
		g_QuickSlotTooltip.Draw( "권각술", _XDEF_DEFAULTTEXTCOLOR );
	}*/
}

void __stdcall _XCallback_QS_Button2( _XIMEContainer* pIMEContainer )
{
/*	_XWindow_QuickSlot* pquickslotwindow = 
		(_XWindow_QuickSlot*)((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUICKSLOT );
	if( pquickslotwindow )
	{
		POINT pt = pquickslotwindow->m_ShortCutIcon[1]->GetWindowPos();

		g_QuickSlotTooltip.MoveWindow( pt.x+32, pt.y+32 );
		g_QuickSlotTooltip.Draw( "목검", _XDEF_DEFAULTTEXTCOLOR );
	}	*/
}

void __stdcall _XCallback_QS_Button3( _XIMEContainer* pIMEContainer )
{
/*	_XWindow_QuickSlot* pquickslotwindow = 
		(_XWindow_QuickSlot*)((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUICKSLOT );
	if( pquickslotwindow )
	{
		POINT pt = pquickslotwindow->m_ShortCutIcon[2]->GetWindowPos();

		g_QuickSlotTooltip.MoveWindow( pt.x+32, pt.y+32 );
		g_QuickSlotTooltip.Draw( "장검", _XDEF_DEFAULTTEXTCOLOR );
	}	*/
}

void __stdcall _XCallback_QS_Button4( _XIMEContainer* pIMEContainer )
{
/*	_XWindow_QuickSlot* pquickslotwindow = 
		(_XWindow_QuickSlot*)((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUICKSLOT );
	if( pquickslotwindow )
	{
		POINT pt = pquickslotwindow->m_ShortCutIcon[3]->GetWindowPos();

		g_QuickSlotTooltip.MoveWindow( pt.x+32, pt.y+32 );
		g_QuickSlotTooltip.Draw( "장봉", _XDEF_DEFAULTTEXTCOLOR );
	}		*/
}

void __stdcall _XCallback_QS_Button5( _XIMEContainer* pIMEContainer )
{
/*	_XWindow_QuickSlot* pquickslotwindow = 
		(_XWindow_QuickSlot*)((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUICKSLOT );
	if( pquickslotwindow )
	{
		POINT pt = pquickslotwindow->m_ShortCutIcon[4]->GetWindowPos();

		g_QuickSlotTooltip.MoveWindow( pt.x+32, pt.y+32 );

		TCHAR str[30];
		sprintf( str, "환약" );

		g_QuickSlotTooltip.Draw( str, _XDEF_DEFAULTTEXTCOLOR );
	}*/
}

void __stdcall _XCallback_QS_Button6( _XIMEContainer* pIMEContainer )
{

}

void __stdcall _XCallback_QS_Button7( _XIMEContainer* pIMEContainer )
{

}

void __stdcall _XCallback_QS_Button8( _XIMEContainer* pIMEContainer )
{

}

void __stdcall _XCallback_QS_Button9( _XIMEContainer* pIMEContainer )
{

}

// 2004.11.17->hotblood insert --------------------------------------------=
void __stdcall _XIMECallback_ChatUpArrow( _XIMEContainer* pIMEContainer )
{

}

void __stdcall _XIMECallback_ChatDownArrow( _XIMEContainer* pIMEContainer )
{

}

void __stdcall _XIMECallback_ChatLeftArrow( _XIMEContainer* pIMEContainer )
{
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	
	if( pChatDefault_Window->m_ChatMode == _XCHATMODE_WHISPER )
	{
		int tmp = pChatDefault_Window->m_SelectedWhisperTarget-1;
		if(  tmp < 0 )
			tmp = 2;
		
		pChatDefault_Window->m_WhisperTargetButton[0].SetCheck( FALSE );
		pChatDefault_Window->m_WhisperTargetButton[1].SetCheck( FALSE );
		pChatDefault_Window->m_WhisperTargetButton[2].SetCheck( FALSE );
		pChatDefault_Window->m_WhisperTargetButton[tmp].SetCheck( TRUE );
				
		if( pChatDefault_Window->m_WhisperTargetButton[tmp].GetButtonText() == NULL )
		{
			POINT tmpPos = pChatDefault_Window->GetWindowPos();
			SIZE tmpSize = pChatDefault_Window->GetWindowSize();
			memset( pChatDefault_Window->m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
			pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();				
			pChatDefault_Window->m_WhisperEditContainer.MoveWindow( tmpPos.x+95+(tmp*93), tmpPos.y + tmpSize.cy - 34 );
			pChatDefault_Window->m_WhisperEditContainer.ShowWindow( TRUE );
			pChatDefault_Window->m_WhisperEditContainer.SetFocus();
		}
		else
		{
			CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
			_XIMEKERNEL.ResetCombinationBuffer();
			pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();
			pChatDefault_Window->m_WhisperEditContainer.ShowWindow( FALSE );			
			pChatDefault_Window->SetChangeWhisperName(tmp);
		}
	}	
}

void __stdcall _XIMECallback_ChatRightArrow( _XIMEContainer* pIMEContainer )
{
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	
	if( pChatDefault_Window->m_ChatMode == _XCHATMODE_WHISPER )
	{
		int tmp = pChatDefault_Window->m_SelectedWhisperTarget+1;
		if(  tmp >= _XDEF_MAXWHISPERTARGETCOUNT )	
			tmp = 0;
		
		pChatDefault_Window->m_WhisperTargetButton[0].SetCheck( FALSE );
		pChatDefault_Window->m_WhisperTargetButton[1].SetCheck( FALSE );
		pChatDefault_Window->m_WhisperTargetButton[2].SetCheck( FALSE );
		pChatDefault_Window->m_WhisperTargetButton[tmp].SetCheck( TRUE );
				
		if( pChatDefault_Window->m_WhisperTargetButton[tmp].GetButtonText() == NULL )
		{
			POINT tmpPos = pChatDefault_Window->GetWindowPos();
			SIZE tmpSize = pChatDefault_Window->GetWindowSize();
			memset( pChatDefault_Window->m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
			pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();				
			pChatDefault_Window->m_WhisperEditContainer.MoveWindow( tmpPos.x+95+(tmp*93), tmpPos.y + tmpSize.cy - 34 );
			pChatDefault_Window->m_WhisperEditContainer.ShowWindow( TRUE );
			pChatDefault_Window->m_WhisperEditContainer.SetFocus();
		}
		else
		{
			CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
			_XIMEKERNEL.ResetCombinationBuffer();
			pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();
			pChatDefault_Window->m_WhisperEditContainer.ShowWindow( FALSE );
			pChatDefault_Window->SetChangeWhisperName(tmp);
		}
	}	
}
//---------------------------------------------------------------------------=

BOOL __stdcall _XIMECallback_DataChanged( _XIMEContainer* pIMEContainer, DWORD lastkey )
{
	if(!pIMEContainer) return FALSE;

	LPTSTR pText = pIMEContainer->GetText();
	if(pText)
	{
		if( lastkey != VK_BACK )
		{
			if( strlen( pText ) == 1 && pText[0] == _T('&') )
			{
				if( strlen(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName) )
				{
					TCHAR herobandnamestring[128];
					_snprintf( herobandnamestring, sizeof(herobandnamestring), _T("(%s)"), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );

					pIMEContainer->AddString( herobandnamestring, strlen(herobandnamestring), FALSE );
				}			
			}
		}
	}

	return TRUE;
}

void __stdcall _XIMECallback_ChatReturn( _XIMEContainer* pIMEContainer )
{	
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( pMainGameProcess->m_bNPCDialogMode || pMainGameProcess->m_bNPCDialogWideView || pMainGameProcess->m_bFunctionalObjectWideView )
		return;

	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING && g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
		return;

	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	if( !pChatDefault_Window->GetShowStatus() ) return;

	BOOL flag = FALSE;

	// 채팅모드 메뉴가 떠 있으면 ---------------------------------------------------------------------------=	
	if( pChatDefault_Window->m_btnChatFunctionMenuButton[0]->GetShowStatus() )	 
	{
		// 이모션 리스트박스가 떠 있으면 -------------------------------------------------------=		
		if( pChatDefault_Window->m_EmotionListBox->GetShowStatus() )
		{
			PostMessage(gHWnd, WM_COMMAND, _XDEF_CHATDEFAULT_EMOTIONLISTBOX, (LPARAM)gHWnd);
			return;
		}
		// -------------------------------------------------------------------------------------=

		int tempmode = pChatDefault_Window->GetChatMode();		
		pChatDefault_Window->ShowChatFunctionMenu( FALSE );		
		PostMessage(gHWnd, WM_COMMAND, _XDEF_CHATDEFAULT_FUNCTION_EMOTIONBUTTON+tempmode, (LPARAM)gHWnd);
		pChatDefault_Window->m_PrevChatMode = _XCHATMODE(tempmode);
		flag = TRUE;
	}
	// -----------------------------------------------------------------------------------------------------=

	int emoindex = -1;
	TCHAR tbuf[128];
	memset( tbuf, 0, sizeof(tbuf) );
	strncpy(tbuf, pMainGameProcess->m_ChatEditContainer.GetText(), sizeof(tbuf));
	
	// 입력한 문자열이 없고 채팅모드 메뉴가 떠 있지 않을 경우 ----------------------------=	
	if( strlen( tbuf ) <= 1 && !flag ) 
	{
#ifdef _XDEF_CASHITEM_SHOUT
#ifdef _XDEF_ZONECHATTING_20070108
		if( tbuf[0] == NULL || tbuf[0] == '/' || tbuf[0] == '!' || tbuf[0] == '@' || tbuf[0] == '#' || tbuf[0] == '%' || tbuf[0] == '*')
#else
		if( tbuf[0] == NULL || tbuf[0] == '/' || tbuf[0] == '!' || tbuf[0] == '@' || tbuf[0] == '#' || tbuf[0] == '%')
#endif
#else
		if( tbuf[0] == NULL || tbuf[0] == '/' || tbuf[0] == '!' || tbuf[0] == '@' || tbuf[0] == '#' )
#endif
		{
			_XIMEKERNEL.ResetCombinationBuffer();
			pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			
			pChatDefault_Window->m_MovingChatWindow = TRUE;
			pChatDefault_Window->SetDrawBorder(FALSE);		
			pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
			_XIMEKERNEL.SetFocus( NULL );
			return;
		}		
	}	

	memset( pMainGameProcess->m_LastSendedChatMessage, 0, sizeof(pMainGameProcess->m_LastSendedChatMessage) );
	strncpy( pMainGameProcess->m_LastSendedChatMessage, tbuf, _XDEF_CHATMAXLENGTH-1 );
	
	// 귓속말 -----------------------------------------------------------------------------------------=
	if( tbuf[0] == '/' )	
	{
		if( strlen( tbuf ) < 2 ) return;

		if(  (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_2416))==0) || (_stricmp(tbuf, "//booth")==0) )
		{
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_USEBUTTON, 0), (LPARAM)gHWnd); 
			return;
		}
		if( _stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_2417))==0 )
		{
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LIFESKILL_PVPPRBUTTON, 0), (LPARAM)gHWnd); 			
			return;
		}
		
		char *action = strtok(tbuf, "/ ");		
		if( !action ) return;

		char *message = NULL;
#ifdef _XADMINISTRATORMODE			
		if( _stricmp(action, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_2418))==0 )
		{
			message = strtok(NULL, " ");
			g_LodTerrain.FindSafePoint( g_pLocalUser->m_Position );
		}
#endif	

#ifdef _XGMCLIENT
		if ( (_stricmp(action, "showcursor")==0) )
		{
			/*
			_XWindow_GM_StatusControl* pWindow = (_XWindow_GM_StatusControl*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERSETTINGWINDOW);
			if( pWindow )
			{
				_XCheckButton* pButton = pWindow->GetDlgCtrlID(_XDEF_GMSTATUSCONTROLCURSORVIEW);
				if(pButton)
				{
					pButton->SetCheck( TRUE );
				}
			}
			*/

			g_FreeMoveMode_CursorView = TRUE;
			::ShowCursor( TRUE );
			((CEmperorOfDragonsApp*)gApp)->SetCursor( _XMOUSETYPE_PEACE );
			return;
		}
		else if ( (_stricmp(action, "viewgraph")==0) )
		{
			extern BOOL g_DrawGraphLog;
			g_DrawGraphLog = !g_DrawGraphLog;
		}
#endif

// 운영자 모드 ------------------------------------------------------------------------------------------------------------------=
#ifdef _XADMINISTRATORMODE			
		if ( (_stricmp(action, "_global")==0) || (_stricmp(action, "_g")==0) )
		{
			message = strtok(NULL, " ");
    
			if (message == NULL) return;        		
			g_NetworkKernel.SendChatMessage(message, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_GLOBALNOTICE );        
			
			_XIMEKERNEL.ResetCombinationBuffer();			
			pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			pMainGameProcess->m_ChatEditContainer.AddString( _T("/_global "), 9 );
			return;
		}
		else if ( (_stricmp(action, "_adminnotice")==0) || (_stricmp(action, "_admin")==0) ) 
		{
			message = strtok(NULL, " ");
    
			if (message == NULL) return;
        			
			g_NetworkKernel.SendChatMessage(message, _XDEF_CHATMESSAGEMODE_ADMINNOTICE );

			_XIMEKERNEL.ResetCombinationBuffer();
			pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			pMainGameProcess->m_ChatEditContainer.AddString( _T("/_admin "), 8 );
			return;
		}	
		else
		{
#endif   	

		_XIMEKERNEL.ResetCombinationBuffer();
		pMainGameProcess->m_ChatEditContainer.ClearBuffer();

#ifdef _XADMINISTRATORMODE
		}
#endif
// ------------------------------------------------------------------------------------------------------------------------------=
		int idlength = strlen(action);
		if( idlength <= _XDEF_MAX_USERNAMESTRINGLENGTH )
		{
			if( (_stricmp(action, g_pLocalUser->m_CharacterInfo.charactername )==0) )
			{
				_XIMEKERNEL.ResetCombinationBuffer();
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
				return;
			}
			strcpy( pChatDefault_Window->m_WhisperTargetName, action );
			pChatDefault_Window->InsertRecentWhisperUserList(action, 3);
			
			message = strtok(NULL, "");
			if (message == NULL) 
			{
				pChatDefault_Window->SetChangeWhisperName(0);
				return;
			}

#ifdef _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 일정 시간안에 같은 메시지를 보내지 못하도록 
			if((pMainGameProcess->m_LastSendChatMessageTime > 0) && (g_LocalSystemTime - pMainGameProcess->m_LastSendChatMessageTime < 5000))
			{
				if(_stricmp(message, pMainGameProcess->m_LastMsg) == 0)
				{
					_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
					if( chattingmessage )
					{				
						g_Chatlist.resetList();
						g_Chatlist.insertItem( chattingmessage );
					}
					
					if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
					{			
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
						pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
	#else
						pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
	#endif
						pChatDefault_Window->m_ChatListScrollBar->UpdateData();
						pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
					}

					return;
				}
			}
#endif

			strncpy( pMainGameProcess->m_LastMsg, message, sizeof(pMainGameProcess->m_LastMsg) );
			pMainGameProcess->m_OverlappingMsgCount = 1;
			g_NetworkKernel.SendChatMessage( message, _XDEF_CHATMESSAGECOLOR_WHISPER );			
			
			pChatDefault_Window->SetChatMode( _XCHATMODE_WHISPER );
			
			if(g_ChattingAutoClose)
			{
				// 채팅창 자동 종료 기능 - 채팅 모드 메뉴가 없을 때 무조건 종료
				if( !flag ) 
				{
					_XIMEKERNEL.ResetCombinationBuffer();
					pMainGameProcess->m_ChatEditContainer.ClearBuffer();
					
					pChatDefault_Window->m_MovingChatWindow = TRUE;
					pChatDefault_Window->SetDrawBorder(FALSE);		
					pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
					_XIMEKERNEL.SetFocus( NULL );
					return;
				}
			}
		}	
		// ------------------------------------------------------------------------------------------------------------------=
	}
	else if( tbuf[0] == '!' )		// 동행
	{
		if( strlen( tbuf ) < 2 ) return;

		char *message = &tbuf[1];//strtok(tbuf, "!");

		if (message == NULL) return;

#ifdef _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 일정 시간안에 같은 메시지를 보내지 못하도록 
		if((pMainGameProcess->m_LastSendChatMessageTime > 0) && (g_LocalSystemTime - pMainGameProcess->m_LastSendChatMessageTime < 5000))
		{
			if(_stricmp(message, pMainGameProcess->m_LastMsg) == 0)
			{
				_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				if( chattingmessage )
				{				
					g_Chatlist.resetList();
					g_Chatlist.insertItem( chattingmessage );
				}
				
				if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
				{			
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
	#else
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
	#endif
					pChatDefault_Window->m_ChatListScrollBar->UpdateData();
					pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}

				return;
			}
		}
#endif
		
		if( g_pLocalUser->m_InParty )
		{
			strncpy(pMainGameProcess->m_LastMsg, message, sizeof(pMainGameProcess->m_LastMsg));
			g_NetworkKernel.SendChatMessage( message, _XDEF_CHATMESSAGECOLOR_PARTY );

			if(g_ChattingAutoClose)
			{
				// 채팅창 자동 종료 기능 - 채팅 모드 메뉴가 없을 때 무조건 종료
				if( !flag ) 
				{
					_XIMEKERNEL.ResetCombinationBuffer();
					pMainGameProcess->m_ChatEditContainer.ClearBuffer();
					
					pChatDefault_Window->m_MovingChatWindow = TRUE;
					pChatDefault_Window->SetDrawBorder(FALSE);		
					pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
					_XIMEKERNEL.SetFocus( NULL );
					return;
				}
			}
		}
		else
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1062), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else				
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChatDefault_Window->m_ChatListScrollBar->UpdateData();
				pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
			}
		}
		
		_XIMEKERNEL.ResetCombinationBuffer();
		pMainGameProcess->m_ChatEditContainer.ClearBuffer();
		pChatDefault_Window->SetChatMode( _XCHATMODE_PARTY );
	}
	else if( tbuf[0] == '@' )		// 분파 방회
	{
		if( strlen( tbuf ) < 2 ) return;

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		pChatDefault_Window->m_HeroBandChatMode = FALSE;
#endif

		char *message = &tbuf[1];//strtok(tbuf, "@");

		if (message == NULL) return;

#ifdef _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 일정 시간안에 같은 메시지를 보내지 못하도록 
		if((pMainGameProcess->m_LastSendChatMessageTime > 0) && (g_LocalSystemTime - pMainGameProcess->m_LastSendChatMessageTime < 5000))
		{
			if(_stricmp(message, pMainGameProcess->m_LastMsg) == 0)
			{
				_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				if( chattingmessage )
				{				
					g_Chatlist.resetList();
					g_Chatlist.insertItem( chattingmessage );
				}
				
				if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
				{			
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
	#else
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
	#endif
					pChatDefault_Window->m_ChatListScrollBar->UpdateData();
					pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}

				return;
			}
		}
#endif

		if( strlen(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName) )
		{
			strncpy(pMainGameProcess->m_LastMsg, message, sizeof(pMainGameProcess->m_LastMsg));

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
			g_NetworkKernel.SendChatMessage( message, _XDEF_CHATMESSAGECOLOR_ALLY );
#else
			g_NetworkKernel.SendChatMessage( message, _XDEF_CHATMESSAGECOLOR_USERCLAN );
#endif

			if(g_ChattingAutoClose)
			{
				// 채팅창 자동 종료 기능 - 채팅 모드 메뉴가 없을 때 무조건 종료
				if( !flag ) 
				{
					_XIMEKERNEL.ResetCombinationBuffer();
					pMainGameProcess->m_ChatEditContainer.ClearBuffer();
					
					pChatDefault_Window->m_MovingChatWindow = TRUE;
					pChatDefault_Window->SetDrawBorder(FALSE);		
					pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
					_XIMEKERNEL.SetFocus( NULL );
					return;
				}
			}
		}
		else
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1840), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChatDefault_Window->m_ChatListScrollBar->UpdateData();
				pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
			}
		}
		
		_XIMEKERNEL.ResetCombinationBuffer();
		pMainGameProcess->m_ChatEditContainer.ClearBuffer();
		pChatDefault_Window->SetChatMode( _XCHATMODE_SUBCLAN );
	}
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	else if( tbuf[0] == '&' )		// 소속된 영웅단만.
	{
		if( strlen( tbuf ) < 2 ) return;

		TCHAR teststring[256];
		_snprintf( teststring, sizeof(teststring), _T("&(%s)"), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );
		if( strcmp(tbuf, teststring) == 0 )
		{
			return;
		}

		pChatDefault_Window->m_HeroBandChatMode = TRUE;

		char *message = &tbuf[1];//strtok(tbuf, "&");

		if (message == NULL) return;

#ifdef _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 일정 시간안에 같은 메시지를 보내지 못하도록 
		if((pMainGameProcess->m_LastSendChatMessageTime > 0) && (g_LocalSystemTime - pMainGameProcess->m_LastSendChatMessageTime < 5000))
		{
			if(_stricmp(message, pMainGameProcess->m_LastMsg) == 0)
			{
				_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				if( chattingmessage )
				{				
					g_Chatlist.resetList();
					g_Chatlist.insertItem( chattingmessage );
				}
				
				if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
				{			
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
	#else
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
	#endif
					pChatDefault_Window->m_ChatListScrollBar->UpdateData();
					pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}

				return;
			}
		}
#endif

		if( strlen(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName) )
		{
			strncpy(pMainGameProcess->m_LastMsg, message, sizeof(pMainGameProcess->m_LastMsg));
			g_NetworkKernel.SendChatMessage( message, _XDEF_CHATMESSAGECOLOR_USERCLAN );

			if(g_ChattingAutoClose)
			{
				// 채팅창 자동 종료 기능 - 채팅 모드 메뉴가 없을 때 무조건 종료
				if( !flag ) 
				{
					_XIMEKERNEL.ResetCombinationBuffer();
					pMainGameProcess->m_ChatEditContainer.ClearBuffer();
					
					pChatDefault_Window->m_MovingChatWindow = TRUE;
					pChatDefault_Window->SetDrawBorder(FALSE);		
					pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
					_XIMEKERNEL.SetFocus( NULL );
					return;
				}
			}
		}
		else
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1840), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChatDefault_Window->m_ChatListScrollBar->UpdateData();
				pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
			}
		}
		
		_XIMEKERNEL.ResetCombinationBuffer();
		pMainGameProcess->m_ChatEditContainer.ClearBuffer();
		pChatDefault_Window->SetChatMode( _XCHATMODE_SUBCLAN );
	}
#endif // #ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

#ifdef _XDEF_ZONECHATTING_20070108
	else if(tbuf[0] == '*')		// 지역대화
	{
		//옵션에서 지역대화 끄기 이면 지역 대화도 할 수 없다.
		if( g_bCommunityOptionDisableZoneChatting ) return;

		if( pChatDefault_Window->m_ChatMode != _XCHATMODE_ZONE)
		{
			if( !g_bProgressZoneChatting )
			{				
				g_bProgressZoneChatting = TRUE;
				
				DWORD playelapsedtime = g_LocalSystemTime - g_DelayTimeZoneChatting;
				
				if(playelapsedtime >= 10000)		// 10초후 
				{
					g_bEnableZoneChatting = TRUE;
					//다시 지역대화 버튼으로 변경
					_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
					pChatDefault_Window->SetChatMode( _XCHATMODE_ZONE );
					pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
				}
			}
		}
			

		if( strlen( tbuf ) < 2 ) return;
		
		//영문기준 5자 이상만 채팅 가능
		if( strlen( tbuf ) < 6 ) 
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_NEW_3211), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChatDefault_Window->m_ChatListScrollBar->UpdateData();
				pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
			}
			return;
		}
		
		// 내공이 부족한 경우를 검사한다.
		bool check_no_energy = false;	//내공이 충분한지 검사
		if( g_CurrentZoneInfoIndex > 0 && g_CurrentZoneInfoIndex < 7)//문파 본당 맵
		{
			bool checkclanmap = false;	//문파 맵인지 일반 맵인지 검사
			switch(g_pLocalUser->m_CharacterInfo.groupindex)
			{
			case _XGROUP_GAEBANG :
				{
					if( g_CurrentZoneInfoIndex == 2 )
						checkclanmap = true;
				}
				break;
			case _XGROUP_SORIM :
				{
					if( g_CurrentZoneInfoIndex == 1 )
						checkclanmap = true;
				}
				break;
			case _XGROUP_MOODANG :
				{
					if( g_CurrentZoneInfoIndex == 5 )
						checkclanmap = true;
				}
				break;
			case _XGROUP_BEEGOONG :
				{
					if( g_CurrentZoneInfoIndex == 4 )
						checkclanmap = true;
				}
				break;
			case _XGROUP_NOCKRIM :
				{
					if( g_CurrentZoneInfoIndex == 3 )
						checkclanmap = true;
				}
				break;
			case _XGROUP_MAKYO :
				{
					if( g_CurrentZoneInfoIndex == 6 )
						checkclanmap = true;
				}
				break;
			}
			
#ifdef _XTAIWANESE
			if( checkclanmap )
			{
				//문파 본당 맵에는 내공소모가 1%이다.
				if( g_pLocalUser->ForcePowerPercentage < 1 )
					check_no_energy = true;
			}
			else
			{
				//일반 맵에는 내공소모가 3%이다.
				if( g_pLocalUser->ForcePowerPercentage < 3 )
					check_no_energy = true;
			}
#else
			if( checkclanmap )
			{
				//문파 본당 맵에는 내공소모가 1%이다.
				if( g_pLocalUser->ForcePowerPercentage < 5 )
					check_no_energy = true;
			}
			else
			{
				//일반 맵에는 내공소모가 3%이다.
				if( g_pLocalUser->ForcePowerPercentage < 15 )
					check_no_energy = true;
			}
#endif

		}
		else
		{
#ifdef _XTAIWANESE
			//일반 맵에는 내공소모가 3%이다.
			if( g_pLocalUser->ForcePowerPercentage < 3 )
				check_no_energy = true;
#else
			if( g_pLocalUser->ForcePowerPercentage < 15 )
				check_no_energy = true;
#endif
		}
		
		// 내공이 부족한 경우를 지역 대화를 할 수 없다.
		if( check_no_energy )
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_NEW_3213), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChatDefault_Window->m_ChatListScrollBar->UpdateData();
				pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
			}
			return;
		}

		// 채팅 후에 딜레이 타임이 지나지 않았는데 채팅을 다시 하는 경우
		if( !g_bEnableZoneChatting )
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_NEW_3214), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChatDefault_Window->m_ChatListScrollBar->UpdateData();
				pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
			}
			return;
		}

		// 지역 대화 옵션 끄기

		char *message = &tbuf[1];//strtok(tbuf, "*");
		
		if (message == NULL) return;

#ifdef _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 일정 시간안에 같은 메시지를 보내지 못하도록 
		if((pMainGameProcess->m_LastSendChatMessageTime > 0) && (g_LocalSystemTime - pMainGameProcess->m_LastSendChatMessageTime < 5000))
		{
			if(_stricmp(message, pMainGameProcess->m_LastMsg) == 0)
			{
				_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				if( chattingmessage )
				{				
					g_Chatlist.resetList();
					g_Chatlist.insertItem( chattingmessage );
				}
				
				if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
				{			
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
	#else
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
	#endif
					pChatDefault_Window->m_ChatListScrollBar->UpdateData();
					pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}

				return;
			}
		}
#endif
		
		strncpy(pMainGameProcess->m_LastMsg, message, sizeof(pMainGameProcess->m_LastMsg));
		g_NetworkKernel.SendChatMessage( message, _XDEF_CHATMESSAGECOLOR_ZONECHAT );
		g_bEnableZoneChatting = FALSE;//10초후에 다시 가능한 상태가 된다.
		pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(FALSE);
		g_DelayTimeZoneChatting = g_LocalSystemTime;//딜레이 시간 측정을 시작한다.
		
		if(g_ChattingAutoClose)
		{
			// 채팅창 자동 종료 기능 - 채팅 모드 메뉴가 없을 때 무조건 종료
			if( !flag ) 
			{
				_XIMEKERNEL.ResetCombinationBuffer();
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
				
				pChatDefault_Window->m_MovingChatWindow = TRUE;
				pChatDefault_Window->SetDrawBorder(FALSE);		
				pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
				_XIMEKERNEL.SetFocus( NULL );
				return;
			}
		}
		
		// 지역대화를 하고 난 후 딜레이 시간동안에는 전체 대화로 자동으로 변경된다.
		_XIMEKERNEL.ResetCombinationBuffer();
		pMainGameProcess->m_ChatEditContainer.ClearBuffer();
		
		pChatDefault_Window->m_ChatMode = _XCHATMODE_NORMAL;
		pChatDefault_Window->m_EmotionListBox->ShowWindow( FALSE );
		
		pChatDefault_Window->m_WhisperStaticButton->ShowWindow( FALSE );
		for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
		{
			pChatDefault_Window->m_WhisperTargetButton[i].ShowWindow( FALSE );
		}
		
		for( i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
		{
			if( pChatDefault_Window->m_ChatMode != i )
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					pChatDefault_Window->m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 177, 116, 195 );
				}
				else
				{
					pChatDefault_Window->m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 124, 0, 212, 18 );
				}
			}
		}
		
		extern D3DCOLOR g_ChatFunctionButtonStringColor[_XCHATMODE_TOTALCOUNT];
		pChatDefault_Window->m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[_XCHATMODE_NORMAL], _XFONT_ALIGNTYPE_LEFT, 6 );
		pChatDefault_Window->m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[_XCHATMODE_NORMAL] );
		pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );
	}
#endif
#ifdef _XDEF_CASHITEM_SHOUT
	else if(tbuf[0] == '%')		// 외치기
	{
		if( strlen( tbuf ) < 2 ) return;
		
		char *message = &tbuf[1];//strtok(tbuf, "%");
		
		if (message == NULL) return;

#ifdef _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 일정 시간안에 같은 메시지를 보내지 못하도록 
		if((pMainGameProcess->m_LastSendChatMessageTime > 0) && (g_LocalSystemTime - pMainGameProcess->m_LastSendChatMessageTime < 5000))
		{
			if(_stricmp(message, pMainGameProcess->m_LastMsg) == 0)
			{
				_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				if( chattingmessage )
				{				
					g_Chatlist.resetList();
					g_Chatlist.insertItem( chattingmessage );
				}
				
				if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
				{			
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
	#else
					pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
	#endif
					pChatDefault_Window->m_ChatListScrollBar->UpdateData();
					pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}

				return;
			}
		}
#endif
		
		if(g_pLocalUser->m_ShoutMode != _XSHOUT_MODE_NONE)
		{
			strncpy(pMainGameProcess->m_LastMsg, message, sizeof(pMainGameProcess->m_LastMsg));
			g_NetworkKernel.SendChatMessage( message, _XDEF_CHATMESSAGECOLOR_SHOUT );
			
			if(g_ChattingAutoClose)
			{
				// 채팅창 자동 종료 기능 - 채팅 모드 메뉴가 없을 때 무조건 종료
				if( !flag ) 
				{
					_XIMEKERNEL.ResetCombinationBuffer();
					pMainGameProcess->m_ChatEditContainer.ClearBuffer();
					
					pChatDefault_Window->m_MovingChatWindow = TRUE;
					pChatDefault_Window->SetDrawBorder(FALSE);		
					pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
					_XIMEKERNEL.SetFocus( NULL );
					return;
				}
			}
		}
		else
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_NEW_2975), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else
				pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChatDefault_Window->m_ChatListScrollBar->UpdateData();
				pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
			}
		}
		
		_XIMEKERNEL.ResetCombinationBuffer();
		pMainGameProcess->m_ChatEditContainer.ClearBuffer();
		pChatDefault_Window->SetChatMode( _XCHATMODE_SHOUT );
	}
#endif
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1064))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1065))==0) )	// emotion 
	{
		emoindex = 0;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1066))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1067))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1068))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1069))==0) ||
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1070))==0) )
	{
		emoindex = 1;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1071))==0) )
	{
		emoindex = 2;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1072))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1073))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1074))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1075))==0) )
	{
		emoindex = 3;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1076))==0) )
	{
		emoindex = 4;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1077))==0) ||
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1078))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1079))==0) )
	{
		emoindex = 5;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1080))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1081))==0) )
	{
		emoindex = 6;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1082))==0) ||
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1083))==0) )
	{
		emoindex = 7;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1084))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1085))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1086))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1087))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1088))==0) )
	{
		emoindex = 8;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1089))==0) || 
		(_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1090))==0) )
	{
		 emoindex = 9;
	}
	else if( (_stricmp(tbuf,_XGETINTERFACETEXT(ID_STRING_NEW_2617))==0) )	
	{
		emoindex = 11;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_NEW_2618))==0) )
	{
		emoindex = 12;
	}
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_NEW_2619))==0) )
	{
		emoindex = 13;
	}
#ifdef _XDEF_ESCAPE_20070205
	else if( (_stricmp(tbuf, _XGETINTERFACETEXT(ID_STRING_NEW_3312)) ==0) )//;탈출
	{
		BOOL bErrorCheck = FALSE;
		TCHAR ErrorString[512];
		memset( ErrorString, 0, sizeof(TCHAR) * 512 );
		
		if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE )
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3317));// 평화 상태에서만 탈출이 가능합니다
			bErrorCheck = TRUE;
		}
		else if( g_pLocalUser->m_bIsBossBattle )
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3318));// 결 안에서는 탈출을 할 수 없습니다
			bErrorCheck = TRUE;
		}
		else if( g_pLocalUser->GetMotionClass() == _XACTION_MOVE )
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3319));// 이동 중에는 탈출할 수 없습니다
			bErrorCheck = TRUE;
		}	
		else if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
			(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// 내공심법 시전중
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3320));// 운기 중에는 탈출할 수 없습니다.
			bErrorCheck = TRUE;
		}		
		else if( g_pLocalUser->m_bReadingBook )	// 비급 읽는 중이면 리턴
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3321));//비급 읽는 중에는 탈출할 수 없습니다
			bErrorCheck = TRUE;
		}
		else if( g_pLocalUser->m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3322));// 소켓 장착 중에는 탈출할 수 없습니다
			bErrorCheck = TRUE;
		}
		else if( g_pLocalUser->m_bGambleBoxOpen )
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3323));// 패왕궤 여는 중에는 탈출할 수 없습니다
			bErrorCheck = TRUE;
		}
		else if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3324));// 영약 복용 중에는 탈출할 수 없습니다
			bErrorCheck = TRUE;
		}
		else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3325));// 노점을 연 상태에서는 탈출할 수 없습니다
			bErrorCheck = TRUE;
		}
		else if(g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_DYING)
		{
			strcpy(ErrorString,_XGETINTERFACETEXT(ID_STRING_NEW_3377) );
			bErrorCheck = TRUE;
		}
		
		_XWindow_MatchPR* pMatchPR_Window = (_XWindow_MatchPR*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MATCHPR);
		if( pMatchPR_Window)
		{
			if( pMatchPR_Window->GetShowStatus() )
			{
				strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3326));// 비무 홍보 중에는 탈출할 수 없습니다
				bErrorCheck = TRUE;
			}
		}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_escape))
		{
			strcpy(ErrorString, _XGETINTERFACETEXT(ID_STRING_NEW_3761));	//"비무 중에는 탈출 기능을 사용할 수 없습니다."
			bErrorCheck = TRUE;
		}
#endif


		if( bErrorCheck )
		{
			g_NetworkKernel.InsertChatString(ErrorString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
			_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
			pMainGameProcess->m_ChatEditContainer.ClearBuffer();
#endif
			
			pChatDefault_Window->m_MovingChatWindow = TRUE;
			pChatDefault_Window->SetDrawBorder(FALSE);		
			pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
			_XIMEKERNEL.SetFocus( NULL );
			return;
		}
		else
		{
			_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
			if(pEscape_Window)
			{
				pEscape_Window->ShowWindow(TRUE);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3242), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
				_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
#endif
				
				pChatDefault_Window->m_MovingChatWindow = TRUE;
				pChatDefault_Window->SetDrawBorder(FALSE);		
				pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
				_XIMEKERNEL.SetFocus( NULL );
				return;
			}
		}
	}
#endif
	else
	{
		if( strlen( tbuf ) > 0 )
		{
#ifdef _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 일정 시간안에 같은 메시지를 보내지 못하도록 
			if((pMainGameProcess->m_LastSendChatMessageTime > 0) && (g_LocalSystemTime - pMainGameProcess->m_LastSendChatMessageTime < 5000))
			{
				if(_stricmp(tbuf, pMainGameProcess->m_LastMsg) == 0)
				{
					_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
					if( chattingmessage )
					{				
						g_Chatlist.resetList();
						g_Chatlist.insertItem( chattingmessage );
					}
					
					if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
					{			
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
						pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
	#else
						pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
	#endif
						pChatDefault_Window->m_ChatListScrollBar->UpdateData();
						pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
					}

					return;
				}
			}
#else
			if( _stricmp( tbuf, pMainGameProcess->m_LastMsg ) == 0 )
			{
				if( pMainGameProcess->m_OverlappingMsgCount > 4 )
				{
					_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1841), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
					if( chattingmessage )
					{				
						g_Chatlist.resetList();
						g_Chatlist.insertItem( chattingmessage );
					}
					
					if(pChatDefault_Window && pChatDefault_Window->m_ChatListScrollBar)
					{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
						pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount( pChatDefault_Window->RecalcChattingListLength() );
#else
						pChatDefault_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
						pChatDefault_Window->m_ChatListScrollBar->UpdateData();
						pChatDefault_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
					}
				}
				else
				{
					if( strncmp( tbuf, _T("$%^"), 3 ) != 0 )
					{
						pMainGameProcess->m_OverlappingMsgCount++;
					}
					
					g_NetworkKernel.SendChatMessage( tbuf );
				}
			}
			else
#endif
			{
				strncpy( pMainGameProcess->m_LastMsg, tbuf, sizeof(pMainGameProcess->m_LastMsg) );
				pMainGameProcess->m_OverlappingMsgCount = 1;
				g_NetworkKernel.SendChatMessage( tbuf );
			}
		}		
		
#ifdef _XDEF_ZONECHATTING_20070108
		if( g_bProgressZoneChatting )
		{
			// 지역대화를 하고 난 후 딜레이 시간동안에는 전체 대화로 자동으로 변경된다.
			_XIMEKERNEL.ResetCombinationBuffer();
			pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			
			pChatDefault_Window->m_ChatMode = _XCHATMODE_NORMAL;
			pChatDefault_Window->m_EmotionListBox->ShowWindow( FALSE );
			
			pChatDefault_Window->m_WhisperStaticButton->ShowWindow( FALSE );
			for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
			{
				pChatDefault_Window->m_WhisperTargetButton[i].ShowWindow( FALSE );
			}
			
			for( i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
			{
				if( pChatDefault_Window->m_ChatMode != i )
				{
					if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					{
						pChatDefault_Window->m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 177, 116, 195 );
					}
					else
					{
						pChatDefault_Window->m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 124, 0, 212, 18 );
					}
				}
			}
			
			extern D3DCOLOR g_ChatFunctionButtonStringColor[_XCHATMODE_TOTALCOUNT];
			pChatDefault_Window->m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[_XCHATMODE_NORMAL], _XFONT_ALIGNTYPE_LEFT, 6 );
			pChatDefault_Window->m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[_XCHATMODE_NORMAL] );
		}
		else
			pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );

#else
		pChatDefault_Window->SetChatMode( _XCHATMODE_NORMAL );
#endif

		if(g_ChattingAutoClose)
		{
			// 채팅창 자동 종료 기능 - 채팅 모드 메뉴가 없을 때 무조건 종료
			if( !flag ) 
			{
				_XIMEKERNEL.ResetCombinationBuffer();
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
				
				pChatDefault_Window->m_MovingChatWindow = TRUE;
				pChatDefault_Window->SetDrawBorder(FALSE);		
				pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );
				_XIMEKERNEL.SetFocus( NULL );
				return;
			}
		}

		return;
	}
	if( emoindex != -1 )
	{
		if( !g_pLocalUser->m_bMatchPRMode ) // 비무 홍보가 아닐때만  
		{
			if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE )
			{
				bool checkmessagebox = false;
				// 메세지 박스가 떠 있는 상태에서는 이모콘티 표현을 할 수 없다.
				_XMessageWindow* pMessageBoxWindow = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
				if( pMessageBoxWindow && pMessageBoxWindow->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
				if( pTradeMessageBox && pTradeMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pPCTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADEMESSAGEWINDOW);
				if( pPCTradeMessageBox && pPCTradeMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pEventMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTMESSAGEWINDOW);
				if( pEventMessageBox && pEventMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pHealMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HEALMESSAGEWINDOW);
				if( pHealMessageBox && pHealMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pInchantMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
				if( pInchantMessageBox && pInchantMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pSkillMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_DELETESKILLMESSAGEWINDOW);
				if( pSkillMessageBox && pSkillMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pStateMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
				if( pStateMessageBox && pStateMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pTargetInfoMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
				if( pTargetInfoMessageBox && pTargetInfoMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				_XMessageWindow* pSocketMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
				if( pSocketMessageBox && pSocketMessageBox->GetShowStatus() ) checkmessagebox = true;
				
				if( checkmessagebox )
				{
					_XIMEKERNEL.ResetCombinationBuffer();
					pMainGameProcess->m_ChatEditContainer.ClearBuffer();
					return;
				}
				
				
				if( !( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_BOW && 
					g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_CLAPPINGHAND ) )
				{
					if(	g_pLocalUser->GetMotionClass() != _XACTION_DYING	&&
						g_pLocalUser->GetMotionClass() != _XACTION_DIE		&& 
						g_pLocalUser->GetMotionClass() != _XACTION_REBIRTH  && 
						g_pLocalUser->GetMotionClass() != _XACTION_INSENSIBLE &&	// 기절 상태
						g_pLocalUser->m_CharacterInfo.Get_current_lifepower() > 0 ) 
					{
						BOOL selfchanneling = FALSE;
						char usefultype = 0;
						char targettype = 0;
						if(g_pLocalUser->m_SelectedSkillID > 0)
						{
							usefultype = g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType;
							targettype = _XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID);//g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->targetType;
						}
						else
							usefultype = _XSI_USEFULTYPE_ACTIVESKILL;
						
						if( usefultype == _XSI_USEFULTYPE_HIDDENMA )
						{
	//						if(targettype == _XSI_TARGETTYPE_SELF && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE )
							{
								selfchanneling = TRUE;
							}				
						}
						
#ifdef _XDEF_LOSTWEAPON_070821_MAGVIPER //@author 양희왕 @breif 무기 사라지는 버그 수정
						if( g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST )
#else
						if( !selfchanneling )
#endif
						{
							g_pLocalUser->ResetAttackTarget();
							
							g_pLocalUser->SetAnimation( _XACTION_IDLE );
							g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
							
							extern _XCHARACTER_MOTION	g_EmotionActionTable[16];			
							g_pLocalUser->SetAnimationIndex( g_EmotionActionTable[emoindex] );
							g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
							
							g_NetworkKernel.SendChatMessage( tbuf );
							_XIMEKERNEL.ResetCombinationBuffer();
							pMainGameProcess->m_ChatEditContainer.ClearBuffer();
						}
					}
				}
				else
				{
					_XIMEKERNEL.ResetCombinationBuffer();
					pMainGameProcess->m_ChatEditContainer.ClearBuffer();
				}
			}	
			else
			{
				g_NetworkKernel.SendChatMessage( tbuf );
				_XIMEKERNEL.ResetCombinationBuffer();
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			}
		}
	}	
}

void __stdcall _XIMECallback_ChatEscape( _XIMEContainer* pIMEContainer )
{
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	_XIMEKERNEL.ResetCombinationBuffer();
	
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		
	pMainGameProcess->m_ChatEditContainer.ClearBuffer();
#endif

	pMainGameProcess->m_ChatEditContainer.ShowWindow( FALSE );

	pChatDefault_Window->m_MovingChatWindow = TRUE;
	pChatDefault_Window->ShowChatFunctionMenu( FALSE );
	pChatDefault_Window->SetDrawBorder(FALSE);
	if( pChatDefault_Window->m_EmotionListBox )
		pChatDefault_Window->m_EmotionListBox->ShowWindow( FALSE );
	_XIMEKERNEL.SetFocus( NULL );
}

void __stdcall _XIMECallback_ChatTab( _XIMEContainer* pIMEContainer )
{
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );				
	if( pMainGameProcess->m_ChatEditContainer.GetShowStatus() && pMainGameProcess->m_ChatEditContainer.GetFocus() )
	{
		
		if( pChatDefault_Window->m_btnChatFunctionMenuButton[0]->GetShowStatus() )
		{
			_XCHATMODE tempmode = pChatDefault_Window->GetChatMode();
			
			// 임시 제한
#ifdef _XDEF_CASHITEM_SHOUT
			if(tempmode == _XCHATMODE_EMOTION)
			{
				if(g_pLocalUser->m_ShoutMode != _XSHOUT_MODE_NONE)
					tempmode = _XCHATMODE_SHOUT;
				else
					tempmode = _XCHATMODE_SUBCLAN;
			}
			else if(tempmode == _XCHATMODE_SHOUT)
				tempmode = _XCHATMODE_SUBCLAN;
#else
			if( tempmode == _XCHATMODE_EMOTION )
				tempmode = _XCHATMODE_SUBCLAN;
#endif
			else if( tempmode == _XCHATMODE_SUBCLAN )				
				tempmode = _XCHATMODE_PARTY;
			else if( tempmode == _XCHATMODE_PARTY )				
				tempmode = _XCHATMODE_WHISPER;
#ifdef _XDEF_ZONECHATTING_20070108
			else if( tempmode == _XCHATMODE_WHISPER )
			{
				if( g_bCommunityOptionDisableZoneChatting )
					tempmode = _XCHATMODE_NORMAL;
				else
				{
					if( pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->GetEnableStatus() )
						tempmode = _XCHATMODE_ZONE;
					else
						tempmode = _XCHATMODE_NORMAL;
				}
			}
			else if( tempmode == _XCHATMODE_ZONE )
				tempmode = _XCHATMODE_NORMAL;
#else
			else if( tempmode == _XCHATMODE_WHISPER )
				tempmode = _XCHATMODE_NORMAL;
#endif
			else if( tempmode == _XCHATMODE_NORMAL )
				tempmode = _XCHATMODE_EMOTION;
			//if( tempmode+1 > 5 ) tempmode = 0;
			//else tempmode++;
			pChatDefault_Window->SetChatMode( tempmode );			
		}
		else
		{
			pChatDefault_Window->ShowChatFunctionMenu( TRUE );
			_XCHATMODE tempmode = pChatDefault_Window->GetChatMode();
			if( tempmode == _XCHATMODE_EMOTION )
				tempmode = _XCHATMODE_NORMAL;
			
#ifdef _XDEF_ZONECHATTING_20070108
			if( !g_bEnableZoneChatting )
			{
				if( tempmode == _XCHATMODE_ZONE )
					tempmode = _XCHATMODE_NORMAL;
			}
#endif
			pChatDefault_Window->SetChatMode( tempmode );
			
#ifdef _XDEF_ZONECHATTING_20070108
			if( g_bEnableZoneChatting )
			{
				pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
			}
			else
			{
				pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(FALSE);
				
			}
#endif
		}		
		
#ifdef _XDEF_ZONECHATTING_20070108
		DWORD playelapsedtime = g_LocalSystemTime - g_DelayTimeZoneChatting;
		if(playelapsedtime >= 10000)		// 10초후 
			pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
#endif
	}
	/*if( gpInput->CheckKeyPress( DIK_LCONTROL ) )
	{
	_XIMEKERNEL.ResetCombinationBuffer();
	pMainGameProcess->m_ChatEditContainer.ClearBuffer();
	pMainGameProcess->m_ChatEditContainer.AddString( pMainGameProcess->m_LastSendedChatMessage, 
	strlen(pMainGameProcess->m_LastSendedChatMessage) );
	}
	else*/
/*	else
	{
		if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE )
		{
			if((g_LocalSystemTime - g_pLocalUser->m_LastModeChangeTime) > 1000)
			{
				g_pLocalUser->SetBattleMode( _XBATTLE_PEACE );
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PEACE );
				g_pLocalUser->m_LastModeChangeTime = g_LocalSystemTime;
			}
		}
		else
		{
			if((g_LocalSystemTime - g_pLocalUser->m_LastModeChangeTime) > 1000)
			{
				g_pLocalUser->SetBattleMode( _XBATTLE_BATTLE );
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_BATTLE );				
				g_pLocalUser->m_LastModeChangeTime = g_LocalSystemTime;
			}
		}
	}	*/
}
