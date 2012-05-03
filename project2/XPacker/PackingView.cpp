// PackingView.cpp : implementation file
//

#include "stdafx.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>

#include "XPacker.h"
#include "PackingView.h"
#include "XPackerDlg.h"

#include "NewPackageFileDlg.h"
#include "NewSectionDlg.h"

#include "DirDialog.h"

#include "SetRootPathDlg.h"

#include "Zip\\ZipArchive.h"

#include "WavePackage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPackingView

IMPLEMENT_DYNCREATE(CPackingView, CDialog)


template <class A> inline void _SWAP(A &a, A &b)
{   A   c;
    c = a;
    a = b;
    b = c;
}

LPSTR getExtention( LPSTR String )
{	  
	int strSize = strlen( String );
	while( --strSize )
	{
		if( String[strSize]== '.' ) return String+strSize;
	}
	return String;
}

extern BOOL CheckExistFile( LPCSTR filename, BOOL resetAttrib );

#define ID_XPACKSTRUCTFILE_HEADER         0x32FB
#define ID_XPACKSTRUCTFILE_VERSION        0xECEF

#define _HEADERINDICATOR_PACKAGEFILE            0xFEFA
#define _HEADERINDICATOR_PACKAGEFILEVERSION		0xA001

typedef struct 
{

	WORD HeaderIndicator;
	WORD FileVersion;	
	int  PackedFileCount;

} _PACKAGEFILE_HEADER;


#define _ENCODEKEY		0xEF

void _EncodeData( LPSTR string, BYTE key )
{
	if( !string ) return;
	ASSERT( string );

	int length = strlen( string );

	LPSTR tptr = string;

	for( int i = 0; i < length; i++ )
	{
		*tptr ^= key;
		tptr++;
	}
}


CPackingView::CPackingView(CWnd* pParent)
	: CDialog(CPackingView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPackingView)
	m_TotalSize = _T("");
	m_TotalCount = _T("");
	m_SelectedPackagename = _T("");
	m_SelectedSectionname = _T("");
	m_Zipping = FALSE;
	m_ConvertTGAToDDS = FALSE;
	//}}AFX_DATA_INIT
	
	char ModulePath[_MAX_PATH];
	memset( ModulePath, 0, _MAX_PATH );
	GetCurrentDirectory( _MAX_PATH-1, ModulePath );
		
	m_PathString = _T(ModulePath);
	m_PackingPath = _T(ModulePath);
}

CPackingView::~CPackingView()
{
	m_FilePackage.deleteAllItem();
	if( pTreeimagelist ) delete pTreeimagelist;
}

void CPackingView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPackingView)
	DDX_Control(pDX, IDC_FILELIST, m_FileListCtrl);
	DDX_Control(pDX, IDC_PACKINGTREE, m_PackingTreeControl);
	DDX_Text(pDX, IDC_TOTALSIZESTATIC, m_TotalSize);
	DDX_Text(pDX, IDC_TOTALCOUNTSTATIC, m_TotalCount);
	DDX_Text(pDX, IDC_PACKAGENAMEEDIT, m_SelectedPackagename);
	DDX_Text(pDX, IDC_SECTIONSTRINGEDIT, m_SelectedSectionname);	
	DDX_Check(pDX, IDC_ZIPPINGCHECK, m_Zipping);
	DDX_Check(pDX, IDM_CONVERTTGATODDS, m_ConvertTGAToDDS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPackingView, CDialog)
	//{{AFX_MSG_MAP(CPackingView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PACKINGTREE, OnSelchangedPackingtree)
	ON_NOTIFY(NM_RCLICK, IDC_PACKINGTREE, OnRclickPackingtree)
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_COMMAND(ID_INSERTMENUITEM, OnInsertmenuitem)
	ON_COMMAND(ID_DELETEMENUITEM, OnDeletemenuitem)
	ON_COMMAND(ID_INSERTSECTIONMENUITEM, OnInsertsectionmenuitem)
	ON_COMMAND(ID_DELETESECTIONMENUITEM, OnDeletesectionmenuitem)
	ON_COMMAND(ID_INSERTFILESMENUITEM, OnInsertfilesmenuitem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, OnItemchangedFilelist)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_PACKINGTREE, OnEndlabeleditPackingtree)
	ON_COMMAND(IDM_DELETEFILELISTITEM, OnDeletefilelistitem)
	ON_COMMAND(IDM_DELETEALLFILELISTITEM, OnDeleteallfilelistitem)
	ON_BN_CLICKED(IDM_MAKEPACKAGE, OnPacking)
	ON_BN_CLICKED(IDM_EXTRACT, OnExtract)
	ON_COMMAND(IDM_PACKLISTLOAD, OnPacklistload)
	ON_COMMAND(IDM_PACKLISTSAVE, OnPacklistsave)
	ON_COMMAND(IDM_RESETPACKINGLIST, OnResetpackinglist)
	ON_WM_KEYDOWN()
	ON_NOTIFY(TVN_KEYDOWN, IDC_PACKINGTREE, OnKeydownPackingtree)
	ON_NOTIFY(LVN_KEYDOWN, IDC_FILELIST, OnKeydownFilelist)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDM_SORTFILELIST, OnSortfilelist)
	ON_BN_CLICKED(ID_MAKEPATCHLIST, OnMakepatchlist)	
	ON_BN_CLICKED(IDM_MAKEALLPACKAGE, OnMakeallpackage)
	ON_BN_CLICKED(IDM_MAKEWAVEPACKAGE, OnMakewavepackage)
	ON_BN_CLICKED(IDC_ZIPPINGCHECK, OnZippingcheck)
	ON_BN_CLICKED(ID_INSERTFILESMENUITEM, OnInsertfilesmenuitem)
	ON_BN_CLICKED(IDM_CONVERTTGATODDS, OnConverttgatodds)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPackingView diagnostics

#ifdef _DEBUG
void CPackingView::AssertValid() const
{
	CDialog::AssertValid();
}

void CPackingView::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPackingView message handlers


int CPackingView::getSelectItemSequence( HTREEITEM rghItem )
{
	int        selectPosition = 0;
	CString    ItemText;
	
	HTREEITEM  parentItem = this->m_PackingTreeControl.GetParentItem( rghItem );
	HTREEITEM  childItem = this->m_PackingTreeControl.GetChildItem( parentItem );
					
	while( TRUE )
	{
		if( childItem == rghItem ) return selectPosition;
		childItem = this->m_PackingTreeControl.GetNextItem( childItem, TVGN_NEXT );
		ItemText = this->m_PackingTreeControl.GetItemText( childItem );
		if( (ItemText.GetBuffer(256))[0] == NULL ) break;
		selectPosition++;
	}

	return -1;
}

int CPackingView::getTreeLevel(HTREEITEM rghItem)
{	
	int Level = -1;
		
	HTREEITEM parentItem = rghItem;
	// 부모 아이템으로 거슬러 올라가면서 레벨 계산
	while( parentItem != NULL )
	{
		parentItem = this->m_PackingTreeControl.GetParentItem( parentItem );
		Level++;
	}

	return Level;
}

int CPackingView::getOnelevelSubTreeCount( HTREEITEM parentItem )
{
	int Count = 0;
	HTREEITEM childItem = NULL;
	
	childItem = m_PackingTreeControl.GetChildItem( parentItem );
		
	if( !childItem ) return 0;
	Count++;
	
	while( 1 )
	{
		childItem = m_PackingTreeControl.GetNextItem( childItem, TVGN_NEXT );
		if( !childItem ) break;
		Count++;
	}
	return Count;
}



void CPackingView::resetTree()
{
	m_FilePackage.deleteAllItem();

	this->m_PackingTreeControl.DeleteAllItems();
			
	this->m_PackingTreeControl.SelectItem( NULL );

	m_FileListCtrl.DeleteAllItems();
		
	m_FileListCtrl.EnableWindow( FALSE );
	GetDlgItem( ID_INSERTFILESMENUITEM )->EnableWindow( FALSE );
	GetDlgItem( IDM_DELETEFILELISTITEM )->EnableWindow( FALSE );
	GetDlgItem( IDM_DELETEALLFILELISTITEM )->EnableWindow( FALSE );

	m_SelectedPackagename = _T("");
	m_SelectedSectionname = _T("");

	UpdateData( FALSE );	

	((CXPackerDlg*)((CXPackerApp*)AfxGetApp())->m_pMainWnd)->m_LoadedFilename = _T("");
	((CXPackerDlg*)((CXPackerApp*)AfxGetApp())->m_pMainWnd)->UpdateData( FALSE );	
}

