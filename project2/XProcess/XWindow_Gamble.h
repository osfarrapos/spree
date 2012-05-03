// XWindow_Gamble.h: interface for the _XWindow_Gamble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GAMBLE_H__062D5303_D83E_4643_B56D_D9BF9D0E3C8A__INCLUDED_)
#define AFX_XWINDOW_GAMBLE_H__062D5303_D83E_4643_B56D_D9BF9D0E3C8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 상자 겜블 창
 * 패왕궤를 연다. 패왕시를 사용하여 확률을 높일 수도 있고 
 * 패왕시 없이 강제 열기도 할 수 있다.
 * 
*/

#define _XDEF_GAMBLEBOX_MAXCOUNT 3		///< 최대 사용할 수 있는 패왕시 갯수

class _XWindow_Gamble : public _XWindow  
{
public:
	_XButton*			m_pBoxOpenButton;
	_XButton*			m_pApplyCancleButton;
	_XButton*			m_pClosebutton;
	
	_XUserItem*			m_GambleBoxItem;							///< 패왕궤
	_XUserItem			m_GambleKeyList[_XDEF_GAMBLEBOX_MAXCOUNT];	///< 패왕시 
	POINT				m_SlotPosition[_XDEF_GAMBLEBOX_MAXCOUNT];	
	_XImageStatic*		m_GambleKeySlot[_XDEF_GAMBLEBOX_MAXCOUNT];	///< 패왕시 뒷 이미지
	
	int					m_GambleKeyFromInvenNumber[_XDEF_GAMBLEBOX_MAXCOUNT];

	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;
	BOOL				m_bDrawSelectedItem;	
	int					m_LastDraggingSlot;
	BOOL				m_MsgBoxOpen;			///< 메세지 창이 열려있는지 

	/// 3디 모델을 위한 변수들
	FLOAT				m_GambleItemRotateAngle;
	D3DXMATRIX			m_GambleItemRHWMatrix;
	_X3PCamera			m_GambleItemModelViewCamera;
	D3DVIEWPORT9		m_GambleItemModelViewPort;	
public:
	_XWindow_Gamble();
	virtual ~_XWindow_Gamble();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );

	int					GetSlotIndex(void);	
	void				DrawDragIcon(void);
	void				DrawSelectedItem(void);
	void				MessagePrint(int msgindex);		///< 에러 메세지 출력
	void				ResetSocketItemList(void);	
	
	void				SetDefaultPosition( void );

	void				DrawSocketPlugInionItem( void );	///< 3디 모델 출력
	int					GetSocketModelIndex();
	void				ProcessDblClick(void);
	
	void				ResetData(void);
	
	BOOL				CheckAllSlot(int invennumber);

};

#endif // !defined(AFX_XWINDOW_GAMBLE_H__062D5303_D83E_4643_B56D_D9BF9D0E3C8A__INCLUDED_)
