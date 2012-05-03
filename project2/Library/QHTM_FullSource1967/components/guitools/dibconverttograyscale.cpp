/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	DIBConvertToGrayScale.cpp
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "DIB.h"

using namespace GS;

void GS::ConvertToGrayScale( GS::CDIB &dib )
{
	const GS::CDIB::CLineArray & arrDest = dib.GetLineArray();
	const UINT uSize = arrDest.GetSize();

	for( register UINT u = 0; u < uSize; u++ )
	{
		DIB_PIXEL *pDest = arrDest[ u ];
		for( int c = 0; c < dib.GetWidth(); c++, pDest++ )
		{
			//const BYTE bNew = pDest->rgbRed * 0.299 + pDest->rgbGreen * 0.258 + pDest->rgbBlue * 0.114;
			//const BYTE bNew = (pDest->rgbRed * 3 + pDest->rgbGreen * 6 + pDest->rgbBlue) / 10;
			const BYTE bNew = (BYTE)( (pDest->rgbRed + pDest->rgbGreen + pDest->rgbBlue) / 3 );
			pDest->rgbRed = bNew;
			pDest->rgbGreen = bNew;
			pDest->rgbBlue = bNew;
		}
	}
	
}
