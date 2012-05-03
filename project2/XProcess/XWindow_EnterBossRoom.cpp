// XWindow_EnterBossRoom.cpp: implementation of the _XWindow_EnterBossRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_Inventory.h"
#include "XMessageWindow.h"
#include "SoundEffectList.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_EnterBossRoom::_XWindow_EnterBossRoom()
{
	m_bSendPartyRequest		= FALSE;
	m_OKButton				= NULL;
	m_CancelButton			= NULL;
	m_NeedItemBorder		= NULL;
	m_ExistNeedItemBorder	= NULL;
	m_OKButtonLeftBorder	= NULL;
	m_OKButtonRightBorder	= NULL;

	m_CashItemBorder		= NULL;
	m_ExistCashItemBorder	= NULL;
	
	ResetNeedItemInfo();

	ResetData();
}

_XWindow_EnterBossRoom::~_XWindow_EnterBossRoom()
{

}

BOOL _XWindow_EnterBossRoom::Initialize( void )
{
	int namedmonsterimageindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		namedmonsterimageindex = g_MainInterfaceTextureArchive.FindResource( "us_MI_namedmob01.tga" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		namedmonsterimageindex = g_MainInterfaceTextureArchive.FindResource( "rs_MI_namedmob01.tga" );
	}
	else
	{
		namedmonsterimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_namedmob01.tga" );
	}
	
	m_NeedItemBorder = new _XImageStatic;
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
	m_NeedItemBorder->Create( 0, 0, 32, 32,&g_MainInterfaceTextureArchive, namedmonsterimageindex );	
	m_NeedItemBorder->SetClipRect( 11, 196, 43, 228 );

	m_CashItemBorder = new _XImageStatic;
	m_CashItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, namedmonsterimageindex);
	m_CashItemBorder->SetClipRect(125, 95, 158, 127);

	m_ExistCashItemBorder = new _XImageStatic;
	m_ExistCashItemBorder->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, namedmonsterimageindex);
	m_ExistCashItemBorder->SetClipRect(126, 139, 156, 169);
#else
	m_NeedItemBorder->Create( 0, 0, 40, 40,&g_MainInterfaceTextureArchive, namedmonsterimageindex );	
	m_NeedItemBorder->SetClipRect( 7, 192, 47, 232 );
#endif

	m_ExistNeedItemBorder = new _XImageStatic;
	m_ExistNeedItemBorder->Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, namedmonsterimageindex );	
	m_ExistNeedItemBorder->SetClipRect( 54, 197, 84, 227 );

	m_OKButtonLeftBorder = new _XImageStatic;
	m_OKButtonLeftBorder->Create( 0, 0, 26, 62, &g_MainInterfaceTextureArchive, namedmonsterimageindex );	
	m_OKButtonLeftBorder->SetClipRect( 222, 17, 248, 79);
	
	m_OKButtonRightBorder = new _XImageStatic;
	m_OKButtonRightBorder->Create( 0, 0, 26, 62, &g_MainInterfaceTextureArchive, namedmonsterimageindex );	
	m_OKButtonRightBorder->SetClipRect( 222, 17, 248, 79 );
	m_OKButtonRightBorder->SetScale( -1.0f, 1.0f );
	
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {71, 251}, {147, 37},
			_XDEF_BOSSROOM_ENTER,
			namedmonsterimageindex,
			namedmonsterimageindex,
			namedmonsterimageindex,
			&g_MainInterfaceTextureArchive
	};
	m_OKButton = new _XButton;
	m_OKButton->Create(btnstruct);
	m_OKButton->AttachXFont( _XFONTID_LARGE );
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 114, 182, 256, 219);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 114, 219, 256, 256);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 114, 219, 256, 256);
	if( g_LanguageType != _XLANGUAGE_TYPE_KOREAN )
	{
		m_OKButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ENTERBOSSROOM_START );	
	}

	m_OKButton->ShowWindow(FALSE);

	InsertChildObject(m_OKButton);
	
	int btnxpos = (int)((FLOAT)(gnWidth-1)*( ((368.0f+71.0f)+73.0f) / 1023.0f) - 73.0f);
	int btnypos = (int)((FLOAT)(gnHeight-1)*( ((200.0f+251.0f)+18.0f) / 767.0f) - 18.0f);	
	m_OKButton->MoveWindow( btnxpos, btnypos );
	
	m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((425.0f+16)/1023.0f) - 16;
	m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((279.0f+16)/767.0f) - 16;

	m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((560.0f+16)/1023.0f) - 16;
	m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((279.0f+16)/767.0f) - 16;

	m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((491.0f+16)/1023.0f) - 16;
	m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((343.0f+16)/767.0f) - 16;

	return TRUE;
}

void _XWindow_EnterBossRoom::MoveWindow( int X, int Y )
{	
	_XWindow::MoveWindow( X, Y );	
}

void _XWindow_EnterBossRoom::SetDefaultPosition( void )
{		
}

void _XWindow_EnterBossRoom::SaveWindowProperty( void )
{	
}

void _XWindow_EnterBossRoom::DestroyWindow( void )
{
	SAFE_DELETE(m_NeedItemBorder);
	SAFE_DELETE(m_ExistNeedItemBorder);
	SAFE_DELETE(m_OKButtonLeftBorder);
	SAFE_DELETE(m_OKButtonRightBorder);
	SAFE_DELETE(m_CashItemBorder);
	SAFE_DELETE(m_ExistCashItemBorder);

	_XWindow::DestroyWindow();
}

