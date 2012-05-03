// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

static CALLBACK MyFORMCallback( HWND hwndQHTM, LPQHTMFORMSubmit pFormSubmit, LPARAM )
{
	HWND hwnd = GetParent( hwndQHTM );
	TCHAR szMain[ 20480 ];	//	20KB should be enough.
	wsprintf( szMain, _T("<h2>QHTM form %s</h2>This demonstrates using QHTM FORMs to catch FORM 'post'.<p>Just set a callback function and you'll then receive everything you need.<p>Form: <i>%s</i>")
		_T("<p>With the following fields:")
		_T("<table border=1 width='200' cellpadding=5><tr><td nowrap><b>Field name</b></td><td nowrap><b>Field value</b></td></tr>")

		, pFormSubmit->pcszName, pFormSubmit->pcszAction );

	for( UINT u = 0; u < pFormSubmit->uFieldCount; u++ )
	{
		TCHAR szField[ 1024 ];
		wsprintf( szField, _T("<tr><td>%s</td><td>%s</td></tr>"), pFormSubmit->parrFields[ u ].pcszName, pFormSubmit->parrFields[ u ].pcszValue );
		_tcscat( szMain, szField );		
	}
	_tcscat( szMain, _T("</table>") );

	QHTM_MessageBox( hwnd, szMain, _T("QHTM Form Submit"), MB_OK );	
}


IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{

}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	m_printContext = QHTM_PrintCreateContext( QHTM_ZOOM_DEFAULT );
	if (!m_printContext)
	{
		AfxMessageBox( _T("Unable to Print!" ));
		return FALSE;
	}
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	if (!DoPreparePrinting(pInfo))
		return FALSE;

	return CView::OnPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo )
{
	CString path = GetDocument()->GetPathName();
	if (path.IsEmpty())
	{
	}
	else
	{
		VERIFY(QHTM_PrintSetTextFile( m_printContext, path));
	}
	// Layout
	CRect rcLayout(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	INT nPages;
	VERIFY(QHTM_PrintLayout( m_printContext, pDC->m_hAttribDC, &rcLayout, &nPages ));
	pInfo->SetMaxPage(nPages);
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	QHTM_PrintDestroyContext(m_printContext);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

int CTestView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect( 0,0,0,0 );
	if( m_pQhtmWnd.Create(QHTM_CLASSNAME, NULL, WS_VISIBLE, rect, this, 102, NULL ) )
	{
		OnFileReload();

		QHTM_SetFormSubmitCallback( m_pQhtmWnd.GetSafeHwnd(), MyFORMCallback );
		return TRUE;
	}

	return 0;
}


void CTestView::OnFileReload() 
{
	CString path = GetDocument()->GetPathName();
	if (path.IsEmpty())
	{
		// Load something from resources.
		//m_pQhtmWnd.LoadFromResource( ID_DEFAULT_HTML );
	}
	else
	{
		m_pQhtmWnd.LoadFromFile(path);
	}
}


void CTestView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() && m_pQhtmWnd.GetSafeHwnd() )
	{
		CRect rect;
		GetClientRect(rect);
		m_pQhtmWnd.SetWindowPos(NULL, 0, 0, rect.right, rect.bottom, SWP_NOZORDER);
	}

}


void CTestView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	OnFileReload();
}

void CTestView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	VERIFY( QHTM_PrintPage( m_printContext, pDC->GetSafeHdc(), pInfo->m_nCurPage - 1, &pInfo->m_rectDraw ) );	
}
