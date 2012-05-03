// WavePackageView.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "WavePackageView.h"

#include "DirDialog.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>

#include "Shlwapi.h"

#include "EditSoundListItemDlg.h"
#include "ModifySoundSceneItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


char g_SoundClass[8][11] = { "Sfx", "Comment", "Crowd", "Background", "Menu", "Master", "Cda", };

#define SND_NORMAL			0x00000000 // Normal Sound
#define SND_3D				0x00000001 // 3D Sound
#define SND_USECPURAM		0x00000002 // Not use snd card memory
#define SND_STREAMING		0x00000004 // Stream the sample from file 
#define SND_LOADRAM			0x00000008 // the sample is loaded also in RAM //??

#define SND_INCONESET		0x00000001
#define SND_OUTCONESET		0x00000002
#define SND_MINDSET			0x00000004
#define SND_MAXDSET			0x00000008
#define SND_OUTVOLSET		0x00000010
#define SND_ALLSET			0x0000001F

/////////////////////////////////////////////////////////////////////////////
// CWavePackageView dialog

int getFileImageListIndex( LPCTSTR filename )
{	 
	SHFILEINFO  rFileInfo;
	SHGetFileInfo ( filename, 0, &rFileInfo, sizeof(rFileInfo), SHGFI_SYSICONINDEX );		

	return rFileInfo.iIcon;
}

CWavePackageView::CWavePackageView(CWnd* pParent /*=NULL*/)
	: CDialog(CWavePackageView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWavePackageView)
	m_Count = _T("");
	m_TotalSize = _T("");
	m_WavePackageFilename = _T("");
	m_SoundListFileString = _T("");
	m_SoundListCount = _T("");	
	m_PackageFilename = _T("");
	m_PackagePath = _T("");
	m_SceneCount = 0;
	m_SceneNameString = _T("");
	m_AutoLoadWavePack = TRUE;
	//}}AFX_DATA_INIT

	m_PathString = _T("");

	m_psndInfo = NULL;	
	m_SelectedSoundTableItem = -1;
}


void CWavePackageView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWavePackageView)	
	DDX_Control(pDX, IDC_SOUNDSCENEFILELIST, m_SoundSceneFileListCtrl);
	DDX_Control(pDX, IDC_FILELIST, m_SoundListControl);
	DDX_Control(pDX, IDC_PACKEDFILELIST, m_WaveFileListCtrl);
	DDX_Text(pDX, IDC_TOTALCOUNTSTATIC, m_Count);
	DDX_Text(pDX, IDC_TOTALSIZESTATIC, m_TotalSize);
	DDX_Text(pDX, IDC_PACKAGENAMEEDIT, m_WavePackageFilename);
	DDX_Text(pDX, IDC_PACKAGENAMEEDIT2, m_SoundListFileString);
	DDX_Text(pDX, IDC_SOUNDLISTCOUNTSTATIC, m_SoundListCount);
	DDX_Text(pDX, IDC_PACKAGEFILENAMEEDIT, m_PackageFilename);
	DDX_Text(pDX, IDC_PATHEDIT, m_PackagePath);
	DDX_Text(pDX, IDC_COUNTEDIT, m_SceneCount);
	DDX_Text(pDX, IDC_SCENENAMEEDIT, m_SceneNameString);
	DDX_Check(pDX, IDC_AUTOLOADWAVEPACKCHECK, m_AutoLoadWavePack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWavePackageView, CDialog)
	//{{AFX_MSG_MAP(CWavePackageView)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PACKEDFILELIST, OnItemchangedPackedfilelist)
	ON_BN_CLICKED(ID_LOADWAVPACKFILE, OnLoadwavpackfile)
	ON_BN_CLICKED(ID_UNPACKALLWAVEFILE, OnUnpackallwavefile)
	ON_BN_CLICKED(ID_UNPACKWAVEFILE, OnUnpackwavefile)
	ON_BN_CLICKED(ID_LOADWAVLISTFILE, OnLoadwavlistfile)
	ON_BN_CLICKED(ID_SAVEWAVLISTFILE, OnSavewavlistfile)
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, OnDblclkFilelist)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDM_INSERTSOUNDLISTITEM, OnInsertsoundlistitem)
	ON_BN_CLICKED(IDM_DELETESOUNDLISTITEM, OnDeletesoundlistitem)
	ON_NOTIFY(LVN_KEYDOWN, IDC_FILELIST, OnKeydownFilelist)
	ON_COMMAND(IDM_OPENSOUNDSCENEFILE, OnOpensoundscenefile)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, OnItemchangedFilelist)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_LOADLINKEDWAVPACKFILE, OnLoadlinkedwavpackfile)
	ON_BN_CLICKED(IDC_AUTOLOADWAVEPACKCHECK, OnAutoloadwavepackcheck)
	ON_NOTIFY(NM_DBLCLK, IDC_SOUNDSCENEFILELIST, OnDblclkSoundscenefilelist)
	ON_BN_CLICKED(IDM_INSERTSOUNDSCENEITEM, OnInsertsoundsceneitem)
	ON_BN_CLICKED(IDM_DELETESOUNDSCENEITEM, OnDeletesoundsceneitem)
	ON_BN_CLICKED(ID_SAVESCENEFILE, OnSavescenefile)
	ON_NOTIFY(NM_DBLCLK, IDC_PACKEDFILELIST, OnDblclkPackedfilelist)
	ON_EN_CHANGE(IDC_PACKAGEFILENAMEEDIT, OnChangePackagefilenameedit)
	ON_EN_CHANGE(IDC_PATHEDIT, OnChangePathedit)
	ON_BN_CLICKED(ID_CHANGEWORKFOLDER, OnSetworkforder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWavePackageView message handlers

void CWavePackageView::OnItemchangedPackedfilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CWavePackageView::OnLoadwavpackfile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog	FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		    = NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "Packed wave file(*.dat)\0*.dat\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Load Wave package file";
	FileDlg.m_ofn.lpstrDefExt		= "*.dat";
		
	if( FileDlg.DoModal() == IDOK )
	{			
		if( loadWavePackage(FileDlg.GetPathName().operator LPCTSTR()) )
		{
			m_WavePackageFilename = FileDlg.GetPathName();
			UpdateData( FALSE );
		}
	}		
}

