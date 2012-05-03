/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	WinHelper.h
Owner:	russf@gipsysoft.com
Purpose:	Windows helper functions, classes, structures and macros
					that make life a little easier
					These should all be zero impact classes etc. that is they
					should *not* have a cpp file associated with them.
----------------------------------------------------------------------*/
#ifndef WINHELPER_H
#define WINHELPER_H

#ifndef COMMON_H
	#include <common.h>
#endif	//	COMMON_H

namespace WinHelper
{
	#define ZeroStructure( t ) memset( &t, 0, sizeof( t ) )
	#define countof( t )	(sizeof( (t) ) / sizeof( (t)[0] ) )
#if ! defined(lint)
	#define UNREF(P)          (P)
#else	//	lint
	#define UNREF(P)          \
    /*lint -save -e527 -e530 */ \
    { \
        (P) = (P); \
    } \
    /*lint -restore */
#endif	//	lint

	inline int MulDiv(int nNumber, int nNumerator, int nDenominator)
	{
#if defined (_WIN32_WCE)
		__int64 x;

		x = (__int64)nNumber * (__int64)nNumerator;
		x /= (__int64)nDenominator;

		return (int)x;
#else	//	(_WIN32_WCE)
		return ::MulDiv( nNumber, nNumerator, nDenominator );
#endif	//	(_WIN32_WCE)
	}


	class CSize : public tagSIZE
	//
	//	Wrapper for the SIZE structure
	{
	public:
		inline CSize() {};
		inline explicit CSize( const SIZE &size ) { cx = size.cx; cy = size.cy; }
		inline explicit CSize( PI_int32 nSizeX, PI_int32 nSizeY ) { cx = nSizeX; cy = nSizeY; }
		inline CSize( const CSize& size ) { cx = size.cx; cy = size.cy; };
		inline operator LPSIZE() { return this; };
		inline void Set( long nSizeX, long nSizeY ) { cx = nSizeX; cy = nSizeY; }


		inline bool operator !=( const SIZE &size ) const { return cx != size.cx || cy != size.cy;}
		inline CSize & operator =( const SIZE &size ) { cx = size.cx; cy = size.cy; return *this; }
		inline bool operator >( const SIZE &size ) { return cx > size.cx || cy > size.cy; }
	};


