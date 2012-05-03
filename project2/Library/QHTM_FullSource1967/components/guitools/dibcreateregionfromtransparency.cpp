/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	DIBCreateRegionFromTransparency.cpp
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "DIB.h"

using namespace GS;

HRGN GS::DIBCreateRegionFromTransparency( GS::CDIB &dib )
{
	const GS::CDIB::CLineArray & arrSrc = dib.GetLineArray();
	const UINT uSize = arrSrc.GetSize();

	Container::CArray< BYTE > arrData;
	UINT uCount = 0;

	WinHelper::CRect rc;
	for( register UINT y = 0; y < uSize; y++ )
	{
		DIB_PIXEL *pSrc = arrSrc[ y ];
		for( int x = 0; x < dib.GetWidth(); x++, pSrc++ )
		{
			if( pSrc->rgbReserved != 0 )
			{
				rc.Set( x, y, x + 1, y + 1 );
				arrData.Add( (BYTE*)&rc, sizeof( rc ) );
				uCount++;
			}
		}
	}

	RGNDATAHEADER rdh;
	rdh.dwSize = sizeof( rdh );
	rdh.iType = RDH_RECTANGLES;
	rdh.nCount = uCount;
	rdh.nRgnSize = 0;
	rdh.rcBound.left = 0;
	rdh.rcBound.top = 0;
	rdh.rcBound.right = dib.GetWidth();
	rdh.rcBound.bottom = dib.GetHeight();
	arrData.InsertAt( 0, (BYTE*)&rdh, sizeof( rdh ) );

	HRGN hrgn = ExtCreateRegion( NULL, arrData.GetSize(), (RGNDATA *)arrData.GetData() );
	return hrgn;
}
