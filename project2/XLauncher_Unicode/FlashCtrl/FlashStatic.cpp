// FlashStatic.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FlashControl.h"
#include "FlashStatic.h"
#include "SPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_PLAY		1
#define TIMER_CURSOR	2

/////////////////////////////////////////////////////////////////////////////
// CFlashStatic

CFlashStatic::CFlashStatic()
{
	flashWin = NULL;
	m_bHighQuality = FALSE;
	m_nZoomIn = 0;

}

CFlashStatic::~CFlashStatic()
{
}


BEGIN_MESSAGE_MAP(CFlashStatic, CStatic)
	//{{AFX_MSG_MAP(CFlashStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATEAPP()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_CONTEXTMENU()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*
ON_COMMAND(IDM_ABOUT_FLASH, OnAboutFlash)
ON_COMMAND(IDM_BACK, OnBack)
ON_COMMAND(IDM_FORWARD, OnForward)
ON_COMMAND(IDM_HIGH_QUALITY, OnHighQuality)
ON_COMMAND(IDM_LOOP, OnLoop)
ON_COMMAND(IDM_PLAY, OnPlay)
ON_COMMAND(IDM_REWIND, OnRewind)
ON_COMMAND(IDM_SHOW_ALL, OnShowAll)
ON_COMMAND(IDM_ZOOM_IN, OnZoomIn)
ON_COMMAND(IDM_ZOOM_OUT, OnZoomOut)
 */

/////////////////////////////////////////////////////////////////////////////
// CFlashStatic message handlers

int CFlashStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!flashWin)
		flashWin = GetFlashControl();
	
	m_brushHollow = (HBRUSH) GetStockObject (HOLLOW_BRUSH);

	return 0;
}


void CFlashStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (flashWin == NULL)
	{
		flashWin = GetFlashControl();
		flashWin->initialize( m_hWnd );
		return;
	}

	flashWin->SetDC( dc.GetSafeHdc());

	CRect rc;
	GetClientRect(rc);

	flashWin->Repaint( rc );

	flashWin->SetDC( 0 );
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CFlashStatic::ResetFlashControl( void )
{
	if( flashWin )
	{
		flashWin->FreeBuffer();
		delete flashWin;
		flashWin = NULL;
	}
}

void CFlashStatic::OnDestroy() 
{
	if(flashWin)
	{
		flashWin->ControlClose();
		flashWin->FreeBuffer();		
		delete flashWin;
		flashWin = 0;
	}

	CStatic::OnDestroy();
}

void CFlashStatic::OnTimer(UINT nIDEvent) 
{
	flashWin->SetDC( GetDC()->GetSafeHdc() );			// the mouse sometimes causes a draw

	int wmId = nIDEvent;             // timer identifier 
	if ( wmId == TIMER_PLAY )
		flashWin->PlayTimerFire();
	else if ( wmId == TIMER_CURSOR )
		flashWin->CursorTimerFire();
	else
		FLASHASSERT( 0 );

	ReleaseDC(CDC::FromHandle(flashWin->GetNWindowDC()) );
	flashWin->SetDC( 0 );			// the mouse sometimes causes a draw
	
	CStatic::OnTimer(nIDEvent);
}

void CFlashStatic::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#ifdef EDITTEXT
	
	if ( nChar != VK_TAB && nChar != VK_RETURN && nChar != VK_BACK) {
		flashWin->HandleKeyDown( nChar, 0 );
	}
	
#endif
	
	CStatic::OnChar(nChar, nRepCnt, nFlags);
}

void CFlashStatic::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#ifdef EDITTEXT

		static BYTE vkCodes[] = {
			VK_LEFT,	//ID_KEY_LEFT
			VK_RIGHT,	//ID_KEY_RIGHT
			VK_HOME,	//ID_KEY_HOME
			VK_END,		//ID_KEY_END
			VK_INSERT,	//ID_KEY_INSERT
			VK_DELETE,	//ID_KEY_DELETE
			0,			//ID_KEY_CLEAR
			VK_BACK,	//ID_KEY_BACKSPACE
			0,			//ID_KEY_CUT		
			0,			//ID_KEY_COPY		
			0,			//ID_KEY_PASTE	
			0,			//ID_KEY_SELECT_ALL
			VK_RETURN,	//ID_KEY_ENTER	
			VK_UP,		//ID_KEY_UP		
			VK_DOWN,	//ID_KEY_DOWN		
			VK_PRIOR,	//ID_KEY_PAGE_UP	
			VK_NEXT,	//ID_KEY_PAGE_DOWN
			VK_TAB,		//ID_KEY_TAB		
			VK_ESCAPE	//ID_KEY_ESCAPE
		};
		int modifiers = 0;

		if (GetKeyState(VK_SHIFT) < 0) {
			modifiers |= Keyboard::ID_KEY_SHIFT;
		}
		if (GetKeyState(VK_CONTROL) < 0) {
			modifiers |= Keyboard::ID_KEY_CTRL;
		}
		BOOL handled = FALSE;
		for (int i=0; i<sizeof(vkCodes); i++) {
			if (nChar == vkCodes[i]) {
				/*handled = */flashWin->HandleKeyDown(i+1, modifiers);
				break;
			}
		}

	
