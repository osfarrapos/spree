	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBDrawLine.cpp
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
PUBLIC	?DrawLine@CDIB@GS@@QAEXHHHHK@Z			; GS::CDIB::DrawLine
EXTRN	?PutPixel@CDIB@GS@@QAEXIIEEEE@Z:NEAR		; GS::CDIB::PutPixel
_TEXT	SEGMENT
_x1$ = 8
_y1$ = 12
_x2$ = 16
_y2$ = 20
_cr$ = 24
_this$ = -32
_x$ = -12
_y$ = 8
_xinc1$ = 16
_xinc2$ = 12
_yinc1$ = 20
_yinc2$ = -4
_numadd$ = -8
_g$ = -28
_b$ = -24
_a$ = -20
?DrawLine@CDIB@GS@@QAEXHHHHK@Z PROC NEAR		; GS::CDIB::DrawLine

; 15   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H

; 16   : 	int deltax = abs(x2 - x1);        // The difference between the x's

	mov	eax, DWORD PTR _x2$[ebp]
	push	ebx
	mov	ebx, DWORD PTR _x1$[ebp]
	mov	DWORD PTR _this$[ebp], ecx
	sub	eax, ebx
	push	esi

; 17   : 	int deltay = abs(y2 - y1);        // The difference between the y's

	mov	esi, DWORD PTR _y1$[ebp]
	push	edi
	cdq
	mov	ecx, eax
	mov	eax, DWORD PTR _y2$[ebp]
	xor	ecx, edx
	sub	eax, esi
	sub	ecx, edx

; 18   : 	int x = x1;                       // Start x off at the first pixel

	mov	DWORD PTR _x$[ebp], ebx
	cdq
	mov	edi, eax

; 19   : 	int y = y1;                       // Start y off at the first pixel
; 20   : 	int xinc1, xinc2;
; 21   : 	int yinc1, yinc2;
; 22   : 	int den;
; 23   : 	int num;
; 24   : 	int numadd;
; 25   : 	int numpixels;
; 26   : 
; 27   : 	if (x2 >= x1)                 // The x-values are increasing

	or	eax, -1
	xor	edi, edx
	mov	DWORD PTR _y$[ebp], esi
	sub	edi, edx
	cmp	DWORD PTR _x2$[ebp], ebx
	jl	SHORT $L55624

; 28   : 	{
; 29   : 		xinc1 = 1;

	push	1
	pop	edx
	mov	DWORD PTR _xinc1$[ebp], edx

; 30   : 		xinc2 = 1;

	mov	DWORD PTR _xinc2$[ebp], edx

; 31   : 	}
; 32   : 	else                          // The x-values are decreasing

	jmp	SHORT $L55625
$L55624:

; 33   : 	{
; 34   : 		xinc1 = -1;
; 35   : 		xinc2 = -1;

	push	1
	mov	DWORD PTR _xinc1$[ebp], eax
	mov	DWORD PTR _xinc2$[ebp], eax
	pop	edx
$L55625:

; 36   : 	}
; 37   : 
; 38   : 	if (y2 >= y1)                 // The y-values are increasing

	cmp	DWORD PTR _y2$[ebp], esi
	jl	SHORT $L55626

; 39   : 	{
; 40   : 		yinc1 = 1;

	mov	DWORD PTR _yinc1$[ebp], edx

; 41   : 		yinc2 = 1;

	mov	DWORD PTR _yinc2$[ebp], edx

; 42   : 	}
; 43   : 	else                          // The y-values are decreasing

	jmp	SHORT $L55627
$L55626:

; 44   : 	{
; 45   : 		yinc1 = -1;

	mov	DWORD PTR _yinc1$[ebp], eax

; 46   : 		yinc2 = -1;

	mov	DWORD PTR _yinc2$[ebp], eax
$L55627:

; 47   : 	}
; 48   : 
; 49   : 	if (deltax >= deltay)         // There is at least one x-value for every y-value

	cmp	ecx, edi
	jl	SHORT $L55628