HTREEITEM CPackingView::insertNode(HTREEITEM Parent, LPSTR Text, HTREEITEM InsertAfter)
{		
	TV_INSERTSTRUCT tvstruct;
	tvstruct.hParent = Parent;
	tvstruct.hInsertAfter = InsertAfter;
	tvstruct.item.pszText = Text;	
	tvstruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	if( Parent == NULL ) tvstruct.item.iSelectedImage = tvstruct.item.iImage = 0;
	else tvstruct.item.iSelectedImage = tvstruct.item.iImage = 1;

	return this->m_PackingTreeControl.InsertItem( &tvstruct );
	
}

void CPackingView::setCurrentFileItem( void )
{
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = getTreeLevel( selectedItem );
	
	if( selectedItemLevel == 0 )
	{
		m_FileListCtrl.DeleteAllItems();
		
		m_FileListCtrl.EnableWindow( FALSE );
		GetDlgItem( ID_INSERTFILESMENUITEM )->EnableWindow( FALSE );
		GetDlgItem( IDM_DELETEFILELISTITEM )->EnableWindow( FALSE );
		GetDlgItem( IDM_DELETEALLFILELISTITEM )->EnableWindow( FALSE );
		GetDlgItem( IDM_SORTFILELIST )->EnableWindow( FALSE );

		m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( selectedItem );
		m_SelectedSectionname = _T("");

		UpdateData( FALSE );
	}
	else if( selectedItemLevel == 1 )
	{	
		int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );

		if( findsectionitemindex != -1 )
		{			
			m_FileListCtrl.EnableWindow( TRUE );

			GetDlgItem( ID_INSERTFILESMENUITEM )->EnableWindow( TRUE );
			GetDlgItem( IDM_DELETEFILELISTITEM )->EnableWindow( TRUE );
			GetDlgItem( IDM_DELETEALLFILELISTITEM )->EnableWindow( TRUE );
			GetDlgItem( IDM_SORTFILELIST )->EnableWindow( TRUE );

			setItem( findsectionitemindex );

			m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( this->m_PackingTreeControl.GetParentItem( selectedItem ) );
			m_SelectedSectionname = this->m_PackingTreeControl.GetItemText( selectedItem );

			UpdateData( FALSE );
		}
	}
	
}

void CPackingView::OnSelchangedPackingtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	setCurrentFileItem(  );
	
	*pResult = 0;
}


BOOL CPackingView::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	m_Level1ContextMenu.CreatePopupMenu();
	m_Level2ContextMenu.CreatePopupMenu();
		
	m_Level1ContextMenu.AppendMenu( MF_STRING, ID_INSERTSECTIONMENUITEM, "New se&ction\tInsert" );
	
	m_Level1ContextMenu.AppendMenu( MF_SEPARATOR );	
	m_Level1ContextMenu.AppendMenu( MF_STRING, ID_INSERTMENUITEM, "&New package\tCtrl+Insert" );	

	m_Level1ContextMenu.AppendMenu( MF_SEPARATOR );	
	m_Level1ContextMenu.AppendMenu( MF_STRING, ID_DELETEMENUITEM, "&Delete \tDel" );		
	m_Level1ContextMenu.AppendMenu( MF_SEPARATOR );	
	m_Level1ContextMenu.AppendMenu( MF_STRING, IDM_RESETPACKINGLIST, "New &package structure" );
	m_Level1ContextMenu.AppendMenu( MF_STRING, IDM_PACKLISTLOAD, "&Load package structure" );
	m_Level1ContextMenu.AppendMenu( MF_STRING, IDM_PACKLISTSAVE, "&Save package structure" );
	m_Level1ContextMenu.AppendMenu( MF_STRING, IDM_MAKEPACKAGE, "&Make package file" );
	m_Level1ContextMenu.AppendMenu( MF_STRING, IDM_MAKEWAVEPACKAGE, "Make &wave package file" );
		
	m_Level2ContextMenu.AppendMenu( MF_STRING, ID_INSERTFILESMENUITEM, "&Add files\tInsert" );	
	m_Level2ContextMenu.AppendMenu( MF_STRING, ID_DELETESECTIONMENUITEM, "&Delete section\tDel" );
	m_Level2ContextMenu.AppendMenu( MF_SEPARATOR );	
	m_Level2ContextMenu.AppendMenu( MF_STRING, IDM_RESETPACKINGLIST, "New &package structure" );
	m_Level2ContextMenu.AppendMenu( MF_STRING, IDM_PACKLISTLOAD, "&Load package structure" );
	m_Level2ContextMenu.AppendMenu( MF_STRING, IDM_PACKLISTSAVE, "&Save package structure" );
	m_Level2ContextMenu.AppendMenu( MF_STRING, IDM_MAKEPACKAGE, "&Make package file" );
	m_Level2ContextMenu.AppendMenu( MF_STRING, IDM_MAKEWAVEPACKAGE, "Make &wave package file" );
	
	m_ListControlContextMenu.CreatePopupMenu();
	m_ListControlContextMenu.AppendMenu( MF_STRING, ID_INSERTFILESMENUITEM, "&Add files\tInsert" );	
	m_ListControlContextMenu.AppendMenu( MF_STRING, IDM_DELETEFILELISTITEM, "&Delete\tDel" );
	m_ListControlContextMenu.AppendMenu( MF_STRING, IDM_DELETEALLFILELISTITEM, "Delete all" );
	m_ListControlContextMenu.AppendMenu( MF_SEPARATOR );	
	m_ListControlContextMenu.AppendMenu( MF_STRING, IDM_MAKEPACKAGE, "&Make package file" );
	m_ListControlContextMenu.AppendMenu( MF_STRING, IDM_MAKEWAVEPACKAGE, "Make &wave package file" );
	m_ListControlContextMenu.AppendMenu( MF_STRING, IDM_EXTRACT, "&Extract selected file" );
	m_ListControlContextMenu.AppendMenu( MF_STRING, IDM_MAKEPATCHLIST, "Make &patch list" );

	
	// File list image list-----------------------------------------------------------------------------------
	
	SHFILEINFO rInfo;
	HIMAGELIST hImglist;

    hImglist = (HIMAGELIST) SHGetFileInfo ( _T(""), 0, &rInfo, sizeof(SHFILEINFO), 
												SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX );
    m_imglist.Attach ( hImglist );

    m_FileListCtrl.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_FileListCtrl.SetImageList ( &m_imglist, LVSIL_SMALL );	
	

	RECT rect;
	m_FileListCtrl.GetClientRect( &rect );

	m_FileListCtrl.InsertColumn( 0, "Filename", LVCFMT_LEFT, rect.right-rect.left - 160 );
	m_FileListCtrl.InsertColumn( 1, "Type", LVCFMT_LEFT, 80 );	
	m_FileListCtrl.InsertColumn( 2, "Size", LVCFMT_RIGHT, 60 );	
	
	m_FileListCtrl.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP );

	m_FileListCtrl.EnableWindow( FALSE );

	
	// Tree image list-----------------------------------------------------------------------------------
	pTreeimagelist = new CImageList;
	pTreeimagelist->Create( 16, 16, ILC_COLOR8, 1, 1 );

	m_PackingTreeControl.SetImageList( pTreeimagelist, TVSIL_NORMAL );
	//m_PackingTreeControl.SetImageList( pTreeimagelist, TVSIL_STATE  );

	pTreeimagelist->Add( AfxGetApp()->LoadIcon(IDI_PACKAGEICON) );
	pTreeimagelist->Add( AfxGetApp()->LoadIcon(IDI_SECTIONICON) );

	    
	// TODO: Add extra initialization here
	resetTree();

	progressdlg.Create( this );
	

	if( ((CXPackerApp*)AfxGetApp())->cmdInfo.m_strFileName.GetLength() > 0 )
	{
		loadXPP( ((CXPackerApp*)AfxGetApp())->cmdInfo.m_strFileName.GetBuffer(256) );

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPackingView::OnRclickPackingtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
/*	
	POINT mPos;	
	UINT Flags;
	GetCursorPos( &mPos );
	this->m_PackingTreeControl.ScreenToClient( &mPos );
		
	HTREEITEM selectItem = this->m_PackingTreeControl.HitTest( mPos, &Flags );
	this->m_PackingTreeControl.SelectItem( selectItem );
*/
	*pResult = 0;
}


void CPackingView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = 0;

	selectedItemLevel = getTreeLevel( selectedItem );

	CPoint glpoint;
	GetCursorPos( &glpoint );

	RECT treecontrolrect;
	m_PackingTreeControl.GetWindowRect( &treecontrolrect );
	
	RECT listcontrolrect;
	m_FileListCtrl.GetWindowRect( &listcontrolrect );

	if( treecontrolrect.left < point.x && treecontrolrect.right  > point.x && 
		treecontrolrect.top  < point.y && treecontrolrect.bottom > point.y	)
	{
		if( selectedItemLevel == -1 || selectedItemLevel == 0 )
		{
			m_Level1ContextMenu.TrackPopupMenu( TPM_LEFTALIGN, glpoint.x, glpoint.y, this );		
		}
		else if( selectedItemLevel == 1 )
		{
			m_Level2ContextMenu.TrackPopupMenu( TPM_LEFTALIGN, glpoint.x, glpoint.y, this );		
		}	
	}
	else if( listcontrolrect.left < point.x && listcontrolrect.right  > point.x && 
			 listcontrolrect.top  < point.y && listcontrolrect.bottom > point.y	)	
	{
		if( selectedItemLevel == 1 )
			m_ListControlContextMenu.TrackPopupMenu( TPM_LEFTALIGN, glpoint.x, glpoint.y, this );
	}

}

