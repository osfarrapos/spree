// XUnicodeFont.h: interface for the _XUnicodeFont class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XUNICODEFONT_H_
#define _XUNICODEFONT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3d9.h"
#include "XFont.h"

class _XUnicodeFont
{	
public :
	_XUnicodeFont();
	virtual ~_XUnicodeFont();
	
	bool				Load(LPCTSTR szFilename);
	
	int					CalcWidth(LPCSTR text);
	int					CalcWidth(wchar_t* text);
	
	int					GetWidth(unsigned short code);
	int					GetHeight() { return m_maxheight; }
	
	int					Draw(unsigned char * ptr, int pitch, unsigned short code);
	int					DrawClip(unsigned char * ptr, int pitch, unsigned short code, int skip, int rest);

public	:
	unsigned short*		m_LookupTable;
	
private :	
	int					m_maxheight;
	
	unsigned short		m_cnt;	
	unsigned char*		m_image;
	
	Chinfo				*m_info ;
	
	inline int			SearchIndex(unsigned short code){ return m_LookupTable[code]; }	
};

typedef struct	_XUNICODEPEICETEX
{
	_XUNICODEPEICETEX	* prev, * next;
	_XUnicodeFont*		font;
	int					texidx;
	int					updatecnt;
	int					u, v, w, h;
	WCHAR				unicodetext[1];
} _XUNICODEPEICETEX;

class _XUnicodeFontWorkspace
{
public :
	_XUnicodeFontWorkspace(int maxtex);
	virtual ~_XUnicodeFontWorkspace();
	
	void				Render(int texhandle, float x, float y, float w, float h, 
		                       float tu, float tv, float tw, float th, 
							   unsigned long type, unsigned long color);

	void				Draw(float x, float y, float scale, unsigned long type, unsigned long color, void * p);
	LPVOID				GetPieceTex(WCHAR* text, _XUnicodeFont * font);
	int					GetWidth(void *p);

	void				ClearAll();
	void				Flush();

	int					CreateTexture(int w, int h);
	void				ReleaseTexture(DWORD texidx);
	void				UpdateTexture(DWORD texidx, unsigned char *ptr, int x, int y, int w, int h, int bpl, unsigned char (* palette)[3]);
	LPVOID				Lock(DWORD texidx, int * pitch);
	void				Unlock(DWORD texidx);
	void				SetTexture(DWORD idx);
	void				SetPolyType(unsigned long type);
	void				DrawPolygon(_XFONTVERTEX *v, int n);

	void				DisableClippingArea( void );
	void				SetClippingArea( _XFRECT& cliprect );	
	void				SetClippingArea( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom );	
	
private :
	_XUNICODEPEICETEX	*m_piece, *m_reserve;
	
	int*				m_texhandle;
	int					m_maxtex;
	int					m_sparetex;
	
	int					m_curtex;
	
	int					m_updatecnt;
	int					m_curtexb;
	
	LPBYTE				m_image;
	
	int					m_u;
	int					m_v;
	int					m_vbottom;
	int					m_vtop;

	_XFONTTEXTURE		m_fonttexturelist[128];
	DWORD				m_oldtype;
	int					m_oldtexhandle;
	
	_XUNICODEPEICETEX*	RemoveLink(_XUNICODEPEICETEX *head, _XUNICODEPEICETEX *p) 
	{
						if (p->next)
							p->next->prev = p->prev;
						if (p->prev)
							p->prev->next = p->next;
						else return p->next;
						return head;
	}
	
	_XUNICODEPEICETEX*	AddLink(_XUNICODEPEICETEX *head, _XUNICODEPEICETEX *p) 
	{
						if (head) head->prev = p;
						p->prev = 0l;
						p->next = head;
						return p;
	}
	
	void				IncTexCnt();
	void				Rearrange();
	
	void				WriteTexture(_XUNICODEPEICETEX * p);
	void				UpdateTexture(int idx, int top, int bottom);
	void				NextLine(int height);
	void				ClearPage(int idx);

	// Render grow font
	_XFONT_EDGEMODE		m_fontedgemode;
	D3DCOLOR			m_fontedgecolor;
	
