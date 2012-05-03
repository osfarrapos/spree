// wndTab.cpp : implementation file
//

#include "stdafx.h"

#include "wndTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define RGB_GETRED(rgb)     (((rgb) >> 16) & 0xff) 
#define RGB_GETGREEN(rgb)   (((rgb) >> 8) & 0xff)
#define RGB_GETBLUE(rgb)    ((rgb) & 0xff) 

/************************************************
*************************************************
******                                     ******
****                                         ****
****    class CwndTab                        ****
****                                         ****
******                                     ******
*************************************************
************************************************/


IMPLEMENT_DYNCREATE( CwndTab, CWnd );

CString CwndTab::m_stringWindowClass;

/************************************************
   *   Constructor/Destructor 
   *
 * * *
  ***
   *
*/
CwndTab::CwndTab()
{
   m_dwFlag = 0;
   m_uHeight = 16;
   m_bTabModified = TRUE;
   m_pFont = NULL;   
   m_pBoldFont = NULL;
   m_iOffsetToLeft = 4,   
   m_uOffsetFontGap = 13;
   m_uSelectedViewIndex = 0;	   
}

CwndTab::~CwndTab()
{
   if( m_dwFlag & PG_WNDTAB_FONT ) delete m_pFont;
   if( m_dwFlag & PG_WNDTAB_FONT ) delete m_pBoldFont;
}

BOOL CwndTab::Create( CWnd* pParentWnd, UINT nID, DWORD dwStyle, const RECT &Rect )
{
	CString wndclass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), 
	                   CreateSolidBrush(GetSysColor(COLOR_BTNFACE)), 0); 

	if (!CWnd::Create(wndclass, _T("Tool tab"), dwStyle , Rect, pParentWnd, nID)) 
		return FALSE; 

	CreateFont();
	
	return TRUE;
}


/*-------------------------------------------------------------------------------------------------
Name:	   SetHeight()
Type:    Function
Purpose: Set height for tabs

Param:   uHeight = tab height
*/
void CwndTab::SetHeight( UINT uHeight )
{ 
   m_uHeight      = uHeight;
   Invalidate();
}


/************************************************
   *   Virtual functions
   *
 * * *
  ***
   *
*/
/*-------------------------------------------------------------------------------------------------
Name:	   PreCreateWindow()
Type:    Virtual function
Purpose: Register a new window class
*/
BOOL CwndTab::PreCreateWindow(CREATESTRUCT& cs) 
{
   if( CwndTab::m_stringWindowClass.IsEmpty() )
   {
      CwndTab::m_stringWindowClass =
         ::AfxRegisterWndClass( CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW,
                                AfxGetApp()->LoadStandardCursor(IDC_ARROW),
                                (HBRUSH)(COLOR_BTNFACE+1),
                                NULL );
   }

   cs.lpszClass = CwndTab::m_stringWindowClass;   
   cs.style &= ~(long)FWS_ADDTOTITLE;
   cs.style = cs.style | CS_DBLCLKS; 
   	
   return CWnd::PreCreateWindow(cs);
}

/************************************************
   *   Functions 
   *
 * * *
  ***
   *
*/
/*-------------------------------------------------------------------------------------------------
Name:	   CreateFont()
Type:    Function
Purpose: Create font for text on tabs
*/
void CwndTab::CreateFont()
{
   LOGFONT logfont;

   memset( &logfont, 0, sizeof( LOGFONT ) );   
   logfont.lfHeight = m_uHeight - 4;   
   strcpy(logfont.lfFaceName,"MS Sans Serif");  
   
   m_pFont = new CFont();
   m_pFont->CreateFontIndirect(&logfont);    

   LOGFONT logboldfont;

   memset( &logboldfont, 0, sizeof( LOGFONT ) );   
   logboldfont.lfHeight = m_uHeight - 4;   
   strcpy(logboldfont.lfFaceName,"MS Sans Serif"); 
   logboldfont.lfWeight = FW_BOLD;
   
   m_pBoldFont = new CFont();
   m_pBoldFont->CreateFontIndirect(&logboldfont);    

   m_dwFlag |= PG_WNDTAB_FONT;
}

