   // XHTMLWrapper.cpp: implementation of the _XHTMLWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XHTMLWrapper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _XDEF_ENABLEQHTMWRAPPER

_XHTMLWrapper::_XHTMLWrapper()
{
	m_pHTMLTexture = NULL;
	m_uBufferWidth = 256;
	m_hDIB		   = NULL;
	m_LastBuildScrollPos = -1;
	m_uViewHeight  = 256;
	memset( m_LastBuildURLAdress, 0, sizeof(TCHAR) * _XDEF_MAX_URLSTRINGLENGTH );
}

_XHTMLWrapper::~_XHTMLWrapper()
{
	SAFE_RELEASE( m_pHTMLTexture );
	if( m_hDIB )
	{
		GlobalFree( m_hDIB );
	}
}

BOOL _XHTMLWrapper::InitializeQHTMLibrary( void )
{
#ifdef _XDEF_ENABLEQHTMWRAPPER

	if( !QHTM_Initialize( gHInst ) )
	{
		_XFatalError( "Can't initialize help system" );
		return FALSE;
	}
	//QHTM_EnableCooltips();
#endif

	return TRUE;
}

void _XHTMLWrapper::ReleaseQHTMLibrary( void )
{

#ifdef _XDEF_ENABLEQHTMWRAPPER
	QHTM_Uninitialize();
#endif

}