void CWavePackageView::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_WaveFile.Stop();

	this->m_WavePackage.Close();
	
	if ( NULL != m_imglist.GetSafeHandle() ) m_imglist.Detach();

	this->m_SoundListMenu.DestroyMenu();

	if( m_psndInfo )
	{
		delete[] m_psndInfo;
	}	
}


BOOL CWavePackageView::loadWavePackage( LPCSTR filename )
{
	if( !this->m_WavePackage.Open( filename ) ) return FALSE;
	
	this->m_WaveFileListCtrl.DeleteAllItems();

	int				totalsize = 0;	
	TCHAR			szFileLen [64];
	LVITEM			rItem;
	CString         strIndexString;

	_WAVEPACKED_INFOHEADER* packedfileinfoheader = NULL;

	for( int i = 0; i < this->m_WavePackage.GetPackedFileCount(); i++ )
	{		
		packedfileinfoheader = this->m_WavePackage.GetPackedFileInfo( i );

		if( !packedfileinfoheader )
		{
			AfxMessageBox( "ERROR : access packed file" );
			m_WaveFileListCtrl.DeleteAllItems();
			return FALSE;
		}

		ZeroMemory ( &rItem, sizeof(LVITEM) );

		strIndexString.Format( "%d", i+1 );

        rItem.mask    = LVIF_TEXT;
        rItem.iItem   = i;
        rItem.pszText = strIndexString.GetBuffer(256);

		this->m_WaveFileListCtrl.InsertItem( &rItem ); 

		this->m_WaveFileListCtrl.SetItemText ( i, 1, packedfileinfoheader->filename );

		this->m_WaveFileListCtrl.SetItemText ( i, 2, "Wave" );

		StrFormatByteSize ( packedfileinfoheader->filesize, szFileLen, 64 ); 
		this->m_WaveFileListCtrl.SetItemText( i, 3, szFileLen );		

		totalsize += packedfileinfoheader->filesize;
	}
	
	m_Count.Format( "%d", this->m_WavePackage.GetPackedFileCount() );
	StrFormatByteSize ( totalsize, szFileLen, 64 ); 
	m_TotalSize = szFileLen;	

	return TRUE;
}

BOOL CWavePackageView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    RECT rect;

	m_SoundSceneFileListCtrl.GetClientRect( &rect );
	m_SoundSceneFileListCtrl.SetExtendedStyle ( LVS_EX_FULLROWSELECT );
	
	m_SoundSceneFileListCtrl.InsertColumn( 0, "Idx", LVCFMT_LEFT, 40 );
	m_SoundSceneFileListCtrl.InsertColumn( 1, "Load ID", LVCFMT_LEFT, ((rect.right-rect.left-60)/4) );
	m_SoundSceneFileListCtrl.InsertColumn( 2, "Class", LVCFMT_LEFT, ((rect.right-rect.left-60)/4)+20 );	
	m_SoundSceneFileListCtrl.InsertColumn( 3, "Type", LVCFMT_LEFT, ((rect.right-rect.left-60)/4) );	
	m_SoundSceneFileListCtrl.InsertColumn( 4, "Inst..Required", LVCFMT_LEFT, ((rect.right-rect.left-60)/4) );


	m_WaveFileListCtrl.GetClientRect( &rect );

	m_WaveFileListCtrl.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_TRACKSELECT | LVS_EX_MULTIWORKAREAS );

	m_WaveFileListCtrl.InsertColumn( 0, "Idx", LVCFMT_LEFT, 30 );
	m_WaveFileListCtrl.InsertColumn( 1, "Filename", LVCFMT_LEFT, rect.right-rect.left - 164 );
	m_WaveFileListCtrl.InsertColumn( 2, "Type", LVCFMT_LEFT, 45 );	
	m_WaveFileListCtrl.InsertColumn( 3, "Size", LVCFMT_RIGHT, 70 );		

	m_SoundListControl.GetClientRect( &rect );
	m_SoundListControl.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_TRACKSELECT | LVS_EX_MULTIWORKAREAS );

	m_SoundListControl.InsertColumn( 0, "Idx", LVCFMT_LEFT, 30 );
	m_SoundListControl.InsertColumn( 1, "Sound name", LVCFMT_LEFT, ((rect.right-rect.left-30)/3) );
	m_SoundListControl.InsertColumn( 2, "Environment", LVCFMT_LEFT, ((rect.right-rect.left-30)/3) );	
	m_SoundListControl.InsertColumn( 3, "Environment link", LVCFMT_LEFT, ((rect.right-rect.left-30)/3) );		

	m_SoundListMenu.CreatePopupMenu();

	m_SoundListMenu.AppendMenu( MF_STRING, IDM_INSERTSOUNDLISTITEM, "&Insert sound list item\tInsert" );
	m_SoundListMenu.AppendMenu( MF_STRING, IDM_DELETESOUNDLISTITEM, "&Delete sound list item\tDelete" );
	m_SoundListMenu.AppendMenu( MF_STRING, ID_LOADWAVLISTFILE, "&Open sound scene list file" );
	m_SoundListMenu.AppendMenu( MF_STRING, ID_SAVEWAVLISTFILE, "&Save sound scene list file" );
		
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

