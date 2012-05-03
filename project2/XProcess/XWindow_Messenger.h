// XWindow_Messenger.h: interface for the _XWindow_Messenger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MESSENGER_H__4DA2D6F1_1226_4E91_8514_F66BA44ACA8D__INCLUDED_)
#define AFX_XWINDOW_MESSENGER_H__4DA2D6F1_1226_4E91_8514_F66BA44ACA8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XList.h"
#include "XMessengerGroup.h"
#include "XMessengerFriend.h"


class _XWindow_Messenger : public _XWindow  
{
public:	
	_XImageStatic*		m_imageMessengerTitleBar;			// 메신져 타이틀 바	
	_XImageStatic*		m_imageMessengerStateBorder;		// 메신져 상태 전환, 로그인 로그아웃 바탕	
	_XImageStatic*		m_imageMessengerLeftSideBorder;		// 메신져 왼쪽 사이드 이미지
	_XImageStatic*		m_imageMessengerRightSideBorder;	// 메신져 오른쪽 사이드 이미지
	_XImageStatic*		m_imageMessengerDownBorder;			// 메신져 창 아래쪽  바탕
	_XImageStatic*		m_imageMessengerDockingBorder;		// 메신져가 메인 메뉴창에 도킹했을때 생성되는 바탕
	_XImageStatic*		m_imageMessengerDownEndLine;		// 메신져 창 아래쪽 끝 라인 
	
	_XButton*			m_btnMessengerClose;				// 메신져 닫기 버튼 
	_XButton*			m_btnMessengerHelp;					// 메신져 도움 버튼 
	_XButton*			m_btnMessengerLogIn;				// 메신져 로그인 버튼	 
	_XButton*			m_btnMessengerLogOut;				// 메신져 로그아웃 버튼	
	_XButton*			m_btnMessengerAddFriend;			// 메신져 친구추가 버튼 	
	_XButton*			m_btnMessengerAddFriendGroup;		// 메신져 그룹추가 버튼 
	_XButton*			m_btnChangeStatus;					//메신져 상태 전환 버튼 
	_XButton*			m_btnMessengerRecallSystem;			// 메신져 소환 버튼
	_XButton*			m_btnMessengerMnDSystem;			// 메신져 사제대화 버튼

	// 메신져 드래깅 관련 변수 
	_XToolTip*			m_DragingDisplayBox;				// 메신져 리스트에서 드래깅 되는 친구 이름이나 그룹 이름을 표시함

	// 메신져 친구 리스트 관련 변수들 
	_XListBox*			m_listboxGroupList ;				//메신져 그룹과 친구리스트를 표시하는 리스트 박스 
	_XList				m_MessengerGroupList;
		
	int					m_IsDragingCharacterNumber;
	int					m_IsDragingGroupNumber;
	
	int					m_CurrentUserStatus;				// 친구의 상태를 알 수 있는 변수 
	int					m_TotalCountFriends;				// 친구 개수를 count : 최대 50명 
	
	int					m_SelectedItemIndex;
	// 
	int					m_MaxViewCount;
	_XWINDOW_RESIZEMODE m_ResizeMode;

	BOOL				m_bDragingDisplayBox;
	BOOL				m_bDrawDragingDisplayBox;
	BOOL				m_Resizing;
	BOOL				m_bDockingMode;
	BOOL				m_bSelectedGroup;
	BOOL				m_bClickRightButton;
	BOOL				m_bClickLeftButton; 
	BOOL				m_bDblClick;						//더블클릭을 한 상태를 알려준다.
	
	float				m_DragStartPosX;					// 마우스 드래그시  다른 쪽에서 클릭해서 
	float				m_DragStartPosY;	

	float				m_ClickRightStartPosX;				// 마우스 오른쪽 클릭시  다른 쪽에서 클릭해서 
	float				m_ClickRightStartPosY;				//움직여서 왔을 때는 툴팁이 뜨지 않게 하게 위한 변수

	TCHAR				m_IsDragingText[256];
	TCHAR				m_IsDragingCharacterName[13];		// 드래그를 시작한 위치에 있는 친구 이름
	TCHAR				m_IsDragingGroupName[15];			// 드래그를 시작한 위치에 있는 친구를 포함하고 있는 그룹 이름 
	// 메신져 친구추가, 그룹추가 관련 변수 
	TCHAR				m_CharacterName[15];				// 친구 추가시 메신저에서 선택된 친구의 이름을 저장한다.
	TCHAR				m_GroupName[15];					// 새로운 그룹생성시 사용,그룹 이름 변경시 결과를 받을 때까지 바꾸기 전 이름을 저장하는 공간
	TCHAR				m_GroupReName[15];					// 그룹 이름 변경시 결과를 받을 때까지 바뀔 이름을 저장하는 공간
	
	TCHAR				m_SelectedCharactername[13];		//메신저에서 선택한 친구 이름
	TCHAR				m_SelectedGroupname[15];			//메신저에서 선택한 그룹 이름
	
	
	
	
public:

	_XWindow_Messenger();
	virtual ~_XWindow_Messenger();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	void MoveWindow( int X, int Y );	
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	void ShowWindow( BOOL show ); 
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL ProcessWheelMessage( short zDelta );
	void ProcessResizeAndDrag( _XGUIObject*& pfocusobject );
	void ProcessSelectedListboxLeftButton();
	void ProcessSelectedListboxRightButton();	
	void ProcessMoveListbox();
	void ProcessSelectMovingGroup(); 
	void ProcessDblClick();
	
	void ReleaseMessengerList();
	
	void ResizeWindow( RECT winrect ){ ResizeWindow(winrect.left, winrect.top, winrect.right, winrect.bottom ); }
	void ResizeWindow( int left, int top, int right, int bottom );
	
	void SetListItem();
	int	 SearchListNumber();
	
	void SortGroupName();
	void SortFriendName();
	
	BOOL CheckGroupListbox();
	void SetDockingMode(BOOL flag);
};

#endif // !defined(AFX_XWINDOW_MESSENGER_H__4DA2D6F1_1226_4E91_8514_F66BA44ACA8D__INCLUDED_)

