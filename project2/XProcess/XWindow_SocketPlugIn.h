// XWindow_SocketPlugIn.h: interface for the _XWindow_SocketPlugIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SOCKETPLUGIN_H__FDDC3519_1E9D_4942_96A8_EB1F5B2DF989__INCLUDED_)
#define AFX_XWINDOW_SOCKETPLUGIN_H__FDDC3519_1E9D_4942_96A8_EB1F5B2DF989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 소켓 장착 창
 * 
 * 소켓 공간이 있는 아이템에 소켓아이템을 장착한다.
*/

/// 최대 소켓 공간 갯수
#define _XDEF_SOCKETITEM_MAXCOUNT 4

/// 소켓 단계
enum ListBoxTextMode 
{
	_BOXTEXTMODE_READY, _BOXTEXTMODE_STEP1, _BOXTEXTMODE_STEP2
};

class _XWindow_SocketPlugIn : public _XWindow  
{
private:
	/** 리스트박스에 들어갈 내용을 담는 임시 텍스트 버퍼 사이즈는 _XDEF_SOCKETITEM_MAXCOUNT*2 임
	* 현재 소켓 아이템의 속성이 소켓 아이템 한개당 최대 2개가 들어가지만 만약 늘어날 경우 늘려줘야 함
	*/	
	TCHAR				m_ListBoxTextList[8][255];	

public:
	_XButton*			m_SocketPlugInButton;
	_XButton*			m_pApplyCancleButton;
	_XButton*			m_pClosebutton;
	
	_XUserItem*			m_IndependentItem;		///< 대상 아이템 (무기, 의상)
	_XUserItem			m_SocketItemList[_XDEF_SOCKETITEM_MAXCOUNT];	///< 소켓 아이템 공간
	POINT				m_SlotPosition[_XDEF_SOCKETITEM_MAXCOUNT];		///< 소켓 아이템 공간 위치
	_XImageStatic*		m_SocketSlot[_XDEF_SOCKETITEM_MAXCOUNT];		///< 소켓 아이템 공간 이미지

	_XMouseSlotItem		m_TempSaveSlotItem;		///< 대상 아이템이 변경되었을 때 필요한 임시 저장 공간
	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;
	BOOL				m_bDrawSelectedItem;	
	int					m_LastDraggingSlot;
	BOOL				m_bChangeButtonMode;
	BOOL				m_MsgSocketPuluInOpen;
	
	_XListBox*			m_pSocketitemListBox;	 
	ListBoxTextMode		m_ListBoxTextMode;	
	
public:
	_XWindow_SocketPlugIn();
	virtual ~_XWindow_SocketPlugIn();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );	
	int					GetSlotIndex(void);
	void				DrawDragIcon(void);
	void				DrawSelectedItem(void);
	void				MessagePrint(int msgindex);
	void				ChangeIndependentItem(void);
	void				ResetSocketItemList(void);
	void				RebuildListBoxText( ListBoxTextMode mode);
	
	void				SetDefaultPosition( void );

	FLOAT				m_SocketRotateAngle;
	D3DXMATRIX			m_SocketRHWMatrix;
	_X3PCamera			m_SocketModelViewCamera;
	D3DVIEWPORT9		m_SocketModelViewPort;	

	void				DrawSocketPlugInionItem( void );
	int					GetSocketModelIndex();
	void				ProcessDblClick(void);
	
	void				ResetData(void);
	
};

#endif // !defined(AFX_XWINDOW_SOCKETPLUGIN_H__FDDC3519_1E9D_4942_96A8_EB1F5B2DF989__INCLUDED_)
