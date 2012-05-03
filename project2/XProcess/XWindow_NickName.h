// XWindow_NickName.h: interface for the _XWindow_NickName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NICKNAME_H__E8822EE2_93C9_47EC_8824_06CCA1D95C1B__INCLUDED_)
#define AFX_XWINDOW_NICKNAME_H__E8822EE2_93C9_47EC_8824_06CCA1D95C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 별호 창
 * 
 * 별호리스트 디스플레이
 * 별호, 은거 선택 
*/


class _XWindow_NickName  : public _XWindow
{
public:
	_XWindow_NickName();
	virtual ~_XWindow_NickName();
	
public:
	_XStatic				m_BorderTile;
	_XImageStatic			m_BorderGradationTitle;

	_XButton*				m_ApplyButton;
	_XButton*				m_OKButton;
	_XButton*				m_DeleteButton;
	
	_XListBox*				m_listboxSpecialNickName ;		///< 특수 별호	
	_XListBox*				m_listboxUniqueNickName ;		///< 고유 별호 
	_XListBox*				m_listboxGeneralNickName ;		///< 일반 별호	
	_XListBox*				m_listboxInfoNickName;			///< 선택된 별호 설명, 효과

	int						m_SelectedNickNameSlotNumber;	///< 선택된 별호
	int						m_listboxNickIndex[213];	///< 별호 인덱스 공간 
	int						m_RemainChangeTime;				///< 별호 재선택 남은 시간
	
public:
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void MoveWindow( int X, int Y );
	
	void ShowWindow( BOOL show );
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );

	void SetNickNameList( void );		///< 별호 리스트 셋팅
	void SetSelectedNickNameInfo( void );	///< 현재 선택된 별호의 정보
	void ShortenedNickName( TCHAR* comNick, TCHAR* srcNick );	///< 긴 별호명 축소
	
	void ResetNicknameInfo(void);		///< 별호 정보창 리셋
	void CheckDeleteNickname(void);		///< 지울 수 있는 별호인지 검사	
};

#endif // !defined(AFX_XWINDOW_NICKNAME_H__E8822EE2_93C9_47EC_8824_06CCA1D95C1B__INCLUDED_)