void CPackingView::OnDestroy() 
{
	CDialog::OnDestroy();
	
	this->m_Level1ContextMenu.DestroyMenu();
	this->m_Level2ContextMenu.DestroyMenu();

	this->m_ListControlContextMenu.DestroyMenu();

	if ( NULL != m_imglist.GetSafeHandle() ) m_imglist.Detach();
	// TODO: Add your message handler code here
	
}

void CPackingView::OnInsertmenuitem() 
{
	// TODO: Add your command handler code here
	CNewPackageFileDlg newpackagefiledlg;

	if( newpackagefiledlg.DoModal() == IDOK )
	{
		HTREEITEM newItem = insertNode( NULL, (LPSTR)newpackagefiledlg.m_PackageName.operator LPCTSTR() );

		this->m_PackingTreeControl.SelectItem( newItem );

		m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( newItem );
		m_SelectedSectionname = _T("");

		UpdateData( FALSE );
	}
	
}

void CPackingView::OnDeletemenuitem() 
{
	// TODO: Add your command handler code here
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	if( AfxMessageBox( "Delete this package list ?", MB_YESNO ) == IDYES )
	{
					
		HTREEITEM childItem = this->m_PackingTreeControl.GetChildItem( selectedItem );

		int finditem;

		if( childItem != NULL )
		{
			while( 1 )
			{	
				finditem = -1;

				finditem = this->m_FilePackage.findLinkedTreeItem( childItem );

				if( finditem != -1 )
				{
					this->m_FilePackage.deleteItem( finditem );
				}

				childItem = this->m_PackingTreeControl.GetNextItem( childItem, TVGN_NEXT );
				
				if( childItem == NULL ) break;
			}
		}

		this->m_PackingTreeControl.DeleteItem( selectedItem );
	}
	
}

void CPackingView::OnInsertsectionmenuitem() 
{
	// TODO: Add your command handler code here

	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = 0;

	selectedItemLevel = getTreeLevel( selectedItem );

	if( selectedItemLevel != 0 ) return;


	CNewSectionDlg newsectiondlg;

	if( newsectiondlg.DoModal() == IDOK )
	{
		CSectionList* newsection;
		
		newsection = new CSectionList;

		if( !newsection )
		{
			AfxMessageBox("ERROR : Create section" );
			return;
		}
		
		this->m_FilePackage.insertItem( newsection );

		HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();
		
		HTREEITEM newItem = insertNode( selectedItem, (LPSTR)newsectiondlg.m_SectionNameString.operator LPCTSTR() );
		this->m_PackingTreeControl.SelectItem( newItem );
			
		m_FileListCtrl.EnableWindow( TRUE );
		GetDlgItem( ID_INSERTFILESMENUITEM )->EnableWindow( TRUE );
		GetDlgItem( IDM_DELETEFILELISTITEM )->EnableWindow( TRUE );
		GetDlgItem( IDM_DELETEALLFILELISTITEM )->EnableWindow( TRUE );
		
		m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( this->m_PackingTreeControl.GetParentItem( newItem ) );
		m_SelectedSectionname = this->m_PackingTreeControl.GetItemText( newItem );

		UpdateData( FALSE );

		newsection->setLinkedTreeItem( newItem );

		this->m_PackingTreeControl.SelectItem( newItem );		
	}	
}

void CPackingView::OnDeletesectionmenuitem() 
{
	// TODO: Add your command handler code here
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();
	
	if( AfxMessageBox( "Delete this section ?", MB_YESNO ) == IDYES )
	{		
		int finditem = this->m_FilePackage.findLinkedTreeItem( selectedItem );

		if( finditem != -1 )
		{
			this->m_FilePackage.deleteItem( finditem );
		}
		
		this->m_PackingTreeControl.DeleteItem( selectedItem );
	}	
}

#define _MAX_LOADFILECOUNT    100



BOOL CPackingView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CPackingView::OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

#include "Shlwapi.h"

void CPackingView::setItem( int SectionIndex )
{
	CSectionList* selectedsectionlist = NULL;
		
	selectedsectionlist = this->m_FilePackage.getSectionList( SectionIndex );

	if( !selectedsectionlist ) return;

	m_FileListCtrl.DeleteAllItems();

	CPackFileItem*  tfileitem = NULL;
	int				totalsize = 0;	
	TCHAR			szFileLen [64];
	LVITEM			rItem;

	CString finalfilename;

	for( int i = 0; i < selectedsectionlist->m_itemcount; i++ )
	{
		tfileitem = NULL;
		tfileitem = selectedsectionlist->getStringItem( i );

		if( !tfileitem )
		{
			AfxMessageBox( "ERROR : access file item" );
			m_FileListCtrl.DeleteAllItems();
			break;
		}

		ZeroMemory ( &rItem, sizeof(LVITEM) );

		finalfilename = tfileitem->rFileInfo.szDisplayName;
		finalfilename.Replace( m_PathString.operator LPCTSTR(), "" );

        rItem.mask    = LVIF_IMAGE | LVIF_TEXT;
        rItem.iItem   = i;
        rItem.pszText = finalfilename.GetBuffer(256);
        rItem.iImage  = tfileitem->rFileInfo.iIcon;

		if ( tfileitem->rFileInfo.dwAttributes & SFGAO_GHOSTED )
        {
			rItem.mask |= LVIF_STATE;
			rItem.state = rItem.stateMask = LVIS_CUT;
        }

		this->m_FileListCtrl.InsertItem( &rItem ); //i, tfileitem->rFileInfo.szDisplayName );

		this->m_FileListCtrl.SetItemText ( i, 1, tfileitem->rFileInfo.szTypeName );

		if( tfileitem->rFileSize != -1 )
		{
			 StrFormatByteSize ( tfileitem->rFileSize, szFileLen, 64 ); //tsizestr.Format( "%d", tfileitem->rFileSize );
			 this->m_FileListCtrl.SetItemText( i, 2, szFileLen );
		}
		else 
		{			
			this->m_FileListCtrl.SetItemText( i, 2, _T("Failed") );
		}		

		if( tfileitem->rFileSize > 0 )	totalsize += tfileitem->rFileSize;
	}

	m_TotalSize.Format( "%d", totalsize );
	m_TotalCount.Format( "%d", selectedsectionlist->m_itemcount );

	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( this->m_PackingTreeControl.GetParentItem( selectedItem ) );
	m_SelectedSectionname = this->m_PackingTreeControl.GetItemText( selectedItem );

	UpdateData( FALSE );
}



void CPackingView::OnEndlabeleditPackingtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	if( pTVDispInfo->item.pszText )
	{		
		HTREEITEM selectedItem = m_PackingTreeControl.GetSelectedItem(); 
		
		m_PackingTreeControl.SetItemText( selectedItem, pTVDispInfo->item.pszText );

		int selectedItemLevel = getTreeLevel( selectedItem );

		if( selectedItemLevel == 0 )
		{
			m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( selectedItem );
			m_SelectedSectionname = _T("");
		}
		else if( selectedItemLevel == 1 )
		{
			m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( this->m_PackingTreeControl.GetParentItem( selectedItem ) );
			m_SelectedSectionname = this->m_PackingTreeControl.GetItemText( selectedItem );
		}

		UpdateData( FALSE );
	}	
	
	*pResult = 0;
}

BOOL CPackingView::insertFileItem( LPCTSTR filename )
{
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = 0;

	selectedItemLevel = getTreeLevel( selectedItem );

	if( selectedItemLevel != 1 ) return FALSE;

	int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );

	if( findsectionitemindex == -1 )
	{
		AfxMessageBox( "ERROR : Find linked tree item" );
		return FALSE;
	}

	CSectionList* selectedsectionlist = NULL;
	
	selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

	if( !selectedsectionlist )
	{
		AfxMessageBox( "ERROR : Find linked item pointer" );
		return FALSE;
	}

	CString orgfile = filename;
	if( orgfile.Find( m_PathString, 0 ) == -1 )
	{
		CString messagestr;
		messagestr.Format( "WARNING : 프로젝트 파일 하부폴더에 있는 파일만 삽입할 수 있습니다.\n\tFile : %s\n\tProject folder : %s",
							filename,
							m_PathString );
		AfxMessageBox( messagestr );
		return TRUE;
	}

	CPackFileItem* fileitem = NULL;
		
	fileitem = new CPackFileItem;			

	if( fileitem )
	{	
		fileitem->setFileInformations( filename );

		selectedsectionlist->insertItem( fileitem );		
	}
	else
	{
		AfxMessageBox( "ERROR : Create file item" );
		return FALSE;
	}

	return TRUE;
}



