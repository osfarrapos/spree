	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIB.cpp
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CSize@WinHelper@@QAE@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CSize@WinHelper@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Width@CRect@WinHelper@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Height@CRect@WinHelper@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Set@CRect@WinHelper@@QAEXHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Unlock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetWidth@CDIB@GS@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHeight@CDIB@GS@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLineArray@CDIB@GS@@QAEABV?$CArray@PAUtagRGBQUAD@@@Container@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBitmap@CDIB@GS@@QAEPAUHBITMAP__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetHasAlphaChannel@CDIB@GS@@QAEX_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CCompatibleDC@GS@@QAE@PAUHDC__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CCompatibleDC@GS@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BCCompatibleDC@GS@@QAEPAUHDC__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCDIB@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CArray@PAUtagRGBQUAD@@@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAUtagRGBQUAD@@@Container@@QBEABQAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAUtagRGBQUAD@@@Container@@QAEAAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsNoMemOverlap@Container@@YAXPAPAUtagRGBQUAD@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsMemOverlap@Container@@YAXPAPAUtagRGBQUAD@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CDIB@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z			; GS::CDIB::CDIB
PUBLIC	?Initialise@CDIB@GS@@AAEXXZ			; GS::CDIB::Initialise
PUBLIC	??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@	; Container::CArray<tagRGBQUAD *>::`vftable'
PUBLIC	??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ; Container::CArray<tagRGBQUAD *>::`scalar deleting destructor'
PUBLIC	??_E?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ; Container::CArray<tagRGBQUAD *>::`vector deleting destructor'
PUBLIC	??_7CDIB@GS@@6B@				; GS::CDIB::`vftable'
PUBLIC	??_GCDIB@GS@@UAEPAXI@Z				; GS::CDIB::`scalar deleting destructor'
PUBLIC	??_ECDIB@GS@@UAEPAXI@Z				; GS::CDIB::`vector deleting destructor'
EXTRN	__imp__CreateDIBSection@24:NEAR
;	COMDAT ??_7CDIB@GS@@6B@
; File ../array.inl
CONST	SEGMENT
??_7CDIB@GS@@6B@ DD FLAT:??_ECDIB@GS@@UAEPAXI@Z		; GS::CDIB::`vftable'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT
??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ DD FLAT:??_E?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ; Container::CArray<tagRGBQUAD *>::`vftable'
CONST	ENDS
_TEXT	SEGMENT
_hdc$ = 8
_cx$ = 12
_cy$ = 16
??0CDIB@GS@@QAE@PAUHDC__@@HH@Z PROC NEAR		; GS::CDIB::CDIB

; 15   : {

	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

; 16   : 
; 17   : 	ZeroMemory( &m_bi, sizeof( m_bi ) );

	push	11					; 0000000bH
	xor	edx, edx
	lea	ebx, DWORD PTR [esi+12]
	mov	DWORD PTR [esi+56], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	pop	ecx
	xor	eax, eax
	mov	edi, ebx
	mov	DWORD PTR [esi+60], edx
	mov	DWORD PTR [esi+64], 1
	mov	DWORD PTR [esi+68], edx
	mov	DWORD PTR [esi+72], edx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CDIB@GS@@6B@ ; GS::CDIB::`vftable'
	rep stosd

; 18   : 	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
; 19   : 	m_bi.bmiHeader.biWidth = cx;
; 20   : 	m_bi.bmiHeader.biHeight = -cy;

	mov	eax, DWORD PTR _cy$[esp+8]

; 21   : 	m_bi.bmiHeader.biPlanes = 1;
; 22   : 	m_bi.bmiHeader.biBitCount = g_knBPP;
; 23   : 	m_bi.bmiHeader.biCompression = BI_RGB;

	mov	DWORD PTR [esi+28], edx
	mov	edi, eax

; 24   : 	m_bi.bmiHeader.biSizeImage = cx * (( cy * g_knBPP / 8 + 3 ) & ~ 3 );

	push	8
	neg	edi
	shl	eax, 5
	mov	DWORD PTR [esi+20], edi
	pop	edi
	cdq
	idiv	edi
	mov	ecx, DWORD PTR _cx$[esp+8]
	mov	DWORD PTR [ebx], 40			; 00000028H
	mov	DWORD PTR [esi+16], ecx
	mov	WORD PTR [esi+24], 1
	mov	WORD PTR [esi+26], 32			; 00000020H
	add	eax, 3
	and	al, -4					; fffffffcH
	imul	eax, ecx
	mov	DWORD PTR [esi+32], eax

; 25   : 
; 26   : 	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	xor	eax, eax
	push	eax
	lea	ecx, DWORD PTR [esi+8]
	push	eax
	push	ecx
	push	eax
	push	ebx
	push	DWORD PTR _hdc$[esp+28]
	call	DWORD PTR __imp__CreateDIBSection@24

; 27   : 	ASSERT( m_hBitmap );
; 28   : 
; 29   : 	Initialise();

	mov	ecx, esi
	mov	DWORD PTR [esi+4], eax
	call	?Initialise@CDIB@GS@@AAEXXZ		; GS::CDIB::Initialise

; 30   : }

	mov	eax, esi
	pop	edi
	pop	esi
	pop	ebx
	ret	12					; 0000000cH
??0CDIB@GS@@QAE@PAUHDC__@@HH@Z ENDP			; GS::CDIB::CDIB
_TEXT	ENDS
PUBLIC	??1CDIB@GS@@UAE@XZ				; GS::CDIB::~CDIB
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCDIB@GS@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_GCDIB@GS@@UAEPAXI@Z PROC NEAR			; GS::CDIB::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L55641
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L55641:
	mov	eax, esi
	pop	esi
	ret	4
??_GCDIB@GS@@UAEPAXI@Z ENDP				; GS::CDIB::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0CDIB@GS@@QAE@PAUHDC__@@HHHH@Z		; GS::CDIB::CDIB
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__BitBlt@36:NEAR
EXTRN	__imp__CreateCompatibleDC@4:NEAR
EXTRN	__imp__DeleteDC@4:NEAR
_TEXT	SEGMENT
_hdc$ = 8
_x$ = 12
_y$ = 16
_cx$ = 20
_cy$ = 24
_hdcMem$ = -4
_hOld$ = -8
??0CDIB@GS@@QAE@PAUHDC__@@HHHH@Z PROC NEAR		; GS::CDIB::CDIB

; 34   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ecx
	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

; 35   : 	ZeroMemory( &m_bi, sizeof( m_bi ) );

	push	11					; 0000000bH
	xor	ebx, ebx
	lea	edx, DWORD PTR [esi+12]
	mov	DWORD PTR [esi+56], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	pop	ecx
	xor	eax, eax
	mov	edi, edx
	mov	DWORD PTR [esi+60], ebx
	mov	DWORD PTR [esi+64], 1
	mov	DWORD PTR [esi+68], ebx
	mov	DWORD PTR [esi+72], ebx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CDIB@GS@@6B@ ; GS::CDIB::`vftable'
	rep stosd

; 36   : 	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
; 37   : 	m_bi.bmiHeader.biWidth = cx;
; 38   : 	m_bi.bmiHeader.biHeight = -cy;

	mov	eax, DWORD PTR _cy$[ebp]
	mov	DWORD PTR [edx], 40			; 00000028H
	mov	edx, eax

; 39   : 	m_bi.bmiHeader.biPlanes = 1;
; 40   : 	m_bi.bmiHeader.biBitCount = g_knBPP;
; 41   : 	m_bi.bmiHeader.biCompression = BI_RGB;
; 42   : 	m_bi.bmiHeader.biSizeImage = cx * (( cy * g_knBPP / 8 + 3 ) & ~ 3 );

	push	8
	neg	edx
	shl	eax, 5
	mov	DWORD PTR [esi+20], edx
	pop	edi
	cdq
	idiv	edi
	mov	ecx, DWORD PTR _cx$[ebp]

; 43   : 
; 44   : 	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	push	ebx
	push	ebx
	mov	DWORD PTR [esi+16], ecx
	mov	WORD PTR [esi+24], 1
	mov	WORD PTR [esi+26], 32			; 00000020H
	mov	DWORD PTR [esi+28], ebx
	add	eax, 3
	and	al, -4					; fffffffcH
	imul	eax, ecx
	mov	DWORD PTR [esi+32], eax
	lea	eax, DWORD PTR [esi+8]
	push	eax
	lea	eax, DWORD PTR [esi+12]
	push	ebx
	push	eax
	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__CreateDIBSection@24

; 45   : 	ASSERT( m_hBitmap );
; 46   : 
; 47   : 	Initialise();

	mov	ecx, esi
	mov	DWORD PTR [esi+4], eax
	call	?Initialise@CDIB@GS@@AAEXXZ		; GS::CDIB::Initialise

; 48   : 
; 49   : 	//
; 50   : 	//	Copy the DC into the DIB.
; 51   : 	CCompatibleDC hdcMem( hdc );

	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__CreateCompatibleDC@4

; 52   : 
; 53   : 	HGDIOBJ hOld = SelectObject( hdcMem, m_hBitmap );

	push	DWORD PTR [esi+4]
	mov	edi, DWORD PTR __imp__SelectObject@8
	mov	DWORD PTR _hdcMem$[ebp], eax
	push	eax
	call	edi

; 54   : 	VERIFY( BitBlt( hdcMem, 0, 0, cx, cy, hdc, x, y, SRCCOPY ) );

	push	13369376				; 00cc0020H
	mov	DWORD PTR _hOld$[ebp], eax
	push	DWORD PTR _y$[ebp]
	push	DWORD PTR _x$[ebp]
	push	DWORD PTR _hdc$[ebp]
	push	DWORD PTR _cy$[ebp]
	push	DWORD PTR _cx$[ebp]
	push	ebx
	push	ebx
	push	DWORD PTR _hdcMem$[ebp]
	call	DWORD PTR __imp__BitBlt@36

; 55   : 	//TextOut( hdcMem, 0, 0, "hello", 5 );
; 56   : 	SelectObject( hdcMem, hOld );

	push	DWORD PTR _hOld$[ebp]
	push	DWORD PTR _hdcMem$[ebp]
	call	edi

; 57   : }

	push	DWORD PTR _hdcMem$[ebp]
	call	DWORD PTR __imp__DeleteDC@4
	mov	eax, esi
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	20					; 00000014H
??0CDIB@GS@@QAE@PAUHDC__@@HHHH@Z ENDP			; GS::CDIB::CDIB
_TEXT	ENDS
PUBLIC	??0CDIB@GS@@QAE@PAUHBITMAP__@@@Z		; GS::CDIB::CDIB
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	__imp__GetObjectA@12:NEAR
_TEXT	SEGMENT
_hbm$ = 8
_bm$ = -40
_hdc$ = -12
_hdcMemDest$55667 = -8
_hdcMemSource$55668 = -4
_hOld$55669 = -16
_hOld2$55670 = 8
??0CDIB@GS@@QAE@PAUHBITMAP__@@@Z PROC NEAR		; GS::CDIB::CDIB

