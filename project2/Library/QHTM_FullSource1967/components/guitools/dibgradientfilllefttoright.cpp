/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	DIBGradientFillLeftToRight.cpp
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "DIB.h"

using namespace GS;


void CDIB::GradientFillLeftToRight( const RECT &rc, COLORREF crTop, COLORREF crBottom )
{
	const UINT nTop = max( rc.top, 0 );
	const UINT nBottom = min( rc.bottom, GetHeight() );
	const UINT nLeft = max( rc.left, 0 );
	const UINT nRight = min( rc.right, GetWidth() );

  const int r1 = GetRValue( crTop ), g1 = GetGValue( crTop ), b1 = GetBValue( crTop ), a1 = GetAValue( crTop );
  const int r2 = GetRValue( crBottom ), g2 = GetGValue( crBottom ), b2 = GetBValue( crBottom ), a2 = GetAValue( crBottom );

	int nEnd = rc.right - rc.left;

	int nCol = 0;
	for( UINT x = nLeft; x < nRight; x++, nCol++)
	{
    int r,g,b, a; 

    r = r1 + (nCol * (r2-r1) / nEnd); 
    g = g1 + (nCol * (g2-g1) / nEnd);
    b = b1 + (nCol * (b2-b1) / nEnd);
		a = a1 + (nCol * (a2-a1) / nEnd);
		COLORREFA cr = DIB_RGBA( r, g, b, a );
		DrawLine( x, nTop, x, nBottom, cr );
	}
}


