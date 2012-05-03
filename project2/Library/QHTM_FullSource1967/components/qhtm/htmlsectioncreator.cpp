/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	HTMLSectionCreator.cpp
Owner:	russf@gipsysoft.com
Purpose:	Section creator for the HTML display.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include <math.h>
#include "qhtm.h"
#include "HTMLParse.h"
#include "HTMLSectionCreator.h"
#include "HTMLTextSection.h"
#include "HTMLSection.h"
#include "defaults.h"

CHTMLSectionCreator::CHTMLSectionCreator( CHTMLSection *psect, GS::CDrawContext &dc, int nTop, int nLeft, int nRight, COLORREF crBack, bool bMeasuring, int nZoomLevel, CHTMLSectionLink* pCurrentLink )
	: m_psect( psect )
	, m_dc( dc )

	, m_nDefaultLeftMargin( nLeft )
	, m_nDefaultRightMargin( nRight )
	, m_nTop( nTop )
	, m_nLeftMargin( nLeft )
	, m_nRightMargin( nRight )
	, m_nYPos( nTop )
	, m_nXPos( 0 )
	, m_bMeasuring( bMeasuring )
	, m_nPreviousParaSpaceBelow( 0 )
	, m_algCurrentPargraph( CStyle::algLeft )
	, m_nFirstShapeOnLine( 0 )
	, m_nNextYPos( nTop )
	, m_nLowestBaseline( 0 )
	, m_nLineWidth( nRight - nLeft )
	, m_nWidth( 0 )
	, m_nZoomLevel( nZoomLevel )
	, m_cCharSet( 0 )
	, m_crBack( crBack )
	, m_pCurrentLink( pCurrentLink )

{
	m_dc.SelectFont( g_defaults.m_strFontName, GetFontSizeAsPixels( m_dc.GetSafeHdc(), g_defaults.m_nFontSize, m_nZoomLevel ), FW_NORMAL, false, false, false, g_defaults.m_cCharSet );

}


CHTMLSectionCreator::~CHTMLSectionCreator()
{

}

void CHTMLSectionCreator::SkipLeftMargin()
{
	int nNewYPos = GetCurrentYPos();
	//
	//	Only do this if there is a right margin!
	if( m_stkLeftMargin.GetSize() )
	{
		CarriageReturn( true );
		nNewYPos = GetCurrentYPos();

		m_nLeftMargin = m_nDefaultLeftMargin;

		while( m_stkLeftMargin.GetSize() )
		{
			MarginStackItem msi = m_stkLeftMargin.Pop();
			if( msi.nYExpiry > nNewYPos )
			{	
				nNewYPos = msi.nYExpiry;
			}
		}
	}

	SetCurrentXPos( GetLeftMargin() );
	SetCurrentYPos( nNewYPos );
}


void CHTMLSectionCreator::SkipRightMargin()
{
	int nNewYPos = GetCurrentYPos();
	//
	//	Only do this if there is a right margin!
	if( m_stkRightMargin.GetSize() )
	{
		CarriageReturn( true );
		nNewYPos = GetCurrentYPos();

		m_nRightMargin = m_nDefaultRightMargin;

		while( m_stkRightMargin.GetSize() )
		{
			MarginStackItem msi = m_stkRightMargin.Pop();
			if( msi.nYExpiry > nNewYPos )
			{	
				nNewYPos = msi.nYExpiry;
			}
		}
	}
	SetCurrentXPos( GetLeftMargin() );
	SetCurrentYPos( nNewYPos );
}