; 50   : 	{
; 51   : 		xinc1 = 0;                  // Don't change the x when numerator >= denominator

	and	DWORD PTR _xinc1$[ebp], 0

; 52   : 		yinc2 = 0;                  // Don't change the y for every iteration

	and	DWORD PTR _yinc2$[ebp], 0

; 53   : 		den = deltax;
; 54   : 		num = deltax / 2;

	mov	eax, ecx
	mov	ebx, ecx
	cdq
	sub	eax, edx

; 55   : 		numadd = deltay;

	mov	DWORD PTR _numadd$[ebp], edi
	mov	esi, eax
	sar	esi, 1

; 56   : 		numpixels = deltax;         // There are more x-values than y-values
; 57   : 	}
; 58   : 	else                          // There is at least one y-value for every x-value

	jmp	SHORT $L55629
$L55628:

; 59   : 	{
; 60   : 		xinc2 = 0;                  // Don't change the x for every iteration

	and	DWORD PTR _xinc2$[ebp], 0

; 61   : 		yinc1 = 0;                  // Don't change the y when numerator >= denominator

	and	DWORD PTR _yinc1$[ebp], 0

; 62   : 		den = deltay;
; 63   : 		num = deltay / 2;

	mov	eax, edi

; 64   : 		numadd = deltax;

	mov	DWORD PTR _numadd$[ebp], ecx
	cdq
	sub	eax, edx
	mov	ebx, edi
	mov	esi, eax

; 65   : 		numpixels = deltay;         // There are more y-values than x-values

	mov	ecx, edi
	sar	esi, 1
$L55629:

; 66   : 	}
; 67   : 
; 68   : 	BYTE r = GetRValue( cr );
; 69   : 	BYTE g = GetGValue( cr );

	xor	eax, eax
	mov	al, BYTE PTR _cr$[ebp+1]
	mov	BYTE PTR _g$[ebp], al

; 70   : 	BYTE b = GetBValue( cr );

	mov	eax, DWORD PTR _cr$[ebp]
	shr	eax, 16					; 00000010H
	mov	BYTE PTR _b$[ebp], al

; 71   : 	BYTE a = GetAValue( cr );

	mov	eax, DWORD PTR _cr$[ebp]
	shr	eax, 24					; 00000018H

; 72   : 	int curpixel;
; 73   : 	for (curpixel = 0; curpixel <= numpixels; curpixel++)

	test	ecx, ecx
	mov	BYTE PTR _a$[ebp], al
	jl	SHORT $L55642
	inc	ecx
	mov	DWORD PTR -16+[ebp], ecx
$L55640:

; 74   : 	{
; 75   : 		PutPixel(x, y, r, g, b, a );             // Draw the current pixel

	push	DWORD PTR _a$[ebp]
	mov	edi, DWORD PTR _x$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	push	DWORD PTR _b$[ebp]
	push	DWORD PTR _g$[ebp]
	push	DWORD PTR _cr$[ebp]
	push	DWORD PTR _y$[ebp]
	push	edi
	call	?PutPixel@CDIB@GS@@QAEXIIEEEE@Z		; GS::CDIB::PutPixel

; 76   : 		num += numadd;              // Increase the numerator by the top of the fraction

	add	esi, DWORD PTR _numadd$[ebp]

; 77   : 		if (num >= den)             // Check if numerator >= denominator

	cmp	esi, ebx
	jl	SHORT $L55700

; 78   : 		{
; 79   : 			num -= den;               // Calculate the new numerator value
; 80   : 			x += xinc1;               // Change the x as appropriate
; 81   : 			y += yinc1;               // Change the y as appropriate

	mov	eax, DWORD PTR _y$[ebp]
	add	edi, DWORD PTR _xinc1$[ebp]
	sub	esi, ebx
	add	eax, DWORD PTR _yinc1$[ebp]
	jmp	SHORT $L55643
$L55700:
	mov	eax, DWORD PTR _y$[ebp]
$L55643:

; 82   : 		}
; 83   : 		x += xinc2;                 // Change the x as appropriate

	add	edi, DWORD PTR _xinc2$[ebp]

; 84   : 		y += yinc2;                 // Change the y as appropriate

	add	eax, DWORD PTR _yinc2$[ebp]
	dec	DWORD PTR -16+[ebp]
	mov	DWORD PTR _x$[ebp], edi
	mov	DWORD PTR _y$[ebp], eax
	jne	SHORT $L55640
$L55642:
	pop	edi
	pop	esi
	pop	ebx

; 85   : 
; 86   : 	}
; 87   : }

	leave
	ret	20					; 00000014H
?DrawLine@CDIB@GS@@QAEXHHHHK@Z ENDP			; GS::CDIB::DrawLine
_TEXT	ENDS
END
