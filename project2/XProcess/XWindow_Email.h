// XWindow_Email.h: interface for the _XWindow_Email class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EMAIL_H__46FF769C_400F_4CD5_A6A8_C5B2DDDD621A__INCLUDED_)
#define AFX_XWINDOW_EMAIL_H__46FF769C_400F_4CD5_A6A8_C5B2DDDD621A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum
{
	_XEMAILTAB_RECEIVE		= 0,
		_XEMAILTAB_SEND,
		_XEMAILTAB_KEEP
		
}_XEmailTab;

typedef struct {
	UINT			uiIndex ;			// 메일 인덱스
	bool			check ;				// false : 읽음 , true : 새로도착(안읽음)	
}_XEMAILLISTTABLE;


struct _XEMAILINFO {
	UINT			uiIndex ;			// 메일 인덱스
	char			cFrom[13] ;			// 보낸사람
	struct _Date {						// 받은시간
		unsigned	year : 12 ;				
		unsigned	month : 4 ;
		unsigned	day : 5 ;
		unsigned	hour : 5 ;
		unsigned	minute : 6 ;
	} date;	// 4Byte
	char			cStrLen ;
	char			cText[101] ;		// 내용
	char			cMode ;
	enum
	{
		_PRIVATE,
		_PUBLIC
	};
};
#define _XDEF_MAX_EMAILLENGTH		100		// 최대 전서구 길이

class _XWindow_Email   : public _XWindow  
{
	
public:
	
	_XImageStatic*		m_imageEmailTitleBar;					// 전서구 타이틀 바
	_XImageStatic*		m_imageEmailDisplayBoxUpBorder;			// 전서구 내용표시 박스 위쪽 바탕 
	_XImageStatic*		m_imageEmailDisplayBoxDownBorder;		// 전서구 내용표시 박스 아래쪽 바탕 
	_XImageStatic*		m_imageEmailDisplayBoxLeftSideBorder;	// 전서구 왼쪽 사이드 이미지
	_XImageStatic*		m_imageEmailDisplayBoxRightSideBorder;	// 전서구 오른쪽 사이드 이미지
	_XImageStatic*		m_imageEmailDownBorder;					// 전서구 창 아래쪽  바탕
	_XImageStatic*		m_imageEmailDockingBorder;				// 전서구 메인 메뉴창에 도킹했을때 생성되는 바탕
	_XImageStatic*		m_imageEmailDownEndLine;				// 전서구 창 아래쪽 끝 라인 
	_XImageStatic*		m_imageEmailCountBoxBorder;				// 전서구 보관된 편지 표시 창 박스  


	// 전서구 관련 변수
	_XEmailTab			m_SelectedTab;
	_XCheckButton*		m_TabButtonKeepingBox;					// 전서구 탭버튼 보관함
	_XCheckButton*		m_TabButtonReceiveBox;					// 전서구 탭버튼 수신함
	_XCheckButton*		m_TabButtonSendBox;						// 전서구 탭버튼 발신함
	
	_XButton*			m_btnEmailClose;						// 전서구 창 닫기 버튼
	_XButton*			m_btnEmailHelp;							// 전서구 창 도움 버튼
	_XButton*			m_btnNext;								// 저장된 편지 다음 보기 버튼 
	_XButton*			m_btnPrev;								// 저장된 편지 이전 보기 버튼
	_XButton*			m_btnKeepingMailDelete;					// 저장된 편지 삭제하기 버튼
	_XButton*			m_btnReceiveReply;						// 받은 편지 답장하기 버튼
	_XButton*			m_btnReceiveSave;						// 받은 편지 보관하기 버튼
	_XButton*			m_btnReceiveDelete;						// 받은 편지 삭제하기 버튼
	_XButton*			m_btnSendDelete;						// 보낼 편지 내용 지우기 버튼
	_XButton*			m_btnSendCancle;						// 보낼 편지 취소 버튼
	_XButton*			m_btnSend;								// 보낼 편지 보내기 버튼
	
	
	_XIMEContainer		m_EmailIMEControl;
	TCHAR				m_EmailDisplayText[256][128];
	TCHAR				m_EmailText[101];
	int					m_EmailTextLineTotalCount;
	int					m_EmailTextCurrentLinePos;
	int					m_EmailTextLength;
	
	_XIMEContainer		m_ReceiverIMEControl;
	TCHAR				m_EmailReceiver[13];
	TCHAR				m_EmailSender[20];
	
	int					m_EmailTextScrollPos;
	_XVScrollBar*		m_EmailTextScrollBar;	
	_XGUIObject*		m_pScrbarFocusedObject;	
	
	_XEMAILINFO			m_TempEmailInfo[20];
	_XEMAILLISTTABLE	m_EmailListReceiveBox[20];
	_XEMAILLISTTABLE	m_EmailList[20];
	int					m_EmailTextTotalCount;
	int					m_indexEmailText;
	int					m_EmailReceiveTextTotalCount;
	int					m_indexEmailReceiveText;
	int					m_EmailYear;				
	int					m_EmailMonth;
	int					m_EmailDay;
	int					m_EmailHour;
	int					m_EmailMinute;
	unsigned long		m_EmailColor;

	BOOL				m_bDockingMode;
	

public:
	_XWindow_Email();
	virtual ~_XWindow_Email();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			ProcessWheelMessage( short zDelta );
	void			ProcessDrag( _XGUIObject*& pfocusobject );
	
	void			ResizeWindow( RECT winrect ){ ResizeWindow(winrect.left, winrect.top, winrect.right, winrect.bottom ); }
	void			ResizeWindow( int left, int top, int right, int bottom );

	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	
	void			ResetEmailList();
	void			ResetEmailText();
	void			ResetTempEmailInfo();
	void			ResetEmailListReceiveBox();
	
	void			SetEmailText( LPSTR emailtext );

	void			DeleteEmailListReceiveBox(int emailindex);
	void			DeleteEmailList( int emailindex );
	
	void			CalculateEmailLength();
	
	void			ChangeTab(_XEmailTab selectedtab);
	
	BOOL			CheckIMEBoxArea();
	BOOL			CheckReceiverIMEBoxArea();
	BOOL			CheckEmailScrollBarArea();
	void			SetDockingMode(BOOL flag);


};

#endif // !defined(AFX_XWINDOW_EMAIL_H__46FF769C_400F_4CD5_A6A8_C5B2DDDD621A__INCLUDED_)
