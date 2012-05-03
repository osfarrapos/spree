	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBRectangle.cpp
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
;	COMDAT ?GetHeight@CDIB@GS@@QBEHXZ
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
PUBLIC	?Rectangle@CDIB@GS@@QAEXABUtagRECT@@K@Z		; GS::CDIB::Rectangle
EXTRN	?DrawLine@CDIB@GS@@QAEXHHHHK@Z:NEAR		; GS::CDIB::DrawLine
_TEXT	SEGMENT
_rc$ = 8
_cr$ = 12
_this$ = -20
_nTop$ = -12
_nLeft$ = -24
_g$ = -1
?Rectangle@CDIB@GS@@QAEXABUtagRECT@@K@Z PROC NEAR	; GS::CDIB::Rectangle

; 14   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H
	push	ebx
	push	esi

; 15   : 	const UINT nTop = max( rc.top, 0 );

	mov	esi, DWORD PTR _rc$[ebp]
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR [esi+4]
	test	eax, eax
	jle	SHORT $L55696
	mov	DWORD PTR _nTop$[ebp], eax
	jmp	SHORT $L55697
$L55696:
	and	DWORD PTR _nTop$[ebp], 0
$L55697:

; 16   : 	const UINT nBottom = min( rc.bottom, GetHeight() );

	mov	eax, DWORD PTR [ecx+80]
	mov	ecx, DWORD PTR [esi+12]
	cmp	ecx, eax
	mov	DWORD PTR -16+[ebp], ecx
	jl	SHORT $L55707
	mov	DWORD PTR -16+[ebp], eax
$L55707:

; 17   : 	const UINT nLeft = max( rc.left, 0 );

	mov	ecx, DWORD PTR [esi]
	test	ecx, ecx
	jle	SHORT $L55700
	mov	DWORD PTR _nLeft$[ebp], ecx
	jmp	SHORT $L55701
$L55700:
	and	DWORD PTR _nLeft$[ebp], 0
$L55701:

; 18   : 	const UINT nWidth = rc.right - rc.left;

	mov	eax, DWORD PTR [esi+8]

; 19   : 
; 20   : 	const BYTE r = GetRValue( cr );
; 21   : 	const BYTE g = GetGValue( cr );
; 22   : 	const BYTE b = GetBValue( cr );

	mov	ebx, DWORD PTR _cr$[ebp]
	sub	eax, ecx
	xor	ecx, ecx
	mov	cl, BYTE PTR _cr$[ebp+1]

; 23   : 	const BYTE a = GetAValue( cr );
; 24   : 
; 25   : 	const int nNegativeAplha = 255 - a;

	mov	edx, 255				; 000000ffH
	mov	BYTE PTR _g$[ebp], cl
	mov	ecx, DWORD PTR _cr$[ebp]
	shr	ecx, 24					; 00000018H
	movzx	ecx, cl
	mov	DWORD PTR -8+[ebp], ecx
	sub	edx, ecx

; 26   :   
; 27   : 	const CLineArray & arrDest = GetLineArray();
; 28   : 	for( UINT y = nTop; y < nBottom; y++ )

	mov	ecx, DWORD PTR -16+[ebp]
	shr	ebx, 16					; 00000010H
	cmp	DWORD PTR _nTop$[ebp], ecx
	jae	$L55627