void CPackingView::OnInsertfilesmenuitem() 
{
	// TODO: Add your command handler code here
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = 0;

	selectedItemLevel = getTreeLevel( selectedItem );

	if( selectedItemLevel != 1 ) return;

	char lpstr[ _MAX_PATH * _MAX_LOADFILECOUNT ];

	memset( lpstr, 0, _MAX_PATH * _MAX_LOADFILECOUNT );		
	CFileDialog	FileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT, NULL, this);

	FileDlg.m_ofn.hwndOwner			= NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "All File(*.*)\0*.*\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Select file";
	FileDlg.m_ofn.lpstrDefExt		= "*.*";
	FileDlg.m_ofn.lpstrFile         = (LPTSTR)lpstr;
	FileDlg.m_ofn.nMaxFile			= sizeof(lpstr);
		
	if( FileDlg.DoModal() == IDOK )
	{		 

		CString	   sFilename[ _MAX_LOADFILECOUNT ]; // select Filename string array;
		POSITION   position    =  FileDlg.GetStartPosition();
		int        BFlistIndex = 0;
										
		while( TRUE )
		{
			if( !position || BFlistIndex >= _MAX_LOADFILECOUNT ) break; // 선택 파일의 끝이면 브레이크
			// 다이알로그에서 파일이름 알아내기
			sFilename[ BFlistIndex ] = FileDlg.GetNextPathName(position);
			BFlistIndex++;
		}

		// Filename array sorting..
		for( int i = 0; i<BFlistIndex; i++ )
		{
			for( int j = 0; j<BFlistIndex-1; j++ )
			{
				if( sFilename[j] > sFilename[j+1] )
				{
					_SWAP( sFilename[j],sFilename[j+1] );
				}
			}
		}

		int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );

		if( findsectionitemindex == -1 )
		{
			AfxMessageBox( "ERROR : Find linked tree item" );
			return;
		}

		CSectionList* selectedsectionlist = NULL;
		
		selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

		if( !selectedsectionlist )
		{
			AfxMessageBox( "ERROR : Find linked item pointer" );
			return;
		}

		CPackFileItem* fileitem;	    
		CString orgfile;	
		
		for( i = 0; i<BFlistIndex; i++ )
		{
			orgfile = sFilename[i];
			if( orgfile.Find( m_PathString, 0 ) == -1 )
			{
				CString messagestr;
				messagestr.Format( "WARNING : 프로젝트 파일 하부폴더에 있는 파일만 삽입할 수 있습니다.\n\tFile : %s\n\tProject folder : %s",
									sFilename[i],
									m_PathString );
				AfxMessageBox( messagestr );				
			}
			else
			{
				fileitem = new CPackFileItem;				
								
				fileitem->setFileInformations( sFilename[i] );

				selectedsectionlist->insertItem( fileitem );
			}
		}

		setItem( findsectionitemindex );
	}	
}


void CPackingView::OnDeletefilelistitem() 
{
	// TODO: Add your command handler code here
	
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();
	int selectedItemLevel = 0;

	selectedItemLevel = getTreeLevel( selectedItem );

	if( selectedItemLevel != 1 ) return;

	if( AfxMessageBox( "Delete selected list items ?", MB_YESNO ) != IDYES ) return;	
					
	int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );

	if( findsectionitemindex == -1 ) return;	

	CSectionList* selectedsectionlist = NULL;
		
	selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

	if( !selectedsectionlist ) return;	
	
	int selectedFilelistindex = -1;

	while( 1 )
	{
		POSITION pos = this->m_FileListCtrl.GetFirstSelectedItemPosition();

		if ( !pos )	break;

		selectedFilelistindex = this->m_FileListCtrl.GetNextSelectedItem(pos);

		if( selectedFilelistindex < 0 ) break;
			
		selectedsectionlist->deleteItem( selectedFilelistindex );
			
		this->m_FileListCtrl.DeleteItem( selectedFilelistindex );


		int totalsize = 0;
		CPackFileItem* fileitem;
		
		for( int i = 0; i < selectedsectionlist->m_itemcount; i++ )
		{
			fileitem = NULL;
			fileitem = selectedsectionlist->getStringItem( i );

			if( fileitem ) totalsize += fileitem->rFileSize;
		}
			
		m_TotalSize.Format( "%d", totalsize );
		m_TotalCount.Format( "%d", selectedsectionlist->m_itemcount );

		selectedItem = this->m_PackingTreeControl.GetSelectedItem();

		m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( this->m_PackingTreeControl.GetParentItem( selectedItem ) );
		m_SelectedSectionname = this->m_PackingTreeControl.GetItemText( selectedItem );
	}

	UpdateData( FALSE );
	
}

void CPackingView::OnDeleteallfilelistitem() 
{
	
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();
	int selectedItemLevel = 0;

	selectedItemLevel = getTreeLevel( selectedItem );

	if( selectedItemLevel != 1 ) return;

	// TODO: Add your command handler code here
	if( AfxMessageBox( "Delete all list item ?", MB_YESNO ) != IDYES ) return;		
			
	int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );

	if( findsectionitemindex == -1 ) return;	

	CSectionList* selectedsectionlist = NULL;
		
	selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

	if( !selectedsectionlist ) return;	
	
	int selectedFilelistindex = -1;
		
	selectedsectionlist->deleteAllItem();
		
	this->m_FileListCtrl.DeleteAllItems();
		
	m_TotalSize.Format( "0" );
	m_TotalCount.Format( "0" );

	selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	m_SelectedPackagename = this->m_PackingTreeControl.GetItemText( this->m_PackingTreeControl.GetParentItem( selectedItem ) );
	m_SelectedSectionname = this->m_PackingTreeControl.GetItemText( selectedItem );

	UpdateData( FALSE );

	
}

void CPackingView::OnMakeallpackage() 
{
	// TODO: Add your control notification handler code here
	FilePacking();	
}

void CPackingView::OnPacking() 
{
	// TODO: Add your control notification handler code here
	FilePacking( FALSE );
}

void CPackingView::OnExtract() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CPackingView::saveStringandSize(CString& string, FILE *fp)
{
	int stringlength = string.GetLength();

	fwrite( &stringlength, sizeof( int ), 1, fp );
	fwrite( string.operator LPCTSTR(), stringlength, 1, fp );	

	return TRUE;
}

BOOL CPackingView::saveStringandSize(LPSTR string, FILE *fp)
{
	if( !string ) return FALSE;

	int stringlength = strlen( string );

	fwrite( &stringlength, sizeof( int ), 1, fp );
	fwrite( string, stringlength, 1, fp );	

	return TRUE;
}

BOOL CPackingView::loadStringandSize(LPSTR string, int maxsize, FILE *fp)
{
	if( !string || !fp ) return FALSE;

	memset( string, 0, maxsize );

	int stringsize = 0;

	fread( &stringsize, sizeof( int ), 1, fp );

	if( maxsize < stringsize )
		fread( string, maxsize, 1, fp );
	else 
		fread( string, stringsize, 1, fp );

	return TRUE;
}