extern int  g_AllOverwrite;
extern BOOL g_OverwriteQuestion;
extern BOOL g_AllOverwriteQuestion;

void CWavePackageView::OnUnpackallwavefile() 
{
	// TODO: Add your control notification handler code here

	if( this->m_WavePackage.GetPackedFileCount() <= 0 ) 
	{
		AfxMessageBox( "Warning : Empty package !" );
		return;
	}

	CDirDialog dlg(m_PathString,"All Files (*.*)|*.*||", this);
	
    if( dlg.DoModal() != IDOK ) return;

	UpdateWindow();

	m_PathString = dlg.GetPath();
		
	CProgressDlg progressdlg;
	int          progressrate = 0;

	progressdlg.Create( this );

	progressdlg.SetWindowText( "Extract packed wave file..." );
	progressdlg.setData( this->m_WavePackage.GetPackedFileCount(), "Extracting..." );
	progressdlg.ShowWindow( SW_SHOW );	

	progressdlg.setProgressRate( 0, "Start..." );

	_WAVEPACKED_INFOHEADER* packedfileinfoheader = NULL;

	g_AllOverwrite = 0;
	g_OverwriteQuestion = TRUE;
	g_AllOverwriteQuestion = TRUE;

	for( int i=0; i < this->m_WavePackage.GetPackedFileCount(); i++ )
	{			
		packedfileinfoheader = this->m_WavePackage.GetPackedFileInfo(i);

		if( !packedfileinfoheader ) 
		{
			AfxMessageBox( "Warning : Access packed informations !" );
			break;
		}
		
		progressdlg.setProgressRate( progressrate++, packedfileinfoheader->filename );

		if( !this->m_WavePackage.ExportPackedFile( i, m_PathString.operator LPCTSTR() ) )
		{
			break;
		}
	}
	
	progressdlg.ShowWindow( SW_HIDE );

	progressdlg.DestroyWindow();
	
}

void CWavePackageView::OnUnpackwavefile() 
{
	// TODO: Add your control notification handler code here
	if( this->m_WavePackage.GetPackedFileCount() <= 0 ) 
	{
		AfxMessageBox( "Warning : Empty package !" );
		return;
	}
			
	int selectedFilelistindex = -1;
	int selectedFileCount = 0;

	POSITION pos = this->m_WaveFileListCtrl.GetFirstSelectedItemPosition();

	if ( !pos )
	{
		AfxMessageBox( "Warning : Select file item !" );
		return;
	}

	do
	{
		selectedFilelistindex = this->m_WaveFileListCtrl.GetNextSelectedItem(pos);

		if( selectedFilelistindex < 0 ) break;					

		selectedFileCount++;

	}while( 1 );

	if( selectedFileCount == 0 )
	{
		AfxMessageBox( "Warning : Select file item !" );
		return;
	}	
	

	CDirDialog dlg(m_PathString,"All Files (*.*)|*.*||", this);
	
    if( dlg.DoModal() != IDOK ) return;

	UpdateWindow();

	m_PathString = dlg.GetPath();


	pos = this->m_WaveFileListCtrl.GetFirstSelectedItemPosition();

	if ( !pos )	return;
	
	CProgressDlg progressdlg;
	int          progressrate = 0;

	progressdlg.Create( this );

	progressdlg.SetWindowText( "Extract packed wave file..." );
	progressdlg.setData( selectedFileCount, "Extracting..." );
	progressdlg.ShowWindow( SW_SHOW );	

	progressdlg.setProgressRate( 0, "Start..." );

	_WAVEPACKED_INFOHEADER* packedfileinfoheader = NULL;

	g_AllOverwrite = 0;
	g_OverwriteQuestion = TRUE;
	g_AllOverwriteQuestion = FALSE;

	do
	{
		selectedFilelistindex = this->m_WaveFileListCtrl.GetNextSelectedItem(pos);

		if( selectedFilelistindex < 0 ) break;			

		packedfileinfoheader = this->m_WavePackage.GetPackedFileInfo(selectedFilelistindex);

		if( !packedfileinfoheader ) 
		{
			AfxMessageBox( "Warning : Access packed informations !" );
			break;
		}
		
		progressdlg.setProgressRate( progressrate++, packedfileinfoheader->filename );

		if( !this->m_WavePackage.ExportPackedFile( selectedFilelistindex, m_PathString.operator LPCTSTR() ) )
		{
			break;
		}

	}while( 1 );
	
	progressdlg.ShowWindow( SW_HIDE );

	progressdlg.DestroyWindow();
}