void _XWindow_EnterBossRoom::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
			
	}
	else
	{
		// 생사결 진입이 끝나면 Inventory lock 해제
		if(g_pInventory_Window)
		{
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}

#ifdef VN_RB_BOSSROOM_BUGFIX_070615_KUKURI
		m_OKButton->ShowWindow(FALSE);
#endif

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
		ResetData();
		m_OKButton->ShowWindow(FALSE);
#endif
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_EnterBossRoom::Draw( _XGUIObject*& pfocusobject )
{			
	_XWindow::Draw(pfocusobject);	

	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	if( m_OKButton->GetShowStatus() )
	{	
		POINT pt = m_OKButton->GetWindowPos();

		m_OKButtonLeftBorder->Draw( pt.x - 26,pt.y);
		m_OKButtonRightBorder->Draw( pt.x + 147+21, pt.y);
	}

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; ++i)
	{
		if(i == 0)
		{
	#ifdef _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI
			// cash item draw 하지 않음
	#else
			// cash item
			m_CashItemBorder->Draw(m_NeedItemSlotPosition[i].x,m_NeedItemSlotPosition[i].y);
			if(m_ItemInSlot[i].nInvenNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
			{
				g_pInventory_Window->DrawIcon(TRUE, m_NeedItemSlotPosition[i].x+1, m_NeedItemSlotPosition[i].y+1, &g_pLocalUser->m_UserItemList[m_ItemInSlot[i].nInvenNumber], FALSE);
			}
			m_ExistCashItemBorder->Draw( m_NeedItemSlotPosition[i].x+1, m_NeedItemSlotPosition[i].y+1 );
	#endif
		}
		else
		{
			m_NeedItemBorder->Draw(m_NeedItemSlotPosition[i].x,m_NeedItemSlotPosition[i].y);
			if(m_ItemInSlot[i].nInvenNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
			{
				g_pInventory_Window->DrawIcon(TRUE, m_NeedItemSlotPosition[i].x+1, m_NeedItemSlotPosition[i].y+1, &g_pLocalUser->m_UserItemList[m_ItemInSlot[i].nInvenNumber], FALSE);
			}
			m_ExistNeedItemBorder->Draw( m_NeedItemSlotPosition[i].x+1, m_NeedItemSlotPosition[i].y+1 );
		}
	}
#else
	// 필요한 아이템 이름  쓰기
	for( int i = 0; i< _XDEF_BOSSROOM_NEEDITEMNUMBER ; i ++)
	{
		m_NeedItemBorder->Draw(m_NeedItemSlotPosition[i].x-5,m_NeedItemSlotPosition[i].y-5);
		if( m_NeedItemID[i].sID != 0 )
		{
			g_pInventory_Window->DrawIcon(TRUE, m_NeedItemSlotPosition[i].x, m_NeedItemSlotPosition[i].y, &m_NeedItem[i]);
			m_ExistNeedItemBorder->Draw( m_NeedItemSlotPosition[i].x, m_NeedItemSlotPosition[i].y );
		}
	}
#endif
		
	if( g_pLocalUser->m_InParty )
	{
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) );
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->PutsAlign( (gnWidth>>1), (gnHeight*0.72916f), _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_2068) );
		g_XBaseFont->PutsAlign( (gnWidth>>1), (gnHeight*0.7486979f), _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_2069) );
		g_XBaseFont->PutsAlign( (gnWidth>>1), (gnHeight*0.76822916f), _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_2070) );
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);

		int count = 0;
		for( i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
		{
			if( m_EnterPartyMember[i].PartyMemberNo != -1 )
			{
				if( m_EnterPartyMember[i].bJoin )//승락
				{
					TCHAR buffer[256];
					sprintf(buffer, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_OK), g_pLocalUser->m_PartyInfo[m_EnterPartyMember[i].PartyMemberNo].playername);// "%s님이 수락하였습니다."
					int length = strlen( buffer );
					length = (gnWidth>>1) - (length*3);
					
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 250, 212, 138 ) );
					g_XBaseFont->Puts( length, (gnHeight*0.811197916f) + count*15 , buffer );	
					g_XBaseFont->Puts( length+1, (gnHeight*0.811197916f) + count*15 , buffer );	
				}
				else//거부 
				{
					TCHAR buffer[256];
					sprintf(buffer, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_CANCLE), g_pLocalUser->m_PartyInfo[m_EnterPartyMember[i].PartyMemberNo].playername);// "%s님이 거절하였습니다."
					int length = strlen( buffer );
					length = (gnWidth>>1) - (length*3);
					
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 154, 146, 130 ) );
					g_XBaseFont->Puts( length, (gnHeight*0.811197916f) + count*15 , buffer );	
					g_XBaseFont->Puts( length+1, (gnHeight*0.811197916f) + count*15 , buffer );		
					
				}
				count++;
			}
		}
	}
	


	g_XBaseFont->Flush();
}


