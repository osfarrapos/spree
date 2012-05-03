	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBGradientFillLeftToRight.cpp
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
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
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
PUBLIC	?GradientFillLeftToRight@CDIB@GS@@QAEXABUtagRECT@@KK@Z ; GS::CDIB::GradientFillLeftToRight
EXTRN	?DrawLine@CDIB@GS@@QAEXHHHHK@Z:NEAR		; GS::CDIB::DrawLine
_TEXT	SEGMENT
_rc$ = 8
_crTop$ = 12
_crBottom$ = 16
_this$ = -36
_nTop$ = -28
_nLeft$ = 8
_r1$ = -20
_g1$ = -12
_b1$ = -16
_a1$ = -8
_nEnd$ = -32
?GradientFillLeftToRight@CDIB@GS@@QAEXABUtagRECT@@KK@Z PROC NEAR ; GS::CDIB::GradientFillLeftToRight

; 15   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 52					; 00000034H

; 16   : 	const UINT nTop = max( rc.top, 0 );

	mov	eax, DWORD PTR _rc$[ebp]
	mov	DWORD PTR _this$[ebp], ecx
	mov	edx, DWORD PTR [eax+4]
	test	edx, edx
	jle	SHORT $L55699
	mov	DWORD PTR _nTop$[ebp], edx
	jmp	SHORT $L55700
$L55699:
	and	DWORD PTR _nTop$[ebp], 0
$L55700:

; 17   : 	const UINT nBottom = min( rc.bottom, GetHeight() );

	mov	edx, DWORD PTR [ecx+80]
	push	ebx
	push	esi
	mov	esi, DWORD PTR [eax+12]
	cmp	esi, edx
	push	edi
	mov	DWORD PTR -24+[ebp], esi
	jl	SHORT $L55712
	mov	DWORD PTR -24+[ebp], edx
$L55712:

; 18   : 	const UINT nLeft = max( rc.left, 0 );

	mov	edx, DWORD PTR [eax]
	test	edx, edx
	mov	DWORD PTR -32+[ebp], edx
	jle	SHORT $L55703
	mov	DWORD PTR _nLeft$[ebp], edx
	jmp	SHORT $L55704
$L55703:
	and	DWORD PTR _nLeft$[ebp], 0
$L55704:

; 19   : 	const UINT nRight = min( rc.right, GetWidth() );

	mov	ecx, DWORD PTR [ecx+76]
	mov	edi, DWORD PTR [eax+8]
	cmp	edi, ecx
	mov	DWORD PTR -4+[ebp], edi
	jl	SHORT $L55720
	mov	DWORD PTR -4+[ebp], ecx
$L55720:

; 20   : 
; 21   :   const int r1 = GetRValue( crTop ), g1 = GetGValue( crTop ), b1 = GetBValue( crTop ), a1 = GetAValue( crTop );

	mov	eax, DWORD PTR _crTop$[ebp]

; 22   :   const int r2 = GetRValue( crBottom ), g2 = GetGValue( crBottom ), b2 = GetBValue( crBottom ), a2 = GetAValue( crBottom );
; 23   : 
; 24   : 	int nEnd = rc.right - rc.left;

	sub	edi, DWORD PTR -32+[ebp]
	movzx	ecx, al
	mov	DWORD PTR _r1$[ebp], ecx
	xor	ecx, ecx
	mov	cl, ah
	mov	edx, DWORD PTR _crBottom$[ebp]
	movzx	ecx, cl
	mov	DWORD PTR _g1$[ebp], ecx
	mov	ecx, eax
	shr	ecx, 16					; 00000010H
	movzx	ecx, cl
	mov	DWORD PTR _b1$[ebp], ecx
	xor	ecx, ecx
	shr	eax, 24					; 00000018H
	mov	DWORD PTR _a1$[ebp], eax
	mov	ebx, edx
	mov	DWORD PTR _nEnd$[ebp], edi

; 25   : 
; 26   : 	int nCol = 0;
; 27   : 	for( UINT x = nLeft; x < nRight; x++, nCol++)

	mov	edi, DWORD PTR _nLeft$[ebp]
	movzx	eax, dl
	mov	cl, dh
	shr	ebx, 16					; 00000010H
	shr	edx, 24					; 00000018H
	cmp	edi, DWORD PTR -4+[ebp]
	movzx	ecx, cl
	movzx	esi, bl
	jae	$L55637
	and	DWORD PTR 16+[ebp], 0
	and	DWORD PTR 12+[ebp], 0
	and	DWORD PTR 8+[ebp], 0

; 28   : 	{
; 29   :     int r,g,b, a; 
; 30   : 
; 31   :     r = r1 + (nCol * (r2-r1) / nEnd); 

	sub	eax, DWORD PTR _r1$[ebp]

; 32   :     g = g1 + (nCol * (g2-g1) / nEnd);
; 33   :     b = b1 + (nCol * (b2-b1) / nEnd);

	sub	esi, DWORD PTR _b1$[ebp]
	sub	ecx, DWORD PTR _g1$[ebp]
	xor	ebx, ebx

; 34   : 		a = a1 + (nCol * (a2-a1) / nEnd);

	sub	edx, DWORD PTR _a1$[ebp]
	mov	DWORD PTR -48+[ebp], esi
	mov	esi, DWORD PTR _nEnd$[ebp]
	mov	DWORD PTR -52+[ebp], eax
	mov	DWORD PTR -44+[ebp], ecx
	mov	DWORD PTR -40+[ebp], edx
$L55635:
	mov	eax, ebx
	cdq
	idiv	esi

; 35   : 		COLORREFA cr = DIB_RGBA( r, g, b, a );
; 36   : 		DrawLine( x, nTop, x, nBottom, cr );

	add	eax, DWORD PTR _a1$[ebp]
	shl	eax, 24					; 00000018H
	mov	ecx, eax
	mov	eax, DWORD PTR 8+[ebp]
	cdq
	idiv	esi
	add	eax, DWORD PTR _g1$[ebp]
	shl	eax, 8
	or	ecx, eax
	mov	eax, DWORD PTR 12+[ebp]
	cdq
	idiv	esi
	add	eax, DWORD PTR _b1$[ebp]
	shl	eax, 16					; 00000010H
	or	ecx, eax
	mov	eax, DWORD PTR 16+[ebp]
	cdq
	idiv	esi
	add	eax, DWORD PTR _r1$[ebp]
	or	ecx, eax
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	push	DWORD PTR -24+[ebp]
	push	edi
	push	DWORD PTR _nTop$[ebp]
	push	edi
	call	?DrawLine@CDIB@GS@@QAEXHHHHK@Z		; GS::CDIB::DrawLine
	mov	eax, DWORD PTR -44+[ebp]
	add	ebx, DWORD PTR -40+[ebp]
	add	DWORD PTR 8+[ebp], eax
	mov	eax, DWORD PTR -48+[ebp]
	add	DWORD PTR 12+[ebp], eax
	mov	eax, DWORD PTR -52+[ebp]
	add	DWORD PTR 16+[ebp], eax
	inc	edi
	cmp	edi, DWORD PTR -4+[ebp]
	jb	SHORT $L55635
$L55637:
	pop	edi
	pop	esi
	pop	ebx

; 37   : 	}
; 38   : }

	leave
	ret	12					; 0000000cH
?GradientFillLeftToRight@CDIB@GS@@QAEXABUtagRECT@@KK@Z ENDP ; GS::CDIB::GradientFillLeftToRight
_TEXT	ENDS
END
