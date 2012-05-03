// XWindow_CInfo.h: interface for the _XWindow_CInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_CINFO_H_
#define _XWINDOW_CINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

/**
 * \brief 주인공 인물 정보 창 - 고정
 * 주인공의 장착중인 아이템, 능력치, 기본정보 등등을 보여줌
 * 
*/

/// 장착슬롯의 타입
typedef enum tagXEQUIPEDITEMEDGEICONTYPE
{
	_XEQUIPEDITEMEDGEICONTYPE_EMPTY = 0,
	_XEQUIPEDITEMEDGEICONTYPE_WEAPON,
	_XEQUIPEDITEMEDGEICONTYPE_CLOTHES,	
	_XEQUIPEDITEMEDGEICONTYPE_ACCESSORY,
	_XEQUIPEDITEMEDGEICONTYPE_DISABLESLOT,	
} _XEQUIPEDITEMEDGEICONTYPE;

class _XWindow_CInfo : public _XWindow  
{
public:

	_XImageStatic			m_EquipedItemEdgeIcon[5];	///< 타입에 따른 이미지
	_XImageStatic			m_PocketImageStatic;
	
	_XButton*				m_ModifyButton[5];			///< 능력치 상승 버튼 
	
	_XStickGauge*			m_pGiBorder;	///< 기 게이지
	_XStickGauge*			m_pFuryBorder;	///< 분노 게이지

	_XCheckButton*			m_pNickNameTabbutton[3];	///< 현재 별호버튼만 쓰임

	int						m_SelectedSlotIndex;
	int						m_LastDraggingSlot;

	// Drag
	BOOL					m_bDragState;
	POINT					m_IconDragStartPos;
	
	POINT					m_PointUpArea[5];
	_XListBox*				m_FavoritesNickNameList;	///< 최근 선택한 별호 리스트
	int						m_FavoritesNickNameIndexList[10]; ///< 최근 선택한 별호 인덱스 리스트 최대 10개
	
	int						m_ScrollFactor;
	_XVScrollBar*			m_pListScrollBar;

public:
	_XWindow_CInfo();
	virtual ~_XWindow_CInfo();
	
	BOOL 		Initialize( void );
	
	void 		MoveWindow( int X, int Y );

	void 		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void 		SetDefaultPosition( void );
	void		SaveWindowProperty( void );

	void		ShowWindow(BOOL show);
	void		SetParameter( void );
	
	void		DrawEquippedItemList(void);		///< 장착된 아이템들 그리기
	void		DrawIcon( BOOL smallicon, int X, int Y, _XUserItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = FALSE );
	void		DrawSelectedItem( void );
	int			SearchSlotRegion( RECT* pRect = NULL );	
	BOOL		CheckEquipedArea( void );		///< 장착공간인지 체크
	void		ProcessInvenMove(void);			///< 인벤->장착창
	void		ProcessDblClick(void);			
	BOOL		ProcessWheelMessage( short zDelta );
	void		InsertNickNameIndex(int index);	///< 해당 별호를 즐겨찾기 별호리스트에 추가
	void		SetFavoritesNickNameList(void);	///< 즐겨찾기 별호리스트 재정렬( 문자길이가 긴 것 축소 )
	
	void		DrawFameTooltip(int select, int posx, int posy); ///< 명성, 악명 단계를 나타내 주는 툴팁( 1단계 이상일 때에만 표시해줌)
};

#endif // !defined(AFX_XWINDOW_CINFO_H__554BEFE1_CE9A_49A3_B01E_BDF6A1DB70F9__INCLUDED_)
