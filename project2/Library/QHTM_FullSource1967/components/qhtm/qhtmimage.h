/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	QHTMImage.h
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#ifndef QHTMIMAGE_H
#define QHTMIMAGE_H

#ifndef QHTM_H
	#include "QHTM.h"
#endif	//	QHTM_H

#ifndef IMGLIB_H
	#include <ImgLib/ImgLib.h>
#endif	//	IMGLIB_H

class CQHTMImage : public CImage, public CQHTMImageABC
{
public:
	CQHTMImage();
	virtual ~CQHTMImage();

	inline BOOL DrawFrame( UINT nFrame, HDC hdc, int left, int top ) const
	{
		return CImage::DrawFrame( nFrame, hdc, left, top );
	}

	inline BOOL StretchFrame( UINT nFrame, HDC hdc, int left, int top, int right, int bottom ) const
	{
		return CImage::StretchFrame( nFrame, hdc, left, top, right, bottom );
	}

	inline UINT GetFrameCount() const
	{
		return CImage::GetFrameCount();
	}

	inline const SIZE &GetSize() const
	{
		return CImage::GetSize();
	}

	inline int GetFrameTime( UINT nFrame ) const
	{
		return CImage::GetFrameTime( nFrame );
	}

	inline void Destroy()
	{
		CImage::Destroy();
	}

private:
	CQHTMImage( const CQHTMImage &rhs );
	CQHTMImage & operator = ( const CQHTMImage &rhs );
};

#endif //QHTMIMAGE_H