BOOL CWavePackageView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) 
	{	
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CWavePackageView::OnLoadwavlistfile() 
{
	// TODO: Add your control notification handler code here
	
	CFileDialog	FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		    = NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "Sound list file(*.slt)\0*.slt\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Load sound list file";
	FileDlg.m_ofn.lpstrDefExt		= "*.slt";

	if( FileDlg.DoModal() == IDOK )
	{			
		if( loadSoundListFile( FileDlg.GetPathName() ) )
		{
			m_SoundListFileString = FileDlg.GetPathName();
			UpdateData( FALSE );
		}
	}			
}

BOOL CWavePackageView::loadSoundListFile(LPCSTR filename)
{		
	if( !m_sndtablelist.loadSndList( filename ) ) return FALSE;
		
	LVITEM			rItem;
	CString         strIndexString;
	LPSNDSCENE      psndscene = NULL;

	this->m_SoundListControl.DeleteAllItems();
	
	for( int i = 0; (DWORD)i < m_sndtablelist.getItemCount(); i++ )
	{	
		psndscene = m_sndtablelist.getSndScene( i );

		if( !psndscene )
		{
			AfxMessageBox( "ERROR : Access sound table list item" );
			break;
		}

		ZeroMemory ( &rItem, sizeof(LVITEM) );

		strIndexString.Format( "%d", i+1 );

        rItem.mask    = LVIF_TEXT;
        rItem.iItem   = i;
        rItem.pszText = strIndexString.GetBuffer(256);

		this->m_SoundListControl.InsertItem( &rItem ); 

		this->m_SoundListControl.SetItemText ( i, 1, psndscene->li.name );

		this->m_SoundListControl.SetItemText ( i, 2, psndscene->li.env );
		
		this->m_SoundListControl.SetItemText( i, 3,  psndscene->li.envLink );
	}

	m_SoundListCount.Format( "%d", m_sndtablelist.getItemCount() );

	UpdateData( FALSE );

	m_SoundListControl.SetItem( 0, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );

	m_SelectedSoundTableItem = 0;

	return TRUE;
}

void CWavePackageView::setSoundList( void )
{
	LVITEM			rItem;
	CString         strIndexString;
	LPSNDSCENE      psndscene = NULL;

	this->m_SoundListControl.DeleteAllItems();
	
	for( int i = 0; (DWORD)i < m_sndtablelist.getItemCount(); i++ )
	{	
		psndscene = m_sndtablelist.getSndScene( i );

		if( !psndscene )
		{
			AfxMessageBox( "ERROR : Access sound table list item" );
			break;
		}

		ZeroMemory ( &rItem, sizeof(LVITEM) );

		strIndexString.Format( "%d", i+1 );

        rItem.mask    = LVIF_TEXT;
        rItem.iItem   = i;
        rItem.pszText = strIndexString.GetBuffer(256);

		this->m_SoundListControl.InsertItem( &rItem ); 

		this->m_SoundListControl.SetItemText ( i, 1, psndscene->li.name );

		this->m_SoundListControl.SetItemText ( i, 2, psndscene->li.env );
		
		this->m_SoundListControl.SetItemText( i, 3,  psndscene->li.envLink );
	}

	m_SoundListCount.Format( "%d", m_sndtablelist.getItemCount() );

	UpdateData( FALSE );
}

void CWavePackageView::OnSavewavlistfile() 
{
	// TODO: Add your control notification handler code here

	CFileDialog	FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		    = NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "Sound list file(*.slt)\0*.slt\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Save sound list file";
	FileDlg.m_ofn.lpstrDefExt		= "*.slt";

	if( FileDlg.DoModal() == IDOK )
	{			
		m_sndtablelist.saveSndList( FileDlg.GetPathName() );
	}
}

void CWavePackageView::OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int selectedFilelistindex = -1;
	
	POSITION pos = this->m_SoundListControl.GetFirstSelectedItemPosition();
	
	if ( !pos )	return;

	selectedFilelistindex = this->m_SoundListControl.GetNextSelectedItem(pos);

	if( selectedFilelistindex < 0 ) return;
		
	LPSNDSCENE      psndscene = NULL;
	psndscene = m_sndtablelist.getSndScene( selectedFilelistindex );
	if( !psndscene ) return;

	CEditSoundListItemDlg editsoundlistitemdlg;

	editsoundlistitemdlg.m_SoundNameString = psndscene->li.name;
	editsoundlistitemdlg.m_EnvironmentString = psndscene->li.env;
	editsoundlistitemdlg.m_EnvironmentlinkString = psndscene->li.envLink;

	if( editsoundlistitemdlg.DoModal() == IDOK )
	{
		strcpy( psndscene->li.name, editsoundlistitemdlg.m_SoundNameString.GetBuffer(256) );
		strcpy( psndscene->li.env, editsoundlistitemdlg.m_EnvironmentString.GetBuffer(256) );
		strcpy( psndscene->li.envLink, editsoundlistitemdlg.m_EnvironmentlinkString.GetBuffer(256) );

		this->m_SoundListControl.SetItemText ( selectedFilelistindex, 1, psndscene->li.name );
		this->m_SoundListControl.SetItemText ( selectedFilelistindex, 2, psndscene->li.env );
		this->m_SoundListControl.SetItemText ( selectedFilelistindex, 3, psndscene->li.envLink );

		setSoundSceneInfo( selectedFilelistindex );
	}
	
	*pResult = 0;
}

void CWavePackageView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CPoint glpoint;
	GetCursorPos( &glpoint );

	RECT soundlistcontrolrect;
	m_SoundListControl.GetWindowRect( &soundlistcontrolrect );
		
	if( soundlistcontrolrect.left < point.x && soundlistcontrolrect.right  > point.x && 
		soundlistcontrolrect.top  < point.y && soundlistcontrolrect.bottom > point.y	)
	{
		m_SoundListMenu.TrackPopupMenu( TPM_LEFTALIGN, glpoint.x, glpoint.y, this );		
	}
	
}

void CWavePackageView::OnInsertsoundlistitem() 
{
	// TODO: Add your control notification handler code here
	int selectedFilelistindex = -1;
	
	POSITION pos = this->m_SoundListControl.GetFirstSelectedItemPosition();
	
	if ( pos )
	{
		selectedFilelistindex = this->m_SoundListControl.GetNextSelectedItem(pos);

		if( selectedFilelistindex < 0 ) return;

		m_sndtablelist.insertSceneList( selectedFilelistindex );

		this->setSoundList();

		m_SoundListControl.SetItem( selectedFilelistindex, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );
	}
	else
	{
		m_sndtablelist.insertSceneList( m_sndtablelist.getItemCount() );
		
		this->setSoundList();

		m_SoundListControl.SetItem( m_sndtablelist.getItemCount()-1, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );
	}	
}

