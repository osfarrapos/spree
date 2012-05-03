// XWindow_SocketDetach.h: interface for the XWindow_SocketDetach class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SOCKETDETACH_H__FCA41503_7054_467F_99D1_37918B54EF4B__INCLUDED_)
#define AFX_XWINDOW_SOCKETDETACH_H__FCA41503_7054_467F_99D1_37918B54EF4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

/// 최대 소켓 공간 갯수
#define _XDEF_SOCKETITEM_MAXCOUNT 4

enum SocketDetachMode
{
	MODE_NONE,
	MODE_READY
};

class _XWindow_SocketDetach : public _XWindow
{
public:
	_XWindow_SocketDetach();
	virtual ~_XWindow_SocketDetach();

	BOOL Initialize();
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void ShowWindow(BOOL show);
	void MoveWindow( int X, int Y );

	void ReBuild( SocketDetachMode Mode );

	void (_XWindow_SocketDetach::*m_FuncProcessPtr)();
	void Process_None();

	int VacancySlot(); //빈슬롯


	void SetIndependentItem( _XUserItem& Item );
	void SetIndependentItemSlot( int nIndex );
	void SetDetachItemSlot( int nIndex );
	void SetSocketItem( int nIndex, short sSocket );
	void SetEnableProcessButton( BOOL bEnable );
	void SetLock( BOOL bLock );

	const BOOL GetLock() const;
	const int GetIndependentItemSlot() const;
	const int GetDetachItemSlot() const;
	const int GetSocketIndex() const;
	BOOL GetHaveMouseSlot() const;
	_XUserItem* GetIndependentItem();

private:
	void DrawSocketPlugInionItem();
	int GetSocketModelIndex();

	int GetSlotIndex();
	void DrawDragIcon();
	void DrawSelectedItem();

	void SetListItem(int nIndex);

private:
	FLOAT m_SocketRotateAngle;
	D3DXMATRIX m_SocketRHWMatrix;
	_X3PCamera m_SocketModelViewCamera;
	D3DVIEWPORT9 m_SocketModelViewPort;	

	_XListBox* m_pSocketitemListBox;

	_XButton* m_pProcessButton;

	BOOL m_bWindowLock;

	/** 리스트박스에 들어갈 내용을 담는 임시 텍스트 버퍼 사이즈는 _XDEF_SOCKETITEM_MAXCOUNT*2 임
	* 현재 소켓 아이템의 속성이 소켓 아이템 한개당 최대 2개가 들어가지만 만약 늘어날 경우 늘려줘야 함
	*/	
	TCHAR m_ListBoxTextList[8][255];	//STL 방식으로 변경,,

	BOOL m_bHaveMouseSlot;
	_XMouseSlotItem	m_TempSaveSlotItem;		///< 대상 아이템이 변경되었을 때 필요한 임시 저장 공간
	_XMouseSlotItem	m_MouseSlotItem;


	_XUserItem* m_IndependentItem;		///< 대상 아이템 (무기, 의상)
	int m_nIndependentSlot; // 대상아이템 슬롯 위치
	int m_nSocketLocation; // 소켓이 인벤토리 에 들어갈 위치 //빈슬롯인데 임시로,, 
	int m_nDeatchIndex; //제거도구 위치
	int m_nSocketIndex; //제거할 소켓의 인덱스

	_XUserItem m_SocketItemList[_XDEF_SOCKETITEM_MAXCOUNT];	///< 소켓 아이템 공간
	POINT m_SlotPosition[_XDEF_SOCKETITEM_MAXCOUNT];		///< 소켓 아이템 공간 위치
	
};






inline void _XWindow_SocketDetach::SetLock( BOOL bLock )
{
	m_bWindowLock = bLock;
}

inline void _XWindow_SocketDetach::SetDetachItemSlot( int nIndex )
{
	m_nDeatchIndex = nIndex;
}

inline void _XWindow_SocketDetach::SetEnableProcessButton( BOOL bEnable )
{
	m_pProcessButton->EnableWindow( bEnable );
}

inline void _XWindow_SocketDetach::SetSocketItem( int nIndex, short sSocket )
{
	m_SocketItemList[nIndex].Reset();
	m_SocketItemList[nIndex].Set_m_cType(_XGI_FC_SOCKET);
	m_SocketItemList[nIndex].Set_m_sID(sSocket);
}

inline void _XWindow_SocketDetach::SetIndependentItemSlot( int nIndex ) 
{
	m_nIndependentSlot = nIndex;
}

inline void _XWindow_SocketDetach::SetIndependentItem( _XUserItem& Item )
{
	m_IndependentItem = &Item;
}

inline BOOL _XWindow_SocketDetach::GetHaveMouseSlot() const
{
	return m_bHaveMouseSlot;
}

inline _XUserItem* _XWindow_SocketDetach::GetIndependentItem()
{
	return m_IndependentItem;
}

inline const int _XWindow_SocketDetach::GetIndependentItemSlot() const
{
	return m_nIndependentSlot;
}

inline const int _XWindow_SocketDetach::GetDetachItemSlot() const
{
	return m_nDeatchIndex;
}

inline const int _XWindow_SocketDetach::GetSocketIndex() const
{
	return m_nSocketIndex;
}

inline const BOOL _XWindow_SocketDetach::GetLock() const
{
	return m_bWindowLock;
}

#endif // !defined(AFX_XWINDOW_SOCKETDETACH_H__FCA41503_7054_467F_99D1_37918B54EF4B__INCLUDED_)
