// XWindow_MessengerDialog.h: interface for the _XWindow_MessengerDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MESSENGERDIALOG_H__0CBB024E_B76C_40B7_B9B0_370CE33C0E80__INCLUDED_)
#define AFX_XWINDOW_MESSENGERDIALOG_H__0CBB024E_B76C_40B7_B9B0_370CE33C0E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XWindow.h"
#include "XMnDStructInfo.h"

#define _XDEF_MESSENGERDIALOGWINDOW_WIDTH		250
#define _XDEF_MESSENGERDIALOGWINDOW_HEIGHT		278

#define _XDEF_INSTANCEMESSAGEWINDOW_WIDTH		183
#define _XDEF_INSTANCEMESSAGEWINDOW_HEIGHT		46

typedef enum MessengerDialogButton
{
	_XMESSENGERDIALOGBUTTON_HELP	= 0,		
		_XMESSENGERDIALOGBUTTON_MINIMIZE,		
		_XMESSENGERDIALOGBUTTON_CLOSE,			
		_XMESSENGERDIALOGBUTTON_INFORMATION,		
		_XMESSENGERDIALOGBUTTON_PARTYREQUEST,	
		_XMESSENGERDIALOGBUTTON_FRIENDCUT,		
		_XMESSENGERDIALOGBUTTON_CHATINPUT,		
		_XMESSENGERDIALOGBUTTON_EMOTICON
}; 

typedef enum _XMessengerDialogMode
{
	_XMESSENGERDIALOGMODE_INSTANCE		= 0,
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	_XMESSENGERDIALOGMODE_DIALOG,
	_XMESSENGERDIALOGMODE_MNDINSTANCE, //사제 최소화 창
	_XMESSENGERDIALOGMODE_MASTERDIALOG, //스승이 보는 대화창
	_XMESSENGERDIALOGMODE_DISCIPLEDIALOG, //제자가 보는 창 
	_XMESSENGERDIALOGMODE_MNDGROUPDIALOG //사제 그룹 대화
#else
	_XMESSENGERDIALOGMODE_DIALOG
#endif

}_XMESSENGERDIALOG_MODE;

class _XWindow_MessengerDialog   : public _XWindow  
{	
public:	
	_XImageStatic*		m_imageInstanceMessageTitleBar;			// 메신져 인스턴트 메세지 창 타이틀 바	
	_XImageStatic*		m_imageInstanceMessageEndLine;			// 메신져 인스턴트 메세지 창 끝 라인

	_XImageStatic*		m_imageMessengerDialogTitleBar;			// 메신져 대화창 타이틀 바	
	_XImageStatic*		m_imageMessengerDialogLeftSideBorder;	// 메신져 대화창 왼쪽 사이드 이미지
	_XImageStatic*		m_imageMessengerDialogRightSideBorder;	// 메신져 대화창 오른쪽 사이드 이미지
	_XImageStatic*		m_imageMessengerDialogDownBorder;		// 메신져 대화창 창 아래쪽  바탕
	_XImageStatic*		m_imageMessengerDialogEmoticonBox;		// 메신져 대화창 이모티콘 박스 


	_XImageStatic*		m_imageMessengerDialogAddTitleBar;		// Author :양희왕 //메신져 대화창 타이틀 바 아랫 부분 확장	
	_XImageStatic*		m_pImageMessengerDialogBorder_1;		// Author :양희왕 //스승 제자 대화시 테두리
	_XImageStatic*		m_pImageMessengerDialogBorder_2;		// Author :양희왕 //스승 제자 대화시 테두리
	_XImageStatic*		m_pImageMessengerDialogBorder_3;		// Author :양희왕 //스승 제자 대화시 테두리
	_XImageStatic*		m_pImageMessengerDialogBorder_4;		// Author :양희왕 //스승 제자 대화시 테두리

	_XButton*			m_pBtnMessengerMnDAccept;				// Author :양희왕 //스승 승인 버튼
	_XButton*			m_pBtnMessengerMnDReject;				// Author :양희왕 //스승 거절 버튼
	
	_XButton*			m_btnMessengerDialogHelp;				// 메신져 대화창 도움 버튼 
	_XButton*			m_btnMessengerDialogMinimize;			// 메신져 대화창 축소 버튼 
	_XButton*			m_btnMessengerDialogClose;				// 메신져 대화창 닫기 버튼 
	
	_XButton*			m_btnMessengerDialogInformation;		// 메신져 대화창 상대 정보 버튼	 
	_XButton*			m_btnMessengerDialogPartyRequest;		// 메신져 대화창 동행신청 버튼	
	_XButton*			m_btnMessengerDialogFriendCut;			// 메신져 대화창 차단 버튼 	
	_XButton*			m_btnMessengerDialogChatInput;			// 메신져 대화창 메세지 입력 버튼 	
	_XButton*			m_btnMessengerDialogEmoticonInput;		// 메신져 대화창 이모티콘 입력 버튼 
	
	_XIMEContainer		m_IMEControlMessengerDialog;			// 메신져 대화창 메세지 입력창 
	
	_XList				m_MessengerChatList;					// 메신져 대화창 내용 
	
	int					m_MessengerChatListScrollPos;			
	_XVScrollBar*		m_MessengeChatListScrollBar;			// 메신져 대화창 표시창 스크롤바 
	_XGUIObject*		m_pScrbarFocusedObject;	
	
	TCHAR				m_ToCharactername[13];					// 메신저에서 선택한 친구 이름
	
	_XMESSENGERDIALOG_MODE	m_MessengerDialogMode;				// 메신져 대화창 모드( 인스턴스 메세지, 대화창 )

	BOOL				m_bReady;								// 인스턴스 메세지 창에 마우스가 올려져 있을때 
	BOOL				m_bMessageReceive;						// 인스턴스 메세지 모드일때 메세지가 도착하면  

	int					m_InstanceMessagePositionNumber;		// 인스턴스 메세지 창의 위치를 나타내는 변수 
	
	BOOL				m_bGMChatting;							// GM과 채팅인지 아닌지를 구분함.

	DiscipleInfo		m_DiscipleInfo;							//Author : 양희왕 //breif : 스승이 보는 제자 정보

public:
	_XWindow_MessengerDialog();
	virtual ~_XWindow_MessengerDialog();
	
	static _XWindow_MessengerDialog*	CreateMessengerDialogBox( _XWindowManager* pWindowManager );

	void			Create( _XWindowManager* pWindowManager, DWORD windowid );	
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			ProcessWheelMessage( short zDelta );
	void			ProcessDblClick();
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	BOOL			CheckIMEBoxArea(void);		

	void			SetMode( _XMESSENGERDIALOG_MODE messengerdialogmode );
	
	void			SetGMChatting(BOOL GMchat) { m_bGMChatting = GMchat; }
};

#endif // !defined(AFX_XWINDOW_MESSENGERDIALOG_H__0CBB024E_B76C_40B7_B9B0_370CE33C0E80__INCLUDED_)
