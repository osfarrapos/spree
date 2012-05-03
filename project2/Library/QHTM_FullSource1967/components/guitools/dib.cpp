/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	DIB.cpp
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "DIB.h"
#include "CompatibleDC.h"

using namespace GS;

CDIB::CDIB( HDC hdc, int cx, int cy )
{

	ZeroMemory( &m_bi, sizeof( m_bi ) );
	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
	m_bi.bmiHeader.biWidth = cx;
	m_bi.bmiHeader.biHeight = -cy;
	m_bi.bmiHeader.biPlanes = 1;
	m_bi.bmiHeader.biBitCount = g_knBPP;
	m_bi.bmiHeader.biCompression = BI_RGB;
	m_bi.bmiHeader.biSizeImage = cx * (( cy * g_knBPP / 8 + 3 ) & ~ 3 );

	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );
	ASSERT( m_hBitmap );

	Initialise();
}


CDIB::CDIB( HDC hdc, int x, int y, int cx, int cy )
{
	ZeroMemory( &m_bi, sizeof( m_bi ) );
	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
	m_bi.bmiHeader.biWidth = cx;
	m_bi.bmiHeader.biHeight = -cy;
	m_bi.bmiHeader.biPlanes = 1;
	m_bi.bmiHeader.biBitCount = g_knBPP;
	m_bi.bmiHeader.biCompression = BI_RGB;
	m_bi.bmiHeader.biSizeImage = cx * (( cy * g_knBPP / 8 + 3 ) & ~ 3 );

	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );
	ASSERT( m_hBitmap );

	Initialise();

	//
	//	Copy the DC into the DIB.
	CCompatibleDC hdcMem( hdc );

	HGDIOBJ hOld = SelectObject( hdcMem, m_hBitmap );
	VERIFY( BitBlt( hdcMem, 0, 0, cx, cy, hdc, x, y, SRCCOPY ) );
	//TextOut( hdcMem, 0, 0, "hello", 5 );
	SelectObject( hdcMem, hOld );
}


CDIB::CDIB( HBITMAP hbm )
{
	BITMAP bm;
	GetObject( hbm, sizeof( bm ), &bm );

	ZeroMemory( &m_bi, sizeof( m_bi ) );
	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
	m_bi.bmiHeader.biWidth = bm.bmWidth;
	m_bi.bmiHeader.biHeight = -bm.bmHeight;
	m_bi.bmiHeader.biPlanes = 1;
	m_bi.bmiHeader.biBitCount = g_knBPP;
	m_bi.bmiHeader.biCompression = BI_RGB;
	m_bi.bmiHeader.biSizeImage = bm.bmWidth * (( bm.bmHeight * g_knBPP / 8 + 3 ) & ~ 3 );

	HDC hdc = GetDC( NULL );
	ASSERT( hdc );
	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	ASSERT( m_hBitmap );
	Initialise();

	//
	//	Copy the DC into the DIB.
	{
		CCompatibleDC hdcMemDest( hdc );
		CCompatibleDC hdcMemSource( hdc );

		HGDIOBJ hOld = SelectObject( hdcMemDest, m_hBitmap );
		HGDIOBJ hOld2 = SelectObject( hdcMemSource, hbm );
		VERIFY( BitBlt( hdcMemDest, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemSource, 0, 0, SRCCOPY ) );
		SelectObject( hdcMemDest, hOld );
		SelectObject( hdcMemSource, hOld2 );
	}

	VERIFY( ReleaseDC( NULL, hdc ) );
}


CDIB::CDIB( CDIB &dib, int x, int y, int cx, int cy )
{
	ZeroMemory( &m_bi, sizeof( m_bi ) );
	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
	m_bi.bmiHeader.biWidth = cx;
	m_bi.bmiHeader.biHeight = -cy;
	m_bi.bmiHeader.biPlanes = 1;
	m_bi.bmiHeader.biBitCount = g_knBPP;
	m_bi.bmiHeader.biCompression = BI_RGB;
	m_bi.bmiHeader.biSizeImage = cx * (( cy * g_knBPP / 8 + 3 ) & ~ 3 );

	HDC hdc = GetDC( NULL );
	ASSERT( hdc );
	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	ASSERT( m_hBitmap );

	Initialise();

	if( dib.m_bHasAlpha )
	{
		SetHasAlphaChannel( true );
	}

	if( dib.m_bTransparentColorSet )
	{
		m_bTransparentColorSet = dib.m_bTransparentColorSet;
		m_crTransparent = dib.m_crTransparent;
	}

	//
	//	Copy the bits over...

	const UINT nLeft = max( x, 0 );
	const UINT nTop = max( y, 0 );
	const UINT nRight = min( x + cx, dib.GetWidth() );
	const UINT nBottom = min( y + cy, dib.GetHeight() );

	const CLineArray & arrDest = GetLineArray();
	const CLineArray & arrSrc = dib.GetLineArray();

	int nDestLine = 0;
	for(  UINT nLine = nTop; nLine < nBottom; nLine++, nDestLine++ )
	{
		DIB_PIXEL *pDest = arrDest[ nDestLine ];
		const DIB_PIXEL *pSrc = arrSrc[ nLine ];
		pSrc += nLeft;

		for( UINT col = nLeft; col < nRight; col++, pDest++, pSrc++ )
		{
			*pDest = *pSrc;
		}
	}
}


