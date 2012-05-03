// CxShadeButton.cpp : implementation file
/** 12/05/2001 v1.00
 * ing.davide.pizzolato@libero.it
 */

#include "stdafx.h"
#include "xShadeButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxDib (under development) 
/////////////////////////////////////////////////////////////////////
CxDib::CxDib()
{
	hDib=NULL;

	memset(&m_bi,0,sizeof(BITMAPINFOHEADER));
    m_bi.biSize = sizeof(BITMAPINFOHEADER);

	m_LineWidth=0;
	m_nColors=0;
}
/////////////////////////////////////////////////////////////////////
CxDib::~CxDib()
{
	if (hDib) free(hDib);
}
/////////////////////////////////////////////////////////////////////
bool CxDib::IsWin30Dib(){
	return ((*(LPDWORD)(hDib))==sizeof(BITMAPINFOHEADER));
}
/////////////////////////////////////////////////////////////////////
WORD CxDib::GetPaletteSize()
{
//    if (IsWin30Dib())
        return (m_nColors * sizeof(RGBQUAD));
//    else return (m_nColors * sizeof(RGBTRIPLE));
}
/////////////////////////////////////////////////////////////////////
BYTE* CxDib::GetBits()
{ 
	if (hDib)	return ((BYTE*)hDib + *(LPDWORD)hDib + GetPaletteSize()); 
	return NULL;
}
/////////////////////////////////////////////////////////////////////
HDIB  CxDib::Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
    LPBITMAPINFOHEADER  lpbi;	// pointer to BITMAPINFOHEADER
    DWORD               dwLen;	// size of memory block

	if (hDib) free(hDib);
	hDib=NULL;

    // Make sure bits per pixel is valid
    if (wBitCount <= 1)			wBitCount = 1;
    else if (wBitCount <= 4)	wBitCount = 4;
    else if (wBitCount <= 8)	wBitCount = 8;
    else				        wBitCount = 24;

    switch (wBitCount){
        case 1:
            m_nColors = 2;
			break;
        case 4:
            m_nColors = 16;
			break;
        case 8:
            m_nColors = 256;
			break;
        default:
            m_nColors = 0;
    }

    m_LineWidth = WIDTHBYTES(wBitCount * dwWidth);

    // initialize BITMAPINFOHEADER
    m_bi.biSize = sizeof(BITMAPINFOHEADER);
    m_bi.biWidth = dwWidth;         // fill in width from parameter
    m_bi.biHeight = dwHeight;       // fill in height from parameter
    m_bi.biPlanes = 1;              // must be 1
    m_bi.biBitCount = wBitCount;    // from parameter
    m_bi.biCompression = BI_RGB;    
    m_bi.biSizeImage = m_LineWidth * dwHeight;
    m_bi.biXPelsPerMeter = 0;
    m_bi.biYPelsPerMeter = 0;
    m_bi.biClrUsed = 0;
    m_bi.biClrImportant = 0;

    // calculate size of memory block required to store the DIB.  This
    // block should be big enough to hold the BITMAPINFOHEADER, the color
    // table, and the bits
    dwLen = GetSize();

    hDib = malloc(dwLen); // alloc memory block to store our bitmap
    if (!hDib) return NULL;

    // use our bitmap info structure to fill in first part of
    // our DIB with the BITMAPINFOHEADER
	lpbi = (LPBITMAPINFOHEADER)(hDib);
    *lpbi = m_bi;

    return hDib; //return handle to the DIB
}
/////////////////////////////////////////////////////////////////////
long CxDib::Draw(HDC pDC, long xoffset, long yoffset)
{
	if((hDib)&&(pDC))  {
		//palette must be correctly filled
		LPSTR lpDIB = (char*)hDib;	//set image to hdc...
		SetStretchBltMode(pDC,COLORONCOLOR);	
		SetDIBitsToDevice(pDC, xoffset, yoffset,
			m_bi.biWidth, m_bi.biHeight, 0, 0, 0,
			m_bi.biHeight, GetBits(),
			(BITMAPINFO*)lpDIB, DIB_RGB_COLORS);
		return 1;
	} 
	return 0;
}
/////////////////////////////////////////////////////////////////////
long CxDib::Stretch(HDC pDC, long xoffset, long yoffset, long xsize, long ysize)
{
	if((hDib)&&(pDC)) {
		//palette must be correctly filled
		LPSTR lpDIB = (char*)hDib;	//set image to hdc...
		SetStretchBltMode(pDC,COLORONCOLOR);	
		StretchDIBits(pDC, xoffset, yoffset,
					xsize, ysize, 0, 0, m_bi.biWidth, m_bi.biHeight,
					GetBits(),(BITMAPINFO*)lpDIB,DIB_RGB_COLORS,SRCCOPY);
		return 1;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPaletteIndex(BYTE idx, BYTE r, BYTE g, BYTE b)
{
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) b;
			iDst[ldx++] = (BYTE) g;
			iDst[ldx++] = (BYTE) r;
			iDst[ldx] = (BYTE) 0;
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPaletteIndex(BYTE idx, RGBQUAD c)
{
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) c.rgbBlue;
			iDst[ldx++] = (BYTE) c.rgbGreen;
			iDst[ldx++] = (BYTE) c.rgbRed;
			iDst[ldx] = (BYTE) 0;
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPaletteIndex(BYTE idx, COLORREF cr)
{
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) GetBValue(cr);
			iDst[ldx++] = (BYTE) GetGValue(cr);
			iDst[ldx++] = (BYTE) GetRValue(cr);
			iDst[ldx] = (BYTE) 0;
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::GetPaletteIndex(BYTE idx)
{
	RGBQUAD rgb = {0,0,0,0};
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			rgb.rgbBlue = iDst[ldx++];
			rgb.rgbGreen=iDst[ldx++];
			rgb.rgbRed =iDst[ldx];
			rgb.rgbReserved = 0;
		}
	}
	return rgb;
}
/////////////////////////////////////////////////////////////////////
BYTE CxDib::GetPixelIndex(long x,long y)
{
	if ((hDib==NULL)||(m_nColors==0)||
		(x<0)||(y<0)||(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return 0;
	BYTE* iDst = GetBits();
	return iDst[(m_bi.biHeight - y)*m_LineWidth + x];
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::GetPixelColor(long x,long y)
{
	RGBQUAD rgb={0,0,0,0};
	if ((hDib==NULL)||(x<0)||(y<0)||
		(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return rgb;
	if (m_nColors) return GetPaletteIndex(GetPixelIndex(x,y));
	else {
		BYTE* iDst = GetBits()+(m_bi.biHeight - y)*m_LineWidth + x*sizeof(RGBQUAD);
		rgb.rgbBlue = *iDst++;
		rgb.rgbGreen= *iDst++;
		rgb.rgbRed =*iDst;
		return rgb;
	}
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPixelIndex(long x,long y,BYTE i)
{
	if ((hDib==NULL)||(m_nColors==0)||
		(x<0)||(y<0)||(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return ;
	BYTE* iDst = GetBits();
	iDst[(m_bi.biHeight - y)*m_LineWidth + x]=i;
	return;
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPixelColor(long x,long y,COLORREF cr)
{
	SetPixelColor(x,y,RGB2RGBQUAD(cr));
	return;
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPixelColor(long x,long y,RGBQUAD c)
{
	if ((hDib==NULL)||(x<0)||(y<0)||
		(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return;
	if (m_nColors)
		SetPixelIndex(x,y,GetNearestIndex(c));
	else {
		BYTE* iDst = GetBits()+(m_bi.biHeight - y)*m_LineWidth + x*sizeof(RGBQUAD);
		*(RGBQUAD*)iDst = c;
	}
	return;
}
/////////////////////////////////////////////////////////////////////
BYTE CxDib::GetNearestIndex(RGBQUAD c)
{
	if ((hDib==NULL)||(m_nColors==0)) return 0;
	BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
	long distance=200000;
	BYTE i,j;
	long k,l;
	for(i=0,l=0;i<m_nColors;i++,l+=sizeof(RGBQUAD)){
		k = (iDst[l]-c.rgbBlue)*(iDst[l]-c.rgbBlue)+
			(iDst[l+1]-c.rgbGreen)*(iDst[l+1]-c.rgbGreen)+
			(iDst[l+2]-c.rgbRed)*(iDst[l+2]-c.rgbRed);
		if (k==0){
			j=i;
			break;
		}
		if (k<distance){
			distance=k;
			j=i;
		}
	}
	return j;
}
/////////////////////////////////////////////////////////////////////
#define  HSLMAX   240	/* H,L, and S vary over 0-HSLMAX */
#define  RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */
                        /* HSLMAX BEST IF DIVISIBLE BY 6 */
                        /* RGBMAX, HSLMAX must each fit in a byte. */
/* Hue is undefined if Saturation is 0 (grey-scale) */
/* This value determines where the Hue scrollbar is */
/* initially set for achromatic colors */
#define UNDEFINED (HSLMAX*2/3)
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::RGBtoHSL(RGBQUAD lRGBColor)
{
	BYTE R,G,B;					/* input RGB values */
	BYTE H,L,S;					/* output HSL values */
	BYTE cMax,cMin;				/* max and min RGB values */
	WORD Rdelta,Gdelta,Bdelta;	/* intermediate value: % of spread from max*/

	R = lRGBColor.rgbRed;	/* get R, G, and B out of DWORD */
	G = lRGBColor.rgbGreen;
	B = lRGBColor.rgbBlue;

	cMax = max( max(R,G), B);	/* calculate lightness */
	cMin = min( min(R,G), B);
	L = (((cMax+cMin)*HSLMAX)+RGBMAX)/(2*RGBMAX);

	if (cMax==cMin){			/* r=g=b --> achromatic case */
		S = 0;					/* saturation */
		H = UNDEFINED;			/* hue */
	} else {					/* chromatic case */
		if (L <= (HSLMAX/2))	/* saturation */
			S = (((cMax-cMin)*HSLMAX)+((cMax+cMin)/2))/(cMax+cMin);
		else
			S = (((cMax-cMin)*HSLMAX)+((2*RGBMAX-cMax-cMin)/2))/(2*RGBMAX-cMax-cMin);
		/* hue */
		Rdelta = ( ((cMax-R)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Gdelta = ( ((cMax-G)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Bdelta = ( ((cMax-B)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);

		if (R == cMax)
			H = Bdelta - Gdelta;
		else if (G == cMax)
			H = (HSLMAX/3) + Rdelta - Bdelta;
		else /* B == cMax */
			H = ((2*HSLMAX)/3) + Gdelta - Rdelta;

		if (H < 0) H += HSLMAX;
		if (H > HSLMAX) H -= HSLMAX;
	}
	RGBQUAD hsl={L,S,H,0};
	return hsl;
}
/////////////////////////////////////////////////////////////////////
WORD CxDib::HueToRGB(WORD n1,WORD n2,WORD hue)
{ 
	/* range check: note values passed add/subtract thirds of range */
	if (hue < 0) hue += HSLMAX;
	if (hue > HSLMAX) hue -= HSLMAX;

	/* return r,g, or b value from this tridrant */
	if (hue < (HSLMAX/6))
		return ( n1 + (((n2-n1)*hue+(HSLMAX/12))/(HSLMAX/6)) );
	if (hue < (HSLMAX/2))
		return ( n2 );
	if (hue < ((HSLMAX*2)/3))
		return ( n1 + (((n2-n1)*(((HSLMAX*2)/3)-hue)+(HSLMAX/12))/(HSLMAX/6))); 
	else
		return ( n1 );
} 
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::HSLtoRGB(COLORREF cHSLColor)
{
	return HSLtoRGB(RGB2RGBQUAD(cHSLColor));
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::HSLtoRGB(RGBQUAD lHSLColor)
{ 
	WORD hue,lum,sat;
	BYTE R,G,B;					/* RGB component values */
	WORD Magic1,Magic2;			/* calculated magic numbers (really!) */

	hue = lHSLColor.rgbRed;	/* get H, S, and L out of DWORD */
	sat = lHSLColor.rgbGreen;
	lum = lHSLColor.rgbBlue;

	if (sat == 0) {            /* achromatic case */
		R=G=B=(lum*RGBMAX)/HSLMAX;
	} else {                   /* chromatic case */
		/* set up magic numbers */
		if (lum <= (HSLMAX/2))
			Magic2 = (lum*(HSLMAX + sat) + (HSLMAX/2))/HSLMAX;
		else
			Magic2 = lum + sat - ((lum*sat) + (HSLMAX/2))/HSLMAX;
		Magic1 = 2*lum-Magic2;

		/* get RGB, change units from HSLMAX to RGBMAX */
		R = (HueToRGB(Magic1,Magic2,(WORD)(hue+(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
		G = (HueToRGB(Magic1,Magic2,hue)*RGBMAX + (HSLMAX/2)) / HSLMAX;
		B = (HueToRGB(Magic1,Magic2,(WORD)(hue-(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
	}
	RGBQUAD rgb={B,G,R,0};
	return rgb;
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::RGB2RGBQUAD(COLORREF cr)
{
	RGBQUAD c;
	c.rgbRed = GetRValue(cr);	/* get R, G, and B out of DWORD */
	c.rgbGreen = GetGValue(cr);
	c.rgbBlue = GetBValue(cr);
	c.rgbReserved=0;
	return c;
}
/////////////////////////////////////////////////////////////////////
COLORREF CxDib::RGBQUAD2RGB (RGBQUAD c)
{
	return RGB(c.rgbRed,c.rgbGreen,c.rgbBlue);
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetGrayPalette()
{
	if ((hDib==NULL)||(m_nColors==0)) return;
	RGBQUAD pal[256];
	RGBQUAD* ppal;
	BYTE* iDst;
	int ni;
	ppal=(RGBQUAD*)&pal[0];
	iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
	for (ni=0;ni<m_nColors;ni++){ pal[ni]=RGB2RGBQUAD(RGB(ni,ni,ni));}
	pal[0]=RGB2RGBQUAD(RGB(0,0,0));
	pal[m_nColors-1]=RGB2RGBQUAD(RGB(255,255,255));
	memcpy(iDst,ppal,GetPaletteSize());
	return;
}
/////////////////////////////////////////////////////////////////////
void CxDib::BlendPalette(COLORREF cr,long perc)
{
	if ((hDib==NULL)||(m_nColors==0)) return;
	BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
	long i,r,g,b;
	RGBQUAD* pPal=(RGBQUAD*)iDst;
	r = GetRValue(cr);
	g = GetGValue(cr);
	b = GetBValue(cr);
	if (perc>100) perc=100;
	for(i=0;i<m_nColors;i++){
		pPal[i].rgbBlue=(BYTE)((pPal[i].rgbBlue*(100-perc)+b*perc)/100);
		pPal[i].rgbGreen =(BYTE)((pPal[i].rgbGreen*(100-perc)+g*perc)/100);
		pPal[i].rgbRed =(BYTE)((pPal[i].rgbRed*(100-perc)+r*perc)/100);
	}
	return;
}
/////////////////////////////////////////////////////////////////////
long CxDib::WriteBMP(LPSTR bmpFileName)
{                         
	if ((*bmpFileName=='\0')||(hDib==0)) return 0;
	BITMAPFILEHEADER	hdr;
	HANDLE	hFile;
	DWORD	nByteWrite;

	hFile=CreateFile(			// open if exist ini file
		bmpFileName,				// pointer to name of the file 
		GENERIC_WRITE,			// access mode 
		0,						// share mode 
		NULL,					// pointer to security descriptor 
		TRUNCATE_EXISTING,		// how to create 
		FILE_ATTRIBUTE_NORMAL,	// file attributes 
		NULL				 	// handle to file with attributes to copy  
		);
	if (hFile==INVALID_HANDLE_VALUE) return 0;	//check if file exist

    /* Fill in the fields of the file header */
	hdr.bfType = BFT_BITMAP;
	hdr.bfSize = GetSize() + sizeof(BITMAPFILEHEADER);
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER)+
					m_bi.biSize + GetPaletteSize();

    // Write the file header
	WriteFile(			// write ini (sync mode <-> no overlapped)
		hFile,			// handle of file to write 
		(LPSTR) &hdr,	// address of buffer that contains data  
		sizeof(BITMAPFILEHEADER),	// number of bytes to write 
		&nByteWrite,	// address of number of bytes written 
		NULL	 		// address of structure for data 
		);

    // Write the DIB header and the bits
	WriteFile(			// write ini (sync mode <-> no overlapped)
		hFile,			// handle of file to write 
		(LPSTR) hDib,	// address of buffer that contains data  
		GetSize(),		// number of bytes to write 
		&nByteWrite,	// address of number of bytes written 
		NULL	 		// address of structure for data 
		);

	CloseHandle(hFile);		//free file handle
	return 1;
}
/////////////////////////////////////////////////////////////////////
long CxDib::GetSize()
{
	return m_bi.biSize + m_bi.biSizeImage + GetPaletteSize();
}
/////////////////////////////////////////////////////////////////////
BOOL CxDib::IsValid()
{
	return (hDib!=NULL);
}
/////////////////////////////////////////////////////////////////////
void CxDib::Clone(CxDib *src)
{
 	Create(src->GetWidth(),src->GetHeight(),src->GetBitCount());
 	if (hDib) memcpy(hDib,src->hDib,GetSize());
	return;
}
/////////////////////////////////////////////////////////////////////
void CxDib::Clear(BYTE bval)
{
	if (hDib) memset(GetBits(),bval,m_bi.biSizeImage);
}
/////////////////////////////////////////////////////////////////////













/////////////////////////////////////////////////////////////////////////////
// CxShadeButton
CxShadeButton::CxShadeButton()
{
	m_Border=1;				//draw 3D border
	m_FocusRectMargin=4;	//focus dotted rect margin
	m_TextColor=GetSysColor(COLOR_BTNTEXT); // default button text color
	m_button_down = m_tracking = false;
}
/////////////////////////////////////////////////////////////////////////////
CxShadeButton::~CxShadeButton()
{ }
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CxShadeButton, CButton)
	//{{AFX_MSG_MAP(CxShadeButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CxShadeButton message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CxShadeButton::OnEraseBkgnd(CDC* pDC) 
{
	return 1;	// doesn't erase the button background
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT (lpDrawItemStruct);
    //TRACE("* Captured: %08X\n", ::GetCapture());

    //Check if the button state in not in inconsistent mode...
    POINT mouse_position;
    if ((m_button_down) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position))){
		if (::WindowFromPoint(mouse_position) == m_hWnd){
			if ((GetState() & BST_PUSHED) != BST_PUSHED) {
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(TRUE);
				return;
			}
		} else {
			if ((GetState() & BST_PUSHED) == BST_PUSHED) {
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(FALSE);
				return;
			}
		}
	}
	
    //TRACE("* Drawing: %08x\n", lpDrawItemStruct->itemState);
	CString sCaption;
	TEXTMETRIC tm;

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	// get device context
	RECT r=lpDrawItemStruct->rcItem;					// context rectangle
	int cx = r.right  - r.left ;						// get width
	int cy = r.bottom - r.top  ;						// get height


	GetWindowText(sCaption);							// get button text
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT)); //get text font
	pDC->GetTextMetrics(&tm);							// get font metrics
	// get top-left corner to draw the text centered on the button
	int tx=(cx-tm.tmAveCharWidth*sCaption.GetLength())/2;
	int ty=(cy-tm.tmHeight)/2;

	// Select the correct skin 
	if (lpDrawItemStruct->itemState & ODS_DISABLED){	// DISABLED BUTTON
		if(!m_dDisabled.IsValid())
			 // no skin selected for disabled state -> standard button
			pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
		else // paint the skin
			m_dDisabled.Draw(pDC->GetSafeHdc(),0,0);
		// if needed, draw the standard 3D rectangular border
		if (m_Border) pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
		// paint the etched button text
		pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));
		pDC->ExtTextOut(tx+1,ty+1,ETO_CLIPPED,&r,sCaption,NULL);
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->ExtTextOut(tx,ty,ETO_CLIPPED,&r,sCaption,NULL);
	} else {
//---------------------------------------------------------------------------
		if (lpDrawItemStruct->itemState & ODS_SELECTED){ //SELECTED (DOWN) BUTTON
			if(!m_dDown.IsValid())
				 // no skin selected for selected state -> standard button
				pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
            else { // paint the skin
				m_dDown.Draw(pDC->GetSafeHdc(),1,1);
            }
			// if needed, draw the standard 3D rectangular border
			if (m_Border) pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
		} else {
//---------------------------------------------------------------------------
			if(!m_dNormal.IsValid())					// DEFAULT BUTTON
				 // no skin selected for normal state -> standard button
				pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
			else // paint the skin
                if ((m_tracking)&&(m_dOver.IsValid())){
					m_dOver.Draw(pDC->GetSafeHdc(),0,0);
				} else {
					m_dNormal.Draw(pDC->GetSafeHdc(),0,0);
				}
			// if needed, draw the standard 3D rectangular border
				if (m_Border){
					if (lpDrawItemStruct->itemState & ODS_DEFAULT){
						pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
						r.left   += 1;
						r.top    += 1;
						r.right  -= 1;
						r.bottom -= 1;
						pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
					} else {
						pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
					}
				}
//---------------------------------------------------------------------------
		}

		// paint the focus rect
		if ((lpDrawItemStruct->itemState & ODS_FOCUS)&&(m_FocusRectMargin>0)){
			r.left   += m_FocusRectMargin ;
			r.top    += m_FocusRectMargin ;
			r.right  -= m_FocusRectMargin ;
			r.bottom -= m_FocusRectMargin ;
			m_dh.Draw(pDC->GetSafeHdc(),r.left,r.top);
			m_dh.Draw(pDC->GetSafeHdc(),1+r.left,r.bottom);
			m_dv.Draw(pDC->GetSafeHdc(),r.left,1+r.top);
			m_dv.Draw(pDC->GetSafeHdc(),r.right,r.top);
		}

		// paint the enabled button text
		pDC->SetTextColor(m_TextColor);
		pDC->ExtTextOut(tx,ty,ETO_CLIPPED,&r,sCaption,NULL);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetShade(UINT shadeID,BYTE granularity,BYTE highlight,BYTE coloring,COLORREF color)
{
	long	sXSize,sYSize,bytes,j,i,k,h;
	BYTE	*iDst ,*posDst;
	
	RECT rect;
	GetWindowRect(&rect);
	sYSize=rect.bottom-rect.top;
	sXSize=rect.right-rect.left ;

	m_dh.Create(max(1,sXSize-2*m_FocusRectMargin),1,8);	//create the horizontal focus bitmap
	m_dv.Create(1,max(1,sYSize-2*m_FocusRectMargin),8);	//create the vertical focus bitmap

	m_dNormal.Create(sXSize,sYSize,8);					//create the default bitmap

	COLORREF hicr=GetSysColor(COLOR_BTNHIGHLIGHT);		//get the button base colors
	COLORREF midcr=GetSysColor(COLOR_BTNFACE);
	COLORREF locr=GetSysColor(COLOR_BTNSHADOW);
	long r,g,b;											//build the shaded palette
	for(i=0;i<129;i++){
		r=((128-i)*GetRValue(locr)+i*GetRValue(midcr))/128;
		g=((128-i)*GetGValue(locr)+i*GetGValue(midcr))/128;
		b=((128-i)*GetBValue(locr)+i*GetBValue(midcr))/128;
		m_dNormal.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
	}
	for(i=1;i<129;i++){
		r=((128-i)*GetRValue(midcr)+i*GetRValue(hicr))/128;
		g=((128-i)*GetGValue(midcr)+i*GetGValue(hicr))/128;
		b=((128-i)*GetBValue(midcr)+i*GetBValue(hicr))/128;
		m_dNormal.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
	}

	m_dNormal.BlendPalette(color,coloring);	//color the palette

	iDst=m_dh.GetBits();		//build the horiz. dotted focus bitmap
	j=(long)m_dh.GetWidth();
	for(i=0;i<j;i++){
//		iDst[i]=64+127*(i%2);	//soft
		iDst[i]=255*(i%2);		//hard
	}

	iDst=m_dv.GetBits();		//build the vert. dotted focus bitmap
	j=(long)m_dv.GetHeight();
	for(i=0;i<j;i++){
//		*iDst=64+127*(i%2);		//soft
		*iDst=255*(i%2);		//hard
		iDst+=4;
	}

	bytes = m_dNormal.GetLineWidth();
	iDst = m_dNormal.GetBits();
	posDst =iDst;
	long a,x,y,d,xs,idxmax,idxmin;

	int grainx2=RAND_MAX/(max(1,2*granularity));
	idxmax=255-granularity;
	idxmin=granularity;

	switch(shadeID){
//----------------------------------------------------
	case 8:	//SHS_METAL
		m_dNormal.Clear();
		// create the strokes
		k=40;	//stroke granularity
		for(a=0;a<200;a++){
			x=rand()/(RAND_MAX/sXSize); //stroke postion
			y=rand()/(RAND_MAX/sYSize);	//stroke position
			xs=rand()/(RAND_MAX/min(sXSize,sYSize))/2; //stroke lenght
			d=rand()/(RAND_MAX/k);	//stroke color
			for(i=0;i<xs;i++){
				if (((x-i)>0)&&((y+i)<sYSize))
					m_dNormal.SetPixelIndex(x-i,y+i,(BYTE)d);
				if (((x+i)<sXSize)&&((y-i)>0))
					m_dNormal.SetPixelIndex(sXSize-x+i,y-i,(BYTE)d);
			}
		}
		//blend strokes with SHS_DIAGONAL
		posDst =iDst;
		a=(idxmax-idxmin-k)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				d=posDst[j]+((a*i)/sYSize+(a*(sXSize-j))/sXSize);
				posDst[j]=(BYTE)d;
				posDst[j]+=rand()/grainx2;
			}
			posDst+=bytes;
		}

		break;
//----------------------------------------------------
	case 7:	// SHS_HARDBUMP
		//set horizontal bump
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		//set vertical bump
		d=min(16,sXSize/6);	//max edge=16
		a=sYSize*sYSize/4;
		posDst =iDst;
		for(i = 0; i < sYSize; i++) {
			y=i-sYSize/2;
			for(j = 0; j < sXSize; j++) {
				x=j-sXSize/2;
				xs=sXSize/2-d+(y*y*d)/a;
				if (x>xs) posDst[j]=idxmin+(BYTE)(((sXSize-j)*128)/d);
				if ((x+xs)<0) posDst[j]=idxmax-(BYTE)((j*128)/d);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 6: //SHS_SOFTBUMP
		for(i = 0; i < sYSize; i++) {
			h=(255*i/sYSize)-127;
			for(j = 0; j < sXSize; j++) {
				k=(255*(sXSize-j)/sXSize)-127;
				k=(h*(h*h)/128)/128+(k*(k*k)/128)/128;
				k=k*(128-granularity)/128+128;
				if (k<idxmin) k=idxmin;
				if (k>idxmax) k=idxmax;
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 5: // SHS_VBUMP
		for(j = 0; j < sXSize; j++) {
			k=(255*(sXSize-j)/sXSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	case 4: //SHS_HBUMP
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 1:	//SHS_DIAGSHADE
		a=(idxmax-idxmin)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)(idxmin+a*i/sYSize+a*(sXSize-j)/sXSize);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 2:	//SHS_HSHADE
		a=idxmax-idxmin;
		for(i = 0; i < sYSize; i++) {
			k=a*i/sYSize+idxmin;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 3:	//SHS_VSHADE:
		a=idxmax-idxmin;
		for(j = 0; j < sXSize; j++) {
			k=a*(sXSize-j)/sXSize+idxmin;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	default:	//SHS_NOISE
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=128+rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
	}
//----------------------------------------------------
	m_dDisabled.Clone(&m_dNormal);	//build the other bitmaps
	m_dOver.Clone(&m_dNormal);
	m_dOver.BlendPalette(GetSysColor(COLOR_BTNHIGHLIGHT),highlight);
	m_dDown.Clone(&m_dOver);
}
/////////////////////////////////////////////////////////////////////////////
COLORREF CxShadeButton::SetTextColor(COLORREF new_color)
{
	COLORREF tmp_color=m_TextColor;
	m_TextColor=new_color;
	return tmp_color;			//returns the previous color
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetToolTipText(CString s)
{
	if(m_tooltip.m_hWnd==NULL){
		if(m_tooltip.Create(this))	//first assignment
			if(m_tooltip.AddTool(this, (LPCTSTR)s))
				m_tooltip.Activate(1);
	} else {
		m_tooltip.UpdateTipText((LPCTSTR)s,this);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
// This function will create a MSG structure, fill it in a pass it to
// the ToolTip control, m_ttip.  Note that we ensure the point is in window
// coordinates (relative to the control's window).
	if(NULL != m_tooltip.m_hWnd){
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);

		m_tooltip.RelayEvent(&msg);
	}
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnLButtonDblClk
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: Used only to be forwarded as WM_LBUTTONDOWN message parameters
//Exceptions..: NONE
//------------
//Description :
//
//  > We do not care about doublelicks - handle this event
//    like an ordinary left-button-down event
//
//---------------------------------------------------------
void CxShadeButton::OnLButtonDblClk(UINT flags, CPoint point) 
{
    SendMessage(WM_LBUTTONDOWN, flags, MAKELPARAM(point.x, point.y));
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnLButtonDown
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle event when left button is pressed down
//
//---------------------------------------------------------
void CxShadeButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: down\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONDOWN,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are tracking this button, cancel it
    if (m_tracking) {
        TRACKMOUSEEVENT t = {
            sizeof(TRACKMOUSEEVENT),
            TME_CANCEL | TME_LEAVE,
            m_hWnd,
            0
        };
        if (::_TrackMouseEvent(&t)) {
            m_tracking = false;
        }
    }

    //Default-process the message
    m_button_down = true;
	CButton::OnLButtonDown(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnLButtonUp
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle event when left button is released (goes up)
//
//---------------------------------------------------------
void CxShadeButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: up\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONUP,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //Default-process the message
    m_button_down = false;
	CButton::OnLButtonUp(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnMouseMove
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle change of mouse position: see the comments in the
//    method for further info.
//
//---------------------------------------------------------
void CxShadeButton::OnMouseMove(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: Mouse\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are in capture mode, button has been pressed down
    //recently and not yet released - therefore check is we are
    //actually over the button or somewhere else. If the mouse
    //position changed considerably (e.g. we moved mouse pointer
    //from the button to some other place outside button area)
    //force the control to redraw
    //
    if ((m_button_down) && (::GetCapture() == m_hWnd)) {
	    POINT p2 = point;
        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        bool pressed = ((GetState() & BST_PUSHED) == BST_PUSHED);
        bool need_pressed = (mouse_wnd == m_hWnd);
        if (pressed != need_pressed) {
            //TRACE("* %08X Redraw\n", GetTickCount());
            SetState(need_pressed ? TRUE : FALSE);
            Invalidate();
        }
    } else {

	//Otherwise the button is released. That means we should
    //know when we leave its area - and so if we are not tracking
    //this mouse leave event yet, start now!
    //
        if (!m_tracking) {
            TRACKMOUSEEVENT t = {
                sizeof(TRACKMOUSEEVENT),
                TME_LEAVE,
                m_hWnd,
                0
            };
            if (::_TrackMouseEvent(&t)) {
                //TRACE("* Mouse enter\n");
                m_tracking = true;
                Invalidate();
            }
        }
    }

    //Forward this event to superclass
    CButton::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnMouseLeave
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NULL
//Parameters..: NOT USED
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle situation when mouse cursor leaves area of this
//    window (button). This event might be generated ONLY
//    if we explicitely call 'TrackMouseEvent'. This is
//    signalled by setting the m_tracking flag (see the assert
//    precondition) - in 'OnMouseMove' method
//
//  > When a mouse pointer leaves area of this button (i.e.
//    when this method is invoked), presumably the look of
//    the button changes (e.g. when hover/non-hover images are set)
//    and therefore we force the control to redraw.
//
//---------------------------------------------------------
LRESULT CxShadeButton::OnMouseLeave(WPARAM, LPARAM)
{
    ASSERT (m_tracking);
    //TRACE("* Mouse leave\n");
    m_tracking = false;
    Invalidate();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnKillFocus
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: See superclass documentation
//Exceptions..: NONE
//------------
//Description :
//
//  > If focus is killed during capture, we may no longer
//    have the exclusive access to user input and therefore
//    release it.
//
//  > Such a situation might happens when the user left-clicks
//    this button, keeps the button down and simultaneously
//    presses TAB key.
//
//---------------------------------------------------------
void CxShadeButton::OnKillFocus(CWnd *new_wnd)
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
        m_button_down = false;
    }
    CButton::OnKillFocus(new_wnd);
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnClicked
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: FALSE (do not stop in this handler - forward to parent)
//Parameters..: NONE
//Exceptions..: NONE
//------------
//Description :
//
//  > Keep consistency of attributes of this instance before
//    submitting click event to the parent.
//
//  > Currently NOT used. To use, umcomment line
//    "ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)" in message map
//    at the beginning of this file.
//
//---------------------------------------------------------
BOOL CxShadeButton::OnClicked() 
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
    }
    m_button_down = false;
    //Invalidate();
    return FALSE;
}
//EOF