; 61   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H
	push	ebx
	push	esi

; 62   : 	BITMAP bm;
; 63   : 	GetObject( hbm, sizeof( bm ), &bm );

	lea	eax, DWORD PTR _bm$[ebp]
	push	edi
	push	eax
	mov	esi, ecx
	push	24					; 00000018H
	xor	ebx, ebx
	push	DWORD PTR _hbm$[ebp]
	mov	DWORD PTR [esi+56], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	mov	DWORD PTR [esi+60], ebx
	mov	DWORD PTR [esi+64], 1
	mov	DWORD PTR [esi+68], ebx
	mov	DWORD PTR [esi+72], ebx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CDIB@GS@@6B@ ; GS::CDIB::`vftable'
	call	DWORD PTR __imp__GetObjectA@12

; 64   : 
; 65   : 	ZeroMemory( &m_bi, sizeof( m_bi ) );

	lea	edx, DWORD PTR [esi+12]
	push	11					; 0000000bH
	pop	ecx
	xor	eax, eax
	mov	edi, edx

; 66   : 	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
; 67   : 	m_bi.bmiHeader.biWidth = bm.bmWidth;
; 68   : 	m_bi.bmiHeader.biHeight = -bm.bmHeight;
; 69   : 	m_bi.bmiHeader.biPlanes = 1;
; 70   : 	m_bi.bmiHeader.biBitCount = g_knBPP;
; 71   : 	m_bi.bmiHeader.biCompression = BI_RGB;
; 72   : 	m_bi.bmiHeader.biSizeImage = bm.bmWidth * (( bm.bmHeight * g_knBPP / 8 + 3 ) & ~ 3 );

	push	8
	rep stosd
	mov	eax, DWORD PTR _bm$[ebp+8]
	mov	DWORD PTR [edx], 40			; 00000028H
	mov	edx, eax
	pop	edi
	neg	edx
	shl	eax, 5
	mov	DWORD PTR [esi+20], edx
	mov	ecx, DWORD PTR _bm$[ebp+4]
	cdq
	idiv	edi

; 73   : 
; 74   : 	HDC hdc = GetDC( NULL );

	push	ebx
	mov	DWORD PTR [esi+16], ecx
	mov	WORD PTR [esi+24], 1
	mov	WORD PTR [esi+26], 32			; 00000020H
	mov	DWORD PTR [esi+28], ebx
	add	eax, 3
	and	al, -4					; fffffffcH
	imul	eax, ecx
	mov	DWORD PTR [esi+32], eax
	call	DWORD PTR __imp__GetDC@4
	mov	DWORD PTR _hdc$[ebp], eax

; 75   : 	ASSERT( hdc );
; 76   : 	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	push	ebx
	lea	eax, DWORD PTR [esi+8]
	push	ebx
	push	eax
	lea	eax, DWORD PTR [esi+12]
	push	ebx
	push	eax
	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__CreateDIBSection@24

; 77   : 
; 78   : 	ASSERT( m_hBitmap );
; 79   : 	Initialise();

	mov	ecx, esi
	mov	DWORD PTR [esi+4], eax
	call	?Initialise@CDIB@GS@@AAEXXZ		; GS::CDIB::Initialise

; 80   : 
; 81   : 	//
; 82   : 	//	Copy the DC into the DIB.
; 83   : 	{
; 84   : 		CCompatibleDC hdcMemDest( hdc );

	push	DWORD PTR _hdc$[ebp]
	mov	edi, DWORD PTR __imp__CreateCompatibleDC@4
	call	edi

; 85   : 		CCompatibleDC hdcMemSource( hdc );

	push	DWORD PTR _hdc$[ebp]
	mov	DWORD PTR _hdcMemDest$55667[ebp], eax
	call	edi

; 86   : 
; 87   : 		HGDIOBJ hOld = SelectObject( hdcMemDest, m_hBitmap );

	push	DWORD PTR [esi+4]
	mov	edi, DWORD PTR __imp__SelectObject@8
	mov	DWORD PTR _hdcMemSource$55668[ebp], eax
	push	DWORD PTR _hdcMemDest$55667[ebp]
	call	edi

; 88   : 		HGDIOBJ hOld2 = SelectObject( hdcMemSource, hbm );

	push	DWORD PTR _hbm$[ebp]
	mov	DWORD PTR _hOld$55669[ebp], eax
	push	DWORD PTR _hdcMemSource$55668[ebp]
	call	edi

; 89   : 		VERIFY( BitBlt( hdcMemDest, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemSource, 0, 0, SRCCOPY ) );

	push	13369376				; 00cc0020H
	push	ebx
	push	ebx
	mov	DWORD PTR _hOld2$55670[ebp], eax
	push	DWORD PTR _hdcMemSource$55668[ebp]
	push	DWORD PTR _bm$[ebp+8]
	push	DWORD PTR _bm$[ebp+4]
	push	ebx
	push	ebx
	push	DWORD PTR _hdcMemDest$55667[ebp]
	call	DWORD PTR __imp__BitBlt@36

; 90   : 		SelectObject( hdcMemDest, hOld );

	push	DWORD PTR _hOld$55669[ebp]
	push	DWORD PTR _hdcMemDest$55667[ebp]
	call	edi

; 91   : 		SelectObject( hdcMemSource, hOld2 );

	push	DWORD PTR _hOld2$55670[ebp]
	push	DWORD PTR _hdcMemSource$55668[ebp]
	call	edi

; 92   : 	}

	push	DWORD PTR _hdcMemSource$55668[ebp]
	mov	edi, DWORD PTR __imp__DeleteDC@4
	call	edi
	push	DWORD PTR _hdcMemDest$55667[ebp]
	call	edi

; 93   : 
; 94   : 	VERIFY( ReleaseDC( NULL, hdc ) );

	push	DWORD PTR _hdc$[ebp]
	push	ebx
	call	DWORD PTR __imp__ReleaseDC@8

; 95   : }

	mov	eax, esi
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	4
??0CDIB@GS@@QAE@PAUHBITMAP__@@@Z ENDP			; GS::CDIB::CDIB
_TEXT	ENDS
PUBLIC	??0CDIB@GS@@QAE@AAV01@HHHH@Z			; GS::CDIB::CDIB
_TEXT	SEGMENT
_dib$ = 8
_x$ = 12
_y$ = 16
_cx$ = 20
_cy$ = 24
_nLeft$ = 8
??0CDIB@GS@@QAE@AAV01@HHHH@Z PROC NEAR			; GS::CDIB::CDIB

; 99   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	mov	ebx, ecx
	push	esi
	push	edi

; 100  : 	ZeroMemory( &m_bi, sizeof( m_bi ) );

	lea	esi, DWORD PTR [ebx+12]
	push	11					; 0000000bH
	xor	edx, edx
	mov	DWORD PTR [ebx+56], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	pop	ecx
	xor	eax, eax
	mov	edi, esi
	mov	DWORD PTR [ebx+60], edx
	mov	DWORD PTR [ebx+64], 1
	mov	DWORD PTR [ebx+68], edx
	mov	DWORD PTR [ebx+72], edx
	mov	DWORD PTR [ebx], OFFSET FLAT:??_7CDIB@GS@@6B@ ; GS::CDIB::`vftable'
	rep stosd

