// TestList.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_TREE	123

#define	WM_MY_FIRST_SELECT	(WM_USER + 1967 )

static CString GetModulePath( HINSTANCE hInst /*= NULL*/ )
{
	TCHAR szBuffer[_MAX_PATH * 2];
	VERIFY( GetModuleFileName( hInst, szBuffer, _MAX_PATH ) );

	LPTSTR pszDelim = _tcsrchr( szBuffer, _T('\\') );
	if( pszDelim )
	{
		*pszDelim = _T('\0');
	}
	return szBuffer;
}


/////////////////////////////////////////////////////////////////////////////
// CTestList

CTestList::CTestList()
{
	m_strLastLoaded = AfxGetApp()->GetProfileString( _T("settings"), _T("lastfileLoaded") );
}

CTestList::~CTestList()
{
	AfxGetApp()->WriteProfileString( _T("settings"), _T("lastfileLoaded"), m_strLastLoaded );
}


BEGIN_MESSAGE_MAP(CTestList, baseCMyBar)
	//{{AFX_MSG_MAP(CTestList)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelectionChanged)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_MY_FIRST_SELECT, OnMyFirstSelect )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTestList message handlers

BOOL CTestList::Create( CWnd* pParentWnd )
{
  BOOL b = CSizingControlBarG::Create( _T("Test list"), pParentWnd, 1967, WS_CLIPCHILDREN );

	SetSCBStyle(GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD);

  if( !b )
		return FALSE;

	SetBarStyle( GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	m_treeList.Create( WS_VISIBLE | WS_CHILD | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS, CRect( 0, 0, 0, 0 ), this, IDC_TREE );

	if( !AddFilesFromPath( GetModulePath( NULL ) + _T("\\Test files\\"), TVI_ROOT ) )
	{
		AddFilesFromPath( GetModulePath( NULL ) +  _T("../../\\components\\QHTM\\Test\\Test files\\"), TVI_ROOT );
	}


	CFont font;
	font.CreateStockObject( DEFAULT_GUI_FONT );
	m_treeList.SetFont( &font );

  return TRUE;
}


void CTestList::OnSize(UINT nType, int cx, int cy) 
{
	baseCMyBar::OnSize(nType, cx, cy);
	
  if ((m_dwSCBStyle & SCBS_SIZECHILD) != 0)
  {
    // automatic child resizing - only one child is allowed
    CWnd* pWnd = GetWindow(GW_CHILD);
    if (pWnd != NULL)
    {
			CRect rc( 0, 0, cx, cy);
      m_treeList.MoveWindow( rc.left, rc.top , rc.Width(), rc.Height() );
    }
  }
	
}


bool CTestList::AddFilesFromPath( LPCTSTR pcszPath, HTREEITEM htreeParent )
{
	CString strPath( pcszPath );
	strPath += _T("\\*.*");

	CFileFind finder;
	BOOL bFound = finder.FindFile( strPath );
	while( bFound )
	{
		bFound = finder.FindNextFile();
		if( !finder.IsDots() )
		{

			CString strName( finder.GetFileName() );
			if( strName.Find( _T(".html") ) != -1 || finder.IsDirectory() )
			{
				TVITEM tvi; 
				TVINSERTSTRUCT tvins; 

				tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
				UINT uItem = m_arrFile.GetSize();
				const CString strFilePath( finder.GetFilePath() );
				m_arrFile.Add( strFilePath );

				// Set the text of the item. 
				tvi.pszText = (LPTSTR)(LPCTSTR)strName; 
				tvi.cchTextMax = strName.GetLength(); 

				tvi.iImage = tvi.iSelectedImage = 0;

				tvi.lParam = (LPARAM) uItem; 

				tvins.item = tvi; 
				tvins.hInsertAfter = TVI_LAST;

				tvins.hParent = htreeParent;

				//
				// Add the item to the tree-view control. 
				HTREEITEM hTree = (HTREEITEM)m_treeList.SendMessage( TVM_INSERTITEM, 0, (LPARAM) (LPTVINSERTSTRUCT) &tvins);
				
				if( strFilePath == m_strLastLoaded )
				{
					PostMessage( WM_MY_FIRST_SELECT, (WPARAM)hTree, 0 );
				}

				if( finder.IsDirectory() )
				{
					CString strPath( pcszPath );
					strPath += _T("\\");
					strPath += finder.GetFileName();
					AddFilesFromPath( strPath, hTree );
				}

			}

		}
	}
	return false;
}


void CTestList::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	UINT u = m_treeList.GetItemData( pNMTreeView->itemNew.hItem );

	CString strFile( m_arrFile[ u ] );

	if( strFile.Find( _T(".html") ) != -1 )
	{
		m_strLastLoaded = m_arrFile[ u ];
		AfxGetApp()->OpenDocumentFile( m_arrFile[ u ] );
	}

	*pResult = 0;
}


LRESULT CTestList::OnMyFirstSelect( WPARAM wParam, LPARAM)
{
	HTREEITEM hTree = (HTREEITEM)wParam;
	HTREEITEM hItem = hTree;
	do
	{
		hItem = m_treeList.GetParentItem( hItem );
		if( hItem )
		{
			m_treeList.Expand( hItem, TVE_EXPAND );
			m_treeList.EnsureVisible( hItem );	
		}
	}
	while( hItem );
	m_treeList.Select( hTree, TVGN_CARET );
	m_treeList.EnsureVisible( hTree );	
	return 0;
}