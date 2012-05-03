
////////////////////////////////////////////////////////////////////////////
//	Copyright : Ashok Jaiswal 2004
//
//	Email :ashok@muvee.com
//
//	This code may be used in compiled form in any way you desire. This
//	file may be redistributed unmodified by any means PROVIDING it is 
//	not sold for profit without the authors written consent, and 
//	providing that this notice and the authors name is included.
//
//	This file is provided 'as is' with no expressed or implied warranty.
//	The author accepts no liability if it causes any damage to your computer.
//
//	Do expect bugs.
//	Please let me know of any bugs/mods/improvements.
//	and I will try to fix/incorporate them into this file.
//	Enjoy!
//
//	Description : Description goes here 
////////////////////////////////////////////////////////////////////////////
// BitComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "BitComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int nListBoxTextLeft = 0;

static WNDPROC m_pWndProc = 0;

//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:23:46 PM
// Function name	: LRESULT FAR PASCAL BitComboBoxListBoxProc
// Description	    : The list box control in combo box is actually owned by window so we let all the messages
//						handled by windows.
// Return type		: extern "C" 
// Argument         : HWND hWnd
// Argument         : UINT nMsg
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
//----------------------------------------------------------------------
extern "C" LRESULT FAR PASCAL BitComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	//TRACE("BitComboBoxListBoxProc 0x%.4X\n",nMsg);
	return CallWindowProc(m_pWndProc, hWnd, nMsg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////
// CBitComboBox


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:42 PM
// Function name	: CBitComboBox::CBitComboBox
// Description	    : Constructor
// Return type		: 
//----------------------------------------------------------------------
CBitComboBox::CBitComboBox()
{
	m_nIDLeft		= 0;
	m_nIDRight		= 0;
	m_nIDCenter		= 0;
	m_nIDListLeft	= 0;
	m_nIDListRight	= 0;
	m_nIDListTop	= 0;
	m_nIDListBot	= 0;
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:43 PM
// Function name	: CBitComboBox::~CBitComboBox
// Description	    : Destructor
// Return type		: 
//----------------------------------------------------------------------
CBitComboBox::~CBitComboBox()
{
}


BEGIN_MESSAGE_MAP(CBitComboBox, CComboBox)
	//{{AFX_MSG_MAP(CBitComboBox)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, ChangeButton)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, ChangeButton)	
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitComboBox message handlers

//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:38 PM
// Function name	: CBitComboBox::OnCtlColorListBox
// Description	    : It is called when list box item are painted
// Return type		: LRESULT 
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
//----------------------------------------------------------------------
LRESULT CBitComboBox::OnCtlColorListBox(WPARAM wParam, LPARAM lParam) 
{
	// Here we need to get a reference to the listbox of the combobox
	// (the dropdown part). We can do it using 
	//TRACE("OnCtlColorListBox()\n");
	if (this->m_listbox.m_hWnd == 0) {
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd) 
		{
			// Save the handle
			m_listbox.m_hWnd = hWnd;

			// Subclass ListBox
			m_pWndProc = (WNDPROC)GetWindowLong(m_listbox.m_hWnd, GWL_WNDPROC);
			SetWindowLong(m_listbox.m_hWnd, GWL_WNDPROC, (LONG)BitComboBoxListBoxProc);
		}
	}

	
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}


#define SET_ZERO(X) memset((void *)&X,0,sizeof(X));

//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:36 PM
// Function name	: CBitComboBox::OnPaint
// Description	    : Main painting procedure. Skin the Combo box and skin the list box once its created.
// Return type		: void 
//----------------------------------------------------------------------
void CBitComboBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT rc;
	CDC MemDC;
	
	GetClientRect(&rc);
	MemDC.CreateCompatibleDC(&dc);

	rc.left +=2;
	rc.top += 2;
	rc.bottom += 2;
	
	CBitmap bmpComboRight,bmpComboLeft, bmpComboCenter,bmpComboBot;
	BITMAP bitRight, bitLeft, bitCenter;
	SET_ZERO(bitRight); SET_ZERO(bitLeft); SET_ZERO(bitCenter);
	if( m_nIDLeft )
	{
		bmpComboLeft.LoadBitmap(m_nIDLeft);
		bmpComboLeft.GetBitmap(&bitLeft);
		MemDC.SelectObject(bmpComboLeft);
		dc.BitBlt(rc.left,rc.top,bitLeft.bmWidth,bitLeft.bmHeight,&MemDC,0,0,SRCCOPY);
	}
	if( m_nIDRight )
	{
		bmpComboRight.LoadBitmap(m_nIDRight);
		bmpComboRight.GetBitmap(&bitRight);
		MemDC.SelectObject(bmpComboRight);
		dc.BitBlt(rc.right-bitRight.bmWidth,rc.top,bitRight.bmWidth,bitRight.bmHeight,&MemDC,0,0,SRCCOPY);
	}

	if( m_nIDCenter )
	{
		bmpComboCenter.LoadBitmap(m_nIDCenter);
		bmpComboCenter.GetBitmap(&bitCenter);
		MemDC.SelectObject(bmpComboCenter);
		dc.StretchBlt(rc.left+bitLeft.bmWidth,rc.top,rc.right-bitLeft.bmWidth-bitRight.bmWidth,bitCenter.bmHeight,&MemDC,0,0,bitCenter.bmWidth,bitCenter.bmHeight,SRCCOPY);
	}

	rc.left -=2;
	rc.top -= 2;
	rc.bottom -= 2;

	CString sz;
	GetWindowText(sz);
	rc.left += nListBoxTextLeft;

	//Select the font only if list box is created
	if( m_listbox )
 		dc.SelectObject(m_listbox.GetFont());

	dc.SetBkMode(TRANSPARENT);
	
	RECT rcRepaint;
	GetClientRect(&rcRepaint);
	rcRepaint.left = rcRepaint.left + bitLeft.bmWidth;
	rcRepaint.right = rcRepaint.right - bitRight.bmWidth;
	rcRepaint.top += 3; 
	rcRepaint.bottom -= 2;
	//Set the normal/highlight color when its repainted 
	CWnd* pFocusWindow = GetFocus();
	if( pFocusWindow && (pFocusWindow->m_hWnd == m_hWnd) )
	{
		rcRepaint.left+=1;
		dc.FillSolidRect(&rcRepaint,m_colBackGroundHighLight);
		dc.SetTextColor(m_colTextHighLight);
		rcRepaint.left-=1;
		TRACE(_T("Filling rect again on focus high\n"));
	}
	else
	{
		dc.FillSolidRect(&rcRepaint,m_colBackGroundNormal);
		dc.SetTextColor(m_colTextNormal);
		TRACE(_T("Filling rect again on focus normal\n"));
	}
	
	dc.SelectObject(GetFont());
	dc.DrawText(sz,&rcRepaint,DT_VCENTER | DT_SINGLELINE|DT_END_ELLIPSIS );
	
	if(IsWindow(m_listbox.m_hWnd))
	{

		 //TRACE("Painting the listbox\n");
		 CDC *pDC = m_listbox.GetDC();
		 m_listbox.GetClientRect(&rc);

		 CBrush pBrush;
		 pBrush.CreateSolidBrush(m_colBackGroundNormal);
		 
 	    m_listbox.GetDC()->FillRect(&rc,&pBrush);
	
		//Draw the left side of the list box
		DrawListLeft();
		//Draw the right side of the list box
		DrawListRight();

		//Draw the top of the list bar
		DrawListTop();

		//Draw the bottom of the list bar
		DrawListBot();
		
		pDC->SetTextColor(m_colTextNormal);
		CBrush brItemData;
		RECT rcItemData;
		CString szItemString;
		brItemData.CreateSolidBrush(m_colBackGroundNormal);
		for(int i =0;i<m_listbox.GetCount();i++)
		{
			m_listbox.GetText(i,szItemString);
			m_listbox.GetItemRect(i,&rcItemData);
			
			rcItemData.left += bitLeft.bmWidth+5;

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_colTextNormal);

			nListBoxTextLeft = rcItemData.left;
			
			//CFont* def_font = pDC->SelectObject(&m_Font);
			pDC->SelectObject(m_listbox.GetFont());
			pDC->DrawText(szItemString,&rcItemData, DT_VCENTER | DT_SINGLELINE);
			//pDC->SelectObject(def_font);

		}
	}
	
	// Do not call CComboBox::OnPaint() for painting messages
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:31 PM
// Function name	: CBitComboBox::Create
// Description	    : Sets the combo box properties to owner draw and fix.
// Return type		: BOOL 
// Argument         : LPCTSTR lpszClassName
// Argument         : LPCTSTR lpszWindowName
// Argument         : DWORD dwStyle
// Argument         : const RECT& rect
// Argument         : CWnd* pParentWnd
// Argument         : UINT nID
// Argument         : CCreateContext* pContext
//----------------------------------------------------------------------
BOOL CBitComboBox::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	// Remove the CBS_SIMPLE and CBS_DROPDOWN styles and add the one I'm designed for
	dwStyle &= ~0xF;
	dwStyle |= CBS_DROPDOWNLIST;

	// Make sure to use the CBS_OWNERDRAWVARIABLE style
	dwStyle |= CBS_OWNERDRAWVARIABLE;

	// Use default strings. We need the itemdata to store the state of the lamps
	dwStyle |= CBS_HASSTRINGS;	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:26 PM