; 101  : 	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
; 102  : 	m_bi.bmiHeader.biWidth = cx;
; 103  : 	m_bi.bmiHeader.biHeight = -cy;

	mov	eax, DWORD PTR _cy$[ebp]

; 104  : 	m_bi.bmiHeader.biPlanes = 1;
; 105  : 	m_bi.bmiHeader.biBitCount = g_knBPP;
; 106  : 	m_bi.bmiHeader.biCompression = BI_RGB;

	mov	DWORD PTR [ebx+28], edx
	mov	edi, eax

; 107  : 	m_bi.bmiHeader.biSizeImage = cx * (( cy * g_knBPP / 8 + 3 ) & ~ 3 );

	push	8
	neg	edi
	shl	eax, 5
	mov	DWORD PTR [ebx+20], edi
	pop	edi
	cdq
	idiv	edi
	mov	ecx, DWORD PTR _cx$[ebp]

; 108  : 
; 109  : 	HDC hdc = GetDC( NULL );

	xor	edi, edi
	push	edi
	mov	DWORD PTR [esi], 40			; 00000028H
	mov	DWORD PTR [ebx+16], ecx
	mov	WORD PTR [ebx+24], 1
	mov	WORD PTR [ebx+26], 32			; 00000020H
	add	eax, 3
	and	al, -4					; fffffffcH
	imul	eax, ecx
	mov	DWORD PTR [ebx+32], eax
	call	DWORD PTR __imp__GetDC@4

; 110  : 	ASSERT( hdc );
; 111  : 	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	push	edi
	lea	ecx, DWORD PTR [ebx+8]
	push	edi
	push	ecx
	push	edi
	push	esi
	push	eax
	call	DWORD PTR __imp__CreateDIBSection@24

; 112  : 
; 113  : 	ASSERT( m_hBitmap );
; 114  : 
; 115  : 	Initialise();

	mov	ecx, ebx
	mov	DWORD PTR [ebx+4], eax
	call	?Initialise@CDIB@GS@@AAEXXZ		; GS::CDIB::Initialise

; 116  : 
; 117  : 	if( dib.m_bHasAlpha )

	mov	eax, DWORD PTR _dib$[ebp]
	cmp	BYTE PTR [eax+89], 0
	je	SHORT $L56107

; 118  : 	{
; 119  : 		SetHasAlphaChannel( true );

	mov	BYTE PTR [ebx+89], 1
$L56107:

; 120  : 	}
; 121  : 
; 122  : 	if( dib.m_bTransparentColorSet )

	mov	cl, BYTE PTR [eax+88]
	test	cl, cl
	je	SHORT $L55688

; 123  : 	{
; 124  : 		m_bTransparentColorSet = dib.m_bTransparentColorSet;

	mov	BYTE PTR [ebx+88], cl

; 125  : 		m_crTransparent = dib.m_crTransparent;

	mov	ecx, DWORD PTR [eax+84]
	mov	DWORD PTR [ebx+84], ecx
$L55688:

; 126  : 	}
; 127  : 
; 128  : 	//
; 129  : 	//	Copy the bits over...
; 130  : 
; 131  : 	const UINT nLeft = max( x, 0 );

	mov	edx, DWORD PTR _x$[ebp]
	cmp	edx, edi
	mov	DWORD PTR _nLeft$[ebp], edx
	jg	SHORT $L56095
	mov	DWORD PTR _nLeft$[ebp], edi
$L56095:

; 132  : 	const UINT nTop = max( y, 0 );

	cmp	DWORD PTR _y$[ebp], edi
	jle	SHORT $L56097
	mov	edi, DWORD PTR _y$[ebp]
$L56097:

; 133  : 	const UINT nRight = min( x + cx, dib.GetWidth() );

	mov	esi, DWORD PTR _cx$[ebp]
	mov	ecx, DWORD PTR [eax+76]
	add	edx, esi
	cmp	edx, ecx
	mov	DWORD PTR 20+[ebp], edx
	jl	SHORT $L56114
	mov	DWORD PTR 20+[ebp], ecx
$L56114:

; 134  : 	const UINT nBottom = min( y + cy, dib.GetHeight() );

	mov	edx, DWORD PTR _cy$[ebp]
	mov	esi, DWORD PTR _y$[ebp]
	mov	ecx, DWORD PTR [eax+80]
	add	edx, esi
	cmp	edx, ecx
	jge	SHORT $L56122
	mov	ecx, edx
$L56122:

; 135  : 
; 136  : 	const CLineArray & arrDest = GetLineArray();
; 137  : 	const CLineArray & arrSrc = dib.GetLineArray();
; 138  : 
; 139  : 	int nDestLine = 0;
; 140  : 	for(  UINT nLine = nTop; nLine < nBottom; nLine++, nDestLine++ )

	cmp	edi, ecx
	jae	SHORT $L55699
	mov	edx, DWORD PTR [ebx+60]
	mov	DWORD PTR 16+[ebp], edx
	mov	edx, edi
	shl	edx, 2
	sub	ecx, edi
	mov	DWORD PTR 24+[ebp], ecx
$L55697:

; 141  : 	{
; 142  : 		DIB_PIXEL *pDest = arrDest[ nDestLine ];

	mov	ecx, DWORD PTR 16+[ebp]

; 143  : 		const DIB_PIXEL *pSrc = arrSrc[ nLine ];
; 144  : 		pSrc += nLeft;
; 145  : 
; 146  : 		for( UINT col = nLeft; col < nRight; col++, pDest++, pSrc++ )

	mov	esi, DWORD PTR _nLeft$[ebp]
	cmp	esi, DWORD PTR 20+[ebp]
	mov	ecx, DWORD PTR [ecx]
	jae	SHORT $L55698
	mov	edi, DWORD PTR [eax+60]
	mov	ecx, DWORD PTR 20+[ebp]
	sub	ecx, DWORD PTR _nLeft$[ebp]
	mov	edi, DWORD PTR [edi+edx]
	lea	esi, DWORD PTR [edi+esi*4]
	mov	edi, DWORD PTR 16+[ebp]
	mov	edi, DWORD PTR [edi]
	rep movsd
$L55698:
	add	DWORD PTR 16+[ebp], 4
	add	edx, 4
	dec	DWORD PTR 24+[ebp]
	jne	SHORT $L55697
$L55699:
	pop	edi

; 147  : 		{
; 148  : 			*pDest = *pSrc;
; 149  : 		}
; 150  : 	}
; 151  : }

	mov	eax, ebx
	pop	esi
	pop	ebx
	pop	ebp
	ret	20					; 00000014H
??0CDIB@GS@@QAE@AAV01@HHHH@Z ENDP			; GS::CDIB::CDIB
_TEXT	ENDS
PUBLIC	??0CDIB@GS@@QAE@PAUHICON__@@@Z			; GS::CDIB::CDIB
EXTRN	__imp__GetIconInfo@8:NEAR
_TEXT	SEGMENT
_hIcon$ = 8
_iconinfo$ = -60
_dibMask$ = -152
_bm$ = -40
_hdc$ = -8
_hdcMemDest$55718 = -4
_hdcMemSource$55719 = 8
_hOld$55720 = -16
_hOld2$55721 = -12
_u$ = 8
??0CDIB@GS@@QAE@PAUHICON__@@@Z PROC NEAR		; GS::CDIB::CDIB

; 155  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 152				; 00000098H
	push	ebx
	push	esi