BOOL CPackingView::savePackStructFile( LPCSTR filename )
{
	
	FILE* fp = NULL;

	fp = fopen( filename, "wb" );

	if( !fp )
	{
		AfxMessageBox( "ERROR : File open" );
		return FALSE;
	}
	
	WORD HeaderIndicator = ID_XPACKSTRUCTFILE_HEADER;
	WORD FileVersion    = ID_XPACKSTRUCTFILE_VERSION;

	fwrite( &HeaderIndicator, sizeof( WORD ), 1, fp );

	fwrite( &FileVersion, sizeof( WORD ), 1, fp );

	int packfilecount = getOnelevelSubTreeCount( NULL );
	
	fwrite( &packfilecount, sizeof( int ), 1, fp );

	if( packfilecount <= 0 )
	{
		fclose( fp );
		return TRUE;
	}
	
	int     itemstringlength = 0;
	CString treeitemstring = _T("");
	int     sectioncount = 0;

	HTREEITEM childItem = this->m_PackingTreeControl.GetChildItem( NULL );
	HTREEITEM subsectionItem;
	CSectionList* selectedsectionlist = NULL;
	CPackFileItem* fileitem = NULL;

	for( int i = 0; i < packfilecount; i++ )
	{
		if( childItem != NULL )
		{
			treeitemstring = this->m_PackingTreeControl.GetItemText( childItem );
			
			saveStringandSize( treeitemstring, fp );

			// sub section count save
			sectioncount = getOnelevelSubTreeCount( childItem );
			fwrite( &sectioncount, sizeof( int ), 1, fp );

			// sub section list save
			if( sectioncount > 0 )
			{
				subsectionItem = m_PackingTreeControl.GetChildItem( childItem );

				int findsectionitemindex = -1;

				for( int j = 0; j < sectioncount; j++ )	
				{

					treeitemstring = this->m_PackingTreeControl.GetItemText( subsectionItem );
			
					// section name save
					saveStringandSize( treeitemstring, fp );

					// Find section item
					findsectionitemindex = 	this->m_FilePackage.findLinkedTreeItem( subsectionItem );

					if( findsectionitemindex != -1 )
					{		
						selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

						if( selectedsectionlist )
						{
							// File item count save
							fwrite( &selectedsectionlist->m_itemcount, sizeof( int ), 1, fp );

							// File item string length & string save 

							CString finalfilename;

							for( int count = 0; count < selectedsectionlist->m_itemcount; count++ )
							{
								fileitem = selectedsectionlist->getStringItem( count );
								
								finalfilename = fileitem->rFileInfo.szDisplayName;

								finalfilename.Replace( m_PathString.operator LPCTSTR(), "" );

								saveStringandSize( finalfilename.GetBuffer(256), fp );
							}
						}
						else 
						{
							AfxMessageBox( "ERROR : section list access" );
							fclose( fp );
							return FALSE;							
						}
					}
					else
					{							
						AfxMessageBox( "ERROR : section list find" );
						fclose( fp );
						return FALSE;							
					}

					subsectionItem = this->m_PackingTreeControl.GetNextItem( subsectionItem, TVGN_NEXT );
				
					if( subsectionItem == NULL ) break;
				}
			}
			
			childItem = this->m_PackingTreeControl.GetNextItem( childItem, TVGN_NEXT );
				
			if( childItem == NULL ) break;
		}
	}

	fclose( fp );

	((CXPackerDlg*)((CXPackerApp*)AfxGetApp())->m_pMainWnd)->m_LoadedFilename = filename;
	((CXPackerDlg*)((CXPackerApp*)AfxGetApp())->m_pMainWnd)->UpdateData( FALSE );	

	CString tString = filename;
	int   cutposition = tString.ReverseFind( '\\' );
	char  strbuffer[ _MAX_PATH ];
	memset( strbuffer, 0, _MAX_PATH );
	strncpy( strbuffer, filename, cutposition );

	m_PathString = strbuffer;	

	return TRUE;
}

BOOL CPackingView::loadXPP(LPCSTR filename)
{
	resetTree();

	FILE* fp = NULL;

	fp = fopen( filename, "rb" );

	if( !fp )
	{
		AfxMessageBox( "ERROR : File open" );
		return FALSE;
	}

	// 패키지 구조파일로 부터 현재 루트패스를 구함.
	m_PathString = filename;
	int cutpos = m_PathString.ReverseFind( '\\' );
	char path[_MAX_PATH];
	memset( path, 0, _MAX_PATH );
	memcpy( path, m_PathString.GetBuffer( 256 ), cutpos );
	m_PathString = path;
	

	WORD HeaderIndicator = 0;
	WORD FileVersion     = 0;

	fread( &HeaderIndicator, sizeof( WORD ), 1, fp );

	if( HeaderIndicator != ID_XPACKSTRUCTFILE_HEADER )
	{
		AfxMessageBox( "ERROR : File header check" );
		fclose( fp );
		return FALSE;
	}

	fread( &FileVersion, sizeof( WORD ), 1, fp );

	if( FileVersion != ID_XPACKSTRUCTFILE_VERSION )
	{
		CString tstr;
		tstr.Format( "ERROR : File version check\n[program:%d file:%d]", ID_XPACKSTRUCTFILE_VERSION, FileVersion );
		AfxMessageBox( tstr );
		fclose( fp );
		return FALSE;
	}
	
	char    treeitemstring[_MAX_PATH];
	int     sectioncount = 0;
	int     sectionitemcount = 0;

		
	CSectionList* selectedsectionlist = NULL;
	CPackFileItem* fileitem = NULL;


	int packfilecount = 0;
	
	fread( &packfilecount, sizeof( int ), 1, fp );

	for( int i = 0; i < packfilecount; i ++ )
	{
		// load & insert packing file node

		loadStringandSize( treeitemstring, _MAX_PATH, fp );
		
		HTREEITEM PackageItem = this->insertNode( NULL, treeitemstring );
		
		// sub section count load
		fread( &sectioncount, sizeof( int ), 1, fp );

		// insert sub section 
		for( int scount = 0; scount < sectioncount; scount++ )
		{

			loadStringandSize( treeitemstring, _MAX_PATH, fp );

			CSectionList* newsection;
		
			newsection = new CSectionList;

			if( !newsection )
			{
				AfxMessageBox("ERROR : Create section" );
				fclose( fp );
				return FALSE;
			}
			
			this->m_FilePackage.insertItem( newsection );
						
			HTREEITEM newItem = insertNode( PackageItem, treeitemstring );
			this->m_PackingTreeControl.SelectItem( newItem );
						
			newsection->setLinkedTreeItem( newItem );

			this->m_PackingTreeControl.SelectItem( newItem );
			
			// load section file name string item count
			fread( &sectionitemcount, sizeof( int ), 1, fp );

			CPackFileItem* fileitem = NULL;
			CString pathfilename;
			CString orgfilename;
			
			for( int icount = 0; icount <  sectionitemcount; icount++ )
			{
				loadStringandSize( treeitemstring, _MAX_PATH, fp );
				
				pathfilename = m_PathString;
				//pathfilename += '\\';
				pathfilename += treeitemstring;

				orgfilename = _T("");

				if( m_ConvertTGAToDDS )
				{					
					TCHAR szTempStr[_MAX_PATH];
					memset( szTempStr, 0, _MAX_PATH );
					strcpy( szTempStr, pathfilename.operator LPCTSTR() );

					LPSTR pTempExtString = getExtention( szTempStr );

					if(pTempExtString)
					{
						if( ( (stricmp( pTempExtString, ".tga" ) == 0) ) ||
							( (stricmp( pTempExtString, ".bmp" ) == 0) ) ||
							( ( (stricmp( pTempExtString, ".jpg"  ) == 0 ) || (stricmp( pTempExtString, ".jpeg" ) == 0 ) ) ) ||
							( (stricmp( pTempExtString, ".png" ) == 0) ) ||
							( (stricmp( pTempExtString, ".dib" ) == 0) )  ) 
						{	
							orgfilename = szTempStr;
							
							strcpy( pTempExtString, ".dds" );

							if( CheckExistFile( szTempStr, FALSE ) )
							{
								pathfilename = szTempStr;
							}							
						}
					}					
				}
				
				fileitem = new CPackFileItem( pathfilename.GetBuffer(256) );				

				if( orgfilename.GetLength() > 0 )
				{
					strcpy( fileitem->m_OrgFileName, orgfilename.operator LPCTSTR() );
				}

				newsection->insertItem( fileitem );
			}

			if( this->m_FilePackage.m_sectioncount > 0 )
				checkPackingFilelistValiable( this->m_FilePackage.m_sectioncount-1 );
		}
	}

	fclose( fp );	

	HTREEITEM childItem = this->m_PackingTreeControl.GetChildItem( NULL );

	this->m_PackingTreeControl.SelectItem( childItem );
	
	((CXPackerDlg*)((CXPackerApp*)AfxGetApp())->m_pMainWnd)->m_LoadedFilename = filename;
	((CXPackerDlg*)((CXPackerApp*)AfxGetApp())->m_pMainWnd)->UpdateData( FALSE );	

			
	return TRUE;
}

void CPackingView::OnPacklistload() 
{
	// TODO: Add your command handler code here
	CFileDialog	FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		    = NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "XPS File(*.XPS)\0*.XPS\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Load XPackage structure file";
	FileDlg.m_ofn.lpstrDefExt		= "*.XPS";
		
	if( FileDlg.DoModal() == IDOK )
	{			
		loadXPP( FileDlg.GetPathName().operator LPCTSTR() );
	}		
}

void CPackingView::OnPacklistsave() 
{
	// TODO: Add your command handler code here
	CFileDialog	FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		= NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "XPS File(*.XPS)\0*.XPS\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Save XPackage structure file";
	FileDlg.m_ofn.lpstrDefExt		= "*.XPS";			
		
	if( FileDlg.DoModal() == IDOK )
	{		
		savePackStructFile( FileDlg.GetPathName().operator LPCTSTR() );
	}		
}

void CPackingView::OnResetpackinglist() 
{
	// TODO: Add your command handler code here
	if( AfxMessageBox( "Reset package structure ?", MB_YESNO ) == IDYES )
	{
		resetTree();
	}		
}