	BOOL				m_renderfontbold;

public:	
	// Screen clipping
	D3DVIEWPORT9		m_ClippingViewPort;
	BOOL				m_EnableClippingArea;

public:
	void				SetGrowMode( _XFONT_EDGEMODE mode, D3DCOLOR	fontedgecolor )
	{
						m_fontedgemode   = mode;
						m_fontedgecolor	 = fontedgecolor;
	}
	void				DisableGrowMode( void )
	{
						m_fontedgemode   = _XFONT_EDGEMODE_NONE;
	}
	void				SetBoldMode( BOOL use )
	{
						m_renderfontbold = use;
	}
} ;

class _XUnicodeFontManager : public _XFontManager_Baseclass
{
public :
	_XUnicodeFontManager();
	virtual ~_XUnicodeFontManager();
	
	void				DeleteDeviceObject();
	bool				Load(LPCTSTR szFilename);
	
	void				SetColor(unsigned long color) { m_color = color; }
	void				SetPolyType(unsigned long type) { m_polytype = type; }
	
	void				Puts(int x, int y, LPCTSTR str, float scale=1.0f);
	int					Puts_Separate(int x, int y, LPCTSTR str, int width, float scale=1.0f, int addlinepitch = 0 );		
	int					Puts_SeparateNewLineVersion(int x, int y, LPCTSTR str, int width, float scale=1.0f, int addlinepitch = 0 );	
	int					Puts_SeparateAlign(int x, int y, LPCTSTR str, int width, _XFONT_ALIGNTYPE aligntype, float scale=1.0f, int addlinepitch = 0 );	
	void				PutsAlign(int x, int y, _XFONT_ALIGNTYPE aligntype, TCHAR* str);
	void				Print(int x, int y, float scale, TCHAR* args, ...);	
	void				PrintClipping(int x, int y, int width, TCHAR* args, ...);	
	void				PrintAlign(int x, int y, float scale, _XFONT_ALIGNTYPE aligntype, TCHAR* args, ...);
	void				PutsVertical(int x, int y, LPCTSTR str, float scale=1.0f);
	
	void				Flush();
	
	int					GetWidth(LPCSTR p){ if( m_workspace ) return m_font->CalcWidth( p ); else return 0;  }
	int					GetWidth(wchar_t* p){ if( m_workspace ) return m_font->CalcWidth( p ); else return 0;  }
	int					GetHeight( void ){ if( m_font ) return m_font->GetHeight(); else return 12; }
	int					GetRowCount( const TCHAR* str, int interval );
	int					GetRowCountNewLineVersion( const TCHAR* str, int interval );

	void				SplitString(LPCTSTR str, int width, _XStringSplitInfo& splitinfo, int tabwidth = 0, 
									_XStringSplitFunction splitfunction = _XSTRINGSPLITFUNCTION_WIDTH );
	
	void				SplitStringByCR(LPCTSTR str, _XStringSplitInfo& splitinfo);
	
	void				SetGrowMode( _XFONT_EDGEMODE mode, D3DCOLOR	fontedgecolor )
	{
		if( m_workspace ) m_workspace->SetGrowMode( mode, fontedgecolor );
	}
	void				DisableGrowMode( void )
	{
		if( m_workspace ) m_workspace->DisableGrowMode();
	}
	void				SetBoldMode( BOOL use )
	{
		if( m_workspace ) m_workspace->SetBoldMode( use );
	}
	
	void				DisableClippingArea( void );
	void				SetClippingArea( _XFRECT& cliprect );
	void				SetClippingArea( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom );	
	void				SetAlphaBlendingEnable( BOOL alphablending );

	BOOL				IsValidCharacter( unsigned short _code );

	void				SetUnicodeDataMode( BOOL unicode ){ m_UnicodeDataMode = unicode; }
	
private :
	_XUnicodeFontWorkspace*	m_workspace;
	_XUnicodeFont*		m_font;
	
	unsigned long		m_color;
	unsigned long		m_polytype;
	
	BOOL				m_alphablending;
	
	PiecePoly			m_polylist[128];
	
	int					m_polylistn;

	BOOL				m_UnicodeDataMode;
} ;

#endif // !defined(_XUNICODEFONT_H_)
