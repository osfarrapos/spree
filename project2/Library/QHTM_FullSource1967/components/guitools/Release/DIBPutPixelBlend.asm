	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBPutPixelBlend.cpp
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
;	COMDAT ?GetWidth@CDIB@GS@@QBEHXZ
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
PUBLIC	?PutPixelBlend@CDIB@GS@@QAEXIIEEEE@Z		; GS::CDIB::PutPixelBlend
_TEXT	SEGMENT
_x$ = 8
_y$ = 12
_r$ = 16
_g$ = 20
_b$ = 24
_a$ = 28
?PutPixelBlend@CDIB@GS@@QAEXIIEEEE@Z PROC NEAR		; GS::CDIB::PutPixelBlend

; 15   : {

	push	ebp
	mov	ebp, esp

; 16   : 	if( a )

	cmp	BYTE PTR _a$[ebp], 0
	push	edi
	je	SHORT $L55618

; 17   : 	{
; 18   : 		const CLineArray & arrDest = GetLineArray();
; 19   : 		if( y < arrDest.GetSize() && y >= 0 )

	mov	edi, DWORD PTR _y$[ebp]
	cmp	edi, DWORD PTR [ecx+72]
	jae	SHORT $L55618

; 20   : 		{
; 21   : 			DIB_PIXEL *pDest = arrDest[ y ];
; 22   : 			if( x >= 0 && x < (UINT)GetWidth() )

	mov	eax, DWORD PTR _x$[ebp]
	cmp	eax, DWORD PTR [ecx+76]
	jae	SHORT $L55618

; 23   : 			{
; 24   : 				const int nNegativeAplha = 255 - a;
; 25   : 				DIB_PIXEL *pPixel = pDest + x;

	mov	ecx, DWORD PTR [ecx+60]
	push	esi
	movzx	esi, BYTE PTR _a$[ebp]
	mov	ecx, DWORD PTR [ecx+edi*4]
	mov	edx, 255				; 000000ffH

; 26   : 				pPixel->rgbRed = static_cast<unsigned char>( ( pPixel->rgbRed * nNegativeAplha + r * a ) >>8 );

	movzx	edi, BYTE PTR _r$[ebp]
	lea	eax, DWORD PTR [ecx+eax*4]
	sub	edx, esi
	imul	edi, esi
	movzx	ecx, BYTE PTR [eax+2]
	imul	ecx, edx
	add	ecx, edi

; 27   : 				pPixel->rgbGreen = static_cast<unsigned char>( ( pPixel->rgbGreen * nNegativeAplha + g * a ) >>8 );

	movzx	edi, BYTE PTR [eax+1]
	sar	ecx, 8
	imul	edi, edx
	mov	BYTE PTR [eax+2], cl
	movzx	ecx, BYTE PTR _g$[ebp]
	imul	ecx, esi
	add	ecx, edi
	sar	ecx, 8
	mov	BYTE PTR [eax+1], cl

; 28   : 				pPixel->rgbBlue = static_cast<unsigned char>( ( pPixel->rgbBlue * nNegativeAplha + b * a ) >>8 );

	movzx	ecx, BYTE PTR _b$[ebp]
	imul	ecx, esi
	movzx	esi, BYTE PTR [eax]
	imul	esi, edx
	add	ecx, esi
	pop	esi
	sar	ecx, 8
	mov	BYTE PTR [eax], cl
$L55618:
	pop	edi

; 29   : 			}
; 30   : 		}
; 31   : 	}
; 32   : }

	pop	ebp
	ret	24					; 00000018H
?PutPixelBlend@CDIB@GS@@QAEXIIEEEE@Z ENDP		; GS::CDIB::PutPixelBlend
_TEXT	ENDS
END