; 156  : 	ICONINFO iconinfo;
; 157  : 	GetIconInfo( hIcon, &iconinfo );

	lea	eax, DWORD PTR _iconinfo$[ebp]
	push	edi
	mov	esi, ecx
	push	eax
	push	DWORD PTR _hIcon$[ebp]
	xor	ebx, ebx
	mov	DWORD PTR [esi+56], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	mov	DWORD PTR [esi+60], ebx
	mov	DWORD PTR [esi+64], 1
	mov	DWORD PTR [esi+68], ebx
	mov	DWORD PTR [esi+72], ebx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CDIB@GS@@6B@ ; GS::CDIB::`vftable'
	call	DWORD PTR __imp__GetIconInfo@8

; 158  : 	CDIB dibMask( iconinfo.hbmMask );

	push	DWORD PTR _iconinfo$[ebp+12]
	lea	ecx, DWORD PTR _dibMask$[ebp]
	call	??0CDIB@GS@@QAE@PAUHBITMAP__@@@Z	; GS::CDIB::CDIB

; 159  : 
; 160  : 	BITMAP bm;
; 161  : 	GetObject( iconinfo.hbmColor, sizeof( bm ), &bm );

	lea	eax, DWORD PTR _bm$[ebp]
	push	eax
	push	24					; 00000018H
	push	DWORD PTR _iconinfo$[ebp+16]
	call	DWORD PTR __imp__GetObjectA@12

; 162  : 
; 163  : 	ZeroMemory( &m_bi, sizeof( m_bi ) );

	lea	edx, DWORD PTR [esi+12]
	push	11					; 0000000bH
	pop	ecx
	xor	eax, eax
	mov	edi, edx

; 164  : 	m_bi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
; 165  : 	m_bi.bmiHeader.biWidth = bm.bmWidth;
; 166  : 	m_bi.bmiHeader.biHeight = -bm.bmHeight;
; 167  : 	m_bi.bmiHeader.biPlanes = 1;
; 168  : 	m_bi.bmiHeader.biBitCount = g_knBPP;
; 169  : 	m_bi.bmiHeader.biCompression = BI_RGB;
; 170  : 	m_bi.bmiHeader.biSizeImage = bm.bmWidth * (( bm.bmHeight * g_knBPP / 8 + 3 ) & ~ 3 );

	push	8
	rep stosd
	mov	eax, DWORD PTR _bm$[ebp+8]
	mov	DWORD PTR [edx], 40			; 00000028H
	mov	edx, eax
	pop	edi
	neg	edx
	shl	eax, 5
	mov	DWORD PTR [esi+20], edx
	mov	ecx, DWORD PTR _bm$[ebp+4]
	cdq
	idiv	edi

; 171  : 
; 172  : 	HDC hdc = GetDC( NULL );

	push	ebx
	mov	DWORD PTR [esi+16], ecx
	mov	WORD PTR [esi+24], 1
	mov	WORD PTR [esi+26], 32			; 00000020H
	mov	DWORD PTR [esi+28], ebx
	add	eax, 3
	and	al, -4					; fffffffcH
	imul	eax, ecx
	mov	DWORD PTR [esi+32], eax
	call	DWORD PTR __imp__GetDC@4
	mov	DWORD PTR _hdc$[ebp], eax

; 173  : 	ASSERT( hdc );
; 174  : 	m_hBitmap = CreateDIBSection( hdc, &m_bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );

	push	ebx
	lea	eax, DWORD PTR [esi+8]
	push	ebx
	push	eax
	lea	eax, DWORD PTR [esi+12]
	push	ebx
	push	eax
	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__CreateDIBSection@24

; 175  : 
; 176  : 	ASSERT( m_hBitmap );
; 177  : 	Initialise();

	mov	ecx, esi
	mov	DWORD PTR [esi+4], eax
	call	?Initialise@CDIB@GS@@AAEXXZ		; GS::CDIB::Initialise

; 178  : 
; 179  : 	//
; 180  : 	//	Copy the DC into the DIB.
; 181  : 	{
; 182  : 		CCompatibleDC hdcMemDest( hdc );

	push	DWORD PTR _hdc$[ebp]
	mov	edi, DWORD PTR __imp__CreateCompatibleDC@4
	call	edi

; 183  : 		CCompatibleDC hdcMemSource( hdc );

	push	DWORD PTR _hdc$[ebp]
	mov	DWORD PTR _hdcMemDest$55718[ebp], eax
	call	edi

; 184  : 
; 185  : 		HGDIOBJ hOld = SelectObject( hdcMemDest, m_hBitmap );

	push	DWORD PTR [esi+4]
	mov	edi, DWORD PTR __imp__SelectObject@8
	mov	DWORD PTR _hdcMemSource$55719[ebp], eax
	push	DWORD PTR _hdcMemDest$55718[ebp]
	call	edi

; 186  : 		HGDIOBJ hOld2 = SelectObject( hdcMemSource, iconinfo.hbmColor );

	push	DWORD PTR _iconinfo$[ebp+16]
	mov	DWORD PTR _hOld$55720[ebp], eax
	push	DWORD PTR _hdcMemSource$55719[ebp]
	call	edi

; 187  : 		VERIFY( BitBlt( hdcMemDest, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemSource, 0, 0, SRCCOPY ) );

	push	13369376				; 00cc0020H
	mov	DWORD PTR _hOld2$55721[ebp], eax
	push	ebx
	push	ebx
	push	DWORD PTR _hdcMemSource$55719[ebp]
	push	DWORD PTR _bm$[ebp+8]
	push	DWORD PTR _bm$[ebp+4]
	push	ebx
	push	ebx
	push	DWORD PTR _hdcMemDest$55718[ebp]
	call	DWORD PTR __imp__BitBlt@36

; 188  : 		SelectObject( hdcMemDest, hOld );

	push	DWORD PTR _hOld$55720[ebp]
	push	DWORD PTR _hdcMemDest$55718[ebp]
	call	edi

; 189  : 		SelectObject( hdcMemSource, hOld2 );

	push	DWORD PTR _hOld2$55721[ebp]
	push	DWORD PTR _hdcMemSource$55719[ebp]
	call	edi

; 190  : 	}

	push	DWORD PTR _hdcMemSource$55719[ebp]
	mov	edi, DWORD PTR __imp__DeleteDC@4
	call	edi
	push	DWORD PTR _hdcMemDest$55718[ebp]
	call	edi

; 191  : 
; 192  : 
; 193  : 	VERIFY( ReleaseDC( NULL, hdc ) );

	push	DWORD PTR _hdc$[ebp]
	push	ebx
	call	DWORD PTR __imp__ReleaseDC@8

; 194  : 
; 195  : 	const CLineArray & arrDest = GetLineArray();
; 196  : 	const UINT uSize = arrDest.GetSize();

	mov	edx, DWORD PTR [esi+72]

; 197  : 	const CLineArray & arrSrc = GetLineArray();
; 198  : 	for( register UINT u = 0; u < uSize; u++ )

	mov	DWORD PTR _u$[ebp], ebx
	cmp	edx, ebx
	jbe	SHORT $L55731
$L55729:
	mov	eax, DWORD PTR _u$[ebp]
	mov	edi, eax

; 199  : 	{
; 200  : 		DIB_PIXEL *pDest = arrDest[ u ];

	mov	eax, DWORD PTR [esi+60]
	shl	edi, 2

; 201  : 		DIB_PIXEL *pSrc = arrSrc[ u ];
; 202  : 		for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	cmp	DWORD PTR [esi+76], ebx
	mov	ecx, DWORD PTR [eax+edi]
	jle	SHORT $L55730
	lea	eax, DWORD PTR [ecx+3]
	mov	ecx, DWORD PTR [esi+76]
	mov	DWORD PTR -12+[ebp], eax
	mov	eax, DWORD PTR [esi+60]
	mov	eax, DWORD PTR [eax+edi]
	mov	edi, DWORD PTR -12+[ebp]
$L55735:

; 203  : 		{
; 204  :       if( pSrc->rgbBlue != 0 )

	cmp	BYTE PTR [eax], bl
	je	SHORT $L55736

; 205  : 			{
; 206  : 				pDest->rgbReserved = 255;

	or	BYTE PTR [edi], 255			; 000000ffH
$L55736:

; 201  : 		DIB_PIXEL *pSrc = arrSrc[ u ];
; 202  : 		for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	add	eax, 4
	add	edi, 4
	dec	ecx
	jne	SHORT $L55735
$L55730:

; 197  : 	const CLineArray & arrSrc = GetLineArray();
; 198  : 	for( register UINT u = 0; u < uSize; u++ )

	inc	DWORD PTR _u$[ebp]
	cmp	DWORD PTR _u$[ebp], edx
	jb	SHORT $L55729
$L55731:

; 207  : 			}
; 208  : 		}
; 209  : 	}
; 210  : 	SetHasAlphaChannel( true );
; 211  : 
; 212  : 
; 213  : }

	lea	ecx, DWORD PTR _dibMask$[ebp]
	mov	BYTE PTR [esi+89], 1
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	mov	eax, esi
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	4
??0CDIB@GS@@QAE@PAUHICON__@@@Z ENDP			; GS::CDIB::CDIB
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ; Container::CArray<tagRGBQUAD *>::RemoveAll
EXTRN	__imp__DeleteObject@4:NEAR
_TEXT	SEGMENT
??1CDIB@GS@@UAE@XZ PROC NEAR				; GS::CDIB::~CDIB

; 217  : {

	push	esi
	mov	esi, ecx

; 218  : 	VERIFY( DeleteObject( m_hBitmap ) );

	push	DWORD PTR [esi+4]
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CDIB@GS@@6B@ ; GS::CDIB::`vftable'
	call	DWORD PTR __imp__DeleteObject@4

