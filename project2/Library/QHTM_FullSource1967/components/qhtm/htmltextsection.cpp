/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.

File:	HTMLTextSection.cpp
Owner:	russf@gipsysoft.com
Purpose:	Simple drawn text object.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "HTMLTextSection.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHTMLTextSection::CHTMLTextSection( CHTMLSection * pParent, LPCTSTR pcszText, int nLength, GS::FontDef &fdef, COLORREF crFore, bool bBackground, COLORREF crBack )
	: CHTMLSectionABC( pParent )
	, m_str( pcszText, nLength )
	, m_fdef( fdef )
	, m_crFore( crFore )
	, m_bBackground( bBackground )
	, m_crBack( crBack )
{
	Transparent( true );
	SetCharacterSet( m_fdef.m_cCharSet );
}

CHTMLTextSection::~CHTMLTextSection()
{

}


void CHTMLTextSection::OnDraw( GS::CDrawContext &dc )
{
	if( m_bBackground )
	{
		dc.FillRect( *this, m_crBack );
	}

	dc.SelectFont( m_fdef );
	if( IsLink() )
	{
		if( IsMouseInSection() )
		{
			dc.DrawText( left, top, m_str, m_str.GetLength(), LinkHoverColour() );
		}
		else
		{
			dc.DrawText( left, top, m_str, m_str.GetLength(), LinkColour() );
		}
	}
	else
	{
		dc.DrawText( left, top, m_str, m_str.GetLength(), m_crFore );
	}

	CHTMLSectionABC::OnDraw( dc );
}
