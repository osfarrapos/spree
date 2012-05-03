/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.

File:	HTMLTextBlock.cpp
Owner:	russf@gipsysoft.com
Purpose:	HTML Text block.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "HTMLParse.h"
#include "HTMLSectionCreator.h"
#include "HTMLTextSection.h"

CHTMLTextBlock::CHTMLTextBlock( const CTextABC &strText, const HTMLFontDef * pFont, COLORREF crFore, bool bBackground, COLORREF crBack, bool bPreformatted )
	: CHTMLParagraphObject( CHTMLParagraphObject::knNone )
	, m_strText( strText, strText.GetLength() )
	, m_pFont( pFont )
	, m_crFore( crFore )
	, m_bPreformatted( bPreformatted )
	, m_bBackground( bBackground )
	, m_crBack( crBack )
{
}

#ifdef _DEBUG
void CHTMLTextBlock::Dump() const
{
	TRACENL(_T("TextBlock\n") );
	TRACENL(_T("\tText(%s)\n"), (LPCTSTR)m_strText );
	TRACENL(_T("\t crFore(%d)\n"), m_crFore );
	TRACENL(_T("\t bPreformatted(%d)\n"), m_bPreformatted );	
}
#endif	//	_DEBUG

bool CHTMLTextBlock::IsEmpty() const
{
	return m_strText.GetLength() == 1 && *(m_strText.GetData()) == _T(' ');
}

int g_CurrentXPos = 0;

void CHTMLTextBlock::AddDisplayElements( class CHTMLSectionCreator *psc )
{
	GS::FontDef fdef( m_pFont->m_strFont, m_pFont->m_nSize, m_pFont->m_nWeight, m_pFont->m_bItalic, m_pFont->m_bUnderline, m_pFont->m_bStrike, psc->GetCurrentCharSet(), m_pFont->m_bFixedPitchFont );
	fdef.m_nSizePixels = GetFontSizeAsPixels( psc->GetDC().GetSafeHdc(), m_pFont->m_nSize, psc->GetZoomLevel() );

	psc->GetDC().SelectFont( fdef );

	const int nCurrentFontHeight = psc->GetDC().GetCurrentFontBaseline();

	LPCTSTR pcszTextLocal = m_strText;
	LPCTSTR pcszPrevious = m_strText;

	if( m_bPreformatted )
	{
		while( 1 )
		{
			if( *pcszTextLocal == '\r' || *pcszTextLocal == '\n' || *pcszTextLocal == '\000' )
			{
				const UINT uLength = pcszTextLocal - pcszPrevious;
				const WinHelper::CSize size( psc->GetDC().GetTextExtent( pcszPrevious, uLength ), nCurrentFontHeight );

				CHTMLTextSection *pText = new CHTMLTextSection( psc->GetHTMLSection(), pcszPrevious, uLength, fdef, m_crFore, m_bBackground, m_crBack );
				psc->AddSection( pText );
				
				pText->Set( psc->GetCurrentXPos(), psc->GetCurrentYPos(), psc->GetCurrentXPos() + size.cx, psc->GetCurrentYPos() + size.cy );
				psc->SetCurrentXPos( psc->GetCurrentXPos() + size.cx );
				psc->AddBaseline( psc->GetDC().GetCurrentFontBaseline() );

				//	

				if( *pcszTextLocal == '\000' )
				{
					break;
				}

				if( *pcszTextLocal == '\r' && *( pcszTextLocal + 1 ) == '\n' )
				{
					*pcszTextLocal++;
					psc->CarriageReturn( true );
				}
				pcszPrevious = pcszTextLocal + 1;
			}
			*pcszTextLocal++;
		}
	}
	else
	{

		/*
			try the text to see if it fits.
				if it does then simply create a new object and bump along the XPos
				if it does not then create an object with the least amount of data to fit
					create a new line (CarriageReturn) and try again.
		*/

		int nBaseLine = nCurrentFontHeight;
		if( m_pFont->m_nSup )
		{
			nBaseLine = nCurrentFontHeight + m_pFont->m_nSup * nCurrentFontHeight / 3;
		}
		else if( m_pFont->m_nSub )
		{
			nBaseLine = nCurrentFontHeight - m_pFont->m_nSub * nCurrentFontHeight / 3;
		}
		WinHelper::CSize size;
		bool bDone = false;
		while( !bDone )
		{
			CHTMLTextSection *pText = NULL;

			g_CurrentXPos = psc->GetCurrentXPos();

			//	richg - 19990224 - Altered to indicate the the CDrawContext whether or not
			//	we are at the beginning of a line.
			if( psc->GetDC().GetTextFitWithWordWrap( psc->GetRightMargin() - psc->GetCurrentXPos(), pcszTextLocal, size, (bool)(psc->GetCurrentXPos() == psc->GetLeftMargin()) ) )
			{
				pText = new CHTMLTextSection( psc->GetHTMLSection(), pcszPrevious, pcszTextLocal - pcszPrevious, fdef, m_crFore, m_bBackground, m_crBack );
				psc->AddSection( pText );
				pText->Set( psc->GetCurrentXPos(), psc->GetCurrentYPos(), psc->GetCurrentXPos() + size.cx, psc->GetCurrentYPos() + size.cy );
				psc->SetCurrentXPos( psc->GetCurrentXPos() + size.cx );

				psc->AddBaseline( nBaseLine );
				bDone = true;
			}
			else
			{
				if( pcszTextLocal - pcszPrevious > 0 && ( psc->GetLeftMargin() == psc->GetCurrentXPos() || _istspace( *pcszTextLocal ) ) )
				{
					pText = new CHTMLTextSection( psc->GetHTMLSection(), pcszPrevious, pcszTextLocal - pcszPrevious, fdef, m_crFore, m_bBackground, m_crBack );
					psc->AddSection( pText );
					pText->Set( psc->GetCurrentXPos(), psc->GetCurrentYPos(), psc->GetCurrentXPos() + size.cx, psc->GetCurrentYPos() + size.cy );

					psc->AddBaseline( nBaseLine );
				}
				else
				{
					pcszTextLocal = pcszPrevious;
				}

				psc->CarriageReturn( true );

				//	Skip over trailing spaces
				while( *pcszTextLocal && _istspace( *pcszTextLocal ) ) pcszTextLocal++;
				pcszPrevious = pcszTextLocal;
			}
		}
		
	}
}