void CHTMLSectionCreator::CarriageReturn( bool bSetNextY )
//
//	Recalcualtes the left and right margins for the current Y position and
//	moves the X position to the new left margin.
{
	if( GetCurrentShapeID() )
	{
		AdjustShapeBaselinesAndHorizontalAlignment();
	}

	m_nLeftMargin = m_nDefaultLeftMargin;
	while( m_stkLeftMargin.GetSize() )
	{
		MarginStackItem msi = m_stkLeftMargin.Top();
		if( msi.nYExpiry >= m_nNextYPos )
		{
			m_nLeftMargin = msi.nMargin;
			break;
		}
		else
		{
			(void)m_stkLeftMargin.Pop();
		}
	}


	m_nRightMargin = m_nDefaultRightMargin;
	while( m_stkRightMargin.GetSize() )
	{
		MarginStackItem msi = m_stkRightMargin.Top();
		if( msi.nYExpiry >= m_nNextYPos )
		{
			m_nRightMargin = msi.nMargin;
			break;
		}
		else
		{
			(void)m_stkRightMargin.Pop();
		}
	}
	m_nXPos = GetLeftMargin();

	m_mapBaseline.RemoveAll();
	m_nLowestBaseline = 0;
	m_nFirstShapeOnLine = GetCurrentShapeID();

	//	If there is nothing in the margin stack, this is a good place for
	//	a page break.
	if( m_dc.IsPrinting() && GetLeftMargin() == m_nDefaultLeftMargin && GetRightMargin() == m_nDefaultRightMargin)
	{
		GetHTMLSection()->AddBreakSection(GetCurrentShapeID());
	}

	if( bSetNextY )
	{
		SetCurrentYPos( GetNextYPos() );
	}
}


void CHTMLSectionCreator::Finished()
//
//	Signals the end fo the document.
{
	CarriageReturn( true );

	//
	//	Adjust the height of our document to take into account any images or tables there may be on the left or right
	//	of the document and that may be taller than we have left the document
	while( m_stkRightMargin.GetSize() )
	{
		MarginStackItem msi = m_stkRightMargin.Top();
		if( msi.nYExpiry > m_nYPos )
			m_nYPos = msi.nYExpiry;
		(void)m_stkRightMargin.Pop();
	}

	while( m_stkLeftMargin.GetSize() )
	{
		MarginStackItem msi = m_stkLeftMargin.Top();
		if( msi.nYExpiry > m_nYPos )
			m_nYPos = msi.nYExpiry;
		(void)m_stkLeftMargin.Pop();
	}
	if( m_dc.IsPrinting() )
		GetHTMLSection()->AddBreakSection(GetCurrentShapeID());
}


void CHTMLSectionCreator::NewParagraph( int nSpaceAbove, int nSpaceBelow, CStyle::Align alg )
//
//	New paragraph.
//	Add on the space below from the previous paragraph and the space above for this paragraph.
{
	CarriageReturn( true );

	if( m_nYPos != m_nTop )
	{
		const int nCurrentFontHeight = m_dc.GetCurrentFontHeight();
		m_nYPos += ( nCurrentFontHeight * m_nPreviousParaSpaceBelow ) / 2;
		m_nYPos += ( nCurrentFontHeight * nSpaceAbove ) / 2;
	}

	if( alg != CStyle::algEmpty )
		m_algCurrentPargraph = alg;

	m_nPreviousParaSpaceBelow = nSpaceBelow;
}


int CHTMLSectionCreator::GetCurrentShapeID() const
{
	return GetHTMLSection()->GetSectionCount();
}


void CHTMLSectionCreator::AddBaseline( int nBaseline )
{
	//	If this fires then either we haven't cleared the baseline list *or* two shapes have the same ID.
	ASSERT( !m_mapBaseline.Lookup( GetCurrentShapeID() - 1 ) );
	//TRACE("Baseline ADDED %d - %d\n", GetCurrentShapeID(), nBaseline );

	m_mapBaseline.SetAt( GetCurrentShapeID() - 1, nBaseline );

	if( nBaseline > m_nLowestBaseline )
	{
		m_nLowestBaseline = nBaseline;
	}
}


int CHTMLSectionCreator::GetBaseline( int nShape ) const
{
	const int *pn = m_mapBaseline.Lookup( nShape );
	if( pn )
		return *pn;

	return -1;
}