#endif
	
	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CFlashStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	int nHittest = nHitTest;
	if ( nHittest == HTCLIENT && flashWin->UpdateCursor() )
		return true;

	
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CFlashStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	int fwKeys = nFlags;        // key flags 
	int xPos = point.x;  // horizontal position of cursor 
	int yPos = point.y;  // vertical position of cursor 

	flashWin->SetDC( GetDC()->GetSafeHdc() );			// the mouse sometimes causes a draw
	flashWin->MouseMove( xPos, yPos, fwKeys & MK_LBUTTON );
	ReleaseDC(CDC::FromHandle(flashWin->GetNWindowDC()) );
	flashWin->SetDC( 0 );
	
	CStatic::OnMouseMove(nFlags, point);
}

void CFlashStatic::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CStatic::OnActivateApp(bActive, hTask);
	
	FLASHOUTPUT( "Activated\n" );
	if ( BOOL( bActive ) )
	{
		HPALETTE hPalIdeal = CreateIdentityPalette( m_hWnd );
		HDC hDC = GetDC()->GetSafeHdc();

		// Realize our ideal palette to be sure all the colors are used in the device palette
		HPALETTE hOldPal = SelectPalette(hDC, hPalIdeal, FALSE);
		RealizePalette(hDC);

		// Restore the old palette
		SelectPalette(hDC, hOldPal, TRUE);
		
		::ReleaseDC(GetSafeHwnd(), hDC);
		DeleteObject(hPalIdeal);
	}	
}

void CFlashStatic::OpenFile(char *szFileName)
{
	if (flashWin == NULL)
	{
		flashWin = GetFlashControl();
		flashWin->initialize( m_hWnd );
	}

	flashWin->ControlOpen(szFileName);
}

void CFlashStatic::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);

	if (flashWin != NULL)
		flashWin->FreeBuffer();
	
}

void CFlashStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int xPos = point.x;  // horizontal position of cursor 
	int yPos = point.y;  // vertical position of cursor 

	SetCapture();

	flashWin->SetDC( GetDC()->GetSafeHdc());			// the mouse sometimes causes a draw
	flashWin->MouseDown( xPos, yPos );
	::ReleaseDC( m_hWnd, flashWin->GetNWindowDC() );
	flashWin->SetDC( 0 );
	
	CStatic::OnLButtonDown(nFlags, point);
}

void CFlashStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int xPos = point.x;  // horizontal position of cursor 
	int yPos = point.y;  // vertical position of cursor 
	ReleaseCapture();

	flashWin->SetDC( GetDC()->GetSafeHdc());			// the mouse sometimes causes a draw
	flashWin->MouseUp( xPos, yPos );
	::ReleaseDC( m_hWnd, flashWin->GetNWindowDC() );
	flashWin->SetDC( 0 );
	
	CStatic::OnLButtonUp(nFlags, point);
}

BOOL CFlashStatic::OnQueryNewPalette() 
{
	FLASHOUTPUT( "QueryNewPalette\n" );
	HPALETTE hPalIdeal = CreateIdentityPalette( m_hWnd );
	if ( hPalIdeal ) 
	{
		HDC hDC = ::GetDC(m_hWnd);
		
		// Realize our ideal palette to be sure all the colors are used in the device palette
		HPALETTE hOldPal = SelectPalette(hDC, hPalIdeal, FALSE);
		int result = RealizePalette(hDC);
		
		// Restore the old palette
		::SelectPalette(hDC, hOldPal, TRUE);
		// 					RealizePalette(hDC);
		
		::ReleaseDC(m_hWnd, hDC);
		::DeleteObject(hPalIdeal);
		
		if ( result )
		{
			flashWin->FreeBuffer();
		}
	}
	
	return CStatic::OnQueryNewPalette();
}

void CFlashStatic::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CStatic::OnPaletteChanged(pFocusWnd);
	
	if (pFocusWnd->GetSafeHwnd() == m_hWnd)
		return;
	
	FLASHOUTPUT( "QueryNewPalette\n" );
	HPALETTE hPalIdeal = CreateIdentityPalette( m_hWnd );
	if ( hPalIdeal ) 
	{
		HDC hDC = ::GetDC(m_hWnd);
		
		// Realize our ideal palette to be sure all the colors are used in the device palette
		HPALETTE hOldPal = SelectPalette(hDC, hPalIdeal, FALSE);
		int result = RealizePalette(hDC);
		
		// Restore the old palette
		::SelectPalette(hDC, hOldPal, TRUE);
		// 					RealizePalette(hDC);
		
		::ReleaseDC(m_hWnd, hDC);
		::DeleteObject(hPalIdeal);
		
		if ( result )
		{
			flashWin->FreeBuffer();
		}
	}

	
}

void CFlashStatic::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();
}