void CWavePackageView::OnDeletesoundlistitem() 
{
	// TODO: Add your control notification handler code here
	int selectedFilelistindex = -1;
	
	POSITION pos = this->m_SoundListControl.GetFirstSelectedItemPosition();
	
	if ( !pos )	return;

	
	selectedFilelistindex = this->m_SoundListControl.GetNextSelectedItem(pos);

	if( selectedFilelistindex < 0 ) return;

	m_sndtablelist.deleteSceneList( selectedFilelistindex );
	
	this->setSoundList();

	m_SoundListControl.SetItem( selectedFilelistindex, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );
}



void CWavePackageView::OnKeydownFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if( pLVKeyDown->wVKey == VK_INSERT )
	{
		this->OnInsertsoundlistitem();
	}
	else if( pLVKeyDown->wVKey == VK_DELETE )
	{
		this->OnDeletesoundlistitem();
	}

	*pResult = 0;
}

void CWavePackageView::OnOpensoundscenefile() 
{
/*	// TODO: Add your command handler code here
	int selectedFilelistindex = -1;
	
	POSITION pos = this->m_SoundListControl.GetFirstSelectedItemPosition();
	
	if ( !pos )	return;

	
	selectedFilelistindex = this->m_SoundListControl.GetNextSelectedItem(pos);

	if( selectedFilelistindex < 0 ) return;

	LPSNDSCENE      psndscene = NULL;
	psndscene = m_sndtablelist.getSndScene( selectedFilelistindex );
	
	CString tFilename;

	tFilename  = psndscene->li.name;
	tFilename += _T(".ssl");

	m_sndtablelist.loadSoundScene( tFilename.GetBuffer(256), psndscene );	*/
}


void CWavePackageView::setSoundSceneInfo( DWORD index )
{	
	if( m_sndtablelist.getItemCount() <= 0 || m_sndtablelist.getItemCount() <= index ) return;

	LPSNDSCENE      psndscene = NULL;
	psndscene = m_sndtablelist.getSndScene( index );
	
	CString tFilename;

	tFilename  = psndscene->li.name;
	tFilename += _T(".ssl");

	SNDSCENE temp_psndscene;

	m_SceneCount = 0;
	m_PackageFilename = _T("");
	m_PackagePath = _T("");
		
	CString         strString;
	
	this->m_SoundSceneFileListCtrl.DeleteAllItems();
	
	if( m_sndtablelist.loadSoundScene( tFilename.GetBuffer(256), &temp_psndscene, m_psndInfo ) )
	{
		m_SceneCount = temp_psndscene.nSnd;
		m_PackageFilename = temp_psndscene.loadBlock;
		m_PackagePath = temp_psndscene.loadPath;

		if( m_psndInfo )
		{
			for( DWORD i = 0; i < m_SceneCount; i++ )
			{				
				strString.Format("%d", i );

				this->m_SoundSceneFileListCtrl.InsertItem( i, strString.GetBuffer(256), 0 );

				strString = m_psndInfo[i].loadID;
				this->m_SoundSceneFileListCtrl.SetItemText ( i, 1, strString );

				strString = g_SoundClass[ m_psndInfo[i].sndClass ];
				this->m_SoundSceneFileListCtrl.SetItemText ( i, 2, strString );

				strString.Format("%d", m_psndInfo[i].sndType );
				this->m_SoundSceneFileListCtrl.SetItemText ( i, 3, strString );

				strString.Format("%d", m_psndInfo[i].nInstRequired );
				this->m_SoundSceneFileListCtrl.SetItemText ( i, 4, strString );				
			}

			m_SceneNameString = tFilename;
			UpdateData( FALSE );
		}		
	}
	else
	{
		m_SceneCount = 0;
		m_PackageFilename = _T("");
		m_PackagePath = _T("");
		m_SceneNameString = _T("");
		UpdateData( FALSE );
	}
}

void CWavePackageView::OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int selectedFilelistindex = -1;
	
	POSITION pos = this->m_SoundListControl.GetFirstSelectedItemPosition();
	
	if ( !pos )	return;

	
	selectedFilelistindex = this->m_SoundListControl.GetNextSelectedItem(pos);

	if( selectedFilelistindex < 0 ) return;

	if( m_SelectedSoundTableItem != selectedFilelistindex )
	{
		setSoundSceneInfo( selectedFilelistindex );
	
		if( m_AutoLoadWavePack )
		{
			OnLoadlinkedwavpackfile();			
		}

		m_SelectedSoundTableItem = selectedFilelistindex;
	}
	
	*pResult = 0;
}


void CWavePackageView::OnLoadlinkedwavpackfile() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_PackageFilename.GetLength() ) return;

	CString strRealPackageName;
	
	strRealPackageName = m_PackageFilename;
	strRealPackageName += _T(".dat");

	if( loadWavePackage(strRealPackageName) )
	{
		m_WavePackageFilename = strRealPackageName;
		UpdateData( FALSE );
	}	
}

void CWavePackageView::OnAutoloadwavepackcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();	

	if( this->m_AutoLoadWavePack )
	{
		OnLoadlinkedwavpackfile();
	}
}

