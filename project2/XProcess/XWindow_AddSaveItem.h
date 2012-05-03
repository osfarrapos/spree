// XWindow_AddSaveItem.h: interface for the _XWindow_AddSaveItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ADDSAVEITEM_H__B491BE9D_2FFF_4C9C_B99D_D8A6BEC3867C__INCLUDED_)
#define AFX_XWINDOW_ADDSAVEITEM_H__B491BE9D_2FFF_4C9C_B99D_D8A6BEC3867C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

/**
 * \brief 창고 계약 창
 * 창고의 공간과 기간 등을 선택하는 클래스
 *
 * 창고사용 중 재계약 할때 창고의 아이템 마지막 인덱스로 재계약 창고의 공간을
 * 결정해야 한다.(마지막 창고 인덱스는 서버에서 보내줌)
*/

class _XWindow_AddSaveItem;

typedef void (_XWindow_AddSaveItem::*pFuncProcess)();
typedef void (_XWindow_AddSaveItem::*pFuncDraw)();

class _XWindow_AddSaveItem : public _XWindow  
{
public:	
	
	_XButton*		m_btnSeleted;
	_XButton*		m_btnCancel;
	
	_XCheckButton*	m_checkbtnList[9]; //2004.05.20->oneway48 modify
	
	//	_XListBox*		m_listboxAddSaveItem; //2004.05.20->oneway48 delete
	
	int				m_SelectedItemIndex;	
	
	int				m_AddStorageSize;		///< 추가하는 칸의 크기	
	int				m_AddStorageTerm;		///< 추가하는 기간	
	int				m_AddStorageCost;		///< 추가하는데 드는 비용

private :
	//Auhtor : 양희왕 //breif : 창고 확장  //보기 불편해
	_XCheckButton* m_pTabButton[2]; //Author : 양희왕 //breif : 창고 이미지 버튼으로 변경

	pFuncProcess m_CurrnetProcess;
	pFuncDraw m_CurrentDraw;
	int m_nSelectTab; //현재 선택한 탭.. 0 을 기준

    _XImageStatic* m_pUseImageBoder[6]; // 국내는 하나씩 쓰니깐 3 다른나라는 2개씩 사용해서 6

	void SetProcess( void(_XWindow_AddSaveItem::*pFunc)() ) { m_CurrnetProcess = pFunc;	}
	void SetDraw(void(_XWindow_AddSaveItem::*pFunc)() )	{ m_CurrentDraw = pFunc; }
	
public:
	void Reset();
	int GetListIndex();
	_XWindow_AddSaveItem();
	virtual ~_XWindow_AddSaveItem();
	
	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void				ShowWindow(BOOL show);
	
	/// 체크 버튼 클릭 처리
	void				SetSelectedIndex( int index );
	/// 계약할 수 있는 공간의 창고 검사
	void				CheckUseSlot(void);
	//	void				SetListItem();

	//Auhtor : 양희왕 //breif : 창고 확장 
	void OnFirstTabProcess();
	void OnSecondTabProcess();
	void OnFirstDraw();
	void OnSecondDraw();

	void SetSelectTab( const int nNumber );
	const int GetSelectTab() const { return m_nSelectTab; }
	const int GetReserveType() const { return m_SelectedItemIndex; }
};

#endif // !defined(AFX_XWINDOW_ADDSAVEITEM_H__B491BE9D_2FFF_4C9C_B99D_D8A6BEC3867C__INCLUDED_)
