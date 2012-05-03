// SeverMessageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XPatch.h"
#include "SeverMessageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeverMessageDialog dialog


CSeverMessageDialog::CSeverMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSeverMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeverMessageDialog)
	m_MessageString = _T("");
	//}}AFX_DATA_INIT
}


void CSeverMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeverMessageDialog)
	DDX_Text(pDX, IDC_MESSAGEEDIT, m_MessageString);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeverMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CSeverMessageDialog)	
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeverMessageDialog message handlers




void CSeverMessageDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSeverMessageDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CSeverMessageDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDC* pDC = GetDC();	
	m_BackgroundImage.Create( pDC, IDB_MBBACKBITMAP );
	ReleaseDC( pDC );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSeverMessageDialog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rect;
	GetWindowRect( &rect );

	for( int i = 0; i < rect.bottom-rect.top ; i+=m_BackgroundImage.m_Size.cy )
	{
		for( int j = 0; j < rect.right-rect.left ; j+=m_BackgroundImage.m_Size.cx )
		{
			m_BackgroundImage.PasteToDC( pDC, j, i );
		}
	}
	
	return FALSE; 	
	//return CDialog::OnEraseBkgnd(pDC);
}