; 219  : }

	lea	ecx, DWORD PTR [esi+56]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	call	?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ; Container::CArray<tagRGBQUAD *>::RemoveAll
	pop	esi
	ret	0
??1CDIB@GS@@UAE@XZ ENDP					; GS::CDIB::~CDIB
_TEXT	ENDS
PUBLIC	?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z ; Container::CArray<tagRGBQUAD *>::SetSize
_TEXT	SEGMENT
?Initialise@CDIB@GS@@AAEXXZ PROC NEAR			; GS::CDIB::Initialise

; 228  : {

	push	esi
	mov	esi, ecx

; 229  : 	m_size.cy = abs( m_bi.bmiHeader.biHeight );

	mov	eax, DWORD PTR [esi+20]

; 230  : 	m_size.cx = m_bi.bmiHeader.biWidth;

	mov	ecx, DWORD PTR [esi+16]
	cdq
	xor	eax, edx
	mov	DWORD PTR [esi+76], ecx
	sub	eax, edx

; 231  : 
; 232  : 	m_arrLine.SetSize( GetHeight() );

	lea	ecx, DWORD PTR [esi+56]
	push	eax
	mov	DWORD PTR [esi+80], eax
	call	?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z ; Container::CArray<tagRGBQUAD *>::SetSize

; 233  : 
; 234  : 	for( int i= GetHeight() - 1; i >=  0; i--)

	mov	ecx, DWORD PTR [esi+80]
	dec	ecx
	js	SHORT $L55749
	mov	eax, DWORD PTR [esi+60]
	push	ebx
	push	edi
	mov	ebx, DWORD PTR [esi+8]
	lea	edi, DWORD PTR [eax+ecx*4]
	mov	eax, DWORD PTR [esi+76]
	mov	edx, eax
	imul	eax, ecx
	neg	edx
	shl	edx, 2
	lea	eax, DWORD PTR [ebx+eax*4]
	inc	ecx
$L55747:

; 235  : 	{
; 236  : 		m_arrLine[i] = m_pBits + i * GetWidth();

	mov	DWORD PTR [edi], eax
	add	eax, edx
	sub	edi, 4
	dec	ecx
	jne	SHORT $L55747
	pop	edi
	pop	ebx
$L55749:

; 237  : 	}
; 238  : 
; 239  : 	m_crTransparent = 0;

	xor	eax, eax
	mov	DWORD PTR [esi+84], eax

; 240  : 	m_bTransparentColorSet = false;

	mov	BYTE PTR [esi+88], al

; 241  : 	m_bHasAlpha = false;

	mov	BYTE PTR [esi+89], al
	pop	esi

; 242  : }

	ret	0
?Initialise@CDIB@GS@@AAEXXZ ENDP			; GS::CDIB::Initialise
_TEXT	ENDS
PUBLIC	?Draw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z		; GS::CDIB::Draw
EXTRN	?DrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHK@Z:NEAR ; GS::CDIB::DrawTransparent
EXTRN	?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z:NEAR ; GS::CDIB::DrawWithAlphaChannel
EXTRN	__imp__SetDIBitsToDevice@48:NEAR
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
?Draw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z PROC NEAR		; GS::CDIB::Draw

; 246  : {

	push	ebp
	mov	ebp, esp

; 247  : 	if( m_bTransparentColorSet )

	xor	edx, edx
	cmp	BYTE PTR [ecx+88], dl
	je	SHORT $L55756

; 248  : 	{
; 249  : 		return DrawTransparent( hdc, nX, nY, m_crTransparent );

	push	DWORD PTR [ecx+84]
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?DrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHK@Z ; GS::CDIB::DrawTransparent
	jmp	SHORT $L55760
$L55756:

; 250  : 	}
; 251  : 	else if( m_bHasAlpha )

	cmp	BYTE PTR [ecx+89], dl
	je	SHORT $L55758

; 252  : 	{
; 253  : 		return DrawWithAlphaChannel( hdc, nX, nY );

	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ; GS::CDIB::DrawWithAlphaChannel
	jmp	SHORT $L55760
$L55758:

; 254  : 	}	
; 255  : 	else if( ::SetDIBitsToDevice( hdc, nX, nY, GetWidth(), GetHeight(), 0, 0, 0, GetHeight(), m_pBits, &m_bi, DIB_RGB_COLORS ) != GDI_ERROR )

	mov	eax, DWORD PTR [ecx+80]
	push	esi
	push	edi
	lea	edi, DWORD PTR [ecx+12]
	push	edx
	mov	esi, DWORD PTR [ecx+76]
	push	edi
	push	DWORD PTR [ecx+8]
	push	eax
	push	edx
	push	edx
	push	edx
	push	eax
	push	esi
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__SetDIBitsToDevice@48
	cmp	eax, -1
	pop	edi
	setne	al
	pop	esi
$L55760:

; 256  : 	{
; 257  : 		return true;
; 258  : 	}
; 259  : 	return false;
; 260  : }

	pop	ebp
	ret	12					; 0000000cH
?Draw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ENDP			; GS::CDIB::Draw
_TEXT	ENDS
PUBLIC	?BasicDraw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z		; GS::CDIB::BasicDraw
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
?BasicDraw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z PROC NEAR	; GS::CDIB::BasicDraw

; 265  : 	return ::SetDIBitsToDevice( hdc, nX, nY, GetWidth(), GetHeight(), 0, 0, 0, GetHeight(), m_pBits, &m_bi, DIB_RGB_COLORS ) != GDI_ERROR; 

	mov	eax, DWORD PTR [ecx+80]
	push	edi
	lea	edi, DWORD PTR [ecx+12]
	push	0
	mov	edx, DWORD PTR [ecx+76]
	push	edi
	push	DWORD PTR [ecx+8]
	push	eax
	push	0
	push	0
	push	0
	push	eax
	push	edx
	push	DWORD PTR _nY$[esp+36]
	push	DWORD PTR _nX$[esp+40]
	push	DWORD PTR _hdc$[esp+44]
	call	DWORD PTR __imp__SetDIBitsToDevice@48
	xor	ecx, ecx
	cmp	eax, -1
	setne	cl
	mov	al, cl
	pop	edi

; 266  : }

	ret	12					; 0000000cH
?BasicDraw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ENDP		; GS::CDIB::BasicDraw
_TEXT	ENDS
PUBLIC	?StretchAlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHHHE@Z ; GS::CDIB::StretchAlphaBlend
EXTRN	__imp__GdiFlush@0:NEAR
EXTRN	?AlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHE@Z:NEAR	; GS::CDIB::AlphaBlend
EXTRN	__imp__StretchDIBits@52:NEAR
EXTRN	__imp__SetStretchBltMode@8:NEAR
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
_nCX$ = 20
_nCY$ = 24
_nAlpha$ = 28
_dib$ = -96
_hOld$ = -4
?StretchAlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHHHE@Z PROC NEAR ; GS::CDIB::StretchAlphaBlend

; 271  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 96					; 00000060H
	push	ebx
	push	esi
	push	edi
	mov	esi, ecx

; 272  : 	CDIB dib( hdc, nCX, nCY );

	push	DWORD PTR _nCY$[ebp]
	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR _nCX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z		; GS::CDIB::CDIB

; 273  : 	CCompatibleDC hdcMem( hdc );

	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__CreateCompatibleDC@4
	mov	ebx, eax