BOOL _XWindow_EnterBossRoom::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{			
			if(mousestate->bButton[0])//마우스 왼쪽 클릭시 
			{
				if(CheckMousePosition())
				{
					m_CurrentSlotNumber = GetEmptySlotNumber();
					if( m_CurrentSlotNumber != -1 ) // 아이템을 올려놓는 곳을 클릭했을 때 
					{
						m_WindowMouseState = _XW_STATE_LEFTCLICK;
					}
					
				}
			}//마우스 왼쪽 클릭하고 있을 때 처리 끝 
			else if( mousestate->bButton[1] )//마우스 오른족 클릭시
			{
				if(CheckMousePosition())
				{
					m_CurrentSlotNumber = GetFullSlotNumber();
					if( m_CurrentSlotNumber != -1 ) // 아이템을 올려놓는 곳을 클릭했을 때 
					{
						m_WindowMouseState = _XW_STATE_RIGHTCLICK;
					}
					
				}
			
			}//마우스 오른족 클릭하고 있을 때 처리 끝 
		}

		
		if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
		{			
			if(!mousestate->bButton[0])
			{								
				if(g_pInventory_Window)
				{
					if(g_pInventory_Window->GetShowStatus())
					{
						if(g_pInventory_Window->m_bHaveMouseSlot)
						{								
							if(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)	//장착하고 있는 것은 불가능
							{
								//필요한 아이템인지 검사
								int reason = 0;
								if( CheckNeedItem(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(), g_pInventory_Window->m_MouseSlotItem.Get_m_sID(), reason) )
								{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
									SetItemInSlot(m_CurrentSlotNumber, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									ProcessItemInSlot();
#else
									if(g_pInventory_Window->m_MouseSlotItem.IsStackable())
									{									
										CopyItemToSlot(m_CurrentSlotNumber, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
										m_NeedItem[m_CurrentSlotNumber].m_ucCount = 1;
										m_NeedItem[m_CurrentSlotNumber].m_InventoryNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
										g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].m_ucCount -= 1;
										
										if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].m_ucCount <= 0)
										{
											g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Reset();
										}
										g_pInventory_Window->m_bHaveMouseSlot = FALSE;
										g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
										
									}
									else
									{
										CopyItemToSlot(m_CurrentSlotNumber, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
										m_NeedItem[m_CurrentSlotNumber].m_InventoryNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
										g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Reset();
										g_pInventory_Window->m_bHaveMouseSlot = FALSE;
										g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									}

									// 아이템을 올리고 난 후에 요구하는 아이템이 모두 장착 되었는지를 체크한다.
									if( !m_bSendPartyRequest )
									{
										if( CheckRequirement() ) // 요구하는 아이템이 모두 장착 되었을 때 
										{			
											if( g_pLocalUser->m_InParty ) // 파티면 파티원들에게 묻는다.
											{
												if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )  // 행수임.
												{
													m_bSendPartyRequest = TRUE;							
													g_pLocalUser->m_bWaitBossBattle = TRUE;
													
													g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 8 );
													
													TCHAR messagestring[256];
													memset( messagestring, 0, sizeof(TCHAR)*256 );
													sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_SENDMSG)); // 동행들에게 생사결에 들어가자는 전서구를 보냈습니다.
													
													_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
													pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);			
												}
											}
										}
									}
									else
									{
										CheckRequirement();
									}
#endif
								}
								else
								{
									if(reason == 2)
										g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3641), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "이미 패를 사용하고 있습니다."
									else
										g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_2071), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
								}
								
							} // if(g_pInventory_Window->m_MouseSlotItem.inventorynumber >= 16)
							
						} // if(g_pInventory_Window->m_bHaveMouseSlot)
					}
				} // if(g_pInventory_Window)
				
			}
		} // if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
		
		if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
		{			
			if(!mousestate->bButton[1])
			{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
				if( m_ItemInSlot[m_CurrentSlotNumber].bInsert)
#else
				if( m_NeedItem[m_CurrentSlotNumber].Get_m_sID() != 0 )
#endif
				{
					bool bitemremove = true;

					// 파티인 경우에 파티원들이 응답을 하지 않은 시기에는
					// 아이템을 삭제할 수 없도록 한다. 
					if( g_pLocalUser->m_InParty )
					{
						if( !m_OKButton->GetShowStatus() )
							bitemremove = false;
					}

					if( bitemremove )
					{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
						ResetItemInSlot(m_CurrentSlotNumber);
#else
						if( m_NeedItem[m_CurrentSlotNumber].IsStackable() )
						{
							int inventorynumber = m_NeedItem[m_CurrentSlotNumber].m_InventoryNumber;
							if(g_pLocalUser->m_UserItemList[inventorynumber].Get_m_sID() == 0)
							{
								SetNeedItemCheck(m_NeedItem[m_CurrentSlotNumber].m_cType, m_NeedItem[m_CurrentSlotNumber].Get_m_sID(), FALSE );
								CopySlotToInventory( m_CurrentSlotNumber, inventorynumber );
								
								m_NeedItem[m_CurrentSlotNumber].m_cType		=-1;
								m_NeedItem[m_CurrentSlotNumber].m_cSecond	= 0;
								m_NeedItem[m_CurrentSlotNumber].Set_m_sID(0);
								
								
								m_NeedItem[m_CurrentSlotNumber].m_cIdentify	= 0;
								m_NeedItem[m_CurrentSlotNumber].m_cThird	= 0;
								m_NeedItem[m_CurrentSlotNumber].m_ucCount	= 0;
								
								m_NeedItem[m_CurrentSlotNumber].m_cReserve	= 0;
								m_NeedItem[m_CurrentSlotNumber].Set_m_usCurDur(0);
								m_NeedItem[m_CurrentSlotNumber].Set_m_usMaxDur(0);
								
								for( int i = 0; i < 4; i++ )
									m_NeedItem[m_CurrentSlotNumber].Set_m_sSocket(i, 0);
								
								m_NeedItem[m_CurrentSlotNumber].Set_m_ucSocketCount(0);
							}
							else
							{
								SetNeedItemCheck(m_NeedItem[m_CurrentSlotNumber].m_cType, m_NeedItem[m_CurrentSlotNumber].Get_m_sID(), FALSE );
								g_pLocalUser->m_UserItemList[inventorynumber].m_ucCount += 1;
								
								m_NeedItem[m_CurrentSlotNumber].m_cType		=-1;
								m_NeedItem[m_CurrentSlotNumber].m_cSecond	= 0;
								m_NeedItem[m_CurrentSlotNumber].Set_m_sID(0);
								
								
								m_NeedItem[m_CurrentSlotNumber].m_cIdentify	= 0;
								m_NeedItem[m_CurrentSlotNumber].m_cThird	= 0;
								m_NeedItem[m_CurrentSlotNumber].m_ucCount	= 0;
								
								m_NeedItem[m_CurrentSlotNumber].m_cReserve	= 0;
								m_NeedItem[m_CurrentSlotNumber].Set_m_usCurDur(0);
								m_NeedItem[m_CurrentSlotNumber].Set_m_usMaxDur(0);
								
								for( int i = 0; i < 4; i++ )
									m_NeedItem[m_CurrentSlotNumber].Set_m_sSocket(i, 0);
								m_NeedItem[m_CurrentSlotNumber].Set_m_ucSocketCount(0);
							}
							
						}
						else
						{
							int inventorynumber = m_NeedItem[m_CurrentSlotNumber].m_InventoryNumber;
							if(g_pLocalUser->m_UserItemList[inventorynumber].Get_m_sID() == 0)
							{
								SetNeedItemCheck(m_NeedItem[m_CurrentSlotNumber].m_cType, m_NeedItem[m_CurrentSlotNumber].Get_m_sID(), FALSE );
								CopySlotToInventory( m_CurrentSlotNumber, inventorynumber );
								
								m_NeedItem[m_CurrentSlotNumber].m_cType		=-1;
								m_NeedItem[m_CurrentSlotNumber].m_cSecond	= 0;
								m_NeedItem[m_CurrentSlotNumber].Set_m_sID(0);
								
								
								m_NeedItem[m_CurrentSlotNumber].m_cIdentify	= 0;
								m_NeedItem[m_CurrentSlotNumber].m_cThird	= 0;
								m_NeedItem[m_CurrentSlotNumber].m_ucCount	= 0;
								
								m_NeedItem[m_CurrentSlotNumber].m_cReserve	= 0;
								m_NeedItem[m_CurrentSlotNumber].Set_m_usCurDur(0);
								m_NeedItem[m_CurrentSlotNumber].Set_m_usMaxDur(0);
								
								for( int i = 0; i < 4; i++)
									m_NeedItem[m_CurrentSlotNumber].Set_m_sSocket(i, 0);
								m_NeedItem[m_CurrentSlotNumber].Set_m_ucSocketCount(0);
							}
						}
#endif
						
						if(m_OKButton->GetShowStatus()) 
						{
							m_OKButton->ShowWindow(FALSE);

							// 파티가 생사결을 들어갈 때 한명이라도 승락을 해서 입장하기 버튼이 활성화 되어 있을떄 
							// 아이템을 빼게 되면 생사결 입장을 취소 시킨다.
							if( g_pLocalUser->m_InParty )
							{								
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_BOSSROOM_CANCLE, 0), 0 );
							}
						}
					}// if(bitemremove)
				}
								
			}
		} // if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
		
		if(!mousestate->bButton[0])
		{
			if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
				m_WindowMouseState = _XW_STATE_NONE;
		}
		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
				m_WindowMouseState = _XW_STATE_NONE;
		}
	}


	if( !_XWindow::Process(pfocusobject) ) return FALSE;
		
	
	return TRUE;
}


