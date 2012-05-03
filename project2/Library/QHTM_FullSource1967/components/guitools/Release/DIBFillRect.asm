	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBFillRect.cpp
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
PUBLIC	?FillRect@CDIB@GS@@QAEXABUtagRECT@@K@Z		; GS::CDIB::FillRect
_TEXT	SEGMENT
_rc$ = 8
_cr$ = 12
_this$ = -16
_nTop$ = -4
_nLeft$ = -8
_nNegativeAplha$ = 8
?FillRect@CDIB@GS@@QAEXABUtagRECT@@K@Z PROC NEAR	; GS::CDIB::FillRect

; 17   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H

; 18   : 	const UINT nTop = max( rc.top, 0 );

	mov	eax, DWORD PTR _rc$[ebp]
	push	ebx
	push	esi
	mov	esi, ecx
	mov	ecx, DWORD PTR [eax+4]
	push	edi
	test	ecx, ecx
	mov	DWORD PTR _this$[ebp], esi
	jle	SHORT $L55699
	mov	DWORD PTR _nTop$[ebp], ecx
	jmp	SHORT $L55700
$L55699:
	and	DWORD PTR _nTop$[ebp], 0
$L55700:

; 19   : 	register const UINT nBottom = min( rc.bottom, GetHeight() );

	mov	ecx, DWORD PTR [esi+80]
	mov	edx, DWORD PTR [eax+12]
	cmp	edx, ecx
	jge	SHORT $L55712
	mov	ecx, edx
$L55712:

; 20   : 	const UINT nLeft = max( rc.left, 0 );

	mov	edx, DWORD PTR [eax]
	test	edx, edx
	jle	SHORT $L55703
	mov	DWORD PTR _nLeft$[ebp], edx
	jmp	SHORT $L55704
$L55703:
	and	DWORD PTR _nLeft$[ebp], 0
$L55704:

; 21   : 	register const UINT nRight = min( rc.right, GetWidth() );

	mov	edx, DWORD PTR [esi+76]
	mov	eax, DWORD PTR [eax+8]
	cmp	eax, edx
	mov	DWORD PTR -12+[ebp], eax
	jl	SHORT $L55720
	mov	DWORD PTR -12+[ebp], edx
$L55720:

; 22   : 
; 23   : 	const UINT a = GetAValue( cr );

	mov	edx, DWORD PTR _cr$[ebp]

; 24   : 	const UINT r = GetRValue( cr ) * a;
; 25   : 	const UINT g = GetGValue( cr ) * a;

	xor	ebx, ebx
	mov	eax, edx
	mov	bl, dh
	movzx	esi, dl

; 26   : 	const UINT b = GetBValue( cr ) * a;

	shr	edx, 16					; 00000010H
	movzx	edi, bl
	movzx	edx, dl
	shr	eax, 24					; 00000018H

; 27   : 	
; 28   : 
; 29   : 	register int nNegativeAplha = 255 - a;

	mov	ebx, 255				; 000000ffH
	imul	esi, eax
	imul	edi, eax
	imul	edx, eax
	sub	ebx, eax

; 30   :   
; 31   : 	const CLineArray & arrDest = GetLineArray();
; 32   : 	for(  UINT y = nTop; y < nBottom; y++ )

	mov	eax, DWORD PTR _nTop$[ebp]
	cmp	eax, ecx
	mov	DWORD PTR _nNegativeAplha$[ebp], ebx
	jae	SHORT $L55628
	mov	ebx, DWORD PTR _this$[ebp]
	sub	ecx, eax
	mov	DWORD PTR -16+[ebp], ecx
	mov	ebx, DWORD PTR [ebx+60]
	lea	ebx, DWORD PTR [ebx+eax*4]
	mov	DWORD PTR 12+[ebp], ebx
$L55626:

; 33   : 	{
; 34   : 		DIB_PIXEL *pDest = arrDest[ y ];
; 35   : 		pDest += nLeft;

	mov	ecx, DWORD PTR _nLeft$[ebp]
	mov	ebx, DWORD PTR 12+[ebp]
	mov	eax, ecx
	shl	eax, 2
	add	eax, DWORD PTR [ebx]

; 36   : 
; 37   : 		for( UINT x = nLeft; x < nRight; x++, pDest++ )

	mov	ebx, DWORD PTR -12+[ebp]
	cmp	ecx, ebx
	jae	SHORT $L55627
	sub	ebx, ecx
	mov	DWORD PTR -4+[ebp], ebx
$L55631:

; 38   : 		{
; 39   : 			Blend( pDest->rgbRed, nNegativeAplha, r, a );

	movzx	ecx, BYTE PTR [eax+2]
	imul	ecx, DWORD PTR _nNegativeAplha$[ebp]
	add	ecx, esi
	shr	ecx, 8
	mov	BYTE PTR [eax+2], cl

; 40   : 			Blend( pDest->rgbGreen, nNegativeAplha, g, a );

	movzx	ecx, BYTE PTR [eax+1]
	imul	ecx, DWORD PTR _nNegativeAplha$[ebp]
	add	ecx, edi
	shr	ecx, 8
	mov	BYTE PTR [eax+1], cl

; 41   : 			Blend( pDest->rgbBlue, nNegativeAplha, b, a );

	movzx	ecx, BYTE PTR [eax]
	imul	ecx, DWORD PTR _nNegativeAplha$[ebp]
	add	ecx, edx
	shr	ecx, 8
	mov	BYTE PTR [eax], cl
	add	eax, 4
	dec	DWORD PTR -4+[ebp]
	jne	SHORT $L55631
$L55627:

; 30   :   
; 31   : 	const CLineArray & arrDest = GetLineArray();
; 32   : 	for(  UINT y = nTop; y < nBottom; y++ )

	add	DWORD PTR 12+[ebp], 4
	dec	DWORD PTR -16+[ebp]
	jne	SHORT $L55626
$L55628:
	pop	edi
	pop	esi
	pop	ebx

; 42   : 		}
; 43   : 	}
; 44   : }

	leave
	ret	8
?FillRect@CDIB@GS@@QAEXABUtagRECT@@K@Z ENDP		; GS::CDIB::FillRect
_TEXT	ENDS
END