void CPackingView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
		
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPackingView::OnKeydownPackingtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = 0;

	selectedItemLevel = getTreeLevel( selectedItem );

	
	if( pTVKeyDown->wVKey == VK_INSERT )
	{
		if( selectedItemLevel < 0 )
		{
			OnInsertmenuitem();
		}
		else if( selectedItemLevel == 0 )
		{
			if( HIBYTE( GetAsyncKeyState( VK_CONTROL ) )  )
			{
				OnInsertmenuitem();
			}
			else OnInsertsectionmenuitem();
		}
		else if( selectedItemLevel == 1 )
		{
			OnInsertfilesmenuitem();		
		}
	}
	else if( pTVKeyDown->wVKey == VK_DELETE )
	{
		if( selectedItemLevel == 0 )
		{
			OnDeletemenuitem();
		}
		else if( selectedItemLevel == 1 )
		{
			OnDeletesectionmenuitem();		
		}
	}
	
	*pResult = 0;
}

void CPackingView::OnKeydownFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if( pLVKeyDown->wVKey == VK_INSERT )
	{
		OnInsertfilesmenuitem();
	}
	else if( pLVKeyDown->wVKey == VK_DELETE )
	{
		OnDeletefilelistitem(); 
	}
	
	*pResult = 0;
}

BOOL CPackingView::checkPackingFilelistValiable(int packageindex)
{
	CSectionList* selectedsectionlist = this->m_FilePackage.getSectionList( packageindex );

	if( !selectedsectionlist )
	{
		AfxMessageBox( "ERROR : Section list access" );
		return FALSE;
	}
	
	CPackFileItem* fileitem;

	HANDLE      hFind;
	WIN32_FIND_DATA rFind;

	BOOL result = TRUE;

	for( int i = 0; i < selectedsectionlist->m_itemcount; i++ )
	{
		fileitem = selectedsectionlist->getStringItem( i );

		hFind = FindFirstFile ( fileitem->rFileInfo.szDisplayName, &rFind );

		if ( INVALID_HANDLE_VALUE != hFind )
		{
			fileitem->rFileSize = rFind.nFileSizeLow;
			FindClose ( hFind );
		}		
		else 
		{
			CString tstr;
			tstr.Format( "ERROR : Invalid file found [%s]", fileitem->rFileInfo.szDisplayName );
			AfxMessageBox( tstr );			
			result = FALSE;			
		}		
	}

	return result;
}



