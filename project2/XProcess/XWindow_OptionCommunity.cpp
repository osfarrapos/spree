// XWindow_OptionCommunity.cpp: implementation of the _XWindow_OptionCommunity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_OptionCommunity.h"
#include "XSR_STRINGHEADER.H"

extern void _XDrawEdgedSolidBar( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_OptionCommunity::_XWindow_OptionCommunity()
{
	m_pRejectionWhisperButton	= NULL;
	m_pRejectionFriendButton	= NULL;	
	m_pRejectionSubPartyButton	= NULL;
	m_pRejectionPartyButton		= NULL;	
	m_pRejectionExchangeButton	= NULL;	
	m_pConnectMessengerButton	= NULL;
#ifdef _XDEF_ZONECHATTING_20070108
	m_pBtnDisableZoneChatting	= NULL;
#endif
}

_XWindow_OptionCommunity::~_XWindow_OptionCommunity()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic option
BOOL _XWindow_OptionCommunity::Initialize( void )
{
	SetDrawBorder( FALSE );

	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 243, 20 }, 
	{ 14, 14 }, 
	_XDEF_OPTION_COMMUNITYREJECTIONWHISPER,
	optionmenuinterface,optionmenuinterface,
	&g_MainInterfaceTextureArchive };
	
	m_pRejectionWhisperButton = new _XCheckButton;
	m_pRejectionWhisperButton->Create( checkbtnstruct );	
	m_pRejectionWhisperButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pRejectionWhisperButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_bCommunityOptionRejectWhisper = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTWHISPER, FALSE, TRUE );	
	m_pRejectionWhisperButton->SetCheck( g_bCommunityOptionRejectWhisper );
	m_pRejectionWhisperButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pRejectionWhisperButton );

	checkbtnstruct.position.y = 40;
	checkbtnstruct.commandid = _XDEF_OPTION_COMMUNITYREJECTFRIEND;
	m_pRejectionFriendButton = new _XCheckButton;
	m_pRejectionFriendButton->Create( checkbtnstruct );	
	m_pRejectionFriendButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pRejectionFriendButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_bCommunityOptionRejectFriend = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTFRIEND, FALSE, TRUE );	
	m_pRejectionFriendButton->SetCheck( g_bCommunityOptionRejectFriend );
	m_pRejectionFriendButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pRejectionFriendButton );

	checkbtnstruct.position.y = 60;
	checkbtnstruct.commandid = _XDEF_OPTION_COMMUNITYREJECTPARTY;
	m_pRejectionSubPartyButton= new _XCheckButton;
	m_pRejectionSubPartyButton->Create( checkbtnstruct );	
	m_pRejectionSubPartyButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pRejectionSubPartyButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_bCommunityOptionRejectParty = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTPARTY, FALSE, TRUE );	
	m_pRejectionSubPartyButton->SetCheck( g_bCommunityOptionRejectParty );
	m_pRejectionSubPartyButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pRejectionSubPartyButton );

	checkbtnstruct.position.y = 80;
	checkbtnstruct.commandid = _XDEF_OPTION_COMMUNITYREJECTORGANIZATION;
	m_pRejectionPartyButton= new _XCheckButton;
	m_pRejectionPartyButton->Create( checkbtnstruct );	
	m_pRejectionPartyButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pRejectionPartyButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pRejectionPartyButton->SetCheck(g_bCommunityOptionRefuseOrganization);
	m_pRejectionPartyButton->SetButtonSwapMode( FALSE );
#ifdef _XTS_REFUSE_ORGANIZATION
#else
	m_pRejectionPartyButton->EnableWindow(FALSE); // 나중에 구현되면 푼다.