HPALETTE CFlashStatic::CreateIdentityPalette( HWND hwnd )
{
	LOGPALETTE256 logpal = { 0x300, 256	};
	int i, n, nStatCols;

	HDC hdc = ::GetDC( NULL );

	/* The number of static colours should be 20, but inquire it anyway */
	nStatCols = GetDeviceCaps(hdc, NUMRESERVED);

	/* Copy the entire system palette, though we only need the first 10
	* and the last 10 entries */
	GetSystemPaletteEntries(hdc, 0, 256, logpal.palEntry);

	/* Clear the peFlags of the lower and upper static colours */
	for (i = 0; i < (nStatCols / 2); i++)
		logpal.palEntry[i].peFlags = 0;
	for (i = 256 - (nStatCols / 2); i < 256; i++)
		logpal.palEntry[i].peFlags = 0;

	// Set the palette entries to the DIB colours.
	// This is a well behaved function, but we are really relying
	// on using 6*6*6 = 216 colors. 
	// The rest is given to grey scale (20 colors) and the OS (20 colors.)
	for (i = (nStatCols / 2), n = 0 ; i < 256 - (nStatCols / 2); i++, n++ ) 
	{
		if ( i < 6*6*6 )
		{
			logpal.palEntry[i].peRed   = colorRamp6[ (n/36) % 6 ];
			logpal.palEntry[i].peGreen = colorRamp6[ (n/6)  % 6 ];
			logpal.palEntry[i].peBlue  = colorRamp6[ (n)    % 6 ];
			logpal.palEntry[i].peFlags = PC_NOCOLLAPSE;
		}
		else
		{
			logpal.palEntry[i].peRed   = ( n - 6*6*6 ) * 12 + 10;
			logpal.palEntry[i].peGreen = ( n - 6*6*6 ) * 12 + 10;
			logpal.palEntry[i].peBlue  = ( n - 6*6*6 ) * 12 + 10; 
			logpal.palEntry[i].peFlags = PC_NOCOLLAPSE;
		}
	}

	::ReleaseDC( 0, hdc );
	return CreatePalette((LOGPALETTE *)&logpal);
}


void CFlashStatic::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	/*
	CMenu menu;
	menu.LoadMenu(IDR_FLASH);
	CMenu *pSubMenu = menu.GetSubMenu(0);

	if (m_bHighQuality)
		pSubMenu->ModifyMenu(IDM_HIGH_QUALITY, MF_BYCOMMAND |MF_CHECKED, IDM_HIGH_QUALITY, "High Quality");
	else
		pSubMenu->ModifyMenu(IDM_HIGH_QUALITY, MF_BYCOMMAND |MF_UNCHECKED, IDM_HIGH_QUALITY, "High Quality");

	if (m_nZoomIn != 0)
		pSubMenu->ModifyMenu(IDM_ZOOM_OUT, MF_BYCOMMAND |MF_ENABLED, IDM_ZOOM_OUT, "Zoom Out");
	else
		pSubMenu->ModifyMenu(IDM_ZOOM_OUT, MF_BYCOMMAND | MF_DISABLED |MF_GRAYED, IDM_ZOOM_OUT, "Zoom Out");

	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN , point.x, point.y, this);
	*/
}

void CFlashStatic::OnAboutFlash() 
{
	ShellExecute(NULL, _T("open"),_T("http://www.macromedia.com"), NULL, NULL,SW_SHOWNORMAL);	
	
}

void CFlashStatic::OnBack() 
{
	flashWin->ControlBack();	
	
}

void CFlashStatic::OnForward() 
{
	flashWin->ControlForward();	
	
}

void CFlashStatic::OnHighQuality() 
{
	flashWin->ControlHighQuality();	
	m_bHighQuality = 1- m_bHighQuality;

}

void CFlashStatic::OnLoop() 
{
	flashWin->ControlLoop();	
	
}

void CFlashStatic::OnPlay() 
{
	flashWin->ControlPlay();	
	
}

void CFlashStatic::OnRewind() 
{
	flashWin->ControlRewind();	
}

void CFlashStatic::OnShowAll() 
{
	flashWin->ControlViewAll();
	
}

void CFlashStatic::OnZoomIn() 
{
	flashWin->ControlZoomIn();

	m_nZoomIn++;
}

void CFlashStatic::OnZoomOut() 
{
	m_nZoomIn--;
	flashWin->ControlZoomOut();
}

HBRUSH CFlashStatic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// Make static controls transparent
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		// Make sure that it's not a slider control
		TCHAR lpszClassName[256];
		GetClassName (pWnd->m_hWnd, lpszClassName, 255);
		if (wcscmp (lpszClassName, TRACKBAR_CLASS) == 0)
			return CStatic::OnCtlColor(pDC, pWnd, nCtlColor);
		
		pDC->SetBkMode (TRANSPARENT);
		return m_brushHollow;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CFlashStatic::OnEraseBkgnd(CDC* pDC) 
{
	return !TRUE;//CDialog::OnEraseBkgnd(pDC);
}