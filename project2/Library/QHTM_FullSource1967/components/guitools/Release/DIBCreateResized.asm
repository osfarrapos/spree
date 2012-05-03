	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBCreateResized.cpp
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
PUBLIC	?CreateResized@CDIB@GS@@QBEPAV12@HH@Z		; GS::CDIB::CreateResized
PUBLIC	__real@4@4008fa00000000000000
PUBLIC	__real@4@3fff8000000000000000
EXTRN	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z:NEAR		; GS::CDIB::CDIB
EXTRN	?ClearToColour@CDIB@GS@@QAEXK@Z:NEAR		; GS::CDIB::ClearToColour
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	__ftol:NEAR
EXTRN	__fltused:NEAR
;	COMDAT __real@4@4008fa00000000000000
; File D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIB.h
CONST	SEGMENT
__real@4@4008fa00000000000000 DD 0447a0000r	; 1000
CONST	ENDS
;	COMDAT __real@4@3fff8000000000000000
CONST	SEGMENT
__real@4@3fff8000000000000000 DD 03f800000r	; 1
CONST	ENDS
_TEXT	SEGMENT
_newx$ = 8
_newy$ = 12
_this$ = -116
_pDib$ = -28
_xScale$ = -40
_yScale$ = -60
_fX$ = -92
_fY$ = -92
_pSourceImageData$ = -88
_dwEffectiveWidth$ = -100
_dwDestinationEffectiveWidth$ = -120
_ifX$55628 = -120
_ifY$55629 = -120
_xmax$55632 = -76
_ymax$55633 = -96
_ir1$55634 = -108
_ir2$55635 = -112
_ig1$55636 = -92
_ig2$55637 = -104
_ib1$55638 = -68
_ib2$55639 = -72
_ia1$55640 = -64
_ia2$55641 = -120
_dx$55642 = -32
_dy$55643 = -16
_rgb1$55648 = -24
_rgb2$55649 = -12
_rgb3$55650 = -84
_rgb4$55651 = -36
_y$55653 = -48
_DYMinus$55658 = -20
_x$55662 = -52
_DXMinus$55667 = -56
_i$55678 = -76
_x$55680 = -16
_y$55681 = -56
_pSource$55682 = -12
_pDest$55683 = -4
_naAccu$55685 = -8
_naCarry$55688 = -24
_nWeightX$55692 = -48
_nWeightY$55693 = -36
_fEndX$55694 = -20
_nScale$55695 = -84
_u$55699 = -72
_v$55700 = -80
_fEndY$55701 = -52
?CreateResized@CDIB@GS@@QBEPAV12@HH@Z PROC NEAR		; GS::CDIB::CreateResized

; 15   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 120				; 00000078H
	push	ebx
	push	esi
	push	edi

; 16   : 	/*
; 17   : 
; 18   : 		Most of this code was taken from CxImage and then heavily modified
; 19   : 		to allow the use of RGBQUAD and inline alpha channel (the way we do things)
; 20   : 
; 21   : 	*/
; 22   : 	HDC hdc = GetDC( NULL );

	xor	edi, edi
	mov	ebx, ecx
	push	edi
	mov	DWORD PTR _this$[ebp], ebx
	call	DWORD PTR __imp__GetDC@4

; 23   : 	CDIB * pDib = new CDIB( hdc, newx, newy );

	push	92					; 0000005cH
	mov	esi, eax
	call	??2@YAPAXI@Z				; operator new
	cmp	eax, edi
	pop	ecx
	je	SHORT $L55816
	push	DWORD PTR _newy$[ebp]
	mov	ecx, eax
	push	DWORD PTR _newx$[ebp]
	push	esi
	call	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z		; GS::CDIB::CDIB
	mov	DWORD PTR _pDib$[ebp], eax
	jmp	SHORT $L55817
$L55816:
	mov	DWORD PTR _pDib$[ebp], edi
$L55817:

; 24   : 	pDib->ClearToColour( DIB_RGBA( 255, 0, 0, 255 ) );

	mov	ecx, DWORD PTR _pDib$[ebp]
	push	-16776961				; ff0000ffH
	call	?ClearToColour@CDIB@GS@@QAEXK@Z		; GS::CDIB::ClearToColour

; 25   : 	::ReleaseDC( NULL, hdc );

	push	esi
	push	edi
	call	DWORD PTR __imp__ReleaseDC@8

; 26   : 
; 27   : 	float xScale, yScale, fX, fY;
; 28   : 	xScale = (float)GetWidth()  / (float)newx;
; 29   : 	yScale = (float)GetHeight() / (float)newy;

	mov	eax, DWORD PTR [ebx+80]
	mov	ecx, DWORD PTR [ebx+76]
	mov	DWORD PTR -84+[ebp], eax

; 30   : 
; 31   : 	LPBYTE pSourceImageData = (LPBYTE)m_pBits;

	mov	eax, DWORD PTR [ebx+8]
	mov	DWORD PTR _pSourceImageData$[ebp], eax

; 32   : 
; 33   : 	DWORD dwEffectiveWidth = ((((32 * GetWidth()) + 31) / 32) * BYTES_PER_PIXEL);

	mov	eax, ecx
	shl	eax, 5
	add	eax, 31					; 0000001fH
	push	32					; 00000020H
	mov	DWORD PTR -100+[ebp], ecx
	pop	esi
	fild	DWORD PTR -100+[ebp]
	cdq
	fstp	DWORD PTR -100+[ebp]
	fild	DWORD PTR _newx$[ebp]
	idiv	esi
	fstp	DWORD PTR -96+[ebp]
	fld	DWORD PTR -100+[ebp]
	fdiv	DWORD PTR -96+[ebp]

