// X3DUFont.h: interface for the _X3DUFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_X3DUFONT_H__178CA5C3_E798_4A96_B8ED_8A0FCF9E8666__INCLUDED_)
#define AFX_X3DUFONT_H__178CA5C3_E798_4A96_B8ED_8A0FCF9E8666__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>

#define _DEFAULT_TEXTCOLOR  D3DCOLOR_ARGB(255,255,255,255)

#define _XDEF_TEXTDEFAULT_LEFTALIGN		DT_LEFT   | DT_TOP //| DT_NOCLIP
#define _XDEF_TEXTDEFAULT_CENTERALIGN	DT_CENTER | DT_TOP //| DT_NOCLIP
#define _XDEF_TEXTDEFAULT_RIGHTALIGN	DT_RIGHT  | DT_TOP //| DT_NOCLIP

class _X3DUFont  
{
public:
	_X3DUFont();
	virtual ~_X3DUFont();

	//BOOL Initialize( TCHAR* fontname = _T("MS Sans Serif"), int fontsize = 12, DWORD createflag  = 0 );
	BOOL Initialize( TCHAR* fontname, int fontsize = 12, DWORD createflag  = 0 );
	void Release( void );

	void Puts( int x, int y, TCHAR* string, D3DCOLOR color = _DEFAULT_TEXTCOLOR, 
			   DWORD flag = _XDEF_TEXTDEFAULT_LEFTALIGN, int width = 512, int height = 0);
	
	void Print( int x, int y, D3DCOLOR color, TCHAR* args, ... );

	//SIZE GetTextExtentPoint( LPSTR string );

public:
	LPD3DXFONT			pD3DXNormalFont;
	int					fontheight;

};

#endif // !defined(AFX_X3DUFONT_H__178CA5C3_E798_4A96_B8ED_8A0FCF9E8666__INCLUDED_)
