//
//	Class:		CxDib
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 5.0
//				Visual C++ 6.0
//
//	Author:		Davide Pizzolato	ing.davide.pizzolato@libero.it
//
#ifndef _XDIB_H_
#define _XDIB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CxDib class
/////////////////////////////////////////////////////////////////////////////
#define HDIB HANDLE
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi))==sizeof(BITMAPINFOHEADER))
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define BFT_ICON   0x4349   // 'IC'
#define BFT_BITMAP 0x4d42   // 'BM'
#define BFT_CURSOR 0x5450   // 'PT'

class CxDib  
{
public:
	CxDib();
	virtual ~CxDib();

	HDIB Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount);
	void Clear(BYTE bval=0);
	void Clone(CxDib *src);
	BOOL IsValid();
	long GetSize();
	BYTE* GetBits();
	DWORD GetHeight() {return m_bi.biHeight;};
	DWORD GetWidth() {return m_bi.biWidth;};
	DWORD GetLineWidth() {return m_LineWidth;};
	WORD GetNumColors() {return m_nColors;};
	WORD GetBitCount() {return m_bi.biBitCount;};
	WORD GetPaletteSize();
	BYTE GetPixelIndex(long x,long y);
	RGBQUAD GetPaletteIndex(BYTE idx);
	RGBQUAD GetPixelColor(long x,long y);
	BYTE GetNearestIndex(RGBQUAD c);
	long Draw(HDC pDC, long xoffset, long yoffset);
	long Stretch(HDC pDC, long xoffset, long yoffset, long xsize, long ysize);
	void BlendPalette(COLORREF cr,long perc);
	void SetPaletteIndex(BYTE idx, BYTE r, BYTE g, BYTE b);
	void SetPaletteIndex(BYTE idx, RGBQUAD c);
	void SetPaletteIndex(BYTE idx, COLORREF cr);
	void SetPixelColor(long x,long y,RGBQUAD c);
	void SetPixelIndex(long x,long y,BYTE i);
	void SetPixelColor(long x,long y,COLORREF cr);
	void SetGrayPalette();
	long WriteBMP(LPTSTR bmpFileName);
	RGBQUAD HSLtoRGB(COLORREF cHSLColor);
	RGBQUAD RGB2RGBQUAD(COLORREF cr);
	COLORREF RGBQUAD2RGB (RGBQUAD c);
	RGBQUAD RGBtoHSL(RGBQUAD lRGBColor);
	RGBQUAD HSLtoRGB(RGBQUAD lHSLColor);
protected:
	HDIB hDib;
    BITMAPINFOHEADER    m_bi;
	DWORD m_LineWidth;
	WORD  m_nColors;
	bool IsWin30Dib();
	WORD HueToRGB(WORD n1,WORD n2,WORD hue);
};

#endif
