// wndTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CwndTab window

#ifndef _WND_TABCTRL
#define _WND_TABCTRL

/////////////////////////////////////////////////////////////////////////////
/*
*************************************************
*************************************************
******                                     ******
****                                         ****
****    class CwndTab                        ****
****                                         ****
******                                     ******
*************************************************
*************************************************
*/

#define WNDTAB_MAXTABCOUNT             10
#define PG_WNDTAB_FONT                 0x00010000

class CwndTab : public CWnd
{
	DECLARE_DYNCREATE(CwndTab)

public:
	CwndTab();
	virtual ~CwndTab();
	
	BOOL Create( CWnd* pParentWnd, UINT nID, DWORD dwStyle, const RECT &Rect );
	
// Attributes
public:

   DWORD            m_dwFlag;             // for various things

   UINT             m_uHeight;            // the height for tabs
   int              m_iOffsetToLeft;      // distance from left side to first tab
   UINT             m_uOffsetFontGap;     // gap between font boundary and tab
   BOOL             m_bTabModified;       // if tabs are modified
   
   CDWordArray      m_dwarrayTabWidth;    // the size of each tab
   CFont*           m_pFont;              // text font on the tabs
   CFont*           m_pBoldFont;          // text font on the tabs

   static CString   m_stringWindowClass;

   UINT             m_uSelectedViewIndex; 
   CStringArray     m_sarrayViewName;
   CPtrArray        m_ptrarrayRCView;     // Array for runtime classes for views

   void             AddView( CWnd* pWnd, LPCTSTR pTabTitle );   
   void             ResetViewList( void );
   virtual void     SelectView( UINT nIndex );
   
// Get/Set
public:
   UINT GetHeight() { return m_uHeight; }

   void SetHeight( UINT uHeight );
   void SetModified() { m_bTabModified = TRUE; }
   void SetFont( CFont* pFont ) { m_pFont = pFont; }

// Operations
public:
	void DrawTab( CDC* pDC );
	void DrawSelectedTab( CDC* pD );
	void UpdateTabWidth( CDC* pDC );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CwndTab)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CwndTab)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CreateFont( );
};


#endif