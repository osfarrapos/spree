	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\RadialGradient.cpp
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
;	COMDAT ??0FontDef@GS@@QAE@PBDHH_N11E1@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MulDiv@WinHelper@@YAHHHH@Z
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
;	COMDAT ?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ
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
;	COMDAT ??1CBufferedDCDIB@GS@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCBufferedDCDIB@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CBufferedDC@GS@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCBufferedDC@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSafeHdc@CDrawContext@GS@@QAEPAUHDC__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectPen@CDrawContext@GS@@AAEXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectBrush@CDrawContext@GS@@AAEXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetClipRect@CDrawContext@GS@@QBEABVCRect@WinHelper@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectFont@CDrawContext@GS@@QAEXABUFontDef@2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScaleX@CDrawContext@GS@@QBEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScaleY@CDrawContext@GS@@QBEHH@Z
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
;	COMDAT ??_7CBufferedDC@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CBufferedDCDIB@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	__real@8@40008000000000000000
PUBLIC	?RadialGradient@GS@@YAXAAVCDIB@1@ABVCRect@WinHelper@@KKHHH@Z ; GS::RadialGradient
EXTRN	__ftol:NEAR
EXTRN	__fltused:NEAR
EXTRN	_pow:NEAR
EXTRN	_sqrt:NEAR
;	COMDAT __real@8@40008000000000000000
; File ../array.inl
CONST	SEGMENT
__real@8@40008000000000000000 DQ 04000000000000000r ; 2
CONST	ENDS
_TEXT	SEGMENT
_dib$ = 8
_rc$ = 12
_crFrom$ = 16
_crTo$ = 20
_nGradientX$ = 24
_nGradientY$ = 28
_nGradientRadius$ = 32
_nTop$ = -4
_nLeft$ = -8
_nR1$ = -24
_nG1$ = -28
_nB1$ = -32
_nA1$ = -20
_nR2$ = -40
_nG2$ = -44
_nB2$ = -48
_nA2$ = -36
_nGradient$ = 12
_nY$ = 16
_dPreY$56462 = -84
_nX$56463 = 20
?RadialGradient@GS@@YAXAAVCDIB@1@ABVCRect@WinHelper@@KKHHH@Z PROC NEAR ; GS::RadialGradient

; 13   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 84					; 00000054H

; 14   : 	const int nTop = max( rc.top, 0 );

	mov	eax, DWORD PTR _rc$[ebp]
	push	ebx
	xor	ebx, ebx
	push	edi
	mov	ecx, DWORD PTR [eax+4]
	cmp	ecx, ebx
	mov	DWORD PTR _nTop$[ebp], ecx
	jg	SHORT $L56581
	mov	DWORD PTR _nTop$[ebp], ebx
$L56581:

; 15   : 	register const int nBottom = min( rc.bottom, dib.GetHeight() );

	mov	edi, DWORD PTR _dib$[ebp]
	mov	edx, DWORD PTR [eax+12]
	mov	DWORD PTR -16+[ebp], edx
	mov	ecx, DWORD PTR [edi+80]
	cmp	edx, ecx
	jl	SHORT $L56594
	mov	DWORD PTR -16+[ebp], ecx
$L56594:

; 16   : 	const int nLeft = max( rc.left, 0 );

	mov	ecx, DWORD PTR [eax]
	cmp	ecx, ebx
	jle	SHORT $L56584
	mov	ebx, ecx
$L56584:

; 17   : 	register const int nRight = min( rc.right, dib.GetWidth() );

	mov	ecx, DWORD PTR [edi+76]
	mov	eax, DWORD PTR [eax+8]
	cmp	eax, ecx
	mov	DWORD PTR _nLeft$[ebp], ebx
	mov	DWORD PTR -12+[ebp], eax
	jl	SHORT $L56602
	mov	DWORD PTR -12+[ebp], ecx
$L56602:

; 18   : 
; 19   :   int nR1 = GetRValue(crTo);

	mov	eax, DWORD PTR _crTo$[ebp]
	movzx	ecx, al
	mov	DWORD PTR _nR1$[ebp], ecx

; 20   :   int nG1 = GetGValue(crTo);

	xor	ecx, ecx
	mov	cl, ah
	movzx	ecx, cl
	mov	DWORD PTR _nG1$[ebp], ecx

; 21   :   int nB1 = GetBValue(crTo);

	mov	ecx, eax
	shr	ecx, 16					; 00000010H
	movzx	ecx, cl

; 22   :   int nA1 = GetAValue(crTo);

	shr	eax, 24					; 00000018H
	mov	DWORD PTR _nA1$[ebp], eax

; 23   : 
; 24   :   int nR2 = GetRValue(crFrom);

	mov	eax, DWORD PTR _crFrom$[ebp]
	mov	DWORD PTR _nB1$[ebp], ecx
	movzx	ecx, al
	mov	DWORD PTR _nR2$[ebp], ecx