void CHTMLSectionCreator::AdjustShapeBaselinesAndHorizontalAlignment()
//
//	Adjust the shapes baselines to allow for different heights and styles of text
{
	int nHorizontalDelta = 0;

	//
	//	Horizontally align the shapes.
	//
	//	Get the horizontal extent of allof the shapes.
	//	if right aligned then move allthe shapes on the line by the difference
	//		between the width between the margins and the line extent.
	//	if centre aligned then move the shapes half the difference between the
	//		margins and the line extent.
	if( !m_bMeasuring && m_algCurrentPargraph != CStyle::algLeft && m_nFirstShapeOnLine != GetCurrentShapeID() )
	{
		const int nCurrentShape = GetCurrentShapeID() - 1;
		const CSectionABC *psectFirst = GetHTMLSection()->GetSectionAt( m_nFirstShapeOnLine );
		const CSectionABC *psectSecond = GetHTMLSection()->GetSectionAt( nCurrentShape );
		const int nExtent =  psectSecond->right -  psectFirst->left;
		const int nSpaceWidth = m_nLineWidth - nExtent;
		if( m_algCurrentPargraph == CStyle::algCentre )
		{
			// richg - 19990225 - Never shift left!
			nHorizontalDelta = max( 0, nSpaceWidth / 2 );
		}
		else if( m_algCurrentPargraph == CStyle::algRight )
		{
			// richg - 19990225 - Never shift left!
			nHorizontalDelta = max( 0, nSpaceWidth );
		}
	}

	//
	//	Minus one is a signal that the shapes are all the same size.
	m_nNextYPos = GetCurrentYPos();

	const int nCurrentShapeID = GetCurrentShapeID();
	if( m_nFirstShapeOnLine != nCurrentShapeID )
	{
		for( int n = m_nFirstShapeOnLine; n < nCurrentShapeID; n++ )
		{
			CSectionABC *pSect = GetHTMLSection()->GetSectionAt( n );
			const int nBaseline = GetBaseline( n );
			if( nBaseline >= 0 )
			{
				const int nBaselineDelta = m_nLowestBaseline - nBaseline;
				if( nBaselineDelta || nHorizontalDelta )
				{
					pSect->MoveXY( nHorizontalDelta, nBaselineDelta );
				}

				//
				//	We only use shapes that have a baseline
				if( pSect->bottom > m_nNextYPos )
				{
					m_nNextYPos = pSect->bottom;
				}
			}

			if( pSect->right > m_nWidth )
				m_nWidth = pSect->right;
		}
	}
}




void CHTMLSectionCreator::AddDocument( CHTMLDocument *pDocument )
{
	m_cCharSet = pDocument->m_cCharSet;

	m_nLineWidth = GetCurrentWidth();

	//	Pointers used to process the anchor information
	CHTMLAnchor* pLastAnchor = NULL;
	const UINT uDocumentSize = pDocument->m_arrItems.GetSize();
	for( UINT n = 0; n < uDocumentSize; n++ )
	{
		CHTMLDocumentObject *pItem = pDocument->m_arrItems[ n ];
		switch( pItem->GetType() )
		{
		case CHTMLDocumentObject::knParagraph:
			{
				CHTMLParagraph *pPara = static_cast<CHTMLParagraph *>( pItem );
				if( pPara->IsEmpty() )
					continue;

				pPara->AddDisplayElements( this );

				NewParagraph( pPara->m_nSpaceAbove, pPara->m_nSpaceBelow, pPara->m_alg );

				const UINT uParagraphSize = pPara->m_arrItems.GetSize();
				for( UINT nItem = 0; nItem < uParagraphSize; nItem++ )
				{
					CHTMLParagraphObject *pParaObj = pPara->m_arrItems[ nItem ];

					// Update the Anchor status
					if (pParaObj->m_pAnchor == NULL || pParaObj->m_pAnchor != pLastAnchor)
						m_pCurrentLink = NULL;
		

					switch( pParaObj->GetType() )
					{
					case CHTMLParagraphObject::knAnchor:
						{
							pParaObj->AddDisplayElements( this );	

							CHTMLAnchor *pAnchor = static_cast<CHTMLAnchor *>( pParaObj );
							pLastAnchor = pAnchor;
						}
						break;

					default:
						pParaObj->AddDisplayElements( this );	
					}
				}
			}
			break;
		}
	}
	Finished();
}