; 34   : 	DWORD dwDestinationEffectiveWidth = ((((32 * pDib->GetWidth()) + 31) / 32) * BYTES_PER_PIXEL);

	push	32					; 00000020H
	pop	edi
	fstp	DWORD PTR _xScale$[ebp]
	fild	DWORD PTR -84+[ebp]
	mov	esi, eax
	mov	eax, DWORD PTR _pDib$[ebp]
	fstp	DWORD PTR -100+[ebp]
	mov	eax, DWORD PTR [eax+76]
	fild	DWORD PTR _newy$[ebp]
	shl	eax, 5
	add	eax, 31					; 0000001fH
	fstp	DWORD PTR -96+[ebp]
	cdq
	fld	DWORD PTR -100+[ebp]
	idiv	edi
	fdiv	DWORD PTR -96+[ebp]

; 35   : 
; 36   : 	if( !( GetWidth() >newx && GetHeight()>newy ) )

	mov	edi, DWORD PTR _newx$[ebp]
	shl	esi, 2
	mov	DWORD PTR _dwEffectiveWidth$[ebp], esi
	fstp	DWORD PTR _yScale$[ebp]
	shl	eax, 2
	cmp	ecx, edi
	mov	DWORD PTR _dwDestinationEffectiveWidth$[ebp], eax
	jle	$L55627
	mov	eax, DWORD PTR -84+[ebp]
	cmp	eax, DWORD PTR _newy$[ebp]
	jle	$L55627

; 121  : 			}
; 122  : 		} 
; 123  : 	}
; 124  : 	else
; 125  : 	{
; 126  : 		//high resolution shrink, thanks to Henrik Stellmann <henrik.stellmann@volleynet.de>
; 127  : 		const long ACCURACY = 1000;
; 128  : 
; 129  : 		long i,j; // index for faValue
; 130  : 		long x,y; // coordinates in  source image
; 131  : 		BYTE* pSource;
; 132  : 		BYTE* pDest = (BYTE* )pDib->m_pBits;

	mov	eax, DWORD PTR _pDib$[ebp]

; 231  : 	}
; 232  : 
; 233  : 	return pDib;

	inc	edi
	shl	edi, 4
	mov	eax, DWORD PTR [eax+8]
	push	edi
	mov	DWORD PTR _pDest$55683[ebp], eax
	call	??2@YAPAXI@Z				; operator new
	push	edi
	mov	DWORD PTR _naAccu$55685[ebp], eax
	call	??2@YAPAXI@Z				; operator new
	fld	DWORD PTR _yScale$[ebp]
	fmul	DWORD PTR _xScale$[ebp]
	pop	ecx
	mov	DWORD PTR _naCarry$55688[ebp], eax
	pop	ecx
	fmul	DWORD PTR __real@4@4008fa00000000000000
	call	__ftol
	mov	edx, DWORD PTR _newx$[ebp]
	mov	edi, DWORD PTR _naAccu$55685[ebp]
	shl	edx, 4
	mov	ecx, edx
	mov	DWORD PTR _nScale$55695[ebp], eax
	mov	esi, ecx
	xor	eax, eax
	shr	ecx, 2
	rep stosd
	fld	DWORD PTR _yScale$[ebp]
	fsub	DWORD PTR __real@4@3fff8000000000000000
	mov	ecx, esi
	and	ecx, 3
	rep stosb
	fstp	DWORD PTR _fEndY$55701[ebp]
	mov	edi, DWORD PTR _naCarry$55688[ebp]
	mov	ecx, edx
	xor	eax, eax
	shr	ecx, 2
	rep stosd
	mov	ecx, edx
	and	ecx, 3
	rep stosb
	xor	ecx, ecx
	cmp	DWORD PTR [ebx+80], ecx
	mov	DWORD PTR _v$55700[ebp], ecx
	mov	DWORD PTR _y$55681[ebp], ecx
	jle	$L55704
	fld	DWORD PTR _xScale$[ebp]
	fsub	DWORD PTR __real@4@3fff8000000000000000
	mov	edi, DWORD PTR _pSourceImageData$[ebp]
	mov	DWORD PTR -92+[ebp], ecx
	mov	DWORD PTR -96+[ebp], edi
	mov	esi, 1000				; 000003e8H
	fstp	DWORD PTR -104+[ebp]
	jmp	SHORT $L55702
$L55915:

; 121  : 			}
; 122  : 		} 
; 123  : 	}
; 124  : 	else
; 125  : 	{
; 126  : 		//high resolution shrink, thanks to Henrik Stellmann <henrik.stellmann@volleynet.de>
; 127  : 		const long ACCURACY = 1000;
; 128  : 
; 129  : 		long i,j; // index for faValue
; 130  : 		long x,y; // coordinates in  source image
; 131  : 		BYTE* pSource;
; 132  : 		BYTE* pDest = (BYTE* )pDib->m_pBits;

	xor	ecx, ecx
$L55702:

; 147  : 		{
; 148  : 			pSource = pSourceImageData + y * dwEffectiveWidth;
; 149  : 			u = i = 0;
; 150  : 			fEndX = xScale - 1.0f;

	fld	DWORD PTR -104+[ebp]
	mov	DWORD PTR _pSource$55682[ebp], edi
	mov	DWORD PTR _i$55678[ebp], ecx
	fstp	DWORD PTR _fEndX$55694[ebp]

; 151  : 			if ((float)y < fEndY)

	fild	DWORD PTR _y$55681[ebp]
	mov	DWORD PTR _u$55699[ebp], ecx
	fstp	DWORD PTR -68+[ebp]
	fld	DWORD PTR -68+[ebp]
	fcomp	DWORD PTR _fEndY$55701[ebp]
	fnstsw	ax
	sahf
	jae	$L55706

; 152  : 			{       // complete source row goes into dest row
; 153  : 				for (x = 0; x < GetWidth(); x++)

	cmp	DWORD PTR [ebx+76], ecx
	mov	DWORD PTR _x$55680[ebp], ecx
	jle	$L55703
	mov	eax, DWORD PTR _naAccu$55685[ebp]
	mov	DWORD PTR -36+[ebp], eax
$L55707:

; 154  : 				{
; 155  : 					if ((float)x < fEndX)

	fild	DWORD PTR _x$55680[ebp]
	fstp	DWORD PTR -32+[ebp]
	fld	DWORD PTR -32+[ebp]
	fcomp	DWORD PTR _fEndX$55694[ebp]
	fnstsw	ax
	sahf
	jae	SHORT $L55711

; 156  : 					{       // complete source pixel goes into dest pixel
; 157  : 						for (j = 0; j < BYTES_PER_PIXEL; j++)

	mov	eax, DWORD PTR -36+[ebp]
	push	4
	pop	ecx
$L55712:

; 158  : 							naAccu[i + j] += (*pSource++) * ACCURACY;

	movzx	edx, BYTE PTR [edi]
	imul	edx, 1000				; 000003e8H
	add	DWORD PTR [eax], edx
	inc	edi
	add	eax, 4
	dec	ecx
	jne	SHORT $L55712

; 159  : 
; 160  : 					}
; 161  : 					else

	jmp	SHORT $L55708
$L55711:

; 162  : 					{       // source pixel is splitted for 2 dest pixels
; 163  : 						nWeightX = (long)(((float)x - fEndX) * ACCURACY);

	fld	DWORD PTR -32+[ebp]
	fsub	DWORD PTR _fEndX$55694[ebp]
	fmul	DWORD PTR __real@4@4008fa00000000000000
	call	__ftol
	mov	ecx, DWORD PTR -36+[ebp]

; 164  : 						for (j = 0; j < BYTES_PER_PIXEL; j++)
; 165  : 						{
; 166  : 							naAccu[i] += (ACCURACY - nWeightX) * (*pSource);

	mov	DWORD PTR -44+[ebp], esi
	sub	DWORD PTR -44+[ebp], eax
	add	DWORD PTR -36+[ebp], 16			; 00000010H
	mov	DWORD PTR _nWeightX$55692[ebp], eax
	mov	DWORD PTR -76+[ebp], 4
$L55718:
	movzx	edx, BYTE PTR [edi]
	mov	eax, DWORD PTR -44+[ebp]
	imul	eax, edx

; 167  : 							naAccu[BYTES_PER_PIXEL + i++] += nWeightX * (*pSource++);

	imul	edx, DWORD PTR _nWeightX$55692[ebp]
	add	DWORD PTR [ecx], eax
	add	DWORD PTR [ecx+16], edx
	inc	edi
	add	ecx, 4
	dec	DWORD PTR -76+[ebp]
	jne	SHORT $L55718

; 168  : 						}
; 169  : 						fEndX += xScale;

	fld	DWORD PTR _fEndX$55694[ebp]
	fadd	DWORD PTR _xScale$[ebp]
	fstp	DWORD PTR _fEndX$55694[ebp]
$L55708:
	inc	DWORD PTR _x$55680[ebp]
	mov	eax, DWORD PTR _x$55680[ebp]
	cmp	eax, DWORD PTR [ebx+76]
	jl	$L55707

; 170  : 						u++;
; 171  : 					}
; 172  : 				}
; 173  : 			}
; 174  : 			else

	jmp	$L55703
$L55706:

; 175  : 			{       // source row is splitted for 2 dest rows       
; 176  : 				nWeightY = (long)(((float)y - fEndY) * ACCURACY);

	fld	DWORD PTR -68+[ebp]
	fsub	DWORD PTR _fEndY$55701[ebp]
	fmul	DWORD PTR __real@4@4008fa00000000000000
	call	__ftol

; 177  : 
; 178  : 				for (x = 0; x < GetWidth(); x++)

	and	DWORD PTR _x$55680[ebp], 0
	cmp	DWORD PTR [ebx+76], 0
	mov	DWORD PTR _nWeightY$55693[ebp], eax
	jle	$L55726
	mov	eax, DWORD PTR _naCarry$55688[ebp]
	mov	DWORD PTR -44+[ebp], eax
	mov	eax, DWORD PTR _naAccu$55685[ebp]
	add	eax, 16					; 00000010H
	mov	DWORD PTR -64+[ebp], eax
$L55724:

; 179  : 				{
; 180  : 					if ((float)x < fEndX)

	fild	DWORD PTR _x$55680[ebp]
	fstp	DWORD PTR -32+[ebp]
	fld	DWORD PTR -32+[ebp]
	fcomp	DWORD PTR _fEndX$55694[ebp]
	fnstsw	ax
	sahf
	jae	SHORT $L55728

; 181  : 					{       // complete source pixel goes into 2 pixel
; 182  : 						for (j = 0; j < BYTES_PER_PIXEL; j++)
; 183  : 						{
; 184  : 							naAccu[i + j] += ((ACCURACY - nWeightY) * (*pSource));

	mov	eax, esi
	mov	ecx, DWORD PTR -44+[ebp]
	sub	eax, DWORD PTR _nWeightY$55693[ebp]
	mov	DWORD PTR -68+[ebp], 4
	mov	DWORD PTR -48+[ebp], eax
	mov	eax, DWORD PTR _naAccu$55685[ebp]
	sub	eax, DWORD PTR _naCarry$55688[ebp]
	mov	DWORD PTR -88+[ebp], eax
$L55729:
	movzx	eax, BYTE PTR [edi]
	mov	edi, DWORD PTR -48+[ebp]
	mov	edx, DWORD PTR -88+[ebp]
	imul	edi, eax

; 185  : 							naCarry[i + j] += nWeightY * (*pSource++);

	imul	eax, DWORD PTR _nWeightY$55693[ebp]
	add	DWORD PTR [edx+ecx], edi
	mov	edi, DWORD PTR _pSource$55682[ebp]
	add	DWORD PTR [ecx], eax
	add	edx, ecx
	inc	edi
	add	ecx, 4
	dec	DWORD PTR -68+[ebp]
	mov	DWORD PTR _pSource$55682[ebp], edi
	jne	SHORT $L55729

; 186  : 						}
; 187  : 					}
; 188  : 					else

	jmp	$L55725
$L55728:

; 189  : 					{       // source pixel is splitted for 4 dest pixels
; 190  : 						nWeightX = (int)(((float)x - fEndX) * ACCURACY);

	fld	DWORD PTR -32+[ebp]
	fsub	DWORD PTR _fEndX$55694[ebp]
	fmul	DWORD PTR __real@4@4008fa00000000000000
	call	__ftol

; 191  : 						for (j = 0; j < BYTES_PER_PIXEL; j++)
; 192  : 						{
; 193  : 							naAccu[i] += ((ACCURACY - nWeightY) * (ACCURACY - nWeightX)) * (*pSource) / ACCURACY;

	mov	DWORD PTR -32+[ebp], esi
	mov	DWORD PTR _nWeightX$55692[ebp], eax
	sub	DWORD PTR -32+[ebp], eax
	mov	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR -68+[ebp], eax
	mov	eax, DWORD PTR _naCarry$55688[ebp]
	sub	eax, DWORD PTR _naAccu$55685[ebp]
	mov	ecx, DWORD PTR -64+[ebp]
	add	DWORD PTR -44+[ebp], 16			; 00000010H
	mov	edi, esi
	sub	edi, DWORD PTR _nWeightY$55693[ebp]
	push	4
	mov	DWORD PTR -112+[ebp], eax
	pop	eax
	add	DWORD PTR _i$55678[ebp], eax
	mov	DWORD PTR -108+[ebp], edi
	imul	edi, DWORD PTR -32+[ebp]
	add	DWORD PTR -64+[ebp], 16			; 00000010H
	mov	DWORD PTR -88+[ebp], eax
$L55735:
	mov	eax, DWORD PTR _pSource$55682[ebp]
	mov	ebx, esi
	movzx	edx, BYTE PTR [eax]
	mov	eax, edi
	imul	eax, edx
	cdq
	idiv	ebx
	add	DWORD PTR [ecx-16], eax

; 194  : 							*pDest++ = (BYTE)(naAccu[i] / nScale);

	mov	eax, DWORD PTR [ecx-16]
	cdq
	idiv	DWORD PTR _nScale$55695[ebp]
	mov	edx, DWORD PTR _pDest$55683[ebp]
	inc	DWORD PTR _pDest$55683[ebp]
	mov	BYTE PTR [edx], al

; 195  : 							naCarry[i] += (nWeightY * (ACCURACY - nWeightX) * (*pSource)) / ACCURACY;

	mov	edx, DWORD PTR _pSource$55682[ebp]
	mov	eax, DWORD PTR -32+[ebp]
	movzx	edx, BYTE PTR [edx]
	imul	eax, edx
	imul	eax, DWORD PTR _nWeightY$55693[ebp]
	cdq
	idiv	ebx
	mov	edx, DWORD PTR -68+[ebp]

; 196  : 							naAccu[i + BYTES_PER_PIXEL] += ((ACCURACY - nWeightY) * nWeightX * (*pSource)) / ACCURACY;
; 197  : 							naCarry[i + BYTES_PER_PIXEL] = (nWeightY * nWeightX * (*pSource)) / ACCURACY;
; 198  : 							i++;

	add	DWORD PTR -68+[ebp], 4
	add	DWORD PTR [edx], eax
	mov	edx, DWORD PTR _pSource$55682[ebp]
	mov	eax, DWORD PTR -108+[ebp]
	movzx	edx, BYTE PTR [edx]
	imul	eax, edx
	imul	eax, DWORD PTR _nWeightX$55692[ebp]
	cdq
	idiv	ebx
	add	DWORD PTR [ecx], eax
	mov	eax, DWORD PTR _pSource$55682[ebp]
	movzx	eax, BYTE PTR [eax]
	imul	eax, DWORD PTR _nWeightY$55693[ebp]
	imul	eax, DWORD PTR _nWeightX$55692[ebp]
	cdq
	idiv	ebx
	mov	edx, DWORD PTR -112+[ebp]
	mov	DWORD PTR [edx+ecx], eax
	add	ecx, 4

; 199  : 							pSource++;

	inc	DWORD PTR _pSource$55682[ebp]
	dec	DWORD PTR -88+[ebp]
	jne	SHORT $L55735

; 200  : 						}
; 201  : 						fEndX += xScale;

	fld	DWORD PTR _fEndX$55694[ebp]
	fadd	DWORD PTR _xScale$[ebp]

; 202  : 						u++;

	inc	DWORD PTR _u$55699[ebp]
	mov	ebx, DWORD PTR _this$[ebp]
	mov	edi, DWORD PTR _pSource$55682[ebp]
	fstp	DWORD PTR _fEndX$55694[ebp]
$L55725:
	inc	DWORD PTR _x$55680[ebp]
	mov	eax, DWORD PTR _x$55680[ebp]
	cmp	eax, DWORD PTR [ebx+76]
	jl	$L55724
$L55726:

; 203  : 					}
; 204  : 				}
; 205  : 
; 206  : 
; 207  : 				if( u < newx )

	mov	eax, DWORD PTR _u$55699[ebp]
	cmp	eax, DWORD PTR _newx$[ebp]
	jge	SHORT $L55742

; 208  : 				{
; 209  : 					// possibly not completed due to rounding errors
; 210  : 					for (j = 0; j < BYTES_PER_PIXEL; j++)

	mov	eax, DWORD PTR _i$55678[ebp]
	mov	ecx, DWORD PTR _naAccu$55685[ebp]
	push	4
	lea	ecx, DWORD PTR [ecx+eax*4]
	pop	edi
$L55740:

; 211  : 						*pDest++ = (BYTE)(naAccu[i++] / nScale);

	mov	eax, DWORD PTR [ecx]
	add	ecx, 4
	cdq
	idiv	DWORD PTR _nScale$55695[ebp]
	mov	edx, DWORD PTR _pDest$55683[ebp]
	inc	DWORD PTR _pDest$55683[ebp]
	dec	edi
	mov	BYTE PTR [edx], al
	jne	SHORT $L55740
$L55742:

; 212  : 				}
; 213  : 
; 214  : 				naTemp = naCarry;

	mov	eax, DWORD PTR _naCarry$55688[ebp]

; 215  : 				naCarry = naAccu;

	mov	edi, DWORD PTR _naAccu$55685[ebp]

; 216  : 				naAccu = naTemp;

	mov	DWORD PTR _naAccu$55685[ebp], eax

; 217  : 				memset(naCarry, 0, sizeof(int) * BYTES_PER_PIXEL);    // need only to set first pixel zero

	xor	eax, eax
	mov	DWORD PTR _naCarry$55688[ebp], edi

; 218  : 				pDest = (BYTE* )pDib->m_pBits + (++v * dwDestinationEffectiveWidth );

	mov	ecx, DWORD PTR _pDib$[ebp]
	stosd

; 219  : 				fEndY += yScale;

	fld	DWORD PTR _fEndY$55701[ebp]
	fadd	DWORD PTR _yScale$[ebp]
	stosd
	stosd
	fstp	DWORD PTR _fEndY$55701[ebp]
	inc	DWORD PTR _v$55700[ebp]
	stosd
	mov	eax, DWORD PTR -92+[ebp]
	add	eax, DWORD PTR _dwDestinationEffectiveWidth$[ebp]
	mov	DWORD PTR -92+[ebp], eax
	add	eax, DWORD PTR [ecx+8]
	mov	DWORD PTR _pDest$55683[ebp], eax
$L55703:
	inc	DWORD PTR _y$55681[ebp]
	mov	edi, DWORD PTR -96+[ebp]
	add	edi, DWORD PTR _dwEffectiveWidth$[ebp]
	mov	eax, DWORD PTR _y$55681[ebp]
	cmp	eax, DWORD PTR [ebx+80]
	mov	DWORD PTR -96+[ebp], edi
	jl	$L55915

; 133  : 		long* naAccu  = new long[BYTES_PER_PIXEL * newx + BYTES_PER_PIXEL];
; 134  : 		long* naCarry = new long[BYTES_PER_PIXEL * newx + BYTES_PER_PIXEL];
; 135  : 		long* naTemp;
; 136  : 		long  nWeightX,nWeightY;
; 137  : 		float fEndX;
; 138  : 		long nScale = (long)(ACCURACY * xScale * yScale);
; 139  : 
; 140  : 		memset(naAccu,  0, sizeof(long) * BYTES_PER_PIXEL * newx);
; 141  : 		memset(naCarry, 0, sizeof(long) * BYTES_PER_PIXEL * newx);
; 142  : 
; 143  : 		int u, v = 0; // coordinates in dest image
; 144  : 		float fEndY = yScale - 1.0f;
; 145  : 
; 146  : 		for (y = 0; y < GetHeight(); y++)

	xor	ecx, ecx
$L55704:

; 220  : 			}
; 221  : 		}
; 222  : 
; 223  : 
; 224  : 		if (v < newy)

	mov	eax, DWORD PTR _v$55700[ebp]
	cmp	eax, DWORD PTR _newy$[ebp]
	jge	SHORT $L55749

; 225  : 		{	// possibly not completed due to rounding errors
; 226  : 			for (i = 0; i < BYTES_PER_PIXEL * newx; i++)

	mov	eax, DWORD PTR _newx$[ebp]
	shl	eax, 2
	cmp	eax, ecx
	jle	SHORT $L55749
	mov	ecx, DWORD PTR _naAccu$55685[ebp]
	mov	esi, eax
$L55747:

; 227  : 				*pDest++ = (BYTE)(naAccu[i] / nScale);

	mov	eax, DWORD PTR [ecx]
	add	ecx, 4
	cdq
	idiv	DWORD PTR _nScale$55695[ebp]
	mov	edx, DWORD PTR _pDest$55683[ebp]
	inc	DWORD PTR _pDest$55683[ebp]
	dec	esi
	mov	BYTE PTR [edx], al
	jne	SHORT $L55747
$L55749:

; 228  : 		}
; 229  : 		delete [] naAccu;

	push	DWORD PTR _naAccu$55685[ebp]
	call	??3@YAXPAX@Z				; operator delete

; 230  : 		delete [] naCarry;

	push	DWORD PTR _naCarry$55688[ebp]
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
	pop	ecx
	jmp	$L55656
$L55627:

; 37   : 	{
; 38   : 		//© 1999 Steve McMahon (steve@dogma.demon.co.uk)
; 39   : 		long ifX, ifY, ifX1, ifY1, xmax, ymax;
; 40   : 		float ir1, ir2, ig1, ig2, ib1, ib2, ia1, ia2, dx, dy;
; 41   : 
; 42   : 		BYTE r,g,b,a;
; 43   : 
; 44   : 		RGBQUAD rgb1, rgb2, rgb3, rgb4;
; 45   : 		xmax = GetWidth()-1;
; 46   : 		ymax = GetHeight()-1;

	mov	eax, DWORD PTR -84+[ebp]

; 47   : 
; 48   : 		const CLineArray & arrDest = pDib->GetLineArray();
; 49   : 
; 50   : 		for(long y=0; y<newy; y++)

	and	DWORD PTR _y$55653[ebp], 0
	dec	ecx
	dec	eax
	cmp	DWORD PTR _newy$[ebp], 0
	mov	DWORD PTR _xmax$55632[ebp], ecx
	mov	DWORD PTR _ymax$55633[ebp], eax
	jle	$L55656
	mov	eax, DWORD PTR _pDib$[ebp]
	mov	eax, DWORD PTR [eax+60]
	mov	DWORD PTR -80+[ebp], eax
$L55654:

; 51   : 		{
; 52   : 			fY = y * yScale;

	fild	DWORD PTR _y$55653[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _yScale$[ebp]
	fstp	DWORD PTR _fY$[ebp]

; 53   : 			ifY = (int)fY;

	fld	DWORD PTR _fY$[ebp]
	call	__ftol

; 54   : 			ifY1 = min(ymax, ifY+1);

	lea	ecx, DWORD PTR [eax+1]
	mov	DWORD PTR _ifY$55629[ebp], eax
	cmp	DWORD PTR _ymax$55633[ebp], ecx
	jge	SHORT $L55819
	mov	ecx, DWORD PTR _ymax$55633[ebp]
$L55819:

; 55   : 			dy = fY - ifY;

	fild	DWORD PTR _ifY$55629[ebp]

; 56   : 			float DYMinus = 1-dy;
; 57   : 			
; 58   : 			DIB_PIXEL *pPixelRow = arrDest[ y ];
; 59   : 			const long lYOffset = ifY * dwEffectiveWidth;

	imul	eax, esi
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR _fY$[ebp]
	fsub	DWORD PTR -120+[ebp]
	fstp	DWORD PTR _dy$55643[ebp]
	mov	edx, DWORD PTR -80+[ebp]

; 60   : 			const long lYOffset1 = ifY1 * dwEffectiveWidth;
; 61   : 			
; 62   : 			for(long x=0; x<newx; x++)

	and	DWORD PTR _x$55662[ebp], 0
	fld1
	imul	ecx, esi
	fsub	DWORD PTR _dy$55643[ebp]
	fstp	DWORD PTR _DYMinus$55658[ebp]
	cmp	DWORD PTR _newx$[ebp], 0
	mov	edi, DWORD PTR [edx]
	mov	ebx, eax
	jle	$L55655

; 79   : 
; 80   : 				pCurrentSource = pSourceImageData + lYOffset + lXOffset1;

	mov	edx, DWORD PTR _pSourceImageData$[ebp]

; 87   : 				rgb3.rgbBlue = *pCurrentSource++;
; 88   : 				rgb3.rgbGreen= *pCurrentSource++;
; 89   : 				rgb3.rgbRed =*pCurrentSource++;
; 90   : 				rgb3.rgbReserved =*pCurrentSource++;
; 91   : 
; 92   : 				pCurrentSource = pSourceImageData + lYOffset1 + lXOffset1;

	add	ecx, edx
	add	ebx, edx
	mov	DWORD PTR -116+[ebp], ecx
	inc	edi
$L55663:
	fild	DWORD PTR _x$55662[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _xScale$[ebp]
	fstp	DWORD PTR _fX$[ebp]
	fld	DWORD PTR _fX$[ebp]
	call	__ftol
	lea	ecx, DWORD PTR [eax+1]
	mov	DWORD PTR _ifX$55628[ebp], eax
	cmp	DWORD PTR _xmax$55632[ebp], ecx
	jge	SHORT $L55821

; 63   : 			{
; 64   : 				fX = x * xScale;
; 65   : 				ifX = (int)fX;
; 66   : 				ifX1 = min(xmax, ifX+1);

	mov	ecx, DWORD PTR _xmax$55632[ebp]
$L55821:

; 67   : 				dx = fX - ifX;
; 68   : 				float DXMinus = 1-dx;
; 69   : 
; 70   : 				BYTE* pCurrentSource;
; 71   : 				const long lXOffset = ifX*BYTES_PER_PIXEL;

	mov	edx, eax

; 72   : 				const long lXOffset1 = ifX1*BYTES_PER_PIXEL;

	mov	esi, ecx
	shl	edx, 2
	shl	esi, 2

; 73   : 
; 74   : 				pCurrentSource = pSourceImageData + lYOffset + lXOffset;
; 75   : 				rgb1.rgbBlue = *pCurrentSource++;

	mov	cl, BYTE PTR [ebx+edx]
	lea	eax, DWORD PTR [ebx+edx]
	inc	eax
	mov	BYTE PTR _rgb1$55648[ebp], cl
	fild	DWORD PTR _ifX$55628[ebp]

; 76   : 				rgb1.rgbGreen= *pCurrentSource++;

	mov	cl, BYTE PTR [eax]
	inc	eax
	mov	BYTE PTR _rgb1$55648[ebp+1], cl

; 77   : 				rgb1.rgbRed =*pCurrentSource++;

	mov	cl, BYTE PTR [eax]

; 78   : 				rgb1.rgbReserved =*pCurrentSource;

	mov	al, BYTE PTR [eax+1]
	mov	BYTE PTR _rgb1$55648[ebp+2], cl

; 79   : 
; 80   : 				pCurrentSource = pSourceImageData + lYOffset + lXOffset1;

	lea	ecx, DWORD PTR [ebx+esi]
	mov	BYTE PTR _rgb1$55648[ebp+3], al

; 81   : 				rgb2.rgbBlue = *pCurrentSource++;

	mov	al, BYTE PTR [ecx]
	inc	ecx
	mov	BYTE PTR _rgb2$55649[ebp], al

; 82   : 				rgb2.rgbGreen= *pCurrentSource++;

	mov	al, BYTE PTR [ecx]
	inc	ecx
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR _fX$[ebp]
	mov	BYTE PTR _rgb2$55649[ebp+1], al

; 83   : 				rgb2.rgbRed =*pCurrentSource++;

	mov	al, BYTE PTR [ecx]
	fsub	DWORD PTR -120+[ebp]
	mov	BYTE PTR _rgb2$55649[ebp+2], al

; 84   : 				rgb2.rgbReserved =*pCurrentSource++;

	mov	al, BYTE PTR [ecx+1]

; 85   : 
; 86   : 				pCurrentSource = pSourceImageData + lYOffset1 + lXOffset;

	mov	ecx, DWORD PTR -116+[ebp]
	mov	BYTE PTR _rgb2$55649[ebp+3], al
	add	edx, ecx

; 87   : 				rgb3.rgbBlue = *pCurrentSource++;
; 88   : 				rgb3.rgbGreen= *pCurrentSource++;
; 89   : 				rgb3.rgbRed =*pCurrentSource++;
; 90   : 				rgb3.rgbReserved =*pCurrentSource++;
; 91   : 
; 92   : 				pCurrentSource = pSourceImageData + lYOffset1 + lXOffset1;

	add	esi, ecx
	fstp	DWORD PTR _dx$55642[ebp]
	mov	al, BYTE PTR [edx]
	inc	edx
	mov	BYTE PTR _rgb3$55650[ebp], al

; 93   : 				rgb4.rgbBlue = *pCurrentSource++;

	mov	cl, BYTE PTR [esi]
	mov	al, BYTE PTR [edx]
	inc	edx
	fld1
	mov	BYTE PTR _rgb3$55650[ebp+1], al
	mov	al, BYTE PTR [edx]
	fsub	DWORD PTR _dx$55642[ebp]

; 94   : 				rgb4.rgbGreen= *pCurrentSource++;
; 95   : 				rgb4.rgbRed =*pCurrentSource++;
; 96   : 				rgb4.rgbReserved =*pCurrentSource++;
; 97   : 
; 98   : 				// Interplate in x direction:
; 99   : 				ir1 = rgb1.rgbRed   * DYMinus + rgb3.rgbRed   * dy;

	movzx	eax, al
	fstp	DWORD PTR _DXMinus$55667[ebp]
	mov	DWORD PTR -120+[ebp], eax
	inc	esi
	fild	DWORD PTR -120+[ebp]
	movzx	eax, BYTE PTR _rgb1$55648[ebp+2]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	mov	dl, BYTE PTR [edx+1]
	fild	DWORD PTR -120+[ebp]

; 100  : 				ig1 = rgb1.rgbGreen * DYMinus + rgb3.rgbGreen * dy;

	movzx	eax, BYTE PTR _rgb3$55650[ebp+1]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	mov	DWORD PTR -120+[ebp], eax
	mov	BYTE PTR _rgb4$55651[ebp], cl
	movzx	eax, BYTE PTR _rgb1$55648[ebp+1]
	faddp	ST(1), ST(0)
	mov	cl, BYTE PTR [esi]
	inc	esi
	fstp	DWORD PTR _ir1$55634[ebp]
	fild	DWORD PTR -120+[ebp]
	mov	BYTE PTR _rgb4$55651[ebp+1], cl
	mov	cl, BYTE PTR [esi]
	mov	BYTE PTR _rgb4$55651[ebp+2], cl
	mov	cl, BYTE PTR [esi+1]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	faddp	ST(1), ST(0)

; 101  : 				ib1 = rgb1.rgbBlue  * DYMinus + rgb3.rgbBlue  * dy;

	movzx	eax, BYTE PTR _rgb3$55650[ebp]
	fstp	DWORD PTR _ig1$55636[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]
	movzx	eax, BYTE PTR _rgb1$55648[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]

; 102  : 				ia1 = rgb1.rgbReserved * DYMinus + rgb3.rgbReserved * dy;

	movzx	eax, dl
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	mov	DWORD PTR -120+[ebp], eax
	movzx	eax, BYTE PTR _rgb1$55648[ebp+3]
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _ib1$55638[ebp]
	fild	DWORD PTR -120+[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]

; 103  : 
; 104  : 				ir2 = rgb2.rgbRed   * DYMinus + rgb4.rgbRed   * dy;

	movzx	eax, BYTE PTR _rgb4$55651[ebp+2]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	mov	DWORD PTR -120+[ebp], eax
	movzx	eax, BYTE PTR _rgb2$55649[ebp+2]
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _ia1$55640[ebp]
	fild	DWORD PTR -120+[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]

; 105  : 				ig2 = rgb2.rgbGreen * DYMinus + rgb4.rgbGreen * dy;

	movzx	eax, BYTE PTR _rgb4$55651[ebp+1]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	mov	DWORD PTR -120+[ebp], eax
	movzx	eax, BYTE PTR _rgb2$55649[ebp+1]
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _ir2$55635[ebp]
	fild	DWORD PTR -120+[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]

; 106  : 				ib2 = rgb2.rgbBlue  * DYMinus + rgb4.rgbBlue  * dy;

	movzx	eax, BYTE PTR _rgb4$55651[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	mov	DWORD PTR -120+[ebp], eax
	movzx	eax, BYTE PTR _rgb2$55649[ebp]
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _ig2$55637[ebp]
	fild	DWORD PTR -120+[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]

; 107  : 				ia2 = rgb2.rgbReserved * DYMinus + rgb4.rgbReserved * dy;

	movzx	eax, cl
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	mov	DWORD PTR -120+[ebp], eax
	movzx	eax, BYTE PTR _rgb2$55649[ebp+3]
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _ib2$55639[ebp]
	fild	DWORD PTR -120+[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _dy$55643[ebp]
	mov	DWORD PTR -120+[ebp], eax
	fild	DWORD PTR -120+[ebp]
	fstp	DWORD PTR -120+[ebp]
	fld	DWORD PTR -120+[ebp]
	fmul	DWORD PTR _DYMinus$55658[ebp]
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _ia2$55641[ebp]

; 108  : 
; 109  : 				// Interpolate in y:
; 110  : 				r = (BYTE)(ir1 * DXMinus + ir2 * dx);

	fld	DWORD PTR _ir2$55635[ebp]
	fmul	DWORD PTR _dx$55642[ebp]
	fld	DWORD PTR _ir1$55634[ebp]
	fmul	DWORD PTR _DXMinus$55667[ebp]
	faddp	ST(1), ST(0)

; 111  : 				g = (BYTE)(ig1 * DXMinus + ig2 * dx);
; 112  : 				b = (BYTE)(ib1 * DXMinus + ib2 * dx);
; 113  : 				a = (BYTE)(ia1 * DXMinus + ia2 * dx);
; 114  : 
; 115  : 				// Set output
; 116  : 				DIB_PIXEL *pPixel = pPixelRow + x;
; 117  : 				pPixel->rgbRed = r;

	call	__ftol
	fld	DWORD PTR _ig2$55637[ebp]
	fmul	DWORD PTR _dx$55642[ebp]
	fld	DWORD PTR _ig1$55636[ebp]
	fmul	DWORD PTR _DXMinus$55667[ebp]
	mov	BYTE PTR [edi+1], al
	faddp	ST(1), ST(0)

; 118  : 				pPixel->rgbGreen = g;

	call	__ftol
	fld	DWORD PTR _ib2$55639[ebp]
	fmul	DWORD PTR _dx$55642[ebp]
	fld	DWORD PTR _ib1$55638[ebp]
	fmul	DWORD PTR _DXMinus$55667[ebp]
	mov	BYTE PTR [edi], al
	faddp	ST(1), ST(0)

; 119  : 				pPixel->rgbBlue = b;

	call	__ftol
	fld	DWORD PTR _ia2$55641[ebp]
	fmul	DWORD PTR _dx$55642[ebp]
	fld	DWORD PTR _ia1$55640[ebp]
	fmul	DWORD PTR _DXMinus$55667[ebp]
	mov	BYTE PTR [edi-1], al
	faddp	ST(1), ST(0)

; 120  : 				pPixel->rgbReserved = a;

	call	__ftol
	inc	DWORD PTR _x$55662[ebp]
	mov	BYTE PTR [edi+2], al
	mov	eax, DWORD PTR _x$55662[ebp]
	add	edi, 4
	cmp	eax, DWORD PTR _newx$[ebp]
	jl	$L55663

; 60   : 			const long lYOffset1 = ifY1 * dwEffectiveWidth;
; 61   : 			
; 62   : 			for(long x=0; x<newx; x++)

	mov	esi, DWORD PTR _dwEffectiveWidth$[ebp]
$L55655:

; 47   : 
; 48   : 		const CLineArray & arrDest = pDib->GetLineArray();
; 49   : 
; 50   : 		for(long y=0; y<newy; y++)

	inc	DWORD PTR _y$55653[ebp]
	add	DWORD PTR -80+[ebp], 4
	mov	eax, DWORD PTR _y$55653[ebp]
	cmp	eax, DWORD PTR _newy$[ebp]
	jl	$L55654
$L55656:

; 231  : 	}
; 232  : 
; 233  : 	return pDib;

	mov	eax, DWORD PTR _pDib$[ebp]
	pop	edi
	pop	esi
	pop	ebx

; 234  : }

	leave
	ret	8
?CreateResized@CDIB@GS@@QBEPAV12@HH@Z ENDP		; GS::CDIB::CreateResized
_TEXT	ENDS
END