	class CRect : public tagRECT
	//
	//	Wrapper for a RECT structure
	{
	public:
		inline CRect() {}
		//	Initialisation constructor
		inline explicit CRect( const RECT& rhs ) { Set( rhs.left, rhs.top, rhs.right, rhs.bottom );}
		inline CRect(PI_int32 xLeft, PI_int32 yTop, PI_int32 xRight, PI_int32 yBottom) { Set( xLeft, yTop, xRight, yBottom ); }
		//	Get the width of the rectangle
		inline PI_int32 Width() const
			{ return right - left; }
		//	Get the height of the rectangle
		inline PI_int32 Height() const
			{ return bottom - top; }
		//	overloaded operator so you don't have to do &rc anymore
		inline operator LPCRECT() const
			{ return this; };
		inline operator LPRECT()
			{ return this; };
		//	Return the SIZE of the rectangle;
		inline CSize Size() const
			{ CSize s( Width(), Height() ); return s; }
		//	Return the top left of the rectangle
		inline POINT TopLeft() const
			{ POINT pt = { left, top }; return pt; }
		//	Return the bottom right of the rectangle
		inline POINT BottomRight() const
			{ POINT pt = { right, bottom }; return pt; }
		//	Set the rectangles left, top, right and bottom
		inline void Set( PI_int32 xLeft, PI_int32 yTop, PI_int32 xRight, PI_int32 yBottom)
			{ top = yTop; bottom = yBottom; right = xRight; left = xLeft; }
		inline void Set( const WinHelper::CRect &rhs )
			{
				left = rhs.left; top = rhs.top;
				right = rhs.right; bottom = rhs.bottom;
			}

		//	Return true if the rectangle contains all zeros
		inline bool IsEmpty() const
			{ return left == 0 && right == 0 && top == 0 && bottom == 0 ? true : false; }
		//	Zero out our rectangle
		inline void Empty()
			{ left = right = top = bottom = 0; }
		//	Set the size of the rect but leave the top left position untouched.
		inline void SetSize( const CSize &size )
			{ bottom = top + size.cy; right = left + size.cx; }
		inline void SetSize( const SIZE &size )
			{ bottom = top + size.cy; right = left + size.cx; }
		inline void SetSize( PI_int32 cx, PI_int32 cy )
			{ bottom = top + cy; right = left + cx; }
		inline void SetHeight( PI_int32 cy )
			{ bottom = top + cy; }
		inline void SetWidth( PI_int32 cx )
			{ right = left + cx; }
		//	Move the rectangle by an offset
		inline void Offset( PI_int32 cx, PI_int32 cy )
			{
				top+=cy;
				bottom+=cy;
				right+=cx;
				left+=cx;
			}
		//	Inflate the rectangle by the cx and cy, use negative to shrink the rectangle
		inline void Inflate( PI_int32 cx, PI_int32 cy )
			{
				top-=cy;
				bottom+=cy;
				right+=cx;
				left-=cx;
			}
		//	Assignment from a RECT
		inline CRect &operator = ( const RECT&rhs )
			{
				left = rhs.left; top = rhs.top;
				right = rhs.right; bottom = rhs.bottom;
				return *this;
			}

		
		//	Return true if the point passed is within the rectangle
		inline bool PtInRect( const POINT &pt ) const	{	return  ( pt.x >= left && pt.x < right && pt.y >=top && pt.y < bottom ); }
		//	Return true if the rectangle passed overlaps this rectangle
		inline bool Intersect( const RECT &rc ) const {	return ( rc.left < right && rc.right > left && rc.top < bottom && rc.bottom > top ); }
	};

	class CPoint : public tagPOINT
	//
	//	Wrapper for the POINT structure
	{
	public:
		inline CPoint() {};
#ifdef WIN32
		inline CPoint( LPARAM lParam ) { x = ((PI_int32)(short)LOWORD(lParam)); y = ((PI_int32)(short)HIWORD(lParam)); }
#endif	//	WIN32
		inline CPoint( PI_int32 nX, PI_int32 nY ) { x = nX; y = nY; }
		inline CPoint( const POINT &pt ) { x = pt.x; y = pt.y; }
		inline bool operator == ( const CPoint &rhs ) const { return x == rhs.x && y == rhs.y; }
		inline bool operator != ( const CPoint &rhs ) const { return x != rhs.x || y != rhs.y; }
		inline operator LPPOINT () { return this; }
	};


	class CCriticalSection
	//
	//	Simple crtical section handler/wrapper
	{
	public:
		inline CCriticalSection()	{ ::InitializeCriticalSection(&m_sect); }
		inline ~CCriticalSection() { ::DeleteCriticalSection(&m_sect); }

		//	Blocking lock.
		inline void Lock()			{ ::EnterCriticalSection(&m_sect); }
		//	Unlock
		inline void Unlock()		{ ::LeaveCriticalSection(&m_sect); }

		class CLock
		//
		//	Simple lock class for the critcal section
		{
		public:
			inline CLock( CCriticalSection &sect ) : m_sect( sect ) { m_sect.Lock(); }
			inline ~CLock() { m_sect.Unlock(); }
		private:
			CCriticalSection &m_sect;

			CLock();
			CLock( const CLock &);
			CLock& operator =( const CLock &);
		};

	private:
		CRITICAL_SECTION m_sect;

		CCriticalSection( const CCriticalSection & );
		CCriticalSection& operator =( const CCriticalSection & );
	};

#ifdef WIN32
	class CBrush
	//
	//	Wrapper for a brush
	{
	public:
		CBrush( HBRUSH hbr ) : m_hbr( hbr ) {}
		CBrush( COLORREF cr ) : m_hbr( CreateSolidBrush( cr ) ) {}
		
		~CBrush() { DeleteObject( m_hbr ); }
		operator HBRUSH() { return m_hbr; }

		HBRUSH m_hbr;
	};