/*-------------------------------------------------------------------------------------------------
Name:	   DrawTab()
Type:    Function
Purpose: Draw all the tabs

Param:   pDC           = DC that the tabs should be drawn on.
*/
void CwndTab::DrawTab( CDC* pDC )
{   
   UINT  uTabIndex;          // tab that are drawn
   POINT ppoint[5];          // points that will be used to draw tab                                                                   
                             
   UINT  uTabHeight;         // the tab height 
   UINT  uDistanceToNextTab; // distance between tabs
   int   iOffsetToLeft;      // distance from left side of client edge                                                                 
                             
   CRect    rect;
   CPen     penBlack( PS_SOLID, 1, RGB(0,0,0) );
   CPen*    ppenOld;

   CRgn   rgn;               // filling region
   CBrush brush;             // brush that the selected tab will be filled with

   if( m_bTabModified ) UpdateTabWidth( pDC );             // update tab ?

   ppenOld = pDC->SelectObject( &penBlack );               // select a black pen

   pDC->SetBkMode( TRANSPARENT );                          // just text
      
   uTabHeight         = m_uHeight - 1;
   uDistanceToNextTab = uTabHeight / 2;
   iOffsetToLeft      = m_iOffsetToLeft;

   COLORREF tColor = ::GetSysColor( COLOR_BTNFACE );

   tColor = RGB( RGB_GETBLUE( tColor )-12, RGB_GETGREEN( tColor )-12, RGB_GETRED( tColor )-12 );

   brush.CreateSolidBrush( tColor );
   
   for( uTabIndex = 0; uTabIndex < (UINT)m_dwarrayTabWidth.GetSize(); uTabIndex++ )
   {
      // ***
      // set all points for tab, then we will be able to draw it
      // ***
      ppoint[0].x = iOffsetToLeft;                         // ""
      ppoint[0].y = 0;

	  iOffsetToLeft += uDistanceToNextTab;                 // "\" 
      ppoint[1].x   =  iOffsetToLeft;
      ppoint[1].y   =  uTabHeight;	  
      
      iOffsetToLeft += m_dwarrayTabWidth[uTabIndex];       // "\____"
      ppoint[2].x   =  iOffsetToLeft;
      ppoint[2].y   =  uTabHeight;

      if( uTabIndex == (UINT)(m_dwarrayTabWidth.GetSize() - 1) ) // "\____/"
      {
         iOffsetToLeft += uDistanceToNextTab;		 
         ppoint[3].x   =  iOffsetToLeft;
         ppoint[3].y   =  0;
      }
      else
      {
         iOffsetToLeft += (uDistanceToNextTab / 2);
         ppoint[3].x   =  iOffsetToLeft;
         ppoint[3].y   =  (uTabHeight / 2);

         iOffsetToLeft -= (uDistanceToNextTab / 2);
      }

	  ppoint[4].x   =  ppoint[3].x;
      ppoint[4].y   =  0;

	  rgn.CreatePolygonRgn( ppoint, 5, ALTERNATE );

	  pDC->FillRgn( &rgn, &brush );

	  rgn.DeleteObject();
	  
      pDC->Polyline( ppoint, 4 );

	  //draw tab-text 
      pDC->TextOut( ppoint[1].x + m_uOffsetFontGap,
                    1,
                    m_sarrayViewName[uTabIndex] );

   }

   GetClientRect( &rect );                                 // get client rect
   pDC->MoveTo( rect.left, rect.top );                     // move to upper left
   pDC->LineTo( rect.right, rect.left );                   // draw a line from left to right

   pDC->SelectObject( ppenOld );
}

/*-------------------------------------------------------------------------------------------------
Name:	   DrawSelectedTab
Type:    Function
Purpose: Draw the tab for selected view

Param:   pDC           = DC that the tabs should be drawn on.
*/
void CwndTab::DrawSelectedTab( CDC* pDC )
{
   CRgn   rgn;               // filling region
   CBrush brush;             // brush that the selected tab will be filled with

   UINT  uTabHeight;         // the tab height
   UINT  uDistanceToNextTab; // distance between tabs
   int   iOffsetToLeft;      // distance from left side of client edge

   POINT  ppoint[4];         // points that will be used to draw tab

   uTabHeight         = m_uHeight - 1;                     // set tab height
   uDistanceToNextTab = uTabHeight / 2;                    // distance to next tab
   iOffsetToLeft      = m_iOffsetToLeft;                   // distance to left side

   if( m_sarrayViewName.GetSize() == 0 )
   {
	   return;
   }
   
   // *** loop to selected tab ***
   if( m_uSelectedViewIndex < (UINT)(m_sarrayViewName.GetSize()) )
   {
	   for( UINT uCounter = 0; uCounter < m_uSelectedViewIndex; uCounter++ )
	   {
		  iOffsetToLeft += m_dwarrayTabWidth[uCounter];
		  iOffsetToLeft += uDistanceToNextTab;
	   }

	   // ***
	   // set all points for selected tab, then we will be able to draw it
	   // ***
	   ppoint[0].x = iOffsetToLeft;                            // ""
	   ppoint[0].y = 0;

	   iOffsetToLeft += uDistanceToNextTab;                    // "\"
	   ppoint[1].x   =  iOffsetToLeft;
	   ppoint[1].y   =  uTabHeight;

	   iOffsetToLeft += m_dwarrayTabWidth[uCounter];           // "\____"
	   ppoint[2].x   =  iOffsetToLeft;
	   ppoint[2].y   =  uTabHeight;

	   iOffsetToLeft += uDistanceToNextTab;                    // "\____/"
	   ppoint[3].x   =  iOffsetToLeft;
	   ppoint[3].y   =  0;

	   //brush.CreateSolidBrush( ::GetSysColor( COLOR_WINDOW ) );
	   brush.CreateSolidBrush( ::GetSysColor( COLOR_BTNFACE ) );
	   rgn.CreatePolygonRgn( ppoint, 4, ALTERNATE );
	
	   pDC->FillRgn( &rgn, &brush );
	   pDC->Polyline( ppoint, 4 );
	  
	   pDC->SetBkMode( TRANSPARENT );                          // just text

	   //pDC->SetTextColor( RGB( 100, 100, 100 ) );
	   //pDC->TextOut( ppoint[1].x + m_uOffsetFontGap , 1, m_sarrayViewName[uCounter] );
	   //pDC->SetTextColor( RGB( 0, 0, 40 ) );
	   //pDC->TextOut( ppoint[1].x + m_uOffsetFontGap - 1, 0, m_sarrayViewName[uCounter] );

	   CSize size = pDC->GetTextExtent( m_sarrayViewName[uCounter] );

	   pDC->SetTextColor( RGB( 0, 0, 60 ) );
	   pDC->TextOut( ppoint[1].x + ((ppoint[2].x - ppoint[1].x)>>1)-(size.cx>>1), 0, m_sarrayViewName[uCounter] );

   }
}

