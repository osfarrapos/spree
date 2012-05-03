// XWindow_Event.h: interface for the _XWindow_Event class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EVENT_H__175FBFE1_1BDA_40C0_A3BE_D44BEECDFBB1__INCLUDED_)
#define AFX_XWINDOW_EVENT_H__175FBFE1_1BDA_40C0_A3BE_D44BEECDFBB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "Xwindow.h"

typedef struct XEventInfo
{
	int		eventid;
	int		eventtype;
	int		startyear;
	int		startmonth;
	int		startday;
	int		endyear;
	int		endmonth;
	int		endday;
	int		nStartTime; //타입 6 에서만
	int		nEndTime; //타입 6 에서만
	int		serialslot[5];
	TCHAR	eventname[512];
	TCHAR	eventdes[512];

	int		nPackageIndex; //Author : 양희왕 //breif : 레벨 선물 이벤트 //타입 6일때,, 버프 아이디
	int		nLevel; //Author : 양희왕 //breif : 레벨 선물 이벤트 // 타입 6 일때 버프 값,,
}_XEvnetInfo;

class _XWindow_Event : public _XWindow  
{
public:
	_XWindow_Event();
	virtual ~_XWindow_Event();
	
public:
	_XStatic				m_BorderTile;
	_XImageStatic			m_BorderGradationTitle;
	
	int						m_SelectedEventID;
	int						m_SelectedEventType;
	int						m_SelectedMaxSerialNumber[5];

	TCHAR					m_SelectedEventName[512];	// 선택된 이벤트 이름
	TCHAR					m_SelectedEventTerm[512];	// 선택된 이벤트 기간
	TCHAR					m_SerialNumber[5][8];		// 시리얼 넘버를 표시하는 문자열 배열
	
	_XImageStatic*			m_imageIMEBorderSerialNumber;	// 입력창 바탕 

	_XListBox*				m_listboxEventTitle;		// 이벤트 목록을 보여주는 리스트 박스
	_XListBox*				m_listboxEventDescription;	// 이벤트 설명을 보여주는 리스트 박스
	
	_XButton*				m_btnSerialNumberOK;		// 이벤트 시리얼 입력 완료 버튼 
	

	_XIMEContainer			m_IMESerialNumber;			// 이벤트 시리얼 번호 입력창

	list <_XEvnetInfo*>		m_listEventInfo;			// 이벤트의 모든 정보를 가지는 리스트 

	int						m_nEventIDinfo; //Author : 양희왕 //breif 레벨 선물 이벤트 하나만 뜨게 한다 //Date :08/03/19
	BOOL					m_bBuffEvent; //Author : 양희왕 //breif : 타입 6 번, 버프 날짜가 되면 아이콘을 뜨게 한다
	BOOL					m_bFirstSetListItem; //Author : 양희왕 //breif : 리스트목록을 처음 갱신했다..
private :
	_XButton*				m_bGetItem;		// 받기 버튼 //Author : 양희왕 //breif : 구룡천하 7일권에만 우선 적용

	struct EventResult
	{
		int	nEventID;
		BOOL bGiveItem;
	};

	EventResult m_EventInfo[8];
	
public:
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL ProcessWheelMessage( short zDelta );
	
	void MoveWindow( int X, int Y );	
	void ShowWindow( BOOL show );
	
	void SetListItem(void);
	void SetSelectedListItme( int selectedeventid );
	
	BOOL LoadEventInfo(void);

	BOOL CheckSerialNumber( int serialslotnumber );	// 시리얼 번호 개수가 맞는지를 확인한다.
	
	BOOL CheckSerialNumberBox1(void);
	BOOL CheckSerialNumberBox2(void);
	BOOL CheckSerialNumberBox3(void);
	BOOL CheckSerialNumberBox4(void);
	BOOL CheckSerialNumberBox5(void);

	void SetEventResult( int nArray, int nID, BOOL bRes )
	{
		m_EventInfo[nArray].nEventID = nID;
		m_EventInfo[nArray].bGiveItem = bRes;
	}
	void SetEventResult( int nID, BOOL bRes )
	{
		for( int i = 0; i < 8; i++ )
		{
			if( m_EventInfo[i].nEventID == nID )
			{
				m_EventInfo[i].bGiveItem = bRes;
				break;
			}
		}
	}
	const BOOL GetCurrentEventStatus() const// 진행중인 이벤트 중 못받은게 있다면
	{
		for( int i = 0; i < 8; i++ )
		{
			if( m_EventInfo[i].nEventID > 0 && !m_EventInfo[i].bGiveItem )
				return TRUE;
		}

		return FALSE;
	}

private :
	int GetTotalCurrentDay( int nYear, int nMonth, int nDay ); //Author : 양희왕 //breif : 날짜 계산..
};

#endif // !defined(AFX_XWINDOW_EVENT_H__175FBFE1_1BDA_40C0_A3BE_D44BEECDFBB1__INCLUDED_)
