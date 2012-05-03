	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DrawBitmapShadow.cpp
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
;	COMDAT ?GetLineArray@CDIB@GS@@QAEABV?$CArray@PAUtagRGBQUAD@@@Container@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAUtagRGBQUAD@@@Container@@QBEABQAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?DrawBitmapShadow@@YAXPAUHDC__@@PAUHBITMAP__@@KKHHHHHHH@Z ; DrawBitmapShadow
EXTRN	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z:NEAR		; GS::CDIB::CDIB
EXTRN	?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z:NEAR ; GS::CDIB::DrawWithAlphaChannel
EXTRN	?Blur@CDIB@GS@@QAEXH@Z:NEAR			; GS::CDIB::Blur
EXTRN	?DrawBitmapOntoUs@CDIB@GS@@QAEXPAUHBITMAP__@@HHHHHH@Z:NEAR ; GS::CDIB::DrawBitmapOntoUs
EXTRN	?ClearToColour@CDIB@GS@@QAEXK@Z:NEAR		; GS::CDIB::ClearToColour
EXTRN	??1CDIB@GS@@UAE@XZ:NEAR				; GS::CDIB::~CDIB
_TEXT	SEGMENT
_hdc$ = 8
_bmp$ = 12
_crBackground$ = 16
_crShadow$ = 20
_x$ = 24
_y$ = 28
_nOffset$ = 32
_nSourceX$ = 36
_nSourceY$ = 40
_nCX$ = 44
_nCY$ = 48
_nWidth$ = -4
_dibDest$ = -96
_bGreen$ = 35
_bShadowGreen$ = 47
_uSourceLines$ = 40
?DrawBitmapShadow@@YAXPAUHDC__@@PAUHBITMAP__@@KKHHHHHHH@Z PROC NEAR ; DrawBitmapShadow

; 14   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 96					; 00000060H
	push	ebx

; 15   : 	const int nBlur = 1;
; 16   : 	const int nWidth = nCX + nOffset + nBlur * 2;

	mov	ebx, DWORD PTR _nCX$[ebp]
	push	esi
	mov	esi, DWORD PTR _nOffset$[ebp]
	push	edi

; 17   : 	const int nHeight = nCY + nOffset + nBlur * 2;
; 18   : 
; 19   : 	GS::CDIB dibDest( hdc, nWidth, nHeight );

	mov	edi, DWORD PTR _nCY$[ebp]
	lea	eax, DWORD PTR [esi+ebx+2]
	lea	ecx, DWORD PTR [esi+edi+2]
	mov	DWORD PTR _nWidth$[ebp], eax
	push	ecx
	push	eax
	push	DWORD PTR _hdc$[ebp]
	lea	ecx, DWORD PTR _dibDest$[ebp]
	call	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z		; GS::CDIB::CDIB

; 20   : 	dibDest.ClearToColour( crBackground );

	push	DWORD PTR _crBackground$[ebp]
	lea	ecx, DWORD PTR _dibDest$[ebp]
	call	?ClearToColour@CDIB@GS@@QAEXK@Z		; GS::CDIB::ClearToColour

; 21   : 	dibDest.DrawBitmapOntoUs( bmp, nBlur, nBlur, nSourceX, nSourceY, nCX, nCY );

	push	edi
	push	ebx
	push	DWORD PTR _nSourceY$[ebp]
	lea	ecx, DWORD PTR _dibDest$[ebp]
	push	DWORD PTR _nSourceX$[ebp]
	push	1
	push	1
	push	DWORD PTR _bmp$[ebp]
	call	?DrawBitmapOntoUs@CDIB@GS@@QAEXPAUHBITMAP__@@HHHHHH@Z ; GS::CDIB::DrawBitmapOntoUs

; 22   : 
; 23   : 	const BYTE bRed = GetRValue( crBackground )
; 24   : 						, bGreen = GetGValue( crBackground )
; 25   : 						, bBlue GetBValue( crBackground )

	mov	edx, DWORD PTR _crBackground$[ebp]

; 26   : 						;
; 27   : 
; 28   : 	const BYTE bShadowRed = GetRValue( crShadow )
; 29   : 						, bShadowGreen = GetGValue( crShadow )
; 30   : 						, bShadowBlue = GetBValue( crShadow )

	mov	ecx, DWORD PTR _crShadow$[ebp]
	xor	eax, eax

