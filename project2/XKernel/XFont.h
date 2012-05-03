// XFont.h: interface for the _XFont class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XFONT_H_
#define _XFONT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3d9.h"

#define _POLYFAN				0x0000
//#define _POLYTRISTRIP			0x0001
//#define _POLYTRILIST			0x0002

//#define _POLYWIRE				0x0004

#define _COORDMASK				0x0010
#define _COORDWORLD				0x0000
#define _COORDPROJECTED			0x0010

#define _TEXBLENDMASK			0x0700
#define _TEXBLENDNORMAL			0x0000
#define _TEXBLENDADD			0x0100
#define _TEXBLENDMODULATE		0x0200
#define _TEXBLENDINVMODULATE	0x0300

//#define _NOCOLOR				0x1000
#define _NOCULL					0x2000

//#define _XUSE_CLIPPING

#define _XMAGICVALUE			-0.5f

#define TEX_W					128
#define TEX_H					128
//#define _XSPLIT_CHAR_

#define _XCHAR_GAP	0

typedef enum _XFONT_ALIGNTYPE
{
	_XFONT_ALIGNTYPE_LEFT = 0,
	_XFONT_ALIGNTYPE_RIGHT,
	_XFONT_ALIGNTYPE_CENTER,
};

typedef enum _XFONT_EDGEMODE
{
	_XFONT_EDGEMODE_NONE = 0,
	_XFONT_EDGEMODE_EDGE,
	_XFONT_EDGEMODE_FULLEDGE,
	_XFONT_EDGEMODE_GLOW
};

typedef struct _pixfmt 
{
	D3DFORMAT fmt;
	int cpp; // char pre pixel
	int r_r_shift, r_l_shift;
	int g_r_shift, g_l_shift;
	int b_r_shift, b_l_shift;
	int a_r_shift, a_l_shift;
	
	unsigned long Pack(unsigned char r, unsigned char g, unsigned char b, unsigned char a) 
	{
		return ((a>>a_r_shift) << a_l_shift) | ((r>>r_r_shift) << r_l_shift) | \
			((g>>g_r_shift) << g_l_shift) | ((b>>b_r_shift) << b_l_shift);
	}
}_pixfmt;

struct _XFONTVERTEX
{
	FLOAT		x, y, z, rhw;
	D3DCOLOR	color;
	FLOAT		tu, tv;
};

typedef struct Chinfo 
{
	int				offset;
	unsigned char	w, h;
}Chinfo;

typedef struct _XFONTTEXTURE
 {
	IDirect3DTexture9 * tex;
	D3DFORMAT			fmt;
	int					fmtidx;
	int					w, h;
}_XFONTTEXTURE;

typedef struct	PiecePoly 
{
	void * p;
	float x, y, scale;
	unsigned long color;
	unsigned long type;
}PiecePoly;

class _XFont  
{	
public :
	_XFont();
	virtual ~_XFont();
	
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

typedef struct	_XPEICETEX
{
	_XPEICETEX			* prev, * next;
	_XFont*				font;
	int					texidx;
	int					updatecnt;
	int					u, v, w, h;
	char				text[1];
} _XPEICETEX;

typedef struct	_tagXFRECT
{
	FLOAT	left;
	FLOAT	top;
	FLOAT	right;
	FLOAT	bottom;
} _XFRECT;

class _XFontWorkspace
{
public :
	_XFontWorkspace(int maxtex);
	virtual ~_XFontWorkspace();
	
	void				Render(int texhandle, float x, float y, float w, float h, 
		                       float tu, float tv, float tw, float th, 
							   unsigned long type, unsigned long color);

	void				Draw(float x, float y, float scale, unsigned long type, unsigned long color, void * p);
	LPVOID				GetPieceTex(LPCTSTR text, _XFont * font);
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
	_XPEICETEX			*m_piece, *m_reserve;
	
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
	
	_XPEICETEX*			RemoveLink(_XPEICETEX *head, _XPEICETEX *p) 
	{
						if (p->next)
							p->next->prev = p->prev;
						if (p->prev)
							p->prev->next = p->next;
						else return p->next;
						return head;
	}
	
	_XPEICETEX*			AddLink(_XPEICETEX *head, _XPEICETEX *p) 
	{
						if (head) head->prev = p;
						p->prev = 0l;
						p->next = head;
						return p;
	}
	
	void				IncTexCnt();
	void				Rearrange();
	
	void				WriteTexture(_XPEICETEX * p);
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


#define _XDEF_STRINGSPRITINFOMAXSIZE	500

typedef struct
{
	int			splitcount;
	LPTSTR		splitpoint[_XDEF_STRINGSPRITINFOMAXSIZE];
	int			splitlength[_XDEF_STRINGSPRITINFOMAXSIZE];	
}_XStringSplitInfo;

typedef enum _XStringSplitFunction
{
	_XSTRINGSPLITFUNCTION_WIDTH = 0,
	_XSTRINGSPLITFUNCTION_SPACECHAR
};

class _XFontManager_Baseclass
{
public :
	_XFontManager_Baseclass(){}
	virtual ~_XFontManager_Baseclass(){}