CDIB::CDIB( HICON hIcon )
{
	ICONINFO iconinfo;
	GetIconInfo( hIcon, &iconinfo );
	CDIB dibMask( iconinfo.hbmMask );

	BITMAP bm;
	GetObject( iconinfo.hbmColor, sizeof( bm ), &bm );

	ZeroMemory( &m_bi, sizeof( m_bi ) );
	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
	m_bi.bmiHeader.biWidth = bm.bmWidth;
	m_bi.bmiHeader.biHeight = -bm.bmHeight;
	m_bi.bmiHeader.biPlanes = 1;
	m_bi.bmiHeader.biBitCount = g_knBPP;
	m_bi.bmiHeader.biCompression = BI_RGB;
	m_bi.bmiHeader.biSizeImage = bm.bmWidth * (( bm.bmHeight * g_knBPP / 8 + 3 ) & ~ 3 );

	HDC hdc = GetDC( NULL );
	ASSERT( hdc );
	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	ASSERT( m_hBitmap );
	Initialise();

	//
	//	Copy the DC into the DIB.
	{
		CCompatibleDC hdcMemDest( hdc );
		CCompatibleDC hdcMemSource( hdc );

		HGDIOBJ hOld = SelectObject( hdcMemDest, m_hBitmap );
		HGDIOBJ hOld2 = SelectObject( hdcMemSource, iconinfo.hbmColor );
		VERIFY( BitBlt( hdcMemDest, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemSource, 0, 0, SRCCOPY ) );
		SelectObject( hdcMemDest, hOld );
		SelectObject( hdcMemSource, hOld2 );
	}


	VERIFY( ReleaseDC( NULL, hdc ) );

	const CLineArray & arrDest = GetLineArray();
	const UINT uSize = arrDest.GetSize();
	const CLineArray & arrSrc = GetLineArray();
	for( register UINT u = 0; u < uSize; u++ )
	{
		DIB_PIXEL *pDest = arrDest[ u ];
		DIB_PIXEL *pSrc = arrSrc[ u ];
		for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )
		{
      if( pSrc->rgbBlue != 0 )
			{
				pDest->rgbReserved = 255;
			}
		}
	}
	SetHasAlphaChannel( true );


}


CDIB::~CDIB()
{
	VERIFY( DeleteObject( m_hBitmap ) );
}


void CDIB::Initialise()
//
//	Initialise various attributes of the DIB,mostly for performance reasons.
//	
//	Sets the elements of the line array to point to the start of each line in the
//	DIB data.
{
	m_size.cy = abs( m_bi.bmiHeader.biHeight );
	m_size.cx = m_bi.bmiHeader.biWidth;

	m_arrLine.SetSize( GetHeight() );

	for( int i= GetHeight() - 1; i >=  0; i--)
	{
		m_arrLine[i] = m_pBits + i * GetWidth();
	}

	m_crTransparent = 0;
	m_bTransparentColorSet = false;
	m_bHasAlpha = false;
}


bool CDIB::Draw( HDC hdc, int nX, int nY )
{
	if( m_bTransparentColorSet )
	{
		return DrawTransparent( hdc, nX, nY, m_crTransparent );
	}
	else if( m_bHasAlpha )
	{
		return DrawWithAlphaChannel( hdc, nX, nY );
	}	
	else if( ::SetDIBitsToDevice( hdc, nX, nY, GetWidth(), GetHeight(), 0, 0, 0, GetHeight(), m_pBits, &m_bi, DIB_RGB_COLORS ) != GDI_ERROR )
	{
		return true;
	}
	return false;
}


bool CDIB::BasicDraw( HDC hdc, int nX, int nY )
{
	return ::SetDIBitsToDevice( hdc, nX, nY, GetWidth(), GetHeight(), 0, 0, 0, GetHeight(), m_pBits, &m_bi, DIB_RGB_COLORS ) != GDI_ERROR; 
}