; 274  : 	SetStretchBltMode( hdcMem, STRETCH_HALFTONE );

	push	4
	push	ebx
	call	DWORD PTR __imp__SetStretchBltMode@8

; 275  : 
; 276  : 	HGDIOBJ hOld = SelectObject( hdcMem, dib.GetBitmap() );

	call	DWORD PTR __imp__GdiFlush@0
	push	DWORD PTR _dib$[ebp+4]
	mov	edi, DWORD PTR __imp__SelectObject@8
	push	ebx
	call	edi
	mov	DWORD PTR _hOld$[ebp], eax

; 277  : 	::StretchDIBits( hdcMem,
; 278  :               0,
; 279  :               0,
; 280  :               nCX,
; 281  :               nCY,
; 282  :               0,
; 283  :               0,
; 284  :               GetWidth(),
; 285  :               GetHeight(),
; 286  :               m_pBits,
; 287  :               (BITMAPINFO *) &m_bi,
; 288  :               DIB_RGB_COLORS,
; 289  :               SRCCOPY );

	xor	eax, eax
	push	13369376				; 00cc0020H
	lea	ecx, DWORD PTR [esi+12]
	push	eax
	push	ecx
	push	DWORD PTR [esi+8]
	push	DWORD PTR [esi+80]
	push	DWORD PTR [esi+76]
	push	eax
	push	eax
	push	DWORD PTR _nCY$[ebp]
	push	DWORD PTR _nCX$[ebp]
	push	eax
	push	eax
	push	ebx
	call	DWORD PTR __imp__StretchDIBits@52

; 290  : 
; 291  : 	SelectObject( hdcMem, hOld );

	push	DWORD PTR _hOld$[ebp]
	push	ebx
	call	edi

; 292  : 
; 293  : 	dib.AlphaBlend( hdc, nX, nY, nAlpha );

	push	DWORD PTR _nAlpha$[ebp]
	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?AlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHE@Z ; GS::CDIB::AlphaBlend

; 294  : }

	push	ebx
	call	DWORD PTR __imp__DeleteDC@4
	lea	ecx, DWORD PTR _dib$[ebp]
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	24					; 00000018H
?StretchAlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHHHE@Z ENDP	; GS::CDIB::StretchAlphaBlend
_TEXT	ENDS
PUBLIC	?StretchDrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHHHK@Z ; GS::CDIB::StretchDrawTransparent
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
_nCX$ = 20
_nCY$ = 24
_crTransparent$ = 28
_dib$ = -96
_hOld$ = -4
$T56296 = 11
?StretchDrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHHHK@Z PROC NEAR ; GS::CDIB::StretchDrawTransparent

; 298  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 96					; 00000060H
	push	ebx
	push	esi
	push	edi
	mov	esi, ecx

; 299  : 	CDIB dib( hdc, nCX, nCY );

	push	DWORD PTR _nCY$[ebp]
	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR _nCX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z		; GS::CDIB::CDIB

; 300  : 	CCompatibleDC hdcMem( hdc );

	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__CreateCompatibleDC@4
	mov	ebx, eax

; 301  : 
; 302  : 	HGDIOBJ hOld = SelectObject( hdcMem, dib.GetBitmap() );

	call	DWORD PTR __imp__GdiFlush@0
	push	DWORD PTR _dib$[ebp+4]
	mov	edi, DWORD PTR __imp__SelectObject@8
	push	ebx
	call	edi

; 303  : 	SetStretchBltMode( hdcMem, STRETCH_HALFTONE );

	push	4
	push	ebx
	mov	DWORD PTR _hOld$[ebp], eax
	call	DWORD PTR __imp__SetStretchBltMode@8

; 304  : 	::StretchDIBits( hdcMem,
; 305  :               0,                        // Destination x
; 306  :               0,                        // Destination y
; 307  :               nCX,	                // Destination width
; 308  :               nCY,                // Destination height
; 309  :               0,                        // Source x
; 310  :               0,                        // Source y
; 311  :               GetWidth(),									// Source width
; 312  :               GetHeight(),	  						// Source height
; 313  :               m_pBits,								// Pointer to bits
; 314  :               (BITMAPINFO *) &m_bi,    // BITMAPINFO
; 315  :               DIB_RGB_COLORS,           // Options
; 316  :               SRCCOPY );

	xor	eax, eax
	push	13369376				; 00cc0020H
	lea	ecx, DWORD PTR [esi+12]
	push	eax
	push	ecx
	push	DWORD PTR [esi+8]
	push	DWORD PTR [esi+80]
	push	DWORD PTR [esi+76]
	push	eax
	push	eax
	push	DWORD PTR _nCY$[ebp]
	push	DWORD PTR _nCX$[ebp]
	push	eax
	push	eax
	push	ebx
	call	DWORD PTR __imp__StretchDIBits@52

; 317  : 
; 318  : 	SelectObject( hdcMem, hOld );

	push	DWORD PTR _hOld$[ebp]
	push	ebx
	call	edi

; 319  : 
; 320  : 	return dib.DrawTransparent( hdc, nX, nY, crTransparent );

	push	DWORD PTR _crTransparent$[ebp]
	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?DrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHK@Z ; GS::CDIB::DrawTransparent
	push	ebx
	mov	BYTE PTR $T56296[ebp], al
	call	DWORD PTR __imp__DeleteDC@4
	lea	ecx, DWORD PTR _dib$[ebp]
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	mov	al, BYTE PTR $T56296[ebp]
	pop	edi
	pop	esi
	pop	ebx

; 321  : }

	leave
	ret	24					; 00000018H
?StretchDrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHHHK@Z ENDP ; GS::CDIB::StretchDrawTransparent
_TEXT	ENDS
PUBLIC	?AddImage@CDIB@GS@@QAEXIIPAV12@@Z		; GS::CDIB::AddImage
_TEXT	SEGMENT
_nLeft$ = 8
_nTop$ = 12
_pDib$ = 16
_uDibWidth$ = -12
_bRed$55807 = 19
_bGreen$55808 = -1
?AddImage@CDIB@GS@@QAEXIIPAV12@@Z PROC NEAR		; GS::CDIB::AddImage

; 325  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH

; 326  : 	const CLineArray & arrThis = GetLineArray();
; 327  : 	const CLineArray & arr = pDib->GetLineArray();

	mov	eax, DWORD PTR _pDib$[ebp]
	push	ebx
	push	esi
	push	edi

; 328  : 
; 329  : 	const UINT uDibHeight = pDib->GetHeight();
; 330  : 	const UINT uDibWidth = pDib->GetWidth();

	mov	edx, DWORD PTR [eax+76]

; 331  : 
; 332  : 	if( pDib->m_bTransparentColorSet )

	cmp	BYTE PTR [eax+88], 0
	mov	esi, DWORD PTR [eax+80]
	mov	DWORD PTR _uDibWidth$[ebp], edx
	je	SHORT $L55806

; 333  : 	{
; 334  : 		BYTE bRed, bGreen, bBlue;
; 335  : 		bRed = GetRValue( pDib->m_crTransparent );

	mov	dl, BYTE PTR [eax+84]

; 336  : 		bGreen = GetGValue( pDib->m_crTransparent );
; 337  : 		bBlue = GetBValue( pDib->m_crTransparent );

	mov	ebx, DWORD PTR [eax+84]
	mov	BYTE PTR _bRed$55807[ebp], dl
	xor	edx, edx
	mov	dl, BYTE PTR [eax+85]
	shr	ebx, 16					; 00000010H

; 338  : 
; 339  : 		for( UINT uRow = 0; uRow < uDibHeight; uRow++ )

	test	esi, esi
	mov	BYTE PTR _bGreen$55808[ebp], dl
	jbe	$L55834
	mov	edi, DWORD PTR [eax+60]
	mov	eax, DWORD PTR [ecx+60]
	mov	ecx, DWORD PTR _nTop$[ebp]
	mov	DWORD PTR -8+[ebp], esi
	lea	eax, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR 12+[ebp], eax
$L55815:

; 340  : 		{
; 341  : 			const DIB_PIXEL *pSourceLine = (RGBQUAD *)arr[ uRow ];
; 342  : 			DIB_PIXEL *pDestLine = (RGBQUAD *)arrThis[ uRow + nTop ];

	mov	eax, DWORD PTR 12+[ebp]

; 343  : 			for( UINT uCol = 0; uCol < uDibWidth; uCol++ )

	mov	esi, DWORD PTR _uDibWidth$[ebp]
	mov	edx, DWORD PTR [edi]
	mov	ecx, DWORD PTR [eax]
	test	esi, esi
	jbe	SHORT $L55816
	mov	eax, DWORD PTR _nLeft$[ebp]
	lea	ecx, DWORD PTR [ecx+eax*4]
$L55823:

; 344  : 			{
; 345  : 				const DIB_PIXEL *pSource = pSourceLine + uCol;
; 346  : 				RGBQUAD *pDest = pDestLine + (uCol + nLeft);
; 347  : 				if( pSource->rgbRed != bRed || pSource->rgbGreen != bGreen || pSource->rgbBlue != bBlue )

	mov	al, BYTE PTR _bRed$55807[ebp]
	cmp	BYTE PTR [edx+2], al
	jne	SHORT $L55829
	mov	al, BYTE PTR _bGreen$55808[ebp]
	cmp	BYTE PTR [edx+1], al
	jne	SHORT $L55829
	cmp	BYTE PTR [edx], bl
	je	SHORT $L55824
$L55829:

; 348  : 				{
; 349  : 					*pDest = *pSource;

	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR [ecx], eax
$L55824:
	add	edx, 4
	add	ecx, 4
	dec	esi
	jne	SHORT $L55823
$L55816:

; 338  : 
; 339  : 		for( UINT uRow = 0; uRow < uDibHeight; uRow++ )

	add	DWORD PTR 12+[ebp], 4
	add	edi, 4
	dec	DWORD PTR -8+[ebp]
	jne	SHORT $L55815

; 350  : 				}
; 351  : 			}
; 352  : 		}
; 353  : 	}
; 354  : 	else

	jmp	SHORT $L55834
