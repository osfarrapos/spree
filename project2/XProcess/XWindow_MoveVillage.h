// XWindow_MoveVillage.h: interface for the _XWindow_MoveVillage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MOVEVILLAGE_H__2BFAB199_B5C5_4CE8_BC57_5BDA21273A13__INCLUDED_)
#define AFX_XWINDOW_MOVEVILLAGE_H__2BFAB199_B5C5_4CE8_BC57_5BDA21273A13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "XWindow.h"

#include <vector>
//#include "XKernel.h"


typedef struct XMoveVillageInfo
{
	int		id;
	int		mapid;
	int		areaid;
	int		strmapid;
	int		strvillageid;
	int		property;
	int		level;

	int		innerlevel;
	int		posx;
	int		posy;

	LPDIRECT3DTEXTURE9	pMapTexture; //Author : 양희왕 //breif : 속도 증가를 위해

	XMoveVillageInfo()
	{
		id		= 0;
		mapid	= 0;
		areaid	= 0;
		strmapid = 0;
		strvillageid = 0;
		property = 0;
		level	= 0;

		innerlevel = 0;
		posx	= 0;
		posy	= 0;

		pMapTexture = NULL;
	};

}_XMoveVillageInfo;

typedef struct XMapInfo
{
	int		mapid;				// map id
	int		strmapid;			// map string id
	int		villageindex;		// _XMoveVillageInfo에 village id의 마지막 번호
	BOOL	expand;				// 리스트 박스에서 펼쳐졌는지 아닌지 
}_XMapInfo;

class _XWindow_MoveVillage  : public _XWindow
{
private :
	vector <_XMoveVillageInfo*>		m_MoveVillageInfo;
	map <int, _XMapInfo*>			m_MapInfo;
	int				m_nMaxIndex;
	FLOAT			m_fScaleRate;

public :
	_XListBox*		m_lstboxVillage;

private :
	_XButton*		m_btnMove;
	_XButton*		m_btnCancel;

	LPDIRECT3DTEXTURE9	m_pMapTexture;

	_XImageStatic		m_imgMap;
	_XStatic			m_BorderTitle;

	int					m_CurrentMapIndex;

	int					m_SelectedMoveVillageIndex;
	int					m_SelectedInvenNumber;
	
	int					m_SelectedMapIndex;

	_XImageStatic*		m_MarkSmall;
	_XImageStatic*		m_MarkLarge;

public:
	_XWindow_MoveVillage();
	virtual ~_XWindow_MoveVillage();

	BOOL		LoadMoveVillageInfo(void);
	BOOL		BuildMapInfo(void);

	BOOL		Initialize();
	void		DestroyWindow();

	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void		ShowWindow(BOOL show);
	void		MoveWindow(int X, int Y);

	BOOL		SetMapImage(int mapindex, LPDIRECT3DTEXTURE9& pTexture);
	void		SetListItem(void);

	void		ProcessListItem(DWORD commandid);
	
	BOOL		CheckUse(void);
	
	void		SetSelectedMoveVillageIndex(int index)	{	m_SelectedMoveVillageIndex = index;	};
	void		SetSelectedInvenNumber(int slot)		{	m_SelectedInvenNumber = slot;		};

	int			GetSelectedMoveVillageIndex(void)		{	return m_SelectedMoveVillageIndex;	};
	int			GetSelectedInvenNumber(void)			{	return m_SelectedInvenNumber;		};

};

#endif // !defined(AFX_XWINDOW_MOVEVILLAGE_H__2BFAB199_B5C5_4CE8_BC57_5BDA21273A13__INCLUDED_)
