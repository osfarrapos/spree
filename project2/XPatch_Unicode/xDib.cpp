#include "stdafx.h"
#include "xDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CxDib::CxDib()
{
	hDib=NULL;

	memset(&m_bi,0,sizeof(BITMAPINFOHEADER));
    m_bi.biSize = sizeof(BITMAPINFOHEADER);

	m_LineWidth=0;
	m_nColors=0;
}

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

	//hDib = malloc(dwLen); // alloc memory block to store our bitmap
	hDib = new (HDIB[dwLen]); //fixes allocation problem under Win2k
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
	return iDst[(m_bi.biHeight - y - 1)*m_LineWidth + x];
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::GetPixelColor(long x,long y)
{
	RGBQUAD rgb={0,0,0,0};
	if ((hDib==NULL)||(x<0)||(y<0)||
		(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return rgb;
	if (m_nColors) return GetPaletteIndex(GetPixelIndex(x,y));
	else {
		BYTE* iDst = GetBits()+(m_bi.biHeight - y - 1)*m_LineWidth + x*sizeof(RGBQUAD);
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
	iDst[(m_bi.biHeight - y - 1)*m_LineWidth + x]=i;
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
		BYTE* iDst = GetBits()+(m_bi.biHeight - y - 1)*m_LineWidth + x*sizeof(RGBQUAD);
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
long CxDib::WriteBMP(LPTSTR bmpFileName)
{                         
	if ((*bmpFileName==_T('\0'))||(hDib==0)) return 0;
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