int _XWindow_EnterBossRoom::GetEmptySlotNumber(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
	{
		if(scrnpos->x > m_NeedItemSlotPosition[i].x && scrnpos->x < m_NeedItemSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_NeedItemSlotPosition[i].y && scrnpos->z < m_NeedItemSlotPosition[i].y+30)
			{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
				if(!m_ItemInSlot[i].bInsert)
#else
				if(m_NeedItem[i].Get_m_sID() == 0)
#endif
				{
					return i;
				}
			}
		}		
	}

	return -1;
}

int _XWindow_EnterBossRoom::GetFullSlotNumber(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
	{
		if(scrnpos->x > m_NeedItemSlotPosition[i].x && scrnpos->x < m_NeedItemSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_NeedItemSlotPosition[i].y && scrnpos->z < m_NeedItemSlotPosition[i].y+30)
			{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
				if(m_ItemInSlot[i].bInsert)
#else
				if(m_NeedItem[i].Get_m_sID() != 0)
#endif
				{
					return i;
				}
			}
		}		
	}

	return -1;
}

void _XWindow_EnterBossRoom::SetNeedItemCheck( int type, int id, bool bneeditem )
{
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
	{
		if( m_NeedItemID[i].sID	 == id && m_NeedItemID[i].cType == type)
		{
			m_NeedItemID[i].bNeedItem = bneeditem;
			break;
		}
	}
}

BOOL _XWindow_EnterBossRoom::CheckNeedItem( int type, int id, int& reason )
{
	reason = 0;

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMTYPE ; ++i)
	{
		if( m_NeedCashItemRef[i].sID == id && m_NeedCashItemRef[i].cType == type)
		{
			return TRUE;
		}
	}

	for(i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMTYPE ; ++i)
	{
		if( m_NeedNormalItemRef[i].sID == id && m_NeedNormalItemRef[i].cType == type)
		{
			if(!m_ItemInSlot[0].bInsert)	// cash가 비어있을 때만 일반 아이템 넣을 수 있음
				return TRUE;
			else
			{
				reason = 2;
				return FALSE;
			}
		}
	}
#else
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
	{
		//if( !m_NeedItemID[i].bNeedItem )
		{
			if( m_NeedItemID[i].sID	 == id && m_NeedItemID[i].cType == type)
			{
				//m_NeedItemID[i].bNeedItem = TRUE;
				return TRUE;
			}
		}		
	}
