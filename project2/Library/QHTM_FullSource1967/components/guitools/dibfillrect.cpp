/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	DIBFillRect.cpp
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "DIB.h"

using namespace GS;

#define Blend( dest, negalpha, src, alpha) dest = static_cast<unsigned char>( ( dest * negalpha + src ) / 256 );
//#define Blend( dest, negalpha, src, alpha) dest = static_cast<unsigned char>( ((src*(alpha*256/255)) + (dest*(256-(a*256/255)))) / 256 );

void CDIB::FillRect( const RECT &rc, COLORREFA cr )
{
	const UINT nTop = max( rc.top, 0 );
	register const UINT nBottom = min( rc.bottom, GetHeight() );
	const UINT nLeft = max( rc.left, 0 );
	register const UINT nRight = min( rc.right, GetWidth() );

	const UINT a = GetAValue( cr );
	const UINT r = GetRValue( cr ) * a;
	const UINT g = GetGValue( cr ) * a;
	const UINT b = GetBValue( cr ) * a;
	

	register int nNegativeAplha = 255 - a;
  
	const CLineArray & arrDest = GetLineArray();
	for(  UINT y = nTop; y < nBottom; y++ )
	{
		DIB_PIXEL *pDest = arrDest[ y ];
		pDest += nLeft;

		for( UINT x = nLeft; x < nRight; x++, pDest++ )
		{
			Blend( pDest->rgbRed, nNegativeAplha, r, a );
			Blend( pDest->rgbGreen, nNegativeAplha, g, a );
			Blend( pDest->rgbBlue, nNegativeAplha, b, a );
		}
	}
}