BOOL _XHTMLWrapper::Initialize( int buffersize )
{
	m_uBufferWidth = buffersize;		

	Release();

	if( FAILED( gpDev->CreateTexture( m_uBufferWidth, m_uBufferWidth, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pHTMLTexture, NULL ) ) )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL _XHTMLWrapper::RestoreDeviceObject( void )
{
	SAFE_RELEASE( m_pHTMLTexture );

	if( FAILED( gpDev->CreateTexture( m_uBufferWidth, m_uBufferWidth, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pHTMLTexture, NULL ) ) )
	{
		return FALSE;
	}
	
	int curscrollpos = m_LastBuildScrollPos;
	m_LastBuildScrollPos = -1;
	UpdateTextureBuffer( curscrollpos );

	return TRUE;
}

void _XHTMLWrapper::Release( void )
{
	SAFE_RELEASE( m_pHTMLTexture );
}

BOOL CheckExistFile( LPCSTR filename )
{
	HANDLE      hFind;
	WIN32_FIND_DATA rFind;	

	hFind = FindFirstFile ( filename, &rFind );

	if ( INVALID_HANDLE_VALUE == hFind )
	{
		FindClose ( hFind );
		return FALSE;
	}

	FindClose ( hFind );

	return TRUE;
}


BOOL _XHTMLWrapper::LoadHTML( LPTSTR szURLString, int imaxWidth )
{

#ifndef _XDEF_ENABLEQHTMWRAPPER
	return TRUE;
#endif

	TCHAR urlstring[_XDEF_MAX_URLSTRINGLENGTH];
	memset( urlstring, 0, sizeof( TCHAR ) * _XDEF_MAX_URLSTRINGLENGTH );

	if( !CheckExistFile( szURLString ) )
	{
		strcpy( urlstring, gModulePath );
		strcat( urlstring, _T("\\HelpSystem\\error.html") );

		if( !CheckExistFile( urlstring ) )
		{
			return FALSE;
		}
	}
	else
	{
		strcpy( urlstring, szURLString );
	}

	if( strcmpi( m_LastBuildURLAdress, urlstring ) == 0 )
	{
		return TRUE;
	}

	memset( m_LastBuildURLAdress, 0, sizeof(TCHAR) * _XDEF_MAX_URLSTRINGLENGTH );
	strcpy( m_LastBuildURLAdress, urlstring );
	m_LastBuildScrollPos = -1;

	HDC hdcWindow = ::GetDC( gHWnd );
	HDC hdcDraw   = ::CreateCompatibleDC( hdcWindow );

	if( QHTM_GetHTMLHeight( hdcDraw, urlstring, NULL, QHTM_SOURCE_FILENAME, imaxWidth, &m_uMaxHeight ) )
	{
		LPBYTE mImage;
		
		BITMAPINFO bitmapinfo;		
		bitmapinfo.bmiHeader.biSize			 = sizeof(bitmapinfo.bmiHeader);
		bitmapinfo.bmiHeader.biWidth		 = imaxWidth;
		bitmapinfo.bmiHeader.biHeight		 = -m_uMaxHeight;
		bitmapinfo.bmiHeader.biPlanes		 = 1;
		bitmapinfo.bmiHeader.biBitCount		 = 32;
		bitmapinfo.bmiHeader.biCompression	 = BI_RGB;
		bitmapinfo.bmiHeader.biSizeImage	 = 0;
		bitmapinfo.bmiHeader.biXPelsPerMeter = 100;
		bitmapinfo.bmiHeader.biYPelsPerMeter = 100;
		bitmapinfo.bmiHeader.biClrUsed	     = 0;
		bitmapinfo.bmiHeader.biClrImportant	 = 0;		
		HBITMAP hBitmap = CreateDIBSection(NULL, &bitmapinfo, DIB_RGB_COLORS, (PVOID*)&mImage, NULL, 0);
		
		if( hBitmap )
		{
			//	Select the bitmap and render the HTML onto it.
			hBitmap = (HBITMAP)::SelectObject( hdcDraw, (HGDIOBJ)hBitmap );
			if( QHTM_RenderHTML( hdcDraw, urlstring, NULL, QHTM_SOURCE_FILENAME, imaxWidth ) )
			{
				//	Deselect the bitmap
				hBitmap = (HBITMAP)::SelectObject( hdcDraw, (HGDIOBJ)hBitmap );
				if( !DDBToDIB( hBitmap ) )
				{
					(void)::DeleteObject( hBitmap );
					(void)::ReleaseDC( gHWnd, hdcWindow );
					(void)::DeleteDC( hdcDraw );
					memset( m_LastBuildURLAdress, 0, sizeof(TCHAR) * _XDEF_MAX_URLSTRINGLENGTH );
					return FALSE;
				}
			}
		
			(void)::DeleteObject( hBitmap );
		}
		else
		{
			(void)::ReleaseDC( gHWnd, hdcWindow );
			(void)::DeleteDC( hdcDraw );
			memset( m_LastBuildURLAdress, 0, sizeof(TCHAR) * _XDEF_MAX_URLSTRINGLENGTH );
			return FALSE;
		}		
	}
	else
	{
		(void)::ReleaseDC( gHWnd, hdcWindow );
		(void)::DeleteDC( hdcDraw );
		memset( m_LastBuildURLAdress, 0, sizeof(TCHAR) * _XDEF_MAX_URLSTRINGLENGTH );
		return FALSE;
	}

	(void)::ReleaseDC( gHWnd, hdcWindow );
	(void)::DeleteDC( hdcDraw );

	return TRUE;
}


BOOL _XHTMLWrapper::DDBToDIB( HGDIOBJ hbmp )
{
	BITMAP                  bm;
    BITMAPINFOHEADER        bi;
    LPBITMAPINFOHEADER      lpbi;
    DWORD                   dwLen;
    HANDLE                  handle = NULL;
    HDC                     hDC;

    // Get bitmap information
    (void)GetObject( hbmp, sizeof(bm), (LPSTR)&bm );

    // Initialize the bitmapinfoheader
    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = static_cast<USHORT>( bm.bmPlanes * bm.bmBitsPixel );
    bi.biCompression        = BI_RGB;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    // Compute the size of the  infoheader and the color table
    int nColors = (1 << bi.biBitCount);
    if( nColors > 256 ) nColors = 0;
    dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

    // We need a device context to get the DIB from
    hDC = GetDC(NULL);
    
	// Allocate enough memory to hold bitmapinfoheader and color table

	if( m_hDIB )
	{
		GlobalFree( m_hDIB );
		m_hDIB = NULL;
	}

    m_hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

    if (!m_hDIB)
	{
            ReleaseDC(NULL,hDC);
            return FALSE;
    }

    lpbi = (LPBITMAPINFOHEADER)m_hDIB;

    *lpbi = bi;

    // Call GetDIBits with a NULL lpBits param, so the device driver 
    // will calculate the biSizeImage field 
    (void)GetDIBits(hDC, (HBITMAP)hbmp, 0L, (DWORD)bi.biHeight,
                    (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

    bi = *lpbi;

    // If the driver did not fill in the biSizeImage field, then compute it
    // Each scan line of the image is aligned on a DWORD (32bit) boundary
    if(bi.biSizeImage == 0)
	{
        bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) * bi.biHeight;
    }

    // Realloc the buffer so that it can hold all the bits
    dwLen += bi.biSizeImage;
			handle = GlobalReAlloc(m_hDIB, dwLen, GMEM_MOVEABLE);
    if( handle )
	{
            m_hDIB = handle;
	}
    else
	{
            GlobalFree(m_hDIB);            
            ReleaseDC(NULL,hDC);
            return FALSE;
    }

    // Get the bitmap bits
    lpbi = (LPBITMAPINFOHEADER)m_hDIB;

	// address for bitmap bits
	LPBYTE	pBitmapImage = (LPBYTE)lpbi + (bi.biSize + nColors * sizeof(RGBQUAD));

    // FINALLY get the DIB
    BOOL bGotBits = GetDIBits( hDC, (HBITMAP)hbmp,
                            0L,                             // Start scan line
                            (DWORD)bi.biHeight,             // # of scan lines
                            pBitmapImage,
                            (LPBITMAPINFO)lpbi,             // address of bitmapinfo
                            (DWORD)DIB_RGB_COLORS);         // Use RGB for color table

    if( !bGotBits )
    {
        GlobalFree(m_hDIB);
        ReleaseDC(NULL,hDC);
        return FALSE;
    }

    ReleaseDC(NULL,hDC);

	LPRGBQUAD pBitmapPtr = (LPRGBQUAD)pBitmapImage;
	DWORD     imagesize = lpbi->biWidth * lpbi->biHeight;
	for( int i = 0; i < imagesize; ++i )
	{
		pBitmapPtr->rgbReserved = 0xFF;
		pBitmapPtr++;
	}

	return TRUE;
}

BOOL _XHTMLWrapper::UpdateTextureBuffer( int ScrollPos )
{

#ifndef _XDEF_ENABLEQHTMWRAPPER
	return TRUE;
#endif

	if( !m_pHTMLTexture ) return FALSE;
	if( !m_hDIB ) return FALSE;

	if( m_LastBuildScrollPos == ScrollPos )
	{
		return TRUE;
	}

	m_LastBuildScrollPos = ScrollPos;

	LPBITMAPINFOHEADER lpbi			= (LPBITMAPINFOHEADER)m_hDIB;
	LPBYTE			   pBitmapImage = (LPBYTE)lpbi + (lpbi->biSize + (1 << lpbi->biBitCount) * sizeof(RGBQUAD));

	D3DLOCKED_RECT  lockrect;
	D3DSURFACE_DESC surfdesc;
	m_pHTMLTexture->GetLevelDesc( 0, &surfdesc );
	if( SUCCEEDED( m_pHTMLTexture->LockRect( 0, &lockrect, 0, 0 ) ) )
	{
		DWORD bitmaplinewidth = (lpbi->biWidth<<2);// * 4;
		DWORD copywidth = ( lockrect.Pitch > bitmaplinewidth ) ? bitmaplinewidth : lockrect.Pitch;
		//DWORD copyheight = ( surfdesc.Height > lpbi->biHeight ) ? lpbi->biHeight : surfdesc.Height;
		DWORD copyheight = ( m_uViewHeight > lpbi->biHeight ) ? lpbi->biHeight : m_uViewHeight;
		
		DWORD scrollpoint = 0;
		
		if( ScrollPos > lpbi->biHeight - copyheight ) ScrollPos = lpbi->biHeight - copyheight;
		
		LPBYTE pTarget = (LPBYTE)lockrect.pBits;
		LPBYTE pSource = pBitmapImage + (lpbi->biHeight-1-ScrollPos) * bitmaplinewidth;

		memset( pTarget, 0xFFFFFFFF, lockrect.Pitch * surfdesc.Height );
		
		for( int i = 0; i < copyheight; ++i )
		{
			memcpy( pTarget, pSource, copywidth );
			pTarget+=lockrect.Pitch;
			pSource-=bitmaplinewidth;
		}
		
		m_pHTMLTexture->UnlockRect(0);	
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}