void CDIB::StretchAlphaBlend( HDC hdc, int nX, int nY, int nCX, int nCY, unsigned char nAlpha )
{
	CDIB dib( hdc, nCX, nCY );
	CCompatibleDC hdcMem( hdc );
	SetStretchBltMode( hdcMem, STRETCH_HALFTONE );

	HGDIOBJ hOld = SelectObject( hdcMem, dib.GetBitmap() );
	::StretchDIBits( hdcMem,
              0,
              0,
              nCX,
              nCY,
              0,
              0,
              GetWidth(),
              GetHeight(),
              m_pBits,
              (BITMAPINFO *) &m_bi,
              DIB_RGB_COLORS,
              SRCCOPY );

	SelectObject( hdcMem, hOld );

	dib.AlphaBlend( hdc, nX, nY, nAlpha );
}


bool CDIB::StretchDrawTransparent( HDC hdc, int nX, int nY, int nCX, int nCY, COLORREF crTransparent )
{
	CDIB dib( hdc, nCX, nCY );
	CCompatibleDC hdcMem( hdc );

	HGDIOBJ hOld = SelectObject( hdcMem, dib.GetBitmap() );
	SetStretchBltMode( hdcMem, STRETCH_HALFTONE );
	::StretchDIBits( hdcMem,
              0,                        // Destination x
              0,                        // Destination y
              nCX,	                // Destination width
              nCY,                // Destination height
              0,                        // Source x
              0,                        // Source y
              GetWidth(),									// Source width
              GetHeight(),	  						// Source height
              m_pBits,								// Pointer to bits
              (BITMAPINFO *) &m_bi,    // BITMAPINFO
              DIB_RGB_COLORS,           // Options
              SRCCOPY );

	SelectObject( hdcMem, hOld );

	return dib.DrawTransparent( hdc, nX, nY, crTransparent );
}


void CDIB::AddImage( UINT nLeft, UINT nTop, CDIB *pDib )
{
	const CLineArray & arrThis = GetLineArray();
	const CLineArray & arr = pDib->GetLineArray();

	const UINT uDibHeight = pDib->GetHeight();
	const UINT uDibWidth = pDib->GetWidth();

	if( pDib->m_bTransparentColorSet )
	{
		BYTE bRed, bGreen, bBlue;
		bRed = GetRValue( pDib->m_crTransparent );
		bGreen = GetGValue( pDib->m_crTransparent );
		bBlue = GetBValue( pDib->m_crTransparent );

		for( UINT uRow = 0; uRow < uDibHeight; uRow++ )
		{
			const DIB_PIXEL *pSourceLine = (RGBQUAD *)arr[ uRow ];
			DIB_PIXEL *pDestLine = (RGBQUAD *)arrThis[ uRow + nTop ];
			for( UINT uCol = 0; uCol < uDibWidth; uCol++ )
			{
				const DIB_PIXEL *pSource = pSourceLine + uCol;
				RGBQUAD *pDest = pDestLine + (uCol + nLeft);
				if( pSource->rgbRed != bRed || pSource->rgbGreen != bGreen || pSource->rgbBlue != bBlue )
				{
					*pDest = *pSource;
				}
			}
		}
	}
	else
	{
		for( UINT uRow = 0; uRow < uDibHeight; uRow++ )
		{
			const DIB_PIXEL *pSourceLine = (RGBQUAD *)arr[ uRow ];
			DIB_PIXEL *pDestLine = (RGBQUAD *)arrThis[ uRow + nTop ];
			for( UINT uCol = 0; uCol < uDibWidth; uCol++ )
			{
				const DIB_PIXEL *pSource = pSourceLine + uCol;
				RGBQUAD *pDest = pDestLine + (uCol + nLeft);
				*pDest = *pSource;
			}
		}
	}
}


bool CDIB::Draw( HDC hdc, int nX, int nY, int nRight, int nBottom )
{
	const int nWidth = nRight - nX;
	const int nHeight = nBottom - nY;
	if( nWidth && nHeight )
	{

		SetStretchBltMode( hdc, STRETCH_HALFTONE );
		if( m_bTransparentColorSet )
		{
			if( nWidth != GetWidth() || nHeight != GetHeight() )
			{
				return StretchDrawTransparent( hdc, nX, nY, nWidth, nHeight, m_crTransparent );
			}
			return DrawTransparent( hdc, nX, nY, m_crTransparent );
		}
		else if( m_bHasAlpha )
		{
			if( nWidth != GetWidth() || nHeight != GetHeight() )
			{
				CDIB *pDib = CreateResized( nRight - nX, nBottom - nY );
				pDib->DrawWithAlphaChannel( hdc, nX, nY );
				delete pDib;
				return true;
			}
			return DrawWithAlphaChannel( hdc, nX, nY );
		}	
		else if( ::StretchDIBits(hdc, nX, nY, nWidth, nHeight, 0, 0, GetWidth(), GetHeight(), m_pBits, &m_bi, DIB_RGB_COLORS, SRCCOPY ) != GDI_ERROR )
		{
			return true;
		}
	}
	return false;
}