; 31   : 						;
; 32   : 
; 33   : 	const int nAlpha = 150;
; 34   : 
; 35   : 	const GS::CDIB::CLineArray & arrDest = dibDest.GetLineArray();
; 36   : 	const UINT uSourceLines = arrDest.GetSize();
; 37   : 
; 38   : 	for( UINT u = 0; u < uSourceLines; u++ )

	xor	edi, edi
	mov	al, dh
	mov	BYTE PTR _bGreen$[ebp], al
	xor	eax, eax
	mov	al, ch
	mov	BYTE PTR _bShadowGreen$[ebp], al
	mov	eax, DWORD PTR _dibDest$[ebp+72]
	shr	edx, 16					; 00000010H
	shr	ecx, 16					; 00000010H
	test	eax, eax
	mov	DWORD PTR _uSourceLines$[ebp], eax
	jbe	SHORT $L55653
$L55651:

; 39   : 	{
; 40   : 		DIB_PIXEL *pDest = arrDest[ u ];

	mov	eax, DWORD PTR _dibDest$[ebp+60]

; 41   : 		for( int c = 0; c < nWidth; c++, pDest++ )

	cmp	DWORD PTR _nWidth$[ebp], 0
	mov	eax, DWORD PTR [eax+edi*4]
	jle	SHORT $L55652
	mov	ebx, DWORD PTR _nWidth$[ebp]
	mov	DWORD PTR 48+[ebp], ebx
$L55656:

; 42   : 		{
; 43   : 			if( pDest->rgbRed != bRed || pDest->rgbGreen != bGreen || pDest->rgbBlue != bBlue )

	mov	bl, BYTE PTR [eax+2]
	cmp	bl, BYTE PTR _crBackground$[ebp]
	jne	SHORT $L55660
	mov	bl, BYTE PTR _bGreen$[ebp]
	cmp	BYTE PTR [eax+1], bl
	jne	SHORT $L55660
	cmp	BYTE PTR [eax], dl
	jne	SHORT $L55660

; 49   : 			}
; 50   : 			else
; 51   : 			{
; 52   : 				pDest->rgbReserved = 0;

	xor	bl, bl
	mov	BYTE PTR [eax+3], bl

; 53   : 				pDest->rgbRed = pDest->rgbGreen = pDest->rgbBlue = 0;

	mov	BYTE PTR [eax], bl
	mov	BYTE PTR [eax+1], bl
	mov	BYTE PTR [eax+2], bl
	jmp	SHORT $L55657
$L55660:

; 44   : 			{
; 45   : 				pDest->rgbRed = bShadowRed;

	mov	bl, BYTE PTR _crShadow$[ebp]

; 46   : 				pDest->rgbGreen = bShadowGreen;
; 47   : 				pDest->rgbBlue = bShadowBlue;

	mov	BYTE PTR [eax], cl
	mov	BYTE PTR [eax+2], bl
	mov	bl, BYTE PTR _bShadowGreen$[ebp]
	mov	BYTE PTR [eax+1], bl

; 48   : 				pDest->rgbReserved = nAlpha;

	mov	BYTE PTR [eax+3], 150			; 00000096H
$L55657:
	add	eax, 4
	dec	DWORD PTR 48+[ebp]
	jne	SHORT $L55656
$L55652:

; 31   : 						;
; 32   : 
; 33   : 	const int nAlpha = 150;
; 34   : 
; 35   : 	const GS::CDIB::CLineArray & arrDest = dibDest.GetLineArray();
; 36   : 	const UINT uSourceLines = arrDest.GetSize();
; 37   : 
; 38   : 	for( UINT u = 0; u < uSourceLines; u++ )

	inc	edi
	cmp	edi, DWORD PTR _uSourceLines$[ebp]
	jb	SHORT $L55651
$L55653:

; 54   : 			}
; 55   : 		}
; 56   : 	}
; 57   : 
; 58   : 	dibDest.Blur( nBlur );

	push	1
	lea	ecx, DWORD PTR _dibDest$[ebp]
	call	?Blur@CDIB@GS@@QAEXH@Z			; GS::CDIB::Blur

; 59   : 	dibDest.DrawWithAlphaChannel( hdc, x + nOffset - nBlur, y + nOffset - nBlur );

	mov	eax, DWORD PTR _y$[ebp]
	lea	ecx, DWORD PTR _dibDest$[ebp]
	lea	eax, DWORD PTR [eax+esi-1]
	push	eax
	mov	eax, DWORD PTR _x$[ebp]
	lea	eax, DWORD PTR [eax+esi-1]
	push	eax
	push	DWORD PTR _hdc$[ebp]
	call	?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ; GS::CDIB::DrawWithAlphaChannel

; 60   : 
; 61   : }

	lea	ecx, DWORD PTR _dibDest$[ebp]
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	0
?DrawBitmapShadow@@YAXPAUHDC__@@PAUHBITMAP__@@KKHHHHHHH@Z ENDP ; DrawBitmapShadow
_TEXT	ENDS
END
