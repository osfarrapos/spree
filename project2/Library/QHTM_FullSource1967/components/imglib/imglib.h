/*----------------------------------------------------------------------
Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	ImgLib.h
Owner:	russf@gipsysoft.com
Purpose:	Imaging library.
----------------------------------------------------------------------*/
#ifndef IMGLIB_H
#define IMGLIB_H

#ifndef _WINDOWS_
	#include <Windows.h>
#endif	//	_WINDOWS_

#ifdef __cplusplus

	#ifndef DIB_H
		#include <guitools/DIB.h>
	#endif	//	DIB_H

	//
	//	Forward declares
	class CDataSourceABC;

	//
	//	Types

	class CImage
	//
	//	Main image class.
	{
	public:
		CImage();
		virtual ~CImage();

		//	Load from data source
		bool Load( CDataSourceABC &ds );

		//	Unload all frame data
		void UnloadFrames();

		BOOL DrawFrame( UINT nFrame, HDC hdc, int left, int top ) const;
		BOOL StretchFrame( UINT nFrame, HDC hdc, int left, int top, int right, int bottom ) const;

		UINT GetFrameCount() const;

		int GetFrameTime( UINT nFrame ) const;
		const SIZE &GetFrameSize( UINT nFrame ) const;

		const SIZE &GetSize() const;

		GS::CDIB * GetFrameDIB( UINT nFrame );

		CImage *CreateCopy() const;

		CImage *CreateSubImage( UINT uFrame, UINT x, UINT y, UINT cx, UINT cy ) const;

		//	Create this object from a standard windows bitmap
		bool CreateFromBitmap( HBITMAP hbmp );
		bool CreateFromIcon( HICON hIcon );

		void ForceTransparent( COLORREF crTransparent );

		void Destroy();

	protected:
		CImage( class CFrameData *m_pFrameData );

		class CFrameData *m_pFrameData;

	private:
		CImage( const CImage &);
		CImage &operator = ( const CImage &);
	};
#else	//	__cplusplus
	#error C++ compiler required.
#endif	//	__cplusplus

#endif //	IMGLIB_H