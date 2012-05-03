// XWindow_SocketPlugInExp.h: interface for the _XWindow_SocketPlugInExp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SOCKETPLUGINEXP_H__8C6B756D_CFCF_4F0C_8E5E_CDE7A483DE90__INCLUDED_)
#define AFX_XWINDOW_SOCKETPLUGINEXP_H__8C6B756D_CFCF_4F0C_8E5E_CDE7A483DE90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 소켓 장착 확장, 제거, 분리 창
 * 
 * 소켓을 확장하거나 박힌 소켓을 제거, 분리 하는 창
 * 현재 작업 중지 상태 
*/

enum SocketPlugInExpMode{ SM_NONE=0, SM_EXPAND, SM_SEPARATION, SM_DESTROY };
enum SocketPlugInExpStep{ SS_READY=0, SS_STEP1, SS_STEP2, SS_STEP3, SS_STEP4 };

class _XWindow_SocketPlugInExp : public _XWindow  
{
public:
	SocketPlugInExpMode m_SocketPlugInMode;
	SocketPlugInExpStep m_SocketPlugInStep;

	_XButton*			m_SocketPlugInButton;
	_XButton*			m_pApplyCancleButton;
	_XButton*			m_pClosebutton;
	
	_XUserItem*			m_IndependentItem;
	_XUserItem			m_SocketItemList[4];
	POINT				m_SlotPosition[4];
	_XImageStatic*		m_SocketSlot[4];
	
	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;	
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;
	BOOL				m_bDrawSelectedItem;	
	int					m_LastDraggingSlot;
	BOOL				m_bChangeButtonMode;
	BOOL				m_MsgSocketPuluInOpen;

	POINT				m_SlotPositionEx[4];
	_XImageStatic*		m_SocketSlotEx[4];
	_XImageStatic		m_DisableSlotImage;
	
	_XUserItem			m_SocketExpItemList[4];

	unsigned int		m_CurrentItemMaxSocketCount;
public:
	_XWindow_SocketPlugInExp();
	virtual ~_XWindow_SocketPlugInExp();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );


	FLOAT				m_SocketRotateAngle;
	D3DXMATRIX			m_SocketRHWMatrix;
	_X3PCamera			m_SocketModelViewCamera;
	D3DVIEWPORT9		m_SocketModelViewPort;	

	void				DrawSocketPlugInionItem( void );
	int					GetSocketModelIndex();
	void				MessagePrint(int msgindex);
	void				SetSocketPlugInExMode(SocketPlugInExpMode mode);
	void				SetSocketPlugInExpStep(SocketPlugInExpStep step);
	int					GetSlotIndex(void);
	void				DrawDragIcon(void);
	void				ProcessDbClick(void);
	
	void				ClickStartButton(void);
	void				ResetSocketItemList(void);
	BOOL				SetSocketPlugInExpandStart(int invenslotnumber);
};

#endif // !defined(AFX_XWINDOW_SOCKETPLUGINEXP_H__8C6B756D_CFCF_4F0C_8E5E_CDE7A483DE90__INCLUDED_)
