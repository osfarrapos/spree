// XWindow_PK_ListBox.h: interface for the XWindow_PK_ListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PK_LISTBOX_H__434E52FD_A961_4CE8_B28D_A578355E9D3C__INCLUDED_)
#define AFX_XWINDOW_PK_LISTBOX_H__434E52FD_A961_4CE8_B28D_A578355E9D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include <vector>

class XWindow_PK_ListBox : public _XWindow
{
public:
	XWindow_PK_ListBox();
	virtual ~XWindow_PK_ListBox();

public:
	BOOL Initialize();
	BOOL Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void DestroyWindow();
	void Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	void MoveWindow(int X, int Y);
	void ShowWindow(BOOL show);
	void SetListItem(void);
	void UpdateLeftTime(void);
	
	void DeleteList(void);

public:
	inline void SetSelectedItem( int nParam ) { m_nSelectedItem = nParam; }
	inline const int GetSelectedItem() const { return m_nSelectedItem; } 

	inline const DWORD GetLapsedTime() const { return m_nLapsed; } 
	inline _XListBox& GetListBox() { return *m_pListBox; } 

private:
	_XListBox* m_pListBox;
	int m_nSelectedItem;
	int m_nLapsed;

	DWORD	m_dwUpdatedTime;
};

#endif // !defined(AFX_XWINDOW_PK_LISTBOX_H__434E52FD_A961_4CE8_B28D_A578355E9D3C__INCLUDED_)
