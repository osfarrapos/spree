// BitListBox.cpp : implementation file
//

#include "stdafx.h"
#include "BitListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitListBox

CBitListBox::CBitListBox()
{
}

CBitListBox::~CBitListBox()
{
}


BEGIN_MESSAGE_MAP(CBitListBox, CListBox)
	//{{AFX_MSG_MAP(CBitListBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitListBox message handlers

void CBitListBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT rc;
	GetClientRect(&rc);
	
	CBrush br;
	br.CreateSolidBrush(RGB(115,138,154));
	dc.FillRect(&rc,&br);
	
	// Do not call CListBox::OnPaint() for painting messages
}