$L55806:

; 355  : 	{
; 356  : 		for( UINT uRow = 0; uRow < uDibHeight; uRow++ )

	test	esi, esi
	jbe	SHORT $L55834
	mov	ecx, DWORD PTR [ecx+60]
	mov	edx, DWORD PTR _nTop$[ebp]
	mov	eax, DWORD PTR [eax+60]
	mov	DWORD PTR 12+[ebp], esi
	lea	edx, DWORD PTR [ecx+edx*4]
$L55832:

; 357  : 		{
; 358  : 			const DIB_PIXEL *pSourceLine = (RGBQUAD *)arr[ uRow ];
; 359  : 			DIB_PIXEL *pDestLine = (RGBQUAD *)arrThis[ uRow + nTop ];
; 360  : 			for( UINT uCol = 0; uCol < uDibWidth; uCol++ )

	mov	ebx, DWORD PTR _uDibWidth$[ebp]
	mov	edi, DWORD PTR [eax]
	mov	ecx, DWORD PTR [edx]
	test	ebx, ebx
	jbe	SHORT $L55833
	mov	esi, DWORD PTR _nLeft$[ebp]
	lea	ecx, DWORD PTR [ecx+esi*4]
$L55840:

; 361  : 			{
; 362  : 				const DIB_PIXEL *pSource = pSourceLine + uCol;
; 363  : 				RGBQUAD *pDest = pDestLine + (uCol + nLeft);
; 364  : 				*pDest = *pSource;

	mov	esi, DWORD PTR [edi]
	add	edi, 4
	mov	DWORD PTR [ecx], esi
	add	ecx, 4
	dec	ebx
	jne	SHORT $L55840
$L55833:

; 355  : 	{
; 356  : 		for( UINT uRow = 0; uRow < uDibHeight; uRow++ )

	add	eax, 4
	add	edx, 4
	dec	DWORD PTR 12+[ebp]
	jne	SHORT $L55832
$L55834:
	pop	edi
	pop	esi
	pop	ebx

; 365  : 			}
; 366  : 		}
; 367  : 	}
; 368  : }

	leave
	ret	12					; 0000000cH
?AddImage@CDIB@GS@@QAEXIIPAV12@@Z ENDP			; GS::CDIB::AddImage
_TEXT	ENDS
PUBLIC	?Draw@CDIB@GS@@QAE_NPAUHDC__@@HHHH@Z		; GS::CDIB::Draw
EXTRN	?CreateResized@CDIB@GS@@QBEPAV12@HH@Z:NEAR	; GS::CDIB::CreateResized
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
_nRight$ = 20
_nBottom$ = 24
?Draw@CDIB@GS@@QAE_NPAUHDC__@@HHHH@Z PROC NEAR		; GS::CDIB::Draw

; 372  : {

	push	ebp
	mov	ebp, esp
	push	ebx

; 373  : 	const int nWidth = nRight - nX;
; 374  : 	const int nHeight = nBottom - nY;

	mov	ebx, DWORD PTR _nBottom$[ebp]
	sub	ebx, DWORD PTR _nY$[ebp]
	push	esi
	push	edi
	mov	edi, DWORD PTR _nRight$[ebp]
	sub	edi, DWORD PTR _nX$[ebp]
	mov	esi, ecx

; 375  : 	if( nWidth && nHeight )

	test	edi, edi
	je	$L55868
	test	ebx, ebx
	je	$L55868

; 376  : 	{
; 377  : 
; 378  : 		SetStretchBltMode( hdc, STRETCH_HALFTONE );

	push	4
	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__SetStretchBltMode@8

; 379  : 		if( m_bTransparentColorSet )

	cmp	BYTE PTR [esi+88], 0
	je	SHORT $L55856

; 380  : 		{
; 381  : 			if( nWidth != GetWidth() || nHeight != GetHeight() )

	cmp	edi, DWORD PTR [esi+76]
	jne	SHORT $L55858
	cmp	ebx, DWORD PTR [esi+80]
	jne	SHORT $L55858

; 384  : 			}
; 385  : 			return DrawTransparent( hdc, nX, nY, m_crTransparent );

	push	DWORD PTR [esi+84]
	mov	ecx, esi
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?DrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHK@Z ; GS::CDIB::DrawTransparent
	jmp	$L55852
$L55858:

; 382  : 			{
; 383  : 				return StretchDrawTransparent( hdc, nX, nY, nWidth, nHeight, m_crTransparent );

	push	DWORD PTR [esi+84]
	mov	ecx, esi
	push	ebx
	push	edi
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?StretchDrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHHHK@Z ; GS::CDIB::StretchDrawTransparent
	jmp	$L55852
$L55856:

; 386  : 		}
; 387  : 		else if( m_bHasAlpha )

	cmp	BYTE PTR [esi+89], 0
	je	SHORT $L55860

; 388  : 		{
; 389  : 			if( nWidth != GetWidth() || nHeight != GetHeight() )

	cmp	edi, DWORD PTR [esi+76]
	jne	SHORT $L55862
	cmp	ebx, DWORD PTR [esi+80]
	jne	SHORT $L55862

; 395  : 			}
; 396  : 			return DrawWithAlphaChannel( hdc, nX, nY );

	push	DWORD PTR _nY$[ebp]
	mov	ecx, esi
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ; GS::CDIB::DrawWithAlphaChannel
	jmp	SHORT $L55852
$L55862:

; 390  : 			{
; 391  : 				CDIB *pDib = CreateResized( nRight - nX, nBottom - nY );

	push	ebx
	push	edi
	mov	ecx, esi
	call	?CreateResized@CDIB@GS@@QBEPAV12@HH@Z	; GS::CDIB::CreateResized

; 392  : 				pDib->DrawWithAlphaChannel( hdc, nX, nY );

	push	DWORD PTR _nY$[ebp]
	mov	esi, eax
	mov	ecx, esi
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ; GS::CDIB::DrawWithAlphaChannel

; 393  : 				delete pDib;

	test	esi, esi
	je	SHORT $L56365
	mov	eax, DWORD PTR [esi]
	push	1
	mov	ecx, esi
	call	DWORD PTR [eax]

; 394  : 				return true;

	jmp	SHORT $L56365
$L55860:

; 397  : 		}	
; 398  : 		else if( ::StretchDIBits(hdc, nX, nY, nWidth, nHeight, 0, 0, GetWidth(), GetHeight(), m_pBits, &m_bi, DIB_RGB_COLORS, SRCCOPY ) != GDI_ERROR )

	mov	eax, DWORD PTR [esi+80]
	mov	ecx, DWORD PTR [esi+76]
	push	13369376				; 00cc0020H
	lea	edx, DWORD PTR [esi+12]
	push	0
	push	edx
	push	DWORD PTR [esi+8]
	push	eax
	push	ecx
	push	0
	push	0
	push	ebx
	push	edi
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__StretchDIBits@52
	cmp	eax, -1
	je	SHORT $L55868
$L56365:

; 399  : 		{
; 400  : 			return true;

	mov	al, 1
	jmp	SHORT $L55852
$L55868:

; 401  : 		}
; 402  : 	}
; 403  : 	return false;

	xor	al, al
$L55852:
	pop	edi
	pop	esi
	pop	ebx

; 404  : }

	pop	ebp
	ret	20					; 00000014H