BOOL CPackingView::FilePacking( BOOL Allpackingflag )
{	
	int StartPackageIndex = 0;
	int EndPackageIndex = this->m_FilePackage.m_sectioncount;

	for( int i = StartPackageIndex; i < EndPackageIndex; i++ )
	{
		if( !checkPackingFilelistValiable( i ) )
		{
			return FALSE;
		}
	}
	

	int packagecount = getOnelevelSubTreeCount( NULL );

	if( packagecount < 1 )
	{
		AfxMessageBox( "ERROR : Package list empty" );
		return FALSE;
	}

	CDirDialog dlg(m_PathString,"All Files (*.*)|*.*||", this);
	
    if( dlg.DoModal() != IDOK ) return TRUE;

	UpdateWindow();

	m_PathString = dlg.GetPath();

	// 끝에 '\' 잘라내기 
	int cutpos = m_PathString.ReverseFind( '\\' );
	m_PathString.GetBuffer(256)[cutpos] = '\0';


/*
	CSetRootPathDlg setrootpathdlg;

	char pathstring[ _MAX_PATH+1 ];
	memset( pathstring, 0, _MAX_PATH+1 );

	m_FileListCtrl.GetItemText( 0, 0, pathstring, _MAX_PATH );	
		
	for( i = strlen(pathstring)-1; i > 0; i-- )
	{
		if(  pathstring[i] == '\\' )
		{
			pathstring[i] = NULL;
			break;
		}
		else pathstring[i] = NULL;
	}
	
	setrootpathdlg.m_RootPath = pathstring;

	if( setrootpathdlg.DoModal() == IDCANCEL ) return FALSE;
*/


	FILE* packingfileptr = NULL;	
	CString treeitemstring = _T("");	

	FILE* idheaderfileptr = NULL;
	CString itemidstring = _T("");	

	CString itemsectionstring = _T("");	

	CString totalidstring = _T("");

	
	HTREEITEM childItem = this->m_PackingTreeControl.GetChildItem( NULL );
	HTREEITEM subsectionItem;
	CSectionList* selectedsectionlist = NULL;
	CPackFileItem* fileitem = NULL;

	BOOL checkData = TRUE;

	int findsectionitemindex = -1;

	long packingposition;
	long packingidindex;


	int selectedpackage = -1;

	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = getTreeLevel( selectedItem );
	
	if( selectedItemLevel == 0 )
	{		
		selectedpackage = getSelectItemSequence(selectedItem);
	}
	else if( selectedItemLevel == 1 )
	{	
		selectedItem = this->m_PackingTreeControl.GetParentItem(selectedItem);
		selectedpackage = getSelectItemSequence( selectedItem );
	}



	// Progress bar intialize-------------------------------------------------------------------------------------------
	int totalsectioncount = 0;

	for( i = StartPackageIndex; i < EndPackageIndex; i++ )
	{
		
		selectedsectionlist = this->m_FilePackage.getSectionList( i );
		totalsectioncount += selectedsectionlist->m_itemcount;		
	}	

	this->progressrate = 0;

	progressdlg.SetWindowText( "Make package file..." );
	progressdlg.setData( totalsectioncount, "Now packing" );
	progressdlg.ShowWindow( SW_SHOW );	

	progressdlg.setProgressRate( 0, "Start..." );

	progressdlg.UpdateWindow();		

	CString sIDString = _T("");
	CString ZipFilename = _T("");

	_PACKAGEFILE_HEADER PackFileHeader;

	PackFileHeader.FileVersion     = _HEADERINDICATOR_PACKAGEFILEVERSION;
	PackFileHeader.HeaderIndicator = _HEADERINDICATOR_PACKAGEFILE;

	CZipArchive zip;

	

	// Packing start---------------------------------------------------------------------------------------------------
	for( i = 0; i < packagecount; i++ )
	{
		if( !Allpackingflag )
		{
			if( i != selectedpackage )
			{
				childItem = this->m_PackingTreeControl.GetNextItem( childItem, TVGN_NEXT );				
				if( childItem == NULL ) break;
				
				continue;
			}
		}

		if( childItem != NULL )
		{		
			treeitemstring = this->m_PackingTreeControl.GetItemText( childItem );

			treeitemstring.MakeUpper();
			
			// ID String initialize...
			sIDString.Format( "ID_%s_", treeitemstring );			
			itemidstring = treeitemstring;
			itemidstring  += _T(".H");

			ZipFilename = treeitemstring;
			ZipFilename += _T(".XPZ");

			treeitemstring += _T(".XP");

			_chdir( m_PathString );
			
			packingfileptr = fopen( treeitemstring, "wb" );

			idheaderfileptr = fopen( itemidstring, "wt" );

			CString preprocessorstring;
			CString tstring;

			tstring = itemidstring;			
			tstring.MakeUpper();			
			tstring.Replace( ".", "_" );
			tstring.Remove( ' ' );			
			preprocessorstring.Format( "#ifndef __%s__", tstring );
			fprintf( idheaderfileptr, "%s\n", preprocessorstring.operator LPCTSTR() );
			
			preprocessorstring.Format( "#define __%s__\n", tstring );
			fprintf( idheaderfileptr, "%s\n\n", preprocessorstring.operator LPCTSTR() );

			packingidindex  = 0L;
			packingposition = 0L;
			checkData = TRUE;

			int sectioncount = getOnelevelSubTreeCount( childItem );

			// find packing file count--------------------------------------------------------------------------------- 
			
			progressdlg.setProgressRate( this->progressrate, "Count file..." );

			int packingfilecount = 0;

			if( sectioncount > 0 )
			{
				subsectionItem = m_PackingTreeControl.GetChildItem( childItem );

				// File position header save
				for( int j = 0; j < sectioncount; j++ )
				{
					// Find section

					findsectionitemindex = 	this->m_FilePackage.findLinkedTreeItem( subsectionItem );

					if( findsectionitemindex != -1 )
					{		
						// found section
						selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

						if( selectedsectionlist )
						{		
							packingfilecount += selectedsectionlist->m_itemcount;							
						}
					}
					// Next section....
					subsectionItem = this->m_PackingTreeControl.GetNextItem( subsectionItem, TVGN_NEXT );
				
					if( subsectionItem == NULL ) break;
				}				
			}

			// save packing file count 
			PackFileHeader.PackedFileCount = packingfilecount;
			fwrite( &PackFileHeader, sizeof( _PACKAGEFILE_HEADER ), 1, packingfileptr );

			progressdlg.setProgressRate( this->progressrate, "Header save..." );

			// Save file name string    -------------------------------------------------------------------------------

			DWORD filenameinfosize = 0L;

			if( sectioncount > 0 )
			{
				subsectionItem = m_PackingTreeControl.GetChildItem( childItem );

				char	szTempFilenameString[_MAX_PATH];
				WORD	filestringsize = 0;
				CString tString;				
				
				for( int j = 0; j < sectioncount; j++ )
				{
					// Find section

					findsectionitemindex = 	this->m_FilePackage.findLinkedTreeItem( subsectionItem );

					if( findsectionitemindex != -1 )
					{		
						// found section
						selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

						if( selectedsectionlist )
						{								
							for( int count = 0; count < selectedsectionlist->m_itemcount; count++ )
							{									
								fileitem = selectedsectionlist->getStringItem( count );
								
								memset( szTempFilenameString, 0, _MAX_PATH );

								if( strlen( fileitem->m_OrgFileName ) > 0 )
								{
									tString = fileitem->m_OrgFileName;
									int cutposition = tString.ReverseFind( '\\' );
									tString = (fileitem->m_OrgFileName + cutposition + 1);
								}
								else
								{
									tString = fileitem->rFileInfo.szDisplayName;
									int cutposition = tString.ReverseFind( '\\' );
									tString = (fileitem->rFileInfo.szDisplayName + cutposition + 1);
								}

								strcpy( szTempFilenameString, tString.GetBuffer(256) );

								_EncodeData( szTempFilenameString, _ENCODEKEY );

								filestringsize = strlen(szTempFilenameString);
								
								fwrite( &filestringsize, sizeof(WORD), 1, packingfileptr );
								fwrite( &szTempFilenameString, filestringsize, 1, packingfileptr );

								filenameinfosize += sizeof( WORD );
								filenameinfosize += filestringsize;
							}
						}
					}
					// Next section....
					subsectionItem = this->m_PackingTreeControl.GetNextItem( subsectionItem, TVGN_NEXT );
				
					if( subsectionItem == NULL ) break;
				}				
			}		


			// Save File position infor...-----------------------------------------------------------------------------
						
			packingposition = sizeof(_PACKAGEFILE_HEADER) +  filenameinfosize + (packingfilecount * sizeof(int));

			if( sectioncount > 0 )
			{
				subsectionItem = m_PackingTreeControl.GetChildItem( childItem );
				
				for( int j = 0; j < sectioncount; j++ )
				{
					// Find section

					findsectionitemindex = 	this->m_FilePackage.findLinkedTreeItem( subsectionItem );

					if( findsectionitemindex != -1 )
					{		
						// found section
						selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

						if( selectedsectionlist )
						{								
							for( int count = 0; count < selectedsectionlist->m_itemcount; count++ )
							{	
								fwrite( &packingposition, sizeof( int ), 1, packingfileptr );
								
								fileitem = selectedsectionlist->getStringItem( count );

								packingposition += fileitem->rFileSize;
							}
						}
					}
					// Next section....
					subsectionItem = this->m_PackingTreeControl.GetNextItem( subsectionItem, TVGN_NEXT );
				
					if( subsectionItem == NULL ) break;
				}				
			}

			// File packing -------------------------------------------------------------------------------------------
			if( sectioncount > 0 )
			{
				subsectionItem = m_PackingTreeControl.GetChildItem( childItem );

				CString finalidstring;

				for( int j = 0; j < sectioncount && checkData; j++ )
				{
					itemsectionstring = m_PackingTreeControl.GetItemText( subsectionItem );
					itemsectionstring += '_';

					totalidstring = sIDString;
					totalidstring += itemsectionstring;
					// Find section

					findsectionitemindex = 	this->m_FilePackage.findLinkedTreeItem( subsectionItem );

					if( findsectionitemindex != -1 )
					{		
						// found section
						selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

						fprintf( idheaderfileptr, "// SECTION START : %s\n", m_PackingTreeControl.GetItemText( subsectionItem ) );

						if( selectedsectionlist )
						{
							FILE* sourcefilePtr;

							for( int count = 0; count < selectedsectionlist->m_itemcount; count++ )
							{								
								fileitem = selectedsectionlist->getStringItem( count );		

								// File packing........................................................................

								progressdlg.setProgressRate( this->progressrate, fileitem->rFileInfo.szDisplayName );
								
								sourcefilePtr = NULL;

								sourcefilePtr = fopen( fileitem->rFileInfo.szDisplayName, "rb" );

								if( !sourcefilePtr )
								{
									CString tmessagestr;
									tmessagestr.Format( "ERROR : Source file open [%s]", fileitem->rFileInfo.szDisplayName );
									AfxMessageBox( tmessagestr );
									checkData = FALSE;
									break;
								}
								
								long filesize = fileitem->rFileSize;
								int  databyte;

								while( filesize-- )
								{			
									databyte = fgetc( sourcefilePtr );

									if( EOF != databyte ) fputc( databyte, packingfileptr );
									else 
									{
										checkData = FALSE;
										break;
									}
								}
								
								fclose( sourcefilePtr );

								finalidstring = totalidstring;

								CString tString = fileitem->rFileInfo.szDisplayName;
								int cutposition = tString.ReverseFind( '\\' );

								finalidstring += (fileitem->rFileInfo.szDisplayName + cutposition + 1);
								finalidstring.MakeUpper();
								finalidstring.Replace( ".", "_" );
								finalidstring.Remove( ' ' );


								fprintf( idheaderfileptr, "#define %-50s %d\n", finalidstring, packingidindex++ );

								this->progressrate++;
							}

						}

						fprintf( idheaderfileptr, "// End of [%s] section \n\n", m_PackingTreeControl.GetItemText( subsectionItem ) );
					}
					else
					{
						AfxMessageBox( "ERROR : Find section item" );
						checkData = FALSE;
					}

					// Next section....
					subsectionItem = this->m_PackingTreeControl.GetNextItem( subsectionItem, TVGN_NEXT );
				
					if( subsectionItem == NULL ) break;
				}
			}
			
			fclose( packingfileptr ); 

			preprocessorstring.Format( "\n\n#endif //#ifndef _%s_", tstring );
			fprintf( idheaderfileptr, "%s\n", preprocessorstring.operator LPCTSTR() );
			fclose( idheaderfileptr );


			
			// Make patch information file...						
			
			CString InfoFilename = treeitemstring;
			InfoFilename += _T("I"); // *.XPI...

			FILE* pPatchListFile = NULL;
			pPatchListFile = fopen( InfoFilename.operator LPCTSTR(), "wt" );

			if( !pPatchListFile )
			{
				AfxMessageBox( "ERROR : Patch information file open" );
				progressdlg.ShowWindow( SW_HIDE );
				return FALSE;
			}

			fputs( "; Patch information file\n\n", pPatchListFile );
			

			if( sectioncount > 0 )
			{
				subsectionItem = this->m_PackingTreeControl.GetChildItem( childItem );				

				for( int treeindex = 0; treeindex < sectioncount; treeindex++ )
				{
					int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( subsectionItem );
					if( findsectionitemindex == -1 )
					{
						AfxMessageBox( "ERROR : Access pacakge list" );
						fclose( pPatchListFile );
						progressdlg.ShowWindow( SW_HIDE );
						return FALSE;
					}


					CSectionList* selectedsectionlist = NULL;
						
					selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

					if( !selectedsectionlist )
					{
						fclose( pPatchListFile );
						progressdlg.ShowWindow( SW_HIDE );
						return FALSE;
					}

					CPackFileItem*  tfileitem = NULL;
					int				totalsize = 0;	
					CString         patchstring;
					CString			rootpath = m_PathString;
					rootpath.MakeUpper();

					for( int k = 0; k < selectedsectionlist->m_itemcount; k++ )
					{
						tfileitem = NULL;
						tfileitem = selectedsectionlist->getStringItem( k );

						if( !tfileitem )
						{
							AfxMessageBox( "ERROR : access file item" );			
							break;
						}

						patchstring = tfileitem->rFileInfo.szDisplayName;
						
						patchstring.MakeUpper();
												
						patchstring.Replace( rootpath, "");

						fprintf( pPatchListFile, "%s\n", patchstring );
						
					}

					subsectionItem = this->m_PackingTreeControl.GetNextItem( subsectionItem, TVGN_NEXT );
					
					if( subsectionItem == NULL ) break;
				}
			}

			fputs( "\n; End of patch information file\n\n", pPatchListFile );
			
			fclose( pPatchListFile );

			if( m_Zipping )
			{
				progressdlg.setProgressRate( this->progressrate, "Compress..." );
				
				zip.Open( ZipFilename.operator LPCTSTR() , CZipArchive::create );

				zip.AddNewFile( treeitemstring );
				zip.AddNewFile( InfoFilename );
				
				zip.Close();
			}

			childItem = this->m_PackingTreeControl.GetNextItem( childItem, TVGN_NEXT );
				
			if( childItem == NULL ) break;
		}
	}

	progressdlg.ShowWindow( SW_HIDE );

	return checkData;
}

