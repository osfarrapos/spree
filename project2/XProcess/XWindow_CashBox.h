// XWindow_CashBox.h: interface for the _XWindow_CashBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASHBOX_H__A29E1817_1391_4878_A1B0_A2C7868D617E__INCLUDED_)
#define AFX_XWINDOW_CASHBOX_H__A29E1817_1391_4878_A1B0_A2C7868D617E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

// 상용 아이템 보관함 아이템 정보
typedef struct 
{
	int		indexBox;		// 보관함 인덱스 
	int		OrderNumber;	// 주문번호
	int		ProductNumber;	// 상품번호
	int		indexPackage;	// 패키지 인덱스
	BOOL	bCheck;			// 선택 여부 
	
	char		PresentName[en_max_lil+1];	// 선물한 유저의 Account
	SYSTEMTIME	PresentTime;				// 선물 받은 날짜(년/월/일/시/분/초); DBTIMESTAMP
	char		PresentMsg[100+1];			// 선물 메시지
}_XCASHITEMBOX_INFO;

typedef enum
{
	_XCASHBOXTAB_PERSONAL		= 0,
		_XCASHBOXTAB_PRESENT,
		_XCASHBOXTAB_TOTALCOUNT
}_XCashBoxTab;

class _XWindow_CashBox : public _XWindow  
{
public:
	_XCashBoxTab	m_SelectedTab;
	
	_XButton*		m_btnMoveSelectedItem;	//선택옮김
	_XButton*		m_btnMoveAllItem;		//전부옮김
	_XButton*		m_btnRenew;		//전부옮김

	_XButton*		m_btnFirst;				//처음으로 
	_XButton*		m_btnEnd;				//끝으로
	_XButton*		m_btnAdjacent;			//이전
	_XButton*		m_btnNext;				//다음
	_XButton*		m_btnMsgClose;			//메세지 닫기
	_XButton*		m_btnPage[5];			//페이지 모음 //Author : 양희왕
	_XButton*		m_btnPage1;				//페이지1
	_XButton*		m_btnPage2;				//페이지2
	_XButton*		m_btnPage3;				//페이지3
	_XButton*		m_btnPage4;				//페이지4
	_XButton*		m_btnPage5;				//페이지5
	
	_XButton*		m_btnMessage[9];		//메세지보기 버튼1
	
	_XCheckButton*	m_TabButton[ _XCASHBOXTAB_TOTALCOUNT ]; // 개인 보관함 , 선물함 
	_XCheckButton*	m_Common_TabButton[ _XCASHBOXTAB_TOTALCOUNT ]; // 개인 보관함 , 선물함 
	_XListBox*		m_listboxCashItem;	// 아이템 몰에서 구입한 캐쉬 아이템
	
	int				m_nCashItemTotalCount;
	int				m_PresentMessageBoxPosition;
	int				m_SendOrderNumberPresentMessage;
	int				m_Maxium5PageCount;
	int				m_Current5PageCount;
	int				m_CurrentPageNumber;
	BOOL			m_bShowPresentMessage;
	
	TCHAR			m_PresentName[128];
	TCHAR			m_PresentTime[128];
	TCHAR			m_PresentMessage[128];

	list <_XCASHITEMBOX_INFO*>		m_listCashItem;		// 상용 아이템 정보 리스트 

public:
	_XWindow_CashBox();
	virtual ~_XWindow_CashBox();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	void			SetListItem(void);
	void			GetIconRect(char cType, int iconnumber, int& imageindex, RECT& rect, BOOL smallicon);
	void			ChangeTab(_XCashBoxTab selectedtab);
	
	void			SetPageNumberButton( void );
	void			SetPresentMessage( int OrderNumber );
	void			SendPresentMessageReq( int MsgNumber );
};

#endif // !defined(AFX_XWINDOW_CASHBOX_H__A29E1817_1391_4878_A1B0_A2C7868D617E__INCLUDED_)
