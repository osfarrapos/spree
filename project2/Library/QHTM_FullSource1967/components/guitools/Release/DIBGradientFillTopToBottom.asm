	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBGradientFillTopToBottom.cpp
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
PUBLIC	?GradientFillTopToBottom@CDIB@GS@@QAEXABUtagRECT@@KK@Z ; GS::CDIB::GradientFillTopToBottom
_TEXT	SEGMENT
_rc$ = 8
_crTop$ = 12
_crBottom$ = 16
_this$ = -40
_nTop$ = -4
_nLeft$ = 8
_r1$ = -24
_g1$ = -28
_b1$ = -32
_a1$ = -36
_nEnd$ = -44
_b$55642 = -40
?GradientFillTopToBottom@CDIB@GS@@QAEXABUtagRECT@@KK@Z PROC NEAR ; GS::CDIB::GradientFillTopToBottom

; 14   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 64					; 00000040H

; 15   : 	const UINT nTop = max( rc.top, 0 );

	mov	eax, DWORD PTR _rc$[ebp]
	mov	DWORD PTR _this$[ebp], ecx
	mov	edx, DWORD PTR [eax+4]
	test	edx, edx
	mov	DWORD PTR -12+[ebp], edx
	jle	SHORT $L55714
	mov	DWORD PTR _nTop$[ebp], edx
	jmp	SHORT $L55715
$L55714:
	and	DWORD PTR _nTop$[ebp], 0
$L55715:

; 16   : 	const UINT nBottom = min( rc.bottom, GetHeight() );

	mov	edx, DWORD PTR [eax+12]
	push	ebx
	push	esi
	mov	esi, DWORD PTR [ecx+80]
	cmp	edx, esi
	push	edi
	mov	DWORD PTR -16+[ebp], edx
	mov	DWORD PTR -20+[ebp], edx
	jl	SHORT $L55727
	mov	DWORD PTR -20+[ebp], esi
$L55727:

; 17   : 	const UINT nLeft = max( rc.left, 0 );

	mov	edx, DWORD PTR [eax]
	test	edx, edx
	jle	SHORT $L55718
	mov	DWORD PTR _nLeft$[ebp], edx
	jmp	SHORT $L55719
$L55718:
	and	DWORD PTR _nLeft$[ebp], 0
$L55719:

; 18   : 	const UINT nRight = min( rc.right, GetWidth() );

	mov	ecx, DWORD PTR [ecx+76]
	mov	eax, DWORD PTR [eax+8]
	cmp	eax, ecx
	mov	DWORD PTR -8+[ebp], eax
	jl	SHORT $L55735
	mov	DWORD PTR -8+[ebp], ecx
$L55735:

; 19   : 
; 20   :   const int r1 = GetRValue( crTop ), g1 = GetGValue( crTop ), b1 = GetBValue( crTop ), a1 = GetAValue( crTop );

	mov	eax, DWORD PTR _crTop$[ebp]

; 21   :   const int r2 = GetRValue( crBottom ), g2 = GetGValue( crBottom ), b2 = GetBValue( crBottom ), a2 = GetAValue( crBottom );

	xor	edx, edx
	movzx	ecx, al
	mov	DWORD PTR _r1$[ebp], ecx
	xor	ecx, ecx
	mov	cl, ah
	movzx	ecx, cl
	mov	DWORD PTR _g1$[ebp], ecx
	mov	ecx, eax
	shr	ecx, 16					; 00000010H
	movzx	ecx, cl
	mov	DWORD PTR _b1$[ebp], ecx
	mov	ecx, DWORD PTR _crBottom$[ebp]
	shr	eax, 24					; 00000018H
	mov	DWORD PTR _a1$[ebp], eax
	mov	dl, ch
	movzx	eax, cl
	mov	ebx, ecx
	shr	ecx, 24					; 00000018H
	mov	edi, ecx

; 22   : 
; 23   : 	int nEnd = rc.bottom - rc.top;

	mov	ecx, DWORD PTR -16+[ebp]
	sub	ecx, DWORD PTR -12+[ebp]
	shr	ebx, 16					; 00000010H
	mov	DWORD PTR _nEnd$[ebp], ecx

; 24   : 
; 25   : 	const CLineArray & arrDest = GetLineArray();
; 26   : 	int nLine = 0;
; 27   : 	for( UINT y = nTop; y < nBottom; y++, nLine++ )

	mov	ecx, DWORD PTR -20+[ebp]
	cmp	DWORD PTR _nTop$[ebp], ecx
	movzx	edx, dl
	movzx	esi, bl
	jae	$L55638