#endif

	reason = 1;
	return FALSE;
}

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
BOOL _XWindow_EnterBossRoom::CheckRequirement( void )
{
	#ifdef _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI

	#else

	// cash부터 검사한다
	if(m_ItemInSlot[0].bInsert)
	{
		for(int k = 0 ; k < 3 ; ++k)
		{
			if(m_NeedCashItemRef[k].cType == m_ItemInSlot[0].cType && m_NeedCashItemRef[k].sID == m_ItemInSlot[0].sID)
			{
				// 0 : 자색
				// 1 : 녹색
				// 2 : 자색
				m_BossRoomLevel = k;

				if( !g_pLocalUser->m_InParty )
				{
					// post message -> 조건 만족했음 이후 처리할 것.
					_XMessageWindow* pMessageWindow = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOMUSECASHITEMWINDOW);
					if(pMessageWindow)
					{
						if(pMessageWindow->GetShowStatus())
						{
							return FALSE;
						}
					}

					TCHAR msg[256];
					sprintf(msg, _XGETINTERFACETEXT(ID_STRING_NEW_3642),  _XGameItem::GetItemName(m_ItemInSlot[0].cType, m_ItemInSlot[0].sID), _XGameItem::GetItemName(m_ItemInSlot[0].cType, m_ItemInSlot[0].sID));	// "%s를 사용하시겠습니까? 사용시 %s 1개가 소모됩니다.",

					pMessageWindow = NULL;
					pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_ENTERBOSSROOMUSECASHITEMWINDOW, TRUE);
					if(pMessageWindow)
					{
						pMessageWindow->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), msg, TRUE, _XDEF_BOSSROOM_ASKPARTYMEMBER, _XDEF_BOSSROOM_USEITEMCANCEL);
						pMessageWindow->StartTimer(10000, TRUE);
					}
				}

				return TRUE;
			}
		}
	}

	#endif

	int count = 0;

	for(int i = 1 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
	{
		if( m_ItemInSlot[i].cType != -1 && m_ItemInSlot[i].sID > 0 )
		{
			count++;
			
		}
	}


	if( count == _XDEF_BOSSROOM_NEEDITEMNUMBER - 1 ) 
	{
		int atypecount = 0;
		int btypecount = 0;
		int ctypecount = 0;
		
		// 올려 놓은 아이템의 타입 조합을 알아낸다.
		for(int i = 1 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				if( m_NeedNormalItemRef[j].sID == m_ItemInSlot[i].sID && 
					m_NeedNormalItemRef[j].cType == m_ItemInSlot[i].cType)
				{
					if( j == 0 ) // A타입 
					{
						ctypecount++;
					}
					else if( j == 1)// B타입
					{
						btypecount++;
					}
					else if( j == 2)// C타입
					{
						atypecount++;
					}					
				}
			}
		}

		int easy, normal, hard, expert;
		easy = normal = hard = expert = 0;
		// 조합종류 10가지 
		if( atypecount == 3 ) //AAA
		{
			normal = 25;
			hard = 70;
			expert = 5;
		}
		else if( atypecount == 2 && btypecount == 1)//AAB
		{
			easy = 10;
			normal = 20;
			hard = 70;
		}
		else if( atypecount == 2 && ctypecount == 1)//AAC
		{
			easy = 20;
			normal = 10;
			hard = 70;
		}
		else if( btypecount == 3 )//BBB
		{
			easy = 20;
			normal = 80;
		}
		else if( btypecount == 2 && atypecount == 1)//BBA
		{
			easy = 20;
			normal = 70;
			hard = 10;
		}
		else if( btypecount == 2 && ctypecount == 1)//BBC
		{
			easy = 25;
			normal = 75;
			hard = 0;
		}
		else if( ctypecount == 3 )//CCC
		{
			easy = 70;
			normal = 30;
		}
		else if( ctypecount == 2 && atypecount == 1)//CCA
		{
			easy = 60;
			normal = 35;
			hard = 5;
		}
		else if( ctypecount == 2 && btypecount == 1)//CCB
		{
			easy = 60;
			normal = 40;
			hard = 0;
		}
		else if( atypecount == 1 && btypecount == 1 && ctypecount == 1)//ABC
		{
			easy = 30;
			normal = 40;
			hard = 30;
		}
		
		int itemdifficulty = _XRandom()%100;
		if( itemdifficulty >= 0 && itemdifficulty < easy)
		{
			m_BossRoomLevel = 0;
		}
		else if( itemdifficulty >= easy && itemdifficulty < easy+normal )
		{
			m_BossRoomLevel = 1;
		}
		else if( itemdifficulty >= easy+normal && itemdifficulty < easy+normal+hard )
		{
			m_BossRoomLevel = 2;
		}
		else if(itemdifficulty >= easy+normal+hard && itemdifficulty < easy+normal+hard+expert)
		{
			m_BossRoomLevel = 3;
		}

		// 같은 색상 3개 사용할 경우 난이도 고정 //2008.09.12 Author:박소현
		if(atypecount == 3)	
		{
			// 적색 3개 
			m_BossRoomLevel = 2;
		}
		else if(btypecount == 3)
		{
			// 녹색 3개
			m_BossRoomLevel = 1;
		}
		else if(ctypecount == 3)
		{
			// 자색 3개
			m_BossRoomLevel = 0;
		}
		
		if( !g_pLocalUser->m_InParty )
		{
			PostMessage(gHWnd, WM_COMMAND, _XDEF_BOSSROOM_ASKPARTYMEMBER, 0);
		}

		return TRUE;
	}
	else
	{
		m_OKButton->ShowWindow(FALSE);
	}

	return FALSE;
}
#else
BOOL _XWindow_EnterBossRoom::CheckRequirement( void )
{
	int count = 0;

	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
	{
		if( m_NeedItem[i].m_cType != -1 && m_NeedItem[i].Get_m_sID() > 0 )
		{
			count++;
			
		}
	}

	if( count == _XDEF_BOSSROOM_NEEDITEMNUMBER ) 
	{
		int atypecount = 0;
		int btypecount = 0;
		int ctypecount = 0;
		
		// 올려 놓은 아이템의 타입 조합을 알아낸다.
		for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				if( m_NeedItemID[j].sID	 == m_NeedItem[i].Get_m_sID() && m_NeedItemID[j].cType == m_NeedItem[i].m_cType)
				{
					if( j == 0 ) // A타입 
					{
						atypecount++;
					}
					else if( j == 1)// B타입
					{
						btypecount++;
					}
					else if( j == 2)// C타입
					{
						ctypecount++;
					}					
				}
			}
		}

		int easy, normal, hard, expert;
		easy = normal = hard = expert = 0;
		// 조합종류 10가지 
		if( atypecount == 3 ) //AAA
		{
			normal = 25;
			hard = 70;
			expert = 5;
		}
		else if( atypecount == 2 && btypecount == 1)//AAB
		{
			easy = 10;
			normal = 20;
			hard = 70;
		}
		else if( atypecount == 2 && ctypecount == 1)//AAC
		{
			easy = 20;
			normal = 10;
			hard = 70;
		}
		else if( btypecount == 3 )//BBB
		{
			easy = 20;
			normal = 80;
		}
		else if( btypecount == 2 && atypecount == 1)//BBA
		{
			easy = 20;
			normal = 70;
			hard = 10;
		}
		else if( btypecount == 2 && ctypecount == 1)//BBC
		{
			easy = 25;
			normal = 75;
			hard = 0;
		}
		else if( ctypecount == 3 )//CCC
		{
			easy = 70;
			normal = 30;
		}
		else if( ctypecount == 2 && atypecount == 1)//CCA
		{
			easy = 60;
			normal = 35;
			hard = 5;
		}
		else if( ctypecount == 2 && btypecount == 1)//CCB
		{
			easy = 60;
			normal = 40;
			hard = 0;
		}
		else if( atypecount == 1 && btypecount == 1 && ctypecount == 1)//ABC
		{
			easy = 30;
			normal = 40;
			hard = 30;
		}
		
		int itemdifficulty = _XRandom()%100;
		if( itemdifficulty >= 0 && itemdifficulty < easy)
		{
			m_BossRoomLevel = 0;
		}
		else if( itemdifficulty >= easy && itemdifficulty < easy+normal )
		{
			m_BossRoomLevel = 1;
		}
		else if( itemdifficulty >= easy+normal && itemdifficulty < easy+normal+hard )
		{
			m_BossRoomLevel = 2;
		}
		else if(itemdifficulty >= easy+normal+hard && itemdifficulty < easy+normal+hard+expert)
		{
			m_BossRoomLevel = 3;
		}
		
		if( !g_pLocalUser->m_InParty )
		{
			m_OKButton->ShowWindow(TRUE);
			_XPlayInterfaceSound(ID_SR_INTERFACE_BOSSBUTTON00_WAV);
		}

		return TRUE;
	}
	else
	{
		m_OKButton->ShowWindow(FALSE);
	}

	return FALSE;
}
#endif


