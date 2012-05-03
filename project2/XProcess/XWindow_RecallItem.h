// XWindow_RecallItem.h: interface for the XWindow_RecallItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_RECALLITEM_H__28870DE7_4EF7_4898_83AF_71E9DF529334__INCLUDED_)
#define AFX_XWINDOW_RECALLITEM_H__28870DE7_4EF7_4898_83AF_71E9DF529334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XWindow_RecallItem : public _XWindow
{
public:
	_XWindow_RecallItem();
	virtual ~_XWindow_RecallItem();

	BOOL	Initialize( void );
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	Update(); //윈도우와는 별개로 Process 진행

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	BOOL	SearchSlotItem( int nIndex ); //Author : 양희왕 //breif : 친구소환 아이템 있는지 검사
	BOOL	CharRecallCondition();
	BOOL	CheckCharacterName( LPCTSTR pString );
	
	LPCTSTR	GetCharacterName( int nIndex ); //지정된 스트링을 리턴..
	
	void	SetMovingVariable( MSG_RECALL_FRIEND* pRecallresult, BOOL bMoving, int nEffectID );
	void	SetCharState( int nState );
	void	SetSelectedItem(int nParam);
	void	SetRecallIndex(int nIndex );
	void	SetFromName( LPTSTR pString );
	void	SetRecallButton( BOOL bEnable );

	LPCTSTR		GetFromName();
	const int	GetRecallIndex() const;
	const int	GetCharState() const; //No일때 패킷보낼때만 사용..
	const int	GetItemIndex() const;
	const int	GetSelectedItem() const;

private :
	void SetListItem();

private :
	// 메신져 친구 리스트 관련 변수들 
	_XListBox*			m_listboxGroupList ;				//메신져 그룹과 친구리스트를 표시하는 리스트 박스 
	int m_nRecallItemIndex; //리콜아이템 인벤토리 위치
	int m_nSelectedItem; //선택된 리스트 번호
	int m_nCallIndex; //실제 리콜할 리스트 
	int m_nCharState; // 0 일땐 거절.. 1일땐 다른이유가 있음.. //거절 할때만 쓰인다
	TCHAR	m_strRecallFromName[64];

	//더러워지는 소스들..
	MSG_RECALL_FRIEND m_Recallresult;
	BOOL	m_bMoving; //이동할껀지..
	int m_nEffectID; //이펙트 아이디


	_XButton* m_pCallButton; //콜 버튼
	_XButton* m_pMoveButton; //이동버튼
};




//inline------------------------------------------------------------------------------------------------------------------
inline void _XWindow_RecallItem::SetRecallButton( BOOL bEnable )
{
	//_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	//친구 한명을 소환중에는 소환을 못하게 해야 함
	if( bEnable )
	{
		//if(SearchSlotItem( 2801 ))
		//	pMessenger_Window->m_btnMessengerRecallSystem->EnableWindow(bEnable);
		//else
		//	pMessenger_Window->m_btnMessengerRecallSystem->EnableWindow(FALSE);
		m_pCallButton->EnableWindow( bEnable );
		m_pMoveButton->EnableWindow( bEnable );
	}
	else
	{
		//		pMessenger_Window->m_btnMessengerRecallSystem->EnableWindow(bEnable);
		m_pCallButton->EnableWindow( bEnable );
		m_pMoveButton->EnableWindow( bEnable );
	}
}

inline void	_XWindow_RecallItem::SetMovingVariable( MSG_RECALL_FRIEND* pRecallresult, BOOL bMoving, int nEffectID )
{
	m_Recallresult = *pRecallresult;
	m_bMoving = bMoving;
	m_nEffectID = nEffectID;
}
inline void _XWindow_RecallItem::SetFromName( LPTSTR pString )
{
	memset( m_strRecallFromName, 0, sizeof(m_strRecallFromName));
	strcpy( m_strRecallFromName, pString );
}

inline void _XWindow_RecallItem::SetRecallIndex( int nIndex )
{
	m_nCallIndex = nIndex;
}

inline void _XWindow_RecallItem::SetCharState( int nState )
{
	m_nCharState = nState;
}

inline void _XWindow_RecallItem::SetSelectedItem( int nParam )
{
	m_nSelectedItem = nParam;
}

inline LPCTSTR _XWindow_RecallItem::GetFromName()
{
	return &m_strRecallFromName[0];
}

inline const int _XWindow_RecallItem::GetRecallIndex() const
{
	return m_nCallIndex;
}

inline const int _XWindow_RecallItem::GetCharState() const
{
	return m_nCharState;
}

inline const int _XWindow_RecallItem::GetItemIndex() const
{
	return m_nRecallItemIndex;
}

inline const int _XWindow_RecallItem::GetSelectedItem() const
{
	return m_nSelectedItem;
}

#endif // !defined(AFX_XWINDOW_RECALLITEM_H__28870DE7_4EF7_4898_83AF_71E9DF529334__INCLUDED_)