; 25   :   int nG2 = GetGValue(crFrom);

	xor	ecx, ecx
	mov	cl, ah
	movzx	ecx, cl
	mov	DWORD PTR _nG2$[ebp], ecx

; 26   :   int nB2 = GetBValue(crFrom);

	mov	ecx, eax
	shr	ecx, 16					; 00000010H

; 27   : 	int nA2 = GetAValue(crFrom);

	shr	eax, 24					; 00000018H
	movzx	ecx, cl
	mov	DWORD PTR _nA2$[ebp], eax

; 28   : 
; 29   :   int nR;
; 30   :   int nG;
; 31   :   int nB;
; 32   : 	int nA;
; 33   :   int nGradient;
; 34   : 
; 35   : 	const GS::CDIB::CLineArray & arrDest = dib.GetLineArray();
; 36   : 
; 37   :   for ( int nY = nTop; nY < nBottom; nY++)

	mov	eax, DWORD PTR _nTop$[ebp]
	cmp	eax, DWORD PTR -16+[ebp]
	mov	DWORD PTR _nB2$[ebp], ecx
	mov	DWORD PTR _nY$[ebp], eax
	jge	$L56460
	push	esi
	jmp	SHORT $L56458
$L56618:
	mov	ebx, DWORD PTR _nLeft$[ebp]
$L56458:

; 38   : 	{
; 39   : 		DIB_PIXEL *pDest = arrDest[ nY ];

	mov	ecx, DWORD PTR [edi+60]

; 40   : 		double dPreY = pow( nY - nTop - nGradientY, 2);

	fld	QWORD PTR __real@8@40008000000000000000
	mov	esi, DWORD PTR [ecx+eax*4]
	sub	eax, DWORD PTR _nTop$[ebp]
	push	ecx
	push	ecx
	sub	eax, DWORD PTR _nGradientY$[ebp]
	fstp	QWORD PTR [esp]
	mov	DWORD PTR 12+[ebp], eax
	push	ecx
	fild	DWORD PTR 12+[ebp]
	push	ecx
	fstp	QWORD PTR -68+[ebp]
	fld	QWORD PTR -68+[ebp]
	fstp	QWORD PTR [esp]
	call	_pow
	fstp	QWORD PTR _dPreY$56462[ebp]
	add	esp, 16					; 00000010H

; 41   : 		for ( int nX = nLeft; nX < nRight; nX++)

	cmp	ebx, DWORD PTR -12+[ebp]
	mov	DWORD PTR _nX$56463[ebp], ebx
	jge	$L56459

; 48   :       }
; 49   : 
; 50   :       nR = nR1 + ((nR2 - nR1) * nGradient / 100);
; 51   :       nG = nG1 + ((nG2 - nG1) * nGradient / 100);
; 52   :       nB = nB1 + ((nB2 - nB1) * nGradient / 100);
; 53   : 			nA = nA1 + ((nA2 - nA1) * nGradient / 100);

	mov	eax, DWORD PTR _nA2$[ebp]
	lea	esi, DWORD PTR [esi+ebx*4+1]
	sub	eax, DWORD PTR _nA1$[ebp]
	mov	DWORD PTR -52+[ebp], eax
	mov	eax, DWORD PTR _nR2$[ebp]
	sub	eax, DWORD PTR _nR1$[ebp]
	mov	DWORD PTR -56+[ebp], eax
	mov	eax, DWORD PTR _nG2$[ebp]
	sub	eax, DWORD PTR _nG1$[ebp]
	mov	DWORD PTR -60+[ebp], eax
	mov	eax, DWORD PTR _nB2$[ebp]
	sub	eax, DWORD PTR _nB1$[ebp]
	mov	DWORD PTR -64+[ebp], eax
$L56464:
	mov	eax, DWORD PTR _nX$56463[ebp]
	push	ecx
	sub	eax, DWORD PTR _nLeft$[ebp]
	push	ecx
	fld	QWORD PTR __real@8@40008000000000000000
	sub	eax, DWORD PTR _nGradientX$[ebp]
	fstp	QWORD PTR [esp]
	mov	DWORD PTR 12+[ebp], eax
	push	ecx
	fild	DWORD PTR 12+[ebp]
	push	ecx
	fstp	QWORD PTR -76+[ebp]
	fld	QWORD PTR -76+[ebp]
	fstp	QWORD PTR [esp]
	call	_pow
	fadd	QWORD PTR _dPreY$56462[ebp]
	add	esp, 8
	fstp	QWORD PTR [esp]
	call	_sqrt
	pop	ecx
	pop	ecx
	call	__ftol
	mov	ecx, eax
	mov	eax, DWORD PTR _nGradientRadius$[ebp]
	sub	eax, ecx
	imul	eax, 100				; 00000064H
	cdq
	idiv	DWORD PTR _nGradientRadius$[ebp]
	test	eax, eax
	mov	DWORD PTR _nGradient$[ebp], eax
	jge	SHORT $L56468