void CWavePackageView::OnDblclkSoundscenefilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	*pResult = 0;

	int selectedFilelistindex = -1;
	
	POSITION pos = this->m_SoundSceneFileListCtrl.GetFirstSelectedItemPosition();
	
	if ( !pos )	return;

	
	selectedFilelistindex = this->m_SoundSceneFileListCtrl.GetNextSelectedItem(pos);

	if( selectedFilelistindex < 0 ) return;

	if( !m_psndInfo ) return;

	CModifySoundSceneItemDlg modifysoundsceneitem;

	modifysoundsceneitem.m_LoadIDString = m_psndInfo[selectedFilelistindex].loadID;
	modifysoundsceneitem.m_SoundClass	= m_psndInfo[selectedFilelistindex].sndClass;

	modifysoundsceneitem.m_Normal		= TRUE; //(m_psndInfo[selectedFilelistindex].sndType & SND_NORMAL );
	modifysoundsceneitem.m_3D			= ((m_psndInfo[selectedFilelistindex].sndType & SND_3D ) ? TRUE : FALSE);
	modifysoundsceneitem.m_UseCpuRam    = ((m_psndInfo[selectedFilelistindex].sndType & SND_USECPURAM ) ? TRUE : FALSE);
	modifysoundsceneitem.m_Streaming    = ((m_psndInfo[selectedFilelistindex].sndType & SND_STREAMING ) ? TRUE : FALSE);
	modifysoundsceneitem.m_LoadRam      = ((m_psndInfo[selectedFilelistindex].sndType & SND_LOADRAM ) ? TRUE : FALSE);

	modifysoundsceneitem.m_dwInsideConeAngle  = m_psndInfo[selectedFilelistindex].param3D.props.dwInsideConeAngle;
	modifysoundsceneitem.m_dwOutsideConeAngle = m_psndInfo[selectedFilelistindex].param3D.props.dwOutsideConeAngle;
	modifysoundsceneitem.m_OutsideVolumePerCent = m_psndInfo[selectedFilelistindex].param3D.props.OutsideVolumePerCent;
	modifysoundsceneitem.m_flMinDistance = m_psndInfo[selectedFilelistindex].param3D.props.flMinDistance;
	modifysoundsceneitem.m_flMaxDistance = m_psndInfo[selectedFilelistindex].param3D.props.flMaxDistance;

	modifysoundsceneitem.m_InsideConAngleFlag		= (( m_psndInfo[selectedFilelistindex].param3D.setFlag & SND_INCONESET ) ? TRUE : FALSE);
	modifysoundsceneitem.m_OutsideConAngleFlag		= (( m_psndInfo[selectedFilelistindex].param3D.setFlag & SND_OUTCONESET) ? TRUE : FALSE);
	modifysoundsceneitem.m_OutSideVolumePercentFlag	= (( m_psndInfo[selectedFilelistindex].param3D.setFlag & SND_MINDSET) ? TRUE : FALSE);
	modifysoundsceneitem.m_MinimumDistanceFlag		= (( m_psndInfo[selectedFilelistindex].param3D.setFlag & SND_MAXDSET) ? TRUE : FALSE);
	modifysoundsceneitem.m_MaximumDistanceFlag		= (( m_psndInfo[selectedFilelistindex].param3D.setFlag & SND_OUTVOLSET) ? TRUE : FALSE);


	if( modifysoundsceneitem.DoModal() == IDOK )
	{
		memset( m_psndInfo[selectedFilelistindex].loadID, 0, sizeof(LOADHANDLE) );

		strncpy( m_psndInfo[selectedFilelistindex].loadID, 
			     modifysoundsceneitem.m_LoadIDString.GetBuffer(256),
				 modifysoundsceneitem.m_LoadIDString.GetLength() );
		m_psndInfo[selectedFilelistindex].sndClass   = modifysoundsceneitem.m_SoundClass;
		
		m_psndInfo[selectedFilelistindex].sndType = 0;					
		if( modifysoundsceneitem.m_3D )        m_psndInfo[selectedFilelistindex].sndType += SND_3D;
		if( modifysoundsceneitem.m_UseCpuRam ) m_psndInfo[selectedFilelistindex].sndType += SND_USECPURAM;
		if( modifysoundsceneitem.m_Streaming ) m_psndInfo[selectedFilelistindex].sndType += SND_STREAMING;
		if( modifysoundsceneitem.m_LoadRam   ) m_psndInfo[selectedFilelistindex].sndType += SND_LOADRAM;

		m_psndInfo[selectedFilelistindex].param3D.props.dwInsideConeAngle = modifysoundsceneitem.m_dwInsideConeAngle;
		m_psndInfo[selectedFilelistindex].param3D.props.dwOutsideConeAngle = modifysoundsceneitem.m_dwOutsideConeAngle;
		m_psndInfo[selectedFilelistindex].param3D.props.OutsideVolumePerCent = modifysoundsceneitem.m_OutsideVolumePerCent;
		m_psndInfo[selectedFilelistindex].param3D.props.flMinDistance = modifysoundsceneitem.m_flMinDistance;
		m_psndInfo[selectedFilelistindex].param3D.props.flMaxDistance = modifysoundsceneitem.m_flMaxDistance;

		m_psndInfo[selectedFilelistindex].param3D.setFlag = 0;

		if( modifysoundsceneitem.m_InsideConAngleFlag		) m_psndInfo[selectedFilelistindex].param3D.setFlag += SND_INCONESET;
		if( modifysoundsceneitem.m_OutsideConAngleFlag      ) m_psndInfo[selectedFilelistindex].param3D.setFlag += SND_OUTCONESET;
		if( modifysoundsceneitem.m_OutSideVolumePercentFlag ) m_psndInfo[selectedFilelistindex].param3D.setFlag += SND_MINDSET;
		if( modifysoundsceneitem.m_MinimumDistanceFlag		) m_psndInfo[selectedFilelistindex].param3D.setFlag += SND_MAXDSET;
		if( modifysoundsceneitem.m_MaximumDistanceFlag		) m_psndInfo[selectedFilelistindex].param3D.setFlag += SND_OUTVOLSET;

		// Update list control data...
		CString strString;
		strString = m_psndInfo[selectedFilelistindex].loadID;
		this->m_SoundSceneFileListCtrl.SetItemText ( selectedFilelistindex, 1, strString );

		strString = g_SoundClass[ m_psndInfo[selectedFilelistindex].sndClass ];
		this->m_SoundSceneFileListCtrl.SetItemText ( selectedFilelistindex, 2, strString );

		strString.Format("%d", m_psndInfo[selectedFilelistindex].sndType );
		this->m_SoundSceneFileListCtrl.SetItemText ( selectedFilelistindex, 3, strString );

		strString.Format("%d", m_psndInfo[selectedFilelistindex].nInstRequired );
		this->m_SoundSceneFileListCtrl.SetItemText ( selectedFilelistindex, 4, strString );			
	}
	
}