	class CPen
	//
	//	Wrapper for a brush
	{
	public:
		CPen( HPEN h ) : m_pen( h ) {}
		CPen( COLORREF cr, int nWidth = 1, int nStyle = PS_SOLID ) : m_pen( CreatePen( nStyle, nWidth, cr ) ) {}
		
		~CPen() { DeleteObject( m_pen ); }
		operator HPEN() { return m_pen; }

		HPEN m_pen;
	};

	
	class CWindowDC
	//
	//	Wrapper to get and release a window DC
	{
	public:
		CWindowDC( HWND hwnd )
			: m_hdc( ::GetDC( hwnd ) )
			, m_hwnd( hwnd )
			{}

		~CWindowDC() { ::ReleaseDC( m_hwnd, m_hdc ); }
		operator HDC() const { return m_hdc; }
	private:
		HWND m_hwnd;
		HDC m_hdc;
	};


	class CSaveDCObject
	//
	//	Simple class to 
	{
	public:
		CSaveDCObject( HDC hdc, HGDIOBJ h )
			: m_hOld( SelectObject( hdc, h ) )
			, m_hdc( hdc )
			{}
		~CSaveDCObject()
		{
			SelectObject( m_hdc, m_hOld );
		}

	private:
		HDC m_hdc;
		HGDIOBJ m_hOld;
	};

	class CScrollInfo : public tagSCROLLINFO
	{
	public:
		CScrollInfo( PI_uint32 fPassedMask ) { cbSize = sizeof( tagSCROLLINFO ); fMask = fPassedMask; }
	};



	inline bool IsShiftPressed()
	{
		return GetKeyState(VK_SHIFT) & 0x8000 ? true : false;
	}

	inline bool IsAltPressed()
	{
		return GetKeyState(VK_MENU) & 0x8000 ? true : false;
	}

	inline bool IsControlPressed()
	{
		return GetKeyState(VK_CONTROL) & 0x8000 ? true : false;
	}

	inline HICON LoadIcon16x16( HINSTANCE hInst, PI_uint32 uID )
	//
	//	Load a 16x16 icon from the same resource as the other size icons.
	{
		return reinterpret_cast<HICON>( ::LoadImage( hInst, MAKEINTRESOURCE( uID ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ) );
	}


	class CDeferWindowPos
	//
	//	Wrapper for the Begin, Defer and End WindowPos functions. Nothing glamorous.
	{
	public:
		inline CDeferWindowPos( const PI_int32 nWindows = 1 ) : m_hdlDef( ::BeginDeferWindowPos( nWindows ) ) {}
		inline ~CDeferWindowPos() { VERIFY( ::EndDeferWindowPos( m_hdlDef ) ); }
		inline HDWP DeferWindowPos( HWND hWnd, HWND hWndInsertAfter , PI_int32 x, PI_int32 y, PI_int32 cx, PI_int32 cy, PI_uint32 uFlags )
		{
			return ::DeferWindowPos( m_hdlDef, hWnd, hWndInsertAfter, x, y, cx, cy, uFlags );
		}
		inline HDWP DeferWindowPos( HWND hWnd, HWND hWndInsertAfter, const CRect &rc, PI_uint32 uFlags )
		{
			return ::DeferWindowPos( m_hdlDef, hWnd, hWndInsertAfter, rc.left, rc.top, rc.Width(), rc.Height(), uFlags );
		}

	private:
		HDWP m_hdlDef;
	};
#endif	//	WIN32

}	//	WinHelper

#define BEGINTHREAD( pfnStartAddr, pvParam, pdwThreadID ) \
		 ((HANDLE) _beginthreadex(                     \
				(void *) (0),                            \
				(unsigned) (0),                      \
				(unsigned (__stdcall *) (void *))pfnStartAddr,            \
				(void *) (pvParam),                        \
				(unsigned) (0),                    \
				(unsigned *) (pdwThreadID))) 

#endif //WINHELPER_H