#endif
	InsertChildObject( m_pRejectionPartyButton );

	checkbtnstruct.position.y = 100;
	checkbtnstruct.commandid = _XDEF_OPTION_COMMUNITYEXCHANGE;
	m_pRejectionExchangeButton= new _XCheckButton;
	m_pRejectionExchangeButton->Create( checkbtnstruct );	
	m_pRejectionExchangeButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pRejectionExchangeButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_bCommunityOptionRejectPvPTrade = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTPVPTRADE, FALSE, TRUE );	
	m_pRejectionExchangeButton->SetCheck( g_bCommunityOptionRejectPvPTrade );
	m_pRejectionExchangeButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pRejectionExchangeButton );

	checkbtnstruct.position.y = 120;
	checkbtnstruct.commandid = _XDEF_OPTION_COMMUNITYTARGETINFO;
	m_pRejectionTargetInfoButton= new _XCheckButton;
	m_pRejectionTargetInfoButton->Create( checkbtnstruct );	
	m_pRejectionTargetInfoButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pRejectionTargetInfoButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_bCommunityOptionRejectTargetInfo = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTTARGETINFO, FALSE, TRUE );	
	m_pRejectionTargetInfoButton->SetCheck( g_bCommunityOptionRejectTargetInfo );
	m_pRejectionTargetInfoButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pRejectionTargetInfoButton );

	checkbtnstruct.position.y = 164;
	checkbtnstruct.commandid = _XDEF_OPTION_COMMUNITYCONNECTMESSENGER;
	m_pConnectMessengerButton= new _XCheckButton;
	m_pConnectMessengerButton->Create( checkbtnstruct );	
	m_pConnectMessengerButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pConnectMessengerButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_bCommunityOptionMessengerAutoLogin = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_MESSENGERAUTOLOGIN, TRUE, TRUE );	
	m_pConnectMessengerButton->SetCheck( g_bCommunityOptionMessengerAutoLogin );
	m_pConnectMessengerButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pConnectMessengerButton );
	
#ifdef _XDEF_ZONECHATTING_20070108
	checkbtnstruct.position.y += 20;
	checkbtnstruct.commandid = _XDEF_OPTION_COMMUNITYDISABLEZONECHAT;
	m_pBtnDisableZoneChatting= new _XCheckButton;
	m_pBtnDisableZoneChatting->Create( checkbtnstruct );	
	m_pBtnDisableZoneChatting->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pBtnDisableZoneChatting->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_bCommunityOptionDisableZoneChatting = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_DISABLEZONECHATTING, FALSE, TRUE );	
	m_pBtnDisableZoneChatting->SetCheck( g_bCommunityOptionDisableZoneChatting );
	m_pBtnDisableZoneChatting->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pBtnDisableZoneChatting );
#endif
	return TRUE;
}


void _XWindow_OptionCommunity::DestroyWindow( void )
{
	_XWindow::DestroyWindow();

	m_pRejectionWhisperButton	= NULL;
	m_pRejectionFriendButton	= NULL;	
	m_pRejectionSubPartyButton	= NULL;
	m_pRejectionPartyButton		= NULL;	
	m_pRejectionExchangeButton	= NULL;	
	m_pRejectionTargetInfoButton = NULL;
	m_pConnectMessengerButton	= NULL;
#ifdef _XDEF_ZONECHATTING_20070108
	m_pBtnDisableZoneChatting	= NULL;
#endif
}


BOOL _XWindow_OptionCommunity::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	


void _XWindow_OptionCommunity::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 9,  m_WindowPosition.x + 269, m_WindowPosition.y + 149, 0x80000000 );
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 153,  m_WindowPosition.x + 269, m_WindowPosition.y +  297, 0x80000000 );
	}

	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
		
	g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 180, 180, 180 ) );

	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 20, _XGETINTERFACETEXT(ID_STRING_OPTION_REFUSEWHISPER) );	// _T("귓속말 거부")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 40, _XGETINTERFACETEXT(ID_STRING_OPTION_REFUSEFRIEND) );	// _T("친구등록 거부")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 60, _XGETINTERFACETEXT(ID_STRING_OPTION_REFUSEPARTY) );		// _T("동행 신청 거부")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 80, _XGETINTERFACETEXT(ID_STRING_OPTION_REFUSEGUILD) );		// _T("분파/방회 신청 거부")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 100, _XGETINTERFACETEXT(ID_STRING_OPTION_REFUSETRADE) );	// _T("거래 신청 거부")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 120, _XGETINTERFACETEXT(ID_STRING_OPTION_2554) );	// _T("인물정보 거부")

	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 164, _XGETINTERFACETEXT(ID_STRING_OPTION_AUTOMESSANGER) );	// _T("메신져 자동 접속")
#ifdef _XDEF_ZONECHATTING_20070108
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 184, _XGETINTERFACETEXT(ID_STRING_NEW_3217) );	// _T("지역대화 끄기")
#endif
	g_XBaseFont->Flush();
}