void CPackingView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnDropFiles(hDropInfo);
}

void CPackingView::OnSortfilelist() 
{
	// TODO: Add your control notification handler code here

	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();

	int selectedItemLevel = getTreeLevel( selectedItem );
	
	if( selectedItemLevel != 1 ) return;
	
	int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );

	if( findsectionitemindex == -1 ) return;



	CSectionList* selectedsectionlist = NULL;
		
	selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

	if( !selectedsectionlist ) return;

	m_FileListCtrl.DeleteAllItems();

	CPackFileItem*  tfileitem = NULL;	
	
	
	for( int i = 0; i < selectedsectionlist->m_itemcount; i++ )
	{
		selectedsectionlist->m_itemlist->resetList();

		for( int j = 0; j < selectedsectionlist->m_itemcount-1; j++ )
		{
			tfileitem = NULL;
			tfileitem = (CPackFileItem*)selectedsectionlist->m_itemlist->currentItem();

			if( !tfileitem || !selectedsectionlist->m_itemlist->currentItem()->getright() )
			{
				AfxMessageBox( "ERROR : access file item" );			
				break;
			}

			if( _stricmp( tfileitem->rFileInfo.szDisplayName, 
				((CPackFileItem*)(selectedsectionlist->m_itemlist->currentItem()->getright()))->rFileInfo.szDisplayName ) > 0 )
			{				
				selectedsectionlist->m_itemlist->currentItemSwap();
			}
			else
			{
				selectedsectionlist->m_itemlist->nextItem();
			}
		}       
		
	}		

	setCurrentFileItem();
}

void CPackingView::OnMakepatchlist() 
{	
	// TODO: Add your command handler code here

	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();
	int selectedItemLevel = getTreeLevel( selectedItem );	
	if( selectedItemLevel != 1 ) return;
	
	int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );
	if( findsectionitemindex == -1 ) return;

	CSetRootPathDlg setrootpathdlg;

	char pathstring[ _MAX_PATH+1 ];
	memset( pathstring, 0, _MAX_PATH+1 );

	m_FileListCtrl.GetItemText( 0, 0, pathstring, _MAX_PATH );	
		
	for( int i = strlen(pathstring)-1; i > 0; i-- )
	{
		if(  pathstring[i] == '\\' )
		{
			pathstring[i] = NULL;
			break;
		}
		else pathstring[i] = NULL;
	}
	
	setrootpathdlg.m_RootPath = pathstring;

	if( setrootpathdlg.DoModal() == IDCANCEL ) return;


	CFileDialog	FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		= NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "XPI File(*.XPI)\0*.XPI\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Save Package patch information file";
	FileDlg.m_ofn.lpstrDefExt		= "*.XPI";			
		
	if( FileDlg.DoModal() == IDCANCEL ) return;	
		
	FILE* pPatchListFile = NULL;
	pPatchListFile = fopen( FileDlg.GetPathName().operator LPCTSTR(), "wt" );

	if( !pPatchListFile )
	{
		AfxMessageBox( "ERROR : File open" );
		return;
	}

	fputs( "; WORLD WAR II Patch information file\n\n", pPatchListFile );

	CSectionList* selectedsectionlist = NULL;
		
	selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

	if( !selectedsectionlist ) return;	

	CPackFileItem*  tfileitem = NULL;
	int				totalsize = 0;	
	CString         patchstring;

	for( i = 0; i < selectedsectionlist->m_itemcount; i++ )
	{
		tfileitem = NULL;
		tfileitem = selectedsectionlist->getStringItem( i );

		if( !tfileitem )
		{
			AfxMessageBox( "ERROR : access file item" );			
			break;
		}

		patchstring = tfileitem->rFileInfo.szDisplayName;
		
		patchstring.MakeUpper();
		setrootpathdlg.m_RootPath.MakeUpper();

		patchstring.Replace( setrootpathdlg.m_RootPath, "");

		fprintf( pPatchListFile, "%s\n", patchstring );
		
	}

	fputs( "\n; End of patch information file\n\n", pPatchListFile );
	
	fclose( pPatchListFile );
		
}



void CPackingView::OnMakewavepackage() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM selectedItem = this->m_PackingTreeControl.GetSelectedItem();
	int selectedItemLevel = getTreeLevel( selectedItem );	
	if( selectedItemLevel != 1 ) return;
	
	int findsectionitemindex = this->m_FilePackage.findLinkedTreeItem( selectedItem );
	if( findsectionitemindex == -1 ) return;

	if( AfxMessageBox( "Sort file list ?", MB_YESNO ) == IDYES ) OnSortfilelist();

	CFileDialog	FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		= NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "DAT File(*.Dat)\0*.Dat\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Save Wave package file";
	FileDlg.m_ofn.lpstrDefExt		= "*.DAT";
		
	if( FileDlg.DoModal() == IDCANCEL ) return;	

	UpdateWindow();
		
	FILE* pWPackFile = NULL;
	pWPackFile = fopen( FileDlg.GetPathName().operator LPCTSTR(), "wb" );

	if( !pWPackFile )
	{
		AfxMessageBox( "ERROR : File open" );
		return;
	}

	CSectionList* selectedsectionlist = NULL;
		
	selectedsectionlist = this->m_FilePackage.getSectionList( findsectionitemindex );

	if( !selectedsectionlist ) return;

	char  tBuffer[16];	
	int   cutposition = 0;
	DWORD fileptrcounter;
	CString finalidstring = _T("");	
	CPackFileItem*  tfileitem = NULL;	
	CString         Filestring;



	memset( tBuffer, 0, 16 );
	

	strcpy( tBuffer, _WP_INDICATOR );
	fwrite( tBuffer, sizeof(char) * 8, 1, pWPackFile );
	fwrite( &(selectedsectionlist->m_itemcount), sizeof(int), 1, pWPackFile );

	_WAVEPACKED_INFOHEADER wp_infoheader;

	fileptrcounter = 8 + 4 + sizeof( _WAVEPACKED_INFOHEADER ) * selectedsectionlist->m_itemcount;


	this->progressrate = 0;
	progressdlg.SetWindowText( "Make package file..." );
	progressdlg.setData( selectedsectionlist->m_itemcount, "Now packing" );
	progressdlg.ShowWindow( SW_SHOW );	
	progressdlg.setProgressRate( 0, "Start..." );
	progressdlg.UpdateWindow();		

	progressdlg.setProgressRate( this->progressrate, "Save informations header..," );
	
	for( int i = 0; i < selectedsectionlist->m_itemcount; i++ )
	{
		tfileitem = NULL;
		tfileitem = selectedsectionlist->getStringItem( i );

		if( !tfileitem )
		{
			AfxMessageBox( "ERROR : access file item" );
			break;
		}

		Filestring = tfileitem->rFileInfo.szDisplayName;		
		cutposition = Filestring.ReverseFind( '\\' );

		finalidstring = (tfileitem->rFileInfo.szDisplayName + cutposition + 1);
		finalidstring.MakeUpper();		

		memset( &wp_infoheader, 0, sizeof(_WAVEPACKED_INFOHEADER) );

		strncpy( wp_infoheader.filename, finalidstring.GetBuffer(256), 16 );

		wp_infoheader.filesize = tfileitem->rFileSize;
		wp_infoheader.packedoffset = fileptrcounter;

		fileptrcounter += tfileitem->rFileSize;

		fwrite( &wp_infoheader, sizeof( _WAVEPACKED_INFOHEADER ), 1, pWPackFile );
	}

	FILE* wavefile = NULL;
	
	for( i = 0; i < selectedsectionlist->m_itemcount; i++ )
	{
		tfileitem = NULL;
		tfileitem = selectedsectionlist->getStringItem( i );

		if( !tfileitem )
		{
			AfxMessageBox( "ERROR : access file item" );
			break;
		}

		wavefile = fopen( tfileitem->rFileInfo.szDisplayName, "rb" );

		if( !wavefile ) 
		{
			CString tMessageString;
			tMessageString.Format( "ERROR : %s file open", tfileitem->rFileInfo.szDisplayName );
			AfxMessageBox( tMessageString );			
			break;
		}
		
		progressdlg.setProgressRate( this->progressrate++, tfileitem->rFileInfo.szDisplayName );

		long filesize = tfileitem->rFileSize;
		int  databyte;

		while( filesize-- )
		{			
			databyte = fgetc( wavefile );

			if( EOF != databyte ) fputc( databyte, pWPackFile );
			else 
			{				
				break;
			}
		}

		fclose( wavefile );
	}
	
	progressdlg.ShowWindow( SW_HIDE );
	fclose( pWPackFile );
	
}

void CPackingView::OnZippingcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();	
}

void CPackingView::OnConverttgatodds() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