void CWavePackageView::OnInsertsoundsceneitem() 
{
	// TODO: Add your control notification handler code here
		
	int selectedFilelistindex = -1;

	if( m_SceneCount <= 0 )	
	{
		if( m_psndInfo ) delete[] m_psndInfo;

		m_psndInfo = new SNDINFO;
		memset( m_psndInfo, 0, sizeof(SNDINFO) );			

		selectedFilelistindex = 0;
	}
	else
	{	
		if( !m_psndInfo ) return;

		LPSNDINFO t_psndInfo = NULL;

		t_psndInfo = new SNDINFO[m_SceneCount+1];

		if( !t_psndInfo )
		{
			AfxMessageBox( "ERROR : Allocation memory" );
			return;
		}

		memset( t_psndInfo, 0, sizeof(SNDINFO) * (m_SceneCount+1) );
			
		POSITION pos = this->m_SoundSceneFileListCtrl.GetFirstSelectedItemPosition();	
		if ( pos )
		{	
			selectedFilelistindex = this->m_SoundSceneFileListCtrl.GetNextSelectedItem(pos);
			if( selectedFilelistindex < 0 ) return;

			if( selectedFilelistindex == 0 )
			{
				memcpy( t_psndInfo, m_psndInfo, sizeof(SNDINFO) );

				memcpy( (t_psndInfo+1), m_psndInfo, sizeof(SNDINFO) * (m_SceneCount) );			
			}		
			else
			{
				memcpy( t_psndInfo + selectedFilelistindex, m_psndInfo+selectedFilelistindex, sizeof(SNDINFO) );

				memcpy( t_psndInfo, m_psndInfo, sizeof(SNDINFO) * (selectedFilelistindex) );
				memcpy( (t_psndInfo+selectedFilelistindex+1), m_psndInfo+selectedFilelistindex, 
						 sizeof(SNDINFO) * (m_SceneCount-selectedFilelistindex) );			
			}
		}
		else 
		{
			memcpy( t_psndInfo + m_SceneCount, m_psndInfo, sizeof(SNDINFO) );
			memcpy( t_psndInfo, m_psndInfo, sizeof(SNDINFO) * (m_SceneCount) );	
		}	

		if( m_psndInfo ) delete[] m_psndInfo;
		m_psndInfo = t_psndInfo;
	}

	m_SceneCount++;	

	if( m_psndInfo )
	{
		this->m_SoundSceneFileListCtrl.DeleteAllItems();

		CString strString;
		for( DWORD i = 0; i < m_SceneCount; i++ )
		{				
			strString.Format("%d", i );

			this->m_SoundSceneFileListCtrl.InsertItem( i, strString.GetBuffer(256), 0 );

			strString = m_psndInfo[i].loadID;
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 1, strString );

			strString = g_SoundClass[ m_psndInfo[i].sndClass ];
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 2, strString );

			strString.Format("%d", m_psndInfo[i].sndType );
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 3, strString );

			strString.Format("%d", m_psndInfo[i].nInstRequired );
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 4, strString );				
		}		

		m_SoundSceneFileListCtrl.SetItem( selectedFilelistindex, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );

		UpdateData( FALSE );
	}		
}

void CWavePackageView::OnDeletesoundsceneitem() 
{
	// TODO: Add your control notification handler code here
	if( !m_psndInfo ) return;
	if( m_SceneCount <= 0 ) return;

	
	POSITION pos = this->m_SoundSceneFileListCtrl.GetFirstSelectedItemPosition();	
	if ( !pos ) return;

	int selectedFilelistindex = -1;
	selectedFilelistindex = this->m_SoundSceneFileListCtrl.GetNextSelectedItem(pos);
	if( selectedFilelistindex < 0 ) return;


	LPSNDINFO t_psndInfo = NULL;

	t_psndInfo = new SNDINFO[m_SceneCount-1];

	if( !t_psndInfo )
	{
		AfxMessageBox( "ERROR : Allocation memory" );
		return;
	}

	memset( t_psndInfo, 0, sizeof(SNDINFO) * (m_SceneCount-1) );
	
	if( selectedFilelistindex == 0 )
	{
		memcpy( (t_psndInfo), m_psndInfo+1, sizeof(SNDINFO) * (m_SceneCount-1) );
	}		
	else
	{
		memcpy( t_psndInfo, m_psndInfo, sizeof(SNDINFO) * (selectedFilelistindex) );
		memcpy( (t_psndInfo+selectedFilelistindex), m_psndInfo+selectedFilelistindex+1, 
				 sizeof(SNDINFO) * (m_SceneCount-selectedFilelistindex-1) );
	}	

	if( m_psndInfo ) delete[] m_psndInfo;
	m_psndInfo = t_psndInfo;

	m_SceneCount--;	


	if( m_psndInfo )
	{
		this->m_SoundSceneFileListCtrl.DeleteAllItems();

		CString strString;
		for( DWORD i = 0; i < m_SceneCount; i++ )
		{				
			strString.Format("%d", i );

			this->m_SoundSceneFileListCtrl.InsertItem( i, strString.GetBuffer(256), 0 );

			strString = m_psndInfo[i].loadID;
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 1, strString );

			strString = g_SoundClass[ m_psndInfo[i].sndClass ];
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 2, strString );

			strString.Format("%d", m_psndInfo[i].sndType );
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 3, strString );

			strString.Format("%d", m_psndInfo[i].nInstRequired );
			this->m_SoundSceneFileListCtrl.SetItemText ( i, 4, strString );				
		}		

		if( (DWORD)selectedFilelistindex < m_SceneCount-1 )
			m_SoundSceneFileListCtrl.SetItem( selectedFilelistindex, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );
		else
			m_SoundSceneFileListCtrl.SetItem( m_SceneCount-1, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );

		UpdateData( FALSE );
	}		

}

