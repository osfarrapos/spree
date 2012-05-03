/*----------------------------------------------------------------------
Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	Image.cpp
Owner:	russf@gipsysoft.com
Purpose:	An image, can consist of many frames.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include <Array.h>
#include "ImgLib.h"
#include "ImgLibInt.h"
#include "Config.h"
#include <ZLib\ZLib.h>
#include <DataSource\DataSourceABC.h>

#ifdef IMGLIB_MNG
	extern bool DecodeMNG( CDataSourceABC &ds, CFrameArray &arrFrames, SIZE &size );
#endif	//	IMGLIB_MNG

#ifdef IMGLIB_PNG
	extern bool DecodePNG( CDataSourceABC &ds, CFrameArray &arrFrames, SIZE &size );
#endif	//	IMGLIB_PNG

#ifdef IMGLIB_JPG
	extern bool DecodeJPEG( CDataSourceABC &ds, CFrameArray &arrFrames, SIZE &size );
#endif	//	IMGLIB_JPG

#ifdef IMGLIB_GIF
	extern bool DecodeGIF( CDataSourceABC &ds, CFrameArray &arrFrames, SIZE &size );
#endif	//	IMGLIB_GIF

#ifdef IMGLIB_PCX
	extern bool DecodePCX( CDataSourceABC &ds, CFrameArray &arrFrames, SIZE &size );
#endif	//	IMGLIB_PCX

#ifdef IMGLIB_BMP
	extern bool DecodeBMP( CDataSourceABC &ds, CFrameArray &arrFrames, SIZE &size );
#endif	//	IMGLIB_BMP

#ifdef IMGLIB_ICO
	extern bool DecodeICO(CDataSourceABC &ds, CFrameArray &arrFrames, SIZE &size );
#endif	//	IMGLIB_ICO


#ifdef TRACING
	#define ITRACE TRACE
#else	//	TRACING
	#define ITRACE 
#endif	//	TRACING

class CFrameData
{
public:
	CFrameData()
		: m_nRefCount( 0 )
		{
			m_size.cx = m_size.cy = 0;
		}

	inline const GS::CDIB *GetFrameImage( int nFrame ) const
	{
		return m_arrFrames[ nFrame ]->GetDib();
	}

	inline GS::CDIB *GetFrameImage( int nFrame )
	{
		return m_arrFrames[ nFrame ]->GetDib();
	}

	void CleanupFrames()
		{
			m_size.cx = m_size.cy = 0;

			for( UINT u = 0; u < m_arrFrames.GetSize(); u++ )
			{
				delete m_arrFrames[ u ];
			}
			m_arrFrames.RemoveAll();
		}

	int GetRefCount() { return m_nRefCount; }

	void AddRef() {	m_nRefCount++; }

	void Release()
		{
			m_nRefCount--;
			if( !m_nRefCount )
				delete this;
		}

	SIZE m_size;
	CFrameArray	m_arrFrames;
private:
	~CFrameData()
		{
			CleanupFrames();
		}

	int m_nRefCount;
};


CImage::CImage( CFrameData *pFrameData )
	: m_pFrameData( pFrameData )
{
	m_pFrameData->AddRef();
}


CImage::CImage()
	: m_pFrameData( new CFrameData )
{
	m_pFrameData->AddRef();
}


CImage::~CImage()
{
	m_pFrameData->Release();
}


void CImage::UnloadFrames()
{
	if( m_pFrameData->GetRefCount() == 1 )
	{
		m_pFrameData->CleanupFrames();
	}
	else
	{
		m_pFrameData->Release();
		m_pFrameData = new CFrameData;
		m_pFrameData->AddRef();
	}
}


bool CImage::Load( CDataSourceABC &ds )
{
	UnloadFrames();
	//
	//	Read the first 8 bytes of the image file to see if we can determine what image format
	//	we have
	BYTE bMagicNumber[ 8 ];
	if( ds.ReadBytes( bMagicNumber, 8 ) )
	{
#ifdef IMGLIB_MNG
    if( !memcmp( bMagicNumber, "\212MNG\r\n\032\n", 8 ) )
		{
			return DecodeMNG( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
		}
		else 
#endif	//	IMGLIB_MNG

#ifdef IMGLIB_PNG
		if( !memcmp( bMagicNumber, "\211PNG\r\n\032\n", 8 ) )
		{
			return DecodePNG( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
		}
		else 
#endif	//	IMGLIB_PNG

#ifdef IMGLIB_GIF
		if( !memcmp( bMagicNumber, "GIF89a", 6 ) || !memcmp( bMagicNumber, "GIF87a", 6 ) )
		{
			ds.Reset();
			return DecodeGIF( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
		}
		else 
#endif	//	IMGLIB_GIF

#ifdef IMGLIB_JPG
		if( !memcmp( bMagicNumber, "\xFF\xD8", 2 ) )
		{
			ds.Reset();
			return DecodeJPEG( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
		}
		else 
#endif	//	IMGLIB_JPG

#ifdef IMGLIB_BMP
		if( !memcmp( bMagicNumber, "BM", 2 ) )
		{
			ds.Reset();
			return DecodeBMP( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
		}
		else
#endif	//	IMGLIB_PCX


#ifdef IMGLIB_ICO
		if( !memcmp( bMagicNumber, "\x00\x00\x01\x00", 1 ) )
		{
			ds.Reset();
			return DecodeICO( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
		}
		else
#endif	//	IMGLIB_ICO

#ifdef IMGLIB_PCX
		if( !memcmp( bMagicNumber, "\x0A", 1 ) )
		{
			ds.Reset();
			return DecodePCX( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
		}
		else
#endif	//	IMGLIB_PCX

		{
#ifdef IMGLIB_BMP
			ITRACE(_T("Unknown format, trying bitmap just in case\n"));
			return DecodeBMP( ds, m_pFrameData->m_arrFrames, m_pFrameData->m_size );
#endif	//	IMGLIB_BMP
		}
		
	}
	else
	{
		ITRACE(_T("Could not read enough signature\n") );
	}
	return false;
}


BOOL CImage::StretchFrame( UINT nFrame, HDC hdc, int left, int top, int right, int bottom ) const
{
	if( GetFrameCount() && nFrame <= GetFrameCount() )
	{
		GS::CDIB *pDib = m_pFrameData->GetFrameImage( nFrame );

		return pDib->Draw( hdc, left, top, right, bottom  );
	}
	return false;
}


BOOL CImage::DrawFrame( UINT nFrame, HDC hdc, int x, int y ) const
{
	if( GetFrameCount() && nFrame <= GetFrameCount() )
	{
		return m_pFrameData->GetFrameImage( nFrame )->Draw( hdc, x, y );
	}
	return false;
}


CImage * CImage::CreateCopy() const
{
	CImage *pImage = new CImage( m_pFrameData );
	return pImage;
}


UINT CImage::GetFrameCount() const
{
	return m_pFrameData->m_arrFrames.GetSize();
}

int CImage::GetFrameTime( UINT nFrame ) const
//
//	Get the number of milliseconds a frame should be displayed for.
{
	return m_pFrameData->m_arrFrames[ nFrame ]->GetTime();
}

const SIZE &CImage::GetFrameSize( UINT nFrame ) const
{
	return m_pFrameData->GetFrameImage( nFrame )->GetSize();
}


const SIZE & CImage::GetSize() const
{
	return m_pFrameData->m_size;
}


void CImage::ForceTransparent( COLORREF crTransparent )
{
	for( UINT u = 0; u < GetFrameCount(); u++ )
	{
		m_pFrameData->GetFrameImage( u )->SetColourTransparent( crTransparent );
	}	
}


bool CImage::CreateFromBitmap( HBITMAP hbmp )
{
	GS::CDIB *pDib = new GS::CDIB( hbmp );	
	CFrame *pFrame = new CFrame( pDib, 0 );
	m_pFrameData->m_arrFrames.Add( pFrame );
	m_pFrameData->m_size.cx = pDib->GetWidth();
	m_pFrameData->m_size.cy = pDib->GetHeight();
	return true;	
}


bool CImage::CreateFromIcon( HICON hIcon )
{
	GS::CDIB *pDib = new GS::CDIB( hIcon );
	CFrame *pFrame = new CFrame( pDib, 0 );
	m_pFrameData->m_arrFrames.Add( pFrame );
	m_pFrameData->m_size.cx = pDib->GetWidth();
	m_pFrameData->m_size.cy = pDib->GetHeight();
	return true;	
}


void CImage::Destroy()
{
	delete this;
}


GS::CDIB * CImage::GetFrameDIB( UINT nFrame )
{
	if( GetFrameCount() && nFrame <= GetFrameCount() )
	{
		GS::CDIB *pDib = m_pFrameData->GetFrameImage( nFrame );

		return pDib;
	}
	return 0;
}


CImage *CImage::CreateSubImage( UINT uFrame, UINT x, UINT y, UINT cx, UINT cy ) const
{
	GS::CDIB *pDibSrc = m_pFrameData->GetFrameImage( uFrame );
	GS::CDIB *pDib = new GS::CDIB( *pDibSrc, x, y, cx, cy );

	CFrame *pFrame = new CFrame( pDib, 1000 );
	CImage *pImage = new CImage;
	pImage->m_pFrameData->m_arrFrames.Add( pFrame );

	return pImage;
}