/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	HTMLComboSection.cpp
Owner:	russf@gipsysoft.com
Purpose:	
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "QHTM.h"
#include "HTMLComboSection.h"
#include "HTMLSectionCreator.h"
#include "HTMLFormInput.h"
#include <WindowText.h>

extern void SubclassControl( HWND hwnd, CHTMLSection *psectHTML, CFocusControlABC *pFocus );

CHTMLComboSection::CHTMLComboSection( CHTMLSection *pSectParent, GS::FontDef &fdef, CHTMLSelect *pFormObject, class CHTMLSectionCreator *psc )
	: CHTMLSectionABC( pSectParent )
	, m_fdef( fdef )
	, m_pFormObject( pFormObject )
	, m_uComboHeight( 0 )
{
	SetKeep( true );

	if( !psc->IsMeasuring() && !psc->GetDC().IsPrinting() )
	{
		DWORD dwStyle = WS_BORDER | WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL | CBS_NOINTEGRALHEIGHT;

		LPTSTR endptr;
		UINT uID = pFormObject->m_strID.GetLength() ? _tcstol( pFormObject->m_strID, &endptr, 10 ) : 0;
		m_hwnd = CreateWindowEx( WS_EX_STATICEDGE, _T("COMBOBOX"), NULL,  dwStyle, 0, 0, 0, 0, pSectParent->GetHwnd(), (HMENU)uID, g_hQHTMInstance, NULL );
		ASSERT( m_hwnd );

		for( UINT u = 0; u < pFormObject->m_arrItems.GetSize(); u++ )
		{
			CHTMLOption *pOption = pFormObject->m_arrItems[ u ];
			LPCTSTR pcszText = (LPCTSTR)pOption->m_strText;
			if( pcszText )
			{
				int nItem = SendMessage( m_hwnd, CB_ADDSTRING, 0, (LPARAM)pcszText );
				if( pOption->m_bSelected )
				{
					SendMessage( m_hwnd, CB_SETCURSEL, nItem, 0 );
				}
			}
		}

		if( pFormObject->m_bDisabled )
		{
			EnableWindow( m_hwnd, FALSE );
		}

		SubclassControl( m_hwnd, pSectParent, this );
	}
}


CHTMLComboSection::~CHTMLComboSection()
{
	DestroyWindow( m_hwnd );
}


void CHTMLComboSection::OnDraw( GS::CDrawContext &dc )
{
	if( dc.IsPrinting() )
	{
		dc.Rectangle( *this, RGB( 0, 0, 0 ) );
		dc.SelectFont( m_fdef );

		WinHelper::CRect rc( *this );
		int nScrollSize = dc.ScaleX( GetSystemMetrics( SM_CXVSCROLL ) );
		rc.left = rc.right - nScrollSize;
		DrawFrameControl( dc.GetSafeHdc(), rc, DFC_SCROLL, DFCS_SCROLLDOWN );
		rc = *this;
		rc.right -= nScrollSize;
		dc.SetClipRect( rc );

		for( UINT u = 0; u < m_pFormObject->m_arrItems.GetSize(); u++ )
		{
			CHTMLOption *pOption = m_pFormObject->m_arrItems[ u ];
			if( pOption->m_bSelected )
			{
				dc.DrawText( left, top, pOption->m_strText, pOption->m_strText.GetLength(), RGB( 0, 0, 0 ) );
				
			}
		}

		dc.RemoveClip();
	}
}


void CHTMLComboSection::FigureOutSize( CHTMLSectionCreator *psc, WinHelper::CSize &size )
{
	GS::CDrawContext &dc = psc->GetDC();
	dc.SelectFont( m_fdef );

	#if defined (_WIN32_WCE)
	// value of all f's causes exception error in wince.
	// occurs before combo is created and sizing is in progress?
	if (m_hwnd == (HWND)0xFFFFFFFF) 
		m_hwnd = 0;
	#endif

	SendMessage( m_hwnd, WM_SETFONT, (WPARAM)psc->GetDC().GetCurrentHFONT(), FALSE );

	//
	//	Measure the list
	UINT uWidth = 0;
	for( UINT u = 0; u < m_pFormObject->m_arrItems.GetSize(); u++ )
	{
		CHTMLOption *pOption = m_pFormObject->m_arrItems[ u ];
		UINT uItemWidth = dc.GetTextExtent( pOption->m_strText, pOption->m_strText.GetLength() );
		if( uItemWidth > uWidth )
		{
			uWidth = uItemWidth;
		}
	}
	
	size.cx = uWidth + 12;
	size.cx += GetSystemMetrics( SM_CXVSCROLL );
	size.cy = dc.GetCurrentFontHeight() + 8;

	m_uComboHeight = dc.GetCurrentFontHeight() * m_pFormObject->m_arrItems.GetSize() +  4;
}


void CHTMLComboSection::OnLayout( const WinHelper::CRect &rc )
{
	SetWindowPos( m_hwnd, NULL, rc.left, rc.top, rc.Width(), rc.Height() + m_uComboHeight, SWP_NOZORDER );
}


void CHTMLComboSection::MoveY( int nOffsetY )
{
	CHTMLSectionABC::MoveY( nOffsetY );
	OnLayout( *this );
}


void CHTMLComboSection::MoveXY( int nOffsetX, int nOffsetY )
{
	CHTMLSectionABC::MoveXY( nOffsetX, nOffsetY );
	OnLayout( *this );
}


void CHTMLComboSection::SetFocus( bool bHasFocus )
{
	CHTMLSectionABC::SetFocus( bHasFocus );

	if( bHasFocus )
	{
		::SetFocus( m_hwnd );
	}
}


bool CHTMLComboSection::IsFocused() const
{
	return ::GetFocus() == m_hwnd;
}


void CHTMLComboSection::ResetContent()
{
	for( UINT u = 0; u < m_pFormObject->m_arrItems.GetSize(); u++ )
	{
		CHTMLOption *pOption = m_pFormObject->m_arrItems[ u ];
		if( pOption->m_bSelected )
		{
			SendMessage( m_hwnd, CB_SETCURSEL, u, 0 );
			break;
		}
	}
}


void CHTMLComboSection::UpdateFormFromControls()
{
	for( UINT u = 0; u < m_pFormObject->m_arrItems.GetSize(); u++ )
	{
		CHTMLOption *pOption = m_pFormObject->m_arrItems[ u ];
		pOption->m_bSelected = false;
	}
	int n = SendMessage( m_hwnd, CB_GETCURSEL, 0, 0 );
	m_pFormObject->m_arrItems[ n ]->m_bSelected = true;
}