// XWindow_NPCBuff.h: interface for the _XWindow_NPCBuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCBUFF_H__4C7BA2A9_5F63_47F3_85CD_1E8608D09347__INCLUDED_)
#define AFX_XWINDOW_NPCBUFF_H__4C7BA2A9_5F63_47F3_85CD_1E8608D09347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"
#include <map>

struct _XNPCBUFFSTR
{
#ifdef _XDEF_NEWSTRINGMANAGER	
	LPTSTR		name;
	LPTSTR		info;
#else
	TCHAR		name[32];
	TCHAR		info[64];
#endif
	int			type;
	int			bufindex;
	int			bufvalue;
	int			time;
	int			price;
	int			nSkillindex;
	
	_XNPCBUFFSTR() : type(0), bufindex(0), bufvalue(0), time(0), price(0), nSkillindex(0)
	{
#ifdef _XDEF_NEWSTRINGMANAGER	
		name = NULL;
		info = NULL;
#else
		memset(name, 0, sizeof(name));
		memset(info, 0, sizeof(info));
#endif
	}
};


struct _XNPCBUFFLIST
{
	int			nList[15];

	_XNPCBUFFLIST()
	{
		ZeroMemory(nList,sizeof(nList));
	}
};

enum
{
	EN_MODE_NPC_VIEW = 0,
	EN_MODE_ITEM_VIEW,
};

class _XWindow_NPCBuff : public _XWindow
{
private :
	_XListBox*		m_lstboxBuff;
	_XStatic		m_BorderTitle;

	int				m_nViewMode;
	
public :
	int				m_SelectedItem;
	_XImageStatic	m_LeftSelectedImage;
	_XImageStatic	m_RightSelectedImage;
	TCHAR			m_szBuffinfo[20][256];
	
public:
	_XWindow_NPCBuff();
	virtual ~_XWindow_NPCBuff();

	BOOL			Initialize();
	void			DestroyWindow();

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	void			SetListItem();
	void			SetListFreeItem();		// 무명신침 사용할 때 (Price : Free)
	
	BOOL			CheckUse();
	void			SetViewMode(int mode);

};

#endif // !defined(AFX_XWINDOW_NPCBUFF_H__4C7BA2A9_5F63_47F3_85CD_1E8608D09347__INCLUDED_)