; 29   : 	{
; 30   : 		DIB_PIXEL *pDest = arrDest[ y ];
; 31   : 
; 32   : 		pDest += nLeft;

	mov	ecx, DWORD PTR _nLeft$[ebp]
	push	edi

; 33   :     pDest->rgbRed = static_cast<unsigned char>( ( pDest->rgbRed * nNegativeAplha + r * a ) >>8 );
; 34   :     pDest->rgbGreen = static_cast<unsigned char>( ( pDest->rgbGreen * nNegativeAplha + g * a ) >>8 );
; 35   :     pDest->rgbBlue = static_cast<unsigned char>( ( pDest->rgbBlue * nNegativeAplha + b * a ) >>8 );
; 36   : 
; 37   : 		pDest += nWidth;

	shl	eax, 2

; 38   :     pDest->rgbRed = static_cast<unsigned char>( ( pDest->rgbRed * nNegativeAplha + r * a ) >>8 );
; 39   :     pDest->rgbGreen = static_cast<unsigned char>( ( pDest->rgbGreen * nNegativeAplha + g * a ) >>8 );

	movzx	esi, BYTE PTR _g$[ebp]
	mov	DWORD PTR -32+[ebp], eax
	mov	eax, DWORD PTR _this$[ebp]
	imul	esi, DWORD PTR -8+[ebp]
	mov	eax, DWORD PTR [eax+60]

; 40   :     pDest->rgbBlue = static_cast<unsigned char>( ( pDest->rgbBlue * nNegativeAplha + b * a ) >>8 );

	movzx	edi, bl
	mov	ebx, DWORD PTR _nTop$[ebp]
	imul	edi, DWORD PTR -8+[ebp]
	shl	ecx, 2
	mov	DWORD PTR -28+[ebp], ecx
	lea	eax, DWORD PTR [eax+ebx*4]
	movzx	ecx, BYTE PTR _cr$[ebp]
	imul	ecx, DWORD PTR -8+[ebp]
	mov	DWORD PTR -12+[ebp], eax
	mov	eax, DWORD PTR -16+[ebp]
	sub	eax, ebx
	mov	DWORD PTR -24+[ebp], eax
$L55625:
	mov	eax, DWORD PTR -12+[ebp]
	add	DWORD PTR -12+[ebp], 4
	mov	eax, DWORD PTR [eax]
	add	eax, DWORD PTR -28+[ebp]
	movzx	ebx, BYTE PTR [eax+2]
	imul	ebx, edx
	add	ebx, ecx
	sar	ebx, 8
	mov	BYTE PTR [eax+2], bl
	movzx	ebx, BYTE PTR [eax+1]
	imul	ebx, edx
	add	ebx, esi
	sar	ebx, 8
	mov	BYTE PTR [eax+1], bl
	movzx	ebx, BYTE PTR [eax]
	imul	ebx, edx
	add	ebx, edi
	sar	ebx, 8
	mov	BYTE PTR [eax], bl
	add	eax, DWORD PTR -32+[ebp]
	movzx	ebx, BYTE PTR [eax+2]
	imul	ebx, edx
	add	ebx, ecx
	sar	ebx, 8
	mov	BYTE PTR [eax+2], bl
	movzx	ebx, BYTE PTR [eax+1]
	imul	ebx, edx
	add	ebx, esi
	sar	ebx, 8
	mov	BYTE PTR [eax+1], bl
	movzx	ebx, BYTE PTR [eax]
	imul	ebx, edx
	add	ebx, edi
	sar	ebx, 8
	dec	DWORD PTR -24+[ebp]
	mov	BYTE PTR [eax], bl
	jne	SHORT $L55625
	mov	esi, DWORD PTR _rc$[ebp]
	pop	edi
$L55627:

; 41   : 	}
; 42   : 	DrawLine( rc.left, rc.top, rc.right, rc.top, cr );

	push	DWORD PTR _cr$[ebp]
	mov	eax, DWORD PTR [esi+4]
	mov	ecx, DWORD PTR _this$[ebp]
	push	eax
	push	DWORD PTR [esi+8]
	push	eax
	push	DWORD PTR [esi]
	call	?DrawLine@CDIB@GS@@QAEXHHHHK@Z		; GS::CDIB::DrawLine

; 43   : 	DrawLine( rc.left, rc.bottom, rc.right, rc.bottom, cr );

	push	DWORD PTR _cr$[ebp]
	mov	eax, DWORD PTR [esi+12]
	mov	ecx, DWORD PTR _this$[ebp]
	push	eax
	push	DWORD PTR [esi+8]
	push	eax
	push	DWORD PTR [esi]
	call	?DrawLine@CDIB@GS@@QAEXHHHHK@Z		; GS::CDIB::DrawLine
	pop	esi
	pop	ebx

; 44   : }

	leave
	ret	8
?Rectangle@CDIB@GS@@QAEXABUtagRECT@@K@Z ENDP		; GS::CDIB::Rectangle
_TEXT	ENDS
END