; 42   : 		{
; 43   :       nGradient = ((nGradientRadius - (int)sqrt( pow(nX - nLeft - nGradientX, 2) + dPreY )) * 100) / nGradientRadius;
; 44   : 
; 45   :       if( nGradient < 0 )
; 46   : 			{
; 47   :           nGradient = 0;

	and	DWORD PTR _nGradient$[ebp], 0
$L56468:

; 48   :       }
; 49   : 
; 50   :       nR = nR1 + ((nR2 - nR1) * nGradient / 100);
; 51   :       nG = nG1 + ((nG2 - nG1) * nGradient / 100);
; 52   :       nB = nB1 + ((nB2 - nB1) * nGradient / 100);
; 53   : 			nA = nA1 + ((nA2 - nA1) * nGradient / 100);

	mov	eax, DWORD PTR -52+[ebp]
	push	100					; 00000064H
	imul	eax, DWORD PTR _nGradient$[ebp]
	cdq
	pop	ecx

; 54   : 			const int nNegativeAplha = 255 - nA;

	mov	edi, 255				; 000000ffH
	idiv	ecx
	push	100					; 00000064H
	pop	ebx
	mov	ecx, eax
	mov	eax, DWORD PTR -56+[ebp]
	imul	eax, DWORD PTR _nGradient$[ebp]
	cdq
	idiv	ebx
	add	ecx, DWORD PTR _nA1$[ebp]

; 55   : 
; 56   :       //dibEntireDisplay.PutPixelBlend( nX, nY, nR, nG, nB, nA );
; 57   : 			DIB_PIXEL *pPixel = pDest + nX;
; 58   : 			pPixel->rgbRed = static_cast<unsigned char>( ( pPixel->rgbRed * nNegativeAplha + nR * nA ) >>8 );

	movzx	edx, BYTE PTR [esi+1]
	sub	edi, ecx
	imul	edx, edi
	add	eax, DWORD PTR _nR1$[ebp]
	imul	eax, ecx
	add	eax, edx
	sar	eax, 8
	mov	BYTE PTR [esi+1], al
	mov	eax, DWORD PTR -60+[ebp]
	imul	eax, DWORD PTR _nGradient$[ebp]
	cdq
	idiv	ebx

; 59   : 			pPixel->rgbGreen = static_cast<unsigned char>( ( pPixel->rgbGreen * nNegativeAplha + nG * nA ) >>8 );

	movzx	edx, BYTE PTR [esi]
	imul	edx, edi
	add	eax, DWORD PTR _nG1$[ebp]
	imul	eax, ecx
	add	eax, edx
	sar	eax, 8
	mov	BYTE PTR [esi], al
	mov	eax, DWORD PTR -64+[ebp]
	imul	eax, DWORD PTR _nGradient$[ebp]
	cdq
	idiv	ebx
	add	esi, 4
	add	eax, DWORD PTR _nB1$[ebp]

; 60   : 			pPixel->rgbBlue = static_cast<unsigned char>( ( pPixel->rgbBlue * nNegativeAplha + nB * nA ) >>8 );

	imul	eax, ecx
	movzx	ecx, BYTE PTR [esi-5]
	imul	ecx, edi
	add	eax, ecx
	sar	eax, 8
	inc	DWORD PTR _nX$56463[ebp]
	mov	BYTE PTR [esi-5], al
	mov	eax, DWORD PTR _nX$56463[ebp]
	cmp	eax, DWORD PTR -12+[ebp]
	jl	$L56464

; 41   : 		for ( int nX = nLeft; nX < nRight; nX++)

	mov	edi, DWORD PTR _dib$[ebp]
$L56459:

; 28   : 
; 29   :   int nR;
; 30   :   int nG;
; 31   :   int nB;
; 32   : 	int nA;
; 33   :   int nGradient;
; 34   : 
; 35   : 	const GS::CDIB::CLineArray & arrDest = dib.GetLineArray();
; 36   : 
; 37   :   for ( int nY = nTop; nY < nBottom; nY++)

	mov	eax, DWORD PTR _nY$[ebp]
	inc	eax
	cmp	eax, DWORD PTR -16+[ebp]
	mov	DWORD PTR _nY$[ebp], eax
	jl	$L56618
	pop	esi
$L56460:
	pop	edi
	pop	ebx

; 61   :     }
; 62   :   }
; 63   : }

	leave
	ret	0
?RadialGradient@GS@@YAXAAVCDIB@1@ABVCRect@WinHelper@@KKHHH@Z ENDP ; GS::RadialGradient
_TEXT	ENDS
END
