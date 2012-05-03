#if !defined(AFX_PACKINGVIEW_H__50B2B908_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_PACKINGVIEW_H__50B2B908_6BAA_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PackingView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPackingView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "XTreeCtrl.h"
#include "SectionList.h"
#include "ProgressDlg.h"

class CPackingView : public CDialog
{
protected:	
	DECLARE_DYNCREATE(CPackingView)

	CMenu m_Level1ContextMenu;
	CMenu m_Level2ContextMenu;

	CMenu m_ListControlContextMenu;	
	
	CString m_PackingPath;
// Form Data
public:
		
	//{{AFX_DATA(CPackingView)
	enum { IDD = IDD_PACKINGFORMVIEW };
	CListCtrl	m_FileListCtrl;
	CXTreeCtrl	m_PackingTreeControl;
	CString	m_TotalSize;
	CString	m_TotalCount;
	CString	m_SelectedPackagename;
	CString	m_SelectedSectionname;
	BOOL	m_Zipping;
	BOOL	m_ConvertTGAToDDS;
	//}}AFX_DATA

// Attributes
public:

	int       getSelectItemSequence( HTREEITEM rghItem );
	int       getTreeLevel(HTREEITEM rghItem);
	int       getOnelevelSubTreeCount( HTREEITEM parentItem );
	void      resetTree();
	HTREEITEM insertNode( HTREEITEM Parent, LPSTR Text, HTREEITEM InsertAfter = TVI_LAST );

	void      setItem( int SectionIndex );
	void      setCurrentFileItem( void );

	BOOL      insertFileItem( LPCTSTR filename );


	CPackage  m_FilePackage;

	CString   m_PathString;
	
	CProgressDlg progressdlg;
	int          progressrate;
	
	CImageList    m_imglist;
	CImageList*   pTreeimagelist;
// Operations
public:
	BOOL FilePacking( BOOL Allpackingflag = TRUE );
	BOOL checkPackingFilelistValiable( int packageindex );
	BOOL loadXPP( LPCSTR filename );
	BOOL loadStringandSize(LPSTR string, int maxsize, FILE *fp);
	
	BOOL saveStringandSize( CString& string, FILE* fp );
	BOOL saveStringandSize(LPSTR string, FILE *fp);
	
	BOOL savePackStructFile( LPCSTR filename );

	CPackingView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPackingView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPackingView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPackingView)
	afx_msg void OnSelchangedPackingtree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickPackingtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnInsertmenuitem();
	afx_msg void OnDeletemenuitem();
	afx_msg void OnInsertsectionmenuitem();
	afx_msg void OnDeletesectionmenuitem();
	afx_msg void OnInsertfilesmenuitem();
	afx_msg void OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditPackingtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeletefilelistitem();
	afx_msg void OnDeleteallfilelistitem();
	afx_msg void OnPacking();
	afx_msg void OnExtract();
	afx_msg void OnPacklistload();
	afx_msg void OnPacklistsave();
	afx_msg void OnResetpackinglist();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeydownPackingtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSortfilelist();
	afx_msg void OnMakepatchlist();	
	afx_msg void OnMakeallpackage();
	afx_msg void OnMakewavepackage();
	afx_msg void OnZippingcheck();
	afx_msg void OnConverttgatodds();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKINGVIEW_H__50B2B908_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