/*-------------------------------------------------------------------------------------------------
Name:	   UpdateTabWidth()
Type:    Function
Purpose: Update widenes for all tabs

Param:   pDC = DC that the tabs should be drawn on.
*/
void CwndTab::UpdateTabWidth( CDC* pDC )
{
   CSize size;

   m_dwarrayTabWidth.RemoveAll();

   for( int iCounter = 0; iCounter < m_sarrayViewName.GetSize(); iCounter++ )
   {
      // get text size
      size = pDC->GetTextExtent( m_sarrayViewName[iCounter] );
      // add the width for text plus gap to array
      m_dwarrayTabWidth.Add( size.cx + (m_uOffsetFontGap << 1) );
   }

   m_bTabModified = FALSE;                                 // tabs updated
}


BEGIN_MESSAGE_MAP(CwndTab, CWnd)
	//{{AFX_MSG_MAP(CwndTab)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/************************************************
   *   Messages
   *
 * * *
  ***
   *
*/

/*-------------------------------------------------------------------------------------------------
Name:	   OnPaint()
Type:    Message
Purpose: Draw all the information in the tab window
*/
void CwndTab::OnPaint() 
{
   CFont*   pfontOld;   // pointer to old font

   CPaintDC dc(this);                                      // device context for painting

   if( m_pFont == NULL ) CreateFont();                     // if no tab font, create one

   pfontOld = dc.SelectObject( m_pFont );                  // select tab font

   DrawTab( &dc );                                         // draw the tabs  
   
   dc.SelectObject( m_pBoldFont );                  // select tab font

   DrawSelectedTab( &dc );                                 // draw th selected tab

   dc.SelectObject( pfontOld );                            // select old font
}



/*-------------------------------------------------------------------------------------------------
Name:	   OnLButtonDown()
Type:    Message
Purpose: Check if klicked on a tab, and if so switch view
*/
void CwndTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
   int  iOffsetToLeft;
   UINT uDistanceToNextTab;

   uDistanceToNextTab = (m_uHeight - 1) / 2;
   
   iOffsetToLeft = m_iOffsetToLeft;

   // *** check if clicked on a tab ***
   for( UINT uCounter = 0; uCounter < (UINT)m_dwarrayTabWidth.GetSize(); uCounter++ )
   {
      iOffsetToLeft += uDistanceToNextTab;

      if( ( point.x >= (iOffsetToLeft - 1) ) && 
          ( point.x <= (int)(iOffsetToLeft + m_dwarrayTabWidth[uCounter] + 1) ) )
      {
		 SelectView( uCounter );
		 break;
      }

      iOffsetToLeft += m_dwarrayTabWidth[uCounter];

   }

	CWnd::OnLButtonDown(nFlags, point);
}

void CwndTab::OnLButtonDblClk( UINT nFlags, CPoint point )
{	
	CWnd::OnLButtonDblClk( nFlags, point );
}

void CwndTab::AddView( CWnd* pWnd, LPCTSTR pTabTitle )
{	
	if( !pWnd ) return;

	m_ptrarrayRCView.Add( pWnd );
	m_sarrayViewName.Add( pTabTitle );

	CDC* pDC = GetDC();
	UpdateTabWidth( pDC );
	ReleaseDC( pDC );

	SetModified();
}

void CwndTab::ResetViewList( void )
{
	m_ptrarrayRCView.RemoveAll();
	m_sarrayViewName.RemoveAll();

	m_uSelectedViewIndex = -1;

	CDC* pDC = GetDC();
	UpdateTabWidth( pDC );
	ReleaseDC( pDC );

	SetModified();
}

void CwndTab::SelectView( UINT nIndex )
{	 	 
	 ASSERT( nIndex < (UINT)m_ptrarrayRCView.GetSize() );

	 for( UINT i = 0; i < (UINT)m_ptrarrayRCView.GetSize(); i++ )
	 {
		 ((CView*)(CRuntimeClass*)m_ptrarrayRCView[i])->ShowWindow( (( nIndex == i ) ? SW_SHOW : SW_HIDE) );
	 }
	 
	 m_uSelectedViewIndex = nIndex;	 

	 Invalidate();         	
}