void CWavePackageView::OnSavescenefile() 
{
	// TODO: Add your control notification handler code here
	int selectedFilelistindex = -1;	
	POSITION pos = this->m_SoundListControl.GetFirstSelectedItemPosition();	
	if ( !pos )	return;	
	selectedFilelistindex = this->m_SoundListControl.GetNextSelectedItem(pos);
	if( selectedFilelistindex < 0 ) return;

	LPSNDSCENE  psndscene = NULL;
	psndscene = m_sndtablelist.getSndScene( selectedFilelistindex );
	
	CString tFilename;
	tFilename  = psndscene->li.name;
	tFilename += _T(".ssl");

	CString tMessage;
	tMessage.Format( "Overwrite %s file ?", tFilename );
	int result = AfxMessageBox( tMessage, MB_YESNOCANCEL );

	if( result == IDYES )
	{
		saveSoundSceneFile( tFilename );
	}
	else if( result == IDNO )
	{
		CFileDialog	FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

		FileDlg.m_ofn.hwndOwner		    = NULL;
		FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
		FileDlg.m_ofn.lpstrFilter		= "Sound scene list file(*.ssl)\0*.ssl\0";
		FileDlg.m_ofn.lpstrInitialDir	= ".\\";
		FileDlg.m_ofn.lpstrTitle		= "Save sound scene list file";
		FileDlg.m_ofn.lpstrDefExt		= "*.ssl";

		if( FileDlg.DoModal() == IDOK )
		{
			saveSoundSceneFile( FileDlg.GetPathName() );
		}
	}
	else
	{
		return;
	}	
}


BOOL CWavePackageView::saveSoundSceneFile( LPCSTR filename )
{	
	FILE* sslfileptr = fopen( filename, "wb" );

	if( !sslfileptr )
	{
		CString tMessageStr; 
		tMessageStr.Format( "ERROR : %s file save !", filename );
		AfxMessageBox( tMessageStr );
		return FALSE;
	}

	char scnHeader[sizeof(SNDSCNHEADER)+1];	
	memset( scnHeader, 0, sizeof(SNDSCNHEADER)+1 );
	strcpy( scnHeader, SNDSCNHEADER );	
	
	fwrite(&scnHeader,sizeof(char),strlen(SNDSCNHEADER)+1,sslfileptr);	

	SNDSCENE temp_psndscene;
	
	temp_psndscene.nSnd = m_SceneCount;
	strcpy( temp_psndscene.loadBlock, m_PackageFilename.GetBuffer(256) );
	strcpy( temp_psndscene.loadPath, m_PackagePath.GetBuffer(256) );

	fwrite(&(temp_psndscene.nSnd),sizeof(DWORD),1,sslfileptr);
	fwrite(temp_psndscene.loadBlock,sizeof(LOADHANDLE),1,sslfileptr);
	fwrite(temp_psndscene.loadPath,sizeof(LOADPATH),1,sslfileptr);
	
	for(DWORD i=0; i<temp_psndscene.nSnd; i++)
	{		
		fwrite(&(m_psndInfo[i]), sizeof(SNDINFO), 1, sslfileptr);				
	}		

	fclose( sslfileptr );
	
	return TRUE;
}




void CWavePackageView::OnDblclkPackedfilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int selectedFilelistindex = -1;	
	POSITION pos = this->m_WaveFileListCtrl.GetFirstSelectedItemPosition();	
	if ( !pos )	
	{
		m_WaveFile.Stop();
		return;
	}

	
	selectedFilelistindex = this->m_WaveFileListCtrl.GetNextSelectedItem(pos);
	if( selectedFilelistindex < 0 ) return;

	g_AllOverwrite = 2;
	g_OverwriteQuestion = FALSE;
	g_AllOverwriteQuestion = FALSE;

	char lpBuffer[512];
	char curBuffer[512];
	GetTempPath( 511, lpBuffer );
	GetCurrentDirectory(511,curBuffer);

	this->m_WavePackage.ExportPackedFile( selectedFilelistindex, lpBuffer );

	_WAVEPACKED_INFOHEADER* wavepackedinfoheader = this->m_WavePackage.GetPackedFileInfo( selectedFilelistindex );

	m_WaveFile.Stop();

	if( m_WaveFile.loadWave( wavepackedinfoheader->filename ) )
	{
		_chdir( lpBuffer );
		CFile::Remove( wavepackedinfoheader->filename );
		_chdir( curBuffer );
		m_WaveFile.Play();
	}

	
	*pResult = 0;
}

void CWavePackageView::OnChangePackagefilenameedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();	
}

void CWavePackageView::OnChangePathedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	
}

void CWavePackageView::OnSetworkforder() 
{
	// TODO: Add your control notification handler code here
	CDirDialog dlg(m_PathString,"All Files (*.*)|*.*||", this);
	
    if( dlg.DoModal() == IDOK )
	{
		//_chdir( dlg.GetPath() );
		SetCurrentDirectory( dlg.GetPath() );  
		return;
	}
	
}
