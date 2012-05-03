// XGUIControlCenter.h: interface for the _XGUIControlCenter class.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _XGUICONTROLCENTER_H_
#define _XGUICONTROLCENTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <d3dx9.h>
#include "XTimer.h"
#include "XGUIObject.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XGUI PreDefine macro
#define _XDEF_DEFAULTDEPTH_FORINTERFACE			1.0f //0.99f
#define _XDEF_XTIMER_BUTTONCLICK				0
#define _XDEF_XTIMER_GUIPROCESSTICK				0.1f

#define	_XDEF_DUMMYID_MESSAGEBOX				((_XGUIObject *)0x01)
#define	_XDEF_DUMMYID_GAMEBACKGROUND			((_XGUIObject *)0x02)
#define _XDEF_DUMMYID_INVENTORY					((_XGUIObject *)0x03)
#define _XDEF_DUMMYID_NPCTRADE					((_XGUIObject *)0x04)
#define _XDEF_DUMMYID_WAREHOUSE					((_XGUIObject *)0x05)
#define _XDEF_DUMMYID_PCTRADE					((_XGUIObject *)0x06)
#define _XDEF_DUMMYID_SOCKETPLUGIN				((_XGUIObject *)0x07)
#define _XDEF_DUMMYID_QUICKSLOT					((_XGUIObject *)0x08)
#define _XDEF_DUMMYID_EQUIPEDSLOT				((_XGUIObject *)0x09)
#define _XDEF_DUMMYID_CONTRIBUTION				((_XGUIObject *)0x0A)
#define _XDEF_DUMMYID_REPAIR					((_XGUIObject *)0x0B)
#define _XDEF_DUMMYID_POCKET					((_XGUIObject *)0x0C)
#define _XDEF_DUMMYID_FUNCTIONALOBJECT			((_XGUIObject *)0x0D)
#define _XDEF_DUMMYID_GAMBLEBOX					((_XGUIObject *)0x0E)
#define _XDEF_DUMMYID_SOCKETPLUGINEX			((_XGUIObject *)0x0F)
#define _XDEF_DUMMYID_CASHITEM					((_XGUIObject *)0x10)
#define _XDEF_DUMMYID_INCHANT					((_XGUIObject *)0x11)

#define _XDEF_WINDOWTITLESTRINGSIZE				51
#define _XDEF_XWINSNAPDISTANCE					15
#define _XDEF_XWINEDGESNAPDISTANCE				8

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System default color macro
#define _XSC_DEFAULT_BLACK						0xFF000000
#define _XSC_DEFAULT_WHITE						0xFFFFFFFF
#define _XSC_DEFAULT							g_SystemColorTable[_XSYSCOLOR_DEFAULT			]
#define _XSC_DEFAULT_HIGHLIGHT					g_SystemColorTable[_XSYSCOLOR_DEFAULT_HIGHLIGHT	]
#define _XSC_DEFAULT_CLICK						g_SystemColorTable[_XSYSCOLOR_DEFAULT_CLICK		]
#define _XSC_WARNING							g_SystemColorTable[_XSYSCOLOR_WARNING			]
#define _XSC_WARNING_HIGHLIGHT					g_SystemColorTable[_XSYSCOLOR_WARNING_HIGHLIGHT	]
#define _XSC_WARNING_CLICK						g_SystemColorTable[_XSYSCOLOR_WARNING_CLICK		]
#define _XSC_AREAINFO							g_SystemColorTable[_XSYSCOLOR_AREAINFO			]
#define _XSC_GAUGENUMBER						g_SystemColorTable[_XSYSCOLOR_GAUGENUMBER		]
#define _XSC_INFORMATION						g_SystemColorTable[_XSYSCOLOR_INFORMATION		]
#define _XSC_TABMENU_1							g_SystemColorTable[_XSYSCOLOR_TABMENU_1			]
#define _XSC_TABMENU_2							g_SystemColorTable[_XSYSCOLOR_TABMENU_2			]
#define _XSC_ITEM_SELECT						g_SystemColorTable[_XSYSCOLOR_ITEM_SELECT		]
#define _XSC_ITEM_CLICK							g_SystemColorTable[_XSYSCOLOR_ITEM_CLICK		]

typedef enum
{	
	_XSYSCOLOR_DEFAULT = 0,
	_XSYSCOLOR_DEFAULT_HIGHLIGHT,
	_XSYSCOLOR_DEFAULT_CLICK,
	_XSYSCOLOR_WARNING,
	_XSYSCOLOR_WARNING_HIGHLIGHT,
	_XSYSCOLOR_WARNING_CLICK,	
	_XSYSCOLOR_AREAINFO,
	_XSYSCOLOR_GAUGENUMBER,
	_XSYSCOLOR_INFORMATION,
	_XSYSCOLOR_TABMENU_1,
	_XSYSCOLOR_TABMENU_2,
	_XSYSCOLOR_ITEM_SELECT,
	_XSYSCOLOR_ITEM_CLICK,
	_XSYSCOLOR_TOTALCOUNT = 16,
}_XSysColorTable;

extern  D3DCOLOR								g_SystemColorTable[_XSYSCOLOR_TOTALCOUNT];

typedef enum _XFONTID
{
	_XFONTID_SMALL = 0,
	_XFONTID_BASE,
	_XFONTID_LARGE,
} XFONTID;

_XFontManager_Baseclass* _XGetFont( _XFONTID fontid );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Tooltip callback
typedef	void (__stdcall *_XGUITooltipCallback)( POINT pos, SIZE size );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Direct draw func...
void		_XDrawLine(D3DXVECTOR3& startvec, D3DXVECTOR3& endvec, D3DCOLOR color);
void		_XDrawLine2D( FLOAT px1, FLOAT py1, FLOAT px2, FLOAT py2, D3DCOLOR color1, D3DCOLOR color2 );
void		_XDrawRectAngle(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT baseheight, D3DCOLOR color);
void		_XDrawRoundRectAngle( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT baseheight, D3DCOLOR color );
void		_XDrawSolidBar(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Control center class
class _XGUIControlCenter  
{
public:
	_XTimer	m_GUITimer;
	
public:
	_XGUIControlCenter();
	virtual ~_XGUIControlCenter();

	BOOL	Initialize( void );

	void	Release( void );

};

extern _XGUIControlCenter	g_XGUIControlCenter;

#endif // !defined(_XGUICONTROLCENTER_H_)