?Draw@CDIB@GS@@QAE_NPAUHDC__@@HHHH@Z ENDP		; GS::CDIB::Draw
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z ; Container::CArray<tagRGBQUAD *>::RemoveAt
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
;	COMDAT ?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z
_TEXT	SEGMENT
_uSize$ = 8
_pItems$55898 = -4
?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::SetSize, COMDAT

; 46   : {

	push	ecx
	push	ebx
	push	ebp
	mov	ebp, ecx
	push	esi

; 47   : 	bool bReallocationNeeded = false;
; 48   : 	ASSERT( m_uItemCount <= m_uItemsAllocated );
; 49   : 
; 50   : 
; 51   : 	if( m_uItemCount < uSize )

	mov	esi, DWORD PTR _uSize$[esp+12]
	xor	bl, bl
	mov	eax, DWORD PTR [ebp+16]
	push	edi
	cmp	eax, esi
	jae	SHORT $L56432

; 52   : 	{
; 53   : 		if( m_uItemsAllocated < uSize )

	mov	eax, DWORD PTR [ebp+12]
	cmp	eax, esi
	jae	SHORT $L55895

; 54   : 		{
; 55   : 			PI_uint32 uAllocateExtra = m_uGrowBy = m_uGrowBy < 262144 ? m_uGrowBy << 2 : 262144;

	mov	ecx, DWORD PTR [ebp+8]
	mov	ebx, 262144				; 00040000H
	cmp	ecx, ebx
	jae	SHORT $L56393
	mov	ebx, ecx
	shl	ebx, 2
$L56393:

; 56   : 
; 57   : 			//
; 58   : 			// Check to see if our grow by is enough?
; 59   : 			if( m_uItemsAllocated + uAllocateExtra < uSize )

	lea	ecx, DWORD PTR [eax+ebx]
	mov	DWORD PTR [ebp+8], ebx
	cmp	ecx, esi
	jae	SHORT $L55897

; 60   : 			{
; 61   : 				//
; 62   : 				//	Nope, so we allocate more
; 63   : 				uAllocateExtra = uSize - m_uItemsAllocated;

	mov	ebx, esi
	sub	ebx, eax
$L55897:

; 64   : 			}
; 65   : 
; 66   : 			T *pItems = reinterpret_cast<T *>( new PI_byte[sizeof( T ) * ( m_uItemsAllocated + uAllocateExtra )] );

	add	eax, ebx
	shl	eax, 2
	push	eax
	call	??2@YAPAXI@Z				; operator new
	pop	ecx
	mov	edi, eax

; 67   : 
; 68   : 			if( m_uItemCount )

	mov	ecx, DWORD PTR [ebp+16]
	mov	DWORD PTR _pItems$55898[esp+20], edi
	test	ecx, ecx
	je	SHORT $L56398

; 69   : 				MoveItemsNoMemOverlap( m_pItems, pItems, m_uItemCount );

	mov	esi, DWORD PTR [ebp+4]
	rep movsd
	mov	esi, DWORD PTR _uSize$[esp+16]
$L56398:

; 70   : 			delete[] (PI_byte*)m_pItems;

	push	DWORD PTR [ebp+4]
	call	??3@YAXPAX@Z				; operator delete

; 71   : 			m_pItems = pItems;

	mov	eax, DWORD PTR _pItems$55898[esp+24]

; 72   : 			m_uItemsAllocated += uAllocateExtra;

	add	DWORD PTR [ebp+12], ebx
	pop	ecx
	mov	DWORD PTR [ebp+4], eax

; 73   : 			bReallocationNeeded = true;

	mov	bl, 1
$L55895:

; 74   : 		}
; 75   : 
; 76   : 		//
; 77   : 		// Constuct the new items
; 78   : 		if( uSize > m_uItemCount )

	mov	eax, DWORD PTR [ebp+16]
	cmp	esi, eax
	jbe	SHORT $L55920

; 79   : 		{
; 80   : 			PI_uint32 uItemsToConstruct = uSize - m_uItemCount;
; 81   : 			ConstructItems( &m_pItems[m_uItemCount], uItemsToConstruct );

	mov	edx, DWORD PTR [ebp+4]
	mov	ecx, esi
	sub	ecx, eax

; 82   : 			m_uItemCount = uSize;

	mov	DWORD PTR [ebp+16], esi
	lea	edi, DWORD PTR [edx+eax*4]
	xor	eax, eax
	rep stosd

; 83   : 		}
; 84   : 
; 85   : 	}
; 86   : 	else

	jmp	SHORT $L55920
$L56432:

; 87   : 	{
; 88   : 		//
; 89   : 		//	Check to see if we need to reduce the size
; 90   : 		if( m_uItemCount > uSize )

	jbe	SHORT $L55920

; 91   : 		{
; 92   : 			//
; 93   : 			//	Get rid of excess elements
; 94   : 			RemoveAt( uSize, m_uItemCount - uSize );

	sub	eax, esi
	mov	ecx, ebp
	push	eax
	push	esi
	call	?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z ; Container::CArray<tagRGBQUAD *>::RemoveAt
$L55920:
	pop	edi
	pop	esi

; 95   : 		}
; 96   : 	}
; 97   : 
; 98   : 	return bReallocationNeeded;

	mov	al, bl
	pop	ebp
	pop	ebx

; 99   : }

	pop	ecx
	ret	4
?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z ENDP ; Container::CArray<tagRGBQUAD *>::SetSize
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	call	?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ; Container::CArray<tagRGBQUAD *>::RemoveAll
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L55924
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L55924:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ENDP	; Container::CArray<tagRGBQUAD *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ PROC NEAR ; Container::CArray<tagRGBQUAD *>::RemoveAll, COMDAT

; 179  : {

	push	esi
	mov	esi, ecx

; 180  : 	DestructItems( m_pItems, m_uItemCount );
; 181  : 	delete [] reinterpret_cast<PI_byte *>( m_pItems );

	push	DWORD PTR [esi+4]
	call	??3@YAXPAX@Z				; operator delete

; 182  : 	m_pItems = NULL;

	xor	eax, eax
	pop	ecx
	mov	DWORD PTR [esi+4], eax

; 183  : 	m_uGrowBy = 1;

	mov	DWORD PTR [esi+8], 1

; 184  : 	m_uItemsAllocated = 0;

	mov	DWORD PTR [esi+12], eax

; 185  : 	m_uItemCount = 0;

	mov	DWORD PTR [esi+16], eax
	pop	esi

; 186  : }

	ret	0
?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ENDP ; Container::CArray<tagRGBQUAD *>::RemoveAll
_TEXT	ENDS
EXTRN	__imp__memmove:NEAR
;	COMDAT ?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z
_TEXT	SEGMENT
_iPos$ = 8
_uItems$ = 12
?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::RemoveAt, COMDAT

; 191  : 	ASSERT( iPos + uItems <= m_uItemCount );
; 192  : 	ASSERT( uItems > 0 );
; 193  : 
; 194  : 	DestructItems( &m_pItems[iPos], uItems );

	mov	edx, DWORD PTR _iPos$[esp-4]
	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

; 195  : 
; 196  : 	//
; 197  : 	// Move Any items above removeal area down
; 198  : 	const PI_uint32 uItemsToMove = m_uItemCount - iPos - uItems;

	mov	edi, DWORD PTR _uItems$[esp+8]
	mov	eax, DWORD PTR [esi+16]
	mov	ecx, DWORD PTR [esi+4]
	sub	eax, edx
	sub	eax, edi
	lea	ebx, DWORD PTR [ecx+edx*4]

; 199  : 	if( uItemsToMove )

	je	SHORT $L56473

; 200  : 		MoveItemsMemOverlap( &m_pItems[iPos + uItems], &m_pItems[iPos], uItemsToMove);

	shl	eax, 2
	add	edx, edi
	push	eax
	lea	eax, DWORD PTR [ecx+edx*4]
	push	eax
	push	ebx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH
$L56473:

; 201  : 	m_uItemCount -= uItems;

	sub	DWORD PTR [esi+16], edi
	pop	edi
	pop	esi
	pop	ebx

; 202  : }

	ret	8
?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z ENDP ; Container::CArray<tagRGBQUAD *>::RemoveAt
_TEXT	ENDS
END