; 28   : 	{
; 29   : 		DIB_PIXEL *pDest = arrDest[ y ];
; 30   : 		pDest += nLeft;
; 31   :     int r,g,b, a; 
; 32   : 
; 33   :     r = r1 + (nLine * (r2-r1) / nEnd); 

	sub	eax, DWORD PTR _r1$[ebp]

; 34   :     g = g1 + (nLine * (g2-g1) / nEnd);

	sub	edx, DWORD PTR _g1$[ebp]
	and	DWORD PTR -20+[ebp], 0
	and	DWORD PTR -16+[ebp], 0
	and	DWORD PTR -12+[ebp], 0
	and	DWORD PTR 16+[ebp], 0

; 35   :     b = b1 + (nLine * (b2-b1) / nEnd);
; 36   : 		a = a1 + (nLine * (a2-a1) / nEnd);

	sub	edi, DWORD PTR _a1$[ebp]
	sub	esi, DWORD PTR _b1$[ebp]
	mov	DWORD PTR -52+[ebp], eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR -56+[ebp], edx
	mov	DWORD PTR -64+[ebp], edi
	mov	edx, DWORD PTR [eax+60]
	mov	eax, DWORD PTR _nTop$[ebp]
	sub	ecx, eax
	mov	DWORD PTR -60+[ebp], esi
	lea	edx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR -4+[ebp], ecx
	mov	DWORD PTR 12+[ebp], edx
$L55636:
	mov	eax, DWORD PTR _nLeft$[ebp]
	mov	ebx, DWORD PTR _nEnd$[ebp]
	mov	ecx, eax
	mov	eax, DWORD PTR 12+[ebp]
	shl	ecx, 2
	add	ecx, DWORD PTR [eax]
	mov	eax, DWORD PTR 16+[ebp]
	cdq
	idiv	ebx
	mov	esi, eax
	mov	eax, DWORD PTR -12+[ebp]
	cdq
	idiv	ebx
	add	esi, DWORD PTR _r1$[ebp]
	mov	edi, eax
	mov	eax, DWORD PTR -16+[ebp]
	cdq
	idiv	ebx
	add	edi, DWORD PTR _g1$[ebp]
	add	eax, DWORD PTR _b1$[ebp]
	mov	DWORD PTR _b$55642[ebp], eax
	mov	eax, DWORD PTR -20+[ebp]
	cdq
	idiv	ebx

; 37   : 		
; 38   : 		const int nNegativeAplha = 255 - a;
; 39   : 
; 40   : 		for( UINT x = nLeft; x < nRight; x++, pDest++ )

	mov	ebx, DWORD PTR -8+[ebp]
	mov	edx, 255				; 000000ffH
	add	eax, DWORD PTR _a1$[ebp]
	sub	edx, eax
	cmp	DWORD PTR _nLeft$[ebp], ebx
	jae	SHORT $L55637

; 41   : 		{
; 42   :       pDest->rgbRed = static_cast<unsigned char>( ( pDest->rgbRed * nNegativeAplha + r * a ) >>8 );

	mov	ebx, eax
	imul	ebx, esi

; 43   :       pDest->rgbGreen = static_cast<unsigned char>( ( pDest->rgbGreen * nNegativeAplha + g * a ) >>8 );

	mov	esi, eax

; 44   :       pDest->rgbBlue = static_cast<unsigned char>( ( pDest->rgbBlue * nNegativeAplha + b * a ) >>8 );

	imul	eax, DWORD PTR _b$55642[ebp]
	imul	esi, edi
	mov	DWORD PTR -48+[ebp], esi
	mov	esi, DWORD PTR -8+[ebp]
	sub	esi, DWORD PTR _nLeft$[ebp]
	mov	DWORD PTR -40+[ebp], eax
$L55646:
	movzx	eax, BYTE PTR [ecx+2]
	imul	eax, edx
	add	eax, ebx
	sar	eax, 8
	mov	BYTE PTR [ecx+2], al
	movzx	eax, BYTE PTR [ecx+1]
	imul	eax, edx
	add	eax, DWORD PTR -48+[ebp]
	sar	eax, 8
	mov	BYTE PTR [ecx+1], al
	movzx	eax, BYTE PTR [ecx]
	imul	eax, edx
	add	eax, DWORD PTR -40+[ebp]
	sar	eax, 8
	mov	BYTE PTR [ecx], al
	add	ecx, 4
	dec	esi
	jne	SHORT $L55646
$L55637:

; 24   : 
; 25   : 	const CLineArray & arrDest = GetLineArray();
; 26   : 	int nLine = 0;
; 27   : 	for( UINT y = nTop; y < nBottom; y++, nLine++ )

	mov	eax, DWORD PTR -52+[ebp]
	add	DWORD PTR 12+[ebp], 4
	add	DWORD PTR 16+[ebp], eax
	mov	eax, DWORD PTR -56+[ebp]
	add	DWORD PTR -12+[ebp], eax
	mov	eax, DWORD PTR -60+[ebp]
	add	DWORD PTR -16+[ebp], eax
	mov	eax, DWORD PTR -64+[ebp]
	add	DWORD PTR -20+[ebp], eax
	dec	DWORD PTR -4+[ebp]
	jne	$L55636
$L55638:
	pop	edi
	pop	esi
	pop	ebx

; 45   : 		}
; 46   : 	}
; 47   : }

	leave
	ret	12					; 0000000cH
?GradientFillTopToBottom@CDIB@GS@@QAEXABUtagRECT@@KK@Z ENDP ; GS::CDIB::GradientFillTopToBottom
_TEXT	ENDS
END
