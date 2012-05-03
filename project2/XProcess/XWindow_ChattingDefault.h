// XWindow_ChattingDefault.h: interface for the _XWindow_ChattingDefault class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CHATTINGDEFAULT_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_)
#define AFX_XWINDOW_CHATTINGDEFAULT_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

/**
 * \brief 일반 채팅 창 - 고정
 * 해당 모드에 따른 채팅 기능
 * 
 * 감정 기능, 시스템창 on/off기능
 * 사이즈 조절 기능
 * 
*/

typedef enum _XCHATMODE
{
	_XCHATMODE_EMOTION = 0,		///< 감정
	_XCHATMODE_SHOUT,			///< 외치기
	_XCHATMODE_CLAN,			///< 방회
	_XCHATMODE_SUBCLAN,			///< 영웅단
	_XCHATMODE_PARTY,			///< 파티
	_XCHATMODE_WHISPER,			///< 귓속말
	_XCHATMODE_ZONE,			///< 지역채팅
	_XCHATMODE_NORMAL,			///< 일반
	_XCHATMODE_TOTALCOUNT
};

/// 최대 저장될 수 있는 귓속말 대상 
#define _XDEF_MAXWHISPERTARGETCOUNT		3

class _XWindow_ChattingDefault : public _XWindow  
{
public:
	_XWINDOW_RESIZEMODE m_ResizeMode;
	BOOL				m_Resizing;
	
	int					m_ChatListScrollPos;
	_XVScrollBar*		m_ChatListScrollBar;	
	_XGUIObject*		m_pScrbarFocusedObject;	
	
	int					m_MaxViewCount;
	_XCHATMODE			m_ChatMode;
	_XCHATMODE			m_PrevChatMode;
	int					m_CurrentSeletedEmotion;
	
	_XImageStatic*		m_ChatBorderLeft;
	_XImageStatic*		m_ChatBorderRight;

	_XButton*			m_btnChatFunction;
	_XImageStatic*		m_btnimageChatFunction;
//#ifdef _XDEF_CASHITEM_SHOUT
	_XButton*			m_btnChatFunctionMenuButton[_XCHATMODE_TOTALCOUNT];			///< 채팅모드 버튼
	_XImageStatic*		m_btnimageChatFunctionMenuButton[_XCHATMODE_TOTALCOUNT];	///< 채팅모드 이미지
//#else
//	_XButton*			m_btnChatFunctionMenuButton[6];			///< 채팅모드 버튼
//	_XImageStatic*		m_btnimageChatFunctionMenuButton[6];	///< 채팅모드 이미지
//#endif
	
	_XListBox*			m_EmotionListBox;
	_XButton*			m_btnChatResize;//2004.05.18->oneway48 insert
	_XButton*			m_btnMaximumMapModeExit;
	
	int					m_ChattingWindowSize;
	
	_XIMEContainer		m_WhisperEditContainer;
	_XButton*			m_WhisperStaticButton;
	TCHAR				m_WhisperTargetName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	TCHAR				m_SavedWhisperTargetName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	_XCheckButton		m_WhisperTargetButton[_XDEF_MAXWHISPERTARGETCOUNT];
	
	_XList				m_RecentChatList;
	int					m_SelectedWhisperTarget;
	
	_XCheckButton		m_DockingSystemMessageButton;		///< 시스템 메시지창 On/Off 

	BOOL				m_MovingChatWindow;
	
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

	BOOL				m_LockChatFilterMenu;
	_XCheckButton*		m_pLockChatFilterMenuButton;
	_XCheckButton*		m_pChatBlackListButton;

	_XCheckButton*		m_pChatFilterButton[4];

	_XImageStatic		m_LockButtonBackImage;
	_XImageStatic		m_LockButtonColorIndicateImage;
	
	BOOL				m_HeroBandChatMode;

#endif

	
public:
	_XWindow_ChattingDefault();
	virtual ~_XWindow_ChattingDefault();
	
	BOOL				Initialize( void );
	void				DestroyWindow( void );
	void				MoveWindow( int X, int Y );
	void				ShowWindow( BOOL show );
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void				DrawEmotion( _XGUIObject*& pfocusobject );
	void				SetDefaultPosition( void );
	void				SaveWindowProperty( void );
	
	void				SetChangeWhisperName( int changeindex ); 
	void				SetChatMode( _XCHATMODE chatmode );
	_XCHATMODE			GetChatMode( void ){ return m_ChatMode; }	
	void				ResizeWindow( int SX, int SY, int EX, int EY );
	void				ResizeWindow( RECT winrect ){ ResizeWindow(winrect.left, winrect.top, winrect.right, winrect.bottom ); }
	BOOL				ProcessWheelMessage( short zDelta );

	void				ShowChatFunctionMenu( BOOL show );  ///< 채팅모드리스트 On/Off

	void				InsertRecentWhisperUserList( LPTSTR szUsername, int target = 0 );
	void				SelectWhisperTargetTab( int index );
	
	BOOL				OnKeyDown(WPARAM wparam, LPARAM lparam);	///< 스크롤 기능 
	BOOL				CheckMousePosition(void);
	void				ResetChattingDefault(void);

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	int					RecalcChattingListLength( void );
#endif
	
	void				SetPositionDie(BOOL bDie);

};

extern void __stdcall _XIMECallback_ChatWhisperReturn( void );
extern void __stdcall _XIMECallback_ChatWhisperLeftArrow( void );
extern void __stdcall _XIMECallback_ChatWhisperRightArrow( void );

#endif // !defined(AFX_XWINDOW_CHATTINGDEFAULT_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_)