	virtual void		DeleteDeviceObject(void) = 0;
	virtual bool		Load(LPCTSTR szFilename) = 0;
	
	virtual void		SetColor(unsigned long color) = 0;
	virtual void		SetPolyType(unsigned long type) = 0;
	
	virtual void		Puts(int x, int y, LPCTSTR str, float scale=1.0f) = 0;
	virtual int			Puts_Separate(int x, int y, LPCTSTR str, int width, float scale=1.0f, int addlinepitch = 0 ) = 0;
	virtual int			Puts_SeparateNewLineVersion(int x, int y, LPCTSTR str, int width, float scale=1.0f, int addlinepitch = 0 ) = 0;
	virtual int			Puts_SeparateAlign(int x, int y, LPCTSTR str, int width, _XFONT_ALIGNTYPE aligntype, float scale=1.0f, int addlinepitch = 0 ) = 0;
	virtual void		PutsAlign(int x, int y, _XFONT_ALIGNTYPE aligntype, TCHAR* str) = 0;
	virtual void		Print(int x, int y, float scale, TCHAR* args, ...) = 0;
	virtual void		PrintClipping(int x, int y, int width, TCHAR* args, ...) = 0;
	virtual void		PrintAlign(int x, int y, float scale, _XFONT_ALIGNTYPE aligntype, TCHAR* args, ...) = 0;
	virtual void		PutsVertical(int x, int y, LPCTSTR str, float scale=1.0f) = 0;
	
	virtual void		Flush() = 0;
	
	virtual int			GetWidth(LPCSTR p) = 0;
	virtual int			GetWidth(wchar_t* p) = 0;
	virtual int			GetHeight( void ) = 0;
	virtual int			GetRowCount( const TCHAR* str, int interval ) = 0;
	virtual int			GetRowCountNewLineVersion( const TCHAR* str, int interval ) = 0;
	
	virtual void		SplitString(LPCTSTR str, int width, _XStringSplitInfo& splitinfo, int tabwidth = 0, 
									_XStringSplitFunction splitfunction = _XSTRINGSPLITFUNCTION_WIDTH ) = 0;
	
	virtual void		SplitStringByCR(LPCTSTR str, _XStringSplitInfo& splitinfo) = 0;	
	virtual void		SetGrowMode( _XFONT_EDGEMODE mode, D3DCOLOR	fontedgecolor ) = 0;	
	virtual void		DisableGrowMode( void ) = 0;	
	virtual void		SetBoldMode( BOOL use ) = 0;		
	virtual void		DisableClippingArea( void ) = 0;
	virtual void		SetClippingArea( _XFRECT& cliprect ) = 0;
	virtual void		SetClippingArea( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom ) = 0;
	virtual void		SetAlphaBlendingEnable( BOOL alphablending ) = 0;

	virtual BOOL		IsValidCharacter( unsigned short _code ) = 0;

	virtual void		SetUnicodeDataMode( BOOL unicode ) = 0;
} ;

class _XFontManager : public _XFontManager_Baseclass
{
public :
	_XFontManager();
	virtual ~_XFontManager();
	
	void				DeleteDeviceObject();
	bool				Load(LPCTSTR szFilename);
	
	void				SetColor(unsigned long color);
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
	_XFontWorkspace*	m_workspace;
	_XFont*				m_font;
	
	unsigned long		m_color;
	unsigned long		m_polytype;

	BOOL				m_alphablending;
	
	PiecePoly			m_polylist[128];

	int					m_polylistn;

	BOOL				m_UnicodeDataMode;
} ;


typedef enum _XCREATECHARACTERRESULT
{
	_XCREATECHARACTERRESULT_NOERROR = 0,
	_XCREATECHARACTERRESULT_STRINGISNULL,
	_XCREATECHARACTERRESULT_LENGTHISZERO,
	_XCREATECHARACTERRESULT_INCLUDESPACE,
	_XCREATECHARACTERRESULT_INVALIDCHARACTER,
	_XCREATECHARACTERRESULT_SINGLEKOREANCHARACTER,
	_XCREATECHARACTERRESULT_NOVALIDASCIICHARACTER,
	_XCREATECHARACTERRESULT_UNKNOWNERROR
};

_XCREATECHARACTERRESULT _XCheckUsableCharacterName( LPCSTR pszName );
_XCREATECHARACTERRESULT	_XCheckUsableCharacterName_Unicode( wchar_t* pszName, wchar_t& invalidcharacter );
extern TCHAR g_LastErrorCheckedCharacterName[3];

#endif // !defined(_XFONT_H_)
