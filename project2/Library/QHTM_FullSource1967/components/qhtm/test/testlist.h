#if !defined(AFX_TESTLIST_H__42FAFD71_A145_472F_A0B2_A6C117203DC5__INCLUDED_)
#define AFX_TESTLIST_H__42FAFD71_A145_472F_A0B2_A6C117203DC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestList window
 
class CTestList : public baseCMyBar
{
// Construction
public:
	CTestList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestList)
	//}}AFX_VIRTUAL
	BOOL Create( CWnd* pParentWnd );

// Implementation
public:
	virtual ~CTestList();

	bool AddFilesFromPath( LPCTSTR pcszPath, HTREEITEM htreeParent );

	CTreeCtrl m_treeList;
	CArray< CString, CString > m_arrFile;

	CString m_strLastLoaded;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTestList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LRESULT OnMyFirstSelect( WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLIST_H__42FAFD71_A145_472F_A0B2_A6C117203DC5__INCLUDED_)