void _XWindow_EnterBossRoom::ResetNeedItemInfo(void)
{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI

#else

	m_CurrentSlotNumber = -1;
	
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
	{
		m_NeedItem[i].ResetTradeItem();

		m_NeedItemID[i].sID	  = 0;
		m_NeedItemID[i].cType = -1;
		m_NeedItemID[i].bNeedItem = FALSE;
	}
	for( i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
	{
		m_EnterPartyMember[i].PartyMemberNo = -1;
		m_EnterPartyMember[i].bJoin = FALSE;
	}
	m_bSendPartyRequest = FALSE;
	m_BossRoomLevel = -1;

#endif
	
}

void _XWindow_EnterBossRoom::CopyItemToSlot(int slotnumber, int invennumber)
{
	_ASSERT( slotnumber < _XDEF_BOSSROOM_NEEDITEMNUMBER );
	_ASSERT( invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT );

	m_NeedItem[slotnumber].Set_m_cType( g_pLocalUser->m_UserItemList[invennumber].Get_m_cType() );
	m_NeedItem[slotnumber].Set_m_cSecond( g_pLocalUser->m_UserItemList[invennumber].Get_m_cSecond() );
	m_NeedItem[slotnumber].Set_m_sID(g_pLocalUser->m_UserItemList[invennumber].Get_m_sID() );
	m_NeedItem[slotnumber].m_cIdentify	= g_pLocalUser->m_UserItemList[invennumber].m_cIdentify;
	
	m_NeedItem[slotnumber].Set_m_cThird( g_pLocalUser->m_UserItemList[invennumber].Get_m_cThird() );
	m_NeedItem[slotnumber].Set_m_ucCount( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucCount() );
	
	m_NeedItem[slotnumber].Set_m_usCurDur(g_pLocalUser->m_UserItemList[invennumber].Get_m_usCurDur());
	m_NeedItem[slotnumber].Set_m_usMaxDur(g_pLocalUser->m_UserItemList[invennumber].Get_m_usMaxDur());
	
	for( int i = 0; i < 4; i++ )
		m_NeedItem[slotnumber].Set_m_sSocket( i,g_pLocalUser->m_UserItemList[invennumber].Get_m_sSocket(i) );

	m_NeedItem[slotnumber].Set_m_ucSocketCount(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucSocketCount());
}

void _XWindow_EnterBossRoom::CopySlotToInventory(int slotnumber, int invennumber)
{
	_ASSERT( slotnumber < _XDEF_BOSSROOM_NEEDITEMNUMBER );
	_ASSERT( invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT );

	g_pLocalUser->m_UserItemList[invennumber].Set_m_cType( m_NeedItem[slotnumber].Get_m_cType() );
	g_pLocalUser->m_UserItemList[invennumber].Set_m_cSecond(m_NeedItem[slotnumber].Get_m_cSecond());
	g_pLocalUser->m_UserItemList[invennumber].Set_m_sID(m_NeedItem[slotnumber].Get_m_sID());	
	g_pLocalUser->m_UserItemList[invennumber].m_cIdentify =		m_NeedItem[slotnumber].m_cIdentify	;	
		
	g_pLocalUser->m_UserItemList[invennumber].Set_m_cThird(_XGameItem::GetThird(m_NeedItem[slotnumber].Get_m_cType(), m_NeedItem[slotnumber].Get_m_sID()) );	
	g_pLocalUser->m_UserItemList[invennumber].Set_m_ucCount(m_NeedItem[slotnumber].Get_m_ucCount());
		
	g_pLocalUser->m_UserItemList[invennumber].Set_m_usCurDur(m_NeedItem[slotnumber].Get_m_usCurDur());
	g_pLocalUser->m_UserItemList[invennumber].Set_m_usMaxDur(_XGameItem::GetMaxDurability(m_NeedItem[slotnumber].Get_m_cType(), m_NeedItem[slotnumber].Get_m_sID()) );
		
	for( int i = 0; i < 4; i++ )
		g_pLocalUser->m_UserItemList[invennumber].Set_m_sSocket(i,m_NeedItem[slotnumber].Get_m_sSocket(i));	
	
	g_pLocalUser->m_UserItemList[invennumber].Set_m_ucSocketCount(m_NeedItem[slotnumber].Get_m_ucSocketCount());
}

void _XWindow_EnterBossRoom::SetSlotPosition(int mapid)
{
	// 아이템 소켓 위치 정렬

	switch(mapid)
	{
	case 50 :	// 남사망곡
	case 51 :	// 북사망곡
	case 52 :	// 서염왕곡
	case 53 :	// 동염왕곡
		{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI

	#ifdef _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI

			m_NeedItemSlotPosition[0].x = 0;
			m_NeedItemSlotPosition[0].y = 0;

			m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((425.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((279.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((560.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((279.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((491.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((343.0f+16)/767.0f) - 16;

	#else

			m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((491.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((200.0f+16)/767.0f) - 16;

			m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((425.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((279.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((560.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((279.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((491.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((343.0f+16)/767.0f) - 16;

	#endif

#else
			m_NeedItemSlotPosition[0].x = gnWidth*(425.0f/1024.0f);
			m_NeedItemSlotPosition[0].y = gnHeight*(279.0f/768.0f);
					
			m_NeedItemSlotPosition[1].x = gnWidth*(560.0f/1024.0f);
			m_NeedItemSlotPosition[1].y = gnHeight*(279.0f/768.0f);
					
			m_NeedItemSlotPosition[2].x = gnWidth*(491.0f/1024.0f);
			m_NeedItemSlotPosition[2].y = gnHeight*(343.0f/768.0f);
#endif
		}
		break;
	case 54 :	// 흑룡결
	case 55 :	// 맹수각
		{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI

	#ifdef _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI

			if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE ||
			   g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
			   g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((245.0f+16)/767.0f) - 16;

				m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((304.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((364.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((424.0f+16)/767.0f) - 16;
			}
			else
			{
				m_NeedItemSlotPosition[0].x = 0;
				m_NeedItemSlotPosition[0].y = 0;

				m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((236.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((321.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((410.0f+16)/767.0f) - 16;
			}

	#else

			m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((245.0f+16)/767.0f) - 16;

			m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((304.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((364.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((424.0f+16)/767.0f) - 16;

	#endif	

#else
			m_NeedItemSlotPosition[0].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[0].y = gnHeight*(236.0f/768.0f);
					
			m_NeedItemSlotPosition[1].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[1].y = gnHeight*(321.0f/768.0f);
					
			m_NeedItemSlotPosition[2].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[2].y = gnHeight*(410.0f/768.0f);

#endif
		}
		break;
	case 56 :	// 만금전
		{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI

	#ifdef _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI

			if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE ||
			   g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
			   g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((195.0f+16)/767.0f) - 16;

				m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((269.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((342.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((412.0f+16)/767.0f) - 16;
			}
			else
			{
				m_NeedItemSlotPosition[0].x = 0;
				m_NeedItemSlotPosition[0].y = 0;

				m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((206.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((306.0f+16)/767.0f) - 16;
						
				m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
				m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((406.0f+16)/767.0f) - 16;
			}
	#else
 
			m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((195.0f+16)/767.0f) - 16;

			m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((269.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((342.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((412.0f+16)/767.0f) - 16;

	#endif

#else
			m_NeedItemSlotPosition[0].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[0].y = gnHeight*(206.0f/768.0f);
					
			m_NeedItemSlotPosition[1].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[1].y = gnHeight*(306.0f/768.0f);
					
			m_NeedItemSlotPosition[2].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[2].y = gnHeight*(406.0f/768.0f);

#endif
		}
		break;
	case 57 :	// 천은전
		{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI

	#ifdef _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI

			m_NeedItemSlotPosition[0].x = 0;
			m_NeedItemSlotPosition[0].y = 0;

			m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((206.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((306.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((406.0f+16)/767.0f) - 16;

	#else

			m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((189.0f+16)/767.0f) - 16;

			m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((265.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((339.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((496.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((416.0f+16)/767.0f) - 16;		

	#endif

#else
			m_NeedItemSlotPosition[0].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[0].y = gnHeight*(206.0f/768.0f);
					
			m_NeedItemSlotPosition[1].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[1].y = gnHeight*(306.0f/768.0f);
					
			m_NeedItemSlotPosition[2].x = gnWidth*(498.0f/1024.0f);
			m_NeedItemSlotPosition[2].y = gnHeight*(406.0f/768.0f);

#endif
		}
		break;
	case 58 : // Author : 양희왕 //breif : 요동, 승천결
		{
			m_NeedItemSlotPosition[0].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[0].y = (FLOAT)(gnHeight-1)*((225.0f+16)/767.0f) - 16;

			m_NeedItemSlotPosition[1].x = (FLOAT)(gnWidth-1)*((408.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[1].y = (FLOAT)(gnHeight-1)*((354.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[2].x = (FLOAT)(gnWidth-1)*((498.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[2].y = (FLOAT)(gnHeight-1)*((354.0f+16)/767.0f) - 16;
					
			m_NeedItemSlotPosition[3].x = (FLOAT)(gnWidth-1)*((582.0f+16)/1023.0f) - 16;
			m_NeedItemSlotPosition[3].y = (FLOAT)(gnHeight-1)*((354.0f+16)/767.0f) - 16;
		}
		break;
	}
}

void _XWindow_EnterBossRoom::SetNeedNormalItemRef(int index, char ctype, short sid)
{
	m_NeedNormalItemRef[index].cType = static_cast<int>(ctype);
	m_NeedNormalItemRef[index].sID = static_cast<int>(sid);
}

void _XWindow_EnterBossRoom::ResetNeedNormalItemRef()
{
	for(int i = 0 ; i < 3 ; ++i)
	{
		m_NeedNormalItemRef[i].cType = 0;
		m_NeedNormalItemRef[i].sID = 0;
	}
}

void _XWindow_EnterBossRoom::SetItemInSlot(int index, int invennumber)
{
	if(m_OKButton->GetShowStatus())
	{
		// 이미 처리 되었음
		return;
	}
				
	if(index == 0)
	{
		// only cash
	#ifdef _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI

	#else

		if(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType() == _XGI_FC_RESOURCE)
		{
			if(g_ResourceItemProperty[g_pLocalUser->m_UserItemList[invennumber].Get_m_sID()].cCashCheck == 1)
			{
				if(!m_ItemInSlot[index].bInsert)
				{
					m_ItemInSlot[index].nInvenNumber = invennumber;
					m_ItemInSlot[index].cType = static_cast <int>(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType());
					m_ItemInSlot[index].sID = static_cast <int>(g_pLocalUser->m_UserItemList[invennumber].Get_m_sID());
					m_ItemInSlot[index].bInsert = TRUE;

					
					for(int i = 1 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; ++i)
					{
						// 다른 일반 아이템 빼기
						ResetItemInSlot(i);
					}
				}
			}
		}

	#endif

	}
	else
	{
		if(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType() == _XGI_FC_RESOURCE)
		{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
			if(CheckAllSlot(invennumber))
			{
#endif
				if(g_ResourceItemProperty[g_pLocalUser->m_UserItemList[invennumber].Get_m_sID()].cCashCheck == 0)
				{
					m_ItemInSlot[index].nInvenNumber = invennumber;
					m_ItemInSlot[index].cType = static_cast <int>(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType());
					m_ItemInSlot[index].sID = static_cast <int>(g_pLocalUser->m_UserItemList[invennumber].Get_m_sID());
					m_ItemInSlot[index].bInsert = TRUE;
				}
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
			}
#endif
		}
	}
}

void _XWindow_EnterBossRoom::ResetItemInSlot(int index)
{
	m_ItemInSlot[index].cType = 0;
	m_ItemInSlot[index].sID = 0;
	m_ItemInSlot[index].nInvenNumber = 0;
	m_ItemInSlot[index].bInsert = FALSE;

	if(index == 0)
	{
		// message 창 떠있으면 닫기
		g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_ENTERBOSSROOMUSECASHITEMWINDOW);
	}
}

void _XWindow_EnterBossRoom::ResetItemInSlot()
{
	for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; ++i)
	{
		m_ItemInSlot[i].cType = 0;
		m_ItemInSlot[i].sID = 0;
		m_ItemInSlot[i].nInvenNumber = 0;
		m_ItemInSlot[i].bInsert = FALSE;
	}

	// message 창 떠있으면 닫기
	g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_ENTERBOSSROOMUSECASHITEMWINDOW);
}

void _XWindow_EnterBossRoom::ProcessItemInSlot()
{
	// 아이템을 올리고 난 후에 요구하는 아이템이 모두 장착 되었는지를 체크한다.
	if( !m_bSendPartyRequest )
	{
		if( CheckRequirement() ) // 요구하는 아이템이 모두 장착 되었을 때 
		{			
			if( g_pLocalUser->m_InParty ) // 파티면 파티원들에게 묻는다.
			{
				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )  // 행수임.
				{
					PostMessage(gHWnd, WM_COMMAND, _XDEF_BOSSROOM_ASKPARTYMEMBER, 0);
				}
			}
		}
	}
	else
	{
		CheckRequirement();
	}		
}

void _XWindow_EnterBossRoom::SetNeedCashItemRef(int index, char ctype, short sid)
{
	m_NeedCashItemRef[index].cType = static_cast<int>(ctype);
	m_NeedCashItemRef[index].sID = static_cast<int>(sid);
}

void _XWindow_EnterBossRoom::ResetNeedCashItemRef()
{
	for(int i = 0 ; i < 3 ; ++i)
	{
		m_NeedCashItemRef[i].cType = 0;
		m_NeedCashItemRef[i].sID = 0;
	}
}

void _XWindow_EnterBossRoom::ResetData()
{
	m_CurrentSlotNumber = -1;

	ResetNeedNormalItemRef();
	ResetNeedCashItemRef();
	ResetItemInSlot();

	for(int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
	{
		m_EnterPartyMember[i].PartyMemberNo = -1;
		m_EnterPartyMember[i].bJoin = FALSE;
	}
	m_bSendPartyRequest = FALSE;
	m_BossRoomLevel = -1;
}

BOOL _XWindow_EnterBossRoom::CheckAllSlot(int invennumber)
{
	int usecnt = 0;

	for(int i = 1 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; ++i)
	{
		if(m_ItemInSlot[i].bInsert)
		{
			if(m_ItemInSlot[i].nInvenNumber == invennumber)
				usecnt++;
		}
	}

	if(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucCount() > usecnt)
		return TRUE;
	else
		return FALSE;
}