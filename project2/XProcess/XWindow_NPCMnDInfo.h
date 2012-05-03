// XWindow_NPCMnDInfo.h: interface for the _XWindow_NPCMnDInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCMNDINFO_H__1D885AB1_891C_4293_9578_BD38550C7574__INCLUDED_)
#define AFX_XWINDOW_NPCMNDINFO_H__1D885AB1_891C_4293_9578_BD38550C7574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMnDStructInfo.h"

class _XWindow_NPCMnDInfo : public _XWindow  
{
public:
	_XWindow_NPCMnDInfo();
	~_XWindow_NPCMnDInfo();

	BOOL Initialize(void);
	void DestroyWindow(void);
	
	void Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void ShowWindow(BOOL show);
	void MoveWindow(int X, int Y);

private :
	_XStatic m_BorderTitle;

	_XListBox* m_pMasterInfoListBox;
	int m_nListBoxCount;
};

#endif // !defined(AFX_XWINDOW_NPCMNDINFO_H__1D885AB1_891C_4293_9578_BD38550C7574__INCLUDED_)