// Function name	: CBitComboBox::OnDestroy
// Description	    : Releases the sub classed control
// Return type		: void 
//----------------------------------------------------------------------
void CBitComboBox::OnDestroy() 
{
      if (m_listbox.GetSafeHwnd() != NULL)
         m_listbox.UnsubclassWindow();

	CComboBox::OnDestroy();	
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:24 PM
// Function name	: CBitComboBox::DrawItem
// Description	    : Draws each item selected and unselected
// Return type		: void 
// Argument         : LPDRAWITEMSTRUCT lpDrawItemStruct
//----------------------------------------------------------------------
void CBitComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(!IsWindow(m_listbox.m_hWnd))
		return;

	// TODO: Add your code to draw the specified item
	//TRACE("DrawItem()\n");	
	CDC			pDC;	
	pDC.Attach(lpDrawItemStruct->hDC);
	CBrush pBrush;
	CString		sItem;

	if(lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		pBrush.CreateSolidBrush(m_colBackGroundHighLight);
		pDC.FillRect(&lpDrawItemStruct->rcItem, &pBrush);
		pDC.SetTextColor(m_colTextHighLight);
	}
	else
	{
		pBrush.CreateSolidBrush(m_colBackGroundNormal);
		pDC.FillRect(&lpDrawItemStruct->rcItem, &pBrush);
		pDC.SetTextColor(m_colTextNormal);
	}

	if( lpDrawItemStruct->itemID == -1 )
	{
		if(lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
			Invalidate();
		return;
	}
	
	//Draw the left image of the list box
	DrawListLeft();
	//Draw the right image of the list box
	DrawListRight();
	//Draw the bottom for the last item
	if( (int)lpDrawItemStruct->itemID == m_listbox.GetCount()-1 )
		DrawListBot();
	//Draw the top for the first item
	if( lpDrawItemStruct->itemID == 0 )
		DrawListTop();

	// Copy the text of the item to a string
	this->GetLBText(lpDrawItemStruct->itemID, sItem);
	pDC.SetBkMode(TRANSPARENT);

	// Draw the text after the images left postion
	lpDrawItemStruct->rcItem.left = nListBoxTextLeft;
	pDC.SelectObject(m_listbox.GetFont());
	pDC.DrawText(sItem, &lpDrawItemStruct->rcItem, DT_VCENTER | DT_SINGLELINE);

	if(lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
		Invalidate();
}

//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:20 PM
// Function name	: CBitComboBox::SetComboBitmap
// Description	    : To save the resource IDs of images for main combo box skin
// Return type		: void 
// Argument         : UINT nIDLeft
// Argument         : UINT nIDRight
// Argument         : UINT nIDCenter
//----------------------------------------------------------------------
void CBitComboBox::SetComboBitmap(UINT nIDLeft, UINT nIDRight, UINT nIDCenter, UINT nIDRightdown )
{
	m_nIDCenter = nIDCenter; m_nIDLeft = nIDLeft; m_nIDRight = nIDRight;
	m_nIDRightdown = nIDRightdown;
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:18:08 PM
// Function name	: CBitComboBox::SetComboListBitmap
// Description	    : To save the resource IDs of images for list box skin
// Return type		: void 
// Argument         : UINT nIDListLeft
// Argument         : UINT nIDListRight
// Argument         : UINT nIDListTop
// Argument         : UINT nIDListBot
//----------------------------------------------------------------------
void CBitComboBox::SetComboListBitmap(UINT nIDListLeft, UINT nIDListRight, UINT nIDListTop, UINT nIDListBot)
{
	m_nIDListLeft = nIDListLeft; m_nIDListRight = nIDListRight, m_nIDListTop = nIDListTop, m_nIDListBot = nIDListBot;
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:17 PM
// Function name	: CBitComboBox::DrawListLeft
// Description	    : Draws the left image of the list box
// Return type		: void 
// Argument         : int nHeight
//----------------------------------------------------------------------
void CBitComboBox::DrawListLeft(int nHeight)
{
	RECT rc;
	CDC MemDC;
	CDC *pDC = m_listbox.GetDC();
	MemDC.CreateCompatibleDC(pDC);
	m_listbox.GetClientRect(&rc);

	CBitmap bmpImage;
	BITMAP bitImage;

	if( m_nIDListLeft )
	{
		bmpImage.LoadBitmap(m_nIDListLeft);
		bmpImage.GetBitmap(&bitImage);
		MemDC.SelectObject(bmpImage);
		pDC->StretchBlt(rc.left,rc.top,bitImage.bmWidth,rc.bottom,&MemDC,0,0,bitImage.bmWidth,bitImage.bmHeight,SRCCOPY);
	}
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:03:10 PM
// Function name	: CBitComboBox::DrawListRight
// Description	    : Draws the right image of the list box
// Return type		: void 
// Argument         : int nHeight
//----------------------------------------------------------------------
void CBitComboBox::DrawListRight(int nHeight)
{
	RECT rc;
	CDC MemDC;
	CDC *pDC = m_listbox.GetDC();
	MemDC.CreateCompatibleDC(pDC);
	m_listbox.GetClientRect(&rc);

	CBitmap bmpImage;
	BITMAP bitImage;

	if( m_nIDListRight )
	{
		bmpImage.LoadBitmap(m_nIDListRight);
		bmpImage.GetBitmap(&bitImage);
		MemDC.SelectObject(bmpImage);
		pDC->StretchBlt(rc.right-bitImage.bmWidth,rc.top,bitImage.bmWidth,rc.bottom,&MemDC,0,0,bitImage.bmWidth,bitImage.bmHeight,SRCCOPY);
	}
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:02:36 PM
// Function name	: CBitComboBox::DrawListTop
// Description	    : Draws the top image of the list box
// Return type		: void 
// Argument         : int nWidth
//----------------------------------------------------------------------
void CBitComboBox::DrawListTop(int nWidth)
{
	RECT rc;
	CDC MemDC;
	CDC *pDC = m_listbox.GetDC();
	MemDC.CreateCompatibleDC(pDC);
	m_listbox.GetClientRect(&rc);

	CBitmap bmpImage;
	BITMAP bitImage;

	if( m_nIDListTop )
	{
		bmpImage.LoadBitmap(m_nIDListTop);
		bmpImage.GetBitmap(&bitImage);
		MemDC.SelectObject(bmpImage);
		pDC->StretchBlt(rc.left,rc.top,rc.right,bitImage.bmHeight,&MemDC,0,0,bitImage.bmWidth,bitImage.bmHeight,SRCCOPY);
	}		

}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:02:31 PM
// Function name	: CBitComboBox::DrawListBot
// Description	    : Draws the bottom image of the list box
// Return type		: void 
// Argument         : int nWidth
//----------------------------------------------------------------------
void CBitComboBox::DrawListBot(int nWidth)
{
	//Width must be check, sometimes we may not need to stretch to the whole control's width
	RECT rc;
	CDC MemDC;
	CDC *pDC = m_listbox.GetDC();
	MemDC.CreateCompatibleDC(pDC);
	m_listbox.GetClientRect(&rc);

	CBitmap bmpImage;
	BITMAP bitImage;

	if( m_nIDListBot )
	{
		bmpImage.LoadBitmap(m_nIDListBot);
		bmpImage.GetBitmap(&bitImage);
		MemDC.SelectObject(bmpImage);
		pDC->StretchBlt(rc.left,rc.bottom-bitImage.bmHeight,rc.right,bitImage.bmHeight,&MemDC,0,0,bitImage.bmWidth,bitImage.bmHeight,SRCCOPY);
	}

}



//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:13:57 PM
// Function name	: CBitComboBox::SetHighlightColor
// Description	    : To save the color values for text and background 
//						to draw the listbos in highlight position. Must be set before using the CBitComboBox
// Return type		: void 
// Argument         : COLORREF colBackGround
// Argument         : COLORREF colText
//----------------------------------------------------------------------
void CBitComboBox::SetHighlightColor(COLORREF colBackGround, COLORREF colText)
{
	m_colBackGroundHighLight = colBackGround; m_colTextHighLight = colText;
}


//----------------------------------------------------------------------
// Author			: Ashok Jaiswal
// Date Time		: 3/23/2004 5:13:54 PM
// Function name	: CBitComboBox::SetNormalPositionColor
// Description	    : To save the color values for text and background 
//						to draw the listbos in normal position. Must be set before using the CBitComboBox
// Return type		: void 
// Argument         : COLORREF colBackGround
// Argument         : COLORREF colText
//----------------------------------------------------------------------
void CBitComboBox::SetNormalPositionColor(COLORREF colBackGround, COLORREF colText)
{
	m_colBackGroundNormal = colBackGround; m_colTextNormal = colText;
}

void CBitComboBox::ChangeButton()
{
	if(m_nIDRightdown != 0)
	{
		UINT h = m_nIDRight;
		m_nIDRight = m_nIDRightdown;
		m_nIDRightdown = h;
		OnPaint();
	}
}

BOOL CBitComboBox::OnEraseBkgnd(CDC* pDC)
{

	//return CComboBox::OnEraseBkgnd( pDC );
	return FALSE;
}

BOOL CBitComboBox::ShowWindow( int nCmdShow )
{
	if( nCmdShow == SW_HIDE )
	{
		if( ::IsWindow(m_listbox.GetSafeHwnd()) )
			m_listbox.SendMessage( WM_KEYDOWN, VK_ESCAPE, 0 );
	}
	return CComboBox::ShowWindow(nCmdShow);
}