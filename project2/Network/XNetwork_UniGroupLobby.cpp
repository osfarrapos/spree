#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XWindow_ServerUnification.h"
#include "XWindow_SelectCharacter.h"
#include "XSR_STRINGHEADER.H"

void _XNetwork::MSGServerUnification(MSG_UNI_GROUP_LOBBY* serveruniinfo)
{
#ifdef _XTS_NEWCHARACLIST // _XTS_SERVERUNIFICATION 이 사용되지 않을때는 캐릭명 변경기능만 사용함.
	switch(serveruniinfo->ucMode) 
	{
		case en_ug_change_name_result :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;			

			MSG_UG_CHANGE_NAME_RESULT* changenameresult = (MSG_UG_CHANGE_NAME_RESULT*)serveruniinfo;
			if( changenameresult )
			{
				if( changenameresult->ucSuccess == 0 )	// 성공
				{
					_XWindowManager* selectcharacterWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.GetWindowManagerPtr();
					if( selectcharacterWindowMgr )
					{
						_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)selectcharacterWindowMgr->FindWindow(_XDEF_WTITLE_SELECTCHARACTER);
						if( pSelectCharacter_Window )
						{
							strcpy( pSelectCharacter_Window->m_Name, changenameresult->ucNewName );
							switch(pSelectCharacter_Window->m_SelectedCharacter) 
							{
							case 0:
								pSelectCharacter_Window->m_Blank01Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
								pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
								break;
							case 1:
								pSelectCharacter_Window->m_Blank02Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
								pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
								break;
							case 2:
								pSelectCharacter_Window->m_Blank03Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
								pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
								break;
							}								
						}
					}				
					strcpy( g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].charactername, changenameresult->ucNewName);
					strcpy( g_pLocalUser->m_CharacterInfo.charactername, changenameresult->ucNewName );
					g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].ucChangeNameFlag = 0;
				}				
				else	// 실패
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_EXISTEDCHARACTER) );
				}
			}
		}
		break;
	}
#else

#ifdef _XTS_SERVERUNIFICATION
	switch(serveruniinfo->ucMode) 
	{
	case en_ug_simple_grp_info :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;

			_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
			if( !pServerUnification_Window )
				return;

			MSG_UG_SIMPLE_GROUP_INFO* groupinfo = (MSG_UG_SIMPLE_GROUP_INFO*)serveruniinfo;
			if( groupinfo )
				pServerUnification_Window->SetSimpleGroupInfo(groupinfo);
		}
		break;
	case en_ug_select_simple_grp_fail :
		break;
	case en_ug_detail_grp_info :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;

			_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
			if( !pServerUnification_Window )
				return;

			MSG_UG_DETAIL_GRP_INFO* detailinfo = (MSG_UG_DETAIL_GRP_INFO*)serveruniinfo;
			if( detailinfo )
				pServerUnification_Window->SetDetailGroupInfo(detailinfo);
		}
		break;
	case en_ug_move_result_complete :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;

			_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
			if( !pServerUnification_Window )
				return;

			MSG_UG_MOVE_RESULT_COMPLETE* moveresult = (MSG_UG_MOVE_RESULT_COMPLETE*)serveruniinfo;
			if( moveresult )
				pServerUnification_Window->SetMoveResult(moveresult);
		}
		break;
	case en_ug_move_result_change_name :
		break;
	case en_ug_move_result_fail :
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("통합서버로 이전이 불가능합니다. 주민번호 뒷자리를 확인 후 다시 시도 하시기 바랍니다.") );
		}
		break;
	case en_ug_change_name_result :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;			

			MSG_UG_CHANGE_NAME_RESULT* changenameresult = (MSG_UG_CHANGE_NAME_RESULT*)serveruniinfo;
			if( changenameresult )
			{
				if( changenameresult->ucSuccess == 0 )	// 성공
				{
					int selectedservernum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					if( selectedservernum == _GR_INDEX_UNION_ )
					{
						_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
						if( pServerUnification_Window )				
							pServerUnification_Window->SetChangeNameResult(changenameresult);
					}
					else
					{
						_XWindowManager* selectcharacterWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.GetWindowManagerPtr();
						if( selectcharacterWindowMgr )
						{
							_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)selectcharacterWindowMgr->FindWindow(_XDEF_WTITLE_SELECTCHARACTER);
							if( pSelectCharacter_Window )
							{
								strcpy( pSelectCharacter_Window->m_Name, changenameresult->ucNewName );
								switch(pSelectCharacter_Window->m_SelectedCharacter) 
								{
								case 0:
									pSelectCharacter_Window->m_Blank01Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
									pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
									break;
								case 1:
									pSelectCharacter_Window->m_Blank02Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
									pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
									break;
								case 2:
									pSelectCharacter_Window->m_Blank03Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
									pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
									break;
								}								
							}
						}				
						strcpy( g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].charactername, changenameresult->ucNewName);
						strcpy( g_pLocalUser->m_CharacterInfo.charactername, changenameresult->ucNewName );
						g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].ucChangeNameFlag = 0;
					}
				}				
				else	// 실패
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("중복된 이름입니다.") );
				}
			}
		}
		break;
	case en_ug_notyet_move_in_new :		// 30 신서버군 : 아직 통합을 완료하지 않았다.
		{
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("서버통합이 완료되지 않았습니다. 서버 통합을 하려면 먼저 서버 리스트에서 “서버 통합” 서버를 선택하셔야 합니다."),
						TRUE, _XDEF_LS_COMPULSORYSTEP2YES );
		}
		break;
	case en_ug_force_use_charac_in_new :	// en_ug_notyet_move_in_new 수락시 
		{
		}
		break;
	case en_ug_already_move_in_old :	// 32 이미 통합이 완료된 구서버군으로 접속. 메세지 보여주고 튕김.
		{
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("통합이 완료된 서버입니다. 신 서버로 접속하여 주시기 바랍니다.") );
		}
		break;
	case en_ug_force_use_result :
		{
			MSG_UG_FORCE_USE_RESULT* forceuseresult = (MSG_UG_FORCE_USE_RESULT*)serveruniinfo;
			if( forceuseresult )
			{
				switch(forceuseresult->ucResult)	// 0 - Success, 1 - 비번 확인, 2 - 실패.
				{
				case 0 :
					break;
				case 1 :
					{
						((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("주민번호 뒷자리를 확인 후 다시 시도 하시기 바랍니다.") );
					}
					break;
				case 2 :
					{
						((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("접속에 실패하였습니다.") );
					}
					break;
				}			
			}			
		}
		break;
	}	
#endif //#ifdef _XTS_SERVERUNIFICATION

#endif //#ifdef _XTS_NEWCHARACLIST
}